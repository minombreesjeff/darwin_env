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

File name: gapxdrop.h

Author: Gennadiy Savchuk, Jinqhui Zhang, Tom Madden

Contents: prototypes to perform a gapped alignment on two sequences.

****************************************************************************/
/* $Revision: 6.19 $
* $Log: gapxdrop.h,v $
* Revision 6.19  2003/02/28 22:57:24  dondosha
* Added prototype for OOFTracebackToGapXEditBlock
*
* Revision 6.18  2002/05/09 17:01:23  dondosha
* Renamed typedefs dp_ptr and dp_node to GapXDPPtr and GapXDP
*
* Revision 6.17  2002/05/09 16:13:51  dondosha
* Changed name of the dp_node structure
*
* Revision 6.16  2002/05/08 22:48:27  dondosha
* Allocate memory for dynamic programming upfront in Mega BLAST case
*
* Revision 6.15  2001/07/19 22:13:32  dondosha
* Made GapXEditScriptDelete public for use in mblast.c
*
* Revision 6.14  2000/11/30 21:37:39  madden
* Roll back before GapXDropSetAlignMask changes
*
* Revision 6.13  2000/11/15 15:15:57  madden
* Add GapXDrop_dp, align_mask, and x_align_mask
*
* Revision 6.12  2000/08/08 21:46:20  shavirin
* Added boolean discontinuous to GapAlignBlk and GapXEditBlock
*
* Revision 6.11  2000/07/17 15:26:06  shavirin
* Added parameter to function OOFGapXEditBlockToSeqAlign().
*
* Revision 6.10  2000/07/11 20:49:07  shavirin
* Added all major functions for Out-Of-Frame alignment.
*
* Revision 6.8  2000/05/16 18:36:15  dondosha
* Added prototype for GXECollectDataForSeqalign
*
* Revision 6.7  2000/03/29 21:54:57  dondosha
* Added prototype for GapXEditScriptNew
*
* Revision 6.6  1999/11/26 22:07:48  madden
* Added PerformNtGappedAlignment and ALIGN_packed_nucl
*
* Revision 6.5  1999/03/17 16:49:10  madden
* Removed comment within comment
*
* Revision 6.4  1998/11/17 13:39:03  madden
* Made ALIGN non-static
*
 * Revision 6.3  1998/08/26 18:51:08  kans
 * fixed -v -fd warning
 *
 * Revision 6.2  1998/04/17 19:41:18  madden
 * Zhengs changes for decline to align
 *
 * Revision 6.1  1997/09/22 17:36:31  madden
 * MACROS for position-specific matrices from Andy Neuwald
 *
 * Revision 6.0  1997/08/25 18:53:17  madden
 * Revision changed to 6.0
 *
 * Revision 1.17  1997/04/15 22:01:53  madden
 * Added original_length[12] for translating searches.
 *
 * Revision 1.16  1997/03/14  21:01:59  madden
 * Changed to use less memory in ALIGN, with GapXDropStateArrayStructPtr.
 *
 * Revision 1.15  1997/03/01  18:25:33  madden
 * Boolean reverse added to GapXEditBlock.
 *
 * Revision 1.14  1997/02/23  16:44:47  madden
 * Memory, saved on GapAlignBlkPtr, reuses.
 *
 * Revision 1.13  1997/02/20  22:58:49  madden
 * Added CODON_LENGTH define.
 *
 * Revision 1.12  1997/02/20  21:50:24  madden
 * Added frame and translation information to GapAlignBlk, assigned it.
 *
 * Revision 1.11  1997/02/10  15:25:33  madden
 * Added posConverged and posMatrix.
 *
 * Revision 1.10  1997/02/04  16:22:32  madden
 * Changes to enable gapped alignments on the reverse strand.
 *
 * Revision 1.9  1997/01/17  17:41:44  madden
 * Added flags for position based BLAST.
 *
 * Revision 1.8  1997/01/16  20:20:49  madden
 * TracebackToGapXEditBlock made non-static.
 *
 * Revision 1.7  1997/01/06  22:40:55  madden
 * Added function SimpleIntervalToGapXEditBlock.
 *
 * Revision 1.6  1997/01/06  19:31:49  madden
 * Removed subject and query ID from GapAlignBlk.
 *
 * Revision 1.5  1997/01/06  17:22:59  madden
 * Added GapXEditBlockPtr.
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


#ifndef __GAPXDROP__
#define __GAPXDROP__

#include <ncbi.h>
#include <readdb.h>
#include <sequtil.h>

#include "blastaltivec.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CODON_LENGTH 3

#define GAPALIGN_SUB ((Uint1)0)  /*op types within the edit script*/
#define GAPALIGN_INS ((Uint1)1)
#define GAPALIGN_DEL ((Uint1)2)
#define GAPALIGN_DECLINE ((Uint1)3)

