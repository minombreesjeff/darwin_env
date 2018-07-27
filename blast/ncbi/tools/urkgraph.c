static char const rcsid[] = "$Id: urkgraph.c,v 6.7 2003/05/30 17:25:38 coulouri Exp $";

/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name: urkgraph.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.7 $
*
* File Description: graph utilities
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkgraph.c,v $
* Revision 6.7  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.6  1998/12/09 20:38:45  kans
* changed compl to compr to avoid new c++ symbol collision
*
* Revision 6.5  1998/09/16 18:03:35  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <ncbibs.h>
#include <objres.h>
#include <picture.h>

#include <urkgraph.h>

static FloatHi GetFloatHi (Pointer data, Uint1 yvaluetype, Int4 pos)
{
  FloatHi       fval;
  ByteStorePtr  bp;

  switch (yvaluetype)
  {
   default:
   case GRAPH_Y_FloatHi:
    fval = (FloatHi) (((FloatHiPtr) data)[pos]);
    break;
   case GRAPH_Y_Int4:
    fval = (FloatHi) (((Int4Ptr) data)[pos]);
    break;
   case GRAPH_Y_BS:
    bp = (ByteStorePtr) data;
    BSSeek (bp, pos, SEEK_SET);
    fval = (FloatHi) BSGetByte (bp);
    break;
  }
  return fval;
}

extern SeqGraphPtr CopyGraph (SeqGraphPtr sgpold)
{
  SeqGraphPtr sgp = NULL;

  if (sgpold == NULL)
    return sgp;

  sgp = (SeqGraphPtr) AsnIoMemCopy ((Pointer) sgpold,
                                    (AsnReadFunc) SeqGraphAsnRead,
                                    (AsnWriteFunc) SeqGraphAsnWrite);
  sgp->next = NULL;
  return sgp;
}

extern GraphInfoPtr GraphInfoNew (GraphInfoPtr gip,
                                  Uint1 yvaluetype,
                                  Int4 number_x_points,
                                  Int4 graphpixelwidth,
                                  Int4 graphpixelheight)
{
  GraphInfoPtr gipnew;

  gipnew = GraphInfoNewEP (yvaluetype, 0, number_x_points-1,
                           graphpixelwidth, graphpixelheight);

  if (gip == NULL || gipnew == NULL)
    return gipnew;
  while (gip->next != NULL)
    gip = gip->next;
  gipnew->prev = gip;
  gip->next = gipnew;
  return gipnew;
}

extern GraphInfoPtr GraphInfoNewEP (Uint1 yvaluetype,
                                    Int4 startplot, Int4 stopplot,
                                    Int4 graphpixelwidth,
                                    Int4 graphpixelheight)
{
  GraphInfoPtr gip;

  if (startplot < 0 || stopplot < 0)
    return NULL;
  if (stopplot <= startplot)
    return NULL;
  if (graphpixelheight <= 0 || graphpixelwidth <= 0)
    return NULL;
  if ((gip = (GraphInfoPtr) MemNew (sizeof (GraphInfo))) == NULL)
    return gip;

  gip->graphtype = GRAPH;
  gip->yvaluetype = yvaluetype;
  gip->window = 0;
  gip->startplot = startplot;
  gip->stopplot = stopplot;
  gip->imaxval = 0;
  gip->iminval = 0;
  gip->iaxisval = 0;
  gip->fmaxval = 0.0;
  gip->fminval = 0.0;
  gip->faxisval = 0.0;
  gip->graphpixelwidth = graphpixelwidth;
  gip->graphpixelheight = graphpixelheight;
  gip->horiz_offset = 0;
  gip->vert_offset = 0;
  gip->localmaxzoom = 0;
  gip->flagPlotZero = TRUE;
  gip->flagAnchorPt = FALSE;
  gip->flagInvert = FALSE;
  gip->flagLogScaleY = FALSE;
  gip->flagCleanupSGP = TRUE;
  gip->sgp = NULL;
  gip->next = NULL;
  gip->prev = NULL;

  gip->localmaxzoom = (gip->stopplot - gip->startplot + 1) /
                       gip->graphpixelwidth;
  if ((gip->stopplot - gip->startplot + 1) %
       gip->graphpixelwidth != 0)
  {
    gip->localmaxzoom += 1;
  }

  return gip;
}

