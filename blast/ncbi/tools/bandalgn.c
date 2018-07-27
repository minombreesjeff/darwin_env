static char const rcsid[] = "$Id: bandalgn.c,v 6.17 2003/05/30 17:25:36 coulouri Exp $";

/* ===========================================================================
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
* ===========================================================================*/
/*****************************************************************************

File name: bandalgn.c

Author: Gennadiy Savchuk, Jinqhui Zhang, Tom Madden

Contents: Functions to perform both local and global banded alignments.

****************************************************************************/
/* $Revision: 6.17 $
 *
 * $Log: bandalgn.c,v $
 * Revision 6.17  2003/05/30 17:25:36  coulouri
 * add rcsid
 *
 * Revision 6.16  2001/05/25 19:32:48  vakatov
 * Nested comment typo fixed
 *
 * Revision 6.15  2000/02/10 22:47:06  vakatov
 * DLL'zation for MSVC on PC, Win-NT
 *
 * Revision 6.14  1999/09/01 21:46:49  chappey
 * SeqPortFree in load_data
 *
 * Revision 6.13  1999/07/13 15:39:48  sicotte
 * Fixed casting warnings
 *
 * Revision 6.12  1999/04/06 15:21:00  sicotte
 * Fixed prototype problems and lint casting and.
 *
 * Revision 6.11  1998/08/24 20:20:32  kans
 * fixed -v -fd warnings
 *
 * Revision 6.10  1998/05/04 19:58:21  tatiana
 * change SEV_ERROR to SEV_WARNING
 *
 * Revision 6.9  1998/01/16 21:29:43  chappey
 * Remove function CC_GetExtremes and use now SeqAlignStart, SeqAlignStop in salsap.c
 *
 * Revision 6.8  1998/01/02 21:58:55  chappey
 * printf removed
 *
 * Revision 6.7  1997/12/19 20:47:16  chappey
 * bugs fixed
 *
 * Revision 6.6  1997/10/21 23:09:39  chappey
 * fixes on ChangeGlobalBandMatrix by Colombe and Hugues
 *
 * Revision 6.4  1997/10/14 16:21:40  tatiana
 * merge utility functions
 *
 * Revision 6.3  1997/10/02 15:54:54  kans
 * added two includes and a return value
 *
 * Revision 6.2  1997/10/02 15:17:55  tatiana
 * global align utility added
 *
 * Revision 6.1  1997/09/16 19:29:14  kans
 * minor fix (CC)
 *
 * Revision 6.0  1997/08/25 18:52:12  madden
 * Revision changed to 6.0
 *
 * Revision 1.3  1997/06/23 16:15:52  tatiana
 * GlobalBandAlign struct changed to use SeqLocs instead of SeqIds
 *
 * Revision 1.2  1997/03/05  17:31:21  savchuk
 * Sliced for 5 pieces (bandalg[n,0-5].c)
 * to fit the Windoze MSVC 16-bit compiler limitations.
 *
 * Revision 1.1  1997/01/22  14:11:05  madden
 * Initial revision
 *
*/



/* A PACKAGE FOR GLOBALLY ALIGNING TWO SEQUENCES WITHIN A BAND:

   To invoke, call ALIGN(A,B,M,N,L,U,W,G,H,S).
   The parameters are explained as follows:
        A, B : two sequences to be aligned
        M : the length of sequence A
        N : the length of sequence B
        L : lower bound of the band
        U : upper bound of the band
        W : scoring table for matches and mismatches
        G : gap-opening penalty
        H : gap-extension penalty
        S : script for DISPLAY routine
*/

#include <seqport.h>
#include <blast.h>
#include <salutil.h>
#include <bandalgn.h>


NLM_EXTERN PSUGapOptionsPtr
PSUGapOptionsDelete(PSUGapOptionsPtr options)

{
  options = MemFree(options);
  
  return NULL;
}

/*
	Create the PSUGapOptionsPtr and fill in with default values.
*/

NLM_EXTERN PSUGapOptionsPtr
PSUGapOptionsCreate(Uint1 search_type)

{
	PSUGapOptionsPtr options;

	options = (PSUGapOptionsPtr) MemNew(sizeof(PSUGapOptions));
	if (options == NULL)
		return NULL;

	/* defaults from WEbb lab. */
	/* Should be defines above?? */
	options->up = 282;
	options->low = -2329;
	options->gopen = 5;
	options->gext = 1;
	options->gext = 1;
	options->start_diag = options->low;
	options->width = options->up - options->low + 1;

	return options;
}

/* 
	Deletes the GlobalBandStruct, including the options.
	Does not delete the sequence or the ID's.
*/


NLM_EXTERN GlobalBandStructPtr
GlobalBandStructDelete(GlobalBandStructPtr gbsp)

{
	if (gbsp == NULL)
		return NULL;

	gbsp->options = PSUGapOptionsDelete(gbsp->options);

	gbsp->edit_block = GapXEditBlockDelete(gbsp->edit_block);

	gbsp = MemFree(gbsp);

	return NULL;
}

/*
	Create the GlobalBandStructPtr and defaults for a search_type.
*/

NLM_EXTERN GlobalBandStructPtr
GlobalBandStructCreate(Uint1 search_type)

{
	GlobalBandStructPtr gbsp;

	gbsp = (GlobalBandStructPtr) MemNew(sizeof(GlobalBandStruct));
	if (gbsp == NULL)
		return NULL;

	gbsp->search_type = search_type;
	gbsp->options = PSUGapOptionsCreate(search_type);
	if (gbsp->options == NULL)
	{
		gbsp = GlobalBandStructDelete(gbsp);
		return NULL;
	}

	return gbsp;
}

