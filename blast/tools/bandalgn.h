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

File name: bandalgn.h

Author: Gennadiy Savchuk, Jinqhui Zhang, Tom Madden

Contents: prototypes to perform a global gapped alignment on two sequences.

****************************************************************************/
/* $Revision: 6.7 $ 
* $Log: bandalgn.h,v $
* Revision 6.7  2000/07/26 17:26:25  lewisg
* fix code for c++ inclusion
*
* Revision 6.6  2000/02/10 22:47:07  vakatov
* DLL'zation for MSVC on PC, Win-NT
*
* Revision 6.5  1999/03/17 16:49:10  madden
* Removed comment within comment
*
* Revision 6.4  1998/01/16 21:29:45  chappey
* Remove function CC_GetExtremes and use now SeqAlignStart, SeqAlignStop in salsap.c
*
 * Revision 6.3  1997/10/22 14:41:39  chappey
 * added CC_GetAlignExtremes, ChangeGlobalBandMatrix, CC_ExtendSeqAlign
 *
 * Revision 6.2  1997/10/02 16:18:20  tatiana
 * *** empty log message ***
 *
 * Revision 6.1  1997/10/02 15:17:15  tatiana
 * global align utility added
 *
 * Revision 6.0  1997/08/25 18:52:15  madden
 * Revision changed to 6.0
 *
 * Revision 1.3  1997/06/23 16:16:13  tatiana
 * GlobalBandAlign struct changed to use SeqLocs instead of SeqIds
 *
 * Revision 1.2  1997/03/05  17:31:21  savchuk
 * definition of data_t has been moved from bandalgn.c
 *
 * Revision 1.1  1997/01/22  14:11:05  madden
 * Initial revision
 *
*/

#ifndef _G_BAND_H_
#define _G_BAND_H_

#include <ncbi.h>
#include <gapxdrop.h>
#include <blastkar.h>
#include <seqport.h>
#include <blast.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define MININT -999999
#define MAXINT 9999999
#define SMBAND 400
/* Should this be removed. */
#define BND_DIGIT ((FloatHi)1.0)

#define PSU_MATRIX_SIZE	128

#ifdef notdef
enum {v,h};
#endif

typedef struct DP {
  Int4 CC, DD, CP, DPDP, FF, FP;
} PNTR dp_ptr, dp_node;

typedef struct {
  dp_ptr CD;
  Int4 IP;
  Int4Ptr MP[4];                /* save crossing points */
				/* 0: rep, 1: del, 2: ins */
  Uint1Ptr MT[4];
  Int4Ptr FP;                   /* forward dividing points */
  Uint1Ptr FT;
  Int4Ptr PNTR w;         /* w = W */

  Int4 g, zzh, m, rr;          /* g = G, zzh = H, m = g+zzh */

  Int4 leggA, leggB, reggA, reggB, leghA, leghB, reghA, reghB;
  Int4Ptr sapp, sapp0;          /* Current script append ptr */
  Int4  last;                   /* Last script op appended */
  Int1Ptr PNTR state;
} data_t;

#define gap(k) ((k) <= 0 ? 0 : (g + zzh * (k))) /* k-symbol indel cost */

/* k-symbol indel cost */
#define _gap(k) ((k) <= 0 ? 0 : (data->g + data->zzh * (k)))

/* k-symbol indel cost */
#define gap_(k) ((k) <= 0 ? 0 : (data.g + data.zzh * (k)))

/* Append "Delete k" op . Not used*/
#define _BND_DEL(k) \
last = (last < 0) ? (sapp[-1] -= (k)) : (*sapp++ = -(k));

/* Append "Delete k" op */
#define _DEL(k) \
data->last = (data->last < 0) ? (data->sapp[-1] -= (k)) : (*data->sapp++ = -(k));

/* Append "Delete k" op */
#define DEL_(k) \
data.last = (data.last < 0) ? (data.sapp[-1] -= (k)) : (*data.sapp++ = -(k));

/* Append "Insert k" op  Not used*/
#define BND_INS(k) \
last = (last > 0) ? (sapp[-1] += (k)) : (*sapp++ = (k));