extern GraphInfoPtr GraphInfoFree (GraphInfoPtr gip)
{
  GraphInfoPtr gipn;

  while (gip != NULL)
  {
    gipn = gip->next;
    if (gip->flagCleanupSGP)
      SeqGraphFree (gip->sgp);
    MemFree (gip);
    gip = gipn;
  }
  return gip;
}

extern GraphInfoPtr GraphInfoFreeAll (GraphInfoPtr gip)
{
  if (gip == NULL)
    return gip;

  while (gip->prev != NULL)
    gip = gip->prev;

  return GraphInfoFree (gip);
}

extern void MinMaxGraph (Uint1 yvaluetype, Pointer score,
                         GraphInfoPtr gip)
{
  if (yvaluetype == GRAPH_Y_FloatHi)
    MinMaxGraphX (yvaluetype, score, (Pointer) &(gip->fmaxval),
                  (Pointer) &(gip->fminval), (Pointer) &(gip->faxisval),
                  gip->startplot, gip->stopplot);
  else
    MinMaxGraphX (yvaluetype, score, (Pointer) &(gip->imaxval),
                  (Pointer) &(gip->iminval), (Pointer) &(gip->iaxisval),
                  gip->startplot, gip->stopplot);
  return;
}

extern void MinMaxGraphX (Uint1 yvaluetype, Pointer score,
                          Pointer maxval, Pointer minval, Pointer axisval,
                          Int4 startplot, Int4 stopplot)
{
  Int4         i, value;
  ByteStorePtr bp;

  if (score == NULL)
    return;

  switch (yvaluetype)
  {
   default:
   case GRAPH_Y_FloatHi:
    * (FloatHiPtr) maxval = * (FloatHiPtr) score;
    * (FloatHiPtr) minval = * (FloatHiPtr) score;
    for (i = startplot; i <= stopplot; i++)
    {
      if (((FloatHiPtr) score)[i] > * (FloatHiPtr) maxval)
        * (FloatHiPtr) maxval = ((FloatHiPtr) score)[i];
      if (((FloatHiPtr) score)[i] < * (FloatHiPtr) minval)
        * (FloatHiPtr) minval = ((FloatHiPtr) score)[i];
    }
    break;
   case GRAPH_Y_Int4:
    * (Int4Ptr) maxval = * (Int4Ptr) score;
    * (Int4Ptr) minval = * (Int4Ptr) score;
    for (i = startplot; i <= stopplot; i++)
    {
      if (((Int4Ptr) score)[i] > * (Int4Ptr) maxval)
        * (Int4Ptr) maxval = ((Int4Ptr) score)[i];
      if (((Int4Ptr) score)[i] < * (Int4Ptr) minval)
        * (Int4Ptr) minval = ((Int4Ptr) score)[i];
    }
   case GRAPH_Y_BS:
    bp = (ByteStorePtr) score;
    BSSeek (bp, startplot, SEEK_SET);
    value = (Int4) BSGetByte (bp);
    * (Int4Ptr) maxval = value;
    * (Int4Ptr) minval = value;
    for (i = startplot; i <= stopplot; i++)
    {
      BSSeek (bp, i, SEEK_SET);
      value = (Int4) BSGetByte (bp);
      if (value > * (Int4Ptr) maxval)
        * (Int4Ptr) maxval = value;
      if (value < * (Int4Ptr) minval)
        * (Int4Ptr) minval = value;
    }
    if (* (Int4Ptr) minval <= 0 && * (Int4Ptr) maxval >= 0)
      * (Int4Ptr) axisval = 0;
    else
      * (Int4Ptr) axisval = * (Int4Ptr) minval;
    break;
  }
  return;
}

