/*   dlgutil1.c
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
* File Name:  dlgutil1.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.27 $
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
#include <edutil.h>

StdPrintOptionsPtr  spop = NULL;

extern Boolean SetupPrintOptions (void)

{
  if (spop == NULL) {
    spop = StdPrintOptionsNew (NULL);
    if (spop != NULL) {
      spop->newline = "\r";
      spop->indent = "";
    } else {
      Message (MSG_FATAL, "StdPrintOptionsNew failed");
    }
  }
  return (Boolean) (spop != NULL);
}

extern void FreePrintOptions (void)

{
  spop = StdPrintOptionsFree (spop);
}

ENUM_ALIST(months_alist)
  {" ",     0},
  {"Jan",   1},
  {"Feb",   2},
  {"Mar",   3},
  {"Apr",   4},
  {"May",   5},
  {"Jun",   6},
  {"Jul",   7},
  {"Aug",   8},
  {"Sep",   9},
  {"Oct",  10},
  {"Nov",  11},
  {"Dec",  12},
END_ENUM_ALIST

extern Boolean DescFormReplaceWithoutUpdateProc (ForM f)

{
  DescriptorFormPtr  dfp;
  Int4Ptr            intptr;
  OMProcControl      ompc;
  Boolean            rsult;
  ValNodePtr         sdp;

  rsult = FALSE;
  dfp = (DescriptorFormPtr) GetObjectExtra (f);
  if (dfp != NULL) {
    MemSet ((Pointer) &ompc, 0, sizeof (OMProcControl));
    ompc.input_entityID = dfp->input_entityID;
    ompc.input_itemID = dfp->input_itemID;
    ompc.input_itemtype = dfp->input_itemtype;
    ompc.output_itemtype = dfp->input_itemtype;
    sdp = SeqDescrNew (NULL);
    if (sdp != NULL) {
      sdp->choice = dfp->this_subtype;
      switch (sdp->choice) {
        case Seq_descr_mol_type :
        case Seq_descr_method :
          intptr = (Int4Ptr) DialogToPointer (dfp->data);
          if (intptr != NULL) {
            sdp->data.intvalue = *intptr;
          }
          break;
        default :
          sdp->data.ptrvalue = DialogToPointer (dfp->data);
          break;
      }
      ompc.output_data = (Pointer) sdp;
      if (ompc.input_entityID == 0) {
        if (! ObjMgrRegister (OBJ_SEQDESC, (Pointer) sdp)) {
          Message (MSG_ERROR, "ObjMgrRegister failed");
        }
      } else if (ompc.input_itemtype != OBJ_SEQDESC) {
        ompc.output_itemtype = OBJ_SEQDESC;
        if (! AttachDataForProc (&ompc, FALSE)) {
          Message (MSG_ERROR, "AttachDataForProc failed");
        }
        rsult = TRUE;
      } else {
        if (! ReplaceDataForProc (&ompc, FALSE)) {
          Message (MSG_ERROR, "ReplaceDataForProc failed");
        }
        rsult = TRUE;
      }
    }
  }
  return rsult;
}

extern void StdDescFormActnProc (ForM f)

{
  DescriptorFormPtr  dfp;

  if (DescFormReplaceWithoutUpdateProc (f)) {
    dfp = (DescriptorFormPtr) GetObjectExtra (f);
    if (dfp != NULL) {
      GetRidOfEmptyFeatsDescStrings (dfp->input_entityID, NULL);
      if (GetAppProperty ("InternalNcbiSequin") != NULL) {
        ExtendGeneFeatIfOnMRNA (dfp->input_entityID, NULL);
      }
      ObjMgrSetDirtyFlag (dfp->input_entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, dfp->input_entityID,
                     dfp->input_itemID, dfp->input_itemtype);
    }
  }
}

extern void StdDescFormCleanupProc (GraphiC g, VoidPtr data)

{
  DescriptorFormPtr  dfp;
  Uint2              userkey;

  dfp = (DescriptorFormPtr) data;
  if (dfp != NULL) {
    if (dfp->input_entityID > 0 && dfp->userkey > 0) {
      userkey = dfp->userkey;
      dfp->userkey = 0;
      ObjMgrFreeUserData (dfp->input_entityID, dfp->procid, dfp->proctype, userkey);
    }
  }
  StdCleanupExtraProc (g, data);
}

extern OMUserDataPtr ItemAlreadyHasEditor (Uint2 entityID, Uint2 itemID, Uint2 itemtype, Uint2 procid)

{
  BaseFormPtr    bfp;
  Int2           j;
  Int2           num;
  ObjMgrPtr      omp;
  ObjMgrDataPtr  PNTR omdpp;
  OMUserDataPtr  omudp;
  ObjMgrDataPtr  tmp;

  if (entityID == 0 || itemID == 0 || itemtype == 0 || procid == 0) return NULL;
  omp = ObjMgrGet ();
  if (omp == NULL) return NULL;
  num = omp->currobj;
  for (j = 0, omdpp = omp->datalist; j < num && omdpp != NULL; j++, omdpp++) {
    tmp = *omdpp;
    if (tmp->parentptr == NULL && tmp->EntityID == entityID) {

      for (omudp = tmp->userdata; omudp != NULL; omudp = omudp->next) {
        if (omudp->proctype == OMPROC_EDIT && omudp->procid == procid) {
          bfp = (BaseFormPtr) omudp->userdata.ptrvalue;
          if (bfp != NULL) {
            if (bfp->input_itemID == itemID && bfp->input_itemtype == itemtype) {
              return omudp;
            }
          }
        }
      }
    }
  }
  return NULL;
}

typedef struct genegatherlist {
  FeatureFormPtr  ffp;
  ObjMgrPtr       omp;
  SeqLocPtr       slp;
  GeneRefPtr      genexref;
  Boolean         xrefmatch;
  Int2            idx;
  Int2            val;
  Int4            min;
  Uint2           geneEntityID;
  Uint2           geneItemID;
  Uint2           geneItemtype;
  Boolean         geneFound;
} GeneGatherList, PNTR GeneGatherPtr;

static Boolean GeneFindFunc (GatherContextPtr gcp)

{
  GeneGatherPtr  ggp;
  ObjMgrTypePtr  omtp;
  SeqFeatPtr     sfp;
  Char           thislabel [41];

  if (gcp == NULL) return TRUE;

  ggp = (GeneGatherPtr) gcp->userdata;
  if (ggp == NULL ) return TRUE;

  thislabel [0] = '\0';

  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_GENE) {
      omtp = ObjMgrTypeFind (ggp->omp, gcp->thistype, NULL, NULL);
      if (omtp == NULL) {
        return TRUE;
      }
      if (omtp->labelfunc != NULL) {
        (*(omtp->labelfunc)) (gcp->thisitem, thislabel, 40, OM_LABEL_CONTENT);
      }
      if (thislabel [0] != '\0') {
        ggp->idx++;
        if (ggp->idx == ggp->val) {
          ggp->geneEntityID = gcp->entityID;
          ggp->geneItemID = gcp->itemID;
          ggp->geneItemtype = gcp->thistype;
          ggp->geneFound = TRUE;
          return FALSE;
        }
      }
    }
  }
  return TRUE;
}

extern void Nlm_LaunchGeneFeatEd (ButtoN b);
extern void Nlm_LaunchGeneFeatEd (ButtoN b)

{
  FeatureFormPtr  ffp;
  GeneGatherList  ggl;
  GatherScope     gs;
  Int2            handled;
  Int2            val;

  ffp = (FeatureFormPtr) GetObjectExtra (b);
  if (ffp != NULL && ffp->gene != NULL && GetValue (ffp->useGeneXref) == 1) {
    val = GetValue (ffp->gene);
    if (val > 2) {
      ggl.ffp = ffp;
      ggl.omp = ObjMgrGet ();
      ggl.idx = 2;
      ggl.val = val;
      ggl.min = INT4_MAX;
      ggl.geneFound = FALSE;
      ggl.geneEntityID = 0;
      ggl.geneItemID = 0;
      ggl.geneItemtype = 0;
      MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
      gs.seglevels = 1;
      gs.get_feats_location = TRUE;
	  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
	  gs.ignore[OBJ_BIOSEQ] = FALSE;
	  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
	  gs.ignore[OBJ_SEQFEAT] = FALSE;
	  gs.ignore[OBJ_SEQANNOT] = FALSE;
      gs.scope = GetBestTopParentForItemID (ffp->input_entityID,
                                            ffp->input_itemID,
                                            ffp->input_itemtype);
      GatherEntity (ffp->input_entityID, (Pointer) &ggl, GeneFindFunc, &gs);
      if (ggl.geneFound) {
        WatchCursor ();
        Update ();
        handled = GatherProcLaunch (OMPROC_EDIT, FALSE, ggl.geneEntityID, ggl.geneItemID,
                                    ggl.geneItemtype, 0, 0, ggl.geneItemtype, 0);
        ArrowCursor ();
        Update ();
        if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
          Message (MSG_ERROR, "Unable to launch editor on gene feature.");
        }
      }
    }
  }
}

static Boolean GeneUpdateFunc (GatherContextPtr gcp)

{
  BioseqPtr      bsp;
  GeneGatherPtr  ggp;
  Boolean        hasNulls;
  Boolean        noLeft;
  Boolean        noRight;
  Boolean        noLeftFeat;
  Boolean        noLeftGene;
  Boolean        noRightFeat;
  Boolean        noRightGene;
  ObjMgrTypePtr  omtp;
  SeqFeatPtr     sfp;
  SeqLocPtr      slp;
  Char           thislabel [41];

  if (gcp == NULL) return TRUE;

  ggp = (GeneGatherPtr) gcp->userdata;
  if (ggp == NULL ) return TRUE;

  thislabel [0] = '\0';

  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_GENE) {
      omtp = ObjMgrTypeFind (ggp->omp, gcp->thistype, NULL, NULL);
      if (omtp == NULL) {
        return TRUE;
      }
      if (omtp->labelfunc != NULL) {
        (*(omtp->labelfunc)) (gcp->thisitem, thislabel, 40, OM_LABEL_CONTENT);
      }
      if (thislabel [0] != '\0') {
        ggp->idx++;
        if (ggp->idx == ggp->val) {
          if (SeqLocAinB (ggp->slp, sfp->location) <= 0) {
            bsp = GetBioseqGivenSeqLoc (sfp->location, gcp->entityID);
            if (bsp != NULL) {
              hasNulls = LocationHasNullsBetween (sfp->location);
              slp = SeqLocMerge (bsp, sfp->location, ggp->slp, TRUE, FALSE, hasNulls);
              if (slp != NULL) {
                CheckSeqLocForPartial (sfp->location, &noLeftGene, &noRightGene);
                sfp->location = SeqLocFree (sfp->location);
                sfp->location = slp;
                CheckSeqLocForPartial (ggp->slp, &noLeftFeat, &noRightFeat);
                if (bsp->repr == Seq_repr_seg) {
                  slp = SegLocToPartsEx (bsp, sfp->location, TRUE);
                  sfp->location = SeqLocFree (sfp->location);
                  sfp->location = slp;
                  hasNulls = LocationHasNullsBetween (sfp->location);
                  sfp->partial = (sfp->partial || hasNulls);
                }
                FreeAllFuzz (sfp->location);
                noLeft = (noLeftFeat || noLeftGene);
                noRight = (noRightFeat || noRightGene);
                SetSeqLocPartial (sfp->location, noLeft, noRight);
                sfp->partial = (sfp->partial || noLeft || noRight);
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

static Boolean GeneGatherFunc (GatherContextPtr gcp)

{
  FeatureFormPtr  ffp;
  GeneGatherPtr   ggp;
  ObjMgrTypePtr   omtp;
  SeqFeatPtr      sfp;
  Char            thislabel [41];
  ValNodePtr      vnp;

  if (gcp == NULL) return TRUE;

  ggp = (GeneGatherPtr) gcp->userdata;
  if (ggp == NULL || ggp->ffp == NULL) return TRUE;

  thislabel [0] = '\0';

  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_GENE) {
      omtp = ObjMgrTypeFind (ggp->omp, gcp->thistype, NULL, NULL);
      if (omtp == NULL) {
        return TRUE;
      }
      if (omtp->labelfunc != NULL) {
        (*(omtp->labelfunc)) (gcp->thisitem, thislabel, 40, OM_LABEL_CONTENT);
      }
      if (thislabel [0] != '\0') {
        ffp = (FeatureFormPtr) ggp->ffp;
        /*
        PopupItem (ffp->gene, thislabel);
        */
        vnp = ValNodeNew (ffp->geneNames);
        if (ffp->geneNames == NULL) {
          ffp->geneNames = vnp;
        }
        if (vnp != NULL) {
          vnp->data.ptrvalue = StringSave (thislabel);
        }
      }
    }
  }

  return TRUE;
}

extern void PopulateGenePopup (FeatureFormPtr ffp)

