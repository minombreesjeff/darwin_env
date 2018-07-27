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
* File Name: dxydraw.c
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.6 $
*
* File Description: sentinel dot plots
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: gxydraw.c,v $
* Revision 6.6  1999/10/04 17:46:23  kans
* include vibrant.h
*
* Revision 6.5  1998/09/16 19:00:37  kuzio
* cvs logs
*
* ==========================================================================
*/

#include <vibrant.h>
#include <seqport.h>
#include <drawingp.h>
#include <gxydraw.h>

/* common */

static void CleanGSP (BigScalar calldata)
{
  GraphXYSentPtr gsp;

  if ((gsp = (GraphXYSentPtr) calldata) == NULL)
    return;
  MemFree (gsp->endpointarray);
  MemFree (gsp->endpointvalues);
  MemFree (gsp);
  return;
}

static void TickLabel (Int4 tickvalue, Int2 xcoord, Int2 ycoord)
{
  Char numberbuffer[32];

  sprintf (numberbuffer, "%8ld", (long) tickvalue);
  PaintStringEx (numberbuffer, xcoord, ycoord);

  return;
}

/* dot plot */

static void PlotTheDotXYPlot (Int4Ptr epa, Int4 xaxis, Int4 yaxis,
                              Int4 xscale, Int4 yscale, Int4Ptr endpoints,
                              Int4 cutoff, AttPData PNTR curattrib,
                              Boolean flagIsGUI)
{
  Int4    ix, iy;
  PoinT   point;
  Int4    xoffset, yoffset, xcorrect, ycorrect;
  Int4    loop, curcut;
  Uint1   curR, curG, curB;
  Int4    window;
  Int4Ptr curepa;

  xoffset = 50;
  yoffset = 50;

  curR = curattrib->color[0];
  curG = curattrib->color[1];
  curB = curattrib->color[2];

  window = 8;
  curepa = epa;
  for (loop = 0; loop <= window; loop++)
  {
    if (loop >= cutoff)
    {
      epa = curepa;
      curcut = loop * ((256/window) - 1);
      curcut = 255 - curcut;
      if (curcut < 0)
        curcut = 0;
      Nlm_SelectColor (255, (Uint1) curcut, 0);
      for (iy = 0; iy < yaxis; iy++)
      {
        for (ix = 0; ix < xaxis; ix++)
        {
          if (*epa == loop)
          {
            point.x = (Int2) (ix + ((window-1)/xscale) + xoffset);
            point.y = (Int2) (iy + ((window-1)/yscale) + yoffset);
            DrawLine (point, point);
          }
          epa++;
        }
      }
    }
  }
  Nlm_SelectColor (curR, curG, curB);

  if (endpoints != NULL)
  {
    if (flagIsGUI)
    {
      xcorrect = 25;
      ycorrect = 50;
    }
    else
    {
      xcorrect = 50;
      ycorrect = 60;
    }
    TickLabel (endpoints[0], (Int2) (xoffset-xcorrect),
                             (Int2) (yoffset-10));
    TickLabel (endpoints[1], (Int2) (xoffset-xcorrect+xaxis),
                             (Int2) (yoffset-10));
    TickLabel (endpoints[2], (Int2) (xoffset-ycorrect),
                             (Int2) (yoffset+10));
    TickLabel (endpoints[3], (Int2) (xoffset-ycorrect),
                             (Int2) (yoffset+yaxis));
  }

  return;
}

static void DotXYSentProc (BigScalar calldata, PrimDrawContext pdc)
{
  GraphXYSentPtr gsp;
  DrawInfoPtr    dInfoPtr;

  if ((gsp = (GraphXYSentPtr) calldata) == NULL)
    return;
  if (gsp->endpointarray == NULL)
    return;

  dInfoPtr = (DrawInfoPtr) pdc;
  PlotTheDotXYPlot (gsp->endpointarray, gsp->xaxiscount, gsp->yaxiscount,
                    gsp->xscale, gsp->yscale, gsp->endpointvalues,
                    gsp->dotcutoff, &(dInfoPtr->curattrib),
                    gsp->flagIsGUI);
  return;
}

