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
* File Name: urkpcc.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.5 $
*
* File Description: coiled-coil header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkpcc.h,v $
* Revision 6.5  1998/11/16 14:29:50  kuzio
* flagBoundaryCondition
*
* Revision 6.4  1998/09/16 17:46:46  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#ifndef _URKPCC__
#define _URKPCC__

#include <ncbi.h>
#include <seqport.h>

#ifdef __cplusplus
extern "C" {
#endif

#define  PI  3.141592654

typedef struct predictCCdata
{
  FloatHi  meang, stdg, meanc, stdc;
  Int4     window, linker;
  CharPtr  pccdatafile;
  CharPtr  res;
  FloatHiPtr PNTR scr;
} PccDat, PNTR PccDatPtr;

/*
 caller responsibiliity
  pccp = PccDatNew ();
  ReadPccData (pccp);
  probability_scores = PredictCC...
  MemFree (probability_scores);
  PccDatFree (pccp);
*/

extern Int4 ReadPccData (PccDatPtr pccp);

extern PccDatPtr PccDatNew (void);
extern PccDatPtr PccDatFree (PccDatPtr pccp);

extern FloatHiPtr PredictCC (CharPtr seq, Int4 start, Int4 end,
                             PccDatPtr pccp);

extern FloatHiPtr PredictCCSeqPort (SeqPortPtr spp,
                                    Int4 start, Int4 end,
                                    PccDatPtr pccp);

extern FloatHiPtr PredictCCBioseq (BioseqPtr bsp,
                                   Int4 start, Int4 end,
                                   PccDatPtr pccp);

extern FloatHiPtr PredictCCSeqLoc (SeqLocPtr slp,
                                   PccDatPtr pccp);

extern SeqLocPtr FilterCC (FloatHiPtr score, FloatHi percentcut,
                           Int4 length, Int4 linker, SeqIdPtr sip,
                           Boolean flagBoundaryCondition);

extern SeqLocPtr FilterCCVS (FloatHiPtr score, FloatHi percentcut,
                             Int4 length, Int4 linker, SeqIdPtr sip,
                             Boolean flagBoundaryCondition);

#ifdef __cplusplus
}
#endif

#endif
