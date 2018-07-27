/*  $Id: seqsrc_readdb.c,v 1.49 2005/04/21 15:00:36 dondosha Exp $
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
*  Author:  Christiam Camacho
* ===========================================================================*/

/** @file seqsrc_readdb.c
 * Implementation of the BlastSeqSrc interface using readdb
 */

static char const rcsid[] = "$Id: seqsrc_readdb.c,v 1.49 2005/04/21 15:00:36 dondosha Exp $";

#include <algo/blast/api/seqsrc_readdb.h>
#include <algo/blast/core/blast_seqsrc_impl.h>
#include <algo/blast/core/blast_def.h>
#include <algo/blast/core/blast_util.h>

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Retrieves the length of the longest sequence in the BlastSeqSrc.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int4 
s_ReaddbGetMaxLength(void* readdb_handle, void* ignoreme)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
    Int4 retval = 0;

    for (; rdfp; rdfp = rdfp->next)
        retval = MAX(retval, readdb_get_maxlen(rdfp));

    return retval;
}

/** Retrieves the number of sequences in the BlastSeqSrc.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int4 
s_ReaddbGetNumSeqs(void* readdb_handle, void* ignoreme)
{
   ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
   Int4 dbnseqs = 0;
   Int8 dblength = 0;
   
   readdb_get_totals_ex(rdfp, &dblength, &dbnseqs, TRUE);
   return dbnseqs;
}

/** Retrieves the total length of all sequences in the BlastSeqSrc.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int8 
s_ReaddbGetTotLen(void* readdb_handle, void* ignoreme)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
    Int4 dbnseqs = 0;
    Int8 dblength = 0;

    readdb_get_totals_ex(rdfp, &dblength, &dbnseqs, TRUE);
    return dblength;
}

/** Retrieves the average length of sequences in the BlastSeqSrc.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int4 
s_ReaddbGetAvgLength(void* readdb_handle, void* ignoreme)
{
   Int8 total_length = s_ReaddbGetTotLen(readdb_handle, ignoreme);
   Int4 num_seqs = MAX(1, s_ReaddbGetNumSeqs(readdb_handle, ignoreme));

   return (Int4) (total_length/num_seqs);
}

/** Retrieves the name of the BLAST database.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static const char* 
s_ReaddbGetName(void* readdb_handle, void* ignoreme)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;

    return readdb_get_filename(rdfp);
}

/** Retrieves the date of the BLAST database.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Boolean 
s_ReaddbGetIsProt(void* readdb_handle, void* ignoreme)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;

    return readdb_is_prot(rdfp);
}

/** Retrieves the sequence meeting the criteria defined by its second argument.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param args Pointer to BlastSeqSrcGetSeqArg structure [in]
 * @return return codes defined in blast_seqsrc.h
 */
static Int2 
s_ReaddbGetSequence(void* readdb_handle, void* args)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
    BlastSeqSrcGetSeqArg* readdb_args = (BlastSeqSrcGetSeqArg*) args;
    Int4 oid = -1, len = 0, buflen = 0;
    Uint1 *buf = NULL, encoding;
    Boolean has_sentinel_byte;
    Boolean buffer_allocated;

    if (!rdfp || !readdb_args)
        return BLAST_SEQSRC_ERROR;

    oid = readdb_args->oid;
    encoding = readdb_args->encoding;
    has_sentinel_byte = (encoding == BLASTNA_ENCODING);
    buffer_allocated = 
       (encoding == BLASTNA_ENCODING || encoding == NCBI4NA_ENCODING);

    /* free buffers if necessary */
    if (readdb_args->seq)
        BlastSequenceBlkClean(readdb_args->seq);

    /* TODO: this should be cached somewhere */
    if (oid >= readdb_get_num_entries_total(rdfp))
        return BLAST_SEQSRC_EOF;

    if (!buffer_allocated) 
        len = readdb_get_sequence(rdfp, oid, &buf);
    else
        len = readdb_get_sequence_ex(rdfp, oid, &buf, &buflen, has_sentinel_byte);
       
    if (len <= 0) {
        sfree(buf);
        return BLAST_SEQSRC_ERROR;
    }

    BlastSetUp_SeqBlkNew(buf, len, 0, &readdb_args->seq, buffer_allocated);
    /* If there is no sentinel byte, and buffer is allocated, i.e. this is
       the traceback stage of a translated search, set "sequence" to the same 
       position as "sequence_start". */
    if (buffer_allocated && !has_sentinel_byte)
       readdb_args->seq->sequence = readdb_args->seq->sequence_start;

    readdb_args->seq->oid = oid;

    return BLAST_SEQSRC_SUCCESS;
}

