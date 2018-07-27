static char const rcsid[] = "$Id: falign.c,v 6.3 2003/05/30 17:25:36 coulouri Exp $";

/*  falign.c
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
* File Name:  falign.c
*
* Author:  Jinghui Zhang and Kun-Mao Chao
*
* Version Creation Date: 5/24/95
*
*
* File Description:   
* Part 1 of sim2: generate the K best fragment alignments.
* This program reads two DNA sequences from files and finds their local 
* similarities.  It can detect alignments containing gaps, while striking a
* balance between speed and sensitivity.  The algorithm is based on three ideas:
* (1) techniques developed by Galil and coworkers that considerably improve
*     time efficiency,
* (2) modification of an approach of Hirschberg that dramatically improves
*     space efficiency, and
* (3) techniques that allow K best alignments to be determined efficiently
*     for K > 1.
*
*	R or r gives the score for replacement.
*	O or o gives the score for opening up a gap.
*	E or e gives the score for extending one more symbol in a gap.
*	W or w gives the word size (<=MAXW).
* 	(cost of a gap of length L is O + E*L 
*	 defaults: R = 0.2, O = 6.0, E = 0.2, W = 5)
*
* ODDITY: any character other than A, C, G or T is changed to 'A'.
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
*
* $Log: falign.c,v $
* Revision 6.3  2003/05/30 17:25:36  coulouri
* add rcsid
*
* Revision 6.2  2000/11/02 20:56:17  vakatov
* Renamed "join()" to "static s_Join()" to avoid name conflicts
*
* Revision 6.1  1998/06/16 18:30:55  kans
* fixed unix compiler warnings
*
* Revision 6.0  1997/08/25 18:53:08  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/03/14 15:45:14  kans
* undefine DEFAULT_W if previously defined in simutil.h
*
 * Revision 5.0  1996/05/28  13:43:15  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:50:15  ostell
 * force revision to 4.0
 *
 * Revision 1.2  1995/05/25  20:21:54  zjing
 * add header
 *
*
*
* ==========================================================================
*/


#ifndef _SIMUTIL_
#include <simutil.h>
#endif

#ifndef _LIST_
#include <list.h>
#endif

#ifndef _DB_LIST_
#include <db_list.h>
#endif


#define ALPHABET_SIZE   4	/* number of letters in the alphabet */
#define DEFAULT_IR	2 	/* default score for replacement (0.2) */
#define DEFAULT_IO	60	/* default score for opening up a gap (6.0) */
#define DEFAULT_IE	2	/* default score for gap extension (0.2) */
#ifdef DEFAULT_W
#undef DEFAULT_W /* in simutil.h */
#endif
#define DEFAULT_W	6	/* default word size */
#define MAXW		8	/* maximum word size */
#define GC_ROW 		200	/* garbage collection frequency */
#define MININT 		-99999	/* minimum value */


#define INTER_ADD(c)						\
{	if (!cintersect[in_row])				\
		cintersect[in_row] = idb_newList();		\
	db_insert(cintersect[in_row],c,NULL);			\
	ccol_int[c] = in_row;					\
}

typedef struct string5 {
	Int4 pos;		/* starting position */
	struct string5 PNTR link;	/* previous location with same 5-mer */
} string5;

typedef struct cut_frag {	/* cross fragment */
	fragment PNTR ff;		/* pointer to the forward fragment */
	fragment PNTR bf;		/* pointer to the backward fragment */
} cut_frag;

typedef struct frag_pt {	/* endpoint of the fragment */
	fragment PNTR fp;		/* pointer to the fragment */
	Int4 col;		/* end column of the fragment */
	struct frag_pt PNTR link;
} frag_pt;

Int4Ptr bucket;		/* hash table for rowwise computation */
Int4Ptr rcbucket;	/* hash table for columnwise computation */
Int4Ptr bls;
Int4Ptr rcbls;
Int4Ptr blink;
Int4Ptr rcblink;


CharPtr seq1, seq2;


Int4 W;			/* word size */
Int4 BUCKETS;		/* hash table size (ALPHABET_SIZE ** W) */
Int4 len1, len2;		/* lengths of sequences */
Int4 offset, offset1;	/* offset to make index positive */
Int4 number_frags;	/* number of fragments generated */
Int4 deleted_frags;	/* number of fragments deleted */
Int4 output_frags;	/* number of fragments output */
Int4 total_frags;	/* total number of fragments between two sequences */
Int4 old_i_start, old_i_end, old_j_start, old_j_end;	/* WEBB */

/* [sb1,sb2]X[se1,se2] is the area for aligning globally */
Int4 sb1, se1, sb2, se2, mid1, mid2, cur_end, cur_end1;

frag_pt PNTR PNTR end_row;	/* lists of endpoints row by row */
frag_pt PNTR PNTR end_col;	/* lists of endpoints column by column */
fragment PNTR PNTR diag_prev;	/* store the previous fragment on the same diagonal */
fragment PNTR PNTR bdiag_prev;	/* store the previous fragment on the same diagonal */
Int4Ptr col_int;		/* store the row position of intersection point */
Int4Ptr rccol_int;		/* store the row position of intersection point */
CharPtr diag_flag;	/* 1 if some intersection point is on the diagonal */
CharPtr rcdiag_flag;	/* 1 if some intersection point is on the diagonal */
idb_list PNTR intersect;	/* list for intersection points */
idb_list PNTR rcintersect;	/* list for intersection points */
ulist PNTR used_frag;	/* mark used fragments */
cut_frag PNTR PNTR cut;		/* store cross fragments */

Int4 K;			/* output K best local alignments */
Int4 IR,IO,IE;
/* cost of a gap of length L is IO/DIGIT + (IE/DIGIT)*L */

fragment PNTR pfo, PNTR pbo; 	/* pseudo origins */
fragment PNTR pff, PNTR pbf;	/* pseudo aligning fragments */
Int4 best_score = 0;	/* best score in the recomputation */
list ri;		/* right influence list */
list bri;		/* backward right influence list */
db_list li_c, li_d;	/* left influence lists */
db_list bli_c, bli_d;	/* backward left influence lists */

typedef struct kbclass {	/* k best equivalent classes */
	fragment PNTR first;	/* first fragment in the alignment */
	fragment PNTR last;		/* last fragment in the alignment */
	Int4 score;		/* best score for the class */
	Int4 t;			/* [t,b]X[l,r] contains all points whose */
	Int4 b;			/* score >= mw in the class */
	Int4 l;
	Int4 r;
} kbclass;
kbclass PNTR PNTR S;		/* store k best classes */
Int4 lastS = 0;		/* store number of classes */
Int4 curS = 0;		/* locality usage */
Int4 minS = 0;		/* store the class whose best score is mw */
Int4 mw = 0;		/* minimum score of all classes */

/* Prototype - added on Feb. 22, 1994 */
void table_init(void);
void fbld_table(void);
void bbld_table(void);
Int4 rcbld_table(Int4Ptr, Int4Ptr, Int4Ptr, CharPtr, Int4, Int4);
Int4 ex_table(Int4Ptr, Int4Ptr, Int4Ptr, CharPtr, Int4, Int4);
void free_table(Int4Ptr);
void update_active(list, db_list, db_list, Int4);
void rcupdate_active(list, db_list, db_list, Int4);
void rotupdate_active(frag_pt PNTR, Int4, Int4);
void cotupdate_active(frag_pt PNTR, Int4, Int4);
void free_all(Int4, Int4, Int4, fragment PNTR PNTR, list, db_list, db_list);
void scan(void);
void fscan_init(void);
void bscan_init(void);
void fscan(void);
void bscan(void);
void found(Int4, Int4);
void rfscan(Int4, Int4, Int4, Int4);
void rffound(Int4, Int4);
void enter_endrow(fragment PNTR);
void enter_endcol(fragment PNTR);
void ffound(Int4, Int4);
void bfound(Int4, Int4);
void setup(void);
void global_setup(fragment PNTR, fragment PNTR);
void global(fragment PNTR, fragment PNTR, Int4,  SeqLocPtr,  SeqLocPtr, SeqAlignPtr);
static Int4 s_Join(fragment PNTR, fragment PNTR);
void recom_setup(kbclass PNTR);
void recompute(kbclass PNTR);
Int4 disjoint(void);
void syn_area(void);
void ex_row(void);
void ex_col(void);
void rcfound(Int4, Int4);
void trcscan(void);
void trcfound(Int4, Int4);
void rclocal(fragment PNTR, Int4, Int4, list, db_list, db_list);
void lactive_ext(fragment PNTR);
void ractive_ext(fragment PNTR);
void rcfree(void);
SeqAlignPtr printS(kbclass PNTR, SeqLocPtr, SeqLocPtr, SeqAlignPtr);
SeqAlignPtr class_print(SeqLocPtr, SeqLocPtr);
kbclass PNTR findmaxS(void);
kbclass PNTR findS(fragment PNTR);
void set_class(Int4, fragment PNTR);
void replaceS(fragment PNTR);
void insertS(fragment PNTR);
Int4 sizeS(void);
Int4 minscoreS(void);
Int4 enterS(fragment PNTR, Int4, Int4);
void local(fragment PNTR);
void align(fragment PNTR, fragment PNTR PNTR, list, db_list, db_list, fragment PNTR);
void calign(fragment PNTR, fragment PNTR PNTR, list, db_list, db_list, fragment PNTR);
void malign(fragment PNTR, fragment PNTR PNTR, list, db_list, db_list, fragment PNTR);
void malign1(fragment PNTR, list, db_list, db_list, fragment PNTR);
frag_pt PNTR tailor(frag_pt PNTR, Int4);
frag_pt PNTR otr_survive(frag_pt PNTR, Int4);
void rotr_merge(frag_pt PNTR);
Int4 rotr_win(fragment PNTR, fragment PNTR);
void cotr_merge(frag_pt PNTR);
Int4 cotr_win(fragment PNTR, fragment PNTR);
frag_pt PNTR otl_survive(frag_pt PNTR, Int4);
void otl_merge(frag_pt PNTR, Int4, Int4, db_list, db_list, idb_list PNTR, Int4 PNTR, Char PNTR, Int4 (PNTR)(fragment PNTR), Int4 (PNTR)(fragment PNTR, fragment PNTR));
Int4 rotl_win(fragment PNTR, fragment PNTR);
Int4 cotl_win(fragment PNTR, fragment PNTR);
void r_survive(frag_pt PNTR);
void r_merge(list, frag_pt PNTR, Int4);
void r_add_point(list, fragment PNTR, Int4);
void resolve(db_list, db_list, idb_list PNTR, Char PNTR, Int4 PNTR, Int4,Int4 (PNTR)(fragment PNTR, Int4, Int4));
void l_merge(db_list, db_list, idb_list PNTR, Char PNTR, Int4 PNTR, frag_pt PNTR, Int4, Int4 (PNTR)(fragment PNTR, Int4, Int4));
void rcr_merge(list, frag_pt PNTR, Int4);
void rcr_add_point(list, fragment PNTR, Int4);
Int4 topo_win(fragment PNTR, fragment PNTR);
Int4 pt_lscore(fragment PNTR, Int4, Int4);
Int4 rcpt_lscore(fragment PNTR, Int4, Int4);
Int4 weight(fragment PNTR, fragment PNTR);
Int4 gap_cost(Int4);
Int4 diag(fragment PNTR);
Int4 rcdiag(fragment PNTR);
Int4 pos_diag(fragment PNTR);
Int4 rcpos_diag(fragment PNTR);
void pf_set(fragment PNTR, Int4, Int4);
Int4 power(Int4, Int4);
void decre(fragment PNTR);
void incre(fragment PNTR);
DenseDiagPtr frag_print(fragment PNTR, SeqLocPtr, SeqLocPtr, DenseDiagPtr);
void Xfrag_print(fragment PNTR);