/*
        Performs a global alignment, producing a SeqAlign.
*/
NLM_EXTERN SeqAlignPtr
GlobalBandToSeqAlign (GlobalBandStructPtr gbsp)
{

	SeqAlignPtr seqalign;
	GapXEditBlockPtr edit_block;
	SeqLocPtr l1, l2;

	if (gbsp == NULL)
		return NULL;

	if (GlobalBandToEditBlock(gbsp) == FALSE)
	{
		return NULL;
	}

	if (gbsp->edit_block == NULL)
		return NULL;
	edit_block = gbsp->edit_block;
	l1 = gbsp->seqloc1;
	l2 = gbsp->seqloc2;
        edit_block->length1 = SeqLocLen(l1);
        edit_block->length2 = SeqLocLen(l2);
	if (SeqLocStrand(l1) == Seq_strand_minus) {
		edit_block->frame1 = -1;
	} else {
		edit_block->frame1 = 1;		
	}
	if (SeqLocStrand(l2) == Seq_strand_minus) {
		edit_block->frame2 = -1;
	} else {
		edit_block->frame2 = 1;		
	}
	seqalign = GapXEditBlockToSeqAlign(gbsp->edit_block, SeqLocId(l2),  SeqLocId(l1));

	gbsp->edit_block = GapXEditBlockDelete(gbsp->edit_block);

	return seqalign;
}

/*
        Performs a global alignment, producing an EditBlock, which
        can be made into a SeqAlign.
*/

NLM_EXTERN Boolean
GlobalBandToEditBlock(GlobalBandStructPtr gbsp)
{
  GapXEditBlockPtr edit_block;
  FloatHi score;
  Int4 Slen;
  Int4Ptr S;		/* conversion operations */

  if (gbsp == NULL)
	return FALSE;

  S = MemGet((gbsp->seq1_length+gbsp->seq2_length)*sizeof(Int4), MGET_ERRPOST);

  switch(gbsp->search_type)
  {

  case G_BAND_LINEAR:
    score = (FloatHi) gband_linear(gbsp->seq1-1, gbsp->seq2-1,
			     gbsp->seq1_length, gbsp->seq2_length,
			     gbsp->matrix, gbsp->options, S, &Slen)/BND_DIGIT; 
    break;
  case G_BAND_QUADRATIC:
    score = (FloatHi) gband_quadratic(gbsp->seq1-1, gbsp->seq2-1,
                             gbsp->seq1_length, gbsp->seq2_length,
                             gbsp->matrix, gbsp->options, S, &Slen)/BND_DIGIT;
    break;
  case G_BAND_LGAP:
    score = (FloatHi) gband_linear_gap(gbsp->seq1-1, gbsp->seq2-1,
                             gbsp->seq1_length, gbsp->seq2_length,
                             gbsp->matrix, gbsp->options, S, &Slen)/BND_DIGIT;
    break;
  case G_BAND_QGAP:
    score = (FloatHi) gband_linear_qgap(gbsp->seq1-1, gbsp->seq2-1,
                             gbsp->seq1_length, gbsp->seq2_length,
                             gbsp->matrix, gbsp->options, S, &Slen)/BND_DIGIT;
    break;
  case G_BAND_L3GAP:
    score = (FloatHi) gband_l3gap(gbsp->seq1-1, gbsp->seq2-1,
                             gbsp->seq1_length, gbsp->seq2_length,
                             gbsp->matrix, gbsp->options, S, &Slen)/BND_DIGIT;
    break;
  case G_BAND_Q3GAP:
    score = (FloatHi) gband_q3gap(gbsp->seq1-1, gbsp->seq2-1,
                             gbsp->seq1_length, gbsp->seq2_length,
                             gbsp->matrix, gbsp->options, S, &Slen)/BND_DIGIT;
    break;
  default:
    score = 0;
    ErrPostEx(SEV_ERROR, 0, 0, "Unknown method.");
    return FALSE;
  }
  if (score == 0) {
	return FALSE;
  }
  edit_block = TracebackToGapXEditBlock(gbsp->seq1-1, gbsp->seq2-1, gbsp->seq1_length, gbsp->seq2_length, S, 0, 0);

  S = MemFree(S);

  gbsp->edit_block = edit_block;
  gbsp->score = (Int4) score;
  gbsp->alignment_length = Slen;

  return TRUE;
}

/* A PACKAGE FOR LOCALLY ALIGNING TWO SEQUENCES WITHIN A BAND:

   To invoke, call BAND_LOCAL_ALIGN(A,B,M,N,L,U,W,G,H,S,&SI,&SJ,&EI,&EJ).
   The parameters are explained as follows:
	A, B : two sequences to be aligned
	M : the length of sequence A
	N : the length of sequence B
	L : lower bound of the band
	U : upper bound of the band
	W : scoring table for matches and mismatches
	G : gap-opening penalty
	H : gap-extension penalty
	S : script for DISPLAY routine
	*SI : starting position of sequence A in the optimal local alignment
	*SJ : starting position of sequence B in the optimal local alignment
	*EI : ending position of sequence A in the optimal local alignment
	*EJ : ending position of sequence B in the optimal local alignment
*/

/* 
	Deletes the LocalBandStruct, including the options.
	Does not delete the sequence or the ID's.
*/

NLM_EXTERN LocalBandStructPtr
LocalBandStructDelete(LocalBandStructPtr lbsp)

{
	if (lbsp == NULL)
		return NULL;

	lbsp->options = PSUGapOptionsDelete(lbsp->options);

	lbsp->edit_block = GapXEditBlockDelete(lbsp->edit_block);

	lbsp = MemFree(lbsp);

	return NULL;
}

/*
	Create the LocalBandStructPtr and defaults for a search_type.
*/

NLM_EXTERN LocalBandStructPtr
LocalBandStructCreate(Uint1 search_type)

{
	LocalBandStructPtr lbsp;

	lbsp = (LocalBandStructPtr) MemNew(sizeof(LocalBandStruct));
	if (lbsp == NULL)
		return NULL;

	lbsp->search_type = search_type;
	lbsp->options = PSUGapOptionsCreate(search_type);
	if (lbsp->options == NULL)
	{
		lbsp = LocalBandStructDelete(lbsp);
		return NULL;
	}

	return lbsp;
}