/** Deallocates uncompressed sequence buffer, obtained by ReaddbGetSequence.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param args Pointer to BlastSeqSrcGetSeqArg structure [in]
 */
static void
s_ReaddbReleaseSequence(void* readdb_handle, void* args)
{
    BlastSeqSrcGetSeqArg* readdb_args = (BlastSeqSrcGetSeqArg*) args;
    ASSERT(readdb_args);
    ASSERT(readdb_args->seq);
    BlastSequenceBlkClean(readdb_args->seq);
}

/** Retrieve length of a given database sequence.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param args Pointer to integer indicating ordinal id [in]
 * @return Length of the database sequence or BLAST_SEQSRC_ERROR.
 */
static Int4 
s_ReaddbGetSeqLen(void* readdb_handle, void* args)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
    Int4* oid = (Int4*) args;

    if (!rdfp || !oid)
       return BLAST_SEQSRC_ERROR;

    return readdb_get_sequence_length(rdfp, *oid);
}

/** Mutex for retrieving ordinal id chunks from ReadDB in a multi-threaded
 * search.
 */
static TNlmMutex ReaddbMutex;


/** Retrieve next chunk of ordinal ids from a ReadDBFILE structure, in case
 * it contains an oidlist.
 * NB: this function is not MT-safe: ReaddbMutex must be locked/unlocked around
 * any call to this function.
 * @param rdfp List of ReadDBFILE structures [in]
 * @param itr BLAST sequence source iterator [in]
 * @param last_oid_assigned Last ordinal id processed [in] [out]
 * @return Status
 */ 
static Int2
s_ReadDbGetNextOidListChunk(ReadDBFILEPtr rdfp, BlastSeqSrcIterator* itr,
                          Uint4* last_oid_assigned)
     
{
   Int2 status = BLAST_SEQSRC_SUCCESS;
   OIDListPtr oidlist;
   Uint4  gi_start, gi_end;
   Int4* id_list;
   Uint4 oidindex  = 0;

   if (!itr || !last_oid_assigned)
      return BLAST_SEQSRC_ERROR;

   for ( ; rdfp; rdfp = rdfp->next) {
      oidlist = rdfp->oidlist;
   
      /* If there is no OID list, go to the next readdb structure. */
      if (!oidlist)
         continue;

      gi_start = MAX(*last_oid_assigned, (Uint4)rdfp->start) - rdfp->start;
      gi_end = (Uint4)oidlist->total + 1;
      id_list = itr->oid_list;

      if (gi_start < gi_end) {
         Uint4 bit_start = gi_start % MASK_WORD_SIZE;
         Uint4 gi;

         for(gi = gi_start; (gi < gi_end) && (oidindex < itr->chunk_sz);) {
            Int4 bit_end = ((gi_end - gi + bit_start) < MASK_WORD_SIZE) ?
               (gi_end - gi + bit_start) : MASK_WORD_SIZE;
            Int4 bit;
            
            Uint4 mask_index = gi / MASK_WORD_SIZE;
            Uint4 mask_word  = Nlm_SwapUint4(oidlist->list[mask_index]);
            
            if ( mask_word ) {
               for(bit = bit_start; bit<bit_end && oidindex<itr->chunk_sz; bit++) {
                  Uint4 bitshift = (MASK_WORD_SIZE-1)-bit;
                  
                  if ((mask_word >> bitshift) & 1) {
                     id_list[ oidindex++ ] = rdfp->start + (gi - bit_start) + bit;
                  }
               }
               gi += bit - bit_start;
            } else {
               gi += bit_end - bit_start;
            }
            
            bit_start = 0;
         }

         if (oidindex == itr->chunk_sz || !rdfp->next) {
            itr->itr_type = eOidList;
            itr->current_pos = 0;
            *last_oid_assigned = rdfp->start + gi;
            itr->chunk_sz = oidindex;
            break;
         }
      } /* End if (gi_start < gi_end) */
   } /* End loop over ReadDBFILE's */
   
   if (!rdfp) 
      status = BLAST_SEQSRC_EOF;

   return status;
}

