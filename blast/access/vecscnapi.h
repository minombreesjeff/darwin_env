/*   vecscnapi.h
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
* File Name:  vecscnapi.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   6/13/00
*
* $Revision: 1.3 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#ifndef _VECSCNAPI_
#define _VECSCNAPI_

#include <ncbi.h>
#include <asn.h>
#include <objseq.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef void (LIBCALLBACK *VecScreenAnnounceProc) (
  Nlm_CharPtr requestID,
  Nlm_CharPtr seqID,
  Nlm_Int2 estimatedSeconds
);

typedef Nlm_Boolean (LIBCALLBACK *VecScreenResultProc) (
  Nlm_CharPtr filename,
  Nlm_VoidPtr userdata,
  Nlm_CharPtr requestID,
  Nlm_CharPtr seqID,
  Nlm_Boolean success
);

/* Opaque handle type.  Variable must be kept by application and initialized
 * to NULL.
 */
struct VQueueTag;
typedef struct VQueueTag* VQUEUE;  /* queue handle */

/* low-level connection functions */

NLM_EXTERN CONN VecScreenOpenConnection (
  CharPtr query
);

NLM_EXTERN EIO_Status VecScreenWaitForReply (
  CONN conn
);

/*
 VecScreenAsynchronousRequest open a connection, send a
 FASTA request, obtains a request ID, and after the expected
 time starts checking for results every 5 seconds
*/

NLM_EXTERN Boolean VecScreenAsynchronousRequest (
  CharPtr database,
  BioseqPtr bsp,
  VQUEUE* queue,
  VecScreenResultProc resultproc,
  VecScreenAnnounceProc announceproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 VecScreenCheckQueue (
  VQUEUE* queue
);

NLM_EXTERN Int4 PrintVecScreenQueue (
  VQUEUE* queue,
  FILE *fp
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

#endif /* _VECSCNAPI_ */

