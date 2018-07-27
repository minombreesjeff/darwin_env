/*   gphview.c
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
* File Name:  gphview.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   2/5/97
*
* $Revision: 6.45 $
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
#include <fstyle.h>
#include <txalign.h>
#include <drawseq.h>
#include <drawingp.h>
#include <viewerp.h>
#include <mapgene.h>
#include <saledit.h>
#include <vsmpriv.h>
#include <edutil.h>
#include <gphdraw.h>

extern SegmenT VSMEntityDraw PROTO((ObjMgrDataPtr omdp, VSMPictPtr vsmpp, VSeqMgrPtr vsmp));

static void InitSelectGraphical (BioseqViewPtr bvp);
static void InitSelectDesktop (BioseqViewPtr bvp);
static void DrawSelectionRange (VieweR viewer, SegmenT segment);
static void PopulateDesktop (BioseqViewPtr bvp);

#define MAXZOOMSCALEVAL 22

static Int4  zoomScaleVal [MAXZOOMSCALEVAL] = {
  1L, 1L, 2L, 5L, 10L, 20L, 50L, 100L, 200L, 500L,
  1000L, 2000L, 5000L, 10000L, 20000L, 50000L,
  100000L, 200000L, 500000L, 1000000L, 2000000L, 5000000L
};

static Boolean Find_segment_IDs (VieweR viewer, PoinT pt, Uint2Ptr entityID,
                                 Uint2Ptr itemID, Uint2Ptr itemType)			
{
  SegmenT seg, pseg;
  Uint2 pID, sID, primID, primCt;

  if (entityID == NULL || itemID == NULL ||itemType == NULL) return FALSE;

  seg = FindSegment (viewer, pt, &sID, &primID, &primCt);
  if (seg == NULL || sID == 0 || primID == 0) return FALSE;
  pseg = ParentSegment (seg);
  if (pseg == NULL) return FALSE;
  pID = SegmentID (pseg);
  if (pID == 0) return FALSE;
		
  *itemType = pID;
  *entityID = sID;
  *itemID = primID;
  return TRUE;
}

static void get_viewer_position (VieweR viewer, Int4Ptr x, Int4Ptr y)

{
  Int4 scale_x, scale_y;
  BoxInfo port, world;
  RecT v_iew;

  ViewerBox (viewer, &world, &port, &v_iew, &scale_x, &scale_y);
  *x = (port.left + port.right) / 2;
  *y = (port.top + port.bottom) / 2;
}

typedef struct makemultidata {
  ValNodePtr  slp_list;
  Int4        len;
} MakeMultiData, PNTR MakeMultiPtr;

static void MakeMultiSlpList (SeqEntryPtr sep, Pointer mydata,
                              Int4 index, Int2 indent)

{
  BioseqPtr     bsp;
  Int4          len;
  MakeMultiPtr  mmp;
  SeqLocPtr     slp;

  if (sep != NULL && sep->choice == 1 && mydata != NULL) {
    mmp = (MakeMultiPtr) mydata;
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp != NULL && ISA_na (bsp->mol)) {
      len = bsp->length;
      slp = SeqLocIntNew (0, len-1, Seq_strand_plus, SeqIdFindBest (bsp->id, 0));
      ValNodeAddPointer (&(mmp->slp_list), 0, (Pointer) slp);
      mmp->len = MAX (mmp->len, len);
    }
  }
}

static void FindAlignment (SeqEntryPtr sep, Pointer mydata,
                           Int4 index, Int2 indent)

{
  SeqAlignPtr   align;
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqAnnotPtr   sap;
  SeqAlignPtr   PNTR  sapp;

  if (sep != NULL && sep->data.ptrvalue != NULL && mydata != NULL) {
    sapp = (SeqAlignPtr PNTR) mydata;
    sap = NULL;
    if (IS_Bioseq (sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      sap = bsp->annot;
    } else if (IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      sap = bssp->annot;
    } else {
      return;
    }
    while (sap != NULL) {
      if (sap->type == 2) {
        align = sap->data;
        if (align != NULL && *sapp == NULL) {
          *sapp = align;
        }
      }
      sap = sap->next;
    }
  }
}

static void CommonFrameProc (VieweR vwr, BioseqViewPtr bvp)

{
  RecT  dr;
  RecT  or;
  RecT  r;

  if (vwr == NULL || bvp == NULL) return;
  Dotted ();
  ObjectRect (vwr, &or);
  InsetRect (&or, 2, 2);
  LoadRect (&r, bvp->pnt_start.x, bvp->pnt_start.y,
            bvp->pnt_stop.x, bvp->pnt_stop.y);
  SectRect (&r, &or, &dr);
  FrameRect (&dr);
}

static void ClickMap (VieweR vwr, SegmenT seg, PoinT pt)

{
  BioseqViewPtr  bvp;

  bvp = (BioseqViewPtr) GetObjectExtra (vwr);
  if (bvp == NULL) return;
  bvp->wasDoubleClick = dblClick;
  bvp->wasShiftKey = shftKey;
  if (bvp->old_rect_shown) {
    InvertMode ();
    CommonFrameProc (vwr, bvp);
  }
  bvp->old_rect_shown = FALSE;
  bvp->pnt_start = pt;
  bvp->pnt_stop = pt;
  InvertMode ();
  CommonFrameProc (vwr, bvp);
}

static void DragMap (VieweR vwr, SegmenT seg, PoinT pt)

{
  BioseqViewPtr  bvp;

  bvp = (BioseqViewPtr) GetObjectExtra (vwr);
  if (bvp == NULL) return;
  if(bvp->wasDoubleClick || bvp->wasShiftKey) return;
  InvertMode ();
  CommonFrameProc (vwr, bvp);
  bvp->pnt_stop = pt;
  CommonFrameProc (vwr, bvp);
}

static void ReleaseMap (VieweR vwr, SegmenT seg, PoinT pt)

{
  BioseqViewPtr  bvp;
  Uint2          entityID;
  Uint2          itemID;
  Uint2          itemtype;
  SeqEntryPtr    sep;
  PntInfo        start_info, stop_info;

  bvp = (BioseqViewPtr) GetObjectExtra (vwr);
  if (bvp == NULL) return;

  if (bvp->wasDoubleClick) {
    if (Find_segment_IDs (vwr, pt, &entityID, &itemID, &itemtype)) {
      sep = GetTopSeqEntryForEntityID (entityID);
      if (bvp->launchSubviewers) {
        WatchCursor ();
        Update ();
        LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
        ArrowCursor ();
        Update ();
      } else if (LaunchViewerNotEditor (bvp, sep, entityID, itemID, itemtype)) {
        WatchCursor ();
        Update ();
        LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
        ArrowCursor ();
        Update ();
      }
      return;
    }
    if (! bvp->sendSelectMessages) return;
    if (bvp->wasShiftKey) {
      ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
    } else {
      ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
    }
  } else if (bvp->sendSelectMessages) {
    if (Find_segment_IDs (vwr, pt, &entityID, &itemID, &itemtype)) {
      if (bvp->wasShiftKey) {
        ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
      } else {
        ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
      }
    } else if (! bvp->wasShiftKey) {
      ObjMgrDeSelect (0, 0, 0, 0, NULL);
    }
  }
  if (bvp->gdraw_p != NULL) {
    bvp->old_rect_shown = TRUE;
    if (bvp->slp_list != NULL) {
      free_slp_list (bvp->slp_list);
    }
    bvp->slp_list = NULL;
    if (bvp->anp_node != NULL) {
      FreeAlignNode (bvp->anp_node);
    }
    bvp->anp_node = NULL;
    bvp->scaleNotCalculated = TRUE;
    MapViewerToWorld(bvp->vwr, bvp->pnt_start, &start_info);
    MapViewerToWorld(bvp->vwr, bvp->pnt_stop, &stop_info);
    bvp->slp_list = find_map_pos (bvp->gdraw_p, start_info, stop_info);
  }
}

static void PopulateMap (BioseqViewPtr bvp)

{
  Uint2        entityID;
  Int2         oldstyle;
  RecT         r;
  SeqEntryPtr  sep;
  Int2         style;

  if (bvp == NULL) return;
  WatchCursor ();
  oldstyle = GetMuskCurrentSt ();
  style = GetValue (bvp->style) - 1;
  SetMuskCurrentSt (GetMuskStyleName (style));
  ObjectRect (bvp->vwr, &r);
  InsetRect (&r, 4, 4);
  bvp->pict = DeletePicture (bvp->pict);
  entityID = ObjMgrGetEntityIDForPointer (bvp->bsp);
  sep = GetTopSeqEntryForEntityID (entityID);
  if (bvp->gdraw_p == NULL) {
    bvp->pict = SequinGlobalPicture (sep, r.right - r.left, r.bottom - r.top,
                                     bvp->g_list, &(bvp->gdraw_p), FALSE);
  } else {
    bvp->pict = GlobalPictureUpdate (bvp->gdraw_p, bvp->g_list,
                                     r.right - r.left, r.bottom - r.top);
  }
  if (bvp->gdraw_p != NULL && bvp->pict != NULL) {
    if (bvp->gdraw_p->is_circle) {
      AttachPicture (bvp->vwr, bvp->pict, 0, 0, UPPER_LEFT, 1, 1, NULL);
    } else {
      AttachPicture (bvp->vwr, bvp->pict, INT4_MIN, 0, UPPER_LEFT, 1, 1, NULL);
    }
    SetViewerProcs (bvp->vwr, ClickMap, DragMap, ReleaseMap, NULL);
  }
  SetMuskCurrentSt (GetMuskStyleName (oldstyle));
  ArrowCursor ();
}

static void ClickGraphic (VieweR vwr, SegmenT seg, PoinT pt)

{
  BioseqViewPtr  bvp;

  bvp = (BioseqViewPtr) GetObjectExtra (vwr);
  if (bvp == NULL) return;
  bvp->wasDoubleClick = dblClick;
  bvp->wasShiftKey = shftKey;
  bvp->old_rect_shown = FALSE;
  bvp->pnt_start = pt;
  bvp->pnt_stop = pt;
}

static void ReleaseGraphic (VieweR vwr, SegmenT seg, PoinT pt)

{
  BioseqViewPtr  bvp;
  Uint2          entityID;
  Uint2          itemID;
  Uint2          itemtype;
  SeqEntryPtr    sep;

  bvp = (BioseqViewPtr) GetObjectExtra (vwr);
  if (bvp == NULL) return;

  if (bvp->wasDoubleClick) {
    if (Find_segment_IDs (vwr, pt, &entityID, &itemID, &itemtype)) {
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
        GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID,
                          itemtype, 0, 0, itemtype, 0);
        ArrowCursor ();
        Update ();
        return;
      } else {
        return;
      }
    }
    if (! bvp->sendSelectMessages) return;
    if (bvp->wasShiftKey) {
      ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
    } else {
      ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
    }
  } else if (bvp->sendSelectMessages) {
    if (Find_segment_IDs (vwr, pt, &entityID, &itemID, &itemtype)) {
      if (bvp->wasShiftKey) {
        ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
      } else {
        ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
      }
    } else if (! bvp->wasShiftKey) {
      ObjMgrDeSelect (0, 0, 0, 0, NULL);
    }
  }
}

static SegmenT DrawCompressNoAlignment (SeqLocPtr m_loc, ValNodePtr PNTR anp_list,
                                        Uint2Ptr seq_entityID, Boolean flat_insert,
                                        Int4 width, Int4Ptr p_scale, Int4Ptr p_maxwidth)

{
	Int2 styleNum;
	CharPtr old_style;
	Int4 scale;
	SegmenT pic;
	ValNode vn;
	Int4 max_width;
	Int4 y_pos;
	ValNodePtr anp_node;
	Int4 len;
	Uint2 entityID;


	anp_node = collect_alignnode_from_slp(m_loc, &entityID, flat_insert);
	if(anp_node == NULL)
		return NULL;

	vn.data.ptrvalue = m_loc;
	vn.next = NULL;
	max_width = CountMaxSeqLabel(&vn);
	scale = FigureMaxScale(&vn, (Int2)width, max_width);
	if(scale <=0)
		return NULL;

	pic = CreatePicture();
	len = slp_list_len(m_loc);

	/*set up the sepcial style */
	styleNum = GetMuskCurrentSt();
	old_style = GetMuskStyleName(styleNum);
	SetMuskCurrentSt("StyleX");

	y_pos = 0;
	DrawMPAlignment(anp_node, 0, len-1, m_loc, entityID, scale, &y_pos, MSM_MPAIR, FALSE, pic);

	*p_scale = scale;
	*p_maxwidth = max_width;
	SetMuskCurrentSt(old_style);

	if(seq_entityID != NULL)
		*seq_entityID = entityID;
	if(anp_list != NULL)
		*anp_list = anp_node;
	else
		FreeAlignNode(anp_node);
	return pic;
}

static void PopulateCompressedNoAlignment (BioseqViewPtr bvp)

{
  ValNodePtr  anp_list;
  BioseqPtr   bsp;
  Uint2       entityID;
  SeqLocPtr   m_loc;
  Int4        max_width;
  RecT        r;
  Int4        scale;
  Int4        X_VIEW;

  if (bvp == NULL) return;
  bsp = bvp->bsp;
  if (bsp == NULL) return;
  m_loc = SeqLocIntNew (0, bsp->length - 1, 0, bsp->id);
  ObjectRect (bvp->vwr, &r);
  InsetRect (&r, 4, 4);
  X_VIEW = r.right - r.left;
  bvp->pict = DrawCompressNoAlignment (m_loc, &(anp_list), &(entityID), FALSE,
                                       X_VIEW, &scale, &max_width);
  if (bvp->pict != NULL) {
	max_width += 2;
	max_width *= scale;
    AttachPicture (bvp->vwr, bvp->pict, INT4_MIN, 0, UPPER_LEFT, scale, 1, NULL);
    SetViewerProcs (bvp->vwr, ClickGraphic, NULL, ReleaseGraphic, NULL);
  }
  FreeAlignNode (anp_list);
  SeqLocFree (m_loc);
  ArrowCursor ();
}

static void PopulateCompressed (BioseqViewPtr bvp)

