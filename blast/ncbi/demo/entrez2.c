/*   entrez2.c
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
* File Name:  entrez2.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   06/16/00
*
* $Revision: 6.25 $
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

/*--------------*/
/* Header files */
/*--------------*/

#include <vibrant.h>
#include <document.h>
#include <asn.h>
#include <ent2api.h>
#include <pmfapi.h>
#include <urlquery.h>
#include <dlogutil.h>
#include <medview.h>
#include <bspview.h>
#include <objsub.h>
#include <vsm.h>
#include <mmdbapi.h>

#include <entrez2.h>

#define ENTREZ_APP_VERSION "9.30"

#define MAX_QUERY_FORMS 256

#define E2_MSG_NEWWIN   537 /* need to register number instead */

/*----------------------------*/
/* File-wide static variables */
/*----------------------------*/

static ForM              s_mainForm [MAX_QUERY_FORMS];
static ForM              s_docSumForm [MAX_QUERY_FORMS];
static Int2              s_currFormNum = 0;
static TextViewProcs     s_txtViewProcs;

static FonT              titleFont = NULL;

static MedlineViewProcs  medviewprocs;
static SeqViewProcs      seqviewprocs;
static Entrez2Globals    entrezglobals;

static TextViewProcs     txtviewprocs;

static Char              entrezVersion [40];


/*==================================================================*/
/*                                                                  */
/*  QuitProc () -                                                   */
/*                                                                  */
/*==================================================================*/


static void QuitProc (void)

{
  Boolean             dirty;
  Int2                j;
  Int2                num;
  ObjMgrPtr           omp;
  ObjMgrDataPtr PNTR  omdpp;
  ObjMgrDataPtr       tmp;

  dirty = FALSE;
  omp = ObjMgrGet ();
  num = omp->currobj;
  for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
    tmp = *omdpp;
    if (tmp->parentptr == NULL)
      if (tmp->dirty)
        dirty = TRUE;
  }

  if (dirty) {
    if (Message (MSG_YN, "Some data have not been saved.\nAre you sure you" " want to exit?") == ANS_YES) {
      QuitProgram ();
    }
  } else{
    QuitProgram ();
  }
}

/*====================================================================*/
/*                                                                    */
/*  RetrieveUid_Callback () - This is the callback for the Lookup Uid */
/*                           mode. It retrieves the given UID and     */
/*                           displays it in the DocSum window         */
/*                           without adding it to the term list.      */
/*                                                                    */
/*====================================================================*/

static void RetrieveUid_Callback (ForM queryForm, Int4 uid, CharPtr dbName)

{
  if (s_currFormNum >= MAX_QUERY_FORMS) return;
  RetrieveDocs (s_docSumForm [s_currFormNum], 1, 0, &uid, dbName);
}

/*==================================================================*/
/*                                                                  */
/*  RefineUids_Callback ()                                          */
/*                                                                  */
/*==================================================================*/

static void RefineUids_Callback (ForM f, CharPtr term, Int4 num, Int4Ptr uids, Int2 db)

{
  if (num <= 0 || uids == NULL) return;
  if (s_currFormNum >= MAX_QUERY_FORMS) return;

  WatchCursor ();
  Update ();

  RefineUIDs (s_mainForm [s_currFormNum], term, num, uids, db);
  Select (s_mainForm [s_currFormNum]);

  ArrowCursor ();
}

static Int2 GetEntrezAppParam (CharPtr section, CharPtr type, CharPtr dflt, CharPtr buf, size_t buflen)

{
  Int2  rsult;

  rsult = GetAppParam ("ENTREZCUSTOM", section, type, NULL, buf, buflen);
  if (rsult) return rsult;
  rsult = GetAppParam ("ENTREZ", section, type, dflt, buf, buflen);
  return rsult;
}

/*
static Boolean SetEntrezAppParam (CharPtr section, CharPtr type, CharPtr value)

{
  Char  tmp [32];

  if (GetAppParam ("ENTREZCUSTOM", section, type, NULL, tmp, sizeof (tmp) - 1)) {
    return SetAppParam ("ENTREZCUSTOM", section, type, value);
  }
  return SetAppParam ("ENTREZ", section, type, value);
}
*/

/*==================================================================*/
/*                                                                  */
/*  SetupAppProperties () -                                         */
/*                                                                  */
/*==================================================================*/

static void SetupAppProperties (void)

