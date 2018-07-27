static char const rcsid[] = "$Id: suggadj.c,v 6.3 2003/05/30 17:25:38 coulouri Exp $";

/*   suggadj.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  suggadj.c
*
* Author:  Karl Sirotkin, Webb Miller, Warren Gish, Jonathan Kans
*
* Version Creation Date:   3/17/94
*
* $Revision: 6.3 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <objall.h>
#include <sequtil.h>
#include <suggest.h>

static Int4 Weirdness_found;

struct alternative_splice {
	Int4 from_left,split_left,score;
} ;
static struct alternative_splice * alternatives;

/* =================================================================
 ----- copy_m_stk_entry 
* ======================================
*
* Description: copies elements of m_stk[] array
*
* Return Value: none
*
* Input Via Formal Parameters: to and from indices
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: m_stk[]
*
* Output Via Global Variables: m_stk[]
* 
*===============================================*/

extern void copy_m_stk_entry(Int4 to, Int4 from)
{
		m_stk[to].orient = m_stk[from].orient;
		m_stk[to].fr_shift = m_stk[from].fr_shift;
		m_stk[to].p_pos = m_stk[from].p_pos;
		m_stk[to].dna_pos = m_stk[from].dna_pos;
		m_stk[to].length = m_stk[from].length;
		m_stk[to].left_add_bases = m_stk[from].left_add_bases;
 	m_stk[to].right_add_bases = 	m_stk[from].right_add_bases;	
}

/* ===================================================
 ----- fix_single_mismatches () --
* ======================================
*
* Description: 
This routine looks for single unassigned amino acids only due
to mismatches
If a single mismatched base accounts for the
problem, the two intervals are coalesced.

  Since the intervals to be combined can be a distance
  away from one another, this has to be an O(n^2) algorithm
*
* Return Value: none
*
* Input Via Formal Parameters: none
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: m_stk[]
*
* Output Via Global Variables: m_stk[]
* 
*===============================================*/

/* SPLIT_CODON_FOR_UNASSIGNED (added function) */

extern void fix_single_mismatches(void)
{
	Int4 i, j, k, upper, right_exon_starts, left_exon_ends;
	Int4 k_scan;
/* this for (;;) loop was mostly lifted from the subsequenct
report() function.  If the variable names seem unclear, etc. 
that is history.
*/

#ifdef DEBUG_SINGLE_MISMATCH 
dump_m_stk(stdout,"DEBUG_SINGLE_MISMATCH before ");
fflush(stdout);
#endif

	for (i = 0; i < m_top; ++i) {
		for (upper=i+1; upper < m_top; upper ++ ){
			 if( (j = m_stk[i].p_pos + m_stk[i].length - 1) < 
		    (k = m_stk[upper].p_pos) - 1){

		if ( j+1 == k-1 && m_stk[upper].orient 
			== m_stk[i].orient){

#ifdef DEBUG_SINGLE_MISMATCH
printf ("DEBUG_SINGLE_MISMATCH Unassigned at %d (%c) \n", 
j+1, p_seq[j+1]);
fflush(stdout);
#endif

/*	we have a single unassigned codon (aa) */
			right_exon_starts = 3*m_stk[upper].dna_pos 
				+ m_stk[upper].fr_shift + 1;
			left_exon_ends = 3*m_stk[i].dna_pos 
				+ m_stk[i].fr_shift + 1
				+ 3*m_stk[i].length - 1;
			if (right_exon_starts-left_exon_ends != 4){
/*-- not due to mismatch ---*/
			}else{
/*-- DUE to mismatch ---*/
/*-- single unassigned codon and 3 dna bases 
		... combine two intervals --*/
/*-- i is index on m_stk[], and the i entry and 
		the upperTH one need to be combined */
				m_stk[i].length += m_stk[upper].length+1;
#ifdef DEBUG_COALESCE_MISMATCH
printf("DEBUG_COALESCE_MISMATCH coalescing %d (%d) and %d (%d) new len %d\n",
m_stk[i].p_pos,i,
m_stk[upper].p_pos,upper, m_stk[i].length);
fflush(stdout);
#endif
				m_top --;
				for (k_scan = upper; k_scan < m_top ; k_scan ++){
					copy_m_stk_entry (k_scan,k_scan+1);
				}
				upper --;
			}
		}
	}
}
	}

#ifdef DEBUG_SINGLE_MISMATCH
dump_m_stk(stdout,"DEBUG_SINGLE_MISMATCH after ");
fflush(stdout);
#endif

}

/* ========================================
 ----- in_list () --
* ======================================
*
* Description: test if 1 parameter is list
*
* Return Value: 1 is, 0 is not
*
* Input Via Formal Parameters: index, list, number in list 
*
* Output Via Formal Parameters: 
*
 ========================================*/
static Int4 in_list (Int4 n, Int4 list[], Int4 num)
{
Int4 retval =0;
Int4 index;

for (index =0; index < num; index ++ ){
	if (list[index]==n){
		retval = 1;
		break;
	}
}
	

return retval;
}

/* ========================================
 ----- overlap_aas () --
* ======================================
*
* Description: determines if two potential exons overlap 
*              in the aa's they predict, if so returns max
*              number of aa's which stick over an end.
*
* Return Value: -1, if identical, or number of aas which
*               exon2 adds, above those in exon1.
*
* Input Via Formal Parameters: 
*
* Output Via Formal Parameters: 
*
* Input Via Global Variables: m_stk[] (through pointers)
*
 ========================================*/
static Int4 overlap_aas (struct match *exon1, struct match *exon2)
{
Int4 retval=0;
Int4 diff;
Int4 end2, end1;

if ( exon1 -> p_pos >= exon2 -> p_pos){
	if ( exon1 -> p_pos <= (end2=exon2 -> p_pos +exon2 ->length -1) ){
		if ( (diff= exon1 -> p_pos - exon2 -> p_pos) > 0 ){
			retval = diff;
		 }else{ 
			retval = -1;
		}
	}
}else {
	if ( exon2 -> p_pos <= (end1=exon1 -> p_pos +exon1 ->length -1) ){
		if ( (diff= exon2 -> p_pos + exon2->length -1 - end1) > 0 ){
			retval = diff;
		 }else{ 
			retval = -1;
		}
	}
} 
#ifdef DEBUG_DETAIL_OVERLAP_AAS
if (retval){
printf ("DEBUG_DETAIL_OVERLAP_AAS retval %d 1 starts %d len %d, 2 starts %d len %d\n",
retval, exon1 -> p_pos, exon1 -> length,
exon2 -> p_pos, exon2 -> length);
fflush (stdout);
}
#endif
return retval;
}

/* ========================================
 ----- does_not_add_enough () --
* ======================================
*
* Description: test if aa acid sequences apecified by
*		parameter index would add > TooShort aas
*
* Return Value: 1 would not, 0 would add enough
*
* Input Via Formal Parameters: index, list, number in list 
*
* Output Via Formal Parameters: 
*
 ========================================*/