{
  ValNodePtr   anp_list;
  BioseqPtr    bsp;
  Uint2        entityID;
  SeqLocPtr    m_loc;
  Int4         max_width;
  RecT         r;
  Int4         scale;
  SeqEntryPtr  sep;
  Int4         X_VIEW;

  if (bvp == NULL) return;
  WatchCursor ();
  Reset (bvp->vwr);
  bvp->pict = DeletePicture (bvp->pict);
  bsp = bvp->bsp;
  if (bsp == NULL) return;
  sep = SeqMgrGetSeqEntryForData (bsp);
  entityID = ObjMgrGetEntityIDForChoice (sep);
  if (! SeqEntryHasAligns (entityID, sep)) {
    PopulateCompressedNoAlignment (bvp);
    return;
  }
  anp_list = MakeCompressAlignList (bvp->bsp, NULL, &entityID);
  if (anp_list == NULL) {
    PopulateCompressedNoAlignment (bvp);
    return;
  }
  m_loc = SeqLocIntNew (0, bsp->length - 1, 0, bsp->id);
  ObjectRect (bvp->vwr, &r);
  InsetRect (&r, 4, 4);
  X_VIEW = r.right - r.left;
  bvp->pict = DrawCompressAlignment (anp_list, m_loc, X_VIEW, &scale,
                                     &max_width, entityID, NULL);
  if (bvp->pict != NULL) {
	max_width += 2;
	max_width *= scale;
    AttachPicture (bvp->vwr, bvp->pict, INT4_MIN, 0, UPPER_LEFT, scale, 1, NULL);
    SetViewerProcs (bvp->vwr, ClickGraphic, NULL, ReleaseGraphic, NULL);
  }
  FreeAlignNode (anp_list);
  SeqLocFree (m_loc);
  ArrowCursor ();
}

typedef struct gphfinddata {
  ValNodePtr  vnp;
  SeqLocPtr   slp;
  Int4        start;
  Int4        stop;
  BioseqPtr   bsp;
} GphFindData, PNTR GphFindPtr;

static Boolean GraphFindFunc (GatherContextPtr gcp)

{
  GphFindPtr   gfp;
  SeqGraphPtr  sgp;
  Int4         left;
  Int4         right;

  if (gcp == NULL) return TRUE;

  gfp = (GphFindPtr) gcp->userdata;
  if (gfp == NULL ) return TRUE;

  if (gcp->thistype == OBJ_SEQGRAPH) {
    sgp = (SeqGraphPtr) gcp->thisitem;
    if (sgp != NULL) {
      if (SeqIdForSameBioseq (SeqLocId (sgp->loc), SeqLocId (gfp->slp))) {
        left = GetOffsetInBioseq (sgp->loc, gfp->bsp, SEQLOC_LEFT_END);
        right = GetOffsetInBioseq (sgp->loc, gfp->bsp, SEQLOC_RIGHT_END);
        if (left == -1 || right == -1) return TRUE;
        if (left >= gfp->start && right <= gfp->stop) {
          ValNodeAddPointer (&(gfp->vnp), 0, (Pointer) sgp);
        }
      }
    }
  }
  return TRUE;
}

static ValNodePtr CollectGraphs (Uint2 entityID, BioseqPtr bsp, Int4 start, Int4 stop)

{
  GphFindData  gfd;
  GatherScope  gs;
  ValNode      vn;

  gfd.vnp = NULL;
  vn.choice = SEQLOC_WHOLE;
  vn.data.ptrvalue = (Pointer) bsp->id;
  gfd.slp = &vn;
  gfd.start = start;
  gfd.stop = stop;
  gfd.bsp = bsp;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQGRAPH] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  GatherEntity (entityID, (Pointer) &gfd, GraphFindFunc, &gs);
  return gfd.vnp;
}

static void PopulateSimpleGraphic (BioseqViewPtr bvp)

{
  BoxInfo        box;
  BioseqPtr      bsp;
  Uint2          entityID;
  GeneDataPtr    gdata;
  ValNodePtr     graphs;
  Int2           i;
  Int4           labelWid;
  Int4           len;
  Int4           max;
  Int4           min;
  MakeMultiData  mmd;
  SeqEntryPtr    oldscope = NULL;
  Int2           oldstyle;
  RecT           r;
  Int4           scaleX;
  Int4           scaleY;
  SeqEntryPtr    sep;
  SeqGraphPtr    sgp;
  ValNodePtr     slp_list;
  SeqLocPtr      slp;
  Char           str [32];
  Int2           style;
  ValNodePtr     vnp;
  Int4           vwr_x;
  Int4           vwr_y;
  Int2           vwr_align;

  if (bvp == NULL) return;
  WatchCursor ();
  vwr_x = INT4_MIN;
  vwr_y = INT4_MAX;
  vwr_align = UPPER_LEFT;
  if (bvp->moveToOldPos) {
    if (bvp->pict != NULL) {
      get_viewer_position (bvp->vwr, &(vwr_x), &(vwr_y));
      vwr_align = MIDDLE_CENTER;
    }
    bvp->moveToOldPos = FALSE;
  }
  Reset (bvp->vwr);
  bvp->pict = DeletePicture (bvp->pict);
  Update ();
  if (bvp->isGenome && bvp->slp_list == NULL) return;
  bsp = bvp->bsp;
  if (bsp != NULL) {
    scaleY = 1;
    len = 0;
    if (! bvp->isGenome) {
      slp_list = NULL;
      sep = NULL;
      len = 0;
      if (bvp->viewWholeEntity) {
        sep = SeqMgrGetSeqEntryForData (bsp);
        entityID = ObjMgrGetEntityIDForChoice (sep);
        sep = GetTopSeqEntryForEntityID (entityID);
        mmd.slp_list = NULL;
        mmd.len = 0;
        BioseqExplore (sep, (Pointer) &mmd, MakeMultiSlpList);
        slp_list = mmd.slp_list;
        len = mmd.len;
      } else {
        sep = SeqMgrGetSeqEntryForData (bsp);
        len = bsp->length;
        slp = SeqLocIntNew (0, len-1, Seq_strand_plus, SeqIdFindBest (bsp->id, 0));
        ValNodeAddPointer (&slp_list, 0, (Pointer) slp);
      }
      if (bvp->slp_list != NULL) {
        free_slp_list (bvp->slp_list);
        bvp->slp_list = NULL;
      }
      bvp->slp_list = slp_list;
    }
    oldstyle = GetMuskCurrentSt ();
    style = GetValue (bvp->style) - 1;
    SetMuskCurrentSt (GetMuskStyleName (style));

    if (bvp->scaleNotCalculated) {
      SafeHide (bvp->scale);
      Reset (bvp->scale);
      ObjectRect (bvp->vwr, &r);
      InsetRect (&r, 4, 4);
      labelWid = CountMaxSeqLabel (bvp->slp_list) + 2;
      if (r.right - r.left < 2 * labelWid) {
        labelWid = 0;
      }
      if (bvp->isGenome) {
        slp = (SeqLocPtr) bvp->slp_list->data.ptrvalue;
        len = slp_list_len (slp);
      }
      max = (Int4) MAX (len / (Int4) (r.right - r.left - labelWid), 1) + 2;
      max = (Int4) MIN (max, 50000L);
      min = (Int4) MAX (len / 320000L, 1L);
      bvp->maxScale = max;
      i = 1;
      while (i < MAXZOOMSCALEVAL && min > zoomScaleVal [i]) {
        i++;
      }
      bvp->minIndex = i;
      while (i < MAXZOOMSCALEVAL && max > zoomScaleVal [i - 1]) {
        sprintf (str, "%ld", (long) (zoomScaleVal [i]));
        if (max < zoomScaleVal [i]) {
          sprintf (str, "%ld", (long) zoomScaleVal [i]);
        }
        PopupItem (bvp->scale, str);
        i++;
      }
      SetValue (bvp->scale, MAX (i - bvp->minIndex, 1));
      bvp->scaleNotCalculated = FALSE;
    }
    SafeShow (bvp->scale);
    i = GetValue (bvp->scale) - 1 + bvp->minIndex;
    if (i < MAXZOOMSCALEVAL && i > 0) {
      scaleX = MIN (zoomScaleVal [i], bvp->maxScale);
    } else {
      scaleX = bvp->minIndex;
    }

    sep = SeqMgrGetSeqEntryForData (bsp);
    entityID = ObjMgrGetEntityIDForChoice (sep);
    if (entityID > 0) {
      sep = GetBestTopParentForData (entityID, bsp);
      oldscope = SeqEntrySetScope (sep);
    }

    gdata = make_gene_data (bvp->g_list);
    bvp->ftype_list = ValNodeFree (bvp->ftype_list);
    bvp->pict = DrawSequinMapEx (bvp->slp_list, sep, scaleX, gdata,
                                 &(bvp->ftype_list), FALSE, bvp->tempResultList);
    GeneDataFree (gdata);

    graphs = CollectGraphs (entityID, bsp, 0, bsp->length - 1);
    if (graphs != NULL) {
      SegmentBox (bvp->pict, &box);
      for (vnp = graphs; vnp != NULL; vnp = vnp->next) {
        sgp = (SeqGraphPtr) vnp->data.ptrvalue;
        AddGraphSentinelToPicture (sgp, bsp, bvp->pict, scaleX,
                                   box.bottom - 10, 0, NULL);
      }
    }

    if (entityID > 0) {
      SeqEntrySetScope (oldscope);
    }

    AttachPicture (bvp->vwr, bvp->pict, vwr_x, vwr_y, vwr_align, scaleX, scaleY,
                   bvp->highlightSelections ? DrawSelectionRange : NULL);
    SetViewerProcs (bvp->vwr, ClickGraphic, NULL, ReleaseGraphic, NULL);
    SetMuskCurrentSt (GetMuskStyleName (oldstyle));
    InitSelectGraphical (bvp);
  }
  ArrowCursor ();
}

/* functions copied from glbpic.c */

static  Int4 get_min_size(Int4 seq_len)
{
	if(seq_len <= 350000)
		return seq_len;

	return MAX(350000, seq_len/8);
}
	
static SeqLocPtr load_seq_loc(Int4 val_1, Int4 val_2, Int4 seq_len, Boolean has_zero, SeqIdPtr sip, BoolPtr show_msg, Boolean add_interval)
{
  Int4 m_start, m_stop;
  Uint1 m_strand = Seq_strand_plus;
  SeqLocPtr slp = NULL, loc;
  Int4 min_len;
  Int4 temp;
  Int4 len;

	min_len = get_min_size(seq_len);

	
	if(has_zero)
	{
		if(val_1 < val_2)
		{
			temp = val_2;
			val_2 = val_1;
			val_1 = temp;
		}
		if(add_interval)
		{
			val_1 -= seq_len/20;
			val_2 += seq_len/20;
		}
		len = (seq_len - 1 - val_1 +1 + val_2 +1);
		if(len > min_len)
		{
			if(*show_msg)
				Message(MSG_OK, "The selected region is too large. Reduced to %ld base pairs", min_len);
			val_1 = seq_len - 1 - min_len/2;
			val_2= min_len/2 -1;
		}
		m_start = val_1;
		m_stop = seq_len -1;
		loc = SeqLocIntNew(m_start, m_stop, m_strand, sip);
		m_start = 0;
		m_stop = val_2;
		loc->next = SeqLocIntNew(m_start, m_stop, m_strand, sip);
		slp = loc;
	}
	else
	{
		m_start = MIN(val_1, val_2);
		m_start = MAX(0, m_start);

		m_stop = MAX(val_1, val_2);
		m_stop = MIN(seq_len-1, m_stop);
		len = m_stop - m_start + 1;
		if(add_interval)
		{
			len += seq_len/10;
			m_start = MAX(0, (m_start - seq_len/20));
			m_stop = m_start + len -1;
		}

		/* if(len > min_len)
		{
			if(*show_msg)
				Message(MSG_OK, "The selected region is too large. Reduced to %ld base pairs", min_len);
			center = (m_start + m_stop)/2;
			m_start = center - min_len/2;
			m_stop = m_start + min_len -1;
		} */
		slp = SeqLocIntNew(m_start, m_stop, m_strand, sip);
	}
	*show_msg = FALSE;
	return slp;
}

typedef struct sentineldata {
  BioseqViewPtr  bvp;
  SegmenT        seg;
  PrimitivE      snt;
  Int4           start;
  Int4           stop;
  Boolean        fetched;
} SentinelData, PNTR SentinelPtr;

static Boolean AddToEntityList (SegmenT seg, PrimitivE prim, Uint2 segID,
                           Uint2 primID, Uint2 primCt, VoidPtr userdata)

{
  BioseqViewPtr  bvp;
  BaseFormPtr    bfp;
  Uint2          entityID;
  OMUserDataPtr  omudp;

  bvp = (BioseqViewPtr) userdata;
  entityID = segID;
  if (bvp == NULL || entityID == 0) return TRUE;
  bfp = (BaseFormPtr) GetObjectExtra (bvp->form);
  if (bfp == NULL || entityID == bfp->input_entityID) return TRUE;
  if (! InBioseqViewEntityList (entityID, bvp)) {
    ValNodeAddInt (&(bvp->entityList), 0, (Int4) entityID);
    omudp = ObjMgrAddUserData (entityID, bfp->procid, OMPROC_VIEW, bfp->userkey);
    if (omudp != NULL) {
      omudp->userdata.ptrvalue = (Pointer) bfp;
      omudp->messagefunc = BioseqViewMsgFunc;
    }
  }
  return TRUE;
}

static void GenomicSentinelProc (BigScalar calldata, PrimDrawContext pdc)

