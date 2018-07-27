/*  $Id: seqsrc_readdb.c,v 1.27 2004/04/28 19:39:01 dondosha Exp $
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
* Author:  Christiam Camacho
*
* File Description:
*   Implementation of the BlastSeqSrc interface using readdb
*
*/

static char const rcsid[] = "$Id: seqsrc_readdb.c,v 1.27 2004/04/28 19:39:01 dondosha Exp $";

#include <algo/blast/api/seqsrc_readdb.h>
#include <algo/blast/core/blast_def.h>
#include <algo/blast/core/blast_util.h>

/** Retrieves the length of the longest sequence in the BlastSeqSrc.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int4 ReaddbGetMaxLength(void* readdb_handle, void* ignoreme)
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
static Int4 ReaddbGetNumSeqs(void* readdb_handle, void* ignoreme)
{
    return readdb_get_num_entries_total_real((ReadDBFILEPtr) readdb_handle);
}

/** Retrieves the total length of all sequences in the BlastSeqSrc.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Int8 ReaddbGetTotLen(void* readdb_handle, void* ignoreme)
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
static Int4 ReaddbGetAvgLength(void* readdb_handle, void* ignoreme)
{
   Int8 total_length = ReaddbGetTotLen(readdb_handle, ignoreme);
   Int4 num_seqs = MAX(1, ReaddbGetNumSeqs(readdb_handle, ignoreme));

   return (Int4) (total_length/num_seqs);
}

/** Retrieves the name of the BLAST database.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static char* ReaddbGetName(void* readdb_handle, void* ignoreme)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;

    return strdup(readdb_get_filename(rdfp));
}

/** Retrieves the definition of the BLAST database.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static char* ReaddbGetDefinition(void* readdb_handle, void* ignoreme)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
    char* definition = NULL;
    
    if ((definition = readdb_get_title(rdfp)) == NULL)
       definition = readdb_get_filename(rdfp);
    return strdup(definition);
}

/** Retrieves the date of the BLAST database.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static char* ReaddbGetDate(void* readdb_handle, void* ignoreme)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;

    return strdup(readdb_get_date(rdfp));
}

/** Retrieves the date of the BLAST database.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param ignoreme Unused by this implementation [in]
 */
static Boolean ReaddbGetIsProt(void* readdb_handle, void* ignoreme)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;

    return readdb_is_prot(rdfp);
}

/** Retrieves the sequence meeting the criteria defined by its second argument.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param args Pointer to GetSeqArg structure [in]
 * @return return codes defined in blast_seqsrc.h
 */
static Int2 ReaddbGetSequence(void* readdb_handle, void* args)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
    GetSeqArg* readdb_args = (GetSeqArg*) args;
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
    if (oid >= readdb_get_num_entries_total_real(rdfp))
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
 * @param args Pointer to GetSeqArg structure [in]
 * @return return codes defined in blast_seqsrc.h
 */
static Int2 ReaddbRetSequence(void* readdb_handle, void* args)
{
    GetSeqArg* readdb_args = (GetSeqArg*) args;

    ASSERT(readdb_args);

    if (readdb_args->seq->sequence_start_allocated)
       sfree(readdb_args->seq->sequence_start);
    return 0;
}

/** Retrieves the sequence identifier meeting the criteria defined by its 
 * second argument. Currently it is an ordinal id (integer value).
 * @todo Need a way to request difference sequence identifiers in redundant
 * databases.
 * Client code is responsible for deallocating the return value. 
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param args Pointer to integer indicating ordinal id [in]
 */
static char* ReaddbGetSeqIdStr(void* readdb_handle, void* args)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
    Int4* oid = (Int4*) args;
    SeqIdPtr sip = NULL;
    char *seqid_str = NULL;

    if (!rdfp || !oid)
        return NULL;

    if ( !(seqid_str = (char*) malloc(sizeof(char)*SEQIDLEN_MAX)))
        return NULL;

    if (!readdb_get_descriptor(rdfp, *oid, &sip, NULL)) {
        sfree(seqid_str);
        return NULL;
    }

    SeqIdWrite(sip, seqid_str, PRINTID_FASTA_LONG, SEQIDLEN_MAX-1);

    sip = SeqIdSetFree(sip);

    return seqid_str;
}

/** Retrieves the sequence identifier meeting the criteria defined by its 
 * second argument. Currently it is an ordinal id (integer value).
 * Client code is responsible for deallocating the return value. 
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param args Pointer to integer indicating ordinal id [in]
 * @return Sequence id structure generated from ASN.1 spec, 
 *         cast to a void pointer.
 */
