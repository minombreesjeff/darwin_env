static char const rcsid[] = "$Id: simutil.c,v 6.8 2003/05/30 17:25:38 coulouri Exp $";

/*  simutil.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name:  simutil.c
*
* Author:  Jinghui Zhang
*
* Version Creation Date: 5/24/95
*
*
* File Description:  functions for using the sim/sim2 algorithm
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
*
* $Log: simutil.c,v $
* Revision 6.8  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.7  2003/05/13 16:02:54  coulouri
* make ErrPostEx(SEV_FATAL, ...) exit with nonzero status
*
* Revision 6.6  2000/04/28 19:10:47  kans
* check_strand_mol protected against bsp == NULL
*
* Revision 6.5  1999/12/17 20:47:06  egorov
* Fix 'gcc -Wall' warnings
*
* Revision 6.4  1999/03/12 18:38:02  kans
* fixed ErrPostEx problems
*
* Revision 6.3  1999/03/11 23:09:19  kans
* casts for sscanf, sprintf
*
* Revision 6.2  1998/07/21 23:11:52  zjing
* add Uint4 cast to SIZE_MAX
*
 * Revision 5.7  1997/04/11  18:41:46  zjing
 * .
 *
 * Revision 5.6  1997/04/11  18:11:34  zjing
 * fix a bug create_edit_script
 *
 * Revision 5.5  1997/03/13  21:01:28  zjing
 * get rid of new variable
 *
 * Revision 5.4  1996/12/23  18:01:01  zjing
 * .
 *
 * Revision 5.3  1996/06/19  14:45:59  zjing
 * move the function AddAlignInfoToSeqAnnot from pobutil to simutil
 *
 * Revision 4.7  1996/05/06  14:39:20  zjing
 * fix ALU filtering
 *
 * Revision 4.6  1996/04/18  22:33:44  zjing
 * change ck_name_same
 *
 * Revision 4.5  1996/02/21  22:08:40  zjing
 * protection for bizzare sequence
 *
 * Revision 4.4  1995/11/08  17:16:44  zjing
 * .
 *
 * Revision 4.3  1995/10/19  17:06:05  zjing
 * fix in make_sim_seq
 *
 * Revision 4.2  1995/09/28  01:58:49  zjing
 * fix in the function convert
 *
*
*
* ==========================================================================
*/


#ifndef _SIMUTIL_
#include <simutil.h>
#endif


static Int4 get_sim_num(FloatHi num)
{
	FloatHi temp;

	temp = (FloatHi)DIGIT * num;
	return (Int4)temp;
}

Boolean DefaultSimPam(SimPamPtr spp)
{
	if(spp == NULL)
		return FALSE;
	spp->cutoff = SIM2_cutoff;
	spp->mismatch = get_sim_num(SIM2_mismatch);
	spp->gap_open = get_sim_num(SIM2_gap_open);
	spp->gap_ext = get_sim_num(SIM2_gap_ext);
	spp->mismatch_2 = get_sim_num(SIM2_mismatch_2);
	spp->gap_open_2 = get_sim_num(SIM2_gap_open_2);
	spp->gap_ext_2 = get_sim_num(SIM2_gap_ext_2);
	spp->word = SIM2_word;
	return TRUE;
}
	
Boolean DefaultPSimPam(PSimPamPtr pspp)
{
	if(pspp == NULL)
		return FALSE;
	pspp->M_val = DEFAULT_M;
	pspp->I_val = DEFAULT_I;
	pspp->V_val = DEFAULT_V;
	pspp->O = DEFAULT_O;
	pspp->E = DEFAULT_E;
	pspp->pam_file = NULL;
	pspp->def_gap = TRUE;
	return TRUE;
}

/**********************************************************************
*
*	write_sim_output(align, out_name)
*	Create a Seq-annot for the list of align and save the Seq-annot
*	to the file out_name. 
*	FREE the Seq-align and returns a NULL pointer
*
**********************************************************************/
SeqAlignPtr write_sim_output(SeqAlignPtr align, CharPtr out_name)
{
	SeqAnnotPtr annot;
	AsnIoPtr aip;

	if(align)
	{
		annot = SeqAnnotNew();
		annot->type = 2;
		annot->data = align;
		aip = AsnIoOpen(out_name, "w");
		SeqAnnotAsnWrite(annot, aip, NULL);
		AsnIoClose(aip);
		SeqAnnotFree(annot);
		
	}
	
	return NULL;
}


/********************************************************************
*
*	check_strand_mol(loc, is_dna)
*	if the bioseq for loc is DNA, *is_dna = TRUE
*	return TRUE for computing both strand, FALSE for one strand
*
********************************************************************/
Boolean check_strand_mol (SeqLocPtr loc, BoolPtr is_dna)
{
	BioseqPtr bsp;
	
	bsp = BioseqFindCore(SeqLocId(loc));
	*is_dna = (bsp == NULL || bsp->mol != Seq_mol_aa);
	if(SeqLocStrand (loc) == Seq_strand_both)
	{
		if(*is_dna)
			return TRUE;
		else
			Change_Loc_Strand(loc, Seq_strand_plus);
	}

	return FALSE;
}


