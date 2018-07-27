/*   sequin9.c
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
* File Name:  sequin9.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   4/20/99
*
* $Revision: 6.138 $
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

#include <subutil.h>
#include <explore.h>
#include <blast.h>
#include <alignmgr.h>
#include <urkptpf.h>
#include <entrez.h>
#include <accentr.h>
#include <urlquery.h>
#include <vecscnapi.h>
#include <qblastapi.h>
#include <dotseq.h>
#include <edutil.h>
#include <actutils.h>
#include "sequin.h"

extern void HandleProjectAsn (ProjectPtr proj, Uint2 entityID)

{
  Int2              db = -1;
  EntrezGlobalsPtr  egp;
  Int4              i;
  ValNodePtr        list;
  Int4              num;
  ValNodePtr        pip;
  Int4Ptr           uids;
  ValNodePtr        vnp;

  if (proj == NULL) return;
  if (! useEntrez) return;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL) return;
  pip = proj->data;
  if (pip == NULL) return;
  list = (ValNodePtr) pip->data.ptrvalue;
  if (list == NULL) return;
  if (pip->choice >= ProjectItem_protent && pip->choice <= ProjectItem_genomeent) {
    if (egp->retrieveProjectProc == NULL) return;
    if (! EntrezIsInited ()) {
      SequinEntrezInit ("Sequin", FALSE, NULL);
    }
    egp->retrieveProjectProc (NULL, (Pointer) proj);
    Update ();
    return;
  }
  if (egp->retrieveDocsProc == NULL) return;
  switch (pip->choice) {
    case ProjectItem_pmuid :
      db = 0;
      break;
    case ProjectItem_protuid :
      db = 1;
      break;
    case ProjectItem_nucuid :
      db = 2;
      break;
    case ProjectItem_genomeuid :
      db = 4;
      break;
    case ProjectItem_structuid :
      db = 3;
      break;
    default :
      break;
  }
  if (db == -1) return;
  if (! EntrezIsInited ()) {
    SequinEntrezInit ("Sequin", FALSE, NULL);
  }
  num = ValNodeLen (list);
  uids = MemNew ((size_t) (num * sizeof (Int4)));
  if (uids == NULL) return;
  for (i = 0, vnp = list; i < 32766 && vnp != NULL; i++, vnp = vnp->next) {
    uids [i] = vnp->data.intvalue;
  }
  if (egp->retrieveDocsProc != NULL) {
    egp->retrieveDocsProc (NULL, (Int2) num, 0, uids, db);
  }
  MemFree (uids);
  Update ();
}

/* BioseqViewOrDocSumChoice allows a single callback for each analysis item */
static Int2 BioseqViewOrDocSumChoice (NewObjectPtr nop)

{
  Int2  which = 0;   /* 1 = bioseq viewer, 2 = docsum window */

  if (nop == NULL) return 0;
#ifdef WIN_MAC
  if (bioseqViewUp) {
    which = 1;
  } else if (docSumUp) {
    which = 2;
  }
#else
  if (nop->bspOK) {
    which = 1;
  } else if (nop->dsmOK) {
    which = 2;
  }
#endif
  return which;
}

/*
#define TEST_APPLE_EVENT_MESSAGING
*/

#ifndef TEST_APPLE_EVENT_MESSAGING
static void AddRestrictionSite (SeqAnnotPtr annot, PackSeqPntPtr pspp, CharPtr name)

{
  DbtagPtr     dbt;
  ObjectIdPtr  oip;
  RsiteRefPtr  rrp;
  SeqFeatPtr   sfp, lastsfp;
  SeqLocPtr    slp;

  if (annot == NULL || pspp == NULL || name == NULL) return;
  slp = ValNodeNew (NULL);
  if (slp == NULL) return;
  slp->choice = SEQLOC_PACKED_PNT;
  slp->data.ptrvalue = (Pointer) pspp;
  sfp = SeqFeatNew ();
  if (sfp == NULL) return;

  sfp->data.choice = SEQFEAT_RSITE;
  sfp->location = slp;
  dbt = DbtagNew ();
  if (dbt != NULL) {
    dbt->db = StringSave ("REBASE");
    oip = ObjectIdNew ();
    if (oip != NULL) {
      oip->str = StringSave (name);
    }
    dbt->tag = oip;
  }
  rrp = ValNodeNew (NULL);
  if (rrp != NULL) {
    rrp->choice = 2;
    rrp->data.ptrvalue = dbt;
  }
  sfp->data.value.ptrvalue = (Pointer) rrp;

  if (annot->data == NULL) {
    annot->data = (Pointer) sfp;
  } else {
    lastsfp = (SeqFeatPtr) annot->data;
    while (lastsfp->next != NULL) {
      lastsfp = lastsfp->next;
    }
    lastsfp->next = sfp;
  }
}

static void RestrictionSearchProc (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  SeqAnnotPtr    annot;
  BioseqPtr      bsp;
  ComPatPtr      cpp, cpph;
  ValNodePtr     desc;
  SeqAnnotPtr    lastannot;
  PackSeqPntPtr  pspp;
  Int4           pt;
  SeqAlignPtr    sap;
  SeqLocPtr      slp, slpn;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  if (! ISA_na (bsp->mol)) return;

  desc = ValNodeNew (NULL);
  desc->choice = Annot_descr_name;
  desc->data.ptrvalue = StringSave ("cutsites");

  annot = SeqAnnotNew ();
  annot->type = 1;
  annot->desc = desc;
  annot->data = NULL;

  cpph = (ComPatPtr) mydata;
  cpp = cpph;
  while (cpp != NULL) {
    sap = PatternMatchBioseq (bsp, cpp, 0);
    slp = MatchSa2Sl (&sap);
    if (slp != NULL) {
      pspp = PackSeqPntNew ();
      pspp->id = SeqIdDup (SeqIdFindBest (bsp->id, 0));
      while (slp != NULL) {
        pt = SeqLocStart (slp);
        PackSeqPntPut (pspp, pt);
        slpn = slp->next;
        SeqLocFree (slp);
        slp = slpn;
      }
      AddRestrictionSite (annot, pspp, cpp->name);
    }
    cpp = cpp->nextpattern;
  }

  if (annot->data == NULL) {
    SeqAnnotFree (annot);
    return;
  }
  if (bsp->annot == NULL) {
    bsp->annot = annot;
  } else {
    lastannot = bsp->annot;
    while (lastannot->next != NULL) {
      lastannot = lastannot->next;
    }
    lastannot->next = annot;
  }
}
#endif

static void SimpleRsiteProc (IteM i)

{
  BaseFormPtr   bfp;
  BioseqPtr     bsp;
  NewObjectPtr  nop;
  SeqEntryPtr   sep = NULL;
  Int2          which;
#ifdef TEST_APPLE_EVENT_MESSAGING
  AsnIoPtr      aip;
  Char          tmp [PATH_MAX];
#else
  ComPatPtr     cpph;
  Char          enzyme [64];
  Int2          j;
  Char          temp [32];
  ValNodePtr    enzymes;
#endif

  nop = (NewObjectPtr) GetObjectExtra (i);
  if (nop == NULL) return;
#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = nop->bfp;
#endif
  if (bfp == NULL) return;
  which = BioseqViewOrDocSumChoice (nop);
  if (which != 1) return;
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
  } else {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  }
  if (sep == NULL) return;

#ifdef TEST_APPLE_EVENT_MESSAGING
  TmpNam (tmp);
  aip = AsnIoOpen (tmp, "w");
  if (aip != NULL) {
    SeqEntryAsnWrite (sep, aip, NULL);
    AsnIoClose (aip);
    /* Nlm_SendOpenDocAppleEventEx (tmp, "REST", NULL, TRUE); */
    Nlm_SendOpenDocAppleEventEx (tmp, NULL, "RsiteFind", TRUE);
  }
#else
  enzymes = NULL;
  j = 1;
  sprintf (temp, "ENZ_%d", (int) j);
  while (GetAppParam ("SEQNCGIS", "ENZYMES", temp, NULL, enzyme, sizeof (enzyme) - 1)) {
    ValNodeCopyStr (&enzymes, 0, enzyme);
    j++;
    sprintf (temp, "ENZ_%d", (int) j);
  }
  if (enzymes == NULL) {
    ValNodeCopyStr (&enzymes, 0, "BamHI");
    ValNodeCopyStr (&enzymes, 0, "EcoRI");
    ValNodeCopyStr (&enzymes, 0, "HindIII");
  }
  cpph = ReadRENPattern ("ncbiren.dat", FALSE, enzymes);
  /* PalindromeCheck (cpph); */
  SeqEntryExplore (sep, (Pointer) cpph, RestrictionSearchProc);
  ComPatFree (cpph);
  ValNodeFreeData (enzymes);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
#endif
}

static VQUEUE  vsquerylist = NULL;

static void LIBCALLBACK AnnounceCallback (CharPtr requestID, CharPtr seqID, Int2 estimatedSeconds)

{
  if (StringHasNoText (requestID)) {
    requestID = "?";
  }
  if (StringHasNoText (seqID)) {
    seqID = "?";
  }
  Message (MSG_POST, "Queued rID %s, seqID %s, estimated seconds = %d",
           requestID, seqID, (int) estimatedSeconds);
}

static Boolean LIBCALLBACK VecScreenCallback (
  CharPtr filename,
  VoidPtr userdata,
  CharPtr requestID,
  CharPtr seqID,
  Boolean success
)

{
  if (StringHasNoText (requestID)) {
    requestID = "?";
  }
  if (StringHasNoText (seqID)) {
    seqID = "?";
  }
  if (success) {
    if (! SequinHandleNetResults (filename)) {
      /* LaunchGeneralTextViewer (path, "QueueFastaQueryToURL failed"); */
    }
  } else {
    Message (MSG_POST, "Failure of rID '%s', seqID %s", requestID, seqID);
  }
  return TRUE;
}

static void SimpleUniVecScreenProc (IteM i)

{
  BaseFormPtr   bfp;
  BioseqPtr     bsp;
  NewObjectPtr  nop;
  SeqEntryPtr   sep = NULL;
  Int2          which;

  nop = (NewObjectPtr) GetObjectExtra (i);
  if (nop == NULL) return;
#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = nop->bfp;
#endif
  if (bfp == NULL) return;
  which = BioseqViewOrDocSumChoice (nop);
  if (which != 1) return;
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
  } else {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  }
  if (sep == NULL) return;

  VecScreenAsynchronousRequest ("UniVec", bsp, &vsquerylist, VecScreenCallback, AnnounceCallback, NULL);
}

static void SimpleUniVecCoreScreenProc (IteM i)

{
  BaseFormPtr   bfp;
  BioseqPtr     bsp;
  NewObjectPtr  nop;
  SeqEntryPtr   sep = NULL;
  Int2          which;

  nop = (NewObjectPtr) GetObjectExtra (i);
  if (nop == NULL) return;
#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = nop->bfp;
#endif
  if (bfp == NULL) return;
  which = BioseqViewOrDocSumChoice (nop);
  if (which != 1) return;
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
  } else {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  }
  if (sep == NULL) return;

  VecScreenAsynchronousRequest ("UniVec_Core", bsp, &vsquerylist, VecScreenCallback, AnnounceCallback, NULL);
}

static QBQUEUE  qbquerylist = NULL;

static void LIBCALLBACK QBAnnounceCallback (CharPtr requestID, CharPtr seqID, Int2 estimatedSeconds)

{
  if (StringHasNoText (requestID)) {
    requestID = "?";
  }
  if (StringHasNoText (seqID)) {
    seqID = "?";
  }
  Message (MSG_POST, "Queued rID %s, seqID %s, estimated seconds = %d",
           requestID, seqID, (int) estimatedSeconds);
}

static Boolean LIBCALLBACK QBCallback (
  CharPtr filename,
  VoidPtr userdata,
  CharPtr requestID,
  CharPtr seqID,
  Boolean success
)

{
  if (StringHasNoText (requestID)) {
    requestID = "?";
  }
  if (StringHasNoText (seqID)) {
    seqID = "?";
  }
  if (success) {
    if (! SequinHandleNetResults (filename)) {
      /* LaunchGeneralTextViewer (path, "QueueFastaQueryToURL failed"); */
    }
  } else {
    Message (MSG_POST, "Failure of rID '%s', seqID %s", requestID, seqID);
  }
  return TRUE;
}

static void SimpleQBlastProc (IteM i)

{
  BaseFormPtr   bfp;
  BioseqPtr     bsp;
  NewObjectPtr  nop;
  SeqEntryPtr   sep = NULL;
  Int2          which;

  nop = (NewObjectPtr) GetObjectExtra (i);
  if (nop == NULL) return;
#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = nop->bfp;
#endif
  if (bfp == NULL) return;
  which = BioseqViewOrDocSumChoice (nop);
  if (which != 1) return;
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
  } else {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  }
  if (sep == NULL) return;

  QBlastAsynchronousRequest ("nr", "blastn", bsp, &qbquerylist, QBCallback, QBAnnounceCallback, NULL);
}

/* Analysis menu can launch external programs or use Web services */

static QUEUE  urlquerylist = NULL;

static Int4 pendingqueries = 0;

static Boolean LIBCALLBACK SubmitToNCBIResultProc (CONN conn, VoidPtr userdata, EIO_Status status)

{
  AsnIoPtr     aop;
  FILE         *fp;
  Char         path [PATH_MAX];
  SeqEntryPtr  sep;

  TmpNam (path);
  fp = FileOpen (path, "wb");
  QUERY_CopyResultsToFile (conn, fp);
  FileClose (fp);
  aop = AsnIoOpen (path, "rb");
  sep = SeqEntryAsnRead (aop, NULL);
  AsnIoClose (aop);
  aop = AsnIoOpen (path, "w");
  SeqEntryAsnWrite (sep, aop, NULL);
  AsnIoFlush (aop);
  AsnIoClose (aop);
  LaunchGeneralTextViewer (path, "Echo binary transformation of Seq-entry");
  FileRemove (path);
  return TRUE;
}

extern void SubmitToNCBI (IteM i);
extern void SubmitToNCBI (IteM i)

{
  AsnIoPtr     aop;
  BaseFormPtr  bfp;
  CONN         conn;
  FILE         *fp;
  Char         path [PATH_MAX];
  Char         progname [64];
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  sprintf (progname, "Sequin/%s", SEQUIN_APPLICATION);

  TmpNam (path);

  aop = AsnIoOpen (path, "wb");
  SeqEntryAsnWrite (sep, aop, NULL);
  AsnIoFlush (aop);
  AsnIoClose (aop);

  conn = QUERY_OpenUrlQuery ("cruncher.nlm.nih.gov", 80,
                             "/cgi-bin/Sequin/testcgi.cgi", "request=echo",
                             progname, 30, eMIME_T_NcbiData, eMIME_AsnBinary,
                             eENCOD_Url,
                             fHCC_UrlDecodeInput | fHCC_UrlEncodeOutput);

  fp = FileOpen (path, "rb");
  QUERY_CopyFileToQuery (conn, fp);
  FileClose (fp);

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (&urlquerylist, conn, SubmitToNCBIResultProc, NULL, TRUE);

  pendingqueries++;

  FileRemove (path);
}

extern void SequinCheckSocketsProc (void)

{
  Int4  remaining;

  remaining = QUERY_CheckQueue (&urlquerylist);
  if (remaining < pendingqueries) {
    Beep ();
    pendingqueries--;
  }
  remaining = VecScreenCheckQueue (&vsquerylist);
  remaining = QBlastCheckQueue (&qbquerylist);
}

static Boolean LIBCALLBACK DemoModeResultProc (CONN conn, VoidPtr userdata, EIO_Status status)

{
  FILE  *fp;
  Char  path [PATH_MAX];

  TmpNam (path);
  fp = FileOpen (path, "w");
  QUERY_CopyResultsToFile (conn, fp);
  FileClose (fp);
  LaunchGeneralTextViewer (path, "QueueFastaQueryToURL results");
  FileRemove (path);
  return TRUE;
}

static Boolean LIBCALLBACK SequinHandleURLResults (CONN conn, VoidPtr userdata, EIO_Status status)

{
  FILE  *fp;
  Char  path [PATH_MAX];

  TmpNam (path);
  fp = FileOpen (path, "w");
  QUERY_CopyResultsToFile (conn, fp);
  FileClose (fp);
  if (! SequinHandleNetResults (path)) {
    /* LaunchGeneralTextViewer (path, "QueueFastaQueryToURL failed"); */
  }
  FileRemove (path);
  return TRUE;
}

static void FinishURLProc (NewObjectPtr nop, CharPtr arguments, CharPtr path)

{
  CONN             conn;
  FILE             *fp;
  Char             progname [64];
  QueryResultProc  resultproc;
  EMIME_SubType    subtype;

  sprintf (progname, "Sequin/%s", SEQUIN_APPLICATION);

  if (nop->demomode) {
    resultproc = DemoModeResultProc;
  } else {
    resultproc = nop->resultproc;
  }
  if (nop->format == 1) {
    subtype = eMIME_Fasta;
  } else if (nop->format == 2) {
    subtype = eMIME_AsnText;
  } else {
    subtype = eMIME_Unknown;
  }

  conn = QUERY_OpenUrlQuery (nop->host_machine, nop->host_port,
                             nop->host_path, arguments,
                             progname, nop->timeoutsec,
                             eMIME_T_NcbiData, subtype, eENCOD_Url,
                             fHCC_UrlDecodeInput | fHCC_UrlEncodeOutput);
  if (conn == NULL) return;

  fp = FileOpen (path, "r");
  QUERY_CopyFileToQuery (conn, fp);
  FileClose (fp);

  QUERY_SendQuery (conn);

  QUERY_AddToQueue (&urlquerylist, conn, resultproc, NULL, TRUE);

  pendingqueries++;
}

static void DoAnalysisProc (NewObjectPtr nop, BaseFormPtr bfp, Int2 which, CharPtr arguments, ResultProc dotheanalysis)

{
  AsnIoPtr     aop;
  BioseqPtr    bsp;
  Char         path1 [PATH_MAX];
  SeqEntryPtr  sep;

  if (nop == NULL || bfp == NULL) return;
  switch (which) {
    case 1 :
      if (BioseqViewCanSaveFasta (bfp->form, nop->fastaNucOK, nop->fastaProtOK, nop->onlyBspTarget)) {
        TmpNam (path1);
        switch (nop->format) {
          case 1 :
            ExportBioseqViewFasta (bfp->form, path1, nop->fastaNucOK, nop->fastaProtOK, nop->onlyBspTarget);
            break;
          case 2 :
            sep = NULL;
            if (nop->onlyBspTarget) {
              bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
              sep = SeqMgrGetSeqEntryForData (bsp);
            } else {
              sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
            }
            if (sep != NULL) {
              aop = AsnIoOpen (path1, "w");
              SeqEntryAsnWrite (sep, aop, NULL);
              AsnIoFlush (aop);
              AsnIoClose (aop);
            }
            break;
          default :
            break;
        }
        if (dotheanalysis != NULL) {
          dotheanalysis (path1);
        } else {
          FinishURLProc (nop, arguments, path1);
        }
        FileRemove (path1);
      } else {
        ErrPostEx (SEV_ERROR, 0, 0, "BioseqView cannot save fasta format");
      }
      break;
    case 2 :
      if (DocSumCanSaveFasta (bfp->form, nop->fastaNucOK, nop->fastaProtOK)) {
        TmpNam (path1);
        ExportDocSumFasta (bfp->form, path1, nop->fastaNucOK, nop->fastaProtOK);
        if (dotheanalysis != NULL) {
          dotheanalysis (path1);
        } else {
          FinishURLProc (nop, arguments, path1);
        }
        FileRemove (path1);
      } else {
        ErrPostEx (SEV_ERROR, 0, 0, "DocSum cannot save fasta format");
      }
      break;
    default :
      break;
  }
}

/* encodes spaces as %20 in URLs */
static CharPtr StrSaveNoNullEncodeSpaces (CharPtr from)

{
  Char     ch;
  size_t   len = 0;
  CharPtr  p;
  CharPtr  q;
  CharPtr  to;

  if (StringHasNoText (from)) return NULL;
  p = from;
  ch = *p;
  while (ch != '\0') {
    if (ch == ' ') {
      len += 3;
    } else {
      len++;
    }
    p++;
    ch = *p;
  }
  to = MemNew (len + 1);
  if (to == NULL) return NULL;

  q = to;
  p = from;
  ch = *p;
  while (ch != '\0') {
    if (ch == ' ') {
      *q = '%';
      q++;
      *q = '2';
      q++;
      *q = '0';
      q++;
    } else {
      *q = ch;
      q++;
    }
    p++;
    ch = *p;
  }
  *q = '\0';
  return to;
}

typedef struct urlargform {
  FORM_MESSAGE_BLOCK

  NewObjectPtr nop;
  BaseFormPtr  bfp;
  ValNodePtr   controls;
  ValNodePtr   helps;
  Int2         which;
} UrlArgForm, PNTR UrlArgFormPtr;

static void AcceptArgumentFormProc (ButtoN b)

