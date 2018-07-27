#include <pobutil.h>
#include <gather.h>
#include <edutil.h>
#include <salign.h>
#include <salutil.h>
#include <salsap.h>

/*****************************************************************
*
*	clean_empty_seqalign(): clean up Seq-align that has no 
*	data
*
*****************************************************************/
SeqAlignPtr clean_empty_seqalign(SeqAlignPtr PNTR head)
{
	SeqAlignPtr curr, next, prev = NULL;

	curr = *head;
	while(curr)
	{
		next = curr->next;
		if(curr->segs == NULL)
		{
			if(prev == NULL)
				*head = curr->next;
			else
				prev->next = curr->next;
			curr->next = NULL;
			SeqAlignFree(curr);
		}
		else
			prev = curr;
		curr = next;
	}

	return (*head);
}


ValNodePtr make_repeat_lib(CharPtr file_name, 
                           ValNodePtr PNTR slp_list, 
                           Boolean io_type)
{
  FILE *ifp;
  SeqEntryPtr sep;
  BioseqPtr bsp;
  ValNodePtr sep_list = NULL;
  SeqLocPtr slp;
  CharPtr next_char;
  
  if(file_name == NULL)
    return NULL;
  *slp_list = NULL;
  
  /* FILE IO */
  
  if(io_type == FILE_IO) {
    if((ifp = FileOpen(file_name, "r")) == NULL) {
      Message(MSG_POSTERR, "Fail to open Alu data file %s", file_name);
      return NULL;
    }
    while((sep = FastaToSeqEntry(ifp, TRUE)) != NULL) {
      if(sep !=NULL) {
        ValNodeAddPointer(&sep_list, 0, sep);
        bsp = sep->data.ptrvalue;
        slp = SeqLocIntNew(0, bsp->length-1, Seq_strand_both, bsp->id);
        ValNodeAddPointer(slp_list, 0, slp);
      }
    }
    FileClose(ifp);
  } else {
    /* STDIN IO - WWW Power Blast case */
    if(file_name[0] == NULLB)
      return NULL;
    next_char = file_name;
    while((sep = FastaToSeqBuff(next_char, 
                                &next_char, TRUE)) != NULL) {
      if(sep !=NULL) {
        ValNodeAddPointer(&sep_list, 0, sep);
        bsp = sep->data.ptrvalue;
        slp = SeqLocIntNew(0, bsp->length-1, Seq_strand_both, bsp->id);
        ValNodeAddPointer(slp_list, 0, slp);
      }
    }
  }
  return sep_list;
}


void free_alu_list(ValNodePtr alu_sep_list, ValNodePtr alu_slp_list)
{
	ValNodePtr curr;
	SeqEntryPtr sep;
	SeqLocPtr slp;

	for(curr = alu_sep_list; curr!= NULL; curr= curr->next)
	{
		sep = (SeqEntryPtr)(curr->data.ptrvalue);
		SeqEntryFree(sep);
	}
	ValNodeFree(alu_sep_list);

	for(curr = alu_slp_list; curr!= NULL; curr= curr->next)
	{
		slp= (SeqLocPtr)(curr->data.ptrvalue);
		SeqLocFree(slp);
	}
	ValNodeFree(alu_slp_list);
}
	

#define MIN_K 1
#define OFFSET_K 100
static Int4 cal_K_num(Int4 query_len, Int4 db_len)
{
	Int4 num;

	num = MAX(MIN_K, (query_len/(db_len+OFFSET_K)+1));
	return num;
}

	

static SeqLocPtr store_align_ends(ValNodePtr ends_list, SeqIdPtr sip)
{
	
	RecordEndsPtr rep;
	ValNodePtr curr;
	SeqLocPtr slp_head = NULL, new;
	SeqLocPtr slp = NULL;
	

	if(ends_list == NULL)
		return NULL;

	for(curr = ends_list; curr != NULL; curr = curr->next)
	{
		rep = (RecordEndsPtr)(curr->data.ptrvalue);
		new = SeqLocIntNew(rep->start, rep->stop, rep->strand, sip);
		/*printf("%s: start is %ld, stop is %ld, strand is %d\n", rep->name, rep->start, rep->stop, rep->strand);*/
		ValNodeLink(&slp_head, new);
	}
	if(slp_head !=NULL)
	{
		slp = ValNodeNew(NULL);
		slp->choice = SEQLOC_MIX;
		slp->data.ptrvalue = slp_head;
	}
	return slp;
}

static Int4 get_word_size(Int4 length)
{
	/*#ifdef WIN_MSWIN
		return 6;
	#endif*/

	/* #ifdef WIN16
		return 6;
	#endif
	*/
	
	if(length < 500)
		return 6;
	if(length <10000)
		return 7;
	return 8;
}


/*******************************************************************
*
*	SaveRepeats(bsp, ends_list)
*	converts the alignments stored in ends_list into the Seq-feat 
*	repeat feature in bsp
*
*******************************************************************/
static SeqFeatPtr build_repeat_feat(ValNodePtr ends_list, SeqIdPtr sip)
{
	RecordEndsPtr rep;
	SeqFeatPtr sfp = NULL, new_sfp, curr;
	ImpFeatPtr ifp;
	/* Char temp[100]; */
	GBQualPtr qual;
	Char c='%';

	while(ends_list)
	{
		rep = (RecordEndsPtr)(ends_list->data.ptrvalue);
		new_sfp = SeqFeatNew();
		new_sfp->data.choice = 8; 
		ifp = ImpFeatNew();
		ifp->key = StringSave("repeat_region");
		new_sfp->data.value.ptrvalue = ifp;
		/* sprintf(temp, "%d%c similarity", (Int2)(100.0 * rep->zscore), c);
		new_sfp->comment = StringSave(temp); */
		new_sfp->location = SeqLocIntNew(rep->start, rep->stop, rep->strand, sip);
		qual = GBQualNew();
		qual->qual = StringSave("rpt_family");
		qual->val = StringSave(rep->name);
		/* if(StringCmp(rep->name, "L1") ==0)
			qual->val = StringSave("L1");
		else
		{
			if(StrStr(rep->name, "MER"))
				qual->val = StringSave(rep->name);
			else
				qual->val = StringSave("ALU");
		} */
		new_sfp->qual = qual;

		if(sfp == NULL)
			sfp = new_sfp;
		else
		{
			curr = sfp;
			while(curr->next != NULL)
				curr = curr->next;
			curr->next = new_sfp;
		}

		ends_list = ends_list->next;
	}

	return sfp;
}
		

void SaveRepeats(BioseqPtr bsp, ValNodePtr ends_list)
{
	SeqFeatPtr newsfp;
	SeqAnnotPtr annot, prev;
	ValNodePtr desc;

	newsfp = build_repeat_feat(ends_list, bsp->id);
	if(newsfp == NULL)
		return;
	desc = ValNodeNew(NULL);
	desc->choice = Annot_descr_name;
	desc->data.ptrvalue = StringSave("powblast");

	annot = SeqAnnotNew();
	annot->type = 1;
	annot->data = newsfp;
	annot->desc = desc;

	if(bsp->annot == NULL)
		bsp->annot = annot;
	else
	{
		prev = bsp->annot;
		while(prev->next != NULL)
			prev = prev->next;
		prev->next = annot;
	}
	return;
}

static Boolean filter_align(Int4 length, FloatHi zscore, FloatHi score)
{
	FloatHi MINSCORE = 0.6;

        if(zscore < MINSCORE)
                return FALSE;
        return (score > 30.0);

}


SeqLocPtr filter_repeats(SeqLocPtr slp, ValNodePtr alu_slp_list, ValNodePtr PNTR ends_list)
{
	SimPam sp;
	ValNodePtr curr;
	SeqLocPtr sloc;
	Int4 length;
	Int4 k;
	ValNodePtr new_list, this_list = NULL;
	SeqLocPtr aluloc = NULL;
	
	if(alu_slp_list == NULL || slp == NULL)
		return NULL;
		
	DefaultSimPam(&sp);
	length = SeqLocLen(slp);
	sp.word = get_word_size(length);
	sp.cutoff = 30.0;
	this_list = NULL;
	for(curr = alu_slp_list; curr != NULL; curr= curr->next)
	{
		sloc = (SeqLocPtr)(curr->data.ptrvalue);
		k = cal_K_num(length, SeqLocLen(sloc));

		new_list = NULL;
		/* SIM2(slp, sloc, k, &sp, &new_list, OUTPUT_ENDS, NULL); */
		SIM2(slp, sloc, k, &sp, &new_list, OUTPUT_ENDS, (FilterProc)filter_align);
		this_list = merge_two_list(this_list, new_list);
		/* Change_Loc_Strand(sloc, Seq_strand_both); */
	}

	if(this_list != NULL)
	{
		this_list = CleanNewList(this_list);
		aluloc = store_align_ends(this_list, SeqLocId(slp));
		if(ends_list != NULL)
		{
			if(*ends_list == NULL)
				*ends_list = this_list;
			else
				*ends_list = merge_two_list(*ends_list, this_list);
		}
	}
		
	return aluloc;
}


