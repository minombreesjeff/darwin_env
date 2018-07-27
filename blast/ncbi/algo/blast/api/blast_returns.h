/* $Id: blast_returns.h,v 1.12 2005/04/27 20:00:15 dondosha Exp $
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

/** @file blast_returns.h
 * Manipulation of data returned from BLAST other than Seq-aligns
 */

#ifndef __BLAST_RETURNS__
#define __BLAST_RETURNS__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NCBI_C_TOOLKIT
#define NCBI_C_TOOLKIT
#endif

#include <algo/blast/core/blast_options.h>
#include <algo/blast/core/blast_hits.h>
#include <algo/blast/core/blast_diagnostics.h>   
#include <blfmtutl.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Small structure containing the just those Karlin-Altschul parameters needed
 * for the BLAST formatting */
typedef struct BLAST_KAParameters {
   double Lambda;     /**< Karlin-Altschul scaling parameter. */
   double K;          /**< Karlin-Altschul K parameter. */
   double H;          /**< Karlin-Altschul entropy */
   double C;          /**< Constant factor for PHI BLAST e-values. */ 
} BLAST_KAParameters;

/** Small structure containing some numbers about database length, adjustment, 
 * etc.
 */
typedef struct BLAST_DatabaseStats {
   Int4 dbnum;              /**< number of sequences in database search */
   Int8 dblength;           /**< Length of databases. */
   Int8 eff_dblength;       /**< Length of databases with edge corrections. */
   Int4 qlen;               /**< Length of query with edge corrections. */
   Int4 eff_qlen;           /**< Length of query with edge corrections. */
   Int4 hsp_length;         /**< Expect length of an HSP. */
   Int8 eff_searchsp;       /**< effective search-space */
   Int8 eff_searchsp_used;  /**< effective search-space actually used. */
} Blast_DatabaseStats;

/** Small structure containing relevant search parameters. 
 *  Mostly for use in building XML or footer in BLAST report.
 */
typedef struct Blast_SearchParams {
   Boolean gapped_search;   /**< true if a gapped search. */
   Int4 gap_open;           /**< Cost of gap existence. */
   Int4 gap_extension;      /**< Cost to extend gap by one letter. */
   char* filter_string;     /**< filtering command. */
   char* matrix;            /**< name of matrix (e.g., BLOSUM62) */
   double expect;           /**< expect value cutoff. */
   Int4 match;              /**< reward for a match (blastn only) */
   Int4 mismatch;           /**< penalty for a mismatch (blastn only) */
   char* pattern;           /**< phi-blast pattern used. */
   char* entrez_query;      /**< query to entrez */
   double ethresh;          /**< PSI-BLAST threshold to keep HSPs in profile. */
   Int4 threshold;          /**< threshold to start extension of hits. */
   Int4 window_size;        /**< max allowed distance between hits to init extension in 2-hit mode. */
} Blast_SearchParams;

/** Structure holding all calculated data returned from a BLAST search other
 * than the alignment.
 */
typedef struct Blast_SummaryReturn {
    BLAST_KAParameters* ka_params; /**< Ungapped Karlin-Altschul parameters */
    BLAST_KAParameters* ka_params_gap;/**< Gapped Karlin-Altschul parameters */
    Blast_DatabaseStats* db_stats; /**< database numbers and adjustments */
    Blast_SearchParams* search_params;/**< parameters used in search. */
    BlastDiagnostics* diagnostics; /**< diagnositics from engine. */
    Blast_Message* error;          /**< Error or warning message. */
    SPHIQueryInfo* pattern_info; /**< In PHI BLAST only: information about 
                                          pattern occurrences in query. */
} Blast_SummaryReturn;

/** Retrieves necessary information from a sequence source and fills the 
 * TxDfDbInfo structure.
 * @param rdfp pointer to BLAST db reader [in]
 * @return pointer to info about database.
 */
TxDfDbInfo* Blast_GetDbInfo(ReadDBFILE* rdfp);

/** Formats the BLAST parameters for the BLAST report.
 *
 * @param program_number indicates blastn, blastp, etc. [in]
 * @param sum_return object constructed by Blast_SummaryReturnFill, used 
 *    to fill buffer [in]
 * @return char* with information, newlines indicated by tildes ('~').
 */	
char*
Blast_GetParametersBuffer(EBlastProgramType program_number,
        const Blast_SummaryReturn* sum_return);

/** Fills the summary returns from the provided information.
 * NOTE: either one of rdfp or subject_loc (below) can be NULL,
 * but not both.
 * @param program_number Type of BLAST program [in]
 * @param score_options pointer for scoring options [in]
 * @param sbp Karlin-Altschul/statistics information [in]
 * @param lookup_options pointer for lookup table options [in]
 * @param word_options pointer for word finding options [in]
 * @param ext_options pointer for extension options [in]
 * @param hit_options pointer for hit saving options [in]
 * @param eff_len_options pointer for effective length options [in]
 * @param query_setup pointer for query setup options [in]
 * @param query_info Information about query. Note: in PHI BLAST, the query 
 *                   pattern information pointer is passed on to the sum_returns 
 *                   structure and set to NULL in the query_info within this 
 *                   function. [in]
 * @param seq_src Source of subject sequences [in]
 * @param diagnostics pointer to diagnostic information, SET to NULL
 *    on return [in|out]
 * @param sum_returns Object to be filled in. Must be already allocated. [out]
 * @return zero returned on success
 */
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
        Blast_SummaryReturn* sum_returns);

/** Allocate the Blast_SummaryReturn structure */
Blast_SummaryReturn* Blast_SummaryReturnNew(void);

/** Deallocates memory for the summary returns structure 
 * @param sum_return object to be freed [in]
 * @return NULL pointer
 */
Blast_SummaryReturn* Blast_SummaryReturnFree(Blast_SummaryReturn* sum_return);

/** Frees the contents of the summary returns structure, but not the structure
 * itself.
 * @param sum_returns Structure to clean. [in] [out]
 */
void 
Blast_SummaryReturnClean(Blast_SummaryReturn* sum_returns);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* !__BLAST_FORMAT__ */

