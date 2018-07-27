static char const rcsid[] = "$Id: bandalg0.c,v 6.3 2003/05/30 17:25:35 coulouri Exp $";

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

File name: bandalgn0.c

Author: Gennadiy Savchuk, Jinqhui Zhang, Tom Madden

Contents: Functions to perform global linear alignments.

****************************************************************************/

#include "bandalgn.h"

static Int4 g_band0_CHECK_SCORE(Uint1Ptr A, Uint1Ptr B,
				Int4 M, Int4 N,
				Int4 PNTR S,
				data_t *data);

static Int4 g_band0_align(Uint1Ptr A, Uint1Ptr B,
			  Int4 M, Int4 N,
			  Int4 low, Int4 up,
			  Uint1 tb, Uint1 te,
			  data_t *data);

Int4 LIBCALL gband_linear(Uint1Ptr Seq1, Uint1Ptr Seq2,
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
  
  low = MIN(MAX(-M, low), MIN(N - M, 0));
  up = MAX(MIN(N, up), MAX(N - M, 0));
  
  if (up < 0 || low > 0) {
    ErrPostEx(SEV_WARNING, 0, 0,
	      "The band does not include (0,0) grid point");
    return 0;
  } 
  if (up + M < N || low + M > N) {
    ErrPostEx(SEV_WARNING, 0, 0,
	      "The band does not include lower corner");
    return 0;
  }
  
  if (N <= 0) { 
    if (M > 0) DEL_(M)
    return -gap_(M);
  }
  if (M <= 0) {
    INS_(N)
    return -gap_(N);
  }
  if ((band = up - low + 1) <= 1) {
    c = 0;
    for (i = 1; i <= M; i++) {
      REP_;
      c += data.w[Seq1[i]][Seq2[i]];
    }
    return c;
  }
  
  j = (band + 2) * sizeof(Int4);
  
  data.CD = (dp_ptr) MemGet(sizeof(dp_node) * (band + 2), MGET_ERRPOST);
  j = M + 1;
  data.MT[0] = MemGet(j, MGET_ERRPOST);
  data.MT[1] = MemGet(j, MGET_ERRPOST);
  data.MT[2] = MemGet(j, MGET_ERRPOST);
  data.FT = MemGet(j, MGET_ERRPOST);
  j *= sizeof(Int4);
  data.MP[0] = (Int4Ptr)MemGet(j, MGET_ERRPOST);
  data.MP[1] = (Int4Ptr)MemGet(j, MGET_ERRPOST);
  data.MP[2] = (Int4Ptr)MemGet(j, MGET_ERRPOST);
  data.FP = (Int4Ptr)MemGet(j, MGET_ERRPOST);
  
  c = g_band0_align(Seq1, Seq2, M, N, low, up, 0, 0, &data);
  score = g_band0_CHECK_SCORE(Seq1, Seq2, M, N, S, &data);
  
  MemFree(data.CD);
  MemFree(data.FT);
  MemFree(data.FP);

  for(j = 0; j < 3; j++) {
    MemFree(data.MT[j]);
    MemFree(data.MP[j]);
  }
  
  if(score != c) {
    ErrPostEx(SEV_WARNING, 0, 0, "Check_Score = %ld align_score = %ld ", 
	      (long) score, (long) c);
	return 0;
  }
  
  *Slen = data.sapp - S;

  return c;
}

/* g_band0_CHECK_SCORE - return the score of the alignment stored in S */
static Int4 g_band0_CHECK_SCORE(Uint1Ptr A, Uint1Ptr B, 
				Int4 M, Int4 N,
				Int4 PNTR S,
				data_t *data)
{ 
  register Int4 i, j, op;
  Int4 score = i = j = op = 0;

  while (i < M || j < N) {
    op = *S++;
    if (op == 0) 
      score = data->w[A[++i]][B[++j]] + score;
    else if (op > 0) {
      score = score - (data->g + op * data->zzh);
      j = j + op;
    }
    else {
      score = score - (data->g - op * data->zzh);
      i = i - op;
    }
  }
  return(score);
}

/************************************************************/
/* g_band0_align(A,B,M,N,up,low,tb,te, data) returns the cost of an
   optimum conversion between
   A[1..M] and B[1..N] and appends such a conversion to the current script.
   tb(te)= 1  no gap-open penalty if the conversion begins(ends) with a delete.
   tb(te)= 2  no gap-open penalty if the conversion begins(ends) with an insert.
*/

