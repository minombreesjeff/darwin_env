static char const rcsid[] = "$Id: sim3.c,v 6.2 2003/05/30 17:25:38 coulouri Exp $";

/*
* sim3.c - compare two very similar sequences with affine gap penalties
*
* Version 1 : July 16, 1995.
* Version 2 : Aug. 31, 1995.  (Heuristic phase 1 + New output format)
* Version 2.1 : Nov. 4, 1995. (Rewrite sim3 as a procedure call)
*
* The command syntax is
*	sim3 file1 file2 [dist_limit]
* where file1 and file2 contain arbitrary sequences of characters.  It is
* assumed that the two character strings are very similar.  Sim3 reports
* an alignment of the two sequences.
*
* If dist_limit > 0, a heuristic phase 1 is used.
* If dist_limit <= 0, an exact phase 1 is used.
* Absolute(dist_limit) specifies the tolerable distance limit.
* (Zero means a default distance limit is used.)
*
* For example,
*  sim3 file1 file2      (A heuristic phase 1 is used with default limit.)
*  sim3 file1 file2 500  (A heuristic phase 1 is used with limit=500.)
*  sim3 file1 file2 0    (An exact phase 1 is used with default limit.)
*  sim3 file1 file2 -500 (An exact phase 1 is used with limit=500.)
*/

#include <simutil.h>

#define MININT -99999
#define W	8	/* Word size */
#define NW	60	/* Number of words used in the heuristic method */


/********************************************************************
*
*	functions related to SIM3
*
********************************************************************/

static Int4 power(Int4 base, Int4 n)
{
	Int4 i, p;

	for (i=1, p=1; i<=n; ++i) p = p*base;
	return p;
}

typedef struct max_chain {
	Int4 j;	/* j position */
	struct max_chain PNTR next;
} MaxChain, PNTR MaxChainPtr;

static void FreeMaxChain(MaxChainPtr head)
{
	MaxChainPtr next;

	while(head)
	{
		next = head->next;
		MemFree(head);
		head = next;
	}
}

