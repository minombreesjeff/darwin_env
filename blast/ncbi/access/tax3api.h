/*   tax3api.h
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
* File Name:  tax3api.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/8/04
*
* $Revision: 1.7 $
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


#ifndef _TAX3API_
#define _TAX3API_

#include <ncbi.h>
#include <asn.h>
#include <objtax3.h>
#include <urlquery.h>
#include <objfeat.h>

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

NLM_EXTERN CONN Tax3OpenConnection (
  void
);

NLM_EXTERN Taxon3ReplyPtr Tax3WaitForReply (
  CONN conn
);

/*
 Tax3SynchronousQuery opens connection, sends an
 ASN.1 request, and waits for a reply, cleaning
 up the connection afterwards.
*/

NLM_EXTERN Taxon3ReplyPtr Tax3SynchronousQuery (
  Taxon3RequestPtr t3rq
);

/*
 Tax3AsynchronousQuery opens connection, send request,
 and queues completion routine using urlquery queueing
 mechanism.

 Tax3CheckQueue should be called several times a second with
 a timer.  It calls QUERY_CheckQueue to poll connection,
 which calls completion routine when result is available,
 cleaning up connection afterwards.

 Tax3ReadReply take conns and status parameters from completion
 routine and reads Taxon3ReplyPtr.
*/

NLM_EXTERN Boolean Tax3AsynchronousQuery (
  Taxon3RequestPtr t3rq,
  QUEUE* queue,
  QueryResultProc resultproc,
  VoidPtr userdata
);

NLM_EXTERN Int4 Tax3CheckQueue (
  QUEUE* queue
);

NLM_EXTERN Taxon3ReplyPtr Tax3ReadReply (
  CONN conn,
  EIO_Status status
);


/* request creation function */

NLM_EXTERN Taxon3RequestPtr CreateTaxon3Request (
  Int4 taxid,
  CharPtr name,
  OrgRefPtr orp
);

NLM_EXTERN Taxon3RequestPtr CreateMultiTaxon3Request (ValNodePtr org_list);

NLM_EXTERN OrgRefPtr Taxon3GetOrg (OrgRefPtr orp);
NLM_EXTERN ValNodePtr Taxon3GetOrgRefList (ValNodePtr org_list);
NLM_EXTERN void Tax3MergeSourceDescr (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
NLM_EXTERN Int4 Taxon3GetTaxIdByOrgRef (OrgRefPtr orp);
NLM_EXTERN OrgRefPtr Taxon3GetOrgRefByName (CharPtr orgname);
NLM_EXTERN Int4 Taxon3GetTaxIdByName (CharPtr orgname);
NLM_EXTERN void Taxon3ReplaceOrgInSeqEntry (SeqEntryPtr sep, Boolean keep_syn);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _TAX3API_ */