/* falign - building fragment alignments (Feb., 1994) */
/**********************************************************************
*
*	falign(slp1, slp2, A, B, w, k, r, o, e)
*	compute the diagnols between slp1 and slp2. It is the first 
*	part of sim2. returns the Dense-Diag as Seq-align
*	A, B: string of Char for slp1 and slp2. Can be set to NULL as well
*	w: word size
*	k: top K alignment
*	r: penalty for replacement
*	o: penalty for open gap
*	e: penalty for gap extension
*
**********************************************************************/
SeqAlignPtr falign(SeqLocPtr slp1, SeqLocPtr slp2, CharPtr A, CharPtr B, Int4 w, Int4 k, Int4 r, Int4 o, Int4 e)
{
	Int4 i, j;
	SeqAlignPtr align;

	if(A == NULL)
		seq1 = make_sim_seq(slp1, TRUE, NULL);
	else
		seq1 = A;
	len1 = SeqLocLen(slp1);

	if(B == NULL)
		seq2 = make_sim_seq(slp2, TRUE, NULL);
	else
		seq2 = B;
	len2 = SeqLocLen(slp2);

	lastS = 0;         /* store number of classes */
	curS = 0;          /* locality usage */
	minS = 0;          /* store the class whose best score is mw */
	mw = 0;
	W = w;
	K = k;
	IR = r;
	IO = o;
	IE = e;
	if (IR > 2*IE)
		fatal("repl_cost should not be greater than 2*gap_extend");
	if (K<0 || IR<0 || IO<0 || IE<0)
		fatal("error! all input should be nonnegative.");

	S = (kbclass **)ckalloc((K+1)*sizeof(kbclass *));


	BUCKETS = power(ALPHABET_SIZE,W); 
	j = BUCKETS*sizeof(Int4);
	bucket = (Int4 *)ckalloc(j);
	bls = (Int4 *)ckalloc(j);
	blink = (Int4 *)ckalloc((len2+1)*sizeof(Int4));
	rcbucket = (Int4 *)ckalloc(j);
	rcbls = (Int4 *)ckalloc(j);
	rcblink = (Int4 *)ckalloc((len1+1)*sizeof(Int4));

	for (i=0; i<BUCKETS; ++i){
		bucket[i] = -1;
		rcbucket[i] = -1;
	}

	used_frag = (ulist *)ckalloc((len1+1)*sizeof(ulist));
	cut = (cut_frag **)ckalloc((len2+1)*sizeof(cut_frag *));
	end_row = (frag_pt **)ckalloc((len1+1)*sizeof(frag_pt *));
	end_col = (frag_pt **)ckalloc((len2+1)*sizeof(frag_pt *));
	col_int = (Int4 *)ckalloc((len2+1)*sizeof(Int4));
	rccol_int = (Int4 *)ckalloc((len1+1)*sizeof(Int4));
	intersect = (idb_list *)ckalloc((len1+1)*sizeof(idb_list));
	rcintersect = (idb_list *)ckalloc((len2+1)*sizeof(idb_list));
	diag_prev = (fragment **)ckalloc((len1+len2+1)*sizeof(fragment *));
	bdiag_prev = (fragment **)ckalloc((len1+len2+1)*sizeof(fragment *));
	diag_flag = (Char *)ckalloc((len1+len2+1)*sizeof(Char));
	rcdiag_flag = (Char *)ckalloc((len1+len2+1)*sizeof(Char));
	for (i=0; i<=len1; ++i) {
		used_frag[i] = NULL;
		end_row[i] = NULL;
		intersect[i] = NULL;
	}
	for (i=0; i<=len2; ++i) {
		cut[i] = NULL;
		end_col[i] = NULL;
		col_int[i] = 0;
		rcintersect[i] = NULL;
	}
	for (i=len1+len2; i>=0; --i) {
		diag_prev[i] = NULL;
		bdiag_prev[i] = NULL;
		diag_flag[i] = 0;
		rcdiag_flag[i] = 0;
	}
	/* align sequence with itself */
	if (slp1 == slp2) {
		used_frag[0] = unewList();
		uinsert(used_frag[0],0);
	}

#ifdef STATS
	/*
	fprintf(out_fp, "#:lav\n\n");

	fprintf(out_fp, "d {\n\"CHAO output with parameters:\n");
	fprintf(out_fp, "W = %ld, ",W);
	fprintf(out_fp, "K = %ld, M = 1, I = V = %.1f,\nO = %.1f, E = %.1f\"\n}\n",
		K, ((FloatLo)IR)/DIGIT, ((FloatLo)IO)/DIGIT, ((FloatLo)IE)/DIGIT);
	fprintf(out_fp, "s {\n  \"%s\" 1 %ld\n  \"%s\" 1 %ld\n}\n", sname1, len1,
		sname2, len2);
	*/
#endif
	/*
	fprintf(out_fp, "%s %ld\n%s %ld\n", sname1, len1, sname2, len2);
	fprintf(out_fp, "%ld\n",K);
	*/
	setup();
	scan();
#ifdef STATS
	/*
	fprintf(out_fp, "\nnumber_frags= %d\n",number_frags);
	*/
#endif
	free_all(len1-1,0,len2-1,diag_prev,ri,li_c,li_d);
	align = class_print(slp1, slp2);
#ifdef STATS
	/*
	fprintf(out_fp, "\nnumber_frags= %ld\n",number_frags);
	fprintf(out_fp, "deleted_frags= %ld\n",deleted_frags);
	*/
#endif
	/* free space before exit (Feb. 10, 1994) */
	list_NIL_free();
	dblist_NIL_free();
	MemFree(bucket);
	MemFree(bls);
	MemFree(blink);
	MemFree(rcbucket);
	MemFree(rcbls);
	MemFree(rcblink);
/*
	MemFree(seq1);
	MemFree(seq2);
*/
	for (i=0; i<=len1; ++i) {
		if (used_frag[i]) ufreeList(used_frag[i]);
	}
	MemFree(used_frag);
	MemFree(cut);
	MemFree(end_row);
	MemFree(end_col);
	MemFree(col_int);
	MemFree(rccol_int);
	MemFree(intersect);
	MemFree(rcintersect);
	MemFree(diag_prev);
	MemFree(bdiag_prev);
	MemFree(diag_flag);
	MemFree(rcdiag_flag);
	MemFree(pfo);
	MemFree(pbo);
	MemFree(pff);
	MemFree(pbf);

	MemFree(S);
	if(A == NULL)
		MemFree(seq1);
	if(B== NULL)
		MemFree(seq2);

	return align;
}


Int4 encoding[128];
Int4 mask;

/* hash table initialization */
void table_init(void)
{

	/* perform initializations */
	encoding['C'] = 1;
	encoding['G'] = 2;
	encoding['T'] = 3;
	mask = (1 << (W+W-2)) - 1;

}

/* build hash table */
void fbld_table(void)
{
	Int4 ecode;
	Int4 i, pos;
	CharPtr q, endp;
	Int4 s5;

	q = seq2 + sb2 - 1;
	ecode = 0L;
	for (i = 1; i < W; ++i)
		ecode = (ecode << 2) + encoding[*++q];
	pos = sb2;
	endp = seq2 + se2;
	while (++q <= endp) {
		ecode = ((ecode & mask) << 2) + encoding[*q];
		s5 = pos++;
		if (bucket[ecode] != -1) {
			blink[bls[ecode]] = s5;
		} else {
			bucket[ecode] = s5;
		}
		blink[s5] = -1;
		bls[ecode] = s5;
	}
}

/* build hash table */
void bbld_table(void)
{
	Int4 ecode;
	Int4 i, pos;
	CharPtr q, endp;
	Int4 s5;

	q = seq2 + se2 + 1;
	ecode = 0L;
	for (i = 1; i < W; ++i)
		ecode = (ecode << 2) + encoding[*--q];
	pos = se2;
	endp = seq2 + sb2;
	while (--q >= endp) {
		ecode = ((ecode & mask) << 2) + encoding[*q];
		s5 = pos--;
		if (bucket[ecode] != -1) {
			blink[bls[ecode]] = s5;
		} else {
			bucket[ecode] = s5;
		}
		blink[s5] = -1;
		bls[ecode] = s5;
	}
}

/* build hash table */
Int4 rcbld_table(Int4Ptr buc, Int4Ptr cbls, Int4Ptr cblink, CharPtr seq, Int4 b, Int4 e)
{
	Int4 ecode;
	Int4 i, pos;
	CharPtr q, endp;
	Int4 s5;

	q = seq + e + 1;
	ecode = 0L;
	for (i = 1; i < W; ++i)
		ecode = (ecode << 2) + encoding[*--q];
	pos = e;
	endp = (b-W+1 >= 0)?  seq + b-W+1 : seq;
	while (--q >= endp) {
		ecode = ((ecode & mask) << 2) + encoding[*q];
		s5 = pos--;
		if (buc[ecode] != -1) {
			cblink[cbls[ecode]] = s5;
		} else {
			buc[ecode] = s5;
		}
		cblink[s5] = -1;
		cbls[ecode] = s5;
	}
	return(ecode);
}

/* ex_table - extend the hash table */
Int4 ex_table(Int4Ptr buc, Int4Ptr cbls, Int4Ptr cblink, CharPtr seq, Int4 pos,Int4 ecode)
{
	CharPtr q;

	q = seq + (pos-W+1);
	ecode = ((ecode & mask) << 2) + encoding[*q];
	if (buc[ecode] != -1) {
		cblink[cbls[ecode]] = pos;
	} else {
		buc[ecode] = pos;
	}
	cblink[pos] = -1;
	cbls[ecode] = pos;
	return(ecode);
}

/* free_table - free the memory occupied by the table */
void free_table(Int4Ptr buc)
{
	Int4 i;

	for (i = 0; i < BUCKETS; ++i) {
		buc[i] = -1;
	}
}

/* update_active - update active region */
void update_active(list cri, db_list cli_c, db_list cli_d, Int4 l)
{
	/* update left influence active regions */
	db_reset_pos(cli_c);
	db_reset_pos(cli_d);
	resolve(cli_c,cli_d,intersect,diag_flag,col_int,l,pt_lscore); 
	db_reset_pos(cli_c);
	db_reset_pos(cli_d);
	l_merge(cli_c,cli_d,intersect,diag_flag,col_int,end_row[l],l,pt_lscore);

	/* update right influence active regions */
	reset_pos(cri);
	r_survive(end_row[l]);
	r_merge(cri,end_row[l],l);

	if (intersect[l]) {
		idb_freeList(intersect[l]);
		intersect[l] = NULL;
	}
	end_row[l] = NULL;
}

/* rcupdate_active - update active region */
void rcupdate_active(list cri, db_list cli_c, db_list cli_d, Int4 l)
{
	/* update left influence active regions */
	db_reset_pos(cli_c);
	db_reset_pos(cli_d);
	resolve(cli_c,cli_d,rcintersect,rcdiag_flag,rccol_int,l,rcpt_lscore); 
	db_reset_pos(cli_c);
	db_reset_pos(cli_d);
	l_merge(cli_c,cli_d,rcintersect,rcdiag_flag,rccol_int,end_col[l],l,rcpt_lscore);

	/* update right influence active regions */
	reset_pos(cri);
	r_survive(end_col[l]);
	rcr_merge(cri,end_col[l],l);

	if (rcintersect[l]) {
		idb_freeList(rcintersect[l]);
		rcintersect[l] = NULL;
	}
	end_col[l] = NULL;
}

/* rotupdate_active - update active regions orthogonally */
void rotupdate_active(frag_pt PNTR rcopy, Int4 col, Int4 row)
{
	frag_pt PNTR r;

	reset_pos(ri);
	r = otr_survive(rcopy,col);
	rotr_merge(r);
	r = otl_survive(rcopy,col);
	otl_merge(r,col,row,li_c,li_d,rcintersect,rccol_int,
		  rcdiag_flag,rcpos_diag,rotl_win);
}

/* cotupdate_active - update active regions orthogonally */
void cotupdate_active(frag_pt PNTR rcopy, Int4 col, Int4 row)
{
	frag_pt PNTR r;

	reset_pos(bri);
	r = otr_survive(rcopy,col);
	cotr_merge(r);
	r = otl_survive(rcopy,col);
	otl_merge(r,col,row,bli_c,bli_d,intersect,col_int,
		  diag_flag,pos_diag,cotl_win);
}

/* free_all - free all useless space */
void free_all(Int4 i, Int4 j1, Int4 j2, fragment PNTR cdiag_prev[], list cri, db_list cli_c, db_list cli_d)
{
	Int4 l, m;
	fragment PNTR df;

	m = len1 + (j2 - i);
	for (l = len1+(j1-i); l <= m; ++l) 
		if (df = cdiag_prev[l]) {
			decre(df);
			cdiag_prev[l] = NULL;
		}
	freeList(cri);
	db_freeList(cli_c);
	db_freeList(cli_d);
}

