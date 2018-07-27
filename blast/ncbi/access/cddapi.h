/*   cddapi.h
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
* File Name:  cddapi.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   8/16/00
*
* $Revision: 1.5 $
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


#ifndef _CDDAPI_
#define _CDDAPI_

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


/* See synchronous and asynchronous code examples at bottom of this header */

/*
   Parameters are as follows:

   BioseqPtr bsp       - Query sequence
   FloatHi expectValue - Expectation value
   Boolean findBest    - Return only non-redundant CDs
   Boolean doFilter    - Filter out low sequence complexity
   Boolean makeFeats   - Return features (T) or alignments (F)
   CharPtr dataLib     - RPS-BLAST database
   Boolean precalc     - Use DART to avoid search
*/

/* low-level connection functions */

NLM_EXTERN CONN CddOpenConnection (
  BioseqPtr bsp,
  FloatHi expectValue,
  Boolean findBest,
  Boolean doFilter,
  Boolean makeFeats,
  CharPtr dataLib,
  Boolean precalc
);

NLM_EXTERN SeqAnnotPtr CddWaitForReply (
  CONN conn
);

/*
 CddSynchronousQuery opens connection, sends a
 uid request, and waits for a reply
*/

NLM_EXTERN SeqAnnotPtr CddSynchronousQuery (
  BioseqPtr bsp,
  FloatHi expectValue,
  Boolean findBest,
  Boolean doFilter,
  Boolean makeFeats,
  CharPtr dataLib,
  Boolean precalc
);

/*
 CddAsynchronousQuery opens connection, send request,
 and queues completion routine using urlquery queueing
 mechanism.

 CddCheckQueue should be called several times a second with
 a timer.  It calls QUERY_CheckQueue to poll connection,
 which calls completion routine when result is available,
 cleaning up connection afterwards.

 CddReadReply take conns and status parameters from completion
 routine and reads SeqAnnotPtr.
*/

NLM_EXTERN Boolean CddAsynchronousQuery (
  BioseqPtr bsp,
  FloatHi expectValue,
  Boolean findBest,
  Boolean doFilter,
  Boolean makeFeats,
  CharPtr dataLib,
  Boolean precalc,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 CddCheckQueue (
  QUEUE* queue
);

NLM_EXTERN SeqAnnotPtr CddReadReply (
  CONN conn,
  EIO_Status status
);

/*
 CddCorrectIDs should be called after the result is read
 in order to replace the temporary Seq-ids on the returned
 Seq-annot.
*/

NLM_EXTERN void CddCorrectIDs (
  BioseqPtr bsp,
  SeqAnnotPtr sap
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

#endif /* _CDDAPI_ */

