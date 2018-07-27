#include <objseq.h>
#include <seqport.h>
#include <gather.h>
#include <picture.h>
#include <viewer.h>
#include <seqgrphx.h>
#include <seqgraph.h>
#include <seqfltr.h>
#include <seqmtrx.h>
#include <seqpcc.h>
#include <aacomp.h>
#include <seqanal.h>

/* functions - internal */

static Boolean UnHLSeg (SegmenT seg, PrimitivE prim, Uint2 segID,
                        Uint2 primID, Uint2 primCt, Pointer data)
{
  VieweR v;

  v = (VieweR) data;
  HighlightPrimitive (v, seg, prim, PLAIN_PRIMITIVE);
  return TRUE;
}

/* functions - message */

static Int2 LIBCALLBACK MatrixMsgFunc (OMMsgStructPtr ommsp)
{
  OMUserDataPtr         omudp;
  GraphViewFormPtr      gvp;
  SeqGraphPtr           sgp;
  VieweR                v;
  PoinT                 pt;
  PntInfo               pw;
  Uint2                 segID, primID, primCt;
  SegmenT               s;
  PrimitivE             p;
  Int4                  pos;
  FloatHi               maxVal;

  if ((omudp = (OMUserDataPtr) (ommsp->omuserdata)) == NULL)
    return OM_MSG_RET_ERROR;
  if ((gvp = (GraphViewFormPtr) (omudp->userdata.ptrvalue)) == NULL)
    return OM_MSG_RET_ERROR;

  switch (ommsp->message)
  {
   case OM_MSG_SELECT:
    if (ommsp->itemtype == OBJ_BIOSEQ)
    {
      if (ommsp->regiontype == OM_REGION_SEQLOC)
      {
        if ((sgp = gvp->sgp) == NULL)
          return OM_MSG_RET_ERROR;

        switch (sgp->flags[2])
        {
         default:
         case 1:
          maxVal = sgp->max.realvalue;
          break;
         case 2:
         case 3:
          maxVal = (FloatHi) sgp->max.intvalue;
          break;
        }

        if (gvp->slp != NULL)
        {
          v = gvp->viewer;
          pt.x = 0;
          pt.y = 0;
          s = FindSegment (v, pt, &segID, &primID, &primCt);
          ExploreSegment (s, v, UnHLSeg);
/*
          ObjMgrDeSelectAll ();
          gvp->slp = NULL;
*/
        }
        pos = SeqLocStart ((SeqLocPtr) ommsp->region);
        pos += SeqLocStop ((SeqLocPtr) ommsp->region);
        pos /= 2;
        pos /= gvp->zoom;
        pw.x = (Int2) pos;
        if (SeqLocStrand ((SeqLocPtr) ommsp->region) == Seq_strand_plus)
        {
          pw.y = (Int2) (gvp->margin + ((Int4) (1.0 +
                               (maxVal/2)) * sgp->a) +
                               (Int4) sgp->b);
        }
        else
        {
          pw.y = (Int2) (gvp->margin + ((Int4) (maxVal - 1.0 -
                               (maxVal/2)) * sgp->a) +
                               (Int4) sgp->b);
        }
        v = gvp->viewer;
        primID = 0;
        MapWorldToViewer (v, pw, &pt);
        s = FindSegment (v, pt, &segID, &primID, &primCt);
        if (primID < 1 || s == NULL)
          return OM_MSG_RET_ERROR;
        p = GetPrimitive (s, primCt);
        WatchCursor ();
        ExploreSegment (s, v, UnHLSeg);
        ArrowCursor ();
        HighlightPrimitive (v, s, p, FRAME_PRIMITIVE);
/* DUP/memcp and delta id
        gvp->slp = (Pointer) (ommsp->region);
*/
/*
        ObjMgrSelect (gvp->entityID, gvp->itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC,
                      gvp->slp);
*/
      }
    }
    break;
   case OM_MSG_DESELECT:
    if (ommsp->itemtype == OBJ_BIOSEQ)
    {
      if (ommsp->regiontype == OM_REGION_SEQLOC)
      {
        if (gvp->slp != NULL)
        {
/*
          v = gvp->viewer;
          pt.x = 0;
          pt.y = 0;
          s = FindSegment (v, pt, &segID, &primID, &primCt);
          ExploreSegment (s, v, UnHLSeg);
          ObjMgrDeSelectAll ();
          gvp->slp = NULL;
*/
        }
      }
    }
    break;
   case OM_MSG_DEL:
   case OM_MSG_CREATE:
   case OM_MSG_UPDATE:
   case OM_MSG_CACHED:
   case OM_MSG_UNCACHED:
   case OM_MSG_TO_CLIPBOARD:
    break;
   default:
    break;
  }
  return OM_MSG_RET_OK;
}

