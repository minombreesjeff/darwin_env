static char const rcsid[] = "$Id: bandalg1.c,v 6.3 2003/05/30 17:25:35 coulouri Exp $";

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

File name: bandalgn1.c

Author: Gennadiy Savchuk, Jinqhui Zhang, Tom Madden

Contents: Functions to perform global quadratic banded alignments.

*****************************************************************************/

#include <bandalgn.h>

static Int4 g_band1_align(Uint1Ptr A, Uint1Ptr B,
			  Int4 M, Int4 N,
			  Int4 low, Int4 up,
			  data_t *data);

static Int4 g_band1_CHECK_SCORE(Uint1Ptr A, Uint1Ptr B,
				Int4 M, Int4 N,
				Int4 PNTR S, data_t *data);

Int4 LIBCALL gband_quadratic(Uint1Ptr A, Uint1Ptr B,
			     Int4 M, Int4 N,
			     Int4Ptr PNTR matrix,
			     PSUGapOptionsPtr option,
			     Int4Ptr S, Int4Ptr Slen)
{
  data_t data;
  Int4 c, i, j;
  Int4 low, up;
  Int4 band;
  Int4 score;

  /* Setup global parameters */
  data.g = option->gopen;
  data.zzh = option->gext;
  data.w = matrix;
  data.m = data.g + data.zzh;

  data.sapp = S;
  data.last = 0;
  *Slen = 0;

  low = option->start_diag;
  band = option->width;
  up = band + low - 1;

  low = MIN(MAX(-M, low),MIN(N - M, 0));
  up = MAX(MIN(N, up),MAX(N - M, 0));

  if (up < 0 || low > 0) {
    ErrPostEx(SEV_WARNING, 0, 0,
	      "The band does not include (0,0) grid point");
    return 0;
  } 
  if (up+M < N || low+M > N) {
    ErrPostEx(SEV_WARNING, 0, 0,
	      "The band does not include lower corner");
    return 0;
  }
  
  if (N <= 0) { 
    if (M > 0) DEL_(M);
    return -gap_(M);
  }
  if (M <= 0) {
    INS_(N);
    return -gap_(N);
  }
  if ((band = up-low+1) <= 1) {
    c = 0;
    for (i = 1; i <= M; i++) {
      REP_;
      c += data.w[A[i]][B[i]];
    }
    return c;
  }

  j = (band+2) * sizeof(Int4);

  data.CD = (dp_ptr) MemGet(sizeof(dp_node)*(band+2), MGET_ERRPOST);

  c = g_band1_align(A, B, M, N, low, up, &data);

  score = g_band1_CHECK_SCORE(A, B, M, N, S, &data);

  if (score != c) {
    ErrPostEx(SEV_WARNING, 0, 0, "Check_Score = %ld align_score = %ld ", 
	      (long) score, (long) c);
	return 0;
  }
  MemFree(data.CD);

  *Slen = data.sapp - S;

  return c;
}

/* g_band1_CHECK_SCORE - return the score of the alignment stored in S */

static Int4 g_band1_CHECK_SCORE(Uint1Ptr A, Uint1Ptr B,
			Int4 M, Int4 N, Int4 PNTR S, data_t *data)
{ 
  register Int4   i,  j, op;
  Int4 score;

  score = i = j = op = 0;
  while (i < M || j < N) {
    op = *S++;
    if (op == 0) 
      score = data->w[A[++i]][B[++j]] + score;
    else if (op > 0) {
      score = score - (data->g + op * data->zzh);
      j = j + op;
    } else {
      score = score - (data->g - op * data->zzh);
      i = i - op;
    }
  }
  return(score);
}

/* g_band1_align(A,B,M,N,up,low,tb,te,data) returns the cost of an optimum
   conversion between
   A[1..M] and B[1..N] and appends such a conversion to the current script.
   tb(te)= 1  no gap-open penalty if the conversion begins(ends) with a delete.
   tb(te)= 2  no gap-open penalty if the conversion begins(ends) with an insert.
*/

