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
* File Name: urkbias.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.5 $
*
* File Description: codon bias header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkbias.h,v $
* Revision 6.5  1998/09/16 17:46:43  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#ifndef _URKBIAS__
#define _URKBIAS__

#include <ncbi.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Gather_Cds
{
  Int4Ptr     tableGlobal, tableRefine;
  SeqLocPtr   slpGlobal, slpRefine, slpAll, slpHit, slpFound;
  Int4        stdcount, globalcount, refinecount, findcount;
  FloatHi     stdev, HIscore, LOscore, mean;
  FloatHi     scorecut;
} Gather_CDS, PNTR Gather_CDSPtr;

extern Gather_CDSPtr GatherCDSNew (void);
extern Gather_CDSPtr GatherCDSFree (Gather_CDSPtr gcdsp);

extern SeqLocPtr SeqLocDup (SeqLocPtr slpold);
extern SeqLocPtr SeqLocDupAll (SeqLocPtr slpold);
extern SeqLocPtr SeqLocLink (SeqLocPtr PNTR slph, SeqLocPtr slp);
extern Boolean SeqLocMatch (SeqLocPtr slplist, SeqLocPtr slpnew);

extern void UniqueOrfs (SeqLocPtr PNTR pslpFound);
extern void SortByLocOrfs (SeqLocPtr PNTR pslpFound);
extern void RemoveInternalOrfs (SeqLocPtr PNTR slpFound);

extern SeqLocPtr FindSimilarBiasOrfs (SeqEntryPtr sep, FloatHi probcut,
                                      Int4 clustmin, Int4 findmin,
                                      SeqLocPtr slpKnown,
                                      SeqLocPtr slpPotential);

extern FloatHiPtr BiasScoreBioseq (BioseqPtr bsp, Int4Ptr globaltable,
                                   Int4 tripletwindow, Int4 xframe,
                                   Uint1 xstrand);

#ifdef __cplusplus
}
#endif

#endif