static Int4 does_not_add_enough (Int4 n, Int4 list[], Int4 num, Int4 pass_number, Int4 TooShort)
{
Int4 retval =0;
Int4 index;
Int4 overlap_found=0;
Int4 best_add = 0;
Int4 this_add;

for (index =0; index < num; index ++ ){
	if ((this_add=overlap_aas (& m_stk[list[index]], & m_stk[n]))  != 0){
		overlap_found = 1;
		if (this_add > best_add) best_add = this_add;
#ifdef DEBUG_DETAIL_ADD_ENOUGH
printf("DEBUG_DETAIL_ADD_ENOUGH overlap of %d with %d add %d best_add %d\n",
list[index], n, this_add, best_add);
#endif
	}
}
if (pass_number){
/*--second pass, be a little forgiving ---*/
		if (overlap_found){
			if (best_add < TooShort) retval = 1;
		}
}else{
/*--first pass (==0), be strict ---*/
	if (overlap_found) retval = 1;
}

return retval;
}

/* ========================================
 ----- is_this_too_weird () --
* ======================================
*
* Description: insures standard gene.  Do not want
*              to suggest weirdness.
*	Weirdness is strand switching or reverses.
* Return Value: 1 would not, 0 would add enough
*
* Input Via Formal Parameters: index, list, number in list 
*
* Output Via Formal Parameters: 
*
 ========================================*/
static Int4 is_this_too_weird (Int4 n, Int4 list[], Int4 num, Int4 pass_number)
{
Int4 retval =0;
Int4 index;
 
for (index =0; index < num; index ++ ){
	if (m_stk[list[index]].orient != 
			m_stk[n].orient){
#ifdef DEBUG_DETAIL_WEIRDNESS
printf("DEBUG_DETAIL_WEIRDNESS orientation %d for %d\n",
list[index],n);
#endif
		retval = 1;
		break;
	}
}

if (! retval)
for (index =0; index < num; index ++ ){
	if (m_stk[list[index]].p_pos >= m_stk[n].p_pos){
		if ( m_stk[list[index]].dna_pos < m_stk[n].dna_pos){
#ifdef DEBUG_DETAIL_WEIRDNESS
printf("DEBUG_DETAIL_WEIRDNESS first reverse %d for %d\n",
list[index],n);
#endif
			retval = 1;
			break;
		}
	}else{
		if ( m_stk[list[index]].dna_pos > m_stk[n].dna_pos){
#ifdef DEBUG_DETAIL_WEIRDNESS
printf("DEBUG_DETAIL_WEIRDNESS second reverse %d for %d\n",
list[index],n);
#endif
			retval = 1;
			break;
		}
	}
}

return retval;
}

/* ========================================
 ----- base_count () --
* ======================================
*
* Description: calculates number of bases in given interval
*	not counting bases that overlap with others in list
*
* Return Value: base count
*
* Input Via Formal Parameters: 
*
* Output Via Formal Parameters: 
*
* Input Via Global Variables: m_stk[] m_top
*
 ========================================*/
static Int4 base_count(Int4 m_dex, Int4 indices_in_best_set[], Int4 num_in_best_set)
{
Int4 bases, bases_remaining;
Int4 best_dex,test_dex;
Int4 right,left;
Int4 right_test;
Int4 worst_left, worst_right, ov;

bases=m_stk[m_dex].length*3 +
	m_stk[m_dex].left_add_bases +
	m_stk[m_dex].right_add_bases;

left = m_stk[m_dex].p_pos;
right = left + m_stk[m_dex].length;

worst_left = worst_right = 0;

#ifdef DEBUG_DETAIL_BEST
printf("DEBUG_DETAIL_BEST index %d left %d right %d\n",
m_dex, left,right);
fflush(stdout);
#endif

for (best_dex=0; best_dex < num_in_best_set; best_dex ++ ){
	test_dex = indices_in_best_set[best_dex];
	if ( left >= m_stk[test_dex].p_pos &&
			left < (right_test =m_stk[test_dex].p_pos + 
			m_stk[test_dex].length -1) ){
		if ( (ov= right_test  - left) > worst_left)
			worst_left=ov;
	}else if ( right >= m_stk[test_dex].p_pos){
		if (right <= (right_test = m_stk[test_dex].p_pos +
         m_stk[test_dex].length -1) ){
			if ( (ov = right - m_stk[test_dex].p_pos) > worst_right)
				worst_right = ov;
		}
	}
}

#ifdef DEBUG_DETAIL_BEST
printf("DEBUG_DETAIL_BEST worse left %d worse right %d\n",
worst_left,worst_right);
fflush(stdout);
#endif

if ( (bases_remaining= bases - (worst_left + worst_right))  >0){
	bases=bases_remaining;
}else{
	bases=0;
}

return bases;
}

/* ========================================
 ----- find_next_best () --
* ======================================
*
* Description: finds best next index to the interval
*	which adds the most bases (not counting overlaps)
*	to those intervals already known.
*
*-------------------------------------
* Well, to make it work on more things, some additional
* features have been added to make it less greedy.
* If no overlaps > TooShort in aa positions have 
* been found, weirdness, in terms of strand switches and
* reversals on a strand, will be overlooked.
* However, once these have been found, straightness
* (an absense of above weirdness) will be inforced.
* 
*
* Return Value: index into m_stk[] to next best interval
*
* Input Via Formal Parameters: 
*
* Output Via Formal Parameters: 
*
* Input Via Global Variables: m_stk[] m_top
*
* Output Via Global Variables: 
 ========================================*/
static Int4 find_next_best(Int4 indices_in_best_set[], Int4 num_in_best_set, Int4 pass_number, Int4 TooShort)
{
Int4 ret_ind = AINT_NONE;
Int4 best_base_ct = 0;
Int4 this_base_ct;
Int4 m_dex;
Int4 best_ind_so_far = AINT_NONE;
Int4 this_is_weird;


#ifdef DEBUG_FIND_BEST_SET
printf("DEBUG_FIND_BEST_SET num %d: ",
num_in_best_set);
for (m_dex=0; m_dex < num_in_best_set; m_dex ++)
	printf("%d ", indices_in_best_set[m_dex]);
printf("\n");
fflush(stdout);
#endif

for ( m_dex=0; m_dex < m_top; m_dex ++){
	if ( in_list (m_dex, indices_in_best_set, num_in_best_set) ) 
		continue;
	if (does_not_add_enough(m_dex,indices_in_best_set,
			num_in_best_set, pass_number, TooShort) )
		continue;
	if ( (this_is_weird = is_this_too_weird( m_dex,
			indices_in_best_set, num_in_best_set, pass_number) ) ){
			Weirdness_found ++;
		if (pass_number > 0)
			continue;
	}
	if ( (this_base_ct = 
			base_count(m_dex, indices_in_best_set, num_in_best_set)) 
			> best_base_ct){
#ifdef DEBUG_DETAIL_BEST
printf("DEBUG_DETAIL_BEST old base %d this %d, %d\n",
best_base_ct, this_base_ct, m_dex);
fflush(stdout);
#endif
		best_base_ct = this_base_ct;
		best_ind_so_far = m_dex;
	}

#ifdef DEBUG_FIND_BEST_SET
printf("DEBUG_FIND_BEST_SET index %d this base %d best_base %d best dex %d\n",
m_dex, this_base_ct, best_base_ct, best_ind_so_far);
fflush(stdout);
#endif

}
ret_ind=best_ind_so_far; 	
#ifdef DEBUG_FIND_BEST_SET
printf("DEBUG_FIND_BEST_SET Returning %d\n",ret_ind);
fflush(stdout);
#endif
return ret_ind;
}

