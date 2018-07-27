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
* $Revision: 1.6 $
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

/* db is "PubMed", "Protein", "Nucleotide", or "Popset" */

NLM_EXTERN CONN PMFetchOpenConnection (
  CharPtr db,
  Int4 uid
);

NLM_EXTERN PubmedEntryPtr PubMedWaitForReply (
  CONN conn
);

NLM_EXTERN SeqEntryPtr PubSeqWaitForReply (
  CONN conn
);

/*
 PubMedSynchronousQuery and PubSeqSynchronousQuery open a
 connection, send a uid request, and wait for a reply
*/

/* db "PubMed" is implied */

NLM_EXTERN PubmedEntryPtr PubMedSynchronousQuery (
  Int4 uid
);

/* db must be "Protein", "Nucleotide", or "Popset" */

NLM_EXTERN SeqEntryPtr PubSeqSynchronousQuery (
  CharPtr db,
  Int4 uid
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
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 PubMedCheckQueue (
  QUEUE* queue
);

NLM_EXTERN PubmedEntryPtr PubMedReadReply (
  CONN conn,
  EIO_Status status
);

NLM_EXTERN Boolean PubSeqAsynchronousQuery (
  CharPtr db,
  Int4 uid,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 PubSeqCheckQueue (
  QUEUE* queue
);

NLM_EXTERN SeqEntryPtr PubSeqReadReply (
  CONN conn,
  EIO_Status status
);

/* SeqId fetch function */

NLM_EXTERN Boolean PubSeqFetchEnable (
  void
);
NLM_EXTERN void PubSeqFetchDisable (
  void
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