/*****************************************************
*
*	filter_repeats_for_bigloc(slp, max_len, min_overlap)
*	break the slp into smaller pieces and merge the results
*
*	slp: the big Seq-loc
*	max_len: the maximum allowed length for search
*	if -1, set to default
*	min_overlap: the minimum length for search
*	if -1, set to the default
*
*******************************************************/
ValNodePtr filter_repeats_for_bigloc(SeqLocPtr slp, Int4 max_len, Int4 min_overlap, ValNodePtr alu_slp_list)
{
	ValNodePtr ends_list = NULL;
	SeqLocPtr b_loc, loc;
	SeqLocPtr aluloc;

	if(slp->choice != SEQLOC_INT && slp->choice != SEQLOC_WHOLE)
	{
		ErrPostEx(SEV_ERROR, 0, 0, "Sorry, can not handle SeqLoc for type %d", slp->choice);
		return NULL;
	}
	if(max_len == -1)
		max_len = REPEAT_BREAK_LEN;
	if(min_overlap == -1)
		min_overlap = REPEAT_OVERLAP_LEN;

	b_loc = break_blast_job(slp, SeqLocId(slp), REPEAT_BREAK_LEN, min_overlap);
	for(loc = b_loc; loc != NULL; loc = loc->next)
	{
		aluloc = filter_repeats(loc, alu_slp_list, &ends_list);
		SeqLocSetFree(aluloc);
	}
	SeqLocSetFree(b_loc);
	return ends_list;
}
	


void get_output_name(CharPtr name, CharPtr buf)
{
	Int4 len, i;


	i =0;
	for(; buf !=NULL; ++buf)
	{
	   if(*buf == '\n' || *buf == '\0' || *buf == '|')
		break;
	   else
	   	name[i++] = *buf;
	}
	name[i] = '\0';
	len = i;
	for(i = len-1; i>=0; --i)
	{
		if(name[i] == '.')
		{
			name[i] = '\0';
			return;
		}
	}
}
			
void write_asn_output(CharPtr name, BioseqPtr bsp, SeqAnnotPtr annot, Int4 option, Boolean io_type)
{
	SeqEntryPtr sep;
	Char out_file[100];
	AsnIoPtr aip;
	SeqAnnotPtr sap, prev, next;
	Boolean free_it;

	if(option == 0)
	{
		SeqAnnotFree(annot);
		return;
	}
	if(bsp == NULL)
	{
		SeqAnnotFree(annot);
		return;
	}

	if(bsp->annot == NULL)
		bsp->annot = annot;
	else
	{
		sap = bsp->annot;
		prev = NULL;

		while(sap)
		{
			free_it = FALSE;
			next = sap->next;
			if(sap->type == 2)
			{
				if(option == 1)	/*overwrite*/
				{
					free_it = TRUE;
					if(prev != NULL)
						prev->next = sap->next;
					else
						bsp->annot = sap->next;
					sap->next = NULL;
					SeqAnnotFree(next);
				}
			}
			if(!free_it)
				prev = sap;
			sap = next;
		}

		if(prev != NULL)
			prev->next = annot;
		else
			bsp->annot = annot;
	}

	sep = SeqEntryFind(bsp->id);	

        if(io_type == FILE_IO) {
          sprintf(out_file, "%s.ent", name);
          aip = AsnIoOpen(out_file, "w");
        } else { /* for STD_IO */
          aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
        }

	SeqEntryAsnWrite(sep, aip, NULL);
	AsnIoClose(aip);
}
	

Boolean save_SeqAnnot(SeqAnnotPtr annot, CharPtr name, Boolean io_type)
{
        Char sat_name[100];
	AsnIoPtr aip;

	if(annot->data == NULL)
		return FALSE;

        if(io_type == FILE_IO) {
          sprintf(sat_name, "%s.sat", name);
          aip = AsnIoOpen(sat_name, "w");
        } else { /* for STD_IO */
          aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
        }
	SeqAnnotAsnWrite(annot, aip, NULL);
	AsnIoClose(aip);
	return TRUE;
}

void load_dust_bsp(BioseqPtr dust_bsp, SeqLocPtr dustloc)
{
	ByteStorePtr bsp;
	SeqLocPtr slp = NULL;
	Int4 start, stop;
	Uint1 res = 'N';
	

	if(dust_bsp->mol == Seq_mol_aa)
		res = 'X';

	bsp = dust_bsp->seq_data;
	while(dustloc)
	{
		slp = NULL;
 		while((slp = SeqLocFindNext(dustloc, slp))!=NULL)
		{
			start = SeqLocStart(slp);
			stop = SeqLocStop(slp);
			BSSeek(bsp, start, SEEK_SET);
			for(; start <=stop; ++start)
                		BSPutByte(bsp, (Int2)res);
		}
		dustloc = dustloc->next;
	}
}


/*mask the current sequence with the repeat feature*/
typedef struct mask_repeat {
	BioseqPtr bsp;
	BioseqPtr dust_bsp;
}MaskFeat, PNTR MaskFeatPtr;
	
static void MaskWithRepeatFeature(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	SeqAnnotPtr annot;
	SeqFeatPtr sfp;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	SeqIdPtr sip;
	MaskFeatPtr mfp;
	ImpFeatPtr ifp;

	mfp = (MaskFeatPtr)(data);
	if(sep->choice == 1)
	{
		bsp = sep->data.ptrvalue;
		annot = bsp->annot;
	}
	else
	{
		bssp = sep->data.ptrvalue;
		annot = bssp->annot;
	}

	while(annot)
	{
		if(annot->type == 1)
		{
			sfp = annot->data;
			while(sfp)
			{
				if(sfp->data.choice == 8)
				{
					sip = SeqLocId(sfp->location);
					if(BioseqMatch(mfp->bsp, sip))
					{
						ifp = sfp->data.value.ptrvalue;
						if(ifp->key != NULL)
						{
							if(StringNCmp(ifp->key, "repeat", 6) == 0)
								load_dust_bsp(mfp->dust_bsp, sfp->location);
						}
					}
				}
				sfp = sfp->next;
			}
		}

		annot = annot->next;
	}
}
				
/*******************************************************************
*
*	look for the repeat feature in sep and mask the residue in 
*	dust_bsp
*
*******************************************************************/
void mask_with_repeat_feature(BioseqPtr bsp, SeqEntryPtr sep, BioseqPtr dust_bsp)
{
	MaskFeat mf;

	mf.bsp = bsp;
	mf.dust_bsp = dust_bsp;

	SeqEntryExplore(sep, (Pointer)&mf, MaskWithRepeatFeature);
}