/*
* =================================================================
 ----- COMPARE_ints () --
* ======================================
*
* Description: for qsort() in ascending order
*
* Return Value: comparing the score only, returns:
			-1 if the first score is < the second
        0 if equal
			+1 is the first score is > the second
*
* Input Via Formal Parameters: 2 pointers to ints structures
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: none
*
* Output Via Global Variables: 
* 
*===============================================*/
static int COMPARE_ints (const void *x1, const void *x2)
{
Int4 retval =0;
Int4 *p1;
Int4 *p2;
p1 = (Int4 *) x1;
p2 = (Int4 *) x2;
if ( * p1 < * p2 ){
	retval = -1;
}else if ( *p1 > *p2 ){
	retval =1;
}

return retval;

}

/* =================================================================
 ----- find_best_set () --
* ======================================
*
* Description: finds best subset of intervals
*              This code find one set that can be processed to
*              become a consistent set.  It is basically greedy
*              and just tries to find a set which uses the maximum
*              bases.  Overlaps are not fixed here.  ensure_consistent
*              set will make sure that there are not overlaps and
*              that the set needs no further processing.
*
* Return Value: 
*
* Input Via Formal Parameters: 
*
* Output Via Formal Parameters: 
*
* Input Via Global Variables: m_stk[] m_top
*
* Output Via Global Variables: m_stk[] m_top
* 
*===============================================*/
extern void find_best_set (Int4 TooShort, Int4 pro_len) 
{
Int4 * indices_in_best_set;
Int4 num_in_best_set = 0, next_best_index, ind_stk;
Int4 ind_best;
Int4 index_rm;
Int4 pass_number=0;
Int4 min_length;
Int4 length_in_best_set;
#ifdef DEBUG_FIND_BEST_SET
char msg [120];
#endif
#ifdef DEBUG_FIND_BEST_SET_KILL
Int4 ind_bestx;
#endif

indices_in_best_set = (Int4* ) MemNew( sizeof(Int4) * m_top);

if (pro_len < 10 * TooShort){
	min_length = TooShort;
}else if (pro_len > 40*TooShort){
	min_length = 4 * TooShort;
}else{
	min_length = pro_len/10;
}
while ( pass_number < 2){
	length_in_best_set = 0;
	num_in_best_set=0;
#ifdef DEBUG_FIND_BEST_SET
sprintf(msg,"DEBUG_FIND_BEST_SET before pass %d", pass_number);
dump_m_stk(stdout,msg);
fflush(stdout);
#endif
Weirdness_found=0;
	while ( (next_best_index = 
		find_next_best( indices_in_best_set, num_in_best_set, 
			pass_number, TooShort))!= AINT_NONE){
		if ( pass_number ==0 &&
				 m_stk[next_best_index].length < min_length) break;
#ifdef DEBUG_FIND_BEST_SET
printf("DEBUG_FIND_BEST_SET next is %d\n", next_best_index);
fflush(stdout);
#endif

		indices_in_best_set [ num_in_best_set  ++] = next_best_index;
		length_in_best_set += m_stk [next_best_index].length;
		}
/*----
 * If in weirdness allowed pass, how decide is good enough?
 *---*/
		if ( pass_number == 0){
/* #define DONOT_ACT_NORMAL*/
#ifndef DONOT_ACT_NORMAL
			if ( (double) num_in_best_set > (double) m_top * 0.78  &&
			length_in_best_set >= pro_len * .995	){
#endif
					/*
					if (Weirdness_found){
						sprintf(msg,"(echo \" aa_id_seqs: aa %d passed suggest pass 1 with weirdness\" for `whoami` at `date` | /bin/mail sirotkin)&",
						0);
						system(msg);
					}
					*/
					 break;
#ifndef DONOT_ACT_NORMAL
				}
#endif
		}
	pass_number ++;
}
qsort ( (char *) indices_in_best_set, num_in_best_set,
	sizeof (Int4), COMPARE_ints);

#ifdef DEBUG_FIND_BEST_SET
printf("DEBUG_FIND_BEST_SET best set after qsort (%d): ", 
num_in_best_set);
for ( ind_best=0 ; ind_best< num_in_best_set; ind_best ++ )
	printf("%d ", indices_in_best_set [ind_best] );
printf("\n");
fflush(stdout);
#endif

	for (ind_stk = 0, ind_best=0 ; ind_stk < m_top ; ind_stk ++ ){
		if ( indices_in_best_set [ind_best] == ind_stk) {
			if (ind_best +1 < num_in_best_set) {
				ind_best ++;
			}
			continue;
		}
		m_top -- ;
#ifdef DEBUG_FIND_BEST_SET
printf("DEBUG_FIND_BEST_SET did not find %d (%d), killing %d now %d:\n",
indices_in_best_set [ind_best], ind_best, ind_stk, m_top);
fprintf_m_stk_entry(stdout,ind_stk, "killing");
fflush(stdout);
#endif
		for (index_rm = ind_stk; index_rm < m_top ; index_rm ++ ){
			copy_m_stk_entry (index_rm, index_rm+1);
		}
		if (ind_stk < num_in_best_set){
#ifdef DEBUG_FIND_BEST_SET_KILL
printf("DEBUG_FIND_BEST_SET_KILL adjusting best from %d ind_stk %d num best %d\n",
ind_best, ind_stk, num_in_best_set);
#endif
			for (index_rm = ind_best ; 
				index_rm < num_in_best_set ; index_rm ++ )
				indices_in_best_set [index_rm] -- ;
		}else{
			m_top = num_in_best_set;
			break;
		}

#ifdef DEBUG_FIND_BEST_SET_KILL
dump_m_stk(stdout,"DEBUG_FIND_BEST_SET_KILL after kill");
printf("DEBUG_FIND_BEST_SET_KILL best set after kill (%d): ", 
num_in_best_set);
for ( ind_bestx=0 ; ind_bestx< num_in_best_set; ind_bestx ++ )
	printf("%d ", indices_in_best_set [ind_bestx] );
printf("\n");
fflush(stdout);
#endif

	ind_stk --;
	}

MemFree(indices_in_best_set);

#ifdef DEBUG_FIND_BEST_SET
dump_m_stk(stdout,"DEBUG_FIND_BEST_SET after ");
fflush(stdout);
#endif

}

/* =================================================================
 ----- test_multiple_unassigned () --
* ======================================
*
* Description: 
This routine looks for multiple unassigned amino acids and 
returns the length of the stretch.  The DNA stretch must be
3 X (in bases) the number of codons.
*
* Return Value: stretch of unassigned that can be accounted for by
*               multiple mismatches
*
* Input Via Formal Parameters: index position with m_stk[]
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: m_stk[], m_top
*
* Output Via Global Variables: 
* 
*===============================================*/

static Int4 test_multiple_unassigned_give2(Int4 lower, Int4 i)
{
	Int4 j, k, right_exon_starts, left_exon_ends;
	Int4 retval = 0, maybe_retval;

		if (i > 0) if( (j = m_stk[lower].p_pos + m_stk[lower].length-1) <= 
		    (k = m_stk[i].p_pos) - 1){

		if (  m_stk[i].orient == m_stk[lower].orient){

		maybe_retval = k - j-1;

/*	we have a single or multiple unassigned codon(s) */
			right_exon_starts = 3*m_stk[i].dna_pos 
				+ m_stk[i].fr_shift + 1;
			left_exon_ends = 3*m_stk[lower].dna_pos 
				+ m_stk[lower].fr_shift + 1
				+ 3*m_stk[lower].length - 1;
			if (right_exon_starts-left_exon_ends == 1 + 3*maybe_retval){
/*-- DUE to mismatch(s) ---*/
				retval = maybe_retval;
				}
			}
	}

return retval;
}

