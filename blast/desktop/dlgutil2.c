/*   dlgutil2.c
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
* File Name:  dlgutil2.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.29 $
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

#include <dlogutil.h>
#include <document.h>
#include <gather.h>
#include <subutil.h>
#include <objfdef.h>
#include <gbfeat.h>
#include <gbftdef.h>
#include <utilpub.h>
#include <objfeat.h>
#include <objseq.h>
#include <toasn3.h>
#ifdef WIN_MOTIF
#include <netscape.h>
#endif

typedef struct datepage {
  DIALOG_MESSAGE_BLOCK
  TexT          year;
  PopuP         month;
  TexT          day;
} DatePage, PNTR DatePagePtr;

extern CharPtr SaveStringFromTextAndStripNewlines (TexT t)

{
  Char     ch;
  size_t   len;
  CharPtr  ptr;
  CharPtr  str;

  len = TextLength (t);
  if (len > 0) {
    str = MemNew (len + 1);
    if (str != NULL) {
      GetTitle (t, str, len + 1);
      ptr = str;
      ch = *ptr;
      while (ch != '\0') {
        if (ch < ' ') {
          *ptr = ' ';
        }
        ptr++;
        ch = *ptr;
      }
      TrimSpacesAroundString (str);
      if (StringHasNoText (str)) {
        str = MemFree (str);
      }
      return str;
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

static void DatePtrToDatePage (DialoG d, Pointer data)

{
  DatePtr      dp;
  DatePagePtr  dpp;

  dpp = (DatePagePtr) GetObjectExtra (d);
  dp = (DatePtr) data;
  if (dpp != NULL) {
    DatePtrToVibrant (dp, dpp->month, dpp->day, dpp->year);
  }
}

static Pointer DatePageToDatePtr (DialoG d)

{
  DatePtr      dp;
  DatePagePtr  dpp;

  dp = NULL;
  dpp = (DatePagePtr) GetObjectExtra (d);
  if (dpp != NULL) {
    dp = VibrantToDatePtr (dpp->month, dpp->day, dpp->year);
  }
  return (Pointer) dp;
}

extern DialoG CreateDateDialog (GrouP prnt, CharPtr title)

{
  DatePagePtr  dpp;
  GrouP        f;
  GrouP        m;
  GrouP        p;
  GrouP        s;

  p = HiddenGroup (prnt, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  dpp = (DatePagePtr) MemNew (sizeof (DatePage));
  if (dpp) {

    SetObjectExtra (p, dpp, StdCleanupExtraProc);
    dpp->dialog = (DialoG) p;
    dpp->todialog = DatePtrToDatePage;
    dpp->fromdialog = DatePageToDatePtr;
    dpp->testdialog = NULL;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    m = HiddenGroup (s, -1, 0, NULL);
    /*
    SetGroupSpacing (m, 10, 10);
    */

    f = HiddenGroup (m, -6, 0, NULL);
    StaticPrompt (f, "Month", 0, popupMenuHeight, programFont, 'l');
    dpp->month = PopupList (f, TRUE, NULL);
    InitEnumPopup (dpp->month, months_alist, NULL);
    SetValue (dpp->month, 1);
    StaticPrompt (f, "Day", 0, dialogTextHeight, programFont, 'l');
    dpp->day = DialogText (f, "", 4, NULL);
    StaticPrompt (f, "Year", 0, dialogTextHeight, programFont, 'l');
    dpp->year = DialogText (f, "", 6, NULL);
  }

  return (DialoG) p;
}

typedef struct featcit {
  DIALOG_MESSAGE_BLOCK
  DoC         citdoc;
  ValNodePtr  pubset;
} FeatCitPage, PNTR FeatCitPagePtr;

