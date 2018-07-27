/*   trmlst.c
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
* File Name:  trmlst.c
*
* Author:  Jonathan Kans, Jonathan Epstein
*
* Version Creation Date:   8/5/96
*
* $Revision: 6.26 $
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
#include <accentr.h>
#include <accutils.h>
#include <vsm.h>
#include <fstyle.h>
#include <maputil.h>

#include <entrez.h>

#define MAX_MODES 11

#define STATE_OFF         0
#define STATE_ON          1

#define GROUP_NONE        0
#define GROUP_SINGLE      1
#define GROUP_FIRST       2
#define GROUP_MIDDLE      3
#define GROUP_LAST        4

#define OP_NONE           0
#define OP_OR             1
#define OP_AND            2
#define OP_NOT            3

#define FETCH_MODE        1
#define EVAL_MODE         2

typedef struct trmstatedata {
  CharPtr                term;
  CharPtr                field;
  Int4                   count;
  Int2                   db;
  Int2                   fld;
  Int2                   group;
  Int2                   above;
  Int2                   below;
  Int2                   state;
  struct trmstatedata    PNTR  prev;
  struct trmstatedata    PNTR  next;
} StateData, PNTR StateDataPtr;

typedef struct termformdata {
  FORM_MESSAGE_BLOCK

  PopuP              database;
  PopuP              PNTR fields;
  PopuP              modes [MAX_MODES];
  ButtoN             accept;
  GrouP              termGrp;
  TexT               term;
  GrouP              rangeGrp;
  TexT               from;
  TexT               to;
  PopuP              taxParents;
  ValNodePtr         taxStrings;

  DoC                avail;
  Int2               avItem;
  Int2               avRow;
  Int2               avClickItem;
  Int2               avClickRow;

  Boolean            wasDoubleClick;
  Boolean            textChanged;
  Int2               currentPage;
  Int2               okayToAccept;

  DoC                chosen;
  Int2               chItem;
  Int2               chClickItem;
  Int2               chClickRow;
  Int2               chClickCol;
  Boolean            inAboveBox;
  Boolean            inBelowBox;
  RecT               trackRect;
  PoinT              trackPt;

  Int2               numChosenLines;
  StateDataPtr       state;

  TexT               advBoolText;
  Boolean            usingAdv;
  Int2               retrieveMode;

  ButtoN             retrieve;
  ButtoN             reset;

  EnumFieldAssocPtr  dbalist;
  EnumFieldAssocPtr  PNTR fldalists;
  Int2Ptr            modeFromField;
  Int2Ptr            fieldModeValues;
  
  Int2               numdbs;
  Int2               numflds;
  Int2               nummods;

  Int4Ptr            uids;
  ValNodePtr         elst;

  Int2               currDb;
  Int2               currFld;
  Int2               currMod;

  Int2               chosenDb;
} TermFormData, PNTR TermFormPtr;

#define SELECTION_MODE     1
#define AUTOMATIC_MODE     2
#define TRUNC_MULT_MODE    3
#define TRUNCATION_MODE    4
#define MESH_TREE_MODE     5
#define TAXONOMY_MODE      6
#define RANGE_MODE         7
#define LOOKUP_ACCN_MODE   8
#define LOOKUP_UID_MODE    9

static ENUM_ALIST(selmultrm_mode_alist)
  {"Automatic",           AUTOMATIC_MODE},
  {"Range",               RANGE_MODE},
  {"Selection",           SELECTION_MODE},
  {"Truncation",          TRUNC_MULT_MODE},
END_ENUM_ALIST

static ENUM_ALIST(selmultru_mode_alist)
  {"Automatic",           AUTOMATIC_MODE},
  {"Range",               RANGE_MODE},
  {"Selection",           SELECTION_MODE},
  {"Truncation",          TRUNCATION_MODE},
END_ENUM_ALIST

static ENUM_ALIST(seltru_mode_alist)
  {"Range",               RANGE_MODE},
  {"Selection",           SELECTION_MODE},
  {"Truncation",          TRUNCATION_MODE},
END_ENUM_ALIST

static ENUM_ALIST(seltrulkp_mode_alist)
  {"Lookup",              LOOKUP_ACCN_MODE},
  {"Range",               RANGE_MODE},
  {"Selection",           SELECTION_MODE},
  {"Truncation",          TRUNCATION_MODE},
END_ENUM_ALIST

static ENUM_ALIST(seltrumsh_mode_alist)
  {"MeSH Tree",           MESH_TREE_MODE},
  {"Range",               RANGE_MODE},
  {"Selection",           SELECTION_MODE},
  {"Truncation",          TRUNCATION_MODE},
END_ENUM_ALIST

static ENUM_ALIST(seltrutax_mode_alist)
  {"Range",               RANGE_MODE},
  {"Selection",           SELECTION_MODE},
  {"Taxonomy",            TAXONOMY_MODE},
  {"Truncation",          TRUNCATION_MODE},
END_ENUM_ALIST

static ENUM_ALIST(lkp_mode_alist)
  {"Lookup",              LOOKUP_UID_MODE},
END_ENUM_ALIST

static EnumFieldAssocPtr mode_alists [] = {
  selmultrm_mode_alist, selmultru_mode_alist, seltru_mode_alist,
  seltrulkp_mode_alist, seltrumsh_mode_alist,
  seltrutax_mode_alist, lkp_mode_alist, NULL
};

#define POPUP_SEL_MUL_TRM      0
#define POPUP_SEL_MUL_TRU      1
#define POPUP_SEL_TRU          2
#define POPUP_SEL_TRU_LKP      3
#define POPUP_SEL_TRU_MSH      4
#define POPUP_SEL_TRU_TAX      5
#define POPUP_LKP              6

extern Int2 DatabaseFromName (CharPtr name)

{
  Int2           db;
  EntrezInfoPtr  eip;

  if (name == NULL) return 0;
  eip = EntrezGetInfo ();
  if (eip == NULL || eip->type_info == NULL) return 0;
  for (db = 0; db < eip->type_count; db++) {
    if (StringICmp (eip->type_info [db].name, name) == 0) {
      return eip->type_info [db].id;
    }
  }
  return 0;
}

extern Int2 FieldFromTag (CharPtr tag)

{
  EntrezInfoPtr  eip;
  Int2           fld;

  if (tag == NULL) return 0;
  eip = EntrezGetInfo ();
  if (eip == NULL || eip->field_info == NULL) return 0;
  for (fld = 0; fld < eip->field_count; fld++) {
    if (StringICmp (eip->field_info [fld].tag, tag) == 0) {
      return eip->field_info [fld].id;
    }
  }
  return 0;
}

static CharPtr TagFromField (Int2 fld)

{
  EntrezInfoPtr  eip;

  eip = EntrezGetInfo ();
  if (eip == NULL || eip->field_info == NULL) return NULL;
  if (fld >= eip->field_count) return NULL;
  return eip->field_info [fld].tag;
}

static void StripNewLine (CharPtr str)

{
  CharPtr  chptr;

  if (StringHasNoText (str)) return;
  chptr = StringRChr (str, '\n');
  if (chptr != NULL) {
    *chptr = '\0';
  }
  chptr = StringRChr (str, '\r');
  if (chptr != NULL) {
    *chptr = '\0';
  }
}

static int loadedlist = 0; /* guarantees that each loaded list has a unique name */

static void DoTLLoadUidList (TermFormPtr tfp)

{
  ByteStorePtr      bsp;
  Int2              db = -1;
  EntrezGlobalsPtr  egp;
  FILE              *fp;
  Int4              num;
  Char              path [PATH_MAX];
  Char              str [32];
  Int4              uid;
  Int4Ptr           uids;


  if (tfp == NULL) return;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL || egp->loadNamedUidProc == NULL) return;
  if (! GetInputFileName (path, sizeof (path), "", "TEXT")) return;
  fp = FileOpen (path, "r");
  if (fp == NULL) return;
  bsp = BSNew (128);
  if (bsp == NULL) {
    FileClose (fp);
    return;
  }
  if (FileGets (str, sizeof (str), fp)) {
    StripNewLine (str);
    if (StringStr (str, ">PubMed")) {
      db = 0;
    } else if (StringStr (str, ">Protein")) {
      db = 1;
    } else if (StringStr (str, ">Nucleotide")) {
      db = 2;
    } else if (StringStr (str, ">Structure")) {
      db = 3;
    } else if (StringStr (str, ">Genome")) {
      db = 4;
    }
    if (db != -1) {
      while (FileGets (str, sizeof (str), fp)) {
        StripNewLine (str);
        if (str [0] != '\0' && StrToLong (str, &uid)) {
          BSWrite (bsp, &uid, sizeof (DocUid));
        }
      }
      BSSeek (bsp, 0L, 0);
      num = (Int4) ((BSLen (bsp)) / sizeof (Int4));
      uids = (Int4Ptr) BSMerge (bsp, NULL);
      if (uids != NULL) {
        loadedlist++;
        sprintf (str, "*loaded_list_%d", (int) loadedlist);
        egp->loadNamedUidProc (tfp->form, str, num, uids, db);
      }
      MemFree (uids);
    } else {
      Message (MSG_POSTERR,
               "First line must be >PubMed, >Protein, >Nucleotide, >Structure, or >Genome");
    }
  }
  BSFree (bsp);
  FileClose (fp);
}

static void DoTLSaveUidList (TermFormPtr tfp)

{
  ByteStorePtr  bs;
  CharPtr       curstr;
  FILE          *fp;
  Int4          j;
  Int4          numLinks;
  Char          path [PATH_MAX];
  Char          str [32];
  Int4          uid;

  if (tfp == NULL) return;
  if (! GetOutputFileName (path, sizeof (path), NULL)) return;
#ifdef WIN_MAC
  FileCreate (path, "TEXT", "ttxt");
#endif
  fp = FileOpen (path, "w");
  if (fp == NULL) return;
  WatchCursor ();
  switch (tfp->currDb) {
    case 0 :
      StringCpy (str, ">PubMed\n");
      break;
    case 1 :
      StringCpy (str, ">Protein\n");
      break;
    case 2 :
      StringCpy (str, ">Nucleotide\n");
      break;
    case 3 :
      StringCpy (str, ">Structure\n");
      break;
    case 4 :
      StringCpy (str, ">Genome\n");
      break;
    default :
      StringCpy (str, ">?\n");
      break;
  }
  FilePuts (str, fp);
  bs = NULL;
  if (tfp->usingAdv) {
    curstr = SaveStringFromText (tfp->advBoolText);
    bs = EntrezTLEvalXString (curstr, tfp->currDb, -1, NULL, NULL);
    MemFree (curstr);
  } else {
    bs = EntrezTLEvalX (tfp->elst);
  }
  if (bs == NULL) {
    ArrowCursor ();
    return;
  }
  numLinks = BSLen (bs) / sizeof (DocUid);
  BSSeek (bs, 0L, 0);
  for (j = 0; j < numLinks; j++) {
    BSRead (bs, &uid, sizeof(DocUid));
    sprintf (str, "%ld\n", (long) uid);
    FilePuts (str, fp);
  }
  BSFree (bs);
  FileClose (fp);
  ArrowCursor ();
}

extern void TLLoadUidListProc (IteM i)

{
  TermFormPtr  tfp;

#ifdef WIN_MAC
  tfp = (TermFormPtr) currentFormDataPtr;
#else
  tfp = (TermFormPtr) GetObjectExtra (i);
#endif
  if (tfp == NULL) return;
  DoTLLoadUidList (tfp);
}

extern void TLSaveUidListProc (IteM i)

{
  TermFormPtr  tfp;

#ifdef WIN_MAC
  tfp = (TermFormPtr) currentFormDataPtr;
#else
  tfp = (TermFormPtr) GetObjectExtra (i);
#endif
  if (tfp == NULL) return;
  DoTLSaveUidList (tfp);
}

static void EntrezTermListFormMessage (ForM f, Int2 mssg)