extern SeqGraphPtr DevelopGraph (Uint1 yvaluetype, Pointer score,
                                 GraphInfoPtr gip)
{
  SeqGraphPtr sgp;

  if (yvaluetype == GRAPH_Y_FloatHi)
    sgp = DevelopGraphX (gip->yvaluetype, score,
                         gip->startplot, gip->stopplot,
                         (Pointer) &(gip->fmaxval),
                         (Pointer) &(gip->fminval),
                         (Pointer) &(gip->faxisval),
                         gip->graphpixelwidth);
  else
    sgp = DevelopGraphX (gip->yvaluetype, score,
                         gip->startplot, gip->stopplot,
                         (Pointer) &(gip->imaxval),
                         (Pointer) &(gip->iminval),
                         (Pointer) &(gip->iaxisval),
                         gip->graphpixelwidth);

  return sgp;
}

extern SeqGraphPtr DevelopGraphX (Uint1 yvaluetype, Pointer score,
                                  Int4 start, Int4 stop,
                                  Pointer maxval, Pointer minval,
                                  Pointer axisval,
                                  Int4 graphpixelwidth)
{
  SeqGraphPtr  sgp;

  if (score == NULL)
    return NULL;

  if ((sgp = SeqGraphNew ()) != NULL)
  {
/* y-values passed and stored as void pointer */
    sgp->values = score;
/* number of values, compression, scaling and transformation */
    sgp->numval = stop - start + 1;
    sgp->compr = (Int4) (sgp->numval / graphpixelwidth);
    if ((sgp->numval%graphpixelwidth) != 0)
      sgp->compr += 1;
/* will always check compression */
    sgp->flags[0] = 1;
/* will always check scaling and transformation */
    sgp->flags[1] = 1;
    sgp->a = 1;                 /* scale factor */
    sgp->b = 0;                 /* transform offset */
/* y value storage type */
    sgp->flags[2] = yvaluetype;
/* min/max */
    switch (yvaluetype)
    {
     default:
     case GRAPH_Y_FloatHi:
      sgp->max.realvalue = * (FloatHiPtr) maxval;
      sgp->min.realvalue = * (FloatHiPtr) minval;
      sgp->axis.realvalue = * (FloatHiPtr) axisval;
      break;
     case GRAPH_Y_Int4:
     case GRAPH_Y_BS:
      sgp->max.intvalue = * (Int4Ptr) maxval;
      sgp->min.intvalue = * (Int4Ptr) minval;
      sgp->axis.intvalue = * (Int4Ptr) axisval;
      break;
    }
  }
  return sgp;
}

extern void AnchorPoint (SegmenT s, GraphInfoPtr gip)
{
  Uint2       primID = 10;
  SeqGraphPtr sgp;

  AddAttribute (s, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                WHITE_COLOR, SOLID_LINE, SOLID_SHADING, STD_PEN_WIDTH, 0);

  sgp = gip->sgp;
  AddLine (s,
           gip->horiz_offset +
            0,
           gip->vert_offset +
            ((Int4) (((gip->graphpixelheight+1) * sgp->a) + (Int4) sgp->b)),
           gip->horiz_offset +
            0,
           gip->vert_offset +
            ((Int4) (((gip->graphpixelheight+1) * sgp->a) + (Int4) sgp->b)),
           FALSE, primID);

  AddAttribute (s, (COLOR_ATT|STYLE_ATT|SHADING_ATT|WIDTH_ATT),
                BLACK_COLOR, SOLID_LINE, SOLID_SHADING, STD_PEN_WIDTH, 0);
  return;
}

extern SegmenT DrawGraph (SegmenT seg, SeqGraphPtr sgp, GraphInfoPtr gip)
{
  if (sgp == NULL || gip == NULL)
    return seg;

  seg = DrawGraphX (seg, sgp,
                    gip->graphtype, gip->startplot,
                    gip->window, gip->graphpixelheight,
                    gip->horiz_offset, gip->vert_offset,
                    gip->flagPlotZero, gip->flagInvert);
  return seg;
}