static ListNode* ReaddbGetSeqId(void* readdb_handle, void* args)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
    Int4* oid = (Int4*) args;
    SeqIdPtr sip = NULL;
    ListNode* seqid_wrap;

    if (!rdfp || !oid)
        return NULL;

    if (!readdb_get_descriptor(rdfp, *oid, &sip, NULL)) {
        return NULL;
    }
    seqid_wrap = ListNodeAddPointer(NULL, BLAST_SEQSRC_C_SEQID, (void*) sip);

    return seqid_wrap;
}

/* There is no need to return locations from readdb, since we always search
   whole sequences in the database */
static ListNode* ReaddbGetSeqLoc(void* readdb_handle, void* args)
{
   return NULL;
}

/** Retrieve length of a given database sequence.
 * @param readdb_handle Pointer to initialized ReadDBFILEPtr structure [in]
 * @param args Pointer to integer indicating ordinal id [in]
 * @return Length of the database sequence or BLAST_SEQSRC_ERROR.
 */
static Int4 ReaddbGetSeqLen(void* readdb_handle, void* args)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
    Int4* oid = (Int4*) args;

    if (!rdfp || !oid)
       return BLAST_SEQSRC_ERROR;

    return readdb_get_sequence_length(rdfp, *oid);
}

/* There are no error messages saved in the ReadDBFILE structure, so the 
 * following getter function is implemented as always returning NULL.
 */
static ListNode* ReaddbGetError(void* readdb_handle, void* args)
{
   return NULL;
}

/** Retrieves the first OIDList attached to the rdfp_list. This is either a
 * virtual oid list (which spans all rdfp elements in rdfp_list from that point
 * until the end of the list) or individual oid lists attached to individual
 * rdfp elements from the rdfp_list.
 * @param rdfp_list Linked list of ReadDBFILEPtr structures 
 * @return First oid list found or NULL
 */
static OIDListPtr ReaddbFetchFirstOIDList(ReadDBFILEPtr rdfp_list)
{
    OIDListPtr virtual_oidlist = NULL;

    for (; rdfp_list; rdfp_list = rdfp_list->next) {
        if ((virtual_oidlist = rdfp_list->oidlist)) {
            break;
        }
    }

    return virtual_oidlist;
}

/* TODO: IMPLEMENT ME! */
static Int4 ReaddbGetNumberOfSeqs(ReadDBFILEPtr rdfp)
{
    register Int4 retval = 0;

    for (; rdfp; rdfp = rdfp->next) {
        if (rdfp->oidlist) {
            /* guess from its length whether it is a local oidlist or a virtual
             * oidlist */
            Boolean local_oidlist = FALSE;
            if (local_oidlist) {
                /* add up and continue iteration over rdfp*/
                ;
            } else {
                /* add up and exit loop */
                break;
            }
        } else if (rdfp->aliasnseq) {
            retval += rdfp->aliasnseq;
        } else {
            retval += rdfp->num_seqs;
        }
    }

    return retval;
}

static TNlmMutex ReaddbMutex;

static Int2 ReaddbGetNextChunk(void* readdb_handle, BlastSeqSrcIterator* itr)
{
    ReadDBFILEPtr rdfp = (ReadDBFILEPtr) readdb_handle;
    OIDListPtr oidlist = ReaddbFetchFirstOIDList(rdfp);
    unsigned int nseqs;

    if (!rdfp || !itr)
        return BLAST_SEQSRC_ERROR;


    if (itr->next_oid < (unsigned int) rdfp->start)
       itr->next_oid = (unsigned int) rdfp->start;

    while (rdfp) {
       if (rdfp->stop > 0) {
          nseqs = rdfp->stop + 1;
       } else {
          /* call get_totals_ex2?: need a less expensive and accurate way of 
             doing this */
          nseqs = ReaddbGetNumberOfSeqs(rdfp);
       }

       if (itr->next_oid >= nseqs) {
          rdfp = rdfp->next;
       } else {
          break;
       }
    }
    if (!rdfp)
       return BLAST_SEQSRC_EOF;

    if (oidlist) {
        itr->itr_type = eOidList;
        /* Should initialize itr->oid_list here? */
        fprintf(stderr, "OidList iterators are not implemented yet!\n");
        abort();    /* FIXME */
    } else {
        itr->itr_type = eOidRange;
        NlmMutexLockEx(&ReaddbMutex);
        itr->current_pos = itr->oid_range[0] = itr->next_oid;
        itr->oid_range[1] = MIN(itr->next_oid + itr->chunk_sz, nseqs);
        itr->next_oid = itr->oid_range[1];
        NlmMutexUnlock(ReaddbMutex);
    }

    return BLAST_SEQSRC_SUCCESS;
}

