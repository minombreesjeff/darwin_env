/*   vsmutil.c
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
* File Name:  vsmutil.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   3/3/95
*
* $Revision: 6.33 $
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

#include <vibrant.h>
#include <document.h>
#include <viewer.h>
#include <objfdef.h>
#include <prtutil.h>
#include <gather.h>
#include <vsmutil.h>
#include <dlogutil.h>
#include <bspview.h>

typedef struct errfltrdata {
  ErrSev         sev;
  int            errcode;
  int            subcode;
  CharPtr        text1;
  CharPtr        text2;
  CharPtr        text3;
} ErrFltrData, PNTR ErrFltrPtr;

typedef struct validextra {
  FORM_MESSAGE_BLOCK
  ButtoN         remove;
  PopuP          verbose;
  PopuP          minlevel;
  PopuP          filter;
  DoC            doc;
  FonT           font;
  ButtoN         find;
  TexT           searchfor;
  PrompT         showncount;
  PrompT         summary;
  Int4           counts [6];
  Int4           totalcount;
  Int4           addedcount;
  Int4           remaining;
  Int2           clicked;
  Int2           selected;
  Boolean        dblClick;
  Boolean        shftKey;
  ErrNotifyProc  notify;
  ValNodePtr     messages;
  ValNodePtr     lastmessage;
  ValNodePtr     errorfilter;
  BaseFormPtr    bfp;
  FormActnFunc   revalProc;
  ButtoN         revalBtn;
  Boolean        okaytosetviewtarget;
} ValidExtra, PNTR ValidExtraPtr;

static WindoW  validWindow = NULL;

static ParData valParFmt = {TRUE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ParData justAccnParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};

static ColData valColFmt [] = {
  {0,  7, 15,  0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* severity */
  {0,  5, 15,  0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* error    */
  {0,  5, 45,  0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* subcode  */
  {0,  0,  0,  0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* hidden   */
  {0,  0, 60, 15, NULL, 'l', TRUE,  FALSE, FALSE, TRUE,  TRUE}   /* message  */
};

extern void FreeValidateWindow (void)

{
  if (validWindow != NULL) {
    Remove (validWindow);
    validWindow = NULL;
  }
}

static void InvalBorder (DoC d, Int2 item)

{
  Int2  bottom;
  RecT  r;
  Int2  top;

  ObjectRect (d, &r);
  InsetRect (&r, 4, 4);
  if (ItemIsVisible (d, item, &top, &bottom, NULL)) {
    r.top = top;
    r.bottom = bottom;
    r.right = r.left + 4;
    InsetRect (&r, -1, -1);
    InvalRect (&r);
  }
}

static void DrawValid (DoC d, RectPtr r, Int2 item, Int2 firstLine)

{
  RecT           rct;
  ValidExtraPtr  vep;

  vep = GetObjectExtra (d);
  if (vep != NULL) {
    if (item == vep->selected) {
      rct = *r;
      rct.right = rct.left + 4;
      PaintRect (&rct);
    }
  }
}

static void ClickValid (DoC d, PoinT pt)

{
  Int2           item;
  Int2           row;
  ValidExtraPtr  vep;

  vep = GetObjectExtra (d);
  if (vep != NULL) {
    MapDocPoint (d, pt, &item, &row, NULL, NULL);
    if (item > 0 && row > 0 && vep->clicked == item) {
      vep->dblClick = dblClick;
    } else {
      vep->dblClick = FALSE;
    }
    vep->clicked = 0;
    vep->shftKey = shftKey;
    if (item > 0 && row > 0) {
      vep->clicked = item;
    }
  }
}

static Boolean FindSfpItem (GatherContextPtr gcp)

{
  SeqFeatPtr  PNTR sfpp;

  sfpp = (SeqFeatPtr PNTR) gcp->userdata;
  if (sfpp != NULL && gcp->thistype == OBJ_SEQFEAT) {
    *sfpp = (SeqFeatPtr) gcp->thisitem;
  }
  return TRUE;
}

static SeqFeatPtr GetSeqFeatGivenIDs (Uint2 entityID, Uint2 itemID, Uint2 itemtype)

{
  SeqFeatPtr  sfp;

  sfp = NULL;
  if (entityID > 0 && itemID > 0 && itemtype == OBJ_SEQFEAT) {
    GatherItem (entityID, itemID, itemtype, (Pointer) (&sfp), FindSfpItem);
  }
  return sfp;
}

static void ValDoNotify (ValidExtraPtr vep, Int2 item, Boolean select, Boolean target)

{
  BioseqPtr     bsp = NULL;
  unsigned int  entityID;
  int           errcode;
  unsigned int  itemID;
  unsigned int  itemtype;
  CharPtr       ptr;
  Char          seqid [128];
  int           sev;
  SeqFeatPtr    sfp;
  SeqIdPtr      sip;
  int           subcode;
  CharPtr       str;

  if (vep != NULL && vep->doc != NULL && vep->notify != NULL) {
    str = GetDocText (vep->doc, item, 1, 4);
    if (str != NULL &&
        sscanf (str, "%d %d %d %u %u %u", &sev, &errcode,
                &subcode, &entityID, &itemID, &itemtype) == 6) {
      if (target) {
        if (itemtype == OBJ_BIOSEQ || itemtype == OBJ_SEQFEAT) {
          if (itemtype == OBJ_BIOSEQ) {
            bsp = GetBioseqGivenIDs (entityID, itemID, itemtype);
          } else {
            sfp = GetSeqFeatGivenIDs (entityID, itemID, itemtype);
            if (sfp != NULL)
            {
              bsp = GetBioseqGivenSeqLoc (sfp->location, entityID);
            }
          }
          if (bsp != NULL) {
            sip = SeqIdFindWorst (bsp->id);
            SeqIdWrite (sip, seqid, PRINTID_REPORT, sizeof (seqid));
            ptr = StringChr (seqid, '|');
            if (ptr == NULL) {
              ptr = seqid;
            } else {
              ptr++;
            }
            SetBioseqViewTarget (vep->bfp, ptr);
          }
        }
      }
      if (select) {
        ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
      } else {
        ObjMgrDeSelect (entityID, itemID, itemtype, 0, NULL);
      }
      (vep->notify) ((ErrSev) sev, errcode, subcode,
                     (Uint2) entityID, (Uint2) itemID, (Uint2) itemtype,
                     select, vep->dblClick);
    }
    MemFree (str);
  }
}

static void ReleaseValid (DoC d, PoinT pt)

{
  Int2           item;
  Int2           old;
  Int2           row;
  ValidExtraPtr  vep;

  vep = GetObjectExtra (d);
  if (vep != NULL) {
    ResetClip ();
    MapDocPoint (d, pt, &item, &row, NULL, NULL);
    if (item > 0 && row > 0) {
      if (item == vep->clicked) {
        old = vep->selected;
        vep->selected = item;
        if (old != item) {
          if (old == 0) {
            InvalBorder (d, item);
          } else {
            InvalBorder (d, MIN (item, old));
            InvalBorder (d, MAX (item, old));
            ValDoNotify (vep, old, FALSE, FALSE);
          }
          if (! Enabled (vep->remove)) {
            Enable (vep->remove);
          }
          Update ();
        }
        ValDoNotify (vep, item, TRUE, vep->okaytosetviewtarget && vep->shftKey);
      }
    } else if (vep->clicked == 0) {
      if (vep->selected != 0) {
        old = vep->selected;
        vep->selected = 0;
        InvalBorder (d, old);
        ValDoNotify (vep, old, FALSE, FALSE);
      }
      if (Enabled (vep->remove)) {
        Disable (vep->remove);
      }
      Update ();
    }
  }
}

/*
static void RemoveProc (ButtoN b)

{
  Int2           numItems;
  Int2           old;
  RecT           r;
  ValidExtraPtr  vep;
  WindoW         w;

  vep = GetObjectExtra (b);
  if (vep != NULL) {
    if (vep->selected != 0) {
      GetDocParams (vep->doc, &numItems, NULL);
      if (vep->selected <= numItems) {
        old = vep->selected;
        vep->selected = 0;
        DeleteItem (vep->doc, old);
        if (Enabled (vep->doc) && AllParentsEnabled (vep->doc) &&
            Visible (vep->doc) && AllParentsVisible (vep->doc)) {
          w = CurrentWindow ();
          Select (vep->doc);
          ObjectRect (vep->doc, &r);
          InsetRect (&r, 3, 3);
          InvalRect (&r);
          UseWindow (w);
        }
        UpdateDocument (vep->doc, 0, 0);
        Disable (b);
      }
    }
  }
}
*/

static void CloseValidWindow (WindoW w)

{
  FreeValidateWindow ();
}

static void CloseValidButton (ButtoN b)

{
  FreeValidateWindow ();
}

extern void ClearValidateWindow (void)

{
  ErrFltrPtr     efp;
  int            sev;
  ValidExtraPtr  vep;
  ValNodePtr     vnp;

  if (validWindow != NULL) {
    vep = GetObjectExtra (validWindow);
    if (vep != NULL) {
      Reset (vep->doc);
      vep->selected = 0;
      vep->messages = ValNodeFreeData (vep->messages);
      vep->lastmessage = NULL;
      for (vnp = vep->errorfilter; vnp != NULL; vnp = vnp->next) {
        efp = (ErrFltrPtr) vnp->data.ptrvalue;
        if (efp != NULL) {
          MemFree (efp->text1);
          MemFree (efp->text2);
          MemFree (efp->text3);
        }
        vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
      }
      vep->errorfilter = ValNodeFree (vep->errorfilter);
      Hide (vep->filter);
      Update ();
      Reset (vep->filter);
      PopupItem (vep->filter, "ALL");
      SetValue (vep->filter, 1);
      Show (vep->filter);
      SetTitle (vep->summary, "");
      SetTitle (vep->showncount, "");
      for (sev = SEV_NONE; sev <= SEV_MAX; sev++) {
        vep->counts [sev] = 0;
      }
      vep->totalcount = 0;
      vep->addedcount = 0;
      vep->remaining = 0;
      SetTitle (vep->searchfor, "");
      if (Enabled (vep->find)) {
        Disable (vep->find);
      }
      Update ();
    }
  }
}

static CharPtr CharPrtProc (DoC d, Int2 item, Pointer ptr)

{
  Char           ch;
  CharPtr        str;
  Int2           tabcount;
  CharPtr        tmp;
  ValidExtraPtr  vep;

  vep = GetObjectExtra (d);
  if (vep != NULL) {
    if (ptr != NULL) {
      str = StringSave (ptr);
      if (GetValue (vep->verbose) == 3) {
        tabcount = 0;
        tmp = str;
        ch = *tmp;
        while (ch != '\0') {
          if (ch == '\t') {
            tabcount++;
            if (tabcount == 8) {
              *tmp = '\0';
              tmp--;
              if (*tmp == '\n') {
                *tmp = '\0';
              }
            } else {
              tmp++;
            }
          } else {
            tmp++;
          }
          ch = *tmp;
        }
      }
      return str;
    }
  }
  return NULL;
}

static void RepopVal (PopuP p)

{
  ErrFltrPtr     efp;
  unsigned int   entityID;
  int            errcode;
  Int2           filt;
  unsigned int   itemID;
  unsigned int   itemtype;
  Int2           minlev;
  Boolean        okay;
  int            sev;
  int            subcode;
  CharPtr        str;
  Char           tmp [32];
  Int2           val;
  ValidExtraPtr  vep;
  ValNodePtr     vnp;

  vep = GetObjectExtra (p);
  if (vep != NULL) {
    Reset (vep->doc);
    vep->addedcount = 0;
    vep->selected = 0;
    minlev = GetValue (vep->minlevel);
    filt = GetValue (vep->filter);
    val = GetValue (vep->verbose);
    efp = NULL;
    if (vep->errorfilter != NULL && filt > 1) {
      vnp = vep->errorfilter;
      while (filt > 2 && vnp != NULL) {
        vnp = vnp->next;
        filt--;
      }
      if (vnp != NULL) {
        efp = (ErrFltrPtr) vnp->data.ptrvalue;
      }
    }
    for (vnp = vep->messages; vnp != NULL; vnp = vnp->next) {
      str = ExtractTagListColumn (vnp->data.ptrvalue, 3);
      if (str != NULL &&
          sscanf (str, "%d %d %d %u %u %u", &sev, &errcode,
                  &subcode, &entityID, &itemID, &itemtype) == 6) {
        if (sev >= minlev || sev == SEV_NONE) {
          okay = FALSE;
          if (efp != NULL) {
            if (efp->errcode == errcode) {
              if (efp->subcode == INT_MIN || efp->subcode == subcode) {
                okay = TRUE;
              }
            }
          } else {
            okay = TRUE;
          }
          if (okay) {
            (vep->addedcount)++;
            if (val == 4) {
              AppendItem (vep->doc, CharPrtProc, vnp->data.ptrvalue, FALSE,
                          (StringLen (vnp->data.ptrvalue) / 50) + 1,
                          &justAccnParFmt, valColFmt, vep->font);
            } else {
              AppendItem (vep->doc, CharPrtProc, vnp->data.ptrvalue, FALSE,
                          (StringLen (vnp->data.ptrvalue) / 50) + 1,
                          &valParFmt, valColFmt, vep->font);
            }
          }
        }
      }
      MemFree (str);
    }
    if (vep->addedcount > 1) {
      sprintf (tmp, " %ld items shown", (long) vep->addedcount);
    } else if (vep->addedcount > 0) {
      sprintf (tmp, " %ld item shown", (long) vep->addedcount);
    } else {
      StringCpy (tmp, "");
    }
    SafeSetTitle (vep->showncount, tmp);
    UpdateDocument (vep->doc, 0, 0);
  }
}

static Boolean ValExportProc (ForM f, CharPtr filename)

{
  Char           buf [128];
  FILE           *fp;
  Int2           i, numItems;
  Char           path [PATH_MAX];
  CharPtr        str;
  ValidExtraPtr  vep;

  vep = (ValidExtraPtr) GetObjectExtra (f);
  if (vep != NULL && vep->doc != NULL) {
    path [0] = '\0';
    StringNCpy_0 (path, filename, sizeof (path));
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        FileClose (fp);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      fp = FileOpen (path, "w");
      if (fp != NULL) {
        if (GetValue (vep->verbose) == 4) {
           GetDocParams (vep->doc, &numItems, NULL);
          for (i = 1; i <= numItems; i++) {
            buf [0] = '\0';
            str = GetDocText (vep->doc, i, 1, 1);
            StringCat (buf, str);
            StringCat (buf, "                ");
            buf [16] = '\0';
            MemFree (str);
            str = GetDocText (vep->doc, i, 1, 2);
            StringCat (buf, str);
            StringCat (buf, "                ");
            buf [32] = '\0';
            MemFree (str);
            str = GetDocText (vep->doc, i, 1, 3);
            StringCat (buf, str);
            MemFree (str);
            fprintf (fp, "%s\n", buf);
          }
        } else {
          SaveDocument (vep->doc, fp);
        }
        FileClose (fp);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static void ValFindProc (ButtoN b)

{
  Boolean        found;
  Boolean        goOn;
  Int2           numItems;
  Int2           old;
  CharPtr        ptr;
  BaR            sb;
  Int2           start;
  Int4           startsAt;
  Int2           stop;
  Char           str [256];
  CharPtr        tmp;
  ValidExtraPtr  vep;

  vep = GetObjectExtra (b);
  if (vep != NULL) {
    vep->dblClick = FALSE;
    GetDocParams (vep->doc, &numItems, NULL);
    start = vep->selected + 1;
    if (start > numItems) {
      start = 1;
    }
    stop = start;
    GetTitle (vep->searchfor, str, sizeof (str) - 1);
    tmp = str;
    while (*tmp != '\0') {
      if (*tmp < ' ') {
        *tmp = ' ';
      }
      tmp++;
    }
    found = FALSE;
    goOn = TRUE;
    while (goOn) {
      ptr = GetDocText (vep->doc, start, 0, 0);
      if (ptr != NULL) {
        tmp = ptr;
        while (*tmp != '\0') {
          if (*tmp < ' ') {
            *tmp = ' ';
          }
          tmp++;
        }
        if (StringISearch (ptr, str) != NULL) {
          found = TRUE;
          goOn = FALSE;
        }
        ptr = MemFree (ptr);
      }
      if (goOn) {
        start++;
        if (start > numItems) {
          start = 1;
        }
        if (start == stop) {
          goOn = FALSE;
        }
      }
    }
    if (found) {
      old = vep->selected;
      vep->selected = start;
      if (old != start) {
        if (old == 0) {
          InvalBorder (vep->doc, start);
        } else {
          InvalBorder (vep->doc, MIN (start, old));
          InvalBorder (vep->doc, MAX (start, old));
          ValDoNotify (vep, old, FALSE, FALSE);
        }
        if (! Enabled (vep->remove)) {
          Enable (vep->remove);
        }
        Update ();
      }
      ValDoNotify (vep, start, TRUE, FALSE);
      if (ItemIsVisible (vep->doc, start, NULL, NULL, NULL)) {
      } else {
        GetItemParams4 (vep->doc, start, &startsAt, NULL, NULL, NULL, NULL);
        sb = GetSlateVScrollBar ((SlatE) vep->doc);
        SetBarValue (sb, startsAt);
      }
    } else {
      Beep ();
    }
  }
}

static void ValTextProc (TexT t)

{
  ValidExtraPtr  vep;

  vep = GetObjectExtra (t);
  if (vep != NULL) {
    if (TextLength (t) > 0) {
      if (! Enabled (vep->find)) {
        Enable (vep->find);
      }
    } else {
      if (Enabled (vep->find)) {
        Disable (vep->find);
      }
    }
  }
}

static void RevalidateProc (ButtoN b)

{
  BaseFormPtr    bfp;
  int            i;
  ValidExtraPtr  vep;

  vep = GetObjectExtra (b);
  if (vep != NULL) {
    for (i = SEV_NONE; i <= SEV_MAX; i++) {
      vep->counts [i] = 0;
    }
    vep->totalcount = 0;
    vep->addedcount = 0;
    vep->remaining = 0;
    vep->clicked = 0;
    vep->selected = 0;
    vep->dblClick = FALSE;
    bfp = vep->bfp;
    if (bfp != NULL && vep->revalProc != NULL) {
      vep->revalProc (bfp->form);
    }
  }
}

static Boolean doSuppressContext = TRUE;

extern Boolean ShouldSetSuppressContext (void)

{
  return doSuppressContext;
}

static Boolean doJustShowAccession = FALSE;

extern Boolean ShouldSetJustShowAccession (void)

{
  return doJustShowAccession;
}

static void SetVerbosityAndRevalidate (PopuP p)

{
  BaseFormPtr    bfp;
  int            i;
  Int2           val;
  ValidExtraPtr  vep;

  vep = GetObjectExtra (p);
  if (vep != NULL) {
    val = GetValue (vep->verbose);
    if (val > 3) {
      doSuppressContext = TRUE;
      doJustShowAccession = TRUE;
    } else if (val > 1) {
      doSuppressContext = TRUE;
      doJustShowAccession = FALSE;
    } else {
      doSuppressContext = FALSE;
      doJustShowAccession = FALSE;
    }
    for (i = SEV_NONE; i <= SEV_MAX; i++) {
      vep->counts [i] = 0;
    }
    vep->totalcount = 0;
    vep->addedcount = 0;
    vep->remaining = 0;
    vep->clicked = 0;
    vep->selected = 0;
    vep->dblClick = FALSE;
    bfp = vep->bfp;
    if (bfp != NULL && vep->revalProc != NULL) {
      vep->revalProc (bfp->form);
    }
  }
}

static void CleanupValidProc (GraphiC g, VoidPtr data)

{
  ErrFltrPtr     efp;
  ValidExtraPtr  vep;
  ValNodePtr     vnp;

  vep = (ValidExtraPtr) data;
  if (vep != NULL) {
    vep->messages = ValNodeFreeData (vep->messages);
    vep->lastmessage = NULL;
    for (vnp = vep->errorfilter; vnp != NULL; vnp = vnp->next) {
      efp = (ErrFltrPtr) vnp->data.ptrvalue;
      if (efp != NULL) {
        MemFree (efp->text1);
        MemFree (efp->text2);
        MemFree (efp->text3);
      }
      vnp->data.ptrvalue = MemFree (vnp->data.ptrvalue);
    }
    vep->errorfilter = ValNodeFree (vep->errorfilter);
  }
  StdCleanupFormProc (g, data);
}

static void CopyValToClipboard (ValidExtraPtr vep)

{
  FILE         *fp;
  Char         path [PATH_MAX];

  if (vep == NULL) return;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    SaveDocument (vep->doc, fp);
    FileClose (fp);
    FileToClipboard (path);
  }
  FileRemove (path);
}

static void PrintValProc (ValidExtraPtr vep)

{
  if (vep == NULL) return;
  WatchCursor ();
  Update ();
  PrintDocument (vep->doc);
  ArrowCursor ();
  Update ();
}

static void ValFormMessage (ForM f, Int2 mssg)

{
  ValidExtraPtr  vep;

  vep = (ValidExtraPtr) GetObjectExtra (f);
  if (vep != NULL) {
    switch (mssg) {
      case VIB_MSG_EXPORT :
        ValExportProc (f, NULL);
        break;
      case VIB_MSG_PRINT :
        PrintValProc (vep);
        break;
      case VIB_MSG_CLOSE :
        FreeValidateWindow ();
        break;
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        CopyValToClipboard (vep);
        break;
      case VIB_MSG_PASTE :
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE :
        ClearValidateWindow ();
        break;
      default :
        if (vep->appmessage != NULL) {
          vep->appmessage (f, mssg);
        }
        break;
    }
  }
}

#ifndef WIN_MAC
extern void CreateStdValidatorFormMenus (WindoW w)

{
  BaseFormPtr   bfp;
  MenU          m;

  bfp = (BaseFormPtr) GetObjectExtra (w);
  if (bfp != NULL) {
    m = PulldownMenu (w, "File");
    if (bfp->importform != NULL || bfp->exportform != NULL) {
      if (bfp->importform != NULL) {
        FormCommandItem (m, "Import...", bfp, VIB_MSG_IMPORT);
      }
      if (bfp->exportform != NULL) {
        FormCommandItem (m, "Export...", bfp, VIB_MSG_EXPORT);
      }
      SeparatorItem (m);
    }
    FormCommandItem (m, "Print", bfp, VIB_MSG_PRINT);
    SeparatorItem (m);
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, CUT_MENU_ITEM, bfp, VIB_MSG_CUT);
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);
    FormCommandItem (m, PASTE_MENU_ITEM, bfp, VIB_MSG_PASTE);
    FormCommandItem (m, CLEAR_MENU_ITEM, bfp, VIB_MSG_DELETE);
  }
}
#endif

static CharPtr canStillSubmitText =
"Please review and correct the following errors. If you are unable to resolve " \
"an error, submit your sequences, and our annotation staff may contact you " \
"to help resolve the remaining errors when your sequences are processed.";

static CharPtr howToClickText =
"Click on an error item to select and scroll to that feature. " \
"Shift click to choose target sequence and feature if in a set of multiple sequences. " \
"Double click on an error item to launch the appropriate feature editor.";

/* CreateValidateWindowEx is hidden, allowing a revalidate button */
extern void CreateValidateWindowEx (ErrNotifyProc notify, CharPtr title,
                                    FonT font, ErrSev sev, Boolean verbose,
                                    BaseFormPtr bfp, FormActnFunc revalProc,
                                    Boolean okaytosetviewtarget)

{
  ButtoN             b;
  GrouP              c;
  GrouP              f;
  GrouP              g;
  GrouP              h;
  int                i;
  GrouP              ppt;
  GrouP              q;
  StdEditorProcsPtr  sepp;
  GrouP              t;
  ValidExtraPtr      vep;
  WindoW             w;

  if (validWindow == NULL) {
    vep = (ValidExtraPtr) MemNew (sizeof (ValidExtra));
    if (vep != NULL) {
      if (title == NULL || title [0] == '\0') {
        title = "Validation Errors";
      }
      w = FixedWindow (-50, -33, -10, -10, title, CloseValidWindow);
      SetObjectExtra (w, vep, CleanupValidProc);
      vep->form = (ForM) w;
      vep->exportform = ValExportProc;
      vep->formmessage = ValFormMessage;

#ifndef WIN_MAC
      CreateStdValidatorFormMenus (w);
#endif

      sepp = (StdEditorProcsPtr) GetAppProperty ("StdValidatorForm");
      if (sepp == NULL) {
        sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
      }
      if (sepp != NULL) {
        SetActivate (w, sepp->activateForm);
        vep->appmessage = sepp->handleMessages;
      }

      if (w != NULL) {
        vep->notify = notify;

        ppt = MultiLinePrompt (w, canStillSubmitText, stdCharWidth * 30, systemFont);

        c = HiddenGroup (w, 8, 0, NULL);
        SetGroupSpacing (c, 10, 2);
        /*
        vep->remove = PushButton (c, "Remove", RemoveProc);
        Disable (vep->remove);
        */
        StaticPrompt (c, "Message", 0, popupMenuHeight, programFont, 'l');
        vep->verbose = PopupList (c, FALSE, SetVerbosityAndRevalidate);
        SetObjectExtra (vep->verbose, vep, NULL);
        PopupItem (vep->verbose, "Verbose");
        PopupItem (vep->verbose, "Normal");
        PopupItem (vep->verbose, "Terse");
        PopupItem (vep->verbose, "Table");
        if (GetAppProperty ("InternalNcbiSequin") != NULL) {
          SetValue (vep->verbose, 1);
          doSuppressContext = FALSE;
          doJustShowAccession = FALSE;
        } else if (verbose) {
          SetValue (vep->verbose, 2);
          doSuppressContext = TRUE;
          doJustShowAccession = FALSE;
        } else {
          SetValue (vep->verbose, 3);
          doSuppressContext = TRUE;
          doJustShowAccession = FALSE;
        }
        StaticPrompt (c, "Severity", 0, popupMenuHeight, programFont, 'l');
        vep->minlevel = PopupList (c, FALSE, RepopVal); /* was SetVerbosityAndRevalidate */
        SetObjectExtra (vep->minlevel, vep, NULL);
        PopupItem (vep->minlevel, "INFO");
        PopupItem (vep->minlevel, "WARN");
        PopupItem (vep->minlevel, "ERROR");
        PopupItem (vep->minlevel, "REJECT");
        if (sev >= 1 && sev <= 4) {
          SetValue (vep->minlevel, (Int2) sev);
        } else {
          SetValue (vep->minlevel, 1);
        }
        vep->revalBtn = PushButton (c, "Revalidate", RevalidateProc);
        SetObjectExtra (vep->revalBtn, vep, NULL);
        Hide (vep->revalBtn);
        /*
        Advance (w);
        */
        Break (w);

        q = HiddenGroup (w, -2, 0, NULL);
        StaticPrompt (q, "Filter", 0, popupMenuHeight, programFont, 'l');
        vep->filter = PopupList (q, FALSE, RepopVal);
        SetObjectExtra (vep->filter, vep, NULL);
        PopupItem (vep->filter, "ALL");
        SetValue (vep->filter, 1);
        Break (w);

        f = HiddenGroup (w, 6, 0, NULL);
        vep->find = PushButton (f, "Find", ValFindProc);
        SetObjectExtra (vep->find, vep, NULL);
        Disable (vep->find);
        vep->searchfor = DialogText (f, "", 15, ValTextProc);
        SetObjectExtra (vep->searchfor, vep, NULL);
        vep->showncount = StaticPrompt (f, " 0000000 items shown", 0, dialogTextHeight, systemFont, 'l');
        SetTitle (vep->showncount, "");

        g = HiddenGroup (w, 0, -5, NULL);
        h = HiddenGroup (g, 5, 0, NULL);
        SetGroupMargins (h, 11, 0);
        SetGroupSpacing (h, 0, 2);
        /*
        StaticPrompt (h, "Severity", stdCharWidth * 5 - 2, 0, programFont, 'l');
        StaticPrompt (h, "Error", stdCharWidth * 6, 0, programFont, 'l');
        StaticPrompt (h, "Subcode", stdCharWidth * 9, 0, programFont, 'l');
        StaticPrompt (h, "Message", stdCharWidth * 20, 0, programFont, 'l');
        */
        t = MultiLinePrompt (g, howToClickText, stdCharWidth * 30, systemFont);
        vep->doc = DocumentPanel (g, stdCharWidth * 30, stdLineHeight * 20);
        SetObjectExtra (vep->doc, vep, NULL);
        SetDocAutoAdjust (vep->doc, FALSE);
        SetDocProcs (vep->doc, ClickValid, NULL, ReleaseValid, NULL);
        SetDocShade (vep->doc, DrawValid, NULL, NULL, NULL);
        vep->font = font;

        for (i = SEV_NONE; i <= SEV_MAX; i++) {
          vep->counts [i] = 0;
        }
        vep->totalcount = 0;
        vep->addedcount = 0;
        vep->remaining = 0;
        vep->clicked = 0;
        vep->selected = 0;
        vep->dblClick = FALSE;

        valColFmt [0].pixWidth = stdCharWidth * 5;
        valColFmt [1].pixWidth = stdCharWidth * 6;
        valColFmt [2].pixWidth = stdCharWidth * 19;
        valColFmt [3].pixWidth = stdCharWidth * 20;
        valColFmt [4].pixWidth = stdCharWidth * 30;
        valColFmt [4].pixInset = stdCharWidth * 5 + 5;
        /*
        valColFmt [0].font = systemFont;
        valColFmt [1].font = systemFont;
        valColFmt [2].font = systemFont;
        */

        vep->summary = StaticPrompt (w, "", stdCharWidth * 30, 0, systemFont, 'c');

        b = PushButton (w, "Dismiss", CloseValidButton);

        AlignObjects (ALIGN_CENTER, (HANDLE) ppt, (HANDLE) vep->doc, (HANDLE) t,
                      (HANDLE) vep->summary, (HANDLE) b, NULL);

        RealizeWindow (w);
        validWindow = w;
      } else {
        MemFree (vep);
      }
    }
  }
  if (validWindow != NULL) {
    vep = (ValidExtraPtr) GetObjectExtra (validWindow);
    if (vep != NULL) {
      vep->bfp = bfp;
      vep->revalProc = revalProc;
      if (vep->revalProc != NULL) {
        SafeShow (vep->revalBtn);
      } else {
        SafeHide (vep->revalBtn);
      }
      vep->okaytosetviewtarget = okaytosetviewtarget;
    }
  }
}

extern void CreateValidateWindow (ErrNotifyProc notify, CharPtr title,
                                  FonT font, ErrSev sev, Boolean verbose)

{
  CreateValidateWindowEx (notify, title, font, sev, verbose, NULL, NULL, FALSE);
}

extern void ShowValidateDoc (void)

{
  Char           str [64];
  ValidExtraPtr  vep;

  if (validWindow == NULL) return;
  vep = GetObjectExtra (validWindow);
  if (vep == NULL) return;
  RepopVal (vep->filter);
  SafeShow (vep->doc);
  SafeShow (vep->searchfor);
  sprintf (str, "INFO: %ld      WARNING: %ld      ERROR: %ld      REJECT: %ld",
           (long) vep->counts [SEV_INFO], (long) vep->counts [SEV_WARNING],
           (long) vep->counts [SEV_ERROR], (long) vep->counts [SEV_REJECT]);
  SafeSetTitle (vep->summary, str);
  if (vep->addedcount > 1) {
    sprintf (str, " %ld items shown", (long) vep->addedcount);
  } else if (vep->addedcount > 0) {
    sprintf (str, " %ld item shown", (long) vep->addedcount);
  } else {
    StringCpy (str, "");
  }
  SafeSetTitle (vep->showncount, str);
  Update ();
  ShowValidateWindow ();
  Update ();
}

extern void HideValidateDoc (void)

{
  ValidExtraPtr  vep;

  if (validWindow == NULL) return;
  vep = GetObjectExtra (validWindow);
  if (vep == NULL) return;
  SafeHide (vep->doc);
  SafeHide (vep->searchfor);
  Update ();
}

extern void ShowValidateWindow (void)

{
  ValidExtraPtr  vep;

  if (validWindow == NULL) {
    CreateValidateWindow (NULL, "Validation Errors",
                          SetSmallFont (), SEV_ERROR, FALSE);
  }
  if (validWindow != NULL) {
    vep = GetObjectExtra (validWindow);
    if (vep != NULL) {
      SetTitle (vep->searchfor, "");
      if (Enabled (vep->find)) {
        Disable (vep->find);
      }
    }
    if (! Visible (validWindow)) {
      if (vep->totalcount > 0) {
        Show (validWindow);
      }
    }
    if (Visible (validWindow)) {
      if (Visible (vep->doc)) {
        Select (validWindow);
      }
    }
  }
}

static CharPtr StringMoveAndConvertTabs (CharPtr dst, CharPtr src, CharPtr suffix)

{
  Char     ch;
  CharPtr  ptr;

  ptr = dst;
  if (dst != NULL) {
    if (src != NULL) {
      ptr = StringMove (dst, src);
      ch = *dst;
      while (ch != '\0') {
        if (ch < ' ') {
          *dst = ' ';
        }
        dst++;
        ch = *dst;
      }
    }
    if (suffix != NULL) {
      ptr = StringMove (ptr, suffix);
    }
  }
  return ptr;
}

static CharPtr severityLabel [] = {
  "NONE", "INFO", "WARN", "ERROR", "REJECT", "FATAL", "MAX", NULL
};

static void FillValidBuffer (CharPtr buf, CharPtr text1, CharPtr text2,
                             CharPtr text3, CharPtr hidden, CharPtr message,
                             CharPtr expanded, ValNodePtr context,
                             Boolean justShowAccession)
{
  CharPtr  ptr;
  Char     temp [256];
  /*
  CharPtr     ctxstr;
  Boolean     notfirst;
  ValNodePtr  vnp;
  */

  if (buf != NULL) {
    ptr = buf;
    /*
    if (context != NULL) {
      vnp = context;
      notfirst = FALSE;
      ptr = StringMoveAndConvertTabs (ptr, NULL, "\t\t\t\t");
      while (vnp != NULL) {
        ctxstr = (CharPtr) vnp->data.ptrvalue;
        if (ctxstr != NULL && ctxstr [0] != '\0') {
          if (notfirst) {
            ptr = StringMoveAndConvertTabs (ptr, NULL, "; ");
          }
          ptr = StringMoveAndConvertTabs (ptr, ctxstr, "");
          notfirst = TRUE;
        }
        vnp = vnp->next;
      }
      ptr = StringMoveAndConvertTabs (ptr, NULL, "\n");
    }
    */
    if (justShowAccession) {
      ptr = StringMoveAndConvertTabs (ptr, message, "\t");
      ptr = StringMoveAndConvertTabs (ptr, text1, "\t");
      StringCpy (temp, text2);
      StringCat (temp, "_");
      StringCat (temp, text3);
      ptr = StringMoveAndConvertTabs (ptr, temp, "\t");
      ptr = StringMoveAndConvertTabs (ptr, hidden, "\n");
    } else {
      ptr = StringMoveAndConvertTabs (ptr, text1, "\t");
      ptr = StringMoveAndConvertTabs (ptr, text2, "\t");
      ptr = StringMoveAndConvertTabs (ptr, text3, "\t");
      ptr = StringMoveAndConvertTabs (ptr, hidden, "\n\t\t\t\t");
      ptr = StringMoveAndConvertTabs (ptr, message, "\n");
      if (expanded != NULL && expanded [0] != '\0') {
        ptr = StringMoveAndConvertTabs (ptr, NULL, "\n\t\t\t\t");
        ptr = StringMoveAndConvertTabs (ptr, expanded, "\n");
      }
    }
  }
}

extern void RepopulateValidateFilter (void)

{
  ErrFltrPtr     curr;
  RecT           r;
  Char           str [128];
  ValidExtraPtr  vep;
  ValNodePtr     vnp;

  ShowValidateWindow ();
  vep = (ValidExtraPtr) GetObjectExtra (validWindow);
  if (vep != NULL) {
    Hide (vep->filter);
    Update ();
    Reset (vep->filter);
    vep->selected = 0;
    PopupItem (vep->filter, "ALL");
    SetValue (vep->filter, 1);
    vnp = vep->errorfilter;
    while (vnp != NULL) {
      curr = (ErrFltrPtr) vnp->data.ptrvalue;
      if (curr != NULL) {
        str [0] = '\0';
        if (! StringHasNoText (curr->text2)) {
          StringNCpy_0 (str, curr->text2, sizeof (str) - 5);
          if (curr->subcode != INT_MIN) {
            if (! StringHasNoText (curr->text3)) {
              StringCat (str, ": ");
              StringCat (str, curr->text3);
            }
          }
        } else {
          StringCpy (str, "??");
        }
        PopupItem (vep->filter, str);
      }
      vnp = vnp->next;
    }
    ObjectRect (vep->filter, &r);
    AdjustPrnt (vep->filter, &r, FALSE);
    Show (vep->filter);
    Update ();
  }
}

static void AppendFilter (ValidExtraPtr vep, CharPtr text1, CharPtr text2,
                          CharPtr text3, ErrSev sev, int errcode, int subcode)

{
  ErrFltrPtr  curr;
  ErrFltrPtr  efp;
  ValNodePtr  last;
  ValNodePtr  tmp;
  ValNodePtr  vnp;

  if (vep != NULL) {
    efp = MemNew (sizeof (ErrFltrData));
    if (efp != NULL) {
      efp->sev = sev;
      efp->errcode = errcode;
      efp->subcode = subcode;
      if (vep->errorfilter == NULL) {
        vnp = ValNodeNew (NULL);
        vep->errorfilter = vnp;
        if (vnp != NULL) {
          vnp->data.ptrvalue = efp;
          efp->text1 = StringSave (text1);
          efp->text2 = StringSave (text2);
          efp->text3 = StringSave (text3);
        }
      } else {
        last = NULL;
        for (vnp = vep->errorfilter; vnp != NULL; vnp = vnp->next) {
          curr = (ErrFltrPtr) vnp->data.ptrvalue;
          if (curr != NULL) {
            if (curr->errcode == efp->errcode &&
                curr->subcode == efp->subcode) {
              MemFree (efp);
              return;
            }
            if (curr->errcode > efp->errcode ||
                (curr->errcode == efp->errcode && curr->subcode > efp->subcode)) {
              tmp = ValNodeNew (NULL);
              if (tmp != NULL) {
                tmp->data.ptrvalue = efp;
                efp->text1 = StringSave (text1);
                efp->text2 = StringSave (text2);
                efp->text3 = StringSave (text3);
                if (last != NULL) {
                  tmp->next = vnp;
                  last->next = tmp;
                } else {
                  tmp->next = vep->errorfilter;
                  vep->errorfilter = tmp;
                }
              }
              return;
            }
          }
          last = vnp;
        }
        tmp = ValNodeNew (vep->errorfilter);
        if (tmp != NULL) {
          tmp->data.ptrvalue = efp;
          efp->text1 = StringSave (text1);
          efp->text2 = StringSave (text2);
          efp->text3 = StringSave (text3);
        }
      }
    }
  }
}

static void ProcessValidMessage (ValidExtraPtr vep, CharPtr text1, CharPtr text2,
                                 CharPtr text3, ErrSev sev, int errcode, int subcode,
                                 Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                 CharPtr message, CharPtr expanded, ValNodePtr context,
                                 size_t ctxlen, Int2 minlev)

{
  CharPtr     buf;
  Boolean     justShowAccession = FALSE;
  size_t      len;
  /* Int2        numItems; */
  Char        str [64];
  Int2        val;
  ValNodePtr  vnp;

  if (vep != NULL) {
    val = GetValue (vep->verbose);
    if (val == 4) {
      justShowAccession = TRUE;
    }
    len = StringLen (text1) + StringLen (text2) + StringLen (text3) +
          StringLen (message) + StringLen (expanded) + 50 + ctxlen;
    buf = MemGet (len, MGET_CLEAR);
    if (buf != NULL) {
      sprintf (str, "%d %d %d %u %u %u", (int) sev, (int) errcode,
               (int) subcode, (unsigned int) entityID,
               (unsigned int) itemID, (unsigned int) itemtype);
      FillValidBuffer (buf, text1, text2, text3, str,
                       message, expanded, context, justShowAccession);
      AppendFilter (vep, text1, text2, NULL, sev, errcode, INT_MIN);
      AppendFilter (vep, text1, text2, text3, sev, errcode, subcode);
      vnp = ValNodeNew (vep->lastmessage);
      if (vep->messages == NULL) {
        vep->messages = vnp;
      }
      vep->lastmessage = vnp;
      if (vnp != NULL) {
        vnp->data.ptrvalue = buf;
        if (sev >= minlev || sev == SEV_NONE) {
          (vep->addedcount)++;
          /*
          AppendItem (vep->doc, CharPrtProc, vnp->data.ptrvalue, FALSE,
                      (StringLen (vnp->data.ptrvalue) / 50) + 1,
                      &valParFmt, valColFmt, vep->font);
          GetDocParams (vep->doc, &numItems, NULL);
          if (Visible (vep->doc) && AllParentsVisible (vep->doc)) {
            UpdateDocument (vep->doc, numItems, numItems);
          }
          */
        }
      }
    }
  }
}

extern void AppendValidMessage (CharPtr text1, CharPtr text2, CharPtr text3,
                                ErrSev sev, int errcode, int subcode,
                                Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                CharPtr message, CharPtr expanded, ValNodePtr context)

{
  size_t         ctxlen;
  CharPtr        ctxstr;
  Int2           minlev;
  Char           str [64];
  ValidExtraPtr  vep;
  ValNodePtr     vnp;

  ShowValidateWindow ();
  if (validWindow != NULL) {
    vep = GetObjectExtra (validWindow);
    if (vep != NULL) {
      if (sev < SEV_NONE || sev > SEV_MAX) {
        sev = SEV_MAX;
      }
      (vep->counts [sev])++;
      (vep->totalcount)++;
      if (vep->remaining > 0) {
        (vep->remaining)--;
      }
      if (vep->remaining < 1) {
        sprintf (str, "INFO: %ld      WARNING: %ld      ERROR: %ld      REJECT: %ld",
                 (long) vep->counts [SEV_INFO], (long) vep->counts [SEV_WARNING],
                 (long) vep->counts [SEV_ERROR], (long) vep->counts [SEV_REJECT]);
        if (vep->totalcount < 30) {
          vep->remaining = 1;
        } else if (vep->totalcount < 100) {
          vep->remaining = 5;
        } else if (vep->totalcount < 300) {
          vep->remaining = 20;
        } else if (vep->totalcount < 1000) {
          vep->remaining = 50;
        } else if (vep->totalcount < 3000) {
          vep->remaining = 100;
        } else if (vep->totalcount < 10000) {
          vep->remaining = 200;
        } else if (vep->totalcount < 30000) {
          vep->remaining = 500;
        } else {
          vep->remaining = 1000;
	    }
        SetTitle (vep->summary, str);
        Update ();
      }
      if (text1 == NULL) {
        text1 = severityLabel [sev];
      }
      if (message == NULL) {
        message = "";
      }
      minlev = GetValue (vep->minlevel);
      ctxlen = 0;
      vnp = context;
      while (vnp != NULL) {
        ctxstr = (CharPtr) vnp->data.ptrvalue;
        if (ctxstr != NULL && ctxstr [0] != '\0') {
          ctxlen += StringLen (ctxstr) + 4;
        }
        vnp = vnp->next;
      }
      /*
      if (GetValue (vep->verbose) == 3) {
        expanded = NULL;
      }
      */
      if (ctxlen == 0) {
        context = NULL;
      }
      ProcessValidMessage (vep, text1, text2, text3, sev, errcode,
                           subcode, entityID, itemID, itemtype,
                           message, expanded, context, ctxlen, minlev); 
      /*
      if (Visible (validWindow)) {
        Select (validWindow);
      }
      */
    }
  }
}

extern int LIBCALLBACK ValidErrHook (const ErrDesc *err)

{
  CharPtr     expanded;
  ErrSev      msg_level;
  CharPtr     name1;
  CharPtr     name2;
  ErrMsgNode  *node1;
  ErrMsgNode  *node2;
  ErrMsgRoot  *root;
  ErrSev      sev;

  if (err != NULL) {
    /* FileOpen report matches ERR_SEQ_DESCR_FileOpenCollision instead of ERR_SEQ_DESCR_Inconsistent */
    if (err->errcode == E_File && err->subcode == E_FOpen) return 1;
    /* suppress connector messages with 0, 0 as code and subcode - reverted after calling ErrSetLogLevel to suppress these */
    /* if (StringCmp (err->errtext, "[CONN_Read]  Cannot read data (connector \"HTTP\", error \"Closed\")") == 0) return 1; */
    msg_level = ErrGetMessageLevel ();
    name1 = NULL;
    name2 = NULL;
    if (err->module [0] != '\0') {
      root = (ErrMsgRoot*) ErrGetMsgRoot (err->module);
      if (root != NULL) {
        for (node1 = root->list; node1 != NULL; node1 = node1->next) {
          if (node1->code == err->errcode) {
            for (node2 = node1->list; node2 != NULL; node2 = node2->next) {
              if (node2->code == err->subcode) {
                name1 = (CharPtr) node1->name;
                name2 = (CharPtr) node2->name;
              }
            }
          }
        }
      }
    }
    sev = (ErrSev) err->severity;
    if (sev < SEV_NONE || sev > SEV_MAX) {
      sev = SEV_MAX;
    }
    /* suppress if severity is less than message level */
    if (sev < msg_level) return 1;
    expanded = (CharPtr) ErrGetExplanation (err->root, err->node);
    AppendValidMessage (severityLabel [sev], name1, name2,
                        (ErrSev) err->severity, err->errcode, err->subcode,
                        err->entityID, err->itemID, err->itemtype,
                        (CharPtr) err->errtext, expanded,
                        ErrGetUserStrings(err));
  }
  return 1;
}

typedef struct searchextra {
  ButtoN            find;
  TexT              searchfor;
  GrouP             features;
  GrouP             featurebuttons;
  ButtoN            featstouse [86];
  GrouP             descriptors;
  GrouP             descriptorbuttons;
  ButtoN            descstouse [25];
  GrouP             pubs;
  GrouP             pubbuttons;
  ButtoN            pubstouse [5];
  Uint2             entityID;
  SearchGatherProc  gather;
} SearchExtra, PNTR SearchExtraPtr;

typedef struct replaceextra {
  DoC                doc;
  FonT               font;
  ButtoN             find;
  TexT               searchfor;
  TexT               replaceWith;
  Int2               clicked;
  Int2               selected;
  Boolean            dblClick;
  Boolean            verbose;
  ReplaceNotifyProc  notify;
} ReplaceExtra, PNTR ReplaceExtraPtr;

static WindoW  searchWindow = NULL;
static WindoW  replaceWindow = NULL;

static ParData repParFmt = {TRUE, FALSE, FALSE, FALSE, FALSE, 0, 0};

static ColData repColFmt [] = {
  {0,  7, 75,  0, NULL, 'l', TRUE,  FALSE, FALSE, FALSE, FALSE}, /* label  */
  {0,  0,  0,  0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE}   /* hidden */
};

extern void FreeSearchWindow (void)

{
  if (searchWindow != NULL) {
    Remove (searchWindow);
    searchWindow = NULL;
  }
}

static void CloseSearchWindow (WindoW w)

{
  FreeSearchWindow ();
}

static void CloseSearchButton (ButtoN b)

{
  FreeSearchWindow ();
}

static void SearchTextProc (TexT t)

{
  SearchExtraPtr  sep;

  sep = GetWindowExtra (ParentWindow (t));
  if (sep != NULL) {
    if (TextLength (t) > 0) {
      if (! Enabled (sep->find)) {
        Enable (sep->find);
      }
    } else {
      if (Enabled (sep->find)) {
        Disable (sep->find);
      }
    }
  }
}

/*
static void SearchFeatProc (GrouP g)

{
  SearchExtraPtr  sep;
  Int2            val;

  sep = GetWindowExtra (ParentWindow (g));
  if (sep != NULL) {
    val = GetValue (g);
    switch (val) {
      case 1 :
        Disable (sep->featurebuttons);
        break;
      case 2 :
        Enable (sep->featurebuttons);
        break;
      case 3 :
        Disable (sep->featurebuttons);
        break;
      default :
        break;
    }
  }
}

static void SearchDescProc (GrouP g)

{
  SearchExtraPtr  sep;
  Int2            val;

  sep = GetWindowExtra (ParentWindow (g));
  if (sep != NULL) {
    val = GetValue (g);
    switch (val) {
      case 1 :
        Disable (sep->descriptorbuttons);
        break;
      case 2 :
        Enable (sep->descriptorbuttons);
        break;
      case 3 :
        Disable (sep->descriptorbuttons);
        break;
      default :
        break;
    }
  }
}

static void SearchPubProc (GrouP g)

{
  SearchExtraPtr  sep;
  Int2            val;

  sep = GetWindowExtra (ParentWindow (g));
  if (sep != NULL) {
    val = GetValue (g);
    switch (val) {
      case 1 :
        Disable (sep->pubbuttons);
        break;
      case 2 :
        Enable (sep->pubbuttons);
        break;
      case 3 :
        Disable (sep->pubbuttons);
        break;
      default :
        break;
    }
  }
}
*/

typedef struct searchlist {
  Uint2             subtype;
  Char              thislabel [41];
  CharPtr           searchFor;
  SearchGatherProc  gather;
  ObjMgrPtr         omp;
  SearchExtraPtr    sep;
} SearchList, PNTR SearchPtr;

static StdPrintOptionsPtr searchSpop = NULL;

static Boolean SearchGatherFunc (GatherContextPtr gcp)

{
  ObjMgrTypePtr  omtp;
  SearchPtr      sp;
  Boolean        success;
  CharPtr        text;
  CharPtr        tmp;

  if (gcp == NULL) return TRUE;

  sp = (SearchPtr) gcp->userdata;
  if (sp == NULL) return TRUE;

  if (sp->gather == NULL) return TRUE;

  sp->subtype = 0;
  sp->thislabel [0] = '\0';

  if (gcp->thistype == OBJ_SEQDESC ||
      gcp->thistype == OBJ_SEQFEAT) {
    omtp = ObjMgrTypeFind (sp->omp, gcp->thistype, NULL, NULL);
    if (omtp == NULL) {
      return TRUE;
    }
    if (omtp->subtypefunc != NULL) {
      sp->subtype = (*(omtp->subtypefunc)) (gcp->thisitem);
    }
    if (omtp->labelfunc != NULL) {
      (*(omtp->labelfunc)) (gcp->thisitem, sp->thislabel, 40, OM_LABEL_BOTH);
    }
  }

  /* filter here */

  if (gcp->thisitem != NULL) {
    success = FALSE;
    switch (gcp->thistype) {
      case OBJ_SEQDESC :
        success = StdFormatPrint (gcp->thisitem, (AsnWriteFunc) SeqDescAsnWrite,
                                  "StdSeqDesc", searchSpop);
        break;
      case OBJ_SEQFEAT :
        success = StdFormatPrint (gcp->thisitem, (AsnWriteFunc) SeqFeatAsnWrite,
                                  "StdSeqFeat", searchSpop);
        break;
      case OBJ_SEQFEAT_CIT :
        success = StdFormatPrint (gcp->thisitem, (AsnWriteFunc) PubAsnWrite,
                                  "StdPubOnFeat", searchSpop);
        break;
      default :
        return TRUE;
    }
    if (success && searchSpop->ptr != NULL &&
        *((CharPtr) (searchSpop->ptr)) != '\0') {
      text = searchSpop->ptr;
      tmp = text;
      while (*tmp != '\0') {
        if (*tmp < ' ') {
          *tmp = ' ';
        }
        tmp++;
      }
      if (StringISearch (text, sp->searchFor) != NULL) {
        sp->gather (sp->searchFor, searchSpop->ptr, sp->thislabel,
                    gcp->entityID, gcp->itemID, gcp->thistype, sp->subtype);
      }
      searchSpop->ptr = MemFree (searchSpop->ptr);
    }
  }

  return TRUE;
}

static void SearchFindProc (ButtoN b)

{
  GatherScope     gs;
  SearchExtraPtr  sep;
  SearchList      sl;
  Char            str [256];
  CharPtr         tmp;
  WindoW          w;

  w = ParentWindow (b);
  sep = GetWindowExtra (w);
  if (sep != NULL && sep->gather != NULL && sep->entityID != 0) {
    WatchCursor ();
    if (searchSpop == NULL) {
      searchSpop = StdPrintOptionsNew (NULL);
      if (searchSpop != NULL) {
        searchSpop->newline = "\r";
        searchSpop->indent = "";
      } else {
        Message (MSG_ERROR, "StdPrintOptionsNew failed");
        return;
      }
    }
    GetTitle (sep->searchfor, str, sizeof (str) - 1);
    tmp = str;
    while (*tmp != '\0') {
      if (*tmp < ' ') {
        *tmp = ' ';
      }
      tmp++;
    }
    Hide (w);
    sl.omp = ObjMgrGet ();
    sl.searchFor = str;
    sl.gather = sep->gather;
    sl.sep = sep;
    MemSet (&gs, 0, sizeof (GatherScope));
    gs.seglevels = 1;
    gs.get_feats_location = FALSE;
	MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
	gs.ignore[OBJ_BIOSEQ] = FALSE;
	gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
	gs.ignore[OBJ_SEQSUB] = FALSE;
	gs.ignore[OBJ_SEQDESC] = FALSE;
	gs.ignore[OBJ_SEQFEAT] = FALSE;
	gs.ignore[OBJ_SEQANNOT] = FALSE;
    GatherEntity (sep->entityID, (Pointer) &sl, SearchGatherFunc, &gs);
    searchSpop = StdPrintOptionsFree (searchSpop);
    ArrowCursor ();
  }
}

static void CleanupSearchProc (WindoW w, VoidPtr data)

{
  SearchExtraPtr  sep;

  sep = (SearchExtraPtr) data;
  if (sep != NULL) {
  }
  MemFree (data);
}

static CharPtr descriptorNames [] = {
  "", "Molecule Type", "Modifiers", "Method", "Name", "Title",
  "Organism", "Comment", "Numbering", "Map Location", "PIR", 
  "GenBank", "Publication", "Region", "User Object", "SWISSPROT", 
  "Neighbors", "EMBL", "Create Date", "Update Date",
  "PRF",  "PDB", "Heterogen", "BioSource", "Molecule Info", NULL
};

static CharPtr pubNames [] = {
  "", "Pub Descriptor", "Pub Feature", "Cit On Feature", NULL
};

extern void CreateSearchWindow (SearchGatherProc gather, CharPtr title, Uint2 entityID)

{
  ButtoN            b;
  GrouP             f;
  GrouP             g1, g2, g3;
  SearchExtraPtr    sep;
  WindoW            w;

  if (searchWindow == NULL) {
    sep = (SearchExtraPtr) MemNew (sizeof (SearchExtra));
    if (sep != NULL) {
      if (title == NULL || title [0] == '\0') {
        title = "String Search";
      }
      w = FixedWindow (-50, -33, -10, -10, title, CloseSearchWindow);
      if (w != NULL) {
        sep->gather = gather;
        sep->entityID = entityID;

        f = HiddenGroup (w, 4, 0, NULL);
        SetGroupSpacing (f, 10, 2);
        sep->searchfor = DialogText (f, "", 20, SearchTextProc);
        sep->find = DefaultButton (f, "Find", SearchFindProc);
        Disable (sep->find);

        g1 = NULL;
        g2 = NULL;
        g3 = NULL;
        /*
        g1 = NormalGroup (w, -1, 0, "Features", systemFont, NULL);
        SetGroupSpacing (g1, 3, 10);
        sep->features = HiddenGroup (g1, 3, 0, SearchFeatProc);
        RadioButton (sep->features, "All");
        RadioButton (sep->features, "Selected");
        RadioButton (sep->features, "None");
        SetValue (sep->features, 1);
        */
        /*
        sep->featurebuttons = HiddenGroup (g1, 5, 0, NULL);
        i = 1;
        curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
        while (curr != NULL && i < 86) {
          if (key != 0) {
            sep->featstouse [i] = CheckBox (sep->featurebuttons, curr->typelabel, NULL);
            i++;
          }
          curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
        }
        */
        /*
        sep->featurebuttons = HiddenGroup (g1, 2, 0, NULL);
        i = 1;
        fdgp = DispGroupFindNext (NULL,  &key, &label);
        while (fdgp != NULL && i < 86) {
          if (key != 0) {
            sep->featstouse [i] = CheckBox (sep->featurebuttons, fdgp->groupname, NULL);
            i++;
          }
          fdgp = DispGroupFindNext (fdgp,  &key, &label);
        }
        Disable (sep->featurebuttons);
        AlignObjects (ALIGN_CENTER, (HANDLE) sep->features,
                      (HANDLE) sep->featurebuttons, NULL);
        */

        /*
        g2 = NormalGroup (w, -1, 0, "Descriptors", systemFont, NULL);
        SetGroupSpacing (g2, 3, 10);
        sep->descriptors = HiddenGroup (g2, 3, 0, SearchDescProc);
        RadioButton (sep->descriptors, "All");
        RadioButton (sep->descriptors, "Selected");
        RadioButton (sep->descriptors, "None");
        SetValue (sep->descriptors, 1);
        sep->descriptorbuttons = HiddenGroup (g2, 4, 0, NULL);
        for (i = 1; i < 25; i++) {
          sep->descstouse [i] = CheckBox (sep->descriptorbuttons, descriptorNames [i], NULL);
        }
        Disable (sep->descriptorbuttons);
        AlignObjects (ALIGN_CENTER, (HANDLE) sep->descriptors,
                      (HANDLE) sep->descriptorbuttons, NULL);

        g3 = NormalGroup (w, -1, 0, "Publications", systemFont, NULL);
        SetGroupSpacing (g3, 3, 10);
        sep->pubs = HiddenGroup (g3, 3, 0, SearchPubProc);
        RadioButton (sep->pubs, "All");
        RadioButton (sep->pubs, "Selected");
        RadioButton (sep->pubs, "None");
        SetValue (sep->pubs, 1);
        sep->pubbuttons = HiddenGroup (g3, 5, 0, NULL);
        for (i = 1; i < 4; i++) {
          sep->pubstouse [i] = CheckBox (sep->pubbuttons, pubNames [i], NULL);
        }
        Disable (sep->pubbuttons);
        AlignObjects (ALIGN_CENTER, (HANDLE) sep->pubs,
                      (HANDLE) sep->pubbuttons, NULL);
        */

        b = NULL;
#ifdef WIN_MOTIF
        b = PushButton (w, "Dismiss", CloseSearchButton);
#endif

        AlignObjects (ALIGN_CENTER, (HANDLE) f, (HANDLE) b, (HANDLE) g1,
                      (HANDLE) g2, (HANDLE) g3, NULL);

        SetWindowExtra (w, sep, CleanupSearchProc);
        RealizeWindow (w);
        searchWindow = w;
      } else {
        MemFree (sep);
      }
    }
  }
}

extern void ShowSearchWindow (void)

{
  SearchExtraPtr  sep;

  if (searchWindow != NULL) {
    sep = GetWindowExtra (searchWindow);
    if (sep != NULL) {
      SetTitle (sep->searchfor, "");
      if (Enabled (sep->find)) {
        Disable (sep->find);
      }
    }
    if (! Visible (searchWindow)) {
      Show (searchWindow);
    }
    Select (searchWindow);
  }
}

static void DrawReplace (DoC d, RectPtr r, Int2 item, Int2 firstLine)

{
  RecT             rct;
  ReplaceExtraPtr  rep;

  rep = GetWindowExtra (ParentWindow (d));
  if (rep != NULL) {
    if (item == rep->selected) {
      rct = *r;
      rct.right = rct.left + 4;
      PaintRect (&rct);
    }
  }
}

static void ClickReplace (DoC d, PoinT pt)

{
  Int2             item;
  Int2             row;
  ReplaceExtraPtr  rep;

  rep = GetWindowExtra (ParentWindow (d));
  if (rep != NULL) {
    rep->clicked = 0;
    rep->dblClick = dblClick;
    MapDocPoint (d, pt, &item, &row, NULL, NULL);
    if (item > 0 && row > 0) {
      rep->clicked = item;
    }
  }
}

static void RepDoNotify (ReplaceExtraPtr rep, Int2 item, Boolean select)

{
  unsigned int  entityID;
  unsigned int  itemID;
  unsigned int  itemtype;
  CharPtr       str;
  unsigned int  subtype;

  if (rep != NULL && rep->doc != NULL && rep->notify != NULL) {
    str = GetDocText (rep->doc, item, 1, 2);
    if (str != NULL &&
        sscanf (str, "%u %u %u %u", &entityID, &itemID, &itemtype, &subtype) == 4) {
      (rep->notify) ((Uint2) entityID, (Uint2) itemID, (Uint2) itemtype,
                     (Uint2) subtype, select, rep->dblClick);
    }
    MemFree (str);
  }
}

static void ReleaseReplace (DoC d, PoinT pt)

{
  Int2             item;
  Int2             old;
  ReplaceExtraPtr  rep;
  Int2             row;

  rep = GetWindowExtra (ParentWindow (d));
  if (rep != NULL) {
    ResetClip ();
    MapDocPoint (d, pt, &item, &row, NULL, NULL);
    if (item > 0 && row > 0) {
      if (item == rep->clicked) {
        old = rep->selected;
        rep->selected = item;
        if (old != item) {
          if (old == 0) {
            InvalBorder (d, item);
          } else {
            InvalBorder (d, MIN (item, old));
            InvalBorder (d, MAX (item, old));
            RepDoNotify (rep, old, FALSE);
          }
          /*
          if (! Enabled (rep->remove)) {
            Enable (rep->remove);
          }
          */
          Update ();
        }
        RepDoNotify (rep, item, TRUE);
      }
    } else if (rep->clicked == 0) {
      if (rep->selected != 0) {
        old = rep->selected;
        rep->selected = 0;
        InvalBorder (d, old);
        RepDoNotify (rep, old, FALSE);
      }
      /*
      if (Enabled (rep->remove)) {
        Disable (rep->remove);
      }
      */
      Update ();
    }
  }
}

extern void FreeReplaceWindow (void)

{
  if (replaceWindow != NULL) {
    Remove (replaceWindow);
    replaceWindow = NULL;
  }
}

static void CloseReplaceWindow (WindoW w)

{
  FreeReplaceWindow ();
}

static void CloseReplaceButton (ButtoN b)

{
  FreeReplaceWindow ();
}

static void CleanupReplaceProc (WindoW w, VoidPtr data)

{
  ReplaceExtraPtr  rep;

  rep = (ReplaceExtraPtr) data;
  if (rep != NULL) {
  }
  MemFree (data);
}

extern void CreateReplaceWindow (ReplaceNotifyProc notify, CharPtr title,
                                 FonT font, Boolean verbose)

{
  ButtoN           b;
  GrouP            g;
  GrouP            h;
  ReplaceExtraPtr  rep;
  WindoW           w;

  if (replaceWindow == NULL) {
    rep = (ReplaceExtraPtr) MemNew (sizeof (ReplaceExtra));
    if (rep != NULL) {
      if (title == NULL || title [0] == '\0') {
        title = "String Replace";
      }
      w = FixedWindow (-50, -33, -10, -10, title, CloseReplaceWindow);
      if (w != NULL) {
        rep->notify = notify;

        g = HiddenGroup (w, 0, -5, NULL);
        h = HiddenGroup (g, 5, 0, NULL);
        SetGroupMargins (h, 11, 0);
        SetGroupSpacing (h, 0, 2);
        rep->doc = DocumentPanel (g, stdCharWidth * 40, stdLineHeight * 20);
        SetDocAutoAdjust (rep->doc, FALSE);
        SetDocProcs (rep->doc, ClickReplace, NULL, ReleaseReplace, NULL);
        SetDocShade (rep->doc, DrawReplace, NULL, NULL, NULL);
        rep->font = font;

        rep->clicked = 0;
        rep->selected = 0;
        rep->dblClick = FALSE;
        rep->verbose = verbose;

        repColFmt [0].pixWidth = stdCharWidth * 40;
        repColFmt [1].pixWidth = stdCharWidth * 40;

        b = NULL;
#ifdef WIN_MOTIF
        b = PushButton (w, "Dismiss", CloseReplaceButton);
#endif

        AlignObjects (ALIGN_CENTER, (HANDLE) rep->doc, (HANDLE) b, NULL);

        SetWindowExtra (w, rep, CleanupReplaceProc);
        RealizeWindow (w);
        replaceWindow = w;
      } else {
        MemFree (rep);
      }
    }
  }
}

extern void LIBCALLBACK AppendReplaceMessage (CharPtr searchFor, CharPtr foundIn, CharPtr label,
                                              Uint2 entityID, Uint2 itemID,
                                              Uint2 itemtype, Uint2 subtype)

{
  CharPtr          buf;
  size_t           len;
  Int2             numItems;
  CharPtr          ptr;
  ReplaceExtraPtr  rep;
  CharPtr          str;

  ShowReplaceWindow ();
  if (replaceWindow != NULL) {
    rep = GetWindowExtra (replaceWindow);
    if (rep != NULL) {
      if (label == NULL) {
        label = "";
      }
      if (foundIn == NULL) {
        foundIn = "";
      }
      len = StringLen (foundIn) + StringLen (label) + 50;
      buf = MemGet (len, MGET_CLEAR);
      if (buf != NULL) {
        if (rep->verbose) {
          str = foundIn;
        } else {
          str = label;
        }
        ptr = StringMoveAndConvertTabs (buf, str, "\t");
        sprintf (ptr, "%u %u %u %u\n", (unsigned int) entityID,
                 (unsigned int) itemID, (unsigned int) itemtype,
                 (unsigned int) subtype);
        AppendText (rep->doc, buf, &repParFmt, repColFmt, rep->font);
        GetDocParams (rep->doc, &numItems, NULL);
        UpdateDocument (rep->doc, numItems, numItems);
        MemFree (buf);
      }
      Select (replaceWindow);
    }
  }
}

extern void LIBCALLBACK StdReplaceNotify (Uint2 entityID, Uint2 itemID, Uint2 itemtype,
                                          Uint2 subtype, Boolean select, Boolean dblClick)

{
  Boolean  handled;

  if (dblClick) {
    WatchCursor ();
    handled = GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID,
                                itemtype, 0, 0, itemtype, 0);
    ArrowCursor ();
    if (handled == OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
      return;
    }
  }
  ObjMgrSelect (entityID, itemID, itemtype,0,NULL);
}

extern void ShowReplaceWindow (void)

{
  if (replaceWindow == NULL) {
    CreateReplaceWindow (NULL, "String Replace", SetSmallFont (), FALSE);
  }
  if (replaceWindow != NULL) {
    if (! Visible (replaceWindow)) {
      Show (replaceWindow);
    }
    Select (replaceWindow);
  }
}