/*
        Performs local alignment, producing a SeqAlign.
*/
NLM_EXTERN SeqAlignPtr
LocalBandToSeqAlign (LocalBandStructPtr lbsp)
{

	SeqAlignPtr seqalign;

	GapXEditBlockPtr edit_block;
	SeqLocPtr l1, l2;

	if (lbsp == NULL)
		return NULL;

	if (LocalBandToEditBlock(lbsp) == FALSE)
	{
		return NULL;
	}

	if (lbsp->edit_block == NULL)
		return NULL;
	edit_block = lbsp->edit_block;
	l1 = lbsp->seqloc1;
	l2 = lbsp->seqloc2;
	if (SeqLocStrand(l1) == Seq_strand_minus) {
		edit_block->frame1 = -1;
	} else {
		edit_block->frame1 = 1;		
	}
	if (SeqLocStrand(l2) == Seq_strand_minus) {
		edit_block->frame2 = -1;
	} else {
		edit_block->frame2 = 1;		
	}
	seqalign = GapXEditBlockToSeqAlign(lbsp->edit_block, SeqLocId(l2),  SeqLocId(l1));

	lbsp->edit_block = GapXEditBlockDelete(lbsp->edit_block);

	return seqalign;
}

/*
        Performs a global alignment, producing an EditBlock, which
        can be made into a SeqAlign.
*/

NLM_EXTERN Boolean
LocalBandToEditBlock(LocalBandStructPtr lbsp)
{
  GapXEditBlockPtr edit_block;
  FloatHi score;
  Int4Ptr S;		/* conversion operations */
  Uint1Ptr seq1, seq2;

  if (lbsp == NULL)
	return FALSE;

  S = (Int4Ptr)MemGet((lbsp->seq1_length + lbsp->seq2_length) * sizeof(Int4), MGET_ERRPOST);
	score = BAND_LOCAL_ALIGN(lbsp->seq1-1, lbsp->seq2-1, lbsp->seq1_length, lbsp->seq2_length, lbsp->matrix, lbsp->options, S, &(lbsp->seq1_start), &(lbsp->seq2_start), &(lbsp->seq1_end), &(lbsp->seq2_end), lbsp->search_type);

/* Compensate for one-offset. */
	lbsp->seq1_start--;
	lbsp->seq2_start--;
	lbsp->seq1_end--;
	lbsp->seq2_end--;

  seq1 = lbsp->seq1;
  seq2 = lbsp->seq2;

  seq1 += lbsp->seq1_start;
  seq2 += lbsp->seq2_start;

  if (score > 0)
  	edit_block = TracebackToGapXEditBlock(seq1-1, seq2-1, (lbsp->seq1_length-lbsp->seq1_start), (lbsp->seq2_length-lbsp->seq2_start), S, lbsp->seq1_start, lbsp->seq2_start);

  S = MemFree(S);

  lbsp->edit_block = edit_block;
  lbsp->score = (Int4)score;

  return TRUE;
}