{
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (f);
  if (tfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_IMPORT :
        DoTLLoadUidList (tfp);
        break;
      case VIB_MSG_EXPORT :
        DoTLSaveUidList (tfp);
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

static void SetTermListImportExportItems (TermFormPtr tfp)

{
  IteM  importItm;
  IteM  exportItm;

  importItm = FindFormMenuItem ((BaseFormPtr) tfp, VIB_MSG_IMPORT);
  if (importItm != NULL) {
    SafeSetTitle (importItm, "Import Uid List...");
  }
  exportItm = FindFormMenuItem ((BaseFormPtr) tfp, VIB_MSG_EXPORT);
  if (exportItm != NULL) {
    SafeSetTitle (exportItm, "Export Uid List...");
  }
}

static void EntrezTermListFormActivate (WindoW w)

{
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (w);
  if (tfp != NULL) {
    if (tfp->activate != NULL) {
      tfp->activate (w);
    }
    SetTermListImportExportItems (tfp);
  }
}

static StateDataPtr StateDataNew (StateDataPtr sdp)

{
  StateDataPtr  newnode;

  newnode = (StateDataPtr) MemNew (sizeof (StateData));
  if (sdp != NULL) {
    while (sdp->next != NULL) {
      sdp = sdp->next;
    }
    sdp->next = newnode;
    newnode->prev = sdp;
  }
  return newnode;
}

static StateDataPtr StateDataFree (StateDataPtr sdp)

{
  StateDataPtr  next;

  if (sdp != NULL && sdp->prev != NULL) {
    sdp->prev->next = NULL;
  }
  while (sdp != NULL) {
    next = sdp->next;
    MemFree (sdp->term);
    MemFree (sdp->field);
    MemFree (sdp);
    sdp = next;
  }
  return NULL;
}

static ParData availParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData availColFmt [] = {
  {0, 5, 70, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* term  */
  {0, 5, 10, 0, NULL, 'r', FALSE, TRUE, FALSE, FALSE, FALSE},  /* count */
  {0, 5, 10, 0, NULL, 'r', FALSE, FALSE, FALSE, FALSE, TRUE}   /* leaf  */
};

static ParData chosenParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData chosenColFmt [] = {
  {0, 23, 70, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE}, /* term  */
  {0, 5, 10, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, FALSE},  /* field */
  {0, 5, 10, 0, NULL, 'r', FALSE, TRUE, FALSE, FALSE, TRUE}     /* count */
};

static void DoResetAvail (TermFormPtr tfp, Boolean clearTerm)

{
  Int2  i;

  if (tfp == NULL) return;
  if (clearTerm) {
    SafeSetTitle (tfp->term, "");
    SafeSetTitle (tfp->to, "");
    SafeSetTitle (tfp->from, "");
    SafeDisable (tfp->accept);
  } else if (tfp->currMod == RANGE_MODE) {
    if (TextHasNoText (tfp->from) && TextHasNoText (tfp->to)) {
      SafeDisable (tfp->accept);
    } else {
      SafeEnable (tfp->accept);
    }
  } else {
    if (TextHasNoText (tfp->term)) {
      SafeDisable (tfp->accept);
    } else {
      SafeEnable (tfp->accept);
    }
  }
  tfp->avItem = 0;
  tfp->avRow = 0;
  Reset (tfp->avail);
  SetDocCache (tfp->avail, NULL, NULL, NULL);
  for (i = 0; i < 7; i++) {
    AppendText (tfp->avail, " \n", &availParFmt, availColFmt, systemFont);
  }
  InvalDocument (tfp->avail);
  SafeHide (tfp->taxParents);
  Reset (tfp->taxParents);
  tfp->taxStrings = ValNodeFreeData (tfp->taxStrings);
  if (tfp->currMod == TAXONOMY_MODE || tfp->currMod == MESH_TREE_MODE) {
    SafeHide (tfp->termGrp);
    SafeHide (tfp->rangeGrp);
    SafeShow (tfp->taxParents);
  } else if (tfp->currMod == RANGE_MODE) {
    SafeHide (tfp->termGrp);
    SafeHide (tfp->taxParents);
    SafeShow (tfp->rangeGrp);
    Select (tfp->from);
  } else {
    SafeHide (tfp->rangeGrp);
    SafeHide (tfp->taxParents);
    SafeShow (tfp->termGrp);
    Select (tfp->term);
  }
}

static void DoResetChosen (TermFormPtr tfp)

{
  Int2  i;

  if (tfp == NULL) return;
  tfp->uids = MemFree (tfp->uids);
  tfp->elst = EntrezTLFree (tfp->elst);
  Reset (tfp->chosen);
  tfp->state = StateDataFree (tfp->state);
  for (i = 0; i < 7; i++) {
    AppendText (tfp->chosen, " \n", &chosenParFmt, chosenColFmt, systemFont);
  }
  InvalDocument (tfp->chosen);
  tfp->numChosenLines = 0;
  Reset (tfp->advBoolText);
  SafeSetTitle (tfp->retrieve, "Retrieve 0 Documents");
  SafeDisable (tfp->retrieve);
}

static void DoReset (TermFormPtr tfp, Boolean clearTerm)

{
  DoResetAvail (tfp, clearTerm);
  DoResetChosen (tfp);
}

static void ResetProc (ButtoN b)

{
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (b);
  DoReset (tfp, TRUE);
}

static ValNodePtr SmartEntrezTLAddTerm (ValNodePtr elst, CharPtr term, DocType type,
                                        DocField field, Boolean special)

{
  CharPtr     ptr;
  CharPtr     str;
  CharPtr     tmp;
  ValNodePtr  vnp;

  vnp = NULL;
  str = StringSave (term);
  ptr = StringChr (str, ':');
  if (ptr != NULL) {
    *ptr = '\0';
    ptr++;
    tmp = str;
    TrimSpacesAroundString (str);
    TrimSpacesAroundString (ptr);
    vnp = EntrezTLAddTermWithRange (elst, tmp, type, field, special, ptr);
  } else {
    vnp = EntrezTLAddTerm (elst, str, type, field, special);
  }
  MemFree (str);
  return vnp;
}

static void RecalculateChosen (TermFormPtr tfp)

{
  Int4          count;
  CharPtr       curstr;
  Int2          group;
  Int2          last;
  StateDataPtr  sdp;
  Char          tmp [32];

  if (tfp == NULL) return;
  count = 0;
  if (tfp->usingAdv) {
    tfp->chosenDb = tfp->currDb;
    tfp->elst = EntrezTLFree (tfp->elst);
    curstr = SaveStringFromText (tfp->advBoolText);
    count = EntrezTLEvalCountString (curstr, tfp->chosenDb, -1, NULL, NULL);
    MemFree (curstr);
  } else {
    if (tfp->state == NULL) {
      SafeSetTitle (tfp->retrieve, "Retrieve 0 Documents");
      SafeDisable (tfp->retrieve);
      return;
    }
    sdp = tfp->state;
    if (tfp->numChosenLines < 1) {
      SafeSetTitle (tfp->retrieve, "Retrieve 0 Documents");
      SafeDisable (tfp->retrieve);
      return;
    }
    tfp->chosenDb = sdp->db;
    if (tfp->chosenDb < 0) {
      SafeSetTitle (tfp->retrieve, "Retrieve 0 Documents");
      SafeDisable (tfp->retrieve);
      return;
    }
    tfp->elst = EntrezTLFree (tfp->elst);
    tfp->elst = EntrezTLNew (tfp->chosenDb);
    group = 0;
    last = 0;
    for (sdp = tfp->state; sdp != NULL; sdp = sdp->next) {
      if (sdp->group == GROUP_SINGLE || sdp->group == GROUP_FIRST) {
        group++;
      }
      if (sdp->state == STATE_ON) {
        if (last == 0) {
          EntrezTLAddLParen (tfp->elst);
          EntrezTLAddLParen (tfp->elst);
        } else if (last == group) {
          EntrezTLAddOR (tfp->elst);
        } else if (sdp->above == OP_NOT) {
          EntrezTLAddRParen (tfp->elst);
          EntrezTLAddRParen (tfp->elst);
          EntrezTLAddBUTNOT (tfp->elst);
          EntrezTLAddLParen (tfp->elst);
          EntrezTLAddLParen (tfp->elst);
        } else {
          EntrezTLAddRParen (tfp->elst);
          EntrezTLAddAND (tfp->elst);
          EntrezTLAddLParen (tfp->elst);
        }
        SmartEntrezTLAddTerm (tfp->elst, sdp->term, sdp->db, sdp->fld, FALSE);
        last = group;
      }
    }
    if (group > 0 && last > 0) {
      EntrezTLAddRParen (tfp->elst);
      EntrezTLAddRParen (tfp->elst);
    }
    count = EntrezTLEvalCount (tfp->elst);
  }
  if (count < 1) {
    SafeSetTitle (tfp->retrieve, "Retrieve 0 Documents");
    SafeDisable (tfp->retrieve);
    return;
  } else if (count > 1) {
    sprintf (tmp, "Retrieve %ld Documents", (long) count);
  } else {
    sprintf (tmp, "Retrieve %ld Document", (long) count);
  }
  SafeSetTitle (tfp->retrieve, tmp);
  if (count < 1 || count > 32000) {
    SafeDisable (tfp->retrieve);
  } else {
    SafeEnable (tfp->retrieve);
  }
}

static void RetrieveDocsProc (ButtoN b)

{
  ByteStorePtr      bs;
  CharPtr           curstr;
  EntrezGlobalsPtr  egp;
  /*
  Int2              i, j;
  */
  LinkSetPtr        lsp;
  Int4              numLinks;
  StateDataPtr      sdp;
  TermFormPtr       tfp;
  /*
  DocUid            tmp;
  Int2              typ_ml;
  DocUidPtr         uids;
  */

  tfp = (TermFormPtr) GetObjectExtra (b);
  if (tfp == NULL) return;
  if (tfp->retrieveMode == EVAL_MODE) {
    WatchCursor ();
    Update ();
    RecalculateChosen (tfp);
    ArrowCursor ();
    tfp->retrieveMode = FETCH_MODE;
    return;
  }
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL || egp->retrieveDocsProc == NULL) return;
  SafeSetTitle (tfp->retrieve, "Retrieve 0 Documents");
  SafeDisable (tfp->retrieve);
  bs = NULL;
  if (tfp->usingAdv) {
    Update ();
    tfp->chosenDb = tfp->currDb;
    curstr = SaveStringFromText (tfp->advBoolText);
    bs = EntrezTLEvalXString (curstr, tfp->chosenDb, -1, NULL, NULL);
    MemFree (curstr);
  } else {
    for (sdp = tfp->state; sdp != NULL; sdp = sdp->next) {
      sdp->state = STATE_OFF;
    }
    InvalDocument (tfp->chosen);
    Update ();
    bs = EntrezTLEvalX (tfp->elst);
  }
  if (bs == NULL) return;
  numLinks = BSLen (bs) / sizeof (DocUid);
  if (numLinks < 32766 /* EntrezGetUserMaxLinks () */ ) {
    lsp = LinkSetNew ();
    if (lsp != NULL) {
      lsp->num = numLinks;
      lsp->uids = MemNew ((size_t) (numLinks * sizeof(DocUid)));
      BSSeek (bs, 0L, 0);
      BSRead (bs, lsp->uids, (numLinks * sizeof(DocUid)));
      /*
      typ_ml = DatabaseFromName ("MEDLINE");
      if (tfp->chosenDb == typ_ml) {
        i = 0;
        j = numLinks - 1;
        uids = lsp->uids;
        while (i < j) {
          tmp = uids [i];
          uids [i] = uids [j];
          uids [j] = tmp;
          i++;
          j--;
        }
      }
      */
      egp->retrieveDocsProc (tfp->form, (Int2) numLinks, 0, lsp->uids, tfp->chosenDb);
      LinkSetFree (lsp);
    }
  }
  BSFree (bs);
  Update ();
}

static void LoadChosen (TermFormPtr tfp, CharPtr strs, Int2 state,
                        Boolean force, Int4 num, Boolean nofield)

{
  ValNodePtr    elst;
  Boolean       found;
  StateDataPtr  prev;
  RecT          r;
  BaR           sb;
  StateDataPtr  sdp;
  Int4          special;
  Char          strn [256];
  Int4          total;

  if (force) {
    found = TRUE;
  } else if (tfp->currMod == RANGE_MODE) {
    found = TRUE;
  } else {
    found = EntrezFindTerm (tfp->currDb, tfp->currFld, strs, &special, &total);
  }
  if (found) {
    total = 0;
    if (force) {
      total = num;
    } else {
      elst = EntrezTLNew (tfp->currDb);
      SmartEntrezTLAddTerm (elst, strs, tfp->currDb, tfp->currFld, FALSE);
      total = EntrezTLEvalCount (elst);
      EntrezTLFree (elst);
    }
    if (total > 0) {
      for (sdp = tfp->state; sdp != NULL; sdp = sdp->next) {
        if (MeshStringICmp (sdp->term, strs) == 0 &&
            sdp->db == tfp->currDb && sdp->fld == tfp->currFld) {
          return;
        }
      }
      (tfp->numChosenLines)++;
      sdp = StateDataNew (tfp->state);
      if (tfp->state == NULL) {
        tfp->state = sdp;
      }
      if (sdp != NULL) {
        sdp->term = StringSave (strs);
        sdp->count = total;
        sdp->db = tfp->currDb;
        if (force) {
          sdp->fld = 0;
        } else {
          sdp->fld = tfp->currFld;
        }
        if (nofield) {
          sdp->field = StringSave ("----");
        } else {
          sdp->field = StringSave (TagFromField (sdp->fld));
        }
        sdp->group = GROUP_SINGLE;
        sdp->above = OP_NONE;
        prev = sdp->prev;
        if (prev != NULL) {
          sdp->above = OP_AND;
          prev->below = OP_AND;
        }
        sdp->below = OP_NONE;
        sdp->state = state;
        sprintf (strn, "%s\t[%s]\t%ld\n", strs, sdp->field, (long) total);
        if (tfp->numChosenLines <= 6) {
          ReplaceText (tfp->chosen, tfp->numChosenLines, strn,
                       &chosenParFmt, chosenColFmt, systemFont);
        } else {
          InsertText (tfp->chosen, tfp->numChosenLines, strn,
                      &chosenParFmt, chosenColFmt, systemFont);
        }
        InvalDocRows (tfp->chosen, tfp->numChosenLines, 1, 1);
        AdjustDocScroll (tfp->chosen);
        sb = GetSlateVScrollBar ((SlatE) tfp->chosen);
        ResetClip ();
        SetBarValue (sb, MAX (tfp->numChosenLines - 7, 0));
        ObjectRect (tfp->chosen, &r);
        InsetRect (&r, 4, 4);
        r.right = r.left + 16;
        InsetRect (&r, -1, -1);
        Select (tfp->chosen);
        InvalRect (&r);
      }
      Update ();
    }
  }
}

static void RepopulateChosen (TermFormPtr tfp)

{
  Int2          i;
  Int4          off;
  BaR           sb;
  StateDataPtr  sdp;
  Char          strn [256];

  if (tfp == NULL) return;
  sb = GetSlateVScrollBar ((SlatE) tfp->chosen);
  off = GetBarValue (sb);
  Reset (tfp->chosen);
  for (i = 0; i < 7; i++) {
    AppendText (tfp->chosen, " \n", &chosenParFmt, chosenColFmt, systemFont);
  }
  InvalDocument (tfp->chosen);
  tfp->numChosenLines = 0;
  for (sdp = tfp->state; sdp != NULL; sdp = sdp->next) {
    (tfp->numChosenLines)++;
    sprintf (strn, "%s\t[%s]\t%ld\n", sdp->term, sdp->field, (long) sdp->count);
    if (tfp->numChosenLines <= 6) {
      ReplaceText (tfp->chosen, tfp->numChosenLines, strn,
                   &chosenParFmt, chosenColFmt, systemFont);
    } else {
      InsertText (tfp->chosen, tfp->numChosenLines, strn,
                  &chosenParFmt, chosenColFmt, systemFont);
    }
  }
  InvalDocument (tfp->chosen);
  AdjustDocScroll (tfp->chosen);
  CorrectBarValue (sb, off);
  Update ();
}

static void AlphabetizeChosenGroups (TermFormPtr tfp)

{
  Int2          compare;
  Boolean       keepGoing;
  StateDataPtr  next;
  StateDataPtr  sdp;
  StateData     tmp;

  if (tfp == NULL || tfp->state == NULL) return;
  keepGoing = TRUE;
  while (keepGoing) {
    keepGoing = FALSE;
    for (sdp = tfp->state; sdp->next != NULL; sdp = sdp->next) {
      next = sdp->next;
      if (sdp->group == GROUP_FIRST || sdp->group == GROUP_MIDDLE) {
        compare = MeshStringICmp (sdp->term, next->term);
        if (compare > 0) {
          tmp.term = next->term;
          tmp.field = next->field;
          tmp.count = next->count;
          tmp.db = next->db;
          tmp.fld = next->fld;
          next->term = sdp->term;
          next->field = sdp->field;
          next->count = sdp->count;
          next->db = sdp->db;
          next->fld = sdp->fld;
          sdp->term = tmp.term;
          sdp->field = tmp.field;
          sdp->count = tmp.count;
          sdp->db = tmp.db;
          sdp->fld = tmp.fld;
          keepGoing = TRUE;
        }
      }
    }
  }
}

static ValNodePtr  termStrings = NULL;

static CharPtr MergeTermStrings (void)

{
  size_t      len;
  CharPtr     str;
  CharPtr     to;
  ValNodePtr  vnp;

  len = 0;
  vnp = termStrings;
  while (vnp != NULL) {
    len += StringLen ((CharPtr) vnp->data.ptrvalue);
    vnp = vnp->next;
  }
  str = (CharPtr) MemNew (len + 4);
  if (str != NULL) {
    vnp = termStrings;
    to = str;
    while (vnp != NULL) {
      to = StringMove (to, (CharPtr) vnp->data.ptrvalue);
      vnp = vnp->next;
    }
  }
  termStrings = ValNodeFreeData (termStrings);
  return str;
}

static Boolean ReadTermProc (CharPtr term, Int4 special, Int4 total)

{
  Char  str [256];
  Char  tmp [16];

  StringNCpy_0 (str, term, sizeof (str) - 16);
  sprintf (tmp, "\t%ld\n", (long) total);
  StringCat (str, tmp);
  ValNodeCopyStr (&termStrings, 0, str);
  MemFree (term);
  return TRUE;
}

static CharPtr FetchFromTermList (DoC d, Int2 item, Pointer ptr)

{
  Int2         numRead;
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (d);
  if (tfp == NULL) return NULL;
  if (item < 1) return NULL;
  termStrings = NULL;
  numRead = EntrezTermListByPage (tfp->currDb, tfp->currFld,
                                  item - 1, 1, ReadTermProc);
  return MergeTermStrings ();
}

static Boolean ReadOneProc (CharPtr term, Int4 special, Int4 total)

{
  MemFree (term);
  return FALSE;
}

static void ChangeMeshSlashSymbol (CharPtr str)

{
  Char     ch;
  CharPtr  ptr;

  if (str == NULL) return;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '/') {
      *ptr = '\31';
    }
    ptr++;
    ch = *ptr;
  }
}

