static char const rcsid[] = "$Id: gapxdrop.c,v 6.76 2003/11/07 21:52:52 dondosha Exp $";

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

File name: gapxdrop.c

Author: Gennadiy Savchuk, Jinqhui Zhang, Tom Madden
[From code originally supplied by Webb Miller's lab].

Contents: Functions to perform a gapped alignment on two sequences.

****************************************************************************/
/* $Revision: 6.76 $
* $Log: gapxdrop.c,v $
* Revision 6.76  2003/11/07 21:52:52  dondosha
* Fixed SEMI_G_ALIGN_EX so it produces correct scores, by making code exactly correspond to ALIGN_EX
*
* Revision 6.75  2003/08/20 22:11:49  dondosha
* Removed some garbage code
*
* Revision 6.74  2003/06/23 21:09:23  dondosha
* Allow left extension from offset 0, since the starting point must be part of the left extension
*
* Revision 6.73  2003/05/30 17:25:36  coulouri
* add rcsid
*
* Revision 6.72  2003/05/27 20:29:57  dondosha
* Fix for OOF alignments starting at offsets 1 or 2 in query
*
* Revision 6.71  2003/05/15 21:50:08  dondosha
* The break in previous change was put in a wrong place; fixed
*
* Revision 6.70  2003/05/06 18:56:12  dondosha
* In ALIGN_packed_nucl: break out of loop if matrix score is MININT, to avoid gapping through sentinel bytes
*
* Revision 6.69  2003/04/22 17:50:07  dondosha
* Correction in OOFGapXEditBlockToSeqAlign for case when gap is followed by a frame shift
*
* Revision 6.68  2002/08/22 12:26:42  madden
* Removed unused variables
*
* Revision 6.67  2002/07/24 19:14:42  dondosha
* Previous fix was incomplete; correcting
*
* Revision 6.66  2002/07/02 17:08:48  dondosha
* If alignment longer than traceback, set traceback remainder to 0
*
* Revision 6.65  2002/06/11 14:44:48  dondosha
* Return status from some functions instead of search block pointer
*
* Revision 6.64  2002/05/09 17:01:23  dondosha
* Renamed typedefs dp_ptr and dp_node to GapXDPPtr and GapXDP
*
* Revision 6.63  2002/05/08 22:48:26  dondosha
* Allocate memory for dynamic programming upfront in Mega BLAST case
*
* Revision 6.62  2002/03/05 17:53:41  dondosha
* Added one sanity check in GapXEditBlockToSeqAlign
*
* Revision 6.61  2001/08/27 18:56:26  madden
* Fix OOFGapXEditBlockToSeqAlign if alignment overruns end of query
*
* Revision 6.60  2001/07/23 13:47:17  dondosha
* Added LIBCALL to GapXEditScriptDelete declaration
*
* Revision 6.59  2001/07/19 22:13:31  dondosha
* Made GapXEditScriptDelete public for use in mblast.c
*
* Revision 6.58  2001/04/25 13:25:47  madden
* Do not memset tback
*
* Revision 6.57  2000/12/29 20:45:38  madden
* Fix for ALIGN_packed_nucl
*
* Revision 6.56  2000/11/30 21:37:39  madden
* Roll back before GapXDropSetAlignMask changes
*
* Revision 6.55  2000/11/29 21:01:39  madden
* Use shorter length for dp_ptr rather than value from X
*
* Revision 6.54  2000/11/15 15:16:41  madden
* Changes to recycle dynamic programming structure
*
* Revision 6.53  2000/11/09 15:15:52  shavirin
* Removed #include <txalign.h>
*
* Revision 6.52  2000/10/13 21:23:48  shavirin
* Fixed OOF problem with truncated alignments.
*
* Revision 6.51  2000/10/04 20:43:36  shavirin
* Fixed case with many multiple frame shifts - protein end corrected
* in SeqAlign.
*
* Revision 6.50  2000/10/04 18:06:40  dondosha
* Subtract decline_penalty from initial values of dyn_prog[i].FF
*
* Revision 6.49  2000/09/07 16:24:27  shavirin
* Fixed minor memory leak.
*
* Revision 6.48  2000/08/30 14:15:16  shavirin
* Empty log message.
*
* Revision 6.47  2000/08/30 13:56:58  shavirin
* Removed (ifdef-ed out) OOF debug printout.
*
* Revision 6.46  2000/08/29 22:09:04  shavirin
* Next update to OOF API. Fixed few cases.
*
* Revision 6.45  2000/08/25 19:00:45  shavirin
* Added adjustment to the order of unaligned regions and gaps to ensure,
* that unaligned regions will be always from the left from gap if adjacent
*
* Revision 6.44  2000/08/08 21:44:25  shavirin
* Enabled possibility to create discontinuous alignment.
*
* Revision 6.43  2000/07/26 21:02:26  shavirin
* Removed debug printing of OOF alignment.
*
* Revision 6.42  2000/07/26 20:33:13  kans
* included txalign.h
*
* Revision 6.41  2000/07/25 16:48:07  shavirin
* Changed function to create ASN.1 for results of OOF search.
*
* Revision 6.40  2000/07/18 22:36:01  shavirin
* Fixed ABR, ABW errors or purify in PerformGapped..() functions.
*
* Revision 6.39  2000/07/17 15:24:21  shavirin
* Added parameter to function OOFGapXEditBlockToSeqAlign()
*
* Revision 6.37  2000/07/13 15:10:55  shavirin
* Changed SEMI_G_ALIGN to OOF_SEMI_G_ALIGN for is_ooframe == TRUE.
*
* Revision 6.36  2000/07/12 23:07:31  kans
* reverse_seq moved from pseed3.c to blastool.c, placed in blast.h header, called by gapxdrop.c
*
* Revision 6.35  2000/07/12 14:48:14  kans
* added prototype for reverse_seq, copied from pseed3.c - unable to easily include seed.h due to cascading dependency errors
*
* Revision 6.34  2000/07/11 20:56:08  shavirin
* Fixed minor typo in GapAlignBlkDelete().
*
* Revision 6.33  2000/07/11 20:49:07  shavirin
* Added all major functions for Out-Of-Frame alignment.
*
* Revision 6.32  2000/06/16 20:36:55  madden
* Remove MemSet from setup of state structure
*
* Revision 6.31  2000/05/16 19:59:39  madden
* Fix for rpsblast extensions
*
* Revision 6.30  2000/05/02 17:17:13  madden
* Changes to prevent gapping between sequences in rps-blast
*
* Revision 6.29  2000/03/29 21:54:28  dondosha
* Made GapXEditScriptNew public for use in MegaBlastFillHspGapInfo
*
* Revision 6.28  2000/02/16 21:45:42  shavirin
* Fixed memory leaks in GXEMakeSeqAlign() function.
*
* Revision 6.27  2000/02/11 21:12:43  shavirin
* Fixed creating reversed seqalign (gaps).
*
* Revision 6.26  2000/02/10 16:41:02  shavirin
* Fixed creation of seqalign in case of reversed tblastn case.
*
* Revision 6.25  1999/12/17 20:47:04  egorov
* Fix 'gcc -Wall' warnings
*
* Revision 6.24  1999/11/26 22:07:48  madden
* Added PerformNtGappedAlignment and ALIGN_packed_nucl
*
* Revision 6.23  1999/10/29 14:50:48  shavirin
* Workaround to bypass DECLINE regions.
*
* Revision 6.22  1999/10/27 16:40:34  shavirin
* Produced more correct discontinuous SeqAlign if DECLINE regions exists.
*
* Revision 6.21  1999/10/04 18:28:45  madden
* Add ALIGN_EX and SEMI_ALIGN_EX that do not require reversing sequences
*
* Revision 6.20  1999/09/23 15:28:23  shavirin
* Temporary disabled ability to create discontinuous alignment.
*
* Revision 6.19  1999/09/21 15:52:53  shavirin
* Added possibility to create discontinuous alignment in case when at
* leaset one DECLINE_ALIGN region exists in results of BLAST search.
*
* Revision 6.18  1999/08/27 18:07:34  shavirin
* Passed parameter decline_align from top to the engine.
*
* Revision 6.17  1999/05/12 18:48:52  madden
* lift assumption that no deletion follows insertion
*
* Revision 6.16  1999/05/03 18:59:33  madden
* Removed set, but unused, variable count
*
* Revision 6.15  1999/03/17 18:39:11  madden
* Removed unneccessary memset
*
* Revision 6.14  1999/02/18 21:18:49  madden
* MINIINT is INT4_MIN/2
*
* Revision 6.13  1999/02/17 19:42:42  madden
* change INT4_MIN to -9999999 again
*
* Revision 6.11  1998/12/18 16:20:42  madden
* Removed unnecessary memsets
*
 * Revision 6.10  1998/11/19 14:03:38  madden
 * minor efficiency
 *
 * Revision 6.9  1998/11/17 13:39:03  madden
 * Made ALIGN non-static
 *
 * Revision 6.8  1998/09/24 15:26:40  egorov
 * Fix lint complaints
 *
 * Revision 6.7  1998/08/05 21:28:16  madden
 * Protection against core-dump when gap_extend is zero
 *
 * Revision 6.6  1998/05/22 19:16:52  egorov
 * Another try to fix time problem (increase chunk size to 2Mb)
 *
 * Revision 6.5  1998/05/21 12:49:26  egorov
 * Remove memory allocation optimization to reduce time
 *
 * Revision 6.4  1998/04/21 18:39:25  madden
 * Zhengs suggestion to save memory on long sequences
 *
 * Revision 6.3  1998/04/17 19:41:17  madden
 * Zhengs changes for decline to align
 *
 * Revision 6.2  1998/01/06 18:26:48  madden
 * Gaps have same strand as surrounding sequence
 *
 * Revision 6.1  1997/09/22 17:36:29  madden
 * MACROS for position-specific matrices from Andy Neuwald
 *
 * Revision 6.0  1997/08/25 18:53:14  madden
 * Revision changed to 6.0
 *
 * Revision 1.32  1997/06/24 21:03:38  madden
 * TracebackToGapXEditBlock() check for last GapXEditScript fixed
 *
 * Revision 1.31  1997/06/24 19:12:12  tatiana
 * TracebackToGapXEditBlock() check for last GapXEditScript added
 *
 * Revision 1.30  1997/06/24 18:14:33  tatiana
 * TracebackToGapXEditBlock() fixed
 *
 * Revision 1.29  1997/05/13 20:45:50  madden
 * fixed offset problem in PerformGappedAlignment
 *
 * Revision 1.28  1997/04/21 15:35:26  madden
 * Fixes for 'gapped' StdSegs.
 *
 * Revision 1.27  1997/04/16  16:21:38  madden
 * Set seqalign_type.
 *
 * Revision 1.26  1997/04/15  22:01:53  madden
 * Added original_length[12] for translating searches.
 *
 * Revision 1.25  1997/04/14  14:10:37  madden
 * Fix for the case of dropoff less than the opening penalty.
 *
 * Revision 1.24  1997/04/11  19:02:49  madden
 * Changes for in-frame blastx, tblastn gapping.
 *
 * Revision 1.23  1997/03/14  21:01:59  madden
 * Changed to use less memory in ALIGN, with GapXDropStateArrayStructPtr.
 *
 * Revision 1.22  1997/03/11  19:24:08  madden
 * Check return value of MemNew for state_array in ALIGN.
 *
 * Revision 1.21  1997/03/01  18:25:33  madden
 * Sequences reversed on SeqAlign if reverse flag set on EditBlockPtr.
 *
 * Revision 1.20  1997/02/24  16:40:38  madden
 * Change to GapXEditBlockToSeqAlign to use first SeqIdPtr, duplicate.
 *
 * Revision 1.19  1997/02/24  15:09:38  madden
 * Replaced MemSet by setting some elements of an array to zero,.
 *
 * Revision 1.18  1997/02/23  16:44:47  madden
 * Memory, saved on GapAlignBlkPtr, reuses.
 *
 * Revision 1.17  1997/02/20  22:58:49  madden
 * Support for gapped translated results.
 *
 * Revision 1.16  1997/02/20  21:50:24  madden
 * Added frame and translation information to GapAlignBlk, assigned it.
 *
 * Revision 1.15  1997/02/12  22:19:08  madden
 * Changes for position-based comparisons.
 *
 * Revision 1.14  1997/02/10  15:25:33  madden
 * Changed args. of ALIGN and SEMI_G_ALIGN to pass in gap_align block,
 * also whether the query was reversed and the query_offset.
 * Also General cleanup.
 *
 * Revision 1.13  1997/02/04  16:22:32  madden
 * Changes to enable gapped alignments on the reverse strand.
 *
 * Revision 1.12  1997/01/22  17:45:08  madden
 * Added positionBased Boolean to ALIGN and SEMI_G_ALIGN.
 *
 * Revision 1.11  1997/01/21  18:55:17  madden
 * Fixed bug with translation of EditScript to SeqAlign.
 *
 * Revision 1.10  1997/01/17  14:29:34  madden
 * Protection against NULL args added in GapXEditBlockDelete.
 *
 * Revision 1.9  1997/01/16  20:20:49  madden
 * TracebackToGapXEditBlock made non-static.
 *
 * Revision 1.8  1997/01/06  22:40:55  madden
 * Added function SimpleIntervalToGapXEditBlock.
 *
 * Revision 1.7  1997/01/06  17:22:59  madden
 * Used GapXEditScriptToSeqAlign to find SeqAlign.
 *
 * Revision 1.6  1996/12/30  21:45:28  madden
 * UMR fix.
 *
 * Revision 1.5  1996/12/30  17:14:06  madden
 * Fixes for changes for "require a portion of the query sequence".
 *
 * Revision 1.4  1996/12/30  15:44:25  madden
 * Added capability to require a portion of the query sequence.
 *
 * Revision 1.3  1996/12/16  15:29:12  madden
 * Changed gapalign.h to gapxdrop.h
 *
 * Revision 1.2  1996/12/12  16:45:03  madden
 * GapAlignBlkPtr used instead of arguments in functions.
 *
 * Revision 1.1  1996/12/12  14:02:51  madden
 * Initial revision
 *
*/


#include <gapxdrop.h>
#include <blast.h>


/* A PACKAGE FOR LOCALLY ALIGNING TWO SEQUENCES WITHIN A BAND:

   To invoke, call SEMI_G_ALIGN(A,B,M,N,W,G,H,S,X,&EI,&EJ, score_only, positionBased).
   The parameters are explained as follows:
	A, B : two sequences to be aligned
	M : the length of sequence A
	N : the length of sequence B
	W : scoring table for matches and mismatches
	G : gap-opening penalty
	H : gap-extension penalty
	X : maximum drop off
	S : script for DISPLAY routine
	*EI : ending position of sequence A in the optimal local alignment
	*EJ : ending position of sequence B in the optimal local alignment
	score_only: indicate if ony score is needed. 1--score only 0--alignment
	            is also needed.

	Use best_score to cut unfeasible points. quadratic space and row-wise

	modification (cfj):
	- These routines can be told to changes their access to A and B, so that they are passed pointers to the 
	  forward sequence, but act as if they had been past pointers to reversed sequences.
	  (This is done just as the accesses to posMatrix are flipped if reverse is set.)
	  This obviates the need to actually reverse A and B, which in some runs was almost 20% of 
	  the total runtime.
	  This option is invoked by setting reverse_sequence. 
*/


/* Append "Delete k" op */
#define DEL_(k) \
data.last = (data.last < 0) ? (data.sapp[-1] -= (k)) : (*data.sapp++ = -(k));

/* Append "Insert k" op */
#define INS_(k) \
data.last = (data.last > 0) ? (data.sapp[-1] += (k)) : (*data.sapp++ = (k));

/* Append "Replace" op */
#define REP_ \
{ data.last = *data.sapp++ = 0; }

/* Divide by two to prevent underflows. */
#define MININT INT4_MIN/2
#define REPP_ \
{ *data.sapp++ = MININT; data.last = 0; }


typedef struct {
  GapXDPPtr CD;
  Int4Ptr PNTR v;	
  Int4Ptr sapp;			/* Current script append ptr */
  Int4  last;
  Int4 h,  g;
} data_t;

/* #define	CHUNKSIZE	1048576 */
#define	CHUNKSIZE	2097152

static GapXDropStateArrayStructPtr
GapXDropGetState(GapXDropStateArrayStructPtr PNTR head, Int4 length)

{
	GapXDropStateArrayStructPtr	retval, var, last;
	Int4				chunksize = MAX(CHUNKSIZE, length + length/3);

	length += length/3;	/* Add on about 30% so the end will get reused. */
	retval = NULL;
	if (*head == NULL)
	{
		retval = (GapXDropStateArrayStructPtr) Nlm_Malloc(sizeof(GapXDropStateArrayStruct));
		retval->state_array = Nlm_Malloc(chunksize*sizeof(Uint1));
		retval->length = chunksize;
		retval->used = 0;
		retval->next = NULL;
		*head = retval;
	}
	else
	{
		var = *head;
		last = *head;
		while (var)
		{
			if (length < (var->length - var->used))
			{
				retval = var;
				break;
			}
			else if (var->used == 0)
			{ /* If it's empty and too small, replace. */
				var->state_array = MemFree(var->state_array);
				var->state_array = Nlm_Malloc(chunksize*sizeof(Uint1));
				var->length = chunksize;
				retval = var;
				break;
			}
			last = var;
			var = var->next;
		}

		if (var == NULL)
		{
			retval = (GapXDropStateArrayStructPtr) Nlm_Malloc(sizeof(GapXDropStateArrayStruct));
			retval->state_array = Nlm_Malloc(chunksize*sizeof(Uint1));
			retval->length = chunksize;
			retval->used = 0;
			retval->next = NULL;
			last->next = retval;
		}
	}

	if (retval->state_array == NULL)
		ErrPostEx(SEV_ERROR, 0, 0, "state array is NULL");
		
	return retval;

}

static Boolean
GapXDropPurgeState(GapXDropStateArrayStructPtr state_struct)

{
	while (state_struct)
	{
/*
		MemSet(state_struct->state_array, 0, state_struct->used);
*/
		state_struct->used = 0;
		state_struct = state_struct->next;
	}

	return TRUE;
}

GapXDropStateArrayStructPtr 
GapXDropStateDestroy(GapXDropStateArrayStructPtr state_struct)

{
	GapXDropStateArrayStructPtr next;

	while (state_struct)
	{
		next = state_struct->next;
		MemFree(state_struct->state_array);
		MemFree(state_struct);
		state_struct = next;
		
	}

	return NULL;
}

/*
        Aligns two nucleotide sequences, one (A) should be packed in the
        same way as the BLAST databases, the other (B) should contain one
        basepair/byte.
        Boolean reverse_sequence: do reverse the sequence.
*/

static Int4 ALIGN_packed_nucl(Uint1Ptr B, Uint1Ptr A, Int4 N, Int4 M, 
		Int4Ptr pej, Int4Ptr pei, GapAlignBlkPtr gap_align,
		Int4 query_offset, Boolean reverse_sequence)
{ 
  GapXDPPtr dyn_prog;
  Int4 i, j, cb, j_r, g, decline_penalty;
  register Int4 c, d, e, m, tt, h, X, f;
  Int4 best_score = 0, new_score;
  Int4Ptr *matrix;
  register Int4Ptr wa;
  register GapXDPPtr dp;
  Uint1Ptr Bptr;
  Uint1 base_pair;
  Int4 B_increment=1;
  
  matrix = gap_align->matrix;
  *pei = *pej = 0;
  m = (g=gap_align->gap_open) + gap_align->gap_extend;
  h = gap_align->gap_extend;
  decline_penalty = gap_align->decline_align;
  X = gap_align->x_parameter;

  if (X < m)
	X = m;

  if(N <= 0 || M <= 0) return 0;

  j = (N + 2) * sizeof(GapXDP);
  if (gap_align->dyn_prog)
     dyn_prog = gap_align->dyn_prog;
  else {
#if BLAST_ALTIVEC
	  if (j > gap_align->dyn_prog_save_SIZE) {
		  if (gap_align->dyn_prog_save_PTR) MemFree(gap_align->dyn_prog_save_PTR);
		  
		  gap_align->dyn_prog_save_PTR = Nlm_Malloc(j);
		  
		  gap_align->dyn_prog_save_SIZE = j;
		  
	  }
	  
	  dyn_prog = 	gap_align->dyn_prog_save_PTR;
#else
	  dyn_prog = (GapXDPPtr)Nlm_Malloc(j);
	  if (!dyn_prog) {
		  ErrPostEx(SEV_ERROR, 0, 0, 
					"Cannot allocate %ld bytes for dynamic programming", j);
		  return -1;
	  }
#endif  
  }

  dyn_prog[0].CC = 0; c = dyn_prog[0].DD = -m;
  dyn_prog[0].FF = -m;
  for(i = 1; i <= N; i++) {
    if(c < -X) break;
    dyn_prog[i].CC = c;
    dyn_prog[i].DD = c - m; 
    dyn_prog[i].FF = c - m - decline_penalty;
    c -= h;
  }

  if(reverse_sequence)
    B_increment=-1;
  else
    B_increment=1;

  tt = 0;  j = i;
  for (j_r = 1; j_r <= M; j_r++) {
	if(reverse_sequence)
	{
		base_pair = READDB_UNPACK_BASE_N(A[(M-j_r)/4], ((j_r-1)%4));
            	wa = matrix[base_pair];
	}
        else
	{
		base_pair = READDB_UNPACK_BASE_N(A[1+((j_r-1)/4)], (3-((j_r-1)%4)));
            	wa = matrix[base_pair];
	}
      e = c =f = MININT;
      Bptr = &B[tt];
      if(reverse_sequence)
	Bptr = &B[N-tt];
/*
      Bptr += B_increment;
*/
      for (cb = i = tt, dp = &dyn_prog[i]; i < j; i++) {
	 Bptr += B_increment;
         new_score = wa[*Bptr];

	  d = dp->DD;
	  if (e < f) e = f;
	  if (d < f) d = f;
	  if (c < d || c < e) {
	      if (d < e) {
		  c = e;
	      } else {
		  c = d; 
	      }
	      if (best_score - c > X) {
		  c = dp->CC+new_score; f = dp->FF;
		  if (tt == i) tt++;
		  else { dp->CC =dp->FF= MININT;}
	      } else {
		  cb = i;
                  if ((c-=m) > (d-=h)) {
                      dp->DD = c;
                  } else {
                      dp->DD = d;
                  }
                  if (c > (e-=h)) {
                      e = c;
                  }
                  c+=m;
		  d = dp->CC+new_score; dp->CC = c; c=d;
		  d = dp->FF; dp->FF = f-decline_penalty; f = d;
	      }
	  } else {
	      if (best_score - c > X){
		  c = dp->CC+new_score; f= dp->FF;
		  if (tt == i) tt++;
		  else { dp->CC =dp->FF= MININT;}
	      } else {
		  cb = i; 
		  if (c > best_score) {
		      best_score = c;
		      *pei = j_r; *pej = i;
		  } 
		  if ((c-=m) > (d-=h)) {
		      dp->DD = c; 
		  } else {
		      dp->DD = d;
		  } 
		  if (c > (e-=h)) {
		      e = c;
		  } 
		  c+=m;
		  d = dp->FF;
		  if (c-g>f) dp->FF = c-g-decline_penalty; else dp->FF = f-decline_penalty;
		  f = d;
		  d = dp->CC+new_score; dp->CC = c; c = d;
	      }
	  }
          if (new_score == MININT)
             break;
	  dp++;
      }
      if (tt == j) break;
      if (cb < j-1) { j = cb+1;}
      else while (e >= best_score-X && j <= N) {
	  dyn_prog[j].CC = e; dyn_prog[j].DD = e-m;dyn_prog[j].FF = e-g-decline_penalty;
	  e -= h; j++;
      }
      if (j <= N) {
	  dyn_prog[j].DD = dyn_prog[j].CC = dyn_prog[j].FF = MININT; j++;
      }
  }
  
  if (!gap_align->dyn_prog) {
#if BLAST_ALTIVEC
	  // do nothing, will be dealloced later
#else	  
	  MemFree(dyn_prog);
#endif
  }

  return best_score;
}
/*
	Boolean revered: has the sequence been reversed? Used for psi-blast
	Boolean reverse_sequence: do reverse the sequence.
	Two Booleans are used to emulate behavior before the sequence could be reversed.
*/


static Int4
ALIGN_EX(Uint1Ptr A, Uint1Ptr B, Int4 M, Int4 N, Int4Ptr S, Int4Ptr pei, 
	Int4Ptr pej, Int4Ptr PNTR sapp, GapAlignBlkPtr gap_align, 
	Int4 query_offset, Boolean reversed, Boolean reverse_sequence)
	
{ 
  data_t data;
  Int4 i, j, cb,  j_r, s, k;
  Uint1 st, std, ste;
  Int4 gap_open, gap_extend, decline_penalty;
  register Int4 c, d, e, m,t, tt, f, tt_start;
  Int4 best_score = 0;
  register Int4Ptr wa;
  register GapXDPPtr dp, dyn_prog;
  Uint1Ptr PNTR state, stp, tmp;
  Uint1Ptr state_array;
  Int4Ptr *matrix;
  register Int4 X;
  GapXDropStateArrayStructPtr state_struct;
  Int4 next_c;
  Uint1Ptr Bptr;
  Int4 B_increment=1;
  Int4 align_len;
  
  matrix = gap_align->matrix;
  *pei = *pej = 0;
  data.sapp = *sapp = S;
  data.last= 0;
  m = gap_align->gap_open + gap_align->gap_extend;
  decline_penalty = gap_align->decline_align;

  gap_open = gap_align->gap_open;
  gap_extend = gap_align->gap_extend;
  X = gap_align->x_parameter;

  if (X < m)
	X = m;

  if(N <= 0 || M <= 0) { 
    *pei = *pej;
    return 0;
  }

  GapXDropPurgeState(gap_align->state_struct);

  j = (N + 2) * sizeof(GapXDP);
  if (gap_align->dyn_prog)
     dyn_prog = gap_align->dyn_prog;
	else {
#if BLAST_ALTIVEC
		if (j > gap_align->dyn_prog_save_SIZE) {
			if (gap_align->dyn_prog_save_PTR) MemFree(gap_align->dyn_prog_save_PTR);
			
			gap_align->dyn_prog_save_PTR = Nlm_Malloc(j);
			
			gap_align->dyn_prog_save_SIZE = j;
			
		}
		
		dyn_prog = 	gap_align->dyn_prog_save_PTR;
#else
		dyn_prog = (GapXDPPtr)Nlm_Malloc(j);
#endif
		
		
		
	}
  state = Nlm_Malloc(sizeof(Uint1Ptr)*(M+1));
  dyn_prog[0].CC = 0;
  dyn_prog[0].FF = -m - decline_penalty;
  c = dyn_prog[0].DD = -m;

  /* Protection against divide by zero. */
  if (gap_extend > 0)
  	state_struct = GapXDropGetState(&gap_align->state_struct, X/gap_extend+5);
  else
	state_struct = GapXDropGetState(&gap_align->state_struct, N+3);

  state_array = state_struct->state_array;
  state[0] = state_array;
  stp  = state[0];
  for(i = 1; i <= N; i++) {
    if(c < -X) break;
    dyn_prog[i].CC = c;
    dyn_prog[i].DD = c - m; 
    dyn_prog[i].FF = c - m - decline_penalty;
    c -= gap_extend;
    stp[i] = 1;
  }
  state_struct->used = i+1;
  
  if(reverse_sequence)
    B_increment=-1;
  else
    B_increment=1;
 
  tt = 0;  j = i;
  for(j_r = 1; j_r <= M; j_r++) {
     /* Protection against divide by zero. */
    if (gap_extend > 0)
    	state_struct = GapXDropGetState(&gap_align->state_struct, j-tt+5+X/gap_extend);
    else
	state_struct = GapXDropGetState(&gap_align->state_struct, N-tt+3);
    state_array = state_struct->state_array + state_struct->used + 1;
    state[j_r] = state_array - tt + 1;
    stp = state[j_r];
    tt_start = tt; 
    if (!(gap_align->positionBased)){ /*AAS*/
      if(reverse_sequence)
        wa = matrix[A[M-j_r]];
      else
        wa = matrix[A[j_r]];
      }
    else {
      if(reversed || reverse_sequence)
        wa = gap_align->posMatrix[M - j_r];
      else
        wa = gap_align->posMatrix[j_r + query_offset];
    }
    e = c = f= MININT;
    Bptr = &B[tt];
    if(reverse_sequence)
      Bptr = &B[N-tt];

    for (cb = i = tt, dp = &dyn_prog[i]; i < j; i++) {
        Int4 next_f;
        d = (dp)->DD;
        Bptr += B_increment;
        next_c = dp->CC+wa[*Bptr];   /* Bptr is & B[i+1]; */
        next_f = dp->FF;
	st = 0;
	if (c < f) {c = f; st = 3;}
	if (f > d) {d = f; std = 60;} 
	else {
	  std = 30;
	  if (c < d) { c= d;st = 2;}
	}
	if (f > e) {e = f; ste = 20;} 
	else {
	  ste = 10;
	  if (c < e) {c=e; st=1;}
	}
	if (best_score - c > X){
	  if (tt == i) tt++;
	  else { dp->CC = MININT; }
	} else {
	  cb = i;
	  if (c > best_score) {
	    best_score = c;
	    *pei = j_r; *pej = i;
	  }
	  if ((c-=m) > (d-=gap_extend)) {
	    dp->DD = c; 
	  } else {
	    dp->DD = d;
	    st+=std;
	  } 
	  if (c > (e-=gap_extend)) {
	    e = c; 
	  }  else {
	    st+=ste;
	  }
	  c+=m; 
	  if (f < c-gap_open) { 
	    dp->FF = c-gap_open-decline_penalty; 
	  } else {
	    dp->FF = f-decline_penalty; st+= 5; 
	  }
	  dp->CC = c;
	}
	stp[i] = st;
	c = next_c;
	f = next_f;
	dp++;
    }
    if (tt == j) { j_r++; break;}
    if (cb < j-1) { j = cb+1;}
    else {
	while (e >= best_score-X && j <= N) {
	    dyn_prog[j].CC = e; dyn_prog[j].DD = e-m; dyn_prog[j].FF = e-gap_open-decline_penalty;
	    e -= gap_extend; stp[j] = 1;
	    j++;
	}
    }
    if (j <= N) {
	dyn_prog[j].DD = dyn_prog[j].CC= dyn_prog[j].FF = MININT;
	j++; 
    }
    state_struct->used += (MAX(i, j) - tt_start + 1);
  }
  i = *pei; j = *pej;
  tmp = Nlm_Malloc(i+j);
  for (s=0, c = 0; i> 0 || j > 0; c++) {
      t = state[i][j];
      k  = t %5;
      if (s == 1) 
	  if ((t/10)%3 == 1) k = 1;
	  else if ((t/10)%3 == 2) k = 3;
      if (s == 2)
	  if ((t/30) == 1) k = 2;
	  else if ((t/30) == 2) k = 3;
      if (s == 3 && ((t/5)%2) == 1) k = 3;
      if (k == 1) { j--;}
      else if (k == 2) {i--;}
      else {j--; i--;}
      tmp[c] = s = k;
  }

  align_len = c;
  c--;
  while (c >= 0) {
      if (tmp[c] == 0) REP_
      else if (tmp[c] == 1) INS_(1)
      else if (tmp[c] == 3) REPP_			  
      else DEL_(1)     
      c--;
  }

  MemFree(tmp);

  MemFree(state);

 if (!gap_align->dyn_prog) {
#if BLAST_ALTIVEC
	// do nothing, will be dealloced later
#else
	MemFree(dyn_prog);
#endif
}

  if ((align_len -= data.sapp - S) > 0)
     MemSet(data.sapp, 0, align_len);
  *sapp = data.sapp;

  return best_score;
}

Int4 LIBCALL 
ALIGN(Uint1Ptr A, Uint1Ptr B, Int4 M, Int4 N,
		Int4Ptr S, Int4Ptr pei, Int4Ptr pej, Int4Ptr PNTR sapp, 
		GapAlignBlkPtr gap_align, Int4 query_offset, Boolean reversed)
	
{ 
	return ALIGN_EX(A, B, M, N, S, pei, pej, sapp, gap_align, query_offset, reversed, FALSE);
}

/*
	Boolean revered: has the sequence been reversed? Used for psi-blast
	Boolean reverse_sequence: do reverse the sequence.
	Two Booleans are used to emulate behavior before the sequence could be reversed.
*/
static Int4 SEMI_G_ALIGN_EX(Uint1Ptr A, Uint1Ptr B, Int4 M, Int4 N,
		Int4Ptr S, Int4Ptr pei, Int4Ptr pej, 
		Boolean score_only, Int4Ptr PNTR sapp, GapAlignBlkPtr gap_align,
		Int4 query_offset, Boolean reversed, Boolean reverse_sequence)
		
{ 
  GapXDPPtr dyn_prog;
  Int4 i, j, cb, j_r, g, decline_penalty;
  register Int4 c, d, e, m, tt, h, X, f;
  Int4 best_score = 0;
  Int4Ptr *matrix;
  register Int4Ptr wa;
  register GapXDPPtr dp;
  Uint1Ptr Bptr;
  Int4 B_increment=1;
  Int4 next_c, next_f;
  
  if(!score_only) {
    return ALIGN_EX(A, B, M, N, S, pei, pej, sapp, gap_align, query_offset, reversed, reverse_sequence);
  }
  
  matrix = gap_align->matrix;
  *pei = *pej = 0;
  m = (g=gap_align->gap_open) + gap_align->gap_extend;
  h = gap_align->gap_extend;
  decline_penalty = gap_align->decline_align;

  X = gap_align->x_parameter;

  if (X < m)
	X = m;

  if(N <= 0 || M <= 0) return 0;

  j = (N + 2) * sizeof(GapXDP);
  dyn_prog = (GapXDPPtr)Nlm_Malloc(j);

  dyn_prog[0].CC = 0; c = dyn_prog[0].DD = -m;
  dyn_prog[0].FF = -m - decline_penalty;
  for(i = 1; i <= N; i++) {
    if(c < -X) break;
    dyn_prog[i].CC = c;
    dyn_prog[i].DD = c - m; 
    dyn_prog[i].FF = c-m - decline_penalty;
    c -= h;
  }

  if(reverse_sequence)
    B_increment=-1;
  else
    B_increment=1;

  tt = 0;  j = i;
  for (j_r = 1; j_r <= M; j_r++) {
      if (!(gap_align->positionBased)){ /*AAS*/
        if(reverse_sequence)
            wa = matrix[A[M-j_r]];
        else
            wa = matrix[A[j_r]];
      }
      else {
	  if(reversed || reverse_sequence)
	  {
	      wa = gap_align->posMatrix[M - j_r];
		if (A[M-j_r] == NULLB)  /* Prevents gapping through a NULL byte in rps-blast. */
			break;
	  }
	  else
	  {
	      wa = gap_align->posMatrix[j_r + query_offset];
		if (A[j_r] == NULLB)
			break;
	  }
      }
      e = c =f = MININT;
      Bptr = &B[tt];
      if(reverse_sequence)
         Bptr = &B[N-tt];
      for (cb = i = tt, dp = &dyn_prog[i]; i < j; i++) {
         d = dp->DD;
	 Bptr += B_increment;
         next_c = dp->CC+wa[*Bptr];   /* Bptr is & B[i+1]; */
         next_f = dp->FF;

         if (c < f) c = f;
         if (f > d) d = f;
         else if (c < d) c= d;
         
         if (f > e) e = f;
         else if (c < e) c = e;
         
         if (best_score - c > X){
            if (tt == i) tt++;
            else { dp->CC = MININT; }
         } else {
            cb = i;
            if (c > best_score) {
               best_score = c;
               *pei = j_r; *pej = i;
            }
            if ((c-=m) > (d-=h)) {
               dp->DD = c; 
            } else {
               dp->DD = d;
            } 
            if (c > (e-=h)) {
               e = c; 
            }
            c+=m; 
            if (f < c-g) { 
               dp->FF = c-g-decline_penalty; 
            } else {
               dp->FF = f-decline_penalty;
            }
            dp->CC = c;
         }
         c = next_c;
         f = next_f;
         dp++;
      }

      if (tt == j) break;
      if (cb < j-1) { j = cb+1;}
      else while (e >= best_score-X && j <= N) {
	  dyn_prog[j].CC = e; dyn_prog[j].DD = e-m;dyn_prog[j].FF = e-g-decline_penalty;
	  e -= h; j++;
      }
      if (j <= N) {
	  dyn_prog[j].DD = dyn_prog[j].CC = dyn_prog[j].FF = MININT; j++;
      }
  }
  
  MemFree(dyn_prog);

  return best_score;
}

Int4 SEMI_G_ALIGN(Uint1Ptr A, Uint1Ptr B, Int4 M, Int4 N,
		Int4Ptr S, Int4Ptr pei, Int4Ptr pej, 
		Boolean score_only, Int4Ptr PNTR sapp, GapAlignBlkPtr gap_align,
		Int4 query_offset, Boolean reversed)
		
{ 
	return SEMI_G_ALIGN_EX(A, B, M, N, S, pei, pej, score_only, sapp, gap_align, 
		query_offset, reversed, FALSE);
}

static Int4 OOF_ALIGN(Uint1Ptr A, Uint1Ptr B, Int4 M, Int4 N,
                      Int4Ptr S, Int4Ptr pei, Int4Ptr pej, Int4Ptr PNTR sapp, 
                      GapAlignBlkPtr gap_align, Int4 query_offset, 
                      Boolean reversed)
	
{ 
  data_t data;
  Int4 i, j, cb,  j_r, s, k, sc, s1, s2, s3, st1, e1, e2, e3, shift;
  register Int4 c, d, m,t, tt, tt_start, f1, f2;
  Int4 best_score = 0;
  register Int4Ptr wa;
  Int4 count = 0;
  register GapXDPPtr dp;
  Uint1Ptr PNTR state, stp, tmp;
  Uint1Ptr state_array;
  Int4Ptr *matrix;
  register Int4 X;
  GapXDropStateArrayStructPtr state_struct;
  
  matrix = gap_align->matrix;
  *pei =0; *pej = -2;
  data.sapp = *sapp = S;
  data.last= 0;
  m = gap_align->gap_open + gap_align->gap_extend;
  data.g = gap_align->gap_open;
  data.h = gap_align->gap_extend;
  data.v = matrix;
  X = gap_align->x_parameter;
  shift = gap_align->shift_pen;

  if (X < m)
	X = m;

  if(N <= 0 || M <= 0) { 
    return 0;
  }

  N+=2;
  GapXDropPurgeState(gap_align->state_struct);

  j = (N + 2) * sizeof(GapXDP);
  data.CD = (GapXDPPtr)MemNew(j);

  state = MemNew(sizeof(Uint1Ptr)*(M+1));
  data.CD[0].CC = 0;
  c = data.CD[0].DD = -m;
  state_struct = GapXDropGetState(&gap_align->state_struct, N+3);
  state_array = state_struct->state_array;
  state[0] = state_array;
  stp  = state[0];
  data.CD[0].CC = 0; c = data.CD[0].DD = -m;
  for(i = 3; i <= N; i+=3) {
    data.CD[i].CC = c;
    data.CD[i].DD = c - m; 
    data.CD[i-1].CC = data.CD[i-2].CC = data.CD[i-1].DD = 
	data.CD[i-2].DD = MININT;
    if(c < -X) break;
    c -= data.h;
    stp[i] = stp[i-1] = stp[i-2] = 6;
  }
  i -= 2;
  data.CD[i].CC = data.CD[i].DD = MININT;
  tt = 0;  j = i;
  state_struct->used = i+1;
  B-=2;
  tt = 0;  j = i;
  for(j_r = 1; j_r <= M; j_r++) {
    count += j - tt; 
    state_struct = GapXDropGetState(&gap_align->state_struct, N-tt+3);
    state_array = state_struct->state_array + state_struct->used + 1;
    state[j_r] = state_array - tt + 1;
    stp = state[j_r];
    tt_start = tt; 
    if (!(gap_align->positionBased)) /*AAS*/
      wa = matrix[A[j_r]]; 
    else {
      if(reversed)
        wa = gap_align->posMatrix[M - j_r];
      else
        wa = gap_align->posMatrix[j_r + query_offset];
    }
    c = MININT; sc =f1=f2=e1 =e2=e3=s1=s2=s3=MININT;
    for(cb = i = tt, dp = &data.CD[i-1]; 1;) {
	if (i >= j) break;
	sc = MAX(MAX(f1, f2)-shift, s3);
	if (sc == s3) st1=3;
	else if (sc+shift == f1) {
	  if (f1 == s2) st1=2; else st1 = 5;
	} else if (f2 == s1) st1 = 1; else st1 = 4;
	sc += wa[B[i++]];
	f1 = s3; 
	s3 = (++dp)->CC; f1 = MAX(f1, s3);
	d = dp->DD;
	if (sc < MAX(d, e1)) {
	    if (d > e1) { sc = d; st1 = 30;}
	    else {sc = e1; st1 = 36;}
	    if (best_score -sc > X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		cb = i;
		dp->CC = sc;
		dp->DD = d-data.h;
		e1-=data.h;
	    }
	} else {
	    if (best_score -sc > X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		cb = i;
		dp->CC = sc;
		if (sc > best_score) {best_score = sc; *pei = j_r;*pej=i-3;}
		if ((sc-=m) > (e1-=data.h)) e1 = sc; else st1+=10;
		if (sc < (d-=data.h)) { dp->DD = d; st1 += 20;} 
		else dp->DD = sc;
	    }
	}
	stp[i-1] = st1;
	if (i >= j) {c = e1; e1 = e2; e2 = e3; e3 = c; break;}
	sc = MAX(MAX(f1,f2)-shift, s2);
	if (sc == s2) st1=3;
	else if (sc+shift == f1) {
	  if (f1 == s3) st1=1; else st1 = 4;
	} else if (f2 == s1) st1 = 2; else st1 = 5;
	sc += wa[B[i++]];
	f2 = s2; s2 = (++dp)->CC; f2 = MAX(f2, s2);
	d = dp->DD;
	if (sc < MAX(d, e2)) {
	    if ((sc=MAX(d,e2)) < best_score-X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		if (sc == d)  st1= 30; else st1=36;
		cb = i;
		dp->CC = sc;
		dp->DD = d-data.h;
		e2-=data.h;
	    }
	} else {
	    if (sc < best_score-X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		cb = i;
		dp->CC = sc;
		if (sc > best_score) {best_score = sc;*pei= j_r; *pej=i-3;}
		if ((sc-=m) > (e2-=data.h)) e2 = sc; else st1+=10;
		if (sc < (d-=data.h)) {dp->DD = d; st1+=20;} 
		else  dp->DD = sc;
	    }
	}
	stp[i-1] = st1;
	if (i >= j) { c = e2; e2 = e1; e1 = e3; e3 = c; break; }
	sc = MAX(MAX(f1, f2)-shift, s1);
	if (sc == s1) st1=3;
	else if (sc+shift == f1) {
	  if (f1 == s3) st1=2; else st1 = 5;
	} else if (f2 == s2) st1 = 1; else st1 = 4;
	sc += wa[B[i++]];
	f1 = f2;
	f2 = s1; s1 = (++dp)->CC; f2 = MAX(f2, s1);
	d = dp->DD;
	if (sc < MAX(d, e3)) {
	    sc = MAX(d, e3);
	    if (sc < best_score-X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		if (sc == d) st1 = 30; else st1 = 36;
		cb = i;
		dp->CC = sc;
		dp->DD = d-data.h;
		e3-=data.h;
	    }
	} else {
	    if (sc < best_score-X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		cb = i;
		dp->CC = sc;
		if (sc > best_score) {best_score = sc;*pei = j_r; *pej=i-3;}
		if ((sc-=m) > (e3-=data.h)) e3 = sc; else st1 += 10;
		if (sc < (d-=data.h)) {dp->DD = d; st1 += 20;}
		else dp->DD = sc;
	    }
	}
	stp[i-1] = st1;
	sc = c; 
    }
    if(tt == j) break;
    if(cb < j) { j = cb;}
    else {
	c = (MAX(e1, MAX(e2, e3))+X-best_score)/data.h+j;
	if (c > N) c = N;
	if (c > j)
	while (1) {
	    data.CD[j].CC = e1;
	    stp[j] = 36;
	    data.CD[j++].DD = e1 - m; e1 -=data.h;
	    if (j > c) break;      
	    data.CD[j].CC = e2; stp[j] = 36;
	    data.CD[j++].DD = e2- m; e2-=data.h;
	    if (j > c) break;      
	    data.CD[j].CC = e3; stp[j] = 36;
	    data.CD[j++].DD = e3- m; e3-=data.h;
	    if (j > c) break;
	}
    }
    c = j+4;
    if (c > N+1) c = N+1;
    while (j < c) {
	data.CD[j].DD = data.CD[j].CC = MININT;
	j++;
    }

    state_struct->used += (MAX(i, j) - tt_start + 1);
  }
  i = *pei; j = *pej+2;
  /* printf("best = %d i,j=%d %d\n", best_score, i, j); */
  tmp = MemNew(i + j);        
  for (s= 1, c= 0; i > 0 || j > 0; c++, i--) {
      k  = (t=state[i][j])%10;
      if (s == 6 && (t/10)%2 == 1) k = 6;
      if (s == 0 && (t/20)== 1) k = 0;
      if (k == 6) { j -= 3; i++;}
      else {j -= k;}
      s = tmp[c] = k;
  }
  c--; 
  while(c >= 0) {
      *data.sapp++ = tmp[c--];
  }

  MemFree(tmp);

  MemFree(state);

  MemFree(data.CD);
  *sapp = data.sapp;

  return best_score;
}


static Int4 OOF_SEMI_G_ALIGN(Uint1Ptr A, Uint1Ptr B, Int4 M, Int4 N,
                             Int4Ptr S, Int4Ptr pei, Int4Ptr pej, 
                             Boolean score_only, Int4Ptr PNTR sapp, 
                             GapAlignBlkPtr gap_align,
                             Int4 query_offset, Boolean reversed)
{ 
  GapXDPPtr CD;
  Int4 i, j, cb, j_r;
  Int4 e1, e2, e3, s1, s2, s3, shift;
  register Int4 c, d, sc, m, tt, h, X, f1, f2;
  Int4 best_score = 0, count = 0;
  Int4Ptr *matrix;
  register Int4Ptr wa;
  register GapXDPPtr dp;
  
  if(!score_only)
      return OOF_ALIGN(A, B, M, N, S, pei, pej, sapp, gap_align, query_offset, reversed);
  
  /*printf("reversed=%d\n", reversed);*/
  matrix = gap_align->matrix;
  *pei = 0; *pej = -2;
  m = gap_align->gap_open + gap_align->gap_extend;
  h = gap_align->gap_extend;
  X = gap_align->x_parameter;
  shift = gap_align->shift_pen;
  /*printf("m=%d h=%d X=%d shift=%d %d\n", m,h, X, shift, B[2]);*/

  B-=2;
  if (X < m)
	X = m;

  if(N <= 0 || M <= 0) return 0;
  N+=2;

  j = (N + 5) * sizeof(GapXDP);
  CD = (GapXDPPtr)MemNew(j);
  CD[0].CC = 0; c = CD[0].DD = -m;
  for(i = 3; i <= N; i+=3) {
    CD[i].CC = c;
    CD[i].DD = c - m; 
    CD[i-1].CC = CD[i-2].CC = CD[i-1].DD = CD[i-2].DD = MININT;
    if(c < -X) break;
    c -= h;
  }
  i -= 2;
  CD[i].CC = CD[i].DD = MININT;
  tt = 0;  j = i;
  for (j_r = 1; j_r <= M; j_r++) {
    count += j - tt; CD[2].CC = CD[2].DD= MININT;
    if (!(gap_align->positionBased)) /*AAS*/
      wa = matrix[A[j_r]]; 
    else {
      if(reversed)
        wa = gap_align->posMatrix[M - j_r];
      else
        wa = gap_align->posMatrix[j_r + query_offset];
    }
    s1 = s2 = s3 = f1= f2 = MININT; f1=f2=e1 = e2 = e3 = MININT; sc = MININT;
    for(cb = i = tt, dp = &CD[i-1]; 1;) {
	if (i >= j) break;
	sc = MAX(MAX(f1, f2)-shift, s3)+wa[B[i++]];
	f1 = s3; 
	s3 = (++dp)->CC; f1 = MAX(f1, s3);
	d = dp->DD;
	if (sc < MAX(d, e1)) {
	    sc = MAX(d, e1);
	    if (best_score -sc > X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		cb = i;
		dp->CC = sc;
		dp->DD = d-h;
		e1-=h;
	    }
	} else {
	    if (best_score -sc > X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		cb = i;
		dp->CC = sc;
		if (sc > best_score) {best_score = sc; *pei = j_r;*pej=i-1;}
		if ((sc-=m) > (e1-=h)) e1 = sc;
		dp->DD = MAX(sc, d-h);
	    }
	}
	if (i >= j) {c = e1; e1 = e2; e2 = e3; e3 = c; break;}
	sc = MAX(MAX(f1, f2)-shift, s2)+wa[B[i++]];
	f2 = s2; s2 = (++dp)->CC; f2 = MAX(f2, s2);
	d = dp->DD;
	if (sc < MAX(d, e2)) {
	    if ((c=MAX(d,e2)) < best_score-X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		cb = i;
		dp->CC = c;
		dp->DD = d-h;
		e2-=h;
	    }
	} else {
	    if (sc < best_score-X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		cb = i;
		dp->CC = sc;
		if (sc > best_score) {best_score = sc;*pei= j_r; *pej=i-1;}
		if ((sc-=m) > (e2-=h)) e2 = sc;
		dp->DD = MAX(sc, d-h);
	    }
	}
	if (i >= j) { c = e2; e2 = e1; e1 = e3; e3 = c; break; }
	sc = MAX(MAX(f1, f2)-shift, s1)+wa[B[++i]];
	f1 = f2;
	f2 = s1; s1 = (++dp)->CC; f2 = MAX(f2, s1);
	d = dp->DD;
	if (sc < MAX(d, e3)) {
	    sc = MAX(d, e3);
	    if (sc < best_score-X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		cb = i;
		dp->CC = sc;
		dp->DD = d-h;
		e3-=h;
	    }
	} else {
	    if (sc < best_score-X) {
		if (tt == i-1) tt = i;
		else dp->CC = MININT;
	    } else {
		cb = i;
		dp->CC = sc;
		if (sc > best_score) {best_score = sc;*pei = j_r; *pej=i-1;}
		if ((sc-=m) > (e3-=h)) e3 = sc;
		dp->DD = MAX(sc, d-h);
	    }
	}
	sc = c;
    }
    if(tt == j) break;
    if(cb < j) { j = cb;}
    else {
	c = (MAX(e1, MAX(e2, e3))+X-best_score)/h+j;
	if (c > N) c = N;
	if (c > j)
	while (1) {
	    CD[j].CC = e1;
	    CD[j++].DD = e1 - m; e1 -=h;
	    if (j > c) break;      
	    CD[j].CC = e2;
	    CD[j++].DD = e2- m; e2-=h;
	    if (j > c) break;      
	    CD[j].CC = e3;
	    CD[j++].DD = e3- m; e3-=h;
	    if (j > c) break;
	}
    }
    c = j+4;
    if (c > N+1) c = N+1;
    while (j < c) {
	CD[j].DD = CD[j].CC = MININT;
	j++;
    }
  }
  /*
  printf("j_r=%d M=%d pei pej=%d %d\n", j_r, M, *pei, *pej);
  */
  /*printf("bestscore = %d\n", best_score);*/ 
  
  MemFree(CD);

  return best_score;
}
/*
	Allocates an EditScriptPtr and puts it on the end of
	the chain of EditScriptPtr's.  Returns a pointer to the
	new one.

*/
GapXEditScriptPtr 
GapXEditScriptNew(GapXEditScriptPtr old)

{
	GapXEditScriptPtr new;

	new = (GapXEditScriptPtr) MemNew(sizeof(GapXEditScript));

	if (old == NULL)
		return new;

	while (old->next != NULL)
	{
		old = old->next;
	}

	old->next = new;

	return new;
}

GapXEditScriptPtr LIBCALL
GapXEditScriptDelete(GapXEditScriptPtr old)
{
	GapXEditScriptPtr next;

	while (old)
	{
		next = old->next;
		old = MemFree(old);
		old = next;
	}
	return old;
}

GapXEditBlockPtr LIBCALL
GapXEditBlockNew(Int4 start1, Int4 start2)

{
	GapXEditBlockPtr edit_block;

	edit_block = (GapXEditBlockPtr) MemNew(sizeof(GapXEditBlock));
	edit_block->start1 = start1;
	edit_block->start2 = start2;

	return edit_block;
}

GapXEditBlockPtr LIBCALL
GapXEditBlockDelete(GapXEditBlockPtr edit_block)

{
	if (edit_block == NULL)
		return NULL;

	edit_block->esp = GapXEditScriptDelete(edit_block->esp);

	edit_block = MemFree(edit_block);

	return edit_block;
}

/* Alignment display routine */

GapXEditBlockPtr LIBCALL
TracebackToGapXEditBlock(Uint1Ptr A, Uint1Ptr B, Int4 M, Int4 N, Int4Ptr S, Int4 start1, Int4 start2)
{

  register Int4 i, j, op, number_of_subs, number_of_decline;
  GapXEditScriptPtr e_script;
  GapXEditBlockPtr edit_block;

  if (S == NULL)
	return NULL;

  i = j = op = number_of_subs = number_of_decline = 0;

  edit_block = GapXEditBlockNew(start1, start2);
  edit_block->esp = e_script = GapXEditScriptNew(NULL);

  while(i < M || j < N) 
  {
	op = *S;
	if (op != MININT && number_of_decline > 0) 
	{
               e_script->op_type = GAPALIGN_DECLINE;
               e_script->num = number_of_decline;
               e_script = GapXEditScriptNew(e_script);
		number_of_decline = 0;
	}
        if (op != 0 && number_of_subs > 0) 
	{
                        e_script->op_type = GAPALIGN_SUB;
                        e_script->num = number_of_subs;
                        e_script = GapXEditScriptNew(e_script);
                        number_of_subs = 0;
        }      
	if (op == 0) {
		i++; j++; number_of_subs++;
	} else if (op == MININT) {
		i++; j++;
		number_of_decline++; 
	}	
	else
	{
		if(op > 0) 
		{
			e_script->op_type = GAPALIGN_DEL;
			e_script->num = op;
			j += op;
			if (i < M || j < N)
                                e_script = GapXEditScriptNew(e_script);
		}
		else
		{
			e_script->op_type = GAPALIGN_INS;
			e_script->num = ABS(op);
			i += ABS(op);
			if (i < M || j < N)
                                e_script = GapXEditScriptNew(e_script);
		}
    	}
	S++;
  }

  if (number_of_subs > 0)
  {
	e_script->op_type = GAPALIGN_SUB;
	e_script->num = number_of_subs;
  } else if (number_of_decline > 0) {
        e_script->op_type = GAPALIGN_DECLINE;
        e_script->num = number_of_decline;
  }

  return edit_block;
}
/*
  Converts a OOF traceback from the gapped alignment to a
  GapXEditBlock.
  
  This function is for out-of-frame gapping:
  
  index1 is for the protein, index2 is for the nucleotides.

  0: deletion of a dna codon, i.e dash aligned with a protein letter.
  1: one nucleotide vs a protein, deletion of 2 nuc.
  2: 2 nucleotides aligned with a protein, i.e deletion of one nuc.
  3: substitution, 3 nucleotides vs an amino acid.
  4: 4 nuc vs an amino acid.
  5: 5 nuc vs an amino acid.
  6: a codon aligned with a dash. opposite of 0.
*/

GapXEditBlockPtr LIBCALL
OOFTracebackToGapXEditBlock(Int4 M, Int4 N, Int4Ptr S, Int4 start1, Int4 start2)
{
    register Int4 current_val, last_val, number, index1, index2;
    GapXEditScriptPtr e_script;
    GapXEditBlockPtr edit_block;
    
    if (S == NULL)
        return NULL;
    
    number = 0;
    index1 = 0;
    index2 = 0;
    
    edit_block = GapXEditBlockNew(start1, start2);
    edit_block->is_ooframe = TRUE;
    edit_block->esp = e_script = GapXEditScriptNew(NULL);
    
    last_val = *S;
    
    /* index1, M - index and length of protein, 
       index2, N - length of the nucleotide */
    
    for(index1 = 0, index2 = 0; index1 < M && index2 < N; S++, number++) {
        
        current_val = *S;
        /* New script element will be created for any new frameshift
           region  0,3,6 will be collected in a single segment */
        if (current_val != last_val || (current_val%3 != 0 && 
                                        edit_block->esp != e_script)) {
            e_script->num = number;
            e_script = GapXEditScriptNew(e_script);
            
            /* if(last_val%3 != 0 && current_val%3 == 0) */
            if(last_val%3 != 0 && current_val == 3) 
                /* 1, 2, 4, 5 vs. 0, 3, 6*/                
                number = 1;
            else
                number = 0;
        }
        
        last_val = current_val;
        
        /* for out_of_frame == TRUE - we have op_type == S parameter */
        e_script->op_type = current_val;
        
        if(current_val != 6) {
            index1++;
            index2 += current_val;
        } else {
            index2 += 3;
        }
    }
    /* Get the last one. */    
    e_script->num = number;

    return edit_block;
}

/*
	Destruct Function for GapAlignBlk.  If "state" is not NULL, then
	it's deallocated.
*/
GapAlignBlkPtr LIBCALL
GapAlignBlkDelete(GapAlignBlkPtr gap_align)

{
    if (gap_align == NULL)
        return NULL;

#if BLAST_ALTIVEC
	if (gap_align->dyn_prog_save_PTR) MemFree(gap_align->dyn_prog_save_PTR);
#endif    	

    
    gap_align->state_struct = GapXDropStateDestroy(gap_align->state_struct);
    /* GapXEditBlockDelete(gap_align->edit_block); */
    
    gap_align->dyn_prog = MemFree(gap_align->dyn_prog);
    gap_align = MemFree(gap_align);
    
    return gap_align;
}

/*
	Allocates GapAlignBlkPtr and "state", if state_column_length and 
	state_row_length are not NULL.

	For "call and forget" applications, state_column_length and
	state_row_length should both be set to zero.  ALIGN will
	then allocate and deallocate this memory.
*/

GapAlignBlkPtr LIBCALL
GapAlignBlkNew(Int4 state_column_length, Int4 state_row_length)

{
	GapAlignBlkPtr gap_align;

	gap_align = MemNew(sizeof(GapAlignBlk));
	if (gap_align == NULL)
		return NULL;

#if BLAST_ALTIVEC
	// zero these out, to be safe
	gap_align->dyn_prog_save_PTR = NULL;
	gap_align->dyn_prog_save_SIZE = 0;
#endif

	/* gap_align->decline_align = INT2_MAX; */
	return gap_align;
}

Boolean LIBCALL
PerformNtGappedAlignment(GapAlignBlkPtr gap_align)

{
	Boolean found_start, found_end;
	Int4 q_length=0, s_length=0, middle_score, score_right, score_left, private_q_start, private_s_start;
	Int4 include_query=0, index;
	Uint1Ptr query, subject, query_var, subject_var;

	if (gap_align == NULL)
		return FALSE;

	found_start = FALSE;
	found_end = FALSE;

	query = gap_align->query;
	subject = gap_align->subject;
	score_left = 0;
	if (gap_align->q_start != 0 && gap_align->s_start != 0)
	{
		found_start = TRUE;
		q_length = (gap_align->q_start + 1);
		s_length = (gap_align->s_start + 1);
		score_left = ALIGN_packed_nucl(query, subject, q_length, s_length, &private_q_start, &private_s_start, gap_align, gap_align->q_start, TRUE);
                if (score_left < 0) 
                   return FALSE;
		gap_align->query_start = q_length - private_q_start;
		gap_align->subject_start = s_length - private_s_start;
	}
	else
	{
		q_length = gap_align->q_start;
		s_length = gap_align->s_start;
	}

	middle_score = 0;
	query_var = query+gap_align->q_start;
	subject_var = subject+gap_align->s_start;
	for (index=0; index<include_query; index++)
	{
		query_var++;
		subject_var++;
		if (!(gap_align->positionBased))  /*AAS*/
		  middle_score += gap_align->matrix[*query_var][*subject_var];
		else 
		  middle_score += MtrxScoreGapAlign(gap_align,
				gap_align->q_start+1 + index,*subject_var);
	}

	score_right = 0;
	if (gap_align->q_start+include_query < gap_align->query_length && gap_align->s_start+include_query < gap_align->subject_length)
	{
		found_end = TRUE;
		score_right = ALIGN_packed_nucl(query+gap_align->q_start+include_query, subject+(gap_align->s_start+include_query)/4, gap_align->query_length-q_length-include_query, gap_align->subject_length-s_length-include_query, &(gap_align->query_stop), &(gap_align->subject_stop), gap_align, gap_align->q_start+include_query, FALSE);
                if (score_right < 0) 
                   return FALSE;
		gap_align->query_stop += gap_align->q_start+include_query;
		gap_align->subject_stop += gap_align->s_start+include_query;
	}

	if (found_start == FALSE)
	{	/* Start never found */
		gap_align->query_start = gap_align->q_start;
		gap_align->subject_start = gap_align->s_start;
	}

	if (found_end == FALSE)
	{
		gap_align->query_stop = gap_align->q_start + include_query - 1;
		gap_align->subject_stop = gap_align->s_start + include_query - 1;
	}

	gap_align->score = score_right+score_left+middle_score;

	return TRUE;
}

Boolean LIBCALL
PerformGappedAlignment(GapAlignBlkPtr gap_align)

{
    Boolean found_start, found_end;
    Int4 q_length=0, s_length=0, middle_score, score_right, score_left, private_q_start, private_s_start;
    Int4 include_query, index;
    Uint1Ptr q_left=NULL, s_left=NULL;
    Uint1Ptr query, subject, query_var, subject_var;
    
    if (gap_align == NULL)
        return FALSE;
    
    found_start = FALSE;
    found_end = FALSE;
    
    query = gap_align->query;
    subject = gap_align->subject;
    include_query = gap_align->include_query;

    /* Looking for "left" score */
    score_left = 0;
    if (gap_align->q_start != 0 && gap_align->s_start != 0) {
        found_start = TRUE;
        if(gap_align->is_ooframe) {
            q_left = (Uint1Ptr) MemNew((gap_align->q_start+3)*sizeof(Uint1));
            s_left = (Uint1Ptr) MemNew((gap_align->s_start+5)*sizeof(Uint1));
        
            q_length = reverse_seq(query, 
                                   query+gap_align->q_start-1, q_left+1);
            s_length = reverse_seq(subject, 
                                   subject+gap_align->s_start-3, s_left+3);

            score_left = OOF_SEMI_G_ALIGN(q_left, s_left+2, q_length, s_length, NULL, &private_q_start, &private_s_start, TRUE, NULL, gap_align, gap_align->q_start, TRUE);
            q_left = MemFree(q_left);
            s_left = MemFree(s_left);
        } else {
            q_length = (gap_align->q_start+1);
            s_length = (gap_align->s_start+1);
            score_left = SEMI_G_ALIGN_EX(query, subject, q_length, s_length, NULL, &private_q_start, &private_s_start, TRUE, NULL, gap_align, gap_align->q_start, FALSE, TRUE);
        }
        
        gap_align->query_start = q_length - private_q_start;
        gap_align->subject_start = s_length - private_s_start;

    } else {
        q_length = gap_align->q_start;
        s_length = gap_align->s_start;
    }

    /* Looking for "middle" score */
    middle_score = 0;
    query_var = query+gap_align->q_start;
    subject_var = subject+gap_align->s_start;
    for (index=0; index<include_query; index++) {
        query_var++;
        subject_var++;
        if (!(gap_align->positionBased))  /*AAS*/
            middle_score += gap_align->matrix[*query_var][*subject_var];
        else 
            middle_score += MtrxScoreGapAlign(gap_align,
                                              gap_align->q_start+1 + index,*subject_var);
    }
    
    score_right = 0;
    if (gap_align->q_start+include_query < gap_align->query_length && gap_align->s_start+include_query < gap_align->subject_length) {
        found_end = TRUE;
        if(gap_align->is_ooframe) {
            score_right = OOF_SEMI_G_ALIGN(query+gap_align->q_start+include_query-1, subject+gap_align->s_start+include_query-1, gap_align->query_length-q_length-include_query, gap_align->subject_length-s_length-include_query, NULL, &(gap_align->query_stop), &(gap_align->subject_stop), TRUE, NULL, gap_align, gap_align->q_start+include_query, FALSE);
        } else {
            score_right = SEMI_G_ALIGN_EX(query+gap_align->q_start+include_query, subject+gap_align->s_start+include_query, gap_align->query_length-q_length-include_query, gap_align->subject_length-s_length-include_query, NULL, &(gap_align->query_stop), &(gap_align->subject_stop), TRUE, NULL, gap_align, gap_align->q_start+include_query, FALSE, FALSE);
        }

        gap_align->query_stop += gap_align->q_start+include_query;
        gap_align->subject_stop += gap_align->s_start+include_query;
    }
    
    if (found_start == FALSE) {	/* Start never found */
        gap_align->query_start = gap_align->q_start;
        gap_align->subject_start = gap_align->s_start;
    }
    
    if (found_end == FALSE) {
        gap_align->query_stop = gap_align->q_start + include_query - 1;
        gap_align->subject_stop = gap_align->s_start + include_query - 1;

        if(gap_align->is_ooframe) {  /* Do we really need this ??? */
            gap_align->query_stop++;
            gap_align->subject_stop++;
        }
    }

    gap_align->score = score_right+score_left+middle_score;
    
    return TRUE;
}

/*
	Perform a gapped alignment and return the score.  A SeqAlignPtr with
	the traceback is also returned.
*/
Boolean LIBCALL
PerformGappedAlignmentWithTraceback(GapAlignBlkPtr gap_align)
{
    Boolean found_start, found_end;
    Int4 q_length=0, s_length=0, score_right, middle_score, score_left, private_q_length, private_s_length, tmp;
    Int4 include_query, index, prev;
    Int4Ptr tback, tback1, p, q;
    Uint1Ptr q_left=NULL, s_left=NULL;
    Uint1Ptr query, subject, query_var, subject_var;
    
    if (gap_align == NULL)
        return FALSE;
    
    found_start = FALSE;
    found_end = FALSE;
    
    query = gap_align->query;
    subject = gap_align->subject;

    /*
       tback = tback1 = MemNew((gap_align->subject_length+gap_align->query_length)*sizeof(Int4));
    */
    tback = tback1 = 
       Nlm_Malloc((gap_align->subject_length+gap_align->query_length)*sizeof(Int4));
    include_query = gap_align->include_query;

    gap_align->tback = tback;

    /* Note: the starting point [q_start,s_start] is included in the left
       extension, but not in the right extension */
    score_left = 0; prev = 3;
    found_start = TRUE;
        
    if(gap_align->is_ooframe) {
       q_left = (Uint1Ptr) MemNew((gap_align->q_start+3)*sizeof(Uint1));
       s_left = (Uint1Ptr) MemNew((gap_align->s_start+5)*sizeof(Uint1));
            
       q_length = reverse_seq(query, 
                              query+gap_align->q_start-1, q_left+1);
       if (gap_align->s_start >= 3) {
          s_length = reverse_seq(subject, 
                                 subject+gap_align->s_start-3, s_left+3);
       } else {
          s_length = gap_align->s_start - 2;
       }
       score_left = OOF_SEMI_G_ALIGN(q_left, s_left+2, q_length, s_length, tback, &private_q_length, &private_s_length, FALSE, &tback1, gap_align, gap_align->q_start, TRUE);
            
       q_left = MemFree(q_left);
       s_left = MemFree(s_left);
    } else {        
       q_length = (gap_align->q_start+1);
       s_length = (gap_align->s_start+1);
       
       score_left = SEMI_G_ALIGN_EX(query, subject, q_length, s_length, tback, &private_q_length, &private_s_length, FALSE, &tback1, gap_align, gap_align->q_start, FALSE, TRUE);
    }

    for(p = tback, q = tback1 - 1; p < q; p++, q--)  {
       tmp = *p;
       *p = *q;
       *q = tmp;
    }

    if(gap_align->is_ooframe){
       for (prev = 3, p = tback; p < tback1; p++) {
          if (*p == 0 || *p ==  6) continue;
          tmp = *p; *p = prev; prev = tmp;
       }
    }
    gap_align->query_start = q_length - private_q_length;
    gap_align->subject_start = s_length - private_s_length;

    middle_score = 0;
    query_var = query+gap_align->q_start;
    subject_var = subject+gap_align->s_start;
    for (index=0; index<include_query; index++) {
        query_var++;
        subject_var++;
        if (!(gap_align->positionBased))  /*AAS*/
            middle_score += gap_align->matrix[*query_var][*subject_var];
        else 
            middle_score += MtrxScoreGapAlign(gap_align,
                                              gap_align->q_start+1 + index,*subject_var);
        *tback1 = 0;
        tback1++;
    }
    
    score_right = 0;
    if ((gap_align->q_start+include_query) < gap_align->query_length && (gap_align->s_start+include_query) < gap_align->subject_length) {
        found_end = TRUE;
        if(gap_align->is_ooframe){
            score_right = OOF_SEMI_G_ALIGN(query+gap_align->q_start+include_query-1, subject+gap_align->s_start+include_query-1, gap_align->query_length-q_length-include_query, gap_align->subject_length-s_length-include_query, tback1, &private_q_length, &private_s_length, FALSE, &tback1, gap_align, gap_align->q_start+include_query, FALSE);
            if (prev != 3) {
                while (*p == 0 || *p == 6) p++;
                *p = prev+*p-3;
            }
        } else {
            score_right = SEMI_G_ALIGN_EX(query+gap_align->q_start+include_query, subject+gap_align->s_start+include_query, gap_align->query_length-q_length-include_query, gap_align->subject_length-s_length-include_query, tback1, &private_q_length, &private_s_length, FALSE, &tback1, gap_align, gap_align->q_start+include_query, FALSE, FALSE);
        }

        gap_align->query_stop = gap_align->q_start + private_q_length+include_query;
        gap_align->subject_stop = gap_align->s_start + private_s_length+include_query;
    }
    
    if (found_start == FALSE) {	/* Start never found */
        gap_align->query_start = gap_align->q_start;
        gap_align->subject_start = gap_align->s_start;
    }
    
    if (found_end == FALSE) {
        gap_align->query_stop = gap_align->q_start + include_query - 1;
        gap_align->subject_stop = gap_align->s_start + include_query - 1;

        if(FALSE && gap_align->is_ooframe){ /* Do we really need this ??? */
            gap_align->query_stop++;
            gap_align->subject_stop++;
        }
    }

    if(gap_align->is_ooframe) {
	gap_align->edit_block = OOFTracebackToGapXEditBlock(gap_align->query_stop-gap_align->query_start+1, gap_align->subject_stop-gap_align->subject_start+1, tback, gap_align->query_start, gap_align->subject_start);
    } else {
        gap_align->edit_block = TracebackToGapXEditBlock(query, subject, gap_align->query_stop-gap_align->query_start+1, gap_align->subject_stop-gap_align->subject_start+1, tback, gap_align->query_start, gap_align->subject_start);
    }

    gap_align->edit_block->frame1 = gap_align->query_frame;
    gap_align->edit_block->frame2 = gap_align->subject_frame;
    gap_align->edit_block->length1 = gap_align->query_length;
    gap_align->edit_block->length2 = gap_align->subject_length;
    gap_align->edit_block->translate1 = gap_align->translate1;
    gap_align->edit_block->translate2 = gap_align->translate2;
    gap_align->edit_block->discontinuous = gap_align->discontinuous;

#ifdef OOF_DEBUG_PRINTOUT
    {{
        CharPtr pro_seq, dnap_seq;
        SeqMapTablePtr smtp;
        Int4 i;

        pro_seq = MemNew(gap_align->query_length+1);
        dnap_seq = MemNew(gap_align->subject_length+3*CODON_LENGTH);

        smtp = SeqMapTableFindObj(Seq_code_ncbieaa, Seq_code_ncbistdaa);
        
        for(i = 0; i < gap_align->query_length; i++)
            pro_seq[i] = SeqMapTableConvert(smtp, query[i]);
        
        for(i = 0; i < gap_align->subject_length; i++)
            dnap_seq[i] = SeqMapTableConvert(smtp, subject[i]);
            
        /* for(i =0; i <gap_align->subject_length+
           gap_align->query_length; i++)
           printf("%d ", tback[i]); */
        
        OOFDisplayTraceBack1(tback, dnap_seq, pro_seq, 
                             gap_align->subject_stop-1, 
                             gap_align->query_stop-1, 
                             gap_align->query_start, 
                             gap_align->subject_start);
        MemFree(dnap_seq);
        MemFree(pro_seq);
    }}
#endif

    tback = MemFree(tback);
    gap_align->tback = NULL;

    gap_align->score = score_right+score_left+middle_score;
    
    return TRUE;
}

/*
	Get the current position.
*/

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

static SeqAlignPtr GXEMakeSeqAlign(SeqIdPtr query_id, SeqIdPtr subject_id, 
                                   Boolean reverse, Boolean translate1, 
                                   Boolean translate2, Int4 numseg,
                                   Int4Ptr length, Int4Ptr start, 
                                   Uint1Ptr strands)
{
    SeqAlignPtr sap;
    DenseSegPtr dsp;
    StdSegPtr sseg, sseg_head, sseg_old;
    SeqLocPtr slp, slp1, slp2;
    SeqIntPtr seq_int1;
    Int4 index;
    Boolean tmp_value;

    sap = SeqAlignNew();
    
    sap->dim =2; /**only two dimention alignment**/
    
    /**make the Denseg Object for SeqAlign**/
    if (translate1 == FALSE && translate2 == FALSE) {
        sap->segtype = SAS_DENSEG; /** use denseg to store the alignment **/
        sap->type = SAT_PARTIAL;   /**partial for gapped translating search.*/
        dsp = DenseSegNew();
        dsp->dim = 2;
        dsp->numseg = numseg;
        if (reverse) {
            dsp->ids = SeqIdDup(subject_id);
            dsp->ids->next = SeqIdDup(query_id);
        } else {
            dsp->ids = SeqIdDup(query_id);
            dsp->ids->next = SeqIdDup(subject_id);
        }
        dsp->starts = start;
        dsp->strands = strands;
        dsp->lens = length;
        sap->segs = dsp;
        sap->next = NULL;
    } else { /****/
        sap->type = SAT_PARTIAL; /**partial for gapped translating search. */
        sap->segtype = SAS_STD;  /**use stdseg to store the alignment**/
        sseg_head = NULL;
        sseg_old = NULL;

        if(reverse) {  /* Reverting translation flags */
            tmp_value = translate1;
            translate1 = translate2;
            translate2 = tmp_value;
        }
        
        for (index=0; index<numseg; index++) {
            sseg = StdSegNew();
            sseg->dim = 2;
            if (sseg_head == NULL) {
                sseg_head = sseg;
            }
            if (reverse) {
                sseg->ids = SeqIdDup(subject_id);
                sseg->ids->next = SeqIdDup(query_id);
            } else {
                sseg->ids = SeqIdDup(query_id);
                sseg->ids->next = SeqIdDup(subject_id);
            }

            slp1 = NULL;
            if (start[2*index] != -1) {
                seq_int1 = SeqIntNew();
                seq_int1->from = start[2*index];
                if (translate1)
                    seq_int1->to = start[2*index] + CODON_LENGTH*length[index] - 1;
                else
                    seq_int1->to = start[2*index] + length[index] - 1;
                seq_int1->strand = strands[2*index];

                if(reverse) { 
                    seq_int1->id = SeqIdDup(subject_id);
                } else {
                    seq_int1->id = SeqIdDup(query_id);
                }

                ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
            } else {
                if(reverse) { 
                    ValNodeAddPointer(&slp1, SEQLOC_EMPTY, SeqIdDup(subject_id));
                } else {
                    ValNodeAddPointer(&slp1, SEQLOC_EMPTY, SeqIdDup(query_id));
                }
            }
            slp2 = NULL;
            if (start[2*index+1] != -1) {
                seq_int1 = SeqIntNew();
                seq_int1->from = start[2*index+1];
                if (translate2)
                    seq_int1->to = start[2*index+1] + CODON_LENGTH*length[index] - 1;
                else
                    seq_int1->to = start[2*index+1] + length[index] - 1;
                seq_int1->strand = strands[2*index+1];

                if(reverse) { 
                    seq_int1->id = SeqIdDup(query_id);
                } else {
                    seq_int1->id = SeqIdDup(subject_id);
                }

                ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int1);
            } else {
                if(reverse) { 
                    ValNodeAddPointer(&slp2, SEQLOC_EMPTY, SeqIdDup(query_id));
                } else {
                    ValNodeAddPointer(&slp2, SEQLOC_EMPTY, SeqIdDup(subject_id));
                }
            }
            /*
              if (reverse) {
              slp = slp2;
              slp2->next = slp1;
              } else {
              slp = slp1;
              slp1->next = slp2;
              } */

            slp = slp1;
            slp1->next = slp2;

            sseg->loc = slp;
            
            if (sseg_old)
                sseg_old->next = sseg;
            sseg_old = sseg;
        }
        sap->segs = sseg_head;
        sap->next = NULL;
        
        MemFree(start);
        MemFree(length);
        MemFree(strands);
    }

    return sap;
}