{
  Char  str [32];
  Int2  val;

  MemSet ((Pointer) (&medviewprocs), 0, sizeof (MedlineViewProcs));
  MemSet ((Pointer) (&seqviewprocs), 0, sizeof (SeqViewProcs));
  MemSet ((Pointer) (&entrezglobals), 0, sizeof (Entrez2Globals));
  MemSet ((Pointer) (&s_txtViewProcs), 0, sizeof (TextViewProcs));

  medviewprocs.cleanupObjectPtr = FALSE;
  medviewprocs.closeForm = NULL;
  medviewprocs.useFolderTabs = CHANGE_VIEW_POPUP;
  medviewprocs.showAsnPage = TRUE;
  medviewprocs.useScrollText = FALSE;

  SetAppProperty ("MedlineDisplayForm", &medviewprocs);

  seqviewprocs.hasTargetControl = TRUE;
  seqviewprocs.hasDoneButton = FALSE;
  seqviewprocs.hasDuplicateButton = FALSE;
  seqviewprocs.launchEditors = FALSE;
  seqviewprocs.launchSubviewers = TRUE;
  seqviewprocs.sendSelectMessages = TRUE;
  seqviewprocs.highlightSelections = TRUE;
  seqviewprocs.cleanupObjectPtr = FALSE;
  seqviewprocs.closeForm = NULL;
  seqviewprocs.useFolderTabs = CHANGE_VIEW_POPUP;
  seqviewprocs.allowScrollText = FALSE;
  seqviewprocs.startInScrollText = FALSE;

  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &mapPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &sumPageData);
  /* AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gphPageData); */
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &asn2gphGphPageData);
  /* AddBioseqPageToList (&(seqviewprocs.pageSpecs), &alnPageData); */
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &seqpnlPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &seqAlnPnlPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gbgnPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gnbkPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &emblPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gnptPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &ftblPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &fstaPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &qualPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &asnPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &xmlPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gbseqPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &dskPageData);

  SetAppProperty ("NewSequinGraphicalViewer", (void *) 1024);
  SetAppProperty ("NewSequinLayoutOverride", (void *) 1024);

  SetAppProperty ("SeqDisplayForm", &seqviewprocs);

  if (GetEntrezAppParam ("PREFERENCES", "TEXTEDITDISPLAYS", "", str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0) {
      medviewprocs.useScrollText = TRUE;
      seqviewprocs.allowScrollText = TRUE;
      seqviewprocs.startInScrollText = TRUE;
    }
  }

  if (GetEntrezAppParam ("PREFERENCES", "MINPIXELWIDTH", "", str, sizeof (str))) {
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, screenRect.right);
      medviewprocs.minPixelWidth = val;
      seqviewprocs.minPixelWidth = val;
    }
  }

  if (GetEntrezAppParam ("PREFERENCES", "MINPIXELHEIGHT", "", str, sizeof (str))) {
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, screenRect.bottom);
      medviewprocs.minPixelHeight = val;
      seqviewprocs.minPixelHeight = val;
    }
  }

  if (GetEntrezAppParam ("PREFERENCES", "TEXTPIXELWIDTH", "", str, sizeof (str))) {
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, screenRect.right);
      s_txtViewProcs.minPixelWidth = val;
    }
  }

  if (GetEntrezAppParam ("PREFERENCES", "TEXTPIXELHEIGHT", "", str, sizeof (str))) {
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, screenRect.bottom);
      s_txtViewProcs.minPixelHeight = val;
    }
  }

  if (GetEntrezAppParam ("SETTINGS", "MEDPAGE", "Abstract", str, sizeof (str))) {
    medviewprocs.initMedLabel = StringSaveNoNull (str);
  }

  if (GetEntrezAppParam ("SETTINGS", "NUCPAGE", "GenBank", str, sizeof (str))) {
    seqviewprocs.initNucLabel = StringSaveNoNull (str);
  }

  if (GetEntrezAppParam ("SETTINGS", "PRTPAGE", "GenPept", str, sizeof (str))) {
    seqviewprocs.initProtLabel = StringSaveNoNull (str);
  }

  if (GetEntrezAppParam ("SETTINGS", "GENMPAGE", "Map", str, sizeof (str))) {
    seqviewprocs.initGenomeLabel = StringSaveNoNull (str);
  }

  seqviewprocs.lockFarComponents = TRUE;
  if (GetEntrezAppParam ("SETTINGS", "LOCKFAR", NULL, str, sizeof (str))) {
    if (StringICmp (str, "FALSE") == 0) {
      seqviewprocs.lockFarComponents = FALSE;
    }
  }

  if (GetEntrezAppParam ("SETTINGS", "XMLPREFIX", NULL, str, sizeof (str))) {
    AsnSetXMLmodulePrefix (StringSave (str));
  }

  s_txtViewProcs.useScrollText = FALSE;

  SetAppProperty ("TextDisplayForm", &s_txtViewProcs);

  entrezglobals.persistDefault = TRUE;
  if (GetEntrezAppParam ("PREFERENCES", "PARENTSPERSIST", "", str, sizeof (str) - 1)) {
    if (StringICmp (str, "FALSE") == 0) {
      entrezglobals.persistDefault = FALSE;
    }
  }

  if (GetEntrezAppParam ("PREFERENCES", "ALIGNCHECKED", NULL, str, sizeof (str) - 1)) {
    if (StringICmp (str, "FALSE") == 0) {
      entrezglobals.alignDefault = FALSE;
    }
  }

  if (GetEntrezAppParam ("PREFERENCES", "POPDOWN", "", str, sizeof (str) - 1)) {
    if (StringICmp (str, "TRUE") == 0) {
      entrezglobals.popdownBehavior = TRUE;
    }
  }

  if (GetEntrezAppParam ("PREFERENCES", "LOOKUPDIRECT", "", str, sizeof (str) - 1)) {
    if (StringICmp (str, "TRUE") == 0) {
      entrezglobals.lookupDirect = TRUE;
    } else {
      entrezglobals.lookupDirect = FALSE;
    }
  }

  if (GetEntrezAppParam ("PREFERENCES", "SORTFIELDS", "", str, sizeof (str) - 1)) {
    if (StringICmp (str, "FALSE") == 0) {
      entrezglobals.sortFields = FALSE;
    } else {
      entrezglobals.sortFields = TRUE;
    }
  }

  if (GetEntrezAppParam ("SETTINGS", "DATABASE", "PubMed", str, sizeof (str))) {
    entrezglobals.initDatabase = StringSaveNoNull (str);
  }

  if (GetEntrezAppParam ("SETTINGS", "FIELD", "All Fields", str, sizeof (str))) {
    entrezglobals.initField = StringSaveNoNull (str);
  }

  if (GetEntrezAppParam ("SETTINGS", "MODE", "Automatic", str, sizeof (str))) {
    entrezglobals.initMode = StringSaveNoNull (str);
  }

  SetAppProperty ("Entrez2Globals", &entrezglobals);
}

