static char const rcsid[] = "$Id: megablast.c,v 6.158 2005/04/27 14:55:09 papadopo Exp $";
/* $Id: megablast.c,v 6.158 2005/04/27 14:55:09 papadopo Exp $
**************************************************************************
*                                                                         *
*                             COPYRIGHT NOTICE                            *
*                                                                         *
* This software/database is categorized as "United States Government      *
* Work" under the terms of the United States Copyright Act.  It was       *
* produced as part of the author's official duties as a Government        *
* employee and thus can not be copyrighted.  This software/database is    *
* freely available to the public for use without a copyright notice.      *
* Restrictions can not be placed on its present or future use.            *
*                                                                         *
* Although all reasonable efforts have been taken to ensure the accuracy  *
* and reliability of the software and data, the National Library of       *
* Medicine (NLM) and the U.S. Government do not and can not warrant the   *
* performance or results that may be obtained by using this software,     *
* data, or derivative works thereof.  The NLM and the U.S. Government     *
* disclaim any and all warranties, expressed or implied, as to the        *
* performance, merchantability or fitness for any particular purpose or   *
* use.                                                                    *
*                                                                         *
* In any work or product derived from this material, proper attribution   *
* of the author(s) as the source of the software or data would be         *
* appreciated.                                                            *
*                                                                         *
************************************************************************** 
 * $Log: megablast.c,v $
 * Revision 6.158  2005/04/27 14:55:09  papadopo
 * change signature of BlastFillHitSavingOptions
 *
 * Revision 6.157  2005/04/26 21:47:24  dondosha
 * Added ObjMgrFreeCache call for tabular formatting
 *
 * Revision 6.156  2005/03/08 21:20:02  dondosha
 * BlastTabularFormatDataNew signature changed
 *
 * Revision 6.155  2005/03/02 20:29:01  dondosha
 * Renamed Blast_FindRepeatFilterLoc to Blast_FindRepeatFilterSeqLoc, to make name different from C++ toolkit counterpart
 *
 * Revision 6.154  2005/02/08 20:43:03  dondosha
 * Added repeats filtering for new engine
 *
 * Revision 6.153  2005/02/07 15:56:34  dondosha
 * 1. Made believe_query option boolean as it should be;
 * 2. In new engine, set believe query as -J option specifies;
 * 3. Do not free dbname, since it is not allocated.
 *
 * Revision 6.152  2005/02/03 18:02:07  dondosha
 * Pass summary returns to BLAST_FormatResults, needed for XML output
 *
 * Revision 6.151  2005/02/02 19:01:36  dondosha
 * Use new high level API for performing the search
 *
 * Revision 6.150  2005/01/10 13:49:25  madden
 *  Database scanning stride, container type, extension method all set automatically, change to BLAST_FillInitialWordOptions prototype
 *
 * Revision 6.149  2004/12/21 18:09:59  dondosha
 * Removed rps_info argument in BlastPrelimSearchThreadDataInit since it is not needed
 *
 * Revision 6.148  2004/12/21 18:04:39  dondosha
 * Use Blast_RunPreliminarySearch function for tabular run; BLAST_SearchEngine renamed to Blast_RunFullSearch
 *
 * Revision 6.147  2004/12/11 15:21:54  dondosha
 * Ideal values calculation is now done outside of Blast_ScoreBlkMatrixInit
 *
 * Revision 6.146  2004/12/09 15:30:02  dondosha
 * BlastScoreBlkMatrixInit renamed to Blast_ScoreBlkMatrixInit
 *
 * Revision 6.145  2004/11/27 18:26:38  coulouri
 * Indicate that single base scanning may only be used with discontiguous words
 *
 * Revision 6.144  2004/11/24 14:26:37  camacho
 * Remove unused variable
 *
 * Revision 6.143  2004/11/24 14:26:05  camacho
 * Retrieve ideal lambda from BlastScoreBlk::ksp_ideal
 *
 * Revision 6.142  2004/11/01 18:45:22  madden
 * From Mike Gertz:
 * - Changed a call to BLAST_FillHitSavingOptions in BLAST_FillOptions
 *      to pass the new is_gapped parameter.
 *
 * Revision 6.141  2004/10/06 19:12:24  dondosha
 * Added #include for blast_tback.h
 *
 * Revision 6.140  2004/10/06 15:02:19  dondosha
 * Free BlastSeqSrc before creating Seq-align
 *
 * Revision 6.139  2004/10/04 14:05:30  madden
 * Use Blast_PrintOutputFooter rather than BLAST_PrintSummaryReturns
 *
 * Revision 6.138  2004/09/13 15:16:12  madden
 * Use BlastSeqLoc rather than ListNode
 *
 * Revision 6.137  2004/08/31 17:05:09  dondosha
 * BlastSeqSrc is no longer used on seqalign creation and formatting stages
 *
 * Revision 6.136  2004/08/17 20:32:00  madden
 * Enable ARG_QUERYLOC for new engine, add function Megablast_GetLoc to parse location
 *
 * Revision 6.135  2004/08/17 13:40:15  madden
 * Changes to support discontiguous megablast and perform ungapped search before a greedy gapped alignment
 *
 * Revision 6.134  2004/08/12 15:59:24  madden
 * Fix problem with relative database path for new engine, eliminate use of SeqSrc to get database name
 *
 * Revision 6.133  2004/08/12 13:41:51  madden
 * Enable use of mulitple threads for new engine
 *
 * Revision 6.132  2004/08/05 14:18:08  dondosha
 * Set ag_blast argument for options initialization to FALSE for non-blastn searches
 *
 * Revision 6.131  2004/08/04 14:32:33  dondosha
 * Added new argument to BLAST_ValidateOptions call for new engine
 *
 * Revision 6.130  2004/08/03 16:45:07  madden
 * Remove redundant include
 *
 * Revision 6.129  2004/07/30 14:46:08  madden
 * Set greedy Boolean before calling BLAST_FillScoringOptions
 *
 * Revision 6.128  2004/07/27 20:20:08  madden
 * Use -V to force use of old engine
 *
 * Revision 6.127  2004/07/27 19:18:58  madden
 * Allow use of BLAST DB with oidlist with new engine
 *
 * Revision 6.126  2004/07/26 18:32:03  madden
 * Moved call to BLAST_FillScoringOptions before call to GetLambdaFast
 *
 * Revision 6.125  2004/07/19 18:16:40  madden
 * Remove include for seqsrc_multiseq.h, not needed
 *
 * Revision 6.124  2004/07/19 16:43:52  dondosha
 * Renamed multiseq_src to seqsrc_multiseq
 *
 * Revision 6.123  2004/07/15 20:23:29  madden
 * Check status of Blast_RunFullSearch, ErrPostEx if status non-zero
 *
 * Revision 6.122  2004/07/15 12:20:22  madden
 * Remove or replace (with ErrPostEx) fprintf to stderr
 *
 * Revision 6.121  2004/07/14 13:54:48  madden
 * Optionally use new engine for searches
 *
 * Revision 6.120  2004/07/09 20:18:38  madden
 * Init mqfp to NULL to prevent crash if unused
 *
 * Revision 6.119  2004/07/08 16:50:55  dondosha
 * Tiny addition to previous check-in: added file name in warning message
 *
 * Revision 6.118  2004/07/08 16:48:06  dondosha
 * Emit a warning if unable to open file for masked query, and do not attempt to write to it then
 *
 * Revision 6.117  2004/07/08 15:50:59  dondosha
 * Added comment that -Q option must be used together with -D2
 *
 * Revision 6.116  2004/06/30 12:51:37  madden
 * Use enums (BlastArguments) for arguments, these are consistent with the ones used in blast_driver.c
 *
 * Revision 6.115  2004/06/30 12:33:30  madden
 * Add include for blfmtutl.h
 *
 * Revision 6.114  2004/05/27 17:37:30  dondosha
 * Do not call GapXEditBlockDelete in formatting callback - this is now done when HSPs are freed
 *
 * Revision 6.113  2004/04/29 19:56:00  dondosha
 * Mask filtered locations in query sequence lines in XML output
 *
 * Revision 6.112  2004/03/24 22:55:54  dondosha
 * Ignore -m option when on-the-fly tabular output is returned
 *
 * Revision 6.111  2004/01/27 20:47:18  dondosha
 * Set no_traceback option to 2 for -D4, 1 for -D0, 0 for others
 *
 * Revision 6.110  2003/11/05 22:28:06  dondosha
 * No need to shift subsequence coordinates in tabular output, since they are already shifted in the seqalign
 *
 * Revision 6.109  2003/10/30 17:30:49  dondosha
 * Fixed error in previous commit - output file pointer has to be set for all cases
 *
 * Revision 6.108  2003/10/29 17:48:15  dondosha
 * Added -D4 option for 2-stage greedy gapped extension with format identical to -D2
 *
 * Revision 6.107  2003/05/30 17:31:09  coulouri
 * add rcsid
 *
 * Revision 6.106  2003/05/13 16:02:42  coulouri
 * make ErrPostEx(SEV_FATAL, ...) exit with nonzero status
 *
 * Revision 6.105  2003/05/06 18:57:46  dondosha
 * Do not set cutoff_s for megablast, it is not needed
 *
 * Revision 6.104  2003/05/06 15:18:22  dondosha
 * Fix in callback function for the -D1 option
 *
 * Revision 6.103  2003/03/20 13:44:24  madden
 * Fix -m 10/11 output to make them SeqAnnots
 *
 * Revision 6.102  2003/03/13 15:26:15  dondosha
 * Close output file; call ReadDBBioseqFetchEnable only when non-empty seqalign is found
 *
 * Revision 6.101  2002/12/31 22:47:16  boemker
 * Added support for printing output as ASN (text, with -m 10, or binary, with
 * -m 11).
 *
 * Revision 6.100  2002/08/09 19:41:25  camacho
 * 1) Added blast version number to command-line options
 * 2) Added explanations for some default parameters
 *
 * Revision 6.99  2002/07/18 19:44:40  dondosha
 * Clarification in the description of max. HSPs option
 *
 * Revision 6.98  2002/07/18 19:40:45  dondosha
 * Added an option to restrict number of HSPs per database sequence
 *
 * Revision 6.97  2002/07/17 22:28:13  dondosha
 * Added support for megablast XML output
 *
 * Revision 6.96  2002/06/19 22:50:17  dondosha
 * Added all queries information for tabular output with multiple queries
 *
 * Revision 6.95  2002/05/14 22:21:36  dondosha
 * Renamed maximal discontiguous template type into optimal
 *
 * Revision 6.94  2002/05/09 15:37:52  dondosha
 * Call BLASTOptionNewEx instead of BLASTOptionNew, so megablast defaults are set in a central place
 *
 * Revision 6.93  2002/05/08 22:46:21  dondosha
 * Allocate query bioseq array 1 longer to make sure last ellement is NULL
 *
 * Revision 6.92  2002/05/04 13:04:43  madden
 * Unsuppress options
 *
 * Revision 6.91  2002/04/29 19:55:25  madden
 * Use ARG_FLOAT for db length
 *
 * Revision 6.90  2002/04/25 21:57:46  madden
 * Strip options for release
 *
 * Revision 6.89  2002/04/24 19:55:14  madden
 * Rolled back last change
 *
 * Revision 6.88  2002/04/23 20:58:53  madden
 * Suppress options for release
 *
 * Revision 6.87  2002/04/09 18:17:34  dondosha
 * Added a discontiguous word type option
 *
 * Revision 6.86  2002/03/08 20:22:56  dondosha
 * Bug fix: masking locations were freed incorrectly in some cases
 *
 * Revision 6.85  2002/03/06 18:34:32  dondosha
 * Pass the filtered locations back from the megablast engine to use in formatting
 *
 * Revision 6.84  2002/02/15 23:30:00  dondosha
 * 1. Fix for very long queries
 * 2. Added -m8 and -m9 options - needed in conjunction with -Q option.
 *
 * Revision 6.83  2001/12/28 20:42:16  dondosha
 * Added options related to discontiguous words
 *
 * Revision 6.82  2001/11/15 12:37:26  dondosha
 * Error in previous commit - comma in wrong place
 *
 * Revision 6.81  2001/11/14 23:37:18  dondosha
 * Added parameters for ungapped and non-greedy gapped x-dropoffs
 *
 * Revision 6.80  2001/09/06 15:40:54  dondosha
 * Uncommented call to PrintTabularOutputHeader
 *
 * Revision 6.79  2001/08/08 22:38:16  dondosha
 * Added protection against endpoints beyond end of sequence for -D0 and -D1 outputs
 *
 * Revision 6.78  2001/07/27 21:47:36  dondosha
 * Fixed dummy variable declaration for call to StringToInt8
 *
 * Revision 6.77  2001/07/26 18:22:29  dondosha
 * Changed the effective length argument from float to string
 *
 * Revision 6.76  2001/07/20 18:47:21  dondosha
 * Scale cutoff_s2 if match reward not 1
 *
 * Revision 6.75  2001/07/03 20:50:33  madden
 * Commented out call to PrintTabularOutputHeader
 *
 * Revision 6.74  2001/06/28 21:23:33  dondosha
 * Print header for the -D3 output; added window size argument -A
 *
 * Revision 6.73  2001/06/21 21:49:55  dondosha
 * No need to declare extra variable vnp
 *
 * Revision 6.72  2001/06/21 21:43:57  dondosha
 * Destroy error returns
 *
 * Revision 6.71  2001/05/25 19:30:00  vakatov
 * Nested comment typo fixed
 *
 * Revision 6.70  2001/04/26 14:10:06  egorov
 * Call ReadDBBioseqFetchEnable function before fetching sequences.
 *
 * Revision 6.69  2001/03/19 22:39:25  dondosha
 * Allow location on the first query sequence for megablast
 *
 * Revision 6.68  2001/02/21 21:26:54  dondosha
 * Prefer gi to accession in -D[01] outputs
 *
 * Revision 6.67  2001/02/07 21:18:42  dondosha
 * Moved the MegaBlastPrintAlignInfo callback to blastool.c
 *
 * Revision 6.66  2001/02/05 16:48:34  dondosha
 * Parse all database general ids except BL_ORD_ID
 *
 * Revision 6.65  2001/01/12 17:48:05  dondosha
 * Check for NULL seqalign array on return from engine
 *
 * Revision 6.64  2001/01/05 17:12:49  dondosha
 * Correction in previous memory leak fix
 *
 * Revision 6.63  2001/01/03 21:45:31  dondosha
 * Fixed a memory leak - some edit blocks not freed in megablast
 *
 * Revision 6.62  2000/12/22 19:50:37  dondosha
 * Enabled percent identity cutoff for -D1 output format
 *
 * Revision 6.61  2000/12/21 22:33:05  dondosha
 * Added option to cut off results by identity percentage
 *
 * Revision 6.60  2000/11/29 16:28:06  dondosha
 * No need to extract ncbi4na-encoded subject from readdb any more
 *
 * Revision 6.59  2000/11/21 21:37:09  dondosha
 * Make -J option default FALSE for traditional output
 *
 * Revision 6.58  2000/11/20 22:13:37  shavirin
 * Changed name of the program to "megablast" for XML output.
 *
 * Revision 6.57  2000/11/20 17:02:06  dondosha
 * Minor corrections in command line option descriptions
 *
 * Revision 6.56  2000/11/08 18:30:20  kans
 * includes <sqnutils.h> for Mac compiler
 *
 * Revision 6.55  2000/11/03 20:18:12  dondosha
 * The adjustment of offsets and sorting of HSP array done before printing results in callbacks
 *
 * Revision 6.54  2000/10/31 15:08:05  dondosha
 * With -Q option print all query sequences
 *
 * Revision 6.53  2000/10/27 19:14:41  madden
 * Change description of -b option
 *
 * Revision 6.52  2000/10/26 18:54:12  dondosha
 * Added printing of reference to the traditional output
 *
 * Revision 6.51  2000/10/25 16:20:54  dondosha
 * Whenever possible, print accession in the -D3 output
 *
 * Revision 6.50  2000/10/23 19:59:03  dondosha
 * Added ability to print XML output
 *
 * Revision 6.49  2000/10/19 16:06:25  dondosha
 * Added a new output format, option -D 3, changed wordsize definition
 *
 * Revision 6.48  2000/10/18 17:05:42  dondosha
 * Added command line option -R to report log info at end of output
 *
 * Revision 6.47  2000/10/16 15:09:37  dondosha
 * Changed default for -U option to FALSE
 *
 * Revision 6.46  2000/10/13 20:15:58  dondosha
 * If local query id and no title in Bioseq, get it from the id
 *
 * Revision 6.45  2000/10/02 15:57:38  dondosha
 * Free query id buffer fo each HSP when it is printed
 *
 * Revision 6.44  2000/10/02 14:50:08  dondosha
 * Improvement to previous change
 *
 * Revision 6.43  2000/09/27 21:57:12  dondosha
 * Corrected one-line output for databases with ids of general type
 *
 * Revision 6.42  2000/09/20 19:12:10  dondosha
 * Print only IDs for subject sequences in -D[01] output with -f T option
 *
 * Revision 6.41  2000/09/19 17:09:17  dondosha
 * Removed restriction on query deflines length
 *
 * Revision 6.40  2000/09/06 21:17:38  dondosha
 * Added a -U option to allow ignoring of lower case
 *
 * Revision 6.39  2000/09/06 18:35:52  dondosha
 * Corrected percentage of identities computation for -D1 output
 *
 * Revision 6.38  2000/08/25 16:57:58  dondosha
 * Corrected comment for the -Q option
 *
 * Revision 6.37  2000/08/24 14:34:05  shavirin
 * Added return 1 if database do not exists on any path.
 *
 * Revision 6.36  2000/08/18 20:15:57  dondosha
 * Print only id, not full defline with -fT option
 *
 * Revision 6.35  2000/08/15 15:21:00  dondosha
 * If readdb returns no subject description, retrieve it from the id
 *
 * Revision 6.34  2000/08/10 17:12:45  dondosha
 * Improved the HTML output
 *
 * Revision 6.33  2000/08/03 17:50:49  dondosha
 * Check HSPs for going beyond ends of query in megablast
 *
 * Revision 6.32  2000/07/17 22:01:22  dondosha
 * Returned default for -J option to TRUE
 *
 * Revision 6.31  2000/07/17 21:53:37  dondosha
 * Set believe query to FALSE if want to print full defline
 *
 * Revision 6.30  2000/07/17 16:55:33  dondosha
 * Fixed uninitialized variable bug
 *
 * Revision 6.29  2000/07/14 18:04:09  dondosha
 * Set perform_culling option explicitly to FALSE; fix for the full defline id printing
 *
 * Revision 6.28  2000/07/11 16:47:33  dondosha
 * Save lower case masking information in options structure
 *
 * Revision 6.27  2000/07/06 17:28:11  dondosha
 * Added option to print full deflines in the output
 *
 * Revision 6.26  2000/06/28 18:30:53  dondosha
 * Another uninitialized variable in revision 6.24
 *
 * Revision 6.25  2000/06/28 15:57:16  dondosha
 * Fixed uninitialized variable bug from previous change
 *
 * Revision 6.24  2000/06/27 22:24:42  dondosha
 * Added option to output masked query sequences
 *
 * Revision 6.23  2000/06/22 14:30:21  dondosha
 * Added option to cut off hits by score
 *
 * Revision 6.22  2000/06/19 22:13:57  dondosha
 * Do not allow searching more than 1/2 INT2_MAX queries at a time (since last_context is an Int2)
 *
 * Revision 6.21  2000/06/15 17:35:34  dondosha
 * Removed program argument, changed master-slave to query-anchored
 *
 * Revision 6.20  2000/06/07 18:20:30  dondosha
 * Append result ASN.1 for all queries if seqalign file is specified
 *
 * Revision 6.19  2000/06/01 15:48:56  dondosha
 * Removed -K and -Y command line options
 *
 * Revision 6.18  2000/05/31 14:23:16  dondosha
 * Hold indexing of Bioseqs until after all of them are read
 *
 * Revision 6.17  2000/05/24 20:35:05  dondosha
 * Set cutoff_s parameter to wordsize - needed for reaping hitlists by evalue
 *
 * Revision 6.16  2000/05/17 21:28:12  dondosha
 * Fixed several memory leaks
 *
 * Revision 6.15  2000/05/17 17:40:54  dondosha
 * Removed unused variables; improved the way subject ids are printed in report; added maximal number of positions for a hash value parameter
 *
 * Revision 6.14  2000/05/12 19:52:53  dondosha
 * Use binary search to retrieve query id for printing results; increased maximal total query length default; do not free SeqEntries after last search
 *
 * Revision 6.13  2000/05/05 14:23:38  dondosha
 * Changed program name from mblastall to megablast
 *
 * Revision 6.12  2000/05/03 20:30:07  dondosha
 * Fixed memory leaks, changed score to number of differences for one-line output, added affine gapping
 *
 * Revision 6.11  2000/04/25 22:04:51  dondosha
 * Report number of differences instead of score in -D [01] mode
 *
 * Revision 6.10  2000/04/24 16:45:58  dondosha
 * Check evalues of hsps in the callbacks; default expect value 0 means it is ignored
 *
 * Revision 6.9  2000/04/21 19:43:29  dondosha
 * Added command line option: total length of queries for a single search
 *
 * Revision 6.8  2000/04/12 22:48:47  dondosha
 * Fixed synchronization problem in writing segments information
 *
 * Revision 6.7  2000/04/12 21:19:20  dondosha
 * Cleaned argument list
 *
 * Revision 6.6  2000/04/12 18:29:52  dondosha
 * Added callback MegaBlastPrintSegments, made BlastSearchHandleResults static
 *
 * Revision 6.5  2000/04/07 16:54:58  dondosha
 * Added option for segments only output; moved result handling callbacks from 
 * mblast.c
 *
 * Revision 6.4  2000/03/31 19:13:41  dondosha
 * Changed some names related to MegaBlast
 *
 * Revision 6.3  2000/02/17 17:42:58  dondosha
 * Added extra parameter to FastaToSeqEntryForDb call due to prototype change;
 * do not fill mask_loc to avoid sequence substitution when filtering
 *
 * Revision 6.2  2000/02/03 22:02:53  dondosha
 * Fixed some memory leaks, added option for one-line results printout
 *
 * Revision 6.1  2000/02/01 21:41:14  dondosha
 * Initial revision
 *
 *
*/

