/*   gbfview.c
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
* File Name:  gbfview.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   2/5/97
*
* $Revision: 6.73 $
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

#include <bspview.h>
#include <asn2gnbp.h>
#include <tofasta.h>
#include <explore.h>

static ParData ffParFmt = {FALSE, FALSE, FALSE, FALSE, TRUE, 0, 0};
static ColData ffColFmt = {0, 0, 80, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE};

typedef struct docdescrstruct {
  Uint2  entityID, itemID, itemtype;
  Int2   docitem;
} DocDescrStruct, PNTR DocDescrPtr;

typedef struct flatstruct {
  Asn2gbJobPtr       ajp;
  BaseBlockPtr PNTR  paragraphs;
  Int4               numParagraphs;
  SeqEntryPtr        sep;
  Int4               numdescr;
  DocDescrPtr        descr;
} FlatStruct, PNTR FlatStructPtr;

static Boolean GetIDsFromDoc (DoC d, Int2 item, Uint2Ptr entityPtr,
                              Uint2Ptr itemPtr, Uint2Ptr typePtr)

{
  BaseBlockPtr       bbp;
  BioseqViewPtr      bvp;
  Pointer            dataPtr;
  unsigned int       entityID;
  FlatStructPtr      fsp;
  unsigned int       itemID;
  unsigned int       itemtype;
  Boolean            okay;
  BaseBlockPtr PNTR  paragraphs;

  okay = FALSE;
  entityID = 0;
  itemID = 0;
  itemtype = 0;
  bvp = (BioseqViewPtr) GetObjectExtra (d);
  if (bvp != NULL) {
    if (! bvp->highlightSelections) return FALSE;
    if (bvp->useScrollText) return FALSE;
    GetItemParams (d, item, NULL, NULL, NULL, NULL, &dataPtr);
    if (dataPtr != NULL) {
      fsp = (FlatStructPtr) dataPtr;
      paragraphs = fsp->paragraphs;
      if (paragraphs != NULL) {
        bbp = paragraphs [item - 1];
        if (bbp != NULL) {
          entityID = bbp->entityID;
          itemID = bbp->itemID;
          itemtype = bbp->itemtype;
          okay = (Boolean) (entityID > 0 && itemID > 0 && itemtype > 0);
        }
      }
    }
  }
  if (entityPtr != NULL) {
    *entityPtr = entityID;
  }
  if (itemPtr != NULL) {
    *itemPtr = itemID;
  }
  if (typePtr != NULL) {
    *typePtr = itemtype;
  }
  return okay;
}

static void DrawIcon (DoC d, RectPtr r, Int2 item, Int2 firstLine)

{
  BioseqViewPtr  bvp;
  Uint2          entityID;
  Uint2          itemID;
  Uint2          itemtype;
  RecT           rct;
  SelStructPtr   sel;

  bvp = (BioseqViewPtr) GetObjectExtra (d);
  if (bvp == NULL) return;
  if (! bvp->highlightSelections) return;
  if (GetIDsFromDoc (d, item, &entityID, &itemID, &itemtype)) {
    for (sel = ObjMgrGetSelected (); sel != NULL; sel = sel->next) {
      if (entityID == sel->entityID &&
          itemID == sel->itemID &&
          itemtype == sel->itemtype) {
        rct = *r;
        rct.right = rct.left + 4;
        PaintRect (&rct);
      }
    }
  }
}

static void ClickIcon (DoC d, PoinT pt)

{
  BioseqViewPtr  bvp;

  bvp = (BioseqViewPtr) GetObjectExtra (d);
  if (bvp == NULL) return;
  bvp->wasDoubleClick = dblClick;
  bvp->wasShiftKey = shftKey;
  MapDocPoint (d, pt, &(bvp->itemClicked), NULL, NULL, NULL);
}

typedef struct matchstruc {
  CharPtr   str;
  Uint2     editItemID;
  Boolean   found;
  Boolean   slashgene;
  Boolean   slashproduct;
} MatchStruc, PNTR MatchStrucPtr;

static void LIBCALLBACK FindStringCallBack (AsnExpOptStructPtr pAEOS)

{
  MatchStrucPtr  msp;
  CharPtr        pchSource;

  if (pAEOS == NULL) return;
  msp = (MatchStrucPtr) pAEOS->data;
  if (msp == NULL) return;
  if (! ISA_STRINGTYPE (AsnFindBaseIsa (pAEOS->atp))) return;
  pchSource = (CharPtr) pAEOS->dvp->ptrvalue;
  if (StringSearch (pchSource, msp->str) == NULL) return;
  msp->found = TRUE;
}

static void CheckForStringInFeature (SeqFeatPtr sfp, MatchStrucPtr msp)

{
  AsnExpOptPtr  aeop;
  AsnIoPtr      aip;
  SeqLocPtr     location;
  SeqLocPtr     product;
  ValNode       vn;

  if (sfp == NULL || msp == NULL) return;
  msp->found = FALSE;
  aip = AsnIoNullOpen ();
  if (aip != NULL) {
    aeop = AsnExpOptNew (aip, NULL, NULL, FindStringCallBack);
    if (aeop != NULL) {
      aeop->user_data = msp;
      location = sfp->location;
      product = sfp->product;
      vn.choice = SEQLOC_NULL;
      vn.data.ptrvalue = NULL;
      sfp->location = &vn;
      sfp->product = &vn;
      SeqFeatAsnWrite (sfp, aip, NULL);
      sfp->location = location;
      sfp->product = product;
    }
    AsnIoClose (aip);
  }
}

typedef struct protgenegatherlist {
  SeqLocPtr  slp;
  Uint2      choice;
  Int4       min;
  Uint2      entityID;
  Uint2      itemID;
  Uint2      itemtype;
  Boolean    found;
} ProtGeneGatherList, PNTR ProtGeneGatherPtr;

static Boolean ProtGeneMatchFunc (GatherContextPtr gcp)

{
  Int4               diff;
  ProtGeneGatherPtr  pgp;
  SeqFeatPtr         sfp;

  if (gcp == NULL) return TRUE;

  pgp = (ProtGeneGatherPtr) gcp->userdata;
  if (pgp == NULL) return TRUE;

  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != pgp->choice ||
      sfp->data.value.ptrvalue == NULL) return TRUE;
  if (pgp->choice == SEQFEAT_PROT) {
    diff = SeqLocAinB (sfp->location, pgp->slp);
  } else {
    diff = SeqLocAinB (pgp->slp, sfp->location);
  }
  if (diff < 0) return TRUE;
  if (diff >= pgp->min) return TRUE;
  pgp->min = diff;
  pgp->entityID = gcp->entityID;
  pgp->itemID = gcp->itemID;
  pgp->itemtype = gcp->thistype;
  pgp->found = TRUE;

  return TRUE;
}

static Uint2 GetBestGeneOrProteinFeature (Uint2 entityID, SeqEntryPtr scope,
                                          SeqLocPtr location, Uint2 choice)

{
  GatherScope         gs;
  ProtGeneGatherList  pgl;

  if (entityID == 0 || location == NULL) return 0;
  pgl.entityID = 0;
  pgl.itemID = 0;
  pgl.itemtype = 0;
  pgl.found = FALSE;
  pgl.slp = location;
  pgl.choice = choice;
  pgl.min = INT4_MAX;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  gs.scope = scope;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (entityID, (Pointer) &pgl, ProtGeneMatchFunc, &gs);
  return pgl.itemID;
}

static Boolean MatchSubItemInFlatFileProc (GatherContextPtr gcp)

{
  MatchStrucPtr  msp;
  SeqFeatPtr     sfp;

  if (gcp == NULL || gcp->thisitem == NULL || gcp->thistype != OBJ_SEQFEAT) {
    return FALSE;
  }
  msp = (MatchStrucPtr) gcp->userdata;
  if (msp == NULL) return FALSE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  CheckForStringInFeature (sfp, msp);
  return FALSE;
}

static Boolean MatchItemInFlatFileProc (GatherContextPtr gcp)

{
  Uint2          itemID;
  MatchStrucPtr  msp;
  SeqFeatPtr     sfp;

  if (gcp == NULL || gcp->thisitem == NULL || gcp->thistype != OBJ_SEQFEAT) {
    return FALSE;
  }
  msp = (MatchStrucPtr) gcp->userdata;
  if (msp == NULL) return FALSE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  /*
  if (msp->slashgene) {
    itemID = GetBestGeneOrProteinFeature (gcp->entityID, NULL, sfp->location, SEQFEAT_GENE);
    if (itemID != 0) {
      GatherItem (gcp->entityID, itemID, OBJ_SEQFEAT, (Pointer) msp, MatchSubItemInFlatFileProc);
      if (msp->found) {
        msp->editItemID = itemID;
        return TRUE;
      }
    }
  }
  CheckForStringInFeature (sfp, msp);
  if (msp->found) {
    return TRUE;
  }
  */
  if (sfp->data.choice == SEQFEAT_CDREGION) {
    itemID = GetBestGeneOrProteinFeature (gcp->entityID, NULL, sfp->product, SEQFEAT_PROT);
    if (itemID != 0) {
      GatherItem (gcp->entityID, itemID, OBJ_SEQFEAT, (Pointer) msp, MatchSubItemInFlatFileProc);
      if (msp->found) {
        msp->editItemID = itemID;
        return TRUE;
      }
    }
  }
  /*
  if (! msp->slashgene) {
    itemID = GetBestGeneOrProteinFeature (gcp->entityID, NULL, sfp->location, SEQFEAT_GENE);
    if (itemID != 0) {
      GatherItem (gcp->entityID, itemID, OBJ_SEQFEAT, (Pointer) msp, MatchSubItemInFlatFileProc);
      if (msp->found) {
        msp->editItemID = itemID;
        return TRUE;
      }
    }
  }
  */
  return FALSE;
}