/*==================================================================*/
/*                                                                  */
/*  SetupCommonFonts ()                                             */
/*                                                                  */
/*==================================================================*/

static FonT ChooseAFont (CharPtr param, CharPtr dfault)

{
  FonT  f;
  Char  str [128];

  f = NULL;
  if (GetEntrezAppParam ("FONTS", param, "", str, sizeof (str))) {
    f = ParseFont (str);
  } else {
    /* SetEntrezAppParam ("FONTS", param, dfault); */
    f = ParseFont (dfault);
  }
  return f;
}

static void SetupCommonFonts (void)

{
#ifdef WIN_MAC
  titleFont = GetFont ("Geneva", 18, TRUE, TRUE, FALSE, "");
#endif
#ifdef WIN_MSWIN
  titleFont = GetFont ("Arial", 24, TRUE, TRUE, FALSE, "");
#endif
#ifdef WIN_MOTIF
  titleFont = GetFont ("Courier", 24, TRUE, TRUE, FALSE, "");
#endif

#ifdef WIN_MAC
  medviewprocs.jourfnt = ChooseAFont ("JOURNAL", "Geneva,10,i");
  medviewprocs.volfnt = ChooseAFont ("VOLUME", "Geneva,10,b");
  medviewprocs.pagesfnt = ChooseAFont ("PAGES", "Geneva,10");
  medviewprocs.titlefnt = ChooseAFont ("TITLE", "Times,14,b");
  medviewprocs.authorsfnt = ChooseAFont ("AUTHORS", "Times,14");
  medviewprocs.affilfnt = ChooseAFont ("AFFILIATION", "Times,12");
  medviewprocs.abstractfnt = ChooseAFont ("ABSTRACT", "Geneva,10");
  medviewprocs.meshfnt = ChooseAFont ("MESH", "Monaco,9");
  medviewprocs.displayFont = ChooseAFont ("DISPLAY", "Monaco,9");
#endif
#ifdef WIN_MSWIN
  medviewprocs.jourfnt = ChooseAFont ("JOURNAL", "Arial,11,i");
  medviewprocs.volfnt = ChooseAFont ("VOLUME", "Arial,11,b");
  medviewprocs.pagesfnt = ChooseAFont ("PAGES", "Arial,11");
  medviewprocs.titlefnt = ChooseAFont ("TITLE", "Times New Roman,14,b");
  medviewprocs.authorsfnt = ChooseAFont ("AUTHORS", "Times New Roman,14");
  medviewprocs.affilfnt = ChooseAFont ("AFFILIATION", "Times New Roman,11");
  medviewprocs.abstractfnt = ChooseAFont ("ABSTRACT", "Times New Roman,11");
  medviewprocs.meshfnt = ChooseAFont ("MESH", "Times New Roman,9");
  medviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier New,10");
#endif
#ifdef WIN_MOTIF
  medviewprocs.jourfnt = ChooseAFont ("JOURNAL", "Helvetica,12,i");
  medviewprocs.volfnt = ChooseAFont ("VOLUME", "Helvetica,12,b");
  medviewprocs.pagesfnt = ChooseAFont ("PAGES", "Helvetica,12");
  medviewprocs.titlefnt = ChooseAFont ("TITLE", "Times,18,b");
  medviewprocs.authorsfnt = ChooseAFont ("AUTHORS", "Times,18");
  medviewprocs.affilfnt = ChooseAFont ("AFFILIATION", "Times,14");
  medviewprocs.abstractfnt = ChooseAFont ("ABSTRACT", "Times,14");
  medviewprocs.meshfnt = ChooseAFont ("MESH", "Times,12");
  medviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier,10");
#endif

#ifdef WIN_MAC
  seqviewprocs.displayFont = ChooseAFont ("DISPLAY", "Monaco,9");
#endif
#ifdef WIN_MSWIN
  seqviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier New,10");
#endif
#ifdef WIN_MOTIF
  seqviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier,10");
#endif

#ifdef WIN_MAC
  entrezglobals.docsumFont = ChooseAFont ("FETCHED", "Monaco,9");
#endif
#ifdef WIN_MSWIN
  entrezglobals.docsumFont = ChooseAFont ("FETCHED", "Courier New,10");
#endif
#ifdef WIN_MOTIF
  entrezglobals.docsumFont = ChooseAFont ("FETCHED", "Courier,10");
#endif

#ifdef WIN_MAC
  txtviewprocs.displayFont = ChooseAFont ("DISPLAY", "Monaco,9");
#endif
#ifdef WIN_MSWIN
  txtviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier New,10");
#endif
#ifdef WIN_MOTIF
  txtviewprocs.displayFont = ChooseAFont ("DISPLAY", "Courier,10");
#endif
}

