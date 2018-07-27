/* $Id: blast_seqalign.c,v 1.31 2004/04/19 15:03:20 papadopo Exp $
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's offical duties as a United States Government employee and
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
* ===========================================================================*/

/*****************************************************************************

File name: blast_seqalign.c

Author: Ilya Dondoshansky

Contents: Conversion of BLAST results to the SeqAlign form

******************************************************************************
 * $Revision: 1.31 $
 * */

static char const rcsid[] = "$Id: blast_seqalign.c,v 1.31 2004/04/19 15:03:20 papadopo Exp $";

#include <algo/blast/api/blast_seqalign.h>

extern Int4 LIBCALL HspArrayPurge PROTO((BlastHSP** hsp_array, 
                       Int4 hspcnt, Boolean clear_num));
extern SeqIdPtr GetTheSeqAlignID (SeqIdPtr seq_id);
extern ScorePtr MakeBlastScore (ScorePtr PNTR old, CharPtr scoretype, 
                                Nlm_FloatHi prob, Int4 score);

static ScorePtr 
GetScoreSetFromBlastHsp(Uint1 program_number, BlastHSP* hsp, 
   BlastScoreBlk* sbp, BlastScoringOptions* score_options)
{
   ScorePtr	score_set=NULL;
   double	prob;
   Int4		score;
   char*	scoretype;
   Blast_KarlinBlk* kbp;

   score = hsp->score;
   if (score > 0)
      MakeBlastScore(&score_set, "score", 0.0, score);

   score = hsp->num;
   scoretype = "sum_n";
   
   if (score > 1)
      MakeBlastScore(&score_set, scoretype, 0.0, score);
   
   prob = hsp->evalue;
   if (hsp->num == 1) {
      scoretype = "e_value";
   } else {
      scoretype = "sum_e";
   }
   if (prob >= 0.) {
      if (prob < 1.0e-180)
         prob = 0.0;
      MakeBlastScore(&score_set, scoretype, prob, 0);
   }

   if (!score_options->gapped_calculation) {
      kbp = sbp->kbp[hsp->context];
   } else {
      kbp = sbp->kbp_gap[hsp->context];
   }

   /* Calculate bit score from the raw score */
   prob = ((hsp->score*kbp->Lambda) - kbp->logK)/NCBIMATH_LN2;
   if (prob >= 0.)
      MakeBlastScore(&score_set, "bit_score", prob, 0);
   
   if (hsp->num_ident > 0)
      MakeBlastScore(&score_set, "num_ident", 0.0, hsp->num_ident);
   
   if (hsp->num > 1 && hsp->splice_junction > 0) { 
      /* In new tblastn this means splice junction was found */
      MakeBlastScore(&score_set, "splice_junction", 0.0, 1);
   }

   return score_set;
}

/** Add information on what gis to use to the score set */
static Int2 AddGiListToScoreSet(ScorePtr score_set, ValNodePtr gi_list)

{
   while (gi_list) {
      MakeBlastScore(&score_set, "use_this_gi", 0.0, gi_list->data.intvalue);
      gi_list = gi_list->next;
   }
   
   return 0;
}

/*************************************************************************
*
*	This function fills in the DenseDiag Information from the variable
*	hsp.  On the first call to this function *old should be
*	NULL, after that pass in the head of the DenseDiagPtr chain.
*	The newest DenseDiagPtr is returned.
*
************************************************************************/

static DenseDiagPtr
BLAST_HSPToDenseDiag(Uint1 program_number, DenseDiagPtr* old, 
   BlastHSP* hsp, Boolean reverse,
   Int4 query_length, Int4 subject_length, BlastScoreBlk* sbp, 
   BlastScoringOptions* score_options)
{
	DenseDiagPtr		ddp, new;

	new = DenseDiagNew();
	
	new->dim = 2;	/* Only 2 is supported in spec. */
	new->len = hsp->query.length;
	new->starts = (Int4*) calloc(2, sizeof(Int4));
	new->strands = (Uint1*) calloc(2, sizeof(Uint1));
	if (reverse)
	{
		if (hsp->subject.frame >= 0)
		{
			new->strands[0] = Seq_strand_plus;
			new->starts[0] = hsp->subject.offset;
		}
		else
		{
			new->strands[0] = Seq_strand_minus;
			new->starts[0] = subject_length - hsp->subject.offset - hsp->subject.length;
		}
		if (hsp->query.frame >= 0)
		{
			new->strands[1] = Seq_strand_plus;
			new->starts[1] = hsp->query.offset;
		}
		else
		{
			new->strands[1] = Seq_strand_minus;
			new->starts[1] = query_length - hsp->query.offset - hsp->query.length;
		}
	}
	else
	{
		if (hsp->query.frame >= 0)
		{
			new->strands[0] = Seq_strand_plus;
			new->starts[0] = hsp->query.offset;
		}
		else
		{
			new->strands[0] = Seq_strand_minus;
			new->starts[0] = query_length - hsp->query.offset - hsp->query.length;
		}
		if (hsp->subject.frame >= 0)
		{
			new->strands[1] = Seq_strand_plus;
			new->starts[1] = hsp->subject.offset;
		}
		else
		{
			new->strands[1] = Seq_strand_minus;
			new->starts[1] = subject_length - hsp->subject.offset - hsp->subject.length;
		}
	}
	new->scores = 
      GetScoreSetFromBlastHsp(program_number, hsp, sbp, score_options);

/* Go to the end of the chain, and then attach "new" */
	if (*old)
	{
		ddp = *old;
		while (ddp->next)
			ddp = ddp->next;
		ddp->next = new;
	}
	else
	{
		*old = new;
	}

	new->next = NULL;

	return new;
}