void scan(void)	/* scan sequence 1 */
{
	Int4 ecode;
	Int4 i, l, m, pos;
	CharPtr q;
	Int4 s5;
	fragment PNTR df;

	q = seq1 - 1;
	ecode = 0L;
	for (i = 1; i < W; ++i)
		ecode = (ecode << 2) + encoding[*++q];
	pos = 0;
	while (*++q) {
		reset_pos(ri);
		db_reset_pos(li_c);
		db_reset_pos(li_d);
		ecode = ((ecode & mask) << 2) + encoding[*q];
		for (s5 = bucket[ecode]; s5 != -1; s5 = blink[s5])
			found(pos, s5);
		/* free part of space occupied by diag_prev[] */
		if (pos % GC_ROW == 0) {
		   m = len1 - pos + len2;
		   for (l = len1-pos; l <= m; ++l) {
			if ((df = diag_prev[l]) && 
			(df->score - IR * (pos - df->i - df->k) < 0)) {
				decre(df);
				diag_prev[l] = NULL;
			}
		   }
		}
		/* update active regions */
		update_active(ri,li_c,li_d,pos);
		if (df = diag_prev[len1+len2-1-pos]) {
			diag_prev[len1+len2-1-pos] = NULL;
			decre(df);
		}
		++pos;
	}
	for (l = pos; l < len1; ++l) update_active(ri,li_c,li_d,l);
	m = len2;	/* len1 + (len2-len1) */
	for (l = len1+(len2-1-pos); l>m; --l)
		if (df=diag_prev[l]) {
			decre(df);
			diag_prev[l]=NULL;
		}
}

void fscan_init(void)
{
	Int4 i,j;

	ri = newList();
	li_c = db_newList();
	li_d = db_newList();
	for (i=sb2; i<=se2; ++i) col_int[i] = 0;
	j = len1 + (se2-sb1);
	for (i = len1 + (sb2-mid1); i <= j; ++i) diag_flag[i]=0;
	cur_end = mid1;
}

void bscan_init(void)
{
	Int4 i,j;

	bri = newList();
	bli_c = db_newList();
	bli_d = db_newList();
	j=len2-sb2;
	for (i=len2-se2; i<=j; ++i) col_int[i] = 0;
	j=len2-(sb2-se1);
	for (i = len2-(se2-mid2); i<=j; ++i) diag_flag[i]=0;
	cur_end = len1 - mid2;
}

void fscan(void)	/* scan sequence 1 */
{
	Int4 ecode;
	Int4 i, pos;
	CharPtr q;
	Int4 s5;
	fragment PNTR df;

	q = seq1 + sb1 - 1;
	ecode = 0L;
	for (i = 1; i < W; ++i)
		ecode = (ecode << 2) + encoding[*++q];
	pos = sb1;
	while (*++q && pos <= mid1) {
		reset_pos(ri);
		db_reset_pos(li_c);
		db_reset_pos(li_d);
		ecode = ((ecode & mask) << 2) + encoding[*q];
		for (s5 = bucket[ecode]; s5 != -1; s5 = blink[s5])
		 	ffound(pos, s5);
		/* update active regions */
		update_active(ri,li_c,li_d,pos);
		if ((pos != mid1) && (df = diag_prev[len1+se2-pos])) {
			decre(df);
			diag_prev[len1+se2-pos] = NULL;
		}

		++pos;
	}
}

void bscan(void) /* scan sequence 1 */
{
	Int4 ecode;
	Int4 i, pos, dd;
	CharPtr q;
	Int4 s5;
	fragment PNTR df;

	q = seq1 + se1 + 1;
	ecode = 0L;
	for (i = 1; i < W; ++i)
		ecode = (ecode << 2) + encoding[*--q];
	pos = se1;
	while (*--q && pos >= mid2) {
		reset_pos(bri);
		db_reset_pos(bli_c);
		db_reset_pos(bli_d);
		ecode = ((ecode & mask) << 2) + encoding[*q];
		for (s5 = bucket[ecode]; s5 != -1; s5 = blink[s5])
			bfound(pos, s5);
		/* update active regions */
		update_active(bri,bli_c,bli_d,len1-pos);
		dd = len2-sb2+pos;
		if ((pos != mid2) && (df = bdiag_prev[dd])) {
			bdiag_prev[dd] = NULL;
			decre(df);
		}

		--pos;
	}
	while (pos >= mid2) {
		/* update active regions */
		update_active(bri,bli_c,bli_d,len1-pos);
		dd = len2-sb2+pos;
		if ((pos != mid2) && (df = bdiag_prev[dd])) {
			bdiag_prev[dd] = NULL;
			decre(df);
		}
		--pos;
	}
}

/* found a matching 5-mer at (i,j) */
void found(Int4 i, Int4 j)
{
	Int4 k;
	fragment PNTR f;

	if (i && j && seq1[i-1] == seq2[j-1])
		return;
	if (usearch(used_frag[i],j)) return;
	for (k = W; seq1[i+k] && seq2[j+k] && seq1[i+k] == seq2[j+k]; ++k)
		;
	f = (fragment *) ckalloc(sizeof(fragment));
	f->i = i;
	f->j = j;
	f->k = k;
	f->ref = 0;

	local(f);

	enter_endrow(f);
	++number_frags;
	++total_frags;
}

/* rfscan - recompute the given area: [t,b]X[l,r] */
void rfscan(Int4 t, Int4 b, Int4 l, Int4 r)
{
	Int4 ecode;
	Int4 i, pos;
	CharPtr q;
	Int4 s5;
	fragment PNTR df;

	if (b-t+1 < W || r-l+1 < W) return;
	sb1 = t;
	se1 = mid1 = b;
	sb2 = l;
	se2 = r;
	free_table(bucket);
	fbld_table();
	fscan_init();

	q = seq1 + sb1 - 1;
	ecode = 0L;
	for (i = 1; i < W; ++i)
		ecode = (ecode << 2) + encoding[*++q];
	pos = sb1;
	while (*++q && pos <= mid1) {
		reset_pos(ri);
		db_reset_pos(li_c);
		db_reset_pos(li_d);
		ecode = ((ecode & mask) << 2) + encoding[*q];
		for (s5 = bucket[ecode]; s5 != -1; s5 = blink[s5])
		 	rffound(pos, s5);
		/* update active regions */
		update_active(ri,li_c,li_d,pos);
		if (df = diag_prev[len1+se2-pos]) {
			decre(df);
			diag_prev[len1+se2-pos] = NULL;
		}
		++pos;
	}
	while (pos<=mid1) {
		update_active(ri,li_c,li_d,pos);
		if (df = diag_prev[len1+se2-pos]) {
			decre(df);
			diag_prev[len1+se2-pos] = NULL;
		}
		++pos;
	}
	free_all(mid1,sb2,se2,diag_prev,ri,li_c,li_d);
}

/* rffound a matching 5-mer at (i,j) */
void rffound(Int4 i, Int4 j)
{
	Int4 k;
	fragment PNTR f;

	if (i && j && seq1[i-1] == seq2[j-1])
		return;
	if (usearch(used_frag[i],j)) return;
	for (k = W; seq1[i+k] && seq2[j+k] && seq1[i+k] == seq2[j+k]; ++k);
	if (i+k-1 > se1 || j+k-1 > se2) return;  /* out of boundary */
	f = (fragment *) ckalloc(sizeof(fragment));
	f->i = i;
	f->j = j;
	f->k = k;
	f->ref = 0;

	local(f);

	enter_endrow(f);
	++number_frags;
}

/* enter_endrow - enter end_point of the given fragment */
void enter_endrow(fragment PNTR f)
{
	Int4 row,col;
	frag_pt PNTR e, PNTR e1, PNTR e2;

	row = f->i + f->k - 1;
	col = f->j + f->k - 1;
	/* put each fragment row by row according to its end point */
	e = (frag_pt *) ckalloc(sizeof(frag_pt));
	e->fp = f;
	e->col = col;

	if (end_row[row]) { 	/* end_row[row] isn't empty */
		e1 = end_row[row];
		e2 = e1->link;
		if (e1->col > e->col) {	/* e contains the smallest column # */
			e->link = e1;
			end_row[row] = e;
		} else {
			while (e2 && e2->col < e->col) {
				e1 = e2;
				e2 = e2->link;
			}
			e1->link = e;
			e->link = e2;
		}
	} else {  /* end_row[row] is empty */
		e->link = end_row[row];
		end_row[row] = e;
	}
}

/* enter_endcol - enter end_point of the given fragment */
void enter_endcol(fragment PNTR f)
{
	Int4 row,col;
	frag_pt PNTR e, PNTR e1, PNTR e2;

	row = f->i + f->k - 1;
	col = f->j + f->k - 1;
	/* put each fragment row by row according to its end point */
	e = (frag_pt *) ckalloc(sizeof(frag_pt));
	e->fp = f;
	e->col = row;
	if (end_col[col]) { 	/* end_col[col] isn't empty */
		e1 = end_col[col];
		e2 = e1->link;
		if (e1->col > e->col) {	/* e contains the smallest column # */
			e->link = e1;
			end_col[col] = e;
		} else {
			while (e2 && e2->col < e->col) {
				e1 = e2;
				e2 = e2->link;
			}
			e1->link = e;
			e->link = e2;
		}
	} else {  /* end_col[col] is empty */
		e->link = end_col[col];
		end_col[col] = e;
	}
}

/* ffound a matching 5-mer at (i,j) */
void ffound(Int4 i, Int4 j)
{
	Int4 k;
	fragment PNTR f;

	if (i && j && seq1[i-1] == seq2[j-1])
		return;
	if (usearch(used_frag[i],j)) return;
	for (k = W; seq1[i+k] && seq2[j+k] && seq1[i+k] == seq2[j+k]; ++k);
	if (i+k-1 > se1 || j+k-1 > se2) return;  /* out of boundary */
	f = (fragment *) ckalloc(sizeof(fragment));
	f->i = i;
	f->j = j;
	f->k = k;
	f->ref = 0;

	++number_frags;
	if (i+k-1>mid1) {
		cut[mid1+j-i] = (cut_frag *) ckalloc(sizeof(cut_frag));
		cut[mid1+j-i]->ff = f;
		calign(pfo,diag_prev,ri,li_c,li_d,f);
		return;
	} else align(pfo,diag_prev,ri,li_c,li_d,f);

	enter_endrow(f);
}

/* bfound a matching 5-mer at (i,j) */
void bfound(Int4 i, Int4 j)
{
	Int4 k;
	fragment PNTR f;

	if (seq1[i+1] && seq2[j+1] && seq1[i+1] == seq2[j+1])
		return;
	/*
	for (k = W; seq1[i-k] && seq2[j-k] && seq1[i-k] == seq2[j-k]; ++k);
	*/
	for (k = W; i-k>=0 && j-k>=0 && seq1[i-k] == seq2[j-k]; ++k);
	if (usearch(used_frag[i-k+1],j-k+1)) return;
	if (i-k+1 < sb1 || j-k+1 < sb2) return;  /* out of boundary */
	f = (fragment *) ckalloc(sizeof(fragment));
	f->i = len1 - i;
	f->j = len2 - j;
	f->k = k;
	f->ref = 0;

	++number_frags;
	if (i-k+1 < mid2) {
		cut[mid1+(j-i)]->bf = f;
		calign(pbo,bdiag_prev,bri,bli_c,bli_d,f);
		return;
	} else align(pbo,bdiag_prev,bri,bli_c,bli_d,f);

	enter_endrow(f);
}

/* setup - initialization */
void setup(void)
{
	/* skip lists initialization */
	init();
	db_init();

	/* hash table initialization */
	table_init();

	/* pseudo fragments initialization */
	pfo = (fragment *) ckalloc(sizeof(fragment));
	pfo->k = 0;
	pfo->score = 0;
	pfo->bgf = pfo;
	pbo = (fragment *) ckalloc(sizeof(fragment));
	pbo->k = 0;
	pbo->score = 0;
	pbo->bgf = pbo;
	pff = (fragment *) ckalloc(sizeof(fragment));
	pff->k = 0;
	pbf = (fragment *) ckalloc(sizeof(fragment));
	pbf->k = 0;

	/* initialization */
	ri = newList();
	li_c = db_newList();
	li_d = db_newList();
	sb2 = 0;
	se2 = len2 - 1;
	cur_end = len1 - 1;
	fbld_table();
	pfo->ref = 1;
	pbo->ref = 1;
	offset = offset1 = len1;
	number_frags = 0;
	deleted_frags = 0;
	output_frags = 0;
	total_frags = 0;
}

void global_setup(fragment PNTR bf, fragment PNTR ef)
{
	sb1 = bf->i + bf->k;
	se1 = ef->i - 1;
	sb2 = bf->j + bf->k;
	se2 = ef->j - 1;
	mid1 = (sb1 + se1) / 2;
	mid2 = mid1 + 1;
	pfo->i = sb1;
	pfo->j = sb2;
	pbo->i = len1-se1;
	pbo->j = len2-se2;
}

