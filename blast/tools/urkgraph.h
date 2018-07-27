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
* File Name: urkgraph.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.5 $
*
* File Description: graph header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkgraph.h,v $
* Revision 6.5  1998/09/16 17:46:45  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#ifndef _URKGRAPH_
#define _URKGRAPH_

#include <objres.h>
#include <picture.h>
#include <viewer.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GRAPH              0  /* simple line plots */
#define GRAPH_LINE         0
#define GRAPH_HISTO        1  /* histograms - bar graphs */
#define GRAPH_POINT        2  /* xy points - scatter plots */
#define GRAPH_PointToPoint 3  /* connected xy points - "dot" plots */
#define GRAPH_PTP          3

#define GRAPH_Y_FloatHi    1
#define GRAPH_Y_Int4       2
#define GRAPH_Y_BS         3

typedef struct graphinfo
{
  Int4        graphtype;
  Uint1       yvaluetype;
  Int4        window, startplot, stopplot;
  Int4        imaxval, iminval, iaxisval;
  FloatHi     fmaxval, fminval, faxisval;
  Int4        graphpixelwidth, graphpixelheight;
  Int4        horiz_offset, vert_offset;
  Int4        localmaxzoom;
  Boolean     flagPlotZero;
  Boolean     flagAnchorPt;
  Boolean     flagInvert;
  Boolean     flagLogScaleY;
  Boolean     flagCleanupSGP;
  SeqGraphPtr sgp;
  struct graphinfo PNTR next;
  struct graphinfo PNTR prev;
} GraphInfo, PNTR GraphInfoPtr;

extern SeqGraphPtr CopyGraph (SeqGraphPtr sgpold);

extern GraphInfoPtr GraphInfoNew (GraphInfoPtr gip,
                                  Uint1 yvaluetype,
                                  Int4 number_x_points,
                                  Int4 graphpixelwidth,
                                  Int4 graphpixelheight);

extern GraphInfoPtr GraphInfoNewEP (Uint1 yvaluetype,
                                    Int4 startplot, Int4 stopplot,
                                    Int4 graphpixelwidth,
                                    Int4 graphpixelheight);

extern GraphInfoPtr GraphInfoFree (GraphInfoPtr gip);
extern GraphInfoPtr GraphInfoFreeAll (GraphInfoPtr gip);

extern void MinMaxGraph (Uint1 yvaluetype, Pointer score,
                         GraphInfoPtr gip);

extern void MinMaxGraphX (Uint1 yvaluetype, Pointer score,
                          Pointer maxval, Pointer minval, Pointer axisval,
                          Int4 startplot, Int4 stopplot);

extern SeqGraphPtr DevelopGraph (Uint1 yvaluetype, Pointer score,
                                 GraphInfoPtr gip);

extern SeqGraphPtr DevelopGraphX (Uint1 yvaluetype, Pointer score,
                                  Int4 start, Int4 stop,
                                  Pointer maxval, Pointer minval,
                                  Pointer axisval,
                                  Int4 graphpixelwidth);

extern void AnchorPoint (SegmenT s, GraphInfoPtr gip);

extern SegmenT DrawGraph (SegmenT seg, SeqGraphPtr sgp, GraphInfoPtr gip);

extern SegmenT DrawGraphSGPAsIs (SegmenT seg, SeqGraphPtr sgp,
                                 Int4 graphtype,
                                 Int4 horiz_offset, Int4 vert_offset);

extern SegmenT DrawGraphSGP (SegmenT seg, SeqGraphPtr sgp, Int4 graphtype,
                             Int4 graphpixelwidth, Int4 graphpixelheight,
                             Int4 horiz_offset, Int4 vert_offset);

extern SegmenT DrawGraphX (SegmenT seg, SeqGraphPtr sgp,
                           Int4 graphtype, Int4 startplot,
                           Int4 window, Int4 graphpixelheight,
                           Int4 horiz_offset, Int4 vert_offset,
                           Boolean flagPlotZero, Boolean flagInvert);

extern SegmenT Xscale (SegmenT seg, SeqGraphPtr sgp, GraphInfoPtr gip);
extern SegmenT Yscale (SegmenT seg, SeqGraphPtr sgp, GraphInfoPtr gip);

extern SegmenT XscaleX (SegmenT seg, Int4 compression,
                        Int4 scale, Int4 trans,
                        Int4 startplot, Int4 stopplot,
                        Int4 horiz_offset, Int4 vert_offset);

extern SegmenT YscaleX (SegmenT seg, Int4 scale, Int4 trans,
                        FloatHi minval, FloatHi maxval,
                        Int4 compression, Int4 graphpixelheight,
                        Int4 startplot, Int4 stopplot,
                        Int4 horiz_offset, Int4 vert_offset);
/*
                        Boolean flagLogScaleY, Boolean flagInvert);
*/
#ifdef __cplusplus
}
#endif

#endif