{
  CharPtr        args = NULL;
  CharPtr        arguments = NULL;
  ButtoN         btn;
  Char           ch;
  Int2           choice;
  Char           cpy [256];
  GrouP          grp;
  ValNodePtr     head = NULL;
  Int2           i;
  CharPtr        itms;
  CharPtr        last;
  size_t         len;
  LisT           lst;
  NewObjectPtr   nop;
  Boolean        notFirst = FALSE;
  PopuP          pop;
  ValNodePtr     ppt;
  CharPtr        ptr;
  CharPtr        str;
  Char           tmp [256];
  TexT           txt;
  UrlArgFormPtr  ufp;
  UrlParamPtr    upp;
  Int2           val;
  ValNodePtr     vnp;

  ufp = (UrlArgFormPtr) GetObjectExtra (b);
  if (ufp == NULL) return;
  Hide (ufp->form);
  Update ();
  nop = ufp->nop;
  if (nop != NULL) {
    if (! StringHasNoText (nop->prefix)) {
      ValNodeCopyStr (&head, 0, nop->prefix);
    }
    for (vnp = ufp->controls, ppt = nop->paramlist;
         vnp != NULL && ppt != NULL;
         vnp = vnp->next, ppt = ppt->next) {
      upp = (UrlParamPtr) ppt->data.ptrvalue;
      if (upp == NULL) continue;
      choice = vnp->choice;
      switch (upp->type) {
        case 1 :
          txt = (TexT) vnp->data.ptrvalue;
          str = SaveStringFromText (txt);
          if (str != NULL) {
            sprintf (tmp, "%s=%s", upp->param, str);
            ValNodeCopyStr (&head, ppt->choice, tmp);
            MemFree (str);
          }
          break;
        case 2 :
          btn = (ButtoN) vnp->data.ptrvalue;
          if (GetStatus (btn)) {
            sprintf (tmp, "%s=TRUE", upp->param);
          } else {
            sprintf (tmp, "%s=FALSE", upp->param);
          }
          ValNodeCopyStr (&head, ppt->choice, tmp);
          break;
        case 3 :
          pop = (PopuP) vnp->data.ptrvalue;
          val = GetValue (pop);
          if (val > 0) {
            i = 0;
            itms = upp->choices;
            StringNCpy_0 (tmp, itms, sizeof (tmp));
            last = tmp;
            ptr = last;
            ch = *ptr;
            while (ch != '\0') {
              if (ch == ',') {
                *ptr = '\0';
                i++;
                if (val == i) {
                  sprintf (cpy, "%s=%s", upp->param, last);
                  ValNodeCopyStr (&head, ppt->choice, cpy);
                }
                ptr++;
                last = ptr;
                ch = *ptr;
              } else {
                ptr++;
                ch = *ptr;
              }
            }
            if (! StringHasNoText (last)) {
              i++;
              if (val == i) {
                sprintf (cpy, "%s=%s", upp->param, last);
                ValNodeCopyStr (&head, ppt->choice, cpy);
              }
            }
          }
          break;
        case 4 :
          grp = (GrouP) vnp->data.ptrvalue;
          val = GetValue (grp);
          if (val > 0) {
            i = 0;
            itms = upp->choices;
            StringNCpy_0 (tmp, itms, sizeof (tmp));
            last = tmp;
            ptr = last;
            ch = *ptr;
            while (ch != '\0') {
              if (ch == ',') {
                *ptr = '\0';
                i++;
                if (val == i) {
                  sprintf (cpy, "%s=%s", upp->param, last);
                  ValNodeCopyStr (&head, ppt->choice, cpy);
                }
                ptr++;
                last = ptr;
                ch = *ptr;
              } else {
                ptr++;
                ch = *ptr;
              }
            }
            if (! StringHasNoText (last)) {
              i++;
              if (val == i) {
                sprintf (cpy, "%s=%s", upp->param, last);
                ValNodeCopyStr (&head, ppt->choice, cpy);
              }
            }
          }
          break;
        case 5 :
          lst = (LisT) vnp->data.ptrvalue;
          val = GetValue (lst);
          if (val > 0) {
            i = 0;
            itms = upp->choices;
            StringNCpy_0 (tmp, itms, sizeof (tmp));
            last = tmp;
            ptr = last;
            ch = *ptr;
            while (ch != '\0') {
              if (ch == ',') {
                *ptr = '\0';
                i++;
                if (val == i) {
                  sprintf (cpy, "%s=%s", upp->param, last);
                  ValNodeCopyStr (&head, ppt->choice, cpy);
                }
                ptr++;
                last = ptr;
                ch = *ptr;
              } else {
                ptr++;
                ch = *ptr;
              }
            }
            if (! StringHasNoText (last)) {
              i++;
              if (val == i) {
                sprintf (cpy, "%s=%s", upp->param, last);
                ValNodeCopyStr (&head, ppt->choice, cpy);
              }
            }
          }
          break;
        default :
          break;
      }
    }
    head = SortValNode (head, SortByVnpChoice);
    if (! StringHasNoText (nop->suffix)) {
      ValNodeCopyStr (&head, 0, nop->suffix);
    }
    for (len = 0, vnp = head; vnp != NULL; vnp = vnp->next) {
      len += StringLen ((CharPtr) vnp->data.ptrvalue) + 1;
    }
    if (len > 0) {
      arguments = MemNew (len + 5);
      if (arguments != NULL) {
        vnp = head;
        notFirst = FALSE;
        while (vnp != NULL) {
          if (notFirst) {
            StringCat (arguments, "&");
          }
          StringCat (arguments, (CharPtr) vnp->data.ptrvalue);
          notFirst = TRUE;
          vnp = vnp->next;
        }
      }
    }
    args = /* StrSaveNoNullEncodeSpaces */ StringSave (arguments);
    MemFree (arguments);
    DoAnalysisProc (nop, ufp->bfp, ufp->which, args, NULL);
    MemFree (args);
  }
  Remove (ufp->form);
}

static void ShowArgumentHelp (ButtoN b)

{
  NewObjectPtr   nop;
  ValNodePtr     ppt;
  CharPtr        str;
  UrlArgFormPtr  ufp;
  UrlParamPtr    upp;
  ValNodePtr     vnp;

  ufp = (UrlArgFormPtr) GetObjectExtra (b);
  if (ufp == NULL) return;
  nop = ufp->nop;
  if (nop == NULL) return;
  for (vnp = ufp->helps, ppt = nop->paramlist;
         vnp != NULL && ppt != NULL;
         vnp = vnp->next, ppt = ppt->next) {
    upp = (UrlParamPtr) ppt->data.ptrvalue;
    if (upp == NULL) continue;
    if ((Pointer) b == (Pointer) vnp->data.ptrvalue) {
      str = upp->help;
      Message (MSG_OK, "%s", str);
      return;
    }
  }
  Beep ();
}

static void ArgumentFormMessage (ForM f, Int2 mssg)

{
  UrlArgFormPtr  ufp;

  ufp = (UrlArgFormPtr) GetObjectExtra (f);
  if (ufp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      default :
        if (ufp->appmessage != NULL) {
          ufp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void CleanupArgumentForm (GraphiC g, VoidPtr data)

{
  UrlArgFormPtr  ufp;

  ufp = (UrlArgFormPtr) data;
  if (ufp != NULL) {
    ValNodeFree (ufp->controls);
    ValNodeFree (ufp->helps);
  }
  StdCleanupFormProc (g, data);
}

static ValNodePtr RearrangeParamList (ValNodePtr paramlist)

{
  ValNodePtr       curr;
  CharPtr          group;
  ValNodePtr       head = NULL;
  ValNodePtr       list;
  ValNodePtr       next;
  ValNodePtr PNTR  prev;
  ValNodePtr       ppt;
  UrlParamPtr      upp;

  ppt = paramlist;
  while (ppt != NULL) {
    list = ppt->next;
    ppt->next = NULL;
    ValNodeLink (&head, ppt);
    upp = (UrlParamPtr) ppt->data.ptrvalue;
    if (upp == NULL) {
      ppt = list;
      continue;
    }
    group = upp->group;
    curr = list;
    prev = &list;
    while (curr != NULL) {
      next = curr->next;
      upp = (UrlParamPtr) curr->data.ptrvalue;
      if (upp == NULL) {
        prev = &(curr->next);
        curr = next;
        continue;
      }
      if (StringICmp (upp->group, group) == 0) {
        *prev = next;
        curr->next = NULL;
        ValNodeLink (&head, curr);
      } else {
        prev = &(curr->next);
      }
      curr = next;
    }
    ppt = list;
  }
  return head;
}

static void BuildArgumentForm (NewObjectPtr nop, BaseFormPtr bfp, Int2 which)

{
  ButtoN             b;
  ButtoN             btn;
  GrouP              c;
  Char               ch;
  CharPtr            def;
  Int2               delta;
  TexT               first = NULL;
  GrouP              g;
  GrouP              grp;
  GrouP              h;
  ValNodePtr         hlp;
  Int2               i;
  CharPtr            itms;
  CharPtr            last;
  CharPtr            lastGroup = " ";
  LisT               lst;
  GrouP              m;
  Int2               max;
  Int2               min;
  ValNodePtr         moveMe = NULL;
  Nlm_Handle         obj1, obj2;
  PopuP              pop;
  PrompT             prmpt;
  ValNodePtr         ppt;
  CharPtr            ptr;
  RecT               r;
  StdEditorProcsPtr  sepp;
  CharPtr            str;
  Char               tmp [128];
  TexT               txt;
  UrlArgFormPtr      ufp;
  UrlParamPtr        upp;
  Int2               val;
  ValNodePtr         vnp;
  WindoW             w;

  if (nop == NULL || bfp == NULL) return;
  ufp = (UrlArgFormPtr) MemNew (sizeof (UrlArgForm));
  if (ufp == NULL) return;

  nop->paramlist = RearrangeParamList (nop->paramlist);

  w = FixedWindow (-50, -33, -10, -10, "Arguments", NULL);
  SetObjectExtra (w, ufp, CleanupArgumentForm);
  ufp->form = (ForM) w;
  ufp->formmessage = ArgumentFormMessage;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    ufp->appmessage = sepp->handleMessages;
  }

  ufp->bfp = bfp;
  ufp->nop = nop;
  ufp->which = which;

  m = HiddenGroup (w, 1, 0, NULL);

  g = NULL;
  for (ppt = nop->paramlist;
       ppt != NULL;
       ppt = ppt->next) {
    upp = (UrlParamPtr) ppt->data.ptrvalue;
    if (upp == NULL) continue;
    if (StringICmp (upp->group, lastGroup) != 0) {
      if (StringHasNoText (upp->group)) {
        if (StringHasNoText (lastGroup)) {
          g = HiddenGroup (m, 3, 0, NULL);
        } else {
          g = NormalGroup (m, 3, 0, "", programFont, NULL);
        }
      } else {
        g = NormalGroup (m, 3, 0, upp->group, programFont, NULL);
      }
      lastGroup = upp->group;
    }
    if (g == NULL) {
      g = HiddenGroup (m, 3, 0, NULL);
    }
    switch (upp->type) {
      case 1 :
        str = upp->prompt;
        StaticPrompt (g, str, 0, dialogTextHeight, programFont, 'l');
        def = upp->dfault;
        if (StringHasNoText (def)) {
          def = "";
        }
        txt = DialogText (g, def, 10, NULL);
        if (first == NULL) {
          first = txt;
        }
        ValNodeAddPointer (&(ufp->controls), 1, (Pointer) txt);
        ValNodeAddPointer (&moveMe, 0, (Pointer) txt);
        b = PushButton (g, "?", ShowArgumentHelp);
        SetObjectExtra (b, ufp, NULL);
        ValNodeAddPointer (&(ufp->helps), 0, (Pointer) b);
        break;
      case 2 :
        str = upp->prompt;
        btn = CheckBox (g, str, NULL);
        def = upp->dfault;
        if (StringICmp (def, "TRUE") == 0) {
          SetStatus (btn, TRUE);
        }
        prmpt = StaticPrompt (g, "", 0, 0, programFont, 'l');
        ValNodeAddPointer (&moveMe, 0, (Pointer) prmpt);
        ValNodeAddPointer (&(ufp->controls), 2, (Pointer) btn);
        b = PushButton (g, "?", ShowArgumentHelp);
        SetObjectExtra (b, ufp, NULL);
        ValNodeAddPointer (&(ufp->helps), 0, (Pointer) b);
        break;
      case 3 :
        str = upp->prompt;
        StaticPrompt (g, str, 0, dialogTextHeight, programFont, 'l');
        h = HiddenGroup (g, 1, 0, NULL);
        pop = PopupList (h, TRUE, NULL);
        def = upp->dfault;
        val = 0;
        i = 0;
        itms = upp->choices;
        StringNCpy_0 (tmp, itms, sizeof (tmp));
        last = tmp;
        ptr = last;
        ch = *ptr;
        while (ch != '\0') {
          if (ch == ',') {
            *ptr = '\0';
            PopupItem (pop, last);
            i++;
            if (StringICmp (def, last) == 0) {
              val = i;
            }
            ptr++;
            last = ptr;
            ch = *ptr;
          } else {
            ptr++;
            ch = *ptr;
          }
        }
        if (! StringHasNoText (last)) {
          PopupItem (pop, last);
          i++;
          if (StringICmp (def, last) == 0) {
            val = i;
          }
        }
        if (val > 0) {
          SetValue (pop, val);
        }
        ValNodeAddPointer (&(ufp->controls), 3, (Pointer) pop);
        ValNodeAddPointer (&moveMe, 0, (Pointer) pop);
        b = PushButton (g, "?", ShowArgumentHelp);
        SetObjectExtra (b, ufp, NULL);
        ValNodeAddPointer (&(ufp->helps), 0, (Pointer) b);
        break;
      case 4 :
        str = upp->prompt;
        StaticPrompt (g, str, 0, dialogTextHeight, programFont, 'l');
        h = HiddenGroup (g, 1, 0, NULL);
        grp = HiddenGroup (h, -3, 0, NULL);
        def = upp->dfault;
        val = 0;
        i = 0;
        itms = upp->choices;
        StringNCpy_0 (tmp, itms, sizeof (tmp));
        last = tmp;
        ptr = last;
        ch = *ptr;
        while (ch != '\0') {
          if (ch == ',') {
            *ptr = '\0';
            RadioButton (grp, last);
            i++;
            if (StringICmp (def, last) == 0) {
              val = i;
            }
            ptr++;
            last = ptr;
            ch = *ptr;
          } else {
            ptr++;
            ch = *ptr;
          }
        }
        if (! StringHasNoText (last)) {
          RadioButton (grp, last);
          i++;
          if (StringICmp (def, last) == 0) {
            val = i;
          }
        }
        if (val > 0) {
          SetValue (grp, val);
        }
        ValNodeAddPointer (&(ufp->controls), 4, (Pointer) grp);
        ValNodeAddPointer (&moveMe, 0, (Pointer) grp);
        b = PushButton (g, "?", ShowArgumentHelp);
        SetObjectExtra (b, ufp, NULL);
        ValNodeAddPointer (&(ufp->helps), 0, (Pointer) b);
        break;
      case 5 :
        str = upp->prompt;
        StaticPrompt (g, str, 0, dialogTextHeight, programFont, 'l');
        h = HiddenGroup (g, 1, 0, NULL);
        lst = SingleList (h, 10, 3, NULL);
        def = upp->dfault;
        val = 0;
        i = 0;
        itms = upp->choices;
        StringNCpy_0 (tmp, itms, sizeof (tmp));
        last = tmp;
        ptr = last;
        ch = *ptr;
        while (ch != '\0') {
          if (ch == ',') {
            *ptr = '\0';
            ListItem (lst, last);
            i++;
            if (StringICmp (def, last) == 0) {
              val = i;
            }
            ptr++;
            last = ptr;
            ch = *ptr;
          } else {
            ptr++;
            ch = *ptr;
          }
        }
        if (! StringHasNoText (last)) {
          ListItem (lst, last);
          i++;
          if (StringICmp (def, last) == 0) {
            val = i;
          }
        }
        if (val > 0) {
          SetValue (lst, val);
        }
        ValNodeAddPointer (&(ufp->controls), 5, (Pointer) lst);
        ValNodeAddPointer (&moveMe, 0, (Pointer) lst);
        b = PushButton (g, "?", ShowArgumentHelp);
        SetObjectExtra (b, ufp, NULL);
        ValNodeAddPointer (&(ufp->helps), 0, (Pointer) b);
        break;
      default :
        break;
    }
  }

  min = 0;
  max = 0;
  for (vnp = moveMe; vnp != NULL; vnp = vnp->next) {
    obj1 = (Nlm_Handle) vnp->data.ptrvalue;
    GetPosition (obj1, &r);
    min = MAX (min, r.left);
  }
  for (vnp = moveMe; vnp != NULL; vnp = vnp->next) {
    obj1 = (Nlm_Handle) vnp->data.ptrvalue;
    GetPosition (obj1, &r);
    delta = min - r.left;
    OffsetRect (&r, delta, 0);
    SetPosition (obj1, &r);
    AdjustPrnt (obj1, &r, FALSE);
    max = MAX (max, r.right);
  }
  max += 3;
  for (vnp = moveMe, hlp = ufp->helps;
       vnp != NULL && hlp != NULL;
       vnp = vnp->next, hlp = hlp->next) {
    obj2 = (Nlm_Handle) hlp->data.ptrvalue;
    GetPosition (obj2, &r);
    delta = max - r.left;
    OffsetRect (&r, delta, 0);
    SetPosition (obj2, &r);
    AdjustPrnt (obj2, &r, TRUE);
  }

  c = HiddenGroup (w, 2, 0, NULL);
  SetGroupSpacing (c, 10, 3);
  b = PushButton (c, "Accept", AcceptArgumentFormProc);
  SetObjectExtra (b, ufp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) m, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
  if (first != NULL) {
    Select (first);
  }
}

static void DoURLProc (IteM i)

{
  CharPtr       args = NULL;
  BaseFormPtr   bfp;
  size_t        len;
  NewObjectPtr  nop;
  Int2          which;

  nop = (NewObjectPtr) GetObjectExtra (i);
  if (nop == NULL) return;
#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = nop->bfp;
#endif
  if (bfp == NULL) return;
  which = BioseqViewOrDocSumChoice (nop);
  if (nop->paramlist == NULL) {
    len = StringLen (nop->prefix) + StringLen (nop->suffix);
    if (len > 0) {
      args = MemNew (sizeof (Char) * (len + 2));
      StringCpy (args, nop->prefix);
      if (! StringHasNoText (nop->suffix)) {
        StringCat (args, "&");
        StringCat (args, nop->suffix);
      }
    }
    DoAnalysisProc (nop, bfp, which, args, NULL);
  } else {
    BuildArgumentForm (nop, bfp, which);
  }
}

extern void EnableAnalysisItems (BaseFormPtr bfp, Boolean isDocSum)

{
  Boolean       hasFastaNuc;
  Boolean       hasFastaProt;
  NewObjectPtr  nop;

  if (bfp == NULL) return;
#ifdef WIN_MAC
  nop = (NewObjectPtr) macUserDataPtr;
#else
  nop = (NewObjectPtr) bfp->userDataPtr;
#endif
  if (isDocSum) {
  } else {
  }
  while (nop != NULL) {
    if (nop->kind == 1) {
      /* annotate menu item, ignore it */
    } else if (isDocSum) {
      if (nop->dsmOK) {
        hasFastaNuc = DocSumCanSaveFasta (bfp->form, TRUE, FALSE);
        hasFastaProt = DocSumCanSaveFasta (bfp->form, FALSE, TRUE);
        if (nop->fastaNucOK && hasFastaNuc) {
          SafeEnable (nop->item);
        } else if (nop->fastaProtOK && hasFastaProt) {
          SafeEnable (nop->item);
        } else {
          SafeDisable (nop->item);
        }
      } else {
        SafeDisable (nop->item);
      }
    } else {
      if (nop->bspOK) {
        hasFastaNuc = BioseqViewCanSaveFasta (bfp->form, TRUE, FALSE, nop->onlyBspTarget);
        hasFastaProt = BioseqViewCanSaveFasta (bfp->form, FALSE, TRUE, nop->onlyBspTarget);
        if (nop->fastaNucOK && hasFastaNuc) {
          SafeEnable (nop->item);
        } else if (nop->fastaProtOK && hasFastaProt) {
          SafeEnable (nop->item);
        } else {
          SafeDisable (nop->item);
        }
      } else {
        SafeDisable (nop->item);
      }
    }
    nop = nop->next;
  }
}

static VoidPtr LinkNewObjectLists (NewObjectPtr list1, NewObjectPtr list2)

{
  NewObjectPtr  nop;

  if (list1 == NULL) return list2;
  nop = list1;
  while (nop->next != NULL) {
    nop = nop->next;
  }
  nop->next = list2;
  return list1;
}

static void CleanupAnalysisExtraProc (GraphiC g, VoidPtr data)

{
  NewObjectPtr  nop;
  ValNodePtr    ppt;
  UrlParamPtr   upp;

  nop = (NewObjectPtr) data;
  if (nop != NULL) {
    MemFree (nop->host_machine);
    MemFree (nop->host_path);
    for (ppt = nop->paramlist; ppt != NULL; ppt = ppt->next) {
      upp = (UrlParamPtr) ppt->data.ptrvalue;
      if (upp == NULL) continue;
      MemFree (upp->param);
      MemFree (upp->prompt);
      MemFree (upp->dfault);
      MemFree (upp->choices);
      MemFree (upp->group);
      MemFree (upp->help);
    }
    ValNodeFreeData (nop->paramlist);
    MemFree (nop->prefix);
    MemFree (nop->suffix);
  }
  MemFree (data);
}

typedef struct sbstruc {
  CharPtr    name;
  MenU       menu;
} Sbstruc, PNTR SbstrucPtr;

static ValNodePtr  analysissubmenulist = NULL;

static void AddAnalysisItem (MenU m, BaseFormPtr bfp,
                             Boolean bspviewOK, Boolean docsumOK,
                             Boolean nucOK, Boolean protOK, Boolean onlyBspTarget,
                             CharPtr host_machine, Uint2 host_port,
                             CharPtr host_path, CharPtr program,
                             Uint2 timeoutsec, Int2 format, Boolean demomode,
                             QueryResultProc resultproc, ValNodePtr paramlist,
                             CharPtr prefix, CharPtr suffix,
                             CharPtr title, CharPtr submenu,
                             ItmActnProc actn, NewObjectPtr PNTR head)

{
  IteM          i;
  NewObjectPtr  last;
  size_t        len;
  NewObjectPtr  nop;
  SbstrucPtr    sbp;
  CharPtr       tmp;
  ValNodePtr    vnp;
  MenU          x;

  if (m == NULL || actn == NULL) return;
  x = NULL;
  if (! StringHasNoText (submenu)) {
    vnp = analysissubmenulist;
    while (vnp != NULL && x == NULL) {
      sbp = (SbstrucPtr) vnp->data.ptrvalue;
      if (sbp != NULL && StringICmp (sbp->name, submenu) == 0) {
        x = sbp->menu;
      }
      vnp = vnp->next;
    }
    if (x == NULL) {
      sbp = (SbstrucPtr) MemNew (sizeof (Sbstruc));
      if (sbp != NULL) {
        sbp->name = StringSave (submenu);
        sbp->menu = SubMenu (m, sbp->name);
        x = sbp->menu;
        ValNodeAddPointer (&analysissubmenulist, 0, (VoidPtr) sbp);
      }
    }
  }
  if (x == NULL) {
    x = m;
  }
  i = CommandItem (x, title, actn);
  nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
  if (nop != NULL) {
    nop->kind = 2; /* analysis menu item */
    nop->bfp = bfp;
    nop->item = i;
    nop->bspOK = bspviewOK;
    nop->dsmOK = docsumOK;
    nop->fastaNucOK = nucOK;
    nop->fastaProtOK = protOK;
    nop->onlyBspTarget = onlyBspTarget;
    nop->host_machine = /* StrSaveNoNullEncodeSpaces */ StringSave (host_machine);
    nop->host_port = host_port;
    len = StringLen (host_path);
    tmp = MemNew (len + StringLen (program) + 5);
    if (tmp != NULL) {
      StringCpy (tmp, host_path);
      if (len > 1 && tmp [len - 1] != '/') {
        StringCat (tmp, "/");
      }
      StringCat (tmp, program);
    }
    nop->host_path = /* StrSaveNoNullEncodeSpaces */ StringSave (tmp);
    MemFree (tmp);
    nop->query = NULL;
    /*
    nop->host_path = StrSaveNoNullEncodeSpaces (host_path);
    nop->query = StrSaveNoNullEncodeSpaces (program);
    */
    nop->timeoutsec = timeoutsec;
    nop->format = format;
    nop->demomode = demomode;
    nop->resultproc = resultproc;
    nop->paramlist = paramlist;
    nop->prefix = StringSaveNoNull (prefix);
    nop->suffix = StringSaveNoNull (suffix);
  }
  SetObjectExtra (i, (Pointer) nop, CleanupAnalysisExtraProc);
  if (head == NULL) return;
  last = *head;
  if (last != NULL) {
    while (last->next != NULL) {
     last = last->next;
    }
    last->next = nop;
  } else {
    *head = nop;
  }
}

/* Sample seqncgis.cnf/seqncgis.ini/.seqncgisrc/sequincgi.cfg config file.
   PATH can contain query (separated by ? symbol), or separate QUERY item can
   be used, or multiple QUERY and TITLE items can also be used.

[SERVICES]
PATH=mydisk:Common Files:services:

[ORDER]
ORDER_1=tRNAscan
ORDER_2=Seg

[tRNAscan]
PROGRAM=testcgi.cgi?request=trnascan
HOST=www.myserver.myschool.edu
PORT=80
PATH=/MyServices/cgi-bin/testcgi.cgi
SUBMENU=Search
FORMATIN=FASTA
FLAGS=SEQ,NUC,TRG
TIMEOUT=30

[Seg]
PROGRAM=segify
HOST=www.myserver.myschool.edu
PORT=80
PATH=/MyServices/cgi-bin/testcgi.cgi
FORMATIN=fasta
FLAGS=SEQ,DOC,PRT,TRG
SUBMENU=Secondary structure prediction
PROMPT_1=Window Size
PARAM_1=window
DESCRIPTION_1=window size for determining low-complexity segments
TYPE_1=text
DEFAULT_1=12
REQUIRED_1=FALSE
IMPORTANCE_1=
GROUP_1=Algorithm
HELP_1=window size for determining low-complexity segments
PROMPT_2=Trigger Complexity
PARAM_2=trigger
DESCRIPTION_2=trigger complexity for determining low-complexity segments
TYPE_2=text
DEFAULT_2=2.2
REQUIRED_2=FALSE
IMPORTANCE_2=
GROUP_2=Algorithm
HELP_2=trigger complexity for determining low-complexity segments
...

[ENZYMES]
ENZ_1=BamHI
ENZ_2=EcoRI
ENZ_3=HindIII

*/

static Int2 GetServiceParam (ValNodePtr head, CharPtr type, CharPtr buf, Int2 buflen)

{
  size_t      len;
  Boolean     seenBracket = FALSE;
  CharPtr     str;
  ValNodePtr  vnp;

  if (buf == NULL || buflen <= 0) return 0;
  *buf = '\0';
  len = StringLen (type);
  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (str != NULL) {
      if (str [0] == '[') {
        if (seenBracket) return 0;
        seenBracket = TRUE;
      } else if (StringNICmp (str, type, len) == 0) {
        str += len;
        StringNCpy_0 (buf, str, buflen);
        return (Int2) StringLen (buf);
      }
    }
  }
  return 0;
}

static ValNodePtr GetConfigParamAndPromptLists (CharPtr sect)

{
  Int2         i;
  ValNodePtr   paramlist = NULL;
  Uint1        paramtype;
  Char         title [512];
  Char         tmp [32];
  UrlParamPtr  upp;

  if (sect == NULL) return NULL;
  i = 1;
  sprintf (tmp, "PARAM_%d", (int) i);
  while (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
    upp = (UrlParamPtr) MemNew (sizeof (UrlParamData));
    if (upp == NULL) continue;
    upp->param = StringSave (title);
    sprintf (tmp, "TYPE_%d", (int) i);
    paramtype = 1;
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      if (StringICmp (title, "text") == 0) {
        paramtype = 1;
      } else if (StringICmp (title, "checkbox") == 0) {
        paramtype = 2;
      } else if (StringICmp (title, "popup") == 0) {
        paramtype = 3;
      } else if (StringICmp (title, "radio") == 0) {
        paramtype = 4;
      } else if (StringICmp (title, "list") == 0) {
        paramtype = 5;
      }
    }
    upp->type = paramtype;
    sprintf (tmp, "PROMPT_%d", (int) i);
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      upp->prompt = StringSave (title);
    } else {
      upp->prompt = StringSave (upp->param);
    }
    sprintf (tmp, "DEFAULT_%d", (int) i);
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      upp->dfault = StringSave (title);
    } else {
      upp->dfault = StringSave (" ");
    }
    sprintf (tmp, "CHOICES_%d", (int) i);
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      upp->choices = StringSave (title);
    } else {
      upp->choices = StringSave (" ");
    }
    sprintf (tmp, "GROUP_%d", (int) i);
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      upp->group = StringSave (title);
    } else {
      upp->group = StringSave (" ");
    }
    sprintf (tmp, "HELP_%d", (int) i);
    if (GetAppParam ("SEQNCGIS", sect, tmp, NULL, title, sizeof (title) - 1)) {
      upp->help = StringSave (title);
    } else {
      upp->help = StringSave (" ");
    }
    ValNodeAddPointer (&paramlist, i, (Pointer) upp);
    i++;
    sprintf (tmp, "PARAM_%d", (int) i);
  }
  return paramlist;
}