/* set up the hash table for the heuristic methods*/
static Boolean locate(Int4Ptr i1, Int4Ptr j1, Int4Ptr i2, Int4Ptr j2, Int4 M, CharPtr A, Int4 N, CharPtr B, Int4 limit)
{
	Int4 encoding[128];
	Int4 mask;
	Int4 BUCKETS;
	CharPtr bucket;
	Int4 i, j;
	CharPtr p, q;
	Int4 ecode, ecode1;
	Int4 fcount, bcount, fmax, bmax;
	FloatHi temp;
	FloatHi trust_ratio;
	Int4 paired_fj, paired_bj, best_range, t;
	MaxChainPtr fj, bj, tj, tj1, tj2;


	if (M < W+NW-1)
	{  /*W=8 word size, NW=60, # of words in heuristic method*/

		/*Message(MSG_POSTERR, "The sequences are too short for the heuristic method.");*/
		return FALSE;
	}

	/* Set up the hash table (derived from Sequence A) */
	for (i=0; i<128; ++i) encoding[i] = 0;
	encoding['C'] = encoding['c'] = 1;
	encoding['G'] = encoding['g'] = 2;
	encoding['T'] = encoding['t'] = 3;
	mask = (1 <<(W+W-2)) - 1;
	BUCKETS = power(4, W);
	bucket = MemNew((size_t)BUCKETS * sizeof(Char));

	/* front end */
	p = A;
	ecode = 0L;
	for (i=1; i<W; ++i)
		ecode = (ecode << 2) + encoding[*p++];
	for (i=0; i<NW; ++i) {
		ecode = ((ecode & mask) << 2) + encoding[*p++];
		bucket[ecode] = 1;
	}

	/* back end */
	p = A+M-W-NW+1;
	ecode = 0L;
        for (i=1; i<W; ++i)
                ecode = (ecode << 2) + encoding[*p++];
        for (i=0; i<NW; ++i) {
                ecode = ((ecode & mask) << 2) + encoding[*p++];
                if (bucket[ecode] == 1 || bucket[ecode] == 3) bucket[ecode] = 3;
		else bucket[ecode] = 2;
        }

	/* Locate the interval with the maximum number of the matched 8-tuples */
	trust_ratio = 0.6;
	temp = (FloatHi)NW * trust_ratio;
	fmax = bmax = (Int4)temp;
	fj = bj = NULL;
	fcount = bcount = 0;
	p = B;
	ecode = 0L;
	for (j=1; j<W; ++j)
                ecode = (ecode << 2) + encoding[*p++];
	ecode1 = ecode;
	q = p;
	for (j=0; j<NW; ++j) {
                ecode = ((ecode & mask) << 2) + encoding[*p++];
		if (bucket[ecode] == 1 || bucket[ecode] == 3) ++fcount;
		if (bucket[ecode] == 2 || bucket[ecode] == 3) ++bcount;
	}

	if (fcount >= fmax) {
		fj = MemNew(sizeof(MaxChain));
		fj->j = 0;
		fj->next = NULL;
		fmax = fcount;
	}
	if (bcount >= bmax) {
		bj = MemNew(sizeof(MaxChain));
		bj->j = W+NW-2;
		bj->next = NULL;
		bmax = bcount;
	}

	for (j=1; j <= N-W-NW+1; ++j) {
		ecode1 = ((ecode1 & mask) << 2) + encoding[*q++];
		if (bucket[ecode1] == 1 || bucket[ecode1] == 3) --fcount;
                if (bucket[ecode1] == 2 || bucket[ecode1] == 3) --bcount;
                ecode = ((ecode & mask) << 2) + encoding[*p++];
		if (bucket[ecode] == 1 || bucket[ecode] == 3) ++fcount;
                if (bucket[ecode] == 2 || bucket[ecode] == 3) ++bcount;

		if (fcount > fmax) {
			FreeMaxChain(fj);
			fj = MemNew(sizeof(MaxChain));
			fj->j = j;
			fj->next = NULL;
			fmax = fcount;
		} else if (fcount == fmax) {
			tj = MemNew(sizeof(MaxChain));
			tj->j = j;
			tj->next = fj;
			fj = tj;
		}

		if (bcount > bmax) {
			FreeMaxChain(bj);
			bj = MemNew(sizeof(MaxChain));
			bj->j = j+W+NW-2;
			bj->next = NULL;
			bmax = bcount;
		} else if (bcount == bmax) {
			tj = MemNew(sizeof(MaxChain));
			tj->j = j+W+NW-2;
			tj->next = bj;
			bj = tj;
		}
	}

	MemFree(bucket);
	
#ifdef STATS
	Message(MSG_OK, "NW=%d, fmax=%d, bmax=%d\n", NW, fmax, bmax);
#endif

	if (fj == NULL || bj == NULL)
	{
		FreeMaxChain(fj);
		FreeMaxChain(bj);
		/*Message(MSG_POSTERR, "The end-intervals are too different for the heuristic method.");*/
		return FALSE;
	}

	best_range = limit+1;
	for (tj1 = fj; tj1 != NULL; tj1 = tj1->next) {
		for (tj2 = bj; tj2 != NULL; tj2 = tj2->next) {
			t = tj2->j - tj1->j + 1 - M;
			if (t<0) t = -t;
/*
Message(MSG_POST, "tj1=%d, tj2=%d, t=%d\n", tj1->j, tj2->j, t);
*/
			if (t<best_range) {
				best_range = t;
				paired_fj = tj1->j;
				paired_bj = tj2->j;
			}
		}
	}

#ifdef STATS
	Message(MSG_OK, "paired_fj=%d, paired_bj=%d, best_range=%d\n", paired_fj, paired_bj, best_range);
#endif

	FreeMaxChain(fj);
	FreeMaxChain(bj);
	if (best_range > limit) {
		/* Message(MSG_POSTERR,"Heuristic phase 1 failed");*/
		return FALSE;
	}

	*i1 = 0;
	*i2 = M;
	*j1 = paired_fj;
	*j2 = paired_bj+1;
	return TRUE;

}


static Int4 snake(Int4 k, Int4 x, Int4 endx, Int4 endy, CharPtr A, CharPtr B)
{
	Int4 y;

	if (x < 0) return x;
	y = x + k;
	while (x < endx && y < endy && A[x]==B[y]) {
		++x; ++y;
	}
	return x;
}

static Int4 rsnake(Int4 k, Int4 x, Int4 startx, Int4 starty, CharPtr A, CharPtr B, Int4 M)
{
	Int4 y;

	if (x > M) return x;
	y = x+k;
	while (x>startx && y>starty && A[x-1]==B[y-1]) {
		--x; --y;
	}
	return x;
}


