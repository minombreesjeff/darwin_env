static char const rcsid[] = "$Id: g_any.c,v 6.5 2003/05/30 17:25:36 coulouri Exp $";

/*  g_any.c
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
* File Name:  g_any.c
*
* Author:  Jinghui Zhang and Kun-Mao Chao
*
* Version Creation Date: 5/24/95
*
*
* File Description:   
* A PACKAGE FOR GLOBALLY ALIGNING TWO SEQUENCES WITHIN AN ARBITRARY REGION:
*
*  To invoke, call z_ALIGN(,B,M,N,L,R,V,G,E,S).
*   The parameters are explained as follows:
*	A, B : two sequences to be aligned
*	M : the length of sequence A
*	N : the length of sequence B
*	L : left boundary
*	R : right boundary
*	V : scoring table for matches and mismatches
*	G : gap-opening penalty
*	E : gap-extension penalty
*	S : script for DISPLAY routine
*
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
*
* $Log: g_any.c,v $
* Revision 6.5  2003/05/30 17:25:36  coulouri
* add rcsid
*
* Revision 6.4  1998/08/24 21:19:38  kans
* fixed -v -fd warnings
*
* Revision 6.3  1998/07/07 20:20:13  kans
* corrected initial value
*
* Revision 6.2  1998/07/07 17:12:11  kans
* fixed my bug, restoring pmr as previous value of mr, and trying a reasonable first value of mr to get around uninitialized variable warning
*
* Revision 6.1  1998/05/28 18:03:51  kans
* switched lines to initialize mr before using
*
* Revision 6.0  1997/08/25 18:53:11  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:43:15  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:50:15  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/05/25  20:23:06  zjing
 * add header
 *
*
*
* ==========================================================================
*/


#include <ncbi.h>

#define MININT -999999
#define Max(x,y)  ((x) >= (y) ? (x) : (y))
#define Min(x,y)  ((x) <= (y) ? (x) : (y))
#define gap(k)  ((k) <= 0 ? 0 : (g+e*(k)))      /* k-symbol indel cost */

						/* Append "Delete k" op */
#define DEL(k)                          \
{                                       \
  if (last < 0)                         \
    last = sapp[-1] -= (k);             \
  else                                  \
    last = *sapp++ = -(k);              \
}
						/* Append "Insert k" op */
#define INS(k)                          \
{                                       \
  if (last > 0)                         \
    last = sapp[-1] += (k);             \
  else                                  \
    last = *sapp++ = (k);               \
}

						/* Append "Replace" op */
#define REP                             \
{ last = *sapp++ = 0;                   \
}

static Int4Ptr SS;
static Int4Ptr DD;
static Int4 I;
static Int4Ptr SP;
static Int4Ptr DP;
static Int4 IP;
static CharPtr ST, DT;
static Char IT;
static Int4Ptr PP[3];
static CharPtr PT[3];
static Int4Ptr RI;                      /* map back diagonal into its row index */
static Int4Ptr PNTR vv;                          /* vv = V */
static Int4 g, e, m;                            /* g = G, e = E, m = g+e */
static Int4Ptr sapp;                            /* Current script append ptr */
static Int4  last;                              /* Last script op appended */
CharPtr A, B;
Int4Ptr L, R;

Int4 z_ALIGN(CharPtr, CharPtr, Int4, Int4, Int4, Int4, Int4Ptr, Int4Ptr, Int4Ptr PNTR, Int4, Int4, Int4Ptr);
void DISPLAY(CharPtr, CharPtr, Int4, Int4, Int4Ptr, Int4, Int4);
static Int4 CHECK_SCORE(CharPtr, CharPtr, Int4, Int4, Int4Ptr);