{
  Int2            count;
  GeneGatherList  ggl;
  GatherScope     gs;
  CharPtr         str;
  Boolean         usePopupForGene;
  ValNodePtr      vnp;

  if (ffp != NULL && ffp->genePopup != NULL && ffp->geneList != NULL) {
    ggl.ffp = ffp;
    ggl.omp = ObjMgrGet ();
    ggl.slp = NULL;
    ggl.genexref = NULL;
    ggl.xrefmatch = FALSE;
    ggl.idx = 0;
    ggl.val = 0;
    ggl.min = 0;
    MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
    gs.seglevels = 1;
    gs.get_feats_location = TRUE;
	MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
	gs.ignore[OBJ_BIOSEQ] = FALSE;
	gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
	gs.ignore[OBJ_SEQFEAT] = FALSE;
	gs.ignore[OBJ_SEQANNOT] = FALSE;
    gs.scope = GetBestTopParentForItemID (ffp->input_entityID,
                                          ffp->input_itemID,
                                          ffp->input_itemtype);
    GatherEntity (ffp->input_entityID, (Pointer) &ggl, GeneGatherFunc, &gs);
    count = 0;
    for (vnp = ffp->geneNames; vnp != NULL; vnp = vnp->next) {
      count++;
    }
    if (count < 32) {
      usePopupForGene = TRUE;
      ffp->gene = ffp->genePopup;
    } else {
      usePopupForGene = FALSE;
      ffp->gene = ffp->geneList;
    }
    for (vnp = ffp->geneNames; vnp != NULL; vnp = vnp->next) {
      str = (CharPtr) vnp->data.ptrvalue;
      if (StringHasNoText (str)) {
        str = "??";
      }
      if (usePopupForGene) {
        PopupItem (ffp->gene, str);
      } else {
        ListItem (ffp->gene, str);
      }
    }
    Show (ffp->gene);
  }
}

static Boolean GeneMatchFunc (GatherContextPtr gcp)

{
  Int4            diff;
  FeatureFormPtr  ffp;
  GeneRefPtr      genexref;
  GeneGatherPtr   ggp;
  GeneRefPtr      grp;
  ObjMgrTypePtr   omtp;
  SeqFeatPtr      sfp;
  Char            thislabel [41];

  if (gcp == NULL) return TRUE;

  ggp = (GeneGatherPtr) gcp->userdata;
  if (ggp == NULL || ggp->ffp == NULL) return TRUE;

  thislabel [0] = '\0';

  if (gcp->thistype == OBJ_SEQFEAT) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == SEQFEAT_GENE && sfp->data.value.ptrvalue != NULL) {
      omtp = ObjMgrTypeFind (ggp->omp, gcp->thistype, NULL, NULL);
      if (omtp == NULL) {
        return TRUE;
      }
      if (omtp->labelfunc != NULL) {
        (*(omtp->labelfunc)) (gcp->thisitem, thislabel, 40, OM_LABEL_CONTENT);
      }
      if (thislabel [0] != '\0') {
        ffp = (FeatureFormPtr) ggp->ffp;
        ggp->idx++;
        genexref = ggp->genexref;
        if (genexref != NULL) {
          grp = (GeneRefPtr) sfp->data.value.ptrvalue;
          if (StringICmp (genexref->locus, grp->locus) == 0) {
            ggp->val = ggp->idx;
            ggp->xrefmatch = TRUE;
            if (ffp != NULL) {
              SetValue (ffp->useGeneXref, 2);
            }
          }
        }
        diff = SeqLocAinB (ggp->slp, sfp->location);
        if (diff >= 0) {
          if (diff < ggp->min) {
            ggp->min = diff;
            if (! ggp->xrefmatch) {
              ggp->val = ggp->idx;
            }
          }
        }
      }
    }
  }

  return TRUE;
}

extern void SeqFeatPtrToFieldPage (DialoG d, SeqFeatPtr sfp);

extern void SeqFeatPtrToCommon (FeatureFormPtr ffp, SeqFeatPtr sfp)

{
  GeneGatherList  ggl;
  GeneRefPtr      grp;
  GatherScope     gs;
  ProtRefPtr      prp;
  CharPtr         str;
  ValNodePtr      vnp;
  SeqFeatXrefPtr  xref;
  /*
  Char            ch;
  CharPtr         ptr;
  */

  if (ffp != NULL) {
    if (sfp != NULL) {
      str = StringSave (sfp->comment);
      /*
      ptr = str;
      if (ptr != NULL) {
        ch = *ptr;
        while (ch != '\0') {
          if (ch == '~') {
#ifdef WIN_MAC
            *ptr = '\015';
#else
            *ptr = '\n';
#endif
          }
          ptr++;
          ch = *ptr;
        }
      }
      */
      SetTitle (ffp->comment, str);
      SetTitle (ffp->title, sfp->title);
      SetValue (ffp->evidence, sfp->exp_ev + 1);
      SetStatus (ffp->partial, sfp->partial);
      SetStatus (ffp->exception, sfp->excpt);
      SetStatus (ffp->pseudo, sfp->pseudo);
      SetTitle (ffp->exceptText, sfp->except_text);
      SetValue (ffp->useGeneXref, 1);
      SetTitle (ffp->geneSymbol, "");
      SetTitle (ffp->geneDesc, "");
      ggl.ffp = ffp;
      ggl.omp = ObjMgrGet ();
      ggl.slp = sfp->location;
      ggl.genexref = NULL;
      grp = NULL;
      xref = sfp->xref;
      while (xref != NULL && xref->data.choice != SEQFEAT_PROT) {
        xref = xref->next;
      }
      if (xref != NULL) {
        prp = (ProtRefPtr) xref->data.value.ptrvalue;
        if (prp != NULL && ffp->protXrefName != NULL) {
          vnp = prp->name;
          if (vnp != NULL) {
            SetTitle (ffp->protXrefName, (CharPtr) vnp->data.ptrvalue);
          }
        }
      }
      xref = sfp->xref;
      while (xref != NULL && xref->data.choice != SEQFEAT_GENE) {
        xref = xref->next;
      }
      if (xref != NULL) {
        grp = (GeneRefPtr) xref->data.value.ptrvalue;
        ggl.genexref = grp;
      }
      ggl.xrefmatch = FALSE;
      ggl.idx = 2;
      ggl.val = 1;
      ggl.min = INT4_MAX;
      MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
      gs.seglevels = 1;
      gs.get_feats_location = TRUE;
	  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
	  gs.ignore[OBJ_BIOSEQ] = FALSE;
	  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
	  gs.ignore[OBJ_SEQFEAT] = FALSE;
	  gs.ignore[OBJ_SEQANNOT] = FALSE;
      gs.scope = GetBestTopParentForItemID (ffp->input_entityID,
                                            ffp->input_itemID,
                                            ffp->input_itemtype);
      GatherEntity (ffp->input_entityID, (Pointer) &ggl, GeneMatchFunc, &gs);
      if (grp != NULL && StringHasNoText (grp->locus) && StringHasNoText (grp->allele) &&
            StringHasNoText (grp->desc) && StringHasNoText (grp->maploc) &&
            grp->db == NULL && grp->syn == NULL) {
        SetValue (ffp->gene, 1);
        SetValue (ffp->useGeneXref, 3);
        SetTitle (ffp->geneSymbol, grp->locus);
        SetTitle (ffp->geneDesc, grp->desc);
        SafeHide (ffp->editGeneBtn);
        SafeHide (ffp->newGeneGrp);
      } else if (ggl.val == 1 && grp != NULL && (! ggl.xrefmatch)) {
        SetValue (ffp->gene, 2);
        SetValue (ffp->useGeneXref, 2);
        SetTitle (ffp->geneSymbol, grp->locus);
        SetTitle (ffp->geneDesc, grp->desc);
        SafeHide (ffp->editGeneBtn);
        SafeShow (ffp->newGeneGrp);
      } else if (grp != NULL && (! ggl.xrefmatch)) {
        SetValue (ffp->gene, 2);
        SetValue (ffp->useGeneXref, 2);
        SetTitle (ffp->geneSymbol, grp->locus);
        SetTitle (ffp->geneDesc, grp->desc);
        SafeHide (ffp->editGeneBtn);
        SafeShow (ffp->newGeneGrp);
      } else if (ggl.val > 2) {
        SetValue (ffp->gene, ggl.val);
        SafeShow (ffp->editGeneBtn);
        if (ffp->gene == ffp->geneList) {
          SetOffset (ffp->gene, 0, (Int2) (ggl.val - 1));
        }
        if (grp != NULL && (! ggl.xrefmatch)) {
          SetValue (ffp->useGeneXref, 2);
        }
      } else {
        SetValue (ffp->gene, ggl.val);
      }
      PointerToDialog (ffp->featcits, sfp->cit);
      PointerToDialog (ffp->dbxrefs, sfp->dbxref);
      PointerToDialog (ffp->gbquals, sfp->qual);
      SeqFeatPtrToFieldPage (ffp->gbquals, sfp);
      PointerToDialog (ffp->usrobjext, sfp->ext);
    } else {
      SetTitle (ffp->comment, "");
      SetValue (ffp->evidence, 1);
      SetStatus (ffp->partial, FALSE);
      SetStatus (ffp->exception, FALSE);
      SetStatus (ffp->pseudo, FALSE);
      SetTitle (ffp->exceptText, "");
      SetValue (ffp->gene, 1);
      SetValue (ffp->useGeneXref, 1);
      SetTitle (ffp->geneSymbol, "");
      SetTitle (ffp->geneDesc, "");
      PointerToDialog (ffp->featcits, NULL);
      PointerToDialog (ffp->dbxrefs, NULL);
      PointerToDialog (ffp->gbquals, NULL);
      PointerToDialog (ffp->usrobjext, NULL);
    }
  }
}

typedef struct replacesdata {
  FeatureFormPtr  ffp;
  SeqFeatPtr      sfp;
} ReplaceData, PNTR ReplaceDataPtr;

static Boolean ReplaceFeatureExtras (GatherContextPtr gcp)

{
  SeqFeatXrefPtr  curr;
  FeatureFormPtr  ffp;
  SeqFeatXrefPtr  PNTR last;
  SeqFeatXrefPtr  next;
  SeqFeatPtr      old;
  ReplaceDataPtr  rdp;
  SeqFeatPtr      sfp;

  rdp = (ReplaceDataPtr) gcp->userdata;
  if (rdp != NULL && rdp->sfp != NULL && rdp->ffp != NULL) {
    sfp = rdp->sfp;
    ffp = rdp->ffp;
    old = gcp->thisitem;
    if (old != NULL) {
      if (ffp->gbquals != NULL) {
        sfp->qual = DialogToPointer (ffp->gbquals);
      } else if (sfp->qual == NULL) {
        sfp->qual = old->qual;
        old->qual = NULL;
      }
      if (ffp->usrobjext != NULL) {
        sfp->ext = DialogToPointer (ffp->usrobjext);
      } else if (sfp->ext == NULL) {
        sfp->ext = old->ext;
        old->ext = NULL;
      }
      /*
      if (sfp->cit == NULL) {
        sfp->cit = old->cit;
        old->cit = NULL;
      }
      */
      if (old->xref != NULL) {
        last = (SeqFeatXrefPtr PNTR) &(old->xref);
        curr = old->xref;
        while (curr != NULL) {
          next = curr->next;
          if (curr->data.choice == SEQFEAT_GENE) {
            *last = next;
            curr->next = NULL;
            SeqFeatXrefFree (curr);
          } else {
            last = &(curr->next);
          }
          curr = next;
        }
      }
      if (sfp->xref == NULL) {
        sfp->xref = old->xref;
      } else {
        curr = sfp->xref;
        while (curr->next != NULL) {
          curr = curr->next;
        }
        if (curr != NULL) {
          curr->next = old->xref;
        }
      }
      old->xref = NULL;
    }
  }
  return TRUE;
}

static Boolean HasExceptionGBQual (SeqFeatPtr sfp)

{
  GBQualPtr  qual;
  Boolean    rsult;

  rsult = FALSE;
  if (sfp != NULL) {
    qual = sfp->qual;
    while (qual != NULL) {
      if (StringICmp (qual->qual, "exception") == 0) {
        if (! StringHasNoText (qual->val)) {
          rsult = TRUE;
        }
      }
      qual = qual->next;
    }
  }
  return rsult;
}

static void AddProtRefXref (SeqFeatPtr sfp, TexT protXrefName)

{
  ProtRefPtr      prp;
  Char            str [256];
  SeqFeatXrefPtr  xref;

  if (sfp == NULL || protXrefName == NULL) return;
  GetTitle (protXrefName, str, sizeof (str) - 1);
  if (StringHasNoText (str)) return;
  for (xref = sfp->xref; xref != NULL; xref = xref->next) {
    if (xref->data.choice == SEQFEAT_PROT) break;
  }
  if (xref == NULL) {
    xref = SeqFeatXrefNew ();
    if (xref != NULL) {
      prp = ProtRefNew ();
      xref->data.choice = SEQFEAT_PROT;
      xref->data.value.ptrvalue = (Pointer) prp;
      xref->next = sfp->xref;
      sfp->xref = xref;
    }
  }
  if (xref != NULL && xref->data.choice == SEQFEAT_PROT) {
    prp = (ProtRefPtr) xref->data.value.ptrvalue;
    xref->data.value.ptrvalue = ProtRefFree (prp);
    prp = CreateNewProtRef (str, NULL, NULL, NULL);
    xref->data.value.ptrvalue = (Pointer) prp;
  }
}

extern Boolean FeatFormReplaceWithoutUpdateProc (ForM f)