#define MB_ALLOW_NEW 1

#include <objsset.h>
#include <blast.h>
#include <txalign.h>
#include <mblast.h>
#include <xmlblast.h>
#include <sqnutils.h>
#include <blfmtutl.h>
#if MB_ALLOW_NEW
#include <algo/blast/api/blast_input.h>
#include <algo/blast/api/blast_format.h>
#include <algo/blast/api/blast_tabular.h>
#include <algo/blast/api/blast_api.h>
#include <algo/blast/api/blast_seq.h>
#include <algo/blast/api/repeats_filter.h>
#endif

#define DEFLINE_BUF 255


/* Used by the callback function. */
FILE *global_fp=NULL;
/*
	Callback to print out ticks, in UNIX only due to file systems
	portability issues.
*/
static int LIBCALLBACK
dummy_callback(Int4 sequence_number, Int4 number_of_positive_hits)
{
   return 0;
}

static int LIBCALLBACK
tick_callback(Int4 sequence_number, Int4 number_of_positive_hits)

{

#ifdef OS_UNIX
   
   fprintf(global_fp, "%s", ".");
   fflush(global_fp);
#endif
	return 0;
}

enum {
   MBLAST_ENDPOINTS = 0,
   MBLAST_SEGMENTS,
   MBLAST_ALIGNMENTS,
   MBLAST_ALIGN_INFO,
   MBLAST_DELAYED_TRACEBACK
};

