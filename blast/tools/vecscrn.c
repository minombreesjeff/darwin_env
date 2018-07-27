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

File name: vecscrn.c

Author: Tom Madden

Contents: functions for Vector screening.

******************************************************************************
 * $Revision: 6.132 $
 *
 * $Log: vecscrn.c,v $
 * Revision 6.132  2001/01/09 17:30:03  madden
 * Fix memory leaks
 *
 * Revision 6.131  2000/09/01 18:29:13  dondosha
 * Removed calls to ReadDBFreeSharedInfo and ReadDBCloseMHdrAndSeqFiles
 *
 * Revision 6.130  2000/08/07 20:38:11  madden
 * Proper casting of int to long for printf
 *
 * Revision 6.129  2000/05/19 20:40:34  kitts
 * 1. Fixed bug in VSMakeCombinedSeqLoc preventing "No hits" being
 *    reported when none of the blast hits were significant.
 * 2. Fixed bug in VSCombineSeqLoc causing overlaps to be missed
 *    when alignment filtering is off.
 * 3. Fixed bug in CombineSeqLoc to allow all SeqLocs in a linked
 *    list to be processed.
 * 4. Added checks to several functions to prevent "No hits"
 *    being reported when the search fails.
 * 5. Other minor changes.
 *
 * Revision 6.128  2000/05/09 21:43:48  kitts
 * Removed unused parameter from VSPrintListFromSeqLocs
 * Moved "No hits" output from VSPrintListFromSeqLocs to vecscreen.c
 *
 * Revision 6.127  2000/05/05 20:11:09  madden
 * Add VSScreenSequenceByLoc
 *
 * Revision 6.126  2000/05/01 16:58:42  kitts
 *
 * Added function VSPrintListFromSeqLocs
 * Added function VSPrintListIdLine
 *
 * Revision 6.125  2000/03/30 21:04:16  madden
 * Added function VSMakeCombinedSeqLoc
 *
 * Revision 6.124  2000/01/21 16:47:49  kans
 * use ValNodeSort instead of original jz SortValNode
 *
 * Revision 6.123  2000/01/20 18:32:36  madden
 * Removed VSMakeDisplaySeqLoc
 *
 * Revision 6.122  2000/01/20 14:40:36  madden
 * Added VSMakeDisplaySeqLoc
 *
 * Revision 6.121  2000/01/19 21:53:21  madden
 * Files for vector screening
 *
******************************************************************************/

#include <vecscrn.h>
#include <salpacc.h>
#include <salpstat.h>
#include <urkbias.h>

/* Used for the '62' notation stuff. */
#define DEFINE_62 62

/*********************************************************** 

CoordTo62Notation converts a position into the '62' notation.
The '62' notation decomposes a number (X) into two parts:

1.) the largest multiple of 62 that is still less than X. Let
this number be A.

2.) the difference between the number found in 1.) and X.  Let 
this number be B.

The original number can be reconstructed from:

X = 62*A + B

In CoordTo62Notation the first Int4 is the position that should be decomposed.
The two CharPtr's are (respectively) for A and B.  These CharPtr's should be
allocated with at least two spaces and will be filled in with A and B.

****************************************************************/
static Boolean
CoordTo62Notation (Int4 position, CharPtr major_part, CharPtr minor_part)

{
	Int4 major_num, minor_num;

	if (major_part == NULL || minor_part == NULL)
		return FALSE;

	major_num = position/DEFINE_62;
	minor_num = position%DEFINE_62;


	if (major_num < 10)
		sprintf(major_part, "%ld", (long) major_num);
	else if (major_num < 36)
		*major_part = 'A' + major_num - 10;
	else
		*major_part = 'a' + major_num - 36;
		

	if (minor_num < 10)
		sprintf(minor_part, "%ld", (long) minor_num);
	else if (minor_num < 36)
		*minor_part = 'A' + minor_num - 10;
	else
		*minor_part = 'a' + minor_num - 36;

	return TRUE;
}

#define ADD_TO_BUFFER_FOR_BLAST 100

/*
	Given a SeqLocPtr (for a query of length query_length), fill in
	buffer with starts and stops in '62' notation (see above doc.).
*/

static Boolean
Print62Notation(SeqLocPtr seqloc, Int4 query_length, CharPtr *buffer)
{
	Char a1[2], a2[2], b1[2], b2[2];
	CharPtr ptr, my_buffer, my_new_buffer;
	Int4 buffer_size, position, start, stop;
	Nlm_FloatHi block_size, screen_width;

	if (seqloc == NULL)
		return FALSE;


/*
	screen_width = (Nlm_FloatHi) MIN(600, query_length);
*/
	screen_width = 600.0;
	block_size = ((Nlm_FloatHi) query_length)/screen_width;
	
	if (seqloc->choice == SEQLOC_PACKED_INT)
		seqloc = (ValNodePtr) seqloc->data.ptrvalue;

	position = 0;
	buffer_size = ADD_TO_BUFFER_FOR_BLAST;
	my_buffer = (CharPtr) MemNew(buffer_size*sizeof(Char));
	ptr = my_buffer;
	while (seqloc)
	{
		if (position+4 > buffer_size)
		{
			buffer_size += ADD_TO_BUFFER_FOR_BLAST;
			my_new_buffer = (CharPtr) MemNew(buffer_size*sizeof(Char));
			MemCpy(my_new_buffer, my_buffer, position);
			my_buffer = (CharPtr) MemFree(my_buffer);
			my_buffer = my_new_buffer;
			ptr = my_buffer + position;
		}
		if (SeqLocStrand(seqloc) == Seq_strand_minus)
		{
			start = SeqLocStop(seqloc)/block_size;
			stop = (SeqLocStart(seqloc)+1)/block_size;
		}
		else
		{
			start = SeqLocStart(seqloc)/block_size;
			stop = (SeqLocStop(seqloc)+1)/block_size;
		}
		CoordTo62Notation(start, a1, b1);
		CoordTo62Notation(stop, a2, b2);
		*ptr = *a1; ptr++;
		*ptr = *b1; ptr++;
		*ptr = *a2; ptr++;
		*ptr = *b2; ptr++;
		position += 4;
		seqloc = seqloc->next;
	}
	*ptr = NULLB;

	*buffer = my_buffer;

	return TRUE;
}