{
  MsgAnswer       ans;
  BioseqPtr       bsp;
  Char            ch;
  Char            desc [128];
  Int2            expev;
  FeatureFormPtr  ffp;
  GeneGatherList  ggl;
  GeneRefPtr      grp;
  GatherScope     gs;
  SeqLocPtr       gslp;
  Boolean         hasNulls;
  Int2            i;
  Int4Ptr         intptr;
  Uint2           itemID;
  Boolean         noLeft;
  Boolean         noRight;
  SeqEntryPtr     oldscope;
  OMProcControl   ompc;
  CharPtr         ptr;
  ReplaceData     rd;
  Boolean         rsult;
  SeqAnnotPtr     sap;
  SeqEntryPtr     sep;
  SeqFeatPtr      sfp;
  SeqLocPtr       slp;
  Char            symbol [128];
  Int2            usexref;
  Int2            val;
  ValNodePtr      vnp;
  SeqFeatXrefPtr  xref;

  rsult = FALSE;
  ffp = (FeatureFormPtr) GetObjectExtra (f);
  if (ffp != NULL) {
    MemSet ((Pointer) &ompc, 0, sizeof (OMProcControl));
    ompc.input_entityID = ffp->input_entityID;
    ompc.input_itemID = ffp->input_itemID;
    ompc.input_itemtype = ffp->input_itemtype;
    ompc.output_itemtype = ffp->input_itemtype;
    sfp = SeqFeatNew ();
    if (sfp != NULL) {
      sep = GetTopSeqEntryForEntityID (ffp->input_entityID);
      oldscope = SeqEntrySetScope (sep);
      sfp->data.choice = FindFeatFromFeatDefType (ffp->this_subtype);
      switch (sfp->data.choice) {
        case SEQFEAT_BOND :
        case SEQFEAT_SITE :
        case SEQFEAT_PSEC_STR :
          intptr = (Int4Ptr) DialogToPointer (ffp->data);
          if (intptr != NULL) {
            sfp->data.value.intvalue = *intptr;
          }
          break;
        case SEQFEAT_COMMENT:
          sfp->data.value.ptrvalue = NULL;
          break;
        default :
          sfp->data.value.ptrvalue = DialogToPointer (ffp->data);
          break;
      }
      sfp->comment = SaveStringFromText (ffp->comment);
      ptr = sfp->comment;
      if (ptr != NULL) {
        ch = *ptr;
        while (ch != '\0') {
          if (ch < ' ' || ch > '~') {
            *ptr = '~';
          }
          ptr++;
          ch = *ptr;
        }
      }
      expev = GetValue (ffp->evidence);
      if (expev > 0 && expev <= 3) {
        sfp->exp_ev = expev - 1;
      } else {
        sfp->exp_ev = 0;
      }
      sfp->partial = GetStatus (ffp->partial);
      sfp->excpt = GetStatus (ffp->exception);
      sfp->pseudo = GetStatus (ffp->pseudo);
      sfp->except_text = SaveStringFromText (ffp->exceptText);
      sfp->title = NULL;
      sfp->product = DialogToPointer (ffp->product);
      sfp->location = DialogToPointer (ffp->location);
      if (sfp->location == NULL) {
        SeqEntrySetScope (oldscope);
        ErrPostEx (SEV_ERROR, 0, 0, "Feature must have a location!");
        return FALSE;
      }
      bsp = GetBioseqGivenSeqLoc (sfp->location, ffp->input_entityID);
      if (bsp != NULL) {
        if (SeqLocBadSortOrder (bsp, sfp->location)) {
          ans = Message (MSG_YN,
            "Feature location intervals are out of order.  Do you want them repaired?");
          if (ans == ANS_YES) {
            hasNulls = LocationHasNullsBetween (sfp->location);
            gslp = SeqLocMerge (bsp, sfp->location, NULL, FALSE, FALSE, hasNulls);
            if (gslp != NULL) {
              CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
              sfp->location = SeqLocFree (sfp->location);
              sfp->location = gslp;
              if (bsp->repr == Seq_repr_seg) {
                gslp = SegLocToParts (bsp, sfp->location);
                sfp->location = SeqLocFree (sfp->location);
                sfp->location = gslp;
              }
              FreeAllFuzz (sfp->location);
              SetSeqLocPartial (sfp->location, noLeft, noRight);
            }
          }
        }
      }
      if (CheckSeqLocForPartial (sfp->location, NULL, NULL)) {
        sfp->partial = TRUE;
      }
      sfp->cit = DialogToPointer (ffp->featcits);
      sfp->dbxref = DialogToPointer (ffp->dbxrefs);
      slp = AsnIoMemCopy (sfp->location, (AsnReadFunc) SeqLocAsnRead,
                          (AsnWriteFunc) SeqLocAsnWrite);
      usexref = GetValue (ffp->useGeneXref);
      if (ffp->gene != NULL) {
        val = GetValue (ffp->gene);
        if (usexref == 3 || (val > 1 && usexref == 2)) {
          grp = NULL;
          if (usexref == 3) {
            grp = GeneRefNew ();
          } else if (val == 2) {
            GetTitle (ffp->geneSymbol, symbol, sizeof (symbol));
            GetTitle (ffp->geneDesc, desc, sizeof (desc));
            grp = CreateNewGeneRef (symbol, NULL, desc, FALSE);
          } else {
            vnp = ffp->geneNames;
            i = val - 3;
            while (i > 0 && vnp != NULL) {
              vnp = vnp->next;
              i--;
            }
            if (vnp != NULL) {
              grp = CreateNewGeneRef ((CharPtr) vnp->data.ptrvalue, NULL, NULL, FALSE);
            }
          }
          if (grp != NULL) {
            xref = SeqFeatXrefNew ();
            sfp->xref = xref;
            if (xref != NULL) {
              xref->data.choice = SEQFEAT_GENE;
              xref->data.value.ptrvalue = (Pointer) grp;
            }
          }
        }
      } else if (usexref == 3) {
        /* protein feature can now suppress gene on GenBank view */
        grp = GeneRefNew ();
        if (grp != NULL) {
          xref = SeqFeatXrefNew ();
          sfp->xref = xref;
          if (xref != NULL) {
            xref->data.choice = SEQFEAT_GENE;
            xref->data.value.ptrvalue = (Pointer) grp;
          }
        }
      }
      ompc.output_data = (Pointer) sfp;
      if (ompc.input_entityID == 0) {
        sfp->qual = DialogToPointer (ffp->gbquals);
        sfp->ext = DialogToPointer (ffp->usrobjext);
        if (HasExceptionGBQual (sfp)) {
          sfp->excpt = TRUE;
        }
        AddProtRefXref (sfp, ffp->protXrefName);
        if (! ObjMgrRegister (OBJ_SEQFEAT, (Pointer) sfp)) {
          Message (MSG_ERROR, "ObjMgrRegister failed");
        }
        SeqLocFree (slp);
        SeqEntrySetScope (oldscope);
        return TRUE;
      } else if (ompc.input_itemtype != OBJ_SEQFEAT) {
        sfp->qual = DialogToPointer (ffp->gbquals);
        sfp->ext = DialogToPointer (ffp->usrobjext);
        if (HasExceptionGBQual (sfp)) {
          sfp->excpt = TRUE;
        }
        AddProtRefXref (sfp, ffp->protXrefName);
        ompc.output_itemtype = OBJ_SEQFEAT;
        if (ompc.input_itemtype == OBJ_BIOSEQ) {
          bsp = GetBioseqGivenIDs (ompc.input_entityID, ompc.input_itemID, ompc.input_itemtype);
          if (bsp != NULL) {
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
            }
            if (sap != NULL) {
              itemID = GetItemIDGivenPointer (ompc.input_entityID, OBJ_SEQANNOT, (Pointer) sap);
              if (itemID > 0) {
                ompc.input_itemID = itemID;
                ompc.input_itemtype = OBJ_SEQANNOT;
              }
            }
          }
        }
        if (! AttachDataForProc (&ompc, FALSE)) {
          Message (MSG_ERROR, "AttachDataForProc failed");
        }
        rsult = TRUE;
      } else {
        rd.ffp = ffp;
        rd.sfp = sfp;
        GatherItem (ompc.input_entityID, ompc.input_itemID, ompc.input_itemtype,
                    (Pointer) &rd, ReplaceFeatureExtras);
        if (HasExceptionGBQual (sfp)) {
          sfp->excpt = TRUE;
        }
        AddProtRefXref (sfp, ffp->protXrefName);
        if (! ReplaceDataForProc (&ompc, FALSE)) {
          Message (MSG_ERROR, "ReplaceDataForProc failed");
        }
        rsult = TRUE;
      }
      if (ffp->gene != NULL && usexref == 1) {
        val = GetValue (ffp->gene);
        if (val == 2) {
          sep = GetBestTopParentForItemID (ffp->input_entityID,
                                           ffp->input_itemID,
                                           ffp->input_itemtype);
          /*
          sep = GetTopSeqEntryForEntityID (ffp->input_entityID);
          */
          if (sep != NULL) {
            sep = FindNucSeqEntry (sep);
          }
          if (sep != NULL && sep->data.ptrvalue != NULL) {
            GetTitle (ffp->geneSymbol, symbol, sizeof (symbol));
            GetTitle (ffp->geneDesc, desc, sizeof (desc));
            grp = CreateNewGeneRef (symbol, NULL, desc, FALSE);
            if (grp != NULL) {
              sfp = CreateNewFeature (sep, NULL, SEQFEAT_GENE, NULL);
              if (sfp != NULL) {
                sfp->data.value.ptrvalue = (Pointer) grp;
                sfp->location = SeqLocFree (sfp->location);
                sfp->location = DialogToPointer (ffp->location);
                bsp = GetBioseqGivenSeqLoc (sfp->location, ffp->input_entityID);
                if (bsp != NULL) {
                  hasNulls = LocationHasNullsBetween (sfp->location);
                  gslp = SeqLocMerge (bsp, sfp->location, NULL, TRUE, FALSE, FALSE);
                  if (gslp != NULL) {
                    CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
                    sfp->location = SeqLocFree (sfp->location);
                    sfp->location = gslp;
                    if (bsp->repr == Seq_repr_seg) {
                      gslp = SegLocToPartsEx (bsp, sfp->location, TRUE);
                      sfp->location = SeqLocFree (sfp->location);
                      sfp->location = gslp;
                      hasNulls = LocationHasNullsBetween (sfp->location);
                      sfp->partial = (sfp->partial || hasNulls);
                    }
                    FreeAllFuzz (sfp->location);
                    SetSeqLocPartial (sfp->location, noLeft, noRight);
                    sfp->partial = (sfp->partial || noLeft || noRight);
                  }
                }
              }
            }
          }
        } else if (val > 2) {
          ggl.ffp = ffp;
          ggl.omp = ObjMgrGet ();
          ggl.slp = slp;
          ggl.genexref = NULL;
          ggl.xrefmatch = FALSE;
          ggl.idx = 2;
          ggl.val = val;
          ggl.min = INT4_MAX;
          MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
          gs.seglevels = 1;
          gs.get_feats_location = TRUE;
	      MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
	      gs.ignore[OBJ_BIOSEQ] = FALSE;
	      gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
	      gs.ignore[OBJ_SEQFEAT] = FALSE;
	      gs.ignore[OBJ_SEQANNOT] = FALSE;
          gs.scope = GetBestTopParentForItemID (ffp->input_entityID,
                                                ffp->input_itemID,
                                                ffp->input_itemtype);
          GatherEntity (ffp->input_entityID, (Pointer) &ggl, GeneUpdateFunc, &gs);
        }
      }
      SeqLocFree (slp);
      SeqEntrySetScope (oldscope);
    }
  }
  return rsult;
}

extern void StdFeatFormActnProc (ForM f)

{
  FeatureFormPtr  ffp;

  if (FeatFormReplaceWithoutUpdateProc (f)) {
    ffp = (FeatureFormPtr) GetObjectExtra (f);
    if (ffp != NULL) {
      GetRidOfEmptyFeatsDescStrings (ffp->input_entityID, NULL);
      if (GetAppProperty ("InternalNcbiSequin") != NULL) {
        ExtendGeneFeatIfOnMRNA (ffp->input_entityID, NULL);
      }
      ObjMgrSetDirtyFlag (ffp->input_entityID, TRUE);
      ObjMgrSendMsg (OM_MSG_UPDATE, ffp->input_entityID,
                     ffp->input_itemID, ffp->input_itemtype);
    }
  }
}

extern void StdSeqFeatPtrToFeatFormProc (ForM f, Pointer data)

{
  FeatureFormPtr  ffp;
  SeqEntryPtr     oldsep;
  SeqEntryPtr     sep;
  SeqFeatPtr      sfp;
  Int4            val;

  ffp = (FeatureFormPtr) GetObjectExtra (f);
  if (ffp != NULL) {
    sep = GetTopSeqEntryForEntityID (ffp->input_entityID);
    oldsep = SeqEntrySetScope (sep);
    sfp = (SeqFeatPtr) data;
    if (sfp != NULL) {
      switch (sfp->data.choice) {
        case SEQFEAT_BOND :
        case SEQFEAT_SITE :
        case SEQFEAT_PSEC_STR :
          val = (Int4) sfp->data.value.intvalue;
          PointerToDialog (ffp->data, (Pointer) &(val));
          break;
        case SEQFEAT_COMMENT:
          break;
        default :
          PointerToDialog (ffp->data, sfp->data.value.ptrvalue);
          break;
      }
      SeqFeatPtrToCommon (ffp, sfp);
      PointerToDialog (ffp->location, sfp->location);
    }
    SeqEntrySetScope (oldsep);
  }
}

extern void StdInitFeatFormProc (ForM f)

{
  FeatureFormPtr  ffp;

  ffp = (FeatureFormPtr) GetObjectExtra (f);
  if (ffp != NULL) {
    PopulateGenePopup (ffp);
  }
}

extern void StdFeatFormAcceptButtonProc (ButtoN b)

{
  FeatureFormPtr  ffp;
  SeqLocPtr       slp;
  WindoW          w;

  if (b != NULL) {
    w = ParentWindow (b);
    ffp = (FeatureFormPtr) GetObjectExtra (b);
    if (ffp != NULL && ffp->form != NULL && ffp->actproc != NULL) {
      slp = DialogToPointer (ffp->location);
      if (slp == NULL) {
        ErrPostEx (SEV_ERROR, 0, 0, "Feature must have a location!");
        return;
      }
      SeqLocFree (slp);
      Hide (w);
      (ffp->actproc) (ffp->form);
    }
    Update ();
    Remove (w);
  }
}

