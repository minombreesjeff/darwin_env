#include <objseq.h>
#include <seqport.h>
#include <gather.h>
#include <picture.h>
#include <viewer.h>
#include <ncbibs.h>

#include <seqgraph.h>
#include <seqfltr.h>
#include <seqmtrx.h>

/* defines */

#define SCALESPACE 100

/* structures - spreadsheet dialogues for endpoints */

typedef struct endpointdialog
{
  DIALOG_MESSAGE_BLOCK
  DialoG                dep;
} EndPointsDialog, PNTR EndPointsDialogPtr;

/* globals - spreadsheet */

Uint2 ep_types_m [] = {TAGLIST_TEXT, TAGLIST_TEXT, TAGLIST_TEXT};
Uint2 ep_width_m [] = {4, 4, 4};
Uint2 ep_types_f [] = {TAGLIST_TEXT, TAGLIST_TEXT};
Uint2 ep_width_f [] = {4, 4};
Uint2Ptr ep_t, ep_w;

/* functions - void pointer math on data array */

static FloatHi GetFloatHi (Pointer data, Uint1 type, Int4 pos)
{
  FloatHi        fval;
  ByteStorePtr   bp;

  switch (type)
  {
   default:
   case 1:
    fval = (FloatHi) (((FloatHiPtr) data)[pos]);
    break;
   case 2:
    fval = (FloatHi) (((Int4Ptr) data)[pos]);
    break;
   case 3:
    bp = (ByteStorePtr) data;
    BSSeek (bp, pos, SEEK_SET);
    fval = (FloatHi) BSGetByte (bp);
    break;
  }
  return fval;
}

/* functions - spreadsheet dialogues for endpoints */

static ValNodePtr ValNodeCopyCharPtr (ValNodePtr v1)
{
  ValNodePtr vhead, v2;

  vhead = v2 = NULL;
  while (v1 != NULL)
  {
    v2 = ValNodeNew (vhead);
    if (vhead == NULL)
      vhead = v2;
    v2->data.ptrvalue = StringSave (v1->data.ptrvalue);
    v1 = v1->next;
  }
  return vhead;
}

static void EndPointToDialog (DialoG d, Pointer data)
{
  ValNodePtr         eplist;
  TagListPtr         tlp;
  ValNodePtr         vnp, epdup;
  Int2               j;

  tlp = (TagListPtr) GetObjectExtra (d);
  eplist = (ValNodePtr) data;
  epdup = ValNodeCopyCharPtr (eplist);
  if (tlp != NULL)
  {
    SendMessageToDialog (tlp->dialog, VIB_MSG_RESET);
    tlp->vnp = epdup;
    SendMessageToDialog (tlp->dialog, VIB_MSG_REDRAW);
    for (j = 0, vnp = tlp->vnp; vnp != NULL; j++, vnp = vnp->next) {
    }
    tlp->max = MAX ((Int2) 0, (Int2) (j - tlp->rows + 1));
    CorrectBarMax (tlp->bar, tlp->max);
    CorrectBarPage (tlp->bar, (Int2) (tlp->rows-1), (Int2) (tlp->rows-1));
  }
  return;
}

static Pointer DialogToEndPoint (DialoG d)
{
  ValNodePtr         ephead, eplist, vnp;
  TagListPtr         tlp;

  ephead = eplist = NULL;
  tlp = (TagListPtr) GetObjectExtra (d);
  if (tlp != NULL && tlp->vnp != NULL)
  {
    for (vnp = tlp->vnp; vnp != NULL; vnp = vnp->next)
    {
      eplist = ValNodeNew (ephead);
      if (ephead == NULL)
        ephead = eplist;
      eplist->data.ptrvalue = StringSave ((CharPtr) vnp->data.ptrvalue);
    }
  }
  return ephead;
}

static void EndPointValNodeToEndPointDialog (DialoG d, Pointer data)
{
  EndPointsDialogPtr epp;

  epp = (EndPointsDialogPtr) GetObjectExtra (d);
  if (epp != NULL)
    PointerToDialog (epp->dep, data);

  return;
}

static Pointer EndPointDialogToEndPointValNode (DialoG d)
{
  EndPointsDialogPtr epp;
  ValNodePtr         eplist;

  epp = (EndPointsDialogPtr) GetObjectExtra (d);
  if (epp != NULL)
  {
    eplist = (ValNodePtr) DialogToPointer (epp->dep);
  }
  return (Pointer) eplist;
}

static void CleanupCutoffProc (GraphiC g, VoidPtr data)
{
  StdCleanupExtraProc (g, data);
  return;
}

static DialoG CreateCutoffsDialog (GrouP g, Uint2 rows, Int2 spacing,
                                   Boolean flagIsMatrix)
{
  EndPointsDialogPtr  epp;
  GrouP               g1, c, c1;
  PrompT              es, ee, em;
  TagListPtr          tlp;
  Uint2               columns;

  c = HiddenGroup (g, 0 , 0, NULL);
  epp = (EndPointsDialogPtr) MemNew (sizeof (EndPointsDialog));
  if (epp != NULL)
  {
    SetObjectExtra (c, epp, CleanupCutoffProc);
    epp->dialog = (DialoG) c;
    epp->todialog = EndPointValNodeToEndPointDialog;
    epp->fromdialog = EndPointDialogToEndPointValNode;
    epp->testdialog = NULL;
    epp->dialogmessage = NULL;

    g1 = HiddenGroup (c, -1, 0, NULL);
    SetGroupSpacing (g1, 4, 2);
    if (flagIsMatrix)
      c1 = HiddenGroup (g1, 3, 0, NULL);
    else
      c1 = HiddenGroup (g1, 2, 0, NULL);
    SetGroupSpacing (c1, spacing, spacing);
    es = StaticPrompt (c1, "Start", 0, 0, programFont, 'c');
    ee = StaticPrompt (c1, "End", 0, 0, programFont, 'c');
    if (flagIsMatrix)
    {
      em = StaticPrompt (c1, "Strand", 0, 0, programFont, 'c');
      columns = 3;
      ep_t = ep_types_m;
      ep_w = ep_width_m;
    }
    else
    {
      columns = 2;
      ep_t = ep_types_f;
      ep_w = ep_width_f;
    }
    epp->dep = CreateTagListDialog (g1, rows, columns, spacing,
                                    ep_t, ep_w, NULL,
                                    EndPointToDialog,
                                    DialogToEndPoint);
    tlp = (TagListPtr) GetObjectExtra (epp->dep);
    if (tlp != NULL)
    {
      AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control[0],
                                   (HANDLE) es, NULL);
      AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control[1],
                                   (HANDLE) ee, NULL);
      if (flagIsMatrix)
        AlignObjects (ALIGN_JUSTIFY, (HANDLE) tlp->control[2],
                                     (HANDLE) em, NULL);
    }
  }
  return (DialoG) c;
}