/*
	Gets the labels for a given class.
	The CharPtr's will be allocated by the function.
	It is up to the caller to deallocate this memory.
	The 'color' buffer will hold the color in the format of RRGGBB (e.g., "00FF00").
*/

Boolean LIBCALL
VSGetLabels (Int2 class, CharPtr PNTR short_desc, CharPtr PNTR long_desc, CharPtr PNTR color)

{
	Boolean retval;

	switch (class) {
		case 1: 
			if (long_desc)
				*long_desc = StringSave("Strong match");
			if (short_desc)
				*short_desc = StringSave("Strong");
			if (color)
				*color = StringSave("FF0000");
			retval = TRUE;
			break;
		case 2:
			if (long_desc)
				*long_desc = StringSave("Moderate match");
			if (short_desc)
				*short_desc = StringSave("Moderate");
			if (color)
				*color = StringSave("FF00FF");
			retval = TRUE;
			break;
		case 3: 
			if (long_desc)
				*long_desc = StringSave("Weak match");
			if (short_desc)
				*short_desc = StringSave("Weak");
			if (color)
				*color = StringSave("00FF00");
			retval = TRUE;
			break;
		case 4: 
			if (long_desc)
				*long_desc = StringSave("Suspect origin");
			if (short_desc)
				*short_desc = StringSave("Suspect");
			if (color)
				*color = StringSave("FFFF00");
			retval = TRUE;
			break;
		default:
			retval = FALSE;
	}

	return retval;
}

/*
	BlastOptionNew function used for VecScreen screening.
	These options were specified by Paul Kitts.
*/


BLAST_OptionsBlkPtr LIBCALL 
VSBlastOptionNew(void)

{
	BLAST_OptionsBlkPtr options;


	options = BLASTOptionNew("blastn", TRUE);
	if (options == NULL)
		return NULL;

	options->gap_open = 3;
	options->gap_extend = 3;
	options->filter_string = StringSave("m D");
	options->penalty = -5;
	options->expect_value = 700;
	options->searchsp_eff = 1.75e12;

	return options;
}


/* 
	Deletes the VSOptions structure. 
*/

VSOptionsPtr LIBCALL
VSOptionsFree(VSOptionsPtr options)

{
	if (options == NULL)
		return NULL;

	options->classes = MemFree(options->classes);

	
	MemFree(options);

	return NULL;

}
/* 
	Makes up the structure for VSOptions, with default values
	filled in.
*/

VSOptionsPtr LIBCALL
VSOptionsNew(void)

{
	VSOptionsPtr options;

	options = MemNew(sizeof(VSOptions));
	if (options == NULL)
		return NULL;

	options->number_of_classes = 3;
	options->classes = MemNew(2*(options->number_of_classes)*sizeof(Int4));
	if (options->classes == NULL) 
	{
		MemFree(options);
		return NULL;
	}
	options->classes[0] = 24;
	options->classes[1] = 30;
	options->classes[2] = 19;
	options->classes[3] = 25;
	options->classes[4] = 16;
	options->classes[5] = 23;
	options->distance_to_end = 24;
	options->distance_between_segments = 50;
	options->filter_by_coverage  = TRUE;

	return options;
}

/*
	Changes the SeqLoc to the PLUS strand.  Only works for SeqInt.
*/

static void
SeqLocToPlusStrand(SeqLocPtr seqloc)

{
	Int4 start, stop;
	SeqIntPtr seq_int;

	if (seqloc == NULL || seqloc->choice != SEQLOC_INT)
		return;

	while (seqloc)
	{
		if (SeqLocStrand(seqloc) == Seq_strand_minus)
		{
			seq_int = seqloc->data.ptrvalue;

			start = seq_int->from;
			stop = seq_int->to;

			seq_int->from = MIN(start, stop);
			seq_int->to = MAX(start, stop);
			seq_int->strand = Seq_strand_plus;
		}
		seqloc = seqloc->next;
	}

	return;
}

/* 
Check for overlap between seqloc1 and seqloc2.  
Keep only seqloc1 if there is an overlap.
Return a new SeqLocPtr with these SeqLoc's.
*/
static SeqLocPtr 
CheckForOverlap(SeqLocPtr seqloc1, SeqLocPtr seqloc2)