static Int4 test_multiple_unassigned(Int4 i)
{
Int4 retval =0;

		if (i>0) retval = test_multiple_unassigned_give2(i-1,i);
		return retval;
}

/* =================================================================
 ----- remove_redundant_intervals () --
* ======================================
*
* Description: 
This routine looks for intervals which are unnecessary and
are overlapped by intervals on both sides, at least one of
which is longer./
*
* Return Value: none
*
* Input Via Formal Parameters: none
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: m_stk[]
*
* Output Via Global Variables: m_stk[]
* 
*===============================================*/

/* SPLIT_CODON_FOR_UNASSIGNED (added function) */

extern void remove_redundant_intervals (Int4 TooShort)
{
	Int4 i, lower_ends; 
	Int4 k_scan, una;

#ifdef DEBUG_REDUNDANT 
dump_m_stk(stdout,"DEBUG_REDUNDANT  before ");
fflush(stdout);
#endif

	for (i = 1; i < m_top-1 ; ++i) { 
/* only need consider internal intervals */

		if( m_stk[i-1].length  <  m_stk[i].length &&
				m_stk[i+1].length  <  m_stk[i].length) continue;

		if (m_stk[i-1].orient != m_stk[i].orient ||
			m_stk[i+1].orient != m_stk[i].orient) continue;

		lower_ends = m_stk[i-1].p_pos + m_stk[i-1].length-1;

		if (lower_ends +1 >= m_stk[i+1].p_pos){
#ifdef DEBUG_REDUNDANT 
printf("DEBUG_REDUNDANT lower ends flush with upper\n");
#endif
			m_top --;
			for (k_scan = i; k_scan < m_top ; k_scan ++){
				copy_m_stk_entry (k_scan,k_scan+1);
			}
			i --; /* look at interval just copied into ith position */
		}else{
			una = test_multiple_unassigned_give2(i-1, i+1);
			if ( una && una < TooShort  + 4){
#ifdef DEBUG_REDUNDANT 
printf("DEBUG_REDUNDANT lower overlaps upper with %d mismatches\n",una);
#endif
				m_top --;
				for (k_scan = i; k_scan < m_top ; k_scan ++){
					copy_m_stk_entry (k_scan,k_scan+1);
				}
				i --; /* look at interval just copied into ith position */
			}
		}
	}

#ifdef DEBUG_REDUNDANT 
dump_m_stk(stdout,"DEBUG_REDUNDANT  after ");
fflush(stdout);
#endif

}

/* =================================================================
 ----- score_possible_splice_sites () --
* ======================================
*
* Description: see name
*
* Return Value: score
*
* Input Via Formal Parameters: either dna seq or its reverse, and
                               putative intron ends
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: vx[] (for value lookup) 
*
* Output Via Global Variables: 
* 
*===============================================*/
static Int4 score_possible_splice_sites(char *seq_or_rev, Int4 putative_donor, Int4 putative_acceptor)
{
Int4 score = 0;

/*----looking for g at intron ends ---*/
/* plus 1 means the intron is left +2 and right -3 */

score = ( vx[seq_or_rev[putative_donor]]==3) ?10:0;
score += (vx[seq_or_rev[putative_acceptor]]==3 )?10:0;

/*---looking for exon/gt intron ag/exon ---*/
score += ( vx[seq_or_rev[putative_donor+1]]==0 )?3:0;
score += ( vx[seq_or_rev[putative_acceptor-1]]==2 )?3:0;

#ifdef DEBUG_SCORE
printf("DEBUG_SCORE is %d at %d donor is %c%c at %d acceptor is %c%c\n",
score, putative_donor,seq_or_rev[putative_donor],seq_or_rev[putative_donor+1],
putative_acceptor,seq_or_rev[putative_acceptor-1],seq_or_rev[putative_acceptor]);
fflush(stdout);
#endif

return score;
}

/* =================================================================
 ----- make_split_choice () --
* ======================================
*
* Description: 
	This function makes a choice when either 1 or 2 bases from
	left exon can be added to explain the unassigned amino acid
*
* Return Value: number of bases to split exon
*
* Input Via Formal Parameters: either dna seq or its reverse, and
*                              left and right starting postion of 
*                              intron
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: vx[] (for value lookup)
*
* Output Via Global Variables: 
* 
*===============================================*/
/* SPLIT_CODON_FOR_UNASSIGNED (added function)*/
static Int4 make_split_choice(char *seq_or_rev, Int4 left_ends, Int4 right_starts) /* base position of last exon position */
{
Int4 retval = 1;
Int4 score1, score2;

/* the vx[] array is used in get_frames() and elsewhere to
turn a base character sequence into a codon score.  We
use it here because it resolves the upper and lower case
nucleotides nicely*/

/* of course a more complex alignment could be used, this
just scores +10 for 'g' at both boundaries and +3
for 'nt' at donor and 'an' at acceptor */

#ifdef DEBUG_SPLIT_CODON_FOR_UNASSIGNED
	printf ("SPLIT_CODON_FOR_UNASSIGNED both look good!\n");
fflush(stdout);
#endif

score1 = score_possible_splice_sites( seq_or_rev,left_ends+2, right_starts-3);
score2 = score_possible_splice_sites( seq_or_rev,left_ends+3, right_starts-2);

if (score2 > score1) retval = 2;

return retval;
}

/*
* =================================================================
 ----- check_split_codon () --
* ======================================
*
* Description:  Only called for single unassigned aa.
* This function handles the logic of determining if a split codon
* can complete the aa to nucleotide alignment
* 
* In the case of either split explaining amino acid, 
* make_split_choice()
* is called to resolve the choice.
*
* Return Value: number of bases to split exon
*
* Input Via Formal Parameters: position that is unassigned,
                               dna seq or its complement,
                               intron boundaries
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: 
*
* Output Via Global Variables: 
* 
*===============================================*/

/* SPLIT_CODON_FOR_UNASSIGNED (added function) */