static int LIBCALLBACK
MegaBlastPrintEndpoints(VoidPtr ptr)
{
   BlastSearchBlkPtr search = (BlastSearchBlkPtr) ptr;
   CharPtr subject_descr;
   SeqIdPtr sip, query_id;
   CharPtr query_buffer, title;
   CharPtr subject_buffer;
   Int4 query_length, q_start, q_end, q_shift=0, s_shift=0;
   Int4 subject_end;
   Int4 hsp_index;
   Boolean numeric_sip_type = FALSE;
   BLAST_HSPPtr hsp; 
   Int2 context;
   Char context_sign;
   Uint4 header_index = 0;
   Int4 subject_gi, score;
   FILE *fp = (FILE *) search->output;

   if (search->current_hitlist == NULL || search->current_hitlist->hspcnt <= 0) {
      search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
      return 0;
   }

   if (search->rdfp)
      readdb_get_descriptor(search->rdfp, search->subject_id, &sip,
                            &subject_descr);
   else 
      sip = SeqIdSetDup(search->subject_info->sip);
   
   if (sip->choice != SEQID_GENERAL ||
       StringCmp(((DbtagPtr)sip->data.ptrvalue)->db, "BL_ORD_ID")) {
      if (search->pbp->mb_params->full_seqids) {
         subject_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         SeqIdWrite(sip, subject_buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
      } else
         numeric_sip_type = GetAccessionFromSeqId(SeqIdFindBest(sip, SEQID_GI), 
                                                  &subject_gi, &subject_buffer);
   } else {
      DbtagPtr db_tag = (DbtagPtr) sip->data.ptrvalue;
      if (db_tag->db && 
          (!StringCmp(db_tag->db, "THC") || 
           !StringICmp(db_tag->db, "TI")) && 
          db_tag->tag->id != 0) {
         subject_buffer = (CharPtr) Malloc(16);
         sprintf(subject_buffer, "%ld", db_tag->tag->id);
      } else {
         subject_buffer = StringTokMT(subject_descr, " \t", &subject_descr);
         subject_descr = subject_buffer;
      }
   }

   search->current_hitlist->hspcnt_max = search->current_hitlist->hspcnt;

   /* Only for the two sequences case, get offset shift if subject 
      is a subsequence */
   if (!search->rdfp && search->query_slp->next) {
       s_shift = SeqLocStart(search->query_slp->next);
       subject_end = SeqLocStop(search->query_slp->next);
   } else {
      s_shift = 0;
      subject_end = 
         readdb_get_sequence_length(search->rdfp, search->subject_id);
   }
   /* Get offset shift if query is a subsequence */
   q_shift = SeqLocStart(search->query_slp);

   for (hsp_index=0; hsp_index<search->current_hitlist->hspcnt; hsp_index++) {
      hsp = search->current_hitlist->hsp_array[hsp_index];
      if (hsp==NULL || (search->pbp->cutoff_e > 0 && 
	  hsp->evalue > search->pbp->cutoff_e)) 
	 continue;
      
      /* Correct query context is already found in BlastGetNonSumStatsEvalue */
      context = hsp->context; 
      query_id = search->qid_array[context/2];


      if (query_id == NULL) /* Bad hsp, something wrong */
	 continue; 
      hsp->context = context & 1;      
      query_length = search->query_context_offsets[context+1] -
         search->query_context_offsets[context] - 1;
      hsp->subject.end = hsp->subject.offset + hsp->subject.length;

      if (hsp->context) {
	 hsp->query.end = query_length - hsp->query.offset;
	 hsp->query.offset = 
	    hsp->query.end - hsp->query.length + 1;
	 context_sign = '-'; 
      } else {
	 hsp->query.end = (++hsp->query.offset) + hsp->query.length - 1;
         if (hsp->query.end > query_length) {
            hsp->subject.end -= (hsp->query.end - query_length);
            hsp->query.end = query_length;
         }
	 context_sign = '+';  
      }
      
      if (hsp->subject.end > subject_end) {
         hsp->query.end -= (hsp->subject.end - subject_end);
         hsp->subject.end = subject_end;
      }
      hsp->subject.offset++;
      
      query_buffer = NULL;
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
         }  
         BioseqUnlock(query_bsp);
      } else {
         query_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         if (!search->pbp->mb_params->full_seqids)
            SeqIdWrite(query_id, query_buffer, PRINTID_TEXTID_ACCESSION,
                       BUFFER_LENGTH);
         else 
            SeqIdWrite(query_id, query_buffer, PRINTID_FASTA_LONG,
                    BUFFER_LENGTH);
      }

      if (search->pbp->gap_open==0 && search->pbp->gap_extend==0)
	 score = ((hsp->subject.length + hsp->query.length)*
		   search->sbp->reward / 2 - hsp->score) / 
	    (search->sbp->reward - search->sbp->penalty);
      else 
	 score = hsp->score;

      if (context_sign == '+') {
	 q_start = hsp->query.offset;
	 q_end = hsp->query.end;
      } else {
	 q_start = hsp->query.end;
	 q_end = hsp->query.offset;
      }

      /* Adjust offsets if query is a subsequence, only for first query */
      if (context < 2) {
          q_start += q_shift;
          q_end += q_shift;
      }

      hsp->subject.offset += s_shift;
      hsp->subject.end += s_shift;

      if (numeric_sip_type)
	 fprintf(fp, "'%ld'=='%c%s' (%d %d %d %d) %d\n", subject_gi, 
		 context_sign, query_buffer, hsp->subject.offset, q_start, 
		 hsp->subject.end, q_end, score);
      else 
	 fprintf(fp, "'%s'=='%c%s' (%d %d %d %d) %d\n", 
		 subject_buffer, context_sign, query_buffer, 
		 hsp->subject.offset, q_start, 
		 hsp->subject.end, q_end, score);
      MemFree(query_buffer);
   }
   if (!numeric_sip_type && subject_buffer != subject_descr)
      MemFree(subject_buffer);
   MemFree(subject_descr);
   sip = SeqIdSetFree(sip);
   return 0;
}