{
	SeqLocPtr seqloc, next_seqloc;
	SeqLocPtr new_seqloc=NULL, retval=NULL, current;

	if (seqloc1 == NULL || seqloc1->choice != SEQLOC_PACKED_INT)
		return seqloc2;

	if (seqloc2 == NULL || seqloc2->choice != SEQLOC_PACKED_INT)
		return NULL;

	seqloc = seqloc2->data.ptrvalue;

	while (seqloc)
	{
		if (SeqLocAinB(seqloc, seqloc1) >= 0)
		{
			seqloc = seqloc->next;
		}
		else
		{
			if (!new_seqloc)
			{
				new_seqloc = seqloc;
				current = new_seqloc;
			}
			else 
			{
				current->next = seqloc;
				current = current->next;
			}
			next_seqloc = seqloc->next;
			seqloc->next = NULL;
			seqloc = next_seqloc;
		}
	}
	if (new_seqloc)
		ValNodeAddPointer(&retval, SEQLOC_PACKED_INT, new_seqloc);

	return retval;
}

static int LIBCALLBACK MySeqLocSortByStartPosition(VoidPtr vp1, VoidPtr vp2)

{
	SeqLocPtr slp1, slp2;

	slp1 = *((SeqLocPtr PNTR) vp1);
	slp2 = *((SeqLocPtr PNTR) vp2);

	if (SeqLocStart(slp1) < SeqLocStart(slp2))
		return -1;
	else if (SeqLocStart(slp1) > SeqLocStart(slp2))
		return 1;
	else
		return 0;
}

/*
	Combine two SeqLoc's that overlap.
*/

static SeqLocPtr
CombineSeqLoc (SeqLocPtr seqloc)

{
	Int4 start, stop;
	SeqLocPtr retval=NULL, seqloc_var, new_seqloc=NULL;
	SeqLocPtr my_seqloc=NULL, tmp_seqloc;
	SeqIntPtr seq_int;

	if (seqloc == NULL || seqloc->choice != SEQLOC_PACKED_INT)
		return NULL;

	while (seqloc)
	{
		tmp_seqloc = seqloc->data.ptrvalue;
		while (tmp_seqloc)
		{
			ValNodeLink(&my_seqloc, (SeqLocPtr) AsnIoMemCopy ((Pointer) tmp_seqloc,
                                     (AsnReadFunc) SeqLocAsnRead,
                                     (AsnWriteFunc) SeqLocAsnWrite));
			tmp_seqloc = tmp_seqloc->next;
		}
		seqloc = seqloc->next;
	}

	my_seqloc = (SeqLocPtr) ValNodeSort ((ValNodePtr) my_seqloc, MySeqLocSortByStartPosition);

	start = SeqLocStart(my_seqloc);
	stop = SeqLocStop(my_seqloc);
	seqloc_var = my_seqloc;

	while (seqloc_var)
	{
		if (seqloc_var->next && stop+1 >= SeqLocStart(seqloc_var->next))
		{
			stop = MAX(stop, SeqLocStop(seqloc_var->next));
		}
		else
		{
			seq_int = SeqIntNew();
			seq_int->from = start;
			seq_int->to = stop;
			seq_int->strand = Seq_strand_plus;
			seq_int->id = SeqIdDup(SeqIdFindBest(SeqLocId(seqloc_var), SEQID_GI));
                	ValNodeAddPointer(&new_seqloc, SEQLOC_INT, seq_int);
			if (seqloc_var->next)
			{
				start = SeqLocStart(seqloc_var->next);
				stop = SeqLocStop(seqloc_var->next);
			}
		}
		seqloc_var = seqloc_var->next;
	}

	if (new_seqloc)
		ValNodeAddPointer(&retval, SEQLOC_PACKED_INT, new_seqloc);

	return retval;
}

/* Get intervening SeqLoc's if the two SeqLoc's are within distance_permitted.  */

static SeqLocPtr 
GetInterveningSeqLoc (SeqLocPtr seqloc, Int4 distance_permitted, Int4 query_length)