{
  BoxInfo        box;
  BioseqPtr      bsp;
  BioseqViewPtr  bvp;
  ValNodePtr     curr;
  Uint2          entityID;
  Boolean        forceSeglevelsTo1 = TRUE;
  GlobalBspPtr   gbp;
  ValNodePtr     graphs;
  /* SeqEntryPtr    oldscope; */
  Int2           oldstyle;
  SegmenT        pic;
  BasePPtr       prim;
  Int4           scaleX;
  Int4           scaleY;
  SeqEntryPtr    sep;
  Int4           seq_len;
  SeqGraphPtr    sgp;
  Boolean        show_msg = FALSE;
  ValNodePtr     slp_list;
  SeqLocPtr      slp;
  SentinelPtr    sp;
  Int2           style;
  ValNodePtr     vnp;
  BoxInfo        world;

  sp = (SentinelPtr) calldata;
  if (sp == NULL) return;
  if (sp->fetched) return;
  sp->fetched = TRUE;
  bvp = sp->bvp;
  bsp = bvp->bsp;
  slp_list = NULL;
  if (bvp->isGenome && bvp->gdraw_p != NULL) {
    slp_list = NULL;
    for (curr = bvp->gdraw_p->gbp_list; curr !=NULL; curr = curr->next) {
      gbp = curr->data.ptrvalue;
      bsp = gbp->bsp;
      seq_len = bsp->length;
      slp = load_seq_loc (sp->start, sp->stop, seq_len, FALSE,
                          SeqIdFindBest (bsp->id, 0), &show_msg, FALSE);
      if (slp != NULL) {
        ValNodeAddPointer ((Pointer) &slp_list, (Uint1) (gbp->priority), slp);
      }
    }
  } else {
    bsp = bvp->bsp;
    slp = SeqLocIntNew (sp->start, sp->stop, Seq_strand_plus, SeqIdFindBest (bsp->id, 0));
    ValNodeAddPointer (&slp_list, 0, (Pointer) slp);
  }
  oldstyle = GetMuskCurrentSt ();
  style = GetValue (bvp->style) - 1;
  SetMuskCurrentSt (GetMuskStyleName (style));

  bsp = bvp->bsp;
  sep = SeqMgrGetSeqEntryForData (bsp);
  if (IsADeltaBioseq (sep)) {
    forceSeglevelsTo1 = FALSE;
  }
  entityID = ObjMgrGetEntityIDForChoice (sep);
  if (entityID > 0) {
    sep = GetBestTopParentForData (entityID, bsp);
    /* oldscope = SeqEntrySetScope (sep); */
  }

  ViewerBox (bvp->vwr, NULL, NULL, NULL, &scaleX, &scaleY);
  pic = DrawSequinMapEx (slp_list, sep, scaleX, NULL, NULL, forceSeglevelsTo1, bvp->tempResultList);
  ChangeAddPrimOrder (ADD_TO_TAIL);
  prim = (BasePPtr) pic;
  prim->code = SEGMENT;
  OffsetSegment (pic, sp->start, 0);
  ExploreSegment (pic, (Pointer) bvp, AddToEntityList);

  graphs = CollectGraphs (entityID, bsp, sp->start, sp->stop);
  if (graphs != NULL) {
    SegmentBox (pic, &box);
    for (vnp = graphs; vnp != NULL; vnp = vnp->next) {
      sgp = (SeqGraphPtr) vnp->data.ptrvalue;
      AddGraphSentinelToPicture (sgp, bsp, pic, scaleX,
                                 box.bottom - 10, /* sp->start */ 0, NULL);
    }
  }

  LinkSegment (bvp->pict, pic);
  RecalculateSegment (pic, scaleX, scaleY);

  ViewerBox (bvp->vwr, &world, NULL, NULL, &scaleX, &scaleY);
  for (vnp = bvp->sentinelList; vnp != NULL; vnp = vnp->next) {
    sp = (SentinelPtr) vnp->data.ptrvalue;
    if (sp != NULL) {
      if (PrimitiveBox (sp->snt, scaleX, scaleY, &box)) {
        ChangeSentinelRectangle (sp->snt, box.left, world.top, box.right, world.bottom);
      }
    }
  }

  /*
  if (entityID > 0) {
    SeqEntrySetScope (oldscope);
  }
  */
  SetMuskCurrentSt (GetMuskStyleName (oldstyle));
}

static void PopulateGraphic (BioseqViewPtr bvp)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  DeltaSeqPtr    dsp;
  Uint2          entityID;
  Int2           i;
  Boolean        isGenome;
  Int4           labelWid;
  Int4           len;
  SeqLocPtr      loc;
  Int4           max;
  Int4           min;
  MakeMultiData  mmd;
  Int2           oldstyle;
  RecT           r;
  Int4           scaleX;
  Int4           scaleY;
  SeqEntryPtr    sep;
  SeqLocPtr      slp;
  ValNodePtr     slp_list;
  SentinelPtr    sp;
  Int4           start;
  Char           str [32];
  Int2           style;
  ValNode        vn;
  ValNodePtr     vnp;
  Int4           vwr_x;
  Int4           vwr_y;
  Int2           vwr_align;

  if (bvp == NULL) return;
  sep = SeqMgrGetSeqEntryForData (bvp->bsp);
  entityID = ObjMgrGetEntityIDForChoice (sep);
  sep = GetBestTopParentForData (entityID, bvp->bsp);
  if (! bvp->hasTargetControl) {
    PopulateSimpleGraphic (bvp); /* Nentrez should do this until more work is done */
    return;
  }
  isGenome = FALSE;
  if (IsAGenomeRecord (sep) ||
      IsSegmentedBioseqWithoutParts (sep)) {
    isGenome = TRUE;
    if (bvp->isGenome) {
      if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL && bssp->_class == BioseqseqSet_class_equiv) {
          PopulateSimpleGraphic (bvp);
          return;
        }
      }
    }

  } else if (IsADeltaBioseq (sep)) {
    /*
    isGenome = FALSE;
    if (bvp->isGenome) {
      if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL && bssp->_class == BioseqseqSet_class_equiv) {
          PopulateSimpleGraphic (bvp);
          return;
        }
      }
    }
    */
    PopulateSimpleGraphic (bvp);
    return;

  } else {
    PopulateSimpleGraphic (bvp);
    return;
  }
  WatchCursor ();
  vwr_x = INT4_MIN;
  vwr_y = INT4_MAX;
  vwr_align = UPPER_LEFT;
  if (bvp->moveToOldPos) {
    if (bvp->pict != NULL) {
      get_viewer_position (bvp->vwr, &(vwr_x), &(vwr_y));
      vwr_align = MIDDLE_CENTER;
    }
    bvp->moveToOldPos = FALSE;
  }
  Reset (bvp->vwr);
  bvp->pict = DeletePicture (bvp->pict);
  Update ();
  if (bvp->isGenome && bvp->gdraw_p == NULL) {
    PopulateMap (bvp);
    Reset (bvp->vwr);
    bvp->pict = DeletePicture (bvp->pict);
    Update ();
  }
  bsp = bvp->bsp;
  if (bsp != NULL) {
    scaleY = 1;
    len = 0;
    if (bvp->sentinelList != NULL) {
      bvp->sentinelList = ValNodeFreeData (bvp->sentinelList);
    }
    if (bvp->sentinelList == NULL) {
      sep = SeqMgrGetSeqEntryForData (bsp);
      if (bsp->repr == Seq_repr_raw) {
        sp = MemNew (sizeof (SentinelData));
        if (sp != NULL) {
          sp->bvp = bvp;
          sp->start = 0;
          sp->stop = bsp->length - 1;
          sp->fetched = FALSE;
          ValNodeAddPointer (&(bvp->sentinelList), 0, (Pointer) sp);
        }
      } else if (bsp->repr == Seq_repr_seg) {
        vn.choice = SEQLOC_MIX;
        vn.next = NULL;
        vn.data.ptrvalue = bsp->seq_ext;
        start = 0;
        slp = SeqLocFindNext (&vn, NULL);
        while (slp != NULL) {
          if (slp->choice != SEQLOC_NULL) {
            len = SeqLocLen (slp);
            if (len > 0) {
              sp = MemNew (sizeof (SentinelData));
              if (sp != NULL) {
                sp->bvp = bvp;
                sp->start = start;
                sp->stop = start + len - 1;
                sp->fetched = FALSE;
                ValNodeAddPointer (&(bvp->sentinelList), 0, (Pointer) sp);
                start += len;
              }
            }
          }
          slp = SeqLocFindNext (&vn, slp);
        }
      } else if (bsp->repr == Seq_repr_delta && bsp->seq_ext_type == 4) {
        dsp = (DeltaSeqPtr) bsp->seq_ext;
        loc = DeltaSeqsToSeqLocs (dsp);
        start = 0;
        slp = SeqLocFindNext (loc, NULL);
        while (slp != NULL) {
          if (slp->choice != SEQLOC_NULL) {
            len = SeqLocLen (slp);
            if (len > 0) {
              sp = MemNew (sizeof (SentinelData));
              if (sp != NULL) {
                sp->bvp = bvp;
                sp->start = start;
                sp->stop = start + len - 1;
                sp->fetched = FALSE;
                ValNodeAddPointer (&(bvp->sentinelList), 0, (Pointer) sp);
                start += len;
              }
            }
          }
          slp = SeqLocFindNext (loc, slp);
        }
        SeqLocFree (loc);
      } else if (bsp->repr == Seq_repr_map) {
        if (bsp->length > 100000000) {
          len = 10000000;
        } else if (bsp->length > 10000000) {
          len = 1000000;
        } else if (bsp->length > 1000000) {
          len = 100000;
        } else if (bsp->length > 100000) {
          len = 10000;
        } else {
          len = bsp->length;
        }
        start = 0;
        while (start < bsp->length) {
          sp = MemNew (sizeof (SentinelData));
          if (sp != NULL) {
            sp->bvp = bvp;
            sp->start = start;
            if (start + len - 1 > bsp->length) {
              len = bsp->length - start;
            }
            sp->stop = start + len - 1;
            sp->fetched = FALSE;
            ValNodeAddPointer (&(bvp->sentinelList), 0, (Pointer) sp);
            start += len;
          }
        }
      }
    }

    len = 0;
    if (! bvp->isGenome) {
      slp_list = NULL;
      sep = NULL;
      len = 0;
      if (bvp->viewWholeEntity) {
        sep = SeqMgrGetSeqEntryForData (bsp);
        entityID = ObjMgrGetEntityIDForChoice (sep);
        sep = GetTopSeqEntryForEntityID (entityID);
        mmd.slp_list = NULL;
        mmd.len = 0;
        BioseqExplore (sep, (Pointer) &mmd, MakeMultiSlpList);
        slp_list = mmd.slp_list;
        len = mmd.len;
      } else {
        sep = SeqMgrGetSeqEntryForData (bsp);
        len = bsp->length;
        slp = SeqLocIntNew (0, len-1, Seq_strand_plus, SeqIdFindBest (bsp->id, 0));
        ValNodeAddPointer (&slp_list, 0, (Pointer) slp);
      }
      if (bvp->slp_list != NULL) {
        free_slp_list (bvp->slp_list);
        bvp->slp_list = NULL;
      }
      bvp->slp_list = slp_list;
    }

    if (bvp->scaleNotCalculated) {
      SafeHide (bvp->scale);
      Reset (bvp->scale);
      ObjectRect (bvp->vwr, &r);
      InsetRect (&r, 4, 4);
      oldstyle = GetMuskCurrentSt ();
      style = GetValue (bvp->style) - 1;
      SetMuskCurrentSt (GetMuskStyleName (style));
      labelWid = CountMaxSeqLabel (bvp->slp_list) + 2;
      SetMuskCurrentSt (GetMuskStyleName (oldstyle));
      if (r.right - r.left < 2 * labelWid) {
        labelWid = 0;
      }
      if (isGenome) {
        if (bsp->length > 100000000) {
          max = 49999L;
        } else if (bsp->length > 10000000) {
          max = 4999L;
        } else if (bsp->length > 1000000) {
          max = 499L;
        } else if (bsp->length > 100000) {
          max = 49L;
        } else if (bsp->length > 10000) {
          max = 19L;
        } else if (bsp->length > 1000) {
          max = 9L;
        } else {
          max = 4L;
        }
        min = (Int4) 1;
      } else {
        max = (Int4) MAX (len / (Int4) (r.right - r.left - labelWid), 1) + 2;
        max = (Int4) MIN (max, 50000L);
        min = (Int4) MAX (len / 320000L, 1L);
      }
      bvp->maxScale = max;
      i = 1;
      while (i < MAXZOOMSCALEVAL && min > zoomScaleVal [i]) {
        i++;
      }
      bvp->minIndex = i;
      while (i < MAXZOOMSCALEVAL && max > zoomScaleVal [i - 1]) {
        sprintf (str, "%ld", (long) (zoomScaleVal [i]));
        if (max < zoomScaleVal [i]) {
          sprintf (str, "%ld", (long) zoomScaleVal [i]);
        }
        PopupItem (bvp->scale, str);
        i++;
      }
      SetValue (bvp->scale, MAX (i - bvp->minIndex - 3, 1));
      bvp->scaleNotCalculated = FALSE;
    }
    SafeShow (bvp->scale);
    i = GetValue (bvp->scale) - 1 + bvp->minIndex;
    if (i < MAXZOOMSCALEVAL && i > 0) {
      scaleX = MIN (zoomScaleVal [i], bvp->maxScale);
    } else {
      scaleX = bvp->minIndex;
    }

    bvp->pict = CreatePicture ();
    for (vnp = bvp->sentinelList; vnp != NULL; vnp = vnp->next) {
      sp = (SentinelPtr) vnp->data.ptrvalue;
      if (sp != NULL) {
        sp->seg = CreateSegment (bvp->pict, 0, 0);
        sp->snt = AddSntRectangle (bvp->pict, sp->start, -35, sp->stop, -40,
                                   GenomicSentinelProc, (BigScalar) sp, NULL, 0);
        /* AddRectangle (bvp->pict, sp->start, -35, sp->stop, -40, 0, FALSE, 0); */
      }
    }

    AttachPicture (bvp->vwr, bvp->pict, vwr_x, vwr_y, vwr_align, scaleX, scaleY,
                   bvp->highlightSelections ? DrawSelectionRange : NULL);
    SetViewerProcs (bvp->vwr, ClickGraphic, NULL, ReleaseGraphic, NULL);
    /* InitSelectGraphical (bvp); */
  }
  ArrowCursor ();
}