/*==================================================================*/
/*                                                                  */
/*  GetNewMainFormKey () - Gets the next unused form id.            */
/*                                                                  */
/*==================================================================*/

static Int2 GetNewMainFormKey (void)

{
  Int2  id;

  for (id = 0; id < MAX_QUERY_FORMS; id++) {
    if (s_mainForm [id] == NULL) break;
  }

  if (id < MAX_QUERY_FORMS) {
    return id;
  } else {
    return -1;
  }
}

/*===================================================================*/
/*                                                                   */
/*  RetrieveDocs_Callback () - This is the callback for the          */
/*                            Retrieve ... Docs button. It retrieves */
/*                            the documents that satisfy the current */
/*                            boolean query.                         */
/*                                                                   */
/*===================================================================*/

static void RetrieveDocs_Callback (ButtoN buttonPressed)

{
  Entrez2IdListPtr        e2UidList;
  Int4Ptr                 uidList;
  Entrez2BooleanReplyPtr  e2BooleanPtr;

  /*----------------------------------------*/
  /* Set the cursor to the "working" cursor */
  /*----------------------------------------*/

  WatchCursor ();
  Update ();

  /*-------------------------------------*/
  /* Query the server for a list of UIDs */
  /* that satisfy the current query.     */
  /*-------------------------------------*/

  e2BooleanPtr = Query_FetchUIDs (s_mainForm [s_currFormNum]);
  if (e2BooleanPtr == NULL) {
    ArrowCursor ();
    return;
  }

  e2UidList = e2BooleanPtr->uids;
  uidList = (Int4Ptr) BSMerge (e2UidList->uids, NULL);

  /*---------------------------------*/
  /* Unselect the currently selected */
  /* terms in the term list.         */
  /*---------------------------------*/

  TermList_UnselectAll (s_mainForm [s_currFormNum]);

  /*------------------------------------*/
  /* Send the UIDs to the DocSum window */
  /*------------------------------------*/

  RetrieveDocs (s_docSumForm [s_currFormNum], e2UidList->num, 0, uidList, e2UidList->db);

  /*---------------------------------*/
  /* Cleanup and return successfully */
  /*---------------------------------*/

  Entrez2BooleanReplyFree (e2BooleanPtr);
  ArrowCursor ();
  Update ();
}

