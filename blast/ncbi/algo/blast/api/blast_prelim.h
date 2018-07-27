/* $Id: blast_prelim.h,v 1.6 2005/04/06 23:27:53 dondosha Exp $
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

/** @file blast_prelim.h
 * Preliminary stage of a BLAST search performed by one of the threads
 * in a multi-threaded search.
 */

#ifndef __BLAST_PRELIM__
#define __BLAST_PRELIM__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NCBI_C_TOOLKIT
#define NCBI_C_TOOLKIT
#endif

#include <algo/blast/core/blast_hits.h>
#include <algo/blast/core/lookup_wrap.h>
#include <algo/blast/core/blast_seqsrc.h>
#include <algo/blast/core/blast_hspstream.h>
#include <algo/blast/core/blast_diagnostics.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Data structure containing all information necessary for performing 
 * preliminary stage of a BLAST search.
 */
typedef struct BlastPrelimSearchThreadData {
   EBlastProgramType program; /**< BLAST program type */
   BLAST_SequenceBlk* query; /**< Query sequence */
   BlastQueryInfo* query_info; /**< Query information, including context
                                  offsets and effective lengths. */
   BlastSeqSrc* seq_src; /**< Source of the subject sequences */
   LookupTableWrap* lut; /**< Lookup table for finding initial seeds. */
   const BlastScoringOptions* score_options; /**< Options for scoring
                                                  alignments. */
   const BlastInitialWordOptions* word_options; /**< Options for ungapped 
                                                 extension of initial seeds. */
   const BlastExtensionOptions* ext_options; /**< Gapped extension options. */
   const BlastHitSavingOptions* hit_options; /**< Hit saving options. */
   const BlastEffectiveLengthsOptions* eff_len_options; /**< Options specifying 
                                                           effective lengths */
   const PSIBlastOptions* psi_options; /**< Options specific to PSI BLAST. */
   const BlastDatabaseOptions* db_options; /**< Database options - genetic 
                                                code */
   BlastScoreBlk* sbp; /**< Scoring block, containing Karlin-Altschul 
                            parameters. */
   BlastDiagnostics* diagnostics; /**< Search diagnostic data, 
                                       e.g. hit counts. */
   BlastHSPStream* hsp_stream; /**< Source of the BLAST results */
} BlastPrelimSearchThreadData;

/** Initialize preliminary search thread data structure.
 * @param program BLAST program [in]
 * @param query Query sequence(s) structure [in]
 * @param query_info Query information [in]
 * @param seq_src Subject sequences source [in]
 * @param lut Lookup table [in]
 * @param score_options Scoring options [in]
 * @param word_options Initial word finding and ungapped extension options [in]
 * @param ext_options Gapped extension options [in]
 * @param hit_options Hit saving options [in]
 * @param eff_len_options Effective lengths calculation options [in]
 * @param psi_options PSI BLAST options [in]
 * @param db_options Database options [in]
 * @param sbp Statistical parameters block [in]
 * @param diagnostics Diagnostical data returned from search [in]
 * @param hsp_stream Stream for saving HSP lists [in]
 * @return Initialized structure.
 */
BlastPrelimSearchThreadData* 
BlastPrelimSearchThreadDataInit(EBlastProgramType program,
   BLAST_SequenceBlk* query, BlastQueryInfo* query_info,
   const BlastSeqSrc* seq_src, LookupTableWrap* lut, 
   const BlastScoringOptions* score_options, 
   const BlastInitialWordOptions* word_options,
   const BlastExtensionOptions* ext_options, 
   const BlastHitSavingOptions* hit_options,
   const BlastEffectiveLengthsOptions* eff_len_options,
   const PSIBlastOptions* psi_options, 
   const BlastDatabaseOptions* db_options,
   BlastScoreBlk* sbp, BlastDiagnostics* diagnostics,
   BlastHSPStream* hsp_stream);

/** Free the preliminary search thread data structure and all its internally 
 * allocated substructures. 
 */
BlastPrelimSearchThreadData* 
BlastPrelimSearchThreadDataFree(BlastPrelimSearchThreadData* data);

/** Driver for the thread producing tabular output.
 * @param data Pointer to the BlastPrelimSearchThreadData structure. [in]
 */
void* Blast_PrelimSearchThreadRun(void* data);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* !__BLAST_PRELIM__ */

