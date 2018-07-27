/*   sequin8.c
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
* File Name:  sequin8.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   2/3/98
*
* $Revision: 6.147 $
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

#include "sequin.h"
#include <objsub.h>
#include <valid.h>
#include <cdrgn.h>
#include <suggslp.h>
#include <toasn3.h>
#include <subutil.h>
#include <explore.h>
#include <medarch.h>
#include <medutil.h>
#include <tofasta.h>
#include <asn2gnbk.h>
#include <alignmgr2.h>
#include <spidey.h>
#include <blast.h>

typedef struct evidenceformdata {
  FEATURE_FORM_BLOCK

  LisT           objlist;
  TexT           findthis;
  Uint2          itemtype;
  Uint2          subtype;
  PopuP          evdence;
  Uint2          exp_ev;
  ObjMgrPtr      omp;
  ObjMgrTypePtr  omtp;
  ValNodePtr     head;
  Boolean        stringfound;
  Char           findStr [128];
} EvidenceFormData, PNTR EvidenceFormPtr;

static void LIBCALLBACK AsnWriteEvidenceCallBack (AsnExpOptStructPtr pAEOS)

{
  EvidenceFormPtr  efp;
  CharPtr          pchFind;
  CharPtr          pchSource;

  efp = (EvidenceFormPtr) pAEOS->data;
  if (ISA_STRINGTYPE (AsnFindBaseIsa (pAEOS->atp))) {
	pchSource = (CharPtr) pAEOS->dvp->ptrvalue;
	pchFind = efp->findStr;
	if (StringSearch (pchSource, pchFind) != NULL) {
	  efp->stringfound = TRUE;
	}
  }
}

static Boolean EvidenceHasSubstring (ObjMgrTypePtr omtp, AsnIoPtr aip, Pointer ptr, EvidenceFormPtr efp)

{
  efp->stringfound = FALSE;
  (omtp->asnwrite) (ptr, aip, NULL);
  return efp->stringfound;
}

static void EvidenceCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  AsnExpOptPtr     aeop;
  AsnIoPtr         aip;
  BioseqPtr        bsp;
  BioseqSetPtr     bssp;
  EvidenceFormPtr  efp;
  Boolean          notext;
  ObjMgrTypePtr    omtp;
  SeqAnnotPtr      sap;
  SeqFeatPtr       sfp;
  Uint2            subtype;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  efp = (EvidenceFormPtr) mydata;
  if (efp == NULL) return;
  omtp = efp->omtp;
  if (omtp == NULL || omtp->subtypefunc == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  GetTitle (efp->findthis, efp->findStr, sizeof (efp->findStr) - 1);
  notext = StringHasNoText (efp->findStr);
  aip = AsnIoNullOpen ();
  aeop = AsnExpOptNew (aip, NULL, NULL, AsnWriteEvidenceCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) efp;
  }
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        subtype = (*(omtp->subtypefunc)) ((Pointer) sfp);
        if (efp->subtype == 0 || subtype == efp->subtype ||
           (efp->subtype == FEATDEF_IMP &&
            subtype >= FEATDEF_allele && subtype <= FEATDEF_site_ref)) {
          if (notext || EvidenceHasSubstring (omtp, aip, (Pointer) sfp, efp)) {
            sfp->exp_ev = (Uint1) efp->exp_ev;
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
  AsnIoClose (aip);
}

static void DoEvidence (ButtoN b)

{
  EvidenceFormPtr  efp;
  SeqEntryPtr      sep;
  Uint2            subtype;
  Int2             val;
  ValNodePtr       vnp;

  efp = GetObjectExtra (b);
  if (efp == NULL) return;
  sep = GetTopSeqEntryForEntityID (efp->input_entityID);
  if (sep == NULL) return;
  Hide (efp->form);
  WatchCursor ();
  Update ();
  efp->itemtype = OBJ_SEQFEAT;
  subtype = 0;
  vnp = NULL;
  val = GetValue (efp->objlist);
  if (val > 0) {
    vnp = efp->head;
    while (vnp != NULL && val > 1) {
      val--;
      vnp = vnp->next;
    }
  }
  if (vnp != NULL) {
    subtype = vnp->choice;
  }
  efp->omp = ObjMgrGet ();
  efp->omtp = NULL;
  if (efp->omp != NULL) {
    efp->omtp = ObjMgrTypeFind (efp->omp, efp->itemtype, NULL, NULL);
  }
  efp->subtype = subtype;
  efp->exp_ev = GetValue (efp->evdence) - 1;
  if (efp->itemtype != 0 && efp->omtp != NULL) {
    SeqEntryExplore (sep, (Pointer) efp, EvidenceCallback);
  }
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (efp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, efp->input_entityID, 0, 0);
  Remove (efp->form);
}

static void EvidenceMessageProc (ForM f, Int2 mssg)

{
  EvidenceFormPtr  efp;

  efp = (EvidenceFormPtr) GetObjectExtra (f);
  if (efp != NULL) {
    if (efp->appmessage != NULL) {
      efp->appmessage (f, mssg);
    }
  }
}

static void CleanupEvidencePage (GraphiC g, VoidPtr data)

{
  EvidenceFormPtr  efp;

  efp = (EvidenceFormPtr) data;
  if (efp != NULL) {
    ValNodeFreeData (efp->head);
  }
  StdCleanupFormProc (g, data);
}

extern void EditEvidenceFlag (IteM i)

{
  BaseFormPtr        bfp;
  ButtoN             b;
  GrouP              c;
  FeatDefPtr         curr;
  EvidenceFormPtr    efp;
  GrouP              g;
  GrouP              h;
  ValNodePtr         head;
  GrouP              k;
  Uint1              key;
  CharPtr            label = NULL;
  Int2               listHeight;
  GrouP              q;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  Uint2              subtype;
  ValNodePtr         vnp;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  efp = (EvidenceFormPtr) MemNew (sizeof (EvidenceFormData));
  if (efp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Feature Evidence", StdCloseWindowProc);
  SetObjectExtra (w, efp, CleanupEvidencePage);
  efp->form = (ForM) w;
  efp->formmessage = EvidenceMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    efp->appmessage = sepp->handleMessages;
  }

  efp->input_entityID = bfp->input_entityID;
  efp->input_itemID = bfp->input_itemID;
  efp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 0, 2, NULL);
  StaticPrompt (g, "Feature", 0, 0, programFont, 'c');
  if (indexerVersion) {
    listHeight = 16;
  } else {
    listHeight = 8;
  }
  efp->objlist = SingleList (g, 16, listHeight, NULL);
  head = ValNodeNew (NULL);
  if (head != NULL) {
    head->choice = 0;
    head->data.ptrvalue = StringSave ("[ALL FEATURES]");
  }
  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key != FEATDEF_BAD) {
      subtype = curr->featdef_key;
      if (subtype != FEATDEF_misc_RNA &&
          subtype != FEATDEF_precursor_RNA &&
          subtype != FEATDEF_mat_peptide &&
          subtype != FEATDEF_sig_peptide &&
          subtype != FEATDEF_transit_peptide) {
        vnp = ValNodeNew (head);
        /* if (head == NULL) {
          head = vnp;
        } */
        if (vnp != NULL) {
          vnp->choice = subtype;
          vnp->data.ptrvalue = StringSave (curr->typelabel);
        }
      }
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }
  if (head != NULL) {
    head = SortValNode (head, SortByVnpChoice);
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      ListItem (efp->objlist, (CharPtr) vnp->data.ptrvalue);
    }
  }
  efp->head = head;

  q = HiddenGroup (h, 2, 0, NULL);
  StaticPrompt (q, "Evidence", 0, popupMenuHeight, programFont, 'c');
  efp->evdence = PopupList (q, TRUE, NULL);
  PopupItem (efp->evdence, " ");
  PopupItem (efp->evdence, "Experimental");
  PopupItem (efp->evdence, "Non-Experimental");
  SetValue (efp->evdence, 1);

  k = HiddenGroup (h, 0, 2, NULL);
  StaticPrompt (k, "Optional string constraint", 0, dialogTextHeight, programFont, 'c');
  efp->findthis = DialogText (k, "", 14, NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoEvidence);
  SetObjectExtra (b, efp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) q, (HANDLE) k, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

typedef struct exceptionformdata {
  FEATURE_FORM_BLOCK

  LisT           objlist;
  TexT           findthis;
  Uint2          itemtype;
  Uint2          subtype;
  GrouP          xception;
  TexT           xceptText;
  ButtoN         rescueExpl;
  Boolean        excpt;
  CharPtr        except_text;
  Boolean        rescue;
  ObjMgrPtr      omp;
  ObjMgrTypePtr  omtp;
  ValNodePtr     head;
  Boolean        stringfound;
  Char           findStr [128];
} ExceptionFormData, PNTR ExceptionFormPtr;

static void LIBCALLBACK AsnWriteExceptionCallBack (AsnExpOptStructPtr pAEOS)

{
  ExceptionFormPtr  efp;
  CharPtr          pchFind;
  CharPtr          pchSource;

  efp = (ExceptionFormPtr) pAEOS->data;
  if (ISA_STRINGTYPE (AsnFindBaseIsa (pAEOS->atp))) {
	pchSource = (CharPtr) pAEOS->dvp->ptrvalue;
	pchFind = efp->findStr;
	if (StringSearch (pchSource, pchFind) != NULL) {
	  efp->stringfound = TRUE;
	}
  }
}

static Boolean ExceptionHasSubstring (ObjMgrTypePtr omtp, AsnIoPtr aip, Pointer ptr, ExceptionFormPtr efp)

{
  efp->stringfound = FALSE;
  (omtp->asnwrite) (ptr, aip, NULL);
  return efp->stringfound;
}

static void ExceptionCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  AsnExpOptPtr      aeop;
  AsnIoPtr          aip;
  BioseqPtr         bsp;
  BioseqSetPtr      bssp;
  ExceptionFormPtr  efp;
  Boolean           notext;
  ObjMgrTypePtr     omtp;
  SeqAnnotPtr       sap;
  SeqFeatPtr        sfp;
  CharPtr           str;
  Uint2             subtype;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  efp = (ExceptionFormPtr) mydata;
  if (efp == NULL) return;
  omtp = efp->omtp;
  if (omtp == NULL || omtp->subtypefunc == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  GetTitle (efp->findthis, efp->findStr, sizeof (efp->findStr) - 1);
  notext = StringHasNoText (efp->findStr);
  aip = AsnIoNullOpen ();
  aeop = AsnExpOptNew (aip, NULL, NULL, AsnWriteExceptionCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) efp;
  }
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        subtype = (*(omtp->subtypefunc)) ((Pointer) sfp);
        if (efp->subtype == 0 || subtype == efp->subtype ||
           (efp->subtype == FEATDEF_IMP &&
            subtype >= FEATDEF_allele && subtype <= FEATDEF_site_ref)) {
          if (notext || ExceptionHasSubstring (omtp, aip, (Pointer) sfp, efp)) {
            sfp->excpt = efp->excpt;
            if (sfp->excpt) {
              if (! StringHasNoText (efp->except_text)) {
                sfp->except_text = MemFree (sfp->except_text);
                sfp->except_text = StringSave (efp->except_text);
              }
            } else {
              if (efp->rescue) {
                if (sfp->comment == NULL) {
                  sfp->comment = sfp->except_text;
                } else {
                  str = MemNew (StringLen (sfp->comment) + StringLen (sfp->except_text) + 5);
                  if (str != NULL) {
                    StringCpy (str, sfp->comment);
                    StringCat (str, "; ");
                    StringCat (str, sfp->except_text);
                    sfp->comment = MemFree (sfp->comment);
                    sfp->comment = str;
                  }
                }
                sfp->except_text = NULL;
              } else {
                sfp->except_text = MemFree (sfp->except_text);
              }
            }
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
  AsnIoClose (aip);
}

static void DoException (ButtoN b)

{
  ExceptionFormPtr  efp;
  SeqEntryPtr      sep;
  Uint2            subtype;
  Int2             val;
  ValNodePtr       vnp;

  efp = GetObjectExtra (b);
  if (efp == NULL) return;
  sep = GetTopSeqEntryForEntityID (efp->input_entityID);
  if (sep == NULL) return;
  Hide (efp->form);
  WatchCursor ();
  Update ();
  efp->itemtype = OBJ_SEQFEAT;
  subtype = 0;
  vnp = NULL;
  val = GetValue (efp->objlist);
  if (val > 0) {
    vnp = efp->head;
    while (vnp != NULL && val > 1) {
      val--;
      vnp = vnp->next;
    }
  }
  if (vnp != NULL) {
    subtype = vnp->choice;
  }
  efp->omp = ObjMgrGet ();
  efp->omtp = NULL;
  if (efp->omp != NULL) {
    efp->omtp = ObjMgrTypeFind (efp->omp, efp->itemtype, NULL, NULL);
  }
  efp->subtype = subtype;
  efp->excpt = (Boolean) (GetValue (efp->xception) == 2);
  if (efp->excpt) {
    efp->except_text = SaveStringFromText (efp->xceptText);
  } else {
    efp->except_text = NULL;
  }
  efp->rescue = GetStatus (efp->rescueExpl);
  if (efp->itemtype != 0 && efp->omtp != NULL) {
    SeqEntryExplore (sep, (Pointer) efp, ExceptionCallback);
  }
  MemFree (efp->except_text);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (efp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, efp->input_entityID, 0, 0);
  Remove (efp->form);
}

static void ExceptionMessageProc (ForM f, Int2 mssg)

{
  ExceptionFormPtr  efp;

  efp = (ExceptionFormPtr) GetObjectExtra (f);
  if (efp != NULL) {
    if (efp->appmessage != NULL) {
      efp->appmessage (f, mssg);
    }
  }
}

static void CleanupExceptionPage (GraphiC g, VoidPtr data)

{
  ExceptionFormPtr  efp;

  efp = (ExceptionFormPtr) data;
  if (efp != NULL) {
    ValNodeFreeData (efp->head);
  }
  StdCleanupFormProc (g, data);
}

extern void EditExceptionFlag (IteM i)

{
  BaseFormPtr        bfp;
  ButtoN             b;
  GrouP              c;
  FeatDefPtr         curr;
  ExceptionFormPtr   efp;
  GrouP              g;
  GrouP              h;
  ValNodePtr         head;
  GrouP              k;
  Uint1              key;
  CharPtr            label = NULL;
  Int2               listHeight;
  PrompT             ppt;
  GrouP              q;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  Uint2              subtype;
  ValNodePtr         vnp;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  efp = (ExceptionFormPtr) MemNew (sizeof (ExceptionFormData));
  if (efp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Feature Exception", StdCloseWindowProc);
  SetObjectExtra (w, efp, CleanupExceptionPage);
  efp->form = (ForM) w;
  efp->formmessage = ExceptionMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    efp->appmessage = sepp->handleMessages;
  }

  efp->input_entityID = bfp->input_entityID;
  efp->input_itemID = bfp->input_itemID;
  efp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 0, 2, NULL);
  StaticPrompt (g, "Feature", 0, 0, programFont, 'c');
  if (indexerVersion) {
    listHeight = 16;
  } else {
    listHeight = 8;
  }
  efp->objlist = SingleList (g, 16, listHeight, NULL);
  head = ValNodeNew (NULL);
  if (head != NULL) {
    head->choice = 0;
    head->data.ptrvalue = StringSave ("[ALL FEATURES]");
  }
  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key != FEATDEF_BAD) {
      subtype = curr->featdef_key;
      if (subtype != FEATDEF_misc_RNA &&
          subtype != FEATDEF_precursor_RNA &&
          subtype != FEATDEF_mat_peptide &&
          subtype != FEATDEF_sig_peptide &&
          subtype != FEATDEF_transit_peptide) {
        vnp = ValNodeNew (head);
        /* if (head == NULL) {
          head = vnp;
        } */
        if (vnp != NULL) {
          vnp->choice = subtype;
          vnp->data.ptrvalue = StringSave (curr->typelabel);
        }
      }
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }
  if (head != NULL) {
    head = SortValNode (head, SortByVnpChoice);
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      ListItem (efp->objlist, (CharPtr) vnp->data.ptrvalue);
    }
  }
  efp->head = head;

  q = HiddenGroup (h, -2, 0, NULL);
  ppt = StaticPrompt (q, "Exception  ", 0, stdLineHeight, programFont, 'l');
  efp->xception = HiddenGroup (q, -2, 0, NULL);
  SetObjectExtra (efp->xception, efp, NULL);
  RadioButton (efp->xception, "Clear");
  RadioButton (efp->xception, "Set");
  SetValue (efp->xception, 1);
  StaticPrompt (q, "Explanation", 0, dialogTextHeight, programFont, 'l');
  efp->xceptText = DialogText (q, "", 12, NULL);
  AlignObjects (ALIGN_MIDDLE, (HANDLE) ppt, (HANDLE) efp->xception, NULL);
  efp->rescueExpl = CheckBox (q, "Move explanation to comment", NULL);

  k = HiddenGroup (h, 0, 2, NULL);
  StaticPrompt (k, "Optional string constraint", 0, dialogTextHeight, programFont, 'c');
  efp->findthis = DialogText (k, "", 14, NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoException);
  SetObjectExtra (b, efp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) q, (HANDLE) k, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

static void BreakIntoAGroup (BioseqSetPtr parent, Uint1 _class, SeqEntryPtr list)

{
  BioseqSetPtr  bssp;
  Int2          count;
  SeqEntryPtr   sep;
  SeqEntryPtr   tmp;

  while (list != NULL) {
    bssp = BioseqSetNew ();
    if (bssp == NULL) return;
    bssp->_class = _class;
    sep = SeqEntryNew ();
    if (sep == NULL) return;
    sep->choice = 2;
    sep->data.ptrvalue = (Pointer) bssp;
    if (parent->seq_set != NULL) {
      tmp = parent->seq_set;
      while (tmp->next != NULL) {
        tmp = tmp->next;
      }
      tmp->next = sep;
    } else {
      parent->seq_set = sep;
    }
    bssp->seq_set = list;
    for (tmp = list, count = 0; tmp != NULL && count < 99; tmp = tmp->next, count++) continue;
    if (tmp != NULL) {
      list = tmp->next;
      tmp->next = NULL;
    } else {
      list = NULL;
    }
  }
}

extern void MakeGroupsOf200 (IteM i)

{
  AsnIoPtr       aip;
  BaseFormPtr    bfp;
  BioseqSetPtr   bssp;
  Uint1          _class;
  Int2           count;
  Char           file [FILENAME_MAX];
  SeqEntryPtr    list;
  SeqEntryPtr    next;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;
  Char           output [PATH_MAX];
  Uint2          parenttype;
  Pointer        parentptr;
  Char           path [PATH_MAX];
  CharPtr        ptr;
  SeqEntryPtr    sep;
  SeqSubmitPtr   ssp;
  Char           str [FILENAME_MAX];
  SeqEntryPtr    tmp;
#ifdef WIN_MAC
  FILE           *f;
#endif

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  if (! IS_Bioseq_set (sep)) return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL) return;
  _class = bssp->_class;
  if (_class != 7 && _class != 13 && _class != 14 && _class != 15) return;

  SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
  GetSeqEntryParent (sep, &parentptr, &parenttype);

  list = bssp->seq_set;
  bssp->seq_set = NULL;
  bssp->_class = 7;
  BreakIntoAGroup (bssp, _class, list);

  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
  PropagateFromGenBankBioseqSet (sep, TRUE);

  if (parenttype == OBJ_SEQSUB) {
    if (GetOutputFileName (path, sizeof (path), "")) {
      ssp = (SeqSubmitPtr) parentptr;
      if (ssp != NULL && ssp->datatype == 1) {
        sep = (SeqEntryPtr) ssp->data;
        ptr = StringRChr (path, DIRDELIMCHR);
        if (ptr != NULL) {
          ptr++;
          StringNCpy_0 (file, ptr, sizeof (file));
          *ptr = '\0';
          tmp = bssp->seq_set;
          count = 0;
          while (tmp != NULL) {
            next = tmp->next;
            tmp->next = NULL;
            ssp->data = (Pointer) tmp;
            StringCpy (output, path);
            count++;
            if (count < 10) {
              sprintf (str, "%s0%1d", file, (int) count);
            } else {
              sprintf (str, "%s%2d", file, (int) count);
            }
            FileBuildPath (output, NULL, str);
#ifdef WIN_MAC
            f = FileOpen (output, "r");
            if (f != NULL) {
              FileClose (f);
            } else {
              FileCreate (output, "TEXT", "ttxt");
            }
#endif
            aip = AsnIoOpen (output, "w");
            if (aip != NULL) {
              SeqSubmitAsnWrite (ssp, aip, NULL);
            }
            AsnIoClose (aip);
            tmp->next = next;
            tmp = next;
          }
          ssp->data = (Pointer) sep;
        }
      }
    }
  }

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  Update ();
}

extern void ParseInNucUpdates (IteM i)

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
  Message (MSG_OK, "Not yet implemented");
}

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

NLM_EXTERN void CdTransCheck(ValidStructPtr vsp, SeqFeatPtr sfp);
static Boolean CdsTranslatesProperly (SeqFeatPtr sfp, GatherContextPtr gcp)

{
  Int2            errors;
  Int2            j;
  ErrSev          oldErrSev;
  ValidStructPtr  vsp;

  if (sfp == NULL) return TRUE;
  vsp = ValidStructNew ();
  if (vsp == NULL) return TRUE;
  vsp->gcp = gcp;
  oldErrSev = ErrSetMessageLevel (SEV_MAX);
  CdTransCheck (vsp, sfp);
  ErrSetMessageLevel (oldErrSev);
  ErrClear ();
  ErrShow ();
  errors = 0;
  for (j = 0; j < 6; j++) {
    errors += vsp->errors [j];
  }
  ValidStructFree (vsp);
  return (Boolean) (errors == 0);
}

typedef struct recompdata {
  Int4        count;
  MonitorPtr  mon;
  BioseqPtr   batchbsp;
  Boolean     include_stop;
} RecompData, PNTR RecompDataPtr;

static Int2 GeneticCodeFromCrp (CdRegionPtr crp)

{
  Int2            code;
  GeneticCodePtr  gcp;
  Char            name [256];
  ValNodePtr      tmp;

  code = 0;
  name [0] = '\0';
  gcp = crp->genetic_code;
  if (gcp != NULL) {
    tmp = (ValNodePtr) gcp->data.ptrvalue;
    for (tmp = (ValNodePtr) gcp->data.ptrvalue; tmp != NULL; tmp = tmp->next) {
      switch (tmp->choice) {
        case 1 :
          if (name [0] == '\0') {
            StringNCpy_0 (name, (CharPtr) tmp->data.ptrvalue, sizeof (name));
          }
          break;
        case 2 :
          code = tmp->data.intvalue;
          break;
        default :
          break;
      }
    }
    if (code == 0) {
      gcp = GeneticCodeFind (code, name);
      if (gcp != NULL) {
        for (tmp = (ValNodePtr) gcp->data.ptrvalue; tmp != NULL; tmp = tmp->next) {
          switch (tmp->choice) {
            case 2 :
              code = tmp->data.intvalue;
              break;
            default :
              break;
          }
        }
      }
    }
  }
  return code;
}

static Boolean RecomputeSuggCallback (GatherContextPtr gcp)