static void ClickAlignment (VieweR vwr, SegmenT seg, PoinT pt)

{
  BioseqViewPtr  bvp;

  bvp = (BioseqViewPtr) GetObjectExtra (vwr);
  if (bvp == NULL) return;
  bvp->wasDoubleClick = dblClick;
  bvp->wasShiftKey = shftKey;
  bvp->old_rect_shown = FALSE;
  bvp->pnt_start = pt;
  bvp->pnt_stop = pt;
  InvertMode ();
  CommonFrameProc (vwr, bvp);
}

static void DragAlignment (VieweR vwr, SegmenT seg, PoinT pt)

{
  BioseqViewPtr  bvp;

  bvp = (BioseqViewPtr) GetObjectExtra (vwr);
  if (bvp == NULL) return;
  if(bvp->wasDoubleClick || bvp->wasShiftKey) return;
  InvertMode ();
  CommonFrameProc (vwr, bvp);
  bvp->pnt_stop = pt;
  CommonFrameProc (vwr, bvp);
}

static void ReleaseAlignment (VieweR vwr, SegmenT seg, PoinT pt)

{
  BioseqViewPtr  bvp;
  Uint2          entityID;
  FILE           *fp;
  Uint2          itemID;
  Uint2          itemtype;
  Int4           left, right;
  Uint4          option = TXALIGN_MISMATCH;
  Char           path [PATH_MAX];
  SeqEntryPtr    sep;
  SeqLocPtr      slp;
  PntInfo        start_info, stop_info;
  Char           title [64];

  bvp = (BioseqViewPtr) GetObjectExtra (vwr);
  if (bvp == NULL) return;

  if (bvp->wasDoubleClick) {
    if (Find_segment_IDs (vwr, pt, &entityID, &itemID, &itemtype)) {
      sep = GetTopSeqEntryForEntityID (entityID);
      if (bvp->launchSubviewers) {
        WatchCursor ();
        Update ();
        LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
        ArrowCursor ();
        Update ();
      } else if (LaunchViewerNotEditor (bvp, sep, entityID, itemID, itemtype)) {
        WatchCursor ();
        Update ();
        LaunchNewBioseqViewer (bvp->bsp, entityID, itemID, itemtype);
        ArrowCursor ();
        Update ();
      } else if (bvp->launchEditors) {
        WatchCursor ();
        Update ();
        GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID,
                          itemtype, 0, 0, itemtype, 0);
        ArrowCursor ();
        Update ();
        return;
      } else {
        return;
      }
    }
    if (! bvp->sendSelectMessages) return;
    if (bvp->wasShiftKey) {
      ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
    } else {
      ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
    }
  } else if (bvp->sendSelectMessages) {
    if (Find_segment_IDs (vwr, pt, &entityID, &itemID, &itemtype)) {
      if (bvp->wasShiftKey) {
        ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
      } else {
        ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
      }
    } else if (! bvp->wasShiftKey) {
      ObjMgrDeSelect (0, 0, 0, 0, NULL);
    }
  }
  if (bvp->slp_list != NULL && bvp->anp_node != NULL) {
    InvertMode ();
    CommonFrameProc (vwr, bvp);
    bvp->pnt_stop = pt;

    if (ABS (bvp->pnt_start.x - bvp->pnt_stop.x) > 5) {
      WatchCursor ();
      Update ();
      GetTitle (ParentWindow (vwr), title, sizeof (title) - 1);
      MapViewerToWorld(vwr, bvp->pnt_start, &start_info);
      MapViewerToWorld(vwr, bvp->pnt_stop, &stop_info);
      left = MIN(start_info.x, stop_info.x);
      right  = MAX(start_info.x, stop_info.x);
      StringCat (title, " alignment");
      TmpNam (path);
      fp = FileOpen(path, "w");
      if (fp != NULL) {
        slp = bvp->slp_list->data.ptrvalue;
        if (ShowAlignNodeText (bvp->anp_node, -1, 40, fp, left, right, option, NULL, NULL))
          FileClose(fp);
        else
          FileClose(fp);
      }
      LaunchGeneralTextViewer (path, title);
    }
    FileRemove (path);
    ArrowCursor ();
    Update ();
  }
}

static void DrawAlignOnAllSequences (BioseqViewPtr bvp, SeqAlignPtr align, Int2 scaleX)

{
	ValNode vn;

	Int4 y_pos = 0;
	Int4 value, style;
	SeqIdPtr m_sip;
	BioseqPtr mbsp;
	SeqLocPtr m_loc;
	Int4 start, stop;
	CollectSeqOption cs_option;
	CollectAlignOption ca_option;
	SeqEntryPtr sep;
	Uint2 entityID;

	
	if (bvp == NULL || align == NULL) return;
	sep = SeqMgrGetSeqEntryForData (bvp->bsp);
	entityID = ObjMgrGetEntityIDForChoice (sep);

	m_sip = make_master(align);
	if(m_sip == NULL)
	{
		Message(MSG_ERROR, "Fail to get the master id");
		return;
	}

	mbsp = BioseqLockById(m_sip);
	if(mbsp == NULL)
	{
		get_boundary(m_sip, &start, &stop, align);
		m_loc = SeqLocIntNew(start, stop, Seq_strand_plus, m_sip);
	}
	else
		m_loc = SeqLocIntNew(0, mbsp->length-1, Seq_strand_plus, m_sip);
	vn.data.ptrvalue = m_loc;
	vn.next = NULL;
	if(scaleX <=0)
	{
		if(mbsp != NULL)
			BioseqUnlock(mbsp);
		SeqLocFree(m_loc);
		return;
	}


	style= GetMuskCParam(MSM_ALIGNMENT, 0, MSM_STYLE);
	if(style== MSM_MDIM)
	{
		if(!use_multiple_dimension(align))
			style= MSM_MPAIR;
	}
	else
		style= MSM_MPAIR;


	load_align_option_for_graphic(&ca_option, &cs_option, style, FALSE);
	if(mbsp == NULL)
		ca_option.show_mismatch = FALSE;
	ca_option.csop = &cs_option;

	if(style== MSM_MDIM)
		value = COLLECT_MD;
	else
		value = COLLECT_MP;
    if (bvp->anp_node != NULL) {
      bvp->anp_node = FreeAlignNode (bvp->anp_node);
    }
	bvp->anp_node = collect_anpnode_with_option(&ca_option, m_loc, entityID,
	                                            value, OBJ_SEQALIGN, NULL, NULL, TRUE);
	if(bvp->anp_node == NULL)
	{
		if(mbsp != NULL)
			BioseqUnlock(mbsp);
		SeqLocFree(m_loc);
		return;
	}

	bvp->pict = CreatePicture();
	DrawMPAlignment(bvp->anp_node, 0, SeqLocLen(m_loc)-1, m_loc, entityID,
	                scaleX, &y_pos, (Uint1)style, FALSE, bvp->pict);
	if(mbsp != NULL)
		BioseqUnlock(mbsp);
	SeqLocFree(m_loc);
}

static void PopulateAlignment (BioseqViewPtr bvp)

{
  SeqAlignPtr    align;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  Uint2          entityID;
  Int2           i;
  Int4           labelWid;
  Int4           len;
  Int4           max;
  Int4           min;
  MakeMultiData  mmd;
  SeqEntryPtr    oldscope = NULL;
  Int2           oldstyle;
  RecT           r;
  Int4           scaleX;
  Int4           scaleY;
  SeqEntryPtr    sep;
  ValNodePtr     slp_list;
  SeqLocPtr      slp;
  Char           str [32];
  Int2           style;
  Int4           vwr_x;
  Int4           vwr_y;
  Int2           vwr_align;
  Int4           y_pos = 0;

  if (bvp == NULL) return;
  WatchCursor ();
  vwr_x = INT4_MIN;
  vwr_y = INT4_MAX;
  vwr_align = UPPER_LEFT;
  if (bvp->moveToOldPos) {
    if (bvp->pict != NULL) {
      get_viewer_position (bvp->vwr, &(vwr_x), &(vwr_y));
      vwr_align = MIDDLE_CENTER;
    }
    bvp->moveToOldPos = FALSE;
  }
  Reset (bvp->vwr);
  bvp->pict = DeletePicture (bvp->pict);
  Update ();
  if (bvp->isGenome && bvp->slp_list == NULL) return;
  bsp = bvp->bsp;
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
    entityID = ObjMgrGetEntityIDForChoice (sep);
    sep = GetBestTopParentForData (entityID, bsp);
    if (sep != NULL && sep->choice == 2) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp != NULL && bssp->_class == 10) {
        ArrowCursor ();
        return;
      }
    }
    scaleY = 1;
    len = 0;
    slp = NULL;
    if (! bvp->isGenome) {
      slp_list = NULL;
      sep = NULL;
      len = 0;
      if (bvp->viewWholeEntity) {
        sep = SeqMgrGetSeqEntryForData (bsp);
        entityID = ObjMgrGetEntityIDForChoice (sep);
        sep = GetTopSeqEntryForEntityID (entityID);
        mmd.slp_list = NULL;
        mmd.len = 0;
        BioseqExplore (sep, (Pointer) &mmd, MakeMultiSlpList);
        slp_list = mmd.slp_list;
        len = mmd.len;
      } else {
        sep = SeqMgrGetSeqEntryForData (bsp);
        len = bsp->length;
        slp = SeqLocIntNew (0, len-1, Seq_strand_plus, SeqIdFindBest (bsp->id, 0));
        ValNodeAddPointer (&slp_list, 0, (Pointer) slp);
      }
      if (bvp->slp_list != NULL) {
        free_slp_list (bvp->slp_list);
        bvp->slp_list = NULL;
      }
      bvp->slp_list = slp_list;
    }
    oldstyle = GetMuskCurrentSt ();
    style = GetValue (bvp->style) - 1;
    SetMuskCurrentSt (GetMuskStyleName (style));

    if (bvp->scaleNotCalculated) {
      SafeHide (bvp->scale);
      Reset (bvp->scale);
      ObjectRect (bvp->vwr, &r);
      InsetRect (&r, 4, 4);
      labelWid = CountMaxSeqLabel (bvp->slp_list) + 2;
      if (r.right - r.left < 2 * labelWid) {
        labelWid = 0;
      }
      if (bvp->isGenome) {
        slp = (SeqLocPtr) bvp->slp_list->data.ptrvalue;
        len = slp_list_len (slp);
      }
      max = (Int4) MAX (len / (Int4) (r.right - r.left - labelWid), 1) + 2;
      max = (Int4) MIN (max, 50000L);
      min = (Int4) MAX (len / 320000L, 1L);
      bvp->maxScale = max;
      i = 1;
      while (i < MAXZOOMSCALEVAL && min > zoomScaleVal [i]) {
        i++;
      }
      bvp->minIndex = i;
      while (i < MAXZOOMSCALEVAL && max > zoomScaleVal [i - 1]) {
        sprintf (str, "%ld", (long) (zoomScaleVal [i]));
        if (max < zoomScaleVal [i]) {
          sprintf (str, "%ld", (long) zoomScaleVal [i]);
        }
        PopupItem (bvp->scale, str);
        i++;
      }
      SetValue (bvp->scale, MAX (i - bvp->minIndex, 1));
      bvp->scaleNotCalculated = FALSE;
    }
    SafeShow (bvp->scale);
    i = GetValue (bvp->scale) - 1 + bvp->minIndex;
    if (i < MAXZOOMSCALEVAL && i > 0) {
      scaleX = MIN (zoomScaleVal [i], bvp->maxScale);
    } else {
      scaleX = bvp->minIndex;
    }

    sep = SeqMgrGetSeqEntryForData (bsp);
    entityID = ObjMgrGetEntityIDForChoice (sep);
    if (entityID > 0) {
      sep = GetBestTopParentForData (entityID, bsp);
      oldscope = SeqEntrySetScope (sep);
    }

    if (bvp->viewWholeEntity) {
      align = NULL;
      sep = GetTopSeqEntryForEntityID (entityID);
      SeqEntryExplore (sep, &align, FindAlignment);
      DrawAlignOnAllSequences (bvp, align, scaleX);
    } else if (bvp->anp_node == NULL) {
      Reset (bvp->vwr);
      bvp->pict = DeletePicture (bvp->pict);
      slp = (SeqLocPtr) bvp->slp_list->data.ptrvalue;
      bvp->pict = DrawSeqHistoryAlignment (slp, scaleX,
                                           &(bvp->anp_node),
                                           &(bvp->seq_entityID),
                                           FALSE);
    } else if (bvp->seq_entityID > 0) {
      bvp->pict = CreatePicture ();
      slp = (SeqLocPtr) bvp->slp_list->data.ptrvalue;
      DrawMPAlignment (bvp->anp_node, 0, SeqLocLen (slp), slp,
                       bvp->seq_entityID, scaleX, &y_pos,
                       MSM_MPAIR, FALSE, bvp->pict);
    } else {
      if (entityID > 0) {
        SeqEntrySetScope (oldscope);
      }
      ArrowCursor ();
      return;
    }

    if (entityID > 0) {
      SeqEntrySetScope (oldscope);
    }

    AttachPicture (bvp->vwr, bvp->pict, vwr_x, vwr_y,
                   vwr_align, scaleX, scaleY, NULL);
    SetViewerProcs (bvp->vwr, ClickAlignment, DragAlignment, ReleaseAlignment, NULL);
    SetMuskCurrentSt (GetMuskStyleName (oldstyle));
    InitSelectGraphical (bvp);
  }
  ArrowCursor ();
}

static void ClickSalsa (PaneL pnl, PoinT pt)

{
  BioseqViewPtr  bvp;

  bvp = (BioseqViewPtr) GetObjectExtra (pnl);
  if (bvp == NULL) return;
  bvp->wasDoubleClick = dblClick;
  bvp->wasShiftKey = shftKey;
}

static void ReleaseSalsa (PaneL pnl, PoinT pt)