/***************************************************************
*
*	get_dist()
*	compute the distance/similarity between the two sequences
*
****************************************************************/
static Int4 get_dist(Int4 i1, Int4 j1, Int4 i2, Int4 j2, CharPtr A, CharPtr B, Int4 limit)
{
	Int4Ptr SS, DD, II;
	Int4 goal_diag;
	Int4 c, k, t1, t2, t;
	Int4 start, lower, upper;

	/* Compute the boundary diagonals */
	start = j1 - i1;
	lower = MAX(j1-i2, start-limit);
	upper = MIN(j2-i1, start+limit);
	goal_diag = j2-i2;

	if (goal_diag > upper || goal_diag < lower)
	{
		/*Message(MSG_POSTERR,"Two sequences are not really similar\n Try exact phase 1 method"); */
		return -1;
	}

	 /* Allocate space for forward vectors */
	SS = MemNew((size_t)(upper-lower+1)*sizeof(Int4));
	DD = MemNew((size_t)(upper-lower+2)*sizeof(Int4));
	II = MemNew((size_t)(upper-lower+2)*sizeof(Int4));
	SS -= lower;
	DD -= lower;
	II -= (lower -1);

	/* Initialization */
	for (k=lower; k<=upper; ++k) SS[k] = MININT;
	for (k=lower; k<=upper+1; ++k) DD[k] = MININT;
	for (k=lower-1; k<=upper; ++k) II[k] = MININT;
	SS[start] = snake(start, i1, i2, j2, A, B);

	if (SS[goal_diag] >= i2)
	{
#ifdef STATS
		Message(MSG_OK, "get_dist = %d\n", 0);
#endif

		/* Free working vectors */
		MemFree(SS+lower);
		MemFree(DD+lower);
		MemFree(II+lower-1);
		return 0;
	}

	for (c=1; c<=limit; ++c)
	{
		t = MAX(lower, start-c);
		t1 = II[t-1];
		for (k=t; k<=MIN(upper, start+c); ++k)
		{
			t2 = II[k];
			II[k] = MAX(t1, SS[k]);
			t1 = t2;
			DD[k] = MAX(DD[k+1]+1, SS[k]);
			SS[k] = snake(k, MIN(j2-k,MAX(MAX(SS[k]+1, II[k]), DD[k])), i2, j2, A, B);
		}

		if (SS[goal_diag] >= i2) 
		{
#ifdef STATS
			Message(MSG_OK, "get_dist = %d\n", c);
#endif

			/* Free working vectors */
			MemFree(SS+lower);
			MemFree(DD+lower);
			MemFree(II+lower-1);
			return c;
		}
	}

	/* Ran out of distance limit */
	MemFree(SS+lower);
	MemFree(DD+lower);
	MemFree(II+lower-1);

	/*Message(MSG_POSTERR,"Two sequences are not really similar.\nTry exact phase 1");*/
	return -1;
}

