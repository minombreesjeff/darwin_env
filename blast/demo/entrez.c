/*   entrez.c
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
* File Name:  entrez.c
*
* Author:  Jonathan Kans, Jonathan Epstein
*
* Version Creation Date:   8/5/96
*
* $Revision: 6.34 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: entrez.c,v $
* Revision 6.34  2000/12/05 17:52:58  kans
* removed cn3d, which caused ddv link problem
*
* Revision 6.33  2000/04/21 23:13:56  kans
* added qualPageData
*
* Revision 6.32  2000/03/02 17:49:24  kans
* call ID1 fetch enable/disable in preference to old Entrez service for sequence fetch
*
* Revision 6.31  2000/03/01 23:52:25  kans
* comment out ID1 fetch for now
*
* Revision 6.30  2000/03/01 22:20:11  kans
* raised version number, calls ID1BioseqFetchEnable to fetch sequences
*
* Revision 6.29  2000/02/11 23:04:49  kans
* remove loadUidItem and saveUidItem - use import and export
*
* Revision 6.28  1999/11/12 23:06:56  kans
* DoLoadNamedUidList takes Int4 num argument
*
* Revision 6.27  1999/11/04 15:53:20  kans
* raised version number for public release
*
* Revision 6.26  1999/08/27 15:59:09  kans
* removed obsolete load and save uid list commands
*
* Revision 6.25  1999/08/24 18:05:58  kans
* has target control, use delayed neighbor policy
*
* Revision 6.24  1999/05/27 16:39:24  kans
* preferences to options menu
*
* Revision 6.23  1999/05/12 17:41:48  kans
* raised version number for public release
*
* Revision 6.22  1999/01/14 19:07:51  kans
* new parameters to Cn3DWin_Entrez
*
* Revision 6.21  1999/01/13 20:09:18  kans
* added neighbor policy control
*
* Revision 6.20  1999/01/08 18:27:35  kans
* commented out makerpt.prt load - not used
*
* Revision 6.19  1999/01/03 22:47:05  kans
* raised version number
*
* Revision 6.18  1998/12/08 16:52:41  kans
* new params to ShowNetConfigForm to simplify configuration
*
* Revision 6.17  1998/12/08 03:19:56  kans
* changes for web dispatcher only config form
*
* Revision 6.16  1998/12/04 23:35:45  kans
* comment out requirement for ncbi config file, now that cdconfig.c is kludged with hard-coded values to psrventr
*
* Revision 6.15  1998/11/11 02:06:31  kans
* set browser path from config file for PubMed launch
*
* Revision 6.14  1998/05/04 15:46:06  kans
* obsoleted load/save uid list, in favor of import/export
*
* Revision 6.13  1998/04/21 12:57:22  kans
* make viewer link controls passed FALSE for no blast choices
*
* Revision 6.12  1998/04/13 20:00:33  kans
* forgot to add import item to mac
*
* Revision 6.11  1998/04/13 19:50:46  kans
* term list now uses import/export
*
* Revision 6.10  1998/03/18 21:31:53  kans
* medline close should remove form, not go to (bioseq) CloseProc
*
* Revision 6.9  1998/02/23 16:14:49  kans
* copy to clipboard has been implemented for motif
*
* Revision 6.8  1998/02/21 22:23:20  kans
* docsum copy menu item if not motif
*
* Revision 6.7  1998/02/21 22:13:00  kans
* enable copyItem for medview, bspview, docsum windows
*
* Revision 6.6  1997/12/04 17:29:20  kans
* raised version
*
* Revision 6.5  1997/11/01 22:59:56  kans
* handle alignDefault preference
*
* Revision 6.4  1997/10/30 23:47:57  kans
* copy alignWithChecked flag
*
* Revision 6.3  1997/10/02 16:16:33  kans
* added AboutBoxWidth, raised version number
*
* Revision 6.2  1997/09/23 16:04:00  kans
* raised version number
*
* Revision 6.1  1997/09/02 16:07:11  kans
* raised version number for new ASN.1 spec
*
* Revision 6.0  1997/08/25 18:19:38  madden
* Revision changed to 6.0
*
* Revision 1.19  1997/07/31 16:43:34  kans
* incremented version number
*
* Revision 1.18  1997/07/23 18:10:27  kans
* DS and TL specific Save/LoadUidProcList functions
*
* Revision 1.17  1997/06/30 18:07:54  kans
* duplicate says duplicate view
*
* Revision 1.16  1997/06/23 15:13:38  kans
* legend button changed to menu item
*
* Revision 1.15  1997/05/16 16:55:07  kans
* register summary, sequence views
*
* Revision 1.14  1997/04/29 19:45:16  kans
* use duplicate view button, allow send and receive messages
*
 * Revision 1.13  1997/04/27  20:58:25  kans
 * enable loadSaveUidList by default
 *
 * Revision 1.12  1997/04/24  21:44:21  kans
 * added compressed page view
 *
 * Revision 1.11  1997/04/21  15:37:09  kans
 * optional calls to LoadUidListProc and SaveUidListProc
 *
 * Revision 1.10  1997/04/16  13:59:31  kans
 * implemented about detailed info
 *
 * Revision 1.9  1997/04/14  19:33:26  kans
 * uses expert boolean mode, clear unused query terms
 *
 * Revision 1.8  1997/04/03  14:41:40  kans
 * add salsa align page, txtviewprocs for NCBI DeskTop, gen text viewer size
 *
 * Revision 1.7  1997/03/20  19:06:06  vakatov
 * [!WIN16] #include <cn3dentr.h>  -- instead of <cn3dmain.h>
 * [!WIN16] Use Cn3DWin_Entrez() instead of Cn3DWin() everywhere
 *
*
* ==========================================================================
*/