/*************************************************************************
*
*	This function fills in the StdSeg Information from the variable
*	hsp.  On the first call to this function *old should be
*	NULL, after that pass in the head of the DenseDiagPtr chain.
*	The newest StdSeg* is returned.
*
************************************************************************/
static StdSeg*
BLAST_HSPToStdSeg(Uint1 program_number, StdSeg** old, 
   BlastHSP* hsp, Int4 query_length, Int4 subject_length, SeqIdPtr sip, 
   Boolean reverse, BlastScoreBlk* sbp, 
   BlastScoringOptions* score_options)
{
	StdSeg*		ssp,* new;
	SeqIdPtr		query_sip, subject_sip;
	SeqIntPtr		seq_int1, seq_int2;
	SeqLocPtr		slp=NULL;

	new = StdSegNew();
/* Duplicate the id and split it up into query and subject parts */
	query_sip = SeqIdDup(sip);
	subject_sip = SeqIdDup(sip->next);
	
	new->dim = 2;	/* Only 2 is supported in spec. */
	seq_int1 = SeqIntNew();
	if (hsp->query.frame == 0)
	{
		seq_int1->from = hsp->query.offset;
		seq_int1->to = hsp->query.offset + hsp->query.length - 1;
		seq_int1->strand = Seq_strand_unknown;
	}
	else if (hsp->query.frame < 0)
	{
		seq_int1->to = query_length - CODON_LENGTH*hsp->query.offset
         + hsp->query.frame;
		seq_int1->from = query_length
         - CODON_LENGTH*(hsp->query.offset+hsp->query.length)
         + hsp->query.frame + 1;
		seq_int1->strand = Seq_strand_minus;
	}
	else if (hsp->query.frame > 0)
	{
		seq_int1->from = CODON_LENGTH*(hsp->query.offset) + hsp->query.frame - 1;
		seq_int1->to = CODON_LENGTH*(hsp->query.offset+hsp->query.length)
         + hsp->query.frame - 2;
		seq_int1->strand = Seq_strand_plus;
	}
	seq_int1->id = query_sip;
	seq_int2 = SeqIntNew();
	if (hsp->subject.frame == 0)
	{
		seq_int2->from = hsp->subject.offset;
		seq_int2->to = hsp->subject.offset + hsp->subject.length - 1;
		seq_int2->strand = Seq_strand_unknown;
	} 
	else if (hsp->subject.frame < 0)
	{
		seq_int2->from = subject_length - 
         CODON_LENGTH*(hsp->subject.offset + hsp->subject.length) 
         + hsp->subject.frame + 1;
		seq_int2->to = subject_length - CODON_LENGTH*(hsp->subject.offset) 
         + hsp->subject.frame;
		seq_int2->strand = Seq_strand_minus;
	}
	else if (hsp->subject.frame > 0)
	{
		seq_int2->from = CODON_LENGTH*(hsp->subject.offset) + 
         hsp->subject.frame - 1;
		seq_int2->to = CODON_LENGTH*(hsp->subject.offset + hsp->subject.length)
         + hsp->subject.frame - 2;
		seq_int2->strand = Seq_strand_plus;
	}
	seq_int2->id = subject_sip;

	if (reverse)
	{
		ValNodeAddPointer(&slp, SEQLOC_INT, seq_int2); 
		ValNodeAddPointer(&slp, SEQLOC_INT, seq_int1); 
	}
	else
	{
		ValNodeAddPointer(&slp, SEQLOC_INT, seq_int1); 
		ValNodeAddPointer(&slp, SEQLOC_INT, seq_int2); 
	}
	new->loc = slp;

	new->scores = 
      GetScoreSetFromBlastHsp(program_number, hsp, sbp, score_options);

/* Go to the end of the chain, and then attach "new" */
	if (*old)
	{
		ssp = *old;
		while (ssp->next)
			ssp = ssp->next;
		ssp->next = new;
	}
	else
	{
		*old = new;
	}

	new->next = NULL;

	return new;
}

/************************************************************************
*
*	This function assembles all the components of the Seq-align from
*	a "sparse" BLAST HitList.  "sparse" means that the hitlist 
*	may contain no sequence and not even a descriptor.  It is only 
*	required to contain the sequence_number that readdb refers to
*	and scoring/alignment information.
*
*	If dbname is non-NULL, then only a general ("gnl") ID is 
*	issued, with the ordinal number of the subject sequence in
*	the ObjectIdPtr.
*
*	Boolean reverse: reverse the query and db order in SeqAlign.
*
************************************************************************/
static Int2 
BLAST_UngappedHSPToSeqAlign(Uint1 program_number, 
   BlastHSPList* hsp_list, SeqIdPtr query_id, 
   SeqIdPtr subject_id, BlastScoreBlk* sbp, Int4 query_length,
   Int4 subject_length, BlastScoringOptions* score_options, 
   SeqAlignPtr* seqalign_ptr)
{
   BlastHSP* hsp;
   DenseDiagPtr	ddp_head=NULL, ddp;
   SeqIdPtr sip;
   SeqIdPtr new_sip=NULL;
   StdSeg* ssp_head=NULL,* ssp;
   SeqAlignPtr seqalign;
   Int4 hsp_cnt, index2, hspset_cnt_old;
   Boolean getdensediag = 
      (program_number == blast_type_blastn ||
       program_number == blast_type_rpsblast ||
       program_number == blast_type_blastp);

	ddp_head = NULL;
	ssp_head = NULL;
	sip = NULL;


   seqalign = SeqAlignNew();
   seqalign->type = 2;		/* alignment is diags */

   hspset_cnt_old = -1;
   hsp_cnt = hsp_list->hspcnt;

   for (index2=0; index2<hsp_cnt; index2++) {
      hsp = hsp_list->hsp_array[index2];

      sip = GetTheSeqAlignID(query_id);
      sip->next = SeqIdDup(subject_id);

      if (getdensediag) {
         ddp = BLAST_HSPToDenseDiag(program_number, &ddp_head, hsp, FALSE,
                  query_length, subject_length, sbp, score_options);
         ddp->id = sip;
      } else {
         ssp = BLAST_HSPToStdSeg(program_number, &ssp_head, hsp, 
                  query_length, subject_length, sip, FALSE, sbp, 
                  score_options);
         ssp->ids = sip;
      }
      sip = NULL; /* This SeqIdPtr is now on the SeqAlign. */
   }

   if (getdensediag) {
      seqalign->segs = ddp_head;
      seqalign->segtype = 1;  /* DenseDiag */
   } else {
      seqalign->segs = ssp_head;
      seqalign->segtype = 3;  /* StdSeg */
   }

   if (new_sip)
      new_sip = SeqIdFree(new_sip);

   *seqalign_ptr = seqalign;

   return 0;
}

