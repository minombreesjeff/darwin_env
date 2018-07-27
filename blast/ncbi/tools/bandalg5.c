static char const rcsid[] = "$Id: bandalg5.c,v 6.3 2003/05/30 17:25:36 coulouri Exp $";

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

#include <bandalgn.h>

static Int4 g_band5_align(Uint1Ptr A, Uint1Ptr B,
			  Int4 M, Int4 N,
			  Int4 low, Int4 up,
			  data_t *data);

static Int4 g_band5_CHECK_SCORE(Uint1Ptr A, Uint1Ptr B,
				Int4 M, Int4 N,
				Int4 PNTR S,
				data_t *data);

/************************************************************/
Int4 LIBCALL gband_q3gap(Uint1Ptr A, Uint1Ptr B,
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

  data.g = option->gopen;
  data.zzh = option->gext;
  data.w = matrix;	/* Setup global parameters */
  data.m = data.g + data.zzh;
  data.rr = 2;

  data.sapp = S;
  data.last = 0;
  *Slen = 0;

  low = option->start_diag;
  band = option->width;
  up = band + low - 1;

  low = MIN(MAX(-M, low), MIN(N - M, 0));
  up = MAX(MIN(N, up), MAX(N - M, 0));

  if (up < 0 || low > 0) {
    ErrPostEx(SEV_WARNING, 0, 0, "The band does not include (0,0) grid point");
    return 0;
  } 
  if (up + M < N || low + M > N) {
    ErrPostEx(SEV_WARNING, 0, 0, "The band does not include lower corner");
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
  data.leggA = data.leggB = data.reggA = data.reggB =
    data.leghA = data.leghB = data.reghA = data.reghB = 0;
  data.CD = (dp_ptr) MemGet(sizeof(dp_node) * (band + 2), MGET_ERRPOST);

  c = g_band5_align(A, B, M, N, low, up, &data);

  score = g_band5_CHECK_SCORE( A, B, M, N, S, &data);
  if (score != c) {
    ErrPostEx(SEV_WARNING, 0, 0, "Check_Score = %ld align_score = %ld ", 
	      (long) score, (long) c);
	return 0;
  }
  MemFree(data.CD);

  *Slen = data.sapp - S;

  return c;
}

/* g_band5_align(A,B,M,N,up,low,tb,te, data) returns the cost of an optimum
   conversion between
   A[1..M] and B[1..N] and appends such a conversion to the current script.
   tb(te)= 1  no gap-open penalty if the conversion begins(ends) with a delete.
   tb(te)= 2  no gap-open penalty if the conversion begins(ends) with an insert.
*/
static Int4 g_band5_align(Uint1Ptr A, Uint1Ptr B,
			  Int4 M, Int4 N,
			  Int4 low, Int4 up,
			  data_t *data)
{
  Int4 k, v;
  Int4 band, j;
  Int4 leftd, rightd;	/* for CC, DD, CP and DP */
  register Int4 curd;	/* current index for CC, DD CP and DP */
  register Int4 i;
  register Int4 c, d, e, x, f;
  register dp_ptr ap;
  Int4 t;
  Int4Ptr wa;
  Int1Ptr PNTR state, st;
  Uint1Ptr tmp;
  Int4 ib, best=MININT, X, Y;

  /* Boundary cases: M <= 0 , N <= 0, or up-low <= 0 */
  band = up - low + 1;
  state = (Int1Ptr PNTR) MemGet(sizeof(Uint1 *) * (M + 1), MGET_ERRPOST);
  state[0] = (Int1Ptr) MemGet((M + 1) * (band + 2), MGET_ERRPOST);
  for(i = 1; i <= M; i++) state[i] = state[i - 1] + band + 2;

  /* Initialization */
  leftd = 1-low;
  rightd = up-low+1;

  data->CD[leftd].CC = 0; state[0][leftd] = -1;

  t = -data->leggB;
  data->CD[leftd].FF = -data->g - data->rr;
  for(j = leftd + 1; j <= rightd; j++) {
    data->CD[j].CC = t = t - data->leghB;
    data->CD[j].FF = t - data->g - data->rr;
    data->CD[j-1].DD = t - data->m;
    state[0][j] = 1;
  }
  for(j = 0; j < leftd; j++) data->CD[j].FF = MININT;
  data->CD[rightd+1].CC = MININT;
  data->CD[rightd].DD = MININT;
  data->CD[leftd-1].DD = -data->leggA;
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
    if(c < data->CD[leftd].FF) { c= data->CD[leftd].FF; k = 3;}
    e = c - data->m;
    if(e < data->CD[leftd].FF - data->zzh) {
      e = data->CD[leftd].FF - data->zzh;
      k += 20;
    }    
    if (c-data->g > data->CD[leftd].FF) {data->CD[leftd].FF = c-data->g-data->rr;}
    else {data->CD[leftd].FF -= data->rr; k+=5;}
    if (ib <= 0) {
      data->CD[leftd-1].DD = d - data->leghA;
      k += 30;
    }
    st = &state[i][leftd];
    *st++ = (Uint1) k;
    data->CD[leftd].CC = c;
    for(curd=leftd+1, ap = &data->CD[curd]; curd <= rightd; curd++) {
      c = ap->CC + wa[B[curd+low-1+i]];
      if((d = ap->DD) > e) {
	if(d > c) {
	  if(d < (f = ap->FF)) { 
	    ap->CC = f;
	    e = (ap - 1)->DD = f - data->zzh;
	    *st++ = 88;
	  }
	  else {
	    if(e < f) {
	      e = f - data->zzh;
	      *st++ = 57;
	    }
	    else {
	      e -= data->zzh;
	      *st++ = 47;
	    }
	    (ap - 1)->DD = d - data->zzh;
	    ap->CC = d;
	  }
	  ap++->FF -= data->rr;
	  continue;
	}
      } else if(e > c) {
	if(e < (f = ap->FF)) {
	  ap->CC = f;
	  e = (ap - 1)->DD = f - data->zzh;
	  *st++ = 88;
	}
	else {
	  ap->CC = e;
	  e -= data->zzh; 
	  if(d < f) {
	    (ap-1)->DD = f - data->zzh;
	    *st++ = 76;
	  }
	  else {
	    (ap-1)->DD = d - data->zzh;
	    *st++ = 46;
	  }
	} 
	ap++->FF -= data->rr;
	continue;
      } 
      if((f = ap->FF) > c) {
	ap->CC = f;
	(ap-1)->DD = e = f-data->zzh;
	ap++->FF -= data->rr;
	*st++ = 88;
      } else {
	ap->CC = c;
	c -= data->g;
	if (c > f) {
	  ap->FF = c-data->rr;
	  if (c > e) {e = c-data->zzh;k=0;}
	  else { e -= data->zzh; k=10;}
	  if (c > d) { (ap++-1)->DD = c-data->zzh; }
	  else { (ap++-1)->DD = d-data->zzh; k+=30;}
	} else {
	  ap->FF -= data->rr; 
	  if (f > e) { 
	    if (f > d) {e = (ap++-1)->DD = f-data->zzh; k = 85;} 
	    else { e = f-data->zzh; (ap++-1)->DD = d-data->zzh; k=55; }
	  } else {
	    e -= data->zzh;
	    if (f > d) {(ap++-1)->DD = f-data->zzh; k=75;}
	    else {(ap++-1)->DD = d-data->zzh; k=45;}
	  }
	}
	*st++= (Uint1) k;
      }
    }
    if(i > N - up && best < (j = (ap - 1)->CC - data->reggA - (N - i) *
			     data->reghA)) {
      best = j;
      X = i;
      Y = rightd;
    }
  }
  for(ap = &data->CD[leftd]; ap <= &data->CD[rightd]; ap++) {
    if((j = ap->CC - data->reggB - data->reghB *
	(x = (&data->CD[rightd] - ap))) > best) {
      X = M;
      best = j;
      Y = rightd - x;
    }
  }
  v = best;
  tmp = MemGet(M + N, MGET_ERRPOST);
  for (i = X, j = Y, e=0, c = 0; i>=0; i--, c++) {
    k  = (t=state[i][j]) %5;
    if (t == -1) break;
    if (e == 1) 
      if ((t/10)%3 == 1) k = 1;
      else if ((t/10)%3 == 2) k = 3;
    if (e == 2) 
      if ((t/30)== 1) k = 2;
      else if ((t/30) == 2) k = 3;
    if (e == 3 &&  ((t/5)%2==1)) k = 3;
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
    case 3:
      _REPP;
      break;
    }
  if (X != M) _DEL(M - X);
  if (Y != rightd) _INS(rightd - Y);
   
  MemFree(tmp);
  MemFree(state[0]);
  MemFree(state);
  return(v);
}