/* global - find the best global alignment in a given area */
void global(fragment PNTR bf, fragment PNTR ef, Int4 gs, SeqLocPtr slp1,  SeqLocPtr slp2, SeqAlignPtr align)
{
	Int4 i, ll;
	fragment PNTR fdf, PNTR bdf;
	fragment PNTR fsb, PNTR fse = NULL, PNTR bsb, PNTR bse = NULL, PNTR mf;
	Int4 js = 0;

	global_setup(bf,ef);
	if ((se1-sb1+1 < W) || (se2-sb2+1 < W))
		return;
	free_table(bucket);
	fbld_table();
	fscan_init();
	fscan();
	free_table(bucket);
	bbld_table();
	bscan_init();
	bscan();
	pf_set(pff,mid2,se2+1);
	pf_set(pbf,len1-mid1,len2-se2);
	mf = NULL;

	reset_pos(bri);
	db_reset_pos(bli_c);
	db_reset_pos(bli_d);
	ll = se2-sb2+1;
	for (i = 0; i <= ll; ++i) {
		if (cut[pff->j]) {
			fdf = cut[pff->j]->ff;
			bdf = cut[pff->j]->bf;
			js = s_Join(fdf,bdf)- (Int4) (DIGIT*(bdf->k));

			if (js == gs) {
				mf = fdf;
				incre(mf);
				fse = fdf->bgf;
				incre(fse);
				bse = bdf->bgf;
				incre(bse);
				break;
			}
		} 
		reset_pos(ri);
		db_reset_pos(li_c);
		db_reset_pos(li_d);
		malign(pfo,diag_prev,ri,li_c,li_d,pff);
		malign(pbo,bdiag_prev,bri,bli_c,bli_d,pbf);
		js = s_Join(pff,pbf);
		if ((pff->j-pff->i != (pff->bgf)->j-(pff->bgf)->i) &&
			(pbf->j-pbf->i != (pbf->bgf)->j-(pbf->bgf)->i))
				js = js + IO;
		if (js == gs) {
			fse = pff->bgf;
			bse = pbf->bgf;
			incre(fse);
			incre(bse);
			break;
		}
		--(pff->j);
		++(pbf->j);
	}
	if (js != gs) { /* type 2 connection */
		reset_pos(bri);
		db_reset_pos(bli_c);
		db_reset_pos(bli_d);
		pf_set(pff,mid2,se2+1);
		pf_set(pbf,len1-mid1,len2-se2);
		ll = se2-sb2+1;
		for (i = 0; i <= ll; ++i) {
			reset_pos(ri);
			db_reset_pos(li_c);
			db_reset_pos(li_d);
			malign1(pfo,ri,li_c,li_d,pff);
			malign1(pbo,bri,bli_c,bli_d,pbf);
			js = s_Join(pff,pbf);
			if ((pff->j-pff->i != (pff->bgf)->j-(pff->bgf)->i) &&
				(pbf->j-pbf->i != (pbf->bgf)->j-(pbf->bgf)->i))
					js = js + IO;
			if (js == gs) {
				fse = pff->bgf;
				bse = pbf->bgf;
				incre(fse);
				incre(bse);
				break;
			}
			--(pff->j);
			++(pbf->j);
		}
		if (js != gs) {
#ifdef STATS
			/*
			fprintf(out_fp, "wrong!!! \n");
			fprintf(out_fp, "sb1=%ld se1=%ld mid1=%ld \n",sb1,se1,mid1);
			fprintf(out_fp, "gs=%ld, js=%ld\n",gs,js);
			*/
#endif
			free_all(mid1,sb2,se2,diag_prev,ri,li_c,li_d);
			free_all(len1-mid2,len2-se2,len2-sb2,bdiag_prev,bri,bli_c,bli_d);
			return;
		}
	}
	fsb = fse->bgf;
	if (fse == pfo) fse = NULL;
	if (fsb == pfo || fsb == fse) fsb = NULL;
	bsb = bse->bgf;
	if (bse == pbo) bse = NULL;
	else {
		bse->i = len1 - bse->i - bse->k + 1;
		bse->j = len2 - bse->j - bse->k + 1;
	}
	if (bsb == pbo || bsb == bse) bsb = NULL;
	else {
		bsb->i = len1 - bsb->i - bsb->k + 1;
		bsb->j = len2 - bsb->j - bsb->k + 1;
	}
	for (i=sb2; i<=se2; ++i)
		if (cut[i]) {
			decre(cut[i]->ff);
			decre(cut[i]->bf);
			MemFree(cut[i]);
			cut[i]=NULL;
		}
	free_all(mid1,sb2,se2,diag_prev,ri,li_c,li_d);
	free_all(len1-mid2,len2-se2,len2-sb2,bdiag_prev,bri,bli_c,bli_d);
	if (fsb) {
		align->segs = frag_print(fsb, slp1, slp2, align->segs);
		gs = fse->score-fsb->score-(Int4) (DIGIT*fse->k);
		global(fsb,fse,gs, slp1, slp2, align);
	}
	if (fse) {
		align->segs = frag_print(fse, slp1, slp2, align->segs);
		decre(fse);
	}
	if (mf) {
		align->segs = frag_print(mf, slp1, slp2, align->segs);
		decre(mf);
	}
	if (bse) {
		align->segs = frag_print(bse, slp1, slp2, align->segs);
		if (bsb) {
			gs = bse->score-bsb->score-(Int4) (DIGIT*bse->k);
			global(bse,bsb,gs, slp1, slp2, align);
			align->segs = frag_print(bsb, slp1, slp2, align->segs);
		}
		decre(bse);
	}
}

/* join - return the cost after join */
static Int4 s_Join(fragment PNTR ff, fragment PNTR bf)
{
	return(ff->score + bf->score);
}

Int4 tt,bb,ll,rr,tt1,ll1; /* current recomputation area */
Int4 ecol,erow;
Int4 row_code, col_code;

/* recom_setup - initialization for recomputation */
void recom_setup(kbclass PNTR sp)
{
	Int4 i,j;

	erow = tt1 = tt = sp->t;
	bb = sp->b;
	ecol = ll1 = ll = sp->l;
	rr = sp->r;
	free_table(bucket);
	free_table(rcbucket);
	col_code = rcbld_table(bucket,bls,blink,seq2,ll1,rr);
	row_code = rcbld_table(rcbucket,rcbls,rcblink,seq1,tt1,bb);

	/* Lists initialization for column-ward recomputation */
	ri = newList();
	li_c = db_newList();
	li_d = db_newList();

	/* Lists initialization for row-ward recomputation */
	bri = newList();
	bli_c = db_newList();
	bli_d = db_newList();

	for (i=len2-rr; i<=len2; ++i) col_int[i] = 0;
	for (i=len1-bb; i<=len1; ++i)  rccol_int[i] = 0;
	j = len2+bb;
	for (i=len2-rr; i<=j; ++i) diag_flag[i] = 0;
	j = len1+rr;
	for (i=len1-bb; i<=j; ++i) rcdiag_flag[i] = 0;

}

/* recompute - recompute the scores of the area affected by removing sp */
void recompute(kbclass PNTR sp)
{
	recom_setup(sp);
	offset = cur_end = len2;	/* len2 - 0 */
	offset1 = cur_end1 = len1; /* len1 - 0 */
	trcscan();
	for (;;) {
		while(tt>erow || ll>ecol) {
			offset = cur_end = len1;
			offset1 = cur_end1 = len2;
			while (tt>erow) ex_row();
			offset = cur_end = len2;
			offset1 = cur_end1 = len1;
			while (ll>ecol) ex_col();
		}
		if (disjoint() || (tt==0 && ll==0)) break;
	}
	rcfree();
	offset = offset1 = len1;
#ifdef STATS
	/*
	fprintf(out_fp, "before tt=%ld, bb=%ld, ll=%ld, rr=%ld\n",tt,bb,ll,rr);
	fprintf(out_fp, "best_score= %ld\n",best_score);
	*/
#endif
	if (best_score > minscoreS()) {
		syn_area();
#ifdef STATS
		/*
		fprintf(out_fp, "after tt=%ld, bb=%ld, ll=%ld, rr=%ld\n",tt,bb,ll,rr);
		*/
#endif
		rfscan(tt,bb,ll,rr);
	}
	best_score = 0;
}

/* disjoint - return TRUE if [tt,bb]X[ll,rr]-[tt1,bb]X[ll1,rr] shares
	      no vertices with the areas bounded by S */
Int4 disjoint(void)
{
	kbclass PNTR sp;
	Int4 i,j;
	Int4 endj;

	for (i=0; i<lastS; ++i) {
		sp = S[i];
		if (sp->t <= bb && sp->b >= tt && sp->l <= rr && sp->r >= ll
		   && (sp->t < tt1 || sp->l < ll1)) {
		   if (sp->t < tt1) tt1 = sp->t;
		   if (sp->l < ll1) ll1 = sp->l;
		   if (erow > tt1) erow = tt1;
		   if (ecol > ll1) ecol = ll1;

		   /* extend erow and ecol */
		   endj = len2 + (bb-ll);
		   for (j=len2+(tt-rr); j<=endj; ++j)
			   ractive_ext(diag_prev[j]);
		   sreset_pos(ri);
		   sreset_pos(bri);
		   db_sreset_pos(li_c);
		   db_sreset_pos(li_d);
		   db_sreset_pos(bli_c);
		   db_sreset_pos(bli_d);
		   while(next_key(ri)!=-1) ractive_ext(snext(ri));
		   while(next_key(bri)!=-1) ractive_ext(snext(bri));
		   while(db_next_key(li_c)!=-1) lactive_ext(db_snext(li_c));
		   while(db_next_key(li_d)!=-1) lactive_ext(db_snext(li_d));
		   while(db_next_key(bli_c)!=-1) lactive_ext(db_snext(bli_c));
		   while(db_next_key(bli_d)!=-1) lactive_ext(db_snext(bli_d));

		   return FALSE;
	   }
	}
	return TRUE;
}

/* syn_area - synchronize [tt,bb]X[ll,rr] with all other areas */
void syn_area(void)
{
	kbclass PNTR sp;
	Int4 i,si,sj;
	Int4 flag;

	flag = TRUE;
	while (flag) {
		flag = FALSE;
		for (i=0; i<lastS; ++i) {
			sp = S[i];
			si = (sp->first)->i;
			sj = (sp->first)->j;
			if (si <= bb && sp->b >= tt && sj <= rr && sp->r >= ll
		   	    && (si < tt || sj < ll)) {
		   		if (si < tt) tt = si;
		   		if (sj < ll) ll = sj;
				flag = TRUE;
			}
		}
	}
}

/* ex_row - extend one row (incrementally) */
void ex_row(void)
{
	Int4 s5;
	Int4 tpos;

	--tt;
	if (tt-W+1 >= 0) {
		row_code = ex_table(rcbucket,rcbls,rcblink,seq1,tt,row_code);
		reset_pos(bri);
		db_reset_pos(bli_c);
		db_reset_pos(bli_d);
		for (s5 = bucket[row_code]; s5 != -1; s5 = blink[s5])
			rcfound(tt, s5);
	}
	/* update active regions */
	tpos = len1-tt;
	end_row[tpos] = tailor(end_row[tpos],len2-ll);
	rotupdate_active(end_row[tpos],len2-ll,tpos);
	update_active(bri,bli_c,bli_d,tpos);
}

/* ex_col - extend one col (incrementally) */
void ex_col(void)
{
	Int4 s5;
	Int4 tpos;

	--ll;
	if (ll-W+1 >= 0) {
		col_code = ex_table(bucket,bls,blink,seq2,ll,col_code);
		reset_pos(ri);
		db_reset_pos(li_c);
		db_reset_pos(li_d);
		for (s5 = rcbucket[col_code]; s5 != -1; s5 = rcblink[s5])
			trcfound(s5,ll);
	}
	/* update active regions */
	tpos = len2-ll;
	end_col[tpos] = tailor(end_col[tpos],len1-tt);
	cotupdate_active(end_col[tpos],len1-tt,tpos);
	rcupdate_active(ri,li_c,li_d,tpos);
}