{
  BioseqViewPtr  bvp;
  Uint2          entityID;
  Uint2          itemID;
  Uint2          itemtype;
  SeqEntryPtr    sep;

  bvp = (BioseqViewPtr) GetObjectExtra (pnl);
  if (bvp == NULL) return;

  if (bvp->wasDoubleClick) {
    if (FindIdsInSalsaPanel (pnl, pt, &entityID, &itemID, &itemtype)) {
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
        GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID,
                          itemtype, 0, 0, itemtype, 0);
        ArrowCursor ();
        Update ();
        return;
      } else {
        return;
      }
    }
    if (! bvp->sendSelectMessages) return;
    if (bvp->wasShiftKey) {
      ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
    } else {
      ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
    }
  } else if (bvp->sendSelectMessages) {
    if (FindIdsInSalsaPanel (pnl, pt, &entityID, &itemID, &itemtype)) {
      if (bvp->wasShiftKey) {
        ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
      } else {
        ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
      }
    } else if (! bvp->wasShiftKey) {
      ObjMgrDeSelect (0, 0, 0, 0, NULL);
    }
  }
}

static void PopulateSalsa (BioseqViewPtr bvp)

{
  BioseqPtr    bsp;
  Uint2        entityID;
  Uint1        numbering;
  Int2         oldstyle;
  RecT         r;
  SeqEntryPtr  sep;
  Uint1        sequence_shown;
  Uint1        show_feat;
  Int2         style;
  WindoW       tempPort;
  Int2         val;

  if (bvp == NULL) return;
  WatchCursor ();
  Reset (bvp->pnl);
  SetPanelClick (bvp->pnl, NULL, NULL, NULL, NULL);
  Update ();
  bsp = bvp->bsp;
  if (bsp != NULL) {
    oldstyle = GetMuskCurrentSt ();
    style = GetValue (bvp->style) - 1;
    SetMuskCurrentSt (GetMuskStyleName (style));
    sep = SeqMgrGetSeqEntryForData (bsp);
    if (bvp->viewWholeEntity) {
      entityID = ObjMgrGetEntityIDForChoice (sep);
      sep = GetTopSeqEntryForEntityID (entityID);
    }
    val = GetValue (bvp->seqControl);
    switch (val) {
      case 1 :
        sequence_shown = SEQ_SHOW1;
        break;
      case 2 :
        sequence_shown = SEQ_SHOWALG;
        break;
      default :
        sequence_shown = SEQ_SHOW1;
        break;
    }
    val = GetValue (bvp->featControl);
    switch (val) {
      case 1 :
        show_feat = SEQ_FEAT0;
        break;
      case 2 :
        show_feat = SEQ_FEAT1;
        break;
      case 3 :
        show_feat = SEQ_FEATALL;
        break;
      default :
        show_feat = SEQ_FEAT1;
        break;
    }
    val = GetValue (bvp->numControl);
    switch (val) {
      case 1 :
        numbering = SEQ_NUM0;
        break;
      case 2 :
        numbering = SEQ_NUM1;
        break;
      case 3 :
        numbering = SEQ_NUM2;
        break;
      default :
        numbering = SEQ_NUM2;
        break;
    }
    PopulateSalsaPanel (bvp->pnl, sep, bvp->viewWholeEntity,
                        sequence_shown, show_feat, numbering,
                        bvp->displayFont);
    SetPanelClick (bvp->pnl, ClickSalsa, NULL, NULL, ReleaseSalsa);
    SetMuskCurrentSt (GetMuskStyleName (oldstyle));
    if (Visible (bvp->pnl) && AllParentsVisible (bvp->pnl)) {
      tempPort = SavePort (bvp->pnl);
      ResetClip ();
      Select (bvp->pnl);
      ObjectRect (bvp->pnl, &r);
      InsetRect (&r, 1, 1);
      InvalRect (&r);
      RestorePort (tempPort);
    }
  }
  ArrowCursor ();
}

#define VSM_PICT_UP_BUTTON  65001
#define VSM_PICT_DOWN_BUTTON 65002

static void ClickDesktop (VieweR vwr, SegmenT s, PoinT pt)

{
  BioseqViewPtr  bvp;

  bvp = (BioseqViewPtr) GetObjectExtra (vwr);
  if (bvp == NULL) return;
  bvp->wasDoubleClick = dblClick;
  bvp->wasShiftKey = shftKey;
}

static void ReleaseDesktop (VieweR vwr, SegmenT s, PoinT pt)

{
  BioseqPtr      bsp;
  BioseqViewPtr  bvp;
  Uint2          entityID;
  Int2           expand = 0;
  Uint2          itemID;
  Uint2          itemtype;
  Uint2          primID;
  SegmenT        seg;
  Uint2          segID;
  SeqEntryPtr    sep;

  bvp = (BioseqViewPtr) GetObjectExtra (vwr);
  if (bvp == NULL) return;
  bsp = bvp->bsp;
  if (bsp == NULL) return;
  sep = SeqMgrGetSeqEntryForData (bsp);
  entityID = ObjMgrGetEntityIDForChoice (sep);

  seg = FindSegment (vwr, pt, &segID, &primID, NULL);
  itemID = primID;
  itemtype = segID;
  if (primID == 0 || segID == 0) {
    if (! bvp->wasShiftKey) {
      ObjMgrDeSelect (0, 0, 0, 0, NULL);
    }
    return;
  }
  if (itemID == VSM_PICT_UP_BUTTON) {
    expand = 1;
  } else if (itemID == VSM_PICT_DOWN_BUTTON) {
    expand = -1;
  }
  if (expand != 0) {
    bvp->expansion += expand;
    Hide (vwr);
    ResetClip ();
    Update ();
    PopulateDesktop (bvp);
    Show (vwr);
    Update ();
    return;
  }
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
      GatherProcLaunch (OMPROC_EDIT, FALSE, entityID, itemID,
                        itemtype, 0, 0, itemtype, 0);
      ArrowCursor ();
      Update ();
      return;
    } else {
      return;
    }
  } else if (bvp->sendSelectMessages) {
    if (bvp->wasShiftKey) {
      ObjMgrAlsoSelect (entityID, itemID, itemtype, 0, NULL);
    } else {
      ObjMgrSelect (entityID, itemID, itemtype, 0, NULL);
    }
  }
}

static void PopulateDesktop (BioseqViewPtr bvp)

{
  BioseqPtr      bsp;
  Uint2          entityID;
  Boolean        oldExtraLevel;
  ObjMgrDataPtr  omdp;
  SegmenT        seg;
  SeqEntryPtr    sep;
  FonT           tempfont;
  VSeqMgrPtr     vsmp;
  VSMPictPtr     vsmpp;
  Int4           vwr_x;
  Int4           vwr_y;
  Int2           vwr_align;

  if (bvp == NULL) return;
  WatchCursor ();
  vwr_x = INT4_MIN;
  vwr_y = INT4_MAX;
  vwr_align = UPPER_LEFT;
  if (bvp->moveToOldPos) {
    if (bvp->pict != NULL) {
      get_viewer_position (bvp->vwr, &(vwr_x), &(vwr_y));
      vwr_align = MIDDLE_CENTER;
    }
    bvp->moveToOldPos = FALSE;
  }
  Reset (bvp->vwr);
  bvp->pict = DeletePicture (bvp->pict);
  Update ();
  bsp = bvp->bsp;
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
    entityID = ObjMgrGetEntityIDForChoice (sep);
    omdp = ObjMgrGetData (entityID);
    vsmpp = MemNew (sizeof (VSMPict));
    if (vsmpp != NULL) {
      vsmpp->expansion = bvp->expansion;
      vsmp = VSeqMgrGet ();
      if (vsmp != NULL) {
        oldExtraLevel = vsmp->extraLevel;
        vsmp->extraLevel = TRUE;
        tempfont = vsmp->font;
        if (bvp->displayFont != NULL) {
          vsmp->font = bvp->displayFont;
        }
        SelectFont (vsmp->font);
        vsmp->lineheight = LineHeight();
        vsmp->leading = Leading();
        vsmp->charw = MaxCharWidth();
        seg = VSMEntityDraw (omdp, vsmpp, vsmp);
        vsmp->font = tempfont;
        SelectFont (vsmp->font);
        vsmp->lineheight = LineHeight();
        vsmp->leading = Leading();
        vsmp->charw = MaxCharWidth();
        SelectFont (systemFont);
        vsmp->extraLevel = oldExtraLevel;
        bvp->pict = CreatePicture ();
        LinkSegment (bvp->pict, seg);
        AttachPicture (bvp->vwr, bvp->pict, vwr_x, vwr_y, vwr_align, 1, 1, NULL);
        SetViewerProcs (bvp->vwr, ClickDesktop, NULL, ReleaseDesktop, NULL);
      }
    }
    MemFree (vsmpp);
    InitSelectDesktop (bvp);
  }
  ArrowCursor ();
}

static ParData  geneParFmt = {FALSE, FALSE, FALSE, FALSE, TRUE, 0, 0};

typedef struct geneinputdata {
  GrouP              geneGrp;
  TexT               geneText;
  DoC                geneList;
  Int2               clickedItem;
  Int2               clickedRow;
  Int2               selectedItem;
  Int2               selectedRow;
} GeneInputData, PNTR GeneInputDataPtr;

typedef struct genechoicedata {
  FORM_MESSAGE_BLOCK

  BioseqViewPtr      bvp;
  GeneInputData      geneData1;
  GeneInputData      geneData2;
  Int2               numItems;
  Int2               geneIdxNum;
  CharPtr PNTR       geneListIdx;
} GeneChoiceData, PNTR GeneChoiceDataPtr;

static void CopyField (CharPtr str, size_t max, CharPtr source, Int2 col)

