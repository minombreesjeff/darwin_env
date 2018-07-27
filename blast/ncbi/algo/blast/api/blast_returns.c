/* $Id: blast_returns.c,v 1.19 2005/04/27 20:00:15 dondosha Exp $
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

/** @file blast_returns.c
 * Manipulating data returned from BLAST other than Seq-aligns
 */

static char const rcsid[] = "$Id: blast_returns.c,v 1.19 2005/04/27 20:00:15 dondosha Exp $";

#include <algo/blast/api/blast_returns.h>
#include <algo/blast/api/blast_seq.h>
#include <algo/blast/core/blast_filter.h>
#include <algo/blast/core/blast_util.h>
#include <algo/blast/core/pattern.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

TxDfDbInfo* Blast_GetDbInfo(ReadDBFILE* rdfp)
{
   TxDfDbInfo* dbinfo = NULL;
   char* chptr = NULL;

   if (!rdfp)
      return NULL;

   dbinfo = calloc(1, sizeof(TxDfDbInfo));

   dbinfo->name = strdup(readdb_get_filename(rdfp));
      
   if (((chptr = readdb_get_title(rdfp)) == NULL) && dbinfo->name)
      dbinfo->definition = strdup(dbinfo->name);
   else
      dbinfo->definition = strdup(chptr);	
      
   dbinfo->date = strdup(readdb_get_date(rdfp));

   dbinfo->is_protein = readdb_is_prot(rdfp);
     
   readdb_get_totals_ex(rdfp, &dbinfo->total_length, &dbinfo->number_seqs, TRUE);
   
   return dbinfo;
}

