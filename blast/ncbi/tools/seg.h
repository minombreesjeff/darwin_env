/*  seg.h
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
 * File Name:  seg.h
 *
 * Author:  Scott Federhen
 *
 * Version Creation Date: 4/24/98
 *
 * $Revision: 6.8 $
 *
 * File Description:  low-complexity filter 'seg'
 *
 * Modifications:
 * --------------------------------------------------------------------------
 * Date     Name        Description of modification
 * -------  ----------  -----------------------------------------------------
 *
 *
 * ==========================================================================
 */

#ifndef __SEGSTR__
#define __SEGSTR__

#include <stdio.h>
#include <fcntl.h>
#include <ctype.h>
#include <math.h>

#include <ncbierr.h>
#include <ncbibs.h>
#include <sequtil.h>
#include <seqport.h>

#ifdef __cplusplus
extern "C" {
#endif


/*-----------------------------------------------------(alphabet defines)---*/

#define NA4     1
#define AA20    2

#define LN20    2.9957322735539909
#define LN4     1.3862943611198906

#define CHAR_SET 128

/*--------------------------------------------------------------(structs)---*/

typedef struct segm
  {
   int begin;
   int end;
   struct segm *next;
  } Seg, PNTR SegPtr;

typedef struct alpha
  {
   Int4 alphabet;
   Int4 alphasize;
   FloatHi lnalphasize;
   Int4Ptr alphaindex;
   BytePtr alphaflag;
   CharPtr alphachar;
  } Alpha, PNTR AlphaPtr;

typedef struct segparams
  {
   Int4 window;
   FloatHi locut;
   FloatHi hicut;
   Int4 period;
   Int4 hilenmin;
   Boolean overlaps;	/* merge overlapping pieces if TRUE. */
   Int4 maxtrim;
   Int4 maxbogus;
   AlphaPtr palpha;
  } SegParams, PNTR SegParamsPtr;

typedef struct sequence
  {
   struct sequence PNTR parent;
   CharPtr seq;
   AlphaPtr palpha;
   Int4 start;
   Int4 length;
   Int4 bogus;
   Boolean punctuation;
   Int4 PNTR composition;
   Int4 PNTR state;
   FloatHi entropy;
  } Sequence, PNTR SequencePtr;

/*---------------------------------------------------------------(protos)---*/

SeqLocPtr BioseqSeg (BioseqPtr bsp, SegParamsPtr sparamsp);
SeqLocPtr BioseqSegNa (BioseqPtr bsp, SegParamsPtr sparamsp);
SeqLocPtr BioseqSegAa (BioseqPtr bsp, SegParamsPtr sparamsp);
SeqLocPtr SeqlocSegNa (SeqLocPtr slp, SegParamsPtr sparamsp);
SeqLocPtr SeqlocSegAa (SeqLocPtr slp, SegParamsPtr sparamsp);
SegParamsPtr SegParamsNewAa(void);
SegParamsPtr SegParamsNewNa(void);
void SegParamsCheck (SegParamsPtr sparamsp);

void SegSeq(SequencePtr seq, SegParamsPtr sparamsp, SegPtr *segs,
            Int4 offset);
SequencePtr SeqNew(void);
void SeqFree(SequencePtr seq);
void SegFree(SegPtr segs);

void SegParamsFree(SegParamsPtr sparamsp);

extern void lower(CharPtr string, size_t len);
extern void upper(CharPtr string, size_t len);

/*--------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif
#endif