typedef struct gapx_edit_script {
        Uint1 op_type;  /* GAPALIGN_SUB, GAPALIGN_INS, or GAPALIGN_DEL */
        Int4 num;       /* Number of operations */
        struct gapx_edit_script PNTR next;
} GapXEditScript, PNTR GapXEditScriptPtr;

typedef struct gapx_edit_block {
    Int4 start1,  start2,       /* starts of alignments. */
        length1, length2,       /* total lengths of the sequences. */
        original_length1,	/* Untranslated lengths of the sequences. */
        original_length2;	
    Int2 frame1, frame2;	    /* frames of the sequences. */
    Boolean translate1,	translate2; /* are either of these be translated. */
    Boolean reverse;	/* reverse sequence 1 and 2 when producing SeqALign? */
    Boolean is_ooframe; /* Is this out_of_frame edit block? */
    Boolean discontinuous; /* Is this OK to produce discontinuous SeqAlign? */
    GapXEditScriptPtr esp;
} GapXEditBlock, PNTR GapXEditBlockPtr;

/*
	Structure to keep memory for state structure.
*/
typedef struct _state_array_struct {
	Int4 	length,		/* length of the state_array. */
		used;		/* how much of length is used. */
	Uint1Ptr state_array;	/* array to be used. */
	struct _state_array_struct PNTR next;
} GapXDropStateArrayStruct, PNTR GapXDropStateArrayStructPtr;

/***************************************************************************
  Macros added by Andy Neuwald in order to allow easy modification of matrices.
***************************************************************************/

#define  MtrxScoreGapAlign(S,x,y)      ((S)->posMatrix[(x)][(y)])
#define  PtrMtrxScoreGapAlign(S,x)     ((S)->posMatrix[(x)])

#define  MtrxScoreGapAlign2(S,x,y)    \
        ((S)->posMatrix[( (x) %(S)->query_length)][(y)])
#define  PtrMtrxScoreGapAlign2(S,x)    \
        ((S)->posMatrix[( (x) %(S)->query_length)])

/********************************************************************/

/* Dynamic Programming structure. */
typedef struct _gapxdrop_DP_ {
  Int4 CC, DD, FF;	/* Values for gap opening and extensions (?). */
} PNTR GapXDPPtr, GapXDP;

/*
	Structure used to pass arguments for gapped alignment functions.
*/

typedef struct _gapalign_blk {
    /* 
       The following must be supplied by caller.
    */
    Uint1Ptr query,		/* The query sequence. */
        subject;	/* The subject sequence. */
    Int4	query_length,	/* the length of the query. */
        subject_length,	/* The subject length. */
        q_start,	/* query letter to start the gapped align. */
        s_start,	/* subject letter to start the gapped align.*/ 
        include_query,	/* length of query (starting from q_start) that 
                           MUST be included in an alignment. */
        gap_open,	/* Cost to open a gap. */
        gap_extend,	/* cost to extend a gap. */
        decline_align,  /* decline to align penalty */
        x_parameter,	/* values of X-dropoff parameter. */	
        shift_pen;      /* penalty for the frame shift */
    Int4Ptr PNTR matrix;	/* Matrix for the alignment. */
    Int4Ptr PNTR posMatrix;	/* Matrix for position-based searches. */
    Boolean translate1,	/* are either of these be translated. */
        translate2;

    /*
      The state, state_column_length, and state_row_length are used by ALIGN.
      If state is NULL, then ALIGN allocates a memory block and frees it before
      returning (for "call and forget" applications).
    */
    GapXDropStateArrayStructPtr state_struct;
    /* 
       The score, start, and stop of alignments are filled in by the
       functions PerformGappedAlignment and PerformGappedAlignmentWithTraceback
    */
    Int4	score, 		/* score of alignment. */
        query_start,	/* start of alignment on query. */
        query_stop,	/* end of alignment on query. */
        subject_start,	/* start of alignment on subject. */
        subject_stop;	/* end of alignment on subject. */
    Int2	query_frame,	/* Frame of the query (0 is no-frame) */
        subject_frame;	/* Frame of the subject (0 is no-frame). */
    /* 
       GapXEditBlockPtr filled in by PerformGappedAlignmentWithTraceback, used
       to make a SeqAlignPtr. 
    */
    GapXEditBlockPtr edit_block;
    /* Another TLM kludge for display. */
    Int4Ptr tback;
    /*	Is the search position based. */
    Boolean positionBased;
    Boolean posConverged;
    Boolean is_ooframe;
    Boolean discontinuous;
    GapXDPPtr dyn_prog;


#if BLAST_ALTIVEC
    void*	dyn_prog_save_PTR;
    Uint4	dyn_prog_save_SIZE;
#endif    	


} GapAlignBlk, PNTR GapAlignBlkPtr;