static int LIBCALLBACK
MegaBlastPrintSegments(VoidPtr ptr)
{
   BlastSearchBlkPtr search = (BlastSearchBlkPtr) ptr;
   ReadDBFILEPtr rdfp = search->rdfp;
   BLAST_HSPPtr hsp; 
   Int4 i, subject_gi;
   Int2 context;
   CharPtr query_buffer, title;
   SeqIdPtr sip, query_id; 
   Int4 hsp_index, score;
   Uint1Ptr query_seq, subject_seq = NULL;
   FloatHi perc_ident;
   Char strand;
   GapXEditScriptPtr esp;
   Int4 q_start, q_end, s_start, s_end, query_length, numseg;
   Int4 q_off, num_ident, align_length, total_ident, q_shift=0, s_shift=0;
   Int4Ptr length, start;
   Uint1Ptr strands;
   CharPtr subject_descr, subject_buffer, buffer;
   Char tmp_buffer[BUFFER_LENGTH];
   Int4 buffer_size, max_buffer_size = LARGE_BUFFER_LENGTH;
   Boolean numeric_sip_type = FALSE;
   FILE *fp = (FILE *) search->output;

   if (search->current_hitlist == NULL || search->current_hitlist->hspcnt <= 0) {
      search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
      return 0;
   }

   subject_seq = search->subject->sequence_start + 1;


   if (rdfp)
      readdb_get_descriptor(rdfp, search->subject_id, &sip, &subject_descr);
   else 
      sip = SeqIdSetDup(search->subject_info->sip);

   if (sip->choice != SEQID_GENERAL ||
       StringCmp(((DbtagPtr)sip->data.ptrvalue)->db, "BL_ORD_ID")) {
      if (search->pbp->mb_params->full_seqids) { 
         subject_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         SeqIdWrite(sip, subject_buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
      } else
         numeric_sip_type = GetAccessionFromSeqId(SeqIdFindBest(sip, SEQID_GI), 
                                                  &subject_gi, &subject_buffer);
   } else {
      subject_buffer = StringTokMT(subject_descr, " \t", &subject_descr);
      subject_descr = subject_buffer;
   }

   buffer = (CharPtr) Malloc(LARGE_BUFFER_LENGTH);

   /* Only for the two sequences case, get offset shift if subject 
      is a subsequence */
   if (!rdfp && search->query_slp->next)
       s_shift = SeqLocStart(search->query_slp->next);
   /* Get offset shift if query is a subsequence */
   q_shift = SeqLocStart(search->query_slp);
   
   for (hsp_index=0; hsp_index<search->current_hitlist->hspcnt; hsp_index++) {
      hsp = search->current_hitlist->hsp_array[hsp_index];
      if (hsp==NULL || (search->pbp->cutoff_e > 0 && 
                        hsp->evalue > search->pbp->cutoff_e)) {
	 continue;
      }
      context = hsp->context;
      query_id = search->qid_array[context/2];

     
      if (query_id == NULL) /* Bad hsp, something wrong */
	 continue; 
      hsp->context = context & 1;

      if (search->pbp->gap_open==0 && search->pbp->gap_extend==0)
	 score = ((hsp->subject.length + hsp->query.length)*
		   search->sbp->reward / 2 - hsp->score) / 
	    (search->sbp->reward - search->sbp->penalty);
      else 
	 score = hsp->score;

      query_length = search->query_context_offsets[context+1] -
         search->query_context_offsets[context] - 1;

      q_off = hsp->query.offset;
      if (hsp->context) {
	 strand = '-'; 
	 hsp->query.end = query_length - hsp->query.offset;
	 hsp->query.offset = 
	    hsp->query.end - hsp->query.length;
      } else {
	 strand = '+';  
	 hsp->query.end = hsp->query.offset + hsp->query.length;
      }

      if (strand == '+') {
	 q_start = hsp->query.offset + 1;
	 q_end = hsp->query.end;
      } else {
	 q_start = hsp->query.end;
	 q_end = hsp->query.offset + 1;
      }
      s_start = hsp->subject.offset + 1;
      s_end = hsp->subject.offset + hsp->subject.length;

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
         }  
         BioseqUnlock(query_bsp);
      } else {
         query_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         if (!search->pbp->mb_params->full_seqids)
            SeqIdWrite(query_id, query_buffer, PRINTID_TEXTID_ACCESSION,
                       BUFFER_LENGTH);
         else 
            SeqIdWrite(query_id, query_buffer, PRINTID_FASTA_LONG,
                    BUFFER_LENGTH);
      }

      if (numeric_sip_type)
	 sprintf(buffer, "\n#'>%ld'=='%c%s' (%d %d %d %d) %d\na {\n  s %d\n  b %d %d\n  e %d %d\n", 
	      subject_gi, strand, query_buffer, 
	      s_start, q_start, s_end, q_end, score, score, 
	      s_start, q_start, s_end, q_end);
      else 
	 sprintf(buffer, "\n#'>%s'=='%c%s' (%d %d %d %d) %d\na {\n  s %d\n  b %d %d\n  e %d %d\n", 
	      subject_buffer, strand, query_buffer, 
	      s_start, q_start, s_end, q_end, score, score, 
	      s_start, q_start, s_end, q_end);
      buffer_size = StringLen(buffer);

      query_seq = search->context[context].query->sequence;

      esp = hsp->gap_info->esp;
        
      for (numseg=0; esp; esp = esp->next, numseg++);

      GXECollectDataForSeqalign(hsp->gap_info, hsp->gap_info->esp, numseg,
				&start, &length, &strands, 
				&q_off, &hsp->subject.offset);

      if (start[0] < 0) {
         length[0] += start[0];
         start[1] -= start[0];
         start[0] = 0;
      } 
      if (start[2*(numseg-1)] + length[numseg-1] > query_length) 
         length[numseg-1] = query_length - start[2*(numseg-1)];
      
      total_ident = 0;
      align_length = 0;
      for (i=0; i<numseg; i++) {
         align_length += length[i];
	 if (strand == '+') {
	    q_start = start[2*i] + 1;
	    q_end = q_start + length[i] - 1;
	 } else {
	    q_start = query_length - start[2*i];
	    q_end = q_start - length[i] + 1;
	 }
	 if (start[2*i] != -1 && start[2*i+1] != -1) {
	    num_ident = MegaBlastGetNumIdentical(query_seq, subject_seq, 
                                                 start[2*i], start[2*i+1], 
                                                 length[i], FALSE);
            perc_ident = (FloatHi) num_ident / length[i] * 100;
            total_ident += num_ident;
	    sprintf(tmp_buffer, "  l %d %d %d %d (%.0f)\n", start[2*i+1]+1, 
		    q_start, start[2*i+1]+length[i],
		    q_end, perc_ident);	 
	    if ((buffer_size += StringLen(tmp_buffer)) > max_buffer_size - 2) {
	       max_buffer_size *= 2;
	       buffer = (CharPtr) Realloc(buffer, max_buffer_size);
	    }
	    StringCat(buffer, tmp_buffer);
	 }
      }
      if (100*total_ident >= 
          align_length*search->pbp->mb_params->perc_identity) {
        StringCat(buffer, "}");
        fprintf(fp, "%s\n", buffer);
      }
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
   return 1;
}

/* Breaks up a location like "2000 3000" into two integers 
   that are returned.

   If location is NULL then the integers are set to 0.
*/

static Boolean
Megablast_GetLoc(char* location, Int4* start, Int4* end)
{
        CharPtr delimiters = " ,;";

        if (start == NULL || end == NULL)
           return FALSE;

        *start = 0;
        *end = 0;

        if (location == NULL)
           return TRUE;

        *start =  atoi(StringTokMT(location, delimiters, &location));
        *end = atoi(location);

        return TRUE;
}

typedef enum {
ARG_DB = 0,
ARG_QUERY,
ARG_EVALUE,
ARG_FORMAT,
ARG_OUT,
ARG_FILTER,
ARG_XDROP,
ARG_SHOWGIS,
ARG_MISMATCH,
ARG_MATCH,
ARG_DESCRIPTIONS,
ARG_ALIGNMENTS,
ARG_OUTTYPE,
ARG_THREADS,
ARG_ASNOUT,
ARG_BELIEVEQUERY,
ARG_MAXQUERY,
ARG_WORDSIZE,
ARG_DBSIZE,
ARG_MAXPOS,
ARG_STRAND,
ARG_HTML,
ARG_GILIST,
ARG_GAPOPEN,
ARG_GAPEXT,
ARG_MINSCORE,
ARG_MASKEDQUERY,
ARG_FULLID,
ARG_LCASE,
ARG_LOGINFO,
ARG_PERC_IDENT,
ARG_QUERYLOC,
ARG_WINDOW,
ARG_XDROP_UNGAPPED,
ARG_XDROP_FINAL,
ARG_TEMPL_LEN,
ARG_EVERYBASE,
ARG_DYNAMIC,
ARG_TEMPL_TYPE,
ARG_MAXHSP,
ARG_FORCE_OLD
} BlastArguments;

#define DO_NOT_SUPPRESS_BLAST_OP

#define NUMARG (sizeof(myargs)/sizeof(myargs[0]))

static Args myargs [] = {
  { "Database", 
    "nr", NULL, NULL, FALSE, 'd', ARG_STRING, 0.0, 0, NULL},       /* ARG_DB */
  { "Query File", 
	NULL, NULL, NULL, FALSE, 'i', ARG_FILE_IN, 0.0, 0, NULL}, /* ARG_QUERY */
  { "Expectation value", 
	"1000000.0", NULL, NULL, FALSE, 'e', ARG_FLOAT, 0.0, 0, NULL},/* ARG_EVALUE */
  { "alignment view options:\n0 = pairwise,\n1 = query-anchored showing identities,\n2 = query-anchored no identities,\n3 = flat query-anchored, show identities,\n4 = flat query-anchored, no identities,\n5 = query-anchored no identities and blunt ends,\n6 = flat query-anchored, no identities and blunt ends,\n7 = XML Blast output,\n8 = tabular, \n9 tabular with comment lines,\n10 ASN, text\n11 ASN, binary", 
        "0", NULL, NULL, FALSE, 'm', ARG_INT, 0.0, 0, NULL},       /* ARG_FORMAT */
  { "BLAST report Output File", 
	"stdout", NULL, NULL, TRUE, 'o', ARG_FILE_OUT, 0.0, 0, NULL},/* ARG_OUT */
  { "Filter query sequence",
        "T", NULL, NULL, FALSE, 'F', ARG_STRING, 0.0, 0, NULL},    /* ARG_FILTER */
  { "X dropoff value for gapped alignment (in bits)",
	"20", NULL, NULL, FALSE, 'X', ARG_INT, 0.0, 0, NULL},      /* ARG_XDROP */
  { "Show GI's in deflines",
        "F", NULL, NULL, FALSE, 'I', ARG_BOOLEAN, 0.0, 0, NULL},   /* ARG_SHOWGIS */
  { "Penalty for a nucleotide mismatch",
	"-3", NULL, NULL, FALSE, 'q', ARG_INT, 0.0, 0, NULL},      /* ARG_MISMATCH */
  { "Reward for a nucleotide match",
	"1", NULL, NULL, FALSE, 'r', ARG_INT, 0.0, 0, NULL},       /* ARG_MATCH */
  { "Number of database sequences to show one-line descriptions for (V)",
        "500", NULL, NULL, FALSE, 'v', ARG_INT, 0.0, 0, NULL},     /* ARG_DESCRIPTIONS */
  { "Number of database sequence to show alignments for (B)",
        "250", NULL, NULL, FALSE, 'b', ARG_INT, 0.0, 0, NULL},     /* ARG_ALIGNMENTS */
  { "Type of output:\n0 - alignment endpoints and score,\n1 - all ungapped segments endpoints,\n2 - traditional BLAST output,\n3 - tab-delimited one line format",
        "0", NULL, NULL, FALSE, 'D', ARG_INT, 0.0, 0, NULL},       /* ARG_OUTTYPE */
  { "Number of processors to use",
        "1", NULL, NULL, FALSE, 'a', ARG_INT, 0.0, 0, NULL},       /* ARG_THREADS */
  { "ASN.1 SeqAlign file; must be used in conjunction with -D2 option", 
	NULL, NULL, NULL, TRUE, 'O', ARG_FILE_OUT, 0.0, 0, NULL},  /* ARG_ASNOUT */
  { "Believe the query defline",
        "F", NULL, NULL, TRUE, 'J', ARG_BOOLEAN, 0.0, 0, NULL},/* ARG_BELIEVEQUERY */
  { "Maximal total length of queries for a single search",
        "20000000", NULL, NULL, FALSE, 'M', ARG_INT, 0.0, 0, NULL},/* ARG_MAXQUERY */
  { "Word size (length of best perfect match)", 
        "28", NULL, NULL, FALSE, 'W', ARG_INT, 0.0, 0, NULL},      /* ARG_WORDSIZE */
  { "Effective length of the database (use zero for the real size)", 
        "0", NULL, NULL, FALSE, 'z', ARG_FLOAT, 0.0, 0, NULL},     /* ARG_DBSIZE */
  { "Maximal number of positions for a hash value (set to 0 to ignore)",
        "0", NULL, NULL, FALSE, 'P', ARG_INT, 0.0, 0, NULL},       /* ARG_MAXPOS */
  { "Query strands to search against database: 3 is both, 1 is top, 2 is bottom",
        "3", NULL, NULL, FALSE, 'S', ARG_INT, 0.0, 0, NULL},       /* ARG_STRAND */
  { "Produce HTML output",
        "F", NULL, NULL, FALSE, 'T', ARG_BOOLEAN, 0.0, 0, NULL},   /* ARG_HTML */
  { "Restrict search of database to list of GI's",
	NULL, NULL, NULL, TRUE, 'l', ARG_STRING, 0.0, 0, NULL},    /* ARG_GILIST */
  { "Cost to open a gap (zero invokes default behavior)",          /* ARG_GAPOPEN */
        "0", NULL, NULL, FALSE, 'G', ARG_INT, 0.0, 0, NULL},
  { "Cost to extend a gap (zero invokes default behavior)",        /* ARG_GAPEXT */
        "0", NULL, NULL, FALSE, 'E', ARG_INT, 0.0, 0, NULL},
  { "Minimal hit score to report (0 for default behavior)",        /* ARG_MINSCORE */
        "0", NULL, NULL, FALSE, 's', ARG_INT, 0.0, 0, NULL},
  { "Masked query output, must be used in conjunction with -D 2 option", 
	NULL, NULL, NULL, TRUE, 'Q', ARG_FILE_OUT, 0.0, 0, NULL},  /* ARG_MASKEDQUERY */
  { "Show full IDs in the output (default - only GIs or accessions)",
        "F", NULL, NULL, FALSE, 'f', ARG_BOOLEAN, 0.0, 0, NULL},   /* ARG_FULLID */
  {"Use lower case filtering of FASTA sequence",                   
        "F", NULL, NULL, TRUE, 'U', ARG_BOOLEAN, 0.0, 0, NULL},    /* ARG_LCASE */
  {"Report the log information at the end of output",
        "F", NULL, NULL, TRUE, 'R', ARG_BOOLEAN, 0.0, 0, NULL},    /* ARG_LOGINFO */
  {"Identity percentage cut-off", 
        "0", NULL, NULL, FALSE, 'p', ARG_FLOAT, 0.0, 0, NULL},     /* ARG_PERC_IDENT */
  { "Location on query sequence",                             
        NULL, NULL, NULL, TRUE, 'L', ARG_STRING, 0.0, 0, NULL},    /* ARG_QUERYLOC */
  { "Multiple Hits window size",  
        "0", NULL, NULL, FALSE, 'A', ARG_INT, 0.0, 0, NULL},      /* ARG_WINDOW  */
  { "X dropoff value for ungapped extension",
	"10", NULL, NULL, FALSE, 'y', ARG_INT, 0.0, 0, NULL},      /* ARG_XDROP_UNGAPPED */
  { "X dropoff value for dynamic programming gapped extension",
	"50", NULL, NULL, FALSE, 'Z', ARG_INT, 0.0, 0, NULL},       /* ARG_XDROP_FINAL */
#ifdef DO_NOT_SUPPRESS_BLAST_OP
  { "Length of a discontiguous word template (contiguous word if 0)",
	"0", NULL, NULL, FALSE, 't', ARG_INT, 0.0, 0, NULL},       /* ARG_TEMPL_LEN */
  {"Generate words for every base of the database (default is every 4th base; may only be used with discontiguous words)",
        "F", NULL, NULL, TRUE, 'g', ARG_BOOLEAN, 0.0, 0, NULL},    /* ARG_EVERYBASE */
  {"Use non-greedy (dynamic programming) extension for affine gap scores",
        "F", NULL, NULL, TRUE, 'n', ARG_BOOLEAN, 0.0, 0, NULL},    /* ARG_DYNAMIC */
  { "Type of a discontiguous word template (0 - coding, 1 - optimal, 2 - two simultaneous",
	"0", NULL, NULL, FALSE, 'N', ARG_INT, 0.0, 0, NULL},       /* ARG_TEMPL_TYPE */
  { "Maximal number of HSPs to save per database sequence (0 = unlimited)",
	"0", NULL, NULL, FALSE, 'H', ARG_INT, 0.0, 0, NULL},        /* ARG_MAXHSP */
#endif
#if MB_ALLOW_NEW
  {"Force use of old engine",
        "T", NULL, NULL, TRUE, 'V', ARG_BOOLEAN, 0.0, 0, NULL}    /* ARG_FORCE_OLD */
#endif

};

