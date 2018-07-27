static char const rcsid[] = "$Id: bandalg4.c,v 6.4 2003/05/30 17:25:36 coulouri Exp $";

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

Contents: Functions to perform global banded alignment.

****************************************************************************/

#include <bandalgn.h>

static Int4 g_band4_align(Uint1Ptr A, Uint1Ptr B,
			  Int4 M, Int4 N,
			  Int4 low, Int4 up,
			  Uint1 tb, Uint1 te,
			  Uint1 legA, Uint1 legB,
			  Uint1 regA, Uint1 regB,
			  data_t *data);

static Int4 g_band4_align_small(Uint1Ptr A, Uint1Ptr B,
				Int4 M, Int4 N,
				Int4 low, Int4 up,
				Uint1 tb, Uint1 te,
				Uint1 leA, Uint1 leB,
				Uint1 reA, Uint1 reB,
				data_t *data);

static Int4 g_band4_CHECK_SCORE(Uint1Ptr A, Uint1Ptr B,
				Int4 M, Int4 N,
				Int4 PNTR S,
				data_t *data);

/************************************************************/
Int4 LIBCALL gband_l3gap(Uint1Ptr A, Uint1Ptr B,
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
  data.rr = 2;

  data.sapp = data.sapp0= S;
  data.last = 0;
  *Slen = 0;

  low = option->start_diag;
  band = option->width;
  up = band + low - 1;

  low = MIN(MAX(-M, low), MIN(N - M, 0));
  up = MAX(MIN(N, up), MAX(N - M, 0));

  if(up < 0 || low > 0) {
    ErrPostEx(SEV_WARNING, 0, 0,
	      "The band does not include (0,0) grid point");
    return 0;
  } 
  if(up + M < N || low + M > N) {
    ErrPostEx(SEV_WARNING, 0, 0,
	      "The band does not include lower corner");
    return 0;
  }
  
  data.state = (Int1Ptr PNTR) MemGet(sizeof(Uint1Ptr)*(M+1), MGET_ERRPOST);
  data.state[0] = (Int1Ptr) MemGet((M+1)*(SMBAND+2), MGET_ERRPOST);
  for (i = 1; i <= M; i++) data.state[i] = data.state[i-1]+SMBAND+2;

  if(N <= 0) { 
    if(M > 0) DEL_(M);
    return -gap_(M);
  }
  if(M <= 0) {
    INS_(N);
    return -gap_(N);
  }
  if((band = up - low + 1) <= 1) {
    c = 0;
    for(i = 1; i <= M; i++) {
      REP_;
      c += data.w[A[i]][B[i]];
    }
    return c;
  }

  j = (band + 2) * sizeof(Int4);

  data.CD = (dp_ptr) MemGet(sizeof(dp_node)*(band+2), MGET_ERRPOST);
  j = M + 1;
  data.MT[0] = (Uint1Ptr) MemGet(j, MGET_ERRPOST);
  data.MT[1] = (Uint1Ptr) MemGet(j, MGET_ERRPOST);
  data.MT[2] = (Uint1Ptr) MemGet(j, MGET_ERRPOST);
  data.MT[3] = MemGet(j, MGET_ERRPOST);
  data.FT = (Uint1Ptr) MemGet(j, MGET_ERRPOST);
  j *= sizeof(Int4);
  data.MP[0] = (Int4Ptr) MemGet(j, MGET_ERRPOST);
  data.MP[1] = (Int4Ptr) MemGet(j, MGET_ERRPOST);
  data.MP[2] = (Int4Ptr) MemGet(j, MGET_ERRPOST);
  data.MP[3] = (Int4Ptr) MemGet(j, MGET_ERRPOST);
  data.FP = (Int4Ptr) MemGet(j, MGET_ERRPOST);

  data.leggA = data.leggB = data.reggA = data.reggB = data.leghA =
    data.leghB = data.reghA = data.reghB = 0;
	
  c = g_band4_align(A, B, M, N, low, up, 0, 0, 0, 0, 0, 0, &data);
  score = g_band4_CHECK_SCORE( A, B, M, N, S, &data);
	
  if (score != c) {
    ErrPostEx(SEV_WARNING, 0, 0, "Check_Score = %ld align_score = %ld ", 
	      (long) score, (long) c);
	return 0;
  }

  /* Now deallocating memory */
  MemFree(data.state[0]);
  MemFree(data.state);
  MemFree(data.CD);
  MemFree(data.FT);
  MemFree(data.FP);
  for(j = 0; j < 4; j++) {
    MemFree(data.MT[j]);
    MemFree(data.MP[j]);
  }

  *Slen = data.sapp - S;

  return c;
}