/* functions - internal */

static Boolean UnHLSeg (SegmenT seg, PrimitivE prim, Uint2 segID,
                        Uint2 primID, Uint2 primCt, Pointer data)
{
  VieweR v;

  v = (VieweR) data;
  HighlightPrimitive (v, seg, prim, PLAIN_PRIMITIVE);
  return TRUE;
}

static void HistoClickProc (PaneL pn, PoinT pt)
{
  VieweR    v;
  Uint2     segID, primID, primCt;
  SegmenT   s;
  PrimitivE p;
  GraphViewFormPtr gvp;
  SeqIntPtr sint;
  PntInfo   pnt;

  segID = 0;
  primID = 0;
  primCt = 0;

  v = (VieweR) pn;
  s = FindSegment (v, pt, &segID, &primID, &primCt);

  gvp = GetObjectExtra (v);
  if (primID < 1)
  {
    WatchCursor ();
    ExploreSegment (s, v, UnHLSeg);
    ObjMgrDeSelectAll ();
    ArrowCursor ();
  }
  else
  {
    WatchCursor ();
    ExploreSegment (s, v, UnHLSeg);
    ObjMgrDeSelectAll ();
    ArrowCursor ();
    p = GetPrimitive (s, primCt);
    HighlightPrimitive (v, s, p, FRAME_PRIMITIVE);
    gvp->slp = (SeqLocPtr) ValNodeNew (NULL);
    sint = SeqIntNew ();
    sint->id = SeqIdDup (gvp->bsp->id);
    if (primID == 1)
      sint->strand = Seq_strand_plus;
    else
      sint->strand = Seq_strand_minus;
    MapViewerToWorld (v, pt, &pnt);
    gvp->start = pnt.x * gvp->zoom;
    gvp->stop = gvp->start+gvp->window;
    sint->from = gvp->start;
    sint->to = gvp->stop;
    gvp->slp->choice = SEQLOC_INT;
    gvp->slp->data.ptrvalue = (Pointer) sint;
    ObjMgrSelect (gvp->entityID, gvp->itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC,
                  gvp->slp);
  }
  return;
}

static void GraphClickProc (PaneL pn, PoinT pt)
{
  VieweR    v;
  Uint2     segID, primID, primCt, start, end, i;
  SegmenT   s;
  PrimitivE p;
  GraphViewFormPtr gvp;
  SeqIntPtr sint;
  PntInfo   pnt;

  segID = 0;
  primID = 0;
  primCt = 0;

  v = (VieweR) pn;
  s = FindSegment (v, pt, &segID, &primID, &primCt);
  p = GetPrimitive (s, primCt);
  if (primID == 0)
    return;

  gvp = GetObjectExtra (v);
  if (gvp->flagNewClick)
  {
    WatchCursor ();
    ExploreSegment (s, v, UnHLSeg);
    ArrowCursor ();
    HighlightPrimitive (v, s, p, FRAME_PRIMITIVE);
    gvp->flagNewClick = FALSE;
    gvp->HLRange = primCt;
    MapViewerToWorld (v, pt, &pnt);
    gvp->start = pnt.x * gvp->zoom;
  }
  else
  {
    start = gvp->HLRange;
    if (start < primCt)
      end = (Uint2) (primCt + 1);
    else
    {
      start = primCt;
      end = (Uint2) (gvp->HLRange + 1);
    }
    for (i = start; i < end; i++)
    {
      p = GetPrimitive (s, i);
      HighlightPrimitive (v, s, p, FRAME_PRIMITIVE);
    }
    if (gvp->slp != NULL)
    {
      ObjMgrDeSelect (gvp->entityID, gvp->itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC,
                      gvp->slp);
/*      gvp->slp = SeqLocFree (gvp->slp); */
      gvp->slp = NULL;
    }
    gvp->slp = (SeqLocPtr) ValNodeNew (NULL);
    sint = SeqIntNew ();
    sint->id = SeqIdDup (gvp->bsp->id);
    sint->strand = Seq_strand_plus;
    MapViewerToWorld (v, pt, &pnt);
    gvp->stop = pnt.x * gvp->zoom;
    if (gvp->start < gvp->stop)
    {
      sint->from = gvp->start;
      sint->to = gvp->stop;
    }
    else
    {
      sint->from = gvp->stop;
      sint->to = gvp->start;
    }
    gvp->slp->choice = SEQLOC_INT;
    gvp->slp->data.ptrvalue = (Pointer) sint;
    ObjMgrSelect (gvp->entityID, gvp->itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC,
                  gvp->slp);
/* ObjMgrAlsoSelect for loop thru slp's */
/* have to clone each slp added to chain and pass clone to OM */
    gvp->flagNewClick = TRUE;
  }
  return;
}

