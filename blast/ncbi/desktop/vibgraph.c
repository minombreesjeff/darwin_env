#include <ncbi.h>
#include <gather.h>
#include <picture.h>
#include <viewer.h>

#include <urkgraph.h>
#include <vibgraph.h>

/*
 internal zoom
*/

#define ZOOMMAX 16

static void InitZoomPopUp (GraphViewFormPtr gvp)
{
  Char            buf[16];
  Int2            i, n, maxzoom;
  BaR             sb;

  if (gvp != NULL)
  {
    if (gvp->maxzoom != 0)
    {
      SafeHide (gvp->ZoomPopUp);
      Reset (gvp->ZoomPopUp);
      maxzoom = (Int2) gvp->maxzoom;

      n = 1;
      i = 2;
      while (n < maxzoom)
      {
        n *= 2;
        if (maxzoom == n)
          i = 1;
      }

      while (maxzoom > 0)
      {
        i++;
        maxzoom /= (Int2) 2;
      }
      if (i > ZOOMMAX)
        i = ZOOMMAX;

      maxzoom = i;
      for (i = 1; i < maxzoom; i++)
      {
        if (i == maxzoom-1)
        {
          gvp->zoom = gvp->maxzoom;
          sprintf (buf, "%ld", (long) gvp->zoom);
        }
        else
        {
          gvp->zoom = (Int2) pow (2, (i-1));
          sprintf (buf, "%ld", (long) gvp->zoom);
        }
        PopupItem (gvp->ZoomPopUp, buf);
      }
    }
    SetValue (gvp->ZoomPopUp, (Int2) (i-1));
    SafeShow (gvp->ZoomPopUp);
  }
  sb = GetSlateHScrollBar ((SlatE) gvp->v);
  CorrectBarMax (sb, 0);
  return;
}

