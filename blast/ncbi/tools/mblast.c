static char const rcsid[] = "$Id: mblast.c,v 6.213 2005/03/07 16:30:57 dondosha Exp $";

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

File name: mblast.c

Author: Ilya Dondoshansky

Contents: Mega BLAST functions

Detailed Contents:

        - Mega BLAST versions of some functions from blast.c returning array
	of SeqAlignPtrs

	- Functions specific to Mega BLAST

******************************************************************************
 * $Revision: 6.213 $
 *
 * $Log: mblast.c,v $
 * Revision 6.213  2005/03/07 16:30:57  dondosha
 * In reevaluation with ambiguities, if HSP start changed, update starting offsets in edit block
 *
 * Revision 6.212  2004/12/07 16:09:04  coulouri
 * check malloc returncode
 *
 * Revision 6.211  2004/11/25 01:59:38  coulouri
 * reinitialize full_query_length to prevent overflow of query_seq_combined when only minus strands are searched
 *
 * Revision 6.210  2004/11/22 20:54:15  coulouri
 * initialize pointer variables
 *
 * Revision 6.209  2004/11/22 15:17:34  dondosha
 * Do not calculate length adjustment for empty contexts, i.e. ones with null Karlin blocks
 *
 * Revision 6.208  2004/11/19 13:22:05  madden
 * Remove no_check_score completely (from Mike Gertz)
 *
 * Revision 6.207  2004/11/05 15:34:53  coulouri
 * bring in system includes after toolkit includes so that LONG_BIT is defined correctly on amd64
 *
 * Revision 6.206  2004/10/29 21:34:45  dondosha
 * Calculate length adjustment for each context separately; do not allow effective query length to be less than 1
 *
 * Revision 6.205  2004/09/13 18:33:01  dondosha
 * Advance mask pointer in the list when one of the queries in a set is discarded due to being too short, and also has a lower case mask
 *
 * Revision 6.204  2004/05/27 17:35:56  dondosha
 * Do not flag HSPs for deletion in sorting before doing inclusion tests
 *
 * Revision 6.203  2004/05/21 13:53:04  dondosha
 * Use BLAST_HSPFree to free BLAST_HSP structures, hence no need to call GapXEditBlockDelete in multiple places
 *
 * Revision 6.202  2004/03/31 17:58:51  papadopo
 * Mike Gertz' changes for length adjustment calculations
 *
 * Revision 6.201  2004/02/26 15:52:30  papadopo
 * Mike Gertz' modifications to unify handling of gapped Karlin blocks between protein and nucleotide searches
 *
 * Revision 6.200  2004/02/24 14:07:01  camacho
 * Use approximate sequence length calculation for entrez-limited
 * nucleotide blast databases.
 *
 * Revision 6.199  2004/01/27 20:46:06  dondosha
 * Allow values 0, 1, 2 for no_traceback megablast option
 *
 * Revision 6.198  2004/01/23 22:29:11  dondosha
 * Increase hitlist size for preliminary alignment if traceback is delayed
 *
 * Revision 6.197  2004/01/16 23:43:45  dondosha
 * No more need for special argument for partial search: it is set in options
 *
 * Revision 6.196  2004/01/13 15:46:54  dondosha
 * Changed cast to nearest integer in length adjustment calculation
 *
 * Revision 6.195  2004/01/06 22:37:10  dondosha
 * Use BLAST_HSPfree function
 *
 * Revision 6.194  2003/12/10 17:05:28  dondosha
 * Added function ReevaluateScoreWithAmbiguities to reevaluate score for one HSP; use it after greedy traceback
 *
 * Revision 6.193  2003/11/06 19:53:07  dondosha
 * Fix in MegaBlastSetupSearchInternalByLoc when query submitted as Bioseq instead of SeqLoc; removed unused variables
 *
 * Revision 6.192  2003/11/05 21:11:09  dondosha
 * Fix for traceback with greedy algorithm
 *
 * Revision 6.191  2003/10/29 17:46:59  dondosha
 * Allow 2-stage greedy extension in megablast
 *
 * Revision 6.190  2003/05/30 17:25:36  coulouri
 * add rcsid
 *
 * Revision 6.189  2003/05/20 21:56:34  dondosha
 * Use correct X-dropoff for preliminary dynamic programming gapped extension
 *
 * Revision 6.188  2003/05/14 20:35:58  camacho
 * Allow searching empty databases
 *
 * Revision 6.187  2003/05/13 16:02:53  coulouri
 * make ErrPostEx(SEV_FATAL, ...) exit with nonzero status
 *
 * Revision 6.186  2003/05/12 12:23:44  camacho
 * Sanity check for number of sequences & db length
 *
 * Revision 6.185  2003/05/02 23:02:11  dondosha
 * Fix for dynamic programming gapped extension
 *
 * Revision 6.184  2003/04/24 14:58:20  dondosha
 * Fixed a minor bug in MegaBlastExtendHit found by A. Morgulis
 *
 * Revision 6.183  2003/04/04 17:16:31  dondosha
 * Previous change reversed, was incorrect
 *
 * Revision 6.182  2003/04/03 22:35:44  dondosha
 * When saving hit list, save hsp contexts modulo 2, since multiple query information is no longer needed
 *
 * Revision 6.181  2003/03/19 19:16:20  dondosha
 * Small correction for a rarely happening case in MegaBlastExtendHit
 *
 * Revision 6.180  2003/03/13 19:03:10  dondosha
 * Minor bug fix in MegaBlastExtendHit
 *
 * Revision 6.179  2003/03/13 16:53:03  dondosha
 * Minor bug fixes
 *
 * Revision 6.178  2002/12/26 20:27:40  dondosha
 * Bug fix for e-values of hits whose extent changes when reevaluating with ambiguities
 *
 * Revision 6.177  2002/11/25 19:57:29  dondosha
 * Further fix to the HSP limit (-H) megablast option
 *
 * Revision 6.176  2002/11/22 23:30:34  dondosha
 * 1. Use array of structures instead of array of pointers for initial offset pairs;
 * 2. Scan subject sequence to the last byte (it wasn't!) when looking for words
 *
 * Revision 6.175  2002/11/06 22:26:49  dondosha
 * Calculate number of identities at the same time when doing reevaluation for ambiguities
 *
 * Revision 6.174  2002/11/04 22:57:24  dondosha
 * Save number of identities in result HSPs
 *
 * Revision 6.173  2002/10/10 21:02:20  dondosha
 * Fixed reevaluation with ambiguities: if score gets negative, check whether best score already qualifies for saving
 *
 * Revision 6.172  2002/09/25 18:35:33  dondosha
 * Fix in MegaBlastExtendHit for contiguous word size 12
 *
 * Revision 6.171  2002/08/30 17:53:51  thiessen
 * fix missing LIBCALL in prototype
 *
 * Revision 6.170  2002/08/30 15:54:11  dondosha
 * 1. Prototypes of internally used functions moved from mblast.h;
 * 2. Do not allocate ewp_params and ewp structures when stacks are used to save
 *    initial hits;
 * 3. Cleaned up initial word extension code.
 *
 * Revision 6.169  2002/08/22 13:39:45  camacho
 * Close the header and sequence files only if allocated
 *
 * Revision 6.168  2002/08/20 22:06:02  madden
 * Restore partial results code for megablast
 *
 * Revision 6.167  2002/08/06 15:56:50  dondosha
 * Removed some old unused code
 *
 * Revision 6.166  2002/08/06 15:45:21  dondosha
 * Corrected the sanity check in MegaBlastGapInfoToSeqAlign
 *
 * Revision 6.165  2002/08/01 20:48:25  dondosha
 * Use BLASTPostSearchLogic function: compute SeqAligns after all results are found
 *
 * Revision 6.164  2002/07/31 17:51:09  dondosha
 * Corrected the two-hit logic in case of word size greater than 12
 *
 * Revision 6.163  2002/07/18 19:40:45  dondosha
 * Added an option to restrict number of HSPs per database sequence
 *
 * Revision 6.162  2002/07/17 22:12:58  dondosha
 * Allow return without computing seqalign for partial search
 *
 * Revision 6.161  2002/06/26 00:56:30  camacho
 *
 * 1. Fixed bug when searching a mixture of real and mask databases.
 * 2. Clean up of code that calculates the number of sequences and database
 *    length.
 *
 * Revision 6.160  2002/06/24 18:50:20  dondosha
 * Memory leak fixed
 *
 * Revision 6.159  2002/06/13 19:49:24  dondosha
 * Hide previous change limiting number of HSPs for very large queries
 *
 * Revision 6.158  2002/06/13 17:44:27  dondosha
 * 1. Fixed a bug in revision 6.156;
 * 2. Limit the maximal number of saved HSPs per database sequence if the
 *    search space is too large to 100 times the number of query sequences.
 *
 * Revision 6.157  2002/06/11 14:44:47  dondosha
 * Return status from some functions instead of search block pointer
 *
 * Revision 6.156  2002/06/07 22:39:19  dondosha
 * Do not mask residues on any of the contexts except first - needed for megablast with non-greedy traceback
 *
 * Revision 6.155  2002/06/07 19:30:08  dondosha
 * Shift offsets for non-greedy preliminary extension when 1-base database scanning is used
 *
 * Revision 6.154  2002/06/07 16:47:42  dondosha
 * Re-sort results after traceback in case of non-greedy extension
 *
 * Revision 6.153  2002/05/17 21:40:13  dondosha
 * Added 2 optimal Mega BLAST word templates for length 21
 *
 * Revision 6.152  2002/05/14 22:20:20  dondosha
 * Renamed maximal discontiguous template type into optimal
 *
 * Revision 6.151  2002/05/10 22:38:49  dondosha
 * Always do preliminary and then traceback gapped extension if dynamic programming extension is used
 *
 * Revision 6.150  2002/05/09 17:01:23  dondosha
 * Renamed typedefs dp_ptr and dp_node to GapXDPPtr and GapXDP
 *
 * Revision 6.149  2002/05/08 22:48:12  dondosha
 * Allocate memory for dynamic programming upfront in Mega BLAST case
 *
 * Revision 6.148  2002/05/03 21:36:26  dondosha
 * Do not allocate memory for greedy algorithm if it is not going to be used
 *
 * Revision 6.147  2002/04/24 17:50:53  dondosha
 * Check percent identity cut-off in all cases
 *
 * Revision 6.146  2002/04/23 20:41:20  dondosha
 * In case of non-affine extension in megablast, check percent identity cutoff after the traceback is obtained
 *
 * Revision 6.145  2002/04/11 20:33:47  dondosha
 * Returned the reaping of hitlists by e-value before reevaluation with ambiguities
 *
 * Revision 6.144  2002/04/10 18:20:05  dondosha
 * Do not subtract 4 from wordsize in BlastFillQueryOffsets
 *
 * Revision 6.143  2002/04/09 18:21:00  dondosha
 * Changed #ifdefs to conditionals, allowing different discontiguous templates in a single binary
 *
 * Revision 6.142  2002/04/04 21:19:15  dondosha
 * Corrections for megablast with non-greedy extensions
 *
 * Revision 6.141  2002/04/01 22:26:51  dondosha
 * Correction in the MegaBlastReevaluateWithAmbiguities function: delete hsp if its evalue dropped below threshold after reevaluation
 *
 * Revision 6.140  2002/03/26 21:17:42  dondosha
 * Changed discontiguous word finding code to allow template length 21
 *
 * Revision 6.139  2002/03/06 18:34:31  dondosha
 * Pass the filtered locations back from the megablast engine to use in formatting
 *
 * Revision 6.138  2002/03/05 17:58:57  dondosha
 * Set same offsets for the traceback as for preliminary extension for megablast with non-greedy extensions
 *
 * Revision 6.137  2002/02/15 23:34:04  dondosha
 * 1. Allow use of hash table instead of index table (need -DUSE_HASH_TABLE)
 * 2. Allow use of two simultaneous word templates (with -DUSE_TWO_TEMPLATES)
 * 3. Added functionality fo print masked query sequence with lower case masking.
 *
 * Revision 6.136  2002/01/07 23:16:00  dondosha
 * Fixed several memory leaks and allocation/freeing bugs in multithreaded megablast
 *
 * Revision 6.135  2002/01/04 20:08:36  dondosha
 * Allocate filter string when rodent repeat filtering is needed for neighboring
 *
 * Revision 6.134  2001/12/28 20:46:21  dondosha
 * 1. Mega BLAST related parameters moved into a separate structure
 * 2. Environment variables for discontiguous words, etc. changed to options
 * 3. Extension from discontiguous word with length 18 implemented
 *
 * Revision 6.133  2001/12/13 16:07:17  dondosha
 * Do not use mb_endpoint_results in neighboring
 *
 * Revision 6.132  2001/12/11 19:45:56  dondosha
 * Repeat inclusion check after reevaluation of HSPs
 *
 * Revision 6.131  2001/12/10 22:54:27  dondosha
 * Correction for determining the number of stacks to use in megablast
 *
 * Revision 6.130  2001/12/04 17:13:08  dondosha
 * Made number of stacks for megablast word processing depend on query and database
 *
 * Revision 6.129  2001/11/27 22:17:20  dondosha
 * Allow dynamic programming gapped extension only with affine gap scores
 *
 * Revision 6.128  2001/11/16 16:25:34  dondosha
 * In MegaBlastReevaluateWithAmbiguities: bug from previous change fixed
 *
 * Revision 6.127  2001/11/14 23:45:46  dondosha
 * 1. Retrieve unpacked subject sequence directly in blastna encoding
 * 2. Implemented a possibility to use dynamic programming gapped extension
 *    via an environment variable.
 *
 * Revision 6.126  2001/11/13 18:23:54  dondosha
 * Move environment variable checking for discontiguous words to higher level function, so it is done only once
 *
 * Revision 6.125  2001/10/12 21:32:46  dondosha
 * Added discontiguous word capability to megablast
 *
 * Revision 6.124  2001/09/18 16:49:25  dondosha
 * Removed unneeded functions, eliminated mbutils.h header
 *
 * Revision 6.123  2001/09/07 14:46:43  dondosha
 * Roll back removal of threshold_first from functions and structures
 *
 * Revision 6.122  2001/09/06 20:24:34  dondosha
 * Removed threshold_first
 *
 * Revision 6.121  2001/09/05 21:30:11  dondosha
 * Purify error fixed
 *
 * Revision 6.120  2001/09/05 20:33:11  dondosha
 * Fixed a bug in changes from 07/20/2001
 *
 * Revision 6.119  2001/08/01 16:23:28  dondosha
 * Improvement in procedure removing HSPs on close diagonals
 *
 * Revision 6.118  2001/07/25 22:10:28  dondosha
 * Bug fix in hsp inclusion check
 *
 * Revision 6.117  2001/07/20 19:27:52  dondosha
 * 1. Added pv_array and diagonal array when needed.
 * 2. Added logic to decide when to unpack db sequence depending on its length.
 * 3. Rewrote reevaluation with ambiguities function to allow gapped alignment.
 *
 * Revision 6.116  2001/06/21 21:44:49  dondosha
 * Free allocated SeqLocs before error return from search set up
 *
 * Revision 6.115  2001/06/15 17:35:13  dondosha
 * Correction to previous changes
 *
 * Revision 6.114  2001/06/14 22:09:15  dondosha
 * Rearranged code for gi lists and oid masks processing to get rid of duplication
 *
 * Revision 6.113  2001/06/13 21:46:27  dondosha
 * Comparison function name in heapsort of gi list changed
 *
 * Revision 6.112  2001/06/05 22:15:10  dondosha
 * If some queries completely mask, destroy respective KarlinBlk
 *
 * Revision 6.111  2001/05/16 14:17:09  dondosha
 * Correction for previous revision
 *
 * Revision 6.110  2001/05/15 20:02:27  dondosha
 * Adjustments to accommodate query in a Bioseq form
 *
 * Revision 6.109  2001/05/08 20:18:16  dondosha
 * Corrections for removal of short queries and score reevaluation for affine gaps
 *
 * Revision 6.108  2001/05/04 19:50:45  dondosha
 * Improved error message when all queries are shorter than word size
 *
 * Revision 6.107  2001/05/04 15:55:21  dondosha
 * Take into account removal of short sequences in function BlastFillQueryOffsets
 *
 * Revision 6.106  2001/05/03 21:48:27  dondosha
 * Handle some cases when memory allocation fails
 *
 * Revision 6.105  2001/04/16 16:19:59  dondosha
 * Remove query sequences with length less than wordsize, print warning
 *
 * Revision 6.104  2001/03/22 19:23:09  dondosha
 * Changed hsp array new size variable Int4 from Int2
 *
 * Revision 6.103  2001/03/19 22:39:24  dondosha
 * Allow location on the first query sequence for megablast
 *
 * Revision 6.102  2001/03/12 19:10:10  dondosha
 * Do not use window size and multiple hits options in megablast
 *
 * Revision 6.101  2001/02/07 21:13:30  dondosha
 * Pass output stream from options block to search
 *
 * Revision 6.100  2001/01/24 20:51:50  dondosha
 * Enabled splitting of the second sequence for 2 sequences with megablast
 *
 * Revision 6.99  2001/01/11 18:31:22  dondosha
 * Changed error level for nonexistent database from ERROR to FATAL
 *
 * Revision 6.98  2001/01/03 21:45:31  dondosha
 * Fixed a memory leak - some edit blocks not freed in megablast
 *
 * Revision 6.97  2000/12/28 22:07:20  dondosha
 * Exit with error if database not found
 *
 * Revision 6.96  2000/12/22 19:48:49  dondosha
 * Corrected handling of percent identity cutoff in MegaBlastSaveCurrentHitlist
 *
 * Revision 6.95  2000/12/21 22:30:42  dondosha
 * Implemented discarding of HSPs below percent identity cutoff
 *
 * Revision 6.94  2000/12/14 17:55:25  dondosha
 * Fixed handling of subsequences masking in MegaBlastMaskTheResidues
 *
 * Revision 6.93  2000/12/07 20:30:37  dondosha
 * Initialize spp to NULL (not done in previous change)
 *
 * Revision 6.92  2000/12/07 17:48:31  dondosha
 * Handle non-whole query SeqLoc correctly in MegaBlastSetUpSearchInternalByLoc
 *
 * Revision 6.91  2000/11/29 23:06:24  dondosha
 * Removed restriction on number of alignments saved for a single query sequence
 *
 * Revision 6.90  2000/11/29 16:27:06  dondosha
 * Get the ncbi4na-encoded subject sequence only once per subject
 *
 * Revision 6.89  2000/11/21 20:18:56  dondosha
 * Look for HSP inclusion based on diagonal instead of offset
 *
 * Revision 6.88  2000/11/16 19:14:10  dondosha
 * Initialize mb_endpoint_results if no traceback
 *
 * Revision 6.87  2000/11/03 20:16:54  dondosha
 * Changed one_line_results parameter to no_traceback
 *
 * Revision 6.86  2000/10/31 15:06:15  dondosha
 * Added Boolean parameter to PrintMaskedSequence function
 *
 * Revision 6.85  2000/10/26 18:46:00  dondosha
 * Check if gi list file is provided from the db alias
 *
 * Revision 6.84  2000/10/19 16:02:13  dondosha
 * Changed MegaBlastGetPercentIdentity to MegaBlastGetNumIdentical
 *
 * Revision 6.83  2000/10/18 14:53:25  dondosha
 * Corrected MegaBlastReevaluateWithAmbiguities function treatment of lower case query regions
 *
 * Revision 6.82  2000/10/12 14:55:55  dondosha
 * Proceed with search even if some, but not all of the queries are completely masked
 *
 * Revision 6.81  2000/10/05 19:53:56  dondosha
 * Save search results in an array of BlastResultsStruct - separate results for each query
 *
 * Revision 6.80  2000/09/29 22:21:47  dondosha
 * Check if memory allocation succeeds to avoid core dumping exit
 *
 * Revision 6.79  2000/09/28 18:27:58  dondosha
 * Do not free lower case mask SeqLocs before the search
 *
 * Revision 6.78  2000/09/28 14:59:01  dondosha
 * Save initial hits in a small structure MegaBlastExactMatch instead of large BLAST_HSP
 *
 * Revision 6.77  2000/09/06 18:35:33  dondosha
 * Corrected percentage of identities computation for -D1 output
 *
 * Revision 6.76  2000/09/01 18:29:12  dondosha
 * Removed calls to ReadDBFreeSharedInfo and ReadDBCloseMHdrAndSeqFiles
 *
 * Revision 6.75  2000/08/30 22:08:44  dondosha
 * Added function BioseqMegaBlastEngineByLoc
 *
 * Revision 6.74  2000/08/25 22:37:15  dondosha
 * Added function MegaBlastReevaluateWithAmbiguities
 *
 * Revision 6.73  2000/08/18 20:12:30  dondosha
 * Do not use search->query_id in megablast, use only qid_array
 *
 * Revision 6.72  2000/08/07 16:59:50  dondosha
 * Correct construction of path for gi list file
 *
 * Revision 6.71  2000/08/07 16:16:12  dondosha
 * Corrected behavior when input contains an empty sequence
 *
 * Revision 6.70  2000/08/02 15:21:49  dondosha
 * Corrected the way Karlin block is computed for megablast
 *
 * Revision 6.69  2000/08/01 18:08:24  dondosha
 * Fixed function MaskSeqLocFromSeqAlign used for masked query output
 *
 * Revision 6.68  2000/07/17 14:37:11  dondosha
 * Fixed a memory leak in sequence masking
 *
 * Revision 6.67  2000/07/14 18:00:39  dondosha
 * Sort db sequences by best hit expect value in traditional output
 *
 * Revision 6.66  2000/07/11 16:46:17  dondosha
 * Fixed memory leak; use options query_lcase_mask variable for lower case masking
 *
 * Revision 6.65  2000/07/10 17:20:17  dondosha
 * Use several stacks for speed up of search with small word sizes
 *
 * Revision 6.64  2000/07/06 17:25:49  dondosha
 * Pass megablast_full_deflines option to search parameters
 *
 * Revision 6.63  2000/07/03 21:51:05  dondosha
 * For small wordsizes look for 2 hits in a window
 *
 * Revision 6.62  2000/06/30 15:07:11  dondosha
 * Fixed single strand search
 *
 * Revision 6.61  2000/06/29 21:10:37  dondosha
 * Sort seqaligns by evalue only among hits from same database sequence
 *
 * Revision 6.60  2000/06/29 14:52:10  dondosha
 * Typo in previous change
 *
 * Revision 6.59  2000/06/29 14:45:43  dondosha
 * Mask queries with N, not lower case for -Q option output
 *
 * Revision 6.58  2000/06/27 22:21:53  dondosha
 * Set no_check_score to TRUE; Added functions for masked query output
 *
 * Revision 6.57  2000/06/27 14:48:39  dondosha
 * Changed the procedure of HSP inclusion checking before gapped extension to avoid quadratic complexity
 *
 * Revision 6.56  2000/06/26 18:21:19  dondosha
 * Set awake_index to FALSE before any return from MegaBlastSetUpSearchInternalByLoc
 *
 * Revision 6.55  2000/06/26 17:35:30  dondosha
 * If some of the queries are completely masked, the search can still proceed
 *
 * Revision 6.54  2000/06/22 22:22:19  dondosha
 * Fixed a memory leak in MegaBlastNtWordExtend
 *
 * Revision 6.53  2000/06/21 18:03:20  dondosha
 * Use the same hsp_array for gapped extension
 *
 * Revision 6.52  2000/06/19 20:08:06  madden
 * Add last parameter to readdb_get_sequence_ex
 *
 * Revision 6.51  2000/06/19 19:42:35  dondosha
 * Fixed a bug causing megablast to crash when database is not found
 *
 * Revision 6.50  2000/06/19 19:26:56  dondosha
 * Do not go through a double loop on HSPs for small wordsize
 *
 * Revision 6.49  2000/06/07 17:39:51  dondosha
 * Fixed bug causing extra progress messages in nucleotide neighboring
 *
 * Revision 6.48  2000/05/31 14:07:14  dondosha
 * Reallocate memory for the hit stack if it is overflowing
 *
 * Revision 6.47  2000/05/26 19:22:28  dondosha
 * For neighboring test HSP length and percent identity before saving
 *
 * Revision 6.46  2000/05/25 21:00:34  dondosha
 * Set redundant HSPs to NULL in BlastSortUniqHspArray
 *
 * Revision 6.45  2000/05/24 19:51:03  dondosha
 * Initialize qid_array during search set-up; do not use readdb when megablast is used for two sequences
 *
 * Revision 6.44  2000/05/22 18:48:07  dondosha
 * Conform to change in ReadDBFILE structure
 *
 * Revision 6.43  2000/05/17 21:27:02  dondosha
 * Small improvement in handling edit script
 *
 * Revision 6.42  2000/05/17 17:11:13  dondosha
 * Removed some unused variables; fixed comparison function for sorting hsps
 *
 * Revision 6.41  2000/05/12 19:44:19  dondosha
 * Do not use SeqPort for retrieving queries; keep query ids in array to allow binary search; use ncbi4na encoding for subject sequences during extension; added routine BinarySearchInt4
 *
 * Revision 6.40  2000/05/03 20:23:04  dondosha
 * Added function MegaBlastGappedAlign to do the gapped extension after all perfect matches are saved - this significantly improves speed
 *
 * Revision 6.39  2000/05/01 21:25:54  dondosha
 * Changed greedy_gapped_align calls to MegaBlastAffineGreedyAlign; bug fix for neighboring; memory leak fix
 *
 * Revision 6.38  2000/04/25 21:51:45  dondosha
 * Little clean-up in MegaBlastNtWordExtend
 *
 * Revision 6.37  2000/04/20 15:14:36  dondosha
 * Fixed MegaBlastGetFirstAndLastContext and BlastFillQueryOffsets for one strand only search
 *
 * Revision 6.36  2000/04/19 18:54:33  dondosha
 * Bug fix: assign values for end-points different from gap or masked characters
 *
 * Revision 6.35  2000/04/18 19:09:03  dondosha
 * Set the X dropoff correctly for the megablast search
 *
 * Revision 6.34  2000/04/14 19:22:52  dondosha
 * Remove HSPs contained in other HSPs
 *
 * Revision 6.33  2000/04/12 19:11:28  dondosha
 * Create index_callback thread right after the creation of BlastSearchBlk
 *
 * Revision 6.32  2000/04/12 18:24:28  dondosha
 * Added MegaBlastGetPercentIdentity; fixed bugs with memory handling
 *
 * Revision 6.31  2000/04/11 21:00:19  dondosha
 * Fixed memory bug in BlastNeedHumanRepeatFiltering
 *
 * Revision 6.30  2000/04/11 19:51:50  dondosha
 * Removed setting of queue_callback to NULL
 *
 * Revision 6.29  2000/04/10 18:03:26  dondosha
 * Fixed a bug in previous change
 *
 * Revision 6.28  2000/04/10 17:44:52  dondosha
 * Find correct path for Homo_sapiens.n.gil, get this gilist only once
 *
 * Revision 6.27  2000/04/10 15:22:35  dondosha
 * Moved call to BlastFillQueryOffsets to MegaBlastSetUpSearchInternalByLoc
 *
 * Revision 6.26  2000/04/07 16:51:57  dondosha
 * Include callback argument for handling results in BioseqMegaBlastEngine, to be initialized in Main
 *
 * Revision 6.25  2000/04/05 18:14:48  dondosha
 * Moved SeqIdSetDup to objloc.c, slightly changed format of BlastSearchHandleResults
 *
 * Revision 6.24  2000/04/04 20:50:46  dondosha
 * Cleaned from non-megablast (non-blastn) stuff
 *
 * Revision 6.23  2000/04/04 16:16:35  dondosha
 * Fixed some memory leaks in MegaBlast traceback
 *
 * Revision 6.22  2000/04/03 23:37:08  dondosha
 * Added test for human repeat filtering for neighboring
 *
 * Revision 6.21  2000/03/31 19:10:32  dondosha
 * Changed some names related to MegaBlast
 *
 * Revision 6.20  2000/03/29 23:32:21  dondosha
 * Fixed minor bugs from previous change
 *
 * Revision 6.19  2000/03/29 22:13:43  dondosha
 * Enabled processing gap information for MegaBlast code
 *
 * Revision 6.18  2000/03/27 20:56:43  madden
 * Set query frame properly in BlastAdjustHitOffsets (for ungapped blast)
 *
 * Revision 6.17  2000/03/23 20:02:02  dondosha
 * BlastSearchHandleResults exits immediately if hspcnt==0
 *
 * Revision 6.16  2000/03/22 17:57:12  dondosha
 * Improved memory management in MegaBlast
 *
 * Revision 6.15  2000/03/16 18:14:00  dondosha
 * Added routine SeqIdSetDup, improved handling of megablast results
 *
 * Revision 6.14  2000/03/13 21:06:59  dondosha
 * Routine printing results rewritten, plus minor improvements
 *
 * Revision 6.13  2000/03/08 20:34:53  madden
 * Remove ungapped psi-blast stuff
 *
 * Revision 6.12  2000/03/03 18:12:22  dondosha
 * Added routine MegaBlastWordFinderDeallocate to fix multithreaded MegaBlast
 *
 * Revision 6.11  2000/03/02 18:29:00  dondosha
 * Minor bug fix in setting context offsets
 *
 * Revision 6.10  2000/03/02 17:23:21  dondosha
 * Added lower case masking plus bug fixes
 *
 * Revision 6.9  2000/02/24 23:20:15  dondosha
 * Fixed a bug in BlastAdjustHitOffsets
 *
 * Revision 6.8  2000/02/24 17:53:38  dondosha
 * Added routine BlastAdjustHitOffsets
 *
 * Revision 6.7  2000/02/23 20:28:38  dondosha
 * Bug fix for ungapped alignment search, plus style improvements
 *
 * Revision 6.6  2000/02/17 20:01:53  dondosha
 * Removed references to theCacheSize
 *
 * Revision 6.5  2000/02/12 21:18:57  kans
 * added prototype for MegaBlastBuildLookupTable - implemented in lookup.c, called from mblast.c
 *
 * Revision 6.4  2000/02/11 21:03:36  dondosha
 * Added new word finder and extension routines to work with new lokup table
 *
 * Revision 6.3  2000/02/03 21:21:10  dondosha
 * Added header; few bug fixes
 *
 * Revision 6.2  2000/02/02  15:03:44  dondosha
 * Removed unused routine ReapHitlistByContext
 *
 * */

#include <ncbi.h>
#include <blastpri.h>
#include <lookup.h>
#include <objcode.h>
#include <objseq.h>
#include <sequtil.h>
#include <tofasta.h>
#include <seqport.h>
#include <readdb.h>
#include <ncbithr.h>
#include <gapxdrop.h>
#include <dust.h>
#include <mbalign.h>
#include <mblast.h>
#include <time.h>

Int4
MegaBlastWordFinder (BlastSearchBlkPtr search, LookupTablePtr lookup);

Int4
MegaBlastExtendHit (BlastSearchBlkPtr search, LookupTablePtr lookup,
                    Int4 s_off, Int4 q_off);
Int2
MegaBlastNtWordExtend (BlastSearchBlkPtr search, Uint1Ptr subject0,
                       Int4 q_off, Int4 s_off);

Int2 MegaBlastGappedAlign (BlastSearchBlkPtr search);

void MegaBlastMaskTheResidues (Uint1Ptr buffer, Int4 max_length,
         Uint1 mask_residue, SeqLocPtr mask_slp, Boolean reverse,
	 Int4 offset, Boolean lowercase_mask);

void BlastLCaseMaskTheResidues (Uint1Ptr buffer, Int4 max_length,
         SeqLocPtr mask_slp, Boolean reverse, Int4 offset);

GapXEditScriptPtr MBToGapXEditScript (edit_script_t PNTR ed_script);

MBTemplateType GetMBTemplateType (Int2 weight, Int2 length,
                                  MBDiscWordType type);

SeqAlignPtr PNTR MegaBlastPackAlignmentsByQuery (BlastSearchBlkPtr search,
		     SeqAlignPtr seqalign);

Int2 LIBCALL MegaBlastSequenceAddSequence (BlastSequenceBlkPtr sequence_blk,
         Uint1Ptr sequence, Uint1Ptr sequence_start, Int4 length,
         Int4 original_length, Int4 effective_length);

Int4 SeqLocTotalLen (CharPtr prog_name, SeqLocPtr slp);

Boolean MegaBlastGetFirstAndLastContext (CharPtr prog_name,
            SeqLocPtr query_slp, Int2Ptr first_context,
            Int2Ptr last_context, Uint1 strand_options);

SeqAlignPtr PNTR
BioseqMegaBlastEngine (BioseqPtr PNTR bspp, CharPtr progname, CharPtr database,
		       BLAST_OptionsBlkPtr options, ValNodePtr *other_returns,
		       ValNodePtr *error_returns, int (LIBCALLBACK
						       *callback)(Int4 done,
								  Int4
								  positives),
		       SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list,
		       Int4 gi_list_total,
		       int (LIBCALLBACK *results_callback)PROTO((VoidPtr Ptr)))
{
   SeqLocPtr slp, next_slp;
   Int4 index = 0;
   SeqAlignPtr PNTR head;
   Int4 from = options->required_start, to = options->required_end;

   slp = NULL;

   /* If location given, apply it to the first sequence */
   if (from > 0 || to > 0) {
      if (to < 0)
         to = bspp[0]->length - 1;
      if (from >= bspp[0]->length || to < 0) {
         ErrPostEx(SEV_FATAL, 1, 0,
                   "Location outside of the query sequence range\n");
         return NULL;
      } else if (to <= from) {
         ErrPostEx(SEV_FATAL, 1, 0, "Empty query location provided\n");
         return NULL;
      }
      slp = SeqLocIntNew(from, to, options->strand_option, bspp[0]->id);
      index++;
   }

   for ( ; bspp[index] != NULL; index++)
      ValNodeAddPointer(&slp, SEQLOC_WHOLE, SeqIdSetDup(bspp[index]->id));

   head = BioseqMegaBlastEngineByLoc(slp, progname, database, options,
                                     other_returns, error_returns, callback,
                                     seqid_list, gi_list, gi_list_total,
                                     results_callback);

   if (slp->choice == SEQLOC_INT) {
     next_slp = slp->next;
     SeqLocFree(slp);
     slp = next_slp;
   }

   while (slp) {
     next_slp = slp->next;
     SeqIdSetFree((SeqIdPtr) slp->data.ptrvalue);
     MemFree(slp);
     slp = next_slp;
   }

   return head;
}

SeqAlignPtr PNTR
BioseqMegaBlastEngineByLoc (SeqLocPtr slp, CharPtr progname, CharPtr database,
                            BLAST_OptionsBlkPtr options, ValNodePtr *other_returns,
                            ValNodePtr *error_returns,
                            int (LIBCALLBACK *callback)(Int4 done, Int4 positives),
                            SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list,
                            Int4 gi_list_total,
                            int (LIBCALLBACK *results_callback)PROTO((VoidPtr Ptr)))
{
	Boolean options_allocated=FALSE;
	BlastSearchBlkPtr search;
	Int2 status;
	SeqAlignPtr PNTR head;
	SeqLocPtr whole_slp=NULL, slp_var;
	Int4 index;


	head = NULL;

	if (error_returns)
	{
		*error_returns = NULL;
	}

	if (other_returns)
	{
		*other_returns = NULL;
	}

	if (progname == NULL)
		return NULL;

	/* If no options, use default. */
        if (options == NULL)
	{
		options = BLASTOptionNewEx(progname, TRUE, TRUE);
		options_allocated = TRUE;
	}

	status = BLASTOptionValidateEx(options, progname, error_returns);
	if (status != 0)
	{	/* error messages in other_returns? */
		return NULL;
	}

	if (slp == NULL || database == NULL)
		return NULL;

	search = MegaBlastSetUpSearchWithReadDbInternal(slp, NULL, progname,
							0,
							database, options, NULL,
							seqid_list, gi_list,
							gi_list_total, NULL);

        if (search == NULL) {
           /* We need to veryfy if database name is wrong and to set error
               returns correctly */
            BlastErrorMsgPtr error_msg;
            CharPtr chptr;
            ReadDBFILEPtr rdfp=NULL;

            rdfp = readdb_new(database, FALSE);
            if(rdfp == NULL) {
                error_msg = MemNew(sizeof(BlastErrorMsg));
                chptr = MemNew(StringLen(database) + 256);
                sprintf(chptr, "Database %s was not found or does not exist",
                        database);
                error_msg->msg = chptr;
                error_msg->level = 3; /* FATAL */
                ValNodeAddPointer(error_returns, 0, error_msg);
            }

            readdb_destruct(rdfp);
            return NULL;
        }

	search->thr_info->tick_callback = callback;
	search->thr_info->star_callback = callback;
	search->handle_results = results_callback;
        search->output = options->output;

	head = BioseqMegaBlastEngineCore(search, options);
	
	if (search->error_return)
	{
		ValNodeLink(error_returns, search->error_return);
		search->error_return = NULL;
	}

	if (other_returns)
	{ /* format dbinfo etc.  */
		*other_returns = BlastOtherReturnsPrepare(search);
	}

	if (options_allocated)
	{
		options = BLASTOptionDelete(options);
	}
 
	search = BlastSearchBlkDestruct(search);

	/* Adjsut the offset if the query does not cover the entire sequence. */
	slp_var = slp;
	for (index=0; slp_var!=NULL; index++,slp_var=slp_var->next) {
           if (slp_var->choice != SEQLOC_WHOLE)	{
              ValNodeAddPointer(&whole_slp, SEQLOC_WHOLE, 
                                SeqIdFindBest(SeqLocId(slp_var), SEQID_GI));
              if (SeqLocAinB(whole_slp, slp_var) != 0)
                 AdjustOffSetsInSeqAlign(head[index], slp_var, NULL);
              ValNodeFree(whole_slp);
           }
	}

	return head;
}

static Int2 mb_two_hit_min_step;

SeqAlignPtr PNTR
BioseqMegaBlastEngineCore(BlastSearchBlkPtr search, BLAST_OptionsBlkPtr options)
{
	SeqAlignPtr seqalign, PNTR seqalignp;
        Int2 num_queries;
        CharPtr tmpstr;

	seqalign = NULL;
	seqalignp = NULL;
start_timer;
	if (search == NULL || search->query_invalid)
		return NULL;

	/* Starting awake thread if multithreaded. */
	if (search->searchsp_eff > AWAKE_THR_MIN_SIZE)
		BlastStartAwakeThread(search->thr_info);

        if ((tmpstr = getenv("MB_TWO_HIT_MIN_STEP")) != NULL) 
           mb_two_hit_min_step = atoi(tmpstr);
        else 
           mb_two_hit_min_step = 0;

	stop_timer("BioseqBlastEngineCore: before do_the_blast_run()");
	do_the_blast_run(search);

   /* If this is a partial search, we do not do traceback, so can return
      immediately */
   if (options->no_traceback == 1 || search->handle_results) {
      BlastStopAwakeThread(search->thr_info);
      return NULL;
   }
   
   /* So far the length of the first query was set to the total 
      concatenated length. It can be reset to the real length of the 
      first query sequence here. */
   search->context[search->first_context].query->length =
      search->query_context_offsets[search->first_context+1] - 1;
   
   
start_timer;
        num_queries = search->last_context / 2 + 1;
        seqalignp = (SeqAlignPtr PNTR) 
           MemNew(num_queries*sizeof(SeqAlignPtr));
        BLASTPostSearchLogic(search, options, seqalignp, FALSE);
        
        /* Stop the awake thread. */
        BlastStopAwakeThread(search->thr_info);
        
 stop_timer("MegaBlastEngineCore: after do_the_blast_run()");
        return seqalignp;
}

#define INDEX_THR_MIN_SIZE 20000
BlastSearchBlkPtr
MegaBlastSetUpSearchWithReadDbInternal (SeqLocPtr query_slp, BioseqPtr
					query_bsp, CharPtr prog_name, Int4 qlen, 
					CharPtr dbname, BLAST_OptionsBlkPtr
					options, int (LIBCALLBACK *callback)
					PROTO((Int4 done, Int4 positives)), 
					SeqIdPtr seqid_list, 
					BlastDoubleInt4Ptr gi_list, Int4
					gi_list_total, ReadDBFILEPtr rdfp)

{

	BlastSearchBlkPtr search;
	Boolean options_alloc=FALSE;
	Int2 status, first_context, last_context;
        Int8	dblen;
	Int4	query_length;
	Nlm_FloatHi	searchsp_eff=0;
        Int4 hitlist_size;

	/* Allocate default options if none are allocated yet. */
	if (options == NULL)
	{
		options = BLASTOptionNew(prog_name, FALSE);
		options_alloc = TRUE;
	}

        /* Increase the hitlist size for the preliminary gapped alignment,
           if traceback is delayed */
        if (options->no_traceback) {
           hitlist_size = 
              MIN(2*options->hitlist_size, options->hitlist_size + 50);
        } else {
           hitlist_size = options->hitlist_size;
        }
     
	/* last context is the total number of contexts in concatenated 
	   queries */

	if (query_slp) {
	   query_length = SeqLocTotalLen(prog_name, query_slp);
           MegaBlastGetFirstAndLastContext(prog_name, query_slp, &first_context, &last_context, options->strand_option);
	} else {
	   query_length = 2*query_bsp->length + 3;
           BlastGetFirstAndLastContext(prog_name, query_slp, &first_context, &last_context, options->strand_option);
        }
		
        qlen = 0;
        /* qlen != 0 is a flag for BlastSearchBlkNewExtra to create the 
           'diagonal array' structure for saving initial hits that will be
           used instead of stacks for word sizes < 11 and short to medium
           length queries only */
        if (options->wordsize < 11 && query_length <= MAX_DIAG_ARRAY)
           qlen = query_length;
	/* Pass nonzero length only if need to allocate ewp */
	search = BlastSearchBlkNewExtra(options->wordsize, qlen,
					dbname, FALSE, 0,
					options->threshold_second,
					hitlist_size, prog_name, NULL,
					first_context, last_context, rdfp,
					options->window_size);

	if (search) {
	   if (NlmThreadsAvailable() && query_length > INDEX_THR_MIN_SIZE) {
	      search->thr_info->awake_index = TRUE;
	      search->thr_info->last_tick = Nlm_GetSecs();
	      search->thr_info->index_thr = 
		 NlmThreadCreate(index_proc, search->thr_info);
	      search->thr_info->index_callback = callback;
	   }
	   readdb_get_totals_ex(search->rdfp, &(dblen), 
                                &(search->dbseq_num), TRUE);

           
       /* Create virtual database if any of the databases have gi lists or 
          ordinal id masks, or if gi list is provided from options */
       BlastProcessGiLists(search, options, gi_list, gi_list_total);

       /* search->thr_info->blast_gi_list will be non-NULL if gi_list or 
        * options->gilist or options->gifile was non-NULL and therefore
        * intersected with any oidlists in the search->rdfp(s). If this is the
        * case, we need to recalculate the database length and number of
        * sequences */
       if (search->thr_info->blast_gi_list && !options->use_real_db_size)
           readdb_get_totals_ex3(search->rdfp, &dblen, &search->dbseq_num,
                                 FALSE, TRUE, eApproximate);

           /* command-line/options trump alias file. */
           if (options->db_length > 0)
              dblen = options->db_length;
           if (options->dbseq_num > 0)
              search->dbseq_num = options->dbseq_num;
           if (options->searchsp_eff > 0)
              searchsp_eff = options->searchsp_eff;
           
           search->dblen = dblen;
           search->searchsp_eff = searchsp_eff;
           status = MegaBlastSetUpSearchInternalByLoc (search, query_slp, query_bsp,
                                                       prog_name, qlen, options,
                                                       callback);
           /* 
              Turn off the index thread by setting this flag.  
              Don't wait for a join, as the search will take much 
              longer than the one second for this to die.
           */
           search->thr_info->awake_index = FALSE;
           
           if (search->pbp->mb_params->no_traceback && 
               !search->pbp->mb_params->is_neighboring)
              search->mb_endpoint_results = ValNodeNew(NULL);
           
           if (status != 0) {
              ErrPostEx(SEV_WARNING, 0, 0, "MegaBlastSetUpSearch failed.");
              search->query_invalid = TRUE;
           }

           if (!search->query_invalid && !search->pbp->mb_params->use_dyn_prog) {
              search = GreedyAlignMemAlloc(search);
              if (search->abmp == NULL) {
                 BlastConstructErrorMessage("Mega BLAST", "Memory allocation for greedy algorithm failed.", 2, &(search->error_return));
                 search->query_invalid = TRUE;
              } 
           } else 
              search->abmp = NULL;
           if (search->rdfp->parameters & READDB_CONTENTS_ALLOCATED)
               search->rdfp = ReadDBCloseMHdrAndSeqFiles(search->rdfp);
	}
	
	if (options_alloc)
           options = BLASTOptionDelete(options);
        
	return search;
}

/* Function below assumes that the gi file is sorted in increasing order */
static Boolean 
FindGiInGiList(Uint4 query_gi, BlastDoubleInt4Ptr gi_list, 
               Int4 gi_list_size)
{
   Int4 begin, end, middle;
   
   begin = 0; 
   end = gi_list_size - 1;
   while (begin <= end) {
      middle = (begin + end) / 2;
      if (gi_list[middle].gi > query_gi)
	 end = middle - 1;
      else if (gi_list[middle].gi < query_gi)
	 begin = middle + 1;
      else {
	 return TRUE;
      }
   }
   return FALSE;
}

#define MIN_LIMIT_HSP_NUM 1e20
#define BLASTNA_N_RESIDUE 14

Int2
MegaBlastSetUpSearchInternalByLoc (BlastSearchBlkPtr search, SeqLocPtr
				   query_slp, BioseqPtr query_bsp, CharPtr
				   prog_name, Int4 qlen, BLAST_OptionsBlkPtr
				   options, int (LIBCALLBACK
						 *callback)PROTO((Int4 done,
								  Int4
								  positives)))

{
   Boolean mask_at_hash=FALSE, private_slp_delete;
   Char buffer[128];
   Int2 retval, status;
   Int4 effective_query_length, query_length,
      index, length, length_adjustment=0,
      min_query_length, full_query_length=0;
   Int4 context, num_queries;
   Nlm_FloatHi avglen;
   SeqIdPtr qid=NULL;
   SeqLocPtr filter_slp=NULL, private_slp=NULL, private_slp_rev=NULL, slp=NULL, tmp_slp=NULL;
   Uint1 residue;
   Uint1Ptr query_seq=NULL, query_seq_start=NULL, query_seq_rev=NULL, query_seq_start_rev=NULL;
   Uint1Ptr query_seq_combined=NULL;
   CharPtr filter_string=NULL;
   Uint4 query_gi;
   Int4 homo_gilist_size, mouse_gilist_size, rat_gilist_size;
   BlastDoubleInt4Ptr homo_gilist=NULL, mouse_gilist=NULL, rat_gilist=NULL;
   SeqLocPtr mask_slp=NULL, next_mask_slp=NULL;
   SeqPortPtr spp = NULL;
   
   if (options == NULL) {
      ErrPostEx(SEV_FATAL, 1, 0, "BLAST_OptionsBlkPtr is NULL\n");
      return 1;
   }
   
   if (query_slp == NULL && query_bsp == NULL) {
      ErrPostEx(SEV_FATAL, 1, 0, "Query is NULL\n");
      return 1;
   }
   
   context = search->first_context; 
   
   num_queries = search->last_context / 2 + 1;
   search->qid_array = (SeqIdPtr PNTR) MemNew(num_queries*sizeof(SeqIdPtr));

   if (!query_slp) {
      query_length = query_bsp->length;
      private_slp = SeqLocIntNew(0, query_length-1 , Seq_strand_plus, SeqIdFindBest(query_bsp->id, SEQID_GI));
      private_slp_rev = SeqLocIntNew(0, query_length-1 , Seq_strand_minus, SeqIdFindBest(query_bsp->id, SEQID_GI));
      private_slp_delete = FALSE;
      search->query_slp = private_slp;
      search->allocated += BLAST_SEARCH_ALLOC_QUERY_SLP;
      /* Now let query_length be the total length of 2 strands together with
         sentinel bytes */
      query_length = 2*query_length + 3;
   } else {
      search->query_slp = query_slp;
      query_length = SeqLocTotalLen(search->prog_name, query_slp);
   }

   if ((search->context[search->first_context].query->sequence_start = 
      (Uint1Ptr) Malloc((query_length+2)*sizeof(Uint1))) == NULL) {
      ErrPostEx(SEV_WARNING, 0, 0, "Memory allocation for query sequence failed");
      retval = 1;
      goto MegaBlastSetUpReturn;
   }

   search = BlastFillQueryOffsets(search, search->query_slp, 
                                  options->wordsize);

   search->translation_buffer = NULL;
   search->translation_buffer_size = 0;
   
   /*
     Set the context_factor, which specifies how many different 
     ways the query or db is examined (e.g., blastn looks at both
     stands of query, context_factor is 2).
   */
   /* All strands of all queries concatenated into one */
   search->context_factor = 1;
   
   /* Set the ambiguous residue before the ScoreBlk is filled. */
   if(options->matrix!=NULL) {
      search->sbp->read_in_matrix = TRUE;
   } else 
      search->sbp->read_in_matrix = FALSE;
   BlastScoreSetAmbigRes(search->sbp, 'N');
   
   search->sbp->penalty = options->penalty;
   search->sbp->reward = options->reward;
   
   /* option is to use alignments chosen by user in PSM computation API (used in WWW PSI-Blast); */
   search->pbp->use_best_align = options->use_best_align;
   
   /* Should culling be used at all? */
   search->pbp->perform_culling = options->perform_culling;
   search->pbp->hsp_range_max = options->hsp_range_max;
   search->pbp->mb_params = MegaBlastParameterBlkNew(options);
   
   search->pbp->mb_params->max_positions = MIN(query_length, options->block_width);
   search->sbp->query_length = query_length;
   
   search->mb_result_struct = (BLASTResultsStructPtr PNTR) 
      MemNew(num_queries*sizeof(BLASTResultsStructPtr));
      
   if (options->matrix != NULL)
      status = BlastScoreBlkMatFill(search->sbp, options->matrix);
   else
      status = BlastScoreBlkMatFill(search->sbp, "BLOSUM62");
   if (status != 0) {
      ErrPostEx(SEV_WARNING, 0, 0, "BlastScoreBlkMatFill returned non-zero status");
      retval = 1;
      goto MegaBlastSetUpReturn;
   }
   
   /* This is used right below. */
   search->pbp->gapped_calculation = options->gapped_calculation;
   search->pbp->do_not_reevaluate = options->do_not_reevaluate;
   search->pbp->do_sum_stats = options->do_sum_stats;
   search->pbp->first_db_seq = options->first_db_seq;
   search->pbp->final_db_seq = options->final_db_seq;

   retval = 0;
   
   context = search->first_context;
   
   if (private_slp && private_slp_delete)
      private_slp = SeqLocFree(private_slp);
   if (private_slp_rev)
      private_slp_rev = SeqLocFree(private_slp_rev);
   
   search->query_id = NULL;
   slp = search->query_slp;
   
   if (search->pbp->mb_params->is_neighboring) {
      CharPtr filename;
      filename = FindBlastDBFile("Homo_sapiens.n.gil");
      homo_gilist = GetGisFromFile (filename, &homo_gilist_size);
      MemFree(filename);
      filename = FindBlastDBFile("Mus_musculus.n.gil");
      mouse_gilist = GetGisFromFile(filename, &mouse_gilist_size);
      MemFree(filename);
      filename = FindBlastDBFile("Rattus_norvegicus.n.gil");
      rat_gilist = GetGisFromFile(filename, &rat_gilist_size);
      MemFree(filename);
   }

   /* All lower case masking locations are in one list */
   next_mask_slp = options->query_lcase_mask;

   while(context<=search->last_context && slp != NULL) {
      if (search->first_context == 0) {
	 private_slp = SeqLocIntNew(SeqLocStart(slp), SeqLocStop(slp), 
				    Seq_strand_plus, SeqLocId(slp));
      }
      if (search->last_context % 2 == 1) {
	 private_slp_rev = SeqLocIntNew(SeqLocStart(slp), SeqLocStop(slp), 
					Seq_strand_minus, SeqLocId(slp));
      }
      
      if (private_slp)
	 tmp_slp = private_slp;
      else 
	 tmp_slp = private_slp_rev;

      search->qid_array[context/2] = SeqIdSetDup(SeqLocId(slp));

      query_length = 0;
      query_length = SeqLocLen(tmp_slp);
      if (query_length < options->wordsize) {
         SeqIdWrite(SeqLocId(tmp_slp), buffer, PRINTID_FASTA_LONG, 127);
         ErrPostEx(SEV_WARNING, 0, 0, 
                   "Query sequence %s removed: length %ld is less than wordsize %d", 
                   buffer, query_length, options->wordsize);
         if (next_mask_slp &&
            SeqIdComp(SeqLocId(next_mask_slp), SeqLocId(tmp_slp)) == SIC_YES) {
            next_mask_slp = next_mask_slp->next;
         }
         slp = slp->next;
         context += 2;
	 continue;
      }

      if (options->filter && !options->filter_string)
	 options->filter_string = BlastConstructFilterString(options->filter);
      if (search->pbp->mb_params->is_neighboring) {
	 /* Test if we have to do human repeat filtering on this query */
	 if (private_slp)
	    qid = SeqLocId(private_slp);
	 else 
	    qid = SeqLocId(private_slp_rev);
	 query_gi = GetGIForSeqId(qid);
	 if (FindGiInGiList(query_gi, homo_gilist, homo_gilist_size)) {
	    filter_string = 
	       (CharPtr) Malloc(StringLen(options->filter_string)+3);
	    sprintf(filter_string, "%s%s", options->filter_string, ";R");
         } else if (FindGiInGiList(query_gi, mouse_gilist, mouse_gilist_size)
                    || FindGiInGiList(query_gi, rat_gilist, rat_gilist_size)) {
            filter_string =
               (CharPtr) Malloc(StringLen(options->filter_string)+18);

            sprintf(filter_string, "%s%s", options->filter_string, 
                    ";R -d rodents.lib");
	 } else
	    filter_string = StringSave(options->filter_string);
      } else
	 filter_string = options->filter_string;
      
      if (private_slp)
	 filter_slp = BlastSeqLocFilterEx(private_slp, filter_string, &mask_at_hash);
      else if (private_slp_rev)
	 filter_slp = BlastSeqLocFilterEx(private_slp_rev, filter_string, &mask_at_hash);
      if (search->pbp->mb_params->is_neighboring)
	 MemFree(filter_string);
	
      if (search->first_context==0 && search->last_context % 2 == 1)
	 query_seq_combined = 
	    (Uint1Ptr) Malloc((2*query_length+3)*sizeof(Uint1));
      else
	 query_seq_combined = 
	    (Uint1Ptr) Malloc((query_length+2)*sizeof(Uint1));

      if (query_seq_combined == NULL) {
         ErrPostEx(SEV_WARNING, 0, 0, "Memory allocation for query sequence failed");
         retval = 1;
         goto MegaBlastSetUpReturn;
      }

      query_seq_combined[0] = 0x0f;
      full_query_length = 0;

      /* Extract the mask location corresponding to this query */
      if (next_mask_slp && 
	  SeqIdComp(SeqLocId(next_mask_slp), SeqLocId(tmp_slp)) == SIC_YES) { 
	 mask_slp = (ValNodePtr) MemDup(next_mask_slp, sizeof(ValNode));
	 next_mask_slp = mask_slp->next;
	 mask_slp->next = NULL;
      } else
	 mask_slp = NULL;

      if (private_slp) {
         if (private_slp->choice == SEQLOC_INT) {
            spp = SeqPortNewByLoc(private_slp, Seq_code_ncbi4na);
            SeqPortSet_do_virtual(spp, TRUE);
         }


	 if ((query_seq_start = (Uint1Ptr)
              Malloc(((query_length)+2)*sizeof(Char))) == NULL) {
            ErrPostEx(SEV_WARNING, 0, 0, "Memory allocation for query sequence failed");
            retval = 1;
            goto MegaBlastSetUpReturn;
         }

	 query_seq = query_seq_start+1;

	 index=0;
         if (spp) {
            while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
               if (IS_residue(residue)) {
                  query_seq[index] = ncbi4na_to_blastna[residue];
                  index++;
               }
            }
            spp = SeqPortFree(spp);
         }

	 query_seq_start[0] = query_seq[query_length] = 0x0f;

         filter_slp = blastMergeFilterLocs(filter_slp, mask_slp, FALSE, 0, 0);
         mask_slp = MemFree(mask_slp);
         /* Mask by the blastna value of 'N' (15 in ncbi4na) */

         /* If masking done for lookup table only, mask only the combined
            sequence that will go into the first context */
         if (filter_slp && !mask_at_hash) {
            MegaBlastMaskTheResidues(query_seq, query_length,
				     BLASTNA_N_RESIDUE, filter_slp, FALSE,
				     SeqLocStart(private_slp), mask_at_hash);
         }
         MemCpy(&query_seq_combined[1], query_seq, query_length+1);
	 if (filter_slp && mask_at_hash) {
            MegaBlastMaskTheResidues(query_seq_combined+1, query_length,
				     BLASTNA_N_RESIDUE, filter_slp, FALSE,
				     SeqLocStart(private_slp), mask_at_hash);
         }

	 full_query_length = query_length + 1;
      }

      if (private_slp_rev) {
         spp = SeqPortNewByLoc(private_slp_rev, Seq_code_ncbi4na);
         SeqPortSet_do_virtual(spp, TRUE);
	 if ((query_seq_start_rev = (Uint1Ptr)
	    Malloc(((query_length)+2)*sizeof(Char))) == NULL) {
            ErrPostEx(SEV_WARNING, 0, 0, "Memory allocation for query sequence failed");
            retval = 1;
            goto MegaBlastSetUpReturn;
         }
	 query_seq_rev = query_seq_start_rev+1;
	 index=0;
         if (spp) {
            while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
               if (IS_residue(residue)) {
                  query_seq_rev[index] = ncbi4na_to_blastna[residue];
                  index++;
               }
            }
            spp = SeqPortFree(spp);
         }

	 query_seq_start_rev[0] = query_seq_rev[query_length] = 0x0f;
         if (filter_slp && !mask_at_hash) {
            MegaBlastMaskTheResidues(query_seq_rev, query_length, 
                                     BLASTNA_N_RESIDUE, filter_slp, TRUE, 
                                     SeqLocStart(private_slp_rev), 
                                     mask_at_hash);
         }
         MemCpy(query_seq_combined+full_query_length+1, 
                query_seq_rev,query_length+1);
	 if (filter_slp && mask_at_hash) {
            MegaBlastMaskTheResidues(query_seq_combined+full_query_length+1, 
                                     query_length, BLASTNA_N_RESIDUE, 
                                     filter_slp, TRUE, 
                                     SeqLocStart(private_slp_rev), 
                                     mask_at_hash);
         }               
	 full_query_length += query_length + 1;
      }

      MegaBlastSequenceAddSequence(search->context[search->first_context].query,
				   NULL, query_seq_combined, full_query_length,
				   full_query_length, 0);
      
      query_seq_combined = MemFree(query_seq_combined);
      if (context==search->first_context)
	 query_seq_start = MemFree(query_seq_start);
      
      if (private_slp && context != search->first_context) 
	 MegaBlastSequenceAddSequence(search->context[context].query, query_seq,
				      query_seq_start, query_length, 
				      query_length, 0);
      if (context % 2 == 0)
	 context++;
      if (private_slp_rev && context != search->first_context) {
	 MegaBlastSequenceAddSequence(search->context[context].query, query_seq_rev,
				      query_seq_start_rev, query_length,
				      query_length, 0);
      }
      context++;

   MegaBlastSetUpReturn:      
      if (filter_slp && !mask_at_hash)
         /* Save filtering locations for formatting */
         ValNodeAddPointer(&(search->mask), SEQLOC_MASKING_NOTSET, filter_slp);
      else /* No longer needed */
         SeqLocSetFree(filter_slp);

      if (private_slp)
	 private_slp = SeqLocFree(private_slp);
      if (private_slp_rev)
	 private_slp_rev = SeqLocFree(private_slp_rev);
      if (retval)
         return retval;
      slp = slp->next;
   } /* End of loop over query contexts (strands) */
   
   if (search->pbp->mb_params->is_neighboring) {
      MemFree(homo_gilist);
      MemFree(mouse_gilist);
      MemFree(rat_gilist);
   }
   
   retval = 1;
   for (index=search->first_context; index<=search->last_context; index++) {
      length = search->query_context_offsets[index+1] - 
         search->query_context_offsets[index] - 1;
      if (length > 0) {
         BLAST_KarlinBlk *kbp_gap;
         BLAST_KarlinBlk *kbp;

         if (retval)
            retval = 2;

         status = BlastScoreBlkFill(search->sbp, (CharPtr)
                                    search->context[index].query->sequence,
                                    length, index);
         search->sbp->kbp_gap_std[index] = BlastKarlinBlkCreate();
         kbp_gap = search->sbp->kbp_gap_std[index];
         kbp     = search->sbp->kbp_std[index];
           
         kbp_gap->Lambda = kbp->Lambda;
         kbp_gap->K      = kbp->K;
         kbp_gap->logK   = kbp->logK;
         kbp_gap->H      = kbp->H;
         kbp_gap->paramC = kbp->paramC;

         if (status==0)
            retval = 0;
         else
            search->sbp->kbp_std[index] = 
               BlastKarlinBlkDestruct(search->sbp->kbp_std[index]);
      }
   }

   /* Error only if all query sequences failed! */
   if (retval != 0) {
      if (retval == 1)
         sprintf(buffer, "All queries are shorter than word size or completely masked, no search performed");
      else 
         sprintf(buffer, "Unable to calculate Karlin-Altschul parameters, check query sequence");
      BlastConstructErrorMessage("BLASTSetUpSearch", buffer, 2, &(search->error_return));
   }

   search->sbp->kbp_gap = search->sbp->kbp_gap_std;
   search->sbp->kbp = search->sbp->kbp_std;
   
   /* If retval was set non-zero above (by the routines calculating Karlin-Altschul params),
      return here before these values are used.
   */
   if (retval)
      return retval;
   
   /* Calculate length adjustments and effective query lengths for 
      each query. */
   for( context=search->first_context;
        context<=search->last_context;
        context++ ) {
      /* Skip those contexts where sequence is not searched. */
      if (!search->sbp->kbp[context])
         continue;
      BlastComputeLengthAdjustment(search->sbp->kbp[context]->K,
                                   search->sbp->kbp[context]->logK,
                                   1/search->sbp->kbp[context]->H,
                                   0.0, 
                                   length,
                                   search->dblen, search->dbseq_num,
                                   &length_adjustment );

       length = search->query_context_offsets[context+1] -
           search->query_context_offsets[context] - 1;
       min_query_length = (Int4) 1/(search->sbp->kbp[context]->K);
       effective_query_length = 
          MAX(min_query_length, length - length_adjustment);
       search->context[context].query->effective_length =
           effective_query_length;
       if (context == search->first_context)
          search->length_adjustment = length_adjustment;
   }
   search->dblen_eff =
     search->dblen - search->dbseq_num*search->length_adjustment;
   /*if (search->searchsp_eff == 0)
      search->searchsp_eff = ((Nlm_FloatHi) search->dblen_eff) *
      ((Nlm_FloatHi) effective_query_length); */
   
   /* The default is that cutoff_s was not set and is zero. */
   if (options->cutoff_s == 0)	{
      search->pbp->cutoff_e = options->expect_value;
      search->pbp->cutoff_e_set = TRUE;
      search->pbp->cutoff_s = options->cutoff_s;
      search->pbp->cutoff_s_set = FALSE;
   } else {
      search->pbp->cutoff_e = options->expect_value;
      search->pbp->cutoff_e_set = FALSE;
      search->pbp->cutoff_s = options->cutoff_s;
      search->pbp->cutoff_s_set = TRUE;
   }
