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
* File Name: gxydraw.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.3 $
*
* File Description: sentinel dot plot header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: gxydraw.h,v $
* Revision 6.3  1998/09/16 19:00:38  kuzio
* cvs logs
*
* ==========================================================================
*/

#ifndef _GXYDRAW_
#define _GXYDRAW_

#include <picture.h>
#include <objseq.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct graphxysentdata {
  Boolean        flagIsGUI;
  SegmenT        seg;
  PrimitivE      snt;
  Int4           linenumber, dotcutoff, xaxiscount, yaxiscount;
  Int4           xscale, yscale;
  Int4Ptr        endpointarray, endpointvalues;
  Uint1Ptr       color;
} GraphXYSentData, PNTR GraphXYSentPtr;

extern Int4 ScalePlot (Int4 xlen, Int4 ylen, Int4 maxlen);

extern SegmenT DotXYPlot (BioseqPtr bspT, BioseqPtr bspP, Int4 cutoff,
                          Int4 xscale, Int4 yscale, SegmenT seg,
                          Boolean flagLabels);
extern SegmenT XYPlot (BioseqPtr bspT, BioseqPtr bspP, SeqAlignPtr sap,
                       Int4 xscale, Int4 yscale, Uint1Ptr color, SegmenT seg,
                       Boolean flagLabels);

#ifdef __cplusplus
}
#endif

#endif
