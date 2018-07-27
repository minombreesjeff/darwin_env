/*   sqnutil1.c
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
* File Name:  sqnutil1.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   9/2/97
*
* $Revision: 6.296 $
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

#include <sqnutils.h>
#include <gather.h>
#include <subutil.h>
#include <objfdef.h>
#include <seqport.h>
#include <objproj.h>
/* #include <objmmdb1.h> */
#include <gbfeat.h>
#include <gbftdef.h>
#include <edutil.h>
#include <tofasta.h>
#include <parsegb.h>
#include <utilpars.h>
#include <validatr.h>
#include <explore.h>

NLM_EXTERN DatePtr DateAdvance (DatePtr dp, Uint1 monthsToAdd)

{
  if (dp == NULL) {
    dp = DateCurr ();
  }
  if (dp != NULL && dp->data [0] == 1 && dp->data [1] > 0) {
    while (monthsToAdd > 12) {
      monthsToAdd--;
      (dp->data [1])++;
    }
    if (dp->data [2] < 13 - monthsToAdd) {
      (dp->data [2]) += monthsToAdd;
    } else {
      (dp->data [1])++;
      (dp->data [2]) -= (12 - monthsToAdd);
    }
    if (dp->data [2] == 0) {
      dp->data [2] = 1;
    }
    if (dp->data [3] == 0) {
      switch (dp->data [2]) {
        case 4 :
        case 6 :
        case 9 :
        case 11 :
          dp->data [3] = 30;
          break;
        case 2 :
          dp->data [3] = 28;
          break;
        default :
          dp->data [3] = 31;
          break;
      }
    }
  }
  switch (dp->data [2]) {
    case 4 :
    case 6 :
    case 9 :
    case 11 :
      if (dp->data [3] > 30) {
        dp->data [3] = 30;
      }
      break;
    case 2 :
      if (dp->data [3] > 28) {
        dp->data [3] = 28;
      }
      break;
    default :
      if (dp->data [3] > 31) {
        dp->data [3] = 31;
      }
      break;
  }
  return dp;
}

typedef struct orgscan {
  ObjMgrPtr     omp;
  Int2          nuclCode;
  Int2          mitoCode;
  Boolean       mito;
  Boolean       plastid;
  Char          taxname [196];
  BioSourcePtr  biop;
} OrgScan, PNTR OrgScanPtr;

static Boolean OrgScanGatherFunc (GatherContextPtr gcp)

{
  BioSourcePtr   biop;
  Boolean        doCodes = FALSE;
  Boolean        doMito = FALSE;
  Boolean        doTaxname = FALSE;
  Boolean        mito = FALSE;
  Int2           mitoCode = 0;
  Int2           nuclCode = 0;
  ObjMgrTypePtr  omtp;
  OrgNamePtr     onp;
  OrgRefPtr      orp;
  OrgScanPtr     osp;
  ValNodePtr     sdp;
  SeqFeatPtr     sfp;
  Uint2          subtype;
  CharPtr        taxname = NULL;
  Int2           val;
  ValNodePtr     vnp;

  if (gcp == NULL || gcp->thisitem == NULL) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT  && gcp->thistype != OBJ_SEQDESC) return TRUE;

  osp = (OrgScanPtr) gcp->userdata;
  if (osp == NULL) return TRUE;

  subtype = 0;
  if (gcp->thistype == OBJ_SEQFEAT  || gcp->thistype == OBJ_SEQDESC) {
    omtp = ObjMgrTypeFind (osp->omp, gcp->thistype, NULL, NULL);
    if (omtp == NULL) {
      return TRUE;
    }
    if (omtp->subtypefunc != NULL) {
      subtype = (*(omtp->subtypefunc)) (gcp->thisitem);
    }
  }

  orp = NULL;
  biop = NULL;
  switch (gcp->thistype) {
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gcp->thisitem;
      switch (subtype) {
        case FEATDEF_ORG :
          orp = (OrgRefPtr) sfp->data.value.ptrvalue;
          break;
        case FEATDEF_BIOSRC :
          biop = (BioSourcePtr) sfp->data.value.ptrvalue;
          break;
        default :
          break;
      }
      break;
    case OBJ_SEQDESC :
      sdp = (ValNodePtr) gcp->thisitem;
      switch (subtype) {
        case Seq_descr_modif :
          vnp = (ValNodePtr) sdp->data.ptrvalue;
          while (vnp != NULL) {
            val = (Int2) vnp->data.intvalue;
            if (val == MODIF_mitochondrial || val == MODIF_kinetoplast) {
              mito = TRUE;
              doMito = TRUE;
              /* osp->mito = TRUE; */
            }
            vnp = vnp->next;
          }
          break;
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
    mito = (Boolean) (biop->genome == 4 || biop->genome == 5);
    doMito = TRUE;
    /* osp->mito = (Boolean) (biop->genome == 4 || biop->genome == 5); */
  }
  if (orp != NULL) {
    taxname = orp->taxname;
    doTaxname = TRUE;
    /* StringNCpy_0 (osp->taxname, orp->taxname, sizeof (osp->taxname)); */
    onp = orp->orgname;
    if (onp != NULL) {
      nuclCode = onp->gcode;
      mitoCode = onp->mgcode;
      doCodes = TRUE;
      /* osp->nuclCode = onp->gcode;
      osp->mitoCode = onp->mgcode; */
    }
  }
  if (biop != NULL) {
    if (osp->biop == NULL || biop->is_focus) {
      osp->biop = biop;
      if (doMito) {
        osp->mito = mito;
      }
      osp->plastid = (Boolean) (biop->genome == GENOME_chloroplast ||
                                biop->genome == GENOME_chromoplast ||
                                biop->genome == GENOME_plastid ||
                                biop->genome == GENOME_cyanelle ||
                                biop->genome == GENOME_apicoplast ||
                                biop->genome == GENOME_leucoplast ||
                                biop->genome == GENOME_proplastid);
      if (doCodes) {
        osp->nuclCode = nuclCode;
        osp->mitoCode = mitoCode;
      }
      if (doTaxname) {
        StringNCpy_0 (osp->taxname, taxname, sizeof (osp->taxname));
      }
    }
  }

  return TRUE;
}

static Int2 SeqEntryOrEntityIDToGeneticCode (SeqEntryPtr sep, Uint2 entityID, BoolPtr mito,
                                             CharPtr taxname, size_t maxsize,
                                             BioSourcePtr PNTR biopp)

{
  GatherScope  gs;
  OrgScan      osp;

  if (mito != NULL) {
    *mito = FALSE;
  }
  if (taxname != NULL && maxsize > 0) {
    *taxname = '\0';
  }
  osp.mito = FALSE;
  osp.plastid = FALSE;
  osp.nuclCode = 0;
  osp.mitoCode = 0;
  osp.omp = ObjMgrGet ();
  osp.taxname [0] = '\0';
  osp.biop = NULL;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  if (sep != NULL) {
    gs.scope = sep;
    GatherSeqEntry (sep, (Pointer) &osp, OrgScanGatherFunc, &gs);
  } else if (entityID > 0) {
    GatherEntity (entityID, (Pointer) &osp, OrgScanGatherFunc, &gs);
  }
  if (mito != NULL) {
    *mito = osp.mito;
  }
  if (taxname != NULL && maxsize > 0) {
    StringNCpy_0 (taxname, osp.taxname, maxsize);
  }
  if (biopp != NULL) {
    *biopp = osp.biop;
  }
  if (osp.plastid) {
    return 11;
  } else if (osp.mito) {
    return osp.mitoCode;
  } else {
    return osp.nuclCode;
  }
}

NLM_EXTERN Int2 EntityIDToGeneticCode (Uint2 entityID, BoolPtr mito, CharPtr taxname, size_t maxsize)

{
  return SeqEntryOrEntityIDToGeneticCode (NULL, entityID, mito, taxname, maxsize, NULL);
}

NLM_EXTERN Int2 SeqEntryToGeneticCode (SeqEntryPtr sep, BoolPtr mito, CharPtr taxname, size_t maxsize)

{
  return SeqEntryOrEntityIDToGeneticCode (sep, 0, mito, taxname, maxsize, NULL);
}

NLM_EXTERN Int2 SeqEntryToBioSource (SeqEntryPtr sep, BoolPtr mito, CharPtr taxname, size_t maxsize, BioSourcePtr PNTR biopp)

{
  return SeqEntryOrEntityIDToGeneticCode (sep, 0, mito, taxname, maxsize, biopp);
}

static Boolean FindBspItem (GatherContextPtr gcp)

{
  BioseqPtr  PNTR bspp;

  bspp = (BioseqPtr PNTR) gcp->userdata;
  if (bspp != NULL && gcp->thistype == OBJ_BIOSEQ) {
    *bspp = (BioseqPtr) gcp->thisitem;
  }
  return TRUE;
}

NLM_EXTERN BioseqPtr GetBioseqGivenIDs (Uint2 entityID, Uint2 itemID, Uint2 itemtype)

{
  BioseqPtr  bsp;

  bsp = NULL;
  if (entityID > 0 && itemID > 0 && itemtype == OBJ_BIOSEQ) {
    GatherItem (entityID, itemID, itemtype, (Pointer) (&bsp), FindBspItem);
  }
  return bsp;
}

NLM_EXTERN BioseqPtr GetBioseqGivenSeqLoc (SeqLocPtr slp, Uint2 entityID)

{
  BioseqPtr    bsp;
  SeqEntryPtr  sep;
  SeqIdPtr     sip;

  if (slp == NULL) return NULL;
  bsp = NULL;
  sip = SeqLocId (slp);
  if (sip != NULL) {
    bsp = BioseqFind (sip);
  } else if (entityID > 0) {
    slp = SeqLocFindNext (slp, NULL);
    if (slp != NULL) {
      sip = SeqLocId (slp);
      if (sip != NULL) {
        bsp = BioseqFind (sip);
        if (bsp != NULL) {
          sep = GetBestTopParentForData (entityID, bsp);
          if (sep != NULL) {
            sep = FindNucSeqEntry (sep);
            if (sep != NULL && sep->choice == 1) {
              bsp = (BioseqPtr) sep->data.ptrvalue;
            }
          }
        }
      }
    }
  }
  return bsp;
}

typedef struct tripletdata {
	Uint2      entityID;
	Uint2      itemID;
	Uint2      itemtype;
	Pointer    lookfor;
} TripletData, PNTR TripletDataPtr;

static Boolean FindIDsFromPointer (GatherContextPtr gcp)

{
  TripletDataPtr  tdp;

  tdp = (TripletDataPtr) gcp->userdata;
  if (tdp != NULL && gcp->thisitem == tdp->lookfor) {
    tdp->entityID = gcp->entityID;
    tdp->itemID = gcp->itemID;
    tdp->itemtype = gcp->thistype;
  }
  return TRUE;
}

NLM_EXTERN Uint2 GetItemIDGivenPointer (Uint2 entityID, Uint2 itemtype, Pointer lookfor)

{
  GatherScope  gs;
  TripletData  td;

  if (entityID > 0 && itemtype > 0 && itemtype < OBJ_MAX && lookfor != NULL) {
    td.entityID = 0;
    td.itemID = 0;
    td.itemtype = 0;
    td.lookfor = lookfor;
    MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
    gs.seglevels = 1;
    gs.get_feats_location = FALSE;
    MemSet ((Pointer)(gs.ignore), (int)(FALSE), (size_t)(OBJ_MAX * sizeof(Boolean)));
    /* gs.ignore[itemtype] = FALSE; */
    GatherEntity (entityID, (Pointer) (&td), FindIDsFromPointer, &gs);
    if (td.entityID == entityID && td.itemID > 0 && td.itemtype == itemtype) {
      return td.itemID;
    }
  }
  return 0;
}

static void AddNucPart (BioseqPtr segseq, BioseqSetPtr parts, SeqEntryPtr addme)

{
  BioseqPtr    bsp;
  SeqLocPtr    slp;
  SeqEntryPtr  tmp;

  if (segseq == NULL || addme == NULL) return;
  if (addme->choice != 1 || addme->data.ptrvalue == NULL) return;
  bsp = (BioseqPtr) addme->data.ptrvalue;

  slp = ValNodeNew ((ValNodePtr) segseq->seq_ext);
  if (slp == NULL) return;
  if (segseq->seq_ext == NULL) {
    segseq->seq_ext = (Pointer) slp;
  }
  if (bsp->length >= 0) {
    segseq->length += bsp->length;
    slp->choice = SEQLOC_WHOLE;
    slp->data.ptrvalue = (Pointer) SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
  } else {
    slp->choice = SEQLOC_NULL;
    addme = SeqEntryFree (addme);
    return;
  }

  if (parts == NULL) {
    addme = SeqEntryFree (addme);
    return;
  }
  if (parts->seq_set != NULL) {
    tmp = parts->seq_set;
    while (tmp->next != NULL) {
      tmp = tmp->next;
    }
    tmp->next = addme;
  } else {
    parts->seq_set = addme;
  }
}

NLM_EXTERN void GetSeqEntryParent (SeqEntryPtr target, Pointer PNTR parentptr, Uint2Ptr parenttype)

{
  ObjMgrPtr      omp;
  ObjMgrDataPtr  omdp;

  if (parentptr == NULL || parenttype == NULL) return;
  *parenttype = 0;
  *parentptr = NULL;
  if (target == NULL || target->data.ptrvalue == NULL) return;
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  omdp = ObjMgrFindByData (omp, target->data.ptrvalue);
  if (omdp == NULL) return;
  *parenttype = omdp->parenttype;
  *parentptr = omdp->parentptr;
}

NLM_EXTERN void SaveSeqEntryObjMgrData (SeqEntryPtr target, ObjMgrDataPtr PNTR omdptopptr, ObjMgrData PNTR omdataptr)

{
  ObjMgrPtr	     omp;
  ObjMgrDataPtr  omdp, omdptop = NULL;

  if (target == NULL || omdptopptr == NULL || omdataptr == NULL) return;
  *omdptopptr = NULL;
  MemSet ((Pointer) omdataptr, 0, sizeof (ObjMgrData));
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  omdp = ObjMgrFindByData (omp, target->data.ptrvalue);
  if (omdp == NULL) return;
  omdptop = ObjMgrFindTop (omp, omdp);
  if (omdptop == NULL) return;
  if (omdptop->EntityID == 0) return;
  *omdptopptr = omdptop;
  MemCopy ((Pointer) omdataptr, omdptop, sizeof (ObjMgrData));
  omdptop->userdata = NULL;
}

extern void ObjMgrRemoveEntityIDFromRecycle (Uint2 entityID, ObjMgrPtr omp);
extern void ObjMgrRecordOmdpByEntityID (Uint2 entityID, ObjMgrDataPtr omdp);
NLM_EXTERN void RestoreSeqEntryObjMgrData (SeqEntryPtr target, ObjMgrDataPtr omdptop, ObjMgrData PNTR omdataptr)

{
  ObjMgrPtr	omp;
  ObjMgrDataPtr omdp, omdpnew = NULL;

  if (target == NULL || omdptop == NULL || omdataptr == NULL) return;
  if (omdataptr->EntityID == 0) return;
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  omdp = ObjMgrFindByData (omp, target->data.ptrvalue);
  if (omdp == NULL) return;
  omdpnew = ObjMgrFindTop (omp, omdp);
  if (omdpnew == NULL) return;
  if (omdpnew != omdptop) {
    omdpnew->EntityID = omdataptr->EntityID;
    omdptop->EntityID = 0;
    omdpnew->lockcnt = omdataptr->lockcnt;
    omdpnew->tempload = omdataptr->tempload;
    omdpnew->clipboard = omdataptr->clipboard;
    omdpnew->dirty = omdataptr->dirty;
    omdpnew->being_freed = omdataptr->being_freed;
    omdpnew->free = omdataptr->free;
    omdpnew->options = omdataptr->options;
    ObjMgrRemoveEntityIDFromRecycle (omdpnew->EntityID, omp);
    ObjMgrRecordOmdpByEntityID (omdpnew->EntityID, omdpnew);
  }
  omdpnew->userdata = omdataptr->userdata;
}

NLM_EXTERN void AddSeqEntryToSeqEntry (SeqEntryPtr target, SeqEntryPtr insert, Boolean relink)

{
  SeqEntryPtr    first;
  BioseqPtr      insertbsp;
  BioseqSetPtr   nuc_prot;
  Uint2          parenttype;
  Pointer        parentptr;
  BioseqSetPtr   parts;
  BioseqPtr      seg;
  BioseqSetPtr   segs;
  BioseqPtr      targetbsp;
  BioseqSetPtr   targetbssp;
  SeqEntryPtr    the_nuc;
  SeqEntryPtr    tmp;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;

  if (target == NULL || insert == NULL) return;
  if (target->data.ptrvalue == NULL || insert->data.ptrvalue == NULL) return;

  if (relink) {
    SaveSeqEntryObjMgrData (target, &omdptop, &omdata);
    GetSeqEntryParent (target, &parentptr, &parenttype);
  }

  if (IS_Bioseq (target) && IS_Bioseq (insert)) {
    targetbsp = (BioseqPtr) target->data.ptrvalue;
    insertbsp = (BioseqPtr) insert->data.ptrvalue;
    if (ISA_na (targetbsp->mol)) {
      if (ISA_na (insertbsp->mol)) {

        seg = BioseqNew ();
        if (seg == NULL) return;
        seg->mol = targetbsp->mol;
        seg->repr = Seq_repr_seg;
        seg->seq_ext_type = 1;
        seg->length = 0;
        /* seg->id = MakeSeqID ("SEG_dna"); */
        /* seg->id = MakeNewProteinSeqId (NULL, NULL); */
        seg->id = MakeUniqueSeqID ("segseq_");
        SeqMgrAddToBioseqIndex (seg);

        the_nuc = SeqEntryNew ();
        if (the_nuc == NULL) return;
        the_nuc->choice = 1;
        the_nuc->data.ptrvalue = (Pointer) seg;

        segs = BioseqSetNew ();
        if (segs == NULL) return;
        segs->_class = 2;
        segs->seq_set = the_nuc;

        parts = BioseqSetNew ();
        if (parts == NULL) return;
        parts->_class = 4;

        tmp = SeqEntryNew ();
        if (tmp == NULL) return;
        tmp->choice = 2;
        tmp->data.ptrvalue = (Pointer) parts;
        the_nuc->next = tmp;

        first = SeqEntryNew ();
        if (first == NULL) return;
        first->choice = 1;
        first->data.ptrvalue = (Pointer) targetbsp;
        target->choice = 2;
        target->data.ptrvalue = (Pointer) segs;

        AddNucPart (seg, parts, first);
        AddNucPart (seg, parts, insert);

      } else if (ISA_aa (insertbsp->mol)) {

        nuc_prot = BioseqSetNew ();
        if (nuc_prot == NULL) return;
        nuc_prot->_class = 1;

        the_nuc = SeqEntryNew ();
        if (the_nuc == NULL) return;
        the_nuc->choice = 1;
        the_nuc->data.ptrvalue = (Pointer) targetbsp;
        target->choice = 2;
        target->data.ptrvalue = (Pointer) nuc_prot;
        nuc_prot->seq_set = the_nuc;

        the_nuc->next = insert;

      }
    }
  } else if (IS_Bioseq_set (target)) {
    targetbssp = (BioseqSetPtr) target->data.ptrvalue;
    if (targetbssp->_class == 1 && IS_Bioseq (insert)) {
     insertbsp = (BioseqPtr) insert->data.ptrvalue;
     if (ISA_aa (insertbsp->mol)) {

        nuc_prot = targetbssp;
        if (nuc_prot->seq_set != NULL) {
          tmp = nuc_prot->seq_set;
          while (tmp->next != NULL) {
            tmp = tmp->next;
          }
          tmp->next = insert;
        } else {
          nuc_prot->seq_set = insert;
        }

      }
    } else if (targetbssp->_class == 2 && IS_Bioseq (insert)) {
      insertbsp = (BioseqPtr) insert->data.ptrvalue;
      if (ISA_na (insertbsp->mol)) {

        the_nuc = FindNucSeqEntry (target);
        if (the_nuc != NULL && the_nuc->next != NULL) {
          tmp = the_nuc->next;
          if (tmp->choice == 2 && tmp->data.ptrvalue != NULL) {
            parts = (BioseqSetPtr) tmp->data.ptrvalue;
            if (parts->_class == 4 && the_nuc->choice == 1) {
              seg = (BioseqPtr) the_nuc->data.ptrvalue;
              AddNucPart (seg, parts, insert);
            }
          }
        }

      } else if (ISA_aa (insertbsp->mol)) {

        nuc_prot = BioseqSetNew ();
        if (nuc_prot == NULL) return;
        nuc_prot->_class = 1;

        first = SeqEntryNew ();
        if (first == NULL) return;
        first->choice = 2;
        first->data.ptrvalue = (Pointer) targetbssp;
        target->choice = 2;
        target->data.ptrvalue = (Pointer) nuc_prot;
        nuc_prot->seq_set = first;

        first->next = insert;

      }
    } else if (targetbssp->_class == 7) {

      if (targetbssp->seq_set != NULL) {
        tmp = targetbssp->seq_set;
        while (tmp->next != NULL) {
          tmp = tmp->next;
        }
        tmp->next = insert;
      } else {
        targetbssp->seq_set = insert;
      }
    } else if ((targetbssp->_class >= BioseqseqSet_class_mut_set &&
                targetbssp->_class <= BioseqseqSet_class_eco_set) ||
               targetbssp->_class >= BioseqseqSet_class_wgs_set) {

      if (targetbssp->seq_set != NULL) {
        tmp = targetbssp->seq_set;
        while (tmp->next != NULL) {
          tmp = tmp->next;
        }
        tmp->next = insert;
      } else {
        targetbssp->seq_set = insert;
      }

    } else if (targetbssp->_class == BioseqseqSet_class_gen_prod_set) {

      if (targetbssp->seq_set != NULL) {
        tmp = targetbssp->seq_set;
        while (tmp->next != NULL) {
          tmp = tmp->next;
        }
        tmp->next = insert;
      } else {
        targetbssp->seq_set = insert;
      }

    }
  }

  if (relink) {
    SeqMgrLinkSeqEntry (target, parenttype, parentptr);
    RestoreSeqEntryObjMgrData (target, omdptop, &omdata);
  }
}

NLM_EXTERN void ReplaceSeqEntryWithSeqEntry (SeqEntryPtr target, SeqEntryPtr replaceWith, Boolean relink)

{
  Uint2          parenttype;
  Pointer        parentptr;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;

  if (target == NULL || replaceWith == NULL) return;

  if (relink) {
    SaveSeqEntryObjMgrData (target, &omdptop, &omdata);
    GetSeqEntryParent (target, &parentptr, &parenttype);
  }

  if (target->choice == 1) {
    BioseqFree ((BioseqPtr) target->data.ptrvalue);
  } else if (target->choice == 2) {
    BioseqSetFree ((BioseqSetPtr) target->data.ptrvalue);
  }
  target->choice = replaceWith->choice;
  target->data.ptrvalue = replaceWith->data.ptrvalue;
  MemFree (replaceWith);

  if (relink) {
    SeqMgrLinkSeqEntry (target, parenttype, parentptr);
    RestoreSeqEntryObjMgrData (target, omdptop, &omdata);
  }
}

static void SeqEntryRemoveLoop (SeqEntryPtr sep, SeqEntryPtr del, SeqEntryPtr PNTR prev)

{
  BioseqSetPtr  bssp;
  SeqEntryPtr   next;

  while (sep != NULL) {
    next = sep->next;
    if (sep == del) {
      *prev = sep->next;
      sep->next = NULL;
      SeqEntryFree (sep);
    } else {
      prev = (SeqEntryPtr PNTR) &(sep->next);
      if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL) {
          SeqEntryRemoveLoop (bssp->seq_set, del, &(bssp->seq_set));
        }
      }
    }
    sep = next;
  }
}

NLM_EXTERN void RemoveSeqEntryFromSeqEntry (SeqEntryPtr top, SeqEntryPtr del, Boolean relink)

{
  SeqEntryPtr    dummy;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;
  Uint2          parenttype;
  Pointer        parentptr;

  if (top == NULL || del == NULL) return;
  if (top->data.ptrvalue == NULL || del->data.ptrvalue == NULL) return;

  if (relink) {
    SaveSeqEntryObjMgrData (top, &omdptop, &omdata);
    GetSeqEntryParent (top, &parentptr, &parenttype);
  }

  dummy = NULL;
  SeqEntryRemoveLoop (top, del, &dummy);

  if (relink) {
    SeqMgrLinkSeqEntry (top, parenttype, parentptr);
    RestoreSeqEntryObjMgrData (top, omdptop, &omdata);
  }
}

NLM_EXTERN void DeleteMultipleTitles (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Boolean       hastitle;
  ValNodePtr    nextsdp;
  Pointer PNTR  prevsdp;
  ValNodePtr    sdp;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else return;
  hastitle = FALSE;
  while (sdp != NULL) {
    nextsdp = sdp->next;
    if (sdp->choice == Seq_descr_title) {
      if (hastitle) {
        *(prevsdp) = sdp->next;
        sdp->next = NULL;
        SeqDescFree (sdp);
      } else {
        hastitle = TRUE;
        prevsdp = (Pointer PNTR) &(sdp->next);
      }
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
}

NLM_EXTERN void RenormalizeNucProtSets (SeqEntryPtr sep, Boolean relink)

{
  SeqAnnotPtr    annot;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  ValNodePtr     descr;
  ObjMgrDataPtr  omdptop;
  ObjMgrData     omdata;
  Uint2          parenttype;
  Pointer        parentptr;
  SeqAnnotPtr    sap;
  SeqEntryPtr    seqentry;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (bssp->_class >= 13 && bssp->_class <= 16) ||
                         bssp->_class == BioseqseqSet_class_wgs_set ||
                         bssp->_class == BioseqseqSet_class_gen_prod_set)) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        RenormalizeNucProtSets (sep, relink);
      }
      return;
    }
    if (bssp != NULL && bssp->_class == 1) {
      seqentry = bssp->seq_set;
      if (seqentry != NULL && seqentry->next == NULL) {

        if (relink) {
          SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
          GetSeqEntryParent (sep, &parentptr, &parenttype);
        }

        descr = bssp->descr;
        bssp->descr = NULL;
        annot = bssp->annot;
        bssp->annot = NULL;

        sep->choice = seqentry->choice;
        sep->data.ptrvalue = seqentry->data.ptrvalue;
        seqentry->data.ptrvalue = NULL;
        bssp->seq_set = NULL;
        bssp->seqentry = NULL;
        MemFree (seqentry);
        BioseqSetFree (bssp);

        sap = NULL;
        if (IS_Bioseq (sep)) {
          bsp = (BioseqPtr) sep->data.ptrvalue;
          ValNodeLink (&(bsp->descr), descr);
          if (bsp->annot == NULL) {
            bsp->annot = annot;
            annot = NULL;
          } else {
            sap = bsp->annot;
          }
        } else if (IS_Bioseq_set (sep)) {
          bssp = (BioseqSetPtr) sep->data.ptrvalue;
          ValNodeLink (&(bssp->descr), descr);
          if (bssp->annot == NULL) {
            bssp->annot = annot;
            annot = NULL;
          } else {
            sap = bssp->annot;
          }
        }
        if (sap != NULL) {
          while (sap->next != NULL) {
            sap = sap->next;
          }
          sap->next = annot;
        }
        DeleteMultipleTitles (sep, NULL, 0, 0);

        if (relink) {
          SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
          RestoreSeqEntryObjMgrData (sep, omdptop, &omdata);
        }
      }
    }
  }
}

NLM_EXTERN ValNodePtr ExtractBioSourceAndPubs (SeqEntryPtr sep)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  ValNodePtr    descr;
  ValNodePtr    last;
  ValNodePtr    nextsdp;
  Pointer PNTR  prevsdp;
  ValNodePtr    sdp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return NULL;
  descr = NULL;
  last = NULL;
  sdp = NULL;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else return NULL;
  while (sdp != NULL) {
    nextsdp = sdp->next;
    if (sdp->choice == Seq_descr_pub || sdp->choice == Seq_descr_source) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      if (descr == NULL) {
        descr = sdp;
        last = descr;
      } else if (last != NULL) {
        last->next = sdp;
        last = last->next;
      }
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
  return descr;
}

NLM_EXTERN void ReplaceBioSourceAndPubs (SeqEntryPtr sep, ValNodePtr descr)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  ValNodePtr    last;
  Pointer PNTR  prevsdp;
  ValNodePtr    sdp;

  if (sep == NULL || descr == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else return;
  last = descr;
  while (last->next != NULL) {
    last = last->next;
  }
  last->next = sdp;
  *(prevsdp) = descr;
}

typedef struct targetdata {
  BioseqPtr    bsp;
  SeqEntryPtr  nps;
  Boolean      skipGenProdSet;
} TargetData, PNTR TargetDataPtr;

static Boolean ReturnStackToItem (GatherContextPtr gcp)