extern SegmenT DrawGraphSGPAsIs (SegmenT seg, SeqGraphPtr sgp,
                                 Int4 graphtype,
                                 Int4 horiz_offset, Int4 vert_offset)
{
  Int4     imaxval = 0, iminval = 0, iaxisval = 0;
  FloatHi  fmaxval = 0, fminval = 0, faxisval = 0;
  Int4     graphpixelwidth, graphpixelheight;

  if (sgp == NULL)
    return seg;

  if (sgp->flags[0] == 0)
  {
    sgp->flags[0] = 1;
    sgp->compr = 1;
  }
  if (sgp->flags[1] == 0)
  {
    sgp->flags[1] = 1;
    sgp->a = 1;                 /* scale factor */
    sgp->b = 0;                 /* transform offset */
  }
  switch (sgp->flags[2])
  {
   default:
   case GRAPH_Y_FloatHi:
    if (sgp->max.realvalue == sgp->min.realvalue)
    {
      MinMaxGraphX (sgp->flags[2], sgp->values,
                    (Pointer) &fmaxval, (Pointer) &fminval,
                    (Pointer) &faxisval, 0, sgp->numval-1);
      sgp->max.realvalue = fmaxval;
      sgp->min.realvalue = fminval;
      graphpixelheight = (Int4) sgp->max.realvalue -
                         (Int4) sgp->min.realvalue + 1;
    }
    break;
   case GRAPH_Y_Int4:
   case GRAPH_Y_BS:
    if (sgp->max.intvalue == sgp->min.intvalue)
    {
      MinMaxGraphX (sgp->flags[2], sgp->values,
                    (Pointer) &imaxval, (Pointer) &iminval,
                    (Pointer) &iaxisval, 0, sgp->numval-1);
      sgp->max.intvalue = imaxval;
      sgp->min.intvalue = iminval;
      graphpixelheight = sgp->max.intvalue - sgp->min.intvalue + 1;
    }
    break;
  }
  graphpixelwidth = sgp->numval;
  seg = DrawGraphSGP (seg, sgp, graphtype,
                      graphpixelwidth, graphpixelheight,
                      horiz_offset, vert_offset);
  return seg;
}

extern SegmenT DrawGraphSGP (SegmenT seg, SeqGraphPtr sgp, Int4 graphtype,
                             Int4 graphpixelwidth, Int4 graphpixelheight,
                             Int4 horiz_offset, Int4 vert_offset)
{
  Int4     imaxval = 0, iminval = 0, iaxisval = 0;
  FloatHi  fmaxval = 0, fminval = 0, faxisval = 0;

  if (sgp == NULL)
    return seg;

  sgp->compr = (Int4) (sgp->numval / graphpixelwidth);
  if ((sgp->numval%graphpixelwidth) != 0)
    sgp->compr += 1;
  sgp->flags[0] = 1;

  sgp->a = 1;                 /* scale factor */
  sgp->b = 0;                 /* transform offset */
  sgp->flags[1] = 1;

  switch (sgp->flags[2])
  {
   default:
   case GRAPH_Y_FloatHi:
    if (sgp->max.realvalue == sgp->min.realvalue)
    {
      MinMaxGraphX (sgp->flags[2], sgp->values,
                    (Pointer) &fmaxval, (Pointer) &fminval,
                    (Pointer) &faxisval, 0, sgp->numval-1);
      sgp->max.realvalue = fmaxval;
      sgp->min.realvalue = fminval;
    }
    break;
   case GRAPH_Y_Int4:
   case GRAPH_Y_BS:
    if (sgp->max.intvalue == sgp->min.intvalue)
    {
      MinMaxGraphX (sgp->flags[2], sgp->values,
                    (Pointer) &imaxval, (Pointer) &iminval,
                    (Pointer) &iaxisval, 0, sgp->numval-1);
      sgp->max.intvalue = imaxval;
      sgp->min.intvalue = iminval;
    }
    break;
  }

  seg = DrawGraphX (seg, sgp, graphtype, 0, 0, graphpixelheight,
                    horiz_offset, vert_offset, TRUE, FALSE);
  return seg;
}