/** Get the current position. */
static Int4 get_current_pos(Int4* pos, Int4 length)
{
    Int4 val;
    if(*pos < 0)
        val = -(*pos + length -1);
    else
        val = *pos;
    *pos += length;
    return val;
}

/** Given gap information in an edit block form, fill the starts, lengths and
 * strands arrays
 */
Boolean GapCollectDataForSeqalign(GapEditBlock* edit_block, 
                                  GapEditScript* curr_in, Int4 numseg,
                                  Int4** start_out, 
                                  Int4** length_out,
                                  Uint1** strands_out,
                                  Int4* start1, Int4* start2)
{
    GapEditScript* curr;
    Boolean reverse, translate1, translate2;
    Int2 frame1, frame2;
    Int4 begin1, begin2, index, length1, length2;
    Int4 original_length1, original_length2, i;
    Int4* length,* start;
    Uint1 strand1, strand2;
    Uint1* strands;
    
    reverse = edit_block->reverse;	
    length1 = edit_block->length1;
    length2 = edit_block->length2;
    original_length1 = edit_block->original_length1;
    original_length2 = edit_block->original_length2;
    translate1 = edit_block->translate1;
    translate2 = edit_block->translate2;
    frame1 = edit_block->frame1;
    frame2 = edit_block->frame2;
    
    if (frame1 > 0)
        strand1 = Seq_strand_plus; 
    else if (frame1 < 0)
        strand1 = Seq_strand_minus; 
    else
        strand1 = Seq_strand_unknown; 
    
    if (frame2 > 0)
        strand2 = Seq_strand_plus; 
    else if (frame2 < 0)
        strand2 = Seq_strand_minus; 
    else
        strand2 = Seq_strand_unknown; 

    start = (Int4 *) calloc((2*numseg+1), sizeof(Int4));
    *start_out = start;
    length = (Int4 *) calloc((numseg+1), sizeof(Int4));
    *length_out = length;
    strands = (Uint1 *) calloc((2*numseg+1), sizeof(Uint1));
    *strands_out = strands;

    index=0;
    for (i = 0, curr=curr_in; curr && i < numseg; curr=curr->next, i++) {
        switch(curr->op_type) {
        case GAPALIGN_DECLINE:
        case GAPALIGN_SUB:
            if (strand1 != Seq_strand_minus) {
                if(translate1 == FALSE)
                    begin1 = get_current_pos(start1, curr->num);
                else
                    begin1 = frame1 - 1 + CODON_LENGTH*get_current_pos(start1, curr->num);
            } else {
                if(translate1 == FALSE)
                    begin1 = length1 - get_current_pos(start1, curr->num) - curr->num;
                else
                    begin1 = original_length1 - CODON_LENGTH*(get_current_pos(start1, curr->num)+curr->num) + frame1 + 1;
            }
					
            if (strand2 != Seq_strand_minus) {
                if(translate2 == FALSE)
                    begin2 = get_current_pos(start2, curr->num);
                else
                    begin2 = frame2 - 1 + CODON_LENGTH*get_current_pos(start2, curr->num);
            } else {
                if(translate2 == FALSE)
                    begin2 = length2 - get_current_pos(start2, curr->num) - curr->num;
                else
                    begin2 = original_length2 - CODON_LENGTH*(get_current_pos(start2, curr->num)+curr->num) + frame2 + 1;
            }
            
            if (reverse) {
                strands[2*index] = strand2;
                strands[2*index+1] = strand1;
                start[2*index] = begin2;
                start[2*index+1] = begin1;
            } else {
                strands[2*index] = strand1;
                strands[2*index+1] = strand2;
                start[2*index] = begin1;
                start[2*index+1] = begin2;
            }
            
            break;
            
        case GAPALIGN_DEL:
            begin1 = -1;
            if (strand2 != Seq_strand_minus) {
                if(translate2 == FALSE)
                    begin2 = get_current_pos(start2, curr->num);
                else
                    begin2 = frame2 - 1 + CODON_LENGTH*get_current_pos(start2, curr->num);
            } else {
                if(translate2 == FALSE)
                    begin2 = length2 - get_current_pos(start2, curr->num) - curr->num;
                else
                    begin2 = original_length2 - CODON_LENGTH*(get_current_pos(start2, curr->num)+curr->num) + frame2 + 1;
            }
            
            if (reverse) {
                strands[2*index] = strand2;
                if (index > 0)
                    strands[2*index+1] = strands[2*(index-1)+1];
                else
                    strands[2*index+1] = Seq_strand_unknown;
                start[2*index] = begin2;
                start[2*index+1] = begin1;
            } else {
                if (index > 0)
                    strands[2*index] = strands[2*(index-1)];
                else
                    strands[2*index] = Seq_strand_unknown;
                strands[2*index+1] = strand2;
                start[2*index] = begin1;
                start[2*index+1] = begin2;
            }
            
            break;
            
        case GAPALIGN_INS:
            if (strand1 != Seq_strand_minus) {
                if(translate1 == FALSE)
                    begin1 = get_current_pos(start1, curr->num);
                else
                    begin1 = frame1 - 1 + CODON_LENGTH*get_current_pos(start1, curr->num);
            } else {
                if(translate1 == FALSE)
                    begin1 = length1 - get_current_pos(start1, curr->num) - curr->num;
                else
                    begin1 = original_length1 - CODON_LENGTH*(get_current_pos(start1, curr->num)+curr->num) + frame1 + 1;
            }
            begin2 = -1;
            if (reverse) {
                if (index > 0)
                    strands[2*index] = strands[2*(index-1)];
                else
                    strands[2*index] = Seq_strand_unknown;
                strands[2*index+1] = strand1;
                start[2*index] = begin2;
                start[2*index+1] = begin1;
            } else {
                strands[2*index] = strand1;
                if (index > 0)
                    strands[2*index+1] = strands[2*(index-1)+1];
                else
                    strands[2*index+1] = Seq_strand_unknown;
                start[2*index] = begin1;
                start[2*index+1] = begin2;
            }
            
            break;
        default:
            break;
        }
        length[index] = curr->num;
        index++;
    }    

    return TRUE;
}

