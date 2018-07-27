/*  $Id: seqsrc_multiseq.c,v 1.22 2005/04/27 20:00:36 dondosha Exp $
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
*  Author:  Ilya Dondoshansky
* ===========================================================================*/

/** @file seqsrc_multiseq.c
 * C implementation of the BlastSeqSrc interface for a list of sequence 
 * locations.
 */

static char const rcsid[] = "$Id: seqsrc_multiseq.c,v 1.22 2005/04/27 20:00:36 dondosha Exp $";

#include <algo/blast/api/seqsrc_multiseq.h>
#include <algo/blast/core/blast_seqsrc_impl.h>
#include <algo/blast/core/blast_util.h>
#include <algo/blast/api/blast_seq.h>
#include <sequtil.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Encapsulates the arguments needed to initialize multi-sequence source. */
typedef struct MultiSeqSrcNewArgs {
    SeqLoc* seqloc_list; /**< List of sequence locations. */
    EBlastProgramType program; /**< Type of BLAST program */
} MultiSeqSrcNewArgs;

/** Contains information about all sequences in a set */
typedef struct MultiSeqInfo {
    Boolean is_prot; /**< Are these sequences protein or nucleotide? */
    BLAST_SequenceBlk** seqblk_array; /**< Array of sequence blocks */
    Uint4 max_length; /**< Maximal length of the participating sequences */
    Uint4 avg_length; /**< Average length of the participating sequences */
    Uint4 num_seqs; /**< Number of sequences. */
    Boolean contents_allocated; /**< Is seqblk_array allocated or just copied */
} MultiSeqInfo;

/** Initializes the MultiSeqInfo structure, given a list of SeqLoc's and 
 * program.
 */
static MultiSeqInfo* 
s_MultiSeqInfoNew(const SeqLoc* seqloc_list, EBlastProgramType program)
{
   Uint4 index;
   Uint4 num_seqs = ValNodeLen((ValNode*)seqloc_list);
   Uint4 max_length = 0;
   SeqLoc* seqloc_ptr;
   MultiSeqInfo* retval = (MultiSeqInfo*) calloc(1, sizeof(MultiSeqInfo));

   retval->num_seqs = num_seqs;
   retval->is_prot = 
      (program == eBlastTypeBlastp || program == eBlastTypePhiBlastp ||
       program == eBlastTypePsiBlast || program == eBlastTypeBlastx);
   retval->seqblk_array = (BLAST_SequenceBlk**) 
      calloc(retval->num_seqs, sizeof(BLAST_SequenceBlk*));
      
   for (index = 0, seqloc_ptr = (SeqLoc*) seqloc_list; 
	index < num_seqs; ++index, seqloc_ptr = seqloc_ptr->next) {
       /** @todo FIXME: check the return value and set the call SetInitErrorStr
        * if appropriate */
      BLAST_SetUpSubject(program, seqloc_ptr, &retval->seqblk_array[index]);
      max_length = MAX(max_length, (Uint4)retval->seqblk_array[index]->length);
   }
   retval->max_length = max_length;
   retval->contents_allocated = TRUE;

   return retval;
}

/** Destruct the MultiSeqInfo structure, except the SeqLoc structures, which it
 * does not own.
 */
static MultiSeqInfo* 
s_MultiSeqInfoFree(MultiSeqInfo* seq_info)
{
   Uint4 index;
   
   if (seq_info->contents_allocated) {
      for (index = 0; index < seq_info->num_seqs; ++index) {
         BlastSequenceBlkFree(seq_info->seqblk_array[index]);
      }
      sfree(seq_info->seqblk_array);
   }
   sfree(seq_info);
   return NULL;
}
  
