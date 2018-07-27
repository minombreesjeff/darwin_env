/*   medview.c
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
* File Name:  medview.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   4/30/95
*
* $Revision: 6.13 $
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

#include <medview.h>
#include <document.h>
#include <tomedlin.h>
#include <objmgr.h>
#include <accentr.h>
#include <prtutil.h>
#ifdef WIN_MOTIF
#include <netscape.h>
#endif

#define NUM_PAGES  6

typedef struct medlineviewform {
  FORM_MESSAGE_BLOCK

  Int2             currentPage;

  DoC              doc;
  TexT             text;
  GrouP            controls;
  Boolean          useScrollText;

  MedlineEntryPtr  mep;

  Boolean          cleanupObjectPtr;
  WndActnProc      activateForm;

  FonT             jourfnt;
  FonT             volfnt;
  FonT             pagesfnt;
  FonT             titlefnt;
  FonT             authorsfnt;
  FonT             affilfnt;
  FonT             abstractfnt;
  FonT             meshfnt;
  FonT             displayFont;
} MedlineViewForm, PNTR MedlineViewFormPtr;

static ParData medParFmt = {FALSE, FALSE, FALSE, FALSE, TRUE, 0, 0};
static ColData medColFmt = {0, 0, 80, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE};

#define BUFSIZE 8192

static CharPtr  buffer;
static CharPtr  pos;

static void ClearString (void)

{
  pos = buffer;
  *pos = '\0';
}

static void AddString (CharPtr string)

{
  pos = StringMove (pos, string);
  *pos = '\0';
}

static ColData  colFmt [3] = {
  {0, 0, 0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
  {0, 0, 0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},
  {0, 0, 0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE}
};

static ColData  mshFmt [1] = {
  {0, 0, 0, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE}
};

static Boolean DisplayArticle (DoC d, MedlineEntryPtr mep, Boolean showMesh)

{
  size_t              len;
  MedlineViewFormPtr  mfp;
  MedlinePtr          mPtr;
  ParData             para;
  CharPtr             ptr;
  RecT                r;
  CharPtr             tmp;

  mfp = (MedlineViewFormPtr) GetObjectExtra (d);
  if (mfp != NULL) {
    mPtr = ParseMedline (mep);
    if (mPtr != NULL) {
      buffer = (CharPtr) MemNew (BUFSIZE);
      if (buffer != NULL) {
        para.openSpace = FALSE;
        para.keepWithNext = FALSE;
        para.keepTogether = FALSE;
        para.newPage = FALSE;
        para.tabStops = FALSE;
        para.minLines = 0;
        para.minHeight = 0;
        colFmt [0].font = mfp->jourfnt;
        colFmt [1].font = mfp->volfnt;
        colFmt [2].font = mfp->pagesfnt;
        ObjectRect (d, &r);
        InsetRect (&r, 4, 4);
        mshFmt [0].pixWidth = r.right - r.left;
        mshFmt [0].charWidth = 80;
        ClearString ();
        AddString (mPtr->journal);
        AddString ("\t");
        AddString (mPtr->volume);
        AddString (":\t");
        AddString (mPtr->pages);
        AddString ("  (");
        AddString (mPtr->year);
        AddString (")");
        AddString (" [");
        AddString (mPtr->uid);
        AddString ("]");
        AddString ("\n");
        AppendText (d, buffer, &para, colFmt, NULL);
        ClearString ();
        if (mPtr->title != NULL) {
          AddString (mPtr->title);
          AddString ("\n");
          AppendText (d, buffer, NULL, NULL, mfp->titlefnt);
          ClearString ();
        }
        if (mPtr->transl != NULL) {
          AddString ("[");
          AddString (mPtr->transl);
          AddString ("]\n");
          AppendText (d, buffer, NULL, NULL, mfp->titlefnt);
          ClearString ();
        }
        if (mPtr->title == NULL && mPtr->transl == NULL) {
        }
        AddString (mPtr->authors);
        AddString ("\n");
        AppendText (d, buffer, NULL, NULL, mfp->authorsfnt);
        ClearString ();
        if (mPtr->affil != NULL) {
          AddString (mPtr->affil);
          AddString ("\n");
          AppendText (d, buffer, NULL, NULL, mfp->affilfnt);
          ClearString ();
        }
        if (mPtr->abstract != NULL) {
          len = StringLen (mPtr->abstract);
          tmp = (CharPtr) MemNew (sizeof (Char) * (len + 10));
          ptr = StringMove (tmp, mPtr->abstract);
          ptr = StringMove (ptr, "\n");
          AppendText (d, tmp, NULL, NULL, mfp->abstractfnt);
          MemFree (tmp);
        }
        if (showMesh) {
          if (mPtr->mesh != NULL) {
            AppendText (d, "MeSH Terms:\n", NULL, NULL, mfp->meshfnt);
            AppendText (d, mPtr->mesh, &para, mshFmt, mfp->meshfnt);
          }
          if (mPtr->gene != NULL) {
            AppendText (d, "Gene Symbols:\n", NULL, NULL, mfp->meshfnt);
            AppendText (d, mPtr->gene, &para, mshFmt, mfp->meshfnt);
          }
          if (mPtr->substance != NULL) {
            AppendText (d, "Substances:\n", NULL, NULL, mfp->meshfnt);
            AppendText (d, mPtr->substance, &para, mshFmt, mfp->meshfnt);
          }
        }
        buffer = (CharPtr) MemFree (buffer);
      }
      mPtr = FreeMedline (mPtr);
    }
  }
  return TRUE;
}

static void MedlineEntryPtrToMedlineForm (ForM f, Pointer data)

{
  AsnIoPtr            aipout;
  FonT                fnt;
  FILE                *fp;
  MedlineEntryPtr     mep;
  MedlineViewFormPtr  mfp;
  Char                path [PATH_MAX];
  RecT                r;

  mfp = (MedlineViewFormPtr) GetObjectExtra (f);
  mep = (MedlineEntryPtr) data;
  if (mfp != NULL && mep != NULL) {
    if (mfp->useScrollText) {
      ObjectRect (mfp->text, &r);
    } else {
      ObjectRect (mfp->doc, &r);
    }
    InsetRect (&r, 4, 4);
    medColFmt.pixWidth = screenRect.right - screenRect.left;
    medColFmt.pixInset = 8;
    WatchCursor ();
    if (mfp->useScrollText) {
      Reset (mfp->text);
    } else {
      Reset (mfp->doc);
    }
    Update ();
    SetDocAutoAdjust (mfp->doc, FALSE);
    switch (mfp->currentPage) {
      case ABSTRACT_PAGE :
        if (mfp->useScrollText) {
          TmpNam (path);
          fp = FileOpen (path, "w");
          if (fp != NULL) {
            if (MedlineEntryToAbsFile (mep, fp)) {
              FileClose (fp);
              if (! FileToScrollText (mfp->text, path)) {
                SetTitle (mfp->text, "(Text is too large to be displayed in this control.)");
              }
            } else {
              FileClose (fp);
            }
          }
          FileRemove (path);
        } else {
          DisplayArticle (mfp->doc, mep, FALSE);
        }
        break;
      case CITATION_PAGE :
        if (mfp->useScrollText) {
          TmpNam (path);
          fp = FileOpen (path, "w");
          if (fp != NULL) {
            if (MedlineEntryToDocFile (mep, fp)) {
              FileClose (fp);
              if (! FileToScrollText (mfp->text, path)) {
                SetTitle (mfp->text, "(Text is too large to be displayed in this control.)");
              }
            } else {
              FileClose (fp);
            }
          }
          FileRemove (path);
        } else {
          DisplayArticle (mfp->doc, mep, TRUE);
        }
        break;
      case MEDLINE_PAGE :
        TmpNam (path);
        fp = FileOpen (path, "w");
        if (fp != NULL) {
          fnt = programFont;
          if (mfp->displayFont != NULL) {
            fnt = mfp->displayFont;
          }
          if (MedlineEntryToDataFile (mep, fp)) {
            FileClose (fp);
            if (mfp->useScrollText) {
              if (! FileToScrollText (mfp->text, path)) {
                SetTitle (mfp->text, "(Text is too large to be displayed in this control.)");
              }
            } else {
              DisplayFancy (mfp->doc, path, &medParFmt, &medColFmt, fnt, 4);
            }
          } else {
            FileClose (fp);
          }
        }
        FileRemove (path);
        break;
      case MEDASN1_PAGE :
        TmpNam (path);
        fp = FileOpen (path, "w");
        if (fp != NULL) {
          fnt = programFont;
          if (mfp->displayFont != NULL) {
            fnt = mfp->displayFont;
          }
          aipout = AsnIoNew (ASNIO_TEXT_OUT, fp, NULL, NULL, NULL);
          if (MedlineEntryAsnWrite (mep, aipout, NULL)) {
            AsnIoClose (aipout);
            if (mfp->useScrollText) {
              if (! FileToScrollText (mfp->text, path)) {
                SetTitle (mfp->text, "(Text is too large to be displayed in this control.)");
              }
            } else {
              DisplayFancy (mfp->doc, path, &medParFmt, &medColFmt, fnt, 4);
            }
          } else {
            AsnIoClose (aipout);
          }
        }
        FileRemove (path);
        break;
      case MEDXML_PAGE :
        TmpNam (path);
        aipout = AsnIoOpen (path, "wx");
        if (aipout != NULL) {
          fnt = programFont;
          if (mfp->displayFont != NULL) {
            fnt = mfp->displayFont;
          }
          if (MedlineEntryAsnWrite (mep, aipout, NULL)) {
            AsnIoClose (aipout);
            if (mfp->useScrollText) {
              if (! FileToScrollText (mfp->text, path)) {
                SetTitle (mfp->text, "(Text is too large to be displayed in this control.)");
              }
            } else {
              DisplayFancy (mfp->doc, path, &medParFmt, &medColFmt, fnt, 4);
            }
          } else {
            AsnIoClose (aipout);
          }
        }
        FileRemove (path);
        break;
      default :
        break;
    }
    if (! mfp->useScrollText) {
      SetDocAutoAdjust (mfp->doc, TRUE);
      AdjustDocScroll (mfp->doc);
      UpdateDocument (mfp->doc, 0, 0);
    }
    ArrowCursor ();
    Update ();
  }
}

static void ResizeMedlineForm (WindoW w)

{
  RecT                c;
  Int2                diff = 0;
  Int2                gap = 0;
  Int2                height;
  MedlineViewFormPtr  mfp;
  RecT                r;
  RecT                s;
  RecT                t;
  Int2                width;

  mfp = (MedlineViewFormPtr) GetObjectExtra (w);
  if (mfp == NULL) return;
  WatchCursor ();
  ObjectRect (w, &r);
  width = r.right - r.left;
  height = r.bottom - r.top;
  SafeHide (mfp->controls);
  if (mfp->controls != NULL) {
    GetPosition (mfp->controls, &c);
  }
  if (mfp->doc != NULL) {
    GetPosition (mfp->doc, &s);
    if (mfp->controls != NULL) {
      LoadRect (&t, c.left, c.top, c.right, c.bottom);
      diff = t.bottom - t.top;
      gap = t.top - s.bottom;
      t.bottom = height - s.left;
      t.top = t.bottom - diff;
      s.right = width - s.left;
      /*
      s.bottom = height - s.left;
      */
      s.bottom = t.top - gap;
      SetPosition (mfp->controls, &t);
      AdjustPrnt (mfp->controls, &t, FALSE);
    } else {
      s.right = width - s.left;
      s.bottom = height - s.left;
    }
    SetPosition (mfp->doc, &s);
    AdjustPrnt (mfp->doc, &s, FALSE);
  }
  if (mfp->text != NULL) {
    GetPosition (mfp->text, &s);
    if (mfp->controls != NULL) {
      LoadRect (&t, c.left, c.top, c.right, c.bottom);
      diff = t.bottom - t.top;
      gap = t.top - s.bottom;
      t.bottom = height - s.left;
      t.top = t.bottom - diff;
      s.right = width - s.left;
      /*
      s.bottom = height - s.left;
      */
      s.bottom = t.top - gap;
      SetPosition (mfp->controls, &t);
      AdjustPrnt (mfp->controls, &t, FALSE);
    } else {
      s.right = width - s.left;
      s.bottom = height - s.left;
    }
    SetPosition (mfp->text, &s);
    AdjustPrnt (mfp->text, &s, FALSE);
  }
  if (mfp->doc != NULL) {
    ObjectRect (mfp->doc, &s);
    InsetRect (&s, 4, 4);
    medColFmt.pixWidth = screenRect.right - screenRect.left;
    medColFmt.pixInset = 8;
    if (Visible (mfp->doc) && AllParentsVisible (mfp->doc)) {
      UpdateDocument (mfp->doc, 0, 0);
    }
  }
  SafeShow (mfp->controls);
  ArrowCursor ();
  Update ();
}