{
	SeqLocPtr retval=NULL, seqloc_var, new_seqloc=NULL;
	SeqLocPtr my_seqloc=NULL, tmp_seqloc;
	SeqIntPtr seq_int;

	if (seqloc == NULL)
		return NULL;

	if (seqloc->choice != SEQLOC_PACKED_INT)
		return NULL;

	while (seqloc)
	{
		tmp_seqloc = seqloc->data.ptrvalue;
		while (tmp_seqloc)
		{
			ValNodeLink(&my_seqloc, (SeqLocPtr) AsnIoMemCopy ((Pointer) tmp_seqloc,
                                     (AsnReadFunc) SeqLocAsnRead,
                                     (AsnWriteFunc) SeqLocAsnWrite));
			tmp_seqloc = tmp_seqloc->next;
		}
		seqloc = seqloc->next;
	}

	my_seqloc = (SeqLocPtr) ValNodeSort ((ValNodePtr) my_seqloc, MySeqLocSortByStartPosition);

	seqloc_var = my_seqloc;

	if (SeqLocStart(seqloc_var) < distance_permitted &&
		SeqLocStart(seqloc_var) > 0)
	{
			seq_int = SeqIntNew();
			seq_int->from = 0;
			seq_int->to = SeqLocStart(seqloc_var) - 1;
			seq_int->strand = Seq_strand_plus;
			seq_int->id = SeqIdDup(SeqIdFindBest(SeqLocId(seqloc_var), SEQID_GI));
                	ValNodeAddPointer(&new_seqloc, SEQLOC_INT, seq_int);
	}

	while (seqloc_var)
	{
		if (seqloc_var->next && SeqLocStop(seqloc_var)+1 < SeqLocStart(seqloc_var->next) && SeqLocStop(seqloc_var)+distance_permitted >= SeqLocStart(seqloc_var->next))
		{
			seq_int = SeqIntNew();
			seq_int->from = SeqLocStop(seqloc_var) + 1;
			seq_int->to = SeqLocStart(seqloc_var->next) - 1;
			seq_int->strand = Seq_strand_plus;
			seq_int->id = SeqIdDup(SeqIdFindBest(SeqLocId(seqloc_var), SEQID_GI));
                	ValNodeAddPointer(&new_seqloc, SEQLOC_INT, seq_int);
		}
		else if ((seqloc_var->next == NULL || SeqLocStop(seqloc_var->next) < SeqLocStop(seqloc_var)) && query_length - (SeqLocStop(seqloc_var)+1) < distance_permitted && query_length - (SeqLocStop(seqloc_var)+1) > 0) /* Check distance to end. */
		{
			seq_int = SeqIntNew();
			seq_int->from = SeqLocStop(seqloc_var) + 1;
			seq_int->to = query_length - 1;
			seq_int->strand = Seq_strand_plus;
			seq_int->id = SeqIdDup(SeqIdFindBest(SeqLocId(seqloc_var), SEQID_GI));
                	ValNodeAddPointer(&new_seqloc, SEQLOC_INT, seq_int);
		}
		seqloc_var = seqloc_var->next;
	}

	if (new_seqloc)
		ValNodeAddPointer(&retval, SEQLOC_PACKED_INT, new_seqloc);

	return retval;
}

/*
	Scans the SeqAlignPtr's, producing SeqLocPtr's as specified.
	Each ValNodePtr points to a list of SeqLoc's.

	This function:

	- categorizes hits
	- eliminates redundancy
	- calculates SeqLocs from the SeqAlign

	if options are NULL, defaults will be used.

*/

Boolean LIBCALL
VSMakeSeqLoc (SeqAlignPtr PNTR seqalign_ptr, ValNodePtr PNTR vnp, Int4 length, VSOptionsPtr options)

{
	Boolean own_options=FALSE;
	Int4 start, stop, score, num, tmp;
	Nlm_FloatHi bit_score, evalue;
	SeqLocPtr seqloc1=NULL, seqloc2=NULL, seqloc3=NULL, seqloc, seqloc_tmp;
	SeqAlignPtr seqalign, head=NULL, next;
	SeqAlignPtr sap1=NULL, sap2=NULL, sap3=NULL;
	Uint2Ptr HitsPerBase=NULL;
	Int4 last_start=INT4_MAX, last_stop=-1;

	if (seqalign_ptr == NULL || vnp == NULL)
	        return FALSE;
	
	if (options == NULL)
	{
		options = VSOptionsNew();
		if (options == NULL)
		        return FALSE;
		own_options = TRUE;
	}

	seqalign = *seqalign_ptr;
	while (seqalign)
	{
		next = seqalign->next;
		GetScoreAndEvalue(seqalign, &score, &bit_score, &evalue, &num);
		start = SeqAlignStart(seqalign, 0);
		stop = SeqAlignStop(seqalign, 0);
		if (start > stop)
		{ /* reverse for minus strand. */
			tmp = start;
			start = stop;
			stop = tmp;
		}
		if (score >= options->classes[1] || (score >= options->classes[0] && (start <= options->distance_to_end || length-(stop+1) <= options->distance_to_end)))
		{
			sap1 = SeqAlignLink(sap1, seqalign);
			seqalign->next = NULL;
		}
		else if (score >= options->classes[3] || (score >= options->classes[2] && (start <= options->distance_to_end || length-(stop+1) <= options->distance_to_end)))
		{
			sap2 =SeqAlignLink(sap2, seqalign);
			seqalign->next = NULL;
		}
		else if (score >= options->classes[5] || (score >= options->classes[4] && (start <= options->distance_to_end || length-(stop+1) <= options->distance_to_end)))
		{
			sap3 = SeqAlignLink(sap3, seqalign);
			seqalign->next = NULL;
		}
		else
		{
			SeqAlignFree(seqalign);
		}
		seqalign = next;
	}

	if (sap1)
	{
		if (options->filter_by_coverage)
			sap1 = SeqAlignFilterByCoverage(sap1, NULL, NULL, 1, &HitsPerBase, &last_start, &last_stop);
		seqalign = sap1;
		while (seqalign)
		{
			seqloc_tmp = SeqLocFromSeqAlign(seqalign, NULL);
			SeqLocToPlusStrand(seqloc_tmp);
			ValNodeLink(&seqloc1, seqloc_tmp);
			seqalign = seqalign->next;
		}
		head = SeqAlignLink(head, sap1);
	}
	if (sap2)
	{
		if (options->filter_by_coverage)
			sap2 = SeqAlignFilterByCoverage(sap2, NULL, NULL, 1, &HitsPerBase, &last_start, &last_stop);
		seqalign = sap2;
		while (seqalign)
		{
			seqloc_tmp = SeqLocFromSeqAlign(seqalign, NULL);
			SeqLocToPlusStrand(seqloc_tmp);
			ValNodeLink(&seqloc2, seqloc_tmp);
			seqalign = seqalign->next;
		}
		head = SeqAlignLink(head, sap2);
	}
	if (sap3)
	{
		if (options->filter_by_coverage)
			sap3 = SeqAlignFilterByCoverage(sap3, NULL, NULL, 1, &HitsPerBase, &last_start, &last_stop);
		seqalign = sap3;
		while (seqalign)
		{
			seqloc_tmp = SeqLocFromSeqAlign(seqalign, NULL);
			SeqLocToPlusStrand(seqloc_tmp);
			ValNodeLink(&seqloc3, seqloc_tmp);
			seqalign = seqalign->next;
		}
		head = SeqAlignLink(head, sap3);
	}

	/* Set for return. */
	*seqalign_ptr = head;

	HitsPerBase = MemFree(HitsPerBase);

	*vnp = NULL;

	if (seqloc1)
	{
		seqloc = NULL;
		ValNodeAddPointer(&seqloc, SEQLOC_PACKED_INT, seqloc1);	
		ValNodeAddPointer(vnp, 1, seqloc);	
	} 

	if (seqloc2)
	{
		seqloc = NULL;
		ValNodeAddPointer(&seqloc, SEQLOC_PACKED_INT, seqloc2);	
		ValNodeAddPointer(vnp, 2, seqloc);	
	}

	if (seqloc3)
	{
		seqloc = NULL;
		ValNodeAddPointer(&seqloc, SEQLOC_PACKED_INT, seqloc3);	
		ValNodeAddPointer(vnp, 3, seqloc);	
	}


	if (own_options)
		options = VSOptionsFree(options);
	
	return TRUE;
}