/* Append "Insert k" op */
#define _INS(k) \
data->last = (data->last > 0) ? (data->sapp[-1] += (k)) : (*data->sapp++ = (k));

/* Append "Insert k" op */
#define INS_(k) \
data.last = (data.last > 0) ? (data.sapp[-1] += (k)) : (*data.sapp++ = (k));

/* Append "Replace" op */
#define REP \
{last = *sapp++ = 0;}

/* Append "Replace" op */
#define _REP \
{data->last = *data->sapp++ = 0;}

/* Append "Replace" op */
#define REP_ \
{data.last = *data.sapp++ = 0;}

#define REPP \
{*sapp++ = MININT; last = 0;}

#define _REPP \
{*data->sapp++ = MININT; data->last = 0;}

#define REPP_ \
{*data.sapp++ = MININT; data.last = 0;}

/***********************************************************
*
*	PSUGapOptions are used for the serise banded alignment 
*	(global and local) with various end gap penalty. This 
*	option works for DNA-DNA, protein-protein and DNA-protein 
*	alignment. gshift is the penalty for frame shift, which 
*	only works for DNA-protein
*	matrix was set only for DNA-protein and protein-
*	protein alignment
*
***********************************************************/ 
typedef struct psu_gapped_options {
  Int4 gopen;         /* gap open */
  Int4 gext;          /* gap extend penalties */
  Int4 gshift;        /* frame-shift penalty, only applies to DNA-protein alignment */
  /* low and up are used to calculate start_diag and width, which are calculated
     differently in local and global alignments. 
  */
  Int4 low, up;
  Int4 start_diag;    /* start diagonal of band */
  Int4 width;         /* width for band alignment */
  Int4 lg1_ext;	      /*the left end gap extension penalty for the first sequence */
  Int4 rg1_ext;	      /*the right end gap ext. penalty for the first sequence */
  Int4 lg2_ext;	      /*the left end gap extension penalty for the second sequence */
  Int4 rg2_ext;	      /*the right end gap ext. penalty for the second sequence */
  Int4 lg1_open;
  Int4 lg2_open;
  Int4 rg1_open;
  Int4 rg2_open;
} PSUGapOptions, PNTR PSUGapOptionsPtr;

/*
	Functions to create and delte the PSUGapOptions, as well as
	set default values.
*/
NLM_EXTERN PSUGapOptionsPtr PSUGapOptionsDelete(PSUGapOptionsPtr options);
NLM_EXTERN PSUGapOptionsPtr PSUGapOptionsCreate(Uint1 search_type);



/* Search choices for global banded alignments are (global search type): */
#define G_BAND_LINEAR		0  /*global banded alignemnt in linear space*/
#define G_BAND_QUADRATIC	1  /*global banded alignment in quadratic space*/
#define G_BAND_LGAP		2  /*global banded alignment in linear space with 
				    options for the four end gap penalties*/
#define G_BAND_QGAP		3  /*global banded alignment in quadratic space, 
				    with options for the four end gap penalties */
#define G_BAND_L3GAP		4  /*global banded  alignment in linear space, 
				     with THREE gap penalties and options for 
				     setting end gap penalties. Not sure if it 
				     works ?*/
#define G_BAND_Q3GAP		5   /*same as 4 except it runs in quadratic space 
				     it WORKS! */

/* Search choices for local banded alignments are (local search type): */
#define L_BAND_LINEAR		10  /*local banded alignemnt in linear space*/
#define L_BAND_QUADRATIC	11  /*local banded alignment in quadratic space*/
#define L_BAND_LGAP			12  /*local banded alignment in linear space with 
				    options for the four end gap penalties*/
#define L_BAND_QGAP			13  /*local banded alignment in quadratic space, 
				    with options for the four end gap penalties */
#define L_BAND_L3GAP		14  /*local banded  alignment in linear space, 
				     with THREE gap penalties and options for 
				     setting end gap penalties. Not sure if it 
				     works ?*/
#define L_BAND_Q3GAP		15   /*same as 4 except it runs in quadratic space 
				     it WORKS! */
					  