static Int4 dist_only(Int4Ptr i1, Int4Ptr j1, Int4Ptr i2, Int4Ptr j2, CharPtr A, CharPtr B, Int4 limit, Int4 M, Int4 N)
{
	Int4Ptr S, D, I;
	Int4Ptr SP, DP, IP;
	Int4 c, k;
	Int4 t1, t2;
	Int4 tp1, tp2;

	/* Initial space allocation */
	S = MemNew((size_t)(N+limit)*sizeof(Int4));
	SP = MemNew((size_t)(N+limit)*sizeof(Int4));
	D = MemNew((size_t)(N+limit+1)*sizeof(Int4));
	DP = MemNew((size_t)(N+limit+1)*sizeof(Int4));
	I = MemNew((size_t)(N+limit+1)*sizeof(Int4));
	IP = MemNew((size_t)(N+limit+1)*sizeof(Int4));
	S += (limit -1);
	SP += (limit -1);
	D += (limit -1);
	DP += (limit -1);
	I += limit;
	IP += limit;

	/* Free end-gaps for the shorter sequence */
	for (k=0; k<=N; ++k)
	{
			S[k] = snake(k, 0, M, N, A, B);
			SP[k] = k;
			if (S[k] >= M)
			{
#ifdef STATS
				Message(MSG_OK, "k=%d, S[k]=%d, SP[k]=%d, distance = %d\n", k, S[k], SP[k], c);
#endif
				*i1 = 0;
				*j1 = SP[k];
				*i2 = M;
				*j2 = MIN(N, k+M);
				MemFree(S-limit+1);
				MemFree(SP-limit+1);
				MemFree(D-limit+1);
				MemFree(DP-limit+1);
				MemFree(I-limit);
				MemFree(IP-limit);
				return 0;
			}
	}
	for (k=1-limit; k<=-1; ++k) {
		S[k] = MININT;
		SP[k] = 0;
	}
	for (k=-limit; k<=N; ++k) I[k] = MININT;
	for (k=1-limit; k<=N+1; ++k) D[k] = MININT;

	for (c=1; c<=limit; ++c) {
		t1 = I[-c];
		tp1 = IP[-c];
		for (k=1-c; k<=N; ++k) {
			t2 = I[k];
			tp2 = IP[k];
			/* I[k] = MAX(t1, S[k]); */
			if (t1 > S[k]) {
				I[k] = t1;
				IP[k] = tp1;
			} else {
				I[k] = S[k];
				IP[k] = SP[k];
			}
			t1 = t2;
			tp1 = tp2;

			/* D[k] = MAX(D[k+1]+1,S[k]); */
			if (D[k+1]+1 > S[k]) {
				D[k] = D[k+1]+1;
				DP[k] = DP[k+1];
			} else {
				D[k] = S[k];
				DP[k] = SP[k];
			}

			/* S[k] = snake(k, MIN(N-k,MAX(MAX(S[k]+1, I[k]), D[k])), M, N); */
			if (S[k]+1 >= I[k]) {
				S[k] = MIN(N-k, S[k]+1);
			} else {
				S[k] = I[k];
				SP[k] = IP[k];
			}
			if (D[k] > S[k]) {
				S[k] = D[k];
				SP[k] = DP[k];
			}

			S[k] = snake(k, S[k], M, N, A, B);

/*
Message(MSG_POST, "k=%d, S[k]=%d, D[k]=%d, I[k]=%d, distance = %d\n", k, S[k], D[k], I[k], c);
*/

			if (S[k] >= M) {
#ifdef STATS
				Message(MSG_OK, "k=%d, S[k]=%d, SP[k]=%d, distance = %d\n", k, S[k], SP[k], c);
#endif
				*i1 = 0;
				*j1 = SP[k];
				*i2 = M;
				*j2 = MIN(N, k+M);
				MemFree(S-limit+1);
				MemFree(SP-limit+1);
				MemFree(D-limit+1);
				MemFree(DP-limit+1);
				MemFree(I-limit);
				MemFree(IP-limit);
				return c;
			}
		}
	}

	/* Ran out of distance limit */
	/*Message(MSG_POSTERR,"Two sequences are not really similar\n"); */
	MemFree(S-limit+1);
	MemFree(SP-limit+1);
	MemFree(D-limit+1);
	MemFree(DP-limit+1);
	MemFree(I-limit);
	MemFree(IP-limit);
	return -1;
}


