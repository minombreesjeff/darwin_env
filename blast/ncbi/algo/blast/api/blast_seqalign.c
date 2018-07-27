/* $Id: blast_seqalign.c,v 1.52 2005/04/06 23:27:53 dondosha Exp $
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
*  Author: Ilya Dondoshansky
* ===========================================================================*/

/** @file blast_seqalign.c
 * Conversion of BLAST results to the SeqAlign form
 */

static char const rcsid[] = "$Id: blast_seqalign.c,v 1.52 2005/04/06 23:27:53 dondosha Exp $";

#include <algo/blast/api/blast_seqalign.h>

extern Int4 LIBCALL HspArrayPurge PROTO((BlastHSP** hsp_array, 
                       Int4 hspcnt, Boolean clear_num));
extern SeqIdPtr GetTheSeqAlignID (SeqIdPtr seq_id);
extern ScorePtr MakeBlastScore (ScorePtr PNTR old, CharPtr scoretype, 
                                Nlm_FloatHi prob, Int4 score);

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Creates a score set corresponding to one HSP.
 * @param hsp HSP structure [in]
 * @return Score set for this HSP.
 */
static ScorePtr 
s_GetScoreSetFromBlastHsp(BlastHSP* hsp)
{
   ScorePtr	score_set=NULL;
   double	prob;
   Int4		score;
   char*	scoretype;

   score = hsp->score;
   if (score > 0)
      MakeBlastScore(&score_set, "score", 0.0, score);

   score = hsp->num;
   scoretype = "sum_n";
   
   if (score > 1)
      MakeBlastScore(&score_set, scoretype, 0.0, score);
   
   prob = hsp->evalue;
   if (hsp->num <= 1) {
      scoretype = "e_value";
   } else {
      scoretype = "sum_e";
   }
   if (prob >= 0.) {
      if (prob < 1.0e-180)
         prob = 0.0;
      MakeBlastScore(&score_set, scoretype, prob, 0);
   }

   /* Calculate bit score from the raw score */
   if (hsp->bit_score >= 0.)
      MakeBlastScore(&score_set, "bit_score", hsp->bit_score, 0);
   
   if (hsp->num_ident > 0)
      MakeBlastScore(&score_set, "num_ident", 0.0, hsp->num_ident);
   
   return score_set;
}

/** Fills in the DenseDiag information from a BlastHSP structure.
 * On the first call to this function *old should be
 * NULL, after that pass in the head of the DenseDiagPtr chain.
 * @param old Pointer to previously created DenseDiag [in]
 * @param hsp HSP structure to get alignment information from [in]
 * @param reverse Should query and subject sequences be reversed? [in]
 * @param query_length Length of the query sequence [in]
 * @param subject_length Length of the subject sequence [in]
 * @return The appended DenseDiagPtr
 */
static DenseDiagPtr
s_HSPToDenseDiag(DenseDiagPtr* old, BlastHSP* hsp, Boolean reverse,
                 Int4 query_length, Int4 subject_length)
{
	DenseDiagPtr		ddp, new;

	new = DenseDiagNew();
	
	new->dim = 2;	/* Only 2 is supported in spec. */
	new->len = hsp->query.end - hsp->query.offset;
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
			new->starts[0] = subject_length - hsp->subject.end;
		}
		if (hsp->query.frame >= 0)
		{
			new->strands[1] = Seq_strand_plus;
			new->starts[1] = hsp->query.offset;
		}
		else
		{
			new->strands[1] = Seq_strand_minus;
			new->starts[1] = query_length - hsp->query.end;
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
			new->starts[0] = query_length - hsp->query.end;
		}
		if (hsp->subject.frame >= 0)
		{
			new->strands[1] = Seq_strand_plus;
			new->starts[1] = hsp->subject.offset;
		}
		else
		{
			new->strands[1] = Seq_strand_minus;
			new->starts[1] = subject_length - hsp->subject.end;
		}
	}
	new->scores = s_GetScoreSetFromBlastHsp(hsp);

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

/** Fills in the StdSeg information from a BlastHSP structure.
 * On the first call to this function *old should be
 * NULL, after that pass in the head of the StdSegPtr chain.
 * @param old Pointer to previously created StdSeg [in]
 * @param hsp HSP structure to get alignment information from [in]
 * @param reverse Should query and subject sequences be reversed? [in]
 * @param query_length Length of the query sequence [in]
 * @param subject_length Length of the subject sequence [in]
 * @param sip Query sequence id [in]
 * @return The appended StdSegPtr
 */