{
  BioseqSetPtr   bssp;
  Int2           i;
  Uint2          itemtype;
  TargetDataPtr  tdp;

  if (gcp == NULL) return TRUE;
  tdp = (TargetDataPtr) gcp->userdata;
  if (tdp == NULL) return TRUE;
  if (gcp->gatherstack != NULL && gcp->numstack > 0) {
    for (i = 0; i < gcp->numstack; i++) {
      itemtype = gcp->gatherstack [i].itemtype;
      if (itemtype == OBJ_BIOSEQ || itemtype == OBJ_BIOSEQSET) {
        tdp->nps = SeqMgrGetSeqEntryForData (gcp->gatherstack [i].thisitem);
        if (gcp->gatherstack [i].itemtype == OBJ_BIOSEQSET) {
          bssp = (BioseqSetPtr) gcp->gatherstack [i].thisitem;
          if (bssp->_class != BioseqseqSet_class_genbank &&
              bssp->_class != BioseqseqSet_class_mut_set &&
              bssp->_class != BioseqseqSet_class_pop_set &&
              bssp->_class != BioseqseqSet_class_phy_set &&
              bssp->_class != BioseqseqSet_class_eco_set &&
              bssp->_class != BioseqseqSet_class_wgs_set &&
              (bssp->_class != BioseqseqSet_class_gen_prod_set ||
	       (! tdp->skipGenProdSet))) {
            return FALSE;
          }
        } else if (gcp->gatherstack [i].itemtype == OBJ_BIOSEQ) {
          return FALSE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean GetStackToTarget (GatherContextPtr gcp)

{
  TargetDataPtr  tdp;

  if (gcp == NULL) return TRUE;
  tdp = (TargetDataPtr) gcp->userdata;
  if (tdp == NULL) return TRUE;
  if (gcp->thistype == OBJ_BIOSEQ) {
    if (tdp->bsp == (BioseqPtr) gcp->thisitem) {
      return ReturnStackToItem (gcp);
    }
  }
  return TRUE;
}

NLM_EXTERN SeqEntryPtr LIBCALL GetBestTopParentForDataEx (Uint2 entityID, BioseqPtr bsp, Boolean skipGenProdSet)

{
  BioseqSetPtr  bssp;
  BioseqSetPtr  parent;
  GatherScope   gs;
  TargetData    td;

  td.bsp = bsp;
  td.nps = NULL;
  td.skipGenProdSet = skipGenProdSet;
  if (entityID > 0 && bsp != NULL) {
    if (bsp->idx.parenttype == OBJ_BIOSEQSET) {
      bssp = (BioseqSetPtr) bsp->idx.parentptr;
      if (bssp != NULL && bssp->_class == BioseqseqSet_class_parts && bssp->idx.parenttype == OBJ_BIOSEQSET) {
        parent = (BioseqSetPtr) bssp->idx.parentptr;
        if (parent != NULL && parent->_class == BioseqseqSet_class_segset) {
          bssp = parent;
        }
      }
      if (bssp != NULL && bssp->_class == BioseqseqSet_class_segset && bssp->idx.parenttype == OBJ_BIOSEQSET) {
        parent = (BioseqSetPtr) bssp->idx.parentptr;
        if (parent != NULL && parent->_class == BioseqseqSet_class_nuc_prot) {
          bssp = parent;
        }
      }
      if (bssp != NULL && bssp->seqentry != NULL) {
        if (bssp->_class == BioseqseqSet_class_nuc_prot ||
            bssp->_class == BioseqseqSet_class_segset ||
            bssp->_class == BioseqseqSet_class_parts) {
          return bssp->seqentry;
        }
      }
      if (bsp->seqentry != NULL) {
        return bsp->seqentry;
      }
    }
    MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
    gs.seglevels = 1;
    MemSet ((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
    gs.ignore[OBJ_BIOSEQ] = FALSE;
    gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
    GatherEntity (entityID, (Pointer) &td, GetStackToTarget, &gs);
  }
  return td.nps;
}

NLM_EXTERN SeqEntryPtr LIBCALL GetBestTopParentForData (Uint2 entityID, BioseqPtr bsp)

{
  return GetBestTopParentForDataEx (entityID, bsp, FALSE);
}

NLM_EXTERN SeqEntryPtr LIBCALL GetBestTopParentForItemIDEx (Uint2 entityID, Uint2 itemID, Uint2 itemtype, Boolean skipGenProdSet)

{
  TargetData  td;

  td.bsp = NULL;
  td.nps = NULL;
  td.skipGenProdSet = skipGenProdSet;
  if (entityID > 0 && itemID > 0 && itemtype > 0) {
    GatherItem (entityID, itemID, itemtype, (Pointer) &td, ReturnStackToItem);
  }
  return td.nps;
}

NLM_EXTERN SeqEntryPtr LIBCALL GetBestTopParentForItemID (Uint2 entityID, Uint2 itemID, Uint2 itemtype)

{
  return GetBestTopParentForItemIDEx (entityID, itemID, itemtype, FALSE);
}

NLM_EXTERN SeqEntryPtr LIBCALL GetTopSeqEntryForEntityID (Uint2 entityID)

{
  ObjMgrDataPtr  omdp;
  SeqSubmitPtr   ssp;

  omdp = ObjMgrGetData (entityID);
  if (omdp != NULL) {
    switch (omdp->datatype) {
      case OBJ_SEQSUB :
        ssp = (SeqSubmitPtr) omdp->dataptr;
        if (ssp != NULL && ssp->datatype == 1) {
          return (SeqEntryPtr) ssp->data;
        }
        break;
      case OBJ_BIOSEQ :
        return (SeqEntryPtr) omdp->choice;
      case OBJ_BIOSEQSET :
        return (SeqEntryPtr) omdp->choice;
      default :
        break;
    }
  }
  return NULL;
}

NLM_EXTERN Boolean CheckSeqLocForPartial (SeqLocPtr location, BoolPtr p5ptr, BoolPtr p3ptr)

{
  SeqLocPtr   firstSlp;
  IntFuzzPtr  ifp;
  SeqLocPtr   lastSlp;
  Boolean     partial5;
  Boolean     partial3;
  SeqIntPtr   sip;
  SeqLocPtr   slp;
  SeqPntPtr   spp;

  partial5 = FALSE;
  partial3 = FALSE;
  if (location != NULL) {
    firstSlp = NULL;
    lastSlp = NULL;
    slp = SeqLocFindNext (location, NULL);
    while (slp != NULL) {
      if (firstSlp == NULL) {
        firstSlp = slp;
      }
      lastSlp = slp;
      slp = SeqLocFindNext (location, slp);
    }
    if (firstSlp != NULL) {
      if (firstSlp->choice == SEQLOC_INT && firstSlp->data.ptrvalue != NULL) {
        sip = (SeqIntPtr) firstSlp->data.ptrvalue;
        if (sip->strand == Seq_strand_minus || sip->strand == Seq_strand_both_rev) {
          ifp = sip->if_to;
          if (ifp != NULL && ifp->choice == 4 && ifp->a == 1) {
            partial5 = TRUE;
          }
        } else {
          ifp = sip->if_from;
          if (ifp != NULL && ifp->choice == 4 && ifp->a == 2) {
            partial5 = TRUE;
          }
        }
      } else if (firstSlp->choice == SEQLOC_PNT && firstSlp->data.ptrvalue != NULL) {
        spp = (SeqPntPtr) firstSlp->data.ptrvalue;
        if (spp->strand == Seq_strand_minus || spp->strand == Seq_strand_both_rev) {
          ifp = spp->fuzz;
          if (ifp != NULL && ifp->choice == 4 && ifp->a == 1) {
            partial5 = TRUE;
          }
        } else {
          ifp = spp->fuzz;
          if (ifp != NULL && ifp->choice == 4 && ifp->a == 2) {
            partial5 = TRUE;
          }
        }
      }
    }
    if (lastSlp != NULL) {
      if (lastSlp->choice == SEQLOC_INT && lastSlp->data.ptrvalue != NULL) {
        sip = (SeqIntPtr) lastSlp->data.ptrvalue;
        if (sip->strand == Seq_strand_minus || sip->strand == Seq_strand_both_rev) {
          ifp = sip->if_from;
          if (ifp != NULL && ifp->choice == 4 && ifp->a == 2) {
            partial3 = TRUE;
          }
        } else {
          ifp = sip->if_to;
          if (ifp != NULL && ifp->choice == 4 && ifp->a == 1) {
            partial3 = TRUE;
          }
        }
      } else if (lastSlp->choice == SEQLOC_PNT && lastSlp->data.ptrvalue != NULL) {
        spp = (SeqPntPtr) lastSlp->data.ptrvalue;
        if (spp->strand == Seq_strand_minus || spp->strand == Seq_strand_both_rev) {
          ifp = spp->fuzz;
          if (ifp != NULL && ifp->choice == 4 && ifp->a == 2) {
            partial3 = TRUE;
          }
        } else {
          ifp = spp->fuzz;
          if (ifp != NULL && ifp->choice == 4 && ifp->a == 1) {
            partial3 = TRUE;
          }
        }
      }
    }
  }
  if (p5ptr != NULL) {
    *p5ptr = partial5;
  }
  if (p3ptr != NULL) {
    *p3ptr = partial3;
  }
  return (Boolean) (partial5 || partial3);
}

NLM_EXTERN void SetSeqLocPartial (SeqLocPtr location, Boolean partial5, Boolean partial3)

{
  SeqLocPtr   firstSlp;
  IntFuzzPtr  ifp;
  SeqLocPtr   lastSlp;
  SeqIntPtr   sip;
  SeqLocPtr   slp;
  SeqPntPtr   spp;

  if (location != NULL) {
    firstSlp = NULL;
    lastSlp = NULL;
    slp = SeqLocFindNext (location, NULL);
    while (slp != NULL) {
      if (firstSlp == NULL) {
        firstSlp = slp;
      }
      lastSlp = slp;
      slp = SeqLocFindNext (location, slp);
    }
    if (firstSlp != NULL) {
      if (firstSlp->choice == SEQLOC_INT && firstSlp->data.ptrvalue != NULL) {
        sip = (SeqIntPtr) firstSlp->data.ptrvalue;
        if (partial5) {
          ifp = IntFuzzNew ();
          if (ifp != NULL) {
            ifp->choice = 4;
            if (sip->strand == Seq_strand_minus || sip->strand == Seq_strand_both_rev) {
              sip->if_to = IntFuzzFree (sip->if_to);
              sip->if_to = ifp;
              ifp->a = 1;
            } else {
              sip->if_from = IntFuzzFree (sip->if_from);
              sip->if_from = ifp;
              ifp->a = 2;
            }
          }
        } else {
          if (sip->strand == Seq_strand_minus || sip->strand == Seq_strand_both_rev) {
            sip->if_to = IntFuzzFree (sip->if_to);
          } else {
            sip->if_from = IntFuzzFree (sip->if_from);
          }
        }
      } else if (firstSlp->choice == SEQLOC_PNT && firstSlp->data.ptrvalue != NULL) {
        spp = (SeqPntPtr) firstSlp->data.ptrvalue;
        if (partial5) {
          ifp = IntFuzzNew ();
          if (ifp != NULL) {
            ifp->choice = 4;
            if (spp->strand == Seq_strand_minus || spp->strand == Seq_strand_both_rev) {
              spp->fuzz = IntFuzzFree (spp->fuzz);
              spp->fuzz = ifp;
              ifp->a = 1;
            } else {
              spp->fuzz = IntFuzzFree (spp->fuzz);
              spp->fuzz = ifp;
              ifp->a = 2;
            }
          }
        } else {
          if (spp->strand == Seq_strand_minus || spp->strand == Seq_strand_both_rev) {
            spp->fuzz = IntFuzzFree (spp->fuzz);
          } else {
            spp->fuzz = IntFuzzFree (spp->fuzz);
          }
        }
      }
    }
    if (lastSlp != NULL) {
      if (lastSlp->choice == SEQLOC_INT && lastSlp->data.ptrvalue != NULL) {
        sip = (SeqIntPtr) lastSlp->data.ptrvalue;
        if (partial3) {
          ifp = IntFuzzNew ();
          if (ifp != NULL) {
            ifp->choice = 4;
            if (sip->strand == Seq_strand_minus || sip->strand == Seq_strand_both_rev) {
              sip->if_from = IntFuzzFree (sip->if_from);
              sip->if_from = ifp;
              ifp->a = 2;
            } else {
              sip->if_to = IntFuzzFree (sip->if_to);
              sip->if_to = ifp;
              ifp->a = 1;
            }
          }
        } else {
          if (sip->strand == Seq_strand_minus || sip->strand == Seq_strand_both_rev) {
            sip->if_from = IntFuzzFree (sip->if_from);
          } else {
            sip->if_to = IntFuzzFree (sip->if_to);
          }
        }
      } else if (lastSlp->choice == SEQLOC_PNT && lastSlp->data.ptrvalue != NULL) {
        spp = (SeqPntPtr) lastSlp->data.ptrvalue;
        if (partial3) {
          ifp = IntFuzzNew ();
          if (ifp != NULL) {
            ifp->choice = 4;
            if (spp->strand == Seq_strand_minus || spp->strand == Seq_strand_both_rev) {
              spp->fuzz = IntFuzzFree (spp->fuzz);
              spp->fuzz = ifp;
              ifp->a = 2;
            } else {
              spp->fuzz = IntFuzzFree (spp->fuzz);
              spp->fuzz = ifp;
              ifp->a = 1;
            }
          }
        } else {
          if (spp->strand == Seq_strand_minus || spp->strand == Seq_strand_both_rev) {
            spp->fuzz = IntFuzzFree (spp->fuzz);
          } else {
            spp->fuzz = IntFuzzFree (spp->fuzz);
          }
        }
      }
    }
  }
}

/* KeyTag section */

NLM_EXTERN int LIBCALLBACK SortVnpByString (VoidPtr ptr1, VoidPtr ptr2)

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
      }
    }
  }
  return 0;
}

NLM_EXTERN ValNodePtr UniqueValNode (ValNodePtr list)

{
  CharPtr       last;
  ValNodePtr    next;
  Pointer PNTR  prev;
  CharPtr       str;
  ValNodePtr    vnp;

  if (list == NULL) return NULL;
  last = (CharPtr) list->data.ptrvalue;
  vnp = list->next;
  prev = (Pointer PNTR) &(list->next);
  while (vnp != NULL) {
    next = vnp->next;
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringICmp (last, str) == 0) {
      vnp->next = NULL;
      *prev = next;
      ValNodeFreeData (vnp);
    } else {
      last = (CharPtr) vnp->data.ptrvalue;
      prev = (Pointer PNTR) &(vnp->next);
    }
    vnp = next;
  }

  return list;
}

NLM_EXTERN void KeyTagInit (KeyTag PNTR ktp, ValNodePtr list)

{
  Int2          i;
  CharPtr PNTR  index;
  Int2          num;
  ValNodePtr    vnp;

  if (ktp == NULL || list == NULL) return;
  list = ValNodeSort (list, SortVnpByString);
  list = UniqueValNode (list);
  num = ValNodeLen (list);
  index = MemNew (sizeof (CharPtr) * (num + 1));

  for (vnp = list, i = 0; vnp != NULL && i < num; vnp = vnp->next, i++) {
    index [i] = (CharPtr) vnp->data.ptrvalue;
  }

  ktp->num = num;
  ktp->list = list;
  ktp->index = index;
}

NLM_EXTERN void KeyTagClear (KeyTag PNTR ktp)

{
  if (ktp == NULL) return;
  ktp->num = 0;
  ktp->list = ValNodeFreeData (ktp->list);
  ktp->index = MemFree (ktp->index);
}

NLM_EXTERN Int2 KeyFromTag (KeyTag PNTR ktp, CharPtr tag)

{
  Int2  L, R, mid, compare;

  if (ktp == NULL || ktp->list == NULL || ktp->index == NULL) return 0;
  if (tag == NULL) return 0;

  L = 0;
  R = ktp->num - 1;
  while (L < R) {
    mid = (L + R) / 2;
    compare = StringICmp (ktp->index [mid], tag);
    if (compare < 0) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }
  if (StringICmp (ktp->index [R], tag) == 0) {
    return (R + 1);
  }

  return 0;
}

NLM_EXTERN CharPtr TagFromKey (KeyTag PNTR ktp, Int2 key)

{
  if (ktp == NULL || ktp->list == NULL || ktp->index == NULL) return 0;
  if (key < 1 || key > ktp->num) return 0;
  key--;
  return ktp->index [key];
}

/* begin PromoteXrefs section */

typedef struct geneextendlist {
  GeneRefPtr  grp;
  SeqLocPtr   slp;
  ObjMgrPtr   omp;
  Boolean     rsult;
  Char        label [41];
} GeneExtendList, PNTR GeneExtendPtr;

static Boolean GeneExtendFunc (GatherContextPtr gcp)

{
  BioseqPtr      bsp;
  GeneExtendPtr  gep;
  GeneRefPtr     grp;
  Boolean        hasNulls;
  ObjMgrTypePtr  omtp;
  SeqFeatPtr     sfp;
  SeqLocPtr      slp;
  Char           thislabel [41];

  if (gcp == NULL) return TRUE;

  gep = (GeneExtendPtr) gcp->userdata;
  if (gep == NULL ) return TRUE;

  thislabel [0] = '\0';

  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_GENE && sfp->data.value.ptrvalue != NULL) {
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      omtp = ObjMgrTypeFind (gep->omp, gcp->thistype, NULL, NULL);
      if (omtp == NULL) {
        return TRUE;
      }
      if (omtp->labelfunc != NULL) {
        (*(omtp->labelfunc)) (gcp->thisitem, thislabel, 40, OM_LABEL_CONTENT);
      }
      if (thislabel [0] != '\0') {
        if (StringICmp (thislabel, gep->label) == 0) {
          if (SeqLocCompare (gep->slp, sfp->location) != SLC_NO_MATCH) {
            bsp = GetBioseqGivenSeqLoc (sfp->location, gcp->entityID);
            if (bsp != NULL) {
              slp = SeqLocMerge (bsp, sfp->location, gep->slp, TRUE, FALSE, FALSE);
              if (slp != NULL) {
                sfp->location = SeqLocFree (sfp->location);
                sfp->location = slp;
                if (bsp->repr == Seq_repr_seg) {
                  slp = SegLocToPartsEx (bsp, sfp->location, TRUE);
                  sfp->location = SeqLocFree (sfp->location);
                  sfp->location = slp;
                  hasNulls = LocationHasNullsBetween (sfp->location);
                  sfp->partial = (sfp->partial || hasNulls);
                }
                FreeAllFuzz (slp);
                gep->rsult = TRUE;
              }
            }
          }
          return FALSE;
        }
      }
    }
  }
  return TRUE;
}

static Boolean ExtendGene (GeneRefPtr grp, SeqEntryPtr nsep, SeqLocPtr slp)

{
  GeneExtendList  gel;
  GatherScope     gs;
  ObjMgrTypePtr   omtp;
  SeqFeatPtr      sfp;

  if (grp == NULL || nsep == NULL || slp == NULL) return FALSE;
  gel.grp = grp;
  gel.slp = slp;
  gel.omp = ObjMgrGet ();
  gel.label [0] = '\0';
  gel.rsult = FALSE;
  omtp = ObjMgrTypeFind (gel.omp, OBJ_SEQFEAT, NULL, NULL);
  if (omtp != NULL && omtp->labelfunc != NULL) {
    sfp = SeqFeatNew ();
    if (sfp != NULL) {
      sfp->data.choice = SEQFEAT_GENE;
      sfp->data.value.ptrvalue = (Pointer) grp;
      (*(omtp->labelfunc)) ((Pointer) sfp, gel.label, 40, OM_LABEL_CONTENT);
      sfp->data.value.ptrvalue = NULL;
      SeqFeatFree (sfp);
    }
  }
  MemSet ((Pointer)(&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherSeqEntry (nsep, (Pointer) &gel, GeneExtendFunc, &gs);
  return gel.rsult;
}

NLM_EXTERN void SetEmptyGeneticCodes (SeqAnnotPtr sap, Int2 genCode)

{
  CdRegionPtr     crp;
  GeneticCodePtr  gc;
  SeqFeatPtr      sfp;
  ValNodePtr      vnp;

  if (sap == NULL || sap->type != 1) return;
  for (sfp = (SeqFeatPtr) sap->data; sfp != NULL; sfp = sfp->next) {
    if (sfp->data.choice == SEQFEAT_CDREGION) {
      crp = (CdRegionPtr) sfp->data.value.ptrvalue;
      if (crp != NULL) {
        gc = crp->genetic_code;
        if (gc != NULL) {
          vnp = gc->data.ptrvalue;
          if (vnp != NULL && vnp->choice == 2) {
            vnp->data.intvalue = (Int4) genCode;
            /*
            if (vnp->data.intvalue == 0) {
              vnp->data.intvalue = (Int4) genCode;
            }
            */
          }
        }
      }
    }
  }
}

NLM_EXTERN void PromoteXrefsEx (SeqFeatPtr sfp, BioseqPtr bsp, Uint2 entityID, Boolean include_stop, Boolean remove_trailingX, Boolean gen_prod_set)

{
  ByteStorePtr         bs;
  BioseqSetPtr         bssp;
  Char                 ch;
  CdRegionPtr          crp;
  Int2                 ctr = 1;
  ValNodePtr           descr;
  SeqFeatPtr           first;
  GBQualPtr            gbq;
  Int4                 i;
  Char                 id [64];
  SeqEntryPtr          last;
  BioseqPtr            mbsp;
  MolInfoPtr           mip;
  SeqEntryPtr          msep;
  SeqFeatXrefPtr       next;
  GBQualPtr            nextqual;
  SeqEntryPtr          old;
  ObjMgrDataPtr        omdptop;
  ObjMgrData           omdata;
  Uint2                parenttype;
  Pointer              parentptr;
  Boolean              partial5;
  Boolean              partial3;
  BioseqPtr            pbsp;
  SeqFeatXrefPtr PNTR  prev;
  GBQualPtr PNTR       prevqual;
  SeqFeatPtr           prot;
  CharPtr              protseq;
  ProtRefPtr           prp;
  SeqEntryPtr          psep;
  CharPtr              ptr;
  CharPtr              rnaseq;
  SeqEntryPtr          sep;
  SeqHistPtr           shp;
  SeqIdPtr             sip;
  SeqEntryPtr          target = NULL;
  Uint4                version = 0;
  long int             val;
  ValNodePtr           vnp;
  SeqFeatXrefPtr       xref;
  /*
  DbtagPtr             dbt;
  SeqFeatPtr           gene;
  GeneRefPtr           grp;
  */

  if (sfp == NULL || bsp == NULL) return;

  /* set subtypes, used to find mRNA features for genomic product sets */

  first = sfp;
  while (sfp != NULL) {
    if (sfp->idx.subtype == 0) {
      sfp->idx.subtype = FindFeatDefType (sfp);
    }
    sfp = sfp->next;
  }

  /* no longer expand genes specified by qualifiers on other features (except repeat_region) */

  /*
  sfp = first;
  while (sfp != NULL) {
    prev = &(sfp->xref);
    xref = sfp->xref;
    while (xref != NULL) {
      next = xref->next;
      if (xref->data.choice == SEQFEAT_GENE &&
          sfp->data.choice != SEQFEAT_GENE &&
          sfp->idx.subtype != FEATDEF_repeat_region) {
        grp = (GeneRefPtr) xref->data.value.ptrvalue;
        if (grp != NULL && SeqMgrGeneIsSuppressed (grp)) {
        } else {
          xref->data.value.ptrvalue = NULL;
          if (grp != NULL) {
            sep = SeqMgrGetSeqEntryForData (bsp);
            if (ExtendGene (grp, sep, sfp->location)) {
              GeneRefFree (grp);
            } else {
              gene = CreateNewFeature (sep, NULL, SEQFEAT_GENE, NULL);
              if (gene != NULL) {
                gene->data.value.ptrvalue = (Pointer) grp;
                gene->location = SeqLocFree (gene->location);
                gene->location = AsnIoMemCopy (sfp->location,
                                               (AsnReadFunc) SeqLocAsnRead,
                                               (AsnWriteFunc) SeqLocAsnWrite);
                for (vnp = sfp->dbxref; vnp != NULL; vnp = vnp->next) {
                  dbt = (DbtagPtr) vnp->data.ptrvalue;
                  if (dbt == NULL) continue;
                  ValNodeAddPointer (&(gene->dbxref), 0, (Pointer) DbtagDup (dbt));
                }
              }
            }
          }
          *(prev) = next;
          xref->next = NULL;
          xref->data.choice = 0;
          SeqFeatXrefFree (xref);
        }
      } else {
        prev = &(xref->next);
      }
      xref = next;
    }
    sfp = sfp->next;
  }
  */

  /* expand mRNA features into cDNA product sequences */

  bssp = NULL;
  sep = NULL;
  last = NULL;
  if (gen_prod_set) {
    sep = GetTopSeqEntryForEntityID (entityID);
    if (IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp != NULL && bssp->seq_set != NULL) {
        last = bssp->seq_set;
        while (last->next != NULL) {
          last = last->next;
        }
      }
    }
  }
  if (gen_prod_set && sep != NULL && bssp != NULL && last != NULL) {
    target = sep;
    SaveSeqEntryObjMgrData (target, &omdptop, &omdata);
    GetSeqEntryParent (target, &parentptr, &parenttype);
    sfp = first;
    while (sfp != NULL) {
      if (sfp->data.choice == SEQFEAT_RNA && sfp->product == NULL) {
        gbq = sfp->qual;
        prevqual = (GBQualPtr PNTR) &(sfp->qual);
        id [0] = '\0';
        sip = NULL;
        while (gbq != NULL) {
          nextqual = gbq->next;
          if (StringICmp (gbq->qual, "transcript_id") == 0) {
            if (StringDoesHaveText (id) && StringDoesHaveText (gbq->val)) {
              ErrPostEx (SEV_WARNING, ERR_FEATURE_QualWrongThisFeat,
                         "RNA transcript_id %s replacing %s", gbq->val, id);
            }
            *(prevqual) = gbq->next;
            gbq->next = NULL;
            StringNCpy_0 (id, gbq->val, sizeof (id));
            GBQualFree (gbq);
          } else {
            prevqual = (GBQualPtr PNTR) &(gbq->next);
          }
          gbq = nextqual;
        }
        if (! StringHasNoText (id)) {
          if (StringChr (id, '|') != NULL) {
            sip = SeqIdParse (id);
          } else {
            ptr = StringChr (id, '.');
            if (ptr != NULL) {
              *ptr = '\0';
              ptr++;
              if (sscanf (ptr, "%ld", &val) == 1) {
                version = (Uint4) val;
              }
            }
            sip = SeqIdFromAccession (id, version, NULL);
          }
        }
        if (sip != NULL || sfp->idx.subtype == FEATDEF_mRNA) {
          rnaseq = GetSequenceByFeature (sfp);
          if (rnaseq != NULL) {
            i = (Int4) StringLen (rnaseq);
            bs = BSNew (i + 2);
            if (bs != NULL) {
              BSWrite (bs, (VoidPtr) rnaseq, (Int4) StringLen (rnaseq));
              mbsp = BioseqNew ();
              if (mbsp != NULL) {
                mbsp->repr = Seq_repr_raw;
                mbsp->mol = Seq_mol_rna;
                mbsp->seq_data_type = Seq_code_iupacna;
                mbsp->seq_data = bs;
                mbsp->length = BSLen (bs);
                BioseqPack (mbsp);
                bs = NULL;
                /*
                sep = GetTopSeqEntryForEntityID (entityID);
                */
                old = SeqEntrySetScope (sep);
                if (sip != NULL) {
                  mbsp->id = sip;
                } else if (sfp->idx.subtype == FEATDEF_mRNA) {
                  /* actually just making rapid unique ID for mRNA */
                  mbsp->id = MakeNewProteinSeqIdEx (sfp->location, NULL, NULL, &ctr);
                }
                CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
                SeqMgrAddToBioseqIndex (mbsp);
                SeqEntrySetScope (old);
                msep = SeqEntryNew ();
                if (msep != NULL) {
                  msep->choice = 1;
                  msep->data.ptrvalue = (Pointer) mbsp;
                  SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) mbsp, msep);
                  mip = MolInfoNew ();
                  if (mip != NULL) {
                    switch (sfp->idx.subtype) {
                      case FEATDEF_preRNA :
                        mip->biomol = MOLECULE_TYPE_PRE_MRNA;
                        break;
                      case FEATDEF_mRNA :
                        mip->biomol = MOLECULE_TYPE_MRNA;
                        break;
                      case FEATDEF_tRNA :
                        mip->biomol = MOLECULE_TYPE_TRNA;
                        break;
                      case FEATDEF_rRNA :
                        mip->biomol = MOLECULE_TYPE_RRNA;
                        break;
                      case FEATDEF_snRNA :
                        mip->biomol = MOLECULE_TYPE_SNRNA;
                        break;
                      case FEATDEF_scRNA :
                        mip->biomol = MOLECULE_TYPE_SCRNA;
                        break;
                      case FEATDEF_otherRNA :
                        mip->biomol = MOLECULE_TYPE_TRANSCRIBED_RNA;
                        break;
                      case FEATDEF_snoRNA :
                        mip->biomol = MOLECULE_TYPE_SNORNA;
                        break;
                      default :
                        mip->biomol = 0;
                        break;
                    }
                    if (partial5 && partial3) {
                      mip->completeness = 5;
                    } else if (partial5) {
                      mip->completeness = 3;
                    } else if (partial3) {
                      mip->completeness = 4;
                    }
                    vnp = CreateNewDescriptor (msep, Seq_descr_molinfo);
                    if (vnp != NULL) {
                      vnp->data.ptrvalue = (Pointer) mip;
                    }
                  }
                  /* add mRNA sequence to genomic product set */
                  last->next = msep;
                  last = msep;
                  SetSeqFeatProduct (sfp, mbsp);
                }
              }
            }
            rnaseq = MemFree (rnaseq);
          }
        }
      }
      sfp = sfp->next;
    }
    SeqMgrLinkSeqEntry (target, parenttype, parentptr);
    RestoreSeqEntryObjMgrData (target, omdptop, &omdata);
  }

  /* expand coding region features into protein product sequences */

  last = NULL;
  sfp = first;
  while (sfp != NULL) {
    prev = &(sfp->xref);
    xref = sfp->xref;
    while (xref != NULL) {
      next = xref->next;
      if (xref->data.choice == SEQFEAT_PROT &&
          sfp->data.choice == SEQFEAT_CDREGION &&
          sfp->product == NULL) {
        prp = (ProtRefPtr) xref->data.value.ptrvalue;
        xref->data.value.ptrvalue = NULL;
        if (prp != NULL) {
          crp = (CdRegionPtr) sfp->data.value.ptrvalue;
          if (crp != NULL) {
/**
            crp->frame = 0;
**/
            bs = ProteinFromCdRegionEx (sfp, include_stop, remove_trailingX);
            if (bs != NULL) {
              protseq = BSMerge (bs, NULL);
              bs = BSFree (bs);
              if (protseq != NULL) {
                ptr = protseq;
                ch = *ptr;
                while (ch != '\0') {
                  *ptr = TO_UPPER (ch);
                  ptr++;
                  ch = *ptr;
                }
                i = (Int4) StringLen (protseq);
                if (i > 0 && protseq [i - 1] == '*') {
                  protseq [i - 1] = '\0';
                }
                bs = BSNew (i + 2);
                if (bs != NULL) {
                  ptr = protseq;
                  /*
                  if (protseq [0] == '-') {
                    ptr++;
                  }
                  */
                  BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
                }
                protseq = MemFree (protseq);
              }
              pbsp = BioseqNew ();
              if (pbsp != NULL) {
                pbsp->repr = Seq_repr_raw;
                pbsp->mol = Seq_mol_aa;
                pbsp->seq_data_type = Seq_code_ncbieaa;
                pbsp->seq_data = bs;
                pbsp->length = BSLen (bs);
                bs = NULL;
                sep = NULL;
                mbsp = NULL;
                if (gen_prod_set) {
                  gbq = sfp->qual;
                  prevqual = (GBQualPtr PNTR) &(sfp->qual);
                  id [0] = '\0';
                  sip = NULL;
                  while (gbq != NULL) {
                    nextqual = gbq->next;
                    if (StringICmp (gbq->qual, "transcript_id") == 0) {
                      if (StringDoesHaveText (id) && StringDoesHaveText (gbq->val)) {
                        ErrPostEx (SEV_WARNING, ERR_FEATURE_QualWrongThisFeat,
                                   "CDS transcript_id %s replacing %s", gbq->val, id);
                      }
                      *(prevqual) = gbq->next;
                      gbq->next = NULL;
                      StringNCpy_0 (id, gbq->val, sizeof (id));
                      GBQualFree (gbq);
                    } else if (StringICmp (gbq->qual, "secondary_accession") == 0) {
                      *(prevqual) = gbq->next;
                      gbq->next = NULL;
                      shp = ParseStringIntoSeqHist (NULL, gbq->val);
                      if (shp != NULL) {
                        pbsp->hist = shp;
                      }
                      GBQualFree (gbq);
                    } else {
                      prevqual = (GBQualPtr PNTR) &(gbq->next);
                    }
                    gbq = nextqual;
                  }
                  if (! StringHasNoText (id)) {
                    if (StringChr (id, '|') != NULL) {
                      sip = SeqIdParse (id);
                    } else {
                      ptr = StringChr (id, '.');
                      if (ptr != NULL) {
                        *ptr = '\0';
                        ptr++;
                        if (sscanf (ptr, "%ld", &val) == 1) {
                          version = (Uint4) val;
                        }
                      }
                      sip = SeqIdFromAccession (id, version, NULL);
                    }
                  }
                  mbsp = BioseqFind (sip);
                  SeqIdFree (sip);
                  if (mbsp != NULL) {
                    sep = SeqMgrGetSeqEntryForData (mbsp);
                  /*
                  } else {
                    sep = GetBestTopParentForDataEx (entityID, bsp, TRUE);
                  */
                  }
                } else {
                  sep = GetBestTopParentForData (entityID, bsp);
                }
                old = SeqEntrySetScope (sep);
                gbq = sfp->qual;
                prevqual = (GBQualPtr PNTR) &(sfp->qual);
                id [0] = '\0';
                sip = NULL;
                while (gbq != NULL) {
                  nextqual = gbq->next;
                  if (StringICmp (gbq->qual, "protein_id") == 0) {
                    if (StringDoesHaveText (id) && StringDoesHaveText (gbq->val)) {
                              ErrPostEx (SEV_WARNING, ERR_FEATURE_QualWrongThisFeat,
                         "CDS protein_id %s replacing %s", gbq->val, id);
                    }
                    *(prevqual) = gbq->next;
                    gbq->next = NULL;
                    StringNCpy_0 (id, gbq->val, sizeof (id));
                    GBQualFree (gbq);
                  } else {
                    prevqual = (GBQualPtr PNTR) &(gbq->next);
                  }
                  gbq = nextqual;
                }
                if (! StringHasNoText (id)) {
                  if (StringChr (id, '|') != NULL) {
                    sip = SeqIdParse (id);
                  } else {
                    ptr = StringChr (id, '.');
                    if (ptr != NULL) {
                      *ptr = '\0';
                      ptr++;
                      if (sscanf (ptr, "%ld", &val) == 1) {
                        version = (Uint4) val;
                      }
                    }
                    sip = SeqIdFromAccession (id, version, NULL);
                  }
                }
                if (sip != NULL) {
                  pbsp->id = sip;
                } else {
                  pbsp->id = MakeNewProteinSeqIdEx (sfp->location, NULL, NULL, &ctr);
                }
                CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
                SeqMgrAddToBioseqIndex (pbsp);
                SeqEntrySetScope (old);
                psep = SeqEntryNew ();
                if (psep != NULL) {
                  psep->choice = 1;
                  psep->data.ptrvalue = (Pointer) pbsp;
                  SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) pbsp, psep);
                  mip = MolInfoNew ();
                  if (mip != NULL) {
                    mip->biomol = 8;
                    mip->tech = 8;
                    if (partial5 && partial3) {
                      mip->completeness = 5;
                    } else if (partial5) {
                      mip->completeness = 3;
                    } else if (partial3) {
                      mip->completeness = 4;
                    }
                    vnp = CreateNewDescriptor (psep, Seq_descr_molinfo);
                    if (vnp != NULL) {
                      vnp->data.ptrvalue = (Pointer) mip;
                    }
                  }
                  /* the first protein may change the set/seq structure,
                  so goes through AddSeqEntryToSeqEntry */

                  if (gen_prod_set || last == NULL) {
                    descr = ExtractBioSourceAndPubs (sep);
                    AddSeqEntryToSeqEntry (sep, psep, TRUE);
                    ReplaceBioSourceAndPubs (sep, descr);
                    last = psep;
                  } else {
                    last->next = psep;
                    last = psep;
                  }
                  if (target == NULL) {
                    target = sep;
                    SaveSeqEntryObjMgrData (target, &omdptop, &omdata);
                    GetSeqEntryParent (target, &parentptr, &parenttype);
                  }
                  SetSeqFeatProduct (sfp, pbsp);
                  psep = SeqMgrGetSeqEntryForData (pbsp);
                  if (psep != NULL) {
                    last = psep;
                    prot = CreateNewFeature (psep, NULL, SEQFEAT_PROT, NULL);
                    if (prot != NULL) {
                      prot->data.value.ptrvalue = (Pointer) prp;
                      SetSeqLocPartial (prot->location, partial5, partial3);
                      prot->partial = (Boolean) (partial5 || partial3);
                    }
                  }
                }
              }
            }
          }
        }
        *(prev) = next;
        xref->next = NULL;
        xref->data.choice = 0;
        SeqFeatXrefFree (xref);
      } else {
        prev = &(xref->next);
      }
      xref = next;
    }
    sfp = sfp->next;
  }
  if (target != NULL) {
    SeqMgrLinkSeqEntry (target, parenttype, parentptr);
    RestoreSeqEntryObjMgrData (target, omdptop, &omdata);
  }
}

NLM_EXTERN void PromoteXrefs (SeqFeatPtr sfp, BioseqPtr bsp, Uint2 entityID)

{
  PromoteXrefsEx (sfp, bsp, entityID, TRUE, FALSE, FALSE);
}

/* begin BasicSeqEntryCleanup section */

static Boolean HasNoText (CharPtr str)

{
  Uchar  ch;	/* to use 8bit characters in multibyte languages */

  if (str != NULL) {
    ch = *str;
    while (ch != '\0') {
      if (ch > ' ') {
        return FALSE;
      }
      str++;
      ch = *str;
    }
  }
  return TRUE;
}

NLM_EXTERN CharPtr TrimSpacesAndSemicolons (CharPtr str)

{
  CharPtr  amp;
  Uchar    ch;	/* to use 8bit characters in multibyte languages */
  CharPtr  dst;
  CharPtr  ptr;

  if (str != NULL && str [0] != '\0') {
    dst = str;
    ptr = str;
    ch = *ptr;
    if (ch != '\0' && (ch <= ' ' || ch == ';')) {
      while (ch != '\0' && (ch <= ' ' || ch == ';')) {
        ptr++;
        ch = *ptr;
      }
      while (ch != '\0') {
        *dst = ch;
        dst++;
        ptr++;
        ch = *ptr;
      }
      *dst = '\0';
    }
    amp = NULL;
    dst = NULL;
    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      if (ch == '&') {
        amp = ptr;
        dst = NULL;
      } else if (ch <= ' ') {
        if (dst == NULL) {
          dst = ptr;
        }
        amp = NULL;
      } else if (ch == ';') {
        if (dst == NULL && amp == NULL) {
          dst = ptr;
        }
      } else {
        dst = NULL;
      }
      ptr++;
      ch = *ptr;
    }
    if (dst != NULL) {
      *dst = '\0';
    }
  }
  return str;
}

NLM_EXTERN CharPtr TrimSpacesAndJunkFromEnds (
  CharPtr str,
  Boolean allowEllipsis
)

{
  Uchar    ch;	/* to use 8bit characters in multibyte languages */
  CharPtr  dst;
  Boolean  isPeriod;
  CharPtr  ptr;

  if (str != NULL && str [0] != '\0') {
    dst = str;
    ptr = str;
    ch = *ptr;
    if (ch != '\0' && ch <= ' ') {
      while (ch != '\0' && ch <= ' ') {
        ptr++;
        ch = *ptr;
      }
      while (ch != '\0') {
        *dst = ch;
        dst++;
        ptr++;
        ch = *ptr;
      }
      *dst = '\0';
    }
    dst = NULL;
    ptr = str;
    ch = *ptr;
    isPeriod = FALSE;
    while (ch != '\0') {
      if (ch <= ' ' || ch == '.' || ch == ',' || ch == '~' || ch == ';') {
        if (dst == NULL) {
          dst = ptr;
        }
        isPeriod = (Boolean) (isPeriod || ch == '.');
      } else {
        dst = NULL;
        isPeriod = FALSE;
      }
      ptr++;
      ch = *ptr;
    }
    if (dst != NULL) {
      /* allow one period at end */
      if (isPeriod) {
        *dst = '.';
        dst++;
        /* ellipsis are now okay */
        if (allowEllipsis && *dst == '.' && dst [1] == '.') {
          dst += 2;
        }
      }
      *dst = '\0';
    }
  }
  return str;
}

static void CleanVisString (CharPtr PNTR strp)

{
  if (strp == NULL) return;
  if (*strp == NULL) return;
  TrimSpacesAndSemicolons (*strp);
  if (HasNoText (*strp)) {
    *strp = MemFree (*strp);
  }
}

static void CleanVisStringJunk (CharPtr PNTR strp)

{
  if (strp == NULL) return;
  if (*strp == NULL) return;
  TrimSpacesAndJunkFromEnds (*strp, TRUE);
  if (HasNoText (*strp)) {
    *strp = MemFree (*strp);
  }
}

static void CleanDoubleQuote (CharPtr str)

{
  Char  ch;

  if (str == NULL) return;
  ch = *str;
  while (ch != '\0') {
    if (ch == '"') {
      *str = '\'';
    }
    str++;
    ch = *str;
  }
}

static CharPtr RemoveSpacesBetweenTildes (CharPtr str)

{
  Char     ch;
  CharPtr  dst;
 CharPtr  ptr;
  CharPtr  tmp;

  if (str == NULL || str [0] == '\0') return str;

  dst = str;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    *dst = ch;
    dst++;
    ptr++;
    if (ch == '~') {
      tmp = ptr;
      ch = *tmp;
      while (ch != 0 && ch <= ' ') {
        tmp++;
        ch = *tmp;
      }
      if (ch == '~') {
        ptr = tmp;
      }
    }
    ch = *ptr;
  }
  *dst = '\0';

  return str;
}

static Boolean AlreadyInVnpList (ValNodePtr head, ValNodePtr curr)

{
  if (head == NULL || curr == NULL) return FALSE;
  /* since we cannot sort these lists, must check against all previous entries */
  while (head != curr && head != NULL) {
    if (StringICmp (head->data.ptrvalue, curr->data.ptrvalue) == 0) return TRUE;
    head = head->next;
  }
  return FALSE;
}

static void CleanVisStringList (ValNodePtr PNTR vnpp)

{
  ValNodePtr       next;
  ValNodePtr PNTR  prev;
  ValNodePtr       vnp;

  if (vnpp == NULL) return;
  prev = vnpp;
  vnp = *vnpp;
  while (vnp != NULL) {
    next = vnp->next;
    TrimSpacesAndSemicolons (vnp->data.ptrvalue);
    if (HasNoText (vnp->data.ptrvalue) || AlreadyInVnpList (*vnpp, vnp)) {
      *prev = vnp->next;
      vnp->next = NULL;
      ValNodeFreeData (vnp);
    } else {
      prev = &(vnp->next);
    }
    vnp = next;
  }
}

static Boolean AlreadyInVnpListCaseSensitive (ValNodePtr head, ValNodePtr curr)

{
  if (head == NULL || curr == NULL) return FALSE;
  /* since we cannot sort these lists, must check against all previous entries */
  while (head != curr && head != NULL) {
    if (StringCmp (head->data.ptrvalue, curr->data.ptrvalue) == 0) return TRUE;
    head = head->next;
  }
  return FALSE;
}

static void CleanVisStringListCaseSensitive (ValNodePtr PNTR vnpp)