static void path(Int4 i1, Int4 j1, Char type1, Int4 i2, Int4 j2, Char type2, Int4 dist, EditScriptPtr PNTR head, EditScriptPtr PNTR tail, CharPtr A, CharPtr B, Int4 M, Int4 N)
{
	Int4Ptr SS, DD, II;	 /* Forward vectors */
	Int4Ptr RS, RD, RI;	 /* Backward vectors */

	EditScriptPtr head1, tail1, head2, tail2;
	Int4 midc, rmidc;
	Int4 start, lower, upper;
	Int4 rstart, rlower, rupper;
	Int4 c, k, t1, t2, t;
	Int4 maxint;
	Int4 mi, mj;
	Uint1 mtype;
	Char flag;


	/* Boundary cases */
	if (i1 == i2)
	{
		if (j1 == j2)
			*head = NULL;
		else
		{
			head1 = MemNew(sizeof(EditScript));
			head1->op_type = INS;
			head1->num = j2-j1;
			head1->next = NULL;
			*head = *tail = head1;
		}
		return;
	}

	if (j1 == j2)
	{
		head1 = MemNew(sizeof(EditScript));
		head1->op_type = DEL;
		head1->num = i2-i1;
		head1->next = NULL;
		*head = *tail = head1;
		return;
	}

	if (dist <= 1)
	{
		if (j2-i2 == j1-i1)
		{
			head1 = MemNew(sizeof(EditScript));
			head1->op_type = SUB;
			head1->num = i2-i1;
			head1->next = NULL;
			*head = *tail = head1;
		}
		else if (j2-i2 > j1-i1)
		{
			if (type1 == INS)
			{
				head1 = MemNew(sizeof(EditScript));
				head1->op_type = INS;
				head1->num = 1;
				head2 = MemNew(sizeof(EditScript));
				head2->op_type = SUB;
				head2->num = i2-i1;
			}
			else
			{
				head1 = MemNew(sizeof(EditScript));
				head1->op_type = SUB;
				head1->num = i2-i1;
				head2 = MemNew(sizeof(EditScript));
				head2->op_type = INS;
				head2->num = 1;
			}
				head1->next = head2;
				head2->next = NULL;
				*head = head1;
				*tail = head2;
		}
		else if (j2-i2 < j1-i1)
		{
			if (type1 == DEL)
			{
				head1 = MemNew(sizeof(EditScript));
				head1->op_type = DEL;
				head1->num = 1;
				head2 = MemNew(sizeof(EditScript));
				head2->op_type = SUB;
				head2->num = j2-j1;
			}
			else
			{
				head1 = MemNew(sizeof(EditScript));
               	head1->op_type = SUB;
               	head1->num = j2-j1;
				head2 = MemNew(sizeof(EditScript));
               	head2->op_type = DEL;
               	head2->num = 1;
			}
			head1->next = head2;
			head2->next = NULL;
			*head = head1;
			*tail = head2;
		}
		return;
	}

	/* Divide the problem at the middle cost */
	midc = dist/2;
	rmidc = dist - midc;

	/* Compute the boundary diagonals */
	start = j1 - i1;
	lower = MAX(j1-i2, start-midc);
	upper = MIN(j2-i1, start+midc);
	rstart = j2-i2;
	rlower = MAX(j1-i2, rstart-rmidc);
	rupper = MIN(j2-i1, rstart+rmidc);

	/* Allocate space for forward vectors */
	SS = MemNew((size_t)(upper-lower+1)*sizeof(Int4));
	DD = MemNew((size_t)(upper-lower+2)*sizeof(Int4));
	II = MemNew((size_t)(upper-lower+2)*sizeof(Int4));
	SS -= lower;
	DD -= lower;
	II -= (lower -1);

	/* Forward computation */
	for (k=lower; k<=upper; ++k) SS[k] = MININT;
	for (k=lower; k<=upper+1; ++k) DD[k] = MININT;
	for (k=lower-1; k<=upper; ++k) II[k] = MININT;
	if (type1 == SUB) SS[start] = snake(start, i1, i2, j2, A, B);
	else if (type1 == DEL) {
		DD[start] = i1;
		SS[start] = snake(start,i1,i2,j2, A, B);
	} else {
		II[start] = i1;
		SS[start] = snake(start,i1,i2,j2, A, B);
	}

	for (c=1; c<=midc; ++c) {
		t = MAX(lower, start-c);
		t1 = II[t-1];
		for (k=t; k<=MIN(upper, start+c); ++k) {
			t2 = II[k];
			II[k] = MAX(t1, SS[k]);
			t1 = t2;
			DD[k] = MAX(DD[k+1]+1, SS[k]);
			SS[k] = snake(k, MIN(j2-k,MAX(MAX(SS[k]+1, II[k]), DD[k])), i2, j2, A, B);
		}
	}

	/* Allocate space for backward vectors */
	RS = MemNew((size_t)(rupper-rlower+1)*sizeof(Int4));
	RD = MemNew((size_t)(rupper-rlower+2)*sizeof(Int4));
	RI = MemNew((size_t)(rupper-rlower+2)*sizeof(Int4));
	RS -= rlower;
	RD -= (rlower -1);
	RI -= rlower;

	/* Backward computation */
	maxint = i2 + dist + N;
	for (k=rlower; k<=rupper; ++k) RS[k] = maxint;
	for (k=rlower-1; k<=rupper; ++k) RD[k] = maxint;
	for (k=rlower; k<=rupper+1; ++k) RI[k] = maxint;
	if (type2 == SUB)
	   RI[rstart] = RD[rstart] = RS[rstart] = rsnake(rstart, i2, i1, j1, A, B, M);
	else if (type2 == DEL) RD[rstart] = i2;
	else RI[rstart] = i2;

	for (c=1; c<=rmidc; ++c) {
		t = MAX(rlower, rstart-c);
		t1 = RD[t-1];
		for (k=t; k<=MIN(rupper, rstart+c); ++k) {
			RS[k] = rsnake(k, MAX(j1-k, MIN(MIN(RS[k]-1,RD[k]),RI[k])),i1,j1,A,B, M);
			t2 = RD[k];
			RD[k] = MIN(t1-1, RS[k]);
			t1 = t2;
			RI[k] = MIN(RI[k+1], RS[k]);
		}
	}

	/* Find (mi, mj, mtype) such that
	       the distance from (i1, j1, type1) to (mi, mj, mtype) is midc
	   and the distance from (mi, mj, mtype) to (i2, j2, type2) is rmidc.
	*/

	flag = FALSE;
	for (k=MAX(lower,rlower); k<=MIN(upper,rupper);++k)
	{

		if (SS[k]>=RS[k] || DD[k]>=RD[k] || II[k]>=RI[k])
		{
			if (DD[k]>=RD[k])
			{
				mi = DD[k];
				mj = k+mi;
				mtype = DEL;
			}
			else if (II[k] >= RI[k])
			{
				mi = II[k];
				mj = k+mi;
				mtype = INS;
			}
			else
			{
				mi = SS[k];
				mj = k+mi;
				mtype = SUB;
			}

			flag = TRUE;
			break;
		}
	}

	/* Free working vectors */
	MemFree(SS+lower);
	MemFree(DD+lower);
	MemFree(II+lower-1);
	MemFree(RS+rlower);
	MemFree(RD+rlower-1);
	MemFree(RI+rlower);

	if (flag)
	{
		/* Find a path from (i1,j1,type1) to (mi,mj,mtype) */
		path(i1,j1,type1,mi,mj,mtype,midc,&head1,&tail1,A,B,M,N);

		/* Find a path from (mi,mj,mtype) to (i2,j2,type2) */

		path(mi,mj,mtype,i2,j2,type2,rmidc,&head2,&tail2,A,B,M,N);

		/* Join these two paths together */
		if (head1)
			tail1->next = head2;
		else
			head1 = head2;
	}
	else
	{
		/* Message(MSG_POSTERR,"Something wrong when dividing\n"); */
		head1 = NULL;
	}
	*head = head1;
	if(head2)
		*tail = tail2;
	else
		*tail = tail1;
}