static Int4 g_band1_align(Uint1Ptr A, Uint1Ptr B,
		  Int4 M, Int4 N,
		  Int4 low, Int4 up, data_t *data)
{
  Int4 k, v;
  Int4 band, j;
  Int4 leftd, rightd;	/* for CC, DD, CP and DP */
  register Int4 curd;	/* current index for CC, DD CP and DP */
  register Int4 i;
  register Int4 c, d, e;
  register dp_ptr ap;
  Int4 t;
  Int4Ptr wa;
  Int1Ptr PNTR state, st, tmp;
  Int4 ib;


  /* Boundary cases: M <= 0 , N <= 0, or up-low <= 0 */
  band = up-low+1;
  state = (Int1Ptr PNTR) MemGet(sizeof(Int1Ptr)*(M+1), MGET_ERRPOST);
  state[0] = (Int1Ptr) MemGet((M+1)*(band+2), MGET_ERRPOST);
  for (i = 1; i <= M; i++) state[i] = state[i-1]+band+2;

  /* Initialization */
  leftd = 1-low;
  rightd = up-low+1;

  data->CD[leftd].CC = 0; state[0][leftd] = -1;
  t = -data->g;
  for(j = leftd+1; j <= rightd; j++) {
    data->CD[j].CC = t = t - data->zzh;
    data->CD[j-1].DD = t - data->m;
    state[0][j] = 1;
  }
  data->CD[rightd+1].CC = MININT;
  data->CD[rightd].DD = MININT;
  data->CD[leftd-1].DD = -data->m;
  data->CD[leftd-1].CC = MININT;
  for (i = 1; i <= M; i++) {
    if (i > N-up) rightd--;
    if (leftd > 1) leftd--;
    wa = data->w[A[i]];
    d = data->CD[leftd].DD;
    k = 0;
    if ((ib = leftd+low-1+i) > 0) c = data->CD[leftd].CC+wa[B[ib]];
    if (d > c || ib <= 0) {
      c = d;
      k = 2;
    }
    e = c - data->m;
    if(leftd >= 1) 
      if((d -= data->zzh) >= e) {
	data->CD[leftd - 1].DD = d;
	k += 20;
      }
      else data->CD[leftd - 1].DD = e;

    st = &state[i][leftd];
    *st++ = (Uint1) k;
    data->CD[leftd].CC = c;
    for(curd = leftd + 1, ap = &data->CD[curd]; curd <= rightd; curd++) {
      c = ap->CC + wa[B[curd + low - 1 + i]];
      if((d = ap->DD) > c) {
	if(d > e) {
	  ap->CC = d; e -= data->zzh; 
	  (ap++ - 1)->DD = d - data->zzh; *st++ = 32;
	}
	else {
	  ap->CC = e; e -= data->zzh;
	  (ap++ - 1)->DD = d - data->zzh; *st++ = 31;
	}
      }
      else if(e > c) {
	ap->CC = e; e -= data->zzh;
	(ap++ - 1)->DD = d - data->zzh; *st++ = 31;
      }
      else {
	ap->CC = c;
	if ((c -= data->m) > (e -= data->zzh)) {
	  e = c;
	  k = 0;
	}
	else k = 10;
	if(c > (d -= data->zzh)) {
	  *st++= (Uint1) k;
	  (ap++ - 1)->DD = c;
	} 
	else {
	  *st++ = k + 20;
	  (ap++-1)->DD = d;
	}
      }
    }
  }

  /* decide which path to be traced back */
  v = (ap-1)->CC;
  tmp = MemGet(M+N, MGET_ERRPOST);
  for (i = M, j = rightd, e=0, c = 0; i>=0; i--, c++) {
    k  = (t=state[i][j]) %10;
    if (t == -1) break;
    if (e == 1 && (t/10)%2 == 1) k = 1;
    if (e == 2 && (t/20)== 1) k = 2;
    if (k == 1) { j--; i++;}
    else if (k == 2) j++;
    e = k;
    tmp[c] = (Uint1) e;
  }
  for (i = c-1; i >= 0; i--) 
    switch(tmp[i]) {
    case 0: 
      _REP;
      break;
    case 1:
      _INS(1);
      break;
    case 2:
      _DEL(1);
      break;
    }
  MemFree(state[0]);
  MemFree(state);
  MemFree(tmp);
  return(v);
}