GapXDropStateArrayStructPtr GapXDropStateDestroy PROTO((GapXDropStateArrayStructPtr state_struct));


SeqAlignPtr LIBCALL GapXEditBlockToSeqAlign PROTO((GapXEditBlockPtr edit_block, SeqIdPtr subject_id, SeqIdPtr query_id));
SeqAlignPtr LIBCALL OOFGapXEditBlockToSeqAlign PROTO((GapXEditBlockPtr edit_block, SeqIdPtr subject_id, SeqIdPtr query_id, Int4 query_length));

GapXEditBlockPtr LIBCALL SimpleIntervalToGapXEditBlock PROTO((Int4 start1, Int4 start2, Int4 length));

GapXEditBlockPtr LIBCALL GapXEditBlockNew PROTO((Int4 start1, Int4 start2));
GapXEditBlockPtr LIBCALL GapXEditBlockDelete PROTO((GapXEditBlockPtr edit_block));
GapXEditScriptPtr LIBCALL GapXEditScriptDelete PROTO((GapXEditScriptPtr esp));

Boolean LIBCALL PerformNtGappedAlignment PROTO((GapAlignBlkPtr gap_align));

Boolean LIBCALL PerformGappedAlignment PROTO((GapAlignBlkPtr));

Boolean LIBCALL PerformGappedAlignmentWithTraceback PROTO((GapAlignBlkPtr));

GapXEditBlockPtr LIBCALL TracebackToGapXEditBlock PROTO((Uint1Ptr A, Uint1Ptr B, Int4 M, Int4 N, Int4Ptr S, Int4 start1, Int4 start2));
GapXEditBlockPtr LIBCALL
OOFTracebackToGapXEditBlock PROTO((Int4 M, Int4 N, Int4Ptr S, Int4 start1, Int4 start2));

/* 
        Allocates GapAlignBlkPtr and "state", if state_column_length and 
        state_row_length are not NULL. 

        For "call and forget" applications, state_column_length and
        state_row_length should both be set to zero.  ALIGN will
        then allocate and deallocate this memory.
*/
GapAlignBlkPtr LIBCALL GapAlignBlkNew PROTO((Int4 state_column_length, Int4 state_row_length));

/*
        Destruct Function for GapAlignBlk.  If "state" is not NULL, then
        it's deallocated.
*/
GapAlignBlkPtr LIBCALL GapAlignBlkDelete PROTO((GapAlignBlkPtr gap_align));


Int4 SEMI_G_ALIGN PROTO((Uint1Ptr A, Uint1Ptr B, Int4 M, Int4 N,
                Int4Ptr S, Int4Ptr pei, Int4Ptr pej,
                Boolean score_only, Int4Ptr PNTR sapp, GapAlignBlkPtr gap_align,
                Int4 query_offset, Boolean reversed));

Int4 LIBCALL ALIGN PROTO((Uint1Ptr A, Uint1Ptr B, Int4 M, Int4 N,
                Int4Ptr S, Int4Ptr pei, Int4Ptr pej, Int4Ptr PNTR sapp,
                GapAlignBlkPtr gap_align, Int4 query_offset, Boolean reversed));

GapXEditScriptPtr 
GapXEditScriptNew PROTO((GapXEditScriptPtr old));

Boolean GXECollectDataForSeqalign PROTO((GapXEditBlockPtr edit_block, 
                                  GapXEditScriptPtr curr_in, Int4 numseg,
                                  Int4Ptr PNTR start_out, 
                                  Int4Ptr PNTR length_out,
                                  Uint1Ptr PNTR strands_out,
                                  Int4Ptr start1, Int4Ptr start2));
#ifdef __cplusplus
}
#endif
#endif /* !__GAPXDROP__ */

