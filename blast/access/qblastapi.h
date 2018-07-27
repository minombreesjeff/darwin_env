/*   qblastapi.h
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
* File Name:  qblastapi.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   6/28/00
*
* $Revision: 1.8 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* $Log: qblastapi.h,v $
* Revision 1.8  2001/02/21 22:07:34  lavr
* Changes for use new CONN interface
*
* Revision 1.7  2000/12/06 17:53:46  madden
* Add prototype for QBBioseqToFasta
*
* Revision 1.6  2000/11/07 21:57:18  shavirin
* Added definition of the new function BLASTGetQueryBioseqByRID()
* Added few connection -oriented defines in the header.
*
* Revision 1.5  2000/07/19 15:17:15  shavirin
* Added loging for this file.
*
* 
* ==========================================================================
*/

#ifndef _QBLASTAPI_
#define _QBLASTAPI_

#include <ncbi.h>
#include <asn.h>
#include <objseq.h>
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


typedef void (LIBCALLBACK *QBlastAnnounceProc) (
  Nlm_CharPtr requestID,
  Nlm_CharPtr seqID,
  Nlm_Int2 estimatedSeconds
);

typedef Nlm_Boolean (LIBCALLBACK *QBlastResultProc) (
  Nlm_CharPtr filename,
  Nlm_VoidPtr userdata,
  Nlm_CharPtr requestID,
  Nlm_CharPtr seqID,
  Nlm_Boolean success
);

/* Opaque handle type.  Variable must be kept by application and initialized
 * to NULL.
 */
struct QBQueueTag;
typedef struct QBQueueTag* QBQUEUE;  /* queue handle */

/* low-level connection functions */

NLM_EXTERN CONN QBlastOpenConnection (
  void
);

NLM_EXTERN EIO_Status QBlastWaitForReply (
  CONN conn
);

/*
 QBlastAsynchronousRequest open a connection, send a FASTA
 request, obtains a request ID, and after the expected
 time starts checking for results every 5 seconds
*/

NLM_EXTERN Boolean QBlastAsynchronousRequest (
  CharPtr database,
  CharPtr program,
  BioseqPtr bsp,
  QBQUEUE* queue,
  QBlastResultProc resultproc,
  QBlastAnnounceProc announceproc,
  VoidPtr userdata
);

/*
 QBlastCheckQueue should be called several times a
 second with a timer.  It calls QUERY_CheckQueue to
 poll connection, which calls completion routine when
 result is available, cleaning up connection afterwards.
*/

NLM_EXTERN Int4 QBlastCheckQueue (
  QBQUEUE* queue
);

/*
 QBlastCheckRequest allows resumption of checking for
 a result if the program was cancelled before the result
 was ready.
*/

NLM_EXTERN Boolean QBlastCheckRequest (
  CharPtr rid,
  QBQUEUE* queue,
  QBlastResultProc resultproc,
  QBlastAnnounceProc announceproc,
  VoidPtr userdata
);

/*
  Forces all connections to be closed, removes all queries from queue.
*/
NLM_EXTERN void QBlastCloseQueue (
  QBQUEUE* queue
);

/*
  Prints a summary of active queries.
*/
NLM_EXTERN Int4 PrintQBlastQueue (
  QBQUEUE* queue,
  FILE *fp
);


/* produce fasta for qblast queries. */
Boolean QBBioseqToFasta (BioseqPtr bsp, FILE *fp, Boolean is_na);
/* 
   Function to get SeqAnnot for RID. We suppose, that search already
   finished and results are exists on the Qblast repository 
*/
NLM_EXTERN SeqAnnotPtr BLASTGetSeqAnnotByRID(CharPtr RID);

/* 
   Function to get Query Bioseq for RID. We suppose, that search already
   finished and results are exists on the Qblast repository 
*/
NLM_EXTERN BioseqPtr BLASTGetQueryBioseqByRID(CharPtr RID);

/* Function to get BlastObject for RID. We suppose, that search already
   finished and results are exists on the Qblast repository. Blast Object
   ASN.1 will be returned as CharPtr buffer
*/
NLM_EXTERN CharPtr BLASTGetBOByRID(CharPtr RID);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _QBLASTAPI_ */

