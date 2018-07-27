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
* File Name: urkepi.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.8 $
*
* File Description: epi header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkepi.h,v $
* Revision 6.8  1998/11/16 14:29:51  kuzio
* flagBoundaryCondition
*
* Revision 6.7  1998/09/16 17:46:45  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#ifndef _URKEPI__
#define _URKEPI__

#include <ncbi.h>
#include <seqport.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LowComplexityData
{
  Int4       method, linker;
  Int4       mwindow, window;
  FloatHi    mpercentcut, percentcut;
  Int4       nucleotuple;
  Uint1Ptr   sequence;
  Int4Ptr    epiarray;
  FloatHiPtr score;
} EpiDat, PNTR EpiDatPtr;

/*
 caller responsibiliity
  epip = EpiDatNew ();
  probability_scores = PredictEpi...
  MemFree (probability_scores);
  EpiDatFree (epip);
*/

extern EpiDatPtr EpiDatNew (void);
extern EpiDatPtr EpiDatFree (EpiDatPtr epip);

extern FloatHiPtr PredictEpi (CharPtr seq, Int4 start, Int4 end,
                              EpiDatPtr epip, Boolean flagIsAA);

extern FloatHiPtr PredictEpiSeqPort (SeqPortPtr spp,
                                     Int4 start, Int4 end,
                                     EpiDatPtr epip, Boolean flagIsAA);

extern FloatHiPtr PredictEpiBioseq (BioseqPtr bsp,
                                    Int4 start, Int4 end,
                                    EpiDatPtr epip);

extern FloatHiPtr PredictEpiSeqLoc (SeqLocPtr slp,
                                    EpiDatPtr epip);

extern SeqLocPtr FilterEpi (EpiDatPtr pepi, CharPtr aa, Int4 length,
                            SeqIdPtr sip, Boolean flagHighPass,
                            Boolean flagIsAA,
                            Boolean flagBoundaryCondition);
extern SeqLocPtr FilterEpiBioseq (EpiDatPtr pepi, BioseqPtr bsp,
                                  Boolean flagHighPass,
                                  Boolean flagBoundaryCondition);
#ifdef __cplusplus
}
#endif

#endif
