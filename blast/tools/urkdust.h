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
* File Name: urkdust.h
*
* Author(s): John Kuzio
*
* Version Creation Date: 98-01-01
*
* $Revision: 6.8 $
*
* File Description: urkdust header
*
* Modifications:
* --------------------------------------------------------------------------
* Date       Name        Description of modification
* --------------------------------------------------------------------------
* $Log: urkdust.h,v $
* Revision 6.8  1998/09/16 17:46:44  kuzio
* cvs logging
*
*
* ==========================================================================
*/

#ifndef _URKDUST__
#define _URKDUST__

#include <ncbi.h>
#include <seqport.h>
#include <sequtil.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct dustdata
{
  FloatHi percent;
  Int4    level, windowsize, minwin, linker, method;
  Boolean flagGraph, flagBrute;
} DustData, PNTR DustDataPtr;

typedef struct dustregion
{
  FloatHi percent;
  Int4    from, to;
  struct  dustregion PNTR next;
} DustRegion, PNTR DustRegionPtr;

typedef struct dustcurrentvalues
{
  Int4    curlevel, curstart, curend;
} DustCurVal, PNTR DustCurValPtr;

#define MAXSEQCHUNK  262144

/*
 caller responsibility
  ddp = DustDataNew ();
  drp = Dust... (..., ddp);
  slp = DustFilter (drp, ddp->percent, ...);
  DustRegionFree (drp);
  DustDataFree (ddp);
*/

/*
 DustDataNew set ddp to original default values
 for original dust percentcut is forced to level
*/

extern DustDataPtr DustDataNew (void);
extern DustDataPtr DustDataFree (DustDataPtr ddp);
extern DustRegionPtr DustRegionNew (DustRegionPtr drp);
extern DustRegionPtr DustRegionFree (DustRegionPtr drp);

extern DustRegionPtr DustSeq (CharPtr seq, Int4 start, Int4 stop,
                              DustDataPtr ddp);
extern DustRegionPtr DustSeqPort (SeqPortPtr spp,
                                  Int4 start, Int4 stop,
                                  DustDataPtr ddp);
extern DustRegionPtr DustBioseq (BioseqPtr bsp,
                                 Int4 start, Int4 stop,
                                 DustDataPtr ddp);
extern DustRegionPtr DustSeqLoc (SeqLocPtr slp,
                                 DustDataPtr ddp);
extern SeqLocPtr DustFilter (DustRegionPtr drp, FloatHi percentcut,
                             SeqIdPtr sip);
extern FloatHiPtr DustForGraph (DustRegionPtr drp, Int4 length,
                                Int4 start, Int4 stop);

/* wrapper for original function */
extern SeqLocPtr BioseqDustWrap (BioseqPtr bsp, Int4 start, Int4 end,
                                 Int2 level, Int2 window, Int2 minwin,
                                 Int2 linker);

#ifdef __cplusplus
}
#endif

#endif
