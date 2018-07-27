/*  $Id: multiseq_src.c,v 1.5 2004/04/28 19:50:02 dondosha Exp $
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
*   C implementation of the BlastSeqSrc interface for a list of sequence 
*   locations.
*
*/

static char const rcsid[] = "$Id: multiseq_src.c,v 1.5 2004/04/28 19:50:02 dondosha Exp $";

#include <algo/blast/api/multiseq_src.h>
#include <algo/blast/core/blast_util.h>
#include <algo/blast/api/blast_seq.h>
#include <sequtil.h>

static MultiSeqInfo* MultiSeqInfoNew(const SeqLoc* seqloc_list, Uint1 program)
{
   Uint4 index;
   Uint4 num_seqs = ValNodeLen((ValNode*)seqloc_list);
   Uint4 max_length = 0;
   SeqLoc* seqloc_ptr;
   MultiSeqInfo* retval = (MultiSeqInfo*) calloc(1, sizeof(MultiSeqInfo));

   retval->num_seqs = num_seqs;
   retval->is_prot = 
      (program == blast_type_blastp || program == blast_type_blastx);
   retval->seqloc_array = (SeqLoc**) malloc(num_seqs*sizeof(SeqLoc*));
   retval->seqblk_array = (BLAST_SequenceBlk**) 
      calloc(retval->num_seqs, sizeof(BLAST_SequenceBlk*));
      
   for (index = 0, seqloc_ptr = (SeqLoc*) seqloc_list; 
	index < num_seqs; ++index, seqloc_ptr = seqloc_ptr->next) {
      retval->seqloc_array[index] = seqloc_ptr;
      BLAST_SetUpSubject(program, seqloc_ptr, &retval->seqblk_array[index]);
      max_length = MAX(max_length, retval->seqblk_array[index]->length);
   }
   retval->max_length = max_length;

   return retval;
}

/** Destruct the MultiSeqInfo structure, except the SeqLoc structures, which it
 * does not own.
 */
static MultiSeqInfo* MultiSeqInfoFree(MultiSeqInfo* seq_info)
{
   Uint4 index;
   
   for (index = 0; index < seq_info->num_seqs; ++index) {
      BlastSequenceBlkFree(seq_info->seqblk_array[index]);
   }
   sfree(seq_info->seqblk_array);
   sfree(seq_info->seqloc_array);
   sfree(seq_info);
   return NULL;
}
  
/** Retrieves the length of the longest sequence in the BlastSeqSrc.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int4 MultiSeqGetMaxLength(void* multiseq_handle, void* ignoreme)
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
static Int4 MultiSeqGetAvgLength(void* multiseq_handle, void* ignoreme)
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
static Int4 MultiSeqGetNumSeqs(void* multiseq_handle, void* ignoreme)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;

    ASSERT(seq_info);
    return seq_info->num_seqs;
}

/** Returns 0 as total length, indicating that this is NOT a database!
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int8 MultiSeqGetTotLen(void* multiseq_handle, void* ignoreme)
{
    return 0;
}

/** Needed for completeness only.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static char* MultiSeqGetName(void* multiseq_handle, void* ignoreme)
{
    return NULL;
}

/** Needed for completeness only
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static char* MultiSeqGetDefinition(void* multiseq_handle, void* ignoreme)
{
    return NULL;
}

/** Needed for completeness only.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static char* MultiSeqGetDate(void* multiseq_handle, void* ignoreme)
{
    return NULL;
}

/** Retrieves the date of the BLAST database.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Boolean MultiSeqGetIsProt(void* multiseq_handle, void* ignoreme)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;

    ASSERT(seq_info);

    return (Boolean) seq_info->is_prot;
}

/** Retrieves the sequence meeting the criteria defined by its second argument.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param args Pointer to GetSeqArg structure [in]
 * @return return codes defined in blast_seqsrc.h
 */