extern void LIBCALL CopyMedlineViewFormToClipboard (Pointer formDataPtr)

{
  FILE                *fp;
  MedlineViewFormPtr  mfp;
  Char                path [PATH_MAX];

  mfp = (MedlineViewFormPtr) formDataPtr;
  if (mfp != NULL && mfp->mep != NULL) {
    if (mfp->useScrollText) {
      if (mfp->text != NULL) {
        CopyText (mfp->text);
      }
    } else {
      TmpNam (path);
      fp = FileOpen (path, "w");
      if (fp != NULL) {
        SaveDocument (mfp->doc, fp);
        FileClose (fp);
        FileToClipboard (path);
      }
      FileRemove (path);
    }
  }
}

extern void LIBCALL ExportMedlineViewFormToFile (Pointer formDataPtr, CharPtr filename)

{
  Char                ch;
  Char                dfault [32];
  FILE                *f;
  Int2                j;
  Int2                k;
  MedlineViewFormPtr  mfp;
  Char                path [PATH_MAX];

  mfp = (MedlineViewFormPtr) formDataPtr;
  if (mfp != NULL && mfp->mep != NULL) {
    if (mfp->doc != NULL || mfp->text != NULL) {
      dfault [0] = '\0';
      GetTitle (mfp->form, dfault, sizeof (dfault));
      j = 0;
      k = 0;
      ch = dfault [j];
      while (j < sizeof (dfault) && ch != '\0') {
        if (ch <= ' ') {
          j++;
        } else {
          dfault [k] = dfault [j];
          k++;
          j++;
        }
        ch = dfault [j];
      }
      dfault [k] = '\0';
#ifdef WIN_MSWIN
      j = 0;
      ch = dfault [j];
      while (j < sizeof (dfault) && ch != '\0') {
        if (ch == '_' || IS_ALPHANUM (ch)) {
          j++;
          ch = dfault [j];
        } else {
          ch = '\0';
        }
      }
      dfault [j] = '\0';
#endif
      path [0] = '\0';
      StringNCpy_0 (path, filename, sizeof (path));
      if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), dfault)) {
        WatchCursor ();
#ifdef WIN_MAC
        f = FileOpen (path, "r");
        if (f != NULL) {
          FileClose (f);
        } else {
          FileCreate (path, "TEXT", "ttxt");
        }
#endif
        if (filename == NULL || filename [0] == '\0') {
          f = FileOpen (path, "w");
        } else {
          f = FileOpen (path, "a");
        }
        if (f != NULL) {
          if (mfp->useScrollText) {
            ScrollTextToFile (mfp->text, path);
          } else {
            SaveDocument (mfp->doc, f);
          }
          FileClose (f);
        }
        ArrowCursor ();
      }
    }
  }
}