/* FindLineOfText assumes a string of text separated by newline,
   characters, and terminated by a newline */

static Int2 FindLineOfText (CharPtr text, CharPtr str)

{
  Char     ch;
  Int2     compare;
  Int2     idx;
  CharPtr  PNTR index;
  Int2     left;
  CharPtr  lookfor;
  size_t   len;
  Int2     mid;
  Int2     numLines;
  CharPtr  ptr;
  Int2     right;

  mid = 0;
  if (StringLen (text) == 0 || StringLen (str) == 0) return 0;
  lookfor = StringSave (str);
  ChangeMeshSlashSymbol (lookfor);
  numLines = 0;
  ptr = text;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '\n') {
      numLines++;
    }
    ptr++;
    ch = *ptr;
  }
  index = MemNew (sizeof (CharPtr) * (size_t) (numLines + 3));
  if (index != NULL) {
    idx = 0;
    ptr = text;
    ch = *ptr;
    index [idx] = ptr;
    while (ch != '\0') {
      if (ch == '\n') {
        idx++;
        *ptr = '\0';
        ptr++;
        ch = *ptr;
        index [idx] = ptr;
      } else {
        ptr++;
        ch = *ptr;
      }
    }
  }
  if (numLines > 0) {
    left = 1;
    right = numLines;
    while (left <= right) {
      mid = (left + right) / 2;
      compare = StringICmp (lookfor, index [mid - 1]);
      if (compare <= 0) {
        right = mid - 1;
      }
      if (compare >= 0) {
        left = mid + 1;
      }
    }
    if (left <= right + 1) {
      len = StringLen (lookfor);
      compare = StringNICmp (lookfor, index [mid - 1], len);
      if (compare > 0) {
        mid++;
        if (mid <= numLines) {
          compare = StringNICmp (lookfor, index [mid - 1], len);
          if (compare > 0) {
            mid = 0;
          }
        } else {
          mid = 0;
        }
      }
    }
  }
  MemFree (index);
  MemFree (lookfor);
  return mid;
}

static void ChangeUnderscoreToSpace (CharPtr str)

{
  Char     ch;
  CharPtr  ptr;

  if (str == NULL) return;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    if (ch == '_') {
      *ptr = ' ';
    }
    ptr++;
    ch = *ptr;
  }
}

static void ScrollToText (TermFormPtr tfp, CharPtr str, Int2 page,
                          Boolean hard, Boolean exact)

{
  Int2     compare;
  size_t   len;
  Int4     numLines;
  Int2     oldItem;
  Int2     oldRow;
  Int2     perfect;
  Int2     row;
  Int4     startsAt;
  BaR      sb;
  Char     temp [256];
  CharPtr  text;

  if (page == 0) return;
  StringNCpy_0 (temp, str, sizeof (temp));
  ChangeUnderscoreToSpace (temp);
  ChangeMeshSlashSymbol (temp);
  text = GetDocText (tfp->avail, page, 0, 1); /* forces format if not before */
  GetItemParams4 (tfp->avail, page, &startsAt, NULL, NULL, NULL, NULL);
  GetDocParams4 (tfp->avail, NULL, &numLines);
  ChangeMeshSlashSymbol (text);
  row = FindLineOfText (text, temp);
  MemFree (text);
  if (row < 1 || row > numLines) {
    row = 1;
  }
  startsAt += row - 1;
  sb = GetSlateVScrollBar ((SlatE) tfp->avail);
  CorrectBarMax (sb, numLines - 7);
  if (! RowIsVisible (tfp->avail, page, row, NULL, NULL)) {
    ForceFormat (tfp->avail, page); /* forces UpdateLineStarts */
    GetItemParams4 (tfp->avail, page, &startsAt, NULL, NULL, NULL, NULL);
    GetDocParams4 (tfp->avail, NULL, &numLines);
    startsAt += row - 1;
    sb = GetSlateVScrollBar ((SlatE) tfp->avail);
    CorrectBarMax (sb, numLines - 7);
    if (hard) {
      SetBarValue (sb, startsAt);
    } else {
      CorrectBarValue (sb, startsAt);
    }
  }
  text = GetDocText (tfp->avail, page, row, 1);
  ChangeMeshSlashSymbol (text);
  perfect = StringICmp (text, temp);
  len = StringLen (temp);
  compare = StringNICmp (text, temp, len);
  MemFree (text);
  if (compare == 0) {
    oldItem = tfp->avItem;
    oldRow = tfp->avRow;
    if (oldItem != page || oldRow != row) {
      tfp->avItem = page;
      tfp->avRow = row;
      InvalDocRows (tfp->avail, oldItem, oldRow, oldRow);
      InvalDocRows (tfp->avail, tfp->avItem, tfp->avRow, tfp->avRow);
    }
    if (exact) {
      if (perfect == 0) {
        tfp->textChanged = FALSE;
      }
    } else {
      tfp->textChanged = FALSE;
    }
  } else {
    tfp->avItem = 0;
    tfp->avRow = 0;
  }
}

static Int2 SmartEntrezTermListByTerm (DocType type, DocField field, CharPtr term,
                                       Int2 numterms, TermListProc proc, Int2Ptr first_page)

{
  Char  temp [256];

  StringNCpy_0 (temp, term, sizeof (temp));
  ChangeUnderscoreToSpace (temp);
  return EntrezTermListByTerm (type, field, temp, numterms, proc, first_page);
}

static Boolean LoadAvailList (TermFormPtr tfp, CharPtr str)

{
  EntrezInfoPtr  eip;
  Int2           page = 0;
  Boolean        rsult;
  Int2           totalPages;

  rsult = FALSE;
  tfp->avItem = 0;
  tfp->avRow = 0;
  Reset (tfp->avail);
  SetDocCache (tfp->avail, NULL, NULL, NULL);
  Update ();
  if (str [0] != '\0') {
    eip = EntrezGetInfo ();
    if (eip != NULL) {
      totalPages = eip->types [tfp->currDb].fields [tfp->currFld].num_bucket;
    } else {
      totalPages = 0;
    }
    if (totalPages > 0) {
      if (StringHasNoText (str) && str [0] == ' ') {
        page = 0;
      } else {
        SmartEntrezTermListByTerm (tfp->currDb, tfp->currFld, str,
                                   1, ReadOneProc, &page);
      }
      /*
      for (i = 0; i < totalPages; i++) {
        AppendItem (tfp->avail, FetchFromTermList, NULL, FALSE, 30,
                    &availParFmt, availColFmt, systemFont);
      }
      */
      BulkAppendItem (tfp->avail, totalPages, FetchFromTermList, 30,
                      &availParFmt, availColFmt, systemFont);
      AppendText (tfp->avail, "\n\n\n\n\n\n\n\n\n\n\n\n \n",
                  &availParFmt, availColFmt, systemFont);
      /*
      twoBlankPages = "\n\n\n\n\n\n\n\n\n\n\n\n \n";
      AppendText (tfp->avail, twoBlankPages,
                  &availParFmt, availColFmt, systemFont);
      */
      page++;
      SetDocCache (tfp->avail, StdPutDocCache, StdGetDocCache, StdResetDocCache);
      tfp->currentPage = page;
      ScrollToText (tfp, str, page, FALSE, FALSE);
      InvalDocument (tfp->avail);
      Update ();
      AdjustDocScroll (tfp->avail);
    }
  }
  return rsult;
}

static void TruncateTerm (TermFormPtr tfp, CharPtr strs)

{
  if (StringChr (strs, '*') == NULL && StringChr (strs, '?') == NULL) {
    StringCat (strs, "...");
  }
  LoadChosen (tfp, strs, STATE_ON, FALSE, 0, FALSE);
}

static Boolean ReadAFew (TermFormPtr tfp, CharPtr str, CharPtr actual, size_t maxsize)

{
  Char     first [256];
  Boolean  found;
  Int2     page;
  CharPtr  ptr;
  CharPtr  txt;

  found = FALSE;
  termStrings = NULL;
  SmartEntrezTermListByTerm (tfp->currDb, tfp->currFld, str,
                             7, ReadTermProc, &page);
  if (termStrings != NULL) {
    StringNCpy_0 (first, termStrings->data.ptrvalue, sizeof (first));
    ptr = StringChr (first, '\t');
    if (ptr != NULL) {
      *ptr = '\0';
    }
    if (StringICmp (str, first) == 0) {
      found = TRUE;
    } else {
      StringNCpy_0 (actual, first, maxsize);
    }
  }
  txt = MergeTermStrings ();
  AppendText (tfp->avail, txt, &availParFmt, availColFmt, systemFont);
  MemFree (txt);
  if (found) {
    tfp->avItem = 1;
    tfp->avRow = 1;
  }
  InvalDocument (tfp->avail);
  return found;
}

static void ProcessMultipleTerms (TermFormPtr tfp, CharPtr strs)

{
  Char     actual [256];
  Char     ch;
  Boolean  found;
  Int2     i;
  Int2     j;
  Int2     k;
  Char     str [256];

  i = 0;
  while (StringLen (strs + i) > 0) {
    StringNCpy_0 (str, strs + i, sizeof (str));
    k = 0;
    ch = str [k];
    while (ch == ' ') {
      k++;
      ch = str [k];
    }
    j = 0;
    if (ch == '"') {
      k++;
      ch = str [j + k];
      while (ch != '\0' && ch != '"') {
        j++;
        ch = str [j + k];
      }
      if (ch == '"') {
        str [j + k] = '\0';
        i += j + k + 1;
      } else {
        i += j + k;
      }
    } else {
      while (ch != '\0' && ch != ' ') {
        j++;
        ch = str [j + k];
      }
      if (ch == ' ') {
        str [j + k] = '\0';
        i += j + k + 1;
      } else {
        i += j + k;
      }
    }
    if (StringLen (str + k) > 0) {
      tfp->avItem = 0;
      tfp->avRow = 0;
      Reset (tfp->avail);
      actual [0] = '\0';
      found = ReadAFew (tfp, str + k, actual, sizeof (actual));
      Update ();
      if (tfp->currMod == TRUNC_MULT_MODE) {
        TruncateTerm (tfp, str + k);
      } else if (found) {
        LoadChosen (tfp, str + k, STATE_ON, FALSE, 0, FALSE);
      } else {
        LoadChosen (tfp, actual, STATE_OFF, FALSE, 0, FALSE);
      }
    }
  }
  tfp->avItem = 0;
  tfp->avRow = 0;
  Reset (tfp->avail);
  for (i = 0; i < 7; i++) {
    AppendText (tfp->avail, " \n", &availParFmt, availColFmt, systemFont);
  }
  InvalDocument (tfp->avail);
  SafeSetTitle (tfp->term, "");
  SafeSetTitle (tfp->from, "");
  SafeSetTitle (tfp->to, "");
  SafeDisable (tfp->accept);
  if (Visible (tfp->term)) {
    Select (tfp->term);
  }
}