/* rcfound a matching 5-mer at (i,j) */
void rcfound(Int4 i, Int4 j)
{
	Int4 k;
	fragment PNTR f;

	if (seq1[i+1] && seq2[j+1] && seq1[i+1] == seq2[j+1])
		return;
	/* modified (Feb., 1994) */
	/*
	for (k = W; seq1[i-k] && seq2[j-k] && seq1[i-k] == seq2[j-k]; ++k);
	*/
	for (k = W; i-k>=0 && j-k>=0 && seq1[i-k] == seq2[j-k]; ++k);
	if (usearch(used_frag[i-k+1],j-k+1)) return;
	f = (fragment *) ckalloc(sizeof(fragment));
	f->i = len1 - i;
	f->j = len2 - j;
	f->k = k;
	f->ref = 0;

	rclocal(f,f->j,pos_diag(f),bri,bli_c,bli_d);
	if ((f->j + f->k - 1 <= len2-ll) || (f->i + f->k - 1 > len1-tt))
		enter_endrow(f);
	if (f->j + f->k - 1 > len2-ll) enter_endcol(f); 
	++number_frags;
}

/* trcscan - scan seq1 and seq2 backward (columnwise) */
void trcscan(void)
{
	Int4 ecode;
	Int4 i, pos, tpos;
	CharPtr q;
	Int4 s5;

	q = seq2 + rr + 1;
	ecode = 0L;
	for (i = 1; i < W; ++i)
		ecode = (ecode << 2) + encoding[*--q];
	pos = rr;
	while (*--q && pos >= ll) {
		reset_pos(ri);
		db_reset_pos(li_c);
		db_reset_pos(li_d);
		ecode = ((ecode & mask) << 2) + encoding[*q];
		for (s5 = rcbucket[ecode]; s5 != -1; s5 = rcblink[s5])
			trcfound(s5,pos);
		/* update active regions */
		tpos = len2-pos;
		end_col[tpos] = tailor(end_col[tpos],len1-tt);
		cotupdate_active(end_col[tpos],len1-tt,tpos);
		rcupdate_active(ri,li_c,li_d,tpos);

		--pos;
	}
}

/* trcfound a matching 5-mer at (i,j) */
void trcfound(Int4 i, Int4 j)
{
	Int4 k;
	fragment PNTR f;

	if (seq1[i+1] && seq2[j+1] && seq1[i+1] == seq2[j+1])
		return;
	/*
	for (k = W; seq1[i-k] && seq2[j-k] && seq1[i-k] == seq2[j-k]; ++k);
	*/
	for (k = W; i-k>=0 && j-k>=0 && seq1[i-k] == seq2[j-k]; ++k);
	if (usearch(used_frag[i-k+1],j-k+1)) return;
	f = (fragment *) ckalloc(sizeof(fragment));
	f->i = len1 - i;
	f->j = len2 - j;
	f->k = k;
	f->ref = 0;

	rclocal(f,f->i,rcpos_diag(f),ri,li_c,li_d);
	if ((f->i + f->k - 1<= len1-tt) || (f->j + f->k - 1> len2-ll))
		enter_endcol(f);
	if (f->i + f->k - 1> len1-tt) enter_endrow(f); 
	++number_frags;
}

void rclocal(fragment PNTR f, Int4 cur_col, Int4 cur_diag, list cri, db_list cli_c, db_list cli_d)
{
	Int4 max_score, cs;
	fragment PNTR pf, PNTR cf, PNTR df;
	Int4 col, diag, ocur_diag;

	max_score = 0;
	f->bgf = f;

	/* compute the best left influence */
	cf = db_rsearch_next(cli_c, cur_col);
	col = db_cur_key(cli_c);
	df = db_lsearch_next(cli_d, cur_diag);
	diag = db_cur_key(cli_d);
	pf = ((cur_col - col > cur_diag-diag)&&(diag != -1)) ? df : cf;
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score) {
			max_score = cs;
			f->bgf = pf->bgf;
			incre(f->bgf);
		}
	}
	
	/* compute the best mismatch */
	ocur_diag = pos_diag(f);
	if (pf = diag_prev[ocur_diag]) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score) {
			max_score = cs;
			if (f != f->bgf) decre(f->bgf);
			f->bgf = pf->bgf;
			incre(f->bgf);
		}
		decre(pf);
	}
	diag_prev[ocur_diag] = f;
	incre(f);

	/* compute the best right influence */
	pf = rsearch_next(cri,cur_diag);
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score) {
			max_score = cs;
			if (f != f->bgf) decre(f->bgf);
			f->bgf = pf->bgf;
			incre(f->bgf);
		}
	}

	f->score = max_score + (Int4) (DIGIT * f->k);
	if (f->score > best_score) best_score = f->score;
	lactive_ext(f);
}

/* lactive_ext - check if we need to extend the area reached by active
		    fragments (for all cases) */
void lactive_ext(fragment PNTR f)
{
	Int4 endi,endj,ai,aj,ai1,aj1,cur_score;

	if (!f) return;
	/* check if we need to extend the area reached by active fragments */
	if ((f->bgf)->i <= len1-tt1 && (f->bgf)->j <= len2-ll1) {
		endi = f->i + f->k - 1;
		endj = f->j + f->k - 1;
		cur_score = f->score;
		ai = endi + cur_score / IR + 2;
		/*
		ai1 = endi + (cur_score-IO)/IE + 1;
		*/
		ai1 = endi + cur_score/IE + 2;
		if (ai1 > ai) ai = ai1;
		if (ai > len1) ai = len1;
		ai = len1-ai;
		if (ai < erow) erow = ai;
		aj = endj + cur_score / IR + 2;
		/*
		aj1 = endj + (cur_score-IO)/IE + 1;
		*/
		aj1 = endj + cur_score/IE + 2;
		if (aj1 > aj) aj = aj1;
		if (aj > len2) aj = len2;
		aj = len2-aj;
		if (aj < ecol) ecol = aj;
	}
}

/* ractive_ext - check if we need to extend the area reached by active
		    fragments (for right influence list or mismatch) */
void ractive_ext(fragment PNTR f)
{
	Int4 endi,endj,ai,aj,cur_score;

	if (!f) return;
	/* check if we need to extend the area reached by active fragments */
	if ((f->bgf)->i <= len1-tt1 && (f->bgf)->j <= len2-ll1) {
		endi = f->i + f->k - 1;
		endj = f->j + f->k - 1;
		cur_score = f->score;
		ai = endi + cur_score / IR + 2;
		if (ai > len1) ai = len1;
		ai = len1-ai;
		if (ai < erow) erow = ai;
		aj = endj + cur_score / IR + 2;
		if (aj > len2) aj = len2;
		aj = len2-aj;
		if (aj < ecol) ecol = aj;
	}
}

void rcfree(void)
{
	Int4 i,j;
	frag_pt PNTR p, PNTR q;

	for (i=len1-tt+1; i<=len1; ++i) {
		if (intersect[i]) {
			idb_freeList(intersect[i]);
			intersect[i] = NULL;
		}
		if (end_row[i]) {
			for (p=end_row[i]; p; q=p, p=p->link, MemFree(q));
			end_row[i] = NULL;
		}
	}
	for (i=len2-ll+1; i<=len2; ++i) {
		if (rcintersect[i]) {
			idb_freeList(rcintersect[i]);
			rcintersect[i] = NULL;
		}
		if (end_col[i]) {
			for (p=end_col[i]; p; q=p, p=p->link, MemFree(q));
			end_col[i] = NULL;
		}
	}
	j = len2-ll+bb;
	for (i=len2-rr+tt; i<=j; ++i)
		if (diag_prev[i]) {
			decre(diag_prev[i]);
			diag_prev[i]=NULL;
		}
	freeList(ri);
	db_freeList(li_c);
	db_freeList(li_d);
	freeList(bri);
	db_freeList(bli_c);
	db_freeList(bli_d);
}

/*static SeqAlignPtr link_align(SeqAlignPtr align, SeqAlignPtr head)
{
	if(head == NULL)
		return align;
	while(head->next != NULL)
		head = head->next;
	head->next = align;
	return align;

}*/

/* printS - print the given class */
SeqAlignPtr printS(kbclass PNTR sp, SeqLocPtr slp1, SeqLocPtr slp2, SeqAlignPtr head)
{
	Int4 gs;
	SeqAlignPtr align = NULL;
	ScorePtr scp;

#ifdef STATS
	/*
	fprintf(out_fp, "first:  ");
	*/

	align->segs = frag_print(sp->first, slp1, slp2, align->segs);

	/*
	fprintf(out_fp, "last:   ");
	*/

	align->segs = frag_print(sp->last, slp1, slp2, align->segs);

	/*
	fprintf(out_fp, "score = %.1f\n", ((FloatLo) sp->score)/DIGIT);
	fprintf(out_fp, "t = %ld, b = %ld, l = %ld, r = %ld\n\n", sp->t,sp->b,sp->l,sp->r);
	*/
#endif
#ifdef STATS
	old_i_end = 0;
	/*
	fprintf(out_fp, "a {\n  s %.1f\n", ((FloatLo) sp->score)/DIGIT);
	fprintf(out_fp, "  b %ld %ld\n  e %ld %ld\n", sp->first->i, sp->first->j,
		sp->last->i + sp->last->k - 1, sp->last->j + sp->last->k - 1);
	*/
#endif

	align = SeqAlignNew();
	align->type = 2;	/*diag*/
	align->segtype = 1;
	align->segs = NULL;

	scp = ScoreNew();
	scp->choice = 2;                /**it is a real number**/
      	scp->value.realvalue =((FloatHi) sp->score)/DIGIT; 
      	scp->next = NULL;
      	align->score = scp;


	/*
	fprintf(out_fp, "%.1f\n", ((FloatHi) sp->score)/DIGIT);
	*/
	/*
	align->segs = frag_print(sp->first, slp1, slp2, align->segs);
	align->segs = frag_print(sp->last, slp1, slp2, align->segs);
	*/
	if (sp->last) {
	   if (sp->last != (sp->last)->bgf) {
		align->segs = frag_print((sp->last)->bgf, slp1, slp2, align->segs);
		gs = sp->score - (Int4) (DIGIT * (((sp->last)->bgf)->k + (sp->last)->k));
		global((sp->last)->bgf,sp->last,gs, slp1, slp2, align);
	   }
	   align->segs = frag_print(sp->last, slp1, slp2, align->segs);
	   decre(sp->last);
	}
#ifdef STATS
	/*
	fprintf(out_fp, "  l %ld %ld %ld %ld 0.0\n}\n",
		old_i_start, old_j_start, old_i_end, old_j_end);
	*/
#endif
	if(head == NULL)
		head = align;
	else
		link_align(align, head);
	return head;
}

/* class_print - print K best local alignments */
SeqAlignPtr class_print(SeqLocPtr slp1, SeqLocPtr slp2)
{
	Int4 i,j;
	kbclass PNTR sp;
	SeqAlignPtr align;

	j = lastS;
	align = NULL;

	for (i=0; i<j; ++i) {
#ifdef STATS
		/*
		fprintf(out_fp, "\n\n************ Rank %ld ************ \n",i+1);
		*/
#endif
		sp = findmaxS();
		align = printS(sp, slp1, slp2, align);
		/**fflush(stdout);**/
		curS = 0;
		--K;
		if (i != j-1) {
			if (IE==0 || IR==0) rfscan(0,sp->b,0,sp->r);
			else recompute(sp);
		}
		MemFree(sp);
	}
	return align;
}

/* findmaxS - return the class with the maximum score */
kbclass PNTR findmaxS(void)
{
	Int4 i, ms, mi;
	kbclass PNTR sp;

	mi = 0;
	ms = S[0]->score;
	for (i=1; i<lastS; ++i) 
		if (S[i]->score > ms) {
			ms = S[i]->score;
			mi = i;
		}
	sp = S[mi];
	if (minS == lastS-1) minS = mi;
	if (lastS>1) { 
		S[mi] = S[lastS-1];
		S[lastS-1] = NULL;
	}
	--lastS;
	return(sp);
}

/* findS - find the equivalent class corresponding to f */
kbclass PNTR findS(fragment PNTR f)
{
	Int4 i, j;
	fragment PNTR sf;

	for (i=curS, j=0; j<lastS; i= (i+1) % lastS, ++j) {
		sf = S[i]->first;
		if (sf->i == f->i && sf->j == f->j) {
			curS = i;
			return S[i];
		}
	}
	return(NULL);
}