static Uint2 MatchItemInFlatFile (Uint2 entityID, Uint2 itemID, CharPtr str,
                                  Boolean slashgene, Boolean slashproduct)

{
  MatchStruc  ms;

  ms.editItemID = itemID;
  ms.str = str;
  ms.slashgene = slashgene;
  ms.slashproduct = slashproduct;
  GatherItem (entityID, itemID, OBJ_SEQFEAT, (Pointer) &ms, MatchItemInFlatFileProc);
  return ms.editItemID;
}

static void ReleaseIcon (DoC d, PoinT pt)

{
  BioseqViewPtr  bvp;
  Char           ch;
  CharPtr        dst;
  Uint2          editItemID;
  Uint2          entityID;
  Boolean        handled;
  Int2           item;
  Uint2          itemID;
  Uint2          itemtype;
  Int2           row;
  SeqEntryPtr    sep;
  Boolean        slashgene;
  Boolean        slashproduct;
  CharPtr        src;
  CharPtr        str;

  bvp = (BioseqViewPtr) GetObjectExtra (d);
  if (bvp == NULL) return;
  MapDocPoint (d, pt, &item, &row, NULL, NULL);
  if (row != 0 && item != 0 && item == bvp->itemClicked) {
    if (GetIDsFromDoc (d, item, &entityID, &itemID, &itemtype)) {
      if (bvp->wasDoubleClick) {
        sep = GetTopSeqEntryForEntityID (entityID);
        if (bvp->launchSubviewers) {
          WatchCursor ();
          Update ();
          LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
          ArrowCursor ();
          Update ();
          return;
        } else if (LaunchViewerNotEditor (bvp, sep, entityID, itemID, itemtype)) {
          WatchCursor ();
          Update ();
          LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
          ArrowCursor ();
          Update ();
          return;
        } else if (bvp->launchEditors) {
          WatchCursor ();
          Update ();
          editItemID = itemID;
          if (itemtype == OBJ_SEQFEAT && GetAppProperty ("InternalNcbiSequin") != NULL) {
            str = GetDocText (d, item, row, 1);
            TrimSpacesAroundString (str);
            src = str;
            dst = str;
            ch = *src;
            slashgene = FALSE;
            slashproduct = FALSE;
            if (str != NULL && str [0] == '/') {
              if (StringStr (str, "/gene") != NULL) {
                slashgene = TRUE;
              }
              if (StringStr (str, "/product") != NULL) {
                slashproduct = TRUE;
              }
              while (ch != '=' && ch != '\0') {
                src++;
                ch = *src;
              }
              if (ch == '=') {
                src++;
                ch = *src;
              }
            }
            while (ch != '\0') {
              if (ch != '"') {
                *dst = ch;
                dst++;
              }
              src++;
              ch = *src;
            }
            *dst = '\0';
            editItemID = MatchItemInFlatFile (entityID, itemID, str, slashgene, slashproduct);
            MemFree (str);
          }
          handled = GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, editItemID,
                                      itemtype, 0, 0, itemtype, 0);
          ArrowCursor ();
          Update ();
          if (handled == OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
            return;
          }
        } else {
          return;
        }
      }
      if (! bvp->sendSelectMessages) return;
      if (bvp->wasShiftKey) {
        ObjMgrAlsoSelect (entityID, itemID, itemtype,0,NULL);
      } else {
        ObjMgrSelect (entityID, itemID, itemtype,0,NULL);
      }
    }
  } else if (item == bvp->itemClicked) {
    if (! bvp->sendSelectMessages) return;
    ObjMgrDeSelect (0, 0, 0,0,NULL);
  }
}

static void DocFreeFlat (DoC d, VoidPtr data)

{
  FlatStructPtr  fsp;

  if (data != NULL) {
    fsp = (FlatStructPtr) data;
    fsp->ajp = asn2gnbk_cleanup (fsp->ajp);
    MemFree (fsp->descr);
    MemFree (fsp);
  }
}

static CharPtr FFPrintFunc (DoC d, Int2 index, Pointer data)

{
  BaseBlockPtr       bbp;
  BaseFormPtr        bfp;
  BioseqPtr          bsp;
  BioseqViewPtr      bvp;
  FlatStructPtr      fsp;
  ErrSev             level;
  SeqEntryPtr        oldsep = NULL;
  OMUserDataPtr      omudp;
  BaseBlockPtr PNTR  paragraphs;
  CharPtr            str = NULL;
  SeqEntryPtr        topsep;

  bvp = (BioseqViewPtr) GetObjectExtra (d);
  fsp = (FlatStructPtr) data;
  if (bvp == NULL || bvp->bsp == NULL || fsp == NULL) return StringSave ("?\n");
  level = ErrSetMessageLevel (SEV_MAX);
  bsp = BioseqLock (bvp->bsp);
  bfp = (BaseFormPtr) GetObjectExtra (bvp->form);
  if (bfp != NULL) {
    topsep = GetTopSeqEntryForEntityID (bfp->input_entityID);
    oldsep = SeqEntrySetScope (topsep);
  }
  str = asn2gnbk_format (fsp->ajp, (Int4) (index - 1));
  if (oldsep != NULL) {
    SeqEntrySetScope (oldsep);
  }
  if (bfp != NULL) {
    paragraphs = fsp->paragraphs;
    if (paragraphs != NULL) {
      bbp = paragraphs [index - 1];
      if (bbp != NULL && bbp->entityID != 0 && bbp->entityID != bfp->input_entityID) {
        if (! InBioseqViewEntityList (bbp->entityID, bvp)) {
          ValNodeAddInt (&(bvp->entityList), 0, (Int4) bbp->entityID);
          omudp = ObjMgrAddUserData (bbp->entityID, bfp->procid,
	                                 OMPROC_VIEW, bfp->userkey);
          if (omudp != NULL) {
            omudp->userdata.ptrvalue = (Pointer) bfp;
            omudp->messagefunc = BioseqViewMsgFunc;
          }
        }
      }
    }
  }
  BioseqUnlock (bsp);
  ErrSetMessageLevel (level);
  return str;
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

static int LIBCALLBACK SortDescrProc (VoidPtr vp1, VoidPtr vp2)

{
  DocDescrPtr  descr1;
  DocDescrPtr  descr2;

  descr1 = (DocDescrPtr) vp1;
  descr2 = (DocDescrPtr) vp2;
  if (descr1 == NULL || descr2 == NULL) return 0;

  if (descr1->entityID > descr2->entityID) return 1;
  if (descr1->entityID < descr2->entityID) return -1;

  if (descr1->itemtype > descr2->itemtype) return 1;
  if (descr1->itemtype < descr2->itemtype) return -1;

  if (descr1->itemID > descr2->itemID) return 1;
  if (descr1->itemID < descr2->itemID) return -1;

  if (descr1->docitem > descr2->docitem) return 1;
  if (descr1->docitem < descr2->docitem) return -1;

  return 0;
}

typedef struct lookforids {
  Boolean isGED;
  Boolean isNTorNW;
  Boolean isNC;
  Boolean isTPA;
  Boolean isAEorCH;
} LookForIDs, PNTR LookForIDsPtr;

static void LookForSeqIDs (BioseqPtr bsp, Pointer userdata)

{
  LookForIDsPtr  lfip;
  SeqIdPtr       sip;
  TextSeqIdPtr   tsip;

  lfip = (LookForIDsPtr) userdata;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_DDBJ :
        lfip->isGED = TRUE;
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL) {
          if (StringNCmp (tsip->accession, "AE", 2) == 0) {
            lfip->isAEorCH = TRUE;
          } else if (StringNCmp (tsip->accession, "CH", 2) == 0) {
            lfip->isAEorCH = TRUE;
          }
        }
        break;
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
        lfip->isTPA = TRUE;
        break;
      case SEQID_OTHER :
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL) {
          if (StringNCmp (tsip->accession, "NC_", 3) == 0) {
            lfip->isNC = TRUE;
          } else if (StringNCmp (tsip->accession, "NT_", 3) == 0) {
            lfip->isNTorNW = TRUE;
          } else if (StringNCmp (tsip->accession, "NW_", 3) == 0) {
            lfip->isNTorNW = TRUE;
          }
        }
        break;
      default :
        break;
    }
  }
}