/* For now e2 is set to 0.5 and cutoff_e2_set is FALSE.  This is then
changed to the proper values in blast_set_parameters.  In the final version
of this program (where more blast programs and command-line options are
available) this needs to be set higher up. */
   if (options->cutoff_s2 == 0)	{
      search->pbp->cutoff_e2 = options->e2;
      search->pbp->cutoff_e2_set = FALSE;
      search->pbp->cutoff_s2 = options->cutoff_s2;
      search->pbp->cutoff_s2_set = FALSE;
   } else {
      search->pbp->cutoff_e2 = options->e2;
      search->pbp->cutoff_e2_set = FALSE;
      search->pbp->cutoff_s2 = options->cutoff_s2;
      search->pbp->cutoff_s2_set = TRUE;
   }
   
   search->pbp->discontinuous = options->discontinuous;
   
   
   /* For postion based blast. */
   search->pbp->ethresh = options->ethresh;
   search->pbp->maxNumPasses = options->maxNumPasses;
   search->pbp->pseudoCountConst = options->pseudoCountConst;
   
   search->pbp->process_num = options->number_of_cpus;
   search->pbp->cpu_limit = options->cpu_limit;
   search->pbp->gap_decay_rate = options->gap_decay_rate;
   search->pbp->gap_size = options->gap_size;
   search->pbp->gap_prob = options->gap_prob;
   search->pbp->old_stats = options->old_stats;
   search->pbp->use_large_gaps = options->use_large_gaps;
   search->pbp->number_of_bits = options->number_of_bits;
   search->pbp->two_pass_method = options->two_pass_method;
   search->pbp->multiple_hits_only = options->multiple_hits_only;
   search->pbp->gap_open = options->gap_open;
   search->pbp->gap_extend = options->gap_extend;
   search->pbp->decline_align = options->decline_align;
   
   if (options->hsp_num_max) {
      search->pbp->hsp_num_max = options->hsp_num_max;
      if (search->hsp_array_size > options->hsp_num_max)
         search->hsp_array_size = options->hsp_num_max;
   } else {
      FloatHi searchsp_eff = ((FloatHi) search->dblen_eff) *
         ((FloatHi) search->context[search->first_context].query->length);
      if (searchsp_eff > MIN_LIMIT_HSP_NUM) {
         char buffer[256];
         search->pbp->hsp_num_max = 50*(search->last_context + 1);
         sprintf(buffer, 
                 "Limiting the number of HSPs per database sequence to %d",
                 search->pbp->hsp_num_max);
         ErrPostEx(SEV_WARNING, 0, 0, buffer);
      }
   }


   /*search->pbp->gap_x_dropoff = (BLAST_Score)
     (options->gap_x_dropoff*NCBIMATH_LN2 /
     search->sbp->kbp[search->first_context]->Lambda);*/
   search->pbp->gap_x_dropoff = options->gap_x_dropoff;
   /* Ensures that gap_x_dropoff_final is at least as large as gap_x_dropoff. */
   search->pbp->gap_x_dropoff_final = MAX(options->gap_x_dropoff_final, search->pbp->gap_x_dropoff);
   
   /* "threshold" (first and second) must be set manually for two-pass right now.*/
   search->pbp->threshold_set = TRUE;
   search->pbp->threshold_second = options->threshold_second;
   
   search->pbp->window_size = options->window_size;
   search->pbp->window_size_set = TRUE;
   
   search->whole_query = TRUE;
   if (options->required_start != 0 || options->required_end != -1) {
      search->whole_query = FALSE;
      search->required_start = options->required_start;
      if (options->required_end != -1)
         search->required_end = options->required_end;
      else 
         search->required_end = qlen;
   }
   
   search->pbp->dropoff_2nd_pass = - options->dropoff_2nd_pass;
   
   avglen = BLAST_NT_AVGLEN;
   /* Use only one type of gap for blastn */
   search->pbp->ignore_small_gaps = TRUE;
   
   search->wfp = search->wfp_second;
   if (!MegaBlastBuildLookupTable(search)) {
      ErrPostEx(SEV_WARNING, 0, 0, "Failed to construct a lookup table");
      return 1;
   }
   
   return 0;
}