#define MAX_NUM_QUERIES 16383 /* == 1/2 INT2_MAX */

static Int2 Main_old (void)
 
{
   AsnIoPtr aip, xml_aip;
   BioseqPtr query_bsp, PNTR query_bsp_array;
   BioSourcePtr source;
   BLAST_MatrixPtr matrix;
   BLAST_OptionsBlkPtr options;
   BLAST_KarlinBlkPtr ka_params=NULL, ka_params_gap=NULL;
   BlastPruneSapStructPtr prune;
   Boolean db_is_na, query_is_na, show_gi, believe_query=FALSE;
   Boolean html=FALSE;
   CharPtr params_buffer=NULL;
   Int4 number_of_descriptions, number_of_alignments;
   SeqAlignPtr  seqalign, PNTR seqalign_array;
   SeqAnnotPtr seqannot;
   SeqEntryPtr PNTR sepp;
   TxDfDbInfoPtr dbinfo=NULL, dbinfo_head;
   Uint1 align_type, align_view;
   Uint4 align_options, print_options;
   ValNodePtr mask_loc, mask_loc_start, next_mask_loc;
   ValNodePtr vnp, other_returns, error_returns;
   
   CharPtr blast_program, blast_database, blast_inputfile, blast_outputfile;
   FILE *infp, *outfp, *mqfp=NULL;
   Int4 index, num_bsps, total_length, total_processed = 0;
   Int2 ctr = 1;
   Char prefix[2];
   SeqLocPtr last_mask, mask_slp;
   Boolean done, hits_found;
   Boolean lcase_masking;
   MBXmlPtr mbxp = NULL;
   Boolean traditional_formatting;
   

	blast_program = "blastn";
        blast_database = myargs [ARG_DB].strvalue;
        blast_inputfile = myargs [ARG_QUERY].strvalue;
        blast_outputfile = myargs [ARG_OUT].strvalue;
	if (myargs[ARG_HTML].intvalue)
		html = TRUE;

	if ((infp = FileOpen(blast_inputfile, "r")) == NULL) {
	   ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open input file %s\n", blast_inputfile);
	   return (1);
	}

	align_view = (Int1) myargs[ARG_FORMAT].intvalue;
	outfp = NULL;

    traditional_formatting = 
        (myargs[ARG_OUTTYPE].intvalue == MBLAST_ALIGNMENTS ||
         myargs[ARG_OUTTYPE].intvalue == MBLAST_DELAYED_TRACEBACK);

	if ((!traditional_formatting ||
            (align_view != 7 && align_view != 10 && align_view != 11)) && 
            blast_outputfile != NULL) {
	   if ((outfp = FileOpen(blast_outputfile, "w")) == NULL) {
	      ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open output file %s\n", blast_outputfile);
	      return (1);
	   }
	}

	align_type = BlastGetTypes(blast_program, &query_is_na, &db_is_na);

    if (!traditional_formatting)
        believe_query = TRUE;
    else
        believe_query = (Boolean) myargs[ARG_BELIEVEQUERY].intvalue;

    /* If ASN.1 output is requested and believe_query is not set to TRUE,
       exit with an error. */
    if (!believe_query && (myargs[ARG_ASNOUT].strvalue ||
                           align_view == 10 || align_view == 11)) {
        ErrPostEx(SEV_FATAL, 1, 0, 
                  "-J option must be TRUE to produce ASN.1 output; before "
                  "changing -J to TRUE please also ensure that all query "
                  "sequence identifiers are unique");
        return -1;
    }
        
	options = BLASTOptionNewEx(blast_program, TRUE, TRUE);
	if (options == NULL)
		return 3;

	options->do_sum_stats = FALSE;
	options->is_neighboring = FALSE;
        options->expect_value  = (Nlm_FloatHi) myargs [ARG_EVALUE].floatvalue;
	number_of_descriptions = myargs[ARG_DESCRIPTIONS].intvalue;	
	number_of_alignments = myargs[ARG_ALIGNMENTS].intvalue;	
	options->hitlist_size = MAX(number_of_descriptions, number_of_alignments);

	if (myargs[ARG_XDROP].intvalue != 0)
           options->gap_x_dropoff = myargs[ARG_XDROP].intvalue;
	if (myargs[ARG_XDROP_UNGAPPED].intvalue != 0)
           options->dropoff_2nd_pass = myargs[ARG_XDROP_UNGAPPED].intvalue;
        if (myargs[ARG_XDROP_FINAL].intvalue != 0)
           options->gap_x_dropoff_final = myargs[ARG_XDROP_FINAL].intvalue;

	if (StringICmp(myargs[ARG_FILTER].strvalue, "T") == 0)
	   options->filter_string = StringSave("D");
	else
	   options->filter_string = StringSave(myargs[ARG_FILTER].strvalue);
	
	show_gi = (Boolean) myargs[ARG_SHOWGIS].intvalue;
	options->penalty = myargs[ARG_MISMATCH].intvalue;
	options->reward = myargs[ARG_MATCH].intvalue;
	options->gap_open = myargs[ARG_GAPOPEN].intvalue;
	options->gap_extend = myargs[ARG_GAPEXT].intvalue;

	if (options->gap_open == 0 && options->reward % 2 == 0 && 
	    options->gap_extend == options->reward / 2 - options->penalty)
	   /* This is the default value */
	   options->gap_extend = 0;

	options->genetic_code = 1;
	options->db_genetic_code = 1; /* Default; it's not needed here anyway */
	options->number_of_cpus = myargs[ARG_THREADS].intvalue;
	if (myargs[ARG_WORDSIZE].intvalue != 0)
           options->wordsize = myargs[ARG_WORDSIZE].intvalue;
        if (myargs[ARG_MINSCORE].intvalue == 0)
           options->cutoff_s2 = options->wordsize*options->reward;
        else 
           options->cutoff_s2 = myargs[ARG_MINSCORE].intvalue;

        options->db_length = (Int8) myargs[ARG_DBSIZE].floatvalue;

	options->perform_culling = FALSE;
	/* Kludge */
        options->block_width  = myargs[ARG_MAXPOS].intvalue;

	options->strand_option = myargs[ARG_STRAND].intvalue;
        options->window_size = myargs[ARG_WINDOW].intvalue;
#ifdef DO_NOT_SUPPRESS_BLAST_OP        
        options->mb_template_length = myargs[ARG_TEMPL_LEN].intvalue;
        options->mb_one_base_step = (Boolean) myargs[ARG_EVERYBASE].intvalue;
        options->mb_disc_type = myargs[ARG_TEMPL_TYPE].intvalue;
#endif
        lcase_masking = (Boolean) myargs[ARG_LCASE].intvalue;
        /* Allow dynamic programming gapped extension only with affine 
           gap scores */
        if (options->gap_open != 0 || options->gap_extend != 0)
           options->mb_use_dyn_prog = (Boolean) myargs[ARG_DYNAMIC].intvalue;

        print_options = 0;
        align_options = 0;
        align_options += TXALIGN_COMPRESS;
        align_options += TXALIGN_END_NUM;
        if (show_gi) {
	   align_options += TXALIGN_SHOW_GI;
	   print_options += TXALIGN_SHOW_GI;
        }
			
        if (align_view) {
	   align_options += TXALIGN_MASTER;
	   if (align_view == 1 || align_view == 3)
	      align_options += TXALIGN_MISMATCH;
	   if (align_view == 3 || align_view == 4 || align_view == 6)
	      align_options += TXALIGN_FLAT_INS;
	   if (align_view == 5 || align_view == 6)
	      align_options += TXALIGN_BLUNT_END;
        } else {
	   align_options += TXALIGN_MATRIX_VAL;
	   align_options += TXALIGN_SHOW_QS;
	}

	if (html) {
	   align_options += TXALIGN_HTML;
	   print_options += TXALIGN_HTML;
	}

	if (myargs[ARG_GILIST].strvalue)
	   options->gifile = StringSave(myargs[ARG_GILIST].strvalue);
   
	if (myargs[ARG_OUTTYPE].intvalue == MBLAST_ENDPOINTS)
      options->no_traceback = 1;
   else if (myargs[ARG_OUTTYPE].intvalue == MBLAST_DELAYED_TRACEBACK)
	   options->no_traceback = 2;
	else
	   options->no_traceback = 0;

	options->megablast_full_deflines = (Boolean) myargs[ARG_FULLID].intvalue;
        options->perc_identity = (FloatLo) myargs[ARG_PERC_IDENT].floatvalue;
        options->hsp_num_max = myargs[ARG_MAXHSP].intvalue;

        if (!believe_query)
           options->megablast_full_deflines = TRUE;
        /*if (options->megablast_full_deflines)
          believe_query = FALSE;*/

	query_bsp_array = (BioseqPtr PNTR) MemNew((MAX_NUM_QUERIES+1)*sizeof(BioseqPtr));
	sepp = (SeqEntryPtr PNTR) MemNew(MAX_NUM_QUERIES*sizeof(SeqEntryPtr));

	StrCpy(prefix, "");

	global_fp = outfp;
        options->output = outfp;

	if (traditional_formatting) {
	   if (align_view < 7) {
              if (html) {
                 fprintf(outfp, "<HTML>\n<TITLE>MEGABLAST Search Results</TITLE>\n");
                 fprintf(outfp, "<BODY BGCOLOR=\"#FFFFFF\" LINK=\"#0000FF\" "
                         "VLINK=\"#660099\" ALINK=\"#660099\">\n");
                 fprintf(outfp, "<PRE>\n");
              }
              init_buff_ex(90);
              BlastPrintVersionInfo("megablast", html, outfp);
              fprintf(outfp, "\n");
              MegaBlastPrintReference(html, 90, outfp);
              fprintf(outfp, "\n");
              
              if(!PrintDbInformation(blast_database, !db_is_na, 70, outfp, html))
                 return 1;
              
              free_buff();
	
#ifdef OS_UNIX
              fprintf(global_fp, "%s", "Searching");
#endif
           }
	}
	
        aip = NULL;
        if (myargs[ARG_ASNOUT].strvalue != NULL) {
           if ((aip = AsnIoOpen (myargs[ARG_ASNOUT].strvalue,"w")) == NULL) {
              ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open output file %s\n", myargs[ARG_ASNOUT].strvalue);
              return 1;
           }
        }
    	else if (align_view == 10 || align_view == 11)
    	{
        	const char* mode = (align_view == 10) ? "w" : "wb";
        	if ((aip = AsnIoOpen (blast_outputfile, (char*) mode)) == NULL) {
                	ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open output file %s\n", blast_outputfile);
                	return 1;
        	}
    	}


        if (align_view == 7) {
           xml_aip = AsnIoOpen(blast_outputfile, "wx");
        }

        if (myargs[ARG_QUERYLOC].strvalue) {       
            Int4 start, end;
            Megablast_GetLoc(myargs[ARG_QUERYLOC].strvalue, &start, &end);
            options->required_start = start - 1;
            options->required_end = end -1;
        }

	done = FALSE;
	while (!done) {
	   num_bsps = 0;
	   total_length = 0;
	   done = TRUE;
	   SeqMgrHoldIndexing(TRUE);
	   mask_slp = last_mask = NULL;
   
	   while ((sepp[num_bsps]=FastaToSeqEntryForDb(infp, query_is_na, NULL,
						       believe_query, prefix, &ctr, 
						       &mask_slp)) != NULL) {
              if (!lcase_masking) /* Lower case ignored */
                 mask_slp = SeqLocFree(mask_slp);
	      if (mask_slp) {
		 if (!last_mask)
		    options->query_lcase_mask = last_mask = mask_slp;
		 else {
		    last_mask->next = mask_slp;
		    last_mask = last_mask->next;
		 }
		 mask_slp = NULL;
	      }
	      query_bsp = NULL;
              SeqEntryExplore(sepp[num_bsps], &query_bsp, FindNuc);

	      if (query_bsp == NULL) {
		 ErrPostEx(SEV_FATAL, 1, 0, "Unable to obtain bioseq\n");
		 return 2;
	      }
	      
	      source = BioSourceNew();
	      source->org = OrgRefNew();
	      source->org->orgname = OrgNameNew();
	      source->org->orgname->gcode = options->genetic_code;
	      ValNodeAddPointer(&(query_bsp->descr), Seq_descr_source, source);
	      
	      query_bsp_array[num_bsps++] = query_bsp;
	      
	      total_length += query_bsp->length;
	      if (total_length > myargs[ARG_MAXQUERY].intvalue || 
		  num_bsps >= MAX_NUM_QUERIES) {
		 done = FALSE;
		 break;
	      }
	   }

           if (num_bsps == 0)
               break;

	   SeqMgrHoldIndexing(FALSE);
	   other_returns = NULL;
	   error_returns = NULL;
	   
	   if (myargs[ARG_OUTTYPE].intvalue==MBLAST_ENDPOINTS) 
	      seqalign_array = BioseqMegaBlastEngine(query_bsp_array, blast_program,
						     blast_database, options,
						     &other_returns, &error_returns,
						     dummy_callback, NULL, NULL, 0, 
						     MegaBlastPrintEndpoints);
	   else if (myargs[ARG_OUTTYPE].intvalue==MBLAST_SEGMENTS) 
	      seqalign_array = BioseqMegaBlastEngine(query_bsp_array, blast_program,
						     blast_database, options,
						     &other_returns, &error_returns,
						     dummy_callback, NULL, NULL, 0,
						     MegaBlastPrintSegments);
	   else if (myargs[ARG_OUTTYPE].intvalue==MBLAST_ALIGN_INFO) {
              PrintTabularOutputHeader(blast_database, 
                                       (num_bsps==1) ? query_bsp_array[0] : NULL,
                                       NULL, "megablast", 0, believe_query,
                                       global_fp);
	      seqalign_array = BioseqMegaBlastEngine(query_bsp_array, blast_program,
						     blast_database, options,
						     &other_returns, &error_returns,
						     dummy_callback, NULL, NULL, 0,
						     MegaBlastPrintAlignInfo);
	   } else /* if (myargs[ARG_OUTTYPE].intvalue==MBLAST_ALIGNMENTS) */
	      seqalign_array = BioseqMegaBlastEngine(query_bsp_array, blast_program,
				  blast_database, options, &other_returns, 
                                  &error_returns, align_view < 7 ? tick_callback : NULL,
                                  NULL, NULL, 0, NULL);
	   
#ifdef OS_UNIX
	   fflush(global_fp);
#endif

           if (error_returns) {
              BlastErrorPrint(error_returns);
              for (vnp = error_returns; vnp; vnp = vnp->next) {
                 BlastDestroyErrorMessage((BlastErrorMsgPtr)vnp->data.ptrvalue);
              }
              ValNodeFree(error_returns);
           }
              
              
	   if (traditional_formatting) {
	      dbinfo = NULL;
	      ka_params = NULL;
	      ka_params_gap = NULL;
	      params_buffer = NULL;
	      mask_loc = NULL;
	      matrix = NULL;
	      for (vnp=other_returns; vnp; vnp = vnp->next) {
		 switch (vnp->choice) {
		 case TXDBINFO:
		    dbinfo = vnp->data.ptrvalue;
		    break;
		 case TXKABLK_NOGAP:
		    ka_params = vnp->data.ptrvalue;
		    break;
		 case TXKABLK_GAP:
		    ka_params_gap = vnp->data.ptrvalue;
		    break;
		 case TXPARAMETERS:
		    params_buffer = vnp->data.ptrvalue;
		    break;
		 case TXMATRIX:
		    matrix = vnp->data.ptrvalue;
		    break;
		 case SEQLOC_MASKING_NOTSET:
		 case SEQLOC_MASKING_PLUS1:
		 case SEQLOC_MASKING_PLUS2:
		 case SEQLOC_MASKING_PLUS3:
		 case SEQLOC_MASKING_MINUS1:
		 case SEQLOC_MASKING_MINUS2:
		 case SEQLOC_MASKING_MINUS3:
		    ValNodeAddPointer(&mask_loc, vnp->choice, vnp->data.ptrvalue);
		    break;
		 default:
		    break;
		 }
	      }	
	      
#ifdef OS_UNIX
              if(align_view < 7) {
                 fprintf(global_fp, "%s\n", " done");
              }
#endif
	      
              if (myargs[ARG_MASKEDQUERY].strvalue) {
                 if ((mqfp = FileOpen(myargs[ARG_MASKEDQUERY].strvalue, "w")) == NULL)
                    ErrPostEx(SEV_WARNING, 1, 0, "Unable to open file %s for masked query\n",
                              myargs[ARG_MASKEDQUERY].strvalue);
              }

              hits_found = FALSE;

              mask_loc_start = next_mask_loc = mask_loc;
              mask_loc = NULL;

              if (align_view == 7) {
                 mbxp = PSIXmlInit(xml_aip, "megablast", blast_database, 
                                   options, query_bsp_array[0], 0);
              }

              if (seqalign_array) {
	         ReadDBBioseqFetchEnable ("megablast", blast_database, db_is_na, TRUE);
                 for (index=0; index<num_bsps; index++) {
                    seqalign = seqalign_array[index];
                    if (next_mask_loc && 
                        SeqIdComp(SeqLocId((SeqLocPtr)next_mask_loc->data.ptrvalue), 
                                  query_bsp_array[index]->id) == SIC_YES) {
                       mask_loc = (SeqLocPtr) 
                          MemDup(next_mask_loc, sizeof(SeqLoc));
                       next_mask_loc = next_mask_loc->next;
                       mask_loc->next = NULL;
                    }
                    if (mqfp) {
                       mask_slp = MaskSeqLocFromSeqAlign(seqalign);
                       if (mask_loc) 
                          mask_slp = blastMergeFilterLocs(mask_slp, 
                              (SeqLocPtr)mask_loc->data.ptrvalue,
                              FALSE, 0, 0);
                       PrintMaskedSequence(query_bsp_array[index], mask_slp,
                                           mqfp, 50, lcase_masking);
                       SeqLocSetFree(mask_slp);
                    }
                    if (seqalign==NULL) {
                       mask_loc = MemFree(mask_loc);
                       continue;
                    }
                    hits_found = TRUE;
                    if (align_view < 7) {
                       init_buff_ex(70);
                       AcknowledgeBlastQuery(query_bsp_array[index], 70, outfp, 
                                             believe_query, html);
                       free_buff();
                    }
                    if (align_view == 8 || align_view == 9) {
                       if (align_view == 9)
                          PrintTabularOutputHeader(blast_database, 
                             query_bsp_array[index], NULL, blast_program, 0,
                             believe_query, global_fp);

                       BlastPrintTabulatedResults(seqalign, 
                          query_bsp_array[index], NULL, number_of_alignments,
                          blast_program, !options->gapped_calculation, 
                          believe_query, 0, 0, 
                          global_fp, (align_view == 9));

                       ObjMgrFreeCache(0);

                       SeqAlignSetFree(seqalign);
                       mask_loc = MemFree(mask_loc);
                       continue;
                    } else if(align_view == 7) {
                       IterationPtr iterp;

                       iterp = BXMLBuildOneQueryIteration(seqalign, 
                                  NULL, FALSE, 
                                  !options->gapped_calculation, index, 
                                  NULL, query_bsp_array[index], mask_loc);
                       IterationAsnWrite(iterp, mbxp->aip, mbxp->atp);
                       AsnIoFlush(mbxp->aip);
                       IterationFree(iterp);
                       SeqAlignSetFree(seqalign);
                       mask_loc = MemFree(mask_loc);
                       continue;
                    }
                    seqannot = SeqAnnotNew();
                    seqannot->type = 2;
                    AddAlignInfoToSeqAnnot(seqannot, align_type);
                    seqannot->data = seqalign;
                    if (aip) {
                       SeqAnnotAsnWrite((SeqAnnotPtr) seqannot, aip, NULL);
                       AsnIoReset(aip);
                    }
                    if (outfp) { /* Uncacheing causes problems with ordinal nos. vs. gi's. */
                       prune = BlastPruneHitsFromSeqAlign(seqalign, number_of_descriptions, NULL);
                       ObjMgrSetHold();
                       init_buff_ex(85);
                       PrintDefLinesFromSeqAlign(prune->sap, 80,
                                                 outfp, print_options, FIRST_PASS, NULL);
                       free_buff();
                       
                       prune = BlastPruneHitsFromSeqAlign(seqalign, number_of_alignments, prune);
                       seqannot->data = prune->sap;
                       if (align_view != 0)
                          ShowTextAlignFromAnnot(seqannot, 60, outfp, NULL,
                                                 NULL, align_options, NULL, 
                                                 mask_loc, NULL);
                       else
                          ShowTextAlignFromAnnot(seqannot, 60, outfp, NULL, NULL, align_options, NULL, mask_loc, FormatScoreFunc);
                       seqannot->data = seqalign;
                       prune = BlastPruneSapStructDestruct(prune);
                       ObjMgrClearHold();
                       ObjMgrFreeCache(0);
                    }
                    seqannot = SeqAnnotFree(seqannot);
                    mask_loc = MemFree(mask_loc);
                 } /* End loop on seqaligns for different queries */
                 ReadDBBioseqFetchDisable();
              } 

              if (mbxp != NULL) {
                 MBXmlClose(mbxp, other_returns, !options->gapped_calculation);
              }

              if (mqfp)
                 FileClose(mqfp);

              if (!hits_found && align_view < 7)
                 fprintf(outfp, "\n\n ***** No hits found ******\n\n");

              matrix = BLAST_MatrixDestruct(matrix);
	      
              if(html) 
                 fprintf(outfp, "<PRE>\n");
              init_buff_ex(85);
              dbinfo_head = dbinfo;
              if(align_view < 7) {
                 while (dbinfo) {
                    PrintDbReport(dbinfo, 70, outfp);
                    dbinfo = dbinfo->next;
                 }
              }
              dbinfo_head = TxDfDbInfoDestruct(dbinfo_head);
              
              if (ka_params) {
                 if(align_view < 7)
                    PrintKAParameters(ka_params->Lambda, ka_params->K, ka_params->H, 70, outfp, FALSE);
                 MemFree(ka_params);
              }
              if (ka_params_gap) {
                 if(align_view < 7)
                    PrintKAParameters(ka_params_gap->Lambda, ka_params_gap->K, ka_params_gap->H, 70, outfp, TRUE);
                 MemFree(ka_params_gap);
              }
              if(align_view < 7)
                 PrintTildeSepLines(params_buffer, 70, outfp);
              MemFree(params_buffer);
              free_buff();
              mask_loc = mask_loc_start;
              while (mask_loc) {
                 SeqLocSetFree(mask_loc->data.ptrvalue);
                 mask_loc = mask_loc->next;
              }
              ValNodeFree(mask_loc_start);
	   } else {
	      /* Just destruct all other_returns parts */
	      for (vnp=other_returns; vnp; vnp = vnp->next) {
		 switch (vnp->choice) {
		 case TXDBINFO:
		    TxDfDbInfoDestruct(vnp->data.ptrvalue);
		    break;
		 case TXKABLK_NOGAP:
		 case TXKABLK_GAP:
		 case TXPARAMETERS:
		    MemFree(vnp->data.ptrvalue);
		    break;
		 case TXMATRIX:
		    BLAST_MatrixDestruct(vnp->data.ptrvalue);
		    break;
		 case SEQLOC_MASKING_NOTSET:
		 case SEQLOC_MASKING_PLUS1:
		 case SEQLOC_MASKING_PLUS2:
		 case SEQLOC_MASKING_PLUS3:
		 case SEQLOC_MASKING_MINUS1:
		 case SEQLOC_MASKING_MINUS2:
		 case SEQLOC_MASKING_MINUS3:
                    mask_loc = vnp->data.ptrvalue;
                    SeqLocSetFree(mask_loc);
		 default:
		    break;
		 }
	      }
	   }
	   other_returns = ValNodeFree(other_returns);
	   MemFree(seqalign_array);
           options->query_lcase_mask = 
              SeqLocSetFree(options->query_lcase_mask);

	   /* Freeing SeqEntries can be very expensive, do this only if 
	      this is not the last iteration of search */
	   if (!done) { 
	      for (index=0; index<num_bsps; index++) {
		 sepp[index] = SeqEntryFree(sepp[index]);
		 query_bsp_array[index] = NULL;
	      }	   
           }
           total_processed += num_bsps;
	} /* End of loop on complete searches */
        
        aip = AsnIoClose(aip);

        /*if (align_view == 7)
          xml_aip = AsnIoClose(xml_aip);*/

        if (align_view < 7 && html) 
           fprintf(outfp, "</PRE>\n</BODY>\n</HTML>\n");
        if (align_view < 7 && myargs[ARG_LOGINFO].intvalue)
           fprintf(outfp, "Mega BLAST run finished, processed %d queries\n",
                   total_processed);
	MemFree(query_bsp_array);
	MemFree(sepp);
	options = BLASTOptionDelete(options);
	FileClose(infp);
        FileClose(outfp);
	
	return 0;
}