Int4
BAND_LOCAL_ALIGN(Uint1Ptr A, Uint1Ptr B,
		 Int4 M, Int4 N,
		 Int4Ptr PNTR matrix, PSUGapOptionsPtr options,
		 Int4Ptr S,
		 Int4Ptr psi, Int4Ptr psj,
		 Int4Ptr pei, Int4Ptr pej,
		 Int4 align_type)
{ 
  Int4 band;
  Boolean flag;
  register Int4 i, j, si, ei, ib;
  register Int4 c, d, e, t, m;
  Int4 leftd, rightd, G, H, low, up;
  Int4 best_score, score1;
  Int4 starti = 1, startj = 1, endi = M - 1, endj = N - 1;
  register Int4Ptr wa;
  register Int4 curd;
  register dp_ptr dp;
  Int4 Slen;
  data_t data;
  Int4Ptr PNTR W;

  G = options->gopen;
  H = options->gext;
  m = G + H;
  low = MAX(-M, options->low);
  up = MIN(N, options->up);
  W = matrix;

  if (N <= 0) { 
    *psi = *psj = *pei = *pej;
    return -1;
  }
  if (M <= 0) {
    *psi = *psj = *pei = *pej;
    return -1;
  }
  band = up - low + 1;
  if (band < 1) {
    ErrPostEx(SEV_WARNING, 0, 0, "low > up is unacceptable");
    return -1;
  }
  j = (band+2) * sizeof(dp_node);
  data.CD = (dp_ptr) MemGet(j, MGET_ERRPOST);

  if (low > 0) leftd = 1;
  else if (up < 0) leftd = band;
  else leftd = 1-low;
  rightd = band;
  si = MAX(0, -up);
  ei = MIN(M, N - low);
  data.CD[leftd].CC = 0;
  for (j = leftd+1; j <= rightd; j++) {
    data.CD[j].CC = 0;
    data.CD[j].DD = -m;
  }
  data.CD[rightd+1].CC = MININT;
  data.CD[rightd].DD = MININT;
  best_score = 0;
  endi = si;
  endj = si + low;
  data.CD[leftd-1].CC = MININT;
  data.CD[leftd].DD = -m; c = 0;
  for (i = si+1; i <= ei; i++) {
    score1 = best_score;
    if (i > N-up) rightd--;
    if (leftd > 1) leftd--;
    wa = W[A[i]];
    d = data.CD[leftd].DD;
    if ((ib = leftd + low - 1 + i) > 0) c = data.CD[leftd].CC+wa[B[ib]];
    if (c < 0) c = 0;
    else if (c > best_score) {
      best_score = c;
      endi = i;
      endj = ib;
    }
    e = c-m;
    data.CD[leftd].CC = c;
    for (curd=leftd+1, dp = &data.CD[curd], curd += i+low-1; 
	 curd <= rightd+i+low-1; curd++) {
      c = dp->CC + wa[B[curd]];
      if ((d=dp->DD) > c) c = d;		
      if (e > c) {
	if (e > 0) {
	  dp->CC = e; e-=H;
	  *(((Int4Ptr) (dp++))-1) = d-H;
	} else {
	  dp->CC = *(((Int4Ptr) (dp))-1) = 0;
          dp++;
	}
      } else {
	if (c <= 0) {
	  dp->CC = *(((Int4Ptr) (dp))-1) = 0;
          dp++;
	} else {
	  if (c > best_score) {
	    best_score = c;
	    endj = curd;
	  }
	  dp->CC = c;
	  if ((c -= m) > (e -= H)) e= c; 
	  if (c > (d-=H))  
	    *((Int4Ptr) (dp++)-1) = c;
	  else *((Int4Ptr) (dp++)-1) = d;
	}
      }
    }
    if (score1 < best_score) endi = i;
  }
  leftd = MAX(1, -endi - low + 1);
  rightd = band - (up - (endj - endi));
  data.CD[rightd].CC = 0; data.CD[rightd + 1].DD = -m;
  t = -G;
  for (j = rightd - 1; j >= leftd; j--) {
    data.CD[j].CC = t = t - H;
    data.CD[j + 1].DD = t - m;
  }
  for (j = rightd + 1; j <= band; ++j) data.CD[j].CC = MININT;
  data.CD[leftd - 1].CC = data.CD[leftd].DD = MININT;
  flag = FALSE; 
  for (i = endi; i >= 1; i--) {
    if (i + low <= 0) leftd++;
    if (rightd < band) rightd++;
    wa = W[A[i]];
    d = data.CD[rightd].DD;
    if ((ib = rightd + low - 1 + i) <= N) c = data.CD[rightd].CC + wa[B[ib]];
    else c = MININT;
    if (d > c) c = d;
    e = c - m;
    data.CD[rightd].CC = c;
    if ((d -= H) < e) data.CD[rightd + 1].DD = e;
    else data.CD[rightd + 1].DD = d;
    if (c == best_score) {
      starti = i;
      startj = ib;
      flag = TRUE;
      break;
    }
    for (curd = rightd - 1, dp = &data.CD[curd]; curd >= leftd; curd--) {
      c = dp->CC + wa[B[curd + low + i - 1]];
      if ((d = dp->DD) > c) c = d;
      if (e >= c) {
	dp->CC = e; e -= H;
	(dp-- + 1)->DD = d - H;
	continue;
      } 
      dp->CC = c;
      if (c == best_score) {
	starti = i;
	startj = curd + low + i - 1;
	flag = TRUE;
	break;
      }
      if ((c -= m) > (e -= H)) e = c;
      if (c < (d -= H)) (dp-- + 1)->DD = d; else (dp-- + 1)->DD = c;
    }
    if (flag == TRUE) 
      break;
  }
  MemFree(data.CD);
  if (starti < 0 || starti > M || startj < 0 || startj > N) 
    {
      ErrPostEx(SEV_WARNING, 0, 0, "starti=%d, startj=%d\n", starti, startj);
      *psi = *psj = *pei = *pej;
      return -1;
    }
  *psi = starti;
  *psj = startj;
  *pei = endi;
  *pej = endj;

  options->start_diag = low - (startj - starti);
  options->width = up - (startj - starti) - low + 1;

  switch(align_type) {
      /* XXX Float Score is converted to Int4 --> Round-off Errors */
  case L_BAND_LINEAR:
    return (Int4) (gband_linear(A + starti - 1, B + startj - 1,
			endi - starti + 1, endj - startj + 1,
			matrix, options, S, &Slen) / BND_DIGIT);
  case L_BAND_QUADRATIC:
    return (Int4) ( gband_quadratic(A + starti - 1, B + startj - 1,
			   endi - starti + 1, endj - startj + 1,
			   matrix, options, S, &Slen) / BND_DIGIT); 
  case L_BAND_LGAP:
    return (Int4) ( gband_linear_gap(A + starti - 1, B + startj - 1,
			    endi - starti + 1, endj - startj + 1,
			    matrix, options, S, &Slen) / BND_DIGIT); 
  case L_BAND_QGAP:
    return (Int4) ( gband_linear_qgap(A + starti - 1, B + startj - 1,
			     endi - starti + 1, endj - startj + 1,
			     matrix, options, S, &Slen) / BND_DIGIT); 
  case L_BAND_L3GAP:
    return (Int4) ( gband_l3gap(A + starti - 1, B + startj - 1,
		       endi - starti + 1, endj - startj + 1,
		       matrix, options, S, &Slen) / BND_DIGIT); 
  case L_BAND_Q3GAP:
    return (Int4) ( gband_q3gap(A + starti - 1, B + startj - 1,
		       endi - starti + 1, endj - startj + 1,
		       matrix, options, S, &Slen) / BND_DIGIT); 
  default:
    ErrPostEx(SEV_ERROR, 0, 0, "Unknown method.");
    return -1;
  }

}

/**********************************************************************
*		Gloabal Alignment utility functions
**********************************************************************/
static Uint1 HSncbi4na_to_blastna[] = {
15,/* Gap, 0 */
0, /* A,   1 */
1, /* C,   2 */
6, /* M,   3 */
2, /* G,   4 */
4, /* R,   5 */
9, /* S,   6 */
13, /* V,   7 */
3, /* T,   8 */
8, /* W,   9 */
 5, /* Y,  10 */
 12, /* H,  11 */
 7, /* K,  12 */
 11, /* D,  13 */
 10, /* B,  14 */
 14  /* N,  15 */
};

