/* $Id: blast_traceback.c,v 1.167 2005/04/27 21:11:35 dondosha Exp $
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
 * ===========================================================================
 *
 * Author: Ilya Dondoshansky
 *
 */

/** @file blast_traceback.c
 * Functions responsible for the traceback stage of the BLAST algorithm.
 * <pre>
 * The hierarchy of function calls for performing traceback is:
 *    Blast_RunTracebackSearch 
 *        BLAST_GapAlignSetUp
 *        BLAST_ComputeTraceback
 *            if ( RPS BLAST ) 
 *                s_RPSComputeTraceback
 *                    for ( all HSP lists )
 *                        Blast_TracebackFromHSPList
 *            else if ( composition based statistics )
 *                Kappa_RedoAlignmentCore
 *            else
 *                for ( all HSP lists )
 *                    if ( PHI BLAST) 
 *                        s_PHITracebackFromHSPList
 *                    else
 *                        Blast_TracebackFromHSPList
 * </pre>
 */

#ifndef SKIP_DOXYGEN_PROCESSING
static char const rcsid[] = 
    "$Id: blast_traceback.c,v 1.167 2005/04/27 21:11:35 dondosha Exp $";
#endif /* SKIP_DOXYGEN_PROCESSING */

#include <algo/blast/core/blast_traceback.h>
#include <algo/blast/core/blast_util.h>
#include <algo/blast/core/link_hsps.h>
#include <algo/blast/core/blast_setup.h>
#include <algo/blast/core/blast_kappa.h>
#include <algo/blast/core/phi_gapalign.h>
#include "blast_gapalign_priv.h"
#include "blast_psi_priv.h"
#include "blast_hits_priv.h"
#include "blast_itree.h"

/** Window size used to scan HSP for highest score region, where gapped
 * extension starts. 
 */
#define HSP_MAX_WINDOW 11

/** Function to check that the highest scoring region in an HSP still gives a 
 * positive score. This value was originally calcualted by 
 * BlastGetStartForGappedAlignment but it may have changed due to the 
 * introduction of ambiguity characters. Such a change can lead to 'strange' 
 * results from ALIGN. 
 * @param hsp An HSP structure [in]
 * @param query Query sequence buffer [in]
 * @param subject Subject sequence buffer [in]
 * @param sbp Scoring block containing matrix [in]
 * @return TRUE if region aroung starting offsets gives a positive score
*/
Boolean
BLAST_CheckStartForGappedAlignment(const BlastHSP* hsp, const Uint1* query, 
                                   const Uint1* subject, const BlastScoreBlk* sbp)
{
    Int4 index, score, start, end, width;
    Uint1* query_var,* subject_var;
    Boolean positionBased = (sbp->psi_matrix != NULL);
    
    width = MIN((hsp->query.gapped_start-hsp->query.offset), HSP_MAX_WINDOW/2);
    start = hsp->query.gapped_start - width;
    end = MIN(hsp->query.end, hsp->query.gapped_start + width);
    /* Assures that the start of subject is above zero. */
    if ((hsp->subject.gapped_start + start - hsp->query.gapped_start) < 0) {
        start -= hsp->subject.gapped_start + (start - hsp->query.gapped_start);
    }
    query_var = ((Uint1*) query) + start;
    subject_var = ((Uint1*) subject) + hsp->subject.gapped_start + 
        (start - hsp->query.gapped_start);
    score=0;
    for (index = start; index < end; index++) {
        if (!positionBased)
            score += sbp->matrix->data[*query_var][*subject_var];
        else
            score += sbp->psi_matrix->pssm->data[index][*subject_var];
        query_var++; subject_var++;
    }
    if (score <= 0) {
        return FALSE;
    }
    
    return TRUE;
}

Int2
Blast_HSPUpdateWithTraceback(BlastGapAlignStruct* gap_align, BlastHSP* hsp)
{
   if (!hsp || !gap_align)
     return -1;

   hsp->score = gap_align->score;
   hsp->query.offset = gap_align->query_start;
   hsp->subject.offset = gap_align->subject_start;
   hsp->query.end = gap_align->query_stop;
   hsp->subject.end = gap_align->subject_stop;
   /* If editing block is non-NULL, transfer ownership to the HSP structure.
      Then fill the missing infomation in the editing block. */
   if (gap_align->edit_script) { 
      hsp->gap_info = gap_align->edit_script;
      gap_align->edit_script = NULL;
   }

   return 0;
}

/** Remove scaling from scores previously calculated on the hsp_list.
 * @param hsp_list list of HPSs with the score field calculated [in|out]
 * @param scale_factor factor by which scores are scaled, for everything other
 * than RPS-BLAST this should be 1 [in] 
 * @todo rename to something which is more intention revealing, merge with
 * function of the same name in blast_kappa.c
 */
static void 
s_HSPListRescaleScores(BlastHSPList* hsp_list, double scale_factor)
{
   Int4 index;

   for (index = 0; index < hsp_list->hspcnt; ++index) {
      BlastHSP* hsp = hsp_list->hsp_array[index];
      
      /* Remove any scaling of the calculated score */
      hsp->score = 
         (Int4) ((hsp->score+(0.5*scale_factor)) / scale_factor);
   }

   /* Sort HSPs by score again because after the loop above scores that
    * were previously different can become equal, and then the order of HSPs
    * should be determined by the tie-breaking criteria 
    * (e.g.: subject offsets, ...) */
   Blast_HSPListSortByScore(hsp_list);
}

/** Simple macro to swap two numbers. */
#define SWAP(a, b) {tmp = (a); (a) = (b); (b) = tmp; }