static void LookForGEDetc (
  SeqEntryPtr topsep,
  BoolPtr isGED,
  BoolPtr isNTorNW,
  BoolPtr isNC,
  BoolPtr isTPA,
  BoolPtr isAEorCH
)

{
  LookForIDs  lfi;

  MemSet ((Pointer) &lfi, 0, sizeof (LookForIDs));
  VisitBioseqsInSep (topsep, (Pointer) &lfi, LookForSeqIDs);
  *isGED = lfi.isGED;
  *isNTorNW = lfi.isNTorNW;
  *isNC = lfi.isNC;
  *isTPA = lfi.isTPA;
  *isAEorCH = lfi.isAEorCH;
}

static void LookForNonLocalID (BioseqPtr bsp, Pointer userdata)

{
  BoolPtr   isNonLocalPtr;
  SeqIdPtr  sip;

  isNonLocalPtr = (BoolPtr) userdata;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_LOCAL :
        break;
      default :
        *isNonLocalPtr = TRUE;
        break;
    }
  }
}

static void LookForPubs (BioseqPtr bsp, Pointer userdata)

{
  BoolPtr  hasPub;

  hasPub = (BoolPtr) userdata;
  *hasPub = TRUE;
}

static void CheckVersionWithGi (BioseqPtr bsp, Pointer userdata)

{
  Boolean       hasGi = FALSE;
  BoolPtr       missingVersion;
  SeqIdPtr      sip;
  TextSeqIdPtr  tsip;
  Boolean       zeroVersion = FALSE;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_TPG:
      case SEQID_TPE:
      case SEQID_TPD:
      case SEQID_GENBANK:
      case SEQID_EMBL:
      case SEQID_DDBJ:
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL && tsip->version == 0) {
          zeroVersion = TRUE;
        }
        break;
      case SEQID_GI :
        hasGi = TRUE;
        break;
      default :
        break;
    }
  }
  if (hasGi && zeroVersion) {
    missingVersion = (BoolPtr) userdata;
    *missingVersion = TRUE;
  }
}

static CharPtr relmodemsg1 = "Record cannot only have a local Seq-id for release mode";
static CharPtr relmodemsg2 = "Record must have a publication for release mode";
static CharPtr relmodemsg3 = "Record with gi must have version number for release mode";
static CharPtr relmodemsg4 = "Release mode failure";

static CharPtr RelModeFailText (
  BioseqPtr bsp,
  SeqEntryPtr usethetop,
  SeqEntryPtr topsep
)

{
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  Boolean            missingVersion;
  Boolean            nonLocalID;
  Boolean            hasPubs;

  nonLocalID = FALSE;
  if (usethetop) {
    VisitBioseqsInSep (usethetop, (Pointer) &nonLocalID, LookForNonLocalID);
  } else {
    LookForNonLocalID (bsp, (Pointer) &nonLocalID);
  }
  if (! nonLocalID) {
    return StringSave (relmodemsg1);
  }

  hasPubs = FALSE;
  if (usethetop) {
    if (VisitPubdescsInSep (usethetop, NULL, NULL) == 0) {
      return StringSave (relmodemsg2);
    }
  } else {
    if (SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pub, &dcontext) == NULL) {
      if (SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_PUB, 0, &fcontext) == NULL) {
        return StringSave (relmodemsg2);
      }
    }
  }

  missingVersion = FALSE;
  VisitBioseqsInSep (topsep, (Pointer) &missingVersion, CheckVersionWithGi);
  if (missingVersion) {
    return StringSave (relmodemsg3);
  }

  return StringSave (relmodemsg4);
}

static Int2 asn2gb_line_estimate [27] = {
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2, /* organism */
  6, /* reference */
  4, /* primary */
  4, /* comment */
  1,
  4, /* source */
  6, /* feature */
  1,
  1,
 20, /* sequence */
  4, /* contig */
  1,
  1,
  1,
  1
};

static Boolean PopulateFF (
  DoC d,
  SeqEntryPtr sep,
  BioseqPtr bsp,
  SeqEntryPtr usethetop,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags,
  CstType custom
)

{
  Asn2gbJobPtr    ajp;
  BaseBlockPtr    bbp;
  BlockType       blocktype;
  BioseqSetPtr    bssp;
  BioseqViewPtr   bvp;
  DocDescrPtr     doscr;
  DocDescrPtr     doscrp;
  Int2            estimate;
  FonT            fnt;
  FlatStructPtr   fsp;
  Int4            index;
  ErrSev          level;
  Boolean         rsult;

  rsult = FALSE;
  if (d != NULL && sep != NULL && bsp != NULL && spop != NULL) {
    fnt = programFont;
    bvp = (BioseqViewPtr) GetObjectExtra (d);
    if (bvp != NULL && bvp->displayFont != NULL) {
      fnt = bvp->displayFont;
    }
    fsp = MemNew (sizeof (FlatStruct));
    if (fsp != NULL) {
      fsp->sep = sep;
      SetDocData (d, (Pointer) fsp, DocFreeFlat);
      level = ErrSetMessageLevel (SEV_MAX);
      if (usethetop != NULL && IS_Bioseq_set (usethetop)) {
        bssp = (BioseqSetPtr) usethetop->data.ptrvalue;
        ajp = asn2gnbk_setup (NULL, bssp, NULL, format, mode, style, flags, 0, custom, NULL);
      } else {
        ajp = asn2gnbk_setup (bsp, NULL, NULL, format, mode, style, flags, 0, custom, NULL);
      }
      if (ajp == NULL) return FALSE;
      fsp->ajp = ajp;
      fsp->numParagraphs = ajp->numParagraphs;
      fsp->paragraphs = ajp->paragraphArray;
      if (fsp->numParagraphs > 0 && fsp->paragraphs != NULL) {
        fsp->numdescr = 0;
        for (index = 0; index < fsp->numParagraphs; index++) {
          estimate = 1;
          bbp = fsp->paragraphs [index];
          if (bbp != NULL) {
            blocktype = bbp->blocktype;
            if (blocktype >= HEAD_BLOCK && blocktype <= TAIL_BLOCK) {
              estimate = asn2gb_line_estimate [(int) blocktype];
            }
          }
          AppendItem (d, FFPrintFunc, (Pointer) fsp, FALSE,
                      estimate, &ffParFmt, &ffColFmt, fnt);
          if (bbp != NULL) {
            (fsp->numdescr)++;
          }
        }
        doscrp = (DocDescrPtr) MemNew (sizeof (DocDescrStruct) * (size_t) (fsp->numdescr + 1));
        fsp->descr = doscrp;
        fsp->numdescr = 0;
        if (doscrp != NULL) {
          for (index = 0; index < fsp->numParagraphs; index++) {
            bbp = fsp->paragraphs [index];
            if (bbp != NULL) {
              doscr = &(doscrp [fsp->numdescr]);
              doscr->entityID = bbp->entityID;
              doscr->itemID = bbp->itemID;
              doscr->itemtype = bbp->itemtype;
              doscr->docitem = index + 1;
              (fsp->numdescr)++;
            }
          }
        }
        HeapSort (doscrp, (size_t) fsp->numdescr, sizeof (DocDescrStruct), SortDescrProc);
        rsult = TRUE;
      }
      ErrSetMessageLevel (level);
    }
  }
  return rsult;
}

static void LookForTpa (
  SeqDescrPtr sdp,
  Pointer userdata
)

{
  BoolPtr        hastpaP;
  ObjectIdPtr    oip;
  UserObjectPtr  uop;

  if (sdp == NULL || sdp->choice != Seq_descr_user) return;
  uop = (UserObjectPtr) sdp->data.ptrvalue;
  if (uop == NULL) return;

  oip = uop->type;
  if (oip == NULL) return;
  if (StringCmp (oip->str, "TpaAssembly") != 0) return;

  hastpaP = (BoolPtr) userdata;
  *hastpaP = TRUE;
}

static void PopulateFlatFile (BioseqViewPtr bvp, FmtType format, FlgType flags)