Boolean GXECollectDataForSeqalign(GapXEditBlockPtr edit_block, 
                                  GapXEditScriptPtr curr_in, Int4 numseg,
                                  Int4Ptr PNTR start_out, 
                                  Int4Ptr PNTR length_out,
                                  Uint1Ptr PNTR strands_out,
                                  Int4Ptr start1, Int4Ptr start2)
{
    GapXEditScriptPtr curr;
    Boolean reverse, translate1, translate2;
    Int2 frame1, frame2;
    Int4 begin1, begin2, index, length1, length2;
    Int4 original_length1, original_length2, i;
    Int4Ptr length, start;
    Uint1 strand1, strand2;
    Uint1Ptr strands;
    
    reverse = edit_block->reverse;	
    length1 = edit_block->length1;
    length2 = edit_block->length2;
    original_length1 = edit_block->original_length1;
    original_length2 = edit_block->original_length2;
    translate1 = edit_block->translate1;
    translate2 = edit_block->translate2;
    frame1 = edit_block->frame1;
    frame2 = edit_block->frame2;
    
    if (frame1 > 0)
        strand1 = Seq_strand_plus; 
    else if (frame1 < 0)
        strand1 = Seq_strand_minus; 
    else
        strand1 = Seq_strand_unknown; 
    
    if (frame2 > 0)
        strand2 = Seq_strand_plus; 
    else if (frame2 < 0)
        strand2 = Seq_strand_minus; 
    else
        strand2 = Seq_strand_unknown; 

    start = MemNew((2*numseg+1)*sizeof(Int4));
    *start_out = start;
    length = MemNew((numseg+1)*sizeof(Int4));
    *length_out = length;
    strands = MemNew((2*numseg+1)*sizeof(Uint1));
    *strands_out = strands;

    index=0;
    for (i = 0, curr=curr_in; curr && i < numseg; curr=curr->next, i++) {
        switch(curr->op_type) {
        case GAPALIGN_DECLINE:
        case GAPALIGN_SUB:
            if (strand1 != Seq_strand_minus) {
                if(translate1 == FALSE)
                    begin1 = get_current_pos(start1, curr->num);
                else
                    begin1 = frame1 - 1 + CODON_LENGTH*get_current_pos(start1, curr->num);
            } else {
                if(translate1 == FALSE)
                    begin1 = length1 - get_current_pos(start1, curr->num) - curr->num;
                else
                    begin1 = original_length1 - CODON_LENGTH*(get_current_pos(start1, curr->num)+curr->num) + frame1 + 1;
            }
					
            if (strand2 != Seq_strand_minus) {
                if(translate2 == FALSE)
                    begin2 = get_current_pos(start2, curr->num);
                else
                    begin2 = frame2 - 1 + CODON_LENGTH*get_current_pos(start2, curr->num);
            } else {
                if(translate2 == FALSE)
                    begin2 = length2 - get_current_pos(start2, curr->num) - curr->num;
                else
                    begin2 = original_length2 - CODON_LENGTH*(get_current_pos(start2, curr->num)+curr->num) + frame2 + 1;
            }
            
            if (reverse) {
                strands[2*index] = strand2;
                strands[2*index+1] = strand1;
                start[2*index] = begin2;
                start[2*index+1] = begin1;
            } else {
                strands[2*index] = strand1;
                strands[2*index+1] = strand2;
                start[2*index] = begin1;
                start[2*index+1] = begin2;
            }
            
            break;
            
        case GAPALIGN_DEL:
            begin1 = -1;
            if (strand2 != Seq_strand_minus) {
                if(translate2 == FALSE)
                    begin2 = get_current_pos(start2, curr->num);
                else
                    begin2 = frame2 - 1 + CODON_LENGTH*get_current_pos(start2, curr->num);
            } else {
                if(translate2 == FALSE)
                    begin2 = length2 - get_current_pos(start2, curr->num) - curr->num;
                else
                    begin2 = original_length2 - CODON_LENGTH*(get_current_pos(start2, curr->num)+curr->num) + frame2 + 1;
            }
            
            if (reverse) {
                strands[2*index] = strand2;
                if (index > 0)
                    strands[2*index+1] = strands[2*(index-1)+1];
                else
                    strands[2*index+1] = Seq_strand_unknown;
                start[2*index] = begin2;
                start[2*index+1] = begin1;
            } else {
                if (index > 0)
                    strands[2*index] = strands[2*(index-1)];
                else
                    strands[2*index] = Seq_strand_unknown;
                strands[2*index+1] = strand2;
                start[2*index] = begin1;
                start[2*index+1] = begin2;
            }
            
            break;
            
        case GAPALIGN_INS:
            if (strand1 != Seq_strand_minus) {
                if(translate1 == FALSE)
                    begin1 = get_current_pos(start1, curr->num);
                else
                    begin1 = frame1 - 1 + CODON_LENGTH*get_current_pos(start1, curr->num);
            } else {
                if(translate1 == FALSE)
                    begin1 = length1 - get_current_pos(start1, curr->num) - curr->num;
                else
                    begin1 = original_length1 - CODON_LENGTH*(get_current_pos(start1, curr->num)+curr->num) + frame1 + 1;
            }
            begin2 = -1;
            if (reverse) {
                if (index > 0)
                    strands[2*index] = strands[2*(index-1)];
                else
                    strands[2*index] = Seq_strand_unknown;
                strands[2*index+1] = strand1;
                start[2*index] = begin2;
                start[2*index+1] = begin1;
            } else {
                strands[2*index] = strand1;
                if (index > 0)
                    strands[2*index+1] = strands[2*(index-1)+1];
                else
                    strands[2*index+1] = Seq_strand_unknown;
                start[2*index] = begin1;
                start[2*index+1] = begin2;
            }
            
            break;
        default:
            break;
        }
        length[index] = curr->num;
        index++;
    }    

    return TRUE;
}