{
  Int2           code;
  CdRegionPtr    crp;
  BioseqPtr      nucbsp;
  BioseqPtr      protbsp;
  RecompDataPtr  rdp;
  SeqFeatPtr     sfp;
  SeqIdPtr       sip;
  SeqLocPtr      slp;
  Char           str [256];
  Char           tmp [256];

  if (gcp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  rdp = (RecompDataPtr) gcp->userdata;
  if (rdp == NULL) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  crp = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (crp == NULL) return TRUE;
  /* if (CdsTranslatesProperly (sfp, gcp)) return TRUE; */

  code = GeneticCodeFromCrp (crp);

  nucbsp = GetBioseqGivenSeqLoc (sfp->location, gcp->entityID);
  if (nucbsp != NULL && rdp->batchbsp != NULL && nucbsp != rdp->batchbsp) {
    ClearBatchSuggestNucleotide ();
    rdp->batchbsp = NULL;
    Message (MSG_POSTERR, "Recompute Suggest is reverting to slower processing");
  }
  sip = SeqLocId (sfp->product);
  if (sip != NULL) {
    protbsp = BioseqFind (sip);
    if (nucbsp != NULL && protbsp != NULL &&
        ISA_na (nucbsp->mol) && ISA_aa (protbsp->mol) &&
        nucbsp->length > 0 && protbsp->length > 0) {
      str [0] = '\0';
      tmp [0] = '\0';
      sip = SeqIdFindWorst (protbsp->id);
      SeqIdWrite (sip, tmp, PRINTID_REPORT, sizeof (tmp));
      (rdp->count)++;
      sprintf (str, "Processing sequence %d [%s]", (int) rdp->count, tmp);
      if (rdp->mon != NULL) {
        MonitorStrValue (rdp->mon, str);
        Update ();
      }
      slp = PredictCodingRegion (nucbsp, protbsp, code);
      if (slp == NULL) return TRUE;
      sfp->location = SeqLocFree (sfp->location);
      sfp->location = slp;
      sfp->partial = LocationHasNullsBetween (sfp->location);
    }
  }
  return TRUE;
}

extern void RecomputeSuggest (IteM i)

{
  BaseFormPtr  bfp;
  Int2         code;
  GatherScope  gs;
  SeqEntryPtr  nucsep;
  RecompData   rd;
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
  rd.count = 0;
  rd.mon = MonitorStrNewEx ("Correcting Coding Regions", 20, FALSE);
  rd.batchbsp = NULL;
  nucsep = FindNucSeqEntry (sep);
  if (nucsep != NULL && IS_Bioseq (nucsep)) {
    rd.batchbsp = (BioseqPtr) nucsep->data.ptrvalue;
  }
  if (rd.batchbsp != NULL) {
    code = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
    SetBatchSuggestNucleotide (rd.batchbsp, code);
  }
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (bfp->input_entityID, (Pointer) (&rd), RecomputeSuggCallback, &gs);
  MonitorFree (rd.mon);
  if (rd.batchbsp != NULL) {
    ClearBatchSuggestNucleotide ();
  }
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean RetranslateOneCDS (SeqFeatPtr sfp, Uint2 entityID, Boolean include_stop)

{
  SeqFeatPtr    bestprot;
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  Char          ch;
  SeqEntryPtr   master;
  MolInfoPtr    mip;
  SeqEntryPtr   old;
  Boolean       partial5;
  Boolean       partial3;
  CharPtr       prot;
  CharPtr       ptr;
  SeqEntryPtr   sep;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  ValNodePtr    vnp;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  slp = SeqLocFindNext (sfp->location, NULL);
  if (slp == NULL) return TRUE;
  CheckSeqLocForPartial (slp, &partial5, &partial3);
  /* if (CdsTranslatesProperly (sfp, gcp)) return TRUE; */

  if (sfp->product == NULL) {
    master = NULL;
    old = NULL;
    bsp = GetBioseqGivenSeqLoc (sfp->location, entityID);
    if (bsp != NULL) {
      master = GetBestTopParentForData (entityID, bsp);
    }
    bsp = BioseqNew ();
    if (bsp != NULL) {
      bsp->mol = Seq_mol_aa;
      bsp->repr = Seq_repr_raw;
      bsp->seq_data_type = Seq_code_ncbieaa;
      bsp->length = 0;
      bsp->seq_data = BSNew (0);
      if (master != NULL) {
        old = SeqEntrySetScope (master);
      }
      bsp->id = MakeNewProteinSeqId (sfp->location, NULL);
      SeqMgrAddToBioseqIndex (bsp);
      if (master != NULL) {
        SeqEntrySetScope (old);
      }
      sep = SeqEntryNew ();
      if (sep != NULL) {
        sep->choice = 1;
        sep->data.ptrvalue = (Pointer) bsp;
        SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, sep);
      }
      SetSeqFeatProduct (sfp, bsp);
      if (master != NULL && sep != NULL) {
        AddSeqEntryToSeqEntry (master, sep, TRUE);
      }
    }
  }

  sip = SeqLocId (sfp->product);
  if (sip != NULL) {
    bsp = BioseqFind (sip);
    if (bsp != NULL && ISA_aa (bsp->mol) && bsp->repr == Seq_repr_raw) {
      bestprot = FindBestProtein (entityID, sfp->product);
      bs = ProteinFromCdRegionEx (sfp, include_stop, FALSE);
      if (bs == NULL) return TRUE;
      prot = BSMerge (bs, NULL);
      bs = BSFree (bs);
      if (prot == NULL) return TRUE;
      ptr = prot;
      ch = *ptr;
      while (ch != '\0') {
        *ptr = TO_UPPER (ch);
        ptr++;
        ch = *ptr;
      }
      bs = BSNew (1000);
      if (bs != NULL) {
        ptr = prot;
        /*
        if (prot [0] == '-') {
          ptr++;
        }
        */
        BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
      }
      bsp->repr = Seq_repr_raw;
      bsp->mol = Seq_mol_aa;
      bsp->seq_data_type = Seq_code_ncbieaa;
      bsp->seq_data = BSFree (bsp->seq_data);
      bsp->seq_data = bs;
      bsp->length = BSLen (bs);
      sep = SeqMgrGetSeqEntryForData (bsp);
      if (sep == NULL) return TRUE;
      if (bestprot != NULL) {
        bestprot->location = SeqLocFree (bestprot->location);
        bestprot->location = CreateWholeInterval (sep);
        SetSeqLocPartial (bestprot->location, partial5, partial3);
        bestprot->partial = (partial5 || partial3);
      }
      vnp = SeqEntryGetSeqDescr (sep, Seq_descr_molinfo, NULL);
      if (vnp == NULL) {
        vnp = CreateNewDescriptor (sep, Seq_descr_molinfo);
        if (vnp != NULL) {
          mip = MolInfoNew ();
          vnp->data.ptrvalue = (Pointer) mip;
          if (mip != NULL) {
            mip->biomol = 8;
            mip->tech = 13;
          }
        }
      }
      if (vnp != NULL) {
        mip = (MolInfoPtr) vnp->data.ptrvalue;
        if (mip != NULL) {
          if (partial5 && partial3) {
            mip->completeness = 5;
          } else if (partial5) {
            mip->completeness = 3;
          } else if (partial3) {
            mip->completeness = 4;
          /*
          } else if (partial) {
            mip->completeness = 2;
          */
          } else {
            mip->completeness = 0;
          }
        }
      }
    }
  }
  return TRUE;
}

static Boolean RetranslateCDSCallback (GatherContextPtr gcp)

{
  RecompDataPtr  rdp;
  SeqFeatPtr     sfp;

  if (gcp == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  rdp = (RecompDataPtr) gcp->userdata;
  if (rdp == NULL) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  return RetranslateOneCDS (sfp, gcp->entityID, rdp->include_stop);
}

static void RetranslateCdRegions (IteM i, Boolean include_stop)

{
  BaseFormPtr  bfp;
  GatherScope  gs;
  RecompData   rd;
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
  rd.count = 0;
  rd.mon = MonitorStrNewEx ("Correcting Coding Regions", 20, FALSE);
  rd.include_stop = include_stop;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (bfp->input_entityID, (Pointer) (&rd), RetranslateCDSCallback, &gs);
  MonitorFree (rd.mon);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

extern void RetranslateCdRegionsNoStop (IteM i)

{
  RetranslateCdRegions (i, FALSE);
}

extern void RetranslateCdRegionsDoStop (IteM i)

{
  RetranslateCdRegions (i, TRUE);
}

static Boolean CorrectGenCodeCallback (GatherContextPtr gcp)

{
  CdRegionPtr     crp;
  GeneticCodePtr  gc;
  Int2            genCode;
  SeqEntryPtr     sep;
  SeqFeatPtr      sfp;
  ValNodePtr      vnp;

  if (gcp == NULL) return TRUE;
  sep = (SeqEntryPtr) gcp->userdata;
  if (sep == NULL ) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  crp = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (crp == NULL) return TRUE;
  genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
  gc = GeneticCodeNew ();
  if (gc == NULL) return TRUE;
  crp->genetic_code = GeneticCodeFree (crp->genetic_code);
  vnp = ValNodeNew (NULL);
  gc->data.ptrvalue = vnp;
  if (vnp != NULL) {
    vnp->choice = 2;
    vnp->data.intvalue = (Int4) genCode;
  }
  crp->genetic_code = gc;
  return TRUE;
}

extern void CorrectGenCodes (SeqEntryPtr sep, Uint2 entityID)

{
  BioseqSetPtr  bssp;
  GatherScope   gs;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (bssp->_class >= 13 && bssp->_class <= 16))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        CorrectGenCodes (sep, entityID);
      }
      return;
    }
  }
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.scope = sep;
  GatherEntity (entityID, (Pointer) sep, CorrectGenCodeCallback, &gs);
}

extern void CorrectCDSGenCodes (IteM i)

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
  CorrectGenCodes (sep, bfp->input_entityID);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static BioseqPtr SqnGetBioseqGivenSeqLoc (SeqLocPtr slp, Uint2 entityID)

{
  BioseqPtr    bsp;
  SeqEntryPtr  sep;
  SeqIdPtr     sip;
  SeqLocPtr    tmp;

  if (slp == NULL) return NULL;
  bsp = NULL;
  sip = SeqLocId (slp);
  if (sip != NULL) {
    bsp = BioseqFind (sip);
  } else {
    tmp = SeqLocFindNext (slp, NULL);
    if (tmp != NULL) {
      sip = SeqLocId (tmp);
      if (sip != NULL) {
        bsp = BioseqFind (sip);
        if (bsp != NULL) {
          sep = SeqMgrGetSeqEntryForData (bsp);
          entityID = ObjMgrGetEntityIDForChoice (sep);
          bsp = GetBioseqGivenSeqLoc (slp, entityID);
        }
      }
    }
  }
  return bsp;
}

static BioseqPtr GetBioseqReferencedByAnnot (SeqAnnotPtr sap, Uint2 entityID)

{
  SeqAlignPtr   align;
  BioseqPtr     bsp;
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  SeqFeatPtr    feat;
  SeqGraphPtr   graph;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  StdSegPtr     ssp;
  SeqLocPtr     tloc;

  if (sap == NULL) return NULL;
  switch (sap->type) {
    case 1 :
      feat = (SeqFeatPtr) sap->data;
      while (feat != NULL) {
        slp = feat->location;
        if (slp != NULL) {
          bsp = SqnGetBioseqGivenSeqLoc (slp, entityID);
          if (bsp != NULL) return bsp;
        }
        feat = feat->next;
      }
      break;
    case 2 :
      align = (SeqAlignPtr) sap->data;
      while (align != NULL) {
        if (align->segtype == 1) {
          ddp = (DenseDiagPtr) align->segs;
          if (ddp != NULL) {
            for (sip = ddp->id; sip != NULL; sip = sip->next) {
              bsp = BioseqFind (sip);
              if (bsp != NULL) return bsp;
            }
          }
        } else if (align->segtype == 2) {
          dsp = (DenseSegPtr) align->segs;
          if (dsp != NULL) {
            for (sip = dsp->ids; sip != NULL; sip = sip->next) {
              bsp = BioseqFind (sip);
              if (bsp != NULL) return bsp;
            }
          }
        } else if (align->segtype == 3) {
          ssp = (StdSegPtr) align->segs;
          if (ssp != NULL && ssp->loc != NULL) {
            for (tloc = ssp->loc; tloc != NULL; tloc = tloc->next) {
              bsp = BioseqFind (SeqLocId (tloc));
              if (bsp != NULL) return bsp;
            }
          }
        }
        align = align->next;
      }
      break;
    case 3 :
      graph = (SeqGraphPtr) sap->data;
      while (graph != NULL) {
        slp = graph->loc;
        if (slp != NULL) {
          bsp = SqnGetBioseqGivenSeqLoc (slp, entityID);
          if (bsp != NULL) return bsp;
        }
        graph = graph->next;
      }
      break;
    default :
      break;
  }
  return NULL;
}

static Int4 GetScore (ScorePtr score)

{
  ObjectIdPtr  id;

  while (score != NULL) {
    id = score->id;
    if (id != NULL) {
      if (StringICmp (id->str, "score") == 0) {
        if (score->choice == 1) {
          return (score->value.intvalue);
        }
      }
    }
    score = score->next;
  }
  return 0;
}

static Int4 FindScore (SeqAlignPtr align)

{
  if (align == NULL) return 0;
  if (align->score != NULL) {
    return GetScore (align->score);
  }
  return 0;
}

static int LIBCALLBACK SortByScoreCallback (VoidPtr ptr1, VoidPtr ptr2)

{
  SeqAlignPtr   sap1;
  SeqAlignPtr   sap2;
  Int4          score1;
  Int4          score2;

  if (ptr1 != NULL && ptr2 != NULL) {
    sap1 = *((SeqAlignPtr PNTR) ptr1);
    sap2 = *((SeqAlignPtr PNTR) ptr2);
    if (sap1 != NULL && sap2 != NULL) {
      score1 = FindScore (sap1);
      score2 = FindScore (sap2);
      if (score1 < score2) {
        return 1;
      } else if (score1 > score2) {
        return -1;
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

static SeqAlignPtr SortBySeqAlignScore (SeqAlignPtr list)

{
  SeqAlignPtr  align;
  Int4         count, i;
  SeqAlignPtr  PNTR head;

  if (list == NULL) return 0;
  count = 0;
  for (align = list; align != NULL; align = align->next) {
    count++;
  }
  head = MemNew (sizeof (SeqAlignPtr) * (size_t) (count + 1));
  if (head == NULL) return 0;
  for (align = list, i = 0; align != NULL && i < count; i++) {
    head [i] = align;
    align = align->next;
  }
  HeapSort (head, (size_t) count, sizeof (SeqAlignPtr), SortByScoreCallback);
  for (i = 0; i < count; i++) {
    align = head [i];
    align->next = head [i + 1];
  }
  list = head [0];
  MemFree (head);
  return list;
}

static void TakeTop10Alignments (SeqAnnotPtr sap)

{
  SeqAlignPtr  align;
  MsgAnswer    ans;
  Int2         count;
  SeqAlignPtr  next;

  if (sap == NULL || sap->type != 2 || sap->data == NULL) return;
  count = 0;
  for (align = (SeqAlignPtr) sap->data; align != NULL; align = align->next) {
    count++;
  }
  if (count <= 10) return;
  ans = Message (MSG_YN, "Do you want to take only the top 10 (out of %d) alignments?", (int) count);
  if (ans == ANS_NO) return;
  sap->data = SortBySeqAlignScore ((SeqAlignPtr) sap->data);
  for (align = (SeqAlignPtr) sap->data, count = 0; align != NULL && count < 10; align = align->next) {
    count++;
  }
  next = align->next;
  align->next = NULL;
  align = next;
  while (align != NULL) {
    next = align->next;
    align->next = NULL;
    SeqAlignFree (align);
    align = next;
  }
}

static void DoOnePub (PubdescPtr pdp)

{
  ValNodePtr    citartptr = NULL;
  Int4          muid = 0;
  Int4          pmid = 0;
  ValNodePtr    tmp = NULL;
  ValNodePtr    vnp;

  if (pdp != NULL) {
    for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
      if (vnp->choice == PUB_Muid) {
        muid = vnp->data.intvalue;
      } else if (vnp->choice == PUB_PMid) {
        pmid = vnp->data.intvalue;
      } else if (vnp->choice == PUB_Article) {
        citartptr = vnp;
      }
    }
    if (pmid != 0) {
      tmp = MedArchGetPubPmId (pmid);
      muid = MedArchPm2Mu (pmid);
    } else if (muid != 0) {
      tmp = MedArchGetPub (muid);
      pmid = MedArchMu2Pm (muid);
    } else if (citartptr != NULL) {
      muid = MedArchCitMatch (citartptr);
      if (muid != 0) {
        tmp = MedArchGetPub (muid);
        pmid = MedArchMu2Pm (muid);
      }
    }
    if (tmp != NULL) {
      MedlineToISO (tmp);
      if (pmid != 0) {
        ValNodeAddInt (&tmp, PUB_PMid, pmid);
      }
      if (muid != 0) {
        ValNodeAddInt (&tmp, PUB_Muid, muid);
      }
      pdp->pub = PubEquivFree (pdp->pub);
      pdp->pub = tmp;
    }
  }
}

static void DoLookupPub (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  PubdescPtr    pdp;
  SeqAnnotPtr   sap;
  ValNodePtr    sdp;
  SeqFeatPtr    sfp;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
    sdp = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
    sdp = bssp->descr;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      for (sfp = (SeqFeatPtr) sap->data; sfp != NULL; sfp = sfp->next) {
        if (sfp->data.choice == SEQFEAT_PUB) {
          pdp = (PubdescPtr) sfp->data.value.ptrvalue;
          DoOnePub (pdp);
        }
      }
    }
    sap = sap->next;
  }
  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_pub) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
      DoOnePub (pdp);
    }
    sdp = sdp->next;
  }
}

extern void LookupAllPubs (IteM i);
extern void LookupAllPubs (IteM i)

{
  BaseFormPtr  bfp;
  MonitorPtr   mon = NULL;
  SeqEntryPtr  sep;
  ErrSev       sev;


  if (! useMedarch) return;
#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  sev = ErrSetMessageLevel (SEV_FATAL);
  WatchCursor ();
  mon = MonitorStrNewEx ("Processing Publications", 40, FALSE);
  MonitorStrValue (mon, "Connecting to MedArch");
  Update ();
  if (! MedArchInit ()) {
    MonitorFree (mon);
    ArrowCursor ();
    Update ();
    Message (MSG_POST, "Unable to connect to MedArch");
    return;
  }
  SeqEntryExplore (sep, NULL, DoLookupPub);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
  MonitorStrValue (mon, "Closing MedArch");
  Update ();
  MedArchFini ();
  MonitorFree (mon);
  ArrowCursor ();
  Update ();
  ErrSetMessageLevel (sev);
  ErrClear ();
  ErrShow ();
}

static void LookupPublications (SeqAnnotPtr sap)

{
  MonitorPtr  mon = NULL;
  PubdescPtr  pdp;
  SeqFeatPtr  sfp;
  ValNodePtr  tmp;
  Int4        uid;
  Boolean     usingMedarch = FALSE;
  ValNodePtr  vnp;

  if (! useMedarch) return;
  if (sap == NULL || sap->type != 1) return;
  for (sfp = (SeqFeatPtr) sap->data; sfp != NULL; sfp = sfp->next) {
    if (sfp->data.choice == SEQFEAT_PUB) {
      pdp = (PubdescPtr) sfp->data.value.ptrvalue;
      if (pdp != NULL) {
        vnp = pdp->pub;
        if (vnp != NULL && vnp->next == NULL) {
          if (vnp->choice == PUB_Muid || vnp->choice == PUB_PMid) {
            if (! usingMedarch) {
              WatchCursor ();
              mon = MonitorStrNewEx ("Processing Publications", 40, FALSE);
              MonitorStrValue (mon, "Connecting to MedArch");
              Update ();
              if (MedArchInit ()) {
                usingMedarch = TRUE;
              } else {
                MonitorFree (mon);
                ArrowCursor ();
                Update ();
                Message (MSG_POST, "Unable to connect to MedArch");
                return;
              }
            }
          }
          tmp = NULL;
          if (vnp->choice == PUB_Muid) {
            uid = vnp->data.intvalue;
            tmp = MedArchGetPub (uid);
          } else if (vnp->choice == PUB_PMid) {
            uid = vnp->data.intvalue;
            tmp = MedArchGetPubPmId (uid);
          }
          if (tmp != NULL) {
            MedlineToISO (tmp);
            tmp->next = vnp;
            pdp->pub = tmp;
          }
        }
      }
    }
  }
  if (usingMedarch) {
    MonitorStrValue (mon, "Closing MedArch");
    Update ();
    MedArchFini ();
    MonitorFree (mon);
    ArrowCursor ();
    Update ();
  }
}

static void PromotePubs (SeqFeatPtr first, BioseqPtr bsp, Uint2 entityID)

{
  MsgAnswer    ans;
  Boolean      asked = FALSE;
  PubdescPtr   pdp;
  SeqDescrPtr  sdp;
  SeqEntryPtr  sep;
  SeqFeatPtr   sfp;
  ValNode      vn;

  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  vn.choice = SEQLOC_WHOLE;
  vn.data.ptrvalue = (Pointer) SeqIdFindBest (bsp->id, 0);
  vn.next = NULL;

  for (sfp = first; sfp != NULL; sfp = sfp->next) {
    if (sfp->data.choice == SEQFEAT_PUB) {
      if (SeqLocCompare (sfp->location, &vn) == SLC_A_EQ_B) {
        if (! asked) {
          ans = Message (MSG_YN, "Do you wish to convert full-length publication features to descriptors?");
          if (ans == ANS_NO) return;
          asked = TRUE;
        }
      }
    }
  }

  sep = GetBestTopParentForData (entityID, bsp);
  for (sfp = first; sfp != NULL; sfp = sfp->next) {
    if (sfp->data.choice == SEQFEAT_PUB) {
      if (SeqLocCompare (sfp->location, &vn) == SLC_A_EQ_B) {
        sfp->idx.deleteme = TRUE;
        sfp->data.choice = SEQFEAT_COMMENT;
        pdp = (PubdescPtr) sfp->data.value.ptrvalue;
        sfp->data.value.ptrvalue = NULL;
        sdp = CreateNewDescriptor (sep, Seq_descr_pub);
        if (sdp != NULL) {
          sdp->data.ptrvalue = (Pointer) pdp;
        }
      }
    }
  }

  DeleteMarkedObjects (entityID, 0, NULL);
}

extern Uint2 SmartAttachSeqAnnotToSeqEntry (Uint2 entityID, SeqAnnotPtr sap)

{
  BioseqPtr      bsp;
  Int2           genCode;
  SeqEntryPtr    oldscope;
  OMProcControl  ompc;
  SeqEntryPtr    sep;
  SeqFeatPtr     sfp = NULL;

  if (sap == NULL) return entityID;
  bsp = GetBioseqReferencedByAnnot (sap, entityID);
  if (bsp == NULL) {
    oldscope = SeqEntrySetScope (NULL);
    if (oldscope != NULL) {
      bsp = GetBioseqReferencedByAnnot (sap, entityID);
      SeqEntrySetScope (oldscope);
    }
  }
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
    entityID = ObjMgrGetEntityIDForChoice (sep);
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      sep = GetBestTopParentForData (entityID, bsp);
      genCode = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
      SetEmptyGeneticCodes (sap, genCode);
      LookupPublications (sap);
    } else if (sap->type == 2) {
      TakeTop10Alignments (sap);
    }
    MemSet ((Pointer) &ompc, 0, sizeof (OMProcControl));
    ompc.input_entityID = entityID;
    ompc.input_itemID = GetItemIDGivenPointer (entityID, OBJ_BIOSEQ, (Pointer) bsp);
    ompc.input_itemtype = OBJ_BIOSEQ;
    ompc.output_itemtype = OBJ_SEQANNOT;
    ompc.output_data = (Pointer) sap;
    if (! AttachDataForProc (&ompc, FALSE)) {
      Message (MSG_ERROR, "SmartAttachSeqAnnotToSeqEntry failed");
    } else if (sfp != NULL) {
      PromoteXrefs (sfp, bsp, entityID);
      PromotePubs (sfp, bsp, entityID);
    }
  } else {
    Message (MSG_ERROR, "Feature table identifiers do not match record");
  }
  return entityID;
}

typedef struct removeformdata {
  FEATURE_FORM_BLOCK

  Boolean        is_feature;
  LisT           objlist;
  TexT           findthis;
  Uint2          itemtype;
  Uint2          subtype;
  ObjMgrPtr      omp;
  ObjMgrTypePtr  omtp;
  ValNodePtr     head;
  Boolean        stringfound;
  Char           findStr [128];
  ValNodePtr     bsplist;
  ValNodePtr     bssplist;
} RemoveFormData, PNTR RemoveFormPtr;

static void LIBCALLBACK AsnWriteRemoveForDCallBack (AsnExpOptStructPtr pAEOS)

{
  CharPtr        pchFind;
  CharPtr        pchSource;
  RemoveFormPtr  rfp;

  rfp = (RemoveFormPtr) pAEOS->data;
  if (ISA_STRINGTYPE (AsnFindBaseIsa (pAEOS->atp))) {
	pchSource = (CharPtr) pAEOS->dvp->ptrvalue;
	pchFind = rfp->findStr;
	if (StringSearch (pchSource, pchFind) != NULL) {
	  rfp->stringfound = TRUE;
	}
  }
}

static Boolean ObjectHasSubstring (ObjMgrTypePtr omtp, AsnIoPtr aip, Pointer ptr, RemoveFormPtr rfp)

{
  rfp->stringfound = FALSE;
  (omtp->asnwrite) (ptr, aip, NULL);
  return rfp->stringfound;
}

static void RemoveFeatureCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  AsnExpOptPtr   aeop;
  AsnIoPtr       aip;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  SeqAnnotPtr    nextsap;
  SeqFeatPtr     nextsfp;
  Boolean        notext;
  ObjMgrTypePtr  omtp;
  Pointer PNTR   prevsap;
  Pointer PNTR   prevsfp;
  BioseqPtr      productbsp;
  BioseqPtr      productcdna;
  BioseqSetPtr   productnps;
  RemoveFormPtr  rfp;
  SeqAnnotPtr    sap;
  SeqFeatPtr     sfp;
  SeqIdPtr       sip;
  Uint2          subtype;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  rfp = (RemoveFormPtr) mydata;
  if (rfp == NULL) return;
  omtp = rfp->omtp;
  if (omtp == NULL || omtp->subtypefunc == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
    prevsap = (Pointer PNTR) &(bsp->annot);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
    prevsap = (Pointer PNTR) &(bssp->annot);
  } else return;
  GetTitle (rfp->findthis, rfp->findStr, sizeof (rfp->findStr) - 1);
  notext = StringHasNoText (rfp->findStr);
  aip = AsnIoNullOpen ();
  aeop = AsnExpOptNew (aip, NULL, NULL, AsnWriteRemoveForDCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) rfp;
  }
  while (sap != NULL) {
    nextsap = sap->next;
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      prevsfp = (Pointer PNTR) &(sap->data);
      while (sfp != NULL) {
        nextsfp = sfp->next;
        subtype = (*(omtp->subtypefunc)) ((Pointer) sfp);
        if (subtype == rfp->subtype ||
           (rfp->subtype == FEATDEF_IMP &&
            subtype >= FEATDEF_allele && subtype <= FEATDEF_site_ref)) {
          if (notext || ObjectHasSubstring (omtp, aip, (Pointer) sfp, rfp)) {
            if (sfp->data.choice == SEQFEAT_CDREGION) {
              if (sfp->product != NULL) {
                sip = SeqLocId (sfp->product);
                if (sip != NULL) {
                  productbsp = BioseqFind (sip);
                  if (productbsp != NULL) {
                    ValNodeAddPointer (&(rfp->bsplist), 0, (Pointer) productbsp);
                  }
                }
              }
            } else if (sfp->data.choice == SEQFEAT_RNA) {
              if (sfp->product != NULL) {
                sip = SeqLocId (sfp->product);
                if (sip != NULL) {
                  productcdna = BioseqFind (sip);
                  if (productcdna != NULL && productcdna->idx.parenttype == OBJ_BIOSEQSET) {
                    productnps = (BioseqSetPtr) productcdna->idx.parentptr;
                    if (productnps != NULL && productnps->_class == BioseqseqSet_class_nuc_prot) {
                      ValNodeAddPointer (&(rfp->bssplist), 0, (Pointer) productnps);
                    }
                  }
                }
              }
            }
            *(prevsfp) = sfp->next;
            sfp->next = NULL;
            SeqFeatFree (sfp);
          } else {
            prevsfp = (Pointer PNTR) &(sfp->next);
          }
        } else {
          prevsfp = (Pointer PNTR) &(sfp->next);
        }
        sfp = nextsfp;
      }
    }
    if (sap->data == NULL) {
      *(prevsap) = sap->next;
      sap->next = NULL;
      SeqAnnotFree (sap);
    } else {
      prevsap = (Pointer PNTR) &(sap->next);
    }
    sap = nextsap;
  }
  AsnIoClose (aip);
}