static Int4 ReaddbIteratorNext(void* seqsrc, BlastSeqSrcIterator* itr)
{
    BlastSeqSrc* bssp = (BlastSeqSrc*) seqsrc;
    Int4 retval = BLAST_SEQSRC_EOF;
    Int4 status = BLAST_SEQSRC_SUCCESS;

    ASSERT(bssp);
    ASSERT(itr);

    /* If iterator is uninitialized/invalid, retrieve the next chunk from the
     * BlastSeqSrc */
    if (itr->current_pos == UINT4_MAX) {
        status = BLASTSeqSrcGetNextChunk(bssp, itr);
        if (status == BLAST_SEQSRC_ERROR) {
            return status;
        }
    }

    if (itr->itr_type == eOidRange) {

        retval = itr->current_pos++;
        if (itr->current_pos >= itr->oid_range[1]) {
            itr->current_pos = UINT4_MAX;   /* invalidate iterator */
        }
        if (status == BLAST_SEQSRC_EOF) {
            retval = status;
        }

    } else if (itr->itr_type == eOidList) {
        /* Unimplemented iterator type! */
        fprintf(stderr, "eOidList iterator type is not implemented\n");
        abort();
    } else {
        /* Unsupported/invalid iterator type! */
        fprintf(stderr, "Invalid iterator type: %d\n", itr->itr_type);
        abort();
    }

    return retval;
}

BlastSeqSrc* ReaddbSeqSrcNew(BlastSeqSrc* retval, void* args)
{
    ReaddbNewArgs* rargs = (ReaddbNewArgs*) args;
    ReadDBFILEPtr rdfp = NULL;

    if (!retval)
        return NULL;

    /* Initialize the rdfp */
    if ( !(rdfp = readdb_new(rargs->dbname, rargs->is_protein)))
        return NULL;

    /* Initialize the BlastSeqSrc structure fields with user-defined function
     * pointers and rdfp */
    SetDeleteFnPtr(retval, &ReaddbSeqSrcFree);
    SetDataStructure(retval, (void*) rdfp);
    SetGetNumSeqs(retval, &ReaddbGetNumSeqs);
    SetGetMaxSeqLen(retval, &ReaddbGetMaxLength);
    SetGetAvgSeqLen(retval, &ReaddbGetAvgLength);
    SetGetTotLen(retval, &ReaddbGetTotLen);
    SetGetAvgSeqLen(retval, &ReaddbGetAvgLength);
    SetGetName(retval, &ReaddbGetName);
    SetGetDefinition(retval, &ReaddbGetDefinition);
    SetGetDate(retval, &ReaddbGetDate);
    SetGetIsProt(retval, &ReaddbGetIsProt);
    SetGetSequence(retval, &ReaddbGetSequence);
    SetGetSeqIdStr(retval, &ReaddbGetSeqIdStr);
    SetGetSeqId(retval, &ReaddbGetSeqId);
    SetGetSeqLoc(retval, &ReaddbGetSeqLoc);
    SetGetSeqLen(retval, &ReaddbGetSeqLen);
    SetGetNextChunk(retval, &ReaddbGetNextChunk);
    SetIterNext(retval, &ReaddbIteratorNext);
    SetGetError(retval, &ReaddbGetError);
    SetRetSequence(retval, &ReaddbRetSequence);

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
          in the arguments, making the subsequent rdfp's ranges empty. */
       for ( ; rdfp; rdfp = rdfp->next)
          rdfp->stop = rargs->final_db_seq;
    }

    return retval;
}

BlastSeqSrc* ReaddbSeqSrcFree(BlastSeqSrc* bssp)
{
    if (!bssp) 
        return NULL;
    readdb_destruct((ReadDBFILEPtr)GetDataStructure(bssp));
    sfree(bssp);
    return NULL;
}

BlastSeqSrc* 
ReaddbBlastSeqSrcInit(const char* dbname, Boolean is_prot, int first_seq, 
                      int last_seq, void* extra_arg)
{
    BlastSeqSrcNewInfo bssn_info;
    BlastSeqSrc* seq_src = NULL;
    ReaddbNewArgs* readdb_args = 
        (ReaddbNewArgs*) calloc(1, sizeof(ReaddbNewArgs));;
    readdb_args->dbname = strdup(dbname);
    readdb_args->is_protein = is_prot;
    readdb_args->first_db_seq = first_seq;
    readdb_args->final_db_seq = last_seq;
    bssn_info.constructor = &ReaddbSeqSrcNew;
    bssn_info.ctor_argument = (void*) readdb_args;

    seq_src = BlastSeqSrcNew(&bssn_info);
    sfree(readdb_args->dbname);
    sfree(readdb_args);
    return seq_src;
}