static ValNodePtr GetServiceParamAndPromptLists (ValNodePtr list)

{
  Int2         i;
  ValNodePtr   paramlist = NULL;
  Uint1        paramtype;
  Char         title [512];
  Char         tmp [32];
  UrlParamPtr  upp;

  if (list == NULL) return NULL;
  i = 1;
  sprintf (tmp, "PARAM_%d=", (int) i);
  while (GetServiceParam (list, tmp, title, sizeof (title) - 1)) {
    upp = (UrlParamPtr) MemNew (sizeof (UrlParamData));
    if (upp == NULL) continue;
    upp->param = StringSave (title);
    sprintf (tmp, "TYPE_%d", (int) i);
    paramtype = 1;
    if (GetServiceParam (list, tmp, title, sizeof (title) - 1)) {
      if (StringICmp (title, "text") == 0) {
        paramtype = 1;
      } else if (StringICmp (title, "checkbox") == 0) {
        paramtype = 2;
      } else if (StringICmp (title, "popup") == 0) {
        paramtype = 3;
      } else if (StringICmp (title, "radio") == 0) {
        paramtype = 4;
      } else if (StringICmp (title, "list") == 0) {
        paramtype = 5;
      }
    }
    upp->type = paramtype;
    sprintf (tmp, "PROMPT_%d=", (int) i);
    if (GetServiceParam (list, tmp, title, sizeof (title) - 1)) {
      upp->prompt = StringSave (title);
    } else {
      upp->prompt = StringSave (upp->param);
    }
    sprintf (tmp, "DEFAULT_%d=", (int) i);
    if (GetServiceParam (list, tmp, title, sizeof (title) - 1)) {
      upp->dfault = StringSave (title);
    } else {
      upp->dfault = StringSave (" ");
    }
    sprintf (tmp, "CHOICES_%d=", (int) i);
    if (GetServiceParam (list, tmp, title, sizeof (title) - 1)) {
      upp->choices = StringSave (title);
    } else {
      upp->choices = StringSave (" ");
    }
    sprintf (tmp, "GROUP_%d=", (int) i);
    if (GetServiceParam (list, tmp, title, sizeof (title) - 1)) {
      upp->group = StringSave (title);
    } else {
      upp->group = StringSave (" ");
    }
    sprintf (tmp, "HELP_%d=", (int) i);
    if (GetServiceParam (list, tmp, title, sizeof (title) - 1)) {
      upp->help = StringSave (title);
    } else {
      upp->help = StringSave (" ");
    }
    ValNodeAddPointer (&paramlist, i, (Pointer) upp);
    i++;
    sprintf (tmp, "PARAM_%d=", (int) i);
  }
  return paramlist;
}

static void ReadAnalysisConfigFile (CharPtr sect, MenU m, BaseFormPtr bfp,
                                    Boolean bspviewOK, Boolean docsumOK,
                                    NewObjectPtr PNTR head)

{
  Boolean     demomode = FALSE;
  Int2        format = 1;
  Char        host [128];
  Boolean     nucOK = FALSE;
  Boolean     onlyBspTarget = FALSE;
  ValNodePtr  paramlist = NULL;
  Char        program [128];
  Char        path [256];
  Uint2       port = 80;
  Char        prefix [128];
  Boolean     protOK = FALSE;
  Char        submenu [128];
  Char        suffix [128];
  Uint2       timeoutsec = 30;
  Char        title [128];
  Char        tmp [32];
  unsigned    int  val;

  if (! GetAppParam ("SEQNCGIS", sect, "TITLE", NULL, title, sizeof (title) - 1)) {
    StringNCpy_0 (title, sect, sizeof (title));
  }
  if (GetAppParam ("SEQNCGIS", sect, "HOST", NULL, host, sizeof (host) - 1)) {
    if (GetAppParam ("SEQNCGIS", sect, "FLAGS", NULL, tmp, sizeof (tmp) - 1)) {
      if (StringStr (tmp, "SEQ") == NULL) {
        bspviewOK = FALSE;
      }
      if (StringStr (tmp, "DOC") == NULL) {
        docsumOK = FALSE;
      }
      if (StringStr (tmp, "NUC") != NULL) {
        nucOK = TRUE;
      }
      if (StringStr (tmp, "PRT") != NULL) {
        protOK = TRUE;
      }
      if (StringStr (tmp, "TRG") != NULL) {
        onlyBspTarget = TRUE;
      }
    }

    if ((! bspviewOK) && (! docsumOK)) return;

    if (GetAppParam ("SEQNCGIS", sect, "PORT", NULL, tmp, sizeof (tmp) - 1) && 
        sscanf (tmp, "%u", &val) == 1) {
      port = (Uint2) val;
    } else {
      port = 80;
    }
    if (GetAppParam ("SEQNCGIS", sect, "FORMATIN", NULL, tmp, sizeof (tmp) - 1)) {
      if (StringICmp (tmp, "FASTA") == 0) {
        format = 1;
      } else if (StringICmp (tmp, "ASN.1") == 0) {
        format = 2;
      }
    }
    if (GetAppParam ("SEQNCGIS", sect, "TIMEOUT", NULL, tmp, sizeof (tmp) - 1) && 
        sscanf (tmp, "%u", &val) == 1) {
      timeoutsec = (Uint2) val;
    } else {
      timeoutsec = 30;
    }
    submenu [0] = '\0';
    GetAppParam ("SEQNCGIS", sect, "SUBMENU", NULL, submenu, sizeof (submenu) - 1);
    if (GetAppParam ("SEQNCGIS", sect, "DEMO", NULL, tmp, sizeof (tmp) - 1)) {
      if (StringICmp (tmp, "TRUE") == 0) {
        demomode = TRUE;
      }
    }

    if (GetAppParam ("SEQNCGIS", sect, "PATH", NULL, path, sizeof (path) - 1)) {
      if (GetAppParam ("SEQNCGIS", sect, "PROGRAM", NULL, program, sizeof (program) - 1)) {
        paramlist = GetConfigParamAndPromptLists (sect);
        prefix [0] = '\0';
        GetAppParam ("SEQNCGIS", sect, "PREFIX", NULL, prefix, sizeof (prefix) - 1);
        suffix [0] = '\0';
        GetAppParam ("SEQNCGIS", sect, "SUFFIX", NULL, suffix, sizeof (suffix) - 1);
        AddAnalysisItem (m, bfp, bspviewOK, docsumOK,
                         nucOK, protOK, onlyBspTarget,
                         host, port, path, program, timeoutsec, format, demomode,
                         SequinHandleURLResults, paramlist, prefix, suffix,
                         title, submenu, DoURLProc, head);
      }
    }
  }
}

static void ReadServiceConfigFile (CharPtr pathbase, ValNodePtr config,
                                   MenU m, BaseFormPtr bfp,
                                   Boolean bspviewOK, Boolean docsumOK,
                                   NewObjectPtr PNTR head)

{
  Char          ch;
  ValNodePtr    choicelist = NULL;
  Boolean       demomode = FALSE;
  ValNodePtr    dfaultlist = NULL;
  Int2          format = 1;
  FILE          *fp;
  Boolean       goOn = TRUE;
  ValNodePtr    helplist = NULL;
  Char          host [128];
  Boolean       keepGoing;
  ValNodePtr    list = NULL;
  Boolean       nucOK = FALSE;
  Boolean       onlyBspTarget = FALSE;
  ValNodePtr    paramlist = NULL;
  Char          program [128];
  ValNodePtr    promptlist = NULL;
  Char          path [PATH_MAX];
  Uint2         port = 80;
  Char          prefix [128];
  Boolean       protOK = FALSE;
  CharPtr       ptr;
  Boolean       seenBracket;
  Char          str [256];
  Char          submenu [128];
  Char          suffix [128];
  Uint2         timeoutsec = 30;
  Char          title [128];
  Char          tmp [32];
  unsigned int  val;
  ValNodePtr    vnp;

  if (path == NULL || config == NULL || config->data.ptrvalue == NULL) return;
  StringNCpy_0 (path, pathbase, sizeof (path));
  FileBuildPath (path, NULL, (CharPtr) config->data.ptrvalue);
  fp = FileOpen (path, "r");
  if (fp == NULL) return;
  while (fgets (str, sizeof (str), fp) != NULL) {
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && ch != '\n' && ch != '\r') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
    ValNodeCopyStr (&list, 1, str);
  }
  FileClose (fp);
  while (goOn) {
    goOn = FALSE;
    title [0] = '\0';
    if (GetServiceParam (list, "TITLE=", tmp, sizeof (tmp) - 1)) {
      StringNCpy_0 (title, tmp, sizeof (title));
    }
    if (StringHasNoText (title)) {
      if (GetServiceParam (list, "[", title, sizeof (title) - 1)) {
        ptr = StringChr (title, ']');
        if (ptr != NULL) {
          *ptr = '\0';
        }
      }
    }
    if (title [0] != '\0' && GetServiceParam (list, "HOST=", host, sizeof (host) - 1)) {
      if (GetServiceParam (list, "FLAGS=", tmp, sizeof (tmp) - 1)) {
        if (StringStr (tmp, "SEQ") == NULL) {
          bspviewOK= FALSE;
        }
        if (StringStr (tmp, "DOC") == NULL) {
          docsumOK= FALSE;
        }
        if (StringStr (tmp, "NUC") != NULL) {
          nucOK= TRUE;
        }
        if (StringStr (tmp, "PRT") != NULL) {
          protOK= TRUE;
        }
        if (StringStr (tmp, "TRG") != NULL) {
          onlyBspTarget= TRUE;
        }
      }

      if (bspviewOK || docsumOK) {

        if (GetServiceParam (list, "PORT=", tmp, sizeof (tmp) - 1) && 
            sscanf (tmp, "%u", &val) == 1) {
          port = (Uint2) val;
        } else {
          port = 80;
        }
        if (GetServiceParam (list, "FORMATIN=", tmp, sizeof (tmp) - 1)) {
          if (StringICmp (tmp, "FASTA") == 0) {
            format = 1;
          } else if (StringICmp (tmp, "ASN.1") == 0) {
            format = 2;
          }
        }
       if (GetServiceParam (list, "TIMEOUT=", tmp, sizeof (tmp) - 1) && 
            sscanf (tmp, "%u", &val) == 1) {
          timeoutsec = (Uint2) val;
        } else {
          timeoutsec = 30;
        }
        submenu [0] = '\0';
        GetServiceParam (list, "SUBMENU=", submenu, sizeof (submenu) - 1);
        if (GetServiceParam (list, "DEMO=", tmp, sizeof (tmp) - 1)) {
          if (StringICmp (tmp, "TRUE") == 0) {
            demomode = TRUE;
          }
        }

        if (GetServiceParam (list, "PATH=", path, sizeof (path) - 1)) {
          if (GetServiceParam (list, "PROGRAM=", program, sizeof (program) - 1)) {
            paramlist = GetServiceParamAndPromptLists (list);
            prefix [0] = '\0';
            GetServiceParam (list, "PREFIX=", prefix, sizeof (prefix) - 1);
            suffix [0] = '\0';
            GetServiceParam (list, "SUFFIX=", suffix, sizeof (suffix) - 1);
            AddAnalysisItem (m, bfp, bspviewOK, docsumOK,
                             nucOK, protOK, onlyBspTarget,
                             host, port, path, program, timeoutsec, format, demomode,
                             SequinHandleURLResults, paramlist, prefix, suffix,
                             title, submenu, DoURLProc, head);
          }
        }

      }
    }

    seenBracket = FALSE;
    keepGoing = TRUE;
    for (vnp = list; vnp != NULL && keepGoing; vnp = vnp->next) {
      ptr = (CharPtr) vnp->data.ptrvalue;
      if (ptr != NULL) {
        if (ptr [0] == '[') {
          if (seenBracket) {
            keepGoing = FALSE;
          } else {
            seenBracket = TRUE;
          }
        }
        if (keepGoing) {
          vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
        }
      }
    }

  }

  ValNodeFreeData (list);
}

extern MenU CreateAnalysisMenu (WindoW w, BaseFormPtr bfp, Boolean bspviewOK, Boolean docsumOK)

{
  NewObjectPtr  first;
  ValNodePtr    head1 = NULL, head2 = NULL;
  Int2          i;
  size_t        len;
  MenU          m;
  Char          path1 [PATH_MAX];
  Char          path2 [PATH_MAX];
  CharPtr       ptr;
  SbstrucPtr    sbp;
  Char          sect [256];
  Char          temp [32];
  ValNodePtr    vnp;

  ProgramPath (path1, sizeof (path1));
  ptr = StringRChr (path1, DIRDELIMCHR);
  if (ptr != NULL) {
    ptr++;
    *ptr = '\0';
  }
  FileBuildPath (path1, "services", NULL);
  head1 = DirCatalog (path1);

  if (GetAppParam ("SEQNCGIS", "SERVICES", "PATH", NULL, path2, sizeof (path2) - 1)) {
    len = StringLen (path2);
    if (path2 [len - 1] != DIRDELIMCHR) {
      StringCat (path2, DIRDELIMSTR);
    }
    if (StringCmp (path1, path2) != 0) {
      head2 = DirCatalog (path2);
    }
  }

  if ((! extraServices) && (! indexerVersion) && (! genomeCenter) &&
      head1 == NULL && head2 == NULL) {
    if (! GetAppParam ("SEQNCGIS", "ORDER", NULL, NULL, sect, sizeof (sect) - 1)) {
      return NULL;
    }
  }
  m = PulldownMenu (w, "Analysis");
  if (m == NULL) return NULL;
  analysissubmenulist = NULL;
  first = NULL;
  if (bspviewOK) {
    AddAnalysisItem (m, bfp, bspviewOK, FALSE, TRUE, FALSE, TRUE,
                     NULL, 0, NULL, NULL, 0, 0, FALSE, NULL, NULL, NULL, NULL,
                     "Restriction Search", "Search",
                     SimpleRsiteProc, &first);
    if (indexerVersion) {
      AddAnalysisItem (m, bfp, bspviewOK, FALSE, TRUE, FALSE, TRUE,
                       NULL, 0, NULL, NULL, 0, 0, FALSE, NULL, NULL, NULL, NULL,
                       "Vector Screen - UniVec", "Search",
                       SimpleUniVecScreenProc, &first);
      AddAnalysisItem (m, bfp, bspviewOK, FALSE, TRUE, FALSE, TRUE,
                       NULL, 0, NULL, NULL, 0, 0, FALSE, NULL, NULL, NULL, NULL,
                       "Vector Screen - UniVec Core", "Search",
                       SimpleUniVecCoreScreenProc, &first);
      AddAnalysisItem (m, bfp, bspviewOK, FALSE, TRUE, FALSE, TRUE,
                       NULL, 0, NULL, NULL, 0, 0, FALSE, NULL, NULL, NULL, NULL,
                       "QBlast Test", "Search",
                       SimpleQBlastProc, &first);
    }
  }
  if (bspviewOK || docsumOK) {
    if (useEntrez) {
      i = 1;
      sprintf (temp, "ORDER_%d", (int) i);
      while (GetAppParam ("SEQNCGIS", "ORDER", temp, NULL, sect, sizeof (sect) - 1)) {
        ReadAnalysisConfigFile (sect, m, bfp, bspviewOK, docsumOK, &first);
        i++;
        sprintf (temp, "ORDER_%d", (int) i);
      }
      for (vnp = head1; vnp != NULL; vnp = vnp->next) {
        if (vnp->choice == 0) {
          ReadServiceConfigFile (path1, vnp, m, bfp, bspviewOK, docsumOK, &first);
        }
      }
      for (vnp = head2; vnp != NULL; vnp = vnp->next) {
        if (vnp->choice == 0) {
          ReadServiceConfigFile (path2, vnp, m, bfp, bspviewOK, docsumOK, &first);
        }
      }
    }
  }
  if (bspviewOK) {
  }
  if (docsumOK) {
  }
#ifdef WIN_MAC
  macUserDataPtr = LinkNewObjectLists (macUserDataPtr, first);
#else
  bfp->userDataPtr = LinkNewObjectLists (bfp->userDataPtr, first);
#endif
  for (vnp = analysissubmenulist; vnp != NULL; vnp = vnp->next) {
    sbp = (SbstrucPtr) vnp->data.ptrvalue;
    if (sbp != NULL) {
      sbp->name = MemFree (sbp->name);
    }
  }
  analysissubmenulist = ValNodeFreeData (analysissubmenulist);
  ValNodeFreeData (head1);
  ValNodeFreeData (head2);
  return m;
}

/* NEW UPDATE SEQUENCE SECTION */


#define SQN_LEFT    1
#define SQN_RIGHT   2
#define SQN_MIDDLE  3

static Uint4 sqn_binary_search_on_uint4_list(Uint4Ptr list, Uint4 pos, Uint4 listlen)
{
   Uint4  L;
   Uint4  mid;
   Uint4  R;

   if (list == NULL || listlen == 0)
      return 0;
   L = 0;
   R = listlen - 1;
   while (L < R)
   {
      mid = (L+R)/2;
      if (list[mid + 1] <= pos)
      {
         L = mid + 1;
      } else
      {
         R = mid;
      }
   }
   return R;
}

static Int4 MapRowCoordsSpecial(SeqAlignPtr sap, Uint4 pos, Int4 row, Int4 which_end)
{
   DenseSegPtr  dsp;
   Int4         idx;
   Int4         offset;
   SAIndexPtr   saip;
   Int4         start;

   if (sap == NULL || row < 0)
      return -1;
   if (sap->saip == NULL)
      return -1;
   saip = (SAIndexPtr)sap->saip;
   dsp = (DenseSegPtr)sap->segs;
   start = sqn_binary_search_on_uint4_list(saip->aligncoords, pos, dsp->numseg);
   offset = pos - saip->aligncoords[start];
   idx = (dsp->dim*start) + row - 1;
   if (dsp->starts[idx] == -1)
   {
      if (which_end == SQN_RIGHT)
      {
         /* round down */
         while (start >= 0) {
            idx = (dsp->dim*start) + row - 1;
            if (dsp->starts[idx] != -1)
               return (dsp->starts[idx] + dsp->lens[start] - 1);
            start--;
         }
         return -2;
      } else if (which_end == SQN_LEFT)
      {
         /* round up */
         while (start < dsp->numseg) {
            idx = (dsp->dim*start) + row - 1;
            if (dsp->starts[idx] != -1)
               return (dsp->starts[idx]);
            start++;
         }
         return -2;
      }
   } else
   {
      idx = (dsp->dim*start) + row - 1;
      if (dsp->strands[idx] != Seq_strand_minus)
         return (dsp->starts[idx] + offset);
      else
         return (dsp->starts[idx] + dsp->lens[start] - 1 - offset);
   }
   return -1;
}