{
  ValNodePtr       next;
  ValNodePtr PNTR  prev;
  ValNodePtr       vnp;

  if (vnpp == NULL) return;
  prev = vnpp;
  vnp = *vnpp;
  while (vnp != NULL) {
    next = vnp->next;
    TrimSpacesAndSemicolons (vnp->data.ptrvalue);
    if (HasNoText (vnp->data.ptrvalue) || AlreadyInVnpListCaseSensitive (*vnpp, vnp)) {
      *prev = vnp->next;
      vnp->next = NULL;
      ValNodeFreeData (vnp);
    } else {
      prev = &(vnp->next);
    }
    vnp = next;
  }
}

static void CleanDoubleQuoteList (ValNodePtr vnp)

{
  while (vnp != NULL) {
    CleanDoubleQuote ((CharPtr) vnp->data.ptrvalue);
    vnp = vnp->next;
  }
}

static Boolean HandledGBQualOnGene (SeqFeatPtr sfp, GBQualPtr gbq)

{
  Int2        choice = 0;
  GeneRefPtr  grp;

  if (StringICmp (gbq->qual, "pseudo") == 0) {
    choice = 1;
  } else if (StringICmp (gbq->qual, "map") == 0) {
    choice = 2;
  } else if (StringICmp (gbq->qual, "allele") == 0) {
    choice = 3;
  } else if (StringICmp (gbq->qual, "locus_tag") == 0) {
    choice = 4;
  }
  if (choice > 0) {
    grp = (GeneRefPtr) sfp->data.value.ptrvalue;
    if (grp == NULL) return FALSE;
    switch (choice) {
      case 1 :
        grp->pseudo = TRUE;
        break;
      case 2 :
        if (grp->maploc != NULL) return FALSE;
        if (StringHasNoText (gbq->val)) return FALSE;
        grp->maploc = StringSave (gbq->val);
        break;
      case 3 :
        if (grp->allele != NULL) return FALSE;
        if (StringHasNoText (gbq->val)) return FALSE;
        grp->allele = StringSave (gbq->val);
        break;
      case 4 :
        if (grp->locus_tag != NULL) return FALSE;
        if (StringHasNoText (gbq->val)) return FALSE;
        grp->locus_tag = StringSave (gbq->val);
        break;
      default :
        break;
    }
    return TRUE;
  }
  return FALSE;
}

/* code break parser functions from the flatfile parser */

static Uint1 GetQualValueAa (CharPtr qval)

{
   CharPtr  str, eptr, ptr;
   Uint1    aa;

	str = StringStr(qval, "aa:");
	if (str != NULL) {
	    str += 3;
	} else {
        ErrPostEx (SEV_WARNING, ERR_QUALIFIER_InvalidDataFormat,
                   "bad transl_except %s", qval);
    	str = StringStr(qval, ",");
    	if (str != NULL) {
    	    str = StringStr(str, ":");
    	    if (str != NULL) {
              str++;
    	    }
    	}
	}

    if (str == NULL) return (Uint1) 'X';

   	while (*str == ' ')
       	++str;
   	for (eptr = str; *eptr != ')' && *eptr != ' ' && *eptr != '\0';  eptr++) continue;

    ptr = TextSave(str, eptr-str);
    aa = ValidAminoAcid(ptr);
    MemFree(ptr);  

    return (aa);
}

static CharPtr SimpleValuePos (CharPtr qval)

{
   CharPtr bptr, eptr;

   if ((bptr = StringStr(qval, "(pos:")) == NULL) {
   		return NULL;
   }
    
   bptr += 5;
   while (*bptr == ' ')
       ++bptr;
   for (eptr = bptr; *eptr != ',' && *eptr != '\0'; eptr++) continue;

   return (TextSave(bptr, eptr-bptr));
}

extern Boolean ParseAnticodon (SeqFeatPtr sfp, CharPtr val, Int4 offset);
extern Boolean ParseAnticodon (SeqFeatPtr sfp, CharPtr val, Int4 offset)

{
  Int4       diff;
  Boolean    locmap;
  int        num_errs;
  CharPtr    pos;
  Boolean    pos_range = FALSE;
  RnaRefPtr  rrp;
  SeqIntPtr  sintp;
  SeqIdPtr   sip;
  Boolean    sitesmap;
  SeqLocPtr  slp;
  SeqPntPtr  spp;
  Uint1      strand;
  Int4       temp;
  tRNAPtr    trp;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA) return FALSE;
  if (StringHasNoText (val)) return FALSE;

  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
  if (rrp == NULL || rrp->ext.choice != 2) return FALSE;

  trp = (tRNAPtr) rrp->ext.value.ptrvalue;
  if (trp == NULL) return FALSE;
      
  /* find SeqId to use */
  sip = SeqLocId (sfp->location);
  if (sip == NULL) {
    slp = SeqLocFindNext (sfp->location, NULL);
    if (slp != NULL) {
      sip = SeqLocId (slp);
    }
  }
  if (sip == NULL) return FALSE;

  /* parse location */
  pos = SimpleValuePos (val);
  if (pos == NULL) {
    ErrPostEx (SEV_WARNING, ERR_FEATURE_LocationParsing,
               "anticodon parsing failed, %s, drop the anticodon", val);
    return FALSE;
  }

  trp->anticodon = Nlm_gbparseint (pos, &locmap, &sitesmap, &num_errs, sip);
  if (trp->anticodon == NULL) {
    ErrPostEx (SEV_WARNING, ERR_FEATURE_LocationParsing,
               "anticodon parsing failed, %s, drop the anticodon", pos);
    MemFree (pos);
    return FALSE;
  }

  if (trp->anticodon->choice == SEQLOC_PNT) {
    /* allow a single point */
    spp = trp->anticodon->data.ptrvalue;
    if (spp != NULL) {
      spp->point += offset;
    }
  }
  if (trp->anticodon->choice == SEQLOC_INT) {
    sintp = trp->anticodon->data.ptrvalue;
    if (sintp == NULL) {
      MemFree (pos);
      return FALSE;
    }
    sintp->from += offset;
    sintp->to += offset;
    if (sintp->from > sintp->to) {
      temp = sintp->from;
      sintp->from = sintp->to;
      sintp->to = temp;
    }
    sintp->strand = SeqLocStrand (sfp->location);
    strand = sintp->strand;
    diff = SeqLocStop(trp->anticodon) - SeqLocStart(trp->anticodon); /* SeqLocStop/Start does not do what you think */
    /*
    if ((diff != 2 && (strand != Seq_strand_minus)) ||
        (diff != -2 && (strand == Seq_strand_minus))) {
      pos_range = TRUE;
    }
    */
    if (diff != 2) {
      pos_range = TRUE;
    }
    if (num_errs > 0 || pos_range) {
      ErrPostEx (SEV_WARNING, ERR_FEATURE_LocationParsing,
                 "anticodon range is wrong, %s, drop the anticodon", pos);
      MemFree (pos);
      return FALSE;
    }
    if (SeqLocCompare (sfp->location, trp->anticodon) != SLC_B_IN_A) {
      ErrPostEx (SEV_WARNING, ERR_FEATURE_LocationParsing,
                 "/anticodon not in tRNA: %s", val);
      MemFree (pos);
      return FALSE;
    }
  }

  MemFree (pos);

  return TRUE;
}

extern Boolean ParseCodeBreak (SeqFeatPtr sfp, CharPtr val, Int4 offset);
extern Boolean ParseCodeBreak (SeqFeatPtr sfp, CharPtr val, Int4 offset)

{
  CodeBreakPtr  cbp;
  CdRegionPtr   crp;
  Int4          diff;
  CodeBreakPtr  lastcbp;
  Boolean       locmap;
  int           num_errs;
  CharPtr       pos;
  Boolean       pos_range = FALSE;
  SeqIntPtr     sintp;
  SeqIdPtr      sip;
  Boolean       sitesmap;
  SeqLocPtr     slp;
  SeqPntPtr     spp;
  Uint1         strand;
  Int4          temp;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return FALSE;
  if (StringHasNoText (val)) return FALSE;
  crp = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (crp == NULL) return FALSE;

  /* find SeqId to use */
  sip = SeqLocId (sfp->location);
  if (sip == NULL) {
    slp = SeqLocFindNext (sfp->location, NULL);
    if (slp != NULL) {
      sip = SeqLocId (slp);
    }
  }
  if (sip == NULL) return FALSE;

  cbp = CodeBreakNew ();
  if (cbp == NULL) return FALSE;
  cbp->aa.choice = 1; /* ncbieaa */
  cbp->aa.value.intvalue = (Int4) GetQualValueAa (val);

  /* parse location */
  pos = SimpleValuePos (val);
  if (pos == NULL) {
    ErrPostEx (SEV_WARNING, ERR_FEATURE_LocationParsing,
               "transl_except parsing failed, %s, drop the transl_except", val);
    return FALSE;
  }
  cbp->loc = Nlm_gbparseint (pos, &locmap, &sitesmap, &num_errs, sip);
  if (cbp->loc == NULL) {
    CodeBreakFree (cbp);
    ErrPostEx (SEV_WARNING, ERR_FEATURE_LocationParsing,
               "transl_except parsing failed, %s, drop the transl_except", pos);
    MemFree (pos);
    return FALSE;
  }
  if (cbp->loc->choice == SEQLOC_PNT) {
    /* allow a single point */
    spp = cbp->loc->data.ptrvalue;
    if (spp != NULL) {
      spp->point += offset;
    }
  }
  if (cbp->loc->choice == SEQLOC_INT) {
    sintp = cbp->loc->data.ptrvalue;
    if (sintp == NULL) {
      MemFree (pos);
      return FALSE;
    }
    sintp->from += offset;
    sintp->to += offset;
    if (sintp->from > sintp->to) {
      temp = sintp->from;
      sintp->from = sintp->to;
      sintp->to = temp;
    }
    sintp->strand = SeqLocStrand (sfp->location);
    strand = sintp->strand;
    diff = SeqLocStop(cbp->loc) - SeqLocStart(cbp->loc); /* SeqLocStop/Start does not do what you think */
    /*
    if ((diff != 2 && (strand != Seq_strand_minus)) ||
        (diff != -2 && (strand == Seq_strand_minus))) {
      pos_range = TRUE;
    }
    */
    if (diff != 2) {
      pos_range = TRUE;
    }
    if (num_errs > 0 || pos_range) {
      CodeBreakFree (cbp);
      ErrPostEx (SEV_WARNING, ERR_FEATURE_LocationParsing,
                 "transl_except range is wrong, %s, drop the transl_except", pos);
      MemFree (pos);
      return FALSE;
    }
    if (SeqLocCompare (sfp->location, cbp->loc) != SLC_B_IN_A) {
      CodeBreakFree (cbp);
      ErrPostEx (SEV_WARNING, ERR_FEATURE_LocationParsing,
                 "/transl_except not in CDS: %s", val);
      MemFree (pos);
      return FALSE;
    }
  }

  /* add to code break list */
  lastcbp = crp->code_break;
  if (lastcbp == NULL) {
    crp->code_break = cbp;
  } else {
     while (lastcbp->next != NULL) {
      lastcbp = lastcbp->next;
    }
    lastcbp->next = cbp;
  }
  MemFree (pos);
  return TRUE;
}

static Boolean CodonsAlreadyInOrder (tRNAPtr trp)

{
  Int2  i, j;

  if (trp == NULL) return TRUE;
  for (i = 0, j = 1; i < 5; i++, j++) {
    if (trp->codon [i] > trp->codon [j]) return FALSE;
  }
  return TRUE;
}

static int LIBCALLBACK SortCodons (VoidPtr ptr1, VoidPtr ptr2)

{
  Uint1  codon1, codon2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  codon1 = *((Uint1Ptr) ptr1);
  codon2 = *((Uint1Ptr) ptr2);
  if (codon1 > codon2) {
    return 1;
  } else if (codon1 < codon2) {
    return -1;
  }
  return 0;
}

static void UniqueCodons (tRNAPtr trp)

{
  Int2   i, j;
  Uint2  last = 255, next;

  if (trp == NULL) return;

  for (i = 0, j = 0; i < 6; i++) {
    next = trp->codon [i];
    if (next != last) {
      trp->codon [j] = next;
      last = next;
      j++;
    }
  }
  while (j < 6) {
    trp->codon [j] = 255;
    j++;
  }
}

static CharPtr  codonLetterExpand [] =
{
  "?", "A", "C", "AC",
  "G", "AG", "CG", "ACG",
  "T", "AT", "CT", "ACT",
  "GT", "AGT", "CGT", "ACGT",
  NULL
};

NLM_EXTERN Boolean ParseDegenerateCodon (tRNAPtr trp, Uint1Ptr codon)

{
  Uint1    ch;
  Uint1    chrToInt [256];
  Int2     i, j;
  Uint1    idx;
  CharPtr  intToChr = "?ACMGRSVTWYHKDBN";
  CharPtr  ptr, str;

  if (trp == NULL || codon == NULL) return FALSE;

  for (i = 0; i < 2; i++) {
    ch = codon [i];
    if (ch != 'A' && ch != 'C' && ch != 'G' && ch != 'T') return FALSE;
  }

  for (i = 0; i < 256; i++) {
    chrToInt [i] = 0;
  }
  for (i = 1; i < 16; i++) {
    ch = intToChr [i];
    chrToInt [(int) ch] = i;
  }

  idx = chrToInt [(int) codon [2]];
  if (idx > 15) return FALSE;

  str = codonLetterExpand [idx];
  ptr = str;
  ch = *ptr;
  j = 0;
  codon [3] = '\0';
  while (ch != '\0' && j < 6) {
    codon [2] = ch;
    trp->codon [j] = IndexForCodon (codon, Seq_code_iupacna);
    ptr++;
    ch = *ptr;
    j++;
  }

  return TRUE;
}

static void CleanupTrna (SeqFeatPtr sfp, tRNAPtr trp)

{
  Uint1           aa = 0;
  Char            codon [16];
  Uint1           curraa;
  Uint1           from = 0;
  Int2            i;
  Int2            j;
  Boolean         justTrnaText;
  Boolean         okayToFree = TRUE;
  SeqMapTablePtr  smtp;
  CharPtr         str;
  Uint1           trpcodon [6];

  /* look for tRNA-OTHER with actual amino acid in comment */

  if (trp == NULL) return;

  if (sfp != NULL && sfp->comment != NULL && trp->codon [0] == 255) {
    if (StringNICmp (sfp->comment, "codon recognized: ", 18) == 0) {
      StringNCpy_0 (codon, sfp->comment + 18, sizeof (codon));
      if (StringLen (codon) > 3 && codon [3] == ';') {
        codon [3] = '\0';
        okayToFree = FALSE;
      }
      if (StringLen (codon) == 3) {
        for (i = 0; i < 3; i++) {
          if (codon [i] == 'U') {
            codon [i] = 'T';
          }
        }
        if (ParseDegenerateCodon (trp, (Uint1Ptr) codon)) {
          if (okayToFree) {
            sfp->comment = MemFree (sfp->comment);
          } else {
            str = StringSave (sfp->comment + 22);
            TrimSpacesAroundString (str);
            sfp->comment = MemFree (sfp->comment);
            if (StringHasNoText (str)) {
              str = MemFree (str);
            }
            sfp->comment = str;
          }
        }
      }
    }
  }

  if (! CodonsAlreadyInOrder (trp)) {
    HeapSort ((VoidPtr) &(trp->codon), 6, sizeof (Uint1), SortCodons);
  }
  UniqueCodons (trp);

  /* now always switch iupacaa to ncbieaa (was just for selenocysteine) */

  if (trp->aatype == 1 /* && trp->aa == 'U' */) {
    trp->aatype = 2;
  }

  if (sfp == NULL || sfp->comment == NULL) return;

  if (trp->aatype == 2) {
    aa = trp->aa;
  } else {
    switch (trp->aatype) {
      case 0 :
        from = 0;
        break;
      case 1 :
        from = Seq_code_iupacaa;
        break;
      case 2 :
        from = Seq_code_ncbieaa;
        break;
      case 3 :
        from = Seq_code_ncbi8aa;
        break;
      case 4 :
        from = Seq_code_ncbistdaa;
        break;
      default:
        break;
    }
    smtp = SeqMapTableFind (Seq_code_ncbieaa, from);
    if (smtp != NULL) {
      aa = SeqMapTableConvert (smtp, trp->aa);
    }
  }
  if (aa != 'X') {
    curraa = ParseTRnaString (sfp->comment, &justTrnaText, trpcodon, TRUE);
    if (aa == 0 && curraa != 0) {
      aa = curraa;
      trp->aa = curraa;
      trp->aatype = 2;
    }
    if (aa != 0 && aa == curraa) {
      if (justTrnaText) {
        for (j = 0; j < 6; j++) {
          if (trp->codon [j] == 255) {
            trp->codon [j] = trpcodon [j];
          }
        }
        if (StringCmp (sfp->comment, "fMet") != 0) {
          sfp->comment = MemFree (sfp->comment);
        }
      }
    }
    return;
  }
  aa = ParseTRnaString (sfp->comment, &justTrnaText, trpcodon, TRUE);
  if (aa == 0) return;
  trp->aa = aa;
  trp->aatype = 2;
  if (justTrnaText) {
    for (j = 0; j < 6; j++) {
      if (trp->codon [j] == 255) {
        trp->codon [j] = trpcodon [j];
      }
    }
    if (StringCmp (sfp->comment, "fMet") != 0) {
      sfp->comment = MemFree (sfp->comment);
    }
  }
}

NLM_EXTERN SeqFeatPtr LIBCALL GetBestProteinFeatureUnindexed (SeqLocPtr product)

{
  BioseqPtr    bsp;
  SeqFeatPtr   prot = NULL;
  SeqAnnotPtr  sap;
  SeqFeatPtr   tmp;
  ValNode      vn;

  if (product == NULL) return NULL;
  bsp = BioseqFindFromSeqLoc (product);
  if (bsp == NULL || bsp->repr != Seq_repr_raw) return NULL;
  vn.choice = SEQLOC_WHOLE;
  vn.data.ptrvalue = (Pointer) SeqIdFindBest (bsp->id, 0);
  vn.next = NULL;
  for (sap = bsp->annot; sap != NULL && prot == NULL; sap = sap->next) {
    if (sap->type == 1) {
      for (tmp = (SeqFeatPtr) sap->data; tmp != NULL && prot == NULL; tmp = tmp->next) {
        if (tmp->data.choice == SEQFEAT_PROT) {
          if (SeqLocCompare (tmp->location, &vn)) {
            /* find first protein feature packaged on and located on bioseq */
            prot = tmp;
          }
        }
      }
    }
  }
  return prot;
}

static Boolean HandledGBQualOnCDS (SeqFeatPtr sfp, GBQualPtr gbq, ValNodePtr PNTR afterMe)

{
  Int2            choice = 0;
  CdRegionPtr     crp;
  Uint1           frame;
  ValNodePtr      gcp;
  Boolean         pos_range = FALSE;
  ValNodePtr      prev;
  SeqFeatPtr      prot;
  ProtRefPtr      prp = NULL;
  Char            str [16];
  Int4            transl_table;
  int             val;
  ValNodePtr      vnp;
  SeqFeatXrefPtr  xref;

  if (StringICmp (gbq->qual, "product") == 0) {
    choice = 1;
  } else if (StringICmp (gbq->qual, "function") == 0) {
    choice = 2;
  } else if (StringICmp (gbq->qual, "EC_number") == 0) {
    choice = 3;
  } else if (StringICmp (gbq->qual, "prot_note") == 0) {
    choice = 4;
  }
  if (choice > 0) {
    prot = GetBestProteinFeatureUnindexed (sfp->product);
    if (prot != NULL) {
      prp = (ProtRefPtr) prot->data.value.ptrvalue;
    }
    if (prp == NULL) {
      /* otherwise make cross reference */
      xref = sfp->xref;
      while (xref != NULL && xref->data.choice != SEQFEAT_PROT) {
        xref = xref->next;
      }
      if (xref == NULL) {
        prp = ProtRefNew ();
        if (prp == NULL) return FALSE;
        xref = SeqFeatXrefNew ();
        if (xref == NULL) return FALSE;
        xref->data.choice = SEQFEAT_PROT;
        xref->data.value.ptrvalue = (Pointer) prp;
        xref->next = sfp->xref;
        sfp->xref = xref;
      }
      if (xref != NULL) {
        prp = (ProtRefPtr) xref->data.value.ptrvalue;
      }
    }
    if (prp == NULL) return FALSE;
    switch (choice) {
      case 1 :
        if (prot != NULL && prot->data.value.ptrvalue != NULL) {
          if (*afterMe == NULL) {
            /* if protein product exists, product gbqual becomes first name */
            vnp = ValNodeCopyStr (NULL, 0, gbq->val);
            if (vnp != NULL) {
              vnp->next = prp->name;
              prp->name = vnp;
            }
            *afterMe = vnp;
          } else {
            vnp = ValNodeCopyStr (NULL, 0, gbq->val);
            prev = *afterMe;
            if (vnp != NULL) {
              vnp->next = prev->next;
              prev->next = vnp;
            }
            *afterMe = vnp;
          }
        } else {
          /* if local xref, append to name */
          ValNodeCopyStr (&(prp->name), 0, gbq->val);
        }
        break;
      case 2 :
        ValNodeCopyStr (&(prp->activity), 0, gbq->val);
        break;
      case 3 :
        ValNodeCopyStr (&(prp->ec), 0, gbq->val);
        break;
      case 4 :
        prot->comment = StringSave (gbq->val);
        break;
      default :
        break;
    }
    return TRUE;
  }

  if (StringICmp (gbq->qual, "transl_except") == 0) {
    return ParseCodeBreak (sfp, gbq->val, 0);
  }

  if (StringICmp (gbq->qual, "codon_start") == 0) {
    crp = (CdRegionPtr) sfp->data.value.ptrvalue;
    if (crp != NULL) {
      frame = crp->frame;
      if (frame == 0) {
        StringNCpy_0 (str, gbq->val, sizeof (str));
        if (sscanf (str, "%d", &val) == 1) {
          if (val > 0 || val < 4) {
            crp->frame = (Uint1) val;
            return TRUE;
          }
        }
        frame = 1;
      }
      sprintf (str, "%d", (int) frame);
      if (StringICmp (str, gbq->val) == 0) {
        return TRUE;
      } else if (sfp->pseudo && sfp->product == NULL) {
        StringNCpy_0 (str, gbq->val, sizeof (str));
        if (sscanf (str, "%d", &val) == 1) {
          if (val > 0 || val < 4) {
            crp->frame = (Uint1) val;
            return TRUE;
          }
        }
      }
    }
  }

  if (StringICmp (gbq->qual, "transl_table") == 0) {
    crp = (CdRegionPtr) sfp->data.value.ptrvalue;
    if (crp != NULL) {
      transl_table = 0;
      gcp = crp->genetic_code;
      if (gcp != NULL) {
        for (vnp = gcp->data.ptrvalue; vnp != NULL; vnp = vnp->next) {
          if (vnp->choice == 2 && vnp->data.intvalue != 0) {
            transl_table = vnp->data.intvalue;
          }
        }
        if (transl_table == 0) {
          transl_table = 1;
        }
        sprintf (str, "%ld", (long) transl_table);
        if (StringICmp (str, gbq->val) == 0) {
          return TRUE;
        }
      } else {
        StringNCpy_0 (str, gbq->val, sizeof (str));
        if (sscanf (str, "%d", &val) == 1) {
          vnp = ValNodeNew (NULL);
          if (vnp != NULL) {
            vnp->choice = 2;
            vnp->data.intvalue = (Int4) val;
            gcp = GeneticCodeNew ();
            if (gcp != NULL) {
              gcp->data.ptrvalue = vnp;
              crp->genetic_code = gcp;
              return TRUE;
            }
          }
        }
      }
    }
  }

  if (StringICmp (gbq->qual, "translation") == 0) {
    return TRUE;
  }

  return FALSE;
}

static Boolean HandledGBQualOnRNA (SeqFeatPtr sfp, GBQualPtr gbq, Boolean isEmblOrDdbj)