/** Swaps query and subject information in an HSP structure for RPS BLAST 
 * search. This is necessary because query and subject are switched during the 
 * traceback alignment, and must be switched back.
 * @param hsp HSP structure to fix. [in] [out]
 */
static void 
s_BlastHSPRPSUpdate(BlastHSP *hsp)
{
   GapEditScript *esp;

   if (hsp->gap_info == NULL)
      return;

   esp = hsp->gap_info;
   while (esp != NULL) {
      if (esp->op_type == eGapAlignIns)
          esp->op_type = eGapAlignDel;
      else if (esp->op_type == eGapAlignDel)
          esp->op_type = eGapAlignIns;

      esp = esp->next;
   }
}

/** Switches back the query and subject in all HSPs in an HSP list; also
 * reassigns contexts to indicate query context, needed to pick correct
 * Karlin block later in the code.
 * @param program Program type: RPS or RPS tblastn [in]
 * @param hsplist List of HSPs [in] [out]
 */
static void 
s_BlastHSPListRPSUpdate(EBlastProgramType program, BlastHSPList *hsplist)
{
   Int4 i;
   BlastHSP **hsp;
   BlastSeg tmp;

   /* If this is not an RPS BLAST search, do not do anything. */
   if (program != eBlastTypeRpsBlast && program != eBlastTypeRpsTblastn)
      return;

   hsp = hsplist->hsp_array;
   for (i = 0; i < hsplist->hspcnt; i++) {

      /* switch query and subject offsets (which are
         already in local coordinates) */

      tmp = hsp[i]->query;
      hsp[i]->query = hsp[i]->subject;
      hsp[i]->subject = tmp;

      /* Change the traceback information to reflect the
         query and subject sequences getting switched */

      s_BlastHSPRPSUpdate(hsp[i]);

      /* If query was nucleotide, set context, because it is needed in order 
	 to pick correct Karlin block for calculating bit scores. There are 
         6 contexts corresponding to each nucleotide query sequence. */
      if (program == eBlastTypeRpsTblastn) {
	 hsp[i]->context = FrameToContext(hsp[i]->query.frame);
      }
   }
   Blast_HSPListSortByScore(hsplist);
}

/** Updates the e-values after the traceback alignment. Also includes relinking
 * of HSPs in case of sum statistics and calculation of bit scores.
 * @param program_number Type of BLAST program [in]
 * @param hsp_list HSPList obtained after a traceback alignment [in] [out]
 * @param query_info Query information structure [in]
 * @param score_params Scoring parameters [in]
 * @param hit_params Hit saving parameters [in]
 * @param sbp Scoring block [in]
 * @param subject_length Length of the subject sequence - needed for linking 
 *                       HSPs [in]
 */
static Int2
s_HSPListPostTracebackUpdate(EBlastProgramType program_number, 
   BlastHSPList* hsp_list, BlastQueryInfo* query_info, 
   const BlastScoringParameters* score_params, 
   const BlastHitSavingParameters* hit_params, 
   BlastScoreBlk* sbp, Int4 subject_length)
{
   BlastScoringOptions* score_options = score_params->options;
   const Boolean kGapped = score_options->gapped_calculation;
   
   /* Revert query and subject to their traditional meanings. 
      This involves switching the offsets around and reversing
      any traceback information */
   s_BlastHSPListRPSUpdate(program_number, hsp_list);
   
   /* Relink and rereap the HSP list, if needed. */
   if (hit_params->link_hsp_params) {
      BLAST_LinkHsps(program_number, hsp_list, query_info, subject_length,
                     sbp, hit_params->link_hsp_params, kGapped);
   } else {
      /* Only use the scaling factor from parameters structure for RPS BLAST, 
       * because for other programs either there is no scaling at all, or, in
       * case of composition based statistics, Lambda is scaled as well as 
       * scores, and hence scaling factor should not be used for e-value 
       * computations. 
       */
      double scale_factor = 
         (program_number == eBlastTypeRpsBlast || 
          program_number == eBlastTypeRpsTblastn) ? 
         score_params->scale_factor : 1.0;
      Blast_HSPListGetEvalues(query_info, hsp_list, kGapped, sbp, 0,
                              scale_factor);
   }

   Blast_HSPListReapByEvalue(hsp_list, hit_params->options);
   
   /* Rescale the scores by scaling factor, if necessary. This rescaling
    * should be done for all programs where scaling factor is not 1.
    */
   s_HSPListRescaleScores(hsp_list, score_params->scale_factor);
   
   /** Calculate and fill the bit scores. @todo: This is not the only time 
    * when they are calculated, s_HSPListRescaleScores also does this in 
    * blast_kappa.c.
    */
   Blast_HSPListGetBitScores(hsp_list, kGapped, sbp);

   return 0;
}

/*
    Comments in blast_traceback.h
 */