#ifdef WIN_MOTIF
extern CharPtr Nlm_XrmGetResource (const Char PNTR _resource);
#endif

extern void LIBCALL PrintMedlineViewForm (Pointer formDataPtr)

{
  MedlineViewFormPtr  mfp;
#ifdef WIN_MOTIF
  Char                cmmd [256];
  Int2                len;
  CharPtr             printCmd;
  Char                str [PATH_MAX];
#endif

  mfp = (MedlineViewFormPtr) formDataPtr;
  if (mfp != NULL && mfp->mep != NULL) {
    if (mfp->doc != NULL) {
      PrintDocument (mfp->doc);
    } else if (mfp->text != NULL) {
#ifdef WIN_MOTIF
      TmpNam (str);
      ScrollTextToFile (mfp->text, str);
      printCmd = Nlm_XrmGetResource ("printCommand");
      if (printCmd != NULL) {
        StringNCpy_0 (cmmd, printCmd, sizeof (cmmd) - 1);
      } else {
        StringCpy (cmmd, "lp -c");
      }
      MemFree (printCmd);
      len = (Int2) StringLen (cmmd);
      while (len > 0 && cmmd [len] == ' ') {
        cmmd [len] = '\0';
        len--;
      }
      StringCat (cmmd, " ");
      StringCat (cmmd, str);
      StringCat (cmmd, "; rm ");
      StringCat (cmmd, str);
      system (cmmd);
      /*
      FileRemove (str);
      */
#endif
    }
  }
}