static GraphXYSentPtr AddDotXYSentinelToPicture (BioseqPtr bspT,
                                                 BioseqPtr bspP,
                                                 SegmenT seg,
                                                 Int4 cutoff,
                                                 Int4 xscale,
                                                 Int4 yscale,
                                                 Boolean flagLabels)
{
  GraphXYSentPtr  gsp;
  Int4            xaxiscount, yaxiscount;
  Int4            i, imatch, ix, iy, xstop, ystop, ixs, iys;
  Int4            window;
  Int4Ptr         epa, epap;
  SeqPortPtr      spp;
  Uint1Ptr        seqT, seqP, pseqT, pseqP;

  if (bspT == NULL || bspP == NULL || seg == NULL)
    return NULL;
  if ((gsp = (GraphXYSentPtr) MemNew (sizeof (GraphXYSentData))) == NULL)
    return NULL;
  gsp->flagIsGUI = VibrantIsGUI ();

  if ((epa = (Int4Ptr) MemNew (sizeof (Int4) * bspT->length * bspP->length))
      == NULL)
  {
    MemFree (gsp);
    return NULL;
  }
  gsp->endpointarray = epa;

  seqT = (Uint1Ptr) MemNew (sizeof (Uint1) * (bspT->length + 1));
  seqP = (Uint1Ptr) MemNew (sizeof (Uint1) * (bspP->length + 1));

  spp = SeqPortNew (bspT, 0, bspT->length-1, 0, Seq_code_iupacaa);
  SeqPortRead (spp, seqT, (Int2) bspT->length);
  SeqPortFree (spp);
  spp = SeqPortNew (bspP, 0, bspP->length-1, 0, Seq_code_iupacaa);
  SeqPortRead (spp, seqP, (Int2) bspP->length);
  SeqPortFree (spp);

  window = 8;
  xstop = bspT->length - window;
  ystop = bspP->length - window;
  yaxiscount = 0;
  iy = 0;
  while (iy < ystop)
  {
    epap = epa;
    for (iys = 0; iys < yscale; iys++)
    {
      epa = epap;
      xaxiscount = 0;
      ix = 0;
      while (ix < xstop)
      {
        for (ixs = 0; ixs < xscale; ixs++)
        {
          imatch = 0;
          pseqT = &(seqT[ix]);
          pseqP = &(seqP[iy]);
          for (i = 0; i < window; i++)
          {
            if (*pseqT == *pseqP)
              imatch++;
            pseqT++;
            pseqP++;
          }
          if (imatch > *epa)
            *epa = imatch;
          ix++;
          if (ix >= xstop)
            break;
        }
        epa++;
        xaxiscount++;
      }
      iy++;
      if (iy >= ystop)
        break;
    }
    yaxiscount++;
  }

  MemFree (seqT);
  MemFree (seqP);

  gsp->xaxiscount = xaxiscount;
  gsp->yaxiscount = yaxiscount;
  gsp->dotcutoff = cutoff;
  gsp->xscale = xscale;
  gsp->yscale = yscale;
  if (flagLabels)
  {
    gsp->endpointvalues = (Int4Ptr) MemNew (sizeof (Int4) * 4);
    gsp->endpointvalues[0] = 0;
    gsp->endpointvalues[1] = bspT->length;
    gsp->endpointvalues[2] = 0;
    gsp->endpointvalues[3] = bspP->length;
  }
  gsp->snt = AddSntRectangle (seg, 0, 0, 400, 400,
                              DotXYSentProc, (BigScalar) gsp,
                              CleanGSP, 0);
  return gsp;
}

/* align */

static void PlotTheXYPlot (Int4Ptr epa, Int4 linenumber,
                           Int4 xaxis, Int4 yaxis, Int4Ptr endpoints,
                           AttPData PNTR curattrib, Boolean flagIsGUI,
                           Uint1Ptr color)
{
  Int4  i;
  Int4  startT, startP, stopT, stopP;
  Int4  xoffset, yoffset, xcorrect, ycorrect;
  Uint1 curR, curG, curB;
  Int1  penW;

  xoffset = 50;
  yoffset = 50;

  curR = curattrib->color[0];
  curG = curattrib->color[1];
  curB = curattrib->color[2];
  penW = curattrib->penwidth;

  Nlm_SelectColor (color[0], color[1], color[2]);
  Nlm_WidePen (2);

  for (i = 0; i < linenumber; i++)
  {
    startT = *epa++ + xoffset;
    startP = *epa++ + yoffset;
    stopT = *epa++ + xoffset;
    stopP = *epa++ + yoffset;
    MoveTo ((Int2) startT, (Int2) startP);
    LineTo ((Int2) stopT, (Int2) stopP);
    if (!flagIsGUI)
    {
      MoveTo ((Int2) (startT-1), (Int2) (startP+1));
      LineTo ((Int2) (stopT-1), (Int2) (stopP+1));
      MoveTo ((Int2) (startT+1), (Int2) (startP-1));
      LineTo ((Int2) (stopT+1), (Int2) (stopP-1));
    }
  }

  Nlm_SelectColor (curR, curG, curB);
  Nlm_WidePen (penW);

  if (endpoints != NULL)
  {
    if (flagIsGUI)
    {
      xcorrect = 25;
      ycorrect = 50;
    }
    else
    {
      xcorrect = 50;
      ycorrect = 60;
    }
    TickLabel (endpoints[0], (Int2) (xoffset-xcorrect),
                             (Int2) (yoffset-10));
    TickLabel (endpoints[1], (Int2) (xoffset-xcorrect+xaxis),
                             (Int2) (yoffset-10));
    TickLabel (endpoints[2], (Int2) (xoffset-ycorrect),
                             (Int2) (yoffset+10));
    TickLabel (endpoints[3], (Int2) (xoffset-ycorrect),
                             (Int2) (yoffset+yaxis));
  }

  return;
}