/* scroll bar stuff */

static void ScrollBarScrollProc (BaR b, GraphiC g, Int2 new, Int2 old)
{
  GraphViewFormPtr gvp;
  Int2             prc;
  Char             buf[8];

  gvp = (GraphViewFormPtr) GetObjectExtra (b);
  prc = (Int2) (100 - new);
  sprintf (buf, "%3d", prc);
  SetTitle (gvp->pc, buf);
  gvp->level = prc;
  return;
}

static void ScrollTextScrollProc (TexT t)
{
  GraphViewFormPtr gvp;
  Int2             prc;
  Char             buf[8];

  gvp = (GraphViewFormPtr) GetObjectExtra (t);
  GetTitle (gvp->pc, buf, sizeof (buf));
  StrToInt (buf, &prc);
  CorrectBarValue (gvp->br, (Int2) (100-prc));
  gvp->level = prc;
  return;
}

static void DrawPercent (VieweR v, SegmenT seg)
{
  GraphViewFormPtr    gvp;
  SeqGraphPtr         sgp;
  Int4                i, loop, compression;
  FloatHi             level;
  PoinT               p1, p2;
  PntInfo             pw1, pw2;
  FloatHi             fval;
  Int4                datapos;
  Int4                count, countmax, markstart, markend;
  Char                outbuf[32];
  ValNodePtr          ephead, eplist;
  RecT                r;
  FloatHi             maxVal;

  if ((gvp = (GraphViewFormPtr) GetObjectExtra (v)) == NULL)
    return;

  if ((sgp = gvp->sgp) == NULL)
    return;

  WatchCursor ();

  GetPosition (gvp->viewer, &r);
  compression = gvp->zoom;
  level = (FloatHi) gvp->level;
  Red ();

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

  if (gvp->graphtype == GRAPH_MATRIX_AA ||
      gvp->graphtype == GRAPH_MATRIX_NA)
  {
    level /= (100.0 * 2.0);
    pw1.x = (Int4) (gvp->margin);
    pw1.y = (Int4) (gvp->margin + ((Int4)
                       ((maxVal/2 +
                       (maxVal*level)) * sgp->a) +
                       (Int4) sgp->b));
    pw2.x = (Int4) (gvp->margin + sgp->numval/compression);
    pw2.y = (Int4) (gvp->margin + ((Int4)
                       ((maxVal/2 +
                       (maxVal*level)) * sgp->a) +
                       (Int4) sgp->b));
    MapWorldToViewer (gvp->viewer, pw1, &p1);
    MapWorldToViewer (gvp->viewer, pw2, &p2);
    if (p1.x < r.left || p1.x > r.right)
      p1.x = r.left;
    if (p2.x > r.right || p2.x < r.left)
      p2.x = r.right;
    DrawLine (p1, p2);
    pw1.x = (Int4) (gvp->margin);
    pw1.y = (Int4) (gvp->margin + ((Int4)
                       ((maxVal/2 -
                       (maxVal*level)) * sgp->a) +
                       (Int4) sgp->b));
    pw2.x = (Int4) (gvp->margin + sgp->numval/compression);
    pw2.y = (Int4) (gvp->margin + ((Int4)
                       ((maxVal/2 -
                       (maxVal*level)) * sgp->a) +
                       (Int4) sgp->b));
    MapWorldToViewer (gvp->viewer, pw1, &p1);
    MapWorldToViewer (gvp->viewer, pw2, &p2);
    if (p1.x < r.left || p1.x > r.right)
      p1.x = r.left;
    if (p2.x > r.right || p2.x < r.left)
      p2.x = r.right;
    DrawLine (p1, p2);
  }
  else if (gvp->graphtype == GRAPH_FILTER)
  {
    level /= 100.0;
    pw1.x = (Int4) (gvp->margin);
    pw1.y = (Int4) (gvp->margin + ((Int4)
                       ((maxVal * level) * sgp->a) +
                       (Int4) sgp->b));
    pw2.x = (Int4) (gvp->margin + sgp->numval/compression);
    pw2.y = (Int4) (gvp->margin + ((Int4)
                       ((maxVal * level) * sgp->a) +
                       (Int4) sgp->b));
    MapWorldToViewer (gvp->viewer, pw1, &p1);
    MapWorldToViewer (gvp->viewer, pw2, &p2);
    if (p1.x < r.left || p1.x > r.right)
      p1.x = r.left;
    if (p2.x > r.right || p2.x < r.left)
      p2.x = r.right;
    DrawLine (p1, p2);
  }

  ephead = eplist = NULL;
  if (gvp->graphtype == GRAPH_MATRIX_AA ||
      gvp->graphtype == GRAPH_MATRIX_NA)
    loop = 2;
  else
    loop = 1;

  for (i = 0; i < loop; i++)
  {
    datapos = 0;
    fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
    countmax = sgp->numval;
    count = 1;                /* got one already */
    while (count < countmax)
    {
      if (fval >= (FloatHi) gvp->level)
      {
        markstart = count;
        datapos++;
        fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
        count++;
        while (count < countmax)
        {
          if (fval < (FloatHi) gvp->level)
          {
            markend = count - 1;
            if (gvp->graphtype == GRAPH_MATRIX_AA ||
                gvp->graphtype == GRAPH_MATRIX_NA)
              markend += gvp->window;
            if (i != 1)
            {
              if (sgp->next == NULL)
                sprintf (outbuf, "%ld\t%ld\n",
                         (long) markstart, (long) markend);
              else
                sprintf (outbuf, "%ld\t%ld\t plus\n",
                         (long) markstart, (long) markend);
            }
            else
            {
              sprintf (outbuf, "%ld\t%ld\t minus\n",
                       (long) markstart, (long) markend);
            }
            eplist = ValNodeNew (ephead);
            if (ephead == NULL)
              ephead = eplist;
            if (eplist != NULL)
              eplist->data.ptrvalue = StringSave (outbuf);
            break;
          }
          datapos++;
          fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
          count++;
        }
        if (count == countmax)
        {
/*          markend = count - 1; */
          break;
        }
      }
      datapos++;
      fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
      count++;
    }
    if (gvp->graphtype == GRAPH_MATRIX_AA ||
        gvp->graphtype == GRAPH_MATRIX_NA)
    {
      sgp = sgp->next;
      if (sgp == NULL)
        break;
    }
  }
  PointerToDialog (gvp->cutoffs, (Pointer) ephead);
  ValNodeFreeData (ephead);
  ArrowCursor ();
  return;
}