/*

	This function checks SeqLocs for redundancy.  It:

	- Combines SeqLoc's in each category
	- checks for overlap
	- produces a fourth SeqLoc for intervening sequences.
*/

Boolean LIBCALL
VSCombineSeqLoc(ValNodePtr PNTR vnp, Int4 query_length, VSOptionsPtr options)
{
	SeqLocPtr seqloc1=NULL, seqloc2=NULL, seqloc3=NULL, seqloc4=NULL;
	SeqLocPtr n_seqloc1=NULL, n_seqloc2=NULL, n_seqloc3=NULL;
	SeqLocPtr tmp_slp=NULL, combo_slp=NULL;
	ValNodePtr var;

	if (vnp == NULL || options == NULL)
		return FALSE;

	if (*vnp == NULL)
		return TRUE;

	var = *vnp;
	while (var)
	{
		if (var->choice == 1)
		{
			seqloc1 = var->data.ptrvalue;
		}
		else if (var->choice == 2)
		{
			seqloc2 = var->data.ptrvalue;
		}
		else if (var->choice == 3)
		{
			seqloc3 = var->data.ptrvalue;
		}
		var = var->next;
	}

	*vnp = ValNodeFree(*vnp);

	if (seqloc1)
	{
		n_seqloc1 = CombineSeqLoc(seqloc1);
		seqloc1 = SeqLocSetFree(seqloc1);
		ValNodeAddPointer(vnp, 1, n_seqloc1);	
	}

	if (seqloc2)
	{
		seqloc2 = CheckForOverlap(n_seqloc1, seqloc2);
		n_seqloc2 = CombineSeqLoc(seqloc2);
		seqloc2 = SeqLocSetFree(seqloc2);
		ValNodeAddPointer(vnp, 2, n_seqloc2);	
	}

	if (seqloc3)
	{
		tmp_slp = SeqLocDupAll(n_seqloc1);
		tmp_slp = SeqLocLink(&tmp_slp, SeqLocDupAll(n_seqloc2));
		combo_slp = CombineSeqLoc(tmp_slp);
		tmp_slp = SeqLocSetFree(tmp_slp);
		seqloc3 = CheckForOverlap(combo_slp, seqloc3);
		n_seqloc3 = CombineSeqLoc(seqloc3);
		seqloc3 = SeqLocSetFree(seqloc3);
		ValNodeAddPointer(vnp, 3, n_seqloc3);	
	}

	combo_slp = SeqLocSetFree(combo_slp);

	tmp_slp = NULL;
	tmp_slp = SeqLocDupAll(n_seqloc1);
	tmp_slp = SeqLocLink(&tmp_slp, SeqLocDupAll(n_seqloc2));
	tmp_slp = SeqLocLink(&tmp_slp, SeqLocDupAll(n_seqloc3));

	seqloc4 = GetInterveningSeqLoc(tmp_slp, options->distance_between_segments, query_length);
	ValNodeAddPointer(vnp, 4, seqloc4);	
/*
	if (seqloc4)
	{
		seqloc = NULL;
		ValNodeAddPointer(&seqloc, SEQLOC_PACKED_INT, seqloc4);	
	} 
*/


	tmp_slp = SeqLocSetFree(tmp_slp);


	return TRUE;
}