{
  BioseqPtr        bsp;
  CstType          custom = 0;
  DoC              doc;
  Boolean          doLockFarComponents = FALSE;
  Uint2            entityID;
  FonT             fnt;
  FILE             *fp;
  Boolean          hastpaaligns;
  Int2             into;
  Boolean          isAEorCH;
  Boolean          isGED;
  Boolean          isNTorNW;
  Boolean          isNC;
  Boolean          isTPA;
  Int2             item;
  ErrSev           level;
  Boolean          lockFar = FALSE;
  Boolean          lookupFar = FALSE;
  ModType          mode = SEQUIN_MODE;
  SeqEntryPtr      oldsep;
  Char             path [PATH_MAX];
  BaR              sb = NULL;
  SeqEntryPtr      sep;
  Int4             startsAt;
  CharPtr          str;
  StlType          style = NORMAL_STYLE;
  SeqViewProcsPtr  svpp;
  SeqEntryPtr      topsep;
  TexT             txt;
  SeqEntryPtr      usethetop = NULL;
  Int2             val;

  if (bvp == NULL) return;
  if (bvp->hasTargetControl && bvp->ffModeCtrl != NULL) {
    val = GetValue (bvp->ffModeCtrl);
    switch (val) {
      case 1 :
        mode = RELEASE_MODE;
        break;
      case 2 :
        mode = ENTREZ_MODE;
        break;
      case 3 :
        mode = SEQUIN_MODE;
        break;
      case 4 :
        mode = DUMP_MODE;
        break;
      default :
        break;
    }
  }
  /* now using control instead of seqid type */
  if (bvp->ffStyleCtrl != NULL) {
    val = GetValue (bvp->ffStyleCtrl);
    switch (val) {
      case 1 :
        style = NORMAL_STYLE;
        lookupFar = TRUE;
        break;
      case 2 :
        style = SEGMENT_STYLE;
        lockFar = TRUE;
        break;
      case 3 :
        style = MASTER_STYLE;
        lockFar = TRUE;
        break;
      case 4 :
        style = CONTIG_STYLE;
        lookupFar = TRUE;
        break;
      default :
        break;
    }
  }

  bsp = bvp->bsp;
  entityID = ObjMgrGetEntityIDForPointer (bsp);
  topsep = GetTopSeqEntryForEntityID (entityID);
  LookForGEDetc (topsep, &isGED, &isNTorNW, &isNC, &isTPA, &isAEorCH);

  if ((flags & SHOW_CONTIG_FEATURES) != 0 || (flags & SHOW_CONTIG_SOURCES) != 0) {
    if (isNTorNW || isTPA) {
      lockFar = FALSE;
      lookupFar = TRUE;
      if (GetAppProperty ("InternalNcbiSequin") != NULL) {
        doLockFarComponents = TRUE;
      }
    } else {
      lockFar = TRUE;
    }
  }
  if (bvp->hasTargetControl && bvp->ffCustomBtn != NULL) {
    if (GetStatus (bvp->ffCustomBtn)) {
      custom |= SHOW_TRANCRIPTION | SHOW_PEPTIDE;
    }
  }
  doc = NULL;
  txt = NULL;
  bsp = bvp->bsp;
  if (bvp->useScrollText) {
    txt = bvp->text;
    Reset (txt);
    Update ();
  } else {
    doc = bvp->doc;
    GetScrlParams (doc, NULL, &item, &into);
    sb = GetSlateVScrollBar ((SlatE) doc);
    Reset (doc);
    SetDocShade (doc, NULL, NULL, NULL, NULL);
    SetDocProcs (doc, NULL, NULL, NULL, NULL);
    SetDocCache (doc, NULL, NULL, NULL);
    Update ();
    SetDocAutoAdjust (doc, FALSE);
  }
  if (bsp == NULL) return;
  if (spop == NULL) {
    spop = StdPrintOptionsNew (NULL);
    if (spop != NULL) {
      spop->newline = "\r";
      spop->indent = "";
    } else {
      Message (MSG_ERROR, "StdPrintOptionsNew failed");
    }
  }

  if (GetAppProperty ("OldFlatfileSource") != NULL) {
    flags |= USE_OLD_SOURCE_ORG;
  }

  svpp = (SeqViewProcsPtr) GetAppProperty ("SeqDisplayForm");
  if (svpp != NULL) {
    if (svpp->lockFarComponents) {
      doLockFarComponents = TRUE;
    }
  }
  if (doLockFarComponents) {
    entityID = ObjMgrGetEntityIDForPointer (bsp);
    sep = GetTopSeqEntryForEntityID (entityID);
    if (bvp->bsplist == NULL && lockFar) {
      bvp->bsplist = LockFarComponentsEx (sep, TRUE, FALSE, FALSE, NULL);
    }
    if (lookupFar) {
      hastpaaligns = FALSE;
      VisitDescriptorsInSep (sep, (Pointer) &hastpaaligns, LookForTpa);
      LookupFarSeqIDs (sep, TRUE, TRUE, TRUE, FALSE, hastpaaligns);
    }
  }

  sep = SeqMgrGetSeqEntryForData (bsp);
  entityID = ObjMgrGetEntityIDForChoice (sep);
  if (bvp->hasTargetControl) {
    if (bvp->viewWholeEntity) {
      sep = GetTopSeqEntryForEntityID (entityID);
      usethetop = sep;
      if (format == FTABLE_FMT) {
        custom |= SHOW_PROT_FTABLE;
      }
    } else if (ISA_na (bsp->mol) && bsp->repr == Seq_repr_seg) {
      sep = GetBestTopParentForData (entityID, bsp);
    } else if (ISA_aa (bsp->mol) && bsp->repr == Seq_repr_seg) {
      sep = GetBestTopParentForData (entityID, bsp);
    }
  } else {
    if (ISA_na (bsp->mol) || bsp->repr == Seq_repr_seg) {
      sep = GetBestTopParentForData (entityID, bsp);
    }
  }
  if (sep == NULL) return;

  topsep = GetTopSeqEntryForEntityID (entityID);
  oldsep = SeqEntrySetScope (topsep);

  WatchCursor ();
  ffColFmt.pixWidth = screenRect.right - screenRect.left;
  ffColFmt.pixInset = 8;
  /* LookForGEDetc (topsep, &isGED, &isNTorNW, &isNC, &isTPA); */
  if ((flags & SHOW_CONTIG_FEATURES) != 0 || (flags & SHOW_CONTIG_SOURCES) != 0) {
    if (isNTorNW || isTPA) {
      flags |= ONLY_NEAR_FEATURES;
    } else if (isNC) {
      flags |= NEAR_FEATURES_SUPPRESS;
    } else if (isAEorCH) {
      flags |= NEAR_FEATURES_SUPPRESS;
    }
  }
  if (bvp->useScrollText) {
    TmpNam (path);
    fp = FileOpen (path, "w");
    if (fp != NULL) {
      level = ErrSetMessageLevel (SEV_MAX);
      if (SeqEntryToGnbk (sep, NULL, format, mode, style, flags, 0, custom, NULL, fp)) {
        FileClose (fp);
        if (! FileToScrollText (txt, path)) {
          SetTitle (txt, "(Text is too large to be displayed in this control.)");
        }
      } else if (mode == RELEASE_MODE) {
        str = RelModeFailText (bsp, usethetop, topsep);
        if (str != NULL) {
          fprintf (fp, "%s", str);
        }
        FileClose (fp);
        if (str != NULL) {
          FileToScrollText (txt, path);
        }
        MemFree (str);
      } else {
        FileClose (fp);
      }
      ErrSetMessageLevel (level);
    }
    FileRemove (path);
  } else {
    if (PopulateFF (doc, sep, bsp, usethetop, format, mode, style, flags, custom)) {
      SetDocShade (doc, DrawIcon, NULL, NULL, NULL);
      SetDocProcs (doc, ClickIcon, NULL, ReleaseIcon, NULL);
      SetDocCache (doc, StdPutDocCache, StdGetDocCache, StdResetDocCache);
      SetDocAutoAdjust (doc, FALSE);
      ForceFormat (doc, item);
      SetDocAutoAdjust (doc, TRUE);
      AdjustDocScroll (doc);
      GetItemParams4 (doc, item, &startsAt, NULL, NULL, NULL, NULL);
      CorrectBarValue (sb, startsAt + into);
      UpdateDocument (doc, 0, 0);
    } else if (mode == RELEASE_MODE) {
      str = RelModeFailText (bsp, usethetop, topsep);
      if (str != NULL) {
        fnt = programFont;
        if (bvp != NULL && bvp->displayFont != NULL) {
          fnt = bvp->displayFont;
        }
        AppendText (doc, str, &ffParFmt, &ffColFmt, fnt);
      }
      MemFree (str);
      UpdateDocument (doc, 0, 0);
    }
  }

  SeqEntrySetScope (oldsep);

  ArrowCursor ();
  Update ();
}

static void PopulateGenBank (BioseqViewPtr bvp)

{
  PopulateFlatFile (bvp, GENBANK_FMT, SHOW_CONTIG_FEATURES | SHOW_CONTIG_SOURCES | SHOW_FAR_TRANSLATION);
}

static void PopulateEMBL (BioseqViewPtr bvp)

{
  if (bvp == NULL) return;
  if (bvp->hasTargetControl) {
    PopulateFlatFile (bvp, EMBL_FMT, 0);
  } else {
    PopulateFlatFile (bvp, EMBL_FMT, 0);
  }
}

static void PopulateDDBJ (BioseqViewPtr bvp)

