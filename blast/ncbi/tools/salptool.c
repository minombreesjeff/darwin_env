static char const rcsid[] = "$Id: salptool.c,v 6.40 2005/04/13 14:56:26 kans Exp $";

#include <sequtil.h> /* SeqIdDupList */
#include <salpedit.h>
#include <salptool.h>
#include <salpacc.h>
#include <blast.h>
#include <simutil.h>
#include <alignval.h>
#include <blastpri.h>
#include <txalign.h>
#include <sqnutils.h>
#include <satutil.h>
#include <alignmgr.h>
#include <alignmgr2.h>
#include <spidey.h>
#include <salsap.h>

#define MIN_SEG_SIZE 10 /* used by MergeTwoDspBySIM4 , check_align_match_diagnol<-ModFilterDenseSegAlign<-ModifyAlignList<-SeqAlignConsistentDiagFilter */


/* Blast can now (2/1999) handle longer queries more efficiently.. though
 there is still a hardcoded limit to the number of HSP's */


#define PERCENT_A_RES	90 /* used by get_polyA_index<-check_polyA_tail<-SeqLocTrimPolyAtail<-compute_alignment  */


#define MAX_HANG 100 /* used in need_recompute<-compute_alignment */

#define MAX_EXTEND_OVERHANG	50 /* used by compute_alignment and SeqAlignSetFlobalFromLocal */

static Boolean is_bad_align (BioseqPtr m_bsp, BioseqPtr s_bsp, SeqAlignPtr align, Int4 min_align_len,Int4 loc_len);

static void OrderInt4(Int4Ptr x, Int4Ptr y) /* replace jzmisc: swap */
{
  Int4 temp;

	if((*x) > (*y)){
	  temp = *x;
	  *x = *y;
	  *y = temp;
	}
}

static void SeqAnnotWrite(SeqAlignPtr align)
{
        SeqAnnotPtr annot;
        AsnIoPtr aip;

        annot = SeqAnnotNew();
        annot->type = 2;
        annot->data = align;

        aip = AsnIoOpen("temp2.sat", "w");
        SeqAnnotAsnWrite(annot, aip, NULL);
        AsnIoClose(aip);

        annot->data = NULL;
        SeqAnnotFree(annot);
}

static Boolean check_align_match_diagnol(SeqAlignPtr align, ValNodePtr ddp_list, Uint1 strand)
{
	DenseSegPtr dsp;
	Int2 i;
	Int4 m_start, s_start;
	Boolean minus;
	Int4 total_len, match_len;
	ValNodePtr curr;
	DenseDiagPtr ddp;

	/*check for orientations first*/
	dsp = (DenseSegPtr) align->segs;
	minus = FALSE;
	if(dsp->strands !=NULL)
	{
		if(dsp->strands[0] != dsp->strands[1])
		{
			if(dsp->strands[1] == Seq_strand_minus  || 
				dsp->strands[0] == Seq_strand_minus)
				minus = TRUE;
		}
	}

	if(strand == Seq_strand_minus && !minus)
		return FALSE;
	if(minus && strand != Seq_strand_minus)
		return FALSE;

	total_len = 0;
	match_len = 0;
	for(i = 0; i<dsp->numseg; ++i)
	{
		m_start = dsp->starts[2*i];
		s_start = dsp->starts[2*i+1];
		if(m_start != -1 && s_start != -1 && dsp->lens[i] > MIN_SEG_SIZE)
		{
			total_len += dsp->lens[i];
			for(curr = ddp_list; curr != NULL; curr = curr->next)
			{
				ddp = (DenseDiagPtr) curr->data.ptrvalue;
				if(ddp->starts[0] == m_start 
					&& ddp->starts[1] == s_start)
				{
					match_len += dsp->lens[i];
					break;
				}
			}
		}
	}

	if(match_len < 2*MIN_SEG_SIZE || total_len < 2*MIN_SEG_SIZE)
		return FALSE;

	if(match_len == total_len)
		return TRUE;

	/*at least 40% need to be matching the fragment*/
	if(match_len > 4 * MIN_SEG_SIZE)
	{
		if((match_len *100 )/40 >= total_len)
			return TRUE;
	}
	else if(total_len < 4* MIN_SEG_SIZE)
		if((match_len *100 )/80 >= total_len)
			return TRUE;

	return FALSE;
}

				
			
/*
*
*	Filter the Dense-seg alignment. Get rid of the alignments that were not 
*	in the main diagnol
*	all the alignments MUST be for the SAME sequences
*	sip is the id of the sequence that is used for the primary ordering
*/
static Boolean ModFilterDenseSegAlign(SeqAlignPtr PNTR align, SeqIdPtr sip)
{
	SeqAlignPtr ddp_align;
	ValNodePtr head = NULL;
	SeqAlignPtr curr, prev, next;
	Uint1 strand;
        /* 	DenseSegPtr dsp; */
	BioseqPtr bsp;

	if(align == NULL || *align == NULL || sip == NULL)
		return FALSE;
	/*check if it has more than two alignments*/
	curr = *align;
	if(curr->segtype != 2)
		return FALSE;
	/* dsp = curr->segs;
	if(dsp->ids->next->choice == SEQID_LOCAL)
	{
           	SeqIdPtr t_sip;
          	ObjectIdPtr oip;
		t_sip = dsp->ids->next;
		oip = t_sip->data.ptrvalue;
		printf("%s\n", oip->str);
	} */
	

	ddp_align = SeqAlignConvertDspToDdpList(*align);
	if(ddp_align == NULL)
		return FALSE;

	bsp = BioseqLockById(sip);
	if(bsp == NULL)
		return FALSE;
	head = FilterSeqAlign(ddp_align, sip, &strand);
	BioseqUnlock(bsp);
	if(head == NULL)
	{
		SeqAlignFree(ddp_align);
		return FALSE;
	}

	curr = *align;
	prev = NULL;
	while(curr)
	{
		next = curr->next;
		if(!check_align_match_diagnol(curr, head, strand))
		{
			if(prev == NULL)
				*align = next;
			else
				prev->next = next;
			curr->next = NULL;
			SeqAlignFree(curr);
		}
		else
			prev = curr;
		curr = next;
	}

	ValNodeFree(head);
	SeqAlignFree(ddp_align);

	return TRUE;
}

static Boolean reload_seq_loc(SeqLocPtr slp, Int4 from, Int4 to, Uint1 strand)
{
	SeqIntPtr sint;

	if(slp == NULL || slp->choice != SEQLOC_INT)
		return FALSE;
	sint = (SeqIntPtr) slp->data.ptrvalue;
	sint->from = from;
	sint->to = to;
	sint->strand = strand;

	return TRUE;
}
/*
*
*	modify the locations on loc_1 and loc_2 to record the from, to for the 
*	end segment. if (head), return the first segment >= min_seg_len, else 
*	return the last segment
*/
static Boolean get_end_seg(DenseSegPtr dsp, Int4 min_seg_len, SeqLocPtr loc_1, SeqLocPtr loc_2, Boolean head)
{
	Int4 i;
	Int4 start_1, start_2;

	if(dsp == NULL || loc_1 == NULL || loc_2 == NULL)
		return FALSE;
	if(head)
		i = 0;
	else
		i = dsp->numseg -1;

	for(; head? i<dsp->numseg : i>=0; head? ++i: --i)
	{
		start_1 = dsp->starts[2*i];
		start_2 = dsp->starts[2*i+1];
		if(start_1 != -1 && start_2 != -1)
		{
			if(dsp->lens[i] >= min_seg_len)
			{
				reload_seq_loc(loc_1, start_1, 
					start_1+dsp->lens[i]-1, dsp->strands[0]);
				reload_seq_loc(loc_2, start_2, 
					start_2+dsp->lens[i]-1, dsp->strands[1]);

				return TRUE;
			}
		}
	}

	return FALSE;
}