Int2
Blast_TracebackFromHSPList(EBlastProgramType program_number, 
   BlastHSPList* hsp_list, BLAST_SequenceBlk* query_blk, 
   BLAST_SequenceBlk* subject_blk, BlastQueryInfo* query_info_in,
   BlastGapAlignStruct* gap_align, BlastScoreBlk* sbp, 
   const BlastScoringParameters* score_params,
   const BlastExtensionOptions* ext_options,
   const BlastHitSavingParameters* hit_params, const Uint1* gen_code_string)
{
   Int4 index;
   BlastHSP* hsp;
   Uint1* query,* subject;
   Int4 query_length;
   Int4 subject_length=0;
   BlastHSP** hsp_array;
   Int4 q_start, s_start;
   BlastHitSavingOptions* hit_options = hit_params->options;
   BlastScoringOptions* score_options = score_params->options;
   Int4 context_offset;
   Uint1* translation_buffer = NULL;
   Int4 * frame_offsets   = NULL;
   Int4 * frame_offsets_a = NULL;
   Boolean partial_translation = FALSE;
   const Boolean kIsOutOfFrame = score_options->is_ooframe;
   const Boolean kGreedyTraceback = (ext_options->eTbackExt == eGreedyTbck);
   const Boolean kTranslateSubject = 
      (program_number == eBlastTypeTblastn ||
       program_number == eBlastTypeRpsTblastn); 
   BlastQueryInfo* query_info = query_info_in;
   Int4 offsets[2];
   BlastIntervalTree* tree = NULL;
   
   if (hsp_list->hspcnt == 0) {
      return 0;
   }

   hsp_array = hsp_list->hsp_array;

   if (kTranslateSubject) {
      if (!gen_code_string && program_number != eBlastTypeRpsTblastn)
         return -1;

      if (kIsOutOfFrame) {
         Blast_SetUpSubjectTranslation(subject_blk, gen_code_string,
                                       NULL, NULL, &partial_translation);
         /* Length of a mixed-frame sequence, corresponding to each single 
            strand of the nucleotide sequence, is equal to nucleotide 
            length. */
         subject_length = subject_blk->length;
      } else if (program_number == eBlastTypeRpsTblastn) {
          translation_buffer = subject_blk->sequence - 1;
          frame_offsets_a = frame_offsets =
              ContextOffsetsToOffsetArray(query_info_in);
      } else {
         Blast_SetUpSubjectTranslation(subject_blk, gen_code_string,
            &translation_buffer, &frame_offsets, &partial_translation);
         frame_offsets_a = frame_offsets;
         /* subject and subject_length will be set later, for each HSP. */
      }
   } else {
      /* Subject is not translated */
      subject = subject_blk->sequence;
      subject_length = subject_blk->length;
   }

   if (program_number == eBlastTypeRpsBlast || 
       program_number == eBlastTypeRpsTblastn) {
      /* Create a local BlastQueryInfo structure for this subject sequence
	 that has been switched with the query. */
      query_info = BlastMemDup(query_info_in, sizeof(BlastQueryInfo));
      query_info->first_context = query_info->last_context = 0;
      query_info->num_queries = 1;
      offsets[0] = 0;
      offsets[1] = query_blk->length + 1;
      OffsetArrayToContextOffsets(query_info, offsets, program_number);
   }

   /* Make sure the HSPs in the HSP list are sorted by score, as they should 
      be. */
   ASSERT(Blast_HSPListIsSortedByScore(hsp_list));

   /* set up the tree for HSP containment tests. subject_length 
      is zero only for translated subject sequences, whose maximum
      length is bounded by the length of the first frame */

   tree = Blast_IntervalTreeInit(0, query_blk->length + 1,
                                 0, (subject_length > 0 ? subject_length :
                                 subject_blk->length / CODON_LENGTH) + 1);

   for (index=0; index < hsp_list->hspcnt; index++) {
      hsp = hsp_array[index];
      if (program_number == eBlastTypeBlastx || 
          program_number == eBlastTypeTblastx) {
         Int4 context = hsp->context - hsp->context % 3;
         context_offset = query_info->contexts[context].query_offset;
         
         if (kIsOutOfFrame) {
            query = query_blk->oof_sequence + CODON_LENGTH + context_offset;
            query_length = query_info->contexts[context+2].query_offset +
                query_info->contexts[context+2].query_length - context_offset;
         } else {
            query = query_blk->sequence + 
               query_info->contexts[hsp->context].query_offset;
            query_length = query_info->contexts[hsp->context].query_length;
         }
      } else {
          query = query_blk->sequence + 
              query_info->contexts[hsp->context].query_offset;
          query_length = query_info->contexts[hsp->context].query_length;
      }
      
      /* preliminary RPS blast alignments have not had
         the composition-based correction applied yet, so
         we cannot reliably check whether an HSP is contained
         within another */

      /** @todo FIXME Traceback is always performed for rpsblast
       * because the composition-based correction can change an
       * HSP. It is optional for RPStblastn since no corrections
       * are applied there. Such corrections should be added.
       */
      if (program_number == eBlastTypeRpsBlast ||
          !BlastIntervalTreeContainsHSP(tree, hsp, query_info, 0)) {

         Int4 start_shift = 0;
         Int4 adjusted_s_length;
         Uint1* adjusted_subject;

         if (kTranslateSubject) {
            if (!kIsOutOfFrame && !partial_translation) {
               Int4 context = FrameToContext(hsp->subject.frame);
               subject = translation_buffer + frame_offsets[context] + 1;
               subject_length = 
                  frame_offsets[context+1] - frame_offsets[context] - 1;
            } else { 
                if (partial_translation) {
                    Blast_HSPGetPartialSubjectTranslation(subject_blk, hsp, 
                        kIsOutOfFrame, gen_code_string, &translation_buffer, 
                        &subject, &subject_length, &start_shift);
                } else {
                    /* Out-of-frame with full translation; point subject to the
                       start of the right strand in the mixed-frame sequence. */
                    subject = subject_blk->oof_sequence + CODON_LENGTH;
                    if (hsp->subject.frame < 0)
                        subject += subject_length + 1;
                }
            }
         }

         if (!kIsOutOfFrame && (((hsp->query.gapped_start == 0 && 
                                  hsp->subject.gapped_start == 0) ||
                 !BLAST_CheckStartForGappedAlignment(hsp, query, 
                                                     subject, sbp)))) {
            Int4 max_offset = 
               BlastGetStartForGappedAlignment(query, subject, sbp,
                  hsp->query.offset, hsp->query.end - hsp->query.offset,
                  hsp->subject.offset, hsp->subject.end - hsp->subject.offset);
            q_start = max_offset;
            s_start = 
               (hsp->subject.offset - hsp->query.offset) + max_offset;
            hsp->query.gapped_start = q_start;
            hsp->subject.gapped_start = s_start;
         } else {
            if(kIsOutOfFrame) {
               /* Code below should be investigated for possible
                  optimization for OOF */
               s_start = hsp->subject.gapped_start;
               q_start = hsp->query.gapped_start;
               gap_align->subject_start = 0;
               gap_align->query_start = 0;
            } else {
               q_start = hsp->query.gapped_start;
               s_start = hsp->subject.gapped_start;
            }
         }
         
         adjusted_s_length = subject_length;
         adjusted_subject = subject;

        /* Perform the gapped extension with traceback */
         if (!kTranslateSubject) {
             AdjustSubjectRange(&s_start, &adjusted_s_length, q_start, 
                                query_length, &start_shift);
             adjusted_subject = subject + start_shift;
             /* Shift the gapped start in HSP structure, to compensate for 
                a shift in the other direction later. */
             hsp->subject.gapped_start = s_start;
         }
         if (kGreedyTraceback) {
             BLAST_GreedyGappedAlignment(query, adjusted_subject, 
                 query_length, adjusted_s_length, gap_align, 
                 score_params, q_start, s_start, FALSE, TRUE);
         } else {
             BLAST_GappedAlignmentWithTraceback(program_number, query, 
                 adjusted_subject, gap_align, score_params, q_start, s_start, 
                 query_length, adjusted_s_length);
         }

         if (gap_align->score >= hit_params->cutoff_score) {
            Boolean delete_hsp = FALSE;
            Blast_HSPUpdateWithTraceback(gap_align, hsp);

            if (kGreedyTraceback) {
               /* Low level greedy algorithm ignores ambiguities, so the score
                * needs to be reevaluated. */
                delete_hsp = 
                    Blast_HSPReevaluateWithAmbiguitiesGapped(hsp, query, 
                        adjusted_subject, hit_params, score_params, sbp);
            }
            if (!delete_hsp) {
                /* Calculate number of identities and check if this HSP meets the
                   percent identity and length criteria. */
                delete_hsp = 
                    Blast_HSPTestIdentityAndLength(program_number, hsp, query, 
                                                   adjusted_subject, 
                                                   score_options, hit_options);
            }
            if (!delete_hsp) {
               Blast_HSPAdjustSubjectOffset(hsp, start_shift);
               BlastIntervalTreeAddHSP(hsp, tree, query_info, 
                                       eQueryAndSubject);
            } else {
               hsp_array[index] = Blast_HSPFree(hsp);
            }
         } else {
            /* Score is below threshold */
            gap_align->edit_script = GapEditScriptDelete(gap_align->edit_script);
            hsp_array[index] = Blast_HSPFree(hsp);
         }
      } else {
         /* Contained within another HSP, delete. */
         hsp_array[index] = Blast_HSPFree(hsp);
      }
   } /* End loop on HSPs */

   if (program_number != eBlastTypeRpsTblastn) {
      if (translation_buffer) {
         sfree(translation_buffer);
      }
   }
   
   if (frame_offsets_a) {
       sfree(frame_offsets_a);
   }
   
   /* Remove any HSPs that share a starting or ending diagonal
      with a higher-scoring HSP. */
   hsp_list->hspcnt = 
       Blast_CheckHSPsForCommonEndpoints(hsp_list->hsp_array, 
                                         hsp_list->hspcnt);

   /* Sort HSPs by score again, as the scores might have changed. */
   Blast_HSPListSortByScore(hsp_list);

   /* Remove any HSPs that are contained within other HSPs.
      Since the list is sorted by score already, any HSP
      contained by a previous HSP is guaranteed to have a
      lower score, and may be purged. */
   Blast_IntervalTreeReset(tree);
   for (index = 0; index < hsp_list->hspcnt; index++) {
       hsp = hsp_array[index];
       
       if (BlastIntervalTreeContainsHSP(tree, hsp, query_info, 0)) {
           hsp_array[index] = Blast_HSPFree(hsp);
       }
       else {
           BlastIntervalTreeAddHSP(hsp, tree, query_info, 
                                   eQueryAndSubject);
       }
   }
   tree = Blast_IntervalTreeFree(tree);

   Blast_HSPListPurgeNullHSPs(hsp_list);

   /* Free the local query_info structure, if necessary (RPS tblastn only) */
   if (query_info != query_info_in)
      sfree(query_info);
   
   s_HSPListPostTracebackUpdate(program_number, hsp_list, query_info_in, 
                                score_params, hit_params, sbp, 
                                subject_blk->length);
   
   return 0;
}