static SegmenT DrawSeqGraph (SeqGraphPtr sgp, Int4 window, Uint1 graphtype,
                             Int4 margin, SegmenT seg, Int2 zoom)
{
  Uint2      primID;
  Int4       i, compression, icomp, ix, stophere, ticks, val;
  Int4       top, bottom;
  FloatHi    fval;
  Int4       datapos;
  FloatHi    fmin, fmax, fthis, fnext;
  Boolean    flagComplement;
  Char       tickpos[16];
  FloatHi    maxVal, minVal;

  Int4       marginT, marginL;

  if (sgp == NULL)
    return seg;

  WatchCursor ();

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

  if (sgp->compr != 0)
    compression = (Int4) sgp->compr;
  else
    compression = 1;

  if (zoom != 0)
    compression = zoom;

  flagComplement = FALSE;
  if ((graphtype == GRAPH_MATRIX_AA) &&
      seg != NULL)
    return seg;

  if ((graphtype == GRAPH_MATRIX_NA) &&
      seg != NULL)
    flagComplement = TRUE;

  if (seg == NULL)
    seg = CreatePicture ();

  primID = 0;
  if (seg != NULL)
  {
    if (!flagComplement)
    {
      AddAttribute (seg, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                    BLACK_COLOR, SOLID_LINE, SOLID_SHADING, STD_PEN_WIDTH, 0);

      marginL = marginT = margin;
      bottom = margin + (((Int4) maxVal - (Int4) minVal) * (Int4) sgp->a) +
                        (Int4) sgp->b;
      if (graphtype == GRAPH_MATRIX_AA)
      {
        marginL = margin;
        marginT = margin + 100 + 1;
        bottom = marginL + ((bottom-margin)/2);
        bottom += marginT;;
      }
      AddRectangle (seg,
                    marginL,
                    marginT,
                    marginL + (sgp->numval/compression),
                    bottom,
                    FALSE, FALSE, primID);
/*
 printf ("L %ld T %ld R %ld B %ld\n", marginL, marginT,
         marginL + (sgp->numval/compression), bottom);
*/
      ticks = 100;
      val = 0;
      while (val < (sgp->numval/compression))
      {
        if ((val % (ticks/2)) == 0)
        {
          while (((val*compression) % 10) != 0)
            val++;

          top = margin + ((Int4) ((-2 * sgp->a/2) + (Int4) sgp->b));
          bottom = margin + ((Int4) ((-6 * sgp->a/2) + (Int4) sgp->b));
          if (graphtype == GRAPH_MATRIX_AA)
          {
            top += marginT;
            bottom += marginT;
          }
          AddLine (seg, margin + val,
                        top,
                        margin + val,
                        bottom,
                        FALSE, 0);
          if ((val % ticks) == 0)
          {
            sprintf (tickpos, "%ld", (long) (val*compression));
            top = margin + ((Int4) ((-18 * sgp->a/2) + (Int4) sgp->b));
            if (graphtype == GRAPH_MATRIX_AA)
              top += marginT;
            AddTextLabel (seg,
                          margin + val,
                          top,
                          tickpos, NULL, 0, UPPER_CENTER, 0);
          }
        }
        val++;
      }

      if (graphtype == GRAPH_MATRIX_AA ||
          graphtype == GRAPH_MATRIX_NA)
      {
        AddLine (seg,
                 margin,
                 margin + ((Int4) ((maxVal/2 * sgp->a) +
                           (Int4) sgp->b)),
                 margin + (sgp->numval/compression),
                 margin + ((Int4) ((maxVal/2 * sgp->a) +
                           (Int4) sgp->b)),
                 FALSE, primID);
      }
    }

    icomp = 0;
    ix = 0;
    datapos = 0;
    fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
    for (i = 0; i < window/2; i++)
    {
      icomp++;
      if (icomp == compression)
      {
        icomp = 0;
        ix++;
      }
      datapos++;
      fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
    }
    fmin = fval;
    fmax = fval;
    stophere = sgp->numval - 1;
    for (i = window; i < stophere; i++)
    {
      icomp++;
      if (fval < fmin)
        fmin = fval;
      if (fval > fmax)
        fmax = fval;
      if (icomp == compression)
      {
        if (graphtype == GRAPH_MATRIX_AA ||
            graphtype == GRAPH_MATRIX_NA)
        {
          fmin = 0.0;
          if (fmax > 0.0)
          {
            fmax /= 2;
            if (flagComplement)
            {
              fmin = maxVal;
              fmax = maxVal - fmax;
              fmin -= maxVal/2;
              fmax -= maxVal/2;
            }
            else
            {
              fmin += maxVal/2;
              fmax += maxVal/2;
            }
            if (!flagComplement)
              primID = 1;
            else
              primID = 2;
            AddLine (seg,
                     margin + ix,
                     margin + ((Int4) ((fmax * sgp->a) + (Int4) sgp->b)),
                     margin + ix,
                     margin + ((Int4) ((fmin * sgp->a) + (Int4) sgp->b)),
                     FALSE, primID);
          }
        }
        else
        {
          primID = 1;
          AddLine (seg,
                   margin + ix,
                   margin + ((Int4) ((fmax * sgp->a) + (Int4) sgp->b)),
                   margin + ix,
                   margin + ((Int4) ((fmin * sgp->a) + (Int4) sgp->b)),
                   FALSE, primID);
          fthis = fval;
          fnext = GetFloatHi (sgp->values, sgp->flags[2], datapos+1);
          primID = 1;
          AddLine (seg,
                   margin + ix,
                   margin + ((Int4) ((fthis * sgp->a) + (Int4) sgp->b)),
                   margin + ix + 1,
                   margin + ((Int4) ((fnext * sgp->a) + (Int4) sgp->b)),
                   FALSE, primID);
        }
        fmin = fval;
        fmax = fval;
        icomp = 0;
        ix++;
      }
      datapos++;
      fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
    }
    if (icomp != 0 && graphtype == GRAPH_FILTER)
    {
      primID = 1;
      AddLine (seg,
               margin + ix,
               margin + ((Int4) ((fmax * sgp->a) + (Int4) sgp->b)),
               margin + ix,
               margin + ((Int4) ((fmin * sgp->a) + (Int4) sgp->b)),
               FALSE, primID);
    }
  }
  ArrowCursor ();
  return seg;
}