/** Assigns next chunk of ordinal ids from the database to the sequence source
 * iterator.
 * @param readdb_handle Pointer to the ReadDBFILE structure [in]
 * @param itr Iterator over the sequence source associated with the 
 *            readdb_handle, requesting the next chunk of the 
 *            database.  [in] [out]
 * @return Status.
 */
static Int2 
s_ReaddbGetNextChunk(void* readdb_handle, BlastSeqSrcIterator* itr)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
    ReadDBFILEPtr rdfp_head = rdfp;
    unsigned int nseqs = 0;
    Uint4 current_oid;
    Int2 status = BLAST_SEQSRC_SUCCESS;
    Uint4 real_readdb_entries;

    if (!rdfp || !itr)
        return BLAST_SEQSRC_ERROR;

    real_readdb_entries = readdb_get_num_entries_total_real(rdfp);
    
    /* Lock the mutex before retrieving the next chunk */
    NlmMutexLockEx(&ReaddbMutex);
    ASSERT(rdfp->shared_info);

    current_oid = rdfp->shared_info->last_oid_assigned;

    if (current_oid < (unsigned int) rdfp->start)
       current_oid = (unsigned int) rdfp->start;
    
    for ( ; rdfp && !rdfp->oidlist; rdfp = rdfp->next) {
       if (rdfp->stop > 0) {
          nseqs = rdfp->stop + 1;
       } else if (rdfp->aliasnseq) {
          nseqs = rdfp->aliasnseq;
       } else {
          nseqs = rdfp->num_seqs;
       }
       
       if (current_oid < nseqs)
          break;
    }

    if (!rdfp) {
       status = BLAST_SEQSRC_EOF;
    } else if (!rdfp->oidlist) {
       itr->itr_type = eOidRange;
       itr->current_pos = itr->oid_range[0] = current_oid;
       itr->oid_range[1] = MIN(current_oid + itr->chunk_sz, nseqs);
       rdfp_head->shared_info->last_oid_assigned = itr->oid_range[1];
    } else {
       status = s_ReadDbGetNextOidListChunk(rdfp, itr, 
                   &rdfp_head->shared_info->last_oid_assigned);
    }

    NlmMutexUnlock(ReaddbMutex);

    return status;
}

/** Given an iterator over a BLAST database, returns the next ordinal id 
 * to search.
 * @param readdb_handle Pointer to the ReadDBFILE structure [in]
 * @param itr Iterator over seqsrc. [in]
 * @return Next ordinal id to search.
 */
static Int4 
s_ReaddbIteratorNext(void* readdb_handle, BlastSeqSrcIterator* itr)
{
    Int4 retval = BLAST_SEQSRC_EOF;
    Int4 status = BLAST_SEQSRC_SUCCESS;
    Uint4 last_pos = 0;

    ASSERT(readdb_handle);
    ASSERT(itr);

    /* If iterator is uninitialized/invalid, retrieve the next chunk from the
     * BlastSeqSrc */
    if (itr->current_pos == UINT4_MAX) {
        status = s_ReaddbGetNextChunk(readdb_handle, itr);
        if (status != BLAST_SEQSRC_SUCCESS) {
            return status;
        }
    }

    if (itr->itr_type == eOidRange) {
        retval = itr->current_pos;
        last_pos = itr->oid_range[1];
    } else if (itr->itr_type == eOidList) {
        retval = itr->oid_list[itr->current_pos];
        last_pos = itr->chunk_sz;
    } else {
        /* Unsupported/invalid iterator type! */
        fprintf(stderr, "Invalid iterator type: %d\n", itr->itr_type);
        retval = BLAST_SEQSRC_ERROR;
    }

    ++itr->current_pos;
    if (itr->current_pos >= last_pos) {
        itr->current_pos = UINT4_MAX;  /* invalidate internal iteration */
    }

    return retval;
}