/** Performs traceback alignment for one HSP list in a PHI BLAST search.
 * @param program_number eBlastTypePhiBlastn or eBlastTypePhiBlastp [in]
 * @param hsp_list HSP list for a single query/subject pair, with preliminary
 *                 alignment information [in]
 * @param query_blk Query sequence [in]
 * @param subject_blk Subject sequence [in]
 * @param gap_align Gapped alignment structure [in]
 * @param sbp Scoring block [in]
 * @param score_params Scoring parameters [in]
 * @param hit_params Hit saving parameters [in]
 * @param query_info Query information, including pattern occurrences in 
 *                   query [in]
 * @param pattern_blk Pattern information and auxiliary structures [in]
 */
static Int2
s_PHITracebackFromHSPList(EBlastProgramType program_number, 
                          BlastHSPList* hsp_list, BLAST_SequenceBlk* query_blk, 
                          BLAST_SequenceBlk* subject_blk, 
                          BlastGapAlignStruct* gap_align, BlastScoreBlk* sbp, 
                          const BlastScoringParameters* score_params,
                          const BlastHitSavingParameters* hit_params,
                          const BlastQueryInfo* query_info,
                          SPHIPatternSearchBlk* pattern_blk)
{
   Int4 index;
   BlastHSP* hsp;
   Uint1* query,* subject;
   Int4 query_length;
   Int4 subject_length=0;
   BlastHSP** hsp_array;
   Int4 q_start, s_start;
   SPHIQueryInfo* pattern_info = NULL;
   
   if (program_number != eBlastTypePhiBlastn &&
       program_number != eBlastTypePhiBlastp)
       return -1;
   
   ASSERT(hsp_list && query_blk && subject_blk && gap_align && sbp &&
          score_params && hit_params && query_info && pattern_blk);

   if (hsp_list->hspcnt == 0) {
      return 0;
   }

   hsp_array = hsp_list->hsp_array;

   query = query_blk->sequence;
   query_length = query_blk->length;
   subject = subject_blk->sequence;
   subject_length = subject_blk->length;
   pattern_info = query_info->pattern_info;

   /* Make sure the HSPs in the HSP list are sorted by score, as they should 
      be. */
   ASSERT(Blast_HSPListIsSortedByScore(hsp_list));

   for (index=0; index < hsp_list->hspcnt; index++) {
       Int4 query_pattern_length;
       hsp = hsp_array[index];
       
       q_start = hsp->query.gapped_start;
       s_start = hsp->subject.gapped_start;
       query_pattern_length =
           pattern_info->occurrences[hsp->pat_info->index].length;
       
       /* Perform the gapped extension with traceback */
       PHIGappedAlignmentWithTraceback(query, subject, gap_align, 
                                       score_params, q_start, s_start, 
                                       query_length, subject_length,
                                       query_pattern_length, 
                                       hsp->pat_info->length, pattern_blk);
       
       if (gap_align->score >= hit_params->cutoff_score) {
           Blast_HSPUpdateWithTraceback(gap_align, hsp);

       } else {
           /* Score is below threshold */
           gap_align->edit_script = GapEditScriptDelete(gap_align->edit_script);
           hsp_array[index] = Blast_HSPFree(hsp);
       }
   } /* End loop on HSPs */

   /* Sort HSPs by score again, as the scores might have changed. */
   Blast_HSPListSortByScore(hsp_list);
   Blast_HSPListPurgeNullHSPs(hsp_list);

   /* Calculate scores and e-values. */
   Blast_HSPListPHIGetEvalues(hsp_list, sbp, query_info);
   Blast_HSPListReapByEvalue(hsp_list, hit_params->options);

   Blast_HSPListPHIGetBitScores(hsp_list, sbp);
   
   return 0;
}