/* set_class - setup values for a new class */
void set_class(Int4 si, fragment PNTR f)
{
	kbclass PNTR sp;
	Int4 endi, endj, t1, t2;
	Int4 endi1, endj1;

	sp = S[si];
	sp->first = f->bgf;
	sp->last = f;
	sp->score = f->score;
	endi = f->i + f->k - 1;
	endj = f->j + f->k - 1;
	t1 = (f->score - mw) / IR;
	t2 = (f->score - mw - IO) / IE;
	if (t2 > t1) t1 = t2;
	endi1 = endi + t1;
	if (endi1 >= len1) endi1 = len1-1;
	if (endi1 < endi) endi1 = endi;
	endj1 = endj + t1;
	if (endj1 >= len2) endj1 = len2-1;
	if (endj1 < endj) endj1 = endj;
	sp->t = endi;
	sp->b = endi1;
	sp->l = endj;
	sp->r = endj1;
	curS = si;
}
	
/* replaceS - replace the min_score class */
void replaceS(fragment PNTR f)
{
	Int4 i;

	incre(f);
	decre(S[minS]->last);
	set_class(minS,f);
	for (i=0; i<lastS; ++i)
		if (S[i]->score < S[minS]->score) minS = i;
}

/* insertS - insert a new class */
void insertS(fragment PNTR f)
{
	incre(f);
	S[lastS] = (kbclass *) ckalloc(sizeof(kbclass));
	set_class(lastS,f);
	if (S[lastS]->score < S[minS]->score) minS = lastS;
	++lastS;
}

/* sizeS - return the number of current equivalent classes */
Int4 sizeS(void)
{
	return lastS;
}

/* minscoreS - return the minimum score in all classes */
Int4 minscoreS(void)
{
	return S[minS]->score;
}

Int4 enterS(fragment PNTR f, Int4 w, Int4 l)
{
	kbclass PNTR s;
	Int4 i;
	Int4 endi, endj;
	Int4 endi1, endj1, t1, t2;

	if (s = findS(f->bgf)) {
		if (s->score < f->score) {
			incre(f);
			decre(s->last);
			s->score = f->score;
			s->first = f->bgf;
			s->last = f;
			if (s == S[minS])
			   for (i=0; i<lastS; ++i)
			      if (S[i]->score < S[minS]->score) minS = i;
		}
		endi = f->i + f->k - 1;
		endj = f->j + f->k - 1;
		t1 = (f->score - mw) / IR;
		t2 = (f->score - mw - IO) / IE;
		if (t2 > t1) t1 = t2;
		endi1 = endi + t1;
		if (endi1 >= len1) endi1 = len1-1;
		if (endi1 < endi) endi1 = endi;
		endj1 = endj + t1;
		if (endj1 >= len2) endj1 = len2-1;
		if (endj1 < endj) endj1 = endj;
		if (s->t > endi) s->t = endi;
		if (s->b < endi1) s->b = endi1;
		if (s->l > endj) s->l = endj;
		if (s->r < endj1) s->r = endj1;
	} else {
		if (sizeS() == l) replaceS(f);
		else insertS(f);
	}
	if (sizeS() == l) return minscoreS();
	else return w;
}

/* find the best local alignment */
void local(fragment PNTR f)
{
	Int4 max_score, cs;
	fragment PNTR pf, PNTR cf, PNTR df;
	Int4 cur_diag, col, diag;

	max_score = 0;
	cur_diag = pos_diag(f);
	f->bgf = f;

	/* compute the best left influence */
	cf = db_rsearch_next(li_c, f->j);
	col = db_cur_key(li_c);
	df = db_lsearch_next(li_d, cur_diag);
	diag = db_cur_key(li_d);
	pf = ((f->j - col > cur_diag-diag)&&(diag != -1)) ? df : cf;
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score || (cs == max_score && topo_win(f,pf))) {
			max_score = cs;
			f->bgf = pf->bgf;
			incre(f->bgf);
		}
	}
	
	/* compute the best mismatch */
	if (pf = diag_prev[cur_diag]) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score || (cs == max_score && topo_win(f,pf))) {
			max_score = cs;
			if (f != f->bgf) decre(f->bgf);
			f->bgf = pf->bgf;
			incre(f->bgf);
		}
		decre(pf);
	}
	diag_prev[cur_diag] = f;
	incre(f);

	/* compute the best right influence */
	pf = rsearch_next(ri,cur_diag);
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score || (cs == max_score && topo_win(f,pf))) {
			max_score = cs;
			if (f != f->bgf) decre(f->bgf);
			f->bgf = pf->bgf;
			incre(f->bgf);
		}
	}

	f->score = max_score + (Int4) (DIGIT * f->k);

	/* update current equivalent classes */
	if (f->score > mw) mw = enterS(f,mw,K);
}

/* find the best alignment */
void align(fragment PNTR pof, fragment PNTR cdiag_prev[],list cri,db_list cli_c, db_list cli_d, fragment PNTR f)
{
	Int4 max_score, cs;
	fragment PNTR pf, PNTR cf, PNTR df;
	Int4 cur_diag, col, diag;

	cur_diag = pos_diag(f);

	/* compute the best mismatch */
	if (pf = cdiag_prev[cur_diag]) {
		cs = pf->score - weight(pf, f); 
		max_score = cs;
		f->bgf = pf->bgf;
		incre(f->bgf);
		decre(pf);
	} else {
		f->bgf = f;
		max_score = -1 * weight(pof,f);
	}
	cdiag_prev[cur_diag] = f;
	incre(f);

	/* compute the best right influence */
	pf = rsearch_next(cri,cur_diag);
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score) {
			max_score = cs;
			if (f != f->bgf) decre(f->bgf);
			f->bgf = pf->bgf;
			incre(f->bgf);
		}
	}

	/* compute the best left influence */
	cf = db_rsearch_next(cli_c, f->j);
	col = db_cur_key(cli_c);
	df = db_lsearch_next(cli_d, cur_diag);
	diag = db_cur_key(cli_d);
	pf = ((f->j - col > cur_diag-diag)&&(diag != -1)) ? df : cf;
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score) {
			max_score = cs;
			if (f != f->bgf) decre(f->bgf);
			f->bgf = pf->bgf;
			incre(f->bgf);
		}
	}
	
	/* update the current score */
	f->score = max_score + (Int4) (DIGIT * f->k);
}

/* find the best alignment for cut_frag */
void calign(fragment PNTR pof, fragment PNTR cdiag_prev[], list cri, db_list cli_c, db_list cli_d, fragment PNTR f)
{
	Int4 max_score, cs;
	fragment PNTR pf, PNTR cf, PNTR df;
	Int4 cur_diag, col, diag;

	cur_diag = pos_diag(f);

	/* compute the best mismatch */
	if (pf = cdiag_prev[cur_diag]) {
		cs = pf->score - weight(pf, f); 
		max_score = cs;
		f->bgf = pf;
		incre(pf);
	} else {
		f->bgf = pof;
		incre(pof);
		max_score = -1 * weight(pof,f);
	}
	incre(f);

	/* compute the best right influence */
	pf = rsearch_next(cri,cur_diag);
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score) {
			max_score = cs;
			decre(f->bgf);
			f->bgf = pf;
			incre(pf);
		}
	}

	/* compute the best left influence */
	cf = db_rsearch_next(cli_c, f->j);
	col = db_cur_key(cli_c);
	df = db_lsearch_next(cli_d, cur_diag);
	diag = db_cur_key(cli_d);
	pf = ((f->j - col > cur_diag-diag)&&(diag != -1)) ? df : cf;
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score) {
			max_score = cs;
			decre(f->bgf);
			f->bgf = pf;
			incre(pf);
		}
	}
	
	/* update the current score */
	f->score = max_score + (Int4) (DIGIT * f->k);
}

/* malign - midpoint alignment */
void malign(fragment PNTR pof, fragment PNTR cdiag_prev[], list cri, db_list cli_c, db_list cli_d, fragment PNTR f)
{
	Int4 max_score, cs;
	fragment PNTR pf, PNTR cf, PNTR df;
	Int4 cur_diag, col, diag;

	cur_diag = pos_diag(f);
	f->bgf = pof;

	/* compute the best mismatch */
	if (pf = cdiag_prev[cur_diag]) {
		cs = pf->score - weight(pf, f); 
		max_score = cs;
		f->bgf = pf;
	} else max_score = -1 * weight(pof,f);

	/* compute the best right influence */
	pf = rsearch_next(cri,cur_diag);
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score) {
			max_score = cs;
			f->bgf = pf;
		}
	}

	/* compute the best left influence */
	cf = db_rsearch_next(cli_c, f->j);
	col = db_cur_key(cli_c);
	df = db_lsearch_next(cli_d, cur_diag);
	diag = db_cur_key(cli_d);
	pf = ((f->j - col > cur_diag-diag)&&(diag != -1)) ? df : cf;
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score) {
			max_score = cs;
			f->bgf = pf;
		}
	}
	
	/* update the current score */
	f->score = max_score + (Int4) (DIGIT * f->k);
}

/* malign1 - midpoint alignment */
void malign1(fragment PNTR pof, list cri, db_list cli_c, db_list cli_d, fragment PNTR f)
{
	Int4 max_score, cs;
	fragment PNTR pf, PNTR cf, PNTR df;
	Int4 cur_diag, col, diag;

	cur_diag = pos_diag(f);
	f->bgf = pof;

	if (cur_diag == pos_diag(pof))
		max_score = MININT;
	else 
		max_score = -1 * weight(pof,f);

	/* compute the best right influence */
	pf = rsearch_next(cri,cur_diag);
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score) {
			max_score = cs;
			f->bgf = pf;
		}
	}

	/* compute the best left influence */
	cf = db_rsearch_next(cli_c, f->j);
	col = db_cur_key(cli_c);
	df = db_lsearch_next(cli_d, cur_diag);
	diag = db_cur_key(cli_d);
	pf = ((f->j - col > cur_diag-diag)&&(diag != -1)) ? df : cf;
	if (pf) {
		cs = pf->score - weight(pf, f); 
		if (cs > max_score) {
			max_score = cs;
			f->bgf = pf;
		}
	}
	
	/* update the current score */
	f->score = max_score + (Int4) (DIGIT * f->k);
}

/* tailor - return proper rcopy */
frag_pt PNTR tailor(frag_pt PNTR rcopy, Int4 col)
{
	frag_pt PNTR p, PNTR t1, PNTR t2;

	if (!rcopy) return NULL;
	p = rcopy;
	t1 = NULL;
	while (p && p->col <= col) {
	   t1 = p;
	   p = p->link;
	}
	while (p) {
		t2 = p->link;
		MemFree(p);
		p = t2;
	}
	if (t1) { /* rcopy->col <= col */
		t1->link = NULL;
		return rcopy;
	} else return NULL;
}

/* otr_survive - reduce the rcopy orthogonally */
frag_pt PNTR otr_survive(frag_pt PNTR rcopy, Int4 col)
{
	frag_pt PNTR p, PNTR q, PNTR r, PNTR t1, PNTR t2;
	Int4 cur_score;

	if (!rcopy) return NULL;
	r = NULL;
	q = NULL;
	p = rcopy;
	while (p) {
	   if ((p->fp)->score-IR*(col-p->col) > 0) {
		r = (frag_pt *) ckalloc(sizeof(frag_pt));
		r->fp = p->fp;
		r->col = p->col;
		r->link = q;
		q = r;
	   }
	   p = p->link;
	}

	/* sweep right */
	p = r;
	while (p) {
		cur_score = (p->fp)->score;
		for (q = p->link; q && ((cur_score - IE*(p->col-q->col))
	    	     > (q->fp)->score - IR*(p->col-q->col)) ; q = q->link);
		t1 = p->link;
		while (t1 != q) {
			t2 = t1->link;
			MemFree(t1);
			t1 = t2;
		}
		p->link = q;
		p = q;
	}
	return r;
}

/* rotr_merge - r_merge orthogonally */
void rotr_merge(frag_pt PNTR rcopy)
{
	frag_pt PNTR p, PNTR q;
	fragment PNTR pf, PNTR rf;

	for (p = rcopy; p; q=p, p=p->link, MemFree(q)) {
		pf = p->fp;
		rf = rsearch_next(ri,rcpos_diag(pf));
		/* determine if we need to keep pf in cri */
		if (!rf || (rf && rotr_win(pf,rf)))
			rcr_add_point(ri,pf,p->col);
	}
}

/* rotr_win - TRUE if pf is better in common right influence area */
Int4 rotr_win(fragment PNTR pf, fragment PNTR rf)
{
	Int4 r1, r2, diag_diff;

	diag_diff = rcpos_diag(pf) - rcpos_diag(rf);
	r1 = pf->j + pf->k - 1;
	r2 = rf->j + rf->k - 1;
	if (rf->score-IR*(r1-r2)-IE*diag_diff < pf->score)
		return TRUE;
	return FALSE;
}
			