/*
*
*	align_1 and align_2 will be  re-sorted by the ascending order in the first 
*	sequence in the alignment. The first sequence needs to aligned 
*	in the plus strand. They have to be alignment on the same 
*	diagnol
*	return NULL if any merge fails
*
*/
static SeqAlignPtr MergeTwoDspBySIM4(SeqAlignPtr align_1, SeqAlignPtr align_2,Int4 MaxGap)
{
	SeqLocPtr loc_1_1, loc_1_2, loc_2_1, loc_2_2;
	Int4 start, stop;
	Int4 match_start, match_stop;
	Boolean found = FALSE;
	BioseqPtr bsp;
	SeqAlignPtr align, t_align;
	DenseSegPtr dsp_1, dsp_2;
	BioseqPtr bsp_1, bsp_2;
	Int4 max_align_size;
	Uint1 strand;
	Int4 gap=0,gap_1, gap_2;
	Int4 start_1, stop_1, start_2, stop_2;
	SeqIdPtr sip;
	Boolean reverse = FALSE;
	Boolean has_overlap;
	Int2 order;

	if(align_1 == NULL || align_2 == NULL)
		return NULL;

	if(align_1->segtype != 2 || align_2->segtype != 2)
		return NULL;
        if(SeqAlignStart(align_1,0)>SeqAlignStart(align_2,0)) {
            VoidPtr segs;
            segs = align_1->segs;
            align_1->segs = align_2->segs;
            align_2->segs = segs;
        }

	dsp_1 = (DenseSegPtr) align_1->segs;
	if(dsp_1 && dsp_1->strands &&  dsp_1->strands[0] == Seq_strand_minus)
	{
		SeqAlignReverse(align_1, 0);
		reverse = TRUE;
	}
	dsp_2 = (DenseSegPtr) align_2->segs;
	if(dsp_2 && dsp_2->strands &&  dsp_2->strands[0] == Seq_strand_minus)
	{
		SeqAlignReverse(align_2, 0);
		reverse = TRUE;
	}

	max_align_size = MIN(SeqAlignLength(align_1), 
		SeqAlignLength(align_2));

	if(dsp_1 == NULL || dsp_2 == NULL)
		return NULL;

	SeqAlignStartStopById(align_1, dsp_1->ids, &start_1, &stop_1, &strand); 
	SeqAlignStartStopById(align_2, dsp_1->ids, &start_2, &stop_2, &strand); 
        /* Redundant alignment covering same region of query */
	if(start_2 >= start_1 && stop_2 <= stop_1)
		return NULL;

	SeqAlignStartStopById(align_1, dsp_1->ids->next, &start_1, &stop_1, &strand); 
	SeqAlignStartStopById(align_2, dsp_1->ids->next, &start_2, &stop_2, &strand); 
        /* Redundant alignment covering same region of subject */
	if(start_2 >= start_1 && stop_2 <= stop_1)
		return NULL;

	SeqAlignStartStopById(align_1, dsp_1->ids, &start, &stop, &strand); 
	loc_1_1 = SeqLocIntNew(start, stop, strand, dsp_1->ids);
	SeqAlignStartStopById(align_1, dsp_1->ids->next, &start, &stop, &strand); 
	loc_1_2 = SeqLocIntNew(start, stop, strand, dsp_1->ids->next);

	/* loc_1_1 = SeqLocIntNew(0, -1, 0, dsp_1->ids);
	loc_1_2 = SeqLocIntNew(0, -1, 0, dsp_1->ids->next);
	if(!get_end_seg(dsp_1, MIN_SEG_SIZE*2, loc_1_1, loc_1_2, FALSE))
	{
		SeqLocFree(loc_1_1);
		SeqLocFree(loc_1_2);
		return NULL;
	} */

	loc_2_1 = SeqLocIntNew(0, -1, 0, dsp_1->ids);
	loc_2_2 = SeqLocIntNew(0, -1, 0, dsp_1->ids->next);
	if(!get_end_seg(dsp_2, MIN_SEG_SIZE*2, loc_2_1, loc_2_2, TRUE))
	{
		SeqLocFree(loc_1_1);
		SeqLocFree(loc_1_2);
		SeqLocFree(loc_2_1);
		SeqLocFree(loc_2_2);
		return NULL;
	}

	found = FALSE;
	has_overlap = FALSE;
	if(select_overlap_loc(loc_1_1, loc_2_1, loc_1_2, loc_2_2, &order))
	{
		if(order == 0)
			is_loc_overlap(loc_1_1, loc_2_1, &start, &stop, 0, max_align_size, NULL);
		else
			is_loc_overlap(loc_1_2, loc_2_2, &start, &stop, 0, max_align_size, NULL);
		found = TRUE;
		has_overlap = TRUE;
	}

	if(!found)
	{
		SeqLocFree(loc_1_1);
		SeqLocFree(loc_1_2);
		loc_1_1 = SeqLocIntNew(0, -1, 0, dsp_1->ids);
		loc_1_2 = SeqLocIntNew(0, -1, 0, dsp_1->ids->next);
		if(!get_end_seg(dsp_1, MIN_SEG_SIZE*2, loc_1_1, loc_1_2, FALSE))
		{
			SeqLocFree(loc_1_1);
			SeqLocFree(loc_1_2);
			SeqLocFree(loc_2_1);
			SeqLocFree(loc_2_2);
			return NULL;
		} 
		/*check for the overlap between the two segments*/
		if(is_loc_overlap(loc_1_1, loc_2_1, &start, &stop, 0, max_align_size, NULL))
		{
			found = TRUE;
			order = 0;
		}
		else if(is_loc_overlap(loc_1_2, loc_2_2, &start, &stop, 0, max_align_size, NULL))
		{
			found = TRUE;
			order = 1;
		}
		else
		{
			start_1 = start;
			stop_1 = stop;
			start_2 = start;
			stop_2 = stop;

			if(is_loc_overlap(loc_1_1, loc_2_1, &start_1, &stop_1, MIN_SEG_SIZE*10, max_align_size, &gap_1))
			{
				found = TRUE;
				order = 0;
                                gap = gap_1;
			}
			if(is_loc_overlap(loc_1_2, loc_2_2, &start_2, &stop_2, MIN_SEG_SIZE*10, max_align_size, &gap_2))
			{
				if(!found || gap_2 < gap_1)
				{
					order = 1;
					found = TRUE;
                                        gap = gap_2;
				}
			}
			if(found)
			{
				if(order == 0)
				{
					start = start_1;
					stop = stop_1;
				}
				else
				{
					start = start_2;
					stop = stop_2;
				}
			}
		}
	}

	SeqLocFree(loc_1_1);
	SeqLocFree(loc_1_2);
	SeqLocFree(loc_2_1);
	SeqLocFree(loc_2_2);

	if(!found)
	{
		return NULL;
	}

	/*
		===================>
			====================>
			^^^^^^^^^^^^^^^^^
			           |----------
                                    need to have some offset into the 
				    second sequence
	*/
	if(order == 0)
		sip = dsp_1->ids;
	else
		sip = dsp_1->ids->next;
	SeqAlignStartStopById(align_1, sip, &start_1, &stop_1, &strand); 
	SeqAlignStartStopById(align_2, sip, &start_2, &stop_2, &strand); 

        start = MAX(start_2, start_1);
        stop = MIN(stop_1, stop_2);
        OrderInt4(&start,&stop); /* Need reordering for gaps */

	if(dsp_1->strands[order] == Seq_strand_minus)
	{
		match_start = find_matching_position(dsp_1, stop, order);
		match_stop = find_matching_position(dsp_2, start, order);
	}
	else
	{
		match_start = find_matching_position(dsp_1, start, order);
		match_stop = find_matching_position(dsp_2, stop, order);
	}

	if(match_start == -1 || match_stop == -1)
		return NULL;
	bsp = NULL;
	if(order == 0)
		bsp = BioseqLockById(dsp_1->ids->next);
	else
		bsp = BioseqLockById(dsp_1->ids);

	if(bsp == NULL)
		return NULL;
        { 
            Int4 old_start=start,old_stop = stop;
            start = MAX(start-100,MIN(start_1,start_2));
            stop = MIN(stop+100,MAX(stop_1,stop_2));
            if(match_start<=match_stop) {
                match_start -=(old_start-start);
                match_stop +=(stop-old_stop);
                if(match_start<0) {
                    start+=match_start;
                    match_start=0;
                } 
                if(match_stop>bsp->length-1) {
                    stop-=(match_stop-(bsp->length-1));
                    match_stop = bsp->length-1;
                }
            }
            else {
                match_start +=(old_start-start);
                match_stop -=(stop-old_stop);
                if(match_stop<0) {
                    stop+=match_stop;
                    match_stop=0;
                } 
                if(match_start>bsp->length-1) {
                    start-=(match_start-(bsp->length-1));
                    match_start = bsp->length-1;
                }

            }

        }
	BioseqUnlock(bsp);


	OrderInt4(&match_start, &match_stop);
	OrderInt4(&start, &stop);
        /* HS Fix to Disallow Very Long Gaps */
        if(abs(abs(stop-start) -abs(match_stop-match_start))>MaxGap+3000)
            return NULL;
	if(order == 0)
	{
		loc_1_1 = SeqLocIntNew(start, stop, dsp_1->strands[order], dsp_1->ids);
		loc_1_2 = SeqLocIntNew(match_start, match_stop, 
			dsp_1->strands[1-order], dsp_1->ids->next);
	}
	else
	{
		loc_1_1 = SeqLocIntNew(match_start, match_stop, 
			dsp_1->strands[1-order], dsp_1->ids);
		loc_1_2 = SeqLocIntNew(start, stop, 
			dsp_1->strands[order], dsp_1->ids->next);
	}

	bsp_1 = BioseqLockById(SeqLocId(loc_1_1));
	if(bsp_1 == NULL)
	{
		SeqLocFree(loc_1_1);
		SeqLocFree(loc_1_2);
		return NULL;
	}
	bsp_2 = BioseqLockById(SeqLocId(loc_1_2));
	if(bsp_2 == NULL)
	{
		BioseqUnlock(bsp_1);
		SeqLocFree(loc_1_1);
		SeqLocFree(loc_1_2);
		return NULL;
	}

	align = SIM4ALN_choice(loc_1_1, loc_1_2, 200, 8);
	SeqLocFree(loc_1_1);
	SeqLocFree(loc_1_2);

	if(align != NULL)
	{
		if(order == 1)
		{
			start = match_start;
			stop = match_stop;
			order = 0;
		}
		t_align = (SeqAlignPtr) AsnIoMemCopy((Pointer)align_1, (AsnReadFunc)SeqAlignAsnRead, (AsnWriteFunc)SeqAlignAsnWrite);

		/* for debugging purpose*/
		/* t_align->next = align;
		align->next = (SeqAlignPtr) AsnIoMemCopy((Pointer)align_2, (AsnReadFunc)SeqAlignAsnRead, (AsnWriteFunc)SeqAlignAsnWrite);
		return t_align;    */

		if(MergeTwoAlignList(t_align, &align, start, stop, order))
		{
			if(align != NULL)
				SeqAlignFree(align);
			align = t_align;
			align->next = NULL;

			t_align = (SeqAlignPtr) AsnIoMemCopy((Pointer)align_2, (AsnReadFunc)SeqAlignAsnRead, (AsnWriteFunc)SeqAlignAsnWrite);
			t_align->next = NULL;
			MergeTwoAlignList(align, &t_align, start, stop, order);
			if(t_align != NULL)
				SeqAlignFree(t_align);
		}
		else
		{
			if(align != NULL)
				align = SeqAlignSetFree(align);
			SeqAlignFree(t_align);
		}
	}
	/*for debugging */
	/* else
	{
		align_1->next = align_2;
		align_2->next = NULL;
		SeqAnnotWrite(align_1);
		exit(1);
	} */
	BioseqUnlock(bsp_1);
	BioseqUnlock(bsp_2);

	if(reverse && align != NULL)
		SeqAlignReverse(align, 1);
	return align;
}

		

static SeqAlignPtr MergeToOneAlignment(SeqAlignPtr PNTR palign,Int4 MaxGap) 
{
	SeqAlignPtr next, prev, align;
	SeqAlignPtr merge_align;
	SeqAlignPtr curr;

	if(palign == NULL || *palign == NULL)
		return NULL;
	align = *palign;
	if(align->segtype != 2)
		return NULL;
	*palign = SeqAlignSortByRegion(align, 0);

	prev = NULL;
	align = *palign;

	while(align)
	{
		next = align->next;
		if(next != NULL)
		{
			merge_align = NULL;
			curr = *palign;
			/* if(next->next == NULL)
				printf("stop here\n"); */
			merge_align = MergeTwoDspBySIM4(align, next,MaxGap);
			if(merge_align != NULL)
			{
				if(prev == NULL)
					*palign = merge_align;
				else
					prev->next = merge_align;
				
				curr = merge_align;
				while(curr->next != NULL)
				{
					prev = curr;
					curr = curr->next;
				}
				curr->next = next->next;
				next->next = NULL;
				SeqAlignSetFree(align);
				align = curr;
			}
			else
			{
				prev = align;
				align = align->next;
			}
		}
		
		else
			align = align->next;
	}

	if(*palign != NULL && (*palign)->next!=NULL)
		*palign = SeqAlignSortByLength(*palign);
	return (*palign);
}

/*
*
*	Filter out any alignments that were not in the main 
*	diagnol
*
*/
static Boolean ModifyAlignList(SeqAlignPtr PNTR palign)
{
	SeqAlignPtr align, next;
	DenseSegPtr dsp;
	SeqAlignPtr h_align = NULL;

	if(palign == NULL || *palign == NULL)
		return FALSE;

	align = *palign;
	while(align)
	{
		next = align->next;
		if(align->segtype == 2)
		{
			dsp = (DenseSegPtr) align->segs;
			align->next = SeqAlignExtractByIds(&next,dsp->ids, dsp->ids->next);
			ModFilterDenseSegAlign(&align, dsp->ids);
			if(align != NULL)
				h_align = SeqAlignLink(align, h_align);
		}
		else
		{
			align->next = NULL;
			SeqAlignFree(align);
		}
		align = next;
	}

	*palign = h_align;
	return TRUE;
}

				

			


static Int2 get_polyA_index (Uint1Ptr res_buf, Int2 len)
{
	Int2 i, total;
	Uint1 res;
	Int2 count_A;

	count_A = 0;
	total = 0;

	for( i= len -1; i>=0; --i)
	{
		res = res_buf[i];
		if(res == 'a' || res == 'A' || res == 'n' || res == 'N')
		{
			++count_A;
			++total;
		}
		else
		{
			if(total > 5 && count_A * 100 < (total + 1) * PERCENT_A_RES)
				break;
			else
				++total;
		}
	}
	/*trim the non-A and non-Ns*/
	for(i = MAX(i, 0); i<len; )
	{
		res = res_buf[i];
		if(res != 'n' && res != 'N' && res != 'a' && res != 'A')
			++i;
		else	/*N/A residue*/
			break;
	}

	if(len-1 - i > 2)
		return i;

	return -1;
}


static SeqLocPtr check_polyA_tail (BioseqPtr bsp)
{
	Int2 len = 100;
	SeqPortPtr spp;
	Uint1 res_buf[101];
	Uint1 res;
	Int2 i;
	SeqLocPtr end_loc, begin_loc;
	Int4 start, stop;

	begin_loc = NULL;
	end_loc = NULL;

	len = MIN(bsp->length-1, 100);
	/*check for the end of the sequence for polyA tail*/
	spp = SeqPortNew(bsp, bsp->length-1-(len-1), bsp->length-1, 
		Seq_strand_plus, Seq_code_iupacna);
	i = 0;
	while((res = SeqPortGetResidue(spp)) != SEQPORT_EOF)
	{
		if(IS_ALPHA(res))
			res_buf[i++] = res;
	}
	SeqPortFree(spp);
	if(i > 10)
	{
		len = i;
		i = get_polyA_index (res_buf, len);
		if(i != -1)
		{
			stop = bsp->length -1;
			start = bsp->length - 1 - (len -1) + i;

			end_loc = SeqLocIntNew(start, stop, 
				Seq_strand_plus, SeqIdFindBest(bsp->id, 0));
		}

	}

	/*check for the beginning on the reverse complement of the sequence 
          for polyA tail. Usually the 3' clone of the EST*/
	spp = SeqPortNew(bsp, 0, len-1, 
		Seq_strand_minus, Seq_code_iupacna);
	i = 0;
	while((res = SeqPortGetResidue(spp)) != SEQPORT_EOF)
	{
		if(IS_ALPHA(res))
			res_buf[i++] = res;
	}
	SeqPortFree(spp);
	if(i > 10)
	{
		len = i;
		i = get_polyA_index (res_buf, len);
		if(i != -1)
		{
			stop = len -1 - i;
			start = 0;

			begin_loc = SeqLocIntNew(start, stop, 
				Seq_strand_minus, SeqIdFindBest(bsp->id, 0));
		}

	}
	if(begin_loc == NULL)
		return end_loc;
	else
	{
		if(end_loc != NULL)
		{
			if(SeqLocLen(end_loc) > SeqLocLen(begin_loc))
			{
				SeqLocFree(begin_loc);
				return end_loc;
			}
			else
			{
				SeqLocFree(end_loc);
				return begin_loc;
			}
		}
		return begin_loc;
	}
}

static void SeqLocTrimPolyATail(SeqLocPtr loc_2, BioseqPtr bsp_2) {
	SeqLocPtr poly_loc;
	SeqIntPtr sint;

	poly_loc = check_polyA_tail (bsp_2);
	if(poly_loc != NULL)
	{
		sint = (SeqIntPtr) loc_2->data.ptrvalue;
		if(SeqLocStart(poly_loc) > (bsp_2->length)/2)
		    sint->to = SeqLocStart(poly_loc) -1;
		else
			sint->from = SeqLocStop(poly_loc) + 1;
		SeqLocFree(poly_loc);
	}
}

static Boolean is_bad_blast_alignment(SeqAlignPtr align, SeqLocPtr slp1, SeqLocPtr slp2, Int4 min_align_len)
{
	SeqAlignPtr best_align;
	BioseqPtr bsp1, bsp2;
	Int4 max_score;

	best_align = find_best_align(align, &max_score);
	if(best_align == NULL)
		return TRUE;
	bsp1 = BioseqFind(SeqLocId(slp1));
	bsp2 = BioseqFind(SeqLocId(slp2));

	if(bsp1 == NULL || bsp2 == NULL)
		return (max_score > 50);
	return is_bad_align(bsp1, bsp2, best_align,
		min_align_len, MIN(SeqLocLen(slp1), SeqLocLen(slp2)));
}

/*
  Blindly reverse the left-right order of the seqalign... to maintain
  'display' order.
 */