static void ProcessDirectLookup (TermFormPtr tfp, CharPtr str)

{
  DocSumPtr         dsp;
  EntrezGlobalsPtr  egp;
  LinkSetPtr        lsp;
  Int4              uid;

  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL || egp->retrieveDocsProc == NULL) return;
  uid = 0;
  if (tfp->currMod == LOOKUP_ACCN_MODE) {
    if (StringLen (str) > 0) {
      lsp = EntrezTLEvalString (str, tfp->currDb, tfp->currFld, NULL, NULL);
      if (lsp != NULL && lsp->num > 0 && lsp->uids != NULL) {
        uid = lsp->uids [0];
      }
      LinkSetFree (lsp);
    }
  } else if (tfp->currMod == LOOKUP_UID_MODE) {
    if (! StrToLong (str, &uid)) {
      Message (MSG_OK, "You must enter an integer");
      return;
    }
  } else return;
  if (uid > 0) {
    dsp = EntrezDocSum (tfp->currDb, uid);
    if (dsp != NULL) {
      DocSumFree (dsp);
      if (egp->lookupDirect) {
        if (egp->launchViewerProc != NULL) {
          egp->launchViewerProc (tfp->form, uid, 0, NULL, tfp->currDb);
        }
      } else {
        egp->retrieveDocsProc (tfp->form, 1, 0, &uid, tfp->currDb);
      }
    } else {
      Message (MSG_OK, "This record is not present in the database");
    }
  } else {
    if (tfp->currMod == LOOKUP_ACCN_MODE) {
      Message (MSG_OK, "This accession is not present in the database");
    } else {
      Message (MSG_OK, "This UID is not present in the database");
    }
  }
}

static void AcceptProc (ButtoN b)

{
  Char         ch;
  Int2         i;
  CharPtr      ptr;
  Char         str [256];
  CharPtr      text;
  TermFormPtr  tfp;
  Char         tmp [128];

  tfp = (TermFormPtr) GetObjectExtra (b);
  if (tfp == NULL) return;
  str [0] = '\0';
  if (tfp->currMod == RANGE_MODE) {
    GetTitle (CurrentText (), str, sizeof (str));
  } else {
    GetTitle (tfp->term, str, sizeof (str));
  }
  if (str [0] == '\0') return;
  WatchCursor ();
  switch (tfp->currMod) {
    case SELECTION_MODE :
      if (tfp->textChanged) {
        LoadAvailList (tfp, str);
        tfp->textChanged = FALSE;
        tfp->okayToAccept = FALSE;
      } else if (tfp->okayToAccept) {
        if (tfp->avItem > 0 && tfp->avRow > 0) {
          text = GetDocText (tfp->avail, tfp->avItem, tfp->avRow, 1);
          if (text != NULL) {
            ptr = text;
            ch = *ptr;
            while (ch != '\0' && ch >= ' ') {
              ptr++;
              ch = *ptr;
            }
            *ptr = '\0';
            SafeSetTitle (tfp->term, text);
            Select (tfp->term);
            Update ();
            StringNCpy_0 (str, text, sizeof (str));
          }
          MemFree (text);
          LoadChosen (tfp, str, STATE_ON, FALSE, 0, FALSE);
          Select (tfp->term);
          /* tfp->okayToAccept = FALSE; */
        }
      }
      break;
    case AUTOMATIC_MODE :
    case TRUNC_MULT_MODE :
      ProcessMultipleTerms (tfp, str);
      break;
    case TRUNCATION_MODE :
      TruncateTerm (tfp, str);
      tfp->avItem = 0;
      tfp->avRow = 0;
      Reset (tfp->avail);
      for (i = 0; i < 7; i++) {
        AppendText (tfp->avail, " \n", &availParFmt, availColFmt, systemFont);
      }
      InvalDocument (tfp->avail);
      SafeSetTitle (tfp->term, "");
      SafeDisable (tfp->accept);
      if (Visible (tfp->term)) {
        Select (tfp->term);
      }
      break;
    case MESH_TREE_MODE :
      ptr = str;
      while (*ptr != '\0' && *ptr != '{') {
        ptr++;
      }
      *ptr = '\0';
      LoadChosen (tfp, str, STATE_ON, FALSE, 0, FALSE);
    case TAXONOMY_MODE :
      LoadChosen (tfp, str, STATE_ON, FALSE, 0, FALSE);
      break;
    case RANGE_MODE :
      if (tfp->textChanged) {
        LoadAvailList (tfp, str);
        tfp->textChanged = FALSE;
        tfp->okayToAccept = FALSE;
      } else if (tfp->okayToAccept) {
        /*
        if (tfp->avItem > 0 && tfp->avRow > 0) {
          text = GetDocText (tfp->avail, tfp->avItem, tfp->avRow, 1);
          if (text != NULL) {
            ptr = text;
            ch = *ptr;
            while (ch != '\0' && ch >= ' ') {
              ptr++;
              ch = *ptr;
            }
            *ptr = '\0';
            SafeSetTitle (CurrentText (), text);
            Update ();
          }
          MemFree (text);
        }
        */
        GetTitle (tfp->from, str, sizeof (str));
        GetTitle (tfp->to, tmp, sizeof (tmp));
        StringCat (str, ":");
        StringCat (str, tmp);
        LoadChosen (tfp, str, STATE_ON, FALSE, 0, FALSE);
        Select (tfp->from);
        tfp->okayToAccept = FALSE;
      }
      break;
    case LOOKUP_ACCN_MODE :
    case LOOKUP_UID_MODE :
      ProcessDirectLookup (tfp, str);
      ArrowCursor ();
      Update ();
      return;
    default :
      break;
  }
  Update ();
  RecalculateChosen (tfp);
  ArrowCursor ();
  Update ();
}

static void AvailTimerProc (WindoW w)

{
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (w);
  if (tfp == NULL) return;
  tfp->okayToAccept = TRUE;
}

static void TextAction (TexT t)

{
  Int2         i;
  Char         str [256];
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (t);
  if (tfp == NULL) return;
  if (tfp->currMod == RANGE_MODE) {
    if (TextHasNoText (tfp->from) && TextHasNoText (tfp->to)) {
      SafeDisable (tfp->accept);
    } else {
      SafeEnable (tfp->accept);
    }
  } else {
    GetTitle (tfp->term, str, sizeof (str));
    if (StringHasNoText (str)) {
      if (tfp->currMod == SELECTION_MODE && str [0] == ' ') {
        SafeEnable (tfp->accept);
      } else {
        SafeDisable (tfp->accept);
      }
    } else {
      SafeEnable (tfp->accept);
    }
  }
  GetTitle (t, str, sizeof (str));
  if (str [0] == '\0') {
    tfp->avItem = 0;
    tfp->avRow = 0;
    Reset (tfp->avail);
    for (i = 0; i < 7; i++) {
      AppendText (tfp->avail, " \n", &availParFmt, availColFmt, systemFont);
    }
    InvalDocument (tfp->avail);
    tfp->textChanged = FALSE;
    tfp->okayToAccept = FALSE;
    tfp->currentPage = 0;
  } else {
    tfp->textChanged = TRUE;
    tfp->okayToAccept = FALSE;
    if (tfp->currentPage > 0) {
      ScrollToText (tfp, str, tfp->currentPage, TRUE, TRUE);
    }
    Update ();
  }
}

static void AddPopupItem (PopuP p, CharPtr str, Int2 maxwid)

{
  Char     ch;
  Int2     hbounds;
  Int2     i;
  CharPtr  ptr;
  Char     title [256];
  Int2     wid;

  if (p != NULL && str != NULL) {
    StringNCpy_0 (title, str, sizeof (title));
    i = StringLen (title);
    /*
    while (i < sizeof (title) - 1) {
      title [i] = ' ';
      i++;
    }
    */
    title [i] = '\0';
    ptr = title;
    ch = *ptr;
    while (ch != '\0') {
      if (ch == '/') {
        *ptr = '-';
      }
      ptr++;
      ch = *ptr;
    }
#ifdef WIN_MAC
    hbounds = 13;
#endif
#ifdef WIN_MSWIN
    hbounds = 13;
#endif
#ifdef WIN_MOTIF
    hbounds = 24;
#endif
    maxwid -= StringWidth ("...") + hbounds * 2 + StringWidth (" ");
    wid = 0;
    i = 0;
    ch = title [i];
    while (ch != '\0' && wid <= maxwid) {
      wid += CharWidth (ch);
      i++;
      ch = title [i];
    }
    title [i] = '\0';
    if (ch != '\0' && i <= (Int2) StringLen (str)) {
      StringCat (title, "...");
    }
    PopupItem (p, title);
  }
}

static Boolean RepopulateTaxonomy (TermFormPtr tfp, CharPtr taxterm)

{
  CharPtr                  canonicalForm;
  EntrezHierarchyChildPtr  childPtr;
  Int2                     delta;
  Int2                     fld_mesh_hier;
  Int2                     fld_orgn_hier;
  Int2                     i;
  Int2                     maxwidth;
  Boolean                  okay;
  RecT                     r;
  RecT                     s;
  Char                     str [256];
  EntrezHierarchyPtr       taxy;
  Int2                     wid;

  if (tfp == NULL) return FALSE;
  okay = FALSE;
  taxy = NULL;
  if (tfp->currMod == TAXONOMY_MODE) {
    fld_orgn_hier = FieldFromTag ("ORGN");
    taxy = EntrezHierarchyGet (taxterm, tfp->currDb, fld_orgn_hier);
  } else if (tfp->currMod == MESH_TREE_MODE) {
    fld_mesh_hier = FieldFromTag ("MESH");
    taxy = EntrezHierarchyGet (taxterm, tfp->currDb, fld_mesh_hier);
  }
  if (taxy != NULL) {
    okay = TRUE;
    SafeSetTitle (tfp->term, taxterm);
    Hide (tfp->taxParents);
    Reset (tfp->taxParents);
    tfp->taxStrings = ValNodeFreeData (tfp->taxStrings);
    tfp->avItem = 0;
    tfp->avRow = 0;
    Reset (tfp->avail);
    SetDocCache (tfp->avail, NULL, NULL, NULL);
    Update ();
    maxwidth = availColFmt [0].pixWidth;
    for (i = 0; i < taxy->numInLineage; i++) {
      AddPopupItem (tfp->taxParents, taxy->lineage [i], maxwidth);
      ValNodeCopyStr (&(tfp->taxStrings), 0, taxy->lineage [i]);
    }
    canonicalForm = taxy->canonicalForm;
    if (canonicalForm != NULL && *canonicalForm != '\0') {
      AddPopupItem (tfp->taxParents, canonicalForm, maxwidth);
      ValNodeCopyStr (&(tfp->taxStrings), 0, canonicalForm);
    } else {
      AddPopupItem (tfp->taxParents, taxterm, maxwidth);
      ValNodeCopyStr (&(tfp->taxStrings), 0, taxterm);
    }
    SetValue (tfp->taxParents, taxy->numInLineage + 1);
    ObjectRect (tfp->taxParents, &r);
    ObjectRect (tfp->avail, &s);
    InsetRect (&s, 4, 4);
    wid = r.right - r.left;
    delta = (maxwidth - wid) / 2;
    r.left = s.left + delta;
    r.right = r.left + wid;
    SetPosition (tfp->taxParents, &r);
    childPtr = taxy->children;
    for (i = 0; i < taxy->numChildren; i++) {
      sprintf (str, "%s\t%ld\t%d\n", childPtr->name, (long) (childPtr->total),
               (int) (childPtr->isLeafNode ? 1 : 0));
      AppendText (tfp->avail, str, &availParFmt, availColFmt, systemFont);
      childPtr++;
    }
    for (i = taxy->numChildren; i < 7; i++) {
      AppendText (tfp->avail, " \n", &availParFmt, availColFmt, systemFont);
    }
    InvalDocument (tfp->avail);
    Show (tfp->taxParents);
    Update ();
    AdjustDocScroll (tfp->avail);
    EntrezHierarchyFree (taxy);
  } else {
    SafeSetTitle (tfp->term, taxterm);
    Hide (tfp->taxParents);
    Reset (tfp->taxParents);
    tfp->taxStrings = ValNodeFreeData (tfp->taxStrings);
    tfp->avItem = 0;
    tfp->avRow = 0;
    Reset (tfp->avail);
    SetDocCache (tfp->avail, NULL, NULL, NULL);
    Show (tfp->taxParents);
    Update ();
  }
  return okay;
}

static void RepopulateTaxonomyRoot (TermFormPtr tfp)

{
  if (tfp == NULL) return;
  if (tfp->currDb == DatabaseFromName ("MEDLINE")) {
    RepopulateTaxonomy (tfp, ".MeSH Root.");
  } else {
    RepopulateTaxonomy (tfp, "root");
  }
}

static void ChangeTaxParents (PopuP p)

{
  TermFormPtr  tfp;
  Int2         val;
  ValNodePtr   vnp;

  tfp = (TermFormPtr) GetObjectExtra (p);
  if (tfp == NULL) return;
  val = GetValue (p);
  if (val > 0) {
    vnp = tfp->taxStrings;
    while (val > 1 && vnp != NULL) {
      val--;
      vnp = vnp->next;
    }
    if (vnp != NULL) {
      WatchCursor ();
      RepopulateTaxonomy (tfp, (CharPtr) vnp->data.ptrvalue);
      ArrowCursor ();
    }
  }
}

static Uint1  andsign [] = {
  0x30, 0x48, 0x50, 0x20, 0x50, 0x8A, 0x84, 0x8A, 0x71, 0x00
};

static Uint1  notsign [] = {
  0x00, 0x00, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00
};

static void DrawChosenBrackets (DoC d, RectPtr r, Int2 item, Int2 frst)