static double
GetLambdaFast(const BlastScoringOptions* scoring_options)
{
    double lambda;
    
    BlastScoreBlk* sbp = BlastScoreBlkNew(BLASTNA_SEQ_CODE, 1);
    Blast_ScoreBlkMatrixInit(eBlastTypeBlastn, scoring_options, sbp);
    if (Blast_ScoreBlkKbpIdealCalc(sbp))
        return 0.0; 
    lambda = sbp->kbp_ideal->Lambda;
    sbp = BlastScoreBlkFree(sbp);
    return lambda;
}

#if MB_ALLOW_NEW
/** Fills all the options structures with user defined values. Uses the 
 * myargs global structure obtained from GetArgs.
 * @param lookup_options Lookup table options [in]
 * @param query_setup_options Query options [in]
 * @param word_options Initial word processing options [in]
 * @param ext_options Extension options [in]
 * @param hit_options Hit saving options [out]
 * @param score_options Scoring options [out]
 * @param eff_len_options Effective length options [out]
 */
static Int2 
BLAST_FillOptions(SBlastOptions* options)
{
   LookupTableOptions* lookup_options = options->lookup_options;
   QuerySetUpOptions* query_setup_options = options->query_options; 
   BlastInitialWordOptions* word_options = options->word_options;
   BlastExtensionOptions* ext_options = options->ext_options;
   BlastHitSavingOptions* hit_options = options->hit_options ;
   BlastScoringOptions* score_options = options->score_options;
   BlastEffectiveLengthsOptions* eff_len_options = options->eff_len_options;
   Boolean ag_blast = FALSE, variable_wordsize = FALSE, mb_lookup = TRUE;
   Boolean greedy=TRUE; /* greedy alignment should be done. */
   double lambda=0;
   Int2 status;
   const Uint1 program_number = eBlastTypeBlastn; 

   if (myargs[ARG_DYNAMIC].intvalue != 0) 
       greedy = FALSE; 

   BLAST_FillScoringOptions(score_options, program_number, 
                greedy, myargs[ARG_MISMATCH].intvalue, myargs[ARG_MATCH].intvalue,
                NULL, myargs[ARG_GAPOPEN].intvalue, myargs[ARG_GAPEXT].intvalue);

   /* Use to "unscale" x-dropoff since they were never properly scaled in the old code but are in the new
      code, so we wish to keep megablast running like the old code without adding this "bug" back to the main 
      engine. The returned Lambda is also increased by 2% to make sure the Xdrop value we use is at least as
      high as the old one. */
   lambda = 1.02*GetLambdaFast(score_options); 

   /* The following options are for blastn only */
   if (myargs[ARG_TEMPL_LEN].intvalue == 0) {
         ag_blast = TRUE;
         /* Variable word size can only be used for word sizes divisible 
            by 4 */
         if (myargs[ARG_WORDSIZE].intvalue % COMPRESSION_RATIO == 0)
            variable_wordsize = TRUE;
   } else {
         /* Discontiguous words */
         variable_wordsize = FALSE;
   }

   BLAST_FillLookupTableOptions(lookup_options, program_number, mb_lookup,
      0, myargs[ARG_WORDSIZE].intvalue, variable_wordsize, FALSE);
   /* Fill the rest of the lookup table options */
   lookup_options->mb_template_length = 
      (Uint1) myargs[ARG_TEMPL_LEN].intvalue;
   lookup_options->mb_template_type = 
      (Uint1) myargs[ARG_TEMPL_TYPE].intvalue;

   if (myargs[ARG_EVERYBASE].intvalue)
      lookup_options->full_byte_scan = FALSE;
   
   BLAST_FillQuerySetUpOptions(query_setup_options, program_number, 
      myargs[ARG_FILTER].strvalue, myargs[ARG_STRAND].intvalue);

   BLAST_FillInitialWordOptions(word_options, program_number, 
      greedy, myargs[ARG_WINDOW].intvalue,
      lambda*myargs[ARG_XDROP_UNGAPPED].intvalue/NCBIMATH_LN2);

   BLAST_FillExtensionOptions(ext_options, program_number, (greedy ? 1 : 0), 
      lambda*myargs[ARG_XDROP].intvalue/NCBIMATH_LN2, 
      lambda*myargs[ARG_XDROP_FINAL].intvalue/NCBIMATH_LN2);

   score_options->gapped_calculation = TRUE;

   BLAST_FillHitSavingOptions(hit_options, 
      myargs[ARG_EVALUE].floatvalue, 
      MAX(myargs[ARG_DESCRIPTIONS].intvalue, 
          myargs[ARG_ALIGNMENTS].intvalue),
      score_options->gapped_calculation,
      0);
 
   
   word_options->ungapped_extension = TRUE;
   /* For discontiguous megablast we need to use a less drastic method of elimination redundant hits (at least to
      be compatiable with old code).  For two hits we do not perform an ungapped extension either. */
   if (lookup_options->mb_template_length > 0)
   {
      hit_options->min_diag_separation = 6;
      if (word_options->window_size > 0)
          word_options->ungapped_extension = FALSE;
   }

   hit_options->percent_identity = myargs[ARG_PERC_IDENT].floatvalue;

/* FIXME ??
   if (myargs[ARG_SEARCHSP].floatvalue != 0) {
      eff_len_options->searchsp_eff = (Int8) myargs[ARG_SEARCHSP].floatvalue; 
   }
*/

   return 0;
}