NLM_EXTERN void SeqAlignReverseOrder(SeqAlignPtr align)
{
	Int2 numseg, i, j;

	if(align->segtype == SAS_DENSEG) {
	    DenseSegPtr dsp;
	    dsp = (DenseSegPtr) align->segs;
	    if(dsp) {
		numseg = dsp->numseg;
		for(i = 0, j = numseg-1; i<(numseg+1)/2 && i!=j; i++, --j)
		    {
			Int4 tmp_start,k;
			Uint1 tmp_strand;
			for(k=0;k<dsp->dim;k++) {
			    tmp_start = dsp->starts[2*i+k];
			    dsp->starts[2*i+k] = dsp->starts[2*j+k];
			    dsp->starts[2*j+k] = tmp_start;
			    tmp_strand = dsp->strands[2*i+k];
			    dsp->strands[2*i+k] = dsp->strands[2*j+k];
			    dsp->strands[2*j+k] = tmp_strand;
			}
			dsp->lens[j] = dsp->lens[i];
		    }
	    }
	} else if (align->segtype == SAS_DENDIAG) {
	    DenseDiagPtr ddp,ddp_last,ddp_next;
	    ddp = (DenseDiagPtr) align->segs;
	    ddp_last =NULL;
	    while(ddp) {
		{
		    ddp_next = ddp->next;
		    ddp->next = ddp_last;
		    ddp_last = ddp;
		    ddp = ddp_next;
		}
		align->segs = ddp_last;
	    }
	} else if (align->segtype == SAS_STD) {
	    StdSegPtr ssp,ssp_last,ssp_next;
	    ssp = (StdSegPtr) align->segs;
	    ssp_last =NULL;
	    while(ssp) {
		{
		    ssp_next = ssp->next;
		    ssp->next = ssp_last;
		    ssp_last = ssp;
		    ssp = ssp_next;
		}
		align->segs = ssp_last;
	    }	    
	} else {
	    ErrPostEx(SEV_WARNING,0,0,"SeqAlignReverse order : unsupported seqalign type %d\n",align->segtype);
	}
}

NLM_EXTERN void SeqAlignSwapSeqs(SeqAlignPtr align) {
    if(align->segtype == SAS_DENSEG) {
	DenseSegPtr dsp;
	dsp = (DenseSegPtr) align->segs;
	if(dsp) {
	    SeqIdPtr sip;
	    Int4 i;
	    sip = dsp->ids;
	    dsp->ids = dsp->ids->next;
	    sip->next = NULL;
	    dsp->ids->next = sip;
	    for(i=0;i<dsp->numseg;i++) {
		Int4 tmp_start;
		Uint1 tmp_strand;
		tmp_start = dsp->starts[2*i];
		dsp->starts[2*i] = dsp->starts[2*i+1];
		dsp->starts[2*i+1] = tmp_start;
		tmp_strand = dsp->strands[2*i];
		dsp->strands[2*i] = dsp->strands[2*i+1];
		dsp->strands[2*i+1] = tmp_strand;
	    }
	}
    } else if (align->segtype == SAS_DENDIAG) {
	DenseDiagPtr ddp;
	ddp = (DenseDiagPtr) align->segs;
	while(ddp) {
	    SeqIdPtr sip;
	    sip = ddp->id;
	    ddp->id = ddp->id->next;
	    sip->next = NULL;
	    ddp->id->next = sip;
	    {
		Int4 tmp_start;
		Uint1 tmp_strand;
		tmp_start = ddp->starts[0];
		ddp->starts[0] = ddp->starts[1];
		ddp->starts[1] = tmp_start;
		tmp_strand = ddp->strands[0];
		ddp->strands[0] = ddp->strands[1];
		ddp->strands[1] = tmp_strand;
	    }
	    ddp = ddp->next;
	}		
    } else if (align->segtype == SAS_STD) {
	StdSegPtr ssp;
	ssp = (StdSegPtr) align->segs;
	while(ssp) {
	    SeqIdPtr sip;
	    sip = ssp->ids;
	    if(sip) {
		ssp->ids = ssp->ids->next;
		sip->next = NULL;
		ssp->ids->next = sip;
	    }
	    {
		SeqLocPtr tmp_slp;
		tmp_slp = ssp->loc;
		ssp->loc = ssp->loc->next;
		ssp->loc->next = tmp_slp;
		tmp_slp->next = NULL;
	    }
	    ssp = ssp->next;
	}				
    } else {
	ErrPostEx(SEV_WARNING,0,0,"SeqAlignSwapSeqs: Do not know how to reverse SeqAlign of this type\n");
    }
    return;
}


NLM_EXTERN PSeqAlignInfoPtr SeqAlignToPSeqAlignInfo (SeqAlignPtr sap)
{
        PSeqAlignInfoPtr alip, alip_head, newalip;
        Int2 found;
        SeqAlignPtr alip_sap;
        SeqIdPtr sip;

        if (!sap)
                return NULL;
        alip = (PSeqAlignInfoPtr)MemNew(sizeof(PSeqAlignInfo));
        alip_head = alip;
        sip = SeqIdPtrFromSeqAlign(sap);
        alip->sap = sap;
        alip->sip = SeqIdDupList(sip);
        alip->next = NULL;
        sap = sap->next;
        alip->sap->next=NULL;
        while (sap)
        {
                sip = SeqIdPtrFromSeqAlign(sap);
                found = 0;
                alip = alip_head;
                while (alip && !found)
                {
                        if (SeqIdComp(sip->next, alip->sip->next))
                        {
                                alip_sap = alip->sap;
                                while(alip_sap->next !=NULL)
                                        alip_sap = alip_sap->next;
                                alip_sap->next = sap;
                                sap = sap->next;
                                alip_sap->next->next = NULL;
                                found = 1;
                        }
                        alip = alip->next;
                }
                if (found == 0)
                {
                        alip = alip_head;
                        while(alip->next != NULL)
                                alip = alip->next;
                        newalip = (PSeqAlignInfoPtr)MemNew(sizeof(PSeqAlignInfo));
                        newalip->sip = SeqIdDupList(sip);
                        newalip->sap = sap;
                        newalip->next = NULL;
                        sap = sap->next;
                        newalip->sap->next = NULL;
                        alip->next = newalip;
                }
        }
        return alip_head;
}

NLM_EXTERN SeqAlignPtr ReassembleSeqAlignFromPSeqAlignInfo(PSeqAlignInfoPtr alip)
{
        SeqAlignPtr sap, head_sap, sap_curr;
        PSeqAlignInfoPtr prevalip;

        head_sap = NULL;
        while(alip)
        {
                if (!head_sap)
                {
                        head_sap = alip->sap;
                        sap_curr = head_sap;
                } else
                {
                        sap = alip->sap;
                        sap_curr->next = sap;
                        while (sap_curr->next)
                                sap_curr = sap_curr->next;
                }
                prevalip = alip;
                alip = alip->next;
                prevalip->sip = SeqIdSetFree(prevalip->sip);
                MemFree(prevalip);
        }
        return head_sap;
}

/* Filter SeqAlign by removing bad hits and hits off the main diagonal (as defined by the
   first hit .. which is the highest scoring in blast */
static SeqAlignPtr SeqAlignConsistentDiagFilter(SeqAlignPtr align,SeqLocPtr slp1, SeqLocPtr slp2, 
					 FILE* err_fp,Int4 MaxGap) {
    Int4 old_num,curr_num,len,len1,len2;

    SeqAlignPtr prev,t_align;

    if(slp1 == NULL || slp2 == NULL || align == NULL)
	return NULL;
    len1 = SeqLocLen(slp1);
    len2 = SeqLocLen(slp2);
    if(len1 == 0 || len2 == 0)
	return NULL;

    if(align != NULL) {
#ifdef DEBUG
	    save_output (SeqLocId(slp1), "blast.out", align);
#endif
		/*
		  Checks for harcoded conditions on the FIRST alignment. 
		  Alignment at least
		  50 bases or 90% of length if sequences are smaller than 50.
                  Also do not allow more than 10% mismatches.
		  */
	if(is_bad_blast_alignment(align, slp1, slp2, 50)) {
	    SeqAlignSetFree(align);
	    return NULL;
	}
	    /* Remove hits that are off the main diagonal of the
		   longest hit */
	ModifyAlignList(&align);
#ifdef DEBUG
	    save_output (SeqLocId(slp1), "merge.out", align);
#endif
	if(is_bad_blast_alignment(align, slp1, slp2, 50))
	    {
		SeqAlignSetFree(align);
		return NULL;
	    }
	
	if(align != NULL)
	    {
		old_num = SeqAlignCount(align);
		MergeToOneAlignment(&align,MaxGap);
		curr_num = SeqAlignCount(align);
		
		if(align == NULL)
		    {
			fprintf(stderr, "Fail in MergeToOneAlignment\n");
		    } 
		while(curr_num > 1 && curr_num < old_num)
		    {
			old_num = curr_num;
			MergeToOneAlignment(&align,MaxGap);
			curr_num = SeqAlignCount(align);
		    }
		
		if(curr_num > 1)
		    {
			prev = align;
			t_align = align->next;
			while(t_align)
			    {
				len = SeqAlignLength(t_align);
				if(len < 50 || SeqAlignLength(prev)/len > 5)
				    {
					prev->next = NULL;
					SeqAlignSetFree(t_align);
						break;
				    }
				else
				    {
					prev = t_align;
						t_align = t_align->next;
				    }
			    }
		    }
						
		
	    }
#ifdef DEBUG
	    save_output (SeqLocId(slp1), "sim4.out", align);
#endif
	if(is_bad_blast_alignment(align, slp1, slp2, MIN(50, MIN(len1, len2)/5)))
	    {
		SeqAlignSetFree(align);
		return NULL;
	    }
	
    }
    return align;
}


static Boolean is_bad_align (BioseqPtr m_bsp, BioseqPtr s_bsp, SeqAlignPtr align, Int4 min_align_len,Int4 loc_len)
{
	DenseSegPtr dsp;
	Int4 m_start, m_stop, s_start, s_stop;
	SeqPortPtr m_spp, s_spp;
	Int2  i;
	Uint1 m_res, s_res;
	Int4 mismatch = 0;
	Uint1 code;
	Int4 j;
	Uint1 strand;
	Int4 align_len;

	code = Seq_code_iupacna;
	dsp = (DenseSegPtr) align->segs;
	SeqAlignStartStopById(align, dsp->ids->next, &s_start, &s_stop, &strand);
	if(s_start<0 || s_stop>= s_bsp->length) {
	    ErrPostEx(SEV_WARNING,0,0,"find_mismatch_residue: corrupted alignment \n");
	    return TRUE;
	} else {
	    s_spp = SeqPortNew(s_bsp, s_start, s_stop, dsp->strands[1], code);
	}
	/* if(s_stop - s_start + 1 < MIN(min_align_len, loc_len-MAX(10, loc_len/10)))
		return TRUE; */

	SeqAlignStartStopById(align, dsp->ids, &m_start, &m_stop, &strand);
	if(m_start<0 || m_stop>= m_bsp->length) {
	    ErrPostEx(SEV_WARNING,0,0,"find_mismatch_residue: corrupted alignment \n");
	    return TRUE;
	} else {

	    m_spp = SeqPortNew(m_bsp, m_start, m_stop, strand, code);
	}


	align_len = 0;
	for(i = 0; i<dsp->numseg; ++i)
	{
		m_start = dsp->starts[2*i];
		s_start = dsp->starts[2*i + 1];
		if(m_start == -1)
		{
			if(dsp->lens[i] <=5)
			{
				mismatch += 1;
				++align_len;
			}
			if(s_start != -1)
				SeqPortSeek(s_spp, dsp->lens[i], SEEK_CUR);
		}
		else if(s_start == -1)
		{
			if(dsp->lens[i] <=5)
			{
				mismatch += 1;
				++align_len;
			}
			if(m_start != -1)
				SeqPortSeek(m_spp, dsp->lens[i], SEEK_CUR);
		}
		else
		{
			for(j = 0; j<dsp->lens[i]; ++j)
			{
				m_res = SeqPortGetResidue(m_spp);
				while(!IS_ALPHA(m_res) && m_res != SEQPORT_EOF)
					m_res = SeqPortGetResidue(m_spp);
				s_res = SeqPortGetResidue(s_spp);
				while(!IS_ALPHA(s_res) && s_res != SEQPORT_EOF)
					s_res = SeqPortGetResidue(s_spp);
				if(m_res != 'n' && m_res != 'N' && s_res != 'n' && s_res != 'N')
				{
					if(m_res != s_res)
						++mismatch;
				}
			}
			align_len += dsp->lens[i];
		}
		
	}

	SeqPortFree(m_spp);
	SeqPortFree(s_spp);
	if(align_len < MIN(min_align_len, loc_len-MAX(10, loc_len/10)))
		return TRUE;
	if(mismatch > 0)
		return ((mismatch*100)/align_len >10);
	else
		return FALSE;
}

static Boolean need_recompute(SeqAlignPtr align, Int4 m_len, Int4 s_len)
{
        DenseSegPtr dsp;
        Int4 m_start, m_stop;
        Int4 s_start, s_stop;
        Int2 end, i;
        Int2 small_seg_num;

        if(m_len > 10000 || s_len > 10000)
                return FALSE;
        dsp = (DenseSegPtr) align->segs;
        end = dsp->numseg-1;
        m_start = dsp->starts[0];
        m_stop = dsp->starts[2*end] + dsp->lens[end] -1;

        if(dsp->strands[1] == Seq_strand_minus)
        {
                s_start = dsp->starts[2*end +1];
                s_stop = dsp->starts[1] + dsp->lens[0] -1;
                if(s_start > MAX_HANG && (m_len - 1 - m_stop) > MAX_HANG)
                        return TRUE;
                if((s_len-1-s_stop)>MAX_HANG && m_start > MAX_HANG)
                        return TRUE;
        }
        else
        {
                s_start = dsp->starts[1];
                s_stop = dsp->starts[2*end+1] + dsp->lens[end] -1;
                if(s_start > MAX_HANG && m_start > MAX_HANG)
                        return TRUE;
                if((s_len-1-s_stop) > MAX_HANG && (m_len-1-m_stop)> MAX_HANG)
                        return TRUE;
        }

        small_seg_num = 0;
        for(i = 0; i<dsp->numseg; ++i)
        {
                if(dsp->lens[i] < 10)
                        ++small_seg_num;
        }

        return (small_seg_num >= 20);

}