/******************************************************************
****
*       Change_Loc_Strand(loc, strand) 
*	Change the loc strand to strnad. 
*	only works for SEQLOC_INT. 
*	return FALSE for fail
*
*******************************************************************
***/
Boolean Change_Loc_Strand(SeqLocPtr loc, Uint1 strand)
{

  SeqIntPtr sip;

	if(loc->choice != SEQLOC_INT)
		return FALSE;
        sip = loc->data.ptrvalue;
        sip->strand = strand;
        loc->data.ptrvalue = sip;
	return TRUE;
}



/***********************************************************************
*
*	make_sim_seq(slp, t_seq)
*	return the instatiated sequence made from slp for sim2/sim search. 
*	sequence starts from 1 of the array)
*	if(t_seq !=NULL), load data to t_seq, return t_seq
*	else create a new array.
*	t_seq is used for the saving of time in computing both strand
*
***********************************************************************/
CharPtr make_sim_seq(SeqLocPtr slp, Boolean is_sim2, CharPtr t_seq)
{
        CharPtr seq;
        Int4 length;
        SeqPortPtr spp;
        Uint1 residue;
        Int4 i;
        BioseqPtr bsp; 
        Uint1 code; 
	SeqIntPtr sint;
	Char buf[21];
 
                 
                if(slp->choice != SEQLOC_WHOLE && slp->choice != SEQLOC_INT)
                        return NULL;
                bsp = BioseqFindCore(SeqLocId(slp));
		if(bsp == NULL)
			return NULL;
		if(bsp->mol == Seq_mol_aa)
			code = Seq_code_ncbieaa;
		else
			code = Seq_code_iupacna;
                length = SeqLocLen(slp);
		if(t_seq)
			seq = t_seq;
		else
                	seq = MemNew((size_t)(length+2) * sizeof(Char));
                spp = SeqPortNewByLoc(slp, code);
		/* spp->do_virtual = FALSE; */

		i =1;
                while((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF)
		{
		   if(residue != SEQPORT_EOS && residue != SEQPORT_VIRT)
		   {
			if(IS_ALPHA(residue))
			{
                        	seq[i] = (Char)residue;
                        	if(bsp->mol != Seq_mol_aa && is_sim2) 
                        	{
                                	if (StringChr("ACGT", seq[i]) == NULL)
                                        	seq[i] = 'A';
                        	} 
                        	++i;
			}
			else
			{
				/*printf("residue is %d\n", residue);*/
				seq[i++] = 'A';
			}
		   }
                }
                seq[i] = '\0';
                SeqPortFree(spp);
		if( i != length+1)
		{
			SeqIdWrite(SeqLocId(slp), buf, PRINTID_FASTA_LONG, 20);
			ErrPostEx(SEV_WARNING, 0, 0, "the expected "
			"length %ld is not the same as the real length %ld for %s", (long) length, (long) (i+1), buf);
			if(i < length +1 && slp->choice == SEQLOC_INT)
			{
				sint = slp->data.ptrvalue;
				if(sint->strand == Seq_strand_minus)
					sint->from += (length + 1 - i);
				else
					sint->to -= (length + 1 -i);
			}
			/* if(t_seq == NULL)
				MemFree(seq);
			return NULL; */
		}
                return seq;
 
}



/*************************************************************************
***
*	convert(): convert the x, y into the value for dsp->starts in Dense
*	seg. 
*	if strand == Seq_strand_minus, take the end point of the second 
*	strand as the start instead of the first strand
*	return Score Pointer
**************************************************************************
***/
#define gap(k) ((k) <= 0 ? 0 : (g+hh*(k)))       /* k-symbol indel cost */
static Int4 g, hh;


static ScorePtr load_score(FloatHi score, ScorePtr PNTR scp_head)
{
	ScorePtr scp, curr;
	
  	scp = ScoreNew();
   	scp->choice = 2;                /**it is a real number**/
   	scp->value.realvalue = score;
   	scp->next = NULL;

	if(*scp_head == NULL)
		*scp_head = scp;
	else
	{
		curr = *scp_head;
		while(curr->next != NULL)
			curr = curr->next;
		curr->next = scp;
	}

	return (*scp_head);
}

static ScorePtr convert(Int4Ptr x, Int4Ptr y, Int4Ptr start, Int4Ptr length, Int4 n, SeqLocPtr locx, SeqLocPtr locy, Int2Ptr numseg, FloatHiPtr SegScores)
{
  Int2 i, j, k;
  Int4 len_x, len_y, len_offset;
  ScorePtr scp_head = NULL;
  Uint1 strand;
  Int4 temp;
  Int4 x_start, x_stop, y_start, y_stop;

   strand = SeqLocStrand(locy);
   for(i=0; i<n; ++i){
     x[i] = x[i] -1 + SeqLocStart(locx) ;
     if(strand == Seq_strand_minus)
	y[i] = SeqLocStop(locy) -  (y[i] -1);
     else
        y[i] = y[i] -1 + SeqLocStart(locy) ;
   }

   if(strand == Seq_strand_minus)
   {
	for( i=0; i<n/2; ++i)
	{
		temp = y[2*i];
		y[2*i]= y[2*i+1];
		y[2*i+1] = temp;
	}
   }

   for( i =0, j=0; i<n/2; ++i)
   {
	x_start = x[2*i];
	x_stop = x[2*i+1];
	y_start = y[2*i];
	y_stop = y[2*i+1];

	start[2*j] = x_start;
	start[2*j+1] = y_start;
	length[j] = x_stop - x_start +1;
	if(SegScores != NULL)
		load_score(SegScores[i], &scp_head);
	++j;

	k = i+1;	/*the next segment*/
	if(k < n/2)	/*not the last segment*/
	{
		len_x = x[2*k] - (x_stop +1);
		if(strand == Seq_strand_minus)
			len_y = (y_start -1) - y[2*k+1];
		else
			len_y = y[2*k] - (y_stop +1);
		len_offset = MIN(len_x, len_y);
		if(len_offset > 0)
		{
			x[2*k] -= len_offset;
			if(strand == Seq_strand_minus)
				y[2*k +1] += len_offset;
			else
				y[2*k] -= len_offset;
		}
		if(len_x > len_y)
		{
			start[2*j] = x_stop +1;
			start[2*j+1] = -1;
			length[j] = len_x - len_y;
			++j;
			if(SegScores != NULL)
				load_score(-(FloatHi) gap(len_x - len_y)/DIGIT, &scp_head);
		}
		if(len_y > len_x)
		{
			start[2*j] = -1;
			if(strand == Seq_strand_minus)
				start[2*j+1] = y[2*k+1]+1;
			else
				start[2*j+1] = y_stop +1;	
			length[j] = len_y - len_x;
			++j;
			if(SegScores != NULL)
				load_score(-(FloatHi) gap(len_y - len_x)/DIGIT, &scp_head);
		}
	}
    }
	*numseg = j;
	return scp_head;
}
			

static void create_new_script PROTO((Int4 len, Uint1 type, EditScriptPtr PNTR head, EditScriptPtr PNTR prev));
static EditScriptPtr convert_array_to_EditScript(Int4Ptr x, Int4Ptr y, Int2 n)
{
	EditScriptPtr prev = NULL, head = NULL;
	Int2 i;
	Int4 gap_size, num;
	Uint1 gap_type;

	gap_size = 0;
	num = 0;
	for(i = 0; i<n/2; ++i)
	{
		gap_size = 0;
		if(i > 0)
		{
			if(x[2*i] > x[2*i-1] +1)
			{
				gap_size = x[2*i] - (x[2*i-1] +1);
				gap_type = DEL;
			}
			else if(y[2*i] > y[2*i-1] +1)
			{
				gap_size = y[2*i] - (y[2*i-1] +1);
				gap_type = INS;
			}
		}

		if(gap_size > 0)
		{
			if(num > 0)
			{
				create_new_script(num, SUB, &head, &prev);
				num = 0;
			}
			create_new_script(gap_size, gap_type, &head, &prev);
		}

		num += x[2*i +1] - x[2*i] +1;
		if(i == n/2-1)
			create_new_script(num, SUB, &head, &prev);

	}
	return head;
	
			
}

/***********************************************************************
*
*	make_align(x, y, n, score, SegScores, loc1, loc2)
*	x, y is the two arrays storing the start, stop of the aligned seg
*	for the two sequences
*	score: the score of the alignment
*	SegScores: the scores for each aligned seg
*	loc1: the Seq-loc of the first sequence
*	loc2: the Seq-loc of the second sequence
*
*	return Seq-align made from the above data
*
************************************************************************/
SeqAlignPtr make_align(Int4Ptr x, Int4Ptr y, Int2 n, FloatHi score, FloatHiPtr SegScores, SeqLocPtr loc1, SeqLocPtr loc2)
{
  ScorePtr scp;
  SeqAlignPtr sap;
  Uint1 strand;
  EditScriptPtr esp;

	strand = SeqLocStrand(loc2);
	esp = convert_array_to_EditScript(x, y, n);
	sap = ConvertEditScript(esp, loc1, loc2, x[0]-1, y[0]-1, FALSE);
	Script_free(esp);

	scp = ScoreNew();
	scp->choice = 2;		/**it is a real number**/
	scp->value.realvalue = score;
	scp->next = NULL;
	sap->score = scp;
	sap->next = NULL;
	return sap;
}
		
		
/**********************************************************************
*
*	link_align(new, head)
*	link the new align to the end of head align. return the 
*	start of the linked chain
*
**********************************************************************/
SeqAlignPtr link_align(SeqAlignPtr a_new, SeqAlignPtr head)
{
	SeqAlignPtr curr;

	curr = head;
        if(curr!= NULL){
                while(curr->next != NULL)
                        curr= curr->next;
                curr->next = a_new;
		return head;
        }
	else
        	return a_new;

}


/***********************************************************************
*
*	free_align_list(head)
*	Free all the Seq-align linked to head
*	return NULL
*
**********************************************************************/
SeqAlignPtr free_align_list(SeqAlignPtr head)
{
	SeqAlignPtr next;

	while(head)
	{
		next = head->next;
		head->next = NULL;
		SeqAlignFree(head);
		head = next;
	}
	return NULL;
}


/*****************************************************************
*
*	get_align_score(sap, score)
*	get the score from the alignment. return FALSE if there is 
*	no score.
*	sap: Seq-align
*	score: for laoding the score
*
*****************************************************************/
Boolean get_align_score(SeqAlignPtr sap, FloatHiPtr score)
{

        ScorePtr scp;

        if(sap->score == NULL)
                return FALSE;
        scp = sap->score;
        *score = scp->value.realvalue;
        return TRUE;
}


/*******************************************************************
*
*	sort_align_list(align)
*	sort the list of Seq-align to the descending order of alignment
*	score
*
*******************************************************************/
SeqAlignPtr sort_align_list(SeqAlignPtr align)
{
	SeqAlignPtr cp1, cp2;
	FloatHi score1, score2;
  	Uint1 ttype, tsegtype;
  	Int2 tdim;
  	ScorePtr tscore;
  	Pointer tsegs;


        for (cp1 = align; cp1; cp1 = cp1->next) {
           for (cp2 = cp1->next; cp2; cp2 = cp2->next) {
                get_align_score(cp1, &score1);
                get_align_score(cp2, &score2);
                if (score2 > score1) { /* swap */
                   /* save cp1 */
                   ttype = cp1->type;
                   tsegtype = cp1->segtype;
                   tdim = cp1->dim;
                   tscore = cp1->score;
                   tsegs = cp1->segs;
 
                   /* cp1 <- cp2 */
                   cp1->type = cp2->type;
                   cp1->segtype = cp2->segtype;
                   cp1->dim = cp2->dim;
                   cp1->score = cp2->score;
                   cp1->segs = cp2->segs;
 
                   /* restore cp1 to cp2 */
                   cp2->type = ttype;
                   cp2->segtype = tsegtype;
                   cp2->dim = tdim;
                   cp2->score = tscore;
                   cp2->segs = tsegs;
                }
           }
        }

	return align;
}


/******************************************************************
*
*	get_top_K_alignment(align, K, cut_off)
*	get the top K SCORED alignment and all the scores should be
*	above the cutoff
*	align	Seq-align
*	K	the number of alignment to keep
*	cutoff	the cut off score
*
*******************************************************************/
SeqAlignPtr get_top_K_alignment(SeqAlignPtr align, Int4 K, FloatHi cut_off)
{
	Boolean check_score;
	Int2 i;
	SeqAlignPtr curr, prev;
	FloatHi score;

	check_score = (cut_off > 0.0);

        align = sort_align_list(align);
        /* save the k best alignments and free the others */
        i = 0;
	curr = align;
	prev = NULL;
        while (i<K && curr) {
	   if(check_score)
	   {
           	get_align_score(curr, &score);
           	if (score < cut_off) break;
	   }
           ++i;
	   prev = curr;
           curr= curr->next;
        }
 
        if (curr) { /* free some alignments */
	   if(prev != NULL)
		prev->next = NULL;
	   else
		align = NULL;
	   free_align_list(curr);
	}
	
	return align;
}


/* fatal - print message and die */
void fatal(CharPtr msg)
{
        Message(MSG_FATAL, "%s\n", msg);
}

/* fatalf - format message, print it, and die */
void fatalf(CharPtr msg, CharPtr val)
{
        Message(MSG_FATAL, msg, val);
}

/* ckopen - open file; check for success */
FILE *ckopen(CharPtr name, CharPtr mode)
{
        FILE *fp;
 
        if ((fp = FileOpen(name, mode)) == NULL) {
                Message(MSG_ERROR,"Can not open %Fs", name);
                fatalf("Cannot open %s.", name);
        }
        return(fp);
}        
 
/* ckalloc - allocate space; check for success */
CharPtr ckalloc(Int4 amount)
{
        CharPtr p;

                   
        if ((Uint4)amount > (Uint4)SIZE_MAX)
        {
                ErrPostEx(SEV_FATAL, 1,0,"Sorry. Can't allocate that much (%ld), %ld", (long) amount, (long) SIZE_MAX);
		exit(1);
        }
 
        if ((p = MemNew( (size_t) amount)) == NULL)
                fatal("Ran out of memory.");
        return(p);
}
 
/* strsame - tell whether two strings are identical */
Int4 strsame(CharPtr s, CharPtr t)
{
        return(strcmp(s, t) == 0);
}


	
				
/********************************************************************
*
*	LoadNewEnds()
*	load the related RecordEnds info to the structrue and link it 
*	to the header of the list. (This is only for convienience)
*
*********************************************************************/
ValNodePtr LoadNewEnds(Int4 start, Int4 stop, Uint1 strand, FloatHi score, FloatHi zscore, CharPtr seq_name, Boolean sort_zscore, ValNodePtr PNTR ends_list)
{
	ValNodePtr c_new, prev, curr, next;
	RecordEndsPtr rep, newrep;
	FloatHi tscore, cscore;


	newrep = MemNew(sizeof(RecordEnds));
	newrep->start = start;
	newrep->stop = stop;
	newrep->strand = strand;
	newrep->score = score;
	newrep->zscore = zscore;
	newrep->gscore = 0.0;
	newrep->count = 1;
	StringCpy(newrep->name, seq_name);
	c_new = ValNodeNew(NULL);
	c_new->data.ptrvalue = newrep;

	if(*ends_list == NULL)
	{
		*ends_list = c_new;
		return c_new;
	}

	prev = NULL;
	tscore = (sort_zscore ? (newrep->zscore) : score);
	for(curr = *ends_list; curr != NULL; curr = curr->next)
	{
		rep = (RecordEndsPtr)(curr->data.ptrvalue);
		cscore = (sort_zscore ? (rep->zscore) : (rep->score));
		if(tscore > cscore)
		{
			if(prev == NULL)
			{
				next = curr;
				*ends_list = c_new;
			}
			else
			{
				next = prev->next;
				prev->next = c_new;
			}
			c_new->next = next;
			return (*ends_list);
		}
		prev = curr;
	}
	prev->next = c_new;
	return (*ends_list);
}


/*******************************************************************
*
*	getp_top_K(head, K)
*	get only the top K of the list, free the others. 
*	The top K is simplily determined by the order in the list
*
********************************************************************/
ValNodePtr get_top_K(ValNodePtr head, Int4 K)
{
	Int4 i;
	ValNodePtr curr, list;

	for(i =0, list=head; i<K && list!=NULL; list = list->next)
	{
		++i;
		if(K ==i)
		{
			curr = list->next;
			list->next = NULL;
			ValNodeFreeData(curr);
			return head;
		}
	}
	return head;
}

static FloatHi re_cal_score(RecordEndsPtr o_rep, RecordEndsPtr n_rep)
{
	Int4 o_len, n_len;
	FloatHi temp;

	o_len = o_rep->stop - o_rep->start +1;
	n_len = n_rep->stop - n_rep->start +1;

	temp = (FloatHi)(o_len) * o_rep->score + (FloatHi)(n_len) * n_rep->score;
	return temp/(FloatHi)(o_len+n_len);
}

static Boolean merge_list(Int4 o_start, Int4 o_stop, Int4 n_start, Int4 n_stop)
{
	/*Int4 min_start, min_stop, min_len;*/

	if(o_stop < n_start || o_start > n_stop)
		return FALSE;
	else
		return TRUE;
	/*if(!(o_stop < n_start || o_start > n_stop))
	{
		min_start = MAX(o_start, n_start);
		min_stop = MIN(o_stop, n_stop);
		min_len = min_stop - min_start;
		if((FloatHi)(min_len)/(FloatHi)(o_stop-o_start) > 0.6)
			return TRUE;
		if((FloatHi)(min_len)/(FloatHi)(n_stop-n_start) > 0.6)
			return TRUE;
	}
	return FALSE;*/
}
		

static Boolean is_alu_seq(CharPtr name)
{
	if(!StrStr(name, "MER"))
		if(!StrStr(name, "L1"))
			return TRUE;
	return FALSE;
}
static Boolean ck_name_same(CharPtr name1, CharPtr name2)
{
	/* if(StringCmp(name1, name2) == 0)
		return TRUE;
	
	if(is_alu_seq(name1) && is_alu_seq(name2))
	{
		StringCpy(name1, "Alu");
		return TRUE;
	}
	else
		return FALSE; */
	return (StringCmp(name1, name2) == 0);
}


/********************************************************************
*
*	CleanNewList(newlist)
*	merge any overlappign RecordEnds in the current list
*
********************************************************************/
ValNodePtr CleanNewList(ValNodePtr newlist)
{
	ValNodePtr c_new, prev;
	RecordEndsPtr o_rep, n_rep;
	Int4 n_start, n_stop, o_start, o_stop;
	Uint1 n_strand, o_strand;
	Boolean found;

	newlist = SortEndsList(newlist);
	prev = NULL;
	for(c_new = newlist; c_new != NULL; )
	{
		if(prev == NULL)
		{
			prev = c_new;
			c_new = c_new->next;
		}
		else
		{
			n_rep = (RecordEndsPtr)(c_new->data.ptrvalue);	
			n_start = n_rep->start;
			n_stop = n_rep->stop;
			n_strand = n_rep->strand;

			o_rep = (RecordEndsPtr)(prev->data.ptrvalue);	
			o_start = o_rep->start;
			o_stop = o_rep->stop;
			o_strand = o_rep->strand;
			found = FALSE;
			if((n_strand == o_strand) && ck_name_same(o_rep->name, n_rep->name))
			{
			   if(merge_list(o_start, o_stop, n_start, n_stop))
			   {
				prev->next = c_new->next;
				c_new->next = NULL;
				o_rep->start = MIN(o_start, n_start);
				o_rep->stop = MAX(o_stop, n_stop);
				o_rep->zscore = MAX(o_rep->zscore, n_rep->zscore);
				o_rep->score = MAX(o_rep->score, n_rep->score);
				ValNodeFreeData(c_new);
				c_new = prev->next;
				found = TRUE;
			    }
			}
			if(!found)
			{
				prev = c_new;
				c_new = c_new->next;
			}
		}
	}

	return newlist;
}


/******************************************************************
*
*	merge_two_list(oldlist, newlist)
*	merge any overlapping piece of RecordEnds in the two list
*	link the new ends to the oldlist
*	resort the oldlist with the region
*	return the head of the oldlist
*
*******************************************************************/	
ValNodePtr merge_two_list(ValNodePtr oldlist, ValNodePtr newlist)
{
	ValNodePtr c_old, c_new, prev, next;
	RecordEndsPtr o_rep, n_rep;
	Int4 n_start, n_stop, o_start, o_stop;
	Uint1 n_strand, o_strand;
	Boolean found;

	newlist = CleanNewList(newlist);
				
	if(oldlist == NULL)
		return newlist;

	
	prev = NULL;
	for(c_new = newlist; c_new != NULL; )
	{
		n_rep = (RecordEndsPtr)(c_new->data.ptrvalue);	
		n_start = n_rep->start;
		n_stop = n_rep->stop;
		n_strand = n_rep->strand;
		found = FALSE;
		for(c_old = oldlist; c_old!=NULL && !found; c_old=c_old->next)
		{
			o_rep = (RecordEndsPtr)(c_old->data.ptrvalue);	
			o_start = o_rep->start;
			o_stop = o_rep->stop;
			o_strand = o_rep->strand;
			if((n_strand == o_strand) && ck_name_same(o_rep->name, n_rep->name))
			{
			   if(merge_list(o_start, o_stop, n_start, n_stop))
			   {
				found = TRUE;
				o_rep->start = MIN(o_start, n_start);
				o_rep->stop = MAX(o_stop, n_stop);
				/*o_rep->score = re_cal_score(o_rep, n_rep);*/
				o_rep->zscore = MAX(o_rep->zscore, n_rep->zscore);
				o_rep->score = MAX(o_rep->score, n_rep->score);
				++(o_rep->count);
				next = c_new->next;
				if(prev == NULL)
					newlist = next;
				else
					prev->next = next;
				c_new->next=NULL;
				ValNodeFreeData(c_new);
				c_new = next;
			   }
			}
		}
		if(!found)
		{
			prev = c_new;
			c_new = c_new->next;
		}
	}

	if(oldlist == NULL)
		oldlist = newlist;
	else
	{
		c_old = oldlist;
		while(c_old->next != NULL)
			c_old = c_old->next;
		c_old->next = newlist;
	}
	/*ValNodeLink(&oldlist, newlist);*/
	return SortEndsList(oldlist);
}
	

		

			
	
static int LIBCALLBACK ValNodeCompProc (VoidPtr ptr1, VoidPtr ptr2)
{
  ValNodePtr  vnp1, vnp2;
  RecordEndsPtr rep1, rep2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
	rep1 = vnp1->data.ptrvalue;
	rep2 = vnp2->data.ptrvalue;
      if (rep1 != NULL && rep2 != NULL) {
        if (rep1->start > rep2->start) {
          return 1;
        } else if (rep1->start < rep2->start) {
          return -1;
        } else if (rep1->stop > rep2->stop) {
          return 1;
        } else if (rep1->stop < rep2->stop) {
          return -1;
        } else {
          return 0;
        }
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}


/**********************************************************************
*
*	SortEndsList(vnp)
*	sort the list of RecordEnds to the ascending order of the 
*	LOCATION
*
**********************************************************************/
ValNodePtr SortEndsList(ValNodePtr vnp)
{
	ValNodePtr tmp, PNTR head;
	Int4 num, i;

	for(tmp =vnp, num=0; tmp!=NULL; tmp=tmp->next)
		++num;
		 
	if(num >1 && vnp)
	{
		head = MemNew((size_t)(num+1) * sizeof (ValNodePtr));
		i=0;
		tmp = vnp;
		while(tmp != NULL && i< num)
		{
			head[i] = tmp;
			tmp = tmp->next;
			++i;
		}

		HeapSort(head, (size_t)num, sizeof(ValNodePtr), ValNodeCompProc);
		for(i =0; i<num; ++i)
		{
			tmp = head[i];
			tmp->next = head[i+1];
		}
		vnp = head[0];
		MemFree(head);
	}
	return vnp;

}


/***********************************************************************
*
*	parse_buf_val(buf, seq_name, start, stop, strand)
*	parse the information stored in the text (buf) into the data 
*	that can be used to make a Seq-loc
*	
***********************************************************************/
static Boolean parse_buf_val(CharPtr buf, CharPtr seq_name, Int4Ptr start, Int4Ptr stop, Uint1Ptr strand)
{
	CharPtr curr;
	Int2 i=0;
	long t_start =-1, t_stop =-1;

	*start = 0;
	*stop = -1;
	*strand = Seq_strand_both;
	if(!StrStr(buf, "|"))
		StringCpy(seq_name, buf);
	else
	{
		i =0;
		curr = StrTok(buf, "|");
		while(curr != NULL)
		{
			switch (i)
			{
				case 0:
					StringCpy(seq_name, curr);
					break;
				case 1:
					sscanf(curr, "%ld", &t_start);
					break;
				case 2:
					sscanf(curr, "%ld", &t_stop);
					break;
				case 3:
					if(curr !=NULL)
					{
						if(*curr == '+')
							*strand = Seq_strand_plus;
						if(*curr == '-')
							*strand = Seq_strand_minus;
					}
					break;
				default:
					break;
			}
			curr = StrTok(NULL, "|");
			++i;
		}
	}
	if(t_start > 0)
		--t_start;
	if(t_stop > 0)
		--t_stop;
	if(t_start >=0 && t_stop >=0)
	{
		*start = (Int4) MIN(t_start, t_stop);
		*stop = (Int4) MAX(t_start, t_stop);
	}


	return TRUE;

}


static Boolean get_local_seq_name(CharPtr seq_name, CharPtr name)
{
	Int4 len, i;
	CharPtr str;

	len = StringLen(seq_name);
	str = seq_name + len -1;
	i = 0;
	while(*str != '.')
	{
		++i;
		if(i == len)
			return FALSE;
		--str;
	}

	StringNCpy(name, seq_name, (len-i-1));
	name[len-i-1] = '\0';

	return TRUE;
}

/******************************************************************
*
*	prepare_align_data(buf, bsp)
*
*	convert data in buf in the format used by SIM ans SIM2 to a 
*	Seq-loc in a Bioseq
*	buf: a special formatted buffer which can be converted into 
*	a Seq-loc
*	bsp: store the Bioseq data
*	return the Seq-loc made from the buf. NULL if the format is 
*	not correct or the Bioseq is NULL
*
******************************************************************/
SeqLocPtr prepare_align_data(CharPtr buf, BioseqPtr PNTR bsp)
{
	Char seq_name[100], name[100];
	Int4 gi;
	BioseqPtr cbsp;
	SeqIdPtr sip;
	Uint1 k;

	ObjectIdPtr obj_id;
	TextSeqIdPtr tsip;
	Int4 start, stop;
	Int4 temp;
	Uint1 strand;


	parse_buf_val(buf, seq_name, &start, &stop, &strand);
	if(!StrStr(seq_name, "."))	/*no extension, assume GenBank entry*/
	{
		gi = atol(seq_name);
		if(gi == 0)	/*not a gi, assumes it is an accession*/
		{
			sip = ValNodeNew(NULL);
			tsip = TextSeqIdNew();
			for(k = SEQID_GENBANK; k<= SEQID_SWISSPROT; ++k)
			{
				sip->choice = k;
				sip->data.ptrvalue = tsip;
				if(tsip->accession != NULL)
					tsip->accession = MemFree(tsip->accession);
				if(tsip->name != NULL)
					tsip->name = MemFree(tsip->name);
				tsip->accession = StringSave(seq_name);
				cbsp = BioseqLockById(sip);
				if(cbsp == NULL)
				{
					MemFree(tsip->accession);
					tsip->accession = NULL;
					tsip->name = StringSave(seq_name);
					cbsp = BioseqLockById(sip);
				}
				if(cbsp != NULL)
					break;
			}
		}
		else
		{
			sip = ValNodeNew(NULL);
			sip->choice = SEQID_GI;
			sip->data.intvalue = gi;
			cbsp = BioseqLockById(sip);
		}
	}
	else	/*treat it as a local file*/
	{
		get_local_seq_name(seq_name, name);
		sip = ValNodeNew(NULL);
		sip->choice = SEQID_LOCAL;
		obj_id = ObjectIdNew();
		obj_id->str = StringSave(name);
		sip->data.ptrvalue = obj_id;
		cbsp = BioseqLockById(sip);
	}

	SeqIdFree(sip);
	if(cbsp == NULL)
		return NULL;

	*bsp = cbsp;
	if(stop == -1)
		stop = cbsp->length -1;
	if(stop < start)
	{
		temp = start;
		start = stop;
		stop = temp;
	}
#ifdef WIN16	/*check the interval with MS_WIN*/
		if((stop - start) > MAX_WINLEN)
		{
			Message(MSG_OK, "The alignment length is chopped to %ld", MAX_WINLEN);
			stop = start + MAX_WINLEN-1;
		}
#endif

	sip = SeqIdFindBest(cbsp->id, SEQID_GI);
	if(sip == NULL || sip->choice != SEQID_GI)
		sip = SeqIdFindBest(cbsp->id, SEQID_LOCAL);
	if(sip == NULL)
		sip = cbsp->id;
	return SeqLocIntNew(start, stop, strand, sip);
}



static void create_new_script(Int4 len, Uint1 type, EditScriptPtr PNTR head, 
	EditScriptPtr PNTR prev)
{
	EditScriptPtr curr;

	curr = MemNew(sizeof(EditScript));
	curr->op_type = type;
	curr->num = ABS(len);

	if(*head == NULL)
		*head = curr;
	else
		(*prev)->next = curr;
	*prev = curr;
}

EditScriptPtr CreateEditScript (Int4Ptr S, Int4 M, Int4 N)
{
	Int4 i, j;
	Int4 num;
	EditScriptPtr prev, head;

	head = NULL;
	prev = NULL;
	i = j = 0;
	num = 0;
	while(i < M || j < N)
	{
		if(*S == 0)
		{
			++i;
			++j;
			++num;
		}
		else
		{
			if(num > 0)
			{
				create_new_script(num, SUB, &head, &prev); 
				num = 0;
			}
			if(*S > 0)
			{
				create_new_script(*S, INS, &head, &prev);
				j += *S;
			}
			else
			{
				create_new_script(*S, DEL, &head, &prev);
				i+= ABS(*S);
			}
		}

		++S;
	}

	return head;
}


EditScriptPtr Script_free(EditScriptPtr head)
{
	EditScriptPtr tp, tp1;

	tp = head;
	while (tp != NULL) {
	   tp1 = tp->next;
	   MemFree(tp);
	   tp = tp1;
	}
	return NULL;
}


static Int4 get_start_pos(SeqLocPtr slp)
{
	if(SeqLocStrand(slp) == Seq_strand_minus)
		return (-SeqLocStop(slp));
	else
		return SeqLocStart(slp);
}

static Int4 get_current_pos(Int4Ptr pos, Int4 length)
{
	Int4 val;
	if(*pos < 0)
		val = -(*pos + length -1);
	else
		val = *pos;
	*pos += length;
	return val;
}

static Boolean is_end_seg(EditScriptPtr curr, EditScriptPtr head)
{
	EditScriptPtr next, prev;
	Boolean prev_sub_seg = FALSE;

	if(curr == head || curr->next == NULL)
		return TRUE;
	for(prev = head; prev != curr; prev = prev->next)
	{
		if(prev->op_type == SUB)
		{
			prev_sub_seg = TRUE;
			break;
		}
	}
	if(prev_sub_seg == FALSE)
		return TRUE;

	if(curr != NULL && curr->op_type != SUB)
	{
		next = curr->next;
		while(next)
		{
			if(next->op_type == SUB)
				return FALSE;
			next = next->next;
		}
	}
	return TRUE;
}

		

SeqAlignPtr ConvertEditScript(EditScriptPtr esp, SeqLocPtr slp1, SeqLocPtr slp2, Int4 offset_1, Int4 offset_2, Boolean show_end_gap)
{
	/*ScorePtr scp;	 */
	Int2 numseg = 0, i;
	EditScriptPtr curr;

	Int4Ptr starts, length;
	Int4 start_1, start_2;
	DenseSegPtr dsp;
	SeqAlignPtr sap;

	Uint1Ptr a_strands;
	Boolean is_end;

	if(esp == NULL || slp1 == NULL || slp2 == NULL)
		return NULL;

	for(curr = esp; curr != NULL; curr = curr->next)
	{
		if(show_end_gap)
			++numseg;
		else
		{
			if(!is_end_seg(curr, esp) || curr->op_type == SUB)
				++numseg;
		}
	}
	if(numseg == 0)
	{
		Message(MSG_POSTERR, "Only contains gaps");
		return NULL;
	}
		

	starts = MemNew((size_t)(2*numseg) * sizeof(Int4));
	a_strands = MemNew((size_t)(2*numseg) * sizeof(Uint1));
	length = MemNew((size_t)numseg * sizeof(Int4));

	start_1 = get_start_pos(slp1) + offset_1;
	start_2 = get_start_pos(slp2) + offset_2;
	i = 0;
	for(curr = esp; curr != NULL; curr = curr->next)
	{
		if(show_end_gap)
			is_end = FALSE;
		else
			is_end = is_end_seg(curr, esp);
		switch(curr->op_type)
		{
			case SUB:
				starts[2*i] = get_current_pos(&start_1, curr->num);
				starts[2*i+1] = get_current_pos(&start_2, curr->num);
				break;

			case DEL:
				if(is_end)
					get_current_pos(&start_1, curr->num);
				else
				{
					starts[2*i] = get_current_pos(&start_1, curr->num);
					starts[2*i+1] = -1;
				}
				break;

			case INS:
				if(is_end)
					get_current_pos(&start_2, curr->num);
				else
				{
					starts[2*i] = -1;
					starts[2*i+1] = get_current_pos(&start_2, curr->num);
				}
				break;
			default:
				break;
		}

		if(!is_end || curr->op_type == SUB)
		{
			length[i] = curr->num;
			a_strands[2*i] = SeqLocStrand(slp1);
			a_strands[2*i+1] = SeqLocStrand(slp2);
			++i;
		}
	}


	sap = SeqAlignNew();

	sap->type = 3; /**the type in this SeqAlignObject are partial**/
	sap->segtype =2; /**use denseg to store the alignment**/
	sap->dim =2; /**only two dimention alignment**/

	/*scp = ScoreNew();
	scp->choice = 2;		
	scp->value.realvalue = score;
	scp->next = NULL;
	sap->score = scp;*/
  
	/**make the Denseg Object for SeqAlign**/
	dsp = DenseSegNew();
	dsp->dim = 2;
	dsp->numseg = numseg;
	dsp->ids = SeqIdDup(SeqLocId(slp1));
	dsp->ids->next = SeqIdDup(SeqLocId(slp2));
	dsp->starts = starts;
	dsp->strands = a_strands;
	dsp->lens = length;
	sap->segs = dsp;
	sap->next = NULL;

	return sap;
}

