static char const rcsid[] = "$Id: sec.c,v 6.1 2003/05/30 17:25:38 coulouri Exp $";

/*  sec.c
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
* File Name:  sec.c
*
* Author:  Jinghui Zhang and Kun-Mao Chao
*
* Version Creation Date: 5/24/95
*
*
* File Description:   Part 2 of sim2 - refine the fragment alignments 
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
*
* $Log: sec.c,v $
* Revision 6.1  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.0  1997/08/25 18:54:29  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:43:15  ostell
* Set to revision 5.0
*
 * Revision 4.3  1996/04/10  21:52:29  zjing
 * fix the strand in Region
 *
 * Revision 4.2  1996/01/05  18:14:30  zjing
 * another fix to sec.c
 *
 * Revision 4.1  1996/01/05  15:49:30  zjing
 * fix in compute two ends of the alignment
 *
*
*
* ==========================================================================
*/


#ifndef _SIMUTIL_
#include <simutil.h>
#endif


#define Max(x,y)  ((x) >= (y) ? (x) : (y))
#define Min(x,y)  ((x) <= (y) ? (x) : (y))
#define gap(k) ((k) <= 0 ? 0 : (g+hh*(k)))	/* k-symbol indel cost */

#define INST	0
#define	DELT	1

#define	G_RANGE	100
static Int4 g, hh;




static Int4 get_align_num(SeqAlignPtr sap)
{
	Int4 n;

		n=0;
		while(sap){
			++n;
			sap = sap->next;
		}
		return n;
}


static void get_diag_info(DenseDiagPtr ddp, Int4Ptr i, Int4Ptr j, Int4Ptr k, SeqLocPtr loc1, SeqLocPtr loc2)
{

	*i= (ddp->starts[0] - SeqLocStart(loc1));
	if(SeqLocStrand(loc2) == Seq_strand_minus)
		*j = SeqLocStop(loc2) - (ddp->len -1) - ddp->starts[1];
	else
		*j = (ddp->starts[1] - SeqLocStart(loc2));
	*k = ddp->len;
}

static DenseDiagPtr get_last_diag(DenseDiagPtr ddp)
{
	while(ddp->next != NULL)
		ddp = ddp->next;
	return ddp;
}


static Int4 get_num_seg(void)
{
	return 10000;
}

static Boolean store_data(Int4 i_start, Int4 i_stop, Int4 j_start, Int4 j_stop, Int4Ptr rec_a, Int4Ptr rec_b, Int4Ptr cur_pos)
{
  Int4 pos;

	pos = *cur_pos;
	rec_a[pos] = i_start;
	rec_a[pos+1] = i_stop;
	rec_b[pos] = j_start;
	rec_b[pos+1] = j_stop;
	*cur_pos = pos+2;

	return TRUE;
}



