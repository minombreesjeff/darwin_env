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
* File Name: urkcnsrt.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.8 $
*
* File Description: consort header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkcnsrt.h,v $
* Revision 6.8  1998/10/13 17:16:37  kuzio
* additional Markov setup
*
* Revision 6.7  1998/10/05 13:35:16  kuzio
* start markov chains and codon bias
*
* Revision 6.6  1998/09/28 16:36:11  kuzio
* no met orf check
*
* Revision 6.5  1998/09/16 17:46:44  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#ifndef _CONSORT__
#define _CONSORT__

#include <ncbi.h>
#include <accentr.h>
#include <urktree.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CONMATSIZE    512

typedef struct FrequencyStructure
{
  Int4Ptr    frame0trip,    frame1trip,    frame2trip;
  Int4Ptr    frame0cdnobs,  frame1cdnobs,  frame2cdnobs;
  Int4Ptr    frame0cdn,     frame1cdn,     frame2cdn;
  Int4Ptr    frame0leftdi,  frame1leftdi,  frame2leftdi;
  Int4Ptr    frame0rightdi, frame1rightdi, frame2rightdi;
  FloatHiPtr frame0exp,     frame1exp,     frame2exp;
  FloatHiPtr frame0std,     frame1std,     frame2std;
  Int4Ptr    globalcdnobs;
} Freq, PNTR FreqPtr;

/* codon usage tree from genome */

extern TreeNodePtr ConsortSeqEntry (SeqEntryPtr sep);
extern Int4Ptr     ConformSeqEntry (SeqEntryPtr sep);

/* exploration:  ORFs related by codon usage */

extern Int4Ptr NewCodonTable (void);
extern Int4Ptr FreeCodonTable (Int4Ptr cutp);
extern Int4Ptr CodonTableFromSeqLoc (BioseqPtr bsp, SeqLocPtr slp);
extern void AddSeqLocToCodonTable (Int4Ptr cutp, BioseqPtr bsp,
                                   SeqLocPtr slp, Boolean flagAdd);
extern Int4Ptr MergeCodonTables (Int4Ptr cutp1, Int4Ptr cutp2);

extern FloatHi Confide (Int4Ptr cutgene, Int4Ptr cutgbl);
extern void Conform (Int4Ptr freq, FILE *fn);

extern ValNodePtr ClearNonMetOrfs (ValNodePtr orflist);

extern FreqPtr FreqNew (void);
extern FreqPtr FreqFree (FreqPtr frqp);

extern FreqPtr ConKovCDSNtFreqs (BioseqPtr bsp, SeqLocPtr slp);
extern Boolean ConKovCDSGlobalNtFreqs (BioseqPtr bsp, FreqPtr frqp);
extern FreqPtr ConKovTrainCDS (BioseqPtr bsp, SeqLocPtr slp);

#ifdef __cplusplus
}
#endif

#endif