Boolean 
MegaBlastGetFirstAndLastContext(CharPtr prog_name, SeqLocPtr query_slp, Int2Ptr first_context, Int2Ptr last_context, Uint1 strand_options)
{
   SeqLocPtr tmp_slp = query_slp;
   Int2 tmp_first, tmp_last;
   Int2 index;

   if (StringCmp(prog_name, "blastn"))
      return BlastGetFirstAndLastContext(prog_name, query_slp, first_context, 
					 last_context, strand_options);
   
   if (!BlastGetFirstAndLastContext(prog_name, query_slp, first_context, 
				    last_context, strand_options))
      return FALSE;
   
   for (index=0; tmp_slp->next != NULL; index++, tmp_slp=tmp_slp->next);

   if (!BlastGetFirstAndLastContext(prog_name, tmp_slp, &tmp_first, 
				    &tmp_last, strand_options))
      return FALSE;
   /* For all intermediate sequences count 2 contexts */
   *last_context = 2 * index + tmp_last;
   return TRUE;
}


Int4 SeqLocTotalLen(CharPtr prog_name, SeqLocPtr slp)
{
   Int4 total_length = 0;
   SeqLocPtr tmp_slp = slp;

   if (StringCmp(prog_name, "blastn"))
      return SeqLocLen(slp);

   while (tmp_slp != NULL) {
      total_length += 2*(SeqLocLen(tmp_slp) + 1);
      tmp_slp = tmp_slp->next;
   }
   return total_length - 1;
}