/* g_band5_CHECK_SCORE - return the score of the alignment stored in S */

static Int4 g_band5_CHECK_SCORE(Uint1Ptr A, Uint1Ptr B,
			Int4 M, Int4 N,
			Int4 PNTR S,
			data_t *data)
{ 
  register Int4   i,  j, op;
  Int4 score;

  score = i = j = op = 0;
  if (*S < 0 && *S != MININT) {
    score -= data->leggA - *S * data->leghA;
    i = i - *S++;
  }
  else if((*S) >0) {
    score -= data->leggB + *S * data->leghB;
    j = j + *S++;
  }
  while(i < M || j < N) {
    op = *S++;
    if (op == 0) 
      score = data->w[A[++i]][B[++j]] + score;
    else if(op == MININT) {
      i++;
      j++; 
      if(*(S - 2) == MININT) score -= data->rr;
      else score -= data->g + data->rr;
    }
    else if(op > 0) {
      score = score - (data->g+op*data->zzh);
      j = j + op;
      if(*(S - 2) == MININT) score += data->g;
    }
    else {
      score = score - (data->g - op * data->zzh);
      i = i - op;
      if(*(S - 2)== MININT) score += data->g;
    }
  }
  if(op < 0 && op != MININT)
    score += (data->g - op * data->zzh) - (data->reggA - data->reghA * op);
  else if(op > 0)
    score += (data->g + op * data->zzh) - (data->reggB + data->reghB * op);
  return(score);
}