static void GapCorrectUASequence(GapEditBlock* edit_block)
{
    GapEditScript* curr,* curr_last,* curr_last2;
    Boolean last_indel;

    last_indel = FALSE;
    curr_last = NULL;
    curr_last2 = NULL;

    for (curr=edit_block->esp; curr; curr = curr->next) {

        if(curr->op_type == GAPALIGN_DECLINE && last_indel == TRUE) {
            /* This is invalid condition and regions should be
               exchanged */

            if(curr_last2 != NULL)
                curr_last2->next = curr;
            else
                edit_block->esp = curr; /* First element in a list */
            
            curr_last->next = curr->next;
            curr->next = curr_last;
        }
        
        last_indel = FALSE;
        
        if(curr->op_type == GAPALIGN_INS || curr->op_type == GAPALIGN_DEL) {
            last_indel = TRUE;
            curr_last2 = curr_last;
        }

        curr_last = curr;
    }
    return;
}

static SeqAlignPtr GapMakeSeqAlign(SeqIdPtr query_id, SeqIdPtr subject_id, 
                                   Boolean reverse, Boolean translate1, 
                                   Boolean translate2, Int4 numseg,
                                   Int4* length, Int4* start, 
                                   Uint1* strands)
{
    SeqAlignPtr sap;
    DenseSeg* dsp;
    StdSeg* sseg,* sseg_head,* sseg_old;
    SeqLocPtr slp, slp1, slp2;
    SeqIntPtr seq_int1;
    Int4 index;
    Boolean tmp_value;

    sap = SeqAlignNew();
    
    sap->dim =2; /**only two dimention alignment**/
    
    /**make the Denseg Object for SeqAlign**/
    if (translate1 == FALSE && translate2 == FALSE) {
        sap->segtype = SAS_DENSEG; /** use denseg to store the alignment **/
        sap->type = SAT_PARTIAL;   /**partial for gapped translating search.*/
        dsp = DenseSegNew();
        dsp->dim = 2;
        dsp->numseg = numseg;
        if (reverse) {
            dsp->ids = SeqIdDup(subject_id);
            dsp->ids->next = SeqIdDup(query_id);
        } else {
            dsp->ids = SeqIdDup(query_id);
            dsp->ids->next = SeqIdDup(subject_id);
        }
        dsp->starts = start;
        dsp->strands = strands;
        dsp->lens = length;
        sap->segs = dsp;
        sap->next = NULL;
    } else { /****/
        sap->type = SAT_PARTIAL; /**partial for gapped translating search. */
        sap->segtype = SAS_STD;  /**use stdseg to store the alignment**/
        sseg_head = NULL;
        sseg_old = NULL;

        if(reverse) {  /* Reverting translation flags */
            tmp_value = translate1;
            translate1 = translate2;
            translate2 = tmp_value;
        }
        
        for (index=0; index<numseg; index++) {
            sseg = StdSegNew();
            sseg->dim = 2;
            if (sseg_head == NULL) {
                sseg_head = sseg;
            }
            if (reverse) {
                sseg->ids = SeqIdDup(subject_id);
                sseg->ids->next = SeqIdDup(query_id);
            } else {
                sseg->ids = SeqIdDup(query_id);
                sseg->ids->next = SeqIdDup(subject_id);
            }

            slp1 = NULL;
            if (start[2*index] != -1) {
                seq_int1 = SeqIntNew();
                seq_int1->from = start[2*index];
                if (translate1)
                    seq_int1->to = start[2*index] + CODON_LENGTH*length[index] - 1;
                else
                    seq_int1->to = start[2*index] + length[index] - 1;
                seq_int1->strand = strands[2*index];

                if(reverse) { 
                    seq_int1->id = SeqIdDup(subject_id);
                } else {
                    seq_int1->id = SeqIdDup(query_id);
                }

                ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
            } else {
                if(reverse) { 
                    ValNodeAddPointer(&slp1, SEQLOC_EMPTY, SeqIdDup(subject_id));
                } else {
                    ValNodeAddPointer(&slp1, SEQLOC_EMPTY, SeqIdDup(query_id));
                }
            }
            slp2 = NULL;
            if (start[2*index+1] != -1) {
                seq_int1 = SeqIntNew();
                seq_int1->from = start[2*index+1];
                if (translate2)
                    seq_int1->to = start[2*index+1] + CODON_LENGTH*length[index] - 1;
                else
                    seq_int1->to = start[2*index+1] + length[index] - 1;
                seq_int1->strand = strands[2*index+1];

                if(reverse) { 
                    seq_int1->id = SeqIdDup(query_id);
                } else {
                    seq_int1->id = SeqIdDup(subject_id);
                }

                ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int1);
            } else {
                if(reverse) { 
                    ValNodeAddPointer(&slp2, SEQLOC_EMPTY, SeqIdDup(query_id));
                } else {
                    ValNodeAddPointer(&slp2, SEQLOC_EMPTY, SeqIdDup(subject_id));
                }
            }
            /*
              if (reverse) {
              slp = slp2;
              slp2->next = slp1;
              } else {
              slp = slp1;
              slp1->next = slp2;
              } */

            slp = slp1;
            slp1->next = slp2;

            sseg->loc = slp;
            
            if (sseg_old)
                sseg_old->next = sseg;
            sseg_old = sseg;
        }
        sap->segs = sseg_head;
        sap->next = NULL;
        
        sfree(start);
        sfree(length);
        sfree(strands);
    }

    return sap;
}