static StdSeg*
s_HSPToStdSeg(StdSeg** old, BlastHSP* hsp, Int4 query_length, 
              Int4 subject_length, SeqIdPtr sip, Boolean reverse)
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
		seq_int1->to = hsp->query.end - 1;
		seq_int1->strand = Seq_strand_unknown;
	}
	else if (hsp->query.frame < 0)
	{
		seq_int1->to = 
            query_length - CODON_LENGTH*hsp->query.offset + hsp->query.frame;
		seq_int1->from = 
            query_length - CODON_LENGTH*(hsp->query.end) + hsp->query.frame + 1;
		seq_int1->strand = Seq_strand_minus;
	}
	else if (hsp->query.frame > 0)
	{
		seq_int1->from = CODON_LENGTH*(hsp->query.offset) + hsp->query.frame - 1;
		seq_int1->to = CODON_LENGTH*(hsp->query.end) + hsp->query.frame - 2;
		seq_int1->strand = Seq_strand_plus;
	}
	seq_int1->id = query_sip;
	seq_int2 = SeqIntNew();
	if (hsp->subject.frame == 0)
	{
		seq_int2->from = hsp->subject.offset;
		seq_int2->to = hsp->subject.end - 1;
		seq_int2->strand = Seq_strand_unknown;
	} 
	else if (hsp->subject.frame < 0)
	{
		seq_int2->from = subject_length - CODON_LENGTH*(hsp->subject.end) + 
            hsp->subject.frame + 1;
		seq_int2->to = subject_length - CODON_LENGTH*(hsp->subject.offset) +
            hsp->subject.frame;
		seq_int2->strand = Seq_strand_minus;
	}
	else if (hsp->subject.frame > 0)
	{
		seq_int2->from = 
            CODON_LENGTH*(hsp->subject.offset) + hsp->subject.frame - 1;
		seq_int2->to = CODON_LENGTH*(hsp->subject.end) + hsp->subject.frame - 2;
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

	new->scores = s_GetScoreSetFromBlastHsp(hsp);

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

/** Assembles all the components of the Seq-align from a BlastHSPList structure
 * for an ungapped alignment search.  
 * @param program_number Type of BLAST program.
 * @param hsp_list HSP list structure [in]
 * @param query_id Seq-id of the query sequence [in]
 * @param subject_id Seq-id of the subject sequence [in]
 * @param query_length Length of the query sequence [in]
 * @param subject_length Length of the subject sequence [in]
 * @param seqalign_ptr Seq-align chain to append to. [in] [out]
 */
static Int2 
s_HSPListToSeqAlignUngapped(EBlastProgramType program_number, 
   BlastHSPList* hsp_list, SeqIdPtr query_id, 
   SeqIdPtr subject_id, Int4 query_length,
   Int4 subject_length, SeqAlignPtr* seqalign_ptr)
{
   BlastHSP* hsp;
   DenseDiagPtr	ddp_head=NULL, ddp;
   SeqIdPtr sip;
   SeqIdPtr new_sip=NULL;
   StdSeg* ssp_head=NULL,* ssp;
   SeqAlignPtr seqalign;
   Int4 hsp_cnt, index2, hspset_cnt_old;
   Boolean getdensediag = 
      (program_number == eBlastTypeBlastn ||
       program_number == eBlastTypeRpsBlast ||
       program_number == eBlastTypeBlastp);

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
         ddp = s_HSPToDenseDiag(&ddp_head, hsp, FALSE, query_length, 
                                subject_length);
         ddp->id = sip;
      } else {
         ssp = s_HSPToStdSeg(&ssp_head, hsp, query_length, subject_length, sip,
                             FALSE);
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

/** Get the current position.
 * @param pos Current position. On return, assigned next position. [in] [out]
 *            @todo FIXME: Can it really be negative? If not, then this function
 *             can be simplified or even gotten rid of.
 * @param length Length of the next segment. [in]
 * @return Next position
 */
static Int4 
s_GetCurrentPos(Int4* pos, Int4 length)
{
    Int4 val;
    if(*pos < 0)
        val = -(*pos + length -1);
    else
        val = *pos;
    *pos += length;
    return val;
}

/** Finds a protein frame length.
 * @param nuc_length Length of nucleotide sequence. [in]
 * @param frame Frame number. [in]
 * @return Length of corresponding protein frame.
 */
static Int4
s_GetProteinFrameLength(Int4 nuc_length, Int2 frame)
{
    return (nuc_length - (ABS(frame)-1)%CODON_LENGTH) / CODON_LENGTH;
}

Int2 
GapCollectDataForSeqalign(BlastHSP* hsp, GapEditScript* curr_in, Int4 numseg,
                          Int4 query_length, Int4 subject_length,
                          Boolean translate1, Boolean translate2,
                          Int4** start_out, Int4** length_out, 
                          Uint1** strands_out, Int4* start1, Int4* start2)
{
    GapEditScript* curr;
    Int2 frame1, frame2;
    Int4 begin1, begin2, index, length1, length2;
    Int4 original_length1, original_length2, i;
    Int4* length,* start;
    Uint1 strand1, strand2;
    Uint1* strands;
    
    length1 = hsp->query.end - hsp->query.offset;
    length2 = hsp->subject.end - hsp->subject.offset;
    original_length1 = length1 = query_length;
    original_length2 = length2 = subject_length;
    frame1 = hsp->query.frame;
    frame2 = hsp->subject.frame;
    
    if (translate1)
        length1 = s_GetProteinFrameLength(length1, frame1);
    if (translate2)
        length2 = s_GetProteinFrameLength(length2, frame2);
    
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
    length = (Int4 *) calloc((numseg+1), sizeof(Int4));
    strands = (Uint1 *) calloc((2*numseg+1), sizeof(Uint1));

    index=0;
    for (i = 0, curr=curr_in; curr && i < numseg; curr=curr->next, i++) {
        switch(curr->op_type) {
        case eGapAlignDecline:
        case eGapAlignSub:
            if (strand1 != Seq_strand_minus) {
                if(translate1 == FALSE)
                    begin1 = s_GetCurrentPos(start1, curr->num);
                else
                    begin1 = frame1 - 1 + CODON_LENGTH*s_GetCurrentPos(start1, curr->num);
            } else {
                if(translate1 == FALSE)
                    begin1 = length1 - s_GetCurrentPos(start1, curr->num) - curr->num;
                else
                    begin1 = original_length1 - CODON_LENGTH*(s_GetCurrentPos(start1, curr->num)+curr->num) + frame1 + 1;
            }
					
            if (strand2 != Seq_strand_minus) {
                if(translate2 == FALSE)
                    begin2 = s_GetCurrentPos(start2, curr->num);
                else
                    begin2 = frame2 - 1 + CODON_LENGTH*s_GetCurrentPos(start2, curr->num);
            } else {
                if(translate2 == FALSE)
                    begin2 = length2 - s_GetCurrentPos(start2, curr->num) - curr->num;
                else
                    begin2 = original_length2 - CODON_LENGTH*(s_GetCurrentPos(start2, curr->num)+curr->num) + frame2 + 1;
            }
            
            strands[2*index] = strand1;
            strands[2*index+1] = strand2;
            start[2*index] = begin1;
            start[2*index+1] = begin2;
            
            break;
            
        case eGapAlignDel:
            begin1 = -1;
            if (strand2 != Seq_strand_minus) {
                if(translate2 == FALSE)
                    begin2 = s_GetCurrentPos(start2, curr->num);
                else
                    begin2 = frame2 - 1 + CODON_LENGTH*s_GetCurrentPos(start2, curr->num);
            } else {
                if(translate2 == FALSE)
                    begin2 = length2 - s_GetCurrentPos(start2, curr->num) - curr->num;
                else
                    begin2 = original_length2 - CODON_LENGTH*(s_GetCurrentPos(start2, curr->num)+curr->num) + frame2 + 1;
            }
            
            if (index > 0)
                strands[2*index] = strands[2*(index-1)];
            else
                strands[2*index] = Seq_strand_unknown;
            strands[2*index+1] = strand2;
            start[2*index] = begin1;
            start[2*index+1] = begin2;
            
            break;
            
        case eGapAlignIns:
            if (strand1 != Seq_strand_minus) {
                if(translate1 == FALSE)
                    begin1 = s_GetCurrentPos(start1, curr->num);
                else
                    begin1 = frame1 - 1 + CODON_LENGTH*s_GetCurrentPos(start1, curr->num);
            } else {
                if(translate1 == FALSE)
                    begin1 = length1 - s_GetCurrentPos(start1, curr->num) - curr->num;
                else
                    begin1 = original_length1 - CODON_LENGTH*(s_GetCurrentPos(start1, curr->num)+curr->num) + frame1 + 1;
            }
            begin2 = -1;
            strands[2*index] = strand1;
            if (index > 0)
                strands[2*index+1] = strands[2*(index-1)+1];
            else
                strands[2*index+1] = Seq_strand_unknown;
            start[2*index] = begin1;
            start[2*index+1] = begin2;
            
            break;
        default:
            break;
        }
        length[index] = curr->num;
        index++;
    }    

    if (start_out)
       *start_out = start;
    else
       sfree(start);
    if (length_out)
       *length_out = length;
    else
       sfree(length);
    if (strands_out)
       *strands_out = strands;
    else
       sfree(strands);

    return 0;
}

/** Corrects an editing script if any decline-to-align segments are present.
 * @param hsp HSP structure, containing the editing script. [in] [out]
 */
static void 
s_GapCorrectUASequence(BlastHSP* hsp)
{
    GapEditScript* curr,* curr_last,* curr_last2;
    Boolean last_indel;

    last_indel = FALSE;
    curr_last = NULL;
    curr_last2 = NULL;

    for (curr=hsp->gap_info; curr; curr = curr->next) {

        if(curr->op_type == eGapAlignDecline && last_indel == TRUE) {
            /* This is invalid condition and regions should be
               exchanged */

            if(curr_last2 != NULL)
                curr_last2->next = curr;
            else
                hsp->gap_info = curr; /* First element in a list */
            
            curr_last->next = curr->next;
            curr->next = curr_last;
        }
        
        last_indel = FALSE;
        
        if(curr->op_type == eGapAlignIns || curr->op_type == eGapAlignDel) {
            last_indel = TRUE;
            curr_last2 = curr_last;
        }

        curr_last = curr;
    }
    return;
}

/** Creates and fills a SeqAlign structure, given all necessary information,
 * with either Dense-seg or Std-seg segments.
 * @param query_id Seq-id of the query sequence. [in]
 * @param subject_id Seq-id of the subject sequence [in]
 * @param translate1 Is query translated? [in]
 * @param translate2 Is subject translated? [in]
 * @param numseg Number of segments in the alignment [in]
 * @param length Array of segment lengths [in]
 * @param start Array of segment starts [in]
 * @param strands Array of segment strands [in]
 * @return Resulting Seq-align.
 */
static SeqAlignPtr 
s_GapMakeSeqAlign(SeqIdPtr query_id, SeqIdPtr subject_id, 
                  Boolean translate1, Boolean translate2, Int4 numseg,
                  Int4* length, Int4* start, Uint1* strands)
{
    SeqAlignPtr sap;
    DenseSeg* dsp;
    StdSeg* sseg,* sseg_head,* sseg_old;
    SeqLocPtr slp, slp1, slp2;
    SeqIntPtr seq_int1;
    Int4 index;

    sap = SeqAlignNew();
    
    sap->dim =2; /**only two dimention alignment**/
    
    /**make the Denseg Object for SeqAlign**/
    if (translate1 == FALSE && translate2 == FALSE) {
        sap->segtype = SAS_DENSEG; /** use denseg to store the alignment **/
        sap->type = SAT_PARTIAL;   /**partial for gapped translating search.*/
        dsp = DenseSegNew();
        dsp->dim = 2;
        dsp->numseg = numseg;
        dsp->ids = SeqIdDup(query_id);
        dsp->ids->next = SeqIdDup(subject_id);
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

        for (index=0; index<numseg; index++) {
            sseg = StdSegNew();
            sseg->dim = 2;
            if (sseg_head == NULL) {
                sseg_head = sseg;
            }
            sseg->ids = SeqIdDup(query_id);
            sseg->ids->next = SeqIdDup(subject_id);

            slp1 = NULL;
            if (start[2*index] != -1) {
                seq_int1 = SeqIntNew();
                seq_int1->from = start[2*index];
                if (translate1)
                    seq_int1->to = start[2*index] + CODON_LENGTH*length[index] - 1;
                else
                    seq_int1->to = start[2*index] + length[index] - 1;
                seq_int1->strand = strands[2*index];

                seq_int1->id = SeqIdDup(query_id);

                ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
            } else {
                ValNodeAddPointer(&slp1, SEQLOC_EMPTY, SeqIdDup(query_id));
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

                seq_int1->id = SeqIdDup(subject_id);

                ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int1);
            } else {
                ValNodeAddPointer(&slp2, SEQLOC_EMPTY, SeqIdDup(subject_id));
            }

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

/** Convert an HSP into a SeqAlign of type DenseSeg.
 * Used for a non-simple interval (i.e., one without subs. or 
 * deletions).  
 * @param program Type of BLAST program [in]
 * @param hsp HSP structure to convert. [in]
 * @param subject_id Seq-id of the subject sequence [in]
 * @param query_id Seq-id of the query sequence [in]
 * @param query_length Length of query sequence [in]
 * @param subject_length Length of subject sequence [in]
 * @return Seq-align corresponding to this alignment.
 */
static SeqAlignPtr
s_BlastHSPToSeqAlign(EBlastProgramType program, BlastHSP* hsp, 
                     SeqIdPtr subject_id, SeqIdPtr query_id,
                     Int4 query_length, Int4 subject_length)

{
    GapEditScript* curr,* curr_last;
    Int4 numseg, start1, start2;
    Int4* length,* start;
    Uint1* strands;
    Boolean is_disc_align = FALSE;
    SeqAlignPtr sap, sap_disc, sap_head, sap_tail;
    Boolean skip_region = FALSE;
    Boolean translate1, translate2;

    is_disc_align = FALSE; numseg = 0;

    for (curr=hsp->gap_info; curr; curr = curr->next) {
        numseg++;
        if(/*edit_block->discontinuous && */curr->op_type == eGapAlignDecline)
           is_disc_align = TRUE;
    }
    
    start1 = hsp->query.offset;
    start2 = hsp->subject.offset;
    translate1 = (program == eBlastTypeBlastx || program == eBlastTypeTblastx ||
                  program == eBlastTypeRpsTblastn);
    translate2 = (program == eBlastTypeTblastn || program == eBlastTypeTblastx);

    
    /* If no eGapAlignDecline regions exists output seqalign will be
       regular Den-Seg or Std-seg */
    if(is_disc_align == FALSE) {
        /* Please note, that edit_block passed only for data like
           strand, translate, reverse etc. Real data is taken starting
           from "curr" and taken only "numseg" segments */
        
        GapCollectDataForSeqalign(hsp, hsp->gap_info, numseg, query_length,
                                  subject_length, translate1, translate2,
                                  &start, &length, &strands, &start1, &start2);
        
        /* Result of this function will be either den-seg or Std-seg
           depending on translation options */
        sap = s_GapMakeSeqAlign(query_id, subject_id, translate1, translate2, 
                                numseg, length, start, strands);
    } else {

        /* By request of Steven Altschul - we need to have 
           the unaligned part being to the left if it is adjacent to the
           gap (insertion or deletion) - so this function will do
           shaffeling */

        s_GapCorrectUASequence(hsp); 

        sap_disc = SeqAlignNew();
        sap_disc->dim = 2;
        sap_disc->type = SAT_PARTIAL; /* ordered segments, over part of seq */
        sap_disc->segtype = SAS_DISC; /* discontinuous alignment */
        
        curr_last = hsp->gap_info; 
        sap_head = NULL; sap_tail = NULL;
        while(curr_last != NULL) {
            skip_region = FALSE;                        
            for (numseg = 0, curr = curr_last; 
                 curr; curr = curr->next, numseg++) {

                if(curr->op_type == eGapAlignDecline) {
                    if(numseg != 0) { /* End of aligned area */
                        break;
                    } else {
                        while(curr && curr->op_type == eGapAlignDecline) {
                            numseg++;
                            curr = curr->next; 
                        }
                        skip_region = TRUE;                        
                        break;
                    }
                }
            }

            GapCollectDataForSeqalign(hsp, curr_last, numseg, query_length,
                                      subject_length, translate1, translate2,
                                      &start, &length, &strands, &start1, 
                                      &start2);
            
            if(!skip_region) {            
                sap = 
                    s_GapMakeSeqAlign(query_id, subject_id, translate1, 
                                      translate2, numseg, length, start, 
                                      strands);
                
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
 * @param program Type of BLAST program (blastx or tblastn) [in]
 * @param hsp Internal HSP structure. [in]
 * @param query_id Seq-id of the query sequence [in]
 * @param subject_id Seq-id of the subject sequence [in]
 * @param query_length Length of query sequence [in]
 * @param subject_length Length of subject sequence [in]
 * @return Resulting Seq-align.
 */
static SeqAlignPtr
s_OOFBlastHSPToSeqAlign(EBlastProgramType program, BlastHSP* hsp, 
                         SeqIdPtr query_id, SeqIdPtr subject_id,
                         Int4 query_length, Int4 subject_length)
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

    if (program == eBlastTypeBlastx) {
       reverse = TRUE;
       start1 = hsp->subject.offset;
       start2 = hsp->query.offset;
       frame1 = hsp->subject.frame;
       frame2 = hsp->query.frame;
       original_length1 = subject_length;
       original_length2 = query_length;
       id1 = subject_id;
       id2 = query_id;
    } else { 
       start1 = hsp->query.offset;
       start2 = hsp->subject.offset;
       frame1 = hsp->query.frame;
       frame2 = hsp->subject.frame;
       original_length1 = query_length;
       original_length2 = subject_length;
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
    
    esp = hsp->gap_info;
    
    sap = SeqAlignNew();
    
    sap->dim =2; /**only two dimention alignment**/
    
    sap->type =3; /**partial for gapped translating search. */
    sap->segtype =3; /**use denseg to store the alignment**/
    sseg_head = NULL;
    sseg_old = NULL;

    first_shift = FALSE;

    for (curr=esp; curr; curr=curr->next) {

        slp1 = NULL;
        slp2 = NULL;
        
        switch (curr->op_type) {
        case eGapAlignDel: /* deletion of three nucleotides. */
            
            first_shift = FALSE;

            seq_int1 = SeqIntNew();
            seq_int1->from = s_GetCurrentPos(&start1, curr->num);
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

        case eGapAlignIns: /* insertion of three nucleotides. */

            /* If gap is followed after frameshift - we have to
               add this element for the alignment to be correct */
            
            if(first_shift == TRUE) { /* Second frameshift in a row */
                /* Protein coordinates */
                seq_int1 = SeqIntNew();
                seq_int1->from =  s_GetCurrentPos(&start1, 1);
                seq_int1->to = start1 - 1;

                if(seq_int1->to >= original_length1)
                    seq_int1->to = original_length1-1;
                
                seq_int1->id = SeqIdDup(id1);
                seq_int1->strand = strand1;
                
                ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
                
                /* Nucleotide scale shifted by op_type */
                seq_int2 = SeqIntNew();

                seq_int2->from = s_GetCurrentPos(&start2, 3);
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
            seq_int2->from = s_GetCurrentPos(&start2, curr->num*3);
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

        case eGapAlignSub: /* Substitution. */

            first_shift = FALSE;

            /* Protein coordinates */
            seq_int1 = SeqIntNew();
            seq_int1->from =  s_GetCurrentPos(&start1, curr->num);
            seq_int1->to = start1 - 1;

            if(seq_int1->to >= original_length1)
                seq_int1->to = original_length1-1;
            
            seq_int1->id = SeqIdDup(id1);
            seq_int1->strand = strand1;

            ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
           
            /* Nucleotide scale shifted by op_type */
            seq_int2 = SeqIntNew();

            seq_int2->from = 
               s_GetCurrentPos(&start2, curr->num*(Uint1)curr->op_type);
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
        case eGapAlignDel2:	/* gap of two nucleotides. */
        case eGapAlignDel1: /* Gap of one nucleotide. */
        case eGapAlignIns1: /* Insertion of one nucleotide. */
        case eGapAlignIns2: /* Insertion of two nucleotides. */

            if(first_shift == TRUE) { /* Second frameshift in a row */
                /* Protein coordinates */
                seq_int1 = SeqIntNew();
                seq_int1->from =  s_GetCurrentPos(&start1, 1);
                seq_int1->to = start1 - 1;

                if(seq_int1->to >= original_length1)
                    seq_int1->to = original_length1-1;
                
                seq_int1->id = SeqIdDup(id1);
                seq_int1->strand = strand1;
                
                ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
                
                /* Nucleotide scale shifted by op_type */
                seq_int2 = SeqIntNew();

                seq_int2->from = 
                   s_GetCurrentPos(&start2, (Uint1)curr->op_type);
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
            if(seq_int2_last != NULL) {
                s_GetCurrentPos(&start2, curr->num*((Uint1)curr->op_type-3));
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

            } else if ((Uint1)curr->op_type > 3) {
                /* Protein piece is empty */
                ValNodeAddPointer(&slp1, SEQLOC_EMPTY, SeqIdDup(id1));
                /* Simulating insertion of nucleotides */
                seq_int2 = SeqIntNew();
                seq_int2->from = 
                   s_GetCurrentPos(&start2, 
                                   curr->num*((Uint1)curr->op_type-3));
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

/** Converts a list of gapped HSPs into a list of Seq-align's and appends to a
 * previously created Seq-align chain.
 * @param program_number Type of BLAST program.
 * @param hsp_list HSP list structure [in]
 * @param query_id Seq-id of the query sequence [in]
 * @param subject_id Seq-id of the subject sequence [in]
 * @param query_length Length of the query sequence [in]
 * @param subject_length Length of the subject sequence [in]
 * @param is_ooframe Is this an alignment with out-of-frame gapping? [in]
 * @param head_seqalign Start of the Seq-align chain [in] [out]
 */
static Int2 
s_HSPListToSeqAlignGapped(EBlastProgramType program_number, 
                          BlastHSPList* hsp_list, SeqIdPtr query_id, 
                          SeqIdPtr subject_id, Int4 query_length, 
                          Int4 subject_length, Boolean is_ooframe, 
                          SeqAlignPtr* head_seqalign)
{
   Int2 status = 0;
   BlastHSP** hsp_array;
   SeqAlignPtr last_seqalign = NULL, seqalign = NULL;
   Int4 index;

   *head_seqalign = NULL;

   hsp_array = hsp_list->hsp_array;

   for (index=0; index<hsp_list->hspcnt; index++) { 
      if (is_ooframe) {
         seqalign = 
             s_OOFBlastHSPToSeqAlign(program_number, hsp_array[index], 
                                      query_id, subject_id, query_length, 
                                      subject_length);
      } else {
         /* The following line is needed for negative frames of translated 
            query */
         seqalign = 
             s_BlastHSPToSeqAlign(program_number, hsp_array[index], subject_id, 
                                  query_id, query_length, subject_length);
      }
      if (index==0) {
         *head_seqalign = last_seqalign = seqalign;
      } else {
         last_seqalign->next = seqalign;
         last_seqalign = last_seqalign->next;
      }
      seqalign->score = s_GetScoreSetFromBlastHsp(hsp_array[index]);
   }

   return status;
}

/** Adjusts the offsets in a Seq-align list produced by a BLAST search
 * of multiple queries against multiple subjects. Seq-aligns in a list are
 * assumed to be sorted by query, and then by subject, i.e. all Seq-aligns for
 * the first query are at the beginning of the list, then all for the second
 * query, etc. Within a list for any single query, all Seq-aligns for one 
 * subject are grouped together. The order of queries in the Seq-align list 
 * must be the same as in the query Seq-loc list; the order of subjects
 * within a Seq-align list for a given query is the same as in the subject
 * Seq-loc list. Some or all queries or subjects might be skipped in the 
 * Seq-align list.
 * @param head List of Seq-aligns [in|out]
 * @param query_slp List of query locations [in]
 * @param subject_slp List of subject locations [in]
 */
static void 
s_AdjustOffsetsInSeqAlign(SeqAlign* head, SeqLoc* query_slp,
                          SeqLoc* subject_slp)
{
   SeqAlign* seqalign, *next_seqalign = NULL, *last_seqalign;
   SeqLoc* qslp, *sslp;
   SeqId* query_id, *subject_id;
   
   qslp = query_slp;
   sslp = subject_slp;

   for (seqalign = head; seqalign; seqalign = next_seqalign) {
      query_id = TxGetQueryIdFromSeqAlign(seqalign);
      subject_id = TxGetSubjectIdFromSeqAlign(seqalign);

      /* Find Seq-locs corresponding to these query and subject. Start looking
	 from the previously found Seq-locs, because Seq-aligns are sorted
	 appropriately. */
      for ( ; qslp; qslp = qslp->next) {
	 if (SeqIdComp(query_id, SeqLocId(qslp)) == SIC_YES)
	    break;
	 /* Changing query: return subject Seq-loc pointer to the beginning 
	    of the list of subject Seq-locs.*/
	 sslp = subject_slp;
      }
      for ( ; sslp; sslp = sslp->next) {
	 if (SeqIdComp(subject_id, SeqLocId(sslp)) == SIC_YES)
	    break;
      }

      /* Find the first Seq-align that has either different subject or 
	 different query. */
      last_seqalign = seqalign;
      for (next_seqalign = seqalign->next; next_seqalign; 
	   next_seqalign = next_seqalign->next) {
	 if ((SeqIdComp(subject_id, TxGetSubjectIdFromSeqAlign(next_seqalign)) 
	      != SIC_YES) || 
	     (SeqIdComp(query_id, TxGetQueryIdFromSeqAlign(next_seqalign)) 
	      != SIC_YES)) {
	    /* Unlink the Seq-align chain */
	    last_seqalign->next = NULL;
	    break;
	 } else {
	    last_seqalign = next_seqalign;
	 }
      }
      AdjustOffSetsInSeqAlign(seqalign, qslp, sslp);
      /* Reconnect the Seq-align chain */
      last_seqalign->next = next_seqalign;
   }
}

Int2 BLAST_ResultsToSeqAlign(EBlastProgramType program_number, 
        BlastHSPResults* results, SeqLocPtr query_slp, 
        ReadDBFILE* rdfp, SeqLoc* subject_slp,
        Boolean is_gapped, Boolean is_ooframe, 
        SeqAlignPtr* head_seqalign)
{
   Int4 query_index, subject_index;
   SeqLocPtr slp = query_slp;
   SeqIdPtr query_id, subject_id = NULL;
   BlastHitList* hit_list;
   BlastHSPList* hsp_list;
   SeqAlignPtr seqalign = NULL, last_seqalign = NULL;
   Int4 subject_length = 0;
   SeqLoc** subject_loc_array = NULL;
   
   *head_seqalign = NULL;
   if (!results)
      return 0;

   if (!rdfp && !subject_slp)
      return -1;

   if (!rdfp) {
      subject_loc_array = 
         (SeqLoc**) malloc(ValNodeLen(subject_slp)*sizeof(SeqLoc*));
      for (slp = subject_slp, subject_index = 0; slp; slp = slp->next, ++subject_index)
         subject_loc_array[subject_index] = slp;
   }

   slp = query_slp;
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

         // Sort HSPs with e-values as first priority and scores as 
         // tie-breakers, since that is the order we want to see them in 
         // in Seq-aligns.
         Blast_HSPListSortByEvalue(hsp_list);

         if (rdfp) {
             /* NB: The following call allocates the SeqId structure. */
            readdb_get_descriptor(rdfp, hsp_list->oid, &subject_id, NULL);
            subject_length = readdb_get_sequence_length(rdfp, hsp_list->oid);
         } else {
             /* NB: The following call does not allocate the SeqId structure,
                but returns the existing one. */
            subject_id = SeqLocId(subject_loc_array[hsp_list->oid]);
            subject_length = SeqLocLen(subject_loc_array[hsp_list->oid]);
         }

         if (is_gapped) {
            s_HSPListToSeqAlignGapped(program_number, hsp_list, query_id, 
                                      subject_id, SeqLocLen(slp), subject_length,
                                      is_ooframe, &seqalign);
         } else {
            s_HSPListToSeqAlignUngapped(program_number, hsp_list, query_id,
                                        subject_id, SeqLocLen(slp), 
                                        subject_length, &seqalign);
         }                      

         /* The subject id must be deallocated only in case of a ReadDB 
            interface */
         if (rdfp)
             subject_id = SeqIdSetFree(subject_id);

         if (seqalign) {
	    if (!last_seqalign) {
	       *head_seqalign = last_seqalign = seqalign;
	    } else {
	       last_seqalign->next = seqalign;
	    }
	    for ( ; last_seqalign->next; last_seqalign = last_seqalign->next);
         }
      }
   }

   sfree(subject_loc_array);

   s_AdjustOffsetsInSeqAlign(*head_seqalign, query_slp, subject_slp);

   return 0;
}
/* @} */