/* Condense_script - merge contiguous operations of the same type together */
static void Condense_script(EditScriptPtr head)
{
	EditScriptPtr tp, tp1;

	tp = head;
	while (tp != NULL)
	{
		while (((tp1 = tp->next) != NULL) && (tp->op_type == tp1->op_type))
		{
			tp->num = tp->num + tp1->num;
			tp->next = tp1->next;
			MemFree(tp1);
		}
		tp = tp->next;
	}
}

/*************************************************************
*
*	Reverse_Script()
*	reverse the insertions and deletions
*
*************************************************************/
static void Reverse_Script(EditScriptPtr head)
{
	EditScriptPtr tp;

	tp = head;
	while (tp != NULL)
	{
	   if (tp->op_type == INS)
		   tp->op_type = DEL;
	   else if (tp->op_type == DEL)
		   tp->op_type = INS;
	   tp = tp->next;
	}
}



/* Check_script - check the total distance described in the script */
static Int4 Check_script(EditScriptPtr head, Int4 I1, Int4 J1, CharPtr A, CharPtr B)
{
	Int4 i, j, t, dist;
	EditScriptPtr tp;

	dist = 0;
	i = I1;
	j = J1;
	tp = head;
	while (tp != NULL)
	{
		if (tp->op_type == SUB)
		{
			for (t=0; t<tp->num; ++t)
			{
				if (A[i] != B[j])
					++dist;
				++i; ++j;	
			}
		}
		else if (tp->op_type == INS)
		{
			dist += 1+tp->num;
			j += tp->num;
		}
		else
		{	/* DEL */
			dist += 1+tp->num;
			i += tp->num;
		}
	tp = tp->next;
	}

	return dist;
}


static void Print_Script(EditScriptPtr head, Int4 M, Int4 N, CharPtr A, CharPtr B)
{
	EditScriptPtr tp;
	Int4 i, j, k, count;

	i = j = 0;
	tp = head;
	while (tp != NULL)
	{
		if (tp->op_type == SUB)
		{
			k = 0;
			while (k < tp->num)
			{
				count = 0;
				while (A[i] == B[j])
				{
					++i;
					++j;
					++count;
					++k;
				}
				if (count > 0)
					fprintf(stdout, "copy %d\n", count);
				if (k < tp->num)
				{
					fprintf(stdout, "replace %c by %c\n", A[i], B[j]);
					++i;
					++j;
					++k;
				}
			}
		}
		else if (tp->op_type == INS)
		{
			if ((tp==head || tp->next==NULL) && (M <= N))
					fprintf(stdout, "skip (second sequence) %d\n", tp->num);
			else
			{
				fprintf(stdout, "insert ");
				for (k=j; k<j+tp->num; ++k) fprintf(stdout, "%c", B[k]);
				fprintf(stdout, "\n");
			}
			j += tp->num;
		}
		else
		{	/* DEL */
			if ((tp==head || tp->next==NULL) && (M > N))
				fprintf(stdout, "skip (first sequence) %d\n", tp->num);
			else
			{
				fprintf(stdout, "delete ");
				for (k=i; k<i+tp->num; ++k) printf("%c", A[k]);
					fprintf(stdout, "\n");
			}
			i += tp->num;
		}
		tp = tp->next;
	}
}