#include <vibrant.h>
#include <document.h>
#include <fstyle.h>
#include <medview.h>
#include <bspview.h>
#include <accentr.h>
#include <accid1.h>
#include <lsqfetch.h>
#include <objsub.h>
#include <gather.h>
#include <netcnfg.h>
#include <vsm.h>

#include <objmime.h>
#include <mmdbapi.h>
/*
#ifndef WIN16
#include <cn3dentr.h>
#endif
*/

#include <entrez.h>

#define ENTREZ_APP_VERSION "7.50"

static ForM  termListForm = NULL;
static ForM  docSumForm = NULL;

#ifdef WIN_MAC
static IteM  closeItem = NULL;
static IteM  duplicateItem = NULL;
static IteM  exportItem = NULL;
static IteM  importItem = NULL;
/*
static IteM  saveItem = NULL;
static IteM  saveAsItem = NULL;
*/
static IteM  printItem = NULL;
static IteM  cutItem = NULL;
static IteM  copyItem = NULL;
static IteM  pasteItem = NULL;
static IteM  deleteItem = NULL;
static IteM  docsumfontItem = NULL;
static IteM  displayfontItem = NULL;
static IteM  preferencesItem = NULL;
static IteM  clearUnusedItem = NULL;
static IteM  legendItem = NULL;
static ChoicE  queryChoice = NULL;
static ChoicE  neighborChoice = NULL;

static Boolean  termListUp = FALSE;
static Boolean  docSumUp = FALSE;
#endif

static Boolean  macLike = FALSE;
static Boolean  showAsnPage = FALSE;

static FonT  titleFont = NULL;

static MedlineViewProcs   medviewprocs;
static SeqViewProcs       seqviewprocs;
static EntrezGlobals      entrezglobals;

static StdEditorProcs     stdedprocs;
static TextViewProcs      txtviewprocs;

static Char  globalPath [PATH_MAX];
static Char  entrezRelease [40];
static Char  entrezVersion [40];

static PRGD  prgdDict;

static void AboutMoreProc (ButtoN b)

{
  DoC      d;
  CharPtr  info;
  WindoW   w;

  Remove (ParentWindow (b));
  w = ModalWindow (-50, -33, -1, -1, StdCloseWindowProc);
  d = DocumentPanel (w, 30 * stdCharWidth, 16 * stdLineHeight);
  b = PushButton (w, "Done", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) d, (HANDLE) b, NULL);
  RealizeWindow (w);
  info = EntrezDetailedInfo ();
  if (info == NULL) {
    info = "No detailed information available\n";
  }
  AppendText (d, info, NULL, NULL, programFont);
  Show (w);
  Select (w);
}

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
  CenterString (&r, entrezRelease, programFont, 10);
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
  wid = StringWidth (entrezRelease);
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
  GrouP    c;
  CharPtr  info;
  PaneL    p;
  WindoW   w;

  w = ModalWindow (-50, -33, -1, -1, StdCloseWindowProc);
  p = SimplePanel (w, AboutBoxWidth (), 14 * stdLineHeight, DrawAbout);
  SetPanelClick (p, NULL, NULL, NULL, CloseAboutPanelProc);
  c = HiddenGroup (w, 2, 0, NULL);
  SetGroupSpacing (c, 20, 2);
  SetGroupMargins (c, 5, 5);
  PushButton (c, "Done", StdCancelButtonProc);
  info = EntrezDetailedInfo ();
  if (info != NULL) {
    PushButton (c, "More", AboutMoreProc);
  }
  AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
}

#ifdef WIN_MAC
static void MedlineViewFormActivated (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  RepeatProcOnHandles (Enable,
                   (HANDLE) closeItem,
                   (HANDLE) duplicateItem,
                   (HANDLE) exportItem,
                   (HANDLE) printItem,
                   (HANDLE) copyItem,
                   (HANDLE) displayfontItem,
                   NULL);
}

static void BioseqViewFormActivated (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  RepeatProcOnHandles (Enable,
                   (HANDLE) closeItem,
                   (HANDLE) duplicateItem,
                   (HANDLE) exportItem,
                   (HANDLE) printItem,
                   (HANDLE) copyItem,
                   (HANDLE) displayfontItem,
                   NULL);
}

static void TermSelectionActivateProc (WindoW w)

{
  termListUp = TRUE;
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  if (UsingTextQuery ((ForM) w)) {
    SafeSetValue (queryChoice, 2);
  } else {
    SafeSetValue (queryChoice, 1);
  }
  RepeatProcOnHandles (Enable,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) preferencesItem,
                   (HANDLE) queryChoice,
                   (HANDLE) clearUnusedItem,
                   NULL);
}

static void DocumentSummaryActivateProc (WindoW w)

{
  docSumUp = TRUE;
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  if (UsingDelayedNeighbor ((ForM) w)) {
    SafeSetValue (neighborChoice, 2);
  } else {
    SafeSetValue (neighborChoice, 1);
  }
  RepeatProcOnHandles (Enable,
                   (HANDLE) closeItem,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   (HANDLE) printItem,
                   (HANDLE) copyItem,
                   (HANDLE) docsumfontItem,
                   (HANDLE) preferencesItem,
                   (HANDLE) neighborChoice,
                   NULL);
}

static void StdEditorFormActivated (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
}

static void HelpActivateProc (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
}

static void ConfigFormActivated (WindoW w)

{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  RepeatProcOnHandles (Enable,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   NULL);
}

static void MacDeactProc (WindoW w)