extern void StdFeatFormCleanupProc (GraphiC g, VoidPtr data)

{
  FeatureFormPtr  ffp;
  Uint2           userkey;

  ffp = (FeatureFormPtr) data;
  if (ffp != NULL) {
    ValNodeFreeData (ffp->geneNames);
    if (ffp->input_entityID > 0 && ffp->userkey > 0) {
      userkey = ffp->userkey;
      ffp->userkey = 0;
      ObjMgrFreeUserData (ffp->input_entityID, ffp->procid, ffp->proctype, userkey);
    }
  }
  StdCleanupFormProc (g, data);
}

extern ValNodePtr AddStringToValNodeChain (ValNodePtr head, CharPtr str, Uint1 choice)

{
  ValNodePtr  vnp;

  vnp = ValNodeNew (head);
  if (head == NULL) {
    head = vnp;
  }
  if (vnp != NULL) {
    vnp->choice = choice;
    vnp->data.ptrvalue = StringSave (str);
  }
  return head;
}

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

extern CharPtr NameStdPtrToAuthorSpreadsheetString (NameStdPtr nsp);
extern CharPtr NameStdPtrToAuthorSpreadsheetString (NameStdPtr nsp)

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

static void StdAuthListPtrToAuthorDialog (DialoG d, Pointer data)

{
  AuthListPtr  alp;
  AuthorPtr    ap;
  ValNodePtr   head;
  Int2         j;
  ValNodePtr   names;
  NameStdPtr   nsp;
  PersonIdPtr  pid;
  TagListPtr   tlp;
  ValNodePtr   vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  alp = (AuthListPtr) data;
  if (tlp != NULL) {
    head = NULL;
    if (alp != NULL) {
      if (alp->choice == 1) {
        names = alp->names;
        while (names != NULL) {
          ap = names->data.ptrvalue;
          if (ap != NULL) {
            pid = ap->name;
            if (pid != NULL) {
              if (pid->choice == 2) {
                nsp = pid->data;
                if (nsp != NULL) {
                  vnp = ValNodeNew (head);
                  if (head == NULL) {
                    head = vnp;
                  }
                  if (vnp != NULL) {
                    vnp->data.ptrvalue = NameStdPtrToAuthorSpreadsheetString (nsp);
                  }
                }
              }
            }
          }
          names = names->next;
        }
      } else {
        Message (MSG_ERROR, "Unable to handle author type %d", (int) alp->choice);
      }
    }
    SendMessageToDialog (tlp->dialog, VIB_MSG_RESET);
    tlp->vnp = head;
    SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
    for (j = 0, vnp = tlp->vnp; vnp != NULL; j++, vnp = vnp->next) {
    }
    tlp->max = MAX ((Int2) 0, (Int2) (j - tlp->rows + 1));
    CorrectBarMax (tlp->bar, tlp->max);
    CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
  }
}

extern NameStdPtr AuthorSpreadsheetStringToNameStdPtr (CharPtr txt);
extern NameStdPtr AuthorSpreadsheetStringToNameStdPtr (CharPtr txt)

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
  nsp->names [0] = ExtractTagListColumn (txt, 2);
  TrimLeadingSpaces (nsp->names [0]);
  first = ExtractTagListColumn (txt, 0);
  StripPeriods (first);
  nsp->names [1] = StringSave (first);
  TrimLeadingSpaces (nsp->names [1]);
  FirstNameToInitials (first, str1, sizeof (str1) - 1);
  str = ExtractTagListColumn (txt, 1);
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
  periods [k] = '\0';
  nsp->names [4] = StringSave (periods);
  TrimLeadingSpaces (nsp->names [4]);
  str = ExtractTagListColumn (txt, 3);
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
  nsp->names [2] = ExtractTagListColumn (txt, 4);
  TrimLeadingSpaces (nsp->names [2]);
  if (StringHasNoText (nsp->names [2])) {
    nsp->names [2] = MemFree (nsp->names [2]);
  }
  return nsp;
}

static Pointer AuthorDialogToStdAuthListPtr (DialoG d)

{
  AuthListPtr  alp;
  AuthorPtr    ap;
  Char         ch;
  Int2         j;
  Int2         len;
  ValNodePtr   names;
  NameStdPtr   nsp;
  Boolean      okay;
  PersonIdPtr  pid;
  CharPtr      str;
  TagListPtr   tlp;
  ValNodePtr   vnp;

  alp = NULL;
  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp != NULL && tlp->vnp != NULL) {
    alp = AuthListNew ();
    if (alp != NULL) {
      alp->choice = 1;
      names = NULL;
      for (vnp = tlp->vnp; vnp != NULL; vnp = vnp->next) {
        str = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 2);
        okay = FALSE;
        len = StringLen (str);
        for (j = 0; j < len; j++) {
          ch = str [j];
          if (ch != ' ' && ch != '\t' && ch != '\n') {
            okay = TRUE;
          }
        }
        MemFree (str);
        if (okay) {
          names = ValNodeNew (names);
          if (alp->names == NULL) {
            alp->names = names;
          }
          if (names != NULL) {
            ap = AuthorNew ();
            names->choice = 1;
            names->data.ptrvalue = ap;
            if (ap != NULL) {
              pid = PersonIdNew ();
              ap->name = pid;
              if (pid != NULL) {
                pid->choice = 2;
                nsp = AuthorSpreadsheetStringToNameStdPtr ((CharPtr) vnp->data.ptrvalue);
                pid->data = nsp;
              }
            }
          }
        }
      }
      if (alp->names == NULL) {
        alp = AuthListFree (alp);
      }
    }
  }
  return (Pointer) alp;
}

static void StrAuthListPtrToAuthorDialog (DialoG d, Pointer data)

{
  AuthListPtr  alp;
  ValNodePtr   head;
  Int2         j;
  ValNodePtr   names;
  Char         str [128];
  TagListPtr   tlp;
  ValNodePtr   vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  alp = (AuthListPtr) data;
  if (tlp != NULL) {
    head = NULL;
    if (alp != NULL) {
      if (alp->choice == 2 || alp->choice == 3) {
        names = alp->names;
        while (names != NULL) {
          StringNCpy_0 (str, names->data.ptrvalue, sizeof (str) - 2);
          StringCat (str, "\n");
          vnp = ValNodeNew (head);
          if (head == NULL) {
            head = vnp;
          }
          if (vnp != NULL) {
            vnp->data.ptrvalue = StringSave (str);
          }
          names = names->next;
        }
      } else {
        Message (MSG_ERROR, "Unable to handle author type %d", (int) alp->choice);
      }
    }
    SendMessageToDialog (tlp->dialog, VIB_MSG_RESET);
    tlp->vnp = head;
    SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
    for (j = 0, vnp = tlp->vnp; vnp != NULL; j++, vnp = vnp->next) {
    }
    tlp->max = MAX ((Int2) 0, (Int2) (j - tlp->rows + 1));
    CorrectBarMax (tlp->bar, tlp->max);
    CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
  }
}

static Pointer AuthorDialogToStrAuthListPtr (DialoG d)

{
  AuthListPtr  alp;
  Char         ch;
  Int2         j;
  Int2         len;
  ValNodePtr   names;
  Boolean      okay;
  CharPtr      str;
  TagListPtr   tlp;
  ValNodePtr   vnp;

  alp = NULL;
  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp != NULL && tlp->vnp != NULL) {
    alp = AuthListNew ();
    if (alp != NULL) {
      alp->choice = 2;
      names = NULL;
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
          names = ValNodeNew (names);
          if (alp->names == NULL) {
            alp->names = names;
          }
          if (names != NULL) {
            names->choice = 2;
            names->data.ptrvalue = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 0);

          }
        }
      }
      if (alp->names == NULL) {
        alp = AuthListFree (alp);
      }
    }
  }
  return (Pointer) alp;
}

Uint2 author_types [] = {
  TAGLIST_TEXT, TAGLIST_TEXT, TAGLIST_TEXT, TAGLIST_TEXT
};

Uint2 std_author_widths [] = {
  8, 4, 9, 3
};

Uint2 str_author_widths [] = {
  24
};

typedef struct authordialog {
  DIALOG_MESSAGE_BLOCK
  DialoG             stdAuthor;
  DialoG             strAuthor;
  GrouP              stdGrp;
  GrouP              strGrp;
  Uint1              type;
} AuthorDialog, PNTR AuthorDialogPtr;

static void AuthListPtrToAuthorDialog (DialoG d, Pointer data)

{
  AuthorDialogPtr  adp;
  AuthListPtr      alp;

  adp = (AuthorDialogPtr) GetObjectExtra (d);
  if (adp != NULL) {
    alp = (AuthListPtr) data;
    if (alp != NULL) {
      adp->type = alp->choice;
    }
    if (adp->type == 1) {
      Hide (adp->strGrp);
      Show (adp->stdGrp);
      PointerToDialog (adp->stdAuthor, data);
    } else if (adp->type == 2 || adp->type == 3) {
      Hide (adp->stdGrp);
      Show (adp->strGrp);
      PointerToDialog (adp->strAuthor, data);
    }
  }
}

static Pointer AuthorDialogToAuthListPtr (DialoG d)

{
  AuthorDialogPtr  adp;
  AuthListPtr      alp;

  adp = (AuthorDialogPtr) GetObjectExtra (d);
  alp = NULL;
  if (adp != NULL) {
    if (adp->type == 1) {
      alp = (AuthListPtr) DialogToPointer (adp->stdAuthor);
    } else if (adp->type == 2 || adp->type == 3) {
      if ((alp = (AuthListPtr) DialogToPointer (adp->strAuthor)) != NULL)
        alp->choice = adp->type;
    }
  }
  return (Pointer) alp;
}

static void AuthorDialogMessage (DialoG d, Int2 mssg)

{
  AuthorDialogPtr  adp;

  adp = (AuthorDialogPtr) GetObjectExtra (d);
  if (adp != NULL) {
    switch (mssg) {
      case VIB_MSG_ENTER :
        if (adp->type == 1) {
          SendMessageToDialog (adp->stdAuthor, VIB_MSG_ENTER);
        } else if (adp->type == 2 || adp->type == 3) {
          SendMessageToDialog (adp->strAuthor, VIB_MSG_ENTER);
        }
        break;
      default :
        break;
    }
  }
}

static Boolean ReadAuthorDialog (DialoG d, CharPtr filename)

{
  AuthorDialogPtr  adp;
  AsnIoPtr         aip;
  AuthListPtr      alp;
  Char             path [PATH_MAX];

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  adp = (AuthorDialogPtr) GetObjectExtra (d);
  if (adp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        alp = AuthListAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (alp != NULL) {
          PointerToDialog (adp->dialog, (Pointer) alp);
          alp = AuthListFree (alp);
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean WriteAuthorDialog (DialoG d, CharPtr filename)

{
  AuthorDialogPtr  adp;
  AsnIoPtr         aip;
  AuthListPtr      alp;
  Char             path [PATH_MAX];
#ifdef WIN_MAC
  FILE            *f;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  adp = (AuthorDialogPtr) GetObjectExtra (d);
  if (adp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      f = FileOpen (path, "r");
      if (f != NULL) {
        FileClose (f);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      aip = AsnIoOpen (path, "w");
      if (aip != NULL) {
        alp = DialogToPointer (adp->dialog);
        AuthListAsnWrite (alp, aip, NULL);
        AsnIoClose (aip);
        alp = AuthListFree (alp);
        return TRUE;
      }
    }
  }
  return FALSE;
}

extern DialoG CreateAuthorDialog (GrouP prnt, Uint2 rows, Int2 spacing)

{
  AuthorDialogPtr  adp;
  GrouP            k;
  GrouP            p;
  PrompT           p1, p2, p3, p4, p5;
  TagListPtr       tlp;

  p = HiddenGroup (prnt, 0, 0, NULL);

  adp = (AuthorDialogPtr) MemNew (sizeof (AuthorDialog));
  if (adp != NULL) {

    SetObjectExtra (p, adp, StdCleanupExtraProc);
    adp->dialog = (DialoG) p;
    adp->todialog = AuthListPtrToAuthorDialog;
    adp->fromdialog = AuthorDialogToAuthListPtr;
    adp->testdialog = NULL;
    adp->importdialog = ReadAuthorDialog;
    adp->exportdialog = WriteAuthorDialog;
    adp->dialogmessage = AuthorDialogMessage;

    adp->strGrp = HiddenGroup (p, -1, 0, NULL);
    SetGroupSpacing (adp->strGrp, 3, 2);

    k = HiddenGroup (adp->strGrp, -4, 0, NULL);
    SetGroupSpacing (k, spacing, spacing);
    p1 = StaticPrompt (k, "Name", 0, 0, programFont, 'c');

    adp->strAuthor = CreateTagListDialog (adp->strGrp, rows, 1, spacing,
                                          author_types, str_author_widths, NULL,
                                          StrAuthListPtrToAuthorDialog,
                                          AuthorDialogToStrAuthListPtr);
    Hide (adp->strGrp);

    adp->stdGrp = HiddenGroup (p, -1, 0, NULL);
    SetGroupSpacing (adp->stdGrp, 3, 2);

    k = HiddenGroup (adp->stdGrp, -4, 0, NULL);
    SetGroupSpacing (k, spacing, spacing);
    p2 = StaticPrompt (k, "First Name", 0, 0, programFont, 'c');
    p3 = StaticPrompt (k, "M.I.", 0, 0, programFont, 'c');
    p4 = StaticPrompt (k, "Last Name", 0, 0, programFont, 'c');
    p5 = StaticPrompt (k, "Sfx", 0, 0, programFont, 'c');

    adp->stdAuthor = CreateTagListDialog (adp->stdGrp, rows, 4, spacing,
                                          author_types, std_author_widths, NULL,
                                          StdAuthListPtrToAuthorDialog,
                                          AuthorDialogToStdAuthListPtr);
    adp->type = 1;

    tlp = (TagListPtr) GetObjectExtra (adp->strAuthor);
    if (tlp != NULL) {
      AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control [0], (HANDLE) p1, NULL);
    }
    tlp = (TagListPtr) GetObjectExtra (adp->stdAuthor);
    if (tlp != NULL) {
      AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control [0], (HANDLE) p2, NULL);
      AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control [1], (HANDLE) p3, NULL);
      AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control [2], (HANDLE) p4, NULL);
      AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control [3], (HANDLE) p5, NULL);
    }
  }

  return (DialoG) p;
}