{
  RecT          s;
  StateDataPtr  sdp;
  TermFormPtr   tfp;

  tfp = (TermFormPtr) GetObjectExtra (d);
  if (tfp == NULL) return;
  sdp = tfp->state;
  while (sdp != NULL && item > 1) {
    sdp = sdp->next;
    item--;
  }
  if (sdp == NULL) return;
  switch (sdp->group) {
    case GROUP_SINGLE:
      /*
      MoveTo (r->left + 16 + 1, r->top + 1);
      LineTo (r->left + 16 + 3, r->top + 1);
      MoveTo (r->left + 16 + 1, r->top + 2);
      LineTo (r->left + 16 + 3, r->top + 2);
      MoveTo (r->left + 16 + 1, r->top + 1);
      LineTo (r->left + 16 + 1, r->bottom - 1);
      MoveTo (r->left + 16 + 1, r->bottom - 2);
      LineTo (r->left + 16 + 3, r->bottom - 2);
      MoveTo (r->left + 16 + 1, r->bottom - 1);
      LineTo (r->left + 16 + 3, r->bottom - 1);
      */
      break;
    case GROUP_FIRST:
      MoveTo (r->left + 16 + 1, r->top + 1);
      LineTo (r->left + 16 + 3, r->top + 1);
      MoveTo (r->left + 16 + 1, r->top + 2);
      LineTo (r->left + 16 + 3, r->top + 2);
      MoveTo (r->left + 16 + 1, r->top + 1);
      LineTo (r->left + 16 + 1, r->bottom);
      break;
    case GROUP_MIDDLE:
      MoveTo (r->left + 16 + 1, r->top);
      LineTo (r->left + 16 + 1, r->bottom);
      break;
    case GROUP_LAST:
      MoveTo (r->left + 16 + 1, r->top);
      LineTo (r->left + 16 + 1, r->bottom - 1);
      MoveTo (r->left + 16 + 1, r->bottom - 2);
      LineTo (r->left + 16 + 3, r->bottom - 2);
      MoveTo (r->left + 16 + 1, r->bottom - 1);
      LineTo (r->left + 16 + 3, r->bottom - 1);
      break;
    default:
      break;
  }
  switch (sdp->above) {
    case OP_NONE:
      break;
    case OP_AND:
      LoadRect (&s, r->left + 6, r->top, r->left + 14, r->top + 5);
      CopyBits (&s, andsign + 5);
      break;
    case OP_NOT:
      LoadRect (&s, r->left, r->top, r->left + 8, r->top + 5);
      CopyBits (&s, notsign + 5);
      break;
    default:
      break;
  }
  switch (sdp->below) {
    case OP_NONE:
      break;
    case OP_AND:
      LoadRect (&s, r->left + 6, r->bottom - 5, r->left + 14, r->bottom);
      CopyBits (&s, andsign);
      break;
    case OP_NOT:
      LoadRect (&s, r->left, r->bottom - 5, r->left + 8, r->bottom);
      CopyBits (&s, notsign);
      break;
    default:
      break;
  }
}

static Boolean HighlightChosen (DoC d, Int2 item, Int2 row, Int2 col)

{
  StateDataPtr  sdp;
  TermFormPtr   tfp;

  tfp = (TermFormPtr) GetObjectExtra (d);
  if (tfp == NULL) return FALSE;
  if (col == 1 || col == 2) {
    if (tfp->inAboveBox || tfp->inBelowBox) return FALSE;
    /* if (item == tfp->chItem) return TRUE; */
    return FALSE;
  } else if (col == 3) {
    sdp = tfp->state;
    while (sdp != NULL && item > 1) {
      sdp = sdp->next;
      item--;
    }
    if (sdp == NULL) return FALSE;
    return (Boolean) (sdp->state == STATE_ON);
  } else return FALSE;
}

static void FrameChosen (TermFormPtr tfp)

{
  RecT  sr;

  if (tfp == NULL) return;
  ObjectRect (tfp->chosen, &sr);
  InsetRect (&sr, 4, 4);
  if (RectInRect (&(tfp->trackRect), &sr)) {
    Dotted ();
    FrameRect (&(tfp->trackRect));
  }
}

static void ClickChosen (DoC d, PoinT pt)

{
  Int2         col;
  Int2         item;
  RecT         r;
  Int2         row;
  RecT         s;
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (d);
  if (tfp == NULL) return;
  MapDocPoint (d, pt, &item, &row, &col, &r);
  if (item > 0 && row == 0 && col > 0) {
    row = 1;
  }
  tfp->chClickItem = item;
  tfp->chClickRow = row;
  tfp->chClickCol = col;
  tfp->wasDoubleClick = dblClick;
  tfp->inAboveBox = FALSE;
  tfp->inBelowBox = FALSE;
  if ((col == 1 || col == 2) && item > 0 && item <= tfp->numChosenLines && row > 0) {
    tfp->chItem = item;
    LoadRect (&s, r.left, r.top, r.left + 16, r.top + 5);
    if (PtInRect (pt, &s)) {
      tfp->inAboveBox = TRUE;
    }
    LoadRect (&s, r.left, r.bottom - 5, r.left + 16, r.bottom);
    if (PtInRect (pt, &s)) {
      tfp->inBelowBox = TRUE;
    }
    if (tfp->inAboveBox || tfp->inBelowBox) return;
    InvalDocCols (tfp->chosen, item, 1, 2);
    Update ();
    r.right = r.left + chosenColFmt [0].pixWidth + chosenColFmt [1].pixWidth;
    LoadRect (&(tfp->trackRect), r.left + 22, r.top, r.right - 2, r.bottom);
    tfp->trackPt = pt;
    InvertMode ();
    FrameChosen (tfp);
  } else {
    tfp->chItem = 0;
  }
}

static void DragChosen (DoC d, PoinT pt)

{
  Int4         off;
  BaR          sb;
  RecT         sr;
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (d);
  if (tfp == NULL) return;
  if (tfp->chItem == 0) return;
  if (tfp->inAboveBox || tfp->inBelowBox) return;
  InvertMode ();
  FrameChosen (tfp);
  Update ();
  ObjectRect (tfp->chosen, &sr);
  InsetRect (&sr, 4, 4);
  if (PtInRect (pt, &sr)) {
    OffsetRect (&(tfp->trackRect), 0, pt.y - tfp->trackPt.y);
    tfp->trackPt = pt;
  }
  sb = GetSlateVScrollBar ((SlatE) tfp->chosen);
  off = GetBarValue (sb);
  ResetClip ();
  if (pt.y < sr.top) {
    SetBarValue (sb, off - 1);
  } else if (pt.y > sr.bottom) {
    SetBarValue (sb, off + 1);
  }
  Update ();
  InvertMode ();
  FrameChosen (tfp);
  Update ();
}

static void FlipOperatorBelow (TermFormPtr tfp, Int2 item)

{
  StateDataPtr  next;
  RecT          r;
  StateDataPtr  sdp;

  if (tfp == NULL) return;
  sdp = tfp->state;
  while (item > 1 && sdp != NULL) {
    item--;
    sdp = sdp->next;
  }
  if (sdp != NULL) {
    next = sdp->next;
    if (next != NULL) {
      if (sdp->below != next->above) {
        Beep ();
      }
      if (sdp->below == OP_AND) {
        sdp->below = OP_NOT;
        next->above = OP_NOT;
      } else if (sdp->below == OP_NOT) {
        sdp->below = OP_AND;
        next->above = OP_AND;
      }
      ObjectRect (tfp->chosen, &r);
      InsetRect (&r, 4, 4);
      r.right = r.left + 16;
      Select (tfp->chosen);
      InvalRect (&r);
      ResetClip ();
      WatchCursor ();
      Update ();
      RecalculateChosen (tfp);
      ArrowCursor ();
    }
  }
}

static void ReleaseChosen (DoC d, PoinT pt)

{
  Int2          col;
  Boolean       goingDown;
  Int2          inval;
  Int2          item;
  Boolean       lastDraggedDown;
  Boolean       merge;
  StateDataPtr  next;
  Int2          oldItem;
  Int2          op;
  StateDataPtr  prev;
  RecT          r;
  Int2          row;
  RecT          s;
  StateDataPtr  sdp;
  RecT          sr;
  TermFormPtr   tfp;
  StateDataPtr  tmp;

  tfp = (TermFormPtr) GetObjectExtra (d);
  if (tfp == NULL) return;
  MapDocPoint (d, pt, &item, &row, &col, &r);
  if (item > 0 && row == 0 && col > 0) {
    row = 1;
  }
  if (tfp->inAboveBox || tfp->inBelowBox) {
    LoadRect (&s, r.left, r.top, r.left + 16, r.top + 5);
    if (PtInRect (pt, &s)) {
      if (tfp->inAboveBox && item == tfp->chItem) {
        FlipOperatorBelow (tfp, tfp->chItem - 1);
      } else if (tfp->inBelowBox && item == tfp->chItem - 1) {
        FlipOperatorBelow (tfp, tfp->chItem - 1);
      }
    }
    LoadRect (&s, r.left, r.bottom - 5, r.left + 16, r.bottom);
    if (PtInRect (pt, &s)) {
      if (tfp->inBelowBox && item == tfp->chItem) {
        FlipOperatorBelow (tfp, tfp->chItem);
      } else if (tfp->inAboveBox && item == tfp->chItem + 1) {
        FlipOperatorBelow (tfp, tfp->chItem);
      }
    }
    return;
  }
  if (tfp->chItem > 0) {
    oldItem = tfp->chItem;
    tfp->chItem = 0;
    InvertMode ();
    FrameChosen (tfp);
    Update ();
    InvalDocCols (tfp->chosen, oldItem, 1, 2);
    Update ();
    if (item == oldItem) return;
    if (tfp->numChosenLines < 2) return;
    lastDraggedDown = FALSE;
    if (oldItem == tfp->numChosenLines && item > tfp->numChosenLines) {
      lastDraggedDown = TRUE;
    }
    merge = TRUE;
    ObjectRect (tfp->chosen, &sr);
    InsetRect (&sr, 4, 4);
    if (item > tfp->numChosenLines && PtInRect (pt, &sr)) {
      merge = FALSE;
      item = INT2_MAX;
    }
    if (item > 0 && tfp->state != NULL) {
      goingDown = FALSE;
      if (item > oldItem) {
        item--;
        goingDown = TRUE;
      }
      sdp = tfp->state;
      while (oldItem > 1 && sdp != NULL) {
        oldItem--;
        sdp = sdp->next;
      }
      if (sdp != NULL) {
        next = sdp->next;
        prev = sdp->prev;
        switch (sdp->group) {
          case GROUP_SINGLE :
            if (lastDraggedDown) return;
            break;
          case GROUP_FIRST :
            if (next != NULL) {
              if (next->group == GROUP_MIDDLE) {
                next->group = GROUP_FIRST;
              } else if (next->group == GROUP_LAST) {
                next->group = GROUP_SINGLE;
              }
            }
            break;
          case GROUP_MIDDLE :
            break;
          case GROUP_LAST :
            if (prev != NULL) {
              if (prev->group == GROUP_MIDDLE) {
                prev->group = GROUP_LAST;
              } else if (prev->group == GROUP_FIRST) {
                prev->group = GROUP_SINGLE;
              }
            }
            break;
          default :
            break;
        }
        sdp->prev = NULL;
        sdp->next = NULL;
        if (prev != NULL) {
          prev->next = next;
        }
        if (next != NULL) {
          next->prev = prev;
        }
        if (prev == NULL) {
          tfp->state = next;
        }
        op = OP_NONE;
        switch (sdp->group) {
          case GROUP_SINGLE :
            if (goingDown) {
              if (prev != NULL) {
                op = prev->below;
              }
            } else {
              if (next != NULL) {
                op = next->above;
              }
            }
            if (prev != NULL) {
              prev->below = op;
            }
            if (next != NULL) {
              next->above = op;
            }
            break;
          case GROUP_FIRST :
            if (prev != NULL) {
              op = prev->below;
            }
            if (next != NULL) {
              next->above = op;
            }
            break;
          case GROUP_MIDDLE :
            break;
          case GROUP_LAST :
            if (next != NULL) {
              op = next->above;
            }
            if (prev != NULL) {
              prev->below = op;
            }
            break;
          default :
            break;
        }
      }
      tmp = sdp;
      sdp = tfp->state;
      if (sdp == NULL) {
        tfp->state = tmp;
      } else {
        while (item > 1 && sdp->next != NULL) {
          item--;
          sdp = sdp->next;
        }
        next = sdp->next;
        prev = sdp->prev;
        sdp->next = tmp;
        tmp->next = next;
        tmp->prev = sdp;
        if (next != NULL) {
          next->prev = tmp;
        }
        op = OP_NONE;
        if (prev != NULL) {
          op = prev->below;
        }
        sdp->above = op;
        tmp->below = sdp->below;
        sdp->below = OP_NONE;
        tmp->above = OP_NONE;
      }
      tmp->group = GROUP_SINGLE;
      if (merge) {
        switch (sdp->group) {
          case GROUP_SINGLE :
            sdp->group = GROUP_FIRST;
            tmp->group = GROUP_LAST;
            break;
          case GROUP_FIRST :
            tmp->group = GROUP_MIDDLE;
            break;
          case GROUP_MIDDLE :
            tmp->group = GROUP_MIDDLE;
            break;
          case GROUP_LAST :
            sdp->group = GROUP_MIDDLE;
            tmp->group = GROUP_LAST;
            break;
          default :
            break;
        }
      } else {
        prev = tmp->prev;
        if (prev != NULL) {
          prev->below = OP_AND;
          tmp->above = OP_AND;
        }
      }
      ResetClip ();
      WatchCursor ();
      Update ();
      AlphabetizeChosenGroups (tfp);
      RepopulateChosen (tfp);
      RecalculateChosen (tfp);
      ArrowCursor ();
    }
    return;
  }
  if (tfp->chClickItem == item && tfp->chClickRow == row &&
      tfp->chClickCol == col && col == 3) {
    inval = item;
    sdp = tfp->state;
    while (sdp != NULL && item > 1) {
      sdp = sdp->next;
      item--;
    }
    if (sdp == NULL) return;
    switch (sdp->state) {
      case STATE_OFF :
        sdp->state = STATE_ON;
        break;
      case STATE_ON :
        sdp->state = STATE_OFF;
        break;
      default :
        sdp->state = STATE_OFF;
        break;
    }
    ResetClip ();
    WatchCursor ();
    InvalDocCols (tfp->chosen, inval, 3, 3);
    Update ();
    RecalculateChosen (tfp);
    ArrowCursor ();
  }
}