/** Readdb sequence source destructor: frees its internal data structure and the
 * BlastSeqSrc structure itself.
 * @param bssp BlastSeqSrc structure to free [in]
 * @return NULL
 */
static BlastSeqSrc* 
s_ReaddbSeqSrcFree(BlastSeqSrc* bssp)
{
    if (!bssp) 
        return NULL;
    readdb_destruct((ReadDBFILEPtr)_BlastSeqSrcImpl_GetDataStructure(bssp));
    sfree(bssp);
    return NULL;
}

/** Readdb sequence source copier: 
 * creates a new copy of the ReadDBFILE structure by calling readdb_attach.
 * @param bssp BlastSeqSrc structure to copy [in]
 * @return New BlastSeqSrc structure
 */
static BlastSeqSrc* 
s_ReaddbSeqSrcCopy(BlastSeqSrc* bssp)
{
   ReadDBFILE* rdfp = NULL;

   if (!bssp) 
      return NULL;

   rdfp = readdb_attach((ReadDBFILEPtr)_BlastSeqSrcImpl_GetDataStructure(bssp));

   _BlastSeqSrcImpl_SetDataStructure(bssp, (void*) rdfp);
    
   return bssp;
}

/** Initializes function pointers and data structure in a new readdb-based 
 * BlastSeqSrc.
 * @param retval Allocated BlastSeqSrc structure [in|out]
 * @param rdfp ReadDBFILE structure to be used as data structure. [in]
 */
static void
s_InitNewReaddbSeqSrc(BlastSeqSrc* retval, ReadDBFILE* rdfp)
{
    ASSERT(retval);

    /* Initialize the BlastSeqSrc structure fields with user-defined function
     * pointers and rdfp */
    _BlastSeqSrcImpl_SetDeleteFnPtr(retval, &s_ReaddbSeqSrcFree);
    _BlastSeqSrcImpl_SetCopyFnPtr(retval, &s_ReaddbSeqSrcCopy);
    _BlastSeqSrcImpl_SetDataStructure(retval, (void*) rdfp);
    _BlastSeqSrcImpl_SetGetNumSeqs(retval, &s_ReaddbGetNumSeqs);
    _BlastSeqSrcImpl_SetGetMaxSeqLen(retval, &s_ReaddbGetMaxLength);
    _BlastSeqSrcImpl_SetGetAvgSeqLen(retval, &s_ReaddbGetAvgLength);
    _BlastSeqSrcImpl_SetGetTotLen(retval, &s_ReaddbGetTotLen);
    _BlastSeqSrcImpl_SetGetName(retval, &s_ReaddbGetName);
    _BlastSeqSrcImpl_SetGetIsProt(retval, &s_ReaddbGetIsProt);
    _BlastSeqSrcImpl_SetGetSequence(retval, &s_ReaddbGetSequence);
    _BlastSeqSrcImpl_SetGetSeqLen(retval, &s_ReaddbGetSeqLen);
    _BlastSeqSrcImpl_SetIterNext(retval, &s_ReaddbIteratorNext);
    _BlastSeqSrcImpl_SetReleaseSequence(retval, &s_ReaddbReleaseSequence);
}

/** Fills contents in the allocated BlastSeqSrc, given an already created
 * ReadDBFILE structure. Uses readdb_attach to ensure that input ReadDBFILE
 * is not freed in BlastSeqSrcFree.
 * @param retval Allocated BlastSeqSrc to fill [in|out]
 * @param args Pointer to a ReadDBFILE structure, cast to void to ensure 
 *             correct signature. [in]
 * @return Same as retval.
 */
static BlastSeqSrc* 
s_ReaddbSeqSrcAttach(BlastSeqSrc* retval, void* args)
{
    ReadDBFILE* rdfp_in = (ReadDBFILE*) args;
    ReadDBFILE* rdfp = NULL;

    ASSERT(retval);

    if (!rdfp_in) {
        _BlastSeqSrcImpl_SetInitErrorStr(retval, 
         strdup("Cannot attach BlastSeqSrc to a NULL ReadDBFILE structure"));
    } else {
        rdfp = readdb_attach(rdfp_in);
        if ( !rdfp ) {
            _BlastSeqSrcImpl_SetInitErrorStr(retval, 
             strdup("Failed to attach to existing ReadDBFILE structure"));
        }
    }

    s_InitNewReaddbSeqSrc(retval, rdfp);

    return retval;
}