/*==================================================================*/
/*                                                                  */
/*  DocumentSummaryActivate_Callback () -                           */
/*                                                                  */
/*==================================================================*/

static void DocumentSummaryActivate_Callback (WindoW w)

{
  ForM  f;
  Int2  formNum;

  f = (ForM) w;

  for (formNum = 0; formNum < MAX_QUERY_FORMS; formNum++) {
    if (s_docSumForm [formNum] == f) {
      s_currFormNum = formNum;
      break;
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  DocumentSummaryMessage_Callback () -                            */
/*                                                                  */
/*==================================================================*/

static void DocumentSummaryMessage_Callback (ForM f, Int2 mssg)

{
  switch (mssg) {
    case VIB_MSG_CLOSE:
      Hide (f);
      break;
    case VIB_MSG_QUIT:
      QuitProc ();
      break;
    default:
      break;
  }
}

/*==================================================================*/
/*                                                                  */
/*  MainActivate_Callback () -                                      */
/*                                                                  */
/*==================================================================*/

static void MainActivate_Callback (WindoW w)

{
  ForM  f;
  Int2  formNum;

  f = (ForM) w;

  for (formNum = 0; formNum < MAX_QUERY_FORMS; formNum++) {
    if (s_mainForm [formNum] != NULL) {
      if (s_mainForm [formNum] == f) {
        s_currFormNum = formNum;
        break;
      }
    }
  }
}

/*==================================================================*/
/*                                                                  */
/*  MainMessage_Callback () -                                       */
/*                                                                  */
/*==================================================================*/

static void MainMessage_Callback (ForM f, Int2 mssg)

{
  switch (mssg) {
    case VIB_MSG_CLOSE:
      QuitProgram ();
      break;
    case VIB_MSG_QUIT:
      QuitProgram ();
      break;
    case E2_MSG_NEWWIN:
      s_currFormNum = GetNewMainFormKey ();
      if (s_currFormNum < 0) return;
      s_mainForm [s_currFormNum] = CreateTermlistForm (-50, -33, "Query",
                                                       MainActivate_Callback,
                                                       MainMessage_Callback,
                                                       RetrieveDocs_Callback,
                                                       RetrieveUid_Callback,
                                                       TRUE, TRUE);
      Show (s_mainForm [s_currFormNum]);
      s_docSumForm [s_currFormNum] = CreateDocsumForm (-10, -90, "Document",
                                                       DocumentSummaryActivate_Callback,
                                                       DocumentSummaryMessage_Callback,
                                                       RefineUids_Callback,
                                                       TRUE);
      break;
    default:
      break;
  }
}

/*==================================================================*/
/*                                                                  */
/*  AboutProc ()                                                    */
/*                                                                  */
/*==================================================================*/

static void CloseAboutPanelProc (PaneL p, PoinT pt)

{
  WindoW  w;

  w = ParentWindow (p);
  Remove (w);
}

static void CenterString (RectPtr rptr, CharPtr text, FonT fnt, Int2 inc)

{
  if (fnt != NULL) {
    SelectFont (fnt);
  }
  rptr->bottom = rptr->top + LineHeight ();
  DrawString (rptr, text, 'c', FALSE);
  rptr->top = rptr->bottom + inc;
}

static void DrawAbout (PaneL p)

{
  RecT  r;

  ObjectRect (p, &r);
  InsetRect (&r, 4, 4);
  r.top += 10;
  Blue ();
  CenterString (&r, "Entrez", titleFont, 5);
  CenterString (&r, entrezVersion, programFont, 5);
  CenterString (&r, "National Center for Biotechnology Information", systemFont, 5);
  CenterString (&r, "National Library of Medicine", systemFont, 5);
  CenterString (&r, "National Institutes of Health", systemFont, 10);
  CenterString (&r, "(301) 496-2475", systemFont, 5);
  CenterString (&r, "info@ncbi.nlm.nih.gov", systemFont, 0);
}

static Int2 AboutBoxWidth (void)

{
  Int2  max;
  Int2  wid;


  SelectFont (titleFont);
  max = StringWidth ("Entrez");
  SelectFont (programFont);
  wid = StringWidth (entrezVersion);
  if (wid > max) {
    max = wid;
  }
  SelectFont (systemFont);
  wid = StringWidth ("National Center for Biotechnology Information");
  if (wid > max) {
    max = wid;
  }
  max += 2 * stdCharWidth + 2;
  return max;
}

static void AboutProc (IteM i)

{
  GrouP   c;
  PaneL   p;
  WindoW  w;

  w = ModalWindow (-50, -33, -1, -1, StdCloseWindowProc);
  p = SimplePanel (w, AboutBoxWidth (), 13 * stdLineHeight, DrawAbout);
  SetPanelClick (p, NULL, NULL, NULL, CloseAboutPanelProc);
  c = HiddenGroup (w, 2, 0, NULL);
  SetGroupSpacing (c, 20, 2);
  SetGroupMargins (c, 5, 5);
  PushButton (c, "Done", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
}

/*==================================================================*/
/*                                                                  */
/*  SetupMacMenus ()                                                */
/*                                                                  */
/*==================================================================*/

#ifdef WIN_MAC
static void DoQuit (IteM i)

{
  QuitProc ();
}

static void SetupMacMenus (void)

{
  MenU  m;

  m = AppleMenu (NULL);
  CommandItem (m, "About Entrez...", AboutProc);
  SeparatorItem (m);
  DeskAccGroup (m);

  m = PulldownMenu (NULL, "File");
  CommandItem (m, "Quit/Q", DoQuit);
}

static void MacDeactProc (WindoW w)

{
  currentFormDataPtr = NULL;
}
#endif

/*==================================================================*/
/*                                                                  */
/*  ConfigForm... ()                                                */
/*                                                                  */
/*==================================================================*/

static void ConfigFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE :
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE :
        StdDeleteTextProc (NULL);
        break;
      default :
        break;
    }
  }
}

static void ConfigAccepted (void)

{
  Message (MSG_OK, "Please rerun Entrez now that it is configured");
  QuitProgram ();
}

static void ConfigCancelled (void)

{
  Message (MSG_OK, "Entrez cannot run without ncbi configuration file");
  QuitProgram ();
}

/*==================================================================*/
/*                                                                  */
/*  Main () - The main controlling logic for this program.          */
/*                                                                  */
/*==================================================================*/

static CharPtr getInfoFailed =
  "Unable to connect to Entrez2 server, use\n\
'setenv CONN_DEBUG_PRINTOUT TRUE' to assist\n\
debugging, or use -c command line argument\n\
for firewall configuration dialog";

static QUEUE bouncequeue = NULL;

static Boolean LIBCALLBACK BounceProc (
  CONN conn, VoidPtr userdata, EIO_Status status
)

{
  BoolPtr  bp;

  if (NetTestReadReply (conn, status)) {
    bp = (BoolPtr) userdata;
    *bp = TRUE;
  }
  return TRUE;
}

Int2 Main (void)

{
  Boolean  advancedQueryToggle;
  Boolean  bouncefound = FALSE;
  time_t   currsecs = 0;
  Boolean  delayedNeighbor;
  Boolean  explodeToggle;
  Int2     i;
  ErrSev   oldsev;
  PaneL    p;
  CharPtr  path = "/entrez/utils/entrez2server.fcgi";
  Uint2    port = 2441;
  time_t   prevsecs = 0;
  CharPtr  server = "www.ncbi.nlm.nih.gov";
  Boolean  showConfigForm = FALSE;
  time_t   starttime = 0;
  time_t   stoptime = 0;
  Char     str [64];
  Boolean  useNormalServ = FALSE;
  Boolean  useTestServ = FALSE;
  Boolean  useURL = FALSE;
  WindoW   w;
#ifdef WIN_MOTIF
  RecT     r;
#endif

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  ProcessUpdatesFirst (FALSE);

  UseLocalAsnloadDataAndErrMsg ();
  ErrPathReset ();

  /*--------------------------------*/
  /* Initialize the global settings */
  /*--------------------------------*/

  SetupAppProperties ();
  SetupCommonFonts ();

  sprintf (entrezVersion, "Entrez Application Version %s", ENTREZ_APP_VERSION);

  w = FixedWindow (-50, -33, -10, -10, "Entrez", NULL);
  p = SimplePanel (w, AboutBoxWidth (), 14 * stdLineHeight, DrawAbout);
  Show (w);
#ifdef WIN_MOTIF
  Select (w);
  ObjectRect (p, &r);
  Select (p);
  InsetRect (&r, 3, 3);
  InvalRect (&r);
#endif
  Update ();

  WatchCursor ();

#ifdef WIN_MAC
  SetDeactivate (NULL, MacDeactProc);
  SetupMacMenus ();
#endif

  /*----------------------------------------------*/
  /* Finish setting up object loader parse tables */
  /*----------------------------------------------*/

  SetTitle (w, "Loading parse tables");
  Update ();

  if (! AllObjLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "AllObjLoad failed");
    return 1;
  }
  if (! SubmitAsnLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "SubmitAsnLoad failed");
    return 1;
  }
  if (! FeatDefSetLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "FeatDefSetLoad failed");
    return 1;
  }
  if (! SeqCodeSetLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "SeqCodeSetLoad failed");
    return 1;
  }
  if (! GeneticCodeTableLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "GeneticCodeTableLoad failed");
    return 1;
  }

  /*--------------------------------*/
  /* Load files from data directory */
  /*--------------------------------*/

  /*
  if (! PrintTemplateSetLoad ("objprt.prt")) {
    ArrowCursor ();
    Message (MSG_FATAL, "PrintTemplateSetLoad objprt.prt failed");
    return 1;
  }
  if (! PrintTemplateSetLoad ("asn2ff.prt")) {
    ArrowCursor ();
    Message (MSG_FATAL, "PrintTemplateSetLoad asn2ff.prt failed");
    return 1;
  }
  */
  if (OpenMMDBAPI ((POWER_VIEW ^ FETCH_ENTREZ), NULL)) {
    GetPRGDDictionary ();
  }

  /*-------------------------------*/
  /* Register program name         */
  /*-------------------------------*/

  EntrezSetProgramName ("Entrez");

  /*-------------------------------*/
  /* Get server URL information    */
  /*-------------------------------*/