typedef struct affildialog {
  DIALOG_MESSAGE_BLOCK
  TexT            affil;
  TexT            div;
  TexT            address;
  TexT            city;
  TexT            state;
  TexT            zip;
  TexT            country;
  TexT            phone;
  TexT            fax;
  TexT            email;
} AffilDialog, PNTR AffilDialogPtr;

static void AffilPtrToAffilDialog (DialoG d, Pointer data)

{
  AffilDialogPtr  adp;
  AffilPtr        ap;

  adp = (AffilDialogPtr) GetObjectExtra (d);
  ap = (AffilPtr) data;
  if (adp != NULL) {
    if (ap != NULL) {
      SafeSetTitle (adp->affil, ap->affil);
      SafeSetTitle (adp->div, ap->div);
      SafeSetTitle (adp->address, ap->street);
      SafeSetTitle (adp->city, ap->city);
      SafeSetTitle (adp->state, ap->sub);
      SafeSetTitle (adp->zip, ap->postal_code);
      SafeSetTitle (adp->country, ap->country);
      SafeSetTitle (adp->phone, ap->phone);
      SafeSetTitle (adp->fax, ap->fax);
      SafeSetTitle (adp->email, ap->email);
    } else {
      SafeSetTitle (adp->affil, "");
      SafeSetTitle (adp->div, "");
      SafeSetTitle (adp->address, "");
      SafeSetTitle (adp->city, "");
      SafeSetTitle (adp->state, "");
      SafeSetTitle (adp->zip, "");
      SafeSetTitle (adp->country, "");
      SafeSetTitle (adp->phone, "");
      SafeSetTitle (adp->fax, "");
      SafeSetTitle (adp->email, "");
    }
  }
}

static Pointer AffilDialogToAffilPtr (DialoG d)

{
  AffilDialogPtr  adp;
  AffilPtr        ap;

  ap = NULL;
  adp = (AffilDialogPtr) GetObjectExtra (d);
  if (adp != NULL) {
    ap = AffilNew ();
    if (ap != NULL) {
      ap->affil = SaveStringFromText (adp->affil);
      ap->div = SaveStringFromText (adp->div);
      ap->street = SaveStringFromText (adp->address);
      ap->city = SaveStringFromText (adp->city);
      ap->sub = SaveStringFromText (adp->state);
      ap->postal_code = SaveStringFromText (adp->zip);
      ap->country = SaveStringFromText (adp->country);
      ap->phone = SaveStringFromText (adp->phone);
      ap->fax = SaveStringFromText (adp->fax);
      ap->email = SaveStringFromText (adp->email);
      if (ap->div == NULL && ap->street == NULL && ap->city == NULL &&
           ap->sub == NULL && ap->postal_code == NULL && ap->country == NULL &&
           ap->phone == NULL && ap->fax == NULL && ap->email == NULL) {
        ap->choice = 1;
        if (ap->affil == NULL) {
          ap = AffilFree (ap);
        }
      } else {
        ap->choice = 2;
      }
    }
  }
  return (Pointer) ap;
}

static void AffilDialogMessage (DialoG d, Int2 mssg)

{
  AffilDialogPtr  adp;

  adp = (AffilDialogPtr) GetObjectExtra (d);
  if (adp != NULL) {
    switch (mssg) {
      case VIB_MSG_ENTER :
        Select (adp->affil);
        break;
      default :
        break;
    }
  }
}

static DialoG CreateAnAffilDialog (GrouP prnt, CharPtr title,
                                   Boolean publisher,
                                   Boolean split,
                                   Boolean proceedings,
                                   GrouP PNTR grp1, GrouP PNTR grp2)

{
  AffilDialogPtr  adp;
  GrouP           g;
  GrouP           g1, g2;
  GrouP           j;
  GrouP           m;
  GrouP           p;
  GrouP           q;
  GrouP           s;

  p = HiddenGroup (prnt, 0, 0, NULL);

  adp = (AffilDialogPtr) MemNew (sizeof (AffilDialog));
  if (adp != NULL) {

    SetObjectExtra (p, adp, StdCleanupExtraProc);
    adp->dialog = (DialoG) p;
    adp->todialog = AffilPtrToAffilDialog;
    adp->fromdialog = AffilDialogToAffilPtr;
    adp->testdialog = NULL;
    adp->dialogmessage = AffilDialogMessage;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    m = HiddenGroup (s, 0, 0, NULL);

    q = HiddenGroup (m, 2, 0, NULL);
    g1 = q;
    g2 = NULL;
    if (grp1 != NULL) {
      *grp1 = q;
    }
    if (grp2 != NULL) {
      *grp2 = NULL;
    }
    g = HiddenGroup (q, 0, 20, NULL);
    if (publisher) {
      StaticPrompt (g, "Publisher", 0, dialogTextHeight, programFont, 'l');
    } else if (proceedings) {
      StaticPrompt (g, "Location", 0, dialogTextHeight, programFont, 'l');
    } else {
      StaticPrompt (g, "Institution", 0, dialogTextHeight, programFont, 'l');
      StaticPrompt (g, "Department", 0, dialogTextHeight, programFont, 'l');
    }
    StaticPrompt (g, "Address", 0, dialogTextHeight, programFont, 'l');
    StaticPrompt (g, "City", 0, dialogTextHeight, programFont, 'l');
    StaticPrompt (g, "State/Province", 0, dialogTextHeight, programFont, 'l');
    StaticPrompt (g, "Country", 0, dialogTextHeight, programFont, 'l');
    if (! split) {
      if (! proceedings) {
        StaticPrompt (g, "", 0, stdLineHeight, programFont, 'l');
        StaticPrompt (g, "Phone", 0, dialogTextHeight, programFont, 'l');
        if (publisher) {
          StaticPrompt (g, "Internet Access", 0, dialogTextHeight, programFont, 'l');
        } else {
          StaticPrompt (g, "Email", 0, dialogTextHeight, programFont, 'l');
        }
      }
    }
    j = HiddenGroup (q, 0, 20, NULL);
    g = HiddenGroup (j, 0, 20, NULL);
    adp->affil = DialogText (g, "", 30, NULL);
    adp->div = NULL;
    if (! publisher && ! proceedings) {
      adp->div = DialogText (g, "", 30, NULL);
    }
    adp->address = DialogText (g, "", 30, NULL);
    adp->city = DialogText (g, "", 30, NULL);
    g = HiddenGroup (j, 3, 0, NULL);
    SetGroupSpacing (g, 20, 2);
    adp->state = DialogText (g, "", 10, NULL);
    if (! proceedings) {
      /* StaticPrompt (g, "Zip/Postal Code", 7 * stdCharWidth,
                    dialogTextHeight, programFont, 'l'); */
      StaticPrompt (g, "Zip/Postal Code", 0, dialogTextHeight, programFont, 'l');
      adp->zip = DialogText (g, "", 10, NULL);
    }
    g = HiddenGroup (j, 0, 20, NULL);
    adp->country = DialogText (g, "", 30, NULL);
    if (split) {
      if (! proceedings) {
        q = HiddenGroup (m, 2, 0, NULL);
        g2 = q;
        if (grp2 != NULL) {
          *grp2 = q;
        }
        g = HiddenGroup (q, 0, 20, NULL);
        StaticPrompt (g, "", 0, stdLineHeight, programFont, 'l');
        StaticPrompt (g, "Phone", 0, dialogTextHeight, programFont, 'l');
        if (publisher) {
          StaticPrompt (g, "Internet Access", 0, dialogTextHeight, programFont, 'l');
        } else {
          StaticPrompt (g, "Email", 0, dialogTextHeight, programFont, 'l');
        }
        j = HiddenGroup (q, 0, 20, NULL);
      }
    }
    if (! proceedings) {
      StaticPrompt (j, "Please include country code for non-U.S. phone numbers.",
                    0, stdLineHeight, programFont, 'l');
      g = HiddenGroup (j, 3, 0, NULL);
      SetGroupSpacing (g, 20, 2);
      adp->phone = DialogText (g, "", 10, NULL);
      if (split) {
        StaticPrompt (g, "Fax", 0,
                      dialogTextHeight, programFont, 'l');
      } else {
        StaticPrompt (g, "Fax", 7 * stdCharWidth,
                      dialogTextHeight, programFont, 'l');
      }
      adp->fax = DialogText (g, "", 10, NULL);
      g = HiddenGroup (j, 0, 20, NULL);
      if (split) {
        adp->email = DialogText (g, "", 20, NULL);
      } else {
        adp->email = DialogText (g, "", 30, NULL);
      }
    }

    if (split) {
      AlignObjects (ALIGN_RIGHT, (HANDLE) adp->affil,
                    (HANDLE) adp->address, (HANDLE) adp->city,
                    (HANDLE) adp->zip, (HANDLE) adp->country,
                    (HANDLE) adp->div, NULL);
      AlignObjects (ALIGN_RIGHT, (HANDLE) adp->fax, (HANDLE) adp->email, NULL);
      AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) g2, NULL);
      Hide (g1);
      Hide (g2);
    } else {
      AlignObjects (ALIGN_RIGHT, (HANDLE) adp->affil,
                    (HANDLE) adp->address, (HANDLE) adp->city,
                    (HANDLE) adp->zip, (HANDLE) adp->country,
                    (HANDLE) adp->fax, (HANDLE) adp->email,
                    (HANDLE) adp->div, NULL);
    }
  }

  return (DialoG) p;
}

extern DialoG CreateAffilDialog (GrouP prnt, CharPtr title)

{
  return CreateAnAffilDialog (prnt, title, FALSE, FALSE, FALSE, NULL, NULL);
}

extern DialoG CreatePublisherAffilDialog (GrouP prnt, CharPtr title)

{
  return CreateAnAffilDialog (prnt, title, TRUE, FALSE, FALSE, NULL, NULL);
}

extern DialoG CreateProceedingsDialog (GrouP prnt, CharPtr title)

{
  return CreateAnAffilDialog (prnt, title, FALSE, FALSE, TRUE, NULL, NULL);
}

extern DialoG CreateExtAffilDialog (GrouP prnt, CharPtr title, GrouP PNTR grp1, GrouP PNTR grp2)

{
  return CreateAnAffilDialog (prnt, title, FALSE, TRUE, FALSE, grp1, grp2);
}

extern DialoG CreateExtPublisherAffilDialog (GrouP prnt, CharPtr title, GrouP PNTR grp1, GrouP PNTR grp2)

{
  return CreateAnAffilDialog (prnt, title, TRUE, TRUE, FALSE, grp1, grp2);
}

extern DialoG CreateExtProceedingsDialog (GrouP prnt, CharPtr title, GrouP PNTR grp1, GrouP PNTR grp2)

{
  return CreateAnAffilDialog (prnt, title, FALSE, TRUE, TRUE, grp1, grp2);
}

extern void DatePtrToVibrant (DatePtr dp, PopuP dateMonth, TexT dateDay, TexT dateYear)

{
  Int2  day;
  Char  str [32];
  Int2  year;

  if (dp != NULL) {
    if (dp->data [0] == 0) {
      DatePrint (dp, str);
    } else if (dp->data [0] == 1) {
      SetEnumPopup (dateMonth, months_alist, (UIEnum) dp->data [2]);
      day = (Int2) dp->data [3];
      if (day > 0 && day <= 31) {
        sprintf (str, "%d", (int) day);
        SafeSetTitle (dateDay, str);
      } else {
        SafeSetTitle (dateDay, "");
      }
      year = (Int2) dp->data [1];
      if (year > 0) {
        sprintf (str, "%d", (int) (year + 1900));
        SafeSetTitle (dateYear, str);
      } else {
        SafeSetTitle (dateYear, "");
      }
    } else {
      Message (MSG_ERROR, "Unknown date type");
    }
  } else {
    SafeSetValue (dateMonth, 1);
    SafeSetTitle (dateDay, "");
    SafeSetTitle (dateYear, "");
  }
}

extern DatePtr VibrantToDatePtr (PopuP dateMonth, TexT dateDay, TexT dateYear)

{
  Int2     day;
  Int2     dateType;
  DatePtr  dp;
  UIEnum   month;
  Char     str [32];
  Int2     year;

  dp = DateNew ();
  if (dp != NULL) {
    dateType = 1;
    dp->data [0] = (Uint1) dateType;
    if (dateType == 0) {
    } else if (dateType == 1) {
      GetTitle (dateYear, str, sizeof (str));
      if (! StringHasNoText (str)) {
        StrToInt (str, &year);
        if (year >= 1900) {
          dp->data [1] = (Uint1) (year - 1900);
        } else {
          /* dp->data [1] = 0; */
          dp = DateFree (dp);
          return dp;
        }
        if (GetEnumPopup (dateMonth, months_alist, &month)) {
          dp->data [2] = (Uint1) month;
        }
        GetTitle (dateDay, str, sizeof (str));
        StrToInt (str, &day);
        dp->data [3] = (Uint1) day;
      } else {
        dp = DateFree (dp);
      }
    } else {
      Message (MSG_ERROR, "Unknown date type");
    }
  }
  return dp;
}