static Uint1 HSblastna_to_ncbi4na[] = {   1, /* A, 0 */
                                 2, /* C, 1 */
                                 4, /* G, 2 */
                                 8, /* T, 3 */
                                 5, /* R, 4 */
                                10, /* Y, 5 */
                                 3, /* M, 6 */
                                12, /* K, 7 */
                                 9, /* W, 8 */
                                 6, /* S, 9 */
                                14, /* B, 10 */
                                13, /* D, 11 */
                                11, /* H, 12 */
                                 7, /* V, 13 */
                                15, /* N, 14 */
                                 0, /* Gap, 15 */
                        };

NLM_EXTERN void SetGlobaltOptions(GlobalBandStructPtr gbsp, Int4 lg1_ext, Int4 rg1_ext, Int4 lg2_ext, Int4 rg2_ext, Int4 lg1_open, Int4 lg2_open, Int4 rg1_open, Int4 rg2_open, Int2 gopen, Int2 gext)
{
	PSUGapOptionsPtr opt;

	opt = gbsp->options;
	opt->gopen = gopen;
	opt->gext = gext;

	opt->lg1_ext = lg1_ext;	
	opt->rg1_ext = rg1_ext;
	opt->lg2_ext = lg2_ext;
	opt->rg2_ext = rg2_ext;
	opt->lg1_open = lg1_open;
	opt->lg2_open = lg2_open;
	opt->rg1_open = rg1_open;
	opt->rg2_open = rg2_open;
}

static Int4Ptr PNTR BlastStyleMatCreate(void){
/* Allocate Matrix as one contigous memory block to increase cache-hits&speed
 */
  Int4Ptr PNTR matrix;
  Int4 i;
   matrix= (Int4Ptr PNTR) Calloc(BLAST_MATRIX_SIZE*BLAST_MATRIX_SIZE,MAX(sizeof(Int4),sizeof(Int4Ptr)));
   for(i=0;i<BLAST_MATRIX_SIZE-1;i++) {
     *(matrix+i)=(Int4Ptr) (matrix+BLAST_MATRIX_SIZE+i*BLAST_MATRIX_SIZE);
   }
   return matrix;
}

NLM_EXTERN Int2 ChangeGlobalBandMatrix(GlobalBandStructPtr gbsp, Boolean is_prot, CharPtr matrix_name, Int4 penalty, Int4 reward)
{
   BLAST_ScoreBlkPtr   sbp;
   Int4                i, j;
   Int2 status=0;

   if (is_prot) {
      sbp = BLAST_ScoreBlkNew((Uint1)Seq_code_ncbistdaa, (Int2)1);
      sbp->read_in_matrix = TRUE;
      status = BlastScoreBlkMatFill(sbp, matrix_name);
   } else { 
     sbp = BLAST_ScoreBlkNew((Uint1)BLASTNA_SEQ_CODE, (Int2)2);
     sbp->read_in_matrix = FALSE;
     sbp->penalty = penalty;
     sbp->reward = reward;
     status = BlastScoreBlkMatFill(sbp, NULL);
   }
   if (status != 0) {
     ErrPostEx(SEV_WARNING, 0, 0, "BlastScoreBlkMatFill returned non-zero status ");
     return -1;
   }
   if(gbsp->matrix==NULL){
     gbsp->matrix=(Int4Ptr PNTR) MemFree(gbsp->matrix);
   }
   if((gbsp->matrix=BlastStyleMatCreate())==NULL) {
     ErrPostEx(SEV_WARNING, 0, 0, "BlastStyleMatCreate returned non-zero status ");
     return -1;
   }
    
   if(is_prot) {
     for (i=0;i<sbp->alphabet_size;i++) {
       for (j=0;j<sbp->alphabet_size;j++) {
         gbsp->matrix[i][j] = (Int4) (sbp->matrix[i][j] *BND_DIGIT*(1.000001) );
       }
     }
   } else {
     for (i=0;i<16;i++) {
       for (j=0;j<16;j++) {
         gbsp->matrix[i][j] = (Int4) (sbp->matrix[HSncbi4na_to_blastna[i]][HSncbi4na_to_blastna[j]] *BND_DIGIT*(1.000001) );
       }
     }
   }
   sbp=BLAST_ScoreBlkDestruct(sbp);
   return 0;
/*
	BLAST_ScoreBlkPtr sbp;
	CharPtr name;
	Int2 i, j, status = 0;
	Int4Ptr PNTR matrix;

	if (gbsp == NULL) {
		return 4;
	}
	name = (matrix_name) ? matrix_name : "BLOSUM62";
	if(gbsp->matrix != NULL){
		gbsp->matrix = (Int4Ptr PNTR ) MemFree(gbsp->matrix);
	}
	if (is_prot) {
        sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa, 1);
        sbp->read_in_matrix = TRUE;
		BlastScoreSetAmbigRes(sbp, 'X');
		if((status=BlastScoreBlkMatFill(sbp, name)) != 0) {
			if((matrix=BlastStyleMatCreate()) == NULL) {
				sbp=BLAST_ScoreBlkDestruct(sbp);
				return 0;
			}
			for (i=0;i<sbp->alphabet_size;i++) {
				for (j=0;j<sbp->alphabet_size;j++) {
					matrix[i][j] = (Int4) (sbp->matrix[i][j] *BND_DIGIT*(1.000001) );
				}
			}
			gbsp->matrix = matrix;
		}
		sbp=BLAST_ScoreBlkDestruct(sbp);
		return status;
	}
	sbp = BLAST_ScoreBlkNew(BLASTNA_SEQ_CODE, 2);
	sbp->read_in_matrix = FALSE;
	BlastScoreSetAmbigRes(sbp, 'N');
	sbp->penalty = (Int4) (BND_DIGIT * penalty);
	sbp->reward = (Int4) (BND_DIGIT * reward);
	if((status=BlastScoreBlkMatFill(sbp, NULL)) != 0) {
		if((matrix=BlastStyleMatCreate()) == NULL) {
			sbp=BLAST_ScoreBlkDestruct(sbp);
			return 0;
		}
		for (i=0;i<16;i++) {
			for (j=0;j<16;j++) {
				matrix[i][j] = (Int4) (sbp->matrix[HSncbi4na_to_blastna[i]][HSncbi4na_to_blastna[j]] *BND_DIGIT*(1.000001) );
			}
		}
		gbsp->matrix = matrix;
	}
	sbp=BLAST_ScoreBlkDestruct(sbp);
	return status;
*/
}