static Int4 check_split_codon(Int4 aa_pos_to_explain, char *seq_or_rev, Char *p_seq,
                              Char *genetic_code, Int4 left_ends, Int4 right_starts) /* base position of last exon position */
{
/*
the vx[] and val stuff are used as in get_frames()
*/

Int4 retval=0;
Int4 val_assuming_1_from_left;
Int4 val_assuming_2_from_left;

char code_assuming_1_from_left=0;
char code_assuming_2_from_left=0;

left_ends --; right_starts--;
val_assuming_1_from_left= vx[seq_or_rev[left_ends+1]] * 16 +
	vx[seq_or_rev[right_starts-2]]*4+
	vx[seq_or_rev[right_starts-1]];
if (val_assuming_1_from_left>=0) code_assuming_1_from_left=
	genetic_code[val_assuming_1_from_left];

#ifdef DEBUG_SPLIT_CODON_FOR_UNASSIGNED
printf ("SPLIT_CODON_FOR_UNASSIGNED assuming 1 %c%c%c %d %d %d \n",
seq_or_rev[left_ends+1], seq_or_rev[right_starts-2], seq_or_rev[right_starts-1],
vx[seq_or_rev[left_ends+1]], vx[seq_or_rev[right_starts-2]], vx[seq_or_rev[right_starts-1]]);
fflush(stdout);
#endif

val_assuming_2_from_left= vx[seq_or_rev[left_ends+1]] * 16 +
	vx[seq_or_rev[left_ends+2]] * 4 +
	vx[seq_or_rev[right_starts-1]];
if (val_assuming_2_from_left>=0) code_assuming_2_from_left=
	genetic_code[val_assuming_2_from_left];

#ifdef DEBUG_SPLIT_CODON_FOR_UNASSIGNED
printf ("SPLIT_CODON_FOR_UNASSIGNED assuming 2 %c%c%c %d %d %d \n",
seq_or_rev[left_ends+1], seq_or_rev[left_ends+2], seq_or_rev[right_starts-1],
vx[seq_or_rev[left_ends+1]], vx[seq_or_rev[left_ends+2]], vx[seq_or_rev[right_starts-1]]);
fflush(stdout);
#endif

#ifdef DEBUG_SPLIT_CODON_FOR_UNASSIGNED
	printf ("SPLIT_CODON_FOR_UNASSIGNED looking for %c at %d: left starts %d right ends %d \n\
val 1 %d code 1 %c%c%c  %c val 2 %d code 2 %c%c%c %c \n", p_seq[aa_pos_to_explain],
aa_pos_to_explain, left_ends, right_starts,
val_assuming_1_from_left,
seq_or_rev[left_ends+1], seq_or_rev[right_starts-2],
	seq_or_rev[right_starts-1], code_assuming_1_from_left,
val_assuming_2_from_left, 
seq_or_rev[left_ends+1], seq_or_rev[left_ends+2],
	seq_or_rev[right_starts-1], code_assuming_2_from_left
);
fflush(stdout);
#endif
if (p_seq[aa_pos_to_explain]==code_assuming_1_from_left &&
	p_seq[aa_pos_to_explain]==code_assuming_2_from_left){
/*--both ways explain missing amino acid need to choose --*/
	retval=make_split_choice(seq_or_rev,left_ends,right_starts);
}else if (p_seq[aa_pos_to_explain]==code_assuming_1_from_left){
	retval = 1;
}else if (p_seq[aa_pos_to_explain]==code_assuming_2_from_left){
	retval = 2;
}
return retval;
}

/* =================================================================
 ----- fix_single_unassigned () --
* ======================================
*
* Description: 
This routine looks for single unassigned amino acids and attempts
to assign them by assuming split codons at exon-exon boundaries.

This routine handles the overhead of finding and setting, the
main logic is in the new function check_split_codon()

Additionally, if a single mismatched base accounts for the
problem, the two intervals are coalesced.

Since the intervals to be combined can be a distance 
away from one another, this has to be an O(n^2) algorithm
*
* Return Value: none
*
* Input Via Formal Parameters: none
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: m_stk[]
*
* Output Via Global Variables: m_stk[]
* 
*===============================================*/

/* SPLIT_CODON_FOR_UNASSIGNED (added function) */

extern void fix_single_unassigned(Char *dna_seq, Char *rev_seq, Char *p_seq, Char *genetic_code)
{
	Int4 i, j, k, right_exon_starts, left_exon_ends;
	Int4 k_scan;
/* this for (;;) loop was mostly lifted from the subsequenct
report() function.  If the variable names seem unclear, etc. 
that is history.
*/

#ifdef DEBUG_SPLIT_CODON_FOR_UNASSIGNED
dump_m_stk(stdout,"DEBUG_SPLIT_CODON_FOR_UNASSIGNED before ");
fflush(stdout);
#endif

	for (i = 0; i < m_top; ++i) {
		/* just for completeness */
		m_stk[i].left_add_bases = 
		m_stk[i].right_add_bases = (char) 0;

		if (i > 0 ) if( (j = m_stk[i-1].p_pos + m_stk[i-1].length - 1) < 
		    (k = m_stk[i].p_pos) ){

/*  in old loop, the following was executed:unassigned(j+1, k-1); */
		if ( j+1 == k-1 && m_stk[i].orient == m_stk[i-1].orient){

#ifdef DEBUG_SPLIT_CODON_FOR_UNASSIGNED
printf ("SPLIT_CODON_FOR_UNASSIGNED Unassigned at %d (%c) \n", 
j+1, p_seq[j+1]);
fflush(stdout);
#endif

/*	we have a single unassigned codon (aa) */
			right_exon_starts = 3*m_stk[i].dna_pos 
				+ m_stk[i].fr_shift + 1;
			left_exon_ends = 3*m_stk[i-1].dna_pos 
				+ m_stk[i-1].fr_shift + 1
				+ 3*m_stk[i-1].length - 1;
			if (right_exon_starts-left_exon_ends != 4){
/*-- not due to mismatch ---*/
						if( (m_stk[i-1].left_add_bases=check_split_codon (
					j+1, (m_stk[i].orient==SuggestMinusStrand)?rev_seq:dna_seq, p_seq,
					genetic_code, left_exon_ends, right_exon_starts) )) 
						m_stk[i].right_add_bases =
							3 - m_stk[i-1].left_add_bases;
			}else{
/*-- DUE to mismatch ---*/
/*-- single unassigned codon and 3 dna bases 
		... combine two intervals --*/
/*-- i is index on m_stk[], and the i-1 entry and this one need to be combined */
				i--;
				m_stk[i].length += m_stk[i+1].length+1;
#ifdef DEBUG_COALESCE_MISMATCH
printf("DEBUG_COALESCE_MISMATCH coalescing %d (%d) and %d (%d) new len %d\n",
m_stk[i].p_pos,i,
m_stk[i+1].p_pos,i+1, m_stk[i].length);
fflush(stdout);
#endif
				m_top --;
				for (k_scan = i+1; k_scan < m_top ; k_scan ++){
					copy_m_stk_entry (k_scan,k_scan+1);
				}
			}
		}
}
	}

#ifdef DEBUG_SPLIT_CODON_FOR_UNASSIGNED
dump_m_stk(stdout,"DEBUG_SPLIT_CODON_FOR_UNASSIGNED after ");
fflush(stdout);
#endif

}

/* ===================================================
 ----- combine_with_long_mismatches () --
* ======================================
*
* Description: 
This routine looks for single unassigned amino acids only due
to mismatches
If a single mismatched base accounts for the
problem, the two intervals are coalesced.

  Since the intervals to be combined can be a distance
  away from one another, this has to be an O(n^2) algorithm
*
* Return Value: number of intervals combined
*
* Input Via Formal Parameters: none
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: m_stk[]
*
* Output Via Global Variables: m_stk[]
* 
*===============================================*/