static void extend_dsp_ends(Int4 first_len, Int4 second_len, SeqAlignPtr align, Int4 max_overhang)
{
	Int4 first, last;
	DenseSegPtr dsp;
	Int2 endseg;
	Int4 extend_len;
	Int4 offset;

	dsp = (DenseSegPtr) align->segs;
	endseg = dsp->numseg-1;
	if(dsp->strands[1] == Seq_strand_minus)
	{
		first = dsp->starts[2*endseg+1];
		if(first > 0 && first < max_overhang)
		{
                    if(dsp->strands[0] != Seq_strand_minus)
                        extend_len = MIN(first, first_len - dsp->starts[2*endseg] - dsp->lens[endseg]);
                    else
                        extend_len = MIN(first, dsp->starts[2*endseg]);
			if(extend_len > 0)
			{
				dsp->lens[endseg] += extend_len;
                                dsp->starts[2*endseg+1]-=extend_len;
                                if(dsp->strands[0]==Seq_strand_minus)
                                    dsp->starts[2*endseg] -= extend_len;
			}
		}

		last = dsp->starts[1] + dsp->lens[0];
		offset = second_len - last;
		if(offset > 0 && offset < max_overhang)
		{
                    if(dsp->strands[0]!=Seq_strand_minus) 
			extend_len = MIN(offset,dsp->starts[0]);
                    else
                        extend_len = MIN(offset,first_len - dsp->starts[0]-dsp->lens[0]);
			if(extend_len > 0)
			{
				dsp->lens[0] += extend_len;
                                if(dsp->strands[0]==Seq_strand_plus)
                                    dsp->starts[0]-=extend_len;
			}
		}
	}
	else
	{
		first = dsp->starts[1];
		if(first> 0 && first < max_overhang)
		{
                    if(dsp->strands[0]!=Seq_strand_minus) 
			extend_len = MIN(first, dsp->starts[0]);
                    else
                        extend_len = MIN(first, first_len -dsp->starts[0]-dsp->lens[0]);
			if(extend_len > 0)
			{
				dsp->lens[0] += extend_len;
				dsp->starts[1] -= extend_len;
                                if(dsp->strands[0]!=Seq_strand_minus)
                                    dsp->starts[0] -= extend_len;
			}
		}
		last = dsp->starts[2*endseg+ 1] + dsp->lens[endseg];
		offset = second_len  - last;

		if(offset > 0 && offset < max_overhang)
		{
                    if(dsp->strands[0]!=Seq_strand_minus)
			extend_len = MIN(offset, first_len - dsp->starts[2*endseg] - dsp->lens[endseg]);
                    else
			extend_len = MIN(offset, dsp->starts[2*endseg]);
			if(extend_len > 0)
			{
				dsp->lens[endseg] += extend_len;
                                if(dsp->strands[0]==Seq_strand_minus)
                                    dsp->starts[2*endseg]-=extend_len;
			}
		}
	}

}
	


/* Tries to Make a global alignment from a Set of local SeqAligns. 
 */
NLM_EXTERN SeqAlignPtr SeqAlignSetGlobalFromLocal(SeqAlignPtr align,SeqLocPtr loc_1, SeqLocPtr loc_2, BioseqPtr bsp_1, BioseqPtr bsp_2, FILE *err_fp,Int4 MaxGap)
{
	Int4 len_1, len_2;
	Char label[101];
        if(align==NULL || loc_1 == NULL || loc_2 == NULL || bsp_1 == NULL || bsp_2 == NULL)
            return NULL;
	len_1 = SeqLocLen(loc_1);
	if(len_1 <=0)
	{
            if(err_fp) {
		MuskSeqIdWrite(SeqLocId(loc_1), label, 100, PRINTID_TEXTID_ACCESSION, TRUE, TRUE);
		fprintf(err_fp, "bad length %ld in sequence %s\n", (long)len_1, label);
            }
            return NULL;
	}
	len_2 = SeqLocLen(loc_2);
	if(len_2 <=0)
	{
            if(err_fp) {
		MuskSeqIdWrite(SeqLocId(loc_2), label, 100, PRINTID_TEXTID_ACCESSION, TRUE, TRUE);
		fprintf(err_fp, "bad length %ld in sequence %s\n", (long) len_1, label);
            }
            return NULL;
	}

	if(align != NULL)
	{

	    align = SeqAlignConsistentDiagFilter(align,loc_1, loc_2,err_fp,MaxGap);
	}

	if(align != NULL)
	{
		extend_dsp_ends(bsp_1->length, bsp_2->length, align, MAX_EXTEND_OVERHANG);
		SeqAlignSwapOrder(align);
		extend_dsp_ends(bsp_2->length, bsp_1->length, align, MAX_EXTEND_OVERHANG);
		SeqAlignSwapOrder(align);
	}
	return align;
}


/***************************************************************************************
***
***  ValidateSeqAlignandACC
***	calls ValidateSeqAlign (in api directory)
***	and tests for occurrence of ACC string in sequence ID.
***	ACC|ACC# will be compared with the corresponding sequence (ACC#)
***	in the database and replaced by a far pointer if the sequences
***	are identical.
***
***************************************************************************************/
typedef struct saval {
  Boolean     message;
  Boolean     msg_success;
  Boolean     find_remote_bsp;
  Boolean     find_acc_bsp;
  Boolean     delete_salp;
  Boolean     delete_bsp;
  Boolean     retdel;
  ValNodePtr  ids;
  Uint2       entityID;
  Boolean     dirty;
} SaVal, PNTR SaValPtr;

static ValNodePtr errorp = NULL;

/******************************************************************
Output error message according to code defined in alignval.h.  
id refers to seqid of the sequence that causes the error 
and idcontext refers to other sequences in the same segment.  
Intvalue is used to indicate 1) the segment where the sequence 
with error is, or 2) the segtype in case of segtype error.  
Please note that not all errors report all three 
parameters(id, idcontext, Intvalue)
******************************************************************/ 
static void ValMessage (Int1 MessageCode, ErrSev errlevel, SeqIdPtr id, SeqIdPtr idcontext , Int4 Intvalue) 
{
  
  Char     buf[256], 
           buf3[64],
           string1[64],
           string2[252];

  string1[0] = '\0';
  string2[0] = '\0';
  SeqIdWrite(id, buf, PRINTID_FASTA_LONG, sizeof(buf)-1);
  switch(MessageCode)
  {
    case Err_SeqId:
      sprintf(string1, "SeqId");
      sprintf(string2, "Invalid Seq_id: %s\n", buf);
      break;

    case Err_Strand_Rev:      
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Strand");
      sprintf(string2, "Alignment strand is reversed in segment %d for Seq ID: %s in the context of%s\n", Intvalue, buf, buf3);
      break;

    case Err_Denseg_Len_Start:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Start/Length");
      sprintf(string2, "Error in length and/or starts in segment %d for sequence ID: %s in the context of %s\n", Intvalue, buf, buf3);
      break;

    case  Err_Start_Less_Than_Zero:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Start");
      sprintf(string2, "Start point is less than zero in segment %d for sequence ID: %s in the context of %s\n", Intvalue, buf, buf3);
      break;

    case Err_Start_More_Than_Biolen:      
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Start");
      sprintf(string2, "Start point is greater than total bioseq length in segment %d for sequence ID: %s in the context of%s\n", Intvalue, buf, buf3);
      break;

    case Err_End_Less_Than_Zero:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Length");
      sprintf(string2, "End point is less than zero in segment %d for sequence ID: %s in the context of %s\n", Intvalue, buf, buf3);
      break;

    case Err_End_More_Than_Biolen:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Length");
      sprintf(string2, "End point is greater than total bioseq length in segment %d for sequence ID: %s in the context of%s\n", Intvalue, buf, buf3);
      break;

    case Err_Len_Less_Than_Zero:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Length");
      sprintf(string2, "Segment length is less than zero in segment %d for sequence ID: %s in the context of %s\n", Intvalue, buf, buf3); 
      break;

    case Err_Len_More_Than_Biolen:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Length");
      sprintf(string2, "Segment length is greater than total bioseq length in segment %d for sequence ID: %s in the context of %s\n", Intvalue, buf, buf3);
      break; 
 
    case Err_Sum_Len_Start:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Start");
      sprintf(string2, "Sum of start point and segment is greater than total bioseq length in segment %d  for sequence ID: %s in the context of %s\n",  Intvalue, buf, buf3); 
      break;

    case Err_SeqAlign_DimSeqId_Not_Match:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "SeqId");
      sprintf(string2, "The number of SeqId does not match the dimensions for sequence ID's %s\n", buf3); 
      break;

    case Err_Segs_DimSeqId_Not_Match:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "SeqId");
      sprintf(string2, "The number of SeqId does not match the dimensions in segment %d for  sequence ID's %s\n", Intvalue, buf3); 
      break;

    case Err_Fastalike:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Fasta");
      sprintf(string2, "This may be a fasta-like alignment for SeqId: %s in the context of %s\n", buf, buf3); 
      break;

    case Err_Null_Segs:
      sprintf(string1, "Segs");
      sprintf(string2, "This alignment contains a null segs\n");
      break;

    case Err_Segment_Gap:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Segs");
      sprintf(string2, "Segment %d is a gap for all sequence with the following ID's: %s\n", Intvalue, buf3); 
      break;

    case Err_Segs_Dim_One:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Segs");
      sprintf(string2, "There is only one dimension in segment %d for  sequence ID's %s\n", Intvalue, buf3); 
      break;

    case Err_SeqAlign_Dim_One:
      SeqIdWrite (idcontext, buf3, PRINTID_REPORT, sizeof (buf3));
      sprintf(string1, "Dim");
      sprintf(string2, "There is only one dimension for sequence ID's %s\n", buf3); 
      break;

    case Err_Segtype :
      sprintf(string1, "Segs");
      sprintf(string2, "This alignment has a undefined or unsupported Seqalign segtype %d\n", Intvalue);
      break;

    default:
      break;
  }
  if (StringLen(string1) > 0)
     errorp = BlastConstructErrorMessage (string1, string2, errlevel, &errorp);
}

 
/********************************************************
***
*** NormalizeSeqAlignId
***   Checks local seqid . if a seqid string contains "acc"
***   seqid has a correspondant sequence in db.
***   This local seqid is replaced by its gi number. 
***
***   The local sequence is compared to the sequence from 
***   the db. If the local sequence is a region of the db sequence
***   the positions in the seqalign are updaded with the offset.
***
***          Thanks to Mark for this useful function! 
**********************************************************/
static Int4 getlengthforid (SeqIdPtr sip)
{
  BioseqPtr        bsp;
  Int4             lens=0;
 
  if (sip==NULL)
     return 0;
  bsp = BioseqLockById (sip);
  if (bsp != NULL) {
     lens = bsp->length;
     BioseqUnlock (bsp);
  }
  return lens;
}

static ValNodePtr nrSeqIdAdd (ValNodePtr vnp, SeqIdPtr sip)
{
  ValNodePtr vnptmp=NULL;
  SeqIdPtr   siptmp;

  if (vnp!=NULL) {
     for (vnptmp=vnp; vnptmp!=NULL; vnptmp=vnptmp->next) {
        siptmp=(SeqIdPtr)vnptmp->data.ptrvalue;
           if (SeqIdForSameBioseq(sip, siptmp))
              break;
     }
  }
  if (vnptmp==NULL)
     ValNodeAddPointer(&vnp, 0, sip);
  return vnp;
}