static void ChangePercentProc (ButtoN b)
{
  Char                buf[8];
  TexT                pc;
  Int2                prc;
  GraphViewFormPtr    gvp;
  RecT                r;
  Uint2               segID, primID, primCt;
  SegmenT             s;
  PoinT               pt;
  PntInfo             pw;

  pc = (TexT) GetObjectExtra (b);
  GetTitle (pc, buf, sizeof (buf));
  StrToInt (buf, &prc);
  gvp = (GraphViewFormPtr) GetObjectExtra (pc);
  gvp->level = prc;

  WatchCursor ();

  pw.x = 1;
  pw.y = 1;
  MapWorldToViewer (gvp->viewer, pw, &pt);
  segID = 0;
  primID = 0;
  primCt = 0;
  s = FindSegment (gvp->viewer, pt, &segID, &primID, &primCt);
  ExploreSegment (s, gvp->viewer, UnHLSeg);
  Select (gvp->viewer);
  ObjectRect (gvp->viewer, &r);
  InvalRect (&r);

  ArrowCursor ();
  return;
}

static void MarkPrims (VieweR v, Int4 start, Int4 end, SeqGraphPtr sgp,
                       Boolean flagFilter, Int4 margin, Int2 zoom)
{
  Uint2      segID, primID, primCt;
  PoinT      pt;
  PntInfo    pw;
  SegmenT    s;
  PrimitivE  p;
  Int4       i, compression;
  FloatHi    fval;
  Int4       datapos;
  FloatHi    val, maxVal;

  compression = (Int4) zoom;

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

  if (flagFilter)
  {
    datapos = 0;
    fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
    for (i = 0; i < sgp->numval; i++)
    {
      if (i < start)
      {
        datapos++;
        fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
        continue;
      }
      if (i > end)
        break;

      pw.x = margin + i/compression;
      if (i%compression != 0)
        pw.x -= 1;
      pw.y = margin + ((Int4) ((fval * sgp->a) + (Int4) sgp->b));
      MapWorldToViewer (v, pw, &pt);
      s = FindSegment (v, pt, &segID, &primID, &primCt);

      if (primID < 1)
      {
        datapos++;
        fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
        continue;
      }

      p = GetPrimitive (s, primCt);
      HighlightPrimitive (v, s, p, FRAME_PRIMITIVE);
      datapos++;
      fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
    }
  }
  else
  {
    datapos = 0;
    for (i = 0; i < sgp->numval; i++)
    {
      if (i < start)
      {
        fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
        datapos++;
        continue;
      }
      if (i > end)
        break;

      pw.x = margin + i/compression;
      if (i%compression != 0)
        pw.x -= 1;
      val = fval/2;
      if (sgp->next != NULL)
      {
        val += maxVal/2;
      }
      else
      {
        val = maxVal - val;
        val -= maxVal/2;
      }
      pw.y = margin + ((Int4) ((val * sgp->a) + (Int4) sgp->b));
      MapWorldToViewer (v, pw, &pt);
      s = FindSegment (v, pt, &segID, &primID, &primCt);

      if (primID < 1)
      {
        datapos++;
        fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
        continue;
      }

      p = GetPrimitive (s, primCt);
      HighlightPrimitive (v, s, p, FRAME_PRIMITIVE);
      datapos++;
      fval = GetFloatHi (sgp->values, sgp->flags[2], datapos);
    }
  }
  return;
}

static void HighlightCutoff (GraphViewFormPtr gvp, ValNodePtr eplist)
{
  Char                buff[32];
  CharPtr             ptr1, ptr2;
  Int4                start, end;
  SeqGraphPtr         sgp;
  Int4                i;

/* note that all error checking is done in the calling functions */

  WatchCursor ();

  if (gvp->graphtype == GRAPH_MATRIX_AA ||
      gvp->graphtype == GRAPH_MATRIX_NA)
  {
    sgp = gvp->sgp;
    while (eplist != NULL)
    {
      StrCpy (buff, (CharPtr) eplist->data.ptrvalue);
      ptr1 = StrChr (buff, '\t');
      *ptr1 = '\0';
      StrToLong (buff, &start);
      StrToLong (buff, &end);
      *ptr1 = '\t';
      ptr1++;
      ptr2 = StrChr (ptr1, '\t');
      ptr2++;
      if (sgp->next != NULL)
      {
        i = StringICmp (ptr2, " minus\n");
        if (i == 0)
          sgp = sgp->next;
      }
      MarkPrims (gvp->viewer, start, end, sgp, FALSE, gvp->margin,
                 gvp->zoom);
      eplist = eplist->next;
    }
  }
  else if (gvp->graphtype == GRAPH_FILTER)
  {
    while (eplist != NULL)
    {
      StrCpy (buff, (CharPtr) eplist->data.ptrvalue);
      ptr1 = StrChr (buff, '\t');
      *ptr1 = '\0';
      StrToLong (buff, &start);
      *ptr1 = '\t';
      ptr1++;
      ptr2 = StrChr (ptr1, '\n');
      *ptr2 = '\0';
      StrToLong (ptr1, &end);
      *ptr2 = '\n';
      MarkPrims (gvp->viewer, start, end, gvp->sgp, TRUE, gvp->margin,
                 gvp->zoom);
      eplist = eplist->next;
    }
  }
  ArrowCursor ();
  return;
}