{
  termListUp = FALSE;
  docSumUp = FALSE;
  currentFormDataPtr = NULL;
  SafeSetTitle (exportItem, "Export...");
  SafeSetTitle (importItem, "Import...");
  SafeSetValue (queryChoice, 0);
  SafeSetValue (neighborChoice, 0);
  RepeatProcOnHandles (Disable,
                   (HANDLE) closeItem,
                   (HANDLE) duplicateItem,
                   (HANDLE) importItem,
                   (HANDLE) exportItem,
                   (HANDLE) printItem,
                   (HANDLE) cutItem,
                   (HANDLE) copyItem,
                   (HANDLE) pasteItem,
                   (HANDLE) deleteItem,
                   (HANDLE) docsumfontItem,
                   (HANDLE) displayfontItem,
                   (HANDLE) preferencesItem,
                   (HANDLE) queryChoice,
                   (HANDLE) clearUnusedItem,
                   (HANDLE) neighborChoice,
                   (HANDLE) legendItem,
                   NULL);
}
#endif

#ifndef WIN_MAC
#define MedlineViewFormActivated NULL
#define BioseqViewFormActivated NULL
#define TermSelectionActivateProc NULL
#define DocumentSummaryActivateProc NULL
#define StdEditorFormActivated NULL
#define HelpActivateProc NULL
#define ConfigFormActivated NULL
#endif

static void HideHelpForm (ButtoN b)

{
  Hide (ParentWindow (b));
}

static void DisplayHelpFormProc (IteM i)

{
  Message (MSG_OK, "Help not yet implemented");
}

static void QuitProc (void)

{
  Boolean        dirty;
  Int2           j;
  Int2           num;
  ObjMgrPtr      omp;
  ObjMgrDataPtr  PNTR omdpp;
  ObjMgrDataPtr  tmp;

  dirty = FALSE;
  omp = ObjMgrGet ();
  num = omp->currobj;
  for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
    tmp = *omdpp;
    if (tmp->parentptr == NULL) {
      if (tmp->dirty) {
        dirty = TRUE;
      }
    }
  }
  if (dirty) {
    if (Message (MSG_YN,
        "Some data have not been saved.\nAre you sure you want to exit?") == ANS_YES) {
      QuitProgram ();
    }
  } else {
    QuitProgram ();
  }
}

static void CloseProc (BaseFormPtr bfp)

{
  Int2           j;
  Int2           num;
  Boolean        numview;
  ObjMgrPtr      omp;
  ObjMgrDataPtr  PNTR omdpp;
  OMUserDataPtr  omudp;
  ObjMgrDataPtr  tmp;

  if (bfp != NULL) {
    omp = ObjMgrGet ();
    num = omp->currobj;
    for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
      tmp = *omdpp;
      if (tmp->parentptr == NULL && tmp->dirty &&
          tmp->EntityID == bfp->input_entityID) {
        numview = 0;
        for (omudp = tmp->userdata; omudp != NULL; omudp = omudp->next) {
          if (omudp->proctype == OMPROC_VIEW) {
            numview++;
          }
        }
        if (numview < 2) {
          if (Message (MSG_OKC,
              "Closing the window will lose unsaved data.") != ANS_OK) {
            return;
          }
        }
      }
    }
    numview = 0;
    for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
      tmp = *omdpp;
      if (tmp->parentptr == NULL) {
        for (omudp = tmp->userdata; omudp != NULL; omudp = omudp->next) {
          if (omudp->proctype == OMPROC_VIEW) {
            numview++;
          }
        }
      }
    }
    Remove (bfp->form);
    if (numview <= 1) {
      /*
      WatchCursor ();
      Update ();
      Message (MSG_OK, "No more viewers present.");
      ArrowCursor ();
      Update ();
      */
    }
  }
}

static Int2 GetEntrezAppParam (CharPtr section, CharPtr type, CharPtr dflt, CharPtr buf, Int2 buflen)

{
  Int2  rsult;

  rsult = GetAppParam ("ENTREZCUSTOM", section, type, NULL, buf, buflen);
  if (rsult) return rsult;
  rsult = GetAppParam ("ENTREZ", section, type, dflt, buf, buflen);
  return rsult;
}

static Boolean SetEntrezAppParam (CharPtr section, CharPtr type, CharPtr value)

{
  Char  tmp [32];

  if (GetAppParam ("ENTREZCUSTOM", section, type, NULL, tmp, sizeof (tmp) - 1)) {
    return SetAppParam ("ENTREZCUSTOM", section, type, value);
  }
  return SetAppParam ("ENTREZ", section, type, value);
}

static void DuplicateViewProc (IteM i)

{
  BaseFormPtr  bfp;
  Int2         handled;
  Uint2        itemID;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  if (bfp->input_itemtype == OBJ_BIOSEQ) {
    WatchCursor ();
    itemID = bfp->input_itemID;
    if (itemID == 0) {
      itemID = 1;
    }
    handled = GatherProcLaunch (OMPROC_VIEW, FALSE, bfp->input_entityID, itemID,
                                OBJ_BIOSEQ, 0, 0, OBJ_BIOSEQ, 0);
    ArrowCursor ();
    if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
      Message (MSG_ERROR, "Unable to launch additional viewer.");
    }
  } else if (bfp->input_itemtype == OBJ_MEDLINE_ENTRY) {
    WatchCursor ();
    itemID = bfp->input_itemID;
    if (itemID == 0) {
      itemID = 1;
    }
    handled = GatherProcLaunch (OMPROC_VIEW, FALSE, bfp->input_entityID, itemID,
                                OBJ_MEDLINE_ENTRY, 0, 0, OBJ_MEDLINE_ENTRY, 0);
    ArrowCursor ();
    if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
      Message (MSG_ERROR, "Unable to launch additional viewer.");
    }
  }
}

static void AddAboutAndHelpMenuItems (MenU m)

{
  CommandItem (m, "About Entrez...", AboutProc);
  CommandItem (m, "Help...", DisplayHelpFormProc);
  SeparatorItem (m);
}

typedef struct prefsform {
  FORM_MESSAGE_BLOCK
  DialoG             prefs;
} PrefsForm, PNTR PrefsFormPtr;

static void ReplaceString (CharPtr PNTR target, CharPtr newstr)

