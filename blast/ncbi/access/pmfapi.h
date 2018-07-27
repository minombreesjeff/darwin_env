/*   pmfapi.h
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  pmfapi.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   5/5/00
*
* $Revision: 1.22 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#ifndef _PMFAPI_
#define _PMFAPI_

#include <ncbi.h>
#include <asn.h>
#include <objmedli.h>
#include <objpubme.h>
#include <objsset.h>
#include <urlquery.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif


/* low-level connection functions */

NLM_EXTERN CONN PubMedFetchOpenConnection (
  Int4 uid
);

NLM_EXTERN CONN PubSeqFetchOpenConnection (
  Int4 uid,
  Int2 retcode,
  Int4 flags
);

NLM_EXTERN CONN GiRevHistOpenConnection (
  Int4 uid,
  Int4 num,
  Int4Ptr uids
);

NLM_EXTERN CONN AccnRevHistOpenConnection (
  CharPtr accn
);

NLM_EXTERN CONN GiSeqIdSetOpenConnection (
  Int4 gi
);

NLM_EXTERN CONN AccnListOpenConnection (
  CharPtr PNTR accns
);

NLM_EXTERN PubmedEntryPtr PubMedWaitForReply (
  CONN conn
);

NLM_EXTERN SeqEntryPtr PubSeqWaitForReply (
  CONN conn
);

NLM_EXTERN CharPtr GiRevHistWaitForReply (
  CONN conn
);

NLM_EXTERN Int4 AccnRevHistWaitForReply (
  CONN conn
);

NLM_EXTERN SeqIdPtr GiSeqIdSetWaitForReply (
  CONN conn
);

NLM_EXTERN CharPtr AccnListWaitForReply (
  CONN conn
);

/*
 PubMedSynchronousQuery and PubSeqSynchronousQuery open a
 connection, send a uid request, and wait for a reply
*/

NLM_EXTERN PubmedEntryPtr PubMedSynchronousQuery (
  Int4 uid
);

NLM_EXTERN SeqEntryPtr PubSeqSynchronousQuery (
  Int4 uid,
  Int2 retcode,
  Int4 flags
);

NLM_EXTERN CharPtr GiRevHistSynchronousQuery (
  Int4 uid,
  Int4 num,
  Int4Ptr uids
);

NLM_EXTERN Int4 AccnRevHistSynchronousQuery (
  CharPtr accn
);

NLM_EXTERN SeqIdPtr GiSeqIdSetSynchronousQuery (
  Int4 gi
);

NLM_EXTERN CharPtr AccnListSynchronousQuery (
  CharPtr PNTR accns
);

/*
 PubMedAsynchronousQuery and PubSeqAsynchronousQuery open
 connection, send request, and queue completion routine
 using urlquery queueing mechanism.

 PubMedCheckQueue and PubSeqCheckQueue should be called
 several times a second with a timer.  They each call
 QUERY_CheckQueue to poll connection, which calls completion
 routine when result is available, cleaning up connection
 afterwards.

 PubMedReadReply and PubSeqReadReply take conn and status
 parameters from completion routine and reads PubmedEntryPtr
 or SeqEntryPtr.
*/

NLM_EXTERN Boolean PubMedAsynchronousQuery (
  Int4 uid,
  QUEUE* q,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 PubMedCheckQueue (
  QUEUE* q
);

NLM_EXTERN PubmedEntryPtr PubMedReadReply (
  CONN conn,
  EIO_Status status
);

NLM_EXTERN Boolean PubSeqAsynchronousQuery (
  Int4 uid,
  Int2 retcode,
  Int4 flags,
  QUEUE* q,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 PubSeqCheckQueue (
  QUEUE* q
);

NLM_EXTERN SeqEntryPtr PubSeqReadReply (
  CONN conn,
  EIO_Status status
);

NLM_EXTERN Boolean GiRevHistAsynchronousQuery (
  Int4 uid,
  Int4 num,
  Int4Ptr uids,
  QUEUE* q,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 GiRevHistCheckQueue (
  QUEUE* q
);

NLM_EXTERN CharPtr GiRevHistReadReply (
  CONN conn,
  EIO_Status status
);

NLM_EXTERN Boolean AccnRevHistAsynchronousQuery (
  CharPtr accn,
  QUEUE* q,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 AccnRevHistCheckQueue (
  QUEUE* q
);

NLM_EXTERN Int4 AccnRevHistReadReply (
  CONN conn,
  EIO_Status status
);

NLM_EXTERN Boolean GiSeqIdSetAsynchronousQuery (
  Int4 gi,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 GiSeqIdSetCheckQueue (
  QUEUE* queue
);

NLM_EXTERN SeqIdPtr GiSeqIdSetReadReply (
  CONN conn,
  EIO_Status status
);

NLM_EXTERN Boolean AccnListAsynchronousQuery (
  CharPtr PNTR accns,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 AccnListCheckQueue (
  QUEUE* queue
);

NLM_EXTERN CharPtr AccnListReadReply (
  CONN conn,
  EIO_Status status
);

/* PMID fetch registration function */

NLM_EXTERN Boolean PubMedFetchEnable (
  void
);

NLM_EXTERN void PubMedFetchDisable (
  void
);

/* SeqId fetch registration function */

NLM_EXTERN Boolean PubSeqFetchEnable (
  void
);

NLM_EXTERN void PubSeqFetchDisable (
  void
);

/* multiple SeqId preload into cache functions */

NLM_EXTERN Int4 GiRevHistPreLoadSeqIdGiCache (
  Int4 num,
  Int4Ptr uids
);

NLM_EXTERN Int4 LIBCALLBACK GiRevHistLookupFarSeqIDs (
  SeqEntryPtr sep,
  Boolean components,
  Boolean locations,
  Boolean products,
  Boolean alignments,
  Boolean history
);

/* SeqId chain fetch function */

NLM_EXTERN SeqIdPtr LIBCALLBACK GiRevHistLookupSeqIdSet (
  Int4 gi
);

/* multiple Accession bulk lookup or preload section */

typedef void (LIBCALLBACK *CacheAccnListProc) (
  Int4 gi,
  CharPtr accnver
);

/*
 CacheAccnList takes the AccnList query result, and calls the
 user callback if it is not NULL, otherwise it preloads the
 sequence manager gi/accession lookup cache
*/

NLM_EXTERN Int4 CacheAccnList (
  CharPtr str,
  CacheAccnListProc userfunc
);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _PMFAPI_ */

