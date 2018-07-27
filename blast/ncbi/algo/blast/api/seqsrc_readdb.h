/*  $Id: seqsrc_readdb.h,v 1.9 2004/02/18 19:38:20 dondosha Exp $
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

#ifndef SEQSRC_READDB_H
#define SEQSRC_READDB_H

#ifndef NCBI_C_TOOLKIT
#define NCBI_C_TOOLKIT
#endif

#include <readdb.h>
#include <algo/blast/core/blast_seqsrc.h>

#define SEQIDLEN_MAX 255

#ifdef __cplusplus
extern "C" {
#endif

/** Encapsulates the arguments needed to initialize a BLAST database using
 * readdb */
typedef struct ReaddbNewArgs {
    char* dbname;     /**< Database name */
    Boolean is_protein; /**< Is this database protein? */
   Int4 first_db_seq; /**< Ordinal id of the first sequence to search */
   Int4 final_db_seq; /**< Ordinal id of the last sequence to search */
} ReaddbNewArgs;

/** Readdb sequence source constructor 
 * @param bssp BlastSeqSrc structure (already allocated) to populate [in]
 * @param args Pointer to ReaddbNewArgs structure above [in]
 * @return Updated bssp structure (with all function pointers initialized
 */
BlastSeqSrc* ReaddbSeqSrcNew(BlastSeqSrc* bssp, void* args);

/** Readdb sequence source destructor: frees its internal data structure and the
 * BlastSeqSrc structure itself.
 * @param bssp BlastSeqSrc structure to free [in]
 * @return NULL
 */
BlastSeqSrc* ReaddbSeqSrcFree(BlastSeqSrc* bssp);

/** Initialize the sequence source structure.
 * @param dbname BLAST database name [in]
 * @param is_prot Is this a protein or nucleotide database? [in]
 * @param first_seq First ordinal id in the database to search [in]
 * @param last_seq Last ordinal id in the database to search 
 *                 (full database if 0) [in]
 * @param extra_arg Reserved for the future implementation of other database
 *                  restrictions [in]
 */
BlastSeqSrc* 
ReaddbBlastSeqSrcInit(const char* dbname, Boolean is_prot, int first_seq, 
                      int last_seq, void* extra_arg);

#ifdef __cplusplus
}
#endif

#endif /* SEQSRC_READDB_H */
