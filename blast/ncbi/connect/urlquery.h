/*   urlquery.h
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
* File Name:  urlquery.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   4/16/98
*
* $Revision: 6.12 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: urlquery.h,v $
* Revision 6.12  2003/09/03 21:15:29  lavr
* Reuse "arguments" in QUERY_OpenServiceQuery() to be real service argument
* (formely it was to modify the dispatcher and was not really used anywhere)
*
* Revision 6.11  2002/11/21 15:24:31  johnson
* changed 'queue' vars to 'q' to avoid stl conflict
*
* Revision 6.10  2001/06/07 20:07:41  kans
* added QUERY_OpenServiceQuery
*
* Revision 6.9  2001/02/21 22:02:04  lavr
* Changes for use new CONN interface
*
* Revision 6.8  2000/08/18 19:08:58  kans
* added QUERY_WaitForNextMacEvent, otherwise QuickDraw collides with mmdbapi
*
* Revision 6.7  2000/06/30 12:46:11  kans
* added QUERY_CloseQueue
*
* Revision 6.6  2000/06/29 18:27:10  kans
* QUERY_OpenUrlQuery has new EMIME_Type type and EMIME_Encoding encoding parameters
*
* Revision 6.5  2000/06/13 12:58:15  kans
* added closeConn parameter to QUERY_AddToQueue
*
* Revision 6.4  1999/07/28 21:05:23  vakatov
* Moved all #include's from inside the NLM_EXTERN & `extern "C"' block
*
* ==========================================================================
*/

#ifndef _URLQUERY_
#define _URLQUERY_

#include <ncbi.h>
#include <connect/ncbi_http_connector.h>
#include <connect/ncbi_service_connector.h>
#include <asn.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif


/* URL QUERY CONVENIENCE FUNCTIONS */

/*
  Initializes a POST connector based on URL query parameters.  For example:

  QUERY_OpenUrlQuery (
    "cruncher.nlm.nih.gov", 80,
    "/cgi-bin/Sequin/testcgi.cgi",
    "request=seg&window=12&lowcut=2.3&hicut=2.6",
    "Sequin", 30, eMIME_T_NcbiData, eMIME_Fasta, eENCOD_Url,
    fHCC_SureFlush | fHCC_UrlDecodeInput | fHCC_UrlEncodeOutput
  );

  The returned CONN value is then passed data before being sent to the cgi.
*/
NLM_EXTERN CONN QUERY_OpenUrlQuery (
  Nlm_CharPtr host_machine,
  Nlm_Uint2 host_port,
  Nlm_CharPtr host_path,
  Nlm_CharPtr arguments,
  Nlm_CharPtr appName,
  Nlm_Uint4 timeoutsec,
  EMIME_Type type,
  EMIME_SubType subtype,
  EMIME_Encoding encoding,
  THCC_Flags flags
);

/*
  Returns connection to NCBI named service.  Pass arguments (if any)
  via the connection to the service (if successful).  Return NULL on error.
*/

NLM_EXTERN CONN QUERY_OpenServiceQuery (
  Nlm_CharPtr service, Nlm_CharPtr arguments, Nlm_Uint4 timeoutsec
);

/*
  Calculates length of data written to connection, writes HTML header, calls
  CONN_Wait with a timeout of 0 to send the query without waiting for response.
  This should be called after all data are copied to the connection, typically
  either with CONN_Read, QUERY_CopyFileToQuery, or with QUERY_AsnIoConnOpen
  followed by a specific AsnWrite function.
*/
NLM_EXTERN void QUERY_SendQuery (
  CONN conn
);

/*
  Copies file to CONN by repeated calls to CONN_Write.  Writing of data must be
  done before the query is sent.  For the above seg example, a FASTA file of
  protein sequence data is appropriate.
*/
NLM_EXTERN void QUERY_CopyFileToQuery (
  CONN conn,
  FILE *fp
);

/*
  Copies results from CONN by repeated calls to CONN_Read.  Reading of data is
  only done when the connection is ready (CONN_Wait returns eCONN_Success).  In
  the query system below, the user's completion routine is called when results
  are ready to be read.  For the seg example, the results are in FASTA format
  with lower case x characters replacing amino acids in low-complexity regions.
*/
NLM_EXTERN void QUERY_CopyResultsToFile (
  CONN conn,
  FILE *fp
);

/* Structure for direct AsnIo-CONN link */
typedef struct asnioconn {    /* for AsnIo to and from a connection */
  AsnIoPtr aip;
  CONN conn;
} AsnIoConn, PNTR AsnIoConnPtr;

/*
  Creates structure containing AsnIoPtr attached to CONN.  Pass aicp->aip
  to AsnRead and AsnWrite functions.
*/
NLM_EXTERN AsnIoConnPtr QUERY_AsnIoConnOpen (
  Nlm_CharPtr mode,
  CONN conn
);

/*
  Frees AsnIoPtr and structure, CONN will be closed later by QUERY_CheckQueue.
*/
NLM_EXTERN AsnIoConnPtr QUERY_AsnIoConnClose (
  AsnIoConnPtr aicp
);

/* FUNCTIONS FOR MAINTAINING A QUEUE OF PENDING URL QUERIES */

/* Callback type for queued queries */
typedef Nlm_Boolean (LIBCALLBACK *QueryResultProc) (
  CONN conn, Nlm_VoidPtr userdata, EIO_Status status
);

/* Opaque handle type.  Variable must be kept by application and initialized
 * to NULL.
 */
struct SQueueTag;
typedef struct SQueueTag* QUEUE;  /* queue handle */

/*
  Records connection, completion routine, and user data in queue.
*/
NLM_EXTERN void QUERY_AddToQueue (
  QUEUE* q,
  CONN conn,
  QueryResultProc resultproc,
  Nlm_VoidPtr userdata,
  Nlm_Boolean closeConn
);

/*
  Checks queued connections (with CONN_Wait), calls completion routine, then
  removes query from queue and closes connection (if closeConn was TRUE).
  Application is responsible for calling QUERY_CheckQueue() every once in a
  while, typically with a timer.
*/
NLM_EXTERN Nlm_Int4 QUERY_CheckQueue (
  QUEUE* q
);

/*
  Forces all connections to be closed, removes all queries from queue.
*/
NLM_EXTERN void QUERY_CloseQueue (
  QUEUE* q
);

#ifdef OS_MAC
NLM_EXTERN void QUERY_WaitForNextMacEvent (
  void
);
#endif


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _URLQUERY_ */

