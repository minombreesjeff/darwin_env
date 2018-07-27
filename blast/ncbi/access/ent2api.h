/*   ent2api.h
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
* File Name:  ent2api.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/29/99
*
* $Revision: 1.25 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#ifndef _ENT2API_
#define _ENT2API_

#include <ncbi.h>
#include <asn.h>
#include <objent2.h>
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


/* See network connection test functions at bottom of this header */

/* See synchronous and asynchronous code examples at bottom of this header */

/* utility functions */

NLM_EXTERN void EntrezSetProgramName (
  CharPtr progname
);

/* to be phased out, currently overrides ncbi named service, goes directly to URL */

NLM_EXTERN void EntrezSetServer (
  CharPtr host_machine,
  Uint2 host_port,
  CharPtr host_path
);

/* use Entrez2Test to override default Entrez2 ncbi named service */

NLM_EXTERN void EntrezSetService (
  CharPtr service
);

/* low-level connection functions */

NLM_EXTERN CONN EntrezOpenConnection (
  void
);

NLM_EXTERN Entrez2ReplyPtr EntrezWaitForReply (
  CONN conn
);

/*
 EntrezSynchronousQuery opens connection, sends
 Entrez2Request ASN.1 query, and waits for reply,
 cleaning up connection afterwards.
*/

NLM_EXTERN Entrez2ReplyPtr EntrezSynchronousQuery (
  Entrez2RequestPtr e2rq
);

/*
 EntrezAsynchronousQuery opens connection, sends
 request, and queues completion routine using urlquery
 queueing mechanism.

 EntrezCheckQueue should be called several times a
 second with a timer.  It calls QUERY_CheckQueue to
 poll connection, which calls completion routine when
 result is available, cleaning up connection afterwards.

 EntrezReadReply takes conn and status parameters from
 completion routine and reads Entrez2ReplyPtr.
*/

