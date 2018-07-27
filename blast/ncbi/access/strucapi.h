/*   strucapi.h
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
* File Name:  strucapi.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   8/18/00
*
* $Revision: 1.4 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/


#ifndef _STRUCAPI_
#define _STRUCAPI_

#include <ncbi.h>
#include <asn.h>
#include <mmdbapi.h>
#include <objmime.h>
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


/* See synchronous and asynchronous code examples at bottom of this header */

/* low-level connection functions */

NLM_EXTERN CONN StrucOpenConnection (
  Int4 uid
);

NLM_EXTERN BiostrucSeqPtr StrucWaitForReply (
  CONN conn
);

/*
 StrucSynchronousQuery opens connection, sends a
 uid request, and waits for a reply
*/

NLM_EXTERN BiostrucSeqPtr StrucSynchronousQuery (
  Int4 uid
);

/*
 StrucAsynchronousQuery opens connection, send request,
 and queues completion routine using urlquery queueing
 mechanism.

 StrucCheckQueue should be called several times a second with
 a timer.  It calls QUERY_CheckQueue to poll connection,
 which calls completion routine when result is available,
 cleaning up connection afterwards.

 StrucReadReply take conns and status parameters from completion
 routine and reads BiostrucSeqPtr.
*/

NLM_EXTERN Boolean StrucAsynchronousQuery (
  Int4 uid,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 StrucCheckQueue (
  QUEUE* queue
);

NLM_EXTERN BiostrucSeqPtr StrucReadReply (
  CONN conn,
  EIO_Status status
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

#endif /* _STRUCAPI_ */