static Int2 LIBCALLBACK FilterMsgFunc (OMMsgStructPtr ommsp)
{
  OMUserDataPtr         omudp;
  GraphViewFormPtr      gvp;
  SeqGraphPtr           sgp;
  VieweR                v;
  PoinT                 pt;
  PntInfo               pw;
  Uint2                 segID, primID, primCt, start, end, i;
  SegmenT               s;
  PrimitivE             p;
  FloatHi               fval;
  Int4                  pos;
  FloatHi               maxVal, minVal;

  if ((omudp = (OMUserDataPtr) (ommsp->omuserdata)) == NULL)
    return OM_MSG_RET_ERROR;
  if ((gvp = (GraphViewFormPtr) (omudp->userdata.ptrvalue)) == NULL)
    return OM_MSG_RET_ERROR;

  switch (ommsp->message)
  {
   case OM_MSG_SELECT:
    if (ommsp->itemtype == OBJ_BIOSEQ)
    {
      if (ommsp->regiontype == OM_REGION_SEQLOC)
      {
        if ((sgp = gvp->sgp) == NULL)
          return OM_MSG_RET_ERROR;

        switch (sgp->flags[2])
        {
         default:
         case 1:
          maxVal = sgp->max.realvalue;
          minVal = sgp->min.realvalue;
          break;
         case 2:
         case 3:
          maxVal = (FloatHi) sgp->max.intvalue;
          minVal = (FloatHi) sgp->min.intvalue;
          break;
        }

        if (gvp->slp != NULL)
        {
          v = gvp->viewer;
          pt.x = 0;
          pt.y = 0;
          s = FindSegment (v, pt, &segID, &primID, &primCt);
          ExploreSegment (s, v, UnHLSeg);
/*
          ObjMgrDeSelectAll ();
          gvp->slp = NULL;
*/
        }
        pos = SeqLocStart ((SeqLocPtr) ommsp->region);
        pos /= gvp->zoom;
        pw.x = (Int2) pos;
        v = gvp->viewer;
        primID = 0;
        fval = minVal - 1;
        while (primID < 1 && fval < maxVal)
        {
          fval++;
          pw.y = (Int2) (gvp->margin + ((Int4) (fval * sgp->a)) +
                         (Int4) sgp->b);
          MapWorldToViewer (v, pw, &pt);
          s = FindSegment (v, pt, &segID, &primID, &primCt);
        }
        if (primCt == 0 && fval == maxVal)
          return OM_MSG_RET_ERROR;
        if (s == NULL)
          return OM_MSG_RET_ERROR;
        start = primCt;

        pos = SeqLocStop ((SeqLocPtr) ommsp->region);
        pos /= gvp->zoom;
        pw.x = (Int2) pos;
        primID = 0;
        fval = minVal - 1;
        while (primID < 1 && fval < maxVal)
        {
          fval++;
          pw.y = (Int2) (gvp->margin + ((Int4) (fval * sgp->a)) +
                         (Int4) sgp->b);
          MapWorldToViewer (v, pw, &pt);
          s = FindSegment (v, pt, &segID, &primID, &primCt);
        }
        if (primCt == 0 || fval == maxVal)
          return OM_MSG_RET_ERROR;
        if (s == NULL)
          return OM_MSG_RET_ERROR;
        end = primCt;
        if (start > end)
          return OM_MSG_RET_ERROR;
        WatchCursor ();
        ExploreSegment (s, v, UnHLSeg);
        ArrowCursor ();
        for (i = start; i < end; i++)
        {
          p = GetPrimitive (s, i);
          HighlightPrimitive (v, s, p, FRAME_PRIMITIVE);
        }
/* DUP/memcp and delta id
        gvp->slp = (Pointer) (ommsp->region);
*/
/*
        ObjMgrSelect (gvp->entityID, gvp->itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC,
                      gvp->slp);
*/
      }
    }
    break;
   case OM_MSG_DESELECT:
    if (ommsp->itemtype == OBJ_BIOSEQ)
    {
      if (ommsp->regiontype == OM_REGION_SEQLOC)
      {
        if (gvp->slp != NULL)
        {
/*
          v = gvp->viewer;
          pt.x = 0;
          pt.y = 0;
          s = FindSegment (v, pt, &segID, &primID, &primCt);
          ExploreSegment (s, v, UnHLSeg);
          ObjMgrDeSelectAll ();
          gvp->slp = NULL;
*/
        }
      }
    }
    break;
   case OM_MSG_DEL:
   case OM_MSG_CREATE:
   case OM_MSG_UPDATE:
   case OM_MSG_CACHED:
   case OM_MSG_UNCACHED:
   case OM_MSG_TO_CLIPBOARD:
    break;
   default:
    break;
  }
  return OM_MSG_RET_OK;
}