static Int4 k_align(Int4 I1, Int4 J1, Char Type1, Int4 I2, Int4 J2, Char Type2)
{
	Int4 i, j, l, r, b, b1, b2;
	Int4 i1, j1, i2, j2;
	Char t1, t2;
	Int4 cmid, nmid, pmid, mr, pmr;
	Int4 s, sp, c, cp, d, dp, ti;
	Char st, ct, dt;
	Int4Ptr va;
	Char flag, rflag, dflag, sflag;
	Int4 l_save, r_save;

/*
TRACE("I1= %ld, J1= %ld, Type1 = %ld, I2= %ld, J2= %ld, Type2= %ld\n",I1,J1,Type1,I2,J2,Type2);
for (i=I1; i<= I2; ++i) TRACE("%ld : %ld %ld\n",i,L[i],R[i]);
*/
	/* Initializations */
	if (Type2 == 0) SS[J2] = DD[J2] = I = 0;
	else if (Type2 == 1) {
		DD[J2] = 0;
		I = SS[J2] = MININT;
	} else {
		I = 0;
		DD[J2] = SS[J2] = MININT;
	}
	RI[I2+J2] = I2;
	cmid = J2;
	if (I1 < I2) nmid = (L[I2-1] + R[I2-1] + 1)/2;
	else nmid = J1;
	l = L[I2];
	for (j = J2-1; j >= l; --j) {
		I = I - e;
		DD[j] = SS[j] = I - g;
		if (j+1 >= nmid) {  /* (I2,j+1) is a partition point */
			IP = SP[j] = I2+j+1;
			IT = ST[j] = 2;
		} else {
			IP = SP[j] = IP;
			IT = ST[j] = IT;
		}
		if (j >= nmid) {           /* (I2,j) is a partition point */
			b = I2+j;
			RI[b] = I2;
			PP[2][b] = PP[0][b] = IP;
			PT[2][b] = PT[0][b] = IT;
			PP[1][b] = DP[j] = b;
			PT[1][b] = DT[j] = 0;
		} else {
			DP[j] = SP[j];
			DT[j] = ST[j];
		}

	}
	c = SS[l];
	SS[J2+1] = MININT;
	for (j = l-1; j >= J1; --j)
		SS[j] = DD[j] = MININT;
	/*mr = 0;*/		/*set up the initiate value?? jz*/
	mr = Min(L[I2], R[I2-1]); /* this will be the initial value */
	for (i = I2-1; i >= I1; --i) {
		pmr = mr; /* previous value of mr */
		mr = Min(L[i+1], R[i]);
		pmid = cmid;
		cmid = nmid;
		if (i > I1) nmid = (L[i-1]+R[i-1] + 1)/2;
		else nmid = J1;
		l = L[i];
		r = R[i];
		I = MININT;
		s = SS[r+1];
		sp = SP[r+1];
		st = ST[r+1];
		va = vv[A[i+1]];
		flag = 0;
		if ((r+1>=cmid && r+1<=pmid) ||
		    (i+1<I2 && r+1>pmid && r+1<=pmr)) dflag = 1;
		else dflag = 0;
		for (j = r; j >= l; --j) {
			rflag = flag;
			sflag = dflag;
			if ((j >= nmid && j <= cmid) ||
			    (i < I2 && j > cmid && j <= mr)) {
				flag = 1;
				b = i + j;
				RI[b] = i;
			} else flag = 0;
			if ((j >= cmid && j <= pmid) ||
			    (i+1<I2 && j>pmid && j<=pmr)) dflag = 1;
			else dflag = 0;
			if (j < J2) c = s + va[B[j+1]];
			else c = MININT;
			d = DD[j] - m;
			ti = I - m;
			if (c >= Max(d,ti)) {
				if (sflag) {
					cp = i+j+2;
					ct = 0;
				} else {
					cp = sp;
					ct = st;
				}
			} else if (d >= ti) {
				c = d;
				if (dflag) {
					cp = i+j+1;
					ct = 1;
				} else {
					cp = DP[j];
					ct = DT[j];
				}
			} else {
				c = ti;
				if (rflag) {
					cp = i+j+1;
					ct = 2;
				} else {
					cp = IP;
					ct = IT;
				}
			}
			if (c >= (d = DD[j]-e)) {
				d = c;
				if (flag) {
					dp = i+j;
					dt = 0;
				} else {
					dp = cp;
					dt = ct;
				}
			} else {
				if (dflag) {
					dp = i+j+1;
					dt = 1;
				} else {
					dp = DP[j];
					dt = DT[j];
				}
			}
			if (c >= (I = I-e)) {
				I = c;
				if (flag) {
					IP = i+j;
					IT = 0;
				} else {
					IP = cp;
					IT = ct;
				}
			} else {
				if (rflag) {
					IP = i+j+1;
					IT = 2;
				}
			}
			s = SS[j];
			sp = SP[j];
			st = ST[j];
			SS[j] = c;
			SP[j] = cp;
			ST[j] = ct;
			DD[j] = d;
			DP[j] = dp;
			DT[j] = dt;
			if (flag) {  /* save information in partition nodes */
				PP[0][b] = cp;
				PT[0][b] = ct;
				PP[1][b] = dp;
				PT[1][b] = dt;
				PP[2][b] = IP;
				PT[2][b] = IT;
			}
		}
		SS[r+1] = MININT;
	}
	b = I2 + J2;
	i1 = I1;
	j1 = J1;
	t1 = Type1;
	b1 = I1 + J1;
	while (b1 != b) {
		b2 = PP[t1][b1];
		i2 = RI[b2];
		j2 = b2 - i2;
		t2 = PT[t1][b1];
		if (i1 == i2) { if (j2 > j1) INS(1) }
		else if (j1 == j2) DEL(1)
		else if (i1+1 == i2 && j1+1 == j2) {
			if (t2 == 1) { INS(1) DEL(1)}
			else if (t2 == 2) { DEL(1) INS(1) }
			else REP
		} else {
			l_save = L[i2];
			r_save = R[i2];
			if (j1 < (L[i1]+R[i1] + 1)/2) {
				for (i = i2; i > i1; --i) {
					nmid = (L[i-1]+R[i-1] + 1)/2;
					L[i] = Max(j1, L[i]);
					R[i] = nmid-1;
				}
				L[i1] = j1;
				R[i1] = j1;
				R[i2] = j2;
			} else {
				for (i = i1; i < i2; ++i) {
					cmid = (L[i]+R[i] + 1)/2;
					L[i] = Max(cmid,L[i+1])+1;
					R[i] = Min(j2, R[i]);
				}
				L[i1] = j1;
				L[i2] = j2;
				R[i2] = j2;
			}
			k_align(i1,j1,t1,i2,j2,t2);
			L[i2] = l_save;
			R[i2] = r_save;
		}
		i1 = i2;
		j1 = j2;
		t1 = t2;
		b1 = b2;
	}
	return c;
}