static Uint1Ptr load_data (SeqLocPtr slp,Boolean is_prot)
{
  SeqPortPtr       spp;
  Uint1Ptr         seq = NULL;
  Int4 len;
  Int4 index;
  Uint1             code;

  if (is_prot)
    code = Seq_code_ncbistdaa;
  else code = Seq_code_ncbi4na;

  len = SeqLocLen(slp);
  seq = (Uint1Ptr) MemNew(((len)+2)*sizeof(Uint1));
  spp = SeqPortNewByLoc(slp, code);
  for (index=0; index < len; index++)
    {
      seq[index] = SeqPortGetResidue(spp);
    }
  SeqPortFree (spp);
  return seq;
}

NLM_EXTERN GlobalBandStructPtr CreatBandStruct(SeqLocPtr slp1, SeqLocPtr slp2, Int4Ptr PNTR W, Boolean is_prot, Int2 method)
{
	GlobalBandStructPtr gbsp;
	Uint1Ptr seq1,seq2;
	Int4 len1, len2;
	Int2 gopen, gext;  /* default values */
	Int4 ma=1, ms = -2; /* default values */
  
	if (is_prot) {
		gopen = 5;
		gext=1;
	} else {
		gopen = 10;
		gext=2;
	}
	len1 = SeqLocLen(slp1);
	len2 = SeqLocLen(slp2);

	seq1 = load_data(slp1, is_prot);
	seq2 = load_data(slp2, is_prot);
	gbsp = GlobalBandStructCreate((Uint1)method);
	gbsp->seq1 = seq1;
	gbsp->seq2 = seq2;
	gbsp->seq1_length = len1;
	gbsp->seq2_length = len2;
	gbsp->seqloc1 = slp1;
	gbsp->seqloc2 = slp2;
	
	if (W != NULL) {
		gbsp->matrix = W;
	} else if (is_prot) {
		ChangeGlobalBandMatrix(gbsp, TRUE, "BLOSUM62", 0, 0);
	} else {
		ChangeGlobalBandMatrix(gbsp, FALSE, NULL, ma, ms);
	}

/* all end gap penalties are set to zero */
	SetGlobaltOptions(gbsp, 0, 0, 0, 0, 0, 0, 0, 0, gopen, gext);

	return gbsp;
}

/*
   Subroutine to define the band to includes segments in the seqalign(s) 
   obtained from Blast. The Band is to be visualized in the space of sequence
   two (vertical (Y) axis) versus sequence 1 (horizontal (X) axis).
   By default sequences are filtered for low-complexity sequences.
   If no alignment is returned, this routine tries again but without filtering.

   the "type" parameter defines how to choose the seqalign(s) to use.
   type 1 and 2 are meant to give narrow optimal bands.
               (e.g. when one has to align sequences with an overlap )
   type = 2 : Only Use the longest Alignment to define the band.
   type = 1 : Only Use the highest scoring alignment to define the band.

   type = 0: Make the widest possible band that will include all possible
             blocks found by BLAST. This is meant to globally align
	     two sequences. 
	     Additionally, the earliest segment's Y coordinate may increase 
	     the upper limit of the band and the last segments may decrease 
	     the lower limit of the band. These last two modifications insure 
	     that all possible global alignments including those segments are 
	     considered, including those with large start or end gaps.
   */