#ifdef WIN_MOTIF
  { {
      Nlm_Int4          argc;
      Nlm_CharPtr PNTR  argv;
      Nlm_Int2          j;
      Nlm_Char          tmp [16];
      int               val;

      argc = GetArgc ();
      argv = GetArgv ();
      for (j = 1; j < argc; j++) {
        if (StringCmp (argv [j], "-s") == 0 && j < argc - 1) {
          server = argv [j + 1];
          useURL = TRUE;
        } else if (StringCmp (argv [j], "-r") == 0 && j < argc - 1) {
          StringNCpy_0 (tmp, argv [j + 1], sizeof (tmp));
          if (sscanf (tmp, "%d", &val) == 1) {
            port = (Uint2) val;
            useURL = TRUE;
          }
        } else if (StringCmp (argv [j], "-p") == 0 && j < argc - 1) {
          path = argv [j + 1];
          useURL = TRUE;
        } else if (StringCmp (argv [j], "-n") == 0) {
          useNormalServ = TRUE;
        } else if (StringCmp (argv [j], "-t") == 0) {
          useTestServ = TRUE;
        } else if (StringCmp (argv [j], "-u") == 0) {
          useURL = TRUE;
        } else if (StringCmp (argv [j], "-c") == 0) {
          showConfigForm = TRUE;
        }
      }
  } }