Uint1 Blast_TracebackGetEncoding(EBlastProgramType program_number) 
{
   Uint1 encoding;

   switch (program_number) {
   case eBlastTypeBlastn:
   case eBlastTypePhiBlastn:
      encoding = BLASTNA_ENCODING;
      break;
   case eBlastTypeBlastp:
   case eBlastTypeRpsBlast:
   case eBlastTypeBlastx:
   case eBlastTypeRpsTblastn:
   case eBlastTypePsiBlast:
   case eBlastTypePhiBlastp:
      encoding = BLASTP_ENCODING;
      break;
   case eBlastTypeTblastn:
   case eBlastTypeTblastx:
      encoding = NCBI4NA_ENCODING;
      break;
   default:
      encoding = ERROR_ENCODING;
      break;
   }
   return encoding;
}

/** Delete extra subject sequences hits, if after-traceback hit list size is
 * smaller than preliminary hit list size.
 * @param results All results after traceback, assumed already sorted by best
 *                e-value [in] [out]
 * @param hitlist_size Final hit list size [in]
 */
static void 
s_BlastPruneExtraHits(BlastHSPResults* results, Int4 hitlist_size)
{
   Int4 query_index, subject_index;
   BlastHitList* hit_list;

   for (query_index = 0; query_index < results->num_queries; ++query_index) {
      if (!(hit_list = results->hitlist_array[query_index]))
         continue;
      for (subject_index = hitlist_size;
           subject_index < hit_list->hsplist_count; ++subject_index) {
         hit_list->hsplist_array[subject_index] = 
            Blast_HSPListFree(hit_list->hsplist_array[subject_index]);
      }
      hit_list->hsplist_count = MIN(hit_list->hsplist_count, hitlist_size);
   }
}

void RPSPsiMatrixAttach(BlastScoreBlk* sbp, Int4** rps_pssm)
{
    ASSERT(sbp);

    /* Create a dummy PSI-BLAST matrix structure, only to then free it as we'd
     * like to piggy back on the already created structure to use the gapped
     * alignment routines */
    sbp->psi_matrix = (SPsiBlastScoreMatrix*) 
        calloc(1, sizeof(SPsiBlastScoreMatrix));
    ASSERT(sbp->psi_matrix);

    sbp->psi_matrix->pssm = (SBlastScoreMatrix*)
        calloc(1, sizeof(SBlastScoreMatrix));
    ASSERT(sbp->psi_matrix->pssm);

    /* The only data field that RPS-BLAST really needs */
    sbp->psi_matrix->pssm->data = rps_pssm;
}

void RPSPsiMatrixDetach(BlastScoreBlk* sbp)
{
    ASSERT(sbp);
    sbp->psi_matrix->pssm->data = NULL;
    sfree(sbp->psi_matrix->pssm);
    sfree(sbp->psi_matrix);
}