/************************************************************/
/* g_band4_align(A, B, M, N, up, low, tb, te, data) returns the cost of an
   optimum conversion between
   A[1..M] and B[1..N] and appends such a conversion to the current script.
   tb(te)= 1  no gap-open penalty if the conversion begins(ends) with a delete.
   tb(te)= 2  no gap-open penalty if the conversion begins(ends) with an insert.
*/

static Int4 g_band4_align(Uint1Ptr A, Uint1Ptr B,
		  Int4 M, Int4 N,
		  Int4 low, Int4 up,
		  Uint1 tb, Uint1 te,
		  Uint1 legA, Uint1 legB,
		  Uint1 regA, Uint1 regB,
		  data_t *data)
{
  Int4 rmid, k, l, r, v, kt, X, Y;
  Int4 t1, t2, t3;
  Uint1Ptr FT;
  Int4Ptr FP;
  {
    Int4 band, midd, j;
    Int4 leftd, rightd;	/* for CC, DD, CP and DP */
    register Int4 curd;	/* current index for CC, DD CP and DP */
    register Int4 i;
    register Int4 c, d, e, x, f;
    register dp_ptr ap;
    Int4 t, fr;
    Int4Ptr wa;
    Int4 ib, best = MININT;

    /* Boundary cases: M <= 0 , N <= 0, or up-low <= 0 */
    if(N <= 0) { 
      if(M > 0) _DEL(M);
      return 0;
    }
    if(M <= 0) {
      _INS(N);
      return 0;
    }
    if((band = up - low + 1) <= SMBAND) {
      return g_band4_align_small(A, B, M, N, low, up, tb, te,
			 legA, legB, regA, regB, data);
    }

    /* Divide: Find all crossing points */

    /* Initialization */
    midd = band/2 + 1;
    rmid = low + midd - 1;
    leftd = 1-low;
    rightd = up-low+1;
    if (leftd < midd) {
      fr = -1;
      for (j = 0; j < midd; j++) 
	data->CD[j].CP = data->CD[j].DPDP = data->CD[j].FP = -1;
      for (j = midd; j <= rightd; j++) {
	data->CD[j].CP = data->CD[j].DPDP = data->CD[j].FP = 0;
      }
      data->CD[midd-1].DPDP = 0;
      data->MP[0][0] = -1;
      data->MP[1][0] = -1;
      data->MP[2][0] = -1;
      data->MP[3][0] = -1;
    } else if (leftd > midd) {
      fr = leftd-midd;
      for (j = 0; j <= midd; j++) {
	data->CD[j].CP = data->CD[j].DPDP = data->CD[j].FP = fr;
      }
      for (j = midd+1; j <= rightd; j++) 
	data->CD[j].CP = data->CD[j].DPDP = data->CD[j].FP = -1;
      data->CD[midd].DPDP = -1;
      data->MP[0][fr] = -1;
      data->MP[1][fr] = -1;
      data->MP[2][fr] = -1;
      data->MP[3][fr] = -1;
    } else {
      for (j = 0; j < rightd; j++) {
	data->CD[j].CP = data->CD[j].DPDP = data->CD[j].FP = 0;
      }
      data->MP[0][0] = -1;
      data->MP[1][0] = -1;
      data->MP[2][0] = -1;
      data->MP[3][0] = -1;
    }

    data->CD[leftd].CC = 0; data->CD[leftd].FF = -data->g - data->rr;
    if(!legB) {
      t = -data->leggB; 
      for(j = leftd + 1; j <= rightd; j++) {
	data->CD[j].CC = t = t - data->leghB;
	data->CD[j].FF = t - data->g - data->rr;
	data->CD[j-1].DD = t - data->m;
      }
    }
    else {
      if(tb == 2) t = 0;
      else t = -data->g;
      for(j = leftd + 1; j <= rightd; j++) {
	data->CD[j].CC = t = t - data->zzh;
	data->CD[j].FF = t - data->g - data->rr;
	data->CD[j-1].DD = t - data->m;
      }
    }
    for(j = 0; j < leftd; j++) data->CD[j].FF = MININT;
    data->CD[rightd+1].CC = MININT;
    data->CD[rightd].DD = MININT;
    data->CD[rightd+1].CP = data->CD[rightd].DPDP = 0;
    if(tb == 1) data->CD[leftd-1].DD = -data->zzh;
    else data->CD[leftd-1].DD = -data->m;
    if(!legA) data->CD[leftd-1].DD = -data->leggA - data->leghA;
    data->CD[leftd-1].CC = MININT;
    for (i = 1; i <= M; i++) {
      if (i > N-up) rightd--;
      if (leftd > 1) leftd--;
      wa = data->w[A[i]];
      d = data->CD[leftd].DD;
      if ((ib = leftd+low-1+i) > 0) c = data->CD[leftd].CC+wa[B[ib]];
      f = data->CD[leftd].FF;
      if (d > c || ib <= 0) {
	c = d;
	data->CD[leftd].CP = data->CD[leftd].DPDP;
      }
      if(c < f) {
	c = f;
	data->CD[leftd].CP = data->CD[leftd].FP;
      }
      e = c - data->m;    
      data->IP = data->CD[leftd].CP;	    
      if(leftd > 1 && !legA) d -= data->leghA;
      else d -= data->zzh;
      data->CD[leftd-1].DD = d;
      data->CD[leftd-1].DPDP = data->CD[leftd].DPDP;
      if(f > c - data->g) {
	data->CD[leftd].FF -= data->rr;
	e = f - data->zzh;
	data->IP = data->CD[leftd].FP;
      } 
      else {
	data->CD[leftd].FF = c - data->g - data->rr;
	data->CD[leftd].FP = data->CD[leftd].CP;
      } 
      if (leftd == midd) 
	data->CD[leftd].CP = data->CD[leftd-1].DPDP = data->CD[leftd].FP =
	  data->IP = i;
      data->CD[leftd].CC = c;
      curd=leftd + 1;
      ap = &data->CD[curd];
      if(midd <= rightd && midd >= leftd + 1) x = midd;
      else x = rightd + 1;
      while(1) {
	for(; curd < x; curd++) {
	  c = ap->CC + wa[B[curd + low - 1 + i]];
	  d = ap->DD;
	  f = ap->FF;
	  if(d < e) {
	    if(e > c) {
	      if(e > f) {
		ap->CC = e;
		e -= data->zzh;
		ap->CP = data->IP;
		if(f > d) { 
		  (ap-1)->DD = f - data->zzh;
		  (ap-1)->DPDP = ap->FP;
		}
		else {
		  (ap-1)->DD = d - data->zzh;
		  (ap-1)->DPDP = ap->DPDP;
		}
	      }
	      else {
		ap->CC = f; 
		ap->CP = data->IP = (ap-1)->DPDP = ap->FP;
		e = (ap-1)->DD = f - data->zzh;
	      }
	      ap++->FF -= data->rr;
	      continue;
	    }
	  } else if (d > c) {
	    if (d > f) {
	      ap->CC = d; (ap-1)->DPDP = ap->CP = ap->DPDP;
	      (ap - 1)->DD = d - data->zzh; 
	      if(f > e) {
		e = f - data->zzh;
		data->IP = ap->FP;
	      }
	      else e -= data->zzh;
	    }
	    else {
	      ap->CC = f;
	      ap->CP = (ap - 1)->DPDP = data->IP = ap->FP;
	      e = (ap - 1)->DD = f - data->zzh;
	    }
	    ap++->FF -= data->rr;
	    continue;
	  }
	  if(c < f) {
	    ap->CC = f; ap->CP = (ap - 1)->DPDP = data->IP = ap->FP;
	    e = (ap - 1)->DD = f - data->zzh;
	    ap++->FF -= data->rr;
	  }
	  else {
	    ap->CC = c;
	    if((c -= data->g) > f) {
	      if(c > e) {
		e = c - data->zzh;
		ap->FP = data->IP = ap->CP;
	      }
	      else {
		ap->FP = ap->CP;
		e -= data->zzh;
	      }
	      if(c > d) {
		(ap - 1)->DD = c - data->zzh;
		(ap - 1)->DPDP = ap->CP;
	      }
	      else {
		(ap - 1)->DD = d - data->zzh;
		(ap - 1)->DPDP = ap->DPDP;
	      }
	      ap++->FF = c - data->rr;
	    }
	    else {
	      if (f > e) {
		data->IP = ap->FP; e = f - data->zzh;
		if(f > d) {
		  (ap - 1)->DD = e;
		  (ap - 1)->DPDP = data->IP;
		} 
		else {
		  (ap - 1)->DD = d - data->zzh;
		  (ap - 1)->DPDP = ap->DPDP;
		}
	      }
	      else {
		e -= data->zzh;
		if (f > d) {
		  (ap - 1)->DD = f - data->zzh;
		  (ap - 1)->DPDP = ap->FP;  
		}
		else {
		  (ap-1)->DD = d - data->zzh;
		  (ap-1)->DPDP = ap->DPDP;
		}
	      }
	      ap++->FF -= data->rr;
	    }
	  }
	}
	if(x != rightd+1) {
	  c = ap->CC + wa[B[curd+low-1+i]];
	  d = ap->DD;
	  f = ap->FF;
	  data->MP[0][i] = i - 1;
	  data->MT[0][i] = 0;
	  if(e > c) {
	    c = e;
	    data->MP[0][i] = data->IP;
	    data->MT[0][i] = 2;
	  }
	  if(f > c) {
	    c = f;
	    data->MP[0][i] = i - 1;
	    data->MT[0][i] = 3;
	  }
	  if(d > c) {
	    c = d;
	    data->MP[0][i] = ap->DPDP;
	    data->MT[0][i] = 1;
	  }
	  ap->CC = c;
	  c -= data->g;
	  if(c > f) {
	    if(c > e) {
	      e = c - data->zzh;
	      data->MP[1][i] = data->MP[0][i];
	      data->MT[1][i] = data->MT[0][i];
	    }
	    else {
	      e -= data->zzh;
	      data->MP[1][i] = data->IP;
	      data->MT[1][i] = 2;
	    }
	    if(c > d) {
	      data->MP[2][i] = data->MP[0][i];
	      data->MT[2][i] = data->MT[0][i];
	      (ap-1)->DD = c - data->zzh;
	    }
	    else {
	      data->MP[2][i] = ap->DPDP;
	      data->MT[2][i] = 1;
	      (ap-1)->DD = d - data->zzh;
	    }
	    ap->FF = c - data->rr;
	    data->MP[3][i] = data->MP[0][i];
	    data->MT[3][i] = data->MT[0][i];
	  }
	  else {
	    if(e > f) {
	      e -= data->zzh;
	      data->MP[1][i] = data->IP;
	      data->MT[1][i] = 2;
	    }
	    else {
	      e = f - data->zzh;
	      data->MP[1][i] = i - 1;
	      data->MT[1][i] = 3;
	    }
	    if(d > f) {
	      data->MP[2][i] = ap->DPDP;
	      data->MT[2][i] = 1;
	      (ap-1)->DD = d - data->zzh;
	    }
	    else {
	      data->MP[2][i] = i - 1;
	      data->MT[2][i] = 3;
	      (ap-1)->DD = f - data->zzh;
	    }
	    ap->FF -= data->rr;
	    data->MP[3][i] = i - 1;
	    data->MT[3][i] = 3;
	  }
	  ap->CP = (ap - 1)->DPDP = data->IP = ap->FP = i; ap++;
	  x = rightd + 1; curd++;
	} else break;
      }
      if (!regA && i > N-up && 
	  best < (j = (ap - 1)->CC - data->reggA - (M - i) * data->reghA)) {
	best = j; X = i; Y = N; k = (ap-1)->CP;
      }
    }
    if((ap - 1)->CC > best) {
      best = (ap - 1)->CC;
      X = M;
      Y = N;
      k = (ap - 1)->CP;
    }
    if(!regB) {
      for(ap = &data->CD[leftd]; ap <= &data->CD[rightd - 1]; ap++) {
	if((j = ap->CC - data->reggB - data->reghB *
	    (x = (&data->CD[rightd] - ap))) > best) {
	  X = M;
	  best = j;
	  Y = N - x;
	  k = ap->CP;
	}
      }
    }

    /* decide which path to be traced back */
    if (regA && regB) {
      X = M; Y = N;
      v = c = data->CD[rightd].CC; d = data->CD[rightd-1].DD;
      if(te == 1 && d + data->m > c) {
	k = data->CD[rightd - 1].DPDP;
	l = 1;
      } else if(te == 2 && e + data->m > c) {
	k = data->IP;
	l = 2;
      }
      else {
	k = data->CD[rightd].CP;
	l = 0;
      }
    }
    else {
      l = 0; v = best;
      if(X != M || Y != N) regA = regB = 1;
    }
    if(rmid > Y - X) l = 2;
    else if(rmid < Y - X) l = 1;
  }
  /* Conquer: Solve subproblems recursively */
  /* trace back */
  r = -1;		
  FP = (Int4Ptr) MemGet(sizeof(Int4)*(M+1), MGET_ERRPOST);
  FT = MemGet(M+1, MGET_ERRPOST);
  for(; k > -1; r = k, k = data->MP[l][r], l = data->MT[l][r]){
    FP[k] = r;
    FT[k] = (Uint1) l;
  }
  /* forward dividing */
  if(r == -1) { /* optimal alignment did not cross the middle diagonal */
    if(rmid < 0)
      g_band4_align(A, B, X, Y, rmid + 1, up, tb, te, legA, legB, regA, regB, data);
    else
      g_band4_align(A, B, X, Y, low, rmid - 1, tb, te, legA, legB, regA, regB, data);
  }
  else {
    k = r;
    l = FP[k];
    kt = FT[k];

    /* first block */
    if (rmid < 0) {
      g_band4_align(A, B, r - 1, r + rmid, rmid + 1,
	    MIN(up, r + rmid), tb, 1, legA, legB, 1, 1, data);
      _DEL(1);
      legB = 1;
      if(data->sapp > data->sapp0 + 1) legA = 1;
    }
    else if (rmid > 0) {
      g_band4_align(A, B, r, r + rmid - 1,
	    MAX(-r, low), rmid - 1, tb, 2, legA, legB, 1, 1, data);
      _INS(1);
      legA = 1;
      if(data->sapp > data->sapp0 + 1) legB = 1;
    }
    /* intermediate blocks */
    t2 = up-rmid-1;
    t3 = low-rmid+1;
    for(; l > -1; k = l, l = FP[k], kt = FT[k]) {
      if(kt == 0) _REP
      else if(kt == 3) _REPP
      else if(kt == 1) { /* right-hand side triangle */
	_INS(1);
	t1 = l - k - 1;
	g_band4_align(A + k, B + k + rmid + 1, t1, t1, 0,
	      MIN(t1, t2), 2, 1, legA, legB, 1,1, data);
	_DEL(1);
      }
      else { /* kt == 2, left-hand side triangle */
	_DEL(1);
	t1 = l - k - 1;
	g_band4_align(A + k + 1, B + k + rmid, t1, t1,
	      MAX(-t1, t3), 0, 1, 2, legA, legB, 1, 1, data);
	_INS(1);
      }
      legA = legB = 1;
    }

    /* last block */
    if(Y - X > rmid) {
      _INS(1);
      t1 = k + rmid + 1;
      g_band4_align(A + k, B + t1, X - k, Y - t1, 0,
	    MIN(Y - t1, t2), 2, te, legA, legB, 1, 1, data);
    }
    else if(Y - X < rmid) {
      _DEL(1);
      t1 = X - (k + 1);
      g_band4_align(A + k + 1, B + k + rmid, t1, Y - (k + rmid),
	    MAX(-t1, t3), 0, 1, te, legA, legB, 1, 1, data);
    }
  }
  if(X != M) _DEL(M-X)
  else if(Y != N) _INS(N - Y);
  MemFree(FP);
  MemFree(FT);

  return(v);
}