/* functions - cleanup */

static void CleanupGCFilterForm  (GraphiC g, VoidPtr data)
{
  GraphViewFormPtr      gvp;

  gvp = (GraphViewFormPtr) data;
  if (gvp != NULL)
  {
    gvp = GraphViewFormFree (gvp, FALSE);
    if (gvp->entityID > 0)
      ObjMgrFreeUserData (gvp->entityID, gvp->procID, OMPROC_FILTER,
                          gvp->userKEY);
  }
  StdCleanupFormProc (g, data);
  return;
}

/* functions - hooks */

Int2 LIBCALLBACK FilterCompGCFunc (Pointer data)
{
  OMProcControlPtr      ompcp;
  BioseqPtr             bsp;
  GraphViewFormPtr      gvp;
  WindoW                w;
  OMUserDataPtr         omudp;
  SeqPortPtr            spp;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
    case OBJ_BIOSEQ:
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    default:
      return OM_MSG_RET_ERROR;
  }

  if (bsp != NULL)
  {
    if (!ISA_na (bsp->mol))
      return OM_MSG_RET_ERROR;
    w = (WindoW) CreateGraphViewForm (-50, -33, "Filter Analysis",
                                      bsp, GRAPH_FILTER);
  }
  else
  {
    return OM_MSG_RET_ERROR;
  }

  if ((gvp = (GraphViewFormPtr) GetObjectExtra (w)) == NULL)
  {
/*    w = Remove (w); */
    return OM_MSG_RET_ERROR;
  }
  else
  {
    SetObjectExtra (w, gvp, CleanupGCFilterForm);
    gvp->graphtype = GRAPH_FILTER;
    gvp->window = 100;
    gvp->type = NA_FILTER_GC;
    gvp->entityID = ompcp->input_entityID;
    gvp->itemID = ompcp->input_itemID;
    gvp->procID = ompcp->proc->procid;
/* msg stuff */
    gvp->userKEY = OMGetNextUserKey ();
    if ((omudp = ObjMgrAddUserData (gvp->entityID, gvp->procID,
                                    OMPROC_FILTER, gvp->userKEY)) != NULL)
    {
      omudp->userdata.ptrvalue = (Pointer) gvp;
      omudp->messagefunc = FilterMsgFunc;
/*      omudp->messagefunc = NULL; */
    }
    WatchCursor ();
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacna);
    gvp->sgp = FilterSeq (spp, 0, bsp->length-1, NULL, NULL,
                          &(gvp->window), gvp->type);
    SeqPortFree (spp);
    if (gvp->sgp == NULL)
    {
/*      w = Remove (w); */
      ArrowCursor ();
      return OM_MSG_RET_ERROR;
    }
    else
    {
      BioseqPtrToGraphViewForm (gvp->form, gvp->sgp);
    }
  }
/* put the screen up */
  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}

