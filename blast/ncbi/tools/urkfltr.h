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
* File Name: urkfltr.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.4 $
*
* File Description: filter header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkfltr.h,v $
* Revision 6.4  1998/09/16 17:46:45  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#ifndef _URKFLTR__
#define _URKFLTR__

#include <ncbi.h>
#include <seqport.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAXSEQCHUNK           262144

#define NA_FILTER             0
#define NA_FILTER_GC          1
#define NA_FILTER_AT          2
#define NA_FILTER_PUR         3
#define NA_FILTER_PYR         4

#define AA_FILTER_COMP        100
#define AA_FILTER_COMP_HOPP   101
#define AA_FILTER_COMP_KYTE   102

typedef struct filterdata
{
  Int4       window, filtertype, filtersize;
  CharPtr    filterdatafile;
  CharPtr    res;
  FloatHiPtr scr;
  FloatHi    maxscr;
} FilterDat, PNTR FilterDatPtr;

extern Int4 ReadFilterData (FilterDatPtr fltp);

extern FilterDatPtr FilterDatNew (Int4 filtertype, Int4 window);
extern FilterDatPtr FilterDatFree (FilterDatPtr fltp);

extern FloatHiPtr urkFilterSeq (CharPtr seq, Int4 start, Int4 end,
                                FilterDatPtr fltp);

extern FloatHiPtr FilterSeqPort (SeqPortPtr spp,
                                 Int4 start, Int4 end,
                                 FilterDatPtr fltp);

extern FloatHiPtr FilterBioseq (BioseqPtr bsp,
                                Int4 start, Int4 end,
                                FilterDatPtr fltp);

extern FloatHiPtr FilterSeqLoc (SeqLocPtr slp,
                                FilterDatPtr fltp);

extern SeqLocPtr FilterFilter (FloatHiPtr score, FloatHi maxscore,
                               Int4 window, FloatHi percentcut,
                               Int4 length, SeqIdPtr sip,
                               Boolean flagMeld, Boolean flagHighPass);

#ifdef __cplusplus
}
#endif

#endif