extern Int4 combine_with_long_mismatches(Int4 TooShort)
{
	Int4   una,  upper;
	Int4 k_scan;
	Int4 retval=0;

#ifdef DEBUG_MULIPLE_MISMATCH 
dump_m_stk(stdout,"DEBUG_MULIPLE_MISMATCH  before ");
fflush(stdout);
#endif

if (m_top > 1)
	for (upper = 1; upper < m_top; ++upper) {
		if ( m_stk[upper].orient == m_stk[upper-1].orient){
			una = test_multiple_unassigned(upper);
			if ( una && una < TooShort  + 4){

#ifdef DEBUG_MULIPLE_MISMATCH
printf ("DEBUG_MULIPLE_MISMATCH Unassigned at %d (%c) %d long \n", 
upper, p_seq[upper], una);
fflush(stdout);
#endif

/*	we have multiple unassigned codons  */
m_stk[upper-1].length += una + m_stk[upper].length;

#ifdef DEBUG_MULIPLE_MISMATCH
printf("DEBUG_MULIPLE_MISMATCH coalescing %d (%d) and %d (%d) new len %d\n",
m_stk[upper-1].p_pos,upper-1,
m_stk[upper].p_pos,upper, m_stk[upper-1].length);
fflush(stdout);
#endif
				m_top --;
				retval ++;
				for (k_scan = upper; k_scan < m_top ; k_scan ++){
					copy_m_stk_entry (k_scan,k_scan+1);
				}
				upper --; /* try for another */
			}
		}
	}

#ifdef DEBUG_MULIPLE_MISMATCH
dump_m_stk(stdout,"DEBUG_MULIPLE_MISMATCH after ");
fflush(stdout);
#endif

return retval;
}

/*
* =================================================================
 ----- COMPARE_alternatives () --
* ======================================
*
* Description: for qsort() in desending order
*
* Return Value: comparing the score only, returns:
			-1 if the first score is > the second  (reverse of documentaiton, 'cause
												is desending
        0 if equal
			+1 is the first score is < the second
*
* Input Via Formal Parameters: 2 pointers to alternatives structures
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: none
*
* Output Via Global Variables: 
* 
*===============================================*/
static int COMPARE_alternatives (const void *x1, const void *x2)
{
Int4 retval =0;
struct alternative_splice *p1;
struct alternative_splice *p2;

p1 = (struct alternative_splice *) x1;
p2 = (struct alternative_splice *) x2;
if ( p1 -> score > p2 -> score ){
	retval = -1;
}else if ( p1 -> score < p2 -> score){
	retval =1;
}

return retval;

}

/*
* =================================================================
 ----- overlapping_fixer () --
* ======================================
*
* Description: In the case of flush or overlapping assignments,
*              check if looking at the splice sites can fix or
*              improve the assignments.
*
* could be split in drive which alls find_alternatives,
* qsort and choose_aalternatives, but if it ain't broke ...
*
* Return Value: none
*
* Input Via Formal Parameters: dna seq or its complement,
*                              reference index in m_stk[]
                               relevant aa positions
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: m_stk[]
*
* Output Via Global Variables: m_stk[]
* 
*===============================================*/

/*-----------------------------
*
*	The logic of what is being done is tricky in the +/- 1 sense,
*	so imagine that there are two sets of aa intervals:
*	
*		1 - 19   and   20 - 30
* or   1 - 20   and   20 - 50
*
*	In the following picture, imagine that the vertical lines '|' stand
*	for extra bases in the split codons being tested.  The following
*	possibilities need to be tested in the fist case:
*
*		18| ||20
*		18|| |20
*		19    20  (what was thought to be only alternative in original code)
*		19| ||21
*		19|| |21
*
*	(  20    21 need not be checked, as the code would have expanded
*	*  1 - 19 to 1 - 20, if that would lead to the same aa )
*
*	In the second case, we start with the same, and add
*
*		20    21
*		20| ||22
*		20|| |22
*
*	The number of alternatives to check is
*	3 X (number of bases of overlap +2) -1.
*	Since the number of bases of overlap is equal to position where
*	the left aa ends - right aa begins +1, the number of places to
*	check is the above, with "+3" replacing "+1".
-------------*/

