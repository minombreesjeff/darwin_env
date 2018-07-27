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

File name: blastool.c

Author: Tom Madden

Contents: Utilities for BLAST

******************************************************************************/
/*
* $Revision: 6.173 $
* $Log: blastool.c,v $
* Revision 6.173  2002/01/07 23:16:00  dondosha
* Fixed several memory leaks and allocation/freeing bugs in multithreaded megablast
*
* Revision 6.172  2001/12/28 20:41:09  dondosha
* 1. Mega BLAST related parameters moved to a separate structure
* 2. Disallow word sizes < 8 for Mega BLAST
*
* Revision 6.171  2001/11/26 20:40:15  camacho
* Used the correct maximum oid in BlastProcessGiLists
*
* Revision 6.170  2001/11/14 23:41:32  dondosha
* Minor change in MegaBlastPrintAlignInfo
*
* Revision 6.169  2001/11/13 14:13:02  egorov
* The change fixes a bug when database specified by a list of GIs is
* smaller than list of GIs returned from Entrez what causes segmentation fault.
*
* Revision 6.168  2001/11/05 17:30:24  egorov
* Initialize 'cutoff' variable. This eliminates UMR problem
* and some of blastsrv3.REAL crashes.
*
* Revision 6.167  2001/10/30 17:27:00  dondosha
* Check if title exists before printing it in PrintTabularOutputHeader
*
* Revision 6.166  2001/10/24 15:02:48  dondosha
* Correction in BlastPrintAlignInfo for 2 sequences case
*
* Revision 6.165  2001/10/15 19:53:20  vakatov
* Get rid of the extra LIBCALL at BLASTPostSearchLogic()
*
* Revision 6.164  2001/10/12 15:26:29  dondosha
* 1. Improvements to BlastClusterHitsFromSeqAlign
* 2. Added BLASTPostSearchLogic function
*
* Revision 6.163  2001/10/02 16:03:58  dondosha
* Fix for ungapped search tabular output
*
* Revision 6.162  2001/09/06 20:24:33  dondosha
* Removed threshold_first
*
* Revision 6.161  2001/09/06 17:11:16  dondosha
* Truncate query defline in tabular output header if it is too long
*
* Revision 6.160  2001/09/05 20:31:04  dondosha
* Fixed tiny memory leak in PrintTabularOutputHeader function
*
* Revision 6.159  2001/08/15 21:37:09  dondosha
* Added functions for clustering of hits from a seqalign
*
* Revision 6.158  2001/07/31 16:42:39  dondosha
* Added function FastaCheckDna
*
* Revision 6.157  2001/07/20 18:38:27  dondosha
* Minor bug fix in MegaBlastPrintAlignInfo
*
* Revision 6.156  2001/07/09 14:17:24  madden
* Fix PC-lint complaints from R. Williams
*
* Revision 6.155  2001/07/09 13:12:04  madden
* Removed unused variables
*
* Revision 6.154  2001/07/06 13:59:02  madden
* Fixed compiler and lint warnings
*
* Revision 6.153  2001/06/29 20:45:21  dondosha
* If percent identity close but less than 100, print 99.99 in tabular output
*
* Revision 6.152  2001/06/28 21:22:20  dondosha
* Do not print query information in tabular output header if it is not available
*
* Revision 6.151  2001/06/28 13:42:09  madden
* Fixes to prevent overflow on number of hits reporting
*
* Revision 6.150  2001/06/25 18:04:24  madden
* Fix to BlastComputeProbs to ignore score > max or < min
*
* Revision 6.149  2001/06/18 21:45:16  dondosha
* Adjusted PrintTabularOutputHeader for web BLAST
*
* Revision 6.148  2001/06/15 21:18:39  dondosha
* Added function PrintTabularOutputHeader
*
* Revision 6.147  2001/06/15 16:38:46  dondosha
* Correction to previous changes
*
* Revision 6.146  2001/06/15 15:48:38  dondosha
* Fixed uninitialized variable bug
*
* Revision 6.145  2001/06/14 22:09:15  dondosha
* Rearranged code for gi lists and oid masks processing to get rid of duplication
*
* Revision 6.144  2001/06/14 16:33:35  dondosha
* Fixed bug in the new function BlastDbGiListToOidList
*
* Revision 6.143  2001/06/13 21:45:09  dondosha
* Search of multiple databases with gi files implemented
*
* Revision 6.142  2001/06/07 19:30:03  dondosha
* Pass believe query argument to BlastPrintTabulatedResults
*
* Revision 6.141  2001/06/06 21:22:43  dondosha
* Added (query) Bioseq and SeqLoc arguments to function BlastPrintTabulatedResults
*
* Revision 6.140  2001/06/05 22:16:15  dondosha
* Check if KarlinBlk exists in FormatBlastParameters
*
* Revision 6.139  2001/05/29 22:00:46  dondosha
* Tabulated output bug fixes
*
* Revision 6.138  2001/05/24 16:25:22  dondosha
* Correction for query and subject ids printed with post-search tabulated format
*
* Revision 6.137  2001/05/23 22:38:48  dondosha
* Added option -m 9 to print post-search tabulated output
*
* Revision 6.136  2001/05/15 19:19:26  shavirin
* Fixed minor bug in the function convertSeqAlignListToValNodeList().
*
* Revision 6.135  2001/05/11 22:05:20  dondosha
* Added BlastPrintTabulatedResults function for post-search formatting
*
* Revision 6.134  2001/05/10 21:58:58  dondosha
* Always print full ids in non-megablast tabulated output
*
* Revision 6.133  2001/05/04 22:14:41  dondosha
* Do not call BioseqLockById from MegaBlastPrintAlignInfo in case of local ids in the database
*
* Revision 6.132  2001/05/04 14:14:28  madden
* Fixes for multiple patterns in phi-blast
*
* Revision 6.131  2001/04/26 16:43:10  madden
* Correction to convertValNodeListToSeqAlignList from AS
*
* Revision 6.130  2001/04/25 13:04:03  madden
* Fix from AS to convertValNodeListToSeqAlignList for multiple occurrences of a pattern
*
* Revision 6.129  2001/04/10 17:01:15  dondosha
* Fixed purify errors, including problem with multithreaded tabulated output
*
* Revision 6.128  2001/04/06 18:15:08  madden
* Move UNIX-specific stuff (HeyIAmInMemory) to bqueue.[ch]
*
* Revision 6.127  2001/04/04 20:32:40  dondosha
* Correction for tabulated output from bl2seq for translated searches
*
* Revision 6.126  2001/04/03 22:35:24  kans
* include simutil.h to get prototype for Change_Loc_Strand for Mac compiler
*
* Revision 6.125  2001/04/03 21:59:49  dondosha
* Implemented tabulated output for non-megablast bl2seq
*
* Revision 6.124  2001/03/27 15:46:16  madden
* Fix problem in BlastSetUserErrorString
*
* Revision 6.123  2001/03/22 21:37:41  madden
* Raise hsp_num_max to 400
*
* Revision 6.122  2001/03/19 22:39:24  dondosha
* Allow location on the first query sequence for megablast
*
* Revision 6.121  2001/03/19 18:54:16  madden
* Added BlastSeqLocFillDoubleIntEx, changed BlastSeqLocFillDoubleIntRev
*
* Revision 6.120  2001/03/13 14:00:24  madden
* Make ungapped case consistent with stand-alone binary
*
* Revision 6.119  2001/03/07 14:09:35  madden
* Default window size for blastn is 30
*
* Revision 6.118  2001/02/28 19:55:30  dondosha
* Corrected handling of local query IDs for megablast tabulated output
*
* Revision 6.117  2001/02/21 15:05:31  dondosha
* Round scaled matrix values to nearest integer instead of integer cast
*
* Revision 6.116  2001/02/16 22:16:22  dondosha
* BlastScaleMatrix: if new lambda is -1, stop changing factor
*
* Revision 6.115  2001/02/16 18:45:40  dondosha
* Fixed minor purify errors
*
* Revision 6.114  2001/02/08 20:41:15  dondosha
* Implemented tabulated output for all translated programs
*
* Revision 6.113  2001/02/08 16:33:03  kans
* include mblast.h for Mac compiler
*
* Revision 6.112  2001/02/07 21:16:05  dondosha
* Added callbacks to produce tabulated output
*
* Revision 6.111  2001/01/25 14:34:46  madden
* Fix for ungapped blastn
*
* Revision 6.110  2001/01/23 21:01:01  dondosha
* Change double quotes to single quotes in previous change
*
* Revision 6.109  2001/01/23 20:25:42  dondosha
* 1. Renamed BlastParceInputString to BlastParseInputString
* 2. Recognize a double quoted string as an option value in
*    BlastParseInputString
*
* Revision 6.108  2001/01/17 17:44:34  madden
* Set default blastn window size to zero
*
* Revision 6.107  2001/01/16 14:03:54  madden
* Enable gapped check for blastn immediately after finding hits
*
* Revision 6.106  2001/01/12 16:57:29  shavirin
* Checked for NULL score frequencies to avoid coredump in case of empty
* sequence.
*
* Revision 6.105  2000/12/19 18:39:50  madden
* Add function BlastSetUserErrorString and BlastDeleteUserErrorString
*
* Revision 6.104  2000/12/15 19:27:17  dondosha
* Corrected the reference link and display for Mega BLAST
*
* Revision 6.103  2000/12/04 18:51:24  madden
* Fix memory leaks
*
* Revision 6.102  2000/11/29 16:32:59  dondosha
* Fixed minor memory leak in MegaBlastPrintReference
*
* Revision 6.101  2000/11/21 18:46:18  madden
* Remove settings for blosum62_20[ab]
*
* Revision 6.100  2000/11/09 16:12:36  madden
* Set do_sum_stats TRUE for ungapped blastp
*
* Revision 6.99  2000/11/07 16:30:26  madden
* Introduce intermediate score (before linking of HSPs) for blastx and tblastn
*
* Revision 6.98  2000/11/01 16:25:56  madden
* Changes from Futamura for psitblastn
*
* Revision 6.97  2000/10/26 18:52:56  dondosha
* Added function MegaBlastPrintReference
*
* Revision 6.96  2000/10/13 18:49:22  lewisg
* move structures out of unix ifdef
*
* Revision 6.95  2000/10/12 21:39:39  shavirin
* Added support for OOF in the function  BLASTFilterOverlapRegions().
*
* Revision 6.94  2000/10/12 17:55:06  shavirin
* Fixed NT error for invalid structure identifier.
*
* Revision 6.93  2000/10/11 21:49:58  shavirin
* Added set of functions around BLASTFilterOverlapRegions() to sort and
* filter output SeqAlign by overlapping reagions.
*
* Revision 6.92  2000/10/05 19:41:43  shavirin
* Rewritten function BlastErrorPrintExtra().
*
* Revision 6.91  2000/09/15 17:11:53  dondosha
* Set gap parameters to zeros for megablast in BLASTOptionSetGapParams
*
* Revision 6.90  2000/08/29 19:35:49  madden
* Add gilist_not_owned to blast_gi_list
*
* Revision 6.89  2000/08/28 17:09:53  madden
* query_lcase_mask no longer freed in BLASTOptionDelete
*
* Revision 6.88  2000/08/23 18:48:44  madden
* Use BlastKarlinBlkGappedCalcEx in place of BlastKarlinBlkGappedCalc
*
* Revision 6.87  2000/08/18 19:34:38  madden
* Do not print lcl| in AcknowledgeBlastQuery
*
* Revision 6.86  2000/08/04 15:02:25  shavirin
* Changed default threshhold values for matrixes BLOSUM62_20,
* BLOSUM62_20A, BLOSUM62_20B.
*
* Revision 6.85  2000/07/31 16:41:00  shavirin
* Reduced POSIT_SCALE_FACTOR from 1000 to 200 to avoid overflow
* with BLOSUM80; moved declaration os POSIT_SCALE_FACTOR to posit.h
*
* Revision 6.84  2000/07/21 15:23:24  madden
* Set default threshold for BLOSUM62_20
*
* Revision 6.83  2000/07/12 23:07:30  kans
* reverse_seq moved from pseed3.c to blastool.c, placed in blast.h header, called by gapxdrop.c
*
* Revision 6.82  2000/06/20 15:50:46  shavirin
* Added new functions: BLASTAddBlastDBTitleToSeqAnnot and
* BLASTGetDatabaseTitleFromSeqAnnot().
*
* Revision 6.81  2000/04/18 16:29:27  madden
* Free gifile name
*
* Revision 6.80  2000/04/10 19:58:06  dondosha
* Added BlastSeqLocFillDoubleIntRev to fill mask locations for concatenated reverse strand in blastn
*
* Revision 6.79  2000/03/31 19:10:57  dondosha
* Changed some names related to MegaBlast
*
* Revision 6.78  2000/03/30 21:45:36  madden
* Make BlastDeleteHeap non-static
*
* Revision 6.77  2000/03/14 13:44:01  madden
* Set default of sort_gi_list to TRUE
*
* Revision 6.76  2000/03/01 20:45:15  shavirin
* Clear some memory in BLASTOptionsDelete() function
*
* Revision 6.75  2000/02/23 20:45:45  dondosha
* Modified heap operations for blastn to handle concatenated contexts
*
* Revision 6.74  2000/01/21 22:24:09  madden
* Use Nlm_Int8tostr in place of Ltostr
*
* Revision 6.73  2000/01/13 18:10:43  madden
* Fix problem with incorrect stat values for blastn and missing hits
*
* Revision 6.72  2000/01/07 16:01:24  madden
* Use readdb_get_totals_ex to get db number to report
*
* Revision 6.71  1999/12/31 14:23:19  egorov
* Add support for using mixture of real and maks database with gi-list files:
* 1. Change logic of creating rdfp list.
* 2. BlastGetDbChunk gets real databases first, then masks.
* 3. Propoper calculation of database sizes using alias files.
* 4. Change to CommonIndex to support using of mask databases.
* 5. Use correct gis in formated output (BlastGetAllowedGis()).
* 6. Other small changes
*
* Revision 6.70  1999/12/21 20:03:23  egorov
* readdb_gi2seq() has new parameter.  Use NULL here.
*
* Revision 6.69  1999/12/21 16:58:00  madden
* Fix command-line parser for options in case there is no space between option and value
*
* Revision 6.68  1999/12/17 20:47:04  egorov
* Fix 'gcc -Wall' warnings
*
* Revision 6.67  1999/12/16 19:16:49  egorov
* Return a value from not-void function
*
* Revision 6.66  1999/12/14 15:35:13  madden
* Added BlastPrintFilterWarning
*
* Revision 6.65  1999/11/30 19:00:51  madden
* Added Nlm_SwapUint4 calls for the ordinal ID list
*
* Revision 6.64  1999/11/26 22:26:49  madden
* Change gap_x_dropoff value for blastn
*
* Revision 6.63  1999/11/09 14:16:53  madden
* made sum_stats default again, rolling back rev 6.61
*
* Revision 6.62  1999/11/02 15:22:18  madden
* Add BlastParseInputString and BlastGetLetterIndex
*
* Revision 6.61  1999/10/27 21:01:32  madden
* made do_sum_stats not default
*
* Revision 6.60  1999/09/29 17:14:50  shavirin
* Fixed memory leak in BLASTOptionsDelete()
*
* Revision 6.59  1999/09/22 20:59:20  egorov
* Add blast db mask stuff
*
* Revision 6.58  1999/09/16 16:54:43  madden
* Allow longer wordsizes
*
* Revision 6.57  1999/09/14 19:56:39  shavirin
* Fixed bug in PHI-Blast when number of hits to DB == 0
*
* Revision 6.56  1999/09/09 18:00:25  madden
* formatting problem for effective db length
*
* Revision 6.55  1999/08/27 20:22:51  shavirin
* Added default value for decline_align in the function BLASTOptionNew().
*
* Revision 6.53  1999/08/17 14:10:05  madden
* Validation for smith_waterman and tweak_parameters options
*
* Revision 6.52  1999/05/25 13:37:15  madden
* Call readdb_get_sequence_length only for seqs in db
*
* Revision 6.51  1999/04/27 17:22:27  madden
* Set hsp_num_max for ungapped BLAST
*
* Revision 6.50  1999/04/14 14:53:48  madden
* Correction for databases over 2 Gig
*
* Revision 6.49  1999/04/01 21:42:47  madden
* Fix memory leaks when gi list is used
*
* Revision 6.48  1999/03/22 15:19:03  beloslyu
* corrections to compile on Red Hat Linux v5.2
*
* Revision 6.47  1999/03/18 16:43:57  shavirin
* Added function Boolean HeyIAmInMemory(Int4 program)
*
* Revision 6.46  1999/03/17 13:21:07  madden
* Fix comment in comment problem
*
* Revision 6.45  1999/03/12 15:03:45  egorov
* Add proper Int4-long type casting
*
* Revision 6.44  1999/02/19 14:18:25  madden
* Added back check for negative nucl. penalty
*
* Revision 6.43  1999/02/19 14:16:20  madden
* list manipulation bug for seed
*
* Revision 6.41  1999/01/26 18:26:54  madden
* make updateLambdaK public
*
* Revision 6.40  1999/01/08 22:08:42  madden
* BlastScaleMatrix returns factor as FloatHi
*
 * Revision 6.39  1998/12/31 18:17:06  madden
 * Added strand option
 *
 * Revision 6.38  1998/12/29 17:45:07  madden
 * Add do_sum_stats flag
 *
 * Revision 6.37  1998/12/03 15:19:32  madden
 * Changes to speed up BlastFreeHeap and InsertToHeap
 *
 * Revision 6.36  1998/11/04 01:36:06  egorov
 * Add support for entrez-query and org-name to blast3
 *
 * Revision 6.35  1998/10/13 22:06:24  madden
 * Fixed AdjustDbNumbers
 *
 * Revision 6.34  1998/09/28 12:29:24  madden
 * Check for problem in rescaling code
 *
 * Revision 6.33  1998/09/22 18:46:43  egorov
 * Add BlastErrorPrintExtra()
 *
 * Revision 6.32  1998/09/17 19:53:02  madden
 * Added fillCandLambda
 *
 * Revision 6.31  1998/09/16 18:59:35  madden
 * Print subset information if entire database not searched
 *
 * Revision 6.30  1998/09/14 15:48:36  madden
 * Fixed PHI-BLAST reference
 *
 * Revision 6.29  1998/09/14 15:11:15  egorov
 * Add support for Int8 length databases; remove unused variables
 *
 * Revision 6.28  1998/09/10 22:36:09  madden
 * Added convertSeqAlignListToValNodeList and convertValNodeListToSeqAlignList
 *
 * Revision 6.27  1998/09/09 21:18:09  madden
 * Added PrintKAParametersExtra
 *
 * Revision 6.26  1998/09/04 14:45:42  madden
 * Moved code from blast.c blastool.c
 *
 * Revision 6.25  1998/08/28 21:21:29  madden
 * Changed PhiBlast ref
 *
 * Revision 6.24  1998/08/25 14:16:22  madden
 * Added BlastGetPhiReference and BlastPrintPhiReference
 *
 * Revision 6.23  1998/07/28 15:23:47  madden
 * Changed Number of sequences better printout again
 *
 * Revision 6.22  1998/07/27 21:54:02  madden
 * Fixed printing of non-integral E-values
 *
 * Revision 6.21  1998/07/21 20:58:06  madden
 * Changes to allow masking at hash only
 *
 * Revision 6.20  1998/07/17 15:40:01  madden
 * Changes for Effective search space.
 *
 * Revision 6.19  1998/07/02 22:15:31  madden
 * Fixed bug in BlastAdjustDbNumbers
 *
 * Revision 6.18  1998/06/17 18:10:24  madden
 * Validate for isPatternSearch
 *
 * Revision 6.17  1998/06/12 15:52:49  madden
 * Fixed warnings
 *
 * Revision 6.16  1998/06/05 15:50:35  madden
 * Return 1 from BLASTOptionValidateEx if wordsize incorrect.
 *
 * Revision 6.15  1998/06/03 17:40:48  madden
 * Added blastn check for wordsize in BLASTOptionValidateEx
 *
 * Revision 6.14  1998/05/28 19:59:42  madden
 * Changed hsp_range_max
 *
 * Revision 6.13  1998/05/21 19:44:45  egorov
 * Make word "Reference" be HTML link in case HTML output requested
 *
 * Revision 6.12  1998/05/03 17:21:04  madden
 * Added error message for expect_value <= 0, fix typo setting open and expect values
 *
 * Revision 6.11  1998/05/01 18:33:56  egorov
 * Add new parametes to BLASTOptionSetGapParam()
 *
 * Revision 6.10  1998/04/30 14:28:46  madden
 * Raise thresholds for blastx, tblast[nx]
 *
 * Revision 6.9  1998/04/29 14:28:07  madden
 * Fix reference formatting problem
 *
 * Revision 6.8  1998/04/27 16:47:34  madden
 * Added window and threshold to BLASTOptionSetGapParams
 *
 * Revision 6.7  1998/04/24 19:28:29  madden
 * Added BlastScaleMatrix (and other rescaling code moved from posit.c)
 *
 * Revision 6.6  1998/04/13 20:29:57  madden
 * Add one to length of array for NULLB at end
 *
 * Revision 6.5  1998/03/24 15:38:23  madden
 * Use BlastDoubleInt4Ptr to keep track of gis and ordinal_ids
 *
 * Revision 6.4  1998/03/18 14:14:18  madden
 * Support random access by gi list
 *
 * Revision 6.3  1998/02/28 17:24:30  madden
 * Default window_size zero for blastn
 *
 * Revision 6.2  1998/02/27 16:52:05  madden
 * Added BlastGetSequenceFromBioseq
 *
 * Revision 6.1  1998/02/27 14:30:30  madden
 * Tools (or utilities) for the BLAST programs
 *
*/

#include <ncbi.h>
#include <blastpri.h>
#include <objcode.h>
#include <objseq.h>
#include <sequtil.h>
#include <readdb.h>
#include <ncbithr.h>
#include <txalign.h>
#include <posit.h>
#include <seed.h>
#include <xmlblast.h>
#include <mblast.h>
#include <simutil.h>


/* Mutex for assignment of db seqs to search. */
TNlmMutex err_message_mutex=NULL;

typedef struct _FilterAlign {
    SeqAlignPtr  sap;
    Char         subject_sip[64];
    HspPtr       hsp;
} FilterAlign, PNTR FilterAlignPtr;


typedef struct _SappSet {
    Int4               count;
    FilterAlignPtr PNTR fapp;
} SappSet, PNTR SappSetPtr;


#define BUFFER_LENGTH 255

/*
	adds the new string to the buffer, separating by a tilde.
	Checks the size of the buffer for FormatBlastParameters and
	allocates longer replacement if needed.
*/

static Boolean 
add_string_to_bufferEx(CharPtr buffer, CharPtr *old, Int2Ptr old_length, Boolean add_tilde)

{
	CharPtr new, ptr;
	Int2 length, new_length;

	length = (StringLen(*old));

	if((StringLen(buffer)+length+3) > *old_length)
	{
		new_length = *old_length + 255;
		new = MemNew(new_length*sizeof(Char));
		if (*old_length > 0 && *old != NULL)
		{
			MemCpy(new, *old, *old_length);
			*old = MemFree(*old);
		}
		*old = new;
		*old_length = new_length;
	}

	ptr = *old;
	ptr += length;
	if (add_tilde)
	{
		*ptr = '~';
		ptr++;
	}

	while (*buffer != NULLB)
	{
		*ptr = *buffer;
		buffer++; ptr++;
	}

	return TRUE;
}

static Boolean 
add_string_to_buffer(CharPtr buffer, CharPtr *old, Int2Ptr old_length)

{
	return add_string_to_bufferEx(buffer, old, old_length, TRUE);
}

/*
	Formats the BLAST parameters for the BLAST report.
	One CharPtr is returned, newlines are indicated by tildes ('~').
*/	


CharPtr
FormatBlastParameters(BlastSearchBlkPtr search)