char*
Blast_GetParametersBuffer(EBlastProgramType program_number, 
        const Blast_SummaryReturn* sum_returns)
{
   char buffer[128];
   char* ret_buffer=NULL;
   Int2 ret_buffer_length=0;
   BlastUngappedStats* ungapped_stats = NULL;
   BlastGappedStats* gapped_stats = NULL;
   BlastRawCutoffs* raw_cutoffs = NULL;
   Blast_SearchParams* search_params=NULL;
   Blast_DatabaseStats* db_stats = NULL;
   BlastDiagnostics* diagnostics = NULL;
   

   if (sum_returns == NULL)
     return NULL;

   search_params = sum_returns->search_params;
   db_stats = sum_returns->db_stats;
   diagnostics = sum_returns->diagnostics;

   if (diagnostics) {
      ungapped_stats = diagnostics->ungapped_stat;
      gapped_stats = diagnostics->gapped_stat;
      raw_cutoffs = diagnostics->cutoffs;
   }

   if (program_number == eBlastTypeBlastn ||
       program_number == eBlastTypePhiBlastn)
   {
      sprintf(buffer, "Matrix: blastn matrix:%d %d", search_params->match, search_params->mismatch);
      add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
   } 
   else if (search_params->matrix) 
   {
      sprintf(buffer, "Matrix: %s", search_params->matrix);
      add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
   }

   if (search_params->gapped_search) {
      sprintf(buffer, "Gap Penalties: Existence: %ld, Extension: %ld",
              (long) search_params->gap_open, 
              (long) search_params->gap_extension);
      add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
   }
   
   sprintf(buffer, "Number of Sequences: %ld", (long) db_stats->dbnum);
   add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
   if (ungapped_stats) {
      sprintf(buffer, "Number of Hits to DB: %s", 
              Nlm_Int8tostr(ungapped_stats->lookup_hits, 1));
      add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
   
      if (ungapped_stats->init_extends) {
          sprintf(buffer, "Number of extensions: %ld", 
                  (long) ungapped_stats->init_extends);
          add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
      }
      if (ungapped_stats->good_init_extends) {
          sprintf(buffer, "Number of successful extensions: %ld", 
                  (long) ungapped_stats->good_init_extends);
          add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
      }
   }

   if (gapped_stats) {
      if (search_params->expect > 0.1) {
         sprintf(buffer, "Number of sequences better than %4.1f: %ld", 
                 search_params->expect, 
                 (long) gapped_stats->num_seqs_passed);
      } else {
         sprintf(buffer, "Number of sequences better than %3.1e: %ld", 
                 search_params->expect, 
                 (long) gapped_stats->num_seqs_passed);
      }
      add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
   
      if (search_params->gapped_search) {
          if (gapped_stats->seqs_ungapped_passed) {
              sprintf(buffer, 
                      "Number of HSP's better than %4.1f without gapping: %ld", 
                      search_params->expect, 
                      (long) gapped_stats->seqs_ungapped_passed);
              add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
          }
          sprintf(buffer, "Number of HSP's gapped: %ld", 
                  (long) gapped_stats->extensions);
          add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
          sprintf(buffer, "Number of HSP's successfully gapped: %ld", 
                  (long) gapped_stats->good_extensions);
          add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
          if (gapped_stats->extra_extensions) {
              sprintf(buffer, 
                  "Number of extra gapped extensions for HSPs above %4.1f: %ld", 
                  search_params->expect, (long) gapped_stats->extra_extensions);
              add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
          }
      }
   }

   /* Query length makes sense only for single query sequence. */
   if (db_stats->qlen > 0) {
      sprintf(buffer, "Length of query: %ld", (long)db_stats->qlen);
      add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
   }

   sprintf(buffer, "Length of database: %s", Nlm_Int8tostr (db_stats->dblength, 1));
   add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
   
   if (db_stats->qlen > 0) {
      sprintf(buffer, "Length adjustment: %ld", (long) db_stats->hsp_length);
      add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);

      /** FIXME: Should this be different for RPS BLAST? */
      sprintf(buffer, "Effective length of query: %ld", (long)db_stats->eff_qlen);
      add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
      
      sprintf(buffer, "Effective length of database: %s", 
              Nlm_Int8tostr (db_stats->eff_dblength , 1));
      add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
      sprintf(buffer, "Effective search space: %8.0f", 
              (double) db_stats->eff_searchsp);
      add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
      sprintf(buffer, "Effective search space used: %8.0f", (double) db_stats->eff_searchsp_used);
      add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
   }
   if (search_params->threshold) {
       sprintf(buffer, "Neighboring words threshold: %ld", 
               (long) search_params->threshold);
       add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
   }
   if (search_params->window_size) {
       sprintf(buffer, "Window for multiple hits: %ld", 
               (long) search_params->window_size);
       add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
   }
   if (raw_cutoffs) {
      BLAST_KAParameters* ka_params_gap = sum_returns->ka_params_gap;
      BLAST_KAParameters* ka_params = sum_returns->ka_params;

      if (raw_cutoffs->x_drop_ungapped) {
          sprintf(buffer, "X1: %ld (%4.1f bits)", 
                  (long)raw_cutoffs->x_drop_ungapped, 
                  (raw_cutoffs->x_drop_ungapped)*(ka_params->Lambda)/NCBIMATH_LN2);
          add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
      }
      if (search_params->gapped_search) {
         sprintf(buffer, "X2: %ld (%4.1f bits)", 
                 (long)raw_cutoffs->x_drop_gap, raw_cutoffs->x_drop_gap*(ka_params_gap->Lambda)/NCBIMATH_LN2);
         add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
         sprintf(buffer, "X3: %ld (%4.1f bits)", 
                 (long)raw_cutoffs->x_drop_gap_final, 
                 raw_cutoffs->x_drop_gap_final*(ka_params_gap->Lambda) /
                 NCBIMATH_LN2);
         add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
      
         if (raw_cutoffs->ungapped_cutoff) {
             sprintf(buffer, "S1: %ld (%4.1f bits)", 
                     (long)raw_cutoffs->ungapped_cutoff, 
                     ((raw_cutoffs->ungapped_cutoff*(ka_params->Lambda)) -
                      (log(ka_params->K)))/NCBIMATH_LN2);
             add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
         }
      }
      if (raw_cutoffs->cutoff_score) {
          if (search_params->gapped_search) {
              sprintf(buffer, "S2: %ld (%4.1f bits)", 
                      (long) raw_cutoffs->cutoff_score, 
                      (((raw_cutoffs->cutoff_score)*(ka_params_gap->Lambda)) -
                       (log(ka_params_gap->K)))/NCBIMATH_LN2);
          } else {
              sprintf(buffer, "S2: %ld (%4.1f bits)", 
                      (long) raw_cutoffs->cutoff_score, 
                      (((raw_cutoffs->cutoff_score)*(ka_params->Lambda)) - 
                       (log(ka_params->K)))/NCBIMATH_LN2);
          }
          add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
      }
   }

   return ret_buffer;
}