static Int4 g_band0_align(Uint1Ptr A, Uint1Ptr B,
			  Int4 M, Int4 N,
			  Int4 low, Int4 up,
			  Uint1 tb, Uint1 te,
			  data_t *data)
{
  Int4 rmid, k, l, r, v, kt;
  Int4 t1, t2, t3;
  {
    Int4 band, midd, j;
    Int4 leftd, rightd;	/* for CC, DD, CP and DP */
    register Int4 curd;	/* current index for CC, DD CP and DP */
    register Int4 i;
    register Int4 c, d, e, x;
    register dp_ptr ap;
    Int4 t, fr;
    Int4Ptr wa;
    Int4 ib;

    /* Boundary cases: M <= 0 , N <= 0, or up-low <= 0 */
    if (N <= 0) { 
      if (M > 0) _DEL(M)
      return 0;
    }
    if (M <= 0) {
      _INS(N)
      return 0;
    }
    if ((band = up - low + 1) <= 1) {
      for (i = 1; i <= M; i++) _REP
      return 0;
    }

    /* Divide: Find all crossing points */

    /* Initialization */
    midd = band/2 + 1;
    rmid = low + midd - 1;
    leftd = 1-low;
    rightd = up-low+1;
    if(leftd < midd) {
      fr = -1;
      for(j = 0; j < midd; j++) 
	data->CD[j].CP = data->CD[j].DPDP = -1;
      for(j = midd; j <= rightd; j++)
	data->CD[j].CP = data->CD[j].DPDP = 0;
      data->MP[0][0] = -1;
      data->MP[1][0] = -1;
      data->MP[2][0] = -1;
    }
    else if (leftd > midd) {
      fr = leftd-midd;
      for (j = 0; j <= midd; j++)
	data->CD[j].CP = data->CD[j].DPDP = fr;
      for (j = midd+1; j <= rightd; j++) 
	data->CD[j].CP = data->CD[j].DPDP = -1;
      data->MP[0][fr] = -1;
      data->MP[1][fr] = -1;
      data->MP[2][fr] = -1;
    }
    else {
      fr = 0;
      for (j = 0; j < midd; j++)
	data->CD[j].CP = data->CD[j].DPDP = 0;
      for (j = midd; j <= rightd; j++)
	data->CD[j].CP = data->CD[j].DPDP = 0;
      data->MP[0][0] = -1;
      data->MP[1][0] = -1;
      data->MP[2][0] = -1;
    }

    data->CD[leftd].CC = 0;
    if(tb == 2) t = 0;
    else t = -data->g;
    for(j = leftd+1; j <= rightd; j++) {
      data->CD[j].CC = t = t - data->zzh;
      data->CD[j-1].DD = t - data->m;
    }
    data->CD[rightd+1].CC = MININT;
    data->CD[rightd].DD = MININT;
    data->CD[rightd+1].CP = data->CD[rightd].DPDP = 0;
    if(tb == 1) data->CD[leftd-1].DD = -data->zzh;
    else data->CD[leftd-1].DD = -data->m;
    data->CD[leftd-1].CC = MININT;
    for(i = 1; i <= M; i++) {
      if(i > N-up) rightd--;
      if(leftd > 1) leftd--;
      wa = data->w[A[i]];
      d = data->CD[leftd].DD;
      if((ib = leftd + low - 1 + i) > 0) c = data->CD[leftd].CC + wa[B[ib]];
      if(d > c || ib <= 0) {
	c = d;
	data->CD[leftd].CP = data->CD[leftd].DPDP;
      }
      e = c - data->m;
      data->IP = data->CD[leftd].CP;	    
      if(leftd == midd) data->CD[leftd].CP = data->CD[leftd].DPDP = data->IP = i;
      if(leftd >= 1) 
	if ((d -= data->zzh) >= e) {
	  data->CD[leftd-1].DD = d;
	  data->CD[leftd-1].DPDP = data->CD[leftd].DPDP;
	} else {
	  data->CD[leftd-1].DD = e;
	  data->CD[leftd-1].DPDP = data->IP;
	}
      data->CD[leftd].CC = c;
      if(midd <= rightd && midd >= leftd + 1) x = midd; else x = rightd + 1;
      for(curd = leftd + 1, ap = &data->CD[curd]; curd < x; curd++) {
	c = ap->CC + wa[B[curd + low - 1 + i]];
	d = ap->DD;
	if(c < d || c < e){ 
	  if(d < e) {
	    c = e;
	    ap->CP = data->IP;
	  }
	  else {
	    c = d;
	    ap->CP = ap->DPDP;
	  }
	}   /* otherwise, CP is unchanged */
	ap->CC = c;
	if((c -= data->m) > (e -= data->zzh)) {
	  e = c;
	  data->IP = ap->CP;
	}
	if(c > (d -= data->zzh)) {
	  (ap - 1)->DD = c;
	  (ap - 1)->DPDP = ap->CP;
	  ap++;
	}
	else {
	  (ap - 1)->DD = d;
	  (ap - 1)->DPDP = ap->DPDP;
	  ap++;
	}
      }
      if(x != rightd + 1) {
	data->MP[1][i] = data->IP;
	data->MT[1][i] = 2;
	data->MP[2][i] = ap->DPDP;
	data->MT[2][i] = 1;	   
	c = ap->CC + wa[B[curd + low - 1 + i]];
	d = ap->DD;
	if(c < d || c < e) {
	  if(e > d) {
	    c = e;
	    data->MP[0][i] = data->MP[1][i];
	    data->MT[0][i] = 2;
	  }
	  else {
	    c = d;
	    data->MP[0][i] = data->MP[2][i];
	    data->MT[0][i] = 1;
	  }
	}
	else {
	  data->MP[0][i] = i-1;
	  data->MT[0][i] = 0;
	}
	if(c - data->g > e) {
	  data->MP[1][i] = data->MP[0][i];
	  data->MT[1][i] = data->MT[0][i];
	}
	if(c - data->g > d) {
	  data->MP[2][i] = data->MP[0][i];
	  data->MT[2][i] = data->MT[0][i];
	}
	ap->CP = (ap - 1)->DPDP = data->IP = i;
	ap->CC = c;
	if((c -= data->m) > (e -= data->zzh)) e = c; 
	if(c > (d -= data->zzh)) (ap++ - 1)->DD = c;
	else (ap++ - 1)->DD = d;
	for(curd++; curd <= rightd; curd++) {
	  c = ap->CC + wa[B[curd+low-1+i]];
	  d = ap->DD;
	  if (c < d || c < e) { 
	    if (d < e) {
	      c = e;
	      ap->CP = data->IP;
	    } else {
	      c = d;
	      ap->CP = ap->DPDP;
	    }
	  }   /* otherwise, CP is unchanged */
	  ap->CC = c;
	  if((c -= data->m) > (e -= data->zzh)) {
	    e = c; data->IP = ap->CP;
	  } 
	  if (c > (d -= data->zzh)) {
	    (ap-1)->DD = c; (ap-1)->DPDP = ap->CP; ap++;
	  } else {
	    (ap-1)->DD = d; (ap-1)->DPDP = ap->DPDP; ap++;
	  }
	}
      }
    }

    /* decide which path to be traced back */
    c = (ap-1)->CC; d = (ap-2)->DD;
    if (te == 1 && d + data->m > c) {
      k = data->CD[rightd-1].DPDP;
      l = 2;
    } else if (te == 2 && e + data->m > c) {
      k = data->IP;
      l = 1;
    } else {
      k = data->CD[rightd].CP;
      l = 0;
    }
    if (rmid > N-M) l = 2;
    else if (rmid < N-M) l = 1;
    v = c;
  }
  /* Conquer: Solve subproblems recursively */

  /* trace back */
  r = -1;	
  for (; k > -1; r=k, k=data->MP[l][r], l=data->MT[l][r]){
    data->FP[k] = r;
    data->FT[k] = (Uint1) l;
  }
  /* forward dividing */
  if (r == -1) { /* optimal alignment did not cross the middle diagonal */
    if(rmid < 0) g_band0_align(A, B, M, N, rmid + 1, up, tb, te, data);
    else g_band0_align(A, B, M, N, low, rmid - 1, tb, te, data);
  } else {
    k = r;
    l = data->FP[k];
    kt = data->FT[k];

    /* first block */
    if (rmid < 0) {
      g_band0_align(A, B, r - 1, r + rmid, rmid + 1, MIN(up, r + rmid), tb, 1, data);
      _DEL(1)
    } else if (rmid > 0) {
      g_band0_align(A, B, r, r + rmid - 1, MAX(-r, low), rmid - 1, tb, 2, data);
      _INS(1)
    }

    /* intermediate blocks */
    t2 = up - rmid - 1;
    t3 = low - rmid + 1;
    for (; l > -1; k = l, l = data->FP[k], kt = data->FT[k]) {
      if (kt == 0) _REP
      else if (kt == 1) { /* right-hand side triangle */
	_INS(1)
	  t1 = l - k - 1;
	g_band0_align(A + k, B + k + rmid + 1, t1, t1, 0, MIN(t1, t2), 2, 1, data);
	_DEL(1)
      } else { /* kt == 2, left-hand side triangle */
	_DEL(1)
	  t1 = l - k - 1;
	g_band0_align(A + k + 1, B + k + rmid, t1, t1, MAX(-t1, t3), 0, 1, 2, data);
	_INS(1)
      }
    }

    /* last block */
    if (N - M > rmid) {
      _INS(1)
	t1 = k + rmid + 1;
      g_band0_align(A + k, B + t1, M - k, N - t1, 0, MIN(N - t1, t2), 2, te, data);
    } else if (N - M < rmid) {
      _DEL(1)
	t1 = M -(k + 1);
      g_band0_align(A + k + 1, B + k + rmid, t1, N - (k + rmid),
	    MAX(-t1, t3), 0, 1, te, data);
    }
  }
  return(v);
}