static void RemoveDescriptorCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  AsnExpOptPtr   aeop;
  AsnIoPtr       aip;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  ValNodePtr     nextsdp;
  Boolean        notext;
  ObjMgrTypePtr  omtp;
  Pointer PNTR   prevsdp;
  RemoveFormPtr  rfp;
  ValNodePtr     sdp;
  Uint2          subtype;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  rfp = (RemoveFormPtr) mydata;
  if (rfp == NULL) return;
  omtp = rfp->omtp;
  if (omtp == NULL || omtp->subtypefunc == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else return;
  GetTitle (rfp->findthis, rfp->findStr, sizeof (rfp->findStr) - 1);
  notext = StringHasNoText (rfp->findStr);
  aip = AsnIoNullOpen ();
  aeop = AsnExpOptNew (aip, NULL, NULL, AsnWriteRemoveForDCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) rfp;
  }
  while (sdp != NULL) {
    nextsdp = sdp->next;
    subtype = (*(omtp->subtypefunc)) ((Pointer) sdp);
    if (subtype == rfp->subtype) {
      if (notext || ObjectHasSubstring (omtp, aip, (Pointer) sdp, rfp)) {
        *(prevsdp) = sdp->next;
        sdp->next = NULL;
        SeqDescFree (sdp);
      } else {
        prevsdp = (Pointer PNTR) &(sdp->next);
      }
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
  AsnIoClose (aip);
}

static void DoRemoveAsnObject (ButtoN b)

{
  MsgAnswer      ans;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  Uint2          itemID;
  OMProcControl  ompc;
  RemoveFormPtr  rfp;
  SeqEntryPtr    sep;
  ValNodePtr     tmp;
  Int2           val;
  ValNodePtr     vnp;

  rfp = GetObjectExtra (b);
  if (rfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (rfp->input_entityID);
  if (sep == NULL) return;
  Hide (rfp->form);
  WatchCursor ();
  Update ();
  if (rfp->is_feature) {
    rfp->itemtype = OBJ_SEQFEAT;
  } else {
    rfp->itemtype = OBJ_SEQDESC;
  }
  vnp = NULL;
  val = GetValue (rfp->objlist);
  if (val > 0) {
    vnp = rfp->head;
    while (vnp != NULL && val > 1) {
      val--;
      vnp = vnp->next;
    }
  }
  if (vnp != NULL) {
    rfp->omp = ObjMgrGet ();
    rfp->omtp = NULL;
    if (rfp->omp != NULL) {
      rfp->omtp = ObjMgrTypeFind (rfp->omp, rfp->itemtype, NULL, NULL);
    }
    rfp->subtype = vnp->choice;
    if (rfp->itemtype != 0 && rfp->subtype != 0 && rfp->omtp != NULL) {
      if (rfp->is_feature) {
        SeqEntryExplore (sep, (Pointer) rfp, RemoveFeatureCallback);
        if (rfp->bsplist != NULL) {
          ans = Message (MSG_YN, "Remove protein products?");
          if (ans == ANS_YES) {
            for (tmp = rfp->bsplist; tmp != NULL; tmp = tmp->next) {
              bsp = (BioseqPtr) tmp->data.ptrvalue;
              itemID = GetItemIDGivenPointer (rfp->input_entityID, OBJ_BIOSEQ, (Pointer) bsp);
              if (itemID > 0) {
                MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
                ompc.do_not_reload_from_cache = TRUE;
                ompc.input_entityID = rfp->input_entityID;
                ompc.input_itemID = itemID;
                ompc.input_itemtype = OBJ_BIOSEQ;
                if (! DetachDataForProc (&ompc, FALSE)) {
                  Message (MSG_POSTERR, "DetachDataForProc failed");
                }
              }
            }
          }
        }
        if (rfp->bssplist != NULL) {
          ans = Message (MSG_YN, "Remove cDNA nuc-prot products?");
          if (ans == ANS_YES) {
            for (tmp = rfp->bssplist; tmp != NULL; tmp = tmp->next) {
              bssp = (BioseqSetPtr) tmp->data.ptrvalue;
              itemID = GetItemIDGivenPointer (rfp->input_entityID, OBJ_BIOSEQSET, (Pointer) bssp);
              if (itemID > 0) {
                MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
                ompc.do_not_reload_from_cache = TRUE;
                ompc.input_entityID = rfp->input_entityID;
                ompc.input_itemID = itemID;
                ompc.input_itemtype = OBJ_BIOSEQSET;
                if (! DetachDataForProc (&ompc, FALSE)) {
                  Message (MSG_POSTERR, "DetachDataForProc failed");
                }
              }
            }
          }
        }
      } else {
        SeqEntryExplore (sep, (Pointer) rfp, RemoveDescriptorCallback);
      }
    }
  }
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (rfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, rfp->input_entityID, 0, 0);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
  Remove (rfp->form);
}

int LIBCALLBACK SortByVnpChoice (VoidPtr ptr1, VoidPtr ptr2)

{
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      if (vnp1->choice > vnp2->choice) {
        return 1;
      } else if (vnp1->choice < vnp2->choice) {
        return -1;
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

static void RemoveMessageProc (ForM f, Int2 mssg)

{
  RemoveFormPtr  rfp;

  rfp = (RemoveFormPtr) GetObjectExtra (f);
  if (rfp != NULL) {
    if (rfp->appmessage != NULL) {
      rfp->appmessage (f, mssg);
    }
  }
}

static void CleanupRemovePage (GraphiC g, VoidPtr data)

{
  RemoveFormPtr  rfp;

  rfp = (RemoveFormPtr) data;
  if (rfp != NULL) {
    ValNodeFreeData (rfp->head);
    ValNodeFree (rfp->bsplist);
    ValNodeFree (rfp->bssplist);
  }
  StdCleanupFormProc (g, data);
}

static CharPtr descNames [] = {
  " ", "MolType", "Modif", "Method", "Name",
  "Title", "Organism", "Comment", "Numbering",
  "MapLoc", "PIR", "GenBank", "Publication",
  "Region", "User", "SWISS-PROT", "dbXREF",
  "EMBL", "Create Date", "Update Date", "PRF",
  "PDB", "Heterogen", "BioSource", "MolInfo", NULL
};

/*
#ifdef INTERNAL_NCBI_SEQUIN
#define LISTHEIGHT 16
#else
#define LISTHEIGHT 8
#endif
*/

static void RemoveAsnObject (IteM i, Boolean feature)

{
  BaseFormPtr        bfp;
  ButtoN             b;
  GrouP              c;
  FeatDefPtr         curr;
  GrouP              g;
  GrouP              h;
  ValNodePtr         head;
  Int2               j;
  GrouP              k;
  Uint1              key;
  CharPtr            label = NULL;
  Int2               listHeight;
  RemoveFormPtr      rfp;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  Char               str [256];
  Uint2              subtype;
  CharPtr            title;
  ValNodePtr         vnp;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  rfp = (RemoveFormPtr) MemNew (sizeof (RemoveFormData));
  if (rfp == NULL) return;
  if (feature) {
    title = "Feature Removal";
  } else {
    title = "Descriptor Removal";
  }
  w = FixedWindow (-50, -33, -10, -10, title, StdCloseWindowProc);
  SetObjectExtra (w, rfp, CleanupRemovePage);
  rfp->form = (ForM) w;
  rfp->formmessage = RemoveMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    rfp->appmessage = sepp->handleMessages;
  }

  rfp->input_entityID = bfp->input_entityID;
  rfp->input_itemID = bfp->input_itemID;
  rfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 0, 2, NULL);
  rfp->is_feature = feature;
  if (feature) {
    StaticPrompt (g, "Feature", 0, 0, programFont, 'c');
  } else {
    StaticPrompt (g, "Descriptor", 0, 0, programFont, 'c');
  }
  if (indexerVersion) {
    listHeight = 16;
  } else {
    listHeight = 8;
  }
  rfp->objlist = SingleList (g, 16, listHeight, NULL);
  head = NULL;
  if (feature) {
    curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
    while (curr != NULL) {
      if (key != FEATDEF_BAD) {
        subtype = curr->featdef_key;
        vnp = ValNodeNew (head);
        if (head == NULL) {
          head = vnp;
        }
        if (vnp != NULL) {
          vnp->choice = subtype;
          if (subtype != FEATDEF_misc_RNA &&
              subtype != FEATDEF_precursor_RNA &&
              subtype != FEATDEF_mat_peptide &&
              subtype != FEATDEF_sig_peptide &&
              subtype != FEATDEF_transit_peptide) {
            vnp->data.ptrvalue = StringSave (curr->typelabel);
          } else {
            StringNCpy_0 (str, curr->typelabel, sizeof (str) - 10);
            StringCat (str, "_imp");
            vnp->data.ptrvalue = StringSave (str);
          }
        }
      }
      curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
    }
  } else {
    for (j = 1; descNames [j] != NULL; j++) {
      vnp = ValNodeNew (head);
      if (head == NULL) {
        head = vnp;
      }
      if (vnp != NULL) {
        vnp->choice = j;
        vnp->data.ptrvalue = StringSave (descNames [j]);
      }
    }
  }
  if (head != NULL) {
    head = SortValNode (head, SortByVnpChoice);
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      ListItem (rfp->objlist, (CharPtr) vnp->data.ptrvalue);
    }
  }
  rfp->head = head;
  rfp->bsplist = NULL;
  rfp->bssplist = NULL;

  k = HiddenGroup (h, 0, 2, NULL);
  StaticPrompt (k, "Optional string constraint", 0, dialogTextHeight, programFont, 'c');
  rfp->findthis = DialogText (k, "", 14, NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoRemoveAsnObject);
  SetObjectExtra (b, rfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) k, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

extern void RemoveFeature (IteM i)

{
  RemoveAsnObject (i, TRUE);
}

extern void RemoveDescriptor (IteM i)

{
  RemoveAsnObject (i, FALSE);
}

#define SLCT_FEAT    1
#define SLCT_DESC    2
#define SLCT_BIOSEQ  3

typedef struct selectformdata {
  FEATURE_FORM_BLOCK

  Int2           type;
  LisT           objlist;
  TexT           findthis;
  Uint2          itemtype;
  Uint2          subtype;
  ObjMgrPtr      omp;
  ObjMgrTypePtr  omtp;
  ValNodePtr     head;
  Boolean        stringfound;
  Char           findStr [128];
} SelectFormData, PNTR SelectFormPtr;

static Boolean SelectObjCallback (GatherContextPtr gcp)

{
  ObjMgrTypePtr  omtp;
  SelectFormPtr  selfp;
  Uint2          subtype;

  if (gcp == NULL) return TRUE;
  selfp = (SelectFormPtr) gcp->userdata;
  if (selfp == NULL) return TRUE;
  if (gcp->thistype != selfp->itemtype) return TRUE;
  omtp = selfp->omtp;
  if (omtp == NULL) return TRUE;
  subtype = (*(omtp->subtypefunc)) ((Pointer) gcp->thisitem);
  if (subtype != selfp->subtype) return TRUE;
  ObjMgrAlsoSelect (gcp->entityID, gcp->itemID, gcp->thistype, 0, NULL);
  return TRUE;
}

static void DoSelectAsnObject (ButtoN b)

{
  GatherScope    gs;
  SelectFormPtr  selfp;
  SeqEntryPtr    sep;
  Int2           val;
  ValNodePtr     vnp;

  selfp = GetObjectExtra (b);
  if (selfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (selfp->input_entityID);
  if (sep == NULL) return;
  Hide (selfp->form);
  switch (selfp->type) {
    case SLCT_FEAT :
      selfp->itemtype = OBJ_SEQFEAT;
      break;
    case SLCT_DESC :
      selfp->itemtype = OBJ_SEQDESC;
      break;
    case SLCT_BIOSEQ :
      selfp->itemtype = OBJ_BIOSEQ;
      break;
    default :
      Remove (selfp->form);
      Update ();
      return;
  }
  WatchCursor ();
  Update ();
  vnp = NULL;
  val = GetValue (selfp->objlist);
  if (val > 0) {
    vnp = selfp->head;
    while (vnp != NULL && val > 1) {
      val--;
      vnp = vnp->next;
    }
  }
  if (selfp->type == SLCT_BIOSEQ || vnp != NULL) {
    selfp->omp = ObjMgrGet ();
    selfp->omtp = NULL;
    if (selfp->omp != NULL) {
      selfp->omtp = ObjMgrTypeFind (selfp->omp, selfp->itemtype, NULL, NULL);
    }
    if (selfp->type == SLCT_BIOSEQ) {
      selfp->subtype = Seq_repr_raw;
    } else {
      selfp->subtype = vnp->choice;
    }
    if (selfp->itemtype != 0 && selfp->subtype != 0 && selfp->omtp != NULL) {
      MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
      gs.seglevels = 1;
      MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
      gs.ignore[OBJ_BIOSEQ] = FALSE;
      gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
      gs.ignore[OBJ_SEQANNOT] = FALSE;
      gs.ignore[OBJ_SEQFEAT] = FALSE;
      gs.ignore[OBJ_SEQDESC] = FALSE;
      GatherEntity (selfp->input_entityID, (Pointer) selfp, SelectObjCallback, &gs);
    }
  }
  ArrowCursor ();
  Update ();
  /* ObjMgrSendMsg (OM_MSG_UPDATE, selfp->input_entityID, 0, 0); */
  Remove (selfp->form);
}

static void SelectMessageProc (ForM f, Int2 mssg)

{
  SelectFormPtr  selfp;

  selfp = (SelectFormPtr) GetObjectExtra (f);
  if (selfp != NULL) {
    if (selfp->appmessage != NULL) {
      selfp->appmessage (f, mssg);
    }
  }
}

static void CleanupSelectPage (GraphiC g, VoidPtr data)

{
  SelectFormPtr  selfp;

  selfp = (SelectFormPtr) data;
  if (selfp != NULL) {
    ValNodeFreeData (selfp->head);
  }
  StdCleanupFormProc (g, data);
}

static void SelectAsnObject (IteM i, Int2 type)

{
  BaseFormPtr        bfp;
  ButtoN             b;
  GrouP              c;
  FeatDefPtr         curr;
  GrouP              g;
  GrouP              h;
  ValNodePtr         head;
  Int2               j;
  Uint1              key;
  CharPtr            label = NULL;
  Int2               listHeight;
  SelectFormPtr      selfp;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  Uint2              subtype;
  CharPtr            title;
  ValNodePtr         vnp;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  selfp = (SelectFormPtr) MemNew (sizeof (SelectFormData));
  if (selfp == NULL) return;
  switch (type) {
    case SLCT_FEAT :
      title = "Feature Selection";
      break;
    case SLCT_DESC :
      title = "Descriptor Selection";
      break;
    case SLCT_BIOSEQ :
      title = "Sequence Selection";
      break;
    default :
      title = "? Selection";
      break;
  }
  w = FixedWindow (-50, -33, -10, -10, title, StdCloseWindowProc);
  SetObjectExtra (w, selfp, CleanupSelectPage);
  selfp->form = (ForM) w;
  selfp->formmessage = SelectMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    selfp->appmessage = sepp->handleMessages;
  }

  selfp->input_entityID = bfp->input_entityID;
  selfp->input_itemID = bfp->input_itemID;
  selfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 0, 2, NULL);
  selfp->type = type;
  switch (type) {
    case SLCT_FEAT :
      StaticPrompt (g, "Feature", 0, 0, programFont, 'c');
      break;
    case SLCT_DESC :
      StaticPrompt (g, "Descriptor", 0, 0, programFont, 'c');
      break;
    case SLCT_BIOSEQ :
      StaticPrompt (g, "Sequence", 0, 0, programFont, 'c');
      break;
    default :
      break;
  }
  if (indexerVersion) {
    listHeight = 16;
  } else {
    listHeight = 8;
  }
  selfp->objlist = SingleList (g, 16, listHeight, NULL);
  head = NULL;
  if (type == SLCT_FEAT) {
    curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
    while (curr != NULL) {
      if (key != FEATDEF_BAD) {
        subtype = curr->featdef_key;
        vnp = ValNodeNew (head);
        if (head == NULL) {
          head = vnp;
        }
        if (vnp != NULL) {
          vnp->choice = subtype;
          vnp->data.ptrvalue = StringSave (curr->typelabel);
        }
      }
      curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
    }
  } else if (type == SLCT_DESC) {
    for (j = 1; descNames [j] != NULL; j++) {
      vnp = ValNodeNew (head);
      if (head == NULL) {
        head = vnp;
      }
      if (vnp != NULL) {
        vnp->choice = j;
        vnp->data.ptrvalue = StringSave (descNames [j]);
      }
    }
  }
  if (head != NULL) {
    head = SortValNode (head, SortByVnpChoice);
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      ListItem (selfp->objlist, (CharPtr) vnp->data.ptrvalue);
    }
  }
  selfp->head = head;

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoSelectAsnObject);
  SetObjectExtra (b, selfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  if (type == SLCT_BIOSEQ) {
    DoSelectAsnObject (b);
    Update ();
    return;
  }
  Show (w);
  Update ();
}

extern void SelectFeature (IteM i)

{
  SelectAsnObject (i, SLCT_FEAT);
}

extern void SelectDescriptor (IteM i)

{
  SelectAsnObject (i, SLCT_DESC);
}

extern void SelectBioseq (IteM i)

{
  SelectAsnObject (i, SLCT_BIOSEQ);
}

typedef struct fuseformdata {
  FEATURE_FORM_BLOCK

  LisT           objlist;
  Uint2          subtype;
  ObjMgrPtr      omp;
  ObjMgrTypePtr  omtp;
  ValNodePtr     head;
} FuseFormData, PNTR FuseFormPtr;

static void FuseFeatureCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  FuseFormPtr    ffp;
  SeqFeatPtr     first;
  SeqFeatPtr     nextsfp;
  ObjMgrTypePtr  omtp;
  Pointer PNTR   prevsfp;
  SeqAnnotPtr    sap;
  SeqFeatPtr     sfp;
  SeqLocPtr      slp;
  Uint2          subtype;
  BioseqPtr      target;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  ffp = (FuseFormPtr) mydata;
  if (ffp == NULL) return;
  omtp = ffp->omtp;
  if (omtp == NULL || omtp->subtypefunc == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  sap = bsp->annot;
  first = NULL;
  while (sap != NULL) {
     if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      prevsfp = (Pointer PNTR) &(sap->data);
      while (sfp != NULL) {
        nextsfp = sfp->next;
        subtype = (*(omtp->subtypefunc)) ((Pointer) sfp);
        if (subtype == ffp->subtype ||
           (ffp->subtype == FEATDEF_IMP &&
            subtype >= FEATDEF_allele && subtype <= FEATDEF_site_ref)) {
          if (first != NULL) {
            target = GetBioseqGivenSeqLoc (sfp->location, ffp->input_entityID);
            if (target != NULL) {
              slp = SeqLocMerge (target, sfp->location, first->location, FALSE, TRUE, FALSE);
              first->location = SeqLocFree (first->location);
              first->location = slp;
              first->partial = CheckSeqLocForPartial (slp, NULL, NULL);
            }
            *(prevsfp) = sfp->next;
            sfp->next = NULL;
            SeqFeatFree (sfp);
          } else {
            first = sfp;
            prevsfp = (Pointer PNTR) &(sfp->next);
          }
        }
        sfp = nextsfp;
      }
    }
    sap = sap->next;
  }
}

static void DoFuseFeature (ButtoN b)

{
  FuseFormPtr  ffp;
  SeqEntryPtr  sep;
  Int2         val;
  ValNodePtr   vnp;

  ffp = (FuseFormPtr) GetObjectExtra (b);
  if (ffp == NULL) return;
  sep = GetTopSeqEntryForEntityID (ffp->input_entityID);
  if (sep == NULL) return;
  Hide (ffp->form);
  WatchCursor ();
  Update ();

  vnp = NULL;
  val = GetValue (ffp->objlist);
  if (val > 0) {
    vnp = ffp->head;
    while (vnp != NULL && val > 1) {
      val--;
      vnp = vnp->next;
    }
  }
  if (vnp != NULL) {
    ffp->omp = ObjMgrGet ();
    ffp->omtp = NULL;
    if (ffp->omp != NULL) {
      ffp->omtp = ObjMgrTypeFind (ffp->omp, OBJ_SEQFEAT, NULL, NULL);
    }
    ffp->subtype = vnp->choice;
    if (ffp->subtype != 0 && ffp->omtp != NULL) {
      SeqEntryExplore (sep, (Pointer) ffp, FuseFeatureCallback);
    }
  }

  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (ffp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ffp->input_entityID, 0, 0);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
  Remove (ffp->form);
}

static void FuseMessageProc (ForM f, Int2 mssg)

{
  FuseFormPtr  ffp;

  ffp = (FuseFormPtr) GetObjectExtra (f);
  if (ffp != NULL) {
    if (ffp->appmessage != NULL) {
      ffp->appmessage (f, mssg);
    }
  }
}

static void CleanupFusePage (GraphiC g, VoidPtr data)

{
  FuseFormPtr  ffp;

  ffp = (FuseFormPtr) data;
  if (ffp != NULL) {
    ValNodeFreeData (ffp->head);
  }
  StdCleanupFormProc (g, data);
}

extern void FuseFeature (IteM i)

{
  BaseFormPtr        bfp;
  ButtoN             b;
  GrouP              c;
  FeatDefPtr         curr;
  FuseFormPtr        ffp;
  GrouP              g;
  GrouP              h;
  ValNodePtr         head;
  Uint1              key;
  CharPtr            label = NULL;
  Int2               listHeight;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  Uint2              subtype;
  ValNodePtr         vnp;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  ffp = (FuseFormPtr) MemNew (sizeof (FuseFormData));
  if (ffp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Fuse Feature", StdCloseWindowProc);
  SetObjectExtra (w, ffp, CleanupFusePage);
  ffp->form = (ForM) w;
  ffp->formmessage = FuseMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    ffp->appmessage = sepp->handleMessages;
  }

  ffp->input_entityID = bfp->input_entityID;
  ffp->input_itemID = bfp->input_itemID;
  ffp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 0, 2, NULL);
  StaticPrompt (g, "Feature", 0, 0, programFont, 'c');
  if (indexerVersion) {
    listHeight = 16;
  } else {
    listHeight = 8;
  }
  ffp->objlist = SingleList (g, 16, listHeight, NULL);
  head = NULL;
  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key != FEATDEF_BAD) {
      subtype = curr->featdef_key;
      if (subtype != FEATDEF_misc_RNA &&
          subtype != FEATDEF_precursor_RNA &&
          subtype != FEATDEF_mat_peptide &&
          subtype != FEATDEF_sig_peptide &&
          subtype != FEATDEF_transit_peptide) {
        vnp = ValNodeNew (head);
        if (head == NULL) {
          head = vnp;
        }
        if (vnp != NULL) {
          vnp->choice = subtype;
          vnp->data.ptrvalue = StringSave (curr->typelabel);
        }
      }
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }
  if (head != NULL) {
    head = SortValNode (head, SortByVnpChoice);
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      ListItem (ffp->objlist, (CharPtr) vnp->data.ptrvalue);
    }
  }
  ffp->head = head;

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoFuseFeature);
  SetObjectExtra (b, ffp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}


extern Int2 LIBCALLBACK RefGeneUserGenFunc (Pointer data);

#define REFGENE_ASSEMBLY   1
#define REFGENE_RELATED    2
#define REFGENE_SPLICEVAR  3
#define REFGENE_RELDREK    4
#define REFGENE_REJECT     5
#define REFGENE_UNKNOWN    6

typedef struct refgeneuserdialog {
  DIALOG_MESSAGE_BLOCK
  GrouP         status;
  TexT          curator;
  DialoG        fields;
} RefgeneUserDialog, PNTR RefgeneUserDialogPtr;

typedef struct refgeneuserform {
  FEATURE_FORM_BLOCK
  SeqEntryPtr   sep;
} RefgeneUserForm, PNTR RefgeneUserFormPtr;

static ENUM_ALIST(changeflags_alist)
  {" ",           0},
  {"Sequence",    1},
  {"Annotation",  2},
  {"Both",        3},
END_ENUM_ALIST

static ENUM_ALIST(refgene_alist)
  {" ",          0},
  {"Assembly",    REFGENE_ASSEMBLY},
  {"Related",     REFGENE_RELATED},
  {"SpliceVar",   REFGENE_SPLICEVAR},
  {"RelatedDrek", REFGENE_RELDREK},
  {"Reject",      REFGENE_REJECT},
  {"Unknown",     REFGENE_UNKNOWN},
END_ENUM_ALIST

static Uint2 refgene_types [] = {
  TAGLIST_TEXT, TAGLIST_TEXT, TAGLIST_TEXT, TAGLIST_POPUP, TAGLIST_POPUP
};

static Uint2 refgene_widths [] = {
  9, 7, 15, 0, 0
};

static EnumFieldAssocPtr refgene_popups [] = {
  NULL, NULL, NULL, changeflags_alist, refgene_alist
};

static CharPtr refgene_labels [] = {
  "", "Assembly", "Related", "SpliceVar", "RelatedDrek", "Reject", "Unknown", NULL
};

static CharPtr refgene_fields [] = {
  "Accession", "GI", "Comment", "Change", "Type", NULL
};

static void AccessionUserFieldPtrToVisStringDialog (DialoG d, Pointer data)

{
  CharPtr       accession;
  Boolean       annotChange;
  CharPtr       comment;
  UserFieldPtr  curr;
  UserFieldPtr  entry;
  Int2          field;
  Int2          flags;
  Int4          from;
  Int4          gi;
  ValNodePtr    head;
  Int2          i;
  Int2          j;
  ObjectIdPtr   oip;
  Boolean       seqChange;
  CharPtr       str;
  TagListPtr    tlp;
  Int4          to;
  UserFieldPtr  ufp;
  ValNodePtr    vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp == NULL) return;
  str = MemNew (sizeof (Char) * 1024);
  head = NULL;
  curr = (UserFieldPtr) data;
  while (curr != NULL) {
    oip = curr->label;
    if (oip != NULL) {
      field = 0;
      for (i = REFGENE_ASSEMBLY; i <= REFGENE_UNKNOWN; i++) {
        if (StringICmp (oip->str, refgene_labels [i]) == 0 && curr->choice == 11) {
          field = i;
        }
      }
      if (field > 0) {
        entry = (UserFieldPtr) curr->data.ptrvalue;
        while (entry != NULL && entry->choice == 11) {
          accession = NULL;
          comment = NULL;
          gi = 0;
          from = 0;
          to = 0;
          annotChange = FALSE;
          seqChange = FALSE;
          ufp = (UserFieldPtr) entry->data.ptrvalue;
          while (ufp != NULL) {
            oip = ufp->label;
            if (oip != NULL && oip->str != NULL) {
              if (StringICmp (oip->str, "accession") == 0 && ufp->choice == 1) {
                accession = (CharPtr) ufp->data.ptrvalue;
              } else if (StringICmp (oip->str, "gi") == 0 && ufp->choice == 2) {
                gi = ufp->data.intvalue;
              } else if (StringICmp (oip->str, "from") == 0 && ufp->choice == 2) {
                from = ufp->data.intvalue;
              } else if (StringICmp (oip->str, "to") == 0 && ufp->choice == 2) {
                to = ufp->data.intvalue;
              } else if (StringICmp (oip->str, "sequenceChange") == 0 && ufp->choice == 4) {
                seqChange = ufp->data.boolvalue;
              } else if (StringICmp (oip->str, "annotationChange") == 0 && ufp->choice == 4) {
                annotChange = ufp->data.boolvalue;
              } else if (StringICmp (oip->str, "comment") == 0 && ufp->choice == 1) {
                comment = (CharPtr) ufp->data.ptrvalue;
              }
            }
            ufp = ufp->next;
          }
          if (accession != NULL) {
            if (comment == NULL) {
              comment = "";
            }
            flags = 0;
            if (seqChange) {
              flags++;
            }
            if (annotChange) {
              flags += 2;
            }
            if (gi != 0) {
              sprintf (str, "%s\t%ld\t%s\t%d\t%d\n", accession, (long) gi, comment, (int) flags, (int) field);
            } else {
              sprintf (str, "%s\t\t%s\t%d\t%d\n", accession, comment, (int) flags, (int) field);
            }
            vnp = ValNodeNew (head);
            if (head == NULL) {
              head = vnp;
            }
            if (vnp != NULL) {
              vnp->data.ptrvalue = StringSave (str);
            }
          }
          entry = entry->next;
        }
      }
    }
    curr = curr->next;
  }
  MemFree (str);
  SendMessageToDialog (tlp->dialog, VIB_MSG_RESET);
  tlp->vnp = head;
  SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
  for (j = 0, vnp = tlp->vnp; vnp != NULL; j++, vnp = vnp->next) {
  }
  tlp->max = MAX ((Int2) 0, (Int2) (j - tlp->rows + 1));
  CorrectBarMax (tlp->bar, tlp->max);
  CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
}