{
  Uint1      aa;
  BioseqPtr  bsp;
  Uint1      codon [6];
  Int4       from;
  Boolean    is_fMet = FALSE;
  Int2       j;
  Boolean    justTrnaText;
  size_t     len;
  CharPtr    name;
  CharPtr    ptr;
  RnaRefPtr  rrp;
  SeqIntPtr  sintp;
  SeqIdPtr   sip;
  CharPtr    str;
  Char       tmp [64];
  Int4       to;
  tRNAPtr    trp;
  long int   val;

  if (StringICmp (gbq->qual, "product") == 0 ||
      (StringICmp (gbq->qual, "standard_name") == 0 && (! isEmblOrDdbj) )) {
    rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
    if (rrp == NULL) return FALSE;
    if (rrp->type == 0) {
      rrp->type = 255;
    }
    if (rrp->type == 3 && rrp->ext.choice == 1) {
      name = (CharPtr) rrp->ext.value.ptrvalue;
      aa = ParseTRnaString (name, &justTrnaText, codon, FALSE);
      if (aa != 0) {
        is_fMet = (Boolean) (StringStr (name, "fMet") != NULL);
        rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
        trp = (tRNAPtr) MemNew (sizeof (tRNA));
        if (trp != NULL) {
          trp->aatype = 2;
          for (j = 0; j < 6; j++) {
            trp->codon [j] = 255;
          }
          if (justTrnaText) {
            for (j = 0; j < 6; j++) {
              trp->codon [j] = codon [j];
            }
          }
          trp->aa = aa;
          rrp->ext.choice = 2;
          rrp->ext.value.ptrvalue = (Pointer) trp;
          if (aa == 'M') {
            if (is_fMet) {
              if (sfp->comment == NULL) {
                sfp->comment = StringSave ("fMet");
              } else {
                len = StringLen (sfp->comment) + StringLen ("fMet") + 5;
                str = MemNew (sizeof (Char) * len);
                StringCpy (str, sfp->comment);
                StringCat (str, "; ");
                StringCat (str, "fMet");
                sfp->comment = MemFree (sfp->comment);
                sfp->comment = str;
              }
            }
          }
          CleanupTrna (sfp, trp);
        }
      }
    }
    if (rrp->type == 3 && rrp->ext.choice == 0) {
      AddQualifierToFeature (sfp, "product", gbq->val);
      return TRUE;
    }
    if (rrp->type == 3 && rrp->ext.choice == 2) {
      trp = (tRNAPtr) rrp->ext.value.ptrvalue;
      if (trp != NULL && trp->aatype == 2) {
        if (trp->aa == ParseTRnaString (gbq->val, NULL, NULL, FALSE)) {
          return TRUE;
        }
      }
    }
    if (rrp->ext.choice != 0 && rrp->ext.choice != 1) return FALSE;
    if (! HasNoText ((CharPtr) rrp->ext.value.ptrvalue)) {
      if (StringICmp ((CharPtr) rrp->ext.value.ptrvalue, gbq->val) == 0) {
        return TRUE;
      }
      str = StringStr (gbq->val, "rDNA");
      if (str != NULL) {
        str [1] = 'R';
        if (StringICmp ((CharPtr) rrp->ext.value.ptrvalue, gbq->val) == 0) {
          return TRUE;
        }
      }
      /* subsequent /product now added to comment */
      if (sfp->comment == NULL) {
        sfp->comment = gbq->val;
        gbq->val = NULL;
      } else if (StringStr (gbq->val, sfp->comment) == NULL) {
        len = StringLen (sfp->comment) + StringLen (gbq->val) + 5;
        str = MemNew (sizeof (Char) * len);
        StringCpy (str, sfp->comment);
        StringCat (str, "; ");
        StringCat (str, gbq->val);
        sfp->comment = MemFree (sfp->comment);
        sfp->comment = str;
      }
      /* return FALSE; */
      return TRUE;
    }
    if (rrp->ext.choice == 1 && rrp->ext.value.ptrvalue != NULL) {
      rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
    }
    if (rrp->ext.choice == 0 || rrp->ext.choice == 1) {
      rrp->ext.choice = 1;
      rrp->ext.value.ptrvalue = StringSave (gbq->val);
      return TRUE;
    }
  } else if (StringICmp (gbq->qual, "anticodon") == 0) {
    rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
    if (rrp == NULL) return FALSE;
    if (rrp->type == 0) {
      rrp->type = 255;
    }
    if (rrp->type == 3 && rrp->ext.choice == 0) {
      trp = (tRNAPtr) MemNew (sizeof (tRNA));
      if (trp != NULL) {
        rrp->ext.choice = 2;
        rrp->ext.value.ptrvalue = trp;
        for (j = 0; j < 6; j++) {
          trp->codon [j] = 255;
        }
      }
    }
    if (rrp->type == 3 && rrp->ext.choice == 2) {
      trp = (tRNAPtr) rrp->ext.value.ptrvalue;
      if (trp != NULL) {
        StringNCpy_0 (tmp, gbq->val, sizeof (tmp));
        ptr = StringStr (tmp, "(");
        if (ptr != NULL) {
          ptr = StringStr (ptr + 1, "pos");
          if (ptr != NULL) {
            ptr = StringStr (ptr + 3, ":");
          }
        }
        if (ptr != NULL) {
          str = ptr + 1;
          ptr = StringStr (str, "..");
          if (ptr != NULL) {
            *ptr = '\0';
            if (sscanf (str, "%ld", &val) == 1) {
              from = val - 1;
              str = ptr + 2;
              ptr = StringStr (str, ",");
              if (ptr != NULL) {
                *ptr = '\0';
                if (sscanf (str, "%ld", &val) == 1) {
                  to = val - 1;
                  sip = SeqLocId (sfp->location);
                  if (sip != NULL) {
                    bsp = BioseqFind (sip);
                    if (bsp != NULL) {
                      if (from >= 0 && from < bsp->length - 1) {
                        if (to >= 0 && to < bsp->length - 1) {
                          sintp = SeqIntNew ();
                          if (sintp != NULL) {
                            if (from > to) {
                              sintp->from = to;
                              sintp->to = from;
                              sintp->strand = Seq_strand_minus;
                            } else {
                              sintp->from = from;
                              sintp->to = to;
                              sintp->strand = Seq_strand_plus;
                            }
                            sintp->id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
                            trp->anticodon = ValNodeAddPointer (NULL, SEQLOC_INT, (Pointer) sintp);
                            if (trp->aatype == 0 && trp->aa == 0) {
                              ptr = StringStr (ptr + 1, "aa:");
                              if (ptr != NULL) {
                                str = ptr + 3;
                                ptr = StringStr (str, ")");
                                if (ptr != NULL) {
                                  *ptr = '\0';
                                  trp->aa = ParseTRnaString (str, NULL, NULL, FALSE);
                                  if (trp->aa != 0) {
                                    trp->aatype = 2;
                                  }
                                }
                              }
                            }
                            return TRUE;
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return FALSE;
}

static Boolean HandledGBQualOnProt (SeqFeatPtr sfp, GBQualPtr gbq)

{
  Int2        choice = 0;
  ProtRefPtr  prp;
  ValNodePtr  vnp;

  prp = (ProtRefPtr) sfp->data.value.ptrvalue;
  if (prp == NULL) return FALSE;
  if (StringICmp (gbq->qual, "product") == 0) {
    choice = 1;
  } else if (StringICmp (gbq->qual, "function") == 0) {
    choice = 2;
  } else if (StringICmp (gbq->qual, "EC_number") == 0) {
    choice = 3;
  } else if (StringICmp (gbq->qual, "standard_name") == 0) {
    choice = 4;
  } else if (StringICmp (gbq->qual, "label") == 0) {
    choice = 5;
  }
  if (choice == 1 || choice == 4) {
    vnp = prp->name;
    if (vnp != NULL && (! HasNoText (vnp->data.ptrvalue))) return FALSE;
    ValNodeCopyStr (&(prp->name), 0, gbq->val);
    vnp = prp->name;
    if (vnp != NULL && prp->desc != NULL) {
      if (StringICmp (vnp->data.ptrvalue, prp->desc) == 0) {
        prp->desc = MemFree (prp->desc);
      }
    }
    return TRUE;
  } else if (choice == 2) {
    ValNodeCopyStr (&(prp->activity), 0, gbq->val);
    return TRUE;
  } else if (choice == 3) {
    ValNodeCopyStr (&(prp->ec), 0, gbq->val);
    return TRUE;
  } else if (choice == 5) {
    return FALSE; /* keep label gbqual only */
  }
  return TRUE; /* all other gbquals not appropriate on protein features */
}

static Boolean HandledGBQualOnImp (SeqFeatPtr sfp, GBQualPtr gbq)

{
  Char        ch;
  ImpFeatPtr  ifp;
  Int4        len;
  CharPtr     ptr;

  if (StringICmp (gbq->qual, "rpt_unit") == 0) {
    if (HasNoText (gbq->val)) return FALSE;
    ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
    if (ifp == NULL) return FALSE;
    if (StringICmp (ifp->key, "repeat_region") != 0) return FALSE;
    len = SeqLocLen (sfp->location);
    if (len != (Int4) StringLen (gbq->val)) return FALSE;
    ptr = gbq->val;
    ch = *ptr;
    while (ch != '\0') {
      if (StringChr ("ACGTNacgtn", ch) == NULL) return FALSE;
      ptr++;
      ch = *ptr;
    }
    /* return TRUE; */
  }
  return FALSE;
}

static CharPtr TrimParenthesesAndCommasAroundString (CharPtr str)

{
  Uchar    ch;	/* to use 8bit characters in multibyte languages */
  CharPtr  dst;
  CharPtr  ptr;

  if (str != NULL && str [0] != '\0') {
    dst = str;
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && (ch < ' ' || ch == '(' || ch == ',')) {
      ptr++;
      ch = *ptr;
    }
    while (ch != '\0') {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
    }
    *dst = '\0';
    dst = NULL;
    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      if (ch != ')' && ch != ',') {
        dst = NULL;
      } else if (dst == NULL) {
        dst = ptr;
      }
      ptr++;
      ch = *ptr;
    }
    if (dst != NULL) {
      *dst = '\0';
    }
  }
  return str;
}

static CharPtr CombineSplitQual (CharPtr origval, CharPtr newval)

{
  size_t   len;
  CharPtr  str = NULL;

  if (StringStr (origval, newval) != NULL) return origval;
  len = StringLen (origval) + StringLen (newval) + 5;
  str = MemNew (sizeof (Char) * len);
  if (str == NULL) return origval;
  TrimParenthesesAndCommasAroundString (origval);
  TrimParenthesesAndCommasAroundString (newval);
  StringCpy (str, "(");
  StringCat (str, origval);
  StringCat (str, ",");
  StringCat (str, newval);
  StringCat (str, ")");
  /* free original string, knowing return value will replace it */
  MemFree (origval);
  return str;
}

static void CleanupRptUnit (GBQualPtr gbq)

{
  Char     ch;
  size_t   len;
  CharPtr  ptr;
  CharPtr  str;
  CharPtr  tmp;

  if (gbq == NULL) return;
  if (StringHasNoText (gbq->val)) return;
  len = StringLen (gbq->val) * 2 + 1;
  str = MemNew (sizeof (Char) * len);
  if (str == NULL) return;
  ptr = str;
  tmp = gbq->val;
  ch = *tmp;
  while (ch != '\0') {
    while (ch == '(' || ch == ')' || ch == ',') {
      *ptr = ch;
      ptr++;
      tmp++;
      ch = *tmp;
    }
    while (IS_WHITESP (ch)) {
      tmp++;
      ch = *tmp;
    }
    while (IS_DIGIT (ch)) {
      *ptr = ch;
      ptr++;
      tmp++;
      ch = *tmp;
    }
    if (ch == '.' || ch == '-') {
      while (ch == '.' || ch == '-') {
        tmp++;
        ch = *tmp;
      }
      *ptr = '.';
      ptr++;
      *ptr = '.';
      ptr++;
    }
    while (IS_WHITESP (ch)) {
      tmp++;
      ch = *tmp;
    }
    while (IS_DIGIT (ch)) {
      *ptr = ch;
      ptr++;
      tmp++;
      ch = *tmp;
    }
    while (IS_WHITESP (ch)) {
      tmp++;
      ch = *tmp;
    }
    if (ch == '\0' || ch == '(' || ch == ')' || ch == ',' || ch == '.' || IS_WHITESP (ch) || IS_DIGIT (ch)) {
    } else {
      MemFree (str);
      return;
    }
  }
  *ptr = '\0';
  gbq->val = MemFree (gbq->val);
  gbq->val = str;
}

static Boolean StringIsJustQuotes (CharPtr str)

{
  Nlm_Uchar  ch;	/* to use 8bit characters in multibyte languages */

  if (str != NULL) {
    ch = *str;
    while (ch != '\0') {
      if (ch > ' ' && ch != '"' && ch != '\'') {
        return FALSE;
      }
      str++;
      ch = *str;
    }
  }
  return TRUE;
}

static void CleanupConsSplice (GBQualPtr gbq)

{
  size_t   len;
  CharPtr  ptr;
  CharPtr  str;

  if (StringNICmp (gbq->val, "(5'site:", 8) != 0) return;
  ptr = StringStr (gbq->val, ",3'site:");
  if (ptr == NULL) return;
  len = StringLen (gbq->val) + 5;
  str = (CharPtr) MemNew (len);
  if (str == NULL) return;
  *ptr = '\0';
  ptr++;
  StringCpy (str, gbq->val);
  StringCat (str, ", ");
  StringCat (str, ptr);
  gbq->val = MemFree (gbq->val);
  gbq->val = str;
}

static void CleanupFeatureGBQuals (SeqFeatPtr sfp, Boolean isEmblOrDdbj)

{
  ValNodePtr      afterMe = NULL;
  DbtagPtr        db;
  GBQualPtr       gbq;
  GeneRefPtr      grp;
  size_t          len;
  GBQualPtr       nextqual;
  ObjectIdPtr     oip;
  GBQualPtr PNTR  prevqual;
  CharPtr         ptr;
  GBQualPtr       rpt_type = NULL;
  GBQualPtr       rpt_unit = NULL;
  CharPtr         str;
  CharPtr         tag;
  Boolean         unlink;
  GBQualPtr       usedin = NULL;
  ValNodePtr      vnp;
  SeqFeatXrefPtr  xref;

  if (sfp == NULL) return;
  gbq = sfp->qual;
  prevqual = (GBQualPtr PNTR) &(sfp->qual);
  while (gbq != NULL) {
    CleanVisString (&(gbq->qual));
    CleanVisString (&(gbq->val));
    if (gbq->qual == NULL) {
      gbq->qual = StringSave ("");
    }
    if (StringIsJustQuotes (gbq->val)) {
      gbq->val = MemFree (gbq->val);
    }
    if (gbq->val == NULL) {
      gbq->val = StringSave ("");
    }
    nextqual = gbq->next;
    unlink = TRUE;
    if (StringICmp (gbq->qual, "partial") == 0) {
      sfp->partial = TRUE;
    } else if (StringICmp (gbq->qual, "evidence") == 0) {
      if (StringICmp (gbq->val, "experimental") == 0) {
        if (sfp->exp_ev != 2) {
          sfp->exp_ev = 1;
        }
      } else if (StringICmp (gbq->val, "not_experimental") == 0) {
        sfp->exp_ev = 2;
      }
    } else if (StringICmp (gbq->qual, "exception") == 0) {
      sfp->excpt = TRUE;
      if (! HasNoText (gbq->val)) {
        if (StringICmp (gbq->val, "TRUE") != 0) {
          if (sfp->except_text == NULL) {
            sfp->except_text = StringSaveNoNull (gbq->val);
          }
        }
      }
    } else if (StringICmp (gbq->qual, "note") == 0) {
      if (sfp->comment == NULL) {
        sfp->comment = gbq->val;
        gbq->val = NULL;
      } else {
        len = StringLen (sfp->comment) + StringLen (gbq->val) + 5;
        str = MemNew (sizeof (Char) * len);
        StringCpy (str, sfp->comment);
        StringCat (str, "; ");
        StringCat (str, gbq->val);
        sfp->comment = MemFree (sfp->comment);
        sfp->comment = str;
      }
    } else if (StringICmp (gbq->qual, "db_xref") == 0) {
      vnp = ValNodeNew (NULL);
      db = DbtagNew ();
      vnp->data.ptrvalue = db;
      tag = gbq->val;
      ptr = StringChr (tag, ':');
      if (ptr != NULL) {
        *ptr = '\0';
        ptr++;
        db->db = StringSave (tag);
        oip = ObjectIdNew ();
        oip->str = StringSave (ptr);
        db->tag = oip;
        vnp->next = sfp->dbxref;
        sfp->dbxref = vnp;
      } else {
        /*
        db->db = StringSave ("?");
        oip = ObjectIdNew ();
        oip->str = StringSave (tag);
        db->tag = oip;
        vnp->next = sfp->dbxref;
        sfp->dbxref = vnp;
        */
        unlink = FALSE;
      }
    } else if (StringICmp (gbq->qual, "gdb_xref") == 0) {
      vnp = ValNodeNew (NULL);
      db = DbtagNew ();
      vnp->data.ptrvalue = db;
      db->db = StringSave ("GDB");
      oip = ObjectIdNew ();
      oip->str = StringSave (gbq->val);
      db->tag = oip;
      vnp->next = sfp->dbxref;
      sfp->dbxref = vnp;
    } else if (StringICmp (gbq->qual, "cons_splice") == 0) {
      CleanupConsSplice (gbq);
      unlink = FALSE;
    } else if (sfp->data.choice == SEQFEAT_GENE && HandledGBQualOnGene (sfp, gbq)) {
    } else if (sfp->data.choice == SEQFEAT_CDREGION && HandledGBQualOnCDS (sfp, gbq, &afterMe)) {
    } else if (sfp->data.choice == SEQFEAT_RNA && HandledGBQualOnRNA (sfp, gbq, isEmblOrDdbj)) {
    } else if (sfp->data.choice == SEQFEAT_PROT && HandledGBQualOnProt (sfp, gbq)) {
    } else if (sfp->data.choice == SEQFEAT_IMP && HandledGBQualOnImp (sfp, gbq)) {
    } else if (StringICmp (gbq->qual, "rpt_type") == 0) {
      if (rpt_type == NULL) {
        rpt_type = gbq;
        unlink = FALSE;
      } else {
        rpt_type->val = CombineSplitQual (rpt_type->val, gbq->val);
      }
    } else if (StringICmp (gbq->qual, "rpt_unit") == 0) {
      if (rpt_unit == NULL) {
        rpt_unit = gbq;
        unlink = FALSE;
      } else {
        rpt_unit->val = CombineSplitQual (rpt_unit->val, gbq->val);
      }
    } else if (StringICmp (gbq->qual, "usedin") == 0) {
      if (usedin == NULL) {
        usedin = gbq;
        unlink = FALSE;
      } else {
        usedin->val = CombineSplitQual (usedin->val, gbq->val);
      }
    } else if (StringICmp (gbq->qual, "pseudo") == 0) {
      sfp->pseudo = TRUE;
    } else if (StringICmp (gbq->qual, "gene") == 0 && (! StringHasNoText (gbq->val))) {
      grp = GeneRefNew ();
      grp->locus = StringSave (gbq->val);
      xref = SeqFeatXrefNew ();
      xref->data.choice = SEQFEAT_GENE;
      xref->data.value.ptrvalue = (Pointer) grp;
      xref->specialCleanupFlag = TRUE; /* flag to test for overlapping gene later */
      xref->next = sfp->xref;
      sfp->xref = xref;
    } else if (sfp->data.choice != SEQFEAT_CDREGION && StringICmp (gbq->qual, "codon_start") == 0) {
      /* not legal on anything but CDS, so remove it */
    } else {
      unlink = FALSE;
    }
    if (rpt_unit != NULL) {
      CleanupRptUnit (rpt_unit);
    }
    if (unlink) {
      *(prevqual) = gbq->next;
      gbq->next = NULL;
      gbq->qual = MemFree (gbq->qual);
      gbq->val = MemFree (gbq->val);
      GBQualFree (gbq);
    } else {
      prevqual = (GBQualPtr PNTR) &(gbq->next);
    }
    gbq = nextqual;
  }
}

static int LIBCALLBACK SortByGBQualKey (VoidPtr ptr1, VoidPtr ptr2)

{
  GBQualPtr  gbq1;
  GBQualPtr  gbq2;
  CharPtr    str1;
  CharPtr    str2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  gbq1 = *((GBQualPtr PNTR) ptr1);
  gbq2 = *((GBQualPtr PNTR) ptr2);
  if (gbq1 == NULL || gbq2 == NULL) return 0;
  str1 = (CharPtr) gbq1->qual;
  str2 = (CharPtr) gbq2->qual;
  if (str1 == NULL || str2 == NULL) return 0;
  return StringICmp (str1, str2);
}

static Boolean GBQualsAlreadyInOrder (GBQualPtr list)

{
  GBQualPtr  curr;
  GBQualPtr  next;

  if (list == NULL || list->next == NULL) return TRUE;
  curr = list;
  next = curr->next;
  while (next != NULL) {
    if (StringICmp (curr->qual, next->qual) > 0) return FALSE;
    curr = next;
    next = curr->next;
  }
  return TRUE;
}

static GBQualPtr SortFeatureGBQuals (GBQualPtr list)

{
  size_t     count, i;
  GBQualPtr  gbq, PNTR head;

  if (list == NULL) return NULL;
  if (GBQualsAlreadyInOrder (list)) return list;

  for (gbq = list, count = 0; gbq != NULL; gbq = gbq->next, count++) continue;
  head = MemNew (sizeof (GBQualPtr) * (count + 1));

  for (gbq = list, i = 0; gbq != NULL && i < count; i++) {
    head [i] = gbq;
    gbq = gbq->next;
  }

  HeapSort (head, count, sizeof (GBQualPtr), SortByGBQualKey);

  for (i = 0; i < count; i++) {
    gbq = head [i];
    gbq->next = head [i + 1];
  }

  list = head [0];
  MemFree (head);

  return list;
}

static void CleanupDuplicateGBQuals (GBQualPtr PNTR prevgbq)

{
  GBQualPtr  gbq;
  GBQualPtr  last = NULL;
  GBQualPtr  next;
  Boolean    unlink;

  if (prevgbq == NULL) return;
  gbq = *prevgbq;
  while (gbq != NULL) {
    next = gbq->next;
    unlink = FALSE;
    if (last != NULL) {
      if (StringICmp (last->qual, gbq->qual) == 0 &&
          StringICmp (last->val, gbq->val) == 0) {
        unlink = TRUE;
      }
    } else {
      last = gbq;
    }
    if (unlink) {
      *prevgbq = gbq->next;
      gbq->next = NULL;
      GBQualFree (gbq);
    } else {
      last = gbq;
      prevgbq = (GBQualPtr PNTR) &(gbq->next);
    }
    gbq = next;
  }
}

/* this identifies gbquals that should have been placed into special fields */

#define NUM_ILLEGAL_QUALS 14

/* StringICmp use of TO_UPPER means translation should go before transl_XXX */

static CharPtr illegalGbqualList [NUM_ILLEGAL_QUALS] = {
  "anticodon",
  "citation",
  "codon_start",
  "db_xref",
  "evidence",
  "exception",
  "gene",
  "note",
  "protein_id",
  "pseudo",
  "transcript_id",
  "transl_except",
  "transl_table",
  "translation",
};

static Int2 QualifierIsIllegal (CharPtr qualname)

{
  Int2  L, R, mid;

  if (qualname == NULL || *qualname == '\0') return FALSE;

  L = 0;
  R = NUM_ILLEGAL_QUALS - 1;

  while (L < R) {
    mid = (L + R) / 2;
    if (StringICmp (illegalGbqualList [mid], qualname) < 0) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }

  if (StringICmp (illegalGbqualList [R], qualname) == 0) {
    return TRUE;
  }

  return FALSE;
}

static void GbqualLink (GBQualPtr PNTR head, GBQualPtr qual)

{
  GBQualPtr  gbq;

  if (head == NULL || qual == NULL) return;
  gbq = *head;
  if (gbq != NULL) {
    while (gbq->next != NULL) {
      gbq = gbq->next;
    }
    gbq->next = qual;
  } else {
    *head = qual;
  }
}

static GBQualPtr SortIllegalGBQuals (GBQualPtr list)

{
  GBQualPtr  gbq, next, legal = NULL, illegal = NULL;

  gbq = list;
  while (gbq != NULL) {
    next = gbq->next;
    gbq->next = NULL;
    if (QualifierIsIllegal (gbq->qual)) {
      GbqualLink (&illegal, gbq);
    } else {
      GbqualLink (&legal, gbq);
    }
    gbq = next;
  }
  GbqualLink (&legal, illegal);
  return legal;
}

static Boolean IsSubString (CharPtr str1, CharPtr str2)

{
  Char    ch;
  size_t  len1, len2;

  len1 = StringLen (str1);
  len2 = StringLen (str2);
  if (len1 >= len2) return FALSE;
  if (StringNICmp (str1, str2, len1) != 0) return FALSE;
  ch = str2 [len1];
  if (IS_ALPHANUM (ch)) return FALSE;
  return TRUE;
}

static int LIBCALLBACK SortByOrgModSubtype (VoidPtr ptr1, VoidPtr ptr2)

{
  OrgModPtr  omp1;
  OrgModPtr  omp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  omp1 = *((OrgModPtr PNTR) ptr1);
  omp2 = *((OrgModPtr PNTR) ptr2);
  if (omp1 == NULL || omp2 == NULL) return 0;
  if (omp1->subtype > omp2->subtype) {
    return 1;
  } else if (omp1->subtype < omp2->subtype) {
    return -1;
  }
  return 0;
}

static Boolean OrgModsAlreadyInOrder (OrgModPtr list)

{
  OrgModPtr  curr;
  OrgModPtr  next;

  if (list == NULL || list->next == NULL) return TRUE;
  curr = list;
  next = curr->next;
  while (next != NULL) {
    if (curr->subtype > next->subtype) return FALSE;
    curr = next;
    next = curr->next;
  }
  return TRUE;
}

static OrgModPtr SortOrgModList (OrgModPtr list)

{
  size_t     count, i;
  OrgModPtr  omp, PNTR head;

  if (list == NULL) return NULL;
  if (OrgModsAlreadyInOrder (list)) return list;

  for (omp = list, count = 0; omp != NULL; omp = omp->next, count++) continue;
  head = MemNew (sizeof (OrgModPtr) * (count + 1));

  for (omp = list, i = 0; omp != NULL && i < count; i++) {
    head [i] = omp;
    omp = omp->next;
  }

  HeapSort (head, count, sizeof (OrgModPtr), SortByOrgModSubtype);

  for (i = 0; i < count; i++) {
    omp = head [i];
    omp->next = head [i + 1];
  }

  list = head [0];
  MemFree (head);

  return list;
}

static void CleanOrgModList (OrgModPtr PNTR ompp)

{
  OrgModPtr       last = NULL;
  OrgModPtr       next;
  OrgModPtr       omp;
  OrgModPtr PNTR  prev;
  Boolean         unlink;

  if (ompp == NULL) return;
  prev = ompp;
  omp = *ompp;
  while (omp != NULL) {
    next = omp->next;
    unlink= FALSE;
    CleanVisString (&(omp->subname));
    CleanVisString (&(omp->attrib));
    if (last != NULL) {
      if (HasNoText (omp->subname)) {
        unlink = TRUE;
      } else if (last->subtype == omp->subtype &&
                 StringICmp (last->subname, omp->subname) == 0 ||
                 (last->subtype == ORGMOD_other &&
                  StringStr (last->subname, omp->subname) != NULL)) {
        unlink = TRUE;
      } else if (last->subtype == omp->subtype &&
                 last->subtype == ORGMOD_other &&
                 IsSubString (last->subname, omp->subname)) {
        last->subname = MemFree (last->subname);
        last->subname = omp->subname;
        omp->subname = NULL;
        unlink = TRUE;
      }
    } else {
      last = omp;
    }
    if (unlink) {
      *prev = omp->next;
      omp->next = NULL;
      OrgModFree (omp);
    } else {
      last = omp;
      prev = &(omp->next);
    }
    omp = next;
  }
}

static int LIBCALLBACK SortBySubSourceSubtype (VoidPtr ptr1, VoidPtr ptr2)

{
  SubSourcePtr  ssp1;
  SubSourcePtr  ssp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  ssp1 = *((SubSourcePtr PNTR) ptr1);
  ssp2 = *((SubSourcePtr PNTR) ptr2);
  if (ssp1 == NULL || ssp2 == NULL) return 0;
  if (ssp1->subtype > ssp2->subtype) {
    return 1;
  } else if (ssp1->subtype < ssp2->subtype) {
    return -1;
  }
  return 0;
}

static Boolean SubSourceAlreadyInOrder (SubSourcePtr list)

{
  SubSourcePtr  curr;
  SubSourcePtr  next;

  if (list == NULL || list->next == NULL) return TRUE;
  curr = list;
  next = curr->next;
  while (next != NULL) {
    if (curr->subtype > next->subtype) return FALSE;
    curr = next;
    next = curr->next;
  }
  return TRUE;
}

static SubSourcePtr SortSubSourceList (SubSourcePtr list)

{
  size_t        count, i;
  SubSourcePtr  ssp, PNTR head;

  if (list == NULL) return NULL;
  if (SubSourceAlreadyInOrder (list)) return list;

  for (ssp = list, count = 0; ssp != NULL; ssp = ssp->next, count++) continue;
  head = MemNew (sizeof (SubSourcePtr) * (count + 1));

  for (ssp = list, i = 0; ssp != NULL && i < count; i++) {
    head [i] = ssp;
    ssp = ssp->next;
  }

  HeapSort (head, count, sizeof (SubSourcePtr), SortBySubSourceSubtype);

  for (i = 0; i < count; i++) {
    ssp = head [i];
    ssp->next = head [i + 1];
  }

  list = head [0];
  MemFree (head);

  return list;
}

static void CleanSubSourceList (SubSourcePtr PNTR sspp)

{
  SubSourcePtr       last = NULL;
  SubSourcePtr       next;
  SubSourcePtr PNTR  prev;
  SubSourcePtr       ssp;
  Boolean            unlink;

  if (sspp == NULL) return;
  prev = sspp;
  ssp = *sspp;
  while (ssp != NULL) {
    next = ssp->next;
    unlink= FALSE;
    if (ssp->subtype != SUBSRC_germline &&
        ssp->subtype != SUBSRC_rearranged &&
        ssp->subtype != SUBSRC_transgenic &&
        ssp->subtype != SUBSRC_environmental_sample) {
      CleanVisString (&(ssp->name));
    }
    CleanVisString (&(ssp->attrib));
    if (last != NULL) {
      if (HasNoText (ssp->name) &&
          ssp->subtype != SUBSRC_germline &&
          ssp->subtype != SUBSRC_rearranged &&
          ssp->subtype != SUBSRC_transgenic &&
          ssp->subtype != SUBSRC_environmental_sample) {
        unlink = TRUE;
      } else if (last->subtype == ssp->subtype &&
                 (ssp->subtype == SUBSRC_germline ||
                  ssp->subtype == SUBSRC_rearranged ||
                  ssp->subtype == SUBSRC_transgenic ||
                  ssp->subtype == SUBSRC_environmental_sample ||
                  StringICmp (last->name, ssp->name) == 0) ||
                  (last->subtype == SUBSRC_other &&
                   StringStr (last->name, ssp->name) != NULL)) {
        unlink = TRUE;
      } else if (last->subtype == ssp->subtype &&
                 last->subtype == SUBSRC_other &&
                 IsSubString (last->name, ssp->name)) {
        last->name = MemFree (last->name);
        last->name = ssp->name;
        ssp->name = NULL;
        unlink = TRUE;
      }
    } else {
      last = ssp;
    }
    if (unlink) {
      *prev = ssp->next;
      ssp->next = NULL;
      SubSourceFree (ssp);
    } else {
      last = ssp;
      prev = &(ssp->next);
    }
    ssp = next;
  }
}

/* if string starts with given prefix, return pointer to remaining text */

static CharPtr StringHasPrefix (CharPtr str, CharPtr pref, Boolean novalneeded, Boolean skippref)

{
  Char     ch;
  size_t   len;
  Char     tmp [64];
  CharPtr  val;

  if (StringHasNoText (str) || StringHasNoText (pref)) return NULL;
  len = StringLen (pref);
  StringNCpy_0 (tmp, pref, sizeof (tmp));
  if (StringNICmp (str, tmp, len) != 0) {
    /* try after replacing dash with underscore */
    val = tmp;
    ch = *val;
    while (ch != '\0') {
      if (ch == '-') {
        *val = '_';
      }
      val++;
      ch = *val;
    }
    if (StringNICmp (str, tmp, len) != 0) return NULL;
  }
  if (skippref) {
    val = str + len;
  } else {
    val = str;
  }
  if (StringHasNoText (val)) {
    if (novalneeded) return " ";
    return NULL;
  }
  ch = *(str + len);
  if (ch != '=' && ch != ' ' && ch != '\0') return NULL;
  ch = *val;
  while (ch == '=' || ch == ' ') {
    val++;
    ch = *val;
  }
  if (StringHasNoText (val)) return NULL;
  return val;
}

static CharPtr unstructured_orgmod_list [] = {
  "?", "?", "strain", "substrain", "type", "subtype", "variety",
  "serotype", "serogroup", "serovar", "cultivar", "pathovar", "chemovar",
  "biovar", "biotype", "group", "subgroup", "isolate", "common name",
  "acronym", "dosage", "natural host", "sub-species", "specimen-voucher",
  "authority", "forma", "forma-specialis", "ecotype", "synonym",
  "anamorph", "teleomorph", "breed", NULL
};

static void OrpModToOrgMod (ValNodePtr PNTR vnpp, OrgModPtr PNTR ompp)

{
  Char        ch;
  Int2        i;
  ValNodePtr  next;
  Int2        numcommas;
  Int2        numspaces;
  OrgModPtr   omp;
  CharPtr     ptr;
  CharPtr     str;
  CharPtr     val;
  ValNodePtr  vnp;

  if (vnpp == NULL || ompp == NULL) return;
  vnp = *vnpp;
  while (vnp != NULL) {
    next = vnp->next;
    str = (CharPtr) vnp->data.ptrvalue;
    val = NULL;
    for (i = 0; unstructured_orgmod_list [i] != NULL; i++) {
      val = StringHasPrefix (str, unstructured_orgmod_list [i], FALSE, TRUE);
      if (val != NULL) break;
    }
    if (val != NULL) {
      numspaces = 0;
      numcommas = 0;
      ptr = str;
      ch = *ptr;
      while (ch != '\0') {
        if (ch == ' ') {
          numspaces++;
        } else if (ch == ',') {
          numcommas++;
        }
        ptr++;
        ch = *ptr;
      }
      if (numspaces > 4 || numcommas > 0) {
        val = NULL;
      }
    }
    if (val != NULL) {
      omp = OrgModNew ();
      if (omp != NULL) {
        omp->subtype = (Uint1) i;
        omp->subname = StringSave (val);
        omp->next = *ompp;
        *ompp = omp;
      }
      *vnpp = vnp->next;
      vnp->next = NULL;
      ValNodeFreeData (vnp);
    } else {
      vnpp = &(vnp->next);
    }
    vnp = next;
  }
}

static CharPtr unstructured_subsource_list [] = {
  "?", "chromosome", "map", "clone", "subclone", "haplotype",
  "genotype", "sex", "cell-line", "cell-type", "tissue-type",
  "clone-lib", "dev-stage", "frequency", "germline", "rearranged",
  "lab-host", "pop-variant", "tissue-lib", "plasmid", "transposon",
  "ins-seq", "plastid", "country", "segment", "endogenous-virus",
  "transgenic", "environmental-sample", "isolation-source", NULL
};

static void OrpModToSubSource (ValNodePtr PNTR vnpp, SubSourcePtr PNTR sspp)

{
  Char          ch;
  Int2          i;
  ValNodePtr    next;
  Int2          numcommas;
  Int2          numspaces;
  CharPtr       ptr;
  SubSourcePtr  ssp;
  CharPtr       str;
  CharPtr       val;
  ValNodePtr    vnp;

  if (vnpp == NULL || sspp == NULL) return;
  vnp = *vnpp;
  while (vnp != NULL) {
    next = vnp->next;
    str = (CharPtr) vnp->data.ptrvalue;
    val = NULL;
    for (i = 0; unstructured_subsource_list [i] != NULL; i++) {
      val = StringHasPrefix (str, unstructured_subsource_list [i],
                             (Boolean) (i == SUBSRC_germline ||
                                        i == SUBSRC_rearranged ||
                                        i == SUBSRC_transgenic ||
                                        i == SUBSRC_environmental_sample),
                             TRUE);
      if (val != NULL) break;
    }
    if (val != NULL) {
      numspaces = 0;
      numcommas = 0;
      ptr = str;
      ch = *ptr;
      while (ch != '\0') {
        if (ch == ' ') {
          numspaces++;
        } else if (ch == ',') {
          numcommas++;
        }
        ptr++;
        ch = *ptr;
      }
      if (numspaces > 4 || numcommas > 0) {
        val = NULL;
      }
    }
    if (val != NULL) {
      ssp = SubSourceNew ();
      if (ssp != NULL) {
        ssp->subtype = (Uint1) i;
        ssp->name = StringSave (val);
        ssp->next = *sspp;
        *sspp = ssp;
      }
      *vnpp = vnp->next;
      vnp->next = NULL;
      ValNodeFreeData (vnp);
    } else {
      vnpp = &(vnp->next);
    }
    vnp = next;
  }
}

static void GbqualToOrpMod (GBQualPtr PNTR prevgbq, ValNodePtr PNTR vnpp)

{
  GBQualPtr  gbq;
  Int2       i;
  size_t     len;
  GBQualPtr  next;
  CharPtr    str;
  Boolean    unlink;
  CharPtr    val;

  if (prevgbq == NULL) return;
  gbq = *prevgbq;
  while (gbq != NULL) {
    next = gbq->next;
    unlink = FALSE;
    str = gbq->qual;
    if (str != NULL) {
      val = NULL;
      for (i = 0; unstructured_orgmod_list [i] != NULL; i++) {
        val = StringHasPrefix (str, unstructured_orgmod_list [i], FALSE, FALSE);
        if (val != NULL) break;
      }
      if (val == NULL) {
        for (i = 0; unstructured_subsource_list [i] != NULL; i++) {
          val = StringHasPrefix (str, unstructured_subsource_list [i],
                                 (Boolean) (i == SUBSRC_germline ||
                                            i == SUBSRC_rearranged ||
                                            i == SUBSRC_transgenic ||
                                            i == SUBSRC_environmental_sample),
                                 FALSE);
          if (val != NULL) break;
        }
      }
      if (val == NULL) {
        val = StringHasPrefix (str, "sub-clone", FALSE, FALSE);
        if (val != NULL) {
          val = "subclone";
        }
      }
      if (val != NULL) {
        len = StringLen (gbq->val);
        str = MemNew (sizeof (Char) * (len + 64));
        if (str != NULL) {
          StringCpy (str, val);
          StringCat (str, "=");
          StringCat (str, gbq->val);
          ValNodeAddStr (vnpp, 0, str);
          unlink = TRUE; 
        }
      }
    }
    if (unlink) {
      *prevgbq = gbq->next;
      gbq->next = NULL;
      GBQualFree (gbq);
    } else {
      prevgbq = (GBQualPtr PNTR) &(gbq->next);
    }
    gbq = next;
  }
}

static int LIBCALLBACK SortDbxref (VoidPtr ptr1, VoidPtr ptr2)

{
  int          compare;
  DbtagPtr     dbt1;
  DbtagPtr     dbt2;
  ObjectIdPtr  oip1;
  ObjectIdPtr  oip2;
  CharPtr      str1;
  CharPtr      str2;
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  dbt1 = (DbtagPtr) vnp1->data.ptrvalue;
  dbt2 = (DbtagPtr) vnp2->data.ptrvalue;
  if (dbt1 == NULL || dbt2 == NULL) return 0;
  str1 = (CharPtr) dbt1->db;
  str2 = (CharPtr) dbt2->db;
  if (str1 == NULL || str2 == NULL) return 0;
  compare = StringICmp (str1, str2);
  if (compare != 0) return compare;
  oip1 = dbt1->tag;
  oip2 = dbt2->tag;
  if (oip1 == NULL || oip2 == NULL) return 0;
  str1 = oip1->str;
  str2 = oip2->str;
  if (str1 != NULL && str2 != NULL) {
    return StringICmp (str1, str2);
  } else if (str1 == NULL && str2 == NULL) {
    if (oip1->id > oip2->id) {
      return 1;
    } else if (oip1->id < oip2->id) {
      return -1;
    }
  }
  return 0;
}

static void FixNumericDbxrefs (ValNodePtr vnp)

{
  Char         ch;
  DbtagPtr     dbt;
  Boolean      isNum;
  Boolean      leadingzero;
  Boolean      notallzero;
  ObjectIdPtr  oip;
  CharPtr      ptr;
  long         val;

  while (vnp != NULL) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL) {
      oip = dbt->tag;
      if (oip != NULL) {
        ptr = oip->str;
        if (ptr != NULL) {
          leadingzero = FALSE;
          notallzero = FALSE;
          isNum = TRUE;
          ch = *ptr;
          if (ch == '0') {
            leadingzero = TRUE;
          }
          while (ch != '\0') {
            if ((! IS_DIGIT (ch)) && (! IS_WHITESP (ch))) {
              isNum = FALSE;
            } else if ('1'<= ch && ch <='9') {
              notallzero = TRUE;
            }
            ptr++;
            ch = *ptr;
          }
          if (isNum) {
            if (leadingzero && notallzero) {
              /* suppress conversion */
            } else if (sscanf (oip->str, "%ld", &val) == 1) {
              oip->id = (Int4) val;
              oip->str = MemFree (oip->str);
            }
          }
        }
      }
    }
    vnp = vnp->next;
  }
}

static void FixOldDbxrefs (ValNodePtr vnp)

{
  DbtagPtr     dbt;

  while (vnp != NULL) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL) {
      if (StringICmp (dbt->db, "SWISS-PROT") == 0 &&
          StringCmp (dbt->db, "Swiss-Prot") != 0) {
        dbt->db = MemFree (dbt->db);
        dbt->db = StringSave ("Swiss-Prot");
      } else if (StringICmp (dbt->db, "SPTREMBL") == 0) {
        dbt->db = MemFree (dbt->db);
        dbt->db = StringSave ("TrEMBL");
      } else if (StringICmp (dbt->db, "SUBTILIS") == 0) {
        dbt->db = MemFree (dbt->db);
        dbt->db = StringSave ("SubtiList");
      }
    }
    vnp = vnp->next;
  }
}

static void CleanupDuplicateDbxrefs (ValNodePtr PNTR prevvnp)

{
  DbtagPtr     dbt;
  DbtagPtr     last = NULL;
  ValNodePtr   nextvnp;
  ObjectIdPtr  oip1;
  ObjectIdPtr  oip2;
  CharPtr      str1;
  CharPtr      str2;
  Boolean      unlink;
  ValNodePtr   vnp;

  if (prevvnp == NULL) return;
  vnp = *prevvnp;
  FixNumericDbxrefs (vnp);
  FixOldDbxrefs (vnp);
  while (vnp != NULL) {
    nextvnp = vnp->next;
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL) {
      unlink = FALSE;
      if (last != NULL) {
        str1 = (CharPtr) dbt->db;
        str2 = (CharPtr) last->db;
        if (str1 != NULL && str2 != NULL && StringICmp (str1, str2) == 0) {
          oip1 = dbt->tag;
          oip2 = last->tag;
          if (oip1 != NULL && oip2 != NULL) {
            str1 = oip1->str;
            str2 = oip2->str;
            if (str1 != NULL && str2 != NULL) {
              if (StringICmp (str1, str2) == 0) {
                unlink = TRUE;
              }
            } else if (str1 == NULL && str2 == NULL) {
              if (oip1->id == oip2->id) {
                unlink = TRUE;
              }
            }
          }
        }
      } else {
        last = dbt;
      }
      if (unlink) {
        *prevvnp = vnp->next;
        vnp->next = NULL;
        DbtagFree (dbt);
        ValNodeFree (vnp);
      } else {
        last = dbt;
        prevvnp = (ValNodePtr PNTR) &(vnp->next);
      }
    }
    vnp = nextvnp;
  }
}

static int LIBCALLBACK SortCits (VoidPtr ptr1, VoidPtr ptr2)

{
  int         compare;
  Char        label1 [128], label2 [128];
  ValNodePtr  ppr1, ppr2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  ppr1 = *((ValNodePtr PNTR) ptr1);
  ppr2 = *((ValNodePtr PNTR) ptr2);
  if (ppr1 == NULL || ppr2 == NULL) return 0;
  PubLabel (ppr1, label1, 127, OM_LABEL_CONTENT);
  PubLabel (ppr2, label2, 127, OM_LABEL_CONTENT);
  compare = StringICmp (label1, label2);
  return compare;
}

static Boolean CitGenTitlesMatch (ValNodePtr pub1, ValNodePtr pub2)

{
  CitGenPtr  cgp1, cgp2;

  if (pub1->choice == PUB_Gen) {
    cgp1 = (CitGenPtr) pub1->data.ptrvalue;
    if (cgp1->serial_number != -1 && pub1->next != NULL) {
      pub1 = pub1->next;
    }
  }
  if (pub2->choice == PUB_Gen) {
    cgp2 = (CitGenPtr) pub2->data.ptrvalue;
    if (cgp2->serial_number != -1 && pub2->next != NULL) {
      pub2 = pub2->next;
    }
  }

  if (pub1->choice != PUB_Gen || pub2->choice != PUB_Gen) return TRUE;
  cgp1 = (CitGenPtr) pub1->data.ptrvalue;
  cgp2 = (CitGenPtr) pub2->data.ptrvalue;
  if (cgp1->title == NULL || cgp2->title == NULL) return TRUE;
  if (StringCmp (cgp1->title, cgp2->title) != 0) return FALSE;
  return TRUE;
}

static void CleanupDuplicateCits (ValNodePtr PNTR prevvnp)

{
  Char        label1 [128], label2 [128];
  ValNodePtr  last = NULL;
  ValNodePtr  nextvnp;
  Boolean     unlink;
  ValNodePtr  vnp;

  if (prevvnp == NULL) return;
  vnp = *prevvnp;
  while (vnp != NULL) {
    nextvnp = vnp->next;
    unlink = FALSE;
    if (last != NULL) {
      PubLabelUnique (last, label1, 127, OM_LABEL_CONTENT, TRUE);
      PubLabelUnique (vnp, label2, 127, OM_LABEL_CONTENT, TRUE);
      if (StringCmp (label1, label2) == 0 && CitGenTitlesMatch (last, vnp)) {
        unlink = TRUE;
      }
    } else {
      last = vnp;
    }
    if (unlink) {
      *prevvnp = vnp->next;
      vnp->next = NULL;
      PubFree (vnp);
    } else {
      last = vnp;
      prevvnp = (ValNodePtr PNTR) &(vnp->next);
    }
    vnp = nextvnp;
  }
}

/* name processing code from Sequin editors */

static void FirstNameToInitials (CharPtr first, CharPtr inits, size_t maxsize)

{
  Char  ch;
  Int2  i;

  if (inits != NULL && maxsize > 0) {
    inits [0] = '\0';
    if (first != NULL) {
      i = 0;
      ch = *first;
      while (ch != '\0' && i < maxsize) {
        while (ch != '\0' && (ch <= ' ' || ch == '-')) {
          first++;
          ch = *first;
        }
        if (IS_ALPHA (ch)) {
          inits [i] = ch;
          i++;
          first++;
          ch = *first;
        }
        while (ch != '\0' && ch > ' ' && ch != '-') {
          first++;
          ch = *first;
        }
        if (ch == '-') {
          inits [i] = ch;
          i++;
          first++;
          ch = *first;
        }
      }
      inits [i] = '\0';
    }
  }
}

static void StripPeriods (CharPtr str)

{
  Char     ch;
  CharPtr  dst;

  if (str != NULL) {
    dst = str;
    ch = *str;
    while (ch != '\0') {
      if (ch != '.') {
        *dst = ch;
        dst++;
      }
      str++;
      ch = *str;
    }
    *dst = '\0';
  }
}

static void TrimLeadingSpaces (CharPtr str)

{
  Char     ch;
  CharPtr  dst;

  if (str != NULL && str [0] != '\0') {
    dst = str;
    ch = *str;
    while (ch != '\0' && ch <= ' ') {
      str++;
      ch = *str;
    }
    while (ch != '\0') {
      *dst = ch;
      dst++;
      str++;
      ch = *str;
    }
    *dst = '\0';
  }
}

static void ExtractSuffixFromInitials (NameStdPtr nsp)

{
  Char     ch;
  Boolean  has_period = FALSE;
  size_t   len;
  CharPtr  str;

  str = nsp->names [4];
  ch = *str;
  while (ch != '\0') {
    if (ch == '.') {
      has_period = TRUE;
    }
    str++;
    ch = *str;
  }
  if (! has_period) return;
  str = nsp->names [4];
  len = StringLen (str);
  if (len >= 4 && StringCmp (str +  len - 3, "III") == 0) {
    str [len - 3] = '\0';
    nsp->names [5] = StringSave ("III");
  } else if (len >= 5 && StringCmp (str +  len - 4, "III.") == 0) {
    str [len - 4] = '\0';
    nsp->names [5] = StringSave ("III");
  } else if (len >= 3 && StringCmp (str +  len - 2, "Jr") == 0) {
    str [len - 2] = '\0';
    nsp->names [5] = StringSave ("Jr");
  } else if (len >= 4 && StringCmp (str +  len - 3, "2nd") == 0) {
    str [len - 3] = '\0';
    nsp->names [5] = StringSave ("II");
  } else if (len >= 3 && StringCmp (str +  len - 2, "IV") == 0) {
    str [len - 2] = '\0';
    nsp->names [5] = StringSave ("IV");
  } else if (len >= 4 && StringCmp (str +  len - 3, "IV.") == 0) {
    str [len - 3] = '\0';
    nsp->names [5] = StringSave ("IV");
  }
}

static CharPtr NameStdPtrToTabbedString (NameStdPtr nsp, Boolean fixInitials)

{
  Char   first [256];
  Char   frstinits [64];
  Char   initials [64];
  Int2   j;
  Char   last [256];
  Char   middle [128];
  Char   str [512];
  Char   suffix [64];

  if (nsp == NULL) return NULL;
  if (nsp->names [5] == NULL && nsp->names [4] != NULL) {
    ExtractSuffixFromInitials (nsp);
  }
  str [0] = '\0';
  StringNCpy_0 (first, nsp->names [1], sizeof (first));
  TrimSpacesAroundString (first);
  StringNCpy_0 (initials, nsp->names [4], sizeof (initials));
  StripPeriods (initials);
  TrimLeadingSpaces (initials);
  StringNCpy_0 (last, nsp->names [0], sizeof (last));
  TrimLeadingSpaces (last);
  StringNCpy_0 (middle, nsp->names [2], sizeof (middle));
  TrimLeadingSpaces (middle);
  if (StringCmp (initials, "al") == 0 &&
      StringCmp (last, "et") == 0 &&
      first [0] == '\0') {
    initials [0] = '\0';
    StringCpy (last, "et al.");
  }
  /*
  if (first [0] == '\0') {
    StringNCpy_0 (first, initials, sizeof (first));
    if (IS_ALPHA (first [0])) {
      if (first [1] == '-') {
        first [3] = '\0';
      } else {
        first [1] = '\0';
      }
    } else {
      first [0] = '\0';
    }
  }
  */
  frstinits [0] = '\0';
  FirstNameToInitials (first, frstinits, sizeof (frstinits) - 1);
  StripPeriods (first);
  TrimLeadingSpaces (first);
  if (first [0] != '\0') {
    StringCat (str, first);
  } else {
    /*
    StringCat (str, " ");
    */
  }
  StringCat (str, "\t");
  if (fixInitials) {
    j = 0;
    while (initials [j] != '\0' && TO_UPPER (initials [j]) == TO_UPPER (frstinits [j])) {
      j++;
    }
    if (initials [j] != '\0') {
      StringCat (str, initials + j);
    } else {
      /*
      StringCat (str, " ");
      */
    }
  } else if (initials [0] != '\0') {
    StringCat (str, initials);
  } else if (frstinits [0] != '\0') {
    StringCat (str, frstinits);
  }
  StringCat (str, "\t");
  StringCat (str, last);
  StringNCpy_0 (suffix, nsp->names [5], sizeof (suffix));
  StringCat (str, "\t");
  StripPeriods (suffix);
  TrimLeadingSpaces (suffix);
  if (suffix [0] != '\0') {
    StringCat (str, suffix);
  } else {
    /*
    StringCat (str, " ");
    */
  }
  StringCat (str, "\t");
  StringCat (str, middle);
  StringCat (str, "\n");
  return StringSave (str);
}

static CharPtr XtractTagListColumn (CharPtr source, Int2 col)

{
  Char     ch;
  size_t   count;
  CharPtr  ptr;
  CharPtr  str;

  if (source == NULL || source [0] == '\0' || col < 0) return NULL;

  ptr = source;
  ch = *ptr;
  while (col > 0 && ch != '\n' && ch != '\0') {
    while (ch != '\t' && ch != '\n' && ch != '\0') {
      ptr++;
      ch = *ptr;
    }
    if (ch == '\t') {
      ptr++;
      ch = *ptr;
    }
    col--;
  }

  count = 0;
  ch = ptr [count];
  while (ch != '\t' && ch != '\n' && ch != '\0') {
    count++;
    ch = ptr [count];
  }
  str = (CharPtr) MemNew(count + 1);
  if (str != NULL) {
    MemCpy (str, ptr, count);
  }
  return str;
}

static NameStdPtr TabbedStringToNameStdPtr (CharPtr txt, Boolean fixInitials)

{
  Char        ch;
  CharPtr     first;
  Char        initials [64];
  Int2        j;
  Int2        k;
  Char        last;
  Int2        len;
  NameStdPtr  nsp;
  Char        periods [128];
  CharPtr     str;
  Char        str1 [64];
  Char        suffix [80];

  if (txt == NULL) return NULL;
  nsp = NameStdNew ();
  if (nsp == NULL) return NULL;
  nsp->names [0] = XtractTagListColumn (txt, 2);
  TrimLeadingSpaces (nsp->names [0]);
  first = XtractTagListColumn (txt, 0);
  StripPeriods (first);
  nsp->names [1] = StringSave (first);
  TrimLeadingSpaces (nsp->names [1]);
  str1 [0] = '\0';
  if (fixInitials) {
    FirstNameToInitials (first, str1, sizeof (str1) - 1);
  }
  str = XtractTagListColumn (txt, 1);
  StringNCat (str1, str, sizeof (str1) - 1);
  MemFree (str);
  j = 0;
  k = 0;
  ch = str1 [j];
  while (ch != '\0') {
    if (ch != ' ') {
      initials [k] = ch;
      k++;
    }
    j++;
    ch = str1 [j];
  }
  initials [k] = '\0';
  periods [0] = '\0';
          j = 0;
          ch = initials [j];
          while (ch != '\0') {
            if (ch == ',') {
              initials [j] = '.';
            }
            j++;
            ch = initials [j];
          }
          str = StringStr (initials, ".ST.");
          if (str != NULL) {
            *(str + 2) = 't';
          }
  j = 0;
  k = 0;
  ch = initials [j];
  while (ch != '\0') {
    if (ch == '-') {
      periods [k] = ch;
      k++;
      j++;
      ch = initials [j];
    } else if (ch == '.') {
      j++;
      ch = initials [j];
            } else if (ch == ' ') {
              j++;
              ch = initials [j];
    } else {
      periods [k] = ch;
              last = ch;
      k++;
      j++;
      ch = initials [j];
              if (ch == '\0') {
                if (! (IS_LOWER (last))) {
                  periods [k] = '.';
                  k++;
                }
              /* } else if (ch == '.' && initials [j + 1] == '\0') { */
              } else if (! (IS_LOWER (ch))) {
                periods [k] = '.';
                k++;
              }
    }
  }
  if (k > 0 && periods [k - 1] != '.') {
    periods [k] = '.';
    k++;
  }
  periods [k] = '\0';
  nsp->names [4] = StringSave (periods);
  TrimLeadingSpaces (nsp->names [4]);
  str = XtractTagListColumn (txt, 3);
  StringNCpy_0 (str1, str, sizeof (str1));
  MemFree (str);
  j = 0;
  k = 0;
  ch = str1 [j];
  while (ch != '\0') {
    if (ch != ' ') {
      suffix [k] = ch;
      k++;
    }
    j++;
    ch = str1 [j];
  }
  suffix [k] = '\0';
  if (suffix [0] != '\0') {
    len = StringLen (suffix);
    if (len > 0 && suffix [len - 1] == '.') {
      suffix [len - 1] = '\0';
    }
    if (StringICmp (suffix, "1d") == 0) {
      StringCpy (suffix, "I");
    } else if (StringICmp (suffix, "1st") == 0) {
      StringCpy (suffix, "I");
    } else if (StringICmp (suffix, "2d") == 0) {
      StringCpy (suffix, "II");
    } else if (StringICmp (suffix, "2nd") == 0) {
      StringCpy (suffix, "II");
    } else if (StringICmp (suffix, "3d") == 0) {
      StringCpy (suffix, "III");
    } else if (StringICmp (suffix, "3rd") == 0) {
      StringCpy (suffix, "III");
    } else if (StringICmp (suffix, "4th") == 0) {
      StringCpy (suffix, "IV");
    } else if (StringICmp (suffix, "5th") == 0) {
      StringCpy (suffix, "V");
    } else if (StringICmp (suffix, "6th") == 0) {
      StringCpy (suffix, "VI");
    } else if (StringICmp (suffix, "Sr") == 0) {
      StringCpy (suffix, "Sr.");
    } else if (StringICmp (suffix, "Jr") == 0) {
      StringCpy (suffix, "Jr.");
    }
    /*
    len = StringLen (suffix);
    if (len > 0 && suffix [len - 1] != '.') {
      StringCat (suffix, ".");
    }
    */
    nsp->names [5] = StringSave (suffix);
    TrimLeadingSpaces (nsp->names [5]);
  }
  if (StringCmp (nsp->names [0], "et al") == 0) {
    nsp->names [0] = MemFree (nsp->names [0]);
    nsp->names [0] = StringSave ("et al.");
  }
  nsp->names [2] = XtractTagListColumn (txt, 4);
  TrimLeadingSpaces (nsp->names [2]);
  if (StringHasNoText (nsp->names [2])) {
    nsp->names [2] = MemFree (nsp->names [2]);
  }
  MemFree (first);
  return nsp;
}

static AffilPtr CleanAffil (AffilPtr afp)

{
  if (afp == NULL) return NULL;
  CleanVisString (&(afp->affil));
  CleanVisString (&(afp->div));
  CleanVisString (&(afp->city));
  CleanVisString (&(afp->sub));
  CleanVisString (&(afp->country));
  CleanVisString (&(afp->street));
  CleanVisString (&(afp->email));
  CleanVisString (&(afp->fax));
  CleanVisString (&(afp->phone));
  CleanVisString (&(afp->postal_code));
  if (afp->affil == NULL &&
      afp->div == NULL &&
      afp->city == NULL &&
      afp->sub == NULL &&
      afp->country == NULL &&
      afp->street == NULL &&
      afp->email == NULL &&
      afp->fax == NULL &&
      afp->phone == NULL &&
      afp->postal_code == NULL) {
    afp = MemFree (afp);
  }
  return afp;
}

static void NormalizeAuthors (AuthListPtr alp, Boolean fixInitials)

{
  AuthorPtr    ap;
  CharPtr      initials;
  ValNodePtr   names;
  NameStdPtr   nsp;
  PersonIdPtr  pid;
  CharPtr      str;
  Boolean      upcaseinits;
  /*
  Char         ch;
  CharPtr      initials;
  Int2         j;
  Int2         k;
  Char         last;
  size_t       len;
  CharPtr      periods;
  */

  if (alp == NULL) return;
  alp->affil = CleanAffil (alp->affil);

#ifndef SUPPRESS_TRIVIAL_FLATFILE_DIFFERENCES
  if (alp == NULL || alp->choice != 1) return;
  for (names = alp->names; names != NULL; names = names->next) {
    ap = names->data.ptrvalue;
    if (ap != NULL) {
      pid = ap->name;
      if (pid == NULL) continue;
      if (pid->choice == 2) {
        nsp = pid->data;
        if (nsp != NULL /* && nsp->names [4] != NULL */) {
          upcaseinits = FALSE;
          initials = nsp->names [4];
          if (StringLen (initials) > 0) {
            if (IS_UPPER (initials [0])) {
              upcaseinits = TRUE;
            }
          }
          str = NameStdPtrToTabbedString (nsp, fixInitials);
          pid->data = NameStdFree (nsp);
          nsp = TabbedStringToNameStdPtr (str, fixInitials);
          if (upcaseinits) {
            initials = nsp->names [4];
            if (StringLen (initials) > 0) {
              if (IS_LOWER (initials [0])) {
                initials [0] = TO_UPPER (initials [0]);
              }
            }
          }
          pid->data = nsp;
          MemFree (str);
#if 0
          initials = nsp->names [4];
          len = MAX ((size_t) (StringLen (initials) * 2 + 4), (size_t) 64);
          periods = MemNew (len);
          if (periods == NULL) return;
          periods [0] = '\0';
          j = 0;
          ch = initials [j];
          while (ch != '\0') {
            if (ch == ',') {
              initials [j] = '.';
            }
            j++;
            ch = initials [j];
          }
          str = StringStr (initials, ".ST.");
          if (str != NULL) {
            *(str + 2) = 't';
          }
          j = 0;
          k = 0;
          ch = initials [j];
          while (ch != '\0') {
            if (ch == '-') {
              periods [k] = ch;
              k++;
              j++;
              ch = initials [j];
            } else if (ch == '.') {
              j++;
              ch = initials [j];
            } else if (ch == ' ') {
              j++;
              ch = initials [j];
            } else {
              periods [k] = ch;
              last = ch;
              k++;
              j++;
              ch = initials [j];
              if (ch == '\0') {
                if (! (IS_LOWER (last))) {
                  periods [k] = '.';
                  k++;
                }
              /* } else if (ch == '.' && initials [j + 1] == '\0') { */
              } else if (! (IS_LOWER (ch))) {
                periods [k] = '.';
                k++;
              }
            }
          }
          periods [k] = '\0';
          nsp->names [4] = MemFree (nsp->names [4]);
          nsp->names [4] = StringSave (periods);
          MemFree (periods);
#endif
          CleanVisString (&(nsp->names [0]));
          CleanVisString (&(nsp->names [1]));
          CleanVisString (&(nsp->names [2]));
          CleanVisString (&(nsp->names [3]));
          CleanVisString (&(nsp->names [4]));
          CleanVisString (&(nsp->names [5]));
          CleanVisString (&(nsp->names [6]));
          if (StringCmp (nsp->names [0], "et") == 0 &&
              (StringCmp (nsp->names [4], "al") == 0 || StringCmp (nsp->names [4], "al.") == 0) &&
              (StringHasNoText (nsp->names [1]) || StringCmp (nsp->names [1], "a") == 0)) {
            nsp->names [4] = MemFree (nsp->names [4]);
            nsp->names [1] = MemFree (nsp->names [1]);
            nsp->names [0] = MemFree (nsp->names [0]);
            nsp->names [0] = StringSave ("et al.");
          }
        }
      } else if (pid->choice == 3 || pid->choice == 4 || pid->choice == 5) {
        TrimSpacesAroundString ((CharPtr) pid->data);
      }
    }
  }
#endif
}

/* from utilpub.c */
static Boolean empty_citgen(CitGenPtr  cit)
{
	if (cit == NULL)
		return TRUE;
	if (cit->cit)
		return FALSE;
	if (cit->authors)
		return FALSE;
	if (cit->muid > 0)
		return FALSE;
	if (cit->journal)
		return FALSE;
	if (cit->volume)
		return FALSE;
	if (cit->issue)
		return FALSE;
	if (cit->pages)
		return FALSE;
	if (cit->date)
		return FALSE;
	if (cit->serial_number > 0)
		return FALSE;
	if (cit->title)
		return FALSE;
	if (cit->pmid > 0)
		return FALSE;
	return TRUE;
}

static void NormalizeAPub (ValNodePtr vnp, Boolean stripSerial, Boolean fixInitials)

{
  AffilPtr     affil;
  AuthListPtr  alp;
  CitArtPtr    cap;
  CitBookPtr   cbp;
  CitGenPtr    cgp;
  CitPatPtr    cpp;
  CitSubPtr    csp;
  ImprintPtr   imp;
  CharPtr      str;
  CharPtr      tmp;

  if (vnp == NULL) return;
  if (vnp->choice == PUB_PMid || vnp->choice == PUB_Muid) return;
  if (vnp->data.ptrvalue == NULL) return;
  switch (vnp->choice) {
    case PUB_Gen :
      cgp = (CitGenPtr) vnp->data.ptrvalue;
      NormalizeAuthors (cgp->authors, fixInitials);
      if (stripSerial) {
        cgp->serial_number = -1; /* but does not remove if empty */
      }
      if (StringNICmp (cgp->cit, "unpublished", 11) == 0) {
        cgp->cit [0] = 'U';
#ifndef SUPPRESS_TRIVIAL_FLATFILE_DIFFERENCES
        /* cgp->date = DateFree (cgp->date); */ /* remove date if unpublished */
        if (cgp->journal == NULL) {
          cgp->volume = MemFree (cgp->volume);
          cgp->issue = MemFree (cgp->issue);
          cgp->pages = MemFree (cgp->pages);
        }
#endif
      }
      TrimSpacesAroundString (cgp->cit);
      break;
    case PUB_Sub :
      csp = (CitSubPtr) vnp->data.ptrvalue;
      NormalizeAuthors (csp->authors, fixInitials);
      alp = csp->authors;
      imp = csp->imp;
      if (alp != NULL && alp->affil == NULL && imp != NULL && imp->pub != NULL) {
        alp->affil = imp->pub;
        imp->pub = NULL;
      }
      if (csp->date == NULL && imp != NULL && imp->date != NULL) {
        csp->date = imp->date;
        imp->date = NULL;
      }
      if (imp != NULL && imp->pub == NULL) {
        csp->imp = ImprintFree (csp->imp);
      }
      if (alp != NULL && alp->affil != NULL) {
        affil = alp->affil;
        if (affil->choice == 1) {
          str = affil->affil;
          if (StringNICmp (str, "to the ", 7) == 0) {
            if (StringNICmp (str + 24, " databases", 10) == 0) {
              str += 34;
              if (*str == '.') {
                str++;
              }
              tmp = StringSaveNoNull (TrimSpacesAroundString (str));
              affil->affil = MemFree (affil->affil);
              affil->affil = tmp;
              alp->affil = CleanAffil (alp->affil);
            }
          }
        }
      }
      break;
    case PUB_Article :
      cap = (CitArtPtr) vnp->data.ptrvalue;
      NormalizeAuthors (cap->authors, fixInitials);
      if (cap->from == 2 || cap->from == 3) {
        cbp = (CitBookPtr) cap->fromptr;
        if (cbp != NULL) {
          NormalizeAuthors (cbp->authors, fixInitials);
        }
      }
      break;
    case PUB_Book :
      cbp = (CitBookPtr) vnp->data.ptrvalue;
      NormalizeAuthors (cbp->authors, fixInitials);
      break;
    case PUB_Man :
      cbp = (CitBookPtr) vnp->data.ptrvalue;
      if (cbp->othertype == 2 && cbp->let_type == 3) {
        NormalizeAuthors (cbp->authors, fixInitials);
      }
      break;
    case PUB_Patent :
      cpp = (CitPatPtr) vnp->data.ptrvalue;
      NormalizeAuthors (cpp->authors, fixInitials);
      NormalizeAuthors (cpp->applicants, fixInitials);
      NormalizeAuthors (cpp->assignees, fixInitials);
      break;
    default :
      break;
  }
}

static Boolean IsOnlinePub (PubdescPtr pdp)

{
  CitGenPtr   cgp;
  ValNodePtr  vnp;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == PUB_Gen) {
      cgp = (CitGenPtr) vnp->data.ptrvalue;
      if (cgp != NULL) {
        if (StringNICmp (cgp->cit, "Online Publication", 18) == 0) {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static void NormalizePubdesc (PubdescPtr pdp, Boolean stripSerial, ValNodePtr PNTR publist)

{
  Char             buf1 [121];
  Char             buf2 [121];
  CitGenPtr        cgp;
  Boolean          fixInitials = TRUE;
  Boolean          hasArt = FALSE;
  Boolean          hasUid = FALSE;
  ValNodePtr       next;
  ValNodePtr PNTR  prev;
  ValNodePtr       vnp;

  if (pdp == NULL) return;
  if (IsOnlinePub (pdp)) {
    TrimSpacesAroundString (pdp->comment);
    if (StringHasNoText (pdp->comment)) {
      pdp->comment = MemFree (pdp->comment);
    }
  } else {
    CleanVisString (&(pdp->comment));
  }
  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == PUB_Muid || vnp->choice == PUB_PMid) {
      if (vnp->data.intvalue > 0) {
        hasUid = TRUE;
      }
    } else if (vnp->choice == PUB_Article) {
      hasArt = TRUE;
    }
  }
  if (hasArt && hasUid) {
    fixInitials = FALSE;
  }
  prev = &(pdp->pub);
  vnp = pdp->pub;
  if (vnp != NULL && vnp->next == NULL && vnp->choice == PUB_Gen) {
    cgp = (CitGenPtr) vnp->data.ptrvalue;
    buf1 [0] = '\0';
    PubLabelUnique (vnp, buf1, sizeof (buf1) - 1, OM_LABEL_CONTENT, TRUE);
    NormalizeAuthors (cgp->authors, fixInitials);
    if (stripSerial) {
      cgp->serial_number = -1;
    }
    if (StringNICmp (cgp->cit, "unpublished", 11) == 0) {
      cgp->cit [0] = 'U';
#ifndef SUPPRESS_TRIVIAL_FLATFILE_DIFFERENCES
      /* cgp->date = DateFree (cgp->date); */ /* remove date if unpublished */
      if (cgp->journal == NULL) {
        cgp->volume = MemFree (cgp->volume);
        cgp->issue = MemFree (cgp->issue);
        cgp->pages = MemFree (cgp->pages);
      }
#endif
    }
    TrimSpacesAroundString (cgp->cit);
    buf2 [0] = '\0';
    PubLabelUnique (vnp, buf2, sizeof (buf2) - 1, OM_LABEL_CONTENT, TRUE);
    if (StringCmp (buf1, buf2) != 0) {
      ValNodeCopyStr (publist, 1, buf1);
      ValNodeCopyStr (publist, 2, buf2);
    }
    return; /* but does not remove if empty and only element of Pub */
  }
  while (vnp != NULL) {
    next = vnp->next;
    buf1 [0] = '\0';
    PubLabelUnique (vnp, buf1, sizeof (buf1) - 1, OM_LABEL_CONTENT, TRUE);
    NormalizeAPub (vnp, stripSerial, fixInitials);
    if (vnp->choice == PUB_Gen && empty_citgen ((CitGenPtr) vnp->data.ptrvalue)) {
      *prev = vnp->next;
      vnp->next = NULL;
      PubFree (vnp);
    } else {
      prev = &(vnp->next);
      buf2 [0] = '\0';
      PubLabelUnique (vnp, buf2, sizeof (buf2) - 1, OM_LABEL_CONTENT, TRUE);
      if (StringCmp (buf1, buf2) != 0) {
        ValNodeCopyStr (publist, 1, buf1);
        ValNodeCopyStr (publist, 2, buf2);
      }
    }
    vnp = next;
  }
}

static Boolean KeywordAlreadyInList (ValNodePtr head, CharPtr kwd)

{
  ValNodePtr  vnp;

  if (head == NULL || kwd == NULL) return FALSE;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (StringICmp ((CharPtr) vnp->data.ptrvalue, kwd) == 0) return TRUE;
  }

  return FALSE;
}

static Boolean CopyGeneXrefToGeneFeat (GeneRefPtr grp, GeneRefPtr grx)

{
  if (grp == NULL || grx == NULL) return FALSE;
  if (grx->db != NULL) {
    ValNodeLink (&(grp->db), grx->db);
    grx->db = NULL;
  }
  if (grx->locus == NULL && grx->allele == NULL &&
      grx->desc == NULL && grx->maploc == NULL &&
      grx->locus_tag == NULL && grx->db == NULL &&
      grx->syn == NULL) return TRUE;
  return FALSE;
}

static void HandleXrefOnGene (SeqFeatPtr sfp)

{
  GeneRefPtr           grp;
  GeneRefPtr           grx;
  SeqFeatXrefPtr       next;
  SeqFeatXrefPtr PNTR  prev;
  SeqFeatXrefPtr       xref;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_GENE) return;
  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp == NULL) return;
   prev = &(sfp->xref);
  xref = sfp->xref;
  while (xref != NULL) {
    next = xref->next;
    if (xref->data.choice == SEQFEAT_GENE) {
      grx = (GeneRefPtr) xref->data.value.ptrvalue;
      if (CopyGeneXrefToGeneFeat (grp, grx)) {
        *(prev) = next;
        xref->next = NULL;
        SeqFeatXrefFree (xref);
      } else {
        prev = &(xref->next);
      }
    } else {
      prev = &(xref->next);
    }
    xref = next;
  }
}

static void CopyProtXrefToProtFeat (ProtRefPtr prp, ProtRefPtr prx)

{
  ValNodePtr       curr;
  size_t           len;
  ValNodePtr       next;
  ValNodePtr PNTR  prev;
  CharPtr          str;

  if (prp == NULL || prx == NULL) return;

  if (prx->db != NULL) {
    ValNodeLink (&(prp->db), prx->db);
    prx->db = NULL;
  }

  prev = &(prx->name);
  curr = prx->name;
  while (curr != NULL) {
    next = curr->next;
    str = (CharPtr) curr->data.ptrvalue;
    if (! KeywordAlreadyInList (prp->name, str)) {
      ValNodeCopyStr (&(prp->name), 0, str);
      *(prev) = next;
      curr->next = NULL;
      curr->data.ptrvalue = NULL;
      ValNodeFree (curr);
    } else {
      prev = &(curr->next);
    }
    curr = next;
  }

  if (prp->desc == NULL) {
    prp->desc = prx->desc;
    prx->desc = NULL;
  } else if (prx->desc != NULL) {
    if (StringCmp (prx->desc, prp->desc) != 0) {
      len = StringLen (prp->desc) + StringLen (prx->desc) + 6;
      str = MemNew (len);
      if (str != NULL) {
        StringCpy (str, prp->desc);
        StringCat (str, "; ");
        StringCat (str, prx->desc);
        prp->desc = MemFree (prp->desc);
        prp->desc = str;
      }
    }
  }

  prev = &(prx->ec);
  curr = prx->ec;
  while (curr != NULL) {
    next = curr->next;
    str = (CharPtr) curr->data.ptrvalue;
    if (! KeywordAlreadyInList (prp->ec, str)) {
      ValNodeCopyStr (&(prp->ec), 0, str);
      *(prev) = next;
      curr->next = NULL;
      curr->data.ptrvalue = NULL;
      ValNodeFree (curr);
    } else {
      prev = &(curr->next);
    }
    curr = next;
  }

  prev = &(prx->activity);
  curr = prx->activity;
  while (curr != NULL) {
    next = curr->next;
    str = (CharPtr) curr->data.ptrvalue;
    if (! KeywordAlreadyInList (prp->activity, str)) {
      ValNodeCopyStr (&(prp->activity), 0, str);
      curr->data.ptrvalue = NULL;
    }
    *(prev) = next;
    curr->next = NULL;
    curr->data.ptrvalue = NULL;
    ValNodeFree (curr);
    curr = next;
  }
}

static Boolean InGpsGenomic (SeqFeatPtr sfp)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;

  if (sfp == NULL) return FALSE;
  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp == NULL) return FALSE;
  if (bsp->idx.parenttype == OBJ_BIOSEQSET) {
    bssp = (BioseqSetPtr) bsp->idx.parentptr;
    while (bssp != NULL) {
      if (bssp->_class == BioseqseqSet_class_nuc_prot) return FALSE;
      if (bssp->_class == BioseqseqSet_class_gen_prod_set) return TRUE;
      if (bssp->idx.parenttype != OBJ_BIOSEQSET) return FALSE;
      bssp = (BioseqSetPtr) bssp->idx.parentptr;
    }
  }
  return FALSE;
}

static void HandleXrefOnCDS (SeqFeatPtr sfp)

{
  SeqFeatXrefPtr       next;
  SeqFeatXrefPtr PNTR  prev;
  SeqFeatPtr           prot;
  ProtRefPtr           prp;
  ProtRefPtr           prx;
  SeqFeatXrefPtr       xref;

  if (sfp != NULL && sfp->product != NULL) {
    if (InGpsGenomic (sfp)) return;
    prot = GetBestProteinFeatureUnindexed (sfp->product);
    if (prot != NULL) {
      prp = (ProtRefPtr) prot->data.value.ptrvalue;
      if (prp != NULL) {
        prev = &(sfp->xref);
        xref = sfp->xref;
        while (xref != NULL) {
          next = xref->next;
          if (xref->data.choice == SEQFEAT_PROT) {
            prx = (ProtRefPtr) xref->data.value.ptrvalue;
            CopyProtXrefToProtFeat (prp, prx);
            *(prev) = next;
            xref->next = NULL;
            SeqFeatXrefFree (xref);
          } else {
            prev = &(xref->next);
          }
          xref = next;
        }
      }
    }
  }
}

static void CleanUserStrings (
  UserFieldPtr ufp,
  Pointer userdata
)

{
  ObjectIdPtr  oip;

  oip = ufp->label;
  if (oip != NULL && oip->str != NULL) {
    if (! StringHasNoText (oip->str)) {
      CleanVisString (&(oip->str));
    }
  }
  if (ufp->choice == 1) {
    if (! StringHasNoText ((CharPtr) ufp->data.ptrvalue)) {
      CleanVisString ((CharPtr PNTR) &(ufp->data.ptrvalue));
    }
  }
}

static void CleanUserFields (
  UserFieldPtr ufp,
  Pointer userdata
)

{
  ObjectIdPtr  oip;

  oip = ufp->label;
  if (oip != NULL && oip->str != NULL) {
    if (! StringHasNoText (oip->str)) {
      CleanVisString (&(oip->str));
    }
  }
  VisitUserFieldsInUfp (ufp, userdata, CleanUserStrings);
}

static void CleanUserObject (
  UserObjectPtr uop,
  Pointer userdata
)

{
  ObjectIdPtr  oip;

  oip = uop->type;
  if (oip != NULL && oip->str != NULL) {
    if (! StringHasNoText (oip->str)) {
      CleanVisString (&(oip->str));
    }
  }
  VisitUserFieldsInUop (uop, userdata, CleanUserFields);
}

static CharPtr siteList [] = {
  "", "active", "binding", "cleavage", "inhibit", "modifi",
  "glycosylation", "myristoylation", "mutagenized", "metal-binding",
  "phosphorylation", "acetylation", "amidation", "methylation",
  "hydroxylation", "sulfatation", "oxidative-deamination",
  "pyrrolidone-carboxylic-acid", "gamma-carboxyglutamic-acid",
  "blocked", "lipid-binding", "np-binding", "DNA-binding",
  "signal-peptide", "transit-peptide", "transmembrane-region", NULL
};

static void StrStripSpaces (
  CharPtr str
)

{
  CharPtr  new_str;

  if (str == NULL) return;

  new_str = str;
  while (*str != '\0') {
    *new_str++ = *str;
    if (*str == ' ' || *str == '\t' || *str == '(') {
      for (str++; *str == ' ' || *str == '\t'; str++) continue;
      if (*str == ')' || *str == ',') {
        new_str--;
      }
    } else {
      str++;
    }
  }
  *new_str = '\0';
}

static CharPtr uninfStrings [] = {
  "signal",
  "transit",
  "peptide",
  "signal peptide",
  "signal-peptide",
  "signal_peptide",
  "transit peptide",
  "transit-peptide",
  "transit_peptide",
  "unnamed",
  "unknown",
  "putative",
  NULL
};

static Boolean InformativeString (CharPtr str)

{
  Int2  i;

  if (StringHasNoText (str)) return FALSE;

  for (i = 0; uninfStrings [i] != NULL; i++) {
    if (StringICmp (str, uninfStrings [i]) == 0) return FALSE;
  }

  return TRUE;
}

static void CleanupFeatureStrings (SeqFeatPtr sfp, Boolean stripSerial, ValNodePtr PNTR publist)

{
  Uint1         aa;
  BioSourcePtr  biop;
  Char          ch;
  Uint1         codon [6];
  GeneRefPtr    grp;
  ImpFeatPtr    ifp;
  Boolean       is_fMet = FALSE;
  Int2          j;
  Boolean       justTrnaText;
  size_t        len;
  CharPtr       name;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  PubdescPtr    pdp;
  ProtRefPtr    prp;
  CharPtr       ptr;
  RnaRefPtr     rrp;
  CharPtr       str;
  CharPtr       suff;
  CharPtr       temp;
  Char          tmp [64];
  Boolean       trimming_junk;
  tRNAPtr       trp;
  CharPtr       val;
  ValNodePtr    vnp;

  if (sfp == NULL) return;
  CleanVisString (&(sfp->comment));
  CleanVisString (&(sfp->title));
  CleanVisString (&(sfp->except_text));
  CleanDoubleQuote (sfp->comment);
  if (StringCmp (sfp->comment, ".") == 0) {
    sfp->comment = MemFree (sfp->comment);
  }
  switch (sfp->data.choice) {
    case SEQFEAT_BOND :
    case SEQFEAT_PSEC_STR :
    case SEQFEAT_COMMENT:
      return;
    case SEQFEAT_SITE :
      for (j = 0; siteList [j] != NULL; j++) {
        StringNCpy_0 (tmp, siteList [j], sizeof (tmp));
        len = StringLen (tmp);
        if (StringNICmp (sfp->comment, tmp, len) == 0) {
          if (sfp->data.value.intvalue == 0 || sfp->data.value.intvalue == 255) {
            sfp->data.value.intvalue = j;
            if (StringHasNoText (sfp->comment + len) || StringICmp (sfp->comment + len, " site") == 0) {
              sfp->comment = MemFree (sfp->comment);
            }
          }
        } else {
          val = tmp;
          ch = *val;
          while (ch != '\0') {
            if (ch == '-') {
              *val = ' ';
            }
            val++;
            ch = *val;
          }
          if (StringNICmp (sfp->comment, tmp, len) == 0) {
            if (sfp->data.value.intvalue == 0 || sfp->data.value.intvalue == 255) {
              sfp->data.value.intvalue = j;
              if (StringHasNoText (sfp->comment + len) || StringICmp (sfp->comment + len, " site") == 0) {
                sfp->comment = MemFree (sfp->comment);
              }
            }
          }
        }
      }
      break;
    default :
      break;
  }
  if (sfp->data.value.ptrvalue == NULL) return;

  orp = NULL;
  switch (sfp->data.choice) {
    case SEQFEAT_ORG :
      orp = (OrgRefPtr) sfp->data.value.ptrvalue;
      break;
    case SEQFEAT_BIOSRC :
      biop = (BioSourcePtr) sfp->data.value.ptrvalue;
      orp = biop->org;
    default :
      break;
  }
  if (orp != NULL && sfp->qual != NULL) {
    GbqualToOrpMod (&(sfp->qual), &(orp->mod));
  }

  orp = NULL;
  switch (sfp->data.choice) {
    case SEQFEAT_GENE :
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      if (sfp->xref != NULL) {
        HandleXrefOnGene (sfp);
      }
      CleanVisString (&(grp->locus));
      CleanVisString (&(grp->allele));
      CleanVisString (&(grp->desc));
      CleanVisString (&(grp->maploc));
      CleanVisString (&(grp->locus_tag));
      CleanVisStringList (&(grp->syn));
      CleanDoubleQuote (grp->locus);
      CleanDoubleQuote (grp->allele);
      CleanDoubleQuote (grp->desc);
      CleanDoubleQuote (grp->maploc);
      CleanDoubleQuote (grp->locus_tag);
      CleanDoubleQuoteList (grp->syn);
      grp->db = ValNodeSort (grp->db, SortDbxref);
      CleanupDuplicateDbxrefs (&(grp->db));
      /* now move grp->dbxref to sfp->dbxref */
      vnp = grp->db;
      grp->db = NULL;
      ValNodeLink ((&sfp->dbxref), vnp);
      if (grp->locus != NULL && grp->syn != NULL) {
        vnp = grp->syn;
        str = (CharPtr) vnp->data.ptrvalue;
        if (StringCmp (grp->locus, str) == 0) {
          grp->syn = vnp->next;
          vnp->next = NULL;
          ValNodeFreeData (vnp);
        }
      }
      if (grp->locus != NULL && sfp->comment != NULL) {
        if (StringCmp (grp->locus, sfp->comment) == 0) {
          sfp->comment = MemFree (sfp->comment);
        }
      }
      break;
    case SEQFEAT_ORG :
      orp = (OrgRefPtr) sfp->data.value.ptrvalue;
      break;
    case SEQFEAT_CDREGION :
      if (sfp->xref != NULL && sfp->product != NULL) {
        HandleXrefOnCDS (sfp);
      }
      break;
    case SEQFEAT_PROT :
      prp = (ProtRefPtr) sfp->data.value.ptrvalue;
      CleanVisString (&(prp->desc));
      CleanVisStringList (&(prp->name));
      CleanVisStringList (&(prp->ec));
      CleanVisStringList (&(prp->activity));
      CleanDoubleQuote (prp->desc);
      CleanDoubleQuoteList (prp->name);
      CleanDoubleQuoteList (prp->ec);
      CleanDoubleQuoteList (prp->activity);
      prp->db = ValNodeSort (prp->db, SortDbxref);
      CleanupDuplicateDbxrefs (&(prp->db));
      /* now move prp->dbxref to sfp->dbxref */
      vnp = prp->db;
      prp->db = NULL;
      ValNodeLink ((&sfp->dbxref), vnp);
      if (prp->processed != 3 && prp->processed != 4 &&
          prp->name == NULL && sfp->comment != NULL) {
        if (StringICmp (sfp->comment, "putative") != 0) {
          ValNodeAddStr (&(prp->name), 0, sfp->comment);
          sfp->comment = NULL;
        }
      }
      if (prp->processed == 3 || prp->processed == 4) {
        if (prp->name != NULL) {
          str = (CharPtr) prp->name->data.ptrvalue;
          if ((StringStr (str, "putative") != NULL ||
               StringStr (str, "put. ") != NULL) &&
              sfp->comment == NULL) {
            sfp->comment = StringSave ("putative");
          }
          if (! InformativeString (str)) {
            prp->name = ValNodeFreeData (prp->name);
          }
        }
      }
      if ((prp->processed == 1 || prp->processed == 2) && prp->name == NULL) {
        ValNodeCopyStr (&(prp->name), 0, "unnamed");
      }
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp->ext.choice == 1) {
        CleanVisString ((CharPtr PNTR) &(rrp->ext.value.ptrvalue));
        CleanDoubleQuote ((CharPtr) rrp->ext.value.ptrvalue);
        if (rrp->ext.value.ptrvalue == NULL) {
          rrp->ext.choice = 0;
        } else if (rrp->type == 4) {
          name = (CharPtr) rrp->ext.value.ptrvalue;
          len = StringLen (name);
          if (len > 5) {
            if (len > 14 && StringNICmp (name + len - 14, " ribosomal rRNA", 14) == 0) {
            } else if (StringNICmp (name + len - 5, " rRNA", 5) == 0) {
              str = MemNew (len + 10);
              if (str != NULL) {
                StringNCpy (str, name, len - 5);
                StringCat (str, " ribosomal RNA");
                rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
                rrp->ext.value.ptrvalue = (Pointer) str;
              }
            }
          }
        } else if (rrp->type == 3) {
          name = (CharPtr) rrp->ext.value.ptrvalue;
          aa = ParseTRnaString (name, &justTrnaText, codon, FALSE);
          if (aa != 0) {
            is_fMet = (Boolean) (StringStr (name, "fMet") != NULL);
            rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
            trp = (tRNAPtr) MemNew (sizeof (tRNA));
            if (trp != NULL) {
              trp->aatype = 2;
              for (j = 0; j < 6; j++) {
                trp->codon [j] = 255;
              }
              if (justTrnaText) {
                for (j = 0; j < 6; j++) {
                  trp->codon [j] = codon [j];
                }
              }
              trp->aa = aa;
              rrp->ext.choice = 2;
              rrp->ext.value.ptrvalue = (Pointer) trp;
              CleanupTrna (sfp, trp);
            }
            if (is_fMet) {
              if (sfp->comment == NULL) {
                sfp->comment = StringSave ("fMet");
              } else {
                len = StringLen (sfp->comment) + StringLen ("fMet") + 5;
                str = MemNew (sizeof (Char) * len);
                StringCpy (str, sfp->comment);
                StringCat (str, "; ");
                StringCat (str, "fMet");
                sfp->comment = MemFree (sfp->comment);
                sfp->comment = str;
              }
            }
          }
        }
      } else if (rrp->ext.choice == 2) {
        trp = (tRNAPtr) rrp->ext.value.ptrvalue;
        CleanupTrna (sfp, trp);
      } else if (rrp->type == 3 && (! StringHasNoText (sfp->comment))) {
        aa = ParseTRnaString (sfp->comment, &justTrnaText, codon, TRUE);
        if (aa != 0) {
          trp = (tRNAPtr) MemNew (sizeof (tRNA));
          if (trp != NULL) {
            trp->aatype = 2;
            for (j = 0; j < 6; j++) {
              trp->codon [j] = 255;
            }
            if (justTrnaText) {
              for (j = 0; j < 6; j++) {
                trp->codon [j] = codon [j];
              }
            }
            trp->aa = aa;
            rrp->ext.choice = 2;
            rrp->ext.value.ptrvalue = (Pointer) trp;
            if (justTrnaText) {
              if (StringCmp (sfp->comment, "fMet") != 0 &&
                  StringCmp (sfp->comment, "fMet tRNA") != 0 &&
                  StringCmp (sfp->comment, "fMet-tRNA") != 0) {
                sfp->comment = MemFree (sfp->comment);
              } else {
                sfp->comment = MemFree (sfp->comment);
                sfp->comment = StringSave ("fMet");
              }
            }
          }
        }
      }
      if (rrp->ext.choice == 0 && sfp->comment != NULL && rrp->type == 4) {
        len = StringLen (sfp->comment);
        if (len > 15 && len < 20) {
          if (StringNICmp (sfp->comment + len - 15, "S ribosomal RNA", 15) == 0) {
            rrp->ext.choice = 1;
            rrp->ext.value.ptrvalue = sfp->comment;
            sfp->comment = NULL;
          }
        } else if (len > 6 && len < 20) {
          if (StringNICmp (sfp->comment + len - 6, "S rRNA", 6) == 0) {
            rrp->ext.choice = 1;
            rrp->ext.value.ptrvalue = sfp->comment;
            sfp->comment = NULL;
          }
        }
      }
      if (rrp->ext.choice == 1 && rrp->ext.value.ptrvalue != NULL) {
        if (StringICmp ((CharPtr) rrp->ext.value.ptrvalue, sfp->comment) == 0) {
          sfp->comment = MemFree (sfp->comment);
        }
      }
      if (rrp->type == 4) {
        name = (CharPtr) rrp->ext.value.ptrvalue;
        len = StringLen (name);
        if (len > 5) {
          suff = NULL;
          str = StringStr (name, " ribosomal");
          if (str != NULL) {
            suff = str + 10;
            ch = *suff;
            if (ch != '\0' && ch != ' ') {
              suff = NULL;
              str = NULL;
            }
          }
          if (str == NULL) {
            str = StringStr (name, " rRNA");
            if (str != NULL) {
              suff = str + 5;
              ch = *suff;
              if (ch != '\0' && ch != ' ') {
                suff = NULL;
                str = NULL;
              }
            }
          }
          if (suff != NULL && StringNICmp (suff, " RNA", 4) == 0) {
            suff += 4;
          }
          if (suff != NULL && StringNICmp (suff, " DNA", 4) == 0) {
            suff += 4;
          }
          if (suff != NULL && StringNICmp (suff, " ribosomal", 10) == 0) {
            suff += 10;
          }
          TrimSpacesAroundString (suff);
          if (str != NULL) {
            *str = '\0';
            len = StringLen (name);
            if (StringHasNoText (suff)) {
              suff = NULL;
            }
            if (suff != NULL) {
              len += StringLen (suff) + 2;
            }
            str = MemNew (len + 15);
            if (str != NULL) {
              StringCpy (str, name);
              StringCat (str, " ribosomal RNA");
              if (suff != NULL) {
                ch = *suff;
                if (ch != ',' && ch != ';') {
                  StringCat (str, " ");
                }
                StringCat (str, suff);
              }
              rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
              rrp->ext.value.ptrvalue = (Pointer) str;
            }
          }
        }
        name = (CharPtr) rrp->ext.value.ptrvalue;
        len = StringLen (name);
        if (len > 5) {
          ch = *name;
          while (ch != '\0' && (ch == '.' || (IS_DIGIT (ch)))) {
            name++;
            ch = *name;
          }
          if (ch == 's' && StringCmp (name, "s ribosomal RNA") == 0) {
            *name = 'S';
          }
        }
        StrStripSpaces ((CharPtr) rrp->ext.value.ptrvalue);
        name = (CharPtr) rrp->ext.value.ptrvalue;
        len = StringLen (name);
        if (len > 17) {
          if (StringNICmp (name + len - 17, "ribosomal RNA RNA", 17) == 0) {
            *(name + len - 4) = '\0';
          }
        }
        trimming_junk = TRUE;
        while (trimming_junk) {
          StrStripSpaces ((CharPtr) rrp->ext.value.ptrvalue);
          name = (CharPtr) rrp->ext.value.ptrvalue;
          ptr = StringStr (name, "ribosomal ribosomal");
          if (ptr != NULL) {
            suff = ptr + 19;
            *(ptr + 10) = '\0';
            temp = MemNew (StringLen (name) + StringLen (suff) + 2);
            TrimSpacesAroundString (suff);
            StringCpy (temp, name);
            if (suff [0] != ' ' && suff [0] != '\0') {
              StringCat (temp, " ");
            }
            StringCat (temp, suff);
            rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
            rrp->ext.value.ptrvalue = (Pointer) temp;
          } else {
            ptr = StringStr (name, "RNA RNA");
            if (ptr != NULL) {
              suff = ptr + 7;
              *(ptr + 4) = '\0';
              temp = MemNew (StringLen (name) + StringLen (suff) + 2);
              TrimSpacesAroundString (suff);
              StringCpy (temp, name);
              if (suff [0] != ' ' && suff [0] != '\0') {
                StringCat (temp, " ");
              }
              StringCat (temp, suff);
              rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
              rrp->ext.value.ptrvalue = (Pointer) temp;
            } else {
              ptr = StringStr (name, "ribosomal RNA ribosomal");
              if (ptr != NULL) {
                suff = ptr + 23;
                *(ptr + 14) = '\0';
                temp = MemNew (StringLen (name) + StringLen (suff) + 2);
                TrimSpacesAroundString (suff);
                StringCpy (temp, name);
                if (suff [0] != ' ' && suff [0] != '\0') {
                  StringCat (temp, " ");
                }
                StringCat (temp, suff);
                rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
                rrp->ext.value.ptrvalue = (Pointer) temp;
              } else {
                ptr = StringStr (name, "ribosomal rRNA");
                if (ptr != NULL) {
                  suff = ptr + 14;
                  *(ptr + 10) = '\0';
                  temp = MemNew (StringLen (name) + StringLen (" RNA") + StringLen (suff) + 2);
                  TrimSpacesAroundString (suff);
                  StringCpy (temp, name);
                  StringCat (temp, " RNA");
                  if (suff [0] != ' ' && suff [0] != '\0') {
                    StringCat (temp, " ");
                  }
                  StringCat (temp, suff);
                  rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
                  rrp->ext.value.ptrvalue = (Pointer) temp;
                } else {
                  ptr = StringStr (name, "RNA rRNA");
                  if (ptr != NULL) {
                    suff = ptr + 8;
                    *(ptr + 3) = '\0';
                    temp = MemNew (StringLen (name) + StringLen (suff) + 2);
                    TrimSpacesAroundString (suff);
                    StringCpy (temp, name);
                    if (suff [0] != ' ' && suff [0] != '\0') {
                      StringCat (temp, " ");
                    }
                    StringCat (temp, suff);
                    rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
                    rrp->ext.value.ptrvalue = (Pointer) temp;
                  } else {
                    trimming_junk = FALSE;
                  }
                }
              }
            }
          }
        }
        TrimSpacesAroundString ((CharPtr) rrp->ext.value.ptrvalue);
      }
      /*
      if (rrp->type == 2 && rrp->ext.choice == 0 && sfp->comment != NULL) {
        rrp->ext.choice = 1;
        rrp->ext.value.ptrvalue = sfp->comment;
        sfp->comment = NULL;
      }
      */
      if (rrp->type == 2 && rrp->ext.choice == 0 && sfp->comment != NULL) {
        len = StringLen (sfp->comment);
        if (len > 5) {
          if (StringNICmp (sfp->comment + len - 4, " RNA", 4) == 0 ||
              StringNICmp (sfp->comment + len - 5, " mRNA", 5) == 0) {
            rrp->ext.choice = 1;
            rrp->ext.value.ptrvalue = sfp->comment;
            sfp->comment = NULL;
          }
        }
      }
      if (rrp->type == 255 && rrp->ext.choice == 1) {
        name = (CharPtr) rrp->ext.value.ptrvalue;
        if (StringICmp (name, "its1") == 0) {
          rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
          rrp->ext.value.ptrvalue = StringSave ("internal transcribed spacer 1");
        } else if (StringICmp (name, "its2") == 0) {
          rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
          rrp->ext.value.ptrvalue = StringSave ("internal transcribed spacer 2");
        } else if (StringICmp (name, "its3") == 0) {
          rrp->ext.value.ptrvalue = MemFree (rrp->ext.value.ptrvalue);
          rrp->ext.value.ptrvalue = StringSave ("internal transcribed spacer 3");
        }
      }
      break;
    case SEQFEAT_PUB :
      pdp = (PubdescPtr) sfp->data.value.ptrvalue;
      CleanDoubleQuote (pdp->comment);
      NormalizePubdesc (pdp, stripSerial, publist);
      break;
    case SEQFEAT_SEQ :
      break;
    case SEQFEAT_IMP :
      ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
      CleanVisString (&(ifp->key));
      CleanVisString (&(ifp->loc));
      CleanVisString (&(ifp->descr));
      break;
    case SEQFEAT_REGION :
      CleanVisString ((CharPtr PNTR) &(sfp->data.value.ptrvalue));
      CleanDoubleQuote ((CharPtr) sfp->data.value.ptrvalue);
      if (sfp->data.value.ptrvalue == NULL) {
        sfp->data.choice = SEQFEAT_COMMENT;
      }
      break;
    case SEQFEAT_COMMENT :
      break;
    case SEQFEAT_BOND :
      break;
    case SEQFEAT_SITE :
      break;
    case SEQFEAT_RSITE :
      break;
    case SEQFEAT_USER :
      VisitUserObjectsInUop ((UserObjectPtr) sfp->data.value.ptrvalue, NULL, CleanUserObject);
      break;
    case SEQFEAT_TXINIT :
      break;
    case SEQFEAT_NUM :
      break;
    case SEQFEAT_PSEC_STR :
      break;
    case SEQFEAT_NON_STD_RESIDUE :
      break;
    case SEQFEAT_HET :
      break;
    case SEQFEAT_BIOSRC :
      biop = (BioSourcePtr) sfp->data.value.ptrvalue;
      orp = biop->org;
      if (orp != NULL) {
        CleanVisStringList (&(orp->mod));
        OrpModToSubSource (&(orp->mod), &(biop->subtype));
      }
      biop->subtype = SortSubSourceList (biop->subtype);
      CleanSubSourceList (&(biop->subtype));
      break;
    default :
      break;
  }
  if (orp != NULL) {
    CleanVisString (&(orp->taxname));
    CleanVisString (&(orp->common));
    CleanVisStringList (&(orp->mod));
    CleanVisStringList (&(orp->syn));
    orp->db = ValNodeSort (orp->db, SortDbxref);
    CleanupDuplicateDbxrefs (&(orp->db));
    onp = orp->orgname;
    while (onp != NULL) {
      CleanVisString (&(onp->attrib));
      CleanVisString (&(onp->lineage));
      CleanVisString (&(onp->div));
      OrpModToOrgMod (&(orp->mod), &(onp->mod));
      onp->mod = SortOrgModList (onp->mod);
      CleanOrgModList (&(onp->mod));
      onp = onp->next;
    }
  }
}

static void CleanupDescriptorStrings (ValNodePtr sdp, Boolean stripSerial, ValNodePtr PNTR publist, Boolean isEmblOrDdbj)

{
  BioSourcePtr  biop;
  EMBLBlockPtr  ebp;
  GBBlockPtr    gbp;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  PubdescPtr    pdp;

  if (sdp == NULL) return;
  switch (sdp->choice) {
    case Seq_descr_mol_type :
    case Seq_descr_method :
      return;
    default :
      break;
  }
  if (sdp->data.ptrvalue == NULL) return;
  orp = NULL;
  switch (sdp->choice) {
    case Seq_descr_mol_type :
      break;
    case Seq_descr_modif :
      break;
    case Seq_descr_method :
      break;
    case Seq_descr_name :
      CleanVisString ((CharPtr PNTR) &sdp->data.ptrvalue);
      break;
    case Seq_descr_title :
      CleanVisString ((CharPtr PNTR) &sdp->data.ptrvalue);
      break;
    case Seq_descr_org :
      orp = (OrgRefPtr) sdp->data.ptrvalue;
      break;
    case Seq_descr_comment :
      CleanVisStringJunk ((CharPtr PNTR) &sdp->data.ptrvalue);
      RemoveSpacesBetweenTildes ((CharPtr) sdp->data.ptrvalue);
      break;
    case Seq_descr_num :
      break;
    case Seq_descr_maploc :
      break;
    case Seq_descr_pir :
      break;
    case Seq_descr_genbank :
      gbp = (GBBlockPtr) sdp->data.ptrvalue;
      CleanVisStringList (&(gbp->extra_accessions));
      if (isEmblOrDdbj) {
        CleanVisStringListCaseSensitive (&(gbp->keywords));
      } else {
        CleanVisStringList (&(gbp->keywords));
      }
      CleanVisStringJunk (&(gbp->source));
      if (StringCmp (gbp->source, ".") == 0) {
        gbp->source = MemFree (gbp->source);
      }
      CleanVisStringJunk (&(gbp->origin));
      if (StringCmp (gbp->origin, ".") == 0) {
        gbp->origin = MemFree (gbp->origin);
      }
      CleanVisString (&(gbp->date));
      CleanVisString (&(gbp->div));
      CleanVisString (&(gbp->taxonomy));
      break;
    case Seq_descr_pub :
      pdp = (PubdescPtr) sdp->data.ptrvalue;
      NormalizePubdesc (pdp, stripSerial, publist);
      break;
    case Seq_descr_region :
      CleanVisString ((CharPtr PNTR) &sdp->data.ptrvalue);
      break;
    case Seq_descr_user :
      VisitUserObjectsInUop ((UserObjectPtr) sdp->data.ptrvalue, NULL, CleanUserObject);
      break;
    case Seq_descr_sp :
      break;
    case Seq_descr_dbxref :
      break;
    case Seq_descr_embl :
      ebp = (EMBLBlockPtr) sdp->data.ptrvalue;
      CleanVisStringList (&(ebp->extra_acc));
      CleanVisStringListCaseSensitive (&(ebp->keywords));
      break;
    case Seq_descr_create_date :
      break;
    case Seq_descr_update_date :
      break;
    case Seq_descr_prf :
      break;
    case Seq_descr_pdb :
      break;
    case Seq_descr_het :
      break;
    case Seq_descr_source :
      biop = (BioSourcePtr) sdp->data.ptrvalue;
      orp = biop->org;
      if (orp != NULL) {
        CleanVisStringList (&(orp->mod));
        OrpModToSubSource (&(orp->mod), &(biop->subtype));
      }
      biop->subtype = SortSubSourceList (biop->subtype);
      CleanSubSourceList (&(biop->subtype));
      break;
    case Seq_descr_molinfo :
      break;
    default :
      break;
  }
  if (orp != NULL) {
    CleanVisString (&(orp->taxname));
    CleanVisString (&(orp->common));
    CleanVisStringList (&(orp->mod));
    CleanVisStringList (&(orp->syn));
    orp->db = ValNodeSort (orp->db, SortDbxref);
    CleanupDuplicateDbxrefs (&(orp->db));
    onp = orp->orgname;
    while (onp != NULL) {
      CleanVisString (&(onp->attrib));
      CleanVisString (&(onp->lineage));
      CleanVisString (&(onp->div));
      OrpModToOrgMod (&(orp->mod), &(onp->mod));
      onp->mod = SortOrgModList (onp->mod);
      CleanOrgModList (&(onp->mod));
      onp = onp->next;
    }
  }
}

static Int2 CheckForQual (GBQualPtr gbqual, CharPtr string_q, CharPtr string_v)

{
  GBQualPtr curq;

  for (curq = gbqual; curq; curq = curq->next) {
    if (StringCmp (string_q, curq->qual) == 0) {
      if (curq->val == NULL) {
        curq->val = StringSave (string_v);
        return 1;
      } 
      if (StringCmp (string_v, curq->val) == 0) return 1;
    }
  }
  return 0;
}
static GBQualPtr AddGBQual (GBQualPtr gbqual, CharPtr qual, CharPtr val)

{
  GBQualPtr curq;

  if (StringCmp (qual, "translation") == 0) {
    if (val == NULL)  return gbqual;
    if (*val == '\0') return gbqual;
  }
  if (gbqual) {
    if (CheckForQual (gbqual, qual, val) == 1) return gbqual;
    for (curq = gbqual; curq->next != NULL; curq = curq->next) continue;
    curq->next = GBQualNew ();
    curq = curq->next;
    if (val)
      curq->val = StringSave (val);
    curq->qual = StringSave (qual);
  } else {
    gbqual = GBQualNew ();
    gbqual->next = NULL;
    if (val)
      gbqual->val = StringSave (val);
    gbqual->qual = StringSave (qual);
  }
  return gbqual;
}

static void AddReplaceQual (SeqFeatPtr sfp, CharPtr p)

{
  CharPtr s, val;

  val = StringChr (p, '\"');
  if (val == NULL) return;
  val++;
  s = p + StringLen (p) - 1;
  if (*s != ')') return;
  for (s--; s > val && *s != '\"'; s--) continue;
  if (*s != '\"') return;
  *s = '\0';
  sfp->qual = (GBQualPtr) AddGBQual (sfp->qual, "replace", val);
  *s = '\"';
}

NLM_EXTERN Boolean SerialNumberInString (CharPtr str)

{
  Char     ch;
  Boolean  hasdigits;
  CharPtr  ptr;
  Boolean  suspicious = FALSE;

  if (str == NULL || StringHasNoText (str)) return FALSE;
  ptr = StringChr (str, '[');
  while ((! suspicious) && ptr != NULL) {
    hasdigits = FALSE;
    ptr++;
    ch = *ptr;
    while (IS_DIGIT (ch)) {
      hasdigits = TRUE;
      ptr++;
      ch = *ptr;
    }
    if (ch == ']' && hasdigits) {
      suspicious = TRUE;
    }
    if (! suspicious) {
      ptr = StringChr (ptr, '[');
    }
  }
  return suspicious;
}

/* now only strips serials for local, general, refseq, and 2+6 genbank ids */
static void CheckForSwissProtID (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  SeqIdPtr      sip;
  BoolPtr       stripSerial;
  TextSeqIdPtr  tsip;

  if (sep == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    stripSerial = (BoolPtr) mydata;
    if (stripSerial == NULL) return;
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      switch (sip->choice) {
        case SEQID_GIBBSQ :
        case SEQID_GIBBMT :
          *stripSerial = FALSE;
          break;
        case SEQID_EMBL :
        case SEQID_PIR :
        case SEQID_SWISSPROT :
        case SEQID_PATENT :
        case SEQID_DDBJ :
        case SEQID_PRF :
        case SEQID_PDB :
        case SEQID_TPE:
        case SEQID_TPD:
          *stripSerial = FALSE;
          break;
        case SEQID_GENBANK :
        case SEQID_TPG:
          tsip = (TextSeqIdPtr) sip->data.ptrvalue;
          if (tsip != NULL) {
            if (StringLen (tsip->accession) == 6) {
              *stripSerial = FALSE;
            }
          }
          break;
        case SEQID_NOT_SET :
        case SEQID_LOCAL :
        case SEQID_OTHER :
        case SEQID_GENERAL :
          break;
        default :
          break;
      }
    }
  }
}