/** Prepares an auxiliary BlastQueryInfo structure for the concatenated 
 * database and creates a memory mapped PSSM for RPS BLAST traceback.
 * @param concat_db_info BlastQueryInfo structure to fill. [out]
 * @param gap_align Gapped alignment structure to modify [in] [out]
 * @param rps_info RPS BLAST information structure [in]
 */
static Int2 
s_RPSGapAlignDataPrepare(BlastQueryInfo* concat_db_info, 
                         BlastGapAlignStruct* gap_align, 
                         const BlastRPSInfo* rps_info)
{
   Int4** rps_pssm = NULL;
   Int4 num_profiles;
   Int4 num_pssm_rows;
   Int4* pssm_start;
   BlastRPSProfileHeader *profile_header;
   Int4 index;

   if (!rps_info)
      return -1;

   memset(concat_db_info, 0, sizeof(BlastQueryInfo)); /* fill in QueryInfo */

   profile_header = rps_info->profile_header;
   num_profiles = profile_header->num_profiles;

   /* Construct an auxiliary BlastQueryInfo structure for the concatenated
      database. */
   concat_db_info->num_queries = num_profiles;
   concat_db_info->first_context = 0;
   concat_db_info->last_context = num_profiles - 1;
   OffsetArrayToContextOffsets(concat_db_info,
                               rps_info->profile_header->start_offsets,
                               eBlastTypeRpsBlast);

   num_pssm_rows = profile_header->start_offsets[num_profiles];
   rps_pssm = (Int4 **)malloc((num_pssm_rows+1) * sizeof(Int4 *));
   pssm_start = profile_header->start_offsets + num_profiles + 1;

   for (index = 0; index < num_pssm_rows + 1; index++) {
      rps_pssm[index] = pssm_start;
      pssm_start += BLASTAA_SIZE;
   }

   gap_align->positionBased = TRUE;
   RPSPsiMatrixAttach(gap_align->sbp, rps_pssm);

   return 0;
}

/** Factor to multiply the Karlin-Altschul K parameter by for RPS BLAST, to make
 * e-values more conservative.
 */
#define RPS_K_MULT 1.2

/** Compute traceback information for alignments found by an
 *  RPS blast search. This function performs two major tasks:
 *  - Computes a composition-specific PSSM to be used during the
 *    traceback computation (non-translated searches only)
 *  - After traceback is computed, switch query offsets with 
 *    subject offsets and switch the edit blocks that describe
 *    the alignment. This is required because the entire RPS search
 *    was performed with these quatities reversed.
 * This call is also the first time that enough information 
 * exists to compute E-values for alignments that are found.
 *
 * @param program_number Type of the BLAST program [in]
 * @param hsp_stream A stream for reading HSP lists [in]
 * @param seq_src Source of RPS database consensus sequences; needed only
 *                to calculate number of identities in alignments [in]
 * @param query The original query sequence [in]
 * @param query_info Information associated with the original query. 
 *                   Only used for the search space [in]
 * @param gap_align The auxiliary structure for gapped alignment [in]
 * @param score_params Scoring parameters (esp. scale factor) [in]
 * @param ext_params Gapped extension parameters [in]
 * @param hit_params Parameters for saving hits. Can change if not a 
                     database search [in]
 * @param rps_info Extra information about RPS database. [in]
 * @param results Results structure containing all HSPs, with added
 *                traceback information. [out]
 * @return nonzero indicates failure, otherwise zero
 */