static void GBQualPtrToQualsDialog (DialoG d, Pointer data)

{
  ValNodePtr  head;
  Int2        j;
  size_t      len;
  GBQualPtr   list;
  CharPtr     str;
  TagListPtr  tlp;
  ValNodePtr  vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  list = (GBQualPtr) data;
  if (tlp != NULL) {
    head = NULL;
    while (list != NULL) {
      vnp = ValNodeNew (head);
      if (head == NULL) {
        head = vnp;
      }
      if (vnp != NULL) {
        if (list->qual != NULL && list->val != NULL) {
          len = StringLen (list->qual) + StringLen (list->val);
          str = MemNew (len + 4);
          if (str != NULL) {
            StringCpy (str, list->qual);
            StringCat (str, "\t");
            StringCat (str, list->val);
            StringCat (str, "\n");
          }
          vnp->data.ptrvalue = str;
        }
      }
      list = list->next;
    }
    SendMessageToDialog (tlp->dialog, VIB_MSG_RESET);
    tlp->vnp = head;
    SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
    for (j = 0, vnp = tlp->vnp; vnp != NULL; j++, vnp = vnp->next) {
    }
    tlp->max = MAX ((Int2) 0, (Int2) (j - tlp->rows + 1));
    CorrectBarMax (tlp->bar, tlp->max);
    CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
  }
}

static Pointer QualsDialogToGBQualPtr (DialoG d)

{
  Char         ch;
  GBQualPtr    gbq;
  GBQualPtr    gbqlast;
  GBQualPtr    head;
  Int2         j;
  Int2         len;
  Boolean      okay;
  CharPtr      str;
  TagListPtr   tlp;
  ValNodePtr   vnp;

  head = NULL;
  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp != NULL && tlp->vnp != NULL) {
    gbq = NULL;
    gbqlast = NULL;
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
        gbq = GBQualNew ();
        if (gbqlast == NULL) {
          head = gbq;
        } else {
          gbqlast->next = gbq;
        }
        gbqlast = gbq;
        if (gbq != NULL) {
          gbq->qual = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 0);
          gbq->val = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 1);
        }
      }
    }
  }
  return (Pointer) head;
}

Uint2 gbqual_types [] = {
  TAGLIST_TEXT, TAGLIST_TEXT
};

Uint2 gbqual_widths [] = {
  0, 0, 0, 0
};

extern DialoG CreateQualsDialog (GrouP h, Uint2 rows, Int2 spacing,
                                 Int2 width1, Int2 width2);
extern DialoG CreateQualsDialog (GrouP h, Uint2 rows, Int2 spacing,
                                 Int2 width1, Int2 width2)

{
  gbqual_widths [0] = width1;
  gbqual_widths [1] = width2;
  return CreateTagListDialog (h, rows, 2, spacing,
                              gbqual_types, gbqual_widths, NULL,
                              GBQualPtrToQualsDialog,
                              QualsDialogToGBQualPtr);
}

typedef struct intervalpage {
  DIALOG_MESSAGE_BLOCK
  DialoG             ivals;
  ButtoN             partial5;
  ButtoN             partial3;
  ButtoN             nullsBetween;
  Int2               count;
  SeqEntryPtr        PNTR bsptr;
  EnumFieldAssoc     PNTR alist;
  EnumFieldAssocPtr  alists [4];
  Int4               PNTR lengths;
  Boolean            nucsOK;
  Boolean            protsOK;
  Boolean            showIdTags;
  FeatureFormPtr     ffp;
  IntEdPartialProc   proc;
} IntervalPage, PNTR IntervalPagePtr;

#define NUM_IVAL_ROWS  7
#define EXTRA_HEIGHT   2

static void AddToBsptrList (IntervalPagePtr ipp, SeqEntryPtr sep)

{
  Int2  j;

  if (ipp == NULL || sep == NULL) return;
  for (j = 1; j <= ipp->count; j++) {
    if (sep == ipp->bsptr [j]) return; /* already exists in list */
  }
  ipp->count++;
  ipp->bsptr [ipp->count] = sep;
}

static void FillInProducts (SeqEntryPtr sep, Pointer mydata,
                            Int4 index, Int2 indent)

{
  BioseqPtr        bsp;
  IntervalPagePtr  ipp;
  SeqIdPtr         sip;
  SeqLocPtr        slp;
  ValNode          vn;

  if (sep != NULL && mydata != NULL && sep->choice == 1) {
    ipp = (IntervalPagePtr) mydata;
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp != NULL) {
      if ((ipp->nucsOK && ISA_na (bsp->mol)) ||
          (ipp->protsOK && ISA_aa (bsp->mol))) {
        AddToBsptrList (ipp, sep);
      }
      if (bsp->repr == Seq_repr_seg && bsp->seq_ext != NULL) {
        vn.choice = SEQLOC_MIX;
        vn.next = NULL;
        vn.data.ptrvalue = bsp->seq_ext;
        slp = SeqLocFindNext (&vn, NULL);
        while (slp != NULL) {
          sip = SeqLocId (slp);
          if (sip != NULL) {
            bsp = BioseqFindCore (sip);
            if (bsp != NULL) {
              sep = SeqMgrGetSeqEntryForData (bsp);
              AddToBsptrList (ipp, sep);
            } else {
              bsp = BioseqLockById (sip);
              if (bsp != NULL) {
                sep = SeqMgrGetSeqEntryForData (bsp);
                AddToBsptrList (ipp, sep);
              }
              BioseqUnlockById (sip);
            }
          }
          slp = SeqLocFindNext (&vn, slp);
        }
      }
    }
  }
}

static Int4 SegmentedEntryList (SeqEntryPtr sep, Pointer mydata,
                                SeqEntryFunc mycallback,
                                Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqLocPtr     slp;
  ValNode       vn;

  if (sep == NULL) return index;
  if (IS_Bioseq (sep)) {
    if (mycallback != NULL)
      (*mycallback) (sep, mydata, index, indent);
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp != NULL && bsp->repr == Seq_repr_seg && bsp->seq_ext != NULL) {
      vn.choice = SEQLOC_MIX;
      vn.next = NULL;
      vn.data.ptrvalue = bsp->seq_ext;
      slp = SeqLocFindNext (&vn, NULL);
      while (slp != NULL) {
        index++;
        slp = SeqLocFindNext (&vn, slp);
      }
    }
    return index + 1;
  }
  /*
  if (Bioseq_set_class (sep) == 4) return index;
  index++;
  */
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  sep = bssp->seq_set;
  indent++;
  while (sep != NULL) {
    index = SegmentedEntryList (sep, mydata, mycallback, index, indent);
    sep = sep->next;
  }
  return index;
}

#define SegmentedEntryCount( a )  SegmentedEntryList( a ,NULL,NULL,0,0);

static ENUM_ALIST(strand_alist)
{" ",             Seq_strand_unknown},  /* 0 */
{"Plus",          Seq_strand_plus},     /* 1 */
{"Minus",         Seq_strand_minus},    /* 2 */
{"Both",          Seq_strand_both},     /* 3 */
{"Reverse",       Seq_strand_both_rev}, /* 4 */
{"Other",         Seq_strand_other},    /* 255 */
END_ENUM_ALIST

static void SeqLocPtrToIntervalPage (DialoG d, Pointer data)