extern SegmenT DrawGraphX (SegmenT seg, SeqGraphPtr sgp,
                           Int4 graphtype, Int4 startplot,
                           Int4 window, Int4 graphpixelheight,
                           Int4 horiz_offset, Int4 vert_offset,
                           Boolean flagPlotZero, Boolean flagInvert)
{
  Pointer values;
  Int4    ix, compression, icomp, datapos;
  Int4    i, istop, iwin;
  FloatHi maxVal, minVal, fval, fmin, fmax, fthis, fnext, fpmax, fpmin;
  Uint2   primID;
  Boolean flagPrevMax, flagPrevMin;

  if (sgp == NULL)
    return seg;

  if (seg == NULL)
    seg = CreatePicture ();

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

  if (sgp->compr > 0)
    compression = (Int4) sgp->compr;
  else
    compression = 1;

  values = sgp->values;

  ix = 0;
  icomp = 0;
  datapos = startplot;
  flagPrevMax = FALSE;
  flagPrevMin = FALSE;
  fval = GetFloatHi (values, sgp->flags[2], datapos);
  iwin = window / 2;
  for (i = 0; i < iwin; i++)
  {
    icomp++;
    if (icomp == compression)
    {
      icomp = 0;
      ix++;
    }
    datapos++;
    fval = GetFloatHi (values, sgp->flags[2], datapos);
  }

  fmin = fval;
  fmax = fval;
  if (iwin > 0 && window % 2 == 0)
    iwin--;
  istop = sgp->numval - iwin - 1;

  for (i = datapos; i <= istop; i++)
  {
    icomp++;
    if (fval < fmin)
      fmin = fval;
    if (fval > fmax)
      fmax = fval;
    fpmax = (fmax - minVal) / (maxVal - minVal) * graphpixelheight;
    fpmin = (fmin - minVal) / (maxVal - minVal) * graphpixelheight;
    if (icomp == compression || i == istop)
    {
      switch (graphtype)
      {
       default:
       case GRAPH_LINE:
        if (fmax <= maxVal && fmin >= minVal)
        {
          primID = 1;
          if (flagPrevMax)
            fpmax = (FloatHi) graphpixelheight;
          if (flagPrevMin)
            fpmin = 0.0;
          flagPrevMax = FALSE;
          flagPrevMin = FALSE;
          AddLine (seg,
                   horiz_offset +
                    ix,
                   vert_offset +
                    ((Int4) ((fpmax * sgp->a) + (Int4) sgp->b)),
                   horiz_offset +
                    ix,
                   vert_offset +
                    ((Int4) ((fpmin * sgp->a) + (Int4) sgp->b)),
                   FALSE, primID);
          if (i != istop)
          {
            fthis = (fval-minVal) / (maxVal-minVal) * graphpixelheight;
            fnext = GetFloatHi (values, sgp->flags[2], datapos+1);
            if (fnext > maxVal)
              fnext = maxVal;
            if (fnext < minVal)
              fnext = minVal;
            fnext = (fnext-minVal) / (maxVal-minVal) * graphpixelheight;
            primID = 1;
            AddLine (seg,
                     horiz_offset +
                      ix,
                     vert_offset +
                      ((Int4) ((fthis * sgp->a) + (Int4) sgp->b)),
                     horiz_offset +
                      ix + 1,
                     vert_offset +
                      ((Int4) ((fnext * sgp->a) + (Int4) sgp->b)),
                     FALSE, primID);
          }
        }
        else
        {
          flagPrevMax = FALSE;
          flagPrevMin = FALSE;
          if (fmax > maxVal)
            flagPrevMax = TRUE;
          if (fmin < minVal)
            flagPrevMin = TRUE;
        }
        break;
       case GRAPH_HISTO:
        fpmin = 0.0;
        if (!flagPlotZero && fpmax > 0.0)
        {
          if (fmax > maxVal)
            fpmax = (FloatHi) graphpixelheight;
          if (flagInvert)
          {
            fpmin = (FloatHi) graphpixelheight;
            fpmax = fpmin - fpmax;
          }
          if (flagInvert)
            primID = 2;
          else
            primID = 1;
          AddLine (seg,
                   horiz_offset +
                    ix,
                   vert_offset +
                    ((Int4) ((fpmax * sgp->a) + (Int4) sgp->b)),
                   horiz_offset +
                    ix,
                   vert_offset +
                    ((Int4) ((fpmin * sgp->a) + (Int4) sgp->b)),
                   FALSE, primID);
        }
        break;
       case GRAPH_POINT:
        primID = 1;
        if (fmax > maxVal)
        {
          AddLine (seg,
                   horiz_offset +
                    ix,
                   vert_offset +
                    ((Int4) ((fpmax * sgp->a) + (Int4) sgp->b)),
                   horiz_offset +
                    ix,
                   vert_offset +
                    ((Int4) ((fpmax * sgp->a) + (Int4) sgp->b)),
                   FALSE, primID);
        }
        if (fmin < minVal)
        {
          AddLine (seg,
                   horiz_offset +
                    ix,
                   vert_offset +
                    ((Int4) ((fpmin * sgp->a) + (Int4) sgp->b)),
                   horiz_offset +
                    ix,
                   vert_offset +
                    ((Int4) ((fpmin * sgp->a) + (Int4) sgp->b)),
                   FALSE, primID);
        }
        break;
      }
      if (i != istop)
      {
        fval = GetFloatHi (values, sgp->flags[2], datapos+1);
      }
      fmin = fval;
      fmax = fval;
      icomp = 0;
      ix++;
    }
    if (i != istop)
    {
      datapos++;
      fval = GetFloatHi (values, sgp->flags[2], datapos);
    }
  }
  return seg;
}

