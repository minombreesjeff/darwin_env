/* $Id: blast_api.h,v 1.4 2005/04/27 19:59:26 dondosha Exp $
***************************************************************************
*                                                                         *
*                             COPYRIGHT NOTICE                            *
*                                                                         *
* This software/database is categorized as "United States Government      *
* Work" under the terms of the United States Copyright Act.  It was       *
* produced as part of the author's official duties as a Government        *
* employee and thus can not be copyrighted.  This software/database is    *
* freely available to the public for use without a copyright notice.      *
* Restrictions can not be placed on its present or future use.            *
*                                                                         *
* Although all reasonable efforts have been taken to ensure the accuracy  *
* and reliability of the software and data, the National Library of       *
* Medicine (NLM) and the U.S. Government do not and can not warrant the   *
* performance or results that may be obtained by using this software,     *
* data, or derivative works thereof.  The NLM and the U.S. Government     *
* disclaim any and all warranties, expressed or implied, as to the        *
* performance, merchantability or fitness for any particular purpose or   *
* use.                                                                    *
*                                                                         *
* In any work or product derived from this material, proper attribution   *
* of the author(s) as the source of the software or data would be         *
* appreciated.                                                            *
* =========================================================================
*
* Author:  Ilya Dondoshansky
*
*/

/** @file blast_api.h
 * Functions for C toolkit applications to run BLAST searches using the 
 * rewritten BLAST engine. 
 */

#ifndef _BLAST_API_H_
#define _BLAST_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NCBI_C_TOOLKIT
#define NCBI_C_TOOLKIT
#endif

#include <algo/blast/api/blast_tabular.h>
#include <algo/blast/api/blast_options_api.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Compares a list of SeqLoc's against a BLAST database using the
 * BLAST algorithm.
 * @param query_seqloc List of query Seq-loc's [in]
 * @param db_name Name of a BLAST database to search [in]
 * @param masking_locs Locations in the queries that should be masked [in]
 * @param options Search options [in]
 * @param tf_data Structure to use for on-the-fly tabular formatting [in]
 * @param seqalign_out All results in Seq-align form. [out]
 * @param filter_out Filtering locations [out]
 * @param mask_at_hash Was filtering performed only for lookup table, but not
 *                     for extension? [out]
 * @param extra_returns Additional information about the search [out]
 */
Int2 
Blast_DatabaseSearch(SeqLoc* query_seqloc, char* db_name,
                     SeqLoc* masking_locs,
                     const SBlastOptions* options,
                     BlastTabularFormatData* tf_data,
                     SeqAlign **seqalign_out,
                     SeqLoc** filter_out,
                     Boolean* mask_at_hash,
                     Blast_SummaryReturn* extra_returns);

/** Compares a list of SeqLoc's against another list of SeqLoc's,
 * using the BLAST algorithm, with all options preset.
 * @param query_seqloc List of query Seq-loc's [in]
 * @param subject_seqloc List of subject Seq-loc's [in]
 * @param masking_locs Locations in the queries that should be masked [in]
 * @param options Search options [in]
 * @param tf_data Structure to use for on-the-fly tabular formatting [in]
 * @param seqalign_out All results in Seq-align form. [out]
 * @param filter_out Filtering locations [out]
 * @param mask_at_hash Was filtering performed only for lookup table, but not
 *                     for extension? [out]
 * @param extra_returns Additional information about the search [out]
 */
Int2 
Blast_TwoSeqLocSetsAdvanced(SeqLoc* query_seqloc, 
                            SeqLoc* subject_seqloc,
                            SeqLoc* masking_locs,
                            const SBlastOptions* options,
                            BlastTabularFormatData* tf_data,
                            SeqAlign **seqalign_out,
                            SeqLoc** filter_out,
                            Boolean* mask_at_hash,
                            Blast_SummaryReturn* extra_returns);

/** Compare a list of query SeqLoc's against a source of subject sequences. 
 * @param query_seqloc List of query sequences locations [in]
 * @param seq_src Source of subject sequences [in]
 * @param masking_locs Locations where query sequences should be masked. [in]
 * @param options Search options [in]
 * @param tf_data Tabular formatting data [in]
 * @param results Search results [out]
 * @param filter_out Query locations that were masked (filtered) during the 
 *                   search [out]
 * @param mask_at_hash Was masking performed only for in the lookup table? [out]
 * @param extra_returns Additional search statistits [out]
 * @return 0 on success, -1 on failure.
 */
Int2 
Blast_RunSearch(SeqLoc* query_seqloc, 
                const BlastSeqSrc* seq_src,
                SeqLoc* masking_locs,
                const SBlastOptions* options,
                BlastTabularFormatData* tf_data,
                BlastHSPResults **results,
                SeqLoc** filter_out,
                Boolean* mask_at_hash,
                Blast_SummaryReturn* extra_returns);

/** Run a PHI BLAST search for a query SeqLoc against a database. Return results
 * in a list of ValNode pointers, containing Seq-aligns corresponding to 
 * different pattern occurrences in query.
 * @param query_seqloc List of query sequences locations [in]
 * @param db_name Name of database to search [in]
 * @param masking_locs Locations where query sequences should be masked. [in]
 * @param options Search options [in]
 * @param phivnps List of ValNode pointers with search results, one ValNode per
 *                query pattern occurrence. 
 *                ValNode data points to a Seq-align. [out]
 * @param filter_out Query locations that were masked (filtered) during the 
 *                   search. [out]
 *                   NB: masking at hash is not applicable to PHI BLAST,
 *                   so there is no mask_at_hash output argument.
 * @param extra_returns Additional search statistits [out]
 * @return 0 on success, -1 on failure.
 */
Int2
PHIBlastRunSearch(SeqLoc* query_seqloc, char* db_name, SeqLoc* masking_locs,
                  const SBlastOptions* options, ValNode* *phivnps,
                  SeqLoc** filter_out, Blast_SummaryReturn* extra_returns);

/* @} */

#ifdef __cplusplus
}
#endif

#endif  /* !_BLAST_API_H_ */