{
  PopulateFlatFile (bvp, GENBANK_FMT, DDBJ_VARIANT_FORMAT);
}

static void PopulateGenPept (BioseqViewPtr bvp)

{
  PopulateFlatFile (bvp, GENPEPT_FMT, 0);
}

static void PopulateFTable (BioseqViewPtr bvp)

{
  PopulateFlatFile (bvp, FTABLE_FMT, 0);
}

static void PopulateFasta (BioseqViewPtr bvp)

{
  BioseqPtr        bsp;
  DoC              doc;
  Uint2            entityID;
  Boolean          fastaOK;
  Boolean          fastaNucOK;
  Boolean          fastaPrtOK;
  FonT             fnt;
  FILE             *fp;
  Uint1            group_segs;
  Int2             into;
  Int2             item;
  Char             path [PATH_MAX];
  BaR              sb = NULL;
  SeqEntryPtr      sep;
  Int4             startsAt;
  SeqViewProcsPtr  svpp;
  TexT             txt;

  if (bvp == NULL) return;
  doc = NULL;
  txt = NULL;
  bsp = bvp->bsp;
  if (bvp->useScrollText) {
    txt = bvp->text;
    Reset (txt);
    Update ();
  } else {
    doc = bvp->doc;
    GetScrlParams (doc, NULL, &item, &into);
    sb = GetSlateVScrollBar ((SlatE) doc);
    Reset (doc);
    SetDocShade (doc, NULL, NULL, NULL, NULL);
    SetDocProcs (doc, NULL, NULL, NULL, NULL);
    SetDocCache (doc, NULL, NULL, NULL);
    Update ();
    SetDocAutoAdjust (doc, FALSE);
  }
  if (bsp == NULL) return;

  svpp = (SeqViewProcsPtr) GetAppProperty ("SeqDisplayForm");
  if (svpp != NULL && svpp->lockFarComponents) {
    entityID = ObjMgrGetEntityIDForPointer (bsp);
    sep = GetTopSeqEntryForEntityID (entityID);
    if (bvp->bsplist == NULL) {
      bvp->bsplist = LockFarComponentsEx (sep, TRUE, FALSE, FALSE, NULL);
    }
  }

  sep = SeqMgrGetSeqEntryForData (bsp);
  if (bvp->hasTargetControl) {
    if (bvp->viewWholeEntity) {
      entityID = ObjMgrGetEntityIDForChoice (sep);
      sep = GetTopSeqEntryForEntityID (entityID);
    }
  } else {
    if (ISA_na (bsp->mol) || bsp->repr == Seq_repr_seg) {
      entityID = ObjMgrGetEntityIDForChoice (sep);
      sep = GetBestTopParentForData (entityID, bsp);
    }
  }
  if (sep == NULL) return;

  WatchCursor ();
  ffColFmt.pixWidth = screenRect.right - screenRect.left;
  ffColFmt.pixInset = 8;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    fnt = programFont;
    if (bvp != NULL && bvp->displayFont != NULL) {
      fnt = bvp->displayFont;
    }
    fastaOK = FALSE;
    if (ISA_na (bsp->mol)) {
      group_segs = 0;
      if (bvp->hasTargetControl) {
        if (bvp->viewWholeEntity) {
          if (bsp->repr == Seq_repr_seg) {
            group_segs = 2;
          }
          fastaNucOK = SeqEntrysToFasta (sep, fp, TRUE, group_segs);
          fastaPrtOK = SeqEntrysToFasta (sep, fp, FALSE, 0);
          fastaOK = fastaNucOK || fastaPrtOK;
        } else {
          if (bsp->repr == Seq_repr_seg) {
            group_segs = 1;
          } else if (bsp->repr == Seq_repr_delta) {
            group_segs = 3;
          }
          fastaOK = SeqEntrysToFasta (sep, fp, TRUE, group_segs);
        }
      } else {
        if (bsp->repr == Seq_repr_seg) {
          group_segs = 1;
        } else if (bsp->repr == Seq_repr_delta) {
          group_segs = 3;
        }
        fastaOK = SeqEntrysToFasta (sep, fp, TRUE, group_segs);
      }
    } else if (ISA_aa (bsp->mol)) {
      fastaOK = SeqEntrysToFasta (sep, fp, FALSE, 0);
    }
    if (fastaOK) {
      FileClose (fp);
      if (bvp->useScrollText) {
        if (! FileToScrollText (txt, path)) {
          SetTitle (txt, "(Text is too large to be displayed in this control.)");
        }
      } else {
        DisplayFancy (doc, path, &ffParFmt, &ffColFmt, fnt, 4);
        SetDocCache (doc, StdPutDocCache, StdGetDocCache, StdResetDocCache);
        SetDocAutoAdjust (doc, FALSE);
        ForceFormat (doc, item);
        SetDocAutoAdjust (doc, TRUE);
        AdjustDocScroll (doc);
        GetItemParams4 (doc, item, &startsAt, NULL, NULL, NULL, NULL);
        CorrectBarValue (sb, startsAt + into);
        UpdateDocument (doc, 0, 0);
      }
    } else {
      FileClose (fp);
    }
  }
  FileRemove (path);
  ArrowCursor ();
  Update ();
}

static void PrintQualProc (CharPtr buf, Uint4 buflen, Pointer userdata)

{
  FILE  *fp;

  fp = (FILE*) userdata;
  fprintf (fp, "%s", buf);
}

static void PrintQualScoresProc (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr  bsp;
  FILE       *fp;

  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  fp = (FILE*) mydata;
  PrintQualityScoresToBuffer (bsp, FALSE, fp, PrintQualProc);
}

static void PrintFarQualScoresProc (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr  bsp;
  FILE       *fp;

  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  fp = (FILE*) mydata;
  PrintQualityScoresForContig (bsp, FALSE, fp);
}

static void PopulateQuality (BioseqViewPtr bvp)

{
  BioseqPtr    bsp;
  DoC          doc;
  Uint2        entityID;
  FonT         fnt;
  FILE         *fp;
  Int2         into;
  Int2         item;
  Char         path [PATH_MAX];
  BaR          sb = NULL;
  SeqEntryPtr  sep;
  Int4         startsAt;
  TexT         txt;

  if (bvp == NULL) return;
  doc = NULL;
  txt = NULL;
  bsp = bvp->bsp;
  if (bvp->useScrollText) {
    txt = bvp->text;
    Reset (txt);
    Update ();
  } else {
    doc = bvp->doc;
    GetScrlParams (doc, NULL, &item, &into);
    sb = GetSlateVScrollBar ((SlatE) doc);
    Reset (doc);
    SetDocShade (doc, NULL, NULL, NULL, NULL);
    SetDocProcs (doc, NULL, NULL, NULL, NULL);
    SetDocCache (doc, NULL, NULL, NULL);
    Update ();
    SetDocAutoAdjust (doc, FALSE);
  }
  if (bsp == NULL) return;
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (bvp->hasTargetControl) {
    if (bvp->viewWholeEntity) {
      entityID = ObjMgrGetEntityIDForChoice (sep);
      sep = GetTopSeqEntryForEntityID (entityID);
    }
  } else {
    if (ISA_na (bsp->mol) || bsp->repr == Seq_repr_seg) {
      entityID = ObjMgrGetEntityIDForChoice (sep);
      sep = GetBestTopParentForData (entityID, bsp);
    }
  }
  if (sep == NULL) return;

  WatchCursor ();
  ffColFmt.pixWidth = screenRect.right - screenRect.left;
  ffColFmt.pixInset = 8;
  TmpNam (path);
  fp = FileOpen (path, "w");
  if (fp != NULL) {
    fnt = programFont;
    if (bvp != NULL && bvp->displayFont != NULL) {
      fnt = bvp->displayFont;
    }
    if (VisitGraphsInSep (sep, NULL, NULL) > 0) {
      SeqEntryExplore (sep, (Pointer) fp, PrintQualScoresProc);
    } else if (bsp->repr == Seq_repr_delta) {
      SeqEntryExplore (sep, (Pointer) fp, PrintFarQualScoresProc);
    } else {
      SeqEntryExplore (sep, (Pointer) fp, PrintQualScoresProc);
    }
    FileClose (fp);
    if (bvp->useScrollText) {
      if (! FileToScrollText (txt, path)) {
        SetTitle (txt, "(Text is too large to be displayed in this control.)");
      }
    } else {
      DisplayFancy (doc, path, &ffParFmt, &ffColFmt, fnt, 4);
      SetDocCache (doc, StdPutDocCache, StdGetDocCache, StdResetDocCache);
      SetDocAutoAdjust (doc, FALSE);
      ForceFormat (doc, item);
      SetDocAutoAdjust (doc, TRUE);
      AdjustDocScroll (doc);
      GetItemParams4 (doc, item, &startsAt, NULL, NULL, NULL, NULL);
      CorrectBarValue (sb, startsAt + into);
      UpdateDocument (doc, 0, 0);
    }
  }
  FileRemove (path);
  ArrowCursor ();
  Update ();
}