static Pointer VisStringDialogToUserFieldPtr (DialoG d)

{
  return NULL;
}

static void UserObjectPtrToRefGeneDialog (DialoG d, Pointer data)

{
  UserFieldPtr          curr;
  ObjectIdPtr           oip;
  RefgeneUserDialogPtr  rdp;
  Int2                  status = 0;
  CharPtr               str;
  UserObjectPtr         uop;

  rdp = (RefgeneUserDialogPtr) GetObjectExtra (d);
  if (rdp == NULL) return;
  uop = (UserObjectPtr) data;
  if (uop == NULL || uop->type == NULL || StringICmp (uop->type->str, "RefGeneTracking") != 0) {
    SetValue (rdp->status, 0);
    PointerToDialog (rdp->fields, NULL);
    return;
  }
  PointerToDialog (rdp->fields, uop->data);
  for (curr = uop->data; curr != NULL; curr = curr->next) {
    oip = curr->label;
    if (oip != NULL && StringICmp (oip->str, "Status") == 0) {
      break;
    }
  }
  if (curr != NULL && curr->choice == 1) {
    str = (CharPtr) curr->data.ptrvalue;
    if (StringICmp (str, "Predicted") == 0) {
      status = 1;
    } else if (StringICmp (str, "Provisional") == 0) {
      status = 2;
    } else if (StringICmp (str, "Validated") == 0) {
      status = 3;
    } else if (StringICmp (str, "Reviewed") == 0) {
      status = 4;
    }
  }
  for (curr = uop->data; curr != NULL; curr = curr->next) {
    oip = curr->label;
    if (oip != NULL && StringICmp (oip->str, "Collaborator") == 0) {
      break;
    }
  }
  if (curr != NULL && curr->choice == 1) {
    str = (CharPtr) curr->data.ptrvalue;
    SetTitle (rdp->curator, str);
  }
  SetValue (rdp->status, status);
}

static Pointer RefGeneDialogToUserObjectPtr (DialoG d)

{
  Boolean               annotChange;
  Char                  ch;
  Char                  curator [256];
  Int2                  i;
  Int2                  j;
  size_t                len;
  Int4                  num [5];
  Boolean               okay;
  CharPtr               organism = NULL;
  RefgeneUserDialogPtr  rdp;
  Boolean               seqChange;
  Int2                  status;
  CharPtr               str;
  TagListPtr            tlp;
  CharPtr               txt [5];
  UserObjectPtr         uop;
  long int              val;
  ValNodePtr            vnp;

  rdp = (RefgeneUserDialogPtr) GetObjectExtra (d);
  if (rdp == NULL) return NULL;

  uop = CreateRefGeneTrackUserObject ();
  if (uop == NULL) return NULL;

  status = GetValue (rdp->status);
  if (status == 1) {
    AddStatusToRefGeneTrackUserObject (uop, "Predicted");
  } else if (status == 2) {
    AddStatusToRefGeneTrackUserObject (uop, "Provisional");
  } else if (status == 3) {
    AddStatusToRefGeneTrackUserObject (uop, "Validated");
  } else if (status == 4) {
    AddStatusToRefGeneTrackUserObject (uop, "Reviewed");
  }

  GetTitle (rdp->curator, curator, sizeof (curator));
  if (! StringHasNoText (curator)) {
    AddCuratorToRefGeneTrackUserObject (uop, curator);
  }

  tlp = (TagListPtr) GetObjectExtra (rdp->fields);
  if (tlp != NULL && tlp->vnp != NULL) {
    for (vnp = tlp->vnp; vnp != NULL; vnp = vnp->next) {
      str = (CharPtr) vnp->data.ptrvalue;
      okay = FALSE;
      len = StringLen (str);
      for (j = 0; j < len; j++) {
        ch = str [j];
        if (ch != ' ' && ch != '\t' && ch != '\n') {
          okay = TRUE;
        }
      }
      if (okay) {
        for (j = 0; j < 5; j++) {
          txt [j] = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, j);
          num [j] = 0;
        }
        for (j = 1; j < 5; j++) {
          if (j != 2) {
            num [j] = 0;
            if (txt [j] != NULL && sscanf (txt [j], "%ld", &val) == 1) {
              num [j] = val;
            }
          }
        }
        annotChange = FALSE;
        seqChange = FALSE;
        if (num [3] >= 2) {
          annotChange = TRUE;
          (num [3]) -= 2;
        }
        if (num [3] > 0) {
          seqChange = TRUE;
        }
        i = num [4];
        if (i >= REFGENE_ASSEMBLY && i <= REFGENE_UNKNOWN) {
          if (! StringHasNoText (txt [0])) {
            AddAccessionToRefGeneTrackUserObject (uop, refgene_labels [i],
                                                  txt [0], num [1],
                                                  seqChange, annotChange,
                                                  txt [2]);
          }
          for (j = 0; j < 5; j++) {
            txt [j] = MemFree (txt [j]);
          }
        }
      }
    }
  }

  return uop;
}

static DialoG CreateRefGeneDialog (GrouP g)

{
  Int2                  i;
  PrompT                lastppt;
  GrouP                 p;
  PrompT                ppt;
  GrouP                 q;
  RefgeneUserDialogPtr  rdp;
  TagListPtr            tlp;
  GrouP                 x;
  GrouP                 y;

  p = HiddenGroup (g, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  rdp = (RefgeneUserDialogPtr) MemNew (sizeof (RefgeneUserDialog));
  if (rdp == NULL) return NULL;

  SetObjectExtra (p, rdp, NULL);
  rdp->dialog = (DialoG) p;
  rdp->todialog = UserObjectPtrToRefGeneDialog;
  rdp->fromdialog = RefGeneDialogToUserObjectPtr;

  x = HiddenGroup (p, 4, 0, NULL);
  /* StaticPrompt (x, "Status", 0, stdLineHeight, programFont, 'l'); */
  rdp->status = HiddenGroup (x, 4, 0, NULL);
  SetObjectExtra (rdp->status, rdp, NULL);
  RadioButton (rdp->status, "Predicted");
  RadioButton (rdp->status, "Provisional");
  RadioButton (rdp->status, "Validated");
  RadioButton (rdp->status, "Reviewed");

  y = HiddenGroup (p, 2, 0, NULL);
  StaticPrompt (y, "Curator", 0, dialogTextHeight, programFont, 'l');
  rdp->curator = DialogText (y, "", 14, NULL);

  q = HiddenGroup (p, -6, 0, NULL);
  lastppt = NULL;
  ppt = NULL;
  for (i = 0; i < 5; i++) {
    lastppt = ppt;
    ppt = StaticPrompt (q, refgene_fields [i], refgene_widths [i] * stdCharWidth, 0, systemFont, 'c');
  }
  rdp->fields = CreateTagListDialog (p, 6, 5, STD_TAG_SPACING,
                                     refgene_types, refgene_widths, refgene_popups,
                                     AccessionUserFieldPtrToVisStringDialog,
                                     VisStringDialogToUserFieldPtr);

  tlp = (TagListPtr) GetObjectExtra (rdp->fields);
  if (tlp != NULL) {
    AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control [3], (HANDLE) lastppt, NULL);
    AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control [4], (HANDLE) ppt, NULL);
  }

  AlignObjects (ALIGN_CENTER, (HANDLE) x, (HANDLE) y, (HANDLE) q, (HANDLE) rdp->fields, NULL);
  return (DialoG) p;
}

static void RefgeneUserFormMessage (ForM f, Int2 mssg)

{
  RefgeneUserFormPtr  rfp;

  rfp = (RefgeneUserFormPtr) GetObjectExtra (f);
  if (rfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Remove (f);
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
        if (rfp->appmessage != NULL) {
          rfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static ForM CreateRefGeneDescForm (Int2 left, Int2 top, Int2 width,
                                   Int2 height, CharPtr title, ValNodePtr sdp,
                                   SeqEntryPtr sep, FormActnFunc actproc)

{
  ButtoN              b;
  GrouP               c;
  GrouP               g;
  RefgeneUserFormPtr  rfp;
  StdEditorProcsPtr   sepp;
  WindoW              w;

  w = NULL;
  rfp = (RefgeneUserFormPtr) MemNew (sizeof (RefgeneUserForm));
  if (rfp != NULL) {
    w = FixedWindow (left, top, width, height, title, StdCloseWindowProc);
    SetObjectExtra (w, rfp, StdDescFormCleanupProc);
    rfp->form = (ForM) w;
    rfp->actproc = actproc;
    rfp->formmessage = RefgeneUserFormMessage;

    rfp->sep = sep;

#ifndef WIN_MAC
    CreateStdEditorFormMenus (w);
#endif
    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      SetActivate (w, sepp->activateForm);
      rfp->appmessage = sepp->handleMessages;
    }

    g = HiddenGroup (w, -1, 0, NULL);
    rfp->data = CreateRefGeneDialog (g);

    c = HiddenGroup (w, 2, 0, NULL);
    b = PushButton (c, "Accept", StdAcceptFormButtonProc);
    SetObjectExtra (b, rfp, NULL);
    PushButton (c, "Cancel", StdCancelButtonProc);
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
    RealizeWindow (w);
  }
  return (ForM) w;
}

extern Int2 LIBCALLBACK RefGeneUserGenFunc (Pointer data)

{
  ObjectIdPtr         oip;
  OMProcControlPtr    ompcp;
  OMUserDataPtr       omudp;
  ObjMgrProcPtr       proc;
  RefgeneUserFormPtr  rfp;
  ValNodePtr          sdp;
  SeqEntryPtr         sep;
  UserObjectPtr       uop;
  WindoW              w;

  ompcp = (OMProcControlPtr) data;
  w = NULL;
  sdp = NULL;
  sep = NULL;
  uop = NULL;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  proc = ompcp->proc;
  switch (ompcp->input_itemtype) {
    case OBJ_SEQDESC :
      sdp = (ValNodePtr) ompcp->input_data;
      if (sdp != NULL && sdp->choice != Seq_descr_user) {
        return OM_MSG_RET_ERROR;
      }
      uop = (UserObjectPtr) sdp->data.ptrvalue;
      break;
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      break;
    default :
      return OM_MSG_RET_ERROR;
  }
  omudp = ItemAlreadyHasEditor (ompcp->input_entityID, ompcp->input_itemID,
                                ompcp->input_itemtype, ompcp->proc->procid);
  if (omudp != NULL) {
    if (StringCmp (proc->procname, "Edit RefGene UserTrack Desc") == 0) {
      rfp = (RefgeneUserFormPtr) omudp->userdata.ptrvalue;
      if (rfp != NULL) {
        Select (rfp->form);
      }
      return OM_MSG_RET_DONE;
    } else {
      return OM_MSG_RET_OK; /* not this type, check next registered user object editor */
    }
  }
  if (uop != NULL) {
    oip = uop->type;
    if (oip == NULL || oip->str == NULL) return OM_MSG_RET_OK;
    if (StringCmp (oip->str, "RefGeneTracking") != 0) return OM_MSG_RET_OK;
  }
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  w = (WindoW) CreateRefGeneDescForm (-50, -33, -10, -10,
                                      "Reference Gene Tracking", sdp, sep,
                                      StdDescFormActnProc);
  rfp = (RefgeneUserFormPtr) GetObjectExtra (w);
  if (rfp != NULL) {
    rfp->input_entityID = ompcp->input_entityID;
    rfp->input_itemID = ompcp->input_itemID;
    rfp->input_itemtype = ompcp->input_itemtype;
    rfp->this_itemtype = OBJ_SEQDESC;
    rfp->this_subtype = Seq_descr_user;
    rfp->procid = ompcp->proc->procid;
    rfp->proctype = ompcp->proc->proctype;
    rfp->userkey = OMGetNextUserKey ();
    omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid,
	                           OMPROC_EDIT, rfp->userkey);
    if (omudp != NULL) {
      omudp->userdata.ptrvalue = (Pointer) rfp;
      omudp->messagefunc = StdVibrantEditorMsgFunc;
    }
    SendMessageToForm (rfp->form, VIB_MSG_INIT);
    if (sdp != NULL) {
      PointerToDialog (rfp->data, (Pointer) sdp->data.ptrvalue);
      SetClosestParentIfDuplicating ((BaseFormPtr) rfp);
    }
  }
  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}

/*
static void TestGeneRefStuff (void)

{
  UserObjectPtr uop;
  ValNodePtr    sdp;

  uop = CreateRefGeneTrackUserObject ();
  AddAccessionToRefGeneTrackUserObject (uop, "Assembly", "U12345", 57, 29, 1995);
  AddAccessionToRefGeneTrackUserObject (uop, "Assembly", "L97531", 142, 66, 963);
  AddAccessionToRefGeneTrackUserObject (uop, "Assembly", "M66778", 823, 7677, 343);
  AddAccessionToRefGeneTrackUserObject (uop, "Related", "P34345", 445, 0, 0);
  AddAccessionToRefGeneTrackUserObject (uop, "Reject", "S19635", 1765, 0, 0);
  AddAccessionToRefGeneTrackUserObject (uop, "Related", "Q14884", 664, 35, 97);
  sdp = ValNodeNew (NULL);
  sdp->choice = Seq_descr_user;
  sdp->data.ptrvalue = (Pointer) uop;
  if (! ObjMgrRegister (OBJ_SEQDESC, (Pointer) sdp)) {
     ErrPostEx (SEV_ERROR, 0, 0, "ObjMgrRegister failed.");
  }
}
*/

typedef struct cka_acc {
   CharPtr      accession;
   SeqIdPtr     sip_whole;
   SeqAlignPtr  sap;
   Int4         start_acc;
   Int4         stop_acc;
   Int4         start_seq;
   Int4         stop_seq;
   Uint1        strand;
   Int4         num;
   struct cka_acc PNTR next;
} CKA_Acc, PNTR CKA_AccPtr;

static SeqAlignPtr CKA_MakeAlign(BioseqPtr bsp, CKA_AccPtr acc_head);

static void CKA_FindAllTpaDescr(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
   CKA_AccPtr         acc;
   CKA_AccPtr         PNTR acc_head;
   CKA_AccPtr         acc_prev;
   BioseqPtr          bsp;
   SeqMgrDescContext  context;
   UserFieldPtr       curr;
   ObjectIdPtr        oip;
   SeqDescrPtr        sdp;
   UserFieldPtr       ufp;
   UserObjectPtr      uop;

   acc_head = (CKA_AccPtr PNTR)data;
   acc_prev = *acc_head;
   while (acc_prev != NULL && acc_prev->next != NULL)
   {
      acc_prev = acc_prev->next;
   }
   sdp = NULL;
   if (IS_Bioseq(sep))
   {
      bsp = (BioseqPtr)sep->data.ptrvalue;
      if (ISA_na(bsp->mol))
      {
         while ((sdp = SeqMgrGetNextDescriptor(bsp, sdp, Seq_descr_user, &context)) != NULL)
         {
            uop = (UserObjectPtr)sdp->data.ptrvalue;
            if (!StringICmp(uop->type->str, "TpaAssembly"))
            {
               for (curr = uop->data; curr != NULL; curr = curr->next)
               {
                  if (curr->choice != 11) continue;
                  for (ufp = curr->data.ptrvalue; ufp != NULL; ufp = ufp->next)
                  {
                     if (ufp->choice != 1) continue;
                     oip = ufp->label;
                     if (oip == NULL || StringICmp (oip->str, "accession") != 0) continue;
                     acc = (CKA_AccPtr)MemNew(sizeof(CKA_Acc));
                     acc->accession = StringSave((CharPtr)ufp->data.ptrvalue);
                     acc->sip_whole = SeqIdDup(bsp->id);
                     if (acc_prev == NULL)
                        *acc_head = acc_prev = acc;
                     else
                     {
                        acc_prev->next = acc;
                        acc_prev = acc;
                     }
                  }
               }
            }
         }
      }
   }
}

static int LIBCALLBACK CKA_SortAccs(VoidPtr ptr1, VoidPtr ptr2)
{
   CKA_AccPtr  acc1;
   CKA_AccPtr  acc2;

   acc1 = *((CKA_AccPtr PNTR)ptr1);
   acc2 = *((CKA_AccPtr PNTR)ptr2);
   if (acc1->num < acc2->num)
      return -1;
   else if (acc1->num > acc2->num)
      return 1;
   else
      return 0; /* no alignment */
}

static Boolean CKA_ValidateSeqAlign(SeqAlignPtr sap, CKA_AccPtr acc_head, Int4 bioseqlen)
{
   CKA_AccPtr        acc;
   CKA_AccPtr        PNTR accarray;
   AMAlignIndex2Ptr  amaip;
   Int4              first;
   Boolean           found;
   Int4              i;
   Int4              j;
  Int4              last;
   Int4              prev;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return FALSE;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   for (i=0; i<amaip->numsaps; i++)
   {
      acc = acc_head;
      found = FALSE;
      while (acc != NULL && !found)
      {
         if (amaip->saps[i] == acc->sap)
            found = TRUE;
         else
            acc = acc->next;
      }
      if (!found) /* big error */
      {
         Message(MSG_ERROR, "Serious error. Please contact sequindev.");
         return FALSE;
      }
      acc->num = i+1;
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, &acc->start_seq, &acc->stop_seq);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, &acc->start_acc, &acc->stop_acc);
      acc->strand = AlnMgr2GetNthStrand(amaip->saps[i], 2);
   }
   acc = acc_head;
   i = 0;
   while (acc != NULL)
   {
      if (acc->num == 0)
         acc->num = amaip->numsaps; /* sort these guys all to the end */
      i++;
      acc = acc->next;
   }
   accarray = (CKA_AccPtr PNTR)MemNew(i*sizeof(CKA_AccPtr));
   i = 0;
   acc = acc_head;
   while (acc != NULL)
   {
      accarray[i] = acc;
      i++;
      acc = acc->next;
   }
   HeapSort(accarray, i, sizeof(CKA_AccPtr), CKA_SortAccs);
   first = last = -1;
   prev = -1;
   for (j=0; j<i && first <=0 ; j++)
   {
      acc = accarray[j];
      if (acc->sap != NULL)
      {
         if (first == -1)
            first = acc->start_seq;
         last = MAX(last, acc->stop_seq);
      } else
         Message (MSG_OKC, "Accession %s does not align to the bioseq.", acc->accession);
      if (prev != -1)
      {
         if (acc->start_seq > prev)
         {
            Message (MSG_ERROR, "The annotated accessions do not completely cover the bioseq:\n %s aligns to %d-%d but next aln is %s to %d-%d", accarray[j-1]->accession, accarray[j-1]->start_seq, accarray[j-1]->stop_seq, acc->accession, acc->start_seq, acc->stop_seq);
            MemFree(accarray);
            return FALSE;
         }
      }
      prev = acc->stop_seq+1;
   }
   if (first != 0 || last != bioseqlen-1)
   {
      if (first != 0)
         Message (MSG_ERROR, "The annotated accessions do not completely cover the bioseq:\n %s (the first aln) starts at position %d", accarray[0]->accession, accarray[0]->start_seq);
      if (accarray[i-1]->stop_seq != bioseqlen-1)
         Message (MSG_ERROR, "The annotated accessions do not completely cover the bioseq:\n %s (the last aln) goes to %d, bioseq length is %d", accarray[i-1]->accession, accarray[i-1]->stop_seq, bioseqlen-1);
      MemFree(accarray);
      return FALSE;
   }
   MemFree(accarray);
   return TRUE;
}

static void CKA_RunChecker(SeqEntryPtr sep)
{
   CKA_AccPtr   acc_head;
   CKA_AccPtr   acc_head_next;
   CKA_AccPtr   acc_head_prev;
   CKA_AccPtr   acc_head_tmp;
   BioseqPtr    bsp;
   Boolean      found;
   SeqIdPtr     lastid;
   SeqAlignPtr  sap;
   SeqHistPtr   shp;

   if (sep == NULL)
   {
      Message(MSG_ERROR, "Null SeqEntry passed to CKA_RunChecker");
      return;
   }
   acc_head = NULL;
   SeqMgrIndexFeatures(0, (Pointer)(sep));
   SeqEntryExplore(sep, &acc_head, CKA_FindAllTpaDescr);
   lastid = NULL;
   if (acc_head == NULL)
   {
      Message(MSG_ERROR, "No Tpa features found in SeqEntry.");
      return;
   }
   acc_head_tmp = acc_head;
   acc_head_prev = NULL;
   while (acc_head != NULL)
   {
      lastid = acc_head->sip_whole;
      acc_head_prev = acc_head;
      acc_head_tmp = acc_head->next;
      found = FALSE;
      while (!found && acc_head_tmp != NULL)
      {
         if (SeqIdComp(lastid, acc_head_tmp->sip_whole) != SIC_YES)
            found = TRUE;
         else
         {
            acc_head_prev = acc_head_tmp;
            acc_head_tmp = acc_head_tmp->next;
         }
      }
      acc_head_next = acc_head_prev->next;
      acc_head_prev->next = NULL;
      bsp = BioseqLockById(acc_head->sip_whole);
      if (ISA_na(bsp->mol))
      {
         sap = CKA_MakeAlign(bsp, acc_head);
         if (CKA_ValidateSeqAlign(sap, acc_head, bsp->length))
         {
         /* make seq-hist and add it to record */
            if (bsp->hist != NULL)
            {
               shp = bsp->hist;
               if (shp->assembly != NULL)
                  SeqAlignSetFree(shp->assembly);
               shp->assembly = sap;
            } else
            {
               shp = SeqHistNew();
               shp->assembly = sap;
               bsp->hist = shp;
            }
         } else
            SeqAlignSetFree(sap);
      } else
         Message(MSG_ERROR, "%s is annotated on a non-nucleotide bioseq.", acc_head->accession);
      while (acc_head != NULL)
      {
         acc_head_tmp = acc_head->next;
         acc_head->next = NULL;
         MemFree(acc_head->accession);
         SeqIdFree(acc_head->sip_whole);
         MemFree(acc_head);
         acc_head = acc_head_tmp;
      }
      acc_head = acc_head_next;
   }
}

static void CKA_FindNuc(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
   BioseqPtr      bsp;
   BioseqPtr      PNTR bspptr;

   bspptr = (BioseqPtr PNTR)data;
   if (IS_Bioseq(sep))
   {
      bsp = (BioseqPtr)sep->data.ptrvalue;
      if (ISA_na(bsp->mol))
      {
         *bspptr = bsp;
      }
   }
}

static int LIBCALLBACK CKA_CompareAlns(VoidPtr ptr1, VoidPtr ptr2)
{
   Int4         len1;
   Int4         len2;
   SeqAlignPtr  sap1;
   SeqAlignPtr  sap2;

   sap1 = *((SeqAlignPtr PNTR) ptr1);
   sap2 = *((SeqAlignPtr PNTR) ptr2);
   if (sap1 == NULL || sap2 == NULL)
      return 0;
   len1 = AlnMgr2GetAlnLength(sap1, FALSE);
   len2 = AlnMgr2GetAlnLength(sap2, FALSE);
   if (len1 < len2)
      return 1;
   if (len1 > len2)
      return -1;
   return 0;
}

static SeqAlignPtr CKA_MakeAlign(BioseqPtr bsp, CKA_AccPtr acc_head)
{
   CKA_AccPtr           acc;
   SeqAlignPtr          allsap;
   SeqAlignPtr          allsap_prev;
   AMAlignIndex2Ptr     amaip;
   BioseqPtr            bsp_tmp;
   Int4                 i;
   BLAST_OptionsBlkPtr  options;
   SeqAlignPtr          salp;
   SeqAlignPtr          sap_new;
   SeqIdPtr             sip;
   Uint1                strand;

   if (bsp == NULL || acc_head == NULL)
      return NULL;
   acc = acc_head;
   allsap = NULL;
   allsap_prev = NULL;
   while (acc != NULL)
   {
      options = BLASTOptionNew("blastn", TRUE);
      options->wordsize = 32;
      options->gap_open = 0;
      options->gap_extend = 0;
   /* faster with no filtering */
      options->is_megablast_search = TRUE;
      sip = SeqIdFromAccessionDotVersion(acc->accession);
      bsp_tmp = BioseqLockById(sip);
      if (!ISA_na(bsp_tmp->mol))
      {
         BioseqUnlock(bsp_tmp);
         Message(MSG_ERROR, "%s is not a nucleotide bioseq.", acc->accession);
         break;
      }
      acc->sap = BlastTwoSequences(bsp_tmp, bsp, "blastn", options);
      if (acc->sap != NULL)
         SPI_flip_sa_list(acc->sap);
      if (acc->sap != NULL && acc->sap->next != NULL)
      {
         AlnMgr2IndexLite(acc->sap);
         amaip = (AMAlignIndex2Ptr)(acc->sap->saip);
         HeapSort(amaip->saps, amaip->numsaps, sizeof(SeqAlignPtr), CKA_CompareAlns);
         salp = (amaip->saps[0]);
         sap_new = SeqAlignDup(salp);
         SeqAlignSetFree(acc->sap);
         acc->sap = sap_new;
      }
      if (acc->sap != NULL)
      {
         AlnMgr2IndexSingleChildSeqAlign(acc->sap);
         strand = AlnMgr2GetNthStrand(acc->sap, 1);
         if (strand == Seq_strand_minus)
         {
            SeqAlignListReverseStrand(acc->sap);
            SAIndex2Free2(acc->sap->saip);
            acc->sap->saip = NULL;
         }
         if (allsap != NULL && acc->sap != NULL)
         {
            allsap_prev->next = acc->sap;
            allsap_prev = allsap_prev->next;;
         } else if (acc->sap != NULL)
            allsap_prev = allsap = (acc->sap);
      }
      BioseqUnlock(bsp_tmp);
      acc = acc->next;
      BLASTOptionDelete(options);
   }
   if (allsap == NULL)
      return NULL;
   sap_new = SeqAlignNew();
   sap_new->segtype = SAS_DISC;
   sap_new->segs = (Pointer)(allsap);
   allsap = sap_new;
   AlnMgr2IndexLite(allsap);
   AlnMgr2SortAlnSetByNthRowPos(allsap, 1);
   amaip = (AMAlignIndex2Ptr)(allsap->saip);
   for (i=0; i<amaip->numsaps-1; i++)
   {
      amaip->saps[i]->next = amaip->saps[i+1];
   }
   amaip->saps[amaip->numsaps-1]->next = NULL;
   allsap->segs = (Pointer)(amaip->saps[0]);
   return allsap;
}