static Int4 MapBioseqToBioseqSpecial(SeqAlignPtr sap, Int4 begin, Int4 fin, Int4 pos, Int4 which_end)
{
   Int4  bspos;
   Int4  sapos;
   Int4  start1;
   Int4  start2;
   Int4  stop1;
   Int4  stop2;

   if (sap == NULL || sap->saip == NULL)
      return -2;
   AlnMgrGetNthSeqRangeInSA(sap, begin, &start1, &stop1);
   AlnMgrGetNthSeqRangeInSA(sap, fin, &start2, &stop2);
   /* check to see whether the position is outside the alignment */
   if (pos < start1)
      return (start2 - (start1 - pos));
   else if (pos > stop1)
      return (stop2 + (pos-stop1));
   sapos = AlnMgrMapBioseqToSeqAlign(sap, pos, begin, NULL);
   bspos = MapRowCoordsSpecial(sap, sapos, fin, which_end);
   if (bspos >= 0)
      return bspos;
   else if (which_end == SQN_LEFT)
      return (start2-1);
   else if (which_end == SQN_RIGHT)
      return (stop2+1);
   else
      return 0;
}

typedef struct upsdata {
  FORM_MESSAGE_BLOCK
  BioseqPtr           oldbsp;
  BioseqPtr           newbsp;
  SeqAlignPtr         salp;
  Boolean             revcomp;
  Boolean             diffOrgs;
  Int4                aln_length;
  Int4                log10_aln_length;
  Int4                old5, olda, old3;
  Int4                new5, newa, new3;
  Int4                recomb1, recomb2;
  Int4                startmax, stopmax;
  Uint1               strandold, strandnew;
  CharPtr             seq1, seq2, aln1, aln2;
  VieweR              overview;
  SegmenT             ovpict;
  VieweR              details;
  SegmenT             dtpict;
  PaneL               letters;
  Int2                charwidth;
  Int2                lineheight;
  Int2                maxchars;
  Int4                scaleX;
  ValNodePtr          indels;
  ValNodePtr          mismatches;
  GrouP               rmc;
  GrouP               sfb;
  GrouP               nobm;
  ButtoN              keepProteinIDs;
  ButtoN              accept;
} UpsData, PNTR UpsDataPtr;

static Boolean AdjustAlignment (
  UpsDataPtr udp,
  Int2 choice
)

{
  DenseSegPtr  dsp;
  Int2         j;
  SeqAlignPtr  sap;

  if (udp == NULL) return FALSE;

  sap = udp->salp;
  if (sap == NULL) return FALSE;
  AMFreeAllIndexes (sap);

  if (sap->segtype == SAS_DENSEG) {
    dsp = (DenseSegPtr) sap->segs;

    switch (choice) {
      case 2 :
        /* adjust alignment 5' */
        if (dsp != NULL && dsp->lens != NULL && dsp->numseg > 0) {
          dsp->lens [dsp->numseg - 1] += udp->old3;
        }
        break;
      case 3 :
        /* adjust alignment 3' */
        if (dsp != NULL && dsp->lens != NULL && dsp->starts != NULL && dsp->numseg > 0) {
          dsp->lens [0] += udp->old5;
          dsp->starts [0] = 0;
          dsp->starts [1] = 0;
          for (j = 1; j < dsp->numseg; j++) {
            if (dsp->starts [1 + j * 2] != -1) {
              dsp->starts [1 + j * 2] += udp->old5 - udp->new5;
            }
          }
        }
        break;
      case 4 :
        /* adjust alignment patch */
        if (dsp != NULL && dsp->lens != NULL && dsp->starts != NULL && dsp->numseg > 0) {
          dsp->lens [dsp->numseg - 1] += udp->old3;
          dsp->lens [0] += udp->old5;
          dsp->starts [0] = 0;
          dsp->starts [1] = 0;
          for (j = 1; j < dsp->numseg; j++) {
            if (dsp->starts [1 + j * 2] != -1) {
              dsp->starts [1 + j * 2] += udp->old5 - udp->new5;
            }
          }
        }
        break;
      default :
        break;
    }
  }

  AlnMgrIndexSingleChildSeqAlign (sap);

  return TRUE;
}

/*
static Int4 MapBioseqToBioseq (SeqAlignPtr salp, Int4 pos, Int4 which_end)

{
  Int4  alnpt, newpt;

  alnpt = AlnMgrMapBioseqToSeqAlign (salp, pos, 1, NULL);
  if (alnpt < 0) return pos;
  newpt = AlnMgrMapRowCoords (salp, alnpt, 2, NULL);
  if (newpt < 0) return pos;
  return newpt;
}
*/

static void OffsetLoc (SeqLocPtr slp, Int4 offset, SeqIdPtr sip)

{
  PackSeqPntPtr  psp;
  SeqIntPtr      sinp;
  SeqPntPtr      spp;
  Uint1          used;

  if (slp == NULL) return;
  switch (slp->choice) {
    case SEQLOC_INT :
      sinp = (SeqIntPtr) slp->data.ptrvalue;
      if (sinp != NULL) {
        sinp->from += offset;
        sinp->to += offset;
        if (sip != NULL) {
          sinp->id = SeqIdFree (sinp->id);
          sinp->id = SeqIdDup (sip);
        }
      }
      break;
    case SEQLOC_PNT :
      spp = (SeqPntPtr) slp->data.ptrvalue;
      if (spp != NULL) {
        spp->point += offset;
        if (sip != NULL) {
          spp->id = SeqIdFree (spp->id);
          spp->id = SeqIdDup (sip);
        }
      }
      break;
    case SEQLOC_PACKED_PNT :
      psp = (PackSeqPntPtr) slp->data.ptrvalue;
      if (psp != NULL) {
        for (used = 0; used < psp->used; used++) {
          psp->pnts [used] += offset;
        }
        if (sip != NULL) {
          psp->id = SeqIdFree (psp->id);
          psp->id = SeqIdDup (sip);
        }
      }
      break;
    default :
      break;
  }
}

static void OffsetLocation (SeqLocPtr loc, Int4 offset, SeqIdPtr sip)

{
  SeqLocPtr  slp;

  slp = SeqLocFindNext (loc, NULL);
  while (slp != NULL) {
    OffsetLoc (slp, offset, sip);
    slp = SeqLocFindNext (loc, slp);
  }
}

static void PromoteSeqId (SeqIdPtr sip, Pointer userdata)

{
  SeqIdPtr  bestid, newid, oldid;

  bestid = (SeqIdPtr) userdata;

  newid = SeqIdDup (bestid);
  if (newid == NULL) return;

  oldid = ValNodeNew (NULL);
  if (oldid == NULL) return;

  MemCopy (oldid, sip, sizeof (ValNode));
  oldid->next = NULL;

  sip->choice = newid->choice;
  sip->data.ptrvalue = newid->data.ptrvalue;

  SeqIdFree (oldid);
  ValNodeFree (newid);

  SeqIdStripLocus (sip);
}

static void CorrectFeatureSeqIds (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  VisitSeqIdsInSeqLoc (sfp->location, userdata, PromoteSeqId);
}

static Boolean DoFeaturePropWithOffset (
  UpsDataPtr udp,
  Int4 offset,
  SeqAnnotPtr PNTR sapp
)

{
  BioseqPtr          bsp, newbsp, oldbsp;
  CodeBreakPtr       cbp;
  SeqMgrFeatContext  context;
  CdRegionPtr        crp;
  SeqFeatPtr         dup, sfp, last = NULL;
  Uint2              entityID;
  Boolean            keepProteinIDs;
  SeqEntryPtr        newsep, prdsep, top;
  RnaRefPtr          rrp;
  SeqAnnotPtr        sap = NULL, saptmp;
  SeqDescrPtr        sdp;
  SeqIdPtr           sip;
  tRNAPtr            trp;

  if (udp == NULL) return FALSE;

  SeqEntrySetScope (NULL);

  sfp = SeqMgrGetNextFeature (udp->newbsp, NULL, 0, 0, &context);
  if (sfp == NULL) return FALSE;

  if (udp->diffOrgs) {
    keepProteinIDs = FALSE;
  } else {
    keepProteinIDs = GetStatus (udp->keepProteinIDs);
  }

  oldbsp = udp->oldbsp;

  entityID = ObjMgrGetEntityIDForPointer (udp->oldbsp);
  top = GetBestTopParentForData (entityID, udp->oldbsp);

  sdp = ExtractBioSourceAndPubs (top);

  sip = SeqIdFindBest (oldbsp->id, 0);

  while (sfp != NULL) {

    dup = AsnIoMemCopy ((Pointer) sfp,
                        (AsnReadFunc) SeqFeatAsnRead,
                        (AsnWriteFunc) SeqFeatAsnWrite);

    if (last == NULL) {
      sap = SeqAnnotNew ();
      if (oldbsp->annot == NULL) {
        oldbsp->annot = sap;
      } else {
        for (saptmp = oldbsp->annot; saptmp->next != NULL; saptmp = saptmp->next) continue;
        saptmp->next = sap;
      }
      sap->type = 1;
      sap->data = (Pointer) dup;
    } else {
      last->next = dup;
    }
    last = dup;

    /*
    sep = SeqMgrGetSeqEntryForData (oldbsp);
    CreateNewFeature (sep, NULL, dup->data.choice, dup);
    */

    OffsetLocation (dup->location, offset, sip);
    switch (dup->data.choice) {
      case SEQFEAT_CDREGION :
        crp = (CdRegionPtr) dup->data.value.ptrvalue;
        if (crp != NULL) {
          for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
            OffsetLocation (cbp->loc, offset, sip);
          }
        }
        break;
      case SEQFEAT_RNA :
        rrp = (RnaRefPtr) dup->data.value.ptrvalue;
        if (rrp != NULL && rrp->ext.choice == 2) {
          trp = (tRNAPtr) rrp->ext.value.ptrvalue;
          if (trp != NULL && trp->anticodon != NULL) {
            OffsetLocation (trp->anticodon, offset, sip);
          }
        }
        break;
      default :
        break;
    }
    if (dup->product != NULL) {
      SeqEntrySetScope (NULL);
      bsp = BioseqFindFromSeqLoc (dup->product);
      if (bsp != NULL) {
        prdsep = SeqMgrGetSeqEntryForData (bsp);
        if (prdsep != NULL) {
          newsep = AsnIoMemCopy ((Pointer) prdsep,
                                 (AsnReadFunc) SeqEntryAsnRead,
                                 (AsnWriteFunc) SeqEntryAsnWrite);
          if (newsep != NULL) {
            if (IS_Bioseq (newsep)) {
              newbsp = (BioseqPtr) newsep->data.ptrvalue;
              if (newbsp != NULL) {
                if (! keepProteinIDs) {
                  newbsp->id = SeqIdSetFree (newbsp->id);
                  newbsp->id = MakeNewProteinSeqId (NULL, sip);
                  newbsp->hist = SeqHistFree (newbsp->hist);
                  VisitFeaturesOnBsp (newbsp, (Pointer) newbsp->id, CorrectFeatureSeqIds);
                  SetSeqFeatProduct (dup, newbsp);
                  /*
                  dup->product = SeqLocFree (dup->product);
                  dup->product = CreateWholeInterval (newsep);
                  */
                }
                SeqMgrReplaceInBioseqIndex (newbsp);
              }
            }
            AddSeqEntryToSeqEntry (top, newsep, TRUE);
          }
        }
      }
    }

    sfp = SeqMgrGetNextFeature (udp->newbsp, sfp, 0, 0, &context);
  }

  ReplaceBioSourceAndPubs (top, sdp);

  if (sapp != NULL) {
    *sapp = sap;
  }

  return TRUE;
}

static void ReplaceLocation (SeqAlignPtr salp, SeqLocPtr slp, Int4 length, Int4 begin, Int4 fin)

{
  PackSeqPntPtr  psp;
  Int4           pt;
  SeqIntPtr      sinp;
  SeqPntPtr      spp;
  Uint1          used;

  if (slp == NULL) return;
  switch (slp->choice) {
    case SEQLOC_INT :
      sinp = (SeqIntPtr) slp->data.ptrvalue;
      if (sinp != NULL) {
        pt = MapBioseqToBioseqSpecial (salp, begin, fin, sinp->from, SQN_LEFT);
        if (pt < 0) {
          pt = 0;
        } else if (pt >= length) {
          pt = length - 1;
        }
        sinp->from = pt;
        pt = MapBioseqToBioseqSpecial (salp, begin, fin, sinp->to, SQN_RIGHT);
        if (pt < 0) {
          pt = 0;
        } else if (pt >= length) {
          pt = length - 1;
        }
        sinp->to = pt;
      }
      break;
    case SEQLOC_PNT :
      spp = (SeqPntPtr) slp->data.ptrvalue;
      if (spp != NULL) {
        pt = MapBioseqToBioseqSpecial (salp, begin, fin, spp->point, SQN_LEFT);
        if (pt < 0) {
          pt = 0;
        } else if (pt >= length) {
          pt = length - 1;
        }
        spp->point = pt;
      }
      break;
    case SEQLOC_PACKED_PNT :
      psp = (PackSeqPntPtr) slp->data.ptrvalue;
      if (psp != NULL) {
        for (used = 0; used < psp->used; used++) {
          pt = MapBioseqToBioseqSpecial (salp, begin, fin, psp->pnts [used], SQN_LEFT);
          if (pt < 0) {
            pt = 0;
          } else if (pt >= length) {
            pt = length - 1;
          }
          psp->pnts [used] = pt;
        }
      }
      break;
    default :
      break;
  }
}

static Boolean DoFeaturePropThruAlign (
  UpsDataPtr udp,
  SeqAnnotPtr PNTR sapp
)

{
  BioseqPtr          bsp, newbsp, oldbsp;
  CodeBreakPtr       cbp, prevcbp, nextcbp;
  SeqMgrFeatContext  context;
  CdRegionPtr        crp;
  SeqFeatPtr         dup, sfp, last = NULL;
  Uint2              entityID;
  Int4               from, to;
  Boolean            keepProteinIDs;
  SeqLocPtr          newloc, slp;
  SeqEntryPtr        newsep, prdsep, top;
  RnaRefPtr          rrp;
  SeqAnnotPtr        sap = NULL, saptmp;
  SeqDescrPtr        sdp;
  SeqIdPtr           sip;
  Boolean            split;
  tRNAPtr            trp;

  if (udp == NULL) return FALSE;

  SeqEntrySetScope (NULL);

  sfp = SeqMgrGetNextFeature (udp->newbsp, NULL, 0, 0, &context);
  if (sfp == NULL) return FALSE;

  keepProteinIDs = GetStatus (udp->keepProteinIDs);

  oldbsp = udp->oldbsp;

  entityID = ObjMgrGetEntityIDForPointer (oldbsp);
  top = GetBestTopParentForData (entityID, oldbsp);

  sdp = ExtractBioSourceAndPubs (top);

  sip = SeqIdFindBest (oldbsp->id, 0);

  from = udp->new5;
  to = udp->new5 + udp->newa;

  while (sfp != NULL) {

    if (context.right >= from && context.left <= to) {
      split = FALSE;
      newloc = SeqLocCopyRegion (oldbsp->id, sfp->location, udp->newbsp, from, to, Seq_strand_plus, &split);
      if (newloc != NULL) {
        OffsetLocation (newloc, from, NULL);
        dup = AsnIoMemCopy ((Pointer) sfp,
                            (AsnReadFunc) SeqFeatAsnRead,
                            (AsnWriteFunc) SeqFeatAsnWrite);
        SeqLocFree (dup->location);
        dup->location = newloc;
        if (split) {
          dup->partial = TRUE;
        }

        if (last == NULL) {
          sap = SeqAnnotNew ();
          if (oldbsp->annot == NULL) {
            oldbsp->annot = sap;
          } else {
            for (saptmp = oldbsp->annot; saptmp->next != NULL; saptmp = saptmp->next) continue;
            saptmp->next = sap;
          }
          sap->type = 1;
          sap->data = (Pointer) dup;
        } else {
          last->next = dup;
        }
        last = dup;

        /*
        sep = SeqMgrGetSeqEntryForData (oldbsp);
        CreateNewFeature (sep, NULL, dup->data.choice, dup);
        */

        slp = SeqLocFindNext (dup->location, NULL);
        while (slp != NULL) {
          ReplaceLocation (udp->salp, slp, oldbsp->length, 2, 1);
          slp = SeqLocFindNext (dup->location, slp);
        }
        switch (dup->data.choice) {
          case SEQFEAT_CDREGION :
            crp = (CdRegionPtr) dup->data.value.ptrvalue;
            if (crp != NULL) {
              prevcbp = NULL;
              for (cbp = crp->code_break; cbp != NULL; cbp = nextcbp) {
                nextcbp = cbp->next;
                newloc = SeqLocCopyRegion (oldbsp->id, cbp->loc, udp->newbsp,  from, to, Seq_strand_plus, &split);
                OffsetLocation (newloc, from, NULL);
                SeqLocFree (cbp->loc);
                cbp->loc = newloc;
                if (cbp->loc == NULL) {
                  if (prevcbp != NULL) {
                    prevcbp->next = nextcbp;
                  } else {
                    crp->code_break = nextcbp;
                  }
                  cbp->next = NULL;
                  CodeBreakFree (cbp);
                } else {
                  prevcbp = cbp;
                  slp = SeqLocFindNext (cbp->loc, NULL);
                  while (slp != NULL) {
                    ReplaceLocation (udp->salp, slp, oldbsp->length, 2, 1);
                    slp = SeqLocFindNext (cbp->loc, slp);
                  }
                }
              }
            }
            break;
          case SEQFEAT_RNA :
            rrp = (RnaRefPtr) dup->data.value.ptrvalue;
            if (rrp != NULL && rrp->ext.choice == 2) {
              trp = (tRNAPtr) rrp->ext.value.ptrvalue;
              if (trp != NULL && trp->anticodon != NULL) {
                newloc = SeqLocCopyRegion (oldbsp->id, trp->anticodon, udp->newbsp,  from, to, Seq_strand_plus, &split);
                OffsetLocation (newloc, from, NULL);
                SeqLocFree (trp->anticodon);
                trp->anticodon = newloc;
                slp = SeqLocFindNext (trp->anticodon, NULL);
                while (slp != NULL) {
                  ReplaceLocation (udp->salp, slp, oldbsp->length, 2, 1);
                  slp = SeqLocFindNext (trp->anticodon, slp);
                }
              }
            }
            break;
          default :
            break;
        }
        if (dup->product != NULL) {
          SeqEntrySetScope (NULL);
          bsp = BioseqFindFromSeqLoc (dup->product);
          if (bsp != NULL) {
            prdsep = SeqMgrGetSeqEntryForData (bsp);
            if (prdsep != NULL) {
              newsep = AsnIoMemCopy ((Pointer) prdsep,
                                     (AsnReadFunc) SeqEntryAsnRead,
                                     (AsnWriteFunc) SeqEntryAsnWrite);
              if (newsep != NULL) {
                if (IS_Bioseq (newsep)) {
                  newbsp = (BioseqPtr) newsep->data.ptrvalue;
                  if (newbsp != NULL) {
                    if (! keepProteinIDs) {
                      newbsp->id = SeqIdSetFree (newbsp->id);
                      newbsp->id = MakeNewProteinSeqId (NULL, sip);
                      VisitFeaturesOnBsp (newbsp, (Pointer) newbsp->id, CorrectFeatureSeqIds);
                      SetSeqFeatProduct (dup, newbsp);
                      /*
                      dup->product = SeqLocFree (dup->product);
                      dup->product = CreateWholeInterval (newsep);
                      */
                    }
                    SeqMgrReplaceInBioseqIndex (newbsp);
                  }
                }
                AddSeqEntryToSeqEntry (top, newsep, TRUE);
              }
            }
          }
        }
      }
    }

    sfp = SeqMgrGetNextFeature (udp->newbsp, sfp, 0, 0, &context);
  }

  ReplaceBioSourceAndPubs (top, sdp);

  if (sapp != NULL) {
    *sapp = sap;
  }

  return TRUE;
}

static Boolean ReplaceSequence (UpsDataPtr udp)

{
  MsgAnswer          ans;
  ByteStorePtr       bs;
  CodeBreakPtr       cbp;
  SeqMgrFeatContext  context;
  CdRegionPtr        crp;
  Int4               len;
  BioseqPtr          newbsp;
  BioseqPtr          oldbsp;
  RnaRefPtr          rrp;
  Uint1              seq_data_type;
  SeqFeatPtr         sfp;
  SeqLocPtr          slp;
  tRNAPtr            trp;

  if (StringICmp (udp->seq1, udp->seq2) == 0) {
    ans = Message (MSG_OKC, "Replacement sequence is identical to original - possible error");
    if (ans == ANS_CANCEL) return FALSE;
  }

  oldbsp = udp->oldbsp;
  newbsp = udp->newbsp;

  sfp = SeqMgrGetNextFeature (oldbsp, NULL, 0, 0, &context);
  while (sfp != NULL) {
    slp = SeqLocFindNext (sfp->location, NULL);
    while (slp != NULL) {
      ReplaceLocation (udp->salp, slp, newbsp->length, 1, 2);
      slp = SeqLocFindNext (sfp->location, slp);
    }
    switch (sfp->data.choice) {
      case SEQFEAT_CDREGION :
        crp = (CdRegionPtr) sfp->data.value.ptrvalue;
        if (crp != NULL) {
          for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
            slp = SeqLocFindNext (cbp->loc, NULL);
            while (slp != NULL) {
              ReplaceLocation (udp->salp, slp, newbsp->length, 1, 2);
              slp = SeqLocFindNext (cbp->loc, slp);
            }
          }
        }
        break;
      case SEQFEAT_RNA :
        rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
        if (rrp != NULL && rrp->ext.choice == 2) {
          trp = (tRNAPtr) rrp->ext.value.ptrvalue;
          if (trp != NULL && trp->anticodon != NULL) {
            slp = SeqLocFindNext (trp->anticodon, NULL);
            while (slp != NULL) {
              ReplaceLocation (udp->salp, slp, newbsp->length, 1, 2);
              slp = SeqLocFindNext (trp->anticodon, slp);
            }
          }
        }
        break;
      default :
        break;
    }
    sfp = SeqMgrGetNextFeature (oldbsp, sfp, 0, 0, &context);
  }

  bs = oldbsp->seq_data;
  oldbsp->seq_data = newbsp->seq_data;
  newbsp->seq_data = bs;
  len = oldbsp->length;
  oldbsp->length = newbsp->length;
  newbsp->length = len;
  seq_data_type = oldbsp->seq_data_type;
  oldbsp->seq_data_type = newbsp->seq_data_type;
  newbsp->seq_data_type = seq_data_type;

  return TRUE;
}

static Boolean Merge5Prime (UpsDataPtr udp)

{
  ByteStorePtr  bs;
  Char          ch;
  Int4          i, newlen;
  BioseqPtr     newbsp;
  CharPtr       ptr, str, tmp;

  newlen = udp->new5 + udp->newa + udp->old3;
  str = (CharPtr) MemNew (sizeof (Char) * (size_t) (newlen + 5));
  if (str == NULL) return FALSE;

  /* construct replacement sequence by recombining between old and overlap */
  ptr = str;

  tmp = udp->seq2;
  for (i = 0; i < udp->new5 + udp->newa; i++) {
    ch = *tmp;
    *ptr = ch;
    tmp++;
    ptr++;
  }

  tmp = udp->seq1 + udp->old5 + udp->olda;
  for (i = 0; i < udp->old3; i++) {
    ch = *tmp;
    *ptr = ch;
    tmp++;
    ptr++;
  }

  *ptr = '\0';
  bs = BSNew (newlen);
  BSWrite (bs, (VoidPtr) str, newlen);

  udp->seq2 = MemFree (udp->seq2);
  udp->seq2 = str;

  if (bs != NULL && BSLen (bs) < 1) {
    bs = BSFree (bs);
  }
  if (bs == NULL) return FALSE;

  /* overlap turned into replacement sequence */

  newbsp = udp->newbsp;
  newbsp->seq_data = BSFree (newbsp->seq_data);
  newbsp->seq_data = bs;
  newbsp->seq_data_type = Seq_code_iupacna;
  newbsp->length = newlen;

  /* adjust alignment and reindex */

  if (! AdjustAlignment (udp, 2)) return FALSE;

  /* then finish by replacing with new sequence */

  return ReplaceSequence (udp);
}

