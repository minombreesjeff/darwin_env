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

File name: vecscrn.h

Author: Tom Madden

Contents: functions for Vector screening.

******************************************************************************
 * $Revision: 6.130 $
 *
 * $Log: vecscrn.h,v $
 * Revision 6.130  2000/07/08 20:44:12  vakatov
 * Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
 *
 * Revision 6.129  2000/05/09 21:36:40  kitts
 * Removed unused parameter from VSPrintListFromSeqLocs
 *
 * Revision 6.128  2000/05/05 20:11:09  madden
 * Add VSScreenSequenceByLoc
 *
 * Revision 6.127  2000/05/01 16:58:43  kitts
 *
 * Added function VSPrintListFromSeqLocs
 * Added function VSPrintListIdLine
 *
 * Revision 6.126  2000/03/30 21:04:16  madden
 * Added function VSMakeCombinedSeqLoc
 *
 * Revision 6.125  2000/01/27 14:24:17  egorov
 * Fix C++ compilation problem
 *
 * Revision 6.124  2000/01/20 18:32:36  madden
 * Removed VSMakeDisplaySeqLoc
 *
 * Revision 6.123  2000/01/20 17:06:36  madden
 * Added missing LIBCALL
 *
 * Revision 6.122  2000/01/20 14:40:36  madden
 * Added VSMakeDisplaySeqLoc
 *
 * Revision 6.121  2000/01/19 21:53:21  madden
 * Files for vector screening
 *
******************************************************************************/

#ifndef __VECSCRN__
#define __VECSCRN__

#include <ncbi.h>
#include <blastdef.h>
#include <blastpri.h>
#include <blast.h>

#ifdef __cplusplus
extern "C" {
#endif


/**********************************************************************
	Structure for VecScreen Search options.  
**********************************************************************/

typedef struct _vs_optionblk {
	Int2 number_of_classes; /* How many categories are there? */
	/* min score for each class w/ and w/o being within 'distance_to_end'. */
	/* score for class 1 if within 'distance_to_end' is in class[0]. */
	/* score for class 1 if not within 'distance_to_end' is in class[1]. */
	Int4Ptr classes; 
	Int4 distance_to_end; /* Distance to end to raise class score. */
	Int4 distance_between_segments; /* Distance between segments to make them suspect. */
	Boolean filter_by_coverage;
} VSOptions, *VSOptionsPtr;

/*****************************************************
		FUNCTION PROTOTYPES

Note: most uses of VecScreen will only require that the
programmer call either VSScreenSequence (stand-alone)
or VSNetScreenSequence (network aware).  This is the
recommended procedure.

*****************************************************/

/*
        Gets the labels for a given class.
        The CharPtr's will be allocated by the function.
        It is up to the caller to deallocate this memory.
        The 'color' buffer will hold the color in the format of RRGGBB (e.g., "00FF00").
*/

Boolean LIBCALL VSGetLabels PROTO((Int2 Class, CharPtr PNTR short_desc, CharPtr PNTR long_desc, CharPtr PNTR color));

/*
        BlastOptionNew function used for VecScreen screening.
        These options were specified by Paul Kitts.
*/
BLAST_OptionsBlkPtr LIBCALL VSBlastOptionNew PROTO((void));

/*
        Deletes the VSOptions structure.
*/
VSOptionsPtr LIBCALL VSOptionsFree PROTO((VSOptionsPtr options));

/*
        Makes up the structure for VSOptions, with default values
        filled in.
*/
VSOptionsPtr LIBCALL VSOptionsNew PROTO((void));

/*
        For a give SeqAlignPtr, make and merge SeqLoc's corresponding to these.

	This function calls VSMakeSeqLoc and VSCombineSeqLoc.

        Note: if 'options' is NULL, default values will be used.  This is STRONGLY recommended.
*/

Int2 LIBCALL
VSMakeCombinedSeqLoc PROTO((SeqAlignPtr PNTR seqalign_ptr, ValNodePtr PNTR vnpp, Int4 length, VSOptionsPtr options));


/*
        Scans the SeqAlignPtr's, producing SeqLocPtr's as specified.
        Each ValNodePtr points to a list of SeqLoc's.

        This function:

        - categorizes hits
        - eliminates redundancy
        - calculates SeqLocs from the SeqAlign

*/
Boolean LIBCALL VSMakeSeqLoc  PROTO((SeqAlignPtr PNTR seqalign_ptr, ValNodePtr PNTR vnp, Int4 length, VSOptionsPtr options));

/*
        This function checks SeqLocs for redundancy.  It:

        - Combines SeqLoc's in each category
        - checks for overlap
        - produces a fourth SeqLoc for intervening sequences.
*/
Boolean LIBCALL VSCombineSeqLoc PROTO((ValNodePtr PNTR vnp, Int4 query_length, VSOptionsPtr options));

/*
Performs VecScreen for stand-alone application.
The SeqLoc's for the different categories of vector hits are on the ValNodePtr.
For category 1 (strongest hit) they are on the ValNode with ValNode.choice = 1
and the SeqLocPtr is of type SEQLOC_PACKED_INT on ValNode.data.ptrvalue.
Category 2 has choice = 2 etc.

Note: 	If 'options' is NULL, default values will be used.  This is STRONGLY recommended.
	If 'database' is NULL, then the default database ('UniVec_Core') will be used. 

Return values:	positive number: class of hit found, 1 is strongest, 2 is moderate, 3 is weak.
		 0 - no hits were found, no errors.
		-1 - an error occurred.

other_returns and error_returns are returned from the BLAST engine.  They may be left NULL if
the information are not desired.
*/
Int2 LIBCALL VSScreenSequence PROTO((BioseqPtr bsp, VSOptionsPtr options, CharPtr database, SeqAlignPtr PNTR seqalign_ptr, ValNodePtr PNTR vnpp, ValNodePtr *other_returns, ValNodePtr *error_returns));

Int2 LIBCALL VSScreenSequenceByLoc PROTO((SeqLocPtr slp, VSOptionsPtr options, CharPtr database, SeqAlignPtr PNTR seqalign_ptr, ValNodePtr PNTR vnpp, ValNodePtr *other_returns, ValNodePtr *error_returns));


/*
Performs VecScreen for network application.

Note: 	If 'options' is NULL, default values will be used.  This is STRONGLY recommended.
	If 'database' is NULL, then the default database ('UniVec_Core') will be used. 

Return values:	positive number: class of hit found, 1 is strongest, 2 is moderate, 3 is weak.
		 0 - no hits were found, no errors.
		-1 - an error occurred.

other_returns and error_returns are returned from the BLAST engine.  They may be left NULL if
the information are not desired.
*/
Int2 VSNetScreenSequence PROTO((BioseqPtr bsp, VSOptionsPtr options, CharPtr database, SeqAlignPtr PNTR seqalign_ptr, ValNodePtr PNTR vnpp, ValNodePtr *other_returns, ValNodePtr *error_returns));


/* 
	Prints bar overview and list of matching segments by category
*/
Boolean LIBCALL VSPrintOverviewFromSeqLocs PROTO((ValNodePtr vnp, Int4 query_length, FILE *outfp));


/* 
	Prints list of matching segments
*/
Boolean LIBCALL VSPrintListFromSeqLocs PROTO((ValNodePtr vnp, FILE *outfp));


/* 
	Prints Id line for list results format
*/
Boolean LIBCALL VSPrintListIdLine PROTO((BioseqPtr bsp, CharPtr proginfo, CharPtr database, FILE *outfp));

#ifdef __cplusplus
}
#endif
#endif /* !__VECSCRN__ */