/*
	For a given SeqAlignPtr, make and merge SeqLoc's corresponding to these.

	This function calls VSMakeSeqLoc and VSCombineSeqLoc.

	Note: if 'options' is NULL, default values will be used.  This is STRONGLY recommended.
*/

Int2 LIBCALL
VSMakeCombinedSeqLoc(SeqAlignPtr PNTR seqalign_ptr, ValNodePtr PNTR vnpp, Int4 length, VSOptionsPtr options)
{
	Boolean delete_options=FALSE;
	Boolean not_failed;
	Int2 retval=0;
	ValNodePtr var;

	if (seqalign_ptr == NULL || vnpp == NULL)
		return -1;

	*vnpp = NULL;

	if (*seqalign_ptr == NULL)
		return 0;
	
	if (options == NULL)
	{
		options = VSOptionsNew();
		if (options == NULL)
		        return -1;
		delete_options = TRUE;
	}

	not_failed = VSMakeSeqLoc(seqalign_ptr, vnpp, length, options);
        if (not_failed)
	        not_failed = VSCombineSeqLoc(vnpp, length, options);

        if (not_failed)
	{
	        retval = *vnpp ? INT2_MAX : 0;
	        var = *vnpp;
	        while (var)
	        {
		        retval = MIN(retval, (Int2) var->choice);
		        var = var->next;
	        }
	}
	else retval = -1;

	if (delete_options)
		options = VSOptionsFree(options);

	return retval;
}

/* 
Performs VecScreen for stand-alone application.

Note: if 'options' is NULL, default values will be used.  This is STRONGLY recommended.
*/

Int2 LIBCALL
VSScreenSequence(BioseqPtr bsp, VSOptionsPtr options, CharPtr database, SeqAlignPtr PNTR seqalign_ptr, ValNodePtr PNTR vnpp, ValNodePtr *other_returns, ValNodePtr *error_returns)

{
	BLAST_OptionsBlkPtr blast_options;
	Boolean delete_options = FALSE;
	Int2 retval=0;
	SeqAlignPtr seqalign;

	if (bsp == NULL)
		return -1;

	if (seqalign_ptr)
		*seqalign_ptr = NULL;

	blast_options = VSBlastOptionNew();
	if (blast_options == NULL)
		return -1;

	if (options == NULL)
	{
		options = VSOptionsNew();
		if (options == NULL)
		        return -1;
		delete_options = TRUE;
	}

	if (database == NULL)
		seqalign = BioseqBlastEngine(bsp, "blastn", "UniVec_Core", blast_options, other_returns, error_returns, NULL); 
	else
		seqalign = BioseqBlastEngine(bsp, "blastn", database, blast_options, other_returns, error_returns, NULL); 
	if (seqalign)
	{
		retval = VSMakeCombinedSeqLoc(&seqalign, vnpp, bsp->length, options);

		if (seqalign_ptr)
			*seqalign_ptr = seqalign;
	}

	if (delete_options)
		options = VSOptionsFree(options);

	blast_options = BLASTOptionDelete(blast_options);

	return retval;
}

/* 
Performs VecScreen for vector filtering.

Note: if 'options' is NULL, default values will be used.  This is STRONGLY recommended.
*/

Int2 LIBCALL
VSScreenSequenceByLoc(SeqLocPtr slp, VSOptionsPtr options, CharPtr database, SeqAlignPtr PNTR seqalign_ptr, ValNodePtr PNTR vnpp, ValNodePtr *other_returns, ValNodePtr *error_returns)

{
	BioseqPtr bsp;
	Int2 retval=0;

	if (slp == NULL)
		return -1;


	bsp = BioseqLockById(SeqLocId(slp));
	if (bsp == NULL)
		return -1;

	retval = VSScreenSequence(bsp, options, database, seqalign_ptr, vnpp, other_returns, error_returns);

	BioseqUnlock(bsp);

	return retval;
}

/* 
	Prints bar overview and list of matching segments by category
*/

Boolean LIBCALL
VSPrintOverviewFromSeqLocs (ValNodePtr vnp, Int4 query_length, FILE *outfp)