/* sim3 - a procedure for comparing two very similar sequences */
/* INPUT: sequence1, length1,
	  sequence2, length2,
	  distance limit (to bound the computation)

   OUTPUT: 1. the distance between sequence1 and sequence2;
	   2. an edit script describing the alignment.
*/
static EditScriptPtr sim3(CharPtr seq1, Int4 len1, CharPtr seq2, CharPtr seq2C, Int4 len2, Int4 LIMIT, Int4Ptr d, BoolPtr use_sec, Boolean heuristic_only)
{
	Int4 dist, check_dist, distC;
	Boolean sflag;
	EditScriptPtr head = NULL, tail, tp;
	Int4 I1, J1, I2, J2;
	Int4 I1_C, J1_C, I2_C, J2_C;
	CharPtr A, B;
	CharPtr A_C = NULL, B_C = NULL;
	Boolean ck_reverse;
	Int4 M, N;
	Int4 limit;

	limit = LIMIT;

	sflag = FALSE;
	*use_sec = FALSE;
	ck_reverse = (seq2C != NULL);
	if (len1>len2) {	/* Switch the sequences */
		A = seq2;
		B = seq1;
		M = len2;
		N = len1;
		A_C = seq2C;
		B_C = seq1;
		sflag = TRUE;
	}
	else
	{
		A = seq1;
		A_C = seq1;
		B = seq2;
		B_C = seq2C;
		M = len1;
		N = len2;
		sflag = FALSE;
	}

	/* Compute the distance between two sequences A and B*/
	dist = -1;
	distC = -1;
	if (limit > 0)
	{
		/* Message(MSG_POST, "A heuristic phase 1 is tried."); */
		if(locate(&I1, &J1, &I2, &J2, M, A, N, B, limit))
			dist = get_dist(I1, J1, I2, J2, A, B, limit);
		if(ck_reverse)	/*check the complementary*/
		{
			if(locate(&I1_C, &J1_C, &I2_C, &J2_C, M, A_C, N, B_C, limit))
				distC = get_dist(I1_C, J1_C, I2_C, J2_C, A_C, B_C, limit);
		}
		if(distC != -1)
		{
			if(dist == -1 || distC < dist)
			{
				dist = distC;
				*use_sec = TRUE;
				I1 = I1_C;
				I2 = I2_C;
				J1 = J1_C;
				J2 = J2_C;
				A = A_C;
				B = B_C;
			}
		}
	}
	else
	{
		if (limit == 0)
			limit = DEF_LIMIT;
		else
			limit = -limit;
	}
	if(dist == -1 && heuristic_only)
		return NULL;

	if(dist == -1)
	{
		/* Message(MSG_POST, "An exact phase 1 is used.\n"); */
		dist = dist_only(&I1, &J1, &I2, &J2, A, B, limit, M, N);
		if(ck_reverse)
		{
			distC = dist_only(&I1_C, &J1_C, &I2_C, &J2_C, A_C, B_C, limit, M, N);
			if(distC != -1)
			{
				if(dist == -1 || distC < dist)
				{
					dist = distC;
					*use_sec = TRUE;
					I1 = I1_C;
					I2 = I2_C;
					J1 = J1_C;
					J2 = J2_C;
					A = A_C;
					B = B_C;
				}
			}
		}
	}
	if(dist == -1)
	{
		/* Message(MSG_POST, "two sequences are not exactly similar\n");*/
		return NULL;
	}

	/*Message(MSG_POST, "distance limit = %d\n", limit); */

#ifdef STATS
	Message(MSG_POST, "I1=%d, J1=%d, I2=%d, J2=%d, dist=%d\n",I1, J1, I2, J2, dist);
#endif

	/* Deliver the actual alignment.
	   Head is a pointer to an alignment.
	   See the description of the EditScript structure. */

	path(I1, J1, SUB, I2, J2, SUB, dist, &head, &tail, A, B,M,N);

	if(head == NULL)
		return NULL;

	/* Merge the contiguous same operations together */
	Condense_script(head);

	/* Reset tail (due to the condensation) */
	tail = head;
	while (tail->next != NULL)
		tail = tail->next;

	check_dist = Check_script(head, I1, J1, A, B);
	/*Message(MSG_POST, "check_dist=%d\n", check_dist);*/
	*d = check_dist;

	/* Add end gaps, if necessary */
	if (J1 > 0)
	{
		if (head->op_type == INS)
			head->num += J1;
	    else
		{
			tp = MemNew(sizeof(EditScript));
			tp->op_type = INS;
			tp->num = J1;
			tp->next = head;
			head = tp;
	    }
	}

	if (J2 < N)
	{
		if (tail->op_type == INS)
			tail->num = tail->num + N - J2;
		else
		{
			tp = MemNew(sizeof(EditScript));
			tp->op_type = INS;
			tp->num = N-J2;
			tp->next = NULL;
			tail->next = tp;
			tail = tp;
		}
	}

	if (sflag == TRUE) {	/* Switch back the sequences */
		Reverse_Script(head);
	}

	/*Print_Script(head, M, N, seq1, seq2);

	*d = dist; */
	return head;
}


