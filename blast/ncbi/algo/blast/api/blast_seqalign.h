/* $Id: blast_seqalign.h,v 1.24 2005/04/06 23:27:53 dondosha Exp $
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

/** @file blast_seqalign.h
 * Functions to convert BLAST results to the SeqAlign form
 */

#ifndef __BLAST_SEQALIGN__
#define __BLAST_SEQALIGN__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NCBI_C_TOOLKIT
#define NCBI_C_TOOLKIT
#endif

#include <readdb.h>
#include <algo/blast/core/blast_hits.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** This should be defined somewhere else!!!!!!!!!!!!! */
#define BLAST_SMALL_GAPS 0

/** Convert BLAST results structure to a list of SeqAlign's.
 * @param program_number Type of BLAST program [in]
 * @param results The BLAST results [in]
 * @param query_slp List of query SeqLoc's [in]
 * @param rdfp Pointer to a BLAST database structure [in]
 * @param subject_slp List of subject sequences locations [in]
 * @param is_gapped Is this a gapped alignment search? [in]
 * @param is_ooframe Is this a search with out-of-frame gapping? [in]
 * @param head_seqalign List of SeqAlign's [out]
 */
Int2 BLAST_ResultsToSeqAlign(EBlastProgramType program_number, 
        BlastHSPResults* results, SeqLocPtr query_slp, 
        ReadDBFILE* rdfp, SeqLoc* subject_slp, 
        Boolean is_gapped, Boolean is_ooframe, SeqAlignPtr* head_seqalign);

/** Given an internal edit block structure, returns the segments information in
 * form of arrays.
 * @param hsp HSP structure containing traceback for one local alignment [in]
 * @param curr_in Link in editing script where to start collecting the data. [in]
 * @param numseg Number of segments in the alignment [in]
 * @param query_length Length of query sequence [in]
 * @param subject_length Length of subject sequence [in]
 * @param translate1 Is query translated? [in]
 * @param translate2 Is subject translated? [in]
 * @param start_out Array of segment starting offsets [out]
 * @param length_out Array of segment lengths [out]
 * @param strands_out Array of segment strands [out]
 * @param start1 Starting query offset; modified to point to next starting
 *               offset if one edit block combines multiple alignments, like
 *               in an ungapped search. [in] [out]
 * @param start2 Starting subject offset for this alignment. [in] [out]
 * @return Status.
 */
Int2 
GapCollectDataForSeqalign(BlastHSP* hsp, GapEditScript* curr_in, Int4 numseg, 
                          Int4 query_length, Int4 subject_length,
                          Boolean translate1, Boolean translate2,
                          Int4** start_out, Int4** length_out,
                          Uint1** strands_out, Int4* start1, Int4* start2);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* !__BLAST_SEQALIGN__ */