{
  if (target == NULL) return;
  MemFree (*target);
  *target = StringSaveNoNull (newstr);
}

static void SetEntrezAppParamTF (CharPtr section, CharPtr type, Boolean value)

{
  if (value) {
    SetEntrezAppParam (section, type, "TRUE");
  } else {
    SetEntrezAppParam (section, type, "FALSE");
  }
}

static void AcceptPrefsProc (ButtoN b)

{
  EntrezPrefsPtr  epp;
  PrefsFormPtr    pfp;

  pfp = (PrefsFormPtr) GetObjectExtra (b);
  if (pfp == NULL) return;
  Hide (pfp->form);
  epp = (EntrezPrefsPtr) DialogToPointer (pfp->prefs);
  if (epp != NULL) {
    entrezglobals.persistDefault = epp->persistDefault;
    entrezglobals.alignDefault = epp->alignDefault;
    entrezglobals.lookupDirect = epp->lookupDirect;
    entrezglobals.showAsn = epp->showAsn;
    ReplaceString (&entrezglobals.initDatabase, epp->initDatabase);
    ReplaceString (&entrezglobals.initField, epp->initField);
    ReplaceString (&entrezglobals.initMode, epp->initMode);
    ReplaceString (&medviewprocs.initMedLabel, epp->initMedLabel);
    ReplaceString (&seqviewprocs.initNucLabel, epp->initNucLabel);
    ReplaceString (&seqviewprocs.initProtLabel, epp->initProtLabel);
    ReplaceString (&seqviewprocs.initGenomeLabel, epp->initGenomeLabel);
    SetEntrezAppParamTF ("PREFERENCES", "PARENTSPERSIST", entrezglobals.persistDefault);
    SetEntrezAppParamTF ("PREFERENCES", "ALIGNCHECKED", entrezglobals.alignDefault);
    SetEntrezAppParamTF ("PREFERENCES", "LOOKUPDIRECT", entrezglobals.lookupDirect);
    SetEntrezAppParamTF ("PREFERENCES", "SHOWASNPAGE", entrezglobals.showAsn);
    SetEntrezAppParam ("SETTINGS", "DATABASE", entrezglobals.initDatabase);
    SetEntrezAppParam ("SETTINGS", "FIELD", entrezglobals.initField);
    SetEntrezAppParam ("SETTINGS", "MODE", entrezglobals.initMode);
    SetEntrezAppParam ("SETTINGS", "MEDPAGE", medviewprocs.initMedLabel);
    SetEntrezAppParam ("SETTINGS", "NUCPAGE", seqviewprocs.initNucLabel);
    SetEntrezAppParam ("SETTINGS", "PRTPAGE", seqviewprocs.initProtLabel);
    SetEntrezAppParam ("SETTINGS", "GENMPAGE", seqviewprocs.initGenomeLabel);
  }
  EntrezPrefsFree (epp);
  Remove (pfp->form);
}

static void DefaultMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

static void PreferencesProc (IteM i)

{
  ButtoN          b;
  GrouP           c;
  EntrezPrefsPtr  epp;
  GrouP           g;
  PrefsFormPtr    pfp;
  WindoW          w;

  pfp = (PrefsFormPtr) MemNew (sizeof (PrefsForm));
  if (pfp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Preferences", StdCloseWindowProc);
  SetObjectExtra (w, pfp, StdCleanupFormProc);
  pfp->form = (ForM) w;
  pfp->formmessage = DefaultMessageProc;
  g = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);
  pfp->prefs = CreateEntrezPrefsDialog (g, NULL);
  c = HiddenGroup (g, 2, 0, NULL);
  b = PushButton (c, "Accept", AcceptPrefsProc);
  SetObjectExtra (b, pfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) pfp->prefs, (HANDLE) c, NULL);
  RealizeWindow (w);
  epp = EntrezPrefsNew ();
  if (epp != NULL) {
    epp->persistDefault = entrezglobals.persistDefault;
    epp->alignDefault = entrezglobals.alignDefault;
    epp->lookupDirect = entrezglobals.lookupDirect;
    epp->showAsn = entrezglobals.showAsn;
    epp->initDatabase = StringSaveNoNull (entrezglobals.initDatabase);
    epp->initField = StringSaveNoNull (entrezglobals.initField);
    epp->initMode = StringSaveNoNull (entrezglobals.initMode);
    epp->initMedLabel = StringSaveNoNull (medviewprocs.initMedLabel);
    epp->initNucLabel = StringSaveNoNull (seqviewprocs.initNucLabel);
    epp->initProtLabel = StringSaveNoNull (seqviewprocs.initProtLabel);
    epp->initGenomeLabel = StringSaveNoNull (seqviewprocs.initGenomeLabel);
  }
  PointerToDialog (pfp->prefs, (Pointer) epp);
  EntrezPrefsFree (epp);
  Show (w);
  Select (w);
}

static void TermSelectionFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        QuitProc ();
        break;
      case VIB_MSG_QUIT :
        QuitProc ();
        break;
      default :
        break;
    }
  }
}

static void StyleManagerProc (IteM i)

{
  MuskStyleManager ();
}


/*
#ifndef WIN16
static void Cn3DWinShowProc (IteM i)
{
  WindoW  w;

  w = Cn3DWin_Entrez(NULL, TRUE);
  if (w == NULL) return;
  Show (w);
  Select (w);
}
#endif
*/


#ifndef WIN_MAC
static void MedlineViewFormMenus (WindoW w)

{
  BaseFormPtr  bfp;
  IteM         i;
  MenU         m;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    i = CommandItem (m, "Duplicate View", DuplicateViewProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (m);
    FormCommandItem (m, "Export...", bfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    /*
    FormCommandItem (m, "Save", bfp, VIB_MSG_SAVE);
    FormCommandItem (m, "Save As...", bfp, VIB_MSG_SAVE_AS);
    SeparatorItem (m);
    */
    FormCommandItem (m, "Print...", bfp, VIB_MSG_PRINT);

    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);
  }
}