extern void CreateSeqHistTPA (IteM i);

extern void CreateSeqHistTPA (IteM i)
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

  CKA_RunChecker(sep);

  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

extern Int2 LIBCALLBACK AssemblyUserGenFunc (Pointer data);

typedef struct assemblyuserdialog {
  DIALOG_MESSAGE_BLOCK
  DialoG        accns;
} AssemblyUserDialog, PNTR AssemblyUserDialogPtr;

typedef struct assemblyuserform {
  FEATURE_FORM_BLOCK
  SeqEntryPtr   sep;
} AssemblyUserForm, PNTR AssemblyUserFormPtr;

static void UserObjectPtrToAssemblyDialog (DialoG d, Pointer data)

{
  AssemblyUserDialogPtr  adp;
  UserFieldPtr           curr;
  ValNodePtr             head = NULL;
  ObjectIdPtr            oip;
  CharPtr                str;
  UserFieldPtr           ufp;
  UserObjectPtr          uop;

  adp = (AssemblyUserDialogPtr) GetObjectExtra (d);
  if (adp == NULL) return;

  uop = (UserObjectPtr) data;
  if (uop == NULL || uop->type == NULL || StringICmp (uop->type->str, "TpaAssembly") != 0) {
    PointerToDialog (adp->accns, NULL);
    return;
  }

  for (curr = uop->data; curr != NULL; curr = curr->next) {
    if (curr->choice != 11) continue;
    for (ufp = curr->data.ptrvalue; ufp != NULL; ufp = ufp->next) {
      if (ufp->choice != 1) continue;
      oip = ufp->label;
      if (oip == NULL || StringICmp (oip->str, "accession") != 0) continue;
      str = (CharPtr) ufp->data.ptrvalue;
      if (StringHasNoText (str)) continue;
      ValNodeCopyStr (&head, 0, (Pointer) str);
    }
  }

  PointerToDialog (adp->accns, (Pointer) head);
  ValNodeFreeData (head);
}

static Pointer AssemblyDialogToUserObjectPtr (DialoG d)

{
  AssemblyUserDialogPtr  adp;
  Char                   ch;
  ValNodePtr             head;
  CharPtr                last;
  UserObjectPtr          uop;
  CharPtr                ptr;
  CharPtr                str;
  CharPtr                tmp;
  ValNodePtr             vnp;

  adp = (AssemblyUserDialogPtr) GetObjectExtra (d);
  if (adp == NULL) return NULL;

  uop = CreateTpaAssemblyUserObject ();
  if (uop == NULL) return NULL;

  head = (ValNodePtr) DialogToPointer (adp->accns);
  if (head == NULL) return NULL;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringHasNoText (str)) continue;
    tmp = StringSave (str);
    last = tmp;
    ptr = last;
    ch = *ptr;
    while (ch != '\0') {
      if (ch == ',') {
        *ptr = '\0';
        TrimSpacesAroundString (last);
        AddAccessionToTpaAssemblyUserObject (uop, last);
        ptr++;
        last = ptr;
        ch = *ptr;
      } else {
        ptr++;
        ch = *ptr;
      }
    }
    if (! StringHasNoText (last)) {
      TrimSpacesAroundString (last);
      AddAccessionToTpaAssemblyUserObject (uop, last);
    }
    /* AddAccessionToTpaAssemblyUserObject (uop, str); */
    MemFree (tmp);
  }

  ValNodeFreeData (head);

  return uop;
}

static DialoG CreateAssemblyDialog (GrouP g)

{
  AssemblyUserDialogPtr  adp;
  GrouP                  p;
  GrouP                  x;

  p = HiddenGroup (g, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  adp = (AssemblyUserDialogPtr) MemNew (sizeof (AssemblyUserDialog));
  if (adp == NULL) return NULL;

  SetObjectExtra (p, adp, NULL);
  adp->dialog = (DialoG) p;
  adp->todialog = UserObjectPtrToAssemblyDialog;
  adp->fromdialog = AssemblyDialogToUserObjectPtr;

  x = HiddenGroup (p, 0, 2, NULL);
  StaticPrompt (x, "Accessions", 0, 0, programFont, 'c');
  adp->accns = CreateVisibleStringDialog (x, 3, -1, 15);

  return (DialoG) p;
}

static void AssemblyUserFormMessage (ForM f, Int2 mssg)

{
  AssemblyUserFormPtr  afp;

  afp = (AssemblyUserFormPtr) GetObjectExtra (f);
  if (afp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Remove (f);
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
        if (afp->appmessage != NULL) {
          afp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static ForM CreateAssemblyDescForm (Int2 left, Int2 top, Int2 width,
                                   Int2 height, CharPtr title, ValNodePtr sdp,
                                   SeqEntryPtr sep, FormActnFunc actproc)

{
  AssemblyUserFormPtr  afp;
  ButtoN               b;
  GrouP                c;
  GrouP                g;
  StdEditorProcsPtr    sepp;
  WindoW               w;

  w = NULL;
  afp = (AssemblyUserFormPtr) MemNew (sizeof (AssemblyUserForm));
  if (afp != NULL) {
    w = FixedWindow (left, top, width, height, title, StdCloseWindowProc);
    SetObjectExtra (w, afp, StdDescFormCleanupProc);
    afp->form = (ForM) w;
    afp->actproc = actproc;
    afp->formmessage = AssemblyUserFormMessage;

    afp->sep = sep;

#ifndef WIN_MAC
    CreateStdEditorFormMenus (w);
#endif
    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      SetActivate (w, sepp->activateForm);
      afp->appmessage = sepp->handleMessages;
    }

    g = HiddenGroup (w, -1, 0, NULL);
    afp->data = CreateAssemblyDialog (g);

    c = HiddenGroup (w, 2, 0, NULL);
    b = PushButton (c, "Accept", StdAcceptFormButtonProc);
    SetObjectExtra (b, afp, NULL);
    PushButton (c, "Cancel", StdCancelButtonProc);
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
    RealizeWindow (w);
  }
  return (ForM) w;
}

extern Int2 LIBCALLBACK AssemblyUserGenFunc (Pointer data)

{
  AssemblyUserFormPtr  afp;
  ObjectIdPtr          oip;
  OMProcControlPtr     ompcp;
  OMUserDataPtr        omudp;
  ObjMgrProcPtr        proc;
  ValNodePtr           sdp;
  SeqEntryPtr          sep;
  UserObjectPtr        uop;
  WindoW               w;

  ompcp = (OMProcControlPtr) data;
  w = NULL;
  sdp = NULL;
  sep = NULL;
  uop = NULL;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  proc = ompcp->proc;
  switch (ompcp->input_itemtype) {
    case OBJ_SEQDESC :
      sdp = (ValNodePtr) ompcp->input_data;
      if (sdp != NULL && sdp->choice != Seq_descr_user) {
        return OM_MSG_RET_ERROR;
      }
      uop = (UserObjectPtr) sdp->data.ptrvalue;
      break;
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      break;
    default :
      return OM_MSG_RET_ERROR;
  }
  omudp = ItemAlreadyHasEditor (ompcp->input_entityID, ompcp->input_itemID,
                                ompcp->input_itemtype, ompcp->proc->procid);
  if (omudp != NULL) {
    if (StringCmp (proc->procname, "Edit Assembly User Desc") == 0) {
      afp = (AssemblyUserFormPtr) omudp->userdata.ptrvalue;
      if (afp != NULL) {
        Select (afp->form);
      }
      return OM_MSG_RET_DONE;
    } else {
      return OM_MSG_RET_OK; /* not this type, check next registered user object editor */
    }
  }
  if (uop != NULL) {
    oip = uop->type;
    if (oip == NULL || oip->str == NULL) return OM_MSG_RET_OK;
    if (StringCmp (oip->str, "TpaAssembly") != 0) return OM_MSG_RET_OK;
  }
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  w = (WindoW) CreateAssemblyDescForm (-50, -33, -10, -10,
                                       "Assembly Tracking", sdp, sep,
                                       StdDescFormActnProc);
  afp = (AssemblyUserFormPtr) GetObjectExtra (w);
  if (afp != NULL) {
    afp->input_entityID = ompcp->input_entityID;
    afp->input_itemID = ompcp->input_itemID;
    afp->input_itemtype = ompcp->input_itemtype;
    afp->this_itemtype = OBJ_SEQDESC;
    afp->this_subtype = Seq_descr_user;
    afp->procid = ompcp->proc->procid;
    afp->proctype = ompcp->proc->proctype;
    afp->userkey = OMGetNextUserKey ();
    omudp = ObjMgrAddUserData (ompcp->input_entityID, ompcp->proc->procid,
	                           OMPROC_EDIT, afp->userkey);
    if (omudp != NULL) {
      omudp->userdata.ptrvalue = (Pointer) afp;
      omudp->messagefunc = StdVibrantEditorMsgFunc;
    }
    SendMessageToForm (afp->form, VIB_MSG_INIT);
    if (sdp != NULL) {
      PointerToDialog (afp->data, (Pointer) sdp->data.ptrvalue);
      SetClosestParentIfDuplicating ((BaseFormPtr) afp);
    }
  }
  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}


typedef struct historyformdata {
  FEATURE_FORM_BLOCK

  BioseqPtr      bsp;
  DialoG         replace_date;
  DialoG         replace_ids;
  ButtoN         secondary_on_part;
  DialoG         replaced_by_date;
  DialoG         replaced_by_ids;
  ButtoN         deleted;
  DialoG         deleted_date;
} HistoryFormData, PNTR HistoryFormPtr;

static SeqIdPtr VisStrDialogToSeqIdSet (DialoG d)

{
  long          gi;
  SeqIdPtr      head = NULL;
  ValNodePtr    list;
  CharPtr       str;
  TextSeqIdPtr  tsip;
  ValNodePtr    vnp;

  if (d == NULL) return NULL;
  list = DialogToPointer (d);
  for (vnp = list; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (str != NULL) {
      if (sscanf (str, "%ld", &gi)) {
        /*
        ValNodeAddInt (&head, SEQID_GI, (Int4) gi);
        */
      } else {
        tsip = TextSeqIdNew ();
        if (tsip != NULL) {
          tsip->accession = StringSaveNoNull (str);
          ValNodeAddPointer (&head, SEQID_GENBANK, (Pointer) tsip);
        }
      }
    }
  }
  ValNodeFreeData (list);
  return head;
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

static ValNodePtr GetStringsForSeqIDs (SeqIdPtr sip)

{
  Char          buf [40];
  ValNodePtr    head = NULL;
  TextSeqIdPtr  tsip;

  if (sip == NULL) return NULL;
  while (sip != NULL) {
    buf [0] = '\0';
    switch (sip->choice) {
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_DDBJ :
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL && (! StringHasNoText (tsip->accession))) {
          StringNCpy_0 (buf, tsip->accession, sizeof (buf));
        }
        break;
      case SEQID_GI :
        /*
        gi = sip->data.intvalue;
        if (gi > 0) {
          sprintf (buf, "%ld", (long) gi);
        }
        */
        break;
      default :
        break;
    }
    if (! StringHasNoText (buf)) {
      ValNodeCopyStr (&head, 0, buf);
    }
    sip = sip->next;
  }
  return head;
}

static void AddGenBankBlockToBioseq (BioseqPtr bsp, ValNodePtr head1, ValNodePtr head2)

{
  GBBlockPtr       gbp = NULL;
  CharPtr          last = NULL;
  ValNodePtr       next;
  ValNodePtr PNTR  prev;
  ValNodePtr       sdp;
  SeqEntryPtr      sep;
  CharPtr          str1, str2;
  ValNodePtr       vnp, vnp1, vnp2;

  sdp = BioseqGetSeqDescr (bsp, Seq_descr_genbank, NULL);
  if (sdp != NULL) {
    gbp = (GBBlockPtr) sdp->data.ptrvalue;
    if (gbp != NULL) {
      for (vnp1 = head1; vnp1 != NULL; vnp1 = vnp1->next) {
        str1 = (CharPtr) vnp1->data.ptrvalue;
        if (str1 != NULL) {
          for (vnp2 = gbp->extra_accessions; vnp2 != NULL; vnp2 = vnp2->next) {
            str2 = (CharPtr) vnp2->data.ptrvalue;
            if (StringICmp (str1, str2) == 0) {
              vnp2->data.ptrvalue = MemFree (vnp2->data.ptrvalue);
            }
          }
        }
      }
    }
  }
  if (sdp == NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
    sdp = CreateNewDescriptor (sep, Seq_descr_genbank);
    if (sdp != NULL) {
      sdp->data.ptrvalue = GBBlockNew ();
    }
  }
  if (sdp != NULL) {
    gbp = (GBBlockPtr) sdp->data.ptrvalue;
    if (gbp != NULL) {
      while (head2 != NULL) {
        ValNodeCopyStr (&(gbp->extra_accessions), 0, (CharPtr) head2->data.ptrvalue);
        head2 = head2->next;
      }
      /*
      ValNodeLink (&(gbp->extra_accessions), head2);
      head2 = NULL;
      */
      gbp->extra_accessions = SortValNode (gbp->extra_accessions, SortByName);
      prev = &(gbp->extra_accessions);
      vnp = gbp->extra_accessions;
      last = NULL;
      while (vnp != NULL) {
        next = vnp->next;
        str2 = (CharPtr) vnp->data.ptrvalue;
        if (str2 == NULL || StringHasNoText (str2) || StringICmp (last, str2) == 0) {
          *prev = next;
          vnp->next = NULL;
          MemFree (vnp);
          vnp = next;
        } else {
          last = str2;
          prev = &(vnp->next);
          vnp = next;
        }
      }
    }
  }

}

static void DoChangeHistory (ButtoN b)

{
  MsgAnswer       ans;
  BioseqPtr       bsp;
  ValNodePtr      head1 = NULL, head2 = NULL;
  HistoryFormPtr  hfp;
  SeqHistPtr      hist;
  BioseqPtr       pbsp;
  SeqEntryPtr     sep;
  SeqLocPtr       slp;
  CharPtr         str1, str2;
  ValNodePtr      vnp1, vnp2;

  hfp = (HistoryFormPtr) GetObjectExtra (b);
  if (hfp == NULL) return;
  ans = Message (MSG_OKC, "Are you sure you want to edit the history?");
  if (ans == ANS_CANCEL) {
    return;
  }
  Hide (hfp->form);
  Update ();
  bsp = hfp->bsp;
  hist = bsp->hist;
  if (hist == NULL) {
    hist = SeqHistNew ();
    bsp->hist = hist;
  }
  if (hist != NULL) {

    hist->replace_date = DateFree (hist->replace_date);
    hist->replace_date = DialogToPointer (hfp->replace_date);
    head1 = GetStringsForSeqIDs (hist->replace_ids);
    hist->replace_ids = SeqIdSetFree (hist->replace_ids);
    hist->replace_ids = VisStrDialogToSeqIdSet (hfp->replace_ids);
    head2 = GetStringsForSeqIDs (hist->replace_ids);

    hist->replaced_by_date = DateFree (hist->replaced_by_date);
    hist->replaced_by_date = DialogToPointer (hfp->replaced_by_date);
    hist->replaced_by_ids = SeqIdSetFree (hist->replaced_by_ids);
    hist->replaced_by_ids = VisStrDialogToSeqIdSet (hfp->replaced_by_ids);

    hist->deleted = GetStatus (hfp->deleted);
    hist->deleted_date = DateFree (hist->deleted_date);
    hist->deleted_date = DialogToPointer (hfp->deleted_date);
  }

  if (hist->assembly == NULL &&
      hist->replace_date == NULL && hist->replace_ids == NULL &&
      hist->replaced_by_date == NULL && hist->replaced_by_ids == NULL &&
      (! hist->deleted) && hist->deleted_date == NULL) {
    bsp->hist = SeqHistFree (bsp->hist);
  }

  head1 = SortValNode (head1, SortByName);
  head2 = SortValNode (head2, SortByName);
  for (vnp1 = head1; vnp1 != NULL; vnp1 = vnp1->next) {
    str1 = (CharPtr) vnp1->data.ptrvalue;
    for (vnp2 = head2; vnp2 != NULL; vnp2 = vnp2->next) {
      str2 = (CharPtr) vnp2->data.ptrvalue;
      if (StringICmp (str1, str2) == 0) {
        vnp1->data.ptrvalue = MemFree (vnp1->data.ptrvalue);
      }
    }
  }

  AddGenBankBlockToBioseq (bsp, head1, head2);

  if (GetStatus (hfp->secondary_on_part)) {
    if (bsp->repr == Seq_repr_seg) {
      for (slp = (SeqLocPtr) bsp->seq_ext; slp != NULL; slp = slp->next) {
        pbsp = BioseqFind (SeqLocId (slp));
        if (pbsp != NULL) {
          AddGenBankBlockToBioseq (pbsp, head1, head2);
        }
      }
    }
  }

  ValNodeFreeData (head1);
  ValNodeFreeData (head2);

  sep = GetTopSeqEntryForEntityID (hfp->input_entityID);
  EntryCheckGBBlock (sep);

  Update ();
  ObjMgrSetDirtyFlag (hfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, hfp->input_entityID, 0, 0);
  ObjMgrDeSelect (0, 0, 0, 0, NULL);
  Remove (hfp->form);
}

static void SeqIdSetToVisStrDialog (DialoG d, SeqIdPtr sip)

{
  ValNodePtr  head = NULL;

  if (d == NULL || sip == NULL) return;
  head = GetStringsForSeqIDs (sip);
  PointerToDialog (d, head);
  ValNodeFreeData (head);
}

static void HistoryMessageProc (ForM f, Int2 mssg)

{
  HistoryFormPtr  hfp;

  hfp = (HistoryFormPtr) GetObjectExtra (f);
  if (hfp != NULL) {
    if (hfp->appmessage != NULL) {
      hfp->appmessage (f, mssg);
    }
  }
}

static void CleanupHistoryPage (GraphiC g, VoidPtr data)

{
  HistoryFormPtr  hfp;

  hfp = (HistoryFormPtr) data;
  if (hfp != NULL) {
  }
  StdCleanupFormProc (g, data);
}

extern void EditSequenceHistory (IteM i)

{
  ButtoN             b;
  BaseFormPtr        bfp;
  BioseqPtr          bsp;
  GrouP              c;
  GrouP              g;
  GrouP              h;
  HistoryFormPtr     hfp;
  SeqHistPtr         hist;
  GrouP              j;
  GrouP              k;
  PrompT             ppt1, ppt2, ppt3;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  WindoW             w;

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
  hist = bsp->hist;

  hfp = (HistoryFormPtr) MemNew (sizeof (HistoryFormData));
  if (hfp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Sequence History", StdCloseWindowProc);
  SetObjectExtra (w, hfp, CleanupHistoryPage);
  hfp->form = (ForM) w;
  hfp->formmessage = HistoryMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    hfp->appmessage = sepp->handleMessages;
  }

  hfp->input_entityID = bfp->input_entityID;
  hfp->input_itemID = bfp->input_itemID;
  hfp->input_itemtype = bfp->input_itemtype;

  hfp->bsp = bsp;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  ppt1 = StaticPrompt (h, "Replaces", 0, 0, systemFont, 'c');

  g = HiddenGroup (h, -1, 0, NULL);
  hfp->replace_ids = CreateVisibleStringDialog (g, 4, -1, 10);
  hfp->secondary_on_part = CheckBox (g, "Secondary on Parts", NULL);
  hfp->replace_date = CreateDateDialog (g, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) hfp->replace_ids, (HANDLE) hfp->secondary_on_part, (HANDLE) hfp->replace_date, NULL);

  ppt2 = StaticPrompt (h, "Replaced By", 0, 0, systemFont, 'c');

  j = HiddenGroup (h, -1, 0, NULL);
  hfp->replaced_by_ids = CreateVisibleStringDialog (j, 4, -1, 10);
  hfp->replaced_by_date = CreateDateDialog (j, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) hfp->replaced_by_ids, (HANDLE) hfp->replaced_by_date, NULL);

  ppt3 = StaticPrompt (h, "Status", 0, 0, systemFont, 'c');

  k = HiddenGroup (h, -1, 0, NULL);
  hfp->deleted = CheckBox (k, "Deleted", NULL);
  hfp->deleted_date = CreateDateDialog (k, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) hfp->deleted, (HANDLE) hfp->deleted_date, NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  b = PushButton (c, "Accept", DoChangeHistory);
  SetObjectExtra (b, hfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) j, (HANDLE) k, (HANDLE) c,
                (HANDLE) ppt1, (HANDLE) ppt2, (HANDLE) ppt3, NULL);
  RealizeWindow (w);

  if (bsp->repr != Seq_repr_seg) {
    Disable (hfp->secondary_on_part);
  } else {
    SetStatus (hfp->secondary_on_part, TRUE);
  }

  if (hist != NULL) {
    PointerToDialog (hfp->replace_date, hist->replace_date);
    SeqIdSetToVisStrDialog (hfp->replace_ids, hist->replace_ids);
    PointerToDialog (hfp->replaced_by_date, hist->replaced_by_date);
    SeqIdSetToVisStrDialog (hfp->replaced_by_ids, hist->replaced_by_ids);
    PointerToDialog (hfp->deleted_date, hist->deleted_date);
    SetStatus (hfp->deleted, hist->deleted);
  }

  Show (w);
  Update ();
}

/* automatic defline generator */

typedef struct deffeats {
  SeqFeatPtr  sfp;
  SeqFeatPtr  gene;
  SeqFeatPtr  prot;
  CharPtr     genename;
  CharPtr     allelename;
  CharPtr     protname;
  Boolean     alreadyTrimmed;
  Uint2       entityID;
  Uint2       itemID;
  Uint2       subtype;
  Boolean     lastInString;
  Boolean     lastInGroup;
  Boolean     lastInType;
  Boolean     lastInPenultimate;
  Boolean     pseudo;
  Boolean     ignore;
  Boolean     suppressprefix;
  Int2        altSplices;
  Int2        numUnknown;
} DefFeatsData, PNTR DefFeatsPtr;

static Boolean GetMolBioFeatsGatherFunc (GatherContextPtr gcp, Boolean getGene, Boolean getMrna)

{
  DefFeatsPtr  dfp;
  RnaRefPtr    rrp;
  SeqFeatPtr   sfp;
  CharPtr      str;
  Uint1        type;
  ValNodePtr   PNTR vnpp;

  if (gcp == NULL || gcp->thisitem == NULL || gcp->userdata == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  vnpp = (ValNodePtr PNTR) gcp->userdata;
  sfp = (SeqFeatPtr) gcp->thisitem;
  switch (sfp->data.choice) {
    case SEQFEAT_GENE :
      if (getGene) {
        dfp = MemNew (sizeof (DefFeatsData));
        if (dfp == NULL) return TRUE;
        dfp->entityID = gcp->entityID;
        dfp->itemID = gcp->itemID;
        dfp->sfp = sfp;
        dfp->subtype = FEATDEF_GENE;
        ValNodeAddPointer (vnpp, 0, (Pointer) dfp);
      }
      break;
    case SEQFEAT_CDREGION :
      dfp = MemNew (sizeof (DefFeatsData));
      if (dfp == NULL) return TRUE;
      dfp->entityID = gcp->entityID;
      dfp->itemID = gcp->itemID;
      dfp->sfp = sfp;
      dfp->subtype = FEATDEF_CDS;
      dfp->altSplices = 1;
      ValNodeAddPointer (vnpp, 0, (Pointer) dfp);
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp == NULL) return TRUE;
      switch (rrp->type) {
        case 2 :
          if (getMrna) {
            dfp = MemNew (sizeof (DefFeatsData));
            if (dfp == NULL) return TRUE;
            dfp->entityID = gcp->entityID;
            dfp->itemID = gcp->itemID;
            dfp->sfp = sfp;
            dfp->subtype = FEATDEF_mRNA;
            ValNodeAddPointer (vnpp, 0, (Pointer) dfp);
          }
          break;
        case 3 :
          dfp = MemNew (sizeof (DefFeatsData));
          if (dfp == NULL) return TRUE;
          dfp->entityID = gcp->entityID;
          dfp->itemID = gcp->itemID;
          dfp->sfp = sfp;
          dfp->subtype = FEATDEF_tRNA;
          ValNodeAddPointer (vnpp, 0, (Pointer) dfp);
          break;
        case 4 :
          dfp = MemNew (sizeof (DefFeatsData));
          if (dfp == NULL) return TRUE;
          dfp->entityID = gcp->entityID;
          dfp->itemID = gcp->itemID;
          dfp->sfp = sfp;
          dfp->subtype = FEATDEF_rRNA;
          ValNodeAddPointer (vnpp, 0, (Pointer) dfp);
          break;
        case 255 :
          if (rrp->ext.choice == 1) {
            str = (CharPtr) rrp->ext.value.ptrvalue;
            if (StringICmp (str, "internal transcribed spacer 1") == 0 ||
                StringICmp (str, "internal transcribed spacer 2") == 0 ||
                StringICmp (str, "internal transcribed spacer 3") == 0 ||
                StringICmp (str, "internal transcribed spacer ITS1") == 0 ||
                StringICmp (str, "internal transcribed spacer ITS2") == 0 ||
                StringICmp (str, "internal transcribed spacer ITS3") == 0 ||
                StringICmp (str, "ITS1") == 0 ||
                StringICmp (str, "ITS2") == 0 ||
                StringICmp (str, "ITS3") == 0) {
              dfp = MemNew (sizeof (DefFeatsData));
              if (dfp == NULL) return TRUE;
              dfp->entityID = gcp->entityID;
              dfp->itemID = gcp->itemID;
              dfp->sfp = sfp;
              dfp->subtype = FEATDEF_otherRNA;
              ValNodeAddPointer (vnpp, 0, (Pointer) dfp);
            }
          }
          break;
        default :
          break;
      }
      break;
    case SEQFEAT_IMP :
      type = FindFeatDefType (sfp);
      if (type == FEATDEF_LTR || type == FEATDEF_exon) {
        dfp = MemNew (sizeof (DefFeatsData));
        if (dfp == NULL) return TRUE;
        dfp->entityID = gcp->entityID;
        dfp->itemID = gcp->itemID;
        dfp->sfp = sfp;
        dfp->subtype = type;
        ValNodeAddPointer (vnpp, 0, (Pointer) dfp);
      }
      break;
    default :
      break;
  }
  return TRUE;
}

static Boolean GetCDStRNArRNAGatherFunc (GatherContextPtr gcp)

{
  return GetMolBioFeatsGatherFunc (gcp, FALSE, FALSE);
}

static Boolean GetGeneCDStRNArRNAGatherFunc (GatherContextPtr gcp)

{
  return GetMolBioFeatsGatherFunc (gcp, TRUE, FALSE);
}