static void GXECorrectUASequence(GapXEditBlockPtr edit_block)
{
    GapXEditScriptPtr curr, curr_last, curr_last2;
    Boolean last_indel;

    last_indel = FALSE;
    curr_last = NULL;
    curr_last2 = NULL;

    for (curr=edit_block->esp; curr; curr = curr->next) {

        if(curr->op_type == GAPALIGN_DECLINE && last_indel == TRUE) {
            /* This is invalid condition and regions should be
               exchanged */

            if(curr_last2 != NULL)
                curr_last2->next = curr;
            else
                edit_block->esp = curr; /* First element in a list */
            
            curr_last->next = curr->next;
            curr->next = curr_last;
        }
        
        last_indel = FALSE;
        
        if(curr->op_type == GAPALIGN_INS || curr->op_type == GAPALIGN_DEL) {
            last_indel = TRUE;
            curr_last2 = curr_last;
        }

        curr_last = curr;
    }
    return;
}

/* 
   Convert an EditScript chain to a SeqAlign of type DenseSeg.
   Used for a non-simple interval (i.e., one without subs. or 
   deletions).  
   
   The first SeqIdPtr in the chain of subject_id and query_id is duplicated for
   the SeqAlign.
   */

SeqAlignPtr LIBCALL
GapXEditBlockToSeqAlign(GapXEditBlockPtr edit_block, SeqIdPtr subject_id, SeqIdPtr query_id)