static void BioseqViewFormMenus (WindoW w)

{
  BaseFormPtr  bfp;
  IteM         i;
  MenU         m;
  MenU         sub;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    i = CommandItem (m, "Duplicate View", DuplicateViewProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (m);
    FormCommandItem (m, "Export...", bfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    /*
    FormCommandItem (m, "Save", bfp, VIB_MSG_SAVE);
    FormCommandItem (m, "Save As...", bfp, VIB_MSG_SAVE_AS);
    SeparatorItem (m);
    */
    FormCommandItem (m, "Print...", bfp, VIB_MSG_PRINT);

    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);

    m = PulldownMenu (w, "Misc");
    sub = SubMenu (m, "Font Selection");
    i = CommandItem (sub, "Display Font...", DisplayFontChangeProc);
    SetObjectExtra (i, bfp, NULL);
    SeparatorItem (m);
    CreateLegendItem (m, bfp);
  }
}

static void TermListFormMenus (WindoW w)

{
  BaseFormPtr  bfp;
  IteM         i;
  MenU         m;
  MenU         sub;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    AddAboutAndHelpMenuItems (m);
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    FormCommandItem (m, "Import Uid List...", bfp, VIB_MSG_IMPORT);
    FormCommandItem (m, "Export Uid List...", bfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    FormCommandItem (m, "Quit/Q", bfp, VIB_MSG_QUIT);

    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, CUT_MENU_ITEM, bfp, VIB_MSG_CUT);
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);
    FormCommandItem (m, PASTE_MENU_ITEM, bfp, VIB_MSG_PASTE);
    FormCommandItem (m, CLEAR_MENU_ITEM, bfp, VIB_MSG_DELETE);

    m = PulldownMenu (w, "Options");
    CommandItem (m, "Preferences...", PreferencesProc);
    SeparatorItem (m);
    sub = SubMenu (m, "Query Style");
    CreateQueryTypeChoice (sub, bfp);
    CreateClearUnusedItem (m, bfp);

    m = PulldownMenu (w, "Misc");
    VSMAddToMenu (m, VSM_DESKTOP);
    SeparatorItem (m);
    CommandItem (m, "Style Manager...", StyleManagerProc);
/*
#ifndef WIN16
    SeparatorItem (m);
    CommandItem (m, "Cn3D Window...", Cn3DWinShowProc);
#endif
*/
  }
}

static void DocSumFormMenus (WindoW w)

{
  BaseFormPtr  bfp;
  ChoicE       c;
  IteM         i;
  MenU         m;
  MenU         sub;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    SeparatorItem (m);
    FormCommandItem (m, "Import...", bfp, VIB_MSG_IMPORT);
    FormCommandItem (m, "Export...", bfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    /*
    FormCommandItem (m, "Save", bfp, VIB_MSG_SAVE);
    FormCommandItem (m, "Save As...", bfp, VIB_MSG_SAVE_AS);
    SeparatorItem (m);
    */
    FormCommandItem (m, "Print...", bfp, VIB_MSG_PRINT);

    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);

    m = PulldownMenu (w, "Options");
    CommandItem (m, "Preferences...", PreferencesProc);
    SeparatorItem (m);
    sub = SubMenu (m, "Neighbor Policy");
    c = CreateNeighborDelayChoice (sub, bfp);
    SetValue (c, 2); /* UseDelayedNeighbor */
    SeparatorItem (m);
    LoadDocsumOptionsMenu (m);
    seqviewprocs.alignWithChecked = entrezglobals.alignWithChecked;

    m = PulldownMenu (w, "Misc");
    sub = SubMenu (m, "Font Selection");
    i = CommandItem (sub, "DocSum Font...", DocSumFontChangeProc);
    SetObjectExtra (i, bfp, NULL);
    i = CommandItem (sub, "Display Font...", DisplayFontChangeProc);
    SetObjectExtra (i, bfp, NULL);
  }
}
#endif

static void EntrezMedlineFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      case VIB_MSG_QUIT :
        QuitProc ();
        break;
      default :
        break;
    }
  }
}

static void EntrezSeqViewFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        CloseProc (bfp);
        break;
      case VIB_MSG_QUIT :
        QuitProc ();
        break;
      default :
        break;
    }
  }
}

static void DocumentSummaryFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Hide (f);
        break;
      case VIB_MSG_QUIT :
        QuitProc ();
        break;
      default :
        break;
    }
  }
}

static void EntrezStdEditorFormMessage (ForM f, Int2 mssg)