/************************************************************/
static Int4 g_band4_align_small(Uint1Ptr A, Uint1Ptr B,
			Int4 M, Int4 N,
			Int4 low, Int4 up,
			Uint1 tb, Uint1 te,
			Uint1 leA, Uint1 leB,
			Uint1 reA, Uint1 reB,
			data_t *data)
{
  Int4 k, l, v;
  Int4 band, j;
  Int4 leftd, rightd;	/* for CC, DD, CP and DP */
  register Int4 curd;	/* current index for CC, DD CP and DP */
  register Int4 i;
  register Int4 c, d, e, x, f;
  register dp_ptr ap;
  Int4 t;
  Int4Ptr wa;
  Int1Ptr st;
  Uint1Ptr tmp;
  Int4 ib, best=MININT, X, Y;
  Int4 legga = data->leggA, leggb = data->leggB, regga = data->reggA,
    reggb = data->reggB, legha = data->leghA, leghb = data->leghB,
    regha = data->reghA, reghb = data->reghB;

  if(leA) {
    data->leggA = data->g;
    data->leghA = data->zzh;
  }
  if(leB) {
    data->leggB = data->g;
    data->leghB = data->zzh;
  }
  if(reA) {
    data->reggA = data->g;
    data->reghA = data->zzh;
  }
  if(reB) {
    data->reggB = data->g;
    data->reghB = data->zzh;
  }
  /* Boundary cases: M <= 0 , N <= 0, or up-low <= 0 */
  band = up - low + 1;

  /* Initialization */
  leftd = 1 - low;
  rightd = up - low + 1;

  data->CD[leftd].CC = 0;
  data->state[0][leftd] = -1;

  if(tb == 2) t = 0;
  else t = -data->leggB;
  data->CD[leftd].FF = -data->g - data->rr;
  for(j = leftd + 1; j <= rightd; j++) {
    data->CD[j].CC = t = t - data->leghB;
    data->CD[j].FF = t - data->g - data->rr;
    data->CD[j-1].DD = t - data->m;
    data->state[0][j] = 1;
  }
  for(j = 0; j < leftd; j++) data->CD[j].FF = MININT;
  data->CD[rightd+1].CC = MININT;
  data->CD[rightd].DD = MININT;
  if(tb == 1) data->CD[leftd - 1].DD = -data->leghA;
  else data->CD[leftd - 1].DD = -data->leghA - data->leggA;
  data->CD[leftd - 1].CC = MININT;
  for(i = 1; i <= M; i++) {
    if(i > N - up) rightd--;
    if(leftd > 1) leftd--;
    wa = data->w[A[i]];
    d = data->CD[leftd].DD;
    k = 0;
    f = data->CD[leftd].FF;
    if((ib = leftd + low - 1 + i) > 0) c = data->CD[leftd].CC + wa[B[ib]];
    if(d > c || ib <= 0) {
      c = d;
      k = 2;
    }
    if(c < data->CD[leftd].FF) {
      c= data->CD[leftd].FF;
      k = 3;
    }
    e = c - data->g;
    if(e < f) { 
      e = f - data->zzh; k += 25;
      data->CD[leftd].FF -= data->rr;
    }
    else {
      data->CD[leftd].FF = e - data->rr;
      e -= data->zzh;
    }
    if(ib <= 0) {
      data->CD[leftd - 1].DD = d - data->leghA;
      k += 30;
    }
    if(d < f) k = k%30 + 60;
    st = &data->state[i][leftd];
    *st++ = (Uint1) k;
    data->CD[leftd].CC = c;
    for (curd=leftd+1, ap = &data->CD[curd]; curd <= rightd; curd++) {
      c = ap->CC + wa[B[curd+low-1+i]];
      if((d=ap->DD) > e) {
	if(d > c) {
	  if(d < (f = ap->FF)) { 
	    ap->CC = f; e = (ap - 1)->DD = f - data->zzh;
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
	    (ap-1)->DD = d - data->zzh;
	    ap->CC = d;
	  }
	  ap++->FF -= data->rr;
	  continue;
	}
      }
      else if(e > c) {
	if(e < (f = ap->FF)) {
	  ap->CC = f;
	  e = (ap-1)->DD = f - data->zzh;
	  *st++ = 88;
	}
	else {
	  ap->CC = e; e -= data->zzh; 
	  if(d < f) {
	    (ap-1)->DD = f - data->zzh;
	    *st++ = 76;
	  }
	  else {
	    (ap - 1)->DD = d - data->zzh;
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
    if(!reA && i > N-up && best < (j = (ap - 1)->CC - data->reggA -
				   (M - i) * data->reghA)) {
      best = j;
      X = i;
      Y = rightd;
    }
  } 
  if((ap - 1)->CC > best) {
    best = (ap - 1)->CC;
    X = M;
    Y = rightd; 
  }
  if(!reB) 
    for(ap = &data->CD[leftd]; ap <= &data->CD[rightd]; ap++) {
      if((j = ap->CC - data->reggB - data->reghB *
	  (x = (&data->CD[rightd] - ap))) > best) {
	X = M;
	best = j;
	Y = rightd - x;
      }
    }
  v = best; l = 0;
  if (reA && reB) {
    X = M; Y = rightd;
    v = c = (ap-1)->CC; d = (ap-2)->DD;
    if (te == 1) {
      l = 2;
    } else if (te == 2) {
      l = 1;
    } 
  }
  tmp = MemGet(M+N, MGET_ERRPOST);
  for (i = X, j = Y, e=l, c = 0; i>=0; i--, c++) {
    k  = (t=data->state[i][j]) %5;
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

  MemFree(tmp);

  if (X != M) _DEL(M - X)
  else if (Y != rightd) _INS(rightd - Y);
  data->leggA = legga; data->leggB = leggb;
  data->reggA = regga; data->reggB = reggb;
  data->leghA = legha; data->leghB = leghb;
  data->reghA = regha; data->reghB = reghb;
  return(v);
}

/* g_band4_CHECK_SCORE - return the score of the alignment stored in S */

static Int4 g_band4_CHECK_SCORE(Uint1Ptr A, Uint1Ptr B,
			Int4 M, Int4 N,
			Int4 PNTR S, data_t *data) 
{ 
  register Int4  i, j, op;
  Int4 score;

  score = i = j = op = 0;
  if(*S < 0 && *S != MININT) {
    score -= data->leggA - *S * data->leghA;
    i = i - *S++;
  }
  else if((*S) >0) {
    score -= data->leggB + *S * data->leghB;
    j = j + *S++;
  }
  while(i < M || j < N) {
    op = *S++;
    if(op == 0) score = data->w[A[++i]][B[++j]] + score;
    else if(op == MININT) {
      i++;
      j++; 
      if(*(S - 2) == MININT) score -= data->rr;
      else score -= data->g + data->rr;
    }
    else if(op > 0) {
      score = score - (data->g + op * data->zzh);
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
  else if(op > 0) score += (data->g + op * data->zzh) -
		    (data->reggB + data->reghB * op);
  return(score);
}