static void ZoomGraphView (PopuP p)
{
  GraphViewFormPtr  gvp;
  GraphInfoPtr      gip;
  SeqGraphPtr       sgp;
  Int2              i;
  Int4              oldsbpos, oldsbmax;
  Int4              newsbmax, curatt;
  Int4              graphpixelwidth;
  Int4              horiz_offset;
  Int4              itemp;
  FloatHi           tempvalue1, tempvalue2;
  BaR               sb;

  gvp = (GraphViewFormPtr) GetObjectExtra (p);
  if (gvp == NULL)
    return;
  if ((gip = gvp->gip) == NULL)
    return;

  sb = GetSlateHScrollBar ((SlatE) gvp->v);
  oldsbpos = (Int4) GetValue (sb);
  oldsbmax = (Int4) GetBarMax (sb);

  i = GetValue (gvp->ZoomPopUp);
  if (i > 0)
  {
    if ((Int4) pow (2, (i-1)) < gvp->maxzoom)
      gvp->zoom = (Int4) pow (2, (i-1));
    else
      gvp->zoom = gvp->maxzoom;
  }
  else
  {
    i = 1;
    gvp->zoom = 1;
  }
  SetValue (gvp->ZoomPopUp, i);

  gvp->s = DeletePicture (gvp->s);
  WatchCursor ();
  while (gip != NULL)
  {
    tempvalue1 = (FloatHi) gvp->zoom * (FloatHi) gip->localmaxzoom /
                 (FloatHi) gvp->maxzoom;
    if (gvp->zoom == gvp->maxzoom)
    {
      graphpixelwidth = gip->graphpixelwidth;
      horiz_offset = gip->horiz_offset;
    }
    else
    {
      graphpixelwidth = (Int4) ((FloatHi) (gip->stopplot-gip->startplot+1) /
                        tempvalue1);
      itemp = (Int4) tempvalue1;
      if (tempvalue1 - (FloatHi) itemp >= 0.5)
        itemp++;
      tempvalue2 = (FloatHi) gip->horiz_offset * (FloatHi) gip->localmaxzoom;
      horiz_offset = (Int4) tempvalue2 / itemp;
    }

    if ((sgp = gip->sgp) != NULL)
    {
      if (gvp->zoom > gip->localmaxzoom)
      {
        sgp->compr = gip->localmaxzoom;
        sgp->flags[0] = 1;
      }
      else
      {
        sgp->compr = (Int4) tempvalue1;
        if (tempvalue1 - (FloatHi) sgp->compr >= 0.5)
          sgp->compr += 1;
        sgp->flags[0] = 1;
      }

      gvp->s = DrawGraphSGP (gvp->s, sgp, gip->graphtype,
                             graphpixelwidth, gip->graphpixelheight,
                             horiz_offset, gip->vert_offset);

      gvp->s = XscaleX (gvp->s, (Int4) sgp->compr,
                        (Int4) sgp->a, (Int4) sgp->b, 0, sgp->numval-1,
                        horiz_offset, gip->vert_offset);

      switch (sgp->flags[2])
      {
       default:
       case 1:
        gvp->s = YscaleX (gvp->s, (Int4) sgp->a, (Int4) sgp->b,
                          sgp->min.realvalue,
                          sgp->max.realvalue,
                          (Int4) sgp->compr, gip->graphpixelheight,
                          0, sgp->numval-1,
                          horiz_offset, gip->vert_offset);
       case 2:
       case 3:
        gvp->s = YscaleX (gvp->s, (Int4) sgp->a, (Int4) sgp->b,
                          (FloatHi) sgp->min.intvalue,
                          (FloatHi) sgp->max.intvalue,
                          (Int4) sgp->compr, gip->graphpixelheight,
                          0, sgp->numval-1,
                          horiz_offset, gip->vert_offset);
      }
    }
    gip = gip->next;
  }
  ArrowCursor ();

  itemp = 0;
  gip = gvp->gip;
  while (gip != NULL)
  {
    if (gip->localmaxzoom > itemp)
      itemp = gip->localmaxzoom;
    gip = gip->next;
  }
  curatt = INT4_MIN;
  if (oldsbmax > 0 && oldsbpos > 0)
  {
    curatt = ((gvp->maxpixelwidth * itemp / gvp->zoom) -
             gvp->maxpixelwidth) * oldsbpos / oldsbmax;
  }

  Reset (gvp->v);
  Update ();
  AttachPicture (gvp->v, gvp->s, curatt, INT4_MIN, UPPER_LEFT,
                 1, 1, NULL);
  sb = GetSlateHScrollBar ((SlatE) gvp->v);
  newsbmax = (Int4) GetBarMax (sb);
  if (oldsbmax > 0)
  {
    tempvalue1 = (FloatHi) newsbmax * (FloatHi) oldsbpos / (FloatHi) oldsbmax;
    itemp = (Int4) tempvalue1;
    if (tempvalue1 - (FloatHi) itemp >= 0.5)
      itemp++;
    CorrectBarValue (sb, (Int2) itemp);
  }
  else
  {
    CorrectBarValue (sb, 0);
  }
  return;
}

/*
 internal zoom max values
*/

static Int4 GetMaxGraphPixels (GraphInfoPtr gip)
{
  Int4 maxpixelwidth = 0;

  while (gip != NULL)
  {
    if (gip->horiz_offset + gip->graphpixelwidth > maxpixelwidth)
      maxpixelwidth = gip->horiz_offset + gip->graphpixelwidth;
    gip = gip->next;
  }
  return maxpixelwidth;
}

static Int4 GetMaxGraphZoom (GraphInfoPtr gip)
{
  Int4     maxzoom = 0;
  Int4     effectivezoom = 0;
  Boolean  flagMultiGraph = FALSE;

  while (gip != NULL)
  {
    if (gip->next != NULL)
      flagMultiGraph = TRUE;
    if (gip->localmaxzoom > maxzoom)
      maxzoom = gip->localmaxzoom;
    gip = gip->next;
  }
  if (flagMultiGraph)
  {
    while (maxzoom > 0)
    {
      effectivezoom++;
      maxzoom /= 2;
    }
    effectivezoom = (Int4) pow (2, effectivezoom);
  }
  else
  {
    effectivezoom = maxzoom;
  }

  return effectivezoom;
}