Int2 LIBCALLBACK FilterDustFunc (Pointer data)
{
  OMProcControlPtr      ompcp;
  BioseqPtr             bsp;
  GraphViewFormPtr      gvp;
  WindoW                w;
  OMUserDataPtr         omudp;
  SeqPortPtr            spp;
  Int4                  exval[4];

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
    case OBJ_BIOSEQ:
      bsp = (BioseqPtr) ompcp->input_data;
      break;
    default:
      return OM_MSG_RET_ERROR;
  }

  if (bsp != NULL)
  {
    if (!ISA_na (bsp->mol))
      return OM_MSG_RET_ERROR;
    w = (WindoW) CreateGraphViewForm (-50, -33, "Dust Filter",
                                      bsp, GRAPH_FILTER);
  }
  else
  {
    return OM_MSG_RET_ERROR;
  }

  if ((gvp = (GraphViewFormPtr) GetObjectExtra (w)) == NULL)
  {
/*    w = Remove (w); */
    return OM_MSG_RET_ERROR;
  }
  else
  {
    SetObjectExtra (w, gvp, CleanupGCFilterForm);
    gvp->graphtype = GRAPH_FILTER;
    gvp->window = 64;
    exval[0] = 64;
    exval[1] = 20;
    exval[2] = 4;
    exval[3] = 1;
    gvp->type = NA_FILTER_DUST;
    gvp->entityID = ompcp->input_entityID;
    gvp->itemID = ompcp->input_itemID;
    gvp->procID = ompcp->proc->procid;
/* msg stuff */
    gvp->userKEY = OMGetNextUserKey ();
    if ((omudp = ObjMgrAddUserData (gvp->entityID, gvp->procID,
                                    OMPROC_FILTER, gvp->userKEY)) != NULL)
    {
      omudp->userdata.ptrvalue = (Pointer) gvp;
      omudp->messagefunc = FilterMsgFunc;
/*      omudp->messagefunc = NULL; */
    }
    WatchCursor ();
    spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_ncbi2na);
    gvp->sgp = FilterSeq (spp, 0, bsp->length-1, NULL, NULL,
                          exval, gvp->type);
    SeqPortFree (spp);
    if (gvp->sgp == NULL)
    {
/*      w = Remove (w); */
      ArrowCursor ();
      return OM_MSG_RET_ERROR;
    }
    else
    {
      BioseqPtrToGraphViewForm (gvp->form, gvp->sgp);
    }
  }
/* put the screen up */
  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}

Int2 LIBCALLBACK PCCPredictFunc (Pointer data)
{
  OMProcControlPtr      ompcp;
  BioseqPtr             bsp = NULL;
  SeqFeatPtr            sfp = NULL;
  WindoW                w;
  GraphViewFormPtr      gvp;
  SeqIdPtr              psip;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
    case OBJ_BIOSEQ:
      bsp = (BioseqPtr) ompcp->input_data;
      if (!ISA_aa (bsp->mol))
        return OM_MSG_RET_ERROR;
      break;
    case OBJ_SEQFEAT:
      sfp = (SeqFeatPtr) ompcp->input_data;
      break;
    default:
      return OM_MSG_RET_ERROR;
  }

  if (bsp != NULL)
  {
    w = (WindoW) CreateGraphViewForm (-50, -33, "Predict coiled-coil",
                                      bsp, GRAPH_FILTER);

    if ((gvp = (GraphViewFormPtr) GetObjectExtra (w)) == NULL)
    {
/*      w = Remove (w); */
      return OM_MSG_RET_ERROR;
    }
    else
    {
      gvp->graphtype = GRAPH_FILTER;
      gvp->window = 22;
      gvp->type = AA_PCC;
      gvp->entityID = ompcp->input_entityID;
      gvp->itemID = ompcp->input_itemID;
      if ((gvp->sgp = PCCProc (bsp, NULL, gvp->window)) == NULL)
      {
/*        w = Remove (w); */
        return OM_MSG_RET_ERROR;
      }
      else
      {
        BioseqPtrToGraphViewForm (gvp->form, gvp->sgp);
      }
    }
  }
  else if (sfp != NULL)
  {
    if (sfp->data.choice != SEQFEAT_CDREGION)
      return OM_MSG_RET_ERROR;
    psip = SeqLocId (sfp->product);
    bsp = BioseqFind (psip);
    w = (WindoW) CreateGraphViewForm (-50, -33, "Predict coiled-coil",
                                      bsp, GRAPH_FILTER);

    if ((gvp = (GraphViewFormPtr) GetObjectExtra (w)) == NULL)
    {
/*      w = Remove (w); */
      return OM_MSG_RET_ERROR;
    }
    else
    {
      gvp->graphtype = GRAPH_FILTER;
      gvp->window = 22;
      gvp->type = AA_PCC;
      gvp->entityID = ompcp->input_entityID;
      gvp->itemID = ompcp->input_itemID;
      if ((gvp->sgp = PCCProc (bsp, NULL, gvp->window)) == NULL)
      {
/*        w = Remove (w); */
        return OM_MSG_RET_ERROR;
      }
      else
      {
        BioseqPtrToGraphViewForm (gvp->form, gvp->sgp);
      }
    }
  }
  else
  {
    return OM_MSG_RET_ERROR;
  }

  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}