{
	BLAST_ParameterBlkPtr   pbp;
	BLAST_Score cutoff = 0;
	Char buffer[128];
	CharPtr ret_buffer;
	Int2 ret_buffer_length;
	Int4 num_entries;
	Int8 total_length;
	Nlm_FloatHi evalue;

	pbp = search->pbp;

	ret_buffer = NULL;
	ret_buffer_length = 0;

	
	sprintf(buffer, "Matrix: %s", search->sbp->name);
	add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
	
	if (pbp->gapped_calculation)
	{
		sprintf(buffer, "Gap Penalties: Existence: %ld, Extension: %ld", (long) search->pbp->gap_open, (long) search->pbp->gap_extend);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
	}

	if (pbp->two_pass_method == FALSE)
	{
		sprintf(buffer, "Number of Hits to DB: %s", Nlm_Int8tostr((Int8) search->second_pass_hits, 1));
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
		
		readdb_get_totals_ex(search->rdfp, &total_length, &num_entries, TRUE);
		
	    	sprintf(buffer, "Number of Sequences: %ld", (long) num_entries);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
		sprintf(buffer, "Number of extensions: %ld", (long) search->second_pass_extends);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
		sprintf(buffer, "Number of successful extensions: %ld", (long) search->second_pass_good_extends);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
	}
	else
	{
		sprintf(buffer, "Number of Hits to DB: 1st pass: %s, 2nd pass: %s", 
			Nlm_Int8tostr((Int8) search->first_pass_hits, 1), Nlm_Int8tostr((Int8) search->second_pass_hits, 1));
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
		readdb_get_totals_ex(search->rdfp, &total_length, &num_entries, TRUE);
		sprintf(buffer, "Number of Sequences: 1st pass: %ld, 2nd pass: %ld", 
			(long) num_entries, (long) search->second_pass_trys);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
		sprintf(buffer, "Number of extensions: 1st pass: %ld, 2nd pass: %ld", 
			(long) search->first_pass_extends, (long) search->second_pass_extends);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
		sprintf(buffer, "Number of successful extensions: 1st pass: %ld, 2nd pass: %ld", 
			(long) search->first_pass_good_extends, (long) search->second_pass_good_extends);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
	}

	if (pbp->cutoff_e > 0.1)
	{
		sprintf(buffer, "Number of sequences better than %4.1f: %ld", 
			pbp->cutoff_e, (long) search->number_of_seqs_better_E);
	}
	else
	{
		sprintf(buffer, "Number of sequences better than %3.1e: %ld", 
			pbp->cutoff_e, (long) search->number_of_seqs_better_E);
	}
	add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);

	if (pbp->gapped_calculation &&
		StringCmp(search->prog_name, "blastn") != 0)
	{
		sprintf(buffer, "Number of HSP's better than %4.1f without gapping: %ld", 
			pbp->cutoff_e, (long) search->prelim_gap_no_contest);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
		sprintf(buffer, "Number of HSP's successfully gapped in prelim test: %ld", 
			(long) search->prelim_gap_passed);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
		sprintf(buffer, "Number of HSP's that attempted gapping in prelim test: %ld", (long) search->prelim_gap_attempts);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
		sprintf(buffer, "Number of HSP's gapped (non-prelim): %ld", (long) search->real_gap_number_of_hsps);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
	}
        /* The following makes sense only when there is only one query */
        if (search->last_context <= 1) {
           sprintf(buffer, "length of query: %ld", (long) search->context[search->first_context].query->length);
           add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
        }
        sprintf(buffer, "length of database: %s", Nlm_Int8tostr ((Int8) search->dblen, 1));
	add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);

	sprintf(buffer, "effective HSP length: %ld", (long) search->length_adjustment);
	add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);

        if (search->last_context <= 1) {
           sprintf(buffer, "effective length of query: %ld", (long) search->context[search->first_context].query->effective_length);
           add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
        }
	sprintf(buffer, "effective length of database: %s", Nlm_Int8tostr ((Int8) search->dblen_eff, 1));
	add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
        if (search->last_context <= 1) {
           sprintf(buffer, "effective search space: %8.0f", ((Nlm_FloatHi) search->dblen_eff)*((Nlm_FloatHi) search->context[search->first_context].query->effective_length));
           add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
        }
	sprintf(buffer, "effective search space used: %8.0f", (Nlm_FloatHi) search->searchsp_eff);
	add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);

        if (StringCmp(search->prog_name, "blastx") == 0
            || StringCmp(search->prog_name, "tblastn") == 0
            || StringCmp(search->prog_name, "tblastx") == 0
            || StringCmp(search->prog_name, "psitblastn") == 0 )
	{
		sprintf(buffer, "frameshift window, decay const: %ld, %4.1f",
			(long) pbp->gap_size, pbp->gap_decay_rate);
		add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
	}

        if (!search->pbp->mb_params) {
           sprintf(buffer, "T: %ld", (long) search->pbp->threshold_second);
           add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
           sprintf(buffer, "A: %ld", (long) search->pbp->window_size);
           add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
           sprintf(buffer, "X1: %ld (%4.1f bits)", (long) (-search->pbp->dropoff_1st_pass), ((-search->pbp->dropoff_1st_pass)*(search->sbp->kbp[search->first_context]->Lambda/NCBIMATH_LN2)));
           add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
           if (StringCmp(search->prog_name, "blastn") == 0 || search->pbp->gapped_calculation == FALSE) {
              sprintf(buffer, "X2: %ld (%4.1f bits)", (long) search->pbp->gap_x_dropoff, ((search->pbp->gap_x_dropoff)*(search->sbp->kbp[search->first_context]->Lambda/NCBIMATH_LN2)));
              add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
           } else {
              sprintf(buffer, "X2: %ld (%4.1f bits)", (long) search->pbp->gap_x_dropoff, ((search->pbp->gap_x_dropoff)*(search->sbp->kbp_gap[search->first_context]->Lambda/NCBIMATH_LN2)));
              add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
              sprintf(buffer, "X3: %ld (%4.1f bits)", (long) search->pbp->gap_x_dropoff_final, ((search->pbp->gap_x_dropoff_final)*(search->sbp->kbp_gap[search->first_context]->Lambda/NCBIMATH_LN2)));
              add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
           }
           sprintf(buffer, "S1: %ld (%4.1f bits)", (long) search->pbp->gap_trigger, ((((search->pbp->gap_trigger)*(search->sbp->kbp[search->first_context]->Lambda))-(search->sbp->kbp[search->first_context]->logK))/NCBIMATH_LN2));
           add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
           cutoff = 0;
        }
        if (search->last_context <= 1) {
           evalue = pbp->cutoff_e;
           if (StringCmp(search->prog_name, "blastn") == 0 ||
               search->pbp->gapped_calculation == FALSE) {
              BlastCutoffs(&cutoff, &evalue, search->sbp->kbp[search->first_context], (Nlm_FloatHi) search->context[search->first_context].query->effective_length, (Nlm_FloatHi) search->dblen_eff, FALSE);
              sprintf(buffer, "S2: %ld (%4.1f bits)", (long) cutoff, (((cutoff)*(search->sbp->kbp[search->first_context]->Lambda))-(search->sbp->kbp[search->first_context]->logK))/NCBIMATH_LN2);
           } else {
              BlastCutoffs(&cutoff, &evalue, search->sbp->kbp_gap[search->first_context], (Nlm_FloatHi) search->context[search->first_context].query->effective_length, (Nlm_FloatHi) search->dblen_eff, FALSE);
              sprintf(buffer, "S2: %ld (%4.1f bits)", (long) cutoff, (((cutoff)*(search->sbp->kbp_gap[search->first_context]->Lambda))-(search->sbp->kbp_gap[search->first_context]->logK))/NCBIMATH_LN2);
           }
           add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
        }
	return ret_buffer;
}

/*
	Print the buffer, adding newlines where tildes are found.
*/

Boolean LIBCALL
PrintTildeSepLines(CharPtr buffer, Int4 line_length, FILE *outfp)

{
	if (outfp == NULL || buffer == NULL)
		return FALSE;

	asn2ff_set_output(outfp, NULL);

	ff_StartPrint(0, 0, line_length, NULL);
	while (*buffer != NULLB)
	{
		if (*buffer != '~')
			ff_AddChar(*buffer);
		else
			NewContLine();
		buffer++;
	}
	ff_EndPrint();

	return TRUE;
}

/*
	Print the Karlin-Altschul parameters.

	if gapped is TRUE, then slightly different formatting is used.
*/

Boolean LIBCALL
PrintKAParameters(Nlm_FloatHi Lambda, Nlm_FloatHi K, Nlm_FloatHi H, Int4 line_length, FILE *outfp, Boolean gapped)

{
	return PrintKAParametersExtra(Lambda, K, H, 0.0, line_length, outfp, gapped);
}

Boolean LIBCALL
PrintKAParametersExtra(Nlm_FloatHi Lambda, Nlm_FloatHi K, Nlm_FloatHi H, Nlm_FloatHi C, Int4 line_length, FILE *outfp, Boolean gapped)

{
	Char buffer[BUFFER_LENGTH];

	if (outfp == NULL)
		return FALSE;

	asn2ff_set_output(outfp, NULL);

	ff_StartPrint(0, 0, line_length, NULL);
	if (gapped)
	{
		ff_AddString("Gapped");
		NewContLine();
	}
	
	if (C == 0.0)
		ff_AddString("Lambda     K      H");
	else
		ff_AddString("Lambda     K      H      C");
	NewContLine();
	sprintf(buffer, "%#8.3g ", Lambda);
	ff_AddString(buffer);
	sprintf(buffer, "%#8.3g ", K);
	ff_AddString(buffer);
	sprintf(buffer, "%#8.3g ", H);
	ff_AddString(buffer);
	if (C != 0.0)
	{
		sprintf(buffer, "%#8.3g ", C);
		ff_AddString(buffer);
	}
	NewContLine();
	ff_EndPrint();

	return TRUE;

}

/*
	Deallocates *BlastErrorMsgPtr produced by BlastConstructErrorMessage.
*/

BlastErrorMsgPtr BlastDestroyErrorMessage(BlastErrorMsgPtr error_msg)

{
	if (error_msg == NULL)
		return NULL;

	MemFree(error_msg->msg);
	MemFree(error_msg);

	return NULL;
}

/* 
	Prepares error message and appends ValNodePtr, containing BlastErrorMsgPtr, to
	end of chain.  The beginning of the ValNodePtr chain is returned.
*/

ValNodePtr BlastConstructErrorMessage(CharPtr function, CharPtr message, Uint1 level, ValNodePtr PNTR vnpp)

{
	Char buffer[BUFFER_LENGTH];
	CharPtr ptr;
	BlastErrorMsgPtr error_msg;

	if (vnpp == NULL)
		return NULL;

	buffer[0] = NULLB;
	ptr = buffer;
	if (function != NULL)
	{
		sprintf(buffer, "%s: ", function);
		ptr = buffer + StringLen(buffer);
	}
	
	if (message != NULL)
	{
		sprintf(ptr, "%s", message);
	}

	error_msg = (BlastErrorMsgPtr) MemNew(sizeof(BlastErrorMsg));
	error_msg->msg = StringSave(buffer);
	error_msg->level = level;

	ValNodeAddPointer(vnpp, 0, error_msg);

	return *vnpp;
}

/*
	Destroys a chain of ValNodes and the BlastErrorMsgPtr data.
*/

ValNodePtr BlastErrorChainDestroy(ValNodePtr vnp)

{
	ValNodePtr start = vnp;

	while (vnp)
	{
		BlastDestroyErrorMessage(vnp->data.ptrvalue);
		vnp->data.ptrvalue = NULL;
		vnp = vnp->next;
	}

	ValNodeFree(start);

	return NULL;
}

/*
	Prints the error messages.
*/

void LIBCALL BlastErrorPrint(ValNodePtr error_return)

{
	BlastErrorMsgPtr error_msg;

	if (error_return == NULL)
		return;

	while (error_return)
	{
		error_msg = error_return->data.ptrvalue;
		switch (error_msg->level)
		{
			case 0:
				ErrPostEx(SEV_INFO, 0, 0, "%s", error_msg->msg);
				break;
			case 1:
				ErrPostEx(SEV_WARNING, 0, 0, "%s", error_msg->msg);
				break;
			case 2:
				ErrPostEx(SEV_ERROR, 0, 0, "%s", error_msg->msg);
				break;
			case 3:
				ErrPostEx(SEV_FATAL, 0, 0, "%s", error_msg->msg);
				break;
			default:
				ErrPostEx(SEV_WARNING, 0, 0, "Unknown BLAST error level");
				break;
		}
		error_return = error_return->next;
	}
	return;
	
}

void LIBCALL BlastErrorPrintExtra(ValNodePtr error_return, Boolean errpostex, FILE* fp)
{
    BlastErrorMsgPtr error_msg;
    ErrSev	     err_sev;
    CharPtr	     msg;
    
    CharPtr errsevmsg[] = { "UNKNOWN","INFO","WARNING","ERROR",
                            "FATAL"};
    
    if (error_return == NULL)
        return;
    
    while (error_return) {
        error_msg = error_return->data.ptrvalue;
        msg = error_msg->msg;
        
        if(error_msg->level > 5)
            error_msg->level = 0;
        
        if(error_msg->level == 4)
            err_sev = SEV_FATAL;
        else
            err_sev = error_msg->level;

        if (errpostex)
            ErrPostEx(err_sev, 0, 0, "%s", msg);

        if (fp)
            fprintf(fp, "%s: %s\n", errsevmsg[error_msg->level], msg);
        
        error_return = error_return->next;
    }
    return;
}


TxDfDbInfoPtr LIBCALL 
TxDfDbInfoNew (TxDfDbInfoPtr old)

{
	TxDfDbInfoPtr dbinfo;
	dbinfo = MemNew(sizeof(TxDfDbInfo));
	if (old)
		old->next = dbinfo;
	return dbinfo;
}

TxDfDbInfoPtr LIBCALL 
TxDfDbInfoDestruct (TxDfDbInfoPtr dbinfo)

{
	TxDfDbInfoPtr next;

	if (dbinfo == NULL)
		return NULL;

	while (dbinfo)
	{
		dbinfo->name = MemFree(dbinfo->name);
		dbinfo->definition = MemFree(dbinfo->definition);
		dbinfo->date = MemFree(dbinfo->date);
		next = dbinfo->next;
		dbinfo = MemFree(dbinfo);
		dbinfo = next;
	}

	return dbinfo;
}

Boolean LIBCALL
PrintDbReport(TxDfDbInfoPtr dbinfo, Int4 line_length, FILE *outfp)

{

	if (dbinfo == NULL || outfp == NULL)
		return FALSE;

	asn2ff_set_output(outfp, NULL);

	ff_StartPrint(2, 2, line_length, NULL);

	if (dbinfo->subset == FALSE)
	{
		ff_AddString("Database: ");
		ff_AddString(dbinfo->definition);
		NewContLine();
		ff_AddString("  Posted date:  ");
		ff_AddString(dbinfo->date);
		NewContLine();
		ff_AddString("Number of letters in database: "); 
		ff_AddString(Nlm_Int8tostr((Int8) dbinfo->total_length, 1));
		NewContLine();
		ff_AddString("Number of sequences in database:  ");
		ff_AddString(Ltostr((long) dbinfo->number_seqs, 1));
		NewContLine();
	}
	else
	{
		ff_AddString("Subset of the database(s) listed below");
		NewContLine();
		ff_AddString("   Number of letters searched: "); 
		ff_AddString(Nlm_Int8tostr((Int8) dbinfo->total_length, 1));
		NewContLine();
		ff_AddString("   Number of sequences searched:  ");
		ff_AddString(Ltostr((long) dbinfo->number_seqs, 1));
		NewContLine();
	}
	ff_EndPrint();

	return TRUE;
}

/*
	Prints an acknowledgement of the Blast Query, in the standard
	BLAST format.
*/


Boolean LIBCALL
AcknowledgeBlastQuery(BioseqPtr bsp, Int4 line_length, FILE *outfp, Boolean believe_query, Boolean html)