static Boolean Merge3Prime (UpsDataPtr udp)

{
  ByteStorePtr  bs;
  Char          ch;
  Int4          i, newlen;
  BioseqPtr     newbsp;
  CharPtr       ptr, str, tmp;

  newlen = udp->old5 + udp->newa + udp->new3;
  str = (CharPtr) MemNew (sizeof (Char) * (size_t) (newlen + 5));
  if (str == NULL) return FALSE;

  /* construct replacement sequence by recombining between old and overlap */
  ptr = str;

  tmp = udp->seq1;
  for (i = 0; i < udp->old5; i++) {
    ch = *tmp;
    *ptr = ch;
    tmp++;
    ptr++;
  }

  tmp = udp->seq2 + udp->new5;
  for (i = 0; i < udp->newa + udp->new3; i++) {
    ch = *tmp;
    *ptr = ch;
    tmp++;
    ptr++;
  }

  *ptr = '\0';
  bs = BSNew (newlen);
  BSWrite (bs, (VoidPtr) str, newlen);

  udp->seq2 = MemFree (udp->seq2);
  udp->seq2 = str;

  if (bs != NULL && BSLen (bs) < 1) {
    bs = BSFree (bs);
  }
  if (bs == NULL) return FALSE;

  /* overlap turned into replacement sequence */

  newbsp = udp->newbsp;
  newbsp->seq_data = BSFree (newbsp->seq_data);
  newbsp->seq_data = bs;
  newbsp->seq_data_type = Seq_code_iupacna;
  newbsp->length = newlen;

  /* adjust alignment and reindex */

  if (! AdjustAlignment (udp, 3)) return FALSE;

  /* then finish by replacing with new sequence */

  return ReplaceSequence (udp);
}

static Boolean PatchSequence (UpsDataPtr udp)

{
  ByteStorePtr  bs;
  Char          ch;
  Int4          i, newlen;
  BioseqPtr     newbsp;
  CharPtr       ptr, str, tmp;

  newlen = udp->old5 + udp->newa + udp->old3;
  str = (CharPtr) MemNew (sizeof (Char) * (size_t) (newlen + 5));
  if (str == NULL) return FALSE;

  /* construct replacement sequence by double recombination */
  ptr = str;

  tmp = udp->seq1;
  for (i = 0; i < udp->old5; i++) {
    ch = *tmp;
    *ptr = ch;
    tmp++;
    ptr++;
  }

  tmp = udp->seq2 + udp->new5;
  for (i = 0; i < udp->newa; i++) {
    ch = *tmp;
    *ptr = ch;
    tmp++;
    ptr++;
  }

  tmp = udp->seq1 + udp->old5 + udp->olda;
  for (i = 0; i < udp->old3; i++) {
    ch = *tmp;
    *ptr = ch;
    tmp++;
    ptr++;
  }

  *ptr = '\0';
  bs = BSNew (newlen);
  BSWrite (bs, (VoidPtr) str, newlen);

  udp->seq2 = MemFree (udp->seq2);
  udp->seq2 = str;

  if (bs != NULL && BSLen (bs) < 1) {
    bs = BSFree (bs);
  }
  if (bs == NULL) return FALSE;

  /* overlap turned into replacement sequence */

  newbsp = udp->newbsp;
  newbsp->seq_data = BSFree (newbsp->seq_data);
  newbsp->seq_data = bs;
  newbsp->seq_data_type = Seq_code_iupacna;
  newbsp->length = newlen;

  /* adjust alignment and reindex */

  if (! AdjustAlignment (udp, 4)) return FALSE;

  /* then finish by replacing with new sequence */

  return ReplaceSequence (udp);
}

static void MarkProductForDeletion (
  SeqLocPtr product
)

{
  BioseqPtr  bsp;
  SeqIdPtr   sip;

  if (product == NULL) return;
  sip = SeqLocId (product);
  if (sip == NULL) return;
  bsp = BioseqFind (sip);
  if (bsp == NULL) return;
  bsp->idx.deleteme = TRUE;
}

static void CombineTexts (
  CharPtr PNTR txtptr,
  CharPtr PNTR oldtxtptr
)

{
  size_t     len;
  CharPtr    str;

  if (txtptr == NULL || oldtxtptr == NULL) return;

  if (*txtptr == NULL) {

    *txtptr = *oldtxtptr;
    *oldtxtptr = NULL;

  } else if (*oldtxtptr != NULL && StringICmp (*txtptr, *oldtxtptr) != 0) {

    len = StringLen (*txtptr) + StringLen (*oldtxtptr) + 5;
    str = MemNew (sizeof (Char) * len);
    StringCpy (str, *txtptr);
    StringCat (str, "; ");
    StringCat (str, *oldtxtptr);
    *txtptr = MemFree (*txtptr);
    *txtptr = str;
  }
}

static void FuseCommonFeatureFields (
  SeqFeatPtr sfp,
  SeqFeatPtr oldsfp
)

{
  GBQualPtr       lastgbq;
  SeqFeatXrefPtr  lastxref;

  if (sfp == NULL || oldsfp == NULL) return;

  CombineTexts (&(sfp->comment), &(oldsfp->comment));
  CombineTexts (&(sfp->title), &(oldsfp->title));
  CombineTexts (&(sfp->except_text), &(oldsfp->except_text));

  if (sfp->qual == NULL) {
    sfp->qual = oldsfp->qual;
    oldsfp->qual = NULL;
  } else if (oldsfp->qual != NULL) {
    for (lastgbq = sfp->qual; lastgbq->next != NULL; lastgbq = lastgbq->next) continue;
    lastgbq->next = oldsfp->qual;
    oldsfp->qual = NULL;
  }

  ValNodeLink (&(sfp->dbxref), oldsfp->dbxref);
  oldsfp->dbxref = NULL;

  ValNodeLink (&(sfp->cit), oldsfp->cit);
  oldsfp->cit = NULL;

  if (sfp->xref == NULL) {
    sfp->xref = oldsfp->xref;
    oldsfp->xref = NULL;
  } else if (oldsfp->xref != NULL) {
    for (lastxref = sfp->xref; lastxref->next != NULL; lastxref = lastxref->next) continue;
    lastxref->next = oldsfp->xref;
    oldsfp->xref = NULL;
  }

  if (sfp->ext == NULL) {
    sfp->ext = oldsfp->ext;
    oldsfp->ext = NULL;
  } else if (oldsfp->ext != NULL) {
    sfp->ext = CombineUserObjects (sfp->ext, oldsfp->ext);
    oldsfp->ext = NULL;
  }

  sfp->partial |= oldsfp->partial;
  sfp->excpt |= oldsfp->excpt;
  sfp->pseudo |= oldsfp->pseudo;
}

static void FuseFeatures (
  SeqFeatPtr sfp,
  SeqFeatPtr oldsfp
)

{
  GeneRefPtr      grp, oldgrp;
  BioseqPtr       prod, oldprod;
  SeqFeatPtr      prot, oldprot;
  ProtRefPtr      prp, oldprp;
  RnaRefPtr       rrp, oldrrp;
  SeqIdPtr        sip;

  if (sfp == NULL || oldsfp == NULL) return;

  /* merge common fields */

  FuseCommonFeatureFields (sfp, oldsfp);

  /* now deal with type-specific data */

  switch (sfp->data.choice) {
    case SEQFEAT_GENE :
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      oldgrp = (GeneRefPtr) oldsfp->data.value.ptrvalue;
      if (grp == NULL || oldgrp == NULL) return;
      CombineTexts (&(grp->locus), &(oldgrp->locus));
      CombineTexts (&(grp->allele), &(oldgrp->allele));
      CombineTexts (&(grp->desc), &(oldgrp->desc));
      CombineTexts (&(grp->maploc), &(oldgrp->maploc));
      grp->pseudo |= oldgrp->pseudo;
      ValNodeLink (&(grp->db), oldgrp->db);
      oldgrp->db = NULL;
      ValNodeLink (&(grp->syn), oldgrp->syn);
      oldgrp->syn = NULL;
      break;
    case SEQFEAT_CDREGION :
      sip = SeqLocId (sfp->product);
      prod = BioseqFind (sip);
      sip = SeqLocId (oldsfp->product);
      oldprod = BioseqFind (sip);
      if (prod == NULL || oldprod == NULL) return;
      prot = SeqMgrGetBestProteinFeature (prod, NULL);
      oldprot = SeqMgrGetBestProteinFeature (oldprod, NULL);
      if (prot == NULL || oldprot == NULL) return;
      FuseCommonFeatureFields (prot, oldprot);
      prp = (ProtRefPtr) prot->data.value.ptrvalue;
      oldprp = (ProtRefPtr) oldprot->data.value.ptrvalue;
      if (prp == NULL || oldprp == NULL) return;
      ValNodeLink (&(prp->name), oldprp->name);
      oldprp->name = NULL;
      ValNodeLink (&(prp->ec), oldprp->ec);
      oldprp->ec = NULL;
      ValNodeLink (&(prp->activity), oldprp->activity);
      oldprp->activity = NULL;
      ValNodeLink (&(prp->db), oldprp->db);
      oldprp->db = NULL;
      CombineTexts (&(prp->desc), &(oldprp->desc));
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      oldrrp = (RnaRefPtr) oldsfp->data.value.ptrvalue;
      if (rrp == NULL || oldrrp == NULL) return;
      if (rrp->ext.choice == 1 && oldrrp->ext.choice == 1) {
        CombineTexts ((CharPtr PNTR) &(rrp->ext.value.ptrvalue), (CharPtr PNTR) &(oldrrp->ext.value.ptrvalue));
      }
      break;
    case SEQFEAT_REGION :
    case SEQFEAT_COMMENT :
      if (sfp->data.value.ptrvalue == NULL || oldsfp->data.value.ptrvalue == NULL) return;
      CombineTexts ((CharPtr PNTR) &(sfp->data.value.ptrvalue), (CharPtr PNTR) &(oldsfp->data.value.ptrvalue));
      break;
    default :
      break;
  }
}

static void ResolveDuplicateFeats (
  UpsDataPtr udp,
  BioseqPtr bsp,
  SeqAnnotPtr sap
)

{
  SeqMgrFeatContext  context, lastcontext;
  Int2               i, j;
  Boolean            ivalssame;
  SeqFeatPtr         lastsfp = NULL, sfp;
  Int2               nobmval;

  if (udp == NULL || bsp == NULL || sap == NULL) return;

  nobmval = GetValue (udp->nobm);
  if (nobmval == 3) return; /* keep both */

  SeqMgrIndexFeatures (0, (Pointer) bsp);

  lastsfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &context);
  if (lastsfp == NULL) return;

  MemCopy ((Pointer) &lastcontext, (Pointer) &context, sizeof (SeqMgrFeatContext));

  sfp = SeqMgrGetNextFeature (bsp, lastsfp, 0, 0, &context);
  if (sfp == NULL) return;

  while (sfp != NULL) {

    if (context.left == lastcontext.left &&
        context.right == lastcontext.right &&
        context.featdeftype == lastcontext.featdeftype) {

      if (context.strand == lastcontext.strand ||
          lastcontext.strand == Seq_strand_unknown ||
          context.strand == Seq_strand_unknown) {

        ivalssame = TRUE;
        if (context.numivals != lastcontext.numivals ||
            context.ivals == NULL ||
            lastcontext.ivals == NULL) {

          ivalssame = FALSE;

        } else {

          for (i = 0, j = 0; i < lastcontext.numivals; i++, j += 2) {
            if (context.ivals [j] != lastcontext.ivals [j]) {
              ivalssame = FALSE;
            }
            if (context.ivals [j + 1] != lastcontext.ivals [j + 1]) {
              ivalssame = FALSE;
            }
          }
        }

        if (ivalssame &&
            context.sap != lastcontext.sap &&
            (context.sap == sap || lastcontext.sap == sap)) {

          if (nobmval == 1) { /* keep new */
            if (context.sap == sap) {
              lastsfp->idx.deleteme = TRUE;
              MarkProductForDeletion (lastsfp->product);
            } else if (lastcontext.sap == sap) {
              sfp->idx.deleteme = TRUE;
              MarkProductForDeletion (sfp->product);
            }

          } else if (nobmval == 2) { /* keep old */
            if (context.sap == sap) {
              sfp->idx.deleteme = TRUE;
              MarkProductForDeletion (sfp->product);
            } else if (lastcontext.sap == sap) {
              lastsfp->idx.deleteme = TRUE;
              MarkProductForDeletion (lastsfp->product);
            }

          } else if (nobmval == 4) { /* merge */
            if (context.sap == sap) {
              FuseFeatures (sfp, lastsfp);
              lastsfp->idx.deleteme = TRUE;
              MarkProductForDeletion (lastsfp->product);
            } else if (lastcontext.sap == sap) {
              FuseFeatures (lastsfp, sfp);
              sfp->idx.deleteme = TRUE;
              MarkProductForDeletion (sfp->product);
            }
          }
        }
      }
    }

    lastsfp = sfp;
    MemCopy ((Pointer) &lastcontext, (Pointer) &context, sizeof (SeqMgrFeatContext));

    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &context);
  }
}

static void AcceptRMC (ButtoN b)