/** Convert an EditScript chain to a SeqAlign of type DenseSeg.
 * Used for a non-simple interval (i.e., one without subs. or 
 * deletions).  
 * 
 * The first SeqIdPtr in the chain of subject_id and query_id is duplicated for
 * the SeqAlign.
 */
SeqAlignPtr LIBCALL
GapEditBlockToSeqAlign(GapEditBlock* edit_block, SeqIdPtr subject_id, SeqIdPtr query_id)

{
    GapEditScript* curr,* curr_last;
    Int4 numseg, start1, start2;
    Int4* length,* start;
    Uint1* strands;
    Boolean is_disc_align = FALSE;
    SeqAlignPtr sap, sap_disc, sap_head, sap_tail;
    Boolean skip_region = FALSE;

    is_disc_align = FALSE; numseg = 0;

    for (curr=edit_block->esp; curr; curr = curr->next) {
        numseg++;
        if(/*edit_block->discontinuous && */curr->op_type == GAPALIGN_DECLINE)
           is_disc_align = TRUE;
    }
    
    start1 = edit_block->start1;
    start2 = edit_block->start2;
    
    /* If no GAPALIGN_DECLINE regions exists output seqalign will be
       regular Den-Seg or Std-seg */
    if(is_disc_align == FALSE) {
        /* Please note, that edit_block passed only for data like
           strand, translate, reverse etc. Real data is taken starting
           from "curr" and taken only "numseg" segments */
        
        GapCollectDataForSeqalign(edit_block, edit_block->esp, numseg,
                                  &start, &length, &strands, &start1, &start2);
        
        /* Result of this function will be either den-seg or Std-seg
           depending on translation options */
        sap = GapMakeSeqAlign(query_id, subject_id, edit_block->reverse, 
                              edit_block->translate1, edit_block->translate2, 
                              numseg, length, start, strands);
    } else {

        /* By request of Steven Altschul - we need to have 
           the unaligned part being to the left if it is adjacent to the
           gap (insertion or deletion) - so this function will do
           shaffeling */

        GapCorrectUASequence(edit_block); 

        sap_disc = SeqAlignNew();
        sap_disc->dim = 2;
        sap_disc->type = SAT_PARTIAL; /* ordered segments, over part of seq */
        sap_disc->segtype = SAS_DISC; /* discontinuous alignment */
        
        curr_last = edit_block->esp; 
        sap_head = NULL; sap_tail = NULL;
        while(curr_last != NULL) {
            skip_region = FALSE;                        
            for (numseg = 0, curr = curr_last; 
                 curr; curr = curr->next, numseg++) {

                if(curr->op_type == GAPALIGN_DECLINE) {
                    if(numseg != 0) { /* End of aligned area */
                        break;
                    } else {
                        while(curr && curr->op_type == GAPALIGN_DECLINE) {
                            numseg++;
                            curr = curr->next; 
                        }
                        skip_region = TRUE;                        
                        break;
                    }
                }
            }

            GapCollectDataForSeqalign(edit_block, curr_last, numseg,
                                      &start, &length, &strands, 
                                      &start1, &start2);
            
            if(!skip_region) {            
                sap = GapMakeSeqAlign(query_id, subject_id, 
                                      edit_block->reverse, 
                                      edit_block->translate1, 
                                      edit_block->translate2,
                                      numseg, length, start, strands);
                
                /* Collecting all seqaligns into single linked list */
                if(sap_tail == NULL) {
                    sap_head = sap_tail = sap;
                } else {
                    sap_tail->next = sap;
                    sap_tail = sap;
                }
            }
            
            curr_last = curr;
        }
        sap_disc->segs = sap_head;
        sap = sap_disc;
    }

    return sap;
}