static void SetMedlineImportExportItems (MedlineViewFormPtr mfp)

{
  IteM  exportItm;

  if (mfp == NULL) return;
  exportItm = FindFormMenuItem ((BaseFormPtr) mfp, VIB_MSG_EXPORT);
  if (exportItm == NULL) return;
  switch (mfp->currentPage) {
    case ABSTRACT_PAGE :
      SafeSetTitle (exportItm, "Export Abstract...");
      break;
    case CITATION_PAGE :
      SafeSetTitle (exportItm, "Export Citation...");
      break;
    case MEDLINE_PAGE :
      SafeSetTitle (exportItm, "Export MEDLINE...");
      break;
    case MEDASN1_PAGE :
      SafeSetTitle (exportItm, "Export ASN.1...");
      break;
    case MEDXML_PAGE :
      SafeSetTitle (exportItm, "Export XML...");
      break;
    default :
      SafeSetTitle (exportItm, "Export...");
      break;
  }
}

static void ChangeMedlineViewTabs (VoidPtr data, Int2 newval, Int2 oldval)

{
  MedlineEntryPtr     mep;
  MedlineViewFormPtr  mfp;

  mfp = (MedlineViewFormPtr) data;
  if (mfp != NULL && mfp->mep != NULL) {
    mep = mfp->mep;
    mfp->currentPage = newval;
    SafeHide (mfp->doc);
    SafeHide (mfp->text);
    WatchCursor ();
    Update ();
    Reset (mfp->doc);
    Reset (mfp->text);
    PointerToForm (mfp->form, (Pointer) mep);
    Update ();
    ArrowCursor ();
    SafeShow (mfp->doc);
    SafeShow (mfp->text);
    SetMedlineImportExportItems (mfp);
    Update ();
    if (mfp->activateForm != NULL) {
      mfp->activateForm ((WindoW) mfp->form);
    }
  }
}

static void ChangeMedlineViewGroup (GrouP g)

{
  MedlineViewFormPtr  mfp;
  Int2                val;

  mfp = (MedlineViewFormPtr) GetObjectExtra (g);
  if (mfp != NULL) {
    val = GetValue (g);
    ChangeMedlineViewTabs ((VoidPtr) mfp, val - 1, mfp->currentPage);
  }
}

static void ChangeMedlineViewPopup (PopuP p)

{
  MedlineViewFormPtr  mfp;
  Int2                val;

  mfp = (MedlineViewFormPtr) GetObjectExtra (p);
  if (mfp != NULL) {
    val = GetValue (p);
    ChangeMedlineViewTabs ((VoidPtr) mfp, val - 1, mfp->currentPage);
  }
}

static CharPtr  medlineViewFormTabs [] = {
  "Abstract", "Citation", "MEDLINE", NULL, NULL, NULL
};

static void CleanupMedlineForm (GraphiC g, VoidPtr data)