NLM_EXTERN Boolean EntrezAsynchronousQuery (
  Entrez2RequestPtr e2rq,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 EntrezCheckQueue (
  QUEUE* queue
);

NLM_EXTERN Entrez2ReplyPtr EntrezReadReply (
  CONN conn,
  EIO_Status status
);

/* request creation functions */

NLM_EXTERN Entrez2IdListPtr EntrezCreateEntrezIdList (
  CharPtr db,
  Int4 uid,
  Int4 num,
  Int4Ptr uids,
  ByteStorePtr bs
);

NLM_EXTERN Entrez2LimitsPtr EntrezCreateEntrezLimits (
  Int4 begin_date,
  Int4 end_date,
  CharPtr type_date,
  Int4 max_uids,
  Int4 offset_uids
);

NLM_EXTERN Entrez2RequestPtr EntrezCreateGetInfoRequest (
  void
);

NLM_EXTERN Entrez2RequestPtr EntrezCreateBooleanRequest (
  Boolean return_uids,
  Boolean return_parsed,
  CharPtr db,
  CharPtr query_string,
  Int4 begin_date,
  Int4 end_date,
  CharPtr type_date,
  Int4 max_uids,
  Int4 offset_uids
);

#define ENTREZ_OP_NONE        0
#define ENTREZ_OP_AND         1
#define ENTREZ_OP_OR          2
#define ENTREZ_OP_BUTNOT      3
#define ENTREZ_OP_RANGE       4
#define ENTREZ_OP_LEFT_PAREN  5
#define ENTREZ_OP_RIGHT_PAREN 6

NLM_EXTERN void EntrezAddToBooleanRequest (
  Entrez2RequestPtr e2rp,
  CharPtr query_string,
  Int4 op,
  CharPtr field,
  CharPtr term,
  CharPtr key,
  Int4 uid,
  Int4 num,
  Int4Ptr uids,
  ByteStorePtr bs,
  Boolean do_not_explode,
  Boolean do_not_translate
);

NLM_EXTERN Entrez2RequestPtr EntrezCreateDocSumRequest (
  CharPtr db,
  Int4 uid,
  Int4 num,
  Int4Ptr uids,
  ByteStorePtr bs
);

NLM_EXTERN Entrez2RequestPtr EntrezCreateGetTermPositionRequest (
  CharPtr db,
  CharPtr field,
  CharPtr term
);

NLM_EXTERN Entrez2RequestPtr EntrezCreateGetTermListRequest (
  CharPtr db,
  CharPtr field,
  Int4 first_term_pos,
  Int4 num_terms
);

NLM_EXTERN Entrez2RequestPtr EntrezCreateGetTermHierarchyRequest (
  CharPtr db,
  CharPtr field,
  CharPtr term,
  Int4 txid
);

NLM_EXTERN Entrez2RequestPtr EntrezCreateGetLinksRequest (
  CharPtr db,
  Int4 uid,
  Int4 num,
  Int4Ptr uids,
  ByteStorePtr bs,
  CharPtr linktype,
  Int4 max_uids,
  Boolean count_only,
  Boolean parents_persist
);

NLM_EXTERN Entrez2RequestPtr EntrezCreateGetLinkedRequest (
  CharPtr db,
  Int4 uid,
  Int4 num,
  Int4Ptr uids,
  ByteStorePtr bs,
  CharPtr linktype,
  Int4 max_uids,
  Boolean count_only,
  Boolean parents_persist
);

NLM_EXTERN Entrez2RequestPtr EntrezCreateGetLinkCountsRequest (
  CharPtr db,
  Int4 uid
);

/* history needs to be used for Boolean ids and key queries */

NLM_EXTERN void EntrezSetUseHistoryFlag (
  Entrez2RequestPtr e2rq
);

/* reply extraction functions - these free the enclosing Entrez2ReplyPtr */

NLM_EXTERN CharPtr EntrezExtractErrorReply (
  Entrez2ReplyPtr e2ry
);

NLM_EXTERN Entrez2InfoPtr EntrezExtractInfoReply (
  Entrez2ReplyPtr e2ry
);

NLM_EXTERN Entrez2BooleanReplyPtr EntrezExtractBooleanReply (
  Entrez2ReplyPtr e2ry
);

NLM_EXTERN Entrez2DocsumListPtr EntrezExtractDocsumReply (
  Entrez2ReplyPtr e2ry
);

NLM_EXTERN Int4 EntrezExtractTermPosReply (
  Entrez2ReplyPtr e2ry
);

NLM_EXTERN Entrez2TermListPtr EntrezExtractTermListReply (
  Entrez2ReplyPtr e2ry
);

NLM_EXTERN Entrez2HierNodePtr EntrezExtractHierNodeReply (
  Entrez2ReplyPtr e2ry
);

NLM_EXTERN Entrez2LinkSetPtr EntrezExtractLinksReply (
  Entrez2ReplyPtr e2ry
);

NLM_EXTERN Entrez2IdListPtr EntrezExtractLinkedReply (
  Entrez2ReplyPtr e2ry
);

NLM_EXTERN Entrez2LinkCountListPtr EntrezExtractLinkCountReply (
  Entrez2ReplyPtr e2ry
);

/* special SeqIdString to UID convenience function */

NLM_EXTERN Uint4 EntrezGetUIDforSeqIdString (
  CharPtr db,
  CharPtr seq_id_string
);

/* result validation function */

NLM_EXTERN Boolean ValidateEntrez2InfoPtr (
  Entrez2InfoPtr e2ip,
  ValNodePtr PNTR head
);

/* sample synchronous query code:

  {
    Entrez2RequestPtr  e2rq;
    Entrez2ReplyPtr    e2ry;

    e2rq = EntrezCreateDocSumRequest ("nucleotide", 1322283, 0, NULL, NULL);
    e2ry = EntrezSynchronousQuery (e2rq);
    e2rq = Entrez2RequestFree (e2rq);
    ...
    Entrez2ReplyFree (e2ry); -- except if you already extracted subreply
    
  }

*/

/* sample asynchronous query code:

callback (completion routine):

  static Boolean LIBCALLBACK MyQueryResultProc (
    CONN conn,
    VoidPtr userdata,
    EIO_Status status
  )

  {
    Entrez2ReplyPtr  e2ry;

    e2ry = EntrezReadReply (conn, status);
    if (e2ry != NULL) {
      ...
      Entrez2ReplyFree (e2ry); -- except if you already extracted subreply
      return TRUE;
    }
    return FALSE;
  }

static queue variable:

  static QUEUE  myquerylist = NULL;

calling function:

  {
    Entrez2RequestPtr  e2rq;

    e2rq = EntrezCreateDocSumRequest ("nucleotide", 1322283, 0, NULL, NULL);
    EntrezAsynchronousQuery (e2rq, &myquerylist, MyQueryResultProc, NULL);
    e2rq = Entrez2RequestFree (e2rq);
  }

via timer call:

  {
    EntrezCheckQueue (&myquerylist);
  }

*/

/* network connection test functions */

NLM_EXTERN Boolean NetTestAsynchronousQuery (
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Boolean NetTestReadReply (
  CONN conn,
  EIO_Status status
);

NLM_EXTERN Int4 NetTestCheckQueue (
  QUEUE* queue
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

#endif /* _ENT2API_ */