/* cotr_merge - r_merge orthogonally */
void cotr_merge(frag_pt PNTR rcopy)
{
	frag_pt PNTR p, PNTR q;
	fragment PNTR pf, PNTR rf;

	for (p = rcopy; p; q=p, p=p->link, MemFree(q)) {
		pf = p->fp;
		rf = rsearch_next(bri,pos_diag(pf));
		/* determine if we need to keep pf in cri */
		if (!rf || (rf && cotr_win(pf,rf)))
			r_add_point(bri,pf,p->col);
	}
}

/* cotr_win - TRUE if pf is better in common right influence area */
Int4 cotr_win(fragment PNTR pf, fragment PNTR rf)
{
	Int4 r1, r2, diag_diff;

	diag_diff = pos_diag(pf) - pos_diag(rf);
	r1 = pf->i + pf->k - 1;
	r2 = rf->i + rf->k - 1;
	if (rf->score-IR*(r1-r2)-IE*diag_diff < pf->score)
		return TRUE;
	return FALSE;
}

/* otl_survive - reduce the rcopy orthogonally (left influence) */
frag_pt PNTR otl_survive(frag_pt PNTR rcopy, Int4 col)
{
	frag_pt PNTR p, PNTR q, PNTR r, PNTR t;
	Int4 cur_score,col_diff;
	fragment PNTR pf;

	if (!rcopy) return NULL;
	r = NULL;
	t = NULL;
	p = rcopy;
	while (p) {
	   pf = p->fp;
	   cur_score = pf->score;	
	   col_diff = col-p->col;
	   if ((cur_score-(IO+IE*col_diff) > 0) ||
		(cur_score-IR*col_diff> 0)) {
		r = (frag_pt *) ckalloc(sizeof(frag_pt));
		r->fp = p->fp;
		r->col = p->col;
		r->link = t;
		t = r;
		for (q = p->link; q && ((cur_score - IE*(q->col-p->col))
	    	     >= (q->fp)->score) ; q = q->link);
		p = q;
	   } else p=p->link;
	}
	return r;
}

/* otl_merge - l_merge for orthogonally recomputation */
void otl_merge(frag_pt PNTR r, Int4 col, Int4 row, db_list cli_c, db_list cli_d, idb_list cintersect[], Int4 ccol_int[], Char cdiag_flag[], Int4 (PNTR cpos_diag)(fragment PNTR), Int4 (PNTR otl_win)(fragment PNTR, fragment PNTR))
{
	fragment PNTR df, PNTR cf, PNTR rf, PNTR pf, PNTR nf;
	Int4 cur_diag, lrow, diag, in_row, next_diag;
	frag_pt PNTR p;
    Int4 fs;
    
	if (!r) return;
	db_reset_pos(cli_c);
	db_reset_pos(cli_d);
	cur_diag = offset1 + (row-col);
	cf = db_rsearch_next(cli_c, row);
	lrow = db_cur_key(cli_c);
	df = db_rsearch_next(cli_d,cur_diag);
	diag = db_cur_key(cli_d);
	rf = ((row-lrow > cur_diag-diag) && (diag != -1)) ? df : cf;
	if ((rf == NULL) && (diag == -1)) { /* cli_c && cli_d are empty */
		db_insert_next(cli_c,row,r->fp);
		incre(r->fp);
		for (p = r; p; MemFree(p),p=r) {
			r = p->link;
			if (r) df = r->fp;
			else df = NULL;
			fs = (*cpos_diag)(p->fp);
			db_insert_next(cli_d,fs,df);
			incre(df);
		}
		return;
	}
	if ((*otl_win)(r->fp,rf)) { /* one more column boundary */
		db_insert_next(cli_c,row,r->fp);
		incre(r->fp);
		if (rf == df) { /* cintersect list is affected */
			in_row = row - diag + offset1;
			if (in_row < cur_end1) {
				if (!cintersect[in_row])
					cintersect[in_row]=idb_newList();
				db_insert(cintersect[in_row],row,NULL);
				ccol_int[row] = in_row;
				cdiag_flag[diag] = 1;
			}
		}
	}
	incre(rf);
	while (r) {
		pf = r->fp;
		cur_diag = (*cpos_diag)(pf);
		p = r->link;
		if (p) df = p->fp;
		else df = NULL;
		next_diag = db_next_key(cli_d);
		nf = db_next_val(cli_d);
		if (next_diag == -1) {
			db_insert_next(cli_d,cur_diag,df);
			incre(df);
			MemFree(r);
			r = p;
		} else if (next_diag > cur_diag) {
			if ((*otl_win)(pf,rf)) {
				cf = ((*otl_win)(df,rf))? df:rf;
				db_insert_next(cli_d,cur_diag,cf);
				incre(cf);
			}
			MemFree(r);
			r = p;
		} else if (next_diag == cur_diag) {
			decre(rf);
			rf = db_next(cli_d);
			incre(rf);
			if ((*otl_win)(df,nf)) {
				db_update_node(cli_d,df);
				decre(nf);
				incre(df);
			}
			MemFree(r);
			r = p;
		} else if ((*otl_win)(pf,rf)) {
		/* next_diag < cur_diag and pf is better */
			decre(rf);
			rf = nf;
			db_delete_next(cli_d);
		} else { /* next_diag < cur_diag and rf is better */
			decre(rf);
			rf = db_next(cli_d);
			incre(rf);
			if ((*otl_win)(pf,nf)) {
				db_update_node(cli_d,pf);
				decre(nf);
				incre(pf);
			}
		}

	}
	decre(rf);
}

/* rotl_win - TRUE if pf is better in common left influence area */
Int4 rotl_win(fragment PNTR pf, fragment PNTR rf)
{
	Int4 c1, c2, diag_diff;

	if (!rf) return TRUE;
	if (!pf) return FALSE;
	diag_diff = rcpos_diag(rf) - rcpos_diag(pf);
	c1 = pf->i + pf->k - 1;
	c2 = rf->i + rf->k - 1;
	if (rf->score-IR*(c1-c2)-IE*diag_diff < pf->score)
		return TRUE;
	return FALSE;
}

/* cotl_win - TRUE if pf is better in common left influence area */
Int4 cotl_win(fragment PNTR pf, fragment PNTR rf)
{
	Int4 c1, c2, diag_diff;

	if (!rf) return TRUE;
	if (!pf) return FALSE;
	diag_diff = pos_diag(rf) - pos_diag(pf);
	c1 = pf->j + pf->k - 1;
	c2 = rf->j + rf->k - 1;
	if (rf->score-IR*(c1-c2)-IE*diag_diff < pf->score)
		return TRUE;
	return FALSE;
}

/* r_survive - reduce the rcopy */
void r_survive(frag_pt PNTR rcopy)
{
	frag_pt PNTR p, PNTR q, PNTR r, PNTR t;
	Int4 cur_score;

	p = rcopy;
	while (p) {
		cur_score = (p->fp)->score;
		/* sweep right */
		for (q = p->link; q && ((cur_score - IE * (q->col - p->col))
		     > (q->fp)->score || ((cur_score - IE * (q->col - p->col))
		     == (q->fp)->score && topo_win(q->fp,p->fp))); q = q->link);
		r = p->link;
		while (r != q) {
			t = r->link;
			MemFree(r);
			r = t;
		}
		p->link = q;
		p = q;
	}
}

/* r_merge - update the active right influence points */
void r_merge(list cri, frag_pt PNTR rcopy, Int4 row)
{
	frag_pt PNTR p, PNTR q;
	Int4 row_diff, diag_diff;
	fragment PNTR pf, PNTR rf;

	for (p = rcopy; p; q = p, p = p->link, MemFree(q)) {
		pf = p->fp;
		rf = rsearch_next(cri,pos_diag(pf));
		/* determine if we need to keep pf in cri */
		if (rf) {
			row_diff = row - (rf->i + rf->k - 1);
			diag_diff = diag(pf) - diag(rf);
			if (pf->score > rf->score-IE*diag_diff-IR * row_diff
			    || (pf->score == rf->score-IE*diag_diff-IR*row_diff
			    && topo_win(rf,pf)))
				r_add_point(cri,pf,row);
		} else r_add_point(cri,pf,row);
	}
}
				
				
/* r_add_point - add point to right influence list cri */
void r_add_point(list cri, fragment PNTR f, Int4 row)
{
	fragment PNTR nf;
	Int4 row_diff,diag_diff;
	
	nf = next_val(cri);
	if (pos_diag(f) == next_key(cri)) { 
		next(cri);
		if ((f->score > (nf->score) -IR*(row-(nf->i +nf->k -1))) ||
		    ((f->score == (nf->score) -IR*(row-(nf->i +nf->k -1))) &&
		     topo_win(nf,f))) {
			update_node(cri,f);
			decre(nf);
			incre(f);
		}
	} else {
		insert_next(cri,pos_diag(f),f);
		incre(f);
	}
	nf = next_val(cri);
	while (nf) {
		row_diff = row - (nf->i + nf->k -1);
		diag_diff = diag(nf) - diag(f); 
		if ((nf->score - IR * row_diff < f->score - IE * diag_diff) ||
		    ((nf->score - IR * row_diff == f->score - IE * diag_diff) &&
		     topo_win(nf,f))) {
			delete_next(cri);
			decre(nf);
			nf = next_val(cri);
		} else nf = NULL;
	}
}

/* resolve - resolve the conflicts of the intersection lists */
void resolve(db_list cli_c, db_list cli_d, idb_list cintersect[], Char cdiag_flag[], Int4 ccol_int[], Int4 row,Int4 (PNTR cpt_lscore)(fragment PNTR, Int4, Int4))
{
	Int4 col, diag;
	fragment PNTR cp, PNTR dp, PNTR cf, PNTR df, PNTR nf;
	Int4 prev_col, prev_diag, next_col, in_row;
	Int4 d_score, c_score;
	idb_list inter;

	if (!cintersect[row]) return;
	inter = cintersect[row];
	idb_reset_pos(inter);
	for (col = idb_key_next(inter); col != -1; col = idb_key_next(inter)){
		diag = col - row + offset;
		cp = db_rsearch_next(cli_c,col);
		cf = db_next_val(cli_c);
		c_score = (*cpt_lscore)(cf,col,diag);
		dp = db_rsearch_next(cli_d,diag);
		prev_col = db_cur_key(cli_c);
		prev_diag = db_cur_key(cli_d);
		df = ((col - prev_col > diag - prev_diag) &&
			  (prev_diag != -1))? dp : cp;
		d_score = (*cpt_lscore)(df,col,diag);
		if (c_score > d_score || (c_score == d_score && topo_win(df,cf))) { /* extend the column boundary */
			nf = db_next_val(cli_d);
			db_delete_next(cli_d);
			decre(nf);
			cdiag_flag[diag] = 0;
			in_row = col-prev_diag+offset;
			ccol_int[col] = 0;
			if (df == dp && in_row<=cur_end)
			{ /* intersection lists are affected */
				INTER_ADD(col);
				cdiag_flag[prev_diag] = 1;	
			}	
		} else { /* extend the diagonal boundary */
			db_delete_next(cli_c);
			next_col = db_next_key(cli_c);
			in_row = next_col-diag+offset;
			if ((next_col != -1) && in_row<=cur_end &&(ccol_int[next_col] == 0)) {
				INTER_ADD(next_col);
				cdiag_flag[diag] = 1;	
			} else cdiag_flag[diag] = 0;
			nf = db_next_val(cli_d);
			db_next(cli_d);
			db_update_node(cli_d,cf);
			decre(nf);
  			ccol_int[col] = 0; 

		}
	}
}

