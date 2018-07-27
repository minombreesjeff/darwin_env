static char const rcsid[] = "$Id: bandalg2.c,v 6.5 2003/05/30 17:25:35 coulouri Exp $";

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

File name: bandalg2.c

Author: Gennadiy Savchuk, Jinqhui Zhang, Tom Madden

Contents: Functions to perform both local and global banded alignments.

*****************************************************************************/

#include <bandalgn.h>

static Int4 gb_linear_align(Uint1Ptr A, Uint1Ptr B,
			    Int4 M, Int4 N,
			    Int4 low, Int4 up,
			    Int1 tb, Int1 te,
			    Boolean legA, Boolean legB, 
			    Boolean regA, Boolean regB,
			    data_t *data);

static Int4 gb_linear_CHECK_SCORE(Uint1Ptr A, Uint1Ptr B,
				  Int4 M, Int4 N,
				  Int4 PNTR S,
				  Boolean lgA, Boolean lgB, 
				  Boolean rgA, Boolean rgB,
				  data_t *data);

/*********************************************************
*
*	Int4 gband_linear_gap(A, B, M, N, option, S, Slen)
*	compute the global alignment with flexible end gap 
*	penalty for DNA-DNA and protein protein alignment. 
*	The alignment is computed with linear space
*	This function was originally from the g_band2.c file
*
*	align the two sequences A, B
*	A, B starts with index 1
*	M, N is the length of A, B
*	option sets the option of the alignment, 
*	which includes penalties for end gaps
*	S is the script that contains the alignment results
*	Slen stores the length of the alignment (the size of S)
*	return the score of the alignment
*
***********************************************************/
Int4 LIBCALL gband_linear_gap(Uint1Ptr A, Uint1Ptr B,
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
	
  /* set up global parameter */
	data.g = option->gopen;
	data.zzh = option->gext;
	data.w = matrix;
	data.m = data.g + data.zzh;
	data.sapp = data.sapp0 = S;
	data.last = 0;
	*Slen = 0;
	
	low = option->start_diag;	/* start diagonal of band */
	band = option->width;		/* the band width for the alignment */
	/* band = up-low+1 */
	up = band + low - 1;
	low = MIN(MAX(-M, low), MIN(N - M, 0));
	up = MAX(MIN(N, up), MAX(N - M, 0));
	
	if (up < 0 || low > 0) {
#ifdef DO_WWW
			content();
			printf("The band does not include (0,0) grid point"); 
				exit (1);
#else
		ErrPostEx(SEV_WARNING, 0, 0, "The band does not include (0,0) grid point");
		return 0;
#endif
	}
	if (up + M < N || low + M > N) {
#ifdef DO_WWW
			content();
			printf("The band does not include lower corner"); 
				exit (1);
#else
		ErrPostEx(SEV_WARNING, 0, 0, "The band does not include lower corner");
		return 0;
#endif
	}
	if (N <= 0) {
		if (M > 0)
			DEL_(M);
		return -gap_(M);
	}
	if (M <= 0) {
		INS_(N);
		return -gap_(N);
	}
	if ((band = up - low + 1) <= 1) {
		c = 0;
		for (i = 1; i <= M; i++) {
			REP_;
			c += data.w[A[i]][B[i]];
		}
		return c;
	}
	data.CD = MemGet((size_t) (band + 2) * sizeof(dp_node), MGET_ERRPOST);
	j = M + 1;
	data.MT[0] = MemGet((size_t) j, MGET_ERRPOST);
	data.MT[1] = MemGet((size_t) j, MGET_ERRPOST);
	data.MT[2] = MemGet((size_t) j, MGET_ERRPOST);
	data.FT = MemGet((size_t) j, MGET_ERRPOST);
	j *= sizeof(Int4);
	data.MP[0] = MemGet((size_t) j, MGET_ERRPOST);
	data.MP[1] = MemGet((size_t) j, MGET_ERRPOST);
	data.MP[2] = MemGet((size_t) j, MGET_ERRPOST);
	data.FP = MemGet((size_t) j, MGET_ERRPOST);

  /*Initiate the end gap penalties */
	data.leggA = option->lg1_open;
	data.leghA = option->lg1_ext;
	data.reggA = option->rg1_open;
	data.reghA = option->rg1_ext;
	
	data.leggB = option->lg2_open;
	data.leghB = option->lg2_ext;
	data.reggB = option->rg2_open;
	data.reghB = option->rg2_ext;
	
	c = gb_linear_align(A, B, M, N, low, up, 0, 0, 0, 0, 0, 0, &data);
	score = gb_linear_CHECK_SCORE(A, B, M, N, S, 0, 0, 0, 0, &data);
	if (score != c) {
#ifdef DO_WWW
			content();
			printf("Check_Score = %ld align_score = %ld ", score, c); 
				exit (1);
#else
		ErrPostEx(SEV_WARNING, 0, 0, "Check_Score = %ld align_score = %ld ",
			  (long) score, (long) c);
		return 0;
#endif
	}
	MemFree(data.CD);
	for (i = 0; i < 3; i++) {
		MemFree(data.MT[i]);
		MemFree(data.MP[i]);
	}
	
	MemFree(data.FT);
	MemFree(data.FP);
	
	*Slen = data.sapp - S;
	return c;
}