/** Save the Karlin-Altschul parameters calculated in the BLAST search.
 * @param sbp Internal scoring block structure [in]
 * @param context Index into the array of structures containing
 *                Karlin-Altschul parameters [in]
 * @param sum_returns Returns summary structure [out]
*/
static void 
s_SummaryKAParametersFill(const BlastScoreBlk* sbp, Int4 context, 
                          Blast_SummaryReturn* sum_returns)
{
   Blast_KarlinBlk* kbp;

   if (!sbp)
      return;

   if (sbp->kbp) {
      kbp = sbp->kbp[context];
      sum_returns->ka_params = 
         (BLAST_KAParameters*) calloc(1, sizeof(BLAST_KAParameters));
      sum_returns->ka_params->Lambda = kbp->Lambda;
      sum_returns->ka_params->K = kbp->K;
      sum_returns->ka_params->H = kbp->H;
   }

   if (sbp->kbp_gap) {
      kbp = sbp->kbp_gap[context];
      sum_returns->ka_params_gap = 
         (BLAST_KAParameters*) malloc(sizeof(BLAST_KAParameters));
      sum_returns->ka_params_gap->Lambda = kbp->Lambda;
      sum_returns->ka_params_gap->K = kbp->K;
      sum_returns->ka_params_gap->H = kbp->H;
      sum_returns->ka_params_gap->C = kbp->paramC; /* Needed only in PHI BLAST */
   }
}

/** Frees Blast_DatabaseStats object
 * @param db_stats object to be freed [in]
 * @return NULL pointer 
 */
static Blast_DatabaseStats*
s_SummaryDBStatsFree(Blast_DatabaseStats* db_stats)
{
    sfree(db_stats);
    return NULL;
}

/** Allocates and fills in Blast_DatabaseStats
 * @param program_number blastn/blastp/etc. [in]
 * @param eff_len_options pointer to effective length options [in]
 * @param query_info information about query [in]
 * @param seq_src Source of subject sequences [in]
 * @param db_stats object to be returned [out]
 * @return zero on success
 */