Int2 LIBCALLBACK HydrophilicFunc (Pointer data)
{
  OMProcControlPtr      ompcp;
  BioseqPtr             bsp = NULL;
  SeqFeatPtr            sfp = NULL;
  WindoW                w;
  GraphViewFormPtr      gvp;
  SeqIdPtr              psip;
  SeqPortPtr            spp;
  FloatHi               scr[24];
  Char                  res[24];

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
    case OBJ_BIOSEQ:
      bsp = (BioseqPtr) ompcp->input_data;
      if (!ISA_aa (bsp->mol))
        return OM_MSG_RET_ERROR;
      break;
    case OBJ_SEQFEAT:
      sfp = (SeqFeatPtr) ompcp->input_data;
      break;
    default:
      return OM_MSG_RET_ERROR;
  }

  if (bsp != NULL)
  {
    w = (WindoW) CreateGraphViewForm (-50, -33, "Hopp-Woods-philicity",
                                      bsp, GRAPH_FILTER);

    if ((gvp = (GraphViewFormPtr) GetObjectExtra (w)) == NULL)
    {
/*      w = Remove (w); */
      return OM_MSG_RET_ERROR;
    }
    else
    {
      gvp->graphtype = GRAPH_FILTER;
      if (ReadAAC ("KShopp.flt", scr, res) != 24)
        return OM_MSG_RET_ERROR;
      gvp->window = 6;
      gvp->type = AA_FILTER_COMP_HOPP;
      gvp->entityID = ompcp->input_entityID;
      gvp->itemID = ompcp->input_itemID;
      spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacaa);
      gvp->sgp = FilterSeq (spp, 0, bsp->length-1, scr, res,
                            &(gvp->window), gvp->type);
      SeqPortFree (spp);
      if (gvp->sgp == NULL)
      {
/*        w = Remove (w); */
        return OM_MSG_RET_ERROR;
      }
      else
      {
        BioseqPtrToGraphViewForm (gvp->form, gvp->sgp);
      }
    }
  }
  else if (sfp != NULL)
  {
    if (sfp->data.choice != SEQFEAT_CDREGION)
      return OM_MSG_RET_ERROR;
    psip = SeqLocId (sfp->product);
    bsp = BioseqFind (psip);
    w = (WindoW) CreateGraphViewForm (-50, -33, "Hopp-Woods-philicity",
                                      bsp, GRAPH_FILTER);

    if ((gvp = (GraphViewFormPtr) GetObjectExtra (w)) == NULL)
    {
/*      w = Remove (w); */
      return OM_MSG_RET_ERROR;
    }
    else
    {
      gvp->graphtype = GRAPH_FILTER;
      if (ReadAAC ("KShopp.flt", scr, res) != 24)
        return OM_MSG_RET_ERROR;
      gvp->window = 6;
      gvp->type = AA_FILTER_COMP_HOPP;
      gvp->entityID = ompcp->input_entityID;
      gvp->itemID = ompcp->input_itemID;
      spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacaa);
      gvp->sgp = FilterSeq (spp, 0, bsp->length-1, scr, res,
                            &(gvp->window), gvp->type);
      SeqPortFree (spp);
      if (gvp->sgp == NULL)
      {
/*        w = Remove (w); */
        return OM_MSG_RET_ERROR;
      }
      else
      {
        BioseqPtrToGraphViewForm (gvp->form, gvp->sgp);
      }
    }
  }
  else
  {
    return OM_MSG_RET_ERROR;
  }

  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}