static SeqAlignPtr LIBCALL SeqAlignBestHit (SeqAlignPtr salp, BioseqPtr bsp1, BioseqPtr bsp2, Int4 threshold, CharPtr PNTR message, Int4Ptr nonly)
{
   AMFreqPtr         afp;
   Int4              alln;
   AMAlignIndex2Ptr  amaip;
   Int4              beg;
   Int4              ctr;
   Int4              end;
   Boolean           found;
   Int4              gaps;
   Int4              i;
   Int4              j;
   Int4              last;
   Int4              len;
   Int4              n;
   Int4              nctr;
   Char              newstr[256];
   Int4              mismatches;
   Uint1             res;
   SeqAlignPtr       sap;
   SeqAlignPtr       sap_new;
   SeqAlignPtr       sap_new2;
   SeqAlignPtr       sap_orig;
   SeqPortPtr        spp;
   Int4              start1;
   Int4              start2;
   Int4              stop1;
   Int4              stop2;
   Uint1             strand;

   if (salp == NULL)
      return NULL;
   sap = NULL;
   if (salp->next != NULL)
   {
      sap_orig = SeqAlignDup(salp);
      AlnMgr2IndexLite(salp);
      AlnMgr2SortAlnSetByNthRowPos(salp, 1);
      SPI_RemoveInconsistentAlnsFromSet(salp, 10, 1, SPI_LEFT);
      amaip = (AMAlignIndex2Ptr)(salp->saip);
      last = 0;
      sap_new = NULL;
      for (i=0; i<amaip->numsaps-1; i++)
      {
         amaip->saps[i]->next = NULL;
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, &start1, &stop1);
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 1, &start2, &stop2);
         strand = AlnMgr2GetNthStrand(amaip->saps[i], 1);
         if (strand == Seq_strand_minus)
         {
            if (start1 <= stop2)
               AlnMgr2TruncateSeqAlign(amaip->saps[i], stop2+1, start1, 1);
            SeqAlignFree(amaip->saps[i]->next);
            amaip->saps[i]->next = NULL;
         } else
         {
            if (stop1 >= start2)
               AlnMgr2TruncateSeqAlign(amaip->saps[i], start1, start2-1, 1);
            SeqAlignFree(amaip->saps[i]->next);
            amaip->saps[i]->next = NULL;
         }
      }
      amaip->saps[amaip->numsaps-1]->next = NULL;
      for (i=0; i<amaip->numsaps-1; i++)
      {
         if (sap_new == NULL)
            sap_new = SeqAlignDup(amaip->saps[i]);
         sap_new2 = AlnMgr2MergeTwoAlignments(sap_new, amaip->saps[i+1]);
         if (sap_new2 == NULL)
            sap_new = amaip->saps[i];
         else
         {
            SeqAlignFree(sap_new);
            sap_new = sap_new2;
            sap_new2 = NULL;
         }
      }
      AlnMgr2IndexSingleChildSeqAlign(sap_new);
      if (AlnMgr2GetAlnLength(sap_orig, FALSE) >= AlnMgr2GetAlnLength(sap_new, FALSE))
      {
         SeqAlignFree(sap_new);
         sap_new = SeqAlignDup(sap_orig);
      }
      salp = sap_new;
   }
   sap = salp;
   AlnMgr2IndexSingleChildSeqAlign(sap);
   AlnMgr2ExtendToCoords(sap, 0, -1, 1);
   AlnMgr2GetNthSeqRangeInSA(sap, 1, &start1, &stop1);
   len = stop1 - start1 + 1; /* the actual length of sequence1 covered by aln */
   afp = AlnMgr2ComputeFreqMatrix(sap, 0, -1, 0);
   gaps = 0;
   mismatches = n = 0;
   alln = 0;
   for (i=0; i<afp->len; i++)
   {
      if (afp->freq[5][i] > 0)
         alln++;
      found = FALSE;
      for (j=0; !found && j<afp->size; j++)
      {
         if (afp->freq[0][i] > 0)
         {
            gaps++;
            found = TRUE;
         } else if (afp->freq[j][i] == 1 && afp->freq[5][i] == 0)
         {
            mismatches++;
            found = TRUE;
         } else if (afp->freq[j][i] == 1 && afp->freq[5][i] == 1)
         {
            n++;
            found = TRUE;
         }
      }
   }
   if (alln > 0 && (gaps>0 || mismatches>0 || n > 0))
   {
      spp = SeqPortNew(bsp1, 0, bsp1->length-1, Seq_strand_plus, Seq_code_ncbi4na);
      beg = end = 0;
      ctr = 0;
      nctr = 0;
      while ((res = SeqPortGetResidue(spp)) != SEQPORT_EOF)
      {
         if (res == 15)
         {
            if (beg == ctr)
               beg++;
            else
            {
               end++;
               nctr++;
            }
         } else
            end = 0;
         ctr++;
      }
      nctr = nctr - end;
      if (beg > 0 || end > 0)
      {
         sprintf(newstr, "The local sequence has %d terminal N%s. ", beg+end, beg+end!=1?"s":"");
         StringCat(*message, newstr);
      }
      if (nctr > 0)
      {
         sprintf(newstr, "The local sequence has %d internal N%s. ", nctr, nctr!=1?"s":"");
         StringCat(*message, newstr);
      }
      SeqPortFree(spp);
   }
   if ((*message[0] != '\0') && len == bsp1->length && gaps == 0 && mismatches == 0)
   {
      if (nctr > 0)
         *nonly = nctr+beg+end;
      else
         *nonly = -(beg+end);
      sprintf(newstr, "\nThere are no other differences between the local and database sequences.");
      StringCat(*message, newstr);
      return sap;
   } else
      *nonly = 0;
   if (len < bsp1->length)
   {
      sprintf(newstr, "%sThe alignment to the database sequence does not cover all of the local sequence. ", *message[0]=='\0'?"":"\n");
      StringCat(*message, newstr);
      spp = SeqPortNew(bsp1, len, bsp1->length-1, Seq_strand_plus, Seq_code_ncbi4na);
      alln = TRUE;
      while (alln && (res = SeqPortGetResidue(spp)) != SEQPORT_EOF)
      {
         if (res != 15)
            alln = FALSE;
      }
      if (alln)
         sprintf(newstr, "\nThe unaligned sequence consists only of Ns. ");
      else
         sprintf(newstr, "\nThere are non-N residues in the unaligned sequence. ");
      StringCat(*message, newstr);
      SeqPortFree(spp);
   }
   if (gaps > 0 || mismatches > 0 || n > 0)
   {
      sprintf(newstr, "%sThe alignment to the database sequence has %d gap%s, %d mismatch%s, and %d N-mismatch%s. ", afp->len<bsp1->length?"\n":"", gaps, (gaps!=1?"s":""), mismatches, (mismatches!=1?"es":""), n, (n!=1?"es":""));
      StringCat(*message, newstr);
   }
   return sap;
}

static void SeqAlignStartUpdate (SeqAlignPtr salp, SeqIdPtr target_sip, Int4 offset, Int4 len, Uint1 strand)
{
  SeqAlignPtr salptmp;
  DenseSegPtr dsp;
  SeqIdPtr    pre, sip, next;
  Int4Ptr     lenp,
              startp;
  Uint1Ptr    strandp;
  Int4        len_sum,
              start;
  Int2        index, k, j;

  if (salp==NULL || offset<=0)
     return;
  for (salptmp=salp; salptmp!=NULL; salptmp=salptmp->next)
  {
     if (salptmp->segtype == 2)
     {
        dsp = (DenseSegPtr) salptmp->segs;
        pre = NULL;
        index=0;
        sip=dsp->ids;
        while (sip)
        {
           next=sip->next;
           if (SeqIdForSameBioseq(target_sip, sip))
           {
              if (strand == Seq_strand_minus)
              {
                 strandp=dsp->strands;
                 if (strandp != NULL) {
                   strandp+=index;
                   for (j=0; j < dsp->numseg; j++)
                   {
                      if (*strandp == Seq_strand_minus)
                         *strandp = Seq_strand_plus;
                      else if (*strandp == Seq_strand_plus)
                         *strandp = Seq_strand_minus;
                      strandp+=dsp->dim;
                   }
                } else
                {
                   dsp->strands = (Uint1Ptr)MemNew(dsp->dim*dsp->numseg*sizeof(Uint1));
                   for (j=0; j<dsp->dim*dsp->numseg; j++)
                      dsp->strands[j] = Seq_strand_plus;
                   for(j=0; j<dsp->numseg; j++)
                      dsp->strands[j*dsp->dim + index] = Seq_strand_minus;
                }
                 lenp=dsp->lens;
                 startp=dsp->starts;
                 start=startp[index];
                 len_sum=0;
                 j=dsp->dim*dsp->numseg-dsp->dim;
                 k=dsp->numseg-1;
                 for (j=0; j<dsp->numseg; j++) {
                   if (startp[j*dsp->dim + index]>=0) {
                       startp[j*dsp->dim + index]=len-startp[j*dsp->dim + index]-dsp->lens[j]+1;
                   }
                 }
              } else
              {
                 for (j=0; j<dsp->numseg; j++) {
                    if (dsp->starts[dsp->dim*j+index] != -1)
                        dsp->starts[dsp->dim*j+index] += offset;
                 }
              }
           }
           pre=sip;
           sip=next;
           index++;
        }
     }
  }
}
static void showtextalign_fromalign (SeqAlignPtr salp, CharPtr path, FILE *fp)
{
  SeqAnnotPtr sap;
  Int4        line = 80;     
  Uint4       option = 0;
  Boolean     do_close = TRUE;

  if (salp == NULL || (path==NULL && fp==NULL))
     return;
  if (path != NULL && fp == NULL) {
     fp = FileOpen (path, "a");
  }
  else
     do_close = FALSE;
  if (fp != NULL) {
/********
{{
     option = RULER_TOP;
     option|=DISPE_SHOWBLOCK;
     option|=VIEW_VARIA;
     option|=DISP_FULL_TXT;
     DDV_DisplayDefaultAlign (salp, 0, -1,-1, option, NULL, fp); 
}}
*******/
     sap = SeqAnnotNew ();
     sap->type = 2;
     sap->data = (Pointer) salp;
     option += TXALIGN_MASTER;
     option += TXALIGN_MISMATCH;
     ShowTextAlignFromAnnot (sap, line, fp, NULL, NULL, option, NULL, NULL, NULL);
     sap->data = NULL;
     SeqAnnotFree (sap);
     if (do_close) {
        FileClose(fp);
     }
  } 
}

typedef struct besthit {
   SeqAlignPtr  sap;
   SeqIdPtr     sip1;
   SeqIdPtr     sip2;
   BioseqPtr    bsp1;
   BioseqPtr    bsp2;
  CharPtr      errstr;
   Int4         nonly;
   Int4         errtype;
    struct besthit PNTR next;
} BestHit, PNTR BestHitPtr;

static int LIBCALLBACK OrderBestHits(Pointer ptr1, Pointer ptr2)
{
   BestHitPtr  hip1;
   BestHitPtr  hip2;

   hip1 = *((BestHitPtr PNTR)(ptr1));
   hip2 = *((BestHitPtr PNTR)(ptr2));
   if (hip1->errtype < hip2->errtype)
      return -1;
   else if (hip1->errtype > hip2->errtype)
      return 1;
   else
      return 0;
}

static SeqIdPtr SWSeqIdReplaceID(SeqIdPtr sip_head, SeqIdPtr sip1, SeqIdPtr sip2)
{
   Boolean   found;
   SeqIdPtr  sip;
   SeqIdPtr  sip_prev;

   sip = sip_head;
   sip_prev = NULL;
   found = FALSE;
   while (sip != NULL && !found)
   {
      if (SeqIdComp(sip, sip1) == SIC_YES)
         found = TRUE;
      else
      {
         sip_prev = sip;
         sip = sip->next;
      }
   }
   if (!found)
      return sip_head;
   if (sip_prev == NULL)
   {
      sip2->next = sip_head->next;
      SeqIdFree(sip_head);
      return sip2;
   } else
   {
      sip_prev->next = sip2;
      sip2->next = sip->next;
      SeqIdFree(sip);
      return sip_head;
   }
}