static ParData cofParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData cofColFmt = {0, 0, 0, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

static ParData cofeParFmt = {TRUE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData cofeColFmt = {0, 0, 0, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

static Uint1 diamondSym [] = {
  0x00, 0x10, 0x38, 0x7C, 0x38, 0x10, 0x00, 0x00
};

static void DrawCitOnFeat (DoC d, RectPtr r, Int2 item, Int2 firstLine)

{
  Int2  lineHeight;
  RecT  rct;

  if (d != NULL && r != NULL && item > 0 && firstLine == 0) {
    if (item == 1) return; /* citonfeattxt or citonfeathdr explanatory text */
    GetItemParams (d, item, NULL, NULL, NULL, &lineHeight, NULL);
    rct = *r;
    rct.left += 1;
    rct.right = rct.left + 7;
    rct.top += (lineHeight - 7) / 2;
    rct.bottom = rct.top + 7;
    CopyBits (&rct, diamondSym);
    /*
    x = r->left + 1;
    y = r->top + lineHeight / 2;
    MoveTo (x, y);
    LineTo (x + 5, y);
    */
  }
}

static int LIBCALLBACK CompareStrings (VoidPtr ptr1, VoidPtr ptr2)

{
  CharPtr  str1;
  CharPtr  str2;

  if (ptr1 != NULL && ptr2 != NULL) {
    str1 = *((CharPtr PNTR) ptr1);
    str2 = *((CharPtr PNTR) ptr2);
    if (str1 != NULL && str2 != NULL) {
      return StringICmp (str1, str2);
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

static CharPtr citonfeattxt =
"Press 'Edit Citations' to attach publications to this feature. \
Publications must first be added to the record. For biological \
justification, and not to credit the sequencer, create publication \
with the 'Cites a feature on the sequence' scope.\n";

static CharPtr citonfeathdr =
"Press 'Edit Citations' to change publications.\n\n";

static void PubsetPtrToFeatCitPage (DialoG d, Pointer data)

{
  Int2            count;
  FeatCitPagePtr  fpp;
  Int2            i;
  Char            label [128];
  ObjMgrPtr       omp;
  ObjMgrTypePtr   omtp;
  ValNodePtr      ppr;
  ValNodePtr      psp;
  RecT            r;
  CharPtr         PNTR strs;

  fpp = (FeatCitPagePtr) GetObjectExtra (d);
  psp = (ValNodePtr) data;
  if (fpp != NULL) {
    Reset (fpp->citdoc);
    fpp->pubset = PubSetFree (fpp->pubset);
    fpp->pubset = AsnIoMemCopy (data,
                                (AsnReadFunc) PubSetAsnRead,
                                (AsnWriteFunc) PubSetAsnWrite);
    SetDocAutoAdjust (fpp->citdoc, FALSE);
    ObjectRect (fpp->citdoc, &r);
    InsetRect (&r, 4, 4);
    cofColFmt.pixWidth = r.right - r.left;
    cofColFmt.pixInset = 10;
    omp = ObjMgrGet ();
    if (omp == NULL) return;
    omtp = ObjMgrTypeFind (omp, OBJ_SEQFEAT_CIT, NULL, NULL);
    if (omtp == NULL || omtp->labelfunc == NULL) return;
    if (psp != NULL && psp->data.ptrvalue != NULL) {
      count = 0;
      for (ppr = psp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
        count++;
      }
      if (count > 0) {
        strs = MemNew (sizeof (CharPtr) * (size_t) (count + 1));
        if (strs != NULL) {
          i = 0;
          for (ppr = psp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
            (*(omtp->labelfunc)) (ppr, label, 127, OM_LABEL_CONTENT);
            strs [i] = StringSave (label);
            i++;
          }
          HeapSort (strs, count, sizeof (CharPtr), CompareStrings);
          AppendText (fpp->citdoc, citonfeathdr, &cofParFmt, NULL, programFont);
          for (i = 0; i < count; i++) {
            AppendText (fpp->citdoc, strs [i],
                        &cofParFmt, &cofColFmt, programFont);
          }
          for (i = 0; i < count; i++) {
            strs [i] = MemFree (strs [i]);
          }
          MemFree (strs);
        } else {
          AppendText (fpp->citdoc, citonfeattxt, &cofParFmt, NULL, programFont);
        }
      }
    } else {
      AppendText (fpp->citdoc, citonfeattxt, &cofParFmt, NULL, programFont);
    }
    SetDocShade (fpp->citdoc, DrawCitOnFeat, NULL, NULL, NULL);
    UpdateDocument (fpp->citdoc, 0, 0);
  }
}

static Pointer FeatCitPageToPubsetPtr (DialoG d)

{
  FeatCitPagePtr  fpp;
  ValNodePtr      psp;

  psp = NULL;
  fpp = (FeatCitPagePtr) GetObjectExtra (d);
  if (fpp != NULL) {
    psp = AsnIoMemCopy (fpp->pubset,
                        (AsnReadFunc) PubSetAsnRead,
                        (AsnWriteFunc) PubSetAsnWrite);
  }
  return (Pointer) psp;
}

static void CleanupCitOnFeatProc (GraphiC g, VoidPtr data)

{
  FeatCitPagePtr  fpp;

  fpp = (FeatCitPagePtr) data;
  if (fpp != NULL) {
    PubSetFree (fpp->pubset);
  }
  MemFree (data);
}

static DialoG CreateCitOnFeatDialog (GrouP h, CharPtr title)

{
  FeatCitPagePtr  fpp;
  Int2            lineHeight;
  GrouP           m;
  GrouP           p;
  GrouP           s;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  fpp = (FeatCitPagePtr) MemNew (sizeof (FeatCitPage));
  if (fpp != NULL) {

    SetObjectExtra (p, fpp, CleanupCitOnFeatProc);
    fpp->dialog = (DialoG) p;
    fpp->todialog = PubsetPtrToFeatCitPage;
    fpp->fromdialog = FeatCitPageToPubsetPtr;
    fpp->testdialog = NULL;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    m = HiddenGroup (s, -1, 0, NULL);
    /*
    SetGroupSpacing (m, 10, 10);
    */

    SelectFont (programFont);
    lineHeight = LineHeight ();
    SelectFont (systemFont);
    cofParFmt.minHeight = lineHeight + 2;
    StaticPrompt (m, "Citations on Feature", 25 * stdCharWidth, 0, programFont, 'c');
    fpp->citdoc = DocumentPanel (m, 25 * stdCharWidth, 5 * cofParFmt.minHeight);
    SetObjectExtra (fpp->citdoc, fpp, NULL);
    SetDocAutoAdjust (fpp->citdoc, FALSE);
    fpp->pubset = NULL;
  }

  return (DialoG) p;
}

typedef struct citlist {
  Uint2    entityID;
  Uint2    itemID;
  Uint2    itemtype;
  CharPtr  label;
} CitListData, PNTR CitListDataPtr;

typedef struct featcitedit {
  FeatureFormPtr  ffp;
  DoC             allcitdoc;
  Int2            clickedItem;
  Int2            clickedRow;
  Int2            numitems;
  Int2            lineheight;
  Int2            index;
  BoolPtr         chosen;
  ValNodePtr      citlist;
  ValNodePtr      psp;
  ObjMgrPtr       omp;
} FeatCitEdit, PNTR FeatCitEditPtr;

static void CleanupFeatCitForm (GraphiC g, VoidPtr data)

{
  FeatCitEditPtr  fcep;

  fcep = (FeatCitEditPtr) data;
  if (fcep != NULL) {
    MemFree (fcep->chosen);
  }
  MemFree (data);
}

static void DrawFeatCit (DoC d, RectPtr r, Int2 item, Int2 firstLine)

{
  FeatCitEditPtr  fcep;
  RecT            rct;

  fcep = (FeatCitEditPtr) GetObjectExtra (d);
  if (fcep != NULL && r != NULL && item > 0 && firstLine == 0) {
    rct = *r;
    rct.left += 1;
    rct.right = rct.left + fcep->lineheight;
    rct.bottom = rct.top + (rct.right - rct.left);
    FrameRect (&rct);
    if (item > 0 && item <= fcep->numitems) {
      if (fcep->chosen != NULL && fcep->chosen [item - 1]) {
        MoveTo (rct.left, rct.top);
        LineTo (rct.right - 1, rct.bottom - 1);
        MoveTo (rct.left, rct.bottom - 1);
        LineTo (rct.right - 1, rct.top);
      }
    }
  }
}

static void ClickFeatCit (DoC d, PoinT pt)

{
}

static void ReleaseFeatCit (DoC d, PoinT pt)

{
  Int2            col;
  FeatCitEditPtr  fcep;
  Int2            item;
  RecT            rct;
  Int2            row;

  fcep = (FeatCitEditPtr) GetObjectExtra (d);
  if (fcep != NULL && fcep->chosen != NULL) {
    MapDocPoint (d, pt, &item, &row, &col, &rct);
    rct.left += 1;
    rct.right = rct.left + fcep->lineheight;
    rct.bottom = rct.top + (rct.right - rct.left);
    if (row == 1 && col == 1 && item > 0 && item <= fcep->numitems && PtInRect (pt, &rct)) {
      if (fcep->chosen [item - 1]) {
        fcep->chosen [item - 1] = FALSE;
      } else {
        fcep->chosen [item - 1] = TRUE;
      }
      InsetRect (&rct, -1, -1);
      InvalRect (&rct);
      Update ();
    }
  }
}

static Boolean GatherAllCits (GatherContextPtr gcp)

{
  CitListDataPtr  cldp;
  FeatCitEditPtr  fcep;
  Char            label [128];
  ObjMgrTypePtr   omtp;
  PubdescPtr      pdp;
  ValNodePtr      sdp;
  SeqFeatPtr      sfp;
  ValNodePtr      vnp;

  if (gcp == NULL) return TRUE;
  fcep = (FeatCitEditPtr) gcp->userdata;
  if (fcep == NULL) return TRUE;
  label [0] = '\0';
  pdp = NULL;
  if (gcp->thistype == OBJ_SEQDESC) {
    sdp = (ValNodePtr) gcp->thisitem;
    if (sdp == NULL || sdp->choice != Seq_descr_pub) return TRUE;
    pdp = sdp->data.ptrvalue;
  } else if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp == NULL || sfp->data.choice != SEQFEAT_PUB) return TRUE;
    pdp = sfp->data.value.ptrvalue;
  } else return TRUE;
  if (pdp == NULL) return TRUE;
  omtp = ObjMgrTypeFind (fcep->omp, gcp->thistype, NULL, NULL);
  if (omtp == NULL || omtp->labelfunc == NULL) return TRUE;
  (*(omtp->labelfunc)) (gcp->thisitem, label, 127, OM_LABEL_CONTENT);
  cldp = (CitListDataPtr) MemNew (sizeof (CitListData));
  if (cldp != NULL) {
    vnp = ValNodeNew (fcep->citlist);
    if (fcep->citlist == NULL) {
      fcep->citlist = vnp;
    }
    if (vnp != NULL) {
      vnp->data.ptrvalue = cldp;
      cldp->entityID = gcp->entityID;
      cldp->itemID = gcp->itemID;
      cldp->itemtype = gcp->thistype;
      cldp->label = StringSave (label);
      (fcep->numitems)++;
    } else {
      MemFree (cldp);
      return TRUE;
    }
  }
  return TRUE;
}

static Boolean PrintCitOnFeatItem (GatherContextPtr gcp)

{
  CharPtr  PNTR  rsultp;
  ValNodePtr     sdp;
  SeqFeatPtr     sfp;
  Boolean        success;

  rsultp = (CharPtr PNTR) gcp->userdata;
  if (rsultp != NULL) {
    success = FALSE;
    switch (gcp->thistype) {
      case OBJ_SEQDESC :
        sdp = (ValNodePtr) gcp->thisitem;
        if (sdp->data.ptrvalue != NULL) {
          success = StdFormatPrint ((Pointer) sdp, (AsnWriteFunc) SeqDescAsnWrite,
                                    "StdSeqDesc", spop);
        } else {
          *rsultp = StringSave ("Empty Descriptor\n");
          success = TRUE;
        }
        break;
      case OBJ_SEQFEAT :
        sfp = (SeqFeatPtr) gcp->thisitem;
        if (sfp != NULL && (sfp->data.choice == 10 || sfp->data.value.ptrvalue != NULL)) {
          success = StdFormatPrint ((Pointer) sfp, (AsnWriteFunc) SeqFeatAsnWrite,
                                    "StdSeqFeat", spop);
        } else {
          *rsultp = StringSave ("Empty Feature\n");
          success = TRUE;
        }
        break;
      default :
        break;
    }
    if (success) {
      if (spop->ptr != NULL && *((CharPtr) (spop->ptr)) != '\0') {
        *rsultp = spop->ptr;
        spop->ptr = NULL;
      } else {
        *rsultp = StringSave ("Empty Data\n");
      }
    } else {
      *rsultp = StringSave ("Data Failure\n");
    }
  }
  return TRUE;
}

static CharPtr CitOnFeatPrintProc (DoC doc, Int2 item, Pointer data)

{
  unsigned int  entityID;
  unsigned int  itemID;
  unsigned int  itemtype;
  CharPtr       rsult;
  CharPtr       str;

  rsult = NULL;
  if (data != NULL) {
    str = (CharPtr) data;
    if (sscanf (str, "%u %u %u", &entityID, &itemID, &itemtype) == 3) {
      GatherItem ((Uint2) entityID, (Uint2) itemID, (Uint2) itemtype,
                  (Pointer) &rsult, PrintCitOnFeatItem);
    }
  } else {
    rsult = StringSave ("Null Data\n");
  }
  return rsult;
}

static int LIBCALLBACK CompareCitList (VoidPtr ptr1, VoidPtr ptr2)

{
  CitListDataPtr  cldp1;
  CitListDataPtr  cldp2;
  CharPtr         str1;
  CharPtr         str2;

  if (ptr1 != NULL && ptr2 != NULL) {
    cldp1 = (CitListDataPtr) ptr1;
    cldp2 = (CitListDataPtr) ptr2;
    if (cldp1 != NULL && cldp2 != NULL) {
      str1 = cldp1->label;
      str2 = cldp2->label;
      if (str1 != NULL && str2 != NULL) {
        return StringICmp (str1, str2);
      } else {
        return 0;
      }
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

static Boolean MakeMinimalCitOnFeatItem (GatherContextPtr gcp)

{
  PubdescPtr  pdp;
  ValNodePtr  ppr;
  ValNodePtr  sdp;
  SeqFeatPtr  sfp;
  ValNodePtr  vnp;
  ValNodePtr  PNTR  vnpp;

  vnpp = (ValNodePtr PNTR) gcp->userdata;
  if (vnpp != NULL) {
    pdp = NULL;
    switch (gcp->thistype) {
      case OBJ_SEQDESC :
        sdp = (ValNodePtr) gcp->thisitem;
        if (sdp->data.ptrvalue != NULL) {
          pdp = (PubdescPtr) sdp->data.ptrvalue;
        }
        break;
      case OBJ_SEQFEAT :
        sfp = (SeqFeatPtr) gcp->thisitem;
        if (sfp != NULL && (sfp->data.choice == 10 || sfp->data.value.ptrvalue != NULL)) {
          pdp = (PubdescPtr) sfp->data.value.ptrvalue;
        }
        break;
      default :
        break;
    }
    if (pdp != NULL) {
      ppr = NULL;
      vnp = ValNodeNew (NULL);
      if (vnp != NULL) {
        vnp->choice = PUB_Equiv;
        vnp->data.ptrvalue = pdp->pub;
        ppr = MinimizePub (vnp);
        ValNodeFree (vnp);
      }
      vnp = ValNodeNew (*vnpp);
      if (*vnpp == NULL) {
        *vnpp = vnp;
      }
      if (vnp != NULL) {
        vnp->choice = PUB_Equiv;
        vnp->data.ptrvalue = ppr;
      }
    }
  }
  return TRUE;
}

static void AcceptFeatCit (ButtoN b)

{
  Pointer         data;
  unsigned int    entityID;
  FeatCitEditPtr  fcep;
  FeatureFormPtr  ffp;
  Int2            i;
  unsigned int    itemID;
  unsigned int    itemtype;
  Int2            numItems;
  ValNodePtr      ppr;
  ValNodePtr      psp;
  CharPtr         str;

  fcep = (FeatCitEditPtr) GetObjectExtra (b);
  if (fcep != NULL && fcep->chosen != NULL) {
    psp = NULL;
    ppr = NULL;
    GetDocParams (fcep->allcitdoc, &numItems, NULL);
    for (i = 1; i <= numItems; i++) {
      if (fcep->chosen [i - 1]) {
        GetItemParams (fcep->allcitdoc, i, NULL, NULL, NULL, NULL, &data);
        if (data != NULL) {
          str = (CharPtr) data;
          if (sscanf (str, "%u %u %u", &entityID, &itemID, &itemtype) == 3) {
            GatherItem ((Uint2) entityID, (Uint2) itemID, (Uint2) itemtype,
                        (Pointer) &ppr, MakeMinimalCitOnFeatItem);
          }
        }
      }
    }
    ffp = (FeatureFormPtr) fcep->ffp;
    if (ffp != NULL) {
      if (ppr != NULL) {
        psp = ValNodeNew (NULL);
        if (psp != NULL) {
          psp->choice = 1;
          psp->data.ptrvalue = ppr;
          PointerToDialog (ffp->featcits, (Pointer) psp);
        }
        PubSetFree (psp);
      } else {
        PointerToDialog (ffp->featcits, NULL);
      }
    }
  }
  Remove (ParentWindow (b));
}

static Boolean MatchMinimalCits (GatherContextPtr gcp)

{
  FeatCitEditPtr  fcep;
  PubdescPtr      pdp;
  ValNodePtr      ppr;
  ValNodePtr      sdp;
  SeqFeatPtr      sfp;
  ValNodePtr      vnp;

  fcep = (FeatCitEditPtr) gcp->userdata;
  if (fcep != NULL) {
    pdp = NULL;
    switch (gcp->thistype) {
      case OBJ_SEQDESC :
        sdp = (ValNodePtr) gcp->thisitem;
        if (sdp->data.ptrvalue != NULL) {
          pdp = (PubdescPtr) sdp->data.ptrvalue;
        }
        break;
      case OBJ_SEQFEAT :
        sfp = (SeqFeatPtr) gcp->thisitem;
        if (sfp != NULL && (sfp->data.choice == 10 || sfp->data.value.ptrvalue != NULL)) {
          pdp = (PubdescPtr) sfp->data.value.ptrvalue;
        }
        break;
      default :
        break;
    }
    if (pdp != NULL && fcep->psp != NULL) {
      vnp = ValNodeNew (NULL);
      if (vnp != NULL) {
        vnp->choice = PUB_Equiv;
        vnp->data.ptrvalue = pdp->pub;
        for (ppr = fcep->psp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
          if (PubLabelMatch (vnp, ppr) == 0) {
            fcep->chosen [fcep->index] = TRUE;
          }
        }
        ValNodeFree (vnp);
      }
    }
  }
  return TRUE;
}

static void EditFeatCitsProc (ButtoN b)

{
  ButtoN          btn;
  GrouP           c;
  CitListDataPtr  cldp;
  CitListDataPtr  cldpp;
  Int2            count;
  FeatCitEditPtr  fcep;
  FeatureFormPtr  ffp;
  GatherScope     gs;
  Int2            i;
  Int2            j;
  Char            last [128];
  CharPtr         ptr;
  RecT            r;
  Char            str [34];
  ValNodePtr      vnp;
  WindoW          w;

  ffp = (FeatureFormPtr) GetObjectExtra (b);
  if (ffp != NULL) {
    fcep = (FeatCitEditPtr) MemNew (sizeof (FeatCitEdit));
    if (fcep != NULL) {
      fcep->ffp = ffp;
      fcep->omp = ObjMgrGet ();
      fcep->numitems = 0;
      WatchCursor ();
      Update ();
      w = MovableModalWindow (-50, -33, -10, -10, "Citations", NULL);
      SetObjectExtra (w, fcep, CleanupFeatCitForm);
      fcep->allcitdoc = DocumentPanel (w, 25 * stdCharWidth, 15 * stdLineHeight);
      SetObjectExtra (fcep->allcitdoc, fcep, NULL);
      SetDocProcs (fcep->allcitdoc, ClickFeatCit, NULL, ReleaseFeatCit, NULL);
      SetDocShade (fcep->allcitdoc, DrawFeatCit, NULL, NULL, NULL);
      c = HiddenGroup (w, 4, 0, NULL);
      SetGroupSpacing (c, 10, 3);
      btn = PushButton (c, "Accept", AcceptFeatCit);
      SetObjectExtra (btn, fcep, NULL);
      PushButton (c, "Cancel", StdCancelButtonProc);
      AlignObjects (ALIGN_CENTER, (HANDLE) fcep->allcitdoc, (HANDLE) c, NULL);
      RealizeWindow (w);
      SelectFont (programFont);
      fcep->lineheight = LineHeight ();
      SelectFont (systemFont);
      MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
      MemSet ((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
      gs.ignore [OBJ_SEQDESC] = FALSE;
      gs.ignore [OBJ_SEQANNOT] = FALSE;
      gs.ignore [OBJ_SEQFEAT] = FALSE;
      gs.seglevels = 1;
      GatherEntity (ffp->input_entityID, (Pointer) fcep, GatherAllCits, &gs);
      count = fcep->numitems;
      cldpp = (CitListDataPtr) MemNew (sizeof (CitListData) * (size_t) (count + 1));
      if (cldpp != NULL) {
        for (i = 0, vnp = fcep->citlist; i < count && vnp != NULL; i++, vnp = vnp->next) {
          cldp = vnp->data.ptrvalue;
          if (cldp != NULL) {
            cldpp [i] = *cldp;
            cldp->label = NULL;
          }
        }
        fcep->citlist = ValNodeFreeData (fcep->citlist);
        HeapSort (cldpp, count, sizeof (CitListData), CompareCitList);
        last [0] = '\0';
        ObjectRect (fcep->allcitdoc, &r);
        InsetRect (&r, 4, 4);
        cofeColFmt.pixWidth = r.right - r.left;
        cofeColFmt.pixInset = 20;
        fcep->numitems = 0;
        fcep->chosen = MemNew (sizeof (Boolean) * (size_t) (count + 1));
        fcep->psp = DialogToPointer (ffp->featcits);
        for (i = 0, j = 0; i < count; i++) {
          cldp = &(cldpp [i]);
          if (cldp != NULL) {
            if (last == NULL || StringCmp (cldp->label, last) != 0) {
              sprintf (str, "%d %d %d", (int) cldp->entityID,
                       (int) cldp->itemID, (int) cldp->itemtype);
              ptr = StringSave (str);
              (fcep->numitems)++;
              AppendItem (fcep->allcitdoc, CitOnFeatPrintProc, (Pointer) ptr,
                          TRUE, 5, &cofeParFmt, &cofeColFmt, programFont);
              if (fcep->chosen != NULL) {
                fcep->index = j;
                GatherItem ((Uint2) cldp->entityID, (Uint2) cldp->itemID,
                            (Uint2) cldp->itemtype, (Pointer) fcep, MatchMinimalCits);
              }
              j++;
            }
            StringNCpy_0 (last, cldp->label, sizeof (last));
            cldpp [i].label = MemFree (cldpp [i].label);
          }
        }
        PubSetFree (fcep->psp);
      }
      MemFree (cldpp);
      Show (w);
      Select (w);
      ArrowCursor ();
      Update ();
    }
  }
}

static void ChangeGenePopupOrList (Handle gene)

{
  FeatureFormPtr  ffp;
  Int2            val;

  ffp = (FeatureFormPtr) GetObjectExtra (gene);
  if (ffp != NULL) {
    val = GetValue (ffp->gene);
    if (val == 1) {
      SafeHide (ffp->newGeneGrp);
      SafeHide (ffp->editGeneBtn);
    } else if (val == 2) {
      SafeHide (ffp->editGeneBtn);
      SafeShow (ffp->newGeneGrp);
    } else {
      SafeHide (ffp->newGeneGrp);
      SafeShow (ffp->editGeneBtn);
    }
  }
}

static void ChangeSubGroup (VoidPtr data, Int2 newval, Int2 oldval)

{
  FeatureFormPtr  ffp;

  ffp = (FeatureFormPtr) data;
  if (ffp != NULL) {
    if (ffp->commonPage >= 0 && ffp->commonPage <= 4) {
      SafeHide (ffp->commonSubGrp [ffp->commonPage]);
    }
    ffp->commonPage = newval;
    if (ffp->commonPage >= 0 && ffp->commonPage <= 4) {
      SafeShow (ffp->commonSubGrp [ffp->commonPage]);
    }
  }
}

typedef struct fieldpage {
  DIALOG_MESSAGE_BLOCK
  Int2               numfields;
  CharPtr            PNTR fields;
  TexT               PNTR values;
  ButtoN             PNTR boxes;
} FieldPage, PNTR FieldPagePtr;

static Boolean ShouldBeAGBQual (Int2 qual, Boolean allowProductGBQual)

{
  if (qual < 0) return FALSE;
  if (allowProductGBQual && qual == GBQUAL_product) return TRUE;
  if (qual == GBQUAL_citation || qual == GBQUAL_db_xref || qual == GBQUAL_evidence ||
      qual == GBQUAL_exception || qual == GBQUAL_gene || qual == GBQUAL_label ||
      qual == GBQUAL_map || qual == GBQUAL_note || qual == GBQUAL_partial ||
      qual == GBQUAL_product || qual == GBQUAL_pseudo) {
    return FALSE;
  }
  if (Nlm_GetAppProperty ("SequinUseEMBLFeatures") == NULL) {
    if (qual == GBQUAL_usedin) {
      return FALSE;
    }
  }
  return TRUE;
}

static void GBQualPtrToFieldPage (DialoG d, Pointer data)

{
  Char          empty [4];
  FieldPagePtr  fpf;
  Int2          i;
  GBQualPtr     list;

  fpf = (FieldPagePtr) GetObjectExtra (d);
  list = (GBQualPtr) data;
  if (fpf != NULL) {
    while (list != NULL) {
      if (list->qual != NULL && list->val != NULL) {
        for (i = 0; i < fpf->numfields; i++) {
          if (StringICmp (list->qual, fpf->fields [i]) == 0) {
            if (fpf->values [i] != NULL) {
              if (*(list->val) == '\0') {
                empty [0] = '"';
                empty [1] = '"';
                empty [2] = '\0';
                SetTitle (fpf->values [i], empty);
              } else {
                SetTitle (fpf->values [i], list->val);
              }
            } else if (fpf->boxes [i] != NULL) {
              SetStatus (fpf->boxes [i], TRUE);
            }
          }
        }
      }
      list = list->next;
    }
  }
}

extern void SeqFeatPtrToFieldPage (DialoG d, SeqFeatPtr sfp);
extern void SeqFeatPtrToFieldPage (DialoG d, SeqFeatPtr sfp)

{
  /*
  FieldPagePtr  fpf;
  Int2          i;

  fpf = (FieldPagePtr) GetObjectExtra (d);
  if (fpf != NULL && sfp != NULL) {
        for (i = 0; i < fpf->numfields; i++) {
          if (StringICmp ("exception", fpf->fields [i]) == 0) {
            if (fpf->values [i] != NULL) {
              if (sfp->except_text == NULL || *(sfp->except_text) == '\0') {
                SetTitle (fpf->values [i], "");
              } else {
                SetTitle (fpf->values [i], sfp->except_text);
              }
            }
          } else if (StringICmp ("pseudo", fpf->fields [i]) == 0) {
            if (fpf->boxes [i] != NULL && (! GetStatus (fpf->boxes [i]))) {
              SetStatus (fpf->boxes [i], sfp->pseudo);
            }
          }
        }
  }
  */

}

static Pointer FieldPageToGBQualPtr (DialoG d)

{
  FieldPagePtr  fpf;
  GBQualPtr     gbq;
  GBQualPtr     gbqlast;
  GBQualPtr     head;
  Int2          i;

  head = NULL;
  fpf = (FieldPagePtr) GetObjectExtra (d);
  if (fpf != NULL) {
    gbq = NULL;
    gbqlast = NULL;
    for (i = 0; i < fpf->numfields; i++) {
      if (fpf->fields [i] == NULL ||
          StringHasNoText (fpf->fields [i]) ||
          (fpf->values [i] == NULL && fpf->boxes [i] ==NULL) ||
          (fpf->values [i] != NULL && TextHasNoText (fpf->values [i]))) {
      } else if (fpf->boxes [i] != NULL && (! GetStatus (fpf->boxes [i]))) {
      } else {
        gbq = GBQualNew ();
        if (gbqlast == NULL) {
          head = gbq;
        } else {
          gbqlast->next = gbq;
        }
        gbqlast = gbq;
        if (gbq != NULL) {
          gbq->qual = StringSave (fpf->fields [i]);
          if (fpf->values [i] != NULL) {
            gbq->val = SaveStringFromText (fpf->values [i]);
          } else {
            gbq->val = StringSave ("");
          }
        }
      }
    }
  }
  return (Pointer) head;
}

static void CleanupFieldsPage (GraphiC g, VoidPtr data)

{
  FieldPagePtr  fpf;
  Int2          i;

  fpf = (FieldPagePtr) data;
  if (fpf != NULL) {
    if (fpf->fields != NULL) {
      for (i = 0; i < fpf->numfields; i++) {
        MemFree (fpf->fields [i]);
      }
    }
    MemFree (fpf->fields);
    MemFree (fpf->values);
    MemFree (fpf->boxes);
  }
  MemFree (data);
}

#define LEGAL_FEATURE    1
#define ILLEGAL_FEATURE  2

extern DialoG CreateImportFields (GrouP h, CharPtr name, SeqFeatPtr sfp, Boolean allowProductGBQual)

{
  FieldPagePtr    fpf;
  GrouP           g;
  GBQualPtr       gbq;
  Boolean         hasillegal;
  Int2            i;
  Int2            index;
  Int2            j;
  Int2            max;
  Int2            num;
  GrouP           p;
  PrompT          ppt;
  Int2            qual;
  Int2            seen [ParFlat_TOTAL_GBQUAL];
  SematicFeatPtr  sefp;
  Char            str [64];
  Int2            wid;

  p = HiddenGroup (h, 1, 0, NULL);
  fpf = (FieldPagePtr) MemNew (sizeof (FieldPage));
  if (fpf != NULL) {

    SetObjectExtra (p, fpf, CleanupFieldsPage);
    fpf->dialog = (DialoG) p;
    fpf->todialog = GBQualPtrToFieldPage;
    fpf->fromdialog = FieldPageToGBQualPtr;
    fpf->testdialog = NULL;

    gbq = NULL;
    if (sfp != NULL) {
      gbq = sfp->qual;
    }

    j = 0;
    hasillegal = FALSE;
    for (i = 0; i < ParFlat_TOTAL_GBQUAL; i++) {
      seen [i] = 0;
    }
    num = 0;

    if (name != NULL) {
      index = GBFeatKeyNameValid (&name, FALSE);
      if (index >= 0) {

        sefp = &(ParFlat_GBFeat [index]);

        for (i = 0; i < sefp->mand_num; i++) {
          qual = sefp->mand_qual [i];
          if (qual > -1 && ShouldBeAGBQual (qual, allowProductGBQual)) {
            seen [qual] = LEGAL_FEATURE;
          }
        }
        for (i = 0; i < sefp->opt_num; i++) {
          qual = sefp->opt_qual [i];
          if (qual > -1 && ShouldBeAGBQual (qual, allowProductGBQual)) {
            seen [qual] = LEGAL_FEATURE;
          }
        }
      }
    } else if (sfp != NULL && sfp->data.choice == SEQFEAT_CDREGION) {
      /*
      seen [GBQUAL_exception] = LEGAL_FEATURE;
      seen [GBQUAL_pseudo] = LEGAL_FEATURE;
      */
    }

    while (gbq != NULL) {
      qual = GBQualNameValid (gbq->qual);
      if (qual > -1) {
        if (seen [qual] == 0) {
          seen [qual] = ILLEGAL_FEATURE;
          hasillegal = TRUE;
        }
      }
      gbq = gbq->next;
    }

    SelectFont (programFont);
    max = 0;
    for (i = 0; i < ParFlat_TOTAL_GBQUAL; i++) {
      if (seen [i] > 0) {
        num++;
        if (seen [i] == LEGAL_FEATURE) {
          StringNCpy_0 (str, ParFlat_GBQual_names [i].name, sizeof (str));
          wid = StringWidth (str) + 2;
        } else {
          str [0] = '\0';
          if (name != NULL) {
            StringCpy (str, "*");
          }
          StringNCat (str, ParFlat_GBQual_names [i].name, sizeof (str) - 2);
          wid = StringWidth (str) + 2;
        }
        if (wid > max) {
          max = wid;
        }
      }
    }
    SelectFont (systemFont);

    fpf->numfields = num;
    fpf->fields = MemNew (sizeof (CharPtr) * (num + 1));
    fpf->values = MemNew (sizeof (TexT) * (num + 1));
    fpf->boxes = MemNew (sizeof (ButtoN) * (num + 1));

    g = HiddenGroup (p, 2, 0, NULL);
    j = 0;
    for (i = 0; i < ParFlat_TOTAL_GBQUAL; i++) {
      if (seen [i] == LEGAL_FEATURE) {
        fpf->fields [j] = StringSave (ParFlat_GBQual_names [i].name);
        ppt = StaticPrompt (g, fpf->fields [j], max, dialogTextHeight, programFont, 'l');
        if (ParFlat_GBQual_names [i].gbclass != Class_none) {
          fpf->values [j] = DialogText (g, "", 20, NULL);
        } else {
          fpf->boxes [j] = CheckBox (g, "", NULL);
          AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt, (HANDLE) fpf->boxes [j], NULL);
        }
        j++;
      }
    }
    if (hasillegal && name != NULL) {
      StaticPrompt (p, "Illegal Qualifiers", 0, 0, programFont, 'c');
    }
    g = HiddenGroup (p, 2, 0, NULL);
    for (i = 0; i < ParFlat_TOTAL_GBQUAL; i++) {
      if (seen [i] == ILLEGAL_FEATURE) {
        fpf->fields [j] = StringSave (ParFlat_GBQual_names [i].name);
        str [0] = '\0';
        if (name != NULL) {
          StringCpy (str, "*");
        }
        StringNCat (str, fpf->fields [j], sizeof (str) - 2);
        ppt = StaticPrompt (g, str, max, dialogTextHeight, programFont, 'l');
        if (ParFlat_GBQual_names [i].gbclass != Class_none) {
          fpf->values [j] = DialogText (g, "", 20, NULL);
        } else {
          fpf->boxes [j] = CheckBox (g, "", NULL);
          AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt, (HANDLE) fpf->boxes [j], NULL);
        }
        j++;
      }
    }

    if (j == 0) {
      StaticPrompt (p, "See Attributes page to set legal qualifiers for this feature.",
                    0, 0, programFont, 'c');
    }
  }
  return (DialoG) p;
}

static void ChangeCannedMessage (PopuP p)

{
  FeatureFormPtr  ffp;
  Int2            val;

  ffp = (FeatureFormPtr) GetObjectExtra (p);
  if (ffp == NULL) return;
  val = GetValue (p);
  switch (val) {
    case 1 :
      if (Message (MSG_YN, "Clear the explanation field?") == ANS_YES) {
        SetTitle (ffp->exceptText, "");
        if (Message (MSG_YN, "Clear the exception flag?") == ANS_YES) {
          SetStatus (ffp->exception, FALSE);
        }
      }
      break;
    case 2 :
      SetTitle (ffp->exceptText, "RNA editing");
      SetStatus (ffp->exception, TRUE);
      break;
    case 3 :
      SetTitle (ffp->exceptText, "reasons given in citation");
      SetStatus (ffp->exception, TRUE);
      break;
    case 4 :
      SetTitle (ffp->exceptText, "ribosomal slippage");
      SetStatus (ffp->exception, TRUE);
      break;
    case 5 :
      SetTitle (ffp->exceptText, "trans splicing");
      SetStatus (ffp->exception, TRUE);
      break;
    case 6 :
      SetTitle (ffp->exceptText, "artificial frameshift");
      SetStatus (ffp->exception, TRUE);
    default :
      break;
  }
}

static CharPtr crossRefWarn =
"A gene mapped by cross-reference does not extend the range\n\
of the indicated gene feature.  Overlap is the usual case, and\n\
it does extend the selected gene.";

static CharPtr suppressWarn =
"This will suppress display of a gene qualifier even though\n\
there is a gene feature that overlaps this one.";

static void GeneXrefWarn (GrouP g)

{
  Int2  val;

  val = GetValue (g);
  if (val == 2) {
    Message (MSG_OK, "%s", crossRefWarn);
  } else if (val == 3) {
    Message (MSG_OK, "%s", suppressWarn);
  }
}

static CharPtr  commonRadioFormTabs [] = {
  "General", "Comment", "Citations", "Cross-Refs", NULL, NULL
};

static CharPtr  commonNoCitFormTabs [] = {
  "General", "Comment", "Cross-Refs", NULL, NULL
};

extern void Nlm_LaunchGeneFeatEd (ButtoN b);
extern GrouP CreateCommonFeatureGroupEx (GrouP h, FeatureFormPtr ffp,
                                         SeqFeatPtr sfp, Boolean hasGeneControl,
                                         Boolean hasCitationTab, Boolean hasGeneSuppress)

{
  ButtoN   b;
  GrouP    c;
  PopuP    canned;
  Boolean  cdsQuals;
  GrouP    f;
  GrouP    g;
  Boolean  hasQuals;
  Boolean  indexerVersion;
  Char     just;
  GrouP    k;
  GrouP    m;
  GrouP    p;
  Int2     page;
  ButtoN   pseudo = NULL;
  GrouP    q;
  GrouP    r;
  GrouP    t;
  GrouP    v;
  GrouP    x;
  GrouP    y;

  c = NULL;
  if (ffp != NULL) {
    hasQuals = FALSE;
    cdsQuals = FALSE;
    if (ffp->gbquals == NULL && sfp != NULL && sfp->qual != NULL) {
      hasQuals = TRUE;
      /*
      if (GetAppProperty ("InternalNcbiSequin") != NULL) {
        if (sfp->data.choice == SEQFEAT_CDREGION) {
          cdsQuals = TRUE;
        }
      }
      */
    }
    m = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (m, 10, 10);
    ffp->commonPage = 0;
    if (cdsQuals) {
    } else if (hasQuals) {
      commonRadioFormTabs [4] = "Qualifiers";
      commonNoCitFormTabs [3] = "Qualifiers";
    }
    if (hasCitationTab) {
      ffp->commonRadio = CreateFolderTabs (m, commonRadioFormTabs, ffp->commonPage,
                                           0, 0, PROGRAM_FOLDER_TAB,
                                           ChangeSubGroup, (Pointer) ffp);
    } else {
      ffp->commonRadio = CreateFolderTabs (m, commonNoCitFormTabs, ffp->commonPage,
                                           0, 0, PROGRAM_FOLDER_TAB,
                                           ChangeSubGroup, (Pointer) ffp);
    }
    commonRadioFormTabs [4] = NULL;
    commonNoCitFormTabs [3] = NULL;
    p = HiddenGroup (m, 0, 0, NULL);

    c = HiddenGroup (p, -1, 0, NULL);
    SetGroupSpacing (c, 10, 10);
    f = HiddenGroup (c, -1, 0, NULL);
    r = HiddenGroup (f, 7, 0, NULL);
    StaticPrompt (r, "Flags", 0, popupMenuHeight, programFont, 'l');
    ffp->partial = CheckBox (r, "Partial", NULL);
    indexerVersion = (Boolean) (GetAppProperty ("InternalNcbiSequin") != NULL);
    if (! indexerVersion) {
      Disable (ffp->partial);
    }
    if (ffp->pseudo == NULL) {
      ffp->pseudo = CheckBox (r, "Pseudo", NULL);
      pseudo = ffp->pseudo; /* allows pseudo control on earlier feature-specific page */
    }
    StaticPrompt (r, "Evidence", 0, popupMenuHeight, programFont, 'l');
    ffp->evidence = PopupList (r, TRUE, NULL);
    PopupItem (ffp->evidence, " ");
    PopupItem (ffp->evidence, "Experimental");
    PopupItem (ffp->evidence, "Non-Experimental");
    AlignObjects (ALIGN_MIDDLE, (HANDLE) ffp->partial,
                  (HANDLE) ffp->evidence, (HANDLE) pseudo, NULL);
    r = HiddenGroup (f, -3, 0, NULL);
    ffp->exception = CheckBox (r, "Exception", NULL);
    StaticPrompt (r, "Explanation", 0, dialogTextHeight, programFont, 'l');
    ffp->exceptText = DialogText (r, "", 12, NULL);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) ffp->exception,
                 (HANDLE) ffp->exceptText, NULL);
    if (ffp->this_subtype == FEATDEF_CDS) {
      StaticPrompt (r, "Standard explanation", 0, popupMenuHeight, programFont, 'l');
      canned = PopupList (r, TRUE, ChangeCannedMessage);
      SetObjectExtra (canned, (Pointer) ffp, NULL);
      PopupItem (canned, " ");
      PopupItem (canned, "RNA editing");
      PopupItem (canned, "reasons given in citation");
      PopupItem (canned, "ribosomal slippage");
      PopupItem (canned, "trans splicing");
      PopupItem (canned, "artificial frameshift");
      if (sfp != NULL && sfp->excpt) {
        if (StringICmp (sfp->except_text, "RNA editing") == 0) {
          SetValue (canned, 2);
        } else if (StringICmp (sfp->except_text, "reasons given in citation") == 0 ||
                   StringICmp (sfp->except_text, "reasons cited in publication") == 0) {
          SetValue (canned, 3);
        } else if (StringICmp (sfp->except_text, "ribosomal slippage") == 0) {
          SetValue (canned, 4);
        } else if (StringICmp (sfp->except_text, "ribosome slippage") == 0) {
          SetValue (canned, 4);
        } else if (StringICmp (sfp->except_text, "trans splicing") == 0) {
          SetValue (canned, 5);
        } else if (StringICmp (sfp->except_text, "trans-splicing") == 0) {
          SetValue (canned, 5);
        } else if (StringICmp (sfp->except_text, "artificial frameshift") == 0) {
          SetValue (canned, 6);
        }
      } else {
        SetValue (canned, 1);
      }
    }

    if (cdsQuals) {
      /*
      ffp->gbquals = CreateImportFields (c, NULL, sfp, FALSE);
      */
    }

    g = NULL;
    k = NULL;
    ffp->gene = NULL;
    ffp->genePopup = NULL;
    ffp->geneList = NULL;
    ffp->geneNames = NULL;
    ffp->useGeneXref = NULL;
    ffp->newGeneGrp = NULL;
    ffp->geneSymbol = NULL;
    ffp->geneDesc = NULL;
    for (page = 0; page < 5; page++) {
      ffp->commonSubGrp [page] = NULL;
    }
    page = 0;

    if (hasGeneControl) {
      g = HiddenGroup (c, -2, 0, NULL);
      StaticPrompt (g, "Gene", 0, popupMenuHeight, programFont, 'l');
      v = HiddenGroup (g, 0, 0, NULL);
      ffp->genePopup = PopupList (v, TRUE, (PupActnProc) ChangeGenePopupOrList);
      SetObjectExtra (ffp->genePopup, (Pointer) ffp, NULL);
      PopupItem (ffp->genePopup, " ");
      PopupItem (ffp->genePopup, "New:");
      SetValue (ffp->genePopup, 1);
      Hide (ffp->genePopup);
      ffp->geneList = SingleList (v, 6, 3, (LstActnProc) ChangeGenePopupOrList);
      SetObjectExtra (ffp->geneList, (Pointer) ffp, NULL);
      ListItem (ffp->geneList, " ");
      ListItem (ffp->geneList, "New:");
      SetValue (ffp->geneList, 1);
      Hide (ffp->geneList);
      k = HiddenGroup (c, 3, 0, NULL);
      StaticPrompt (k, "Map by", 0, stdLineHeight, programFont, 'l');
      ffp->useGeneXref = HiddenGroup (k, 3, 0, GeneXrefWarn);
      SetObjectExtra (ffp->useGeneXref, ffp, NULL);
      RadioButton (ffp->useGeneXref, "Overlap");
      RadioButton (ffp->useGeneXref, "Cross-reference");
      RadioButton (ffp->useGeneXref, "Suppress");
      SetValue (ffp->useGeneXref, 1);
      y = HiddenGroup (c, 0, 0, NULL);
      ffp->newGeneGrp = HiddenGroup (y, 2, 0, NULL);
      StaticPrompt (ffp->newGeneGrp, "Gene Symbol", 0, dialogTextHeight, programFont, 'l');
      ffp->geneSymbol = DialogText (ffp->newGeneGrp, "", 20, NULL);
      StaticPrompt (ffp->newGeneGrp, "Description", 0, dialogTextHeight, programFont, 'l');
      ffp->geneDesc = DialogText (ffp->newGeneGrp, "", 20, NULL);
      Hide (ffp->newGeneGrp);
      ffp->editGeneBtn = PushButton (y, "Edit Gene Feature", Nlm_LaunchGeneFeatEd);
      SetObjectExtra (ffp->editGeneBtn, ffp, NULL);
      Hide (ffp->editGeneBtn);
    } else if (hasGeneSuppress) {
      k = HiddenGroup (c, 3, 0, NULL);
      StaticPrompt (k, "Map by", 0, stdLineHeight, programFont, 'l');
      ffp->useGeneXref = HiddenGroup (k, 3, 0, GeneXrefWarn);
      SetObjectExtra (ffp->useGeneXref, ffp, NULL);
      RadioButton (ffp->useGeneXref, "Overlap");
      b = RadioButton (ffp->useGeneXref, "Cross-reference");
      Disable (b);
      RadioButton (ffp->useGeneXref, "Suppress");
      SetValue (ffp->useGeneXref, 1);
      y = HiddenGroup (c, 0, 0, NULL);
    }
    ffp->commonSubGrp [page] = c;
    page++;
    AlignObjects (ALIGN_CENTER, (HANDLE) f, (HANDLE) g,
                  (HANDLE) k, (HANDLE) ffp->newGeneGrp,
                  (HANDLE) ffp->editGeneBtn, NULL);

    c = HiddenGroup (p, -1, 0, NULL);
    SetGroupSpacing (c, 10, 10);
    q = HiddenGroup (c, 0, 2, NULL);
    StaticPrompt (q, "Comment", 25 * stdCharWidth, 0, programFont, 'c');
    if (GetAppProperty ("InternalNcbiSequin") != NULL) {
      ffp->comment = ScrollText (q, 25, 10, programFont, TRUE, NULL);
    } else {
      ffp->comment = ScrollText (q, 25, 5, programFont, TRUE, NULL);
    }
    ffp->commonSubGrp [page] = c;
    Hide (ffp->commonSubGrp [page]);
    page++;

    if (hasCitationTab) {
      c = HiddenGroup (p, -1, 0, NULL);
      SetGroupSpacing (c, 10, 10);
      ffp->featcits = CreateCitOnFeatDialog (c, NULL);
      b = PushButton (c, "Edit Citations", EditFeatCitsProc);
      SetObjectExtra (b, ffp, NULL);
      ffp->commonSubGrp [page] = c;
      AlignObjects (ALIGN_CENTER, (HANDLE) ffp->featcits, (HANDLE) b, NULL);
      Hide (ffp->commonSubGrp [page]);
      page++;
    }

    c = HiddenGroup (p, -1, 0, NULL);
    SetGroupSpacing (c, 10, 10);
    if (GetAppProperty ("ReadOnlyDbTags") == NULL) {
      just = 'c';
    } else {
      just = 'l';
      StaticPrompt (c, "This page is read-only", 15 * stdCharWidth, 0, programFont, 'c');
    }
    t = HiddenGroup (c, 2, 0, NULL);
    StaticPrompt (t, "Database", 7 * stdCharWidth, 0, programFont, just);
    StaticPrompt (t, "Object ID", 8 * stdCharWidth, 0, programFont, just);
    ffp->dbxrefs = CreateDbtagDialog (c, 3, -1, 7, 8);
    ffp->commonSubGrp [page] = c;
    Hide (ffp->commonSubGrp [page]);
    page++;

    c = HiddenGroup (p, -1, 0, NULL);
    SetGroupSpacing (c, 10, 10);
    x = NULL;
    /* z = NULL; */
    if (hasQuals && ffp->gbquals == NULL) {
      x = HiddenGroup (c, -1, 0, NULL);
      /*
      z = HiddenGroup (x, -4, 0, NULL);
      SetGroupSpacing (z, -1, 0);
      StaticPrompt (z, "Qualifier", 7 * stdCharWidth, 0, programFont, 'c');
      StaticPrompt (z, "Value", 10 * stdCharWidth, 0, programFont, 'c');
      ffp->gbquals = CreateQualsDialog (x, 5, -1, 7, 10);
      */
      ffp->gbquals = CreateImportFields (x, NULL, sfp, FALSE);
    }
    ffp->commonSubGrp [page] = c;
    Hide (ffp->commonSubGrp [page]);
    page++;

    AlignObjects (ALIGN_CENTER, (HANDLE) ffp->commonRadio, (HANDLE) ffp->commonSubGrp [0],
                  (HANDLE) ffp->commonSubGrp [1], (HANDLE) ffp->commonSubGrp [2],
                  (HANDLE) ffp->commonSubGrp [3], (HANDLE) ffp->commonSubGrp [4],
                  (HANDLE) ffp->gbquals, NULL);
  }
  return c;
}

extern GrouP CreateCommonFeatureGroup (GrouP h, FeatureFormPtr ffp,
                                       SeqFeatPtr sfp, Boolean hasGeneControl,
                                       Boolean hasCitationTab)

{
  return CreateCommonFeatureGroupEx (h, ffp, sfp, hasGeneControl, hasCitationTab, FALSE);
}

static Boolean DlgutilFindBspItem (GatherContextPtr gcp)

{
  BioseqPtr  PNTR bspp;

  bspp = (BioseqPtr PNTR) gcp->userdata;
  if (bspp != NULL && gcp->thistype == OBJ_BIOSEQ) {
    *bspp = (BioseqPtr) gcp->thisitem;
  }
  return TRUE;
}

extern void SetNewFeatureDefaultInterval (FeatureFormPtr ffp)

{
  BioseqPtr     bsp;
  SelStructPtr  sel;
  SeqIntPtr     sip;
  SeqLocPtr     slp;

  if (ffp == NULL) return;
  bsp = NULL;
  GatherItem (ffp->input_entityID, ffp->input_itemID, ffp->input_itemtype,
              (Pointer) (&bsp), DlgutilFindBspItem);
  if (bsp == NULL) return;
  slp = NULL;
  sel = ObjMgrGetSelected ();
  if (sel != NULL && sel->next == NULL && sel->entityID == ffp->input_entityID &&
      sel->itemID == ffp->input_itemID && sel->itemtype == ffp->input_itemtype) {
    if (sel->regiontype == 1 && sel->region != NULL) {
      if (GetBioseqGivenSeqLoc ((SeqLocPtr) sel->region, ffp->input_entityID) == bsp) {
        slp = AsnIoMemCopy (sel->region,
                            (AsnReadFunc) SeqLocAsnRead,
                            (AsnWriteFunc) SeqLocAsnWrite);
      }
    }
  }
  if (slp == NULL) {
    slp = ValNodeNew (NULL);
    if (slp == NULL) return;
    sip = SeqIntNew ();
    if (sip == NULL) {
      slp = SeqLocFree (slp);
      return;
    }
    slp->choice = SEQLOC_INT;
    slp->data.ptrvalue = (Pointer) sip;
    sip->from = 0;
    sip->to = bsp->length - 1;
    sip->strand = Seq_strand_plus;
    sip->id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
  }
  if (slp != NULL) {
    PointerToDialog (ffp->location, (Pointer) slp);
    SeqLocFree (slp);
  }
}

extern Boolean FileToScrollText (TexT t, CharPtr path)

{
  FILE     *fp;
  Int4     len;
  Int4     max;
  CharPtr  str;
#ifdef WIN_MAC
  CharPtr  p;
#endif
#if (defined(OS_DOS) || defined (OS_NT))
  CharPtr  p;
  CharPtr  q;
#endif

  if (t != NULL && path != NULL && *path != '\0') {
    len = FileLength (path);
#ifdef WIN_MOTIF
    max = INT4_MAX;
#else
    max = (Int4) INT2_MAX;
#endif
    if (len > 0 && len < max - 4) {
      str = MemNew (sizeof (char) * (len + 3));
      if (str != NULL) {
        fp = FileOpen (path, "r");
        if (fp != NULL) {
          FileRead (str, sizeof (char), (size_t) len, fp);
#if (defined(OS_DOS) || defined (OS_NT))
          p = str;
          q = str;
          while (*p) {
            if (*p == '\r') {
              p++;
            } else {
              *q = *p;
              p++;
              q++;
            }
          }
          *q = '\0';
#endif
#ifdef WIN_MAC
          p = str;
          while (*p) {
            if (*p == '\r' || *p == '\n') {
              *p = '\015';
            }
            p++;
          }
#endif
          FileClose (fp);
          SetTitle (t, str);
        }
        MemFree (str);
        return TRUE;
      }
    }
  }
  return FALSE;
}

extern void ScrollTextToFile (TexT t, CharPtr path)

{
  FILE     *fp;
  size_t   len;
  Int4     max;
  CharPtr  str;
#ifdef WIN_MAC
  CharPtr  p;
#endif

  if (t != NULL && path != NULL && *path != '\0') {
    len = TextLength (t);
#ifdef WIN_MOTIF
    max = INT4_MAX;
#else
    max = (Int4) INT2_MAX;
#endif
    if (len > 0 && (Int4) len < max - 4) {
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
        str = MemNew (sizeof (char) * (len + 3));
        if (str != NULL) {
          GetTitle (t, str, len + 1);
#ifdef WIN_MAC
          p = str;
          while (*p) {
            if (*p == '\r' || *p == '\n') {
              *p = '\n';
            }
            p++;
          }
#endif
          FileWrite (str, sizeof (char), len, fp);
          MemFree (str);
        }
        FileClose (fp);
      }
    }
  }
}

extern void FileToClipboard (CharPtr path)

{
  FILE     *fp;
  Int4     len;
  Int4     max;
  CharPtr  str;
#ifdef WIN_MAC
  CharPtr  p;
#endif
#if (defined(OS_DOS) || defined (OS_NT))
  CharPtr  p;
  CharPtr  q;
#endif

  if (path != NULL && *path != '\0') {
    len = FileLength (path);
#ifdef WIN_MOTIF
    max = INT4_MAX;
#else
    max = (Int4) INT2_MAX;
#endif
    if (len > 0 && len < max - 4) {
      str = MemNew (sizeof (char) * (len + 3));
      if (str != NULL) {
        fp = FileOpen (path, "r");
        if (fp != NULL) {
          FileRead (str, sizeof (char), (size_t) len, fp);
#if (defined(OS_DOS) || defined (OS_NT))
          p = str;
          q = str;
          while (*p) {
            if (*p == '\r') {
              p++;
            } else {
              *q = *p;
              p++;
              q++;
            }
          }
          *q = '\0';
#endif
#ifdef WIN_MAC
          p = str;
          while (*p) {
            if (*p == '\r' || *p == '\n') {
              *p = '\015';
            }
            p++;
          }
#endif
          FileClose (fp);
          Nlm_StringToClipboard (str);
        }
        MemFree (str);
      }
    }
  }
}

typedef struct textviewform {
  FORM_MESSAGE_BLOCK

  DoC              doc;
  TexT             text;
} TextViewForm, PNTR TextViewFormPtr;

static ParData txtParFmt = {FALSE, FALSE, FALSE, FALSE, TRUE, 0, 0};
static ColData txtColFmt = {0, 0, 80, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE};

static void ResizeTextViewer (WindoW w)

{
  Int2             height;
  RecT             r;
  RecT             s;
  TextViewFormPtr  tfp;
  Int2             width;

  tfp = (TextViewFormPtr) GetObjectExtra (w);
  if (tfp != NULL) {
    WatchCursor ();
    ObjectRect (w, &r);
    width = r.right - r.left;
    height = r.bottom - r.top;
    if (tfp->doc != NULL) {
      GetPosition (tfp->doc, &s);
      s.right = width - s.left;
      s.bottom = height - s.left;
      SetPosition (tfp->doc, &s);
      AdjustPrnt (tfp->doc, &s, FALSE);
      txtColFmt.pixWidth = screenRect.right - screenRect.left;
      txtColFmt.pixInset = 8;
      if (Visible (tfp->doc) && AllParentsVisible (tfp->doc)) {
        UpdateDocument (tfp->doc, 0, 0);
      }
    }
    if (tfp->text != NULL) {
      GetPosition (tfp->text, &s);
      s.right = width - s.left;
      s.bottom = height - s.left;
      SetPosition (tfp->text, &s);
      AdjustPrnt (tfp->text, &s, FALSE);
    }
    ArrowCursor ();
    Update ();
  }
}

static Boolean ExportTextViewForm (ForM f, CharPtr filename)

{
  FILE             *fp;
  Char             path [PATH_MAX];
  TextViewFormPtr  tfp;

  tfp = (TextViewFormPtr) GetObjectExtra (f);
  if (tfp != NULL && (tfp->doc != NULL || tfp->text != NULL)) {
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
      if (tfp->doc != NULL) {
        fp = FileOpen (path, "w");
        if (fp != NULL) {
          SaveDocument (tfp->doc, fp);
          FileClose (fp);
          return TRUE;
        }
      } else if (tfp->text != NULL) {
        ScrollTextToFile (tfp->text, path);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static void LIBCALL PrintTextViewForm (Pointer formDataPtr)

{
  TextViewFormPtr  tfp;

  tfp = (TextViewFormPtr) formDataPtr;
  if (tfp != NULL && tfp->doc != NULL) {
    PrintDocument (tfp->doc);
  }
}

static void TextViewFormMessage (ForM f, Int2 mssg)

{
  TextViewFormPtr  tfp;

  tfp = (TextViewFormPtr) GetObjectExtra (f);
  if (tfp != NULL) {
    switch (mssg) {
      case VIB_MSG_EXPORT :
        ExportTextViewForm (f, NULL);
        break;
      case VIB_MSG_CLOSE :
        Remove (f);
        break;
      case VIB_MSG_PRINT :
        PrintTextViewForm (tfp);
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
        if (tfp->appmessage != NULL) {
          tfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

extern void LaunchGeneralTextViewer (CharPtr path, CharPtr title)

{
  FonT               fnt;
  Int2               pixheight;
  Int2               pixwidth;
  StdEditorProcsPtr  sepp;
  TextViewFormPtr    tfp;
  TextViewProcsPtr   tvpp;
  WindoW             w;
#ifndef WIN_MAC
  MenU               m;
#endif

  tfp = (TextViewFormPtr) MemNew (sizeof (TextViewForm));
  if (tfp != NULL) {
    w = DocumentWindow (-50, -33, -10, -10, title, StdCloseWindowProc, ResizeTextViewer);
    SetObjectExtra (w, tfp, StdCleanupFormProc);
    tfp->form = (ForM) w;
    tfp->exportform = ExportTextViewForm;
    tfp->formmessage = TextViewFormMessage;

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      SetActivate (w, sepp->activateForm);
      tfp->appmessage = sepp->handleMessages;
    }

    fnt = programFont;
    pixwidth = 35 * stdCharWidth + 17;
    pixheight = 20 * stdLineHeight;

    tvpp = (TextViewProcsPtr) GetAppProperty ("TextDisplayForm");
    if (tvpp != NULL) {
      pixwidth = MAX (pixwidth, tvpp->minPixelWidth);
      pixheight = MAX (pixheight, tvpp->minPixelHeight);
      if (tvpp->displayFont != NULL) {
        fnt = tvpp->displayFont;
      }
      if (tvpp->activateForm != NULL) {
        SetActivate (w, tvpp->activateForm);
      }
    }

#ifndef WIN_MAC
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Export...", (BaseFormPtr) tfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
    FormCommandItem (m, "Close", (BaseFormPtr) tfp, VIB_MSG_CLOSE);
    if (tvpp != NULL && tvpp->useScrollText) {
      m = PulldownMenu (w, "Edit");
      FormCommandItem (m, CUT_MENU_ITEM, (BaseFormPtr) tfp, VIB_MSG_CUT);
      FormCommandItem (m, COPY_MENU_ITEM, (BaseFormPtr) tfp, VIB_MSG_COPY);
      FormCommandItem (m, PASTE_MENU_ITEM, (BaseFormPtr) tfp, VIB_MSG_PASTE);
      FormCommandItem (m, CLEAR_MENU_ITEM, (BaseFormPtr) tfp, VIB_MSG_DELETE);
    }
#endif

    if (tvpp != NULL && tvpp->useScrollText) {
      tfp->text = ScrollText (w, (pixwidth + stdCharWidth - 1) / stdCharWidth,
                              (pixheight + stdLineHeight - 1) / stdLineHeight,
                              fnt, FALSE, NULL);
      SetObjectExtra (tfp->text, tfp, NULL);
      RealizeWindow (w);
      if (! FileToScrollText (tfp->text, path)) {
        SetTitle (tfp->text, "(Text is too large to be displayed in this control.)");
      }
    } else {
      tfp->doc = DocumentPanel (w, pixwidth, pixheight);
      SetObjectExtra (tfp->doc, tfp, NULL);
      RealizeWindow (w);
      txtColFmt.pixWidth = screenRect.right - screenRect.left;
      txtColFmt.pixInset = 8;
      DisplayFancy (tfp->doc, path, &txtParFmt, &txtColFmt, fnt, 0);
      /* document.c: SaveTableItem does not strip preceeding tabs if tabCount is 0 */
    }
    Show (w);
    Select (w);
    Update ();
  } 
}

#ifndef WIN_MAC
extern void CreateStdEditorFormMenus (WindoW w)

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
    FormCommandItem (m, "Close", bfp, VIB_MSG_CLOSE);
    m = PulldownMenu (w, "Edit");
    FormCommandItem (m, CUT_MENU_ITEM, bfp, VIB_MSG_CUT);
    FormCommandItem (m, COPY_MENU_ITEM, bfp, VIB_MSG_COPY);
    FormCommandItem (m, PASTE_MENU_ITEM, bfp, VIB_MSG_PASTE);
    FormCommandItem (m, CLEAR_MENU_ITEM, bfp, VIB_MSG_DELETE);
  }
}
#endif

static Boolean DlgutilGetLowestStackSeqEntry (GatherContextPtr gcp)

{
  BaseFormPtr  bfp;
  Int2         i;

  if (gcp == NULL) return TRUE;
  bfp = (BaseFormPtr) gcp->userdata;
  if (bfp == NULL) return TRUE;
  if (gcp->gatherstack != NULL && gcp->numstack > 0) {
    for (i = 0; i < gcp->numstack; i++) {
      if (gcp->gatherstack [i].itemtype == OBJ_BIOSEQ ||
          gcp->gatherstack [i].itemtype == OBJ_BIOSEQSET) {
        bfp->input_itemID = gcp->gatherstack [i].itemID;
        bfp->input_itemtype = gcp->gatherstack [i].itemtype;
      }
    }
  }
  return FALSE;
}

extern Boolean SetClosestParentIfDuplicating (BaseFormPtr bfp)

{
  Uint2              itemID;
  Uint2              itemtype;
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (bfp == NULL || sepp == NULL || (! sepp->duplicateExisting)) return FALSE;
  itemID = bfp->input_itemID;
  itemtype = bfp->input_itemtype;
  GatherItem (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype,
              (Pointer) bfp, DlgutilGetLowestStackSeqEntry);
  if (itemID == bfp->input_itemID && itemtype == bfp->input_itemtype) {
    return FALSE;
  }
  return TRUE;
}

/*****************************************************************************
*
*   Bond and Point SeqLoc dialogs
*
*****************************************************************************/

typedef struct pointpage {
  DIALOG_MESSAGE_BLOCK
  TexT               point;
  Int2               count;
  SeqEntryPtr        PNTR bsptr;
  EnumFieldAssoc     PNTR alist;
  PopuP              strand;
  PopuP              seqIdx;
  Boolean            nucsOK;
  Boolean            protsOK;
  Boolean            showIdTags;
} PointPage, PNTR PointPagePtr;

typedef struct bondpage {
  DIALOG_MESSAGE_BLOCK
  DialoG             pointA;
  DialoG             pointB;
} BondPage, PNTR BondPagePtr;

static void FillInPointProducts (SeqEntryPtr sep, Pointer mydata,
                                 Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  PointPagePtr  ppp;

  if (sep != NULL && mydata != NULL && sep->choice == 1) {
    ppp = (PointPagePtr) mydata;
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp != NULL) {
      if ((ppp->nucsOK && ISA_na (bsp->mol)) ||
          (ppp->protsOK && ISA_aa (bsp->mol))) {
        ppp->count++;
        ppp->bsptr [ppp->count] = sep;
      }
    }
  }
}

static ENUM_ALIST(strand_alist)
{" ",             Seq_strand_unknown},  /* 0 */
{"Plus",          Seq_strand_plus},     /* 1 */
{"Minus",         Seq_strand_minus},    /* 2 */
{"Both",          Seq_strand_both},     /* 3 */
{"Reverse",       Seq_strand_both_rev}, /* 4 */
{"Other",         Seq_strand_other},    /* 255 */
END_ENUM_ALIST

static void SeqPntPtrToPointPage (DialoG d, Pointer data)


{
  BioseqPtr     bsp;
  Int2          j;
  PointPagePtr  ppp;
  SeqEntryPtr   sep;
  Int2          seq;
  SeqPntPtr     spp;
  Char          str [16];
  Uint1         strand;

  ppp = (PointPagePtr) GetObjectExtra (d);
  if (ppp == NULL) return;
  spp = (SeqPntPtr) data;
  if (spp != NULL) {
    sprintf (str, "%ld", (long) (spp->point + 1));
    SafeSetTitle (ppp->point, str);
    seq = 0;
    strand = 0;
    bsp = BioseqFind (spp->id);
    if (bsp != NULL) {
      strand = spp->strand;
      if (strand > Seq_strand_both_rev && strand != Seq_strand_other) {
        strand = Seq_strand_unknown;
      }
      if (ppp->bsptr != NULL) {
        for (j = 1; j <= ppp->count && seq == 0; j++) {
          sep = ppp->bsptr [j];
          if (sep != NULL && sep->choice == 1) {
            if (bsp == (BioseqPtr) sep->data.ptrvalue) {
              seq = j;
            }
          }
        }
      }
    }
    SetEnumPopup (ppp->strand, strand_alist, (UIEnum) strand);
    SetEnumPopup (ppp->seqIdx, ppp->alist, (UIEnum) seq);
  } else {
    SafeSetTitle (ppp->point, "");
    SafeSetValue (ppp->strand, 0);
    SafeSetValue (ppp->seqIdx, 0);
  }
}

static Pointer PointPageToSeqPntPtr (DialoG d)

{
  BioseqPtr     bsp;
  PointPagePtr  ppp;
  SeqEntryPtr   sep;
  UIEnum        seq;
  SeqPntPtr     spp;
  Char          str [16];
  UIEnum        strand;
  Int4          val;

  ppp = (PointPagePtr) GetObjectExtra (d);
  if (ppp == NULL) return NULL;
  spp = NULL;
  GetTitle (ppp->point, str, sizeof (str) - 1);
  if (StrToLong (str, &val) && val > 0) {
    if (GetEnumPopup (ppp->seqIdx, ppp->alist, &seq) &&
        seq > 0 && seq <= ppp->count) {
      spp = SeqPntNew ();
      if (spp != NULL) {
        spp->point = val - 1;
        if (GetEnumPopup (ppp->strand, strand_alist, &strand)) {
          spp->strand = (Uint1) strand;
        }
        sep = ppp->bsptr [(Int2) seq];
        if (sep != NULL && sep->choice == 1) {
          bsp = (BioseqPtr) sep->data.ptrvalue;
          if (bsp != NULL) {
            spp->id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
          }
        }
      }
    }
  }
  return (Pointer) spp;
}

static void PointEditorMessage (DialoG d, Int2 mssg)

{
  PointPagePtr  ppp;

  ppp = (PointPagePtr) GetObjectExtra (d);
  if (ppp != NULL) {
    if (mssg == VIB_MSG_INIT) {
      SeqPntPtrToPointPage (d, NULL);
    } else if (mssg == VIB_MSG_ENTER) {
      Select (ppp->point);
    } else if (mssg == VIB_MSG_RESET) {
    }
  }
}

static void CleanupPointPage (GraphiC g, VoidPtr data)

{
  Int2          j;
  PointPagePtr  ppp;

  ppp = (PointPagePtr) data;
  if (ppp != NULL) {
    MemFree (ppp->bsptr);
    if (ppp->alist != NULL) {
      for (j = 0; j <= ppp->count + 1; j++) {
        MemFree (ppp->alist [j].name);
      }
    }
    MemFree (ppp->alist);
  }
  MemFree (data);
}

static DialoG CreatePointEditorDialog (GrouP h, CharPtr title, SeqEntryPtr sep,
                                       Boolean nucsOK, Boolean protsOK)

{
  BioseqPtr     bsp;
  Int4          count;
  GrouP         f;
  Int2          j;
  GrouP         m;
  GrouP         p;
  PointPagePtr  ppp;
  CharPtr       ptr;
  GrouP         s;
  Boolean       showIdTags;
  SeqIdPtr      sip;
  Char          str [128];

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  ppp = (PointPagePtr) MemNew (sizeof (PointPage));
  if (ppp != NULL) {

    SetObjectExtra (p, ppp, CleanupPointPage);
    ppp->dialog = (DialoG) p;
    ppp->todialog = SeqPntPtrToPointPage;
    ppp->fromdialog = PointPageToSeqPntPtr;
    ppp->dialogmessage = PointEditorMessage;
    ppp->testdialog = NULL;
    ppp->importdialog = NULL;
    ppp->exportdialog = NULL;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    m = HiddenGroup (s, -1, 0, NULL);
    /*
    SetGroupSpacing (m, 10, 10);
    */

    ppp->nucsOK = nucsOK;
    ppp->protsOK = protsOK;
    ppp->showIdTags = FALSE;
    ppp->count = 0;

    if (sep != NULL) {
      count = SeqEntryCount (sep);
      count += 4;
      ppp->bsptr = MemNew (sizeof (BioseqPtr) * (size_t) count);
      ppp->alist = MemNew (sizeof (EnumFieldAssoc) * (size_t) count);
      ppp->count = 0;

      if (ppp->bsptr != NULL && ppp->alist != NULL) {
        SeqEntryExplore (sep, (Pointer) ppp, FillInPointProducts);
        j = 0;
        ppp->alist [j].name = StringSave ("     ");
        ppp->alist [j].value = (UIEnum) 0;
        for (j = 1; j <= ppp->count; j++) {
          sep = ppp->bsptr [j];
          if (sep != NULL && sep->choice == 1 && sep->data.ptrvalue != NULL) {
            bsp = (BioseqPtr) sep->data.ptrvalue;
            sip = SeqIdFindWorst (bsp->id);
            SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
            ptr = StringChr (str, '|');
            showIdTags = FALSE;
            if (ptr == NULL) {
              ptr = str;
            } else if (showIdTags) {
              ptr = str;
            } else {
              ptr++;
            }
            ppp->alist [j].name = StringSave (ptr);
            ppp->alist [j].value = (UIEnum) j;
          }
        }
        j = ppp->count + 1;
        ppp->alist [j].name = NULL;
        ppp->alist [j].value = (UIEnum) 0;
      }

    } else {
      ppp->alist = MemNew (sizeof (EnumFieldAssoc) * (size_t) 4);
      if (ppp->alist != NULL) {
        j = 0;
        ppp->alist [j].name = StringSave ("     ");
        ppp->alist [j].value = (UIEnum) 0;
        j = 1;
        ppp->alist [j].name = NULL;
        ppp->alist [j].value = (UIEnum) 0;

      }
    }

    f = HiddenGroup (m, 6, 0, NULL);
    /*StaticPrompt (f, "Point", 0, dialogTextHeight, programFont, 'l');*/
    ppp->point = DialogText (f, "", 5, NULL);
    if (nucsOK) {
      /*StaticPrompt (f, "Strand", 0, popupMenuHeight, programFont, 'c');*/
      ppp->strand = PopupList (f, TRUE, NULL);
      InitEnumPopup (ppp->strand, strand_alist, NULL);
    }
    /*StaticPrompt (f, "SeqID", 0, popupMenuHeight, programFont, 'l');*/
    ppp->seqIdx = PopupList (f, TRUE, NULL);
    InitEnumPopup (ppp->seqIdx, ppp->alist, NULL);
  }

  return (DialoG) p;
}

static void SeqLocPtrToBondPage (DialoG d, Pointer data)


{
  BondPagePtr  bpp;
  SeqBondPtr   sbp;
  SeqIdPtr     sip;
  SeqLocPtr    slp;
  SeqPnt       sqp;

  bpp = (BondPagePtr) GetObjectExtra (d);
  if (bpp == NULL) return;
  slp = (SeqLocPtr) data;
  if (slp != NULL) {
    if (slp->choice == SEQLOC_BOND) {
      sbp = (SeqBondPtr) slp->data.ptrvalue;
      if (sbp != NULL) {
        PointerToDialog (bpp->pointA, (Pointer) sbp->a);
        PointerToDialog (bpp->pointB, (Pointer) sbp->b);
      }
    } else {
      sip = SeqLocId (slp);
      if (sip != NULL) {
        sqp.strand = SeqLocStrand (slp);
        sqp.id = sip;
        sqp.point = SeqLocStart (slp);
        PointerToDialog (bpp->pointA, (Pointer) &sqp);
        sqp.point = SeqLocStop (slp);
        PointerToDialog (bpp->pointB, (Pointer) &sqp);
      }
    }
  }
}

static Pointer BondPageToSeqLocPtr (DialoG d)

{
  BondPagePtr  bpp;
  SeqBondPtr   sbp;
  SeqLocPtr    slp;

  bpp = (BondPagePtr) GetObjectExtra (d);
  if (bpp == NULL) return NULL;
  slp = NULL;
  sbp = SeqBondNew ();
  if (sbp != NULL) {
    slp = ValNodeNew (NULL);
    if (slp != NULL) {
      slp->choice = SEQLOC_BOND;
      slp->data.ptrvalue = (Pointer) sbp;
      sbp->a = DialogToPointer (bpp->pointA);
      sbp->b = DialogToPointer (bpp->pointB);
    } else {
      SeqBondFree (sbp);
    }
  }
  return (Pointer) slp;
}

static void BondEditorMessage (DialoG d, Int2 mssg)

{
  BondPagePtr  bpp;

  bpp = (BondPagePtr) GetObjectExtra (d);
  if (bpp != NULL) {
    if (mssg == VIB_MSG_INIT) {
      SeqLocPtrToBondPage (d, NULL);
    } else if (mssg == VIB_MSG_ENTER) {
      SendMessageToDialog (bpp->pointA, VIB_MSG_ENTER);
    } else if (mssg == VIB_MSG_RESET) {
    }
  }
}

extern DialoG CreateBondEditorDialog (GrouP h, CharPtr title, SeqEntryPtr sep);

extern DialoG CreateBondEditorDialog (GrouP h, CharPtr title, SeqEntryPtr sep)

{
  BondPagePtr  bpp;
  GrouP        f;
  GrouP        m;
  GrouP        p;
  GrouP        s;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  bpp = (BondPagePtr) MemNew (sizeof (BondPage));
  if (bpp != NULL) {

    SetObjectExtra (p, bpp, StdCleanupExtraProc);
    bpp->dialog = (DialoG) p;
    bpp->todialog = SeqLocPtrToBondPage;
    bpp->fromdialog = BondPageToSeqLocPtr;
    bpp->dialogmessage = BondEditorMessage;
    bpp->testdialog = NULL;
    bpp->importdialog = NULL;
    bpp->exportdialog = NULL;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    m = HiddenGroup (s, -1, 0, NULL);
    /*
    SetGroupSpacing (m, 10, 10);
    */

    f = HiddenGroup (m, 2, 0, NULL);
    StaticPrompt (f, "From", 0, popupMenuHeight, programFont, 'l');
    bpp->pointA = CreatePointEditorDialog (f, NULL, sep, FALSE, TRUE);
    StaticPrompt (f, "(To)", 0, popupMenuHeight, programFont, 'l');
    bpp->pointB = CreatePointEditorDialog (f, NULL, sep, FALSE, TRUE);
  }

  return (DialoG) p;
}

void GetRidOfEmptyFeatsDescStrings (Uint2 entityID, SeqEntryPtr sep)

{
  if (entityID < 1 && sep == NULL) return;
  if (entityID > 0 && sep == NULL) {
    sep = GetTopSeqEntryForEntityID (entityID);
  }
  if (sep == NULL) return;
  SeqEntryExplore (sep, NULL, GetRidOfEmptyFeatsDescCallback);
}

extern Int2 LIBCALLBACK StdVibrantEditorMsgFunc (OMMsgStructPtr ommsp)

{
  BaseFormPtr    bfp;
  OMUserDataPtr  omudp;

  omudp = (OMUserDataPtr)(ommsp->omuserdata);
  if (omudp == NULL) return OM_MSG_RET_ERROR;
  bfp = (BaseFormPtr) omudp->userdata.ptrvalue;
  if (bfp == NULL) return OM_MSG_RET_ERROR;
  switch (ommsp->message) {
    case OM_MSG_DEL:
      Remove (bfp->form);
      Update ();
      break;
    default :
      break;
  }
  return OM_MSG_RET_OK;
}

/* launch url section */

NLM_EXTERN void LaunchEntrezURL (CharPtr database, Int4 uid, CharPtr format)

{
#ifdef WIN_MOTIF
  NS_Window  window = NULL;
#endif

  Char  url [256];

  if (uid < 1 || StringHasNoText (database) || StringHasNoText (format)) return;
  sprintf (url,
           "http://www.ncbi.nlm.nih.gov:80/entrez/query.fcgi?cmd=Retrieve&db=%s&list_uids=%ld&dopt=%s",
            database, (long) uid, format);

#ifdef WIN_MAC
  Nlm_SendURLAppleEvent (url, "MOSS", NULL);
#endif
#ifdef WIN_MSWIN
  if (! Nlm_MSWin_OpenDocument (url)) {
    Message (MSG_POST, "Unable to launch browser");
  }
#endif
#ifdef WIN_MOTIF
  if (! NS_OpenURL (&window, url, NULL, TRUE)) {
    Message (MSG_POST, "Unable to launch browser");
  }
  NS_WindowFree (window);
#endif
}