static void CheckForEmblDdbjID (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr  bsp;
  BoolPtr    isEmblOrDdbj;
  SeqIdPtr   sip;

  if (sep == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    isEmblOrDdbj = (BoolPtr) mydata;
    if (isEmblOrDdbj == NULL) return;
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      switch (sip->choice) {
        case SEQID_EMBL :
        case SEQID_DDBJ :
          *isEmblOrDdbj = TRUE;
          break;
          break;
        default :
          break;
      }
    }
  }
}

static void CleanupSeqLoc (SeqLocPtr slp)

{
  SeqLocPtr  last;
  SeqLocPtr  loc;
  SeqIntPtr  sintp;
  Int4       swp;

  if (slp == NULL) return;

  /* from < to for all intervals */
  loc = SeqLocFindNext (slp, NULL);
  while (loc != NULL) {
    if (loc->choice == SEQLOC_INT) {
      sintp = (SeqIntPtr) loc->data.ptrvalue;
      if (sintp != NULL) {
        if (sintp->from > sintp->to) {
          swp = sintp->from;
          sintp->from = sintp->to;
          sintp->to = swp;
        }
      }
    }
    loc = SeqLocFindNext (slp, loc);
  }

  if (slp->choice == SEQLOC_PACKED_INT) {
    loc = (SeqLocPtr) slp->data.ptrvalue;
    if (loc == NULL || loc->next != NULL) return;
    /* here seqloc_packed_int points to a single location element, so no need for seqloc_packed_int parent */
    slp->choice = loc->choice;
    slp->data.ptrvalue = (Pointer) loc->data.ptrvalue;
    MemFree (loc);
    return;
  }

  if (slp->choice != SEQLOC_MIX) return;
  loc = (SeqLocPtr) slp->data.ptrvalue;
  if (loc == NULL) return;

  if (loc->next != NULL) {
    /* check for null NULL at beginning */
    if (loc->choice == SEQLOC_NULL) {
      slp->data.ptrvalue = (Pointer) loc->next;
      loc->next = NULL;
      ValNodeFree (loc);
    }
    /* check for null NULL at end */
    loc = (SeqLocPtr) slp->data.ptrvalue;
    last = NULL;
    while (loc->next != NULL) {
      last = loc;
      loc = loc->next;
    }
    if (loc->choice == SEQLOC_NULL && last != NULL) {
      last->next = NULL;
      ValNodeFree (loc);
    }
  }

  loc = (SeqLocPtr) slp->data.ptrvalue;
  if (loc == NULL || loc->next != NULL) return;
  /* here seqloc_mix points to a single location element, so no need for seqloc_mix parent */
  slp->choice = loc->choice;
  slp->data.ptrvalue = (Pointer) loc->data.ptrvalue;
  MemFree (loc);
}