static Int2 MultiSeqGetSequence(void* multiseq_handle, void* args)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;
    GetSeqArg* seq_args = (GetSeqArg*) args;
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
 * @param args Pointer to GetSeqArg structure [in]
 * @return return codes defined in blast_seqsrc.h
 */
static Int2 MultiSeqRetSequence(void* multiseq_handle, void* args)
{
    GetSeqArg* seq_args = (GetSeqArg*) args;

    ASSERT(seq_args);

    if (seq_args->seq->sequence_start_allocated)
       sfree(seq_args->seq->sequence_start);

    return 0;
}

/** Retrieves the sequence identifier given its index into the sequence vector.
 * Client code is responsible for deallocating the return value. 
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param args Pointer to integer indicating ordinal id [in]
 * @return Sequence id structure generated from ASN.1 spec, 
 *         cast to a void pointer.
 */
static ListNode* MultiSeqGetSeqId(void* multiseq_handle, void* args)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;
    Int4 index;
    ListNode* seqid_wrap = NULL;
    SeqId* seqid;

    ASSERT(seq_info);
    ASSERT(args);

    index = *((Int4*) args);
    
    seqid = SeqLocId(seq_info->seqloc_array[index]);

    if (seqid)
       seqid_wrap = ListNodeAddPointer(NULL, BLAST_SEQSRC_C_SEQID, 
                                       (void*) seqid);
    return seqid_wrap;
}

#define MAX_SEQID_LENGTH 255

/** Retrieves the sequence identifier in character string form.
 * Client code is responsible for deallocating the return value. 
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param args Pointer to integer indicating index into the vector [in]
 */
static char* MultiSeqGetSeqIdStr(void* multiseq_handle, void* args)
{
    SeqId* seqid;
    char *seqid_str = NULL;
    ListNode* seqid_wrap;

    ASSERT(args);
    seqid_wrap = MultiSeqGetSeqId(multiseq_handle, args);
    if (seqid_wrap->choice != BLAST_SEQSRC_C_SEQID)
        return NULL;
    seqid = (SeqId*) seqid_wrap->ptr;

    if (seqid) {
       seqid_str = (char*) malloc(MAX_SEQID_LENGTH+1);
       SeqIdWrite(seqid, seqid_str, PRINTID_FASTA_LONG, MAX_SEQID_LENGTH);
    }

    return seqid_str;
}

/** Retrieves the sequence identifier given its index into the sequence vector.
 * Client code is responsible for deallocating the return value. 
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param args Pointer to integer indicating ordinal id [in]
 * @return Sequence id structure generated from ASN.1 spec, 
 *         cast to a void pointer.
 */
static ListNode* MultiSeqGetSeqLoc(void* multiseq_handle, void* args)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;
    Int4 index;
    ListNode* seqloc_wrap = NULL;
    SeqLoc* seqloc;

    ASSERT(seq_info);
    ASSERT(args);

    index = *((Int4*) args);

    seqloc = seq_info->seqloc_array[index];

    if (seqloc)
       seqloc_wrap = ListNodeAddPointer(NULL, BLAST_SEQSRC_C_SEQLOC,
                                        (void*) seqloc);

    return seqloc_wrap;
}

/** Retrieve length of a given sequence.
 * @param multiseq_handle Pointer to the structure containing sequences [in]
 * @param args Pointer to integer indicating index into the sequences 
 *             vector [in]
 * @return Length of the sequence or BLAST_SEQSRC_ERROR.
 */
static Int4 MultiSeqGetSeqLen(void* multiseq_handle, void* args)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;
    Int4 index;

    ASSERT(seq_info);
    ASSERT(args);

    index = *((Int4*) args);
    return seq_info->seqblk_array[index]->length;
}

static ListNode* 
MultiSeqGetErrorMessage(void* multiseq_handle, void* ignoreme)
{
    MultiSeqInfo* seq_info = (MultiSeqInfo*) multiseq_handle;
    ListNode* retval = NULL;
    if (seq_info->error_msg)
       retval = ListNodeAddPointer(NULL, BLAST_SEQSRC_MESSAGE, 
                                   (void*) seq_info->error_msg);
    return retval;
}