/** Retrieves the length of the longest sequence in the BlastSeqSrc.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int4 
s_MultiSeqGetMaxLength(void* multiseq_handle, void* ignoreme)
{
    Int4 retval = 0;
    Uint4 index;
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;

    ASSERT(seq_info);
    
    if ((retval = seq_info->max_length) > 0)
        return retval;

    for (index=0; index<seq_info->num_seqs; ++index)
        retval = MAX(retval, seq_info->seqblk_array[index]->length);
    seq_info->max_length = retval;

    return retval;
}

/** Retrieves the length of the longest sequence in the BlastSeqSrc.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int4 
s_MultiSeqGetAvgLength(void* multiseq_handle, void* ignoreme)
{
    Int8 total_length = 0;
    Uint4 avg_length;
    Uint4 index;
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;

    ASSERT(seq_info);

    if ((avg_length = seq_info->avg_length) > 0)
        return avg_length;

    for (index=0; index<seq_info->num_seqs; ++index) 
        total_length += (Int8) seq_info->seqblk_array[index]->length;
    avg_length = (Uint4) (total_length / seq_info->num_seqs);
    seq_info->avg_length = avg_length;

    return avg_length;
}

/** Retrieves the number of sequences in the BlastSeqSrc.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int4 
s_MultiSeqGetNumSeqs(void* multiseq_handle, void* ignoreme)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;

    ASSERT(seq_info);
    return seq_info->num_seqs;
}

/** Returns 0 as total length, indicating that this is NOT a database!
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int8 
s_MultiSeqGetTotLen(void* multiseq_handle, void* ignoreme)
{
    return 0;
}

/** Needed for completeness only.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static const char* 
s_MultiSeqGetName(void* multiseq_handle, void* ignoreme)
{
    return NULL;
}

/** Retrieves the date of the BLAST database.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Boolean 
s_MultiSeqGetIsProt(void* multiseq_handle, void* ignoreme)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;

    ASSERT(seq_info);

    return (Boolean) seq_info->is_prot;
}

/** Retrieves the sequence meeting the criteria defined by its second argument.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param args Pointer to BlastSeqSrcGetSeqArg structure [in]
 * @return return codes defined in blast_seqsrc.h
 */
static Int2 
s_MultiSeqGetSequence(void* multiseq_handle, void* args)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;
    BlastSeqSrcGetSeqArg* seq_args = (BlastSeqSrcGetSeqArg*) args;
    Int4 index;

    ASSERT(seq_info);
    ASSERT(args);

    if (seq_info->num_seqs == 0 || !seq_args)
        return BLAST_SEQSRC_ERROR;

    index = seq_args->oid;

    if (index >= (Int4) seq_info->num_seqs)
        return BLAST_SEQSRC_EOF;

    BlastSequenceBlkCopy(&seq_args->seq, seq_info->seqblk_array[index]);
    /* If this is a nucleotide sequence, and it is the traceback stage, 
       we need the uncompressed buffer, stored in the 'sequence_start' 
       pointer. That buffer has a sentinel byte in case of blastn, but
       no sentinel byte for translated programs. */
    if (seq_args->encoding == BLASTNA_ENCODING)
       seq_args->seq->sequence = seq_args->seq->sequence_start + 1;
    else if (seq_args->encoding == NCBI4NA_ENCODING)
       seq_args->seq->sequence = seq_args->seq->sequence_start;

    seq_args->seq->oid = index;
    return BLAST_SEQSRC_SUCCESS;
}

/** Deallocates uncompressed sequence buffer, obtained by MultiSeqGetSequence
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param args Pointer to BlastSeqSrcGetSeqArg structure [in]
 */
static void
s_MultiSeqReleaseSequence(void* multiseq_handle, void* args)
{
    BlastSeqSrcGetSeqArg* seq_args = (BlastSeqSrcGetSeqArg*) args;
    ASSERT(seq_args);
    if (seq_args->seq->sequence_start_allocated)
       sfree(seq_args->seq->sequence_start);
}

/** Retrieve length of a given sequence.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param args Pointer to integer indicating index into the sequences 
 *             vector [in]
 * @return Length of the sequence or BLAST_SEQSRC_ERROR.
 */
static Int4 
s_MultiSeqGetSeqLen(void* multiseq_handle, void* args)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;
    Int4 index;

    ASSERT(seq_info);
    ASSERT(args);

    index = *((Int4*) args);
    return seq_info->seqblk_array[index]->length;
}

/** Gets the next sequence index, given a MultiSeqInfo pointer. */
static Int2 
s_MultiSeqGetNextChunk(void* multiseq_handle, BlastSeqSrcIterator* itr)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;

    ASSERT(itr);

    if (itr->current_pos == UINT4_MAX) {
        itr->current_pos = 0;
    }

    if (itr->current_pos >= seq_info->num_seqs)
        return BLAST_SEQSRC_EOF;

    return BLAST_SEQSRC_SUCCESS;
}

/** Gets the next sequence index, given a BlastSeqSrc pointer. */
static Int4 
s_MultiSeqIteratorNext(void* multiseq_handle, BlastSeqSrcIterator* itr)
{
    Int4 retval = BLAST_SEQSRC_EOF;
    Int2 status = 0;

    ASSERT(multiseq_handle);
    ASSERT(itr);

    if ((status = s_MultiSeqGetNextChunk(multiseq_handle, itr))
        == BLAST_SEQSRC_EOF) {
        return status;
    }
    retval = itr->current_pos++;

    return retval;
}