NLM_EXTERN void AsnPrintNewLine PROTO((AsnIoPtr aip));

static void PopulateAsnOrXML (BioseqViewPtr bvp, CharPtr outmode, Boolean doGbseq)

{
  AsnIoPtr     aipout = NULL;
  AsnTypePtr   atp = NULL;
  BioseqPtr    bsp;
  CstType      custom = 0;
  DoC          doc;
  Uint2        entityID;
  XtraPtr      extra = NULL;
  FonT         fnt;
  FmtType      format = GENBANK_FMT;
  GBSeq        gbsq;
  GBSet        gbst;
  Int2         into;
  Int2         item;
  Boolean      lockFar = FALSE;
  Boolean      lookupFar = FALSE;
  ModType      mode = SEQUIN_MODE;
  Char         path [PATH_MAX];
  BaR          sb = NULL;
  SeqEntryPtr  sep;
  Int4         startsAt;
  StlType      style = NORMAL_STYLE;
  TexT         txt;
  Int2         val;
  Char         xmlbuf [128];
  XtraBlock    xtra;

  if (bvp == NULL) return;
  doc = NULL;
  txt = NULL;
  bsp = bvp->bsp;
  if (bvp->useScrollText) {
    txt = bvp->text;
    Reset (txt);
    Update ();
  } else {
    doc = bvp->doc;
    GetScrlParams (doc, NULL, &item, &into);
    sb = GetSlateVScrollBar ((SlatE) doc);
    Reset (doc);
    SetDocShade (doc, NULL, NULL, NULL, NULL);
    SetDocProcs (doc, NULL, NULL, NULL, NULL);
    SetDocCache (doc, NULL, NULL, NULL);
    Update ();
    SetDocAutoAdjust (doc, FALSE);
  }
  if (bsp == NULL) return;
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (bvp->hasTargetControl) {
    if (bvp->viewWholeEntity) {
      entityID = ObjMgrGetEntityIDForChoice (sep);
      sep = GetTopSeqEntryForEntityID (entityID);
    }
  } else {
    if (ISA_na (bsp->mol) || bsp->repr == Seq_repr_seg) {
      entityID = ObjMgrGetEntityIDForChoice (sep);
      sep = GetBestTopParentForData (entityID, bsp);
    }
  }
  if (sep == NULL) return;

  WatchCursor ();
  ffColFmt.pixWidth = screenRect.right - screenRect.left;
  ffColFmt.pixInset = 8;
  TmpNam (path);
  aipout = AsnIoOpen (path, outmode);
  fnt = programFont;
  if (bvp != NULL && bvp->displayFont != NULL) {
    fnt = bvp->displayFont;
  }
  if (bsp != NULL && ISA_aa (bsp->mol)) {
    format = GENPEPT_FMT;
  }
  if (doGbseq && aipout != NULL) {
    if (bvp->hasTargetControl && bvp->ffModeCtrl != NULL) {
      val = GetValue (bvp->ffModeCtrl);
      switch (val) {
        case 1 :
          mode = RELEASE_MODE;
          break;
        case 2 :
          mode = ENTREZ_MODE;
          break;
        case 3 :
          mode = SEQUIN_MODE;
          break;
        case 4 :
          mode = DUMP_MODE;
          break;
        default :
          break;
      }
    }
    /* now using control instead of seqid type */
    if (bvp->ffStyleCtrl != NULL) {
      val = GetValue (bvp->ffStyleCtrl);
      switch (val) {
        case 1 :
          style = NORMAL_STYLE;
          lookupFar = TRUE;
          break;
        case 2 :
          style = SEGMENT_STYLE;
          lockFar = TRUE;
          break;
        case 3 :
          style = MASTER_STYLE;
          lockFar = TRUE;
          break;
        case 4 :
          style = CONTIG_STYLE;
          lookupFar = TRUE;
          break;
        default :
          break;
      }
    }
    if (bvp->hasTargetControl && bvp->ffCustomBtn != NULL) {
      if (GetStatus (bvp->ffCustomBtn)) {
        custom = SHOW_TRANCRIPTION | SHOW_PEPTIDE;
      }
    }
    if (GetAppParam ("NCBI", "SETTINGS", "XMLPREFIX", NULL, xmlbuf, sizeof (xmlbuf))) {
      AsnSetXMLmodulePrefix (StringSave (xmlbuf));
    }
    objgbseqAsnLoad ();
    objinsdseqAsnLoad ();
    MemSet ((Pointer) &xtra, 0, sizeof (XtraBlock));
    MemSet ((Pointer) &gbsq, 0, sizeof (GBSeq));
    xtra.gbseq = &gbsq;
    xtra.aip = aipout;
    /*
    atp = AsnLinkType (NULL, AsnFind ("GBSet"));
    xtra.atp = AsnLinkType (NULL, AsnFind ("GBSet.E"));
    */
    atp = AsnLinkType (NULL, AsnFind ("INSDSet"));
    xtra.atp = AsnLinkType (NULL, AsnFind ("INSDSet.E"));
    extra = &xtra;
    MemSet ((Pointer) &gbst, 0, sizeof (GBSet));
    AsnOpenStruct (aipout, atp, (Pointer) &gbst);
    if (SeqEntryToGnbk (sep, NULL, format, mode, style, CREATE_XML_GBSEQ_FILE, 0, custom, extra, NULL)) {
      AsnCloseStruct (aipout, atp, NULL);
      AsnPrintNewLine (aipout);
      AsnIoClose (aipout);
      if (bvp->useScrollText) {
        if (! FileToScrollText (txt, path)) {
          SetTitle (txt, "(Text is too large to be displayed in this control.)");
        }
      } else {
        DisplayFancy (doc, path, &ffParFmt, &ffColFmt, fnt, 4);
        SetDocCache (doc, StdPutDocCache, StdGetDocCache, StdResetDocCache);
        SetDocAutoAdjust (doc, FALSE);
        ForceFormat (doc, item);
        SetDocAutoAdjust (doc, TRUE);
        AdjustDocScroll (doc);
        GetItemParams4 (doc, item, &startsAt, NULL, NULL, NULL, NULL);
        CorrectBarValue (sb, startsAt + into);
        UpdateDocument (doc, 0, 0);
      }
    } else {
      AsnIoClose (aipout);
    }
  } else if (aipout != NULL) {
    if (SeqEntryAsnWrite (sep, aipout, NULL)) {
      AsnIoClose (aipout);
      if (bvp->useScrollText) {
        if (! FileToScrollText (txt, path)) {
          SetTitle (txt, "(Text is too large to be displayed in this control.)");
        }
      } else {
        DisplayFancy (doc, path, &ffParFmt, &ffColFmt, fnt, 4);
        SetDocCache (doc, StdPutDocCache, StdGetDocCache, StdResetDocCache);
        SetDocAutoAdjust (doc, FALSE);
        ForceFormat (doc, item);
        SetDocAutoAdjust (doc, TRUE);
        AdjustDocScroll (doc);
        GetItemParams4 (doc, item, &startsAt, NULL, NULL, NULL, NULL);
        CorrectBarValue (sb, startsAt + into);
        UpdateDocument (doc, 0, 0);
      }
    } else {
      AsnIoClose (aipout);
    }
  }
  FileRemove (path);
  ArrowCursor ();
  Update ();
}

static void PopulateXML (BioseqViewPtr bvp)

{
  PopulateAsnOrXML (bvp, "wx", FALSE);
}

static void PopulateAsn (BioseqViewPtr bvp)

{
  PopulateAsnOrXML (bvp, "w", FALSE);
}

static void PopulateGBSeq (BioseqViewPtr bvp)

{
  PopulateAsnOrXML (bvp, "wx", TRUE);
}

static void ShowFlatFile (BioseqViewPtr bvp, Boolean show)

{
  if (bvp == NULL) return;
  if (show) {
    if (bvp->useScrollText) {
      SafeShow (bvp->text);
    } else {
      SafeShow (bvp->doc);
    }
    SafeShow (bvp->baseCtgControlGrp);
    SafeShow (bvp->modeControlGrp);
    SafeShow (bvp->docTxtControlGrp);
    SafeShow (bvp->clickMe);
  } else {
    SafeHide (bvp->text);
    SafeHide (bvp->doc);
    Reset (bvp->text);
    Reset (bvp->doc);
    SetDocShade (bvp->doc, NULL, NULL, NULL, NULL);
    SetDocProcs (bvp->doc, NULL, NULL, NULL, NULL);
    SetDocCache (bvp->doc, NULL, NULL, NULL);
    SafeHide (bvp->styleControlGrp);
    SafeHide (bvp->scaleControlGrp);
    EnableDisableLegendItem (bvp, FALSE);
    SafeHide (bvp->findGeneGrp);
    SafeHide (bvp->docTxtControlGrp);
    SafeHide (bvp->baseCtgControlGrp);
    SafeHide (bvp->modeControlGrp);
    SafeHide (bvp->newGphControlGrp);
    SafeHide (bvp->clickMe);
  }
}