{
  Uint2        entityID;
  Int2         rmcval, sfbval;
  SeqAnnotPtr  sap = NULL;
  SeqEntryPtr  sep;
  UpsDataPtr   udp;
  Boolean      update = FALSE;

  udp = (UpsDataPtr) GetObjectExtra (b);
  if (udp == NULL) return;
  SafeHide (udp->form);
  sfbval = GetValue (udp->sfb);
  rmcval = GetValue (udp->rmc);
  if (sfbval == 1 || sfbval == 3) {
    switch (rmcval) {
      case 1 :
        if (ReplaceSequence (udp)) {
          update = TRUE;
        }
        break;
      case 2 :
        if (Merge5Prime (udp)) {
          update = TRUE;
        }
        break;
      case 3 :
        if (Merge3Prime (udp)) {
          update = TRUE;
        }
        break;
      case 4 :
        if (PatchSequence (udp)) {
          update = TRUE;
        }
        break;
      default :
        break;
    }
    if ( sfbval == 3) {
      switch (rmcval) {
        case 1 :
          if (DoFeaturePropWithOffset (udp, 0, &sap)) {
            update = TRUE;
          }
          break;
        case 2 :
          if (DoFeaturePropWithOffset (udp, 0, &sap)) {
            update = TRUE;
          }
          break;
        case 3 :
          if (DoFeaturePropWithOffset (udp, udp->old5 - udp->new5, &sap)) {
            update = TRUE;
          }
          break;
        case 4 :
          if (DoFeaturePropWithOffset (udp, udp->old5 - udp->new5, &sap)) {
            update = TRUE;
          }
          break;
        default :
          break;
      }
    }
  } else if (sfbval == 2) {
    switch (rmcval) {
      case 1 :
        if (DoFeaturePropThruAlign (udp, &sap)) {
          update = TRUE;
        }
        break;
      case 2 :
        if (DoFeaturePropThruAlign (udp, &sap)) {
          update = TRUE;
        }
        break;
      case 3 :
        if (DoFeaturePropThruAlign (udp, &sap)) {
          update = TRUE;
        }
        break;
      case 4 :
        if (DoFeaturePropThruAlign (udp, &sap)) {
          update = TRUE;
        }
        break;
      default :
        break;
    }
  }
  if (sfbval == 2 || sfbval == 3) {
    if (update) {
      entityID = ObjMgrGetEntityIDForPointer (udp->oldbsp);
      sep = GetTopSeqEntryForEntityID (entityID);
      /* need to set scope to make sure we mark the right bioseq for deletion */
      SeqEntrySetScope (sep);
      ResolveDuplicateFeats (udp, udp->oldbsp, sap);
      SeqEntrySetScope (NULL);
      DeleteMarkedObjects (entityID, 0, NULL);
    }
  }
  if (update) {
    entityID = ObjMgrGetEntityIDForPointer (udp->oldbsp);
    ObjMgrSetDirtyFlag (entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
  }
  Remove (udp->form);
}

static void UpdateAccept (GrouP g)

{
  Int2        nobmval, rmcval, sfbval;
  UpsDataPtr  udp;

  udp = (UpsDataPtr) GetObjectExtra (g);
  if (udp == NULL) return;
  rmcval = GetValue (udp->rmc);
  if (rmcval == 0) {
    SafeDisable (udp->accept);
    return;
  }
  sfbval = GetValue (udp->sfb);
  if (sfbval == 0 || sfbval == 1 || udp->diffOrgs) {
    SafeDisable (udp->keepProteinIDs);
  } else if (sfbval == 2 || sfbval == 3) {
    SafeEnable (udp->keepProteinIDs);
  }
  if (sfbval == 0) {
    SafeDisable (udp->accept);
    return;
  }
  nobmval = GetValue (udp->nobm);
  if (sfbval == 2 || sfbval == 3) {
    if (nobmval == 0) {
      SafeDisable (udp->accept);
      return;
    }
  }
  SafeEnable (udp->accept);
}

static void DrawAlignBlock (
  SegmenT pict,
  Int4 top,
  Int4 bottom,
  Int4 labelpt,
  Int2 labelaln,
  Int4 len5,
  Int4 lena,
  Int4 len3,
  Int4 aln_length
)

{
  Char  str [96];

  if (len5 > 0) {
    AddRectangle (pict, -len5, top, 0, bottom, NO_ARROW, FALSE, 0);
  }
  sprintf (str, "%ld", (long) len5);
  AddLabel (pict, -len5, (top + bottom) / 2, str, SMALL_TEXT, 5, MIDDLE_LEFT, 0);

  if (len3 > 0) {
    AddRectangle (pict, aln_length, top, aln_length + len3, bottom, NO_ARROW, FALSE, 0);
  }
  sprintf (str, "%ld", (long) len3);
  AddLabel (pict, aln_length + len3, (top + bottom) / 2, str, SMALL_TEXT, 5, MIDDLE_RIGHT, 0);

  AddRectangle (pict, 0, top, aln_length, bottom, NO_ARROW, TRUE, 0);
  sprintf (str, "%ld", (long) lena);
  AddLabel (pict, aln_length / 2, labelpt, str, SMALL_TEXT, 5, labelaln, 0);
}

static SegmenT MakeAlignPicture (
  UpsDataPtr udp,
  CharPtr strid1,
  CharPtr strid2,
  SeqAlignPtr sap
)

{
  SegmenT  pict;
  Char     str [96];
  Int4     top, bottom;

  pict = CreatePicture ();
  if (sap == NULL) return pict;

  top = 0;
  bottom = top - 10;

  DrawAlignBlock (pict, top, bottom, bottom, LOWER_CENTER, udp->old5, udp->olda, udp->old3, udp->aln_length);

  /*
  AddLabel (pict, (udp->stopmax - udp->startmax) / 2, bottom - 20, strid1, SMALL_TEXT, 5, LOWER_CENTER, 0);
  */


  sprintf (str, "%ld", (long) udp->aln_length);
  AddLabel (pict, udp->aln_length / 2, 10, str, SMALL_TEXT, 5, MIDDLE_CENTER, 0);


  top = 30;
  bottom = top - 10;

  DrawAlignBlock (pict, top, bottom, top, UPPER_CENTER, udp->new5, udp->newa, udp->new3, udp->aln_length);

  /*
  AddLabel (pict, (udp->stopmax - udp->startmax) / 2, top + 20, strid2, SMALL_TEXT, 5, UPPER_CENTER, 0);
  */

  return pict;
}

/*
static void DrawAlignBits (
  SegmenT pict,
  Int4 top,
  Int4 bottom,
  Int4 row,
  SeqAlignPtr sap
)

{
  Int4       len, start, stop;
  AlnMsgPtr  amp;

  amp = AlnMsgNew ();
  if (amp == NULL) return;

  amp->from_m = 0;
  amp->to_m = -1;
  amp->row_num = row;

  start = 0;
  while (AlnMgrGetNextAlnBit (sap, amp)) {
    len = amp->to_b - amp->from_b + 1;
    stop = start + len;
    if (amp->gap != 0) {
      AddLine (pict, start, (top + bottom) / 2, stop, (top + bottom) / 2, FALSE, 0);
    } else {
      AddRectangle (pict, start, top, stop, bottom, NO_ARROW, FALSE, 0);
    }
    start += len;
  }

  AlnMsgFree (amp);
}
*/

static void DrawAlignDiffs (
  UpsDataPtr udp,
  SegmenT pict,
  Int4 top,
  Int4 bottom,
  SeqAlignPtr sap
)

{
  AlnMsgPtr  amp1, amp2;
  SegmenT    seg;
  Int4       start, stop, len, len1, len2, i, j, k;

  if (udp->seq1 == NULL || udp->seq2 == NULL) return;
  len1 = StringLen (udp->seq1);
  len2 = StringLen (udp->seq2);

  amp1 = AlnMsgNew ();
  amp2 = AlnMsgNew ();
  if (amp1 == NULL || amp2 == NULL) return;

  seg = CreateSegment (pict, 0, 0);
  AddAttribute (seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);

  amp1->from_m = 0;
  amp1->to_m = -1;
  amp1->row_num = 1;

  amp2->from_m = 0;
  amp2->to_m = -1;
  amp2->row_num = 2;

  start = 0;
  stop = 0;

  while (AlnMgrGetNextAlnBit (sap, amp1) && AlnMgrGetNextAlnBit (sap, amp2)) {
    len = amp1->to_b - amp1->from_b + 1;
    if (len == amp2->to_b - amp2->from_b + 1) {
      stop = start + len;
      if (amp1->gap == 0 && amp2->gap == 0) {
        for (i = start, j = amp1->from_b, k = amp2->from_b;
             i < stop && j <= len1 && k <= len2; i++, j++, k++) {
          if (udp->seq1 [j] != udp->seq2 [k]) {

            /* record for accurate scrolling to text view */
            ValNodeAddInt (&(udp->mismatches), 0, i);

            AddLine (seg, i, top, i, bottom, FALSE, 0);
          }
        }
      }
    }
    start += len;
  }

  AlnMsgFree (amp1);
  AlnMsgFree (amp2);
}

static void DrawAlignBits (
  UpsDataPtr udp,
  SegmenT pict,
  Int4 top,
  Int4 bottom,
  Int4 row,
  Int4 pos1,
  Int4 pos2,
  SeqAlignPtr sap
)

{
  AlnMsgPtr  amp;
  Int4       len, start, stop, from, to;
  Char       str [96];
  Boolean    wasgap;

  amp = AlnMsgNew ();
  if (amp == NULL) return;

  amp->from_m = 0;
  amp->to_m = -1;
  amp->row_num = row;

  start = 0;
  stop = 0;
  from = 0;
  to = 0;
  wasgap = FALSE;

  while (AlnMgrGetNextAlnBit (sap, amp)) {
    len = amp->to_b - amp->from_b + 1;
    stop = start + len;
    if (amp->gap != 0) {
      if (wasgap) {
        to = stop;
      } else {
        AddRectangle (pict, from, top, to, bottom, NO_ARROW, FALSE, 0);
        wasgap = TRUE;
        from = start;
        to = stop;
      }
    } else {
      if (wasgap) {

        /* record for accurate scrolling to text view */
        ValNodeAddInt (&(udp->indels), 0, from);

        AddLine (pict, from, (top + bottom) / 2, to, (top + bottom) / 2, FALSE, 0);
        wasgap = FALSE;
        from = start;
        to = stop;
      } else {
        to = stop;
      }
    }
    start += len;
  }

  if (to > from) {
    if (wasgap) {

      /* record for accurate scrolling to text view */
      ValNodeAddInt (&(udp->indels), 0, from);

      AddLine (pict, from, (top + bottom) / 2, to, (top + bottom) / 2, FALSE, 0);
    } else {
      AddRectangle (pict, from, top, to, bottom, NO_ARROW, FALSE, 0);
    }
  }

  AlnMsgFree (amp);

  sprintf (str, "%ld", (long) pos1);
  AddLabel (pict, 0, (top + bottom) / 2, str, SMALL_TEXT, 5, MIDDLE_LEFT, 0);

  sprintf (str, "%ld", (long) pos2);
  AddLabel (pict, to, (top + bottom) / 2, str, SMALL_TEXT, 5, MIDDLE_RIGHT, 0);
}

static SegmenT MakeAlignDetails (
  UpsDataPtr udp,
  CharPtr strid1,
  CharPtr strid2,
  SeqAlignPtr sap
)

{
  Int4     aln_length;
  SegmenT  pict;
  Int4     top, bottom;

  pict = CreatePicture ();
  if (sap == NULL) return pict;

  aln_length = udp->aln_length;

  top = 0;
  bottom = top - 10;

  DrawAlignBits (udp, pict, top, bottom, 1, udp->old5 + 1, udp->old5 + udp->olda, sap);

  /*
  AddLabel (pict, aln_length / 2, bottom, strid1, SMALL_TEXT, 5, LOWER_CENTER, 0);
  */

  top = 30;
  bottom = top - 10;

  if (udp->revcomp) {
    DrawAlignBits (udp, pict, top, bottom, 2, udp->new3 + udp->newa, udp->new3 + 1, sap);
  } else {
    DrawAlignBits (udp, pict, top, bottom, 2, udp->new5 + 1, udp->new5 + udp->newa, sap);
  }

  /*
  AddLabel (pict, aln_length / 2, top, strid2, SMALL_TEXT, 5, UPPER_CENTER, 0);
  */

  top = 15;
  bottom = top - 10;

  DrawAlignDiffs (udp, pict, top, bottom, sap);

  return pict;
}

static CharPtr MakeAlignSequence (
  UpsDataPtr udp,
  SeqAlignPtr sap,
  Int4 row,
  CharPtr seq
)

{
  CharPtr    aln;
  AlnMsgPtr  amp;
  Int4       aln_length, len, lens, start, stop, from, to, i, j;

  if (udp == NULL || sap == NULL || seq == NULL) return NULL;
  lens = StringLen (seq);

  aln = (CharPtr) MemNew (sizeof (Char) * (udp->aln_length + 2));
  if (aln == NULL) return NULL;
  aln_length = udp->aln_length;
  MemSet ((Pointer) aln, '-', aln_length);

  amp = AlnMsgNew ();
  if (amp == NULL) return aln;

  amp->from_m = 0;
  amp->to_m = -1;
  amp->row_num = row;

  start = 0;
  stop = 0;
  from = 0;
  to = 0;

  while (AlnMgrGetNextAlnBit (sap, amp)) {
    len = amp->to_b - amp->from_b + 1;
    stop = start + len;

    if (amp->gap == 0) {
      for (i = start, j = amp->from_b; i < stop && j < lens; i++, j++) {
        aln [i] = seq [j];
      }
    }
    start += len;
  }

  AlnMsgFree (amp);

  return aln;
}

static void PrintTAln (ButtoN b)

{
  AsnIoPtr    aip;
  Char        path [PATH_MAX];
  UpsDataPtr  udp;

  udp = (UpsDataPtr) GetObjectExtra (b);
  if (udp == NULL) return;
  TmpNam (path);
  aip = AsnIoOpen (path, "w");
  if (aip != NULL) {
    SeqAlignAsnWrite (udp->salp, aip, NULL);
    AsnIoClose (aip);
    LaunchGeneralTextViewer (path, "Update Sequence Alignment");
  }
  FileRemove (path);
}

static void PrintGAln (ButtoN b)

{
  UpsDataPtr  udp;

  udp = (UpsDataPtr) GetObjectExtra (b);
  if (udp == NULL) return;
  PrintViewer (udp->overview);
  PrintViewer (udp->details);
}

static void CalculateOverhangs (
  UpsDataPtr udp
)

{
  Int4         aln_length;
  Uint2        entityID;
  SeqAlignPtr  sap;
  SeqEntryPtr  sep;
  Int4         stopold, startold, lenold, stopnew, startnew, lennew;

  if (udp == NULL) return;
  sap = udp->salp;
  if (sap == NULL) return;
  aln_length = AlnMgrGetAlnLength (sap, FALSE);
  AlnMgrGetNthSeqRangeInSA (sap, 1, &startold, &stopold);
  AlnMgrGetNthSeqRangeInSA (sap, 2, &startnew, &stopnew);
  lenold = udp->oldbsp->length;
  lennew = udp->newbsp->length;

  udp->old5 = startold;
  udp->old3 = lenold - stopold - 1;
  udp->olda = stopold - startold + 1;

  udp->new5 = startnew;
  udp->new3 = lennew - stopnew - 1;
  udp->newa = stopnew - startnew + 1;

  udp->aln_length = aln_length;
  udp->startmax = MAX (startold, startnew);
  udp->stopmax = MAX (aln_length + lenold - stopold, aln_length + lennew - stopnew);

  udp->strandold = AlnMgrGetNthStrand (sap, 1);
  udp->strandnew = AlnMgrGetNthStrand (sap, 2);

  entityID = ObjMgrGetEntityIDForPointer (udp->oldbsp);
  sep = GetTopSeqEntryForEntityID (entityID);
  SeqEntrySetScope (sep);
  udp->seq1 = GetSequenceByBsp (udp->oldbsp);
  SeqEntrySetScope (NULL);

  entityID = ObjMgrGetEntityIDForPointer (udp->oldbsp);
  sep = GetTopSeqEntryForEntityID (entityID);
  SeqEntrySetScope (sep);
  udp->seq2 = GetSequenceByBsp (udp->newbsp);
  SeqEntrySetScope (NULL);

  udp->aln1 = MakeAlignSequence (udp, sap, 1, udp->seq1);
  udp->aln2 = MakeAlignSequence (udp, sap, 2, udp->seq2);

  udp->log10_aln_length = 1;
  while (aln_length >= 10) {
    aln_length /= 10;
    (udp->log10_aln_length)++;
  }
}

static Int4 CalculateBestScale (
  UpsDataPtr udp,
  VieweR vwr,
  SegmenT pict
)

{
  BoxInfo  box;
  Int2     i;
  Int4     max, worldwid, portwid;
  RecT     r;
  Int4     scaleX, oldscaleX;
  Int4     wid;

  ObjectRect (vwr, &r);
  InsetRect (&r, 4, 4);
  wid = (Int4) (r.right - r.left + 1);

  SegmentBox (pict, &box);
  oldscaleX = (box.right - box.left + wid - 1) / wid;
  RecalculateSegment (pict, oldscaleX, 1);
  SegmentBox (pict, &box);
  portwid = wid * oldscaleX;
  worldwid = box.right - box.left + 20 * oldscaleX + 1;
  max = MAX (worldwid, portwid);
  scaleX = (max + wid - 1) / wid;
  i = 0;
  while (i < 10 && (scaleX > oldscaleX || portwid < worldwid)) {
    oldscaleX = scaleX;
    RecalculateSegment (pict, oldscaleX, 1);
    SegmentBox (pict, &box);
    portwid = wid * oldscaleX;
    worldwid = box.right - box.left + 20 * oldscaleX + 1;
    max = MAX (worldwid, portwid);
    scaleX = (max + wid - 1) / wid;
    i++;
  }

  return scaleX;
}

/*
static Uint1 leftTriFillSym [] = {
  0x06, 0x1E, 0x7E, 0xFE, 0x7E, 0x1E, 0x06, 0x00
};
static Uint1 rightTriFillSym [] = {
  0xC0, 0xF0, 0xFC, 0xFE, 0xFC, 0xF0, 0xC0, 0x00
};
*/
static Uint1 leftTriFillSym [] = {
  0x0C, 0x3C, 0xFC, 0x3C, 0x0C, 0x00, 0x00, 0x00
};
static Uint1 rightTriFillSym [] = {
  0xC0, 0xF0, 0xFC, 0xF0, 0xC0, 0x00, 0x00, 0x00
};

static void LetDraw (
  PaneL pnl
)

{
  Char        ch1, ch2;
  Int2        i, k, q, left, top, bottom, arrowwidth;
  size_t      len;
  Int4        offset, j, pos, realpos;
  RecT        r, x;
  BaR         sb;
  Char        str [32];
  UpsDataPtr  udp;

  udp = (UpsDataPtr) GetObjectExtra (pnl);
  if (udp == NULL) return;

  ObjectRect (pnl, &r);
  InsetRect (&r, 4, 4);

  sb = GetSlateHScrollBar ((SlatE) pnl);
  offset = GetBarValue (sb);

  SelectFont (SetSmallFont ());

  /* draw top (new) letters */

  MoveTo (r.left, r.top + 8 + 3 * udp->lineheight);
  for (i = 0, j = offset; i < udp->maxchars && j < udp->aln_length; i++, j++) {
    PaintChar (udp->aln2 [j]);
  }

  /* draw bottom (old) letters */

  MoveTo (r.left, r.top + 8 + 5 * udp->lineheight);
  for (i = 0, j = offset; i < udp->maxchars && j < udp->aln_length; i++, j++) {
    PaintChar (udp->aln1 [j]);
  }

  /* draw recombination arrows */

  arrowwidth = MIN (6, udp->charwidth);
  if (udp->recomb1 >= offset && udp->recomb1 <= offset + udp->maxchars) {
    left = r.left + udp->charwidth * (udp->recomb1 - offset);
    LoadRect (&x, left, r.top, left + arrowwidth, r.top + 6);
    CopyBits (&x, leftTriFillSym);
  }

  if (udp->recomb2 >= offset && udp->recomb2 <= offset + udp->maxchars) {
    left = r.left + udp->charwidth * (udp->recomb2 - offset - 1);
    LoadRect (&x, left, r.top, left + arrowwidth, r.top + 6);
    CopyBits (&x, rightTriFillSym);
  }

  /* draw red mismatch lines */

  Red ();
  top = r.top + 8 + 4 * udp->lineheight - Ascent ();
  bottom = top + udp->lineheight - 2;

  for (i = 0, j = offset; i < udp->maxchars && j < udp->aln_length; i++, j++) {
    ch1 = udp->aln1 [j];
    ch2 = udp->aln2 [j];
    if (ch1 == ch2) {
    } else if (ch1 == '-' || ch2 == '-') {
    } else {
      left = r.left + i * udp->charwidth + udp->charwidth / 2 - 1;
      MoveTo (left, top);
      LineTo (left, bottom);
    }
  }
  Black ();

  /* draw top (new) tick marks and coordinates */

  bottom = r.top + 8 + 3 * udp->lineheight - Ascent () - 2;
  top = bottom - 5;
  i = 0;
  j = offset;
  pos = AlnMgrMapRowCoords (udp->salp, j, 2, NULL);
  while (pos < 1 && i < udp->maxchars && j < udp->aln_length) {
    i++;
    j++;
    pos = AlnMgrMapRowCoords (udp->salp, j, 2, NULL);
  }
  for (; i < udp->maxchars + udp->log10_aln_length && j < udp->aln_length; i++, j++) {
    ch1 = udp->aln2 [j];
    if (ch1 != '-') {
      if (udp->revcomp) {
        realpos = (udp->newbsp->length - pos - 1);
      } else {
        realpos = pos;
      }
      if (((realpos + 1) % 10) == 0) {
        left = r.left + i * udp->charwidth + udp->charwidth / 2 - 1;
        if (i < udp->maxchars) {
          MoveTo (left, top);
          LineTo (left, bottom);
        }
        sprintf (str, "%ld", (long) (realpos + 1));
        len = StringLen (str);
        if (len <= j + 1) {
          k = i - len + 1;
          q = 0;
          if (k < 0) {
            q -= k;
            k = 0;
          }
          if (q < len) {
            left = r.left + k * udp->charwidth;
            MoveTo (left, r.top + 8 + udp->lineheight);
            while (k < udp->maxchars && q < len) {
              PaintChar (str [q]);
              k++;
              q++;
            }
          }
        }
      } else if (((realpos + 1) % 5) == 0) {
        left = r.left + i * udp->charwidth + udp->charwidth / 2 - 1;
        if (i < udp->maxchars) {
          MoveTo (left, top + 3);
          LineTo (left, bottom);
        }
      }
      pos++;
    }
  }

  /* draw bottom (old) tick marks and coordinates */

  top = r.top + 8 + 6 * udp->lineheight - Ascent () + 2;
  bottom = top + 5;
  i = 0;
  j = offset;
  pos = AlnMgrMapRowCoords (udp->salp, j, 1, NULL);
  while (pos < 1 && i < udp->maxchars && j < udp->aln_length) {
    i++;
    j++;
    pos = AlnMgrMapRowCoords (udp->salp, j, 1, NULL);
  }
  for (; i < udp->maxchars + udp->log10_aln_length && j < udp->aln_length; i++, j++) {
    ch1 = udp->aln1 [j];
    if (ch1 != '-') {
      if (((pos + 1) % 10) == 0) {
        left = r.left + i * udp->charwidth + udp->charwidth / 2 - 1;
        if (i < udp->maxchars) {
          MoveTo (left, top);
          LineTo (left, bottom);
        }
        sprintf (str, "%ld", (long) (pos + 1));
        len = StringLen (str);
        if (len <= j + 1) {
          k = i - len + 1;
          q = 0;
          if (k < 0) {
            q -= k;
            k = 0;
          }
          if (q < len) {
            left = r.left + k * udp->charwidth;
            MoveTo (left, r.top + 8 + 7 * udp->lineheight);
            while (k < udp->maxchars && q < len) {
              PaintChar (str [q]);
              k++;
              q++;
            }
          }
        }
      } else if (((pos + 1) % 5) == 0) {
        left = r.left + i * udp->charwidth + udp->charwidth / 2 - 1;
        if (i < udp->maxchars) {
          MoveTo (left, top);
          LineTo (left, bottom - 3);
        }
      }
      pos++;
    }
  }

  SelectFont (systemFont);
}

static void LetScrl (
  BaR sb,
  SlatE slt,
  Int4 newval,
  Int4 oldval
)

{
  RecT        r;
  UpsDataPtr  udp;

  udp = (UpsDataPtr) GetObjectExtra (slt);
  if (udp == NULL) return;

  ObjectRect (udp->letters, &r);
  InsetRect (&r, 4, 4);
  Select (udp->letters);
  if (ABS (oldval - newval) < udp->maxchars) {
    ScrollRect (&r, (oldval - newval) * udp->charwidth, 0);
  } else {
    InsetRect (&r, -2, -2);
    InvalRect (&r);
  }
  Update ();
}

static void DtlClck (
  VieweR vwr,
  SegmenT pict,
  PoinT pt
)

{
  Int4        goHere;
  Int4        offset;
  Int4        maxover2;
  PntInfo     pnt;
  BaR         sb;
  UpsDataPtr  udp;
  ValNodePtr  vnp;

  udp = (UpsDataPtr) GetViewerData (vwr);
  if (udp == NULL) return;

  sb = GetSlateHScrollBar ((SlatE) udp->letters);

  MapViewerToWorld (vwr, pt, &pnt);
  maxover2 = udp->maxchars / 2;
  if (pnt.x <= 0) {
    pnt.x = 0;
  } else if (pnt.x >= udp->aln_length) {
    pnt.x = udp->aln_length  - udp->maxchars;
  } else if (pnt.x >= maxover2) {

    offset = GetBarValue (sb);

    /* look for clicks within 5 pixels of an indel start or a mismatch */

    goHere = -1;
    for (vnp = udp->indels; vnp != NULL && goHere < 0; vnp = vnp->next) {
      if (ABS (pnt.x - vnp->data.intvalue) < udp->scaleX * 5) {
        goHere = vnp->data.intvalue;
      }
    }
    for (vnp = udp->mismatches; vnp != NULL && goHere < 0; vnp = vnp->next) {
      if (ABS (pnt.x - vnp->data.intvalue) < udp->scaleX * 5) {
        goHere = vnp->data.intvalue;
      }
    }

    if (goHere >= 0) {
      pnt.x = goHere;
    } else {
      /* if already visible, no need to scroll */
      if (pnt.x - maxover2 > offset && pnt.x - maxover2 < offset + maxover2 - 5) return;
      if (pnt.x - maxover2 < offset && pnt.x - maxover2 > offset - maxover2 + 5) return;
    }

    /* go left 1/2 screen so desired point is in the middle */

    pnt.x -= maxover2;
  }

  ResetClip ();
  SetBarValue (sb, pnt.x);
  Update ();
}

static void FrameVwr (
  VieweR vwr,
  SegmenT pict
)

{
  RecT  r;

  ResetClip ();
  ObjectRect (vwr, &r);
  FrameRect (&r);
}

static int LIBCALLBACK SortVnpByInt (VoidPtr ptr1, VoidPtr ptr2)

{
  ValNodePtr  vnp1;
  ValNodePtr  vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;

  if (vnp1->data.intvalue > vnp2->data.intvalue) {
    return 1;
  } else if (vnp1->data.intvalue < vnp2->data.intvalue) {
    return -1;
  }

  return 0;
}

extern void QuitProc (void);

static void UpdateSequenceFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr        bfp;
  StdEditorProcsPtr  sepp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp == NULL) return;
  switch (mssg) {
    case VIB_MSG_CLOSE :
      Remove (f);
      break;
    case VIB_MSG_QUIT :
      QuitProc ();
      break;
    default :
      sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
      if (sepp != NULL && sepp->handleMessages != NULL) {
        sepp->handleMessages (f, mssg);
      }
      break;
  }
}

static void CleanupUpdateSequenceForm (GraphiC g, VoidPtr data)

{
  Uint2       entityID;
  UpsDataPtr  udp;

  udp = (UpsDataPtr) data;
  if (udp != NULL) {
    udp->ovpict = DeletePicture (udp->ovpict);
    udp->dtpict = DeletePicture (udp->dtpict);
    udp->salp = SeqAlignFree (udp->salp);
    entityID = ObjMgrGetEntityIDForPointer (udp->newbsp);
    udp->newbsp = ObjMgrFreeByEntityID (entityID);
    udp->seq1 = MemFree (udp->seq1);
    udp->seq2 = MemFree (udp->seq2);
    udp->aln1 = MemFree (udp->aln1);
    udp->aln2 = MemFree (udp->aln2);
    udp->indels = ValNodeFree (udp->indels);
    udp->mismatches = ValNodeFree (udp->mismatches);
  }
  StdCleanupFormProc (g, data);
}

#ifdef WIN_MAC
extern void UpdateSequenceFormActivated (WindoW w);

static void UpdateSequenceFormActivate (WindoW w)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    if (bfp->activate != NULL) {
      bfp->activate (w);
    }
  }
}
#endif

static CharPtr txt1 =
  "Sequence Relationship displays sequence lengths";

static CharPtr txt2 =
  "Alignment Details displays sequence positions";

static CharPtr txt3 =
  "Click above to scroll Alignment Text position";

static CharPtr txt4 =
  "Alignment Text displays sequence letters";

static ForM UpdateSequenceForm (
  BioseqPtr oldbsp,
  BioseqPtr newbsp,
  SeqAlignPtr salp,
  Boolean revcomp
)