NLM_EXTERN void SetLowUpFromBlast(PSUGapOptionsPtr opt, Boolean is_prot, Int2 type, Int2 width, SeqLocPtr slp1, SeqLocPtr slp2)
{
	BLAST_OptionsBlkPtr options;
	SeqAlignPtr seqalign, sap;
	Int2 index1, i2;
	Int4 x,y,maxy,miny,mindi,maxdi,mindi_low,maxdi_up;
	Int4 len,len1,len2,maxlen=0,alnlen,lastx,lowestx,diag;
	DenseDiagPtr  ddp;
	ScorePtr score;
	FloatHi val, maxsc = 0;
	DenseSegPtr	dsp;
	ObjectIdPtr id;

	if (opt == NULL)
		return;
	if (width ==0) {
	  width = 30;
	} else if (width<0) {
	  width=0;
	}
	options = BLASTOptionNew((is_prot == FALSE) ? "blastn":"blastp", TRUE);
	while ((seqalign = BlastTwoSequencesByLoc(slp1, slp2,NULL, options))==NULL && (is_prot==FALSE && options->wordsize>8)) {
	  options->wordsize--;
	}
       	options=BLASTOptionDelete(options);

	len1 = SeqLocLen(slp1);
	len2 = SeqLocLen(slp2);

	if (seqalign == NULL || seqalign->segtype > 2) {
 		opt->low = -len2;
		opt->up = len1;
		opt->start_diag = opt->low;
		opt->width = opt->up-opt->low+ 1;
		return;
	}
	miny = len2;
	maxy = - len1;
	lastx = 0;
	lowestx = len1+1;
	sap = seqalign;
	for (; sap != NULL; sap = sap->next) {
		for (score = sap->score; score; score = score->next) {
			id = score->id;
			if (StringCmp(id->str, "score") == 0) {
				if (score->choice == 1) {
					val = score->value.intvalue;
					break;
				}
			}
		}
		if (sap->segtype == 1) {
			ddp = sap->segs;
			x = ddp->starts[0];
			y = ddp->starts[1];
			alnlen = len = ddp->len;
			maxdi = mindi = y - x;
			if(type == 0 ) {
			  if(x+len > lastx) {
			    mindi_low = y-len2+len;
			    lastx = x+len;
			  }
			  if (x<lowestx) {
			    maxdi_up = y;
			    lowestx = x;
			  }
			}
		} else if (sap->segtype == 2) {
			dsp = sap->segs;
			x = dsp->starts[0];
			y = dsp->starts[1];
			alnlen = len = dsp->lens[0];
			maxdi = mindi = y - x;
			if(type == 0 ) {
			  if(x+len >= lastx) {
			    mindi_low = y-len2+len;
			    lastx = x+len;
			  }
			  if (x<lowestx) {
			    maxdi_up = y;
			    lowestx = x;
			  }
			}
			for (index1=1; index1 < dsp->numseg; index1++) {
				i2 = 2 * index1;
				x = dsp->starts[i2];
				y = dsp->starts[i2+1];
				len = dsp->lens[index1];
				if (x == -1 || y == -1) {
					continue;
				}
				if (dsp->strands[i2] == Seq_strand_minus ||
					dsp->strands[i2+1] == Seq_strand_minus){
					continue;
				}
				diag = y-x;
				mindi = MIN(mindi,diag);
				maxdi = MAX(maxdi,diag);
				alnlen += len;
				if(type == 0 ) {
				  if(x+len > lastx) {
				    mindi_low = y-len2+len;
				    lastx = x+len;
				  }
				  if (x<lowestx) {
				    maxdi_up = y;
				    lowestx = x;
				  }
				}
			}
		}
		switch(type) {
			case 0:
			        mindi-=width;
				maxdi+=width;
				miny = MIN(miny, mindi);
				maxy = MAX(maxy, maxdi);
				break;
			case 1:
			if (maxsc < val) {
				maxsc = val;
				maxy = maxdi + width;
				miny = mindi - width;
			}
			break;
			case 2:
			if (maxlen < alnlen) {
				maxlen = alnlen;
				maxy = maxdi + width;
				miny = mindi - width;
			}
			break;
		}
	}
	if(type==0) {
	  miny = MIN(miny,mindi_low);
	  maxy = MAX(maxy,maxdi_up);
	}
	if (miny<-len1) miny=-len1;
	if (maxy>len2) maxy=len2;
	opt->low = miny;
	opt->up = maxy;
	opt->start_diag = miny;
	opt->width = maxy - miny + 1;
	opt->start_diag=opt->low;
	opt->width = opt->up - opt->low + 1;
	SeqAlignFree(seqalign);
}


NLM_EXTERN SeqAlignPtr GlobalBandByLoc(GlobalBandStructPtr gbsp, SeqLocPtr slp1, SeqLocPtr slp2,  Boolean is_prot, Int2 band_method)
{
	PSUGapOptionsPtr opt;
	SeqAlignPtr seqalign;

	opt = gbsp->options;
	SetLowUpFromBlast(opt, is_prot, band_method, 0, slp1, slp2);
	opt->start_diag = opt->low;
	opt->width = opt->up - opt->low + 1;

	seqalign = GlobalBandToSeqAlign(gbsp);
	AdjustOffSetsInSeqAlign(seqalign, slp1, slp2);
	return seqalign;
}

NLM_EXTERN SeqAlignPtr ExtendSeqAlign(SeqAlignPtr sap, Int4 start1, Int4 start2, Int4 stop1, Int4 stop2, Int4 x1, Int4 y1, Int4 x2, Int4 y2)
{
   DenseSegPtr   dsp;
   Int4          index1;
   Int4Ptr       n_starts, n_lens;
   Uint1Ptr      n_strands;
   Int2          n;
   Int2          offset=0;

   if (sap==NULL)
      return NULL;
   if (start1==x1 && start2==y1 && stop1==x2 && stop2==y2)
      return sap;
   if (sap->segtype == 1) {
       DenseDiagPtr  ddp;
       ddp = sap->segs;
       ErrPostEx(SEV_WARNING,0,0,"ExtendSeqAlign doesn't support segtype==1\n");
   } else if (sap->segtype == 2) {
      dsp = sap->segs;
      n = dsp->numseg;
      n_starts = (Int4Ptr) MemNew((2*(n+2)+4)*sizeof(Int4));
      n_lens = (Int4Ptr) MemNew((n+4)*sizeof(Int4));
      n_strands = MemNew(2*(n+2)+4);
      if (x1 > start1 || y1 > start2) { 
         if (x1 > start1) {
            n_starts[0] = 0;
            n_starts[1] = -1;
            n_lens[0] = x1 - start1;
            n_strands[0] = Seq_strand_plus;
            n_strands[1] = Seq_strand_plus; 
         } else if (y1 > start2) {
            n_starts[0] = -1;
            n_starts[1] = 0;
            n_lens[0] = y1 - start2;
            n_strands[0] = Seq_strand_plus;
            n_strands[1] = Seq_strand_plus; 
         }
         offset += 1;
      }
      for (index1=0; index1<n; index1++) {
         n_starts [2*(index1+offset)] = dsp->starts [2*index1];
         n_strands[2*(index1+offset)] = dsp->strands[2*index1];
         n_starts [2*(index1+offset)+1]=dsp->starts [2*index1+1];
         n_strands[2*(index1+offset)+1]=dsp->strands[2*index1+1];
         n_lens[index1+offset] = dsp->lens[index1];
      }
      if (x2 < stop1 || y2 < stop2) { 
         n += offset;
         if (x2 < stop1) {
            n_starts[2*n] = x2;
            n_starts[2*n+1] = -1;
            n_lens[n] = stop1 - x2;
            n_strands[2*n] = Seq_strand_unknown;
            n_strands[2*n+1] = Seq_strand_plus;
         } else if (y2 < stop2) {
            n_starts[2*n] = -1;
            n_starts[2*n+1] = y2;
            n_lens[n] = stop2 - y2;
            n_strands[2*n] = Seq_strand_unknown;
            n_strands[2*n+1] = Seq_strand_plus;
         }
         offset += 1;
      }
      dsp->numseg += offset;
      MemFree(dsp->starts);
      MemFree(dsp->strands);
      MemFree(dsp->lens);
      dsp->starts = n_starts;
      dsp->lens = n_lens;
      dsp->strands = n_strands;
   }
   return sap;
}