/** Gets the next sequence index, given a BlastSeqSrc pointer. */
static Int4 MultiSeqIteratorNext(void* seqsrc, BlastSeqSrcIterator* itr)
{
    BlastSeqSrc* seq_src = (BlastSeqSrc*) seqsrc;
    Int4 retval = BLAST_SEQSRC_EOF;
    Int2 status = 0;

    ASSERT(seq_src);
    ASSERT(itr);

    if ((status = BLASTSeqSrcGetNextChunk(seq_src, itr))
        == BLAST_SEQSRC_EOF) {
        return status;
    }
    retval = itr->current_pos++;

    return retval;
}

/** Gets the next sequence index, given a MultiSeqInfo pointer. */
static Int2 MultiSeqGetNextChunk(void* multiseq_handle, 
                                 BlastSeqSrcIterator* itr)
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

BlastSeqSrc* MultiSeqSrcNew(BlastSeqSrc* retval, void* args)
{
    MultiSeqSrcNewArgs* seqsrc_args = (MultiSeqSrcNewArgs*) args;
    MultiSeqInfo* seq_info;

    if (!retval)
        return NULL;
    
    seq_info = 
        MultiSeqInfoNew(seqsrc_args->seqloc_list, seqsrc_args->program);
    
    /* Initialize the BlastSeqSrc structure fields with user-defined function
     * pointers and seq_info */
    SetDeleteFnPtr(retval, &MultiSeqSrcFree);
    SetDataStructure(retval, (void*) seq_info);
    SetGetNumSeqs(retval, &MultiSeqGetNumSeqs);
    SetGetMaxSeqLen(retval, &MultiSeqGetMaxLength);
    SetGetAvgSeqLen(retval, &MultiSeqGetAvgLength);
    SetGetTotLen(retval, &MultiSeqGetTotLen);
    SetGetName(retval, &MultiSeqGetName);
    SetGetDefinition(retval, &MultiSeqGetDefinition);
    SetGetDate(retval, &MultiSeqGetDate);
    SetGetIsProt(retval, &MultiSeqGetIsProt);
    SetGetSequence(retval, &MultiSeqGetSequence);
    SetGetSeqIdStr(retval, &MultiSeqGetSeqIdStr);
    SetGetSeqId(retval, &MultiSeqGetSeqId);
    SetGetSeqLoc(retval, &MultiSeqGetSeqLoc);
    SetGetSeqLen(retval, &MultiSeqGetSeqLen);
    SetGetError(retval, &MultiSeqGetErrorMessage);
    SetGetNextChunk(retval, &MultiSeqGetNextChunk);
    SetIterNext(retval, &MultiSeqIteratorNext);
    SetRetSequence(retval, &MultiSeqRetSequence);

    return retval;
}

BlastSeqSrc* MultiSeqSrcFree(BlastSeqSrc* seq_src)
{
   MultiSeqInfo* seq_info;

    if (!seq_src) 
        return NULL;

    seq_info = (MultiSeqInfo*)GetDataStructure(seq_src);

    seq_info = MultiSeqInfoFree(seq_info);
    sfree(seq_src);

    return NULL;
}

BlastSeqSrc*
MultiSeqSrcInit(SeqLoc* seqloc_list, Uint1 program)
{
    BlastSeqSrcNewInfo bssn_info;
    MultiSeqSrcNewArgs* args =
        (MultiSeqSrcNewArgs*) calloc(1, sizeof(MultiSeqSrcNewArgs));;
    args->seqloc_list = seqloc_list;
    args->program = program;
    bssn_info.constructor = &MultiSeqSrcNew;
    bssn_info.ctor_argument = (void*) args;

    return BlastSeqSrcNew(&bssn_info);
}