static void BasicSeqEntryCleanupInternal (SeqEntryPtr sep, ValNodePtr PNTR publist)

{
  BioSourcePtr  biop;
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  CodeBreakPtr  cbp;
  CdRegionPtr   crp;
  Char          div [10];
  GBBlockPtr    gbp;
  Boolean       hasNulls;
  SeqIdPtr      id;
  ImpFeatPtr    ifp;
  Boolean       isEmblOrDdbj = FALSE;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  Boolean       partial5;
  Boolean       partial3;
  Uint1         processed;
  ProtRefPtr    prp;
  ValNodePtr    psp;
  RnaRefPtr     rrp;
  Uint1         rrptype;
  SeqAnnotPtr   sap = NULL;
  ValNodePtr    sdp = NULL;
  SeqFeatPtr    sfp;
  SeqIntPtr     sintp;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  CharPtr       str;
  Uint1         strand;
  Boolean       stripSerial = TRUE;
  SeqEntryPtr   tmp;
  tRNAPtr       trp;

  if (sep == NULL) return;
  SeqEntryExplore (sep, (Pointer) &stripSerial, CheckForSwissProtID);
  SeqEntryExplore (sep, (Pointer) &isEmblOrDdbj, CheckForEmblDdbjID);
#ifdef SUPPRESS_TRIVIAL_FLATFILE_DIFFERENCES
  stripSerial = FALSE;
#endif
#ifdef SUPPRESS_STRIP_SERIAL_DIFFERENCES
  stripSerial = FALSE;
#endif
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    sap = bsp->annot;
    sdp = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
      BasicSeqEntryCleanupInternal (tmp, publist);
    }
    sap = bssp->annot;
    sdp = bssp->descr;
  } else return;
  biop = NULL;
  orp = NULL;
  gbp = NULL;
  div [0] = '\0';
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        crp = NULL;
        if (sfp->data.choice == SEQFEAT_IMP) {
          ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
          if (ifp != NULL) {
            if (ifp->loc != NULL) {
              str = StringStr (ifp->loc, "replace");
              if (str != NULL) {
                AddReplaceQual (sfp, str);
                ifp->loc = MemFree (ifp->loc);
              }
            }
            if (StringCmp (ifp->key, "CDS") == 0) {
              if (! isEmblOrDdbj) {
                sfp->data.value.ptrvalue = ImpFeatFree (ifp);
                sfp->data.choice = SEQFEAT_CDREGION;
                crp = CdRegionNew ();
                sfp->data.value.ptrvalue = crp;
                sfp->idx.subtype = FEATDEF_CDS;
              }
            } else if (StringCmp (ifp->key, "allele") == 0 ||
                       StringCmp (ifp->key, "mutation") == 0) {
              ifp->key = MemFree (ifp->key);
              ifp->key = StringSave ("variation");
              sfp->idx.subtype = FEATDEF_variation;
            } else if (StringHasNoText (ifp->loc)) {
              rrptype = 0;
              if (StringCmp (ifp->key, "precursor_RNA") == 0) {
                rrptype = 1;
              } else if (StringCmp (ifp->key, "mRNA") == 0) {
                rrptype = 2;
              } else if (StringCmp (ifp->key, "tRNA") == 0) {
                rrptype = 3;
              } else if (StringCmp (ifp->key, "rRNA") == 0) {
                rrptype = 4;
              } else if (StringCmp (ifp->key, "snRNA") == 0) {
                rrptype = 5;
              } else if (StringCmp (ifp->key, "scRNA") == 0) {
                rrptype = 6;
              } else if (StringCmp (ifp->key, "snoRNA") == 0) {
                rrptype = 7;
              } else if (StringCmp (ifp->key, "misc_RNA") == 0) {
                rrptype = 255;
              }
              if (rrptype != 0) {
                sfp->data.value.ptrvalue = ImpFeatFree (ifp);
                sfp->data.choice = SEQFEAT_RNA;
                rrp = RnaRefNew ();
                sfp->data.value.ptrvalue = rrp;
                rrp->type = rrptype;
                sfp->idx.subtype = FindFeatDefType (sfp);
              } else {
                processed = 0;
                if (StringCmp (ifp->key, "proprotein") == 0 || StringCmp (ifp->key, "preprotein") == 0) {
                  processed = 1;
                } else if (StringCmp (ifp->key, "mat_peptide") == 0) {
                  processed = 2;
                } else if (StringCmp (ifp->key, "sig_peptide") == 0) {
                  processed = 3;
                } else if (StringCmp (ifp->key, "transit_peptide") == 0) {
                  processed = 4;
                }
                if (processed != 0 || StringCmp (ifp->key, "Protein") == 0) {
                  bsp = BioseqFind (SeqLocId (sfp->location));
                  if (bsp != NULL && ISA_aa (bsp->mol)) {
                    sfp->data.value.ptrvalue = ImpFeatFree (ifp);
                    sfp->data.choice = SEQFEAT_PROT;
                    prp = ProtRefNew ();
                    sfp->data.value.ptrvalue = prp;
                    prp->processed = processed;
                    sfp->idx.subtype = FindFeatDefType (sfp);
                  }
                }
              }
            }
          }
        }
        if (crp != NULL && crp->frame == 0 && (! sfp->pseudo)) {
          crp->frame = GetFrameFromLoc (sfp->location);
        }
        sfp->qual = SortFeatureGBQuals (sfp->qual);
        CleanupDuplicateGBQuals (&(sfp->qual));
        CleanupFeatureGBQuals (sfp, isEmblOrDdbj);
        sfp->qual = SortIllegalGBQuals (sfp->qual);
        CleanupFeatureStrings (sfp, stripSerial, publist);
        sfp->dbxref = ValNodeSort (sfp->dbxref, SortDbxref);
        CleanupDuplicateDbxrefs (&(sfp->dbxref));
        psp = sfp->cit;
        if (psp != NULL && psp->data.ptrvalue) {
          psp->data.ptrvalue = ValNodeSort ((ValNodePtr) psp->data.ptrvalue, SortCits);
          CleanupDuplicateCits ((ValNodePtr PNTR) &(psp->data.ptrvalue));
        }
        CleanupSeqLoc (sfp->location);
        strand = SeqLocStrand (sfp->location);
        id = SeqLocId (sfp->location);
        if (sfp->data.choice == SEQFEAT_CDREGION) {
          crp = (CdRegionPtr) sfp->data.value.ptrvalue;
          if (crp != NULL) {
            for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
              CleanupSeqLoc (cbp->loc);
              if (strand == Seq_strand_minus && id != NULL) {
                slp = cbp->loc;
                if (slp != NULL && slp->choice == SEQLOC_INT) {
                  sip = SeqLocId (slp);
                  if (sip != NULL && SeqIdComp (id, sip) == SIC_YES) {
                    sintp = (SeqIntPtr) slp->data.ptrvalue;
                    if (sintp != NULL) {
                      sintp->strand = Seq_strand_minus;
                    }
                  }
                }
              }
            }
          }
        } else if (sfp->data.choice == SEQFEAT_RNA) {
          rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
          if (rrp != NULL && rrp->ext.choice == 2) {
            trp = (tRNAPtr) rrp->ext.value.ptrvalue;
            if (trp != NULL && trp->anticodon != NULL) {
              CleanupSeqLoc (trp->anticodon);
              if (strand == Seq_strand_minus && id != NULL) {
                slp = trp->anticodon;
                if (slp != NULL && slp->choice == SEQLOC_INT) {
                  sip = SeqLocId (slp);
                  if (sip != NULL && SeqIdComp (id, sip) == SIC_YES) {
                    sintp = (SeqIntPtr) slp->data.ptrvalue;
                    if (sintp != NULL) {
                      sintp->strand = Seq_strand_minus;
                    }
                  }
                }
              }
            }
          }
        }
        CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
        hasNulls = LocationHasNullsBetween (sfp->location);
        sfp->partial = (sfp->partial || partial5 || partial3 || hasNulls);
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
  while (sdp != NULL) {
    switch (sdp->choice) {
      case Seq_descr_org :
        orp = (OrgRefPtr) sdp->data.ptrvalue;
        break;
      case Seq_descr_genbank :
        gbp = (GBBlockPtr) sdp->data.ptrvalue;
        break;
      case Seq_descr_source :
        biop = (BioSourcePtr) sdp->data.ptrvalue;
        if (biop != NULL) {
          orp = biop->org;
        }
        break;
      default :
        break;
    }
    CleanupDescriptorStrings (sdp, stripSerial, publist, isEmblOrDdbj);
    sdp = sdp->next;
  }

  /* copy genbank block division into biosource, if necessary */

  if (orp != NULL && gbp != NULL) {
    StringNCpy_0 (div, gbp->div, sizeof (div));
    if (StringHasNoText (div)) return;
    onp = orp->orgname;
    while (onp != NULL) {
      if (StringHasNoText (onp->div)) {
        onp->div = MemFree (onp->div);
        onp->div = StringSaveNoNull (div);
      }
      onp = onp->next;
    }
  }
}

static void ReplaceCitOnFeat (CitGenPtr cgp, ValNodePtr publist)

{
  ValNodePtr  nxt;
  ValNodePtr  vnp;

  for (vnp = publist; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice != 1) continue;
    if (StringCmp (cgp->cit, (CharPtr) vnp->data.ptrvalue) == 0) {
      nxt = vnp->next;
      if (nxt != NULL && nxt->choice == 2) {
        cgp->cit = MemFree (cgp->cit);
        cgp->cit = StringSaveNoNull ((CharPtr) nxt->data.ptrvalue);
        if (cgp->cit != NULL) {
          if (StringNICmp (cgp->cit, "unpublished", 11) == 0) {
            cgp->cit [0] = 'U';
          }
        }
      }
      return;
    }
  }
}

static void ChangeCitsOnFeats (SeqFeatPtr sfp, Pointer userdata)

{
  CitGenPtr   cgp;
  ValNodePtr  ppr;
  ValNodePtr  psp;
  ValNodePtr  vnp;

  psp = sfp->cit;
  if (psp != NULL && psp->data.ptrvalue) {
    for (ppr = (ValNodePtr) psp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
      vnp = NULL;
      if (ppr->choice == PUB_Gen) {
        vnp = ppr;
      } else if (ppr->choice == PUB_Equiv) {
        for (vnp = (ValNodePtr) ppr->data.ptrvalue;
             vnp != NULL && vnp->choice != PUB_Gen;
             vnp = vnp->next) continue;
      }
      if (vnp != NULL && vnp->choice == PUB_Gen) {
        cgp = (CitGenPtr) vnp->data.ptrvalue;
        if (cgp != NULL && (! StringHasNoText (cgp->cit))) {
          ReplaceCitOnFeat (cgp, (ValNodePtr) userdata);
        }
      }
    }
  }
}

static Int4 GetPmidForMuid (ValNodePtr pairlist, Int4 muid)