static Boolean GetGeneCDStRNArRNAmRNAGatherFunc (GatherContextPtr gcp)

{
  return GetMolBioFeatsGatherFunc (gcp, TRUE, TRUE);
}

static void LabelAModifier (CharPtr str, CharPtr text, Boolean labelMods)

{
  Char     ch;
  CharPtr  ptr;

  if (str == NULL || text == NULL) return;
  if (StringHasNoText (text)) {
    str [0] = '\0';
    text [0] = '\0';
  } else {
    if (labelMods) {
      ptr = str;
      while (*ptr != '\0') {
        ch = *ptr;
        *ptr = TO_LOWER (ch);
        ptr++;
      }
      StringCat (str, " ");
    } else {
      str [0] = '\0';
    }
    StringCat (str, text);
  }
}

static int LIBCALLBACK SortCDStRNArRNAByLocation (VoidPtr ptr1, VoidPtr ptr2)

{
  BioseqPtr    bsp1;
  BioseqPtr    bsp2;
  DefFeatsPtr  dfp1;
  DefFeatsPtr  dfp2;
  Int4         leftend1;
  Int4         leftend2;
  Int4         rightend1;
  Int4         rightend2;
  SeqFeatPtr   sfp1;
  SeqFeatPtr   sfp2;
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      dfp1 = (DefFeatsPtr) vnp1->data.ptrvalue;
      dfp2 = (DefFeatsPtr) vnp2->data.ptrvalue;
      if (dfp1 != NULL && dfp2 != NULL) {
        sfp1 = dfp1->sfp;
        sfp2 = dfp2->sfp;
        if (sfp1 != NULL && sfp2 != NULL) {
          bsp1 = GetBioseqGivenSeqLoc (sfp1->location, dfp1->entityID);
          bsp2 = GetBioseqGivenSeqLoc (sfp2->location, dfp2->entityID);
          if (bsp1 != NULL && bsp2 != NULL) {
            leftend1 = GetOffsetInBioseq (sfp1->location, bsp1, SEQLOC_LEFT_END);
            leftend2 = GetOffsetInBioseq (sfp2->location, bsp2, SEQLOC_LEFT_END);
            rightend1 = GetOffsetInBioseq (sfp1->location, bsp1, SEQLOC_RIGHT_END);
            rightend2 = GetOffsetInBioseq (sfp2->location, bsp2, SEQLOC_RIGHT_END);
            if (leftend1 > leftend2) {
              return 1;
            } else if (leftend1 < leftend2) {
              return -1;
            } else if (sfp2->data.choice == SEQFEAT_GENE) {
              return 1;
            } else if (sfp1->data.choice == SEQFEAT_GENE) {
              return -1;
            } else if (rightend1 > rightend2) {
              return 1;
            } else if (rightend1 < rightend2) {
              return -1;
            } else {
              return 0;
            }
          }
        }
      }
    }
  }
  return 0;
}

static int LIBCALLBACK SortCDSAfterExons (VoidPtr ptr1, VoidPtr ptr2)

{
  DefFeatsPtr  dfp1;
  DefFeatsPtr  dfp2;
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;

  if (ptr1 != NULL && ptr2 != NULL) {
    vnp1 = *((ValNodePtr PNTR) ptr1);
    vnp2 = *((ValNodePtr PNTR) ptr2);
    if (vnp1 != NULL && vnp2 != NULL) {
      dfp1 = (DefFeatsPtr) vnp1->data.ptrvalue;
      dfp2 = (DefFeatsPtr) vnp2->data.ptrvalue;
      if (dfp1 != NULL && dfp2 != NULL) {
        if (dfp1->subtype == FEATDEF_CDS && dfp2->subtype == FEATDEF_exon) {
          return 1;
        } else if (dfp1->subtype == FEATDEF_exon && dfp2->subtype == FEATDEF_CDS) {
          return -1;
        } else {
          /* return 0; */
          return SortCDStRNArRNAByLocation (ptr1, ptr2);
        }
      }
    }
  }
  return 0;
}

extern EnumFieldAssoc  orgmod_subtype_alist [];
extern EnumFieldAssoc  subsource_subtype_alist [];

static Int2  orgmod_rank [32];
static Int2  subsource_rank [26];

static Boolean StrainAlreadyInParentheses (CharPtr taxname, CharPtr strain)

{
  size_t   len;
  CharPtr  ptr;

  ptr = StringChr (taxname, '(');
  if (ptr == NULL) return FALSE;
  ptr++;
  len = StringLen (strain);
  if (StringNCmp (taxname, strain, len) != 0) return FALSE;
  ptr += len;
  if (*ptr != ')') return FALSE;
  return TRUE;
}

static void AddOrgModsToDef (ValNodePtr PNTR stringsPtr, BioSourcePtr biop, Boolean labelMods)

{
  EnumFieldAssocPtr  ap;
  OrgModPtr          mod;
  OrgNamePtr         onp;
  OrgRefPtr          orp;
  CharPtr            ptr;
  SubSourcePtr       ssp;
  Char               str [128];
  Char               text [64];

  if (stringsPtr != NULL && biop != NULL) {
    orp = biop->org;
    if (orp != NULL) {
      onp = orp->orgname;
      if (onp != NULL) {
        mod = onp->mod;
        while (mod != NULL) {
          if (mod->subtype < 32 && orgmod_rank [mod->subtype] > 0) {
            if (mod->subtype == 2 && StrainAlreadyInParentheses (orp->taxname, mod->subname)) {
              /* do not add strain if already parenthetical in organism name */
            } else {
              text [0] = '\0';
              str [0] = '\0';
              StringNCpy_0 (text, mod->subname, sizeof (text));
              for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
                if (ap->value == mod->subtype) {
                  StringNCpy_0 (str, ap->name, sizeof (str));
                }
              }
              LabelAModifier (str, text, labelMods);
              if (! StringHasNoText (str)) {
                ValNodeCopyStr (stringsPtr, orgmod_rank [mod->subtype], str);
              }
            }
          }
          mod = mod->next;
        }
      }
    }

    ssp = biop->subtype;
    while (ssp != NULL) {
      if (ssp->subtype < 26 && subsource_rank [ssp->subtype] > 0) {
        text [0] = '\0';
        str [0] = '\0';
        StringNCpy_0 (text, ssp->name, sizeof (text));
        for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
          if (ap->value == ssp->subtype) {
            StringNCpy_0 (str, ap->name, sizeof (str));
            ptr = StringStr (str, "-name");
            if (ptr != NULL) {
              *ptr = '\0';
            }
            if (ssp->subtype == 23) { /* country */
              ptr = StringStr (text, ":");
              if (ptr != NULL) {
                *ptr = '\0';
              }
            }
          }
        }
        if (ssp->subtype == 23 /* && (! labelMods) */) {
          StringCpy (str, "from");
        }
        LabelAModifier (str, text, labelMods || (ssp->subtype == 23));
        if (! StringHasNoText (str)) {
          ValNodeCopyStr (stringsPtr, subsource_rank [ssp->subtype], str);
        }
      }
      ssp = ssp->next;
    }
  }
}

static CharPtr GetExonNumber (GBQualPtr gbq)

{
  while (gbq != NULL) {
    if (StringICmp (gbq->qual, "number") == 0) {
      return gbq->val;
    }
    gbq = gbq->next;
  }

  return NULL;
}

static Boolean NextIsExon (ValNodePtr vnp)

{
  DefFeatsPtr  nextdfp;
  ValNodePtr   nextvnp;

  if (vnp == NULL) return FALSE;
  nextvnp = vnp->next;
  if (nextvnp == NULL) return FALSE;
  nextdfp = (DefFeatsPtr) nextvnp->data.ptrvalue;
  if (nextdfp != NULL && nextdfp->subtype == FEATDEF_exon) return TRUE;
  return FALSE;
}

static CharPtr organelleByGenome [] = {
  NULL,
  NULL,
  "chloroplast",
  "chromoplast",
  "kinetoplast",
  "mitochondrial",
  "plastid",
  "",
  "",
  "", 
  "",
  "",
  "cyanelle",
  "",
  "",
  "",
  "apicoplast",
  "leucoplast",
  "proplastid",
  NULL
};

static CharPtr organelleByPopup [] = {
  NULL,
  "mitochondrial",
  "chloroplast",
  "kinetoplast",
  "plastid",
  "chromoplast",
  "cyanelle",
  "apicoplast",
  "leucoplast",
  "proplastid",
  NULL
};

static void FinishAutoDefProc (Uint2 entityID, SeqEntryPtr sep,
                               ValNodePtr head, BioseqPtr target,
                               SeqEntryPtr nsep, MolInfoPtr mip,
                               ValNodePtr strings, BioSourcePtr biop,
                               Int2 mitochloroflag)

{
  Int2          count;
  Boolean       ddbjstyle = FALSE;
  DefFeatsPtr   dfp;
  CharPtr       exonnumber;
  Int2          mitocount;
  DefFeatsPtr   nextdfp;
  CharPtr       ptr;
  RnaRefPtr     rrp;
  SeqFeatPtr    sfp;
  Char          str [380];
  tRNAPtr       trna;
  ValNodePtr    ttl;
  Char          text [64];
  Char          orgnelle [80];
  ValNodePtr    vnp;

  if (GetAppParam ("SEQUIN", "PREFERENCES", "DATABASE", NULL, str, sizeof (str))) {
    if (StringICmp (str, "DDBJ") == 0) {
      ddbjstyle = TRUE;
    }
  }
  vnp = head;
  count = 0;
  mitocount = 0;
  while (vnp != NULL) {
    str [0] = '\0';
    dfp = (DefFeatsPtr) vnp->data.ptrvalue;
    if (dfp != NULL) {
      sfp = dfp->sfp;
      if (sfp != NULL || dfp->numUnknown > 0) {
        count++;
        mitocount++;
        /* FindGeneAndProtForCDS (entityID, sfp, &(dfp->gene), &(dfp->prot)); */
        /* StringCpy (str, "unknown"); */
        text [0] = '\0';
        if (dfp->subtype == FEATDEF_CDS && dfp->prot != NULL) {
          if (dfp->suppressprefix) {
            str [0] = '\0';
            if (dfp->sfp->partial) {
              StringCat (str, "partial cds");
            } else {
              StringCat (str, "complete cds");
            }
            if (dfp->altSplices > 1) {
              StringCat (str, ", alternatively spliced");
            }
          } else if (dfp->protname != NULL) {
            if (StringICmp (dfp->protname, "unknown") == 0 && (! StringHasNoText (dfp->genename))) {
              StringNCpy_0 (text, dfp->genename, sizeof (text));
              /* StringCat (str, "("); */
              StringCat (str, text);
              /* StringCat (str, ")"); */
            } else {
              StringNCpy_0 (str, dfp->protname, sizeof (str) - 100);
              if (dfp->genename != NULL) {
                StringNCpy_0 (text, dfp->genename, sizeof (text));
                if (! StringHasNoText (text)) {
                  StringCat (str, " (");
                  StringCat (str, text);
                  StringCat (str, ")");
                }
              }
            }
            if (dfp->lastInGroup || dfp->lastInType) {
              if (mip != NULL) {
                ptr = StringISearch (str, "precursor");
                if (ptr != NULL && StringICmp (ptr, "precursor") == 0) {
                  StringCat (str, ",");
                }
                if (mip->biomol == MOLECULE_TYPE_MRNA) {
                  StringCat (str, " mRNA");
                } else if (mip->biomol == MOLECULE_TYPE_PRE_MRNA) {
                  StringCat (str, " precursor RNA");
                } else {
                  StringCat (str, " gene");
                }
              }
              if (count > 1) {
                StringCat (str, "s");
              }
              if (count < 2 && /* dfp->altSplices < 2 && */ (! StringHasNoText (dfp->allelename))) {
                StringNCpy_0 (text, dfp->allelename, sizeof (text));
                StringCat (str, ", ");
                StringCat (str, text);
                StringCat (str, " allele");
              }
              if (dfp->sfp->partial) {
                StringCat (str, ", partial cds");
              } else {
                StringCat (str, ", complete cds");
              }
              if (dfp->altSplices > 1) {
                StringCat (str, ", alternatively spliced");
              }
            }
          }
        } else if (dfp->subtype == FEATDEF_CDS && dfp->pseudo) {
          if (dfp->genename != NULL) {
            StringNCpy_0 (str, dfp->genename, sizeof (str) - 50);
          }
          if (dfp->lastInGroup || dfp->lastInType) {
            StringCat (str, " pseudogene");
            if (count > 1) {
              StringCat (str, "s");
            }
            if (count < 2 && (! StringHasNoText (dfp->allelename))) {
              StringNCpy_0 (text, dfp->allelename, sizeof (text));
              StringCat (str, ", ");
              StringCat (str, text);
              StringCat (str, " allele");
            }
            if (dfp->sfp->partial) {
              StringCat (str, ", partial sequence");
            } else {
              StringCat (str, ", complete sequence");
            }
          }
        } else if (dfp->subtype == FEATDEF_CDS && dfp->prot == NULL && dfp->genename != NULL) {
          StringNCpy_0 (str, dfp->genename, sizeof (str) - 50);
          if (dfp->lastInGroup || dfp->lastInType) {
            if (mip != NULL) {
              ptr = StringISearch (str, "precursor");
              if (ptr != NULL && StringICmp (ptr, "precursor") == 0) {
                StringCat (str, ",");
              }
              if (mip->biomol == MOLECULE_TYPE_MRNA) {
                StringCat (str, " mRNA");
              } else if (mip->biomol == MOLECULE_TYPE_PRE_MRNA) {
                StringCat (str, " precursor RNA");
              } else {
                StringCat (str, " gene");
              }
            }
            if (count > 1) {
              StringCat (str, "s");
            }
            if (count < 2 && (! StringHasNoText (dfp->allelename))) {
              StringNCpy_0 (text, dfp->allelename, sizeof (text));
              StringCat (str, ", ");
              StringCat (str, text);
              StringCat (str, " allele");
            }
            if (dfp->sfp->partial) {
              StringCat (str, ", partial cds");
            } else {
              StringCat (str, ", complete cds");
            }
          }
        } else if (dfp->subtype == FEATDEF_GENE) {
          if (dfp->genename != NULL) {
            StringNCpy_0 (str, dfp->genename, sizeof (str) - 50);
          }
          if (dfp->lastInGroup || dfp->lastInType) {
            if (mip != NULL) {
              if (mip->biomol == MOLECULE_TYPE_MRNA) {
                StringCat (str, " mRNA");
              } else if (mip->biomol == MOLECULE_TYPE_PRE_MRNA) {
                StringCat (str, " precursor RNA");
              } else {
                StringCat (str, " gene");
              }
            } else {
              StringCat (str, " gene");
            }
            if (count > 1) {
              StringCat (str, "s");
            }
            if (count < 2 && (! StringHasNoText (dfp->allelename))) {
              StringNCpy_0 (text, dfp->allelename, sizeof (text));
              StringCat (str, ", ");
              StringCat (str, text);
              StringCat (str, " allele");
            }
            if (dfp->sfp->partial) {
              StringCat (str, ", partial sequence");
            } else {
              StringCat (str, ", complete sequence");
            }
          }
        } else if (dfp->subtype == FEATDEF_rRNA || dfp->subtype == FEATDEF_otherRNA) {
          rrp = (RnaRefPtr) dfp->sfp->data.value.ptrvalue;
          if (rrp != NULL) {
            if (rrp->ext.choice == 1) {
              StringNCpy_0 (str, (CharPtr) rrp->ext.value.ptrvalue, sizeof (str) - 50);
              if (dfp->subtype == FEATDEF_rRNA) {
                ptr = StringISearch (str, " rRNA");
                if (ptr != NULL) {
                  *ptr = '\0';
                }
                ptr = StringISearch (str, " ribosomal RNA");
                if (ptr != NULL) {
                  *ptr = '\0';
                }
                if (! StringHasNoText (str)) {
                  StringCat (str, " ribosomal RNA");
                }
              } else if (dfp->subtype == FEATDEF_otherRNA) {
              }
              if (dfp->genename != NULL) {
                StringNCpy_0 (text, dfp->genename, sizeof (text));
                if (! StringHasNoText (text)) {
                  StringCat (str, " (");
                  StringCat (str, text);
                  StringCat (str, ")");
                }
              }
              if (dfp->lastInString || dfp->lastInGroup || dfp->lastInType) {
                if (dfp->subtype == FEATDEF_rRNA && mip->biomol == MOLECULE_TYPE_GENOMIC) {
                  StringCat (str, " gene");
                  if (count > 1) {
                    StringCat (str, "s");
                  }
                }
              }
              if (dfp->lastInGroup || dfp->lastInType) {
                if (dfp->sfp->partial) {
                  StringCat (str, ", partial sequence");
                } else {
                  StringCat (str, ", complete sequence");
                }
              }
            }
          }
        } else if (dfp->subtype == FEATDEF_tRNA) {
          rrp = (RnaRefPtr) dfp->sfp->data.value.ptrvalue;
          if (rrp != NULL) {
            if (rrp->ext.choice == 2) {
              trna = rrp->ext.value.ptrvalue;
              if (trna != NULL) {
                if (FeatDefLabel (dfp->sfp, str, sizeof (str) - 2, OM_LABEL_CONTENT) > 0) {
                  if (dfp->genename != NULL) {
                    StringNCpy_0 (text, dfp->genename, sizeof (text));
                    if (! StringHasNoText (text)) {
                      StringCat (str, " (");
                      StringCat (str, text);
                      StringCat (str, ")");
                    }
                  }
                  if (dfp->lastInGroup || dfp->lastInType) {
                    StringCat (str, " gene");
                    if (count > 1) {
                      StringCat (str, "s");
                    }
                    if (dfp->sfp->partial) {
                      StringCat (str, ", partial sequence");
                    } else {
                      StringCat (str, ", complete sequence");
                    }
                  }
                }
              }
            }
          }
        } else if (dfp->subtype == FEATDEF_LTR) {
          if (! StringHasNoText (sfp->comment)) {
            StringNCpy_0 (str, sfp->comment, sizeof (str));
            ptr = StringISearch (str, " long terminal repeat");
            if (ptr != NULL) {
              *ptr = '\0';
            }
            ptr = StringISearch (str, " long terminal repeat");
            if (ptr != NULL) {
              *ptr = '\0';
            }
            if (! StringHasNoText (str)) {
              StringCat (str, " long terminal repeat");
            }
          } else {
            /* StringCpy (str, "uncharacterized"); */
            StringCpy (str, "long terminal repeat");
          }
          if (dfp->lastInGroup || dfp->lastInType) {
            if (dfp->sfp->partial) {
              StringCat (str, ", partial sequence");
            } else {
              StringCat (str, ", complete sequence");
            }
          }
        } else if (dfp->subtype == FEATDEF_exon && target != NULL) {
          if (dfp->protname != NULL) {
            str [0] = '\0';
            if (! dfp->suppressprefix) {
              StringNCpy_0 (str, dfp->protname, sizeof (str) - 100);
              if (dfp->genename != NULL) {
                StringNCpy_0 (text, dfp->genename, sizeof (text));
                if (! StringHasNoText (text)) {
                  StringCat (str, " (");
                  StringCat (str, text);
                  StringCat (str, ") gene,");
                  if (/* count < 2 && */ /* dfp->altSplices < 2 && */ (! StringHasNoText (dfp->allelename))) {
                    StringNCpy_0 (text, dfp->allelename, sizeof (text));
                    StringCat (str, " ");
                    StringCat (str, text);
                    StringCat (str, " allele,");
                  }
                }
              }
            }
            if ((! StringHasNoText (str)) || dfp->suppressprefix) {
              exonnumber = GetExonNumber (sfp->qual);
              if (! dfp->suppressprefix) {
                if (exonnumber == NULL) {
                  if (StringStr (sfp->comment, "exon") != NULL && (! NextIsExon (vnp))) {
                  } else {
                    StringCat (str, " exon");
                    if (NextIsExon (vnp)) {
                      StringCat (str, "s");
                    }
                  }
                } else {
                  StringCat (str, " exon");
                  if (NextIsExon (vnp)) {
                    StringCat (str, "s");
                  }
                }
              }
              if (exonnumber != NULL) {
                if (! dfp->suppressprefix) {
                  StringCat (str, " ");
                }
                StringCat (str, exonnumber);
              } else {
                if (! StringHasNoText (sfp->comment)) {
                  if (! dfp->suppressprefix) {
                    StringCat (str, " ");
                  }
                  StringCat (str, sfp->comment);
                }
              }
            }
          } else if (dfp->genename != NULL) {
            str [0] = '\0';
            if (! dfp->suppressprefix) {
              StringNCpy_0 (str, dfp->genename, sizeof (str));
              StringCat (str, " gene,");
            }
            if ((! StringHasNoText (str)) || dfp->suppressprefix) {
              exonnumber = GetExonNumber (sfp->qual);
              if (! dfp->suppressprefix) {
                if (exonnumber == NULL) {
                  if (StringStr (sfp->comment, "exon") != NULL && (! NextIsExon (vnp))) {
                  } else {
                    StringCat (str, " exon");
                    if (NextIsExon (vnp)) {
                      StringCat (str, "s");
                    }
                  }
                } else {
                  StringCat (str, " exon");
                  if (NextIsExon (vnp)) {
                    StringCat (str, "s");
                  }
                }
              }
              if (exonnumber != NULL) {
                if (! dfp->suppressprefix) {
                  StringCat (str, " ");
                }
                StringCat (str, exonnumber);
              } else {
                if (! StringHasNoText (sfp->comment)) {
                  if (! dfp->suppressprefix) {
                    StringCat (str, " ");
                  }
                  StringCat (str, sfp->comment);
                }
              }
            }
          } else {
            StringCpy (str, "uncharacterized exon");
          }
          /*
          if (dfp->lastInGroup || dfp->lastInType) {
            if (dfp->sfp->partial) {
              StringCat (str, ", partial sequence");
            } else {
              StringCat (str, ", complete sequence");
            }
          }
          */
        } else if (dfp->numUnknown > 0) {
          if (mip != NULL && mip->biomol == MOLECULE_TYPE_MRNA) {
            StringCat (str, "unknown mRNA");
          } else if (mip->biomol == MOLECULE_TYPE_PRE_MRNA) {
            StringCat (str, " unknown precursor RNA");
          } else {
            StringCat (str, "unknown gene");
          }
          if (dfp->numUnknown > 1) {
            StringCat (str, "s");
          }
        }
      }
    }
    vnp = vnp->next;
    if (! StringHasNoText (str)) {
      if (vnp == NULL) {
        if (biop != NULL) {
          orgnelle [0] = '\0';
          switch (biop->genome) {
            case GENOME_chloroplast :
            case GENOME_chromoplast :
            case GENOME_kinetoplast :
            case GENOME_mitochondrion :
            case GENOME_plastid :
            case GENOME_cyanelle :
            case GENOME_apicoplast :
            case GENOME_leucoplast :
            case GENOME_proplastid :
              if (mitocount > 1) {
                sprintf (orgnelle, "; %s genes for %s products", organelleByGenome [biop->genome], organelleByGenome [biop->genome]);
              } else {
                sprintf (orgnelle, "; %s gene for %s product", organelleByGenome [biop->genome], organelleByGenome [biop->genome]);
              }
              StringCat (str, orgnelle);
              break;
            default :
              if (mitochloroflag > 0) {
                if (mitochloroflag > 10) {
                  /* beyond list */
                } else if (mitochloroflag == 10) {
                  StringCat (str, "; alternatively spliced");
                } else {
                  if (mitocount > 1) {
                    sprintf (orgnelle, "; nuclear genes for %s products", organelleByPopup [mitochloroflag]);
                  } else {
                    sprintf (orgnelle, "; nuclear gene for %s product", organelleByPopup [mitochloroflag]);
                  }
                  StringCat (str, orgnelle);
                }
              }
              break;
          }
          /*
          if (biop->genome == GENOME_mitochondrion) {
            if (mitocount > 1) {
              StringCat (str, "; mitochondrial genes for mitochondrial products");
            } else {
              StringCat (str, "; mitochondrial gene for mitochondrial product");
            }
          } else if (biop->genome == GENOME_chloroplast) {
            if (mitocount > 1) {
              StringCat (str, "; chloroplast genes for chloroplast products");
            } else {
              StringCat (str, "; chloroplast gene for chloroplast product");
            }
          } else if (mitochloroflag > 0) {
            switch (mitochloroflag) {
              case 1 :
                if (mitocount > 1) {
                  StringCat (str, "; nuclear genes for mitochondrial products");
                } else {
                  StringCat (str, "; nuclear gene for mitochondrial product");
                }
                break;
              case 2 :
                if (mitocount > 1) {
                   StringCat (str, "; nuclear genes for chloroplast products");
                 } else {
                   StringCat (str, "; nuclear gene for chloroplast product");
                 }
                break;
              case 3 :
                if (mitocount > 1) {
                  StringCat (str, "; mitochondrial genes for mitochondrial products");
                } else {
                  StringCat (str, "; mitochondrial gene for mitochondrial product");
                }
                break;
              case 4 :
                if (mitocount > 1) {
                   StringCat (str, "; chloroplast genes for chloroplast products");
                 } else {
                   StringCat (str, "; chloroplast gene for chloroplast product");
                 }
                break;
              default :
                break;
            }
          }
          */
        }
        StringCat (str, ".");
      } else if (vnp->next == NULL) {
        nextdfp = (DefFeatsPtr) vnp->data.ptrvalue;
        if (dfp->lastInPenultimate) {
          if ((dfp->subtype == FEATDEF_rRNA && nextdfp->subtype == FEATDEF_otherRNA &&
               dfp->sfp->partial == nextdfp->sfp->partial) ||
              (dfp->subtype == FEATDEF_otherRNA && nextdfp->subtype == FEATDEF_rRNA &&
               dfp->sfp->partial == nextdfp->sfp->partial)) {
            StringCat (str, ", and");
          } else if (dfp->subtype == FEATDEF_exon && nextdfp->subtype == FEATDEF_exon /* &&
               dfp->sfp->partial == nextdfp->sfp->partial */) {
            StringCat (str, " and");
          } else {
            StringCat (str, "; and");
          }
        } else if (dfp->lastInType || dfp->lastInGroup) {
          if (count > 1) {
            StringCat (str, ", and");
          } else {
            StringCat (str, " and");
          }
        } else if (nextdfp->lastInType || nextdfp->lastInGroup) {
          if (count > 1) {
            StringCat (str, ", and");
          } else {
            StringCat (str, " and");
          }
        } else {
          if (count > 1) {
            StringCat (str, ", and");
          } else {
            StringCat (str, " and");
          }
        }
      } else {
        nextdfp = (DefFeatsPtr) vnp->data.ptrvalue;
        if (dfp->lastInPenultimate) {
          if ((dfp->subtype == FEATDEF_rRNA && nextdfp->subtype == FEATDEF_otherRNA &&
               dfp->sfp->partial == nextdfp->sfp->partial) ||
              (dfp->subtype == FEATDEF_otherRNA && nextdfp->subtype == FEATDEF_rRNA &&
               dfp->sfp->partial == nextdfp->sfp->partial)) {
            StringCat (str, ", and");
          } else if (dfp->subtype == FEATDEF_exon && nextdfp->subtype == FEATDEF_exon /* &&
               dfp->sfp->partial == nextdfp->sfp->partial */) {
            StringCat (str, ",");
          } else {
            StringCat (str, "; and");
          }
        } else if (dfp->lastInType || dfp->lastInGroup) {
          StringCat (str, ";");
        } else if (nextdfp->lastInType || nextdfp->lastInGroup) {
          if (count > 1) {
            StringCat (str, ", and");
          } else {
            StringCat (str, " and");
          }
        } else {
          StringCat (str, ",");
        }
      }
      ValNodeCopyStr (&strings, 0, str);
    }
    if (dfp->lastInString || dfp->lastInGroup || dfp->lastInType) {
      count = 0;
    }
  }

  ptr = MergeValNodeStrings (strings, FALSE);
  if (nsep != NULL) {
    ttl = SeqEntryGetSeqDescr (nsep, Seq_descr_title, NULL);
    if (ttl == NULL) {
      ttl = SeqEntryGetSeqDescr (sep, Seq_descr_title, NULL);
    }
    if (ttl == NULL) {
      ttl = CreateNewDescriptor (nsep, Seq_descr_title);
    }
    if (ttl != NULL) {
      MemFree (ttl->data.ptrvalue);
      ttl->data.ptrvalue = ptr;
      ptr = NULL;
    }
  }
  MemFree (ptr);
  ValNodeFreeData (strings);
  ValNodeFreeData (head);
}