NLM_EXTERN SeqAlignPtr CC_ExtendSeqAlign(SeqAlignPtr sap, Int4 start1, Int4 start2, Int4 stop1, Int4 stop2, Int4 x1, Int4 y1, Int4 x2, Int4 y2, Uint1 strand1, Uint1 strand2)
{
   DenseSegPtr   dsp;
   Int4Ptr       n_starts, n_lens;
   Uint1Ptr      n_strands;
   Int4          index1,
                 minlen = 0;
   Int2          n;
   Int2          offset=0;

   if (sap==NULL)
      return NULL;
   if (start1==x1 && start2==y1 && stop1==x2 && stop2==y2)
      return sap;
   if (sap->segtype == 1) {
       DenseDiagPtr  ddp; 
     ddp = (DenseDiagPtr) sap->segs;
       ErrPostEx(SEV_WARNING,0,0,"ExtendSeqAlign doesn't support segtype==1\n");
   } else if (sap->segtype == 2) {
      dsp = (DenseSegPtr) sap->segs;
      n = dsp->numseg;
      n_starts = (Int4Ptr) MemNew((2*(n+2)+4)*sizeof(Int4));
      n_lens = (Int4Ptr) MemNew((n+4)*sizeof(Int4));
      n_strands = MemNew((2*(n+2)+4)*sizeof(Int1));
      if (x1 > start1 && y1 > start2) { 
         minlen = MIN ((x1-start1), (y1-start2)); 
         n = 0;
      }
      if ((x1 > start1 || y1 > start2)  && ((x1-start1) != (y1-start2))) { 
         if (x1 > start1) {
            n_starts[0] = 0;
            n_starts[1] = -1;
            n_lens[0] = (x1 - start1) - minlen;
            n_strands[0] = strand1;
            n_strands[1] = strand2; 
         } 
         else if (y1 > start2) {
            n_starts[0] = -1;
            n_starts[1] = 0;
            n_lens[0] = (y1 - start2) - minlen;
            n_strands[0] = strand1;
            n_strands[1] = strand2; 
         }
         offset += 1;
      }
      if (minlen > 0) {
         n += offset;
         n_starts[2*n] = x1 - minlen;
         n_starts[2*n+1] = y1 - minlen;
         n_lens[n] = minlen;
         n_strands[2*n] = strand1;
         n_strands[2*n+1] = strand2;
         offset += 1;
      }
      n = dsp->numseg;
      for (index1=0; index1<n; index1++) {
         n_starts [2*(index1+offset)] = dsp->starts [2*index1];
         n_strands[2*(index1+offset)] = dsp->strands[2*index1];
         n_starts [2*(index1+offset)+1]=dsp->starts [2*index1+1];
         n_strands[2*(index1+offset)+1]=dsp->strands[2*index1+1];
         n_lens[index1+offset] = dsp->lens[index1];
      }
      if (x2 < stop1 &&  y2 < stop2) {
         n += offset;
         minlen = MIN ((stop1-x2), (stop2-y2));
         n_starts[2*n] = x2 + 1;
         n_starts[2*n+1] = y2 +1;
         n_lens[n] = minlen;
         n_strands[2*n] = strand1;
         n_strands[2*n+1] = strand2;
         x2 += minlen;
         y2 += minlen;
         offset += 1;
      }
      if (x2 < stop1 || y2 < stop2) { 
         n += offset;
         if (x2 < stop1) {
            n_starts[2*n] = x2 +1;
            n_starts[2*n+1] = -1;
            n_lens[n] = stop1 - x2;
            n_strands[2*n] = strand1;
            n_strands[2*n+1] = strand2;
         } else if (y2 < stop2) {
            n_starts[2*n] = -1;
            n_starts[2*n+1] = y2 +1;
            n_lens[n] = stop2 - y2;
            n_strands[2*n] = strand1;
            n_strands[2*n+1] = strand2;
         }
         offset += 1;
      }
      dsp->numseg = n+1;
      MemFree(dsp->starts);
      MemFree(dsp->strands);
      MemFree(dsp->lens);
      dsp->starts = n_starts;
      dsp->lens = n_lens;
      dsp->strands = n_strands;
   }
   return sap;
}

/* doesn't work for minus strands in SeqAlign */
/* deals with segtypes 1 and 2 only*/
NLM_EXTERN void GetAlignExtremes(SeqAlignPtr sap, Int4Ptr xx1, Int4Ptr yy1, Int4Ptr xx2, Int4Ptr yy2)
{
	DenseDiagPtr  	ddp;
	DenseSegPtr		dsp;
	Int4         	index1, x, y, x1, x2, y1, y2;
	
	if (sap == NULL || sap->segtype > 2) {
		return;
	}
	if (sap->segtype == 1) {
		ddp = sap->segs;
		x1 = ddp->starts[0];
		y1 = ddp->starts[1];
		x2 = x1 + ddp->len;
		y2 = y1 + ddp->len;
	} else if (sap->segtype == 2) {
		dsp = sap->segs;
		x1 = dsp->starts[0];
		y1 = dsp->starts[1];
		for (index1=0; index1<dsp->numseg; index1++) {
			x = dsp->starts[2 * index1];
			y = dsp->starts[2 * index1+1];
			if (x == -1 || y == -1) {
				continue;
			}
			x2 = x + dsp->lens[index1] - 1;
			y2 = y + dsp->lens[index1] - 1;
		}
	}
	*xx1 = x1;
	*xx2 = x2;
	*yy1 = y1;
	*yy2 = y2;
}

