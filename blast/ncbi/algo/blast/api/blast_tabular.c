/* $Id: blast_tabular.c,v 1.27 2005/04/19 13:56:48 kans Exp $
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

/** @file blast_tabular.c
 * On-the-fly tabular formatting of BLAST results
 */

static char const rcsid[] = "$Id: blast_tabular.c,v 1.27 2005/04/19 13:56:48 kans Exp $";

#include <algo/blast/api/blast_tabular.h>
#include <algo/blast/core/blast_util.h>
#include <algo/blast/core/blast_setup.h>
#include <algo/blast/core/blast_engine.h>
#include <algo/blast/core/blast_traceback.h>
#include <algo/blast/api/blast_format.h>
#include <algo/blast/api/blast_seqalign.h>
#include <algo/blast/core/blast_seqsrc_impl.h>

#include <txalign.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

BlastTabularFormatData*
BlastTabularFormatDataNew(FILE* outfp, SeqLoc* query_seqloc,
                          EBlastTabularFormatOptions format_option)
{
   BlastTabularFormatData* tf_data = 
      (BlastTabularFormatData*) calloc(1, sizeof(BlastTabularFormatData));
   tf_data->outfp = outfp;
   tf_data->query_slp = query_seqloc;
   tf_data->format_options = format_option;

   return tf_data;
}

Int2
Blast_TabularFormatDataSetUp(BlastTabularFormatData* tf_data,
                             EBlastProgramType program, 
                             BlastHSPStream* hsp_stream, 
                             const BlastSeqSrc* seq_src, 
                             BLAST_SequenceBlk* query, 
                             BlastQueryInfo* query_info,
                             const BlastScoringOptions* score_options, 
                             BlastScoreBlk* sbp,
                             const BlastEffectiveLengthsOptions* eff_len_options,
                             const BlastExtensionOptions* ext_options,
                             const BlastHitSavingOptions* hit_options,
                             const BlastDatabaseOptions* db_options)
{
    Int2 status = 0;

    ASSERT(score_options && db_options);

    tf_data->perform_traceback =
        (score_options->gapped_calculation && 
         ext_options->ePrelimGapExt != eGreedyWithTracebackExt);
    
    tf_data->program = program;
    tf_data->hsp_stream = hsp_stream;
    tf_data->query = query;
    tf_data->gen_code_string = db_options->gen_code_string;
    /* Sequence source must be copied, to guarantee multi-thread safety. */
    tf_data->seq_src = BlastSeqSrcCopy(seq_src);
    /* Effective lengths must be duplicated in query info structure, because
       they might be changing in the preliminary search. */
    tf_data->query_info = BlastQueryInfoDup(query_info);
    
    /* If traceback will have to be performed before tabular output, 
       do the preparation for it here. */
    if (tf_data->perform_traceback) {
        status = 
            BLAST_GapAlignSetUp(program, seq_src, score_options, 
                                eff_len_options, ext_options, hit_options, 
                                tf_data->query_info, sbp, &tf_data->score_params,
                                &tf_data->ext_params, &tf_data->hit_params, 
                                &tf_data->eff_len_params, &tf_data->gap_align);
        tf_data->gap_align->gap_x_dropoff = tf_data->ext_params->gap_x_dropoff_final;
    }
    return status;
}

void
BlastTabularFormatDataClean(BlastTabularFormatData* tf_data)
{
    if (!tf_data)
        return;

    /* Free the structures that have been allocated internally */
    tf_data->query_info = BlastQueryInfoFree(tf_data->query_info);
    tf_data->score_params = BlastScoringParametersFree(tf_data->score_params);
    tf_data->ext_params = BlastExtensionParametersFree(tf_data->ext_params);
    tf_data->hit_params = BlastHitSavingParametersFree(tf_data->hit_params);
    tf_data->eff_len_params = 
        BlastEffectiveLengthsParametersFree(tf_data->eff_len_params);
    tf_data->gap_align = BLAST_GapAlignStructFree(tf_data->gap_align);
    tf_data->seq_src = BlastSeqSrcFree(tf_data->seq_src);
}

BlastTabularFormatData* 
BlastTabularFormatDataFree(BlastTabularFormatData* tf_data)
{
    if (!tf_data)
        return NULL;

    /* Free the internal structures, if they haven't been freed earlier. */
    BlastTabularFormatDataClean(tf_data);

    sfree(tf_data);
    return tf_data;
}