{
  ValNodePtr  vnp;

  vnp = pairlist;
  while (vnp != NULL) {
    if (muid == vnp->data.intvalue) {
      vnp = vnp->next;
      if (vnp == NULL) return 0;
      return vnp->data.intvalue;
    } else {
      vnp = vnp->next;
      if (vnp == NULL) return 0;
      vnp = vnp->next;
    }
  }

  return 0;
}

static void ChangeFeatCitsToPmid (SeqFeatPtr sfp, Pointer userdata)

{
  Int4        muid = 0;
  Int4        pmid = 0;
  ValNodePtr  ppr;
  ValNodePtr  psp;
  ValNodePtr  vnp;

  psp = sfp->cit;
  if (psp != NULL && psp->data.ptrvalue) {
    for (ppr = (ValNodePtr) psp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
      vnp = NULL;
      if (ppr->choice == PUB_Muid) {
        vnp = ppr;
      } else if (ppr->choice == PUB_Equiv) {
        for (vnp = (ValNodePtr) ppr->data.ptrvalue;
             vnp != NULL && vnp->choice != PUB_Muid;
             vnp = vnp->next) continue;
      }
      if (vnp != NULL && vnp->choice == PUB_Muid) {
        muid = vnp->data.intvalue;
        if (muid != 0) {
          pmid = GetPmidForMuid ((ValNodePtr) userdata, muid);
          if (pmid != 0) {
            vnp->choice = PUB_PMid;
            vnp->data.intvalue = pmid;
          }
        }
      }
    }
  }
}

static void GetMuidPmidPairs (PubdescPtr pdp, Pointer userdata)

{
  Int4        muid = 0;
  Int4        pmid = 0;
  ValNodePtr  vnp;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
      case PUB_Muid :
        muid = vnp->data.intvalue;
        break;
      case PUB_PMid :
        pmid = vnp->data.intvalue;
        break;
      default :
        break;
    }
  }
  if (muid == 0 || pmid == 0) return;
  ValNodeAddInt ((ValNodePtr PNTR) userdata, 0, muid);
  ValNodeAddInt ((ValNodePtr PNTR) userdata, 0, pmid);
}

static void FlattenPubSet (ValNodePtr PNTR prev)

{
  ValNodePtr  next;
  ValNodePtr  ppr;
  ValNodePtr  vnp;

  if (prev == NULL || *prev == NULL) return;
  ppr = *prev;
  while (ppr != NULL) {
    next = ppr->next;

    if (ppr->choice == PUB_Equiv) {
      vnp = (ValNodePtr) ppr->data.ptrvalue;
      if (vnp != NULL && vnp->next == NULL) {
        ppr->choice = vnp->choice;
        switch (vnp->choice) {
          case PUB_Muid :
          case PUB_PMid :
            ppr->data.intvalue = vnp->data.intvalue;
            break;
          default :
            ppr->data.ptrvalue = vnp->data.ptrvalue;
            break;
        }
        ValNodeFree (vnp);
      }
    }

    ppr = next;
  }
} 

static void FlattenDupInPubSet (ValNodePtr PNTR prev)

{
  ValNodePtr  next;
  ValNodePtr  nxt;
  ValNodePtr  ppr;
  ValNodePtr  vnp;

  if (prev == NULL || *prev == NULL) return;
  ppr = *prev;
  while (ppr != NULL) {
    next = ppr->next;

    if (ppr->choice == PUB_Equiv) {
      vnp = (ValNodePtr) ppr->data.ptrvalue;
      if (vnp != NULL) {
        nxt = vnp->next;
        if (nxt != NULL && nxt->next == NULL && vnp->choice == nxt->choice) {
          switch (vnp->choice) {
            case PUB_Muid :
            case PUB_PMid :
              if (vnp->data.intvalue == nxt->data.intvalue) {
                vnp->next = ValNodeFree (nxt);
              }
              break;
            default :
              break;
          }
        }
      }
    }

    ppr = next;
  }
} 

static void FlattenPubdesc (PubdescPtr pdp, Pointer userdata)

{
  FlattenPubSet (&(pdp->pub));
}

static void FlattenSfpCit (SeqFeatPtr sfp, Pointer userdata)

{
  ValNodePtr  psp;

  psp = sfp->cit;
  if (psp == NULL) return;
  FlattenDupInPubSet ((ValNodePtr PNTR) &(psp->data.ptrvalue));
  FlattenPubSet ((ValNodePtr PNTR) &(psp->data.ptrvalue));
}

static void GetCitGenLabels (PubdescPtr pdp, Pointer userdata)

{
  Char             buf [121];
  CitGenPtr        cgp;
  ValNodePtr PNTR  labellist;
  ValNodePtr       vnp;
 
  if (pdp == NULL) return;
  labellist = (ValNodePtr PNTR) userdata;
  if (labellist == NULL) return;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice != PUB_Gen) continue;
    cgp = (CitGenPtr) vnp->data.ptrvalue;
    if (cgp == NULL) continue;
    if (cgp->cit == NULL && cgp->journal == NULL &&
        cgp->date == NULL && cgp->serial_number) continue;
    PubLabelUnique (vnp, buf, sizeof (buf) - 1, OM_LABEL_CONTENT, TRUE);
    ValNodeCopyStr (labellist, 0, buf);
  }
}

static void ReplaceShortCitGenOnFeat (CitGenPtr cgp, ValNodePtr labellist)

{
  Char        buf [128];
  Char        ch;
  size_t      len1;
  size_t      len2;
  CharPtr     ptr;
  CharPtr     str;
  CharPtr     tmp;
  ValNodePtr  vnp;

  for (vnp = labellist; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringHasNoText (str)) continue;
    len1 = StringLen (cgp->cit);
    if (len1 < 2 || len1 > 120) continue;
    StringCpy (buf, cgp->cit);
    ptr = StringStr (buf, "Unpublished");
    if (ptr != NULL) {
      ptr += 11;
      *ptr = '\0';
      tmp = StringStr (cgp->cit, "Unpublished");
      if (tmp != NULL) {
        tmp += 11;
        ch = *tmp;
        while (ch == ' ') {
          tmp++;
          ch = *tmp;
        }
        StringCat (buf, tmp);
      }
    }
    len1 = StringLen (buf);
    if (buf [len1 - 1] != '>') continue;
    len1--;
    len2 = StringLen (str);
    if (len1 >= len2) continue;
    if (StringNCmp (str, buf, len1) == 0) {
      cgp->cit = MemFree (cgp->cit);
      cgp->cit = StringSaveNoNull (str);
      if (cgp->cit != NULL) {
        if (StringNICmp (cgp->cit, "unpublished", 11) == 0) {
          cgp->cit [0] = 'U';
        }
      }
      return;
    }
  }
}

static void UpdateShortFeatCits (SeqFeatPtr sfp, Pointer userdata)

{
  CitGenPtr   cgp;
  ValNodePtr  ppr;
  ValNodePtr  psp;
  ValNodePtr  vnp;

  psp = sfp->cit;
  if (psp != NULL && psp->data.ptrvalue) {
    for (ppr = (ValNodePtr) psp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
      vnp = NULL;
      if (ppr->choice == PUB_Gen) {
        vnp = ppr;
      } else if (ppr->choice == PUB_Equiv) {
        for (vnp = (ValNodePtr) ppr->data.ptrvalue;
             vnp != NULL && vnp->choice != PUB_Gen;
             vnp = vnp->next) continue;
      }
      if (vnp != NULL && vnp->choice == PUB_Gen) {
        cgp = (CitGenPtr) vnp->data.ptrvalue;
        if (cgp != NULL && (! StringHasNoText (cgp->cit))) {
          ReplaceShortCitGenOnFeat (cgp, (ValNodePtr) userdata);
        }
      }
    }
  }
}

NLM_EXTERN void BasicSeqEntryCleanup (SeqEntryPtr sep)

{
  ValNodePtr   labellist = NULL;
  ValNodePtr   pairlist = NULL;
  ValNodePtr   publist = NULL;
  SeqEntryPtr  oldscope;

  /* HandleXrefOnCDS call to GetBestProteinFeatureUnindexed now scoped within record */

  oldscope = SeqEntrySetScope (sep);

  /* removed unnecessarily nested Pub-equivs */

  VisitPubdescsInSep (sep, NULL, FlattenPubdesc);
  VisitFeaturesInSep (sep, NULL, FlattenSfpCit);

  BasicSeqEntryCleanupInternal (sep, &publist);
  if (publist != NULL) {
    VisitFeaturesInSep (sep, (Pointer) publist, ChangeCitsOnFeats);
  }
  ValNodeFreeData (publist);

  /* now get muid/pmid pairs, update sfp->cits to pmids */

  VisitPubdescsInSep (sep, (Pointer) &pairlist, GetMuidPmidPairs);
  if (pairlist != NULL) {
    VisitFeaturesInSep (sep, (Pointer) pairlist, ChangeFeatCitsToPmid);
  }
  ValNodeFree (pairlist);

  VisitPubdescsInSep (sep, (Pointer) &labellist, GetCitGenLabels);
  if (labellist != NULL) {
    VisitFeaturesInSep (sep, (Pointer) labellist, UpdateShortFeatCits);
  }
  ValNodeFreeData (labellist);

  SeqEntrySetScope (oldscope);
}

/* end BasicSeqEntryCleanup section */

NLM_EXTERN void ResynchCDSPartials (SeqFeatPtr sfp, Pointer userdata)

{
  SeqFeatPtr   bestprot;
  BioseqPtr    bsp;
  MolInfoPtr   mip;
  Boolean      partial5;
  Boolean      partial3;
  SeqEntryPtr  sep;
  SeqIdPtr     sip;
  SeqLocPtr    slp;
  ValNodePtr   vnp;

  if (sfp->data.choice != SEQFEAT_CDREGION) return;
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  sfp->partial = (Boolean) (sfp->partial || partial5 || partial3);
  slp = SeqLocFindNext (sfp->location, NULL);
  if (slp == NULL) return;
  sip = SeqLocId (sfp->product);
  if (sip == NULL) return;
  bsp = BioseqFind (sip);
  if (bsp != NULL && ISA_aa (bsp->mol) && bsp->repr == Seq_repr_raw) {
    bestprot = SeqMgrGetBestProteinFeature (bsp, NULL);
    if (bestprot == NULL) {
      bestprot = GetBestProteinFeatureUnindexed (sfp->product);
    }
    if (bestprot != NULL) {
      sep = SeqMgrGetSeqEntryForData (bsp);
      if (sep == NULL) return;
      bestprot->location = SeqLocFree (bestprot->location);
      bestprot->location = CreateWholeInterval (sep);
      SetSeqLocPartial (bestprot->location, partial5, partial3);
      bestprot->partial = (partial5 || partial3);
      vnp = SeqEntryGetSeqDescr (sep, Seq_descr_molinfo, NULL);
      if (vnp == NULL) {
        vnp = CreateNewDescriptor (sep, Seq_descr_molinfo);
        if (vnp != NULL) {
          mip = MolInfoNew ();
          vnp->data.ptrvalue = (Pointer) mip;
          if (mip != NULL) {
            mip->biomol = 8; /* peptide */
            mip->tech = 13; /* concept-trans-author */
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
}

NLM_EXTERN void ResynchCodingRegionPartials (SeqEntryPtr sep)

{
  VisitFeaturesInSep (sep, NULL, ResynchCDSPartials);
}

NLM_EXTERN void ResynchMRNAPartials (SeqFeatPtr sfp, Pointer userdata)

{
  BioseqPtr    bsp;
  MolInfoPtr   mip;
  Boolean      partial5;
  Boolean      partial3;
  RnaRefPtr    rrp;
  SeqEntryPtr  sep;
  SeqIdPtr     sip;
  SeqLocPtr    slp;
  ValNodePtr   vnp;

  if (sfp->data.choice != SEQFEAT_RNA) return;
  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
  if (rrp == NULL || rrp->type != 2) return;
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  sfp->partial = (Boolean) (sfp->partial || partial5 || partial3);
  slp = SeqLocFindNext (sfp->location, NULL);
  if (slp == NULL) return;
  sip = SeqLocId (sfp->product);
  if (sip == NULL) return;
  bsp = BioseqFind (sip);
  if (bsp != NULL && ISA_na (bsp->mol) && bsp->repr == Seq_repr_raw) {
    sep = SeqMgrGetSeqEntryForData (bsp);
    if (sep == NULL) return;
    vnp = SeqEntryGetSeqDescr (sep, Seq_descr_molinfo, NULL);
    if (vnp == NULL) {
      vnp = CreateNewDescriptor (sep, Seq_descr_molinfo);
      if (vnp != NULL) {
        mip = MolInfoNew ();
        vnp->data.ptrvalue = (Pointer) mip;
        if (mip != NULL) {
          mip->biomol = 3; /* mRNA */
          mip->tech = 1; /* standard */
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

NLM_EXTERN void ResynchMessengerRNAPartials (SeqEntryPtr sep)

{
  VisitFeaturesInSep (sep, NULL, ResynchMRNAPartials);
}

NLM_EXTERN void ResynchPeptidePartials (SeqFeatPtr sfp, Pointer userdata)

{
  SeqFeatPtr   bestprot;
  BioseqPtr    bsp;
  MolInfoPtr   mip;
  Boolean      partial5;
  Boolean      partial3;
  ProtRefPtr   prp;
  SeqEntryPtr  sep;
  SeqIdPtr     sip;
  SeqLocPtr    slp;
  ValNodePtr   vnp;

  if (sfp->data.choice != SEQFEAT_PROT) return;
  prp = (ProtRefPtr) sfp->data.value.ptrvalue;
  if (prp == NULL) return;
  if (prp->processed < 1 || prp->processed > 4) return;
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  sfp->partial = (Boolean) (sfp->partial || partial5 || partial3);
  slp = SeqLocFindNext (sfp->location, NULL);
  if (slp == NULL) return;
  sip = SeqLocId (sfp->product);
  if (sip == NULL) return;
  bsp = BioseqFind (sip);
  if (bsp != NULL && ISA_aa (bsp->mol) && bsp->repr == Seq_repr_raw) {
    sep = SeqMgrGetSeqEntryForData (bsp);
    if (sep == NULL) return;
    bestprot = SeqMgrGetBestProteinFeature (bsp, NULL);
    if (bestprot == NULL) {
      bestprot = GetBestProteinFeatureUnindexed (sfp->product);
    }
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

NLM_EXTERN void ResynchProteinPartials (SeqEntryPtr sep)

{
  VisitFeaturesInSep (sep, NULL, ResynchPeptidePartials);
}

/* SeqIdStripLocus removes the SeqId.name field if accession is set */

NLM_EXTERN SeqIdPtr SeqIdStripLocus (SeqIdPtr sip)

{
  TextSeqIdPtr  tip;

  if (sip != NULL) {
    switch (sip->choice) {
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_DDBJ :
      case SEQID_OTHER :
      case SEQID_TPG:
      case SEQID_TPE:
      case SEQID_TPD:
        tip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tip != NULL) {
          if (! HasNoText (tip->accession)) {
            tip->name = MemFree (tip->name);
          }
        }
        break;
      default :
        break;
    }
  }
  return sip;
}

NLM_EXTERN SeqLocPtr StripLocusFromSeqLoc (SeqLocPtr location)

{
  SeqLocPtr      loc;
  SeqLocPtr      next;
  PackSeqPntPtr  psp;
  SeqBondPtr     sbp;
  SeqIntPtr      sinp;
  SeqIdPtr       sip;
  SeqLocPtr      slp;
  SeqPntPtr      spp;

  if (location == NULL) return NULL;
  slp = SeqLocFindNext (location, NULL);
  while (slp != NULL) {
    next = SeqLocFindNext (location, slp);
    switch (slp->choice) {
      case SEQLOC_EMPTY :
      case SEQLOC_WHOLE :
        sip = (SeqIdPtr) slp->data.ptrvalue;
        SeqIdStripLocus (sip);
        break;
      case SEQLOC_INT :
        sinp = (SeqIntPtr) slp->data.ptrvalue;
        if (sinp != NULL) {
          SeqIdStripLocus (sinp->id);
        }
        break;
      case SEQLOC_PACKED_INT :
      case SEQLOC_MIX :
      case SEQLOC_EQUIV :
        loc = (SeqLocPtr) slp->data.ptrvalue;
        while (loc != NULL) {
          sip = SeqLocId (loc);
          SeqIdStripLocus (sip);
          loc = loc->next;
        }
        break;
      case SEQLOC_BOND :
        sbp = (SeqBondPtr) slp->data.ptrvalue;
        if (sbp != NULL) {
          spp = sbp->a;
          if (spp != NULL) {
            SeqIdStripLocus (spp->id);
          }
          spp = sbp->b;
          if (spp != NULL) {
            SeqIdStripLocus (spp->id);
          }
        }
        break;
      case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          SeqIdStripLocus (spp->id);
        }
        break;
      case SEQLOC_PACKED_PNT :
        psp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (psp != NULL) {
          SeqIdStripLocus (psp->id);
        }
        break;
      default :
        break;
    }
    slp = next;
  }
  return location;
}

static void GetRidOfLocusCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqAnnotPtr   sap;
  SeqFeatPtr    sfp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  sap = NULL;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1 && sap->data != NULL) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        StripLocusFromSeqLoc (sfp->location);
        StripLocusFromSeqLoc (sfp->product);
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

NLM_EXTERN void GetRidOfLocusInSeqIds (Uint2 entityID, SeqEntryPtr sep)

{
  if (entityID < 1 && sep == NULL) return;
  if (entityID > 0 && sep == NULL) {
    sep = GetTopSeqEntryForEntityID (entityID);
  }
  if (sep == NULL) return;
  SeqEntryExplore (sep, NULL, GetRidOfLocusCallback);
}

/* Mac can now use static parse tables by using
   Make Strings Read-Only and Store Static Data in TOC
#ifdef OS_MAC
#define ASNLOAD_NEEDED 1
#endif
*/
#if defined(OS_DOS) || defined(WIN16)
#define ASNLOAD_NEEDED 1
#endif

static Boolean FileExists (CharPtr dirname, CharPtr subname, CharPtr filename)

{
  Char  path [PATH_MAX];

  StringNCpy_0 (path, dirname, sizeof (path));
  FileBuildPath (path, subname, NULL);
  FileBuildPath (path, NULL, filename);
  return (Boolean) (FileLength (path) > 0);
}

/*
static Boolean CheckAsnloadPath (CharPtr dirname, CharPtr subdir)

{
#ifdef ASNLOAD_NEEDED
  Char  fname [16];
  int   i;

  for (i = 60; i <= 99; ++i) {
    sprintf (fname, "asnmedli.l%02d", (int) i);
    if (FileExists (dirname, subdir, fname)) {
      return TRUE;
    }
  }
  return FALSE;
#else
  return TRUE;
#endif
}
*/

static Boolean CheckDataPath (CharPtr dirname, CharPtr subdir)

{
  if (FileExists (dirname, subdir, "seqcode.val")) return TRUE;
  return (Boolean) (FileExists (dirname, subdir, "objprt.prt"));
}

static Boolean CheckErrMsgPath (CharPtr dirname, CharPtr subdir)

{
  return (Boolean) (FileExists (dirname, subdir, "valid.msg"));
}

static void SetTransientPath (CharPtr dirname, CharPtr subname, CharPtr file,
                              CharPtr section, CharPtr type)

{
  Char  path [PATH_MAX];

  StringNCpy_0 (path, dirname, sizeof (path));
  FileBuildPath (path, subname, NULL);
  TransientSetAppParam (file, section, type, path);
}

NLM_EXTERN Boolean UseLocalAsnloadDataAndErrMsg (void)

{
  Boolean  dataFound;
  Char     path [PATH_MAX];
  Char     appPath[PATH_MAX];
  CharPtr  ptr;

  ProgramPath (appPath, sizeof (appPath));
  StrCpy(path, appPath);
  /* data a sibling of our application? */
  ptr = StringRChr (path, DIRDELIMCHR);
  if (ptr != NULL) {
    ptr++;
    *ptr = '\0';
  }
  dataFound = CheckDataPath (path, "data");
  if (! (dataFound)) {
  /* data an uncle of our application? */
    if (ptr != NULL) {
      ptr--;
      *ptr = '\0';
      ptr = StringRChr (path, DIRDELIMCHR);
      if (ptr != NULL) {
        ptr++;
        *ptr = '\0';
      }
      dataFound = CheckDataPath (path, "data");
    }
  }
#ifdef OS_UNIX_DARWIN
  if (! (dataFound) &&  IsApplicationPackage(appPath)) {
      /* is data inside our application within Contents/Resources? */
      StrCpy(path, appPath);
      FileBuildPath(path, "Contents", NULL);
      FileBuildPath (path, "Resources", NULL);
      dataFound = CheckDataPath (path, "data");
  }
#endif
  if (dataFound) {
    SetTransientPath (path, "asnload", "NCBI", "NCBI", "ASNLOAD");
    SetTransientPath (path, "data", "NCBI", "NCBI", "DATA");
    if (CheckErrMsgPath (path, "errmsg")) {
      SetTransientPath (path, "errmsg", "NCBI", "ErrorProcessing", "MsgPath");
      TransientSetAppParam ("NCBI", "ErrorProcessing", "EO_BEEP", "No");
    }
    return TRUE;
  }
  return FALSE;
}

typedef struct miscdata {
  SeqEntryPtr  sep;
  Int2         count;
  Int2         desired;
  Uint1        _class;
} MiscData, PNTR MiscDataPtr;

static void FindNthSeqEntryCallback (SeqEntryPtr sep, Pointer mydata,
                                     Int4 index, Int2 indent)

{
  MiscDataPtr  mdp;

  if (sep != NULL && mydata != NULL) {
    mdp = (MiscDataPtr) mydata;
    (mdp->count)++;
    if (mdp->count == mdp->desired) {
      mdp->sep = sep;
    }
  }
}

NLM_EXTERN SeqEntryPtr LIBCALL FindNthSeqEntry (SeqEntryPtr sep, Int2 seq)

{
  MiscData  md;

  md.sep = NULL;
  md.count = 0;
  md.desired = seq;
  if (sep != NULL) {
    SeqEntryExplore (sep, (Pointer) (&md), FindNthSeqEntryCallback);
  }
  return md.sep;
}

NLM_EXTERN SeqEntryPtr LIBCALL FindNthBioseq (SeqEntryPtr sep, Int2 seq)

{
  MiscData  md;

  md.sep = NULL;
  md.count = 0;
  md.desired = seq;
  if (sep != NULL) {
    BioseqExplore (sep, (Pointer) (&md), FindNthSeqEntryCallback);
  }
  return md.sep;
}

NLM_EXTERN SeqEntryPtr LIBCALL FindNthSequinEntry (SeqEntryPtr sep, Int2 seq)

{
  MiscData  md;

  md.sep = NULL;
  md.count = 0;
  md.desired = seq;
  if (sep != NULL) {
    SequinEntryExplore (sep, (Pointer) (&md), FindNthSeqEntryCallback);
  }
  return md.sep;
}

static void FindNucSeqEntryCallback (SeqEntryPtr sep, Pointer mydata,
                                     Int4 index, Int2 indent)

{
  BioseqPtr    bsp;
  MiscDataPtr  mdp;

  if (sep != NULL && sep->choice == 1 && mydata != NULL) {
    mdp = (MiscDataPtr) mydata;
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp != NULL && ISA_na (bsp->mol)) {
      if (mdp->sep == NULL) {
        mdp->sep = sep;
      }
    }
  }
}

NLM_EXTERN SeqEntryPtr LIBCALL FindNucSeqEntry (SeqEntryPtr sep)

{
  MiscData  md;

  md.sep = NULL;
  md.count = 0;
  md.desired = 0;
  if (sep != NULL) {
    BioseqExplore (sep, (Pointer) (&md), FindNucSeqEntryCallback);
  }
  return md.sep;
}

NLM_EXTERN BioseqPtr LIBCALL FindNucBioseq (SeqEntryPtr sep)

{
  BioseqPtr    nbsp;
  SeqEntryPtr  nsep;

  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL) return NULL;
  if (! IS_Bioseq (nsep)) return NULL;
  nbsp = (BioseqPtr) nsep->data.ptrvalue;
  return nbsp;
}

static void FindBioseqSetByClassCallback (SeqEntryPtr sep, Pointer mydata,
                                          Int4 index, Int2 indent)

{
  BioseqSetPtr  bssp;
  MiscDataPtr   mdp;

  if (sep != NULL && sep->choice == 2 && mydata != NULL) {
    mdp = (MiscDataPtr) mydata;
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && bssp->_class == mdp->_class) {
      if (mdp->sep == NULL) {
        mdp->sep = sep;
      }
    }
  }
}

NLM_EXTERN SeqEntryPtr LIBCALL FindBioseqSetByClass (SeqEntryPtr sep, Uint1 _class)

{
  MiscData  md;

  md.sep = NULL;
  md.count = 0;
  md.desired = 0;
  md._class = _class;
  if (sep != NULL) {
    SeqEntryExplore (sep, (Pointer) (&md), FindBioseqSetByClassCallback);
  }
  return md.sep;
}

typedef struct kinddata {
  Boolean  hasNuc;
  Boolean  hasProt;
} KindData, PNTR KindPtr;

static void HasNucOrProtCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr  bsp;
  KindPtr    kptr;

  if (sep != NULL && sep->choice == 1 && sep->data.ptrvalue != NULL && mydata != NULL) {
    kptr = (KindPtr) mydata;
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (ISA_na (bsp->mol)) {
      kptr->hasNuc = TRUE;
    } else if (ISA_aa (bsp->mol)) {
      kptr->hasProt = TRUE;
    }
  }
}

NLM_EXTERN Boolean LIBCALL SeqEntryHasNucs (SeqEntryPtr sep)

{
  KindData  kd;

  kd.hasNuc = FALSE;
  kd.hasProt = FALSE;
  if (sep != NULL) {
    BioseqExplore (sep, (Pointer) (&kd), HasNucOrProtCallback);
  }
  return kd.hasNuc;
}

NLM_EXTERN Boolean LIBCALL SeqEntryHasProts (SeqEntryPtr sep)

{
  KindData  kd;

  kd.hasNuc = FALSE;
  kd.hasProt = FALSE;
  if (sep != NULL) {
    BioseqExplore (sep, (Pointer) (&kd), HasNucOrProtCallback);
  }
  return kd.hasProt;
}

static Boolean CheckForAlignments (GatherContextPtr gcp)

{
  BoolPtr  boolptr;

  if (gcp == NULL) return TRUE;

  boolptr = (BoolPtr) gcp->userdata;
  if (boolptr == NULL ) return TRUE;

  switch (gcp->thistype) {
    case OBJ_SEQALIGN :
    case OBJ_SEQHIST_ALIGN :
      *boolptr = TRUE;
      return TRUE;
    default :
      break;
  }
  return TRUE;
}

NLM_EXTERN Boolean LIBCALL SeqEntryHasAligns (Uint2 entityID, SeqEntryPtr sep)

{
  GatherScope  gs;
  Boolean      rsult;

  rsult = FALSE;
  if (entityID == 0 || sep == NULL) return FALSE;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  MemSet((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQALIGN] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SEQHIST] = FALSE;
  gs.ignore[OBJ_SEQHIST_ALIGN] = FALSE;
  gs.scope = sep;
  GatherEntity (entityID, (Pointer) (&rsult), CheckForAlignments, &gs);
  return rsult;
}

static void FindPowerBLASTAsnCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  AnnotDescrPtr  desc;
  ObjectIdPtr    oip;
  SeqAnnotPtr    sap;
  BoolPtr        rsult;

  if (sep == NULL || sep->data.ptrvalue == NULL || mydata == NULL) return;
  rsult = (BoolPtr) mydata;
  sap = (IS_Bioseq (sep)) ?
         ((BioseqPtr) sep->data.ptrvalue)->annot :
         ((BioseqSetPtr) sep->data.ptrvalue)->annot;
  while (sap != NULL) {
    if (sap->type == 2) {
      desc = NULL;
      while ((desc = ValNodeFindNext (sap->desc, desc, Annot_descr_user)) != NULL) {
        if (desc->data.ptrvalue != NULL) {
          oip = ((UserObjectPtr) desc->data.ptrvalue)->type;
          if (oip != NULL && StringCmp (oip->str, "Hist Seqalign") == 0) {
            *rsult = TRUE;
          }
        }
      }
    }
    sap = sap->next;
  }
}

NLM_EXTERN Boolean LIBCALL PowerBLASTASN1Detected (SeqEntryPtr sep)

{
  Boolean  rsult;

  rsult = FALSE;
  SeqEntryExplore (sep, (Pointer) &rsult, FindPowerBLASTAsnCallback);
  return rsult;
}

NLM_EXTERN SeqLocPtr CreateWholeInterval (SeqEntryPtr sep)

{
  BioseqPtr  bsp;
  SeqIntPtr  sip;
  SeqLocPtr  slp;

  slp = NULL;
  if (sep != NULL && sep->choice == 1 && sep->data.ptrvalue != NULL) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    slp = ValNodeNew (NULL);
    if (slp != NULL) {
      sip = SeqIntNew ();
      if (sip != NULL) {
        slp->choice = SEQLOC_INT;
        slp->data.ptrvalue = (Pointer) sip;
        sip->from = 0;
        sip->to = bsp->length - 1;
        sip->strand = Seq_strand_plus;
        sip->id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
      }
    }
  }
  return slp;
}

NLM_EXTERN void FreeAllFuzz (SeqLocPtr location)

{
  SeqIntPtr  sip;
  SeqLocPtr  slp;

  if (location == NULL) return;
  slp = SeqLocFindNext (location, NULL);
  while (slp != NULL) {
    if (slp->choice == SEQLOC_INT) {
      sip = (SeqIntPtr) slp->data.ptrvalue;
      if (sip != NULL) {
        sip->if_to = IntFuzzFree (sip->if_to);
        sip->if_from = IntFuzzFree (sip->if_from);
      }
    }
    slp = SeqLocFindNext (location, slp);
  }
}

NLM_EXTERN Boolean LocationHasNullsBetween (SeqLocPtr location)

{
  SeqLocPtr  slp;

  if (location == NULL) return FALSE;
  slp = SeqLocFindNext (location, NULL);
  while (slp != NULL) {
    if (slp->choice == SEQLOC_NULL) return TRUE;
    slp = SeqLocFindNext (location, slp);
  }
  return FALSE;
}

NLM_EXTERN Uint2 FindFeatFromFeatDefType (Uint2 subtype)

{
  switch (subtype) {
    case FEATDEF_GENE :
      return SEQFEAT_GENE;
    case FEATDEF_ORG :
      return SEQFEAT_ORG;
    case FEATDEF_CDS :
      return SEQFEAT_CDREGION;
    case FEATDEF_PROT :
      return SEQFEAT_PROT;
    case FEATDEF_PUB :
      return SEQFEAT_PUB;
    case FEATDEF_SEQ :
      return SEQFEAT_SEQ;
    case FEATDEF_REGION :
      return SEQFEAT_REGION;
    case FEATDEF_COMMENT :
      return SEQFEAT_COMMENT;
    case FEATDEF_BOND :
      return SEQFEAT_BOND;
    case FEATDEF_SITE :
      return SEQFEAT_SITE;
    case FEATDEF_RSITE :
      return SEQFEAT_RSITE;
    case FEATDEF_USER :
      return SEQFEAT_USER;
    case FEATDEF_TXINIT :
      return SEQFEAT_TXINIT;
    case FEATDEF_NUM :
      return SEQFEAT_NUM;
    case FEATDEF_PSEC_STR :
      return SEQFEAT_PSEC_STR;
    case FEATDEF_NON_STD_RESIDUE :
      return SEQFEAT_NON_STD_RESIDUE;
    case FEATDEF_HET :
      return SEQFEAT_HET;
    case FEATDEF_BIOSRC :
      return SEQFEAT_BIOSRC;
    default :
      if (subtype >= FEATDEF_preRNA && subtype <= FEATDEF_otherRNA) {
        return SEQFEAT_RNA;
      }
      if (subtype == FEATDEF_snoRNA) {
        return SEQFEAT_RNA;
      }
      if (subtype >= FEATDEF_IMP && subtype <= FEATDEF_site_ref) {
        return SEQFEAT_IMP;
      }
      if (subtype == FEATDEF_oriT) {
        return SEQFEAT_IMP;
      }
      if (subtype >= FEATDEF_preprotein && subtype <= FEATDEF_transit_peptide_aa) {
        return SEQFEAT_PROT;
      }
      if (subtype == FEATDEF_operon) {
        return SEQFEAT_IMP;
      }
      if (subtype == FEATDEF_gap) {
        return SEQFEAT_IMP;
      }
  }
  return 0;
}

NLM_EXTERN SeqIdPtr MakeSeqID (CharPtr str)

{
  long int     num;
  ObjectIdPtr  oid;
  SeqIdPtr     sip;
  CharPtr      tmp;

  sip = NULL;
  if (str != NULL && *str != '\0') {
    if (StringChr (str, '|') != NULL) {
      sip = SeqIdParse (str);
    } else {
      oid = ObjectIdNew ();
      if (oid != NULL) {
        for (tmp = str; *tmp != '\0'; tmp++) {
          if (! IS_DIGIT (*tmp)) {
            oid->str = StringSave (str);
            break;
          }
        }
        if (oid->str == NULL) {
          sscanf (str, "%ld", &num);
          oid->id = (Int4) num;
        }
        sip = ValNodeNew (NULL);
        if (sip != NULL) {
          sip->choice = SEQID_LOCAL;
          sip->data.ptrvalue = (Pointer) oid;
        } else {
          ObjectIdFree (oid);
        }
      }
    }
  }
  return sip;
}

NLM_EXTERN SeqIdPtr MakeUniqueSeqID (CharPtr prefix)

{
	Char buf[40];
	CharPtr tmp;
	Int2 ctr;
	ValNodePtr newid;
	ObjectIdPtr oid;
	ValNode vn;
	TextSeqId tsi;
	ValNodePtr altid;
	size_t len;

	altid = &vn;
	vn.choice = SEQID_GENBANK;
	vn.next = NULL;
	vn.data.ptrvalue = &tsi;
	tsi.name = NULL;
	tsi.accession = NULL;
	tsi.release = NULL;
	tsi.version = INT2_MIN;

	len = StringLen (prefix);
	if (len > 0 && len < 32) {
		tmp = StringMove(buf, prefix);
	} else {
		tmp = StringMove(buf, "tmpseq_");
	}

	newid = ValNodeNew(NULL);
	oid = ObjectIdNew();
	oid->str = buf;   /* allocate this later */
	newid->choice = SEQID_LOCAL;
	newid->data.ptrvalue = oid;

	tsi.name = buf;   /* check for alternative form */

	for (ctr = 1; ctr < 32000; ctr++)
	{
		sprintf(tmp, "%d", (int)ctr);
		if ((BioseqFindCore(newid) == NULL) && (BioseqFindCore(altid) == NULL))
		{
			oid->str = StringSave(buf);
			return newid;
		}
	}

	return NULL;
}

NLM_EXTERN SeqIdPtr SeqIdFindWorst (SeqIdPtr sip)

{
  Uint1  order [NUM_SEQID];

  SeqIdBestRank (order, NUM_SEQID);
  order [SEQID_LOCAL] = 10;
  order [SEQID_GENBANK] = 5;
  order [SEQID_EMBL] = 5;
  order [SEQID_PIR] = 5;
  order [SEQID_SWISSPROT] = 5;
  order [SEQID_DDBJ] = 5;
  order [SEQID_PRF] = 5;
  order [SEQID_PDB] = 5;
  order [SEQID_TPG] = 5;
  order [SEQID_TPE] = 5;
  order [SEQID_TPD] = 5;
  order [SEQID_PATENT] = 10;
  order [SEQID_OTHER] = 8;
  order [SEQID_GENERAL] = 15;
  order [SEQID_GIBBSQ] = 15;
  order [SEQID_GIBBMT] = 15;
  order [SEQID_GIIM] = 20;
  order [SEQID_GI] = 20;
  return SeqIdSelect (sip, order, NUM_SEQID);
}

NLM_EXTERN SeqFeatPtr CreateNewFeature (SeqEntryPtr sep, SeqEntryPtr placeHere,
                             Uint1 choice, SeqFeatPtr useThis)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqFeatPtr    prev;
  SeqAnnotPtr   sap;
  SeqFeatPtr    sfp;

  if (sep == NULL || sep->choice != 1) return NULL;
  sfp = NULL;
  bsp = NULL;
  bssp = NULL;
  if (placeHere == NULL) {
    placeHere = sep;
  }
  if (placeHere != NULL && placeHere->data.ptrvalue != NULL) {
    if (placeHere->choice == 1) {
      bsp = (BioseqPtr) placeHere->data.ptrvalue;
      sap = bsp->annot;
      while (sap != NULL && (sap->name != NULL || sap->desc != NULL || sap->type != 1)) {
        sap = sap->next;
      }
      if (sap == NULL) {
        sap = SeqAnnotNew ();
        if (sap != NULL) {
          sap->type = 1;
          sap->next = bsp->annot;
          bsp->annot = sap;
        }
        sap = bsp->annot;
      }
    } else if (placeHere->choice == 2) {
      bssp = (BioseqSetPtr) placeHere->data.ptrvalue;
      sap = bssp->annot;
      while (sap != NULL && (sap->name != NULL || sap->desc != NULL || sap->type != 1)) {
        sap = sap->next;
      }
      if (sap == NULL) {
        sap = SeqAnnotNew ();
        if (sap != NULL) {
          sap->type = 1;
          sap->next = bssp->annot;
          bssp->annot = sap;
        }
        sap = bssp->annot;
      }
    } else {
      return NULL;
    }
    if (sap != NULL) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (useThis != NULL) {
        sfp = useThis;
      } else {
        sfp = SeqFeatNew ();
      }
      if (sap->data != NULL) {
        prev = sap->data;
        while (prev->next != NULL) {
          prev = prev->next;
        }
        prev->next = sfp;
      } else {
        sap->data = (Pointer) sfp;
      }
      if (sfp != NULL) {
        sfp->data.choice = choice;
        if (useThis == NULL) {
          sfp->location = CreateWholeInterval (sep);
        }
      }
    }
  }
  return sfp;
}

NLM_EXTERN SeqFeatPtr CreateNewFeatureOnBioseq (BioseqPtr bsp, Uint1 choice, SeqLocPtr slp)

{
  SeqEntryPtr  sep;
  SeqFeatPtr   sfp;

  if (bsp == NULL) return NULL;
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (sep == NULL) return NULL;
  sfp = CreateNewFeature (sep, NULL, choice, NULL);
  if (sfp == NULL) return NULL;
  if (slp != NULL) {
    sfp->location = SeqLocFree (sfp->location);
    sfp->location = AsnIoMemCopy (slp, (AsnReadFunc) SeqLocAsnRead,
                                  (AsnWriteFunc) SeqLocAsnWrite);
  }
  return sfp;
}

NLM_EXTERN ValNodePtr CreateNewDescriptor (SeqEntryPtr sep, Uint1 choice)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Uint1         _class;
  ValNodePtr    descr;
  SeqEntryPtr   seqentry;
  ValNodePtr    vnp;

  vnp = NULL;
  if (sep != NULL) {
    descr = NULL;
    vnp = NULL;
    bsp = NULL;
    bssp = NULL;
    seqentry = sep;
    while (seqentry != NULL) {
      if (seqentry->choice == 1) {
        bsp = (BioseqPtr) seqentry->data.ptrvalue;
        if (bsp != NULL) {
          descr = bsp->descr;
          vnp = SeqDescrNew (descr);
          if (descr == NULL) {
            bsp->descr = vnp;
          }
        }
        seqentry = NULL;
      } else if (seqentry->choice == 2) {
        bssp = (BioseqSetPtr) seqentry->data.ptrvalue;
        if (bssp != NULL) {
          _class = bssp->_class;
          if (_class == 7) {
            descr = bssp->descr;
            vnp = SeqDescrNew (descr);
            if (descr == NULL) {
              bssp->descr = vnp;
            }
            seqentry = NULL;
          } else if ((_class >= 5 && _class <= 8) || _class == 11 /* || _class == BioseqseqSet_class_gen_prod_set */) {
            seqentry = bssp->seq_set;
          } else {
            descr = bssp->descr;
            vnp = SeqDescrNew (descr);
            if (descr == NULL) {
              bssp->descr = vnp;
            }
            seqentry = NULL;
          }
        } else {
          seqentry = NULL;
        }
      } else {
        seqentry = NULL;
      }
    }
    if (vnp != NULL) {
      vnp->choice = choice;
    }
  }
  return vnp;
}