{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      case VIB_MSG_QUIT :
        QuitProc ();
        break;
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

static void ProcessHelpMessage (CharPtr heading, CharPtr section)

{
}

/*
*  The following callbacks process requests between forms.  This can be extended
*  to have the application track multiple term list and docsum windows.
*/

static void DoRetrieveDocuments (ForM f, Int2 num, Int2 parents, Int4Ptr uids, Int2 db)

{
  RetrieveDocuments (docSumForm, num, parents, uids, db);
}

static void DoLoadNamedUidList (ForM f, CharPtr term, Int4 num, Int4Ptr uids, Int2 db)

{
  LoadNamedUidList (termListForm, term, num, uids, db);
}

static void DoLaunchRecordViewer (ForM f, Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db)

{
  LaunchRecordViewer (uid, numAlign, alignuids, db);
}

static GrouP DoMakeViewerLinkControls (GrouP prnt, BaseFormPtr bfp, Int2 doctype, Int4 uid)

{
  return MakeViewerLinkControls (prnt, bfp, doctype, uid, FALSE);
}

static void SetupAppProperties (void)

{
  Char     str [32];
  Int2     val;

  showAsnPage = TRUE;
  if (GetEntrezAppParam ("PREFERENCES", "SHOWASNPAGE", "", str, sizeof (str) - 1)) {
    if (StringICmp (str, "FALSE") == 0) {
      showAsnPage = FALSE;
    }
  }

  MemSet ((Pointer) (&medviewprocs), 0, sizeof (MedlineViewProcs));
  medviewprocs.cleanupObjectPtr = FALSE;
  medviewprocs.activateForm = MedlineViewFormActivated;
  medviewprocs.closeForm = NULL;
  medviewprocs.useFolderTabs = CHANGE_VIEW_POPUP;
  /*
  medviewprocs.initPage = CITATION_PAGE;
  */
#ifndef WIN_MAC
  medviewprocs.createMenus = MedlineViewFormMenus;
#endif
  medviewprocs.showAsnPage = showAsnPage;
  medviewprocs.useScrollText = FALSE;
  medviewprocs.handleMessages = EntrezMedlineFormMessage;
  medviewprocs.makeControls = DoMakeViewerLinkControls;
  SetAppProperty ("MedlineDisplayForm", &medviewprocs);

  MemSet ((Pointer) (&seqviewprocs), 0, sizeof (SeqViewProcs));
  seqviewprocs.hasTargetControl = TRUE;
  seqviewprocs.hasDoneButton = FALSE;
  seqviewprocs.hasDuplicateButton = FALSE;
  seqviewprocs.launchEditors = FALSE;
  seqviewprocs.launchSubviewers = TRUE;
  seqviewprocs.sendSelectMessages = TRUE;
  seqviewprocs.highlightSelections = TRUE;
  seqviewprocs.cleanupObjectPtr = FALSE;
  seqviewprocs.activateForm = BioseqViewFormActivated;
  seqviewprocs.closeForm = NULL;
  seqviewprocs.useFolderTabs = CHANGE_VIEW_POPUP;
  /*
  seqviewprocs.initNucPage = NUCASN2FF_PAGE_1;
  seqviewprocs.initProtPage = PROTGENPEPT_PAGE;
  */
#ifndef WIN_MAC
  seqviewprocs.createMenus = BioseqViewFormMenus;
#endif
  seqviewprocs.allowScrollText = FALSE;
  seqviewprocs.startInScrollText = FALSE;
  seqviewprocs.handleMessages = EntrezSeqViewFormMessage;
  seqviewprocs.makeControls = DoMakeViewerLinkControls;

  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &mapPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &sumPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gphPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &alnPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &seqPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gbgnPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gnbkPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &emblPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &gnptPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &fstaPageData);
  AddBioseqPageToList (&(seqviewprocs.pageSpecs), &qualPageData);
  if (showAsnPage) {
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &asnPageData);
    AddBioseqPageToList (&(seqviewprocs.pageSpecs), &dskPageData);
  }

  SetAppProperty ("SeqDisplayForm", &seqviewprocs);

  if (GetAppParam ("ENTREZ", "PREFERENCES", "TEXTEDITDISPLAYS", "", str, sizeof (str))) {
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
      txtviewprocs.minPixelWidth = val;
    }
  }

  if (GetEntrezAppParam ("PREFERENCES", "TEXTPIXELHEIGHT", "", str, sizeof (str))) {
    if (StrToInt (str, &val) && val > 0) {
      val = MIN (val, screenRect.bottom);
      txtviewprocs.minPixelHeight = val;
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

  MemSet ((Pointer) (&entrezglobals), 0, sizeof (EntrezGlobals));
  entrezglobals.retrieveDocsProc = DoRetrieveDocuments;
  entrezglobals.loadNamedUidProc = DoLoadNamedUidList;
  entrezglobals.launchViewerProc = DoLaunchRecordViewer;
#ifndef WIN_MAC
  entrezglobals.createTrmLstMenus = TermListFormMenus;
  entrezglobals.createDocSumMenus = DocSumFormMenus;
#endif
  SetAppProperty ("EntrezGlobals", &entrezglobals);

  entrezglobals.showAsn = showAsnPage;

  entrezglobals.persistDefault = TRUE;
  if (GetEntrezAppParam ("PREFERENCES", "PARENTSPERSIST", "", str, sizeof (str) - 1)) {
    if (StringICmp (str, "FALSE") == 0) {
      entrezglobals.persistDefault = FALSE;
    }
  }
  entrezglobals.alignDefault = TRUE;
  if (GetEntrezAppParam ("PREFERENCES", "ALIGNCHECKED", NULL, str, sizeof (str) - 1)) {
    if (StringICmp (str, "FALSE") == 0) {
      entrezglobals.alignDefault = FALSE;
    }
  }
  if (GetAppParam ("ENTREZ", "PREFERENCES", "POPDOWN", "", str, sizeof (str) - 1)) {
    if (StringICmp (str, "TRUE") == 0) {
      entrezglobals.popdownBehavior = TRUE;
    }
  }
  if (GetEntrezAppParam ("PREFERENCES", "LOOKUPDIRECT", "", str, sizeof (str) - 1)) {
    if (StringICmp (str, "TRUE") == 0) {
      entrezglobals.lookupDirect = TRUE;
    }
  }
  entrezglobals.sortFields = TRUE;
  if (GetAppParam ("ENTREZ", "PREFERENCES", "SORTFIELDS", "", str, sizeof (str) - 1)) {
    if (StringICmp (str, "FALSE") == 0) {
      entrezglobals.sortFields = FALSE;
    }
  }
  if (GetEntrezAppParam ("SETTINGS", "DATABASE", "MEDLINE", str, sizeof (str))) {
    entrezglobals.initDatabase = StringSaveNoNull (str);
  }
  if (GetEntrezAppParam ("SETTINGS", "FIELD", "All Fields", str, sizeof (str))) {
    entrezglobals.initField = StringSaveNoNull (str);
  }
  if (GetEntrezAppParam ("SETTINGS", "MODE", "Automatic", str, sizeof (str))) {
    entrezglobals.initMode = StringSaveNoNull (str);
  }

  if (GetEntrezAppParam ("SETTINGS", "BROWSER", NULL, str, sizeof (str))) {
    SetAppProperty ("MedviewBrowserPath", (void *) StringSave (str));
  }

  MemSet ((Pointer) (&stdedprocs), 0, sizeof (StdEditorProcs));
#ifdef WIN_MAC
  stdedprocs.activateForm = StdEditorFormActivated;
#endif
  stdedprocs.handleMessages = EntrezStdEditorFormMessage;
  SetAppProperty ("StdEditorForm", &stdedprocs);

  MemSet ((Pointer) (&txtviewprocs), 0, sizeof (TextViewProcs));
  txtviewprocs.useScrollText = FALSE;
  SetAppProperty ("TextDisplayForm", &txtviewprocs);
}

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

#ifdef WIN_MAC
static void SetupMacMenus (void)

{
  MenU  m;
  MenU  sub;

  m = AppleMenu (NULL);
  AddAboutAndHelpMenuItems (m);
  DeskAccGroup (m);

  m = PulldownMenu (NULL, "File");
  closeItem = FormCommandItem (m, "Close", NULL, VIB_MSG_CLOSE);
  SeparatorItem (m);
  duplicateItem = CommandItem (m, "Duplicate View", DuplicateViewProc);
  SeparatorItem (m);
  importItem = FormCommandItem (m, "Import...", NULL, VIB_MSG_IMPORT);
  exportItem = FormCommandItem (m, "Export...", NULL, VIB_MSG_EXPORT);
  SeparatorItem (m);
  /*
  saveItem = FormCommandItem (m, "Save", NULL, VIB_MSG_SAVE);
  saveAsItem = FormCommandItem (m, "Save As...", NULL, VIB_MSG_SAVE_AS);
  SeparatorItem (m);
  */
  printItem = FormCommandItem (m, "Print...", NULL, VIB_MSG_PRINT);
  SeparatorItem (m);
  FormCommandItem (m, "Quit/Q", NULL, VIB_MSG_QUIT);

  m = PulldownMenu (NULL, "Edit");
  /*
  undoItem = FormCommandItem (m, UNDO_MENU_ITEM, NULL, VIB_MSG_UNDO);
  Disable (undoItem);
  SeparatorItem (m);
  */
  cutItem = FormCommandItem (m, CUT_MENU_ITEM, NULL, VIB_MSG_CUT);
  copyItem = FormCommandItem (m, COPY_MENU_ITEM, NULL, VIB_MSG_COPY);
  pasteItem = FormCommandItem (m, PASTE_MENU_ITEM, NULL, VIB_MSG_PASTE);
  deleteItem = FormCommandItem (m, CLEAR_MENU_ITEM, NULL, VIB_MSG_DELETE);

  m = PulldownMenu (NULL, "Options");
  preferencesItem = CommandItem (m, "Preferences...", PreferencesProc);
  SeparatorItem (m);
  sub = SubMenu (m, "Query Style");
  queryChoice = CreateQueryTypeChoice (sub, NULL);
  clearUnusedItem = CreateClearUnusedItem (m, NULL);
  SeparatorItem (m);
  sub = SubMenu (m, "Neighbor Policy");
  neighborChoice = CreateNeighborDelayChoice (sub, NULL);
  SeparatorItem (m);
  LoadDocsumOptionsMenu (m);
  seqviewprocs.alignWithChecked = entrezglobals.alignWithChecked;

  m = PulldownMenu (NULL, "Misc");
  sub = SubMenu (m, "Font Selection");
  docsumfontItem = CommandItem (sub, "DocSum Font...", DocSumFontChangeProc);
  displayfontItem = CommandItem (sub, "Display Font...", DisplayFontChangeProc);
  SeparatorItem (m);
  legendItem = CreateLegendItem (m, NULL);
  SeparatorItem (m);
  VSMAddToMenu (m, VSM_DESKTOP);
  SeparatorItem (m);
  CommandItem (m, "Style Manager...", StyleManagerProc);
/*
#ifndef WIN16
  SeparatorItem (m);
  CommandItem (m, "Cn3D Window...", Cn3DWinShowProc);
#endif
*/
}
#endif

static void DoQuit (ButtoN b)

{
  QuitProgram ();
}

static void CleanupEntrez (void)

{
  ExitMuskStyles ();
  FreePrintOptions ();

  LocalSeqFetchDisable ();
  ID1BioseqFetchDisable ();
  EntrezBioseqFetchDisable ();
}

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

/*
static CharPtr configMessage =
"Entrez cannot find ncbi configuration file.\n\
Do you wish to run the configuration program?";
*/

static CharPtr configMessage =
"Entrez cannot connect to the data server.\n\
Do you wish to run the configuration program?";

Int2 Main (void)

{
  EntrezInfoPtr  eip;
  PaneL          p;
  RecT           r;
  Char           str [16];
  Int2           val;
  WindoW         w;
/*
#if defined(WIN16)
#else
  PRGD           prgdDict;
#endif
*/

  ErrSetFatalLevel (SEV_MAX);
  ErrClearOptFlags (EO_SHOW_USERSTR);
  ProcessUpdatesFirst (FALSE);

  /*
  if (! GetAppParam ("NCBI", "NCBI", NULL, NULL, str, sizeof (str) - 1)) {
    if (Message (MSG_YN, configMessage) == ANS_YES) {
      ShowNetConfigForm (ConfigFormActivated, ConfigFormMessage,
                         ConfigAccepted, ConfigCancelled);
      ProcessEvents ();
      return 0;
    } else {
      Message (MSG_FATAL, "Entrez cannot run without ncbi configuration file");
    }
    return 0;
  }

  if (! GetAppParam ("NCBI", "NCBI", NULL, NULL, str, sizeof (str) - 1)) {
    Message (MSG_FATAL, "Entrez cannot find ncbi configuration file");
    return 0;
  }
  */

  UseLocalAsnloadDataAndErrMsg ();
  SetupAppProperties ();
  SetupCommonFonts ();

  entrezRelease [0] = '\0';
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

  SetTitle (w, "Finding Entrez Data");
  if (! EntrezBioseqFetchEnable ("Entrez", TRUE)) {
    ArrowCursor ();
    /* Message (MSG_FATAL, "Unable to connect to Entrez databases"); */
    if (Message (MSG_YN, configMessage) == ANS_YES) {
      ShowNetConfigForm (ConfigFormActivated, ConfigFormMessage,
                         ConfigAccepted, ConfigCancelled, NULL, TRUE);
      ProcessEvents ();
      return 0;
    } else {
      Message (MSG_FATAL, "Entrez cannot run connection to the data server");
    }
    return 0;
  }
  ID1BioseqFetchEnable ("Entrez", TRUE);
  LocalSeqFetchInit (FALSE);

#ifdef WIN_MAC
  SetDeactivate (NULL, MacDeactProc);
  SetupMacMenus ();
#endif

  eip = EntrezGetInfo ();
  if (eip != NULL) {
    /*
    {
      AsnIoPtr aip;
      aip = AsnIoOpen ("eip", "w");
      if (aip != NULL) {
        EntrezInfoAsnWrite (eip, aip, NULL);
        AsnIoClose (aip);
      }
    }
    */
    sprintf (entrezRelease, "Entrez Data Release %d.%d", (int) eip->version, (int) eip->issue);
    Select (p);
    ObjectRect (p, &r);
    InsetRect (&r, 3, 3);
    InvalRect (&r);
    Update ();
  }

  SetTitle (w, "Loading parse tables");
  if (! AllObjLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "AllObjLoad failed");
    return 0;
  }
  if (! SubmitAsnLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "SubmitAsnLoad failed");
    return 0;
  }

  SetTitle (w, "Loading print templates");
  if (! PrintTemplateSetLoad ("objprt.prt")) {
    ArrowCursor ();
    Message (MSG_FATAL, "PrintTemplateSetLoad objprt.prt failed");
    return 0;
  }
  if (! PrintTemplateSetLoad ("asn2ff.prt")) {
    ArrowCursor ();
    Message (MSG_FATAL, "PrintTemplateSetLoad asn2ff.prt failed");
    return 0;
  }
  /*
  if (! PrintTemplateSetLoad ("makerpt.prt")) {
    ArrowCursor ();
    Message (MSG_FATAL, "PrintTemplateSetLoad makerpt.prt failed");
    return 0;
  }
  */

  SetTitle (w, "Loading sequence alphabet converter");
  if (! SeqCodeSetLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "SeqCodeSetLoad failed");
    return 0;
  }

  SetTitle (w, "Loading feature definitions");
  if (! FeatDefSetLoad ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "FeatDefSetLoad failed");
    return 0;
  }

  if (! SetupPrintOptions ()) {
    ArrowCursor ();
    Message (MSG_FATAL, "StdPrintOptionsNew failed");
    return 0;
  }

