/*   entrez2.h
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
* File Name:  entrez2.h
*
* Author:  Jonathan Kans, Greg Schuler, Jonathan Epstein, Tim Ford
*
* Version Creation Date:   10/30/01
*
* $Revision: 6.5 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
*
* ==========================================================================
*/

#ifndef _ENTREZ2_
#define _ENTREZ2_

#include <vibrant.h>
#include <ent2api.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
*  The following callback types help process requests between forms.  The
*  form passed in belongs to the calling form, not the target form.  It is
*  up to the application to pass the request to the appropriate form.  This
*  way, there can be multiple term list and docsum forms, and the window
*  handles are not global to the libraries.
*/

typedef void (*E2RetrieveDocsProc) (ButtoN buttonPressed);
typedef void (*E2RetrieveUidProc) (ForM queryForm, Int4 uid, CharPtr dbName);
typedef void (*E2ProjectToFormProc) (ForM f, Pointer proj);
typedef void (*E2SimpleSeqProc) (ForM f, ValNodePtr simpleSeqs);
typedef void (*E2NamedUidListProc) (ForM f, CharPtr term, Int4 num, Int4Ptr uids, Int2 db);
typedef void (*E2LaunchViewerProc) (ForM f, Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db, Int2 launchType);

/*
*  The Entrez2GlobalsPtr may be registered with a call to SetAppProperty
*  e.g., SetAppProperty ("Entrez2Globals", &entrezglobals), where entrezglobals
*  is a persistent structure filled with Vibrant objects or callback
*  function pointers specific for a given application.
*/

typedef struct entrez2globals {
  IteM                 parentsPersist;
  IteM                 alignWithChecked;
  ChoicE               seqComplex;
  ChoicE               strucComplex;
  ChoicE               strucModels;
  FonT                 docsumFont;

  Boolean              persistDefault;
  Boolean              alignDefault;
  Boolean              popdownBehavior;
  Boolean              lookupDirect;
  Boolean              showAsn;
  Boolean              sortFields;
  CharPtr              initDatabase;
  CharPtr              initField;
  CharPtr              initMode;

  WndActnProc          createTrmLstMenus;
  WndActnProc          createDocSumMenus;

  E2RetrieveDocsProc   retrieveDocsProc;
  E2RetrieveUidProc    retrieveUidProc;
  E2ProjectToFormProc  retrieveProjectProc;
  E2SimpleSeqProc      retrieveSimpleProc;
  E2NamedUidListProc   loadNamedUidProc;
  E2LaunchViewerProc   launchViewerProc;
} Entrez2Globals, PNTR Entrez2GlobalsPtr;

NLM_EXTERN ForM CreateTermlistForm (
  Int2 left,
  Int2 top,
  CharPtr title,
  WndActnProc activateCallback,
  FormMessageFunc messagesCallback,
  E2RetrieveDocsProc retrieveCallback,
  E2RetrieveUidProc retrieveUidCallback,
  Boolean explodeToggle,
  Boolean advancedQueryToggle
);

NLM_EXTERN ForM CreateDocsumForm (
  Int2 left,
  Int2 top,
  CharPtr title,
  WndActnProc activate,
  FormMessageFunc messages,
  E2NamedUidListProc refineUidCallback,
  Boolean delayedNeighbor
);

NLM_EXTERN void ShowNewNetConfigForm (
  WndActnProc activate,
  FormMessageFunc messages,
  VoidProc accepted,
  VoidProc cancelled,
  VoidProc turnedOff,
  Boolean netCurrentlyOn
);

NLM_EXTERN void RetrieveDocs (ForM f, Int2 num, Int2 parents, Int4Ptr uids, CharPtr dbName);
NLM_EXTERN void LaunchRecViewer (ForM f, Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db, Int2 launchType);

NLM_EXTERN Boolean RefineUIDs (ForM f, CharPtr term, Int4 num, Int4Ptr uids, Int2 db);

NLM_EXTERN Boolean ShowASN (void);
NLM_EXTERN void DisplayEntrezReply (Entrez2ReplyPtr e2ry);
NLM_EXTERN void DisplayEntrezRequest (Entrez2RequestPtr e2rq);

NLM_EXTERN Int2 DBGetIDFromName (CharPtr dbName);
NLM_EXTERN CharPtr DBGetNameFromID (Int2 dbId);

NLM_EXTERN Entrez2InfoPtr Query_GetInfo (void);

NLM_EXTERN Entrez2BooleanReplyPtr Query_FetchUIDs (ForM f);
NLM_EXTERN Int4 Query_FetchCount (ForM f);
NLM_EXTERN Int4 Query_FetchParsedCount (ForM f);
NLM_EXTERN Entrez2TermListPtr Query_FetchSeveralCounts (CharPtr dbName, CharPtr fieldName, CharPtr searchStr, Int2 count);

NLM_EXTERN Boolean TermList_UnselectAll (ForM f);

NLM_EXTERN EnumFieldAssocPtr CreateDatabaseAlist (Entrez2InfoPtr e2ip);
NLM_EXTERN EnumFieldAssocPtr CreateFieldAlist (Entrez2DbInfoPtr e2db);

#ifdef __cplusplus
}
#endif

#endif /* _ENTREZ2_ */