static void CombineProteinNames (DefFeatsPtr dfp1, DefFeatsPtr dfp2)

{
  Int2     i, j, lastspace, lastdash, lastcomma;
  size_t   len1, len2;
  CharPtr  str1 = NULL, str2 = NULL;

  if (dfp1 == NULL || dfp2 == NULL) return;
  if (dfp1->protname == NULL && dfp2->protname == NULL) return;
  len1 = StringLen (dfp1->protname);
  len2 = StringLen (dfp2->protname);
  if (len1 < 1 || len2 < 1) return;
  i = 0;
  j = 0;
  lastspace = 0;
  lastdash = 0;
  lastcomma = 0;
  while (i < len1 && j < len2 && dfp1->protname [i] == dfp2->protname [j]) {
    if (dfp1->protname [i] == ' ') {
      lastspace = i;
    }
    if (dfp1->protname [i] == '-') {
      lastdash = i;
    }
    if (dfp1->protname [i] == ',') {
      lastcomma = i;
    }
    i++;
    j++;
  }
  str1 = StringSave (dfp1->protname);
  if (str1 != NULL) {
    str1 [i] = '\0';
    if (! dfp1->alreadyTrimmed) {
      if (lastcomma > 0) {
        str1 [lastcomma] = '\0';
        dfp1->alreadyTrimmed = TRUE;
      } else if (lastdash > 0) {
        str1 [lastdash] = '\0';
        dfp1->alreadyTrimmed = TRUE;
      } else if (lastspace > 0) {
        str1 [lastspace] = '\0';
        dfp1->alreadyTrimmed = TRUE;
      }
    }
  }

  i = len1;
  j = len2;
  while (i > 0 && j > 0 && dfp1->protname [i - 1] == dfp2->protname [j - 1]) {
    i--;
    j--;
  }
  str2 = StringSave (dfp1->protname + i);
  TrimSpacesAroundString (str1);
  TrimSpacesAroundString (str2);
  len1 = StringLen (str1);
  len2 = StringLen (str2);
  if (len1 > len2) {
    dfp1->protname = str1;
    MemFree (str2);
  } else {
    dfp1->protname = str2;
    MemFree (str1);
  }
}

static Boolean AreAltSpliceGenes (DefFeatsPtr dfp1, DefFeatsPtr dfp2)

{
  Int2        comp;
  SeqFeatPtr  sfp1, sfp2;
  Uint1       strand1, strand2;

  if (dfp1 == NULL || dfp2 == NULL) return FALSE;
  if (dfp1->prot == NULL || dfp2->prot == NULL) return FALSE;
  if (dfp1->pseudo || dfp2->pseudo) return FALSE;
  sfp1 = dfp1->sfp;
  sfp2 = dfp2->sfp;
  if (sfp1 == NULL || sfp2 == NULL) return FALSE;
  if (sfp1->partial != sfp2->partial) return FALSE;
  strand1 = SeqLocStrand (sfp1->location);
  strand2 = SeqLocStrand (sfp2->location);
  if (strand1 != strand2) return FALSE;
  comp = SeqLocCompare (sfp1->location, sfp2->location);
  if (comp == SLC_NO_MATCH) return FALSE;
  if (dfp1->genename == NULL || dfp2->genename == NULL) return FALSE;
  if (StringICmp (dfp1->genename, dfp2->genename) != 0) return FALSE;
  CombineProteinNames (dfp1, dfp2);
  return TRUE;
}

static void MergeAltSpliceCDSs (ValNodePtr head)

{
  DefFeatsPtr  dfp1, dfp2;
  ValNodePtr   nextvnp;
  ValNodePtr   PNTR prevvnp;
  ValNodePtr   vnp1, vnp2;

  vnp1 = head;
  while (vnp1 != NULL) {
    dfp1 = (DefFeatsPtr) vnp1->data.ptrvalue;
    if (dfp1 != NULL && dfp1->subtype == FEATDEF_CDS) {
      vnp2 = vnp1->next;
      prevvnp = &(vnp1->next);
      while (vnp2 != NULL) {
        nextvnp = vnp2->next;
        dfp2 = (DefFeatsPtr) vnp2->data.ptrvalue;
        if (dfp2 != NULL && dfp2->subtype == FEATDEF_CDS) {
          if (AreAltSpliceGenes (dfp1, dfp2)) {
            (dfp1->altSplices)++;
            *prevvnp = vnp2->next;
            vnp2->next = NULL;
            ValNodeFreeData (vnp2);
          } else {
            prevvnp = (ValNodePtr PNTR) &(vnp2->next);
          }
        } else {
          prevvnp = (ValNodePtr PNTR) &(vnp2->next);
        }
        vnp2 = nextvnp;
      }
    }
    vnp1 = vnp1->next;
  }
}

static void AutoDefProc (Uint2 entityID, SeqEntryPtr sep, Boolean addMods,
                         Boolean labelMods, Int2 maxMods, Boolean leaveInParen,
                         BioseqPtr target, BioseqPtr seg, ValNodePtr nonUniqueOrgs,
                         Int2 mitochloroflag, Boolean suppressAltSplice, BioseqPtr parent)

{
  Char            allele [256];
  BioseqContextPtr  bcp;
  BioSourcePtr    biop;
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  SeqFeatPtr      cds;
  Boolean         change;
  CdRegionPtr     crp;
  DefFeatsData    df;
  DefFeatsPtr     dfp;
  DefFeatsPtr     dfpx;
  SeqMgrFeatContext  fcontext;
  GBQualPtr       gbq;
  SeqFeatPtr      gene;
  Int2            group;
  GeneRefPtr      grp;
  GatherScope     gs;
  ValNodePtr      head;
  SeqLocPtr       lastslp;
  Int4            left;
  size_t          lenallele;
  size_t          lenlocus;
  MolInfoPtr      mip;
  DefFeatsPtr     nextdfp;
  SeqLocPtr       nextslp;
  ValNodePtr      nextvnp;
  SeqEntryPtr     nsep;
  Int2            numUnknown;
  BioseqPtr       part;
  DefFeatsPtr     penult;
  ValNodePtr      PNTR prevvnp;
  ProtRefPtr      prp;
  CharPtr         ptr;
  Int4            right;
  ValNodePtr      sdp;
  SeqFeatPtr      sfp;
  SeqIdPtr        sip;
  SeqLocPtr       slp;
  Char            str [128];
  ValNodePtr      ttl;
  ValNodePtr      strings;
  ValNode         vn;
  ValNodePtr      vnp;
  ValNodePtr      vnpx;
  SeqFeatXrefPtr  xref;

  if (sep == NULL) return;
  if (target == NULL && seg == NULL) {
    if (IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp == NULL) return;
      if (bssp->_class == 7 ||
          (bssp->_class >= 13 && bssp->_class <= 16)) {
        for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
          AutoDefProc (entityID, sep, addMods, labelMods, maxMods, leaveInParen, NULL, NULL, nonUniqueOrgs, mitochloroflag, suppressAltSplice, NULL);
        }
        return;
      }
    }

    nsep = FindNucSeqEntry (sep);
    if (nsep != NULL) {
      bsp = (BioseqPtr) nsep->data.ptrvalue;
      if (bsp != NULL && bsp->repr == Seq_repr_seg && bsp->seq_ext != NULL && bsp->seq_ext_type == 1) {
        vn.choice = SEQLOC_MIX;
        vn.next = NULL;
        vn.data.ptrvalue = bsp->seq_ext;
        slp = SeqLocFindNext (&vn, NULL);
        while (slp != NULL) {
          nextslp = SeqLocFindNext (&vn, slp);
          sip = SeqLocId (slp);
          if (sip != NULL) {
            part = BioseqFind (sip);
            if (part != NULL) {
              AutoDefProc (entityID, sep, addMods, labelMods, maxMods, leaveInParen, part, NULL, nonUniqueOrgs, mitochloroflag, suppressAltSplice, bsp);
            }
          }
          slp = nextslp;
        }
        AutoDefProc (entityID, sep, addMods, labelMods, maxMods, leaveInParen, NULL, bsp, nonUniqueOrgs, mitochloroflag, suppressAltSplice, bsp);
        return;
      }
    }
  } else if (target != NULL) {
    nsep = SeqMgrGetSeqEntryForData (target);
  } else {
    nsep = SeqMgrGetSeqEntryForData (seg);
  }

  biop = NULL;
  strings = NULL;
  str [0] = '\0';

  SeqEntryToBioSource (sep, NULL, str, sizeof (str) - 1, &biop);
  if (! leaveInParen) {
    ptr = StringStr (str, "(");
    if (ptr != NULL) {
      *ptr = '\0';
    }
  }
  TrimSpacesAroundString (str);
  if (StringICmp (str, "Human immunodeficiency virus type 1") == 0) {
    StringCpy (str, "HIV-1");
  } else if (StringICmp (str, "Human immunodeficiency virus type 2") == 0) {
    StringCpy (str, "HIV-2");
  }
  str [0] = TO_UPPER (str [0]);
  ValNodeCopyStr (&strings, 0, str);

  mip = NULL;
  if (nsep != NULL) {
    bsp = (BioseqPtr) nsep->data.ptrvalue;
    bcp = BioseqContextNew (bsp);
    sdp = BioseqContextGetSeqDescr (bcp, Seq_descr_molinfo, NULL, NULL);
    BioseqContextFree (bcp);
    if (sdp != NULL) {
      mip = (MolInfoPtr) sdp->data.ptrvalue;
      if (mip != NULL) {
        if (mip->tech == MI_TECH_htgs_0 ||
            mip->tech == MI_TECH_htgs_1 ||
            mip->tech == MI_TECH_htgs_2 ||
            mip->tech == MI_TECH_est ||
            mip->tech == MI_TECH_sts ||
            mip->tech == MI_TECH_survey) {
          ttl = ValNodeExtract (&(bsp->descr), Seq_descr_title);
          if (ttl != NULL) {
            ttl = ValNodeFreeData (ttl);
          }
          return;
        }
      }
    }
  }

  /*
  nsep = FindNucSeqEntry (sep);
  if (nsep != NULL) {
    sdp = SeqEntryGetSeqDescr (nsep, Seq_descr_molinfo, NULL);
    if (sdp == NULL) {
      sdp = SeqEntryGetSeqDescr (sep, Seq_descr_molinfo, NULL);
    }
    if (sdp != NULL) {
      mip = (MolInfoPtr) sdp->data.ptrvalue;
    }
  }
  */

  if (nonUniqueOrgs != NULL) {
    for (vnp = nonUniqueOrgs; vnp != NULL; vnp = vnp->next) {
      if (StringICmp ((CharPtr) vnp->data.ptrvalue, str) == 0) {
        if (vnp->choice == 1) {
          addMods = FALSE; /* if only one organism in record, already unique defline */
        }
      }
    }
  }
  if (addMods) {
    AddOrgModsToDef (&strings, biop, labelMods);
    strings = SortValNode (strings, SortByVnpChoice);
    vnp = strings;
    while (vnp != NULL && maxMods > 0) {
      maxMods--;
      vnp = vnp->next;
    }
    if (vnp != NULL) {
      vnp->next = ValNodeFreeData (vnp->next);
    }
  }

  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.scope = sep;
  gs.target = NULL;
  if (target != NULL) {
    slp = ValNodeNew (NULL);
    slp->choice = SEQLOC_WHOLE;
    sip = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (target->id, 0)));
    slp->data.ptrvalue = sip;
    gs.target = slp;
  }
  head = NULL;
  GatherEntity (entityID, (Pointer) (&head), GetGeneCDStRNArRNAGatherFunc, &gs);
  gs.target = SeqLocFree (gs.target);
  head = SortValNode (head, SortCDStRNArRNAByLocation);

  if (head == NULL && mip != NULL && mip->tech == MI_TECH_survey) {
    ValNodeCopyStr (&strings, 0, ", genome survey sequence.");
  }

  numUnknown = 0;
  vnp = head;
  while (vnp != NULL) {
    dfp = (DefFeatsPtr) vnp->data.ptrvalue;
    if (dfp != NULL) {
      sfp = dfp->sfp;
      if (sfp != NULL) {
        FindGeneAndProtForCDS (entityID, sfp, &(dfp->gene), &(dfp->prot));
        grp = SeqMgrGetGeneXref (sfp);
        if (SeqMgrGeneIsSuppressed (grp)) {
          dfp->gene = NULL;
        }
        dfp->pseudo = FALSE;
        dfp->genename = NULL;
        dfp->allelename = NULL;
        grp = NULL;
        if (dfp->gene != NULL) {
          grp = (GeneRefPtr) dfp->gene->data.value.ptrvalue;
          if (grp != NULL && grp->pseudo) {
            dfp->pseudo = TRUE;
          }
        }
        xref = sfp->xref;
        while (xref != NULL && xref->data.choice != SEQFEAT_GENE) {
          xref = xref->next;
        }
        if (xref != NULL) {
          grp = (GeneRefPtr) xref->data.value.ptrvalue;
        }
        if (grp != NULL) {
          dfp->genename = (CharPtr) grp->locus;
          if ((! StringHasNoText (grp->locus)) && (! StringHasNoText (grp->allele))) {
            lenallele = StringLen (grp->allele);
            lenlocus = StringLen (grp->locus);
            if (lenallele > lenlocus && StringNICmp (grp->locus, grp->allele, lenlocus) == 0) {
              sprintf (allele, "%s", grp->allele);
            } else if (StringNCmp (grp->allele, "-", 1) == 0) {
              sprintf (allele, "%s%s", grp->locus, grp->allele);
            } else {
              sprintf (allele, "%s-%s", grp->locus, grp->allele);
            }
            dfp->allelename = StringSave (allele);
          }
          if (grp->pseudo) {
            dfp->pseudo = TRUE;
          }
        }
        if (dfp->subtype == FEATDEF_CDS) {
          if (target != NULL) {
            lastslp = NULL;
            slp = SeqLocFindNext (sfp->location, NULL);
            while (slp != NULL) {
              lastslp = slp;
              slp = SeqLocFindNext (sfp->location, slp);
            }
            if (lastslp != NULL && nsep != NULL) {
              bsp = (BioseqPtr) nsep->data.ptrvalue;
              if (GetBioseqGivenSeqLoc (lastslp, entityID) != bsp) {
                dfp->ignore = TRUE;
              }
            }
          }
          crp = (CdRegionPtr) sfp->data.value.ptrvalue;
          if (crp != NULL) {
            if (crp->orf) {
              dfp->ignore = TRUE;
            }
          }
          gbq = sfp->qual;
          while (gbq != NULL) {
            if (StringICmp (gbq->qual, "pseudo") == 0) {
              dfp->pseudo = TRUE;
            }
            gbq = gbq->next;
          }
          if (dfp->pseudo) {
          } else if (dfp->prot == NULL) {
            if (dfp->gene == NULL) {
              dfp->ignore = TRUE;
            }
          } else {
            prp = (ProtRefPtr) dfp->prot->data.value.ptrvalue;
            if (prp != NULL) {
              if (prp->name == NULL || StringHasNoText ((CharPtr) prp->name->data.ptrvalue)) {
                if (prp->desc == NULL || StringHasNoText (prp->desc)) {
                  dfp->ignore = TRUE;
                } else {
                  dfp->protname = prp->desc;
                }
              } else {
                dfp->protname = (CharPtr) prp->name->data.ptrvalue;
              }
              if (! dfp->ignore) {
                if (StringICmp (dfp->protname, "unknown") == 0) {
                  if (StringHasNoText (dfp->genename)) {
                    numUnknown++;
                    dfp->ignore = TRUE;
                  }
                }
              }
            }
          }
        } else if (dfp->subtype == FEATDEF_exon && target == NULL) {
          dfp->ignore = TRUE;
        }
      }
    }
    vnp = vnp->next;
  }

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    dfp = (DefFeatsPtr) vnp->data.ptrvalue;
    if (dfp != NULL) {
      sfp = dfp->sfp;
      if (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_GENE) {
          for (vnpx = vnp->next; vnpx != NULL; vnpx = vnpx->next) {
            dfpx = (DefFeatsPtr) vnpx->data.ptrvalue;
            if (dfpx != NULL) {
              if (sfp == dfpx->gene) {
                dfp->ignore = TRUE;
              }
            }
          }
        }
      }
    }
  }

  vnp = head;
  prevvnp = &head;
  while (vnp != NULL) {
    nextvnp = vnp->next;
    dfp = (DefFeatsPtr) vnp->data.ptrvalue;
    if (dfp->ignore) {
      *prevvnp = vnp->next;
      vnp->next = NULL;
      ValNodeFreeData (vnp);
    } else {
      prevvnp = (ValNodePtr PNTR) &(vnp->next);
    }
    vnp = nextvnp;
  }

  if (! suppressAltSplice) {
    MergeAltSpliceCDSs (head);
  }

  if (target != NULL) {
    head = SortValNode (head, SortCDSAfterExons);
  }

  if (numUnknown > 0) {
    dfp = (DefFeatsPtr) MemNew (sizeof (DefFeatsData));
    if (dfp != NULL) {
      dfp->entityID = entityID;
      dfp->subtype = 0;
      dfp->numUnknown = numUnknown;
      ValNodeAddPointer (&head, 0, (Pointer) dfp);
    }
  }

  if (target != NULL) {
    for (vnp = head; vnp != NULL; vnp = vnp->next) {
      dfp = (DefFeatsPtr) vnp->data.ptrvalue;
      if (dfp != NULL && dfp->subtype == FEATDEF_exon && dfp->gene != NULL) {
        gene = SeqMgrGetDesiredFeature (entityID, NULL, 0, 0, dfp->gene, &fcontext);
        if (gene == dfp->gene && (! gene->pseudo)) {
          left = fcontext.left;
          right = fcontext.right;
          cds = SeqMgrGetNextFeature (parent, NULL, SEQFEAT_CDREGION, 0, &fcontext);
          while (cds != NULL) {
            if (fcontext.left >= left && fcontext.right <= right && (! cds->pseudo)) {
              if (dfp->protname == NULL) {
                dfp->protname = fcontext.label; /* points to stable string */
              } else {
                df.protname = fcontext.label;
                CombineProteinNames (dfp, &df);
              }
            }
            cds = SeqMgrGetNextFeature (parent, cds, SEQFEAT_CDREGION, 0, &fcontext);
          }
        }
      }
    }
    vnp = head;
    while (vnp != NULL) {
      dfp = (DefFeatsPtr) vnp->data.ptrvalue;
      vnp = vnp->next;
      if (dfp != NULL && dfp->subtype == FEATDEF_exon && vnp != NULL) {
        nextdfp = (DefFeatsPtr) vnp->data.ptrvalue;
        if (nextdfp != NULL && (nextdfp->subtype == FEATDEF_exon || nextdfp->subtype == FEATDEF_CDS)) {
          if (StringCmp (dfp->genename, nextdfp->genename) == 0 &&
              StringCmp (dfp->protname, nextdfp->protname) == 0) {
            nextdfp->suppressprefix = TRUE;
          }
        }
      }
    }
  }

  vnp = head;
  group = 0;
  penult = NULL;
  while (vnp != NULL) {
    dfp = (DefFeatsPtr) vnp->data.ptrvalue;
    vnp->choice = (Uint1) group;
    vnp = vnp->next;
    if (vnp != NULL) {
      change = FALSE;
      nextdfp = (DefFeatsPtr) vnp->data.ptrvalue;
      if ((dfp->subtype == FEATDEF_rRNA && nextdfp->subtype == FEATDEF_otherRNA) ||
          (dfp->subtype == FEATDEF_otherRNA && nextdfp->subtype == FEATDEF_rRNA)) {
        dfp->lastInString = TRUE;
        if (dfp->sfp->partial != nextdfp->sfp->partial) {
          dfp->lastInGroup = TRUE;
        }
        change = TRUE;
      } else if (dfp->subtype != nextdfp->subtype) {
        if (dfp->subtype == FEATDEF_exon && nextdfp->subtype == FEATDEF_CDS && nextdfp->suppressprefix) {
          /* no separator between exons and appropriate cds */
        } else {
          dfp->lastInType = TRUE;
          change = TRUE;
        }
      } else if (dfp->sfp->partial != nextdfp->sfp->partial) {
        dfp->lastInGroup = TRUE;
        change = TRUE;
      } else if (dfp->pseudo != nextdfp->pseudo) {
        dfp->lastInGroup = TRUE;
        change = TRUE;
      } else if (dfp->allelename != NULL || nextdfp->allelename != NULL) {
        dfp->lastInGroup = TRUE;
        change = TRUE;
      } else if (dfp->altSplices > 1 || nextdfp->altSplices > 1) {
        dfp->lastInGroup = TRUE;
        change = TRUE;
      }
      if (change) {
        group++;
        penult = dfp;
      }
    } else {
      dfp->lastInString = TRUE;
      dfp->lastInGroup = TRUE;
      dfp->lastInType = TRUE;
      group++;
    }
  }
  if (penult != NULL) {
    penult->lastInPenultimate = TRUE;
  }

  FinishAutoDefProc (entityID, sep, head, target, nsep, mip, strings, biop, mitochloroflag);
}

static CharPtr sourceModRankList [] = {
  "Strain", "Isolate", "Clone", "Type", "Cultivar", "Haplotype",
  "Substrain", "Subclone", "Subtype", "Serotype", "Serogroup", "Serovar",
  "Variety", "Pathovar", "Chemovar", "Biovar", "Biotype", "Group", "Subgroup",
  "Cell-line", "Cell-type", "Tissue-type", "Clone-lib", "Tissue-lib", "Dev-stage",
  "Lab-host", "Pop-variant", "Frequency", "Germline", "Rearranged", "Transgenic",
  "Environmental-sample", "Chromosome", "Segment", "Map", "Genotype", "Sex",
  "Plasmid-name", "Transposon-name", "Ins-seq-name", "Endogenous-virus-name",
  "Plastid-name", "Country", "Old Name", "Common", "Acronym", "Dosage",
  "Natural-host", "Sub-species", "Specimen-voucher", "Isolation-source",
  "Authority", "Forma", "Forma-specialis", "Ecotype", "Synonym", "Anamorph",
  "Teleomorph", "Breed",
  NULL
};

typedef struct {
  CharPtr value;
  Int2    rank;
} AlphaInfo, PNTR AlphaInfoPtr;

static AlphaInfoPtr sourceModAlphaList = NULL;

typedef struct deflineform {
  FORM_MESSAGE_BLOCK
  SeqEntryPtr    sep;
  ButtoN         addLabels;
  GrouP          customGrp;
  GrouP          sourceListGrp;
  ButtoN         PNTR sourceBoxList;
  PopuP          modLimit;
  ButtoN         onlyModifyTargeted;
  BioseqPtr      target;
  ButtoN         leaveInParentheses;
  /*
  GrouP          nucformitoorchloro;
  */
  PopuP          nucformitoorchloro;
  Boolean        smartMods;
  ButtoN         suppressAltSplice;
} DeflineForm, PNTR DeflineFormPtr;

static Boolean GatherOrgnamesFunc (GatherContextPtr gcp)

{
  BioSourcePtr     biop;
  OrgRefPtr        orp;
  ValNodePtr       sdp;
  SeqFeatPtr       sfp;
  CharPtr          str;
  ValNodePtr PNTR  vnpp;

  if (gcp == NULL || gcp->thisitem == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT  && gcp->thistype != OBJ_SEQDESC) return TRUE;
  vnpp = (ValNodePtr PNTR) gcp->userdata;
  if (vnpp == NULL) return TRUE;

  orp = NULL;
  biop = NULL;
  switch (gcp->thistype) {
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gcp->thisitem;
      switch (sfp->data.choice) {
        case SEQFEAT_ORG :
          orp = (OrgRefPtr) sfp->data.value.ptrvalue;
          break;
        case SEQFEAT_BIOSRC :
          biop = (BioSourcePtr) sfp->data.value.ptrvalue;
          break;
        default :
          break;
      }
      break;
    case OBJ_SEQDESC :
      sdp = (ValNodePtr) gcp->thisitem;
      switch (sdp->choice) {
        case Seq_descr_org :
          orp = (OrgRefPtr) sdp->data.ptrvalue;
          break;
        case Seq_descr_source :
          biop = (BioSourcePtr) sdp->data.ptrvalue;
          break;
        default :
          break;
      }
      break;
    default :
      break;
  }

  if (orp == NULL && biop != NULL) {
    orp = biop->org;
  }
  if (orp != NULL) {
    str = StringSave (orp->taxname);
    if (str == NULL) return TRUE;
    TrimSpacesAroundString (str);
    if (StringICmp (str, "Human immunodeficiency virus type 1") == 0) {
      StringCpy (str, "HIV-1");
    } else if (StringICmp (str, "Human immunodeficiency virus type 2") == 0) {
      StringCpy (str, "HIV-2");
    }
    str [0] = TO_UPPER (str [0]);
    ValNodeAddStr (vnpp, 1, str);
  }
  return TRUE;
}

static void DefLineModFormAcceptProc (ButtoN b)