static void SWPrintFarpointerAln(SeqAlignPtr sap, CharPtr filename)
{
   AlnMsg2Ptr  amp;
   BioseqPtr   bsp;
   BioseqPtr   bsp2;
   Uint1Ptr    buf;
   Int4        ctr;
   CharPtr     dig;
   Int4        first_from;
   Int4        i;
   Int4        l;
   Int4        len;
   Int4        linesize;
   FILE        *ofp;
   CharPtr     seq;
   CharPtr     seq1;
   CharPtr     seq2;
   SeqIdPtr    sip;
   SeqIdPtr    sip2;
   SeqPortPtr  spp;
   Int4        start;
   Int4        stop;
   Uint1       strand;
   Char        textid1[16];
   Char        textid2[16];
   CharPtr     tmp;

   linesize=70;
   buf = (Uint1Ptr)MemNew(70*sizeof(Uint1));
   if (sap == NULL || filename == NULL)
      return;
   if (sap->saip == NULL)
      AlnMgr2IndexSingleChildSeqAlign(sap);
   ofp = FileOpen(filename, "a");
   if (ofp == NULL)
      return;
   AlnMgr2GetNthSeqRangeInSA(sap, 1, &start, &stop);
   for (i=0; i<16; i++)
   {
      textid1[i] = textid2[i] = ' ';
   }
   sip = AlnMgr2GetNthSeqIdPtr(sap, 1);
   SeqIdWrite(sip, textid1, PRINTID_TEXTID_ACC_VER, 15);
   sip2 = AlnMgr2GetNthSeqIdPtr(sap, 2);
   SeqIdWrite(sip2, textid2, PRINTID_TEXTID_ACC_VER, 15);
   for (i=0; i<15; i++)
   {
      if (textid1[i] == '\0')
         textid1[i] = ' ';
      if (textid2[i] == '\0')
         textid2[i] = ' ';
   }
   textid1[15] = '\0';
   textid2[15] = '\0';
   bsp = BioseqLockById(sip);
   bsp2 = BioseqLockById(sip2);
   strand = AlnMgr2GetNthStrand(sap, 1);
   dig = (CharPtr)MemNew(6*sizeof(Char));
   for (l=0; l<start; l+=linesize)
   {
      spp = SeqPortNew(bsp, l, MIN(l+linesize-1, start-1), strand, Seq_code_iupacna);
      ctr = SeqPortRead(spp, buf, MIN(l+linesize-1, start-1)-l+1);
      buf[ctr] = '\0';
      sprintf(dig, "%d", l+1);
      for (i=StrLen(dig); i<5; i++)
      {
         dig[i] = ' ';
      }
      dig[i] = '\0';
      fprintf(ofp, "%s%c%s %s\n", textid1, strand == Seq_strand_plus?'>':'<', dig, buf);
      SeqPortFree(spp);
   }
   len = AlnMgr2GetAlnLength(sap, FALSE);
   amp = AlnMsgNew2();
   for (l=0; l<len; l+=linesize)
   {
      AlnMsgReNew2(amp);
      amp->from_aln = l;
      amp->to_aln = MIN(l+linesize-1, len-1);
      amp->row_num = 1;
      seq1 = (CharPtr)MemNew((linesize+1)*sizeof(Char));
      seq = seq1;
      first_from = -1;
      while (AlnMgr2GetNextAlnBit(sap, amp))
      {
         if (amp->type == AM_GAP)
         {
            for (i=0; i<amp->to_row - amp->from_row+1; i++)
            {
               *seq = '-';
               seq++;
            }
         } else if (amp->type == AM_SEQ)
         {
            if (first_from == -1)
            {
               if (amp->strand == Seq_strand_minus)
                  first_from = amp->to_row+1;
               else
                  first_from = amp->from_row+1;
            }
            spp = SeqPortNew(bsp, amp->from_row, amp->to_row, amp->strand, Seq_code_iupacna);
            ctr = SeqPortRead(spp, buf, amp->to_row-amp->from_row+1);
            buf[ctr] = '\0';
            SeqPortFree(spp);
            for (i=0; i<ctr; i++)
            {
               *seq = buf[i];
               seq++;
            }
         }
      }
      sprintf(dig, "%d", first_from);
      for (i=StrLen(dig); i<5; i++)
      {
         dig[i] = ' ';
      }
      dig[i] = '\0';
      fprintf(ofp, "%s%c%s %s\n", textid1, amp->strand==Seq_strand_plus?'>':'<', dig, seq1);
      AlnMsgReNew2(amp);
      amp->from_aln = l;
      amp->to_aln = MIN(l+linesize-1, len-1);
      amp->row_num = 2;
      seq2 = (CharPtr)MemNew((linesize+1)*sizeof(Char));
      seq = seq2;
      first_from = -1;
      while (AlnMgr2GetNextAlnBit(sap, amp))
      {
         if (amp->type == AM_GAP)
         {
            for (i=0; i<amp->to_row - amp->from_row+1; i++)
            {
               *seq = '-';
               seq++;
            }
         } else if (amp->type == AM_SEQ)
         {
            if (first_from == -1)
            {
               if (amp->strand == Seq_strand_minus)
                  first_from = amp->to_row+1;
               else
                  first_from = amp->from_row+1;
            }
            spp = SeqPortNew(bsp2, amp->from_row, amp->to_row, amp->strand, Seq_code_iupacna);
            ctr = SeqPortRead(spp, buf, amp->to_row-amp->from_row+1);
            buf[ctr] = '\0';
            SeqPortFree(spp);
            for (i=0; i<ctr; i++)
            {
               *seq = buf[i];
               seq++;
            }
         }
      }
      sprintf(dig, "%d", first_from);
      for (i=StrLen(dig); i<5; i++)
      {
         dig[i] = ' ';
      }
      dig[i] = '\0';
      fprintf(ofp, "%s%c%s ", textid2, amp->strand==Seq_strand_plus?'>':'<', dig);
      tmp = seq1;
      seq = seq2;
      while (*tmp != '\0')
      {
         if (*tmp == *seq)
            fprintf(ofp, ".");
         else
            fprintf(ofp, "%c", *seq);
         tmp++;
         seq++;
      }
      fprintf(ofp, "\n\n");
   }
   AlnMsgFree2(amp);
   for (l=stop+1; l<bsp->length; l+=linesize)
   {
      spp = SeqPortNew(bsp, l, MIN(l+linesize-1, bsp->length-1), strand, Seq_code_iupacna);
      ctr = SeqPortRead(spp, buf, MIN(l+linesize-1, bsp->length-1)-l+1);
      buf[ctr] = '\0';
      sprintf(dig, "%d", l+1);
      for (i=StrLen(dig); i<5; i++)
      {
         dig[i] = ' ';
      }
      dig[i] = '\0';
      fprintf(ofp, "%s%c%s %s\n", textid1, strand == Seq_strand_plus?'>':'<', dig, buf);
      SeqPortFree(spp);
   }
   BioseqUnlock(bsp);
   SeqIdFree(sip);
   MemFree(buf);
   FileClose(ofp);
}