static Int4 get_start_pos(SeqLocPtr slp)
{
	if(SeqLocStrand(slp) == Seq_strand_minus)
		return (-SeqLocStop(slp));
	else
		return SeqLocStart(slp);
}

static Int4 get_current_pos(Int4Ptr pos, Int4 length)
{
	Int4 val;
	if(*pos < 0)
		val = -(*pos + length -1);
	else
		val = *pos;
	*pos += length;
	return val;
}

static Boolean is_end_seg(EditScriptPtr curr, EditScriptPtr head)
{
	EditScriptPtr next;

	if(curr == head || curr->next == NULL)
		return TRUE;
	if(curr != NULL && curr->op_type != SUB)
	{
		next = curr->next;
		while(next)
		{
			if(next->op_type == SUB)
				return FALSE;
			next = next->next;
		}
	}
	return TRUE;
}
		

SeqAlignPtr SIM3ALN_choice  (SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 limit, Boolean heuristic_only, Boolean show_end_gap)
{
	SeqAlignPtr sap = NULL;
	Boolean both_strand, is_dna;
	Uint1 strand_2, strand_1;
	CharPtr A, B, compB;
	Int4 len1, len2;
	EditScriptPtr esp;
	Int4 dist = -1;
	Boolean use_sec;

 
	if(cslp1 == NULL || cslp2 == NULL)
	{
		Message(MSG_ERROR, "Insufficient input");
		return NULL;
	}

	if(cslp1->choice != SEQLOC_INT || cslp2->choice !=SEQLOC_INT)
	{
		Message(MSG_ERROR, "Incorrect type of Seq-loc. Only take Seq-int");
		return NULL;
	}
	strand_1 = SeqLocStrand(cslp1);
	strand_2 = SeqLocStrand(cslp2);
	both_strand = check_strand_mol(cslp2, &is_dna);
	if(!is_dna)
	{
		Message(MSG_POSTERR, "Sorry, SIM3 can only compare two DNA sequences");
		return NULL;
	}

	if(both_strand)
	{
		Change_Loc_Strand(cslp1, Seq_strand_plus);
		Change_Loc_Strand(cslp2, Seq_strand_plus);
	}

	A= make_sim_seq(cslp1, TRUE, NULL);
	if(A == NULL)
	{
		Change_Loc_Strand(cslp1, strand_1);
		Change_Loc_Strand(cslp2, strand_2);
		return NULL;
	}

	B = make_sim_seq(cslp2, TRUE, NULL);
	if(B == NULL)
	{
		MemFree(A);
		Change_Loc_Strand(cslp1, strand_1);
		Change_Loc_Strand(cslp2, strand_2);
		return NULL;
	}

	compB = NULL;
	if(both_strand)
	{
		Change_Loc_Strand(cslp2,(Uint1)2);
		compB = make_sim_seq(cslp2, TRUE, NULL);
	}
	len1 = SeqLocLen(cslp1);
	len2 = SeqLocLen(cslp2);

	use_sec = FALSE;
	if(compB == NULL)
		esp = sim3(A+1, len1, B+1, NULL, len2, limit, &dist, &use_sec, heuristic_only);
	else
		esp = sim3(A+1, len1, B+1, compB+1, len2, limit, &dist, &use_sec, heuristic_only);
	MemFree(A);
	MemFree(B);
	if(compB != NULL)
		MemFree(compB);
	if(esp != NULL)
	{
		if(!use_sec && both_strand)
			Change_Loc_Strand(cslp2, Seq_strand_plus);
		sap = ConvertEditScript(esp, cslp1, cslp2, 0, 0, show_end_gap);
		Script_free(esp);
	}

	Change_Loc_Strand(cslp1, strand_1);
	Change_Loc_Strand(cslp2, strand_2);
	return sap;
}


SeqAlignPtr SIM3ALN (SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 limit)
{
	return SIM3ALN_choice(cslp1, cslp2, limit, FALSE, FALSE);
}

SeqAlignPtr SIM3ALN_heuristic(SeqLocPtr cslp1, SeqLocPtr cslp2, Int4 limit)
{
	return SIM3ALN_choice(cslp1, cslp2, limit, TRUE, FALSE);
}