static 
Int2 s_RPSComputeTraceback(EBlastProgramType program_number, 
                           BlastHSPStream* hsp_stream, 
                           const BlastSeqSrc* seq_src, 
                           BLAST_SequenceBlk* query, BlastQueryInfo* query_info, 
                           BlastGapAlignStruct* gap_align,
                           const BlastScoringParameters* score_params,
                           const BlastExtensionParameters* ext_params,
                           BlastHitSavingParameters* hit_params,
                           const BlastRPSInfo* rps_info,                
                           BlastHSPResults* results)
{
   Int2 status = 0;
   BlastHSPList* hsp_list;
   BlastScoreBlk* sbp;
   Int4 **rpsblast_pssms = NULL;
   Int4 db_seq_start;
   Uint1 encoding;
   BlastSeqSrcGetSeqArg seq_arg;
   BlastQueryInfo* one_query_info = NULL;
   BLAST_SequenceBlk* one_query = NULL;
   BlastQueryInfo* concat_db_info = NULL;

   if (!hsp_stream || !seq_src || !results) {
      return -1;
   }
   
   concat_db_info = (BlastQueryInfo*) malloc(sizeof(BlastQueryInfo));
   if ((status = 
        s_RPSGapAlignDataPrepare(concat_db_info, gap_align, rps_info)) != 0)
      return status;
      
   sbp = gap_align->sbp;
   rpsblast_pssms = gap_align->sbp->psi_matrix->pssm->data;

   encoding = Blast_TracebackGetEncoding(program_number);
   memset((void*) &seq_arg, 0, sizeof(seq_arg));

   while (BlastHSPStreamRead(hsp_stream, &hsp_list) 
          != kBlastHSPStream_Eof) {
      if (!hsp_list)
         continue;

      /* Restrict the query sequence block and information structures 
         to the one query this HSP list corresponds to. */
      if (Blast_GetOneQueryStructs(&one_query_info, &one_query, 
                                   query_info, query, 
                                   hsp_list->query_index) != 0)
          return -1;

      /* Pick out one of the sequences from the concatenated DB (given by the 
         OID of this HSPList). The sequence length does not include the 
         trailing NULL. The sequence itself is only needed to calculate number
         of identities, since scoring is done with a portion of the PSSM 
         corresponding to this sequence. */
      seq_arg.oid = hsp_list->oid;
      seq_arg.encoding = encoding;
      if (BlastSeqSrcGetSequence(seq_src, (void*) &seq_arg) < 0)
          continue;

      db_seq_start = concat_db_info->contexts[hsp_list->oid].query_offset;
      
      /* Update the statistics for this database sequence
         (if not a translated search) */
      
      if (program_number == eBlastTypeRpsTblastn) {
         sbp->psi_matrix->pssm->data = rpsblast_pssms + db_seq_start;
      } else {
         const double* karlin_k = rps_info->aux_info.karlin_k;
         /* replace the PSSM and the Karlin values for this DB sequence
            and this query sequence. */
         sbp->psi_matrix->pssm->data = 
            RPSRescalePssm(score_params->scale_factor,
                           one_query->length, one_query->sequence, 
                           seq_arg.seq->length,
                           rpsblast_pssms + db_seq_start,
                           sbp->name);
         /* The composition of the query could have caused this one
            subject sequence to produce a bad PSSM. This should
            not be a fatal error, so just go on to the next subject
            sequence */
         if (sbp->psi_matrix->pssm->data == NULL) {
            /** @todo FIXME Results should not be silently skipped,
             *        need a warning here
             */
            hsp_list = Blast_HSPListFree(hsp_list);
            BlastSeqSrcReleaseSequence(seq_src, (void*)&seq_arg);
            continue;
         }
         
         sbp->kbp_gap[0]->K = RPS_K_MULT * karlin_k[hsp_list->oid];
         sbp->kbp_gap[0]->logK = log(RPS_K_MULT * karlin_k[hsp_list->oid]);
      }


      /* compute the traceback information and calculate E values
         for all HSPs in the list */
      
      Blast_TracebackFromHSPList(program_number, hsp_list, seq_arg.seq, 
         one_query, one_query_info, gap_align, sbp, score_params, 
         ext_params->options, hit_params, NULL);

      BlastSeqSrcReleaseSequence(seq_src, (void*)&seq_arg);

      if (program_number != eBlastTypeRpsTblastn) {
         _PSIDeallocateMatrix((void**)sbp->psi_matrix->pssm->data, 
                              seq_arg.seq->length);
      }

      if (hsp_list->hspcnt == 0) {
         hsp_list = Blast_HSPListFree(hsp_list);
         continue;
      }

      /* Save this HSP list in the results structure. */
      Blast_HSPResultsInsertHSPList(results, hsp_list, 
                                    hit_params->options->hitlist_size);
   }

   BlastQueryInfoFree(concat_db_info);

   /* Free the sequence block allocated inside the loop */
   BlastSequenceBlkFree(seq_arg.seq);

   /* Free the single-query structures allocated inside the loop. */
   BlastQueryInfoFree(one_query_info);
   BlastSequenceBlkFree(one_query);

   /* The traceback calculated the E values, so it's safe
      to sort the results now */
   Blast_HSPResultsSortByEvalue(results);

   /* Free the allocated array of memory mapped matrix columns and restore
      the original settings in the gapped alignment structure. */
   sfree(rpsblast_pssms);
   gap_align->positionBased = FALSE;
   RPSPsiMatrixDetach(sbp);

   return status;
}