/** This function is used for Out-Of-Frame traceback conversion
 * Convert an OOF EditScript chain to a SeqAlign of type StdSeg.
 * Used for a non-simple interval (i.e., one without subs. or 
 * deletions).  
 * The first SeqIdPtr in the chain of subject_id and query_id is 
 * duplicated for the SeqAlign.
*/
static SeqAlignPtr LIBCALL
BLAST_OOFEditBlockToSeqAlign(Uint1 program, GapEditBlock* edit_block, 
   SeqIdPtr query_id, SeqIdPtr subject_id)
{
    Boolean reverse = FALSE;
    GapEditScript* curr,* esp;
    Int2 frame1, frame2;
    Int4 start1, start2;
    Int4 original_length1, original_length2;
    SeqAlignPtr sap;
    SeqIntPtr seq_int1, seq_int2;
    SeqIntPtr seq_int1_last = NULL, seq_int2_last = NULL;
    SeqIdPtr sip, id1, id2;
    SeqLocPtr slp, slp1, slp2;
    StdSeg* sseg,* sseg_head,* sseg_old;
    Uint1 strand1, strand2;
    Boolean first_shift;

    if (program == blast_type_blastx) {
       reverse = TRUE;
       start1 = edit_block->start2;
       start2 = edit_block->start1;
       frame1 = edit_block->frame2;
       frame2 = edit_block->frame1;
       original_length1 = edit_block->original_length2;
       original_length2 = edit_block->original_length1;
       id1 = subject_id;
       id2 = query_id;
    } else { 
       start1 = edit_block->start1;
       start2 = edit_block->start2;
       frame1 = edit_block->frame1;
       frame2 = edit_block->frame2;
       original_length1 = edit_block->original_length1;
       original_length2 = edit_block->original_length2;
       id1 = query_id;
       id2 = subject_id;
    }
 
    if(frame1 > 0) 
        strand1 = Seq_strand_plus;
    else if (frame1 < 0)
        strand1 = Seq_strand_minus;
    else
        strand1 = Seq_strand_unknown;
    
    if(frame2 > 0) 
        strand2 = Seq_strand_plus;
    else if (frame2 < 0)
        strand2 = Seq_strand_minus;
    else
        strand2 = Seq_strand_unknown;
    
    esp = edit_block->esp;
    
    sap = SeqAlignNew();
    
    sap->dim =2; /**only two dimention alignment**/
    
    sap->type =3; /**partial for gapped translating search. */
    sap->segtype =3; /**use denseg to store the alignment**/
    sseg_head = NULL;
    sseg_old = NULL;
    esp = edit_block->esp;

    first_shift = FALSE;

    for (curr=esp; curr; curr=curr->next) {

        slp1 = NULL;
        slp2 = NULL;
        
        switch (curr->op_type) {
        case 0: /* deletion of three nucleotides. */
            
            first_shift = FALSE;

            seq_int1 = SeqIntNew();
            seq_int1->from = get_current_pos(&start1, curr->num);
            seq_int1->to = start1 - 1;            

            if(seq_int1->to >= original_length1)
                seq_int1->to = original_length1-1;
            
            seq_int1->id = SeqIdDup(id1);
            seq_int1->strand = strand1;

            ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);

            /* Empty nucleotide piece */
            ValNodeAddPointer(&slp2, SEQLOC_EMPTY, SeqIdDup(id2));
            
            seq_int1_last = seq_int1;
            /* Keep previous seq_int2_last, in case there is a frame shift
               immediately after this gap */
            
            break;

        case 6: /* insertion of three nucleotides. */

            /* If gap is followed after frameshift - we have to
               add this element for the alignment to be correct */
            
            if(first_shift == TRUE) { /* Second frameshift in a row */
                /* Protein coordinates */
                seq_int1 = SeqIntNew();
                seq_int1->from =  get_current_pos(&start1, 1);
                seq_int1->to = start1 - 1;

                if(seq_int1->to >= original_length1)
                    seq_int1->to = original_length1-1;
                
                seq_int1->id = SeqIdDup(id1);
                seq_int1->strand = strand1;
                
                ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
                
                /* Nucleotide scale shifted by op_type */
                seq_int2 = SeqIntNew();

                seq_int2->from = get_current_pos(&start2, 3);
                seq_int2->to = start2 - 1;

                if(seq_int2->to >= original_length2) {
                    seq_int2->to = original_length2 - 1;
                    seq_int1->to--;
                }

                /* Transfer to DNA minus strand coordinates */
                if(strand2 == Seq_strand_minus) {
                    int tmp_int;
                    tmp_int = seq_int2->to;
                    seq_int2->to = original_length2 - seq_int2->from - 1;
                    seq_int2->from = original_length2 - tmp_int - 1;
                }
            
                seq_int2->id = SeqIdDup(id2);
                seq_int2->strand = strand2;
                
                ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int2);

                /* seq_int1_last = seq_int1; 
                   seq_int2_last = seq_int2; */

                /* first_shift = FALSE; */

                if (reverse) {
                    slp = slp2;
                    slp2->next = slp1;
                    sip = SeqIdDup(id2);
                    sip->next = SeqIdDup(id1);
                } else {
                    slp = slp1;
                    slp1->next = slp2;
                    sip = SeqIdDup(id1);
                    sip->next = SeqIdDup(id2);
                }
                
                sseg = StdSegNew();
                sseg->dim = 2;
                
                if (sseg_head == NULL)
                    sseg_head = sseg;
                
                sseg->loc = slp;
                sseg->ids = sip;
                
                if (sseg_old)
                    sseg_old->next = sseg;
                
                sseg_old = sseg;

                slp1 = NULL;
                slp2 = NULL;
            }

            first_shift = FALSE;

            /* Protein piece is empty */
            ValNodeAddPointer(&slp1, SEQLOC_EMPTY, SeqIdDup(id1));
            
            /* Nucleotide scale shifted by 3, protein gapped */
            seq_int2 = SeqIntNew();              
            seq_int2->from = get_current_pos(&start2, curr->num*3);
            seq_int2->to = start2 - 1;

            if(seq_int2->to >= original_length2) {
                seq_int2->to = original_length2 -1;
            }

            /* Transfer to DNA minus strand coordinates */
            if(strand2 == Seq_strand_minus) {
                int tmp_int;
                tmp_int = seq_int2->to;
                seq_int2->to = original_length2 - seq_int2->from - 1;
                seq_int2->from = original_length2 - tmp_int - 1;
            }

            seq_int2->id = SeqIdDup(id2);
            seq_int2->strand = strand2;
            
            ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int2);
            
            seq_int1_last = NULL;
            seq_int2_last = seq_int2; /* Will be used to adjust "to" value */
            
            break;

        case 3: /* Substitution. */

            first_shift = FALSE;

            /* Protein coordinates */
            seq_int1 = SeqIntNew();
            seq_int1->from =  get_current_pos(&start1, curr->num);
            seq_int1->to = start1 - 1;

            if(seq_int1->to >= original_length1)
                seq_int1->to = original_length1-1;
            
            seq_int1->id = SeqIdDup(id1);
            seq_int1->strand = strand1;

            ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
           
            /* Nucleotide scale shifted by op_type */
            seq_int2 = SeqIntNew();

            /* Adjusting last segment and new start point in
               nucleotide coordinates */
            /* if(seq_int2_last != NULL) {
               seq_int2_last->to = seq_int2_last->to - (3 - curr->op_type);
               start2 = start2 - (3 - curr->op_type);
               } */
            
            seq_int2->from = get_current_pos(&start2, curr->num*curr->op_type);
            seq_int2->to = start2 - 1;

		/* Chop off three bases and one residue at a time.
			Why does this happen, seems like a bug?
		*/
            while (seq_int2->to >= original_length2) {
                seq_int2->to -= 3;
                seq_int1->to--;
            }

            /* Transfer to DNA minus strand coordinates */
            if(strand2 == Seq_strand_minus) {
                int tmp_int;
                tmp_int = seq_int2->to;
                seq_int2->to = original_length2 - seq_int2->from - 1;
                seq_int2->from = original_length2 - tmp_int - 1;
            }
            
            seq_int2->id = SeqIdDup(id2);
            seq_int2->strand = strand2;

            ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int2);

            seq_int1_last = seq_int1; /* Will be used to adjust "to" value */
            seq_int2_last = seq_int2; /* Will be used to adjust "to" value */
            
            break;
        case 1:	/* gap of two nucleotides. */
        case 2: /* Gap of one nucleotide. */
        case 4: /* Insertion of one nucleotide. */
        case 5: /* Insertion of two nucleotides. */

            if(first_shift == TRUE) { /* Second frameshift in a row */
                /* Protein coordinates */
                seq_int1 = SeqIntNew();
                seq_int1->from =  get_current_pos(&start1, 1);
                seq_int1->to = start1 - 1;

                if(seq_int1->to >= original_length1)
                    seq_int1->to = original_length1-1;
                
                seq_int1->id = SeqIdDup(id1);
                seq_int1->strand = strand1;
                
                ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
                
                /* Nucleotide scale shifted by op_type */
                seq_int2 = SeqIntNew();

                seq_int2->from = get_current_pos(&start2, curr->op_type);
                seq_int2->to = start2 - 1;

                if(seq_int2->to >= original_length2) {
                    seq_int2->to = original_length2 -1;
                    seq_int1->to--;
                }

                /* Transfer to DNA minus strand coordinates */
                if(strand2 == Seq_strand_minus) {
                    int tmp_int;
                    tmp_int = seq_int2->to;
                    seq_int2->to = original_length2 - seq_int2->from - 1;
                    seq_int2->from = original_length2 - tmp_int - 1;
                }
            
                seq_int2->id = SeqIdDup(id2);
                seq_int2->strand = strand2;
                
                ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int2);

                seq_int1_last = seq_int1; 
                seq_int2_last = seq_int2; 

                /* first_shift = FALSE; */

                break;
            }
            
            first_shift = TRUE;

            /* If this substitution is following simple frameshift
               we do not need to start new segment, but may continue
               old one */
            /* printf("curr_num = %d (%d)\n",  curr->num, curr->op_type); */

            if(seq_int2_last != NULL) {
                get_current_pos(&start2, curr->num*(curr->op_type-3));
                if(strand2 != Seq_strand_minus) {
                    seq_int2_last->to = start2 - 1;
                } else {
                    /* Transfer to DNA minus strand coordinates */
                    seq_int2_last->from = original_length2 - start2;
                }

                /* Adjustment for multiple shifts - theoretically possible,
                   but very unprobable */
                if(seq_int2_last->from > seq_int2_last->to) {
                    
                    if(strand2 != Seq_strand_minus) {
                        seq_int2_last->to += 3;
                    } else {
                        seq_int2_last->from -= 3;
                    }
                    
                    if(seq_int1_last != 0)
                        seq_int1_last++;
                }

            } else if (curr->op_type > 3) {
                /* Protein piece is empty */
                ValNodeAddPointer(&slp1, SEQLOC_EMPTY, SeqIdDup(id1));
                /* Simulating insertion of nucleotides */
                seq_int2 = SeqIntNew();
                seq_int2->from = get_current_pos(&start2, 
                                                 curr->num*(curr->op_type-3));
                seq_int2->to = start2 - 1;
                
                if(seq_int2->to >= original_length2) {
                    seq_int2->to = original_length2 - 1;
                }

                /* Transfer to DNA minus strand coordinates */
                if(strand2 == Seq_strand_minus) {
                    int tmp_int;
                    tmp_int = seq_int2->to;
                    seq_int2->to = original_length2 - seq_int2->from - 1;
                    seq_int2->from = original_length2 - tmp_int - 1;
                }

                seq_int2->id = SeqIdDup(id2);
                seq_int2->strand = strand2;
                
                ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int2);
                
                seq_int1_last = NULL;
                seq_int2_last = seq_int2; /* Will be used to adjust "to" value */
                break;
            } else {
                continue;       /* Main loop */
            }
            continue;       /* Main loop */
            /* break; */
        default:
            continue;       /* Main loop */
            /* break; */
        } 

        if (reverse) {
            slp = slp2;
            slp2->next = slp1;
            sip = SeqIdDup(id2);
            sip->next = SeqIdDup(id1);
        } else {
            slp = slp1;
            slp1->next = slp2;
            sip = SeqIdDup(id1);
            sip->next = SeqIdDup(id2);
        }

        sseg = StdSegNew();
        sseg->dim = 2;
        
        if (sseg_head == NULL)
            sseg_head = sseg;
        
        sseg->loc = slp;
        sseg->ids = sip;
        
        if (sseg_old)
            sseg_old->next = sseg;

        sseg_old = sseg;
    }
    sap->segs = sseg_head;
    sap->next = NULL;
    
    return sap;
}