{
  MedlineViewFormPtr  mfp;

  mfp = (MedlineViewFormPtr) data;
  if (mfp != NULL && mfp->input_entityID > 0) {
    ObjMgrFreeUserData (mfp->input_entityID, mfp->procid, mfp->proctype, mfp->userkey);
  }
  if (mfp != NULL) {
    WatchCursor ();
    if (mfp->cleanupObjectPtr && mfp->objectDataPtr != NULL) {
      MedlineEntryFree ((MedlineEntryPtr) mfp->objectDataPtr);
    }
    ArrowCursor ();
  }
  StdCleanupFormProc (g, data);
}

static Boolean ExportMedlineViewForm (ForM f, CharPtr filename)

{
  ExportMedlineViewFormToFile (GetObjectExtra (f), filename);
  return TRUE;
}

static void MedlineViewFormMessage (ForM f, Int2 mssg)

{
  MedlineViewFormPtr   mfp;
  MedlineViewProcsPtr  mvpp;

  mfp = (MedlineViewFormPtr) GetObjectExtra (f);
  if (mfp != NULL) {
    switch (mssg) {
      case VIB_MSG_REDRAW :
        mvpp = (MedlineViewProcsPtr) GetAppProperty ("MedlineDisplayForm");
        if (mvpp != NULL) {
          mfp->jourfnt = mvpp->jourfnt;
          mfp->volfnt = mvpp->volfnt;
          mfp->pagesfnt = mvpp->pagesfnt;
          mfp->titlefnt = mvpp->titlefnt;
          mfp->authorsfnt = mvpp->authorsfnt;
          mfp->affilfnt = mvpp->affilfnt;
          mfp->abstractfnt = mvpp->abstractfnt;
          mfp->meshfnt = mvpp->meshfnt;
          mfp->displayFont = mvpp->displayFont;
          SafeHide (mfp->doc);
          SafeHide (mfp->text);
          Update ();
          Reset (mfp->doc);
          Reset (mfp->text);
          PointerToForm (mfp->form, (Pointer) mfp->mep);
          Update ();
          ArrowCursor ();
          SafeShow (mfp->doc);
          SafeShow (mfp->text);
        }
        break;
      case VIB_MSG_COPY :
        CopyMedlineViewFormToClipboard (mfp);
        break;
      case VIB_MSG_EXPORT :
        ExportMedlineViewForm (f, NULL);
        break;
      case VIB_MSG_PRINT :
        PrintMedlineViewForm (mfp);
        break;
      default :
        if (mfp->appmessage != NULL) {
          mfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void MedlineViewFormActivate (WindoW w)

{
  MedlineViewFormPtr  mfp;

  mfp = (MedlineViewFormPtr) GetObjectExtra (w);
  if (mfp != NULL) {
    if (mfp->activate != NULL) {
      mfp->activate (w);
    }
    SetMedlineImportExportItems (mfp);
  }
}

/*
static void LaunchPubMedArticle (ButtoN b)

{
#ifndef WIN_MAC
  CharPtr             argv [8];
#endif
  CharPtr             browser;
  MedlineViewFormPtr  mfp;
  Char                str [256];
#ifdef WIN_MOTIF
  NS_Window          window = NULL;
#endif

  mfp = (MedlineViewFormPtr) GetObjectExtra (b);
  if (mfp == NULL) return;
  if (mfp->docuid < 1) return;
  browser = GetAppProperty ("MedviewBrowserPath");
  sprintf (str,
           "http://www.ncbi.nlm.nih.gov/htbin-post/Entrez/query?db=m&form=6&uid=%ld&Dopt=r",
           (long) mfp->docuid);
#ifdef WIN_MAC
  if (browser == NULL || StringHasNoText (browser)) {
    Nlm_SendURLAppleEvent (str, "MOSS", NULL);
  } else {
    Nlm_SendURLAppleEvent (str, NULL, browser);
  }
#endif
#ifdef WIN_MSWIN
  argv [0] = str;
  argv [1] = NULL;
  if (browser != NULL && (! StringHasNoText (browser))) {
    if (! Execv (browser, argv)) {
      Message (MSG_POST, "Unable to launch %s", browser);
    }
  } else {
    if (! Nlm_MSWin_OpenDocument (str)) {
      Message (MSG_POST, "Unable to launch browser");
    }
  }
#endif
#ifdef WIN_MOTIF
  argv [0] = str;
  argv [1] = NULL;
  if (browser != NULL && (! StringHasNoText (browser))) {
    if (! Execv (browser, argv)) {
      Message (MSG_POST, "Unable to launch %s", browser);
    }
  } else {
    if (! NS_OpenURL (&window, str, NULL, TRUE)) {
      Message (MSG_POST, "Unable to launch netscape");
    }
    NS_WindowFree (window);
  }
#endif
}
*/

static void LaunchPubMedArticle (ButtoN b)

{
  MedlineViewFormPtr  mfp;

  mfp = (MedlineViewFormPtr) GetObjectExtra (b);
  if (mfp == NULL) return;
  if (mfp->docuid < 1) return;
  LaunchEntrezURL ("PubMed", mfp->docuid, "Abstract");
}

static FonT SetFontIfNull (FonT f, CharPtr dfault)

{
  if (f != NULL) return f;
  f = ParseFont (dfault);
  return f;
}

static void SetupDefaultMvppFonts (MedlineViewProcsPtr mvpp)

{
  if (mvpp == NULL) return;

#ifdef WIN_MAC
  mvpp->jourfnt = SetFontIfNull (mvpp->jourfnt, "Geneva,10,i");
  mvpp->volfnt = SetFontIfNull (mvpp->volfnt, "Geneva,10,b");
  mvpp->pagesfnt = SetFontIfNull (mvpp->pagesfnt, "Geneva,10");
  mvpp->titlefnt = SetFontIfNull (mvpp->titlefnt, "Times,14,b");
  mvpp->authorsfnt = SetFontIfNull (mvpp->authorsfnt, "Times,14");
  mvpp->affilfnt = SetFontIfNull (mvpp->affilfnt, "Times,12");
  mvpp->abstractfnt = SetFontIfNull (mvpp->abstractfnt, "Geneva,10");
  mvpp->meshfnt = SetFontIfNull (mvpp->meshfnt, "Monaco,9");
  mvpp->displayFont = SetFontIfNull (mvpp->displayFont, "Monaco,9");
#endif
#ifdef WIN_MSWIN
  mvpp->jourfnt = SetFontIfNull (mvpp->jourfnt, "Arial,11,i");
  mvpp->volfnt = SetFontIfNull (mvpp->volfnt, "Arial,11,b");
  mvpp->pagesfnt = SetFontIfNull (mvpp->pagesfnt, "Arial,11");
  mvpp->titlefnt = SetFontIfNull (mvpp->titlefnt, "Times New Roman,14,b");
  mvpp->authorsfnt = SetFontIfNull (mvpp->authorsfnt, "Times New Roman,14");
  mvpp->affilfnt = SetFontIfNull (mvpp->affilfnt, "Times New Roman,11");
  mvpp->abstractfnt = SetFontIfNull (mvpp->abstractfnt, "Times New Roman,11");
  mvpp->meshfnt = SetFontIfNull (mvpp->meshfnt, "Times New Roman,9");
  mvpp->displayFont = SetFontIfNull (mvpp->displayFont, "Courier New,10");
#endif
#ifdef WIN_MOTIF
  mvpp->jourfnt = SetFontIfNull (mvpp->jourfnt, "Helvetica,12,i");
  mvpp->volfnt = SetFontIfNull (mvpp->volfnt, "Helvetica,12,b");
  mvpp->pagesfnt = SetFontIfNull (mvpp->pagesfnt, "Helvetica,12");
  mvpp->titlefnt = SetFontIfNull (mvpp->titlefnt, "Times,18,b");
  mvpp->authorsfnt = SetFontIfNull (mvpp->authorsfnt, "Times,18");
  mvpp->affilfnt = SetFontIfNull (mvpp->affilfnt, "Times,14");
  mvpp->abstractfnt = SetFontIfNull (mvpp->abstractfnt, "Times,14");
  mvpp->meshfnt = SetFontIfNull (mvpp->meshfnt, "Times,12");
  mvpp->displayFont = SetFontIfNull (mvpp->displayFont, "Courier,10");
#endif
}

extern ForM LIBCALL CreateMedlineViewForm (Int2 left, Int2 top, CharPtr title,
                                           MedlineEntryPtr mep,
                                           MedlineViewProcsPtr mvpp)

{
  ButtoN               b;
  WndActnProc          close;
  FonT                 fnt;
  GrouP                g;
  GrouP                h;
  Int2                 j;
  GrouP                k;
  MedViewControlsProc  makeControls = NULL;
  MedlineViewFormPtr   mfp;
  Int2                 minwid;
  Int2                 pixheight;
  Int2                 pixwidth;
  PopuP                pops;
  PrompT               ppt;
  RecT                 r;
  GrouP                rads;
  GrouP                s;
  DialoG               tbs;
  WindoW               w;

  w = NULL;
  mfp = (MedlineViewFormPtr) MemNew (sizeof (MedlineViewForm));
  if (mfp != NULL && mep != NULL) {
    close = StdCloseWindowProc;
    if (mvpp == NULL) {
      mvpp = (MedlineViewProcsPtr) GetAppProperty ("MedlineDisplayForm");
    }
    if (mvpp != NULL && mvpp->closeForm != NULL) {
      close = mvpp->closeForm;
    }
    w = DocumentWindow (left, top, -10, -10, title,
                        close, ResizeMedlineForm);
    SetObjectExtra (w, mfp, CleanupMedlineForm);
    mfp->form = (ForM) w;
    mfp->actproc = NULL;
    mfp->toform = MedlineEntryPtrToMedlineForm;
    mfp->exportform = ExportMedlineViewForm;
    mfp->formmessage = MedlineViewFormMessage;

    mfp->mep = mep;
    mfp->useScrollText = FALSE;
    if (mvpp != NULL) {
      SetupDefaultMvppFonts (mvpp);
      mfp->cleanupObjectPtr = mvpp->cleanupObjectPtr;
      mfp->activateForm = mvpp->activateForm;
      mfp->jourfnt = mvpp->jourfnt;
      mfp->volfnt = mvpp->volfnt;
      mfp->pagesfnt = mvpp->pagesfnt;
      mfp->titlefnt = mvpp->titlefnt;
      mfp->authorsfnt = mvpp->authorsfnt;
      mfp->affilfnt = mvpp->affilfnt;
      mfp->abstractfnt = mvpp->abstractfnt;
      mfp->meshfnt = mvpp->meshfnt;
      mfp->displayFont = mvpp->displayFont;
      mfp->useScrollText = mvpp->useScrollText;
      mfp->appmessage = mvpp->handleMessages;
      makeControls = mvpp->makeControls;
    }

    if (mvpp != NULL && mvpp->createMenus != NULL) {
      mvpp->createMenus (w);
    }

    g = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (g, 3, 10);

    if (mvpp != NULL && mvpp->showAsnPage) {
      minwid = 35 * stdCharWidth + 17;
    } else {
      minwid = 27 * stdCharWidth + 17;
    }
    pixwidth = minwid;
    pixheight = 14 * stdLineHeight;
    mfp->currentPage = 0;

    if (mvpp != NULL) {
      mfp->currentPage = mvpp->initPage;
      if (mvpp->initMedLabel != NULL) {
        for (j = 0; medlineViewFormTabs [j] != NULL; j++) {
          if (StringICmp (medlineViewFormTabs [j], mvpp->initMedLabel) == 0) {
            mfp->currentPage = j;
          }
        }
      }
      if (mvpp->showAsnPage) {
        medlineViewFormTabs [3] = "ASN.1";
        medlineViewFormTabs [4] = "XML";
      }
      switch (mvpp->useFolderTabs) {
        case CHANGE_VIEW_NOTABS :
          break;
        case CHANGE_VIEW_FOLDERTABS :
          tbs = CreateFolderTabs (g, medlineViewFormTabs, mfp->currentPage,
                                  0, 0, SYSTEM_FOLDER_TAB,
                                  ChangeMedlineViewTabs, (Pointer) mfp);
          ObjectRect (tbs, &r);
          pixwidth = r.right - 2 * r.left - Nlm_vScrollBarWidth;
          if (pixwidth < minwid) {
            pixwidth = minwid;
          }
          break;
        case CHANGE_VIEW_TEXTTABS :
          tbs = CreateTextTabs (g, medlineViewFormTabs, mfp->currentPage,
                                  0, 0, SYSTEM_TEXT_TAB,
                                  ChangeMedlineViewTabs, (Pointer) mfp);
          ObjectRect (tbs, &r);
          pixwidth = r.right - 2 * r.left - Nlm_vScrollBarWidth;
          if (pixwidth < minwid) {
            pixwidth = minwid;
          }
          break;
        case CHANGE_VIEW_RADIOBUTTONS :
          k = HiddenGroup (g, -3, 0, NULL);
          ppt = StaticPrompt (k, "Format:", 0, 0, programFont, 'l');
          rads = HiddenGroup (k, 8, 0, ChangeMedlineViewGroup);
          SetObjectExtra (rads, (Pointer) mfp, NULL);
          for (j = 0; medlineViewFormTabs [j] != NULL; j++) {
            RadioButton (rads, medlineViewFormTabs [j]);
          }
          SetValue (rads, mfp->currentPage + 1);
          ObjectRect (rads, &r);
          pixwidth = r.right - 2 * r.left - Nlm_vScrollBarWidth;
          if (pixwidth < minwid) {
            pixwidth = minwid;
          }
          b = PushButton (k, "PubMed", LaunchPubMedArticle);
          SetObjectExtra (b, (Pointer) mfp, NULL);
          AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt, (HANDLE) rads, (HANDLE) b, NULL);
          break;
        case CHANGE_VIEW_POPUP :
          k = HiddenGroup (g, -3, 0, NULL);
          ppt = StaticPrompt (k, "Display Format", 0, popupMenuHeight, programFont, 'l');
          pops = PopupList (k, TRUE, ChangeMedlineViewPopup);
          SetObjectExtra (pops, (Pointer) mfp, NULL);
          for (j = 0; medlineViewFormTabs [j] != NULL; j++) {
            PopupItem (pops, medlineViewFormTabs [j]);
          }
          SetValue (pops, mfp->currentPage + 1);
          ObjectRect (pops, &r);
          pixwidth = r.right - 2 * r.left - Nlm_vScrollBarWidth;
          if (pixwidth < minwid) {
            pixwidth = minwid;
          }
          b = PushButton (k, "PubMed", LaunchPubMedArticle);
          SetObjectExtra (b, (Pointer) mfp, NULL);
          AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt, (HANDLE) pops, (HANDLE) b, NULL);
          break;
        default :
          break;
      }
      medlineViewFormTabs [3] = NULL;
      medlineViewFormTabs [4] = NULL;
      pixwidth = MAX (pixwidth, mvpp->minPixelWidth);
      pixheight = MAX (pixheight, mvpp->minPixelHeight);
    }

    h = HiddenGroup (g, 0, 0, NULL);

    s = HiddenGroup (h, -1, 0, NULL);
    if (mfp->useScrollText) {
      fnt = programFont;
      if (mfp->displayFont != NULL) {
        fnt = mfp->displayFont;
      }
      mfp->text = ScrollText (s, pixwidth / stdCharWidth,
                                     pixheight / stdLineHeight, fnt, FALSE, NULL);
      SetObjectExtra (mfp->text, mfp, NULL);
    } else {
      mfp->doc = DocumentPanel (s, pixwidth, pixheight);
      SetObjectExtra (mfp->doc, mfp, NULL);
    }

    if (makeControls != NULL) {
      mfp->controls = makeControls (g, (BaseFormPtr) mfp, TYP_ML, mep->uid);
    }

    RealizeWindow (w);

    mfp->activate = NULL;
    if (mvpp != NULL) {
      mfp->activate = mvpp->activateForm;
    }
    SetActivate (w, MedlineViewFormActivate);
    Update ();
    MedlineViewFormActivate ((WindoW) mfp->form);

    SendMessageToForm (mfp->form, VIB_MSG_INIT);
    if (mep != NULL) {
      PointerToForm (mfp->form, (Pointer) mep);
    }
  }
  return (ForM) w;
}

CharPtr PersonIdPrint PROTO((PersonIdPtr pip, CharPtr buf)); /* in prtutil.h */

#define ART_JOURNAL     1
#define ART_BOOK        2

static void GetMedlineCaption (MedlineEntryPtr mep, CharPtr buf, size_t max)

{
  AuthListPtr  alp;
  AuthorPtr    ap;
  CitArtPtr    cap;
  CitBookPtr   cbp;
  CitJourPtr   cjp;
  DatePtr      dp;
  ImprintPtr   imp;
  ValNodePtr   names;
  PersonIdPtr  pid;
  Char         str [64];
  Char         tmp [16];

  if (mep == NULL || buf == NULL || max < 1) return;
  cap = mep->cit;
  if (cap == NULL) return;
  alp = cap->authors;
  if (alp == NULL) return;
  str [0] = '\0';
  tmp [0] = '\0';
  if (alp->choice == 1) {
    names = alp->names;
    if (names != NULL) {
      ap = names->data.ptrvalue;
      if (ap != NULL) {
        pid = ap->name;
        if (pid != NULL) {
          PersonIdPrint (pid, str);
        }
      }
    }
  } else if (alp->choice == 2 || alp->choice == 3) {
    names = alp->names;
    if (names != NULL) {
      StringNCpy_0 (str, names->data.ptrvalue, sizeof (str) - 2);
    }
  }
  imp = NULL;
  switch (cap->from) {
    case ART_JOURNAL :
      cjp = (CitJourPtr) cap->fromptr;
      if (cjp != NULL) {
        imp = cjp->imp;
      }
      break;
    case ART_BOOK :
      cbp = (CitBookPtr) cap->fromptr;
      if (cbp != NULL) {
        imp = cbp->imp;
      }
      break;
    default :
      break;
  }
  if (imp != NULL) {
    dp = imp->date;
    if (dp != NULL) {
      if (dp->data [0] == 1) {
        sprintf (tmp, "  %d", (dp->data [1] + 1900));
      } else if (dp->data [0] == 0 && dp->str != NULL) {
        sprintf (tmp, "  %s", dp->str);
      }
    }
  }
  StringCat (str, tmp);
  StringNCpy_0 (buf, str, max);
}

extern Int2 LIBCALLBACK MedlineViewGenFunc (Pointer data)

{
  MedlineEntryPtr     mep;
  MedlineViewFormPtr  mfp;
  OMProcControlPtr    ompcp;
  OMUserDataPtr       omudp;
  Char                str [64];
  WindoW              w;

  ompcp = (OMProcControlPtr) data;
  mep = NULL;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_MEDLINE_ENTRY :
      mep = (MedlineEntryPtr) ompcp->input_data;
      break;
   case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (mep == NULL) return OM_MSG_RET_ERROR;
  str [0] = '\0';
  GetMedlineCaption (mep, str, sizeof (str));
  w = (WindoW) CreateMedlineViewForm (-50, -33, str, mep, NULL);
  mfp = (MedlineViewFormPtr) GetObjectExtra (w);
  if (mfp != NULL) {
    mfp->input_entityID = ompcp->input_entityID;
    mfp->input_itemID = ompcp->input_itemID;
    mfp->input_itemtype = ompcp->input_itemtype;
    mfp->this_itemtype = OBJ_MEDLINE_ENTRY;
    mfp->this_subtype = 0;
    mfp->procid = ompcp->proc->procid;
    mfp->proctype = ompcp->proc->proctype;
    mfp->docuid = mep->uid;
    mfp->doctype = TYP_ML;
    mfp->userkey = OMGetNextUserKey ();
    omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid,
	                           OMPROC_VIEW, mfp->userkey);
    if (omudp != NULL) {
      omudp->userdata.ptrvalue = (Pointer) mfp;
      omudp->messagefunc = NULL;
    }
  }
  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}