#endif

  if (showConfigForm) {
    Remove (w);
    ArrowCursor ();
    ShowNewNetConfigForm (NULL, ConfigFormMessage,
                          ConfigAccepted, ConfigCancelled,
                          NULL, TRUE);
    ProcessEvents ();
    return 0;
  }

  /*--------------------------------------*/
  /* Optionally choose the Entrez2 server */
  /*--------------------------------------*/

  if (useNormalServ) {
    EntrezSetService ("Entrez2");
  } else if (useTestServ) {
    EntrezSetService ("Entrez2Test");
  } else if (useURL) {
    EntrezSetServer (server, port, path);
  }

  /*---------------------------------------*/
  /* Check for connection to NCBI services */
  /*---------------------------------------*/

  SetTitle (w, "Checking Internet connection to NCBI");
  Update ();

  bouncefound = FALSE;
  starttime = GetSecs ();
  oldsev = ErrSetMessageLevel (SEV_FATAL);
  if (! NetTestAsynchronousQuery (&bouncequeue, BounceProc, (Pointer) &bouncefound)) {
    ArrowCursor ();
    ErrSetMessageLevel (oldsev);
    Message (MSG_FATAL, "NetTestAsynchronousQuery failed");
    return 1;
  }

  /* busy wait here, would normally call NetTestCheckQueue from event loop timer */
  while (! bouncefound) {
    stoptime = GetSecs ();
    currsecs = stoptime - starttime;
    if (currsecs != prevsecs && currsecs > 5) {
      sprintf (str, "Checking connection - %ld seconds", (long) currsecs);
      SetTitle (w, str);
      Update ();
      prevsecs = currsecs;
    }
    if (stoptime - starttime >= 30) {
      ArrowCursor ();
      Message (MSG_OK, "Internet connection attempt timed out, exiting");
      return 1;
    }
    NetTestCheckQueue (&bouncequeue);
  }
  QUERY_CloseQueue (&bouncequeue);
  ErrSetMessageLevel (oldsev);

  /*---------------------------------*/
  /* Get info about the DB server(s) */
  /*---------------------------------*/

  SetTitle (w, "Finding Entrez2 Server");
  Update ();

  if (Query_GetInfo () == NULL) {
    ArrowCursor ();
    Message (MSG_OK, "%s", getInfoFailed);
    QuitProgram ();
    return 1;
  }

  if (! PubSeqFetchEnable ()) {
    Message (MSG_POSTERR, "PubSeqFetchEnable failed");
  }

  /*--------------------------------*/
  /* Register viewers, NCBI Desktop */
  /*--------------------------------*/

  SetTitle (w, "Registering Viewers");
  Update ();

  REGISTER_MEDLINE_VIEW;
  REGISTER_NEW_SEQENTRY_VIEW;

  VSMFileInit ();
  VSeqMgrInit (FALSE);

  /*-------------------------------------------*/
  /* Get some preferences from the config file */
  /*-------------------------------------------*/

  advancedQueryToggle = FALSE;
  if (GetEntrezAppParam ("PREFERENCES", "ADVQUERYMODE", "FALSE", str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0 || StringICmp (str, "YES") == 0) {
      advancedQueryToggle = TRUE;
    }
  }

  explodeToggle = FALSE;
  if (GetEntrezAppParam ("PREFERENCES", "EXPLODETERMS", "TRUE", str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0 || StringICmp (str, "YES") == 0) {
      explodeToggle = TRUE;
    }
  }

  delayedNeighbor = FALSE;
  if (GetEntrezAppParam ("PREFERENCES", "DELAYEDNEIGHBOR", "TRUE", str, sizeof (str))) {
    if (StringICmp (str, "TRUE") == 0 || StringICmp (str, "YES") == 0) {
      delayedNeighbor = TRUE;
    }
  }

  /*-----------------------------*/
  /* Create the term list window */
  /*-----------------------------*/

  for (i = 0; i < MAX_QUERY_FORMS; i++) {
    s_mainForm [i] = NULL;
    s_docSumForm [i] = NULL;
  }

  s_currFormNum = GetNewMainFormKey ();

  SetTitle (w, "Creating Query Window");
  Update ();

  s_mainForm [s_currFormNum] = CreateTermlistForm (-50, -33,
                                                   "Query",
                                                   MainActivate_Callback, MainMessage_Callback,
                                                   RetrieveDocs_Callback, RetrieveUid_Callback,
                                                   explodeToggle, advancedQueryToggle);
  Show (s_mainForm [s_currFormNum]);

  /*------------------------------------*/
  /* Create the Document Summary window */
  /*------------------------------------*/

  SetTitle (w, "Creating Document Window");
  Update ();

  s_docSumForm [s_currFormNum] = CreateDocsumForm (-10, -90, "Document",
                                                   DocumentSummaryActivate_Callback,
                                                   DocumentSummaryMessage_Callback,
                                                   RefineUids_Callback,
                                                   delayedNeighbor);

  /*----------------------------------------*/
  /* Hang out and wait for things to happen */
  /*----------------------------------------*/

  Remove (w);
  ArrowCursor ();

  ProcessEvents ();

  /*------*/
  /* Exit */
  /*------*/

  PubSeqFetchDisable ();

  return 0;
}