NLM_EXTERN ValNodePtr CreateNewDescriptorOnBioseq (BioseqPtr bsp, Uint1 choice)

{
  SeqEntryPtr  sep;

  if (bsp == NULL) return NULL;
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (sep == NULL) return NULL;
  return CreateNewDescriptor (sep, choice);
}

/* common functions to scan binary ASN.1 file of entire release as Bioseq-set */

static Int4 VisitSeqIdList (SeqIdPtr sip, Pointer userdata, VisitSeqIdFunc callback)

{
  Int4  index = 0;

  while (sip != NULL) {
    if (callback != NULL) {
      callback (sip, userdata);
    }
    index++;
    sip = sip->next;
  }
  return index;
}

NLM_EXTERN Int4 VisitSeqIdsInSeqLoc (SeqLocPtr slp, Pointer userdata, VisitSeqIdFunc callback)

{
  Int4           index = 0;
  SeqLocPtr      loc;
  PackSeqPntPtr  psp;
  SeqBondPtr     sbp;
  SeqIntPtr      sinp;
  SeqIdPtr       sip;
  SeqPntPtr      spp;

  if (slp == NULL) return index;

  while (slp != NULL) {
    switch (slp->choice) {
      case SEQLOC_NULL :
        break;
      case SEQLOC_EMPTY :
      case SEQLOC_WHOLE :
        sip = (SeqIdPtr) slp->data.ptrvalue;
        index += VisitSeqIdList (sip, userdata, callback);
        break;
      case SEQLOC_INT :
        sinp = (SeqIntPtr) slp->data.ptrvalue;
        if (sinp != NULL) {
          sip = sinp->id;
          index += VisitSeqIdList (sip, userdata, callback);
        }
        break;
      case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          sip = spp->id;
          index += VisitSeqIdList (sip, userdata, callback);
        }
        break;
      case SEQLOC_PACKED_PNT :
        psp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (psp != NULL) {
          sip = psp->id;
          index += VisitSeqIdList (sip, userdata, callback);
        }
        break;
      case SEQLOC_PACKED_INT :
      case SEQLOC_MIX :
      case SEQLOC_EQUIV :
        loc = (SeqLocPtr) slp->data.ptrvalue;
        while (loc != NULL) {
          index += VisitSeqIdsInSeqLoc (loc, userdata, callback);
          loc = loc->next;
        }
        break;
      case SEQLOC_BOND :
        sbp = (SeqBondPtr) slp->data.ptrvalue;
        if (sbp != NULL) {
          spp = (SeqPntPtr) sbp->a;
          if (spp != NULL) {
            sip = spp->id;
            index += VisitSeqIdList (sip, userdata, callback);
          }
          spp = (SeqPntPtr) sbp->b;
          if (spp != NULL) {
            sip = spp->id;
            index += VisitSeqIdList (sip, userdata, callback);
          }
        }
        break;
      case SEQLOC_FEAT :
        break;
      default :
        break;
    }
    slp = slp->next;
  }

  return index;
}

NLM_EXTERN Int4 VisitSeqIdsInSeqFeat (SeqFeatPtr sfp, Pointer userdata, VisitSeqIdFunc callback)

{
  CodeBreakPtr  cbp;
  CdRegionPtr   crp;
  Int4          index = 0;
  RnaRefPtr     rrp;
  tRNAPtr       trp;

  if (sfp == NULL) return index;

  index += VisitSeqIdsInSeqLoc (sfp->location, userdata, callback);
  index += VisitSeqIdsInSeqLoc (sfp->product, userdata, callback);

  switch (sfp->data.choice) {
    case SEQFEAT_CDREGION :
      crp = (CdRegionPtr) sfp->data.value.ptrvalue;
      if (crp != NULL) {
        for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
          index += VisitSeqIdsInSeqLoc (cbp->loc, userdata, callback);
        }
      }
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp != NULL && rrp->ext.choice == 2) {
        trp = (tRNAPtr) rrp->ext.value.ptrvalue;
        if (trp != NULL && trp->anticodon != NULL) {
          index += VisitSeqIdsInSeqLoc (trp->anticodon, userdata, callback);
        }
      }
      break;
    default :
      break;
  }

  return index;
}

NLM_EXTERN Int4 VisitSeqIdsInSeqAlign (SeqAlignPtr sap, Pointer userdata, VisitSeqIdFunc callback)

{
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  Int4          index = 0;
  SeqIdPtr      sip;
  SeqLocPtr     slp = NULL;
  StdSegPtr     ssp;

  if (sap == NULL) return index;

  if (sap->bounds != NULL) {
    sip = SeqLocId (sap->bounds);
    index += VisitSeqIdList (sip, userdata, callback);
  }

  if (sap->segs == NULL) return index;

  switch (sap->segtype) {
    case SAS_DENDIAG :
      ddp = (DenseDiagPtr) sap->segs;
      if (ddp != NULL) {
        for (sip = ddp->id; sip != NULL; sip = sip->next) {
          index += VisitSeqIdList (sip, userdata, callback);
        }
      }
      break;
    case SAS_DENSEG :
      dsp = (DenseSegPtr) sap->segs;
      if (dsp != NULL) {
        for (sip = dsp->ids; sip != NULL; sip = sip->next) {
          index += VisitSeqIdList (sip, userdata, callback);
        }
      }
      break;
    case SAS_STD :
      ssp = (StdSegPtr) sap->segs;
      for (slp = ssp->loc; slp != NULL; slp = slp->next) {
        sip = SeqLocId (slp);
        index += VisitSeqIdList (sip, userdata, callback);
      }
      break;
    case SAS_DISC :
      /* recursive */
      for (sap = (SeqAlignPtr) sap->segs; sap != NULL; sap = sap->next) {
        index += VisitSeqIdsInSeqAlign (sap, userdata, callback);
      }
      break;
    default :
      break;
  }

  return index;
}

NLM_EXTERN Int4 VisitSeqIdsInSeqGraph (SeqGraphPtr sgp, Pointer userdata, VisitSeqIdFunc callback)

{
  Int4      index = 0;
  SeqIdPtr  sip;

  if (sgp == NULL) return index;

  if (sgp->loc != NULL) {
    sip = SeqLocId (sgp->loc);
    index += VisitSeqIdList (sip, userdata, callback);
  }

  return index;
}

NLM_EXTERN Int4 VisitSeqIdsInSeqAnnot (SeqAnnotPtr annot, Pointer userdata, VisitSeqIdFunc callback)

{
  Int4         index = 0;
  SeqAlignPtr  sap;
  SeqFeatPtr   sfp;
  SeqGraphPtr  sgp;

  if (annot == NULL || annot->data == NULL) return index;

  switch (annot->type) {

    case 1 :
      for (sfp = (SeqFeatPtr) annot->data; sfp != NULL; sfp = sfp->next) {
        index += VisitSeqIdsInSeqFeat (sfp, userdata, callback);
      }
      break;

    case 2 :
      for (sap = (SeqAlignPtr) annot->data; sap != NULL; sap = sap->next) {
        index += VisitSeqIdsInSeqAlign (sap, userdata, callback);
      }
      break;

    case 3 :
      for (sgp = (SeqGraphPtr) annot->data; sgp != NULL; sgp = sgp->next) {
        index += VisitSeqIdsInSeqGraph (sgp, userdata, callback);
      }
      break;

    default :
      break;
  }

  return index;
}


NLM_EXTERN Int4 VisitUserFieldsInUfp (UserFieldPtr ufp, Pointer userdata, VisitUserFieldsFunc callback)

{
  UserFieldPtr  curr;
  Int4          index = 0;
  Boolean       nested = FALSE;

  if (ufp == NULL) return index;
  if (ufp->choice == 11) {
    for (curr = (UserFieldPtr) ufp->data.ptrvalue; curr != NULL; curr = curr->next) {
      index += VisitUserFieldsInUfp (curr, userdata,callback);
      nested = TRUE;
    }
  }
  if (! nested) {
    if (callback != NULL) {
      callback (ufp, userdata);
    }
    index++;
  }
  return index;
}

NLM_EXTERN Int4 VisitUserFieldsInUop (UserObjectPtr uop, Pointer userdata, VisitUserFieldsFunc callback)

{
  Int4          index = 0;
  UserFieldPtr  ufp;

  if (uop == NULL) return index;
  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    if (callback != NULL) {
      callback (ufp, userdata);
    }
    index++;
  }
  return index;
}

NLM_EXTERN Int4 VisitUserObjectsInUop (UserObjectPtr uop, Pointer userdata, VisitUserObjectFunc callback)

{
  Int4           index = 0;
  Boolean        nested = FALSE;
  UserObjectPtr  obj;
  UserFieldPtr   ufp;

  if (uop == NULL) return index;
  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    if (ufp->choice == 6) {
      obj = (UserObjectPtr) ufp->data.ptrvalue;
      index += VisitUserObjectsInUop (obj, userdata, callback);
      nested = TRUE;
    } else if (ufp->choice == 12) {
      for (obj = (UserObjectPtr) ufp->data.ptrvalue; obj != NULL; obj = obj->next) {
        index += VisitUserObjectsInUop (obj, userdata, callback);
      }
      nested = TRUE;
    }
  }
  if (! nested) {
    if (callback != NULL) {
      callback (uop, userdata);
    }
    index++;
  }
  return index;
}

NLM_EXTERN UserObjectPtr CombineUserObjects (UserObjectPtr origuop, UserObjectPtr newuop)

{
  UserFieldPtr   prev = NULL;
  ObjectIdPtr    oip;
  UserFieldPtr   ufp;
  UserObjectPtr  uop;

  if (newuop == NULL) return origuop;
  if (origuop == NULL) return newuop;

  /* adding to an object that already chaperones at least two user objects */

  oip = origuop->type;
  if (oip != NULL && StringICmp (oip->str, "CombinedFeatureUserObjects") == 0) {

    for (ufp = origuop->data; ufp != NULL; ufp = ufp->next) {
      prev = ufp;
    }

    ufp = UserFieldNew ();
    oip = ObjectIdNew ();
    oip->id = 0;
    ufp->label = oip;
    ufp->choice = 6; /* user object */
    ufp->data.ptrvalue = (Pointer) newuop;

    /* link new set at end of list */

    if (prev != NULL) {
      prev->next = ufp;
    } else {
      origuop->data = ufp;
    }
    return origuop;
  }

  /* creating a new chaperone, link in first two user objects */

  uop = UserObjectNew ();
  oip = ObjectIdNew ();
  oip->str = StringSave ("CombinedFeatureUserObjects");
  uop->type = oip;

  ufp = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->id = 0;
  ufp->label = oip;
  ufp->choice = 6; /* user object */
  ufp->data.ptrvalue = (Pointer) origuop;
  uop->data = ufp;
  prev = ufp;

  ufp = UserFieldNew ();
  oip = ObjectIdNew ();
  oip->id = 0;
  ufp->label = oip;
  ufp->choice = 6; /* user object */
  ufp->data.ptrvalue = (Pointer) newuop;
  prev->next = ufp;

  return uop;
}


static Int4 VisitDescriptorsProc (SeqDescrPtr descr, Pointer userdata, VisitDescriptorsFunc callback)

{
  Int4         index = 0;
  SeqDescrPtr  sdp;

  for (sdp = descr; sdp != NULL; sdp = sdp->next) {
    if (callback != NULL) {
      callback (sdp, userdata);
    }
    index++;
  }
  return index;
}

NLM_EXTERN Int4 VisitDescriptorsOnBsp (BioseqPtr bsp, Pointer userdata, VisitDescriptorsFunc callback)

{
  Int4  index = 0;

  if (bsp == NULL) return index;
  index += VisitDescriptorsProc (bsp->descr, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitDescriptorsOnSet (BioseqSetPtr bssp, Pointer userdata, VisitDescriptorsFunc callback)

{
  Int4  index = 0;

  if (bssp == NULL) return index;
  index += VisitDescriptorsProc (bssp->descr, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitDescriptorsInSet (BioseqSetPtr bssp, Pointer userdata, VisitDescriptorsFunc callback)

{
  Int4         index = 0;
  SeqEntryPtr  tmp;

  if (bssp == NULL) return index;
  index += VisitDescriptorsProc (bssp->descr, userdata, callback);
  for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
    index += VisitDescriptorsInSep (tmp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitDescriptorsOnSep (SeqEntryPtr sep, Pointer userdata, VisitDescriptorsFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitDescriptorsOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitDescriptorsOnSet (bssp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitDescriptorsInSep (SeqEntryPtr sep, Pointer userdata, VisitDescriptorsFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitDescriptorsOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitDescriptorsInSet (bssp, userdata, callback);
  }
  return index;
}


static Int4 VisitFeaturesProc (SeqAnnotPtr annot, Pointer userdata, VisitFeaturesFunc callback)

{
  Int4         index = 0;
  SeqAnnotPtr  sap;
  SeqFeatPtr   sfp;

  for (sap = annot; sap != NULL; sap = sap->next) {
    if (sap->type != 1) continue;
    for (sfp = (SeqFeatPtr) sap->data; sfp != NULL; sfp = sfp->next) {
      if (callback != NULL) {
        callback (sfp, userdata);
      }
      index++;
    }
  }
  return index;
}

NLM_EXTERN Int4 VisitFeaturesOnSap (SeqAnnotPtr sap, Pointer userdata, VisitFeaturesFunc callback)

{
  Int4        index = 0;
  SeqFeatPtr  sfp;

  if (sap == NULL) return index;
  if (sap->type != 1) return index;
  for (sfp = (SeqFeatPtr) sap->data; sfp != NULL; sfp = sfp->next) {
    if (callback != NULL) {
      callback (sfp, userdata);
    }
    index++;
  }
  return index;
}

NLM_EXTERN Int4 VisitFeaturesOnBsp (BioseqPtr bsp, Pointer userdata, VisitFeaturesFunc callback)

{
  Int4  index = 0;

  if (bsp == NULL) return index;
  index += VisitFeaturesProc (bsp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitFeaturesOnSet (BioseqSetPtr bssp, Pointer userdata, VisitFeaturesFunc callback)

{
  Int4  index = 0;

  if (bssp == NULL) return index;
  index += VisitFeaturesProc (bssp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitFeaturesInSet (BioseqSetPtr bssp, Pointer userdata, VisitFeaturesFunc callback)

{
  Int4         index = 0;
  SeqEntryPtr  tmp;

  if (bssp == NULL) return index;
  index += VisitFeaturesProc (bssp->annot, userdata, callback);
  for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
    index += VisitFeaturesInSep (tmp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitFeaturesOnSep (SeqEntryPtr sep, Pointer userdata, VisitFeaturesFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitFeaturesOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitFeaturesOnSet (bssp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitFeaturesInSep (SeqEntryPtr sep, Pointer userdata, VisitFeaturesFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitFeaturesOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitFeaturesInSet (bssp, userdata, callback);
  }
  return index;
}


static Int4 VisitAlignmentsOnDisc (Pointer segs, Pointer userdata, VisitAlignmentsFunc callback)

{
  Int4         index = 0;
  SeqAlignPtr  salp;

  for (salp = (SeqAlignPtr) segs; salp != NULL; salp = salp->next) {
    if (callback != NULL) {
      callback (salp, userdata);
    }
    index++;
    if (salp->segtype == SAS_DISC) {
      index += VisitAlignmentsOnDisc (salp->segs, userdata, callback);
    }
  }
  return index;
}

static Int4 VisitAlignmentsProc (SeqAnnotPtr annot, Pointer userdata, VisitAlignmentsFunc callback)

{
  Int4         index = 0;
  SeqAlignPtr  salp;
  SeqAnnotPtr  sap;

  for (sap = annot; sap != NULL; sap = sap->next) {
    if (sap->type != 2) continue;
    for (salp = (SeqAlignPtr) sap->data; salp != NULL; salp = salp->next) {
      if (callback != NULL) {
        callback (salp, userdata);
      }
      index++;
      if (salp->segtype == SAS_DISC) {
        index += VisitAlignmentsOnDisc (salp->segs, userdata, callback);
      }
    }
  }
  return index;
}

NLM_EXTERN Int4 VisitAlignmentsOnSap (SeqAnnotPtr sap, Pointer userdata, VisitAlignmentsFunc callback)

{
  Int4         index = 0;
  SeqAlignPtr  salp;

  if (sap == NULL) return index;
  if (sap->type != 2) return index;
  for (salp = (SeqAlignPtr) sap->data; salp != NULL; salp = salp->next) {
    if (callback != NULL) {
      callback (salp, userdata);
    }
    index++;
    if (salp->segtype == SAS_DISC) {
      index += VisitAlignmentsOnDisc (salp->segs, userdata, callback);
    }
  }
  return index;
}

NLM_EXTERN Int4 VisitAlignmentsOnBsp (BioseqPtr bsp, Pointer userdata, VisitAlignmentsFunc callback)

{
  Int4  index = 0;

  if (bsp == NULL) return index;
  index += VisitAlignmentsProc (bsp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitAlignmentsOnSet (BioseqSetPtr bssp, Pointer userdata, VisitAlignmentsFunc callback)

{
  Int4  index = 0;

  if (bssp == NULL) return index;
  index += VisitAlignmentsProc (bssp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitAlignmentsInSet (BioseqSetPtr bssp, Pointer userdata, VisitAlignmentsFunc callback)

{
  Int4         index = 0;
  SeqEntryPtr  tmp;

  if (bssp == NULL) return index;
  index += VisitAlignmentsProc (bssp->annot, userdata, callback);
  for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
    index += VisitAlignmentsInSep (tmp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitAlignmentsOnSep (SeqEntryPtr sep, Pointer userdata, VisitAlignmentsFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitAlignmentsOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitAlignmentsOnSet (bssp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitAlignmentsInSep (SeqEntryPtr sep, Pointer userdata, VisitAlignmentsFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitAlignmentsOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitAlignmentsInSet (bssp, userdata, callback);
  }
  return index;
}


static Int4 VisitGraphsProc (SeqAnnotPtr annot, Pointer userdata, VisitGraphsFunc callback)

{
  Int4         index = 0;
  SeqAnnotPtr  sap;
  SeqGraphPtr  sgp;

  for (sap = annot; sap != NULL; sap = sap->next) {
    if (sap->type != 3) continue;
    for (sgp = (SeqGraphPtr) sap->data; sgp != NULL; sgp = sgp->next) {
      if (callback != NULL) {
        callback (sgp, userdata);
      }
      index++;
    }
  }
  return index;
}

NLM_EXTERN Int4 VisitGraphsOnSap (SeqAnnotPtr sap, Pointer userdata, VisitGraphsFunc callback)

{
  Int4         index = 0;
  SeqGraphPtr  sgp;

  if (sap == NULL) return index;
  if (sap->type != 3) return index;
  for (sgp = (SeqGraphPtr) sap->data; sgp != NULL; sgp = sgp->next) {
    if (callback != NULL) {
      callback (sgp, userdata);
    }
    index++;
  }
  return index;
}

NLM_EXTERN Int4 VisitGraphsOnBsp (BioseqPtr bsp, Pointer userdata, VisitGraphsFunc callback)

{
  Int4  index = 0;

  if (bsp == NULL) return index;
  index += VisitGraphsProc (bsp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitGraphsOnSet (BioseqSetPtr bssp, Pointer userdata, VisitGraphsFunc callback)

{
  Int4  index = 0;

  if (bssp == NULL) return index;
  index += VisitGraphsProc (bssp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitGraphsInSet (BioseqSetPtr bssp, Pointer userdata, VisitGraphsFunc callback)

{
  Int4         index = 0;
  SeqEntryPtr  tmp;

  if (bssp == NULL) return index;
  index += VisitGraphsProc (bssp->annot, userdata, callback);
  for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
    index += VisitGraphsInSep (tmp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitGraphsOnSep (SeqEntryPtr sep, Pointer userdata, VisitGraphsFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitGraphsOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitGraphsOnSet (bssp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitGraphsInSep (SeqEntryPtr sep, Pointer userdata, VisitGraphsFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitGraphsOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitGraphsInSet (bssp, userdata, callback);
  }
  return index;
}


static Int4 VisitAnnotsProc (SeqAnnotPtr annot, Pointer userdata, VisitAnnotsFunc callback)

{
  Int4         index = 0;
  SeqAnnotPtr  sap;

  for (sap = annot; sap != NULL; sap = sap->next) {
    if (callback != NULL) {
      callback (sap, userdata);
    }
    index++;
  }
  return index;
}

NLM_EXTERN Int4 VisitAnnotsOnBsp (BioseqPtr bsp, Pointer userdata, VisitAnnotsFunc callback)

{
  Int4  index = 0;

  if (bsp == NULL) return index;
  index += VisitAnnotsProc (bsp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitAnnotsOnSet (BioseqSetPtr bssp, Pointer userdata, VisitAnnotsFunc callback)

{
  Int4  index = 0;

  if (bssp == NULL) return index;
  index += VisitAnnotsProc (bssp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitAnnotsInSet (BioseqSetPtr bssp, Pointer userdata, VisitAnnotsFunc callback)

{
  Int4         index = 0;
  SeqEntryPtr  tmp;

  if (bssp == NULL) return index;
  index += VisitAnnotsProc (bssp->annot, userdata, callback);
  for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
    index += VisitAnnotsInSep (tmp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitAnnotsOnSep (SeqEntryPtr sep, Pointer userdata, VisitAnnotsFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitAnnotsOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitAnnotsOnSet (bssp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitAnnotsInSep (SeqEntryPtr sep, Pointer userdata, VisitAnnotsFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitAnnotsOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitAnnotsInSet (bssp, userdata, callback);
  }
  return index;
}


static Int4 VisitPubdescsProc (SeqDescrPtr descr, SeqAnnotPtr annot, Pointer userdata, VisitPubdescsFunc callback)

{
  Int4         index = 0;
  PubdescPtr   pdp;
  SeqAnnotPtr  sap;
  SeqDescrPtr  sdp;
  SeqFeatPtr   sfp;

  for (sdp = descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice == Seq_descr_pub) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
      if (pdp != NULL) {
        if (callback != NULL) {
          callback (pdp, userdata);
        }
        index++;
      }
    }
  }
  for (sap = annot; sap != NULL; sap = sap->next) {
    if (sap->type != 1) continue;
    for (sfp = (SeqFeatPtr) sap->data; sfp != NULL; sfp = sfp->next) {
      if (sfp->data.choice == SEQFEAT_PUB) {
        pdp = (PubdescPtr) sfp->data.value.ptrvalue;
        if (pdp != NULL) {
          if (callback != NULL) {
            callback (pdp, userdata);
          }
          index++;
        }
      }
    }
  }
  return index;
}

NLM_EXTERN Int4 VisitPubdescsOnBsp (BioseqPtr bsp, Pointer userdata, VisitPubdescsFunc callback)

{
  Int4  index = 0;

  if (bsp == NULL) return index;
  index += VisitPubdescsProc (bsp->descr, bsp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitPubdescsOnSet (BioseqSetPtr bssp, Pointer userdata, VisitPubdescsFunc callback)

{
  Int4  index = 0;

  if (bssp == NULL) return index;
  index += VisitPubdescsProc (bssp->descr, bssp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitPubdescsInSet (BioseqSetPtr bssp, Pointer userdata, VisitPubdescsFunc callback)

{
  Int4         index = 0;
  SeqEntryPtr  tmp;

  if (bssp == NULL) return index;
  index += VisitPubdescsProc (bssp->descr, bssp->annot, userdata, callback);
  for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
    index += VisitPubdescsInSep (tmp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitPubdescsOnSep (SeqEntryPtr sep, Pointer userdata, VisitPubdescsFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitPubdescsOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitPubdescsOnSet (bssp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitPubdescsInSep (SeqEntryPtr sep, Pointer userdata, VisitPubdescsFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitPubdescsOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitPubdescsInSet (bssp, userdata, callback);
  }
  return index;
}


static Int4 VisitBioSourcesProc (SeqDescrPtr descr, SeqAnnotPtr annot, Pointer userdata, VisitBioSourcesFunc callback)

{
  BioSourcePtr  biop;
  Int4          index = 0;
  SeqAnnotPtr   sap;
  SeqDescrPtr   sdp;
  SeqFeatPtr    sfp;

  for (sdp = descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice == Seq_descr_source) {
      biop = (BioSourcePtr) sdp->data.ptrvalue;
      if (biop != NULL) {
        if (callback != NULL) {
          callback (biop, userdata);
        }
        index++;
      }
    }
  }
  for (sap = annot; sap != NULL; sap = sap->next) {
    if (sap->type != 1) continue;
    for (sfp = (SeqFeatPtr) sap->data; sfp != NULL; sfp = sfp->next) {
      if (sfp->data.choice == SEQFEAT_BIOSRC) {
        biop = (BioSourcePtr) sfp->data.value.ptrvalue;
        if (biop != NULL) {
          if (callback != NULL) {
            callback (biop, userdata);
          }
          index++;
        }
      }
    }
  }
  return index;
}

NLM_EXTERN Int4 VisitBioSourcesOnBsp (BioseqPtr bsp, Pointer userdata, VisitBioSourcesFunc callback)

{
  Int4  index = 0;

  if (bsp == NULL) return index;
  index += VisitBioSourcesProc (bsp->descr, bsp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitBioSourcesOnSet (BioseqSetPtr bssp, Pointer userdata, VisitBioSourcesFunc callback)

{
  Int4  index = 0;

  if (bssp == NULL) return index;
  index += VisitBioSourcesProc (bssp->descr, bssp->annot, userdata, callback);
  return index;
}

NLM_EXTERN Int4 VisitBioSourcesInSet (BioseqSetPtr bssp, Pointer userdata, VisitBioSourcesFunc callback)

{
  Int4         index = 0;
  SeqEntryPtr  tmp;

  if (bssp == NULL) return index;
  index += VisitBioSourcesProc (bssp->descr, bssp->annot, userdata, callback);
  for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
    index += VisitBioSourcesInSep (tmp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitBioSourcesOnSep (SeqEntryPtr sep, Pointer userdata, VisitBioSourcesFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitBioSourcesOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitBioSourcesOnSet (bssp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitBioSourcesInSep (SeqEntryPtr sep, Pointer userdata, VisitBioSourcesFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    index += VisitBioSourcesOnBsp (bsp, userdata, callback);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitBioSourcesInSet (bssp, userdata, callback);
  }
  return index;
}


NLM_EXTERN Int4 VisitBioseqsInSet (BioseqSetPtr bssp, Pointer userdata, VisitBioseqsFunc callback)

{
  Int4         index = 0;
  SeqEntryPtr  tmp;

  if (bssp == NULL) return index;
  for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
    index += VisitBioseqsInSep (tmp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitBioseqsInSep (SeqEntryPtr sep, Pointer userdata, VisitBioseqsFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (callback != NULL) {
      callback (bsp, userdata);
    }
    index++;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitBioseqsInSet (bssp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitSequencesInSet (BioseqSetPtr bssp, Pointer userdata, Int2 filter, VisitSequencesFunc callback)

{
  Int4         index = 0;
  SeqEntryPtr  tmp;

  if (bssp == NULL) return index;
  if (bssp->_class == BioseqseqSet_class_parts) {
    if (filter != VISIT_PARTS) return index;
    filter = VISIT_MAINS;
  }
  for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
    index += VisitSequencesInSep (tmp, userdata, filter, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitSequencesInSep (SeqEntryPtr sep, Pointer userdata, Int2 filter, VisitSequencesFunc callback)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (filter == VISIT_MAINS ||
        (filter == VISIT_NUCS && ISA_na (bsp->mol)) ||
        (filter == VISIT_PROTS && ISA_aa (bsp->mol))) {
      if (callback != NULL) {
        callback (bsp, userdata);
      }
      index++;
    }
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitSequencesInSet (bssp, userdata, filter, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitSetsInSet (BioseqSetPtr bssp, Pointer userdata, VisitSetsFunc callback)

{
  Int4         index = 0;
  SeqEntryPtr  tmp;

  if (bssp == NULL) return index;
  if (callback != NULL) {
    callback (bssp, userdata);
  }
  index++;
  for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
    index += VisitSetsInSep (tmp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitSetsInSep (SeqEntryPtr sep, Pointer userdata, VisitSetsFunc callback)

{
  BioseqSetPtr  bssp;
  Int4          index = 0;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    index += VisitSetsInSet (bssp, userdata, callback);
  }
  return index;
}

NLM_EXTERN Int4 VisitElementsInSep (SeqEntryPtr sep, Pointer userdata, VisitElementsFunc callback)

{
  BioseqSetPtr  bssp;
  Int4          index = 0;
  SeqEntryPtr   tmp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return index;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return index;
    if (bssp->_class == 7 ||
        (bssp->_class >= 13 && bssp->_class <= 16) ||
        bssp->_class != BioseqseqSet_class_wgs_set ||
        bssp->_class == BioseqseqSet_class_gen_prod_set) {
      for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
        index += VisitElementsInSep (tmp, userdata, callback);
      }
      return index;
    }
  }
  if (callback != NULL) {
    callback (sep, userdata);
  }
  index++;
  return index;
}

NLM_EXTERN Boolean IsPopPhyEtcSet (Uint1 _class)

{
  if (_class == BioseqseqSet_class_mut_set ||
      _class == BioseqseqSet_class_pop_set ||
      _class == BioseqseqSet_class_phy_set ||
      _class == BioseqseqSet_class_eco_set ||
      _class == BioseqseqSet_class_wgs_set) return TRUE;
  return FALSE;
}


NLM_EXTERN Int4 ScanBioseqSetRelease (CharPtr inputFile, Boolean binary, Boolean compressed, Pointer userdata, ScanBioseqSetFunc callback)

{
  AsnIoPtr      aip;
  AsnModulePtr  amp;
  AsnTypePtr    atp, atp_bss, atp_se;
  FILE          *fp;
  Int4          index = 0;
  SeqEntryPtr   sep;
#ifdef OS_UNIX
  Char          cmmd [256];
  CharPtr       gzcatprog;
  int           ret;
  Boolean       usedPopen = FALSE;
#endif
  if (StringHasNoText (inputFile) || callback == NULL) return index; 

#ifndef OS_UNIX
  if (compressed) {
    Message (MSG_ERROR, "Can only decompress on-the-fly on UNIX machines");
    return index;
  }
#endif

  amp = AsnAllModPtr ();
  if (amp == NULL) {
    Message (MSG_ERROR, "Unable to load AsnAllModPtr");
    return index;
  }

  atp_bss = AsnFind ("Bioseq-set");
  if (atp_bss == NULL) {
    Message (MSG_ERROR, "Unable to find ASN.1 type Bioseq-set");
    return index;
  }

  atp_se = AsnFind ("Bioseq-set.seq-set.E");
  if (atp_se == NULL) {
    Message (MSG_ERROR, "Unable to find ASN.1 type Bioseq-set.seq-set.E");
    return index;
  }

#ifdef OS_UNIX
  if (compressed) {
    gzcatprog = getenv ("NCBI_UNCOMPRESS-BINARY");
    if (gzcatprog != NULL) {
      sprintf (cmmd, "%s %s", gzcatprog, inputFile);
    } else {
      ret = system ("gzcat -h >/dev/null 2>&1");
      if (ret == 0) {
        sprintf (cmmd, "gzcat %s", inputFile);
      } else if (ret == -1) {
        Message (MSG_FATAL, "Unable to fork or exec gzcat in ScanBioseqSetRelease");
        return index;
      } else {
        ret = system ("zcat -h >/dev/null 2>&1");
        if (ret == 0) {
          sprintf (cmmd, "zcat %s", inputFile);
        } else if (ret == -1) {
          Message (MSG_FATAL, "Unable to fork or exec zcat in ScanBioseqSetRelease");
          return index;
        } else {
          Message (MSG_FATAL, "Unable to find zcat or gzcat in ScanBioseqSetRelease - please edit your PATH environment variable");
          return index;
        }
      }
    }
    fp = popen (cmmd, /* binary? "rb" : */ "r");
    usedPopen = TRUE;
  } else {
    fp = FileOpen (inputFile, binary? "rb" : "r");
  }
#else
  fp = FileOpen (inputFile, binary? "rb" : "r");
#endif
  if (fp == NULL) {
    Message (MSG_ERROR, "FileOpen failed for input file '%s'", inputFile);
    return index;
  }

  aip = AsnIoNew (binary? ASNIO_BIN_IN : ASNIO_TEXT_IN, fp, NULL, NULL, NULL);
  if (aip == NULL) {
    Message (MSG_ERROR, "AsnIoNew failed for input file '%s'", inputFile);
    return index;
  }

  atp = atp_bss;

  while ((atp = AsnReadId (aip, amp, atp)) != NULL) {
    if (atp == atp_se) {
      sep = SeqEntryAsnRead (aip, atp);
      callback (sep, userdata);
      SeqEntryFree (sep);
      index++;
    } else {
      AsnReadVal (aip, atp, NULL);
    }
  }

  AsnIoFree (aip, FALSE);

#ifdef OS_UNIX
  if (usedPopen) {
    pclose (fp);
  } else {
    FileClose (fp);
  }
#else
  FileClose (fp);
#endif
  return index;
}