{
    GapXEditScriptPtr curr, curr_last;
    Int4 numseg, start1, start2;
    Int4Ptr length, start;
    Uint1Ptr strands;
    Boolean is_disc_align = FALSE;
    SeqAlignPtr sap, sap_disc, sap_head, sap_tail;
    Boolean skip_region = FALSE;

    is_disc_align = FALSE; numseg = 0;

    for (curr=edit_block->esp; curr; curr = curr->next) {
        numseg++;
        if(/*edit_block->discontinuous && */curr->op_type == GAPALIGN_DECLINE)
           is_disc_align = TRUE;
    }
    
    start1 = edit_block->start1;
    start2 = edit_block->start2;
    
    /* If no GAPALIGN_DECLINE regions exists output seqalign will be
       regular Den-Seg or Std-seg */
    if(is_disc_align == FALSE) {
        /* Please note, that edit_block passed only for data like
           strand, translate, reverse etc. Real data is taken starting
           from "curr" and taken only "numseg" segments */
        
        GXECollectDataForSeqalign(edit_block, edit_block->esp, numseg,
                                  &start, &length, &strands, &start1, &start2);
        
        /* Result of this function will be either den-seg or Std-seg
           depending on translation options */
        sap = GXEMakeSeqAlign(query_id, subject_id, edit_block->reverse, 
                              edit_block->translate1, edit_block->translate2, 
                              numseg, length, start, strands);
    } else {

        /* By request of Steven Altschul - we need to have 
           the unaligned part being to the left if it is adjacent to the
           gap (insertion or deletion) - so this function will do
           shaffeling */

        GXECorrectUASequence(edit_block); 

        sap_disc = SeqAlignNew();
        sap_disc->dim = 2;
        sap_disc->type = SAT_PARTIAL; /* ordered segments, over part of seq */
        sap_disc->segtype = SAS_DISC; /* discontinuous alignment */
        
        curr_last = edit_block->esp; 
        sap_head = NULL; sap_tail = NULL;
        while(curr_last != NULL) {
            skip_region = FALSE;                        
            for (numseg = 0, curr = curr_last; 
                 curr; curr = curr->next, numseg++) {

                if(curr->op_type == GAPALIGN_DECLINE) {
                    if(numseg != 0) { /* End of aligned area */
                        break;
                    } else {
                        while(curr && curr->op_type == GAPALIGN_DECLINE) {
                            numseg++;
                            curr = curr->next; 
                        }
                        skip_region = TRUE;                        
                        break;
                    }
                }
            }

            GXECollectDataForSeqalign(edit_block, curr_last, numseg,
                                      &start, &length, &strands, 
                                      &start1, &start2);
            
            if(!skip_region) {            
                sap = GXEMakeSeqAlign(query_id, subject_id, 
                                      edit_block->reverse, 
                                      edit_block->translate1, 
                                      edit_block->translate2,
                                      numseg, length, start, strands);
                
                /* Collecting all seqaligns into single linked list */
                if(sap_tail == NULL) {
                    sap_head = sap_tail = sap;
                } else {
                    sap_tail->next = sap;
                    sap_tail = sap;
                }
            }
            
            curr_last = curr;
        }
        sap_disc->segs = sap_head;
        sap = sap_disc;
    }

    return sap;
}