static Int2 Main_new(void)
{
   const Boolean query_is_na = TRUE; 
   const Boolean db_is_na = TRUE;
   Boolean believe_query = FALSE;
   char* program_name = "blastn";
   Int2 status = 0;
   Int4 start=0, end=0;   /* start and end of sequence to be searched as specified by ARG_QUERYLOC */
   SeqLoc* lcase_mask = NULL;
   SeqLoc* filter_loc=NULL;	/* All masking locations */
   SeqLoc* query_slp = NULL;
   FILE *infp=NULL, *outfp=NULL;
   SBlastOptions* options = NULL;
   BlastFormattingOptions* format_options;
   Int2 ctr = 1;
   Int4 num_queries_total=0;  /* total number of queries read. */
   Boolean tabular_output = FALSE;
   BlastTabularFormatData* tf_data = NULL;
   int num_threads;
   char* dbname = myargs[ARG_DB].strvalue;
   Blast_SummaryReturn* sum_returns = NULL;
   Int4 align_view;

   if (myargs[ARG_OUTTYPE].intvalue == 3)
       tabular_output = TRUE;

   sum_returns = Blast_SummaryReturnNew();
   status = SBlastOptionsNew(program_name, &options, sum_returns);

   if (status) {
       if (sum_returns->error) {
           Blast_SummaryReturnsPostError(sum_returns);
           sum_returns = Blast_SummaryReturnFree(sum_returns);
       }
       return -1;
   }

   BLAST_FillOptions(options);

   align_view = myargs[ARG_FORMAT].intvalue;

   /* If tabular output is requested, set believe_query to TRUE,
      to guarantee meaningful sequence ids in the output. */
   if (tabular_output)
       believe_query = TRUE;
   else
       believe_query = (Boolean) myargs[ARG_BELIEVEQUERY].intvalue;

   /* If ASN.1 output is requested and believe_query is not set to TRUE,
      exit with an error. */
   if (!believe_query && (myargs[ARG_ASNOUT].strvalue ||
                          align_view == 10 || align_view == 11)) {
       ErrPostEx(SEV_FATAL, 1, 0,
                 "-J option must be TRUE to produce ASN.1 output; before "
                 "changing -J to TRUE please also ensure that all query "
                 "sequence identifiers are unique");
       return -1;
   }

   if (!tabular_output) {
      if ((status = BlastFormattingOptionsNew(options->program, 
                       myargs[ARG_OUT].strvalue, 
                       myargs[ARG_DESCRIPTIONS].intvalue, 
                       myargs[ARG_ALIGNMENTS].intvalue, 
                       align_view, &format_options)) != 0)
         return status;
      format_options->html = (Boolean) myargs[ARG_HTML].intvalue;
      if (myargs[ARG_SHOWGIS].intvalue == 0)
      { /* These are default in new api, so we turn off if not requested. */
           format_options->align_options -= TXALIGN_SHOW_GI;
           format_options->print_options -= TXALIGN_SHOW_GI;
      }

      if (dbname) {
         BLAST_PrintOutputHeader(format_options, TRUE, program_name, dbname, 
                                 !db_is_na);
      }
   }
   else
   { /* tabular output requires raw FILE*. */
       if ((outfp = FileOpen(myargs[ARG_OUT].strvalue, "w")) == NULL) {
            ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open output file %s\n", 
                myargs[ARG_OUT].strvalue);
            return (1);
       }
       believe_query = TRUE;
   }


   if ((infp = FileOpen(myargs[ARG_QUERY].strvalue, "r")) == NULL) {
      ErrPostEx(SEV_FATAL, 1, 0, "blast: Unable to open input file %s\n", 
                myargs[ARG_QUERY].strvalue);
      return (1);
   }


   Megablast_GetLoc(myargs[ARG_QUERYLOC].strvalue, &start, &end);

   num_threads = myargs[ARG_THREADS].intvalue;

   /* Get the query (queries), loop if necessary. */
   while (1) {
       SeqAlign* seqalign = NULL;
       SeqLoc* filter_loc=NULL;	/* All masking locations */
       Boolean mask_at_hash;
       SeqLoc* repeat_mask = NULL; /* Repeat mask locations */
       Int4 num_queries; /* Number of queries read this time. */
       Int4  letters_read;  /* number of letters (bases/residues) read. */

      if ((Boolean)myargs[ARG_LCASE].intvalue) {
         letters_read = BLAST_GetQuerySeqLoc(infp, query_is_na, 
                   myargs[ARG_STRAND].intvalue, myargs[ARG_MAXQUERY].intvalue, start, end,
                   &lcase_mask, &query_slp, &ctr, &num_queries, believe_query);
      } else {
         letters_read = BLAST_GetQuerySeqLoc(infp, query_is_na,
                   myargs[ARG_STRAND].intvalue, myargs[ARG_MAXQUERY].intvalue, start, end, 
                   NULL, &query_slp, &ctr, &num_queries, believe_query);
      }

      if (letters_read == 0)
          break;

      if (letters_read < 0)
      {
	   ErrPostEx(SEV_FATAL, 1, 0, "BLAST_GetQuerySeqLoc returned an error\n");
           return -1;
      }

      num_queries_total += num_queries;

      if (tabular_output) {
          EBlastTabularFormatOptions tab_option = eBlastTabularDefault;
          if (getenv("PRINT_SEQUENCES") != NULL)
              tab_option = eBlastTabularAddSequences;
          
          /* Print the header of tabular output. */
          PrintTabularOutputHeader(myargs[ARG_DB].strvalue, NULL, query_slp, 
                                   program_name, 0, believe_query, outfp);
          tf_data = BlastTabularFormatDataNew(outfp, query_slp, tab_option);
          tf_data->show_gi = (Boolean) myargs[ARG_SHOWGIS].intvalue;
          tf_data->show_accession = !((Boolean) myargs[ARG_FULLID].intvalue);
      }

      /* Find repeat mask, if necessary */
      Blast_FindRepeatFilterSeqLoc(query_slp, myargs[ARG_FILTER].strvalue, 
                                &repeat_mask);
      /* Combine repeat mask with lower case mask */
      if (repeat_mask)
          lcase_mask = ValNodeLink(&lcase_mask, repeat_mask);
      
       /* The main search is here. */
      status = 
          Blast_DatabaseSearch(query_slp, dbname, lcase_mask, options, tf_data,
                               &seqalign, &filter_loc, &mask_at_hash, 
                               sum_returns);

      /* Deallocate the data structure used for tabular formatting. */
      BlastTabularFormatDataFree(tf_data);
      
      /* Free the lower case mask in SeqLoc form. */
      lcase_mask = Blast_ValNodeMaskListFree(lcase_mask);
      
      /* If masking was done for lookup table only, free the masking locations,
         because they will not be used for formatting. */
      if (mask_at_hash)
          filter_loc = Blast_ValNodeMaskListFree(filter_loc);

      /* Post warning or error messages, no matter what the search status was. */
      Blast_SummaryReturnsPostError(sum_returns);

      if (!status && !tabular_output) {
          if (myargs[ARG_ASNOUT].strvalue) {
              AsnIoPtr asnout = AsnIoOpen(myargs[ARG_ASNOUT].strvalue, (char*)"w");
              GenericSeqAlignSetAsnWrite(seqalign, asnout);
              asnout = AsnIoClose(asnout);
          }
 
          /* Format the results; note that seqalign and filter locations 
             are freed inside. */
          status = 
              BLAST_FormatResults(seqalign, myargs[ARG_DB].strvalue, 
                                  program_name, num_queries, query_slp, 
                                  filter_loc, format_options, FALSE, NULL, 
                                  sum_returns);
          
          seqalign = SeqAlignSetFree(seqalign);

          Blast_PrintOutputFooter(options->program, format_options, dbname, 
                                  sum_returns);
      }
      Blast_SummaryReturnClean(sum_returns);
      filter_loc = Blast_ValNodeMaskListFree(filter_loc);
      query_slp = SeqLocSetFree(query_slp);
   } /* End loop on sets of queries */
   
   options = SBlastOptionsFree(options);
   sum_returns = Blast_SummaryReturnFree(sum_returns);

   if (!tabular_output)
   {
      if (align_view < 7 && myargs[ARG_LOGINFO].intvalue)
            BlastPrintLogReport(format_options, num_queries_total);
      format_options = BlastFormattingOptionsFree(format_options);
   }
   else
   {
      if (myargs[ARG_LOGINFO].intvalue)
           fprintf(outfp, "Mega BLAST run finished, processed %ld queries\n", 
                   (long) num_queries_total);
      FileClose(outfp);
   }

   if (infp)
      FileClose(infp);
   
   return status;
}
#endif