static void AdvBoolTextProc (TexT t)

{
  CharPtr      curstr;
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (t);
  if (tfp == NULL) return;
  curstr = SaveStringFromText (tfp->advBoolText);
  tfp->retrieveMode = EVAL_MODE;
  SafeSetTitle (tfp->retrieve, "Evaluate");
  if (EntrezTLParseString (curstr, tfp->currDb, -1, NULL, NULL)) {
    SafeEnable (tfp->retrieve);
  } else {
    SafeDisable (tfp->retrieve);
  }
  MemFree (curstr);
}

static CharPtr MakeStringFromChosen (TermFormPtr tfp)

{
  CharPtr       fldStr;
  Int2          group;
  Int2          last;
  CharPtr       ptr;
  StateDataPtr  sdp;
  CharPtr       tmp;
  CharPtr       userString;

  if (tfp == NULL || tfp->state == NULL || tfp->usingAdv) return NULL;
  sdp = tfp->state;
  tfp->chosenDb = sdp->db;
  if (tfp->numChosenLines < 1 || tfp->chosenDb < 0) return NULL;
  userString = MemNew (1000);
  userString [0] = '\0';
  group = 0;
  last = 0;
  for (sdp = tfp->state; sdp != NULL; sdp = sdp->next) {
    if (sdp->group == GROUP_SINGLE || sdp->group == GROUP_FIRST) {
      group++;
    }
    if (sdp->state == STATE_ON) {
      if (last == 0) {
        StrCat (userString, "( ");
        StrCat (userString, "( ");
      } else if (last == group) {
        StrCat (userString, " | ");
      } else if (sdp->above == OP_NOT) {
        StrCat (userString, " )");
        StrCat (userString, " )");
        StrCat (userString, " - ");
        StrCat (userString, "( ");
        StrCat (userString, "( ");
      } else {
        StrCat (userString, " )");
        StrCat (userString, " & ");
        StrCat (userString, "( ");
      }
      StringCat (userString, "\"");
      tmp = StringSave (sdp->term);
      ptr = StringChr (tmp, ':');
      if (ptr != NULL) {
        *ptr = '\0';
        ptr++;
        TrimSpacesAroundString (tmp);
        TrimSpacesAroundString (ptr);
        StringCat (userString, tmp);
        StringCat (userString, "\" : \"");
        StringCat (userString, ptr);
      } else {
        StringCat (userString, tmp);
      }
      MemFree (tmp);
      StringCat (userString, "\" [");
      fldStr = EntrezFieldToString (tfp->chosenDb, sdp->fld);
      StringCat (userString, fldStr);
      StringCat (userString, "]");
      last = group;
    }
  }
  if (group > 0 && last > 0) {
    StrCat (userString, " )");
    StrCat (userString, " )");
  }
  return userString;
}

static int LIBCALLBACK SortByName (VoidPtr ptr1, VoidPtr ptr2)