BlastSearchBlkPtr 
BlastFillQueryOffsets(BlastSearchBlkPtr search, SeqLocPtr query_slp,
                      Int4 wordsize)
{
   SeqLocPtr slp = query_slp;
   Int2 num_contexts;
   Int2 i = 0;
   Int4 length;

   if (slp == NULL) return search;
   /* Note: the last offset will be equal to the combined query length + 1, for
      convenience of computing each individual query length later */
   for (slp=query_slp, num_contexts=0; slp; slp=slp->next, num_contexts+=2);
      
   search->query_context_offsets = (Int4Ptr) Malloc((num_contexts+1)*sizeof(Int4));
   
   search->query_context_offsets[0] = 0;
   for (slp = query_slp; slp; slp = slp->next) {
      length = SeqLocLen(slp) + 1;
      if (length <= wordsize)
         /* Sequence shorter than word size will be removed */
         length = 0;
      if (search->first_context == 0) 
	 search->query_context_offsets[i+1] = 
	    search->query_context_offsets[i] + length;
      else /* Top strand is not searched */
	 search->query_context_offsets[i+1] = 
	    search->query_context_offsets[i];
      i++;
      if (search->last_context % 2 == 1) 
	    search->query_context_offsets[i+1] = 
	       search->query_context_offsets[i] + length;
      else 
	 /* Bottom strand is not searched */
	 search->query_context_offsets[i+1] = 
	    search->query_context_offsets[i];
      i++;
   }	    
   

   return search;
}

static int LIBCALLBACK
evalue_compare_seqaligns(VoidPtr v1, VoidPtr v2)
{
   SeqAlignPtr sap1, sap2, PNTR sapp1, PNTR sapp2;

   sapp1 = (SeqAlignPtr PNTR) v1;
   sapp2 = (SeqAlignPtr PNTR) v2;
   
   sap1 = *sapp1;
   sap2 = *sapp2;
   
   if (sap1->score->value.realvalue > sap2->score->value.realvalue)
      return -1;
   else if (sap1->score->value.realvalue < sap2->score->value.realvalue)
      return 1;
   else if (sap1->score->value.intvalue < sap2->score->value.intvalue)
      return -1;
   else if (sap1->score->value.intvalue > sap2->score->value.intvalue)
      return 1;
   else 
      return 0;

}

/* Disassemble a list of SeqAligns into an array by query (in case of 
   multiple queries */
SeqAlignPtr PNTR MegaBlastPackAlignmentsByQuery(BlastSearchBlkPtr search,
						SeqAlignPtr seqalign)
{
   SeqAlignPtr PNTR seqalignp, PNTR sapp, PNTR sapp1;
   SeqAlignPtr seqalign_var, sap, seqalign_prev;
   Int2 index, i, hit_count, dbseq_count;
   SeqIdPtr query_id, seg_query_id;
   SeqLocPtr slp;
   Int4 num_queries = search->last_context / 2 + 1;
   Int4 count = 0;

   slp = search->query_slp;
   
   seqalignp = (SeqAlignPtr PNTR)
      MemNew(num_queries*sizeof(SeqAlignPtr));

   if (!seqalign) { 
      *seqalignp = NULL;
      return seqalignp;
   }

   for (index=0; index<num_queries; index++) {
      query_id = search->qid_array[index];
      hit_count = 0;
      seqalign_var = seqalign;
      seqalign_prev = NULL;
      while (seqalign_var != NULL) {
	 seg_query_id = ((DenseSegPtr)seqalign_var->segs)->ids;
	 if (SeqIdComp(query_id, seg_query_id) == SIC_YES) {
	    /* Remove this link from the list - we won't need it any more */
	    if (seqalign_prev != NULL)
	       seqalign_prev->next = seqalign_var->next;
	    else /* first link in the list */
	       seqalign = seqalign_var->next;
	    
	    seqalign_var->next = NULL;
	    
	    /* Add this seqalign to the list for this query */
	    if (seqalignp[index]==NULL) {
	       seqalignp[index] = seqalign_var;
	       sap = seqalignp[index];
	    } else {
	       sap->next = seqalign_var;
	       sap = sap->next;
	    }
	    hit_count++;
	    /* Move to the next link */
	    if (seqalign_prev != NULL)
	       seqalign_var = seqalign_prev->next;
	    else
	       seqalign_var = seqalign;
	 } else { /* hit from different query */
	    seqalign_prev = seqalign_var;
	    seqalign_var = seqalign_var->next;
	 }
      }

      count += hit_count;
      /* Now sort seqaligns for this query based on evalue */
      if (hit_count>1) {
	 Int4 start, end;
	 SeqIdPtr sid;

	 sapp = Nlm_Malloc(hit_count*sizeof(SeqAlignPtr));
	 sapp1 = Nlm_Malloc(hit_count*sizeof(SeqAlignPtr));
	 sapp[0] = seqalignp[index];

	 /* sapp is an array of pointers to all hits for this query */
	 /* sapp1 is an array of pointers to best hits for each db  */
	 /* sequence for this query                                 */
	 for (i=1; i<hit_count; i++)
	    sapp[i] = sapp[i-1]->next;

	 dbseq_count = 0;
	 for (start=0; start<hit_count; ) {
	    end = start; 
	    sid = ((DenseSegPtr)sapp[start]->segs)->ids->next;
	    for(end=start+1; end < hit_count &&
		SeqIdComp(sid, ((DenseSegPtr)sapp[end]->segs)->ids->next)
		   ==SIC_YES;
		end++);

	    if (end > start + 1)
	       HeapSort(&sapp[start], end-start, sizeof(SeqAlignPtr),
			evalue_compare_seqaligns);
	    for (i=start; i<end-1; i++)
	       sapp[i]->next = sapp[i+1];
	    sapp[end-1]->next = NULL; /* Separate lists for different db
					 sequences */
	    sapp1[dbseq_count++] = sapp[start];
	    start = end;
	 }
	 /* Sort the lists for different db sequences based on their 
	    best hits */
	 HeapSort(sapp1, dbseq_count, sizeof(SeqAlignPtr),
		  evalue_compare_seqaligns);
	 /* Connect the sorted lists */
	 for (i=0; i<dbseq_count-1; i++) {
	    for (sap=sapp1[i]; sap->next != NULL; sap = sap->next);
	    sap->next = sapp1[i+1];
	 }
	 
	 seqalignp[index] = sapp1[0];
	 MemFree(sapp);
	 MemFree(sapp1);
      }
   }
   /* Shouldn't be necessary, but clean just in case */
   SeqAlignSetFree(seqalign);
   return seqalignp;
}

/* Attach the "sequence" pointer to the BlastSequenceBlkPtr. sequence_start may be the
actual start of the sequence (this pointer is kept for deallocation purposes). The
sequence may start before "sequence" starts as there may be a sentinel (i.e., NULLB)
before the start of the sequence.  When the extension function extends this way it
can tell that there is a NULLB there and stop the extension.

*/

Int2 LIBCALL
MegaBlastSequenceAddSequence (BlastSequenceBlkPtr sequence_blk, Uint1Ptr sequence, 
			      Uint1Ptr sequence_start, Int4 length, 
			      Int4 original_length, Int4 effective_length)
{
   Uint1Ptr seqptr;

   if (sequence_blk == NULL)
      return 1;
   /* Assume that memory for sequence_blk->sequence is
      allocated elsewhere */
   if (sequence == NULL && sequence_start != NULL) {
      if (sequence_blk->sequence!=NULL && sequence_blk->length>0) {
	 seqptr = sequence_blk->sequence_start + sequence_blk->length;
	 MemCpy(seqptr, sequence_start, length+1);
      } else {
	 MemCpy(sequence_blk->sequence_start, sequence_start, length+1);
	 sequence_blk->sequence = sequence_blk->sequence_start + 1;
	 sequence_blk->effective_length = effective_length;
      }
   }
   else if (sequence != NULL) {
      sequence_blk->sequence = sequence;
      sequence_blk->sequence_start = sequence_start;
   }

   sequence_blk->length += length;
   sequence_blk->original_length += original_length;
   
   return 0;
}

static Boolean
MegaBlastSaveExactMatch(BlastSearchBlkPtr search, Int4 q_off, Int4 s_off) 
{
   MegaBlastExactMatchPtr match_array;
   Int4 num, num_avail;

   if (!search || !search->wfp || !search->wfp->lookup || 
       !search->wfp->lookup->mb_lt)
      return FALSE;
   
   num = search->current_hitlist->hspcnt;
   num_avail = search->current_hitlist->exact_match_max;

   match_array = search->current_hitlist->exact_match_array;
   if (num>=num_avail) {
      if (search->current_hitlist->do_not_reallocate) 
         return FALSE;
      num_avail *= 2;
      match_array = (MegaBlastExactMatchPtr) 
         Realloc(match_array, num_avail*sizeof(MegaBlastExactMatch));
      if (!match_array) {
         ErrPostEx(SEV_WARNING, 0, 0, "UNABLE to reallocate in MegaBlastSaveExactMatch for ordinal id %ld, continuing with fixed array of %ld HSP's", (long) search->subject_id, num_avail);
         search->current_hitlist->do_not_reallocate = TRUE;
         return FALSE;
      } else {
         search->current_hitlist->exact_match_max = num_avail;
         search->current_hitlist->exact_match_array = match_array;
      }
   }

   match_array[num].q_off = q_off;
   match_array[num].s_off = s_off;
   search->current_hitlist->hspcnt++;
   return TRUE;
}

#define MB_TEMPLATE_LENGTH 18

#ifndef BUFFER_LENGTH
#define BUFFER_LENGTH 255
#endif
#define MAX_LINE 48

/* Discontiguous words, database scanned 4 bases at a time */