Int2 
BLAST_ComputeTraceback(EBlastProgramType program_number, 
                       BlastHSPStream* hsp_stream, BLAST_SequenceBlk* query, 
                       BlastQueryInfo* query_info, const BlastSeqSrc* seq_src, 
                       BlastGapAlignStruct* gap_align, 
                       BlastScoringParameters* score_params,
                       const BlastExtensionParameters* ext_params,
                       BlastHitSavingParameters* hit_params,
                       BlastEffectiveLengthsParameters* eff_len_params,
                       const BlastDatabaseOptions* db_options,
                       const PSIBlastOptions* psi_options, 
                       const BlastRPSInfo* rps_info, 
                       SPHIPatternSearchBlk* pattern_blk,
                       BlastHSPResults** results_out)
{
   Int2 status = 0;
   BlastHSPResults* results = NULL;
   BlastHSPList* hsp_list = NULL;
   BlastScoreBlk* sbp;
   Uint1* gen_code_string = NULL;
 
   if (!query_info || !seq_src || !hsp_stream || !results_out) {
      return -1;
   }
   
   /* Set the raw X-dropoff value for the final gapped extension with 
      traceback */
   gap_align->gap_x_dropoff = ext_params->gap_x_dropoff_final;

   sbp = gap_align->sbp;
  
   if (db_options)
      gen_code_string = db_options->gen_code_string;
 

   results = Blast_HSPResultsNew(query_info->num_queries);

   if (program_number == eBlastTypeRpsBlast ||
       program_number == eBlastTypeRpsTblastn) {
       status = 
           s_RPSComputeTraceback(program_number, hsp_stream, seq_src, query, 
                                 query_info, gap_align, score_params, ext_params,
                                 hit_params, rps_info, results);
   } else if ((program_number == eBlastTypeBlastp || 
               program_number == eBlastTypePhiBlastp ||
               program_number == eBlastTypePsiBlast) && 
              (ext_params->options->compositionBasedStats == TRUE || 
               ext_params->options->eTbackExt == eSmithWatermanTbck)) {
      status = 
          Kappa_RedoAlignmentCore(program_number, query, query_info, sbp, 
                                  hsp_stream, seq_src, gen_code_string,
                                  score_params, ext_params, hit_params, 
                                  psi_options, results); 
   } else {
      BlastSeqSrcGetSeqArg seq_arg;
      Uint1 encoding = Blast_TracebackGetEncoding(program_number);
      Boolean perform_traceback = 
         (score_params->options->gapped_calculation && 
          (ext_params->options->ePrelimGapExt != eGreedyWithTracebackExt));
      const Boolean kPhiBlast = (program_number == eBlastTypePhiBlastn ||
                                 program_number == eBlastTypePhiBlastp);

      memset((void*) &seq_arg, 0, sizeof(seq_arg));

      /* Retrieve all HSP lists from the HSPStream. */
      while (BlastHSPStreamRead(hsp_stream, &hsp_list) 
             != kBlastHSPStream_Eof) {

         /* Perform traceback here, if necessary. */
         if (perform_traceback) {
            seq_arg.oid = hsp_list->oid;
            seq_arg.encoding = encoding;
            BlastSequenceBlkClean(seq_arg.seq);
            if (BlastSeqSrcGetSequence(seq_src, (void*) &seq_arg) < 0)
               continue;
            
            if (BlastSeqSrcGetTotLen(seq_src) == 0) {
               /* This is not a database search, so effective search spaces
                * need to be recalculated based on this subject sequence 
                * length.
                * NB: The initial word parameters structure is not available 
                * here, so the small gap cutoff score for linking of HSPs will 
                * not be updated. Since by default linking is done with uneven 
                * gap statistics, this can only influence a corner non-default 
                * case, and is a tradeoff for a benefit of not having to deal 
                * with ungapped extension parameters in the traceback stage.
                */
               if ((status = BLAST_OneSubjectUpdateParameters(program_number, 
                                seq_arg.seq->length, score_params->options, 
                                query_info, sbp, hit_params, 
                                NULL, eff_len_params)) != 0)
                  return status;
            }

            if (kPhiBlast) {
                s_PHITracebackFromHSPList(program_number, hsp_list, query, 
                                          seq_arg.seq, gap_align, sbp, 
                                          score_params, hit_params, 
                                          query_info, pattern_blk);
            } else {
                Blast_TracebackFromHSPList(program_number, hsp_list, query, 
                                           seq_arg.seq, query_info, gap_align,
                                           sbp, score_params, 
                                           ext_params->options, hit_params, 
                                           gen_code_string);
            }

            BlastSeqSrcReleaseSequence(seq_src, (void*)&seq_arg);
         }
         
         /* Free HSP list structure if all HSPs have been deleted. */
         if (hsp_list->hspcnt == 0) {
             hsp_list = Blast_HSPListFree(hsp_list);
             continue;
         }

         Blast_HSPResultsInsertHSPList(results, hsp_list, 
                                       hit_params->options->hitlist_size);
      }
      BlastSequenceBlkFree(seq_arg.seq);
   }

   if (hit_params->options->culling_limit > 0)
      Blast_HSPResultsPerformCulling(results, query_info, 
                                    hit_params->options->culling_limit,
                                    query->length);

   /* Re-sort the hit lists according to their best e-values, because
      they could have changed. Only do this for a database search. */
   if (BlastSeqSrcGetTotLen(seq_src) > 0)
      Blast_HSPResultsSortByEvalue(results);

   /* Eliminate extra hits from results, if preliminary hit list size is larger
      than the final hit list size */
   if (hit_params->options->hitlist_size < 
       hit_params->options->prelim_hitlist_size)
      s_BlastPruneExtraHits(results, hit_params->options->hitlist_size);

   *results_out = results;

   return status;
}

Int2 
Blast_RunTracebackSearch(EBlastProgramType program, 
   BLAST_SequenceBlk* query, BlastQueryInfo* query_info, 
   const BlastSeqSrc* seq_src, const BlastScoringOptions* score_options,
   const BlastExtensionOptions* ext_options,
   const BlastHitSavingOptions* hit_options,
   const BlastEffectiveLengthsOptions* eff_len_options,
   const BlastDatabaseOptions* db_options, 
   const PSIBlastOptions* psi_options, BlastScoreBlk* sbp,
   BlastHSPStream* hsp_stream, const BlastRPSInfo* rps_info,
   SPHIPatternSearchBlk* pattern_blk, BlastHSPResults** results)
{
   Int2 status = 0;
   BlastScoringParameters* score_params = NULL; /**< Scoring parameters */
   BlastExtensionParameters* ext_params = NULL; /**< Gapped extension 
                                                   parameters */
   BlastHitSavingParameters* hit_params = NULL; /**< Hit saving parameters*/
   BlastEffectiveLengthsParameters* eff_len_params = NULL; /**< Parameters
                                        for effective lengths calculations */
   BlastGapAlignStruct* gap_align = NULL; /**< Gapped alignment structure */

   status = 
      BLAST_GapAlignSetUp(program, seq_src, score_options, eff_len_options, 
         ext_options, hit_options, query_info, sbp, &score_params, 
         &ext_params, &hit_params, &eff_len_params, &gap_align);
   if (status)
      return status;

   /* Prohibit any subsequent writing to the HSP stream. */
   BlastHSPStreamClose(hsp_stream);

   status = 
      BLAST_ComputeTraceback(program, hsp_stream, query, query_info,
                             seq_src, gap_align, score_params, ext_params, 
                             hit_params, eff_len_params, db_options, psi_options,
                             rps_info, pattern_blk, results);

   /* Do not destruct score block here */
   gap_align->sbp = NULL;
   BLAST_GapAlignStructFree(gap_align);

   score_params = BlastScoringParametersFree(score_params);
   hit_params = BlastHitSavingParametersFree(hit_params);
   ext_params = BlastExtensionParametersFree(ext_params);
   eff_len_params = BlastEffectiveLengthsParametersFree(eff_len_params);
   return status;
}