static Int2 
BLAST_GapInfoToSeqAlign(Uint1 program_number, BlastHSPList* hsp_list, 
   SeqIdPtr query_id, SeqIdPtr subject_id, BlastScoreBlk* sbp, 
   Int4 query_length, BlastScoringOptions* score_options, 
   SeqAlignPtr* head_seqalign)
{
   Int2 status = 0;
   BlastHSP** hsp_array;
   SeqAlignPtr last_seqalign = NULL, seqalign = NULL;
   Int4 index;

   *head_seqalign = NULL;

   hsp_array = hsp_list->hsp_array;

   for (index=0; index<hsp_list->hspcnt; index++) { 
      hsp_array[index]->gap_info->original_length1 = query_length;
      if (score_options->is_ooframe) {
         seqalign = BLAST_OOFEditBlockToSeqAlign(program_number, 
                       hsp_array[index]->gap_info, 
                       query_id, subject_id);
      } else {
         /* The following line is needed for negative frames of translated 
            query */
         seqalign = GapEditBlockToSeqAlign(hsp_array[index]->gap_info, 
                                            subject_id, query_id);
      }
      if (index==0) {
         *head_seqalign = last_seqalign = seqalign;
      } else {
         last_seqalign->next = seqalign;
         last_seqalign = last_seqalign->next;
      }
      seqalign->score = 
         GetScoreSetFromBlastHsp(program_number, hsp_array[index], sbp, 
                                 score_options);
   }

   return status;
}