static Int4 
MegaBlastWordFinder_disc(BlastSearchBlkPtr search, LookupTablePtr lookup)
{
   register Uint1Ptr subject;
   register Int4 s_off, ecode, mask, q_off, extra_code;
   Int4 ecode1, ecode2;
   Int4 subj_length = search->subject->length;
   Int4 min_hit_size;
   PV_ARRAY_TYPE *pv_array = lookup->pv_array;
   Int4 pv_array_bts;
   Int4 bit0, no_bit0;
   Int2 word_length;
#ifdef USE_HASH_TABLE
   Uint1Ptr hash_buf;
   Int4 hash_shift, hash_mask, crc, size;
#endif
   MegaBlastParameterBlkPtr mb_params = search->pbp->mb_params;
   Int2 template_length = mb_params->template_length;
   MBTemplateType template_type = mb_params->template_type;
   Boolean use_two_templates = mb_params->use_two_templates;

   min_hit_size = lookup->mb_lt->lpm;
   if (search->pbp->window_size > 0)
      min_hit_size += 4;

   if (mb_params->is_neighboring && 
       subj_length < MIN_NEIGHBOR_HSP_LENGTH)
      return 0;
   if (search->current_hitlist == NULL)
      search->current_hitlist = BlastHitListNew(search);
   
   if (mb_params->word_weight == 11) {
      no_bit0 = 0x007fffff;
      bit0 = 0x00800000;
   } else {
      no_bit0 = 0xffffffff;
      bit0 = 0;
   }

#ifdef USE_HASH_TABLE
   for (hash_shift=-1,size=lookup->mb_lt->hashsize; size; 
        size=size>>1,hash_shift++); 
   hash_shift = (32 - hash_shift)/2;
   hash_mask = lookup->mb_lt->hashsize - 1;
#endif

   word_length = (lookup->mb_lt->width + 1) * 4;
   if (template_length > word_length) {
      subj_length -= template_length - word_length;
   }

   mask = lookup->mb_lt->mask;
   subject = search->subject->sequence - 1;
   ecode = 0;
   if (lookup->mb_lt->estack)
      MemSet(lookup->mb_lt->stack_index, 0, 
             lookup->mb_lt->num_stacks*sizeof(Int4));

#ifdef USE_HASH_TABLE
   pv_array_bts = PV_ARRAY_BTS;
#else
   pv_array_bts = PV_ARRAY_BTS + ((lookup->mb_lt->width < 3) ? 0 : 5);
#endif

   for (s_off = 0; s_off < lookup->mb_lt->width*4; s_off += 4) {
      ecode = (ecode << 8) + *++subject;
   }

   s_off += 4;
   if (template_length == 16) {
      while (s_off<=subj_length) {
         ecode = ((ecode & mask) << 8) + *++subject;
         if (template_type == TEMPL_11_16)
            ecode1 = GET_WORD_INDEX_11_16(ecode) & no_bit0;
         else if (template_type == TEMPL_12_16)
            ecode1 = GET_WORD_INDEX_12_16(ecode) & no_bit0;
         else if (template_type == TEMPL_11_16_OPT)
            ecode1 = GET_WORD_INDEX_11_16_OPT(ecode) & no_bit0;
         else if (template_type == TEMPL_12_16_OPT)
            ecode1 = GET_WORD_INDEX_12_16_OPT(ecode) & no_bit0;
#ifdef USE_HASH_TABLE /* Using hash table */
         hash_buf = (Uint1Ptr)&ecode1;
         CRC32(crc, hash_buf);
         ecode1 = (crc>>hash_shift) & hash_mask;
#endif
         if (use_two_templates) {
            if (template_type == TEMPL_11_16)
               ecode2 = GET_WORD_INDEX_11_16_OPT(ecode) | bit0;
            else if (template_type == TEMPL_12_16)
               ecode2 = GET_WORD_INDEX_12_16_OPT(ecode) | bit0;
         }
#ifdef USE_HASH_TABLE /* Using hash table */
         hash_buf = (Uint1Ptr)&ecode2;
         CRC32(crc, hash_buf);
         ecode2 = (crc>>hash_shift) & hash_mask;
#endif
         if (pv_array) {
            while ((s_off <= subj_length) && ((pv_array[ecode1>>pv_array_bts]&
                     (((PV_ARRAY_TYPE) 1)<<(ecode1&PV_ARRAY_MASK))) == 0)
                   && (!use_two_templates || ((pv_array[ecode2>>pv_array_bts]&
                     (((PV_ARRAY_TYPE) 1)<<(ecode2&PV_ARRAY_MASK))) == 0))) {
               ecode = ((ecode & mask) << 8) + *++subject;
               if (template_type == TEMPL_11_16)
                  ecode1 = GET_WORD_INDEX_11_16(ecode) & no_bit0;
               else if (template_type == TEMPL_12_16)
                  ecode1 = GET_WORD_INDEX_12_16(ecode) & no_bit0;
               else if (template_type == TEMPL_11_16_OPT)
                  ecode1 = GET_WORD_INDEX_11_16_OPT(ecode) & no_bit0;
               else if (template_type == TEMPL_12_16_OPT)
                  ecode1 = GET_WORD_INDEX_12_16_OPT(ecode) & no_bit0;
#ifdef USE_HASH_TABLE
               hash_buf = (Uint1Ptr)&ecode1;
               CRC32(crc, hash_buf);
               ecode1 = (crc>>hash_shift) & hash_mask;
#endif

               if (use_two_templates) {
                  if (template_type == TEMPL_11_16)
                     ecode2 = GET_WORD_INDEX_11_16_OPT(ecode) | bit0;
                  else if (template_type == TEMPL_12_16)
                     ecode2 = GET_WORD_INDEX_12_16_OPT(ecode) | bit0;
               }
#ifdef USE_HASH_TABLE
               hash_buf = (Uint1Ptr)&ecode2;
               CRC32(crc, hash_buf);
               ecode2 = (crc>>hash_shift) & hash_mask;
#endif
               s_off += 4;
            } 
            if (s_off > subj_length)
               break;
         }
         for (q_off = lookup->mb_lt->hashtable[ecode1]; q_off>0;
              q_off = lookup->mb_lt->next_pos[q_off]) {
            search->second_pass_hits++;
            MegaBlastExtendHit(search, lookup, s_off, q_off);
         }
         if (use_two_templates) {
            for (q_off = lookup->mb_lt->hashtable2[ecode2]; q_off>0;
                 q_off = lookup->mb_lt->next_pos2[q_off]) {
               search->second_pass_hits++;
               MegaBlastExtendHit(search, lookup, s_off, q_off);
            }
         }
         s_off += 4;
      }

   } else { /* template length > 16 */
      extra_code = 0;
      while (s_off<=subj_length) {
         ecode = ((ecode & mask) << 8) + *++subject;
         switch (template_type) {
         case TEMPL_11_18: 
            extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18(subject);
            ecode1 = (GET_WORD_INDEX_11_18(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_12_18: 
            extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18(subject);
            ecode1 = (GET_WORD_INDEX_12_18(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_11_18_OPT: 
            extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18_OPT(subject);
            ecode1 = (GET_WORD_INDEX_11_18_OPT(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_12_18_OPT:
            extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18_OPT(subject);
            ecode1 = (GET_WORD_INDEX_12_18_OPT(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_11_21: 
            extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21(subject);
            ecode1 = (GET_WORD_INDEX_11_21(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_12_21:
            extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21(subject);
            ecode1 = (GET_WORD_INDEX_12_21(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_11_21_OPT: 
            extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21_OPT(subject);
            ecode1 = (GET_WORD_INDEX_11_21_OPT(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_12_21_OPT:
            extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21_OPT(subject);
            ecode1 = (GET_WORD_INDEX_12_21_OPT(ecode) | extra_code) & no_bit0;
            break;
         default: 
            extra_code = 0; 
            ecode1 = 0;
            break;
         }
#ifdef USE_HASH_TABLE
         hash_buf = (Uint1Ptr)&ecode1;
         CRC32(crc, hash_buf);
         ecode1 = (crc>>hash_shift) & hash_mask;
#endif

         if (use_two_templates) {
            switch (template_type) {
            case TEMPL_11_18:
               extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18_OPT(subject);
               ecode2 = GET_WORD_INDEX_11_18_OPT(ecode) |
                  extra_code | bit0;
               break;
            case TEMPL_12_18:
               extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18_OPT(subject);
               ecode2 = GET_WORD_INDEX_12_18_OPT(ecode) |
                  extra_code | bit0;
               break;
            case TEMPL_11_21:
               extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21_OPT(subject);
               ecode2 = GET_WORD_INDEX_11_21_OPT(ecode) |
                  extra_code | bit0;
               break;
            case TEMPL_12_21:
               extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21_OPT(subject);
               ecode2 = GET_WORD_INDEX_12_21_OPT(ecode) |
                  extra_code | bit0;
               break;
            default: 
               ecode2 = 0; 
               break;
            }
#ifdef USE_HASH_TABLE
            hash_buf = (Uint1Ptr)&ecode2;
            CRC32(crc, hash_buf);
            ecode2 = (crc>>hash_shift) & hash_mask;
#endif
         }
         if (pv_array) {
            while ((s_off <= subj_length) && ((pv_array[ecode1>>pv_array_bts]&
                     (((PV_ARRAY_TYPE) 1)<<(ecode1&PV_ARRAY_MASK))) == 0)
                   && (!use_two_templates || ((pv_array[ecode2>>pv_array_bts]&
                       (((PV_ARRAY_TYPE) 1)<<(ecode2&PV_ARRAY_MASK))) == 0))) {
               ecode = ((ecode & mask) << 8) + *++subject;

               switch (template_type) {
               case TEMPL_11_18:
                  extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18(subject);
                  ecode1 = (GET_WORD_INDEX_11_18(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_12_18:
                  extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18(subject);
                  ecode1 = (GET_WORD_INDEX_12_18(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_11_18_OPT: 
                  extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18_OPT(subject);
                  ecode1 = (GET_WORD_INDEX_11_18_OPT(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_12_18_OPT:
                  extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18_OPT(subject);
                  ecode1 = (GET_WORD_INDEX_12_18_OPT(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_11_21: 
                  extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21(subject);
                  ecode1 = (GET_WORD_INDEX_11_21(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_12_21:
                  extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21(subject);
                  ecode1 = (GET_WORD_INDEX_12_21(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_11_21_OPT: 
                  extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21_OPT(subject);
                  ecode1 = (GET_WORD_INDEX_11_21_OPT(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_12_21_OPT:
                  extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21_OPT(subject);
                  ecode1 = (GET_WORD_INDEX_12_21_OPT(ecode) | extra_code) & no_bit0;
                  break;
               default: 
                  extra_code = 0; 
                  ecode1 = 0;
                  break;
               }

#ifdef USE_HASH_TABLE
               hash_buf = (Uint1Ptr)&ecode1;
               CRC32(crc, hash_buf);
               ecode1 = (crc>>hash_shift) & hash_mask;
#endif

               if (use_two_templates) {
                  switch (template_type) {
                  case TEMPL_11_18:
                     extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18_OPT(subject);
                     ecode2 = GET_WORD_INDEX_11_18_OPT(ecode) |
                        extra_code | bit0;
                     break;
                  case TEMPL_12_18:
                     extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_18_OPT(subject);
                     ecode2 = GET_WORD_INDEX_12_18_OPT(ecode) |
                        extra_code | bit0;
                     break;
                   case TEMPL_11_21:
                     extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21_OPT(subject);
                     ecode2 = GET_WORD_INDEX_11_21_OPT(ecode) |
                        extra_code | bit0;
                     break;
                  case TEMPL_12_21:
                     extra_code = (Int4) GET_EXTRA_CODE_PACKED_4_21_OPT(subject);
                     ecode2 = GET_WORD_INDEX_12_21_OPT(ecode) |
                        extra_code | bit0;
                     break;
                 default:
                     ecode2 = 0; break;
                  }
#ifdef USE_HASH_TABLE
                  hash_buf = (Uint1Ptr)&ecode2;
                  CRC32(crc, hash_buf);
                  ecode2 = (crc>>hash_shift) & hash_mask;
#endif
               }
               s_off += 4;
            } 
            if (s_off > subj_length)
               break;
         }
         for (q_off = lookup->mb_lt->hashtable[ecode1]; q_off>0;
              q_off = lookup->mb_lt->next_pos[q_off]) {
            search->second_pass_hits++;
            MegaBlastExtendHit(search, lookup, s_off, q_off);
         }
         if (use_two_templates) {
            for (q_off = lookup->mb_lt->hashtable2[ecode2]; q_off>0;
                 q_off = lookup->mb_lt->next_pos2[q_off]) {
               search->second_pass_hits++;
               MegaBlastExtendHit(search, lookup, s_off, q_off);
            }
         }
         s_off += 4;
      }
   }

   if (!lookup->mb_lt->estack)
      BlastExtendWordExit(search);

   search->current_hitlist->do_not_reallocate = FALSE;

   /* Do greedy gapped extension */
   if (search->current_hitlist->hspcnt > 0)
      if (MegaBlastGappedAlign(search) != 0)
         return -1;

   return search->current_hitlist->hspcnt;
}

/* Discontiguous words, database scanned 1 base at a time */

#define mask30b 0x3fffffff
#define mask2b  0x00000003

static Int4 
MegaBlastWordFinder_disc_1b(BlastSearchBlkPtr search, LookupTablePtr lookup)
{
   register Uint1Ptr subject;
   register Int4 s_off, ecode, mask, q_off, extra_code;
   Int4 ecode1, ecode2;
   Int4 subj_length = search->subject->length;
   Int4 min_hit_size;
   PV_ARRAY_TYPE *pv_array = lookup->pv_array;
   Int4 pv_array_bts;
   Uint1 bit;
   Int4 no_bit0, bit0;
   Int2 word_length;
#ifdef USE_HASH_TABLE
   Uint1Ptr hash_buf;
   Int4 hash_shift, hash_mask, crc, size;
#endif
   MegaBlastParameterBlkPtr mb_params = search->pbp->mb_params;
   Int2 template_length = mb_params->template_length;
   MBTemplateType template_type = mb_params->template_type;
   Boolean use_two_templates = mb_params->use_two_templates;

   min_hit_size = lookup->mb_lt->lpm;
   if (search->pbp->window_size > 0)
      min_hit_size += 4;

   if (mb_params->is_neighboring && 
       subj_length < MIN_NEIGHBOR_HSP_LENGTH)
      return 0;
   if (search->current_hitlist == NULL)
      search->current_hitlist = BlastHitListNew(search);
   
   if (mb_params->word_weight == 11) {
      no_bit0  = 0x007fffff;
      bit0     = 0x00800000;
   } else {
      no_bit0 = -1;
      bit0 = 0;
   }


#ifdef USE_HASH_TABLE
   for (hash_shift=0,size=lookup->mb_lt->hashsize; size; 
        size=size>>1,hash_shift++); 
   hash_shift = (32 - hash_shift)/2;
   hash_mask = lookup->mb_lt->hashsize - 1;
#endif

   word_length = (lookup->mb_lt->width + 1) * 4;
   if (template_length > word_length) {
      subj_length -= template_length - word_length;
   }

   mask = lookup->mb_lt->mask;
   subject = search->subject->sequence - 1;
   ecode = 0;
   if (lookup->mb_lt->estack)
      MemSet(lookup->mb_lt->stack_index, 0, 
             lookup->mb_lt->num_stacks*sizeof(Int4));

   pv_array_bts = PV_ARRAY_BTS + ((lookup->mb_lt->width < 3) ? 0 : 5);

   for (s_off = 0; s_off < lookup->mb_lt->width*4; s_off += 4) {
      ecode = (ecode << 8) + *++subject;
   }

   bit = 3;
   s_off++;
   if (template_length == word_length) {
      while (s_off<=subj_length) {
         if (bit == 3) subject++;
         ecode = ((ecode & mask30b) << 2) | (((*subject)>>(2*bit)) & mask2b);
         if (template_type == TEMPL_11_16)
            ecode1 = GET_WORD_INDEX_11_16(ecode) & no_bit0;
         else if (template_type == TEMPL_12_16)
            ecode1 = GET_WORD_INDEX_12_16(ecode) & no_bit0;
         else if (template_type == TEMPL_11_16_OPT)
            ecode1 = GET_WORD_INDEX_11_16_OPT(ecode) & no_bit0;
         else if (template_type == TEMPL_12_16_OPT)
            ecode1 = GET_WORD_INDEX_12_16_OPT(ecode) & no_bit0;
#ifdef USE_HASH_TABLE
         hash_buf = (Uint1Ptr)&ecode1;
         CRC32(crc, hash_buf);
         ecode1 = (crc>>hash_shift) & hash_mask;
#endif

         if (use_two_templates) {
            switch (template_type) {
            case TEMPL_11_16:
               ecode2 = GET_WORD_INDEX_11_16_OPT(ecode) | bit0;
               break;
            case TEMPL_12_16:
               ecode2 = GET_WORD_INDEX_12_16_OPT(ecode) | bit0;
               break;
            default: 
               ecode2 = 0; break;
            }
#ifdef USE_HASH_TABLE
            hash_buf = (Uint1Ptr)&ecode2;
            CRC32(crc, hash_buf);
            ecode2 = (crc>>hash_shift) & hash_mask;
#endif
         }
         bit = (bit - 1) & mask2b;
         
         if (pv_array) {
            while ((s_off <= subj_length) && ((pv_array[ecode1>>pv_array_bts]&
                      (((PV_ARRAY_TYPE) 1)<<(ecode1&PV_ARRAY_MASK))) == 0)
                   && (!use_two_templates || ((pv_array[ecode2>>pv_array_bts]&
                        (((PV_ARRAY_TYPE) 1)<<(ecode2&PV_ARRAY_MASK))) == 0))) {
               if (bit == 3) subject++;
               ecode = ((ecode & mask30b) << 2) | 
                  (((*subject)>>(2*bit)) & mask2b);
               if (template_type == TEMPL_11_16)
                  ecode1 = GET_WORD_INDEX_11_16(ecode) & no_bit0;
               else if (template_type == TEMPL_12_16)
                  ecode1 = GET_WORD_INDEX_12_16(ecode) & no_bit0;
               else if (template_type == TEMPL_11_16_OPT)
                  ecode1 = GET_WORD_INDEX_11_16_OPT(ecode) & no_bit0;
               else if (template_type == TEMPL_12_16_OPT)
                  ecode1 = GET_WORD_INDEX_12_16_OPT(ecode) & no_bit0;
#ifdef USE_HASH_TABLE
               hash_buf = (Uint1Ptr)&ecode1;
               CRC32(crc, hash_buf);
               ecode1 = (crc>>hash_shift) & hash_mask;
#endif
               if (use_two_templates) {
                  switch (template_type) {
                  case TEMPL_11_16:
                     ecode2 = GET_WORD_INDEX_11_16_OPT(ecode) | bit0;
                     break;
                  case TEMPL_12_16:
                     ecode2 = GET_WORD_INDEX_12_16_OPT(ecode) | bit0;
                     break;
                  default: 
                     ecode2 = 0; break;
                  }
#ifdef USE_HASH_TABLE
                  hash_buf = (Uint1Ptr)&ecode2;
                  CRC32(crc, hash_buf);
                  ecode2 = (crc>>hash_shift) & hash_mask;
#endif
               }
               bit = (bit - 1) & mask2b;
               s_off++;
            } 
            if (s_off > subj_length)
               break;
         }
         for (q_off = lookup->mb_lt->hashtable[ecode1]; q_off>0;
              q_off = lookup->mb_lt->next_pos[q_off]) {
            search->second_pass_hits++;
            MegaBlastExtendHit(search, lookup, s_off, q_off);
         }
         if (use_two_templates) {
            for (q_off = lookup->mb_lt->hashtable2[ecode2]; q_off>0;
                 q_off = lookup->mb_lt->next_pos2[q_off]) {
               search->second_pass_hits++;
               MegaBlastExtendHit(search, lookup, s_off, q_off);
            }
         }
         s_off++;
      }
   } else {
      Uint1Ptr seqptr;
      Uint1 bitval;
      Int4 tmpval;
      extra_code = 0;
      while (s_off<=subj_length) {
         if (bit == 3) subject++;
         ecode = ((ecode & mask30b) << 2) | (((*subject)>>(2*bit)) & mask2b);
         seqptr = subject;
         bitval = bit;
         extra_code = 0;
         tmpval = 0;
         switch (template_type) {
         case TEMPL_11_18:
            GET_EXTRA_CODE_PACKED_18(seqptr, bitval, tmpval, extra_code);
            ecode1 = (GET_WORD_INDEX_11_18(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_12_18:
            GET_EXTRA_CODE_PACKED_18(seqptr, bitval, tmpval, extra_code);
            ecode1 = (GET_WORD_INDEX_12_18(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_11_18_OPT:
            GET_EXTRA_CODE_PACKED_18_OPT(seqptr, bitval, tmpval, extra_code);
            ecode1 = (GET_WORD_INDEX_11_18_OPT(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_12_18_OPT:
            GET_EXTRA_CODE_PACKED_18_OPT(seqptr, bitval, tmpval, extra_code);
            ecode1 = (GET_WORD_INDEX_12_18_OPT(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_11_21:
            GET_EXTRA_CODE_PACKED_21(seqptr, bitval, tmpval, extra_code);
            ecode1 = (GET_WORD_INDEX_11_21(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_12_21:
            GET_EXTRA_CODE_PACKED_21(seqptr, bitval, tmpval, extra_code);
            ecode1 = (GET_WORD_INDEX_12_21(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_11_21_OPT:
            GET_EXTRA_CODE_PACKED_21_OPT(seqptr, bitval, tmpval, extra_code);
            ecode1 = (GET_WORD_INDEX_11_21_OPT(ecode) | extra_code) & no_bit0;
            break;
         case TEMPL_12_21_OPT:
            GET_EXTRA_CODE_PACKED_21_OPT(seqptr, bitval, tmpval, extra_code);
            ecode1 = (GET_WORD_INDEX_12_21_OPT(ecode) | extra_code) & no_bit0;
            break;
         default:
            extra_code = 0; ecode1 = 0; break;
         }
#ifdef USE_HASH_TABLE
         hash_buf = (Uint1Ptr)&ecode1;
         CRC32(crc, hash_buf);
         ecode1 = (crc>>hash_shift) & hash_mask;
#endif
         if (use_two_templates) {
            seqptr = subject;
            bitval = bit;
            extra_code = 0;
            tmpval = 0;
            switch (template_type) {
            case TEMPL_11_18:
               GET_EXTRA_CODE_PACKED_18_OPT(seqptr, bitval, tmpval, extra_code);
               ecode2 = GET_WORD_INDEX_11_18_OPT(ecode) | extra_code | bit0;
               break;
            case TEMPL_12_18:
               GET_EXTRA_CODE_PACKED_18_OPT(seqptr, bitval, tmpval, extra_code);
               ecode2 = GET_WORD_INDEX_12_18_OPT(ecode) | extra_code | bit0;
               break;
            case TEMPL_11_21:
               GET_EXTRA_CODE_PACKED_21_OPT(seqptr, bitval, tmpval, extra_code);
               ecode2 = GET_WORD_INDEX_11_21_OPT(ecode) | extra_code | bit0;
               break;
            case TEMPL_12_21:
               GET_EXTRA_CODE_PACKED_21_OPT(seqptr, bitval, tmpval, extra_code);
               ecode2 = GET_WORD_INDEX_12_21_OPT(ecode) | extra_code | bit0;
               break;
            default: 
               ecode2 = 0; break;
            }
#ifdef USE_HASH_TABLE
            hash_buf = (Uint1Ptr)&ecode2;
            CRC32(crc, hash_buf);
            ecode2 = (crc>>hash_shift) & hash_mask;
#endif
         }
         bit = (bit - 1) & mask2b;
         if (pv_array) {
            while ((s_off <= subj_length) && ((pv_array[ecode1>>pv_array_bts]&
                      (((PV_ARRAY_TYPE) 1)<<(ecode1&PV_ARRAY_MASK))) == 0)
                   && (!use_two_templates || ((pv_array[ecode2>>pv_array_bts]&
                        (((PV_ARRAY_TYPE) 1)<<(ecode2&PV_ARRAY_MASK))) == 0))) {
               if (bit == 3) subject++;
               ecode = ((ecode & mask30b) << 2) | (((*subject)>>(2*bit)) & mask2b);
               seqptr = subject;
               bitval = bit;
               extra_code = 0;
               tmpval = 0;
               switch (template_type) {
               case TEMPL_11_18:
                  GET_EXTRA_CODE_PACKED_18(seqptr, bitval, tmpval, extra_code);
                  ecode1 = (GET_WORD_INDEX_11_18(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_12_18:
                  GET_EXTRA_CODE_PACKED_18(seqptr, bitval, tmpval, extra_code);
                  ecode1 = (GET_WORD_INDEX_12_18(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_11_18_OPT:
                  GET_EXTRA_CODE_PACKED_18_OPT(seqptr, bitval, tmpval, extra_code);
                  ecode1 = (GET_WORD_INDEX_11_18_OPT(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_12_18_OPT:
                  GET_EXTRA_CODE_PACKED_18_OPT(seqptr, bitval, tmpval, extra_code);
                  ecode1 = (GET_WORD_INDEX_12_18_OPT(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_11_21:
                  GET_EXTRA_CODE_PACKED_21(seqptr, bitval, tmpval, extra_code);
                  ecode1 = (GET_WORD_INDEX_11_21(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_12_21:
                  GET_EXTRA_CODE_PACKED_21(seqptr, bitval, tmpval, extra_code);
                  ecode1 = (GET_WORD_INDEX_12_21(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_11_21_OPT:
                  GET_EXTRA_CODE_PACKED_21_OPT(seqptr, bitval, tmpval, extra_code);
                  ecode1 = (GET_WORD_INDEX_11_21_OPT(ecode) | extra_code) & no_bit0;
                  break;
               case TEMPL_12_21_OPT:
                  GET_EXTRA_CODE_PACKED_21_OPT(seqptr, bitval, tmpval, extra_code);
                  ecode1 = (GET_WORD_INDEX_12_21_OPT(ecode) | extra_code) & no_bit0;
                  break;
               default:
                  extra_code = 0; ecode1 = 0; break;
               }
#ifdef USE_HASH_TABLE
               hash_buf = (Uint1Ptr)&ecode1;
               CRC32(crc, hash_buf);
               ecode1 = (crc>>hash_shift) & hash_mask;
#endif
               if (use_two_templates) {
                  seqptr = subject;
                  bitval = bit;
                  extra_code = 0;
                  tmpval = 0;
                  switch (template_type) {
                  case TEMPL_11_18:
                     GET_EXTRA_CODE_PACKED_18_OPT(seqptr, bitval, tmpval, extra_code);
                     ecode2 = GET_WORD_INDEX_11_18_OPT(ecode) | extra_code | bit0;
                     break;
                  case TEMPL_12_18:
                     GET_EXTRA_CODE_PACKED_18_OPT(seqptr, bitval, tmpval, extra_code);
                     ecode2 = GET_WORD_INDEX_12_18_OPT(ecode) | extra_code | bit0;
                     break;
                  case TEMPL_11_21:
                     GET_EXTRA_CODE_PACKED_21_OPT(seqptr, bitval, tmpval, extra_code);
                     ecode2 = GET_WORD_INDEX_11_21_OPT(ecode) | extra_code | bit0;
                     break;
                  case TEMPL_12_21:
                     GET_EXTRA_CODE_PACKED_21_OPT(seqptr, bitval, tmpval, extra_code);
                     ecode2 = GET_WORD_INDEX_12_21_OPT(ecode) | extra_code | bit0;
                     break;
                  default: 
                     ecode2 = 0; break;
                  }
#ifdef USE_HASH_TABLE
                  hash_buf = (Uint1Ptr)&ecode2;
                  CRC32(crc, hash_buf);
                  ecode2 = (crc>>hash_shift) & hash_mask;
#endif
               }
               bit = (bit - 1) & mask2b;
               s_off++;
            } 
            if (s_off > subj_length)
               break;
         }
         for (q_off = lookup->mb_lt->hashtable[ecode1]; q_off>0;
              q_off = lookup->mb_lt->next_pos[q_off]) {
            search->second_pass_hits++;
            MegaBlastExtendHit(search, lookup, s_off, q_off);
         }
         if (use_two_templates) {
            for (q_off = lookup->mb_lt->hashtable2[ecode2]; q_off>0;
                 q_off = lookup->mb_lt->next_pos2[q_off]) {
               search->second_pass_hits++;
               MegaBlastExtendHit(search, lookup, s_off, q_off);
            }
         }
         s_off++;
      }
   }
   if (!lookup->mb_lt->estack)
      BlastExtendWordExit(search);

   search->current_hitlist->do_not_reallocate = FALSE;

   /* Do greedy gapped extension */
   if (search->current_hitlist->hspcnt > 0)
      if (MegaBlastGappedAlign(search) != 0)
         return -1;

   if (search->current_hitlist)
      return search->current_hitlist->hspcnt;
   else return 0;
}

/* Contiguous words, database scanned 4 bases at a time */

Int4 
MegaBlastWordFinder(BlastSearchBlkPtr search, LookupTablePtr lookup)
{
   register Uint1Ptr subject;
   register Int4 s_off, ecode, mask, q_off;
   Int4 subj_length = search->subject->length;
   Int4 min_hit_size;
   PV_ARRAY_TYPE *pv_array = lookup->pv_array;
   Int4 pv_array_bts;

   if (search->pbp->mb_params->disc_word) {
      if (search->pbp->mb_params->one_base_step)
         return MegaBlastWordFinder_disc_1b(search, lookup);
      else
         return MegaBlastWordFinder_disc(search, lookup);
   }

   min_hit_size = lookup->mb_lt->lpm;
   if (search->pbp->window_size > 0)
      min_hit_size += 4;

   if (search->pbp->mb_params->is_neighboring && 
       subj_length < MIN_NEIGHBOR_HSP_LENGTH)
      return 0;
   if (search->current_hitlist == NULL)
      search->current_hitlist = BlastHitListNew(search);
   
   mask = lookup->mb_lt->mask;
   subject = search->subject->sequence - 1;
   ecode = 0;
   if (lookup->mb_lt->estack)
      MemSet(lookup->mb_lt->stack_index, 0, 
             lookup->mb_lt->num_stacks*sizeof(Int4));

   pv_array_bts = PV_ARRAY_BTS + ((lookup->mb_lt->width < 3) ? 0 : 5);

   for (s_off = 0; s_off < (lookup->mb_lt->width - 1)*4; s_off += 4) {
      ecode = (ecode << 8) + *++subject;
   }

   s_off += 4;
   while (s_off<=subj_length) {
      if (pv_array) {
         ecode = ((ecode & mask) << 8) + *++subject;
         while ((s_off < subj_length) && ((pv_array[ecode>>pv_array_bts]&
                 (((PV_ARRAY_TYPE) 1)<<(ecode&PV_ARRAY_MASK))) == 0)) {
            ecode = ((ecode & mask) << 8) + *++subject;
            s_off += 4;
         } 
         if (s_off > subj_length)
            break;
      } else {
         ecode = ((ecode & mask) << 8) + *++subject;
      }
      for (q_off = lookup->mb_lt->hashtable[ecode]; q_off>0; 
	   q_off = lookup->mb_lt->next_pos[q_off]) {
         search->second_pass_hits++;
	 MegaBlastExtendHit(search, lookup, s_off, q_off);
      }
      s_off += 4;
   } 

   if (!lookup->mb_lt->estack)
      BlastExtendWordExit(search);

   search->current_hitlist->do_not_reallocate = FALSE;
   /* Do greedy gapped extension */
   if (search->current_hitlist->hspcnt > 0)
      if (MegaBlastGappedAlign(search) != 0)
         return -1;

   if (search->current_hitlist)
      return search->current_hitlist->hspcnt;
   else return 0;
}

static int LIBCALLBACK
diag_compare_match(VoidPtr v1, VoidPtr v2)
{
   MegaBlastExactMatchPtr h1, h2;

   h1 = *((MegaBlastExactMatchPtr PNTR) v1);
   h2 = *((MegaBlastExactMatchPtr PNTR) v2);
   
   return (h1->q_off - h1->s_off) - (h2->q_off - h2->s_off);
}

#define MB_MAX_LENGTH_TO_UNPACK 400
Int2 MegaBlastGappedAlign(BlastSearchBlkPtr search)
{
   MegaBlastExactMatchPtr PNTR e_hsp_array;
   MegaBlastExactMatchPtr e_hsp;
   Int4 index, i, hspcnt, buf_len=0;
   Uint1Ptr subject0 = NULL;
   Boolean delete_hsp;
   GapAlignBlkPtr gap_align;
   Boolean use_dyn_prog = search->pbp->mb_params->use_dyn_prog;

   hspcnt = search->current_hitlist->hspcnt;
   
   if (hspcnt == 0) 
     return 0;

   /* Make current hitlist available for rewriting of extended hsps 
      without freeing the hsp_array since it's used in this function */
   search->current_hitlist->hspcnt = 0;

   e_hsp_array = (MegaBlastExactMatchPtr PNTR) 
      Malloc(hspcnt*sizeof(MegaBlastExactMatchPtr));

   if (e_hsp_array == NULL)
      return 1;

   for (index=0; index < hspcnt; index++) {
      e_hsp_array[index] = 
         &(search->current_hitlist->exact_match_array[index]);
   }

   if (use_dyn_prog || (search->rdfp && search->subject->length > MB_MAX_LENGTH_TO_UNPACK)) {
      subject0 = search->subject->sequence;
   } else {
      /* Get subject sequence in Seq_code_ncbi4na format with all 
         ambiguities information.
         If database sequence is split, do it only once and save in
         search->subject->sequence_start.
         For the 2 sequences engine this is done beforehand.
      */
      if (search->subject->sequence_start == NULL && search->rdfp) {
         readdb_get_sequence_ex(search->rdfp, search->subject_id, 
                                &search->subject->sequence_start, 
                                &buf_len, TRUE);
      }
      subject0 = 
         (&search->subject->sequence_start[search->subject->original_length])
         + 1;
   }

   HeapSort(e_hsp_array, hspcnt, sizeof(MegaBlastExactMatchPtr), diag_compare_match);
   if (use_dyn_prog) {
      if (search->gap_align == NULL) {
         search->gap_align = GapAlignBlkNew(1, 1);
         search->gap_align->dyn_prog = (GapXDPPtr)
            Malloc((search->context[search->first_context].query->length + 2)
                   *sizeof(GapXDP));
      }
      gap_align = search->gap_align;
      gap_align->gap_open = search->pbp->gap_open;
      gap_align->gap_extend = search->pbp->gap_extend;
      gap_align->x_parameter = search->pbp->gap_x_dropoff;
      gap_align->matrix = search->sbp->matrix;
      gap_align->query =
         search->context[search->first_context].query->sequence;
      gap_align->subject = subject0;
      /* Subtract 1 because the concatenated length includes the last 
         sentinel byte */
      gap_align->query_length = 
         search->context[search->first_context].query->length - 1;
      gap_align->subject_length = search->subject->length;
   }

   for (index=0; index<hspcnt; index++) {
      e_hsp = e_hsp_array[index];
      delete_hsp = FALSE;
      for (i = search->current_hitlist->hspcnt-1; 
	   i >= 0 && MB_HSP_CLOSE(e_hsp->q_off, search->current_hitlist->hsp_array[i]->query.offset, e_hsp->s_off, search->current_hitlist->hsp_array[i]->subject.offset, MB_DIAG_NEAR);
	   i--) {
	if (MB_HSP_CONTAINED(e_hsp->q_off, search->current_hitlist->hsp_array[i]->query.offset, search->current_hitlist->hsp_array[i]->query.end, e_hsp->s_off, search->current_hitlist->hsp_array[i]->subject.offset, search->current_hitlist->hsp_array[i]->subject.end, MB_DIAG_CLOSE)) {
	  delete_hsp = TRUE;
	  break;
	}
      }
      if (!delete_hsp) {
         if ((search->pbp->mb_params->disc_word && 
             (search->pbp->window_size == 0))) {
            delete_hsp = BlastNtWordUngappedExtend(search, e_hsp->q_off, 
                              e_hsp->s_off, search->pbp->cutoff_s2);
         }
         if (!delete_hsp) {
            search->second_pass_extends++;

            if (use_dyn_prog) {
               Uint1 rem = e_hsp->s_off & 0x03;
               gap_align->q_start = e_hsp->q_off - 1 - rem;
               gap_align->s_start = e_hsp->s_off - 1 - rem;
               gap_align->decline_align = INT2_MAX;
               if (!PerformNtGappedAlignment(gap_align))
                  return 1;
               if (gap_align->score >= search->pbp->cutoff_s2) {
                  BlastNtSaveCurrentHspGapped(search, gap_align->score, 
                      gap_align->query_start, gap_align->subject_start, 
                      gap_align->query_stop-gap_align->query_start+1, 
                      gap_align->subject_stop-gap_align->subject_start+1,
                                              e_hsp->q_off, e_hsp->s_off, search->first_context, NULL); 
               }
            } else 
               MegaBlastNtWordExtend(search, subject0, e_hsp->q_off,
                                     e_hsp->s_off);
         }
      }
   }

   MemFree(e_hsp_array);
   return 0;
}

Int4
MegaBlastExtendHit(BlastSearchBlkPtr search, LookupTablePtr lookup, 
		   Int4 s_off, Int4 q_off) {
   Int4 index, len = 4*lookup->mb_lt->width; 
   Int4 index1, step;
   Int2 min_hit_length;
   MbStackPtr estack;
   Int4 diag, stack_top;
   BLAST_ExtendWordParamsPtr ewp_params = search->ewp_params;
   register Int4 window;
   Int4 step_unit;
   Boolean one_word, hit_ready, two_hits;

   window = search->pbp->window_size + len;
   two_hits = (search->pbp->window_size > 0);

   if (search->pbp->mb_params->one_base_step)
      step_unit = 1;
   else
      step_unit = 4;

   min_hit_length = lookup->mb_lt->lpm;

   /* For short query/db stacks are not used; 
      instead use a diagonal array */
   if (!lookup->mb_lt->estack) {
      CfjModStruct *combo_array =
         search->context[search->first_context].ewp->combo_array;
      CfjModStruct *combo_array_elem;
      register Int4 min_diag_mask;
      Int4 offset, s_pos;

      min_diag_mask = ewp_params->min_diag_mask;
      offset = ewp_params->offset;

      s_pos = s_off + offset;
      diag = (s_off - q_off) & min_diag_mask;
      combo_array_elem = &combo_array[diag];
      step = s_pos - combo_array_elem->last_hit;

      one_word = (combo_array_elem->diag_level >= min_hit_length);
      hit_ready = ((!two_hits && one_word) || 
                    combo_array_elem->diag_level > min_hit_length);

      if (two_hits && step < mb_two_hit_min_step && step > step_unit
          && one_word) {
         /* Words too close are not allowed */
         return 0;
      } else if (step == step_unit || 
               (step > step_unit && step <= window 
                && one_word && !hit_ready)) {
         /* We had a hit on this diagonal that is extended by current hit */
         combo_array_elem->last_hit = s_pos;
         combo_array_elem->diag_level += step;
         /* Save the hit immediately if:
               For single-hit case - it reached the word size
               For double-hit case - hit is longer than word size, or 
               2 words have already been found, and there is another gap. 
            If hit is longer than the required minimum, it must have already 
            been saved earlier.
         */                  
         if (!hit_ready && ((combo_array_elem->diag_level > min_hit_length) ||
              (!two_hits && combo_array_elem->diag_level == min_hit_length)))
         {
            MegaBlastSaveExactMatch(search, q_off, s_off);
         }
      } else {
         /* Start the new hit, overwriting the diagonal array entry */
         combo_array_elem->last_hit = s_pos;
         combo_array_elem->diag_level = len;
         /* Save the hit if it already qualifies */
         if (!two_hits && (len >= min_hit_length))
            MegaBlastSaveExactMatch(search, q_off, s_off);
      }
      return 0;
   }

   index1 = (s_off - q_off) % lookup->mb_lt->num_stacks;
   if (index1<0)
      index1 += lookup->mb_lt->num_stacks;
   estack = lookup->mb_lt->estack[index1];
   stack_top = lookup->mb_lt->stack_index[index1] - 1;

   for (index=0; index<=stack_top; ) {
      step = s_off - estack[index].level;
      one_word = (estack[index].length >= min_hit_length);
      if (estack[index].diag == s_off - q_off) {
         hit_ready = ((!two_hits && one_word) || 
                              estack[index].length > min_hit_length);
         if (two_hits && step < mb_two_hit_min_step && step > step_unit
             && one_word) {
            lookup->mb_lt->stack_index[index1] = stack_top + 1;
            /* Skip words that are too close to each other (experimental) */
            return 0;
         } else if (step == step_unit || 
                  (step > step_unit && step <= window && 
                   one_word && !hit_ready)) {
            estack[index].length += step;
            /* We had a hit on this diagonal that is extended by current hit */
            /* Save the hit if:
               For single-hit case - it has just reached the word size
               For double-hit case - second hit has just been found, 
                  after the first reached the word size.
               Otherwise, just update the hit length and offset.
            */                  
            if (!hit_ready && (estack[index].length > min_hit_length || 
                (!two_hits && estack[index].length == min_hit_length))) {
               MegaBlastSaveExactMatch(search, q_off, s_off);
            }    
            estack[index].level = s_off;
         } else {
            /* Start the new hit */            
            estack[index].length = len;
            estack[index].level = s_off;
            if (!two_hits && (len >= min_hit_length))
               MegaBlastSaveExactMatch(search, q_off, s_off);
         }
         lookup->mb_lt->stack_index[index1] = stack_top + 1;
         return 0;
      } else if (step <= step_unit || (step <= window && one_word)) {
         /* Hit from a different diagonal, and it can potentially continue */
         index++;
      } else {
         /* Hit from a different diagonal that does not continue: remove it 
            from the stack */
         estack[index] = estack[stack_top--];
      }
   }

   /* Need an extra slot on the stack for this hit */
   if (++stack_top>=lookup->mb_lt->stack_size[index1]) {
      /* Stack about to overflow - reallocate memory */
      MbStackPtr ptr;
      if (!(ptr = (MbStackPtr)Realloc(estack,
                                      2*lookup->mb_lt->stack_size[index1]*sizeof(MbStack)))) {
         ErrPostEx(SEV_WARNING, 0, 0, "Unable to allocate %ld extra spaces for stack %d",
                   2*lookup->mb_lt->stack_size[index1], index1);
         return 1;
      } else {
         lookup->mb_lt->stack_size[index1] *= 2;
         estack = lookup->mb_lt->estack[index1] = ptr;
      }
   }
   estack[stack_top].diag = s_off - q_off;
   estack[stack_top].level = s_off;
   estack[stack_top].length = len;
   lookup->mb_lt->stack_index[index1] = stack_top + 1;
   if (!two_hits && (len >= min_hit_length))
      MegaBlastSaveExactMatch(search, q_off, s_off);
      
   return 0;
}
 
/* subject sequence is compressed 4:1 */
Int2
MegaBlastNtWordExtend(BlastSearchBlkPtr search, Uint1Ptr subject0, 
		      Int4 q_off, Int4 s_off)
{
	register Uint1Ptr	q;
	register BLAST_Score    score;
	Uint1Ptr query0, s;
	BLAST_Score	X;
        BLAST_ParameterBlkPtr   pbp;
        BLAST_ScoreBlkPtr       sbp;
	Int4 q_avail, s_avail;
	/* Variables for the call to MegaBlastGreedyAlign */
	Int4 q_ext_l, q_ext_r, s_ext_l, s_ext_r;
	edit_script_t *ed_script_fwd=NULL, *ed_script_rev=NULL;
	Boolean good_hit;
        Uint1 rem;
        GapXEditScriptPtr esp = NULL;
	
	good_hit = TRUE;
        sbp=search->sbp;
        pbp=search->pbp;

	query0 = (Uint1Ptr) search->context[search->first_context].query->sequence;
        q_avail = search->context[search->first_context].query->length - q_off;
        s_avail = search->subject->length - s_off;

	q = query0 + q_off;
        if (!search->rdfp || search->subject->length <= MB_MAX_LENGTH_TO_UNPACK) {
           s = subject0 + s_off;
           rem = 4;
        } else {
           s = subject0 + s_off/4;
           rem = s_off % 4;
        }

	/* Find where positive scoring starts & ends within the word hit */
	score = 0;

	X = pbp->gap_x_dropoff;

	if (!search->pbp->mb_params->no_traceback) {
	   ed_script_fwd = edit_script_new();
	   ed_script_rev = edit_script_new();
	}

	/* extend to the right */
	score = MegaBlastAffineGreedyAlign(s, s_avail, q, q_avail, FALSE, X,
					   sbp->reward, -sbp->penalty,
					   pbp->gap_open, pbp->gap_extend,
					   &s_ext_r, &q_ext_r, search->abmp, 
					   ed_script_fwd, rem);
	if (search->rdfp && search->subject->length > MB_MAX_LENGTH_TO_UNPACK)
           rem = 0;
	/* extend to the left */
	score += MegaBlastAffineGreedyAlign(subject0, s_off, query0, q_off, 
					    TRUE, X, sbp->reward, 
					    -sbp->penalty, pbp->gap_open, 
					    pbp->gap_extend, &s_ext_l, 
					    &q_ext_l, search->abmp, 
					    ed_script_rev, rem);
	/* For neighboring we have a stricter criterion to keep an HSP */
	if (search->pbp->mb_params->is_neighboring) {
	   FloatHi perc_identity;
	   Int4 hsp_length;
	   
	   hsp_length = MIN(q_ext_l+q_ext_r, s_ext_l+s_ext_r);
	   perc_identity = 100 * (1 - ((FloatHi) score) / hsp_length);
	   if (hsp_length < MIN_NEIGHBOR_HSP_LENGTH || 
	       perc_identity < MIN_NEIGHBOR_PERC_IDENTITY)
	      good_hit = FALSE;
	}

	/* In basic case the greedy algorithm returns number of 
	   differences, hence we need to convert it to score */
	if (pbp->gap_open==0 && pbp->gap_extend==0) 
	   score = 
	      (q_ext_r + s_ext_r + q_ext_l + s_ext_l)*sbp->reward/2 - 
	      score*(sbp->reward - sbp->penalty);
	else if (sbp->reward % 2 == 1)
	   score /= 2;

	if (good_hit && score >= pbp->cutoff_s2) { /* Score is reportable */
	   if (!search->pbp->mb_params->no_traceback) {
	      edit_script_append(ed_script_rev, ed_script_fwd);
              esp = MBToGapXEditScript(ed_script_rev);
	   }

	   search->second_pass_good_extends++;
           
	   BlastNtSaveCurrentHspGapped(search, score, q_off-q_ext_l,
				     s_off-s_ext_l, q_ext_l+q_ext_r,
				     s_ext_l+s_ext_r, q_off, s_off,
                                     search->first_context, esp); 
	}
        edit_script_free(ed_script_fwd);
        edit_script_free(ed_script_rev);

	return 0;
}

#define NO_UNPACK_MBALIGN_ARG 4
void PerformGreedyAlignmentWithTraceback(GapAlignBlkPtr gap_align,
        GreedyAlignMemPtr gamp, BLAST_ScoreBlkPtr sbp)
{
    Int4 score, s_ext_r, q_ext_r, s_ext_l, q_ext_l;
    Uint1Ptr query0, subject0, query, subject;
    Int4 X = gap_align->x_parameter;
    Int4 reward = sbp->reward;
    Int4 penalty = -sbp->penalty;
    Int4 q_off, s_off, q_avail, s_avail;
    edit_script_t *ed_script_fwd=NULL, *ed_script_rev=NULL;
    GapXEditScriptPtr esp = NULL;

    query0 = gap_align->query;
    subject0 = gap_align->subject;
    q_off = gap_align->q_start;
    s_off = gap_align->s_start;
    query = query0 + q_off;
    subject = subject0 + s_off;
    q_avail = gap_align->query_length - q_off;
    s_avail = gap_align->subject_length - s_off;
    ed_script_fwd = edit_script_new();
    ed_script_rev = edit_script_new();

    /* extend to the right */
    score = MegaBlastAffineGreedyAlign(subject, s_avail, query, q_avail, 
               FALSE, X, reward, penalty, gap_align->gap_open, 
               gap_align->gap_extend, &s_ext_r, &q_ext_r, gamp, 
               ed_script_fwd, NO_UNPACK_MBALIGN_ARG);
    /* extend to the left */
    score += MegaBlastAffineGreedyAlign(subject0, s_off, query0, q_off, 
                TRUE, X, reward, penalty, gap_align->gap_open, 
                gap_align->gap_extend, &s_ext_l, &q_ext_l, gamp, 
                ed_script_rev, NO_UNPACK_MBALIGN_ARG);

    /* In basic case the greedy algorithm returns number of 
       differences, hence we need to convert it to score */
    if (gap_align->gap_open==0 && gap_align->gap_extend==0) 
       score = 
          (q_ext_r + s_ext_r + q_ext_l + s_ext_l)*sbp->reward/2 - 
          score*(sbp->reward - sbp->penalty);
    else if (sbp->reward % 2 == 1)
       score /= 2;
    
    edit_script_append(ed_script_rev, ed_script_fwd);
    esp = MBToGapXEditScript(ed_script_rev);

    edit_script_free(ed_script_fwd);
    edit_script_free(ed_script_rev);

    gap_align->score = score;
    gap_align->query_start = q_off - q_ext_l;
    gap_align->subject_start = s_off - s_ext_l;
    gap_align->query_stop = q_off + q_ext_r;
    gap_align->subject_stop = s_off + s_ext_r;
    gap_align->edit_block = GapXEditBlockNew(gap_align->query_start, gap_align->subject_start);

    gap_align->edit_block->start1 = gap_align->query_start;
    gap_align->edit_block->start2 = gap_align->subject_start;
    gap_align->edit_block->length1 = gap_align->query_length;
    gap_align->edit_block->length2 = gap_align->subject_length;
    gap_align->edit_block->frame1 = gap_align->query_frame;
    gap_align->edit_block->frame2 = gap_align->subject_frame;
    gap_align->edit_block->reverse = 0;
    
    gap_align->edit_block->esp = esp;
}

/* A routine to mask the residues from a mask SeqLoc by changing to a given 
 * value (mask_residue).
 * The buffer should contain an already decoded sequence. 
 * 
 */
void MegaBlastMaskTheResidues(Uint1Ptr buffer, Int4 max_length, Uint1
			      mask_residue, SeqLocPtr mask_slp, Boolean reverse,
			      Int4 offset, Boolean mask_at_hash)

{
   SeqLocPtr slp=NULL;
   Int4 index, start, stop;
   
   while (mask_slp) {
      slp=NULL;
      while((slp = SeqLocFindNext(mask_slp, slp))!=NULL) {
	 if (reverse) {
	    start = MAX(max_length + offset - 1 - SeqLocStop(slp), 0);
	    stop = max_length - 1 - MAX(SeqLocStart(slp) - offset, 0);
	 } else {
	    start = MAX(SeqLocStart(slp) - offset, 0);
	    stop = MIN(SeqLocStop(slp) - offset, max_length - 1);
	 }
	 
	 if (mask_at_hash) 
	    for (index=start; index<=stop; index++)
	       /* Set the 5th bit to mark that this is a masked residue */
	       buffer[index] = buffer[index] | 0x10;
	 else
	    for (index=start; index<=stop; index++)
	       buffer[index] = mask_residue;
      }
      mask_slp = mask_slp->next;
   }
}

/* The following function can only be used for IUPAC encoding */
void
BlastLCaseMaskTheResidues(Uint1Ptr buffer, Int4 max_length, SeqLocPtr mask_slp, Boolean reverse, Int4 offset)

{
   SeqLocPtr slp=NULL;
   Int4 index, start, stop;
   
   while (mask_slp) {
      slp=NULL;
      while((slp = SeqLocFindNext(mask_slp, slp))!=NULL) {
	 if (reverse) {
	    start = max_length - 1 - SeqLocStop(slp);
	    stop = max_length - 1 - SeqLocStart(slp);
	 }	else {
	    start = SeqLocStart(slp);
	    stop = SeqLocStop(slp);
	 }
	 
	 start -= offset;
	 stop  -= offset;
	 
	 for (index=start; index<=stop; index++)
	    buffer[index] = tolower(buffer[index]);
      }
      mask_slp = mask_slp->next;
   }
}

static int LIBCALLBACK
diag_compare_hsps(VoidPtr v1, VoidPtr v2)

{
   BLAST_HSPPtr h1, h2;
   BLAST_HSPPtr PNTR hp1, PNTR hp2;
   
   hp1 = (BLAST_HSPPtr PNTR) v1;
   hp2 = (BLAST_HSPPtr PNTR) v2;
   h1 = *hp1;
   h2 = *hp2;
   
   if (h1==NULL && h2==NULL) return 0;
   else if (h1==NULL) return 1;
   else if (h2==NULL) return -1;
   
   /* Separate different queries and/or strands */
   if (h1->context < h2->context)
      return -1;
   else if (h1->context > h2->context)
      return 1;
   
   return (h1->query.offset - h1->subject.offset) -
      (h2->query.offset - h2->subject.offset);
}

typedef enum E_HSPInclusionStatus {
   e_Equal = 0,      /**< Identical */
   e_FirstInSecond,  /**< First included in rectangle formed by second */
   e_SecondInFirst,  /**< Second included in rectangle formed by first */
   e_DiagNear,       /**< Diagonals are near, but neither HSP is included in
                       the other. */
   e_DiagDistant     /**< Diagonals are far apart, or different contexts */
} E_HSPInclusionStatus;

/** HSP inclusion criterion for megablast: one HSP must be included in a
 * diagonal strip of a certain width around the other, and also in a rectangle
 * formed by the other HSP's endpoints.
 */
static E_HSPInclusionStatus 
BLAST_HSPInclusionTest(BLAST_HSP* hsp1, BLAST_HSP* hsp2)
{
   if (hsp1->context != hsp2->context || 
       !MB_HSP_CLOSE(hsp1->query.offset, hsp2->query.offset,
                     hsp1->subject.offset, hsp2->subject.offset, 
                     2*MB_DIAG_NEAR))
      return e_DiagDistant;

   if (hsp1->query.offset == hsp2->query.offset && 
       hsp1->query.end == hsp2->query.end &&  
       hsp1->subject.offset == hsp2->subject.offset && 
       hsp1->subject.end == hsp2->subject.end && 
       hsp1->score == hsp2->score) {
      return e_Equal;
   } else if (hsp1->query.offset >= hsp2->query.offset && 
       hsp1->query.end <= hsp2->query.end &&  
       hsp1->subject.offset >= hsp2->subject.offset && 
       hsp1->subject.end <= hsp2->subject.end && 
       hsp1->score < hsp2->score) { 
      return e_FirstInSecond;
   } else if (hsp1->query.offset <= hsp2->query.offset &&  
              hsp1->query.end >= hsp2->query.end &&  
              hsp1->subject.offset <= hsp2->subject.offset && 
              hsp1->subject.end >= hsp2->subject.end && 
              hsp1->score >= hsp2->score) { 
      return e_SecondInFirst;
   }
   return e_DiagNear;
}

/** How many HSPs to check for inclusion for each new HSP? */
#define MAX_NUM_CHECK_INCLUSION 20

static void
BlastSortUniqHspArray(BLAST_HitListPtr hitlist)
{
   Int4 index, new_hspcnt, index1, index2;
   BLAST_HSPPtr PNTR hsp_array = hitlist->hsp_array;
   Boolean shift_needed = FALSE;
   E_HSPInclusionStatus inclusion_status = e_DiagNear;

   HeapSort(hitlist->hsp_array, hitlist->hspcnt, sizeof(BLAST_HSPPtr), 
            diag_compare_hsps);

   for (index=1, new_hspcnt=0; index<hitlist->hspcnt; index++) {
      if (hsp_array[index]==NULL) 
	 continue;
      inclusion_status = e_DiagNear;
      for (index1 = new_hspcnt; inclusion_status != e_DiagDistant &&
           index1 >= 0 && new_hspcnt-index1 < MAX_NUM_CHECK_INCLUSION;
           index1--) {
         inclusion_status = 
            BLAST_HSPInclusionTest(hsp_array[index], hsp_array[index1]);
         if (inclusion_status == e_FirstInSecond || 
             inclusion_status == e_Equal) {
            /* Free the new HSP and break out of the inclusion test loop */
            hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
            break;
         } else if (inclusion_status == e_SecondInFirst) {
            hsp_array[index1] = BLAST_HSPFree(hsp_array[index1]);
            shift_needed = TRUE;
         }
      }
      
      /* If some lower indexed HSPs have been removed, shift the subsequent 
         HSPs */
      if (shift_needed) {
         /* Find the first non-NULL HSP, going backwards */
         while (index1 >= 0 && !hsp_array[index1])
            index1--;
         /* Go forward, and shift any non-NULL HSPs */
         for (index2 = ++index1; index1 <= new_hspcnt; index1++) {
            if (hsp_array[index1])
               hsp_array[index2++] = hsp_array[index1];
         }
         new_hspcnt = index2 - 1;
         shift_needed = FALSE;
      }
      if (hsp_array[index] != NULL)
         hsp_array[++new_hspcnt] = hsp_array[index];
   }
   /* Set all HSP pointers that will not be used any more to NULL */
   MemSet(&hsp_array[new_hspcnt+1], 0, 
	  (hitlist->hspcnt - new_hspcnt - 1)*sizeof(BLAST_HSPPtr));
   hitlist->hspcnt = new_hspcnt + 1;
   hitlist->hspcnt_max = hitlist->hspcnt;
}

void
MegaBlastFillHspGapInfo(BLAST_HSPPtr hsp, GapXEditScriptPtr esp)
{
   hsp->gap_info = GapXEditBlockNew(hsp->query.offset, hsp->subject.offset);

   hsp->gap_info->start1 = hsp->query.offset;
   hsp->gap_info->start2 = hsp->subject.offset;
   hsp->gap_info->length1 = hsp->query.length;
   hsp->gap_info->length2 = hsp->subject.length;
   hsp->gap_info->frame1 = hsp->gap_info->frame2 = 1;
   hsp->gap_info->reverse = 0;
   
   hsp->gap_info->esp = esp;
}

GapXEditScriptPtr
MBToGapXEditScript (edit_script_t PNTR ed_script)
{
   GapXEditScriptPtr esp_start, esp, esp_prev;
   Int4 i;

   if (ed_script==NULL || ed_script->num == 0)
      return NULL;

   for (i=0; i<ed_script->num; i++) {
      esp = (GapXEditScriptPtr) MemNew(sizeof(GapXEditScript));
      esp->num = EDIT_VAL(ed_script->op[i]); 
      esp->op_type = 3 - EDIT_OPC(ed_script->op[i]);
      if (esp->op_type == 3)
         fprintf(stderr, "op_type = 3\n");
      if (i==0) 
	 esp_start = esp_prev = esp;
      else {
	 esp_prev->next = esp;
	 esp_prev = esp;
      }
   }

   return esp_start;

}


SeqAlignPtr
MegaBlastGapInfoToSeqAlign(BlastSearchBlkPtr search, Int4 subject_index,
                           Int2 query_index)
{
   SeqAlignPtr seqalign, seqalign_var, sap;
   BLASTResultHitlistPtr result_hitlist;
   SeqIdPtr subject_id;
   BLASTResultHspPtr hsp_array;
   Int4 index;
   SeqIdPtr query_id, new_subject_seqid;
   StdSegPtr ssp;
   ValNodePtr gi_list=NULL;

   /* Sanity check */
   if (query_index > search->last_context / 2 ||
       search->mb_result_struct[query_index] == NULL || 
       subject_index >= search->mb_result_struct[query_index]->hitlist_count)
      return NULL;

   result_hitlist = 
      search->mb_result_struct[query_index]->results[subject_index];

   if (search->rdfp) {
      readdb_get_descriptor(search->rdfp,
                            result_hitlist->subject_id,
                            &subject_id, NULL); 
   } else if (result_hitlist->subject_info && 
              result_hitlist->subject_info->sip) {
      subject_id = 
         SeqIdDup(result_hitlist->subject_info->sip);
   }

   hsp_array = result_hitlist->hsp_array;

   gi_list = BlastGetAllowedGis(search, result_hitlist->subject_id, &new_subject_seqid);
   
   for (index=0; index<result_hitlist->hspcnt; index++) { 
      query_id = search->qid_array[hsp_array[index].context/2];
      if (index==0) {
	 if (new_subject_seqid)
	    seqalign = seqalign_var =
	       GapXEditBlockToSeqAlign(hsp_array[index].gap_info, 
				       new_subject_seqid, query_id); 
	 else
	    seqalign = seqalign_var =
	       GapXEditBlockToSeqAlign(hsp_array[index].gap_info, 
				       subject_id, query_id);
      } else {
	 if (new_subject_seqid)
	    seqalign_var->next = 
	       GapXEditBlockToSeqAlign(hsp_array[index].gap_info, 
				       new_subject_seqid, query_id); 
	 else
	    seqalign_var->next = 
	       GapXEditBlockToSeqAlign(hsp_array[index].gap_info, 
				       subject_id, query_id); 
	    
	 seqalign_var = seqalign_var->next;
      }
      seqalign_var->score = GetScoreSetFromBlastResultHsp(&hsp_array[index], gi_list);
      if (seqalign_var->segtype == 3) {
	 ssp = seqalign_var->segs;
	 while (ssp) {
	    ssp->scores = GetScoreSetFromBlastResultHsp(&hsp_array[index], gi_list);
	    ssp = ssp->next;
	 }
      } else if (seqalign_var->segtype == 5) { /* Discontinuous */
	 sap = (SeqAlignPtr) seqalign_var->segs;
	 for(;sap != NULL; sap = sap->next) {
	    sap->score = GetScoreSetFromBlastResultHsp(&hsp_array[index], gi_list);
	 }
      }
   }

   SeqIdSetFree(subject_id);
   return seqalign;
}

Int4
MegaBlastGetNumIdentical(Uint1Ptr query, Uint1Ptr subject, Int4 q_start, 
                         Int4 s_start, Int4 length, Boolean reverse)
{
   Int4 i, ident = 0;
   Uint1Ptr q, s;
   Uint1 at_hash_mask = 0x0f;

   q = &query[q_start];
   if (!reverse) 
      s = &subject[s_start];
   else
      s = &subject[s_start + length - 1];

   for (i=0; i<length; i++) {
      if ((*q & at_hash_mask) == *s)
	 ident++;
      q++;
      if (!reverse)
         s++;
      else
         s--;
   }

   return ident;
}

/* Reevaluate score of one HSP, using ambiguity information */
Boolean ReevaluateScoreWithAmbiguities(BlastSearchBlkPtr search,
           Uint1Ptr subject_start, BLAST_HSPPtr hsp)
{
   BLAST_Score	sum, score, gap_open, gap_extend;
   BLAST_ScorePtr PNTR    matrix;
   Uint1Ptr query_start, query, subject;
   Uint1Ptr new_q_start, new_s_start, new_q_end, new_s_end;
   Int4 i, context, last_esp_num;
   Int2 factor;
   Uint1 mask = 0x0f;
   Boolean delete_hsp;
   GapXEditBlockPtr gap_info = hsp->gap_info;
   GapXEditScriptPtr esp, last_esp, prev_esp, first_esp;
   FloatHi searchsp_eff;
   Int4 align_length;

   if (search->sbp->reward % 2 == 1) 
      factor = 2;
   else 
      factor = 1;

   if (search->pbp->gap_open == 0 && search->pbp->gap_extend == 0) {
      gap_open = 0;
      gap_extend = 
         (search->sbp->reward - 2*search->sbp->penalty) * factor / 2;
   } else {
      gap_open = factor*search->pbp->gap_open;
      gap_extend = factor*search->pbp->gap_extend;
   }

   matrix = search->sbp->matrix;

   context = hsp->context;

   searchsp_eff = (FloatHi) search->dblen_eff *
      ((FloatHi) search->context[context].query->effective_length);
      
   query_start = search->context[context].query->sequence;
   query = query_start + hsp->query.offset; 
   subject = subject_start + hsp->subject.offset;
   score = 0;
   sum = 0;
   new_q_start = new_q_end = query;
   new_s_start = new_s_end = subject;
   i = 0;
   
   esp = gap_info->esp;
   prev_esp = NULL;
   last_esp = first_esp = esp;
   last_esp_num = 0;
   
   while (esp) {
      if (esp->op_type == GAPALIGN_SUB) {
         sum += factor*matrix[*query & mask][*subject];
         query++;
         subject++;
         i++;
      } else if (esp->op_type == GAPALIGN_DEL) {
         sum -= gap_open + gap_extend * esp->num;
         subject += esp->num;
         i += esp->num;
      } else if (esp->op_type == GAPALIGN_INS) {
         sum -= gap_open + gap_extend * esp->num;
         query += esp->num;
         i += esp->num;
      }
      
      if (sum < 0) {
         if (score < search->pbp->cutoff_s2 ||
             BlastKarlinStoE_simple(score, search->sbp->kbp[context],
                                    searchsp_eff) > search->pbp->cutoff_e) {
            /* Start from new offset */
            new_q_start = query;
            new_s_start = subject;
            score = sum = 0;
            if (i < esp->num) {
               esp->num -= i;
               first_esp = esp;
               i = 0;
            } else {
               first_esp = esp->next;
            }
            /* Unlink the bad part of the esp chain 
               so it can be freed later */
            if (prev_esp)
               prev_esp->next = NULL;
            last_esp = NULL;
         } else {
            /* Stop here */
            break;
         }
      } else if (sum > score) {
         /* Remember this point as the best scoring end point */
         score = sum;
         last_esp = esp;
         last_esp_num = i;
         new_q_end = query;
         new_s_end = subject;
      }
      if (i >= esp->num) {
         i = 0;
         prev_esp = esp;
         esp = esp->next;
      }
   }
   
   score /= factor;
   
   delete_hsp = FALSE;

   if (score >= search->pbp->cutoff_s2) {
      hsp->score = score;
      searchsp_eff = (FloatHi) search->dblen_eff *
         (FloatHi) search->context[context].query->effective_length;
      hsp->evalue = BlastKarlinStoE_simple(score,
                       search->sbp->kbp[context], searchsp_eff);
      if (hsp->evalue > search->pbp->cutoff_e) {
         delete_hsp = TRUE;
      } else {
         hsp->query.length = new_q_end - new_q_start;
         hsp->subject.length = new_s_end - new_s_start;
         hsp->query.offset = hsp->gap_info->start1 = new_q_start - query_start;
         hsp->query.end = hsp->query.offset + hsp->query.length;
         hsp->subject.offset = hsp->gap_info->start2 = 
             new_s_start - subject_start;
         hsp->subject.end = hsp->subject.offset + hsp->subject.length;
         /* Make corrections in edit block and free any parts that
            are no longer needed */
         if (first_esp != hsp->gap_info->esp) {
            GapXEditScriptDelete(hsp->gap_info->esp);
            hsp->gap_info->esp = first_esp;
         }
         if (last_esp->next != NULL) {
            last_esp->next = GapXEditScriptDelete(last_esp->next);
         }
         last_esp->num = last_esp_num;
         BlastHSPGetNumIdentical(search, hsp, NULL, &hsp->num_ident, 
                                 &align_length);
      }
   } else {
      delete_hsp = TRUE;
   }

   return delete_hsp;
}

/* In the following function the forward strand is assumed */

Int2
MegaBlastReevaluateWithAmbiguities(BlastSearchBlkPtr search)
{
   BLAST_HitListPtr current_hitlist;
   BLAST_HSPPtr PNTR hsp_array, hsp;
   Uint1Ptr subject_start = NULL;
   Int4 index, context, hspcnt, buf_len=0;
   Boolean purge, delete_hsp;
   Int4 query_length;
   Int2 status;

   if (!search->pbp->mb_params)
      return 1;
   
   current_hitlist = search->current_hitlist;
   if (current_hitlist == NULL || current_hitlist->hspcnt == 0)
      return 0;


   status = BlastGetNonSumStatsEvalue(search);

   /* While query offsets are not yet adjusted, remove the redundant HSPs */
   /*if (current_hitlist->hspcnt > 1)
     BlastSortUniqHspArray(current_hitlist);*/
   hspcnt = current_hitlist->hspcnt;
   
   hsp_array = current_hitlist->hsp_array;
   /* In case of greedy extension without traceback, 
      only adjust the offsets */
   if (!search->pbp->mb_params->use_dyn_prog && 
       search->pbp->mb_params->no_traceback) {
      for (index=0; index<hspcnt; index++) {
         if (hsp_array[index] != NULL) {
            /* Adjust the query offsets here */
            hsp_array[index]->query.offset -= 
               search->query_context_offsets[hsp_array[index]->context];
            hsp_array[index]->query.end -= 
               search->query_context_offsets[hsp_array[index]->context];
            hsp_array[index]->query.gapped_start -= 
               search->query_context_offsets[hsp_array[index]->context];
         }
      }
      status = BlastReapHitlistByEvalue(search);
      return status;
   }

   status = BlastReapHitlistByEvalue(search);
   
   if (current_hitlist->hspcnt == 0)
      /* All HSPs have been deleted */
      return 0;
   
   /* Can happen only when subject was being unpacked on the fly 
      during gapped extensions */
   if (!search->subject->sequence_start)
      readdb_get_sequence_ex(search->rdfp, search->subject_id, 
                             &search->subject->sequence_start, 
                             &buf_len, TRUE);
   /* The sequence in blastna encoding is now stored in sequence_start */
   subject_start = search->subject->sequence_start + 1;

   purge = FALSE;
   for (index=0; index<hspcnt; index++) {
      if (hsp_array[index] == NULL)
         continue;
      else
         hsp = hsp_array[index];

      context = hsp->context;

      /* Adjust the query offsets here */
      hsp->query.offset -= search->query_context_offsets[context];
      hsp->query.end -= search->query_context_offsets[context];
      hsp->query.gapped_start -= search->query_context_offsets[context];

      if (search->pbp->mb_params->use_dyn_prog) {
         /* Check if the extension jumped through the border between
            sequences or sequence strands. If this happens, leave the part 
            which contains the original offset pair, saved in gapped_start's
         */
         Int4 extra_length;
         query_length = search->query_context_offsets[context+1] - 
            search->query_context_offsets[context] - 1;

         while ((hsp->query.end - 1) / query_length > 0) {
            if (hsp->query.gapped_start < query_length) {
               extra_length = hsp->query.end - query_length + 1;
               hsp->subject.end -= extra_length;
               hsp->query.length -= extra_length;
               hsp->subject.length -= extra_length;
               hsp->query.end = query_length - 1;
            } else {
               extra_length = query_length + 1 - hsp->query.offset;
               hsp->subject.offset += extra_length;
               hsp->query.offset = 0;
               hsp->query.end -= (query_length + 1);
               hsp->query.length -= extra_length;
               hsp->subject.length -= extra_length;
               hsp->query.gapped_start -= query_length + 1;
               context++;
               query_length = search->query_context_offsets[context+1] - 
                  search->query_context_offsets[context] - 1;
            }
         }
         hsp->context = context;
         /* Gap information not available here, so no reevaluation 
            with ambiguities can be done - it will be done with the final
            extension with traceback */
         continue;
      }

      delete_hsp =
         ReevaluateScoreWithAmbiguities(search, subject_start, hsp);

      if (delete_hsp) { /* This HSP is now below the cutoff */
         hsp_array[index] = BLAST_HSPFree(hsp);
         purge = TRUE;
      }
   }
   if (purge) {
      index = HspArrayPurge(hsp_array, hspcnt, TRUE);
      current_hitlist->hspcnt = index;	
      current_hitlist->hspcnt_max = index;	
   }
   
   if (search->pbp->mb_params->use_dyn_prog) {
      status = BlastReapHitlistByEvalue(search);
   }

   if (current_hitlist->hspcnt > 1)
      BlastSortUniqHspArray(current_hitlist);
   
   if (search->pbp->hsp_num_max && 
       search->pbp->hsp_num_max < search->current_hitlist->hspcnt && 
       !search->pbp->mb_params->use_dyn_prog && search->last_context <= 1) {
      /* Number of HSPs to be saved is restricted by user;
         delete extra HSPs unless gapped alignments are done later, or
         there are multiple query sequences - then this is done elsewhere.
      */
      for (index=search->pbp->hsp_num_max; 
           index < search->current_hitlist->hspcnt; ++index) {
         current_hitlist->hsp_array[index] = 
            BLAST_HSPFree(current_hitlist->hsp_array[index]);
      }
      search->current_hitlist->hspcnt = search->pbp->hsp_num_max;
   }
   
   return 0;
}

/* The following binary search routine assumes that array A is filled. */
Int4 BinarySearchInt4(Int4 n, Int4Ptr A, Int4 size)
{
    Int4 m, b, e;

    b = 0;
    e = size;
    while (b < e - 1) {
	m = (b + e) / 2;
	if (A[m] > n)
	    e = m;
	else
	    b = m;
    }
    return b;
}

/* Create a SeqLoc chain of query segments covered by all HSPs from a given 
   SeqAlign - needed for masked query output only */
SeqLocPtr MaskSeqLocFromSeqAlign(SeqAlignPtr seqalign)
{
   SeqLocPtr slp = NULL, new_slp, slp_var;
   SeqAlignPtr sap;
   DenseSegPtr dsp;
   Int4 from, to;

   for (sap = seqalign; sap; sap = sap->next) {
      dsp = (DenseSegPtr) sap->segs;
      if (dsp->strands[0] == Seq_strand_plus) {
         from = dsp->starts[0];
         to = dsp->starts[2*(dsp->numseg - 1)] + 
            dsp->lens[dsp->numseg - 1] - 1;
      } else {
         to = dsp->starts[0] + dsp->lens[0] - 1;
         from = dsp->starts[2*(dsp->numseg - 1)];
      }
      new_slp = SeqLocIntNew(from, to, Seq_strand_plus, dsp->ids);
      if (slp==NULL)
	 slp = slp_var = new_slp;
      else {
	 slp_var->next = new_slp;
	 slp_var = slp_var->next;
      }
   }
   
   return slp;
}

void PrintMaskedSequence(BioseqPtr query_bsp, SeqLocPtr mask_slp, 
                         FILE *fp, Int2 line_len, Boolean lcase_masking)
{
   SeqLocPtr query_slp = NULL;
   SeqPortPtr spp;
   Uint1Ptr query_seq, seq_line;
   Int4 index, id_length = 0, defline_len;
   Uint1 residue;
   CharPtr buffer = NULL;

   ValNodeAddPointer(&query_slp, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBest(query_bsp->id, SEQID_GI)));
   

   spp = SeqPortNewByLoc(query_slp, Seq_code_iupacna);
   SeqPortSet_do_virtual(spp, TRUE);
   query_seq = (Uint1Ptr) MemNew((query_bsp->length + 1)*sizeof(Uint1));
   index=0;
   while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
      if (IS_residue(residue)) {
	 query_seq[index] = residue;
	 index++;
      }
   }
   if (lcase_masking)
      BlastLCaseMaskTheResidues(query_seq, query_bsp->length, mask_slp, FALSE,
                                SeqLocStart(query_slp));
   else
      BlastMaskTheResidues(query_seq, query_bsp->length, 'N', mask_slp, FALSE,
                           SeqLocStart(query_slp));

   SeqLocFree(query_slp);
   if (query_bsp->descr)
      defline_len = StringLen(BioseqGetTitle(query_bsp));
   else
      defline_len = 0;
   defline_len += 255;     /* Sufficient for an ID. */
   buffer = MemNew((defline_len+1)*sizeof(Char));
   if (query_bsp->id->choice != SEQID_LOCAL) {
      SeqIdWrite(query_bsp->id, buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
      id_length = StringLen(buffer);
      buffer[id_length] = ' ';
      id_length++;
   }
   StringCpy(&buffer[id_length], BioseqGetTitle(query_bsp));
   fprintf(fp, ">%s\n", buffer);
   
   seq_line = query_seq;
   for (index=0; index<=(query_bsp->length-1)/line_len; index++) {
      fprintf(fp, "%.*s\n", line_len, seq_line);
      seq_line += line_len;
   }
      
   spp = SeqPortFree(spp);
   MemFree(buffer);
   query_seq = MemFree(query_seq);
}

#define MIN_HSP_ARRAY_SIZE 20
Int4 LIBCALL
MegaBlastSaveCurrentHitlist(BlastSearchBlkPtr search)
{
	BLASTResultHitlistPtr result_hitlist, PNTR results, PNTR mb_results;
	BLASTResultsStructPtr result_struct;
	BLAST_HitListPtr current_hitlist;
	BLAST_HSPPtr hsp; 
	BLAST_KarlinBlkPtr kbp;
	BLASTResultHspPtr hsp_array;
	Int4 hspcnt, index, index1, new_index, old_index, low_index, high_index;
	Int4 hitlist_count, hitlist_max, hspmax, high_score=0, retval;
	Nlm_FloatHi current_evalue=DBL_MAX;
	Int4 query_length, hsp_index, new_size;
        Int4Ptr hsp_array_sizes;
	Int2 num_queries, query_index;
        Boolean PNTR do_not_reallocate;
        Boolean use_dyn_prog = search->pbp->mb_params->use_dyn_prog;

	if (search == NULL)
		return 0;	

	if (search->current_hitlist == NULL || search->current_hitlist->hspcnt <= 0)	/* No hits to save. */
	{
		search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
		return 0;
	}

	current_hitlist = search->current_hitlist;

	retval = current_hitlist->hspcnt;
	
        num_queries = search->last_context / 2 + 1;
        mb_results = (BLASTResultHitlistPtr PNTR) 
           MemNew(num_queries*sizeof(BLASTResultHitlistPtr));
        hsp_array_sizes = (Int4Ptr) MemNew(num_queries*sizeof(Int4));
        do_not_reallocate = (Boolean PNTR) MemNew(num_queries*sizeof(Boolean));

        /* The HSPs in the hsp_array are sorted previously by score, therefore
           for each individual query they already come in the increasing order
           of evalue, since linking of HSPs is not done in megablast */
        index1 = 0;
        hspcnt = current_hitlist->hspcnt;
        hspmax = current_hitlist->hspcnt_max;

        HeapSort(current_hitlist->hsp_array, hspcnt,
                 sizeof(BLAST_HSPPtr), score_compare_hsps);

        hsp = current_hitlist->hsp_array[0];
        for (index=0; index<hspcnt; index++) { 
           while (hsp == NULL && index1 < hspmax) {
              index1++;
              hsp = current_hitlist->hsp_array[index1];
           }
           if (index1==hspmax) break;
           if (search->pbp->mb_params->perc_identity > 0) {
              if (MegaBlastGetHspPercentIdentity(search, hsp) < 
                  search->pbp->mb_params->perc_identity) {
                 index1++;
                 if (index1 >= hspmax)
                    break;
                 hsp = current_hitlist->hsp_array[index1];
                 continue;
              }
           }
           query_index = hsp->context / 2;

           result_hitlist = mb_results[query_index];
           if (!result_hitlist) {
              hsp_array_sizes[query_index] = MIN_HSP_ARRAY_SIZE;
              if (search->pbp->hsp_num_max && 
                  search->pbp->hsp_num_max < MIN_HSP_ARRAY_SIZE)
                 hsp_array_sizes[query_index] = search->pbp->hsp_num_max;  
              result_hitlist = mb_results[query_index] =
                 BLASTResultHitlistNew(hsp_array_sizes[query_index]);
              result_hitlist->hspcnt = 1;
              /* This is the first HSP for this query, so it is the 
                 best one */
              result_hitlist->best_evalue = hsp->evalue;
              result_hitlist->high_score = hsp->score;
           } else if (result_hitlist->hspcnt == hsp_array_sizes[query_index]) { 
              if (!do_not_reallocate[query_index]) {
                 new_size = 2*hsp_array_sizes[query_index];
                 if (search->pbp->hsp_num_max && 
                     search->pbp->hsp_num_max < new_size && !use_dyn_prog) {
                    /* Number of HSPs to be saved is restricted by user;
                       stop at this limit if gapped alignment has already
                       been completed */
                    new_size = search->pbp->hsp_num_max;
                    if (new_size <= hsp_array_sizes[query_index]) {
                       do_not_reallocate[query_index] = TRUE;
                       continue;
                    }
                 }

                 if (!(hsp_array = Realloc(result_hitlist->hsp_array,
                           new_size*sizeof(BLASTResultHsp)))) {
                    ErrPostEx(SEV_WARNING, 0, 0, 
                              "Cannot reallocate hsp_array for a result hitlist, size %d", new_size);
                    do_not_reallocate[query_index] = TRUE;
                 } else {
                    result_hitlist->hsp_array = hsp_array;
                    result_hitlist->hspcnt++;
                    hsp_array_sizes[query_index] = new_size;
                 }
              } else {
		/* hsp_array is already full and reallocation not allowed */
                 continue;
	      }
           } else 
              result_hitlist->hspcnt++;

           if (result_hitlist == NULL) /* Should never happen */
              continue;
	
           result_hitlist->subject_id = search->subject_id;
           result_hitlist->subject_info = search->subject_info;
           
           hsp_array = result_hitlist->hsp_array;
           hsp_index = result_hitlist->hspcnt - 1;

           hsp_array[hsp_index].ordering_method = hsp->ordering_method;
           hsp_array[hsp_index].number = hsp->num;
           hsp_array[hsp_index].score = hsp->score;
           hsp_array[hsp_index].e_value = hsp->evalue;
           hsp_array[hsp_index].num_ident = hsp->num_ident;
           kbp = search->sbp->kbp[hsp->context];
           hsp_array[hsp_index].bit_score = ((hsp->score*kbp->Lambda) -
                                             kbp->logK)/NCBIMATH_LN2;
                        
           if (hsp->context & 1) 
              hsp_array[hsp_index].query_frame = -1;
           else 
              hsp_array[hsp_index].query_frame = 1;
           query_length = 
              search->query_context_offsets[hsp->context+1] -
              search->query_context_offsets[hsp->context] - 1;
           if (!use_dyn_prog && !search->pbp->mb_params->no_traceback) {
              hsp->gap_info->start1 = hsp->query.offset;
              hsp->gap_info->start2 = hsp->subject.offset;
              hsp->gap_info->frame1 = hsp_array[hsp_index].query_frame;
              hsp->gap_info->length1 = query_length;
              hsp_array[hsp_index].query_gapped_start = hsp->query.offset + 1;
              hsp_array[hsp_index].subject_gapped_start =
                 hsp->subject.offset + 1;
           } else { 
              hsp_array[hsp_index].query_gapped_start = 
                 hsp->query.gapped_start;
              hsp_array[hsp_index].subject_gapped_start = 
                 hsp->subject.gapped_start;
           }
           hsp_array[hsp_index].gap_info = hsp->gap_info;
           /* Edit block pointer has been copied; remove it from hsp to avoid
              double freeing */
           hsp->gap_info = NULL;
           hsp_array[hsp_index].context = hsp->context; 
           hsp_array[hsp_index].query_offset = hsp->query.offset;
           hsp_array[hsp_index].query_length = hsp->query.length;
           hsp_array[hsp_index].subject_offset = hsp->subject.offset;
           hsp_array[hsp_index].subject_length = hsp->subject.length;
           hsp_array[hsp_index].subject_frame = hsp->subject.frame;;
           hsp_array[hsp_index].point_back = result_hitlist;
           hsp_array[hsp_index].back_left = 
              hsp_array[hsp_index].back_right = NULL; 
           hsp_array[hsp_index].hspset_cnt = -1;

           index1++;
           if (index1 >= hspmax)
              break;
           hsp = current_hitlist->hsp_array[index1];
        }

        search->subject_info = NULL;

/* For MT BLAST we check that no other thread is attempting to insert results. */
	if (search->thr_info->results_mutex)
            NlmMutexLock(search->thr_info->results_mutex);
        
        for (query_index=0; query_index<num_queries; query_index++) {
           result_hitlist = mb_results[query_index];
           if (!result_hitlist)
              continue;

           /* This is the structure that is identical on every thread. */
           result_struct = search->mb_result_struct[query_index];

           if (!result_struct)
              result_struct = search->mb_result_struct[query_index] = 
                 BLASTResultsStructNew(search->result_size, 0, 0);

           hitlist_count = result_struct->hitlist_count;
           hitlist_max = result_struct->hitlist_max;
           results = result_struct->results;

           /* Record the worst evalue for ReevaluateWithAmbiguities. */
           if (hitlist_count == hitlist_max)
              search->worst_evalue = results[hitlist_count-1]->best_evalue;
           
           current_evalue = result_hitlist->best_evalue;
           high_score = result_hitlist->high_score;
           /* New hit is less significant than all the other hits. */
           if (hitlist_count > 0 && 
               (current_evalue > results[hitlist_count-1]->best_evalue ||
                (current_evalue == results[hitlist_count-1]->best_evalue &&
                 high_score < results[hitlist_count-1]->high_score))) {
              if (hitlist_count == hitlist_max) {
                 /* Array is full, delete the entry. */
                 result_hitlist = BLASTResultHitlistFreeEx(search, result_hitlist);
                 continue;
              } else       /* Add to end of array. */
                 new_index = hitlist_count;
           } else if (hitlist_count > 0) {
              /* The array is all NULL's if hitlist_count==0 */
              high_index=0;
              low_index=hitlist_count-1;
              new_index = (high_index+low_index)/2;
              old_index = new_index;
              for (index=0; index<BLAST_SAVE_ITER_MAX; index++) {
                 if (results[new_index]->best_evalue > current_evalue)
                    low_index = new_index;
                 else if (results[new_index]->best_evalue < current_evalue)
                    high_index = new_index;
                 else { /* If e-values are the same, use high score. */
                    /* If scores are the same, use ordinal number. */
                    if (results[new_index]->high_score < high_score)
                       low_index = new_index;
                    else if (results[new_index]->high_score > high_score)
                       high_index = new_index;
                    else if (results[new_index]->subject_id < search->subject_id)
                       low_index = new_index;
                    else
                       high_index = new_index;
                 }
                 
                 new_index = (high_index+low_index)/2;
                 if (old_index == new_index) {
                    if (results[new_index]->best_evalue < current_evalue)
                       /* Perform this check as new_index get rounded DOWN above.*/
                       new_index++;
                    else if (results[new_index]->best_evalue == current_evalue && results[new_index]->high_score > high_score)
                       new_index++;
                    break;
                 }
                 old_index = new_index;
              }
              if (hitlist_count == hitlist_max) {	
                 /* The list is full, delete the last entry. */
                 if (results[hitlist_max-1]->seqalign)
                    SeqAlignSetFree(results[hitlist_max-1]->seqalign);
                 results[hitlist_max-1] = 
                    BLASTResultHitlistFreeEx(search, results[hitlist_max-1]);
                     result_struct->hitlist_count--;	
                     hitlist_count = result_struct->hitlist_count;	
              }
              if (hitlist_max > 1)
                 Nlm_MemMove((results+new_index+1), (results+new_index), 
                             (hitlist_count-new_index)*sizeof(results[0]));
           } else /* First hit to be stored. */
              new_index = 0;
	
           if (new_index < hitlist_max) {
              results[new_index] = result_hitlist;
              result_struct->hitlist_count++;	
           }
        }
        MemFree(do_not_reallocate);
        MemFree(hsp_array_sizes);
        MemFree(mb_results);
        /* Free mutex. */
	if (search->thr_info->results_mutex)
            NlmMutexUnlock(search->thr_info->results_mutex);
	return retval;
}

FloatLo 
MegaBlastGetHspPercentIdentity(BlastSearchBlkPtr search, BLAST_HSPPtr hsp)
{
   FloatLo perc_ident;
   Int4 numseg, align_length, i, q_off, s_off;
   Int2 context;
   Int4Ptr length, start;
   Uint1Ptr strands;
   Uint1Ptr query_seq, subject_seq = NULL;
   GapXEditScriptPtr esp;

   if (!hsp->gap_info) 
      /* Cannot do any meaningful check at this time, return 100 so nothing is
         discarded */
      return 100.0;

   context = hsp->context;
   query_seq = search->context[context].query->sequence;
   subject_seq = search->subject->sequence_start + 1;
   
   esp = hsp->gap_info->esp;
   
   for (numseg=0; esp; esp = esp->next, numseg++);
   
   /* GXECollectDataForSeqalign might change the query offset, so need an extra
      variable */
   q_off = hsp->query.offset;
   s_off = hsp->subject.offset;
   GXECollectDataForSeqalign(hsp->gap_info, hsp->gap_info->esp, numseg,
                             &start, &length, &strands, 
                             &q_off, &s_off);
   
   perc_ident = 0;
   align_length = 0;
   for (i=0; i<numseg; i++) {
      align_length += length[i];
      if (start[2*i] != -1 && start[2*i+1] != -1) {
         perc_ident += 
            (FloatLo) MegaBlastGetNumIdentical(query_seq, subject_seq, 
                                               start[2*i], start[2*i+1], 
                                               length[i], FALSE);
      }
   }
   perc_ident = perc_ident / align_length * 100;
   MemFree(start);
   MemFree(length);
   MemFree(strands);
   return perc_ident;
}

MegaBlastParameterBlkPtr MegaBlastParameterBlkNew(BLAST_OptionsBlkPtr options)
{
   MegaBlastParameterBlkPtr mb_params = (MegaBlastParameterBlkPtr)
      MemNew(sizeof(MegaBlastParameterBlk));
   
   mb_params->no_traceback = options->no_traceback;
   mb_params->is_neighboring = options->is_neighboring;
   mb_params->full_seqids = options->megablast_full_deflines;
   mb_params->perc_identity = options->perc_identity;
   mb_params->max_positions = options->block_width;
   if (options->mb_template_length > 0) {
      mb_params->template_length = options->mb_template_length;
      mb_params->disc_word = TRUE;
      mb_params->word_weight = options->wordsize;
      mb_params->template_type = 
         GetMBTemplateType(mb_params->word_weight, mb_params->template_length,
                           options->mb_disc_type);
      if (options->mb_disc_type == MB_TWO_TEMPLATES)
         mb_params->use_two_templates = TRUE;
   } else {
      mb_params->template_length = MIN(options->wordsize, 12);
      mb_params->word_weight = options->wordsize;
   }
   mb_params->one_base_step = options->mb_one_base_step;
   mb_params->use_dyn_prog = options->mb_use_dyn_prog;

   return mb_params;
}

MBTemplateType GetMBTemplateType(Int2 weight, Int2 length, MBDiscWordType type)
{
   if (weight == 11) {
      if (length == 16) {
         if (type == MB_WORD_CODING || type == MB_TWO_TEMPLATES)
            return TEMPL_11_16;
         else if (type == MB_WORD_OPTIMAL)
            return TEMPL_11_16_OPT;
      } else if (length == 18) {
         if (type == MB_WORD_CODING || type == MB_TWO_TEMPLATES)
            return TEMPL_11_18;
         else if (type == MB_WORD_OPTIMAL)
            return TEMPL_11_18_OPT;
      } else if (length == 21) {
         if (type == MB_WORD_CODING || type == MB_TWO_TEMPLATES)
            return TEMPL_11_21;
         else if (type == MB_WORD_OPTIMAL)
            return TEMPL_11_21_OPT;
      }
   } else if (weight == 12) {
      if (length == 16) {
         if (type == MB_WORD_CODING || type == MB_TWO_TEMPLATES)
            return TEMPL_12_16;
         else if (type == MB_WORD_OPTIMAL)
            return TEMPL_12_16_OPT;
      } else if (length == 18) {
         if (type == MB_WORD_CODING || type == MB_TWO_TEMPLATES)
            return TEMPL_12_18;
         else if (type == MB_WORD_OPTIMAL)
            return TEMPL_12_18_OPT;
      } else if (length == 21) {
         if (type == MB_WORD_CODING || type == MB_TWO_TEMPLATES)
            return TEMPL_12_21;
         else if (type == MB_WORD_OPTIMAL)
            return TEMPL_12_21_OPT;
      }
   }
   return TEMPL_ERROR;
}