static void ShowFastaOrAsn (BioseqViewPtr bvp, Boolean show)

{
  if (bvp == NULL) return;
  if (show) {
    if (bvp->useScrollText) {
      SafeShow (bvp->text);
    } else {
      SafeShow (bvp->doc);
    }
    SafeHide (bvp->modeControlGrp);
    SafeHide (bvp->baseCtgControlGrp);
    SafeShow (bvp->docTxtControlGrp);
    SafeHide (bvp->clickMe);
  } else {
    SafeHide (bvp->text);
    SafeHide (bvp->doc);
    Reset (bvp->text);
    Reset (bvp->doc);
    SetDocShade (bvp->doc, NULL, NULL, NULL, NULL);
    SetDocProcs (bvp->doc, NULL, NULL, NULL, NULL);
    SetDocCache (bvp->doc, NULL, NULL, NULL);
    SafeHide (bvp->styleControlGrp);
    SafeHide (bvp->scaleControlGrp);
    EnableDisableLegendItem (bvp, FALSE);
    SafeHide (bvp->findGeneGrp);
    SafeHide (bvp->docTxtControlGrp);
    SafeHide (bvp->baseCtgControlGrp);
    SafeHide (bvp->modeControlGrp);
    SafeHide (bvp->newGphControlGrp);
    SafeHide (bvp->clickMe);
  }
}

static void ShowGBSeq (BioseqViewPtr bvp, Boolean show)

{
  if (bvp == NULL) return;
  if (show) {
    if (bvp->useScrollText) {
      SafeShow (bvp->text);
    } else {
      SafeShow (bvp->doc);
    }
    SafeShow (bvp->modeControlGrp);
    SafeShow (bvp->baseCtgControlGrp);
    SafeShow (bvp->docTxtControlGrp);
    SafeHide (bvp->clickMe);
  } else {
    SafeHide (bvp->text);
    SafeHide (bvp->doc);
    Reset (bvp->text);
    Reset (bvp->doc);
    SetDocShade (bvp->doc, NULL, NULL, NULL, NULL);
    SetDocProcs (bvp->doc, NULL, NULL, NULL, NULL);
    SetDocCache (bvp->doc, NULL, NULL, NULL);
    SafeHide (bvp->styleControlGrp);
    SafeHide (bvp->scaleControlGrp);
    EnableDisableLegendItem (bvp, FALSE);
    SafeHide (bvp->findGeneGrp);
    SafeHide (bvp->docTxtControlGrp);
    SafeHide (bvp->baseCtgControlGrp);
    SafeHide (bvp->modeControlGrp);
    SafeHide (bvp->newGphControlGrp);
    SafeHide (bvp->clickMe);
  }
}

static Int2 FindFFParagraph (DocDescrPtr doscr, Int4 numdescr,
                             Uint2 entityID, Uint2 itemID, Uint2 itemtype)

{
  DocDescrPtr  dsp;
  Int4         L, R, mid;
  Int2         rsult;

  L = 0;
  R = numdescr - 1;
  while (L <= R) {
    mid = (L + R) / 2;
    dsp = &(doscr [mid]);
    if (dsp == NULL) return 0;
    if (dsp->entityID > entityID) {
      R = mid - 1;
    } else if (dsp->entityID < entityID) {
      L = mid + 1;
    } else if (dsp->itemtype > itemtype) {
      R = mid - 1;
    } else if (dsp->itemtype < itemtype) {
      L = mid + 1;
    } else if (dsp->itemID > itemID) {
      R = mid - 1;
    } else if (dsp->itemID < itemID) {
      L = mid + 1;
    } else {
      rsult = dsp->docitem;
      /* scan to first paragraph for item */
      while (mid >= 0) {
        dsp = &(doscr [mid]);
        if (dsp != NULL) {
          if (dsp->entityID == entityID &&
              dsp->itemtype == itemtype &&
              dsp->itemID == itemID) {
            rsult = dsp->docitem;
          }
        }
        mid--;
      }
      return rsult;
    }
  }

  return 0;
}


static void SelectFlatFile (BioseqViewPtr bvp, Uint2 selentityID, Uint2 selitemID,
                            Uint2 selitemtype, SeqLocPtr region,
                            Boolean select, Boolean scrollto)

{
  Int2           bottom = 0;
  DoC            doc;
  FlatStructPtr  fsp;
  Uint2          entityID;
  Uint2          itemID;
  Uint2          itemtype;
  Int2           item;
  Boolean        needToScroll;
  Int2           numItems;
  RecT           rct;
  BaR            sb;
  Int2           scrollhere;
  Int4           startsAt;
  WindoW         tempPort;
  CharPtr        text;
  Int2           top = 0;

  if (bvp == NULL) return;
  if (! bvp->highlightSelections) return;
  if (bvp->useScrollText) return;
  doc = bvp->doc;
  if (doc == NULL) return;
  tempPort = SavePort (doc);
  Select (doc);
  GetDocParams (doc, &numItems, NULL);
  needToScroll = TRUE;
  scrollhere = 0;
  fsp = (FlatStructPtr) GetDocData (doc);
  if (fsp != NULL && fsp->descr != NULL && fsp->numdescr > 0) {
    if (GetScrlParams (doc, NULL, &item, NULL)) {
      while (item <= numItems && ItemIsVisible (doc, item, &top, &bottom, NULL)) {
        if (GetIDsFromDoc (doc, item, &entityID, &itemID, &itemtype)) {
          if (entityID == selentityID &&
              itemID == selitemID &&
              itemtype == selitemtype) {
            needToScroll = FALSE;
            ObjectRect (doc, &rct);
            InsetRect (&rct, 4, 4);
            rct.right = rct.left + 4;
            rct.top = top;
            rct.bottom = bottom;
            InsetRect (&rct, -1, -1);
            InvalRect (&rct);
          }
        }
        item++;
      }
    }
    scrollhere = FindFFParagraph (fsp->descr, fsp->numdescr,
                                  selentityID, selitemID, selitemtype);
    if (scrollhere > 0) {
      if (ItemIsVisible (doc, scrollhere, &top, &bottom, NULL)) {
        needToScroll = FALSE;
      }
      ObjectRect (doc, &rct);
      InsetRect (&rct, 4, 4);
      rct.right = rct.left + 4;
      rct.top = top;
      rct.bottom = bottom;
      InsetRect (&rct, -1, -1);
      InvalRect (&rct);
    } else if (selitemtype == OBJ_BIOSEQ) { /* not preindexed in fsp descr */
      ObjectRect (doc, &rct);
      InsetRect (&rct, 4, 4);
      rct.right = rct.left + 4;
      rct.top = top;
      rct.bottom = bottom;
      InsetRect (&rct, -1, -1);
      InvalRect (&rct);
    }
  }
  /* for items currently not preindexed (LOCUS, ORGANISM), do a short linear search */
  if (/* scrollhere == 0 && */ selitemID > 0 /* && needToScroll */) {
    for (item = 1; item <= numItems && item < 10; item++) {
      if (GetIDsFromDoc (doc, item, &entityID, &itemID, &itemtype)) {
        if (entityID == selentityID &&
            itemID == selitemID &&
            itemtype == selitemtype) {
          /*
          if (ItemIsVisible (doc, item, &top, &bottom, NULL)) {
            needToScroll = FALSE;
            ObjectRect (doc, &rct);
            InsetRect (&rct, 4, 4);
            rct.right = rct.left + 4;
            rct.top = top;
            rct.bottom = bottom;
            InsetRect (&rct, -1, -1);
            InvalRect (&rct);
          } else if (scrollhere == 0) {
            scrollhere = item;
          }
          */
          if (needToScroll) {
            scrollhere = item;
            item = 10; /* break the for loop */
          }
        }
      }
    }
  }
  if (scrollto && needToScroll && scrollhere > 0 && selitemtype != OBJ_BIOSEQ) {
    text = GetDocText (doc, scrollhere, 0, 1); /* forces format if not before */
    MemFree (text);
    ForceFormat (doc, scrollhere); /* forces UpdateLineStarts */
    GetItemParams4 (doc, scrollhere, &startsAt, NULL, NULL, NULL, NULL);
    sb = GetSlateVScrollBar ((SlatE) doc);
    CorrectBarValue (sb, startsAt);
    ObjectRect (doc, &rct);
    InsetRect (&rct, 4, 4);
    InsetRect (&rct, -1, -1);
    InvalRect (&rct);
  }
  RestorePort (tempPort);
}

static void CopyFlatFileFastaOrAsn (BioseqViewPtr bvp)