static void overlapping_fixer (char *seq_or_rev, Char *p_seq, Char *genetic_code, Int4 index,
                               Int4 left_aa_ends, Int4 right_aa_begins)
{
Int4 num_places_to_check = left_aa_ends - right_aa_begins +3;
Int4 max_number_alternatives = 3 * num_places_to_check;
Int4 number_alternatives=0;
Int4 split_now;
Int4 try_left_end;
Int4 shifted_right_while_checking;
Int4 test_val;
char test_code;
Int4 right_exon_starts, left_exon_ends;
Int4 aa_change_score;
Int4 change_in_left_position ;
#ifdef DEBUG_SCORE
Int4 this_alt;
#endif

#ifdef DEBUG_FIX_OVERLAPPING_ASSIGNMENTS
printf("DEBUG_FIX_OVERLAPPING_ASSIGNMENTS left end %d right end %d num %d max num %d \n", 
left_aa_ends, right_aa_begins, num_places_to_check, 
max_number_alternatives);
#endif
if (max_number_alternatives){
	alternatives = (struct alternative_splice *) MemNew( 
		sizeof(struct alternative_splice) * max_number_alternatives);

/* --- see the comment above to understand the settings for
*  right_exon_starts and left_exon_ends.  These variables will contain
*  values one to the left of the first place to check
*-------*/

	try_left_end = right_aa_begins-2 ;
 shifted_right_while_checking=0; 
	right_exon_starts = 
		3*m_stk[index].dna_pos + m_stk[index].fr_shift + 1
		- 1   ; 
	left_exon_ends =
		3*m_stk[index-1].dna_pos + m_stk[index-1].fr_shift + 1 + 
		3*m_stk[index-1].length - 1 
		-3 * (num_places_to_check-1);
#ifdef DEBUG_FIX_OVERLAPPING_ASSIGNMENTS
printf("DEBUG_FIX_OVERLAPPING_ASSIGNMENTS left end %d right begin %d num %d\n",
left_aa_ends,right_aa_begins,num_places_to_check);
fflush(stdout);
#endif
	for (	; shifted_right_while_checking < num_places_to_check;
		try_left_end ++ , shifted_right_while_checking++ , right_exon_starts += 3, left_exon_ends += 3 ){
		for (split_now=1; split_now < 4; split_now ++){
			aa_change_score = 30;
			if (split_now != 3){

/*--must maintain aa coded for by putative split--*/
				test_code=0;
				test_val = vx[seq_or_rev[left_exon_ends]]*16 +
					vx[seq_or_rev[right_exon_starts-1]];
				if (split_now ==1){
					test_val += vx[seq_or_rev[right_exon_starts-2]]*4;
				}else{
					test_val += vx[seq_or_rev[left_exon_ends+1]]*4;
				}
				if (test_val >=0) test_code = genetic_code [test_val];
					if ( test_code != p_seq[try_left_end+1]){ 
#ifdef DEBUG_SCORE
printf("DEBUG_SCORE right %d frame %d NOT equal aa %c predicted %c (%d)\
try left %d %c %c%c %c\n",
shifted_right_while_checking, split_now, p_seq[try_left_end+1],
test_code, test_val,
try_left_end, seq_or_rev[left_exon_ends],
seq_or_rev[left_exon_ends+1],
seq_or_rev[right_exon_starts-2], 
seq_or_rev[right_exon_starts-1]);
#endif
						aa_change_score=1;;

					}else{
#ifdef DEBUG_SCORE
printf("DEBUG_SCORE right %d frame %d EQUAL aa %c predicted %c (%d)\
try left %d %c %c%c %c\n",
shifted_right_while_checking, split_now, p_seq[try_left_end+1],
test_code, test_val,
try_left_end, seq_or_rev[left_exon_ends],
seq_or_rev[left_exon_ends+1],
seq_or_rev[right_exon_starts-2], 
seq_or_rev[right_exon_starts-1]);
#endif
					}

			}else{
/*--------
*	The below "continue" could be a "break" as well.  If this is
*	not clear, also note that this is preventing the check of the
*	last unnecessary alternative described in the block comment before
*	the code for this function.
*-------*/
		 		if (shifted_right_while_checking == num_places_to_check-1) 
					continue;
			}
/*---the aa coded for is maintained, or score will
		be 1/10 score this guy ---*/
			alternatives[number_alternatives].from_left =
				shifted_right_while_checking;
			alternatives[number_alternatives].split_left=split_now;

			alternatives[number_alternatives ++ ].score=
				aa_change_score + aa_change_score * score_possible_splice_sites (seq_or_rev,
					left_exon_ends+split_now ,
					right_exon_starts + split_now -4 ) ;
		}
	}

#ifdef DEBUG_SCORE
printf("DEBUG_SCORE alternatives before sort:\n\n  # >>>> f score\n");
for (this_alt =0; this_alt < number_alternatives; this_alt ++ ){
	printf ("%3d %3d %d %3d\n",this_alt,alternatives[this_alt].from_left,
	alternatives[this_alt].split_left,alternatives[this_alt].score);
}
fflush(stdout);
#endif
	qsort ( (char *)alternatives, number_alternatives, 
		sizeof (struct alternative_splice), COMPARE_alternatives);

#ifdef DEBUG_FIX_OVERLAPPING_ASSIGNMENTS
printf("DEBUG_SCORE alternatives after sort:\n\n  # >>>> f score\n");
for (this_alt =0; this_alt < number_alternatives; this_alt ++ ){
	printf ("%3d %3d %d %3d\n",this_alt,alternatives[this_alt].from_left,
	alternatives[this_alt].split_left,alternatives[this_alt].score);
}
fflush(stdout);
#endif

#ifdef INSIST_ON_POSITIVE_SCORE
if (alternatives[0].score > 0)
#endif
											{
		change_in_left_position = num_places_to_check - 
			alternatives[0].from_left -1;
		if (alternatives[0].split_left==3){
			change_in_left_position -- ;
		}

#ifdef DEBUG_FIX_OVERLAPPING_ASSIGNMENTS
fprintf_m_stk_entry(stdout,index-1, "DEBUG_FIX_OVERLAPPING_ASSIGNMENTS before ");
fprintf_m_stk_entry(stdout,index, "DEBUG_FIX_OVERLAPPING_ASSIGNMENTS before ");
fflush(stdout);
#endif
		m_stk[index-1].left_add_bases  = alternatives[0].split_left %3;
 	m_stk[index].right_add_bases = 3-alternatives[0]. split_left;
		m_stk[index-1].length -= change_in_left_position;
		m_stk[index].length -= alternatives[0].from_left;
		m_stk[index].p_pos += alternatives[0].from_left;
		m_stk[index].dna_pos += alternatives[0].from_left;

#ifdef DEBUG_FIX_OVERLAPPING_ASSIGNMENTS
fprintf_m_stk_entry(stdout,index-1, "DEBUG_FIX_OVERLAPPING_ASSIGNMENTS after ");
fprintf_m_stk_entry(stdout,index, "DEBUG_FIX_OVERLAPPING_ASSIGNMENTS after ");
dump_m_stk(stdout,"DEBUG_FIX_OVERLAPPING_ASSIGNMENTS after ");
fflush(stdout);
#endif
}
	MemFree(alternatives);
}
}

/* =================================================================
 ----- fix_overlaps_by_splice_sites () --
* ======================================
*
* Description: Overlapping assignments are fixed, if aa is preserved
* and  /g INTRON g/ or even /ng ..intron ..an/
*
* Return Value: none
*
* Input Via Formal Parameters: none
*
* Output Via Formal Parameters: none
*
* Input Via Global Variables: m_stk[]
*
* Output Via Global Variables: m_stk[]
* 
*===============================================*/

extern void fix_overlaps_by_splice_sites(Char *dna_seq, Char *rev_seq, Char *p_seq, Char *genetic_code)
{
	Int4 i, j, k;
/* this for (;;) loop was mostly lifted from the subsequenct
report() function.  If the variable names seem unclear, etc. 
that is history.
*/

	for (i = 1; i < m_top; ++i) {
/*---since we want to find flush ends, the additional -1
* 	was added this will cause the else cause to 
* 	fire if j+1==k  
---*/
#ifdef DEBUG_FIX_OVERLAPPING_ASSIGNMENTS
printf("DEBUG_FIX_OVERLAPPING_ASSIGNMENTS pos %d len %d > pos %d \n",
m_stk[i-1].p_pos, m_stk[i-1].length, m_stk[i].p_pos);
#endif
		if( (j = m_stk[i-1].p_pos + m_stk[i-1].length - 1)   >=
		    (k = m_stk[i].p_pos) - 1){

/*------FIX OVERLAPPING ASSIGNMENTS 
* 	(and compare flush assignments for splicing alternatives) 
---*/

	if (m_stk[i].orient == m_stk[i-1].orient)
		overlapping_fixer((m_stk[i].orient==SuggestMinusStrand)?rev_seq:dna_seq,
			p_seq, genetic_code, i, j, k);
}
	}
}

/* =================================================================
 ----- test_for_collision () --
* ======================================
*
* Description: test for inconsistent exons
* Return Value: 1 if collision found, 0 if ok
*
* Input Via Formal Parameters: 
*
* Output Via Formal Parameters: 
*
* Input Via Global Variables: m_stk[]
*
* Output Via Global Variables: 
* 
*===============================================*/
static Int4 test_for_collision (Int4 test_this_interval, Int4 try_start, Int4 last_ok_interval)
{
Int4 last,first;
Int4 retval = 0;

/*--this logic and code originally copied from 
				the tests in report() */

/*--CHECK FOR ORIENTATION IN FIND_BEST_SET NOW ! ! !
----------different orientations -----------------
		if (m_stk[test_this_interval].orient != 
			m_stk[try_start].orient){
#ifdef DEBUG_DETAIL_COLLIDE
printf("DEBUG_DETAIL_COLLIDE on orientation\n");
#endif
			  retval = 1;	
		}
||||||||||||||||||||| */

/* overlapping nucleotide sequences*/
		last = 3*m_stk[last_ok_interval].dna_pos + 
			m_stk[last_ok_interval].fr_shift + 
			3*m_stk[last_ok_interval].length - 1;
	/* first dna position of current exon */
		first = 3*m_stk[test_this_interval].dna_pos + 
			m_stk[test_this_interval].fr_shift;
		if (last>=first){
#ifdef DEBUG_DETAIL_COLLIDE
printf("DEBUG_DETAIL_COLLIDE on  nucleotide \n");
#endif
			 retval = 1;	
		}

/* overlapping protein sequences */
/* last protein position for previous exon */
		last = m_stk[last_ok_interval].p_pos + 
			m_stk[last_ok_interval].length - 1;
		/* first protein position for current exon */
		first = m_stk[test_this_interval].p_pos;
		if (last >= first){
#ifdef DEBUG_DETAIL_COLLIDE
printf("DEBUG_DETAIL_COLLIDE on protein\n");
#endif
			 retval = 1; 
		}
/*-----------end borrowed logic ---------*/
#ifdef DEBUG_FIND_SET
printf("DEBUG_FIND_SET for this %d, last ok %d, base %d %s\n",
test_this_interval, last_ok_interval, try_start,
(retval?"collided":"ok"));
fflush(stdout);
#endif
return retval;
}