/** Creates nucleotide sequence buffers corresponding to a local alignment.
 * Used in tabular output with "print sequences" option.
 * @param program Type of BLAST program [in]
 * @param hsp Internal HSP structure [in]
 * @param query_seq Query sequence in blastna encoding. [in]
 * @param subject_seq Subject sequence in blastna encoding [in]
 * @param query_length Length of query sequence [in]
 * @param subject_length Length of subject sequence [in]
 * @param query_buffer Preallocated buffer for text query sequence [in] [out]
 * @param subject_buffer Preallocated buffer for text subject sequence [in] [out]
 */
static void 
FillNuclSequenceBuffers(EBlastProgramType program, BlastHSP* hsp, 
                        Uint1* query_seq, Uint1* subject_seq, Int4 query_length, 
                        Int4 subject_length, char* query_buffer, 
                        char* subject_buffer)
{
   Int4 index, index1;
   const char* blastna_to_iupacna     = "ACGTRYMKWSBDHVN-";
   const char* blastna_to_iupacna_rev = "TGCAYRKMSWVHDBN-"; 
   Uint1* query_ptr;
   Uint1* subject_ptr;
   Int4 numseg;
   Int4* starts;
   Int4* lengths;
   Int4 offset;
   GapEditScript* esp;
   Int4 start1, start2;
   char* buffer;
   Boolean reverse;
   Boolean translate1, translate2;

   translate1 = (program == eBlastTypeBlastx || program == eBlastTypeTblastx ||
                 program == eBlastTypeRpsTblastn);
   translate2 = (program == eBlastTypeTblastn || program == eBlastTypeTblastx);

   reverse = (hsp->query.frame != hsp->subject.frame);

   /* Calculate number of segments. */
   esp = hsp->gap_info;
   for (numseg = 0; esp; esp = esp->next, ++numseg);
   /* Find the starts and lengths of each segment. */
   start1 = hsp->query.offset;
   start2 = hsp->subject.offset;
   GapCollectDataForSeqalign(hsp, hsp->gap_info, numseg, query_length,
                             subject_length, translate1, translate2,
                             &starts, &lengths, NULL, &start1, &start2);

   offset = 0;
   if (!reverse) {
      for (index = 0; index < numseg; ++index) {
         buffer = &query_buffer[offset];
         if (starts[2*index] != -1) {
            query_ptr = &query_seq[starts[2*index]];
            for (index1 = 0; index1 < lengths[index]; ++index1) {
               *buffer = blastna_to_iupacna[*query_ptr];
               buffer++;
               query_ptr++;
            }
         } else {
            memset(buffer, '-', lengths[index]);
         }
         buffer = &subject_buffer[offset];
         if (starts[2*index+1] != -1) {
            subject_ptr = &subject_seq[starts[2*index+1]];
            for (index1 = 0; index1 < lengths[index]; ++index1) {
               *buffer = blastna_to_iupacna[*subject_ptr];
               buffer++;
               subject_ptr++;
            }
         } else {
            memset(buffer, '-', lengths[index]);
         }
         offset += lengths[index];
      }
   } else {
      for (index = numseg-1; index >=0; --index) {
         buffer = &query_buffer[offset];
         if (starts[2*index] != -1) {
            query_ptr = &query_seq[starts[2*index]];
            for (index1 = 0; index1 < lengths[index]; ++index1) {
               *buffer = blastna_to_iupacna[*query_ptr];
               buffer++;
               query_ptr++;
            }
         } else {
            memset(buffer, '-', lengths[index]);
         }
         buffer = &subject_buffer[offset];
         if (starts[2*index+1] != -1) {
            subject_ptr = &subject_seq[starts[2*index+1]+lengths[index]-1];
            for (index1 = 0; index1 < lengths[index]; ++index1) {
               *buffer = blastna_to_iupacna_rev[*subject_ptr];
               buffer++;
               subject_ptr--;
            }
         } else {
            memset(buffer, '-', lengths[index]);
         }
         offset += lengths[index];
      }
   }

   sfree(starts);
   sfree(lengths);
}

/** Maximal buffer length to use for a Seq-id in tabular output. */
#define SEQIDLEN_MAX 255