/* l_merge - merge lcopy into cli_c & cli_d */
void l_merge(db_list cli_c, db_list cli_d, idb_list cintersect[], Char cdiag_flag[], Int4 ccol_int[], frag_pt PNTR lcopy, Int4 row, Int4 (PNTR cpt_lscore)(fragment PNTR, Int4, Int4))
{
	Int4 col, diag;
	Int4 c_col, d_diag, in_row;
	Int4 next_col, p_score;
	Int4 prev_diag;
	fragment PNTR f, PNTR cf, PNTR df, PNTR dp, PNTR nf, PNTR lp;
	frag_pt PNTR e;

	for (e = lcopy; e; e = e->link) {
		col = e->col;
		f = e->fp;
		diag = col - row + offset;
		cf = db_rsearch_next(cli_c,col);
		c_col = db_cur_key(cli_c);
		df = db_lsearch_next(cli_d,diag);
		d_diag = db_cur_key(cli_d);
		next_col = db_next_key(cli_c);
		lp = ((col - c_col >= diag - d_diag) && (d_diag != -1)) ?
			df : cf;
		if (lp != df) { /* it's owned by the column bound */
			if (col != next_col) { /* next_col > col */
				p_score = (*cpt_lscore)(lp,col,diag);
				if (f->score > p_score || (f->score == p_score
				    && topo_win(lp,f))) {
				   in_row = next_col-diag+offset;
				   if ((next_col != -1) && in_row<=cur_end && (ccol_int[next_col] == 0)){
					INTER_ADD(next_col);
					cdiag_flag[diag] = 1;
				   }
				   db_insert_next(cli_d,diag,lp);
				   incre(lp);
				   db_insert_next(cli_c,col,f);
				   incre(f);
				}
			} else { /* next_col == col */
				nf = db_next(cli_c);
				p_score = (*cpt_lscore)(nf,col,diag);
				if (f->score > p_score || (f->score == p_score
				    && topo_win(nf,f))) {
					db_update_node(cli_c,f);
					db_insert_next(cli_d,diag,nf);
					incre(f); /* nf->ref remains the same */
					next_col = db_next_key(cli_c);
					in_row = next_col-diag+offset;
					if ((next_col != -1) && in_row<=cur_end
					    && (ccol_int[next_col] == 0)) {
					    INTER_ADD(next_col);
					    cdiag_flag[diag] = 1;
					}
				}
			}
		} else { /* it is owned by the diagonal bound */
			if ((col != next_col) && (diag != d_diag)) {
				p_score = (*cpt_lscore)(df,col,diag);
				if ((df == NULL) || (f->score > p_score) ||
				    (f->score == p_score && topo_win(df,f))){
				   if (d_diag == -1) { 
				   /* it isn't in any active region */
					in_row = next_col-diag+offset;
					if (next_col != -1 && in_row<=cur_end) { 
					   INTER_ADD(next_col);
					   cdiag_flag[diag] = 1;
					}
				   } else if (cdiag_flag[d_diag] && ccol_int[next_col]) { 
					db_delete(cintersect[ccol_int[next_col]],next_col);
					in_row = col-d_diag+offset;
					INTER_ADD(col);
					in_row = next_col-diag+offset;
					INTER_ADD(next_col);
					cdiag_flag[diag] = 1;
				   } else {
					in_row = col-d_diag+offset;
					if (in_row<=cur_end) {
					   INTER_ADD(col);
					   cdiag_flag[d_diag] = 1;
					}
					in_row=next_col-diag+offset;
					if (next_col != -1 && !ccol_int[next_col] && in_row<=cur_end) {
					   INTER_ADD(next_col);
					   cdiag_flag[diag]=1;
					 }
				   }
				   db_insert_next(cli_c,col,f);
				   incre(f);
				   db_insert_next(cli_d,diag,df);
				   incre(df);
				}
			} else if ((col != next_col) && (diag == d_diag)) {
				dp = db_prev_val(cli_d);
				prev_diag = db_prev_key(cli_d);
				df = ((col - c_col >= diag - prev_diag) &&
					   (prev_diag != -1))?
					 dp : cf;
				p_score = (*cpt_lscore)(df,col,diag);
				if (f->score > p_score || (f->score == p_score
				    && topo_win(df,f))) {
					db_insert_next(cli_c,col,f);
					incre(f);
					in_row = col-prev_diag+offset;
					if (df == dp && in_row<=cur_end) {
					   INTER_ADD(col);
				           cdiag_flag[prev_diag] = 1;
					}
				}
			} else { /* col == next_col */
				nf = db_next(cli_c);
				p_score = (*cpt_lscore)(nf,col,diag);
				if (f->score > p_score || (f->score == p_score
				    && topo_win(nf,f))) {
					db_update_node(cli_c,f);
					db_insert_next(cli_d,diag,nf);
					incre(f);
					next_col = db_next_key(cli_c);
					in_row = next_col-diag+offset;
					if ((next_col != -1) && in_row<=cur_end
					    && (ccol_int[next_col] == 0)) {
					    INTER_ADD(next_col);
					    cdiag_flag[diag] = 1;
					}
				}

			}
		}

	}
}

/* rcr_merge - update the active right influence points */
void rcr_merge(list cri, frag_pt PNTR rcopy, Int4 row)
{
	frag_pt PNTR p, PNTR q;
	Int4 row_diff, diag_diff;
	fragment PNTR pf, PNTR rf;

	for (p = rcopy; p; q = p, p = p->link, MemFree(q)) {
		pf = p->fp;
		rf = rsearch_next(cri,rcpos_diag(pf));
		/* determine if we need to keep pf in cri */
		if (rf) {
			row_diff = row - (rf->j + rf->k - 1);
			diag_diff = rcdiag(pf) - rcdiag(rf);
			if (pf->score > rf->score-IE*diag_diff-IR * row_diff
			    || (pf->score == rf->score-IE*diag_diff-IR*row_diff
			    && topo_win(rf,pf)))
				rcr_add_point(cri,pf,row);
		} else rcr_add_point(cri,pf,row);
	}
}
				
				
/* rcr_add_point - add point to right influence list cri */
void rcr_add_point(list cri, fragment PNTR f, Int4 row)
{
	fragment PNTR nf;
	Int4 row_diff,diag_diff;
	
	nf = next_val(cri);
	if (rcpos_diag(f) == next_key(cri)) { 
		next(cri);
		if ((f->score > (nf->score) -IR*(row-(nf->j +nf->k -1))) ||
		    ((f->score == (nf->score) -IR*(row-(nf->j +nf->k -1))) &&
		     topo_win(nf,f))) {
			update_node(cri,f);
			decre(nf);
			incre(f);
		}
	} else {
		insert_next(cri,rcpos_diag(f),f);
		incre(f);
	}
	nf = next_val(cri);
	while (nf) {
		row_diff = row - (nf->j + nf->k -1);
		diag_diff = rcdiag(nf) - rcdiag(f); 
		if ((nf->score - IR * row_diff < f->score - IE * diag_diff) ||
		    ((nf->score - IR * row_diff == f->score - IE * diag_diff) &&
		     topo_win(nf,f))) {
			delete_next(cri);
			decre(nf);
			nf = next_val(cri);
		} else nf = NULL;
	}
}

/* topo_win - return TRUE if f->bgf is greater than pf->bgf in the topological
	      order */
Int4 topo_win(fragment PNTR pf, fragment PNTR f)
{
	if (!pf) return(TRUE);
	if (!f) return(FALSE);
	if (diag(f->bgf) > diag(pf->bgf)) return(TRUE);
	if (diag(f->bgf) == diag(pf->bgf) && (f->bgf)->i >= (pf->bgf)->i)
	   return(TRUE);
	return(FALSE);
}

/* pt_lscore - compute the fading score of the given fragment (left inf)*/
Int4 pt_lscore(fragment PNTR pf, Int4 col, Int4 diag)
{
	Int4 col_diff, diag_diff;

	if (!pf) return(MININT);
	else {  col_diff = col - (pf->j + pf->k -1);
		diag_diff = pos_diag(pf) - diag;
		return(pf->score -IE*diag_diff-IR*col_diff);
	}
}
		
/* rcpt_lscore - compute the fading score of the given fragment (left inf)*/
Int4 rcpt_lscore(fragment PNTR pf, Int4 col, Int4 diag)
{
	Int4 col_diff, diag_diff;

	if (!pf) return(MININT);
	else {  col_diff = col - (pf->i + pf->k -1);
		diag_diff = rcpos_diag(pf) - diag;
		return(pf->score -IE*diag_diff-IR*col_diff);
	}
}
		
/* weight - compute the cost of the connection of fragments pf and f */
Int4 weight(fragment PNTR pf, fragment PNTR f)
{
	Int4 msi, msj;

	msi = IR*(f->i - pf->i - pf->k);
	msj = IR*(f->j - pf->j - pf->k);
	if (diag(pf) == diag(f))
		return(msi);
	else if (diag(pf) < diag(f))
		return(gap_cost(diag(f)-diag(pf)) + msi);
	else
		return(gap_cost(diag(pf)-diag(f)) + msj);
}

/* gap_cost - compute the cost of a gap, given length l */
Int4 gap_cost(Int4 l)
{
	return(IO+IE*l);
}

/* diag - give the diagonal number for fragment */
Int4 diag(fragment PNTR f)
{
	return(f->j - f->i);
}

/* rcdiag - give the diagonal number for reversed fragment */
Int4 rcdiag(fragment PNTR f)
{
	return(f->i - f->j);
}

/* pos_diag - give the positive diagonal number for fragment */
Int4 pos_diag(fragment PNTR f)
{
	return(f->j - f->i + len1);
}

/* rcpos_diag - give the positive diagonal number for reversed fragment */
Int4 rcpos_diag(fragment PNTR f)
{
	return(f->i - f->j + len2);
}

/* pf_set - set the coordinate of the given pseudo fragment */
void pf_set(fragment PNTR f, Int4 i, Int4 j)
{
	f->i = i;
	f->j = j;
}

/* power - raise base to n-th power; n >= 0 */
Int4 power(Int4 base, Int4 n)
{
	Int4 i, p;

	p = 1;
	for (i=1; i<=n; ++i)
		p = p*base;
	return p;
}

/* decre - decrease the reference count */
void decre(fragment PNTR f)
{
	if (f && (--(f->ref) == 0)) {
		if (f != f->bgf) decre(f->bgf);
		MemFree(f);
		++deleted_frags;
	}
}

/* incre - increase the reference count */
void incre(fragment PNTR f)
{
	if (f) ++(f->ref);
}


static DenseDiagPtr link_diag(DenseDiagPtr new, DenseDiagPtr head)
{
	if(head == NULL)
		return new;
	while(head->next != NULL)
		head = head->next;
	head->next = new;
	return new;
}

/* frag_print - print the given fragment */
DenseDiagPtr frag_print(fragment PNTR f, SeqLocPtr slp1, SeqLocPtr slp2, DenseDiagPtr head)
{
   	DenseDiagPtr new;
	Int4Ptr starts;
	Uint1Ptr strands;

	if (f) {
		new = DenseDiagNew();
		new->dim = 2;
		new->id = SeqIdDup(SeqLocId(slp1));
		new->id->next = SeqIdDup(SeqLocId(slp2));

		starts = MemNew((size_t)(new->dim) * sizeof(Int4));
		strands = MemNew((size_t)(new->dim) * sizeof(Uint1));
		starts[0] = f->i + SeqLocStart(slp1);
		if(SeqLocStrand(slp2) == Seq_strand_minus)
			starts[1] = SeqLocStop(slp2) - (f->j+ f->k -1);
		else
			starts[1] = f->j + SeqLocStart(slp2);
		strands[0] = SeqLocStrand(slp1);
		strands[1] = SeqLocStrand(slp2);
		new->starts = starts;
		new->strands = strands;
		new->len = f->k;

		if(head == NULL)
			head = new;
		else
			link_diag(new, head);

		/*
		fprintf(out_fp, "%ld %ld %ld \n",f->i,f->j,f->k);
		*/
		if (!used_frag[f->i]) used_frag[f->i]=unewList();
		uinsert(used_frag[f->i],f->j);
	}
	++output_frags;

	return head;
}

void Xfrag_print(fragment PNTR f)
{
	Int4 len, decr_i, decr_j, decr;

	if (f) {
		len = f->k - 1;
		if (old_i_end) {
			if (old_j_end - old_i_end == f->j - f->i) {
				old_i_end = f->i + len;
				old_j_end = f->j + len;
			} else {
				decr_i = f->i - old_i_end;
				decr_j = f->j - old_j_end;
				decr = (decr_i < decr_j) ? decr_i : decr_j;
				--decr;
				/*
				fprintf(out_fp, "  l %ld %ld %ld %ld 0.0\n", old_i_start,
				    old_j_start, old_i_end, old_j_end);
				*/
				old_i_start = f->i - decr;
				old_i_end = f->i + len;
				old_j_start = f->j - decr;
				old_j_end = f->j + len;
			}
		} else {
			old_i_start = f->i;
			old_i_end = f->i + len;
			old_j_start = f->j;
			old_j_end = f->j + len;
		}

		if (!used_frag[f->i]) used_frag[f->i]=unewList();
		uinsert(used_frag[f->i],f->j);
	}
	++output_frags;
}