{
	CharPtr buffer50=NULL, buffer25=NULL, buffer20=NULL, buffer15=NULL;
	SeqLocPtr seqloc1=NULL, seqloc2=NULL, seqloc3=NULL, seqloc4=NULL, tmp;
	ValNodePtr var=NULL;

        if (outfp == NULL || query_length < 1)
		return FALSE;

	var = vnp;
	while (var)
	{
		if (var->choice == 1)
		{
			seqloc1 = var->data.ptrvalue;
		}
		else if (var->choice == 2)
		{
			seqloc2 = var->data.ptrvalue;
		}
		else if (var->choice == 3)
		{
			seqloc3 = var->data.ptrvalue;
		}
		else if (var->choice == 4)
		{
			seqloc4 = var->data.ptrvalue;
		}
		var = var->next;
	}

	if (!seqloc1 && !seqloc2 && !seqloc3 && !seqloc4)
		return TRUE;

	if (seqloc1)
	{
		Print62Notation(seqloc1, query_length, &buffer50);
	}

	if (seqloc2)
	{
		Print62Notation(seqloc2, query_length, &buffer25);
	}

	if (seqloc3)
	{
		Print62Notation(seqloc3, query_length, &buffer20);
	}

	if (seqloc4)
	{
		Print62Notation(seqloc4, query_length, &buffer15);
	}


	fprintf(outfp, "<B>Distribution of Vector Matches on the Query Sequence</B>\n\n");
	fprintf(outfp, "<IMG hspace=1 SRC=http://www.ncbi.nlm.nih.gov/cgi-bin/Entrez/tunf?450x12-1(1)150(%ld)300(%ld)450(%ld)600(%ld)\n", (long) query_length/4, (long) query_length/2, (long) 3*query_length/4, (long) query_length); 
	fprintf(outfp, ">\n");
	fprintf(outfp, "<IMG hspace=1 SRC=http://www.ncbi.nlm.nih.gov/gorf/gun2.cgi?0M(009g)");
	if (buffer20)
		fprintf(outfp, "00FF00(%s)", buffer20);
	if (buffer25)
		fprintf(outfp, "FF00FF(%s)", buffer25);
	if (buffer50)
		fprintf(outfp, "FF0000(%s)", buffer50);
	if (buffer15)
		fprintf(outfp, "FFFF00(%s)", buffer15);

	MemFree(buffer20);
	MemFree(buffer25);
	MemFree(buffer50);
	MemFree(buffer15);

	fprintf(outfp, ">\n");
	fprintf(outfp, "   <B>Match to Vector: </B>");
	fprintf(outfp, "<IMG SRC=http://www.ncbi.nlm.nih.gov/gorf/gun2.cgi?0M(000M)FF0000(000M)> <B>Strong </B>  ");
	fprintf(outfp, "<IMG SRC=http://www.ncbi.nlm.nih.gov/gorf/gun2.cgi?0M(000M)FF00FF(000M)> <B>Moderate </B>  ");
	fprintf(outfp, "<IMG SRC=http://www.ncbi.nlm.nih.gov/gorf/gun2.cgi?0M(000M)00FF00(000M)> <B>Weak </B>\n");
	fprintf(outfp, "   <B>Segment of suspect origin: </B>");
	fprintf(outfp, "<IMG SRC=http://www.ncbi.nlm.nih.gov/gorf/gun2.cgi?0M(000M)FFFF00(000M)>\n\n");


	fprintf(outfp, "<B>Segments matching vector: </B>\n");
	if (seqloc1)
	{
		if (seqloc1->choice == SEQLOC_PACKED_INT)
			tmp = seqloc1->data.ptrvalue;
		fprintf(outfp, "<A HREF=\"http://www.ncbi.nlm.nih.gov/VecScreen/VecScreen_docs.html#Strong\" TARGET=\"VecScreenInfo\">Strong match</A>:  ");
		while (tmp)
		{
			fprintf(outfp, " %ld-%ld", (long) (SeqLocStart(tmp)+1), (long) (SeqLocStop(tmp)+1));
			if (tmp->next)
				fprintf(outfp, ",");
			else
				fprintf(outfp, "\n");
			tmp = tmp->next;
		}
	}

	if (seqloc2)
	{
		if (seqloc2->choice == SEQLOC_PACKED_INT)
			tmp = seqloc2->data.ptrvalue;
		fprintf(outfp, "<A HREF=\"http://www.ncbi.nlm.nih.gov/VecScreen/VecScreen_docs.html#Moderate\" TARGET=\"VecScreenInfo\">Moderate match</A>:");
		while (tmp)
		{
			fprintf(outfp, " %ld-%ld", (long) (SeqLocStart(tmp)+1), (long) (SeqLocStop(tmp)+1));
			if (tmp->next)
				fprintf(outfp, ",");
			else
				fprintf(outfp, "\n");
			tmp = tmp->next;
		}
	}

	if (seqloc3)
	{
		if (seqloc3->choice == SEQLOC_PACKED_INT)
			tmp = seqloc3->data.ptrvalue;
		fprintf(outfp, "<A HREF=\"http://www.ncbi.nlm.nih.gov/VecScreen/VecScreen_docs.html#Weak\" TARGET=\"VecScreenInfo\">Weak match</A>:    ");
		while (tmp)
		{
			fprintf(outfp, " %ld-%ld", (long) (SeqLocStart(tmp)+1), (long) (SeqLocStop(tmp)+1));
			if (tmp->next)
				fprintf(outfp, ",");
			else
				fprintf(outfp, "\n");
			tmp = tmp->next;
		}
	}

	if (seqloc4)
	{
		if (seqloc4->choice == SEQLOC_PACKED_INT)
			tmp = seqloc4->data.ptrvalue;
		fprintf(outfp, "<A HREF=\"http://www.ncbi.nlm.nih.gov/VecScreen/VecScreen_docs.html#Suspect\" TARGET=\"VecScreenInfo\">Suspect origin</A>:");
		while (tmp)
		{
			fprintf(outfp, " %ld-%ld", (long) (SeqLocStart(tmp)+1), (long) (SeqLocStop(tmp)+1));
			if (tmp->next)
				fprintf(outfp, ",");
			else
				fprintf(outfp, "\n");
			tmp = tmp->next;
		}
	}

	fprintf(outfp, "\n");

	return TRUE;
}



/* 
	Prints list of matching segments
*/

Boolean LIBCALL
VSPrintListFromSeqLocs (ValNodePtr vnp, FILE *outfp)