/************************************************************************
*
*	Region(sap, loc1, loc2, A, B, ms, G, H, Width, new_list, 
*	output_type, filter)
*	second part of SIM2. produces gaped alignment from HSPs. 
*	sap: the HSPs stored as DenseDiag
*	loc1: the first sequence
*	loc2: the seconde sequence
*	A, B: strings of Chars for loc1, loc2. Can be set to NULL
*	ms: mismatch score
*	G: open gap
*	H: gap extension
*	Width: set to -1, I don't know what is the purpose
*	new_list: to store the ends of the alignment
*	otuput_type: determine get ends, or Seq-align or both?
*	filter: filter based on the mismatch rate
*
************************************************************************/
SeqAlignPtr Region(SeqAlignPtr sap, SeqLocPtr loc1, SeqLocPtr loc2, CharPtr A, CharPtr B, Int4 ms, Int4 G, Int4 H, Int4 Width, ValNodePtr PNTR new_list, Uint1 output_type, FilterProc filter)
{
	Int4 K, Kcount;
	Int4Ptr LB;
	Int4Ptr RB;
	Int4 i, j, op;
	Int4 fi, fj, fk, li, lj, lk;
	Int4 ci, cj, ck;
	Int4 bi, bj, ei, ej, t;
	FloatHi c;
	CharPtr seq1, seq2;
	Int4 M, N;		/* lengths of sequences */
	Int4 len1, len2;
	Int4Ptr PNTR V;
	Int4Ptr S;
	Int4Ptr sp;		/* conversion operations */
	Int4 old_i, old_j, match_no;
	Int4 pi, pj, ppi, ppj;
	Int4 z_ALIGN(CharPtr, CharPtr, Int4, Int4, Int4, Int4, Int4Ptr, Int4Ptr, Int4Ptr PNTR, Int4, Int4, Int4Ptr);

	DenseDiagPtr ddp, last_ddp;

	SeqAlignPtr new, head;
	Int4Ptr rec_a, rec_b;
	Int4 cur_pos;
	Int4 num_seg;
	FloatHi score;
	Int4 diag_score;
	FloatHiPtr SegScores;
	Int4 seg_count;
	Int4 old_l, tl, tr;
	Int4 front_score, front_offset, front_num, k, ext_score, ext_num;
	Uint1 front_type;
	Int4 end_score, end_offset, end_num;
	Uint1 end_type;
	Int4 front_i, front_j, end_i, end_j;

	Int4  ALL_MATCH;
	Int4 start, stop;
	Uint1 strand;
	Char seq_name[100];
	Boolean record_align= TRUE, record_ends = TRUE;
	Int4 a_len;
	FloatHi zscore;
	Boolean takeit;



	if (sap == NULL) return NULL;
	if(output_type == OUTPUT_ALIGN)
		record_ends = FALSE;
	if(output_type == OUTPUT_ENDS)
		record_align= FALSE;

	M = SeqLocLen(loc1);
	if(A == NULL)
		seq1 = make_sim_seq(loc1, TRUE, NULL);
	else
		seq1 = A;

	N = SeqLocLen(loc2);
	if(B == NULL)
		seq2 = make_sim_seq(loc2, TRUE, NULL);
	else
		seq2 = B;
	SeqIdPrint(SeqLocId(loc2), seq_name, PRINTID_TEXTID_ACCESSION);
	strand = SeqLocStrand(loc2);
	g = G;
	hh = H;

	V = MemNew ((size_t)128 * sizeof(Int4Ptr));
	for (i =0; i<128; ++i)
	   V[i] = MemNew((size_t)128 * sizeof(Int4));
	/* set up match and mismatch weights */
	for (i = 0; i < 128; i++)
	   for (j = 0; j < 128; j++)
		if (i == j)
		   V[i][j] = (Int4) DIGIT;
		else
		   V[i][j] = ms;
	
	K = get_align_num(sap);

	Kcount = 0;
	head = NULL;
	num_seg = get_num_seg();
	rec_a = MemNew((size_t)(2*num_seg) * sizeof(Int4));
	rec_b = MemNew((size_t)(2*num_seg) * sizeof(Int4));
	SegScores = MemNew((size_t)(num_seg * sizeof(FloatHi)));
	while(sap){
		ALL_MATCH = 0;
		get_align_score(sap, &score);
		ddp = sap->segs;
		get_diag_info(ddp, &fi, &fj, &fk, loc1, loc2);
		last_ddp = get_last_diag(ddp);
		get_diag_info(last_ddp, &li, &lj, &lk, loc1, loc2);
		cur_pos= 0;
		seg_count = 0;

		/* extend front end */
		/* Insertion type */
		front_score = 0;
		front_type = INST;
		front_offset = 0;
		front_num = 0;
		k = 1;
		while (fj-k>1 && k<G_RANGE) {
			ext_score = 0;
			i = fi-1;
			j = fj-k-1;
			ext_num = 0;
			while (i>=0 && j>=0) {
			   if (seq1[i] == seq2[j]) {
				ext_score += V[seq1[i]][seq2[j]];
				++ext_num;
			   } else break;
			   --i;
			   --j;
			}
			if (ext_score - gap(k) > front_score) {
			   front_score = ext_score - gap(k);
			   front_offset = k;
			   front_type = INST;
			   front_num = ext_num;
			}
			++k;
		}

		/* Deletion type */
		k = 1;
		while (fi-k>1 && k<G_RANGE) {
			ext_score = 0;
			i = fi-k-1;
			j = fj-1;
			ext_num = 0;
			while (i>=0 && j>=0) {
			   if (seq1[i] == seq2[j]) {
				ext_score += V[seq1[i]][seq2[j]];
				++ext_num;
			   } else break;
			   --i;
			   --j;
			}
			if (ext_score - gap(k) > front_score) {
			   front_score = ext_score - gap(k);
			   front_offset = k;
			   front_type = DELT;
			   front_num = ext_num;
			}
			++k;
		}

		/* extend back end */
		/* Insertion type */
		end_score = 0;
		end_type = INST;
		end_offset = 0;
		end_num = 0;
		k = 1;
		while (lj+lk+k<N && k<G_RANGE) {
			ext_score = 0;
			i = li+lk;
			j = lj+lk+k;
			ext_num = 0;
			while (i<M && j<N) {
			   if (seq1[i] == seq2[j]) {
				ext_score += V[seq1[i]][seq2[j]];
				++ext_num;
			   } else break;
			   ++i;
			   ++j;
			}
			if (ext_score - gap(k) > end_score) {
			   end_score = ext_score - gap(k);
			   end_offset = k;
			   end_type = INST;
			   end_num = ext_num;
			}
			++k;
		}

		/* Deletion type */
		k = 1;
		while (li+lk+k<M && k<G_RANGE) {
			ext_score = 0;
			i = li+lk+k;
			j = lj+lk;;
			ext_num = 0;
			while (i<M && j<N) {
			   if (seq1[i] == seq2[j]) {
				ext_score += V[seq1[i]][seq2[j]];
				++ext_num;
			   } else break;
			   ++i;
			   ++j;
			}
			if (ext_score - gap(k) > end_score) {
			   end_score = ext_score - gap(k);
			   end_offset = k;
			   end_type = DELT;
			   end_num = ext_num;
			}
			++k;
		}

		/* store the front segment */
		if (front_score > 0) {
			if (front_type == INST) {
			   front_i = fi - front_num;
			   front_j = fj - front_num - front_offset;
			} else {
			   front_i = fi - front_num - front_offset;
			   front_j = fj - front_num;
			}
			SegScores[seg_count] = (FloatHi) front_num;
			store_data(front_i+1, front_i+front_num, front_j+1, front_j+front_num, rec_a, rec_b, &cur_pos);
			++seg_count;
		}

		while(ddp != NULL){
		   get_diag_info(ddp, &ci, &cj, &ck, loc1, loc2);
		   if (ci == li) {
			
			len1 = li+lk-fi+1;
			len2 = lj+lk-fj+1;
			S = (Int4 *)ckalloc((len1+len2)*sizeof(Int4));
			for (i = 0; i < len1+len2; ++i) S[i]=0;
			if (fi == li) { /* only one fragment */
				/*
				printf("a {\n");
				printf("  s %.1f\n",(FloatHi)fk);
				printf("  b %ld %ld\n",fi+1,fj+1);
				printf("  e %ld %ld\n",li+lk,lj+lk);
				printf("  l %ld %ld %ld %ld %.1f\n",fi+1,fj+1,fi+fk,fj+fk,100.0);
				printf("}\n");
				*/
				score = (FloatHi)fk;
			} else {
				if (Width < 0) {
				   for (i = ppi; i <= ci; ++i) {
					LB[i] = Min(ppj,LB[i]);
					RB[i] = Max(cj,RB[i]);
				   }
				} else {
				   tr = ej;
				   for (i = pi; i <= ei; ++i) {
					LB[i] = old_l;
					RB[i] = tr;
				   }
				}
				c = ((FloatHi)z_ALIGN(seq1-1,seq2-1,bi,bj,ei,ej,LB,RB,V,G,H,S+fk))/DIGIT + (FloatHi)fk + (FloatHi)lk;
				/*
				Message(MSG_OK,"after z_ALIGN");
				*/
#ifdef STATS
				DISPLAY(seq1-1+fi,seq2-1+fj,len1,len2,S,fi+1,fj+1);
#endif
				/*
				printf("a {\n");
				printf("  s %.1f\n",c);
				printf("  b %ld %ld\n",fi+1,fj+1);
				printf("  e %ld %ld\n",li+lk,lj+lk);*/
				score = c;
			}
			old_i = i = fi;
			old_j = j = fj;
			sp = S;
			match_no = 0;
			diag_score = 0;
			while (i < li+lk || j < lj+lk) {
				op = *sp++;
				if (op == 0) {
				   if (seq1[i] == seq2[j]) ++match_no;
				   diag_score = diag_score + V[seq1[i]][seq2[j]];
				   ++i;
				   ++j;
				   if (i == li+lk || j == lj+lk || *sp != 0) {
				      	SegScores[seg_count] = ((FloatHi) diag_score)/DIGIT;
				      	store_data(old_i+1, i, old_j+1, j, rec_a, rec_b, &cur_pos);
					
				        seg_count++;

					ALL_MATCH += match_no;
					/*ALL_NO += (i - old_i);*/
				      
				      match_no = 0;
				      diag_score = 0;
				      old_i = i;
				      old_j = j;
				   }
				} else {
				   if (op > 0) {
					j = j+op;
					old_j = j;
				   } else {
					i = i-op;
					old_i = i;
				   }
				}
			}

			/*
			printf("}\n");
			*/
			MemFree(S);
			if (++Kcount == K) {
				if (fi != li) {
					MemFree(LB+bi);
					MemFree(RB+bi);
				}
				break;
			} else {
				if (fi != li) {
					MemFree(LB+bi);
					MemFree(RB+bi);
				}
			}
		   } else if (ci == fi) { /* fi != li */
			bi = fi + fk;
			bj = fj + fk;
			ei = li;
			ej = lj;
			t = (ei - bi + 1) * sizeof(Int4);
			LB = (Int4 *) ckalloc(t) - bi;
			RB = (Int4 *) ckalloc(t) - bi;
			if (Width < 0) {
			   for (i = bi; i <= ei; ++i) {
				LB[i] = ej;
				RB[i] = bj;
			   }
			   ppi = pi = bi;
			   ppj = pj = bj;
			} else {
			   pi = bi;
			   old_l = bj;
			}
		   } else { /* intermediate fragments */
			if (Width < 0) {
			   for (i = ppi; i <= ci; ++i) {
				LB[i] = Min(ppj,LB[i]);
				RB[i] = Max(cj,RB[i]);
			   }
			   ppi = pi;
			   ppj = pj;
			   pi = ci+ck;
			   pj = cj+ck;
			} else {
			   tr = Min(cj + Width, ej);
			   for (i = pi; i <= ci; ++i) {
				LB[i] = old_l;
				RB[i] = tr;
			   }
			   tl = cj - Width;
			   pi = ci + ck;
			   for (i = ci + 1; i < pi; ++i) {
				++tl;
				LB[i] = Max(tl, bj);
				++tr;
				RB[i] = Min(tr, ej);
			   }
			   ++tl;
			   old_l = Max(tl,bj);
			}
		   }

		   ddp = ddp->next;
		}/*end of while(ddp)*/

		/* store the end segment */
		if (end_score > 0) {
			if (end_type == INST) {
			   end_i = li+lk+end_num-1;
			   end_j = lj+lk+end_num+end_offset-1;
			} else {
			   end_i = li+lk+end_num+end_offset-1;
			   end_j = lj+lk+end_num-1;
			}
			SegScores[seg_count] = (FloatHi) end_num;
			store_data(end_i-end_num+2, end_i+1, end_j-end_num+2, end_j+1, rec_a, rec_b, &cur_pos);
			++seg_count;
		}

		score = score + ((FloatHi)front_score/DIGIT) + ((FloatHi)end_score/DIGIT);
		for(i =0, a_len =0; i<seg_count; ++i)
			a_len +=(rec_a[2*i+1] - rec_a[2*i] +1);
		zscore = (FloatHi)(ALL_MATCH)/(FloatHi)a_len;

		takeit = TRUE;
		if(filter != NULL)
			takeit = filter(a_len, zscore, score);
		if(takeit)
		{
		   if(record_align)
		   {
			new = make_align(rec_a, rec_b, (Int2)cur_pos, score, SegScores, loc1, loc2);
			head = link_align(new, head);
		   }
		   if(record_ends)
		   {
			if(SeqLocStrand(loc1)!= Seq_strand_minus)
			{
				start = rec_a[0]-1 + SeqLocStart(loc1);
				stop = rec_a[cur_pos-1] -1 + SeqLocStart(loc1);
			}	
			else
			{
				start = SeqLocStop(loc1) - (rec_a[cur_pos-1] -1);
				stop = SeqLocStop(loc1) - (rec_a[0] -1);
			}
			LoadNewEnds(start, stop, strand, score, zscore, seq_name, FALSE, new_list);
		   }
		}
		sap = sap->next;
	}

	MemFree(rec_a);
	MemFree(rec_b);
	MemFree(SegScores);
	for (i=0; i<128; ++i)
		MemFree(V[i]);
	MemFree(V);
	if(A == NULL)
		MemFree(seq1);
	if(B == NULL)
		MemFree(seq2);
	return head;
}