/*
#ifndef WIN16
  SetTitle (w, "Loading structure dictionary");
  if (OpenMMDBAPI ((POWER_VIEW ^ FETCH_ENTREZ), NULL)) {
    prgdDict = GetPRGDDictionary ();
    Cn3DWin_Entrez(NULL, TRUE);
  }
#endif
*/

  SetTitle (w, "Creating menus");

  REGISTER_MEDLINE_VIEW;
  REGISTER_NEW_SEQENTRY_VIEW;

  VSMFileInit ();
  VSeqMgrInit (FALSE);

  SetTitle (w, "Creating window");
  InitMuskStyles ();

  if (GetEntrezAppParam ("PREFERENCES", "DEFAULTSTYLE", "", str, sizeof (str))) {
    if (StrToInt (str, &val) && val >= 0) {
      SetMuskCurrentSt (GetMuskStyleName (val));
    }
  }

  /*
  SetTitle (w, "Creating help window");
  if (helpForm == NULL) {
    helpForm = CreateHelpForm (-95, -5, "Entrez Help", "entrez.hlp",
                               HideHelpForm, HelpActivateProc);
  }
  */
  SetTitle (w, "Creating initial window");

  termListForm = CreateTermListForm (-50, -33, "Query",
                                    TermSelectionActivateProc,
                                    TermSelectionFormMessage);

  Remove (w);
  ArrowCursor ();

  if (termListForm != NULL) {
    Show (termListForm);
    Select (termListForm);
    Update ();

    docSumForm = CreateDocSumForm (-10, -90, "Document",
                                   DocumentSummaryActivateProc,
                                   DocumentSummaryFormMessage);
    UseDelayedNeighbor (docSumForm, TRUE);
    if (docSumForm != NULL) {
      ProcessEvents ();
    } else {
      Message (MSG_FATAL, "Unable to create document window");
    }
  } else {
    Message (MSG_FATAL, "Unable to create term list window");
  }

  WatchCursor ();
  val = GetMuskCurrentSt ();
  if (val < 0) {
    val = 0;
  }
  if (val >= GetMuskTotalSt ()) {
    val = 0;
  }
  if (val > 0) {
    sprintf (str, "%d", (int) val);
    SetEntrezAppParam ("PREFERENCES", "DEFAULTSTYLE", str);
  } else {
    SetEntrezAppParam ("PREFERENCES", "DEFAULTSTYLE", "0");
  }

  CleanupEntrez ();
  ArrowCursor ();

  return 0;
}