static Int2 
s_SummaryDBStatsFill(EBlastProgramType program_number, 
   const BlastEffectiveLengthsOptions* eff_len_options,
   const BlastQueryInfo* query_info, const BlastSeqSrc* seq_src,
   Blast_DatabaseStats** db_stats)
{
   Int8 total_length=0;  /* total length of database. */
   Int4 num_entries=0;   /* number of entries in database. */
   Int4 num_frames;      /* number of frames allowed. */
   

   ASSERT(db_stats);

   *db_stats = (Blast_DatabaseStats*) calloc(1, sizeof(Blast_DatabaseStats));
   if (*db_stats == NULL)
     return -1;

   if (eff_len_options->db_length) {
      total_length = eff_len_options->db_length;
   } else {
      total_length = BlastSeqSrcGetTotLen(seq_src);

      if (total_length == 0) {
         /* If total length cannot be retrieved directly, calculate it by
            adding individual sequence lengths. */
         BlastSeqSrcIterator* itr = BlastSeqSrcIteratorNew();
         Int4 oid;
         while ( (oid = BlastSeqSrcIteratorNext(seq_src, itr)) 
                 != BLAST_SEQSRC_EOF) {
            total_length += BlastSeqSrcGetSeqLen(seq_src, (void*) &oid);
         }
         itr = BlastSeqSrcIteratorFree(itr);
      }
   }
      
   if (program_number == eBlastTypeTblastn || 
       program_number == eBlastTypeRpsTblastn ||
       program_number == eBlastTypeTblastx)
      total_length /= 3;

   (*db_stats)->dblength = total_length;


   if (eff_len_options->dbseq_num)
      num_entries = eff_len_options->dbseq_num;
   else
      num_entries = BlastSeqSrcGetNumSeqs(seq_src);

   (*db_stats)->dbnum = num_entries;

   if (program_number == eBlastTypeBlastx ||
       program_number == eBlastTypeTblastx)
      num_frames = NUM_FRAMES;
   else if (program_number == eBlastTypeBlastn ||
            program_number == eBlastTypePhiBlastn)
      num_frames = 2;
   else
      num_frames = 1;

   
   if (query_info->last_context < num_frames) {  /* Only one query here. */
      Int4 qlen = query_info->contexts[query_info->first_context].query_length;
      (*db_stats)->hsp_length = 
          query_info->contexts[query_info->first_context].length_adjustment;
      /** FIXME: Should this be different for RPS BLAST? */
      (*db_stats)->qlen = qlen;
      (*db_stats)->eff_qlen = qlen - ((*db_stats)->hsp_length);
      (*db_stats)->eff_dblength = 
          total_length - num_entries*((*db_stats)->hsp_length);
      (*db_stats)->eff_searchsp = 
          query_info->contexts[query_info->first_context].eff_searchsp;
      if (eff_len_options && eff_len_options->searchsp_eff)
         (*db_stats)->eff_searchsp_used = eff_len_options->searchsp_eff;
      else
         (*db_stats)->eff_searchsp_used = (*db_stats)->eff_searchsp;
   }

   return 0;
}

/** Free Blast_SearchParams structure and underlying data
 *
 * @param search_params the object to be freed [in]
 * @return NULL pointer 
 */
static Blast_SearchParams*
s_SummarySearchParamsFree(Blast_SearchParams* search_params)
{

   if (search_params == NULL)
      return NULL;

   sfree(search_params->matrix);
   sfree(search_params->entrez_query);
   sfree(search_params->filter_string);
   sfree(search_params->pattern);
   sfree(search_params);

   return NULL;
}

/** Allocated and fills some search parameters.  
 * 
 * @param program_number identifies blastn/blastp/etc. [in]
 * @param score_options pointer to scoring options [in]
 * @param lookup_options pointer to options for lookup table creation [in]
 * @param hit_options options for saving and evaluating hits [in]
 * @param query_setup options for filtering etc. [in]
 * @param word_options options for processing initial hits [in]
 * @param entrez_query limit search by this query [in]
 * @param search_params object to be allocated and filled [out]
 * @return zero on success
 */