{
  Char     ch;
  size_t   count;
  CharPtr  ptr;

  if (str != NULL && max > 0 && source != NULL) {
    MemSet (str, 0, max);
      ptr = source;
      ch = *ptr;
      while (col > 1 && ch != '\n' && ch != '\0') {
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
      max = MIN (max, count);
      StringNCpy (str, ptr, max); /* remains StringNCpy, not _0 */
  }
}

static void CopyStrFromGeneList (GeneChoiceDataPtr gcp, CharPtr str, size_t max, Int2 row)

{
  CharPtr  source;

  if (gcp != NULL && str != NULL && max > 0) {
    MemSet (str, 0, max);
    if (gcp->geneListIdx != NULL && row > 0) {
      source = gcp->geneListIdx [row - 1];
      CopyField (str, max, source, 1);
    }
  }
}

static Int2 FindGeneName (GeneChoiceDataPtr gcp, CharPtr text)

{
  Int2  compare;
  Int2  left;
  Int2  mid;
  Int2  right;
  Char  str [256];

  mid = 0;
  if (gcp != NULL && text != NULL && gcp->geneIdxNum > 0) {
    left = 1;
    right = gcp->geneIdxNum;
    while (left <= right) {
      mid = (left + right) / 2;
      CopyStrFromGeneList (gcp, str, sizeof (str) - 2, mid);
      compare = StringICmp (text, str);
      if (compare <= 0) {
        right = mid - 1;
      }
      if (compare >= 0) {
        left = mid + 1;
      }
    }
    if (left <= right + 1) {
      CopyStrFromGeneList (gcp, str, sizeof (str) - 2, mid);
      str [StringLen (text)] = '\0';
      compare = StringICmp (text, str);
      if (compare > 0) {
        mid++;
        if (mid <= gcp->geneIdxNum) {
          CopyStrFromGeneList (gcp, str, sizeof (str) - 2, mid);
          str [StringLen (text)] = '\0';
          compare = StringICmp (text, str);
          if (compare > 0) {
            mid = 0;
          }
        }
      }
    }
  }
  return mid;
}

static void AutoScrollGeneName (GeneChoiceDataPtr gcp, TexT t)

{
  GeneInputDataPtr  gdp;
  Boolean           goOn;
  Int2              item;
  Int2              itm;
  Int4              line;
  Int2              num;
  Int2              numRows;
  Int2              oldItem;
  Int2              oldRow;
  Int2              row;
  Int4              startsAt;
  Char              str [256];
  Char              txt [256];

  if (gcp != NULL && t != NULL && gcp->geneListIdx != NULL) {
    if (t == gcp->geneData1.geneText) {
      gdp = &(gcp->geneData1);
    } else if (t == gcp->geneData2.geneText) {
      gdp = &(gcp->geneData2);
    } else {
      return;
    }
    GetTitle (t, str, sizeof (str) - 2);
    num = 0;
    oldItem = gdp->selectedItem;
    oldRow = gdp->selectedRow;
    line = FindGeneName (gcp, str);
    if (line > 0 && line <= gcp->geneIdxNum) {
      item = 0;
      row = 0;
      itm = 1;
      goOn = TRUE;
      while (goOn && itm <= gcp->numItems) {
        GetItemParams4 (gdp->geneList, itm, &startsAt, &numRows, NULL, NULL, NULL);
        if (line > startsAt + numRows) {
          itm++;
        } else {
          item = itm;
          row = line - startsAt;
          goOn = FALSE;
        }
      }
      if (! RowIsVisible (gdp->geneList, item, row, NULL, NULL)) {
        SetOffset (gdp->geneList, 0, line - 1);
      }
      CopyStrFromGeneList (gcp, txt, sizeof (txt) - 2, line);
      if (StringICmp (txt, str) != 0) {
        gdp->selectedItem = 0;
        gdp->selectedRow = 0;
        InvalDocRows (gdp->geneList, oldItem, oldRow, oldRow);
        return;
      }
      if (oldItem != item || oldRow != row) {
        gdp->selectedItem = 0;
        gdp->selectedRow = 0;
        InvalDocRows (gdp->geneList, oldItem, oldRow, oldRow);
        gdp->selectedItem = item;
        gdp->selectedRow = row;
        InvalDocRows (gdp->geneList, item, row, row);
      }
    }
  }
}

static void GeneNameText (TexT t)

{
  GeneChoiceDataPtr  gcp;

  gcp = (GeneChoiceDataPtr) GetObjectExtra (t);
  if (gcp != NULL) {
    AutoScrollGeneName (gcp, t);
  }
}

static ValNodePtr dup_slp_list(ValNodePtr slp_list, Int4 window_size)
{
	ValNodePtr head = NULL;
	SeqLocPtr slp;
	SeqLocPtr dup_slp;
	Int4 start, stop;
	Int4 offset;
	SeqIntPtr sint;

	while(slp_list)
	{
		slp = slp_list->data.ptrvalue;
		if(slp->choice == SEQLOC_INT)
		{
			sint = slp->data.ptrvalue;
			start = MIN(sint->from, sint->to);
			stop = MAX(sint->from, sint->to);
			offset = window_size - (stop - start);
			if(offset > 0)
			{
				start -= offset/2;
				stop += offset/2;
				if(start < 0)
					start = 0;
			}
			
			dup_slp = SeqLocIntNew(start, stop, Seq_strand_plus, sint->id);
			ValNodeAddPointer(&head, 0, dup_slp);
		}
		slp_list = slp_list->next;
	}

	return head;
}

static Int4 get_default_window_size(GlobalDrawPtr gdraw_p)
{
	GlobalBspPtr gbp;
	ValNodePtr curr;
	Int4 max_len = 0, val;

	for(curr = gdraw_p->gbp_list; curr != NULL; curr = curr->next)
	{
		gbp = (GlobalBspPtr)(curr->data.ptrvalue);
		max_len = MAX(max_len, gbp->bsp->length);
	}

	val = MIN(max_len, 10000);
	return MAX(max_len/20, val);
}

static void AcceptGeneName (ButtoN b)

{
  BioseqViewPtr      bvp;
  GeneChoiceDataPtr  gcp;
  GeneDataPtr        gdata;
  ValNodePtr         old_g_list = NULL;
  RecT               r;
  Char               str [64];
  CharPtr            txt;
  ValNodePtr         vnp;
  WindoW             w;
  Int4               window_size;

  w = ParentWindow (b);
  Hide (w);
  gcp = (GeneChoiceDataPtr) GetObjectExtra (b);
  if (gcp != NULL && gcp->bvp != NULL) {
    bvp = gcp->bvp;

    str [0] = '\0';
    txt = GetDocText (gcp->geneData1.geneList, gcp->geneData1.selectedItem,
                      gcp->geneData1.selectedRow, 1);
    SafeSetTitle (gcp->geneData1.geneText, txt);
    MemFree (txt);

    GetTitle (gcp->geneData1.geneText, str, sizeof (str));
    bvp->pict = DeletePicture (bvp->pict);
    ObjectRect (bvp->vwr, &r);
    InsetRect (&r, 4, 4);
    if (StringHasNoText (str)) {
      old_g_list = bvp->g_list;
      bvp->g_list = NULL;
    } else {
      vnp = ValNodeNew (NULL);
      old_g_list = bvp->g_list;
      if (vnp != NULL) {
        vnp->data.ptrvalue = StringSave (str);
      }
      bvp->g_list = vnp;
      if (Visible (gcp->geneData2.geneGrp)) {
        str [0] = '\0';
        txt = GetDocText (gcp->geneData2.geneList, gcp->geneData2.selectedItem, gcp->geneData2.selectedRow, 1);
        SafeSetTitle (gcp->geneData2.geneText, txt);
        MemFree (txt);
        GetTitle (gcp->geneData2.geneText, str, sizeof (str));
        if (!StringHasNoText (str)) {
          ValNodeCopyStr (&(bvp->g_list), 0, str);
        }
      }
    }

    gdata = NULL;
    if (map_gene_location (bvp->gdraw_p, bvp->g_list, &gdata, NULL, FALSE)) {
    }

    PopulateMap (bvp);

    if (old_g_list != NULL) {
      ValNodeFreeData (old_g_list);
    }
    if (bvp->gdraw_p != NULL) {
      if (bvp->slp_list != NULL) {
        free_slp_list (bvp->slp_list);
      }
      bvp->slp_list = NULL;
      window_size = get_default_window_size (bvp->gdraw_p);
      bvp->slp_list = dup_slp_list (bvp->gdraw_p->slp_list, window_size);
    }
  }
  Remove (w);
}

static Boolean HighlightGeneName (DoC d, Int2 item, Int2 row, Int2 col)

{
  GeneChoiceDataPtr  gcp;
  GeneInputDataPtr   gdp;

  gcp = (GeneChoiceDataPtr) GetObjectExtra (d);
  if (gcp != NULL) {
    if (d == gcp->geneData1.geneList) {
      gdp = &(gcp->geneData1);
    } else if (d == gcp->geneData2.geneList) {
      gdp = &(gcp->geneData2);
    } else {
      return FALSE;
    }
    return (Boolean) (item == gdp->selectedItem && row == gdp->selectedRow);
  } else {
    return FALSE;
  }
}

static void ClickGeneName (DoC d, PoinT pt)

{
  GeneChoiceDataPtr  gcp;
  GeneInputDataPtr   gdp;
  Int2               item;
  Int2               oldItem;
  Int2               oldRow;
  Int2               row;

  gcp = (GeneChoiceDataPtr) GetObjectExtra (d);
  if (gcp != NULL) {
    if (d == gcp->geneData1.geneList) {
      gdp = &(gcp->geneData1);
    } else if (d == gcp->geneData2.geneList) {
      gdp = &(gcp->geneData2);
    } else {
      return;
    }
    MapDocPoint (d, pt, &item, &row, NULL, NULL);
    if (item > 0 && row > 0) {
      gdp->clickedItem = item;
      gdp->clickedRow = row;
      oldItem = gdp->selectedItem;
      oldRow = gdp->selectedRow;
      if (gdp->selectedItem != item || gdp->selectedRow != row) {
        gdp->selectedItem = 0;
        gdp->selectedRow = 0;
        InvalDocRows (d, oldItem, oldRow, oldRow);
        gdp->selectedItem = item;
        gdp->selectedRow = row;
        InvalDocRows (d, item, row, row);
      }
    }
  }
}

static void ReleaseGeneName (DoC d, PoinT pt)

{
  GeneChoiceDataPtr  gcp;
  GeneInputDataPtr   gdp;
  Int2               item;
  Int2               row;
  CharPtr            str;

  gcp = (GeneChoiceDataPtr) GetObjectExtra (d);
  if (gcp != NULL) {
    if (d == gcp->geneData1.geneList) {
      gdp = &(gcp->geneData1);
    } else if (d == gcp->geneData2.geneList) {
      gdp = &(gcp->geneData2);
    } else {
      return;
    }
    MapDocPoint (d, pt, &item, &row, NULL, NULL);
    if (gdp->clickedItem == item && gdp->clickedRow == row && item > 0 && row > 0) {
      str = GetDocText (d, item, row, 1);
      SafeSetTitle (gdp->geneText, str);
      Select (gdp->geneText);
      MemFree (str);
    }
  }
}

static void ShowSecondGroup (GrouP g)

{
  GeneChoiceDataPtr  gcp;

  gcp = (GeneChoiceDataPtr) GetObjectExtra (g);
  if (gcp != NULL) {
    if (GetValue (g) == 2) {
      SafeShow (gcp->geneData2.geneGrp);
    } else {
      SafeHide (gcp->geneData2.geneGrp);
    }
  }
}

static void CleanupGeneChoiceForm (GraphiC g, VoidPtr data)

{
  GeneChoiceDataPtr  gcp;

  gcp = (GeneChoiceDataPtr) data;
  if (gcp != NULL) {
    gcp->geneListIdx = MemFree (gcp->geneListIdx);
  }
  StdCleanupFormProc (g, data);
}

static void DisplayGeneChoiceWindow (CharPtr path, BioseqViewPtr bvp)

{
  ButtoN             b;
  GrouP              c;
  Char               ch;
  Pointer            dataptr;
  GrouP              g;
  GeneChoiceDataPtr  gcp;
  CharPtr PNTR       geneIdx;
  ValNodePtr         g_list;
  GrouP              h;
  Boolean            hasTwoGenesSelected;
  Int2               idx;
  Int2               item;
  GrouP              k;
  Int2               numItems;
  Int4               numLines;
  Int2               numRows;
  PrompT             p;
  Char               str [64];
  CharPtr            tmp;
  WindoW             w;

  if (bvp == NULL) return;
  gcp = (GeneChoiceDataPtr) MemNew (sizeof (GeneChoiceData));
  if (gcp == NULL) return;
  gcp->bvp = bvp;
  w = MovableModalWindow (-50, -33, -10, -10, "Gene and Product List", NULL);
  SetObjectExtra (w, gcp, CleanupGeneChoiceForm);
  k = HiddenGroup (w, 2, 0, ShowSecondGroup);
  SetObjectExtra (k, gcp, NULL);
  RadioButton (k, "Find Single Gene");
  RadioButton (k, "Enter Gene Range");
  SetValue (k, 1);
  h = HiddenGroup (w, -2, 0, NULL);

  str [0] = '\0';
  if (bvp->g_list != NULL) {
    g_list = bvp->g_list;
    StringNCpy_0 (str, (CharPtr) g_list->data.ptrvalue, sizeof (str));
  }
  gcp->geneData1.geneGrp = HiddenGroup (h, -1, 0, NULL);
  g = HiddenGroup (gcp->geneData1.geneGrp, 4, 0, NULL);
  p = StaticPrompt (g, "First Symbol", 0, dialogTextHeight, programFont, 'l');
  gcp->geneData1.geneText = DialogText (g, str, 10, GeneNameText);
  SetObjectExtra (gcp->geneData1.geneText, gcp, NULL);
  gcp->geneData1.geneList = DocumentPanel (gcp->geneData1.geneGrp, 15 * stdCharWidth,
                                           10 * stdLineHeight);
  SetObjectExtra (gcp->geneData1.geneList, gcp, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) gcp->geneData1.geneList, NULL);
  SetDocProcs (gcp->geneData1.geneList, ClickGeneName, NULL, ReleaseGeneName, NULL);
  SetDocShade (gcp->geneData1.geneList, NULL, NULL, HighlightGeneName, NULL);
  DisplayFancy (gcp->geneData1.geneList, path, &geneParFmt, NULL, systemFont, 4);

  str [0] = '\0';
  if (bvp->g_list != NULL) {
    g_list = bvp->g_list;
    g_list = g_list->next;
    if (g_list != NULL) {
      StringNCpy_0 (str, (CharPtr) g_list->data.ptrvalue, sizeof (str));
    }
  }
  hasTwoGenesSelected = (Boolean) (str [0] == '\0');
  gcp->geneData2.geneGrp = HiddenGroup (h, -1, 0, NULL);
  g = HiddenGroup (gcp->geneData2.geneGrp, 4, 0, NULL);
  p = StaticPrompt (g, "Second Symbol", 0, dialogTextHeight, programFont, 'l');
  gcp->geneData2.geneText = DialogText (g, str, 10, GeneNameText);
  SetObjectExtra (gcp->geneData2.geneText, gcp, NULL);
  gcp->geneData2.geneList = DocumentPanel (gcp->geneData2.geneGrp, 15 * stdCharWidth,
                                           10 * stdLineHeight);
  SetObjectExtra (gcp->geneData2.geneList, gcp, NULL);
  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) gcp->geneData2.geneList, NULL);
  SetDocProcs (gcp->geneData2.geneList, ClickGeneName, NULL, ReleaseGeneName, NULL);
  SetDocShade (gcp->geneData2.geneList, NULL, NULL, HighlightGeneName, NULL);
  DisplayFancy (gcp->geneData2.geneList, path, &geneParFmt, NULL, systemFont, 4);
  if (hasTwoGenesSelected) {
    Hide (gcp->geneData2.geneGrp);
  } else {
    SetValue (k, 2);
  }

  gcp->geneIdxNum = 0;
  gcp->geneListIdx = NULL;
  gcp->numItems = 0;
  c = HiddenGroup (w, 4, 0, NULL);
  SetGroupSpacing (c, 10, 3);
  b = DefaultButton (c, "Accept", AcceptGeneName);
  SetObjectExtra (b, gcp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) h, (HANDLE) c, NULL);
  GetDocParams4 (gcp->geneData1.geneList, &numItems, &numLines);
  if (numItems > 0 && numLines > 0 && numLines < 32000) {
    gcp->numItems = numItems;
    geneIdx = MemNew (sizeof (CharPtr) * (size_t) (numLines + 3));
    gcp->geneListIdx = geneIdx;
    if (geneIdx != NULL) {
      gcp->geneIdxNum = numLines;
      idx = 0;
      for (item = 1; item <= numItems; item++) {
        GetItemParams4 (gcp->geneData1.geneList, item, NULL, &numRows, NULL, NULL, &dataptr);
        if (dataptr != NULL && numRows > 0 && idx < numLines) {
          tmp = (CharPtr) dataptr;
          ch = *tmp;
          geneIdx [idx] = tmp;
          while (ch != '\0') {
            if (ch == '\n') {
              idx++;
              tmp++;
              ch = *tmp;
              geneIdx [idx] = tmp;
            } else {
              tmp++;
              ch = *tmp;
            }
          }
        }
      }
      GeneNameText (gcp->geneData1.geneText);
    }
  }
  Show (w);
  Select (w);
  Select (gcp->geneData1.geneText);
}

static int LIBCALLBACK CompareValNodeStrings (VoidPtr ptr1, VoidPtr ptr2)

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

typedef struct getgenesdata {
  SeqEntryPtr  nsep;
  BioseqPtr    segbsp;
  Boolean      dirty;
  MonitorPtr   mon;
  Int2         count;
} GetGenesData, PNTR GetGenesPtr;

static Boolean GetGenesOnParts (GatherContextPtr gcp)