extern SegmenT Xscale (SegmenT seg, SeqGraphPtr sgp, GraphInfoPtr gip)
{
  if (sgp == NULL || gip == NULL)
    return seg;

  seg = XscaleX (seg, (Int4) sgp->compr, (Int4) sgp->a, (Int4) sgp->b,
                 gip->startplot, gip->stopplot,
                 gip->horiz_offset, gip->vert_offset);
  return seg;
}

extern SegmenT Yscale (SegmenT seg, SeqGraphPtr sgp, GraphInfoPtr gip)
{
  FloatHi minVal, maxVal;

  if (sgp == NULL || gip == NULL)
    return seg;

  switch (sgp->flags[2])
  {
   default:
   case 1:
    minVal = sgp->min.realvalue;
    maxVal = sgp->max.realvalue;
    break;
   case 2:
   case 3:
    minVal = (FloatHi) sgp->min.intvalue;
    maxVal = (FloatHi) sgp->max.intvalue;
    break;
  }

  seg = YscaleX (seg, (Int4) sgp->a, (Int4) sgp->b, minVal, maxVal,
                 (Int4) sgp->compr, gip->graphpixelheight,
                 gip->startplot, gip->stopplot,
                 gip->horiz_offset, gip->vert_offset);
  return seg;
}

extern SegmenT XscaleX (SegmenT seg, Int4 compression,
                        Int4 scale, Int4 trans,
                        Int4 startplot, Int4 stopplot,
                        Int4 horiz_offset, Int4 vert_offset)
{
  Int4     numval, ticks1, ticks2, val, end, top, bottom;
  Char     tickpos[16];

  if (seg == NULL)
    return seg;

  numval = stopplot - startplot + 1;
  ticks1 = 100;
  ticks2 = 10;
  val = startplot;
  end = startplot + (numval/compression);
  while (val < end)
  {
    if ((val % (ticks1/2)) == 0)
    {
      while (((val*compression) % ticks2) != 0)
        val++;
      top = vert_offset + (-2 * scale) + trans;
      bottom = vert_offset + (-6 * scale) + trans;
      AddLine (seg, horiz_offset + val, top, horiz_offset + val, bottom,
               FALSE, 0);
      if ((val % ticks1) == 0)
      {
        sprintf (tickpos, "%ld", (long) (val*compression));
        top = vert_offset + (-22 * scale) + trans;
        AddTextLabel (seg, horiz_offset + val, top,
                      tickpos, NULL, 0, UPPER_CENTER, 0);
      }
    }
    val++;
  }
  return seg;
}

extern SegmenT YscaleX (SegmenT seg, Int4 scale, Int4 trans,
                        FloatHi minval, FloatHi maxval,
                        Int4 compression, Int4 graphpixelheight,
                        Int4 startplot, Int4 stopplot,
                        Int4 horiz_offset, Int4 vert_offset)