{
  FILE  *fp;
  Char  path [PATH_MAX];

  if (bvp == NULL) return;
  if (bvp->useScrollText) {
    if (bvp->text != NULL) {
      CopyText (bvp->text);
    }
  } else {
    TmpNam (path);
    fp = FileOpen (path, "w");
    if (fp != NULL) {
      SaveDocument (bvp->doc, fp);
      FileClose (fp);
      FileToClipboard (path);
    }
    FileRemove (path);
  }
}

#ifdef WIN_MOTIF
extern CharPtr Nlm_XrmGetResource (const Char PNTR _resource);
#endif

static void PrintFlatFileFastaOrAsn (BioseqViewPtr bvp)

{
  DoC      doc;
  TexT     txt;
#ifdef WIN_MOTIF
  Char     cmmd [256];
  Int2     len;
  CharPtr  printCmd;
  Char     str [PATH_MAX];
#endif

  if (bvp == NULL) return;
  if (bvp->bsp == NULL) return;
  doc = NULL;
  txt = NULL;
  if (bvp->useScrollText) {
    txt = bvp->text;
  } else {
    doc = bvp->doc;
  }
  if (doc != NULL) {
    PrintDocument (doc);
  } else if (txt != NULL) {
#ifdef WIN_MOTIF
    TmpNam (str);
    ScrollTextToFile (txt, str);
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

static void ExportFlatFileFastaOrAsnEx (BioseqViewPtr bvp, CharPtr filename, CharPtr dfault, Boolean specialSave)

{
  Char     ch;
  Char     dfaultFile [32];
  DoC      doc;
  FILE     *f;
  Int2     i;
  Int2     j;
  Int2     k;
  Int2     numItems;
  Char     path [PATH_MAX];
  CharPtr  str;
  TexT     txt;

  if (bvp == NULL) return;
  if (bvp->bsp == NULL) return;
  doc = NULL;
  txt = NULL;
  if (bvp->useScrollText) {
    txt = bvp->text;
  } else {
    doc = bvp->doc;
  }
  if (doc != NULL || txt != NULL) {
    dfault [0] = '\0';
    StringNCpy_0 (dfaultFile, dfault, sizeof (dfaultFile));
    j = 0;
    k = 0;
    ch = dfaultFile [j];
    while (j < sizeof (dfaultFile) && ch != '\0') {
      if (ch <= ' ') {
        j++;
      } else {
        dfaultFile [k] = dfaultFile [j];
        k++;
        j++;
      }
      ch = dfaultFile [j];
    }
    dfaultFile [k] = '\0';
#ifdef WIN_MSWIN
    j = 0;
    ch = dfaultFile [j];
    while (j < sizeof (dfaultFile) && ch != '\0') {
      if (ch == '_' || IS_ALPHANUM (ch)) {
        j++;
        ch = dfaultFile [j];
      } else {
        ch = '\0';
      }
    }
    dfaultFile [j] = '\0';
#endif
    path [0] = '\0';
    StringNCpy_0 (path, filename, sizeof (path));
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), dfaultFile)) {
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
        if (bvp->useScrollText) {
          ScrollTextToFile (txt, path);
        } else if (specialSave) {
          GetDocParams (doc, &numItems, NULL);
          for (i = 1; i <= numItems; i++) {
            str = GetDocText (doc, i, 0, 0);
            if (! StringHasNoText (str)) {
              fprintf (f, "%s", str);
            }
            MemFree (str);
          }
        } else {
          SaveDocument (doc, f);
        }
        FileClose (f);
      }
      ArrowCursor ();
    }
  }
}

static void ExportFlatFileFastaOrAsn (BioseqViewPtr bvp, CharPtr filename, CharPtr dfault)

{
  ExportFlatFileFastaOrAsnEx (bvp, filename, dfault, FALSE);
}

static void ExportFeatureTable (BioseqViewPtr bvp, CharPtr filename, CharPtr dfault)

{
  ExportFlatFileFastaOrAsnEx (bvp, filename, dfault, TRUE);
}

static CharPtr asnconfirmmsg =
"'Export' saves only the targeted portion of the record.\n\
Use 'Save' to save the entire record to a file, so that\n\
it can be read back in.  Proceed with export?";

static void ExportAsnAfterConfirming (BioseqViewPtr bvp, CharPtr filename, CharPtr dfault)

{
  MsgAnswer  ans;

  if (bvp->hasTargetControl) {
    ans = Message (MSG_YN, "%s", asnconfirmmsg);
    if (ans == ANS_NO) return;
  }
  ExportFlatFileFastaOrAsn (bvp, filename, dfault);
}

static void ResizeFlatFileFastaOrAsn (BioseqViewPtr bvp)

{
  if (bvp == NULL) return;
  if (bvp->doc != NULL) {
    if (Visible (bvp->doc) && AllParentsVisible (bvp->doc)) {
      UpdateDocument (bvp->doc, 0, 0);
    }
  }
}

BioseqPageData gbgnPageData = {
  "GenBank", FALSE, FALSE, TRUE, FALSE, -1,
  PopulateGenBank, ShowFlatFile, SelectFlatFile,
  CopyFlatFileFastaOrAsn, PrintFlatFileFastaOrAsn,
  ExportFlatFileFastaOrAsn, NULL, ResizeFlatFileFastaOrAsn, NULL
};

BioseqPageData gnbkPageData = {
  "GenBank", TRUE, FALSE, FALSE, FALSE, -1,
  PopulateGenBank, ShowFlatFile, SelectFlatFile,
  CopyFlatFileFastaOrAsn, PrintFlatFileFastaOrAsn,
  ExportFlatFileFastaOrAsn, NULL, ResizeFlatFileFastaOrAsn, NULL
};

BioseqPageData emblPageData = {
  "EMBL", TRUE, FALSE, FALSE, FALSE, -1,
  PopulateEMBL, ShowFlatFile, SelectFlatFile,
  CopyFlatFileFastaOrAsn, PrintFlatFileFastaOrAsn,
  ExportFlatFileFastaOrAsn, NULL, ResizeFlatFileFastaOrAsn, NULL
};

BioseqPageData ddbjPageData = {
  "DDBJ", TRUE, FALSE, FALSE, FALSE, -1,
  PopulateDDBJ, ShowFlatFile, SelectFlatFile,
  CopyFlatFileFastaOrAsn, PrintFlatFileFastaOrAsn,
  ExportFlatFileFastaOrAsn, NULL, ResizeFlatFileFastaOrAsn, NULL
};

BioseqPageData gnptPageData = {
  "GenPept", FALSE, TRUE, FALSE, FALSE, -1,
  PopulateGenPept, ShowFlatFile, SelectFlatFile,
  CopyFlatFileFastaOrAsn, PrintFlatFileFastaOrAsn,
  ExportFlatFileFastaOrAsn, NULL, ResizeFlatFileFastaOrAsn, NULL
};

BioseqPageData ftblPageData = {
  "Table", TRUE, TRUE, TRUE, FALSE, -1,
  PopulateFTable, ShowFlatFile, SelectFlatFile,
  CopyFlatFileFastaOrAsn, PrintFlatFileFastaOrAsn,
  ExportFeatureTable, NULL, ResizeFlatFileFastaOrAsn, NULL
};

BioseqPageData fstaPageData = {
  "FASTA", TRUE, TRUE, TRUE, FALSE, -1,
  PopulateFasta, ShowFastaOrAsn, NULL,
  CopyFlatFileFastaOrAsn, PrintFlatFileFastaOrAsn,
  ExportFlatFileFastaOrAsn, NULL, ResizeFlatFileFastaOrAsn, NULL
};

BioseqPageData qualPageData = {
  "Quality", TRUE, TRUE, FALSE, FALSE, -1,
  PopulateQuality, ShowFastaOrAsn, NULL,
  CopyFlatFileFastaOrAsn, PrintFlatFileFastaOrAsn,
  ExportFlatFileFastaOrAsn, NULL, ResizeFlatFileFastaOrAsn, NULL
};

BioseqPageData asnPageData = {
  "ASN.1", TRUE, TRUE, TRUE, FALSE, -1,
  PopulateAsn, ShowFastaOrAsn, NULL,
  CopyFlatFileFastaOrAsn, PrintFlatFileFastaOrAsn,
  ExportAsnAfterConfirming, NULL, ResizeFlatFileFastaOrAsn, NULL
};

BioseqPageData xmlPageData = {
  "XML", TRUE, TRUE, TRUE, FALSE, -1,
  PopulateXML, ShowFastaOrAsn, NULL,
  CopyFlatFileFastaOrAsn, PrintFlatFileFastaOrAsn,
  ExportAsnAfterConfirming, NULL, ResizeFlatFileFastaOrAsn, NULL
};

BioseqPageData gbseqPageData = {
  "INSDSeq", TRUE, TRUE, TRUE, FALSE, -1,
  PopulateGBSeq, ShowGBSeq, NULL,
  CopyFlatFileFastaOrAsn, PrintFlatFileFastaOrAsn,
  ExportAsnAfterConfirming, NULL, ResizeFlatFileFastaOrAsn, NULL
};