{
  BioseqContextPtr  bcp;
  BioseqPtr         bsp;
  Int4              from;
  GetGenesPtr       ggp;
  GatherRangePtr    grp;
  Int2              i;
  SeqFeatPtr        newsfp;
  Boolean           noLeft;
  Boolean           noRight;
  Boolean           partial5;
  Boolean           partial3;
  SeqFeatPtr        prot;
  ProtRefPtr        prp;
  SeqIdPtr          sip;
  SeqFeatPtr        sfp;
  Int4              to;
  SeqFeatXrefPtr    xref;

  if (gcp == NULL) return TRUE;
  ggp = (GetGenesPtr) gcp->userdata;
  if (ggp == NULL) return TRUE;
  switch (gcp->thistype) {
    case OBJ_BIOSEQ_SEG :
      (ggp->count)++;
      MonitorIntValue (ggp->mon, ggp->count);
      break;
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gcp->thisitem;
      if (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_GENE || sfp->data.choice == SEQFEAT_CDREGION) {
          CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
          ggp->dirty = TRUE;
          newsfp = AsnIoMemCopy (sfp, (AsnReadFunc) SeqFeatAsnRead,
                                 (AsnWriteFunc) SeqFeatAsnWrite);
          if (newsfp != NULL) {
            newsfp->location = SeqLocFree (newsfp->location);
            for (grp = gcp->rdp, i = 0;  grp != NULL && i < gcp->num_interval; grp++, i++) {
              if (grp->strand == Seq_strand_minus) {
                from = grp->right;
                to = grp->left;
                partial5 = grp->r_trunc;
                partial3 = grp->l_trunc;
              } else {
                from = grp->left;
                to = grp->right;
                partial5 = grp->l_trunc;
                partial3 = grp->r_trunc;
              }
              AddSeqFeatInterval (newsfp, ggp->segbsp, from, to, partial5, partial3);
            }
            FreeAllFuzz (newsfp->location);
            SetSeqLocPartial (newsfp->location, noLeft, noRight);
            newsfp->partial = (newsfp->partial || noLeft || noRight);
            CreateNewFeature (ggp->nsep, NULL, sfp->data.choice, newsfp);
            if (sfp->data.choice == SEQFEAT_CDREGION) {
              sip = SeqLocId (newsfp->product);
              if (sip != NULL) {
                bsp = BioseqFind (sip);
                if (bsp != NULL) {
                  bcp = BioseqContextNew (bsp);
                  prot = BioseqContextGetSeqFeat (bcp, SEQFEAT_PROT, NULL, NULL, 0);
                  BioseqContextFree(bcp);
                  if (prot != NULL) {
                    prp = (ProtRefPtr) prot->data.value.ptrvalue;
                    if (prp != NULL) {
                      xref = SeqFeatXrefNew ();
                      if (xref != NULL) {
                        xref->data.choice = SEQFEAT_PROT;
                        xref->data.value.ptrvalue = AsnIoMemCopy (prp,
                                                      (AsnReadFunc) ProtRefAsnRead,
                                                      (AsnWriteFunc) ProtRefAsnWrite);
                        xref->next = newsfp->xref;
                        newsfp->xref = xref;
                      }
                    }
                  }
                }
              }
              newsfp->product = SeqLocFree (newsfp->product);
            }
          }
        }
      }
      break;
    default :
      break;
  }
  return TRUE;
}

static Boolean FetchGenesFromParts (Uint2 entityID, SeqEntryPtr nsep, BioseqPtr segbsp)

{
  GetGenesData  ggd;
  GatherScope   gs;
  Int2          num;
  ValNode       vn;

  if (nsep == NULL || segbsp == NULL || segbsp->repr != Seq_repr_seg) return FALSE;
  ggd.nsep = nsep;
  ggd.segbsp = segbsp;
  ggd.dirty = FALSE;
  ggd.count = 0;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 2;
  gs.nointervals = FALSE;
  gs.get_feats_location = TRUE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_BIOSEQ_DELTA] = FALSE;
  gs.ignore[OBJ_BIOSEQ_MAPFEAT] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  vn.choice = SEQLOC_WHOLE;
  vn.data.ptrvalue = (Pointer) segbsp->id;
  gs.target = &vn;
  num = get_seg_num ((SeqLocPtr) (segbsp->seq_ext));
  ggd.mon = MonitorIntNew ("Searching components", 0, (Int4) num);
  GatherEntity (entityID, (Pointer) &ggd, GetGenesOnParts, &gs);
  MonitorFree (ggd.mon);
  return ggd.dirty;
}

extern void ShowGeneList (ButtoN b)

{
  BioseqPtr      bsp;
  BioseqViewPtr  bvp;
  Uint2          entityID;
  FILE           *fp;
  ValNodePtr     g_list;
  SeqEntryPtr    nsep;
  Char           path [PATH_MAX];
  SeqEntryPtr    sep;
  CharPtr        str;
  ValNodePtr     vnp;
  ValNodePtr     vnpn;

  bvp = (BioseqViewPtr) GetObjectExtra (b);
  if (bvp == NULL) return;
  WatchCursor ();
  TmpNam (path);
  entityID = ObjMgrGetEntityIDForPointer (bvp->bsp);
  sep = GetTopSeqEntryForEntityID (entityID);
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL) return;
  bsp = (BioseqPtr) nsep->data.ptrvalue;
  if (bsp == NULL) return;

  if (bsp->repr == Seq_repr_seg && (! BioseqHasFeature (bsp))) {
    if (bvp->hasTargetControl) {
      if (FetchGenesFromParts (entityID, nsep, bsp)) {
        ObjMgrSetDirtyFlag (entityID, TRUE);
        ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);
      }
    }
  }

  g_list = BuildGeneList (sep);
  if (g_list != NULL) {
    g_list = SortValNode (g_list, CompareValNodeStrings);
    vnp = g_list;
    while (vnp != NULL) {
      vnpn = vnp->next;
      if (vnpn != NULL) {
        if (StringICmp ((CharPtr) vnp->data.ptrvalue, (CharPtr) vnpn->data.ptrvalue) == 0) {
          vnp->next = vnpn->next;
          vnpn->next = NULL;
          ValNodeFreeData (vnpn);
        } else {
          vnp = vnpn;
        }
      } else {
        vnp = vnpn;
      }
    }
    fp = FileOpen (path, "w");
    if (fp != NULL) {
      vnp = g_list;
      while (vnp != NULL) {
        str = (CharPtr) vnp->data.ptrvalue;
        if (str != NULL && StringLen (str) < 200) {
          fprintf (fp, "%s\n", str);
        }
        vnp = vnp->next;
      }
      FileClose (fp);
      DisplayGeneChoiceWindow (path, bvp);
    }
  }
  ValNodeFreeData (g_list);
  FileRemove (path);
  ArrowCursor ();
}

static void ShowMap (BioseqViewPtr bvp, Boolean show)

