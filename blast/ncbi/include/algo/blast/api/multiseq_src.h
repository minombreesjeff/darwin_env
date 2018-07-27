/*  $Id: multiseq_src.h,v 1.1 2004/03/13 00:25:29 dondosha Exp $
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
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
* Author:  Ilya Dondoshansky
*
* File Description:
*   C Implementation of the BlastSeqSrc interface for a list of sequence 
*   locations.
*
*/

#ifndef MULTISEQ_SRC_H
#define MULTISEQ_SRC_H

#include <algo/blast/core/blast_seqsrc.h>
#include <algo/blast/core/blast_def.h>
#include <algo/blast/core/blast_message.h>
#include <objloc.h>

/** Encapsulates the arguments needed to initialize multi-sequence source. */
typedef struct MultiSeqSrcNewArgs {
    SeqLoc* seqloc_list;
    Uint1 program;
} MultiSeqSrcNewArgs;

/** Contains information about all sequences in a set */
typedef struct MultiSeqInfo {
   SeqLoc** seqloc_array; /**< Array of sequence locations. Individual 
			   SeqLoc's are not owned by this structure! */
   Boolean is_prot; /**< Are these sequences protein or nucleotide? */
   BLAST_SequenceBlk** seqblk_array; /**< Array of sequence blocks */
   Uint4 max_length;
   Uint4 avg_length;
   Uint4 num_seqs;
   Blast_Message* error_msg;
} MultiSeqInfo;

/** Multi-sequence source constructor 
 * @param seq_src BlastSeqSrc structure (already allocated) to populate [in]
 * @param args Pointer to MultiSeqSrcNewArgs structure above [in]
 * @return Updated bssp structure (with all function pointers initialized
 */
BlastSeqSrc* MultiSeqSrcNew(BlastSeqSrc* seq_src, void* args);

/** Initialize the sequence source structure.
 * @param seq_vector Vector of sequence locations [in]
 * @param program Type of BLAST to be performed [in]
 */
BlastSeqSrc* 
MultiSeqSrcInit(SeqLoc* seqloc_list, Uint1 program);

/** Multi sequence source destructor: frees its internal data structure and the
 * BlastSeqSrc structure itself.
 * @param seq_src BlastSeqSrc structure to free [in]
 * @return NULL
 */
BlastSeqSrc* MultiSeqSrcFree(BlastSeqSrc* seq_src);

#endif /* MULTISEQ_SRC_H */