{
  CharPtr     str1;
  CharPtr     str2;
  ValNodePtr  vnp1;
  ValNodePtr  vnp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      str1 = (CharPtr) vnp1->data.ptrvalue;
      str2 = (CharPtr) vnp2->data.ptrvalue;
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

#ifdef WIN_MAC
#define textObjectReturnChar '\015'
#else
#define textObjectReturnChar '\n'
#endif

static Boolean InsertAdvText (TermFormPtr tfp, CharPtr str)

{
  CharPtr curstr;
  EntrezInfoPtr eip;
  CharPtr finalstr;
  Int2 i;
  Boolean retval = TRUE;
  Int4 begin;
  Int4 end;
  Int4 curstrLen;
  Char textObjectReturnStr [3];
  Int2 strLen;
  ValNodePtr head = NULL, vnp;

  if (tfp == NULL) return TRUE;
  if (StringHasNoText (str)) {
    curstr = MemNew (512);
    if (curstr != NULL) {
      eip = EntrezGetInfo ();
      if (eip != NULL && eip->field_info != NULL) {
        StringCpy (curstr, "Field names are ");
        head = NULL;
        for (i = 0; i < eip->field_count; i++) {
          ValNodeAddStr (&head, 0, eip->field_info [i].tag);
        }
        head = SortValNode (head, SortByName);
        for (vnp = head; vnp != NULL; vnp = vnp->next) {
          StringCat (curstr, "[");
          StringCat (curstr, (CharPtr) vnp->data.ptrvalue);
          StringCat (curstr, "], ");
        }
        ValNodeFree (head);
        textObjectReturnStr [0] = textObjectReturnChar;
        textObjectReturnStr [1] = '\0';
        StringCat (curstr, "and [*] to search all fields. ");
        StringCat (curstr, "Operators are & (and), | (or), - (butnot), ");
        StringCat (curstr, "and : (range). For example:");
        StringCat (curstr, textObjectReturnStr);
        StringCat (curstr, "((\"glucagon\" [WORD] | \"insulin\" ");
        StringCat (curstr, "[MESH]) & (\"1995\" : \"1996\" [PDAT]))");
        SetTitle (tfp->advBoolText, curstr);
      }
    }
    MemFree (curstr);
    return TRUE;
  }
  curstr = MemNew ((curstrLen = TextLength (tfp->advBoolText)) + 1);
  GetTitle (tfp->advBoolText, curstr, curstrLen + 1);
  if (curstrLen == 0)
  {
    begin = 0;
    end = 0;
  } else {
    TextSelectionRange (tfp->advBoolText, &begin, &end);
  }

  if (begin >= 0 && end <= curstrLen && begin <= end)
  {
    strLen = StrLen (str);
    finalstr = MemNew (strLen + curstrLen + begin - end + 1);
    StrNCpy (finalstr, curstr, begin);
    StrCpy (&finalstr [begin], str);
    StrCat (finalstr, &curstr[end]);
    SetTitle (tfp->advBoolText, finalstr);
    MemFree (finalstr);
    SelectText (tfp->advBoolText, begin + strLen, begin + strLen);
    /* AdvBoolTextProc (tfp->advBoolText); */
  } else {
    SelectText (tfp->advBoolText, curstrLen, curstrLen);
    /* Beep(); */
    retval = FALSE;
  }

  MemFree(curstr);

  return retval;
}

static void ConvertChosenToAdvanced (TermFormPtr tfp)

{
  CharPtr  str;

  if (tfp == NULL) return;
  str = MakeStringFromChosen (tfp);
  InsertAdvText (tfp, str);
  MemFree (str);
  AdvBoolTextProc (tfp->advBoolText);
}

static Int4 GetValueFromField (DoC d, Int2 item, Int2 row, Int2 col)

{
  CharPtr  str;
  Int4     value;

  value = -1;
  str = GetDocText (d, item, row, col);
  if (str != NULL) {
    if (! StrToLong (str, &value)) {
      value = -1;
    }
  }
  MemFree (str);
  return value;
}

static void DrawAvailLeaf (DoC d, RectPtr r, Int2 item, Int2 frst)

{
  RecT  q;
  Int2  value;

  if (r == NULL || frst != 0) return;
  value = GetValueFromField (d, item, 1, 3);
  if (value == 1) {
    q = *r;
    q.left++;
    q.right = q.left + 4;
    q.top += stdLineHeight / 2 - 2;
    q.bottom = q.top + 4;
    value = GetValueFromField (d, item, 1, 6);
    if (value == STATE_ON) {
      InvertColors ();
      EraseRect (&q);
      PaintRect (&q);
      InvertColors ();
    } else {
      PaintRect (&q);
    }
  }
}

static Boolean HighlightAvail (DoC d, Int2 item, Int2 row, Int2 col)

{
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (d);
  if (tfp == NULL) return FALSE;
  if (item == tfp->avItem && row == tfp->avRow) return TRUE;
  return FALSE;
}

static void ClickAvail (DoC d, PoinT pt)

{
  Int2         item;
  Int2         row;
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (d);
  if (tfp == NULL) return;
  MapDocPoint (d, pt, &item, &row, NULL, NULL);
  tfp->avClickItem = item;
  tfp->avClickRow = row;
  tfp->wasDoubleClick = dblClick;
}

static void ReleaseAvail (DoC d, PoinT pt)

{
  Char         ch;
  Int2         item;
  Int2         olditem;
  Int2         oldrow;
  CharPtr      ptr;
  Int2         row;
  CharPtr      text;
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (d);
  if (tfp == NULL) return;
  MapDocPoint (d, pt, &item, &row, NULL, NULL);
  if (tfp->avClickItem != item || tfp->avClickRow != row) return;
  olditem = tfp->avItem;
  oldrow = tfp->avRow;
  tfp->avItem = item;
  tfp->avRow = row;
  if (olditem > 0 && oldrow > 0) {
    InvalDocRows (tfp->avail, olditem, oldrow, oldrow);
  }
  if (item > 0 && row > 0) {
    InvalDocRows (tfp->avail, item, row, row);
  }
  text = GetDocText (tfp->avail, item, row, 1);
  if (text != NULL) {
    ptr = text;
    ch = *ptr;
    while (ch != '\0' && ch >= ' ') {
      ptr++;
      ch = *ptr;
    }
    *ptr = '\0';
    if (tfp->currMod == RANGE_MODE) {
      if (CurrentText () == tfp->from) {
        SafeSetTitle (tfp->from, text);
        Select (tfp->from);
      } else if (CurrentText () == tfp->to) {
        SafeSetTitle (tfp->to, text);
        Select (tfp->to);
      }
    } else {
      SafeSetTitle (tfp->term, text);
      Select (tfp->term);
    }
    Update ();
  }
  if (tfp->wasDoubleClick) {
    WatchCursor ();
    Update ();
    if (tfp->currMod == TAXONOMY_MODE || tfp->currMod == MESH_TREE_MODE) {
      Update ();
      ResetClip ();   /* clipped to panel, need to update popup */
      RepopulateTaxonomy (tfp, text);
    } else if (tfp->currMod == RANGE_MODE) {
      ResetClip ();
      LoadChosen (tfp, text, STATE_ON, FALSE, 0, FALSE);
      Update ();
      RecalculateChosen (tfp);
    } else {
      ResetClip ();
      LoadChosen (tfp, text, STATE_ON, FALSE, 0, FALSE);
      Update ();
      RecalculateChosen (tfp);
    }
    ArrowCursor ();
  }
  MemFree (text);
}

static void ChangeMode (PopuP p)

{
  Char         ch;
  Int2         i;
  Int2         idx;
  Int2         md;
  Int2         mode;
  CharPtr      ptr;
  TermFormPtr  tfp;
  CharPtr      text;
  UIEnum       val;

  tfp = (TermFormPtr) GetObjectExtra (p);
  if (tfp == NULL || tfp->fldalists == NULL) return;
  mode = -1;
  for (md = 0; md < MAX_MODES; md++) {
    if (p == tfp->modes [md]) {
      mode = md;
    }
  }
  if (mode < 0) return;
  if (GetEnumPopup (p, mode_alists [mode], &val) && val < MAX_MODES) {
    mode = (Int2) val;
    tfp->currMod = mode;
    if (tfp->fieldModeValues != NULL && tfp->currFld < tfp->numflds) {
      idx = tfp->currDb * tfp->numflds + tfp->currFld;
      tfp->fieldModeValues [idx] = mode;
    }
    if (mode == TAXONOMY_MODE || mode == MESH_TREE_MODE) {
      SafeHide (tfp->termGrp);
      SafeHide (tfp->rangeGrp);
      text = SaveStringFromText (tfp->term);
      WatchCursor ();
      Update ();
      if (text != NULL) {
        ptr = text;
        ch = *ptr;
        while (ch != '\0' && ch >= ' ') {
          ptr++;
          ch = *ptr;
        }
        *ptr = '\0';
        if (*text == '\0' || (! RepopulateTaxonomy (tfp, text))) {
          RepopulateTaxonomyRoot (tfp);
        }
      } else {
        RepopulateTaxonomyRoot (tfp);
      }
      ArrowCursor ();
      MemFree (text);
      SafeShow (tfp->taxParents);
      tfp->textChanged = FALSE;
      tfp->okayToAccept = TRUE;
    } else if (tfp->currMod == RANGE_MODE) {
      SafeHide (tfp->termGrp);
      SafeHide (tfp->taxParents);
      SafeShow (tfp->rangeGrp);
      TextAction (tfp->from);
      tfp->avItem = 0;
      tfp->avRow = 0;
      Reset (tfp->avail);
      SetDocCache (tfp->avail, NULL, NULL, NULL);
      for (i = 0; i < 7; i++) {
        AppendText (tfp->avail, " \n", &availParFmt, availColFmt, systemFont);
      }
      InvalDocument (tfp->avail);
      Select (tfp->from);
    } else {
      SafeHide (tfp->rangeGrp);
      SafeHide (tfp->taxParents);
      SafeShow (tfp->termGrp);
      TextAction (tfp->term);
      tfp->avItem = 0;
      tfp->avRow = 0;
      Reset (tfp->avail);
      SetDocCache (tfp->avail, NULL, NULL, NULL);
      for (i = 0; i < 7; i++) {
        AppendText (tfp->avail, " \n", &availParFmt, availColFmt, systemFont);
      }
      InvalDocument (tfp->avail);
      Select (tfp->term);
    }
    Update ();
  }
}

static void ChangeField (PopuP p)

{
  Int2         db;
  Int2         fld;
  Int2         md;
  Int2         idx;
  TermFormPtr  tfp;
  UIEnum       val;

  tfp = (TermFormPtr) GetObjectExtra (p);
  if (tfp == NULL || tfp->fldalists == NULL) return;
  fld = -1;
  for (db = 0; db < tfp->numdbs; db++) {
    if (p == tfp->fields [db]) {
      fld = db;
    }
  }
  if (fld < 0) return;
  if (GetEnumPopup (p, tfp->fldalists [fld], &val) && val <= tfp->numflds) {
    for (md = 0; md < tfp->nummods; md++) {
      if (md != tfp->modeFromField [(Int2) val]) {
        SafeHide (tfp->modes [md]);
      }
    }
    fld = (Int2) val;
    tfp->currFld = fld;
    md = tfp->modeFromField [fld];
    if (tfp->fieldModeValues != NULL && fld < tfp->numflds) {
      idx = tfp->currDb * tfp->numflds + tfp->currFld;
      SetEnumPopup (tfp->modes [md], mode_alists [md],
                    (UIEnum) tfp->fieldModeValues [idx]);
    }
    SafeShow (tfp->modes [md]);
    ChangeMode (tfp->modes [md]);
  }
}

static void ChangeDatabase (PopuP p)

{
  Int2         db;
  Int2         dbase;
  TermFormPtr  tfp;
  UIEnum       val;

  tfp = (TermFormPtr) GetObjectExtra (p);
  if (tfp == NULL || tfp->dbalist == NULL) return;
  if (GetEnumPopup (p, tfp->dbalist, &val) && val < tfp->numdbs) {
    for (db = 0; db < tfp->numdbs; db++) {
      if (db != (Int2) val) {
        SafeHide (tfp->fields [db]);
      }
    }
    dbase = (Int2) val;
    tfp->currDb = dbase;
    SafeShow (tfp->fields [dbase]);
    ChangeField (tfp->fields [dbase]);
  }
  DoReset (tfp, FALSE);
}

extern void LoadNamedUidList (ForM f, CharPtr term, Int4 num, Int4Ptr uids, Int2 db)

{
  Char         str [64];
  WindoW       tempPort;
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (f);
  if (tfp == NULL) return;
  if (db >= tfp->numdbs) return;
  if (num <= 0 || uids == NULL) return;
  WatchCursor ();
  Update ();
  if (tfp->currDb != db) {
    SetEnumPopup (tfp->database, tfp->dbalist, (UIEnum) db);
    ChangeDatabase (tfp->database);
  } else {
    DoResetAvail (tfp, TRUE);
  }
  SafeSetTitle (tfp->term, "");
  SafeSetTitle (tfp->from, "");
  SafeSetTitle (tfp->to, "");
  SafeDisable (tfp->accept);
  if (StringHasNoText (term)) {
    term = "*Current_Documents";
  }
  if (term [0] == '*') {
    term++;
  }
  StringCpy (str, "*");
  StringNCpy (str + 1, term, sizeof (str) - 3);
  EntrezCreateNamedUidList (str, db, 0, num, uids);
  tempPort = SavePort (tfp->chosen);
  LoadChosen (tfp, str, STATE_ON, TRUE, num, TRUE);
  SafeHide (tfp->advBoolText);
  SafeShow (tfp->chosen);
  tfp->usingAdv = FALSE;
  Update ();
  RecalculateChosen (tfp);
  RestorePort (tempPort);
  ArrowCursor ();
  Update ();
  Show (tfp->form);
  Select (tfp->form);
}

extern Boolean UsingTextQuery (ForM f)

{
  TermFormPtr  tfp;

  tfp = (TermFormPtr) GetObjectExtra (f);
  if (tfp == NULL) return FALSE;
  return tfp->usingAdv;
}

static void QueryTypeProc (ChoicE c)

{
  TermFormPtr  tfp;

#ifdef WIN_MAC
  tfp = (TermFormPtr) currentFormDataPtr;
#else
  tfp = (TermFormPtr) GetObjectExtra (c);
#endif
  if (tfp == NULL) return;
  if (GetValue (c) == 1) {
    SafeHide (tfp->advBoolText);
    Reset (tfp->advBoolText);
    Update ();
    SafeShow (tfp->chosen);
    tfp->usingAdv = FALSE;
  } else {
    if (! tfp->usingAdv) {
      ConvertChosenToAdvanced (tfp);
    }
    SafeHide (tfp->chosen);
    SafeShow (tfp->advBoolText);
    /* AdvBoolTextProc (tfp->advBoolText); */
    tfp->usingAdv = TRUE;
  }
}

extern ChoicE CreateQueryTypeChoice (MenU m, BaseFormPtr bfp)

{
  ChoicE  c;

  c = ChoiceGroup (m, QueryTypeProc);
  SetObjectExtra (c, bfp, NULL);
  ChoiceItem (c, "Regular");
  ChoiceItem (c, "Advanced");
  SetValue (c, 1);
  return c;
}

static void ClearUnusedQueryTerms (IteM i)

{
  StateDataPtr  last;
  StateDataPtr  next;
  StateDataPtr  PNTR prev;
  StateDataPtr  sdp;
  TermFormPtr   tfp;

#ifdef WIN_MAC
  tfp = (TermFormPtr) currentFormDataPtr;
#else
  tfp = (TermFormPtr) GetObjectExtra (i);
#endif
  if (tfp == NULL || tfp->state == NULL || tfp->usingAdv) return;
  WatchCursor ();
  Update ();
  sdp = tfp->state;
  prev = &(tfp->state);
  while (sdp != NULL) {
    last = sdp->prev;
    next = sdp->next;
    if (sdp->state == STATE_ON) {
      prev = (StateDataPtr  PNTR) &(sdp->next);
    } else {
      switch (sdp->group) {
        case GROUP_SINGLE :
          break;
        case GROUP_FIRST :
          if (next != NULL) {
            if (next->group == GROUP_MIDDLE) {
              next->group = GROUP_FIRST;
            } else if (next->group == GROUP_LAST) {
              next->group = GROUP_SINGLE;
            }
          }
          break;
        case GROUP_MIDDLE :
          break;
        case GROUP_LAST :
          if (last != NULL) {
            if (last->group == GROUP_MIDDLE) {
              last->group = GROUP_LAST;
            } else if (last->group == GROUP_FIRST) {
              last->group = GROUP_SINGLE;
            }
          }
          break;
        default :
          break;
      }
      *prev = sdp->next;
      sdp->next = NULL;
      sdp->prev = NULL;
      StateDataFree (sdp);
    }
    sdp = next;
  }
  sdp = tfp->state;
  while (sdp != NULL) {
    last = sdp->prev;
    next = sdp->next;
    if (last == NULL) {
      sdp->above = OP_NONE;
    }
    if (next == NULL) {
      sdp->below = OP_NONE;
    }
    sdp = next;
  }
  AlphabetizeChosenGroups (tfp);
  RepopulateChosen (tfp);
  RecalculateChosen (tfp);
  ArrowCursor ();
}

extern IteM CreateClearUnusedItem (MenU m, BaseFormPtr bfp)

{
  IteM  i;

  i = CommandItem (m, "Clear Unused Query Terms", ClearUnusedQueryTerms);
  SetObjectExtra (i, bfp, NULL);
  return i;
}

static void CleanupEntrezTermListForm (GraphiC g, VoidPtr data)

{
  Int2         i;
  TermFormPtr  tfp;

  tfp = (TermFormPtr) data;
  if (tfp != NULL) {
    tfp->dbalist = FreeEnumFieldAlist (tfp->dbalist);
    if (tfp->fldalists != NULL) {
      for (i = 0; i < tfp->numdbs; i++) {
        tfp->fldalists [i] = FreeEnumFieldAlist (tfp->fldalists [i]);
      }
      MemFree (tfp->fldalists);
    }
    MemFree (tfp->fields);
    MemFree (tfp->modeFromField);
    MemFree (tfp->fieldModeValues);
    StateDataFree (tfp->state);
    MemFree (tfp->uids);
    ValNodeFreeData (tfp->taxStrings);
    EntrezTLFree (tfp->elst); 
  }
  StdCleanupFormProc (g, data);
}

static CharPtr blastnames [] = {
  "BLASTN", "BLASTP", "BLASTX", "TBLASTN", NULL
};

extern EnumFieldAssocPtr MakeDatabaseAlist (EntrezTypeInfo *type_info,
                                            short type_count, Boolean blast)

{
  EnumFieldAssocPtr  alist;
  EnumFieldAssocPtr  ap;
  Char               ch;
  Int2               i;
  Int2               j;
  CharPtr            ptr;

  if (type_info == NULL || type_count < 1) return NULL;
  alist = MemNew (sizeof (EnumFieldAssoc) * (type_count + 7));
  if (alist == NULL) return NULL;
  ap = alist;
  i = 0;
  while (i < type_count) {
    ap->name = StringSave (type_info [i].name);
    ptr = StringStr (ap->name, " (");
    if (ptr != NULL) {
      *ptr = '\0';
    }
    if (ap->name != NULL) {
      ptr = ap->name;
      ch = *ptr;
      while (ch != '\0') {
        if (ch == '/') {
          *ptr = '-';
        }
        ptr++;
        ch = *ptr;
      }
    }
    ap->value = i;
    ap++;
    i++;
  }
  if (blast) {
    i = ALIST_BLASTN;
    j = 0;
    while (blastnames [j] != NULL) {
      ap->name = StringSave (blastnames [j]);
      ap->value = i;
      ap++;
      i++;
      j++;
    }
  }
  ap->name = NULL;
  return alist;
}

extern EnumFieldAssocPtr MakeFieldAlist (EntrezTypeData *types,
                                         EntrezFieldInfo *field_info,
                                         short field_count, Int2 db,
                                         Boolean allFields)

{
  EnumFieldAssocPtr  alist;
  EnumFieldAssocPtr  ap;
  Char               ch;
  EntrezGlobalsPtr   egp;
  Int2               fld;
  Int2               fld_prot;
  CharPtr            ptr;
  Boolean            sortit;
  Int2               typ_aa;
  Int2               typ_ch;
  Int2               typ_ml;
  Int2               typ_nt;
  Int2               typ_st;

  if (types == NULL || field_info == NULL || field_count < 1) return NULL;
  alist = MemNew (sizeof (EnumFieldAssoc) * (field_count + 5));
  if (alist == NULL) return NULL;
  ap = alist;
  fld = 0;
  typ_ml = DatabaseFromName ("MEDLINE");
  typ_aa = DatabaseFromName ("Protein");
  typ_nt = DatabaseFromName ("Nucleotide");
  typ_st = DatabaseFromName ("Structure");
  typ_ch = DatabaseFromName ("Genome");
  fld_prot = FieldFromTag ("PROT");
  while (fld < field_count) {
    if (allFields || types [db].fields [fld].num_terms > 0) {
      if (field_info [fld].name != NULL && field_info [fld].name [0] != '[') {
        if (db == typ_ml && fld == fld_prot && (! allFields)) {
        } else {
          ap->name = StringSave (field_info [fld].name);
          if (ap->name != NULL) {
            ptr = ap->name;
            ch = *ptr;
            while (ch != '\0') {
              if (ch == '/') {
                *ptr = '-';
              }
              ptr++;
              ch = *ptr;
            }
          }
          ap->value = fld;
          ap++;
        }
      }
    }
    fld++;
  }
  if (allFields) {
    ap->name = StringSave ("NCBI Publication ID");
    ap->value = fld;
    ap++;
    fld++;
    ap->name = StringSave ("NCBI Sequence ID");
    ap->value = fld;
    ap++;
    fld++;
    ap->name = StringSave ("NCBI Structure ID");
    ap->value = fld;
  } else if (db == typ_ml) {
    ap->name = StringSave ("NCBI Publication ID");
    ap->value = fld;
  } else if (db == typ_aa || db == typ_nt || db == typ_ch) {
    ap->name = StringSave ("NCBI Sequence ID");
    ap->value = fld;
  } else if (db == typ_st) {
    ap->name = StringSave ("NCBI Structure ID");
    ap->value = fld;
  } else {
    ap->name = NULL;
    ap->value = 0;
  }
  ap++;
  ap->name = NULL;
  sortit = TRUE;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp != NULL) {
    sortit = egp->sortFields;
  }
  if (sortit) {
    SortEnumFieldAlist (alist);
  }
  return alist;
}

static void ReadDefaultSettings (TermFormPtr tfp)

{
  EnumFieldAssocPtr  alist;
  Int2               db;
  EntrezGlobalsPtr   egp;
  Int2               fld;
  Int2               idx;
  Int2               j;
  Int2               md;
  PopuP              p;
  CharPtr            str;
  UIEnum             val;

  if (tfp == NULL) return;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL) return;
  str = egp->initDatabase;
  if (str != NULL) {
    alist = tfp->dbalist;
    p = tfp->database;
    for (j = 0; alist [j].name != NULL; j++) {
      if (StringICmp (alist [j].name, str) == 0) {
        val = (UIEnum) alist [j].value;
        SetEnumPopup (p, alist, val);
        tfp->currDb = (Int2) val;
      }
    }
  }
  str = egp->initField;
  if (str != NULL) {
    db = tfp->currDb;
    if (db < tfp->numdbs) {
      alist = tfp->fldalists [db];
      p = tfp->fields [db];
      for (j = 0; alist [j].name != NULL; j++) {
        if (StringICmp (alist [j].name, str) == 0) {
          val = (UIEnum) alist [j].value;
          SetEnumPopup (p, alist, val);
          tfp->currFld = (Int2) val;
        }
      }
    }
  }
  str = egp->initMode;
  if (str != NULL) {
    fld = tfp->currFld;
    md = tfp->modeFromField [fld];
    if (tfp->fieldModeValues != NULL && fld < tfp->numflds) {
      alist = mode_alists [md];
      p = tfp->modes [md];
      for (j = 0; alist [j].name != NULL; j++) {
        if (StringICmp (alist [j].name, str) == 0) {
          val = (UIEnum) alist [j].value;
          SetEnumPopup (p, alist, val);
          tfp->currMod = (Int2) val;
          idx = tfp->currDb * tfp->numflds + tfp->currFld;
          tfp->fieldModeValues [idx] = tfp->currMod;
        }
      }
    }
  }
}