static ValNodePtr CCNormalizeSeqAlignId (SeqAlignPtr salp, ValNodePtr vnp)
{
  BLAST_OptionsBlkPtr options;
  MsgAnswer           ans;
  DenseSegPtr         dsp;
  SeqIdPtr            sip,
                      dbsip = NULL,
                      lclsip,
                      presip, 
                      next,
                      tmpsip;
  SeqAlignPtr         seqalign = NULL;
  SeqAlignPtr         bestsalp = NULL;
  CharPtr             TmpBuff, tmp;
  Char                str [52];
  Int4                gi = 0,
                      offset,
                      totlenlcl = 0, totlendb = 0;
  Int4                i, j, k, len = 0, n;
  Int2                index;
  Uint1               strand;
  Boolean             ok, 
                      found;
  
  Char                strLog[50];
  BioseqPtr           bsp1, bsp2;
  Int4                start1, start2, stop1, stop2;
  CharPtr             errstr;
  Int4                nonly;
  BestHitPtr          hip, hip_prev, hip_head;
  BestHitPtr          PNTR hiparray;
  Char                messagestr[1500];
  Int4                numhips;
  FILE                *ofp;

  hip_prev = hip_head = NULL;
  errstr = (CharPtr)MemNew(500*sizeof(Char));
  if (salp!=NULL) {
     if (salp->segtype == 2) {
        dsp = (DenseSegPtr) salp->segs;
        presip = NULL;
        sip = dsp->ids;
        index = 0;
        found = FALSE;
        while (sip != NULL) 
        {
           next = sip->next;
           lclsip = SeqIdDup (sip);
           SeqIdWrite (lclsip, str, PRINTID_FASTA_LONG, 50);
           tmp = StringStr (str, "acc");
           if (tmp==NULL) 
           {
              tmp = StringStr (str, "ACC");
           }
           if (tmp!=NULL) {
              tmp++; tmp++; tmp++;
              if (*tmp == '|')
                 tmp++;   
              TmpBuff = tmp;
              while (*tmp!='\0' && *tmp != '|' && *tmp!='\n')
                 tmp++;
              *tmp = '\0';

              ok = FALSE;
              j = StringLen (TmpBuff);
              for(k =0; k < j; k++) {
                 if(!isdigit(TmpBuff[k])) {
                    break;
                 }
              }
              dbsip=NULL;
              if(k != j) {
                 ok=(IS_ntdb_accession(TmpBuff) || IS_protdb_accession(TmpBuff));
                 if (ok) {
                    dbsip = SeqIdFromAccession (TmpBuff, 0, NULL);
                 }
              }
              else {
                 gi = (Int4)atol(TmpBuff);
                 if (gi>0) {
                    dbsip = ValNodeNew (NULL);
                    if (dbsip) {
                       dbsip->choice = SEQID_GI;
                       dbsip->data.intvalue = (Int4)gi;
                    }
                 }
              }
              if (dbsip!=NULL) {
                 bsp1 = BioseqLockById(lclsip);
                 bsp2 = BioseqLockById(dbsip);
                 if ( bsp1 != NULL && bsp2 != NULL && bsp1->length > 0 && bsp2->length > 0) {
                 options = BLASTOptionNew("blastn", TRUE);
                 options->filter_string = StringSave("m L;R");
                 seqalign = BlastTwoSequences (bsp1, bsp2, "blastn", options);
                 if (errstr != NULL)
                    MemFree(errstr);
                 errstr = (CharPtr)MemNew(1000*sizeof(Char));
                 bestsalp = SeqAlignBestHit (seqalign, bsp1, bsp2, 100, &errstr, &nonly);
                 hip = (BestHitPtr)MemNew(sizeof(BestHit));
                 hip->sap = bestsalp;
                 hip->sip1 = lclsip;
                 if (dbsip->choice != SEQID_GI && bsp2->id != NULL) {
                   /* recreate sip to get correct version number */
                   for (tmpsip = bsp2->id; tmpsip != NULL; tmpsip = tmpsip->next) {
                     if (tmpsip->choice == dbsip->choice) break;
                   }
                   if (tmpsip != NULL) {
                     dbsip = SeqIdFree (dbsip);
                     dbsip = SeqIdStripLocus (SeqIdDup (tmpsip));
                   }
                 }
                 hip->sip2 = dbsip;
                 hip->bsp1 = bsp1;
                 hip->bsp2 = bsp2;
                 hip->errstr = StringSave(errstr);
                 hip->nonly = nonly;
                 if (*hip->errstr == '\0')
                    hip->errtype = 0;
                 else if (nonly < 0)
                    hip->errtype = 1;
                 else
                    hip->errtype = 2;
                 if (hip_head != NULL)
                 {
                    hip_prev->next = hip;
                    hip_prev = hip;
                 } else
                    hip_head = hip_prev = hip;
             } else {
                 if (totlendb == 0) {
                    SeqIdWrite(dbsip, strLog, PRINTID_TEXTID_ACCESSION, 50);
                    sprintf(errstr, "This alignment contains \"%s\" that can not be found in GenBank.\nPlease check the accession number.\n", strLog);
                   sip->next = next;
                 } else if (totlenlcl == 0) {
                   SeqIdWrite (lclsip, strLog, PRINTID_TEXTID_ACCESSION, 50);
                   sprintf(errstr, "This alignment contains \"%s\" that can not be found.\nPlease check the accession number.\n", strLog);
                   sip->next = next;
                 }
                 hip = (BestHitPtr)MemNew(sizeof(BestHit));
                 hip->sap = bestsalp;
                 hip->sip1 = lclsip;
                 hip->sip2 = dbsip;
                 hip->errstr = StringSave(errstr);
                 hip->nonly = 0;
                 hip->errtype = 3;
                 hip->bsp1 = NULL;
                 hip->bsp2 = NULL;
                 if (hip_head != NULL)
                 {
                    hip_prev->next = hip;
                    hip_prev = hip;
                 } else
                    hip_head = hip_prev = hip;
              }
              } else {
                 SeqIdWrite (sip, strLog, PRINTID_TEXTID_ACCESSION, 50);
                 sprintf(errstr, "This alignment contains \"%s\" that can not be found in GenBank.\nPlease check the accession number.\n", strLog);
                 sip->next = next;
                 hip = (BestHitPtr)MemNew(sizeof(BestHit));
                 hip->sap = bestsalp;
                 hip->sip1 = lclsip;
                 hip->sip2 = dbsip;
                 hip->errstr = StringSave(errstr);
                 hip->nonly = 0;
                 hip->errtype = 3;
                 hip->bsp1 = NULL;
                 hip->bsp2 = NULL;
                 if (hip_head != NULL)
                 {
                    hip_prev->next = hip;
                    hip_prev = hip;
                 } else
                    hip_head = hip_prev = hip;
              }
           }
           presip = sip;
           sip = next;
           index++;
           found = FALSE;
        }
        hip = hip_head;
        numhips = 0;
        while (hip != NULL)
        {
           numhips++;
           hip = hip->next;
        }
        hiparray = (BestHitPtr PNTR)MemNew(numhips*sizeof(BestHitPtr));
        hip = hip_head;
        numhips = 0;
        while (hip != NULL)
        {
           hiparray[numhips] = hip;
           numhips++;
           hip = hip->next;
        }
        HeapSort(hiparray, numhips, sizeof(BestHitPtr), OrderBestHits);
        i = 0;
        while (i<numhips && *hiparray[i]->errstr == '\0')
        {
           i++;
        }
        if (i > 0)
        {
           messagestr[0] = '\0';
           for (j=0; j<i; j++)
           {
              SeqIdWrite(hiparray[j]->sip2, strLog, PRINTID_TEXTID_ACCESSION, 50);
              StringCat(messagestr, strLog);
              StringCat(messagestr, ", ");
           }
           ans = Message(MSG_OKC, "This alignment contains %s that %s already in GenBank. \nDo you wish to replace %s?", messagestr, i>1?"are":"is", i>1?"them":"it");
           if (ans != ANS_CANCEL)
           {
              for (j=0; j<i; j++)
              {
                 offset = SeqAlignStart(hiparray[j]->sap, 1)-SeqAlignStart(hiparray[j]->sap, 0);
                 if ((SeqAlignStrand(hiparray[j]->sap, 0)==Seq_strand_minus && SeqAlignStrand(hiparray[j]->sap, 1) != Seq_strand_minus) || (SeqAlignStrand(hiparray[j]->sap, 1)==Seq_strand_minus && SeqAlignStrand(hiparray[j]->sap, 0) != Seq_strand_minus))
                 {
                     strand=Seq_strand_minus;
                     AlnMgr2IndexSingleChildSeqAlign(hiparray[j]->sap);
                     AlnMgr2GetNthSeqRangeInSA(hiparray[j]->sap, 1, &start1, &stop1);
                     AlnMgr2GetNthSeqRangeInSA(hiparray[j]->sap, 2, &start2, &stop2);
                     len = stop2 + start1;
                 } else
                      strand=Seq_strand_plus;
                 SeqAlignStartUpdate (salp, hiparray[j]->sip1, abs(offset), len, strand);
                 dsp->ids = SWSeqIdReplaceID(dsp->ids, hiparray[j]->sip1, hiparray[j]->sip2);

                 if (presip)
                    sip = presip->next;
                 else
                    sip = dsp->ids;
                 /*SeqAlignReplaceId (hiparray[j]->sip1, hiparray[j]->sip2, salp);*/
                 vnp = nrSeqIdAdd (vnp, hiparray[j]->sip1);
                 found = TRUE;
                 SeqAlignFree(hiparray[j]->sap);
                 if (hiparray[j]->bsp1 != NULL) {
                   BioseqUnlock (hiparray[j]->bsp1);
                   hiparray[j]->bsp1->idx.deleteme = TRUE;
                   hiparray[j]->bsp1 = NULL;
                 }
                 if (hiparray[j]->bsp2 != NULL) {
                   BioseqUnlock (hiparray[j]->bsp2);
                   hiparray[j]->bsp2 = NULL;
                 }
              }
           } else /* need to ask one by one */
           {
              for (j=0; j<i; j++)
              {
                 SeqIdWrite(hiparray[j]->sip2, strLog, PRINTID_TEXTID_ACCESSION, 50);
                 ans = Message(MSG_OKC, "This alignment contains %s that is already in GenBank. \nDo you wish to replace it?", strLog);
                 if (ans != ANS_CANCEL)
                 {
                    offset = SeqAlignStart(hiparray[j]->sap, 1)-SeqAlignStart(hiparray[j]->sap, 0);
                    if ((SeqAlignStrand(hiparray[j]->sap, 0)==Seq_strand_minus && SeqAlignStrand(hiparray[j]->sap, 1) != Seq_strand_minus) || (SeqAlignStrand(hiparray[j]->sap, 1)==Seq_strand_minus && SeqAlignStrand(hiparray[j]->sap, 0) != Seq_strand_minus))
                    {
                        strand=Seq_strand_minus;
                        AlnMgr2IndexSingleChildSeqAlign(hiparray[j]->sap);
                        AlnMgr2GetNthSeqRangeInSA(hiparray[j]->sap, 1, &start1, &stop1);
                        AlnMgr2GetNthSeqRangeInSA(hiparray[j]->sap, 2, &start2, &stop2);
                        len = stop2 + start1;
                    } else
                         strand=Seq_strand_plus;
                    SeqAlignStartUpdate (salp, hiparray[j]->sip1, offset, len, strand);
                    dsp->ids = SWSeqIdReplaceID(dsp->ids, hiparray[j]->sip1, hiparray[j]->sip2);
                    if (presip)
                       sip = presip->next;
                    else
                       sip = dsp->ids;
                    SeqAlignReplaceId (hiparray[j]->sip1, hiparray[j]->sip2, salp);
                    vnp = nrSeqIdAdd (vnp, hiparray[j]->sip1);
                    found = TRUE;
                    if (hiparray[j]->bsp1 != NULL) {
                      hiparray[j]->bsp1->idx.deleteme = TRUE;
                    }
                 }
                 SeqAlignFree(hiparray[j]->sap);
                 if (hiparray[j]->bsp1 != NULL) {
                   BioseqUnlock (hiparray[j]->bsp1);
                   hiparray[j]->bsp1 = NULL;
                 }
                 if (hiparray[j]->bsp2 != NULL) {
                   BioseqUnlock (hiparray[j]->bsp2);
                   hiparray[j]->bsp2 = NULL;
                 }
              }
           }
        }
        n = i;
        i = 0;
      /*  while (n<numhips && hiparray[n]->nonly < 0)
        {
           i++;
           n++;
        }
        if (i > 0)
        {
           messagestr[0] = '\0';
           for (j=0; j<i; j++)
           {
              SeqIdWrite(hiparray[j+n]->sip2, strLog, PRINTID_TEXTID_ACCESSION, 50);
              StringCat(messagestr, strLog);
              StringCat(messagestr, ", ");
           }
           ans = Message(MSG_OKC, "This alignment contains %s that %s already in GenBank. \n However, the local sequence%s %s terminal Ns.\nDo you wish to replace %s?", messagestr, i>1?"are":"is", i>1?"s":"", i>1?"have":"has", i>1?"them": "it");
           if (ans != ANS_CANCEL)
           {
              for (j=0; j<i; j++)
              {
                 offset = SeqAlignStart(hiparray[j+n]->sap, 1)-SeqAlignStart(hiparray[j+n]->sap, 0);
                 if ((SeqAlignStrand(hiparray[j+n]->sap, 0)==Seq_strand_minus && SeqAlignStrand(hiparray[j+n]->sap, 1) != Seq_strand_minus) || (SeqAlignStrand(hiparray[j+n]->sap, 1)==Seq_strand_minus && SeqAlignStrand(hiparray[j+n]->sap, 0) != Seq_strand_minus))
                 {
                     strand=Seq_strand_minus;
                     AlnMgr2IndexSingleChildSeqAlign(hiparray[j+n]->sap);
                     AlnMgr2GetNthSeqRangeInSA(hiparray[j+n]->sap, 1, &start1, &stop1);
                     AlnMgr2GetNthSeqRangeInSA(hiparray[j+n]->sap, 2, &start2, &stop2);
                     len = stop2 + start1;
                 } else
                      strand=Seq_strand_plus;
                 SeqAlignStartUpdate (salp, hiparray[j+n]->sip1, abs(offset), len, strand);
                 dsp->ids = SWSeqIdReplaceID(dsp->ids, hiparray[j+n]->sip1, hiparray[j+n]->sip2);
                 if (presip)
                    sip = presip->next;
                 else
                    sip = dsp->ids;
                 SeqAlignReplaceId (hiparray[j+n]->sip1, hiparray[j+n]->sip2, salp);
                 vnp = nrSeqIdAdd (vnp, hiparray[j+n]->sip1);
                 found = TRUE;
                 SeqAlignFree(hiparray[j+n]->sap);
              }
           } else
           {
              for (j=0; j<i; j++)
              {
                 SeqIdWrite(hiparray[j+n]->sip2, strLog, PRINTID_TEXTID_ACCESSION, 50);
                 ans = Message(MSG_OKC, "This alignment contains %s that is already in GenBank. \n %s\nDo you wish to replace it?", strLog, hiparray[j+n]->errstr);
                 if (ans != ANS_CANCEL)
                 {
                    offset = SeqAlignStart(hiparray[j+n]->sap, 1)-SeqAlignStart(hiparray[j+n]->sap, 0);
                    if ((SeqAlignStrand(hiparray[j+n]->sap, 0)==Seq_strand_minus && SeqAlignStrand(hiparray[j+n]->sap, 1) != Seq_strand_minus) || (SeqAlignStrand(hiparray[j+n]->sap, 1)==Seq_strand_minus && SeqAlignStrand(hiparray[j+n]->sap, 0) != Seq_strand_minus))
                    {
                        strand=Seq_strand_minus;
                        AlnMgr2IndexSingleChildSeqAlign(hiparray[j+n]->sap);
                        AlnMgr2GetNthSeqRangeInSA(hiparray[j+n]->sap, 1, &start1, &stop1);
                        AlnMgr2GetNthSeqRangeInSA(hiparray[j+n]->sap, 2, &start2, &stop2);
                        len = stop2 + start1;
                    } else
                         strand=Seq_strand_plus;
                    SeqAlignStartUpdate (salp, hiparray[j+n]->sip1, offset, len, strand);
                    dsp->ids = SWSeqIdReplaceID(dsp->ids, hiparray[j+n]->sip1, hiparray[j+n]->sip2);
                    if (presip)
                       sip = presip->next;
                    else
                       sip = dsp->ids;
                    SeqAlignReplaceId (hiparray[j+n]->sip1, hiparray[j+n]->sip2, salp);
                    vnp = nrSeqIdAdd (vnp, hiparray[j+n]->sip1);
                    found = TRUE;
                    SeqAlignFree(hiparray[j+n]->sap);
                 }
              }
           }
        } */
        for (j=n+i; j<numhips; j++)
        {
           if (hiparray[j]->errtype <3)
           {
               SeqIdWrite (hiparray[j]->sip2, strLog, PRINTID_TEXTID_ACCESSION, 50);
               ans = Message (MSG_OKC, "This alignment contains \"%s\" that is already in GenBank.\n However, the local version is not identical to the most recent database version.\n %s \nDo you wish to replace it anyway ?\n If you cancel, the alignment of the local and the database versions \nof \"%s\" will be saved in the error file \"error.log\"", strLog, hiparray[j]->errstr, strLog);
               if (ans != ANS_CANCEL)
               {
                  offset = SeqAlignStart(hiparray[j]->sap, 1)-SeqAlignStart(hiparray[j]->sap, 0);
                  if (SeqAlignStrand(hiparray[j]->sap, 0)==Seq_strand_minus || SeqAlignStrand(hiparray[j]->sap, 1)==Seq_strand_minus)
                     strand=Seq_strand_minus;
                  else
                     strand=Seq_strand_plus;
                  SeqAlignStartUpdate (salp, hiparray[j]->sip1, offset, len, strand);
                  dsp->ids = SWSeqIdReplaceID(dsp->ids, hiparray[j]->sip1, hiparray[j]->sip2);
                  if (presip)
                     sip = presip->next;
                  else
                     sip = dsp->ids;
                  SeqAlignReplaceId (hiparray[j]->sip1, hiparray[j]->sip2, salp);
                  vnp = nrSeqIdAdd (vnp, hiparray[j]->sip1);
                  found = TRUE;
               }
               else {
                  ofp = FileOpen("error.log", "a");
                  fprintf(ofp, "This alignment contains %s that is already in GenBank; \nhowever, the local version is not identical to the most recent database version.\n %s\n", strLog, hiparray[j]->errstr);
                  FileClose(ofp);
                  SWPrintFarpointerAln(hiparray[j]->sap, "error.log");
               }
            } else
               Message(MSG_OK, "%s", hiparray[j]->errstr);
         }
     }
  }
  return vnp;  
}

/* we need to iterate through the actual SeqEntries, because theoretically the
 * same SeqID should appear in the SeqEntry with the new alignment and again
 * in the SeqEntry of the original record.
 */
static BioseqPtr FindBioseqInSep (SeqEntryPtr sep, SeqIdPtr sip)
{
  BioseqPtr    bsp = NULL;
  BioseqSetPtr bssp;
  SeqEntryPtr  this_sep;
  
  if (sep == NULL || sip == NULL) return NULL;
  
  if (IS_Bioseq (sep))
  {
  	bsp = (BioseqPtr) sep->data.ptrvalue;
	if (! BioseqMatch(bsp, sip))
	{
	  bsp = NULL;
	}
  }
  else if (IS_Bioseq_set (sep))
  {
  	bssp = (BioseqSetPtr) sep->data.ptrvalue;
    for (this_sep = bssp->seq_set; this_sep != NULL && bsp == NULL; this_sep = this_sep->next)
    {
      bsp = FindBioseqInSep (this_sep, sip);
    }
  }
  return bsp;
}

extern void CalculateAlignmentOffsets (SeqEntryPtr sepnew, SeqEntryPtr sepold)
{
  SeqAlignPtr         salpnew;
  DenseSegPtr         dsp;
  SeqIdPtr            sip_temp, sip_next;
  BioseqPtr           bsp1, bsp2;
  BLAST_OptionsBlkPtr options;
  SeqAlignPtr         seqalign = NULL;
  SeqAlignPtr         bestsalp = NULL;
  Int4                start1, start2, stop1, stop2;
  CharPtr             errstr = NULL;
  Uint1               strand;
  Int4                offset, len, nonly;
  BioseqPtr           copybsp1, copybsp2;
  SeqIdPtr            tmp_id_1, tmp_id_2;
  
  if (sepnew == NULL || sepold == NULL)
  {
  	return;
  }
  /* this function needs to look at the bioseqs we have created while reading in the
   * alignment, align them with the existing bioseqs, and adjust the alignment start
   * positions if necessary.
   */

  salpnew = (SeqAlignPtr) FindSeqAlignInSeqEntry (sepnew, OBJ_SEQALIGN);
  if (salpnew == NULL)
  {
  	return;
  }
  
  if (salpnew->segtype != 2) return;
  dsp = (DenseSegPtr) salpnew->segs;
  if (dsp == NULL) return;

  /* create IDs to use when copying Bioseqs.
   * BioseqCopyEx makes a copy of these for the Bioseq it creates,
   * so we can reuse them and then free them at the end of the for-next loop.
   */
  tmp_id_1 = MakeSeqID ("lcl|tmp_1_for_update");
  tmp_id_2 = MakeSeqID ("lcl|tmp_2_for_update");
  
  for (sip_temp = dsp->ids; sip_temp != NULL; sip_temp = sip_next)
  {
  	sip_next = sip_temp->next;
  	sip_temp->next = NULL;
  	
  	/* find bsp1 in sepnew, bsp2 in sepold */
    bsp1 = FindBioseqInSep (sepnew, sip_temp);
    bsp2 = FindBioseqInSep (sepold, sip_temp);
    
    if (bsp1 != NULL && bsp2 != NULL) 
    {
  	  /* create alignment between old and new bioseqs */
  	  /* new bioseq will have same ID as old bioseq, so BLAST won't work
  	   * because it's looking for IDs using indexing.
  	   * Create a temporary copy of the two bioseqs with different IDS,
  	   * add them to the BioseqIndex, BLAST them, then remove them 
  	   * from the index and delete them.
  	   */
      copybsp1 = BioseqCopyEx (tmp_id_1, bsp1, 0, bsp1->length - 1, Seq_strand_plus, FALSE);
      copybsp2 = BioseqCopyEx (tmp_id_2, bsp2, 0, bsp2->length - 1 , Seq_strand_plus, FALSE);
      SeqMgrAddToBioseqIndex (copybsp1);
      SeqMgrAddToBioseqIndex (copybsp2);
 	   
      options = BLASTOptionNew("blastn", TRUE);
      options->filter_string = StringSave("m L;R");
      seqalign = BlastTwoSequences (copybsp1, copybsp2, "blastn", options);
      if (errstr != NULL)
        MemFree(errstr);
      errstr = (CharPtr)MemNew(1000*sizeof(Char));
      bestsalp = SeqAlignBestHit (seqalign, copybsp1, copybsp2, 100, &errstr, &nonly);
  
      /* we don't need the copies after this, and we don't want them in the BioseqIndex
       * (or BLAST might get confused the next time through the loop).
       */	
  	  copybsp1->idx.deleteme = TRUE;
  	  copybsp2->idx.deleteme = TRUE;
  	  SeqMgrDeleteFromBioseqIndex (copybsp1);
  	  SeqMgrDeleteFromBioseqIndex (copybsp2);
  	
  	  /* update start position in alignment */
      offset = SeqAlignStart(bestsalp, 1)-SeqAlignStart(bestsalp, 0);
      if ((SeqAlignStrand(bestsalp, 0)==Seq_strand_minus && SeqAlignStrand(bestsalp, 1) != Seq_strand_minus) || (SeqAlignStrand(bestsalp, 1)==Seq_strand_minus && SeqAlignStrand(bestsalp, 0) != Seq_strand_minus))
      {
        strand=Seq_strand_minus;
        AlnMgr2IndexSingleChildSeqAlign(bestsalp);
        AlnMgr2GetNthSeqRangeInSA(bestsalp, 1, &start1, &stop1);
        AlnMgr2GetNthSeqRangeInSA(bestsalp, 2, &start2, &stop2);
        len = stop2 + start1;
      } 
      else
      {
        strand=Seq_strand_plus;
      }
      SeqAlignStartUpdate (salpnew, sip_temp, abs(offset), len, strand);	    	
    }  	
  	sip_temp->next = sip_next;
  }
  SeqIdFree (tmp_id_1);
  SeqIdFree (tmp_id_2);
    
  if (errstr != NULL)
  {
  	MemFree (errstr);
  }
}