static void CutOffProc (ButtoN b)
{
  GraphViewFormPtr    gvp;
  ValNodePtr          eplist, ephead, epprev, epnext;
  Char                buff[32];
  CharPtr             ptr1, ptr2;
  Int4                start, end;

  WatchCursor ();

  gvp = (GraphViewFormPtr) GetObjectExtra (b);
  if (gvp != NULL)
  {
    eplist = DialogToPointer (gvp->cutoffs);
  }
  ephead = epprev = NULL;
  while (eplist != NULL)
  {
    StrCpy (buff, (CharPtr) eplist->data.ptrvalue);
    if ((ptr1 = StrChr (buff, '\t')) == NULL)
    {
      epnext = eplist->next;
      eplist->next = NULL;
      ValNodeFreeData (eplist);
      eplist = epnext;
      if (epprev != NULL)
        epprev->next = eplist;
      continue;
    }
    *ptr1 = '\0';
    StrToLong (buff, &start);
    *ptr1 = '\t';
    ptr1++;
    if (gvp->graphtype == GRAPH_MATRIX_AA ||
        gvp->graphtype == GRAPH_MATRIX_NA)
    {
      if ((ptr2 = StrChr (ptr1, '\t')) != NULL)
        *ptr2 = '\0';
    }
    else
    {
      if ((ptr2 = StrChr (ptr1, '\n')) != NULL)
        *ptr2 = '\0';
    }
    StrToLong (ptr1, &end);
    if ((start > end) || (start == 0 && end == 0))
    {
      epnext = eplist->next;
      eplist->next = NULL;
      ValNodeFreeData (eplist);
      eplist = epnext;
      if (epprev != NULL)
        epprev->next = eplist;
      continue;
    }
    if (gvp->graphtype == GRAPH_MATRIX_AA ||
        gvp->graphtype == GRAPH_MATRIX_NA)
    {
      *ptr2 = '\t';
      ptr2++;
      if ((ptr1 = StrChr (ptr2, '\n')) != NULL)
      {
        *ptr1 = '\0';
        *ptr1 = '\n';
      }
    }
    else
    {
      *ptr2 = '\n';
    }
    if (ephead == NULL)
      ephead = eplist;
    epprev = eplist;
    eplist = eplist->next;
  }
  PointerToDialog (gvp->cutoffs, (Pointer) ephead);
  HighlightCutoff (gvp, ephead);
  ValNodeFreeData (ephead);
  ArrowCursor ();
  return;
}

/* functions - external */

static void CleanupGraphViewForm (GraphiC g, VoidPtr data)
{
  GraphViewFormPtr gvp;

  if ((gvp = (GraphViewFormPtr) data) != NULL)
  {
    WatchCursor ();
    GraphViewFormFree (gvp, FALSE);
    ArrowCursor ();
  }
  StdCleanupFormProc (g, data);
  return;
}

static void CloseGraphViewProc (ButtoN b)
{
  WindoW           w;
  GraphViewFormPtr gvp;

  if (b != NULL)
  {
    if ((gvp = (GraphViewFormPtr) GetObjectExtra (b)) != NULL)
    {
      ObjMgrDeSelectAll ();
      w = (WindoW) gvp->form;
      Remove (w);
    }
  }
  return;
}

static void SeqLocGraphViewProc (ButtoN b)
{
  GraphViewFormPtr    gvp;
  ValNodePtr          ephead, eplist;
  Char                buff[32];
  CharPtr             ptr1, ptr2;
  Int4                start, end;
  SeqLocPtr           slp = NULL;
  ValNodePtr          vnp = NULL;
  SeqIntPtr           snp;

  if (b == NULL)
    return;

  gvp = (GraphViewFormPtr) GetObjectExtra (b);
  if (gvp != NULL)
    eplist = DialogToPointer (gvp->cutoffs);

  ephead = eplist;
  while (eplist != NULL)
  {
    StrCpy (buff, (CharPtr) eplist->data.ptrvalue);
    ptr1 = StrChr (buff, '\t');
    *ptr1 = '\0';
    StrToLong (buff, &start);
    ptr1++;
    if (gvp->graphtype == GRAPH_MATRIX_AA ||
        gvp->graphtype == GRAPH_MATRIX_NA)
      ptr2 = StrChr (ptr1, '\t');
    else
      ptr2 = StrChr (ptr1, '\n');
    *ptr2 = '\0';
    StrToLong (ptr1, &end);

    if (slp == NULL)
    {
      if ((slp = ValNodeNew (NULL)) == NULL)
        return;
      if (eplist->next == NULL)
        slp->choice = SEQLOC_INT;
      else
        slp->choice = SEQLOC_PACKED_INT;
    }

    if (slp != NULL)
    {
      snp = SeqIntNew ();
      if (snp == NULL)
      {
        ValNodeFreeData (slp);
        return;
      }
      snp->id = SeqIdDup (gvp->bsp->id);
      snp->from = start;
      snp->to = end;

      if (slp->choice == SEQLOC_INT)
        ValNodeAddPointer (&vnp, SEQLOC_INT, snp);
      slp = NULL;
    }
    eplist = eplist->next;
  }
  ValNodeFreeData (ephead);

  if (slp != NULL)
  {
    if (slp->choice == SEQLOC_INT)
      slp->data.ptrvalue = (Pointer) snp;
    else
      slp->data.ptrvalue = vnp;
  }
  gvp->seqloc = slp;

  return;
}