static void XYSentProc (BigScalar calldata, PrimDrawContext pdc)
{
  GraphXYSentPtr gsp;
  DrawInfoPtr    dInfoPtr;

  if ((gsp = (GraphXYSentPtr) calldata) == NULL)
    return;
  if (gsp->endpointarray == NULL)
    return;

  dInfoPtr = (DrawInfoPtr) pdc;
  PlotTheXYPlot (gsp->endpointarray, gsp->linenumber,
                 gsp->xaxiscount, gsp->yaxiscount, gsp->endpointvalues,
                 &(dInfoPtr->curattrib), gsp->flagIsGUI, gsp->color);
  return;
}

static GraphXYSentPtr AddXYSentinelToPicture (BioseqPtr bspT, BioseqPtr bspP,
                                              SeqAlignPtr saph, SegmenT seg,
                                              Int4 xscale, Int4 yscale,
                                              Uint1Ptr color,
                                              Boolean flagLabels)
{
  GraphXYSentPtr  gsp;
  Int4            linenumber;
  Int4Ptr         epa;
  SeqAlignPtr     sap;
  DenseSegPtr     dsp;
  Int4            loop;
  Int4            startT, stopT, startP, stopP;

  if (bspT == NULL || bspP == NULL || saph == NULL || seg == NULL)
    return NULL;
  if ((gsp = (GraphXYSentPtr) MemNew (sizeof (GraphXYSentData))) == NULL)
    return NULL;
  gsp->flagIsGUI = VibrantIsGUI ();
  gsp->color = color;

  sap = saph;
  linenumber = 0;
  while (sap != NULL)
  {
    if (sap->segtype == 2)   /* DenseSeg gapped search */
    {
      dsp = sap->segs;
      for (loop = 0; loop < (Int4) dsp->numseg; loop++)
      {
        if (!(dsp->starts[loop*2] == -1 || dsp->starts[(loop*2)+1] == -1))
        {
          linenumber++;
        }
      }
    }
    sap = sap->next;
  }

  if ((epa = (Int4Ptr) MemNew (sizeof (Int4) * 4 * linenumber)) == NULL)
  {
    MemFree (gsp);
    return NULL;
  }

  gsp->linenumber = linenumber;
  gsp->endpointarray = epa;
  gsp->xscale = xscale;
  gsp->yscale = yscale;
  gsp->xaxiscount = bspT->length / xscale;
  gsp->yaxiscount = bspP->length / yscale;

  sap = saph;
  while (sap != NULL)
  {
    if (sap->segtype == 2)   /* DenseSeg gapped search */
    {
      dsp = sap->segs;
      for (loop = 0; loop < (Int4) dsp->numseg; loop++)
      {
        if (!(dsp->starts[loop*2] == -1 || dsp->starts[(loop*2)+1] == -1))
        {
          startT = dsp->starts[loop*2];
          startP = dsp->starts[(loop*2)+1];
          stopT = startT + dsp->lens[loop] - 1;
          stopP = startP + dsp->lens[loop] - 1;
          *epa++ = startT/xscale;
          *epa++ = startP/yscale;
          *epa++ = stopT/xscale;
          *epa++ = stopP/yscale;
        }
      }
    }
    sap = sap->next;
  }
  if (flagLabels)
  {
    gsp->endpointvalues = (Int4Ptr) MemNew (sizeof (Int4) * 4);
    gsp->endpointvalues[0] = 0;
    gsp->endpointvalues[1] = bspT->length;
    gsp->endpointvalues[2] = 0;
    gsp->endpointvalues[3] = bspP->length;
  }
  gsp->snt = AddSntRectangle (seg, 0, 0, 400, 400,
                              XYSentProc, (BigScalar) gsp, CleanGSP, 0);
  return gsp;
}

extern Int4 ScalePlot (Int4 xlen, Int4 ylen, Int4 maxlen)
{
  Int4 scale, max, trim;

  if (xlen > ylen)
    max = xlen;
  else
    max = ylen;

  scale = 1;
  trim = max;
  while (trim > maxlen)
  {
    scale++;
    trim = max / scale;
  }
  return scale;
}

extern SegmenT DotXYPlot (BioseqPtr bspT, BioseqPtr bspP, Int4 cutoff,
                          Int4 xscale, Int4 yscale, SegmenT seg,
                          Boolean flagLabels)
{
  if (bspT == NULL || bspP == NULL)
    return seg;

  if (seg == NULL)
    seg = CreatePicture ();

  AddDotXYSentinelToPicture (bspT, bspP, seg, cutoff, xscale, yscale,
                             flagLabels);
  return seg;
}

extern SegmenT XYPlot (BioseqPtr bspT, BioseqPtr bspP, SeqAlignPtr sap,
                       Int4 xscale, Int4 yscale, Uint1Ptr color, SegmenT seg,
                       Boolean flagLabels)
{
  if (bspT == NULL || bspP == NULL || sap == NULL)
    return seg;

  if (seg == NULL)
    seg = CreatePicture ();

  AddXYSentinelToPicture (bspT, bspP, sap, seg, xscale, yscale,
                          color, flagLabels);

  return seg;
}
