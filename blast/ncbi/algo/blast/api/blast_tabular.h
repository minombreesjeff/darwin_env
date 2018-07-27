/* $Id: blast_tabular.h,v 1.12 2005/04/06 23:27:53 dondosha Exp $
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

/** @file blast_tabular.h
 * Functions needed for formatting of BLAST results
 */

#ifndef __BLAST_TABULAR__
#define __BLAST_TABULAR__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NCBI_C_TOOLKIT
#define NCBI_C_TOOLKIT
#endif

#include <ncbi.h>
#include <asn.h>
#include <algo/blast/core/blast_hits.h>
#include <algo/blast/core/lookup_wrap.h>
#include <algo/blast/core/blast_seqsrc.h>
#include <algo/blast/core/blast_hspstream.h>
#include <algo/blast/core/blast_gapalign.h>
#include <objloc.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Tabular formatting options. */
typedef enum {
   eBlastTabularDefault=1,
   eBlastTabularAddSequences
} EBlastTabularFormatOptions;

/** Data structure containing all information necessary for production of the
 * tabular output.
 */
typedef struct BlastTabularFormatData {
   EBlastProgramType program; /**< Type of BLAST program */
   BlastHSPStream* hsp_stream; /**< Source of the BLAST results */
   BlastSeqSrc* seq_src; /**< Source of the subject sequences */
   BLAST_SequenceBlk* query; /**< Query sequence */
   BlastQueryInfo* query_info; /**< Query information, including context
                                  offsets and effective lengths. */
   BlastScoringParameters* score_params; /**< Scoring parameters. */
   BlastExtensionParameters* ext_params; /**< Gapped extension parameters. */
   BlastHitSavingParameters* hit_params; /**< Hit saving parameters. */
   BlastEffectiveLengthsParameters* eff_len_params; /**< Effective lengths 
                                                         parameters. */
   Uint1* gen_code_string; /**< Database genetic code string. */
   BlastGapAlignStruct* gap_align; /**< Auxiliary structure used for gapped 
                                        alignment. */
   SeqLoc* query_slp; /**< Source of query sequences identifiers */
   FILE* outfp; /**< Output stream */
   Boolean perform_traceback; /**< Must gapped extension with traceback be
                                 performed before formatting? */
   Boolean show_gi; /**< Show gi's instead of full ids in output, if 
                       possible. */
   Boolean show_accession; /**< Show accessions instead of full ids in output,
                              if possible. This option has lower priority than
                              show_gi. */
   EBlastTabularFormatOptions format_options; /**< Tabular formatting options. */
} BlastTabularFormatData;

/** Allocate the tabular formatting data structure and save the output 
 * stream and formatting option. 
 * @param outfp Output stream to write to [in]
 * @param query_seqloc List of query sequence locations [in]
 * @param format_option What type of tabular output is requested? [in]
 * @return Allocated structure
 */
BlastTabularFormatData*
BlastTabularFormatDataNew(FILE* outfp, SeqLoc* query_seqloc,
                          EBlastTabularFormatOptions format_option);


/** Function initializing the BlastTabularFormatData data structure fields. 
 * @param tf_data Structure to fill. Must be already allocated. [in] [out]
 * @param program Type of BLAST program [in]
 * @param hsp_stream Placeholder for saving HSP lists. [in]
 * @param seq_src Source of subject sequences [in]
 * @param query Structure containing query sequence [in]
 * @param query_info Information about query contexts [in]
 * @param scoring_options Scoring options [in]
 * @param sbp Scoring block with matrix, Karlin-Altschul parameters etc. [in]
 * @param eff_len_options Effective lengths options [in]
 * @param ext_options Gapped extension options [in]
 * @param hit_options Hit saving options [in]
 * @param db_options Structure containing database genetic code option. [in]
 */
Int2
Blast_TabularFormatDataSetUp(BlastTabularFormatData* tf_data,
                             EBlastProgramType program, 
                             BlastHSPStream* hsp_stream, 
                             const BlastSeqSrc* seq_src, 
                             BLAST_SequenceBlk* query, 
                             BlastQueryInfo* query_info,
                             const BlastScoringOptions* scoring_options, 
                             BlastScoreBlk* sbp,
                             const BlastEffectiveLengthsOptions* eff_len_options,
                             const BlastExtensionOptions* ext_options,
                             const BlastHitSavingOptions* hit_options,
                             const BlastDatabaseOptions* db_options);

/** Free the substructures of the tabular formatting data structure that are
 * allocated internally. 
 * @param tf_data Structure to clean. [in]
 */
void 
BlastTabularFormatDataClean(BlastTabularFormatData* tf_data);

/** Free the tabular formatting data structure and all its internally 
 * allocated substructures. 
 * @param tf_data Structure to free. [in]
 * @return NULL.
 */
BlastTabularFormatData* 
BlastTabularFormatDataFree(BlastTabularFormatData* tf_data);

/** Driver for the thread producing tabular output. 
 * @param data Pointer to a BlastTabularFormatData structure. [in]
 */
void* Blast_TabularFormatThread(void* data);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* !__BLAST_TABULAR__ */