/* zoom stuff */

/* defines */

#define ZOOMAX 16

/* functions */

static void InitZoomPopup (GraphViewFormPtr gvp, SeqGraphPtr sgp)
{
  Char            buf[16];
  Int2            i, maxzoom;
  BaR             sb;

  if (gvp != NULL && sgp != NULL)
  {
    SafeHide (gvp->scale);
    Reset (gvp->scale);
    maxzoom = (Int2) sgp->compr;
    i = 2;
    while (maxzoom > 0)
    {
      i++;
      maxzoom /= (Int2) 2;
    }
    if (i > ZOOMAX)
      i = ZOOMAX;
    maxzoom = i;

    for (i = 1; i < maxzoom; i++)
    {
      if (i == maxzoom-1)
      {
        gvp->zoom = (Int2) sgp->compr;
        sprintf (buf, "%ld", (long) gvp->zoom);
      }
      else
      {
        gvp->zoom = (Int2) pow (2, (i-1));
        sprintf (buf, "%ld", (long) gvp->zoom);
      }
      PopupItem (gvp->scale, buf);
    }
    SetValue (gvp->scale, (Int2) (i-1));
    SafeShow (gvp->scale);
  }

  sb = GetSlateHScrollBar ((SlatE) gvp->viewer);
  CorrectBarMax (sb, 0);

  return;
}

static void ScaleGraphView (PopuP p)
{
  GraphViewFormPtr  gvp;
  Int2              i;
  Int4              oldsbpos, oldsbmax;
  Int4              newsbmax, curatt;
  BaR               sb;

  gvp = (GraphViewFormPtr) GetObjectExtra (p);
  if (gvp == NULL)
    return;

  sb = GetSlateHScrollBar ((SlatE) gvp->viewer);
  oldsbpos = (Int4) GetValue (sb);
  oldsbmax = (Int4) GetBarMax (sb);

  i = GetValue (gvp->scale);
  if (i > 0)
  {
    if ((Int2) pow (2, (i-1)) < (Int2) gvp->sgp->compr)
      gvp->zoom = (Int2) pow (2, (i-1));
    else
      gvp->zoom = (Int2) gvp->sgp->compr;
  }
  else
  {
    i = 1;
    gvp->zoom = 1;
  }
  SetValue (gvp->scale, i);

  gvp->seg = DeletePicture (gvp->seg);
  gvp->seg = DrawSeqGraph (gvp->sgp, gvp->window, gvp->graphtype,
                           gvp->margin, gvp->seg, gvp->zoom);
  if (gvp->graphtype == GRAPH_MATRIX_NA)
  {
    gvp->seg = DrawSeqGraph (gvp->sgp->next, gvp->window, gvp->graphtype,
                             gvp->margin, gvp->seg, gvp->zoom);
  }

  curatt = INT4_MIN;
  if (oldsbmax > 0 && oldsbpos > 0)
  {
    if (oldsbpos != oldsbmax)
    {
      curatt = (((gvp->sgp->numval/gvp->zoom)-(gvp->width+SCALESPACE))
                  *oldsbpos/oldsbmax);
    }
    else
    {
      curatt = ((gvp->sgp->numval/gvp->zoom)-(gvp->width));
    }
  }

  Reset (gvp->viewer);
  Update ();
  AttachPicture (gvp->viewer, gvp->seg, curatt, INT4_MIN, UPPER_LEFT,
                 1, 1, DrawPercent);
  sb = GetSlateHScrollBar ((SlatE) gvp->viewer);
  newsbmax = (Int4) GetBarMax (sb);
  if (oldsbmax > 0)
    CorrectBarValue (sb, (Int2) (newsbmax*oldsbpos/oldsbmax));
  else
    CorrectBarValue (sb, 0);
  return;
}