Int4 z_ALIGN(Char Seq1[], Char Seq2[], Int4 bi, Int4 bj, Int4 ei, Int4 ej, Int4 lb[], Int4 rb[], Int4Ptr PNTR V, Int4 G, Int4 E, Int4 S[])
{ 
	Int4 c;
	Int4 j;
	Int4 check_score;
	CharPtr ckalloc(Int4);
	Int4 M, N;

	A = Seq1;
	B = Seq2;
	L = lb;
	R = rb;
	vv = V;                  /* Setup global parameters */
	g = G;
	e = E;
	m = g+e;
	sapp = S;
	last = 0;
	M = ei - bi;
	N = ej - bj;

	j = (N+2) * sizeof(Int4);
	SS = ((Int4 *) ckalloc(j)) - bj;
	DD = ((Int4 *) ckalloc(j)) - bj;
	j = (N+2) * sizeof(Int4);
	SP = ((Int4 *) ckalloc(j)) - bj;
	DP = ((Int4 *) ckalloc(j)) - bj;
	j = (N+2) * sizeof(Char);
	ST = ((Char *) ckalloc(j)) - bj;
	DT = ((Char *) ckalloc(j)) - bj;
	j = (M+N+1) * sizeof(Int4);
	PP[0] = (Int4 *) ckalloc(j) - bi - bj;
	PP[1] = (Int4 *) ckalloc(j) - bi - bj;
	PP[2] = (Int4 *) ckalloc(j) - bi - bj;
	RI = (Int4 *) ckalloc(j) - bi - bj;
	j = (M+N+1) * sizeof(Char);
	PT[0] = (Char *) ckalloc(j) - bi - bj;
	PT[1] = (Char *) ckalloc(j) - bi - bj;
	PT[2] = (Char *) ckalloc(j) - bi - bj;

	c = k_align(bi,bj,0,ei,ej,0);
	check_score = CHECK_SCORE(Seq1+bi,Seq2+bj,M,N,S);
/**#ifdef STATS
	TRACE("\nAlign_score=%.1f\n",((FloatLo)c)/10.0);
	TRACE("\nCheck_score=%.1f\n",((FloatLo)check_score)/10.0);
	if (check_score != c) TRACE("\nCheck_score=%.1f\n",((FloatLo)check_score)/10.0);
#endif**/
	MemFree(SS+bj);
	MemFree(DD+bj);
	MemFree(SP+bj);
	MemFree(DP+bj);
	MemFree(ST+bj);
	MemFree(DT+bj);
	MemFree(PP[0]+bi+bj);
	MemFree(PP[1]+bi+bj);
	MemFree(PP[2]+bi+bj);
	MemFree(RI+bi+bj);
	MemFree(PT[0]+bi+bj);
	MemFree(PT[1]+bi+bj);
	MemFree(PT[2]+bi+bj);
	return c;
}