/* 
   This function is used for Out-Of-Frame traceback conversion
   Convert an OOF EditScript chain to a SeqAlign of type StdSeg.
   Used for a non-simple interval (i.e., one without subs. or 
   deletions).  
   
   The first SeqIdPtr in the chain of subject_id and query_id is duplicated for
   the SeqAlign.
*/
SeqAlignPtr LIBCALL OOFGapXEditBlockToSeqAlign(GapXEditBlockPtr edit_block, SeqIdPtr subject_id, SeqIdPtr query_id, Int4 query_length)
{
    Boolean reverse;
    GapXEditScriptPtr curr, esp;
    Int2 frame1, frame2;
    Int4 start1, start2;
    Int4 original_length1, original_length2;
    SeqAlignPtr sap;
    SeqIntPtr seq_int1, seq_int2;
    SeqIntPtr seq_int1_last = NULL, seq_int2_last = NULL;
    SeqIdPtr sip;
    SeqLocPtr slp, slp1, slp2;
    StdSegPtr sseg, sseg_head, sseg_old;
    Uint1 strand1, strand2;
    Boolean first_shift;

    reverse = edit_block->reverse;	
    

    start1 = edit_block->start1;
    start2 = edit_block->start2;
    frame1 = edit_block->frame1;
    frame2 = edit_block->frame2;

    original_length1 = edit_block->original_length2; /* Protein */
    original_length2 = edit_block->original_length1; /* DNA */
    
    /* printf("%d %d %d %d\n", start1, start2, original_length1,
       original_length2); */

    if(frame1 > 0) 
        strand1 = Seq_strand_plus;
    else if (frame1 < 0)
        strand1 = Seq_strand_minus;
    else
        strand1 = Seq_strand_unknown;
    
    if(frame2 > 0) 
        strand2 = Seq_strand_plus;
    else if (frame2 < 0)
        strand2 = Seq_strand_minus;
    else
        strand2 = Seq_strand_unknown;
    
    esp = edit_block->esp;
    
    sap = SeqAlignNew();
    
    sap->dim =2; /**only two dimention alignment**/
    
    sap->type =3; /**partial for gapped translating search. */
    sap->segtype =3; /**use denseg to store the alignment**/
    sseg_head = NULL;
    sseg_old = NULL;
    esp = edit_block->esp;

    /* query_length--; */

    first_shift = FALSE;

    for (curr=esp; curr; curr=curr->next) {

        slp1 = NULL;
        slp2 = NULL;
        
        switch (curr->op_type) {
        case 0: /* deletion of three nucleotides. */
            
            first_shift = FALSE;

            seq_int1 = SeqIntNew();
            seq_int1->from = get_current_pos(&start1, curr->num);
            seq_int1->to = start1 - 1;            

            if(seq_int1->to >= original_length1)
                seq_int1->to = original_length1-1;
            
            seq_int1->id = SeqIdDup(subject_id);
            seq_int1->strand = strand1;

            ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);

            /* Empty nucleotide piece */
            ValNodeAddPointer(&slp2, SEQLOC_EMPTY, SeqIdDup(query_id));
            
            seq_int1_last = seq_int1;
            /* Keep previous seq_int2_last, in case there is a frame shift
               immediately after this gap */
            
            break;

        case 6: /* insertion of three nucleotides. */

            /* If gap is followed after frameshift - we have to
               add this element for the alignment to be correct */
            
            if(first_shift == TRUE) { /* Second frameshift in a row */
                /* Protein coordinates */
                seq_int1 = SeqIntNew();
                seq_int1->from =  get_current_pos(&start1, 1);
                seq_int1->to = start1 - 1;

                if(seq_int1->to >= original_length1)
                    seq_int1->to = original_length1-1;
                
                seq_int1->id = SeqIdDup(subject_id);
                seq_int1->strand = strand1;
                
                ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
                
                /* Nucleotide scale shifted by op_type */
                seq_int2 = SeqIntNew();

                seq_int2->from = get_current_pos(&start2, 3);
                seq_int2->to = start2 - 1;

                if(seq_int2->to >= query_length) {/* Possible with frame shifts */
                    seq_int2->to = query_length -1;
                    seq_int1->to--;
                }

                /* Transfer to DNA minus strand coordinates */
                if(strand2 == Seq_strand_minus) {
                    int tmp_int;
                    tmp_int = seq_int2->to;
                    seq_int2->to = original_length2 - seq_int2->from - 1;
                    seq_int2->from = original_length2 - tmp_int - 1;
                }
            
                seq_int2->id = SeqIdDup(query_id);
                seq_int2->strand = strand2;
                
                ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int2);

                /* seq_int1_last = seq_int1; 
                   seq_int2_last = seq_int2; */

                /* first_shift = FALSE; */

                if (reverse) {
                    slp = slp1;
                    slp1->next = slp2;
                    sip = SeqIdDup(subject_id);
                    sip->next = SeqIdDup(query_id);
                } else {
                    slp = slp2;
                    slp2->next = slp1;
                    sip = SeqIdDup(query_id);
                    sip->next = SeqIdDup(subject_id);
                }
                
                sseg = StdSegNew();
                sseg->dim = 2;
                
                if (sseg_head == NULL)
                    sseg_head = sseg;
                
                sseg->loc = slp;
                sseg->ids = sip;
                
                if (sseg_old)
                    sseg_old->next = sseg;
                
                sseg_old = sseg;

                slp1 = NULL;
                slp2 = NULL;
            }

            first_shift = FALSE;

            /* Protein piece is empty */
            ValNodeAddPointer(&slp1, SEQLOC_EMPTY, SeqIdDup(subject_id));
            
            /* Nucleotide scale shifted by 3, protein gapped */
            seq_int2 = SeqIntNew();              
            seq_int2->from = get_current_pos(&start2, curr->num*3);
            seq_int2->to = start2 - 1;

            if(seq_int2->to >= query_length) {/* Possible with frame shifts */
                seq_int2->to = query_length -1;
            }

            /* Transfer to DNA minus strand coordinates */
            if(strand2 == Seq_strand_minus) {
                int tmp_int;
                tmp_int = seq_int2->to;
                seq_int2->to = original_length2 - seq_int2->from - 1;
                seq_int2->from = original_length2 - tmp_int - 1;
            }

            seq_int2->id = SeqIdDup(query_id);
            seq_int2->strand = strand2;
            
            ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int2);
            
            seq_int1_last = NULL;
            seq_int2_last = seq_int2; /* Will be used to adjust "to" value */
            
            break;

        case 3: /* Substitution. */

            first_shift = FALSE;

            /* Protein coordinates */
            seq_int1 = SeqIntNew();
            seq_int1->from =  get_current_pos(&start1, curr->num);
            seq_int1->to = start1 - 1;

            if(seq_int1->to >= original_length1)
                seq_int1->to = original_length1-1;
            
            seq_int1->id = SeqIdDup(subject_id);
            seq_int1->strand = strand1;

            ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
           
            /* Nucleotide scale shifted by op_type */
            seq_int2 = SeqIntNew();

            /* Adjusting last segment and new start point in
               nucleotide coordinates */
            /* if(seq_int2_last != NULL) {
               seq_int2_last->to = seq_int2_last->to - (3 - curr->op_type);
               start2 = start2 - (3 - curr->op_type);
               } */
            
            seq_int2->from = get_current_pos(&start2, curr->num*curr->op_type);
            seq_int2->to = start2 - 1;

		/* Chop off three bases and one residue at a time.
			Why does this happen, seems like a bug?
		*/
            while (seq_int2->to >= query_length) {/* Possible with frame shifts */
                seq_int2->to -= 3;
                seq_int1->to--;
            }

            /* Transfer to DNA minus strand coordinates */
            if(strand2 == Seq_strand_minus) {
                int tmp_int;
                tmp_int = seq_int2->to;
                seq_int2->to = original_length2 - seq_int2->from - 1;
                seq_int2->from = original_length2 - tmp_int - 1;
            }
            
            seq_int2->id = SeqIdDup(query_id);
            seq_int2->strand = strand2;

            ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int2);

            seq_int1_last = seq_int1; /* Will be used to adjust "to" value */
            seq_int2_last = seq_int2; /* Will be used to adjust "to" value */
            
            break;
        case 1:	/* gap of two nucleotides. */
        case 2: /* Gap of one nucleotide. */
        case 4: /* Insertion of one nucleotide. */
        case 5: /* Insertion of two nucleotides. */

            if(first_shift == TRUE) { /* Second frameshift in a row */
                /* Protein coordinates */
                seq_int1 = SeqIntNew();
                seq_int1->from =  get_current_pos(&start1, 1);
                seq_int1->to = start1 - 1;

                if(seq_int1->to >= original_length1)
                    seq_int1->to = original_length1-1;
                
                seq_int1->id = SeqIdDup(subject_id);
                seq_int1->strand = strand1;
                
                ValNodeAddPointer(&slp1, SEQLOC_INT, seq_int1);
                
                /* Nucleotide scale shifted by op_type */
                seq_int2 = SeqIntNew();

                seq_int2->from = get_current_pos(&start2, curr->op_type);
                seq_int2->to = start2 - 1;

                if(seq_int2->to >= query_length) {/* Possible with frame shifts */
                    seq_int2->to = query_length -1;
                    seq_int1->to--;
                }

                /* Transfer to DNA minus strand coordinates */
                if(strand2 == Seq_strand_minus) {
                    int tmp_int;
                    tmp_int = seq_int2->to;
                    seq_int2->to = original_length2 - seq_int2->from - 1;
                    seq_int2->from = original_length2 - tmp_int - 1;
                }
            
                seq_int2->id = SeqIdDup(query_id);
                seq_int2->strand = strand2;
                
                ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int2);

                seq_int1_last = seq_int1; 
                seq_int2_last = seq_int2; 

                /* first_shift = FALSE; */

                break;
            }
            
            first_shift = TRUE;

            /* If this substitution is following simple frameshift
               we do not need to start new segment, but may continue
               old one */
            /* printf("curr_num = %d (%d)\n",  curr->num, curr->op_type); */

            if(seq_int2_last != NULL) {
                get_current_pos(&start2, curr->num*(curr->op_type-3));
                if(strand2 != Seq_strand_minus) {
                    seq_int2_last->to = start2 - 1;
                } else {
                    /* Transfer to DNA minus strand coordinates */
                    seq_int2_last->from = original_length2 - start2;
                }

                /* Adjustment for multiple shifts - theoretically possible,
                   but very unprobable */
                if(seq_int2_last->from > seq_int2_last->to) {
                    
                    if(strand2 != Seq_strand_minus) {
                        seq_int2_last->to += 3;
                    } else {
                        seq_int2_last->from -= 3;
                    }
                    
                    if(seq_int1_last != 0)
                        seq_int1_last++;
                }

            } else if (curr->op_type > 3) {
                /* Protein piece is empty */
                ValNodeAddPointer(&slp1, SEQLOC_EMPTY, SeqIdDup(subject_id));
                /* Simulating insertion of nucleotides */
                seq_int2 = SeqIntNew();
                seq_int2->from = get_current_pos(&start2, 
                                                 curr->num*(curr->op_type-3));
                seq_int2->to = start2 - 1;
                
                if(seq_int2->to >= query_length) {
                    seq_int2->to = query_length - 1;
                }

                /* Transfer to DNA minus strand coordinates */
                if(strand2 == Seq_strand_minus) {
                    int tmp_int;
                    tmp_int = seq_int2->to;
                    seq_int2->to = original_length2 - seq_int2->from - 1;
                    seq_int2->from = original_length2 - tmp_int - 1;
                }

                seq_int2->id = SeqIdDup(query_id);
                seq_int2->strand = strand2;
                
                ValNodeAddPointer(&slp2, SEQLOC_INT, seq_int2);
                
                seq_int1_last = NULL;
                seq_int2_last = seq_int2; /* Will be used to adjust "to" value */
                break;
            } else {
                continue;       /* Main loop */
            }
            continue;       /* Main loop */
            /* break; */
        default:
            continue;       /* Main loop */
            /* break; */
        } 

        if (reverse) {
            slp = slp1;
            slp1->next = slp2;
            sip = SeqIdDup(subject_id);
            sip->next = SeqIdDup(query_id);
        } else {
            slp = slp2;
            slp2->next = slp1;
            sip = SeqIdDup(query_id);
            sip->next = SeqIdDup(subject_id);
        }

        sseg = StdSegNew();
        sseg->dim = 2;
        
        if (sseg_head == NULL)
            sseg_head = sseg;
        
        sseg->loc = slp;
        sseg->ids = sip;
        
        if (sseg_old)
            sseg_old->next = sseg;

        sseg_old = sseg;
    }
    sap->segs = sseg_head;
    sap->next = NULL;
    
    return sap;
}
/*
	SimpleIntervalToGapXEditBlock(Int4 start1, Int4 start2, Int4 length)

	Int4 start1, start2: offset of this interval in sequence 1 and 2.
	Int4 length: length of the interval.

	May be used to produce a gapXEditBlock when there are no subs. or deletions
	in the interval (e.g., ungapped BLAST HSP).
*/

GapXEditBlockPtr LIBCALL
SimpleIntervalToGapXEditBlock (Int4 start1, Int4 start2, Int4 length)

{
	GapXEditBlockPtr edit_block;
  	GapXEditScriptPtr e_script;

	edit_block = GapXEditBlockNew(start1, start2);

	edit_block->esp = e_script = GapXEditScriptNew(NULL);

	e_script->op_type = GAPALIGN_SUB;
	e_script->num = length;

	return edit_block;
}