extern void BioseqPtrToGraphViewForm (ForM f, Pointer data)
{
  GraphViewFormPtr gvp;
  SeqGraphPtr      sgp;
  VieweR           v1;
  GrouP            g1, g2, g3, g4;
  GrouP            c;
  ButtoN           b1, b2, ac, cc;
  SegmenT          s1;
  DialoG           cutoffs;
  Char             buf[8];
  Int2             startv = 20;
  FloatHi          maxVal, minVal;

  WatchCursor ();

  gvp = (GraphViewFormPtr) GetObjectExtra (f);
  sgp = (SeqGraphPtr) data;
  if (gvp != NULL && sgp != NULL)
  {
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
    gvp->width = (Int4) (sgp->numval/sgp->compr);
    gvp->height = (((Int4) maxVal - (Int4) minVal) *
                  (Int4) sgp->a) + (Int4) sgp->b;

    s1 = gvp->seg;
    if ((s1 = DrawSeqGraph (sgp, gvp->window, gvp->graphtype,
                            gvp->margin, s1, gvp->zoom)) != NULL)
    {
      if (gvp->seg == NULL)
      {
        g1 = HiddenGroup ((WindoW) gvp->form, -1, 0, NULL);
        g2 = HiddenGroup (g1, 2, 0, NULL);
        v1 = CreateViewer (g2, (Int2) (gvp->width+SCALESPACE),
                           (Int2) (gvp->height+SCALESPACE), FALSE, TRUE);
        SetObjectExtra (v1, gvp, NULL);
        gvp->viewer = v1;
        gvp->flagNewClick = TRUE;
        AttachPicture (v1, s1, INT4_MIN, INT4_MIN, UPPER_LEFT,
                       1, 1, DrawPercent);

          g3 = HiddenGroup (g2, -2, 0, NULL);
          SetGroupSpacing (g3, 20, 5);
          gvp->pc = DialogText (g3, "", 2, ScrollTextScrollProc);
          SetObjectExtra (gvp->pc, gvp, NULL);
          gvp->br = ScrollBar (g3, 0, 2, ScrollBarScrollProc);
          SetObjectExtra (gvp->br, gvp, NULL);
          CorrectBarMax (gvp->br, 100);
          CorrectBarValue (gvp->br, (Int2) (100-startv));
          sprintf (buf, "%3d", startv);
          SetTitle (gvp->pc, buf);
          gvp->level = startv;

          g4 = HiddenGroup (g3, -1, 0, NULL);
          ac = PushButton (g4, "Accept", ChangePercentProc);
          SetObjectExtra (ac, gvp->pc, NULL);
          cc = PushButton (g4, "Cut Off", CutOffProc);
          SetObjectExtra (cc, gvp, NULL);
          cutoffs = CreateCutoffsDialog (g4, 5, 3, (Boolean)
                    ((gvp->graphtype == GRAPH_MATRIX_AA ||
                      gvp->graphtype == GRAPH_MATRIX_NA) ? TRUE : FALSE));
          gvp->cutoffs = cutoffs;

          AlignObjects (ALIGN_CENTER, (HANDLE) ac, (HANDLE) cc,
                        (HANDLE) cutoffs, NULL);
          AlignObjects (ALIGN_CENTER, (HANDLE) gvp->pc, (HANDLE) gvp->br,
                        (HANDLE) g4, NULL);
          AlignObjects (ALIGN_MIDDLE, (HANDLE) v1, (HANDLE) g3, NULL);

        c = HiddenGroup (g1, 3, 0, NULL);
        b1 = PushButton (c, "Close", CloseGraphViewProc);
        SetObjectExtra (b1, gvp, NULL);
/*        SetObjectExtra ((WindoW) gvp->form, gvp, CleanupGraphViewForm); */
        gvp->scale = PopupList (c, TRUE, ScaleGraphView);
        InitZoomPopup (gvp, sgp);
        SetObjectExtra (gvp->scale, gvp, NULL);
        b2 = PushButton (c, "SeqLoc", SeqLocGraphViewProc);
        SetObjectExtra (b2, gvp, NULL);
        AlignObjects (ALIGN_VERTICAL, (HANDLE) b1, (HANDLE) gvp->scale,
                      (HANDLE) b2, NULL);
        AlignObjects (ALIGN_CENTER, (HANDLE) g2, (HANDLE) c, NULL);
        if (gvp->graphtype == GRAPH_MATRIX_AA ||
            gvp->graphtype == GRAPH_MATRIX_NA)
          SetPanelClick ((PaneL) v1, HistoClickProc, NULL, NULL, NULL);
        else if (gvp->graphtype == GRAPH_FILTER)
          SetPanelClick ((PaneL) v1, GraphClickProc, NULL, NULL, NULL);
        else
          SetPanelClick ((PaneL) v1, NULL, NULL, NULL, NULL);
      }
      gvp->seg = s1;
      RealizeWindow ((WindoW) gvp->form);
    }
  }
  ArrowCursor ();
  return;
}

extern GraphViewFormPtr GraphViewFormNew (void)
{
  GraphViewFormPtr  gvp;

  gvp = (GraphViewFormPtr) MemNew (sizeof (GraphViewForm));
  if (gvp != NULL)
  {
    gvp->sgp = NULL;
    gvp->height = 0;
    gvp->width = 0;
    gvp->seg = NULL;
    gvp->margin = 0;
    gvp->zoom = 0;
    gvp->seqloc = NULL;
  }
  return gvp;
}

extern GraphViewFormPtr GraphViewFormFree (GraphViewFormPtr gvp,
                                           Boolean flagFree)
{
  SeqLocPtr slp, tempslp;

  gvp->sgp->next = SeqGraphFree (gvp->sgp->next);
  gvp->sgp = SeqGraphFree (gvp->sgp);
  gvp->seg = DeletePicture (gvp->seg);

  slp = gvp->seqloc;
  while (slp != NULL)
  {
    tempslp = slp->next;
    SeqLocFree (slp);
    slp = tempslp;
  }
/* don't mess with the object manager
  slp = gvp->slp;
  while (slp != NULL)
  {
    tempslp = slp->next;
    slp = SeqLocFree (slp);
    slp = tempslp;
  }
*/
  if (flagFree)
    gvp = MemFree (gvp);
  return gvp;
}

static void CloseGraphViewFormProc (WindoW w)
{
  ObjMgrDeSelectAll ();
  Remove (w);
  return;
}

static void ResizeGraphViewForm (WindoW w)
{
  Update ();
  return;
}

extern ForM CreateGraphViewForm (Int2 left, Int2 top, CharPtr title,
                                 BioseqPtr bsp, Uint1 graphtype)
{
  WindoW           w;
  GraphViewFormPtr gvp;

  w = NULL;
  if (bsp == NULL)
    return (ForM) w;
  gvp = GraphViewFormNew ();
  if (gvp != NULL)
  {
    gvp->sgp = NULL;
    gvp->height = 0;
    gvp->width = 0;
    gvp->seg = NULL;
    w = DocumentWindow (left, top, -10, -10, title,
                        CloseGraphViewFormProc, ResizeGraphViewForm);
    SetObjectExtra (w, gvp, CleanupGraphViewForm);
    gvp->form = (ForM) w;
    gvp->actproc = NULL;
    gvp->toform = BioseqPtrToGraphViewForm;
    gvp->bsp = bsp;
    gvp->entityID = 0;
    gvp->itemID = 0;
    gvp->slp = NULL;
  }
  return (ForM) w;
}