/*************************************************************************
*
*	The structure that is passed in during the call to Nlm_GlobalBand
*
*****************************************************************************/
typedef struct global_band_struct {
	/* The two sequences to be aligned. */
	Uint1Ptr	seq1,
			seq2;
	Int4		seq1_length,	/* length of sequence 1. */
			seq2_length;	/* length of sequence 2. */
	/* used to identify sequence in GlobalBandToSeqAlign if filled in. */
	SeqLocPtr	seqloc1,		/* SeqLoc for the first sequence. */
			seqloc2;		/* SeqLoc for the second. */

	Uint1		search_type; /* as in global search_type above */

  	Int4Ptr PNTR matrix;  /* scoring matrix, provided by and deleted by caller */
	PSUGapOptionsPtr	options;	/* parameters for search. */
	/* GapXEditBlockPtr filled in by TracebackToGapXEditBlock */
	/* A SeqAlign can be made from this. */
        GapXEditBlockPtr edit_block;
	Int4 score;		/* score of the alignment */
	Int4 alignment_length;	/* length of the alignment. */
} GlobalBandStruct, PNTR GlobalBandStructPtr;

/*
        Deletes the GlobalBandStruct, including the options.
        Does not delete the sequence matrix, or the ID's.
*/
NLM_EXTERN GlobalBandStructPtr GlobalBandStructDelete(GlobalBandStructPtr gbsp);

/*
	Creates the GlobalBandStructPtr, needed to run GlobalBandToEditScript,
	with the default values.
*/
NLM_EXTERN GlobalBandStructPtr GlobalBandStructCreate(Uint1 search_type);

/*
	Performs a global alignment, producing a SeqAlign.
*/
NLM_EXTERN SeqAlignPtr GlobalBandToSeqAlign(GlobalBandStructPtr gbsp);

/*
	Performs a global alignment, producing an EditBlock, which
	can be made into a SeqAlign.
*/
NLM_EXTERN Boolean GlobalBandToEditBlock(GlobalBandStructPtr gbsp);


/*************************************************************************
*
*	The structure that is passed in during the call to Nlm_GlobalBand
*
*****************************************************************************/
typedef struct local_band_struct {
	/* The two sequences to be aligned. */
	Uint1Ptr	seq1, seq2;
	Int4		seq1_length,	/* length of sequence 1. */
				seq2_length;	/* length of sequence 2. */
	/* used to identify sequence in GlobalBandToSeqAlign if filled in. */
	SeqIdPtr	seqloc1,		/* SeqLoc for the first sequence. */
				seqloc2;		/* SeqLoc for the second. */

	Uint1		search_type; /* as in local search_type above */

  	Int4Ptr PNTR matrix;  /* scoring matrix provided by and deleted by caller */
	PSUGapOptionsPtr	options;	/* parameters for search. */
	/* GapXEditBlockPtr filled in by TracebackToGapXEditBlock */
	/* A SeqAlign can be made from this. */
        GapXEditBlockPtr edit_block;
	Int4		score;		/* score of the alignment */
	Int4		seq1_start,	/* start of sequence one's alignment. */
				seq2_start,	/* start of sequence two's alignment. */
				seq1_end,	/* end of sequence one's alignment. */
				seq2_end;	/* end of sequence two's alignment. */
} LocalBandStruct, PNTR LocalBandStructPtr;


/*
        Deletes the LocalBandStruct, including the options.
        Does not delete the sequence matrix, or the ID's.
*/
NLM_EXTERN LocalBandStructPtr LocalBandStructDelete(LocalBandStructPtr gbsp);

/*
	Creates the LocalBandStructPtr, needed to run LocalBandToEditScript,
	with the default values.
*/
NLM_EXTERN LocalBandStructPtr LocalBandStructCreate(Uint1 search_type);

/*
	Performs a global alignment, producing a SeqAlign.
*/
NLM_EXTERN SeqAlignPtr LocalBandToSeqAlign(LocalBandStructPtr lbsp);

/*
	Performs a global alignment, producing an EditBlock, which
	can be made into a SeqAlign.
*/
NLM_EXTERN Boolean LocalBandToEditBlock(LocalBandStructPtr lbsp);


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