Int2 BLAST_ResultsToSeqAlign(Uint1 program_number, 
        BlastHSPResults* results, SeqLocPtr query_slp, 
        BlastSeqSrc* bssp, SeqLocPtr subject_slp,
        BlastScoringOptions* score_options, BlastScoreBlk* sbp,
        Boolean is_gapped, SeqAlignPtr* head_seqalign)
{
   Int4 query_index, subject_index;
   SeqLocPtr slp = query_slp;
   SeqIdPtr query_id, subject_id = NULL;
   BlastHitList* hit_list;
   BlastHSPList* hsp_list;
   SeqAlignPtr seqalign = NULL, last_seqalign = NULL;
   Int4 subject_length = 0;
   
   *head_seqalign = NULL;

   if (!bssp) {
      subject_id = SeqLocId(subject_slp);
      subject_length = SeqLocLen(subject_slp);
   }

   for (query_index = 0; slp && query_index < results->num_queries; 
        ++query_index, slp = slp->next) {
      hit_list = results->hitlist_array[query_index];
      if (!hit_list)
         continue;
      query_id = SeqLocId(slp);

      for (subject_index = 0; subject_index < hit_list->hsplist_count;
           ++subject_index) {
         hsp_list = hit_list->hsplist_array[subject_index];
         if (!hsp_list)
            continue;
         if (bssp) {
            char* id_str = BLASTSeqSrcGetSeqIdStr(bssp, (void*) &hsp_list->oid);
            subject_id = SeqIdParse(id_str);
            subject_length = BLASTSeqSrcGetSeqLen(bssp, (void*) &hsp_list->oid); 
            sfree(id_str);
         }
         if (is_gapped) {
            BLAST_GapInfoToSeqAlign(program_number, hsp_list, query_id, 
               subject_id, sbp, SeqLocLen(slp), score_options, &seqalign);
         } else {
            BLAST_UngappedHSPToSeqAlign(program_number, hsp_list, query_id,
               subject_id, sbp, SeqLocLen(slp), subject_length, 
               score_options, &seqalign);
         }                      

         if (bssp)
            SeqIdSetFree(subject_id);

         if (seqalign) {
            if (!last_seqalign) {
               *head_seqalign = last_seqalign = seqalign;
            } else {
               last_seqalign->next = seqalign;
            }
            for ( ; last_seqalign->next; 
                  last_seqalign = last_seqalign->next);
         }
      }
   }

   return 0;
}