/*
                        Boolean flagLogScaleY, Boolean flagInvert)
*/
{
  FloatHi  range, fmax, fmin, tick, tickinc, ftick;
  Int4     tickcount, itick, imin, iloop;
  Int4     graphpixelwidth;
  long int tickextrememax, tickextrememin;
  Char     tickpos[16], tickstring[16];

  if (seg == NULL)
    return seg;

  graphpixelwidth = (stopplot - startplot) / compression;
  tickcount = graphpixelheight / 20;
  range = maxval - minval;

  tickextrememax = 2;
  fmax = maxval;
  while (fmax > 1.0)
  {
    tickextrememax++;
    fmax /= 10;
  }
  tickextrememin = 0;
  fmin = minval;
  while (fmin < 0.0)
  {
    tickextrememin++;
    fmin *= 10;
  }

  StrCpy (tickstring, "%");
  sprintf (tickpos, "%ld", (tickextrememax + tickextrememin));
  StrCat (tickstring, tickpos);
  StrCat (tickstring, ".");
  sprintf (tickpos, "%ld", tickextrememin);
  StrCat (tickstring, tickpos);
  StrCat (tickstring, "f");

  if (tickcount < 3)
  {
    AddLine (seg, horiz_offset + graphpixelwidth + 4,
                  vert_offset + (graphpixelheight * scale) + trans,
                  horiz_offset + graphpixelwidth + 8,
                  vert_offset + (graphpixelheight * scale) + trans, FALSE, 0);
    sprintf (tickpos, tickstring, maxval);
    AddTextLabel (seg, horiz_offset + graphpixelwidth + 10,
                  vert_offset + (graphpixelheight * scale) + trans,
                  tickpos, NULL, 0, MIDDLE_RIGHT, 0);
    AddLine (seg, horiz_offset + graphpixelwidth + 4,
                  vert_offset + trans,
                  horiz_offset + graphpixelwidth + 8,
                  vert_offset + trans, FALSE, 0);
    sprintf (tickpos, tickstring, minval);
    AddTextLabel (seg, horiz_offset + graphpixelwidth + 10,
                  vert_offset + trans,
                  tickpos, NULL, 0, MIDDLE_RIGHT, 0);
    return seg;
  }

/*
 don't know if you can depend on floor, ceiling, fmod
                   can trust the power function -- simple math anyhow
*/

  tickinc = range / (FloatHi) tickcount;
  if (tickinc > 1.0)
  {
    fmin = tickinc;
    imin = 0;
    while (fmin > 1.0)
    {
      fmin /= 10.0;
      imin++;
    }
    fmin *= 10.0;
    imin--;
    itick = (Int4) fmin;
    fmin -= (FloatHi) itick;
    if (fmin >= 0.5)
      itick++;
    tickinc = (FloatHi) itick;
    for (iloop = 0; iloop < imin; iloop++)
      tickinc *= 10.0;
  }
  else
  {
    fmin = tickinc;
    imin = 0;
    while (fmin < 1.0)
    {
      fmin *= 10.0;
      imin++;
    }
    itick = (Int4) fmin;
    fmin -= (FloatHi) itick;
    if (fmin >= 0.5)
      itick++;
    tickinc = (FloatHi) itick;
    for (iloop = 0; iloop < imin; iloop++)
      tickinc /= 10.0;
  }
  fmin = minval / tickinc;
  if (fmin >= 0.0)
  {
    if (fmin == 1.0)
    {
      tick = fmin;
    }
    else
    {
      imin = (Int4) fmin;
      imin++;
      tick = tickinc * (FloatHi) imin;
    }
  }
  else
  {
    tick = tickinc;
  }

  while (tick <= maxval)
  {
    ftick = (tick - minval) / range * graphpixelheight;
    AddLine (seg, horiz_offset + graphpixelwidth + 4,
                  vert_offset + (Int4) ((ftick * scale) + trans),
                  horiz_offset + graphpixelwidth + 8,
                  vert_offset + (Int4) ((ftick * scale) + trans),
                  FALSE, 0);
    sprintf (tickpos, tickstring, tick);
    AddTextLabel (seg, horiz_offset + graphpixelwidth + 10,
                  vert_offset + (Int4) ((ftick * scale) + trans),
                  tickpos, NULL, 0, MIDDLE_RIGHT, 0);
    tick += tickinc;
  }

  return seg;
}