/*
 internal GraphViewForm callbacks
*/

static GraphViewFormPtr GraphViewFormNew (void)
{
  GraphViewFormPtr  gvp;

  if ((gvp = (GraphViewFormPtr) MemNew (sizeof (GraphViewForm)))
              != NULL)
  {
    gvp->v = NULL;
    gvp->s = NULL;
    gvp->sy = NULL;
    gvp->gip = NULL;
    gvp->maxzoom = 0;
    gvp->zoom = 0;
    gvp->maxpixelwidth = 0;
    gvp->ZoomPopUp = NULL;
    gvp->entityID = 0;
    gvp->itemID = 0;
    gvp->procID = 0;
    gvp->userKEY = 0;
  }
  return gvp;
}

static GraphViewFormPtr GraphViewFormFree (GraphViewFormPtr gvp)
{
  if (gvp != NULL)
  {
/*
    DeletePicture (gvp->s);
*/
    GraphInfoFree (gvp->gip);
    gvp = (GraphViewFormPtr) MemFree (gvp);
  }
  return gvp;
}

static void CleanupGraphViewForm (GraphiC g, VoidPtr data)
{
  GraphViewFormPtr gvp;

  if ((gvp = (GraphViewFormPtr) data) != NULL)
  {
    gvp = GraphViewFormFree (gvp);
    data = (VoidPtr) gvp;
  }
  StdCleanupFormProc (g, data);
  return;
}

static void CloseGraphViewForm (WindoW w)
{
  ObjMgrDeSelectAll ();
  Remove (w);
  return;
}

static void CloseGraphView (ButtoN b)
{
  WindoW           w;

  w = (WindoW) GetObjectExtra (b);
  CloseGraphViewForm (w);
  return;
}

static void ResizeGraphViewForm (WindoW w)
{
  Update ();
  Select (w);
  return;
}

/*
 externals
*/

extern void GraphInfoPtrToGraphViewForm (ForM f, Pointer data)
{
  GraphViewFormPtr gvp;
  GraphInfoPtr     gip, giph;
  SeqGraphPtr      sgp;
  GrouP            g1, h1;
  ButtoN           b1;

  gvp = (GraphViewFormPtr) GetObjectExtra (f);
  giph = gip = (GraphInfoPtr) data;

  if (gvp != NULL && gip != NULL)
  {
    while (gip != NULL)
    {
      if ((sgp = gip->sgp) != NULL)
      {
        gvp->s = DrawGraphSGP (gvp->s, sgp, gip->graphtype,
                               gip->graphpixelwidth, gip->graphpixelheight,
                               gip->horiz_offset, gip->vert_offset);
        gvp->s = XscaleX (gvp->s, (Int4) sgp->compr,
                          (Int4) sgp->a, (Int4) sgp->b, 0, sgp->numval-1,
                          gip->horiz_offset, gip->vert_offset);

        switch (sgp->flags[2])
        {
         default:
         case 1:
          gvp->s = YscaleX (gvp->s, (Int4) sgp->a, (Int4) sgp->b,
                            sgp->min.realvalue,
                            sgp->max.realvalue,
                            (Int4) sgp->compr, gip->graphpixelheight,
                            0, sgp->numval-1,
                            gip->horiz_offset, gip->vert_offset);
         case 2:
         case 3:
          gvp->s = YscaleX (gvp->s, (Int4) sgp->a, (Int4) sgp->b,
                            (FloatHi) sgp->min.intvalue,
                            (FloatHi) sgp->max.intvalue,
                            (Int4) sgp->compr, gip->graphpixelheight,
                            0, sgp->numval-1,
                            gip->horiz_offset, gip->vert_offset);
        }
      }
      gip = gip->next;
    }
    gvp->maxpixelwidth = GetMaxGraphPixels (giph);
    gvp->maxzoom = GetMaxGraphZoom (giph);
    g1 = HiddenGroup ((WindoW) gvp->form, 1, 0, NULL);
    gvp->v = CreateViewer (g1, 600, 200, FALSE, TRUE);
    AttachPicture (gvp->v, gvp->s, INT4_MIN, INT4_MIN, UPPER_LEFT,
                   1, 1, NULL);
    h1 = HiddenGroup (g1, 2, 0, NULL);
    gvp->ZoomPopUp = PopupList (h1, TRUE, ZoomGraphView);
    SetObjectExtra (gvp->ZoomPopUp, gvp, NULL);
    InitZoomPopUp (gvp);
  }
  b1 = PushButton (h1, "Close", CloseGraphView);
  SetObjectExtra (b1, (WindoW) gvp->form, NULL);
  RealizeWindow ((WindoW) gvp->form);
  return;
}