{
  ButtoN             b2, b3, b4;
  BioSourcePtr       biop1, biop2;
  GrouP              c, g, y, z = NULL;
  SeqMgrDescContext  dcontext;
  Uint2              entityID, hgt;
  SeqMgrFeatContext  fcontext;
  GrouP              gp1, gp2, gp3, ppt0, ppt1, ppt2, ppt3, ppt4 = NULL;
  OrgRefPtr          orp1, orp2;
  RecT               r;
  BaR                sb;
  Int4               scaleX;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  Boolean            sidebyside = FALSE;
  SeqIdPtr           sip;
  Char               strid1 [41], strid2 [41], txt0 [256];
  UpsDataPtr         udp;
  WindoW             w;

  if (oldbsp == NULL || newbsp == NULL) return NULL;
  udp = (UpsDataPtr) MemNew (sizeof (UpsData));
  if (udp == NULL) return NULL;
  w = FixedWindow (-50, -33, -10, -10, "Update Sequence", NULL);
  if (w == NULL) return NULL;

#ifdef WIN_MSWIN
  if (screenRect.bottom - screenRect.top < 750) {
    sidebyside = TRUE;
  }
#endif

  sip = SeqIdFindWorst (oldbsp->id);
  SeqIdWrite (sip, strid1, PRINTID_REPORT, sizeof (strid1) - 1);
  sip = SeqIdFindWorst (newbsp->id);
  SeqIdWrite (sip, strid2, PRINTID_REPORT, sizeof (strid2) - 1);
  if (StringICmp (strid2, "SequinUpdateSequence") == 0 && newbsp->id->next != NULL) {
    sip = SeqIdFindWorst (newbsp->id->next);
    SeqIdWrite (sip, strid2, PRINTID_REPORT, sizeof (strid2) - 1);
  }

  SetObjectExtra (w, (Pointer) udp, CleanupUpdateSequenceForm);
  udp->form = (ForM) w;
  udp->formmessage = UpdateSequenceFormMessage;

#ifdef WIN_MAC
  udp->activate = UpdateSequenceFormActivated;
  SetActivate (w, UpdateSequenceFormActivate);
#endif

  udp->oldbsp = oldbsp;
  udp->newbsp = newbsp;
  udp->salp = salp;
  udp->revcomp = revcomp;
  udp->diffOrgs = FALSE;

  CalculateOverhangs (udp);

  sprintf (txt0, "New sequence: %s - Length: %ld\nOld Sequence: %s - Length: %ld",
           strid2, (long) newbsp->length, strid1, (long) oldbsp->length);
  ppt0 = MultiLinePrompt (w, txt0, 350, programFont);

  if (sidebyside) {
    y = HiddenGroup (w, 0, -5, NULL);
  } else {
    y = HiddenGroup (w, -1, 0, NULL);
  }

  ppt1 = MultiLinePrompt (y, txt1, 350, programFont);
  udp->overview = CreateViewer (y, 350 + Nlm_vScrollBarWidth, 100, FALSE, FALSE);

  ppt2 = MultiLinePrompt (y, txt2, 350, programFont);
  udp->details = CreateViewer (y, 350 + Nlm_vScrollBarWidth, 80, FALSE, FALSE);

  ppt3 = MultiLinePrompt (y, txt3, 350, programFont);

  if (sidebyside) {
    ppt4 = MultiLinePrompt (y, txt4, 350, programFont);
  }

#ifdef WIN_MAC
  hgt = 90;
#else
  hgt = 110;
#endif
  udp->letters = AutonomousPanel4 (y, 350 + Nlm_vScrollBarWidth, hgt, LetDraw,
                                   NULL, LetScrl, 0, NULL, NULL);
  SetObjectExtra (udp->letters, (Pointer) udp, NULL);

#ifdef WIN_MAC
  if (indexerVersion && shftKey) {
    ButtoN  b;
    z = HiddenGroup (y, 2, 0, NULL);
    SetGroupSpacing (z, 10, 3);
    b = PushButton (z, "Print Graphic", PrintGAln);
    SetObjectExtra (b, (Pointer) udp, NULL);
    b = PushButton (z, "Display Alignment", PrintTAln);
    SetObjectExtra (b, (Pointer) udp, NULL);
  }
#endif

  udp->ovpict = NULL;
  udp->dtpict = NULL;

  g = HiddenGroup (y, -1, 0, NULL);
  SetGroupSpacing (g, 5, 5);

  gp1 = NormalGroup (g, 4, 0, "Alignment Relationship", programFont, NULL);
  udp->rmc = HiddenGroup (gp1, 4, 0, UpdateAccept);
  SetObjectExtra (udp->rmc, (Pointer) udp, NULL);
  SetGroupSpacing (udp->rmc, 10, 5);
  RadioButton (udp->rmc, "Replace");
  b2 = RadioButton (udp->rmc, "Extend 5'");
  b3 = RadioButton (udp->rmc, "Extend 3'");
  b4 = RadioButton (udp->rmc, "Patch");

  gp2 = NormalGroup (g, 4, 0, "Update Operation", programFont, NULL);
  udp->sfb = HiddenGroup (gp2, 4, 0, UpdateAccept);
  SetObjectExtra (udp->sfb, (Pointer) udp, NULL);
  SetGroupSpacing (udp->sfb, 10, 5);
  RadioButton (udp->sfb, "Sequence");
  RadioButton (udp->sfb, "Features");
  RadioButton (udp->sfb, "Sequence + Features");

  udp->keepProteinIDs = CheckBox (g, "Keep Protein IDs", NULL);

  gp3 = NormalGroup (g, 4, 0, "Duplicate Feature Policy", programFont, NULL);
  udp->nobm = HiddenGroup (gp3, 4, 0, UpdateAccept);
  SetObjectExtra (udp->nobm, (Pointer) udp, NULL);
  SetGroupSpacing (udp->nobm, 10, 5);
  RadioButton (udp->nobm, "New");
  RadioButton (udp->nobm, "Old");
  RadioButton (udp->nobm, "Both");
  RadioButton (udp->nobm, "Merge");

  AlignObjects (ALIGN_CENTER, (HANDLE) gp1, (HANDLE) gp2, (HANDLE) gp3,
                (HANDLE) udp->keepProteinIDs, NULL);


  c = HiddenGroup (w, 4, 0, NULL);
  udp->accept = PushButton (c, "Accept", AcceptRMC);
  SetObjectExtra (udp->accept, (Pointer) udp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  if (sidebyside) {
    AlignObjects (ALIGN_CENTER, (HANDLE) udp->overview, (HANDLE) udp->details,
                  (HANDLE) ppt1, (HANDLE) ppt2, (HANDLE) ppt3, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) udp->letters, (HANDLE) ppt4,
                  (HANDLE) g, (HANDLE) z, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) ppt0, (HANDLE) y, (HANDLE) c, NULL);
  } else {
    AlignObjects (ALIGN_CENTER, (HANDLE) udp->overview, (HANDLE) udp->details,
                  (HANDLE) udp->letters, (HANDLE) ppt0, (HANDLE) ppt1, (HANDLE) ppt2,
                  (HANDLE) ppt3, (HANDLE) g, (HANDLE) c, (HANDLE) z, NULL);
  }
  RealizeWindow (w);

  udp->ovpict = MakeAlignPicture (udp, strid1, strid2, salp);
  scaleX = CalculateBestScale (udp, udp->overview, udp->ovpict);
  AttachPicture (udp->overview, udp->ovpict, 0, 0, UPPER_LEFT, scaleX, 1, FrameVwr);

  udp->dtpict = MakeAlignDetails (udp, strid1, strid2, salp);
  scaleX = CalculateBestScale (udp, udp->details, udp->dtpict);
  udp->scaleX = scaleX;
  AttachPicture (udp->details, udp->dtpict, 0, 0, UPPER_LEFT, scaleX, 1, FrameVwr);
  SetViewerData (udp->details, (Pointer) udp, NULL);
  SetViewerProcs (udp->details, DtlClck, NULL, NULL, NULL);

  udp->indels = ValNodeSort (udp->indels, SortVnpByInt);
  udp->mismatches = ValNodeSort (udp->mismatches, SortVnpByInt);

  SelectFont (SetSmallFont ());
  ObjectRect (udp->letters, &r);
  InsetRect (&r, 4, 4);
  udp->lineheight = LineHeight ();
  udp->charwidth = MaxCharWidth ();
  udp->maxchars = (r.right - r.left - 2 + udp->charwidth - 1) / udp->charwidth;
  SelectFont (systemFont);

  sb = GetSlateHScrollBar ((SlatE) udp->letters);
  SetBarMax (sb, udp->aln_length - (Int4) udp->maxchars);
  CorrectBarPage (sb, (Int4) udp->maxchars - 1, (Int4) udp->maxchars - 1);

  udp->recomb1 = -1;
  udp->recomb2 = -1;

  if (udp->new5 > udp->old5 && udp->new3 < udp->old3) {
    /* extend 5' */
    SetValue (udp->rmc, 2);
    /*
    Disable (udp->rmc);
    */
    Disable (b3);
    Disable (b4);
    udp->recomb2 = udp->aln_length;
  } else if (udp->new5 < udp->old5 && udp->new3 > udp->old3) {
    /* extend 3' */
    SetValue (udp->rmc, 3);
    /*
    Disable (udp->rmc);
    */
    Disable (b2);
    Disable (b4);
    udp->recomb1 = 0;
  } else if (udp->new5 >= udp->old5 && udp->new3 >= udp->old3) {
    /* replace */
    SetValue (udp->rmc, 1);
    Disable (udp->rmc);
    udp->recomb1 = 0;
    udp->recomb2 = udp->aln_length;
  } else if (udp->new5 <= udp->old5 && udp->new3 <= udp->old3) {
    /* patch */
    SetValue (udp->rmc, 4);
    /*
    Disable (udp->rmc);
    */
    Disable (b2);
    Disable (b3);
    udp->recomb1 = 0;
    udp->recomb2 = udp->aln_length;
    /* if patch sequence matches, must be feature propagation only */
    if (StringNICmp (udp->seq1 + udp->old5 - udp->new5, udp->seq2, StringLen (udp->seq2)) == 0) {
      SetValue (udp->sfb, 2);
      Disable (udp->sfb);
    }
  }

  /* if no features, must be sequence update only */

  entityID = ObjMgrGetEntityIDForPointer (newbsp);
  if (! SeqMgrFeaturesAreIndexed (entityID)) {
    SeqMgrIndexFeatures (entityID, NULL);
  }
  sfp = SeqMgrGetNextFeature (newbsp, NULL, 0, 0, &fcontext);
  if (sfp == NULL) {
    SetValue (udp->sfb, 1);
    Disable (udp->sfb);
    Disable (udp->nobm);
  }

  /* if different organisms, must be feature propagation only */

  orp1 = NULL;
  orp2 = NULL;
  sdp = SeqMgrGetNextDescriptor (oldbsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop1 = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop1 != NULL) {
      orp1 = biop1->org;
    }
  }
  sdp = SeqMgrGetNextDescriptor (newbsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop2 = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop2 != NULL) {
      orp2 = biop2->org;
    }
  }
  if (orp1 != NULL && orp2 != NULL) {
    if (StringICmp (orp1->taxname, orp2->taxname) != 0) {
      if (sfp != NULL) {
        SetValue (udp->sfb, 2);
        Disable (udp->sfb);
        udp->diffOrgs = TRUE;
      } else {
        /* no features, cannot do anything */
        SetValue (udp->sfb, 0);
        Disable (udp->sfb);
        Disable (udp->nobm);
        Message (MSG_OK, "Organisms are different, no features to propagate, so nothing to do");
      }
      Disable (udp->rmc);
    }
  }

  /* if either sequence is not raw, only allow feature propagation */

  if (oldbsp->repr != Seq_repr_raw || newbsp->repr != Seq_repr_raw) {
    SetValue (udp->sfb, 2);
    Disable (udp->sfb);
  }

  /* disable accept button unless rmc and sfb are both preset */

  UpdateAccept (udp->rmc);

/*
{
  AsnIoPtr  aip;
  Char      path [PATH_MAX];

  TmpNam (path);
  aip = AsnIoOpen (path, "w");
  if (aip != NULL) {
    SeqAlignAsnWrite (salp, aip, NULL);
    AsnIoClose (aip);
    DisplayFancy (udp->doc, path, NULL, NULL, programFont, 0);
  }
  FileRemove (path);
}
*/

  return (ForM) w;
}

static void HasPubsCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  BoolPtr       rsult;
  ValNodePtr    sdp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  rsult = (BoolPtr) mydata;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
  } else return;
  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_pub) {
      *rsult = TRUE;
      return;
    }
    sdp = sdp->next;
  }
}

static Boolean LIBCALL SeqEntryHasPubs (SeqEntryPtr sep)

{
  Boolean  rsult;

  rsult = FALSE;
  if (sep != NULL) {
    SeqEntryExplore (sep, (Pointer) &rsult, HasPubsCallback);
  }
  return rsult;
}

static Boolean CheckForIDCollision (
  BioseqPtr oldbsp,
  BioseqPtr newbsp
)

{
  SeqIdPtr  sip;

  if (oldbsp == NULL || newbsp == NULL) return FALSE;
  for (sip = newbsp->id; sip != NULL; sip = sip->next) {
    if (SeqIdIn (sip, oldbsp->id)) return TRUE;
  }
  return FALSE;
}

static CharPtr convPubDescMssg =
"Do you wish to convert publications to apply only to the appropriate ranges?";

extern void PrepareToUpdateSequences (
  BioseqPtr oldbsp,
  BioseqPtr newbsp
);
extern void PrepareToUpdateSequences (
  BioseqPtr oldbsp,
  BioseqPtr newbsp
)

{
  Uint2        entityID;
  ForM         f;
  SeqEntryPtr  oldsep, newsep;
  Boolean      revcomp = FALSE;
  SeqAlignPtr  salp = NULL;
  SeqIdPtr     sip, tempid = NULL;

  if (oldbsp == NULL || newbsp == NULL) return;
  if (ISA_na (oldbsp->mol) != ISA_na (newbsp->mol)) {
    Message (MSG_OK, "Both sequences must be either nucleotides or proteins");
    return;
  }
  /*
  if (oldbsp->repr != Seq_repr_raw || newbsp->repr != Seq_repr_raw) {
    Message (MSG_OK, "Both sequences must be raw");
    return;
  }
  */

  entityID = ObjMgrGetEntityIDForPointer (oldbsp);
  oldsep = GetBestTopParentForData (entityID, oldbsp);
  entityID = ObjMgrGetEntityIDForPointer (newbsp);
  newsep = GetBestTopParentForData (entityID, newbsp);
  if (oldsep == NULL || newsep == NULL) return;

  if (SeqEntryHasPubs (oldsep) || SeqEntryHasPubs (newsep)) {
    if (Message (MSG_YN, convPubDescMssg) == ANS_YES) {
      ConvertPubSrcComDescsToFeats (oldsep, TRUE, FALSE, FALSE, FALSE);
      ConvertPubSrcComDescsToFeats (newsep, TRUE, FALSE, FALSE, FALSE);
    }
  }

  WatchCursor ();
  tempid = NULL;
  if (CheckForIDCollision (oldbsp, newbsp)) {
    sip = SeqIdParse ("lcl|SequinUpdateSequence");
    if (sip != NULL) {
      /* unlink colliding id, temporarily replace */
      tempid = newbsp->id;
      newbsp->id = sip;
      SeqMgrReplaceInBioseqIndex (newbsp);
    }
  }
  salp = Sqn_GlobalAlign2Seq (oldbsp, newbsp, &revcomp);
  if (tempid != NULL) {
    /* add back colliding id now that blast is done */
    sip->next = tempid;
    SeqMgrReplaceInBioseqIndex (newbsp);
  }
  ArrowCursor ();
  if (salp == NULL) {
    Message (MSG_OK, "Unable to align the sequences");
    return;
  }

  f = UpdateSequenceForm (oldbsp, newbsp, salp, revcomp);
  if (f == NULL) return;
  Show (f);
  Select (f);
  SendHelpScrollMessage (helpForm, "Edit Menu", "Update Sequence");
}

extern void NewUpdateSequence (
  IteM i
);
extern void NewUpdateSequence (
  IteM i
)

{
  MsgAnswer     ans;
  BaseFormPtr   bfp;
  BioseqPtr     bsp, nbsp;
  Pointer       dataptr;
  Uint2         datatype;
  FILE          *fp;
  Char          path [PATH_MAX];
  SeqEntryPtr   sep, nwsep = NULL;
  SeqSubmitPtr  ssp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  bsp = GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp == NULL) return;

  /*
  if (bsp->repr != Seq_repr_raw) {
    Message (MSG_OK, "Only raw sequences can be updated");
    return;
  }
  */

  if (! GetInputFileName (path, sizeof (path),"","TEXT")) return;
  fp = FileOpen (path, "r");
  if (fp == NULL) return;
  dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE, TRUE, FALSE);
  FileClose (fp);
  if (dataptr == NULL) return;

  if (datatype == OBJ_SEQENTRY) {
    nwsep = (SeqEntryPtr) dataptr;
  } else if (datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET) {
    nwsep = SeqMgrGetSeqEntryForData (dataptr);
  } else if (datatype == OBJ_SEQSUB) {
    ssp = (SeqSubmitPtr) dataptr;
    if (ssp != NULL && ssp->datatype == 1) {
      nwsep = (SeqEntryPtr) ssp->data;
    }
  }
  if (nwsep == NULL) return;
  if (ISA_na (bsp->mol)) {
    nbsp = FindNucBioseq (nwsep);
  } else {
    nwsep = FindNthBioseq (nwsep, 1);
    if (nwsep == NULL || nwsep->choice != 1) return;
    nbsp = (BioseqPtr) nwsep->data.ptrvalue;
  }
  if (nbsp == NULL) return;

  if (bsp->repr != Seq_repr_raw) {
    ans = Message (MSG_YN, "Only raw sequences can be updated.  Do you wish to proceed for copying features?");
    if (ans == ANS_NO) return;
  }

  PrepareToUpdateSequences (bsp, nbsp);
}


/* NEW FEATURE PROPAGATION SECTION */

typedef struct fprdata {
  FORM_MESSAGE_BLOCK
  BioseqPtr           bsp;
  SeqAlignPtr         salp;
  Uint2               selFeatItemID;
  Int4                aln_length;
  Int4                log10_aln_length;
  VieweR              details;
  SegmenT             dtpict;
  Int4                scaleX;
  GrouP               allOrSel;
  GrouP               gapSplit;
  ButtoN              stopCDS;
  ButtoN              transPast;
  ButtoN              accept;
} FprData, PNTR FprDataPtr;

typedef struct ivalinfo {
  Int4  start1;
  Int4  stop1;
  Int4  start2;
  Int4  stop2;
  struct ivalinfo PNTR next;
} IvalInfo, PNTR IvalInfoPtr;

static IvalInfoPtr IvalInfoFree (
  IvalInfoPtr ival
)

{
  IvalInfoPtr  next;

  while (ival != NULL) {
    next = ival->next;
    MemFree (ival);
    ival = next;
  }
  return NULL;
}

static IvalInfoPtr GetAlignmentIntervals (SeqAlignPtr sap, Int4 row1, Int4 row2, Int4 from, Int4 to)
{
   AlnMsgPtr    amp1;
   AlnMsgPtr    amp2;
   IvalInfoPtr  ival;
   IvalInfoPtr  ival_head;
   IvalInfoPtr  ival_prev;
   Int4         from_aln;
   Int4         start;
   Int4         stop;
   Int4         tmp;
   Int4         to_aln;

   if (sap == NULL || sap->saip == NULL)
      return NULL;
   AlnMgrGetNthSeqRangeInSA(sap, row1, &start, &stop);
   if (from < start)
      from = start;
   if (to > stop)
      to = stop;
   from_aln = AlnMgrMapBioseqToSeqAlign(sap, from, row1, NULL);
   to_aln = AlnMgrMapBioseqToSeqAlign(sap, to, row1, NULL);
   if (from_aln > to_aln)
   {
      tmp = from_aln;
      from_aln = to_aln;
      to_aln = tmp;
   }
   amp1 = AlnMsgNew();
   amp1->from_m = from_aln;
   amp1->to_m = to_aln;
   amp1->row_num = row1;
   amp2 = AlnMsgNew();
   amp2->from_m = from_aln;
   amp2->to_m = to_aln;
   amp2->row_num = row2;
   ival_head = ival_prev = NULL;
   while (AlnMgrGetNextAlnBit(sap, amp1) && AlnMgrGetNextAlnBit(sap, amp2))
   {
      if (amp1->gap == 0 && amp2->gap == 0)
      {
         ival = (IvalInfoPtr)MemNew(sizeof(IvalInfo));
         ival->start1 = amp1->from_b;
         ival->stop1 = amp1->to_b;
         ival->start2 = amp2->from_b;
         ival->stop2 = amp2->to_b;
         if (ival_head == NULL)
            ival_head = ival_prev = ival;
         else
         {
            ival_prev->next = ival;
            ival_prev = ival;
         }
      }
   }
   AlnMsgFree(amp1);
   AlnMsgFree(amp2);
   return ival_head;
}

static IvalInfoPtr MergeAdjacentIntervals (
  IvalInfoPtr list
)

{
  IvalInfoPtr  curr, last, next;

  if (list != NULL) {
    curr = list->next;
    last = list;
    while (curr != NULL) {
      next = curr->next;
      if (curr->start2 == last->stop2 + 1) {
        last->stop2 = MAX (curr->stop2, last->stop2);
        MemFree (curr);
        last->next = next;
      } else {
        last = curr;
      }
      curr = next;
    }
  }
  return list;
}

static SeqLocPtr MapLocForProp (
  SeqAlignPtr salp,
  Int4 begin,
  Int4 fin,
  SeqIdPtr sip,
  Boolean gapSplit,
  SeqLocPtr location
)

{
  SeqLocPtr    head = NULL;
  IvalInfoPtr  ival;
  IvalInfoPtr  ival_head;
  SeqIntPtr    sinp;
  SeqLocPtr    slp;
  SeqPntPtr    spp;
  Uint1        strand;

  slp = SeqLocFindNext (location, NULL);
  while (slp != NULL) {
    switch (slp->choice) {
    case SEQLOC_INT :
      sinp = (SeqIntPtr) slp->data.ptrvalue;
      if (sinp != NULL) {
        strand = sinp->strand;
        ival_head = GetAlignmentIntervals (salp, begin, fin, sinp->from, sinp->to);
        ival_head = MergeAdjacentIntervals (ival_head);
        if (ival_head != NULL) {

          /* what if one or the other interval maps into a gap? */

          if (gapSplit) {
            for (ival = ival_head; ival != NULL; ival = ival->next) {
              sinp = SeqIntNew ();
              sinp->from = ival->start2;
              sinp->to = ival->stop2;
              sinp->strand = strand;
              sinp->id = SeqIdDup (sip);
              ValNodeAddPointer (&head, SEQLOC_INT, (Pointer) sinp);
            }
          } else {
            sinp = SeqIntNew ();
            sinp->from = ival_head->start2;
            for (ival = ival_head; ival->next != NULL; ival = ival->next) continue;
            sinp->to = ival->stop2;
            sinp->strand = strand;
            sinp->id = SeqIdDup (sip);
            ValNodeAddPointer (&head, SEQLOC_INT, (Pointer) sinp);
          }

        }
        IvalInfoFree (ival_head);
      }
      break;
    case SEQLOC_PNT :
      spp = (SeqPntPtr) slp->data.ptrvalue;
      if (spp != NULL) {
        strand = spp->strand;
        ival_head = GetAlignmentIntervals (salp, begin, fin, spp->point, spp->point);
        if (ival_head != NULL) {

          spp = SeqPntNew ();
          spp->point = ival_head->start2;
          spp->strand = strand;
          spp->id = SeqIdDup (sip);
          ValNodeAddPointer (&head, SEQLOC_PNT, (Pointer) spp);

        }
        IvalInfoFree (ival_head);
      }
      break;
    case SEQLOC_PACKED_PNT :
      /* not yet implemented */
      break;
    default :
      break;
    }

    slp = SeqLocFindNext (location, slp);
  }

  if (head == NULL) return NULL;
  if (head->next == NULL) return head;

  slp = ValNodeNew (NULL);
  slp->choice = SEQLOC_MIX;
  slp->data.ptrvalue = (Pointer) head;
  slp->next = NULL;

  return slp;
}

static void ExtendLocToEnd (
  SeqLocPtr location,
  BioseqPtr bsp,
  Uint1 strand
)

{
  SeqIntPtr  sinp;
  SeqLocPtr  slp, last = NULL;

  slp = SeqLocFindNext (location, NULL);
  while (slp != NULL) {
    last = slp;
    slp = SeqLocFindNext (location, slp);
  }
  if (last == NULL) return;

  switch (last->choice) {
    case SEQLOC_INT :
      sinp = (SeqIntPtr) last->data.ptrvalue;
      if (sinp != NULL) {
        if (strand == Seq_strand_minus) {
          sinp->from = 0;
        } else {
          sinp->to = bsp->length - 1;
        }
      }
    case SEQLOC_PNT :
      /* not yet implemented */
      break;
    case SEQLOC_PACKED_PNT :
      /* not yet implemented */
      break;
    default :
      break;
  }
}

static void TruncateCDS (
  SeqFeatPtr sfp,
  BioseqPtr pbsp
)

{
  Int4       len;
  SeqIntPtr  sinp;
  SeqLocPtr  slp;
  Int4       total = 0;

  slp = SeqLocFindNext (sfp->location, NULL);
  while (slp != NULL) {
    len = SeqLocLen (slp);

    if (len + total <= (pbsp->length + 1) * 3) {
      total += len;
    } else {
      if (slp->choice == SEQLOC_INT) {
        sinp = (SeqIntPtr) slp->data.ptrvalue;
        if (sinp != NULL) {
          len = (pbsp->length + 1) * 3 - total;
          if (sinp->strand == Seq_strand_minus) {
            sinp->from = sinp->to - len + 1;
          } else {
            sinp->to = sinp->from + len - 1;
          }
        }
      }
      return;
    }

    slp = SeqLocFindNext (sfp->location, slp);
  }
}

static void DoFeatProp (
  BioseqPtr bsp,
  SeqAlignPtr salp,
  Int4 begin,
  Int4 fin,
  SeqFeatPtr sfp,
  Boolean gapSplit,
  Boolean stopCDS,
  Boolean transPast,
  Boolean cds3end,
  Uint1 strand
)