static Boolean check_dbid_seqalign (SeqAlignPtr salp)
{
  DenseSegPtr dsp;
  SeqIdPtr    sip, next;
  Char        str [52];
  CharPtr     TmpBuff, tmp;
  Int4        j, k;
  Boolean     found = FALSE;

  if (salp!=NULL) 
  {
     if (salp->segtype == 2) 
     {
        dsp = (DenseSegPtr) salp->segs;
        sip = dsp->ids;
        while (!found && sip != NULL) 
        {
           next = sip->next;
           sip->next = NULL;
           SeqIdWrite (sip, str, PRINTID_FASTA_LONG, 50);
           sip->next = next;
           tmp = StringStr (str, "acc");
           if (tmp!=NULL) {
              tmp++; tmp++; tmp++;
              if (*tmp == '|')
                 tmp++;
              TmpBuff = tmp;
              while (*tmp!='\0' && *tmp != '|' && *tmp!='\n')
                 tmp++;
              *tmp = '\0';

              j = StringLen (TmpBuff);
              for(k =0; k < j; k++) {
                 if(!isdigit(TmpBuff[k])) {
                    break;
                 }
              }
              if(k != j) {
                found=(IS_ntdb_accession(TmpBuff) || IS_protdb_accession(TmpBuff));
              }
           }  
           sip = next;
        }     
     }
  }     
  return found;
}


/******************************************************************
validate each alignment sequentially.  
This function will subject the seqalign to all validation functions
******************************************************************/ 
/*********************************************************/
static void delete_bioseqs (ValNodePtr ids, Uint2 entityID)
{
  SeqEntryPtr  sep_top;
  SeqEntryPtr  sep_del;
  ValNodePtr   vnp;
  SeqIdPtr     sip;
  SeqLocPtr    slp;
  BioseqPtr    bsp;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;
  Uint2          parenttype;
  Pointer        parentptr;

  if (ids == NULL)
     return;
  sep_top = GetTopSeqEntryForEntityID (entityID);
  SaveSeqEntryObjMgrData (sep_top, &omdptop, &omdata);
  GetSeqEntryParent (sep_top, &parentptr, &parenttype);

  vnp=ids;
  while (vnp!=NULL)
  {
     sip = (SeqIdPtr) vnp->data.ptrvalue;
     if (sip!=NULL) {
        slp = (SeqLocPtr)ValNodeNew (NULL);
        slp->choice = SEQLOC_WHOLE;
        slp->data.ptrvalue = sip;
        bsp = GetBioseqGivenSeqLoc (slp, entityID);
        if (bsp!=NULL) {
           sep_del=GetBestTopParentForData (entityID, bsp);
           RemoveSeqEntryFromSeqEntry (sep_top, sep_del, FALSE);
        }
        slp->data.ptrvalue = NULL;
        SeqLocFree (slp);
     }
     vnp=vnp->next;
  }
  SeqMgrLinkSeqEntry (sep_top, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep_top, omdptop, &omdata);
  RenormalizeNucProtSets (sep_top, TRUE);

  for (vnp=ids; vnp!=NULL; vnp=vnp->next) {
     SeqIdFree ((SeqIdPtr) vnp->data.ptrvalue);
     vnp->data.ptrvalue = NULL;
  }
  ValNodeFree (vnp);
  return;
}

NLM_EXTERN Boolean ValidateSeqAlignandACC (SeqAlignPtr salp, Uint2 entityID, Boolean message,
                         Boolean msg_success, Boolean find_remote_bsp,Boolean find_acc_bsp,
                         Boolean delete_bsp, Boolean delete_salp, BoolPtr dirty)
{  
  SeqAlignPtr  pre,
               salptmp;
  SaVal        sv;
  SaValPtr     svp;
  MsgAnswer    ans;
  Int2         err_count=0,
               salp_count=0;
  Boolean      ok; 

  if(salp!=NULL)
  {
        sv.message = message;
        sv.msg_success = msg_success;
        sv.find_remote_bsp = find_remote_bsp;
        sv.find_acc_bsp = find_acc_bsp;
        sv.delete_salp = delete_salp;
        sv.delete_bsp = delete_bsp;
        sv.retdel = TRUE;
        sv.ids = NULL;
        sv.entityID = entityID; 
        sv.dirty = FALSE;   
        svp = &sv;   
     pre=NULL;
     salptmp=salp; 
     while (salptmp)
     {
        salp_count++;
        if(salp->segtype==5)
        {
           ValidateSeqAlignandACC ((SeqAlignPtr) (salptmp->segs), entityID, message, msg_success, find_remote_bsp, find_acc_bsp, delete_bsp, delete_salp, &svp->dirty);
        } 
        else if (salp->segtype<1 || salp->segtype>4)
        {
           ValMessage (Err_Segtype, SEV_ERROR, NULL, NULL, salptmp->segtype);
        }
        else {
           ValidateSeqAlign (salptmp, svp->entityID, svp->message, svp->msg_success, svp->find_remote_bsp, svp->delete_bsp, svp->delete_salp, &svp->dirty);
           if (svp->find_acc_bsp) {
	      ok = check_dbid_seqalign (salptmp);
	      if (ok) {
                 svp->ids = CCNormalizeSeqAlignId (salptmp, svp->ids);
                 if (svp->ids!=NULL && svp->entityID > 0) {
                    if (svp->delete_bsp)
                       delete_bioseqs (svp->ids, svp->entityID); 
                    svp->dirty = TRUE;
                 }
              }       	
           }
        }     	
       	if (errorp)
       	{
       	   if(svp->message)
       	   {
              BlastErrorPrint (errorp);
       	      errorp = BlastErrorChainDestroy (errorp);
       	   }
       	   if (svp->delete_salp)
       	   {
            if (pre==NULL) {
              salp=salptmp->next;
              salptmp->next = NULL;
              SeqAlignFree (salptmp);
              salptmp = salp;
            }
            else {
              pre->next = salptmp->next;
              salptmp->next = NULL;
              SeqAlignFree (salptmp);
              salptmp = pre->next;
            }
           }
       	   else {
       	      salptmp = salptmp->next;
       	   }
       	   err_count++;
           svp->retdel=FALSE;
        }
       	else {
       	   salptmp = salptmp->next;
       	}
     }
     if (err_count==0 && svp->msg_success) {
        if (salp_count>1)
           ans = Message (MSG_OK, "Validation test of %d alignments succeded", salp_count);
        else
           ans = Message (MSG_OK, "Validation test of the alignment succeded");
     }
     if (dirty)
        *dirty = svp->dirty;
  }   
  return svp->retdel;
} 


/******************************************************************
call back function for REGISTER_ALIGNVALIDATION defined in sequin4.c.  
Starting point for seqalign validation if user clicked on 
SeqalignValidation under menu Filer/Alignment.  
Either individual alignment or alignment block 
should be highlighted for this validation to work
******************************************************************/ 

static Pointer LIBCALL sap_empty (SeqAnnotPtr sap, Uint1 type, Pointer PNTR ptr)
{
  SeqAlignPtr      salp = NULL;

  if (sap != NULL) {
     for (; sap!= NULL; sap=sap->next) {
        if (sap->type == type) {
           salp = (SeqAlignPtr) sap->data;
           if (ptr!=NULL)
              *ptr = (Pointer) sap;
           break;
        }
     }
  }
  return salp;
}

NLM_EXTERN Int2 LIBCALLBACK ValidateSeqAlignandACCFromData (Pointer data)
{ 
 
  OMProcControlPtr  ompcp;
  SeqAlignPtr       salp=NULL;
  SeqAnnotPtr       sap=NULL;
  SeqEntryPtr       sep=NULL;
  
  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  
  switch(ompcp->input_itemtype)
    {
    case OBJ_BIOSEQ :
      sep = SeqMgrGetSeqEntryForData (ompcp->input_data);
      break;
    case OBJ_BIOSEQSET :
      sep = SeqMgrGetSeqEntryForData (ompcp->input_data);
      break;
      /*if clicked on alignment block*/
    case OBJ_SEQANNOT:
      sap=(SeqAnnotPtr) (ompcp->input_data);
      break;
      /*if clicked on individual alignment*/
    case OBJ_SEQALIGN:
      salp=(SeqAlignPtr) (ompcp->input_data);
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  
  ErrSetMessageLevel(SEV_ERROR);
  if(sap!=NULL)
  {
     salp=is_salp_in_sap(sap, 2);
     ValidateSeqAlignandACC (salp, 0, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, NULL);
  }
  if (salp!=NULL) {
     ValidateSeqAlignandACC (salp, 0, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, NULL);
  }
  if (sep!=NULL) {
     ValidateSeqAlignandACCInSeqEntry (sep, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE);
  }
  return OM_MSG_RET_DONE;
}

static void ValidateSeqAlignandACCCallback (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)
{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  SeqAlignPtr        salp;
  SaValPtr           svp;

  if (sep != NULL && sep->data.ptrvalue && mydata != NULL) {
     svp = (SaValPtr)mydata;
     if (IS_Bioseq(sep)) {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        if (bsp!=NULL) {
           salp=sap_empty(bsp->annot, 2, NULL);
           if (salp!=NULL) {
              ValidateSeqAlignandACC (salp, svp->entityID, svp->message, svp->msg_success, svp->find_remote_bsp, svp->find_acc_bsp, svp->delete_bsp, svp->delete_salp, &svp->dirty);
           }
        }
     }   
     else if(IS_Bioseq_set(sep)) {
        bssp = (BioseqSetPtr)sep->data.ptrvalue;
        if (bssp!=NULL) {
           salp=sap_empty(bssp->annot, 2, NULL);
           if (salp!=NULL) {
              ValidateSeqAlignandACC (salp, svp->entityID, svp->message, svp->msg_success, svp->find_remote_bsp, svp->find_acc_bsp, svp->delete_bsp, svp->delete_salp, &svp->dirty);
           }
        }
     }
  }
}



NLM_EXTERN Boolean ValidateSeqAlignandACCInSeqEntry (SeqEntryPtr sep, Boolean message, 
                                 Boolean msg_success, Boolean find_remote_bsp, Boolean find_acc_bsp,
                                 Boolean delete_bsp, Boolean delete_salp)
{
  SeqEntryPtr      sep_head;
  Uint2            entityID;
  SaVal            sv;
  Boolean          success=TRUE;

  entityID = ObjMgrGetEntityIDForChoice (sep);
  if (entityID > 0) {
     sep_head = GetTopSeqEntryForEntityID (entityID);
     if (sep_head != NULL) {
        sv.message = message;
        sv.msg_success = msg_success;
        sv.find_remote_bsp = find_remote_bsp;
        sv.find_acc_bsp = find_acc_bsp;
        sv.delete_salp = delete_salp;
        sv.delete_bsp = delete_bsp;
        sv.retdel = TRUE;
        sv.ids = NULL;
        sv.entityID = entityID; 
        sv.dirty = FALSE;
        SeqEntryExplore (sep_head, (Pointer)&sv, ValidateSeqAlignandACCCallback);
        if (sv.dirty) {
           ObjMgrSetDirtyFlag (entityID, TRUE);
           ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
        }
        success = sv.retdel;
     }
  }
  return success;
}