extern ForM CreateTermListForm (Int2 left, Int2 top, CharPtr title,
                                WndActnProc activate, FormMessageFunc messages)

{
  GrouP             c;
  Int2              db;
  EntrezGlobalsPtr  egp;
  EntrezInfoPtr     eip;
  Int2              fld;
  Int2              fld_accn;
  Int2              fld_all;
  Int2              fld_majr;
  Int2              fld_mesh;
  Int2              fld_orgn;
  Int2              fld_pacc;
  Int2              fld_prot;
  GrouP             g;
  GrouP             h;
  Int2              idx;
  GrouP             j;
  Boolean           macLike;
  Int2              md;
  PrompT            ppt1, ppt2;
  GrouP             q1, q2, q3;
  RecT              r;
  TermFormPtr       tfp;
  Int2              typ_ml;
  WindoW            w;
  Int2              wid;

  w = NULL;
  eip = EntrezGetInfo ();
  if (eip == NULL || eip->type_info == NULL ||
      eip->types == NULL || eip->field_info == NULL) return NULL;
  egp = (EntrezGlobalsPtr) GetAppProperty ("EntrezGlobals");
  if (egp == NULL) return NULL;
  macLike = egp->popdownBehavior;
  tfp = (TermFormPtr) MemNew (sizeof (TermFormData));

  if (tfp != NULL) {
    w = FixedWindow (left, top, -10, -10, title, StdSendCloseWindowMessageProc);
    SetObjectExtra (w, tfp, CleanupEntrezTermListForm);
    tfp->form = (ForM) w;
    tfp->formmessage = EntrezTermListFormMessage;

    tfp->appmessage = messages;
    tfp->activate = activate;
    SetActivate (w, EntrezTermListFormActivate);

    if (egp->createTrmLstMenus != NULL) {
      egp->createTrmLstMenus (w);
    }

    h = HiddenGroup (w, -1, 0, NULL);

    g = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (g, 5, 5);

    c = HiddenGroup (g, 0, 2, NULL);
    SetGroupSpacing (c, 15, 2);

    tfp->numdbs = eip->type_count;
    tfp->numflds = eip->field_count;
    tfp->fieldModeValues = (Int2Ptr) MemNew (sizeof (Int2) * (tfp->numdbs * tfp->numflds + 1));
    tfp->modeFromField = (Int2Ptr) MemNew (sizeof (Int2) * (tfp->numflds + 1));
    for (fld = 0; fld <= tfp->numflds; fld++) {
      tfp->modeFromField [fld] = POPUP_LKP;
    }

    typ_ml = DatabaseFromName ("MEDLINE");
    fld_accn = FieldFromTag ("ACCN");
    fld_all = FieldFromTag ("ALL");
    fld_majr = FieldFromTag ("MAJR");
    fld_mesh = FieldFromTag ("MESH");
    fld_orgn = FieldFromTag ("ORGN");
    fld_pacc = FieldFromTag ("PACC");
    fld_prot = FieldFromTag ("PROT");

    tfp->dbalist = MakeDatabaseAlist (eip->type_info, eip->type_count, FALSE);

    StaticPrompt (c, "Database:", 0, 0, programFont, 'l');
    q1 = HiddenGroup (c, 0, 0, NULL);
    tfp->database = PopupList (q1, macLike, ChangeDatabase);
    SetObjectExtra (tfp->database, tfp, NULL);
    InitEnumPopup (tfp->database, tfp->dbalist, NULL);
    SetEnumPopup (tfp->database, tfp->dbalist, (UIEnum) typ_ml);

    tfp->fldalists = (EnumFieldAssocPtr PNTR) MemNew (sizeof (EnumFieldAssocPtr) * (eip->type_count + 2));
    for (db = 0; db < eip->type_count; db++) {
      tfp->fldalists [db] = MakeFieldAlist (eip->types, eip->field_info, eip->field_count, db, FALSE);
    }

    StaticPrompt (c, "Field:", 0, 0, programFont, 'l');
    q2 = HiddenGroup (c, 0, 0, NULL);
    tfp->fields = MemNew (sizeof (PopuP) * (eip->type_count + 2));
    for (db = 0; db < eip->type_count; db++) {
      tfp->fields [db] = PopupList (q2, macLike, ChangeField);
      SetObjectExtra (tfp->fields [db], tfp, NULL);
      InitEnumPopup (tfp->fields [db], tfp->fldalists [db], NULL);
      SetEnumPopup (tfp->fields [db], tfp->fldalists [db], (UIEnum) fld_all);
      Hide (tfp->fields [db]);
    }

    StaticPrompt (c, "Mode:", 0, 0, programFont, 'l');
    q3 = HiddenGroup (c, 0, 0, NULL);
    for (md = 0; mode_alists [md] != NULL; md++) {
      tfp->modes [md] = PopupList (q3, macLike, ChangeMode);
      SetObjectExtra (tfp->modes [md], tfp, NULL);
      InitEnumPopup (tfp->modes [md], mode_alists [md], NULL);
      if (md == POPUP_SEL_MUL_TRM) {
        SetEnumPopup (tfp->modes [md], mode_alists [md], (UIEnum) AUTOMATIC_MODE);
      } else if (md == POPUP_SEL_MUL_TRU) {
        SetEnumPopup (tfp->modes [md], mode_alists [md], (UIEnum) AUTOMATIC_MODE);
      } else if (md == POPUP_LKP) {
        SetEnumPopup (tfp->modes [md], mode_alists [md], (UIEnum) LOOKUP_UID_MODE);
      } else {
        SetEnumPopup (tfp->modes [md], mode_alists [md], (UIEnum) SELECTION_MODE);
      }
      Hide (tfp->modes [md]);
    }
    tfp->nummods = md;

    for (idx = 0; idx < tfp->numdbs * tfp->numflds; idx++) {
      tfp->fieldModeValues [idx] = SELECTION_MODE;
    }
    for (db = 0; db < eip->type_count; db++) {
      for (fld = 0; fld < eip->field_count; fld++) {
        if (eip->types [db].fields [fld].num_terms > 0) {
          if (eip->field_info [fld].name != NULL && eip->field_info [fld].name [0] != '[') {
            md = -1;
            if (fld == fld_all) {
              md = POPUP_SEL_MUL_TRU;
            } else if (db == typ_ml && fld == fld_prot) {
            } else if (fld == fld_accn || fld == fld_pacc) {
              md = POPUP_SEL_TRU_LKP;
            } else if (fld == fld_orgn) {
              md = POPUP_SEL_TRU_TAX;
            } else if (fld == fld_mesh) {
              md = POPUP_SEL_TRU_MSH;
            } else if (fld == fld_majr) {
              md = POPUP_SEL_TRU_MSH;
            } else if (eip->field_info [fld].single_token) {
              md = POPUP_SEL_MUL_TRM;
            } else {
              md = POPUP_SEL_TRU;
            }
            if (md >= 0) {
              tfp->modeFromField [fld] = md;
              idx = db * tfp->numflds + fld;
              if (md == POPUP_SEL_MUL_TRM) {
                tfp->fieldModeValues [idx] = AUTOMATIC_MODE;
              } else if (md == POPUP_SEL_MUL_TRU) {
                tfp->fieldModeValues [idx] = AUTOMATIC_MODE;
              } else {
                tfp->fieldModeValues [idx] = SELECTION_MODE;
              }
            }
          }
        }
      }
    }

    StaticPrompt (c, "", 0, 0, programFont, 'l');
    tfp->accept = DefaultButton (c, "Accept", AcceptProc);
    SetObjectExtra (tfp->accept, tfp, NULL);
    Disable (tfp->accept);

    AlignObjects (ALIGN_MIDDLE, (HANDLE) q1, (HANDLE) q2, (HANDLE) q3, (HANDLE) tfp->accept, NULL);

    GetPosition (tfp->accept, &r);
    SelectFont (programFont);
    wid = r.right - (StringWidth ("From:") + StringWidth ("To:")) - 30;
    SelectFont (systemFont);
    wid /= 2 * stdCharWidth;

    c = HiddenGroup (g, 0, 0, NULL);
    tfp->termGrp = HiddenGroup (c, 2, 0, NULL);
    StaticPrompt (tfp->termGrp, "Term:", 0, dialogTextHeight, programFont, 'l');
    tfp->term = DialogText (tfp->termGrp, "", 20, TextAction);
    SetObjectExtra (tfp->term, tfp, NULL);
    tfp->rangeGrp = HiddenGroup (c, 4, 0, NULL);
    StaticPrompt (tfp->rangeGrp, "From:", 0, dialogTextHeight, programFont, 'l');
    tfp->from = DialogText (tfp->rangeGrp, "", wid, TextAction);
    SetObjectExtra (tfp->from, tfp, NULL);
    StaticPrompt (tfp->rangeGrp, "To:", 0, dialogTextHeight, programFont, 'l');
    tfp->to = DialogText (tfp->rangeGrp, "", wid, TextAction);
    SetObjectExtra (tfp->to, tfp, NULL);
    Hide (tfp->rangeGrp);
    tfp->taxParents = PopupList (c, TRUE, ChangeTaxParents);
    SetObjectExtra (tfp->taxParents, tfp, NULL);
    Hide (tfp->taxParents);
    tfp->taxStrings = NULL;

    /*
    tfp->avail = DocumentPanel (g, 10 * stdCharWidth, 7 * stdLineHeight);
    SetObjectExtra (tfp->avail, tfp, NULL);
    c = HiddenGroup (g, 5, 0, NULL);
    StaticPrompt (c, "Term Selection", 0, 0, programFont, 'l');
    ppt = StaticPrompt (c, "Query Refinement", 0, 0, programFont, 'r');
    tfp->chosen = DocumentPanel (g, 10 * stdCharWidth, 7 * stdLineHeight);
    SetObjectExtra (tfp->chosen, tfp, NULL);
    */

    c = HiddenGroup (g, -1, 0, NULL);
    ppt1 = StaticPrompt (c, "Term Selection", 0, 0, programFont, 'c');
    tfp->avail = DocumentPanel (c, 10 * stdCharWidth, 7 * stdLineHeight);
    SetObjectExtra (tfp->avail, tfp, NULL);
    tfp->avItem = 0;
    tfp->avRow = 0;
    SetDocShade (tfp->avail, DrawAvailLeaf, NULL, HighlightAvail, NULL);
    SetDocProcs (tfp->avail, ClickAvail, NULL, ReleaseAvail, NULL);
    SetDocCache (tfp->avail, NULL, NULL, NULL);

    ppt2 = StaticPrompt (c, "Query Refinement", 0, 0, programFont, 'c');
    j = HiddenGroup (c, 0, 0, NULL);

    tfp->advBoolText = ScrollText (j, 10, 7, programFont, TRUE, AdvBoolTextProc);
    SetObjectExtra (tfp->advBoolText, tfp, NULL);
    Hide (tfp->advBoolText);
    tfp->usingAdv = FALSE;
    tfp->retrieveMode = FETCH_MODE;

    tfp->chosen = DocumentPanel (j, 10 * stdCharWidth, 7 * stdLineHeight);
    SetObjectExtra (tfp->chosen, tfp, NULL);
    SetDocProcs (tfp->chosen, ClickChosen, DragChosen, ReleaseChosen, NULL);
    SetDocShade (tfp->chosen, DrawChosenBrackets, NULL, HighlightChosen, NULL);
    tfp->numChosenLines = 0;

    AlignObjects (ALIGN_CENTER, (HANDLE) tfp->taxParents,
                  (HANDLE) tfp->termGrp, NULL);
    AlignObjects (ALIGN_MIDDLE, (HANDLE) tfp->taxParents,
                  (HANDLE) tfp->termGrp, (HANDLE) tfp->rangeGrp, NULL);
    AlignObjects (ALIGN_RIGHT, (HANDLE) tfp->accept, (HANDLE) tfp->term,
                  (HANDLE) tfp->avail, (HANDLE) tfp->chosen, (HANDLE) tfp->advBoolText,
                  (HANDLE) tfp->to, (HANDLE) ppt1, (HANDLE) ppt2, NULL);

    c = HiddenGroup (g, 5, 0, NULL);
    SetGroupSpacing (c, 10, 10);
    tfp->retrieve = PushButton (c, "Retrieve 000000000 Documents", RetrieveDocsProc);
    SetObjectExtra (tfp->retrieve, tfp, NULL);
    SetTitle (tfp->retrieve, "Retrieve 0 Documents");
    Disable (tfp->retrieve);
    tfp->reset = PushButton (c, "Reset", ResetProc);
    SetObjectExtra (tfp->reset, tfp, NULL);

    RealizeWindow (w);

    ObjectRect (tfp->avail, &r);
    InsetRect (&r, 4, 4);
    SelectFont (systemFont);

    availColFmt [1].pixWidth = StringWidth ("0000000") + 10;
    availColFmt [0].pixWidth = (r.right - r.left) - availColFmt [1].pixWidth;
    availColFmt [2].pixWidth = 0;
    chosenColFmt [2].pixWidth = availColFmt [1].pixWidth;
    chosenColFmt [1].pixWidth = StringWidth ("0000000") + 10;
    chosenColFmt [0].pixWidth = availColFmt [0].pixWidth - chosenColFmt [1].pixWidth;

    SetDocAutoAdjust (tfp->avail, FALSE);
    SetDocAutoAdjust (tfp->chosen, FALSE);

    tfp->currDb = typ_ml;
    tfp->currFld = fld_all;
    tfp->currMod = POPUP_SEL_MUL_TRU;

    ReadDefaultSettings (tfp);

    /* ChangeDatabase calls DoReset, which puts blank lines in avail and chosen */
    ChangeDatabase (tfp->database);
    SetWindowTimer (w, AvailTimerProc);

    InvalDocument (tfp->avail);
    InvalDocument (tfp->chosen);
  }
  return (ForM) w;
}