{
  CodeBreakPtr    cbp, prevcbp, nextcbp;
  CdRegionPtr     crp;
  SeqFeatPtr      dup, prot;
  Uint2           entityID;
  MolInfoPtr      mip;
  BioseqPtr       newbsp, pbsp;
  SeqLocPtr       newloc;
  Boolean         partial5;
  Boolean         partial3;
  ProtRefPtr      prp;
  RnaRefPtr       rrp;
  SeqDescrPtr     sdp;
  SeqEntryPtr     sep;
  SeqIdPtr        sip;
  tRNAPtr         trp;
  SeqFeatXrefPtr  xref;
  Boolean         xtend;

  if (bsp == NULL || salp == NULL || sfp == NULL) return;

  sip = AlnMgrGetNthSeqIdPtr (salp, fin);
  if (sip == NULL) return;

  newloc = MapLocForProp (salp, begin, fin, sip, gapSplit, sfp->location);
  if (newloc == NULL) return;

  dup = AsnIoMemCopy ((Pointer) sfp,
                      (AsnReadFunc) SeqFeatAsnRead,
                      (AsnWriteFunc) SeqFeatAsnWrite);
  SeqLocFree (dup->location);
  dup->location = newloc;

  switch (dup->data.choice) {
    case SEQFEAT_CDREGION :
      crp = (CdRegionPtr) dup->data.value.ptrvalue;
      if (crp != NULL) {
        prevcbp = NULL;
        for (cbp = crp->code_break; cbp != NULL; cbp = nextcbp) {
          nextcbp = cbp->next;
          newloc = MapLocForProp (salp, begin, fin, sip, gapSplit, cbp->loc);
          SeqLocFree (cbp->loc);
          cbp->loc = newloc;
          if (cbp->loc == NULL) {
            if (prevcbp != NULL) {
              prevcbp->next = nextcbp;
            } else {
              crp->code_break = nextcbp;
            }
            cbp->next = NULL;
            CodeBreakFree (cbp);
          } else {
            prevcbp = cbp;
          }
        }
      }
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) dup->data.value.ptrvalue;
      if (rrp != NULL && rrp->ext.choice == 2) {
        trp = (tRNAPtr) rrp->ext.value.ptrvalue;
        if (trp != NULL && trp->anticodon != NULL) {
          newloc = MapLocForProp (salp, begin, fin, sip, gapSplit, trp->anticodon);
          SeqLocFree (trp->anticodon);
          trp->anticodon = newloc;
        }
      }
      break;
    default :
      break;
  }

  newbsp = BioseqFindCore (sip);
  if (newbsp == NULL) return;
  sep = SeqMgrGetSeqEntryForData (newbsp);
  if (sep == NULL) return;
  CreateNewFeature (sep, NULL, dup->data.choice, dup);

  if (dup->product != NULL) {
    dup->product = SeqLocFree (dup->product);
  }

  if (dup->data.choice != SEQFEAT_CDREGION) return;

  if (transPast && cds3end) {
    ExtendLocToEnd (dup->location, newbsp, strand);
  }

  sip = SeqLocId (sfp->product);
  if (sip == NULL) return;
  pbsp = BioseqFindCore (sip);
  if (pbsp == NULL) return;
  prot = SeqMgrGetBestProteinFeature (pbsp, NULL);
  if (prot == NULL || prot->data.choice != SEQFEAT_PROT) return;
  prp = (ProtRefPtr) prot->data.value.ptrvalue;
  if (prp == NULL) return;
  prp = AsnIoMemCopy ((Pointer) prp,
                       (AsnReadFunc) ProtRefAsnRead,
                       (AsnWriteFunc) ProtRefAsnWrite);
  xref = SeqFeatXrefNew ();
  if (xref == NULL) return;
  xref->data.choice = SEQFEAT_PROT;
  xref->data.value.ptrvalue = (Pointer) prp;
  xref->next = dup->xref;
  dup->xref = xref;

  entityID = ObjMgrGetEntityIDForPointer (newbsp);
  PromoteXrefsEx (dup, newbsp, entityID, (Boolean) (! stopCDS), FALSE);

  /* now truncate new cds based on new protein length */

  sip = SeqLocId (dup->product);
  if (sip == NULL) return;
  pbsp = BioseqFindCore (sip);
  if (pbsp == NULL) return;
  TruncateCDS (dup, pbsp);

  CheckSeqLocForPartial (dup->location, &partial5, &partial3);
  if (cds3end) {
    xtend = FALSE;
    if (strand == Seq_strand_minus) {
      if (SeqLocStop (dup->location) == 0) {
        xtend = TRUE;
      }
    } else {
      if (SeqLocStop (dup->location) == newbsp->length - 1) {
        xtend = TRUE;
      }
    }
    if (xtend) {
      partial3 = TRUE;
      SetSeqLocPartial (dup->location, partial5, partial3);
      for (sdp = pbsp->descr; sdp != NULL; sdp = sdp->next) {
        if (sdp->choice != Seq_descr_molinfo) continue;
        mip = (MolInfoPtr) sdp->data.ptrvalue;
        if (mip == NULL) continue;
        if (partial5 && partial3) {
          mip->completeness = 5;
        } else if (partial5) {
          mip->completeness = 3;
        } else if (partial3) {
          mip->completeness = 4;
        }
      }
    }
  }
  dup->partial = (Boolean) (partial5 || partial3);
}

static Boolean CDSgoesToEnd (
  BioseqPtr bsp,
  SeqMgrFeatContext PNTR fcontext
)

{
  if (fcontext->strand == Seq_strand_minus) {
    if (fcontext->left == 0 && fcontext->partialR) return TRUE;
  } else {
    if (fcontext->right == bsp->length - 1 && fcontext->partialR) return TRUE;
  }
  return FALSE;
}

static void AcceptFeatProp (
  ButtoN b
)

{
  BioseqPtr          bsp;
  Boolean            cds3end;
  DenseSegPtr        dsp;
  Uint2              entityID;
  SeqMgrFeatContext  fcontext;
  FprDataPtr         fdp;
  Boolean            gapSplit;
  Int4               i;
  Int4               numRows;
  Int4               row;
  SeqAlignPtr        salp;
  SeqFeatPtr         sfp;
  SeqIdPtr           sip;
  SeqIdPtr           sip_head;
  SeqIdPtr           sip_new;
  SeqIdPtr           sip_prev;
  SeqIdPtr           sip_tmp;
  Boolean            stopCDS;
  Boolean            transPast;

  fdp = (FprDataPtr) GetObjectExtra (b);
  if (fdp == NULL) return;
  SafeHide (fdp->form);

  bsp = fdp->bsp;
  salp = fdp->salp;
  if (bsp == NULL || salp == NULL) {
    Remove (fdp->form);
    return;
  }

  if (GetValue (fdp->allOrSel) == 1) {
    fdp->selFeatItemID = 0;
  }
  if (GetValue (fdp->gapSplit) == 1) {
    gapSplit = FALSE;
  } else {
    gapSplit= TRUE;
  }
  if (GetStatus (fdp->stopCDS)) {
    stopCDS = TRUE;
  } else {
    stopCDS = FALSE;
  }
  if (GetStatus (fdp->transPast)) {
    transPast = TRUE;
  } else {
    transPast = FALSE;
  }

  SeqEntrySetScope (NULL);

  SAIndexFree (salp->saip);
  salp->saip = NULL;
  AlnMgrIndexSingleChildSeqAlign (salp);

  dsp = (DenseSegPtr)(salp->segs);
  sip = SeqIdFindBest (bsp->id, 0);
  sip_tmp = dsp->ids;
  sip_head = sip_prev = NULL;
  while (sip_tmp != NULL)
  {
    if (SeqIdComp(sip_tmp, bsp->id) == SIC_YES)
       sip_new = SeqIdDup(sip);
    else
       sip_new = SeqIdFindBest(sip_tmp, 0);
    if (sip_head != NULL)
    {
       sip_prev->next = sip_new;
       sip_prev = sip_new;
    } else
       sip_head = sip_prev = sip_new;
    sip_tmp = sip_tmp->next;
  }
  dsp->ids = sip_head;
  row = AlnMgrGetNForSip (salp, sip);
  numRows = AlnMgrGetNumRows (salp);

  if (row == -1) {
    Message (MSG_OK, "AlnMgrGetNForSip failed");
    Remove (fdp->form);
    return;
  }
  if (numRows < 1) {
    Message (MSG_OK, "AlnMgrGetNumRows failed");
    Remove (fdp->form);
    return;
  }
  if (row < 1 || row > numRows) {
    Message (MSG_OK, "row is %ld, numRows is %ld, unable to process", (long) row, (long) numRows);
    Remove (fdp->form);
    return;
  }

  if (fdp->selFeatItemID != 0) {

    /* propagate single selected feature */

    sfp = SeqMgrGetDesiredFeature (0, bsp, fdp->selFeatItemID, 0, NULL, &fcontext);
    if (sfp != NULL) {
      cds3end = CDSgoesToEnd (bsp, &fcontext);
      for (i = 1; i <= numRows; i++) {
        if (i != row) {
          DoFeatProp (bsp, salp, row, i, sfp, gapSplit, stopCDS, transPast, cds3end, fcontext.strand);
        }
      }
    }
  } else {

    /* propagate all features on bioseq */

    sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
    while (sfp != NULL) {
      cds3end = CDSgoesToEnd (bsp, &fcontext);
      for (i = 1; i <= numRows; i++) {
        if (i != row) {
          DoFeatProp (bsp, salp, row, i, sfp, gapSplit, stopCDS, transPast, cds3end, fcontext.strand);
        }
      }

      sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
    }
  }

  entityID = ObjMgrGetEntityIDForPointer (bsp);
  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);

  Remove (fdp->form);
}

static ForM FeaturePropagateForm (
  BioseqPtr bsp,
  SeqAlignPtr salp,
  Uint2 selFeatItemID
)

{
  ButtoN      b;
  GrouP       c;
  FprDataPtr  fdp;
  GrouP       g;
  PrompT      ppt;
  SeqIdPtr    sip;
  Char        strid [41];
  Char        txt [128];
  WindoW      w;

  if (bsp == NULL) return NULL;
  fdp = (FprDataPtr) MemNew (sizeof (FprData));
  if (fdp == NULL) return NULL;
  w = FixedWindow (-50, -33, -10, -10, "Feature Propagate", NULL);
  if (w == NULL) return NULL;

  SetObjectExtra (w, (Pointer) fdp, StdCleanupFormProc);
  fdp->form = (ForM) w;
  fdp->formmessage = UpdateSequenceFormMessage;

#ifdef WIN_MAC
  fdp->activate = UpdateSequenceFormActivated;
  SetActivate (w, UpdateSequenceFormActivate);
#endif

  fdp->bsp = bsp;
  fdp->salp = salp;
  fdp->selFeatItemID = selFeatItemID;

  sip = SeqIdFindWorst (bsp->id);
  SeqIdWrite (sip, strid, PRINTID_REPORT, sizeof (strid) - 1);
  if (ISA_na (bsp->mol)) {
    sprintf (txt, "Propagate from %s to all other nucleotides", strid);
  } else {
    sprintf (txt, "Propagate from %s to all other proteins", strid);
  }

  g = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (g, 5, 5);

  ppt = StaticPrompt (g, txt, 0, 0, programFont, 'c');

  fdp->allOrSel = HiddenGroup (g, 2, 0, NULL);
  RadioButton (fdp->allOrSel, "All Features");
  b = RadioButton (fdp->allOrSel, "Selected Feature");
  if (selFeatItemID > 0) {
    SetValue (fdp->allOrSel, 2);
  } else {
    Disable (b);
    SetValue (fdp->allOrSel, 1);
  }

  fdp->gapSplit = HiddenGroup (g, 2, 0, NULL);
  RadioButton (fdp->gapSplit, "Extend over gaps");
  RadioButton (fdp->gapSplit, "Split at gaps");
  SetValue (fdp->gapSplit, 1);

  fdp->stopCDS = CheckBox (g, "Stop CDS translation at internal stop codon", NULL);
  SetStatus (fdp->stopCDS, TRUE);

  fdp->transPast = CheckBox (g, "Translate CDS after partial 3' boundary", NULL);

  c = HiddenGroup (w, 4, 0, NULL);
  fdp->accept = PushButton (c, "Accept", AcceptFeatProp);
  SetObjectExtra (fdp->accept, (Pointer) fdp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) ppt, (HANDLE) fdp->allOrSel,
                (HANDLE) fdp->gapSplit, (HANDLE) fdp->stopCDS,
                (HANDLE) fdp->transPast, (HANDLE) c, NULL);
  RealizeWindow (w);

  return (ForM) w;
}

extern void NewFeaturePropagate (
  IteM i
);
extern void NewFeaturePropagate (
  IteM i
)

{
  BaseFormPtr        bfp;
  BioseqPtr          bsp;
  ForM               f;
  SeqMgrFeatContext  fcontext;
  Uint2              itemID = 0;
  SeqAlignPtr        salp;
  SeqFeatPtr         sfp;
  SelStructPtr       sel;
  SeqEntryPtr        sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  bsp = GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  if (bsp == NULL) {
    Message (MSG_OK, "You must target a single sequence in order to propagate");
    return;
  }
  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  if (sfp == NULL) {
    Message (MSG_OK, "The sequence must have features in order to propagate");
    return;
  }
  salp = SeqMgrFindSeqAlignByID (bfp->input_entityID, 1);
  if (salp == NULL) {
    Message (MSG_OK, "The record must have an alignment in order to propagate");
    return;
  }

  sel = ObjMgrGetSelected ();
  if (sel != NULL && sel->entityID == bfp->input_entityID &&
      sel->next == NULL && sel->itemtype == OBJ_SEQFEAT) {
    sfp = SeqMgrGetDesiredFeature (bfp->input_entityID, NULL, sel->itemID, 0, NULL, &fcontext);
    if (sfp != NULL && fcontext.bsp == bsp) {
      itemID = sel->itemID;
    }
  }

  f = FeaturePropagateForm (bsp, salp, itemID);
  if (f == NULL) return;
  Show (f);
  Select (f);
  SendHelpScrollMessage (helpForm, "Edit Menu", "Feature Propagate");
}

/* taken from ripen.c */

static Boolean PropagateFromGenomicProductSet (SeqEntryPtr sep)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqEntryPtr   seqentry;
  ValNodePtr    sourcedescr;

  if (sep != NULL) {
    if (sep->choice == 2 && sep->data.ptrvalue != NULL) {

      /* get descriptors packaged on genomic product set */

      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      sourcedescr = bssp->descr;
      if (sourcedescr == NULL) return FALSE;
      if (bssp->_class == BioseqseqSet_class_gen_prod_set) {
        seqentry = bssp->seq_set;
        while (seqentry != NULL) {

          /* copy descriptors onto contig and nuc-prot sets */

          if (seqentry->data.ptrvalue != NULL) {
            if (seqentry->choice == 1) {
              bsp = (BioseqPtr) seqentry->data.ptrvalue;
              ValNodeLink (&(bsp->descr),
                           AsnIoMemCopy ((Pointer) sourcedescr,
                                         (AsnReadFunc) SeqDescrAsnRead,
                                         (AsnWriteFunc) SeqDescrAsnWrite));
            } else if (seqentry->choice == 2) {
              bssp = (BioseqSetPtr) seqentry->data.ptrvalue;
              ValNodeLink (&(bssp->descr),
                           AsnIoMemCopy ((Pointer) sourcedescr,
                                         (AsnReadFunc) SeqDescrAsnRead,
                                         (AsnWriteFunc) SeqDescrAsnWrite));
            }
          }
          seqentry = seqentry->next;
        }

        /* and free descriptors from genomic product set */

        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        bssp->descr = SeqDescrFree (bssp->descr);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static void CopyUserObject (SeqFeatPtr sfp, Pointer userdata)

{
  BioseqPtr          bsp;
  SeqMgrDescContext  dcontext;
  SeqDescrPtr        sdp;
  UserObjectPtr      uop;

  if (sfp->idx.subtype != FEATDEF_mRNA) return;

  /* find product cdna of mrna feature */

  bsp = BioseqFindFromSeqLoc (sfp->product);
  if (bsp == NULL) return;

  /* get closest user object descriptor */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &dcontext);

  /* make sure evidence user object is no higher than nuc-prot set */

  if (sdp == NULL || dcontext.level > 1) return;

  /* copy user object, place on mrna feature */

  uop = (UserObjectPtr) sdp->data.ptrvalue;
  if (uop == NULL) return;
  uop = AsnIoMemCopy (uop,
                       (AsnReadFunc) UserObjectAsnRead,
                       (AsnWriteFunc) UserObjectAsnWrite);
  if (uop == NULL) return;

  /* should not be a user object there, but use combine function just in case */

  sfp->ext = CombineUserObjects (sfp->ext, uop);
}

static void CopyGene (SeqFeatPtr sfp, Pointer userdata)

{
  BioseqPtr          bsp;
  SeqMgrFeatContext  gcontext;
  SeqFeatPtr         gene, copy, temp;

  /* input mrna features are multi-interval on contig */

  if (sfp->idx.subtype != FEATDEF_mRNA) return;

  /* overlapping gene should be single interval on contig */

  gene = SeqMgrGetOverlappingGene (sfp->location, &gcontext);
  if (gene == NULL) return;

  /* find cdna product of mrna */

  bsp = BioseqFindFromSeqLoc (sfp->product);
  if (bsp == NULL) return;

  /* copy gene feature fields to paste into new gene feature */

  temp = AsnIoMemCopy (gene,
                       (AsnReadFunc) SeqFeatAsnRead,
                       (AsnWriteFunc) SeqFeatAsnWrite);
  if (temp == NULL) return;

  /* make new gene feature on full-length of cdna */

  copy = CreateNewFeatureOnBioseq (bsp, SEQFEAT_GENE, NULL);
  if (copy == NULL) return;

  /* paste fields from temp copy of original gene */

  copy->data.value.ptrvalue = temp->data.value.ptrvalue;
  copy->partial = temp->partial;
  copy->excpt = temp->excpt;
  copy->comment = temp->comment;
  copy->qual = temp->qual;
  copy->title = temp->title;
  copy->ext = temp->ext;
  copy->cit = temp->cit;
  copy->exp_ev = temp->exp_ev;
  copy->xref = temp->xref;
  copy->dbxref = temp->dbxref;
  copy->pseudo = temp->pseudo;
  copy->except_text = temp->except_text;

  MemFree (temp); /* do not SeqFeatFree */
}

static void CopyCDS (SeqFeatPtr sfp, Pointer userdata)

{
  BioseqPtr          bsp, cdna;
  Boolean            madeMrnaProtLink = FALSE;
  SeqMgrFeatContext  mcontext;
  SeqFeatPtr         mrna, cds, temp;
  SeqLocPtr          slp, cbslp;
  UserObjectPtr      uop;
  CdRegionPtr        crp;
  CodeBreakPtr       cbp;

  /* input cds features are single interval on cdna in nuc-prot sets, non on contig */

  if (sfp->idx.subtype != FEATDEF_CDS) return;

  /* map cds location from cdna to contig via mrna feature intervals on contig */

  cdna = BioseqFindFromSeqLoc (sfp->location);
  if (cdna == NULL) return;
  mrna = SeqMgrGetRNAgivenProduct (cdna, &mcontext);
  if (mrna == NULL) return;
  slp = productLoc_to_locationLoc (mrna, sfp->location);
  if (slp == NULL) return;
  bsp = BioseqFindFromSeqLoc (slp);
  if (bsp == NULL) return;

  /* copy cds feature fields to paste into new cds feature */

  temp = AsnIoMemCopy (sfp,
                       (AsnReadFunc) SeqFeatAsnRead,
                       (AsnWriteFunc) SeqFeatAsnWrite);
  if (temp == NULL) return;

  cds = CreateNewFeatureOnBioseq (bsp, SEQFEAT_CDREGION, NULL);
  if (cds == NULL) return;

  /* replace cdna location with contig location */

  cds->location = SeqLocFree (cds->location);
  cds->location = slp;

  /* now two cds products point to same protein bioseq, okay for genomic product set */

  cds->product = AsnIoMemCopy (sfp->product,
                               (AsnReadFunc) SeqLocAsnRead,
                               (AsnWriteFunc) SeqLocAsnWrite);

  /* paste fields from temp copy of original cds */

  cds->data.value.ptrvalue = temp->data.value.ptrvalue;

  /* update code breaks */
  crp = (CdRegionPtr) cds->data.value.ptrvalue;
  if (crp != NULL) {
    if (mrna->excpt) {
      /* Exception, e.g. unclassified transcription discrepancy: gap */
      if (crp->code_break != NULL) {
	crp->code_break =  CodeBreakFree (crp->code_break); /* XXX Remove list of code breaks; any better fix?*/
      }
    } else {
      for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
	cbslp = productLoc_to_locationLoc (mrna, cbp->loc);
	assert (cbslp != NULL);
	cbp->loc = SeqLocFree (cbp->loc);
	cbp->loc = cbslp;
      }
    }
  }

  /*
  if (crp != NULL) {
    crp->frame = 0;
  }
  */
  cds->partial = temp->partial;
  cds->excpt = temp->excpt;
  cds->comment = temp->comment;
  cds->qual = temp->qual;
  cds->title = temp->title;
  cds->ext = temp->ext;
  cds->cit = temp->cit;
  cds->exp_ev = temp->exp_ev;
  cds->xref = temp->xref;
  cds->dbxref = temp->dbxref;
  cds->pseudo = temp->pseudo;
  cds->except_text = temp->except_text;

  MemFree (temp); /* do not SeqFeatFree */

  bsp = BioseqFindFromSeqLoc (sfp->product);
  if (bsp == NULL) return;

  if (mrna->excpt && StringICmp (mrna->except_text, "unclassified transcription discrepancy") == 0) {
    cds->excpt = TRUE;
    cds->except_text = StringSave ("unclassified translation discrepancy");
  }

  /* evidence user object and mrna-cds link user object combined onto mrna on contig */

  if (! madeMrnaProtLink) {
    uop = CreateMrnaProteinLinkUserObject (bsp);
    mrna->ext = CombineUserObjects (mrna->ext, uop);
  }
}

extern void MakeRedundantGPS (
  IteM i
);
extern void MakeRedundantGPS (
  IteM i
)

{
  BaseFormPtr        bfp;
  SeqEntryPtr        sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitFeaturesInSep (sep, NULL, CopyGene);
  VisitFeaturesInSep (sep, NULL, CopyUserObject);
  PropagateFromGenomicProductSet (sep);
  VisitFeaturesInSep (sep, NULL, CopyCDS);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void FuseFeatJoins (SeqFeatPtr sfp, Pointer userdata)

{
  BioseqPtr  bsp;
  SeqLocPtr  slp;

  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp == NULL) return;
  slp = SeqLocMerge (bsp, sfp->location, NULL, FALSE, TRUE, FALSE);
  if (slp == NULL) return;
  sfp->location = SeqLocFree (sfp->location);
  sfp->location = slp;
}

extern void FuseSlpJoins (IteM i);
extern void FuseSlpJoins (IteM i)

{
  BaseFormPtr        bfp;
  SeqEntryPtr        sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitFeaturesInSep (sep, NULL, FuseFeatJoins);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