/*
	This function decides whether the new blast code can handle this database or not.
	Currently it should return FALSE for any database that uses a gilist.
        This implementation only works for nucleotide databases.

	If it is not possible to initialize the database or some error condition exists then FALSE
	will also be returned and the old engine should deal with this.
*/
static Boolean
readdb_use_new_blast(char* dbname)
{
      Boolean retval=TRUE;
      ReadDBFILEPtr rdfp=NULL;
      ReadDBFILEPtr rdfp_var=NULL;

      if (!dbname)
           return FALSE;

      rdfp = readdb_new(dbname, FALSE);
      if (!rdfp)
           return FALSE;

      rdfp_var = rdfp;
      while (rdfp_var)
      {
            if (rdfp_var->gilist != NULL)
            {
                   retval = FALSE;
                   break;  /* Break out and free rdfp. */
            }
            rdfp_var = rdfp_var->next;
      }
      rdfp = readdb_destruct(rdfp);
      return retval;
}


Int2 Nlm_Main(void)
{
    Boolean use_new_engine=FALSE;
    char buf[256] = { '\0' };

    StringCpy(buf, "megablast ");
    StringNCat(buf, BlastGetVersionNumber(), sizeof(buf)-StringLen(buf)-1);
    if (! GetArgs (buf, NUMARG, myargs))
	   return (1);

    UseLocalAsnloadDataAndErrMsg ();

    if (! SeqEntryLoad())
		return 1;

    ErrSetMessageLevel(SEV_WARNING);

    if (myargs[ARG_FORCE_OLD].intvalue == 0 &&
                  myargs[ARG_OUTTYPE].intvalue > 1 &&
                      myargs[ARG_GILIST].strvalue == NULL)
          use_new_engine = readdb_use_new_blast(myargs[ARG_DB].strvalue);

   if ((myargs[ARG_EVERYBASE].intvalue == 1) && (myargs[ARG_TEMPL_LEN].intvalue == 0))
      ErrPostEx(SEV_FATAL, 1, 0, "Single base scanning may only be used with discontiguous words."); 

    if (use_new_engine)
    	return Main_new();
    else
    	return Main_old();
}