void* Blast_TabularFormatThread(void* data) 
{
   BlastTabularFormatData* tf_data;
   EBlastProgramType program;
   BlastHSPList* hsp_list = NULL;
   BlastSeqSrc* seq_src;
   BLAST_SequenceBlk* query = NULL; 
   BlastQueryInfo* query_info = NULL;
   BlastScoringParameters* score_params = NULL;
   BlastExtensionParameters* ext_params = NULL;
   BlastHitSavingParameters* hit_params = NULL;
   BlastEffectiveLengthsParameters* eff_len_params = NULL;
   Uint1* gen_code_string = NULL;
   BlastGapAlignStruct* gap_align = NULL;
   Int4 query_index, index;
   char* query_buffer = NULL;
   char* subject_buffer = NULL;
   Int4 q_start=0, q_end=0, s_start=0, s_end=0;
   SeqLoc* slp;
   char bit_score_buff[10], eval_buff[10];
   char* eval_buff_ptr = NULL;
   BlastHSP* hsp;
   SeqId** query_id_array = NULL;
   SeqId* subject_id = NULL;
   Int4 align_length = 0;
   Int4 num_gaps = 0, num_gap_opens = 0, num_mismatches = 0;
   double perc_ident = 0;
   BlastSeqSrcGetSeqArg seq_arg;
   Boolean one_seq_update_params;
   ReadDBFILE* rdfp = NULL;
   char* descr;
   Int4 num_queries;
   Int4* query_lengths;
 
   tf_data = (BlastTabularFormatData*) data;
   if (!tf_data || !tf_data->query_slp || !tf_data->hsp_stream ||
       !tf_data->seq_src || !tf_data->outfp) 
      return NULL;

   program = tf_data->program;
   seq_src = tf_data->seq_src;
   query = tf_data->query;
   query_info = tf_data->query_info;

   seq_arg.seq = NULL;
   seq_arg.oid = 0;

   if (tf_data->perform_traceback) {
      score_params = tf_data->score_params;
      ext_params = tf_data->ext_params;
      hit_params = tf_data->hit_params;
      eff_len_params = tf_data->eff_len_params;
      gap_align = tf_data->gap_align;
      gen_code_string = tf_data->gen_code_string;
      seq_arg.encoding = Blast_TracebackGetEncoding(program);
   }

   num_queries = ValNodeLen(tf_data->query_slp);
   query_id_array = (SeqId**) malloc(num_queries*sizeof(SeqId*));
   query_lengths = (Int4*) malloc(num_queries*sizeof(Int4));

   for (index = 0, slp = tf_data->query_slp; slp; ++index, slp = slp->next) {
      query_id_array[index] = SeqLocId(slp);
      query_lengths[index] = SeqLocLen(slp);
   }

   one_seq_update_params = (BlastSeqSrcGetTotLen(seq_src) == 0);

   /* The line below shouldn't have to access the BlastSeqSrc's data structure
    * FIXME*/
   rdfp = (ReadDBFILE*) _BlastSeqSrcImpl_GetDataStructure(seq_src);

   while (BlastHSPStreamRead(tf_data->hsp_stream, &hsp_list) 
          != kBlastHSPStream_Eof) {
       Int4 subject_length; 
      if (!hsp_list) {
         /* This should not happen, but just in case */
         continue;
      }

      /* Perform traceback if necessary */
      if (tf_data->perform_traceback) {
         seq_arg.oid = hsp_list->oid;
         if (BlastSeqSrcGetSequence(seq_src, (void*) &seq_arg) < 0)
             continue;
         
         if (one_seq_update_params) {
            Int2 status;
            /* This is not a database search, so effective search spaces
               need to be recalculated based on this subject sequence length */
            if ((status = BLAST_OneSubjectUpdateParameters(program, 
                             seq_arg.seq->length, 
                             score_params->options, 
                             query_info, gap_align->sbp, 
                             hit_params, NULL, 
                             eff_len_params)) != 0) {
               hsp_list = Blast_HSPListFree(hsp_list);
               BlastSeqSrcReleaseSequence(seq_src, (void*)&seq_arg);
               continue;
            }
         }

         Blast_TracebackFromHSPList(program, hsp_list, query,
            seq_arg.seq, query_info, gap_align, gap_align->sbp, score_params,
            ext_params->options, hit_params, gen_code_string);
         /* Return subject sequence unless it is needed for the sequence
            printout */
         if (tf_data->format_options != eBlastTabularAddSequences)
            BlastSeqSrcReleaseSequence(seq_src, (void*)&seq_arg);
         /* Recalculate the bit scores, since they might have changed. */
         Blast_HSPListGetBitScores(hsp_list, 
            score_params->options->gapped_calculation, gap_align->sbp);
      }

      if (!readdb_get_descriptor(rdfp, hsp_list->oid, &subject_id, &descr)) {
          subject_buffer = strdup("Unknown");
      } else if (subject_id->choice != SEQID_GENERAL ||
                 strcmp(((DbtagPtr)subject_id->data.ptrvalue)->db, 
                        "BL_ORD_ID")) {
         /* All cases except when database was formatted without seqid indices. 
            In that case all real Seq-id information is hidden in the 
            description. */
         if (tf_data->show_gi || tf_data->show_accession) {
            Blast_SeqIdGetDefLine(subject_id, &subject_buffer, 
                                  tf_data->show_gi, tf_data->show_accession); 
         } else {
            if ( !(subject_buffer = (char*) malloc(sizeof(char)*SEQIDLEN_MAX)))
               return NULL;
            SeqIdWrite(subject_id, subject_buffer, PRINTID_FASTA_LONG, 
                       SEQIDLEN_MAX-1);
         }
         /* Found something for the seqid buffer; description can be 
            discarded now. */
         if (subject_buffer != NULL)
            sfree(descr);
      }
      if (subject_id)
         subject_id = SeqIdSetFree(subject_id);

      /* Last chance to assign anything - take the first token from the 
         description. */
      if (!subject_buffer && descr)
         subject_buffer = strtok(descr, " \t\n\r");

      /* Retrieve the subject sequence if it is needed and this has not 
	 already been done. */ 
      if (tf_data->format_options == eBlastTabularAddSequences && 
          !tf_data->perform_traceback) {
          seq_arg.oid = hsp_list->oid;
          seq_arg.encoding = BLASTNA_ENCODING;
          if (BlastSeqSrcGetSequence(seq_src, (void*) &seq_arg) < 0)
              continue;
      }

      subject_length = BlastSeqSrcGetSeqLen(seq_src, (void*)&hsp_list->oid);

      for (index = 0; index < hsp_list->hspcnt; ++index) {
         char* query_buffer_ptr=NULL;
         hsp = hsp_list->hsp_array[index];
         query_index = 
            Blast_GetQueryIndexFromContext(hsp->context, program);
         Blast_SeqIdGetDefLine(query_id_array[query_index], &query_buffer, 
                               tf_data->show_gi, tf_data->show_accession);
         
         eval_buff_ptr = eval_buff;
         ScoreAndEvalueToBuffers(hsp->bit_score, hsp->evalue, 
                                 bit_score_buff, &eval_buff_ptr, 0);
         
         /* Calculate percentage of identities */
         Blast_HSPCalcLengthAndGaps(hsp, &align_length, &num_gaps, 
                                    &num_gap_opens);
         perc_ident = ((double)hsp->num_ident)/align_length * 100;
         num_mismatches = align_length - hsp->num_ident - num_gaps;
         
         Blast_HSPGetAdjustedOffsets(program, hsp, query_lengths[query_index],
                                     subject_length, &q_start, &q_end, 
                                     &s_start, &s_end);
         
         query_buffer_ptr = query_buffer;
         if (strstr(query_buffer, "lcl|") == query_buffer)
            query_buffer_ptr += 4;


         if (tf_data->format_options == eBlastTabularAddSequences) {
            char* query_seq_buffer = NULL, *subject_seq_buffer = NULL;
            Uint1* query_seq = NULL;
            Int4 context;
            context = hsp->context - (hsp->context % 2);
            query_seq =
                & query->sequence[query_info->contexts[context].query_offset];
            
            query_seq_buffer = MemNew((align_length+1));
            subject_seq_buffer = MemNew((align_length+1));

            FillNuclSequenceBuffers(program, hsp, query_seq, 
                                    seq_arg.seq->sequence, 
                                    query_lengths[query_index], 
                                    seq_arg.seq->length, query_seq_buffer, 
                                    subject_seq_buffer);
            
            fprintf(tf_data->outfp, 
                    "%s\t%s\t%.2f\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%s\t%s\t%s\t%s\n",
                    query_buffer_ptr, subject_buffer, perc_ident, 
                    (long) align_length, (long) num_mismatches, 
                    (long) num_gap_opens, (long) q_start, (long) q_end, 
                    (long) s_start, (long) s_end, eval_buff, bit_score_buff,
                    query_seq_buffer, subject_seq_buffer);
            sfree(query_seq_buffer);
            sfree(subject_seq_buffer);
         } else {
            fprintf(tf_data->outfp, 
                    "%s\t%s\t%.2f\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%s\t%s\n",
                    query_buffer_ptr, subject_buffer, perc_ident, 
                    (long) align_length, (long) num_mismatches, 
                    (long) num_gap_opens, (long) q_start, (long) q_end, 
                    (long) s_start, (long) s_end, eval_buff, bit_score_buff);
         }
         sfree(query_buffer);
      }

      /* Return the subject sequence */
      BlastSeqSrcReleaseSequence(seq_src, (void*)&seq_arg);
      fflush(tf_data->outfp);
      sfree(subject_buffer);
      hsp_list = Blast_HSPListFree(hsp_list);
   }

   BlastSequenceBlkFree(seq_arg.seq);

   sfree(query_lengths);
   sfree(query_id_array);

   return NULL;
}
/* @} */