{
  BioseqPtr        bsp;
  SeqLocPtr        firstSlp;
  Char             fuzz_from_ch [4];
  Char             fuzz_to_ch [4];
  ValNodePtr       head;
  SeqIdPtr         id;
  IntFuzzPtr       ifp;
  IntervalPagePtr  ipp;
  Boolean          isInterval;
  Boolean          isPoint;
  Int2             j;
  SeqLocPtr        lastSlp;
  SeqLocPtr        location;
  SeqLocPtr        next;
  SeqEntryPtr      oldscope;
  Boolean          partial5;
  Boolean          partial3;
  SeqEntryPtr      sep;
  Int2             seq;
  SeqIntPtr        sip;
  SeqLocPtr        slp;
  SeqPntPtr        spp;
  Int4             start;
  Int4             stop;
  Char             str [128];
  Uint1            strand;
  TagListPtr       tlp;
  ValNodePtr       vnp;

  ipp = (IntervalPagePtr) GetObjectExtra (d);
  if (ipp == NULL) return;
  SafeSetStatus (ipp->partial5, FALSE);
  SafeSetStatus (ipp->partial3, FALSE);
  tlp = GetObjectExtra (ipp->ivals);
  if (tlp == NULL) return;

  location = (SeqLocPtr) data;
  partial5 = FALSE;
  partial3 = FALSE;
  head = NULL;

  if (location != NULL) {
    firstSlp = NULL;
    lastSlp = NULL;
    slp = SeqLocFindNext (location, NULL);
    while (slp != NULL) {
      if (firstSlp == NULL) {
        firstSlp = slp;
      }
      lastSlp = slp;
      next = SeqLocFindNext (location, slp);
      if (slp->choice == SEQLOC_NULL) {
        SafeSetStatus (ipp->nullsBetween, TRUE);
        SafeShow (ipp->nullsBetween);
      } else {
        id = SeqLocId (slp);
        if (id != NULL) {
          bsp = BioseqFind (id);
          if (bsp == NULL) {
            oldscope = SeqEntrySetScope (NULL);
            if (oldscope != NULL) {
              bsp = BioseqFind (id);
              SeqEntrySetScope (oldscope);
            }
          }
          if (bsp != NULL) {
            isInterval = TRUE;
            isPoint = FALSE;
            StringCpy (fuzz_from_ch, "");
            StringCpy (fuzz_to_ch, "");
            start = GetOffsetInBioseq (slp, bsp, SEQLOC_START);
            stop = GetOffsetInBioseq (slp, bsp, SEQLOC_STOP);
            if (start == stop && slp->choice == SEQLOC_PNT) {
              spp = (SeqPntPtr) slp->data.ptrvalue;
              if (spp != NULL) {
                ifp = spp->fuzz;
                if (ifp != NULL && ifp->choice == 4 && ifp->a ==  3) {
                  isInterval = FALSE;
                  isPoint = TRUE;
                  StringCpy (fuzz_from_ch, "^");
                  /* start--; */  /* compensate for other fix */
                  stop++; /* compensate for other fix */
                }
              }
            }
            strand = SeqLocStrand (slp);
            if (strand > Seq_strand_both_rev && strand != Seq_strand_other) {
              strand = Seq_strand_unknown;
            }
            /*
            if (strand == Seq_strand_unknown) {
              strand = Seq_strand_plus;
            }
            */
            if (! ipp->nucsOK) {
              strand = 0;
            }
            seq = 0;
            if (ipp->bsptr != NULL) {
              for (j = 1; j <= ipp->count && seq == 0; j++) {
                sep = ipp->bsptr [j];
                if (sep != NULL && sep->choice == 1) {
                  if (bsp == (BioseqPtr) sep->data.ptrvalue) {
                    seq = j;
                  }
                }
              }
            }
            if (ipp->nucsOK) {
              if (isInterval) {
                sprintf (str, "%s%ld\t%s%ld\t%d\t%d\n",
                         fuzz_from_ch, (long) (start + 1),
                         fuzz_to_ch, (long) (stop + 1),
                         (int) strand, (int) seq);
              } else if (isPoint) {
                /*
                sprintf (str, "%s%ld\t%s%ld\t%d\t%d\n",
                         fuzz_from_ch, (long) (start + 1),
                         fuzz_to_ch, (long) (stop + 1),
                         (int) strand, (int) seq);
                */
                sprintf (str, "%ld%s\t%ld%s\t%d\t%d\n",
                         (long) (start + 1), fuzz_from_ch,
                         (long) (stop + 1), fuzz_to_ch,
                         (int) strand, (int) seq);
              }
            } else {
              sprintf (str, "%s%ld\t%s%ld\t%d\n",
                       fuzz_from_ch, (long) (start + 1),
                       fuzz_to_ch, (long) (stop + 1),
                       (int) seq);
            }
            vnp = ValNodeNew (head);
            if (head == NULL) {
              head = vnp;
            }
            if (vnp != NULL) {
              vnp->data.ptrvalue = StringSave (str);
            }
          }
        }
      }
      slp = next;
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
  SafeSetStatus (ipp->partial5, partial5);
  SafeSetStatus (ipp->partial3, partial3);

  SendMessageToDialog (tlp->dialog, VIB_MSG_RESET);
  tlp->vnp = head;
  SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
  for (j = 0, vnp = tlp->vnp; vnp != NULL; j++, vnp = vnp->next) {
  }
  tlp->max = MAX ((Int2) 0, (Int2) (j - tlp->rows + 1));
  CorrectBarMax (tlp->bar, tlp->max);
  CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
}

static Pointer IntervalPageToSeqLocPtr (DialoG d)

{
  BioseqPtr        bsp;
  Char             ch;
  SeqLocPtr        firstSlp;
  Int4             from;
  Boolean          fuzz_after;
  Boolean          fuzz_before;
  Int2             fuzz_from;
  Int2             fuzz_to;
  ValNodePtr       head;
  IntFuzzPtr       ifp;
  IntervalPagePtr  ipp;
  Boolean          isInterval;
  Boolean          isPoint;
  Int2             j;
  SeqLocPtr        lastSlp;
  Int2             len;
  Boolean          notFirst;
  Boolean          nullsBetween;
  Boolean          okay;
  Boolean          partial5;
  Boolean          partial3;
  CharPtr          ptr;
  SeqEntryPtr      sep;
  SeqFeatPtr       sfp;
  SeqIntPtr        sip;
  SeqLocPtr        slp;
  SeqPntPtr        spp;
  Int2             strand;
  TagListPtr       tlp;
  Int4             tmp;
  SeqLocPtr        tmploc1;
  SeqLocPtr        tmploc2;
  Int4             to;
  CharPtr          txt;
  Int4             val;
  Int2             val2;
  ValNodePtr       vnp;

  ipp = (IntervalPagePtr) GetObjectExtra (d);
  if (ipp == NULL) return NULL;
  tlp = GetObjectExtra (ipp->ivals);
  if (tlp == NULL) return NULL;

  slp = NULL;
  sfp = SeqFeatNew ();
  if (sfp != NULL) {
    nullsBetween = GetStatus (ipp->nullsBetween);
    partial5 = GetStatus (ipp->partial5);
    partial3 = GetStatus (ipp->partial3);
    notFirst = FALSE;
    head = tlp->vnp;
    for (vnp = tlp->vnp; vnp != NULL; vnp = vnp->next) {
      txt = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 0);
      okay = FALSE;
      isInterval = TRUE;
      isPoint = FALSE;
      len = StringLen (txt);
      for (j = 0; j < len; j++) {
        ch = txt [j];
        if (ch != ' ' && ch != '\t' && ch != '\n') {
          okay = TRUE;
        }
      }
      MemFree (txt);
      if (okay) {
        fuzz_from = -1;
        fuzz_to = -1;
        fuzz_before = FALSE;
        fuzz_after = FALSE;
        from = 0;
        txt = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 0);
        if (txt != NULL) {
          ptr = StringChr (txt, '<');
          if (ptr != NULL) {
            /*
            fuzz_to = 2;
            */
            *ptr = ' ';
          }
          ptr = StringChr (txt, '>');
          if (ptr != NULL) {
            /*
            fuzz_to = 1;
            */
            *ptr = ' ';
          }
          ptr = StringChr (txt, '^');
          if (ptr != NULL) {
            fuzz_after = TRUE;
            *ptr = ' ';
          }
          TrimSpacesAroundString (txt);
          if (StrToLong (txt, &val)) {
            from = val;
          } else {
            okay = FALSE;
          }
        } else {
          okay = FALSE;
        }
        MemFree (txt);
        to = 0;
        txt = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 1);
        if (! StringHasNoText (txt)) {
          ptr = StringChr (txt, '<');
          if (ptr != NULL) {
            /*
            fuzz_to = 2;
            */
            *ptr = ' ';
          }
          ptr = StringChr (txt, '>');
          if (ptr != NULL) {
            /*
            fuzz_to = 1;
            */
            *ptr = ' ';
          }
          ptr = StringChr (txt, '^');
          if (ptr != NULL) {
            fuzz_after = TRUE;
            *ptr = ' ';
          }
          TrimSpacesAroundString (txt);
          if (StrToLong (txt, &val)) {
            to = val;
            if (fuzz_after && to == from + 1) {
              isInterval = FALSE;
              isPoint = TRUE;
              /* from++; */ /* this was causing point to be thrown off */
            } else if (to == from && (! partial5) && (! partial3)) {
              isInterval = FALSE;
              isPoint = TRUE;
            }
          } else {
            okay = FALSE;
          }
        } else {
          /*
          okay = FALSE;
          */
          isInterval = FALSE;
          isPoint = TRUE;
          to = from;
        }
        MemFree (txt);
        strand = Seq_strand_unknown;
        if (ipp->nucsOK) {
          txt = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 2);
          if (txt != NULL && StrToInt (txt, &val2)) {
            strand = val2;
            if (strand > Seq_strand_both_rev) {
              strand = Seq_strand_other;
            }
          } else {
            /*
            okay = FALSE;
            */
          }
          MemFree (txt);
        }
        if (isInterval) {
          if (from > to) {
            tmp = from;
            from = to;
            to = tmp;
          }
        }
        bsp = NULL;
        txt = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, ipp->nucsOK ? 3 : 2);
        if (txt != NULL) {
          if (StrToInt (txt, &val2) && val2 > 0 && val2 <= ipp->count) {
            sep = ipp->bsptr [val2];
            if (sep != NULL && sep->choice == 1) {
              bsp = (BioseqPtr) sep->data.ptrvalue;
            } else {
              okay = FALSE;
            }
          } else {
            okay = FALSE;
          }
        }
        MemFree (txt);
        if (okay) {
          if (nullsBetween  && notFirst) {
            slp = ValNodeNew (NULL);
            if (slp != NULL) {
              slp->choice = SEQLOC_NULL;
              tmploc1 = sfp->location;
              if (tmploc1 != NULL) {
                if (tmploc1->choice == SEQLOC_MIX) {
                  tmploc2 = (ValNodePtr) (tmploc1->data.ptrvalue);
                  if (tmploc2 != NULL) {
                    while (tmploc2->next != NULL) {
                      tmploc2 = tmploc2->next;
                    }
                    tmploc2->next = slp;
                  }
                } else {
                  tmploc2 = ValNodeNew (NULL);
                  if (tmploc2 != NULL) {
                    tmploc2->choice = SEQLOC_MIX;
                    tmploc2->data.ptrvalue = (Pointer) tmploc1;
                    tmploc1->next = slp;
                    sfp->location = tmploc2;
                  }
                }
              }
            }
          }
          if (strand == Seq_strand_unknown) {
            strand = Seq_strand_plus;
          }
          if (isInterval) {
            AddIntToSeqFeat (sfp, from - 1, to - 1, bsp,
                             fuzz_from, fuzz_to, strand);
          } else if (isPoint) {
            AddSeqFeatPoint (sfp, bsp, from, fuzz_before, fuzz_after, strand);
          }
          notFirst = TRUE;
        }
      }
    }
    firstSlp = NULL;
    lastSlp = NULL;
    slp = SeqLocFindNext (sfp->location, NULL);
    while (slp != NULL) {
      if (firstSlp == NULL) {
        firstSlp = slp;
      }
      lastSlp = slp;
      slp = SeqLocFindNext (sfp->location, slp);
    }
    if (firstSlp != NULL && GetStatus (ipp->partial5)) {
      if (firstSlp->choice == SEQLOC_INT && firstSlp->data.ptrvalue != NULL) {
        sip = (SeqIntPtr) firstSlp->data.ptrvalue;
        ifp = IntFuzzNew ();
        if (ifp != NULL) {
          ifp->choice = 4;
          if (sip->strand == Seq_strand_minus || sip->strand == Seq_strand_both_rev) {
            sip->if_to = ifp;
            ifp->a = 1;
          } else {
            sip->if_from = ifp;
            ifp->a = 2;
          }
        }
      } else if (firstSlp->choice == SEQLOC_PNT && firstSlp->data.ptrvalue != NULL) {
        spp = (SeqPntPtr) firstSlp->data.ptrvalue;
        ifp = IntFuzzNew ();
        if (ifp != NULL) {
          ifp->choice = 4;
          if (spp->strand == Seq_strand_minus || spp->strand == Seq_strand_both_rev) {
            spp->fuzz = ifp;
            ifp->a = 1;
          } else {
            spp->fuzz = ifp;
            ifp->a = 2;
          }
        }
      }
    }
    if (lastSlp != NULL && GetStatus (ipp->partial3)) {
      if (lastSlp->choice == SEQLOC_INT && lastSlp->data.ptrvalue != NULL) {
        sip = (SeqIntPtr) lastSlp->data.ptrvalue;
        ifp = IntFuzzNew ();
        if (ifp != NULL) {
          ifp->choice = 4;
          if (sip->strand == Seq_strand_minus || sip->strand == Seq_strand_both_rev) {
            sip->if_from = ifp;
            ifp->a = 2;
          } else {
            sip->if_to = ifp;
            ifp->a = 1;
          }
        }
      } else if (lastSlp->choice == SEQLOC_PNT && lastSlp->data.ptrvalue != NULL) {
        spp = (SeqPntPtr) lastSlp->data.ptrvalue;
        ifp = IntFuzzNew ();
        if (ifp != NULL) {
          ifp->choice = 4;
          if (spp->strand == Seq_strand_minus || spp->strand == Seq_strand_both_rev) {
            spp->fuzz = ifp;
            ifp->a = 2;
          } else {
            spp->fuzz = ifp;
            ifp->a = 1;
          }
        }
      }
    }
    slp = sfp->location;
    sfp->location = NULL;
    SeqFeatFree (sfp);
  }
  return (Pointer) slp;
}

static void CleanupIntervalPage (GraphiC g, VoidPtr data)

{
  IntervalPagePtr  ipp;
  Int2             j;

  ipp = (IntervalPagePtr) data;
  if (ipp != NULL) {
    MemFree (ipp->bsptr);
    if (ipp->alist != NULL) {
      for (j = 0; j <= ipp->count + 1; j++) {
        MemFree (ipp->alist [j].name);
      }
    }
    MemFree (ipp->alist);
    MemFree (ipp->lengths);
  }
  MemFree (data);
}

Uint2 interval_types [] = {
  TAGLIST_TEXT, TAGLIST_TEXT, TAGLIST_POPUP, TAGLIST_POPUP
};

Uint2 interval_widths [] = {
  5, 5, 0, 0
};

static Boolean ReadSeqLocDialog (DialoG d, CharPtr filename)