Int2 LIBCALLBACK HydrophobicFunc (Pointer data)
{
  OMProcControlPtr      ompcp;
  BioseqPtr             bsp = NULL;
  SeqFeatPtr            sfp = NULL;
  WindoW                w;
  GraphViewFormPtr      gvp;
  SeqIdPtr              psip;
  SeqPortPtr            spp;
  FloatHi               scr[24];
  Char                  res[24];

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
    case OBJ_BIOSEQ:
      bsp = (BioseqPtr) ompcp->input_data;
      if (!ISA_aa (bsp->mol))
        return OM_MSG_RET_ERROR;
      break;
    case OBJ_SEQFEAT:
      sfp = (SeqFeatPtr) ompcp->input_data;
      break;
    default:
      return OM_MSG_RET_ERROR;
  }

  if (bsp != NULL)
  {
    w = (WindoW) CreateGraphViewForm (-50, -33, "Kyte-Doolittle-phobicity",
                                      bsp, GRAPH_FILTER);

    if ((gvp = (GraphViewFormPtr) GetObjectExtra (w)) == NULL)
    {
/*      w = Remove (w); */
      return OM_MSG_RET_ERROR;
    }
    else
    {
      gvp->graphtype = GRAPH_FILTER;
      if (ReadAAC ("KSkyte.flt", scr, res) != 24)
        return OM_MSG_RET_ERROR;
      gvp->window = 19;
      gvp->type = AA_FILTER_COMP_KYTE;
      gvp->entityID = ompcp->input_entityID;
      gvp->itemID = ompcp->input_itemID;
      spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacaa);
      gvp->sgp = FilterSeq (spp, 0, bsp->length-1, scr, res,
                            &(gvp->window), gvp->type);
      SeqPortFree (spp);
      if (gvp->sgp == NULL)
      {

/*        w = Remove (w); */
        return OM_MSG_RET_ERROR;
      }
      else
      {
        BioseqPtrToGraphViewForm (gvp->form, gvp->sgp);
      }
    }
  }
  else if (sfp != NULL)
  {
    if (sfp->data.choice != SEQFEAT_CDREGION)
      return OM_MSG_RET_ERROR;
    psip = SeqLocId (sfp->product);
    bsp = BioseqFind (psip);
    w = (WindoW) CreateGraphViewForm (-50, -33, "Kyte-Doolittle-phobicity",
                                      bsp, GRAPH_FILTER);

    if ((gvp = (GraphViewFormPtr) GetObjectExtra (w)) == NULL)
    {
/*      w = Remove (w); */
      return OM_MSG_RET_ERROR;
    }
    else
    {
      gvp->graphtype = GRAPH_FILTER;
      if (ReadAAC ("KSkyte.flt", scr, res) != 24)
        return OM_MSG_RET_ERROR;
      gvp->window = 19;
      gvp->type = AA_FILTER_COMP_KYTE;
      gvp->entityID = ompcp->input_entityID;
      gvp->itemID = ompcp->input_itemID;
      spp = SeqPortNew (bsp, 0, bsp->length-1, 0, Seq_code_iupacaa);
      gvp->sgp = FilterSeq (spp, 0, bsp->length-1, scr, res,
                            &(gvp->window), gvp->type);
      SeqPortFree (spp);
      if (gvp->sgp == NULL)
      {
/*        w = Remove (w); */
        return OM_MSG_RET_ERROR;
      }
      else
      {
        BioseqPtrToGraphViewForm (gvp->form, gvp->sgp);
      }
    }
  }
  else
  {
    return OM_MSG_RET_ERROR;
  }

  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}
/*
Int2 LIBCALLBACK SeqAnalFunc (Pointer data)
{
  OMProcControlPtr      ompcp;
  BioseqPtr             bsp = NULL;
  SeqFeatPtr            sfp = NULL;
  WindoW                w;
  SeqAnalFormPtr        sqap;
  SeqIdPtr              psip;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->input_itemtype == 0)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
   case OBJ_BIOSEQ:
    bsp = (BioseqPtr) ompcp->input_data;
    break;
   case OBJ_SEQFEAT:
    sfp = (SeqFeatPtr) ompcp->input_data;
    break;
   default:
    return OM_MSG_RET_ERROR;
  }

  if (sfp != NULL)
  {
    if (sfp->data.choice != SEQFEAT_CDREGION)
      return OM_MSG_RET_ERROR;
    if ((psip = SeqLocId (sfp->product)) == NULL)
      return OM_MSG_RET_ERROR;
    if ((bsp = BioseqFind (psip)) == NULL)
      return OM_MSG_RET_ERROR;
  }

  if (bsp != NULL)
  {
    w = (WindoW) CreateSeqAnalForm (-50, -33, "Sequence Analysis", bsp);
    if ((sqap = (SeqAnalFormPtr) GetObjectExtra (w)) == NULL)
    {
      return OM_MSG_RET_ERROR;
    }
    else
    {
      if (sqap == NULL)
      {
        return OM_MSG_RET_ERROR;
      }
      else
      {
        BioseqPtrToSeqAnalForm (sqap->form, bsp);
      }
    }
  }
  else
  {
    return OM_MSG_RET_ERROR;
  }

  Show (w);
  Select (w);
  return OM_MSG_RET_DONE;
}
*/
