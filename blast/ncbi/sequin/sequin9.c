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
* $Revision: 6.326 $
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
#include <alignmgr.h>
#include <urkptpf.h>
#include <entrez.h>
#include <accentr.h>
#include <urlquery.h>
#include <vecscnapi.h>
#include <qblastapi.h>
#include <edutil.h>
#include <actutils.h>
#include <findrepl.h>
#include <rpsutil.h>
#include "sequin.h"
#include <seqpanel.h>
#include <assert.h>

/*-------------------*/
/* Defined Constants */
/*-------------------*/

#define MAX_ID_LEN 41
#define FASTA_READ_OK     0
#define FASTA_READ_ERROR -1
#define FASTA_READ_DONE   1

#define CONVERTPUBS_NOT_SET 0
#define CONVERTPUBS_YES     1
#define CONVERTPUBS_NO      2

/* constants for update sequence */
#define UPDATE_CHOICE_NOT_SET        0
#define UPDATE_SEQUENCE_ONLY         1
#define UPDATE_FEATURES_ONLY         2
#define UPDATE_SEQUENCE_AND_FEATURES 3

#define UPDATE_REPLACE               1
#define UPDATE_EXTEND5               2
#define UPDATE_EXTEND3               3
#define UPDATE_PATCH                 4

enum update_dup_feat_type
{
  UPDATE_FEAT_DUP_NOT_SET = 0,
  UPDATE_FEAT_DUP_USE_NEW,
  UPDATE_FEAT_DUP_USE_OLD,
  UPDATE_FEAT_DUP_USE_BOTH,
  UPDATE_FEAT_DUP_MERGE,
  UPDATE_FEAT_DUP_REPLACE
};


/*-----------------*/
/* Data Structures */
/*-----------------*/

typedef struct {
  Char       newId[MAX_ID_LEN];
  BioseqPtr  matchingBsp;
} UpdateData, PNTR UpdateDataPtr;

typedef struct upsdata {
  FORM_MESSAGE_BLOCK
  ButtoN              accept;
  ButtoN              acceptAll;
  CharPtr             aln1;
  CharPtr             aln2;
  Int4                aln_length;
  Int2                charwidth;
  Int2                convertPubs;
  VieweR              details;
  Boolean             diffOrgs;
  SegmenT             dtpict;
  FILE                *fp;
  ValNodePtr          indels;
  Boolean             isSet;
  ButtoN              keepProteinIDs;
  PaneL               letters;
  Int2                lineheight;
  Int4                log10_aln_length;
  Int2                maxchars;
  ValNodePtr          mismatches;
  Int4                new3;
  Int4                new5;
  Int4                newa;
  BioseqPtr           newbsp;
  ButtoN              replace_all;
  GrouP               nobm;
  Int4                old3;
  Int4                old5;
  Int4                olda;
  BioseqPtr           oldbsp;
  VieweR              overview;
  SegmenT             ovpict;
  Int4                recomb1;
  Int4                recomb2;
  Boolean             revcomp;
  GrouP               rmc;
  SeqAlignPtr         salp;
  Int4                scaleX;
  CharPtr             seq1;
  CharPtr             seq2;
  GrouP               sfb;
  Int4                startmax;
  Int4                stopmax;
  Uint1               strandnew;
  Uint1               strandold;
  Boolean             useGUI;
  Boolean             do_update;
  ButtoN              add_cit_subs;
  ButtoN              update_proteins;
  Boolean             suppress_continue_msg;
  Boolean             suppress_instant_refresh;
  ButtoN              update_quality_scores_btn;
  FILE                *log_fp;
  Char                log_path [PATH_MAX];
  Boolean             data_in_log;
  ButtoN              truncate_proteins_btn;
  ButtoN              extend_proteins5_btn;
  ButtoN              extend_proteins3_btn;
  ButtoN              correct_cds_genes_btn;
  Boolean             truncate_proteins;
  Boolean             extend_proteins5;
  Boolean             extend_proteins3;
  Boolean             correct_cds_genes;
  ValNodePtr          transl_except_list;
  Int2                rmcval;      /* This is the choice selected from the
                                    * rmc radio button group.
                                    * We store it so that we can use it in
                                    * Accept All.
                                    */
  
  SeqEntryPtr         seqsubsep;   /* when we are updating from a Sequin record
                                    * that contains a set, store the set here.
                                    */
  Int4                seqsubpos;   /* when we are updating from a Sequin record
                                    * that contains a set, store the number of
                                    * Bioseqs already processed here.
                                    */
                                    
  Int2                no_aln_choice; /* what to do when updating a set and no
                                      * alignment is found.
                                      */
                                      
} UpsData, PNTR UpsDataPtr;

/*---------------------*/
/* Function prototypes */
/*---------------------*/

static Int2 UpdateNextBioseqInFastaSet (UpsDataPtr udp);
static void FreeUdpFields (UpsDataPtr udp);
static void TruncateCDS (SeqFeatPtr sfp, Uint1 frame, BioseqPtr pbsp);

extern void SubmitToNCBI (IteM i);
extern void QuitProc (void);
extern void NewFeaturePropagate (IteM i);
extern void FixCdsAfterPropagate (IteM i);
extern void MakeRedundantGPSwithProp (IteM i);
extern void MakeRedundantGPSnoProp (IteM i);
extern void MakeRedundantGPSjustXref (IteM i);
extern void FuseSlpJoins (IteM i);

/*-----------*/
/* Functions */
/*-----------*/

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

static Int2 vsquerynum = 0;

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

  vsquerynum++;
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

static void DoVecScreens (BioseqPtr bsp, Pointer userdata)

{
  CharPtr  service;

  if (bsp == NULL || ISA_aa (bsp->mol)) return;
  service = (CharPtr) userdata;
  VecScreenAsynchronousRequest (service, bsp, &vsquerylist, VecScreenCallback, AnnounceCallback, NULL);
}

static void SimpleVecScreenCommon (IteM i, CharPtr service)

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
    if (sep == NULL) return;
    VecScreenAsynchronousRequest (service, bsp, &vsquerylist, VecScreenCallback, AnnounceCallback, NULL);
  } else {
    sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
    if (sep == NULL) return;
    VisitBioseqsInSep (sep, (Pointer) service, DoVecScreens);
  }
}

static void SimpleUniVecScreenProc (IteM i)

{
  SimpleVecScreenCommon (i, "UniVec");
}

static void SimpleUniVecCoreScreenProc (IteM i)