/* align(A, B, M, N, up, low, tb, te, data) returns the cost
   of an optimum conversion between
   A[1..M] and B[1..N] and appends such a conversion to the current script.
   tb(te)= 1  no gap-open penalty if the conversion begins(ends) with a delete.
   tb(te)= 2  no gap-open penalty if the conversion begins(ends) with an
   insert.
*/

/************************************************************/
static Int4 gb_linear_align(Uint1Ptr A, Uint1Ptr B,
			    Int4 M, Int4 N,
			    Int4 low, Int4 up,
			    Int1 tb, Int1 te,
			    Boolean legA, Boolean legB, 
			    Boolean regA, Boolean regB,
			    data_t *data)
{
  Int4 rmid, k =0, l=0, r, v, kt, X, Y;
  Int4 t1, t2, t3;
  Int4Ptr PNTR dmp = data->MP;
  Uint1Ptr PNTR dmt = data->MT;
  register Uint1Ptr BB;
  register Uint1 AA;
  register Int4 match, misma;
  {
    Int4 band, midd, j;
    Int4 leftd, rightd;	/* for CC, DD, CP and DP */
    register Int4 curd;	/* current index for CC, DD CP and DP */
    register Int4 i;
    register Int4 c, d, e, x;
    register dp_ptr ap;
    Int4 t, fr, tz = data->zzh, tm = data->m;
    Int4Ptr wa;
    Int4 ib, best = MININT;

    /* Boundary cases: M <= 0 , N <= 0, or up-low <= 0 */
	if (up > N) {
		up = N;
	}
	if (N <= 0) {
		if (M > 0) {
			_DEL(M);
		}
		return 0;
	}
	if (M <= 0) {
		_INS(N);
		return 0;
	}
	if ((band = up - low + 1) <= 1) {
		for (i = 1; i <= M; i++) {
			_REP;
		}
		return 0;
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
			data->CD[j].CP = data->CD[j].DPDP = -1;
		for (j = midd; j <= rightd; j++) {
			data->CD[j].CP = data->CD[j].DPDP = 0;
		}
		data->CD[midd - 1].DPDP = 0;
		dmp[0][0] = -1;
		dmp[1][0] = -1;
		dmp[2][0] = -1;
		dmt[0][0] = dmt[1][0] = dmt[2][0] = 0;
	} else if (leftd > midd) {
		fr = leftd - midd;
		for (j = 0; j <= midd; j++) {
			data->CD[j].CP = data->CD[j].DPDP = fr;
		}
		for (j = midd + 1; j <= rightd; j++)
			data->CD[j].CP = data->CD[j].DPDP = -1;
		data->CD[midd].DPDP = -1;
		dmp[0][fr] = -1;
		dmp[1][fr] = -1;
		dmp[2][fr] = -1;
		dmt[0][fr] = dmt[1][fr] = dmt[2][fr] = 0;
	} else {
		for (j = 0; j < rightd; j++) {
			data->CD[j].CP = data->CD[j].DPDP = 0;
		}
		dmp[0][0] = -1;
		dmp[1][0] = -1;
		dmp[2][0] = -1;
		dmt[0][0] = dmt[1][0] = dmt[2][0] = 0;
	}

	data->CD[leftd].CC = 0;
	if (!legB) {
		t = -data->leggB;
		for (j = leftd + 1; j <= rightd; j++) {
			data->CD[j].CC = t = t - data->leghB;
			data->CD[j - 1].DD = t - data->m;
		}
	} else {
		if (tb == 2) {
			t = 0;
		} else {
			t = -data->g;
		}
		for (j = leftd + 1; j <= rightd; j++) {
			data->CD[j].CC = t = t - tz;
			data->CD[j - 1].DD = t - tm;
		}
	}
	data->CD[rightd + 1].CC = MININT;
	data->CD[rightd].DD = MININT;
	data->CD[rightd + 1].CP = data->CD[rightd].DPDP = 0;
	if (tb == 1) {
		data->CD[leftd - 1].DD = -tz;
	} else {
		data->CD[leftd - 1].DD = -tm;
	}
	if (!legA) {
		data->CD[leftd - 1].DD = -data->leggA - data->leghA;
	}
	data->CD[leftd - 1].CC = MININT;
	match = data->w[0][0];
	misma = data->w[0][1];
    for (i = 1; i <= M; i++) {
		if (i > N - up) {
			rightd--;
		}
		if (leftd > 1) {
			leftd--;
		}
		wa = data->w[A[i]];
		AA = A[i];
		d = data->CD[leftd].DD;
		if ((ib = leftd + low - 1 + i) > 0) {
			c = data->CD[leftd].CC + wa[B[ib]];
		}
		if (d > c || ib <= 0) {
			c = d;
			data->CD[leftd].CP = data->CD[leftd].DPDP;
      	}
      	e = c - data->m;	    
      	data->IP = data->CD[leftd].CP;    
		if (leftd > 1 && !legA) {
			d -= data->leghA;
		} else {
			d -= tz;
		}
		if (d >= e) {
			data->CD[leftd - 1].DD = d;
			data->CD[leftd - 1].DPDP = data->CD[leftd].DPDP;
		} else {
			data->CD[leftd - 1].DD = e;
			data->CD[leftd - 1].DPDP = data->IP;
		}
		if (leftd == midd) {
			data->CD[leftd].CP = data->CD[leftd - 1].DPDP = data->IP = i;
		}
		data->CD[leftd].CC = c;
		if (midd <= rightd && midd >= leftd + 1) {
			x = midd;
		} else {
			x = rightd + 1;
		}
		BB = B + low - 1 + i;
		for (curd = leftd + 1, ap = &data->CD[curd]; curd < x; curd++) {
			c = ap->CC + wa[BB[curd]];
			d = ap->DD;
			if (c < d || c < e) {
				if (d < e) {
					c = e;
					ap->CP = data->IP;
				} else {
					c = d;
					ap->CP = ap->DPDP;
				}
			}
			ap->CC = c;
			if ((c -= tm) > (e -= tz)) {
	  			e = c;
	  			data->IP = ap->CP;
			} 
			if (c > (d -= tz)) {
	  			(ap - 1)->DD = c;
	  			(ap - 1)->DPDP = ap->CP;
	 			 ap++;
			} else {
	  			(ap - 1)->DD = d;
	  			(ap - 1)->DPDP = ap->DPDP;
	  			ap++;
			}
		}
		if (x != rightd+1) {
			dmp[1][i] = data->IP;
			dmt[1][i] = 2;
			dmp[2][i] = ap->DPDP;
			dmt[2][i] = 1;	   
			c = ap->CC + wa[B[curd+low-1+i]];
			d = ap->DD;
			if (c < d || c < e) {
	  			if (e > d) {
	    			c = e;
	    			dmp[0][i] = dmp[1][i];
	    			dmt[0][i] = 2;
	  			} else {
					c = d;
					dmp[0][i] = dmp[2][i];
	    			dmt[0][i] = 1;
	  			}
			} else {
	  			dmp[0][i] = i-1;
	  			dmt[0][i] = 0;
			}
			if (c - data->g > e) {
		  		dmp[1][i] = dmp[0][i];
		  		dmt[1][i] = dmt[0][i];
			}
			if (c - data->g > d) {
		  		dmp[2][i] = dmp[0][i];
		  		dmt[2][i] = dmt[0][i];
			}
			ap->CP = (ap - 1)->DPDP = data->IP = i;
			ap->CC = c;
			if ((c -= data->m) > (e -= tz)) {
				e = c;
			}
			if (c > (d -= tz)) {
				(ap++ - 1)->DD = c;
			} else {
				(ap++ - 1)->DD = d;
			}
			for (curd++; curd <= rightd; curd++) {
	  			c = ap->CC + wa[BB[curd]];
	  			d = ap->DD;
	  			if (c < d || c < e) { 
	    			if (d < e) {
	      				c = e;
	      				ap->CP  = data->IP;
	    			} else {
	      				c = d;
	      				ap->CP = ap->DPDP;
	    			}
	  			}   /* otherwise, CP is unchanged */
	  			ap->CC = c;
	  			if ((c -= tm) > (e -= tz)) {
	    			e = c;
	    			data->IP = ap->CP;
	  			} 
	  			if (c > (d -= tz)) {
	    			(ap - 1)->DD = c;
	    			(ap-1)->DPDP = ap->CP;
	    			ap++;
	  			} else {
	    			(ap-1)->DD = d;
	    			(ap-1)->DPDP = ap->DPDP;
	    			ap++;
	  			}
			}
		}
		if (!regA && i > N-up && 
	 	  best < (j = (ap - 1)->CC - data->reggA - (M - i) * data->reghA)) {
			best = j; 
			X = i; 
			Y = N; 
			k = (ap-1)->CP;
      	}
	}
	if (!regB) {
		for (ap = &data->CD[leftd]; ap <= &data->CD[rightd]; ap++) {
			if ((j = ap->CC - data->reggB - data->reghB *
			     (x = (&data->CD[rightd] - ap))) > best) {
				X = M;
				best = j;
				Y = N - x;
				k = ap->CP;
			}
		}
	}
	if (best < data->CD[rightd].CC) {
		X = M;
		Y = N;
		best = data->CD[rightd].CC;
		k = data->CD[rightd].CP;
	}

    /* decide which path to be traced back */
	if (regA && regB) {
		X = M; 
		Y = N;
		v = c = (ap-1)->CC; 
		d = (ap-2)->DD;
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
	} else {
		l = 0;
		v = best;
    }
	if (rmid > Y - X) {
		l = 2;
	} else if (rmid < Y - X) {
		l = 1;
	}
  }
/* Conquer: Solve subproblems recursively */
/* trace back */
	r = -1;	
	for (; k > -1; r=k, k=dmp[l][r], l=dmt[l][r]) {
		data->FP[k] = r;
    	data->FT[k] = (Uint1) l;
  	}
/* forward dividing */
	if(r == -1) { /* optimal alignment did not cross the middle diagonal */
		if(rmid < 0) {
			gb_linear_align(A, B, X, Y, rmid + 1, up, tb, te,
		      legA, legB, regA, regB, data);
		} else {
			gb_linear_align(A, B, X, Y, low, rmid-1, tb, te,
		      legA, legB, regA, regB, data);
		}		  
	} else {
		k = r;
		l = data->FP[k];
    	kt = data->FT[k];

/* first block */
    	if (rmid < 0) {
      		gb_linear_align(A, B, r - 1, r + rmid, rmid + 1,
		      MIN(up, r + rmid), tb, 1, legA, legB, 1, 1, data);
      		_DEL(1);
      		legB = 1;
      		if (data->sapp > data->sapp0 + 1) {
      			legA = 1;
      		}
    	} else if (rmid > 0) {
      		gb_linear_align(A, B, r, r + rmid - 1, MAX(-r, low), rmid - 1,
		      	tb, 2, legA, legB, 1, 1, data);
      		_INS(1);
      		legA = 1;
      		if(data->sapp > data->sapp0 + 1) {
      			legB = 1;
      		}
		}
/* intermediate blocks */
		t2 = up-rmid-1;
		t3 = low-rmid+1;
		for (; l > -1; k = l, l = data->FP[k], kt = data->FT[k]) {
      		if (kt == 0) {
      			_REP;
      		} else if (kt == 1) { /* right-hand side triangle */
				_INS(1);
				t1 = l - k - 1;
				gb_linear_align(A + k, B + k + rmid + 1, 
						t1, t1, 0, MIN(t1, t2), 2, 1, legA, legB, 1, 1, data);
				_DEL(1);
      		} else { /* kt == 2, left-hand side triangle */
				_DEL(1);
				t1 = l - k - 1;
				gb_linear_align(A + k + 1, B + k + rmid, t1, t1,
						MAX(-t1, t3), 0, 1, 2, legA, legB, 1, 1, data);
				_INS(1);
      		}
      		legA = legB = 1;
    	}

/* last block */
		if (Y-X > rmid) {
      		_INS(1);
      		t1 = k + rmid + 1;
      		gb_linear_align(A + k, B + t1, X - k, Y - t1, 0, 
      				MIN(Y - t1, t2), 2, te, legA, legB, 1, 1, data);
    	} else if (Y-X < rmid) {
      		_DEL(1);
     		 t1 = X - (k + 1);
      		gb_linear_align(A + k + 1, B + k + rmid, t1, Y - (k + rmid),
		      MAX(-t1, t3), 0, 1, te, legA, legB, 1, 1, data);
    	}
  	}
	if(X != M) {
		_DEL(M - X)
	} else if(Y != N) {
  		_INS(N - Y);
	}
	return(v);
}

/************************************************************/
/* gb_linear_CHECK_SCORE - return the score of the alignment stored in S */
static Int4 gb_linear_CHECK_SCORE(Uint1Ptr A, Uint1Ptr B,
				  Int4 M, Int4 N,
				  Int4 PNTR S,
				  Boolean lgA, Boolean lgB, 
				  Boolean rgA, Boolean rgB,
				  data_t *data)
{
	register Int4   i,  j, op;
	Int4 score;

	score = i = j = op = 0;
	if (!lgA && (*S) < 0) {
		i = i-*S++;
	} else if (!lgB && (*S) >0) {
    	j = j+*S++;
	}
	while (i < M || j < N) {
    	op = *S++;
    	if (op == 0) {
      		score = data->w[A[++i]][B[++j]] + score;
    	} else if (op > 0) {
      		score = score - (data->g + op * data->zzh);
      		j = j + op;
    	} else {
      		score = score - (data->g - op * data->zzh);
      		i = i - op;
    	}
	if (i==M)
		break;
	}
	if (!rgA && op < 0) {
		score += (data->g - op * data->zzh);
	}
	if (!rgB && op > 0) {
		score += (data->g + op * data->zzh);
	}
	return (score);
}