/** Multi sequence source destructor: frees its internal data structure and the
 * BlastSeqSrc structure itself.
 * @param seq_src BlastSeqSrc structure to free [in]
 * @return NULL
 */
static BlastSeqSrc* 
s_MultiSeqSrcFree(BlastSeqSrc* seq_src)
{
   MultiSeqInfo* seq_info;

    if (!seq_src) 
        return NULL;

    seq_info = (MultiSeqInfo*)_BlastSeqSrcImpl_GetDataStructure(seq_src);

    seq_info = s_MultiSeqInfoFree(seq_info);
    sfree(seq_src);

    return NULL;
}

/** Multi sequence source copier; copies the MultiSeqInfo structure
 * @param seq_src BlastSeqSrc structure to copy [in]
 * @return New BlastSeqSrc structure
 */
static BlastSeqSrc* 
s_MultiSeqSrcCopy(BlastSeqSrc* seq_src)
{
   MultiSeqInfo* seq_info;

   if (!seq_src) 
      return NULL;

   seq_info = (MultiSeqInfo*) BlastMemDup
       (_BlastSeqSrcImpl_GetDataStructure(seq_src), sizeof(MultiSeqInfo));
   seq_info->contents_allocated = FALSE;

   _BlastSeqSrcImpl_SetDataStructure(seq_src, (void*) seq_info);
    
   return seq_src;
}

/** Multi-sequence source constructor 
 * @param retval BlastSeqSrc structure (already allocated) to populate [in]
 * @param args Pointer to MultiSeqSrcNewArgs structure above [in]
 * @return Updated bssp structure (with all function pointers initialized)
 */
static BlastSeqSrc* 
s_MultiSeqSrcNew(BlastSeqSrc* retval, void* args)
{
    MultiSeqSrcNewArgs* seqsrc_args = (MultiSeqSrcNewArgs*) args;
    MultiSeqInfo* seq_info = NULL;

    ASSERT(retval);
    ASSERT(seqsrc_args);
    
    seq_info = 
        s_MultiSeqInfoNew(seqsrc_args->seqloc_list, seqsrc_args->program);
    
    /* Initialize the BlastSeqSrc structure fields with user-defined function
     * pointers and seq_info */
    _BlastSeqSrcImpl_SetDeleteFnPtr(retval, &s_MultiSeqSrcFree);
    _BlastSeqSrcImpl_SetCopyFnPtr(retval, &s_MultiSeqSrcCopy);
    _BlastSeqSrcImpl_SetDataStructure(retval, (void*) seq_info);
    _BlastSeqSrcImpl_SetGetNumSeqs(retval, &s_MultiSeqGetNumSeqs);
    _BlastSeqSrcImpl_SetGetMaxSeqLen(retval, &s_MultiSeqGetMaxLength);
    _BlastSeqSrcImpl_SetGetAvgSeqLen(retval, &s_MultiSeqGetAvgLength);
    _BlastSeqSrcImpl_SetGetTotLen(retval, &s_MultiSeqGetTotLen);
    _BlastSeqSrcImpl_SetGetName(retval, &s_MultiSeqGetName);
    _BlastSeqSrcImpl_SetGetIsProt(retval, &s_MultiSeqGetIsProt);
    _BlastSeqSrcImpl_SetGetSequence(retval, &s_MultiSeqGetSequence);
    _BlastSeqSrcImpl_SetGetSeqLen(retval, &s_MultiSeqGetSeqLen);
    _BlastSeqSrcImpl_SetIterNext(retval, &s_MultiSeqIteratorNext);
    _BlastSeqSrcImpl_SetReleaseSequence(retval, &s_MultiSeqReleaseSequence);
    
    return retval;
}

BlastSeqSrc*
MultiSeqBlastSeqSrcInit(SeqLoc* seqloc_list, EBlastProgramType program)
{
    BlastSeqSrc* seq_src;
    BlastSeqSrcNewInfo bssn_info;
    MultiSeqSrcNewArgs* args =
        (MultiSeqSrcNewArgs*) calloc(1, sizeof(MultiSeqSrcNewArgs));;
    args->seqloc_list = seqloc_list;
    args->program = program;
    bssn_info.constructor = &s_MultiSeqSrcNew;
    bssn_info.ctor_argument = (void*) args;

    seq_src = BlastSeqSrcNew(&bssn_info);
    sfree(args);

    return seq_src;
}

/* @} */