{
  SimpleVecScreenCommon (i, "UniVec_Core");
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

static QUEUE  cddquerylist = NULL;

static Int2  cddquerynum = 0;

#include <cddapi.h>

#define CDD_EXPECT_VALUE 0.01

static Boolean LIBCALLBACK CddProc (
  CONN conn,
  VoidPtr userdata,
  EIO_Status status
)

{
  BioseqPtr    bsp;
  SeqAnnotPtr  prev;
  SeqAnnotPtr  sap;

  if (conn == NULL || userdata == NULL) return TRUE;
  if (status != eIO_Success) return TRUE;
  sap = CddReadReply (conn, status);
  if (sap == NULL) return FALSE;

  bsp = (BioseqPtr) userdata;
  CddCorrectIDs (bsp, sap);

  prev = bsp->annot;
  if (prev == NULL) {
    bsp->annot = sap;
  } else {
    while (prev->next != NULL) {
      prev = prev->next;
    }
    prev->next = sap;
  }

  ObjMgrSetDirtyFlag (bsp->idx.entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bsp->idx.entityID, 0, 0);

  return TRUE;
}

static void SearchCDD (BioseqPtr bsp, Pointer userdata)

{
  BoolPtr  dofeats;

  if (bsp == NULL) return;
  if (! ISA_aa (bsp->mol)) return;

  dofeats = (BoolPtr) userdata;
  if (! CddAsynchronousQuery (bsp, CDD_EXPECT_VALUE, TRUE, TRUE, *dofeats, "cdd", FALSE, &cddquerylist, CddProc, (Pointer) bsp)) {
    ErrPostEx (SEV_ERROR, 0, 0, "Unable to run CDD search");
  } else {
    cddquerynum++;
  }
}

/*
static void SearchCDD (BioseqPtr bsp, Pointer userdata)

{
  BoolPtr      dofeats;
  SeqAnnotPtr  prev;
  SeqAnnotPtr  sap;

  if (bsp == NULL) return;
  if (! ISA_aa (bsp->mol)) return;

  dofeats = (BoolPtr) userdata;
  sap = CddSynchronousQuery (bsp, CDD_EXPECT_VALUE, TRUE, TRUE, *dofeats, "cdd", FALSE);
  if (sap == NULL) return;
  CddCorrectIDs (bsp, sap);

  prev = bsp->annot;
  if (prev == NULL) {
    bsp->annot = sap;
  } else {
    while (prev->next != NULL) {
      prev = prev->next;
    }
    prev->next = sap;
  }
}
*/

static void SimpleCDDSearchCommonProc (IteM i, Boolean makeFeats)

{
  BaseFormPtr  bfp;
  Boolean      dofeats;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  WatchCursor ();
  Update ();

  if (makeFeats) {
    FreeCDDRegions (sep);
  } else {
    FreeCDDAligns (sep);
  }

  dofeats = makeFeats;
  VisitBioseqsInSep (sep, (Pointer) &dofeats, SearchCDD);

  /*
  RemoveDuplicateCDDs (sep);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  */
  ArrowCursor ();
  Update ();
 }

void SimpleCDDSearchFeatProc (IteM i)

{
  SimpleCDDSearchCommonProc (i, TRUE);
}

void SimpleCDDSearchAlignProc (IteM i)

{
  SimpleCDDSearchCommonProc (i, FALSE);
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
  if (remaining < vsquerynum) {
    vsquerynum = remaining;
    Message (MSG_POST, "There are %d vector screens remaining", (int) vsquerynum);
  }
  remaining = QBlastCheckQueue (&qbquerylist);
  remaining = CddCheckQueue (&cddquerylist);
  if (remaining < cddquerynum) {
    cddquerynum = remaining;
    Message (MSG_POST, "There are %d cdd searches remaining", (int) cddquerynum);
  }
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
  b = DefaultButton (c, "Accept", AcceptArgumentFormProc);
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
  Boolean       demomode = FALSE;
  Int2          format = 1;
  FILE          *fp;
  Boolean       goOn = TRUE;
  Char          host [128];
  Boolean       keepGoing;
  ValNodePtr    list = NULL;
  Boolean       nucOK = FALSE;
  Boolean       onlyBspTarget = FALSE;
  ValNodePtr    paramlist = NULL;
  Char          program [128];
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
  while (FileGets (str, sizeof (str), fp) != NULL) {
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
    if (useEntrez) {
      AddAnalysisItem (m, bfp, bspviewOK, FALSE, TRUE, FALSE, TRUE,
                       NULL, 0, NULL, NULL, 0, 0, FALSE, NULL, NULL, NULL, NULL,
                       "Vector Screen - UniVec", "Search",
                       SimpleUniVecScreenProc, &first);
      AddAnalysisItem (m, bfp, bspviewOK, FALSE, TRUE, FALSE, TRUE,
                       NULL, 0, NULL, NULL, 0, 0, FALSE, NULL, NULL, NULL, NULL,
                       "Vector Screen - UniVec Core", "Search",
                       SimpleUniVecCoreScreenProc, &first);
    }
    if (indexerVersion) {
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
   AlnMgr2GetNthSeqRangeInSA(sap, begin, &start1, &stop1);
   AlnMgr2GetNthSeqRangeInSA(sap, fin, &start2, &stop2);
   /* check to see whether the position is outside the alignment */
   if (pos < start1)
      return (start2 - (start1 - pos));
   else if (pos > stop1)
      return (stop2 + (pos-stop1));
   sapos = AlnMgr2MapBioseqToSeqAlign(sap, pos, begin);
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

static void ListPhrapGraphsCallback (SeqGraphPtr sgp, Pointer userdata)
{
  ValNodePtr PNTR vnpp;
  
  if (sgp == NULL || userdata == NULL) return;
  if (StringICmp (sgp->title, "Phrap Quality") == 0)
  {
    vnpp = (ValNodePtr PNTR) userdata;
    ValNodeAddPointer (vnpp, 0, sgp);
  }
}

/* THOUGHTS:
 * Can we/must we update quality scores before/after the old Bioseq has been replaced?
 * If we replace quality scores after the bioseq has been replaced, the oldbsp->length
 * is the length of the buffer we need to hold the quality scores,
 * otherwise use the newbsp->length.
 * Useful functions:
  aln_len = AlnMgr2GetAlnLength(salp, FALSE);

NLM_EXTERN Int4 AlnMgr2GetNumAlnBlocks(SeqAlignPtr sap)
NLM_EXTERN Boolean AlnMgr2GetNthBlockRange(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop)
  
  
 * Assumptions: data replacement has already taken place, oldbsp is in row 1 of salp,
 *              newbsp is in row 2 of salp.
 
 */
static Boolean 
ReplaceQualityScores
(BioseqPtr   oldbsp,
 BioseqPtr   newbsp,
 SeqAlignPtr salp,
 FILE        *log_fp,
 BoolPtr     data_in_log)
{
  ValNodePtr    oldhead = NULL, newhead = NULL, vnp;
  BytePtr       new_store = NULL;  
  Int4          cur_pos;
  SeqGraphPtr   sgp, sgp_list = NULL, last_sgp = NULL;
  ByteStorePtr  bs;
  Int4          len;
  Int4          graph_left, i, graph_len;
  SeqIntPtr     sintp;
  SeqAnnotPtr   sap, last_sap;
  Int2          min, max;
  Char          acc_str [256];
  
  if (oldbsp == NULL || newbsp == NULL || salp == NULL
      || !ISA_na (oldbsp->mol) || !ISA_na (newbsp->mol) || oldbsp->length >= MAXALLOC)
  {
    return FALSE;
  }
  
  VisitGraphsOnBsp (oldbsp, &oldhead, ListPhrapGraphsCallback);
  VisitGraphsOnBsp (newbsp, &newhead, ListPhrapGraphsCallback);
  
  /* prepare buffer to hold scores from both */
  len = oldbsp->length;
  new_store = MemNew (sizeof (Byte) * (len + 2));
  if (new_store == NULL) 
  {
    oldhead = ValNodeFreeData (oldhead);
    newhead = ValNodeFreeData (newhead);
    return FALSE;
  }
  /* init byte store */
  for (cur_pos = 0; cur_pos < len; cur_pos ++)
  {
    new_store [cur_pos] = 255;
  }

  /* copy in old scores */
  for (vnp = oldhead; vnp != NULL; vnp = vnp->next) 
  {
    sgp = vnp->data.ptrvalue;
    if (sgp != NULL)
    {
      sgp->idx.deleteme = TRUE;
    }
  }

  if (oldhead != NULL && newhead == NULL && log_fp != NULL && data_in_log != NULL)
  {
    SeqIdWrite (newbsp->id, acc_str, PRINTID_REPORT, sizeof (acc_str));
    fprintf (log_fp, "Quality scores cleared for %s\n", acc_str);
    *data_in_log = TRUE;
  }
  oldhead = ValNodeFree (oldhead);
  
  /* now copy in new scores */
  for (vnp = newhead; vnp != NULL; vnp = vnp->next) {
    sgp = vnp->data.ptrvalue;
    bs = (ByteStorePtr) sgp->values;
    BSSeek (bs, 0, SEEK_SET);
    cur_pos = GetOffsetInBioseq (sgp->loc, newbsp, SEQLOC_LEFT_END);
    for (i = 0; i < sgp->numval && cur_pos < len; i++, cur_pos++)
    {
      new_store [cur_pos] = (Byte) BSGetByte (bs);        
    }
  }
  
  newhead = ValNodeFree (newhead);

  /* Now we have a Byte array that contains the quality scores.
   * Time to create graphs from the Byte array.
   */
  i = 0;
  graph_left = -1;
  while (i < len)
  {
    if (new_store [i] == (Byte)-1)
    {
      if (graph_left > -1)
      {
        /* add new SeqGraph to list */
        graph_len = i - graph_left;
        bs = BSNew (1000);
        if (bs != NULL)
        {
          BSSeek (bs, 0, SEEK_SET);
          BSWrite (bs, new_store + graph_left, graph_len);
          sgp = SeqGraphNew ();
          sgp->numval = BSLen (bs);
          BSPutByte (bs, EOF);
          sgp->title = StringSave ("Phrap Quality");
          if (len != sgp->numval) {
            sgp->flags [0] = 1;
            sgp->compr = (len) / sgp->numval;
          } else {
            sgp->flags [0] = 0;
            sgp->compr = 1;
          }
          sgp->flags [1] = 0;
          sgp->flags [2] = 3;
          sgp->axis.intvalue = 0;
          sgp->min.intvalue = min;
          sgp->max.intvalue = max;
          sgp->a = 1.0;
          sgp->b = 0;
          sgp->values = (Pointer) bs;

          sintp = SeqIntNew ();
          sintp->from = 0;
          sintp->to = len - 1;
          sintp->id = SeqIdDup (oldbsp->id);
          ValNodeAddPointer (&(sgp->loc), SEQLOC_INT, (Pointer) sintp);
          if (last_sgp == NULL)
          {
            sgp_list = sgp;
          }
          else
          {
            last_sgp->next = sgp;
          }
          last_sgp = sgp;
        }
        graph_left = -1;
      }
    }
    else
    {
      if (graph_left == -1)
      {
        graph_left = i;
        min = new_store [i];
        max = min;
      }
      else
      {
        min = MIN (min, new_store[i]);
        max = MAX (max, new_store[i]);
      }
    }
    i++;
  }
  if (graph_left > -1)
  {
    /* add new SeqGraph to list */
    graph_len = i - graph_left;
    bs = BSNew (1000);
    if (bs != NULL)
    {
      BSSeek (bs, 0, SEEK_SET);
      BSWrite (bs, new_store + graph_left, graph_len);
      sgp = SeqGraphNew ();
      sgp->numval = BSLen (bs);
      BSPutByte (bs, EOF);
      sgp->title = StringSave ("Phrap Quality");
      if (len != sgp->numval) {
        sgp->flags [0] = 1;
        sgp->compr = (len) / sgp->numval;
      } else {
        sgp->flags [0] = 0;
        sgp->compr = 1;
      }
      sgp->flags [1] = 0;
      sgp->flags [2] = 3;
      sgp->axis.intvalue = 0;
      sgp->min.intvalue = min;
      sgp->max.intvalue = max;
      sgp->a = 1.0;
      sgp->b = 0;
      sgp->values = (Pointer) bs;

      sintp = SeqIntNew ();
      sintp->from = 0;
      sintp->to = len - 1;
      sintp->id = SeqIdDup (oldbsp->id);
      ValNodeAddPointer (&(sgp->loc), SEQLOC_INT, (Pointer) sintp);
      if (last_sgp == NULL)
      {
        sgp_list = sgp;
      }
      else
      {
        last_sgp->next = sgp;
      }
      last_sgp = sgp;
    }
  }
  
  if (sgp_list != NULL)
  {
    /* now add in new phrap annotations */
    last_sap = NULL;
    last_sgp = NULL;
    for (sap = oldbsp->annot; sap != NULL && sap->type !=3; sap = sap->next) 
    {
      last_sap = sap;
    }
    
    if (sap == NULL)
    {
      sap = SeqAnnotNew ();
      sap->type = 3;
      sap->data = sgp_list;
      if (last_sap == NULL)
      {
        oldbsp->annot = sap;
      }
      else
      {
        last_sap->next = sap;
      }
    }
    else
    {
      for (sgp = (SeqGraphPtr) sap->data; sgp != NULL; sgp = sgp->next) 
      {
        last_sgp = sgp;  
      }
      if (last_sgp == NULL)
      {
        sap->data = sgp_list;
      }
      else
      {
        last_sgp->next = sgp_list;
      }
    }
    
  }

  /* now remove old phrap annotations */
  DeleteMarkedObjects (0, OBJ_BIOSEQ, (Pointer) oldbsp);
  return TRUE;
}


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

  AlnMgr2IndexSingleChildSeqAlign (sap);

  return TRUE;
}

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

extern void OffsetLocation (SeqLocPtr loc, Int4 offset, SeqIdPtr sip)

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
  SeqAnnotPtr PNTR sapp,
  Boolean patch
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

    if ((! patch) || (context.right >= udp->new5 && context.left <= udp->new5 + udp->newa)) {

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
  Boolean            partial5, partial3;

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
        CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
        SetSeqLocPartial (newloc, partial5, partial3);
        OffsetLocation (newloc, from, NULL);
        dup = AsnIoMemCopy ((Pointer) sfp,
                            (AsnReadFunc) SeqFeatAsnRead,
                            (AsnWriteFunc) SeqFeatAsnWrite);
        SeqLocFree (dup->location);
        dup->location = newloc;
        if (split) {
          dup->partial = TRUE;
        }
        dup->partial |= partial5;
        dup->partial |= partial3;

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

static void 
ReplaceOneSequence 
(SeqAlignPtr salp,
 BioseqPtr oldbsp,
 BioseqPtr newbsp)
{
  SeqMgrFeatContext  context;
  SeqFeatPtr         sfp;
  SeqLocPtr          slp;
  ByteStorePtr       bs;
  CodeBreakPtr       cbp;
  CdRegionPtr        crp;
  Int4               len;
  RnaRefPtr          rrp;
  Uint1              seq_data_type, seq_ext_type;
  Pointer            seq_ext;
  tRNAPtr            trp;
  Uint1              repr;
  
  if (oldbsp == NULL || newbsp == NULL)
  {
    return;
  }

  if (salp != NULL)
  {
    sfp = SeqMgrGetNextFeature (oldbsp, NULL, 0, 0, &context);
    while (sfp != NULL) {
      slp = SeqLocFindNext (sfp->location, NULL);
      while (slp != NULL) {
        ReplaceLocation (salp, slp, newbsp->length, 1, 2);
        slp = SeqLocFindNext (sfp->location, slp);
      }
      switch (sfp->data.choice) {
        case SEQFEAT_CDREGION :
          crp = (CdRegionPtr) sfp->data.value.ptrvalue;
          if (crp != NULL) 
          {
            for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) 
            {
              slp = SeqLocFindNext (cbp->loc, NULL);
              while (slp != NULL) 
              {
                ReplaceLocation (salp, slp, newbsp->length, 1, 2);
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
                ReplaceLocation (salp, slp, newbsp->length, 1, 2);
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
  }

  /* switch bioseqs to finish update */

  bs = oldbsp->seq_data;
  oldbsp->seq_data = newbsp->seq_data;
  newbsp->seq_data = bs;
  len = oldbsp->length;
  oldbsp->length = newbsp->length;
  newbsp->length = len;
  seq_data_type = oldbsp->seq_data_type;
  oldbsp->seq_data_type = newbsp->seq_data_type;
  newbsp->seq_data_type = seq_data_type;  
  /* also move seq_ext, for delta sequences */
  seq_ext_type = oldbsp->seq_ext_type;
  seq_ext = oldbsp->seq_ext;
  oldbsp->seq_ext_type = newbsp->seq_ext_type;
  oldbsp->seq_ext = newbsp->seq_ext;
  newbsp->seq_ext_type = seq_ext_type;
  newbsp->seq_ext = seq_ext;
  
  /* swap repr */
  repr = oldbsp->repr;
  oldbsp->repr = newbsp->repr;
  newbsp->repr = repr;
    
}

static Boolean ReplaceSequence (UpsDataPtr udp)

{
  MsgAnswer          ans;

  if (udp == NULL)
  {
    return TRUE;
  }
  
  if (FALSE == udp->isSet)
  {
    if ((udp->seq1 != NULL || udp->seq2 != NULL)
        && StringICmp (udp->seq1, udp->seq2) == 0
        && ! udp->revcomp) 
    {
	    ans = Message (MSG_OKC, "Replacement sequence is identical to"
	                           " original - possible error");
	    if (ans == ANS_CANCEL) return FALSE;
    }    
  }

  ReplaceOneSequence (udp->salp, udp->oldbsp, udp->newbsp);
  return TRUE;
}

static Boolean Merge5Prime (UpsDataPtr udp)

{
  ByteStorePtr  bs;
  Char          ch;
  Int4          i, newlen;
  BioseqPtr     newbsp;
  CharPtr       ptr, str, tmp;

  /* construct replacement sequence by recombining between old and overlap */

  tmp = udp->seq2;

  newlen = udp->new5 + udp->newa + udp->old3;
  str = (CharPtr) MemNew (sizeof (Char) * (size_t) (newlen + 5));
  if (str == NULL)
    return FALSE;
  ptr = str;

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

  /* construct replacement sequence by recombining between old and overlap */

  newlen = udp->old5 + udp->newa + udp->new3;
  str = (CharPtr) MemNew (sizeof (Char) * (size_t) (newlen + 5));
  if (str == NULL)
    return FALSE;
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

static Boolean ExtendFeatures (UpsDataPtr udp, Int4 offset);

/*------------------------------------------------------------------*/
/*                                                                  */
/*  Merge5PrimeNoOverlap () -- Merge a new sequence onto the 5' end */
/*                             of an existing sequence.             */
/*                                                                  */
/*                             Performs a similar function to       */
/*                             Merge5Prime() except works when      */
/*                             there is no alignment between the    */
/*                             two sequences.                       */
/*                                                                  */
/*------------------------------------------------------------------*/

static Boolean Merge5PrimeNoOverlap (UpsDataPtr udp)

{
  CharPtr       origSeqStr;
  CharPtr       newSeqStr;
  CharPtr       mergedSeqStr;
  Int4          mergedLen;
  ByteStorePtr  mergedBS;

  /* Get original and new sequences */

  origSeqStr = GetSequenceByBsp (udp->oldbsp);
  newSeqStr = GetSequenceByBsp (udp->newbsp);

  /* Concatenate the new sequence onto the beginning */
  /* (i.e. the 5' end) of the original sequence.     */

  mergedLen =  StringLen (newSeqStr) + StringLen (origSeqStr);
  mergedSeqStr = (CharPtr) MemNew (mergedLen + 1);
  sprintf (mergedSeqStr, "%s%s", newSeqStr, origSeqStr);

  /* Convert the new sequence into a ByteStore */

  mergedBS = BSNew (mergedLen);
  BSWrite (mergedBS, (VoidPtr) mergedSeqStr, mergedLen);

  /* Replace the original sequence with the */
  /* new concatenated sequence.             */

  udp->newbsp->seq_data      = BSFree (udp->newbsp->seq_data);
  udp->newbsp->seq_data      = mergedBS;
  udp->newbsp->seq_data_type = Seq_code_iupacna;
  udp->newbsp->length        = mergedLen;

  /* Replace the merged sequence and return */

  return ExtendFeatures (udp, StringLen (newSeqStr));
}

/*------------------------------------------------------------------*/
/*                                                                  */
/*  Merge3PrimeNoOverlap () -- Merge a new sequence onto the 3' end */
/*                             of an existing sequence.             */
/*                                                                  */
/*                             Performs a similar function to       */
/*                             Merge3Prime() except works when      */
/*                             there is no alignment between the    */
/*                             two sequences.                       */
/*                                                                  */
/*------------------------------------------------------------------*/

static Boolean Merge3PrimeNoOverlap (UpsDataPtr udp)

{
  CharPtr       origSeqStr;
  CharPtr       newSeqStr;
  CharPtr       mergedSeqStr;
  Int4          mergedLen;
  ByteStorePtr  mergedBS;

  /* Get original and new sequences */

  origSeqStr = GetSequenceByBsp (udp->oldbsp);
  newSeqStr = GetSequenceByBsp (udp->newbsp);

  /* Concatenate the new sequence onto the end   */
  /* (i.e. the 3' end) of the original sequence. */

  mergedLen =  StringLen (newSeqStr) + StringLen (origSeqStr);
  mergedSeqStr = (CharPtr) MemNew (mergedLen + 1);
  sprintf (mergedSeqStr, "%s%s", origSeqStr, newSeqStr);

  /* Convert the new sequence into a ByteStore */

  mergedBS = BSNew (mergedLen);
  BSWrite (mergedBS, (VoidPtr) mergedSeqStr, mergedLen);

  /* Replace the original sequence with the */
  /* new concatenated sequence.             */

  udp->newbsp->seq_data      = BSFree (udp->newbsp->seq_data);
  udp->newbsp->seq_data      = mergedBS;
  udp->newbsp->seq_data_type = Seq_code_iupacna;
  udp->newbsp->length        = mergedLen;

  /* Replace the merged sequence and return */

  return ExtendFeatures (udp, 0);
}

static Boolean OkToPatchDelta (UpsDataPtr udp)
{
  Boolean rval = TRUE;
  
  if (udp == NULL || udp->oldbsp == NULL || udp->newbsp == NULL
      || udp->oldbsp->repr != Seq_repr_delta || udp->newbsp->repr != Seq_repr_delta
      || udp->oldbsp->seq_ext_type != 4 || udp->newbsp->seq_ext_type != 4)
  {
    rval = FALSE;
  }

  return rval;
}

static void SplitDeltaSeq (DeltaSeqPtr dsp, Int4 offset)
{
  SeqLocPtr   slp1, slp2;
  SeqLitPtr   slip1, slip2;
  Int4        len;
  Boolean     changed;
  DeltaSeqPtr dsp_new;
  ByteStorePtr bs_1, bs_2;
  Int2         residue;
  Int4         pos;
  
  if (dsp == NULL || dsp->data.ptrvalue == NULL || offset == 0)
  {
    return;
  }
  
  if (dsp->choice == 1)
  {
    slp1 = (SeqLocPtr)(dsp->data.ptrvalue);
    len = SeqLocLen (slp1);
    if (offset > len)
    {
      return;
    }
    slp2 = (SeqLocPtr) AsnIoMemCopy (slp1, (AsnReadFunc) SeqLocAsnRead,
                                     (AsnWriteFunc) SeqLocAsnWrite);
    slp1 = SeqLocDelete (slp1, SeqLocId (slp1), 
                          offset, len - 1, FALSE, &changed);
    slp2 = SeqLocDelete (slp2, SeqLocId (slp2),
                         0, offset, FALSE, &changed);
    dsp_new = ValNodeNew (NULL);
    dsp_new->choice = 1;
    dsp_new->data.ptrvalue = slp2;
    dsp_new->next = dsp->next;
    dsp->next = dsp_new;
  }
  else if (dsp->choice == 2)
  {
    slip1 = (SeqLitPtr) dsp->data.ptrvalue;
    if (offset > slip1->length)
    {
      return;
    }
    slip2 = SeqLitNew ();
    if (slip1->seq_data == NULL)
    {
      slip2->length = slip1->length - offset;
      slip1->length = offset;
    }
    else
    {
      if (slip1->seq_data_type == Seq_code_iupacna)
      {
        bs_1 = slip1->seq_data;
      }
      else
      {
        bs_1 = BSConvertSeq(slip1->seq_data, Seq_code_iupacna, 
                              slip1->seq_data_type, 
                              slip1->length);
        slip1->seq_data_type = Seq_code_iupacna;
        slip1->seq_data = bs_1;
      }
      bs_2 = BSNew (slip1->length - offset);
      pos = offset;
      BSSeek(bs_1, pos, SEEK_SET);
      BSSeek (bs_2, 0L, SEEK_SET);
      while (pos < slip1->length)
      {
        residue = BSGetByte (bs_1);
        BSPutByte (bs_2, residue);
        pos++;
      }
      BSSeek(bs_1, offset, SEEK_SET);
      BSDelete(bs_1, slip1->length - offset);
      
      slip2->seq_data = bs_2;
      slip2->seq_data_type = slip1->seq_data_type;
      slip2->length = slip1->length - offset;
      slip1->length = offset;
    }
    dsp_new = ValNodeNew (NULL);
    dsp_new->choice = 2;
    dsp_new->data.ptrvalue = slip2;
    dsp_new->next = dsp->next;
    dsp->next = dsp_new;
  }
}

/* This function will patch a delta sequence with another delta sequence.
 * The pieces in the overlap from the old sequence will be replaced by pieces
 * in the overlap from the new sequence.
 */
static Boolean PatchDeltaSequence (UpsDataPtr udp)

{
  Int4        currnew_pos = 0, currold_pos;
  SeqLitPtr   slip, slip_new;
  DeltaSeqPtr dspold, dspnew;
  Int4        seqstart;
  DeltaSeqPtr new_list = NULL;
  
  if (! OkToPatchDelta (udp))
  {
    return FALSE;
  }

  /* keep old 5' end intact */
  currold_pos = 0;
  seqstart = 0;
  dspold = (DeltaSeqPtr) udp->oldbsp->seq_ext;
  while (dspold != NULL && currold_pos < udp->old5)
  {
    seqstart = currold_pos;
    if (dspold->data.ptrvalue == NULL || dspold->choice != 2)
    {
      return FALSE;
    }
    slip = (SeqLitPtr) (dspold->data.ptrvalue);
	  currold_pos += slip->length;
		if (currold_pos > udp->old5)
		{
      SplitDeltaSeq (dspold, udp->old5 - seqstart);
      slip = (SeqLitPtr) (dspold->data.ptrvalue);
      currold_pos = udp->old5;		  
		}
		slip_new = (SeqLitPtr) AsnIoMemCopy (slip, (AsnReadFunc) SeqLitAsnRead,
		                                     (AsnWriteFunc) SeqLitAsnWrite);
		ValNodeAddPointer (&new_list, 2, slip_new);
	  dspold = dspold->next;
  }
  
  /* skip over new 5' end */
  currnew_pos = 0;
  seqstart = 0;
  dspnew = (DeltaSeqPtr) udp->newbsp->seq_ext;
  while (dspnew != NULL && currnew_pos < udp->new5)
  {
    seqstart = currold_pos;
    if (dspnew->data.ptrvalue == NULL || dspnew->choice != 2)
    {
      return FALSE;
    }
	  slip = (SeqLitPtr) (dspnew->data.ptrvalue);
	  currnew_pos += slip->length;
	  if (currnew_pos > udp->new5)
	  {
      SplitDeltaSeq (dspnew, udp->new5 - seqstart);
      currnew_pos = udp->new5;
	  }
	  dspnew = dspnew->next;
  }
  
  /* copy in new overlap */
  while (dspnew != NULL && currnew_pos < udp->new5 + udp->newa)
  {
    seqstart = currold_pos;
    if (dspnew->data.ptrvalue == NULL || dspnew->choice != 2)
    {
      return FALSE;
    }
	  slip = (SeqLitPtr) (dspnew->data.ptrvalue);
	  currnew_pos += slip->length;
		if (currnew_pos > udp->new5 + udp->newa)
		{
      SplitDeltaSeq (dspnew, udp->new5 + udp->newa - seqstart);
      slip = (SeqLitPtr) (dspnew->data.ptrvalue);
      currnew_pos = udp->new5 + udp->newa;		  
		}
		slip_new = (SeqLitPtr) AsnIoMemCopy (slip, (AsnReadFunc) SeqLitAsnRead,
		                                     (AsnWriteFunc) SeqLitAsnWrite);
		ValNodeAddPointer (&new_list, 2, slip_new);
		dspnew = dspnew->next;
  }
  
  /* skip over old overlap */
  
  while (dspold != NULL && currold_pos < udp->old5 + udp->olda)
  {
    seqstart = currold_pos;
    if (dspold->data.ptrvalue == NULL || dspold->choice != 2)
    {
      return FALSE;
    }
    slip = (SeqLitPtr) (dspold->data.ptrvalue);
    currold_pos += slip->length;
    if (currold_pos > udp->old5 + udp->olda)
    {
      SplitDeltaSeq (dspold, udp->new5 + udp->newa - seqstart);
      currold_pos = udp->old5 + udp->olda;		        
    }
    dspold = dspold->next;
  }
  
  /* copy in old 3' */
  
  while (dspold != NULL)
  {
    if (dspold->data.ptrvalue == NULL || dspold->choice != 2)
    {
      return FALSE;
    }
    slip = (SeqLitPtr) (dspold->data.ptrvalue);
		slip_new = (SeqLitPtr) AsnIoMemCopy (slip, (AsnReadFunc) SeqLitAsnRead,
		                                     (AsnWriteFunc) SeqLitAsnWrite);
		ValNodeAddPointer (&new_list, 2, slip_new);
		dspold = dspold->next;
  }
  
  /* free newbsp's old SeqLit List */
  for (dspnew = (DeltaSeqPtr) udp->newbsp->seq_ext;
       dspnew != NULL; 
       dspnew = dspnew->next)
  {
    slip = (SeqLitPtr) (dspnew->data.ptrvalue);
    SeqLitFree (slip);
  }
  udp->newbsp->seq_ext = ValNodeFree (udp->newbsp->seq_ext);
  udp->newbsp->seq_ext = new_list;
  udp->newbsp->length = udp->old5 + udp->newa + udp->old3;
  return TRUE;  
}

static Boolean OkToPatchRaw (UpsDataPtr udp)
{
  Boolean rval = TRUE;
  
  if (udp == NULL || udp->oldbsp == NULL || udp->newbsp == NULL
      || udp->oldbsp->repr != Seq_repr_raw || udp->newbsp->repr != Seq_repr_raw)
  {
    rval = FALSE;
  }

  return rval;
}

static Boolean PatchRawSequence (UpsDataPtr udp)

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
  return TRUE;  
}

static Boolean PatchSequence (UpsDataPtr udp)

{
  Boolean rval = FALSE;

  if (OkToPatchRaw (udp))
  {
    rval = PatchRawSequence (udp);
  }
  else if (OkToPatchDelta (udp))
  {
    rval = PatchDeltaSequence (udp);
  }
  
  if (!rval)
  {
    return rval;
  }

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
      CombineTexts (&(grp->locus_tag), &(oldgrp->locus_tag));
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

static void RemoveOldFeatsInRegion (
  UpsDataPtr udp,
  BioseqPtr bsp,
  SeqAnnotPtr sap
)

{
  SeqMgrFeatContext  context;
  Int4               left, right;
  SeqFeatPtr         sfp;

  if (udp == NULL || bsp == NULL || sap == NULL) return;
  if (sap->type != 1) return;

  left = INT4_MAX;
  right = INT4_MIN;

  for (sfp = (SeqFeatPtr) sap->data; sfp != NULL; sfp = sfp->next) {
    if (sfp != SeqMgrGetDesiredFeature (0, bsp, 0, 0, sfp, &context)) continue;
    left = MIN (left, context.left);
    right = MAX (right, context.right);
  }

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &context);

  while (sfp != NULL) {

    if (context.sap != sap && context.right >= left && context.left <= right) {
      sfp->idx.deleteme = TRUE;
      MarkProductForDeletion (sfp->product);
    }

    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &context);
  }
}

static void RemoveOldFeats (BioseqPtr bsp)

{
  SeqMgrFeatContext  context;
  SeqFeatPtr         sfp;

  if (bsp == NULL) return;

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &context);

  while (sfp != NULL) 
  {
    sfp->idx.deleteme = TRUE;
    MarkProductForDeletion (sfp->product);
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &context);
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
  if (nobmval == UPDATE_FEAT_DUP_USE_BOTH) return; /* keep both */

  SeqMgrIndexFeatures (0, (Pointer) bsp);

  if (nobmval == UPDATE_FEAT_DUP_REPLACE) {
    RemoveOldFeatsInRegion (udp, bsp, sap);
    return;
  }

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

          if (nobmval == UPDATE_FEAT_DUP_USE_NEW) { /* keep new */
            if (context.sap == sap) {
              lastsfp->idx.deleteme = TRUE;
              MarkProductForDeletion (lastsfp->product);
            } else if (lastcontext.sap == sap) {
              sfp->idx.deleteme = TRUE;
              MarkProductForDeletion (sfp->product);
            }

          } else if (nobmval == UPDATE_FEAT_DUP_USE_OLD) { /* keep old */
            if (context.sap == sap) {
              sfp->idx.deleteme = TRUE;
              MarkProductForDeletion (sfp->product);
            } else if (lastcontext.sap == sap) {
              lastsfp->idx.deleteme = TRUE;
              MarkProductForDeletion (lastsfp->product);
            }

          } else if (nobmval == UPDATE_FEAT_DUP_MERGE) { /* merge */
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

extern void AddCitSubToUpdatedSequence (BioseqPtr upd_bsp, Uint2 input_entityID)
{
  SeqEntryPtr top_sep, upd_sep;

  upd_sep = SeqMgrGetSeqEntryForData (upd_bsp);
  if (upd_sep == NULL) return;
  top_sep = GetTopSeqEntryForEntityID ( input_entityID);
  if (top_sep == NULL) return;
  CreateUpdateCitSubFromBestTemplate (top_sep, upd_sep);
}

static Boolean ExtendFeatures (UpsDataPtr udp, Int4 offset)

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
  SeqIdPtr           sip;
  tRNAPtr            trp;

  if (udp->salp != NULL)
    if (FALSE == udp->isSet)
      if (StringICmp (udp->seq1, udp->seq2) == 0) {
	ans = Message (MSG_OKC, "Replacement sequence is identical to"
		       " original - possible error");
	if (ans == ANS_CANCEL) return FALSE;
      }

  oldbsp = udp->oldbsp;
  newbsp = udp->newbsp;

  sip = SeqIdFindBest (oldbsp->id, 0);
  if (sip == NULL) return FALSE;

  if (offset > 0) {
    sfp = SeqMgrGetNextFeature (oldbsp, NULL, 0, 0, &context);
    while (sfp != NULL) {
      OffsetLocation (sfp->location, offset, sip);
      switch (sfp->data.choice) {
        case SEQFEAT_CDREGION :
          crp = (CdRegionPtr) sfp->data.value.ptrvalue;
          if (crp != NULL) {
            for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
              OffsetLocation (cbp->loc, offset, sip);
            }
          }
          break;
        case SEQFEAT_RNA :
          rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
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
      sfp = SeqMgrGetNextFeature (oldbsp, sfp, 0, 0, &context);
    }
  }

  /* switch bioseqs to finish extension */

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

static Boolean ExtendBothEnds (UpsDataPtr udp)

{
  ByteStorePtr  bs;
  Char          ch;
  Int4          i, newlen;
  BioseqPtr     newbsp;
  BioseqPtr     oldbsp;
  CharPtr       ptr, str, tmp;

  /* construct replacement sequence by extending old sequence */

  newlen = udp->new5 + udp->olda + udp->new3;
  str = (CharPtr) MemNew (sizeof (Char) * (size_t) (newlen + 5));
  if (str == NULL)
    return FALSE;
  ptr = str;

  tmp = udp->seq2;
  for (i = 0; i < udp->new5; i++) {
    ch = *tmp;
    *ptr = ch;
    tmp++;
    ptr++;
  }
    
  tmp = udp->seq1 + udp->old5;
  for (i = 0; i < udp->olda; i++) {
    ch = *tmp;
    *ptr = ch;
    tmp++;
    ptr++;
  }

  tmp = udp->seq2 + udp->new5 + udp->newa;
  for (i = 0; i < udp->new3; i++) {
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

  oldbsp = udp->oldbsp;
  newbsp = udp->newbsp;
  newbsp->seq_data = BSFree (newbsp->seq_data);
  newbsp->seq_data = bs;
  newbsp->seq_data_type = Seq_code_iupacna;
  newbsp->length = newlen;

  /* then finish by offsetting features */

  return ExtendFeatures (udp, udp->new5);
}

static Boolean Extend5Prime (UpsDataPtr udp)

{
  ByteStorePtr  bs;
  Char          ch;
  Int4          i, newlen;
  BioseqPtr     newbsp;
  BioseqPtr     oldbsp;
  CharPtr       ptr, str, tmp;

  /* construct replacement sequence by extending old sequence */

  newlen = udp->new5 + udp->olda + udp->old3;
  str = (CharPtr) MemNew (sizeof (Char) * (size_t) (newlen + 5));
  if (str == NULL)
    return FALSE;
  ptr = str;

  tmp = udp->seq2;
  for (i = 0; i < udp->new5; i++) {
    ch = *tmp;
    *ptr = ch;
    tmp++;
    ptr++;
  }
    
  tmp = udp->seq1 + udp->old5;
  for (i = 0; i < udp->olda + udp->old3; i++) {
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

  oldbsp = udp->oldbsp;
  newbsp = udp->newbsp;
  newbsp->seq_data = BSFree (newbsp->seq_data);
  newbsp->seq_data = bs;
  newbsp->seq_data_type = Seq_code_iupacna;
  newbsp->length = newlen;

  /* then finish by offsetting features */

  return ExtendFeatures (udp, udp->new5);
}

static Boolean Extend3Prime (UpsDataPtr udp)

{
  ByteStorePtr  bs;
  Char          ch;
  Int4          i, newlen;
  BioseqPtr     newbsp;
  BioseqPtr     oldbsp;
  CharPtr       ptr, str, tmp;

  /* construct replacement sequence by extending old sequence */

  newlen = udp->old5 + udp->olda + udp->new3;
  str = (CharPtr) MemNew (sizeof (Char) * (size_t) (newlen + 5));
  if (str == NULL)
    return FALSE;
  ptr = str;

  tmp = udp->seq1;
  for (i = 0; i < udp->old5 + udp->olda; i++) {
    ch = *tmp;
    *ptr = ch;
    tmp++;
    ptr++;
  }
    
  tmp = udp->seq2 + udp->new5 + udp->newa;
  for (i = 0; i < udp->new3; i++) {
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

  oldbsp = udp->oldbsp;
  newbsp = udp->newbsp;
  newbsp->seq_data = BSFree (newbsp->seq_data);
  newbsp->seq_data = bs;
  newbsp->seq_data_type = Seq_code_iupacna;
  newbsp->length = newlen;

  /* no offset, but ExtendFeatures finishes switch */

  return ExtendFeatures (udp, 0);
}

static Boolean ExtendOneSequence (UpsDataPtr udp)
{
  Boolean      update = FALSE;
  MsgAnswer    ans;
  Uint2        entityID;
  CharPtr      errmsg = NULL;
  
  
  if (udp == NULL) return FALSE;
  
  switch (udp->rmcval) {
    case UPDATE_REPLACE :
      if (udp->old5 > 0 && udp->old3 > 0) {
        errmsg = "Unaligned sequence at 5' and 3' ends.  Do you wish to proceed?";
      } else if (udp->old5 > 0) {
        errmsg = "Unaligned sequence at 5' end.  Do you wish to proceed?";
      } else if (udp->old3 > 0) {
        errmsg = "Unaligned sequence at 3' end.  Do you wish to proceed?";
      }
      break;
    case UPDATE_EXTEND5 :
      if (udp->old5 > 0) {
        errmsg = "Unaligned sequence at 5' end.  Do you wish to proceed?";
      }
      break;
    case UPDATE_EXTEND3 :
      if (udp->old3 > 0) {
        errmsg = "Unaligned sequence at 3' end.  Do you wish to proceed?";
      }
      break;
    default :
      break;
  }
  if (errmsg != NULL) {
    ans = Message (MSG_YN, "%s", errmsg);
    if (ans == ANS_NO) {
      return FALSE;
    }
  }

  switch (udp->rmcval) {
    case UPDATE_REPLACE :
      if (ExtendBothEnds (udp)) {
        update = TRUE;
      }
      break;
    case UPDATE_EXTEND5:
      if (udp->salp == NULL) {
        if (Merge5PrimeNoOverlap (udp)) {
          update = TRUE;
        }
      } else if (Extend5Prime (udp)) {
        update = TRUE;
      }
      break;
    case UPDATE_EXTEND3 :
      if (udp->salp == NULL) {
        if (Merge3PrimeNoOverlap (udp)) {
          update = TRUE;
        }
      } else if (Extend3Prime (udp)) {
        update = TRUE;
      }
      break;
    default :
      break;
  }

  if (update) {
      
    entityID = ObjMgrGetEntityIDForPointer (udp->oldbsp);
    if (GetStatus (udp->add_cit_subs))
    {
      AddCitSubToUpdatedSequence ( udp->oldbsp, entityID);
    }
  }
  return update;
}

static void OpenSequenceUpdateLog (UpsDataPtr udp)
{
  if (udp == NULL || udp->log_fp != NULL)
  {
  	return;
  }
  TmpNam (udp->log_path);
  udp->log_fp = FileOpen (udp->log_path, "wb");
}

static void CloseOutSequenceUpdateLog (UpsDataPtr udp)
{
  if (udp == NULL || udp->log_fp == NULL) 
  {
    return;
  }
  FileClose (udp->log_fp);
  udp->log_fp = NULL;
  if (udp->data_in_log) {
    LaunchGeneralTextViewer (udp->log_path, "Protein changes");
    udp->data_in_log = FALSE;
  }
  FileRemove (udp->log_path);  	
}


static Boolean PrepareToUpdateSequences (UpsDataPtr udp);
static Boolean PrepareUpdatePtr (UpsDataPtr    udp);
static ForM UpdateSequenceForm (UpsDataPtr udp);
static void UpdateOneSequence (
  UpsDataPtr   udp,
  Int2         sfbval,
  Boolean      add_cit_subs,
  Boolean      update_proteins);


static Int4 FindNewCDSStop (SeqLocPtr slp, BioseqPtr bsp, Int4 prot_len)
{
  Int4      loc_len, tot_len = 0;
  SeqLocPtr this_slp;
  Int4      curr_start;
  
  for (this_slp = SeqLocFindNext (slp, NULL);
       this_slp != NULL;
       this_slp = SeqLocFindNext (slp, this_slp))
  {
    loc_len = SeqLocLen (this_slp);
    curr_start = GetOffsetInBioseq (this_slp, bsp, SEQLOC_START);
    if (loc_len + tot_len > prot_len)
    {
      curr_start = GetOffsetInBioseq (this_slp, bsp, SEQLOC_START);
      if (SeqLocStrand (this_slp) == Seq_strand_minus)
      {
        return curr_start - (prot_len - tot_len) + 1;
      }
      else
      {
        return curr_start + prot_len - tot_len;
      }
    }
    tot_len += loc_len;
  }
  return tot_len;
}


static CharPtr FixProteinString
(SeqFeatPtr    sfp,
 Uint1         strand,
 Boolean       truncate_proteins,
 Boolean PNTR  truncated,
 Boolean PNTR  contains_start,
 Boolean PNTR  contains_stop)
{
  ByteStorePtr bs;
  BioseqPtr    nucBsp;
  CharPtr      newprot;
  CharPtr      ptr;
  Char         ch;
  Int4         start, stop, new_stop;
  Boolean      changed;
  
  if (sfp == NULL || truncated == NULL 
      || contains_start == NULL
      || contains_stop == NULL) {
    return NULL;
  }
  
  bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
  if (bs == NULL) return NULL;
  newprot = BSMerge (bs, NULL);
  bs = BSFree (bs);
  if (newprot == NULL) return NULL;

  ptr = newprot;
  ch = *ptr;
  if (ch == 'M') {
    *contains_start = TRUE;
  } else {
    *contains_start = FALSE;
  }
  *contains_stop = FALSE;
  *truncated = FALSE;
  while (ch != '\0')
  {
    *ptr = TO_UPPER (ch);
    if (ch == '*') {
      *contains_stop = TRUE;
      if (*(ptr + 1) == 0 || truncate_proteins) {
        *ptr = 0;
        if (truncate_proteins && *(ptr + 1) != 0) {
          *truncated = TRUE;
          nucBsp = BioseqFindFromSeqLoc (sfp->location);
          if (nucBsp == NULL) return newprot;
          start = GetOffsetInBioseq (sfp->location, nucBsp, SEQLOC_START);
          stop = GetOffsetInBioseq (sfp->location, nucBsp, SEQLOC_STOP);
          new_stop = FindNewCDSStop (sfp->location, nucBsp, (1 + ptr - newprot) * 3);
          if (strand == Seq_strand_minus) {
            sfp->location = SeqLocDelete (sfp->location, SeqLocId (sfp->location),
                          stop, new_stop - 1, FALSE, &changed);
          } else {
            sfp->location = SeqLocDelete (sfp->location, SeqLocId (sfp->location),
                          new_stop + 1, stop, FALSE, &changed);
          }
        }
        return newprot;
      }
    }
    ptr++;
    ch = *ptr;
  }
  return newprot;
}

/* This function will truncate base pairs to make the length of the CDS
 * a multiple of three.  If truncation occurs, the function returns TRUE,
 * otherwise the function returns FALSE.
 */
static SeqLocPtr ShiftStopForLengthChange
(SeqLocPtr slp,
 BioseqPtr nucBsp,
 Int4      transl_except_len,
 Boolean PNTR changed)
{
  Int4      start, stop, new_stop;
  Uint1     strand;
  SeqIdPtr  sip;
  Int4      len;

  if (slp == NULL || nucBsp == NULL || changed == NULL) {
    return NULL;
  }

  *changed = FALSE;

  start = GetOffsetInBioseq (slp, nucBsp, SEQLOC_START);
  stop = GetOffsetInBioseq (slp, nucBsp, SEQLOC_STOP);
  new_stop = stop;
  strand = SeqLocStrand (slp);
  len = SeqLocLen (slp);

  if (strand == Seq_strand_minus) {
    if (len % 3 != transl_except_len) {
      new_stop += len % 3 + transl_except_len;
      sip = SeqLocId (slp);
      slp = SeqLocDelete (slp, sip, stop, new_stop - 1, FALSE, changed);
      if (slp == NULL) {
        return NULL;
      }
    }
  } else {
    if (len % 3 != transl_except_len) {
      new_stop -= len % 3 - transl_except_len;
      sip = SeqLocId (slp);
      slp = SeqLocDelete (slp, sip, new_stop + 1, stop, FALSE, changed);
      if (slp == NULL) {
        return NULL;
      }
    }
  }
  return slp;
}

static SeqLocPtr 
ExpandSeqLoc 
(Int4 start,
 Int4 stop,
 Uint1 strand,
 BioseqPtr bsp,
 SeqLocPtr slp)
{
  Int4      curr_start, curr_stop, tmp_start, tmp_stop;
  SeqLocPtr this_slp;

  if (slp == NULL) return NULL;

  if (slp->choice == SEQLOC_INT || slp->choice == SEQLOC_PNT) {
    slp = expand_seq_loc (start, stop, strand, slp);
  } else {
    curr_start = GetOffsetInBioseq (slp, bsp, SEQLOC_START);
    curr_stop = GetOffsetInBioseq (slp, bsp, SEQLOC_STOP);
    for (this_slp = SeqLocFindNext (slp, NULL);
         this_slp != NULL;
         this_slp = SeqLocFindNext (slp, this_slp))
    {
      tmp_start = GetOffsetInBioseq (this_slp, bsp, SEQLOC_START);
      tmp_stop = GetOffsetInBioseq (this_slp, bsp, SEQLOC_STOP);
      if (strand == Seq_strand_minus) {
        if (tmp_start == curr_start && tmp_start < stop) {
          this_slp = expand_seq_loc (tmp_stop, stop, strand, this_slp);
          tmp_start = stop;
        }
        if (tmp_stop == curr_stop && tmp_stop > start) {
          this_slp = expand_seq_loc (start, tmp_start, strand, this_slp);
        }
      } else {
        if (tmp_start == curr_start && tmp_start > start) {
          this_slp = expand_seq_loc (start, tmp_stop, strand, this_slp);
          tmp_start = start;
        }
        if (tmp_stop == curr_stop && tmp_stop < stop) {
          this_slp = expand_seq_loc (tmp_start, stop, strand, this_slp);
        }
      }
    }
  }
  return slp;
}

static SeqLocPtr ShiftLocationForFrame
(SeqLocPtr slp,
 Uint1 frame,
 BioseqPtr nucBsp)
{
  Int4      max_stop, start, stop;
  Uint1     strand;
  SeqIdPtr  sip;
  Int4      offset, new_start;
  Boolean   changed;
  

  if (slp == NULL || nucBsp == NULL) return NULL;
  if (frame == 0 || frame == 1) return slp;

  start = GetOffsetInBioseq (slp, nucBsp, SEQLOC_START);
  stop = GetOffsetInBioseq (slp, nucBsp, SEQLOC_STOP);
  strand = SeqLocStrand (slp);
  max_stop = nucBsp->length - 1;
  new_start = start;
 
  offset = frame - 1;
  sip = SeqLocId (slp);
  if (strand == Seq_strand_minus) {
    stop -= offset;
    new_start = start - offset;
    if ((1 + new_start - stop ) % 3 != 0) {
      new_start -= (1 + new_start - stop) % 3;
    }
    if (stop < 0) stop = 0;
    slp = ExpandSeqLoc (stop, start, strand, nucBsp, slp);
    if (new_start < 0) new_start = 0;
    if (new_start < start) {
      slp = SeqLocDelete (slp, sip, new_start + 1, start, FALSE, &changed);
    }
  } else {
    stop += offset;
    new_start = start + offset;
    if ((1 + stop - new_start) % 3 != 0) {
      new_start += (1 + stop - new_start) % 3;
    }
    if (stop > max_stop) stop = max_stop;
    slp = ExpandSeqLoc (start, stop, strand, nucBsp, slp);
    if (start > max_stop) start = max_stop;
    if (new_start > start) {
      slp = SeqLocDelete (slp, sip, start, new_start - 1, FALSE, &changed);
    }
  }
  return slp;
}

static TransTablePtr GetTranslationTable (CdRegionPtr crp, Boolean PNTR table_is_local)
{
  TransTablePtr tbl = NULL;
  ValNodePtr    vnp;
  Int2          genCode = 0;
  Char          str [32];

  if (crp == NULL || table_is_local == NULL) return NULL;

  *table_is_local = FALSE;
  /* find genetic code */

  if (crp->genetic_code != NULL) {
    vnp = (ValNodePtr) crp->genetic_code->data.ptrvalue;
    while (vnp != NULL) {
      if (vnp->choice == 2) {
        genCode = (Int2) vnp->data.intvalue;
      }
      vnp = vnp->next;
    }
  }

  if (genCode == 7) {
    genCode = 4;
  } else if (genCode == 8) {
    genCode = 1;
  } else if (genCode == 0) {
    genCode = 1;
  }

  /* set up translation table */
  /* set app property name for storing desired FSA */

  sprintf (str, "TransTableFSAforGenCode%d", (int) genCode);

  /* get FSA for desired genetic code if it already exists */

  tbl = (TransTablePtr) GetAppProperty (str);
  if (tbl == NULL) {
    tbl = TransTableNew (genCode);
    *table_is_local = TRUE;
  }
  return tbl;
}

static CharPtr ExtendProtein5
(SeqFeatPtr sfp,
 Uint2      input_entityID,
 Boolean    force_partial)
{
  CdRegionPtr   crp;
  TransTablePtr tbl = NULL;
  Boolean       table_is_local = FALSE;
  SeqLocPtr     test_slp;
  SeqIdPtr      sip;
  BioseqPtr     nucBsp;
  Int4          start, new_start;
  Int4          stop, new_stop;
  Int4          increment = 3000;
  Int4          offset;
  Uint1         strand;
  Boolean       found_start = FALSE;
  Boolean       found_stop = FALSE;
  Boolean       stop_looking = FALSE;
  Int4          dna_len;
  CharPtr       bases;
  Int4          total;
  Int2          state;
  CharPtr       codon_start;
  Boolean       partial3, partial5;
  Boolean       contains_start, contains_stop;
  Boolean       changed;
  CharPtr       newprot;
  Boolean       truncated;
 
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION 
      || (crp = (CdRegionPtr)sfp->data.value.ptrvalue) == NULL) {
    return NULL;
  }
  nucBsp = GetBioseqGivenSeqLoc (sfp->location, input_entityID);
  if (nucBsp == NULL) return NULL;

  tbl = GetTranslationTable (crp, &table_is_local);
  if (tbl == NULL) return NULL;

  test_slp = SeqLocMerge (nucBsp, sfp->location, NULL, FALSE, FALSE, FALSE);
  strand = SeqLocStrand (sfp->location);
  sip = SeqLocId (sfp->location);
  offset = -1;

  start = GetOffsetInBioseq (test_slp, nucBsp, SEQLOC_START);
  if (start == 0)
  {
  	stop_looking = TRUE;
  }
 
  while (((! found_start && ! found_stop) || force_partial) && ! stop_looking) {
    start = GetOffsetInBioseq (test_slp, nucBsp, SEQLOC_START);
    stop = GetOffsetInBioseq (test_slp, nucBsp, SEQLOC_STOP);
    if (strand == Seq_strand_minus) {
      new_start = start + increment;
      new_stop = start + 1;
      if (new_start > nucBsp->length - 1) {
        new_start = start + ((Int4)((nucBsp->length - 1 - start) / 3)) * 3;
        stop_looking = TRUE;
      }
      test_slp = ExpandSeqLoc (stop, new_start, strand, nucBsp, test_slp);
      test_slp = SeqLocDelete (test_slp, sip, stop, start, FALSE, &changed);
    } else {
      new_start = start - increment;
      new_stop = start - 1;
      if (new_start < 0) {
        new_start = start % 3;
        stop_looking = TRUE;
      }
      test_slp = ExpandSeqLoc (new_start, stop, strand, nucBsp, test_slp);
      test_slp = SeqLocDelete (test_slp, sip, start, stop, FALSE, &changed);
    }
    dna_len = SeqLocLen (test_slp);
    bases = ReadCodingRegionBases (test_slp, dna_len, crp->frame, &total);
    if (bases == NULL) {
      stop_looking = TRUE;
    } else {
      state = 0;
      codon_start = bases + StringLen (bases) - 3;
      while (codon_start >= bases && ! found_start && ! found_stop) {
        state = 0;
        state = NextCodonState (tbl, state, (Uint1)*codon_start);
        state = NextCodonState (tbl, state, (Uint1)*(codon_start + 1));
        state = NextCodonState (tbl, state, (Uint1)*(codon_start + 2));
        if (IsOrfStart (tbl, state, TTBL_TOP_STRAND)) {
          found_start = TRUE;
          if (strand == Seq_strand_minus) {
            offset = new_start - (codon_start - bases);
          } else {
            offset = new_start + codon_start - bases;
          }
        } else if (IsOrfStop (tbl, state, TTBL_TOP_STRAND)) {
          found_stop = TRUE;
        } else {
          codon_start -= 3;
        }
      }
      MemFree (bases);
    }
  }
  
  SeqLocFree (test_slp);
  if (! found_stop) { 
    start = GetOffsetInBioseq (sfp->location, nucBsp, SEQLOC_START);
    stop = GetOffsetInBioseq (sfp->location, nucBsp, SEQLOC_STOP);
    if (found_start) {
      if (strand == Seq_strand_minus) {
        sfp->location = ExpandSeqLoc (stop, offset, strand, nucBsp, sfp->location);
      } else {
        sfp->location = ExpandSeqLoc (offset, stop, strand, nucBsp, sfp->location);
      }
    } else {
      CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
      SetSeqLocPartial (sfp->location, TRUE, partial3);
      sfp->partial = TRUE;
      if (crp->frame == 0)
      {
        crp->frame = 1;
      }
      if (strand == Seq_strand_minus) {
        sfp->location = ExpandSeqLoc (stop, nucBsp->length - 1, strand, nucBsp, sfp->location);
        crp->frame = (nucBsp->length - 1 - start + crp->frame - 1) % 3 + 1;
      } else {
        sfp->location = ExpandSeqLoc (0, stop, strand, nucBsp, sfp->location);
        crp->frame = (start + crp->frame - 1) % 3 + 1;
      }
    }
  }
  newprot = FixProteinString (sfp, strand, FALSE, &truncated,
                              &contains_start, &contains_stop);
  if (table_is_local) {
    TransTableFree (tbl);
  }

  return newprot;
}


static CharPtr ExtendProtein3 
(SeqFeatPtr sfp,
 Uint2      input_entityID,
 Boolean    force_partial)
{
  BioseqPtr nucBsp;
  Int4      max_stop, min_start, start, stop;
  Uint1     strand;
  Boolean   contains_start, contains_stop;
  Int4      increment = 3000;
  CharPtr   newprot;
  Boolean   partial5, partial3;
  Boolean   truncated;

  if (sfp == NULL) return NULL;

  nucBsp = GetBioseqGivenSeqLoc (sfp->location, input_entityID);
  if (nucBsp == NULL) return NULL;

  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  start = GetOffsetInBioseq (sfp->location, nucBsp, SEQLOC_START);
  stop = GetOffsetInBioseq (sfp->location, nucBsp, SEQLOC_STOP);
  strand = SeqLocStrand (sfp->location);
  max_stop = nucBsp->length - 1;
  if (stop > start) {
    while (max_stop % 3 != stop % 3) {
      max_stop --;
    }
    min_start = start %3;
  } else {
    while (max_stop % 3 != start % 3) {
      max_stop --;
    }
    min_start = stop % 3;
  } 
  
  contains_stop = FALSE;
  contains_start = FALSE;
  newprot = NULL;
  /* need to initialize newprot in case we're already at the edge */
  if ((strand != Seq_strand_minus && stop == max_stop)
      || (strand == Seq_strand_minus && stop == min_start))
  {
  	newprot = FixProteinString (sfp, strand, FALSE, &truncated,
                              &contains_start, &contains_stop);
  }
  while ((! contains_stop || force_partial) &&
         (   (strand == Seq_strand_minus && stop > min_start) 
          || (strand != Seq_strand_minus && stop < max_stop)))
  {
    if (newprot != NULL) {
      MemFree (newprot);
      newprot = NULL;
    }
    if (strand == Seq_strand_minus) {
      stop -= increment;
      if (stop < min_start) {
        stop = min_start;
      }
      sfp->location = ExpandSeqLoc (stop, start, strand, nucBsp, sfp->location);
    } else {
      stop += increment;
      if (stop > max_stop) {
        stop = max_stop;
      }
      sfp->location = ExpandSeqLoc (start, stop, strand, nucBsp, sfp->location);
    }
    newprot = FixProteinString (sfp, strand, TRUE, &truncated,
                              &contains_start, &contains_stop);
  }

  if (! contains_stop || force_partial) {
    start = GetOffsetInBioseq (sfp->location, nucBsp, SEQLOC_START);
    stop = GetOffsetInBioseq (sfp->location, nucBsp, SEQLOC_STOP);
    if (strand == Seq_strand_minus) {
      sfp->location = ExpandSeqLoc (0, start, strand, nucBsp, sfp->location);
    } else {
      sfp->location = ExpandSeqLoc (start, nucBsp->length - 1, strand, nucBsp, sfp->location);
    }
    partial3 = TRUE;
    SetSeqLocPartial (sfp->location, partial5, TRUE);
  }
  return newprot;
}

static Boolean MergeOverlapsForThisFeature (SeqFeatPtr sfp)
{  
  if (sfp == NULL) return FALSE;
  return ! sfp->excpt;
}

static Boolean 
AdjustCDSForUpdate
(SeqFeatPtr   sfp,
 Uint2        input_entityID,
 Uint1        frame,
 Int4         transl_except_len,
 Boolean      truncate_proteins,
 Boolean      extend_proteins5,
 Boolean      extend_proteins3,
 Boolean PNTR truncated,
 Boolean PNTR stop_shifted,
 Boolean PNTR extended5,
 Boolean PNTR extended3,
 BioseqPtr    protBsp,
 BioseqPtr PNTR newbsp)
{
  ByteStorePtr  bs;
  CharPtr       newprot, ptr;
  SeqEntryPtr   nwsep;
  BioseqPtr     newBsp;
  Boolean       contains_start, contains_stop;
  Uint1         strand;
  SeqLocPtr     newloc;
  BioseqPtr     nucBsp;
  Boolean       partial5, partial3;
  CharPtr       seqnew = NULL, seqold = NULL;
  Boolean       rval = FALSE;
 
  if (sfp == NULL
    || sfp->idx.subtype != FEATDEF_CDS
    || sfp->product == NULL
    || protBsp == NULL
    || truncated == NULL || stop_shifted == NULL
    || extended5 == NULL || extended3 == NULL
    || newbsp == NULL)
  {
    return FALSE;
  }
  
  CheckSeqLocForPartial (sfp->location, &partial3, &partial5);
  
  nucBsp = GetBioseqGivenSeqLoc (sfp->location, input_entityID);
  if (nucBsp == NULL) return FALSE;
  
  newloc = SeqLocMergeExEx (nucBsp, sfp->location, NULL, FALSE, FALSE,
                            MergeOverlapsForThisFeature (sfp), FALSE, FALSE);

  if (newloc == NULL) return FALSE;
  sfp->location = newloc;
  strand = SeqLocStrand (sfp->location);
  sfp->location = ShiftStopForLengthChange (sfp->location, nucBsp, transl_except_len, stop_shifted);
  if (sfp->location == NULL) {
    return FALSE;
  }

  sfp->location = ShiftLocationForFrame (sfp->location, frame, nucBsp);

  newprot = FixProteinString (sfp, strand, truncate_proteins, truncated,
                              &contains_start, &contains_stop);

  /* Must do 3' end first, otherwise may truncate at stops introduced by expanding 5' end for partiality */
  if ((! contains_stop && extend_proteins3 && transl_except_len == 0)
      || ((extend_proteins3 || partial3) && !truncate_proteins)) {
    MemFree (newprot);
    newprot = ExtendProtein3 (sfp, input_entityID, partial3 && !truncate_proteins);
    if (newprot == NULL) return FALSE;
    *extended3 = TRUE;
  } else {
    *extended3 = FALSE;
  }
  if (! contains_start && (extend_proteins5 || partial5)) {
    MemFree (newprot);
    newprot = ExtendProtein5 (sfp, input_entityID, partial5);
    if (newprot == NULL) return FALSE;
    *extended5 = TRUE;
  } else {
    *extended5 = FALSE;
  }

  sfp->partial = CheckSeqLocForPartial (sfp->location, &partial5, &partial3);

  bs = BSNew (1000);
  if (bs != NULL)
  {
    ptr = newprot;
    BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
  } 
  MemFree (newprot);
  
  newBsp = BioseqNew ();
  if (newBsp == NULL) {
    return FALSE;
  }

  newBsp->id = SeqIdParse ("lcl|ProtAlign");
  newBsp->repr = Seq_repr_raw;
  newBsp->mol = Seq_mol_aa;
  newBsp->seq_data_type = Seq_code_ncbieaa;
  newBsp->seq_data = bs;
  newBsp->length = BSLen (bs);

  /* create SeqEntry for temporary protein bioseq to live in */
  nwsep = SeqEntryNew ();
  nwsep->choice = 1;
  nwsep->data.ptrvalue = newBsp;
  SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) newBsp, nwsep);

  seqold = GetSequenceByBsp (protBsp);
  seqnew = GetSequenceByBsp (newBsp);

  if (StringCmp (seqold, seqnew) == 0
    || (contains_stop && ! *truncated
      && StringNCmp (seqold,
                     seqnew,
                     StringLen (seqold)) == 0))
  {
    SeqEntryFree (nwsep);
    rval = FALSE;
  }
  else
  {
    *newbsp = newBsp;
    rval = TRUE;
  }

  return rval;   
}

  
extern SeqLocPtr SeqLocWholeNew (BioseqPtr bsp)
{
  ValNodePtr vnp;

  if (bsp == NULL) return NULL;

  vnp = ValNodeNew (NULL);

  if (vnp == NULL) return NULL;

  vnp->choice = SEQLOC_WHOLE;
  vnp->data.ptrvalue = (Pointer) SeqIdDup (SeqIdFindBest (bsp->id, 0));
  return (SeqLocPtr)vnp;
}


static void WarnNoProteinUpdate (BioseqPtr bsp)
{
  Char     acc_str [256];
  CharPtr  warn_format = "Protein %s has not been updated - you must manually re-translate the coding regions and move any protein features";
  CharPtr  warn_msg;

  if (bsp == NULL || bsp->id == NULL) return;
  SeqIdWrite (bsp->id, acc_str, PRINTID_REPORT, sizeof (acc_str));
  warn_msg = (CharPtr) MemNew (StringLen (warn_format) + StringLen (acc_str));
  if (warn_msg == NULL) return;
  sprintf (warn_msg, warn_format, acc_str);
  ErrPostEx (SEV_ERROR, 0, 0, warn_msg);
  MemFree (warn_msg);
}


static void LogFrameChange (FILE *fp, BioseqPtr bsp, Uint1 frame)
{
  Char     acc_str [256];

  if (fp == NULL || bsp == NULL || bsp->id == NULL) return;
  SeqIdWrite (bsp->id, acc_str, PRINTID_REPORT, sizeof (acc_str));
  fprintf (fp, "Changed frame for %s to %d\n", acc_str, frame);
}


static void LogProteinTruncate (FILE *fp, BioseqPtr bsp)
{
  Char     acc_str [256];

  if (fp == NULL || bsp == NULL || bsp->id == NULL) return;
  SeqIdWrite (bsp->id, acc_str, PRINTID_REPORT, sizeof (acc_str));
  fprintf (fp, "Truncated protein %s at stop\n", acc_str);
}


static void LogProteinStopShift (FILE *fp, BioseqPtr bsp)
{
  Char     acc_str [256];

  if (fp == NULL || bsp == NULL || bsp->id == NULL) return;
  SeqIdWrite (bsp->id, acc_str, PRINTID_REPORT, sizeof (acc_str));
  fprintf (fp, "Adjusted length of CDS for protein %s to be multiple of 3\n", acc_str);
}


static void LogProteinExtend5 (FILE *fp, BioseqPtr bsp)
{
  Char     acc_str [256];

  if (fp == NULL || bsp == NULL || bsp->id == NULL) return;
  SeqIdWrite (bsp->id, acc_str, PRINTID_REPORT, sizeof (acc_str));
  fprintf (fp, "Extended protein %s on 5' end\n", acc_str);
}


static void LogProteinExtend3 (FILE *fp, BioseqPtr bsp)
{
  Char     acc_str [256];

  if (fp == NULL || bsp == NULL || bsp->id == NULL) return;
  SeqIdWrite (bsp->id, acc_str, PRINTID_REPORT, sizeof (acc_str));
  fprintf (fp, "Extended protein %s on 3' end\n", acc_str);
}

static void LogGeneCorrection (FILE *fp, SeqFeatPtr gene, SeqLocPtr oldloc)
{
  CharPtr           loc1, loc2;
  SeqMgrFeatContext gcontext;
  SeqFeatPtr        found_gene;

  if (fp == NULL || gene == NULL || gene->location == NULL || oldloc == NULL) return;
  found_gene = SeqMgrGetDesiredFeature (gene->idx.entityID, NULL, 0, 0, gene, &gcontext);
  if (found_gene == NULL) return;
  loc1 = SeqLocPrint (gene->location);
  loc2 = SeqLocPrint (oldloc);
  if (loc1 != NULL && loc2 != NULL && StringCmp (loc1, loc2) != 0) {
    fprintf (fp, "Moved gene %s from %s to %s\n", gcontext.label, loc2, loc1);
  }
  MemFree (loc1);
  MemFree (loc2);
}


static void LogGeneBadCorrection (FILE *fp, SeqFeatPtr gene)
{
  SeqMgrFeatContext gcontext;
  SeqFeatPtr        found_gene;

  if (fp == NULL || gene == NULL || gene->location == NULL) return;
  found_gene = SeqMgrGetDesiredFeature (gene->idx.entityID, NULL, 0, 0, gene, &gcontext);
  if (found_gene == NULL) return;
  fprintf (fp, "Please examine gene %s, new gene may be too large\n", gcontext.label);
}


static void CorrectCDSGene
(SeqFeatPtr sfp,
 SeqFeatPtr gene_sfp,
 FILE *fp,
 Boolean PNTR data_in_log)
{
  BioseqPtr bsp;
  Boolean   partial5, partial3;
  SeqLocPtr log_slp, new_slp;
  Int2      res;

  if (sfp == NULL || gene_sfp == NULL || fp == NULL 
      || data_in_log == NULL){
    return;
  }
  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp == NULL) return;
  new_slp = SeqLocMerge (bsp, sfp->location, NULL, TRUE, FALSE, FALSE);
  res = SeqLocCompare (gene_sfp->location, new_slp);
  if (res == SLC_A_EQ_B) {
    SeqLocFree (new_slp);
    return;
  }
  if (SeqLocLen (gene_sfp->location) > SeqLocLen (new_slp)) {
    SeqLocFree (new_slp);
    new_slp = SeqLocMerge (bsp, sfp->location, gene_sfp->location, TRUE, FALSE, FALSE);
    LogGeneBadCorrection (fp, gene_sfp);
  }
  log_slp = SeqLocMerge (bsp, sfp->location, NULL, FALSE, FALSE, FALSE);

  *data_in_log = TRUE;
  SeqLocFree (gene_sfp->location);
  gene_sfp->location = new_slp;
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  SetSeqLocPartial (gene_sfp->location, partial5, partial3);
  gene_sfp->partial = sfp->partial;
  LogGeneCorrection (fp, gene_sfp, log_slp);
  SeqLocFree (log_slp);
}

static void FixProtRefPtrs (ValNodePtr feat_list)
{
  ValNodePtr vnp;
  SeqFeatPtr sfp;
  Uint1      strand;
  BioseqPtr  bsp;

  if (feat_list == NULL) return;
  for (vnp = feat_list; vnp != NULL; vnp = vnp->next) {
    sfp = (SeqFeatPtr)vnp->data.ptrvalue;
    if (sfp == NULL) continue;
    bsp = BioseqFindFromSeqLoc (sfp->location);
    if (bsp == NULL) continue;
    strand = SeqLocStrand (sfp->location);
    if (strand == Seq_strand_minus) {
      sfp->location = ExpandSeqLoc (bsp->length - 1, 0, strand, bsp, sfp->location);
    } else {
      sfp->location = ExpandSeqLoc (0, bsp->length - 1, strand, bsp, sfp->location);
    }
  }
}


static Boolean PrepareUpdateAlignmentForProtein
(SeqFeatPtr sfp,
 BioseqPtr  protBsp,
 Uint2      input_entityID,
 FILE *     fp,
 Boolean    truncate_proteins,
 Boolean    extend_proteins5,
 Boolean    extend_proteins3,
 Boolean    correct_cds_genes,
 Int4       transl_except_len,
 Boolean    *data_in_log,
 SeqAlignPtr PNTR salpptr,
 BioseqPtr PNTR newbspptr)
{
  Boolean       align_succeeded;
  Boolean       changed_frame;
  Uint1         frame_attempts;
  ErrSev        level;
  CdRegionPtr   crp;
  SeqLocPtr     orig_slp;
  Boolean       orig_partial;
  SeqFeatPtr    gene_sfp;
  SeqMgrFeatContext gcontext;
  Boolean       truncated, stop_shifted;
  Boolean       extended5, extended3;
  Uint1         old_frame;
  BioseqPtr     newBsp = NULL;
  Boolean       adjust_succeeded;
  SeqAlignPtr   salp = NULL;
  Boolean       revcomp;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION
      || (crp = (CdRegionPtr)sfp->data.value.ptrvalue) == NULL
      || protBsp == NULL
      || data_in_log == NULL) {
    return FALSE;
  }

  old_frame = crp->frame;

  orig_slp = sfp->location;
  orig_partial = sfp->partial;
  if (correct_cds_genes) {
    gene_sfp = SeqMgrGetOverlappingGene (sfp->location, &gcontext);
  }
  align_succeeded = FALSE;
  changed_frame = FALSE;

  level = ErrSetMessageLevel (SEV_MAX);
  for (frame_attempts = 1;
       frame_attempts < 4 && ! align_succeeded;
       frame_attempts ++) {
    if (sfp->location != orig_slp) {
      if (sfp->location->choice == 0) {
        SeqLocFree (sfp->location);
      }
      sfp->location = orig_slp;
    }
    sfp->partial = orig_partial;    
    crp->frame = old_frame;
    if (newBsp != NULL) {
      newBsp = BioseqFree (newBsp);
    }
    truncated= FALSE;
    stop_shifted = FALSE;
    extended5 = FALSE;
    extended3 = FALSE;
    adjust_succeeded = AdjustCDSForUpdate (sfp,
                                           input_entityID,
                                           frame_attempts,
                                           transl_except_len,
                                           truncate_proteins,
                                           extend_proteins5,
                                           extend_proteins3,
                                           &truncated, &stop_shifted,
                                           &extended5, &extended3,
                                           protBsp, &newBsp);
    if (sfp->location == NULL || sfp->location->choice == 0) 
    {
      sfp->location = orig_slp;
    }
    if (adjust_succeeded)
    {
      salp = Sequin_GlobalAlign2Seq (protBsp, newBsp, &revcomp);
    }
    if (!adjust_succeeded || salp != NULL)
    {
      if (frame_attempts > 1) {
        changed_frame = TRUE;
        LogFrameChange (fp, protBsp, frame_attempts);
        *data_in_log = TRUE;
      }
      break;
    }    
  }
  ErrSetMessageLevel (level);
  if (adjust_succeeded && salp == NULL) 
  {
    /* put CD Region back to original state */
    crp->frame = old_frame;
    if (sfp->location != orig_slp) {
      SeqLocFree (sfp->location);
      sfp->location = orig_slp;
    }
    sfp->partial = orig_partial;
    WarnNoProteinUpdate (protBsp);
    newBsp = BioseqFree (newBsp);
    return FALSE;
  }

  if (truncated) {
    LogProteinTruncate (fp, protBsp);
    *data_in_log = TRUE;
  }
  if (stop_shifted) {
    LogProteinStopShift (fp, protBsp);
    *data_in_log = TRUE;
  }
  if (extended5) {
    LogProteinExtend5 (fp, protBsp);
    *data_in_log = TRUE;
  }
  if (extended3) {
    LogProteinExtend3 (fp, protBsp);
    *data_in_log = TRUE;
  }
  
  if (correct_cds_genes && gene_sfp != NULL) {
    CorrectCDSGene (sfp, gene_sfp, fp, data_in_log);
  }
  if (sfp->location != orig_slp) {
    SeqLocFree (orig_slp);
  }
  
  *salpptr = salp;
  *newbspptr = newBsp;
  return TRUE;
}

static Boolean CodingRegionHasTranslExcept (SeqFeatPtr sfp)
{
  CodeBreakPtr cbp;
  Int4         len;
  CdRegionPtr  crp;
  SeqLocPtr    slp;
  Int4         codon_start, codon_stop, pos, codon_length;
  
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION
      || (crp = (CdRegionPtr)sfp->data.value.ptrvalue) == NULL
      || crp->code_break == NULL)
  {
  	return FALSE;
  }

  len = SeqLocLen (sfp->location);
  if (crp->frame > 1) 
  {
  	len -= crp->frame - 1;
  }
  if (len % 3 == 0) 
  {
  	return FALSE;
  }
  for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next)
  {
    codon_start = INT4_MAX;
    codon_stop = -10;
    slp = NULL;
    while ((slp = SeqLocFindNext (cbp->loc, slp)) != NULL) {
      pos = GetOffsetInLoc (slp, sfp->location, SEQLOC_START);
      if (pos < codon_start)
      {
        codon_start = pos;
        pos = GetOffsetInLoc (slp, sfp->location, SEQLOC_STOP);
        if (pos > codon_stop)
        {
          codon_stop = pos;
        }
        codon_length = codon_stop - codon_start;      /* codon length */
        if (codon_length >= 0 && codon_length <= 1 && codon_stop == len - 1)
        {                       /*  a codon */
          /* allowing a partial codon at the end */
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

typedef struct proteinfromcdsdata {
 Uint2      input_entityID;
 Uint4      input_itemID;
 Uint4      input_itemtype;
 FILE *     fp;
 Boolean    data_to_report;
} ProteinFromCDSData, PNTR ProteinFromCDSPtr;

static void UpdateOneProteinFromCDS (SeqFeatPtr sfp, Pointer userdata)
{
  ProteinFromCDSPtr pfcp;
  SeqLocPtr         new_product;
  Boolean           data_in_log;
  Int4              transl_except_len = 0;
  BioseqPtr         protBsp;
  SeqAlignPtr       salp = NULL;
  BioseqPtr         newbsp = NULL;
  Boolean           rval;
  
  pfcp = (ProteinFromCDSPtr) userdata;
  if ( pfcp == NULL
    || sfp == NULL
    || sfp->idx.subtype != FEATDEF_CDS
    || sfp->product == NULL)
  {
    return;
  }
  
  protBsp = BioseqFindFromSeqLoc (sfp->product);
  if (protBsp == NULL)
  {
    return;
  }
 
  data_in_log = FALSE;
  
  if (CodingRegionHasTranslExcept(sfp))
  {
  	transl_except_len = SeqLocLen (sfp->location) % 3;
  }
  
  rval = PrepareUpdateAlignmentForProtein (sfp,
                                           protBsp,
                                           pfcp->input_entityID,
                                           pfcp->fp,
                                           TRUE, TRUE, TRUE, TRUE,
                                           transl_except_len,
                                           &data_in_log,
                                           &salp,
                                           &newbsp);
  if (data_in_log) {
    pfcp->data_to_report = TRUE;
  }
  if (!rval) return;

  ReplaceOneSequence (salp, protBsp, newbsp);
  if (sfp->product->choice != SEQLOC_WHOLE)
  {
    new_product = SeqLocWholeNew (protBsp);
    if (new_product == NULL) return;
    SeqLocFree (sfp->product);
    sfp->product = new_product;
  }
}

static Boolean CheckForIDCollision (
  BioseqPtr oldbsp,
  BioseqPtr newbsp,
  BoolPtr islocal
)

{
  SeqIdPtr  sip;

  if (oldbsp == NULL || newbsp == NULL) return FALSE;
  for (sip = newbsp->id; sip != NULL; sip = sip->next) {
    if (SeqIdIn (sip, oldbsp->id)) {
      if (sip->choice == SEQID_LOCAL) {
        *islocal = TRUE;
      }
      return TRUE;
    }
  }
  return FALSE;
}

static CharPtr convPubDescMssg =
"Do you wish to convert publications to apply only to the appropriate ranges?";

#define UPDATE_SKIP_THIS    1
#define UPDATE_SKIP_ALL     2
#define UPDATE_REPLACE_THIS 3
#define UPDATE_REPLACE_ALL  4
#define UPDATE_CANCEL       5

typedef struct noalignmentchoice
{
  Boolean done;
  Boolean cancelled;
  GrouP   action_choice;
  Boolean skip_this;
  Boolean skip_all;
  Boolean replace_this;
  Boolean replace_all;
} NoAlignmentChoiceData, PNTR NoAlignmentChoicePtr;

static void NoAlignmentChoiceOk (ButtoN b)
{
  NoAlignmentChoicePtr nacp;
  
  nacp = (NoAlignmentChoicePtr) GetObjectExtra (b);
  if (nacp == NULL) return;
  nacp->cancelled = FALSE;
  nacp->done = TRUE;
}

static void NoAlignmentChoiceCancel (ButtoN b)
{
  NoAlignmentChoicePtr nacp;
  
  nacp = (NoAlignmentChoicePtr) GetObjectExtra (b);
  if (nacp == NULL) return;
  nacp->cancelled = TRUE;
  nacp->done = TRUE;
}

static Int2 GetNoAlignmentChoice (SeqIdPtr id, Int2 previous_choice)
{
  Char                  buf [64];
  WindoW                w;
  GrouP                 h, g, c;
  ButtoN                b;
  NoAlignmentChoiceData nacd;
  Char                  promptstr[115];
  Int2                  no_aln_choice;
  SeqIdPtr              sip, sip_next;

  sip = SeqIdFindBest (id, 0);
  if (sip == NULL) return UPDATE_CANCEL;
  w = ModalWindow(-20, -13, -10, -10, NULL);
  if (w == NULL) return 0;

  h = HiddenGroup (w, -1, 0, NULL);
  g = HiddenGroup(h, 0, 4, NULL);
  sip_next = sip->next;
  sip->next = NULL;
  SeqIdWrite (sip, buf, PRINTID_REPORT, sizeof (buf) - 1);
  sip->next = sip_next;
  sprintf (promptstr, "There is no alignment between the sequences for %s.", buf);
  StaticPrompt (g, promptstr, 0, popupMenuHeight, programFont, 'l');
  StaticPrompt (g, "You may choose to :", 0, popupMenuHeight, programFont, 'l');
  nacd.action_choice = HiddenGroup (g, 0, 4, NULL);
  RadioButton (nacd.action_choice, "Skip This Sequence");
  RadioButton (nacd.action_choice, "Skip All Sequences without Alignments");
  RadioButton (nacd.action_choice, "Replace This Sequence");
  RadioButton (nacd.action_choice, "Replace All Sequences without Alignments");
  if (previous_choice > 0 && previous_choice < UPDATE_CANCEL)
  {
    SetValue (nacd.action_choice, previous_choice);
  }
  else
  {
    SetValue (nacd.action_choice, 1);
  }
  c = HiddenGroup (h, 2, 0, NULL);
  b = PushButton (c, "Ok", NoAlignmentChoiceOk);
  SetObjectExtra (b, &nacd, NULL);
  b = PushButton (c, "Cancel", NoAlignmentChoiceCancel);
  SetObjectExtra (b, &nacd, NULL);
  
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);

  nacd.cancelled = FALSE;
  nacd.done = FALSE;
  Show(w); 
  Select (w);
  while (!nacd.done)
  {
    ProcessExternalEvent ();
    Update ();
  }
  ProcessAnEvent ();
  no_aln_choice = GetValue (nacd.action_choice);
  Remove (w);
  if (nacd.cancelled)
  {
    return UPDATE_CANCEL;
  }
  else
  {
    return no_aln_choice;
  }
}

static Boolean PrepareUpdatePtr (UpsDataPtr    udp)
{
  Uint2        entityID;
  SeqEntryPtr  oldsep, newsep;
  SeqIdPtr     sip;
  Boolean      islocal = FALSE;
  Char         buf [64];
  SeqAlignPtr  salp = NULL;
  Boolean      revcomp = FALSE;
  Boolean      asked_about_desc_prop = FALSE;
  Boolean      propagate_descriptors = FALSE;
  SeqIdPtr     id, id_next;
  MsgAnswer    ans;
  Char         collision_id [100];

  if (udp->oldbsp == NULL || udp->newbsp == NULL) return FALSE;
  if (ISA_na (udp->oldbsp->mol) != ISA_na (udp->newbsp->mol)) {
    Message (MSG_OK, "Both sequences must be either nucleotides or proteins");
    return FALSE;
  }

  entityID = ObjMgrGetEntityIDForPointer (udp->oldbsp);
  oldsep = GetBestTopParentForData (entityID, udp->oldbsp);
  entityID = ObjMgrGetEntityIDForPointer (udp->newbsp);
  newsep = GetBestTopParentForData (entityID, udp->newbsp);
  if (oldsep == NULL || newsep == NULL)
    return FALSE;

  if (CONVERTPUBS_NOT_SET == udp->convertPubs) 
  {
    if (Message (MSG_YN, convPubDescMssg) == ANS_YES) {
      ConvertPubSrcComDescsToFeats (oldsep, TRUE, FALSE, FALSE, FALSE, &asked_about_desc_prop, &propagate_descriptors, NULL);
      ConvertPubSrcComDescsToFeats (newsep, TRUE, FALSE, FALSE, FALSE, &asked_about_desc_prop, &propagate_descriptors, NULL);
      udp->convertPubs = CONVERTPUBS_YES;
    }
    else
      udp->convertPubs = CONVERTPUBS_NO;
  }

  if (CheckForIDCollision (udp->oldbsp, udp->newbsp, &islocal)) {
    sprintf (collision_id, "lcl|SequinUpdateSequence%d", udp->seqsubpos);
    sip = SeqIdParse (collision_id);
    if (sip != NULL) {
      BioseqReplaceID (udp->newbsp, sip);
      sip = SeqIdFree (sip);
    }
  }
  
  salp = Sequin_GlobalAlign2Seq (udp->oldbsp, udp->newbsp, &revcomp);

  if (salp == NULL) {
    if (udp->log_fp != NULL) {
      id = SeqIdFindBest (udp->oldbsp->id, 0);
      if (id != NULL)
      {
        id_next = id->next;
        id->next = NULL;
        SeqIdWrite (id, buf, PRINTID_REPORT, sizeof (buf) - 1);
        id->next = id_next;
        fprintf (udp->log_fp, "No sequence similarity for %s\n", buf);
        udp->data_in_log = TRUE;  
      }
    }
    if (udp->suppress_continue_msg) {
      return FALSE;
    } else {
      if (udp->isSet && udp->do_update)
      {
        if (udp->no_aln_choice != UPDATE_SKIP_ALL && udp->no_aln_choice != UPDATE_REPLACE_ALL)
        {
          udp->no_aln_choice = GetNoAlignmentChoice (udp->oldbsp->id, udp->no_aln_choice);
        }
        if (udp->no_aln_choice == UPDATE_CANCEL)
        {
          return FALSE;
        }
      }
      else if (udp->do_update)
      {
        ans = Message (MSG_YN, "There is no alignment between the sequences.  Do you wish to continue?");
        if (ans == ANS_YES)
        {
          udp->useGUI = TRUE;
        }
        else
        {
          return FALSE;
        }
      }
    }
  }
  udp->salp     = salp;
  udp->revcomp  = revcomp;
  udp->diffOrgs = FALSE;
  udp->recomb1  = -1;
  udp->recomb2  = -1;

  return TRUE;
}

static void UpdateProteinsOnNewBsp (SeqFeatPtr sfp, Pointer userdata);

static void
FindProtRefPtrsOnBsp
(BioseqPtr bsp,
 ValNodePtr PNTR feat_list)
{
  SeqMgrFeatContext context;
  SeqFeatPtr        sfp;
  ValNodePtr        vnp;
 
  if (bsp == NULL || feat_list == NULL) return;
  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_PROT, 0, &context);
  while (sfp != NULL) {
    if (context.left == 0 && context.right == bsp->length - 1) {
      vnp = ValNodeNew (*feat_list);
      if (vnp != NULL) {
        vnp->data.ptrvalue = sfp;
      }
      if (*feat_list == NULL) {
        *feat_list = vnp;
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_PROT, 0, &context);
  }
}

static ValNodePtr FindProductProtRefs (BioseqPtr bsp)
{
  SeqFeatPtr        sfp;
  SeqMgrFeatContext context;
  ValNodePtr        feat_list = NULL;

  if (bsp == NULL) return NULL;
  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &context);
  while (sfp != NULL) {
    FindProtRefPtrsOnBsp (BioseqFindFromSeqLoc (sfp->product), &feat_list);
    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_CDREGION, 0, &context);
  }
  return feat_list; 
}

static ValNodePtr FindTranslExceptCDSs (BioseqPtr bsp)
{
  SeqFeatPtr        sfp;
  SeqMgrFeatContext context;
  ValNodePtr        feat_list = NULL;
  ValNodePtr        vnp;
  Int4              cd_len;

  if (bsp == NULL) return NULL;
  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &context);
  while (sfp != NULL) {
    if (CodingRegionHasTranslExcept (sfp)) 
    {
      vnp = ValNodeNew(feat_list);
      if (feat_list == NULL) 
      {
      	feat_list = vnp;
      }
      if (vnp != NULL)
      {
      	cd_len = SeqLocLen (sfp->location);
      	vnp->choice = cd_len % 3;
      	vnp->data.ptrvalue = sfp;
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_CDREGION, 0, &context);
  }
  return feat_list; 
}

static Int4 GetOriginalTranslExceptLen (SeqFeatPtr sfp, ValNodePtr list)
{
  ValNodePtr vnp;
  
  if (sfp == NULL || list == NULL) return 0;
  for (vnp = list; vnp != NULL; vnp = vnp->next)
  {
  	if (vnp->data.ptrvalue == sfp) 
  	{
  		return vnp->choice;
  	}
  }
  if (CodingRegionHasTranslExcept (sfp)) 
  {
  	return SeqLocLen (sfp->location) % 3;
  }
  return 0;
}

static void UpdateOneSequence (
  UpsDataPtr   udp,
  Int2         sfbval,
  Boolean      add_cit_subs,
  Boolean      update_proteins
)
{
  SeqAnnotPtr  sap = NULL;
  Uint2        entityID;
  SeqEntryPtr  sep;
  Boolean      update = FALSE;
  Boolean      feature_update = FALSE;
  ValNodePtr   prot_feat_list = NULL;

  if (udp != NULL)
  {
    if (GetStatus (udp->replace_all))
    {
  	  RemoveOldFeats (udp->oldbsp);
    }
  }

  if (update_proteins && udp != NULL) {
    prot_feat_list = FindProductProtRefs (udp->oldbsp);
    if (udp->transl_except_list != NULL)
    {
      ValNodeFree (udp->transl_except_list);
      udp->transl_except_list = NULL;
    }
    udp->transl_except_list = FindTranslExceptCDSs (udp->oldbsp);
  }

  if (sfbval == UPDATE_SEQUENCE_ONLY || sfbval == UPDATE_SEQUENCE_AND_FEATURES) {
    switch (udp->rmcval) {
      case UPDATE_REPLACE :
        if (ReplaceSequence (udp)) {
          update = TRUE;
        }
        break;
      case UPDATE_EXTEND5 :
	      if (NULL == udp->salp) {
	        if (Merge5PrimeNoOverlap (udp))
	          update = TRUE;
	      }
	      else 
	      {
	        if (Merge5Prime (udp))
	          update = TRUE;
        }
        break;
      case UPDATE_EXTEND3 :
	      if (NULL == udp->salp) {
	        if (Merge3PrimeNoOverlap (udp))
	          update = TRUE;
	      }
	      else 
	      {
	        if (Merge3Prime (udp))
	          update = TRUE;
        }
        break;
      case UPDATE_PATCH :
        if (PatchSequence (udp)) {
          update = TRUE;
        }
        break;
      default :
        break;
    }
    if ( sfbval == UPDATE_SEQUENCE_AND_FEATURES) {
      switch (udp->rmcval) {
        case UPDATE_REPLACE :
          if (DoFeaturePropWithOffset (udp, 0, &sap, FALSE)) {
            update = TRUE;
            feature_update = TRUE;
          }
          break;
        case UPDATE_EXTEND5 :
          if (DoFeaturePropWithOffset (udp, 0, &sap, FALSE)) {
            update = TRUE;
            feature_update = TRUE;
          }
          break;
        case UPDATE_EXTEND3 :
          if (DoFeaturePropWithOffset (udp, udp->old5 - udp->new5, &sap, FALSE)) {
            update = TRUE;
            feature_update = TRUE;
          }
          break;
        case UPDATE_PATCH :
          if (DoFeaturePropWithOffset (udp, udp->old5 - udp->new5, &sap, TRUE)) {
            update = TRUE;
            feature_update = TRUE;
          }
          break;
        default :
          break;
      }
    }
    if (udp->rmcval == UPDATE_REPLACE && udp->revcomp && update)
    {
      ReverseComplementBioseqAndFeats (udp->oldbsp, udp->input_entityID);
    }

  } else if (sfbval == UPDATE_FEATURES_ONLY) {
    switch (udp->rmcval) {
      case UPDATE_REPLACE :
        if (DoFeaturePropThruAlign (udp, &sap)) {
          update = TRUE;
          feature_update = TRUE;
        }
        break;
      case UPDATE_EXTEND5 :
        if (DoFeaturePropThruAlign (udp, &sap)) {
          update = TRUE;
          feature_update = TRUE;
        }
        break;
      case UPDATE_EXTEND3 :
        if (DoFeaturePropThruAlign (udp, &sap)) {
          update = TRUE;
          feature_update = TRUE;
        }
        break;
      case UPDATE_PATCH :
        if (DoFeaturePropThruAlign (udp, &sap)) {
          update = TRUE;
          feature_update = TRUE;
        }
        break;
      default :
        break;
    }
  }
  if (update) {
    entityID = ObjMgrGetEntityIDForPointer (udp->oldbsp);
    if (add_cit_subs
      && (feature_update || StringCmp (udp->seq1, udp->seq2) != 0))
    {
      AddCitSubToUpdatedSequence ( udp->oldbsp, entityID);
    }
    if (update_proteins)
    {
      SeqMgrClearFeatureIndexes (entityID, udp->oldbsp);
      SeqMgrIndexFeatures (entityID, NULL);
      sep = GetBestTopParentForData (entityID, udp->oldbsp);
      udp->truncate_proteins = GetStatus (udp->truncate_proteins_btn);
      udp->extend_proteins5 = GetStatus (udp->extend_proteins5_btn);
      udp->extend_proteins3 = GetStatus (udp->extend_proteins3_btn);
      udp->correct_cds_genes = GetStatus (udp->correct_cds_genes_btn);
      VisitFeaturesInSep (sep, udp, UpdateProteinsOnNewBsp);
      FixProtRefPtrs (prot_feat_list);
    }
    if (! udp->suppress_instant_refresh) {
      ObjMgrSetDirtyFlag (entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
    }
  }
  
  if (GetStatus (udp->update_quality_scores_btn) && udp->rmcval == UPDATE_REPLACE 
      && (sfbval == UPDATE_SEQUENCE_ONLY || sfbval == UPDATE_SEQUENCE_AND_FEATURES))
  {
    ReplaceQualityScores (udp->oldbsp, udp->newbsp, udp->salp, udp->log_fp, &(udp->data_in_log));
  }
  
  ValNodeFree (prot_feat_list);
  ValNodeFree (udp->transl_except_list);
  udp->transl_except_list = NULL;
  if (sfbval == UPDATE_FEATURES_ONLY || sfbval == UPDATE_SEQUENCE_AND_FEATURES) {
    if (update) {
      entityID = ObjMgrGetEntityIDForPointer (udp->oldbsp);
      sep = GetTopSeqEntryForEntityID (entityID);
      /* need to set scope to make sure we mark the right bioseq for deletion */
      SeqEntrySetScope (sep);
      /* resolve features unless the policy was to remove all the old ones */
      if (!GetStatus (udp->replace_all))
      {
        ResolveDuplicateFeats (udp, udp->oldbsp, sap);      	
      }
      SeqEntrySetScope (NULL);
      DeleteMarkedObjects (entityID, 0, NULL);
      SeqMgrClearFeatureIndexes (entityID, NULL);
      ObjMgrSetDirtyFlag (entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
    }
  }
}

static void UpdateProteinsOnNewBsp (SeqFeatPtr sfp, Pointer userdata)
{
  UpsDataPtr    udp_orig;
  SeqLocPtr     new_product;
  Boolean       data_in_log;
  Int4          transl_except_len = 0;
  Boolean       fix_products = TRUE;
  BioseqPtr     protBsp = NULL;
  SeqAlignPtr   salp = NULL;
  BioseqPtr     newbsp = NULL;
  Boolean       rval;

  if (sfp == NULL || sfp->idx.subtype != FEATDEF_CDS || userdata == NULL)
  {
    return;
  }
  
  if (sfp->idx.deleteme)
  {
    return;
  }
  
  protBsp = BioseqFindFromSeqLoc (sfp->product);
  if (protBsp == NULL)
  {
    return;
  }
  udp_orig = (UpsDataPtr) userdata;

  data_in_log = FALSE;
  transl_except_len = GetOriginalTranslExceptLen (sfp, udp_orig->transl_except_list);
  rval = PrepareUpdateAlignmentForProtein (sfp,
                                           protBsp,
                                           udp_orig->input_entityID,
                                           udp_orig->log_fp,
                                           udp_orig->truncate_proteins,
                                           udp_orig->extend_proteins5,
                                           udp_orig->extend_proteins3,
                                           udp_orig->correct_cds_genes,
                                           transl_except_len,
                                           &data_in_log,
                                           &salp,
                                           &newbsp);
  if (data_in_log) {
    udp_orig->data_in_log = TRUE;
  }
  if (!rval) return;
  
  if (protBsp->idx.deleteme)
  {
    fix_products = FALSE;
  }

  ReplaceOneSequence (salp, protBsp, newbsp);
  
  if (fix_products)
  {
    if (sfp->product->choice != SEQLOC_WHOLE) {
      new_product = SeqLocWholeNew (protBsp);
      if (new_product == NULL) return;
      SeqLocFree (sfp->product);
      sfp->product = new_product;
    }
    newbsp = BioseqFree (newbsp);
  }
}

extern void UpdateProteinsFromCDS ( IteM i)
{
  BaseFormPtr        bfp;
  SeqEntryPtr        sep;
  ProteinFromCDSData pfcd;
  Char               path [PATH_MAX];

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL)
    return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  pfcd.input_entityID = bfp->input_entityID;
  pfcd.input_itemID = bfp->input_itemID;
  pfcd.input_itemtype = bfp->input_itemtype;
  pfcd.data_to_report = FALSE;
  TmpNam (path);
  pfcd.fp = FileOpen (path, "wb");
  if (pfcd.fp == NULL) return;

  WatchCursor ();
  VisitFeaturesInSep (sep, (Pointer) &pfcd, UpdateOneProteinFromCDS);
  FileClose (pfcd.fp);
  if (pfcd.data_to_report) {
    LaunchGeneralTextViewer (path, "Protein Changes");
  }
  FileRemove (path);
  ArrowCursor ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void AcceptRMCOrExtend (ButtoN b)
{
  UpsDataPtr   udp;
  Int2         sfbval;
  Boolean      log_is_local;
  Boolean      update_proteins = FALSE;
  Uint2        entityID;

  udp = (UpsDataPtr) GetObjectExtra (b);
  if (udp == NULL) return;
  SafeHide (udp->form);

  sfbval = GetValue (udp->sfb);
  udp->rmcval = GetValue (udp->rmc);

  if (udp->log_fp == NULL) {
    OpenSequenceUpdateLog (udp);
    if (udp->log_fp == NULL) return;
    log_is_local = TRUE;
  } else {
    log_is_local = FALSE;
  }
  WatchCursor ();
  Update ();
  
  if (udp->do_update)
  {
    if (udp->update_proteins != NULL 
      && Enabled (udp->update_proteins)
      && GetStatus (udp->update_proteins))
    {
      update_proteins = TRUE;
    }
    UpdateOneSequence (udp, sfbval,
                       GetStatus (udp->add_cit_subs),
                       update_proteins);
  }
  else
  {
    if (ExtendOneSequence (udp))
    {
      entityID = ObjMgrGetEntityIDForPointer (udp->oldbsp);
      ObjMgrSetDirtyFlag (entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
    }
  }
  
  Remove (udp->form);
  if (log_is_local) {
    CloseOutSequenceUpdateLog (udp);
  }
  ArrowCursor ();
  Update();
}

static void DoAcceptRMCOrExtendSet (UpsDataPtr udp)
{
  Int2         sfbval;
  Boolean      log_is_local = FALSE;
  Boolean      update_proteins = FALSE;
  Boolean      do_update = TRUE;
  Boolean      old_useGUI;
  Char         acc_str [256];
  SeqIdPtr     sip, sip_next;
  Int2         prior_rmcval;

  if (udp == NULL) return;
  
  SafeHide (udp->form);
  old_useGUI = udp->useGUI;
  
  prior_rmcval = udp->rmcval;
  if (udp->do_update)
  {
    if (udp->salp == NULL 
        && (udp->no_aln_choice == UPDATE_REPLACE_THIS
         || udp->no_aln_choice == UPDATE_REPLACE_ALL))
    {
      sfbval = UPDATE_SEQUENCE_ONLY;
      udp->rmcval = UPDATE_REPLACE;
      udp->useGUI = FALSE;
    }
    else if (udp->salp == NULL
             && (udp->no_aln_choice == UPDATE_SKIP_THIS
              || udp->no_aln_choice == UPDATE_SKIP_ALL))
    {
      do_update = FALSE;
    }
    else
    {
      sfbval = GetValue (udp->sfb);
      udp->rmcval = GetValue (udp->rmc);
      prior_rmcval = udp->rmcval;
    }
  }
  else
  {
    udp->rmcval = GetValue (udp->rmc);
    prior_rmcval = udp->rmcval;
  }

  if (udp->log_fp == NULL) 
  {
    OpenSequenceUpdateLog (udp);
    if (udp->log_fp == NULL) return;
    log_is_local = TRUE;
  }
    
  if (do_update)
  {
    if (udp->do_update)
    {
      if (udp->update_proteins != NULL 
          && Enabled (udp->update_proteins)
          && GetStatus (udp->update_proteins))
      {
        update_proteins = TRUE;
      }

      UpdateOneSequence (udp, sfbval, GetStatus (udp->add_cit_subs),
                         update_proteins);
    }
    else
    {
      ExtendOneSequence (udp);
    }

    Remove (udp->form);
  }
  else
  {
    sip = SeqIdFindBest (udp->oldbsp->id, 0);
    if (sip != NULL)
    {
      sip_next = sip->next;
      SeqIdWrite (sip, acc_str, PRINTID_REPORT, sizeof (acc_str));
      fprintf (udp->log_fp, "Skipped update for %s\n", acc_str);
	    udp->data_in_log = TRUE;
      sip->next = sip_next;
    }
  }
  
  if (log_is_local) 
  {
    CloseOutSequenceUpdateLog (udp);
  }
  
  udp->useGUI = old_useGUI;
  udp->rmcval = prior_rmcval;
  /* if we are updating a set from a SeqSub, we don't want to free the SeqSub yet */
  if (udp->seqsubsep != NULL)
  {
    udp->newbsp = NULL;
  }
  FreeUdpFields (udp);
}

/*------------------------------------------------------------------*/
/*                                                                  */
/* AcceptRMCAll () -- Breaks out of the GUI interface and updates   */
/*                    all remaining sequences in the file without   */
/*                    user intervention.                            */
/*                                                                  */
/*------------------------------------------------------------------*/

static void AcceptRMCOrExtendAll (ButtoN b)

{
  UpsDataPtr   udp;
  Int2         state;
  Int4         count;
  Char         msgStr[256];

  /* Get current data */

  udp = (UpsDataPtr) GetObjectExtra (b);
  if (udp == NULL)
    return;

  /* Process the current sequence */

  WatchCursor ();
  udp->useGUI = FALSE;

  if (udp->log_fp == NULL) {
    OpenSequenceUpdateLog (udp);
    if (udp->log_fp == NULL) return;
  }
  
  
  udp->rmcval = GetValue (udp->rmc);

  DoAcceptRMCOrExtendSet (udp);

  /* Loop through the file, processing all others */

  state = FASTA_READ_OK;
  count = 0;

  while (FASTA_READ_OK == state) {
    count++;
    WatchCursor ();
    state = UpdateNextBioseqInFastaSet (udp);
    if (udp->useGUI)
      return;
  }

  CloseOutSequenceUpdateLog (udp);
  ArrowCursor ();

  /* If there was an error, report it, otherwise */
  /* print a status message.                     */

  if (FASTA_READ_ERROR == state) {
    sprintf (msgStr, "Encountered error while updating.  Only %d sequences "
	     "were updated.", count);
    Message (MSG_OK, msgStr);
  }
  else {
    sprintf (msgStr, "Successfully processed %d sequences from file (not"
	     " counting any updated before hitting 'Accept All').", count);
    Message (MSG_OK, msgStr);
  }
}


static void AcceptRMCOrExtendSet (ButtoN b)

{
  UpsDataPtr   udp;

  udp = (UpsDataPtr) GetObjectExtra (b);
  if (udp == NULL)
    return;
  
  if (udp->log_fp == NULL) {
    OpenSequenceUpdateLog (udp);
    if (udp->log_fp == NULL) return;
  }

  udp->rmcval = GetValue (udp->rmc);
  
  DoAcceptRMCOrExtendSet (udp);

  UpdateNextBioseqInFastaSet (udp);
}

static void SetProteinOptionsEnable (UpsDataPtr udp)
{
  if (udp == NULL || udp->update_proteins == NULL) return;
  
  if (Enabled (udp->update_proteins) && GetStatus (udp->update_proteins))
  {
    Enable (udp->truncate_proteins_btn);
    Enable (udp->extend_proteins3_btn);
    Enable (udp->extend_proteins5_btn);
    Enable (udp->correct_cds_genes_btn);
  }
  else
  {
    Disable (udp->truncate_proteins_btn);
    Disable (udp->extend_proteins3_btn);
    Disable (udp->extend_proteins5_btn);
    Disable (udp->correct_cds_genes_btn);
  }
}

static void SetStatusUpdateAcceptBtns (UpsDataPtr udp, Boolean status)
{
  if (udp == NULL) return;
  
  if (status)
  {
    SafeEnable (udp->accept);
    SafeEnable (udp->acceptAll);
  }
  else
  {
    SafeDisable (udp->accept);
    SafeDisable (udp->acceptAll);
  }    
}

static void UpdateAccept (GrouP g)

{
  Int2        rmcval, sfbval;
  UpsDataPtr  udp;
  Int2        nobmval = UPDATE_FEAT_DUP_NOT_SET;

  udp = (UpsDataPtr) GetObjectExtra (g);
  if (udp == NULL) return;

  rmcval = GetValue (udp->rmc);
  sfbval = GetValue (udp->sfb);
  
  if (rmcval == UPDATE_REPLACE)
  {
    sfbval = GetValue (udp->sfb);
    if (sfbval == UPDATE_SEQUENCE_ONLY || sfbval == UPDATE_SEQUENCE_AND_FEATURES)
    {
      Enable (udp->update_quality_scores_btn);
    }
    else
    {
      Disable (udp->update_quality_scores_btn);
    }
  }
  else
  {
    Disable (udp->update_quality_scores_btn);
  }
  
  if (sfbval == UPDATE_FEATURES_ONLY || sfbval == UPDATE_SEQUENCE_AND_FEATURES)
  {
    if (!GetStatus (udp->replace_all))
    {
      nobmval = GetValue (udp->nobm);
    }
  }
  
  /* set enables for protein updates */
  if (sfbval == UPDATE_SEQUENCE_ONLY
     || (sfbval == UPDATE_SEQUENCE_AND_FEATURES
        && (nobmval == UPDATE_FEAT_DUP_USE_OLD || nobmval == UPDATE_FEAT_DUP_USE_BOTH)))
  {
    SafeEnable (udp->update_proteins);
  }
  else
  {
    SafeDisable (udp->update_proteins);
  }
  SetProteinOptionsEnable (udp);    

  if (rmcval == UPDATE_CHOICE_NOT_SET) {
    SetStatusUpdateAcceptBtns (udp, FALSE);
    return;
  }
  if (! udp->do_update) {
    SetStatusUpdateAcceptBtns (udp, TRUE);
    return;
  }
    
  if (sfbval == UPDATE_CHOICE_NOT_SET || sfbval == UPDATE_SEQUENCE_ONLY || udp->diffOrgs) {
    SafeDisable (udp->keepProteinIDs);
    if (sfbval == UPDATE_CHOICE_NOT_SET || sfbval == UPDATE_SEQUENCE_ONLY) {
      SafeDisable (udp->nobm);
      SafeDisable (udp->replace_all);
    } else {
      SafeEnable (udp->replace_all);
      if (GetStatus (udp->replace_all))
      {
        SafeDisable (udp->nobm);     	
      }
      else
      {
      	SafeEnable (udp->nobm);
      }
    }
  } else if (sfbval == UPDATE_FEATURES_ONLY || sfbval == UPDATE_SEQUENCE_AND_FEATURES) {
    SafeEnable (udp->keepProteinIDs);
    SafeEnable (udp->replace_all);
    if (GetStatus (udp->replace_all))
    {
      SafeDisable (udp->nobm);      	
    }
    else
    {
      SafeEnable (udp->nobm);
    }
  }
  if (sfbval == UPDATE_CHOICE_NOT_SET) {
    SetStatusUpdateAcceptBtns (udp, FALSE);
    return;
  }
  if (sfbval == UPDATE_FEATURES_ONLY || sfbval == UPDATE_SEQUENCE_AND_FEATURES) {
    if (!GetStatus (udp->replace_all) && nobmval == UPDATE_FEAT_DUP_NOT_SET)
    {
      SetStatusUpdateAcceptBtns (udp, FALSE);
      return;
    }
  }

  SetStatusUpdateAcceptBtns (udp, TRUE);
}

static void UpdateButtons (GrouP g)
{
  UpsDataPtr        udp;
  Int2              rmcval;
  Uint2             entityID;
  SeqFeatPtr        sfp;
  SeqMgrFeatContext fcontext;

  udp = (UpsDataPtr) GetObjectExtra (g);
  if (udp == NULL) return;

  rmcval = GetValue (udp->rmc);

  if (udp->new5 <= udp->old5 && udp->new3 <= udp->old3) 
  {
    if (rmcval == UPDATE_PATCH)
    {
      /* If patch sequence matches, must be feature propagation only */

      if (StringNICmp (udp->seq1 + udp->old5 - udp->new5,
                       udp->seq2,
                       StringLen (udp->seq2)) == 0) {
        SetValue (udp->sfb, UPDATE_FEATURES_ONLY);
        Disable (udp->sfb);
      }
    }
    else if (rmcval == UPDATE_REPLACE)
    {
      /* If no features, must be sequence update only */

      entityID = ObjMgrGetEntityIDForPointer (udp->newbsp);
      if (! SeqMgrFeaturesAreIndexed (entityID))
        SeqMgrIndexFeatures (entityID, NULL);

      sfp = SeqMgrGetNextFeature (udp->newbsp, NULL, 0, 0, &fcontext);
      if (sfp == NULL) 
      {
        SetValue (udp->sfb, UPDATE_SEQUENCE_ONLY);
        Disable (udp->sfb);
        Disable (udp->replace_all);
        Disable (udp->nobm);
      }
      else if (!indexerVersion &&
               (udp->newbsp->repr != Seq_repr_raw || udp->oldbsp->repr != Seq_repr_raw))
      {
        SetValue (udp->sfb, UPDATE_FEATURES_ONLY);
        Disable (udp->sfb);
      }
      else
      {
        Enable (udp->sfb);
      }
    }
  }
  UpdateAccept (g);
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

static void DrawAlignDiffs (
  UpsDataPtr udp,
  SegmenT pict,
  Int4 top,
  Int4 bottom,
  SeqAlignPtr sap
)

{
  AlnMsg2Ptr  amp1, amp2;
  SegmenT    seg;
  Int4       len1, len2, i;
  Int4       seg_i, seg_n, seg_start, seg_stop;

  if (udp->seq1 == NULL || udp->seq2 == NULL) return;
  len1 = StringLen (udp->seq1);
  len2 = StringLen (udp->seq2);

  seg = CreateSegment (pict, 0, 0);
  AddAttribute (seg, COLOR_ATT, RED_COLOR, 0, 0, 0, 0);

  seg_n = AlnMgr2GetNumSegs(sap);
  for (seg_i = 1; seg_i<=seg_n; seg_i++) {
    AlnMgr2GetNthSegmentRange(sap, seg_i, &seg_start, &seg_stop);

    amp1 = AlnMsgNew2 ();
    amp2 = AlnMsgNew2 ();
    if (amp1 == NULL || amp2 == NULL) return;

    amp1->from_aln = seg_start;
    amp1->to_aln = seg_stop;
    amp1->row_num = 1;

    amp2->from_aln = seg_start;
    amp2->to_aln = seg_stop;
    amp2->row_num = 2;

    AlnMgr2GetNextAlnBit (sap, amp1);
    AlnMgr2GetNextAlnBit (sap, amp2);

    if (amp1->to_row - amp1->from_row == amp2->to_row - amp2->from_row &&
        amp1->type == AM_SEQ && amp2->type == AM_SEQ) {
      for (i=0; i<seg_stop-seg_start+1; i++) {
        if (udp->seq1[amp1->from_row+i] != udp->seq2[amp2->from_row+i]) {

          /* record for accurate scrolling to text view */
          ValNodeAddInt (&(udp->mismatches), 0, i);

          AddLine (seg, seg_start+i, top, seg_start+i, bottom, FALSE, 0);
        }
      }
    }

    AlnMsgFree2 (amp1);
    AlnMsgFree2 (amp2);
  }
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
  AlnMsg2Ptr  amp;
  Int4       len, start, stop, from, to;
  Char       str [96];
  Boolean    wasgap;

  amp = AlnMsgNew2 ();
  if (amp == NULL) return;

  amp->from_aln = 0;
  amp->to_aln = -1;
  amp->row_num = row;

  start = 0;
  stop = 0;
  from = 0;
  to = 0;
  wasgap = FALSE;

  while (AlnMgr2GetNextAlnBit (sap, amp)) {
    len = amp->to_row - amp->from_row + 1;
    stop = start + len;
    if (amp->type == AM_GAP) {
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

  AlnMsgFree2 (amp);

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
  AlnMsg2Ptr  amp;
  Int4       aln_length, len, lens, start, stop, from, to, i, j;

  if (udp == NULL || sap == NULL || seq == NULL || udp->aln_length < 1) return NULL;
  lens = StringLen (seq);

  aln = (CharPtr) MemNew (sizeof (Char) * (udp->aln_length + 2));
  if (aln == NULL) return NULL;
  aln_length = udp->aln_length;
  MemSet ((Pointer) aln, '-', aln_length);

  amp = AlnMsgNew2 ();
  if (amp == NULL) return aln;

  amp->from_aln = 0;
  amp->to_aln = -1;
  amp->row_num = row;

  start = 0;
  stop = 0;
  from = 0;
  to = 0;

  while (AlnMgr2GetNextAlnBit (sap, amp)) {
    len = amp->to_row - amp->from_row + 1;
    stop = start + len;

    if (amp->type == AM_SEQ) {
      for (i = start, j = amp->from_row; i < stop && j < lens; i++, j++) {
        aln [i] = seq [j];
      }
    }
    start += len;
  }

  AlnMsgFree2 (amp);

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
  aln_length = AlnMgr2GetAlnLength (sap, FALSE);
  AlnMgr2GetNthSeqRangeInSA (sap, 1, &startold, &stopold);
  AlnMgr2GetNthSeqRangeInSA (sap, 2, &startnew, &stopnew);
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

  udp->strandold = AlnMgr2GetNthStrand (sap, 1);
  udp->strandnew = AlnMgr2GetNthStrand (sap, 2);

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

  if (udp->aln2 != NULL)
  {
    MoveTo (r.left, r.top + 8 + 3 * udp->lineheight);
    for (i = 0, j = offset; i < udp->maxchars && j < udp->aln_length; i++, j++) {
      PaintChar (udp->aln2 [j]);
    }
  }

  /* draw bottom (old) letters */

  if (udp->aln1 != NULL) 
  {
    MoveTo (r.left, r.top + 8 + 5 * udp->lineheight);
    for (i = 0, j = offset; i < udp->maxchars && j < udp->aln_length; i++, j++) {
      PaintChar (udp->aln1 [j]);
    }
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

  if (udp->aln1 == NULL || udp->aln2 == NULL) 
  {
  	return;
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
  pos = AlnMgr2MapSeqAlignToBioseq (udp->salp, j, 2);
  while (pos < 1 && i < udp->maxchars && j < udp->aln_length) {
    i++;
    j++;
    pos = AlnMgr2MapSeqAlignToBioseq (udp->salp, j, 2);
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
  pos = AlnMgr2MapSeqAlignToBioseq (udp->salp, j, 1);
  while (pos < 1 && i < udp->maxchars && j < udp->aln_length) {
    i++;
    j++;
    pos = AlnMgr2MapSeqAlignToBioseq (udp->salp, j, 1);
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

static void FreeUdpFields (UpsDataPtr udp)
{
  Uint2       entityID;

  udp->ovpict     = DeletePicture (udp->ovpict);
  udp->dtpict     = DeletePicture (udp->dtpict);
  udp->salp       = SeqAlignFree (udp->salp);
  entityID        = ObjMgrGetEntityIDForPointer (udp->newbsp);
  udp->newbsp     = ObjMgrFreeByEntityID (entityID);
  udp->seq1       = MemFree (udp->seq1);
  udp->seq2       = MemFree (udp->seq2);
  udp->aln1       = MemFree (udp->aln1);
  udp->aln1       = NULL;
  udp->aln2       = MemFree (udp->aln2);
  udp->aln2       = NULL;
  udp->indels     = ValNodeFree (udp->indels);
  udp->mismatches = ValNodeFree (udp->mismatches);
  udp->transl_except_list = ValNodeFree (udp->transl_except_list);
}

static void CleanupUpdateSequenceForm (GraphiC g, VoidPtr data)

{
  UpsDataPtr  udp;

  udp = (UpsDataPtr) data;
  if (udp != NULL)
    FreeUdpFields (udp);
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

/*------------------------------------------------------------------*/
/*                                                                  */
/* DetermineButtonState () -- Enable/disable buttons based on the   */
/*                            nature of the alignment.              */
/*                                                                  */
/*------------------------------------------------------------------*/

static void DetermineButtonState (UpsDataPtr   udp,
				  ButtoN PNTR  replaceButtonPtr,
				  ButtoN PNTR  extend5ButtonPtr,
				  ButtoN PNTR  extend3ButtonPtr,
				  ButtoN PNTR  patchButtonPtr)
{
  BioSourcePtr       biop1;
  BioSourcePtr       biop2;
  SeqMgrDescContext  dcontext;
  Uint2              entityID;
  SeqMgrFeatContext  fcontext;
  OrgRefPtr          orp1;
  OrgRefPtr          orp2;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;

  /* If no alignment then disable the patch button */

  if (udp->salp == NULL) {
    if (udp->do_update && patchButtonPtr != NULL && *patchButtonPtr != NULL) {
      Disable (*patchButtonPtr);
    }
    if (!udp->do_update)
    {
      Disable (*replaceButtonPtr);
    }
  }

  /* Extend 5' */

  else if (udp->new5 > udp->old5 && udp->new3 < udp->old3) {
    SetValue (udp->rmc, UPDATE_EXTEND5);
    Disable (*extend3ButtonPtr);
    udp->recomb2 = udp->aln_length;
    if (! udp->do_update) {
      Disable (*replaceButtonPtr);
    }
  }

  /* Extend 3' */

  else if (udp->new5 < udp->old5 && udp->new3 > udp->old3) {
    SetValue (udp->rmc, UPDATE_EXTEND3);
    Disable (*extend5ButtonPtr);
    udp->recomb1 = 0;
    if (! udp->do_update) {
      Disable (*replaceButtonPtr);
    }
  }

  /* Replace */

  else {
    SetValue (udp->rmc, UPDATE_REPLACE);
    Disable (*extend5ButtonPtr);
    Disable (*extend3ButtonPtr);
    udp->recomb1 = 0;
    udp->recomb2 = udp->aln_length;
  }
  
  switch (udp->rmcval)
  {
    case UPDATE_REPLACE:
      if (Enabled (*replaceButtonPtr))
      {
        SetValue (udp->rmc, UPDATE_REPLACE);
      }
      break;
    case UPDATE_EXTEND5:
      if (Enabled (*extend5ButtonPtr))
      {
        SetValue (udp->rmc, UPDATE_EXTEND5);
      }
      break;
    case UPDATE_EXTEND3:
      if (Enabled (*extend3ButtonPtr))
      {
        SetValue (udp->rmc, UPDATE_EXTEND3);
      }
      break;
    case UPDATE_PATCH:
      if (Enabled (*patchButtonPtr))
      {
        SetValue (udp->rmc, UPDATE_PATCH);
      }
      break;
  }

  /* If no features, must be sequence update only */

  entityID = ObjMgrGetEntityIDForPointer (udp->newbsp);
  if (! SeqMgrFeaturesAreIndexed (entityID))
    SeqMgrIndexFeatures (entityID, NULL);

  sfp = SeqMgrGetNextFeature (udp->newbsp, NULL, 0, 0, &fcontext);
  if (sfp == NULL) {
    SetValue (udp->sfb, UPDATE_SEQUENCE_ONLY);
    Disable (udp->sfb);
    Disable (udp->replace_all);
    Disable (udp->nobm);
  }

  /* If different organisms, must be feature propagation only */

  orp1 = NULL;
  orp2 = NULL;
  sdp = SeqMgrGetNextDescriptor (udp->oldbsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop1 = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop1 != NULL) {
      orp1 = biop1->org;
    }
  }
  sdp = SeqMgrGetNextDescriptor (udp->newbsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop2 = (BioSourcePtr) sdp->data.ptrvalue;
    if (biop2 != NULL) {
      orp2 = biop2->org;
    }
  }
  if (orp1 != NULL && orp2 != NULL) {
    if (StringICmp (orp1->taxname, orp2->taxname) != 0) {
      if (sfp != NULL) {
        SetValue (udp->sfb, UPDATE_FEATURES_ONLY);
        Disable (udp->sfb);
        udp->diffOrgs = TRUE;
	if (FALSE == udp->isSet)
	  Message (MSG_OK, "Organisms are different, so features will"
		   " be propagated, but sequence will not be changed");
      } else {
        /* no features, cannot do anything */
        SetValue (udp->sfb, UPDATE_CHOICE_NOT_SET);
        Disable (udp->sfb);
        Disable (udp->replace_all);
        Disable (udp->nobm);
	if (FALSE == udp->isSet)
	  Message (MSG_OK, "Organisms are different, no features"
		   " to propagate, so nothing to do");
      }
      Disable (udp->rmc);
    }
  }

  /* If either sequence is not raw and not indexer version, only allow feature propagation */
  if (!indexerVersion &&
      (udp->oldbsp->repr != Seq_repr_raw || udp->newbsp->repr != Seq_repr_raw)) {
    SetValue (udp->sfb, UPDATE_FEATURES_ONLY);
    Disable (udp->sfb);
  }

  /* Disable accept button unless rmc and sfb are both preset */

  UpdateAccept (udp->rmc);

}

static void ChangeUpdateReplaceAll (ButtoN b)
{
  UpsDataPtr udp;
  
  if (b == NULL) return;
  udp = (UpsDataPtr) GetObjectExtra (b);
  if (udp == NULL) return;
 
  if (GetStatus (b))
  {
  	Disable (udp->nobm);
  }
  else
  {
  	Enable (udp->nobm);
  }
  /* update accept button */
  UpdateAccept (udp->rmc);
}

static void CancelUpdate (ButtoN b)
{
  UpsDataPtr udp;
  
  if (b == NULL) return;
  udp = (UpsDataPtr) GetObjectExtra (b);
  if (udp == NULL) return;
  CloseOutSequenceUpdateLog (udp);
  StdCancelButtonProc (b);
}

static GrouP CreateUpdateOperationsGroup (GrouP parent, UpsDataPtr udp)
{
  GrouP  g;
  GrouP  gp1, gp2, gp3;
  ButtoN b1 = NULL, b2 = NULL, b3 = NULL, b4 = NULL;
  
  if (udp == NULL) return NULL;
  
  g = HiddenGroup (parent, -1, 0, NULL);
  SetGroupSpacing (g, 5, 5);
  
  gp1 = NormalGroup (g, 4, 0, "Alignment Relationship", programFont, NULL);
  udp->rmc = HiddenGroup (gp1, 4, 0, UpdateButtons);
  SetObjectExtra (udp->rmc, (Pointer) udp, NULL);
  SetGroupSpacing (udp->rmc, 10, 5);
  if (udp->do_update) {
    b1 = RadioButton (udp->rmc, "Replace");
  } else {
    b1= RadioButton (udp->rmc, "Extend Both Ends");
  }
  b2 = RadioButton (udp->rmc, "Extend 5'");
  b3 = RadioButton (udp->rmc, "Extend 3'");
  if (udp->do_update) {
    b4 = RadioButton (udp->rmc, "Patch");
  } else {
    b4 = NULL;
  }

  if (udp->do_update) {
    gp2 = NormalGroup (g, 4, 0, "Update Operation", programFont, NULL);
    udp->sfb = HiddenGroup (gp2, 3, 0, UpdateAccept);
    SetObjectExtra (udp->sfb, (Pointer) udp, NULL);
    SetGroupSpacing (udp->sfb, 10, 5);
    RadioButton (udp->sfb, "Sequence");
    RadioButton (udp->sfb, "Features");
    RadioButton (udp->sfb, "Sequence + Features");
  
    udp->keepProteinIDs = CheckBox (g, "Keep Protein IDs", NULL);
  
    gp3 = NormalGroup (g, 1, 0, "Feature Policy", programFont, NULL);
    udp->replace_all = CheckBox (gp3, "Replace All Features", ChangeUpdateReplaceAll);
    SetObjectExtra (udp->replace_all, (Pointer) udp, NULL);
    SetValue (udp->replace_all, FALSE);
    
    udp->nobm = NormalGroup (gp3, 5, 0, "Duplicate Features Only", programFont, UpdateAccept);
    SetObjectExtra (udp->nobm, (Pointer) udp, NULL);
    SetGroupSpacing (udp->nobm, 10, 5);
    RadioButton (udp->nobm, "New");
    RadioButton (udp->nobm, "Old");
    RadioButton (udp->nobm, "Both");
    RadioButton (udp->nobm, "Merge");
    RadioButton (udp->nobm, "Replace");
  
    AlignObjects (ALIGN_CENTER, (HANDLE) gp1, (HANDLE) gp2, 
                  (HANDLE) udp->keepProteinIDs,
                  (HANDLE) gp3,  NULL);
  }
  /* Enable/disable buttons based on the nature of the alignment */
    
  DetermineButtonState (udp, &b1, &b2, &b3, &b4);
  return g;
  
}


static void ChangeProteinUpdateStatus (ButtoN b)
{
  UpsDataPtr udp;
  
  udp = (UpsDataPtr) GetObjectExtra (b);
  if (udp == NULL) return;
  SetProteinOptionsEnable (udp);
}

static void SkipUpdate (UpsDataPtr udp)
{
  Char     acc_str [256];
  SeqIdPtr sip, sip_next;
  
  if (udp == NULL) return;

  OpenSequenceUpdateLog (udp);
  if (udp->log_fp != NULL && udp->oldbsp != NULL && udp->oldbsp->id != NULL)
  {
    sip = SeqIdFindBest (udp->oldbsp->id, 0);
    if (sip != NULL)
    {
      sip_next = sip->next;
      sip->next = NULL;
      SeqIdWrite (sip, acc_str, PRINTID_REPORT, sizeof (acc_str));
      fprintf (udp->log_fp, "Skipped update for %s\n", acc_str);
	    udp->data_in_log = TRUE;
	    sip->next = sip_next;
    }
  }
  
  /* if we are updating a set from a SeqSub, we don't want to free the SeqSub yet */
  if (udp->seqsubsep != NULL)
  {
    udp->newbsp = NULL;
  }
  FreeUdpFields (udp);
  UpdateNextBioseqInFastaSet (udp); 
}

static GrouP CreateExtraUpdateOptionsGroup (GrouP g, UpsDataPtr udp)
{
  GrouP y, protein_options;
  
  if (udp == NULL || g == NULL) return NULL;

  y = HiddenGroup (g, -1, 0, NULL);
  
  udp->add_cit_subs = CheckBox (y, "Add Cit-subs for Updated Sequences", NULL);
  udp->update_quality_scores_btn = NULL;
  if (! ISA_aa (udp->oldbsp->mol) && udp->do_update)
  {
    udp->update_quality_scores_btn = CheckBox (y, "Replace Quality Scores", NULL);
    SetStatus (udp->update_quality_scores_btn, TRUE);

    udp->update_proteins = CheckBox (y, "Update Proteins for Updated Sequences", ChangeProteinUpdateStatus);
    SetObjectExtra (udp->update_proteins, (Pointer) udp, NULL);
    SetStatus (udp->update_proteins, FALSE);
    protein_options = HiddenGroup (y, 1, 0, NULL);
    udp->truncate_proteins_btn = CheckBox (protein_options,
                                     "Truncate retranslated proteins at stops",
                                           NULL);
    SetStatus (udp->truncate_proteins_btn,
               udp->truncate_proteins);
    udp->extend_proteins3_btn = CheckBox (protein_options,
                                  "Extend retranslated proteins without stops",
                                         NULL);
    udp->extend_proteins5_btn = CheckBox (protein_options,
                                  "Extend retranslated proteins without starts",
                                         NULL);
    udp->correct_cds_genes_btn = CheckBox (protein_options, "Correct CDS genes", NULL);
  
    SetStatus (udp->extend_proteins3_btn, udp->extend_proteins3);
    SetStatus (udp->extend_proteins5_btn, udp->extend_proteins5);
    SetStatus (udp->correct_cds_genes_btn, udp->correct_cds_genes);
  }
  AlignObjects (ALIGN_CENTER, (HANDLE) udp->add_cit_subs, 
                (HANDLE) udp->update_quality_scores_btn,
                (HANDLE) udp->update_proteins,
                (HANDLE) protein_options,
                NULL);
  return y;
}

static void SkipUpdateBtn (ButtoN b)
{
  UpsDataPtr udp;
  
  udp = (UpsDataPtr) GetObjectExtra (b);
  if (udp == NULL) return;
  SafeHide (udp->form);
  Remove (udp->form);

  SkipUpdate (udp);  
}

/*------------------------------------------------------------------*/
/*                                                                  */
/* UpdateSequenceForm () -- Compares two sequences and displays a   */
/*                          window giving the user options on how   */
/*                          to update one from the other.           */
/*                                                                  */
/*------------------------------------------------------------------*/

static ForM UpdateSequenceForm (UpsDataPtr udp)
{
  ButtoN             b;
  GrouP              c, g, y, k, x, z = NULL;
  Uint2              hgt;
  GrouP              ppt0, ppt1, ppt2, ppt3;
  RecT               r;
  BaR                sb;
  Int4               scaleX;
  SeqIdPtr           sip;
  Char               strid1 [MAX_ID_LEN], strid2 [MAX_ID_LEN], txt0 [256];
  CharPtr            title;
  WindoW             w;
  GrouP              misc_options;
  Int4               prompt_width = 400;
  GrouP              left_panel;
  GrouP              right_panel;

  /* Check parameters */

  if ((udp->oldbsp == NULL) || (udp->newbsp == NULL))
    return NULL;

  /* Create window */

  if (udp->do_update) {
    title = "Update Sequence";
  } else {
    title = "Extend Sequence";
  }
  w = FixedWindow (-50, -33, -10, -10, title, NULL);

  if (w == NULL)
    return NULL;
  
  if (FALSE == udp->isSet)
    SetObjectExtra (w, (Pointer) udp, CleanupUpdateSequenceForm);
  udp->form = (ForM) w;
  
  /* Get string IDs for the Bioseqs */

  sip = SeqIdFindWorst (udp->oldbsp->id);
  SeqIdWrite (sip, strid1, PRINTID_REPORT, sizeof (strid1) - 1);
  sip = SeqIdFindWorst (udp->newbsp->id);
  SeqIdWrite (sip, strid2, PRINTID_REPORT, sizeof (strid2) - 1);
  if (StringNICmp (strid2, "SequinUpdateSequence", 20) == 0 &&
      udp->newbsp->id->next != NULL) {
    sip = SeqIdFindWorst (udp->newbsp->id->next);
    SeqIdWrite (sip, strid2, PRINTID_REPORT, sizeof (strid2) - 1);
  }

  /* FIll in some of the data structure */
  /* for passing to the callbacks.      */

  udp->formmessage = UpdateSequenceFormMessage;

#ifdef WIN_MAC
  udp->activate = UpdateSequenceFormActivated;
  SetActivate (w, UpdateSequenceFormActivate);
#endif

  udp->diffOrgs = FALSE;

  CalculateOverhangs (udp);

  /* Display the sequences */

  sprintf (txt0,
	   "New sequence: %s - Length: %ld\nOld Sequence: %s - Length: %ld",
	   strid2, (long) udp->newbsp->length, strid1,
	   (long) udp->oldbsp->length);
  ppt0 = MultiLinePrompt (w, txt0, prompt_width, programFont);
  
  x = HiddenGroup (w, 2, 0, NULL);
  left_panel = HiddenGroup (x, -1, 0, NULL);
  y = left_panel;
  
  ppt1 = MultiLinePrompt (y, txt1, prompt_width, programFont);
  udp->overview = CreateViewer (y, prompt_width + Nlm_vScrollBarWidth, 100,
				FALSE, FALSE);
  
  ppt2 = MultiLinePrompt (y, txt2, prompt_width, programFont);
  udp->details = CreateViewer (y, prompt_width + Nlm_vScrollBarWidth, 80,
			       FALSE, FALSE);
  
  ppt3 = MultiLinePrompt (y, txt3, prompt_width, programFont);
    
#ifdef WIN_MAC
  hgt = 90;
#else
  hgt = 110;
#endif
  udp->letters = AutonomousPanel4 (y, prompt_width + Nlm_vScrollBarWidth, hgt,
				   LetDraw, NULL, LetScrl, 0, NULL, NULL);
  SetObjectExtra (udp->letters, (Pointer) udp, NULL);
  
  if (indexerVersion && shftKey) {
    ButtoN  b;
    z = HiddenGroup (y, 2, 0, NULL);
    SetGroupSpacing (z, 10, 3);
    b = PushButton (z, "Print Graphic", PrintGAln);
    SetObjectExtra (b, (Pointer) udp, NULL);
    b = PushButton (z, "Display Alignment", PrintTAln);
    SetObjectExtra (b, (Pointer) udp, NULL);
  }
  
  udp->ovpict = NULL;
  udp->dtpict = NULL;

  AlignObjects (ALIGN_CENTER, (HANDLE) udp->overview,
                (HANDLE) udp->details, (HANDLE) udp->letters,
                (HANDLE) ppt1, (HANDLE) ppt2,
                (HANDLE) ppt3, NULL);

  right_panel = HiddenGroup (x, -1, 0, NULL);
  y = right_panel;

  k = HiddenGroup (y, -1, 0, NULL);
  g = CreateUpdateOperationsGroup (k, udp);
  misc_options = CreateExtraUpdateOptionsGroup (k, udp);  
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) misc_options, NULL);
  
  c = HiddenGroup (w, 5, 0, NULL);
  if (udp->isSet)
  {
    udp->accept = DefaultButton (c, "Accept", AcceptRMCOrExtendSet);
    SetObjectExtra (udp->accept, (Pointer) udp, NULL);
    udp->acceptAll = DefaultButton (c, "Accept All", AcceptRMCOrExtendAll);
    SetObjectExtra (udp->acceptAll, (Pointer) udp, NULL);
    b = PushButton (c, "Skip", SkipUpdateBtn);
    SetObjectExtra (b, (Pointer) udp, NULL);  
  }
  else
  {
    udp->accept = DefaultButton (c, "Accept", AcceptRMCOrExtend);
    SetObjectExtra (udp->accept, (Pointer) udp, NULL);
  }
  
  b = PushButton (c, "Cancel", CancelUpdate);
  SetObjectExtra (b, (Pointer) udp, NULL);  
  UpdateButtons (udp->rmc);

  AlignObjects (ALIGN_CENTER,     
                (HANDLE) ppt0,
                (HANDLE) x,
                (HANDLE) c, (HANDLE) z, NULL);
  RealizeWindow (w);
  
  udp->ovpict = MakeAlignPicture (udp, strid1, strid2, udp->salp);
  scaleX = CalculateBestScale (udp, udp->overview, udp->ovpict);
  AttachPicture (udp->overview, udp->ovpict, 0, 0, UPPER_LEFT,
		 scaleX, 1, FrameVwr);
  
  udp->dtpict = MakeAlignDetails (udp, strid1, strid2, udp->salp);
  scaleX = CalculateBestScale (udp, udp->details, udp->dtpict);
  udp->scaleX = scaleX;
  AttachPicture (udp->details, udp->dtpict, 0, 0, UPPER_LEFT,
		 scaleX, 1, FrameVwr);
  SetViewerData (udp->details, (Pointer) udp, NULL);
  SetViewerProcs (udp->details, DtlClck, NULL, NULL, NULL);
  
  udp->indels = ValNodeSort (udp->indels, SortVnpByInt);
  udp->mismatches = ValNodeSort (udp->mismatches, SortVnpByInt);
  
  SelectFont (SetSmallFont ());
  ObjectRect (udp->letters, &r);
  InsetRect (&r, 4, 4);
  udp->lineheight = LineHeight ();
  udp->charwidth = MaxCharWidth ();
  udp->maxchars = (r.right-r.left-2+udp->charwidth - 1) / udp->charwidth;
  SelectFont (systemFont);
  
  sb = GetSlateHScrollBar ((SlatE) udp->letters);
  SetBarMax (sb, udp->aln_length - (Int4) udp->maxchars);
  CorrectBarPage (sb, (Int4) udp->maxchars - 1, (Int4) udp->maxchars - 1);
  
  udp->recomb1 = -1;
  udp->recomb2 = -1;
  
  return (ForM) w;
}


/*=====================================================================*/
/*                                                                     */
/* PrepareToUpdateSequences ()                                         */
/*                                                                     */
/*=====================================================================*/

static Boolean PrepareToUpdateSequences (UpsDataPtr udp)
{
  ForM         f;

  if ( ! PrepareUpdatePtr (udp)) 
  {
    CloseOutSequenceUpdateLog (udp);
    return FALSE;	
  }

  if (TRUE == udp->useGUI)
  {
    if (udp->salp == NULL && udp->do_update &&
        (udp->no_aln_choice == UPDATE_REPLACE_THIS
         || udp->no_aln_choice == UPDATE_SKIP_THIS
         || udp->no_aln_choice == UPDATE_REPLACE_ALL
         || udp->no_aln_choice == UPDATE_REPLACE_THIS)) 
    {
      CalculateOverhangs (udp);
      DoAcceptRMCOrExtendSet (udp);
      UpdateNextBioseqInFastaSet (udp);
    }
    else
    {
      f = UpdateSequenceForm (udp);
      if (f == NULL) 
      {
        CloseOutSequenceUpdateLog (udp);	
        return FALSE;
      }
      Show (f);
      Select (f);
      SendHelpScrollMessage (helpForm, "Edit Menu", "Update Sequence");
    }
  }
  else {
    CalculateOverhangs (udp);
    DoAcceptRMCOrExtendSet (udp);
  }
  return TRUE;
}

/*=====================================================================*/
/*                                                                     */
/* FindMatchingBioseq () -- Callback function for exploring Bioseqs.   */
/*                          Finds the bioseq that matches a given      */
/*                          string ID.                                 */
/*                                                                     */
/*=====================================================================*/

static Boolean LIBCALLBACK FindMatchingBioseq (BioseqPtr bsp,
					SeqMgrBioseqContextPtr bContext)
{
  Char          currentId[MAX_ID_LEN];
  UpdateDataPtr pUpdateData;
  Int2          result;
  SeqIdPtr      sip, sip_next;

  pUpdateData = (UpdateDataPtr) bContext->userdata;

  /* Get the string IDs for the current Bioseq */
  
  for (sip = bsp->id; sip != NULL; sip = sip->next) 
  {
    sip_next = sip->next;
    sip->next = NULL;
    SeqIdWrite (sip, currentId, PRINTID_TEXTID_ACC_ONLY,
	            sizeof (currentId) - 1);

    /* Compare it to the string ID of the new Bioseq */

    result = StringICmp (pUpdateData->newId, currentId);

    /* if TEXTID_ACC_ONLY doesn't match, try PRINTID_REPORT */
    if (result != 0)
    {
      SeqIdWrite (sip, currentId, PRINTID_REPORT,
	                sizeof (currentId) - 1);

      /* Compare it to the string ID of the new Bioseq */

      result = StringICmp (pUpdateData->newId, currentId);
    }

	  sip->next = sip_next;
    /* If they match, save the Bioseq and quit searching */

    if (0 == result) {
      pUpdateData->matchingBsp = bsp;
      return FALSE;
    }
  }

  /* Else continue */

  return TRUE;
}

static Boolean SkipProteinInNucUpdate (SeqEntryPtr sep, UpsDataPtr udp)
{
  Char       newId[MAX_ID_LEN];
  SeqIdPtr   sip;
  BioseqPtr  bsp;
  Boolean    rval = FALSE;
  MsgAnswer  ans;
  
  if (sep == NULL || ! IS_Bioseq (sep) || sep->data.ptrvalue == NULL || udp == NULL)
  {
    return FALSE;
  }
   
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (ISA_na (bsp->mol))
  {
    return FALSE;
  }
  
  sip = SeqIdFindWorst (bsp->id);
  SeqIdWrite (sip, newId, PRINTID_REPORT, sizeof (newId) - 1);
  ans = Message (MSG_YN, "Found a protein (%s) in the update file, expecting "
         "only nucleotides.  Do you want to skip this sequence and continue?", newId);
  if (ans == ANS_YES)
  {
    rval = TRUE;
    fprintf (udp->log_fp, "Skipped protein Bioseq (%s) in nucleotide update\n", newId);
	  udp->data_in_log = TRUE;
  }
  return rval;
}

static void RemoveUpdateSet (UpsDataPtr udp)
{
#if 0
  ObjMgrPtr omp;
  Int2      entityID;
  
  if (udp == NULL || udp->seqsubsep == NULL)
  {
    return;
  }
  entityID = SeqMgrGetEntityIDForSeqEntry (udp->seqsubsep);  
  udp->seqsubsep = SeqEntryFree (udp->seqsubsep);
	omp = ObjMgrGet ();	
	ObjMgrReapOne (omp);
	ObjMgrFreeCache (0);
  ObjMgrSendMsg(OM_MSG_DEL, entityID, 0, 0);
#endif
}

/*=====================================================================*/
/*                                                                     */
/* UpdateNextBioseqInFastaSet () - Reads in one Bioseq from a FASTA set*/
/*                                 file and updates the corresponding  */
/*                                 Bioseq in memory.                   */
/*                                                                     */
/*=====================================================================*/

static Int2 UpdateNextBioseqInFastaSet (UpsDataPtr udp)
{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Pointer       dataptr;
  Uint2         datatype;
  Char          errMsg[256];
  BioseqPtr     nbsp;
  SeqEntryPtr   nwsep = NULL;
  SeqEntryPtr   sep = NULL;
  SeqIdPtr      sip;
  SeqSubmitPtr  ssp;
  UpdateData    updateData;
  SeqEntryPtr   nthBspSep;
  BioseqPtr     nthBsp;
  Boolean       skip_prot_in_nuc;

  sep = GetTopSeqEntryForEntityID (udp->input_entityID);
  bsp = GetBioseqGivenIDs (udp->input_entityID,
			   udp->input_itemID,
			   udp->input_itemtype);

  updateData.matchingBsp = NULL;
  /* keep reading file until we find a sequence that matches
   * one that we have.
   */
  while (updateData.matchingBsp == NULL)
  {
    skip_prot_in_nuc = FALSE;
    if (udp->seqsubsep == NULL)
    {
      /* Read in one sequence from the file */
      dataptr = ReadAsnFastaOrFlatFile (udp->fp, &datatype, NULL, FALSE, FALSE,
		                   	                  TRUE, FALSE);      

      if (NULL == dataptr) 
      {
        FileClose (udp->fp);
        CloseOutSequenceUpdateLog (udp);
        RemoveUpdateSet (udp); 
        return FASTA_READ_DONE;
      }

      /* Convert the file data to a SeqEntry */
  
      if (datatype == OBJ_SEQENTRY)
        nwsep = (SeqEntryPtr) dataptr;
      else if (datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET)
        nwsep = SeqMgrGetSeqEntryForData (dataptr);
      else if (datatype == OBJ_SEQSUB) 
      {
        ssp = (SeqSubmitPtr) dataptr;
        if (ssp != NULL && ssp->datatype == 1)
        {
          nwsep = (SeqEntryPtr) ssp->data;
          udp->seqsubsep = nwsep;
          udp->seqsubpos = 1; 
        }
      }
  
      if (nwsep == NULL) 
      {
        FileClose (udp->fp);
        ErrPostEx (SEV_ERROR, 0, 0, "Unable to convert file data into SeqEntry.");
        CloseOutSequenceUpdateLog (udp);
        return FASTA_READ_ERROR;
      }  

      /* Use the new SeqEntry to get a Bioseq */
  
      if (ISA_na (bsp->mol))
      {
        nbsp = FindNucBioseq (nwsep);
      }
      else 
      {
        nwsep = FindNthBioseq (nwsep, 1);
        if (nwsep == NULL || nwsep->choice != 1)
        {
          CloseOutSequenceUpdateLog (udp);
          return FASTA_READ_ERROR;
        }
        nbsp = (BioseqPtr) nwsep->data.ptrvalue;
      }
      if (nbsp == NULL) 
      {
        if (ISA_na (bsp->mol))
        {
          skip_prot_in_nuc = SkipProteinInNucUpdate (nwsep, udp);
        }
        if (! skip_prot_in_nuc)
        {
          FileClose (udp->fp);
          ErrPostEx (SEV_ERROR, 0, 0, "Unable to convert file data into Bioseq.");
          CloseOutSequenceUpdateLog (udp);
          return FASTA_READ_ERROR;
        }
      }
    }
    else
    {
      /* get the next Bioseq from the record */
      udp->seqsubpos ++;
      nthBspSep = FindNthBioseq (udp->seqsubsep, udp->seqsubpos);
      nbsp = NULL;
      while (nthBspSep != NULL && nbsp == NULL)
      {
        if (!IS_Bioseq (nthBspSep))
        {
          udp->seqsubpos++;
          nthBspSep = FindNthBioseq (udp->seqsubsep, udp->seqsubpos);
        }
        else
        {
          nthBsp = nthBspSep->data.ptrvalue;
          if (ISA_na (bsp->mol) && ISA_na (nthBsp->mol))
          {
            nbsp = nthBsp;
          }
          else if (ISA_aa (bsp->mol) && ISA_aa (nthBsp->mol))
          {
            nbsp = nthBsp;
          }
          else
          {
            udp->seqsubpos++;
            nthBspSep = FindNthBioseq (udp->seqsubsep, udp->seqsubpos);
          }
        }
      }
      if (nthBspSep == NULL)
      {
        RemoveUpdateSet (udp); 
        return FASTA_READ_DONE;
      }
    }
  
    if (!skip_prot_in_nuc)
    {
      /* Get the string ID for the new Bioseq so that we */
      /* can find a matching ID among current Bioseqs.   */
  
      sip = SeqIdFindWorst (nbsp->id);
      SeqIdWrite (sip, updateData.newId, PRINTID_REPORT,
	                sizeof (updateData.newId) - 1);

      /* Find the matching bioseq in the current sequence set */

      updateData.matchingBsp = NULL;
      if (2 == sep->choice ) 
      {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        SeqMgrExploreBioseqs (0, (Pointer) bssp, &updateData, FindMatchingBioseq,
			                     TRUE, TRUE, TRUE);
      }
      else 
      {
        bsp = (BioseqPtr) sep->data.ptrvalue;
        SeqMgrExploreBioseqs (0, (Pointer) bsp, &updateData, FindMatchingBioseq,
			                        TRUE, TRUE, TRUE);
      }


      if (updateData.matchingBsp == NULL) 
      {
        OpenSequenceUpdateLog (udp);
        if (udp->log_fp != NULL)
        {
          fprintf (udp->log_fp, "No Bioseq found with ID matching that of the"
	                     " one in the file (%s)\n", updateData.newId);
	        udp->data_in_log = TRUE;
        }
        sprintf (errMsg, "No Bioseq found with ID matching that of the"
                       " one in the file (%s)", updateData.newId);
        ErrPostEx (SEV_ERROR, 0, 0, errMsg);
      }
    }
  }

  /* Do the updating of the sequences */
  
  udp->oldbsp = updateData.matchingBsp;
  udp->newbsp = nbsp;

  if (! PrepareToUpdateSequences (udp))
  {
    return FASTA_READ_DONE;
  }

  return FASTA_READ_OK;
}
  
/*=====================================================================*/
/*                                                                     */
/* UpdateFastaSet () - Updates a set of sequence from a FASTA file     */
/*                     containing a set of sequences.                  */
/*                                                                     */
/*=====================================================================*/

static void UpdateOrExtendFastaSet (IteM i, Boolean do_update)
{
  BaseFormPtr   bfp;
  FILE         *fp;
  Char          path [PATH_MAX];
  UpsDataPtr    udp;

  /* Check parameters */

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL)
    return;

  /* Read in the update data from a file */

  if (! GetInputFileName (path, sizeof (path),"","TEXT"))
    return;
  fp = FileOpen (path, "r");
  if (fp == NULL)
    return;

  /* Create data ptr */

  udp = (UpsDataPtr) MemNew (sizeof (UpsData));
  if (udp == NULL)
    return;

  udp->input_entityID = bfp->input_entityID;
  udp->input_itemID   = bfp->input_itemID;
  udp->input_itemtype = bfp->input_itemtype;
  udp->fp             = fp;
  udp->useGUI         = TRUE;
  udp->isSet          = TRUE;
  udp->convertPubs    = CONVERTPUBS_NO; /* was CONVERTPUBS_NOT_SET */
  udp->do_update      = do_update;
  udp->suppress_continue_msg = FALSE;
  udp->suppress_instant_refresh = FALSE;
  udp->log_fp         = NULL;
  udp->data_in_log    = FALSE;
  udp->transl_except_list = NULL;
  udp->aln1           = NULL;
  udp->aln2           = NULL;

  /* Update one Bioseq from the file.  Note that this chains */
  /* to the processing of the Bioseq after that, so that     */
  /* actually all Bioseqs are processed by this call.        */

  UpdateNextBioseqInFastaSet (udp);

}

extern void UpdateFastaSet (IteM i)
{
  UpdateOrExtendFastaSet (i, TRUE);
}

extern void ExtendFastaSet (IteM i)
{
  UpdateOrExtendFastaSet (i, FALSE);
}

typedef struct extendsequences
{
  FEATURE_FORM_BLOCK

  BioseqPtr  newbsp;
  Boolean    add_cit_sub;
  Boolean    extend5;
  FILE *     log_fp;
  Char       log_path [PATH_MAX];
  ValNodePtr sequence_list;
  Boolean    data_in_log;
  LisT       sequence_list_ctrl;
  ButtoN     add_cit_sub_btn;
  GrouP      extend_end;
  
} ExtendSequencesData, PNTR ExtendSequencesPtr;

static void ExtendAllSequencesInSetCallback (BioseqPtr bsp, Pointer userdata)
{
  ExtendSequencesPtr    esp;
  SeqIdPtr              sip, id_next;
  Char                  acc_str [256];
  CharPtr               origSeqStr;
  CharPtr               newSeqStr;
  CharPtr               mergedSeqStr;
  Int4                  mergedLen;
  ByteStorePtr          mergedBS;
  Int4                  offset;
  SeqMgrFeatContext     context;
  SeqFeatPtr            sfp;
  CdRegionPtr           crp;
  CodeBreakPtr          cbp;
  RnaRefPtr             rrp;
  tRNAPtr               trp;
  
  if (bsp == NULL || userdata == NULL) return;
  esp = (ExtendSequencesPtr) userdata;
  
  if (bsp == esp->newbsp) return;
  
  /* Get original and new sequences */

  origSeqStr = GetSequenceByBsp (bsp);
  newSeqStr = GetSequenceByBsp (esp->newbsp);
  
  /* create string to hold extended sequence */
  mergedLen =  StringLen (newSeqStr) + StringLen (origSeqStr);
  mergedSeqStr = (CharPtr) MemNew (mergedLen + 1);
    
  if (esp->extend5)
  {
    /* prepend the new sequence */
    sprintf (mergedSeqStr, "%s%s", newSeqStr, origSeqStr);
  }
  else
  {
    /* append the new sequence */
    sprintf (mergedSeqStr, "%s%s", origSeqStr, newSeqStr);
  }

  /* Convert the new sequence into a ByteStore */

  mergedBS = BSNew (mergedLen);
  BSWrite (mergedBS, (VoidPtr) mergedSeqStr, mergedLen);

  /* Replace the original sequence with the */
  /* new concatenated sequence.             */

  bsp->seq_data      = BSFree (bsp->seq_data);
  bsp->seq_data      = mergedBS;
  bsp->seq_data_type = Seq_code_iupacna;
  bsp->length        = mergedLen;

  /* shift the features downstream for 5' extension */
  offset = StringLen (newSeqStr);
  sip = SeqIdFindBest (bsp->id, 0);
  if (sip == NULL) return;
  if (esp->extend5 && offset > 0)
  {
    sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &context);
    while (sfp != NULL) {
      OffsetLocation (sfp->location, offset, sip);
      switch (sfp->data.choice) {
        case SEQFEAT_CDREGION :
          crp = (CdRegionPtr) sfp->data.value.ptrvalue;
          if (crp != NULL) {
            for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
              OffsetLocation (cbp->loc, offset, sip);
            }
          }
          break;
        case SEQFEAT_RNA :
          rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
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
      sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &context);
    }
  }
  
  if (esp->add_cit_sub)
  {
    AddCitSubToUpdatedSequence ( bsp, esp->input_entityID);
  }
  
  if (esp->log_fp != NULL)
  {
    id_next = sip->next;
    sip->next = NULL;
    SeqIdWrite (sip, acc_str, PRINTID_REPORT, sizeof (acc_str) - 1);
    sip->next = id_next;
    if (esp->extend5)
    {
      fprintf (esp->log_fp, "Extended %s at 5' end\n", acc_str);
    }
    else
    {
      fprintf (esp->log_fp, "Extended %s at 3' end\n", acc_str);
    }
    esp->data_in_log = TRUE;  
  }
}

static void DoExtendAllSequencesInSet (ButtoN b)
{
  ExtendSequencesPtr    esp;
  SeqEntryPtr           topsep;
  ValNodePtr            sip_list, vnp;
  SeqIdPtr              sip;
  BioseqPtr             bsp;

  esp = (ExtendSequencesPtr) GetObjectExtra (b);
  if (esp == NULL)
  {
    return;
  }
  
  esp->add_cit_sub = GetStatus (esp->add_cit_sub_btn);
  if (GetValue (esp->extend_end) == 1)
  {
    esp->extend5 = TRUE;
  }
  else
  {
    esp->extend5 = FALSE;
  }
  sip_list = GetSelectedSequenceList (esp->sequence_list_ctrl);

  /* create file for log */
  TmpNam (esp->log_path);
  esp->log_fp = FileOpen (esp->log_path, "wb");
  
  topsep = GetTopSeqEntryForEntityID (esp->input_entityID);
  if (topsep == NULL)
    return;
  
  for (vnp = sip_list; vnp != NULL; vnp = vnp->next)
  {
    sip = (SeqIdPtr) vnp->data.ptrvalue;
    bsp = BioseqFind (sip);
    if (bsp != NULL)
    {
      ExtendAllSequencesInSetCallback (bsp, esp);
    }
  }

  if (esp->log_fp != NULL) 
  {
    FileClose (esp->log_fp);
    esp->log_fp = NULL;
    if (esp->data_in_log) 
    {
      LaunchGeneralTextViewer (esp->log_path, "Extended Sequences");
      esp->data_in_log = FALSE;
    }
    FileRemove (esp->log_path);  	
  }
  ObjMgrSetDirtyFlag (esp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, esp->input_entityID, 0, 0);  
  Remove (esp->form);
  Update ();
}

static void SelectAllSequencesForExtend (ButtoN b)
{
  ExtendSequencesPtr    esp;

  esp = (ExtendSequencesPtr) GetObjectExtra (b);
  if (esp == NULL)
  {
    return;
  }
  SelectAllSequencesInListCtrl (esp->sequence_list_ctrl);  
}

static void UnSelectAllSequencesForExtend (ButtoN b)
{
  ExtendSequencesPtr    esp;

  esp = (ExtendSequencesPtr) GetObjectExtra (b);
  if (esp == NULL)
  {
    return;
  }
  UnSelectAllSequencesInListCtrl (esp->sequence_list_ctrl);  
}


extern void ExtendAllSequencesInSet (IteM i)
{
  BaseFormPtr        bfp;
  FILE              *fp;
  Char               path [PATH_MAX];
  Pointer            dataptr;
  Uint2              datatype;
  SeqEntryPtr        nwsep, topsep;
  SeqSubmitPtr       ssp;
  BioseqPtr          nbsp;
  BioseqPtr          bsp;
  ExtendSequencesPtr esp;
  WindoW             w;
  GrouP              h, g, c;
  ButtoN             b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL)
    return;
  
  topsep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (topsep == NULL)
    return;
  
  bsp = GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID,
			   bfp->input_itemtype);
  if (bsp == NULL)
  {
    Message (MSG_ERROR, "You must select a bioseq");
    return;
  }

  /* Read in the update data from a file */

  if (! GetInputFileName (path, sizeof (path),"","TEXT"))
    return;
  fp = FileOpen (path, "r");
  if (fp == NULL)
  {
    Message (MSG_ERROR, "Unable to open %s", path);
    return;
  }
    
  /* Create data ptr */
  dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE,
		                 	              TRUE, FALSE);

  FileClose (fp);
  if (NULL == dataptr) 
  {
    Message (MSG_ERROR, "Unable to read sequence data from %s", path);
    return;
  }

  /* Convert the file data to a SeqEntry */
  
  if (datatype == OBJ_SEQENTRY)
    nwsep = (SeqEntryPtr) dataptr;
  else if (datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET)
    nwsep = SeqMgrGetSeqEntryForData (dataptr);
  else if (datatype == OBJ_SEQSUB) 
  {
    ssp = (SeqSubmitPtr) dataptr;
    if (ssp != NULL && ssp->datatype == 1)
    {
      nwsep = (SeqEntryPtr) ssp->data;
    }
  }
  
  if (nwsep == NULL) 
  {
    ErrPostEx (SEV_ERROR, 0, 0, "Unable to convert file data into SeqEntry.");
    return;
  }  

  /* Use the new SeqEntry to get a Bioseq */
  
  if (ISA_na (bsp->mol))
  {
    nbsp = FindNucBioseq (nwsep);
  }
  else 
  {
    nwsep = FindNthBioseq (nwsep, 1);
    if (nwsep == NULL || nwsep->choice != 1)
    {
      return;
    }
    nbsp = (BioseqPtr) nwsep->data.ptrvalue;
  }
  
  if (nbsp == NULL) 
  {
    ErrPostEx (SEV_ERROR, 0, 0, "Unable to convert file data into Bioseq.");
    return;
  }
  
  esp = (ExtendSequencesPtr) MemNew (sizeof (ExtendSequencesData));
  if (esp == NULL) return;
  esp->newbsp = nbsp;
  
  w = FixedWindow (-50, -33, -10, -10, "Extend Sequences", NULL);

  SetObjectExtra (w, esp, StdCleanupFormProc);
  esp->form = (ForM) w;
  esp->input_entityID = bfp->input_entityID;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  esp->sequence_list_ctrl = MakeSequenceListControl (h, topsep, NULL, NULL, TRUE, TRUE);
  g = HiddenGroup (h, 2, 0, NULL);
  b = PushButton (g, "Select All", SelectAllSequencesForExtend);
  SetObjectExtra (b, esp, NULL);
  b = PushButton (g, "Unselect All", UnSelectAllSequencesForExtend);
  SetObjectExtra (b, esp, NULL);
  
  esp->extend_end = HiddenGroup (h, 2, 0, NULL);
  RadioButton (esp->extend_end, "5' end");
  RadioButton (esp->extend_end, "3' end");
  SetValue (esp->extend_end, 1);
    
  esp->add_cit_sub_btn = CheckBox (h, "Add Cit Subs to extended sequences", NULL);


  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoExtendAllSequencesInSet);
  SetObjectExtra (b, esp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) esp->sequence_list_ctrl,
                (HANDLE) esp->add_cit_sub_btn, (HANDLE) g,
                (HANDLE) esp->extend_end,
                (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}



/*=====================================================================*/
/*                                                                     */
/* NewUpdateSequence () - Updates a sequence from a file.              */
/*                                                                     */
/*=====================================================================*/

static Boolean DeltaLitOnly (BioseqPtr bsp)

{
  ValNodePtr  vnp;

  if (bsp == NULL || bsp->repr != Seq_repr_delta) return FALSE;
  for (vnp = (ValNodePtr)(bsp->seq_ext); vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 1) return FALSE;
  }
  return TRUE;
}

static void NewUpdateOrExtendSequence (IteM i, Boolean do_update)
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
  UpsDataPtr    udp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  /* Get the current Bioseq */

  if (bfp == NULL)
    return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL)
    return;
  bsp = GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID,
			   bfp->input_itemtype);
  if (bsp == NULL)
    return;

  /* Read in the update data from a file */

  if (! GetInputFileName (path, sizeof (path),"","TEXT"))
    return;
  fp = FileOpen (path, "r");
  if (fp == NULL)
  {
    Message (MSG_ERROR, "Unable to open %s", path);
    return;
  }
  if (bsp->repr == Seq_repr_delta)
  {
    nwsep = ImportOneGappedSequence (fp);
    if (nwsep != NULL && IS_Bioseq (nwsep))
    {
      nbsp = (BioseqPtr) nwsep->data.ptrvalue;
      if (nbsp->repr != Seq_repr_delta)
      {
        if (ANS_CANCEL == Message (MSG_OKC, "You are updating a delta sequence with a non-delta sequence. "
                          "If you choose replace, your delta sequence will no longer be a delta sequence. "
                          "Do you wish to continue?"))
        {
          SeqEntryFree (nwsep);
          return;
        }
      }
      SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) nwsep->data.ptrvalue, nwsep);
      SeqMgrAddToBioseqIndex (nwsep->data.ptrvalue);
      dataptr = nwsep;
      datatype = OBJ_SEQENTRY;
    }
  }
  else
  {
    dataptr = ReadAsnFastaOrFlatFile (fp, &datatype, NULL, FALSE, FALSE,
		  		    TRUE, FALSE);
  }
  FileClose (fp);
  if (dataptr == NULL)
    return;

  /* Get a pointer to the new SeqEntry */

  if (datatype == OBJ_SEQENTRY)
    nwsep = (SeqEntryPtr) dataptr;
  else if (datatype == OBJ_BIOSEQ || datatype == OBJ_BIOSEQSET)
    nwsep = SeqMgrGetSeqEntryForData (dataptr);
  else if (datatype == OBJ_SEQSUB) {
    ssp = (SeqSubmitPtr) dataptr;
    if (ssp != NULL && ssp->datatype == 1)
      nwsep = (SeqEntryPtr) ssp->data;
  }

  if (nwsep == NULL)
    return;

  /* Use the new SeqEntry to get a Bioseq */

  if (ISA_na (bsp->mol))
    nbsp = FindNucBioseq (nwsep);
  else {
    nwsep = FindNthBioseq (nwsep, 1);
    if (nwsep == NULL || nwsep->choice != 1) return;
    nbsp = (BioseqPtr) nwsep->data.ptrvalue;
  }

  if (nbsp == NULL)
    return;

  /* convert delta lit to raw so sequence can be updated */
  if (!indexerVersion)
  {
    /* If original sequence is a not a raw sequence then */
    /* ask user for advice on how to proceed.      */

    if (bsp->repr != Seq_repr_raw) {
      ans = Message (MSG_YN, "Only raw sequences can be updated."
		     " Do you wish to proceed for copying features?");
      if (ans == ANS_NO)
        return;
    }
  }

  /* Create data ptr */

  udp = (UpsDataPtr) MemNew (sizeof (UpsData));
  if (udp == NULL)
    return;

  udp->input_entityID = bfp->input_entityID;
  udp->input_itemID   = bfp->input_itemID;
  udp->input_itemtype = bfp->input_itemtype;
  udp->oldbsp         = bsp;
  udp->newbsp         = nbsp;
  udp->fp             = NULL;
  udp->isSet          = FALSE;
  udp->useGUI         = TRUE;
  udp->convertPubs    = CONVERTPUBS_NO; /* was CONVERTPUBS_NOT_SET */
  udp->do_update      = do_update;
  udp->suppress_continue_msg = FALSE;
  udp->suppress_instant_refresh = FALSE;
  udp->log_fp         = NULL;
  udp->data_in_log    = FALSE;
  udp->transl_except_list = NULL;
  udp->aln1           = NULL;
  udp->aln2           = NULL;

  /* Do the updating of the sequences */

  PrepareToUpdateSequences (udp);
}

extern void NewUpdateSequence (IteM i)

{
  NewUpdateOrExtendSequence (i, TRUE);
}

extern void NewExtendSequence (IteM i)

{
  NewUpdateOrExtendSequence (i, FALSE);
}

extern void UpdateSeqAfterDownload
(BaseFormPtr bfp,
 BioseqPtr oldbsp,
 BioseqPtr newbsp)
{
  MsgAnswer   ans;
  UpsDataPtr  udp;

  /* convert delta lit to raw so sequence can be updated */

  if (oldbsp->repr == Seq_repr_delta && DeltaLitOnly (oldbsp)) {
    if (indexerVersion) {
      SegOrDeltaBioseqToRaw (oldbsp);
      ObjMgrSetDirtyFlag (oldbsp->idx.entityID, TRUE);
    } else {
      ans = Message (MSG_YN, "Only raw sequences can be updated."
		     " Do you wish to convert this delta sequence to raw?");
      if (ans == ANS_YES) {
        SegOrDeltaBioseqToRaw (oldbsp);
        ObjMgrSetDirtyFlag (oldbsp->idx.entityID, TRUE);
      }
    }
  }

  if (newbsp->repr == Seq_repr_delta && DeltaLitOnly (newbsp)) {
    if (indexerVersion) {
      SegOrDeltaBioseqToRaw (newbsp);
      ObjMgrSetDirtyFlag (newbsp->idx.entityID, TRUE);
    } else {
      ans = Message (MSG_YN, "Only raw sequences can be updated."
		     " Do you wish to convert this delta sequence to raw?");
      if (ans == ANS_YES) {
        SegOrDeltaBioseqToRaw (newbsp);
        ObjMgrSetDirtyFlag (newbsp->idx.entityID, TRUE);
      }
    }
  }
  /* Create data ptr */

  udp = (UpsDataPtr) MemNew (sizeof (UpsData));
  if (udp == NULL)
    return;

  udp->input_entityID = bfp->input_entityID;
  udp->input_itemID   = bfp->input_itemID;
  udp->input_itemtype = bfp->input_itemtype;
  udp->oldbsp         = oldbsp;
  udp->newbsp         = newbsp;
  udp->fp             = NULL;
  udp->isSet          = FALSE;
  udp->useGUI         = TRUE;
  udp->convertPubs    = CONVERTPUBS_NO; /* was CONVERTPUBS_NOT_SET */
  udp->do_update      = TRUE;
  udp->suppress_continue_msg = FALSE;
  udp->suppress_instant_refresh = FALSE;
  udp->log_fp         = NULL;
  udp->data_in_log    = FALSE;
  udp->transl_except_list = NULL;
  udp->aln1           = NULL;
  udp->aln2           = NULL;

  /* Do the updating of the sequences */

  PrepareToUpdateSequences (udp);
}


extern void ExtendSeqAfterDownload 
(BaseFormPtr bfp,
 BioseqPtr oldbsp,
 BioseqPtr newbsp)

{
  MsgAnswer   ans;
  UpsDataPtr  udp;

  /* convert delta lit to raw so sequence can be updated */

  if (oldbsp->repr == Seq_repr_delta && DeltaLitOnly (oldbsp)) {
    if (indexerVersion) {
      SegOrDeltaBioseqToRaw (oldbsp);
      ObjMgrSetDirtyFlag (oldbsp->idx.entityID, TRUE);
    } else {
      ans = Message (MSG_YN, "Only raw sequences can be extended."
		     " Do you wish to convert this delta sequence to raw?");
      if (ans == ANS_YES) {
        SegOrDeltaBioseqToRaw (oldbsp);
        ObjMgrSetDirtyFlag (oldbsp->idx.entityID, TRUE);
      }
    }
  }

  if (newbsp->repr == Seq_repr_delta && DeltaLitOnly (newbsp)) {
    if (indexerVersion) {
      SegOrDeltaBioseqToRaw (newbsp);
      ObjMgrSetDirtyFlag (newbsp->idx.entityID, TRUE);
    } else {
      ans = Message (MSG_YN, "Only raw sequences can be extended."
		     " Do you wish to convert this delta sequence to raw?");
      if (ans == ANS_YES) {
        SegOrDeltaBioseqToRaw (newbsp);
        ObjMgrSetDirtyFlag (newbsp->idx.entityID, TRUE);
      }
    }
  }
  /* Create data ptr */

  udp = (UpsDataPtr) MemNew (sizeof (UpsData));
  if (udp == NULL)
    return;

  udp->input_entityID = bfp->input_entityID;
  udp->input_itemID   = bfp->input_itemID;
  udp->input_itemtype = bfp->input_itemtype;
  udp->oldbsp         = oldbsp;
  udp->newbsp         = newbsp;
  udp->fp             = NULL;
  udp->isSet          = FALSE;
  udp->useGUI         = TRUE;
  udp->convertPubs    = CONVERTPUBS_NO; /* was CONVERTPUBS_NOT_SET */
  udp->do_update      = FALSE;
  udp->suppress_continue_msg = FALSE;
  udp->suppress_instant_refresh = FALSE;
  udp->log_fp         = NULL;
  udp->data_in_log    = FALSE;
  udp->transl_except_list = NULL;
  udp->aln1           = NULL;
  udp->aln2           = NULL;

  /* Do the updating of the sequences */

  PrepareToUpdateSequences (udp);
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
  DialoG              sequence_list_dlg;
  ButtoN              stopCDS;
  ButtoN              transPast;
  ButtoN              fixCDS;
  ButtoN              fuseJoints;
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
   AlnMsg2Ptr    amp1;
   AlnMsg2Ptr    amp2;
   IvalInfoPtr  ival;
   IvalInfoPtr  ival_head = NULL;
   IvalInfoPtr  ival_prev = NULL;
   Int4         from_aln;
   Int4         start;
   Int4         stop;
   Int4         tmp;
   Int4         to_aln;
   Int4         seg_i, seg_n, seg_start, seg_stop;

   if (sap == NULL || sap->saip == NULL)
      return NULL;
   AlnMgr2GetNthSeqRangeInSA(sap, row1, &start, &stop);
   if (from < start)
      from = start;
   if (to > stop)
      to = stop;
   from_aln = AlnMgr2MapBioseqToSeqAlign(sap, from, row1);
   to_aln = AlnMgr2MapBioseqToSeqAlign(sap, to, row1);
   if (from_aln > to_aln)
   {
      tmp = from_aln;
      from_aln = to_aln;
      to_aln = tmp;
   }
   seg_n = AlnMgr2GetNumSegs(sap);
   for (seg_i=1; seg_i<=seg_n; seg_i++) {
     AlnMgr2GetNthSegmentRange(sap, seg_i, &seg_start, &seg_stop);
     if (seg_start > to_aln) continue;
     if (seg_stop < from_aln) continue;
     if (seg_start < from_aln) seg_start = from_aln;
     if (seg_stop > to_aln) seg_stop = to_aln;
     
     amp1 = AlnMsgNew2();
     amp1->from_aln = seg_start;
     amp1->to_aln = seg_stop;
     amp1->row_num = row1;
     amp2 = AlnMsgNew2();
     amp2->from_aln = seg_start;
     amp2->to_aln = seg_stop;
     amp2->row_num = row2;
     AlnMgr2GetNextAlnBit(sap, amp1);
     AlnMgr2GetNextAlnBit(sap, amp2);
     if (amp1->type == AM_SEQ && amp2->type == AM_SEQ) {
       ival = (IvalInfoPtr)MemNew(sizeof(IvalInfo));
       ival->start1 = amp1->from_row;
       ival->stop1 = amp1->to_row;
       ival->start2 = amp2->from_row;
       ival->stop2 = amp2->to_row;
       if (ival_head == NULL)
         ival_head = ival_prev = ival;
       else {
         ival_prev->next = ival;
         ival_prev = ival;
       }
     }
     AlnMsgFree2(amp1);
     AlnMsgFree2(amp2);
   }
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

static SeqLocPtr MergeAdjacentSeqLocIntervals (SeqLocPtr location)

{
  SeqLocPtr    head = NULL;
  SeqIntPtr    sinp, last_sinp;
  SeqLocPtr    slp;
  SeqPntPtr    spp, last_spp;
  SeqLocPtr    last_slp = NULL;
  Int4         last_from = -1, last_to = -1;
  Uint1        last_strand = 0;
  SeqIdPtr     last_id = NULL;

  head = NULL;
  slp = SeqLocFindNext (location, NULL);
  while (slp != NULL) {
    switch (slp->choice) {
    case SEQLOC_INT :
      sinp = (SeqIntPtr) slp->data.ptrvalue;
      if (sinp != NULL) {
	    if (last_slp != NULL && last_strand == sinp->strand && last_id != NULL
			&& SeqIdComp (sinp->id, last_id) == SIC_YES
			&& ((last_from <= sinp->from + 1 && last_to >= sinp->from - 1)
			  || (last_from <= sinp->to + 1 && last_to >= sinp->to - 1)
			  || (last_from >= sinp->from - 1 && last_to <= sinp->to + 1)
			  || (last_from <= sinp->from + 1 && last_to >= sinp->to - 1)))
		{
		  /* intervals are adjacent, so expand previous interval */
  	      if (last_slp->choice == SEQLOC_INT) {
		    last_sinp = last_slp->data.ptrvalue;
		    if (last_sinp != NULL) {
		      last_from = MIN (sinp->from, last_sinp->from);
			  last_sinp->from = last_from;
			  last_to = MAX (sinp->to, last_sinp->to);
			  last_sinp->to = last_to;
			}
		  } else if (last_slp->choice == SEQLOC_PNT) {
		    /* change previous entry from point to interval that includes point */
		    spp = (SeqPntPtr)last_slp->data.ptrvalue;
		    if (spp != NULL) {
              last_sinp = SeqIntNew ();
			  last_from = MIN (spp->point, sinp->from);
			  last_sinp->from = last_from;
			  last_to = MAX (sinp->to, spp->point);
			  last_sinp->to = last_to;
			  last_sinp->strand = sinp->strand;
			  last_sinp->id = SeqIdDup (sinp->id);
			  last_slp->choice = SEQLOC_INT;
			  last_slp->data.ptrvalue = last_sinp;
			  SeqPntFree (spp);
			}
		  }		  	
		} else {
		  /* add new interval */
		  last_sinp = SeqIntNew ();
          last_sinp->from = sinp->from;
		  last_from = sinp->from;
          last_sinp->to = sinp->to;
		  last_to = sinp->to;
          last_sinp->strand = sinp->strand;
		  last_strand = sinp->strand;
		  last_sinp->id = SeqIdDup (sinp->id);
          last_id = last_sinp->id;
          last_slp = ValNodeAddPointer (&head, SEQLOC_INT, (Pointer) last_sinp);
        }
	  }
	  break;
	case SEQLOC_PNT:
	  spp = (SeqPntPtr)slp->data.ptrvalue;
	  if (spp != NULL) {
	    if (last_slp != NULL && last_strand == spp->strand && last_id != NULL
			&& SeqIdComp (spp->id, last_id) == SIC_YES
			&& last_to >= spp->point - 1)
		{
		  /* intervals are adjacent, so expand previous interval */
		  if (last_slp->choice == SEQLOC_INT) {
		    last_sinp = last_slp->data.ptrvalue;
			if (last_sinp != NULL) {
			  last_to = MAX (spp->point, last_sinp->to);
			  last_sinp->to = last_to;
			  last_from = MIN (spp->point, last_sinp->from);
			  last_sinp->from = last_from;
			}
		  } else if (last_slp->choice == SEQLOC_PNT) {
		    /* change previous entry from point to interval that includes point */
		    last_spp = (SeqPntPtr)last_slp->data.ptrvalue;
			if (last_spp != NULL) {
              last_sinp = SeqIntNew ();
			  last_from = MIN (spp->point, last_spp->point);
			  last_sinp->from = last_from;
			  last_to = MAX (last_spp->point, spp->point);
			  last_sinp->to = last_to;
			  last_sinp->strand = spp->strand;
			  last_sinp->id = SeqIdDup (spp->id);
			  last_slp->choice = SEQLOC_INT;
			  last_slp->data.ptrvalue = last_sinp;
			  SeqPntFree (spp);
			}
		  }
		} else {
		  /* add new point */
          last_spp = SeqPntNew ();
          last_spp->point = spp->point;
		  last_to = spp->point;
		  last_from = spp->point;
          last_spp->strand = spp->strand;
		  last_strand = spp->strand;
          last_spp->id = SeqIdDup (spp->id);
          last_slp = ValNodeAddPointer (&head, SEQLOC_PNT, (Pointer) last_spp);
		}
	  }
	  break;
	default:
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

/* We need to be able to propagate features with locations on multiple segments
 * whose IDs may be found in separate alignments.
 * To do this, we will take the location of the master feature and for each sublocation,
 * we will find the sequence ID of the sublocation.  We will find the alignment that
 * contains the master feature sublocation sequence ID and construct a propagated 
 * sublocation for each sequence in the same alignment as the master feature sublocation 
 * sequence ID and add this to a list.
 * Once we have created all of the propagated sublocations, we will reconstitute the
 * total locations for the propagated features by associating each propagated sublocation
 * with other propagated sublocations with the same sequence ID or having a sequence ID
 * that belongs to the same segmented set.
 * We can then use the list of total locations to construct the propagated features.
 */

static SeqLocPtr MapSubLoc 
(SeqLocPtr   master_subloc,
 SeqAlignPtr salp, 
 Int4        master_row,
 Int4        prop_row,
 Boolean     gapSplit) 
{
  SeqLocPtr   prop_loc = NULL;
  SeqIdPtr    prop_sip;
  SeqIntPtr   sinp;
  IvalInfoPtr ival_head, ival;
  SeqPntPtr   spp;
  Uint1       strand;
   
  if (master_subloc == NULL || salp == NULL || master_row < 1 || prop_row < 1)
  {
    return NULL;
  }
  
  prop_sip = AlnMgr2GetNthSeqIdPtr (salp, prop_row);
  if (prop_sip == NULL) return NULL;
  
  switch (master_subloc->choice) {
    case SEQLOC_INT :
      sinp = (SeqIntPtr) master_subloc->data.ptrvalue;
      if (sinp != NULL) {
        strand = sinp->strand;
        ival_head = GetAlignmentIntervals (salp, master_row, prop_row, sinp->from, sinp->to);
        ival_head = MergeAdjacentIntervals (ival_head);
        if (ival_head != NULL) {

          /* what if one or the other interval maps into a gap? */

          if (gapSplit) {
            for (ival = ival_head; ival != NULL; ival = ival->next) {
              sinp = SeqIntNew ();
              sinp->from = ival->start2;
              sinp->to = ival->stop2;
              sinp->strand = strand;
              sinp->id = SeqIdDup (prop_sip);
              ValNodeAddPointer (&prop_loc, SEQLOC_INT, (Pointer) sinp);
            }
          } else {
            sinp = SeqIntNew ();
            sinp->from = ival_head->start2;
            for (ival = ival_head; ival->next != NULL; ival = ival->next) continue;
            sinp->to = ival->stop2;
            sinp->strand = strand;
            sinp->id = SeqIdDup (prop_sip);
            ValNodeAddPointer (&prop_loc, SEQLOC_INT, (Pointer) sinp);
          }

        }
        IvalInfoFree (ival_head);
      }
      break;
    case SEQLOC_PNT :
      spp = (SeqPntPtr) master_subloc->data.ptrvalue;
      if (spp != NULL) {
        strand = spp->strand;
        ival_head = GetAlignmentIntervals (salp, master_row, prop_row, spp->point, spp->point);
        if (ival_head != NULL) {

          spp = SeqPntNew ();
          spp->point = ival_head->start2;
          spp->strand = strand;
          spp->id = SeqIdDup (prop_sip);
          ValNodeAddPointer (&prop_loc, SEQLOC_PNT, (Pointer) spp);

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
  if (prop_loc == NULL)
  {
    prop_sip = SeqIdFree (prop_sip);
  }
  return prop_loc;
}

typedef struct loclist
{
  SeqIdPtr sip_list;
  SeqLocPtr slp;
} LocListData, PNTR LocListPtr;

static LocListPtr FindLocListForSeqId (ValNodePtr loc_list, SeqIdPtr sip)
{
  SeqIdPtr   search_sip, find_sip;
  LocListPtr llp;
  ValNodePtr vnp;
  
  for (vnp = loc_list; vnp != NULL; vnp = vnp->next)
  {
    llp = (LocListPtr) vnp->data.ptrvalue;
    if (llp != NULL && llp->slp != NULL)
    {
      for (search_sip = llp->sip_list; search_sip != NULL; search_sip = search_sip->next)
      {
        for (find_sip = sip; find_sip != NULL; find_sip = find_sip->next)
        {
          if (SeqIdComp (find_sip, search_sip) == SIC_YES)
          {
            return llp;
          } 
        }
      }
    }
  }
  return NULL;
}

/* Compare the Sequence ID of this location with the sequence IDs of other locations in the
 * list.  If the sequence ID matches that of another loclist, add to that loclist,
 * otherwise add a new loclist entry with the sequence ID of this sequence and the sequence IDs
 * of other sequences in the same segset if the segset flag is set.
 */ 
static ValNodePtr AssociatePropagatedSubloc (ValNodePtr subloc_list, SeqLocPtr prop_loc, Boolean segset)
{
  ValNodePtr   vnp;
  LocListPtr   llp = NULL;
  SeqIdPtr     prop_sip;
  BioseqPtr    prop_bsp;
  BioseqSetPtr parent_set;
  SeqEntryPtr  sep;
  
  if (prop_loc == NULL) return subloc_list;
  prop_sip = SeqLocId (prop_loc);
  if (prop_sip == NULL) 
  {
    SeqLocFree (prop_loc);
    return subloc_list;
  }
  
  llp = FindLocListForSeqId (subloc_list, prop_sip);
  if (llp != NULL)
  {
    ValNodeAddPointer (&(llp->slp), prop_loc->choice, prop_loc->data.ptrvalue);
    prop_loc = ValNodeFree (prop_loc);
  }
  else
  {
    llp = (LocListPtr) MemNew (sizeof (LocListData));
    if (llp != NULL)
    {
      llp->sip_list = SeqIdDup (prop_sip);
      llp->slp = prop_loc;
      if (segset)
      {
        prop_bsp = BioseqFind (prop_sip);
        if (prop_bsp != NULL && prop_bsp->idx.parenttype == OBJ_BIOSEQSET)
        {
          parent_set = prop_bsp->idx.parentptr;
          if (parent_set != NULL && parent_set->_class == BioseqseqSet_class_parts)
          {
            /* add other IDs from set to list */
            for (sep = parent_set->seq_set; sep != NULL; sep = sep->next)
            {
              if (IS_Bioseq (sep))
              {
                prop_bsp = (BioseqPtr) sep->data.ptrvalue;
                prop_sip = SeqIdDup (prop_bsp->id);
                ValNodeAddPointer (&(llp->sip_list), prop_sip->choice, prop_sip->data.ptrvalue);
                prop_sip = ValNodeFree (prop_sip);
              }
            }
          }
        }
      }
      vnp = ValNodeAddPointer (&subloc_list, 0, llp);
    }
  }
  return subloc_list;
}

static ValNodePtr FreeLocList (ValNodePtr loc_list)
{
  LocListPtr llp;
  
  if (loc_list == NULL) return NULL;
  loc_list->next = FreeLocList (loc_list->next);
  llp = loc_list->data.ptrvalue;
  if (llp != NULL)
  {
    SeqIdFree (llp->sip_list);
    SeqLocFree (llp->slp);
    MemFree (llp);
  }
  ValNodeFree (loc_list);
  return NULL;
}

static Int4 GetMasterRow (SeqAlignPtr salp, SeqIdPtr sip)
{
  Int4     master_row = -1;
  SeqIdPtr sip_next;
  
  if (salp == NULL || sip == NULL)
  {
    return -1;
  }
  
  while (sip != NULL && master_row == -1)
  {
    sip_next = sip->next;
    sip->next = NULL;
    master_row = AlnMgr2GetFirstNForSip (salp, sip);
    sip->next = sip_next;
    sip = sip_next;
  }
  return master_row;
}

static Boolean 
NthAlignmentSequenceInSeqPropList
(SeqAlignPtr salp,
 Int4        n,
 ValNodePtr  seq_for_prop)
{
  SeqIdPtr    sip;
  ValNodePtr  seq_vnp;
  BioseqPtr   bsp;
  SeqEntryPtr sep;
  BioseqSetPtr bssp;
  
  if (salp == NULL || seq_for_prop == NULL)
  {
    return FALSE;
  }
  
  sip = AlnMgr2GetNthSeqIdPtr(salp, n);
  for (seq_vnp = seq_for_prop; seq_vnp != NULL; seq_vnp = seq_vnp->next)
  {
    if (SeqIdIn (sip, (SeqIdPtr) seq_vnp->data.ptrvalue))
    {
      return TRUE;
    }
    /* check for segments */
    bsp = BioseqFind (seq_vnp->data.ptrvalue);
    if (bsp != NULL && bsp->repr == Seq_repr_seg)
    {
      sep = SeqMgrGetSeqEntryForData (bsp);
      sep = sep->next; /* parts should be next */
      if (sep != NULL && IS_Bioseq_set (sep))
      {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL && bssp->_class == BioseqseqSet_class_parts)
        {
          for (sep = bssp->seq_set; sep != NULL; sep = sep->next)
          {
            if (IS_Bioseq (sep))
            {
              bsp = (BioseqPtr) sep->data.ptrvalue;
              if (bsp != NULL && SeqIdIn (sip, bsp->id))
              {
                return TRUE;
              }
            }
          }
        }
      }
    }
  }
  
  
  return FALSE;
}

static ValNodePtr GetPropagatedSublocations 
(SeqLocPtr master_location,
 Boolean gap_split,
 ValNodePtr prop_loc_list,
 ValNodePtr seq_for_prop,
 BoolPtr warned_about_master)
{
  SeqLocPtr   master_subloc;
  SeqIdPtr    master_subloc_id;
  BioseqPtr   master_subloc_bsp;
  SeqLocPtr   tmp_loc;
  SeqAlignPtr salp;
  Int4        master_row, num_rows, prop_row; 
  SeqLocPtr   prop_loc;

  if (master_location == NULL) return prop_loc_list;
  
  master_subloc = SeqLocFindNext (master_location, NULL);
  while (master_subloc != NULL) 
  {
    master_subloc_id = SeqLocId (master_subloc);
    master_subloc_bsp = BioseqFind (master_subloc_id);
    master_subloc_id = master_subloc_bsp->id;
    if (master_subloc_bsp != NULL)
    {
      if (master_subloc_bsp->repr == Seq_repr_seg)
      {
        if (warned_about_master != NULL && ! *warned_about_master)
        {
          Message (MSG_OK, "Warning - you are propagating a feature that "
                  "contains locations on the master sequence.  These locations"
                  " will be mapped to the segments in the propagated features.");
          *warned_about_master = TRUE;
        }
        /* this is a location on the master segment */
        tmp_loc = SegLocToParts (master_subloc_bsp, master_subloc);
        prop_loc_list = GetPropagatedSublocations (tmp_loc, gap_split, prop_loc_list, 
                                                   seq_for_prop, warned_about_master);
        tmp_loc = SeqLocFree (tmp_loc);
      }
      else
      {
        salp = FindAlignmentsForBioseq (master_subloc_bsp);
        while (salp != NULL)
        {
          master_row = GetMasterRow (salp, master_subloc_id);
          num_rows = AlnMgr2GetNumRows (salp);
          for (prop_row = 1; prop_row <= num_rows; prop_row++)
          {
            if (prop_row == master_row) continue;
            if (! NthAlignmentSequenceInSeqPropList (salp, prop_row, seq_for_prop))
            {
              continue;
            }

            prop_loc = MapSubLoc (master_subloc, salp, master_row, prop_row, gap_split);
            prop_loc_list = AssociatePropagatedSubloc (prop_loc_list, prop_loc, TRUE);           
          }
          salp = salp->next;
        }
      }
    }
    master_subloc = SeqLocFindNext (master_location, master_subloc);
  }
  return prop_loc_list;
}

static ValNodePtr MapLocForProp
(SeqLocPtr   master_location,
 Boolean     gapSplit,
 ValNodePtr  seq_for_prop,
 BoolPtr     warned_about_master)
{
  ValNodePtr  prop_loc_list = NULL, vnp;
  LocListPtr  llp;
  SeqLocPtr   slp;
  
  if (master_location == NULL) 
  {
    return NULL;
  }

  prop_loc_list = GetPropagatedSublocations (master_location, gapSplit, prop_loc_list, 
                                             seq_for_prop, warned_about_master);
  
  /* now fix locations in prop_loc_list (add SEQLOC_MIX header to the locations that
   * are mixed)
   */
  for (vnp = prop_loc_list; vnp != NULL; vnp = vnp->next)
  {
    llp = (LocListPtr) vnp->data.ptrvalue;
    if (llp != NULL)
    {
      if (llp->slp != NULL)
      {
        if (llp->slp->next != NULL)
        {
          slp = ValNodeNew (NULL);
          slp->choice = SEQLOC_MIX;
          slp->data.ptrvalue = llp->slp;
          llp->slp = slp;
        }
      }
    }
  }
  return prop_loc_list;
}


static SeqIdPtr GetSegIdList (BioseqPtr master_seg)
{
  SeqIdPtr  sip_list = NULL, sip_last = NULL, sip;
  SeqLocPtr slp;
  if (master_seg == NULL)
  {
    return NULL;
  }
  if (master_seg->repr != Seq_repr_seg)
  {
    sip_list = SeqIdDup (master_seg->id);
  }
  else
  {    
    for (slp = master_seg->seq_ext; slp != NULL; slp = slp->next)
    {
      sip = SeqIdDup (SeqLocId (slp));
      if (sip_last == NULL)
      {
        sip_list = sip;
      }
      else
      {
        sip_last->next = sip;
      }
      sip_last = sip;
    }
  }
  return sip_list;
}

static void PropagateCodeBreaks 
(CdRegionPtr crp,
 SeqIdPtr sip,
 ValNodePtr codebreak_location_list,
 ValNodePtr codebreak_choice_list)
{
  CodeBreakPtr cbp, last_cbp = NULL;
  ValNodePtr   choice_vnp, cbp_vnp;
  LocListPtr   llp;
  BioseqPtr    cds_bsp;
  SeqIdPtr     sip_list;
  
  if (crp == NULL || codebreak_location_list == NULL || crp->code_break == NULL)
  {
    return;
  }
  
  crp->code_break = CodeBreakFree (crp->code_break);
  
  cds_bsp = BioseqFind (sip);
  if (cds_bsp == NULL)
  {
    return;
  }

  sip_list = GetSegIdList (cds_bsp);
  
  for (cbp_vnp = codebreak_location_list, choice_vnp = codebreak_choice_list;
       cbp_vnp != NULL && choice_vnp != NULL;
       cbp_vnp = cbp_vnp->next, choice_vnp = choice_vnp->next)
  {
    llp = FindLocListForSeqId (cbp_vnp->data.ptrvalue, sip_list);
    if (llp != NULL)
    {
      cbp = CodeBreakNew ();
      if (cbp != NULL)
      {
        cbp->loc = llp->slp;
        llp->slp = NULL;
        MemCpy (&(cbp->aa), choice_vnp->data.ptrvalue, sizeof (cbp->aa));
        if (last_cbp == NULL)
        {
          crp->code_break = cbp;
        }
        else
        {
          last_cbp->next = cbp;
        }
      }
    }
  }
  SeqIdFree (sip_list);
}

static void PropagateAnticodons 
(tRNAPtr trp,
 SeqIdPtr sip,
 ValNodePtr anticodon_location_list)
{
  LocListPtr llp;
  BioseqPtr  trna_bsp;
  SeqIdPtr   sip_list;
  
  if (trp == NULL || anticodon_location_list == NULL || trp->anticodon == NULL)
  {
    return;
  }
  
  trp->anticodon = SeqLocFree (trp->anticodon);
  
  trna_bsp = BioseqFind (sip);
  if (trna_bsp == NULL)
  {
    return;
  }
  sip_list = GetSegIdList (trna_bsp);
  
  llp = FindLocListForSeqId (anticodon_location_list, sip_list);
  if (llp != NULL)
  {
    trp->anticodon = llp->slp;
    llp->slp = NULL;
  }
  
  SeqIdFree (sip_list);
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
  Uint1 frame,
  BioseqPtr pbsp
)

{
  Int4       len;
  SeqIntPtr  sinp;
  SeqLocPtr  slp;
  Int4       total = 0;

  if (frame > 0) {
    frame--;
  }
  slp = SeqLocFindNext (sfp->location, NULL);
  while (slp != NULL) {
    len = SeqLocLen (slp);

    if (len + total - frame <= (pbsp->length + 1) * 3) {
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

/*------------------------------------------------------------------*/
/*                                                                  */
/* PropagateCDS () - Called from DoFeatProp() for CDS-specific      */
/*                   feature propagation.                           */
/*                                                                  */
/*------------------------------------------------------------------*/

static void PropagateCDS (SeqFeatPtr dup,
			  ProtRefPtr prp,
			  BioseqPtr  newbsp,
			  Boolean    stopCDS,
			  Boolean    transPast,
			  Boolean    cds3end,
			  Uint1      frame,
			  Uint1      strand)
{
  Uint2           entityID;
  MolInfoPtr      mip;
  Boolean         partial3;
  Boolean         partial5;
  BioseqPtr       pbsp;
  SeqDescrPtr     sdp;
  SeqIdPtr        sip;
  SeqFeatXrefPtr  xref;
  Boolean         xtend;

  /* Check parameters */

  if (dup == NULL || dup->data.choice != SEQFEAT_CDREGION || prp == NULL)
    return;

  /* Extend the location to the end if that was checked */

  if (transPast && cds3end) 
    ExtendLocToEnd (dup->location, newbsp, strand);

  /**/

  prp = AsnIoMemCopy ((Pointer) prp,
                       (AsnReadFunc) ProtRefAsnRead,
                       (AsnWriteFunc) ProtRefAsnWrite);

  xref = SeqFeatXrefNew ();
  if (xref == NULL)
    return;
  xref->data.choice = SEQFEAT_PROT;
  xref->data.value.ptrvalue = (Pointer) prp;
  xref->next = dup->xref;
  dup->xref = xref;

  entityID = ObjMgrGetEntityIDForPointer (newbsp);
  PromoteXrefsEx (dup, newbsp, entityID, (Boolean) (! stopCDS), FALSE, FALSE);

  /* Truncate new CDS based on new protein length */

  sip = SeqLocId (dup->product);
  if (sip == NULL)
    return;

  pbsp = BioseqFindCore (sip);
  if (pbsp == NULL)
    return;

  TruncateCDS (dup, frame, pbsp);

  /**/

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

  /* Set partial flag */

  dup->partial = (Boolean) (partial5 || partial3);
}

/*------------------------------------------------------------------*/
/*                                                                  */
/* CalculateReadingFrame () -- Calculates a sequence's reading      */
/*                             frame by seeing which frame          */
/*                             generates the most amino acids       */
/*                             when converted to a protein.         */
/*                                                                  */
/*------------------------------------------------------------------*/

static Int2 CalculateReadingFrame (SeqFeatPtr sfp, Boolean partial3)
{
  ByteStorePtr  bs;
  CdRegionPtr   crp;
  Int4          len;
  Int4          max;
  Uint1         frame;
  Int2          i;
  CharPtr       protstr;

  crp = (CdRegionPtr) sfp->data.value.ptrvalue;

  max = 0;
  frame = 0;

  if (! partial3 && crp->frame != 0) 
  {
  	bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
  	if (bs != NULL)
  	{
      protstr = BSMerge (bs, NULL);
      BSFree (bs);
      if (protstr != NULL) {
        len = StringLen (protstr);
        if (len > 0 && protstr [len - 1] == '*') {
          MemFree (protstr);
          return crp->frame; 	
        }
        MemFree (protstr);
      }
  	}
  }
  for (i = 1; i <= 3; i++) {
    crp->frame = (Uint1) i;
    bs = ProteinFromCdRegionEx (sfp, FALSE, FALSE);
    len = BSLen (bs);
    BSFree (bs);
    if (len > max) {
      max = len;
      frame = (Uint1) i;
    }
  }

  return frame;
}

/* we don't want to propagate Prot features if the target
 * sequence already has one.
 */
static Boolean OkToPropagate(SeqFeatPtr sfp, BioseqPtr bsp)
{
  SeqMgrFeatContext fcontext;
  
  if (sfp == NULL || bsp == NULL) return FALSE;
  /* never ok if a gap */
  if (sfp->idx.subtype == FEATDEF_gap) return FALSE;
  /* always ok if not a Prot feature */
  if (sfp->idx.subtype != FEATDEF_PROT) return TRUE;
  /* always ok if not a protein sequence */
  if (ISA_na (bsp->mol)) return TRUE;
  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, FEATDEF_PROT, &fcontext);
  if (sfp == NULL)
    return TRUE;
  else
    return FALSE;
}


static SeqIdPtr GetSegSetId (SeqLocPtr slp)
{
  SeqLocPtr    sub_slp;
  SeqIdPtr     master_sip, part_sip;
  BioseqPtr    seg_bsp;
  BioseqSetPtr parent_set;
  BioseqSetPtr last_parent_set = NULL;
  SeqEntryPtr  sep;
  BioseqPtr    parent_bsp;
  
  if (slp == NULL) return NULL;
  master_sip = SeqLocId (slp);
  if (master_sip != NULL || slp->choice != SEQLOC_MIX)
  {
    return master_sip;
  }
  /* make sure all parts are from the same segmented set */
  for (sub_slp = slp->data.ptrvalue; sub_slp != NULL; sub_slp = sub_slp->next)
  {
    part_sip = SeqLocId (sub_slp);
    seg_bsp = BioseqFind (part_sip);
    if (seg_bsp == NULL || seg_bsp->idx.parenttype != OBJ_BIOSEQSET 
        || seg_bsp->idx.parentptr == NULL)
    {
      return NULL;
    }
    parent_set = (BioseqSetPtr) seg_bsp->idx.parentptr;
    if (parent_set->_class != BioseqseqSet_class_parts 
        || parent_set->idx.parenttype != OBJ_BIOSEQSET
        || parent_set->idx.parentptr == NULL)
    {
      return NULL;
    }
    if (last_parent_set == NULL)
    {
      last_parent_set = parent_set;
    }
    else if (last_parent_set != parent_set)
    {
      return NULL;
    }
  }
  if (last_parent_set == NULL)
  {
    return NULL;
  }
  parent_set = (BioseqSetPtr) last_parent_set->idx.parentptr;
  if (parent_set->_class != BioseqseqSet_class_segset)
  {
    return NULL;
  }
  for (sep = parent_set->seq_set; sep != NULL && ! IS_Bioseq (sep); sep = sep->next)
  {
  }
  if (sep == NULL) return NULL;
  parent_bsp = sep->data.ptrvalue;
  if (parent_bsp == NULL) return NULL;
  master_sip = parent_bsp->id;
  return master_sip;  
}

/* for each feature, find all propagated locations and create features using
 * those locations.
 */
static void 
PropagateOneFeat
(SeqFeatPtr sfp,
 Boolean    gapSplit,
 Boolean    fuse_joints,
 Boolean    stopCDS,
 Boolean    transPast,
 Boolean    cds3end,
 ValNodePtr seq_for_prop,
 BoolPtr    warned_about_master)
{
  ValNodePtr      feature_location_list, vnp;
  ValNodePtr      codebreak_location_list = NULL;
  ValNodePtr      codebreak_choice_list = NULL;
  ValNodePtr      anticodon_location_list = NULL;
  CodeBreakPtr    cbp;
  CdRegionPtr     crp;
  SeqFeatPtr      dup;
  Uint1           frame = 0;
  BioseqPtr       newbsp;
  SeqLocPtr       newloc, mergedloc;
  Boolean         partial5;
  Boolean         partial3;
  RnaRefPtr       rrp;
  SeqEntryPtr     sep;
  SeqIdPtr        sip;
  tRNAPtr         trp;
  LocListPtr      llp;
  Uint2           strand;
  ProtRefPtr      prp = NULL;
  BioseqPtr       pbsp;
  SeqFeatPtr      prot;
  
  if (sfp == NULL || sfp->location == NULL) return;
  
  feature_location_list = MapLocForProp (sfp->location, gapSplit, 
                                         seq_for_prop, warned_about_master);
  if (feature_location_list == NULL) return;

  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  
  /* also need to propagate locations for CDS code breaks and tRNA anticodons */
  if (sfp->data.choice == SEQFEAT_CDREGION)
  {
    crp = (CdRegionPtr) sfp->data.value.ptrvalue;
    if (crp != NULL)
    {
      for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) 
      {
        vnp = MapLocForProp (cbp->loc, gapSplit, seq_for_prop, warned_about_master);
        ValNodeAddPointer (&codebreak_location_list, 0, vnp);
        ValNodeAddPointer (&codebreak_choice_list, 0, &(cbp->aa));
      }
    }
    sip = SeqLocId (sfp->product);
    if (sip != NULL)
    {
      pbsp = BioseqFindCore (sip);
      if (pbsp != NULL)
      {
        prot = SeqMgrGetBestProteinFeature (pbsp, NULL);
        if (prot != NULL && prot->data.choice == SEQFEAT_PROT)
        {
          prp = (ProtRefPtr) prot->data.value.ptrvalue;          
        }
      }
    }
  }
  else if (sfp->data.choice == SEQFEAT_RNA)
  {
    rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
    if (rrp != NULL && rrp->ext.choice == 2) 
    {
      trp = (tRNAPtr) rrp->ext.value.ptrvalue;
      if (trp != NULL && trp->anticodon != NULL) 
      {
        anticodon_location_list = MapLocForProp (trp->anticodon, gapSplit, 
                                                 seq_for_prop, warned_about_master);
      }
    }
  }
  
  for (vnp = feature_location_list; vnp != NULL; vnp = vnp->next)
  {
    llp = (LocListPtr) vnp->data.ptrvalue;
    if (llp == NULL)
    {
      continue;
    }
    newloc = llp->slp;
    llp->slp = NULL;
    if (newloc == NULL)
    {
      continue;
    }

    mergedloc = NULL;
    if (fuse_joints) 
    {
      mergedloc = MergeAdjacentSeqLocIntervals (newloc);
	    if (mergedloc != NULL) 
	    {
        SeqLocFree (newloc);
        newloc = mergedloc;
	    }
    }
    
    /* if we have a mixed location with multiple sequences, SeqLocId will
     * return NULL.
     * We should check to see if we are trying to create a feature for
     * a segset, in which case we'll want the Bioseq for the master segment.
     */
    sip = GetSegSetId (newloc);

    dup = AsnIoMemCopy ((Pointer) sfp,
                        (AsnReadFunc) SeqFeatAsnRead,
                        (AsnWriteFunc) SeqFeatAsnWrite);
    SeqLocFree (dup->location);
    dup->location = newloc;
    SetSeqLocPartial (dup->location, partial5, partial3);

    /* clean up product before we look for (and maybe don't find) the newbsp */
    if (dup->product != NULL)
      dup->product = SeqLocFree (dup->product);

    switch (dup->data.choice) {
      case SEQFEAT_CDREGION :
        crp = (CdRegionPtr) dup->data.value.ptrvalue;
        if (crp != NULL) {
          crp->frame = CalculateReadingFrame (dup, partial3);
          frame = crp->frame;
 
          PropagateCodeBreaks (crp, sip,
                               codebreak_location_list,         
                               codebreak_choice_list);         
        }
        break;
      case SEQFEAT_RNA :
        rrp = (RnaRefPtr) dup->data.value.ptrvalue;
        if (rrp != NULL && rrp->ext.choice == 2) {
          trp = (tRNAPtr) rrp->ext.value.ptrvalue;
          if (trp != NULL && trp->anticodon != NULL) 
          {
            PropagateAnticodons (trp, sip, anticodon_location_list);
          }
        }
        break;
      default :
        break;
    }

    newbsp = BioseqFindCore (sip);
    if (newbsp == NULL)
      return;
  
    /* need to call OkToPropagate with sfp instead of dup
     * because dup has not been indexed yet, so subtype isn't set.
     */
    if (OkToPropagate(sfp, newbsp))
    {
      sep = SeqMgrGetSeqEntryForData (newbsp);
      if (sep == NULL)
        return;
      CreateNewFeature (sep, NULL, dup->data.choice, dup);

      /* If we're doing a CDS propagation, then */
      /* do the extra stuff related to that.    */

      if (SEQFEAT_CDREGION == dup->data.choice)
      {
        strand = SeqLocStrand (dup->location);

        PropagateCDS (dup, prp, newbsp, stopCDS, transPast, cds3end, frame, strand);        
      }
    }
    else
    {
      SeqFeatFree (dup);
    }
  }  
  
  feature_location_list = FreeLocList (feature_location_list);
  anticodon_location_list = FreeLocList (anticodon_location_list);
  for (vnp = codebreak_location_list; vnp != NULL; vnp = vnp->next)
  {
    vnp->data.ptrvalue = FreeLocList (vnp->data.ptrvalue);
  }
  codebreak_location_list = ValNodeFree (codebreak_location_list);
  codebreak_choice_list = ValNodeFree (codebreak_choice_list);
  
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

static void DoFixCDS (
  SeqFeatPtr sfp,
  Pointer userdata
)

{
  BaseFormPtr        bfp;
  ByteStorePtr       bs;
  BioseqPtr          bsp;
  Boolean            change_partials = FALSE;
  SeqMgrFeatContext  context;
  CdRegionPtr        crp;
  size_t             len;
  Boolean            partial5;
  Boolean            partial3;
  SeqIntPtr          sintp;
  SeqLocPtr          slp;
  CharPtr            str;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return;
  bfp = (BaseFormPtr) userdata;
  if (SeqMgrGetDesiredFeature (bfp->input_entityID, NULL,
                               0, 0, sfp, &context) != sfp) return;
  bsp = context.bsp;
  if (bsp == NULL) return;

  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  crp = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (crp->frame > 1) {
    if (context.strand == Seq_strand_minus) {
      if (context.right == bsp->length - 1) {
        partial5 = TRUE;
        change_partials = TRUE;
      }
    } else {
      if (context.left == 0) {
        partial5 = TRUE;
        change_partials = TRUE;
      }
    }
  }
    bs = ProteinFromCdRegion (sfp, TRUE);
    if (bs != NULL) {
      str = BSMerge (bs, NULL);
      BSFree (bs);
      if (str != NULL) {
        if (*str == '-') {
          if (! partial5) {
            partial5 = TRUE;
            change_partials = TRUE;
          }
        }
        len = StringLen (str);
        if (len > 0 && str [len - 1] != '*') {
          if (context.strand == Seq_strand_minus) {
          } else {
            if (bsp->length - context.right < 3) {
              slp = SeqLocFindNext (sfp->location, NULL);
              while (slp != NULL) {
                if (slp->choice == SEQLOC_INT) {
                  sintp = (SeqIntPtr) slp->data.ptrvalue;
                  if (sintp != NULL) {
                    if (sintp->to == context.right) {
                      sintp->to = bsp->length - 1;
                    }
                  }
                }
                slp = SeqLocFindNext (sfp->location, slp);
              }
            }
            partial3 = TRUE;
            change_partials = TRUE;
          }
        }
        MemFree (str);
      }
    }
  if (change_partials) {
    SetSeqLocPartial (sfp->location, partial5, partial3);
    ResynchCDSPartials (sfp, NULL);
  }
}

extern void FixCdsAfterPropagate (
  IteM i
)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitFeaturesInSep (sep, (Pointer) bfp, DoFixCDS);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static SeqFeatPtr 
GetNextFeatureOnSegOrMaster 
(BioseqPtr bsp, SeqFeatPtr sfp, Uint4 itemID, Uint4 index, SeqMgrFeatContextPtr fcontext)
{
  BioseqSetPtr       bssp;
  SeqEntryPtr        sep;
  BioseqPtr          master_bsp = NULL;
  SeqFeatPtr         next_sfp;
  SeqLocPtr          slp;
  SeqIdPtr           loc_id;
  Boolean            on_this_segment = FALSE;
  
  if (bsp == NULL)
  {
    return NULL;
  }
  if (bsp->idx.parenttype != OBJ_BIOSEQSET || bsp->idx.parentptr == NULL)
  {
    return SeqMgrGetNextFeature (bsp, sfp, itemID, index, fcontext);
  }

  bssp = (BioseqSetPtr) bsp->idx.parentptr;
  if (bssp == NULL || bssp->_class != BioseqseqSet_class_parts
      || bssp->idx.parenttype != OBJ_BIOSEQSET || bsp->idx.parentptr == NULL)
  {
    return SeqMgrGetNextFeature (bsp, sfp, itemID, index, fcontext);
  }
  
  bssp = bssp->idx.parentptr;
  if (bssp->_class != BioseqseqSet_class_segset)
  {
    return SeqMgrGetNextFeature (bsp, sfp, itemID, index, fcontext);
  }

  for (sep = bssp->seq_set; sep != NULL && master_bsp == NULL; sep = sep->next)
  {
    if (IS_Bioseq (sep))
    {
      master_bsp = sep->data.ptrvalue;
      if (master_bsp != NULL && master_bsp->repr != Seq_repr_seg)
      {
        master_bsp = NULL;
      }
    }
  }
  
  if (master_bsp == NULL)
  {
    return SeqMgrGetNextFeature (bsp, sfp, itemID, index, fcontext);
  }
  
  next_sfp = SeqMgrGetNextFeature (master_bsp, sfp, itemID, index, fcontext);
  if (next_sfp == NULL) return NULL;

  while (next_sfp != NULL && !on_this_segment)
  {
    for (slp = SeqLocFindNext (next_sfp->location, NULL);
         slp != NULL && ! on_this_segment; slp = SeqLocFindNext (next_sfp->location, slp))
    {
      loc_id = SeqLocId (slp);
      if (SeqIdIn (loc_id, bsp->id))
      {
        on_this_segment = TRUE;
      }
    }
    if (!on_this_segment)
    {
      next_sfp = SeqMgrGetNextFeature (master_bsp, next_sfp, itemID, index, fcontext);
    }
  }
  
  return next_sfp;  
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
  Boolean            fixCDS;
  Boolean            gapSplit;
  SeqAlignPtr        salp;
  SeqEntryPtr        sep;
  SeqFeatPtr         sfp;
  SeqIdPtr           sip;
  SeqIdPtr           sip_head;
  SeqIdPtr           sip_new;
  SeqIdPtr           sip_prev;
  SeqIdPtr           sip_tmp;
  Boolean            stopCDS;
  Boolean            transPast;
  Boolean            fuse_joints = FALSE;
  Boolean            warned_about_master = FALSE;
  ValNodePtr         seq_for_prop = NULL;

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
  if (GetStatus (fdp->fixCDS)) {
    fixCDS = TRUE;
  } else {
    fixCDS = FALSE;
  }
  if (GetStatus (fdp->fuseJoints)) {
    fuse_joints = TRUE;
  } else {
	  fuse_joints = FALSE;
  }
  
  seq_for_prop = DialogToPointer (fdp->sequence_list_dlg);

  SeqEntrySetScope (NULL);

  /* need to find alignment for each feature and row within that alignment for the feature */

  dsp = (DenseSegPtr)(salp->segs);
  sip = SeqIdFindBest (bsp->id, 0);
  sip_tmp = dsp->ids;
  sip_head = sip_prev = NULL;
  while (sip_tmp != NULL)
  {
    if (SeqIdComp(sip_tmp, bsp->id) == SIC_YES)
       sip_new = SeqIdDup(sip);
    else
       sip_new = SeqIdDup(sip_tmp);
    if (sip_head != NULL)
    {
       sip_prev->next = sip_new;
       sip_prev = sip_new;
    } else
       sip_head = sip_prev = sip_new;
    sip_tmp = sip_tmp->next;
  }
  dsp->ids = sip_head;

  if (fdp->selFeatItemID != 0) {

    /* propagate single selected feature */

    sfp = SeqMgrGetDesiredFeature (0, bsp, fdp->selFeatItemID, 0, NULL, &fcontext);
    if (sfp != NULL) {
      cds3end = CDSgoesToEnd (bsp, &fcontext);
      PropagateOneFeat (sfp, gapSplit, fuse_joints, stopCDS, transPast, 
                        cds3end, seq_for_prop, &warned_about_master);
    }
  } else {

    /* propagate all features on bioseq */

    sfp = GetNextFeatureOnSegOrMaster (bsp, NULL, 0, 0, &fcontext);
    while (sfp != NULL) {
      cds3end = CDSgoesToEnd (bsp, &fcontext);
      PropagateOneFeat (sfp, gapSplit, fuse_joints, stopCDS, transPast,
                        cds3end, seq_for_prop, &warned_about_master);

      sfp = GetNextFeatureOnSegOrMaster (bsp, sfp, 0, 0, &fcontext);
    }
  }

  seq_for_prop = ValNodeFree (seq_for_prop);
  if (fixCDS) {
    entityID = ObjMgrGetEntityIDForPointer (bsp);
    sep = GetTopSeqEntryForEntityID (entityID);
    fdp->input_entityID = entityID;
    /* reindex before calling DoFixCDS */
    SeqMgrIndexFeatures (entityID, NULL);
    VisitFeaturesInSep (sep, fdp, DoFixCDS);
  }

  entityID = ObjMgrGetEntityIDForPointer (bsp);
  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);

  Remove (fdp->form);
}

static void SetFeaturePropagateAccept (Pointer userdata)
{
  FprDataPtr  fdp;
  ValNodePtr  err_list;
  
  fdp = (FprDataPtr) userdata;
  if (fdp == NULL)
  {
    return;
  }
  err_list = TestDialog (fdp->sequence_list_dlg);
  if (err_list == NULL)
  {
    Enable (fdp->accept);
  }
  else
  {
    Disable (fdp->accept);
  }
  ValNodeFree (err_list);
}

static ForM FeaturePropagateForm (
  BioseqPtr bsp,
  SeqAlignPtr salp,
  Uint2 selFeatItemID
)

{
  ButtoN      b;
  GrouP       c;
  GrouP       seq_choice_grp;
  FprDataPtr  fdp;
  GrouP       g;
  PrompT      ppt;
  SeqIdPtr    sip;
  Char        strid [MAX_ID_LEN];
  Char        txt [128];
  WindoW      w;
  Uint2       entityID;

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
    sprintf (txt, "Propagate from %s to", strid);
  } else {
    sprintf (txt, "Propagate from %s to", strid);
  }

  g = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (g, 5, 5);

  seq_choice_grp = HiddenGroup (g, 0, 2, NULL);
  ppt = StaticPrompt (seq_choice_grp, txt, 0, 0, programFont, 'c');
  entityID = ObjMgrGetEntityIDForPointer (bsp);
  fdp->sequence_list_dlg = SequenceSelectionDialog (seq_choice_grp, 
                                                    SetFeaturePropagateAccept,
                                                    fdp, 
                                                    TRUE, 
                                                    ISA_na (bsp->mol), 
                                                    ISA_aa (bsp->mol), 
                                                    entityID);

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
  SetStatus (fdp->stopCDS, FALSE);

  fdp->transPast = CheckBox (g, "Translate CDS after partial 3' boundary", NULL);

  fdp->fixCDS = CheckBox (g, "Cleanup CDS partials after propagation", NULL);
  SetStatus (fdp->fixCDS, TRUE);

  fdp->fuseJoints = CheckBox (g, "Fuse adjacent propagated intervals", NULL);
  SetStatus (fdp->fuseJoints, FALSE);

  c = HiddenGroup (w, 4, 0, NULL);
  fdp->accept = DefaultButton (c, "Accept", AcceptFeatProp);
  SetObjectExtra (fdp->accept, (Pointer) fdp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) seq_choice_grp, (HANDLE) fdp->allOrSel,
                (HANDLE) fdp->gapSplit, (HANDLE) fdp->stopCDS,
                (HANDLE) fdp->transPast, (HANDLE) fdp->fixCDS,
				(HANDLE) fdp->fuseJoints,
                (HANDLE) c, NULL);
  RealizeWindow (w);
  SendMessageToDialog (fdp->sequence_list_dlg, NUM_VIB_MSG + 1);


  return (ForM) w;
}

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
  sfp = GetNextFeatureOnSegOrMaster (bsp, NULL, 0, 0, &fcontext);
  if (sfp == NULL)
  {
    Message (MSG_OK, "The sequence must have features in order to propagate");
    return;
  }
  
  salp = FindAlignmentsForBioseq (bsp);

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

  if (sfp->data.choice != SEQFEAT_RNA) return;

  /* overlapping gene should be single interval on contig */

  gene = SeqMgrGetOverlappingGene (sfp->location, &gcontext);
  if (gene == NULL) return;

  /* find cdna product of mrna */

  bsp = BioseqFindFromSeqLoc (sfp->product);
  if (bsp == NULL) return;

  /* make sure gene feature doesn't already exist on cDNA */

  temp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_GENE, 0, &gcontext);
  if (temp != NULL) return;

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

static void MakeProtRefXref (SeqFeatPtr sfp)

{
  BioseqPtr          bsp;
  SeqMgrFeatContext  pcontext;
  SeqFeatPtr         prot;
  ProtRefPtr         prp;
  SeqFeatXrefPtr     xref;

  for (xref = sfp->xref; xref != NULL; xref = xref->next) {
    if (xref->data.choice == SEQFEAT_PROT) return;
  }

  bsp = BioseqFindFromSeqLoc (sfp->product);
  if (bsp == NULL) return;

  prot = SeqMgrGetBestProteinFeature (bsp, &pcontext);
  if (prot != NULL && prot->data.choice == SEQFEAT_PROT) {
    prp = (ProtRefPtr) prot->data.value.ptrvalue;
    if (prp != NULL) {
      xref = SeqFeatXrefNew ();
      if (xref != NULL) {
        xref->data.choice = SEQFEAT_PROT;
        xref->data.value.ptrvalue = AsnIoMemCopy ((Pointer) prp,
                                                  (AsnReadFunc) ProtRefAsnRead,
                                                  (AsnWriteFunc) ProtRefAsnWrite);
        xref->next = sfp->xref;
        sfp->xref = xref;
      }
    }
  }

}

static void RemoveSfpTitle (SeqFeatPtr sfp, Pointer userdata)

{
  if (sfp->title == NULL) return;
  sfp->title = MemFree (sfp->title);
}

static void MakeRedundantGPS (
  IteM i, Boolean doprop, Boolean justprotxref
)

{
  BaseFormPtr        bfp;
  BioseqPtr          bsp;
  SeqMgrFeatContext  fcontext;
  SeqEntryPtr        sep;
  SeqFeatPtr         sfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  if (! justprotxref) {  	
    VisitFeaturesInSep (sep, NULL, CopyGene);
    VisitFeaturesInSep (sep, NULL, CopyUserObject);
  }

  /* find genomic sequence */
  bsp = FindNucBioseq (sep);
  if (bsp != NULL) {
    if (! justprotxref) {
      sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &fcontext);
      if (sfp == NULL) {
        /* only copy CDS features up if none already on genomic */
        VisitFeaturesInSep (sep, NULL, CopyCDS);
        /* reindex with new CDS features */
        SeqMgrIndexFeatures (bfp->input_entityID, NULL);
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &fcontext);
    while (sfp != NULL) {
      MakeProtRefXref (sfp);
      sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_CDREGION, 0, &fcontext);
    }
  }
  VisitFeaturesInSep (sep, NULL, RemoveSfpTitle);
  if (doprop) {
    PropagateFromGenomicProductSet (sep);
  }

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

extern void MakeRedundantGPSwithProp (
  IteM i
)

{
  MakeRedundantGPS (i, TRUE, FALSE);
}

extern void MakeRedundantGPSnoProp (
  IteM i
)

{
  MakeRedundantGPS (i, FALSE, FALSE);
}

extern void MakeRedundantGPSjustXref (IteM i)

{
  MakeRedundantGPS (i, FALSE, TRUE);
}

static void FuseFeatJoins (SeqFeatPtr sfp, Pointer userdata)

{
  BioseqPtr  bsp;
  Boolean    partial5;
  Boolean    partial3;
  SeqLocPtr  slp;

  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp == NULL) return;

  slp = SeqLocFindNext (sfp->location, NULL);
  if (slp == NULL) return;
  slp = SeqLocFindNext (sfp->location, slp);
  if (slp == NULL) return;

  slp = SeqLocMerge (bsp, sfp->location, NULL, FALSE, TRUE, FALSE);
  if (slp == NULL) return;
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  sfp->location = SeqLocFree (sfp->location);
  sfp->location = slp;
  SetSeqLocPartial (sfp->location, partial5, partial3);
}

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

static void DoAuthorityPrefix (BioSourcePtr biop, Pointer userdata)

{
  size_t      len;
  OrgModPtr   omp;
  OrgNamePtr  onp;
  OrgRefPtr   orp;
  CharPtr     str;

  if (biop == NULL) return;
  orp = biop->org;
  if (orp == NULL) return;
  if (StringHasNoText (orp->taxname)) return;
  len = StringLen (orp->taxname);
  onp = orp->orgname;
  if (onp == NULL) return;
  for (omp = onp->mod; omp != NULL; omp = omp->next) {
    if (omp->subtype != ORGMOD_authority) continue;
    if (StringNCmp (omp->subname, orp->taxname, len) == 0) continue;
    str = MemNew (StringLen (omp->subname) + len + 3);
    if (str == NULL) continue;
    StringCpy (str, orp->taxname);
    StringCat (str, " ");
    StringCat (str, omp->subname);
    omp->subname = MemFree (omp->subname);
    omp->subname = str;
  }
}

extern void PrefixAuthorityWithOrganism (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitBioSourcesInSep (sep, NULL, DoAuthorityPrefix);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

typedef struct addtranslexceptdata {
  FEATURE_FORM_BLOCK

  TexT        cds_comment;
  ButtoN      strict_checking_btn;
  CharPtr     cds_comment_txt;
  Boolean     strict_checking;
} AddTranslExceptData, PNTR AddTranslExceptPtr; 

static void AddTranslExcept (SeqFeatPtr sfp, Pointer userdata)

{
  CdRegionPtr        crp;
  Boolean            partial5, partial3;
  Int4               dna_len;
  CharPtr            bases;
  Int4               except_len;
  Int4               total;
  TransTablePtr      tbl = NULL;
  Int2               state;
  CharPtr            codon_start;
  BioseqPtr          bsp;
  Int4               from, to;
  Boolean            changed;
  CodeBreakPtr       new_cbp, last_cbp;
  Boolean            table_is_local;
  CharPtr            cds_comment = NULL;
  CharPtr            new_comment;
  Int4               comment_len;
  AddTranslExceptPtr ap;
  Boolean            use_strict = FALSE;

  if (sfp == NULL 
      || sfp->idx.subtype != FEATDEF_CDS
      || (crp = (CdRegionPtr)sfp->data.value.ptrvalue)== NULL) {
    return;
  }
  
  ap = (AddTranslExceptPtr) userdata;
  if (ap != NULL)
  {
    cds_comment = ap->cds_comment_txt;
  	use_strict = ap->strict_checking;
  }

  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  if (partial3) return;

  dna_len = SeqLocLen (sfp->location);
  if (partial5 && crp->frame > 1) {
    except_len = (dna_len - crp->frame + 1) % 3;
  } else {
    except_len = dna_len % 3;
  }
  if (except_len == 0) return;

  /* don't add code break if one already exists */
  last_cbp = crp->code_break;
  if (last_cbp != NULL && last_cbp->aa.choice == 1 && last_cbp->aa.value.intvalue == 42) {
    return;
  }
  while (last_cbp != NULL && last_cbp->next != NULL) {
    if (last_cbp->aa.choice == 1 && last_cbp->aa.value.intvalue == 42) {
      return;
    }
    last_cbp = last_cbp->next;
  }

  bases = ReadCodingRegionBases (sfp->location, dna_len, crp->frame, &total);
  if (bases == NULL) return;

  /* don't add transl_except if cds has valid stop codon */
  state = 0;
  codon_start = bases + StringLen (bases) - 6;
  if (codon_start < bases) {
    MemFree (bases);
    return;
  }
  tbl = GetTranslationTable (crp, &table_is_local);
  state = 0;
  state = NextCodonState (tbl, state, (Uint1)*codon_start);
  state = NextCodonState (tbl, state, (Uint1)*(codon_start + 1));
  state = NextCodonState (tbl, state, (Uint1)*(codon_start + 2));
  if (IsOrfStop (tbl, state, TTBL_TOP_STRAND)) {
    MemFree (bases);
    if (table_is_local) {
      TransTableFree (tbl);
    }
    return;
  }
  if (table_is_local) {
    TransTableFree (tbl);
    tbl = NULL;
  }
  
  if (use_strict)
  {
  	if (except_len == 2)
  	{
  	  if (toupper (*(codon_start + 3)) != 'T' || toupper(*(codon_start + 4)) != 'A')
  	  {
  	  	MemFree (bases);
  	  	return;
  	  }
  	}
  	else
  	{
  	  if (toupper (*(codon_start + 3)) != 'T')
  	  {
  	  	MemFree (bases);
  	  	return;
  	  }
  	}
  
  }
  else
  {
    /* don't add transl_except if exception location does not start with 'T' or 'N' */
    if (*(codon_start + 3) != 'T' && *(codon_start + 3) != 't'
        && *(codon_start + 3) != 'N' && *(codon_start + 3) != 'n') {
      MemFree (bases);
      return;
    }
  }
  MemFree (bases);

  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp == NULL) return;
  from = GetOffsetInBioseq (sfp->location, bsp, SEQLOC_LEFT_END);
  to = GetOffsetInBioseq (sfp->location, bsp, SEQLOC_RIGHT_END);

  new_cbp = CodeBreakNew ();
  new_cbp->aa.choice = 1;
  new_cbp->aa.value.intvalue = 42;
  new_cbp->loc = SeqLocMerge (BioseqFindFromSeqLoc (sfp->location), sfp->location, NULL,
                                   FALSE, FALSE, FALSE);
  if (SeqLocStrand (new_cbp->loc) == Seq_strand_minus) {
    new_cbp->loc = SeqLocDelete (new_cbp->loc, SeqLocId (new_cbp->loc),  from + except_len, to,
                  FALSE, &changed);
  } else {
    new_cbp->loc = SeqLocDelete (new_cbp->loc, SeqLocId (new_cbp->loc), from, to - except_len,
                  FALSE, &changed);
  }

  /* add code break to end of list */
  if (last_cbp == NULL) {
    crp->code_break = new_cbp;
  } else {
    last_cbp->next = new_cbp;
  }
  
  /* add comment if there is one */
  if (cds_comment != NULL)
  {
  	if (StringHasNoText (sfp->comment))
  	{
  	  sfp->comment = MemFree (sfp->comment);
  	  sfp->comment = StringSave (cds_comment);
  	}
  	else
  	{
      comment_len = StringLen (sfp->comment) + StringLen (cds_comment) + 2;
      new_comment = (CharPtr) MemNew (sizeof (Char) * comment_len);
      if (new_comment != NULL)
      {
      	StringCpy (new_comment, sfp->comment);
      	StringCat (new_comment, ";");
      	StringCat (new_comment, cds_comment);
      	sfp->comment = MemFree (sfp->comment);
      	sfp->comment = new_comment;
      }
  	}
  }
}

extern void GlobalAddTranslExcept (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  VisitFeaturesInSep (sep, NULL, AddTranslExcept);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static void DoAddTranslExceptWithComment (ButtoN b)
{
  AddTranslExceptPtr ap;
  SeqEntryPtr  sep;

  ap = (AddTranslExceptPtr) GetObjectExtra (b);
  if (ap == NULL) return;
  Hide (ap->form);
  sep = GetTopSeqEntryForEntityID (ap->input_entityID);
  if (sep == NULL) return;
  ap->cds_comment_txt = SaveStringFromText (ap->cds_comment);
  ap->strict_checking = GetStatus (ap->strict_checking_btn);
  if (StringHasNoText (ap->cds_comment_txt))
  {
  	ap->cds_comment_txt = MemFree (ap->cds_comment_txt);
  }
  VisitFeaturesInSep (sep, ap, AddTranslExcept);
  ObjMgrSetDirtyFlag (ap->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ap->input_entityID, 0, 0);
  ap->cds_comment_txt = MemFree (ap->cds_comment_txt);
  Remove (ap->form);
}

extern void AddTranslExceptWithComment (IteM i)
{
  BaseFormPtr        bfp;
  AddTranslExceptPtr ap;
  WindoW             w;
  GrouP              h, g, c;
  ButtoN             b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  ap = (AddTranslExceptPtr) MemNew (sizeof (AddTranslExceptData));
  w = FixedWindow (-50, -33, -10, -10, "Add Translation Exception", NULL);

  SetObjectExtra (w, ap, StdCleanupFormProc);
  ap->form = (ForM) w;
  ap->input_entityID = bfp->input_entityID;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (g, "CDS comment", 0, 0, programFont, 'c');
  ap->cds_comment = DialogText (g, "", 20, NULL);
  ap->strict_checking_btn = CheckBox (h, "Overhang must be T or TA", NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoAddTranslExceptWithComment);
  SetObjectExtra (b, ap, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) ap->strict_checking_btn, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}