/*===========================================
 ----- ensure_consistent_set () --
* ======================================
*
* Description: finds consistent subset of intervals
*
* Return Value: 
*
* Input Via Formal Parameters: 
*
* Output Via Formal Parameters: 
*
* Input Via Global Variables: m_stk[]
*
* Output Via Global Variables: m_stk[]
* 
*===============================================*/
extern void ensure_consistent_set(Int4 pro_len)
{
Int4 try_start = AINT_NONE;
Int4 this_num_bases = 0;
Int4 best_num_base=AINT_NONE;
Int4 best_start=AINT_NONE;
Int4 try_this_one_next=AINT_NONE;
Int4 collision_found=0;
Int4 last_ok_interval = 0;
Int4 test_this_interval;
Int4 index_rm;
Int4 killed_any;


#ifdef DEBUG_FIND_SET
dump_m_stk(stdout,"DEBUG_FIND_SET before insure ");
fflush(stdout);
#endif

/*-------------
 * Goes through and tries to find consistent set.
 * had a case where interval 1 was bad and 0,2,3 were
 * the right set to use, but until do {}while added 
 * (and check for smallest in bad set to kill),
 * 0 and 1 looked like a set and 3 and 4 looked like a set.
 * now, only kills one at a time, and picks smallest and retries.
 *-----------------*/

if (m_top) {					/* if any intervals at all */
do {
	 try_start = AINT_NONE;
	 best_num_base=AINT_NONE;
	 best_start=AINT_NONE;
	 try_this_one_next=AINT_NONE;
	 collision_found=0;
killed_any = AINT_NONE;
try_this_one_next = 0; 	/* start looking for consistent 
									set with first interval */

	/* while we have found a collision, look to see if
    starting with interferring interval leads to
    a larger set of bases */

	while (try_this_one_next != AINT_NONE){
		try_start = try_this_one_next;

#ifdef DEBUG_FIND_SET
printf("DEBUG_FIND_SET trying set starting at %d\n", try_start);
fflush(stdout);
#endif

		this_num_bases = 3 * (m_stk[try_start].length) + 
			m_stk[try_start].left_add_bases;
		try_this_one_next = AINT_NONE;
		for (last_ok_interval = try_start,
				test_this_interval = try_start +1;
				test_this_interval < m_top ; test_this_interval ++ ){
			if (test_for_collision(test_this_interval,
					 try_start, last_ok_interval)){
				collision_found ++ ;
#ifdef DEBUG_FIND_SET 
printf("DEBUG_FIND_SET collision found with %d, now %d collisions\n",
test_this_interval, collision_found);
fflush(stdout);
#endif

				if (try_this_one_next == AINT_NONE){
					try_this_one_next = test_this_interval;
#ifdef DEBUG_FIND_SET 
printf("DEBUG_FIND_SET set next one to try as %d\n",
try_this_one_next);
fflush(stdout);
#endif
				}
			}else{
				this_num_bases += 3 * (m_stk[test_this_interval].length)+
					m_stk[test_this_interval].left_add_bases +
					m_stk[last_ok_interval].right_add_bases;
				last_ok_interval = test_this_interval;
#ifdef DEBUG_FIND_SET
printf("DEBUG_FIND_SET bases now %d, %d was ok\n",
this_num_bases,last_ok_interval);
fflush(stdout);
#endif
			}
				
	} /* end for(;;) on higher numbered exons */

	if (this_num_bases > best_num_base){
		best_num_base = this_num_bases;
		best_start = try_start;
	}

} /* end while() on scanning attempt */

#ifdef DEBUG_FIND_SET
printf("DEBUG_FIND_SET %d has best bases %d pro_len %d\n",
best_start, best_num_base, pro_len);
fflush(stdout);
#endif

if ( collision_found &&  
		(double) best_num_base / ((double) pro_len*3) > 0.30){
/*-----change the set to the consistent best ---*/

#ifdef DEBUG_FIND_SET
printf("DEBUG_FIND_SET best using from %d\n",best_start);
fflush(stdout);
#endif
		for (test_this_interval = 0;
				test_this_interval < best_start ; test_this_interval ++ ){
			if (killed_any == AINT_NONE){
				killed_any = test_this_interval;
				this_num_bases = 3 * (m_stk[test_this_interval].length)+
					m_stk[test_this_interval].left_add_bases +
					m_stk[last_ok_interval].right_add_bases;
			}else{
				if (this_num_bases > 
						3 * (m_stk[test_this_interval].length)+
						m_stk[test_this_interval].left_add_bases +
						m_stk[last_ok_interval].right_add_bases){
				killed_any = test_this_interval;
				this_num_bases = 3 * (m_stk[test_this_interval].length)+
					m_stk[test_this_interval].left_add_bases +
					m_stk[last_ok_interval].right_add_bases;
				}
			}
#ifdef DEBUG_FIND_SET
printf("DEBUG_FIND_SET at front, would kill %d now %d bases\n",
killed_any,this_num_bases);
#endif
		}
		for ( test_this_interval = 1;
				test_this_interval < m_top ; test_this_interval ++ ){
			if (test_for_collision(test_this_interval,
					 best_start, test_this_interval-1)){
			if (killed_any == AINT_NONE){
				killed_any = test_this_interval;
				this_num_bases = 3 * (m_stk[test_this_interval].length)+
					m_stk[test_this_interval].left_add_bases +
					m_stk[last_ok_interval].right_add_bases;
			}else{
				if (this_num_bases > 
						3 * (m_stk[test_this_interval].length)+
						m_stk[test_this_interval].left_add_bases +
						m_stk[last_ok_interval].right_add_bases){
				killed_any = test_this_interval;
				this_num_bases = 3 * (m_stk[test_this_interval].length)+
					m_stk[test_this_interval].left_add_bases +
					m_stk[last_ok_interval].right_add_bases;
				}
			}
#ifdef DEBUG_FIND_SET
printf("DEBUG_FIND_SET in middle, would kill %d now %d based\n",
killed_any,this_num_bases);
#endif
			}else{
				last_ok_interval = test_this_interval;
			}
		}
			m_top -- ;
			test_this_interval = killed_any;
#ifdef DEBUG_FIND_SET
printf("DEBUG_FIND_SET killing %d now %d:\n",
test_this_interval, m_top);
fprintf_m_stk_entry(stdout,test_this_interval, "killing");
fflush(stdout);
#endif
			for (index_rm = test_this_interval;
					index_rm < m_top ; index_rm ++ ){
				copy_m_stk_entry (index_rm, index_rm+1);
			}
}else{
	break;
}
}while (killed_any == AINT_NONE);

}

#ifdef DEBUG_FIND_SET
dump_m_stk(stdout,"DEBUG_FIND_SET after insure");
fflush(stdout);
#endif

}