{
  if (bvp == NULL) return;
  if (show) {
    SafeShow (bvp->vwr);
    SafeShow (bvp->styleControlGrp);
    SafeHide (bvp->scaleControlGrp);
    EnableDisableLegendItem (bvp, FALSE);
    SafeShow (bvp->findGeneGrp);
    SafeHide (bvp->clickMe);
  } else {
    SafeHide (bvp->vwr);
    Reset (bvp->vwr);
    bvp->pict = DeletePicture (bvp->pict);
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

static void ShowCompressed (BioseqViewPtr bvp, Boolean show)

{
  if (bvp == NULL) return;
  if (show) {
    SafeShow (bvp->vwr);
    SafeHide (bvp->styleControlGrp);
    SafeHide (bvp->scaleControlGrp);
    EnableDisableLegendItem (bvp, FALSE);
    SafeHide (bvp->findGeneGrp);
    SafeShow (bvp->clickMe);
  } else {
    SafeHide (bvp->vwr);
    Reset (bvp->vwr);
    bvp->pict = DeletePicture (bvp->pict);
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

static void ShowGraphical (BioseqViewPtr bvp, Boolean show)

{
  if (bvp == NULL) return;
  if (show) {
    SafeShow (bvp->vwr);
    SafeShow (bvp->styleControlGrp);
    SafeShow (bvp->scaleControlGrp);
    EnableDisableLegendItem (bvp, TRUE);
    SafeHide (bvp->findGeneGrp);
    SafeShow (bvp->clickMe);
  } else {
    SafeHide (bvp->vwr);
    Reset (bvp->vwr);
    bvp->pict = DeletePicture (bvp->pict);
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

static void ShowAlignment (BioseqViewPtr bvp, Boolean show)

{
  if (bvp == NULL) return;
  if (show) {
    SafeShow (bvp->vwr);
    SafeShow (bvp->styleControlGrp);
    SafeShow (bvp->scaleControlGrp);
    EnableDisableLegendItem (bvp, FALSE);
    SafeHide (bvp->findGeneGrp);
    SafeShow (bvp->clickMe);
  } else {
    SafeHide (bvp->vwr);
    Reset (bvp->vwr);
    bvp->pict = DeletePicture (bvp->pict);
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

static void ShowSalsa (BioseqViewPtr bvp, Boolean show)

{
  if (bvp == NULL) return;
  if (show) {
    SafeShow (bvp->pnl);
    SafeShow (bvp->pnlParentGrp);
    SafeShow (bvp->styleControlGrp);
    SafeHide (bvp->scaleControlGrp);
    EnableDisableLegendItem (bvp, FALSE);
    SafeHide (bvp->findGeneGrp);
    SafeShow (bvp->clickMe);
  } else {
    SafeHide (bvp->pnlParentGrp);
    SafeHide (bvp->pnl);
    Reset (bvp->pnl);
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

static void ShowDesktop (BioseqViewPtr bvp, Boolean show)

{
  if (bvp == NULL) return;
  if (show) {
    SafeShow (bvp->vwr);
    SafeHide (bvp->styleControlGrp);
    SafeHide (bvp->scaleControlGrp);
    EnableDisableLegendItem (bvp, FALSE);
    SafeHide (bvp->findGeneGrp);
    SafeShow (bvp->clickMe);
  } else {
    SafeHide (bvp->vwr);
    Reset (bvp->vwr);
    bvp->pict = DeletePicture (bvp->pict);
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

typedef struct selectdata {
  VieweR        vwr;
  SelStructPtr  sel;
  Uint2         entityID;
  Uint2         itemID;
  Uint2         itemtype;
  SeqLocPtr     region;
} SelectData, PNTR SelectPtr;

static Boolean SelectPrim (SegmenT seg, PrimitivE prim, Uint2 segID,
                           Uint2 primID, Uint2 primCt, VoidPtr userdata)

{
  Uint2      entityID;
  Uint2      itemID;
  Uint2      itemtype;
  Int2       parID;
  SelectPtr  sp;

  sp = (SelectPtr) userdata;
  parID = SegmentID (ParentSegment (seg));
  entityID = segID;
  itemID = primID;
  itemtype = (Uint2) parID;
  if (entityID == sp->entityID &&
      itemID == sp->itemID &&
      itemtype == sp->itemtype) {
    HighlightPrimitive (sp->vwr, seg, prim, FRAME_PRIMITIVE);
  }
  return TRUE;
}

static Boolean DeselectPrim (SegmenT seg, PrimitivE prim, Uint2 segID,
                             Uint2 primID, Uint2 primCt, VoidPtr userdata)

{
  Uint2      entityID;
  Uint2      itemID;
  Uint2      itemtype;
  Int2       parID;
  SelectPtr  sp;

  sp = (SelectPtr) userdata;
  parID = SegmentID (ParentSegment (seg));
  entityID = segID;
  itemID = primID;
  itemtype = (Uint2) parID;
  if (entityID == sp->entityID &&
      itemID == sp->itemID &&
      itemtype == sp->itemtype) {
    HighlightPrimitive (sp->vwr, seg, prim, PLAIN_PRIMITIVE);
  }
  return TRUE;
}

static void SelectGraphical (BioseqViewPtr bvp, Uint2 selentityID, Uint2 selitemID,
                             Uint2 selitemtype, SeqLocPtr region,
                             Boolean select, Boolean scrollto)

{
  SelectData  sd;

  if (bvp == NULL) return;
  if (! bvp->highlightSelections) return;
  /* if (bvp->viewWholeEntity) return; */
  sd.vwr = bvp->vwr;
  sd.sel = ObjMgrGetSelected ();
  sd.entityID = selentityID;
  sd.itemID = selitemID;
  sd.itemtype = selitemtype;
  sd.region = region;
  if (select) {
    ExploreSegment (bvp->pict, (Pointer) &sd, SelectPrim);
  } else {
    ExploreSegment (bvp->pict, (Pointer) &sd, DeselectPrim);
  }
}

static Boolean InitSelectPrim (SegmenT seg, PrimitivE prim, Uint2 segID,
                               Uint2 primID, Uint2 primCt, VoidPtr userdata)

{
  Uint2         entityID;
  Uint2         itemID;
  Uint2         itemtype;
  Int2          parID;
  SelStructPtr  sel;
  SelectPtr     sp;

  sp = (SelectPtr) userdata;
  parID = SegmentID (ParentSegment (seg));
  entityID = segID;
  itemID = primID;
  itemtype = (Uint2) parID;
  for (sel = sp->sel; sel != NULL; sel = sel->next) {
    if (entityID == sel->entityID &&
        itemID == sel->itemID &&
        itemtype == sel->itemtype) {
      HighlightPrimitive (sp->vwr, seg, prim, FRAME_PRIMITIVE);
    }
  }
  return TRUE;
}

static void InitSelectGraphical (BioseqViewPtr bvp)

{
  SelectData  sd;

  if (bvp == NULL) return;
  if (! bvp->highlightSelections) return;
  /* if (bvp->viewWholeEntity) return; */
  sd.vwr = bvp->vwr;
  sd.sel = ObjMgrGetSelected ();
  sd.entityID = 0;
  sd.itemID = 0;
  sd.itemtype = 0;
  sd.region = NULL;
  ExploreSegment (bvp->pict, (Pointer) &sd, InitSelectPrim);
}

static void SelectSalsa (BioseqViewPtr bvp, Uint2 selentityID, Uint2 selitemID,
                         Uint2 selitemtype, SeqLocPtr region,
                         Boolean select, Boolean scrollto)

{
}

static Boolean VSMSelectProc (SegmenT seg, PrimitivE prim, Uint2 segid, Uint2 primID, Uint2 primct, Pointer data)

{
  Uint2      itemID;
  Uint2      itemtype;
  SelectPtr  sp;

  sp = (SelectPtr) data;
  itemID = primID;
  itemtype = segid;
  if (itemID == sp->itemID &&
      itemtype == sp->itemtype) {
    HighlightSegment (sp->vwr, seg, OUTLINE_SEGMENT);
  }
  return TRUE;
}

static Boolean VSMDeselectProc (SegmenT seg, PrimitivE prim, Uint2 segid, Uint2 primID, Uint2 primct, Pointer data)

{
  Uint2      itemID;
  Uint2      itemtype;
  SelectPtr  sp;

  sp = (SelectPtr) data;
  itemID = primID;
  itemtype = segid;
  if (itemID == sp->itemID &&
      itemtype == sp->itemtype) {
    HighlightSegment (sp->vwr, seg, PLAIN_SEGMENT);
  }
  return TRUE;
}

static void SelectDesktop (BioseqViewPtr bvp, Uint2 selentityID, Uint2 selitemID,
                           Uint2 selitemtype, SeqLocPtr region,
                           Boolean select, Boolean scrollto)

{
  SelectData  sd;

  if (bvp == NULL) return;
  if (! bvp->highlightSelections) return;
  /* if (bvp->viewWholeEntity) return; */
  sd.vwr = bvp->vwr;
  sd.sel = ObjMgrGetSelected ();
  sd.entityID = selentityID;
  sd.itemID = selitemID;
  sd.itemtype = selitemtype;
  sd.region = region;
  if (select) {
    ExploreSegment (bvp->pict, (Pointer) &sd, VSMSelectProc);
  } else {
    ExploreSegment (bvp->pict, (Pointer) &sd, VSMDeselectProc);
  }
}

static Boolean VSMInitSelectProc (SegmenT seg, PrimitivE prim, Uint2 segid, Uint2 primID, Uint2 primct, Pointer data)

{
  Uint2         itemID;
  Uint2         itemtype;
  SelStructPtr  sel;
  SelectPtr     sp;

  sp = (SelectPtr) data;
  itemID = primID;
  itemtype = segid;
  for (sel = sp->sel; sel != NULL; sel = sel->next) {
    if (itemID == sel->itemID &&
        itemtype == sel->itemtype) {
      HighlightSegment (sp->vwr, seg, OUTLINE_SEGMENT);
    }
  }
  return TRUE;
}

static void InitSelectDesktop (BioseqViewPtr bvp)

{
  SelectData  sd;

  if (bvp == NULL) return;
  if (! bvp->highlightSelections) return;
  /* if (bvp->viewWholeEntity) return; */
  sd.vwr = bvp->vwr;
  sd.sel = ObjMgrGetSelected ();
  sd.entityID = 0;
  sd.itemID = 0;
  sd.itemtype = 0;
  sd.region = NULL;
  ExploreSegment (bvp->pict, (Pointer) &sd, VSMInitSelectProc);
}

static void SelectRegion (VieweR viewer, SegmenT segment, PrimitivE primitive,
                          SeqLocPtr region, Uint2 entityID)

{
  BioseqPtr  bsp;
  RecT       d;
  ViewPData  extra;
  RecT       r;
  RecT       s;
  SegPPtr    seg;
  WindoW     tempPort;
  BoxInfo    pLimits;

  if (viewer == NULL || segment == NULL || primitive == NULL || region == NULL) return;
  seg = (SegPPtr) segment;
  if (seg->base.code != SEGMENT && seg->base.code != PICTURE) return;
  if (! seg->seg.visible) return;
  if (Visible (viewer) && AllParentsVisible (viewer)) {
    GetPanelExtra ((PaneL) viewer, &extra);
    TryGetPrimitiveLimits ((BasePPtr) primitive, extra.scale.scaleX,
                           extra.scale.scaleY, &pLimits);
    bsp = GetBioseqGivenSeqLoc (region, entityID);
    if (bsp == NULL) return;
    if (ABS (pLimits.left - pLimits.right) < bsp->length) return;
    pLimits.left = SeqLocStart (region);
    pLimits.right = SeqLocStop (region);
    if (! BoxInViewport (&r, &(pLimits), &(extra.scale))) return;
    InsetRect (&r, 0, -3);
    if (ABS (r.left - r.right) < 2) {
      InsetRect (&r, -1, 0);
    }
    ObjectRect (viewer, &s);
    InsetRect (&s, 2, 2);
    SectRect (&r, &s, &d);
    tempPort = SavePort (viewer);
    Select (viewer);
    Dotted ();
    FrameRect (&d);
    Solid ();
    RestorePort (tempPort);
  }
}

static Boolean DrawSelectionPrim (SegmenT seg, PrimitivE prim, Uint2 segID,
                                  Uint2 primID, Uint2 primCt, VoidPtr userdata)

{
  Uint2         entityID;
  Uint2         itemID;
  Uint2         itemtype;
  Int2          parID;
  SelStructPtr  sel;
  SelectPtr     sp;

  parID = SegmentID (ParentSegment (seg));
  itemtype = (Uint2) parID;
  if (itemtype != OBJ_BIOSEQ) return TRUE;
  sp = (SelectPtr) userdata;
  entityID = segID;
  itemID = primID;
  for (sel = sp->sel; sel != NULL; sel = sel->next) {
    if (entityID == sel->entityID &&
        itemID == sel->itemID &&
        itemtype == sel->itemtype &&
        sel->region != NULL) {
      SelectRegion (sp->vwr, seg, prim, sel->region, entityID);
    }
  }
  return TRUE;
}

static void DrawSelectionRange (VieweR viewer, SegmenT segment)

{
  Boolean       okay;
  SelectData    sd;
  SelStructPtr  sel;

  sd.vwr = viewer;
  sd.sel = ObjMgrGetSelected ();
  sd.entityID = 0;
  sd.itemID = 0;
  sd.itemtype = 0;
  sd.region = NULL;
  okay = FALSE;
  for (sel = sd.sel; sel != NULL; sel = sel->next) {
    if (sel->itemtype == OBJ_BIOSEQ && sel->region != NULL) {
      okay = TRUE;
    }
  }
  if (okay) {
    ExploreSegment (segment, (Pointer) &sd, DrawSelectionPrim);
  }
}


static void CopyGraphicalToClipboard (BioseqViewPtr bvp)

{
  if (bvp == NULL || bvp->vwr == NULL) return;
  CopyViewer (bvp->vwr);
}

#ifdef WIN_MOTIF
extern CharPtr Nlm_XrmGetResource (const Char PNTR _resource);
#endif

static void PrintGraphical (BioseqViewPtr bvp)

{
  MsgAnswer  ans;

  if (bvp == NULL || bvp->vwr == NULL) return;
#ifdef WIN_MOTIF
  Message (MSG_OK, "Printing of graphics under MOTIF is not supported at this time.");
#else
  ans = Message (MSG_YN, "Do you want to print just the visible area?");
  if (ans == ANS_YES) {
    PrintViewer (bvp->vwr);
  } else {
    PrintAllViewer (bvp->vwr);
  }
#endif
}

typedef struct expboundstruc {
  FILE       *file;
  ViewPData  extra;
  RecT       vwrct;
} ExpBound, PNTR ExpBoundPtr;

static Boolean ExpBoundCallback (SegmenT seg, PrimitivE prim, Uint2 segID,
                                 Uint2 primID, Uint2 primCt, VoidPtr userdata)

{
  ExpBoundPtr  ebp;
  Uint2        entityID;
  Uint2        itemID;
  Uint2        itemType;
  BoxInfo      pLimits;
  SegmenT      pseg;
  RecT         r;

  if (seg == NULL || prim == NULL || segID == 0 ||
      primID == 0 || userdata == NULL) return TRUE;
  ebp = (ExpBoundPtr) userdata;
  pseg = ParentSegment (seg);
  if (pseg == NULL) return TRUE;
  entityID = segID;
  itemID = primID;
  itemType = SegmentID (pseg);
  if (itemType == 0) return TRUE;
  if (TryGetPrimitiveLimits ((BasePPtr) prim, ebp->extra.scale.scaleX,
      ebp->extra.scale.scaleY, &pLimits)) {
    if (BoxInViewport (&r, &(pLimits), &(ebp->extra.scale))) {
      OffsetRect (&r, -ebp->vwrct.left, -ebp->vwrct.top);
      if (ebp->file != NULL) {
        fprintf (ebp->file, "%d %d %d %d %d %d %d\n", (int) entityID,
                 (int) itemID, (int) itemType,
                 (int) r.left, (int) r.top,
                 (int) r.right, (int) r.bottom);
      }
    }
  }
  return TRUE;
}

static void ExploreToGetBoundaries (VieweR vwr, SegmenT seg, CharPtr filename)

{
  ExpBound   exp;

  if (vwr == NULL || seg == NULL ||
      filename == NULL || *filename == '\0') return;
  GetPanelExtra ((PaneL) vwr, &(exp.extra));
  ObjectRect (vwr, &(exp.vwrct));
  InsetRect (&(exp.vwrct), 4, 4);
  exp.file = FileOpen (filename, "w");
  if (exp.file != NULL) {
    ExploreSegment (seg, &exp, ExpBoundCallback);
  }
  FileClose (exp.file);
}

static void GifGraphical (BioseqViewPtr bvp, CharPtr filename, CharPtr dfault)

{
  if (bvp == NULL || bvp->vwr == NULL) return;
  ExploreToGetBoundaries (bvp->vwr, bvp->pict, filename);
}

static void ExportSalsa (BioseqViewPtr bvp, CharPtr filename, CharPtr dfault)

{
  SaveSalsaPanel (bvp->pnl);
}

#define MIN_HEIGHT 100

static void ResizeMap (BioseqViewPtr bvp)

{
  Int2  height;
  Int2  oldstyle;
  RecT  r;
  Int2  style;

  if (bvp == NULL) return;
  if (bvp->vwr != NULL) {
    if (Visible (bvp->vwr) && AllParentsVisible (bvp->vwr)) {
      ObjectRect (ParentWindow (bvp->vwr), &r);
      height = r.bottom - r.top;
      if (height >= MIN_HEIGHT) {
        WatchCursor ();
        oldstyle = GetMuskCurrentSt ();
        style = GetValue (bvp->style) - 1;
        SetMuskCurrentSt (GetMuskStyleName (style));
        ObjectRect (bvp->vwr, &r);
        InsetRect (&r, 4, 4);
        bvp->pict = DeletePicture (bvp->pict);
        bvp->pict = GlobalPictureUpdate (bvp->gdraw_p, bvp->g_list,
                                         r.right - r.left, r.bottom - r.top);
        if (bvp->gdraw_p->is_circle) {
          AttachPicture (bvp->vwr, bvp->pict, 0, 0, UPPER_LEFT, 1, 1, NULL);
        } else {
          AttachPicture (bvp->vwr, bvp->pict, INT4_MIN, 0, UPPER_LEFT, 1, 1, NULL);
        }
        SetViewerProcs (bvp->vwr, ClickMap, DragMap, ReleaseMap, NULL);
        SetMuskCurrentSt (GetMuskStyleName (oldstyle));
        ArrowCursor ();
      }
    }
  }
}

static void ResizeCompressed (BioseqViewPtr bvp)

{
  BioseqPtr  bsp;
  ErrSev     oldErrSev;

  if (bvp == NULL) return;
  if (bvp->vwr != NULL) {
    if (Visible (bvp->vwr) && AllParentsVisible (bvp->vwr)) {
      Reset (bvp->vwr);
      bvp->pict = DeletePicture (bvp->pict);
      bsp = bvp->bsp;
      oldErrSev = ErrSetMessageLevel (SEV_FATAL);
      BioseqLock (bsp);
      PopulateCompressed (bvp);
      BioseqUnlock (bsp);
      ErrSetMessageLevel (oldErrSev);
    }
  }
}

static void ResizeGraphical (BioseqViewPtr bvp)

{
  if (bvp == NULL) return;
  if (bvp->vwr != NULL) {
    if (Visible (bvp->vwr) && AllParentsVisible (bvp->vwr)) {
      ViewerWasResized (bvp->vwr);
    }
  }
}

static void ResizeSalsa (BioseqViewPtr bvp)

{
  if (bvp == NULL) return;
  if (bvp->pnl != NULL) {
    if (Visible (bvp->pnl) && AllParentsVisible (bvp->pnl)) {
      SalsaPanelHasResized (bvp->pnl);
    }
  }
}

static void ResizeDesktop (BioseqViewPtr bvp)

{
  if (bvp == NULL) return;
  if (bvp->vwr != NULL) {
    if (Visible (bvp->vwr) && AllParentsVisible (bvp->vwr)) {
      ViewerWasResized (bvp->vwr);
    }
  }
}

BioseqPageData mapPageData = {
  "Map", FALSE, FALSE, TRUE, FALSE, -1,
  PopulateMap, ShowMap, SelectGraphical,
  CopyGraphicalToClipboard, PrintGraphical,
  NULL, NULL, ResizeMap, NULL
};

BioseqPageData sumPageData = {
  "Summary", TRUE, FALSE, FALSE, FALSE, -1,
  PopulateCompressed, ShowCompressed, SelectGraphical,
  CopyGraphicalToClipboard, PrintGraphical,
  NULL, NULL, ResizeCompressed, NULL
};

BioseqPageData gphPageData = {
  "Graphic", TRUE, TRUE, TRUE, FALSE, -1,
  PopulateGraphic, ShowGraphical, SelectGraphical,
  CopyGraphicalToClipboard, PrintGraphical,
  NULL, GifGraphical, ResizeGraphical, NULL
};

BioseqPageData alnPageData = {
  "Alignment", TRUE, TRUE, TRUE, TRUE, -1,
  PopulateAlignment, ShowAlignment, SelectGraphical,
  CopyGraphicalToClipboard, PrintGraphical,
  NULL, NULL, ResizeGraphical, NULL
};

BioseqPageData seqPageData = {
  "Sequence", TRUE, TRUE, FALSE, FALSE, -1,
  PopulateSalsa, ShowSalsa, SelectSalsa,
  NULL, NULL,
  ExportSalsa, NULL, ResizeSalsa, NULL
};

BioseqPageData dskPageData = {
  "Desktop", TRUE, TRUE, TRUE, FALSE, -1,
  PopulateDesktop, ShowDesktop, SelectDesktop,
  CopyGraphicalToClipboard, PrintGraphical,
  NULL, NULL, ResizeDesktop, NULL
};