/* Alignment display routine */

static Char ALINE[51], BLINE[51], CLINE[51];
/**#if 0
void DISPLAY(Char A[], Char B[], Int4 M, Int4 N, Int4 S[], Int4 AP, Int4 BP)
{ register CharPtr a, b, c;
  register Int4   i,  j, op;
	   Int4   lines, ap, bp;

  i = j = op = lines = 0;
  ap = AP;
  bp = BP;
  a = ALINE;
  b = BLINE;
  c = CLINE;
  while (i < M || j < N)
    { if (op == 0 && *S == 0)
	{ op = *S++;
	  *a = A[++i];
	  *b = B[++j];
	  *c++ = (*a++ == *b++) ? '|' : ' ';
	}
      else
	{ if (op == 0)
	    op = *S++;
	  if (op > 0)
	    { *a++ = ' ';
	      *b++ = B[++j];
	      op--;
	    }
	  else
	    { *a++ = A[++i];
	      *b++ = ' ';
	      op++;
	    }
	  *c++ = '-';
	}
      if (a >= ALINE+50 || i >= M && j >= N)
	{ *a = *b = *c = '\0';
	  TRACE("\n%6d ",50*lines++);
	  for (b = ALINE+10; b <= a; b += 10)
	    TRACE("    .    :");
	  if (b <= a+5)
	    TRACE("    .");
	  TRACE("\n%6d %s\n       %s\n%6d %s\n",ap,ALINE,CLINE,bp,BLINE);
	  ap = AP + i;
	  bp = BP + j;
	  a = ALINE;
	  b = BLINE;
	  c = CLINE;
	}
    }
}
#endif**/

/* CHECK_SCORE - return the score of the alignment stored in S */

static Int4 CHECK_SCORE(Char A[], Char B[], Int4 M, Int4 N, Int4 S[])
{ 
  register Int4   i,  j, op;
  Int4 score;

  score = i = j = op = 0;
  while (i < M || j < N) {
	op = *S++;
	if (op == 0) 
		score = vv[A[++i]][B[++j]] + score;
	else if (op > 0) {
		score = score - (g+op*e);
		j = j+op;
	} else {
		score = score - (g-op*e);
		i = i-op;
	}
  }
  return(score);
}