{
  EnumFieldAssocPtr  ap;
  Int2               count;
  DeflineFormPtr     dfp;
  GatherScope        gs;
  Boolean            labelMods;
  Boolean            leaveInParen;
  Int2               maxMods;
  Int2               mitochloroflag;
  ValNodePtr         nextvnp;
  ValNodePtr         nonUniqueOrgs;
  Boolean            suppressAltSplice;
  Int2               val;
  ValNodePtr         vnp;

  dfp = (DeflineFormPtr) GetObjectExtra (b);
  if (dfp == NULL) return;
  WatchCursor ();
  Hide (dfp->form);
  Update ();
  labelMods = GetStatus (dfp->addLabels);

  if (dfp->sourceBoxList != NULL && GetValue (dfp->customGrp) == 2) {
    count = 0;
    while ((sourceModAlphaList [count].value != NULL) &&
	   (dfp->sourceBoxList [count] != NULL)) {
      if (! GetStatus (dfp->sourceBoxList [count])) {
        for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
          if (StringICmp (ap->name, sourceModAlphaList [count].value) == 0 &&
              ap->value > 0 && ap->value < 32) {
            orgmod_rank [ap->value] = 0;
          }
        }
        for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
          if (StringICmp (ap->name, sourceModAlphaList [count].value) == 0 &&
              ap->value > 0 && ap->value < 25) {
            subsource_rank [ap->value] = 0;
          }
        }
      }
      count++;
    }
  }

  maxMods = INT2_MAX;
  val = GetValue (dfp->modLimit);
  if (val > 1) {
    maxMods = val - 1;
  }

  if (dfp->onlyModifyTargeted != NULL && GetStatus (dfp->onlyModifyTargeted)) {
    dfp->sep = GetBestTopParentForData (dfp->input_entityID, dfp->target);
  }

  leaveInParen = FALSE;
  if (dfp->leaveInParentheses != NULL && GetStatus (dfp->leaveInParentheses)) {
    leaveInParen = TRUE;
  }

  nonUniqueOrgs = NULL;
  if (dfp->smartMods) {
    MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
    gs.seglevels = 1;
    MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
    gs.ignore[OBJ_BIOSEQ] = FALSE;
    gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
    gs.ignore[OBJ_SEQANNOT] = FALSE;
    gs.ignore[OBJ_SEQFEAT] = FALSE;
    gs.ignore[OBJ_SEQDESC] = FALSE;
    GatherEntity (dfp->input_entityID, (Pointer) (&nonUniqueOrgs), GatherOrgnamesFunc, &gs);
    nonUniqueOrgs = SortValNode (nonUniqueOrgs, SortByName);
    vnp = nonUniqueOrgs;
    while (vnp != NULL) {
      nextvnp = vnp->next;
      if (nextvnp != NULL && StringICmp ((CharPtr) vnp->data.ptrvalue, (CharPtr) nextvnp->data.ptrvalue) == 0) {
        vnp->next = nextvnp->next;
        nextvnp->next = NULL;
        ValNodeFreeData (nextvnp);
        nextvnp = vnp;
        (vnp->choice)++;
      }
      vnp = nextvnp;
    }
  }

  mitochloroflag = GetValue (dfp->nucformitoorchloro) - 1;
  suppressAltSplice = GetStatus (dfp->suppressAltSplice);
  AutoDefProc (dfp->input_entityID, dfp->sep, TRUE, labelMods, maxMods, leaveInParen, NULL, NULL, nonUniqueOrgs, mitochloroflag, suppressAltSplice, NULL);
  ValNodeFreeData (nonUniqueOrgs);
  ClearProteinTitlesInNucProts (dfp->input_entityID, NULL);
  InstantiateProteinTitles (dfp->input_entityID, NULL);
  ArrowCursor ();
  Remove (dfp->form);
  Update ();
  ObjMgrSetDirtyFlag (dfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, dfp->input_entityID, 0, 0);
}

static void ChangeCustomGrp (GrouP g)

{
  DeflineFormPtr  dfp;

  dfp = (DeflineFormPtr) GetObjectExtra (g);
  if (dfp == NULL) return;
  if (GetValue (g) == 1) {
    SafeDisable (dfp->sourceListGrp);
  } else {
    SafeEnable (dfp->sourceListGrp);
  }
}

static void CleanupDeflineForm (GraphiC g, VoidPtr data)

{
  DeflineFormPtr  dfp;

  dfp = (DeflineFormPtr) data;
  if (dfp != NULL) {
    MemFree (dfp->sourceBoxList);
  }
  StdCleanupFormProc (g, data);
}

static void DeflineMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* SourceModCompare () -- Compare callback for qsort function used in  */
/*                        AlphabetizeSourceMods ().                    */
/*                                                                     */
/*---------------------------------------------------------------------*/

static int SourceModCompare (const void *p1, const void *p2)
{
  AlphaInfoPtr sourceMod1;
  AlphaInfoPtr sourceMod2;

  sourceMod1 = (AlphaInfoPtr) p1;
  sourceMod2 = (AlphaInfoPtr) p2;

  if (StringCmp (sourceMod1->value, sourceMod2->value) < 0)
    return -1;
  else if (StringCmp (sourceMod1->value, sourceMod2->value) > 0)
    return 1;
  else
    return 0;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* AlphabetizeSourceMods () -- Create an alphabetized copy of the list */
/*                             of source mods in sourceModRankList.    */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void AlphabetizeSourceMods ()
{
  Int2 i;
  Int2 count;

  /* If already alphabetized, don't re-do */

  if (sourceModAlphaList != NULL)
    return;

  /* Size the new list based on the existing one */
  /* and initialize it to all zeros.             */

  count = 0;
  while (sourceModRankList [count] != NULL)
    count++;

  sourceModAlphaList = (AlphaInfoPtr) MemNew ((count+1) * sizeof (AlphaInfo));
  MemSet (sourceModAlphaList, (count+1) * sizeof (AlphaInfo), 0);

  /* Point each item in the new list to the old */

  for (i = 0; i < count; i++) {
    sourceModAlphaList[i].value = sourceModRankList[i];
    sourceModAlphaList[i].rank = i;
  }

  /* Sort the new list */

  qsort ((void *) sourceModAlphaList, count, sizeof (AlphaInfo),
	 SourceModCompare);
  
}

static ForM CreateDefLineModForm (Uint2 entityID, SeqEntryPtr sep, BioseqPtr target,
                                  Boolean smartMods, Int2 maxCount)

{
  ButtoN          b;
  GrouP           c;
  Int2            count;
  DeflineFormPtr  dfp;
  GrouP           h;
  Int2            i;
  GrouP           p;
  GrouP           q;
  Char            str [16];
  WindoW          w;

  w = NULL;
  dfp = MemNew (sizeof (DeflineForm));
  if (dfp != NULL) {
    w = FixedWindow (-50, -33, -10, -10, "Defline Modifier Customization", StdCloseWindowProc);
    SetObjectExtra (w, dfp, CleanupDeflineForm);
    dfp->form = (ForM) w;
    dfp->formmessage = DeflineMessageProc;
    dfp->input_entityID = entityID;
    dfp->sep = sep;
    dfp->target = target;
    dfp->smartMods = smartMods;

    h = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (h, 10, 10);

    dfp->addLabels = CheckBox (h, "Use Labels (e.g., 'strain BALB/c')", NULL);
    if (smartMods) {
      SetStatus (dfp->addLabels, TRUE);
    }

    p = NormalGroup (h, -1, 0, "Modifier Classes", programFont, ChangeCustomGrp);
    SetGroupSpacing (p, 10, 10);

    dfp->customGrp = HiddenGroup (p, 2, 0, ChangeCustomGrp);
    SetObjectExtra (dfp->customGrp, dfp, NULL);
    RadioButton (dfp->customGrp, "All");
    RadioButton (dfp->customGrp, "Custom");
    SetValue (dfp->customGrp, 1);

    dfp->sourceListGrp = HiddenGroup (p, 3, 0, NULL);
    count = 0;
    while (sourceModRankList [count] != NULL) {
      count++;
    }

    /* Create an alphabetically sorted list for */
    /* display purposes.                        */

    AlphabetizeSourceMods ();

    dfp->sourceBoxList = MemNew (sizeof (ButtoN) * (count + 3));

    if (count > maxCount)
      count = maxCount;
    if (dfp->sourceBoxList != NULL) {
      for (i = 0; i < count; i++)
        dfp->sourceBoxList [i] = CheckBox (dfp->sourceListGrp,
					   sourceModAlphaList [i].value,
					   NULL);
    }
    Disable (dfp->sourceListGrp);

    q = HiddenGroup (h, 4, 0, NULL);
    StaticPrompt (q, "Maximum modifiers per line", 0, popupMenuHeight, programFont, 'l');
    dfp->modLimit = PopupList (q, TRUE, NULL);
    PopupItem (dfp->modLimit, "no limit");
    for (i = 1; i <= count; i++) {
      sprintf (str, "%d", (int) i);
      PopupItem (dfp->modLimit, str);
    }
    if (smartMods) {
      SetValue (dfp->modLimit, 2);
    } else {
      SetValue (dfp->modLimit, 1);
    }

    /*
    dfp->nucformitoorchloro = HiddenGroup (h, -1, 0, NULL);
    RadioButton (dfp->nucformitoorchloro, "No mitochondrial or chloroplast suffix");
    RadioButton (dfp->nucformitoorchloro, "Nuclear gene(s) for mitochondrial product(s)");
    RadioButton (dfp->nucformitoorchloro, "Nuclear gene(s) for chloroplast product(s)");
    RadioButton (dfp->nucformitoorchloro, "Mitochondrial gene(s) for mitochondrial product(s)");
    RadioButton (dfp->nucformitoorchloro, "Chloroplast gene(s) for chloroplast product(s)");
    */
    dfp->nucformitoorchloro = PopupList (h, FALSE, NULL);
    PopupItem (dfp->nucformitoorchloro, "No mitochondrial or chloroplast suffix");
    PopupItem (dfp->nucformitoorchloro, "Nuclear gene(s) for mitochondrial product(s)");
    PopupItem (dfp->nucformitoorchloro, "Nuclear gene(s) for chloroplast product(s)");
    PopupItem (dfp->nucformitoorchloro, "Nuclear gene(s) for kinetoplast product(s)");
    PopupItem (dfp->nucformitoorchloro, "Nuclear gene(s) for plastid product(s)");
    PopupItem (dfp->nucformitoorchloro, "Nuclear gene(s) for chromoplast product(s)");
    PopupItem (dfp->nucformitoorchloro, "Nuclear gene(s) for cyanelle product(s)");
    PopupItem (dfp->nucformitoorchloro, "Nuclear gene(s) for apicoplast product(s)");
    PopupItem (dfp->nucformitoorchloro, "Nuclear gene(s) for leucoplast product(s)");
    PopupItem (dfp->nucformitoorchloro, "Nuclear gene(s) for proplastid product(s)");
    PopupItem (dfp->nucformitoorchloro, "Alternatively Spliced");
    SetValue (dfp->nucformitoorchloro, 1);

    dfp->leaveInParentheses = CheckBox (w, "Leave in parenthetical organism info", NULL);
    SetStatus (dfp->leaveInParentheses, TRUE);

    dfp->suppressAltSplice = CheckBox (w, "Suppress alternative splice phrase", NULL);

    dfp->onlyModifyTargeted = NULL;
    if (target != NULL) {
      dfp->onlyModifyTargeted = CheckBox (w, "Only modify targeted record", NULL);
    }

    c = HiddenGroup (w, 4, 0, NULL);
    b = PushButton (c, "Accept", DefLineModFormAcceptProc);
    SetObjectExtra (b, dfp, NULL);
    PushButton (c, "Cancel", StdCancelButtonProc);

    AlignObjects (ALIGN_CENTER, (HANDLE) dfp->addLabels, (HANDLE) dfp->customGrp,
                  (HANDLE) dfp->sourceListGrp, (HANDLE) q,
                  (HANDLE) dfp->leaveInParentheses,
                  (HANDLE) dfp->suppressAltSplice,
                  (HANDLE) dfp->nucformitoorchloro, (HANDLE) c,
                  (HANDLE) dfp->onlyModifyTargeted, NULL);

    RealizeWindow (w);
  }
  if (smartMods) {
    DefLineModFormAcceptProc (b);
    return NULL;
  }
  return (ForM) w;
}

extern void GenerateAutomaticDefLinesCommon (IteM i, Boolean addMods, Boolean smartMods, ButtoN b)

{
  EnumFieldAssocPtr  ap;
  BaseFormPtr        bfp;
  BioseqSetPtr       bssp;
  Int2               count;
  ForM               f;
  Int2               maxCount;
  SeqEntryPtr        sep;
  BioseqPtr          target;

  if (b != NULL) {
    bfp = GetObjectExtra (b);
  } else {
#ifdef WIN_MAC
    bfp = currentFormDataPtr;
#else
    bfp = GetObjectExtra (i);
#endif
  }
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  MemSet ((Pointer) (orgmod_rank), (int)(0), sizeof(orgmod_rank));
  MemSet ((Pointer) (subsource_rank), (int)(0), sizeof(subsource_rank));

  AlphabetizeSourceMods ();

  count = 0;

  while (sourceModAlphaList [count].value != NULL) {
    for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
      if (StringICmp (ap->name, sourceModAlphaList [count].value) == 0 &&
          ap->value > 0 && ap->value < 32) {
        orgmod_rank [ap->value] = sourceModAlphaList[count].rank + 1;
      }
    }
    for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
      if (StringICmp (ap->name, sourceModAlphaList [count].value) == 0 &&
          ap->value > 0 && ap->value < 25) {
        subsource_rank [ap->value] = sourceModAlphaList[count].rank + 1;
      }
    }
    count++;
  }

  if (smartMods) {
    if (CountSeqEntryComponents (sep) == 1) {
      smartMods = FALSE;
    } else if (IS_Bioseq_set (sep) && b != NULL /* from toolbar */ ) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp != NULL && bssp->_class == BioseqseqSet_class_genbank) {
        smartMods = FALSE;
      }
    }
  }

  if (addMods || smartMods) {
    target =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
    if (addMods) {
      maxCount = INT2_MAX;
    } else {
      maxCount = 6;
    }
    f = CreateDefLineModForm (bfp->input_entityID, sep, target, smartMods, maxCount);
    if (addMods && f != NULL) {
      Show (f);
      Select (f);
    } else if (bfp->activate != NULL) {
      bfp->activate ((WindoW) bfp->form);
    }
    return;
  }

  WatchCursor ();
  Update ();
  AutoDefProc (bfp->input_entityID, sep, FALSE, FALSE, INT2_MAX, TRUE, NULL, NULL, NULL, 0, FALSE, NULL);
  ClearProteinTitlesInNucProts (bfp->input_entityID, NULL);
  InstantiateProteinTitles (bfp->input_entityID, NULL);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

void GenerateAutoDefLinesNoMods (IteM i)

{
  GenerateAutomaticDefLinesCommon (i, FALSE, FALSE, NULL);
}

void GenerateAutoDefLinesWithMods (IteM i)

{
  GenerateAutomaticDefLinesCommon (i, TRUE, FALSE, NULL);
}

void GenerateAutoDefLinesSmartMods (IteM i)

{
  GenerateAutomaticDefLinesCommon (i, FALSE, TRUE, NULL);
}

static void StringToLower (CharPtr str)

{
  Char  ch;

  if (str == NULL) return;
  ch = *str;
  while (ch != '\0') {
    *str = TO_LOWER (ch);
    str++;
    ch = *str;
  }
}

static void MakeNucleotideTitlesInSequinStyle (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  EnumFieldAssocPtr  ap;
  BioseqContextPtr   bcp;
  BioSourcePtr       biop;
  BioseqPtr          bsp;
  OrgModPtr          mod;
  OrgNamePtr         onp;
  OrgRefPtr          orp;
  SeqDescrPtr        sdp;
  SubSourcePtr       ssp;
  CharPtr            str;
  Char               text [256];
  ValNodePtr         ttl;
  ValNodePtr         vnp;

  if (sep == NULL) return;
  if (! IS_Bioseq (sep)) return;
  bsp = sep->data.ptrvalue;
  if (bsp == NULL) return;
  if (! ISA_na (bsp->mol)) return;
  bcp = BioseqContextNew (bsp);
  sdp = BioseqContextGetSeqDescr (bcp, Seq_descr_source, NULL, NULL);
  BioseqContextFree (bcp);
  if (sdp == NULL) return;
  biop = (BioSourcePtr) sdp->data.ptrvalue;
  if (biop == NULL) return;
  if (bsp->descr != NULL) {
    vnp = ValNodeExtract (&(bsp->descr), Seq_descr_title);
    vnp = ValNodeFreeData (vnp);
  }
  str = MemNew (2000);

  orp = biop->org;
  if (orp != NULL) {
    StringCpy (text, "[organism=");
    StringCat (text, orp->taxname);
    StringCat (text, "] ");
    StringCat (str, text);
  }

  ssp = biop->subtype;
  while (ssp != NULL) {
    for (ap = subsource_subtype_alist; ap->name != NULL; ap++) {
      if (ssp->subtype == ap->value) {
        StringCpy (text, "[");
        if (ap->value != 255) {
          StringCat (text, ap->name);
        } else {
          StringCat (text, "subsource");
        }
        StringToLower (text);
        StringCat (text, "=");
        StringCat (text, ssp->name);
        StringCat (text, "] ");
        StringCat (str, text);
      }
    }
    ssp = ssp->next;
  }
  orp = biop->org;
  if (orp != NULL) {
    onp = orp->orgname;
    if (onp != NULL) {
      mod = onp->mod;
      while (mod != NULL) {
        for (ap = orgmod_subtype_alist; ap->name != NULL; ap++) {
          if (mod->subtype == ap->value) {
            StringCpy (text, "[");
            if (ap->value != 255) {
              StringCat (text, ap->name);
            } else {
              StringCat (text, "note");
            }
            StringToLower (text);
            StringCat (text, "=");
            StringCat (text, mod->subname);
            StringCat (text, "] ");
            StringCat (str, text);
          }
        }
        mod = mod->next;
      }
    }
  }

  TrimSpacesAroundString (str);
  if (! StringHasNoText (str)) {
    ttl = CreateNewDescriptor (sep, Seq_descr_title);
    if (ttl != NULL) {
      ttl->data.ptrvalue = StringSave (str);
    }
  }
  MemFree (str);
}

extern Int2 LIBCALLBACK MakeSequinProteinTitles (Pointer data);
extern Int2 LIBCALLBACK MakeSequinNucleotideTitles (Pointer data);
extern Int2 LIBCALLBACK MakeSequinFeatureTable (Pointer data);
extern Int2 LIBCALLBACK MakeContigBuildTable (Pointer data);

extern Int2 LIBCALLBACK MakeSequinNucleotideTitles (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  SeqEntryExplore (sep, NULL, MakeNucleotideTitlesInSequinStyle);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  return OM_MSG_RET_DONE;
}

static void MakeProteinTitlesInSequinStyle (Uint2 entityID, SeqEntryPtr sep)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  DefFeatsPtr   dfp;
  GeneRefPtr    grp;
  GatherScope   gs;
  ValNodePtr    head;
  SeqEntryPtr   nsep;
  ProtRefPtr    prp;
  SeqEntryPtr   psep;
  Char          str [256];
  Char          text [256];
  ValNodePtr    ttl;
  ValNodePtr    vnp;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    if (bssp->_class == 7 ||
        (bssp->_class >= 13 && bssp->_class <= 16)) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        MakeProteinTitlesInSequinStyle (entityID, sep);
      }
      return;
    }
  }
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL) return;

  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.scope = sep;
  gs.target = NULL;
  head = NULL;
  GatherEntity (entityID, (Pointer) (&head), GetCDStRNArRNAGatherFunc, &gs);
  /* head = SortValNode (head, SortCDStRNArRNAByLocation); */
  if (head == NULL) return;

  vnp = head;
  while (vnp != NULL) {
    dfp = (DefFeatsPtr) vnp->data.ptrvalue;
    if (dfp != NULL && dfp->sfp != NULL && dfp->subtype == FEATDEF_CDS) {
      FindGeneAndProtForCDS (entityID, dfp->sfp, &(dfp->gene), &(dfp->prot));
      bsp = GetBioseqGivenSeqLoc (dfp->sfp->product, entityID);
      if (bsp != NULL) {
        str [0] = '\0';
        if (dfp->gene != NULL) {
          grp = (GeneRefPtr) dfp->gene->data.value.ptrvalue;
          if (grp != NULL) {
            StringNCpy_0 (text, (CharPtr) grp->locus, sizeof (text));
            if (! StringHasNoText (text)) {
              StringCat (str, "[gene=");
              StringCat (str, text);
              StringCat (str, "]");
            }
          }
        }
        if (dfp->prot != NULL) {
          prp = (ProtRefPtr) dfp->prot->data.value.ptrvalue;
          if (prp != NULL && prp->name != NULL) {
            StringNCpy_0 (text, (CharPtr) prp->name->data.ptrvalue, sizeof (text));
            if (! StringHasNoText (text)) {
              if (str [0] != '\0') {
                StringCat (str, " ");
              }
              StringCat (str, "[protein=");
              StringCat (str, text);
              StringCat (str, "]");
            }
          }
        }
        if (! StringHasNoText (str)) {
          psep = SeqMgrGetSeqEntryForData (bsp);
          if (psep != NULL) {
            ttl = CreateNewDescriptor (psep, Seq_descr_title);
            if (ttl != NULL) {
              ttl->data.ptrvalue = StringSave (str);
            }
          }
        }
      }
    }
    vnp = vnp->next;
  }
  ValNodeFreeData (head);
}

extern Int2 LIBCALLBACK MakeSequinProteinTitles (Pointer data)

{
  OMProcControlPtr  ompcp;
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  MakeProteinTitlesInSequinStyle (ompcp->input_entityID, sep);
  ObjMgrSetDirtyFlag (ompcp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, ompcp->input_entityID, 0, 0);
  return OM_MSG_RET_DONE;
}

static Boolean LIBCALLBACK SequinFTableBioseq (BioseqPtr bsp, SeqMgrBioseqContextPtr context)

{
  FILE      *fp;

  if (bsp == NULL) return TRUE;
  fp = (FILE *) context->userdata;
  BioseqToGnbk (bsp, NULL, FTABLE_FMT, DUMP_MODE, NORMAL_STYLE, 0, fp);
  return TRUE;
}

extern Int2 LIBCALLBACK MakeSequinFeatureTable (Pointer data)

{
  FILE              *fp;
  OMProcControlPtr  ompcp;
  Char              path [PATH_MAX];
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      break;
    case OBJ_BIOSEQSET :
      break;
    case 0 :
      return OM_MSG_RET_ERROR;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (ompcp->input_data == NULL) return OM_MSG_RET_ERROR;
  sep = GetTopSeqEntryForEntityID (ompcp->input_entityID);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp == NULL) return OM_MSG_RET_ERROR;
  SeqMgrExploreBioseqs (ompcp->input_entityID, NULL, (Pointer) fp, SequinFTableBioseq, TRUE, FALSE, FALSE);
  FileClose (fp);
  LaunchGeneralTextViewer (path, "Gene - CDS Feature Table");
  FileRemove (path);
  return OM_MSG_RET_DONE;
}

/* the following two functions are modified from PrintGenome in wprint.c */

static void SqLitPrintGenome(SeqLitPtr slp, FILE *fp)
{
	static Char		val[166];

	if (slp->seq_data != NULL)         /* not a gap */
	{
		if (slp->length == 0)  /* unknown length */
		{
			fprintf(fp, "gap\t0\t0\t0\n");
		} else {
/* don't know what to do here */
		}
	} else {                  /* gap length was set */
			fprintf(fp, "gap\t0\t0\t%ld\n", (long) slp->length);
	}
}

static void SqLocPrintGenome(SeqLocPtr slp_head, FILE *fp)
{
	SeqLocPtr	slp;
	static Char		buf[11];
	SeqIdPtr	sid, newid;
	Int4 		start, stop;
		
	for (slp = slp_head; slp; slp = slp->next) {
		sid = SeqLocId(slp);
		if (slp->choice == SEQLOC_INT || slp->choice == SEQLOC_WHOLE) {
			start = SeqLocStart(slp);
			stop = SeqLocStop(slp);
		} else if (slp->choice == SEQLOC_NULL){
			fprintf(fp, "gap\t0\t0\t0\n");
			continue;
		} else {
			continue;
		}
		if (sid->choice == SEQID_GI) {
			newid = GetSeqIdForGI(sid->data.intvalue);
		} else if (sid->choice == SEQID_GENERAL) {
			newid = sid;
		} else {
			newid = sid;
		}
		SeqIdWrite(newid, buf, PRINTID_TEXTID_ACCESSION, 10);
		fprintf(fp, "%s\t%ld\t%ld\n", buf, (long) start+1, (long) stop+1);
	}
}

static Boolean DeltaLitOnly (BioseqPtr bsp)

{
  ValNodePtr  vnp;

  if (bsp == NULL || bsp->repr != Seq_repr_delta) return FALSE;
  for (vnp = (ValNodePtr)(bsp->seq_ext); vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 1) return FALSE;
  }
  return TRUE;
}

extern Int2 LIBCALLBACK MakeContigBuildTable (Pointer data)

{
  BioseqPtr         bsp;
  DeltaSeqPtr       dsp;
  FILE              *fp;
  SeqLitPtr 	    litp;
  OMProcControlPtr  ompcp;
  Char              path [PATH_MAX];
  SeqEntryPtr       sep;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL) return OM_MSG_RET_ERROR;
  bsp = NULL;
  switch (ompcp->input_itemtype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    default :
      return OM_MSG_RET_ERROR;
  }
  if (bsp == NULL) return OM_MSG_RET_ERROR;
  sep = GetBestTopParentForData (ompcp->input_entityID, bsp);
  if (sep == NULL) return OM_MSG_RET_ERROR;
  if (IsAGenomeRecord (sep) ||
      IsSegmentedBioseqWithoutParts (sep)) {
  } else if (IsADeltaBioseq (sep) && (! DeltaLitOnly (bsp))) {
  } else return OM_MSG_RET_ERROR;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp == NULL) return OM_MSG_RET_ERROR;
/* the following code is modified from PrintGenome in wprint.c */
	if (bsp->seq_ext_type == 1) {
		SqLocPrintGenome((SeqLocPtr) bsp->seq_ext, fp);
	} else if (bsp->seq_ext_type == 4) {
		for (dsp = (DeltaSeqPtr) bsp->seq_ext; dsp; dsp=dsp->next) {
			if (dsp->choice == 1) {  /* SeqLoc */
				SqLocPrintGenome((SeqLocPtr)(dsp->data.ptrvalue), fp);
			} else {
				litp = (SeqLitPtr)(dsp->data.ptrvalue);
				if (litp == NULL) continue;
				SqLitPrintGenome(litp, fp);
			}
		}
	}
  FileClose (fp);
  LaunchGeneralTextViewer (path, "Contig Build Table");
  FileRemove (path);
  return OM_MSG_RET_DONE;
}

