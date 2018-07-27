/*  $Id: seqsrc_readdb.h,v 1.16 2005/04/21 15:00:36 dondosha Exp $
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
* Author:  Christiam Camacho
* ===========================================================================*/

/** @file seqsrc_readdb.h
 * Implementation of the BlastSeqSrc interface using readdb
 */

#ifndef SEQSRC_READDB_H
#define SEQSRC_READDB_H

#ifndef NCBI_C_TOOLKIT
#define NCBI_C_TOOLKIT
#endif

#include <readdb.h>
#include <algo/blast/core/blast_seqsrc.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup CToolkitAlgoBlast
 *
 * @{
 */

/** Maximal allowed length for a sequence id string. */
#define SEQIDLEN_MAX 255

/** Initialize the sequence source structure.
 * @param dbname BLAST database name [in]
 * @param is_prot Is this a protein or nucleotide database? [in]
 * @param first_seq First ordinal id in the database to search [in]
 * @param last_seq Last ordinal id in the database to search 
 *                 (full database if 0) [in]
 */
BlastSeqSrc* 
ReaddbBlastSeqSrcInit(const char* dbname, Boolean is_prot, int first_seq, 
                      int last_seq);

/** Initialize the sequence source structure given an already created
 * ReadDBFILE structure.
 * @param rdfp Initialized BLAST database structure [in]
 * @return New allocated and initialized BlastSeqSrc, using rdfp as data 
 *         structure.
 */
BlastSeqSrc*
ReaddbBlastSeqSrcAttach(ReadDBFILE* rdfp);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* SEQSRC_READDB_H */