extern ForM xCreateGraphViewForm (Int2 left, Int2 top, CharPtr title)
{
  WindoW           w = NULL;
  GraphViewFormPtr gvp;

  if ((gvp = GraphViewFormNew ()) != NULL)
  {
    if ((w = DocumentWindow (left, top, -10, -10, title,
                             CloseGraphViewForm,
                             ResizeGraphViewForm)) != NULL)
    {
      SetObjectExtra (w, gvp, CleanupGraphViewForm);
      gvp->form = (ForM) w;
      gvp->actproc = NULL;
      gvp->toform = GraphInfoPtrToGraphViewForm;
    }
  }
  return (ForM) w;
}

extern Int2 LIBCALLBACK GraphGenFunc (Pointer data)
{
  OMProcControlPtr      ompcp;
  SeqAnnotPtr           annot;
  WindoW                wg;
  GraphViewFormPtr      gvp;
  GraphInfoPtr          gip;
  SeqGraphPtr           sgp;
  Int4                  graphpixelwidth, graphpixelheight;

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL || ompcp->proc == NULL)
    return OM_MSG_RET_ERROR;

  switch (ompcp->input_itemtype)
  {
   case OBJ_SEQANNOT:                      /* shouldn't be ? */
   case OBJ_ANNOTDESC:
    annot = (SeqAnnotPtr) ompcp->input_data;
    if (annot != NULL && annot->type != 3) /* graph */
    {
      return OM_MSG_RET_ERROR;
    }
    if ((sgp = (SeqGraphPtr) annot->data) == NULL)
    {
      return OM_MSG_RET_ERROR;
    }
    break;
   case OBJ_SEQGRAPH:
    if ((sgp = (SeqGraphPtr) ompcp->input_data) == NULL)
    {
      return OM_MSG_RET_ERROR;
    }
    break;
   default :
    return OM_MSG_RET_ERROR;
  }

  if (sgp != NULL)
  {
    if ((wg = (WindoW) xCreateGraphViewForm
                       (-50, -33, "Graph Annotation")) == NULL)
    {
      return OM_MSG_RET_ERROR;
    }
    if ((gvp = GetObjectExtra (wg)) == NULL)
    {
      Show (wg);
      Remove (wg);
      return OM_MSG_RET_ERROR;
    }

    gip = NULL;
    while (sgp != NULL)
    {
      graphpixelwidth = 400;
      graphpixelheight = 100;

      if ((gip = GraphInfoNew (gip, sgp->flags[2], sgp->numval,
                               graphpixelwidth, graphpixelheight)) == NULL)
      {
        Show (wg);
        Remove (wg);
        GraphInfoFree (gvp->gip);
        return OM_MSG_RET_ERROR;
      }
      if (gvp->gip == NULL)
        gvp->gip = gip;

      gip->vert_offset = 0;
      gip->horiz_offset = 0;

      gip->horiz_offset = gip->horiz_offset;
      gip->vert_offset = gip->vert_offset;
      gip->sgp = CopyGraph (sgp);
      sgp = sgp->next;
    }
    GraphInfoPtrToGraphViewForm (gvp->form, gvp->gip);
  }
  else
  {
    return OM_MSG_RET_ERROR;
  }

  Show (wg);
  Select (wg);
  return OM_MSG_RET_DONE;
}