static Int2 
s_SummarySearchParamsFill(EBlastProgramType program_number,
   const BlastScoringOptions* score_options,
   const LookupTableOptions* lookup_options,
   const BlastHitSavingOptions* hit_options,
   const QuerySetUpOptions* query_setup, 
   const BlastInitialWordOptions* word_options,
   const char* entrez_query,
   Blast_SearchParams** search_params)
{
   Blast_SearchParams* search_params_lcl;

   ASSERT(search_params);
   ASSERT(score_options && lookup_options && hit_options && query_setup);

   *search_params = search_params_lcl = (Blast_SearchParams*) calloc(1, sizeof(Blast_SearchParams));
   if (search_params_lcl == NULL)
     return -1;

   if (program_number == eBlastTypeBlastn ||
       program_number == eBlastTypePhiBlastn)
   {
      search_params_lcl->match = score_options->reward;
      search_params_lcl->mismatch = score_options->penalty;
   } 
   else if (score_options->matrix) 
   {
      search_params_lcl->matrix = StringSave(score_options->matrix);
   }

   if (score_options->gapped_calculation) 
   {
      search_params_lcl->gapped_search = TRUE;
      search_params_lcl->gap_open = score_options->gap_open;
      search_params_lcl->gap_extension = score_options->gap_extend;
   }
   else
      search_params_lcl->gapped_search = FALSE;

   if (query_setup && query_setup->filter_string)
      search_params_lcl->filter_string = StringSave(query_setup->filter_string);
   else
      search_params_lcl->filter_string = StringSave("F");

   search_params_lcl->expect = hit_options->expect_value;

   if (lookup_options->phi_pattern)
      search_params_lcl->pattern = StringSave(lookup_options->phi_pattern); 

   search_params_lcl->threshold = lookup_options->threshold;

   search_params_lcl->window_size = word_options->window_size;

   if (entrez_query)
      search_params_lcl->entrez_query = StringSave(entrez_query);

   return 0;
}

Blast_SummaryReturn* Blast_SummaryReturnNew()
{
    return (Blast_SummaryReturn*) calloc(1, sizeof(Blast_SummaryReturn));
}

void 
Blast_SummaryReturnClean(Blast_SummaryReturn* sum_returns)
{
   if (sum_returns) {
      sfree(sum_returns->ka_params);
      sfree(sum_returns->ka_params_gap);
      sum_returns->db_stats = s_SummaryDBStatsFree(sum_returns->db_stats);
      sum_returns->search_params = 
          s_SummarySearchParamsFree(sum_returns->search_params);
      sum_returns->diagnostics = Blast_DiagnosticsFree(sum_returns->diagnostics);
      sum_returns->error = Blast_MessageFree(sum_returns->error);
      sum_returns->pattern_info = 
          SPHIQueryInfoFree(sum_returns->pattern_info);
   }
}

Blast_SummaryReturn* 
Blast_SummaryReturnFree(Blast_SummaryReturn* sum_returns)
{
    Blast_SummaryReturnClean(sum_returns);
    sfree(sum_returns);
    return NULL;
}

Int2 Blast_SummaryReturnFill(EBlastProgramType program_number, 
        const BlastScoringOptions* score_options, 
        const BlastScoreBlk* sbp,
        const LookupTableOptions* lookup_options,
        const BlastInitialWordOptions* word_options,
        const BlastExtensionOptions* ext_options,
        const BlastHitSavingOptions* hit_options,
        const BlastEffectiveLengthsOptions* eff_len_options,
        const QuerySetUpOptions* query_setup,
        const BlastQueryInfo* query_info,
        const BlastSeqSrc* seq_src,
        BlastDiagnostics** diagnostics,
        Blast_SummaryReturn* sum_returns)
{
    if (!sum_returns)
        return -1;

   if (score_options == NULL || sbp == NULL || lookup_options == NULL || 
      word_options == NULL || ext_options == NULL || hit_options == NULL ||
      eff_len_options == NULL || query_info == NULL)
     return -1;

   s_SummaryKAParametersFill(sbp, query_info->first_context, sum_returns);
   s_SummaryDBStatsFill(program_number, eff_len_options, query_info, 
                            seq_src, &(sum_returns->db_stats));
   
   s_SummarySearchParamsFill(program_number, score_options,
            lookup_options, hit_options, query_setup, word_options,
            NULL, &(sum_returns->search_params));
   if (diagnostics)
   {
      sum_returns->diagnostics = *diagnostics;
      *diagnostics = NULL;
   }

   if (program_number == eBlastTypePhiBlastn || 
       program_number == eBlastTypePhiBlastp) {
       /* Copy the pattern information structure. */
       sum_returns->pattern_info = 
           SPHIQueryInfoCopy(query_info->pattern_info);
   }

   return 0;
}

/* @} */