{
  AsnIoPtr         aip;
  IntervalPagePtr  ipp;
  SeqLocPtr        slp;
  Char             path [PATH_MAX];

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  ipp = (IntervalPagePtr) GetObjectExtra (d);
  if (ipp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        slp = SeqLocAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (slp != NULL) {
          SeqLocPtrToIntervalPage (ipp->dialog, slp);
          slp = SeqLocFree (slp);
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean WriteSeqLocDialog (DialoG d, CharPtr filename)

{
  AsnIoPtr         aip;
  IntervalPagePtr  ipp;
  SeqLocPtr        slp;
  Char             path [PATH_MAX];
#ifdef WIN_MAC
  FILE            *f;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  ipp = (IntervalPagePtr) GetObjectExtra (d);
  if (ipp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      f = FileOpen (path, "r");
      if (f != NULL) {
        FileClose (f);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      aip = AsnIoOpen (path, "w");
      if (aip != NULL) {
        slp = IntervalPageToSeqLocPtr (ipp->dialog);
        SeqLocAsnWrite (slp, aip, NULL);
        AsnIoClose (aip);
        slp = SeqLocFree (slp);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static void IntervalEditorMessage (DialoG d, Int2 mssg)

{
  IntervalPagePtr  ipp;

  ipp = (IntervalPagePtr) GetObjectExtra (d);
  if (ipp != NULL) {
    if (mssg == VIB_MSG_INIT) {
      SeqLocPtrToIntervalPage (d, NULL);
    } else if (mssg == VIB_MSG_ENTER) {
      SendMessageToDialog (ipp->ivals, VIB_MSG_ENTER);
    } else if (mssg == VIB_MSG_RESET) {
    }
  }
}

extern void StdFeatIntEdPartialCallback (FeatureFormPtr ffp, Boolean partial5, Boolean partial3, Boolean order)

{
  if (ffp == NULL) return;
  SafeSetStatus (ffp->partial, (partial5 || partial3 || order));
  Update ();
}

static void ChangedPartialProc (ButtoN b)

{
  IntervalPagePtr  ipp;

  ipp = (IntervalPagePtr) GetObjectExtra (b);
  if (ipp == NULL) return;
  if (ipp->proc != NULL) {
    ipp->proc (ipp->ffp, GetStatus (ipp->partial5),
               GetStatus (ipp->partial3), GetStatus (ipp->nullsBetween));
  }
}

extern DialoG CreateTagListDialogEx (GrouP h, Uint2 rows, Uint2 cols,
                                     Int2 spacing, Uint2Ptr types,
                                     Uint2Ptr textWidths, EnumFieldAssocPtr PNTR alists,
                                     Boolean useBar, Boolean noExtend,
                                     ToDialogFunc tofunc, FromDialogFunc fromfunc);

extern DialoG CreateIntervalEditorDialogEx (GrouP h, CharPtr title, Uint2 rows,
                                            Int2 spacing, SeqEntryPtr sep,
                                            Boolean nucsOK, Boolean protsOK,
                                            Boolean useBar, Boolean showPartials,
                                            Boolean allowGaps, FeatureFormPtr ffp,
                                            IntEdPartialProc proc)

{
  BioseqPtr        bsp;
  Int4             count;
  GrouP            f;
  IntervalPagePtr  ipp;
  Int2             j;
  GrouP            m;
  GrouP            p;
  PrompT           p1;
  PrompT           p2;
  CharPtr          ptr;
  GrouP            q;
  GrouP            s;
  Boolean          showIdTags;
  SeqIdPtr         sip;
  Char             str [128];
  TagListPtr       tlp;

  p = HiddenGroup (h, 1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  ipp = (IntervalPagePtr) MemNew (sizeof (IntervalPage));
  if (ipp != NULL) {

    SetObjectExtra (p, ipp, CleanupIntervalPage);
    ipp->dialog = (DialoG) p;
    ipp->todialog = SeqLocPtrToIntervalPage;
    ipp->fromdialog = IntervalPageToSeqLocPtr;
    ipp->dialogmessage = IntervalEditorMessage;
    ipp->testdialog = NULL;
    ipp->importdialog = ReadSeqLocDialog;
    ipp->exportdialog = WriteSeqLocDialog;

    ipp->ffp = ffp;
    ipp->proc = proc;

    if (title != NULL && title [0] != '\0') {
      s = NormalGroup (p, 0, -2, title, systemFont, NULL);
    } else {
      s = HiddenGroup (p, 0, -2, NULL);
    }
    m = HiddenGroup (s, -1, 0, NULL);
    /*
    SetGroupSpacing (m, 10, 10);
    */

    ipp->nucsOK = nucsOK;
    ipp->protsOK = protsOK;
    ipp->showIdTags = FALSE;
    ipp->count = 0;

    if (sep != NULL) {
      count = SegmentedEntryCount (sep);
      count += 4;
      ipp->bsptr = MemNew (sizeof (BioseqPtr) * (size_t) count);
      ipp->alist = MemNew (sizeof (EnumFieldAssoc) * (size_t) count);
      ipp->lengths = MemNew (sizeof (Int4) * (size_t) count);
      ipp->count = 0;

      if (ipp->bsptr != NULL && ipp->alist != NULL && ipp->lengths != NULL) {
        SeqEntryExplore (sep, (Pointer) ipp, FillInProducts);
        j = 0;
        ipp->alist [j].name = StringSave ("     ");
        ipp->alist [j].value = (UIEnum) 0;
        for (j = 1; j <= ipp->count; j++) {
          sep = ipp->bsptr [j];
          if (sep != NULL && sep->choice == 1 && sep->data.ptrvalue != NULL) {
            bsp = (BioseqPtr) sep->data.ptrvalue;
            ipp->lengths [j] = bsp->length;
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
            ipp->alist [j].name = StringSave (ptr);
            ipp->alist [j].value = (UIEnum) j;
          }
        }
        j = ipp->count + 1;
        ipp->alist [j].name = NULL;
        ipp->alist [j].value = (UIEnum) 0;
#ifdef WIN_MOTIF
        if (ipp->count > 31) {
          if (nucsOK) {
            interval_types [3] = TAGLIST_LIST;
          } else {
            interval_types [2] = TAGLIST_LIST;
          }
        } else {
          interval_types [2] = TAGLIST_POPUP;
          interval_types [3] = TAGLIST_POPUP;
        }
#endif
      }

      if (ipp->alists != NULL) {
        ipp->alists [0] = NULL;
        ipp->alists [1] = NULL;
        if (nucsOK) {
          ipp->alists [2] = strand_alist;
          ipp->alists [3] = ipp->alist;
        } else {
          ipp->alists [2] = ipp->alist;
        }
      }
    } else {
      ipp->alist = MemNew (sizeof (EnumFieldAssoc) * (size_t) 4);
      if (ipp->alist != NULL) {
        j = 0;
        ipp->alist [j].name = StringSave ("     ");
        ipp->alist [j].value = (UIEnum) 0;
        j = 1;
        ipp->alist [j].name = NULL;
        ipp->alist [j].value = (UIEnum) 0;

        ipp->alists [0] = NULL;
        ipp->alists [1] = NULL;
        if (nucsOK) {
          ipp->alists [2] = strand_alist;
          ipp->alists [3] = ipp->alist;
        } else {
          ipp->alists [2] = ipp->alist;
        }
      }
    }

    q = NULL;
    if (showPartials) {
      q = HiddenGroup (m, 4, 0, NULL);
      SetGroupSpacing (q, 20, 2);
      if (nucsOK) {
        ipp->partial5 = CheckBox (q, "5' Partial", ChangedPartialProc);
      } else {
        ipp->partial5 = CheckBox (q, "NH2 Partial", ChangedPartialProc);
      }
      SetObjectExtra (ipp->partial5, ipp, NULL);
      if (nucsOK) {
        ipp->partial3 = CheckBox (q, "3' Partial", ChangedPartialProc);
      } else {
        ipp->partial3 = CheckBox (q, "CO2H Partial", ChangedPartialProc);
      }
      SetObjectExtra (ipp->partial3, ipp, NULL);
    }

    f = HiddenGroup (m, 4, 0, NULL);
    StaticPrompt (f, "From", 5 * stdCharWidth, 0, programFont, 'c');
    StaticPrompt (f, "To", 5 * stdCharWidth, 0, programFont, 'c');
    if (nucsOK) {
      p1 = StaticPrompt (f, "Strand", 0, 0, programFont, 'c');
    } else {
      p1 = NULL;
    }
    p2 = StaticPrompt (f, "SeqID", 0, 0, programFont, 'c');

    f = HiddenGroup (m, 0, 4, NULL);
    SetGroupSpacing (f, 0, 0);

    ipp->ivals = CreateTagListDialogEx (f, rows, nucsOK ? 4 : 3, spacing,
                                        interval_types, interval_widths, ipp->alists,
                                        useBar, FALSE, NULL, NULL);

    interval_types [2] = TAGLIST_POPUP;
    interval_types [3] = TAGLIST_POPUP;
    ipp->nullsBetween = CheckBox (m, "'order' (intersperse intervals with gaps)", ChangedPartialProc);
    SetObjectExtra (ipp->nullsBetween, ipp, NULL);
    if (GetAppProperty ("InternalNcbiSequin") != NULL) {
      allowGaps = TRUE;
    }
    if (! allowGaps) {
      Hide (ipp->nullsBetween);
    }

    AlignObjects (ALIGN_CENTER, (HANDLE) ipp->ivals,
                  (HANDLE) q, (HANDLE) ipp->nullsBetween, NULL);
    tlp = (TagListPtr) GetObjectExtra (ipp->ivals);
    if (tlp != NULL) {
      if (nucsOK) {
        AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control [2], (HANDLE) p1, NULL);
      }
      AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control [nucsOK ? 3 : 2], (HANDLE) p2, NULL);
    }
  }

  return (DialoG) p;
}

extern DialoG CreateIntervalEditorDialog (GrouP h, CharPtr title, Uint2 rows,
                                          Int2 spacing, SeqEntryPtr sep,
                                          Boolean nucsOK, Boolean protsOK)

{
  return CreateIntervalEditorDialogEx (h, title, rows, spacing, sep,
                                       nucsOK, protsOK, TRUE, TRUE, FALSE,
                                       NULL, NULL);
}

static void ValNodePtrToVisStringDialog (DialoG d, Pointer data)

{
  ValNodePtr   head;
  Int2         j;
  ValNodePtr   list;
  CharPtr      str;
  TagListPtr   tlp;
  ValNodePtr   vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  list = (ValNodePtr) data;
  if (tlp != NULL) {
    head = NULL;
    while (list != NULL) {
      vnp = ValNodeNew (head);
      if (head == NULL) {
        head = vnp;
      }
      if (vnp != NULL) {
        str = MemNew (StringLen ((CharPtr) list->data.ptrvalue) + 3);
        if (str != NULL) {
          StringCpy (str, (CharPtr) list->data.ptrvalue);
          StringCat (str, "\n");
        }
        vnp->data.ptrvalue = str;
      }
      list = list->next;
    }
    SendMessageToDialog (tlp->dialog, VIB_MSG_RESET);
    tlp->vnp = head;
    SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
    for (j = 0, vnp = tlp->vnp; vnp != NULL; j++, vnp = vnp->next) {
    }
    tlp->max = MAX ((Int2) 0, (Int2) (j - tlp->rows + 1));
    CorrectBarMax (tlp->bar, tlp->max);
    CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
  }
}

static Pointer VisStringDialogToValNodePtr (DialoG d)

{
  Char         ch;
  ValNodePtr   head;
  Int2         j;
  Int2         len;
  ValNodePtr   list;
  Boolean      okay;
  CharPtr      str;
  TagListPtr   tlp;
  ValNodePtr   vnp;

  head = NULL;
  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp != NULL && tlp->vnp != NULL) {
    list = NULL;
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
        list = ValNodeNew (list);
        if (head == NULL) {
          head = list;
        }
        if (list != NULL) {
          list->choice = 0;
          list->data.ptrvalue = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 0);
        }
      }
    }
  }
  return (Pointer) head;
}

static void DbtagPtrToDbtagDialog (DialoG d, Pointer data, Boolean readOnly)

{
  DbtagPtr     dp;
  ValNodePtr   head;
  Int2         j;
  size_t       len;
  ValNodePtr   list;
  ObjectIdPtr  oid;
  CharPtr      ptr;
  CharPtr      str;
  TagListPtr   tlp;
  Char         tmp [16];
  ValNodePtr   vnp;

  tlp = (TagListPtr) GetObjectExtra (d);
  list = (ValNodePtr) data;
  if (tlp != NULL) {
    head = NULL;
    while (list != NULL) {
      vnp = ValNodeNew (head);
      if (head == NULL) {
        head = vnp;
      }
      if (vnp != NULL) {
        dp = (DbtagPtr) list->data.ptrvalue;
        if (dp != NULL && dp->db != NULL && dp->tag != NULL) {
          oid = dp->tag;
          ptr = NULL;
          if (oid->str != NULL) {
            ptr = oid->str;
          } else {
            sprintf (tmp, "%ld", (long) oid->id);
            ptr = tmp;
          }
          len = StringLen (dp->db) + StringLen (ptr);
          str = MemNew (len + 4);
          if (str != NULL) {
            StringCpy (str, dp->db);
            StringCat (str, "\t");
            StringCat (str, ptr);
            StringCat (str, "\n");
          }
          vnp->data.ptrvalue = str;
        }
      }
      list = list->next;
    }
    SendMessageToDialog (tlp->dialog, VIB_MSG_RESET);
    tlp->vnp = head;
    SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
    for (j = 0, vnp = tlp->vnp; vnp != NULL; j++, vnp = vnp->next) {
    }
    if (readOnly) {
      tlp->max = MAX ((Int2) 0, (Int2) (j - tlp->rows));
      CorrectBarMax (tlp->bar, tlp->max);
      CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
      if (tlp->max > 0) {
        SafeShow (tlp->bar);
      } else {
        SafeHide (tlp->bar);
      }
    } else {
      tlp->max = MAX ((Int2) 0, (Int2) (j - tlp->rows + 1));
      CorrectBarMax (tlp->bar, tlp->max);
      CorrectBarPage (tlp->bar, tlp->rows - 1, tlp->rows - 1);
    }
  }
}

static void DbtagPtrToRODbtagDialog (DialoG d, Pointer data)

{
  DbtagPtrToDbtagDialog (d, data, TRUE);
}

static void DbtagPtrToRWDbtagDialog (DialoG d, Pointer data)

{
  DbtagPtrToDbtagDialog (d, data, FALSE);
}

static Pointer DbtagDialogToDbtagPtr (DialoG d)

{
  Boolean      alldigits = TRUE;
  Char         ch;
  DbtagPtr     dp;
  ValNodePtr   head;
  Int2         j;
  Int2         len;
  ValNodePtr   list;
  ObjectIdPtr  oid;
  Boolean      okay;
  CharPtr      ptr;
  CharPtr      str;
  TagListPtr   tlp;
  CharPtr      tmp;
  long int     val;
  ValNodePtr   vnp;

  head = NULL;
  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp != NULL && tlp->vnp != NULL) {
    list = NULL;
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
        list = ValNodeNew (list);
        if (head == NULL) {
          head = list;
        }
        if (list != NULL) {
          list->choice = 0;
          dp = DbtagNew ();
          list->data.ptrvalue = (Pointer) dp;
          if (dp != NULL) {
            dp->db = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 0);
            oid = ObjectIdNew ();
            dp->tag = oid;
            if (oid != NULL) {
              tmp = ExtractTagListColumn ((CharPtr) vnp->data.ptrvalue, 1);
              TrimSpacesAroundString (tmp);
              if (tmp != NULL) {
                ptr = tmp;
                ch = *ptr;
                while (ch != '\0') {
                  if (ch == ' ' || ch == '+' || ch == '-') {
                  } else if (! (IS_DIGIT (ch))) {
                    alldigits = FALSE;
                  }
                  ptr++;
                  ch = *ptr;
                }
                if (alldigits && sscanf (tmp, "%ld", &val) == 1) {
                  oid->id = (Int4) val;
                  MemFree (tmp);
                } else {
                  oid->str = tmp;
                }
              } else {
                oid->str = StringSave ("?");
              }
            }
          }
        }
      }
    }
  }
  return (Pointer) head;
}

Uint2 visstring_types [] = {
  TAGLIST_TEXT, TAGLIST_TEXT
};

Uint2 readonlystring_types [] = {
  TAGLIST_PROMPT, TAGLIST_PROMPT
};

Uint2 visstring_widths [] = {
  0, 0, 0, 0
};

extern DialoG CreateVisibleStringDialog (GrouP h, Uint2 rows,
                                         Int2 spacing, Int2 width)

{
  visstring_widths [0] = width;
  return CreateTagListDialog (h, rows, 1, spacing,
                              visstring_types, visstring_widths, NULL,
                              ValNodePtrToVisStringDialog,
                              VisStringDialogToValNodePtr);
}

extern DialoG CreateDbtagDialog (GrouP h, Uint2 rows, Int2 spacing,
                                 Int2 width1, Int2 width2)

{
  DialoG      d;
  TagListPtr  tlp;

  visstring_widths [0] = width1;
  visstring_widths [1] = width2;
  if (GetAppProperty ("ReadOnlyDbTags") == NULL) {
    return CreateTagListDialog (h, rows, 2, spacing,
                                visstring_types, visstring_widths, NULL,
                                DbtagPtrToRWDbtagDialog,
                                DbtagDialogToDbtagPtr);
  } else {
    d = CreateTagListDialog (h, rows, 2, spacing,
                             readonlystring_types, visstring_widths, NULL,
                             DbtagPtrToRODbtagDialog,
                             DbtagDialogToDbtagPtr);
    tlp = (TagListPtr) GetObjectExtra (d);
    if (tlp != NULL) {
      Hide (tlp->bar);
    }
    return d;
  }
}