{
	SeqLocPtr seqloc1=NULL, seqloc2=NULL, seqloc3=NULL, seqloc4=NULL, tmp;
	ValNodePtr var=NULL;

        if (outfp == NULL)
		return FALSE;

	var = vnp;
	while (var)
	{
		if (var->choice == 1)
		{
			seqloc1 = var->data.ptrvalue;
		}
		else if (var->choice == 2)
		{
			seqloc2 = var->data.ptrvalue;
		}
		else if (var->choice == 3)
		{
			seqloc3 = var->data.ptrvalue;
		}
		else if (var->choice == 4)
		{
			seqloc4 = var->data.ptrvalue;
		}
		var = var->next;
	}

	if (!seqloc1 && !seqloc2 && !seqloc3 && !seqloc4)
		return TRUE;

	if (seqloc1)
	{
		if (seqloc1->choice == SEQLOC_PACKED_INT)
			tmp = seqloc1->data.ptrvalue;
		fprintf(outfp, "Strong match\n");
		while (tmp)
		{
			fprintf(outfp, "%ld\t%ld\n", (long) (SeqLocStart(tmp)+1), (long) (SeqLocStop(tmp)+1));
			tmp = tmp->next;
		}
	}

	if (seqloc2)
	{
		if (seqloc2->choice == SEQLOC_PACKED_INT)
			tmp = seqloc2->data.ptrvalue;
		fprintf(outfp, "Moderate match\n");
		while (tmp)
		{
			fprintf(outfp, "%ld\t%ld\n", (long) (SeqLocStart(tmp)+1), (long) (SeqLocStop(tmp)+1));
			tmp = tmp->next;
		}
	}

	if (seqloc3)
	{
		if (seqloc3->choice == SEQLOC_PACKED_INT)
			tmp = seqloc3->data.ptrvalue;
		fprintf(outfp, "Weak match\n");
		while (tmp)
		{
			fprintf(outfp, "%ld\t%ld\n", (long) (SeqLocStart(tmp)+1), (long) (SeqLocStop(tmp)+1));
			tmp = tmp->next;
		}
	}

	if (seqloc4)
	{
		if (seqloc4->choice == SEQLOC_PACKED_INT)
			tmp = seqloc4->data.ptrvalue;
		fprintf(outfp, "Suspect origin\n");
		while (tmp)
		{
			fprintf(outfp, "%ld\t%ld\n", (long) (SeqLocStart(tmp)+1), (long) (SeqLocStop(tmp)+1));
			tmp = tmp->next;
		}
	}

	return TRUE;
}


#define BUFFER_LENGTH 255

/* 
	Prints Id line for list results format
*/

Boolean LIBCALL
VSPrintListIdLine (BioseqPtr query_bsp, CharPtr proginfo, CharPtr database, FILE *outfp)

{ 

	Char            idbuf [BUFFER_LENGTH];
	CharPtr         dbdef, ptr, chptr;
	Int4            length;
	ReadDBFILEPtr   rdfp, rdfp_var;
	Boolean         first_title;


	if (outfp == NULL)
		return FALSE;


	/* prepare Query ID string for output */

	ptr = NULL; chptr = NULL;

	if (query_bsp) 
		ptr = BioseqGetTitle (query_bsp);

	if (ptr) {
		while (IS_WHITESP(*ptr))
			ptr++;

		if (*ptr == '"') {
			ptr++;
			chptr = StringChr (ptr, '"');
		}
		else {
			chptr = StringPBrk (ptr, " \t");
		}
	}

	if (ptr && *ptr != '\0') {
		StringNCpy_0 (idbuf, ptr, (chptr ? MIN((chptr - ptr) + 1, BUFFER_LENGTH) : BUFFER_LENGTH));
	} 
	else {
		StringCpy (idbuf, "Id_unknown");
	}


	/* prepare database description(s) for output */

	dbdef = NULL; rdfp = NULL;

	if (database)
		rdfp = readdb_new_ex(database, READDB_DB_IS_NUC, FALSE);

	if (rdfp) {
		rdfp_var = rdfp;
		length = 0;
		while (rdfp_var) {
			length += StringLen(readdb_get_title(rdfp_var));
			length += 3;
			rdfp_var = rdfp_var->next;
		}
		dbdef = MemNew(length*sizeof(Char));
	}

	if (dbdef) {
		ptr = dbdef;
		rdfp_var = rdfp;
	
		first_title = TRUE;
		while (rdfp_var) {
			chptr = readdb_get_title(rdfp_var);
	    
			if(chptr == NULL) {
				rdfp_var = rdfp_var->next;
				continue;
			}
	    
			if(!first_title) {
				*ptr = ';';
				ptr++;
				*ptr = ' ';
				ptr++;
			}
			else
				first_title = FALSE;	     
	    
			StringCpy(ptr, readdb_get_title(rdfp_var));	 

			length = StringLen(ptr);
			ptr += length;

			rdfp_var = rdfp_var->next;
		}

		*ptr = '\0';

		rdfp = readdb_destruct(rdfp);

	}


	fprintf(outfp, ">Vector %s Screen: %s Database: %s\n", 
		idbuf, proginfo ? proginfo : "unknown", dbdef ? dbdef : "unknown");

	dbdef = MemFree(dbdef);

	return TRUE;

}