extern Int4 LIBCALL gband_linear(Uint1Ptr Seq1, Uint1Ptr Seq2,
				 Int4 M, Int4 N,
				 Int4Ptr PNTR matrix,
				 PSUGapOptionsPtr option,
				 Int4Ptr S, Int4Ptr Slen);

extern Int4 LIBCALL gband_quadratic(Uint1Ptr Seq1, Uint1Ptr Seq2,
				    Int4 M, Int4 N,
				    Int4Ptr PNTR matrix,
				    PSUGapOptionsPtr option,
				    Int4Ptr S, Int4Ptr Slen);

extern Int4 LIBCALL gband_linear_gap(Uint1Ptr Seq1, Uint1Ptr Seq2,
				     Int4 M, Int4 N, 
				     Int4Ptr PNTR matrix,
				     PSUGapOptionsPtr option,
				     Int4Ptr S, Int4Ptr Slen);

extern Int4 LIBCALL gband_linear_qgap(Uint1Ptr Seq1, Uint1Ptr Seq2,
				      Int4 M, Int4 N,
				      Int4Ptr PNTR matrix,
				      PSUGapOptionsPtr option,
				      Int4Ptr S, Int4Ptr Slen);

extern Int4 LIBCALL gband_l3gap(Uint1Ptr Seq1, Uint1Ptr Seq2,
				Int4 M, Int4 N,
				Int4Ptr PNTR matrix,
				PSUGapOptionsPtr option,
				Int4Ptr S, Int4Ptr Slen);

extern Int4 LIBCALL gband_q3gap(Uint1Ptr Seq1, Uint1Ptr Seq2,
				Int4 M, Int4 N,
				Int4Ptr PNTR matrix,
				PSUGapOptionsPtr option,
				Int4Ptr S, Int4Ptr Slen);

extern Int4 BAND_LOCAL_ALIGN(Uint1Ptr A, Uint1Ptr B,
				     Int4 M, Int4 N, 
				     Int4Ptr PNTR matrix,
				     PSUGapOptionsPtr options,
				     Int4Ptr S,
				     Int4Ptr psi, Int4Ptr psj,
				     Int4Ptr pei, Int4Ptr pej,
				     Int4 align_type);


/**********************************************************************
*		Global Alignment utility functions
**********************************************************************/

NLM_EXTERN void SetGlobaltOptions(GlobalBandStructPtr gbsp, Int4 lg1_ext, Int4 rg1_ext, Int4 lg2_ext, Int4 rg2_ext, Int4 lg1_open, Int4 lg2_open, Int4 rg1_open, Int4 rg2_open, Int2 gopen, Int2 gext);

NLM_EXTERN GlobalBandStructPtr CreatBandStruct(SeqLocPtr slp1, SeqLocPtr slp2, Int4Ptr PNTR W, Boolean is_prot, Int2 method);
		
NLM_EXTERN void SetLowUpFromBlast(PSUGapOptionsPtr opt, Boolean is_prot, Int2 type, Int2 width, SeqLocPtr slp1, SeqLocPtr slp2);

NLM_EXTERN SeqAlignPtr GlobalBandByLoc(GlobalBandStructPtr gbsp, SeqLocPtr slp1, SeqLocPtr slp2,  Boolean is_prot, Int2 band_method);

NLM_EXTERN SeqAlignPtr ExtendSeqAlign(SeqAlignPtr seqalign, Int4 start1, Int4 start2, Int4 stop1, Int4 stop2, Int4 x1, Int4 y1, Int4 x2, Int4 y2);

NLM_EXTERN SeqAlignPtr CC_ExtendSeqAlign(SeqAlignPtr sap, Int4 start1, Int4 start2, Int4 stop1, Int4 stop2, Int4 x1, Int4 y1, Int4 x2, Int4 y2, Uint1 strand1, Uint1 strand2);

NLM_EXTERN void GetAlignExtremes(SeqAlignPtr seqalign, Int4Ptr xx1, Int4Ptr yy1, Int4Ptr xx2, Int4Ptr yy2);

NLM_EXTERN Int2 ChangeGlobalBandMatrix(GlobalBandStructPtr gbsp, Boolean is_prot, CharPtr matrix_name, Int4 penalty, Int4 reward);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