BioseqPtr make_dust_bsp(BioseqPtr bsp, Int4 start, Int4 stop, SeqLocPtr dustloc)
{
	BioseqPtr dust_bsp;
	SeqPortPtr spp;
	ByteStorePtr b_store;
	Uint1 residue;
	Uint1 code;

	if(bsp->mol == Seq_mol_aa)
		code = Seq_code_iupacaa;
	else
		code = Seq_code_iupacna;

	spp = SeqPortNew(bsp, start, stop, Seq_strand_plus, code);
	b_store = BSNew(stop - start +2);
	BSSeek(b_store, 0, SEEK_SET);
	while((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF)
                BSPutByte(b_store, (Int2)residue);
	SeqPortFree(spp);
	

	dust_bsp = BioseqNew();
	dust_bsp->id = local_id_make("dust_bsp");
	dust_bsp->seq_data = b_store;
	dust_bsp->repr = Seq_repr_raw;
	dust_bsp->mol = bsp->mol;
	dust_bsp->length = stop - start +1;
	dust_bsp->topology = 1;
	dust_bsp->seq_data_type = code;

	if(dustloc != NULL)
		load_dust_bsp(dust_bsp, dustloc);
	return dust_bsp;
}

void prt_FASTA_file(BioseqPtr bsp, SeqLocPtr dustloc, Boolean io_type)
{
	FILE *fp;

	load_dust_bsp(bsp, dustloc);
        if(io_type == FILE_IO)
          fp = FileOpen("xxx.out", "w");
        else
          fp = stdout;
	BioseqRawToFasta(bsp, fp, TRUE);
        if(io_type == FILE_IO)
          FileClose(fp);
}


void restore_blast_id(SeqAnnotPtr sap, SeqIdPtr sip)
{
	SeqAlignPtr align;
	DenseDiagPtr ddp;
	DenseSegPtr dsp;
	SeqIdPtr next_id;
	StdSegPtr ssp;
	SeqLocPtr slp;
	SeqIntPtr sint;

	if(sap == NULL || sip == NULL)
		return;
	align = sap->data;
	while(align)
	{
		switch(align->segtype)
		{
			case 1:
				ddp = align->segs;
				while(ddp)
				{
					next_id = ddp->id->next;
					ddp->id->next = NULL;
					SeqIdFree(ddp->id);
					ddp->id = SeqIdDup(sip);
					ddp->id->next = next_id;
					ddp = ddp->next;
				}
				break;

			case 2:
				dsp = align->segs;
				if(dsp)
				{
					next_id = dsp->ids->next;
					dsp->ids->next = NULL;
					SeqIdFree(dsp->ids);
					dsp->ids = SeqIdDup(sip);
					dsp->ids->next = next_id;
				}
				break;

			case 3:
				ssp = align->segs;
				while(ssp)
				{
					next_id = ssp->ids->next;
					ssp->ids->next = NULL;
					SeqIdFree(ssp->ids);
					ssp->ids = SeqIdDup(sip);
					ssp->ids->next = next_id;
					
					slp = ssp->loc;
					if(slp->choice == SEQLOC_INT)
					{
						sint = slp->data.ptrvalue;
						SeqIdFree(sint->id);
						sint->id = SeqIdDup(sip);
					}
					ssp = ssp->next;
				}
				break;

			default:
				break;
		}
		align = align->next;
	}
}

static void link_ddp(DenseDiagPtr PNTR head, DenseDiagPtr ddp_2)
{
	DenseDiagPtr ddp_1;
	
	if(*head == NULL)
		*head = ddp_2;
	else
	{
		ddp_1 = *head;
		while(ddp_1->next != NULL)
			ddp_1 = ddp_1->next;
		ddp_1->next = ddp_2;
	}
}

void link_blast_sap(SeqAnnotPtr PNTR blast_sap, SeqAnnotPtr new)
{
	SeqAlignPtr align, new_align, next_new_align;
	SeqAnnotPtr head;
	DenseDiagPtr ddp, new_ddp;
	SeqIdPtr new_id;

	if(*blast_sap == NULL)
		*blast_sap = new;
	else
	{
		head = *blast_sap;
		align = head->data;
		if(align == NULL)
			head->data = new->data;
		else
		{
			new_align = new->data;
			while(new_align)
			{
				next_new_align = new_align->next;
				new_align->next = NULL;
				new_ddp = new_align->segs;
				new_id = new_ddp->id->next;

				align = head->data;
				while(align)
				{
					ddp = align->segs;
					if(SeqIdMatch(new_id, ddp->id->next))
					{
						link_ddp(&ddp, new_ddp);
						new_align->segs = NULL;
						SeqAlignFree(new_align);
						break;
					}
					align = align->next;
				}
				if(align == NULL)
					head->data = link_align(new_align, (SeqAlignPtr )(head->data));
				new_align = next_new_align;
			}
		}
		new->data = NULL;
		SeqAnnotFree(new);
	}
}
				
static Uint1 get_strand_val(Uint1Ptr strands, Int2 i)
{
	if(strands == NULL)
		return Seq_strand_plus;
	if(strands[i] != Seq_strand_minus)
		return Seq_strand_plus;
	else
		return Seq_strand_minus;
}

static DenseDiagPtr filter_self_diagnol(DenseDiagPtr PNTR head)
{
	DenseDiagPtr prev = NULL, ddp;
	DenseDiagPtr next;
	Uint1 strand_1, strand_2;

	ddp = *head;
	while(ddp)
	{
		next = ddp->next;
		strand_1 = get_strand_val(ddp->strands, 0);
		strand_2 = get_strand_val(ddp->strands, 1);
		if((strand_1  == strand_2) && (ddp->starts[0] == ddp->starts[1]))
		{
			if(prev == NULL)
				*head = next;
			else
				prev->next = next;
			ddp->next = NULL;
			DenseDiagFree(ddp);
		}
		else
			prev = ddp;
		ddp = next;
	}

	return (*head);
}

		
			
void filter_blast_query(SeqAnnotPtr blast_sap, BioseqPtr query)
{
	SeqAlignPtr align, next, prev = NULL;
	DenseDiagPtr ddp;
	Boolean free_it;
	

	if(query == NULL || blast_sap == NULL)
		return;

	align = blast_sap->data;
	if(align == NULL)
		return;
	while(align)
	{
		next = align->next;
		free_it = FALSE;
		switch(align->segtype) {
			case 1:
				ddp = align->segs;
				if(ddp && ddp->id != NULL)
				{
					if(BioseqMatch(query, ddp->id->next))
					{
						filter_self_diagnol(&ddp);
						if(ddp == NULL)
						{
							if(prev == NULL)
								blast_sap->data = next;
							else
								prev->next = next;
							align->next = NULL;
							align->segs = NULL;
							SeqAlignFree(align);
							free_it = TRUE;
						}
						else
							align->segs = ddp;
					
					}
				}
				break;
			case 2:
				break;
			case 3:
				break;
			default:
				break;
		}
		if(!free_it)
			prev = align;
		align = next;
	}
}
	


	
	
			


/*********************************************************************
*
*	break_blast_job(slp)
*	if the Seq-loc is too long for blast search, break it into 
*	overlapping pieces to search more efficiently
*	slp: the query sequence
*	return  a list of Seq-locs for blast search
*
*********************************************************************/
SeqLocPtr break_blast_job(SeqLocPtr slp, SeqIdPtr sip, Int4 max_len, Int4 overlap_len)
{
	Int4 K;
	Int4 length, len;
	SeqLocPtr head = NULL, new;
	Int4 start, stop;
	Uint1 strand;

	length = SeqLocLen(slp);
	start = SeqLocStart(slp);
	stop = SeqLocStop(slp);
	strand = SeqLocStrand(slp);

	if(length < max_len)
		return SeqLocIntNew(start, stop, strand, sip);
	else
	{
		K = (length)/max_len;
		if((length)%max_len!= 0)
			++K;
		len = length/K + overlap_len;
		while(K >0)
		{
			if(K == 1)
				stop = SeqLocStop(slp);
			else
				stop = (start + len -1);
			new = SeqLocIntNew(start, stop, strand, sip);
			ValNodeLink(&head, new);
			start += (len - overlap_len-1);
			--K;
		}
		return head;
	}
}


/*##################################################################
#
# 	functions related to merge_blast_result
#
###################################################################*/

static Boolean DenseDiagForSameSequence(DenseDiagPtr ddp_1, DenseDiagPtr ddp_2)
{
	SeqIdPtr sip_1, sip_2;

	if(ddp_1 == NULL || ddp_2 == NULL)
		return FALSE;

	if(ddp_1->dim != ddp_2->dim)
		return FALSE;
	sip_1 = ddp_1->id;
	sip_2 = ddp_2->id;
	while(sip_1 && sip_2)
	{
		if(!SeqIdMatch(sip_1, sip_2))
			return FALSE;
		sip_1 = sip_1->next;
		sip_2 = sip_2->next;
	}
	return TRUE;
}

static Boolean is_same_orientation(Uint1 strand_1, Uint1 strand_2)
{
	if(strand_1 == strand_2)
		return TRUE;
	if(strand_1 == Seq_strand_minus)
		return (strand_2 == Seq_strand_minus);
	else
		return (strand_2 != Seq_strand_minus);
}

static Boolean DenseDiagSameOrient(DenseDiagPtr ddp_1, DenseDiagPtr ddp_2)
{
	Int2 dim, i;
	Uint1 strand_1, strand_2;

	if(ddp_1 == NULL || ddp_2 == NULL)
		return FALSE;
	if(ddp_1->dim != ddp_2->dim)
		return FALSE;

	dim = ddp_1->dim;
	for(i =0; i<dim; ++i)
	{
		strand_1 = get_strand_val(ddp_1->strands, i);
		strand_2 = get_strand_val(ddp_2->strands, i);
		if(!is_same_orientation(strand_1, strand_2))
			return FALSE;
	}

	return TRUE;
}


static Boolean StdSegForSameSequence(StdSegPtr ssp_1, StdSegPtr ssp_2)
{
	SeqIdPtr sip_1, sip_2;
	SeqLocPtr loc_1, loc_2;

	if(ssp_1 == NULL || ssp_2 == NULL)
		return FALSE;

	loc_1 = ssp_1->loc;
	loc_2 = ssp_2->loc;
	while(loc_1 && loc_2)
	{
		sip_1 = SeqLocId(loc_1);
		sip_2 = SeqLocId(loc_2);
		if(!SeqIdMatch(sip_1, sip_2))
			return FALSE;
		loc_1 = loc_1->next;
		loc_2 = loc_2->next;
	}
	return TRUE;
}



static Boolean StdSegSameOrient(StdSegPtr ssp_1, StdSegPtr ssp_2)
{
	Uint1 strand_1, strand_2;
	SeqLocPtr loc_1, loc_2;

	if(ssp_1 == NULL || ssp_2 == NULL)
		return FALSE;
	if(ssp_1->dim != ssp_2->dim)
		return FALSE;

	loc_1 = ssp_1->loc;
	loc_2 = ssp_2->loc;
	while(loc_1 && loc_2)
	{
		strand_1 = SeqLocStrand(loc_1);
		strand_2 = SeqLocStrand(loc_2);
		if(!is_same_orientation(strand_1, strand_2))
			return FALSE;
		loc_1 = loc_1->next;
		loc_2 = loc_2->next;
	}
	return TRUE;
}


static void StdSegLink (StdSegPtr PNTR head, StdSegPtr ssp)
{
	StdSegPtr curr;

	curr = *head;
	if(curr == NULL)
		*head = ssp;
	else
	{
		while(curr->next != NULL)
			curr = curr->next;
		curr->next = ssp;
	}
}

static Boolean merge_StdSeg(StdSegPtr ssp_1, StdSegPtr ssp_2)
{
	Int4 overlap = -1;
	Uint1 strand_1, strand;
	Int4 start_1, stop_1, start_2, stop_2;
	Int4 a_start, a_stop;	/*start and stop position of an aa sequence*/
	SeqIntPtr sint;
	SeqLocPtr slp;


	if(!StdSegSameOrient(ssp_1, ssp_2))
		return FALSE;

	strand_1 = SeqLocStrand(ssp_1->loc);
	strand = strand_1;
	start_1 = SeqLocStart(ssp_1->loc);
	stop_1 = SeqLocStop(ssp_1->loc);
	start_2 = SeqLocStart(ssp_2->loc);
	stop_2 = SeqLocStop(ssp_2->loc);
	if(start_2 > stop_1)	/*no overlaps*/
		return FALSE;
	else
	{
		if((stop_1 - start_2 + 1)%3 != 0)	/*check if they are in frame*/
			return FALSE;
		else
		{
			if(strand == Seq_strand_minus)
			{
				a_stop = SeqLocStart(ssp_2->loc->next);
				a_start = SeqLocStop(ssp_1->loc->next);
			}
			else
			{
				a_stop = SeqLocStop(ssp_1->loc->next);
				a_start = SeqLocStart(ssp_2->loc->next);
			}
			if((a_stop - a_start +1) *3 != (stop_1 - start_2 +1))
				return FALSE;
			else
			{
				slp = ssp_1->loc;
				if(slp->choice != SEQLOC_INT)
					return FALSE;
				sint = slp->data.ptrvalue;
				sint->to = MAX(sint->to, stop_2);

				slp = ssp_1->loc->next;
				sint = slp->data.ptrvalue;
				if(strand == Seq_strand_minus)
					sint->from = MIN(sint->from, SeqLocStart(ssp_2->loc->next));
				else
					sint->to = MAX(sint->to, SeqLocStop(ssp_2->loc->next));
				return TRUE;
			}
		}
	}

}


/*******************************************************************
*
*	merge_DenseDiag(DenseDiagPtr ddp_1, DenseDiagPtr ddp_2)
*	merge the two DenseDiags if they overlap and form the same 
*	aligned diagnol. It is assumed that ddp_1 is a diag computed
*	on a location before ddp_2. Their relative order is taken 
*	into account
*
********************************************************************/
static Boolean merge_DenseDiag(DenseDiagPtr ddp_1, DenseDiagPtr ddp_2)
{
	Int2 i, dim;
	Int4 overlap = -1;
	Uint1 strand;
	Int4 start, stop;

	/*it is assumed that the dimension and Seq-id has been checked before-hand*/
	if(!DenseDiagSameOrient(ddp_1, ddp_2))
		return FALSE;

	dim = ddp_1->dim;
	for(i =0; i<dim; ++i)
	{
		strand = get_strand_val(ddp_1->strands, i);
		if(strand== Seq_strand_minus)
		{
			start = ddp_2->starts[i] + ddp_2->len -1;
			stop = ddp_1->starts[i];
		}
		else
		{
			start = ddp_2->starts[i];
			stop = ddp_1->starts[i] + ddp_1->len -1;
		}
		if(start > stop)	/*no overlap*/
			return FALSE;
		if(overlap == -1)
			overlap = stop - start;
		else
			if(stop - start != overlap)
				return FALSE;
	}

	if(overlap == -1)
		return FALSE;
	for(i = 0; i<dim; ++i)
	{
		if(get_strand_val(ddp_1->strands, i) == Seq_strand_minus)
			ddp_1->starts[i] -= overlap;
	}
	ddp_1->len += (ddp_2->len -1 - overlap);
	return TRUE;
}




/*******************************************************************
*
*	align_for_same_sequences(sap_1, sap_2)
*	check if the same sequences are involved in alignments
*
********************************************************************/
static Boolean align_for_same_sequences(SeqAlignPtr sap_1, SeqAlignPtr sap_2)
{
	DenseDiagPtr ddp_1, ddp_2;
	StdSegPtr ssp_1, ssp_2;

	if(sap_1->segtype != sap_2->segtype)
		return FALSE;
	if(sap_1->segs == NULL || sap_2->segs == NULL)
		return FALSE;

	switch(sap_1->segtype)
	{
		case 1:	/*it is a Dense-diag*/
			ddp_1 = sap_1->segs;
			ddp_2 = sap_2->segs;
			return DenseDiagForSameSequence(ddp_1, ddp_2);

		case 3:
			ssp_1 = sap_1->segs;
			ssp_2 = sap_2->segs;
			return StdSegForSameSequence(ssp_1, ssp_2);
		default:
			return FALSE;
	}
}

static SeqAlignPtr find_previous_align_in_list(SeqAlignPtr list, SeqAlignPtr new_sap)
{
	while(list)
	{
		if(align_for_same_sequences(list, new_sap))
			return list;
		list = list->next;
	}

	return NULL;
}


Int4 get_score_value(ScorePtr sp)
{
	ObjectIdPtr oip;

	while(sp)
	{
		if(sp->id)
		{
			oip = sp->id;
			if(oip && oip->str && StringCmp(oip->str, "score") == 0)
			{
				if(sp->choice == 1)
					return sp->value.intvalue;
			}
		}
		sp = sp->next;
	}

	return -1;
}

static Boolean replace_with_big_score (DenseDiagPtr free_ddp, DenseDiagPtr ddp)
{
	Int4 free_score, score;

	if(free_ddp == NULL || ddp == NULL)
		return FALSE;
	if(ddp->scores == NULL)
	{
		ddp->scores = free_ddp->scores;
		free_ddp->scores = NULL;
		return TRUE;
	}
	else if(free_ddp->scores != NULL)
	{
		free_score = get_score_value(free_ddp->scores);
		score = get_score_value(ddp->scores);

		if(free_score > score)
		{
			ScoreSetFree(ddp->scores);
			ddp->scores = free_ddp->scores;
			free_ddp->scores = NULL;
			return TRUE;
		}
	}

	return FALSE;
}
static Boolean merge_one_align(SeqAlignPtr list, SeqAlignPtr new_sap)
{
	SeqAlignPtr prev_sap;
	DenseDiagPtr ddp_1, ddp_2;
	DenseDiagPtr newddp;
	DenseDiagPtr next;
	Boolean merge;
	StdSegPtr ssp_1, ssp_2;
	StdSegPtr new_ssp;
	StdSegPtr next_ssp;


	prev_sap  = find_previous_align_in_list(list, new_sap);
	if(prev_sap == NULL)
		return FALSE;

	switch(prev_sap->segtype)
	{
		case 1:	/*it is the Dense-diag*/
			ddp_2 = (DenseDiagPtr)(new_sap->segs);
			newddp = NULL;
			while(ddp_2)
			{
				next = ddp_2->next;
				ddp_2->next = NULL;
				ddp_1 = (DenseDiagPtr)(prev_sap->segs);
				merge = FALSE;
				while(ddp_1 )
				{
					merge = merge_DenseDiag(ddp_1, ddp_2);
					if(merge)
						break;
					else
						ddp_1 = ddp_1->next;
				}
				if(merge)
				{
					replace_with_big_score(ddp_2, ddp_1);
					DenseDiagFree(ddp_2);
				}
				else
					link_ddp(&newddp, ddp_2);

				ddp_2 = next;
			}
			if(newddp)
				link_ddp((DenseDiagPtr PNTR)&(prev_sap->segs), newddp);
			new_sap->segs = NULL;
		   return TRUE;

		case 3:
			ssp_2 = (StdSegPtr)(new_sap->segs);
			new_ssp = NULL;
			while(ssp_2)
			{
				next_ssp = ssp_2->next;
				ssp_2->next = NULL;
				merge = FALSE;
				ssp_1 = (StdSegPtr)(prev_sap->segs);
				while(ssp_1 && !merge)
				{
					merge = merge_StdSeg(ssp_1, ssp_2);
					ssp_1 = ssp_1->next;
				}
				if(merge)
					StdSegFree(ssp_2);
				else
					StdSegLink(&new_ssp, ssp_2);
				ssp_2 = next_ssp;
			}
			if(new_ssp)
				StdSegLink((StdSegPtr PNTR)&(prev_sap->segs), new_ssp);
			new_sap->segs = NULL;
			return TRUE;

		default:
			return FALSE;
	}

}
			
/*******************************************************************
*
*	merge_blast_result(head, new)
*	when the input query is too big, it is broken into several 
*	overlapping Seq-loc to be sent to blast. The results can be
*	merged if two Dense-diags are actually represent one HSP
*
********************************************************************/
SeqAnnotPtr merge_blast_result(SeqAnnotPtr PNTR head, SeqAnnotPtr new)
{
	SeqAlignPtr list, align, next;
	SeqAlignPtr newlist = NULL;

	if(*head == NULL)
		*head = new;
	else
	{
		list = (*head)->data;
		align = new->data;
		while(align)
		{
			next = align->next;
			align->next = NULL;
			if(merge_one_align(list, align))
				SeqAlignFree(align);
			else
				newlist = link_align(align, newlist);
			align = next;
		}
		if(newlist !=NULL)
		{
			list = link_align(newlist, list);
			(*head)->data = list;
		}
		new->data = NULL;
		SeqAnnotFree(new);
	}
	return (*head);
}
			
static void LIBCALLBACK GetOrg (AsnExpOptStructPtr aeosp)
{
  OrgRefPtr orp;
  CharPtr PNTR org_name;

        if (aeosp->dvp->intvalue != START_STRUCT){
		return;
	}

	 orp = (OrgRefPtr)aeosp->the_struct;
	 org_name = aeosp->data;
	 if(*org_name == NULL && orp->taxname)
		*org_name = StringSave(orp->taxname);
	return;
}
		
/********************************************************************
*
*	get_organism(bsp)
*	filter the taxname from the organism field in Bioseq
*
********************************************************************/
CharPtr get_organism(BioseqPtr bsp)
{
	SeqEntryPtr sep;
	AsnIoPtr aip;
	CharPtr org_name; 
	AsnExpOptPtr aeop;

	sep = SeqEntryFind(bsp->id);
        aip = AsnIoNullOpen();

	org_name = NULL;
        if ((aeop = AsnExpOptNew(aip, "Org-ref", (Pointer)(&org_name), GetOrg)) ==NULL)
                ErrPost(100, 1, "no aeop 1");
	SeqEntryAsnWrite(sep, aip, NULL);
        AsnExpOptFree(aip, aeop);
	AsnIoClose(aip);
	return org_name;
}
		

static Boolean merge_dense_diag_internal_repeats(DenseDiagPtr ddp_1, DenseDiagPtr ddp_2, BoolPtr pkeep_1)
{
	Int4 a_start_1, a_stop_1;
	Int4 a_start_2, a_stop_2;
	Int4 b_start_1, b_stop_1;
	Int4 b_start_2, b_stop_2;
	Boolean keep_1;

	if(!DenseDiagSameOrient(ddp_1, ddp_2))
		return FALSE;

	/*check overlap*/
	a_start_1 = ddp_1->starts[0];
	a_stop_1 = a_start_1 + ddp_1->len -1;
	a_start_2 = ddp_2->starts[0];
	a_stop_2 = a_start_2 + ddp_2->len -1;

	if(a_start_2 >= a_start_1 && a_stop_2 <= a_stop_1)
		keep_1 = TRUE;
	else if(a_start_1 >= a_start_2 && a_stop_1 <= a_stop_2)
		keep_1 = FALSE;
	else
		return FALSE;

	b_start_1 = ddp_1->starts[1];
	b_stop_1 = b_start_1 + ddp_1->len -1;
	b_start_2 = ddp_2->starts[1];
	b_stop_2 = b_start_2 + ddp_2->len -1;

	*pkeep_1 = keep_1;
	if(keep_1)
		return (b_start_2 >= b_start_1 && b_stop_2 <= b_stop_1);
	else
		return (b_start_1 >= b_start_2 && b_stop_1 <= b_stop_2);
}

static Boolean merge_std_seg_internal_repeats(StdSegPtr ssp_1, StdSegPtr ssp_2, BoolPtr pkeep_1)
{
	Int4 a_start_1, a_stop_1;
	Int4 a_start_2, a_stop_2;
	Int4 b_start_1, b_stop_1;
	Int4 b_start_2, b_stop_2;
	Boolean keep_1;
	SeqLocPtr loc_1, loc_2;

	if(!StdSegSameOrient(ssp_1, ssp_2))
		return FALSE;


	/*check overlap*/
	loc_1 = ssp_1->loc;
	loc_2 = ssp_2->loc;

	a_start_1 = SeqLocStart(loc_1);
	a_stop_1 = SeqLocStop(loc_1);
	a_start_2 = SeqLocStart(loc_2);
	a_stop_2 = SeqLocStop(loc_2);

	if(a_start_2 >= a_start_1 && a_stop_2 <= a_stop_1)
		keep_1 = TRUE;
	else if(a_start_1 >= a_start_2 && a_stop_1 <= a_stop_2)
		keep_1 = FALSE;
	else
		return FALSE;

	loc_1 = ssp_1->loc->next;
	loc_2 = ssp_2->loc->next;
	b_start_1 = SeqLocStart(loc_1);
	b_stop_1 = SeqLocStop(loc_1);
	b_start_2 = SeqLocStart(loc_2);
	b_stop_2 = SeqLocStop(loc_2);

	*pkeep_1 = keep_1;
	if(keep_1)
		return (b_start_2 >= b_start_1 && b_stop_2 <= b_stop_1);
	else
		return (b_start_1 >= b_start_2 && b_stop_1 <= b_stop_2);
}


static SeqAlignPtr clean_repeats_in_chain PROTO((SeqAlignPtr head));




static Boolean clean_dense_diag(DenseDiagPtr curr, DenseDiagPtr PNTR chain)
{

	DenseDiagPtr c_next, c_prev, next; 
	Boolean keep_1;

	c_prev = NULL;
	c_next = *chain;

	while(c_next)
	{
		keep_1 = FALSE;
		next = c_next->next;
		if(merge_dense_diag_internal_repeats(curr, c_next, &keep_1))
		{
		   if(keep_1)
		   {
				c_next->next = NULL;
				replace_with_big_score(c_next, curr);
				DenseDiagFree(c_next);
				if(c_prev == NULL)
					*chain = next;
				else
					c_prev->next = next;
			}
		    else
				return TRUE;
		}
		if(!keep_1)
		   c_prev = c_next;
		c_next = next;
	}

	return FALSE;
}


static Boolean clean_std_seg(StdSegPtr curr, StdSegPtr PNTR chain)
{

	StdSegPtr c_next, c_prev, next; 
	Boolean keep_1;

	c_prev = NULL;
	c_next = *chain;

	while(c_next)
	{
		keep_1 = FALSE;
		next = c_next->next;
		if(merge_std_seg_internal_repeats(curr, c_next, &keep_1))
		{
		   if(keep_1)
		   {
				c_next->next = NULL;
				StdSegFree(c_next);
				if(c_prev == NULL)
					*chain = next;
				else
					c_prev->next = next;
			}
		    else
				return TRUE;
		}
		if(!keep_1)
		   c_prev = c_next;
		c_next = next;
	}

	return FALSE;
}

/*clean all the repeats within the Seq-align*/
void clean_all_internal_repeats(SeqAlignPtr PNTR head)
{
	DenseDiagPtr ddp, ddp_next, ddp_prev;
	SeqAlignPtr align, ali_next, ali_prev = NULL;
	StdSegPtr ssp, ssp_next, ssp_prev;


	align = *head;

	while(align)
	{
		ali_next = align->next;
		switch(align->segtype)
		{
		case 1:
			ddp = align->segs;
			ddp_prev = NULL;
			while(ddp !=NULL && ddp->next !=NULL)
			{
				ddp_next = ddp->next;
				if(clean_dense_diag(ddp, &ddp_next))
				{
					if(ddp_prev == NULL)
						align->segs = ddp_next;
					else
						ddp_prev->next = ddp_next;
					ddp->next = NULL;
					replace_with_big_score(ddp, ddp_next);
					DenseDiagFree(ddp);
				}
				else
				{
					ddp->next = ddp_next;
					ddp_prev = ddp;
				}
				ddp = ddp_next;
			}
			break;

		case 3:
			ssp = align->segs;
			ssp_prev = NULL;
			while(ssp !=NULL && ssp->next !=NULL)
			{
				ssp_next = ssp->next;
				if(clean_std_seg(ssp, &ssp_next))
				{
					if(ssp_prev == NULL)
						align->segs = ssp_next;
					else
						ssp_prev->next = ssp_next;
					ssp->next = NULL;
					StdSegFree(ssp);
				}
				else
				{
					ssp->next = ssp_next;
					ssp_prev = ssp;
				}
				ssp = ssp_next;
			}
			break;

		default:
			break;
		}
		if(align->segs == NULL)
		{
			if(ali_prev)
				ali_prev->next = ali_next;
			else
				*head = ali_next;
			align->next = NULL;
			SeqAlignFree(align);
		}
		else
			ali_prev = align;
		align = ali_next;
	}

	*head = clean_repeats_in_chain(*head);
}
		

static void clean_repeat_current(SeqAlignPtr align, SeqAlignPtr next)
{
	DenseDiagPtr ddp_1, ddp_2, pddp_1, pddp_2, nddp_1, nddp_2;
	Boolean keep_1, free_1;
	StdSegPtr ssp_1, ssp_2, pssp_1, pssp_2, nssp_1, nssp_2;
	Uint1 segtype;

	if(align == NULL || next == NULL)
		return;
	segtype = align->segtype;

	while(next)
	{
		switch(segtype)
		{
		case 1:/*it is a Dense-diag*/
			ddp_1 = align->segs;
			pddp_1 = NULL;
			while(ddp_1)
			{
				ddp_2 = next->segs;
				pddp_2 = NULL;
				nddp_1 = ddp_1->next;
				free_1 = FALSE;
				while(ddp_2 && !free_1)
				{
					nddp_2 = ddp_2->next;
					if(merge_dense_diag_internal_repeats(ddp_1, ddp_2, &keep_1))
					{
						if(keep_1)
						{
							if(pddp_2 == NULL)
								next->segs = nddp_2;
							else
								pddp_2->next = nddp_2;
							ddp_2->next = NULL;
							replace_with_big_score(ddp_2, ddp_1);
							DenseDiagFree(ddp_2);
							ddp_2 = nddp_2;
						}
						else
						{
							if(pddp_1 == NULL)
								align->segs = nddp_1;
							else
								pddp_1->next = nddp_1;
							ddp_1->next = NULL;
							replace_with_big_score(ddp_1, ddp_2);
							DenseDiagFree(ddp_1);
							ddp_1 = nddp_1;
							free_1 = TRUE;
							break;
						}
					}
					else
					{
						pddp_2 = ddp_2;
						ddp_2 = ddp_2->next;
					}
				}

				if(!free_1)
				{
					pddp_1 = ddp_1;
					ddp_1 = ddp_1->next;
				}
			}
			break;

		case 3:		/*it is a Std-seg*/
			ssp_1 = align->segs;
			pssp_1 = NULL;
			while(ssp_1)
			{
				ssp_2 = next->segs;
				pssp_2 = NULL;
				nssp_1 = ssp_1->next;
				free_1 = FALSE;
				while(ssp_2 && !free_1)
				{
					nssp_2 = ssp_2->next;
					if(merge_std_seg_internal_repeats(ssp_1, ssp_2, &keep_1))
					{
						if(keep_1)
						{
							if(pssp_2 == NULL)
								next->segs = nssp_2;
							else
								pssp_2->next = nssp_2;
							ssp_2->next = NULL;
							StdSegFree(ssp_2);
							ssp_2 = nssp_2;
						}
						else
						{
							if(pssp_1 == NULL)
								align->segs = nssp_1;
							else
								pssp_1->next = nssp_1;
							ssp_1->next = NULL;
							StdSegFree(ssp_1);
							ssp_1 = nssp_1;
							free_1 = TRUE;
							break;
						}
					}
					else
					{
						pssp_2 = ssp_2;
						ssp_2 = ssp_2->next;
					}
				}

				if(!free_1)
				{
					pssp_1 = ssp_1;
					ssp_1 = ssp_1->next;
				}
			}
			break;

		default:
			break;

		}
		next = next->next;
	}
}


static SeqAlignPtr clean_repeats_in_chain(SeqAlignPtr head)
{
	SeqAlignPtr align, next, prev;

	align = head;
	while(align->next !=NULL)
	{
		next = align->next;
		if(align_for_same_sequences(align, next))
			clean_repeat_current(align, next);
		align = next;
	}

	/*free all the empty records*/
	align = head;
	prev = NULL;
	while(align)
	{
		next = align->next;
		if(align->segs == NULL)
		{
			if(prev == NULL)
				head->next = next;
			else
				prev->next = next;
			align->next = NULL;
			SeqAlignFree(align);
		}
		else
			prev = align;
		align = next;
	}

	return head;
}
		
				
			
static Boolean switch_Dense_diag(DenseDiagPtr ddp_1, DenseDiagPtr ddp_2)
{
	if(ddp_1->starts[0] > ddp_2->starts[0])
		return TRUE;

	if(ddp_1->starts[0] < ddp_2->starts[0])
		return FALSE;

	return (ddp_1->len < ddp_2->len);
}

static DenseDiagPtr sort_dense_diag(DenseDiagPtr head)
{
	DenseDiagPtr PNTR list;
	DenseDiagPtr curr, last = NULL;
	Int4 num =0, i, j;
	Boolean s_witch = TRUE;

	for(curr = head; curr != NULL; curr = curr->next)
		++num;
	if(num == 0 || num == 1)
		return head;

	list = MemNew((size_t)num * sizeof(DenseDiagPtr));
	for(i =0, curr = head; i<num; ++i)
	{
		list[i] = curr;
		curr = curr->next;
	}
	for(i = 0; i<num-1 && s_witch == TRUE; ++i)
	{
		s_witch = FALSE;
		for(j =0; j<num-i-1; ++j)
		{
			if(switch_Dense_diag(list[j], list[j+1]))
			{
				s_witch = TRUE;
				curr = list[j];
				list[j] = list[j+1];
				list[j +1] = curr;
			}
		}
	}

	for(i =0; i<num -1; ++i)
	{
		curr = list[i];
		curr->next = list[i+1];
		last = curr->next;
	}
	last->next = NULL;
	head = list[0];
	MemFree(list);
	return head;
}
	

static Boolean switch_std_seg(StdSegPtr ssp_1, StdSegPtr ssp_2)
{
	Int4 start_1, start_2;

	start_1 = SeqLocStart(ssp_1->loc);
	start_2 = SeqLocStart(ssp_2->loc);

	if(start_2 < start_1)
		return TRUE;
	else if(start_2 == start_1)
	{
		if(SeqLocStop(ssp_2->loc) < SeqLocStop(ssp_1->loc))
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
}


static StdSegPtr sort_std_seg(StdSegPtr ssp)
{
	StdSegPtr PNTR list;
	StdSegPtr curr, last = NULL;
	Int4 num =0, i, j;
	Boolean s_witch = TRUE;

	for(curr = ssp; curr != NULL; curr = curr->next)
		++num;
	if(num == 0 || num == 1)
		return ssp;

	list = MemNew((size_t)num * sizeof(DenseDiagPtr));
	for(i =0, curr = ssp; i<num; ++i)
	{
		list[i] = curr;
		curr = curr->next;
	}
	for(i = 0; i<num-1 && s_witch == TRUE; ++i)
	{
		s_witch = FALSE;
		for(j =0; j<num-i-1; ++j)
		{
			if(switch_std_seg(list[j], list[j+1]))
			{
				s_witch = TRUE;
				curr = list[j];
				list[j] = list[j+1];
				list[j +1] = curr;
			}
		}
	}

	for(i =0; i<num -1; ++i)
	{
		curr = list[i];
		curr->next = list[i+1];
		last = curr->next;
	}
	last->next = NULL;
	ssp = list[0];
	MemFree(list);
	return ssp;
}

typedef struct parse_blast{
	Int4 K;
	SeqLocPtr m_loc, s_loc;
	ScorePtr scores;	/*the original scores from blast */
}ParseBlast, PNTR ParseBlastPtr;

#define MAX_GAP_LEN 200	/*maximum gap length allowed by sim2*/
#define DNA_EDGE_LEN 1000	/*add the length of the edge factor*/
#define AA_EDGE_LEN 100

static void check_sim_list(DenseDiagPtr ddp, ValNodePtr PNTR head)
{
	ValNodePtr sim_list;
	ParseBlastPtr pbp;
	Int4 m_start, m_stop;
	Int4 s_start, s_stop;
	Int4 c_start, c_stop;
	Boolean increase_K;
	Uint1 strand;
	SeqIdPtr s_sip;
	Boolean found;
	Int4 score, blast_score;

	m_start = ddp->starts[0];
	m_stop = m_start + ddp->len -1;
	s_start = ddp->starts[1];
	s_stop = s_start + ddp->len -1;
	strand = get_strand_val(ddp->strands, 0);

	sim_list = *head;
	while(sim_list)
	{
	   pbp = sim_list->data.ptrvalue;
	   s_sip = SeqLocId(pbp->s_loc);
	   if(SeqIdMatch(s_sip, ddp->id->next) && strand == SeqLocStrand(pbp->s_loc))
	   {
		   if(s_sip->choice == SEQID_GI && s_sip->data.intvalue == 1418720)
			   found = TRUE;
		increase_K = FALSE;
		found = TRUE;
		c_start = SeqLocStart(pbp->m_loc);
		c_stop = SeqLocStop(pbp->m_loc);
		if(c_start > m_stop)
		{
			if(c_start - m_stop > MAX_GAP_LEN)
			{
				if(c_start - m_stop > DNA_EDGE_LEN)
					found = FALSE;
				else
					increase_K = TRUE;
			}
		}

		if(c_stop < m_start)
		{
			if(m_start - c_stop > MAX_GAP_LEN)
			{
				if(m_start - c_stop > DNA_EDGE_LEN)
					found = FALSE;
				else
					increase_K = TRUE;
			}
		}

		c_start = SeqLocStart(pbp->s_loc);
		c_stop = SeqLocStop(pbp->s_loc);
		if(c_start > s_stop)
		{
			if(c_start - s_stop > MAX_GAP_LEN)
			{
				if(c_start - s_stop > DNA_EDGE_LEN)
					found = FALSE;
				else
					increase_K = TRUE;
			}
		}

		if(c_stop < s_start)
		{
			if(s_start - c_stop > MAX_GAP_LEN)
			{
				if(s_start - c_stop > DNA_EDGE_LEN)
					found = FALSE;
				else
					increase_K = TRUE;
			}
		}
		if(found)
		{
			c_start = MIN(m_start, SeqLocStart(pbp->m_loc));
			c_stop = MAX(m_stop, SeqLocStop(pbp->m_loc));
			update_seq_loc(c_start, c_stop, Seq_strand_plus, pbp->m_loc);

			c_start = MIN(s_start, SeqLocStart(pbp->s_loc));
			c_stop = MAX(s_stop, SeqLocStop(pbp->s_loc));
			update_seq_loc(c_start, c_stop, 0, pbp->s_loc);

			if(increase_K)
				++pbp->K;
			blast_score = get_score_value(ddp->scores);
			score = get_score_value(pbp->scores);
			if(blast_score > score)
			{
				ScoreSetFree(pbp->scores);
				pbp->scores = ddp->scores;
				ddp->scores = NULL;
			}
			return;
		}
	    }
	    sim_list = sim_list->next;
	}

	pbp = MemNew(sizeof(ParseBlast));
	pbp->K = 1;
	pbp->m_loc = SeqLocIntNew(m_start, m_stop, Seq_strand_plus, ddp->id);
	pbp->s_loc = SeqLocIntNew(s_start, s_stop, strand, ddp->id->next);
	pbp->scores = ddp->scores;
	ddp->scores = NULL;
	ValNodeAddPointer(head, 0, (Pointer)pbp);

}

/****************************************************************
*
*	SortAlignByLocation()
*	Sort the DenseDiag from Blastn and Blastp to ascending order 
*	of the locations
*
*****************************************************************/
void SortAlignByLocation(SeqAnnotPtr blast_sap)
{
	SeqAlignPtr align;
	DenseDiagPtr ddp;
	StdSegPtr ssp;


	if(blast_sap == NULL)
		return;

	align = blast_sap->data;
	while(align)
	{
		switch(align->segtype)
		{
		case 1:
			ddp = (DenseDiagPtr)align->segs;
			align->segs = sort_dense_diag(ddp);
			break;
		case 2:
			break;
		case 3:
			ssp = align->segs;
			align->segs = (StdSegPtr)sort_std_seg(ssp);
			break;
		default:
			break;
		}
		align = align->next;
	}

}


static SeqAlignPtr filter_redundent_sim_alignment(SeqAlignPtr PNTR align)
{
	SeqAlignPtr curr, prev, next;
	DenseSegPtr dsp;
	SeqLocPtr slp, h_slp, n_slp;
	SeqIdPtr sip;
	Int4 start, stop;
	Uint1 strand;

	h_slp = NULL;
	for(curr = *align; curr != NULL; curr = curr->next)
	{
		dsp = curr->segs;
		sip = dsp->ids->next;
		get_align_ends(curr, sip, &start, &stop, &strand);
		slp = SeqLocIntNew(start, stop, strand, sip);
		ValNodeLink(&h_slp, slp);
	}

	for(slp = h_slp; slp != NULL; slp = slp->next)
	{
		if(slp->choice != 0)	
		{
			for(n_slp = slp->next; n_slp != NULL; n_slp = n_slp->next)
			{
				if(n_slp->choice != 0)
				{
					if(SeqLocStrand(slp) == SeqLocStrand(n_slp))
					{
						switch(SeqLocCompare(slp, n_slp))
						{
							case SLC_A_EQ_B:
							case SLC_B_IN_A:
							case SLC_A_OVERLAP_B:
								n_slp->choice = 0;
								break;
							case SLC_A_IN_B:
								slp->choice = 0;
								break;
							default:
								break;
						}
					}
				}
			}
		}
	}

	curr = *align;
	slp = h_slp;
	prev = NULL;

	while(curr && slp)
	{
		next = curr->next;
		if(slp->choice == 0)
		{
			slp->choice = SEQLOC_INT;
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
		slp = slp->next;
	}

	SeqLocSetFree(h_slp);
	return (*align);
}


			   
/****************************************************************
*
*	sim2_for_blast(blast_sap, query_id)
*	run sim2 with the Seq-annot from blast
*
****************************************************************/
SeqAnnotPtr  sim_for_blast(SeqAnnotPtr blast_sap, SeqIdPtr query_id, Int4 which_sim)
{
	SeqAlignPtr sim_align, align;
	ValNodePtr sim_list = NULL, curr;
	ParseBlastPtr pbp;
	BioseqPtr mbsp, sbsp;
	Int4 start, stop;
	SeqIdPtr s_sip;
	Uint1 strand;
	SeqAnnotPtr annot;
	DenseDiagPtr ddp;
	SimPam sp;		/*parameters for the sim2 alignment*/
	PSimPam psp;	/*parameters for the SIM alignment*/
	Int4 limit = 200;	/*limit of sequence discrepancies used in SIM3*/
	Int4 edge_len;	/*length for adding up to the edge*/
	Boolean is_aa;
	Char label[100];
	Int4 K;
	Int4 mbsplength;
	DenseSegPtr dsp;
/** BANDALIGN**/
	ValNodePtr vnp;
	MashPtr    msp;

	if(blast_sap == NULL || blast_sap->data == NULL)
		return NULL;
	mbsp = BioseqLockById(query_id);
	if(mbsp == NULL)
		return blast_sap;
	if(mbsp->mol == Seq_mol_aa)
	{
		edge_len = AA_EDGE_LEN;
		DefaultPSimPam(&psp);
		is_aa = TRUE;
	}
	else
	{
		edge_len = DNA_EDGE_LEN;
		DefaultSimPam(&sp);
		is_aa = FALSE;
	}
	mbsplength = mbsp->length;
	BioseqUnlock(mbsp);
        
	align = blast_sap->data;

        if (which_sim == RUN_BANDALGN) 
	{
	   vnp = SeqLocListOfBioseqsFromSeqAlign (align);
	   msp = MashNew (is_aa);
           align = SeqLocListToSeqAlign (vnp, PRGALIGNDEFAULT, msp); 
	   annot = SeqAnnotForSeqAlign (align);
	   return annot;
	}
	while(align)
   	   {
   	   switch (align->segtype) {
   	   	case 1:
			ddp = align->segs;
			while(ddp)
			{
				check_sim_list(ddp, &sim_list);
				ddp = ddp->next;
			}
   	   		break;
   	   	case 2:
   	   		dsp = align->segs;
   	   		if (dsp) {
   	   		}
   	   		break;
   	   	case 3:
   	   		break;
   	   	default:
   	   		break;
   	   }
		align = align->next;
	}
	sim_align = NULL;

	for(curr = sim_list; curr !=NULL; curr = curr->next)
	{
		pbp = curr->data.ptrvalue;
		s_sip = SeqLocId(pbp->s_loc);
		sbsp = BioseqLockById(s_sip);
		if(sbsp == NULL)
		{
			seqid_name(s_sip, label, FALSE, FALSE);
			Message(MSG_POSTERR, "Fail to retrieve sequence %s", label);
		}
		else
		{
			strand = SeqLocStrand(pbp->s_loc);

			start = SeqLocStart(pbp->s_loc);
			stop = SeqLocStop(pbp->s_loc);
			start = MAX(0, start - edge_len);
			stop = MIN(stop+edge_len, sbsp->length-1);
			update_seq_loc(start, stop, strand, pbp->s_loc);
		
			start = SeqLocStart(pbp->m_loc);
			stop = SeqLocStop(pbp->m_loc);
			start = MAX(0, start - edge_len);
			stop = MIN(stop+edge_len, mbsplength-1);
			update_seq_loc(start, stop, Seq_strand_plus, pbp->m_loc);

			if(pbp->K >1)
			{
				sp.cutoff = 30.0;
				K = MIN((pbp->K + 10), (pbp->K * 2));
			}
			else
			{
				sp.cutoff = 0.0;
				K = 1;
			}
			
			if(is_aa) {
				align = CSIM(pbp->m_loc, pbp->s_loc, pbp->K, sp.cutoff, &psp);
			}
			else
			{
				/* if(pbp->K >1)
					align = SIM2(pbp->m_loc, pbp->s_loc, pbp->K, &sp, NULL, OUTPUT_ALIGN, NULL);
				else
				{ */
					switch( which_sim)
					{
						case RUN_SIM_1:
							/* align = CSIM(pbp->m_loc, pbp->s_loc, pbp->K, sp.cutoff, &psp);
							break; */
	
						case RUN_SIM_2:
							align = SIM2(pbp->m_loc, pbp->s_loc, K, &sp, NULL, OUTPUT_ALIGN, NULL);
							if(K > 1 && align != NULL)
								filter_redundent_sim_alignment(&align);
							break;
	
						case RUN_SIM_3:
							align = SIM3ALN(pbp->m_loc, pbp->s_loc, limit);
							if(align == NULL)
								align = SIM2(pbp->m_loc, pbp->s_loc, pbp->K, &sp, NULL, OUTPUT_ALIGN, NULL);
							break;
						default:
							align = NULL;
					}
				/* } */
			}
			if(align != NULL)
			{
				dsp = align->segs;
				if(pbp->scores != NULL && dsp != NULL)
				{
					if(dsp->scores != NULL)
						ScoreSetFree(dsp->scores);
					dsp->scores = pbp->scores;
					pbp->scores = NULL;
				}
				sim_align = link_align(align, sim_align);
			}
			
			BioseqUnlock(sbsp);
		}

		pbp->m_loc = SeqLocFree(pbp->m_loc);
		pbp->s_loc = SeqLocFree(pbp->s_loc);
		if(pbp->scores != NULL)
			ScoreSetFree(pbp->scores);
		
	}

	ValNodeFreeData(sim_list);

	if (sim_align == NULL) return NULL;

	annot = SeqAnnotNew();
	annot->type = 2;
	annot->data = sim_align;
	return annot;
}

/***********************************************************
*
*  Function for finding human repeat features
*
************************************************************/

Boolean LIBCALL FindHumanRepeats (BioseqPtr bsp, Boolean show_prog_mon)
{
 	MonitorPtr mon=NULL;
	ValNodePtr alu_slp_list = NULL, alu_sep_list = NULL, ends_list = NULL;
	Char buf[80];
	Boolean got_some = FALSE;
	ValNode tmp;
	SeqInt si;

	MemSet((Pointer) (&tmp), sizeof(ValNode), 0);
	MemSet((Pointer) (&si), sizeof(SeqInt), 0);
	tmp.choice = SEQLOC_INT;
	tmp.data.ptrvalue = &si;
	si.id = bsp->id;
	si.from = 0;
	si.to = bsp->length - 1;
	si.strand = Seq_strand_both;

	if (show_prog_mon)
	{
		mon = MonitorStrNewEx("Alu Features", 40, FALSE);
		MonitorStrValue(mon, "Reading Repeats File");
	}

    if (! FindPath("ncbi", "ncbi", "data", buf, sizeof (buf)))
	{
		MonitorFree(mon);
		ErrPostEx(SEV_WARNING, 0, 0, "FindPath failed in repeat filter");
        return got_some;
	}

    StringCat(buf, "humrep.fsa");

	alu_sep_list = make_repeat_lib (buf, &alu_slp_list, FILE_IO);
	if (show_prog_mon)
		MonitorStrValue(mon, "Finding Repeats");
	ends_list = filter_repeats_for_bigloc(&tmp, -1, -1, alu_slp_list);
	if(ends_list != NULL)
	{
		got_some = TRUE;
		if (show_prog_mon)
			MonitorStrValue(mon, "Building repeat features");
		SaveRepeats (bsp, ends_list);
	}
	free_alu_list (alu_sep_list, alu_slp_list);

	MonitorFree(mon);

	return got_some;
}
/***********************************************************
*
*  Desktop function for doing human repeat features
*
************************************************************/

Int2 LIBCALLBACK AluFeat (Pointer data)
{
	OMProcControlPtr ompcp;
	ObjMgrProcPtr ompp;
	Pointer my_userdata;
	Boolean got_some;
	BioseqPtr bsp;
	
	ompcp = (OMProcControlPtr)data;
	if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
	if (ompcp->input_itemtype != OBJ_BIOSEQ) return OM_MSG_RET_ERROR;
	ompp = ompcp->proc;      /* this is your proceedure */
	my_userdata = ompp->procdata;   /* this is the user data pointer you passed in, if any */

	bsp = (BioseqPtr)(ompcp->input_data);

	got_some = FindHumanRepeats (bsp, TRUE);

	if (! got_some)
		Message(MSG_OK, "No Repeat regions found.");
	else
		ObjMgrSendMsg(OM_MSG_UPDATE, ompcp->input_entityID, ompcp->input_itemID, ompcp->input_itemtype);
		
	return OM_MSG_RET_OK;
}

			

/*functions to convert alignment to a feature*/
			
/**************************************************************
*
*	convert the alignment to an interval 
*
**************************************************************/
static SeqFeatPtr convert_align_to_interval(SeqAlignPtr align, Int4 min_gap_len)
{
	DenseSegPtr dsp;
	Int2 i;
	SeqLocPtr slp, slp_head, slp_next;
	Int4 s_start, start;
	Uint1 strand;
	Boolean load;
	SeqFeatPtr new_sfp;
	ImpFeatPtr ifp;

	if(align->segtype != 2)
		return NULL;

	dsp = align->segs;
	strand = dsp->strands[1];
	slp_head = NULL;

	for(i = 0; i<dsp->numseg; ++i)
	{
		start = dsp->starts[2*i];
		s_start = dsp->starts[2*i+1];
		load = FALSE;
		if(start != -1)
		{
			if(s_start != -1)
				load = TRUE;
			else if(dsp->lens[i] <= min_gap_len)
				load = TRUE;
		}
		if(load)
		{
			slp = SeqLocIntNew(start, start+dsp->lens[i]-1, strand, dsp->ids);
			if(slp_head == NULL)
				slp_head = slp;
			else
			{
			if(strand == Seq_strand_minus)
			{
				slp_next = slp_head->next;
				slp_head->next = NULL;
				slp_head = SeqLocAdd(&slp, slp_head, TRUE, FALSE);
				slp_head->next = slp_next;
				slp_head = slp;
			}
			else
				SeqLocAdd(&slp_head, slp, TRUE, FALSE);
			}
		}
	}

	new_sfp = SeqFeatNew();
	new_sfp->data.choice = 8;
	ifp = ImpFeatNew();
	ifp->key = StringSave("hit");
	new_sfp->data.value.ptrvalue = ifp;
	new_sfp->location = SeqLocPackage(slp_head);
	return new_sfp;
}

Boolean load_align_to_interval(SeqAnnotPtr align_annot, Int4 min_gap_len, BioseqPtr bsp)
{
	SeqFeatPtr sfp_head = NULL, prev, sfp;
	SeqAlignPtr align;
	ValNodePtr desc;
	SeqAnnotPtr annot, p_annot;

	prev = NULL;
	while(align_annot)
	{
		if(align_annot->type == 2)
		{
			align = align_annot->data;
			while(align)
			{
				sfp = convert_align_to_interval(align, min_gap_len);
				if(sfp != NULL)
				{
					if(prev == NULL)
						sfp_head = sfp;
					else
						prev->next =sfp;
					prev = sfp;
				}
				align = align->next;
			}
		}

		align_annot = align_annot->next;
	}

	if(sfp_head != NULL)
	{
        	desc = ValNodeNew(NULL);
        	desc->choice = Annot_descr_name;
        	desc->data.ptrvalue = StringSave("HITS");

        	annot = SeqAnnotNew();
        	annot->type = 1;
        	annot->data = sfp_head;
        	annot->desc = desc;


        	if(bsp->annot == NULL)
                	bsp->annot = annot;
        	else
        	{
                	p_annot = bsp->annot;
                	while(p_annot->next != NULL)
                        	p_annot = p_annot->next;
			p_annot->next = annot;
		}
		return TRUE;
	}
	else
		return FALSE;
}