{
	Char buffer[BUFFER_LENGTH];

	if (bsp == NULL || outfp == NULL)
		return FALSE;
	
	asn2ff_set_output(outfp, NULL);

	ff_StartPrint(0, 0, line_length, NULL);
	if (html)
		ff_AddString("<b>Query=</b> ");
	else
		ff_AddString("Query= ");
	if (bsp->id && believe_query)
	{
		SeqIdWrite(bsp->id, buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
		if (StringNCmp(buffer, "lcl|", 4) == 0)
			ff_AddString(buffer+4);
		else
			ff_AddString(buffer);
		ff_AddChar(' ');
	}
	ff_AddString(BioseqGetTitle(bsp));
	NewContLine();
	TabToColumn(10);
	ff_AddChar('(');
	ff_AddString(Ltostr((long) BioseqGetLen(bsp), 1));
	ff_AddString(" letters)");
	NewContLine();
        ff_EndPrint();

        return TRUE;
}

/*
	return the version of BLAST as a char. string.
*/
CharPtr LIBCALL
BlastGetReleaseDate (void)

{
	return BLAST_RELEASE_DATE;
}


/*
	return the version of BLAST as a char. string.
*/
CharPtr LIBCALL
BlastGetVersionNumber (void)

{
	return BLAST_ENGINE_VERSION;
}

Boolean BlastPrintVersionInfo (CharPtr program, Boolean html, FILE *outfp)

{
	return BlastPrintVersionInfoEx(program, html, BlastGetVersionNumber(), BlastGetReleaseDate(), outfp);
}

Boolean BlastPrintVersionInfoEx (CharPtr program, Boolean html, CharPtr version, CharPtr date, FILE *outfp)

{
	CharPtr ret_buffer;


	if (outfp == NULL)
		return FALSE;

	ret_buffer = StringSave(program);
	Nlm_StrUpper(ret_buffer);
	if (html)
		fprintf(outfp, "<b>%s %s [%s]</b>\n", ret_buffer, version, date);
	else
		fprintf(outfp, "%s %s [%s]\n", ret_buffer, version, date);
	ret_buffer = MemFree(ret_buffer);

	return TRUE;
}

/* 
	Returns a reference for the header.
	The newlines are represented by tildes, use PrintTildeSepLines
	to print this.
*/

CharPtr LIBCALL
BlastGetReference(Boolean html)

{
	CharPtr ret_buffer;
	Int2 ret_buffer_length;

	ret_buffer = NULL;
	ret_buffer_length = 0;

	
	if (html) {
		add_string_to_bufferEx("<b><a href=\"http://www.ncbi.nlm.nih.gov/htbin-post/Entrez/query?uid=9254694&form=6&db=m&Dopt=r\">Reference</a>:</b>", &ret_buffer, &ret_buffer_length, TRUE);
		add_string_to_bufferEx("Altschul, Stephen F., Thomas L. Madden, Alejandro A. Sch&auml;ffer, ", &ret_buffer, &ret_buffer_length, TRUE);
	} else
		add_string_to_bufferEx("Reference: Altschul, Stephen F., Thomas L. Madden, Alejandro A. Schaffer, ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("Jinghui Zhang, Zheng Zhang, Webb Miller, and David J. Lipman (1997), ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("\"Gapped BLAST and PSI-BLAST: a new generation of protein database search", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("programs\",  Nucleic Acids Res. 25:3389-3402.", &ret_buffer, &ret_buffer_length, TRUE);
	
	return ret_buffer;
}

Boolean LIBCALL
MegaBlastPrintReference(Boolean html, Int4 line_length, FILE *outfp)

{
	CharPtr ret_buffer;
	Int2 ret_buffer_length;

	ret_buffer = NULL;
	ret_buffer_length = 0;

	if (outfp == NULL)
		return FALSE;
	
	if (html) {
           add_string_to_bufferEx("<b><a href=\"http://www.ncbi.nlm.nih.gov/htbin-post/Entrez/query?uid=10890397&form=6&db=m&Dopt=r\">Reference</a>:</b>", &ret_buffer, &ret_buffer_length, TRUE);
           add_string_to_bufferEx("Zheng Zhang, Scott Schwartz, Lukas Wagner, and Webb Miller (2000),", &ret_buffer, &ret_buffer_length, TRUE);
	} else
           add_string_to_bufferEx("Reference: Zheng Zhang, Scott Schwartz, Lukas Wagner, and Webb Miller (2000), ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("\"A greedy algorithm for aligning DNA sequences\", ", 
                               &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("J Comput Biol 2000; 7(1-2):203-14.", 
                               &ret_buffer, &ret_buffer_length, TRUE);
	
        PrintTildeSepLines(ret_buffer, line_length, outfp);
        ret_buffer = MemFree(ret_buffer);
	return TRUE;
}

Boolean LIBCALL
BlastPrintReference(Boolean html, Int4 line_length, FILE *outfp)

{
	CharPtr ret_buffer;
	
	if (outfp == NULL)
		return FALSE;
	
        ret_buffer = BlastGetReference(html);
        PrintTildeSepLines(ret_buffer, line_length, outfp);
        ret_buffer = MemFree(ret_buffer);

	return TRUE;
}

/* 
	Returns a reference for the header.
	The newlines are represented by tildes, use PrintTildeSepLines
	to print this.
*/

CharPtr LIBCALL
BlastGetPhiReference(Boolean html)

{
	CharPtr ret_buffer;
	Int2 ret_buffer_length;

	ret_buffer = NULL;
	ret_buffer_length = 0;

	
	if (html) {
		add_string_to_bufferEx("<b><a href=\"http://www.ncbi.nlm.nih.gov/htbin-post/Entrez/query?uid=9705509&form=6&db=m&Dopt=r\">Reference</a>:</b>", &ret_buffer, &ret_buffer_length, TRUE);
		add_string_to_bufferEx("Zhang, Zheng, Alejandro A. Sch&auml;ffer, Webb Miller, Thomas L. Madden, ", &ret_buffer, &ret_buffer_length, TRUE);
	} else
		add_string_to_bufferEx("Reference: Zhang, Zheng, Alejandro A. Schaffer, Webb Miller, Thomas L. Madden, ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("David J. Lipman, Eugene V. Koonin, and Stephen F. Altschul (1998), ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("\"Protein sequence similarity searches using patterns as seeds\", ", &ret_buffer, &ret_buffer_length, TRUE);
	add_string_to_bufferEx("Nucleic Acids Res. 26:3986-3990.", &ret_buffer, &ret_buffer_length, TRUE);
	
	return ret_buffer;
}

Boolean LIBCALL
BlastPrintPhiReference(Boolean html, Int4 line_length, FILE *outfp)

{
	CharPtr ret_buffer;
	
	if (outfp == NULL)
		return FALSE;
	
        ret_buffer = BlastGetPhiReference(html);
        PrintTildeSepLines(ret_buffer, line_length, outfp);
        ret_buffer = MemFree(ret_buffer);

	return TRUE;
}
CharPtr scan_to_break (CharPtr ptr)
{

	while (*ptr != NULLB)
	{
		if (*ptr == ';')
		{
			ptr++;
			break;
		}
		ptr++;
	}

	return ptr;
}

Boolean LIBCALL
BlastPrintFilterWarning (CharPtr filter_string, Int4 line_length, FILE *outfp, Boolean html)

{
	CharPtr ptr;

	ptr = filter_string;

	if (filter_string == NULL || outfp == NULL)
		return FALSE;

	while (*ptr != NULLB)
	{
		if (*ptr == 'S')
		{
			ptr = scan_to_break(ptr);
		}
		else if (*ptr == 'C')
		{
			ptr = scan_to_break(ptr);
		}
		else if (*ptr == 'D')
		{
			ptr = scan_to_break(ptr);
		}
		else if (*ptr == 'R')
		{
			ptr = scan_to_break(ptr);
			if (html)
				fprintf(outfp, "<B>NOTE:</B>");
			else
				fprintf(outfp, "NOTE:");
			fprintf(outfp, " This query has been filtered for human repeats.\n");
			fprintf(outfp, "This filtering is effective for 70-90%% of all repeats.\n\n");
		}
		else if (*ptr == 'L')
		{ /* do low-complexity filtering; dust for blastn, otherwise seg.*/
			ptr = scan_to_break(ptr);
		}
		else
		{
			ptr++;
		}
	}

	return TRUE;

}

#define BLAST_SMALLEST_EVALUE 1.0e-300
/*
	Initialize the options structure.

	The fields should be set to default values, that depend on the program.
*/
BLAST_OptionsBlkPtr LIBCALL 
BLASTOptionNew(CharPtr progname, Boolean gapped)

{
	BLAST_OptionsBlkPtr options;

	options = (BLAST_OptionsBlkPtr) MemNew(sizeof(BLAST_OptionsBlk));

	options->perform_culling = FALSE;	/* Results should not be culled at all right now. */

	options->program_name = StringSave(progname);
	options->required_start = 0;
	options->required_end = -1;	/* -1 indicates the end of the query. */
	options->cutoff_s = 0;
	options->cutoff_s2 = 0;
	options->db_length = 0;		/* zero means that real size will be used. */
	options->searchsp_eff = 0;	/* zero means that real size will be used. */

	options->block_width = 20;
	options->hsp_range_max = 100;
	options->entrez_query = NULL;
	options->gifile = NULL;
	options->gilist = NULL;
	options->sort_gi_list = TRUE;

	if (gapped)
	{
		options->gapped_calculation = TRUE;
		options->do_sum_stats = FALSE;
	}
	else
	{
		options->gapped_calculation = FALSE;
		options->do_sum_stats = TRUE;
		options->hsp_num_max = 400;
	}

	options->discontinuous = FALSE;	/* discontinuous is default. */
	if (StringICmp(progname, "blastn") == 0)
	{
		options->gap_decay_rate = 0.5;
		options->gap_prob = 0.5;
		options->gap_size = 50;
		options->window_size = 30;
		options->threshold_second = 0;
		options->expect_value  = 10;
		options->hitlist_size = 500;
		options->two_pass_method  = FALSE;
		options->multiple_hits_only  = FALSE;
		options->number_of_bits  = 0.0;
		/* 1st pass not done for blastn. */
		options->dropoff_2nd_pass  = 20;
		options->matrix  = NULL;
		options->old_stats  = FALSE;
		options->wordsize  = 11;
		options->penalty  = -3;
		options->reward  = 1;
		options->e2 = 0.05;
		/* Used in the post-process gapping of the blastn result. */
		options->gap_open  = 5;
		options->gap_extend  = 2;
                options->decline_align = INT2_MAX;
		options->gap_x_dropoff  = 30;
		options->gap_x_dropoff_final  = 50;
		options->gap_trigger  = 25.0;
		options->strand_option  = BLAST_BOTH_STRAND;
		options->no_check_score  = FALSE;
		if (gapped)
			options->do_sum_stats = FALSE;
		else
			options->do_sum_stats = TRUE;
	}
	else
	{
		options->gap_size = 50;
		options->window_size = 40;
		options->expect_value  = 10;
		options->hitlist_size = 500;
		options->two_pass_method  = FALSE;
		options->multiple_hits_only  = TRUE;
		options->number_of_bits  = 0.0;
		options->dropoff_1st_pass  = 7;
		options->dropoff_2nd_pass  = 10;
		options->matrix  = StringSave("BLOSUM62");
		options->old_stats  = FALSE;
		options->wordsize  = 3;
		options->penalty  = 0;
		options->reward  = 0;
		options->gap_decay_rate = 0.5;
		options->gap_prob = 0.5;
		options->no_check_score  = TRUE;
		if (gapped)
		{
			options->two_pass_method = FALSE;
			options->multiple_hits_only  = TRUE;
			options->dropoff_2nd_pass  = options->dropoff_1st_pass;
			options->gap_decay_rate = 0.1;
			options->gap_prob = 1.0;
		}

		options->gap_open  = 11;
		options->gap_extend  = 1;
                options->decline_align = INT2_MAX;
		options->gap_x_dropoff  = 15;
		options->gap_x_dropoff_final  = 25;
		options->gap_trigger  = 22.0;

		if (StringICmp(progname, "blastp") == 0)
		{
			options->e2 = BLAST_SMALLEST_EVALUE;
			options->threshold_second = 11;
		}
		else if (StringICmp(progname, "blastx") == 0)
		{
			options->e2 = 1.0;
			options->genetic_code = 1;
			options->threshold_second = 12;
			options->do_sum_stats = TRUE;
			options->strand_option  = BLAST_BOTH_STRAND;
		}

               else if ( StringICmp(progname, "tblastn") == 0
			 || StringICmp(progname, "psitblastn") == 0 )
		{
			options->e2 = 1.0;
			options->db_genetic_code = 1;
			options->threshold_second = 13;
			options->do_sum_stats = TRUE;
		}
		else if (StringICmp(progname, "tblastx") == 0)
		{
			options->e2 = BLAST_SMALLEST_EVALUE;
			options->genetic_code = 1;
			options->db_genetic_code = 1;
			options->threshold_second = 13;
			options->gap_open  = 0;
			options->gap_extend  = 0;
			options->gap_x_dropoff  = 0;
			options->gap_x_dropoff_final  = 0;
			options->gapped_calculation = FALSE;
			options->do_sum_stats = TRUE;
			options->strand_option  = BLAST_BOTH_STRAND;
			options->hsp_num_max = 100;
		}
	}

	return options;
}

/*
	Delete the Options structure.
*/
BLAST_OptionsBlkPtr LIBCALL 
BLASTOptionDelete(BLAST_OptionsBlkPtr options)
{
    if (options == NULL)
        return NULL;
    
    if (options->matrix != NULL)
        MemFree(options->matrix);
    
    if (options->program_name != NULL)
        MemFree(options->program_name);
    
    MemFree(options->filter_string);

    MemFree(options->gifile);

    options = MemFree(options);
    return options;
}


/*
	Validate the Options structure.  If an invalid option is found,
	call BLASTOptionDelete and issue an error message.
*/
BLAST_OptionsBlkPtr LIBCALL 
BLASTOptionValidate(BLAST_OptionsBlkPtr options, CharPtr progname)

{
	Int2 status;
	ValNodePtr error_return=NULL;

	status = BLASTOptionValidateEx(options, progname, &error_return);

	if (status != 0)
		options = BLASTOptionDelete(options);
	
	BlastErrorPrint(error_return);
	BlastErrorChainDestroy(error_return);

	return options;
}

/*
	Validate the Options structure.  If an invalid option is found,
	call BLASTOptionDelete and issue an error message.
*/
Int2 LIBCALL 
BLASTOptionValidateEx (BLAST_OptionsBlkPtr options, CharPtr progname, ValNodePtr PNTR error_return)

{
	Int2 status=0;

	if (options->hitlist_size < 1)
	{
		BlastConstructErrorMessage("BLASTOptionValidateEx", "No hits are being saved", 1, error_return);
		return 1;
	}

	if (options->expect_value <= 0.0 && options->cutoff_s == 0)
	{
		BlastConstructErrorMessage("BLASTOptionValidateEx", "expect value must be greater than zero", 1, error_return);
		return 1;
	}

	if (options->wordsize <= 0)
	{
		BlastConstructErrorMessage("BLASTOptionValidateEx", "wordsize must be non-zero", 1, error_return);
		return 1;
	}

	if (StringICmp(progname, "blastn") == 0)
	{
               if (options->penalty >= 0)
               {
                       BlastConstructErrorMessage("BLASTOptionValidateEx", "BLASTN penalty must be negative", 1, error_return);
                       return 1;
               }

               if (options->is_megablast_search && options->wordsize < 8) {
                  BlastConstructErrorMessage("BLASTOptionValidateEx", 
                     "Wordsize must be 8 or greater", 1, error_return);
                  return 1;
               } else if (options->wordsize < 7) {
                  BlastConstructErrorMessage("BLASTOptionValidateEx", "Wordsize must be 7 or greater", 1, error_return);
                  return 1;
               }

		if (options->threshold_second != 0)
		{
			BlastConstructErrorMessage("BLASTOptionValidateEx", "non-zero threshold not permitted with blastn", 1, error_return);
			return 1;
		}

		if (options->two_pass_method == TRUE || options->number_of_bits != 0.0)
		{
			BlastConstructErrorMessage("BLASTOptionValidateEx", "two-passes not available for blastn", 1, error_return);
			return 1;
		}

		if ((options->strand_option | BLAST_BOTH_STRAND) == 0)
		{
			BlastConstructErrorMessage("BLASTOptionValidateEx", "invalid strand specified", 1, error_return);
			return 1;
		}

/*
		if (options->multiple_hits_only == TRUE)
		{
			BlastConstructErrorMessage("BLASTOptionValidateEx", "multiple hits not available for blastn", 1, error_return);
			return 1;
		}
*/
	}
	else
	{
		if (StringICmp(progname, "blastx") == 0 || StringICmp(progname, "tblastx") == 0)
		{
			if ((options->strand_option | BLAST_BOTH_STRAND) == 0)
			{
				BlastConstructErrorMessage("BLASTOptionValidateEx", "invalid strand specified", 1, error_return);
				return 1;
			}
		}
		if (options->wordsize < 2 || options->wordsize > 3)
		{
			BlastConstructErrorMessage("BLASTOptionValidateEx", "Valid wordsize range is 2 to 3", 1, error_return);
			return 1;
		}
		if (options->threshold_second == 0)
		{
			BlastConstructErrorMessage("BLASTOptionValidateEx", "non-zero threshold required", 1, error_return );
			return 1;
		}
		if (options->penalty != 0 || options->reward != 0)
		{
			BlastConstructErrorMessage("BLASTOptionValidateEx", "penalty or reward can only be non-zero for blastn", 1, error_return);
			return 1;
		}

		if (StringICmp(progname, "tblastx") == 0)
		{
			if (options->gapped_calculation == TRUE)
			{
				BlastConstructErrorMessage("BLASTOptionValidateEx", "gapped calculations not available with tblastx", 1, error_return);
				return 1;
			}
		}
		
		if (options->gapped_calculation == TRUE)
		{
			status = BlastKarlinBlkGappedCalcEx(NULL, options->gap_open, options->gap_extend, options->decline_align, options->matrix, error_return);
		}
	}

        if (options->isPatternSearch && (!options->gapped_calculation))
	    {
		BlastConstructErrorMessage("BLASTOptionValidateEx", "PHI-BLAST cannot use ungapped alignments", 1, error_return);
		return 1;
	    }

        if (options->tweak_parameters && (!options->gapped_calculation))
	    {
		BlastConstructErrorMessage("BLASTOptionValidateEx", "parameter adjustment not supported with ungapped alignments", 1, error_return);
		return 1;
	    }

        if (options->smith_waterman && (!options->gapped_calculation))
	    {
		BlastConstructErrorMessage("BLASTOptionValidateEx", "locally optimal alignments not supported with ungapped alignments", 1, error_return);
		return 1;
	    }
        if ((StringICmp(progname, "psitblastn") != 0) && (options->CheckpointFileName !=NULL))
	  {
              BlastConstructErrorMessage("BLASTOptionValidateEx",
                                         "-R option is available only for psitblastn", 1, error_return);
              return 1;
	  }

	return status;
}

/*
	Changes the matrix value to the one given and sets the 
	default parameters for that Matrix. 
*/

Int2 LIBCALL 
BLASTOptionSetGapParams (BLAST_OptionsBlkPtr options, 
                         CharPtr matrix_name, Int4 open, Int4 extended)

{
    Boolean found_matrix=FALSE, threshold_set=FALSE;
    
    if (options == NULL || matrix_name == NULL)
        return -1;
    
    /* blastn is different. */
    if (StringICmp("blastn", options->program_name) == 0) {
        if (!options->is_megablast_search) {
           options->gap_open  = 5;
           options->gap_extend  = 2;
        } else {
           options->gap_open = options->gap_extend = 0;
        }
        return 0;
    }
    
    if (StringICmp(matrix_name, "BLOSUM62") == 0) {
        options->gap_open  = 11;
        options->gap_extend  = 1;
        options->window_size = 40;
        options->threshold_second = 11;
        found_matrix = TRUE;
        threshold_set = TRUE;
    } else if (StringICmp(matrix_name, "BLOSUM45") == 0) {
        options->gap_open  = 14;
        options->gap_extend  = 2;
        options->window_size = 60;
        options->threshold_second = 14;
        found_matrix = TRUE;
        threshold_set = TRUE;
    } else if (StringICmp(matrix_name, "BLOSUM50") == 0) {
        options->gap_open  = 13;
        options->gap_extend  = 2;
        found_matrix = TRUE;
    } else if (StringICmp(matrix_name, "PAM250") == 0) {
        options->gap_open  = 14;
        options->gap_extend  = 2;
        found_matrix = TRUE;
    } else if (StringICmp(matrix_name, "BLOSUM62_20") == 0) {
        options->gap_open  = 100;
        options->gap_extend  = 10;
        options->threshold_second = 100;
        found_matrix = TRUE;
    } else if (StringICmp(matrix_name, "BLOSUM90") == 0) {
        options->gap_open  = 10;
        options->gap_extend  = 1;
        found_matrix = TRUE;
    } else if (StringICmp(matrix_name, "BLOSUM80") == 0) {
        options->gap_open  = 10;
        options->gap_extend  = 1;
        options->window_size = 25;
        options->threshold_second = 12;
        found_matrix = TRUE;
        threshold_set = TRUE;
    } else if (StringICmp(matrix_name, "PAM30") == 0) {
        options->gap_open  = 9;
        options->gap_extend  = 1;
        options->window_size = 15;
        options->threshold_second = 16;
        found_matrix = TRUE;
        threshold_set = TRUE;
    } else if (StringICmp(matrix_name, "PAM70") == 0) {
        options->gap_open  = 10;
        options->gap_extend  = 1;
        options->window_size = 20;
        options->threshold_second = 14;
        found_matrix = TRUE;
        threshold_set = TRUE;
    }
    
    if (open)
        options->gap_open  = open;
    if (extended)
        options->gap_extend  = extended;
    
    if (matrix_name) {
        if (options->matrix)
            MemFree(options->matrix);
        options->matrix = StringSave(matrix_name);
    }
    
    if (!found_matrix)
        return -1;
    
    if (threshold_set) {
        if (StringICmp(options->program_name, "blastx") == 0) {
            options->threshold_second++;

        } else if (StringICmp(options->program_name, "tblastn") == 0
                   || StringICmp(options->program_name, "tblastx") == 0
                   || StringICmp(options->program_name, "psitblastn") == 0) {
            options->threshold_second += 2;
        }
    }
    return 0;
}

/*
  This function obtains the sequence from a BioseqPtr in ASCII alphabet.
  The return value is a Uint1Ptr containing the sequence, the Int4Ptr
  length inidcates the length of the seqeunce.
*/	

Uint1Ptr
BlastGetSequenceFromBioseq (BioseqPtr bsp, Int4Ptr length)

{
	Int4 index;
	SeqPortPtr spp;
	Uint1 residue;
	Uint1Ptr sequence;

	*length = 0;
	if (bsp == NULL)
		return NULL;

	sequence = MemNew((1+bsp->length)*sizeof(Uint1));
	if (sequence == NULL)
		return NULL;

        if (ISA_na(bsp->mol))
        	spp = SeqPortNew(bsp, 0, -1, Seq_strand_plus, Seq_code_iupacna);
	else
        	spp = SeqPortNew(bsp, 0, -1, Seq_strand_unknown, Seq_code_ncbieaa);

	index=0;
	while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF)
	{
		if (residue == SEQPORT_VIRT)
			continue;
		sequence[index] = residue;
		index++;
	}
	sequence[index] = NULLB;
	spp = SeqPortFree(spp);

	*length = index;
	return sequence;
}


/*
	Adjusts the length and number of sequences in a database according
	to the gi_list or seqIdPtr list given.
*/

Boolean
BlastAdjustDbNumbers (ReadDBFILEPtr rdfp, Int8Ptr db_length, Int4Ptr db_number, SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, OIDListPtr oidlist, BlastDoubleInt4Ptr PNTR gi_list_pointers, Int4 gi_list_total)

{
	Int4 count, ordinal_id;
        Int8	db_length_private;
	SeqIdPtr sip;

		
	count = 0;
	db_length_private = 0;

	if (seqid_list)
	{
		sip = seqid_list;
		while (sip)
		{
			ordinal_id = SeqId2OrdinalId(rdfp, sip);
			if (ordinal_id > 0)
				count++;
			sip = sip->next;
		}
	}
	else if (oidlist) {

	    Uint4	mask, base=0, maskindex, i;
	    Uint4	total_mask = oidlist->total/MASK_WORD_SIZE + 1;

	    maskindex = 0;

	    while (maskindex < total_mask) {
		/* for each long-word mask */
		mask = Nlm_SwapUint4(oidlist->list[maskindex]);
		i = 0;
		while (mask) {
		    if (mask & (((Uint4)0x1)<<(MASK_WORD_SIZE-1))) {
			count++;
			db_length_private +=
			    readdb_get_sequence_length(rdfp, base + i);
		    }
		    mask <<= 1;
		    i++;
		}
		maskindex++;
		base += MASK_WORD_SIZE;
	    }
	}

	*db_length = db_length_private;
	*db_number = count;

	return TRUE;
}

/*
	Deletes only the BlastGiListPtr, not
	the associated arrays.
*/
BlastGiListPtr 
BlastGiListDestruct(BlastGiListPtr blast_gi_list, Boolean contents)

{
	if (blast_gi_list == NULL)
		return NULL;

	if (contents)
	{ /* On the main thread.  Deallocate contents. */
		if (blast_gi_list->gilist_not_owned == FALSE)
			MemFree(blast_gi_list->gi_list);
		MemFree(blast_gi_list->gi_list_pointer);
	}

	return MemFree(blast_gi_list);
}

/*
	Allocates BlastGiListPtr.  The caller still owns
	the gi_list and must delete it.
*/

BlastGiListPtr 
BlastGiListNew(BlastDoubleInt4Ptr gi_list, BlastDoubleInt4Ptr PNTR gi_list_pointers, Int4 total)

{
	BlastGiListPtr blast_gi_list;

	if (gi_list == NULL || total == 0)
		return NULL;

	blast_gi_list = MemNew(sizeof(BlastGiList));
	blast_gi_list->gi_list = gi_list;
	blast_gi_list->gi_list_pointer = gi_list_pointers;
	blast_gi_list->total = total;
	blast_gi_list->current = 0;
	
	return blast_gi_list;
}

/* This is a simple callback comparison function for sorting */
static int LIBCALLBACK
blast_double_int_compare(VoidPtr v1, VoidPtr v2)

{
	BlastDoubleInt4Ptr h1, h2;
	BlastDoubleInt4Ptr *hp1, *hp2;

	hp1 = (BlastDoubleInt4Ptr PNTR) v1;
	hp2 = (BlastDoubleInt4Ptr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (h1->ordinal_id < h2->ordinal_id)
		return -1;
	if (h1->ordinal_id > h2->ordinal_id)
		return 1;

	return 0;
}
static int LIBCALLBACK
blast_double_int_gi_compare(VoidPtr v1, VoidPtr v2)

{
	BlastDoubleInt4Ptr h1, h2;
	BlastDoubleInt4Ptr *hp1, *hp2;

	hp1 = (BlastDoubleInt4Ptr PNTR) v1;
	hp2 = (BlastDoubleInt4Ptr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (h1->gi < h2->gi)
		return -1;
	if (h1->gi > h2->gi)
		return 1;

	return 0;
}

/* The function below returns an intersection of the two gi lists, if the first
   list is non-empty. However it returns the second gi list, if the first list
   is empty. 
*/
static
BlastGiListPtr MergeBlastGiLists(BlastGiListPtr gi_list, BlastGiListPtr
                                 gi_list_1)
{
    Int4 index, index1, gi, new_total = 0;
    BlastDoubleInt4Ptr new_gi_list;
    BlastDoubleInt4Ptr *gi_list_pointers, *gi_list_1_pointers;

    gi_list_1_pointers = gi_list_1->gi_list_pointer;

    /* If first list is empty, sort second list by ordinal ids 
       and return it */
    if (gi_list == NULL) {
       HeapSort(gi_list_1_pointers, gi_list_1->total, sizeof(BlastDoubleInt4Ptr PNTR), blast_double_int_compare);
       return gi_list_1;
    }

    /* Sort both lists by gis */
    HeapSort(gi_list_1_pointers, gi_list_1->total, sizeof(BlastDoubleInt4Ptr PNTR), blast_double_int_gi_compare);
   
    gi_list_pointers = gi_list->gi_list_pointer;
    HeapSort(gi_list_pointers, gi_list->total, sizeof(BlastDoubleInt4Ptr PNTR), blast_double_int_gi_compare);

    for (index=0, index1=0; index<gi_list_1->total; index++) {
        gi = gi_list_1_pointers[index]->gi;
        for ( ; index1<gi_list->total && 
                  gi_list_pointers[index1]->gi < gi; 
              index1++);
        if (index1<gi_list->total && gi_list_pointers[index1]->gi == gi)
            gi_list_1_pointers[new_total++] = gi_list_1_pointers[index];
        else
            gi_list_1_pointers[index] = NULL;
    }


    new_gi_list = (BlastDoubleInt4Ptr)
        MemNew(new_total*sizeof(BlastDoubleInt4));

    for (index=0; index<new_total; index++) {
        MemCpy(&(new_gi_list[index]), gi_list_1_pointers[index], 
               sizeof(BlastDoubleInt4));
        gi_list_1_pointers[index] = &(new_gi_list[index]);
    }

    MemFree(gi_list_1->gi_list);
    gi_list_1->gi_list = new_gi_list;
    gi_list_1->total = new_total;

    /* Now re-sort by ordinal ids */
    HeapSort(gi_list_1_pointers, gi_list_1->total, sizeof(BlastDoubleInt4Ptr PNTR), blast_double_int_compare);
    /* Don't need the first list any more */
    BlastGiListDestruct(gi_list, TRUE);

    return gi_list_1;
} 

static
BlastDoubleInt4Ptr CombineDoubleInt4Lists(BlastDoubleInt4Ptr list1, Int4Ptr new_total,
                                          BlastDoubleInt4Ptr list2, Int4 total2)
{
   BlastDoubleInt4Ptr new_list;
   Int4 total1;

   if (list1 == NULL) {
      *new_total = total2;
      return list2;
   }

   total1 = *new_total;

   new_list = (BlastDoubleInt4Ptr) 
      Realloc(list1, (total1+total2)*sizeof(BlastDoubleInt4));
   if (!new_list) {
      ErrPostEx(SEV_WARNING, 0, 0, "Failed to reallocate a gi list");
      MemFree(list2);
      return list1;
   }
   
   MemCpy(&(new_list[total1]), list2, total2*sizeof(BlastDoubleInt4));
   MemFree(list2);
   *new_total = total1 + total2;
   
   return new_list;
}

/* The following function creates ordinal id lists from gi lists
   for each of the databases in the readdb chain separately;
   Function returns the union of all gi lists, which needs to be
   saved for formatting */
static
BlastGiListPtr BlastDbGiListToOidList(ReadDBFILEPtr rdfp_chain) 
{
   Int4	gi_list_total=0, total, i, start, maxoid, num_gis, index;
   BlastDoubleInt4Ptr	gi_list=NULL, full_gi_list = NULL;
   BlastDoubleInt4Ptr *gi_list_pointers;
   OIDListPtr	oidlist;
   ReadDBFILEPtr  rdfp;
   Int4		virtual_mask_index, virtual_oid;
   Uint4	virtual_oid_bit;
   BlastGiListPtr blast_gi_list = NULL;
         
   for (rdfp = rdfp_chain; rdfp; rdfp = rdfp->next) {
      if (!rdfp->gifile)
         continue;

      gi_list = GetGisFromFile(rdfp->gifile, &num_gis);
      
      if (gi_list) {
         maxoid = 0;
         for (index=0, i=0; i < num_gis; i++) {
            gi_list[index].ordinal_id = readdb_gi2seq(rdfp, gi_list[i].gi, &start);
            if (gi_list[index].ordinal_id >= 0) {
               gi_list[index].gi = gi_list[i].gi;
               gi_list[index].start = start;
               maxoid = MAX(maxoid, gi_list[index].ordinal_id);
               index++;
            }
         }
         num_gis = index;


         /* allocate space for mask for virtual database */
         oidlist = (OIDListPtr) MemNew(sizeof(OIDList));
         oidlist->total = maxoid + 1;
         total = maxoid/MASK_WORD_SIZE + 2;
         oidlist->list = (Uint4Ptr) MemNew (total*sizeof(Int4));
         oidlist->memory = oidlist->list;

         for (i=0; i < num_gis; i++) {
            /* populate the mask */
            virtual_oid = gi_list[i].ordinal_id - gi_list[i].start;
            
            if (virtual_oid >= 0) {
               virtual_mask_index = virtual_oid/MASK_WORD_SIZE;
               virtual_oid_bit = 0x1 << (MASK_WORD_SIZE - 1 - virtual_oid % MASK_WORD_SIZE);
               oidlist->list[virtual_mask_index] |= virtual_oid_bit;
            }
         }
         for (i=0; i<total; i++) {
            oidlist->list[i] = Nlm_SwapUint4(oidlist->list[i]);
         }

         rdfp->oidlist = oidlist;
         full_gi_list = CombineDoubleInt4Lists(full_gi_list, &gi_list_total, 
                                               gi_list, num_gis);
      }
   }
   
   if (gi_list && gi_list_total > 0) {
      gi_list_pointers = (BlastDoubleInt4Ptr *) 
         MemNew(gi_list_total*sizeof(BlastDoubleInt4Ptr));
      for (i=0; i<gi_list_total; i++)
         gi_list_pointers[i] = &(full_gi_list[i]);
      HeapSort(gi_list_pointers, gi_list_total, sizeof(BlastDoubleInt4Ptr PNTR), blast_double_int_gi_compare);
      blast_gi_list = BlastGiListNew(full_gi_list, gi_list_pointers, gi_list_total);
      blast_gi_list->gilist_not_owned = FALSE;
   }
   return blast_gi_list;

}

static Int4 BlastGiListToOid(BlastGiListPtr blast_gi_list, Int4 gi, 
                             Int4Ptr start, Int4Ptr first_index)
{
   BlastDoubleInt4Ptr *gi_list_pointers = blast_gi_list->gi_list_pointer;
   Int4 total = blast_gi_list->total;
   Int4 index;
   Boolean found = FALSE;

   for (index = *first_index; index < total &&
           gi > gi_list_pointers[index]->gi; index++);
      
   if (index < total && gi == gi_list_pointers[index]->gi) {
      found = TRUE;
      *start = gi_list_pointers[index]->start;
   }
   *first_index = index;
   if (found)
      return gi_list_pointers[index]->ordinal_id;
   else 
      return -1;
}

void
BlastProcessGiLists(BlastSearchBlkPtr search, BLAST_OptionsBlkPtr options,
                    BlastDoubleInt4Ptr gi_list, Int4Ptr gi_list_size)
{
   Int4		i, maxoid, virtual_oid, total, start;
   Int4         gi_list_total = *gi_list_size;
   OIDListPtr	oidlist, alias_oidlist;
   Boolean	use_private_gilist = FALSE, done;
   Int4		mask_index, virtual_mask_index;
   Uint4	oid_bit, virtual_oid_bit;
   ValNodePtr	vnp;
   ReadDBFILEPtr rdfp;
   BlastGiListPtr db_gi_list;
   BlastDoubleInt4Ptr PNTR gi_list_pointers;
   
   /* Create individual ordinal id masks for those databases 
      which have gi lists */
   db_gi_list = BlastDbGiListToOidList(search->rdfp);
   
   /* non-NULL gi_list means that standalone program called the function */
   /* non-NULL options->gilist means that server got this gilist from client */
   if(options->gilist) {
      /* translate list of gis from ValNodePtr to BlastDoubleInt4Ptr */
      
      gi_list = MemNew(ValNodeLen(options->gilist) * sizeof(BlastDoubleInt4));
      
      for (vnp=options->gilist, i=0; vnp; vnp = vnp->next, ++i) {
         gi_list[i].gi = vnp->data.intvalue;
      }
      gi_list_total = i;
   }
   
   /* Using "options->gifile" file and gi_list,
      construct new gi_list with all needed gis */
   
   if (options->gifile && StringCmp(options->gifile, "")) {
      Int4	gi_list_total_2;
      BlastDoubleInt4Ptr	gi_list_2, tmptr;
      Int4	size = sizeof(BlastDoubleInt4);
      
      gi_list_2 = GetGisFromFile(options->gifile, &gi_list_total_2);
      
      /* replace or append this list to main one */
      if (gi_list && gi_list_2) {
         /* append */
         tmptr = MemNew((gi_list_total+gi_list_total_2)*size);
         MemCpy(tmptr, gi_list, gi_list_total * size);
         MemCpy(tmptr+gi_list_total, gi_list_2, gi_list_total_2 * size);
         
         MemFree(gi_list);
         MemFree(gi_list_2);
         
         gi_list = tmptr;
         gi_list_total += gi_list_total_2;
      } else if (gi_list_2) {
         /* replace */
         gi_list = gi_list_2;
         gi_list_total = gi_list_total_2;
      }
   }
   
   if (gi_list) {
      /* transform the list into OID mask */
      use_private_gilist = TRUE;
      
      gi_list_pointers = 
         Nlm_Malloc(gi_list_total*sizeof(BlastDoubleInt4Ptr));
      for (i=0; i < gi_list_total; i++)
         gi_list_pointers[i] = &(gi_list[i]);
      if (db_gi_list)
         HeapSort(gi_list_pointers, gi_list_total,
                  sizeof(BlastDoubleInt4Ptr PNTR),
                  blast_double_int_gi_compare);
      
      maxoid = 0;
      if (options->gilist_already_calculated == TRUE)
         { /* gi to ordinalID already done, probably for neighboring software. */
            for (i=0; i < gi_list_total; i++) {
               /* get virtual OID and start position for the database this gi is in */
               maxoid = MAX(maxoid, gi_list[i].ordinal_id);
               gi_list_pointers[i] = &(gi_list[i]);
            }
         }
      else
         {
            Int4 first_index = 0;
            for (i=0; i < gi_list_total; i++) {
               /* get virtual OID and start position for the database 
                  this gi is in */
               gi_list[i].ordinal_id = -1;
               if (db_gi_list)
                  gi_list[i].ordinal_id = 
                     BlastGiListToOid(db_gi_list, gi_list[i].gi, 
                                      &start, &first_index);
               if (gi_list[i].ordinal_id == -1)
                  gi_list[i].ordinal_id = readdb_gi2seq(search->rdfp, gi_list[i].gi, &start);
               gi_list[i].start = start;
               maxoid = MAX(maxoid, gi_list[i].ordinal_id);
            }
         }
      
      /* allocate space for mask for virtual database */
      oidlist = (OIDListPtr) MemNew(sizeof(OIDList));
      oidlist->total = maxoid + 1;
      total = maxoid/MASK_WORD_SIZE + 2;
      oidlist->list = (Uint4Ptr) MemNew (total*sizeof(Int4));
      oidlist->memory = oidlist->list;
      
      /* Merge this list with virtual database (OID list) */
      
      for (i=0; i < gi_list_total; i++) {
         /* get start possition in that database */
         start = gi_list[i].start;
         
         /* find out if this is an mask database */
         
         done = FALSE;
         rdfp = search->rdfp;
         
         alias_oidlist = NULL;
         while (rdfp && !done) {
            if (rdfp->start == start) {
               alias_oidlist = rdfp->oidlist;
               done = TRUE;
            } else if (rdfp->start > start) {
               done = TRUE;
            } else {
               rdfp = rdfp->next;
            }
         }
         
         /* populate the mask */
         virtual_oid = gi_list[i].ordinal_id;
         
         if (virtual_oid >= 0) {
            virtual_mask_index = virtual_oid/MASK_WORD_SIZE;
            if (alias_oidlist) {
               mask_index = (virtual_oid - start) / MASK_WORD_SIZE;
               oid_bit = 0x1 << (MASK_WORD_SIZE - 1 - (virtual_oid-start) % MASK_WORD_SIZE);
            }
            
            virtual_oid_bit = 0x1 << (MASK_WORD_SIZE - 1 - virtual_oid % MASK_WORD_SIZE);
            
            if ((!alias_oidlist) ||
		        (alias_oidlist && virtual_oid < oidlist->total &&
                 alias_oidlist->list && 
                 (Nlm_SwapUint4(alias_oidlist->list[mask_index])) & oid_bit)) { 
               oidlist->list[virtual_mask_index] |= virtual_oid_bit;
            }
         }
      }
      
      for (i=0; i<total; i++) {
         oidlist->list[i] = Nlm_SwapUint4(oidlist->list[i]);
      }
      /* The individual database oid masks are not needed any more */
      for (rdfp = search->rdfp; rdfp; rdfp = rdfp->next) {
         rdfp->oidlist = OIDListFree(rdfp->oidlist);
      }
      search->rdfp->oidlist = oidlist;
      
      /* in this case, the case when we have .gil file, the only database mask
         should be used in Blast Search, so set number of sequences for the first
         database in rdfp list to 0 avoiding search this real database: */
      search->rdfp->num_seqs = 0;
      
      /* keep list of gi's (needed for formating) */
      if (options->sort_gi_list && !db_gi_list)
         HeapSort(gi_list_pointers, gi_list_total, sizeof(BlastDoubleInt4Ptr PNTR), blast_double_int_compare);
      search->thr_info->blast_gi_list = BlastGiListNew(gi_list, gi_list_pointers, gi_list_total);
      search->thr_info->blast_gi_list->gilist_not_owned =
         options->gilist_already_calculated;
      *gi_list_size = gi_list_total;
   } else {
      /* Ok, we do not have a gi-list specified, but maybe
         we have an a mask database in the list of databases,
         we need to create one mask for all such databases */
      OIDListPtr		virtual_oidlist = NULL;
      Int4		final_virtual_db_seq=0, final_db_seq=0;
      Int4		mask, oid, virtual_oid, maskindex,
         virtual_mask_index, total_virtual_mask,
         base;
      Uint4		virtual_oid_bit;
      ReadDBFILEPtr       mask_rdfp = NULL;
      
      rdfp = search->rdfp;
      while (rdfp) {
         
         final_virtual_db_seq = rdfp->stop;
         if (!rdfp->oidlist)
            final_db_seq = rdfp->stop;
         rdfp = rdfp->next;
      }
      
      rdfp = search->rdfp;
      while (rdfp) {
         if (rdfp->oidlist) {
            if (mask_rdfp == NULL)
               mask_rdfp = rdfp;
            if (!virtual_oidlist) {
				/* create new oidlist for virtual database */
               virtual_oidlist = (OIDListPtr) MemNew(sizeof(OIDList));
               virtual_oidlist->total = final_virtual_db_seq + 1;
               total_virtual_mask = final_virtual_db_seq/MASK_WORD_SIZE + 2;
               virtual_oidlist->list = (Uint4Ptr) MemNew (total_virtual_mask*sizeof(Int4));
                                /* use the 'memory' field to OIDListFee this memory after use */
               virtual_oidlist->memory = virtual_oidlist->list;
            }
            /* Now populate the virtual_oidlist */
            maskindex = 0;
            base = 0;
            
            while (maskindex < (rdfp->oidlist->total/MASK_WORD_SIZE +1)) {
				/* for each long-word mask */
               mask = Nlm_SwapUint4(rdfp->oidlist->list[maskindex]);
               
               i = 0;
               while (mask) {
                  if (mask & (((Uint4)0x1)<<(MASK_WORD_SIZE-1))) {
                     oid = base + i;
                     virtual_oid = oid + rdfp->start;
                     
                     virtual_mask_index = virtual_oid/MASK_WORD_SIZE;
                     virtual_oid_bit = 0x1 << (MASK_WORD_SIZE - 1 - virtual_oid % MASK_WORD_SIZE);
                     virtual_oidlist->list[virtual_mask_index] |= virtual_oid_bit;
                  }
                  mask <<= 1;
                  i++;
               }
               maskindex++;
               base += MASK_WORD_SIZE;
            }
            
            /* free old mask */
            rdfp->oidlist = OIDListFree(rdfp->oidlist);
         }
         rdfp = rdfp->next;
      }
      if (virtual_oidlist) {
         for (i=0; i<total_virtual_mask; i++) {
            virtual_oidlist->list[i] = Nlm_SwapUint4(virtual_oidlist->list[i]);
         }
         mask_rdfp->oidlist = virtual_oidlist;
      }
   }
   /* Save the gi lists from databases; if blast_gi_list exists,
      then take intersection */
   if (db_gi_list) 
      search->thr_info->blast_gi_list = 
         MergeBlastGiLists(search->thr_info->blast_gi_list, db_gi_list);
}


#define POSIT_PERCENT 0.05
#define POSIT_NUM_ITERATIONS 10

#define Xchar   21    /*character for low-complexity columns*/


/* This should be more than large enough for any alphabet. */
#define POSIT_MAX_ALPHABET_SIZE 30

/*Compute probabilities for each score in posMatrix,
also sets minScore and maxScore*/
static BLAST_ScoreFreqPtr 
BlastComputeProbs(BlastMatrixRescalePtr matrix_rescale, Boolean position_dependent)
{
   Int2 alphabet_total;
   Int4 c;  /*index on characters*/
   Int4 p;  /*index on positions*/
   Int4 s;  /*index on scores */
   Int4 dim1, dim2;
   BLAST_Score score_min, score_max;
   BLAST_ScoreFreqPtr sfp;
   Int4 numberOfScores; /* number of distinct scores*/
   Int4 score;  /*one score in the matrix*/
   Nlm_FloatHi increment;  /*Increment in probability due to one score*/
   Int4 effectiveLength;
   Int4Ptr *matrix;
   Uint1 std_alphabet[POSIT_MAX_ALPHABET_SIZE];

   alphabet_total = BlastGetStdAlphabet(Seq_code_ncbistdaa, std_alphabet, POSIT_MAX_ALPHABET_SIZE);
   if (alphabet_total <= 0)
	return NULL;
    
   matrix = matrix_rescale->matrix;
   score_min = 0;
   score_max = 0;
   if(position_dependent)
   {
	dim1 = matrix_rescale->query_length;
	dim2 = alphabet_total;
   }
   else
   {
	dim1 = alphabet_total;
	dim2 = alphabet_total;
   }

   effectiveLength = 0;
   for (p = 0; p < matrix_rescale->query_length; p++)
     if (Xchar != matrix_rescale->query[p])
       effectiveLength++;
   for (p = 0; p < dim1; p++)
     if (Xchar != matrix_rescale->query[p])
       for (c = 0; c < dim2; c++) {
	 if (matrix[p][std_alphabet[c]] <= BLAST_SCORE_MIN || matrix[p][std_alphabet[c]] >= BLAST_SCORE_MAX)
		continue;
	 if (matrix[p][std_alphabet[c]] < (score_min))
	   (score_min) = matrix[p][std_alphabet[c]];
	 if (matrix[p][std_alphabet[c]] > (score_max))
	   (score_max) = matrix[p][std_alphabet[c]];
       }

   if((sfp = BlastScoreFreqNew(score_min, score_max)) == NULL) {
       ErrPostEx(SEV_ERROR, 0, 0, 
                 "BlastComputeProbs(): Unable to create Score Frequencies");
       return NULL;
   }

   sfp->obs_min = sfp->score_min;
   sfp->obs_max = sfp->score_max;
   numberOfScores = (sfp->score_max) - (sfp->score_min) + 1;
   for (p = 0; p < dim1; p++)
     if (Xchar != matrix_rescale->query[p])
       for (c = 0; c < dim2; c++) {
	 /*Increment the weight for the score in position [p][std_alphabet[c]] */
	 score = matrix[p][std_alphabet[c]];
	 if (score <= BLAST_SCORE_MIN || score >= BLAST_SCORE_MAX)
		continue;
	 increment =
	   (matrix_rescale->standardProb[std_alphabet[c]]/ effectiveLength);
	 sfp->sprob[score]+= increment;
       }

   sfp->score_avg = 0.0;
   for(s = sfp->score_min; s <= sfp->score_max; s++)
     sfp->score_avg += s*sfp->sprob[s];
   return(sfp);
}

void LIBCALL
updateLambdaK(BlastMatrixRescalePtr matrix_rescale, Boolean position_dependent)
{
  BLAST_ScoreFreqPtr sfp;
 
  if((sfp = BlastComputeProbs(matrix_rescale, position_dependent)) == NULL)
      return;

  BlastKarlinBlkCalc(matrix_rescale->kbp_psi[0], sfp);
  matrix_rescale->kbp_gap_psi[0]->K = (matrix_rescale->kbp_psi[0]->K)*(matrix_rescale->kbp_gap_std[0]->K)/matrix_rescale->K_ideal;
  matrix_rescale->kbp_gap_psi[0]->logK = log(matrix_rescale->kbp_gap_psi[0]->K);
  sfp = BlastScoreFreqDestruct(sfp);
}

Nlm_FloatHi 
BlastScaleMatrix(BlastMatrixRescalePtr matrix_rescale, Boolean position_dependent)
{
   Int4 dim1, dim2;
   Int4 a,c; /*loop indices*/
   Boolean too_high=TRUE, first_time;
   Nlm_FloatHi factor, factor_low=1.0, factor_high=1.0;
   Nlm_FloatHi lambda, new_lambda; /*Karlin-Altschul parameter*/
   Int4 index; /*loop index for binary search*/
   Int4Ptr *private_matrix;
   Int4Ptr *matrix;

   private_matrix = matrix_rescale->private_matrix;
   matrix = matrix_rescale->matrix;


/* Bracket the values. */
   if (position_dependent)
   {
   	dim1 = matrix_rescale->query_length;
   	dim2 = matrix_rescale->alphabet_size;
   }
   else
   {
   	dim1 = matrix_rescale->alphabet_size;
   	dim2 = matrix_rescale->alphabet_size;
   }
   lambda = matrix_rescale->lambda_ideal;

   first_time = TRUE;
   factor = 1.0;
   while (1)
   {
   	for(c = 0; c < dim1; c++)
   	{
       	    for(a = 0; a < dim2; a++)
	    {
		if (private_matrix[c][a] == BLAST_SCORE_MIN)
		{
			matrix[c][a] = BLAST_SCORE_MIN;
		}
		else
		{
			matrix[c][a] = Nlm_Nint(((FloatHi)(factor*private_matrix[c][a]))/POSIT_SCALE_FACTOR);
		}
	    }
        }

        updateLambdaK(matrix_rescale, position_dependent);

        if(matrix_rescale->kbp_psi[0] != NULL)
            new_lambda = matrix_rescale->kbp_psi[0]->Lambda;
        else
            return 0.0;

	if (new_lambda > lambda)
	{
		if (first_time)
		{
			factor_high = 1.0 + POSIT_PERCENT;
			factor = factor_high;
			factor_low = 1.0;
			too_high = TRUE;
			first_time = FALSE;
		}
		else
		{
			if (too_high == FALSE)
				break;
			factor_high += (factor_high-1.0);
			factor = factor_high;
		}
	}
	else if (new_lambda > 0) 
	{
		if (first_time)
		{
			factor_high = 1.0;
			factor_low = 1.0 - POSIT_PERCENT;
			factor = factor_low;
			too_high = FALSE;
			first_time = FALSE;
		}
		else
		{
			if (too_high == TRUE)
				break;
			factor_low += (factor_low-1.0);
			factor = factor_low;
		}
	} else {
           ErrPostEx(SEV_FATAL, 0, 0, "Matrix has positive average score");
           return 0.0;
        }
                
   }

/* binary search for ten times. */
   for (index=0; index<POSIT_NUM_ITERATIONS; index++)
   {
        factor = 0.5*(factor_high+factor_low);
   	for(c = 0; c < dim1; c++)
   	{
       	    for(a = 0; a < dim2; a++)
	    {
		if (private_matrix[c][a] == BLAST_SCORE_MIN)
		{
			matrix[c][a] = BLAST_SCORE_MIN;
		}
		else
		{
			matrix[c][a] = Nlm_Nint(((FloatHi)(factor*private_matrix[c][a]))/POSIT_SCALE_FACTOR);
		}
	    }
   	}
        updateLambdaK(matrix_rescale, position_dependent);

        if(matrix_rescale->kbp_psi[0] != NULL)
            new_lambda = matrix_rescale->kbp_psi[0]->Lambda;
        else
            return 0.0;

	if (new_lambda > lambda)
	{
		factor_low = factor;
	}
	else
	{
		factor_high = factor;
	}
    }

   for(a = 0; a < matrix_rescale->alphabet_size; a++) 
     matrix[dim1][a] = BLAST_SCORE_MIN;

	return factor;
}


/*
	Deallocates only memory for BlastMatrixRescalePtr.
*/
BlastMatrixRescalePtr
BlastMatrixRescaleDestruct (BlastMatrixRescalePtr matrix_rescale)

{
	if (matrix_rescale == NULL)
		return NULL;

	return MemFree(matrix_rescale);
}

/*
	Allocates and fills the BlastMatrixRescalePtr.
*/

BlastMatrixRescalePtr
BlastMatrixRescaleNew(Int4 alphabet_size, Int4 query_length, Uint1Ptr query,  Nlm_FloatHiPtr standardProb, Int4Ptr *matrix, Int4Ptr *private_matrix, BLAST_KarlinBlkPtr *kbp_std, BLAST_KarlinBlkPtr *kbp_psi, BLAST_KarlinBlkPtr *kbp_gap_std, BLAST_KarlinBlkPtr *kbp_gap_psi, Nlm_FloatHi lambda_ideal,  Nlm_FloatHi K_ideal)

{
	BlastMatrixRescalePtr matrix_rescale;

	matrix_rescale = (BlastMatrixRescalePtr) MemNew(sizeof(BlastMatrixRescale));

	matrix_rescale->alphabet_size = alphabet_size;
	matrix_rescale->query_length = query_length;
	matrix_rescale->query = query;
	matrix_rescale->standardProb = standardProb;
	matrix_rescale->matrix = matrix;
	matrix_rescale->private_matrix = private_matrix;
	matrix_rescale->kbp_std = kbp_std;
	matrix_rescale->kbp_psi = kbp_psi;
	matrix_rescale->kbp_gap_std = kbp_gap_std;
	matrix_rescale->kbp_gap_psi = kbp_gap_psi;
	matrix_rescale->lambda_ideal = lambda_ideal;
	matrix_rescale->K_ideal = K_ideal;

	return matrix_rescale;
}

/*
	BlastSeqLocCount counts the number of SeqLoc's 
*/
static Int4 
BlastSeqLocCount (SeqLocPtr mask_slp)

{
        Int4 index=0;
	SeqLocPtr slp;
       
	while (mask_slp)
	{
		slp=NULL;
        	while((slp = SeqLocFindNext(mask_slp, slp))!=NULL)
        	{
			index++;
        	}
		mask_slp = mask_slp->next;
	}

	return index;
}

static Boolean 
BlastIntervalSort (BlastDoubleInt4Ptr *link_ptr, Int4 link_value, Int4 total, int (LIBCALLBACK *callback )PROTO ((Nlm_VoidPtr, Int4 start, Int4 stop)), VoidPtr ptr)

{
	Boolean do_callback, start=TRUE;
	Int4 index, start_pos, stop_pos, largest_stop_pos;

	index=0;
	while (index < total)
	{
		if (start == TRUE)
		{
			start_pos = link_ptr[index]->gi;
			start = FALSE;
			largest_stop_pos = 0;
		}
		else
		{
			/* Keep track of largest stop position. */
			largest_stop_pos = MAX(largest_stop_pos, link_ptr[index]->ordinal_id);
			do_callback = FALSE;
			if (index == total-1)	/* Last one. */
			{
				stop_pos = link_ptr[index]->ordinal_id;
				start = TRUE;
				do_callback = TRUE;
			}
			else if (largest_stop_pos+link_value < link_ptr[index+1]->gi)
			{ /* Check overlap with next one. */
				stop_pos = link_ptr[index]->ordinal_id;
				start = TRUE;
				do_callback = TRUE;
			}
			
			if (do_callback)
			{
				callback(ptr, start_pos, MAX(largest_stop_pos, stop_pos));
			}
			index++;
		}
	}

	return TRUE;
}

static int LIBCALLBACK
list_ptr_compare(VoidPtr v1, VoidPtr v2)

{
	BlastDoubleInt4Ptr h1, h2;
	BlastDoubleInt4Ptr *hp1, *hp2;

	hp1 = (BlastDoubleInt4Ptr PNTR) v1;
	hp2 = (BlastDoubleInt4Ptr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (h1->gi < h2->gi)
		return -1;
	if (h1->gi > h2->gi)
		return 1;

	return 0;
}

int LIBCALLBACK
slp_callback(VoidPtr ptr, Int4 start, Int4 stop)

{
	ValNodePtr *vnpp;


	vnpp = (ValNodePtr PNTR) ptr;	

	ValNodeAddInt(vnpp, 0, start);
	ValNodeAddInt(vnpp, 1, stop);

	return 1;
}


ValNodePtr
BlastSeqLocFillDoubleInt (SeqLocPtr mask_slp, Int4 max_length, Boolean reverse)

{
	return BlastSeqLocFillDoubleIntEx(mask_slp, 0, max_length, reverse, 0);

}

/*
	SeqLocPtr mask_slp: location to be masked
	Int4 full_query_length: length of the query, not just portion being blasted.
	Int4 max_length: length of query being blasted.
	Boolean reverse: do minus strand.
	Int4 offset: offset in full query to portion being blasted.

	the coordinates reported here are in terms of the portion of the query
	being blasted for use by the function that makes up words.
*/

ValNodePtr
BlastSeqLocFillDoubleIntEx (SeqLocPtr mask_slp, Int4 full_query_length, Int4 max_length, Boolean reverse, Int4 offset)

{
	Int4 count, index, start, stop;
	BlastDoubleInt4Ptr list_pri, *list_ptr_pri;
	SeqLocPtr slp;
	ValNodePtr vnp;

	vnp = NULL;
	if (mask_slp == NULL)
	{
		ValNodeAddInt(&vnp, 1, -1);
		ValNodeAddInt(&vnp, 0, max_length);
		return vnp;
	}

	/* use length of segment if full_query_length no set. */
	if (full_query_length == 0)
		full_query_length = max_length;

	count = BlastSeqLocCount (mask_slp);
	list_pri = (BlastDoubleInt4Ptr) MemNew(count*sizeof(BlastDoubleInt4)); 
	list_ptr_pri = (BlastDoubleInt4Ptr PNTR) MemNew(count*sizeof(BlastDoubleInt4Ptr)); 

	index=0;
	while (mask_slp)
	{
		slp=NULL;
        	while((slp = SeqLocFindNext(mask_slp, slp))!=NULL)
        	{
			if (reverse)
			{
				start = full_query_length - 1 - SeqLocStop(slp);
				stop = full_query_length - 1 - SeqLocStart(slp);
			}
			else
			{
              			start = SeqLocStart(slp);
              			stop = SeqLocStop(slp);
			}

			list_pri[index].gi = start - offset;
			list_pri[index].ordinal_id = stop -offset;
			list_ptr_pri[index] = &(list_pri[index]);
			index++;
        	}
		mask_slp = mask_slp->next;
	}

	HeapSort(list_ptr_pri, count, sizeof(BlastHitRangePtr PNTR), list_ptr_compare);

	/* Allows the proper start. */
	ValNodeAddInt(&vnp, 1, -1);
	BlastIntervalSort(list_ptr_pri, 0, count, slp_callback, (VoidPtr) &vnp);

	list_pri = MemFree(list_pri);
	list_ptr_pri = MemFree(list_ptr_pri);

	return vnp;
}

ValNodePtr
BlastSeqLocFillDoubleIntRev (ValNodePtr location, SeqLocPtr mask_slp, Int4 max_length, Int4 full_query_length, Int4 offset)

{
   Int4 count, index, start, stop;
   BlastDoubleInt4Ptr list_pri, *list_ptr_pri;
   SeqLocPtr slp;
   ValNodePtr vnp;
   
   vnp = location;
   if (mask_slp == NULL) {
      if (!vnp) {
	 ValNodeAddInt(&vnp, 1, -1);
	 ValNodeAddInt(&vnp, 0, max_length);
      }
      return vnp;
   }

   /* use length of segment if full_query_length no set. */
   if (full_query_length == 0)
	full_query_length = max_length;

   
   count = BlastSeqLocCount (mask_slp);
   list_pri = (BlastDoubleInt4Ptr) MemNew(count*sizeof(BlastDoubleInt4)); 
   list_ptr_pri = (BlastDoubleInt4Ptr PNTR) MemNew(count*sizeof(BlastDoubleInt4Ptr)); 
   
   index=0;
   while (mask_slp) {
      slp=NULL;
      while((slp = SeqLocFindNext(mask_slp, slp))!=NULL) {
	 start = full_query_length + max_length - SeqLocStop(slp);
	 stop = full_query_length + max_length - SeqLocStart(slp);
	 
	 list_pri[index].gi = start - offset;
	 list_pri[index].ordinal_id = stop - offset;
	 list_ptr_pri[index] = &(list_pri[index]);
	 index++;
      }
      mask_slp = mask_slp->next;
   }
   
   HeapSort(list_ptr_pri, count, sizeof(BlastHitRangePtr PNTR), list_ptr_compare);
   BlastIntervalSort(list_ptr_pri, 0, count, slp_callback, (VoidPtr) &vnp);
   
	list_pri = MemFree(list_pri);
	list_ptr_pri = MemFree(list_ptr_pri);

   return vnp;
}



static Boolean
small(BLASTResultHspPtr a, BLASTResultHspPtr b)
{
  if (a->e_value > b->e_value) return TRUE;
  if (a->e_value < b->e_value) return FALSE;
  if (a->score > b->score) return FALSE;
  if (a->score < b->score) return TRUE;
  if (a->point_back->subject_id > b->point_back->subject_id) return FALSE;
  return TRUE;
}


/*
	this is some sort of HeapSort (or it makes the heap
	as it is used in HeapSort).
*/
static void
BlastHeapify(BLASTHeapPtr which_heap, Int4 position)
{
  Int4 heap_size, index, lim, left_son, small_son;
  BLASTResultHspPtr tmp, PNTR heap;

  heap_size = which_heap->num_in_heap;
  heap = which_heap->heap;
  index = position; lim = heap_size/2;

  while (index < lim) {
    left_son = 2*index + 1;
    if (left_son == heap_size-1)
      small_son = left_son;
    else {
      if (small(heap[left_son],heap[left_son+1])) small_son = left_son;
      else small_son = left_son+1;
    }
/* If heap[small_son] is less significant than heap[index], then
   switch them.  Otherwise exit the loop. 
*/
    if (small(heap[small_son], heap[index])) {
      tmp = heap[index];
      heap[index] = heap[small_son];
      heap[small_son] = tmp;
      index = small_son;
    } else
      break;
  }
}

/*
	Insert a new element into the BLASTHeapPtr list, which
	appears to be ordered (here) from least to most significant.
	the order does not seem to be strictly from least to
	most significant, or is it?
*/
static void
BlastInsertHeap(BLASTHeapPtr which_heap, BLASTResultHspPtr hp)
{
  Int4 heap_size, index, father;
  BLASTResultHspPtr PNTR heap;

  hp->point_back->num_ref+= 1;
  /* Increase heap size by one for new entry. */
  heap_size = (which_heap->num_in_heap+=1);
  heap = which_heap->heap;
  index = heap_size-1; 
 
  while (index > 0) {
    father = (index-1)/2;
    /* If heap[father] is LESS significant than hp, then exit loop. */
    if (small(heap[father], hp)) {
      break;
    }
    /* The 1st time this is called heap[index] is NULL as it's one past
	the last filled in element.  The idea here is obviously to
	move the more significant elements to the end, but don't
	they get out of order then?  
    */
    heap[index] = heap[father];
    index = father;
  }
  /* Fill in new element. */
  heap[index] = hp;
} 

Int4
BlastDeleteHeap(BLASTHeapPtr which_heap, Int4 position)
{
  Int4 last, return_value;
  BLASTResultHspPtr PNTR heap;

  last = (which_heap->num_in_heap-=1);
  heap = which_heap->heap;
  return_value = (heap[position]->point_back->num_ref -= 1);
  if (position != last) {
    heap[position] = heap[last];
    BlastHeapify(which_heap, position);
  }
  return return_value;
}

static void
BlastInsertWholeHeap(BlastSearchBlkPtr search, BLASTHeapPtr which_heap, Int4 cutvalue)
{
  BLASTHeapPtr hp;
  Int4 i;

  hp = (BLASTHeapPtr) MemNew(sizeof(BLASTHeapStruct));
  hp->heap = (BLASTResultHspPtr PNTR) MemNew(sizeof(BLASTResultHspPtr)*search->pbp->hsp_range_max);
  hp->next = which_heap;
  hp->num_of_ref = 0;
  hp->cutvalue = cutvalue;
  if (which_heap->prev) {
    hp->prev = which_heap->prev;
    which_heap->prev->next = hp;
  } else {
    hp->prev = NULL;
    search->result_struct->heap_ptr = hp;
  }
  which_heap->prev = hp;
  hp->num_in_heap = which_heap->num_in_heap;
  for (i = 0; i < which_heap->num_in_heap; i++) {
    hp->heap[i] = which_heap->heap[i];
    which_heap->heap[i]->point_back->num_ref +=1;
  }
}

static void
BlastDeleteWholeHeap(BlastSearchBlkPtr search, BLASTHeapPtr which_heap)
{
  Int4 i;
  if ((which_heap->num_of_ref -= 1) >0) return;
  if (which_heap->next) which_heap->next->prev = which_heap->prev;
  if (which_heap->prev) which_heap->prev->next = which_heap->next;
  else search->result_struct->heap_ptr = which_heap->next;
  for (i = 0; i < which_heap->num_in_heap; i++) {
     which_heap->heap[i]->point_back->num_ref -= 1;
  }
  which_heap->heap = MemFree(which_heap->heap);
  which_heap = MemFree(which_heap);
}

static Int2
BlastPossibleDeleteWholeHeap(BlastSearchBlkPtr search, BLASTHeapPtr PNTR hhp,  BLASTResultHspPtr heap0)
     /* if the deleted hit result a remove of the right end point, 
	return 1 to make the program rerun 
	*/
{
  BLASTHeapPtr hp = *hhp;

  if (heap0 == NULL || hp == NULL)
	return 0;

  if (heap0->back_left && hp->prev && heap0->back_left == hp->prev) {
    heap0->back_left = NULL;
    if ((hp->prev->num_of_ref -= 1) == 0) {
      BlastDeleteWholeHeap(search, hp->prev);
    }
  }
  if (heap0->back_right == hp) {
    heap0->back_right = NULL;
    if ((hp->num_of_ref -= 1)==0) {
      *hhp = hp->next;    
      BlastDeleteWholeHeap(search, hp);
      *hhp = (*hhp)->prev;
      return 1;
    }
  }
  return 0;
}



Int2
BlastInsertList2Heap(BlastSearchBlkPtr search, BLASTResultHitlistPtr result_hitlist)
{
    BLASTResultHspPtr PNTR heap, hsp;
    BLASTResultHspPtr hsp_array;
    Int4 index, hsp_range_max;
    Int4 begin, end, hspcnt, query_length;
    Int2 context;
    Boolean hsp_deleted, new_inserted;
    BLASTHeapPtr hp;

    if (search->pbp->perform_culling == FALSE || 
        search->pbp->mb_params)  /* Culling is turned off. */
        return 3;

    hsp_deleted = new_inserted  = FALSE;  
    hsp_range_max = search->pbp->hsp_range_max;
    hspcnt = result_hitlist->hspcnt;
    hsp_array = result_hitlist->hsp_array;

    if (search->prog_number==blast_type_blastn)
       query_length = search->query_context_offsets[1] - 1;

    for (index = 0; index < hspcnt; index++) {
      hsp = &hsp_array[index];
      if (search->prog_number==blast_type_blastn) {
	 context = hsp->query_offset / query_length;
	 if (context == 0) {
	    begin = hsp->query_offset;
	    end = (hsp->query_offset+hsp->query_length-1);
	 } else {
	    end = 2*query_length - hsp->query_offset;
	    begin = end - hsp->query_length + 1;
	 }
      } else {
	 begin = hsp->query_offset;
	 end = (hsp->query_offset+hsp->query_length-1);
      }
      for (hp = search->result_struct->heap_ptr; hp; hp = hp->next) 
	if (hp->cutvalue >= begin) break;
      if (hp->num_in_heap < hsp_range_max || small(hp->heap[0], hsp)) {
	if (!hp->prev || hp->prev->cutvalue != begin-1) {
	  BlastInsertWholeHeap(search, hp, begin-1);
	}
	hp->prev->num_of_ref +=1;
	hsp->back_left = hp->prev;
      } else hsp->back_left = NULL;
      for (; hp; hp = hp->next) {
	if (end <= hp->cutvalue) break;
	heap = hp->heap;
	if (hp->num_in_heap >= hsp_range_max) {
	  if (small(heap[0], hsp)) {
	    if (BlastPossibleDeleteWholeHeap(search, &hp, heap[0])) continue;
	    if ((heap[0]->point_back->num_ref-=1)==0) 
	      hsp_deleted = TRUE;
	    heap = hp->heap;
	    heap[0] = hsp;
	    hsp->point_back->num_ref +=1;
	    BlastHeapify(hp, 0);
	    new_inserted = TRUE;
	  } 
	} else {
	  BlastInsertHeap(hp, hsp);
	  new_inserted = TRUE;
	} 
      }
      hp = search->result_struct->heap_ptr;
      if (hp->num_in_heap < hsp_range_max || small(hp->heap[0], hsp)) {
	if (end != hp->cutvalue) {
	  BlastInsertWholeHeap(search, hp, end);
	  hp = hp->prev;
	} 
	hp->num_of_ref += 1;
	heap = hp->heap;
	if (hp->num_in_heap >= hsp_range_max) {
	    BlastPossibleDeleteWholeHeap(search, &hp, heap[0]);
	    if ((heap[0]->point_back->num_ref-=1)==0) 
	      hsp_deleted = TRUE;
	    heap = hp->heap;
	    heap[0] = hsp;
	    hsp->point_back->num_ref+=1;
	    BlastHeapify(hp, 0);
	    new_inserted = TRUE; 
	} else {
	  BlastInsertHeap(hp, hsp);
	  new_inserted = TRUE;
	}
	hsp->back_right = hp;
      } else hsp->back_right = NULL;
    }
    if (hsp_deleted) return 1;
    if (new_inserted) return 2;
    return 0;
}

void
BlastFreeHeap(BlastSearchBlkPtr search, BLASTResultHitlistPtr result_hitlist)
{ 
    BLASTResultHspPtr PNTR heap, hsp;
    BLASTResultHspPtr hsp_array;
    Int4 index, hsp_range_max;
    Int4 begin, end, i, hspcnt, query_length;
    Int2 context;
    BLASTHeapPtr hp;

    if (search->pbp->perform_culling == FALSE || 
        search->pbp->mb_params)  /* Culling is turned off. */
        return;

    hsp_range_max = search->pbp->hsp_range_max;
    hspcnt = result_hitlist->hspcnt;
    hsp_array = result_hitlist->hsp_array;

    if (search->prog_number==blast_type_blastn)
       query_length = search->query_context_offsets[1] - 1;
    
    for (index = 0; index < hspcnt; index++) {
      hsp = &hsp_array[index];
      if (search->prog_number==blast_type_blastn) {
	 context = hsp->query_offset / query_length;
	 if (context == 0) {
	    begin = hsp->query_offset;
	    end = (hsp->query_offset+hsp->query_length-1);
	 } else {
	    end = 2*query_length - hsp->query_offset;
	    begin = end - hsp->query_length + 1;
	 }
      } else {
	 begin = hsp->query_offset;
	 end = (hsp->query_offset+hsp->query_length-1);
      }
      if (hsp->back_left) {
	hp = hsp->back_left->next;
	if (BlastPossibleDeleteWholeHeap(search, &hp, hsp)) continue;
      } else {
	for (hp = search->result_struct->heap_ptr; hp; hp = hp->next) {
	  if (hp->cutvalue >= begin) break;
	}
      }
      for (; hp; hp = hp->next) {
	if (hp->cutvalue > end ) break;
	heap = hp->heap;
	for (i = 0; i < hp->num_in_heap; i++) {
	  if (heap[i] == hsp) {
	    BlastDeleteHeap(hp, i);
	    break;
	  }
	}
      }
      hp = hp->prev;
      BlastPossibleDeleteWholeHeap(search, &hp, hsp);
    }
}

/*converts a 1-level list of SeqAligns to a 2-level
list store in a ValNodePtr; the list lastSeqAligns
specifies where to break up seqAlignList */
ValNodePtr convertSeqAlignListToValNodeList(SeqAlignPtr seqAlignList, SeqAlignPtr * lastSeqAligns, Int4 numLastSeqAligns)
{
    ValNodePtr  returnValNodePtr, thisValNodePtr, nextValNodePtr;
    SeqAlignPtr thisSeqAlign;
    Int4 lastAlignIndex;
    Int4 lastNonEmptyAlignIndex;
    
    if(seqAlignList == NULL)
        return NULL;
    
    lastAlignIndex = 0;
    lastNonEmptyAlignIndex = 0;
    returnValNodePtr = (ValNodePtr) MemNew (sizeof(ValNode));
    if (NULL == lastSeqAligns[lastAlignIndex]) 
        returnValNodePtr->data.ptrvalue = NULL;
    else
        returnValNodePtr->data.ptrvalue = seqAlignList;
    returnValNodePtr->next = NULL;
    thisValNodePtr = returnValNodePtr;
    thisSeqAlign = seqAlignList;
    
    /*Find first sub-list that is non-NULL and set it
      to point to seqAlignList*/
    while ((lastAlignIndex < (numLastSeqAligns -1) &&
            (NULL == lastSeqAligns[lastAlignIndex]))) {
        nextValNodePtr = (ValNodePtr) MemNew (sizeof(ValNode));
        if (NULL == lastSeqAligns[lastAlignIndex+1]) 
            nextValNodePtr->data.ptrvalue = NULL;
        else {
            nextValNodePtr->data.ptrvalue = seqAlignList;
            lastNonEmptyAlignIndex = lastAlignIndex +1;
        }
        thisValNodePtr->next = nextValNodePtr;
        nextValNodePtr->next = NULL;
        thisValNodePtr = nextValNodePtr;
        lastAlignIndex++;
    }
    
    while ((NULL != thisSeqAlign) || (lastAlignIndex < (numLastSeqAligns -1))) { 
        if ((lastAlignIndex < (numLastSeqAligns -1)) &&
            ((NULL == lastSeqAligns[lastAlignIndex])  ||
             (thisSeqAlign == lastSeqAligns[lastAlignIndex]))) {
            nextValNodePtr = (ValNodePtr) MemNew (sizeof(ValNode));
            
            if (NULL != lastSeqAligns[lastAlignIndex+1]) {
                nextValNodePtr->data.ptrvalue = thisSeqAlign->next;
                thisSeqAlign = thisSeqAlign->next;
                lastSeqAligns[lastNonEmptyAlignIndex]->next = NULL;
                lastNonEmptyAlignIndex = lastAlignIndex +1;
            } else {
                nextValNodePtr->data.ptrvalue = NULL;
            }
            thisValNodePtr->next = nextValNodePtr;
            nextValNodePtr->next = NULL;
            thisValNodePtr = nextValNodePtr;
            lastAlignIndex++;
        } else {
            if (NULL != thisSeqAlign)
                thisSeqAlign = thisSeqAlign->next;
        }
    }
    return (returnValNodePtr);
}

/*converts a 2-level list of SeqAligns stored as a ValNodePtr */
 
SeqAlignPtr 
convertValNodeListToSeqAlignList(ValNodePtr seqAlignDoubleList, 
                                 SeqAlignPtr ** lastSeqAligns, 
                                 Int4 * numLastSeqAligns)
{
    ValNodePtr thisValNodePtr, secondValNodePtr;
    SeqAlignPtr returnSeqAlign, thisSeqAlign;
    Int4 numValNodePtrs, indexValNodePtrs;

    
    thisValNodePtr = seqAlignDoubleList;
    numValNodePtrs = 0;

    while (NULL != thisValNodePtr) {
        numValNodePtrs++;
        thisValNodePtr = thisValNodePtr->next;
    }

    if (seqAlignDoubleList == NULL || 
        seqAlignDoubleList->data.ptrvalue == NULL) {
        *numLastSeqAligns = 0;
        *lastSeqAligns = NULL;
        return NULL;
    } else {
        indexValNodePtrs = 0;
        *numLastSeqAligns = numValNodePtrs;
        *lastSeqAligns = (SeqAlignPtr *) MemNew (numValNodePtrs * sizeof(SeqAlignPtr));
        returnSeqAlign = seqAlignDoubleList->data.ptrvalue;
        thisValNodePtr = seqAlignDoubleList;
	do {
	  thisSeqAlign = thisValNodePtr->data.ptrvalue;
	  if (thisSeqAlign != NULL) {
	    while (thisSeqAlign->next != NULL) 
	      thisSeqAlign = thisSeqAlign->next;
	  }
	  (*lastSeqAligns)[indexValNodePtrs] = thisSeqAlign;
	  indexValNodePtrs++;
	  /*find next non-empty list to link in*/
	  if (thisSeqAlign != NULL) {
	    secondValNodePtr = thisValNodePtr->next;
	    while (NULL != secondValNodePtr) {
	      if (NULL != secondValNodePtr->data.ptrvalue)
	      {
		thisSeqAlign->next = secondValNodePtr->data.ptrvalue;
		break;
	      }
	      else
		secondValNodePtr = secondValNodePtr->next;
	    }
	    /*if all remaning ValNodePtrs have NULL seqAligns, finish off
	      list of seqAligns with a NULL*/
	    if (NULL == secondValNodePtr)
	      thisSeqAlign->next = NULL;
	  }
	  thisValNodePtr = thisValNodePtr->next;
        } while (NULL != thisValNodePtr);
        return returnSeqAlign;
    }
}

void LIBCALL
fillCandLambda(seedSearchItems * seedSearch, Char *matrixName, BLAST_OptionsBlkPtr options)
{
  if (0 == StringCmp("BLOSUM62", matrixName)) {
    seedSearch->paramC = 0.50;
    if ((11 == options->gap_open) && (1 == options->gap_extend)) {
      seedSearch->paramLambda = 0.270;
      seedSearch->paramK = 0.047;
      return;
    }
    if ((9 == options->gap_open) && (2 == options->gap_extend)) {
      seedSearch->paramLambda = 0.285;
      seedSearch->paramK = 0.075;
      return;
    }
    if ((8 == options->gap_open) && (2 == options->gap_extend)) {
      seedSearch->paramLambda = 0.265;
      seedSearch->paramK = 0.046;
      return;
    }
    if ((7 == options->gap_open) && (2 == options->gap_extend)) {
      seedSearch->paramLambda = 0.243;
      seedSearch->paramK = 0.032;
      return;
    }
    if ((12 == options->gap_open) && (1 == options->gap_extend)) {
      seedSearch->paramLambda = 0.281;
      seedSearch->paramK = 0.057;
      return;
    }
    if ((10 == options->gap_open) && (1 == options->gap_extend)) {
      seedSearch->paramLambda = 0.250;
      seedSearch->paramK = 0.033;
      return;
    }
    ErrPostEx(SEV_FATAL, 0, 0, "The combination %d for gap opening cost and %d for gap extension is not supported in PHI-BLAST with matrix %s\n", options->gap_open, options->gap_extend, matrixName);
  }
  else {
    if (0 == StringCmp("PAM30", matrixName)) { 
      seedSearch->paramC = 0.30;
      if ((9 == options->gap_open) && (1 == options->gap_extend)) {
	seedSearch->paramLambda = 0.295;
	seedSearch->paramK = 0.13;
	return;
      }
      if ((7 == options->gap_open) && (2 == options->gap_extend)) {
	seedSearch->paramLambda = 0.306;
	seedSearch->paramK = 0.15;
	return;
      }
      if ((6 == options->gap_open) && (2 == options->gap_extend)) {
	seedSearch->paramLambda = 0.292;
	seedSearch->paramK = 0.13;
	return;
      }
      if ((5 == options->gap_open) && (2 == options->gap_extend)) {
	seedSearch->paramLambda = 0.263;
	seedSearch->paramK = 0.077;
	return;
      }
      if ((10 == options->gap_open) && (1 == options->gap_extend)) {
	seedSearch->paramLambda = 0.309;
	seedSearch->paramK = 0.15;
	return;
      }
      if ((8 == options->gap_open) && (1 == options->gap_extend)) {
	seedSearch->paramLambda = 0.270;
	seedSearch->paramK = 0.070;
	return;
      }
      ErrPostEx(SEV_FATAL, 0, 0, "The combination %d for gap opening cost and %d for gap extension is not supported in PHI-BLAST with matrix %s\n", options->gap_open, options->gap_extend, matrixName);
    }
    else {
      if (0 == StringCmp("PAM70", matrixName)) { 
	seedSearch->paramC = 0.35;
	if ((10 == options->gap_open) && (1 == options->gap_extend)) {
	  seedSearch->paramLambda = 0.291;
	  seedSearch->paramK = 0.089;
	  return;
	}
	if ((8 == options->gap_open) && (2 == options->gap_extend)) {
	  seedSearch->paramLambda = 0.303;
	  seedSearch->paramK = 0.13;
	  return;
	}
	if ((7 == options->gap_open) && (2 == options->gap_extend)) {
	  seedSearch->paramLambda = 0.287;
	  seedSearch->paramK = 0.095;
	  return;
	}
	if ((6 == options->gap_open) && (2 == options->gap_extend)) {
	  seedSearch->paramLambda = 0.269;
	  seedSearch->paramK = 0.079;
	  return;
	}
	if ((11 == options->gap_open) && (1 == options->gap_extend)) {
	  seedSearch->paramLambda = 0.307;
	  seedSearch->paramK = 0.13;
	  return;
	}
	if ((9 == options->gap_open) && (1 == options->gap_extend)) {
	  seedSearch->paramLambda = 0.269;
	  seedSearch->paramK = 0.058;
	  return;
	}
	ErrPostEx(SEV_FATAL, 0, 0, "The combination %d for gap opening cost and %d for gap extension is not supported in PHI-BLAST with matrix %s\n", options->gap_open, options->gap_extend, matrixName);
      }
      else {
	if (0 == StringCmp("BLOSUM80", matrixName)) { 
	  seedSearch->paramC = 0.40;
	  if ((10 == options->gap_open) && (1 == options->gap_extend)) {
	    seedSearch->paramLambda = 0.300;
	    seedSearch->paramK = 0.072;
	    return;
	  }
	  if ((8 == options->gap_open) && (2 == options->gap_extend)) {
	    seedSearch->paramLambda = 0.308;
	    seedSearch->paramK = 0.089;
	    return;
	  }
	  if ((7 == options->gap_open) && (2 == options->gap_extend)) {
	    seedSearch->paramLambda = 0.295;
	    seedSearch->paramK = 0.077;
	    return;
	  }
	  if ((6 == options->gap_open) && (2 == options->gap_extend)) {
	    seedSearch->paramLambda = 0.271;
	    seedSearch->paramK = 0.051;
	    return;
	  }
	  if ((11 == options->gap_open) && (1 == options->gap_extend)) {
	    seedSearch->paramLambda = 0.314;
	    seedSearch->paramK = 0.096;
	    return;
	  }
	  if ((9 == options->gap_open) && (1 == options->gap_extend)) {
	    seedSearch->paramLambda = 0.277;
	    seedSearch->paramK = 0.046;
	    return;
	  }
	  ErrPostEx(SEV_FATAL, 0, 0, "The combination %d for gap opening cost and %d for gap extension is not supported in PHI-BLAST with matrix %s\n", options->gap_open, options->gap_extend, matrixName);
	}
	else {
	  if (0 == StringCmp("BLOSUM45", matrixName)) { 
	    seedSearch->paramC = 0.60;
	    if ((14 == options->gap_open) && (2 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.199;
	      seedSearch->paramK = 0.040;
	      return;
	    }
	    if ((13 == options->gap_open) && (3 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.209;
	      seedSearch->paramK = 0.057;
	      return;
	    }
	    if ((12 == options->gap_open) && (3 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.203;
	      seedSearch->paramK = 0.049;
	      return;
	    }
	    if ((11 == options->gap_open) && (3 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.193;
	      seedSearch->paramK = 0.037;
	      return;
	    }
	    if ((10 == options->gap_open) && (3 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.182;
	      seedSearch->paramK = 0.029;
	      return;
	    }
	    if ((15 == options->gap_open) && (2 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.206;
	      seedSearch->paramK = 0.049;
	      return;
	    }
	    if ((13 == options->gap_open) && (2 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.190;
	      seedSearch->paramK = 0.032;
	      return;
	    }
	    if ((12 == options->gap_open) && (2 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.177;
	      seedSearch->paramK = 0.023;
	      return;
	    }
	    if ((19 == options->gap_open) && (1 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.209;
	      seedSearch->paramK = 0.049;
	      return;
	    }
	    if ((18 == options->gap_open) && (1 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.202;
	      seedSearch->paramK = 0.041;
	      return;
	    }
	    if ((17 == options->gap_open) && (1 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.195;
	      seedSearch->paramK = 0.034;
	      return;
	    }
	    if ((16 == options->gap_open) && (1 == options->gap_extend)) {
	      seedSearch->paramLambda = 0.183;
	      seedSearch->paramK = 0.024;
	      return;
	    }
	    ErrPostEx(SEV_FATAL, 0, 0, "The combination %d for gap opening cost and %d for gap extension is not supported in PHI-BLAST with matrix %s\n", options->gap_open, options->gap_extend, matrixName);

	  }
	  else {
	    ErrPostEx(SEV_FATAL, 0, 0, "Matrix %s not allowed in PHI-BLAST\n", matrixName);
          }
        }
      }
    }
  }
}

/* 
	Used by BlastParseInputString to get the 'index' of the
	option and to check it's validity.
*/
Int4 BlastGetLetterIndex(CharPtr letters, Char ch)
{
    Int4 index;

    for(index = 0; letters[index] != NULLB; index++) {
	if (letters[index] == ch) {
	    return index;
	}
    }
    return -1;
}

/*
	Parses a string of input options.
	For use in the Web page and filtering options. 
	Not for use in command-line programs - use GetArgs.
*/

Boolean BlastParseInputString(CharPtr string, 
	CharPtr letters, 
	CharPtr PNTR *values_in,
	CharPtr PNTR ErrorMessage)
{
    CharPtr chptr;
    Int4 i, index = 0, max_par_num;
    Char option[1024];
    CharPtr PNTR values;
    Char message[1024];

    if(string == NULL || letters == NULL || 
	    *letters == '\0' || values_in == NULL) {
	return FALSE;
    }

    max_par_num = StringLen(letters);

    values = (CharPtr PNTR)MemNew(max_par_num * sizeof(CharPtr));
    *values_in = values;

    chptr = string;

    while(1) {
	while(IS_WHITESP(*chptr)) /* Rolling spaces */
	    chptr++;

	if(*chptr == NULLB)   /* Check for NULLB */
	    break;

	if (*chptr != '-') {   /* Check for the option sign */
	    sprintf(message, "Invalid input string started from \"%s\"", 
		    chptr);
	    *ErrorMessage = StringSave(message);
	    return FALSE;
	} else {
	    chptr++;
	}

	/* checking index in options table */

	if((index = BlastGetLetterIndex(letters, *chptr)) < 0) {
	    sprintf(message, "Character \'%c\' is not a valid option", 
		    *chptr);
	    *ErrorMessage = StringSave(message);
	    return FALSE;
	}

	if(*chptr == NULLB)   /* Check for NULLB */
	    break;

	chptr++;

	while(IS_WHITESP(*chptr)) /* Rolling spaces */
	    chptr++;

	if(*chptr == NULLB)   /* Check for NULLB */
	    break;

        /* If option is in double quotes, get the entire string between the
           quotes */
        if (*chptr == '\'') {
           for (i=0; *++chptr != NULLB && *chptr != '\''; i++)
              option[i] = *chptr;
           if (*chptr == '\'')
              chptr++;
        } else {
           for(i=0; !IS_WHITESP(*chptr) && *chptr != NULLB; i++, chptr++)
              option[i] = *chptr;
        }
	option[i] = NULLB;

	MemFree(values[index]);
	values[index] = StringSave(option);
    }

    return TRUE;
}

CharPtr BLASTGetDatabaseTitleFromSeqAnnot(SeqAnnotPtr seqannot)
{
    ValNodePtr vnp;
    
    UserObjectPtr uop;
    UserFieldPtr ufp;
    ObjectIdPtr oip;
    
    for(vnp = seqannot->desc; vnp != NULL; vnp = vnp->next) {
        
        if (vnp->choice == Annot_descr_user) {
            uop = (UserObjectPtr) vnp->data.ptrvalue;
            oip = uop->type;
            if(!StringICmp(oip->str, "BLAST database title")) {
                ufp = uop->data;
                oip = ufp->label;
                return oip->str;
            }
        }
    }
    return NULL;
}

void BLASTAddBlastDBTitleToSeqAnnot(SeqAnnotPtr seqannot, CharPtr title)
{
    UserObjectPtr uop;
    UserFieldPtr ufp;
    ObjectIdPtr oip;
    
    uop = UserObjectNew();
    oip = ObjectIdNew();
    uop->type = oip;
    oip->str = StringSave("BLAST database title");
    
    ufp = UserFieldNew();
    oip = ObjectIdNew();
    ufp->label = oip;
    uop->data = ufp;
    
    oip->str = StringSave(title);
    ufp->choice = 4;
    ufp->data.boolvalue = TRUE; /* Just to fill required field ... */
    
    ValNodeAddPointer(&(seqannot->desc), Annot_descr_user, (Pointer)uop);
}

/* pos is assumed to be the address of a chracter in the array seq
   if so, copy from pos backwards to the start of seq
   into target.
   return the number of characters copied*/
Int4 reverse_seq(Uint1 *seq, Uint1 *pos, Uint1 *target) 
{
    Uint1 *c; /*index over addresses of characters in seq*/
    Int4 numCopied; /*number of characters copied*/

    for (c = pos, numCopied = 0; c >= seq; c--, numCopied++) 
	*target++ = *c;
    *target = '\0';
    return numCopied;
}

static void FilterAlignFree(FilterAlignPtr fap)
{
    HspFree(fap->hsp);
    
    if(fap->sap != NULL)
        SeqAlignSetFree(fap->sap);
    
    MemFree(fap);

    return;
}

static void SappSetFree(SappSetPtr ssp)
{
    Int4 i;
    FilterAlignPtr fap;
    
    for(i = 0; i < ssp->count; i++) {
        if ((fap = ssp->fapp[i]) != NULL) {
            FilterAlignFree(fap);
        }
    }

    MemFree(ssp->fapp);
    MemFree(ssp);

    return;
}
static int LIBCALLBACK SappArrayScoreCmp(VoidPtr v1, VoidPtr v2)
{
    FilterAlignPtr f1, f2;
    
    f1 = (*(FilterAlignPtr PNTR) v1);
    f2 = (*(FilterAlignPtr PNTR) v2);

    if (f1->hsp->score < f2->hsp->score)
        return 1;
    else if (f1->hsp->score > f2->hsp->score)
        return -1;

    if (f1->hsp->evalue > f2->hsp->evalue)
        return 1;
    else if (f1->hsp->evalue < f2->hsp->evalue)
        return -1;
    
    else return 0;
}

static Boolean BLASTSortSapArray(SappSetPtr ssp)
{

    HeapSort((VoidPtr)ssp->fapp, ssp->count, sizeof(FilterAlignPtr), 
             SappArrayScoreCmp);

    return TRUE;
}

static Boolean BLASTTestHSPPosition(HspPtr hsp1, HspPtr hsp2, Int4 pct)
{
    Boolean from_is_inside, to_is_inside;
    Int4 length;
    Nlm_FloatHi pct_overlap = 0.0;

    if(hsp1 == NULL || hsp2 == NULL)
        return TRUE;
    
    from_is_inside = FALSE;
    to_is_inside   = FALSE;

    if((SIGN(hsp1->query_frame) != SIGN(hsp2->query_frame)) || 
       (SIGN(hsp1->hit_frame) != SIGN(hsp2->hit_frame)))
        return TRUE;


    if(SIGN(hsp1->query_frame) == SIGN(hsp1->hit_frame)) {

        if(CONTAINED_IN_HSP(hsp1->query_from, hsp1->query_to, hsp2->query_from,hsp1->hit_from, hsp1->hit_to, hsp2->hit_from)) {
            from_is_inside = TRUE;
        }
        
        if(CONTAINED_IN_HSP(hsp1->query_from, hsp1->query_to, hsp2->query_from, hsp1->hit_from, hsp1->hit_to, hsp2->hit_from)) {
            to_is_inside = TRUE;
        }
    } else {
        if(CONTAINED_IN_HSP(hsp1->query_to, hsp1->query_from, hsp2->query_from, hsp1->hit_from, hsp1->hit_to, hsp2->hit_from)) {
            from_is_inside = TRUE;
        }

        if(CONTAINED_IN_HSP(hsp1->query_to, hsp1->query_from, hsp2->query_from, hsp1->hit_from, hsp1->hit_to, hsp2->hit_from)) {
            to_is_inside = TRUE;
        }
        
    }

    if(from_is_inside && to_is_inside)
        return FALSE;

    length = abs(hsp2->query_to -  hsp2->query_from);
    
    if(from_is_inside) {
        pct_overlap = (length - (MIN(abs(hsp2->query_to - hsp1->query_from), abs(hsp2->query_to - hsp1->query_to))))*100.0/length;
        
    } else if (to_is_inside) {
        
        pct_overlap = (length - (MIN(abs(hsp2->query_from - hsp1->query_from), abs(hsp2->query_from - hsp1->query_to))))*100.0/length;
        
    }

    if(pct_overlap > pct)
        return FALSE;
    
    return TRUE;
}
static SeqAlignPtr BLASTFilterSapArray(SappSetPtr ssp,  Int4 pct, 
                                       SeqAlignPtr PNTR next_tail)
{
    Int4 i, j;
    HspPtr hsp1, hsp2;
    SeqAlignPtr sap_head;

    if(ssp == NULL || ssp->count == 0)
        return NULL;

    
    for(i = 0; i < ssp->count; i++) {
        
        if(ssp->fapp[i] == NULL)
            continue;
        
        hsp1 = ssp->fapp[i]->hsp;
        
        for(j = 0; j < i; j++) {
            
            if(ssp->fapp[j] == NULL)
                continue;
            
            hsp2 = ssp->fapp[j]->hsp;
            
            if(!BLASTTestHSPPosition(hsp1, hsp2, pct)) { 
                /* FALSE mean hsp2 to be removed */
                if(ssp->fapp[j] != NULL) {
                    FilterAlignFree(ssp->fapp[j]);    
                    ssp->fapp[j] = NULL;
                }
                break;
            }
        }
    }

    sap_head   = NULL;
    *next_tail = NULL;
    
    for(i = 0; i < ssp->count; i++) {
        if(ssp->fapp[i] != NULL) {
            if(sap_head == NULL) {
                sap_head = ssp->fapp[i]->sap;
                *next_tail = sap_head;
            } else {
                (*next_tail)->next = ssp->fapp[i]->sap;
                *next_tail = (*next_tail)->next;
            }
            ssp->fapp[i]->sap = NULL;
        }
    }
    
    return sap_head;
}

static SappSetPtr BLASTCreateSappArray(SeqAlignPtr sap_in, 
                                       Boolean subject_is_aa, 
                                       Boolean is_ooframe)
{
    Int4 count;
    SeqAlignPtr sap;
    SappSetPtr ssp;

    if(sap_in == NULL)
        return NULL;
    
    for(count = 0, sap = sap_in; sap != NULL; sap = sap->next)
        count++;
    
    ssp = (SappSetPtr) MemNew(sizeof(SappSet));
    
    ssp->count = count;
    
    ssp->fapp = (FilterAlignPtr PNTR) MemNew(count*sizeof(FilterAlignPtr));
    
    for(count = 0, sap = sap_in; sap != NULL; count++) {
        ssp->fapp[count] = MemNew(sizeof(FilterAlign));
        ssp->fapp[count]->sap = sap;
        SeqIdWrite(TxGetSubjectIdFromSeqAlign(sap), 
                   ssp->fapp[count]->subject_sip, 
                   PRINTID_FASTA_LONG,
                   sizeof(ssp->fapp[count]->subject_sip));
        ssp->fapp[count]->hsp = BXMLGetHspFromSeqAlign(sap, subject_is_aa, 0,
                                                       is_ooframe);

        sap = sap->next;
        ssp->fapp[count]->sap->next = NULL; /* Single SAP */
    }

    return ssp;
}

static SappSetPtr BLASTGetNextSapArray(SappSetPtr ssp_in, Int4 pct)
{
    CharPtr current_sip;
    Int4 count, i;
    SappSetPtr ssp;

    /* First - calculating number of SeqAligns for the next sap array */
    current_sip = NULL;
    count = 0;
    for(i = 0; i < ssp_in->count; i++) {
        
        if (ssp_in->fapp[i] != NULL) {
            if(current_sip == NULL) {
                current_sip = ssp_in->fapp[i]->subject_sip;
                count++;
                continue;
            }
            
            if(!StringCmp(current_sip, ssp_in->fapp[i]->subject_sip))
                count++;
        }
    }

    if(count == 0)
        return NULL;

    ssp = (SappSetPtr) MemNew(sizeof(SappSet));
    ssp->count = count;
    ssp->fapp = (FilterAlignPtr PNTR) MemNew(sizeof(FilterAlignPtr) * ssp->count);

    for(i = 0, count = 0; i < ssp_in->count && count < ssp->count; i++) {
        if (ssp_in->fapp[i] != NULL) {
            if(!StringCmp(current_sip, ssp_in->fapp[i]->subject_sip)) {
                ssp->fapp[count] = ssp_in->fapp[i];
                ssp_in->fapp[i] = NULL;
                count++;
            }
        }
    }
    return ssp;
}
        
/* -----------------------------------------------------------------
   This function will filter given SeqAlignPtr for overlaping
   regions for the same query/subject pair. Another input parameter is
   percentage of overlapping required for the alignment to be removed
   from SeqAlignPtr
   ---------------------------------------------------------------- */

SeqAlignPtr BLASTFilterOverlapRegions(SeqAlignPtr sap, Int4 pct, 
                                      Boolean subject_is_aa, 
                                      Boolean is_ooframe,
                                      Boolean sort_array)
{
    SeqAlignPtr seqalign, head, tail=NULL, next_tail;
    SappSetPtr  ssp, ssp_tmp;
    
    if(sap == NULL)
        return NULL;
    
    head = NULL;
    
    ssp = BLASTCreateSappArray(sap, subject_is_aa, is_ooframe);
    
    /* Generally speaking SeqAlignPtr coming from Blast engine should be
       correctly sorted - if not sorting below may be used */
    
    if(sort_array)
        BLASTSortSapArray(ssp);
    
    while(TRUE) {
        
        if((ssp_tmp = BLASTGetNextSapArray(ssp, pct)) == NULL)
            break;
        
        BLASTSortSapArray(ssp_tmp);
        
        seqalign = BLASTFilterSapArray(ssp_tmp, pct, &next_tail);

        SappSetFree(ssp_tmp);
        
        if(head == NULL) {
            head = seqalign;
        } else {
            tail->next = seqalign;
        }
        
        tail = next_tail;
    }
    
    SappSetFree(ssp);
    
    return head;
}

#define BLAST_ERROR_BULEN 50
/*
	The following functions fill a the Error user string with
	text to identify BLAST and the entry being worked on.
	The SeqIdPtr is used to make a FASTA id, which is appended
	to string.

	A Uint1 is returned, which allows Nlm_ErrUserDelete to delete
	this error string when it's done.
*/

Uint1
BlastSetUserErrorString(CharPtr string, SeqIdPtr sip, Boolean use_id)

{
	BioseqPtr bsp;
	Char buffer[2*BLAST_ERROR_BULEN+1], textid[BLAST_ERROR_BULEN+1];
	CharPtr buf_start, ptr, title;
	Int2 length=0, index;
	Uint1 retval=0;

	buffer[0] = NULLB;
	ptr = buf_start = &buffer[0];

	if (string)
		StringNCpy(ptr, string, BLAST_ERROR_BULEN);

	if (sip != NULL)
	{
	    bsp = BioseqLockById(sip);
	    if(bsp)
	    {
		if (use_id)
			sip = bsp->id;
		else
			title = BioseqGetTitle(bsp);
	    }

	    if (string)
	    {
	    	length = StringLen(string);
	    	if (length > BLAST_ERROR_BULEN)
			length = BLAST_ERROR_BULEN;
	    }

	    ptr += length;

	    if (use_id)
	    {
    	    	SeqIdWrite(sip, textid, PRINTID_FASTA_LONG, BLAST_ERROR_BULEN-1);
	    	StringNCpy(ptr, textid, BLAST_ERROR_BULEN-1);
	    }
	    else if (title)
	    {
		for (index=0; index<BLAST_ERROR_BULEN-1; index++)
		{
			if (title[index] == NULLB || title[index] == ' ')
			{
				break;
			}
			*ptr = title[index];
			ptr++;
		}
		*ptr = NULLB;
	    }
	    BioseqUnlock(bsp);
	    StringCpy(ptr+StringLen(ptr), ":");
	}
	NlmMutexLockEx(&err_message_mutex);
	retval = Nlm_ErrUserInstall (buf_start, 0);
	NlmMutexUnlock(err_message_mutex);

	return retval;
}

void
BlastDeleteUserErrorString(Uint1 err_id)

{
	NlmMutexLockEx(&err_message_mutex);
	Nlm_ErrUserDelete(err_id);
	NlmMutexUnlock(err_message_mutex);
	return;
}

static Int4
BlastBioseqGetNumIdentical(BioseqPtr q_bsp, BioseqPtr s_bsp, Int4 q_start, 
                     Int4 s_start, Int4 length, 
                     Uint1 q_strand, Uint1 s_strand)
{
   SeqLocPtr q_slp, s_slp;
   SeqPortPtr q_spp, s_spp;
   Int4 i, ident = 0;
   Uint1 q_res, s_res;

   if (!q_bsp || !s_bsp)
      return 0;

   q_slp = SeqLocIntNew(q_start, q_start+length-1, q_strand, q_bsp->id); 
   s_slp = SeqLocIntNew(s_start, s_start+length-1, s_strand, s_bsp->id); 
   if (ISA_na(q_bsp->mol))
      q_spp = SeqPortNewByLoc(q_slp, Seq_code_ncbi4na);
   else
      q_spp = SeqPortNewByLoc(q_slp, Seq_code_ncbistdaa);
   if (ISA_na(s_bsp->mol))
      s_spp = SeqPortNewByLoc(s_slp, Seq_code_ncbi4na);
   else
      s_spp = SeqPortNewByLoc(s_slp, Seq_code_ncbistdaa);

   for (i=0; i<length; i++) {
      while ((q_res = SeqPortGetResidue(q_spp)) != SEQPORT_EOF &&
             !IS_residue(q_res));
      while ((s_res = SeqPortGetResidue(s_spp)) != SEQPORT_EOF &&
             !IS_residue(s_res));
      if (q_res == SEQPORT_EOF || s_res == SEQPORT_EOF)
         break;
      else if (q_res == s_res)
         ident++;
   }

   SeqLocFree(q_slp);
   SeqLocFree(s_slp);
   SeqPortFree(q_spp);
   SeqPortFree(s_spp);

   return ident;
}

Int4
BlastGetNumIdentical(Uint1Ptr query, Uint1Ptr subject, Int4 q_start, 
                     Int4 s_start, Int4 length, Boolean reverse)
{
   Int4 i, ident = 0;
   Uint1Ptr q, s;

   q = &query[q_start];
   if (!reverse) 
      s = &subject[s_start];
   else
      s = &subject[s_start + length - 1];

   for (i=0; i<length; i++) {
      if (*q == *s)
	 ident++;
      q++;
      if (!reverse)
         s++;
      else
         s--;
   }

   return ident;
}

static void ScoreAndEvalueToBuffers(FloatHi bit_score, FloatHi evalue, 
                                  CharPtr *bit_score_buf, CharPtr *evalue_buf)
{
   if (evalue < 1.0e-180)
      sprintf(*evalue_buf, "0.0");
   else if (evalue < 1.0e-99)
      sprintf(*evalue_buf, "%2.0le", evalue);
   else if (evalue < 0.0009) 
         sprintf(*evalue_buf, "%3.1le", evalue);
   else if (evalue < 1.0) 
      sprintf(*evalue_buf, "%4.3lf", evalue);
   else 
      sprintf(*evalue_buf, "%5.1lf", evalue);
   
   if (bit_score > 9999)
      sprintf(*bit_score_buf, "%4.3le", bit_score);
   else if (bit_score > 99.9)
      sprintf(*bit_score_buf, "%4.1lf", bit_score);
   else
      sprintf(*bit_score_buf, "%4.2lf", bit_score);
}

/* 
   Function to print results in tab-delimited format, given a SeqAlign list.
   q_shift and s_shift are the offsets in query and subject in case of a
   subsequence search 
*/
void BlastPrintTabulatedResults(SeqAlignPtr seqalign, BioseqPtr query_bsp,
                                SeqLocPtr query_slp, Int4 num_alignments, 
                                CharPtr blast_program, Boolean is_ungapped, 
                                Boolean believe_query, Int4 q_shift, 
                                Int4 s_shift, FILE *fp)
{
   SeqAlignPtr sap, sap_tmp = NULL;
   FloatHi perc_ident, bit_score, evalue;
   Int4 numseg, num_gap_opens, num_mismatches, num_ident, score;
   Int4 number, align_length, index, i;
   Int4 q_start, q_end, s_start, s_end;
   CharPtr eval_buff, bit_score_buff;
   Boolean is_translated;
   SeqIdPtr query_id, old_query_id = NULL, subject_id, old_subject_id = NULL;
   BioseqPtr subject_bsp=NULL;
   Char query_buffer[BUFFER_LENGTH+1], subject_buffer[BUFFER_LENGTH+1];
   DenseSegPtr dsp;
   StdSegPtr ssp = NULL;
   DenseDiagPtr ddp = NULL;
   AlignSumPtr asp = NULL;
   CharPtr defline, title;
   SeqLocPtr slp;

   is_translated = (StringCmp(blast_program, "blastn") &&
                    StringCmp(blast_program, "blastp"));
   
   eval_buff = Malloc(10);
   bit_score_buff = Malloc(10);
   if (is_translated) {
      asp = MemNew(sizeof(AlignSum));
      asp->matrix = load_default_matrix();
      asp->is_aa = TRUE;
   }

   if (is_ungapped)
      sap_tmp = SeqAlignNew();

   slp = query_slp;
   if (query_bsp)
      query_id = query_bsp->id;

   for (sap = seqalign; sap; sap = sap->next) {
      if (query_slp)
         query_id = TxGetQueryIdFromSeqAlign(sap);
      if (SeqIdComp(query_id, old_query_id) != SIC_YES) {
         /* New query: find the corresponding SeqLoc */
         while (slp && SeqIdComp(query_id, SeqLocId(slp)) != SIC_YES)
            slp = slp->next;
         if (slp != NULL) {
            query_id = old_query_id = SeqLocId(slp);
         } else if (query_bsp)
            old_query_id = query_bsp->id;
         defline = (CharPtr) Malloc(BUFFER_LENGTH+1);
         SeqIdWrite(query_id, defline, PRINTID_FASTA_LONG, BUFFER_LENGTH);
         if (StringNCmp(defline, "lcl|", 4))
            StringCpy(query_buffer, defline);
         else if (!believe_query) {
            if (slp) {
               BioseqUnlock(query_bsp);
               query_bsp = BioseqLockById(query_id);
            }
            if ((title = StringSave(BioseqGetTitle(query_bsp))) != NULL) {
               defline = MemFree(defline);
               defline = StringTokMT(title, " ", &title);
               StringCpy(query_buffer, defline);
               defline = MemFree(defline);
            } else
               StringCpy(query_buffer, defline+4);
            defline = MemFree(defline);
         } else
            StringCpy(query_buffer, defline+4);
      } else
         query_id = old_query_id;      

      perc_ident = 0;
      align_length = 0;
      num_gap_opens = 0;
      num_mismatches = 0;

      GetScoreAndEvalue(sap, &score, &bit_score, &evalue, &number);

      ScoreAndEvalueToBuffers(bit_score, evalue, 
                              &bit_score_buff, &eval_buff);

      subject_id = TxGetSubjectIdFromSeqAlign(sap);

      if (SeqIdComp(subject_id, old_subject_id) != SIC_YES) {
         /* New subject sequence has been found in the seqalign list */
         if (--num_alignments < 0)
            break;
         BioseqUnlock(subject_bsp);
         subject_bsp = BioseqLockById(subject_id);
      
         if (!subject_bsp || !subject_bsp->id)
            continue;
         if (subject_bsp->id->choice != SEQID_GENERAL ||
             StringCmp(((DbtagPtr)subject_id->data.ptrvalue)->db, "BL_ORD_ID")) {
            defline = (CharPtr) Malloc(BUFFER_LENGTH+1);
            SeqIdWrite(subject_bsp->id, defline, PRINTID_FASTA_LONG, BUFFER_LENGTH);
            if (StringNCmp(defline, "lcl|", 4))
               StringCpy(subject_buffer, defline);
            else
               StringCpy(subject_buffer, defline+4);
         } else {
            defline = StringSave(BioseqGetTitle(subject_bsp));
            defline = StringTokMT(defline, " \t", &title);
            StringCpy(subject_buffer, defline);
         }
         defline = MemFree(defline);
      }
      
      /* Loop on segments within this seqalign (in ungapped case) */
      while (TRUE) {
         if (sap->segtype == SAS_DENSEG) {
            dsp = (DenseSegPtr) sap->segs;
            numseg = dsp->numseg;
            
            for (i=0; i<numseg; i++) {
               align_length += dsp->lens[i];
               if (dsp->starts[2*i] != -1 && dsp->starts[2*i+1] != -1) {
                  num_ident = BlastBioseqGetNumIdentical(query_bsp, subject_bsp, 
                                 dsp->starts[2*i], dsp->starts[2*i+1], 
                                 dsp->lens[i], 
                                 dsp->strands[2*i], dsp->strands[2*i+1]);
                  perc_ident += num_ident;
                  num_mismatches += dsp->lens[i] - num_ident;
               } else
                  num_gap_opens++;
            }
            perc_ident = perc_ident / align_length * 100;
            
            if (dsp->strands[0] != dsp->strands[1]) {
               q_start = dsp->starts[2*numseg-2] + 1;
               q_end = dsp->starts[0] + dsp->lens[0];
               s_end = dsp->starts[1] + 1;
               s_start = dsp->starts[2*numseg-1] + dsp->lens[numseg-1];
            } else {
               q_start = dsp->starts[0] + 1;
               q_end = dsp->starts[2*numseg-2] + dsp->lens[numseg-1];
               s_start = dsp->starts[1] + 1;
               s_end = dsp->starts[2*numseg-1] + dsp->lens[numseg-1];
            }
         } else if (sap->segtype == SAS_STD) {
            if (!ssp)
               ssp = (StdSegPtr) sap->segs;
            
            if (is_ungapped) {
               sap_tmp->segtype = SAS_STD;
               sap_tmp->segs = ssp;
               GetScoreAndEvalue(sap_tmp, &score, &bit_score, &evalue, &number);
               ScoreAndEvalueToBuffers(bit_score, evalue, 
                                       &bit_score_buff, &eval_buff);
               find_score_in_align(sap_tmp, 1, asp);
            } else
               find_score_in_align(sap, 1, asp);
            
            if (asp->m_frame < 0)
               q_start = SeqLocStop(ssp->loc) + 1;
            else
               q_start = SeqLocStart(ssp->loc) + 1;
            
            if (asp->t_frame < 0)
               s_start = SeqLocStop(ssp->loc->next) + 1;
            else
               s_start = SeqLocStart(ssp->loc->next) + 1;
            
            if (!is_ungapped) {
               for (index=1; ssp->next; index++)
                  ssp = ssp->next;
               num_gap_opens = index / 2;
            } else 
               num_gap_opens = 0;

            if (asp->m_frame < 0)
               q_end = SeqLocStart(ssp->loc) + 1;
            else
               q_end = SeqLocStop(ssp->loc) + 1;
            
            if (asp->t_frame < 0)
               s_end = SeqLocStart(ssp->loc->next) + 1;
            else
               s_end = SeqLocStop(ssp->loc->next) + 1;
            
            align_length = asp->totlen;
            num_mismatches = asp->totlen - asp->gaps - asp->identical;
            perc_ident = ((FloatHi) 100*asp->identical)/ (asp->totlen);
         } else if (sap->segtype == SAS_DENDIAG) {
            if (!ddp)
               ddp = (DenseDiagPtr) sap->segs;
            sap_tmp->segtype = SAS_DENDIAG;
            sap_tmp->segs = ddp;
            GetScoreAndEvalue(sap_tmp, &score, &bit_score, &evalue, &number);
            ScoreAndEvalueToBuffers(bit_score, evalue, 
                                    &bit_score_buff, &eval_buff);

            align_length = ddp->len;
            if (ddp->strands[0] == Seq_strand_minus) {
               q_start = ddp->starts[0] + align_length;
               q_end = ddp->starts[0] + 1;
            } else {
               q_start = ddp->starts[0] + 1;
               q_end = ddp->starts[0] + align_length;
            }

            if (ddp->strands[1] == Seq_strand_minus) {
               s_start = ddp->starts[1] + align_length;
               s_end = ddp->starts[1] + 1;
            } else {
               s_start = ddp->starts[1] + 1;
               s_end = ddp->starts[1] + align_length;
            }
            num_gap_opens = 0;
            num_ident = BlastBioseqGetNumIdentical(query_bsp, subject_bsp, 
                           ddp->starts[0], ddp->starts[1], align_length, 
                           ddp->strands[0], ddp->strands[1]);
            num_mismatches = align_length - num_ident;
            perc_ident = ((FloatHi)num_ident) / align_length * 100;
         }
         if (!is_translated) {
            /* Adjust coordinates if query and/or subject is a subsequence */
            q_start += q_shift;
            q_end += q_shift;
            s_start += s_shift;
            s_end += s_shift;
         }
         
         if (perc_ident >= 99.995 && perc_ident < 100.00)
            perc_ident = 99.99;
         
         fprintf(fp, 
                 "%s\t%s\t%.2f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\n",
                 query_buffer, subject_buffer, perc_ident, align_length, 
                 num_mismatches, num_gap_opens, q_start, 
                 q_end, s_start, s_end, eval_buff, bit_score_buff);
         old_subject_id = subject_id;
         if (sap->segtype == SAS_DENSEG)
            break;
         else if (sap->segtype == SAS_DENDIAG) {
            if ((ddp = ddp->next) == NULL)
               break;
         } else if (sap->segtype == SAS_STD) {
            if ((ssp = ssp->next) == NULL)
               break;
         }
      }
   }

   if (is_ungapped)
      sap_tmp = MemFree(sap_tmp);

   if (is_translated) {
      free_default_matrix(asp->matrix);
      MemFree(asp);
   }
   MemFree(eval_buff);
   MemFree(bit_score_buff);
   BioseqUnlock(subject_bsp);
   if (query_slp)
      BioseqUnlock(query_bsp);
}

int LIBCALLBACK BlastPrintAlignInfo(VoidPtr srch)
{
   BlastSearchBlkPtr search = (BlastSearchBlkPtr) srch;
   BLAST_HSPPtr hsp, PNTR hsp_array; 
   Int4 i;
   CharPtr title, ptr;
   Char query_buffer[BUFFER_LENGTH+1], subject_buffer[BUFFER_LENGTH+1];
   SeqIdPtr sip, subject_id, query_id; 
   Int4 index, score;
   Int4 num_mismatches, num_gap_opens, align_length, num_ident;
   Uint1Ptr query_seq, subject_start=NULL, subject_seq, rev_subject=NULL;
   FloatHi perc_ident, bit_score, evalue;
   Char eval_buff[10], bit_score_buff[10];
   Int4 length=0, query_length, subject_length=0, rev_subject_length=0;
   Int4 q_start, q_end, s_start, s_end, q_shift=0, s_shift=0;
   CharPtr subject_descr = NULL;
   Int4 cutoff_s;
   Boolean numeric_sip_type = FALSE, is_na, is_translated, db_is_na;
   Int4 hspcnt, new_hspcnt, number, numseg;
   SeqAlignPtr seqalign = NULL, sap;
   FILE *fp = (FILE *)search->output;
   BioseqPtr bsp = NULL, query_bsp;
   SeqPortPtr spp;
   SeqLocPtr subject_slp = NULL;
   Uint1 residue;
   AlignSumPtr asp = NULL;

   if (search->current_hitlist == NULL || search->current_hitlist->hspcnt <= 0) {
      search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
      return 0;
   }

   hspcnt = search->current_hitlist->hspcnt;
   is_na = (search->prog_number == blast_type_blastn);
   is_translated = (search->prog_number != blast_type_blastn &&
                    search->prog_number != blast_type_blastp);

   subject_slp = search->query_slp->next;
   /* Only for the two sequences case, get offset shift if subject 
      is a subsequence */
   if (!search->rdfp && subject_slp)
       s_shift = SeqLocStart(subject_slp);
   /* Check if query is a subsequence */
   q_shift = SeqLocStart(search->query_slp);

   if (is_na)
      cutoff_s = search->pbp->cutoff_s;
   else
      cutoff_s = 0;
  
   db_is_na = (search->prog_number != blast_type_blastp && 
               search->prog_number != blast_type_tblastn);

   if (search->rdfp) {
      ReadDBBioseqFetchEnable("blastall", search->rdfp->filename, 
                              db_is_na, TRUE);
      readdb_get_descriptor(search->rdfp, search->subject_id, &sip,
                            &subject_descr);
   } else
      sip = SeqIdSetDup(search->subject_info->sip);

   if (search->prog_number == blast_type_tblastn) {
      if (subject_slp) {
         spp = SeqPortNewByLoc(subject_slp, Seq_code_ncbi4na);
         subject_length = SeqLocLen(subject_slp);
      } else {
         if (search->rdfp)
            bsp = readdb_get_bioseq(search->rdfp, search->subject_id);
         else
            bsp = BioseqLockById(sip);
         spp = SeqPortNew(bsp, 0, -1, Seq_strand_plus, Seq_code_ncbi4na);
         subject_length = bsp->length;
      }
      /* make one longer to "protect" ALIGN. */
      subject_start = subject_seq = 
         MemNew((1+subject_length)*sizeof(Uint1));
      for (index=0; index<subject_length; index++) {
         subject_seq[index] = SeqPortGetResidue(spp);
      }
      /* Gap character in last space. */
      subject_seq[subject_length] = NULLB;
      spp = SeqPortFree(spp);
      
      if (subject_slp && !bsp) {
         Change_Loc_Strand(subject_slp, Seq_strand_minus);
         spp = SeqPortNewByLoc(subject_slp, Seq_code_ncbi4na);
         Change_Loc_Strand(subject_slp, Seq_strand_both);
      } else if (bsp)
         spp = SeqPortNew(bsp, 0, -1, Seq_strand_minus, Seq_code_ncbi4na);

      /* make one longer to "protect" ALIGN. */
      rev_subject = MemNew((1+subject_length)*sizeof(Uint1));
      for (index=0; index<subject_length; index++)
         rev_subject[index] = SeqPortGetResidue(spp);
      /* Gap character in last space. */
      rev_subject[subject_length] = NULLB;
      rev_subject_length = subject_length;
      spp = SeqPortFree(spp);
      if (search->rdfp)
         bsp = BioseqFree(bsp);
      else if (bsp) 
         BioseqUnlock(bsp);
   } else if (search->rdfp) {
      if (is_na) {
         subject_length = 
            readdb_get_sequence_ex(search->rdfp, search->subject_id, 
                                   &subject_start, &length, TRUE);
         subject_seq = subject_start + 1;
      } else {
         subject_length = 
            readdb_get_sequence(search->rdfp, search->subject_id, 
                                &subject_seq);
      }
   } else if (search->prog_number == blast_type_blastn) {
      bsp = BioseqLockById(sip);
      if (subject_slp) {
         spp = SeqPortNewByLoc(subject_slp, Seq_code_ncbi4na);
         subject_length = SeqLocLen(subject_slp);
      } else {
         spp = SeqPortNew(bsp, 0, -1, Seq_strand_plus, Seq_code_ncbi4na);
         subject_length = bsp->length;
      }
      if (bsp->repr == Seq_repr_delta) 
         SeqPortSet_do_virtual(spp, TRUE);
      /* make one longer to "protect" ALIGN. */
      subject_start = MemNew((2+subject_length)*sizeof(Uint1));
      subject_start[0] = ncbi4na_to_blastna[0];
      subject_seq = subject_start+1;
      index=0;
      while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
         if (IS_residue(residue))
            subject_seq[index++] = ncbi4na_to_blastna[residue];
      }
      /* Gap character in last space. */
      subject_seq[index] = ncbi4na_to_blastna[0];
      spp = SeqPortFree(spp);
      BioseqUnlock(bsp);
   } else {
      if (subject_slp) {
         subject_length = SeqLocLen(subject_slp);
         spp = SeqPortNewByLoc(subject_slp, Seq_code_ncbistdaa);
      } else {
         bsp = BioseqLockById(sip);
         subject_length = bsp->length;
         spp = SeqPortNew(bsp, 0, -1, Seq_strand_unknown, 
                          Seq_code_ncbistdaa);
         BioseqUnlock(bsp);
      }
      subject_start = (Uint1Ptr) MemNew(((subject_length)+2)*sizeof(Uint1));
      /* The first residue is the sentinel. */
      subject_start[0] = NULLB;
      subject_seq = subject_start+1;
      index = 0;
      while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
         if (IS_residue(residue))
            subject_seq[index++] = residue;
      }
      subject_seq[index] = NULLB;
      spp = SeqPortFree(spp);
   }

   if (sip->choice != SEQID_GENERAL ||
       StringCmp(((DbtagPtr)sip->data.ptrvalue)->db, "BL_ORD_ID")) {
      ptr = (CharPtr) Malloc(BUFFER_LENGTH+1);
      SeqIdWrite(sip, ptr, PRINTID_FASTA_LONG, BUFFER_LENGTH);
      if (StringNCmp(ptr, "lcl|", 4))
         StringCpy(subject_buffer, ptr);
      else
         StringCpy(subject_buffer, ptr+4);
      ptr = MemFree(ptr);
   } else {
      ptr = StringTokMT(subject_descr, " \t", &subject_descr);
      subject_descr = ptr;
      StringCpy(subject_buffer, ptr);
   }

   query_id = search->query_id;
   if (is_na) 
      query_length = search->query_context_offsets[search->first_context+1] -
         search->query_context_offsets[search->first_context] - 1;
   else
      query_length = search->context[0].query->length;

   query_bsp = BioseqLockById(query_id);
   if (query_id->choice == SEQID_LOCAL) {
      title = BioseqGetTitle(query_bsp);
      if (title) {
         ptr = StringTokMT(title, " ", &title);
         StringCpy(query_buffer, ptr);
      } else {
         ptr = (CharPtr) Malloc(BUFFER_LENGTH+1);
         SeqIdWrite(query_bsp->id, ptr, PRINTID_FASTA_LONG, 
                    BUFFER_LENGTH);
         if (StringNCmp(ptr, "lcl|", 4))
            StringCpy(query_buffer, ptr);
         else
            StringCpy(query_buffer, ptr+4);
         ptr = MemFree(ptr);
         
      }  
   } else {
      SeqIdWrite(query_bsp->id, query_buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
   }
   BioseqUnlock(query_bsp);
   
   if (is_na) {
      search->sbp->kbp_gap[search->first_context] = 
         search->sbp->kbp[search->first_context];
   }

   hsp_array = search->current_hitlist->hsp_array;

   subject_id = GetTheSeqAlignID(sip);
   sip = SeqIdSetFree(sip);
   if (search->prog_number != blast_type_tblastx && 
       search->pbp->gapped_calculation)
      new_hspcnt = RealBlastGetGappedAlignmentTraceback(search, subject_seq, subject_length, rev_subject, rev_subject_length, subject_id, hsp_array, hspcnt, &seqalign, NULL, cutoff_s, FALSE, search->subject_id, TRUE);
   else {
      SeqIdPtr new_sip, gi_list=NULL;
      StdSegPtr ssp, ssp_head = NULL;

      gi_list = BlastGetAllowedGis(search, search->subject_id, &new_sip);
      sip = SeqIdDup(query_id);
      if (new_sip)
         sip->next = new_sip;
      else
         sip->next = subject_id;
      for (index=0; index<hspcnt; index++) {
         hsp = hsp_array[index];
         ssp = BLASTHspToStdSeg(search, subject_length, hsp, sip, 
                                FALSE, gi_list);
         ssp->ids = SeqIdSetDup(sip);
         if (!seqalign)
            sap = seqalign = SeqAlignNew();
         else {
            sap->next = SeqAlignNew();
            sap = sap->next;
         }
         sap->type = 2;
         sap->segtype = 3;
         sap->segs = ssp;
      }
      SeqIdFree(sip);
   }

   subject_id = SeqIdSetFree(subject_id);

   for (index=0; index<hspcnt; index++) {
      if (search->current_hitlist->hsp_array[index] != NULL)
         search->current_hitlist->hsp_array[index]->gap_info = 
            GapXEditBlockDelete(search->current_hitlist->hsp_array[index]->gap_info);
   }

   if (is_translated || !search->pbp->gapped_calculation) {
      asp = MemNew(sizeof(AlignSum));
      asp->matrix = NULL;
      asp->matrix = load_default_matrix();
      asp->is_aa = !is_na;
      if (is_translated)
         AdjustOffSetsInSeqAlign(seqalign, search->query_slp, subject_slp);
   }

   /* Now print the tab-delimited fields, using seqalign */
   for (sap = seqalign; sap; sap = sap->next) {
      perc_ident = 0;
      align_length = 0;
      num_gap_opens = 0;
      num_mismatches = 0;

      GetScoreAndEvalue(sap, &score, &bit_score, &evalue, &number);

      if (evalue < 1.0e-180)
         sprintf(eval_buff, "0.0");
      else if (evalue < 1.0e-99)
         sprintf(eval_buff, "%2.0le", evalue);
      else if (evalue < 0.0009) 
         sprintf(eval_buff, "%3.1le", evalue);
      else if (evalue < 1.0) 
         sprintf(eval_buff, "%4.3lf", evalue);
      else 
         sprintf(eval_buff, "%5.1lf", evalue);
      
      if (bit_score > 9999)
         sprintf(bit_score_buff, "%4.3le", bit_score);
      else if (bit_score > 99.9)
         sprintf(bit_score_buff, "%4.1lf", bit_score);
      else
         sprintf(bit_score_buff, "%4.2lf", bit_score);

      query_seq = search->context[search->first_context].query->sequence;

      if (!is_translated && search->pbp->gapped_calculation) {
         DenseSegPtr dsp = (DenseSegPtr) sap->segs;
         numseg = dsp->numseg;
         
         for (i=0; i<numseg; i++) {
            align_length += dsp->lens[i];
            if (dsp->starts[2*i] != -1 && dsp->starts[2*i+1] != -1) {
               num_ident = BlastGetNumIdentical(query_seq, subject_seq, 
                                                dsp->starts[2*i], dsp->starts[2*i+1], 
                                                dsp->lens[i], FALSE);
               perc_ident += num_ident;
               num_mismatches += dsp->lens[i] - num_ident;
            } else
               num_gap_opens++;
         }
         perc_ident = perc_ident / align_length * 100;
         
         /* Adjust offsets for (concatenated) minus strand */
         if (is_na && dsp->starts[0] >= query_length) {
            q_start = 2*query_length - dsp->starts[2*numseg-2] - 
               dsp->lens[numseg-1] + 2;
            q_end = 2*query_length - dsp->starts[0] + 1;
            s_start = dsp->starts[2*numseg-1] + dsp->lens[numseg-1];
            s_end = dsp->starts[1]+1;
         } else {
            q_start = dsp->starts[0] + 1;
            q_end = dsp->starts[2*numseg-2] + dsp->lens[numseg-1];
            s_start = dsp->starts[1] + 1;
            s_end = dsp->starts[2*numseg-1] + dsp->lens[numseg-1];
         }
      } else {
         StdSegPtr ssp = (StdSegPtr) sap->segs;
         
         find_score_in_align(sap, 1, asp);

         if (asp->m_frame < 0)
            q_start = SeqLocStop(ssp->loc) + 1;
         else
            q_start = SeqLocStart(ssp->loc) + 1;

         if (asp->t_frame < 0)
            s_start = SeqLocStop(ssp->loc->next) + 1;
         else
            s_start = SeqLocStart(ssp->loc->next) + 1;
         
         for (index=1; ssp->next; index++)
            ssp = ssp->next;
         
         if (asp->m_frame < 0)
            q_end = SeqLocStart(ssp->loc) + 1;
         else
            q_end = SeqLocStop(ssp->loc) + 1;

         if (asp->t_frame < 0)
            s_end = SeqLocStart(ssp->loc->next) + 1;
         else
            s_end = SeqLocStop(ssp->loc->next) + 1;

         align_length = asp->totlen;
         num_gap_opens = index / 2;
         num_mismatches = asp->totlen - asp->gaps - asp->identical;
         perc_ident = ((FloatHi) 100*asp->identical)/ (asp->totlen);
      }

      if (!is_translated) {
         /* Adjust coordinates if query and/or subject is a subsequence */
         q_start += q_shift;
         q_end += q_shift;
         s_start += s_shift;
         s_end += s_shift;
      }

      if (perc_ident >= 99.995 && perc_ident < 100.00)
         perc_ident = 99.99;
         
      fprintf(fp, 
              "%s\t%s\t%.2f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\t%s\n",
              query_buffer, subject_buffer, perc_ident, align_length, 
              num_mismatches, num_gap_opens, q_start, 
              q_end, s_start, s_end, eval_buff, bit_score_buff);
   }

   if (is_translated) {
      free_default_matrix(asp->matrix);
      MemFree(asp);
   }

   if (search->rdfp)
      ReadDBBioseqFetchDisable();
   
   MemFree(subject_start);
   MemFree(rev_subject);
   SeqAlignSetFree(seqalign);
   MemFree(subject_descr);
   fflush(fp);
   return 0;
}


int LIBCALLBACK
MegaBlastPrintAlignInfo(VoidPtr ptr)
{
   BlastSearchBlkPtr search = (BlastSearchBlkPtr) ptr;
   ReadDBFILEPtr rdfp = search->rdfp;
   BLAST_HSPPtr hsp; 
   Int4 i, subject_gi;
   Int2 context;
   CharPtr query_buffer, title;
   SeqIdPtr sip, subject_id, query_id; 
   Int4 hsp_index;
   Int4 num_mismatches, num_gap_opens, align_length, num_ident;
   BLAST_KarlinBlkPtr kbp;
   Uint1Ptr query_seq, subject_seq = NULL;
   FloatHi perc_ident, bit_score;
   Char eval_buff[10], bit_score_buff[10];
   GapXEditScriptPtr esp;
   Int4 q_start, q_end, s_start, s_end, query_length, subj_length, numseg;
   Int4 q_off, q_shift = 0, s_shift = 0;
   Int4Ptr length, start;
   Uint1Ptr strands;
   CharPtr subject_descr=NULL, subject_buffer, buffer;
   Boolean numeric_sip_type = FALSE;
   FILE *fp = (FILE *) search->output;

   if (search->current_hitlist == NULL || search->current_hitlist->hspcnt <= 0) {
      search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
      return 0;
   }

   subject_seq = search->subject->sequence_start + 1;

   if (search->rdfp) {
      readdb_get_descriptor(search->rdfp, search->subject_id, &sip,
                            &subject_descr);
   } else 
      sip = SeqIdSetDup(search->subject_info->sip);

   subject_id = SeqIdFindBestAccession(sip);

   if (subject_id->choice != SEQID_GENERAL ||
       StringCmp(((DbtagPtr)subject_id->data.ptrvalue)->db, "BL_ORD_ID")) {
      if (search->pbp->mb_params->full_seqids) { 
         subject_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         SeqIdWrite(subject_id, subject_buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
      } else
         numeric_sip_type = GetAccessionFromSeqId(subject_id, &subject_gi, 
                                                  &subject_buffer);
   } else {
      subject_buffer = StringTokMT(subject_descr, " \t", &subject_descr);
      subject_descr = subject_buffer;
   }

   buffer = (CharPtr) Malloc(LARGE_BUFFER_LENGTH);

   /* Only for the two sequences case, get offset shift if subject 
      is a subsequence */
   if (!search->rdfp && search->query_slp->next)
       s_shift = SeqLocStart(search->query_slp->next);
   /* Get offset shift if query is a subsequence */
   q_shift = SeqLocStart(search->query_slp);

   for (hsp_index=0; hsp_index<search->current_hitlist->hspcnt; hsp_index++) {
      hsp = search->current_hitlist->hsp_array[hsp_index];
      if (hsp==NULL || (search->pbp->cutoff_e > 0 && 
                        hsp->evalue > search->pbp->cutoff_e)) {
         hsp->gap_info = 
            GapXEditBlockDelete(hsp->gap_info); /* Don't need it anymore */
	 continue;
      }
      context = hsp->context;
      query_id = search->qid_array[context/2];

     
      if (query_id == NULL) /* Bad hsp, something wrong */
	 continue; 
      hsp->context = context & 1;

      kbp = search->sbp->kbp[context];
      bit_score = (hsp->score*kbp->Lambda - kbp->logK) / NCBIMATH_LN2;

      query_length = search->query_context_offsets[context+1] -
         search->query_context_offsets[context] - 1;

      q_off = hsp->query.offset;
      if (hsp->context) {
	 hsp->query.end = query_length - hsp->query.offset;
	 hsp->query.offset = 
	    hsp->query.end - hsp->query.length;
         s_end = hsp->subject.offset + 1;
         s_start = hsp->subject.end;
      } else {
	 hsp->query.end = hsp->query.offset + hsp->query.length;
         s_start = hsp->subject.offset + 1;
         s_end = hsp->subject.offset + hsp->subject.length;
      }

      q_start = hsp->query.offset + 1;
      q_end = hsp->query.end;
         
      /* Adjust offsets if query is a subsequence, only for first query */
      if (context < 2) {
          q_start += q_shift;
          q_end += q_shift;
      }

      s_start += s_shift;
      s_end += s_shift;

      if (query_id->choice == SEQID_LOCAL && 
       search->pbp->mb_params->full_seqids) {
         BioseqPtr query_bsp = BioseqLockById(query_id);
         title = StringSave(BioseqGetTitle(query_bsp));
         if (title)
            query_buffer = StringTokMT(title, " ", &title);
         else {
            Int4 query_gi;
            Boolean numeric_query_id =
               GetAccessionFromSeqId(query_bsp->id, &query_gi,
                                     &query_buffer);
            if (numeric_query_id) {
               query_buffer = (CharPtr) Malloc(16);
               sprintf(query_buffer, "%ld", (long) query_gi);
            }
         }  
         BioseqUnlock(query_bsp);
      } else {
         query_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         if (!search->pbp->mb_params->full_seqids) {
            SeqIdWrite(SeqIdFindBestAccession(query_id), query_buffer,
                       PRINTID_TEXTID_ACC_VER, BUFFER_LENGTH);
         } else 
            SeqIdWrite(query_id, query_buffer, PRINTID_FASTA_LONG,
                       BUFFER_LENGTH);
      }

      query_seq = search->context[context].query->sequence;

      esp = hsp->gap_info->esp;
        
      for (numseg=0; esp; esp = esp->next, numseg++);

      GXECollectDataForSeqalign(hsp->gap_info, hsp->gap_info->esp, numseg,
				&start, &length, &strands, 
				&q_off, &hsp->subject.offset);
      hsp->gap_info = 
         GapXEditBlockDelete(hsp->gap_info); /* Don't need it anymore */

      if (start[0] < 0) {
         length[0] += start[0];
         start[1] -= start[0];
         start[0] = 0;
      } 
      if (start[2*(numseg-1)] + length[numseg-1] > query_length) 
         length[numseg-1] = query_length - start[2*(numseg-1)];
      
      perc_ident = 0;
      align_length = 0;
      num_gap_opens = 0;
      num_mismatches = 0;
      for (i=0; i<numseg; i++) {
         align_length += length[i];
	 if (start[2*i] != -1 && start[2*i+1] != -1) {
	    num_ident = MegaBlastGetNumIdentical(query_seq, subject_seq, 
                                                 start[2*i], start[2*i+1], 
                                                 length[i], FALSE);
            perc_ident += num_ident;
            num_mismatches += length[i] - num_ident;
	 } else
            num_gap_opens++;
      }
      perc_ident = perc_ident / align_length * 100;
      /* Avoid printing 100.00 when the hit is not an exact match */
      if (perc_ident >= 99.995 && perc_ident < 100.00)
         perc_ident = 99.99;

      if (perc_ident < search->pbp->mb_params->perc_identity) {
         hsp->gap_info = 
            GapXEditBlockDelete(hsp->gap_info); /* Don't need it anymore */
         MemFree(start);
         MemFree(length);
         MemFree(strands);
         MemFree(query_buffer);
         continue;
      }
      if (hsp->evalue < 1.0e-180)
         sprintf(eval_buff, "0.0");
      else if (hsp->evalue < 1.0e-99)
         sprintf(eval_buff, "%2.0le", hsp->evalue);
      else if (hsp->evalue < 0.0009) 
         sprintf(eval_buff, "%3.1le", hsp->evalue);
      else if (hsp->evalue < 1.0) 
         sprintf(eval_buff, "%4.3lf", hsp->evalue);
      else 
         sprintf(eval_buff, "%5.1lf", hsp->evalue);
      
      if (bit_score > 9999)
         sprintf(bit_score_buff, "%4.3le", bit_score);
      else if (bit_score > 99.9)
         sprintf(bit_score_buff, "%4.1lf", bit_score);
      else
         sprintf(bit_score_buff, "%4.2lf", bit_score);

      if (numeric_sip_type)
         fprintf(fp, 
                 "%s\t%ld\t%.2f\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%s\t%s\n",
                 query_buffer, (long) subject_gi, perc_ident, (long) align_length, 
                 (long) num_mismatches, (long) num_gap_opens, (long) q_start, (long) q_end, (long) s_start, 
                 (long) s_end, eval_buff, bit_score_buff);
      else 
	 fprintf(fp, 
                 "%s\t%s\t%.2f\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%s\t%s\n",
                 query_buffer, subject_buffer, perc_ident, (long) align_length, 
                 (long) num_mismatches, (long) num_gap_opens, (long) q_start, (long) q_end, (long) s_start, 
                 (long) s_end, eval_buff, bit_score_buff);

      MemFree(start);
      MemFree(length);
      MemFree(strands);
      MemFree(query_buffer);
   } /* End loop on hsp's */
   if (!numeric_sip_type && subject_buffer != subject_descr)
      MemFree(subject_buffer);
   MemFree(subject_descr);
   MemFree(buffer);
   sip = SeqIdSetFree(sip);
   fflush(fp);
   return 0;
}

void PrintTabularOutputHeader(CharPtr blast_database, BioseqPtr query_bsp,
                              SeqLocPtr query_slp, CharPtr blast_program,
                              Int4 iteration, Boolean believe_query,
                              FILE *outfp)
{
   Char buffer[BUFFER_LENGTH];
   CharPtr program, title;
   Boolean no_bioseq = (query_bsp == NULL);
   const CharPtr str = "# Query: ";
   Int4 str_len = StrLen(str);

   asn2ff_set_output(outfp, NULL);
   
   ff_StartPrint(0, 0, BUFFER_LENGTH, NULL);

   program = StringSave(blast_program);
   Nlm_StrUpper(program);
   sprintf(buffer, "# %s %s [%s]", program, BlastGetVersionNumber(),
           BlastGetReleaseDate());
   MemFree(program);
   ff_AddString(buffer);
   NewContLine();

   ff_AddString("# Database: ");
   ff_AddString(blast_database);
   NewContLine();
   
   if (iteration > 0) {
      ff_AddString("# Iteration: ");
      ff_AddString(Ltostr((long) iteration, 1));
      NewContLine();
   }

   if (query_bsp || query_slp) {
      ff_AddString(str);
      if (no_bioseq)
         query_bsp = BioseqLockById(SeqLocId(query_slp));
      if (query_bsp->id && believe_query) {
         SeqIdWrite(query_bsp->id, buffer, PRINTID_FASTA_LONG, 
                    BUFFER_LENGTH-str_len);
         if (StringNCmp(buffer, "lcl|", 4) == 0) {
            ff_AddString(buffer+4);
            str_len += StrLen(buffer) - 3;
         } else {
            ff_AddString(buffer);
            str_len += StrLen(buffer) + 1;
         }
         ff_AddChar(' ');
      }

      if ((title = BioseqGetTitle(query_bsp)) != NULL)
        sprintf(buffer, "%.*s", BUFFER_LENGTH-str_len, 
                BioseqGetTitle(query_bsp)); 
      
      ff_AddString(buffer);
      if (no_bioseq)
         BioseqUnlock(query_bsp);
      
      NewContLine();
   }
   ff_AddString("# Fields: Query id, Subject id, % identity, alignment length, mismatches, gap openings, q. start, q. end, s. start, s. end, e-value, bit score");
   ff_EndPrint();
}

Boolean FastaCheckDna(CharPtr seq)
{
    Int2 len;
    
    if (*seq == '>') {
        for (++seq; *seq != NULLB && *seq != '\n'; seq++) ;
    }
    len = MIN(100, StringLen(seq));
    return (CheckDnaResidue(seq, len, NULL));
}

BLASTHSPSegmentPtr BLASTHSPSegmentFromSeqAlign (SeqAlignPtr sap)
{
   BLASTHSPSegmentPtr hsp = (BLASTHSPSegmentPtr) 
      MemNew(sizeof(BLASTHSPSegment));
   Int4 numseg;
   DenseDiagPtr ddp;
   DenseSegPtr dsp;
   StdSegPtr ssp;

   switch(sap->segtype) {
   case SAS_DENDIAG:
      ddp = (DenseDiagPtr) sap->segs;
      if (ddp->strands == NULL || ddp->strands[0] != Seq_strand_minus) {
         hsp->q_start = ddp->starts[0] + 1;
         hsp->q_end = ddp->starts[0] + ddp->len;
      } else {
         hsp->q_start = ddp->starts[0] + ddp->len;
         hsp->q_end = ddp->starts[0] + 1;
      }
      if (ddp->strands == NULL || ddp->strands[1] != Seq_strand_minus) {
         hsp->s_start = ddp->starts[1] + 1;
         hsp->s_end = ddp->starts[1] + ddp->len;
      } else {
         hsp->s_start = ddp->starts[1] + ddp->len;
         hsp->s_end = ddp->starts[1] + 1;
      }
      break;
   case SAS_DENSEG:
      dsp = (DenseSegPtr) sap->segs;
      numseg = dsp->numseg;
      if (dsp->strands == NULL || dsp->strands[0] != Seq_strand_minus) {
         hsp->q_start = dsp->starts[0] + 1;
         hsp->q_end = dsp->starts[2*numseg-2] + dsp->lens[numseg-1];
      } else {
         hsp->q_start = dsp->starts[2*numseg-2] + 1;
         hsp->q_end = dsp->starts[0] + dsp->lens[0];
      }
      
      if (dsp->strands == NULL || dsp->strands[1] != Seq_strand_minus) {
         hsp->s_start = dsp->starts[1] + 1;
         hsp->s_end = dsp->starts[2*numseg-1] + dsp->lens[numseg-1];
      } else {
         hsp->s_start = dsp->starts[2*numseg-1] + 1;
         hsp->s_end = dsp->starts[1] + dsp->lens[0];
      }
      break;
   case SAS_STD:
      ssp = (StdSegPtr) sap->segs;
      
      if (SeqLocStrand(ssp->loc) != Seq_strand_minus)
         hsp->q_start = SeqLocStart(ssp->loc);
      else
         hsp->q_end = SeqLocStop(ssp->loc);
      if (SeqLocStrand(ssp->loc->next) != Seq_strand_minus)
         hsp->s_start = SeqLocStart(ssp->loc->next);
      else
         hsp->s_end = SeqLocStop(ssp->loc->next);

      while (ssp->next)
         ssp = ssp->next;

      if (SeqLocStrand(ssp->loc) != Seq_strand_minus)
         hsp->q_end = SeqLocStop(ssp->loc);
      else
         hsp->q_start = SeqLocStart(ssp->loc);
      if (SeqLocStrand(ssp->loc->next) != Seq_strand_minus)
         hsp->s_end = SeqLocStop(ssp->loc->next);
      else
         hsp->s_start = SeqLocStart(ssp->loc->next);
      break;
   default:
      break;
   }
   return hsp;
}

#define CLUSTER_GOOD_HIT_LTHRESH 50

SeqAlignPtr BlastClusterHitsFromSeqAlign(SeqAlignPtr seqalign, 
                                         CharPtr prog_name, 
                                         CharPtr database,
                                         BLAST_OptionsBlkPtr options,
                                         FloatHi length_thresh,
                                         FloatHi score_thresh,
                                         FloatHi overlap_thresh,
                                         Boolean two_sided)
{
   SeqAlignPtr sap, prev_sap, head, next_sap;
   SeqAlignPtr PNTR sapp;
   SeqIdPtr sip, next_sip;
   Int4 index, hspcnt, index1, q_overlap;
   ValNodePtr mask;
   Int4Ptr subj_len;
   BLASTHSPSegmentPtr PNTR hit_array;
   BLASTHSPSegmentPtr hsp, hsp1;
   BioseqPtr PNTR bspp;
   BioseqPtr bsp1, bsp2;
   BlastSearchBlkPtr search1;
   BLASTResultHitlistPtr hitlist;
   BLASTResultHspPtr best_hsp;
   BLAST_KarlinBlkPtr kbp;
   FloatHi bit_score, length_coverage, score_coverage;
   Boolean db_is_na;
   Int4 root1, root2;
   Int4Ptr root;

   for (sap = seqalign, hspcnt=0; sap; sap = sap->next, hspcnt++);
      
   sapp = (SeqAlignPtr PNTR) Malloc(hspcnt*sizeof(SeqAlignPtr));
   bspp = (BioseqPtr PNTR) Malloc(hspcnt*sizeof(BioseqPtr));
   hit_array = (BLASTHSPSegmentPtr PNTR) 
      Malloc(hspcnt*sizeof(BLASTHSPSegmentPtr));

   db_is_na = TRUE;
   if (!StrCmp(prog_name, "blastp") || !StrCmp(prog_name, "blastx"))
      db_is_na = FALSE;

   ReadDBBioseqFetchEnable ("blastall", database, db_is_na, TRUE);

   sip = next_sip = NULL;
   prev_sap = NULL;
   for (sap = seqalign, hspcnt=0; sap; sap = sap->next) {
      next_sip = TxGetSubjectIdFromSeqAlign(sap);
      if (SeqIdComp(next_sip, sip) != SIC_YES) {
         sapp[hspcnt] = sap;
         if (prev_sap)
            prev_sap->next = NULL;
         bspp[hspcnt]= BioseqLockById(next_sip);
         hit_array[hspcnt] = BLASTHSPSegmentFromSeqAlign(sap);
         hspcnt++;
         sip = next_sip;
      }         
      prev_sap = sap;
   }

   root = (Int4Ptr) Malloc(hspcnt*sizeof(Int4));
   for (index=0; index<hspcnt; index++)
      root[index] = index;

   index = 0;
   while (index<hspcnt) {
      hsp = hit_array[index];
      
      bsp1 = bspp[index];
      
      search1 = 
         BlastQuerySequenceSetUp(bsp1, prog_name, options);
      root2 = index;
      while (root[root2] != root2)
         root2 = root[root2];

      for (index1=index+1; index1<hspcnt; index1++) {
         root1 = index1;
         while (root[root1] != root1)
            root1 = root[root1];
         /* If these hits are already in one cluster, no need to compare them */
         if (root1 == root2)
            continue;
         hsp1 = hit_array[index1];
         bsp2 = bspp[index1];

         /* For blastn, check if these hits are good enough to be involved in 
            clustering, to avoid pairwise comparison of too many sequences */
         if (!StrCmp(prog_name, "blastn") && 
             MIN(hsp->q_end-hsp->q_start, hsp1->q_end-hsp1->q_start) < 
             CLUSTER_GOOD_HIT_LTHRESH)
            continue;
         /* Check if the two lengths are comparable, for the 2-sided case */
         if (two_sided && ((FloatHi)ABS(bsp1->length - bsp2->length)) / 
             MIN(bsp1->length, bsp2->length) > length_thresh)
            continue;
         /* Check if the next hit passes a simple query overlap test to be a
            candidate to belong to this cluster */
         q_overlap = 
            MIN(hsp->q_end, hsp1->q_end) - 
            MAX(hsp->q_start, hsp1->q_start);
         if (((FloatHi)q_overlap) / 
             (MAX(hsp->q_end-hsp->q_start, hsp1->q_end-hsp1->q_start) + 1) <
             overlap_thresh)
            continue;
         
         /* Do the two sequences search to determine whether this 
            candidate in fact belongs to this cluster */
         search1 = BlastSequencesOnTheFlyEx(search1, bsp2); 
         
         if (search1) {
            hitlist = search1->result_struct->results[0];
            if (hitlist && hitlist->hspcnt > 0) {
               best_hsp = &hitlist->hsp_array[0];
               if (StrCmp(prog_name, "blastn") &&
                   search1->pbp->gapped_calculation)
                  kbp = search1->sbp->kbp_gap[search1->first_context];
               else
                  kbp = search1->sbp->kbp[search1->first_context]; 
               bit_score = ((hitlist->high_score *
                             kbp->Lambda) - kbp->logK)/NCBIMATH_LN2;

               if (two_sided)
                  length_coverage = MIN(((FloatHi)best_hsp->query_length) / 
                                        bsp1->length, 
                                        ((FloatHi)best_hsp->subject_length) / 
                                        bsp2->length);
               else
                  length_coverage = MAX(((FloatHi)best_hsp->query_length) / 
                                        bsp1->length, 
                                        ((FloatHi)best_hsp->subject_length) / 
                                        bsp2->length);
               score_coverage = bit_score /
                  MAX(best_hsp->query_length, best_hsp->subject_length);
               if (score_coverage > score_thresh && 
                   length_coverage > length_thresh) {
                  /* remove the respective hit */
                  root1 = index1;
                  while (root[root1] != root1)
                     root1 = root[root1];
                  root[root1] = root[root2];
               }
            }
         }
      }
      mask = search1->mask;
      while (mask) {
         SeqLocSetFree(mask->data.ptrvalue);
         mask = mask->next;
      }
      ValNodeFree(search1->mask);
      search1 = BlastSearchBlkDestruct(search1);
      for (++index; index<hspcnt && hit_array[index]==NULL; index++);
   }

   /* Remove the unneeded seqaligns */
   head = NULL;

   for (index=0; index<hspcnt; index++) {
      if (root[index] == index) {
         if (!head) {
            head = (SeqAlignPtr) Malloc(sizeof(SeqAlign));
            MemCpy(head, sapp[index], sizeof(SeqAlign));
            for (sap = head; sap->next; sap = sap->next) {
               next_sap = sap->next;
               sap->next = (SeqAlignPtr) Malloc(sizeof(SeqAlign));
               MemCpy(sap->next, next_sap, sizeof(SeqAlign));
            }
            sapp[index] = head;
         } else {
            sap->next = sapp[index];
            for (index1=0; sap->next; sap = sap->next, index1=1) {
               next_sap = sap->next;
               sap->next = (SeqAlignPtr) Malloc(sizeof(SeqAlign));
               MemCpy(sap->next, next_sap, sizeof(SeqAlign));
               if (index1==0)
                  sapp[index] = sap->next;
            }
         }
      } else {
         if (!sapp[root[index]]->master) {
            sapp[root[index]]->master = 
               SeqIdDup(TxGetSubjectIdFromSeqAlign(sapp[index]));
         } else {
            sip = sapp[root[index]]->master;
            while (sip->next)
               sip = sip->next;
            sip->next = SeqIdDup(TxGetSubjectIdFromSeqAlign(sapp[index]));
         }
      }
   }

   for (index=0; index<hspcnt; index++)
      BioseqUnlock(bspp[index]);
   MemFree(bspp);

   ReadDBBioseqFetchDisable();
  
   return head;
}

SeqAlignPtr BLASTPostSearchLogic(BlastSearchBlkPtr search,BLAST_OptionsBlkPtr options)
{
  StdSegPtr ssp;
  BLASTResultHspPtr hsp;
  GapXEditBlockPtr edit_block;
  SeqIdPtr gi_list=NULL,subject_id;
  SeqAlignPtr head,seqalign,seqalign_var,sap;
  BLASTResultsStructPtr result_struct;
  BLASTResultHitlistPtr   result_hitlist;
  Uint1Ptr sequence;
  Int4 length,hitlist_count,sequence_length,index,index1,total_num_hsp=0,hspcnt;
  Char buffer[512];
  
  
  head = NULL;
  
  if (search->prog_number==blast_type_blastn) {
    /* Unconcatenate the strands by adjusting the query offsets in
       all hsps */
    search->context[search->first_context].query->length = 
      search->query_context_offsets[search->first_context+1] - 1;
  }

	if (StringCmp(search->prog_name, "blastn") == 0 && 
		search->pbp->gapped_calculation)
        {
		search->sbp->kbp_gap[search->first_context] = search->sbp->kbp[search->first_context];

		search->pbp->gap_open = options->gap_open;
		search->pbp->gap_extend = options->gap_extend;
/*
		search->pbp->gap_x_dropoff = (BLAST_Score) (options->gap_x_dropoff*NCBIMATH_LN2 / search->sbp->kbp_gap[search->first_context]->Lambda);
		search->pbp->gap_x_dropoff_final = (BLAST_Score) (options->gap_x_dropoff_final*NCBIMATH_LN2 / search->sbp->kbp_gap[search->first_context]->Lambda);
*/


		result_struct = search->result_struct;
       		hitlist_count = result_struct->hitlist_count;

		sequence=NULL;
		sequence_length=0;
		
		for (index=0; index<hitlist_count; index++)
		{
			length = readdb_get_sequence_ex(search->rdfp, result_struct->results[index]->subject_id, &sequence, &sequence_length, TRUE);
			seqalign = SumBlastGetGappedAlignmentTraceback(search, index, FALSE, FALSE, sequence+1, length);
			result_struct->results[index]->seqalign = seqalign;
			total_num_hsp += result_struct->results[index]->hspcnt;
			BLASTResultFreeHsp(result_struct->results[index]);
			if (search->pbp->total_hsp_limit > 0 && total_num_hsp > search->pbp->total_hsp_limit)
			{
				sprintf(buffer, "Only alignments to %ld best database sequences returned", (long) index+1);
				BlastConstructErrorMessage("EngineCore", buffer, 1, &(search->error_return));
				break;
			}	
		}
		sequence = MemFree(sequence);
		search->sbp->kbp_gap[search->first_context] = NULL;

		HeapSort(result_struct->results, hitlist_count, sizeof(BLASTResultHitlistPtr), evalue_compare_hits);

		  /* 
		     The next loop organizes the SeqAligns (and the alignments in the
		     BLAST report) in the same order as the deflines.
		  */
		  head = NULL;
		  for (index=0; index<hitlist_count; index++)
		    {
		      seqalign = result_struct->results[index]->seqalign;
		      if (seqalign)
			{
			  if (head == NULL)
			    {
			      head = seqalign;
			    }
			  else
			    {
			      for (seqalign_var=head; seqalign_var->next;)
				seqalign_var = seqalign_var->next;
			      seqalign_var->next = seqalign;
			    }
			}
		    }
	}
	else if (search->pbp->gapped_calculation)
	{
		result_struct = search->result_struct;
                hitlist_count = result_struct->hitlist_count;

		if (!(options->smith_waterman)) {
                    
                    for (index=0; index<hitlist_count; index++) {
                        seqalign = BlastGetGapAlgnTbckWithReaddb(search, index, FALSE);
                        result_struct->results[index]->seqalign = seqalign;
                        /*BLASTResultFreeHsp(result_struct->results[index]);*/
			total_num_hsp += result_struct->results[index]->hspcnt;
			if (search->pbp->total_hsp_limit > 0 && total_num_hsp > search->pbp->total_hsp_limit)
			{
				sprintf(buffer, "Only alignments to %ld best database sequences returned", (long) index+1);
				BlastConstructErrorMessage("EngineCore", buffer, 1, &(search->error_return));
				break;
			}	
                    }
                    
                    HeapSort(result_struct->results, hitlist_count, sizeof(BLASTResultHitlistPtr), evalue_compare_hits);
                }

		/* 
		The next loop organizes the SeqAligns (and the alignments in the
		BLAST report) in the same order as the deflines.
		*/

		if (options->tweak_parameters) {
                  /*restore settings of number of matches and E-value threshold
                    to what user requested*/
		  options->hitlist_size = options->hitlist_size/2;
		  options->expect_value = search->pbp->cutoff_e = options->original_expect_value;
		}

		if ((options->tweak_parameters) || (options->smith_waterman))
                  head = RedoAlignmentCore(search, options, hitlist_count, 
		       options->tweak_parameters, options->smith_waterman);

                for (index=0; index<hitlist_count; index++)
                {
		     BLASTResultFreeHsp(result_struct->results[index]);
                }

		if ((!(options->tweak_parameters)) && 
		    (!(options->smith_waterman))) {


		  head = NULL;
		  for (index=0; index<hitlist_count; index++)
		    {
		      seqalign = result_struct->results[index]->seqalign;
		      if (seqalign)
			{
			  if (head == NULL)
			    {
			      head = seqalign;
			    }
			  else
			    {
			      for (seqalign_var=head; seqalign_var->next;)
				seqalign_var = seqalign_var->next;
			      seqalign_var->next = seqalign;
			    }
			}
		    }
		} else {
                    /* We eventually have to free unused seqalign */
                    for (index=0; index<hitlist_count; index++) {
                        seqalign = result_struct->results[index]->seqalign;
                        SeqAlignSetFree(seqalign);
                    }
                }
	}
	else
	{
		/* Ungapped psi-blast. */
		if (StringCmp("blastp", search->prog_name) == 0 && search->positionBased == TRUE)
		{
		      result_struct = search->result_struct;
                      hitlist_count = result_struct->hitlist_count;
		      for (index=0; index<hitlist_count; index++)
		      {
			  result_hitlist = result_struct->results[index];
		      	  gi_list = BlastGetAllowedGis(search, result_hitlist->subject_id, NULL);
			  hspcnt = result_hitlist->hspcnt;
			  subject_id = BlastGetSubjectId(search, index, TRUE, NULL);
			  for (index1=0; index1<hspcnt; index1++)
			    {
			      hsp = &(result_hitlist->hsp_array[index1]);
			      edit_block = SimpleIntervalToGapXEditBlock(hsp->query_offset, hsp->subject_offset, hsp->query_length);
			      seqalign = GapXEditBlockToSeqAlign(edit_block, SeqIdDup(subject_id), SeqIdDup(search->query_id));
                              seqalign->score = GetScoreSetFromBlastResultHsp(hsp, gi_list);
			      
                              if (seqalign->segtype == 3) {
                                  ssp = seqalign->segs;
                                  while (ssp) {
                                      ssp->scores = GetScoreSetFromBlastResultHsp(hsp, gi_list);
                                      ssp = ssp->next;
                                  }
                              } else if (seqalign->segtype == 5) { /* Discontinuous */
                                  sap = (SeqAlignPtr) seqalign->segs;
                                  for(;sap != NULL; sap = sap->next) {
                                      sap->score = GetScoreSetFromBlastResultHsp(hsp, gi_list);
                                  }
                              }

			      if (head == NULL)
				{
				  head = seqalign;
				}
			      else
				{
				  for (seqalign_var=head; seqalign_var->next;)
				    seqalign_var = seqalign_var->next;
				  seqalign_var->next = seqalign;
				}
				total_num_hsp += result_struct->results[index]->hspcnt;
				if (search->pbp->total_hsp_limit > 0 && total_num_hsp > search->pbp->total_hsp_limit)
				{
					sprintf(buffer, "Only alignments to %ld best database sequences returned", (long) index+1);
					BlastConstructErrorMessage("EngineCore", buffer, 1, &(search->error_return));
					break;
				}	
			    }
			}
		} /* end blastp */
		else
		{
		    if (StringCmp("blastn", search->prog_name) == 0 || StringCmp("blastp", search->prog_name) == 0)
			head = GetSeqAlignForResultHitList(search, TRUE, FALSE, options->discontinuous, FALSE, FALSE);
		    else
			head = GetSeqAlignForResultHitList(search, FALSE, FALSE, options->discontinuous, FALSE, FALSE);
		}
	}

	gi_list = SeqIdSetFree(gi_list);
	return head;

}