/** Encapsulates the arguments needed to initialize a BLAST database using
 * readdb */
typedef struct ReaddbNewArgs {
    char* dbname;     /**< Database name */
    Boolean is_protein; /**< Is this database protein? */
   Int4 first_db_seq; /**< Ordinal id of the first sequence to search */
   Int4 final_db_seq; /**< Ordinal id of the last sequence to search */
} ReaddbNewArgs;

/** Readdb sequence source constructor 
 * @param retval BlastSeqSrc structure (already allocated) to populate [in]
 * @param args Pointer to ReaddbNewArgs structure above [in]
 * @return Updated bssp structure (with all function pointers initialized
 */
static BlastSeqSrc* 
s_ReaddbSeqSrcNew(BlastSeqSrc* retval, void* args)
{
    ReaddbNewArgs* rargs = (ReaddbNewArgs*) args;
    ReadDBFILEPtr rdfp = NULL;

    ASSERT(retval);
    ASSERT(rargs);

    if ( !rargs->dbname ) {
        _BlastSeqSrcImpl_SetInitErrorStr(retval, 
         strdup("Cannot initialize readdb BlastSeqSrc "
               "with NULL database name"));
    } else {
        /* Initialize the rdfp */
        if ( !(rdfp = readdb_new(rargs->dbname, rargs->is_protein))) {
            char buf[1024];
            snprintf(buf, sizeof(buf), 
                     "s_ReaddbSeqSrcNew: could not open %s %s database\n",
                     rargs->dbname, 
                     (rargs->is_protein ? "protein" : "nucleotide"));
            _BlastSeqSrcImpl_SetInitErrorStr(retval, strdup(buf));
        }
    }

    s_InitNewReaddbSeqSrc(retval, rdfp);

    /* Set the range, if it is specified */
    if (rargs->first_db_seq > 0) {
       while (rdfp && rdfp->stop < rargs->first_db_seq) {
          /* Make this rdfp's range empty */
          rdfp->start = rdfp->stop + 1;
          rdfp = rdfp->next;
       }
       rdfp->start = rargs->first_db_seq;
    }
    if (rargs->final_db_seq > 0) {
       while (rdfp && rdfp->stop < rargs->final_db_seq)
          rdfp = rdfp->next;
       /* Set last sequence for this and all subsequent rdfp's to the one
          in the arguments, making the subsequent rdfp's ranges empty. 
          Note that final_db_seq in arguments is 1 beyond the last sequence
          number to search. */
       for ( ; rdfp; rdfp = rdfp->next)
          rdfp->stop = rargs->final_db_seq - 1;
    }

    return retval;
}

BlastSeqSrc* 
ReaddbBlastSeqSrcInit(const char* dbname, Boolean is_prot, int first_seq, 
                      int last_seq)
{
    BlastSeqSrcNewInfo bssn_info;
    BlastSeqSrc* seq_src = NULL;
    ReaddbNewArgs readdb_args;

    readdb_args.dbname = (char*) dbname;
    readdb_args.is_protein = is_prot;
    readdb_args.first_db_seq = first_seq;
    readdb_args.final_db_seq = last_seq;
    bssn_info.constructor = &s_ReaddbSeqSrcNew;
    bssn_info.ctor_argument = (void*) &readdb_args;

    seq_src = BlastSeqSrcNew(&bssn_info);
    return seq_src;
}

BlastSeqSrc*
ReaddbBlastSeqSrcAttach(ReadDBFILE* rdfp)
{
    BlastSeqSrcNewInfo bssn_info;
    BlastSeqSrc* seq_src = NULL;

    bssn_info.constructor = &s_ReaddbSeqSrcAttach;
    bssn_info.ctor_argument = (void*) rdfp;

    seq_src = BlastSeqSrcNew(&bssn_info);
    return seq_src;
}

/* @} */

