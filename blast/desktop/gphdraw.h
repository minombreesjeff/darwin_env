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
* File Name: gphdraw.h
*
* Author(s): Jonathan Kans, John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.5 $
*
* File Description: sentinel graph header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: gphdraw.h,v $
* Revision 6.5  1998/10/13 17:13:55  kuzio
* colored superimposable graphs
*
* Revision 6.4  1998/09/16 19:00:36  kuzio
* cvs logs
*
* ==========================================================================
*/

#ifndef _GPHDRAW_
#define _GPHDRAW_

#include <picture.h>
#include <objres.h>
#include <objseq.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct graphsentdata {
  Boolean        flagIsGUI;
  SegmenT        seg;
  PrimitivE      snt;
  BoxInfo        box;
  Int4           min;
  Int4           max;
  Int4           axis;
  Int4           bottom;
  FloatHi        a;
  FloatHi        b;
  SeqGraphPtr    sgp;
  Uint1          red, green, blue;
} GraphSentData, PNTR GraphSentPtr;

extern GraphSentPtr AddGraphSentinelToPicture (SeqGraphPtr sgp, BioseqPtr bsp,
                                               SegmenT pict, Int4 scaleX,
                                               Int4 top, Int2 start,
                                               Uint1Ptr uRGB);

extern SegmenT DrawSeqGraphSegment (SeqGraphPtr sgp, BioseqPtr bsp,
                                    Int4 xlen, Int4 top, Int2 start,
                                    Int4 leftend, Int4 rightend,
                                    SegmenT pict);

extern void DrawGraphToPanel (SeqGraphPtr sgp, Int4 Xscale,
                              Int4 Yscale, Int4 Ylength, RecT PNTR rp,
                              Int4 leftend, Int4 rightend);

#ifdef __cplusplus
}
#endif

#endif
