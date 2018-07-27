/* $Id: blast.c,v 6.330 2002/01/04 22:01:33 coulouri Exp $
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's offical duties as a United States Government employee and
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

File name: blast.c

Author: Tom Madden

Contents: BLAST functions

Detailed Contents: 

	- Functions that allocate and deallocate structures used by BLAST.

	- Functions that find the initial word hits for BLAST (both contiguous
	and discontiguous).

	- Functions that extend these initial word hits and decide if the
	results HSP (High-Scoring Segment Pairs) are worth keeping.

	- Functions that link together HSP's to a "hitlist".

	- Functions that save the hitlist to a structure appropriate for 
	further manipulation.

******************************************************************************
 * $Revision: 6.330 $
 *
 * $Log: blast.c,v $
 * Revision 6.330  2002/01/04 22:01:33  coulouri
 * Fixed BlastSetLimits() to work under linux
 *
 * Revision 6.329  2002/01/04 20:16:12  dondosha
 * Correction for single strand blastx with OOF gapping
 *
 * Revision 6.328  2001/12/28 20:38:40  dondosha
 * Moved Mega BLAST related parameters into a separate structure
 *
 * Revision 6.327  2001/12/17 17:31:35  madden
 * Fix memory leaks
 *
 * Revision 6.326  2001/12/14 21:04:31  madden
 * Reinit start to zero for every frame
 *
 * Revision 6.325  2001/12/10 23:04:19  dondosha
 * Corrected how number of db sequences is set when gi list exists
 *
 * Revision 6.324  2001/11/23 21:11:16  dondosha
 * Correction to previous change
 *
 * Revision 6.323  2001/11/23 19:57:55  dondosha
 * Correction for bl2seq related to recent changes in megablast
 *
 * Revision 6.322  2001/11/14 23:39:31  dondosha
 * Switched return value for BlastNtWordUngappedExtend
 *
 * Revision 6.321  2001/11/13 18:17:26  dondosha
 * Added BlastNtWordUngappedExtend for use in Mega BLAST
 *
 * Revision 6.320  2001/09/21 14:42:08  dondosha
 * Correction of previous fix in BlastReapPartialHitlistByEvalue for non-megablast programs
 *
 * Revision 6.319  2001/09/20 14:39:15  madden
 * Fix for non-blastn programs in BlastReapPartialHitlistByEvalue
 *
 * Revision 6.318  2001/09/17 16:33:39  dondosha
 * Bug fix in BlastReapPartialHitlistByEvalue
 *
 * Revision 6.317  2001/09/11 14:28:30  madden
 * Added timed_out Boolean to SearchBlk
 *
 * Revision 6.316  2001/09/07 14:46:43  dondosha
 * Roll back removal of threshold_first from functions and structures
 *
 * Revision 6.315  2001/09/06 20:24:33  dondosha
 * Removed threshold_first
 *
 * Revision 6.314  2001/08/10 14:55:55  madden
 * Add pv_array for multiple hits blastn
 *
 * Revision 6.313  2001/07/24 19:50:32  dondosha
 * Do not create a star_proc thread if there is no tick_proc
 *
 * Revision 6.312  2001/07/20 18:52:25  dondosha
 * Removed unused code
 *
 * Revision 6.311  2001/07/18 19:24:17  madden
 * Set options->dbseq_num if use_real_db TRUE
 *
 * Revision 6.310  2001/07/09 14:17:23  madden
 * Fix PC-lint complaints from R. Williams
 *
 * Revision 6.309  2001/07/09 13:12:02  madden
 * Removed unused variables
 *
 * Revision 6.308  2001/07/06 15:22:42  madden
 * Correction for BLASTN
 *
 * Revision 6.307  2001/06/29 18:07:20  madden
 * Fix problem with scalingFactor
 *
 * Revision 6.306  2001/06/28 13:42:09  madden
 * Fixes to prevent overflow on number of hits reporting
 *
 * Revision 6.305  2001/06/27 17:46:33  madden
 * Add mutex to protect number_of_pos_hits, found by H. Gabb at KAI
 *
 * Revision 6.304  2001/06/26 20:37:04  madden
 * Fixes for realdb_done problem found by H. Gabb at KAI
 *
 * Revision 6.303  2001/06/25 16:03:12  madden
 * Correctly set gapped_start for blastn
 *
 * Revision 6.302  2001/06/21 21:29:07  dondosha
 * Fixed memory leaks: destroy all error returns, free private_slp
 *
 * Revision 6.301  2001/06/15 16:38:45  dondosha
 * Correction to previous changes
 *
 * Revision 6.300  2001/06/14 22:09:14  dondosha
 * Rearranged code for gi lists and oid masks processing to get rid of duplication
 *
 * Revision 6.299  2001/06/13 21:45:08  dondosha
 * Search of multiple databases with gi files implemented
 *
 * Revision 6.298  2001/06/12 19:48:55  madden
 * Introduce total_hsp_limit, check before making SeqAlign
 *
 * Revision 6.297  2001/05/25 19:34:17  vakatov
 * Nested comment typo fixed
 *
 * Revision 6.296  2001/05/04 15:59:46  dondosha
 * Function BlastFillQueryOffsets now has an extra argument for megablast use
 *
 * Revision 6.295  2001/05/03 21:48:28  dondosha
 * Handle some cases when memory allocation fails
 *
 * Revision 6.294  2001/04/23 17:09:18  madden
 * Use StringSave for gifile variable
 *
 * Revision 6.293  2001/04/16 16:37:01  madden
 * Restore old length correction behavior for blastn
 *
 * Revision 6.292  2001/04/13 20:46:42  madden
 * Changed edge effect correction in BLASTSetUpSearchInternalByLoc to use new method with  alpha and beta parameters from Altschul, Bundschuh, Olsen, Hwa, Nucleic Acids Research 29(2001), 351-361.
 *
 * Revision 6.291  2001/04/11 20:56:06  madden
 * Added scalingFactor for rpsblast
 *
 * Revision 6.290  2001/04/04 20:31:16  dondosha
 * Bug fix for blastx with a subsequence query
 *
 * Revision 6.289  2001/04/02 15:55:27  dondosha
 * Check HSP frames when merging hitlists from split subject sequence
 *
 * Revision 6.288  2001/03/30 23:53:45  dondosha
 * Correction in splitting long database sequences for tblastn
 *
 * Revision 6.287  2001/03/19 18:53:45  madden
 * Added call to BlastSeqLocFillDoubleIntEx, changed call to BlastSeqLocFillDoubleIntRev
 *
 * Revision 6.286  2001/03/14 14:54:35  madden
 * fix problem with partial translating query
 *
 * Revision 6.285  2001/03/12 21:38:59  dondosha
 * Bug fix in database sequence splitting change
 *
 * Revision 6.284  2001/03/08 22:05:47  dondosha
 * Split very long database sequences in all BLAST programs
 *
 * Revision 6.283  2001/03/07 14:09:17  madden
 * Set multiple_hits depending on option block
 * 
 * Revision 6.282  2001/03/06 22:02:32  dondosha
 * Rolled back accidental change in BlastReevaluateWithAmbiguities
 *
 * Revision 6.281  2001/03/01 15:41:33  dondosha
 * Added protection from infinite loop in new_link_hsps
 *
 * Revision 6.280  2001/01/24 21:55:53  dondosha
 * Correction to previous change
 *
 * Revision 6.279  2001/01/24 20:51:49  dondosha
 * Enabled splitting of the second sequence for 2 sequences with megablast
 *
 * Revision 6.278  2001/01/19 17:23:16  madden
 * Optimization for 2-hit blastn
 *
 * Revision 6.277  2001/01/16 14:03:53  madden
 * Enable gapped check for blastn immediately after finding hits
 *
 * Revision 6.276  2001/01/09 20:10:37  shavirin
 * Added sorting of all hits in result_struct for every element in
 * results. Added function RPSResultHspScoreCmp.
 *
 * Revision 6.275  2001/01/08 20:21:40  dondosha
 * Adjust subject offset in the gap edit blocks if database sequence was split in megablast search
 *
 * Revision 6.274  2001/01/03 21:45:29  dondosha
 * Fixed a memory leak - some edit blocks not freed in megablast
 *
 * Revision 6.273  2001/01/02 22:29:45  dondosha
 * Assign virtual oidlist to the first non-whole database rdfp in the linked list
 *
 * Revision 6.272  2000/12/28 18:22:29  madden
 * Fixes to BlastNtWordFinder_mh
 *
 * Revision 6.271  2000/12/27 16:51:17  dondosha
 * When splitting database sequence for megablast, keep only significant HSPs from partial hitlists
 *
 * Revision 6.270  2000/12/26 17:50:46  dondosha
 * Fixed bug in BLASTMergeHsps function for merging HSPs after splitting of a database sequence
 *
 * Revision 6.269  2000/12/21 17:37:24  dondosha
 * Fixed bug with minus-strand blastn search
 *
 * Revision 6.268  2000/12/20 15:44:01  madden
 * Better error message if query is shorter than wordsize
 *
 * Revision 6.267  2000/12/18 20:38:55  shavirin
 * Removed include <time.h> before <ncbi.h>.
 *
 * Revision 6.266  2000/12/07 17:45:13  dondosha
 * Use actual subject sequence length in GreedyAlignMemAlloc for 2 Sequences engine
 *
 * Revision 6.265  2000/12/04 18:51:23  madden
 * Fix memory leaks
 *
 * Revision 6.264  2000/11/29 16:58:16  dondosha
 * Small fix to previous revision
 *
 * Revision 6.263  2000/11/29 16:29:31  dondosha
 * For megablast, allow splitting of long subject sequences and merging hitlists
 *
 * Revision 6.262  2000/11/17 17:51:59  dondosha
 * Removed is_megablast argument from BLASTSetUpSearchWithReadDbInternalEx since it is part of options
 *
 * Revision 6.261  2000/11/13 20:38:48  madden
 * Fix for zero length db sequence in ungapped blast
 *
 * Revision 6.260  2000/11/09 14:59:38  dondosha
 * Longest intron length in options set in nucleotide coordinates
 *
 * Revision 6.259  2000/11/08 22:21:32  dondosha
 * Enabled new tblastn by adding a longest_intron option
 *
 * Revision 6.258  2000/11/07 16:30:24  madden
 * Introduce intermediate score (before linking of HSPs) for blastx and tblastn
 *
 * Revision 6.257  2000/11/03 20:13:55  dondosha
 * Do not call readdb_get_sequence_ex from new_link_hsps for two sequences BLAST
 *
 * Revision 6.256  2000/11/01 16:25:58  madden
 * Changes from Futamura for psitblastn
 *
 * Revision 6.255  2000/11/01 00:05:18  vakatov
 * Added missing "LIBCALL"
 *
 * Revision 6.254  2000/10/31 16:30:56  shavirin
 * Function BLASTSetUpSearchInternalByLoc became external.
 *
 * Revision 6.253  2000/10/30 16:51:04  shavirin
 * Changed function with creation temporary bioseqs for SEG filtering.
 *
 * Revision 6.252  2000/10/26 18:45:58  dondosha
 * Check if gi list file is provided from the db alias
 *
 * Revision 6.251  2000/10/24 19:05:45  dondosha
 * Moved function UniqueLocalId to sequtil.c
 *
 * Revision 6.250  2000/10/06 21:36:02  dondosha
 * Do not multiply window size by 3 for subject in new_link_hsps
 *
 * Revision 6.249  2000/10/06 16:36:57  shavirin
 * Correctly closed file with gi list in the function GetGisFromFile().
 *
 * Revision 6.248  2000/10/05 19:54:50  dondosha
 * For Mega BLAST, call MegaBlastSaveCurrentHitlist instead of BlastSaveCurrentHitlist
 *
 * Revision 6.247  2000/09/28 15:05:59  dondosha
 * Added splice junction search; corrected sum evalue calculation
 *
 * Revision 6.246  2000/09/28 14:27:52  madden
 * Correct use of search space for linked hsps
 *
 * Revision 6.245  2000/09/18 16:04:38  madden
 * No call to BlastFindWords if rpsblast
 *
 * Revision 6.244  2000/09/14 14:58:20  dondosha
 * Further improvements with new tblastn (still not in the executable)
 *
 * Revision 6.243  2000/09/12 16:11:31  dondosha
 * Changed window size, plus some bug fixes for new_link_hsps
 *
 * Revision 6.242  2000/09/01 18:25:10  dondosha
 * Pass start and length to BlastFindWords, not start and end
 *
 * Revision 6.241  2000/09/01 13:47:39  shavirin
 * Fixed error and typecast warnings from Windows NT compilation.
 *
 * Revision 6.240  2000/08/31 18:37:22  shavirin
 * Added check for NULL in BlastMakeCopyQueryDNAP().
 *
 * Revision 6.239  2000/08/31 17:06:20  shavirin
 * Added few OOF related functions to copy and delete query_dnap.
 *
 * Revision 6.238  2000/08/31 15:59:12  dondosha
 * No need to call ReadDBFreeSharedInfo from do_the_blast_run
 *
 * Revision 6.237  2000/08/29 19:36:37  madden
 * Do not lookup gis if gilist_already_calculated is set
 *
 * Revision 6.236  2000/08/29 18:09:34  dondosha
 * Adjust the reverse strand offsets for non-megablast blastn in BlastSaveCurrentHitlist
 *
 * Revision 6.235  2000/08/25 22:41:49  dondosha
 * Do reevaluation of score with ambiguities for megablast
 *
 * Revision 6.234  2000/08/23 18:48:44  madden
 * Use BlastKarlinBlkGappedCalcEx in place of BlastKarlinBlkGappedCalc
 *
 * Revision 6.233  2000/08/18 20:12:28  dondosha
 * Do not use search->query_id in megablast, use only qid_array
 *
 * Revision 6.232  2000/08/07 16:59:49  dondosha
 * Correct construction of path for gi list file
 *
 * Revision 6.231  2000/08/03 17:50:37  dondosha
 * Check HSPs for going beyond ends of query in megablast
 *
 * Revision 6.230  2000/08/02 15:26:09  dondosha
 * For megablast compute search space depending on query when getting evalue
 *
 * Revision 6.229  2000/07/25 16:52:49  shavirin
 * Corrected function BlastCreateQueryDNAP().
 *
 * Revision 6.228  2000/07/24 16:12:05  hurwitz
 * made definition of BLASTSetUpSearchWithReadDbInternalEx match the one in blastpri.h
 *
 * Revision 6.227  2000/07/21 21:26:43  dondosha
 * Added BLASTSetUpSearchWithReadDbInternalEx with Boolean argument is_megablast
 *
 * Revision 6.226  2000/07/18 22:32:38  shavirin
 * Adjusted space allocated for DNA-P query sequence
 *
 * Revision 6.225  2000/07/17 14:17:10  shavirin
 * Added new function BlastCreateQueryDNAP() and OOF_TranslateToDNAP() and
 * support for Out of frame gap algorithm.
 *
 * Revision 6.224  2000/07/12 13:36:29  shavirin
 * Removed last NULL parameter from MegaBlastSetUpSearchInternalByLoc().
 *
 * Revision 6.223  2000/07/11 17:16:19  shavirin
 * Added new parameter is_ooframe for Out-Of-Frame gapping algorithm.
 *
 * Revision 6.222  2000/06/22 22:28:07  dondosha
 * Only look at HSPs up to hspcnt_max in BlastSaveCurrentHitlist - this allows not to use MemNew when initializing hsp_array
 *
 * Revision 6.221  2000/06/22 14:08:20  madden
 * Fix bug in BlastWordExtend_prelim if word-hit is at end of sequence
 *
 * Revision 6.220  2000/06/08 20:34:10  madden
 * add explode_seqids option to show all ids in a defline
 *
 * Revision 6.219  2000/05/25 21:03:56  dondosha
 * In BlastSaveCurrentHitlist assign hspcnt for result hitlist correctly
 *
 * Revision 6.218  2000/05/24 19:48:06  dondosha
 * Moved initialization of qid_array in megablast to search set-up
 *
 * Revision 6.217  2000/05/19 19:36:18  madden
 * Fix for longer words in BlastNtWordFinder, do not call BlastNTPreliminaryGappedScore
 *
 * Revision 6.216  2000/05/17 17:13:36  dondosha
 * Removed some unused variables
 *
 * Revision 6.215  2000/05/16 19:59:24  madden
 * Do no set ignore_small_gaps to TRUE
 *
 * Revision 6.214  2000/05/12 19:42:29  dondosha
 * Use array instead of linked list of query ids in megablast
 *
 * Revision 6.213  2000/05/12 18:53:25  shavirin
 * Fixed memory leak with OIDList.
 *
 * Revision 6.212  2000/05/11 18:02:23  shavirin
 * Minor change for using gi_list together with oid-databasees.
 *
 * Revision 6.211  2000/05/09 19:42:49  shavirin
 * Fixed in BlastGetDbChunk() no-mutex regular database case.
 *
 * Revision 6.210  2000/05/03 17:08:26  shavirin
 * Fixed minor bug in the function BLASTSetUpSearchWithReadDbInternal().
 *
 * Revision 6.209  2000/05/01 21:24:54  dondosha
 * Changed greedy_gapped_align to MegaBlastGreedyAlign
 *
 * Revision 6.208  2000/04/28 17:51:49  shavirin
 * Replaced define RPS_BLAST with checking parameter is_rps_blast.
 *
 * Revision 6.207  2000/04/25 19:05:13  dondosha
 * Before search assign db_chunk_last to first_db_seq
 *
 * Revision 6.206  2000/04/24 16:43:51  dondosha
 * Call BlastReapHitlistByEvalue in MegaBlast if hitlists are saved
 *
 * Revision 6.205  2000/04/20 15:12:32  dondosha
 * Bug fix for minus-strand only search - do not try to concatenate second strand to first
 *
 * Revision 6.204  2000/04/11 12:47:08  madden
 * Proper casting to Int8
 *
 * Revision 6.203  2000/04/10 20:01:24  dondosha
 * Fill both strands mask locations in a one location list for blastn
 *
 * Revision 6.202  2000/04/10 17:16:44  madden
 * Make search_sp Int8 to prevent overflow
 *
 * Revision 6.201  2000/04/10 15:24:24  dondosha
 * Enabled use of MegaBlast for BlastTwoSequences
 *
 * Revision 6.200  2000/04/07 20:19:45  dondosha
 * Do not call BlastReapHitlistByEvalue for megablast
 *
 * Revision 6.199  2000/04/07 16:43:25  dondosha
 * Assign dbseq_num to min of actual db size and gilist size
 *
 * Revision 6.198  2000/04/07 13:11:56  shavirin
 * Checked for queue_callback != NULL.
 *
 * Revision 6.197  2000/04/06 13:13:33  shavirin
 * Changed sequence to post semaphore info for internal queueing.
 *
 * Revision 6.196  2000/04/04 20:48:21  dondosha
 * Fixed a memory leak in saving hitlists for MegaBlast
 *
 * Revision 6.195  2000/04/04 16:16:59  dondosha
 * Fixed some memory leaks in MegaBlast traceback
 *
 * Revision 6.194  2000/04/03 21:21:44  dondosha
 * Assign is_neighboring parameter from option
 *
 * Revision 6.193  2000/03/31 21:14:24  dondosha
 * Changed some names related to MegaBlast
 *
 * Revision 6.192  2000/03/31 16:50:51  dondosha
 * Sort hsps and remove redundant when saving hitlist in MegaBlast
 *
 * Revision 6.191  2000/03/30 21:45:04  madden
 * Add call to BLASTResultHitlistFreeEx
 *
 * Revision 6.190  2000/03/29 22:19:43  dondosha
 * BlastSaveCurrentHitlist adjusts query offsets for blastn; creates seqaligns for MegaBlast
 *
 * Revision 6.189  2000/03/27 16:46:22  madden
 * Moved call to BlastFillQueryOffsets to BLASTSetUpSearchInternalByLoc
 *
 * Revision 6.188  2000/03/23 20:51:15  dondosha
 * Set dbseq_num to gi_list_total if search space is not recalculated and gi_list exists
 *
 * Revision 6.187  2000/03/22 18:08:59  dondosha
 * Free rdfp->shared_info in single threaded case the same way as in multithreaded after the search
 *
 * Revision 6.186  2000/03/14 21:01:16  dondosha
 * Call BlastTickProc even when gi_list is set
 *
 * Revision 6.185  2000/03/13 21:11:35  dondosha
 * Check options parameters use_real_db_size and sort_gi_list when dealing with gi_list
 *
 * Revision 6.184  2000/03/03 18:02:05  shavirin
 * Added support for low character filering in "blastx", "tblastx"
 * and translated RPS Blast.
 *
 * Revision 6.183  2000/03/03 17:41:09  egorov
 * fix memory leak with oidlist
 *
 * Revision 6.182  2000/03/02 21:24:16  shavirin
 * Checked for SEQLOC_PACKED_INT in blastMergeFilterLocs()
 *
 * Revision 6.181  2000/03/02 18:30:46  dondosha
 * Minor bug fix in BlastSaveCurrentHsp for blastn
 *
 * Revision 6.180  2000/03/02 17:11:01  dondosha
 * Fixed bug with one strand search option for blastn
 *
 * Revision 6.179  2000/03/01 21:40:53  shavirin
 * Added code to filter lower-case character regions (except blastx and tblastx)
 *
 * Revision 6.178  2000/02/29 18:17:24  shavirin
 * Variable query_dna_mask changed to query_lcase_mask.
 *
 * Revision 6.177  2000/02/29 18:09:36  dondosha
 * Call BlastFillQueryOffsets in BLASTSetUpSearchEx
 *
 * Revision 6.176  2000/02/23 20:56:51  dondosha
 * Returning strand concatenation for blastn with bug fixes
 *
 * Revision 6.175  2000/02/18 15:30:36  shavirin
 * Added parameter query_dna_mask into options and parameters.
 *
 * Revision 6.174  2000/02/17 21:23:08  shavirin
 * Added parameter is_rps_blast.
 *
 * Revision 6.173  2000/02/17 19:02:08  shavirin
 * Removed all references to absolete theCacheSize variable.
 *
 * Revision 6.172  2000/02/17 18:29:02  shavirin
 * Added function DefineToFrame().
 *
 * Revision 6.171  2000/02/16 21:47:45  shavirin
 * Fixed memory leaks in the function BlastReapHitlistByEvalue ().
 *
 * Revision 6.170  2000/02/15 21:02:00  shavirin
 * Added support to filter DNA sequence in translated RPS Blast.
 *
 * Revision 6.169  2000/02/15 19:17:29  shavirin
 * Added filter_string to Parameters block.
 *
 * Revision 6.168  2000/02/14 16:15:40  madden
 * Revert to 6.166
 *
 * Revision 6.167  2000/02/11 20:41:46  dondosha
 * Search on two query strands concatenated in blastn
 *
 * Revision 6.166  2000/02/09 19:40:00  madden
 * Fix purify problems in link_hsps
 *
 * Revision 6.165  2000/02/09 19:35:36  madden
 * Changed GetGisFromFile to also read binary gilists
 *
 * Revision 6.164  2000/02/03 21:34:07  dondosha
 * Fixed bug in setting extra_bytes_needed
 *
 * Revision 6.163  2000/02/02 20:01:57  madden
 * Added LIBCALLBACK to a callback
 *
 * Revision 6.162  2000/02/02 18:21:31  madden
 * Joerg optimizations for link_hsps
 *
 * Revision 6.161  2000/02/02 16:56:23  dondosha
 * Do not call BlastSaveCurrentHitlist if handle_results callback set
 *
 * Revision 6.160  2000/02/02 15:05:42  dondosha
 * Removed call to ReapHitlistByContext, erroneously included in previous version
 *
 * Revision 6.159  2000/02/01 22:37:05  dondosha
 * Call the new routine BlastReapHitlistByContext only when greedy alignment option is set
 *
 * Revision 6.158  2000/02/01 21:47:04  dondosha
 * Added greedy basic gapped alignment option
 *
 * Revision 6.157  2000/01/14 15:17:13  madden
 * Set no_check_score in pbp
 *
 * Revision 6.156  2000/01/13 18:10:41  madden
 * Fix problem with incorrect stat values for blastn and missing hits
 *
 * Revision 6.155  2000/01/13 14:27:04  madden
 * Fixed other problem in BlastWordFinder_contig()
 *
 * Revision 6.154  2000/01/12 18:52:23  shavirin
 * Fixed lookup_pos in BlastWordFinder_contig().
 *
 * Revision 6.153  2000/01/11 18:36:25  shavirin
 * Added functions, those handle dynamic lookup table.
 *
 * Revision 6.152  2000/01/11 15:32:46  dondosha
 * Fixed memory leaks in opening shared header and sequence file memory maps
 *
 * Revision 6.151  2000/01/04 22:52:25  madden
 * Restored code for using real db size
 *
 * Revision 6.150  1999/12/31 14:23:18  egorov
 * Add support for using mixture of real and maks database with gi-list files:
 * 1. Change logic of creating rdfp list.
 * 2. BlastGetDbChunk gets real databases first, then masks.
 * 3. Propoper calculation of database sizes using alias files.
 * 4. Change to CommonIndex to support using of mask databases.
 * 5. Use correct gis in formated output (BlastGetAllowedGis()).
 * 6. Other small changes
 *
 * Revision 6.149  1999/12/29 19:03:59  shavirin
 * Relative pointers in BlastWordFinder_mh_contig() updated to 8 byte pointers
 *
 * Revision 6.148  1999/12/29 18:57:03  shavirin
 * Added possibility to use relative pointers in BlastWordFinder_mh_contig().
 *
 * Revision 6.147  1999/12/22 21:55:38  dondosha
 * Close header and sequence files when search is done
 *
 * Revision 6.146  1999/12/21 20:05:48  egorov
 * Change logic of generating mask file when we have a gi-list file,
 * real database and mask database.  In fact, this is a big bug fix.
 *
 * Revision 6.145  1999/12/16 19:17:34  egorov
 * Code cleanup
 *
 * Revision 6.144  1999/12/02 14:39:35  egorov
 * When both mask and gi_list are specified, do not overwrite calculated
 * number of sequences and database length with values from alias file.
 *
 * Revision 6.143  1999/11/30 19:00:49  madden
 * Added Nlm_SwapUint4 calls for the ordinal ID list
 *
 * Revision 6.142  1999/11/26 22:26:13  madden
 * Added BlastNT functions for nucl. extensions
 *
 * Revision 6.141  1999/11/24 21:43:35  madden
 * Added Nlm_SwapUint4 call to make database masks work with both big and small endian systems
 *
 * Revision 6.140  1999/11/12 20:57:39  shavirin
 * Added parameter use_best_align into BLAST_ParameterBlkPtr
 *
 * Revision 6.139  1999/10/27 21:33:00  madden
 * Use housekeeping threads only for larger sequences
 *
 * Revision 6.138  1999/10/26 20:45:18  madden
 * Add use_real_db_size option
 *
 * Revision 6.137  1999/10/19 17:41:20  madden
 * Ensure that ThreadJoin is called on every thread created
 *
 * Revision 6.136  1999/10/14 17:57:44  madden
 * Fix for database size set by user, remove ununsed variables
 *
 * Revision 6.135  1999/10/12 19:34:08  madden
 * Call MutexDestroy on callback_mutex
 *
 * Revision 6.134  1999/10/08 17:39:57  egorov
 * Store input gi list to pick up correct definition for redundant sequences
 *
 * Revision 6.133  1999/10/05 18:16:06  shavirin
 * Functions tick_proc and get_db_chunk were renamed and become public.
 *
 * Revision 6.132  1999/10/05 17:42:53  shavirin
 * Removed global variables from blast.c
 *
 * Revision 6.131  1999/10/01 21:07:12  shavirin
 * Chanded definition and adjusted function get_db_list().
 *
 * Revision 6.130  1999/09/28 20:14:32  madden
 * Joerg changes to mimize cache misses
 *
 * Revision 6.129  1999/09/22 21:54:08  egorov
 * remove debug info
 *
 * Revision 6.128  1999/09/22 21:03:55  egorov
 * Add mask DB stuff
 *
 * Revision 6.127  1999/09/16 16:54:23  madden
 * Changes to BlastNtWordFinder for long words
 *
 * Revision 6.126  1999/09/16 14:16:54  madden
 * Changed call to lookup_find_init
 *
 * Revision 6.125  1999/08/27 18:07:32  shavirin
 * Passed parameter decline_align from top to the engine.
 *
 * Revision 6.124  1999/08/26 14:55:15  madden
 * Fixed Int8 problem
 *
 * Revision 6.123  1999/08/25 13:11:16  madden
 * Roll back to rev 6.121
 *
 * Revision 6.121  1999/08/20 19:47:24  madden
 * Changed call to BlastSearchBlkNew(Extra), removed use of version array
 *
 * Revision 6.120  1999/08/06 18:46:13  madden
 * Fixed spelling of incompatible
 *
 * Revision 6.119  1999/06/07 18:28:20  beloslyu
 * NetBSD port
 *
 * Revision 6.118  1999/05/27 17:33:04  madden
 * Fixed Int2 (should have been Int4) problem
 *
 * Revision 6.117  1999/04/28 13:30:03  madden
 * Use BlastConstructErrorMessage for error messages
 *
 * Revision 6.116  1999/04/23 16:45:53  madden
 * call BQ_IncSemaphore as callback
 *
 * Revision 6.115  1999/04/22 16:45:29  shavirin
 * Added load-ballancing function.
 *
 * Revision 6.114  1999/04/13 16:39:14  madden
 * Fixed problem if first context not plus strand
 *
 * Revision 6.113  1999/04/07 20:43:33  egorov
 * Fix a bug when ordinal_id == 0 was not allowed
 *
 * Revision 6.112  1999/04/01 21:42:45  madden
 * Fix memory leaks when gi list is used
 *
 * Revision 6.111  1999/03/23 21:38:19  madden
 * Add Join to BlastStopAwakeThread
 *
 * Revision 6.110  1999/03/19 17:03:29  egorov
 * Initialize global variable
 *
 * Revision 6.109  1999/03/16 15:52:25  vakatov
 * Got rid of extra comments-within-comments in the CVS Log section
 *
 * Revision 6.108  1999/03/16 02:49:31  beloslyu
 * typo fixed
 *
 * Revision 6.107  1999/03/15 22:06:01  madden
 * Changed cpu limit message
 *
 * Revision 6.106  1999/03/12 15:03:43  egorov
 * Add proper Int4-long type casting
 *
 * Revision 6.105  1999/03/04 14:18:08  egorov
 * Do correct filter masking when query is seqloc
 * The only BlastMaskTheResidues() function is changed:
 *
 * Revision 6.104  1999/02/26 22:23:06  madden
 * Fixed bug when only one HSP allowed per area
 *
 * Revision 6.103  1999/02/25 17:40:48  madden
 * Check that proper sequence type is used in setup function
 *
 * Revision 6.102  1999/02/17 13:23:00  madden
 * Added hsp_num_max
 *
 * Revision 6.101  1999/02/11 13:52:59  madden
 * fixed memory leak
 *
 * Revision 6.100  1999/01/28 17:19:50  madden
 * Call BlastSeqLocFilterEx on reverse strand if plus strand NULL
 *
 * Revision 6.99  1999/01/28 16:04:25  madden
 * HspArrayPurge change, HeapSort of HSPs, efficiency in blastn wordfinder
 *
 * Revision 6.98  1999/01/26 17:55:50  madden
 * start set to last_db_seq
 *
 * Revision 6.97  1999/01/19 13:32:33  madden
 * Fix for final db sequence to search
 *
 * Revision 6.96  1998/12/31 18:17:02  madden
 * Added strand option
 *
 * Revision 6.95  1998/12/31 15:36:05  victorov
 * filtering internals is now based on SeqLoc instead of Bioseq
 *
 * Revision 6.94  1998/12/29 17:44:43  madden
 * Add BlastGetNonSumStatsEvalue, optimizations for NtWordFinder
 *
 * Revision 6.93  1998/12/18 16:19:57  madden
 * Make BLASTSetUpSearchWithReadDbInternal public, add BlastSearchBlkNewExtra
 *
 * Revision 6.92  1998/12/17 22:29:47  victorov
 * the way gifile is found has changed: now we look first in the
 * current directory then $BLASTDB and then in ncbirc
 *
 * Revision 6.91  1998/12/15 14:11:27  madden
 * Change to permit an arbitrary number of HSPs
 *
 * Revision 6.90  1998/11/27 15:44:58  madden
 * Ensure that gap_x_dropoff_final is at least as large as gap_x_dropoff.
 *
 * Revision 6.89  1998/11/23 13:36:07  madden
 * Check for non-NULL tick_callback before acquiring mutex
 *
 * Revision 6.88  1998/11/19 14:03:24  madden
 * Added comments, minor efficiency
 *
 * Revision 6.87  1998/10/13 20:37:51  madden
 * Use IS_residue after call to SeqPortGetResidue
 *
 * Revision 6.86  1998/09/24 15:26:34  egorov
 * Fix lint complaints
 *
 * Revision 6.85  1998/09/22 16:28:03  madden
 * Added call to lookup_position_aux_destruct
 *
 * Revision 6.84  1998/09/14 15:11:12  egorov
 * Add support for Int8 length databases; remove unused variables
 *
 * Revision 6.83  1998/09/04 14:45:39  madden
 * Moved code from blast.c blastool.c
 *
 * Revision 6.82  1998/08/29 20:06:46  madden
 * Do not find words for pattern search
 *
 * Revision 6.81  1998/08/26 19:20:26  madden
 * Added SignalIgnore
 *
 * Revision 6.80  1998/08/13 20:00:20  egorov
 * Add check if gilist file exists on server
 *
 * Revision 6.79  1998/08/11 13:27:22  madden
 * Fix to small function for culling
 *
 * Revision 6.78  1998/08/05 13:08:16  madden
 * Removed obsolete global_rdfp
 *
 * Revision 6.77  1998/07/30 19:00:24  madden
 * Change to allow search of subset of database
 *
 * Revision 6.76  1998/07/28 21:17:45  madden
 * added do_not_reevaluate and mask_at_hash
 *
 * Revision 6.75  1998/07/25 14:26:39  madden
 * Added comments
 *
 * Revision 6.74  1998/07/22 20:31:25  madden
 * Added comments
 *
 * Revision 6.73  1998/07/22 12:16:23  madden
 * Added handle_results
 *
 * Revision 6.72  1998/07/21 20:58:01  madden
 * Changes to allow masking at hash only
 *
 * Revision 6.71  1998/07/17 15:39:53  madden
 * Changes for Effective search space.
 *
 * Revision 6.70  1998/07/14 20:14:37  egorov
 * Allow to specify gilist and gifile from client side
 *
 * Revision 6.69  1998/07/09 14:39:04  madden
 * Fix memory leak
 *
 * Revision 6.68  1998/07/02 21:00:36  egorov
 * Remove memory leak in threaded version
 *
 * Revision 6.67  1998/06/25 13:14:48  madden
 * check for NULL pointer in BlastPossibleDeleteWholeHeap
 *
 * Revision 6.66  1998/06/12 16:07:40  madden
 * Fixed typo
 *
 * Revision 6.65  1998/06/12 15:52:52  madden
 * Fixed warnings
 *
 * Revision 6.64  1998/06/02 21:21:18  madden
 * Changes for DNA matrices
 *
 * Revision 6.63  1998/06/02 13:10:14  madden
 * Fixed increment problem in for loop
 *
 * Revision 6.62  1998/05/28 19:58:48  madden
 * Zhengs new culling code
 *
 * Revision 6.61  1998/05/22 20:19:51  madden
 * Changes to fix multi-db search bug
 *
 * Revision 6.60  1998/05/17 16:28:39  madden
 * Allow changes to filter options and cc filtering.
 *
 * Revision 6.59  1998/05/05 14:05:32  madden
 * Added functions BlastStartAwakeThread and BlastStopAwakeThread
 *
 * Revision 6.58  1998/04/24 21:51:12  madden
 * Check return value on BlastScoreBlkFill
 *
 * Revision 6.57  1998/04/24 19:26:47  madden
 * Allocate ideal Karlin-Blk
 *
 * Revision 6.56  1998/04/15 20:23:47  madden
 * offset arg removed from BlastMaskTheResidues
 *
 * Revision 6.55  1998/04/01 22:46:55  madden
 * Set query_invalid flag when there is no valid sequence
 *
 * Revision 6.54  1998/03/27 01:39:08  madden
 * Check for non-zero subject length in link_hsps
 *
 * Revision 6.53  1998/03/25 22:26:46  madden
 * Use NlmThreadCreateEx
 *
 * Revision 6.52  1998/03/24 15:38:20  madden
 * Use BlastDoubleInt4Ptr to keep track of gis and ordinal_ids
 *
 * Revision 6.51  1998/03/19 22:16:18  madden
 * Changes to allow blasting by gi list
 *
 * Revision 6.50  1998/03/18 14:14:05  madden
 * Support random access by gi list
 *
 * Revision 6.49  1998/03/14 18:29:16  madden
 * Added BlastSeqIdListPtr
 *
 * Revision 6.48  1998/03/09 22:14:39  madden
 * Set seqid_list to NULL for child threads
 *
 * Revision 6.47  1998/02/27 14:34:26  madden
 * Added missing return value
 *
 * Revision 6.46  1998/02/26 22:35:00  madden
 * Added return value to link_hsp
 *
 * Revision 6.45  1998/02/26 19:08:07  madden
 *  Removed BlastNtFindWords BlastPopulateAllWordArrays BlastFindWords and BlastNewFindWords
 *
 * Revision 6.44  1998/02/26 16:56:02  madden
 * Fix for flyblast type searches
 *
 * Revision 6.43  1998/02/24 22:46:00  madden
 * Added option to shutdown culling
 *
 * Revision 6.42  1998/02/19 22:57:20  madden
 * Correctly set multiple_hits flag in BlastSetUpSearchEx
 *
 * Revision 6.41  1998/02/02 21:42:17  madden
 * link_hsps returns first BLAST_HSPPtr in list
 *
 * Revision 6.40  1998/01/31 21:33:49  madden
 * Fix to ensure hits are ranked properly
 *
 * Revision 6.39  1998/01/27 20:33:19  madden
 * Adjustments for query and db lengths
 *
 * Revision 6.38  1998/01/23 22:01:49  madden
 * Effective query length fixes for short sequences
 *
 * Revision 6.37  1998/01/15 19:30:31  madden
 * Protection against crashes for short sequences
 *
 * Revision 6.36  1998/01/09 22:30:06  madden
 * Fix for range-dependent BLAST with short sequences
 *
 * Revision 6.35  1998/01/07 23:04:25  madden
 * Added mutex for callbacks
 *
 * Revision 6.34  1998/01/06 18:25:24  madden
 * Save query_slp
 *
 * Revision 6.33  1998/01/05 22:37:34  madden
 * Check that options->multiple_hits_only is set before using multiple_hits
 *
 * Revision 6.32  1998/01/05 21:14:51  madden
 * Added protection against NULL LookupTablePtr and BLAST_WordFinderPtr
 *
 * Revision 6.31  1998/01/05 16:46:46  madden
 * One or both strands can be searched, as opposed to only both, changes to number of contexts
 *
 * Revision 6.30  1997/12/31 19:46:40  madden
 * Optimization of database scanning loop
 *
 * Revision 6.29  1997/12/31 17:50:42  madden
 * Added function BlastNtWordFinder_mh
 *
 * Revision 6.28  1997/12/29 16:15:01  madden
 * Optimizations for BlastNtWordFinder
 *
 * Revision 6.27  1997/12/24 19:42:57  madden
 * Fix for cell dependent blast
 *
 * Revision 6.26  1997/12/23 19:13:36  madden
 * Removed flags parameter from NlmThreadCreate
 *
 * Revision 6.25  1997/12/23 18:11:51  madden
 * Changes for range-dependent blast
 *
 * Revision 6.24  1997/12/17 19:25:36  madden
 * replace THR_BOUND with THREAD_BOUND
 *
 * Revision 6.23  1997/12/11 22:19:49  madden
 * Removed unused variables and function
 *
 * Revision 6.22  1997/12/10 22:40:28  madden
 * Floats used in call to blast_set_parameters, use of defines rather than strings
 *
 * Revision 6.21  1997/12/08 21:56:25  madden
 * Check for queries without valid sequences
 *
 * Revision 6.20  1997/12/04 21:49:05  madden
 * Check for NULL returned by BioseqLockById
 *
 * Revision 6.19  1997/11/07 21:38:40  madden
 * Check for virtual Bioseqs
 *
 * Revision 6.18  1997/10/30 15:40:55  madden
 * Casts and fixes for DEC alpha
 *
 * Revision 6.17  1997/10/24 19:09:14  madden
 * Removed BlastSetReadDB and BlastGetReadDB_ID, changed to ReadDBGetDb and ReadDBGetDbId
 *
 * Revision 6.16  1997/10/21 19:49:53  madden
 * Fix for no valid query sequence and hitlist_max of 1
 *
 * Revision 6.15  1997/10/06 17:57:49  madden
 * DB chunk size now done properly
 *
 * Revision 6.14  1997/09/29 17:19:30  madden
 * Checks for two threads using the same resource
 *
 * Revision 6.13  1997/09/25 13:44:56  madden
 * tblastn fix for mutliple db searches
 *
 * Revision 6.12  1997/09/24 22:36:29  madden
 * Fixes for MT multidb searches
 *
 * Revision 6.11  1997/09/22 18:24:25  madden
 * Added ifdef for OS_UNIX_LINUX
 *
 * Revision 6.10  1997/09/22 17:36:18  madden
 * MACROS for position-specific matrices from Andy Neuwald
 *
 * Revision 6.9  1997/09/16 18:47:44  madden
 * ifdef for OS_UNIX_SUN
 *
 * Revision 6.8  1997/09/16 16:31:22  madden
 * More changes for multiple db runs
 *
 * Revision 6.7  1997/09/15 22:07:19  madden
 * Replacing ifdef RLIMIT_CPU with ifdef OS_UNIX
 *
 * Revision 6.6  1997/09/12 19:56:53  madden
 * Fix for multi-threaded runs
 *
 * Revision 6.5  1997/09/11 18:49:20  madden
 * Changes to enable searches against multiple databases.
 *
 * Revision 6.4  1997/09/10 23:10:53  kans
 * added ifdef RLIMIT_CPU for signal and headers
 *
 * Revision 6.3  1997/09/10 21:27:52  madden
 * Changes to set CPU limits
 *
 * Revision 6.2  1997/09/03 19:06:02  madden
 * Bug fix for effective HSP longer than query
 *
 * Revision 6.1  1997/08/27 14:46:43  madden
 * Changes to enable multiple DB searches
 *
 * Revision 6.0  1997/08/25 18:52:19  madden
 * Revision changed to 6.0
 *
 * Revision 1.227  1997/08/19 18:19:16  madden
 * Cast arg of log to Nlm_FloatHi
 *
 * Revision 1.226  1997/08/12 20:50:28  madden
 * Fixed case where two HSPs start at same query offset
 *
 * Revision 1.225  1997/07/29 17:07:01  madden
 * Fix for possible collision of two star threads
 *
 * Revision 1.224  1997/07/25 15:39:27  madden
 * Set correct query ID for filtering
 *
 * Revision 1.223  1997/07/24 21:08:31  madden
 * Take frame into account in sorting of hits for linking
 *
 * Revision 1.222  1997/07/22 17:17:23  madden
 * Added index callback
 *
 * Revision 1.221  1997/07/17 20:27:51  madden
 * Set choice to indicat frame when masking seqLoc is saved
 *
 * Revision 1.220  1997/07/16 20:35:11  madden
 * Call to BlastConvertProteinSeqLoc
 *
 * Revision 1.219  1997/07/16 18:51:55  madden
 * call to BioseqSeg, added static function BlastMakeTempProteinBioseq
 *
 * Revision 1.218  1997/07/15 20:37:05  madden
 * Calls to SeqLocSeg and BioseqSeg
 *
 * Revision 1.217  1997/07/14 20:11:03  madden
 * Removed unused variables
 *
 * Revision 1.216  1997/07/14 15:30:46  madden
 * Changed call to BlastKarlinBlkGappedCalc
 *
 * Revision 1.215  1997/07/11 19:28:23  madden
 * Added function BLASTSetUpSearchByLocWithReadDb
 *
 * Revision 1.214  1997/07/01 17:50:52  madden
 * used gapped Karlin-Altschul parameters when needed in LinkHsp
 *
 * Revision 1.213  1997/06/27 22:18:31  madden
 * MT fix for more threads than db seqs.
 *
 * Revision 1.212  1997/06/24 13:51:20  madden
 * Fixed SeqLoc leak
 *
 * Revision 1.211  1997/05/27 20:19:17  madden
 * Use of SeqLocDust rather than BioseqDust
 *
 * Revision 1.210  1997/05/22 21:24:46  madden
 * Added support for final gapX dropoff value
 *
 * Revision 1.209  1997/05/20 17:49:55  madden
 * Added functions BLASTSetUpSearchByLoc and BLASTSetUpSearchInternalByLoc
 *
 * Revision 1.208  1997/05/07 20:59:13  madden
 * Call to SeqId2OrdinalId replaces call to readdb_gi2seq
 *
 * Revision 1.207  1997/05/07 13:45:08  madden
 * Set mutex lock for ambiguity reevaluation, added use_large_gaps flag
 *
 * Revision 1.206  1997/05/01  21:08:26  madden
 * use ordinal index to rank results when they are statist. equivalent
 *
 * Revision 1.205  1997/05/01  15:53:07  madden
 * Addition of extra KarlinBlk's for psi-blast
 *
 * Revision 1.204  1997/04/25  13:57:43  madden
 * Fixed floating point exception by checking for zero query length value.
 *
 * Revision 1.203  1997/04/23  21:56:07  madden
 * Changes in BlastGetGappedAlignmentTraceback for in-frame gapping tblastn.
 *
 * Revision 1.202  1997/04/22  14:00:14  madden
 * Removed unused variables.
 *
 * Revision 1.201  1997/04/22  13:04:19  madden
 * Changes for in-frame blastx gapping.
 *
 * Revision 1.200  1997/04/17  22:07:48  madden
 * Changes to allow in-frame gapped tblastn.
 *
 * Revision 1.199  1997/04/09  20:01:53  madden
 * Added global_seqid's to allow only certain sequences in a db to be searched.
 *
 * Revision 1.198  1997/04/07  18:17:09  madden
 * Changed length_adjustment calculation.
 *
 * Revision 1.197  1997/04/04  15:30:37  madden
 * Removed extra fprint statement.
 *
 * Revision 1.196  1997/04/03  19:48:13  madden
 * Changes to use effective database length instead of the length of each
 * sequence in statistical calculations.
 *
 * Revision 1.195  1997/03/27  22:30:51  madden
 * Used gapped Karlin-Altschul parameters to calculate trigger for gapping.
 *
 * Revision 1.194  1997/03/20  22:09:52  madden
 * Used SeqIdFindBest to find GI in query.
 *
 * Revision 1.193  1997/03/20  19:57:40  madden
 * Changes to support segmented Bioseq queries.
 *
 * Revision 1.192  1997/03/14  22:06:11  madden
 * fixed MT bug in BlastReevaluateWithAmbiguities.
 *
 * Revision 1.191  1997/03/08  16:52:16  madden
 * Check in Reevaluate function to see if sequence is worth checking,
 * Added discontinuous option to ParameterBlk.
 *
 * Revision 1.190  1997/03/07  21:58:36  madden
 * Added Boolean gapped argument to BLASTOptionNew.
 *
 * Revision 1.189  1997/03/07  21:11:22  madden
 * Added in check for blastn on gapped calculations.
 *
 * Revision 1.188  1997/03/05  14:29:46  madden
 * Moved BlastSaveCurrentHsp to blastutl.c.
 *
 * Revision 1.187  1997/03/04  21:34:59  madden
 * Added in HspArrayPurge.
 *
 * Revision 1.186  1997/03/04  20:08:19  madden
 * Moved gapped alignment code from blast.c to blastutl.c
 *
 * Revision 1.185  1997/03/03  22:39:45  madden
 * Moved code from blast.c to blastutl.c.
 *
 * Revision 1.184  1997/03/03  21:47:22  madden
 * Moved functions from blast.c to blastutl.c for 16-bit windows.
 *
 * Revision 1.183  1997/03/03  20:58:09  madden
 * Fixed call to BlastGetGappedAlignmentTraceback; purged hitlist
 * for very short database sequences.
 *
 * Revision 1.182  1997/03/01  18:25:33  madden
 * reverse flag added to BlastGetGappedAlignmentTraceback functions.
 *
 * Revision 1.181  1997/02/24  16:40:38  madden
 * Change to GapXEditBlockToSeqAlign to use first SeqIdPtr, duplicate.
 *
 * Revision 1.180  1997/02/24  15:09:38  madden
 * Fixed bug where NULL pointer was dereferenced.
 *
 * Revision 1.179  1997/02/24  13:10:27  madden
 * Added function BlastGappedScoreInternal.
 *
 * Revision 1.178  1997/02/23  16:44:47  madden
 * GapAlignBlk became GapAlignBlkPtr and GapAlignBlkNew called.
 *
 * Revision 1.177  1997/02/20  21:50:24  madden
 * Added frame and translation information to GapAlignBlk, assigned it.
 *
 * Revision 1.176  1997/02/20  18:38:34  madden
 * Allowed theoretical database length to be set.
 *
 * Revision 1.175  1997/02/19  22:29:32  madden
 * Changes to handle multiple contexts in BlastGetGappedScore.
 *
 * Revision 1.174  1997/02/19  14:17:03  madden
 * GappedScore routines now work on all contexts.
 *
 * Revision 1.173  1997/02/17  17:39:54  madden
 * Changes to RealBlastGetGappedAlignmentTraceback for gapped blastn.
 *
 * Revision 1.172  1997/02/13  21:04:15  madden
 * fixed UMR.
 *
 * Revision 1.171  1997/02/12  22:19:08  madden
 * Added functions BlastNewWordExtend, BlastNewWordExtend_prelim, and
 * BlastNewFindWords for use in position based blast.
 *
 * Revision 1.170  1997/02/11  19:29:34  madden
 * Addition of BlastGetGappedScoreWithReaddb, removed dependence of
 * BlastGetGappedScore on readdb.
 *
 * Revision 1.169  1997/02/10  20:27:01  madden
 * Changed some CharPtr's into Uint1Ptr's.
 *
 * Revision 1.168  1997/02/10  20:14:23  madden
 * replaced doubles by Nlm_FloatHi's.
 *
 * Revision 1.167  1997/02/10  20:02:58  madden
 * Changed BlastSearchBlkNew to allow a set of words to be passed in.
 *
 * Revision 1.166  1997/02/10  15:24:59  madden
 * Set posMatrix element in gap_align structure.
 *
 * Revision 1.165  1997/02/07  22:43:03  madden
 * Moved BLAST_WordFinderNew and Destruct from blast.c to blastutl.c, made
 * non-static.
 *
 * Revision 1.164  1997/02/07  22:32:40  madden
 * Moved BlastGetSubjectId to blastutl.c, changed calling convention of
 * BlastGetSubjectId.
 *
 * Revision 1.163  1997/02/06  15:36:14  madden
 * Resuse 1st threshold if necessary.
 *
 * Revision 1.162  1997/02/06  14:27:15  madden
 * Addition of BlastAllWord structure.
 *
 * Revision 1.161  1997/02/05  19:54:59  madden
 * Changes for blastn gapped alignments.
 *
 * Revision 1.160  1997/02/04  22:12:59  madden
 * Added function RealBlastGetGappedAlignmentTraceback.
 *
 * Revision 1.159  1997/02/04  20:11:42  madden
 * Moved functions to blastutl.c
 *
 * Revision 1.158  1997/02/04  16:22:32  madden
 * Changes to enable gapped alignments on the reverse strand.
 *
 * Revision 1.157  1997/02/03  19:24:01  madden
 * Added function CheckGappedAlignmentsForOverlap.
 *
 * Revision 1.156  1997/02/03  17:19:03  madden
 * Increased number of bits for second pass if context factor > 1.
 *
 * Revision 1.155  1997/02/03  13:02:12  madden
 * Corrected SeqAlign offsets for minus strands.
 *
 * Revision 1.154  1997/01/31  22:42:51  madden
 * changed default thresholds and added strands to construction of SeqAlign.s
 *
 * Revision 1.153  1997/01/31  22:13:02  madden
 * Adjusted bit score by logK.
 *
 * Revision 1.152  1997/01/31  14:45:27  madden
 * Added check for threshold value to ValidateOptions.
 *
 * Revision 1.151  1997/01/30  19:12:19  madden
 * Fixed memory leak.
 *
 * Revision 1.150  1997/01/28  22:38:56  madden
 * Added function BLASTOptionValidate.
 *
 * Revision 1.149  1997/01/28  21:50:05  madden
 * Adjustments to CopyResultHspToHSP.
 *
 * Revision 1.148  1997/01/24  16:51:44  madden
 * Fixed memory leak.
 *
 * Revision 1.147  1997/01/24  15:13:02  madden
 * Changes to accommodate gapped blastn.
 *
 * Revision 1.146  1997/01/22  17:45:08  madden
 * Added search to GetStartForGappedAlignment.
 *
 * Revision 1.145  1997/01/17  17:41:44  madden
 * Added flags for position based BLAST.
 *
 * Revision 1.144  1997/01/14  17:22:30  madden
 * Changes for MT, especially for small databases.
 *
 * Revision 1.143  1997/01/13  22:13:41  madden
 * set further_process to FALSE as needed.
 *
 * Revision 1.142  1997/01/13  20:06:36  madden
 * Added index_addition to strings before checking for ambiguties.
 *
 * Revision 1.141  1997/01/13  15:37:05  madden
 * Changed prototypes for star_callback and tick_callback.
 *
 * Revision 1.140  1997/01/11  18:58:29  madden
 * Removed defunct PerformBlastSearch... functions.
 *
 * Revision 1.139  1997/01/11  18:39:48  madden
 * Simplified ranged blast model.
 *
 * Revision 1.138  1997/01/11  18:22:10  madden
 * Changes to allow S2 to be set.
 *
 * Revision 1.137  1997/01/11  16:41:42  madden
 * Fix to tick_proc for MT runs.
 *
 * Revision 1.136  1997/01/09  17:44:35  madden
 * Added "bit_score" to BLASTResultHsp.
 *
 * Revision 1.135  1997/01/09  13:33:43  madden
 * Fixed NlmThreadCompare typo.
 *
 * Revision 1.134  1997/01/08  23:05:37  madden
 * Added call to TNlmThreadCompare.
 *
 * Revision 1.133  1997/01/07  20:40:29  madden
 * Added reverse Boolean to GetSeqAlignForResultHitList.
 *
 * Revision 1.132  1997/01/06  22:40:55  madden
 * Added function BlastGetSubjectId.
 *
 * Revision 1.131  1997/01/06  19:31:49  madden
 * Removed subject and query ID from GapAlignBlk.
 *
 * Revision 1.130  1997/01/06  17:22:59  madden
 * Used GapXEditScriptToSeqAlign to find SeqAlign.
 *
 * Revision 1.129  1997/01/04  20:41:11  madden
 * Shorter sequence is always the query in BlastTwoSequences.
 *
 * Revision 1.128  1997/01/03  20:29:32  madden
 * Corrected count of significant sequences.
 *
 * Revision 1.127  1997/01/03  19:03:35  madden
 * Fixed incorrect KarlinBlkPtr use.
 *
 * Revision 1.126  1997/01/03  17:26:50  madden
 * Fixed stats recordation.
 *
 * Revision 1.125  1996/12/30  21:45:28  madden
 * Added "strict" Boolean to CheckForRequiredRegion.
 *
 * Revision 1.124  1996/12/30  17:14:06  madden
 * Fixes for changes for "require a portion of the query sequence".
 *
 * Revision 1.123  1996/12/30  15:44:25  madden
 * Added capability to require a portion of the query sequence.
 *
 * Revision 1.122  1996/12/27  20:44:10  madden
 * Chnages to require that part of the query be included.
 *
 * Revision 1.121  1996/12/23  22:02:05  madden
 * Changes to allow two sequences to be compared.
 *
 * Revision 1.120  1996/12/23  15:57:21  madden
 * Removed extra call to BlastPreliminaryGappedScore.
 * y
 *
 * Revision 1.119  1996/12/23  14:04:44  madden
 * Added gap_trigger.
 *
 * Revision 1.118  1996/12/20  21:11:40  madden
 * Changes to allow multiple hits runs only.
 *
 * Revision 1.117  1996/12/20  15:31:05  madden
 * Removed defunct function.
 *
 * Revision 1.116  1996/12/20  14:22:48  madden
 * Added discontinuous Boolean to GetSeqAlignForResultHitList.
 *
 * Revision 1.115  1996/12/18  14:33:13  madden
 * Checked for high score when E-values are equivalent.
 *
 * Revision 1.114  1996/12/17  18:28:10  madden
 * Changed score used to gap HSP's.
 *
 * Revision 1.113  1996/12/17  17:28:27  madden
 * Removed sleep function for non-UNIX platforms.
 *
 * Revision 1.112  1996/12/17  17:27:03  madden
 * Count number of attempted gappings.
 *
 * Revision 1.111  1996/12/17  13:47:57  madden
 * Added star_proc.
 *
 * Revision 1.110  1996/12/16  19:24:38  madden
 * Correct to initial wordsize for blastn.
 *
 * Revision 1.109  1996/12/16  18:24:21  madden
 * Corrected shift in BlastNtFindWords.
 *
 * Revision 1.108  1996/12/16  15:29:12  madden
 * Changed gapalign.h to gapxdrop.h
 *
 * Revision 1.107  1996/12/16  14:35:48  madden
 * Replaced BLAST_GAPPED_OPTION ifdef with gapped_calculation Boolean.
 *
 * Revision 1.106  1996/12/13  22:00:23  madden
 * Corrected starting point for gapped extension with traceback.
 *
 * Revision 1.105  1996/12/13  18:13:56  madden
 * Added tick callback functions
 *
 * Revision 1.104  1996/12/13  15:09:31  madden
 * Changes to parameters used for gapped extensions.
 *
 * Revision 1.103  1996/12/12  16:44:35  madden
 * Removed unused variables.
 *
 * Revision 1.102  1996/12/12  16:34:58  madden
 * GapAlignBlk replaces arguments in PerformGappedAlignment etc.
 *
 * Revision 1.101  1996/12/12  14:04:03  madden
 * Fixes for check on whether HSP is already contained by gapped alignment.
 *
 * Revision 1.100  1996/12/10  19:20:15  madden
 * Changed minimal HSP score for gapped alignments.
 *
 * Revision 1.99  1996/12/10  17:30:59  madden
 * Changed statistics for gapped blastp
 *
 * Revision 1.98  1996/12/09  23:24:05  madden
 * Added parameters to control which sequences get a gapped alignment.
 *
 * Revision 1.97  1996/12/09  20:45:47  madden
 * Adjustments to calculation of gapped HSP's.
 *
 * Revision 1.96  1996/12/08  15:19:59  madden
 * Added functions to enable gapped alignments.
 *
 * Revision 1.95  1996/11/27  22:46:08  madden
 * Removed includes that are no longer used.
 *
 * Revision 1.94  1996/11/27  22:25:09  madden
 * Corrected collection of statistics for MT runs.
 *
 * Revision 1.93  1996/11/27  21:52:30  madden
 * Added function FilterWithSeg.
 *
 * Revision 1.92  1996/11/26  19:53:46  madden
 * Checked for return value on BlastScoreBlkMatFill.
 *
 * Revision 1.91  1996/11/25  20:13:47  madden
 * Changed how NlmMutexInit is called.
 *
 * Revision 1.90  1996/11/25  19:51:41  madden
 * Fix for tblastx stats.
 *
 * Revision 1.89  1996/11/25  18:58:24  madden
 * Adjustments for translated database.
 *
 * Revision 1.88  1996/11/22  19:04:58  madden
 * Removed ifdef for OLD_BIT_ORDER; changed default values.
 *
 * Revision 1.87  1996/11/22  15:28:03  madden
 * Fixed problem of last query residue examined on a diagonal.
 *
 * Revision 1.86  1996/11/21  18:08:38  madden
 * Changed order of if-else statements in get_db_chunk for
 * possible improvement of parallelization.
 *
 * Revision 1.85  1996/11/20  23:15:50  madden
 * Changes to acquisition of Mutex in BlastSaveCurrentHitlist to
 * improve parallelization.
 *
 * Revision 1.84  1996/11/19  22:23:52  madden
 * Changed link_hsps to link HSP's faster.
 *
 * Revision 1.83  1996/11/18  19:32:09  madden
 * Removed unused variables found by CodeWarrior.
 *
 * Revision 1.82  1996/11/18  18:07:57  madden
 * Duplicated translation_buffer (for tblast[nx]).
 *
 * Revision 1.81  1996/11/18  17:28:13  madden
 * Duplicated translation information in BlastSearchBlkDuplicate and
 * also number of contexts.
 *
 * Revision 1.80  1996/11/18  15:45:40  madden
 * FilterDNA function to perform dusting added (by Sergei Shavirin).
 *
 * Revision 1.79  1996/11/15  17:54:54  madden
 * Added support for alternate genetic codes for blastx, tblast[nx].
 *
 * Revision 1.78  1996/11/14  16:37:58  madden
 * Put average lengths in defines.
 *
 * Revision 1.77  1996/11/14  16:21:55  madden
 * changed CharPtr to Uint1Ptr in GetTranslation.
 *
 * Revision 1.76  1996/11/13  22:35:18  madden
 * Added tblast[nx] capability to BlastReevaluateWithAmbiguities.
 *
 * Revision 1.75  1996/11/12  19:56:35  madden
 * Small gaps not considered for blastn.
 *
 * Revision 1.74  1996/11/12  16:21:17  madden
 * Added in context_factor.
 *
 * Revision 1.73  1996/11/12  13:46:15  madden
 * Removed defunct SetUpBlastSearch type functions.
 *
 * Revision 1.72  1996/11/11  17:44:21  madden
 * Fixed check for overlap in search.
 *
 * Revision 1.71  1996/11/09  21:02:59  madden
 * Fixes for blastn extensions.
 *
 * Revision 1.70  1996/11/08  21:45:03  madden
 * Fix for blastn extensions.
 *
 * Revision 1.69  1996/11/07  22:31:15  madden
 * Added function BlastReevaluateWithAmbiguities for nucl. db's.
 *
 * Revision 1.68  1996/11/07  17:31:26  madden
 * Fixed over-incrementing of index in link_hsps.
 *
 * Revision 1.67  1996/11/06  22:10:01  madden
 * Further optimization of BlastTranslateUnambiguousSequence.
 *
 * Revision 1.66  1996/11/05  23:19:08  madden
 * Rewrote BlastTranslateUnambiguousSequence so it's faster.
 *
 * Revision 1.65  1996/11/04  19:27:13  madden
 * Deallocated search->translation_buffer if allocated.
 *
 * Revision 1.64  1996/11/04  16:59:43  madden
 * Added function GetPrivatTranslationTable to optimize translation
 * of database.
 *
 * Revision 1.63  1996/11/01  21:06:49  madden
 * Corrected the (nucl.) database for the translated length for tblast[nx].
 *
 * Revision 1.62  1996/10/31  16:27:20  shavirin
 * Multiple changes due to reverce of residues in BLAST database
 * for nucleotide sequences from (4321) to (1234)
 * New dumper now required to create BLAST databases.
 *
 * Revision 1.61  1996/10/28  22:15:24  madden
 * Added check in BlastNtWordFinder that subject sequence is longet
 * than min. word size.
 *
 * Revision 1.60  1996/10/04  20:12:26  madden
 * Fixed memory leaks found by purify.
 *
 * Revision 1.59  1996/10/03  20:49:29  madden
 * Calculate standard Karlin parameters for blastx and tblastx,
 * Use proper Karlin parameters in linking of HSP's.
 *
 * Revision 1.58  1996/10/02  19:59:44  madden
 * Fixed translation of query in blastx, calculated different karlin parameters
 * for each frame.
 *
 * Revision 1.57  1996/10/01  21:24:02  madden
 * e2 value now depends on program, correct cutoffs for blastn.
 *
 * Revision 1.56  1996/10/01  18:49:06  madden
 * Properly placed counters for number of hits, extensions.
 *
 * Revision 1.55  1996/09/30  21:56:12  madden
 * Replaced query alphabet of ncbi2na with blastna alphabet.
 *
 * Revision 1.54  1996/09/26  21:48:29  madden
 * Set small/large gaps in SeqALign.
 *
 * Revision 1.53  1996/09/26  20:18:43  madden
 * Addition of ExperimentalLocalBlastSearch function, fixes to SeqIdPtr's.
 *
 * Revision 1.52  1996/09/25  19:05:24  madden
 * Fixes to nucl. extension functions.
 *
 * Revision 1.51  1996/09/25  14:31:06  madden
 * Removed functions and statements for discontiguous word hits.
 *
 * Revision 1.50  1996/09/24  22:13:06  madden
 * BlastNtWordExtend now extends properly to end of query or subject.
 *
 * Revision 1.49  1996/09/24  18:39:51  madden
 * Changes to extend into the remainder of nucl. sequences (for blastn) and
 * to perform minus strand extensions.
 *
 * Revision 1.48  1996/09/20  21:58:14  madden
 * Changed CharPtr's to Uint1Ptr, got remainder length out of top order bits.
 *
 * Revision 1.47  1996/09/19  13:46:29  madden
 * Removed unused variables.
 *
 * Revision 1.46  1996/09/19  13:16:20  madden
 * Adjusted subject offset by READDB_COMPRESSION_RATIO for calc. of diagonal.
 *
 * Revision 1.45  1996/09/18  21:25:30  madden
 * Fixed bug in WordFinder for nucleotides.
 *
 * Revision 1.44  1996/09/18  13:39:24  madden
 * fixed offsets for SeqAligns on minus strands.
 *
 * Revision 1.43  1996/09/17  12:27:04  madden
 * Changes to perform correct extensions in blastn.
 *
 * Revision 1.42  1996/09/16  19:41:14  sad
 * Changed BlastTimeFillStructure() to use new functions from ncbitime.
 * That removes platform-dependent code from this function.
 *
 * Revision 1.41  1996/09/13 20:01:52  madden
 * put in READDB_UNPACK macros.
 *
 * Revision 1.40  1996/09/12  21:11:55  madden
 * Added extension funcitons for blastn
 *
 * Revision 1.39  1996/09/11  22:21:06  madden
 * Changes for blastn.
 *
 * Revision 1.38  1996/09/11  20:36:41  shavirin
 * Removed few Windows NT compiler warnings
 *
 * Revision 1.35  1996/09/11  19:14:09  madden
 * Added BLAST_OptionsBlkPtr structure and use thereof.
 *
 * Revision 1.34  1996/09/10  19:40:35  madden
 * Added functions to perform blastn comparison.
 *
 * Revision 1.33  1996/09/05  19:39:52  madden
 * Added "word_width" to position already covered on diagonal.
 *
 * Revision 1.32  1996/09/05  19:26:16  madden
 * Combined masking and shifting, removed some checks if prelim.
 *
 * Revision 1.31  1996/09/05  14:12:19  madden
 * New (faster) type of extension.
 *
 * Revision 1.30  1996/09/03  16:27:21  madden
 * Added efficiency in scanning of database.
 *
 * Revision 1.29  1996/08/30  19:27:37  madden
 * Fix for one-pass blast, memory-mapped file was being freed.
 *
 * Revision 1.28  1996/08/30  18:23:50  madden
 * A few efficiencies and a correction for one-pass blast.
 *
 * Revision 1.27  1996/08/30  15:17:57  madden
 * Minor efficiency in BlastReapHitlistByEvalue.
 *
 * Revision 1.25  1996/08/28  20:07:36  madden
 * Fix for UMR when the (nucl) sequence is exactly div. by four.
 *
 * Revision 1.24  1996/08/28  17:11:07  madden
 * Fixes for the translation of (nucl.) database sequences.
 *
 * Revision 1.23  1996/08/27  21:51:44  madden
 * Changes for tblastx
 *
 * Revision 1.22  1996/08/27  17:47:37  madden
 * current_hitlist purged on second pass for tblastn.
 *
 * Revision 1.21  1996/08/26  17:20:20  shavirin
 * Added support for WIN32 in function BlastTimeFillStructure()
 *
 * Revision 1.20  1996/08/23  18:50:23  madden
 * Adjusted some of the NT warning fixes to give correct results.
 *
 * Revision 1.19  1996/08/23  16:52:07  madden
 * Changed Int1 to Int4 in SetUpBlastSearchInternal.
 *
 * Revision 1.18  1996/08/23  16:39:02  madden
 * Fixed problem with SaveCurrentHsp.
 *
 * Revision 1.17  1996/08/23  15:29:44  shavirin
 * Fixed a lot of NT compiler warnings about type mismatch
 *
 * Revision 1.16  1996/08/21  21:37:01  madden
 * Added casts to silence compiler warning.s
 *
 * Revision 1.15  1996/08/21  21:24:56  madden
 * Changes for tblastn.
 *
 * Revision 1.14  1996/08/21  12:55:54  madden
 * Changed "purge" frame.
 *
 * Revision 1.13  1996/08/15  17:07:57  madden
 * Added efficiencies in loop that scans database.
 *
 * Revision 1.12  1996/08/14  20:01:30  madden
 * Efficiencies suggested by Zheng Zhang.
 *
 * Revision 1.11  1996/08/14  18:15:31  madden
 * Query frame moved from context to BlastSeqBlk.
 *
 * Revision 1.10  1996/08/14  17:19:29  madden
 * Correctly set frame for subject.
 *
 * Revision 1.9  1996/08/14  15:20:37  madden
 * Added Blast prefix to TranslateUnambiguousSequence function name.
 *
 * Revision 1.8  1996/08/14  14:30:42  madden
 * Cleaned up problem with UMR in TranslateUnambiguousSequence.
 *
 * Revision 1.7  1996/08/13  22:04:36  madden
 * Fixed TranslateUnambiguousSequence to properly read a nucl. db.
 *
 * Revision 1.6  1996/08/13  15:26:29  madden
 * Changes for tblastn.
 *
 * Revision 1.5  1996/08/09  22:11:12  madden
 * Added original_sequence to BlastSequenceAddSequence.
 *
 * Revision 1.4  1996/08/08  21:39:00  madden
 * Added some functions for tblastn.
 *
 * Revision 1.3  1996/08/07  14:23:45  madden
 * Added functions to produce SeqAlign from BLAST results.
 *
 * Revision 1.2  1996/08/06  16:07:31  madden
 * Removed unused functions Bsp2BLAST0Request.
 *
 * Revision 1.1  1996/08/05  19:45:46  madden
 * Initial revision
 *
 * Revision 1.118  1996/08/05  13:56:44  madden
 * Check if threads are available with NlmThreadsAvailable.
 *
 * Revision 1.117  1996/08/02  14:20:06  madden
 * Changes in call to readdb.
 *
 * Revision 1.116  1996/07/31  13:46:23  madden
 * Each thread gets own copy of ewp_params in SearchBlk.
 *
 * Revision 1.115  1996/07/31  13:09:17  madden
 * Changes for threaded blast.
 *
 * Revision 1.114  1996/07/25  20:45:20  madden
 * Change to calling convention of PerformBlastSearchWithReadDb.
 *
 * Revision 1.113  1996/07/25  12:55:20  madden
 * readdb_get_sequence call changed to allow for systems w/o mmap.
 *
 * Revision 1.112  1996/07/24  13:16:28  madden
 * Removed commented out fprintf.
 *
 * Revision 1.111  1996/07/24  12:00:07  madden
 * Changes for blastx.
 *
 * Revision 1.110  1996/07/18  22:00:02  madden
 * Changes for multiple contexts.
 *
 * Revision 1.109  1996/07/18  13:35:51  madden
 * Addition of the BLASTContextStructPtr.
 *
 * Revision 1.108  1996/07/16  15:01:02  madden
 * Cleaned up link_hsp function.
 *
 * Revision 1.107  1996/07/16  14:37:42  madden
 * Changes to link_hsp's so another array is not needed for the HSP's.
 *
 * Revision 1.106  1996/07/11  16:03:58  madden
 * SaveCurrentHitlist keeps track of which set an HSP belongs to.
 *
 * Revision 1.105  1996/07/05  17:16:34  madden
 * Optimized loop in contiguous word finder.
 *
 * Revision 1.104  1996/07/03  14:26:05  madden
 * Added test extension function.
 *
 * Revision 1.103  1996/07/02  14:32:53  madden
 * Added hspcnt_max.
 *
 * Revision 1.102  1996/07/02  12:04:15  madden
 * HSP's saved on array, rather than linked list.
 *
 * Revision 1.101  1996/07/01  15:30:06  madden
 * Don't NULL out hit if extension to left does not succeed.
 *
 * Revision 1.100  1996/06/27  18:41:39  madden
 * Changes to cutoff score to start second pass.
 *
 * Revision 1.99  1996/06/26  19:38:12  madden
 * Don't continue extension on 1st pass if the first (left) extension
 * doesn't reach to the first hit.
 *
 * Revision 1.98  1996/06/26  15:53:54  madden
 * Second dropoff score parameter added.
 *
 * Revision 1.97  1996/06/26  14:30:25  madden
 * Removed unused variables.
 *
 * Revision 1.96  1996/06/26  14:09:16  madden
 * Added comments and indents to loops.
 *
 * Revision 1.95  1996/06/26  13:29:50  madden
 * Changes to reduce the amount of memory and time of BlastFindWords.
 *
 * Revision 1.94  1996/06/24  20:26:46  madden
 * Dropoff ("X") set to first or second dropoff parameter.
 *
 * Revision 1.93  1996/06/24  17:57:09  madden
 * Added wordFinders to test dropoff scores.
 *
 * Revision 1.92  1996/06/20  16:51:17  madden
 * Removed unused parameters.
 *
 * Revision 1.91  1996/06/20  16:15:57  madden
 * Replaced int's with Int4's.
 *
 * Revision 1.90  1996/06/19  14:18:33  madden
 * Addition of SetUpBlastSearchInternal function.
 *
 * Revision 1.89  1996/06/17  19:02:13  madden
 * Removed unused MP code.
 *
 * Revision 1.88  1996/06/17  18:23:31  madden
 * Removed unused functions.
 *
 * Revision 1.87  1996/06/14  17:58:13  madden
 * Changes to avoid nulling out arrays for every sequence.
 *
 * Revision 1.86  1996/06/13  21:16:33  madden
 * database length removed from BLAST_ExtendWordNew.
 *
 * Revision 1.85  1996/06/13  21:04:17  madden
 * Added efficiencies to word finders.
 *
 * Revision 1.84  1996/06/11  18:13:54  madden
 * Removed unused variables.
 *
 * Revision 1.83  1996/06/11  17:58:31  madden
 * Changes to allow shorter arrays for multiple hits type blast.
 *
 * Revision 1.82  1996/06/10  16:52:16  madden
 * Use bit-shifting and masking instead of dividing and remainder.
 *
 * Revision 1.81  1996/06/10  13:44:07  madden
 * Changes to reduce the size of the "already visited" array.
 *
 * Revision 1.80  1996/06/06  17:54:09  madden
 * number_of_bits added to SetUpBlastSearch and SetUpBlastSearchWithReadDb.
 *
 * Revision 1.79  1996/06/06  14:09:22  madden
 * Removed defunct function BlastNWSThreshold, blast_set_parameters became
 * static.
 *
 * Revision 1.78  1996/06/06  13:54:51  madden
 * Removed defunct function BLAST_ParameterBlkFill
 *
 * Revision 1.77  1996/06/06  13:23:17  madden
 * CalculateSecondCutoffs only called for second pass.
 *
 * Revision 1.76  1996/06/04  15:32:53  madden
 * Changed counting of first and second pass hits.
 *
 * Revision 1.75  1996/06/04  13:50:28  madden
 * Purge HitList, rather than deleting it.
 *
 * Revision 1.74  1996/05/29  17:21:07  madden
 * Removed defunct BlastFixEandPValues function, replaced one call
 * to BlastSequenceAddSequence.
 *
 * Revision 1.73  1996/05/29  12:43:25  madden
 * Function BlastTimeFillStructure added to keep track of time.
 *
 * Revision 1.72  1996/05/28  14:12:53  madden
 * Added code to collect statistics.
 *
 * Revision 1.71  1996/05/23  21:55:04  madden
 * Removed unused variable initlen
 *
 * Revision 1.70  1996/05/22  20:19:22  madden
 * Removed unused variables, fixed codecenter nits.
 *
 * Revision 1.68  1996/05/20  21:17:49  madden
 * Changed (incorrect) NULL's to zero's.
 *
 * Revision 1.67  1996/05/16  19:50:15  madden
 * Added documentation block.
 *
 * Revision 1.66  1996/05/16  13:28:24  madden
 * Both 1st and 2nd pass can separately be contiguous or discontiguous.
 *
 * Revision 1.64  1996/05/14  19:51:37  madden
 * Added some register variables.
 *
 * Revision 1.63  1996/05/14  18:56:53  madden
 * Unrolled some loops in extension function.
 *
 * Revision 1.62  1996/05/14  16:15:59  madden
 * Fixes to SaveCurrentHitlist
 *
 * Revision 1.61  1996/05/10  18:19:20  madden
 * Made lookup_pos a register variable.
 *
 * Revision 1.59  1996/05/09  13:14:56  madden
 * Consolidated CalculateEffectiveLengths and BlastReapHSPsByEvalue into other
 * functions.
 *
 * Revision 1.58  1996/05/03  19:54:24  madden
 * Removed defunct seqalign functions, optimized BlastWordFinder functions.
 *
 * Revision 1.57  1996/05/01  14:57:37  madden
 * Added BlastResults structures.
 *
 * Revision 1.56  1996/04/24  19:46:34  madden
 * Removed q_rightmost and q_leftmost from the extend function.
 *
 * Revision 1.55  1996/04/24  18:01:11  madden
 * Used call to readdb_get_max_length for first call to BLAST_ExtendWordNew.
 *
 * Revision 1.54  1996/04/24  16:16:58  madden
 * Changed LinkHsp's not to reallocate the hsp array every time.
 *
 * Revision 1.53  1996/04/24  12:51:15  madden
 * deleted function BlastSequenceAddSequenceIdToSequenceBlk.
 *
 * Revision 1.52  1996/04/22  21:39:31  madden
 * New calls to readdb_get_sequence.
 *
 * Revision 1.51  1996/04/18  13:39:33  madden
 * demodularized lookup of initial hits.
 *
 * Revision 1.50  1996/04/16  15:32:47  madden
 * economies added to new extension functions, non-scoring identical
 * words not added to lookup tables.
 *
 * Revision 1.48  1996/04/11  14:29:33  madden
 * function BlastWordExtend completely rewritten.
 *
 * Revision 1.47  1996/04/04  20:46:22  madden
 * Optimized extension function; made "lookup_find" a FnPtr.
 *
 * Revision 1.46  1996/04/03  19:13:04  madden
 * added functions PerformBlastSearchWithReadDb and Perform2PassBlastSearchWithReadDb.
 *
 * Revision 1.45  1996/03/29  21:26:01  madden
 * "hitlist" now kept on SeqAlign rather than HitList.
 *
 * Revision 1.44  1996/03/29  14:08:18  madden
 * SetUpBlastSearchWithReadDb added.
 *
 * Revision 1.43  1996/03/28  18:45:45  madden
 * sequence now added to hitlist after significance has been established.
 *
 * Revision 1.42  1996/03/27  23:51:11  madden
 * added function AddDescriptorsToHitlistWithReadDb.
 *
 * Revision 1.41  1996/03/27  23:19:24  madden
 * Added PerformBlastSearchWithReadDb and Perform2PassBlastSearchWithReadDb,
 * changed parameters for PerformBlastSearch and Perform2PassBlastSearch.
 *
 * Revision 1.40  1996/03/27  19:51:09  madden
 * current hits now saved on "current_hitlist", not saved to main
 * hitlist until significance decided upon.
 *
 * Revision 1.39  1996/03/26  19:36:15  madden
 * Changes to read databases formatted with formatdb.
 *
 * Revision 1.38  1996/03/25  16:34:19  madden
 * Changes to mimic old statistics.
 *
 * Revision 1.37  1996/03/20  14:28:57  madden
 * Changed cutoff values.
 *
 * Revision 1.36  1996/03/11  13:52:52  madden
 * Ignore gaps when the sequences are too short.
 *
 * Revision 1.35  1996/02/28  21:36:54  madden
 * changes for discontiguous words.
 *
 * Revision 1.34  1996/02/15  23:31:19  madden
 * Trimmed ends of HSP's in comparison with gap.
 *
 * Revision 1.33  1996/02/15  23:19:43  madden
 * Changed call to BlastScoreBlkFill
 *
 * Revision 1.32  1996/02/15  15:22:52  madden
 * Trimming of sequence ends for linking.
 *
 * Revision 1.31  1996/02/13  14:05:57  madden
 * changes to ensure that closer to optimal HSP's are found.
 *
 * Revision 1.30  1996/02/09  13:50:09  madden
 * Added BlastReapHSPsByEvalue; changes to allow both one and two pass runs.
 *
 * Revision 1.29  1996/02/06  22:50:56  madden
 * Changes for two-pass runs.
 *
 * Revision 1.28  1996/02/05  18:46:09  madden
 * Added support for two threshold values.
 *
 * Revision 1.27  1996/02/02  19:24:53  madden
 * Added wfp_first and wfp_second for first and second pass.
 *
 * Revision 1.26  1996/01/31  17:33:54  madden
 * Added function BlastReapHitlistByEvalue.
 *
 * Revision 1.25  1996/01/29  21:11:38  madden
 * Changes for MultipleHits BLAST.
 *
 * Revision 1.24  1996/01/23  16:30:52  madden
 * e_cutoff changed from BLAST_Score to double in SetUpBlastSearch.
 *
 * Revision 1.23  1996/01/22  22:31:01  madden
 * Fixed BlastFindWords to increment index1 correctly.
 *
 * Revision 1.22  1996/01/22  22:05:05  madden
 * Set initial e2 to 0.5.
 *
 * Revision 1.20  1996/01/17  16:59:56  madden
 * Added gap arguments to SetUpBlastSearch.
 *
 * Revision 1.19  1996/01/17  13:45:03  madden
 * Added function BlastFixEandPValues.
 *
 * Revision 1.18  1996/01/16  15:28:05  madden
 * Set i_am_multitasking flag.
 *
 * Revision 1.16  1996/01/10  17:50:21  madden
 * sort hitlist by pvalue.
 *
 * Revision 1.15  1996/01/08  23:23:22  madden
 * Fixed neighborhood bug, added some MP stuff
 *
 * Revision 1.14  1996/01/06  18:56:52  madden
 * Removed obsolete code, fixed purify nit.
 *
 * Revision 1.13  1996/01/06  17:50:20  madden
 * Fixed HeapSort functions for linking of HSP's.
 *
 * Revision 1.12  1996/01/06  17:18:42  madden
 * Fixed setting of "next" pointers when the HSp is part of a linked set.
 *
 * Revision 1.11  1996/01/06  16:29:38  madden
 * NULL'ed out some "link" pointers.
 *
 * Revision 1.10  1996/01/05  22:54:18  madden
 * Fixed HeapSort calls in linking routines.
 *
 * Revision 1.9  1996/01/05  15:51:14  madden
 * Added Stephen Altschul's link_hsps.
 *
 * Revision 1.8  1995/12/30  19:21:01  madden
 * Added PerformBlastSearch.
 *
 * Revision 1.7  1995/12/30  18:38:51  madden
 * Added function SetUpBlastSearch.
 *
 * Revision 1.6  1995/12/28  21:22:19  madden
 * Deallocated leaking memory.
 *
 * Revision 1.5  1995/12/26  23:03:22  madden
 * Added in functions to automatically set some parameters.
 *
 * Revision 1.4  1995/12/26  20:27:11  madden
 * simplified hit extension routine.
 *
 * Revision 1.3  1995/12/21  23:09:57  madden
 * BLAST_Score functions moved to blastkar.c
 *
 * */


#include "blastaltivec.h"

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

/* For rlimit stuff. */
#if defined(OS_UNIX)
#include <sys/resource.h>
#include <signal.h>
#endif

#include <mbalign.h>
#include <mblast.h>
/* 
The last database sequence a tick (progress indicator) was issued for
and the increments (i.e., number of db sequences completed) that a tick 
should be emitted. 
*/
/* Int4	last_db_seq=0, db_incr=0; */

/*
	Set to TRUE if the process has timed out.
*/
Boolean time_out_boolean=FALSE;

/*
	SeqId lists if only a certain number of the database sequences will be
	used for the search.
*/
/* SeqIdPtr global_seqid_list=NULL, global_seqid_ptr; */

/*
	GI List to be used if database will be searched by GI.
	current is the current element in the array being worked on.
	global_gi_being_used specifies that it will be used.
*/

/* Int4 global_gi_current=0;
   Boolean global_gi_being_used=FALSE; */

/* Function to emit progress messages, set by user. */
/* int (LIBCALLBACK *tick_callback)PROTO((Int4 done, Int4 positives)); */

/* int (LIBCALLBACK *star_callback)PROTO((Int4 done, Int4 positives));
   int (LIBCALLBACK *index_callback)PROTO((Int4 done, Int4 positives)); */

/* tells star_proc to check that a star should be emitted. */
/* TNlmThread awake_thr=NULL;
   Boolean awake; */

/* tells index_proc to check that a message should be emitted. */
/* TNlmThread index_thr=NULL;
   Boolean awake_index; */

/* period of sending out a star/message. */
/* #define PERIOD 60 */

/* Use by star_proc to determine whether to emit a star. */
/* time_t last_tick=0; */

/* How many positive hits were found (set by ReapHitlist, read by tick_proc
and star_proc). */
/* Int4 number_of_pos_hits=0; */

/* Mutex for assignment of db seqs to search. */
/* TNlmMutex db_mutex=NULL; */

/* Mutex for insertion of results into list. */
/* TNlmMutex results_mutex = NULL; */
/* Mutex for the callbacks (star_proc, tick_proc, index_proc). */
/* TNlmMutex callback_mutex=NULL; */

/* The last db sequence to be assigned.  Used only in get_db_chunk after
the acquisition of the "db_mutex" (above). */
/* Int4 db_chunk_last=0; */

/* the last sequence in the database to be compared against. */
/* Int4 final_db_seq; */

/* Default size of the chunks be that are assigned in the function get_db_chunk. */
/* Actually db_chunk_size is used, which is smaller if the db is smaller. */

/* #define BLAST_DB_CHUNK_SIZE 500
   Int4 db_chunk_size; */

static Int4 BlastExtendWordSearch PROTO((BlastSearchBlkPtr search, Boolean multiple_hits));

static Int2 BlastWordExtend PROTO((BlastSearchBlkPtr search, Int4 q_off, Int4 s_off, Int4 word_width, BLAST_Diag diag, BLAST_Diag real_diag, Boolean PNTR succeed_to_right, Int2 context));

/*AAS*/
static Int2 BlastNewWordExtend PROTO((BlastSearchBlkPtr search, Int4 q_off, Int4 s_off, Int4 word_width, BLAST_Diag diag, BLAST_Diag real_diag, Boolean PNTR succeed_to_right, Int2 context));

static Int2 BlastWordExtend_prelim PROTO((BlastSearchBlkPtr search, Int4 q_off, Int4 s_off, Int4 word_width, BLAST_Diag diag, BLAST_Diag real_diag, Boolean PNTR succeed_to_right, Int2 context));

/*AAS*/
static Int2 BlastNewWordExtend_prelim PROTO((BlastSearchBlkPtr search, Int4 q_off, Int4 s_off, Int4 word_width, BLAST_Diag diag, BLAST_Diag real_diag, Boolean PNTR succeed_to_right, Int2 context));


static Int4 BlastWordFinder PROTO((BlastSearchBlkPtr search));
static Int4 BlastWordFinder_mh PROTO((BlastSearchBlkPtr search));
static Int4 BlastWordFinder_contig PROTO((BlastSearchBlkPtr search, LookupTablePtr lookup));
static Int4 BlastWordFinder_mh_contig PROTO((BlastSearchBlkPtr search, LookupTablePtr lookup));

static BLAST_HSPPtr link_hsps PROTO((BlastSearchBlkPtr search, BLAST_HitListPtr hitlist, BLAST_HSPPtr PNTR hsp_array));

static Int4 BlastNtWordFinder PROTO((BlastSearchBlkPtr search, LookupTablePtr lookup));
static Int4 BlastNtWordFinder_mh PROTO((BlastSearchBlkPtr search, LookupTablePtr lookup));

/* DEBUGGING stuff */
#ifdef BLAST_TIMER
clock_t	last_clock = 0;
#endif
/* end DEBUGGING stuff */


/*
	The function that decides whether or not a tick should be
	emitted.  This is performed through the callback function
	("tick_callback") that is set in "do_the_blast_run".  This
	function is called from "do_blast_search" for single processing
	machines and "get_db_chunk" for MT machines, after the db_mutex
	has been obtained in "get_db_chunk".
*/

void BlastTickProc(Int4 sequence_number, BlastThrInfoPtr thr_info)

{
    if(thr_info->tick_callback && 
       (sequence_number > (thr_info->last_db_seq + thr_info->db_incr))) {
        NlmMutexLockEx(&thr_info->callback_mutex);
        thr_info->last_db_seq += thr_info->db_incr;
        thr_info->tick_callback(sequence_number, thr_info->number_of_pos_hits);
        thr_info->last_tick = Nlm_GetSecs();
        NlmMutexUnlock(thr_info->callback_mutex);
    }
    return;
}

/*
	Sends out a message every PERIOD (i.e., 60 secs.) for the index.

	THis function runs as a separate thread and only runs on a threaded
	platform.
*/
VoidPtr
index_proc(VoidPtr dummy)

{

    /* Sleep only works on UNIX.  An ifdef is used until
       a portable solution can be found. */
#ifdef OS_UNIX
    
    Int2 index;
    BlastThrInfoPtr thr_info = (BlastThrInfoPtr) dummy;
    
    while (thr_info->awake_index) {
        for (index=0; index < STAR_MSG_PERIOD; index++) {
            sleep(1);
            if (thr_info->awake_index == FALSE)
                break;
        }
        
        if (thr_info->awake_index && thr_info->index_callback) {
            NlmMutexLockEx(&thr_info->callback_mutex);
            thr_info->last_tick = Nlm_GetSecs();
            thr_info->index_callback(0, 0);
            NlmMutexUnlock(thr_info->callback_mutex);
        }
    }
#endif
    return dummy;
}

/*
	Sends out a message every PERIOD (i.e., 60 secs.) and sends out a
	"star" if a tick has not been sent out in the last PERIOD. 

	THis function runs as a separate thread and only runs on a threaded
	platform.
*/
static VoidPtr
star_proc(VoidPtr dummy)

{
    /* Sleep only works on UNIX.  An ifdef is used until
       a portable solution can be found. */
#ifdef OS_UNIX
    
    time_t now;
    Int2 index;
    BlastThrInfoPtr thr_info = (BlastThrInfoPtr) dummy;
    
    now = Nlm_GetSecs();
    while (thr_info->awake) {
        if (now - thr_info->last_tick < STAR_MSG_PERIOD / 2) {
            for (index = 0; index < STAR_MSG_PERIOD; index++) {
                sleep(1);
                if (thr_info->awake == FALSE)
                    break;
            }
        }
        if (thr_info->awake) {
            NlmMutexLockEx(&thr_info->callback_mutex);
            now = Nlm_GetSecs();
            if (now-thr_info->last_tick > STAR_MSG_PERIOD) {
                if (thr_info->star_callback) {
                    thr_info->star_callback(thr_info->db_chunk_last, 
                                            thr_info->number_of_pos_hits);
                    thr_info->last_tick = now;
                }
            }
            NlmMutexUnlock(thr_info->callback_mutex);
        }
    }
#endif
    return dummy;
}

/*
	Make a temporary protein BioseqPtr to use with seg.
*/
BioseqPtr
BlastMakeTempProteinBioseq (Uint1Ptr sequence, Int4 length, Uint1 alphabet)

{
    BioseqPtr bsp;
    Int4 byte_store_length;
    Nlm_ByteStorePtr byte_store;
    ObjectIdPtr oip;

    if (sequence == NULL || length == 0)
        return NULL;
    
    byte_store = Nlm_BSNew(length);
    
    byte_store_length = Nlm_BSWrite(byte_store, (VoidPtr) sequence, length);
    if (length != byte_store_length) {
        Nlm_BSDelete(byte_store, length);
        return NULL;
    }
    
    bsp = BioseqNew();
    bsp->seq_data = byte_store;
    bsp->length = length;
    bsp->seq_data_type = alphabet;
    bsp->mol = Seq_mol_aa;
    bsp->repr = Seq_repr_raw;
    
    oip = UniqueLocalId();
    ValNodeAddPointer(&(bsp->id), SEQID_LOCAL, oip);
    SeqMgrAddToBioseqIndex(bsp);
    
    return bsp;
}

#define MY_EPS 1.0e-9
/*
	Calculates cutoff scores and returns them.
	Equations provided by Stephen Altschul.

	BlastSearchBlkPtr search: provides info to perform calculation.
	Int4 subject_length: length of the DB sequence.
	Boolean PNTR ignore_small_gaps: If TRUE, test only for large gaps.
	BLAST_Score PNTR cutoff_s_second: S2 score for second pass.
	BLAST_Score PNTR cutoff_big_gap: Cutoff score for big gaps.

*/
static void
CalculateSecondCutoffScore(BlastSearchBlkPtr search, Int4 subject_length, Boolean PNTR ignore_small_gaps, BLAST_Score PNTR cutoff_s_second, BLAST_Score PNTR cutoff_big_gap)

{
	Nlm_FloatHi gap_prob, gap_decay_rate, x_variable, y_variable;
	BLAST_KarlinBlkPtr kbp;
	Int4 expected_length, gap_size, query_length;
	Int8 search_sp;

	/* Do this for the first context, should this be changed?? */
	kbp = search->sbp->kbp[search->first_context];
	gap_size = search->pbp->gap_size;
	gap_prob = search->pbp->gap_prob;
	gap_decay_rate = search->pbp->gap_decay_rate;
	query_length = search->context[search->first_context].query->length;

	if (search->pbp->old_stats == FALSE)
	{
	/* Subtract off the expected score. */
	   expected_length = Nint(log(kbp->K*((Nlm_FloatHi) query_length)*((Nlm_FloatHi) subject_length))/(kbp->H));
	   query_length = query_length - expected_length;
	   subject_length = subject_length - expected_length;
	   query_length = MAX(query_length, 1);
	   subject_length = MAX(subject_length, 1);

	   if (search->dblen > subject_length)
	   	y_variable = log((Nlm_FloatHi) (search->dblen)/(Nlm_FloatHi) subject_length)*(kbp->K)/(gap_decay_rate);
	   else
	   	y_variable = log((Nlm_FloatHi) (subject_length + expected_length)/(Nlm_FloatHi) subject_length)*(kbp->K)/(gap_decay_rate);
	   search_sp = ((Int8) query_length)* ((Int8) subject_length);
	   x_variable = 0.25*y_variable*((FloatHi) search_sp);

/* To use "small" gaps the query and subject must be "large" compared to
the gap size. If small gaps may be used, then the cutoff values must be
adjusted for the "bayesian" possibility that both large and small gaps are
being checked for. */

	   if (search_sp > 8*gap_size*gap_size)
	   {
		x_variable /= (1.0 - gap_prob + MY_EPS);
		*cutoff_big_gap = (BLAST_Score) floor((log(x_variable)/kbp->Lambda)) + 1;
		x_variable = y_variable*(gap_size*gap_size);
		x_variable /= (gap_prob + MY_EPS);
		*cutoff_s_second= (BLAST_Score) floor((log(x_variable)/kbp->Lambda)) + 1;
		*ignore_small_gaps = FALSE;
	   }
	   else
	   {
		*cutoff_big_gap = (BLAST_Score) floor((log(x_variable)/kbp->Lambda)) + 1;
		*cutoff_s_second = *cutoff_big_gap;
		*ignore_small_gaps = TRUE;
	   }	
	   *cutoff_big_gap *= search->pbp->scalingFactor;
	   *cutoff_s_second *= search->pbp->scalingFactor;
	}
	else
	{
	/* USE the old statistics, for comparison to the OLD BLAST. */
		*cutoff_big_gap = search->pbp->cutoff_s_second;
		*cutoff_s_second = *cutoff_big_gap;
		*ignore_small_gaps = TRUE;
	}
}


#if 0
/* TODO: The following function is not used in that file */

/*
	Check that the HSP's pass through the region of the
	query that is required, if this is requried.
*/

static Int2 LIBCALL
CheckForRequiredRegion (BlastSearchBlkPtr search, Boolean strict)

{
	BLAST_HitListPtr hitlist;
	BLAST_HSPPtr hsp;
	BLAST_HSPPtr PNTR hsp_array, PNTR hsp_array_new;
	Int4 hsp_cnt=0, hspcnt_max;
	Int4 index, index1;

	if (search == NULL)
		return 1;

	if (search->whole_query == TRUE)
		return 0;

	hsp_array = NULL;
	hitlist = search->current_hitlist;
	hitlist->hspcnt_max = hitlist->hspcnt;
	hspcnt_max = hitlist->hspcnt_max;
	if (hitlist)
	{
		hsp_array = hitlist->hsp_array;
		for (index=0; index<hitlist->hspcnt; index++)
		{
			hsp = hsp_array[index];
			if (strict)
			{
				if (hsp->query.offset > search->required_start ||
					hsp->query.end < search->required_end)
				{
					hsp_array[index] = MemFree(hsp_array[index]);
				}
				else
				{
					hsp_cnt++;
				}
			}
			else
			{
				if ((search->required_start > hsp->query.offset && 
					search->required_start < hsp->query.end) ||
						(search->required_end > hsp->query.offset && 
							search->required_end < hsp->query.end))
				{
					hsp_cnt++;
				}
				else
				{
					hsp_array[index] = MemFree(hsp_array[index]);
				}
					
					
			}

		}

		if (hitlist->hspcnt > 0)
		{
			hitlist->hspcnt = hsp_cnt;
		}
		else
		{
			BlastHitListPurge(hitlist);
		}
	}

/* Save HSP's again, discarding those that have been NULLed out. */
	hsp_array_new = MemNew((hitlist->hspmax)*sizeof(BLAST_HSPPtr));
	index1 = 0;
	for (index=0; index<hspcnt_max; index++)
	{
		if (hsp_array[index] != NULL)
		{
			hsp_array_new[index1] = hsp_array[index];
			index1++;
		}
	}

	hsp_array = MemFree(hsp_array);

	hitlist->hspcnt = index1;	
	hitlist->hspcnt_max = index1;	
	hitlist->hsp_array = hsp_array_new;

	search->current_hitlist = hitlist;
	return 0;
}

#endif

/*
	This function reevaluates the HSP's from a blast run, checking that
	ambiguity characters, ignored until now, don't change the score or
	extent of the HSP's.

	Only works for blastn right now.
*/

static Int2
BlastReevaluateWithAmbiguities (BlastSearchBlkPtr search, Int4 sequence_number)

{
	BioseqPtr bsp;
	register BLAST_Score	sum, score;
	register BLAST_ScorePtr PNTR    matrix;
	BLAST_HitListPtr current_hitlist;
	BLAST_HSPPtr PNTR hsp_array;
	Int4 context, hspcnt, hspcnt_max, index, index1, status;
	Int4 length, longest_hsp_length, start, stop;
	Nlm_FloatHi current_evalue=DBL_MAX;
	SeqPortPtr spp=NULL;
	Uint1Ptr nt_seq, nt_seq_start, subject, subject_start, query, old_query_s, old_query_f, new_query_s, new_query_f=NULL;
	Uint1Ptr query_start, query_end, subject_real_start=NULL;

/* Only nucl. db's. */
	if (search->prog_number == blast_type_blastp || search->prog_number == blast_type_blastx)
		return 0;

/* Gapped alignments will be reevaluated anyway.*/
	if (search->pbp->gapped_calculation == TRUE || search->pbp->do_not_reevaluate == TRUE)
		return 0;

/* No hits to reevaluate. */
	if (search->current_hitlist == NULL || search->current_hitlist->hspcnt == 0)
		return 0;

/* Check if there are ambiguites at all, return 0 if there are none. */
	if(readdb_ambchar_present(search->rdfp, sequence_number) == FALSE)
		return 0;

	current_hitlist = search->current_hitlist;
	hspcnt = current_hitlist->hspcnt;
	hspcnt_max = current_hitlist->hspcnt_max;
	hsp_array = current_hitlist->hsp_array;
	matrix = search->sbp->matrix;

	/* Look for longest HSP. */
	longest_hsp_length = 0;
	for (index=0; index<hspcnt_max; index++)
	{
		if (hsp_array[index] == NULL)
			continue;

		if (hsp_array[index]->subject.length > longest_hsp_length)
			longest_hsp_length = hsp_array[index]->subject.length;

		if (current_evalue > hsp_array[index]->evalue)
			current_evalue = hsp_array[index]->evalue;
	}

	/* The evalue is worse the worst evalue, and the list is full. */
	if (search->worst_evalue < current_evalue)
		return 0;

	if (StringCmp(search->prog_name, "blastn") != 0)
	{
		longest_hsp_length *= CODON_LENGTH;
	}

	if (longest_hsp_length > 0)
	{
		nt_seq_start = MemNew(longest_hsp_length*sizeof(Uint1));
		if (nt_seq_start == NULL)
			return 0;
	}
	else
	{
		return longest_hsp_length;
	}
        
        if (search->thr_info->ambiguities_mutex)
            NlmMutexLock(search->thr_info->ambiguities_mutex);
        
	bsp = readdb_get_bioseq(search->rdfp, sequence_number);

	for (index=0; index<hspcnt_max; index++)
	{
		if (hsp_array[index] == NULL)
			continue;

		context = hsp_array[index]->context;

		if (StringCmp(search->prog_name, "blastn") == 0)
		{
			start = hsp_array[index]->subject.offset;
			stop = hsp_array[index]->subject.end - 1;
			length = hsp_array[index]->subject.length;
		}
		else
		{	/* Convert for translated alphabet. */
		    if (hsp_array[index]->subject.frame > 0)
		    {
			start = hsp_array[index]->subject.frame - 1 + CODON_LENGTH*(hsp_array[index]->subject.offset);
			stop = start + CODON_LENGTH*(hsp_array[index]->subject.length) - 1;
			length = CODON_LENGTH*(hsp_array[index]->subject.length);
		    }
		    else
		    {
			start = bsp->length - CODON_LENGTH*(hsp_array[index]->subject.offset + hsp_array[index]->subject.length) + hsp_array[index]->subject.frame + 1;
			stop = bsp->length - CODON_LENGTH*(hsp_array[index]->subject.offset) + hsp_array[index]->subject.frame;
			length = CODON_LENGTH*(hsp_array[index]->subject.length);
		     }
		}

		if (hsp_array[index]->subject.frame > 0)
		{
			spp = SeqPortNew(bsp, start, stop, Seq_strand_plus, Seq_code_ncbi4na);
                        SeqPortSet_do_virtual(spp, TRUE);

		}
		else
		{	/* Offsets correct here?? */
			spp = SeqPortNew(bsp, start, stop, Seq_strand_minus, Seq_code_ncbi4na);
                        SeqPortSet_do_virtual(spp, TRUE);
		}

		if (StringCmp(search->prog_name, "blastn") == 0)
		{
			nt_seq = nt_seq_start;
			while (length > 0)
			{
				*nt_seq = ncbi4na_to_blastna[SeqPortGetResidue(spp)];
				nt_seq++;
				length--;
			}
			subject_start = nt_seq_start;
		}
		else
		{
			nt_seq = nt_seq_start;
			while (length > 0)
			{
				*nt_seq = SeqPortGetResidue(spp);
				nt_seq++;
				length--;
			}
			/* Set frame to one so we start at beginning of nt seq. */
			subject_real_start = GetTranslation(nt_seq_start, CODON_LENGTH*(hsp_array[index]->subject.length), 1, &length, search->db_genetic_code);
			/* The first Residue is a NULLB */
			subject_start = subject_real_start+1;
		}
		spp = SeqPortFree(spp);

		query_start = (Uint1Ptr) search->context[context].query->sequence;
		query_end = query_start + search->context[context].query->length;

		score = 0;
		sum = 0;
		subject = subject_start;
		old_query_s = query_start + hsp_array[index]->query.offset;
		old_query_f = query_start + hsp_array[index]->query.end; 
		/* Assume, for now, that the real HSP starts where it does now. */
		new_query_s = old_query_s;
		for (query=old_query_s; query<old_query_f; query++, subject++)
		{
			if ((sum += matrix[*query][*subject]) <= 0)
			{
				if (score > 0)
				{
					subject = subject_start + (new_query_f-old_query_s); 
					if (score >= search->pbp->cutoff_s2)
					{
						break;
					}
				}
				score = sum = 0;
				new_query_s = new_query_f = query;
			}
			else if (sum > score)
			{	/* Start of scoring regime. */
				if (score == 0)
					new_query_s = query;
				score = sum;
				new_query_f = query+1;
			}
		}

		if (score >= search->pbp->cutoff_s2)
		{ /* Adjust the information here. */
			hsp_array[index]->score = score;
			hsp_array[index]->query.offset = new_query_s - query_start;
			hsp_array[index]->query.end = new_query_f - query_start;
			hsp_array[index]->query.length = hsp_array[index]->query.end - hsp_array[index]->query.offset;
			hsp_array[index]->subject.offset = hsp_array[index]->subject.offset + new_query_s - old_query_s;
			hsp_array[index]->subject.end = hsp_array[index]->subject.end + new_query_f - old_query_f;
			hsp_array[index]->subject.length = hsp_array[index]->subject.end - hsp_array[index]->subject.offset;
			hsp_array[index]->linked_set = FALSE;
			hsp_array[index]->start_of_chain = FALSE;
			Nlm_MemSet((VoidPtr) &(hsp_array[index]->hsp_link), 0, sizeof(BLAST_HSP_LINK));
			/* Need to NULL out more in HSP? */
		}
		else
		{ /* Delete if this is now below the cutoff score. */
			hsp_array[index] = MemFree(hsp_array[index]);
		}

		if (StringCmp(search->prog_name, "blastn") != 0)
		{
			subject_real_start = MemFree(subject_real_start);
		}
	}

	bsp = BioseqFree(bsp);
        if (search->thr_info->ambiguities_mutex)
                NlmMutexUnlock(search->thr_info->ambiguities_mutex);
	nt_seq_start = MemFree(nt_seq_start);

/* Save HSP's again, discarding those that have been NULLed out. */
	index1 = HspArrayPurge(hsp_array, hspcnt_max, TRUE);
	current_hitlist->hspcnt = index1;	
	current_hitlist->hspcnt_max = index1;	

	/* Relink the HSP's, ReReap the Hits. */
	if (search->pbp->do_sum_stats == TRUE && 
            !search->pbp->mb_params) 
           status = BlastLinkHsps(search);
        else
           status = BlastGetNonSumStatsEvalue(search);
        status = BlastReapHitlistByEvalue(search);
	
	return status;
}

/*
	Function to assign chunks of the database to a thread.  
	The "start" and "stop" points are returned by the arguments.
	Note that this is a half-closed interval (stop is not searched).

	The Int4 "db_chunk_last" (a global variable) keeps track of the last 
	database number assigned and is only changed if the db_mutex has been acquired.

	The Boolean done specifies that the search has already been
	completed.
*/

Boolean BlastGetDbChunk(ReadDBFILEPtr rdfp, Int4Ptr start, Int4Ptr stop, 
                     Int4Ptr id_list, Int4Ptr id_list_number, 
                     BlastThrInfoPtr thr_info)
     
{
    Boolean done=FALSE;
    Int4 ordinal_id;
    SeqIdPtr sip;

    *id_list_number = 0;
    
    NlmMutexLockEx(&thr_info->db_mutex);
    if (thr_info->realdb_done) {
	if (rdfp->oidlist) {
	    /* Virtual database.   Create id_list using mask file */
	    OIDListPtr oidlist = rdfp->oidlist;
	    Uint4	mask;
	    Int4	maskindex, base, i;
	    Boolean	cont;
	    Int4	oidindex;
	    Int4	total_mask = oidlist->total/MASK_WORD_SIZE + 1;


	    if (thr_info->gi_current < total_mask) {

		oidindex = 0;
		maskindex = thr_info->gi_current;
		base = thr_info->gi_current * MASK_WORD_SIZE;
		cont = TRUE;

		while (cont) {
		    /* for each long-word mask */
		    mask = Nlm_SwapUint4(oidlist->list[maskindex]);

		    i = 0;
		    while (mask) {
			if (mask & (((Uint4)0x1)<<(MASK_WORD_SIZE-1))) {
			    id_list[oidindex++] = base + i;
			}
			mask <<= 1;
			i++;
		    }
		    maskindex++;
		    base += MASK_WORD_SIZE;
		    if (maskindex >= total_mask ||
			    oidindex > thr_info->db_chunk_size) {
			cont = FALSE;
		    }
		}
		thr_info->gi_current = maskindex;
		*id_list_number = oidindex;
                BlastTickProc(thr_info->gi_current, thr_info);
	    } else {
		done = TRUE;
	    }

	} else {
	    done = TRUE;
	}
    } else if (thr_info->blast_seqid_list) {
	/* global_seqid_list indicates there is a list of selected ID's, 
	   global_seqid_ptr is the position in the list. */
	sip = thr_info->blast_seqid_list->seqid_ptr;
	if (sip == NULL) {
	    NlmMutexUnlock(thr_info->db_mutex);
	    return TRUE;
	}

	ordinal_id = SeqId2OrdinalId(rdfp, sip);
	while (ordinal_id < 0 && sip) {
	    thr_info->blast_seqid_list->seqid_ptr = 
		thr_info->blast_seqid_list->seqid_ptr->next;
	    sip = thr_info->blast_seqid_list->seqid_ptr;
	    ordinal_id = SeqId2OrdinalId(rdfp, sip);
	}

	if (thr_info->blast_seqid_list->seqid_ptr) {
	    *start = ordinal_id;
	    *stop = ordinal_id+1;
	    done = FALSE;
	    thr_info->blast_seqid_list->seqid_ptr = 
		thr_info->blast_seqid_list->seqid_ptr->next;
	} else {
	    done = TRUE;
	}

    } else {
	/* we have real database with start/stop specified */
	if (thr_info->db_mutex) {

	    /* Emit a tick if needed. */
	    BlastTickProc(thr_info->db_chunk_last, thr_info);
	    *start = thr_info->db_chunk_last;
	    if (thr_info->db_chunk_last < thr_info->final_db_seq) {
		*stop = MIN((thr_info->db_chunk_last + 
			    thr_info->db_chunk_size), 
			thr_info->final_db_seq);
	    } else {/* Already finished. */
		*stop = thr_info->db_chunk_last;
		thr_info->realdb_done = TRUE;
	    }
	    thr_info->db_chunk_last = *stop;
	} else {
	    if (*stop != thr_info->final_db_seq) {
		done = FALSE;
		*start = thr_info->last_db_seq;
		*stop = thr_info->final_db_seq;
	    } else {
		thr_info->realdb_done = TRUE;
                done = TRUE;
	    }
	}
    }
    NlmMutexUnlock(thr_info->db_mutex);
    
    return done;
}

/* Signal handler for SIGXCPU, exceeded cpu time soft limit. */

static void 
timeout_shutdown(int flag)

{
#ifdef OS_UNIX
  time_out_boolean = TRUE;

  /* this probably isn't the best way to do this.
   * probably ought to be rewritten with posix sigaction() instead.
   * -coulouri 20020104
   */

#ifdef SIG_IGN
  signal(SIGXCPU, SIG_IGN);
#else
  sigignore(SIGXCPU);
#endif

#endif
}


/* 
 * call setrlimit() on unix systems to cap cpu time.
 * changed to ignore number of cpus, since this limit is per process,
 * not per thread. also added a hard limit at 2x the soft limit.
 * -coulouri 20020104
 */

static Boolean
BlastSetLimits(Int4 cpu_limit, Int2 num_cpu)
{
#ifdef OS_UNIX
  struct rlimit   rl;

  if (cpu_limit <= 0)
    return FALSE;

  if (num_cpu < 1)
    return FALSE;

  rl.rlim_cur = cpu_limit;
  rl.rlim_max = 2 * cpu_limit;

  if (setrlimit(RLIMIT_CPU, &rl) == -1) return FALSE;
  
  if (signal(SIGXCPU, timeout_shutdown) == SIG_ERR) return FALSE;

#endif

  time_out_boolean = FALSE;
  return TRUE;
}

static void do_on_exit_func(VoidPtr ptr)
{
    BlastSearchBlkPtr search;
    search = (BlastSearchBlkPtr) ptr;
    
    if(ptr != NULL && search->queue_callback != NULL) {
        search->queue_callback(search->semid, 3, 1);
    }
    
    return;
}

static VoidPtr
do_gapped_blast_search(VoidPtr ptr)

{
	BlastSearchBlkPtr search;
	Int2 status;
	Int4 index, index1, start=0, stop=0, id_list_length;
	Int4Ptr id_list=NULL;

	search = (BlastSearchBlkPtr) ptr;
	if (search->thr_info->blast_gi_list || search->rdfp->oidlist) {
            id_list = MemNew((search->thr_info->db_chunk_size+33)
                             *sizeof(Int4));
        }

        NlmThreadAddOnExit(do_on_exit_func, ptr);


	while (BlastGetDbChunk(search->rdfp, &start, &stop, id_list, &id_list_length, search->thr_info) != TRUE)
	{
	    if (id_list)
	    {
		for (index=0; index<id_list_length; index++)
		{
			index1 = id_list[index];
			search = BLASTPerformSearchWithReadDb(search, index1);
           if (search->prog_number == blast_type_blastx 
	       || search->prog_number == blast_type_tblastn 
	       || search->prog_number == blast_type_psitblastn)

				status = BlastLinkHsps(search);

			status = BlastReapHitlistByEvalue(search);
			if (search->handle_results)
				search->handle_results((VoidPtr) search);
			else
				BlastSaveCurrentHitlist(search);
			/* Emit a tick if needed and we're not MT. */
			if (search->thr_info->db_mutex == NULL)
                            BlastTickProc(index1, search->thr_info);
			if (time_out_boolean == TRUE)
                            break;	
		}
             } else {
		for (index=start; index<stop; index++)
		{
			search = BLASTPerformSearchWithReadDb(search, index);

                       if (search->prog_number == blast_type_blastx 
			   || search->prog_number == blast_type_tblastn 
			   ||  search->prog_number == blast_type_psitblastn)

				status = BlastLinkHsps(search);

			status = BlastReapHitlistByEvalue(search);
			if (search->handle_results)
				search->handle_results((VoidPtr) search);
			else
				BlastSaveCurrentHitlist(search);
			/* Emit a tick if needed and we're not MT. */
			if (search->thr_info->db_mutex == NULL)
				BlastTickProc(index, search->thr_info);
			if (time_out_boolean == TRUE)
				break;	
		}
	     }
		/* Get out if "stop" was the last seq. */
		if (time_out_boolean)
			break;
	}

	if (id_list)
		id_list = MemFree(id_list);

	return (VoidPtr) search;
} 

static VoidPtr
do_blast_search(VoidPtr ptr)

{
    BlastSearchBlkPtr search;
    Int2 status;
    Int4 index, index1, start=0, stop=0, id_list_length;
    Int4Ptr id_list=NULL;

    search = (BlastSearchBlkPtr) ptr;
    if (search->thr_info->blast_gi_list || search->rdfp->oidlist) {
        id_list = MemNew((search->thr_info->db_chunk_size+33)
                         *sizeof(Int4));
    }

    NlmThreadAddOnExit(do_on_exit_func, ptr);
    
    while (BlastGetDbChunk(search->rdfp, &start, &stop, id_list,
			   &id_list_length, search->thr_info) != TRUE) {
        if (search->thr_info->realdb_done && id_list) {
            for (index=0; index<id_list_length; index++) {
                index1 = id_list[index];
                search = BLASTPerformSearchWithReadDb(search, index1);
                if (search->pbp->do_sum_stats == TRUE && 
		    !search->pbp->mb_params)
                    status = BlastLinkHsps(search);
                else
                    status = BlastGetNonSumStatsEvalue(search);
                if (!search->pbp->mb_params || !search->handle_results)
                   status = BlastReapHitlistByEvalue(search);
                if (search->pbp->mb_params)
                   MegaBlastReevaluateWithAmbiguities(search, index1);
		else if (!search->handle_results)
		   status = BlastReevaluateWithAmbiguities(search, index1);
		
                if (search->handle_results)
                    search->handle_results((VoidPtr) search);
		else if (!search->pbp->mb_params)
		   BlastSaveCurrentHitlist(search);
                else
		   MegaBlastSaveCurrentHitlist(search);
                if (search->pbp->mb_params)
                   /* Free the ncbi4na-encoded sequence */
                   search->subject->sequence_start = (Uint1Ptr)
                      MemFree(search->subject->sequence_start);
                /* Emit a tick if needed and we're not MT. */
                if (search->thr_info->db_mutex == NULL)
                    BlastTickProc(index1, search->thr_info);
                if (time_out_boolean == TRUE)
                    break;	
            }
        } else if (!search->thr_info->realdb_done) {
            for (index=start; index<stop; index++) {
                search = BLASTPerformSearchWithReadDb(search, index);
                if (search->pbp->do_sum_stats == TRUE && 
		    !search->pbp->mb_params)
                    status = BlastLinkHsps(search);
                else
                    status = BlastGetNonSumStatsEvalue(search);
                if (!search->pbp->mb_params ||
                    !search->handle_results) 
                   status = BlastReapHitlistByEvalue(search);
                if (search->pbp->mb_params)
                   MegaBlastReevaluateWithAmbiguities(search, index);
                else if (!search->handle_results)
                   status = BlastReevaluateWithAmbiguities(search, index);
                
                if (search->handle_results)
                   search->handle_results((VoidPtr) search);
		else if (!search->pbp->mb_params)
		   BlastSaveCurrentHitlist(search);
                else
		   MegaBlastSaveCurrentHitlist(search);

                if (search->pbp->mb_params)
                   /* Free the ncbi4na-encoded sequence */
                   search->subject->sequence_start = (Uint1Ptr)
                      MemFree(search->subject->sequence_start);
                /* Emit a tick if needed and we're not MT. */
                if (search->thr_info->db_mutex == NULL)
                    BlastTickProc(index, search->thr_info);
                if (time_out_boolean == TRUE)
                    break;	
            }
        }
        /* Get out if "stop" was the last seq. */
        if (time_out_boolean)
            break;
    }
    
    if (id_list)
        id_list = MemFree(id_list);
    
    return (VoidPtr) search;
} 

void LIBCALL
do_the_blast_run(BlastSearchBlkPtr search)

{
    BlastSearchBlkPtr PNTR array;
    Char buffer[256];
    Int2 index;
    Int4 number_of_entries, num_seq;
    ReadDBFILEPtr rdfp;
    TNlmThread PNTR thread_array;
    VoidPtr status=NULL;
    
    if (search == NULL)
        return;
    
    search->thr_info->db_chunk_size = BLAST_DB_CHUNK_SIZE;
    
#if 0    
    readdb_get_totals(search->rdfp, &total_length, &num_seq);	
#else
    num_seq = search->dbseq_num;
#endif
    search->thr_info->db_incr = num_seq / BLAST_NTICKS;
    search->thr_info->last_db_seq = search->pbp->first_db_seq;  /* The 1st sequence to compare against. */
    if (search->pbp->final_db_seq == 0)
        search->thr_info->final_db_seq = readdb_get_num_entries_total_real(search->rdfp);
    else
        search->thr_info->final_db_seq = search->pbp->final_db_seq;
    
    search->thr_info->db_chunk_last = search->pbp->first_db_seq;
    
    if (search->thr_info->blast_gi_list || search->rdfp->oidlist)
        search->thr_info->gi_current = 0;

    /* guess if we need to search any real database */
    if (search->thr_info->final_db_seq == 0) {
	/* that means that we have only mask and no real database should be searched */
	search->thr_info->realdb_done = TRUE;
    } else {
	/* otherwise, we need to start searching all real databases */
	search->thr_info->realdb_done = FALSE;
    }
    
    BlastSetLimits(search->pbp->cpu_limit, search->pbp->process_num);	
    if (NlmThreadsAvailable() && search->pbp->process_num > 1) {
        rdfp = search->rdfp;
        number_of_entries = INT4_MAX;
        /* Look for smallest database. */
        while (rdfp) {
            number_of_entries = MIN(number_of_entries, readdb_get_num_entries(rdfp));
            rdfp = rdfp->next;
        }
        /* Divide up the chunks differently if db is small. */
        if (search->thr_info->db_chunk_size*(search->pbp->process_num) > number_of_entries){
            /* check that it is at least one. */
            search->thr_info->db_chunk_size = MAX(number_of_entries/(search->pbp->process_num), 1);
        }
        NlmMutexInit(&search->thr_info->db_mutex);
        NlmMutexInit(&search->thr_info->results_mutex);
        NlmMutexInit(&search->thr_info->ambiguities_mutex);
        
        array = (BlastSearchBlkPtr PNTR) MemNew((search->pbp->process_num)*sizeof(BlastSearchBlkPtr));
        array[0] = search;
        for (index=1; index<search->pbp->process_num; index++) {
            array[index] = BlastSearchBlkDuplicate(search);
            if (array[index] == NULL) {
               search->pbp->process_num = index;
	       ErrPostEx(SEV_WARNING, 0, 0, "Number of threads reduced to %d", index);
               break;
            }
        }
        
        thread_array = (TNlmThread PNTR) MemNew((search->pbp->process_num)*sizeof(TNlmThread));
        for (index=0; index<search->pbp->process_num; index++) {
            if (search->pbp->gapped_calculation &&
                StringCmp(search->prog_name, "blastn") != 0)
                thread_array[index] = NlmThreadCreateEx(do_gapped_blast_search, (VoidPtr) array[index], THREAD_RUN|THREAD_BOUND, eTP_Default, NULL, NULL);
            else
                thread_array[index] = NlmThreadCreateEx(do_blast_search, (VoidPtr) array[index], THREAD_RUN|THREAD_BOUND, eTP_Default, NULL, NULL);
            
            if (NlmThreadCompare(thread_array[index], NULL_thread)) {
                ErrPostEx(SEV_ERROR, 0, 0, "Unable to open thread.");
            }
        }

        for (index=0; index<search->pbp->process_num; index++) {
            NlmThreadJoin(thread_array[index], &status);
        }

#ifdef OS_UNIX
        if(search->pbp->process_num > 1 && 
           search->semid > 0 &&
           search->queue_callback) {
            /* despite the fact, that this function may
               return error - it will not be fatal for a
               given blast search */
            /*
              BQ_IncSemaphore(search->semid, 3, 
              search->pbp->process_num -1);
            */

            /* search->queue_callback(search->semid, 3, 
               search->pbp->process_num -1); */
            
            search->queue_callback(search->semid, 3, -1);
        }
#endif

        for (index=1; index<search->pbp->process_num; index++) {
#ifdef BLAST_COLLECT_STATS
            search->first_pass_hits += array[index]->first_pass_hits;
            search->second_pass_hits += array[index]->second_pass_hits;
            search->second_pass_trys += array[index]->second_pass_trys;
            search->first_pass_extends += array[index]->first_pass_extends;
            search->second_pass_extends += array[index]->second_pass_extends;
            search->first_pass_good_extends += array[index]->first_pass_good_extends;
            search->second_pass_good_extends += array[index]->second_pass_good_extends;
            search->number_of_seqs_better_E += array[index]->number_of_seqs_better_E;
            search->prelim_gap_no_contest += array[index]->prelim_gap_no_contest;
            search->prelim_gap_passed += array[index]->prelim_gap_passed;
            search->prelim_gap_attempts += array[index]->prelim_gap_attempts;
            search->real_gap_number_of_hsps += array[index]->real_gap_number_of_hsps;
#endif
            /* Not copied at thread start. */
            /* search->blast_seqid_list = NULL; */
            array[index] = BlastSearchBlkDestruct(array[index]);	
        }
        array = MemFree(array);

        thread_array = MemFree(thread_array);
        
        NlmMutexDestroy(search->thr_info->db_mutex);
        search->thr_info->db_mutex = NULL;
        NlmMutexDestroy(search->thr_info->results_mutex);
        search->thr_info->results_mutex = NULL;
        NlmMutexDestroy(search->thr_info->ambiguities_mutex);
        search->thr_info->ambiguities_mutex = NULL;
    } else {
        if (search->pbp->gapped_calculation &&
            StringCmp(search->prog_name, "blastn") != 0)
            do_gapped_blast_search((VoidPtr) search);
        else
	   do_blast_search((VoidPtr) search);
    }
    search->rdfp = ReadDBCloseMHdrAndSeqFiles(search->rdfp); 
    if (time_out_boolean) {
        sprintf(buffer, "CPU limit exceeded");
        BlastConstructErrorMessage("Blast", buffer, 2, &(search->error_return));
	search->timed_out = TRUE;
    } 
    else
      /*
       * We've finished the search without timing out,
       * so we can ignore the soft limit now. -coulouri 20020104
       */
      {
        #ifdef OS_UNIX

	#ifdef SIG_IGN
        signal(SIGXCPU, SIG_IGN);
	#else
	sigignore(SIGXCPU);
	#endif

        #endif
      }
    
    return;
}

Uint1
FrameToDefine(Int2 frame)

{
	Uint1 retval;

	switch (frame) {
		case -1:
			retval = SEQLOC_MASKING_MINUS1;
			break;
		case -2:
			retval = SEQLOC_MASKING_MINUS2;
			break;
		case -3:
			retval = SEQLOC_MASKING_MINUS3;
			break;
		case 1:
			retval = SEQLOC_MASKING_PLUS1;
			break;
		case 2:
			retval = SEQLOC_MASKING_PLUS2;
			break;
		case 3:
			retval = SEQLOC_MASKING_PLUS3;
			break;
		default:
			retval = SEQLOC_MASKING_NOTSET;
			break;
	}

	return retval;
}
Int2
DefineToFrame(Uint1 define)

{
    Int2 frame;
    
    switch (define) {
    case SEQLOC_MASKING_MINUS1:
        frame = -1;
        break;
    case SEQLOC_MASKING_MINUS2:
        frame = -2;
        break;
    case SEQLOC_MASKING_MINUS3:
        frame = -3;
        break;
    case SEQLOC_MASKING_PLUS1:
        frame = 1;
        break;
    case SEQLOC_MASKING_PLUS2:
        frame = 2;
        break;
    case SEQLOC_MASKING_PLUS3:
        frame = 3;
        break;
    case SEQLOC_MASKING_NOTSET:
    default:
        frame = 0;
        break;
    }
    
    return frame;
}

CharPtr
BlastConstructFilterString(Int4 filter_value)

{
	Char buffer[32];
	CharPtr ptr;

	ptr = buffer;

	if (filter_value == FILTER_NONE)
		return NULL;
	
	if (filter_value & FILTER_DUST)
	{
		*ptr = 'D'; ptr++;
		*ptr = ';'; ptr++;
	}
	
	if (filter_value & FILTER_SEG)
	{
		*ptr = 'S'; ptr++;
		*ptr = ';'; ptr++;
	}

	*ptr = NULLB;

	return StringSave(buffer);
}

void
HackSeqLocId(SeqLocPtr slp, SeqIdPtr id)
{
	if (slp == NULL) {
		return;
	}
	switch (slp->choice) {
	case SEQLOC_BOND:
	case SEQLOC_FEAT:
		/* unsupported */
		/* assert(0); */
		break;
	case SEQLOC_NULL:
	case SEQLOC_EMPTY:
		break;
	case SEQLOC_WHOLE:
		SeqIdSetFree((SeqIdPtr)slp->data.ptrvalue);
		slp->data.ptrvalue = SeqIdDup(id);
		break;
	case SEQLOC_EQUIV:
	case SEQLOC_MIX:
	case SEQLOC_PACKED_INT:
		slp = (SeqLocPtr)slp->data.ptrvalue;
		for (; slp != NULL; slp = slp->next) {
			HackSeqLocId(slp, id);
		}
		break;
	case SEQLOC_INT:
		SeqIdSetFree(((SeqIntPtr)slp->data.ptrvalue)->id);
		((SeqIntPtr)slp->data.ptrvalue)->id = SeqIdDup(id);
		break;
	case SEQLOC_PNT:
		SeqIdSetFree(((SeqPntPtr)slp->data.ptrvalue)->id);
		((SeqPntPtr)slp->data.ptrvalue)->id = SeqIdDup(id);
		break;
	case SEQLOC_PACKED_PNT:
		SeqIdSetFree(((PackSeqPntPtr)slp->data.ptrvalue)->id);
		((PackSeqPntPtr)slp->data.ptrvalue)->id = SeqIdDup(id);
		break;
	/* default:
		assert(0); */
	}
}
/* This function assume specific structure of SeqLoc !! */
SeqLocPtr  blastDuplicateSeqLocInt(SeqLocPtr lcmask)
{
    SeqLocPtr dup_slp, slp, dup_head;
    SeqIntPtr sqip;

    if(lcmask == NULL)
        return NULL;

    /* Top level SeqLoc */    
    dup_head = dup_slp = ValNodeNew(NULL);
    dup_slp->choice = lcmask->choice;

    /* First seqLoc in lower level */

    slp = lcmask->data.ptrvalue;
    sqip = slp->data.ptrvalue;
    
    dup_slp->data.ptrvalue = (VoidPtr) SeqLocIntNew(sqip->from, sqip->to, sqip->strand, sqip->id);
    
    dup_slp = dup_slp->data.ptrvalue;
    
    /* Loop over all SeqIntPtr s in this SeqLoc */
    
    for(slp = slp->next; slp != NULL; slp = slp->next) {
        sqip = slp->data.ptrvalue;
        dup_slp->next = (VoidPtr) SeqLocIntNew(sqip->from, sqip->to, sqip->strand, sqip->id);
        dup_slp = dup_slp->next;
    }
    
    return dup_head;
}
/* This function use PACKED INT as mask */
void BLASTUpdateSeqIdInSeqInt(SeqLocPtr mask, SeqIdPtr sip)
{
    SeqLocPtr slp;
    SeqIntPtr sintp;

    if(mask == NULL)
        return;
    
    for(slp = mask->data.ptrvalue; slp != NULL; slp = slp->next) {
        if(slp->choice != SEQLOC_INT)
            continue;
        sintp = (SeqIntPtr)slp->data.ptrvalue;
        SeqIdSetFree(sintp->id);
        sintp->id = SeqIdDup(sip);
    }
    return;
}
/* This function use PACKED INT as lcmask */
static SeqLocPtr blastMergeFilterLocs(SeqLocPtr filter_slp, 
                                      SeqLocPtr lcmask, Boolean translate,
                                      Int2 frame, Int4 length)
{

    SeqLocPtr slp, dup_slp;

    if(filter_slp == NULL && lcmask == NULL)
        return NULL;

    if(lcmask == NULL)
        return filter_slp;
    
    dup_slp = blastDuplicateSeqLocInt(lcmask);

    /* Request to translate means, that lcmask is DNA SeqLoc, that should be
       translated into protein SeqLoc corresponding to the specific frame */

    if(translate) {
        BlastConvertDNASeqLoc(dup_slp, frame, length);
    }
    
    if(filter_slp == NULL) {
        return dup_slp;
    }

    /* OK We have 2 not NULL filters - merging... */
    
    if(filter_slp->choice == SEQLOC_PACKED_INT)
        slp = (SeqLocPtr) filter_slp->data.ptrvalue;
    else
        slp = filter_slp;
    
    if(slp == NULL) {
        ErrPostEx(SEV_WARNING, 0, 0, "Invalid filter detected");
        filter_slp->data.ptrvalue = dup_slp->data.ptrvalue;
        dup_slp->data.ptrvalue = NULL;
        SeqLocFree(dup_slp);
    } 
    else
    {
    	while(slp->next != NULL)
       		 slp = slp->next;
    
    	slp->next = dup_slp->data.ptrvalue;
    	dup_slp->data.ptrvalue = NULL;
    	SeqLocFree(dup_slp);
     }
    
    return filter_slp;
}

/* This function is used to filter one frame of the translated DNA
   sequence */
static void rpsBlastFilterSequence(BlastSearchBlkPtr search, Int4 frame,
                                   Uint1Ptr sequence, Int4 prot_length,
                                   Int4 dna_length)
{
    BioseqPtr bsp_temp;
    Boolean mask_at_hash = FALSE;
    SeqLocPtr filter_slp = NULL;
    
    if(search->pbp->query_lcase_mask == NULL) {
        if(search->pbp->filter_string == NULL || !StringICmp(search->pbp->filter_string, "F"))
            return;                 /* No filtering */
    }
    
    bsp_temp = BlastMakeTempProteinBioseq(sequence+1, prot_length, 
                                          Seq_code_ncbistdaa);
        
    filter_slp = BlastBioseqFilterEx(bsp_temp, search->pbp->filter_string, 
                                     &mask_at_hash);
    HackSeqLocId(filter_slp, search->subject_info->sip);
    
    if(search->pbp->query_lcase_mask != NULL) {
        filter_slp = blastMergeFilterLocs(filter_slp, search->pbp->query_lcase_mask, TRUE, frame, dna_length);
    }
    
    /* SeqMgrDeleteFromBioseqIndex(bsp_temp); */
    
    /* bsp_temp->id = SeqIdSetFree(bsp_temp->id); */
    bsp_temp = BioseqFree(bsp_temp);

    BlastMaskTheResidues(sequence+1, prot_length, 21, filter_slp, FALSE, 0);

    /* Conversion to ProteinSeqLoc will be done after original SeqLoc will
       be used once again on the Gapped extention stage */
    
    /*    BlastConvertProteinSeqLoc(filter_slp, frame, dna_length); */

    if (filter_slp)
        ValNodeAddPointer(&(search->mask), FrameToDefine(frame), filter_slp);
    
    return;
}
BlastSequenceBlkPtr PNTR LIBCALL
BlastMakeCopyQueryDNAP(BlastSequenceBlkPtr PNTR bsbpp_in)
{
    BlastSequenceBlkPtr PNTR bsbpp;
    Int4 buff_size, m;

    if(bsbpp_in == NULL)
        return NULL;

    bsbpp = MemNew(sizeof(BlastSequenceBlkPtr)*2);
    for(m = 0; m < 2; m++) {
        
        bsbpp[m] = (BlastSequenceBlkPtr) MemNew(sizeof(BlastSequenceBlk));
        
        buff_size = bsbpp_in[m]->length+3*CODON_LENGTH;
        bsbpp[m]->sequence_start = MemNew(buff_size);

        MemCpy(bsbpp[m]->sequence_start, 
               bsbpp_in[m]->sequence_start, buff_size);

        bsbpp[m]->sequence = bsbpp_in[m]->sequence;
        
        bsbpp[m]->length = bsbpp_in[m]->length;
        bsbpp[m]->original_length = bsbpp_in[m]->original_length;
        bsbpp[m]->effective_length = bsbpp_in[m]->effective_length;
    }

    return bsbpp;
}

void LIBCALL BlastFreeQueryDNAP(BlastSequenceBlkPtr PNTR bsbpp)
{
    Int4 m;
    
    for(m = 0; m < 2; m++) {
        BlastSequenceBlkDestruct(bsbpp[m]);
    }
    
    MemFree(bsbpp);
    
    return;
}

BlastSequenceBlkPtr PNTR LIBCALL
BlastCreateQueryDNAP(BlastSearchBlkPtr search, Int4 length)
{

    BlastSequenceBlkPtr PNTR bsbpp;
    Uint1Ptr dnap;
    Int4 i, j, k, m;
    Int4 shift;
    BLASTContextStructPtr context = search->context;
    Uint1 strand_option;

    if(context == NULL)
        return NULL;
    
    strand_option = search->last_context / CODON_LENGTH;

    bsbpp = MemNew(sizeof(BlastSequenceBlkPtr)*2);

    for(m = search->first_context/CODON_LENGTH; 
        m <= search->last_context/CODON_LENGTH; m++) {

        bsbpp[m] = (BlastSequenceBlkPtr) MemNew(sizeof(BlastSequenceBlk));
        
        dnap = MemNew(length+3*CODON_LENGTH);
        /* dnap = MemNew(length + 1); */
        
        dnap[0]=dnap[1]=dnap[2] = NULLB;
        
        shift = m*CODON_LENGTH;
        for (i = 0, j = 0; i < length+1;) {
            for(k = shift; k < shift + CODON_LENGTH; k++) {
                dnap[i] = context[k].query->sequence_start[j];
                i++;
            }
            j++;
        }
        BlastSequenceAddSequence(bsbpp[m], dnap+3, dnap, length, length, 0);
    }

    return bsbpp;
}

#define DROPOFF_NUMBER_OF_BITS 10.0
#define INDEX_THR_MIN_SIZE 20000
Int2 LIBCALL BLASTSetUpSearchInternalByLoc (BlastSearchBlkPtr search, SeqLocPtr query_slp, BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)(Int4 done, Int4 positives))

{
	BioseqPtr bsp_temp, bsp;
	Boolean mask_at_hash=FALSE, private_slp_delete;
	Boolean query_is_na, db_is_na;
	Char buffer[128];
	Int2 retval = 0, status, last_index;
	Int4 effective_query_length, query_length, full_query_length,
		index, length, length_adjustment=0, last_length_adjustment, min_query_length;
	Int4 array_size, max_length, block_width;
	Int4Ptr open, extend;
	Nlm_FloatHiPtr lambda, K, H;
	Nlm_FloatHi avglen;
	ReadDBFILEPtr rdfp;
	SeqIdPtr query_id;
	SeqPortPtr spp=NULL, spp_reverse=NULL;
	SeqLocPtr filter_slp=NULL, private_slp=NULL, private_slp_rev=NULL, private_slp_double=NULL;
	GeneticCodePtr gcp;
	Uint1 residue, strand;
	Uint1Ptr sequence;
	Uint1Ptr query_seq, query_seq_start, query_seq_rev, query_seq_start_rev;
	ValNodePtr vnp;
	Nlm_FloatHi alpha, beta; /*alpha and beta for new scoring system*/
        Nlm_FloatHi KtoUse, logKtoUse; /*K to use in length adjustment calculation*/
	Nlm_FloatHi LambdaToUse; /*Lambda to use in length adjuatment*/

	if (options == NULL)
	{
	  	ErrPostEx(SEV_FATAL, 0, 0, "BLAST_OptionsBlkPtr is NULL\n");
		return 1;
	}

	if (query_slp == NULL && query_bsp == NULL)
	{
	  	ErrPostEx(SEV_FATAL, 0, 0, "Query is NULL\n");
		return 1;
	}

	query_seq = NULL;	/* Gets rid of warning. */
	query_seq_rev = NULL;	/* Gets rid of warning. */
	query_seq_start = NULL;	/* Gets rid of warning. */
	query_seq_start_rev = NULL;	/* Gets rid of warning. */

        /* These parameters are used by translated RPS Blast */
        search->pbp->filter_string = StringSave(options->filter_string);
        search->pbp->is_rps_blast = options->is_rps_blast;
        search->pbp->query_lcase_mask = options->query_lcase_mask;
        search->pbp->is_ooframe = options->is_ooframe;
        search->pbp->shift_pen = options->shift_pen; 

	if (query_slp)
	{
		strand = SeqLocStrand(query_slp);
		if (strand == Seq_strand_unknown || strand == Seq_strand_plus || strand == Seq_strand_both)
		{
			private_slp = SeqLocIntNew(SeqLocStart(query_slp), SeqLocStop(query_slp), Seq_strand_plus, SeqLocId(query_slp));
		}
		if (strand == Seq_strand_minus || strand == Seq_strand_both)
		{
			private_slp_rev = SeqLocIntNew(SeqLocStart(query_slp), SeqLocStop(query_slp), Seq_strand_minus, SeqLocId(query_slp));
		}
		private_slp_delete = TRUE;
	   	if (search->prog_number==blast_type_blastn)
	      		search = BlastFillQueryOffsets(search, query_slp, 1); 

	}
	else
	{
		private_slp = SeqLocIntNew(0, query_bsp->length-1 , Seq_strand_plus, SeqIdFindBest(query_bsp->id, SEQID_GI));
		private_slp_rev = SeqLocIntNew(0, query_bsp->length-1 , Seq_strand_minus, SeqIdFindBest(query_bsp->id, SEQID_GI));
		private_slp_delete = FALSE;

		private_slp_double = SeqLocIntNew(0, query_bsp->length-1 , Seq_strand_both, SeqIdFindBest(query_bsp->id, SEQID_GI));
	   	if (search->prog_number==blast_type_blastn)
	      		search = BlastFillQueryOffsets(search,
                                                       private_slp_double, 1); 
		SeqLocFree(private_slp_double);
	}

	query_length = 0;
	if (private_slp)
		query_length = SeqLocLen(private_slp);
	else if (private_slp_rev)
		query_length = SeqLocLen(private_slp_rev);
	if (query_length == 0)
	{
		sprintf(buffer, "No valid query sequence");
		BlastConstructErrorMessage("Blast", buffer, 2,
                                           &(search->error_return));
                retval = 1;
		goto BlastSetUpReturn;
	}

	bsp = NULL;
        if (private_slp)
           bsp = BioseqLockById(SeqLocId(private_slp));
        else if (private_slp_rev)
           bsp = BioseqLockById(SeqLocId(private_slp_rev));
        
        if (bsp == NULL) {
           ErrPostEx(SEV_WARNING, 0, 0, "No valid query sequence, BioseqLockById returned NULL\n");
           retval = 1;
           goto BlastSetUpReturn;
        }
        full_query_length = bsp->length;
        
        BlastGetTypes(prog_name, &query_is_na, &db_is_na);
        if (query_is_na != ISA_na(bsp->mol))	{
           ErrPostEx(SEV_WARNING, 0, 0, "Query molecule is incompatible with %s program", prog_name);
           BioseqUnlock(bsp);
           retval = 1;
           goto BlastSetUpReturn;
        }
	if (bsp && bsp->repr == Seq_repr_virtual) {
           BioseqUnlock(bsp);
           ErrPostEx(SEV_WARNING, 0, 0, "Virtual sequence detected\n");
           retval = 1;
           goto BlastSetUpReturn;
	}
	BioseqUnlock(bsp);

	if (query_slp)	
	{
		search->query_slp = query_slp;
	}
	else
	{
		search->query_slp = private_slp;
		search->allocated += BLAST_SEARCH_ALLOC_QUERY_SLP;
	}
		

	search->translation_buffer = NULL;
	search->translation_buffer_size = 0;

	/* 
	Get genetic codes (should be determined from BLAST_OptionsBlkPtr.
	Only needed for blastx, tblast[nx] 
	*/
	if (StringCmp(prog_name, "blastp") != 0 && StringCmp(prog_name, "blastn") != 0)
	{

                if (StringCmp(prog_name, "tblastx") == 0 
		    || StringCmp(prog_name, "tblastn") == 0 
		    ||StringCmp(prog_name, "psitblastn") == 0)

		{
			gcp = GeneticCodeFind(options->db_genetic_code, NULL);
			for (vnp = (ValNodePtr)gcp->data.ptrvalue; vnp != NULL; vnp = vnp->next)
			{
				if (vnp->choice == 3)	/* ncbieaa */
				{
					search->db_genetic_code = (CharPtr)vnp->data.ptrvalue;
					break;
				}
			}
			search->translation_table = GetPrivatTranslationTable(search->db_genetic_code, FALSE);
			search->translation_table_rc = GetPrivatTranslationTable(search->db_genetic_code, TRUE);
			max_length = 0;
			rdfp = search->rdfp;
			while (rdfp)
			{
				max_length = MAX(max_length, readdb_get_maxlen(rdfp));
				rdfp = rdfp->next;
			}
			search->translation_buffer = MemNew((3+(max_length/3))*sizeof(Uint1));
			search->translation_buffer_size = 1+(max_length/3);
			search->allocated += BLAST_SEARCH_ALLOC_TRANS_INFO;
		}

		if (StringCmp(prog_name, "blastx") == 0 || StringCmp(prog_name, "tblastx") == 0)
		{
			gcp = GeneticCodeFind(options->genetic_code, NULL);
			for (vnp = (ValNodePtr)gcp->data.ptrvalue; vnp != NULL; vnp = vnp->next)
			{
				if (vnp->choice == 3)	/* ncbieaa */
				{
					search->genetic_code = (CharPtr)vnp->data.ptrvalue;
					break;
				}
			}
		}
	}

	if (options->filter && !options->filter_string)
		options->filter_string = BlastConstructFilterString(options->filter);

	/* If the query is translated do this below. */ 
	if (StringCmp(prog_name, "blastx") && 
            StringCmp(prog_name, "tblastx")) {
                                /* Futamura */
            if(!(search->pbp->is_rps_blast &&
                                  (!StringCmp(prog_name, "tblastn")||
                                   !StringCmp(prog_name, "psitblastn")))) {
                if (private_slp)
                    filter_slp = BlastSeqLocFilterEx(private_slp, options->filter_string, &mask_at_hash);
                else if (private_slp_rev)
                    filter_slp = BlastSeqLocFilterEx(private_slp_rev, options->filter_string, &mask_at_hash);
                
                /* If lower case characters were detected in the input
                   their locations will be masked out */
                
                if(search->pbp->query_lcase_mask != NULL) {
                    filter_slp = blastMergeFilterLocs(filter_slp, search->pbp->query_lcase_mask, FALSE, 0, 0);
                }
            }
        }

	/* 
           Dusting of query sequence. Only needed for blastn, optional
        */

        if(StringCmp(prog_name, "blastn") == 0) {
		if (filter_slp && !mask_at_hash)
			ValNodeAddPointer(&(search->mask), SEQLOC_MASKING_NOTSET, filter_slp);
        }


        if (StringCmp(prog_name, "blastp") == 0
            || StringCmp(prog_name, "tblastn") == 0
            || StringCmp(prog_name, "psitblastn") == 0)
	{
		spp = SeqPortNewByLoc(private_slp, Seq_code_ncbistdaa);
                SeqPortSet_do_virtual(spp, TRUE);
		if (filter_slp && !mask_at_hash)
			ValNodeAddPointer(&(search->mask), SEQLOC_MASKING_NOTSET, filter_slp);
	}
	else if (StringCmp(prog_name, "blastx") == 0 || StringCmp(prog_name, "tblastx") == 0 || StringCmp(prog_name, "blastn") == 0)
	{
		if (private_slp)
		{
			spp = SeqPortNewByLoc(private_slp, Seq_code_ncbi4na);
                	SeqPortSet_do_virtual(spp, TRUE);
		}
		if (private_slp_rev)
		{
			spp_reverse = SeqPortNewByLoc(private_slp_rev, Seq_code_ncbi4na);
                	SeqPortSet_do_virtual(spp_reverse, TRUE);
		}
	}
	else
	{
	  	ErrPostEx(SEV_FATAL, 0, 0, "Only blastn, blastp, blastx, tblastn tblastx is allowed\n");
                retval = 1;
		goto BlastSetUpReturn;
	}

	if (spp)
	{
		query_seq_start = (Uint1Ptr) MemNew(2*((query_length)+2)*sizeof(Char));
		query_seq_start[0] = NULLB;
		query_seq = query_seq_start+1;
		index=0;
		while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF)
		{

			if (IS_residue(residue))
			{
				query_seq[index] = residue;
				index++;
			}
		}
		query_seq[index] = NULLB;
		spp = SeqPortFree(spp);
		if (StringCmp(prog_name, "blastn") == 0)
		{
			if (filter_slp)
			{
				if (mask_at_hash)
                			search->context[0].location =
                        			BlastSeqLocFillDoubleIntEx(filter_slp, full_query_length, query_length, FALSE, SeqLocStart(private_slp));
				else
					BlastMaskTheResidues(query_seq, full_query_length, 15, filter_slp, FALSE, SeqLocStart(private_slp));
			}
			for (index=0; index<=query_length+1; index++)
				query_seq_start[index] = ncbi4na_to_blastna[query_seq_start[index]];
		}
	}

	if (spp_reverse)
	{
		query_seq_start_rev = (Uint1Ptr) MemNew(((query_length)+2)*sizeof(Char));
		query_seq_start_rev[0] = NULLB;
		query_seq_rev = query_seq_start_rev+1;
		index=0;
		while ((residue=SeqPortGetResidue(spp_reverse)) != SEQPORT_EOF)
		{
			if (IS_residue(residue))
			{
				query_seq_rev[index] = residue;
				index++;
			}
		}
		query_seq_rev[index] = NULLB;
		spp_reverse = SeqPortFree(spp_reverse);
		if (StringCmp(prog_name, "blastn") == 0)
		{
			if (filter_slp)
			{
			   if (mask_at_hash) 
			     search->context[0].location =
				BlastSeqLocFillDoubleIntRev(search->context[0].location, filter_slp, query_length, full_query_length, full_query_length - SeqLocStop(private_slp_rev) - 1);
			   else
			      BlastMaskTheResidues(query_seq_rev, full_query_length, 15, filter_slp, TRUE, full_query_length - SeqLocStop(private_slp_rev) - 1);
			}
			for (index=0; index<=query_length+1; index++)
				query_seq_start_rev[index] =
				   ncbi4na_to_blastna[query_seq_start_rev[index]];
			if (query_seq_start)
			   MemCpy(query_seq_start+query_length+1, 
				  query_seq_start_rev,query_length+2);
		}
	}

/*
	Set the context_factor, which specifies how many different 
	ways the query or db is examined (e.g., blastn looks at both
	stands of query, context_factor is 2).
*/
	if (StringCmp(prog_name, "blastp") == 0)
	{
		search->context_factor = 1;
		length = query_length;
	}
	else if (StringCmp(prog_name, "blastn") == 0)
	{	/* two strands concatenated in one sequence */
		search->context_factor = 1;
		length = query_length;
	}
	else if (StringCmp(prog_name, "blastx") == 0)
	{	/* query translated in six frames. */
		search->context_factor = search->last_context-search->first_context+1;
		length = query_length/3;
	}
        else if ( (StringCmp(prog_name, "tblastn") == 0)
                  || (StringCmp(prog_name, "psitblastn") == 0))
	{	/* db translated in six frames. */
		search->context_factor = 6;
		length = query_length;
	}
	else if (StringCmp(prog_name, "tblastx") == 0)
	{	/* db and query each translated in six frames. */
		search->context_factor = 6*CODON_LENGTH*(search->last_context-search->first_context+1);
		length = query_length/3;
	}
	else
	{
		sprintf(buffer, "%s is not a valid program name", prog_name);
		BlastConstructErrorMessage("BLASTSetUpSearch", buffer, 2, &(search->error_return));
                retval = 1;
		goto BlastSetUpReturn;
	}

	if (private_slp)
		query_id = SeqIdFindBest(SeqLocId(private_slp), SEQID_GI);
	else
		query_id = SeqIdFindBest(SeqLocId(private_slp_rev), SEQID_GI);

	search->query_id = SeqIdDup(query_id);

/* Store the query sequence, or the translation thereof. */
        if (StringCmp(prog_name, "blastp") == 0
            || StringCmp(prog_name, "tblastn") == 0
            || StringCmp(prog_name, "psitblastn") == 0)
	{	/* One blastp context for now. */
		if (filter_slp)
		{
			if (mask_at_hash)
                		search->context[0].location =
                        		BlastSeqLocFillDoubleInt(filter_slp, query_length, FALSE);
			else
				BlastMaskTheResidues(query_seq, full_query_length, 21, filter_slp, FALSE, SeqLocStart(private_slp));
		}
		BlastSequenceAddSequence(search->context[0].query, NULL, query_seq_start, query_length, query_length, 0);
	}
	else if (StringCmp(prog_name, "blastx") == 0  || StringCmp(prog_name, "tblastx") == 0)
	{
		
		for (index=search->first_context; index<=search->last_context; index++)
		{
		   if (search->context[index].query->frame > 0)
		   {
			sequence = GetTranslation(query_seq, query_length, search->context[index].query->frame, &length, search->genetic_code);
		   }
		   else
		   {
			sequence = GetTranslation(query_seq_rev, query_length, search->context[index].query->frame, &length, search->genetic_code);
		   }
		   if (options->filter_string && length > 0)
		   {
		  	bsp_temp = BlastMakeTempProteinBioseq(sequence+1, length, Seq_code_ncbistdaa);
			
			filter_slp = BlastBioseqFilterEx(bsp_temp, options->filter_string, &mask_at_hash);
			HackSeqLocId(filter_slp, search->query_id);

                        /* If FASTA filtering is set - updating this SeqLoc */
                        if(search->pbp->query_lcase_mask != NULL) {
                            filter_slp = blastMergeFilterLocs(filter_slp, search->pbp->query_lcase_mask, TRUE, search->context[index].query->frame, query_length);
                        }

			/* SeqMgrDeleteFromBioseqIndex(bsp_temp); */
			
			/* bsp_temp->id = SeqIdSetFree(bsp_temp->id); */

			bsp_temp = BioseqFree(bsp_temp);
			if (mask_at_hash)
			{
                		search->context[index].location = 
					BlastSeqLocFillDoubleInt(filter_slp, query_length, FALSE);
			}
			else
			{
				BlastMaskTheResidues(sequence+1, length, 21, filter_slp, FALSE, 0);
				BlastConvertProteinSeqLoc(filter_slp, search->context[index].query->frame, query_length);
			}
			if (filter_slp && !mask_at_hash)
				ValNodeAddPointer(&(search->mask), FrameToDefine(search->context[index].query->frame), filter_slp);
		   }
		   BlastSequenceAddSequence(search->context[index].query, NULL, sequence, length, query_length, 0);
		}
		query_seq_start = MemFree(query_seq_start);
		query_seq_start_rev = MemFree(query_seq_start_rev);

                if(search->pbp->is_ooframe) {
                    search->query_dnap = BlastCreateQueryDNAP(search, query_length);
                }
	} else if (StringCmp(prog_name, "blastn") == 0) {
	   if (search->last_context - search->first_context > 0) {
	      /* Both strands are searched */
	      BlastSequenceAddSequence(search->context[search->first_context].query, NULL, query_seq_start, 2*query_length+2, 2*query_length+2, 0);
	      BlastSequenceAddSequence(search->context[search->last_context].query, NULL,
				       query_seq_start_rev, query_length,
				       query_length, 0);
	   } else if (search->first_context==0) 
	      /* Only first strand is searched */
	      BlastSequenceAddSequence(search->context[search->first_context].query, NULL, query_seq_start, query_length+1, query_length+1, 0);
	   else {/* Only second strand is searched */
	      BlastSequenceAddSequence(search->context[search->first_context].query, NULL,
				       query_seq_start_rev, query_length+1,
				       query_length+1, 0);
	   }
	}

	if (mask_at_hash)
	{ /* No longer needed. */
		filter_slp = SeqLocSetFree(filter_slp);
	}
	
/* Set the ambiguous residue before the ScoreBlk is filled. */
	if (StringCmp(prog_name, "blastn") != 0)
	{
		search->sbp->read_in_matrix = TRUE;
		BlastScoreSetAmbigRes(search->sbp, 'X');
	}
	else
	{
  	        if(options->matrix!=NULL) {
		     search->sbp->read_in_matrix = TRUE;
	        } else {
		     search->sbp->read_in_matrix = FALSE;
	        }
		BlastScoreSetAmbigRes(search->sbp, 'N');
	}


	search->sbp->penalty = options->penalty;
	search->sbp->reward = options->reward;
        
        /* option is to use alignments chosen by user in PSM computation API (used in WWW PSI-Blast); */
	search->pbp->use_best_align = options->use_best_align;


	/* Should culling be used at all? */
	search->pbp->perform_culling = options->perform_culling;
	search->pbp->hsp_range_max = options->hsp_range_max;
        /* This assures that search->pbp->max_pieces is at least one wide. */
        block_width = MIN(query_length, options->block_width);
        if (block_width > 0)
           search->pbp->max_pieces = query_length/block_width;

	search->sbp->query_length = query_length;

	search->result_struct = BLASTResultsStructNew(search->result_size,
						      search->pbp->max_pieces,
						      search->pbp->hsp_range_max);
	   
	if (options->matrix != NULL)
		status = BlastScoreBlkMatFill(search->sbp, options->matrix);
	else 
		status = BlastScoreBlkMatFill(search->sbp, "BLOSUM62");

	if (status != 0)
	{
		ErrPostEx(SEV_WARNING, 0, 0, "BlastScoreBlkMatFill returned non-zero status");
                retval = 1;
                goto BlastSetUpReturn;
	}

	/* This is used right below. */
	search->pbp->gapped_calculation = options->gapped_calculation;
	search->pbp->do_not_reevaluate = options->do_not_reevaluate;
	search->pbp->do_sum_stats = options->do_sum_stats;
	search->pbp->first_db_seq = options->first_db_seq;
	search->pbp->final_db_seq = options->final_db_seq;

	retval = 0;
	for (index=search->first_context; index<=search->last_context; index++)
	{
	   if (search->prog_number != blast_type_blastn || 
	       index>search->first_context || 
	       search->last_context==search->first_context)
	      status = BlastScoreBlkFill(search->sbp, (CharPtr)
					   search->context[index].query->sequence,search->context[index].query->length, index);
	   else
	      status = BlastScoreBlkFill(search->sbp, (CharPtr)
					 search->context[index].query->sequence, 
					 search->context[index+1].query->length, 
					 index);
		if (status != 0)
		{
			sprintf(buffer, "Unable to calculate Karlin-Altschul params, check query sequence");
			BlastConstructErrorMessage("BLASTSetUpSearch", buffer, 2, &(search->error_return));
			retval = 1;
		}
		if (search->pbp->gapped_calculation &&
				StringCmp(search->prog_name, "blastn") != 0)
		{
			search->sbp->kbp_gap_std[index] = BlastKarlinBlkCreate();
                	status = BlastKarlinBlkGappedCalcEx(search->sbp->kbp_gap_std[index], options->gap_open, options->gap_extend, options->decline_align, search->sbp->name, &(search->error_return));
			if (status != 0)
			{
				retval = 1;
			}
			search->sbp->kbp_gap_psi[index] = BlastKarlinBlkCreate();
                	status = BlastKarlinBlkGappedCalcEx(search->sbp->kbp_gap_psi[index], options->gap_open, options->gap_extend, options->decline_align, search->sbp->name, &(search->error_return));
			if (status != 0)
			{
				retval = 1;
			}
		}
	}

	search->sbp->kbp_gap = search->sbp->kbp_gap_std;
        search->sbp->kbp = search->sbp->kbp_std;
	if (StringCmp(prog_name, "blastn") != 0)
	{
		array_size = BlastKarlinGetMatrixValues(search->sbp->name, &open, &extend, &lambda, &K, &H, NULL);
		if (array_size > 0)
		{
			for (index=0; index<array_size; index++)
			{
				if (open[index] == INT2_MAX && extend[index] == INT2_MAX)
				{
					search->sbp->kbp_ideal = BlastKarlinBlkCreate();
					search->sbp->kbp_ideal->Lambda = lambda[index];
					search->sbp->kbp_ideal->K = K[index];
					search->sbp->kbp_ideal->H = H[index];
				}
			}
			MemFree(open);
			MemFree(extend);
			MemFree(lambda);
			MemFree(K);
			MemFree(H);
		}
		if (search->sbp->kbp_ideal == NULL)
        		search->sbp->kbp_ideal = BlastKarlinBlkStandardCalcEx(search->sbp);
	}

	/* Adjust the Karlin parameters. */
	if (StringCmp(prog_name, "blastx") == 0  || StringCmp(prog_name, "tblastx") == 0)
	{
                BlastKarlinBlkStandardCalc(search->sbp, search->first_context, search->last_context);
	}

	/* If retval was set non-zero above (by the routines calculating Karlin-Altschul params),
	   return here before these values are used.
	*/
	if (retval)
           goto BlastSetUpReturn;

	if (search->pbp->gapped_calculation &&
		StringCmp(search->prog_name, "blastn") != 0)
		min_query_length = (Int4) 1/(search->sbp->kbp_gap_std[search->first_context]->K);
	else
		min_query_length = (Int4) 1/(search->sbp->kbp[search->first_context]->K);

	if (search->prog_number != blast_type_blastn)
	{
	
		getAlphaBeta(options->matrix,&alpha,&beta,search->pbp->gapped_calculation, search->pbp->gap_open, search->pbp->gap_extend);
		if (search->pbp->gapped_calculation) {
	  		KtoUse = search->sbp->kbp_gap_std[search->first_context]->K;
	  		logKtoUse = search->sbp->kbp_gap_std[search->first_context]->logK;
	  		LambdaToUse = search->sbp->kbp_gap_std[search->first_context]->Lambda;
		}
		else 
		{
	  		KtoUse = search->sbp->kbp[search->first_context]->K;
	  		logKtoUse = search->sbp->kbp[search->first_context]->logK;
	  		LambdaToUse = search->sbp->kbp[search->first_context]->Lambda;
		}
	}
	last_length_adjustment = 0;
	for (index=0; index<5; index++)
	{
		if (search->prog_number != blast_type_blastn)
		{
	  		length_adjustment = Nlm_Nint(((logKtoUse+log((Nlm_FloatHi)(length-last_length_adjustment)*(Nlm_FloatHi)(MAX(search->dbseq_num, (search->dblen)-(search->dbseq_num*last_length_adjustment)))))*alpha/LambdaToUse) + beta);
		}
		else
		{
			length_adjustment = (Int4) ((search->sbp->kbp[search->first_context]->logK)+log((Nlm_FloatHi)(length-last_length_adjustment)*(Nlm_FloatHi)(MAX(1, (search->dblen)-(search->dbseq_num*last_length_adjustment)))))/(search->sbp->kbp[search->first_context]->H);
		}
		if (length_adjustment >= length-min_query_length)
		{
			length_adjustment = length-min_query_length;
			break;
		}
	
		if (ABS(last_length_adjustment-length_adjustment) <= 1)
			break;
		last_length_adjustment = length_adjustment;
	}
	search->length_adjustment = MAX(length_adjustment, 0);

	if (search->prog_number != blast_type_blastn)
		search->dblen_eff = MAX(search->dbseq_num, search->dblen - search->dbseq_num*search->length_adjustment); 
	else
		search->dblen_eff = MAX(1, search->dblen - search->dbseq_num*search->length_adjustment);
	effective_query_length = MAX(length - search->length_adjustment, min_query_length);
	
	for (index=search->first_context; index<=search->last_context; index++)
	{
		search->context[index].query->effective_length = effective_query_length;
	}

	if (search->searchsp_eff == 0)
		search->searchsp_eff = ((Nlm_FloatHi) search->dblen_eff)*((Nlm_FloatHi) effective_query_length);

	/* The default is that cutoff_s was not set and is zero. */
	if (options->cutoff_s == 0)
	{
		search->pbp->cutoff_e = options->expect_value;
		search->pbp->cutoff_e_set = TRUE;
		search->pbp->cutoff_s = options->cutoff_s;
		search->pbp->cutoff_s_set = FALSE;
	}
	else
	{
		search->pbp->cutoff_e = options->expect_value;
		search->pbp->cutoff_e_set = FALSE;
		search->pbp->cutoff_s = options->cutoff_s;
		search->pbp->cutoff_s_set = TRUE;
	}
/* For now e2 is set to 0.5 and cutoff_e2_set is FALSE.  This is then
changed to the proper values in blast_set_parameters.  In the final version
of this program (where more blast programs and command-line options are
available) this needs to be set higher up. */
	if (options->cutoff_s2 == 0)
	{
		search->pbp->cutoff_e2 = options->e2;
		search->pbp->cutoff_e2_set = TRUE;
		search->pbp->cutoff_s2 = options->cutoff_s2;
		search->pbp->cutoff_s2_set = FALSE;
	}
	else
	{
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
        search->pbp->total_hsp_limit = options->total_hsp_limit;

	search->pbp->hsp_num_max = options->hsp_num_max;
/* CHANGE HERE??? */
	if (search->pbp->gapped_calculation && StringCmp(search->prog_name, "blastn"))
	{
/*
		search->pbp->cutoff_s2_set = TRUE;
*/
		if (StringCmp(search->prog_name, "blastn") != 0)
		{
			search->pbp->gap_x_dropoff = (BLAST_Score) (options->gap_x_dropoff*NCBIMATH_LN2 / search->sbp->kbp_gap[search->first_context]->Lambda);
			search->pbp->gap_x_dropoff_final = (BLAST_Score) (options->gap_x_dropoff_final*NCBIMATH_LN2 / search->sbp->kbp_gap[search->first_context]->Lambda);
			search->pbp->gap_trigger = (BLAST_Score) ((options->gap_trigger*NCBIMATH_LN2+search->sbp->kbp[search->first_context]->logK)/ search->sbp->kbp[search->first_context]->Lambda);
		}
		else
		{
			search->pbp->gap_x_dropoff = (BLAST_Score) (options->gap_x_dropoff*NCBIMATH_LN2 / search->sbp->kbp[search->first_context]->Lambda);
			search->pbp->gap_x_dropoff_final = (BLAST_Score) (options->gap_x_dropoff_final*NCBIMATH_LN2 / search->sbp->kbp[search->first_context]->Lambda);
			search->pbp->gap_trigger = (BLAST_Score) ((options->gap_trigger*NCBIMATH_LN2+search->sbp->kbp[search->first_context]->logK)/ search->sbp->kbp[search->first_context]->Lambda);
		}
		/* The trigger value sets the s2 cutoff. */
		search->pbp->cutoff_s2 = (Int4) search->pbp->gap_trigger;
	}
	else
	{
		search->pbp->gap_x_dropoff = (BLAST_Score) (options->gap_x_dropoff*NCBIMATH_LN2 / search->sbp->kbp[search->first_context]->Lambda);
		search->pbp->gap_x_dropoff_final = (BLAST_Score) (options->gap_x_dropoff_final*NCBIMATH_LN2 / search->sbp->kbp[search->first_context]->Lambda);
		search->pbp->gap_trigger = (BLAST_Score) ((options->gap_trigger*NCBIMATH_LN2+search->sbp->kbp[search->first_context]->logK)/ search->sbp->kbp[search->first_context]->Lambda);
		/* Set S and S2 equal if not sum stats. */
		if (search->pbp->do_sum_stats == FALSE)
			search->pbp->cutoff_s2 = search->pbp->cutoff_s;
	}
	/* Ensures that gap_x_dropoff_final is at least as large as gap_x_dropoff. */
	search->pbp->gap_x_dropoff_final = MAX(search->pbp->gap_x_dropoff_final, search->pbp->gap_x_dropoff);

/* "threshold" (first and second) must be set manually for two-pass right now.*/
	search->pbp->threshold_set = TRUE;
	search->pbp->threshold_second = options->threshold_second;

	search->pbp->window_size = options->window_size;
	search->pbp->window_size_set = TRUE;

	search->whole_query = TRUE;
	if (options->required_start != 0 || options->required_end != -1)
	{
		search->whole_query = FALSE;
		search->required_start = options->required_start;
		if (options->required_end != -1)
			search->required_end = options->required_end;
		else
			search->required_end = query_length;
	}

	if (qlen <= 0)
		qlen = query_length;

	/* Use DROPOFF_NUMBER_OF_BITS as the default if it's set to zero. */
	if (options->dropoff_1st_pass == 0)
		options->dropoff_1st_pass = (Int4) DROPOFF_NUMBER_OF_BITS;

	if (options->dropoff_2nd_pass == 0)
		options->dropoff_2nd_pass = (Int4) DROPOFF_NUMBER_OF_BITS;

	search->pbp->no_check_score = options->no_check_score;

	if (StringCmp(search->prog_name, "blastn") != 0)
	{
		avglen = BLAST_AA_AVGLEN;
	}
	else
	{
		avglen = BLAST_NT_AVGLEN;
		/* Use only one type of gap for blastn */
		search->pbp->ignore_small_gaps = FALSE;
	}

	if (search->rdfp)
	{
		Int4 total_number;
		Int8 total_length;

		readdb_get_totals(search->rdfp, &total_length, &total_number);
		if (total_number > 0)
			avglen = total_length/total_number;
	}

	if (blast_set_parameters(search, options->dropoff_1st_pass, options->dropoff_2nd_pass, avglen, search->searchsp_eff, options->window_size) != 0) {
           retval = 1;
           goto BlastSetUpReturn;
        }
	if (options->scalingFactor == 0.0)
		options->scalingFactor = 1.0;

	if (options->scalingFactor != 0.0 && options->scalingFactor != 1.0)
	{
		search->pbp->gap_open *= options->scalingFactor;
		search->pbp->gap_extend *= options->scalingFactor;
		search->pbp->dropoff_1st_pass *= options->scalingFactor;
		search->pbp->dropoff_2nd_pass *= options->scalingFactor;
		search->pbp->gap_x_dropoff *= options->scalingFactor;
		search->pbp->gap_x_dropoff_final *= options->scalingFactor;
		search->pbp->decline_align *= options->scalingFactor;
		search->pbp->gap_trigger *= options->scalingFactor;
		search->pbp->cutoff_s *= options->scalingFactor;
		search->pbp->cutoff_s1 *= options->scalingFactor;
		search->pbp->cutoff_s2 *= options->scalingFactor;
		search->pbp->cutoff_s2_max *= options->scalingFactor;
		search->pbp->cutoff_s_first *= options->scalingFactor;
		search->pbp->cutoff_s_second *= options->scalingFactor;
	}
	search->pbp->scalingFactor = options->scalingFactor;
        if (options->is_megablast_search) 
           search->pbp->mb_params = MegaBlastParameterBlkNew(options);
	search->pbp->explode_seqids = options->explode_seqids;
        
        if (search->prog_number == blast_type_tblastn)
           search->pbp->longest_intron = options->longest_intron / 3;

	if (search->pbp->multiple_hits_only)
	{ 	
		if (search->context[search->first_context].query->length < 2*options->wordsize)
		{
			BlastConstructErrorMessage("Blast", 
				"Query must be at least twice wordsize for two hit mode", 2, &(search->error_return));
                        retval = 1;
			goto BlastSetUpReturn;
		}
	}
	else
	{
		if (search->context[search->first_context].query->length < options->wordsize)
		{
			BlastConstructErrorMessage("Blast", buffer, 2, &(search->error_return));
			BlastConstructErrorMessage("Blast", 
				"Query must be at least wordsize", 2, &(search->error_return));
                        retval = 1;
			goto BlastSetUpReturn;
		}
	}
		
	search->thr_info->awake_index = FALSE;
	if (NlmThreadsAvailable() && (search->context_factor*query_length) > INDEX_THR_MIN_SIZE) {
            search->thr_info->awake_index = TRUE;
            search->thr_info->last_tick = Nlm_GetSecs();
            search->thr_info->index_thr = 
                NlmThreadCreate(index_proc, search->thr_info);
            search->thr_info->index_callback = callback;
	}
        
	/* Only do this if this is not a pattern search. */
	if (options->isPatternSearch == FALSE && search->pbp->is_rps_blast == FALSE)
	{
	   if (StrCmp(search->prog_name, "blastn")) 
	      last_index = search->last_context;
	   else 
	      last_index = search->first_context;
	   for (index=search->first_context; index<=last_index; index++)
	   {
		if (options->threshold_second > 0)
		{
			search->wfp = search->wfp_first;
			if (!(search->positionBased)) /*AAS*/
			    status = BlastFindWords(search, 0, search->context[index].query->length, options->threshold_second, (Uint1) index);
			else
			    status = BlastNewFindWords(search, 0, search->context[index].query->length, options->threshold_second, (Uint1) index);
			if (status != 0) {
                            search->thr_info->awake_index = FALSE;
                            ErrPostEx(SEV_WARNING, 0, 0, "BlastFindWords returned non-zero status");
                            retval = 1;
                            goto BlastSetUpReturn;
			}
		}
		search->wfp = search->wfp_second;
		if (StringCmp(prog_name, "blastn") != 0)
		{
		    if (search->allocated & BLAST_SEARCH_ALLOC_WFP_SECOND)
		    {
			if (!(search->positionBased))
			    status = BlastFindWords(search, 0, search->context[index].query->length, options->threshold_second, (Uint1) index);
			else
			    status = BlastNewFindWords(search, 0, search->context[index].query->length, options->threshold_second, (Uint1) index);
		    }
		}
		else
		{ 
			status = BlastNtFindWords(search, 0, search->context[index].query->length, 
		 		                      (Uint1) index);
		}

		search->context[index].location = ValNodeFree(search->context[index].location);

		if (status > 0)
		{
			search->thr_info->awake_index = FALSE;
			sprintf(buffer, "No valid letters to be indexed");
			BlastConstructErrorMessage("Blast", buffer, 2, &(search->error_return));
                        retval = 1;
			goto BlastSetUpReturn;
		}
		else if (status < 0)
		{
			search->thr_info->awake_index = FALSE;
			sprintf(buffer, "Error finding words");
			BlastConstructErrorMessage("Blast", buffer, 2, &(search->error_return));
                        retval = 1;
			goto BlastSetUpReturn;
		}
	   }
	   if (StrCmp(search->prog_name, "blastn"))
	      lookup_position_aux_destruct(search->wfp->lookup);
	   else
	      mb_lookup_position_aux_destruct(search->wfp->lookup);
	}
	/* 
	Turn off the index thread by setting this flag.  Don't wait for a join, as the
	search will take much longer than the one second for this to die.
	*/
	search->thr_info->awake_index = FALSE;
 BlastSetUpReturn:
	if (private_slp && private_slp_delete)
		private_slp = SeqLocFree(private_slp);
	if (private_slp_rev)
		private_slp_rev = SeqLocFree(private_slp_rev);

	return retval;
}

Boolean 
BlastGetFirstAndLastContext(CharPtr prog_name, SeqLocPtr query_slp, Int2Ptr first_context, Int2Ptr last_context, Uint1 strand_options)
{
	Uint1 strand;

	if (query_slp == NULL)
	{	/* Query was a BioseqPtr, Check strand_options. */
		strand = Seq_strand_both;
	}
	else
	{
		strand = SeqLocStrand(query_slp);
	}
	
	/* 
	Check the strand_options and use that if top or bottom is specified. 
	otherwise use what's specified above. 
	*/
	if (strand_options == BLAST_TOP_STRAND)
		strand = Seq_strand_plus;
	else if (strand_options == BLAST_BOTTOM_STRAND)
		strand = Seq_strand_minus;
	
        if (StringCmp(prog_name, "blastp") == 0
            || StringCmp(prog_name, "tblastn") == 0
            ||  StringCmp(prog_name, "psitblastn") == 0)
	{
		*first_context = 0;
		*last_context = 0;
	}
	else if (StringCmp(prog_name, "blastx") == 0 || StringCmp(prog_name, "tblastx") == 0)
	{
		if (strand == Seq_strand_unknown || strand == Seq_strand_plus || strand == Seq_strand_both)
			*first_context = 0;
		else 
			*first_context = 3;
			
		if (strand == Seq_strand_minus || strand == Seq_strand_both)
			*last_context = 5;
		else
			*last_context = 2;
	}
	else if (StringCmp(prog_name, "blastn") == 0)
	{
		if (strand == Seq_strand_unknown || strand == Seq_strand_plus || strand == Seq_strand_both)
			*first_context = 0;
		else 
			*first_context = 1;
			
		if (strand == Seq_strand_minus || strand == Seq_strand_both)
			*last_context = 1;
		else
			*last_context = 0;
	}
	return TRUE;
}


/*
	This function reads in a list of gi's from a file.
The file may be either in binary or text format.  

The binary gilist format has the following construction:

1.) 1st 4 bytes: a 'magic' number: UINT4_MAX
2.) 2nd 4 bytes: total number of gi's in the file (call this value 'number').
3.) 'number' set of 4 bytes, allowing 4 bytes for each gi.

The function GetGisFromFile first checks what the first 4 bytes
of a file are, if they are the 'magic' number, then it proceeds
to read values assuming a binary format.  If they are not the
'magic' number, then a text format is assumed.

The binary gilist can be produced from a text gilist using the
function readdb_MakeGiFileBinary.

*/

#define	LINE_LEN	1024
BlastDoubleInt4Ptr 
GetGisFromFile (CharPtr gifile, Int4Ptr gi_list_size)

{
    BlastDoubleInt4Ptr	gi_list;
    FILE		*gifp = NULL;
    Int4		index = 0, value, chunk_size = 24, number;
    Int2		status;
    Char		line[LINE_LEN];
    long		tmplong;
    NlmMFILEPtr		mfp;
    Uint4Ptr		tmp_value;
    Char	        file_name[PATH_MAX], blast_dir[PATH_MAX];
    
    /**
     * first looking in current directory, then checking .ncbirc,
     * then $BLASTDB and then assuming BLASTDB_DIR
     */
    if (FileLength(gifile) > 0) {
       char *path = Nlm_FilePathFind(gifile);
       if (StringLen(path) > 0) {
          StringCpy(blast_dir, path);
       } else {
          StringCpy(blast_dir, ".");
       }
       MemFree(path);
    } else {
#ifdef OS_UNIX
       if (getenv("BLASTDB"))
          Nlm_GetAppParam("NCBI", "BLAST", "BLASTDB", getenv("BLASTDB"), blast_dir, PATH_MAX);
       else
#endif
          Nlm_GetAppParam ("NCBI", "BLAST", "BLASTDB", BLASTDB_DIR, blast_dir, PATH_MAX);
    }
    sprintf(file_name, "%s%s%s", blast_dir, DIRDELIMSTR, FileNameFind(gifile));


    
    mfp = NlmOpenMFILE(file_name);
    if (mfp == NULL) {
        ErrPostEx(SEV_ERROR, 0, 0, "Unable to open file %s", file_name);
        return NULL;
    }
    
    tmp_value = (Uint4Ptr) mfp->mmp;
    if (SwapUint4(*tmp_value) == READDB_MAGIC_NUMBER) {
        mfp->mmp += 4;
        tmp_value = (Uint4Ptr) mfp->mmp;
        number = SwapUint4(*tmp_value);
        gi_list = MemNew(number * sizeof(BlastDoubleInt4));
        index = 0;
        while (index<number) {
            mfp->mmp += 4;
            tmp_value = (Uint4Ptr) mfp->mmp;
            gi_list[index++].gi = SwapUint4(*tmp_value);
        }
        *gi_list_size = number;
        mfp = NlmCloseMFILE(mfp);
    } else {
        mfp = NlmCloseMFILE(mfp);
        if (!(gifp = FileOpen(file_name, "r"))) {
            ErrPostEx(SEV_ERROR, 0, 0, "Unable to open file %s", file_name);
            return NULL;
        }
	
        gi_list = MemNew(chunk_size * sizeof(BlastDoubleInt4));
        
        while (FileGets(line, LINE_LEN, gifp)) {
            
            /* do correct casting */
            status = sscanf(line, "%ld", &tmplong);
            value = tmplong;
            
            /* skip non-valid lines */
            if (status > 0) {
				/* do we have enough space in gi_list ? */
                if (chunk_size < index + 1) {
                    chunk_size *= 2;
                    gi_list = Realloc(gi_list, chunk_size * sizeof(BlastDoubleInt4));
                }
                
                gi_list[index++].gi = value;
            }
        }
        
        *gi_list_size = index;
    }

    if(gifp != NULL)
        FileClose(gifp);

    return gi_list;
}

BlastSearchBlkPtr LIBCALL
BLASTSetUpSearchWithReadDbInternalEx (SeqLocPtr query_slp, BioseqPtr query_bsp,
				      CharPtr prog_name, Int4 qlen, CharPtr
				      dbname, BLAST_OptionsBlkPtr options, int
				      (LIBCALLBACK *callback)PROTO((Int4 done,
								    Int4
								    positives)),
				      SeqIdPtr seqid_list, BlastDoubleInt4Ptr
				      gi_list, Int4 gi_list_total, ReadDBFILEPtr
				      rdfp)
{
   if (options->is_megablast_search)
      return MegaBlastSetUpSearchWithReadDbInternal(query_slp, query_bsp,
						    prog_name, 0,
						    dbname, options, callback,
						    seqid_list, gi_list,
						    gi_list_total, NULL);
   else
      return BLASTSetUpSearchWithReadDbInternal(query_slp, query_bsp,
						prog_name, qlen,
						dbname, options, callback,
						seqid_list, gi_list,
						gi_list_total, rdfp);
}



BlastSearchBlkPtr
BLASTSetUpSearchWithReadDbInternal (SeqLocPtr query_slp, BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, CharPtr dbname, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total, ReadDBFILEPtr rdfp)

{

    BlastSearchBlkPtr search;
    Boolean multiple_hits, options_alloc=FALSE;
    Int2 status, first_context, last_context;
    Int8	dblen;
    Int4	query_length;
    Nlm_FloatHi	searchsp_eff=0;
    
    /* Allocate default options if none are allocated yet. */
    if (options == NULL) {
        options = BLASTOptionNew(prog_name, FALSE);
        options_alloc = TRUE;
    }
    
    multiple_hits = options->multiple_hits_only;
    /*
    if (options->window_size != 0)
        multiple_hits = TRUE;
    else
        multiple_hits = FALSE;
    */
    BlastGetFirstAndLastContext(prog_name, query_slp, &first_context, &last_context, options->strand_option);
    
    if (query_slp)
        query_length = SeqLocLen(query_slp);
    else
        query_length = query_bsp->length;
    
    /* On the first call query length is used for the subject length. */
    search = BlastSearchBlkNewExtra(options->wordsize, query_length, dbname, multiple_hits, 0, options->threshold_second, options->hitlist_size, prog_name, NULL, first_context, last_context, rdfp, options->window_size);
    
    if (search) {
       readdb_get_totals_ex(search->rdfp, &(dblen), &(search->dbseq_num), TRUE);

       if (seqid_list && !options->use_real_db_size)
          BlastAdjustDbNumbers(search->rdfp, &(dblen), &(search->dbseq_num),
                               seqid_list, NULL, NULL, NULL, 0);
   
       /* Create virtual database if any of the databases have gi lists or 
          ordinal id masks, or if gi list is provided from options */
       BlastProcessGiLists(search, options, gi_list, &gi_list_total);
       /* Intended for use when a list of gi's is sent in, but the real size is needed. */
        /* It's probably still necessary to call BlastAdjustDbNumbers, but it would be nice
           if this were not required. */
       if (gi_list_total > 0 && !options->use_real_db_size)
          BlastAdjustDbNumbers(search->rdfp, &(dblen), 
                               &(search->dbseq_num), NULL, NULL, 
                               search->rdfp->oidlist, NULL, 0);

#if 0
        /* use length and num of seqs of the database from alias file */
        if (search->rdfp->aliaslen && !gi_list)
            dblen = search->rdfp->aliaslen;
        if (search->rdfp->aliasnseq && !gi_list) 
            search->dbseq_num = search->rdfp->aliasnseq;
#endif
        /* command-line/options trump alias file. */
        if (options->db_length > 0)
            dblen = options->db_length;
        if (options->dbseq_num > 0)
            search->dbseq_num = options->dbseq_num;
        if (options->searchsp_eff > 0)
            searchsp_eff = options->searchsp_eff;

        if (StringCmp(prog_name, "tblastn") == 0 ||
            StringCmp(prog_name, "tblastx") == 0 ||
            StringCmp(prog_name, "psitblastn") == 0) {
            dblen /= 3;
            searchsp_eff /= 3.0;
        }
        search->dblen = dblen;
        search->searchsp_eff = searchsp_eff;
        status = BLASTSetUpSearchInternalByLoc (search, query_slp, query_bsp, prog_name, qlen, options, callback);
        if (status != 0) {
            ErrPostEx(SEV_WARNING, 0, 0, "SetUpBlastSearch failed.");
            search->query_invalid = TRUE;
        }
        
        if (search->pbp->mb_params) 
            search = GreedyAlignMemAlloc(search);
        else 
            search->abmp = NULL;
        
        search->rdfp = ReadDBCloseMHdrAndSeqFiles(search->rdfp);
    }
    
    if (options_alloc)
        options = BLASTOptionDelete(options);
    
    return search;
}

/*
	Performs setup for a BLAST search.  This function must be used
	with a search file accessed through readdb.
*/

BlastSearchBlkPtr LIBCALL 
BLASTSetUpSearchWithReadDb(BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, CharPtr dbname, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)))

{
	return BLASTSetUpSearchWithReadDbInternal (NULL, query_bsp, prog_name, qlen, dbname, options, callback, NULL, NULL, 0, NULL);
}

BlastSearchBlkPtr LIBCALL 
BLASTSetUpSearchWithReadDbEx(BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, CharPtr dbname, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total)

{
	return BLASTSetUpSearchWithReadDbInternal (NULL, query_bsp, prog_name, qlen, dbname, options, callback, seqid_list, gi_list, gi_list_total, NULL);
}

/*
	Performs setup for a BLAST search.  This function must be used
	with a search file accessed through readdb.
*/

BlastSearchBlkPtr LIBCALL 
BLASTSetUpSearchByLocWithReadDb(SeqLocPtr query_slp, CharPtr prog_name, Int4 qlen, CharPtr dbname, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)))

{
	return BLASTSetUpSearchWithReadDbInternal (query_slp, NULL, prog_name, qlen, dbname, options, callback, NULL, NULL, 0, NULL);
}


BlastSearchBlkPtr LIBCALL 
BLASTSetUpSearchByLocWithReadDbEx(SeqLocPtr query_slp, CharPtr prog_name, Int4 qlen, CharPtr dbname, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total)

{
	return BLASTSetUpSearchWithReadDbInternal (query_slp, NULL, prog_name, qlen, dbname, options, callback, seqid_list, gi_list, gi_list_total, NULL);
}
static BlastSearchBlkPtr
BLASTSetUpSearchEx (SeqLocPtr query_slp, BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, Int8 dblen, BlastAllWordPtr all_words, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)))

{
	BlastSearchBlkPtr search;
	Boolean options_alloc=FALSE, multiple_hits;
	Int2 status, first_context, last_context;
	Int4 actual_query_length=0;
	Nlm_FloatHi searchsp_eff=0;

	/* Allocate default options if no are allocated yet. */
	if (options == NULL)
	{
		options = BLASTOptionNew(prog_name, FALSE);
		options_alloc = TRUE;
	}

        multiple_hits = options->multiple_hits_only;
        /*
	if (options->window_size != 0)
		multiple_hits = TRUE;
	else
		multiple_hits = FALSE;
        */
	if (query_slp == NULL && query_bsp == NULL)
		return NULL;

	if (query_slp)
		actual_query_length = SeqLocLen(query_slp);
	else if (query_bsp)
		actual_query_length = query_bsp->length;

	if (qlen <= 0)
	{
		qlen = actual_query_length;
	}

	/* If dblen is not set, use qlen. */
	if (dblen <= 0)
		dblen = qlen;

	BlastGetFirstAndLastContext(prog_name, query_slp, &first_context, &last_context, options->strand_option);

/* On the first call query length is used for the subject length. */
	search = BlastSearchBlkNew(options->wordsize, actual_query_length, NULL, multiple_hits, 0, options->threshold_second, options->hitlist_size, prog_name, all_words, first_context, last_context, options->window_size);

	if (search)
	{
                search->subject->length = dblen; 
		/* Options setting overrides parameter. */
                if (options->db_length > 0)
			dblen = options->db_length;
		if (options->searchsp_eff > 0)
			searchsp_eff = options->searchsp_eff;
                if (StringCmp(prog_name, "tblastn") == 0
                    || StringCmp(prog_name, "tblastx") == 0
                    || StringCmp(prog_name, "psitblastn") == 0)
                {
                        dblen /= 3;
                        searchsp_eff /= 3.0;
                }
		if (options->dbseq_num > 0)
			search->dbseq_num = options->dbseq_num;
		else
			search->dbseq_num = (Int4) dblen/qlen;
	
		if (search->dbseq_num <=0)
			search->dbseq_num = 1;

		search->dblen = dblen;
		/* If searchsp_eff is > 0 it will be used. */
		search->searchsp_eff = searchsp_eff;
                if (options->is_megablast_search)
                   search->pbp->mb_params = MegaBlastParameterBlkNew(options);
		if (search->pbp->mb_params)
		   status = MegaBlastSetUpSearchInternalByLoc (search, query_slp, query_bsp, prog_name, qlen, options, callback);
		else
		   status = BLASTSetUpSearchInternalByLoc(search, query_slp, query_bsp, prog_name, qlen, options, callback);
		if (status != 0)
		{
	  		ErrPostEx(SEV_WARNING, 0, 0, "SetUpBlastSearch failed.");
			search->query_invalid = TRUE;
		}

		if (search->pbp->mb_params) 
	   		search = GreedyAlignMemAlloc(search);
	}

	if (options_alloc)
		options = BLASTOptionDelete(options);

	return search;
}

/*
	Performs necessary setup for a BLAST search.  The arguments are:

	 - search: BlastSearchBlkPtr created by BlastSearchBlkNew
	 - query_bsp: BioseqPtr for the query
	 - matrix: CharPtr containing the name of the matrix
	 - prog_name: CharPtr containing name of the program
	 - qlen: Int4 with length of the query, if a lenght should be
		specified (for statistical calculations); if this argument is
		zero, then query_bsp->length is used.
	 -dblen: Int8 with length of the database.
	 - e_cutoff: BLAST_Score specifying the "expect" value.
	 - number_of_processors: number of processors to use.
	 - gap_decay_rate: between zero and one, related to prob. of # of HSP's.
	 - gap_size: largest allowable gap if "small" gaps are used.
	 - gap_prob: probability of "small" gap model being correct.
	 - multiple_hits: if TRUE, multiple hits method is used.
	 - window: window size for multiple hits method
	 - threshold_second: initial hit threshold for 2nd pass
	 - discontiguous: should discontiguous words be used?
	 - old_stats: should the old statistics be used?
	 - is_prot: is this a protein?

	
*/

BlastSearchBlkPtr LIBCALL 
BLASTSetUpSearch (BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, Int8 dblen, BlastAllWordPtr all_words, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)))

{
	return BLASTSetUpSearchEx (NULL, query_bsp, prog_name, qlen, dblen, all_words, options, callback);
}

BlastSearchBlkPtr LIBCALL 
BLASTSetUpSearchByLoc (SeqLocPtr query_slp, CharPtr prog_name, Int4 qlen, Int8 dblen, BlastAllWordPtr all_words, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)))

{
	return BLASTSetUpSearchEx (query_slp, NULL, prog_name, qlen, dblen, all_words, options, callback);
}

static void OOF_TranslateToDNAP(BLAST_HitListPtr hitlist, Int4 length)
{
    BLAST_HitListPtr hlst;
    BLAST_Seg query;
    Int4 i, from, to, frame;

    for(hlst = hitlist; hlst != NULL; hlst = hlst->next) {
        
        for(i = 0; i < hlst->hspcnt; i++) {

            query = hlst->hsp_array[i]->query;            

            from = query.offset;
            to = query.end;
            frame = query.frame;
            
            query.offset = CODON_LENGTH*from + frame - 1;
            query.end = CODON_LENGTH*to + frame - 1;
            query.length = query.end - query.offset + 1;
            query.gapped_start = CODON_LENGTH*query.gapped_start + frame - 1;
        }
    }
    return;
}

static int LIBCALLBACK
diag_compare_hsps(VoidPtr v1, VoidPtr v2)
{
   BLAST_HSPPtr h1, h2;

   h1 = *((BLAST_HSPPtr PNTR) v1);
   h2 = *((BLAST_HSPPtr PNTR) v2);
   
   return (h1->query.offset - h1->subject.offset) - 
      (h2->query.offset - h2->subject.offset);
}

/* 
   Shifts all HSP coordinates according to where the partial sequence
   started in the large sequence
*/
void AdjustOffsetsInBLASTHitList(BLAST_HitListPtr hitlist, Int4 start)
{
   Int4 index;
   BLAST_HSPPtr hsp;

   for (index=0; index<hitlist->hspcnt; index++) {
      hsp = hitlist->hsp_array[index];
      hsp->subject.offset += start;
      hsp->subject.end += start;
      hsp->subject.gapped_start += start;
      if (hsp->gap_info)
         hsp->gap_info->start2 += start;
   }
}

#define DBSEQ_CHUNK_OVERLAP 100
#define OVERLAP_DIAG_CLOSE 10
/* This is a hard merge, i.e. the two HSPs will be merged only 
   if they intersect 
*/
static Boolean
BLASTMergeHsps(BlastSearchBlkPtr search, BLAST_HSPPtr hsp1, BLAST_HSPPtr hsp2,
               Int4 start)
{
   BLAST_HSPPtr new_hsp = NULL;
   BLASTHSPSegmentPtr segments1, segments2, new_segment1, new_segment2;
   GapXEditScriptPtr esp1, esp2, esp;
   Int4 end = start + DBSEQ_CHUNK_OVERLAP - 1;
   Int4 min_diag, max_diag, num1, num2, dist, next_dist;
   Int4 diag1_start, diag1_end, diag2_start, diag2_end;
   Int4 index;
   Uint1 intersection_found;
   Uint1 op_type;

   if (!hsp1->gap_info || !hsp2->gap_info) {
      /* Assume that this is an ungapped alignment, hence simply compare 
         diagonals. Do not merge if they are on different diagonals */
      if (diag_compare_hsps(&hsp1, &hsp2) == 0 &&
          hsp1->query.end >= hsp2->query.offset) {
         hsp1->query.end = hsp2->query.end;
         hsp1->subject.end = hsp2->subject.end;
         hsp1->query.length = hsp1->query.end - hsp1->query.offset;
         hsp1->subject.length = hsp1->subject.end - hsp1->subject.offset;
         return TRUE;
      } else
         return FALSE;
   }
   /* Find whether these HSPs have an intersection point */
   segments1 = (BLASTHSPSegmentPtr) MemNew(sizeof(BLASTHSPSegment));
   
   esp1 = hsp1->gap_info->esp;
   esp2 = hsp2->gap_info->esp;
   
   segments1->q_start = hsp1->query.offset;
   segments1->s_start = hsp1->subject.offset;
   while (segments1->s_start < start) {
      if (esp1->op_type == GAPALIGN_INS)
         segments1->q_start += esp1->num;
      else if (segments1->s_start + esp1->num < start) {
         if (esp1->op_type == GAPALIGN_SUB) { 
            segments1->s_start += esp1->num;
            segments1->q_start += esp1->num;
         } else if (esp1->op_type == GAPALIGN_DEL)
            segments1->s_start += esp1->num;
      } else 
         break;
      esp1 = esp1->next;
   }
   /* Current esp is the first segment within the overlap region */
   segments1->s_end = segments1->s_start + esp1->num - 1;
   if (esp1->op_type == GAPALIGN_SUB)
      segments1->q_end = segments1->q_start + esp1->num - 1;
   else
      segments1->q_end = segments1->q_start;
   
   new_segment1 = segments1;
   
   for (esp = esp1->next; esp; esp = esp->next) {
      new_segment1->next = (BLASTHSPSegmentPtr)
         MemNew(sizeof(BLASTHSPSegment));
      new_segment1->next->q_start = new_segment1->q_end + 1;
      new_segment1->next->s_start = new_segment1->s_end + 1;
      new_segment1 = new_segment1->next;
      if (esp->op_type == GAPALIGN_SUB) {
         new_segment1->q_end += esp->num - 1;
         new_segment1->s_end += esp->num - 1;
      } else if (esp->op_type == GAPALIGN_INS) {
         new_segment1->q_end += esp->num - 1;
         new_segment1->s_end = new_segment1->s_start;
      } else {
         new_segment1->s_end += esp->num - 1;
         new_segment1->q_end = new_segment1->q_start;
      }
   }
   
   /* Now create the second segments list */
   
   segments2 = (BLASTHSPSegmentPtr) MemNew(sizeof(BLASTHSPSegment));
   segments2->q_start = hsp2->query.offset;
   segments2->s_start = hsp2->subject.offset;
   segments2->q_end = segments2->q_start + esp2->num - 1;
   segments2->s_end = segments2->s_start + esp2->num - 1;
   
   new_segment2 = segments2;
   
   for (esp = esp2->next; esp && new_segment2->s_end < end; 
        esp = esp->next) {
      new_segment2->next = (BLASTHSPSegmentPtr)
         MemNew(sizeof(BLASTHSPSegment));
      new_segment2->next->q_start = new_segment2->q_end + 1;
      new_segment2->next->s_start = new_segment2->s_end + 1;
      new_segment2 = new_segment2->next;
      if (esp->op_type == GAPALIGN_INS) {
         new_segment2->s_end = new_segment2->s_start;
         new_segment2->q_end = new_segment2->q_start + esp->num - 1;
      } else if (esp->op_type == GAPALIGN_DEL) {
         new_segment2->s_end = new_segment2->s_start + esp->num - 1;
         new_segment2->q_end = new_segment2->q_start;
      } else if (esp->op_type == GAPALIGN_SUB) {
         new_segment2->s_end = new_segment2->s_start + esp->num - 1;
         new_segment2->q_end = new_segment2->q_start + esp->num - 1;
      }
   }
   
   new_segment1 = segments1;
   new_segment2 = segments2;
   intersection_found = 0;
   num1 = num2 = 0;
   while (new_segment1 && new_segment2 && !intersection_found) {
      if (new_segment1->s_end < new_segment2->s_start || 
          new_segment1->q_end < new_segment2->q_start) {
         new_segment1 = new_segment1->next;
         num1++;
         continue;
      }
      if (new_segment2->s_end < new_segment1->s_start || 
          new_segment2->q_end < new_segment1->q_start) {
         new_segment2 = new_segment2->next;
         num2++;
         continue;
      }
      diag1_start = new_segment1->s_start - new_segment1->q_start;
      diag2_start = new_segment2->s_start - new_segment2->q_start;
      diag1_end = new_segment1->s_end - new_segment1->q_end;
      diag2_end = new_segment2->s_end - new_segment2->q_end;
      
      if (diag1_start == diag1_end && diag2_start == diag2_end &&  
          diag1_start == diag2_start) {
         /* Both segments substitutions, on same diagonal */
         intersection_found = 1;
         dist = new_segment2->s_end - new_segment1->s_start + 1;
         break;
      } else if (diag1_start != diag1_end && diag2_start != diag2_end) {
         /* Both segments gaps - must intersect */
         intersection_found = 3;

         dist = new_segment2->s_end - new_segment1->s_start + 1;
         op_type = GAPALIGN_INS;
         next_dist = new_segment2->q_end - new_segment1->q_start - dist + 1;
         if (new_segment2->q_end - new_segment1->q_start < dist) {
            dist = new_segment2->q_end - new_segment1->q_start + 1;
            op_type = GAPALIGN_DEL;
            next_dist = new_segment2->s_end - new_segment1->s_start - dist + 1;
         }
         break;
      } else if (diag1_start != diag1_end) {
         max_diag = MAX(diag1_start, diag1_end);
         min_diag = MIN(diag1_start, diag1_end);
         if (diag2_start >= min_diag && diag2_start <= max_diag) {
            intersection_found = 2;
            dist = diag2_start - min_diag + 1;
            if (new_segment1->s_end == new_segment1->s_start)
               next_dist = new_segment2->s_end - new_segment1->s_end + 1;
            else
               next_dist = new_segment2->q_end - new_segment1->q_end + 1;
            break;
         }
      } else if (diag2_start != diag2_end) {
         max_diag = MAX(diag2_start, diag2_end);
         min_diag = MIN(diag2_start, diag2_end);
         if (diag1_start >= min_diag && diag1_start <= max_diag) {
            intersection_found = 2;
            next_dist = max_diag - diag1_start + 1;
            if (new_segment2->s_end == new_segment2->s_start)
               dist = new_segment2->s_start - new_segment1->s_start + 1;
            else
               dist = new_segment2->q_start - new_segment1->q_start + 1;
            break;
         }
      }
      if (new_segment1->s_end <= new_segment2->s_end) {
         new_segment1 = new_segment1->next;
         num1++;
      }
      if (new_segment1->s_end >= new_segment2->s_end) {
         new_segment2 = new_segment2->next;
         num2++;
      }
   }

   if (intersection_found) {
      esp = NULL;
      for (index = 0; index < num1-1; index++)
         esp1 = esp1->next;
      for (index = 0; index < num2-1; index++) {
         esp = esp2;
         esp2 = esp2->next;
      }
      if (intersection_found < 3) {
         if (num1 > 0)
            esp1 = esp1->next;
         if (num2 > 0) {
            esp = esp2;
            esp2 = esp2->next;
         }
      }
      switch (intersection_found) {
      case 1:
         esp1->num = dist;
         esp1->next = esp2->next;
         esp2->next = NULL;
         break;
      case 2:
         esp1->num = dist;
         esp2->num = next_dist;
         esp1->next = esp2;
         if (esp)
            esp->next = NULL;
         break;
      case 3:
         esp1->num += dist;
         esp2->op_type = op_type;
         esp2->num = next_dist;
         esp1->next = esp2;
         if (esp)
            esp->next = NULL;
         break;
      default: break;
      }
      hsp1->query.end = hsp2->query.end;
      hsp1->subject.end = hsp2->subject.end;
      hsp1->query.length = hsp1->query.end - hsp1->query.offset;
      hsp1->subject.length = hsp1->subject.end - hsp1->subject.offset;
   }

   return (Boolean) intersection_found;
}

static Boolean BLASTHspContained(BLAST_HSPPtr hsp1, BLAST_HSPPtr hsp2)
{
   Boolean hsp_start_is_contained=FALSE, hsp_end_is_contained=FALSE;

   if (hsp1->score > hsp2->score || 
       SIGN(hsp2->query.frame) != SIGN(hsp1->query.frame) || 
       SIGN(hsp2->subject.frame) != SIGN(hsp1->subject.frame))
      return FALSE;

   if (CONTAINED_IN_HSP(hsp2->query.offset, hsp2->query.end, hsp1->query.offset, hsp2->subject.offset, hsp2->subject.end, hsp1->subject.offset) == TRUE) {
      hsp_start_is_contained = TRUE;
   }
   if (CONTAINED_IN_HSP(hsp2->query.offset, hsp2->query.end, hsp1->query.end, hsp2->subject.offset, hsp2->subject.end, hsp1->subject.end) == TRUE) {
      hsp_end_is_contained = TRUE;
   }
   
   return (hsp_start_is_contained && hsp_end_is_contained);
}

/*
  Merges the hits from different chunks of the subject sequence that
  have been searched separately
*/
static BLAST_HitListPtr
BLASTMergeHitLists(BlastSearchBlkPtr search, BLAST_HitListPtr hitlist1, 
                   BLAST_HitListPtr hitlist2, Int4 start, Boolean merge_hsps)
{
   BLAST_HSPPtr hsp, PNTR hspp1, PNTR hspp2;
   Int4 index, index1, hspcnt1, hspcnt2, new_hspcnt = 0;
   BLAST_HSPPtr PNTR new_hsp_array;
   Int4Ptr index_array;

   if (hitlist1 == NULL) {
      hitlist1 = (BLAST_HitListPtr)
         MemDup(hitlist2, sizeof(BLAST_HitList));
      hitlist1->hsp_array = (BLAST_HSPPtr PNTR) 
         MemNew(hitlist2->hspmax*sizeof(BLAST_HSPPtr));
      MemCpy(hitlist1->hsp_array, hitlist2->hsp_array, 
             hitlist2->hspcnt*sizeof(BLAST_HSPPtr));
      return hitlist1;
   } else {
      /* In case these have changed */
      hitlist1->exact_match_array = hitlist2->exact_match_array;
      hitlist1->exact_match_max = hitlist2->exact_match_max;
   }

   hspcnt1 = hspcnt2 = 0;
   hspp1 = (BLAST_HSPPtr PNTR) MemNew(hitlist1->hspcnt*sizeof(BLAST_HSPPtr));
   hspp2 = (BLAST_HSPPtr PNTR) MemNew(hitlist2->hspcnt*sizeof(BLAST_HSPPtr));
   index_array = (Int4Ptr) MemNew(hitlist1->hspcnt*sizeof(Int4));

   for (index=0; index<hitlist1->hspcnt; index++) {
      hsp = hitlist1->hsp_array[index];
      if (hsp->subject.end > start) {
         index_array[hspcnt1] = index;
         hspp1[hspcnt1++] = hsp;
         hitlist1->hsp_array[index] = NULL;
      } else
         new_hspcnt++;
   }
   for (index=0; index<hitlist2->hspcnt; index++) {
      hsp = hitlist2->hsp_array[index];
      if (hsp->subject.offset < start + DBSEQ_CHUNK_OVERLAP) {
         hspp2[hspcnt2++] = hsp;
         hitlist2->hsp_array[index] = NULL;
      } else
         new_hspcnt++;
   }

   HeapSort(hspp1, hspcnt1, sizeof(BLAST_HSPPtr), diag_compare_hsps);
   HeapSort(hspp2, hspcnt2, sizeof(BLAST_HSPPtr), diag_compare_hsps);

   for (index=0; index<hspcnt1; index++) {
      for (index1=0; index1<hspcnt2; index1++) {
         if (hspp2[index1] && 
             hspp2[index1]->query.frame == hspp1[index]->query.frame &&
             hspp2[index1]->subject.frame == hspp1[index]->subject.frame &&
             ABS(diag_compare_hsps(&hspp1[index], &hspp2[index1])) < 
             OVERLAP_DIAG_CLOSE) {
            if (merge_hsps) {
               if (BLASTMergeHsps(search, hspp1[index], hspp2[index1], start)) {
                  hspp2[index1]->gap_info = 
                     GapXEditBlockDelete(hspp2[index1]->gap_info);
                  hspp2[index1] = MemFree(hspp2[index1]);
                  break;
               }
            } else { /* No gap information available */
               if (BLASTHspContained(hspp1[index], hspp2[index1])) {
                  hspp1[index] = MemFree(hspp1[index]);
                  hspp1[index] = hspp2[index1];
                  hspp2[index1] = NULL;
               } else if (BLASTHspContained(hspp2[index1], hspp1[index]))
                  hspp2[index1] = MemFree(hspp2[index1]);

            }
         }
      }
   }

   new_hspcnt += hspcnt1;
   for (index=0; index<hspcnt2; index++) {
      if (hspp2[index] != NULL)
         new_hspcnt++;
   }
   
   if (new_hspcnt >= hitlist1->hspmax-1 && hitlist1->do_not_reallocate == FALSE) {
      new_hsp_array = (BLAST_HSPPtr PNTR) Realloc(hitlist1->hsp_array, new_hspcnt*2*sizeof(BLAST_HSPPtr));
      if (new_hsp_array == NULL) {
         ErrPostEx(SEV_WARNING, 0, 0, "UNABLE to reallocate in BlastSaveCurrentHsp for ordinal id %ld, continuing with fixed array of %ld HSP's", (long) search->subject_id, (long) hitlist1->hspmax);
         hitlist1->do_not_reallocate = TRUE; 
      } else {
         hitlist1->hsp_array = new_hsp_array;
         hitlist1->hspmax = 2*new_hspcnt;
      }
   }

   for (index=0; index<hspcnt1; index++) 
      hitlist1->hsp_array[index_array[index]] = hspp1[index];
   for (index=hitlist1->hspcnt, index1=0; 
        index1<hitlist2->hspcnt && index<hitlist1->hspmax; index1++) {
      if (hitlist2->hsp_array[index1] != NULL)
         hitlist1->hsp_array[index++] = hitlist2->hsp_array[index1];
   }
   for (index1=0; index1<hspcnt2 && index<hitlist1->hspmax; index1++) {
      if (hspp2[index1] != NULL)
         hitlist1->hsp_array[index++] = hspp2[index1];
   }

   hspp1 = MemFree(hspp1);
   hspp2 = MemFree(hspp2);
   index_array = MemFree(index_array);
   
   hitlist1->hspcnt = index;
   return hitlist1;
}

/* Remove HSPs that do not touch the overlap region and have initial evalue 
   estimate more than 10 times higher than the cutoff.
*/
static BlastSearchBlkPtr 
BlastReapPartialHitlistByEvalue(BlastSearchBlkPtr search, Int4 start)
{
   BLAST_HSPPtr hsp;
   Int4 index, hspcnt;
   FloatHi searchsp_eff;
   BLAST_KarlinBlkPtr PNTR kbp;
   Int4 context;

   if (search->pbp->gapped_calculation && 
       search->prog_number != blast_type_blastn)
      kbp = search->sbp->kbp_gap;
   else
      kbp = search->sbp->kbp;

   hspcnt = search->current_hitlist->hspcnt;
   for (index=0; index<hspcnt; index++) {
      hsp = search->current_hitlist->hsp_array[index];
      
      if (hsp->subject.offset > start + DBSEQ_CHUNK_OVERLAP) {
	 if (search->pbp->mb_params)
            context = BinarySearchInt4(hsp->query.offset,
                                       search->query_context_offsets, 
                                       (Int4) (search->last_context+1));
         else
            context = (Int4) hsp->context;
         searchsp_eff = (FloatHi) search->dblen_eff *
            (FloatHi) search->context[context].query->effective_length;
         
         hsp->evalue = BlastKarlinStoE_simple(hsp->score, kbp[context], 
                                              searchsp_eff);
         if (hsp->evalue > 10*search->pbp->cutoff_e) {
            hsp->gap_info = GapXEditBlockDelete(hsp->gap_info);
            hsp = MemFree(hsp);
            search->current_hitlist->hsp_array[index] = NULL;
         }
      }
   }
   search->current_hitlist->hspcnt = 
      HspArrayPurge(search->current_hitlist->hsp_array, hspcnt, FALSE);
   return search;
}

/*
	Performs a BLAST search using a sequence from obtained from readdb.
*/
BlastSearchBlkPtr LIBCALL
BLASTPerformSearchWithReadDb (BlastSearchBlkPtr search, Int4 sequence_number)

{
	Int4 subject_length;
	Uint1Ptr subject_seq=NULL;

	subject_length = readdb_get_sequence(search->rdfp, sequence_number, &subject_seq);

	search->dblen_eff_real += MAX(subject_length-search->length_adjustment, 1);
	search->subject_id = sequence_number;

	search->dblen_eff_real += MAX(subject_length-search->length_adjustment, 1);
        search->subject_id = sequence_number;

        search = BLASTPerformSearch(search, subject_length, subject_seq); 

	return search;
}

/*
	Performs a BLAST search with a subject sequence that is passed in.
	Used when an entire database is being scanned (by 
	BLASTPerformSearchWithReadDb) and when only two seqs are being
	compared.
*/
BlastSearchBlkPtr LIBCALL
BLASTPerformSearch (BlastSearchBlkPtr search, Int4 subject_length, Uint1Ptr subject_seq)

{
	Int2 status;

	if (search->pbp->two_pass_method)
	{
		status = BLASTPerform2PassSearch(search, subject_length, subject_seq);
	}
	else
	{
		status = BLASTPerformFinalSearch(search, subject_length, subject_seq);
	}
	
	return search;
}

/*

	Performs a BLAST search using the two-pass method: the first pass
	looks for multiple initial hits and then performs a second pass
	(with single hits extended) wiht a lower T value.

	 Arguments are:

	 - search: BlastSearchBlkPtr returned by SetUpBlastSearch, call
		SetUpBlastSearch before calling this function.
	 - sequence_number: number assigned to sequence (by user).  The
		"readdb" library uses this number to access the sequence.
		This number should be zero if it's not important.
	 - subject_length: the length of the database sequence (not the length
		allocated in *subject_seq).
	 - subject_seq: CharPtr pointing to the sequence.

	NOTE: static variables in PerformBlastSearch for subject_seq and 
	allocated_length are not an option as they can't be deallocated 
	after the last call and they are NOT MP-safe.
*/

Int2 LIBCALL 
BLASTPerform2PassSearch (BlastSearchBlkPtr search, Int4 subject_length, Uint1Ptr subject_seq)

{
	Int2 outer_frame, outer_frame_max, status, outer_frame_min;
	Int4 prot_length;
	Uint1Ptr prot_seq;

	search->current_hitlist_purge = TRUE; /* The default. */
	outer_frame_max = 1;

        if (StringCmp(search->prog_name, "tblastn") == 0
            || StringCmp(search->prog_name, "tblastx") == 0
            || StringCmp(search->prog_name, "psitblastn") == 0)
	{
		outer_frame_min = -3;
		outer_frame_max = 3;
	}
	else
	{
		outer_frame_min = 0;
		outer_frame_max = 0;
	}

	for (outer_frame=outer_frame_min; outer_frame<=outer_frame_max; outer_frame++)
	{
		search->subject->frame = outer_frame;
               if (StringCmp("tblastn", search->prog_name) == 0
                    || StringCmp("tblastx", search->prog_name) == 0
                    || StringCmp("psitblastn", search->prog_name) == 0)
		{
			if (outer_frame == 0)
				continue;
			prot_seq = search->translation_buffer;
			prot_length = BlastTranslateUnambiguousSequence(search, subject_length, prot_seq, subject_seq, outer_frame);

                        if(search->pbp->is_rps_blast) {
                            /* SEG Filtering of query DNA sequence */
                            
                            rpsBlastFilterSequence(search, outer_frame,
                                                   prot_seq, prot_length,
                                                   subject_length);
                        }
                        
			BlastSequenceAddSequence(search->subject, NULL, prot_seq, prot_length, subject_length, 0);
		}
		else
		{
			BlastSequenceAddSequence(search->subject, NULL, subject_seq-1, subject_length, subject_length, 0);
		}

		search->prelim = TRUE;
		search->wfp = search->wfp_first;

/* First pass with multiple hits. */
		status = BlastExtendWordSearch(search, TRUE);
	/* status = 0 means NO significant matches found on first pass.*/
		if (status > 0)
		{	/* Match found on initial pass, DO second pass. */
			status = BLASTPerformFinalSearch(search, subject_length, subject_seq);
			break;
		}
		else
		{ /* NULL out the sequence to prevent unintentional FREE's
			(it's in "*subject_seq"), but delete the descriptor. */
			search->subject->sequence = NULL; 
		}

		if (status < 0)
		{		/* Error */
			ErrPostEx(SEV_FATAL, 0, 0, "BlastExtendWordSearch returned non-zero status");
			return 1;
		}
	}

/* NULL out the sequence, leave in the proper length which is still needed
for the significance evaluation. */
	search->subject->length = subject_length;
	search->subject->sequence = NULL;
	search->subject->sequence_start = NULL;

	return 0;
}

/*

	Performs a BLAST search using the two-pass method: the first pass
	looks for multiple initial hits and then performs a second pass
	(with single hits extended) wiht a lower T value.

	 Arguments are:

	 - search: BlastSearchBlkPtr returned by SetUpBlastSearch, call
		SetUpBlastSearch before calling this function.
	 - sequence_number: number assigned to sequence (by user).  The
		"readdb" library uses this number to access the sequence.
		This number should be zero if it's not important.
	 - subject_length: the length of the database sequence (not the length
		allocated in *subject_seq).
	 - subject_seq: CharPtr pointing to the sequence.

	NOTE: static variables in PerformBlastSearch for subject_seq and 
	allocated_length are not an option as they can't be deallocated 
	after the last call and they are NOT MP-safe.
*/

Int2 LIBCALL 
BLASTPerformFinalSearch (BlastSearchBlkPtr search, Int4 subject_length, Uint1Ptr subject_seq)

{
    BLAST_HitListPtr current_hitlist, hitlist = NULL;
    Int2 inner_frame, inner_frame_max, status, inner_frame_min;
    Int4 real_length, length, start = 0, num_chunks, index;
    Uint1Ptr prot_seq;
    
    BlastHitListPurge(search->current_hitlist);
    if (subject_length == 0)
	return 1;

    BlastSequenceAddSequence(search->subject, NULL, subject_seq-1, subject_length, subject_length, 0);
    search->current_hitlist_purge = TRUE; /* The default. */
    inner_frame_max = 1;
    if (search->prog_number == blast_type_tblastn
        || search->prog_number == blast_type_tblastx
        || search->prog_number == blast_type_psitblastn) {
        inner_frame_min = -3;
        inner_frame_max = 3;
    } else if (search->prog_number == blast_type_blastn) {
        inner_frame_min = 1;
        inner_frame_max = 1;
    } else {
        inner_frame_min = 0;
        inner_frame_max = 0;
    }

    /* Match found on initial pass, DO second pass. */
    for (inner_frame=inner_frame_min; inner_frame<=inner_frame_max; inner_frame++) {
        search->subject->frame = inner_frame;
        if (search->prog_number == blast_type_tblastn
            || search->prog_number == blast_type_tblastx
            || search->prog_number == blast_type_psitblastn) {
            if (inner_frame == inner_frame_min) /* Purge on 1st call. */
                search->current_hitlist_purge = TRUE; 
            else
                search->current_hitlist_purge = FALSE; 
            if (inner_frame == 0)
                continue;
	    start = 0;
            prot_seq = search->translation_buffer;
            real_length = BlastTranslateUnambiguousSequence(search, subject_length, prot_seq, subject_seq, inner_frame);
            
            if(search->pbp->is_rps_blast) {
                /* SEG Filtering of query DNA sequence */
                
                rpsBlastFilterSequence(search, inner_frame,
                                       prot_seq, real_length,
                                       subject_length);
            }
            
            /* subject seq stays the same, except for tblast[nx]. */
            BlastSequenceAddSequence(search->subject, NULL, prot_seq, real_length, subject_length, 0);
            if (real_length == 0)
                continue;
        } else 
           real_length = subject_length;
        
        search->prelim = FALSE;
        /* Calculate some cutoff scores, these depend upon the seq lengths.*/
        /* For blastn  and gapped calc. use the cutoff's originally found. */
        if (!search->pbp->gapped_calculation && 
            search->prog_number != blast_type_blastn) {
            CalculateSecondCutoffScore(search, search->subject->length, &search->pbp->ignore_small_gaps, &search->pbp->cutoff_s_second, &search->pbp->cutoff_big_gap);
        }
        
#ifdef BLAST_COLLECT_STATS
        search->second_pass_trys++;
#endif

        if (search->pbp->mb_params)
           /* sequence_start is reserved for ncbi4na encoded sequence
              in this case */
           search->subject->sequence_start = NULL;

        length = real_length;
        /* Split subject sequence into chunks if it is too long */
        num_chunks = (length - DBSEQ_CHUNK_OVERLAP) / 
           (MAX_DBSEQ_LEN - DBSEQ_CHUNK_OVERLAP) + 1;
        search->subject->original_length = 0;
        if (search->pbp->mb_params && !search->rdfp) { 
           /* Coming from the 2 sequences engine: save the entire 
              ncbi4na sequence in search->subject->sequence_start
           */
           Uint1Ptr seq_blastna, seq_2na;
           Uint1 rem;
           
           search->subject->sequence_start = 
              (Uint1Ptr) MemNew(subject_length + 1);
           seq_blastna = search->subject->sequence_start;
           seq_2na = search->subject->sequence;
           rem = 3;
           *seq_blastna = (Uint1) ncbi4na_to_blastna[NULLB];
           seq_blastna++;
           for (index=0; index<subject_length; index++) {
              *seq_blastna = 
                 (Uint1) ncbi4na_to_blastna[(1 << READDB_UNPACK_BASE_N(*seq_2na, rem))];
              seq_blastna++;
              if (rem>0) rem--;
              else {
                 rem = 3;
                    seq_2na++;
              }
           }
        }

        for (index=0; index<num_chunks; index++) {
           length = MIN(real_length-start, MAX_DBSEQ_LEN);
           search->subject->length = length;
           /* THE BLAST SEARCH _IS_ HERE! */
           status = BlastExtendWordSearch(search, search->pbp->multiple_hits_only);
           
           /* HSP's were not saved in any special order, sort. */
           current_hitlist = search->current_hitlist;
           if (current_hitlist && current_hitlist->do_not_reallocate == FALSE)
              HeapSort(current_hitlist->hsp_array, current_hitlist->hspcnt,sizeof(BLAST_HSPPtr), score_compare_hsps);
           if (search->pbp->gapped_calculation &&
               search->prog_number != blast_type_blastn) {
              status = BlastPreliminaryGappedScore(search, search->subject->sequence, search->subject->length, inner_frame);
              status = BlastGetGappedScore(search, search->subject->length, search->subject->sequence, inner_frame);
           }
#if 1
           else if (!search->pbp->do_sum_stats && !search->pbp->mb_params) {
              status = BlastNTPreliminaryGappedScore(search, search->subject->sequence, search->subject->length);
              status = BlastNTGetGappedScore(search, search->subject->length, search->subject->sequence);
           }  
#endif
           if (num_chunks > 1) {
              AdjustOffsetsInBLASTHitList(search->current_hitlist, start);
              
              if (search->current_hitlist->hspcnt > 0) {
                 search = BlastReapPartialHitlistByEvalue(search, start);
                 hitlist = BLASTMergeHitLists(search, hitlist,
                                              search->current_hitlist, start,
                                              (search->pbp->mb_params != NULL)); 
              }
              start += length - DBSEQ_CHUNK_OVERLAP;
              search->subject->original_length = start;
              search->subject->sequence += 
                 (length - DBSEQ_CHUNK_OVERLAP)/READDB_COMPRESSION_RATIO;
              search->current_hitlist->hspcnt = 
                 search->current_hitlist->hspcnt_max = 0;
           }
        }
    } /* for (inner_frame=inner_frame_min; inner_frame */

    if (hitlist) {
       MemFree(search->current_hitlist->hsp_array);
       MemCpy(search->current_hitlist, hitlist, sizeof(BLAST_HitList));
       if (!search->pbp->mb_params)
          search->subject->sequence = search->subject->sequence_start + 1;
    }
        
    /* NULL out the sequence, leave in the proper length which is still 
       needed for the significance evaluation. */
    search->subject->length = subject_length;
    search->subject->sequence = NULL;
    if (!search->pbp->mb_params)
       /* This holds the ncbi4na-encoded sequence for Mega BLAST */
       search->subject->sequence_start = NULL;

    return 0;
}



/*
	Gets the translation array for a give genetic code.  
	This array is optimized for the NCBI2na alphabet.
	The reverse complement can also be spcified.

	Int4 id: The number of the NCBI genetic code,
	CharPtr name: The name of the NCBI genetic code,
		(only one of id or name must be specified).
	Boolean reverse_complement: translations for reverse
		complement are needed.
*/

Uint1Ptr
GetPrivatTranslationTable(CharPtr genetic_code, Boolean reverse_complement)

{
	Int2 index1, index2, index3, bp1, bp2, bp3;
	Int2 codon;
  	SeqMapTablePtr smtp;
	Uint1Ptr translation;
/* The next array translate between the ncbi2na rep's and 
the rep's used by the genetic_code tables.  The rep used by the 
genetic code arrays is in mapping: T=0, C=1, A=2, G=3 */
  	static Uint1 mapping[4] = {2, /* A in ncbi2na */
       	               1, /* C in ncbi2na. */
       	               3, /* G in ncbi2na. */
       	               0 /* T in ncbi2na. */ };


	if (genetic_code == NULL)
		return NULL;

	translation = MemNew(64*sizeof(Uint1));
	if (translation == NULL)
		return NULL;

	smtp = SeqMapTableFind(Seq_code_ncbistdaa, Seq_code_ncbieaa);

	for (index1=0; index1<4; index1++)
	{
		for (index2=0; index2<4; index2++)
		{
			for (index3=0; index3<4; index3++)
			{
/* 
The reverse complement codon is saved in it's orginal (non-complement)
form AND with the high-order bits reversed from the non-complement form,
as this is how they appear in the sequence. 
*/
			   if (reverse_complement)
			   {
				bp1 = 3 - index1;
				bp2 = 3 - index2;
				bp3 = 3 - index3;
			   	codon = (mapping[bp1]<<4) + (mapping[bp2]<<2) + (mapping[bp3]);
			   	translation[(index3<<4) + (index2<<2) + index1] = SeqMapTableConvert(smtp, genetic_code[codon]);
			   }
			   else
			   {
			   	codon = (mapping[index1]<<4) + (mapping[index2]<<2) + (mapping[index3]);
			   	translation[(index1<<4) + (index2<<2) + index3] = SeqMapTableConvert(smtp, genetic_code[codon]);
			   }
				
			}
		}
	}
	return translation;
}	/* GetPrivatTranslationTable */

/* Attach the "sequence" pointer to the BlastSequenceBlkPtr. sequence_start may be the
actual start of the sequence (this pointer is kept for deallocation purposes).  The
sequence may start before "sequence" starts as there may be a sentinel (i.e., NULLB)
before the start of the sequence.  When the extension function extends this way it
can tell that there is a NULLB there and stop the extension.

*/

Int2 LIBCALL
BlastSequenceAddSequence (BlastSequenceBlkPtr sequence_blk, Uint1Ptr sequence, Uint1Ptr sequence_start, Int4 length, Int4 original_length, Int4 effective_length)

{
	if (sequence_blk == NULL)
		return 1;

	if (sequence == NULL && sequence_start != NULL)
	{
		sequence_blk->sequence = sequence_start+1;
	}
	else if (sequence != NULL)
	{
		sequence_blk->sequence = sequence;
	}
	sequence_blk->sequence_start = sequence_start;
	sequence_blk->length = length;
	sequence_blk->original_length = original_length;
	sequence_blk->effective_length = effective_length;

	return 0;
}

/*
	Select the appropriate wordfinder and then perform the search.
	The "wordfinder's" called here look through the already found
	words and extend those above a set limit ("T").

	These wordfinders operate in two modes.  One is the "preliminary"
	mode (search->prelim is TRUE); the wordfinders attempt to extend
	an initial hit.  If they succeed at all, they return a positive
	return status.  On the second pass (search->prelim is FALSE)
	only those db seqs with hits are further investigated.

*/
static Int4
BlastExtendWordSearch(BlastSearchBlkPtr search, Boolean multiple_hits)
{
	Int2 status=0;


	/* multiple hits structure needed to perform mh extensions. */
	if (multiple_hits == TRUE && search->ewp_params->multiple_hits == FALSE)
		return -1;

	if (multiple_hits == TRUE)
		status = BlastWordFinder_mh(search);
	else
		status = BlastWordFinder(search);

	return status;
}

/*----------   search a sequence with 1 Context, 1 Letter per byte  ---------*/
static Int4 
BlastWordFinder(BlastSearchBlkPtr search)
{
	BLAST_WordFinderPtr	wfp;
	LookupTablePtr		lookup;
	BLAST_ParameterBlkPtr	pbp;


	pbp = search->pbp;
	if (search->prelim == TRUE)
	{
		wfp=search->wfp_first;
		if (pbp->cutoff_s2_set == TRUE)
			pbp->cutoff_s2 = pbp->cutoff_s2_max;
		else
			pbp->cutoff_s2 = MIN(pbp->cutoff_s_first, pbp->cutoff_s2_max);
		pbp->X = pbp->dropoff_1st_pass;
	}
	else
	{
		wfp=search->wfp_second;
		if (!search->pbp->mb_params) {
		   if (pbp->cutoff_s2_set == TRUE)
		      pbp->cutoff_s2 = pbp->cutoff_s2_max;
		   else
		      pbp->cutoff_s2 = MIN(pbp->cutoff_s_second,
					   pbp->cutoff_s2_max);
		}
		pbp->X = pbp->dropoff_2nd_pass;
	}

	lookup = wfp->lookup;

	if (search->prog_number == blast_type_blastn)
	{
	   if (search->pbp->mb_params)
	      return MegaBlastWordFinder(search, lookup);
	   else
	      return BlastNtWordFinder(search, lookup);
	}
	else
	{
		return BlastWordFinder_contig(search, lookup);
	}
}

/* This function was updated to use mod_lt instead of the original lookup table, 
 * but was not heavily optimized or tested.
 * (Modifications listed in comments before BlastWordFinder_mh_contig.)
 * -cfj
 */

/*
	Search a sequence with contiguous words.
*/
static Int4
BlastWordFinder_contig(BlastSearchBlkPtr search, LookupTablePtr lookup)
{
	register Uint1Ptr	s, s_end;
	register Int4	char_size, lookup_index, mask;
	register BLAST_Diag	diag, diag_tmp, real_diag;
	BLAST_ExtendWordPtr     ewp;
	BLAST_ExtendWordParamsPtr	ewp_params;
	Boolean			prelim, succeed_to_right;
	Uint1Ptr			subject0;
	CfjModStruct *combo_array;
	Int4	 index=0;
	register ModLookupPosition hit_info;
	Int2		context;
	Int4		q_off, s_off, offset, word_width;
	register Int4 bits_to_shift, min_diag_length, min_diag_mask;
	Int8	number_of_hits=0;
	register Int4 num_hits;
	register ModLookupPositionPtr lookup_pos;
	ModLAEntry *mod_lt=lookup->mod_lt;
	ewp_params=search->ewp_params;
	prelim = search->prelim;

/* this function only does final run, prelim is done by BlastWordFinder_mh_contig */
	if (prelim)
		return 1;

	char_size = lookup->char_size;
	mask = lookup->mask;
	offset = ewp_params->offset;
	subject0 = s = search->subject->sequence;
	min_diag_length = ewp_params->min_diag_length;
	bits_to_shift = ewp_params->bits_to_shift;
	min_diag_mask = ewp_params->min_diag_mask;

/* The word_width tells how "long" a word is; if it's contiguous then it's
the size of the word. */
	word_width = lookup->wordsize;


	if (search->current_hitlist == NULL)
	{
		search->current_hitlist = BlastHitListNew(search); 
	}
	else
	{ /* Scrub the hitlist. */
		if (search->current_hitlist_purge)
			BlastHitListPurge(search->current_hitlist);
	}

	/* subject is too short to find anything! */
	if (word_width > search->subject->length)
		return 0;

	s = lookup_find_init(lookup, &index, s);
	lookup_index = index;

        /* Determines when to stop scanning the database. */
        s_end = subject0 + search->subject->length;
	if ((search->last_context-search->first_context+1) > 1)
	{
	    for (;;) 
	    {
		do {
			/* lookup a contiguous word. */
			s++;
        		lookup_index = (((lookup_index) & mask)<<char_size) + *s;
			if (s == s_end)
				goto NormalReturn;
		} while (mod_lt[lookup_index].num_used == 0);

		num_hits = mod_lt[lookup_index].num_used;
		lookup_pos = mod_lt[lookup_index].entries;
		hit_info = *((Uint4 *) lookup_pos);
		lookup_pos++;

                if(num_hits > 3){
                    if(search->pbp->is_rps_blast) {
                        lookup_pos = (ModLookupPositionPtr) ((Uint1Ptr) lookup->mod_lookup_table_memory + (Uint4) *lookup_pos);
                    } else {
                        lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
                    }
                }

 		s_off = s-subject0;
		diag_tmp = s_off + min_diag_length;
		/* Extend each hit in the linked list */
		do {
#ifdef BLAST_COLLECT_STATS
		    number_of_hits++;
#endif
		    q_off = hinfo_get_pos(hit_info);
		    context = hinfo_get_context(hit_info);
		    num_hits--;
		    hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
		    lookup_pos++;

		    diag = diag_tmp - q_off;

		    real_diag = diag & min_diag_mask;
		    ewp=search->context[context].ewp;
		    combo_array = ewp->combo_array;

		    if (combo_array[real_diag].diag_level > (s_off+offset))
		    {
			continue;
		    }
		    if (!(search->positionBased)) {
		      if (BlastWordExtend(search, q_off, s_off, word_width, diag, real_diag, &succeed_to_right, context) != 0)
			goto ErrorReturn;
		    }
		    else {
		      if (BlastNewWordExtend(search, q_off, s_off, word_width, diag, real_diag, &succeed_to_right, context) != 0)
			goto ErrorReturn;
                    }
		} while (num_hits>0);
	   }
	}
	else	/* only one context. */
	{
	   ewp=search->context[search->first_context].ewp;
	   combo_array = ewp->combo_array;
	   for (;;) 
	   {
		do {
			/* lookup a contiguous word. */
        		lookup_index = (((lookup_index) & mask)<<char_size);
			s++;
        		lookup_index += *s;
			if (s == s_end)
				goto NormalReturn;
		} while (mod_lt[lookup_index].num_used == 0);


		num_hits = mod_lt[lookup_index].num_used;
		lookup_pos = mod_lt[lookup_index].entries;
		hit_info = *((Uint4 *) lookup_pos);
		lookup_pos++;

                if(num_hits > 3){
                    if(search->pbp->is_rps_blast) {
                	lookup_pos = (ModLookupPositionPtr) ((Uint1Ptr) lookup->mod_lookup_table_memory + (Uint4) *lookup_pos);
                    } else {
                	lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
                    }
		}

		s_off = s-subject0;
		diag_tmp = s_off + min_diag_length;
		/* Extend each hit in the linked list */
		do {
#ifdef BLAST_COLLECT_STATS
		    number_of_hits++;
#endif
		    q_off = hinfo_get_pos(hit_info);
		    num_hits--;
		    hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
		    lookup_pos++;

		    diag = diag_tmp - q_off;
		    real_diag = diag & min_diag_mask;
		    if (combo_array[real_diag].diag_level > (s_off+offset))
		    {
			continue;
		    }
		    if (!(search->positionBased)) {
		      if (BlastWordExtend(search, q_off, s_off, word_width, diag, real_diag, &succeed_to_right, 0) != 0)
			goto ErrorReturn;
		    }
		    else {
		      if (BlastNewWordExtend(search, q_off, s_off, word_width, diag, real_diag, &succeed_to_right, 0) != 0)
			goto ErrorReturn;
		    }
		} while (num_hits>0);
	   }
	}

NormalReturn:
	if (search->prelim)
		search->first_pass_hits += number_of_hits;
	else
		search->second_pass_hits += number_of_hits;
	BlastExtendWordExit(search);
	return search->current_hitlist->hspcnt;

ErrorReturn:
	BlastExtendWordExit(search);
	return 3;
}

/***************************************************************************
*	This function is called once for each subject sequence.
*
*	New (experimental) version of the Word Finder that makes use of
*	an idea of Stephen Altschul's.  Multiple hits are found before a
*	hit is extended.

*	"diagpos" is an Int4 array that is as long as the query sequence
*	and the longest database sequence.   An efficient comparison of
*	whether a new hit is in the same window as the last one is done
*	by keeping track of how far along an "imaginary" array (i.e.,
*	increment) one is; this array changes every time this function is 
*	called by the subject length plus window.
*
***************************************************************************/
/*----------   search a sequence with 1 Context, 1 Letter per byte  ---------*/
static Int4
BlastWordFinder_mh(BlastSearchBlkPtr search)
{
	BLAST_WordFinderPtr	wfp;
	LookupTablePtr lookup;
	BLAST_ParameterBlkPtr	pbp;

	pbp = search->pbp;
	if (search->prelim == TRUE)
	{
		wfp=search->wfp_first;
		if (pbp->cutoff_s2_set == TRUE)
			pbp->cutoff_s2 = pbp->cutoff_s2_max;
		else
			pbp->cutoff_s2 = MIN(pbp->cutoff_s_first, pbp->cutoff_s2_max);
		pbp->X = pbp->dropoff_1st_pass;
	}
	else
	{
		wfp=search->wfp_second;
		if (pbp->cutoff_s2_set == TRUE)
			pbp->cutoff_s2 = pbp->cutoff_s2_max;
		else
			pbp->cutoff_s2 = MIN(pbp->cutoff_s_second, pbp->cutoff_s2_max);
		pbp->X = search->pbp->dropoff_2nd_pass;
	}

	lookup = wfp->lookup;

	if (search->prog_number == blast_type_blastn)
	{
		return BlastNtWordFinder_mh(search, lookup);
	}
	else
	{
		return BlastWordFinder_mh_contig(search, lookup);
	}
}

/****************************************************************************

	This function scans the database, looking for matches to the words in
	the 'lookup_index'.  

	In order to keep track of how far along a certain diagonal has already
	been extended an Int4 array that is twice as long as the shortest sequence
	is used (actually it is the power of two that is more than twice as long as the 
	shortest sequence).  There is a need for a mapping from 'true' diagonals (which would
	be the length of both query and database sequence) to the pseudo-diagonals
	used here (i.e., the Int4 array).  This is done below with the 'version'.
	The procedure is as follows:

	1.) diag_tmp is calculated with the 'subject' offset + min_diag_length: s_off + min_diag_length
	(min_diag_length is 2**n such that n is large enough to make min_diag_length larger
	than the shorter of the query and database sequence).

	2.) diag is calculated with diag_tmp - q_off.  This is the 'real' diagonal, except
	for the sum min_diag_length.  

	3.) real_diag is calculated by keeping only those bits in diag that are less than 
	min_diag_length-1.  This provides a unique number within a range.

	4.) the version is calculated by shifting over 'bits_to_shift', which 
	corresonds to dividing by min_diag_length.

	5.) the combination of the version and the 'real_diag' provide a unique location
	for the diagonal.



	modifications (cfj):
 	 - changed hash_table entries to reduce cache misses (see comments in lookup.c)
	 - when walking through sequence, precompute next_index and prefetch the entry
	 - combined last_hit/version/diag_level into array of struct for better locality.
	 - eliminated the need for the version[] array by changing the value stored as diag_level.
	     (This is done by measuring diag_level along s (rather than q) -- With this measure, 
	     previous hits found in XX[real_diag] will either really be from the same diag, or will 
	     have a diag_level and last_hit much smaller (by at least min_diag_length) than the current 
	     position.)


******************************************************************************/



static Int4
BlastWordFinder_mh_contig(BlastSearchBlkPtr search, LookupTablePtr lookup)
{
    register Uint1Ptr	s;
    register Uint1Ptr s_end;
    Uint1Ptr	subject0;
    BLAST_Diag	diag, diag_tmp, real_diag;
    BLAST_ExtendWordPtr     ewp, ewp_pointer[40];
    Uint4 q_off;
    register Int4 s_off;
    Uint2 context;
    Int4 diff, offset, s_pos, window;
    Int4 min_diag_length, min_diag_mask;
    Int4 *last_hit_p;
    CfjModStruct *combo_array;
    CfjModStruct *ca_ptr[40];
    register ModLookupPositionPtr lookup_pos;
    register Uint4 hit_info;
    
    Int4 char_size, lookup_index, mask, wordsize;
    Int4 next_lindex;
    Int4 * next_nhits_addr;
    Int4 word_width, index=0; 
    Int8 number_of_hits=0;
    register Int4 num_hits;
    register Int4 next_nhits;
    
    BLAST_ExtendWordParamsPtr     ewp_params;
    Boolean			prelim, succeed_to_right;
    ModLAEntry *mod_lt=lookup->mod_lt;
    PV_ARRAY_TYPE *pv_array = lookup->pv_array;
    register PV_ARRAY_TYPE PNTR next_pv_array_addr;
    register PV_ARRAY_TYPE next_pv_val,pv_val;
    
    ewp = NULL;	/* Gets rid of a warning. */
    
    ewp_params=search->ewp_params;
    prelim = search->prelim;
    
    /* The word_width tells how "long" a word is; for a contiguous word it's
       the length of the word. */
    word_width = lookup->wordsize;
    
    wordsize = lookup->wordsize;
    char_size = lookup->char_size;
    mask = lookup->mask;
    subject0 = s = (Uint1Ptr) search->subject->sequence;
    
    window = ewp_params->window;
    offset = ewp_params->offset;
    min_diag_length = ewp_params->min_diag_length;
    min_diag_mask = ewp_params->min_diag_mask;
    
    if (search->current_hitlist == NULL) {
        search->current_hitlist = BlastHitListNew(search); 
    } else { /* Scrub the hitlist. */
        if (search->current_hitlist_purge)
            BlastHitListPurge(search->current_hitlist);
    }
    
    /* subject is too short to find anything! */
    if (word_width > search->subject->length)
        return 0;
    
    /* Move along string to appropriate starting point. */
    s = lookup_find_init(lookup, &index, s);
    lookup_pos=NULL;
    lookup_index = index;
    /* Determines when to stop scanning the database. */
    s_end = subject0 + search->subject->length;
    
    if (pv_array) {
        if ((search->last_context-search->first_context+1) > 1) { 
            /* Only used if more than one context. */
            for (index=search->first_context; index<=search->last_context; index++){
                ewp_pointer[index] = search->context[index].ewp;
                ca_ptr[index]=ewp_pointer[index]->combo_array;
            }
            s_off = (Int4) (s - subject0);
            next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
            next_pv_val = pv_array[next_lindex>>PV_ARRAY_BTS];
            for (;;) {
                do {
                    /* lookup a contiguous word. */
                    s++;
                    lookup_index = next_lindex;
                    
                    if (s == s_end)
                        goto NormalReturn;
                    
                    next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
                    next_pv_array_addr = &pv_array[next_lindex>>PV_ARRAY_BTS];
                    pv_val = next_pv_val;
                    next_pv_val = *next_pv_array_addr;
                    
                } while ((pv_val&(((PV_ARRAY_TYPE) 1)<<(lookup_index&PV_ARRAY_MASK))) == 0); 

                num_hits = mod_lt[lookup_index].num_used;
                
                /* Changed by TLM. */
                lookup_pos = mod_lt[lookup_index].entries;
                hit_info = *((Uint4 *) lookup_pos);
                lookup_pos++;
                
                if(num_hits > 3){
                    if(search->pbp->is_rps_blast) {
                        lookup_pos = (ModLookupPositionPtr) ((Uint1Ptr) lookup->mod_lookup_table_memory + (Uint4) *lookup_pos);
                    } else {
                        lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
                    }
                }
                /* Changed by TLM. */
                s_off = (Int4) (s - subject0);
                
                s_pos = s_off + offset;
                diag_tmp = s_off + min_diag_length;
                
                /* Extend each hit in the linked list */
                /* Each link corresponds to different hits on the query sequence */
                do {  /* for each hit */
                    
#ifdef BLAST_COLLECT_STATS
                    number_of_hits++;
#endif
                    q_off = hinfo_get_pos(hit_info);
                    context = hinfo_get_context(hit_info);
                    hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
                    
                    diag = diag_tmp - q_off;
                    real_diag = (diag_tmp - q_off) & min_diag_mask;
                    /* conxtext dependent values */
                    combo_array = ca_ptr[context];
                    
                    last_hit_p = &combo_array[real_diag].last_hit;
                    diff = s_pos - *last_hit_p;
                    num_hits--;
                    lookup_pos++;
                    
                    /* diff is always greater than window for the first time in a function. */
                    if (diff >= window) {
                        *last_hit_p = s_pos;
                    } else if (diff >= wordsize) {
                        succeed_to_right = TRUE;
                        if (combo_array[real_diag].diag_level <= (s_off+offset)) {
                            ewp = ewp_pointer[context];
                            ewp->actual_window = diff;
                            if (!(search->positionBased)) {
                                if (BlastWordExtend_prelim(search, q_off, s_off, word_width, diag, real_diag, &succeed_to_right, context) != 0)
				    goto ErrorReturn;
                            } else {
                                if (BlastNewWordExtend_prelim(search, q_off, s_off, word_width, diag,  real_diag, &succeed_to_right, context) != 0)
				    goto ErrorReturn;
                            }
                            if (search->current_hitlist->hspcnt > 0 && prelim)
                                goto NormalReturn;
                            
                        } 
                        if (succeed_to_right)
                            *last_hit_p = 0;
                        else
                            *last_hit_p = s_pos;
                    }
		} while(num_hits>0); /* end for pos_cnt... */
	    }
	} else { /* Only one context. */
	
	    ewp=search->context[search->first_context].ewp;
	    combo_array=ewp->combo_array;
            
            next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
            next_pv_val = pv_array[next_lindex>>PV_ARRAY_BTS];
            for (;;) {
	        do {
                    /* lookup a contiguous word. */
                    s++;
                    lookup_index = next_lindex;
                    
                    if (s == s_end)
                        goto NormalReturn;
                    
                    next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
                    next_pv_array_addr = &pv_array[next_lindex>>PV_ARRAY_BTS];
                    pv_val = next_pv_val;
                    next_pv_val = *next_pv_array_addr;
                    
		} while ((pv_val&(((PV_ARRAY_TYPE) 1)<<(lookup_index&PV_ARRAY_MASK))) == 0); 
                
		num_hits = mod_lt[lookup_index].num_used;
                
                /* Changed by TLM. */
		lookup_pos = mod_lt[lookup_index].entries;
		hit_info = *((Uint4 *) lookup_pos);
		lookup_pos++;
                
		if(num_hits > 3){
                    if(search->pbp->is_rps_blast) {
                        lookup_pos = (ModLookupPositionPtr) ((Uint1Ptr) lookup->mod_lookup_table_memory + (Uint4) *lookup_pos);
                    } else {
                        lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
                    }
		}
                
                /* Changed by TLM. */
		s_off = (Int4) (s - subject0);
		s_pos = s_off + offset;
		diag_tmp = s_off + min_diag_length;
                
                /* Extend each hit in the linked list */
                /* Each link corresponds to different hits on the query sequence */
		do {  /* for each hit */
                    
#ifdef BLAST_COLLECT_STATS
                    number_of_hits++;
#endif
                    /* Changed by TLM. */
                    q_off = hit_info;
                    num_hits--;
                    hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
                    lookup_pos++;
                    
                    diag = diag_tmp - q_off;
                    real_diag = diag & min_diag_mask;
                    
                    last_hit_p = &combo_array[real_diag].last_hit;
                    diff = s_pos - *last_hit_p;
                    
                    
                    /* diff is always greater than window for the first time in a function. */
                    if (diff >= window) {
                        *last_hit_p = s_pos;
                    } else if (diff >= wordsize) {
                        succeed_to_right = TRUE;
                        if (combo_array[real_diag].diag_level <= (s_off+offset)) {
                            ewp->actual_window = diff;
                            if (!(search->positionBased)) {
                                if (BlastWordExtend_prelim(search, q_off, s_off, word_width, diag, real_diag, &succeed_to_right, 0) != 0)
				    goto ErrorReturn;
                            } else {
                                if (BlastNewWordExtend_prelim(search, q_off, s_off, word_width, diag, real_diag, &succeed_to_right, 0) != 0)
				    goto ErrorReturn;
                            }
                            if (search->current_hitlist->hspcnt > 0 && prelim)
                                goto NormalReturn;
                            
                        } 
                        if (succeed_to_right)
                            *last_hit_p = 0;
                        else
                            *last_hit_p = s_pos;
                    }
		} while(num_hits > 0); /* end for pos_cnt... */
                
                
            }
	}
    } else {
	if ((search->last_context-search->first_context+1) > 1) {
            /* Only used if more than one context. */
            for (index=search->first_context; index<=search->last_context; index++){
		ewp_pointer[index] = search->context[index].ewp;
		ca_ptr[index]=ewp_pointer[index]->combo_array;
            }
	    next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
	    next_nhits_addr=&mod_lt[next_lindex].num_used ;
	    next_nhits=*next_nhits_addr;
	    s_off = (Int4) (s - subject0);
	    for (;;) {
		do {
                    /* lookup a contiguous word. */
                    lookup_index = next_lindex;
                    s++; 
                    
                    if (s == s_end) goto NormalReturn;
                    
                    next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
                    
                    next_nhits_addr = &mod_lt[next_lindex].num_used;
                    
                    num_hits = next_nhits;
                    next_nhits=*next_nhits_addr;
                    
		} while (num_hits == 0); 
                
                /* Changed by TLM. */
		lookup_pos = mod_lt[lookup_index].entries;
		hit_info = *((Uint4 *) lookup_pos);
		lookup_pos++;
                
		if(num_hits>3){
                    if(search->pbp->is_rps_blast) {
                        lookup_pos = (ModLookupPositionPtr) ((Uint1Ptr) lookup->mod_lookup_table_memory + (Uint4) *lookup_pos);
                    } else {
                        lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
                    }
		}
                /* Changed by TLM. */
	    	s_off = (Int4) (s - subject0);
                
		s_pos = s_off + offset;
		diag_tmp = s_off + min_diag_length;
                
                /* Extend each hit in the linked list */
                /* Each link corresponds to different hits on the query sequence */
 		/* printf(" dtmp:%3d     ",diag_tmp); */
		do{  /* for each hit */
                    
#ifdef BLAST_COLLECT_STATS
                    number_of_hits++;
#endif
                    q_off = hinfo_get_pos(hit_info);
                    context = hinfo_get_context(hit_info);
                    hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
                    
                    diag = diag_tmp - q_off;
                    real_diag = (diag_tmp - q_off) & min_diag_mask;
                    /* conxtext dependent values */
                    combo_array = ca_ptr[context];
                    
                    last_hit_p = &combo_array[real_diag].last_hit;
                    diff = s_pos - *last_hit_p;
                    num_hits--;
                    lookup_pos++;
                    
                    /* diff is always greater than window for the first time in a function. */
                    if (diff >= window) {
                        *last_hit_p = s_pos;
                    } else if (diff >= wordsize) {
                        succeed_to_right = TRUE;
                        if (combo_array[real_diag].diag_level <= (s_off+offset)) {
                            ewp = ewp_pointer[context];
                            ewp->actual_window = diff;
                            if (!(search->positionBased)) {
                                if (BlastWordExtend_prelim(search, q_off, s_off, word_width, diag, real_diag, &succeed_to_right, context) != 0)
				    goto ErrorReturn;
                            } else {
                                if (BlastNewWordExtend_prelim(search, q_off, s_off, word_width, diag, real_diag, &succeed_to_right, context) != 0)
				    goto ErrorReturn;
                            }
                            if (search->current_hitlist->hspcnt > 0 && prelim)
                                goto NormalReturn;
                            
                        } 
                        if (succeed_to_right)
                            *last_hit_p = 0;
                        else
                            *last_hit_p = s_pos;
                    }
		} while(num_hits>0); /* end for pos_cnt... */
	    }
	} else { /* Only one context. */
            
	    ewp=search->context[search->first_context].ewp;
	    combo_array=ewp->combo_array;
            
	    next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
            /* CHanged by TLM. 
               next_nhits_addr=&mod_lt[next_lindex].num_used ;
               next_nhits=*next_nhits_addr;
            */
	    next_nhits=mod_lt[next_lindex].num_used ;
	    for (;;) {
		do {
                    /* lookup a contiguous word. */
                    lookup_index = next_lindex;
                    s++; 
                    if (s == s_end) goto NormalReturn;
                    
                    next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
                    /* CHanged by TLM. 
                       next_nhits_addr = &mod_lt[next_lindex].num_used;
                    */
                    
                    num_hits = next_nhits;
                    next_nhits=mod_lt[next_lindex].num_used;
                    
		} while (num_hits == 0); 
                
                
                /* Changed by TLM. */
		lookup_pos = mod_lt[lookup_index].entries;
		hit_info = *((Uint4 *) lookup_pos);
		lookup_pos++;
                
		if(num_hits > 3){
                    if(search->pbp->is_rps_blast) {
                        lookup_pos = (ModLookupPositionPtr) ((Uint1Ptr) lookup->mod_lookup_table_memory + (Uint4) *lookup_pos);
                    } else {
                        lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
                    }
		}
                
                /* Changed by TLM. */
		s_off = (Int4) (s - subject0);
		s_pos = s_off + offset;
		diag_tmp = s_off + min_diag_length;
                
                /* Extend each hit in the linked list */
                /* Each link corresponds to different hits on the query sequence */
		do {  /* for each hit */
                    
#ifdef BLAST_COLLECT_STATS
                    number_of_hits++;
#endif
                    /* Changed by TLM. */
                    q_off = hit_info;
                    num_hits--;
                    hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
                    lookup_pos++;
                    
                    diag = diag_tmp - q_off;
                    real_diag = diag & min_diag_mask;
                    
                    last_hit_p = &combo_array[real_diag].last_hit;
                    diff = s_pos - *last_hit_p;
                    
/* diff is always greater than window for the first time in a function. */
                    if (diff >= window) {
                        *last_hit_p = s_pos;
                    } else if (diff >= wordsize) {
                        succeed_to_right = TRUE;
                        if (combo_array[real_diag].diag_level <= (s_off+offset)) {
                            ewp->actual_window = diff;
                            if (!(search->positionBased)) {
                                if (BlastWordExtend_prelim(search, q_off, s_off, word_width, diag, real_diag, &succeed_to_right, 0) != 0)
				    goto ErrorReturn;
                            } else {
                                if (BlastNewWordExtend_prelim(search, q_off, s_off, word_width, diag, real_diag, &succeed_to_right, 0) != 0)
				    goto ErrorReturn;
                            }
                            if (search->current_hitlist->hspcnt > 0 && prelim)
                                goto NormalReturn;
                            
                        } 
                        if (succeed_to_right)
                            *last_hit_p = 0;
                        else
                            *last_hit_p = s_pos;
                    }
		} while(num_hits>0); /* end for pos_cnt... */
            }  /* for(;;) */
	}
    }
    
 NormalReturn:
    if (search->prelim)
        search->first_pass_hits += number_of_hits;
    else
        search->second_pass_hits += number_of_hits;
    BlastExtendWordExit(search);
    return search->current_hitlist->hspcnt;
    
 ErrorReturn:
    BlastExtendWordExit(search);
    return 3;
}

/* BlastWordExtend -- extend a word-sized hit to a longer match */
static Int2
BlastWordExtend(BlastSearchBlkPtr search, Int4 q_off, Int4 s_off, Int4 word_width, BLAST_Diag diag, BLAST_Diag real_diag, Boolean PNTR succeed_to_right, Int2 context)
{
	BLAST_ExtendWordPtr     ewp;
	BLAST_ParameterBlkPtr	pbp;
	BLAST_ScoreBlkPtr	sbp;
	BLAST_Score		leftsum, rightsum, rightscore, leftscore;
	Uint1Ptr		query;
	register Uint1Ptr	q, s;
	register Uint1Ptr	q_right, q_left, s_left, q_best_right, q_best_left;
	register BLAST_Score	score, sum; 
	register BLAST_ScorePtr PNTR	matrix;
	register BLAST_Score	x, X;


	q_best_left = NULL;	/* Gets rid of warning. */
	q_best_right = NULL;	/* Gets rid of warning. */

#ifdef BLAST_COLLECT_STATS
	if (search->prelim)
		search->first_pass_extends++;
	else
		search->second_pass_extends++;
#endif

	*succeed_to_right = FALSE;

	ewp=search->context[context].ewp;

	diag -= search->ewp_params->min_diag_length;

	sbp=search->sbp;
	pbp=search->pbp;

	query = search->context[context].query->sequence;
	q = query + q_off;
	s = search->subject->sequence + s_off; 

	X=pbp->X;
	matrix = sbp->matrix;

	score=0;
	sum = 0;
	q_left = q - word_width;
	q_right = q;

/* Look for the highest scoring region in the initial word. */
	while (q > q_left)
	{
		if ((sum += matrix[*q][*s]) > score)
		{
			score = sum;
			q_best_right = q_right;
			q_best_left = q;
		}
		else if (sum <= 0)
		{
			sum = 0;
			q_right = q-1;
		}
		q--; s--;
	}

	if ((x = -score) < X)
		x = X;

	leftsum = rightsum = rightscore = 0;

/* q_left is the where the "attempted" extension along the query was 
stopped (and may be picked up again if the "goto Extend_Left" is used).
q_best_left is the "best" extension along the query that should be
reported. Analogous logic applies to q_right and q_best_right. */

	q_left = q_best_left;
	q_right = q_best_right;

Extend_Left:
	q = q_left;
	s = search->subject->sequence + (q - query) + diag;
	sum = leftsum;

	do
	{
		q--; s--;
		if ((sum += matrix[*q][*s]) > 0)
		{
			do {
				score += sum;
				q_best_left = q;
				q--; s--;
			} while ((sum = matrix[*q][*s]) > 0);
			if ((x = -score) < X)
				x = X;
		}
	} while (sum >= x);


	if (score > rightscore && rightsum > X && -rightscore > X)
	{
		leftscore = score;
		leftsum = sum;
		q_left = q;

		q = q_right;
		s = search->subject->sequence + (q - query) + diag;
		sum = rightsum;

/* "score" is actually the "maxscore", if sum drops by "score", then the
total new score is zero and the extension can stop. */
		if ((x = -score) < X)
			x = X;

		do
		{
			q++; s++;
			if ((sum += matrix[*q][*s]) > 0)
			{
				do {
					score += sum;
					q_best_right = q;
					q++; s++;
				} while ((sum = matrix[*q][*s]) > 0);
				/* do this if score changes. */
				if ((x = -score) < X)
					x = X;
			}
		} while (sum >= x);

		q_right = q;
		if (score > leftscore && leftsum > X && -leftscore > X)
		{
			rightsum = sum;
			rightscore = score;
			goto Extend_Left;
		}
	}

	/* Record how far this diagonal has been traversed,
	"q_right" was the last position on the query sequence.
	ewp_params->offset is added to provide the proper "zero-point" */	
	ewp->combo_array[real_diag].diag_level = q_right - query - q_off + word_width + s_off + search->ewp_params->offset;

	if (score >= pbp->cutoff_s2) /* Score is reportable */
	{

#ifdef BLAST_COLLECT_STATS
		if (search->prelim)
			search->first_pass_good_extends++;
		else
			search->second_pass_good_extends++;
#endif
		s_left = search->subject->sequence + (q_best_left - query) + diag;
		BlastSaveCurrentHsp(search, score, (q_best_left-query), (s_left-search->subject->sequence), (q_best_right-q_best_left+1), context);
	}

	return 0;
}
/*AAS*/
/* BlastWordExtend -- extend a word-sized hit to a longer match,
   BlastNewWordExtend is position based */
static Int2
BlastNewWordExtend(BlastSearchBlkPtr search, Int4 q_off, Int4 s_off, Int4 word_width, BLAST_Diag diag, BLAST_Diag real_diag, Boolean PNTR succeed_to_right, Int2 context)
{
	BLAST_ExtendWordPtr     ewp;
	BLAST_ParameterBlkPtr	pbp;
	BLAST_Score		leftsum, rightsum, rightscore, leftscore;
	Uint1Ptr		query;
	register Uint1Ptr	q, s;
	register Uint1Ptr	q_right, q_left, s_left, q_best_right, q_best_left;
	register BLAST_Score	score, sum; 
	register BLAST_Score	x, X;


#ifdef BLAST_COLLECT_STATS
	if (search->prelim)
		search->first_pass_extends++;
	else
		search->second_pass_extends++;
#endif

	*succeed_to_right = FALSE;

	ewp=search->context[context].ewp;

	diag -= search->ewp_params->min_diag_length;

	pbp=search->pbp;

	query = search->context[context].query->sequence;
	q = query + q_off;
	s = search->subject->sequence + s_off; 

	X=pbp->X;

	score=0;
	sum = 0;
	q_left = q - word_width;
	q_right = q;
        q_best_left = q;
	q_best_right = q; /*AAS*/

/* Look for the highest scoring region in the initial word. */
	while (q > q_left)
	{
		if ((sum += MtrxScorePosSearch(search->sbp,
				(Int4) (q - query),*s)) > score)
		{
			score = sum;
			q_best_right = q_right;
			q_best_left = q;
		}
		else if (sum <= 0)
		{
			sum = 0;
			q_right = q-1;
		}
		q--; s--;
	}

	if ((x = -score) < X)
		x = X;

	leftsum = rightsum = rightscore = 0;

/* q_left is the where the "attempted" extension along the query was 
stopped (and may be picked up again if the "goto Extend_Left" is used).
q_best_left is the "best" extension along the query that should be
reported. Analogous logic applies to q_right and q_best_right. */

	q_left = q_best_left;
	q_right = q_best_right;

Extend_Left_New:
	q = q_left;
	s = search->subject->sequence + (q - query) + diag;
	sum = leftsum;

	do
	{
		q--; s--;
		if (((q -query) >=0) &&
		    (sum += MtrxScorePosSearch(search->sbp,
				(Int4) (q - query),*s)) > 0)
		{
			do {
				score += sum;
				q_best_left = q;
				q--; s--;
			} while (((q -query) >= 0) &&
			   ((sum = MtrxScorePosSearch(search->sbp,
					(Int4) (q - query),*s)) > 0));
			if ((x = -score) < X)
				x = X;
		}
	} while (((q -query) >= 0) && (sum >= x));


	if (score > rightscore && rightsum > X && -rightscore > X)
	{
		leftscore = score;
		leftsum = sum;
		q_left = q;

		q = q_right;
		s = search->subject->sequence + (q - query) + diag;
		sum = rightsum;

/* "score" is actually the "maxscore", if sum drops by "score", then the
total new score is zero and the extension can stop. */
		if ((x = -score) < X)
			x = X;

		do
		{
			q++; s++;
			if ((sum += MtrxScorePosSearch(search->sbp,
					(Int4) (q - query),*s)) > 0)
			{
				do {
					score += sum;
					q_best_right = q;
					q++; s++;
				} while ((sum = MtrxScorePosSearch(search->sbp,
					(Int4) (q - query),*s)) > 0);
				/* do this if score changes. */
				if ((x = -score) < X)
					x = X;
			}
		} while (sum >= x);

		q_right = q;
		if (score > leftscore && leftsum > X && -leftscore > X)
		{
			rightsum = sum;
			rightscore = score;
			goto Extend_Left_New;
		}
	}

	/* Record how far this diagonal has been traversed,
	"q_right" was the last position on the query sequence.
	ewp_params->offset is added to provide the proper "zero-point" */	
	ewp->combo_array[real_diag].diag_level = q_right - query - q_off + word_width + s_off + search->ewp_params->offset;

	if (score >= pbp->cutoff_s2) /* Score is reportable */
	{

#ifdef BLAST_COLLECT_STATS
		if (search->prelim)
			search->first_pass_good_extends++;
		else
			search->second_pass_good_extends++;
#endif
		s_left = search->subject->sequence + (q_best_left - query) + diag;
		BlastSaveCurrentHsp(search, score, (q_best_left-query), (s_left-search->subject->sequence), (q_best_right-q_best_left+1), context);
	}

	return 0;
}



/* BlastWordExtend_prelim -- for timing purposes. */
static Int2
BlastWordExtend_prelim(BlastSearchBlkPtr search, Int4 q_off, Int4 s_off, Int4 word_width, BLAST_Diag diag, BLAST_Diag real_diag, Boolean PNTR succeed_to_right, Int2 context)
{
	BLAST_ExtendWordPtr     ewp;
	BLAST_ParameterBlkPtr	pbp;
	BLAST_ScoreBlkPtr	sbp;
	register Uint1Ptr	q, s, query;
	register Uint1Ptr	q_right, q_left, s_left, q_best_right, q_best_left;
	register BLAST_Score	score, sum;
	register BLAST_ScorePtr PNTR	matrix;
	register BLAST_Score	x, X;
	
	

	q_best_left = NULL;	/* Gets rid of warning. */
	q_best_right = NULL;	/* Gets rid of warning. */

#ifdef BLAST_COLLECT_STATS
	if (search->prelim)
		search->first_pass_extends++;
	else
		search->second_pass_extends++;
#endif

	*succeed_to_right = FALSE;

	ewp=search->context[context].ewp;

	diag -= search->ewp_params->min_diag_length;

	sbp=search->sbp;
	pbp=search->pbp;

	query = search->context[context].query->sequence;
	q = query + q_off;
	s =  search->subject->sequence + s_off; 

	X=pbp->X;
	matrix = sbp->matrix;

	score=0;
	sum = 0;
	q_left = q - word_width;
	q_right = q;

/* Look for the highest scoring region in the initial word. */
	while (q > q_left)
	{
		sum += matrix[*q][*s];
		if (sum > score)
		{
			score = sum;
			q_best_right = q_right;
			q_best_left = q;
		}
		else if (sum <= 0)
		{
			sum = 0;
			q_right = q;
		}
		q--; s--;
	}

	q = q_left = q_best_left;
	s = s_left = search->subject->sequence + (q_left - query) + diag;

	q_left--;

	sum = 0;
	x = X;
	while (sum > x)
	{
		q--; s--;
		if ((sum += matrix[*q][*s]) > 0)
		{
			do {
				score += sum;
				q--; s--;
			} while ((sum = matrix[*q][*s]) > 0);
			q_left = q;
		}
	}
	/* Adjust for extra decrement in do-while loop above. */
	q_left++;
	s_left = search->subject->sequence + (q_left - query) + diag;

/* Extend towards the right (for this preliminary run) if
q_off - q_left is greater than the window. */
	if (((query+q_off)-q_left) >= ewp->actual_window)
	{
		*succeed_to_right = TRUE;
		q = q_right = q_best_right;
		s = search->subject->sequence + (q - query) + diag;
		sum = 0;
/* "score" is actually the "maxscore", if sum drops by "score", then the
total new score is zero and the extension can stop. */
		if ((x = -score) < X)
			x = X;
		while (sum > x)
		{
			q++; s++;
			if ((sum += matrix[*q][*s]) > 0)
			{
				do {
					score += sum;
					q++; s++;
				} while ((sum = matrix[*q][*s]) > 0);
				q_right = q;
				/* do this if score changes. */
				if ((x = -score) < X)
					x = X;
			}
		}
		/* Adjust for extra increment in do-while loop above. */
		q_right--;
	}

	/* Record how far this diagonal has been traversed,
	"q" was the last position on the query sequence.
	ewp->offset is added to provide the proper "zero-point" */	
	ewp->combo_array[real_diag].diag_level = q - query - q_off + word_width + s_off + search->ewp_params->offset;

	if (score >= pbp->cutoff_s2) /* Score is reportable */
	{

#ifdef BLAST_COLLECT_STATS
		if (search->prelim)
			search->first_pass_good_extends++;
		else
			search->second_pass_good_extends++;
#endif

		BlastSaveCurrentHsp(search, score, (q_left-query), (s_left-search->subject->sequence), (q_right-q_left+1), context);
	}

	return 0;
}

/*AAS*/
/* BlastWordExtend_prelim -- for timing purposes. */
static Int2
BlastNewWordExtend_prelim(BlastSearchBlkPtr search, Int4 q_off, Int4 s_off, Int4 word_width, BLAST_Diag diag, BLAST_Diag real_diag, Boolean PNTR succeed_to_right, Int2 context)
{
	BLAST_ExtendWordPtr     ewp;
	BLAST_ParameterBlkPtr	pbp;
	register Uint1Ptr	q, s, query;
	register Uint1Ptr	q_right, q_left, s_left, q_best_right, q_best_left;
	register BLAST_Score	score, sum;
	register BLAST_Score	x, X;
	
	

#ifdef BLAST_COLLECT_STATS
	if (search->prelim)
		search->first_pass_extends++;
	else
		search->second_pass_extends++;
#endif

	*succeed_to_right = FALSE;

	ewp=search->context[context].ewp;

	diag -= search->ewp_params->min_diag_length;

	pbp=search->pbp;

	query = search->context[context].query->sequence;
	q = query + q_off;
	s = search->subject->sequence + s_off; 

	X=pbp->X;

	score=0;
	sum = 0;
	q_left = q - word_width;
	q_right = q+1;
        q_best_left = q;
        q_best_right = q; /*AAS*/

/* Look for the highest scoring region in the initial word. */
	while (q > q_left)
	{
		sum += MtrxScorePosSearch(search->sbp,(Int4) (q - query),*s);
		if (sum > score)
		{
			score = sum;
			q_best_right = q_right;
			q_best_left = q;
		}
		else if (sum <= 0)
		{
			sum = 0;
			q_right = q;
		}
		q--; s--;
	}

	q = q_left = q_best_left;
	s = s_left = search->subject->sequence + (q_left - query) + diag;

	q_left--;

	sum = 0;
	x = X;
	while (((q - query) >= 0) && (sum > x))
	{
		q--; s--;
		if (((q - query) >= 0) && 
		    ((sum += MtrxScorePosSearch(search->sbp,
					(Int4) (q - query),*s)) > 0))
		{
			do {
				score += sum;
				q--; s--;
			} while (((q -query) >= 0) &&
				 ((sum = MtrxScorePosSearch(search->sbp,
					(Int4) ( q- query),*s)) > 0));
			q_left = q;
		}
	}
	/* Adjust for extra decrement in do-while loop above. */
	q_left++;
	s_left = search->subject->sequence + (q_left - query) + diag;

/* Extend towards the right (for this preliminary run) if
q_off - q_left is greater than the window. */
	if (((query+q_off)-q_left) >= ewp->actual_window)
	{
		*succeed_to_right = TRUE;
		q = q_right = q_best_right;
		q--;
		s = search->subject->sequence + (q - query) + diag;
		sum = 0;
/* "score" is actually the "maxscore", if sum drops by "score", then the
total new score is zero and the extension can stop. */
		if ((x = -score) < X)
			x = X;
		while (sum > x)
		{
			q++; s++;
			if ((sum += MtrxScorePosSearch(search->sbp,
					(Int4) (q - query),*s)) > 0)
			{
				do {
					score += sum;
					q++; s++;
				} while ((sum = MtrxScorePosSearch(search->sbp,
						(Int4) (q - query),*s)) > 0);
				q_right = q;
				/* do this if score changes. */
				if ((x = -score) < X)
					x = X;
			}
		}
		/* Adjust for extra increment in do-while loop above. */
		q_right--;
	}

	/* Record how far this diagonal has been traversed,
	"q" was the last position on the query sequence.
	ewp->offset is added to provide the proper "zero-point" */	
	ewp->combo_array[real_diag].diag_level = q - query -q_off + word_width + s_off + search->ewp_params->offset;

	if (score >= pbp->cutoff_s2) /* Score is reportable */
	{

#ifdef BLAST_COLLECT_STATS
		if (search->prelim)
			search->first_pass_good_extends++;
		else
			search->second_pass_good_extends++;
#endif

		BlastSaveCurrentHsp(search, score, (q_left-query), (s_left-search->subject->sequence), (q_right-q_left+1), context);
	}

	return 0;
}


/* Ungapped extension a blastn type word hit, to be used in Mega BLAST with
   discontiguous word models.  

	BlastSearchBlkPtr search: main BLAST structure,
	Int4 q_off: offset of query sequence,
	Int4 s_off: offset of subject sequence, divided by four!
        Return: true if ungapped score below cutoff (to indicate that this
                HSP should be deleted.
*/

Boolean
BlastNtWordUngappedExtend(BlastSearchBlkPtr search, Int4 q_off, Int4 s_off, 
                          Int4 cutoff)
{
	register Uint1Ptr	q;
	register BLAST_ScorePtr PNTR	matrix;
	register BLAST_Score	sum, score;
	Uint1	ch;
	Uint1Ptr query0, subject0, sf, q_beg, q_end, s_beg, s_end, s, start;
	BLAST_Score	X;
	Int2		remainder, base;
        BLAST_ParameterBlkPtr   pbp;
        BLAST_ScoreBlkPtr       sbp;
	Int4 q_avail, s_avail;

        base = 3 - (s_off % 4);

        sbp=search->sbp;
        pbp=search->pbp;

	matrix = sbp->matrix;
	matrix = sbp->matrix;
	query0 = (Uint1Ptr) search->context[search->first_context].query->sequence;
	subject0 = (Uint1Ptr) search->subject->sequence;
        q_avail = search->context[search->first_context].query->length - q_off;
        s_avail = search->subject->length - s_off;

	q = q_beg = q_end = query0 + q_off;
	s = s_beg = s_end = subject0 + s_off/READDB_COMPRESSION_RATIO;
	if (q_off < s_off) {
           start = (Uint1Ptr) search->subject->sequence + 
              (s_off-q_off)/READDB_COMPRESSION_RATIO;
           remainder = 3 - ((s_off-q_off)%READDB_COMPRESSION_RATIO);
	} else {
           start = (Uint1Ptr) search->subject->sequence;
           remainder = 3;
	}

	/* Find where positive scoring starts & ends within the word hit */
	score = 0;
        sum = 0;

	X = pbp->X;

	/* extend to the left */
	do {
           if (base == 3) {
              s--;
              base = 0;
           } else
              base++;
           ch = *s;
           if ((sum += matrix[*--q][READDB_UNPACK_BASE_N(ch, base)]) > 0) {
              q_beg = q;
              score += sum;
              sum = 0;
           } else if (sum < X)
              break;
	} while ((s > start) || (s == start && base <= remainder));

        if (score >= cutoff) 
           return FALSE;

        if (q_avail < s_avail) {
           sf = subject0 + (s_off + q_avail)/READDB_COMPRESSION_RATIO;
           remainder = 3 - ((s_off + q_avail)%READDB_COMPRESSION_RATIO);
        } else {
           sf = subject0 + (search->subject->length)/READDB_COMPRESSION_RATIO;
           remainder = 3 - ((search->subject->length)%READDB_COMPRESSION_RATIO);
        }
	/* extend to the right */
	q = q_end;
	s = s_end;
	sum = 0;
        base = 3 - (s_off % 4);

	while (s < sf || (s == sf && base >= remainder)) {
           ch = *s;
           if ((sum += matrix[*q++][READDB_UNPACK_BASE_N(ch, base)]) > 0) {
              q_end = q;
              score += sum;
              sum = 0;
           } else if (sum < X)
              break;
           if (base == 0) {
              base = 3;
              s++;
           } else
              base--;
	}
        
        return (score < cutoff);
}

/* Extend a blastn type word hit.  

	BlastSearchBlkPtr search: main BLAST structure,
	Int4 q_off: offset of query sequence,
	Int4 s_off: offset of subject sequence, divided by four!
	BLAST_Diag real_diag: diagonal,
	Int2 context: must be 0 (plus strand) or 1 (minus strand).
*/
Int2
BlastNtWordExtend(BlastSearchBlkPtr search, Int4 q_off, Int4 s_off, BLAST_Diag real_diag, Int2 context)
{
	register Uint1Ptr	q;
	register BLAST_ScorePtr PNTR	matrix;
	register BLAST_Score	sum, score;
	Uint1	ch;
	Uint1Ptr query0, subject0, sf, q_beg, q_end, s_beg, s_end, s, start;
	BLAST_Score	x, X;
	Int2		remainder;
        BLAST_ExtendWordPtr     ewp;
        BLAST_ParameterBlkPtr   pbp;
        BLAST_ScoreBlkPtr       sbp;
	Int4 q_avail, s_avail;

#ifdef BLAST_COLLECT_STATS
	search->second_pass_extends++;
#endif
	ewp=search->context[context].ewp;

        sbp=search->sbp;
        pbp=search->pbp;

	matrix = sbp->matrix;
	matrix = sbp->matrix;
	query0 = (Uint1Ptr) search->context[context].query->sequence;
	subject0 = (Uint1Ptr) search->subject->sequence;
        q_avail = search->context[context].query->length - q_off;
        s_avail = search->subject->length - s_off*READDB_COMPRESSION_RATIO;
        if (q_avail < s_avail)
        {
                sf = subject0 + s_off + q_avail/READDB_COMPRESSION_RATIO;
		remainder = q_avail%READDB_COMPRESSION_RATIO;
        }
        else
        {
                sf = subject0 + (search->subject->length)/READDB_COMPRESSION_RATIO;
		remainder = (search->subject->length)%READDB_COMPRESSION_RATIO;
        }

	q = q_beg = q_end = query0 + q_off;
	s = s_beg = s_end = subject0 + s_off;
	if (q_off < s_off*READDB_COMPRESSION_RATIO)
	{
		start = (Uint1Ptr) search->subject->sequence + (s_off-q_off/READDB_COMPRESSION_RATIO);
	}
	else
	{
		start = (Uint1Ptr) search->subject->sequence;
	}

	/* Find where positive scoring starts & ends within the word hit */
	score = sum = 0;

	x = X = pbp->X;

	/* extend to the left */
	do {
		s--;
		ch = *s;
		if ((sum += matrix[*--q][READDB_UNPACK_BASE_4(ch)]) > 0) {
			q_beg = q;
			score += sum;
			sum = 0;
			if ((x = -score) < X)
				x = X;
		}
		else
			if (sum < x)
				break;
		if ((sum += matrix[*--q][READDB_UNPACK_BASE_3(ch)]) > 0) {
			q_beg = q;
			score += sum;
			sum = 0;
			if ((x = -score) < X)
				x = X;
		}
		else
			if (sum < x)
				break;
		if ((sum += matrix[*--q][READDB_UNPACK_BASE_2(ch)]) > 0) {
			q_beg = q;
			score += sum;
			sum = 0;
			if ((x = -score) < X)
				x = X;
		}
		else
			if (sum < x)
				break;
		if ((sum += matrix[*--q][READDB_UNPACK_BASE_1(ch)]) > 0) {
			q_beg = q;
			score += sum;
			sum = 0;
			if ((x = -score) < X)
				x = X;
		}
		else
			if (sum < x)
				break;
	} while (s > start);

	/* There is still another partial byte to be extended through. */
	if (sum >= x && start != (Uint1Ptr) search->subject->sequence)
	{
		s--;
		ch = *s;
		while (q > query0)
		{
			if ((sum += matrix[*--q][READDB_UNPACK_BASE_4(ch)]) > 0) 
			{
				q_beg = q;
				score += sum;
				sum = 0;
				if ((x = -score) < X)
					x = X;
			}
			else if (sum < x)
			{
				break;
			}
			ch >>= 2;
		}
	}

	/* extend to the right */
	q = q_end;
	s = s_end;
	sum = 0;
	while (s < sf) 
	{
		ch = *s;
		if ((sum += matrix[*q++][READDB_UNPACK_BASE_1(ch)]) > 0) 
		{
			q_end = q;
			score += sum;
			sum = 0;
			if ((x = -score) < X)
				x = X;
		}
		else if (sum < x)
		{
				break;
		}

		if ((sum += matrix[*q++][READDB_UNPACK_BASE_2(ch)]) > 0) 
		{
			q_end = q;
			score += sum;
			sum = 0;
			if ((x = -score) < X)
				x = X;
		}
		else if (sum < x)
		{
				break;
		}

		if ((sum += matrix[*q++][READDB_UNPACK_BASE_3(ch)]) > 0) 
		{
			q_end = q;
			score += sum;
			sum = 0;
			if ((x = -score) < X)
				x = X;
		}
		else if (sum < x)
		{
				break;
		}

		if ((sum += matrix[*q++][READDB_UNPACK_BASE_4(ch)]) > 0) 
		{
			q_end = q;
			score += sum;
			sum = 0;
			if ((x = -score) < X)
				x = X;
		}
		else if (sum < x)
		{
				break;
		}
		s++;
	}

	/* extend into the final, partially packed byte (if one exists) */
/* If the query ends before the subject, then don't extend any more as the query 
has no remainder. */
	if (remainder > 0 && sum >= x)
	{
		ch = *sf;

		while (remainder > 0)
		{
			if ((sum += matrix[*q++][READDB_UNPACK_BASE_1(ch)]) > 0) 
			{
				q_end = q;
				score += sum;
				sum = 0;
				if ((x = -score) < X)
					x = X;
			}
			else if (sum < x)
			{
					break;
			}
#ifdef OLD_BYTE_ORDER
			ch >>= 2;
#else
			ch <<= 2;
#endif
			remainder--;
		}
	} /* End ungapped alignment */

	/* Record how far this diagonal has been traversed */
	/* 	ewp->combo_array[real_diag].diag_level = q_end - query0 + search->ewp_params->offset; */
	ewp->combo_array[real_diag].diag_level = (q_end - query0 - q_off) + s_off*READDB_COMPRESSION_RATIO + search->ewp_params->offset;



	if (score >= pbp->cutoff_s2) /* Score is reportable */
	{
#ifdef BLAST_COLLECT_STATS
           search->second_pass_good_extends++;
#endif
           if (search->pbp->do_sum_stats)
              BlastSaveCurrentHsp(search, score, (q_beg-query0), (q_beg-query0+READDB_COMPRESSION_RATIO*s_off-q_off), (q_end-q_beg), context);
           else
              BlastNtSaveCurrentHsp(search, score, (q_beg-query0), (q_beg-query0+READDB_COMPRESSION_RATIO*s_off-q_off), (q_end-q_beg), context, q_off, READDB_COMPRESSION_RATIO*s_off);
	}

	return 0;
}

/*
	search_nt_orig -- an adaptation of the original search_nt() function
	of BLASTN

	* Can this ever be called?
	*  - It is only called for blastn, only from BlastWordFinder_mh().
	*  - BlastWordFinder_mh() is only called if BlastExtendWordSearch() is called w/ multiphe_hits==TRUE
	*  - BlastExtendWordSearch() is called in 2 places:
	*       BLASTPerform2PassSearch(called w/ multiple_hits=TRUE)
	*         which is called from BLASTPerfromSearch if search->pbp->two_pass_method
	*       BLASTPerformFinalSearch(called w/ search->pbp->multiple_hits_only)
	*  * so multiple_hits_only, or two_pass_method must be set for this to be called.
	*  For blastn, these are set to false in blastool, blastutl. 
	*  These can be set to TRUE in blastpgp.c, but can blastn alse be true in this case?
	*
	*  I have updated the array accesses to use the new mod_lt[], as the other WordFinder routines
	*  now do, but I have not been able to test this change.
	* -cfj
*/
static Int4
BlastNtWordFinder_mh(BlastSearchBlkPtr search, LookupTablePtr lookup)
{
	register Uint1Ptr s, s_end;
	Uint1Ptr subject0;
	BLAST_Diag	diag, diag_tmp, real_diag;
	BLAST_ExtendWordPtr     ewp;
	BLAST_ExtendWordParamsPtr     ewp_params;
	BLAST_WordFinderPtr	wfp;
	CfjModStruct *combo_array;
        register Int4  diff, window, lookup_index, mask;
        Int4  char_size, index=0, current_count;
	register ModLookupPosition hit_info;
        Int4            s_pos, q_off, s_off, offset, virtual_wordsize, wordsize, compressed_wordsize, compression_factor;
        register Int4 bits_to_shift, min_diag_length, min_diag_mask;
	register Int4 num_hits;
	register Int4 next_nhits;
        Int4 * next_nhits_addr;
	register ModLookupPositionPtr lookup_pos;
	int next_lindex;
    	PV_ARRAY_TYPE *pv_array = lookup->pv_array;
    	register PV_ARRAY_TYPE PNTR next_pv_array_addr;
    	register PV_ARRAY_TYPE next_pv_val,pv_val;
#if 1
	ModLAEntry *mod_lt=lookup->mod_lt;
#endif

	ewp_params=search->ewp_params;

	wfp = search->wfp_second;
        char_size = lookup->char_size;
        mask = lookup->mask;
        offset = ewp_params->offset;
        window = ewp_params->window;
        subject0 = s = (Uint1Ptr) search->subject->sequence;
        min_diag_length = ewp_params->min_diag_length;
        bits_to_shift = ewp_params->bits_to_shift;
        min_diag_mask = ewp_params->min_diag_mask;

        if (search->current_hitlist == NULL)
        {
                search->current_hitlist = BlastHitListNew(search);
        }
        else
        { /* Scrub the hitlist. */
                if (search->current_hitlist_purge)
                        BlastHitListPurge(search->current_hitlist);
        }

	compressed_wordsize = lookup->wordsize;
	wordsize = wfp->wordsize;

/* The subject sequence is too short, exit this function now. */
	if (wordsize > search->subject->length)
		goto NormalReturn;

	s = lookup_find_init(lookup, &index, s);
        lookup_index = index;
/* Determines when to stop scanning the database; does not include remainder. */
        s_end = subject0 + (search->subject->length)/READDB_COMPRESSION_RATIO;
	compression_factor = wfp->compression_ratio*compressed_wordsize;
	virtual_wordsize = READDB_COMPRESSION_RATIO*compressed_wordsize;

	/* conxtext dependent values */
	ewp=search->context[search->first_context].ewp;
	combo_array=ewp->combo_array;

	if (pv_array)
	{
           next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
	   next_pv_val = pv_array[next_lindex>>PV_ARRAY_BTS];

           for (;;) {
              do {
                        /* lookup a contiguous word. */
                        lookup_index = next_lindex;
                        s++;

                        if (s == s_end) goto NormalReturn;

                        next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
			next_pv_array_addr = &pv_array[next_lindex>>PV_ARRAY_BTS];
                    	pv_val = next_pv_val;
                    	next_pv_val = *next_pv_array_addr;
                } while ((pv_val&(((PV_ARRAY_TYPE) 1)<<(lookup_index&PV_ARRAY_MASK))) == 0);


		num_hits = mod_lt[lookup_index].num_used;
		lookup_pos = mod_lt[lookup_index].entries;
		hit_info = *((Uint4 *) lookup_pos);
		lookup_pos++;

		if(num_hits>3){
		  lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
		}

		s_off = s-subject0+1;
		diag_tmp = s_off*READDB_COMPRESSION_RATIO + min_diag_length;
		s_pos = (s-subject0)*READDB_COMPRESSION_RATIO+offset;
		/* Extend each hit in the linked list */
		do {
		        q_off = hit_info;
			num_hits--;
			hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
			lookup_pos++;

		    	diag = diag_tmp - q_off;
		    	real_diag = diag & min_diag_mask;

			diff = s_pos - combo_array[real_diag].last_hit;

			if (diff >= window)
			{
				combo_array[real_diag].last_hit = s_pos;
			}
			else if (diff >= virtual_wordsize)
			{
#ifdef BLAST_COLLECT_STATS
				search->second_pass_hits++;
#endif
        			current_count = search->current_hitlist->hspcnt;
				if (combo_array[real_diag].diag_level <= (s_off*READDB_COMPRESSION_RATIO+offset))
				{
					if (BlastNtWordExtend(search, q_off, s_off, real_diag, search->first_context) != 0)
						goto ErrorReturn;
				}
				/* If no HSP's saved, save last hit. */
				if (current_count == search->current_hitlist->hspcnt)
					combo_array[real_diag].last_hit = s_pos;
				else
					combo_array[real_diag].last_hit = 0;
			}

		} while (num_hits>0);
	   }
	}
	else
	{
           next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
           next_nhits_addr=&mod_lt[next_lindex].num_used ;
           next_nhits=*next_nhits_addr;

           for (;;) {
              do {
                        /* lookup a contiguous word. */
                        lookup_index = next_lindex;
                        s++;

                        if (s == s_end) goto NormalReturn;

                        next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);

                        next_nhits_addr = &mod_lt[next_lindex].num_used;

                        num_hits = next_nhits;
                        next_nhits=*next_nhits_addr;
                } while (num_hits == 0);

		lookup_pos = mod_lt[lookup_index].entries;
		hit_info = *((Uint4 *) lookup_pos);
		lookup_pos++;

		if(num_hits>3){
		  lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
		}

		s_off = s-subject0+1;
		diag_tmp = s_off*READDB_COMPRESSION_RATIO + min_diag_length;
		s_pos = (s-subject0)*READDB_COMPRESSION_RATIO+offset;
		/* Extend each hit in the linked list */
		do {
		        q_off = hit_info;
			num_hits--;
			hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
			lookup_pos++;

		    	diag = diag_tmp - q_off;
		    	real_diag = diag & min_diag_mask;

			diff = s_pos - combo_array[real_diag].last_hit;

			if (diff >= window)
			{
				combo_array[real_diag].last_hit = s_pos;
			}
			else if (diff >= virtual_wordsize)
			{
#ifdef BLAST_COLLECT_STATS
				search->second_pass_hits++;
#endif
        			current_count = search->current_hitlist->hspcnt;
				if (combo_array[real_diag].diag_level <= (s_off*READDB_COMPRESSION_RATIO+offset))
				{
					if (BlastNtWordExtend(search, q_off, s_off, real_diag, search->first_context) != 0)
						goto ErrorReturn;
				}
				/* If no HSP's saved, save last hit. */
				if (current_count == search->current_hitlist->hspcnt)
					combo_array[real_diag].last_hit = s_pos;
				else
					combo_array[real_diag].last_hit = 0;
			}

		} while (num_hits>0);
	   }
	}

NormalReturn:
	BlastExtendWordExit(search);
        return search->current_hitlist->hspcnt;

ErrorReturn:
	BlastExtendWordExit(search);
	return 3;
}




#define CACHE_PREFETCH_BLOCKSIZE 64

static inline Uint1	LoadDBElement(unsigned char *pSBase, Uint4 nucleotideOffset)
{
	Uint4	quadOffset;
	Uint4   oddOffset;
	Uint1	*pStart;
	Uint1	thechar;
			
	quadOffset = (nucleotideOffset) / 4;
	oddOffset = (nucleotideOffset) & 3;
	
	pStart = pSBase + quadOffset;

	thechar = *pStart;
	
	thechar >>= (3-oddOffset) * 2;
	
	thechar &= 0x3;
	
	return thechar;
}

static int expandword_immediate(BlastSearchBlkPtr search, LookupTablePtr lookup, Uint4 *pQ, int count, Int4 s_off_nucl, Uint4 firstQ)
{
	int								index = 0;
	Uint1Ptr						s_BASE;
	Uint1Ptr 						s, s_end;
	Int4							mask;
	Int4            				char_size;
	Int4            				lookup_index;
	Uint1Ptr						subject0;
	Int4            				q_off;
	Int4							left, right;
	
	BLAST_Diag      				diag, diag_tmp, real_diag;

	Uint1Ptr        				q;

	Uint1Ptr        				nextQ;
	Int4							nextQ_off;	
	
	Int4							compression_factor;	
	Uint1Ptr        				q_end, query0;
	Int4							compressed_wordsize;
	Int4							min_diag_length;
	Int4            				min_diag_mask;
	Int4            				virtual_wordsize;
	CfjModStruct   					*combo_array;
	Int4            				offset;
	Uint4							s_offByte;
	
	unsigned long			dstinfo;

#if DO_TT6
	startTrace_alt("expand_sameS.tt6", params);
#endif	
	
	// 4 vectors, 1 block, 64 bytes apart
	dstinfo =	((16/16) << 24) | (1 << 16) | 16;

	vec_dst(query0+firstQ-16, dstinfo, 0);

	s	 			= (Uint1Ptr)search->subject->sequence;
	s_end			= s + (search->subject->length) / READDB_COMPRESSION_RATIO;
	mask 			= lookup->mask;
	subject0		= s;
	
	offset 			= search->ewp_params->offset;
	
	min_diag_length = search->ewp_params->min_diag_length;
	min_diag_mask 	= search->ewp_params->min_diag_mask;
 
	query0 			= search->context[search->first_context].query->sequence;
	q_end 			= query0 + search->context[search->first_context].query->length;	
	combo_array 	= search->context[search->first_context].ewp->combo_array;
	
	q_off = firstQ;
	
	s_offByte = ((s_off_nucl+3)/4)+1;
	
	diag_tmp = s_offByte * READDB_COMPRESSION_RATIO + min_diag_length;

	for (index = 0; index< 	count; index++) {
				
		q_off -= 8;		
		
		if (1) {

			Int4	qOff_extend = q_off + (s_offByte*4)-s_off_nucl;
				
			diag = diag_tmp - qOff_extend;

			real_diag = diag & min_diag_mask;
			 
			if ( !(combo_array[real_diag].diag_level >= (s_offByte * READDB_COMPRESSION_RATIO + offset)) )
			{

#ifdef BLAST_COLLECT_STATS
				search->second_pass_hits++;
#endif


				if (BlastNtWordExtend(search, qOff_extend, s_offByte, real_diag, search->first_context) != 0)
					goto ErrorReturn;
			
			}
		
		
		}
		
		// get q_off for next		
		q_off = *pQ++;
	}

	return 0;	

ErrorReturn:
	return -1;
	
}



int FindNTMatchList_toextend(BlastSearchBlkPtr search, LookupTablePtr lookup, Uint4 stepsize, Int4 nuclsize)
{
	ModLAEntry     					*mod_lt;
	Uint1Ptr 						s, s_end;
	Int4							mask;
	Int4            				char_size;
	Int4           					*next_nhits_addr;
	register Int4				   	next_nhits;
	int             				next_lindex;
	Int4            				lookup_index;
	register Int4   				num_hits;
	Uint1Ptr						subject0;
	Int4    		   		     	s_off;
	register ModLookupPositionPtr 	lookup_pos;
	ModLookupPosition		 		hit_info;
	Int4							hits_index;
	Int4							outercount;
	Int4							outerindex;	
		
	Int4							stepcount;
		
	unsigned long					dstinfo;
	
	Uint1Ptr						sBase;
	Uint1Ptr						prevS;
	
	Uint4							curoffset;
	
	Int4							wordmask;
	
	Int4							partshift;
	
	// set up masks and shifts
	wordmask = 0xffff;
	
	partshift = 0;
			
	if (nuclsize < 8) {
		partshift = 8-nuclsize;
		partshift *= 2;
		wordmask >>= partshift;
	}
			
	// 4 vectors, 1 block, 64 bytes apart
	dstinfo =	((CACHE_PREFETCH_BLOCKSIZE/16) << 24) | (1 << 16) | CACHE_PREFETCH_BLOCKSIZE;
	
	mod_lt 			= lookup->mod_lt;
	s	 			= (Uint1Ptr)search->subject->sequence;
	s_end			= s + (search->subject->length) / READDB_COMPRESSION_RATIO;
	mask 			= lookup->mask;
	char_size		= lookup->char_size;
	subject0		= s;
	
	// stepcount = length - 8 / stepsize
	stepcount		= (((s_end-s) * 4)-8) / stepsize;
//	stepcount		= ((s_end-s) * 4) / stepsize;
						
	for (outerindex=0; outerindex<=stepcount+1; outerindex++) {

		curoffset = outerindex*stepsize;
		
		s = subject0 + (curoffset / 4);
		
		lookup_index = (*s) << 16 | ((*(s+1)) << 8) | ((*(s+2)) << 0);
		
		lookup_index >>= (8-(2*(curoffset & 3)));
				
		s_off = (s - subject0);
				
		lookup_index >>= partshift;		
				
		lookup_index &= wordmask;
		
		next_nhits_addr = &mod_lt[lookup_index].num_used;

		num_hits = *next_nhits_addr;
	
		if (!num_hits) continue;	

		lookup_pos = mod_lt[lookup_index].entries;

		hit_info = *((Uint4 *) lookup_pos);
		
		if (num_hits <= 3) {

			expandword_immediate(search, lookup, lookup_pos+1, num_hits, (s_off)*4+(curoffset&3), hit_info);
         
		} else {

			lookup_pos++;
			lookup_pos = *((ModLookupPositionPtr PNTR) lookup_pos);			
			
			expandword_immediate(search, lookup, lookup_pos, num_hits, (s_off)*4+(curoffset&3), hit_info);
					
		}
								
	}	
	
	
exit:

	return 0;
	
}



static int scalar_expandword(BlastSearchBlkPtr search, LookupTablePtr lookup, Uint4 *pQ, int count, Int4 s_off_nucl, Uint4 firstQ, Uint4 maxscan)
{
	int								index = 0;
	Uint1Ptr						s_BASE;
	Uint1Ptr 						s, s_end;
	Int4							mask;
	Uint1Ptr						subject0;
	Int4            				q_off;
	Int4            				q_off_next;
	Int4							left, right;
	
	BLAST_Diag      				diag, diag_tmp, real_diag;
	
	Uint1Ptr        				q_end, query0;
	Int4							min_diag_length;
	Int4            				min_diag_mask;
	CfjModStruct   					*combo_array;
	Uint4							s_offByte;
		
	Int4							maxremainingextendleft;
	Int4							maxremainingextendright;

	Int4							spacetoendleft;
	Int4							spacetoendright;

	Int4							qroomleft;

	Uint1							dbchar;
	Int4							extendOffset;
	
	Int4							offset;

	Uint4							neededExtension;

	unsigned long					dstinfo;
				
	// 4 vectors, 1 block, 64 bytes apart
	dstinfo =	(1 << 24) | (1 << 16) | 1;
		
	vec_dst(query0+firstQ, dstinfo, 2);	
		
#if DO_TT6
	startTrace_alt("expand_sameS.tt6", params);
#endif	

	neededExtension =  search->wfp_second->wordsize - 8;


	s	 			= (Uint1Ptr)search->subject->sequence;
	s_end			= s + (search->subject->length) / READDB_COMPRESSION_RATIO;
	mask 			= lookup->mask;
	subject0		= s;
	
	offset 			= search->ewp_params->offset;
	
	min_diag_length = search->ewp_params->min_diag_length;
	min_diag_mask 	= search->ewp_params->min_diag_mask;
 
	query0 			= search->context[search->first_context].query->sequence;
	q_end 			= query0 + search->context[search->first_context].query->length;	
	combo_array 	= search->context[search->first_context].ewp->combo_array;

	s_BASE = subject0;
	
	q_off_next = firstQ-8;
	
	s_offByte = ((s_off_nucl+8)/4);
	
	diag_tmp = s_offByte * READDB_COMPRESSION_RATIO + min_diag_length;

	spacetoendleft 	= s_off_nucl;
	
	spacetoendright = (s_end-s_BASE)*4-s_off_nucl-8;

	if (spacetoendleft > maxscan) 	spacetoendleft = maxscan;
	if (spacetoendright > maxscan) 	spacetoendright = maxscan;

	for (index = 0; index< 	count; index++) {
		
		q_off = q_off_next;

		// get q_off for next		
		q_off_next = (*pQ++)-8;
		
		vec_dst(query0+q_off_next, dstinfo, 2);

		left 	= 0;
		right 	= 0;

		///////////////////////////////////////////////
		// set up scanning distances left & right
		///////////////////////////////////////////////

		// expand left
		maxremainingextendleft = spacetoendleft;
		
		// if search will go beyond q boundary, limit to boundary
		if (q_off < maxremainingextendleft)	maxremainingextendleft = q_off;
		
		// jak! needed?
		if (maxremainingextendleft < 0) maxremainingextendleft = 0;

		// expand right
		maxremainingextendright = spacetoendright;
		
		qroomleft = (q_end-query0)-q_off-8;
		
		if (qroomleft < maxremainingextendright)	maxremainingextendright = qroomleft;
		
		// jak! needed?
		if (maxremainingextendright < 0) maxremainingextendright = 0;


		///////////////////////////////////////////////
		// extend left		
		///////////////////////////////////////////////

		extendOffset = 0;
					
		while (maxremainingextendleft--) {
			Uint1	queryChar;
			dbchar = LoadDBElement(s_BASE, s_off_nucl+extendOffset-1);
			
			queryChar = *(query0+(q_off+extendOffset-1));
			
			if (dbchar != queryChar) {
				break;
			}
			
			left++;
			extendOffset--;
			
		}

		///////////////////////////////////////////////
		// extend right		
		///////////////////////////////////////////////
						
		extendOffset = 0;
		
		while (maxremainingextendright--) {
			dbchar = LoadDBElement(s_BASE, s_off_nucl+8+extendOffset);
			
			if (dbchar != *(query0+q_off+8+extendOffset)) {
				break;
			}
			
			right++;
			extendOffset++;
			
		}

		if (left+right >= neededExtension) 
		{
			Int4	qOff_extend = q_off + (s_offByte*4)-s_off_nucl;
				
			diag = diag_tmp - qOff_extend;

			real_diag = diag & min_diag_mask;
			 
			if ( !(combo_array[real_diag].diag_level >= (s_offByte * READDB_COMPRESSION_RATIO + offset)) )
			{

#ifdef BLAST_COLLECT_STATS
				search->second_pass_hits++;
#endif


				if (BlastNtWordExtend(search, qOff_extend, s_offByte, real_diag, search->first_context) != 0)
					goto ErrorReturn;
			
			}
		
		}
		
	}

	return 0;	

ErrorReturn:
	return -1;
	
}

static int FindNTMatchList_Bitmask(BlastSearchBlkPtr search, LookupTablePtr lookup, Uint4 stepsize, PV_ARRAY_TYPE *pv_array)
{
	ModLAEntry     					*mod_lt;
	Uint1Ptr 						s, s_end;
	Int4							mask;
	Int4            				char_size;
	register Int4   				num_hits;
	Uint1Ptr						subject0;
	Int4    		   		     	s_off;
	register ModLookupPositionPtr 	lookup_pos;
	ModLookupPosition		 		hit_info;
	Int4							outerindex;	
		
	Int4							stepcount;	
	
	Uint4							curoffset;
	Uint4							nextoffset;

	Uint4            				lookup_index;
	Uint4            				next_index;

	Int4							wordlen;

	register PV_ARRAY_TYPE 			next_pv_val,pv_val;

	unsigned long					dstinfo;
				
	// 2 vectors, 1 block
	dstinfo =	((2) << 24) | (1 << 16) | 0;
	
	wordlen 		= search->wfp_second->wordsize;
	
	mod_lt 			= lookup->mod_lt;
	s	 			= (Uint1Ptr)search->subject->sequence;
	s_end			= s + (search->subject->length) / READDB_COMPRESSION_RATIO;
	mask 			= lookup->mask;
	char_size		= lookup->char_size;
	subject0		= s;
	
	// stepcount = length - 8 / stepsize
	stepcount		= (((s_end-s) * 4)-8) / stepsize;
//	stepcount		= (((s_end-s) * 4)) / stepsize;

	if (stepcount < 1) return 0;

	outerindex = 0;

	nextoffset = outerindex*stepsize;
	
	s = subject0 + (nextoffset / 4);
	
	next_index = (*s) << 16 | ((*(s+1)) << 8) | ((*(s+2)) << 0);
	
	next_index >>= (8-(2*(nextoffset & 3)));
							
	next_index &= 0xffff;
	
	next_pv_val = pv_array[next_index>>PV_ARRAY_BTS];
	
	next_pv_val &= ((PV_ARRAY_TYPE) 1)<<(next_index&PV_ARRAY_MASK);
			
	for (outerindex=1; outerindex<=stepcount; outerindex++) {

		vec_dst(s+(stepsize/4), dstinfo, 1);
		
		curoffset 		= nextoffset;
		lookup_index 	= next_index;
		pv_val			= next_pv_val;

		nextoffset = outerindex*stepsize;
		
		s = subject0 + (nextoffset / 4);
		
		next_index = (*s) << 16 | ((*(s+1)) << 8) | ((*(s+2)) << 0);
		
		next_index >>= (8-(2*(nextoffset & 3)));
								
		next_index &= 0xffff;
		
		next_pv_val = pv_array[next_index>>PV_ARRAY_BTS];
		
		next_pv_val &= ((PV_ARRAY_TYPE) 1)<<(next_index&PV_ARRAY_MASK);
						
		s_off = curoffset/4;
		
		if (pv_val) {
		
			num_hits = mod_lt[lookup_index].num_used;

			lookup_pos = mod_lt[lookup_index].entries;

			hit_info = *((Uint4 *) lookup_pos);
				
			if (num_hits <= 3) {

				scalar_expandword(search, lookup, lookup_pos+1, num_hits, (s_off)*4+(curoffset&3), hit_info, wordlen-8);
	         
			} else {

				lookup_pos++;
				lookup_pos = *((ModLookupPositionPtr PNTR) lookup_pos);			

				scalar_expandword(search, lookup, lookup_pos, num_hits, (s_off)*4+(curoffset&3), hit_info, wordlen-8);
						
			}
		}
										
	}	
	
	
exit:

	return 0;
	
}

static int FindNTMatchList_StepAny(BlastSearchBlkPtr search, LookupTablePtr lookup, Uint4 stepsize)
{
	ModLAEntry     					*mod_lt;
	Uint1Ptr 						s, s_end;
	Int4							mask;
	Int4            				char_size;
	register Int4   				num_hits;
	Uint1Ptr						subject0;
	Int4    		   		     	s_off;
	register ModLookupPositionPtr 	lookup_pos;
	ModLookupPosition		 		hit_info;
	Int4							outerindex;	
		
	Int4							stepcount;	
	
	Uint4							curoffset;
	Uint4							nextoffset;

	Uint4            				lookup_index;
	Uint4            				next_index;
	
	Int4							wordlen;

	unsigned long					dstinfo;
				
	// 2 vectors, 1 block
	dstinfo =	((2) << 24) | (1 << 16) | 0;
	
	wordlen 		= search->wfp_second->wordsize;
	
	mod_lt 			= lookup->mod_lt;
	s	 			= (Uint1Ptr)search->subject->sequence;
	s_end			= s + (search->subject->length) / READDB_COMPRESSION_RATIO;
	mask 			= lookup->mask;
	char_size		= lookup->char_size;
	subject0		= s;
	
	// stepcount = length - 8 / stepsize
	stepcount		= (((s_end-s) * 4)-8) / stepsize;
//	stepcount		= (((s_end-s) * 4)) / stepsize;

	if (stepcount < 1) return 0;

	outerindex = 0;

	nextoffset = outerindex*stepsize;
	
	s = subject0 + (nextoffset / 4);
	
	next_index = (*s) << 16 | ((*(s+1)) << 8) | ((*(s+2)) << 0);
	
	next_index >>= (8-(2*(nextoffset & 3)));
							
	next_index &= 0xffff;
	
	for (outerindex=1; outerindex<=stepcount; outerindex++) {

		vec_dst(s+(stepsize/4), dstinfo, 1);
		
		curoffset 		= nextoffset;
		lookup_index 	= next_index;

		nextoffset = outerindex*stepsize;
		
		s = subject0 + (nextoffset / 4);
		
		next_index = (*s) << 16 | ((*(s+1)) << 8) | ((*(s+2)) << 0);
		
		next_index >>= (8-(2*(nextoffset & 3)));
								
		next_index &= 0xffff;
		
		vec_dst((Uint1Ptr)&mod_lt[next_index], dstinfo, 0);
				
		s_off = curoffset/4;
		
		num_hits = mod_lt[lookup_index].num_used;
	
		if (!num_hits) continue;	

		lookup_pos = mod_lt[lookup_index].entries;

		hit_info = *((Uint4 *) lookup_pos);
			
		if (num_hits <= 3) {

			scalar_expandword(search, lookup, lookup_pos+1, num_hits, (s_off)*4+(curoffset&3), hit_info, wordlen-8);
         
		} else {

			lookup_pos++;
			lookup_pos = *((ModLookupPositionPtr PNTR) lookup_pos);			

			scalar_expandword(search, lookup, lookup_pos, num_hits, (s_off)*4+(curoffset&3), hit_info, wordlen-8);
					
		}
								
	}	
	
	
exit:

	return 0;
	
}




/*
	search_nt_orig -- an adaptation of the original search_nt() function
	of BLASTN
*/
static Int4
BlastNtWordFinder(BlastSearchBlkPtr search, LookupTablePtr lookup)
{
	BLASTContextStructPtr search_context;
	register Uint1Ptr s, s_end;
	Uint1Ptr q, q_end, subject0, query0;
	Uint1		p, packed_query, p_start;
	BLAST_Diag	diag, diag_tmp, real_diag;
	BLAST_ExtendWordPtr     ewp;
	BLAST_ExtendWordParamsPtr     ewp_params;
	BLAST_WordFinderPtr	wfp;
	CfjModStruct *combo_array;
        Int4  lookup_index, mask;
        Int4  char_size, index=0, query_length=0;

	register ModLookupPosition hit_info;

	register PV_ARRAY_TYPE PNTR next_pv_array_addr;
	register PV_ARRAY_TYPE next_pv_val,pv_val;
	register ModLookupPositionPtr lookup_pos;
	register Int4 num_hits;
	register Int4 next_nhits;
	Int4 * next_nhits_addr;
	

        Int2            left, right;
        Int4            q_off, s_off, offset, virtual_wordsize, wordsize, compressed_wordsize, compression_factor;
	Int4	extra_bytes, extra_bytes_needed, my_index;
        Int4 bits_to_shift, min_diag_length, min_diag_mask;
	int next_lindex;
	PV_ARRAY_TYPE *pv_array = lookup->pv_array;
	ModLAEntry *mod_lt=lookup->mod_lt;

	query0 = NULL;	/* Gets rid of a warning. */
	p = 255;	/* Gets rid of a warning. */
	ewp_params=search->ewp_params;

	wfp = search->wfp_second;
        char_size = lookup->char_size;
        mask = lookup->mask;
        offset = ewp_params->offset;
        subject0 = s = (Uint1Ptr) search->subject->sequence;
        min_diag_length = ewp_params->min_diag_length;
        bits_to_shift = ewp_params->bits_to_shift;
        min_diag_mask = ewp_params->min_diag_mask;
	

        if (search->current_hitlist == NULL)
        {
                search->current_hitlist = BlastHitListNew(search);
        }
        else
        { /* Scrub the hitlist. */
                if (search->current_hitlist_purge)
                        BlastHitListPurge(search->current_hitlist);
        }
	
	compressed_wordsize = lookup->wordsize;
	wordsize = wfp->wordsize;
	extra_bytes = lookup->wordsize - lookup->reduced_wordsize;

/* The subject sequence is too short, exit this function now. */
	if (wordsize > search->subject->length)
		goto NormalReturn;

	s = lookup_find_init(lookup, &index, s);
        lookup_index = index;

#if BLAST_ALTIVEC
	if (wordsize < 11 || wordsize > 11) {
		Int4	lookupSizeNucleotides;
		Int4	stepsize;

		stepsize = (wordsize - 8) + 1 ;
		
		if (wordsize <= 8) {
			stepsize = 1;
			
			if (FindNTMatchList_toextend(search, lookup, stepsize, wordsize)) {
				goto ErrorReturn;
			}
		} else {
			if (pv_array) {
				
				if (FindNTMatchList_Bitmask(search, lookup, stepsize, pv_array)) {
					goto ErrorReturn;
				}
					
			} else {
			
			if (FindNTMatchList_StepAny(search, lookup, stepsize)) {
				goto ErrorReturn;
			}
		
			}
		}
		
		goto NormalReturn;
		
	}
#endif // BLAST_ALTIVEC

/* Determines when to stop scanning the database; does not include remainder. */
        s_end = subject0 + (search->subject->length)/READDB_COMPRESSION_RATIO;
	compression_factor = wfp->compression_ratio*lookup->reduced_wordsize;
	virtual_wordsize = wordsize - READDB_COMPRESSION_RATIO*compressed_wordsize;
	search_context = search->context;
	query_length = search_context[search->first_context].query->length;
        extra_bytes_needed = extra_bytes;
	query0 = search_context[search->first_context].query->sequence;
	q_end = query0 + query_length;
	ewp = search_context[search->first_context].ewp;
	combo_array = ewp->combo_array;

        if (extra_bytes_needed) {

	/** The first for() loop is optimized for sparse tables (which rarely hit), the second for dense */
	if(pv_array){
	  /* We use the pv_array here, since (on short-med queries) most lookups fail */

	  next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
	  next_pv_val = pv_array[next_lindex>>PV_ARRAY_BTS];

	  for (;;) {
	     do {
		/* lookup a contiguous word. */
		s++;
		lookup_index = next_lindex;
		
		if (s == s_end)
		   goto NormalReturn;
		
		next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
		next_pv_array_addr = &pv_array[next_lindex>>PV_ARRAY_BTS];
		pv_val = next_pv_val;
		next_pv_val = *next_pv_array_addr;
		
	     }while ((pv_val&(((PV_ARRAY_TYPE) 1)<<(lookup_index&PV_ARRAY_MASK))) == 0); 
	     
		num_hits = mod_lt[lookup_index].num_used;
		s_off = s-subject0+1;
		diag_tmp = s_off*READDB_COMPRESSION_RATIO + min_diag_length;
		lookup_pos = mod_lt[lookup_index].entries;
		hit_info = *((Uint4 *) lookup_pos);		  
		lookup_pos++;

		if(num_hits>3){
		  lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
		}

		p_start = *((Uint1Ptr) search->subject->sequence + s_off);
		/* Extend each hit in the linked list */
		do {
		        q_off = hit_info;
			num_hits--;
			hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
			lookup_pos++;

		    	diag = diag_tmp - q_off;

			/* Check for extra bytes if required for longer words. */
				/* extend to the right */
				p = p_start;
				q = query0 + q_off;
				my_index=0;
				while (extra_bytes_needed)
				{
				/* Note: no check is done that q[0-3] is not an ambiguity code.  Could be done, but might slow things down. */
				   packed_query = (q[0]<<6) + (q[1]<<4) + (q[2]<<2) + q[3]; 
					if (p != packed_query)
						break;
					q += 4;
					extra_bytes_needed--;
					my_index++;
					p = *((Uint1Ptr) search->subject->sequence + s_off + my_index);
				}
				if (extra_bytes_needed)
				{ /* extra_bytes_needed next round. */
					extra_bytes_needed = extra_bytes;
					continue; /* not enough bytes found. */
				}
				extra_bytes_needed = extra_bytes;


			q = query0 + q_off - compression_factor;
			if (s_off > compressed_wordsize)
				p = *(subject0 + s_off - compressed_wordsize - 1);

			/* extend to the left */
			if (s_off == compressed_wordsize || READDB_UNPACK_BASE_4(p) != *--q || q < query0)
			{
				left = 0;
			}
			else
			{
				if (READDB_UNPACK_BASE_3(p) != *--q || q < query0)
				{
					left = 1;
				}
				else
				{
					if (READDB_UNPACK_BASE_2(p) != *--q || q < query0)
					{
						left = 2;
					}
					else
					{
						if (READDB_UNPACK_BASE_1(p) != *--q || q < query0)
						{
							left = 3;
						}
						else
						{
							left = 4;
						}
					}
				}
			}
			/* extend to the right */
			p = *(subject0 + s_off + extra_bytes_needed);
			q = query0 + q_off + 4*extra_bytes_needed;
			if (s+extra_bytes_needed >= s_end || READDB_UNPACK_BASE_1(p) != *q++ || q >= q_end)
			{
				right = 0;
			}
			else
			{
				if (READDB_UNPACK_BASE_2(p) != *q++ || q >= q_end)
				{
					right = 1;
				}
				else
				{
					if (READDB_UNPACK_BASE_3(p) != *q++ || q >= q_end)
					{
						right = 2;
					}
					else
					{
						if (READDB_UNPACK_BASE_4(p) != *q++ || q >= q_end)
						{
							right = 3;
						}
						else
						{
							right = 4;
						}
					}
				}
			}
			if (left + right >= virtual_wordsize)
			{
				/* Check if this diagonal has already been explored. */
		    		real_diag = diag & min_diag_mask;
				if (combo_array[real_diag].diag_level >= (s_off*READDB_COMPRESSION_RATIO+offset))
		    		{
					continue;
		    		}
#ifdef BLAST_COLLECT_STATS
				search->second_pass_hits++;
#endif
				if (BlastNtWordExtend(search, q_off, s_off, real_diag, search->first_context) != 0)
					goto ErrorReturn;
			}
		} while (num_hits>0);
	  } /* end for(;;) */
	}else{
	  /* Dense version - doesn't use pv_array */
	    next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
	    next_nhits_addr=&mod_lt[next_lindex].num_used ;
	    next_nhits=*next_nhits_addr;
	    
	    for (;;) {
	        do {
			/* lookup a contiguous word. */
			lookup_index = next_lindex;
			s++; 
 
			if (s == s_end) goto NormalReturn;

        		next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);

			next_nhits_addr = &mod_lt[next_lindex].num_used;

			num_hits = next_nhits;
			next_nhits=*next_nhits_addr;
		} while (num_hits == 0);  

		lookup_pos = mod_lt[lookup_index].entries;

		s_off = s-subject0+1;
		diag_tmp = s_off*READDB_COMPRESSION_RATIO + min_diag_length;
		hit_info = *((Uint4 *) lookup_pos);		  
		lookup_pos++;


		if(num_hits>3){
		  lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
		}

		p_start = *((Uint1Ptr) search->subject->sequence + s_off);
		/* Extend each hit in the linked list */
		do {
		        q_off = hit_info;
			num_hits--;
			hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
			lookup_pos++;

		    	diag = diag_tmp - q_off;


			/* Check for extra bytes if required for longer words. */
				/* extend to the right */
				p = p_start;
				q = query0 + q_off;
				my_index=0;
				while (extra_bytes_needed)
				{
				/* Note: no check is done that q[0-3] is not an ambiguity code.  Could be done, but might slow things down. */
				   packed_query = (q[0]<<6) + (q[1]<<4) + (q[2]<<2) + q[3];
					if (p != packed_query)
						break;
					q += 4;
					extra_bytes_needed--;
					my_index++;
					p = *((Uint1Ptr) search->subject->sequence + s_off + my_index);
				}
				if (extra_bytes_needed)
				{ /* extra_bytes_needed next round. */
					extra_bytes_needed = extra_bytes;
					continue; /* not enough bytes found. */
				}
				extra_bytes_needed = extra_bytes;

			q = query0 + q_off - compression_factor;
			if (s_off > compressed_wordsize)
				p = *(subject0 + s_off - compressed_wordsize - 1);

			/* extend to the left */
			if (s_off == compressed_wordsize || READDB_UNPACK_BASE_4(p) != *--q || q < query0)
			{
				left = 0;
			}
			else
			{
				if (READDB_UNPACK_BASE_3(p) != *--q || q < query0)
				{
					left = 1;
				}
				else
				{
					if (READDB_UNPACK_BASE_2(p) != *--q || q < query0)
					{
						left = 2;
					}
					else
					{
						if (READDB_UNPACK_BASE_1(p) != *--q || q < query0)
						{
							left = 3;
						}
						else
						{
							left = 4;
						}
					}
				}
			}
			/* extend to the right */
			p = *(subject0 + s_off + extra_bytes_needed);
			q = query0 + q_off + 4*extra_bytes_needed;
			if (s+extra_bytes_needed >= s_end || READDB_UNPACK_BASE_1(p) != *q++ || q >= q_end)
			{
				right = 0;
			}
			else
			{
				if (READDB_UNPACK_BASE_2(p) != *q++ || q >= q_end)
				{
					right = 1;
				}
				else
				{
					if (READDB_UNPACK_BASE_3(p) != *q++ || q >= q_end)
					{
						right = 2;
					}
					else
					{
						if (READDB_UNPACK_BASE_4(p) != *q++ || q >= q_end)
						{
							right = 3;
						}
						else
						{
							right = 4;
						}
					}
				}
			}
			if (left + right >= virtual_wordsize)
			{
				/* Check if this diagonal has already been explored. */
		    		real_diag = diag & min_diag_mask;
				if (combo_array[real_diag].diag_level >= (s_off*READDB_COMPRESSION_RATIO+offset))
		    		{
					continue;
		    		}
#ifdef BLAST_COLLECT_STATS
				search->second_pass_hits++;
#endif
				if (BlastNtWordExtend(search, q_off, s_off, real_diag, search->first_context) != 0)
					goto ErrorReturn;
			}
		} while (num_hits>0);
	    }
         }
	}
	else /* extra_bytes not needed. */
	{
	/** The first for() loop is optimized for sparse tables (which rarely hit), the second for dense */
	if(pv_array){
	  /* We use the pv_array here, since (on short-med queries) most lookups fail */
	   next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
	   next_pv_val = pv_array[next_lindex>>PV_ARRAY_BTS];

	   for (;;) {
	      do {
		 /* lookup a contiguous word. */
		 s++;
		 lookup_index = next_lindex;
		 
		 if (s == s_end)
		    goto NormalReturn;
		 
		 next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
		 next_pv_array_addr = &pv_array[next_lindex>>PV_ARRAY_BTS];
		 pv_val = next_pv_val;
		 next_pv_val = *next_pv_array_addr;
		 
	      } while ((pv_val&(((PV_ARRAY_TYPE) 1)<<(lookup_index&PV_ARRAY_MASK))) == 0); 
	     
		num_hits = mod_lt[lookup_index].num_used;
		s_off = s-subject0+1;
		diag_tmp = s_off*READDB_COMPRESSION_RATIO + min_diag_length;
		lookup_pos = mod_lt[lookup_index].entries;
		hit_info = *((Uint4 *) lookup_pos);		  
		lookup_pos++;

		if(num_hits>3){
		  lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
		}
		
		/* Extend each hit in the linked list */
		do {
		        q_off = hit_info;
			num_hits--;
			hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
			lookup_pos++;


			q = query0 + q_off - compression_factor;

			if (s_off > compressed_wordsize)
				p = *(subject0 + s_off - compressed_wordsize - 1);

		    	diag = diag_tmp - q_off;

			/* extend to the left */
			if (s_off == compressed_wordsize || READDB_UNPACK_BASE_4(p) != *--q || q < query0)
			{
				left = 0;
			}
			else
			{
				if (READDB_UNPACK_BASE_3(p) != *--q || q < query0)
				{
					left = 1;
				}
				else
				{
					if (READDB_UNPACK_BASE_2(p) != *--q || q < query0)
					{
						left = 2;
					}
					else
					{
						if (READDB_UNPACK_BASE_1(p) != *--q || q < query0)
						{
							left = 3;
						}
						else
						{
							left = 4;
						}
					}
				}
			}
			/* extend to the right */
			p = *(subject0 + s_off);
			q = query0 + q_off;
			if (s >= s_end || READDB_UNPACK_BASE_1(p) != *q++ || q >= q_end)
			{
				right = 0;
			}
			else
			{
				if (READDB_UNPACK_BASE_2(p) != *q++ || q >= q_end)
				{
					right = 1;
				}
				else
				{
					if (READDB_UNPACK_BASE_3(p) != *q++ || q >= q_end)
					{
						right = 2;
					}
					else
					{
						if (READDB_UNPACK_BASE_4(p) != *q++ || q >= q_end)
						{
							right = 3;
						}
						else
						{
							right = 4;
						}
					}
				}
			}
			if (left + right >= virtual_wordsize)
			{
				/* Check if this diagonal has already been explored. */
		    		real_diag = diag & min_diag_mask;
				if (combo_array[real_diag].diag_level >= (s_off*READDB_COMPRESSION_RATIO+offset))
		    		{
					continue;
		    		}
#ifdef BLAST_COLLECT_STATS
				search->second_pass_hits++;
#endif
				if (BlastNtWordExtend(search, q_off, s_off, real_diag, search->first_context) != 0)
					goto ErrorReturn;
			}
		} while (num_hits>0);
	  } /* end for(;;) */

	}else{
	  /* Dense version - doesn't use pv_array */
	    next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);
	    next_nhits_addr=&mod_lt[next_lindex].num_used ;
	    next_nhits=*next_nhits_addr;
	    
	    for (;;) {
	        do {
			/* lookup a contiguous word. */
			lookup_index = next_lindex;
			s++; 
 
			if (s == s_end) goto NormalReturn;

        		next_lindex = (((lookup_index) & mask)<<char_size) + *(s+1);

			next_nhits_addr = &mod_lt[next_lindex].num_used;

			num_hits = next_nhits;
			next_nhits=*next_nhits_addr;
		} while (num_hits == 0);  

		lookup_pos = mod_lt[lookup_index].entries;

		s_off = s-subject0+1;
		diag_tmp = s_off*READDB_COMPRESSION_RATIO + min_diag_length;
		hit_info = *((Uint4 *) lookup_pos);		  
		lookup_pos++;


		if(num_hits>3){
		  lookup_pos=*((ModLookupPositionPtr PNTR) lookup_pos);
		}

		/* Extend each hit in the linked list */
		do {
		        q_off = hit_info;
			num_hits--;
			hit_info = *((Uint4 *) lookup_pos); /* load next hit_info */
			lookup_pos++;


			q = query0 + q_off - compression_factor;

			if (s_off > compressed_wordsize)
				p = *(subject0 + s_off - compressed_wordsize - 1);

		    	diag = diag_tmp - q_off;

			/* extend to the left */
			if (s_off == compressed_wordsize || READDB_UNPACK_BASE_4(p) != *--q || q < query0)
			{
				left = 0;
			}
			else
			{
				if (READDB_UNPACK_BASE_3(p) != *--q || q < query0)
				{
					left = 1;
				}
				else
				{
					if (READDB_UNPACK_BASE_2(p) != *--q || q < query0)
					{
						left = 2;
					}
					else
					{
						if (READDB_UNPACK_BASE_1(p) != *--q || q < query0)
						{
							left = 3;
						}
						else
						{
							left = 4;
						}
					}
				}
			}
			/* extend to the right */
			p = *(subject0 + s_off);
			q = query0 + q_off;
			if (s >= s_end || READDB_UNPACK_BASE_1(p) != *q++ || q >= q_end)
			{
				right = 0;
			}
			else
			{
				if (READDB_UNPACK_BASE_2(p) != *q++ || q >= q_end)
				{
					right = 1;
				}
				else
				{
					if (READDB_UNPACK_BASE_3(p) != *q++ || q >= q_end)
					{
						right = 2;
					}
					else
					{
						if (READDB_UNPACK_BASE_4(p) != *q++ || q >= q_end)
						{
							right = 3;
						}
						else
						{
							right = 4;
						}
					}
				}
			}
			if (left + right >= virtual_wordsize)
			{
				/* Check if this diagonal has already been explored. */
		    		real_diag = diag & min_diag_mask;
				if (combo_array[real_diag].diag_level >= (s_off*READDB_COMPRESSION_RATIO+offset))
		    		{
					continue;
		    		}
#ifdef BLAST_COLLECT_STATS
				search->second_pass_hits++;
#endif
				if (BlastNtWordExtend(search, q_off, s_off, real_diag, search->first_context) != 0)
					goto ErrorReturn;
			}
		} while (num_hits>0);
	    }
	  }
	}

NormalReturn:
	BlastExtendWordExit(search);
        return search->current_hitlist->hspcnt;

ErrorReturn:
	BlastExtendWordExit(search);
	return 3;
}

static Int4
BlastPurgeResultList(BLASTResultHitlistPtr PNTR results, Int4 hitlist_count)
{
	Int4 index, index_new;

	for (index=0; index<hitlist_count; index++) 
	{
	   if (results[index]->num_ref <= 0) {
	      if (results[index]->seqalign)
		 SeqAlignSetFree(results[index]->seqalign);
	      results[index] = BLASTResultHitlistFree(results[index]);
	   }
	}

	index_new=0;
	for (index=0; index < hitlist_count; index++) 
	{
		if (results[index] != NULL)
		{
			results[index_new] = results[index];
			index_new++;
		}
	}
  	for (index=index_new; index<hitlist_count; index++)
    		results[index] = NULL;

	return index_new;
}

int LIBCALLBACK RPSResultHspScoreCmp(VoidPtr v1, VoidPtr v2)
{
    BLASTResultHspPtr h1, h2;
    
    h1 = (BLASTResultHspPtr) v1;
    h2 = (BLASTResultHspPtr) v2;
    
    if (h1->score < h2->score)
        return 1;
    else if (h1->score > h2->score)
        return -1;
    else return 0;
}
/*
	Move the "current_hitlist" to the BLASTResultHitlistPtr
	result_hitlist.  This function should be called after a 
	subject sequence has been thoroughly investigated.
	If a hitlist is not significant, it will be deleted.  Note that
	the actual sequence is not saved.  This can be retrieved later
	with readdb when the formatting is done.

	The number of significant HSP's is returned.
*/

Int4 LIBCALL
BlastSaveCurrentHitlist(BlastSearchBlkPtr search)
{
	BLASTResultHitlistPtr result_hitlist, PNTR results;
	BLASTResultsStructPtr result_struct;
	BLAST_HitListPtr current_hitlist;
	BLAST_HSPPtr hsp; 
	BLAST_KarlinBlkPtr kbp;
	BLASTResultHspPtr hsp_array;
	Int4 hspcnt, index, index1, new_index, old_index, low_index, high_index;
	Int4 hitlist_count, hitlist_max, hspmax, hspset_cnt, high_score=0, retval;
	Nlm_FloatHi current_evalue=DBL_MAX;
	Int2 deleted;
	Int4 query_length;
	SeqIdPtr subject_id=NULL;

	if (search == NULL)
		return 0;	

	if (search->current_hitlist == NULL || search->current_hitlist->hspcnt <= 0)	/* No hits to save. */
	{
		search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
		return 0;
	}

	current_hitlist = search->current_hitlist;

	retval = current_hitlist->hspcnt;

	if (search->pbp->gapped_calculation &&
		search->prog_number != blast_type_blastn)
		kbp = search->sbp->kbp_gap[search->first_context];
	else
		kbp = search->sbp->kbp[search->first_context];
	
	if (search->prog_number==blast_type_blastn)
	      query_length = search->query_context_offsets[search->first_context+1] - 1;
	result_hitlist = BLASTResultHitlistNew(current_hitlist->hspcnt);
	if (result_hitlist != NULL)
	{
		result_hitlist->subject_id = search->subject_id;
		result_hitlist->subject_info = search->subject_info;
		search->subject_info = NULL;

		hspcnt = result_hitlist->hspcnt;
		hsp_array = result_hitlist->hsp_array;
		index1 = 0;
		hspmax = current_hitlist->hspcnt_max;

		hsp = current_hitlist->hsp_array[0];
		hspset_cnt = -1;

		for (index=0; index<hspcnt; index++)
		{
			while (hsp == NULL && index1 < hspmax)
			{
				index1++;
				hsp = current_hitlist->hsp_array[index1];
			}
			if (index1==hspmax) break;
			if (current_evalue > hsp->evalue)
				current_evalue = hsp->evalue;
			if (high_score < hsp->score)
				high_score = hsp->score;
			hsp_array[index].ordering_method = hsp->ordering_method;
			hsp_array[index].number = hsp->num;
			hsp_array[index].score = hsp->score;
			hsp_array[index].e_value = hsp->evalue;
			hsp_array[index].p_value = hsp->pvalue;
			hsp_array[index].bit_score = ((hsp->score*kbp->Lambda) -
						      kbp->logK)/NCBIMATH_LN2;
			if (search->prog_number==blast_type_blastn) {
                           if (search->last_context > 0 && 
                               hsp->query.offset >=
                               search->query_context_offsets[search->last_context]) {
                              hsp->context = 1;
                              hsp->query.offset -=
                                 search->query_context_offsets[hsp->context];
                              hsp->query.gapped_start -=
                                 search->query_context_offsets[hsp->context];
                           }

			   if (hsp->context & 1) 
			      hsp_array[index].query_frame = -1;
			   else 
			      hsp_array[index].query_frame = 1;
			   hsp_array[index].query_gapped_start = hsp->query.gapped_start;
			   hsp_array[index].subject_gapped_start = hsp->subject.gapped_start;
			} else {
			   hsp_array[index].query_frame = hsp->query.frame;
			   hsp_array[index].query_gapped_start = hsp->query.gapped_start;
			   hsp_array[index].subject_gapped_start =
			      hsp->subject.gapped_start;
			}
			hsp_array[index].context = hsp->context; 
			hsp_array[index].query_offset = hsp->query.offset;
			hsp_array[index].query_length = hsp->query.length;
			hsp_array[index].subject_offset = hsp->subject.offset;
			hsp_array[index].subject_length = hsp->subject.length;
			hsp_array[index].subject_frame = hsp->subject.frame;;
			hsp_array[index].point_back = result_hitlist;

			if (hsp->start_of_chain)
			{	/* starting new set of HSP's, incr count.*/
				hspset_cnt++;
			}
			hsp_array[index].hspset_cnt = hspset_cnt;

			index1++;
			if (index1 >= hspmax)
				break;
			hsp = current_hitlist->hsp_array[index1];
		}
		/* Check if there were less HSPs than expected */
		result_hitlist->hspcnt = index1;
		result_hitlist->best_evalue = current_evalue;
		result_hitlist->high_score = high_score;
	}

/* For MP BLAST we check that no other thread is attempting to insert results. */
	if (search->thr_info->results_mutex)
            NlmMutexLock(search->thr_info->results_mutex);

/* This is the structure that is identical on every thread. */
	result_struct = search->result_struct;
	hitlist_count = result_struct->hitlist_count;
	hitlist_max = result_struct->hitlist_max;
	results = result_struct->results;

	/* Record the worst evalue for ReevaluateWithAmbiguities. */
	if (hitlist_count == hitlist_max)
	{
		search->worst_evalue = results[hitlist_count-1]->best_evalue;
	}

        /* New hit is less significant than all the other hits. */
        if (hitlist_count > 0 && (current_evalue > results[hitlist_count-1]->best_evalue ||
        	(current_evalue >= results[hitlist_count-1]->best_evalue &&
                 high_score < results[hitlist_count-1]->high_score)))
        {
                if (hitlist_count == hitlist_max)
                {       /* Array is full, delete the entry. */
                        search->current_hitlist =
			   BlastHitListDestruct(search->current_hitlist);
                        result_hitlist = BLASTResultHitlistFreeEx(search, result_hitlist);
                        if (search->thr_info->results_mutex)
                            NlmMutexUnlock(search->thr_info->results_mutex); /* Free mutex. */
                        return 0;
                }
                else
                {       /* Add to end of array. */
	    		deleted = BlastInsertList2Heap(search, result_hitlist);
                }

	        if (deleted == 1) 
	        	hitlist_count = result_struct->hitlist_count =
			   BlastPurgeResultList(results, hitlist_count);
	    	else if (deleted == 0) 
		{
	      		result_hitlist = BLASTResultHitlistFreeEx(search, result_hitlist);
	      		if (search->thr_info->results_mutex)
                            NlmMutexUnlock(search->thr_info->results_mutex);	/* Free mutex. */
	      		return retval;
		}
                new_index = hitlist_count;
        }
        else
        {
	  if (hitlist_count != 0)		/* The array is all NULL's if hitlist_count==0 */
	  {
	    deleted = BlastInsertList2Heap(search, result_hitlist);
	    if (deleted == 1) 
	      hitlist_count = result_struct->hitlist_count =
		 BlastPurgeResultList(results, hitlist_count);
	    else if (deleted == 0) {
	      result_hitlist = BLASTResultHitlistFreeEx(search, result_hitlist);
	      if (search->thr_info->results_mutex)
		NlmMutexUnlock(search->thr_info->results_mutex);	/* Free mutex. */
	      return retval;
	    }
	    if (hitlist_count > 0)
	    {
	  	  high_index=0;
		  low_index=hitlist_count-1;
		  new_index = (high_index+low_index)/2;
		  old_index = new_index;
		  for (index=0; index<BLAST_SAVE_ITER_MAX; index++)
		  {
			if (results[new_index]->best_evalue > current_evalue)
			{
			    low_index = new_index;
			}
			else if (results[new_index]->best_evalue < current_evalue)
			{
			    high_index = new_index;
			}
			else
			{ /* If e-values are the same, use high score. */
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
			if (old_index == new_index)
			{
			    if (results[new_index]->best_evalue < current_evalue)
			    { /* Perform this check as new_index get rounded DOWN above.*/
				new_index++;
			    } 
			    else if (results[new_index]->best_evalue == current_evalue && results[new_index]->high_score > high_score)
			    {
				new_index++;
			    }
			    break;
                        }
			old_index = new_index;
                    }
		    if (hitlist_count == hitlist_max)
		    {	/* The list is full, delete the last entry. */
			BlastFreeHeap(search, results[hitlist_max-1]);
			if (results[hitlist_max-1]->seqalign)
			   SeqAlignSetFree(results[hitlist_max-1]->seqalign);
			results[hitlist_max-1] = BLASTResultHitlistFreeEx(search, results[hitlist_max-1]);
			result_struct->hitlist_count--;	
			hitlist_count = result_struct->hitlist_count;	
		    }
		    if (hitlist_max > 1)
		    	Nlm_MemMove((results+new_index+1), (results+new_index), (hitlist_count-new_index)*sizeof(results[0]));
	    }
	    else
	    {  /* Case of K=1 and the first hit is eliminated */
	    	new_index = 0;
	    	BlastInsertList2Heap(search, result_hitlist);
	    }
	  }
	else
	  {	/* First hit to be stored. */
	    new_index = 0;
	    BlastInsertList2Heap(search, result_hitlist);
	  }
	}
	
	if (new_index < hitlist_max)
        {
		results[new_index] = result_hitlist;
		result_struct->hitlist_count++;	
	}

        /* We need to sort all hits by score/e_value in results[new_index] */


        HeapSort(results[new_index]->hsp_array, results[new_index]->hspcnt, 
                 sizeof(BLASTResultHsp), RPSResultHspScoreCmp);

        /* --------------------------------------------------------------- */

	if (search->thr_info->results_mutex)
            NlmMutexUnlock(search->thr_info->results_mutex);	/* Free mutex. */
	return retval;
}
	
Int2
blast_set_parameters(BlastSearchBlkPtr search, 
	Int4 dropoff_number_of_bits_1st_pass,
	Int4 dropoff_number_of_bits_2nd_pass,
	Nlm_FloatHi	avglen, /* Average length of a sequence. */
	Nlm_FloatHi	searchsp, /* total search space. */
	Int4 window) /* length where two hits must be found to count. */
{
	BLAST_ExtendWordPtr	ewp;
	BLAST_KarlinBlkPtr	kbp, kbp_gap;
	BLAST_ParameterBlkPtr	pbp; 
	BLAST_ScoreBlkPtr	sbp;
	BLAST_Score	s, s2;
	BLAST_Score	dropoff_1st_pass, dropoff_2nd_pass;
	Int2 index;
	
	Nlm_FloatHi meff, e, e2;

	if (search == NULL)
		return 1;

	pbp = search->pbp;
	if (pbp == NULL)
		return 1;

	sbp = search->sbp;
	if (sbp == NULL)
		return 1;

	/* Do for first context only, should this be changed?? */
	kbp_gap = sbp->kbp_gap[search->first_context];
	kbp = sbp->kbp[search->first_context];
	if (kbp == NULL && kbp_gap == NULL)
		return 1;

	for (index=search->first_context; index<=search->last_context; index++)
	{
		ewp = search->context[index].ewp;
		if (ewp == NULL && !pbp->mb_params)
			return 1;

	}

	s = pbp->cutoff_s;
	e = pbp->cutoff_e;
	s2 = pbp->cutoff_s2;
	e2 = pbp->cutoff_e2;
	if (pbp->cutoff_s_set && !pbp->cutoff_e_set)
		e = 0.;

	meff = (Nlm_FloatHi) search->context[search->first_context].query->length;
	if (pbp->mb_params)
	   BlastCutoffs_simple(&s, &e, kbp, searchsp, TRUE);
	else
	{
	   if (pbp->gapped_calculation && search->prog_number != blast_type_blastn)
	   	BlastCutoffs_simple(&s, &e, kbp_gap, searchsp, FALSE);
	   else
	   	BlastCutoffs_simple(&s, &e, kbp, searchsp, FALSE);
	}
	/* Determine the secondary cutoff score, S2, to use */
	if (e2 == 0. && !pbp->cutoff_s2_set)
		s2 = s;

	if ((pbp->cutoff_e2_set && !pbp->cutoff_s2_set && e2 == 0.) || 
		(pbp->cutoff_s2_set && s2 > s))
	{
		e2 = 0., s2 = s;
	}
	else 
	{
		/* e2 = MIN(e, e2); */
		if (pbp->cutoff_s2_set && !pbp->cutoff_e2_set)
			e2 = 0.;
/*
		BlastCutoffs(&s2, &e2, kbp, meff, avglen, TRUE);
*/
		if (pbp->gapped_calculation && search->prog_number != blast_type_blastn)
			BlastCutoffs(&s2, &e2, kbp_gap, MIN(avglen,meff), avglen, TRUE);
		else
			BlastCutoffs(&s2, &e2, kbp, MIN(avglen,meff), avglen, TRUE);
		/* Adjust s2 to be in line with s, as necessary */
		s2 = MAX(s2, 1);
		if (s2 > s)
			s2 = s;
/*
		e2 = BlastKarlinStoE_simple(s2, kbp, searchsp);
*/
	}

	if (pbp->cutoff_s2_set)
		pbp->cutoff_s2_max = s2;
	else
		pbp->cutoff_s2_max = s;
	
	if (pbp->do_sum_stats)
		pbp->cutoff_s1 = s2;
	else
		pbp->cutoff_s1 = s;

	if (pbp->gapped_calculation && search->prog_number != blast_type_blastn)
	{
		pbp->gap_trigger = MIN(pbp->gap_trigger, s2);	
		s2 = MIN(pbp->gap_trigger, s2);
	}
	
	dropoff_1st_pass = (BLAST_Score) ceil((Nlm_FloatHi) dropoff_number_of_bits_1st_pass * NCBIMATH_LN2 / kbp->Lambda);
	dropoff_1st_pass = (BLAST_Score) MIN((Nlm_FloatHi) dropoff_1st_pass, s);
	dropoff_2nd_pass = (BLAST_Score) ceil((Nlm_FloatHi) dropoff_number_of_bits_2nd_pass * NCBIMATH_LN2 / kbp->Lambda);
	dropoff_2nd_pass = (BLAST_Score) MIN((Nlm_FloatHi) dropoff_2nd_pass, s);	
	/* The drop-off parameter MUST be negative. */
	pbp->dropoff_1st_pass = -dropoff_1st_pass;
	pbp->dropoff_2nd_pass = -dropoff_2nd_pass;
	pbp->cutoff_s = s;
	pbp->cutoff_e = e;
	pbp->cutoff_s2 = s2;
	pbp->cutoff_e2 = e2;

/* The first and second pass S2 values are from formula by Stephen Altschul.*/
/* If no bits were specified on the command line, then the following
formula is used:
	calculate ln(25000*query_length*K)/lambda

	and

		21(bits)*ln2/lammbda

Take the smaller of those two formulas.
*/
	if (pbp->number_of_bits == 0.0)
	{
		pbp->cutoff_s_first = (BLAST_Score) MIN(log((Nlm_FloatHi)(25000*(kbp->K)*(search->context[search->first_context].query->length)))/kbp->Lambda, 21*NCBIMATH_LN2/kbp->Lambda);
		/* Adjust the cutoff value for translating searches. */
		pbp->cutoff_s_first += (BLAST_Score) log((Nlm_FloatHi)search->context_factor)/kbp->Lambda;
	}
	else
	{
		pbp->cutoff_s_first = (BLAST_Score) (pbp->number_of_bits*NCBIMATH_LN2 / kbp->Lambda);
	}

/* This value is used only if the "old" statistics are used.  If not an
individual cutoff score is calculated for each subject sequence in 
CalculateSecondCutoffScore. */

	pbp->cutoff_s_second = s2;

	/* If we're just collecting HSP's, use one cutoff. */
	if (!pbp->gapped_calculation && !pbp->do_sum_stats)
	{
		pbp->cutoff_s2 = MAX(pbp->cutoff_s, pbp->cutoff_s2);
		pbp->cutoff_s2_max = MAX(pbp->cutoff_s, pbp->cutoff_s2);
	}

	return 0;
}

/*
	Arrange the HSP's (on every HitList) for linking by "link_hsps".

	link_hsps requires an array of HSP's and the first member of this
	array is used just to hold the HSP's (i.e., not a real HSP).

	Could this all be integrated with link_hsp's?? 
*/
static BLAST_HSPPtr
new_link_hsps(BlastSearchBlkPtr search, BLAST_HitListPtr hitlist, BLAST_HSPPtr PNTR hsp_array);

Int2 LIBCALL
BlastLinkHsps (BlastSearchBlkPtr search)

{
	BLAST_HitListPtr hitlist;
	BLAST_HSPPtr hsp;
	Int4 index;

	hitlist = search->current_hitlist;
	if (hitlist && hitlist->hspcnt > 0)
	{
           /* Link up the HSP's for this hitlist. */
         if ((search->prog_number != blast_type_tblastn &&
                 search->prog_number != blast_type_psitblastn) || 
             search->pbp->longest_intron <= 0)
	  {
              hsp = link_hsps(search, hitlist, hitlist->hsp_array);
              /* The HSP's may be in a different order than they were before, 
                 but hsp contains the first one. */
              for (index=0; index<hitlist->hspcnt; index++) {
                 hitlist->hsp_array[index] = hsp;
                 hsp = hsp->next;
              }
           } else {
              hsp = new_link_hsps(search, hitlist, hitlist->hsp_array);
              for (index=0; index<hitlist->hspcnt; index++) {
                 hitlist->hsp_array[index] = hsp;
                 hsp = hsp->next;
              }
           }
	}

	return 0;
}

/*
	Sort the HSP's by starting position of the query.  Called by HeapSort.  
	The first function sorts in forward, the second in reverse order.
*/

static int LIBCALLBACK
fwd_compare_hsps(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (SIGN(h1->query.frame) != SIGN(h2->query.frame))
	{
		if (h1->query.frame < h2->query.frame)
			return 1;
		else
			return -1;
	}
	if (h1->query.offset < h2->query.offset) 
		return -1;
	if (h1->query.offset > h2->query.offset) 
		return 1;
	/* Necessary in case both HSP's have the same query offset. */
	if (h1->subject.offset < h2->subject.offset) 
		return -1;
	if (h1->subject.offset > h2->subject.offset) 
		return 1;

	return 0;
}

/* Comparison function based on end position in the query */
static int LIBCALLBACK
end_compare_hsps(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (SIGN(h1->query.frame) != SIGN(h2->query.frame))
	{
		if (h1->query.frame < h2->query.frame)
			return 1;
		else
			return -1;
	}
	if (h1->query.end < h2->query.end) 
		return -1;
	if (h1->query.end > h2->query.end) 
		return 1;
	/* Necessary in case both HSP's have the same query end. */
	if (h1->subject.end < h2->subject.end) 
		return -1;
	if (h1->subject.end > h2->subject.end) 
		return 1;

	return 0;
}

static int LIBCALLBACK
rev_compare_hsps(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;
	
	if (SIGN(h1->query.frame) != SIGN(h2->query.frame))
	{
		if (h1->query.frame > h2->query.frame)
			return 1;
		else
			return -1;
	}

	if (h1->query.offset < h2->query.offset) 
		return  1;
	if (h1->query.offset > h2->query.offset) 
		return -1;
	return 0;
}


static int LIBCALLBACK
rev_compare_hsps_cfj(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (SIGN(h1->query.frame) != SIGN(h2->query.frame))
	{
		if (h1->query.frame > h2->query.frame)
			return 1;
		else
			return -1;
	}

	if (SIGN(h1->subject.frame) != SIGN(h2->subject.frame))
	{
		if (h1->subject.frame > h2->subject.frame)
			return 1;
		else
			return -1;
	}

	if (h1->query.offset < h2->query.offset) 
		return  1;
	if (h1->query.offset > h2->query.offset) 
		return -1;
	return 0;
}

/* Find an HSP with offset closest, but not smaller (larger) than a given one. */
   
static Int4 hsp_binary_search(BLAST_HSPPtr PNTR hsp_array, Int4 size, 
                              Int4 offset, Boolean right)
{
   Int4 index, begin, end, coord;
   
   begin = 0;
   end = size;
   while (begin < end) {
      index = (begin + end) / 2;
      if (right) 
         coord = hsp_array[index]->query.offset;
      else
         coord = hsp_array[index]->query.end;
      if (coord >= offset) 
         end = index;
      else
         begin = index + 1;
   }

   return end;
}

#define WINDOW_SIZE 20
static FloatHi SumHSPEvalue(BlastSearchBlkPtr search, BLAST_HSPPtr head_hsp, 
                            BLAST_HSPPtr hsp, BLAST_ScorePtr sumscore)
{
   FloatHi gap_prob, gap_decay_rate, sum_evalue, score_prime;
   Int4 gap_size, num, subject_length;

   gap_prob = search->pbp->gap_prob;
   gap_size = search->pbp->gap_size;
   gap_decay_rate = search->pbp->gap_decay_rate;
   num = head_hsp->num + hsp->num;
   subject_length = 
      MAX((search->subject->length - search->length_adjustment), 1);
   if (!StringCmp(search->prog_name, "tblastn")) 
      subject_length /= 3;
   subject_length = MAX(subject_length, 1);
	

   *sumscore = MAX(hsp->score, hsp->sumscore) + 
      MAX(head_hsp->score, head_hsp->sumscore);
   score_prime = *sumscore * search->sbp->kbp_gap[search->first_context]->Lambda;

   sum_evalue = 
      BlastUnevenGapSumE(search->sbp->kbp_gap[search->first_context],
                         2*WINDOW_SIZE, search->pbp->longest_intron + WINDOW_SIZE, 
                         gap_prob, gap_decay_rate, num,*sumscore, score_prime, 
                         search->context[search->first_context].query->effective_length, 
                         subject_length, FALSE);
   sum_evalue *= (FloatHi) search->dblen / subject_length;
   return sum_evalue;
}

static int LIBCALLBACK
sumscore_compare_hsps(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;
        BLAST_Score score1, score2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (h1 == NULL || h2 == NULL)
		return 0;

        score1 = MAX(h1->sumscore, h1->score);
        score2 = MAX(h2->sumscore, h2->score);

	if (score1 < score2) 
		return 1;
	if (score1 > score2)
		return -1;

	return 0;
}


/* The following function should be used only for new tblastn. 
   Current implementation does not allow its use in two sequences BLAST */

#define MAX_SPLICE_DIST 5
static Boolean
FindSpliceJunction(Uint1Ptr subject_seq, BLAST_HSPPtr hsp1, 
                   BLAST_HSPPtr hsp2)
{
   Boolean found = FALSE;
   Int4 overlap, length, i;
   Uint1Ptr nt_seq;
   SeqMapTablePtr smtp = SeqMapTableFind(Seq_code_ncbi4na, Seq_code_iupacna);
   Uint1 g, t, a;

   g = SeqMapTableConvert(smtp, 'G');
   t = SeqMapTableConvert(smtp, 'T');
   a = SeqMapTableConvert(smtp, 'A');

   overlap = hsp1->query.end - hsp2->query.offset;
   

   if (overlap >= 0) {
      length = 3*overlap + 2;
      nt_seq = &subject_seq[hsp1->subject.end - 3*overlap];
   } else {
      length = MAX_SPLICE_DIST;
      nt_seq = &subject_seq[hsp1->subject.end];
   }

   for (i=0; i<length-1; i++) {
      if (nt_seq[i] == g && nt_seq[i+1] == t) {
         found = TRUE;
         break;
      }
   }

   if (!found) 
      return FALSE;
   else
      found = FALSE;

   if (overlap >= 0) 
      nt_seq = &subject_seq[hsp2->subject.offset - 2];
   else 
      nt_seq = &subject_seq[hsp2->subject.offset - MAX_SPLICE_DIST];

   for (i=0; i<length-1; i++) {
      if (nt_seq[i] == a && nt_seq[i+1] == g) {
         found = TRUE;
         break;
      }
   }
   return found;
}

static BLAST_HSPPtr
new_link_hsps(BlastSearchBlkPtr search, BLAST_HitListPtr hitlist, BLAST_HSPPtr PNTR hsp_array)
{
   BLAST_HSPPtr PNTR score_hsp_array, PNTR offset_hsp_array, PNTR end_hsp_array;
   BLAST_HSPPtr hsp, head_hsp, best_hsp, var_hsp;
   Int4 hspcnt, index, index1, i;
   FloatHi best_evalue, evalue;
   BLAST_Score sumscore, best_sumscore;
   Boolean reverse_link;
   Uint1Ptr subject_seq = NULL;
   Int4 length, buf_len=0; 

   hspcnt = hitlist->hspcnt;

   for (index=0; index<hspcnt; index++) {
      hsp_array[index]->num = 1;
      hsp_array[index]->linked_set = FALSE;
      hsp_array[index]->ordering_method = 3;
   }

   score_hsp_array = (BLAST_HSPPtr PNTR) Malloc(hspcnt*sizeof(BLAST_HSPPtr));
   offset_hsp_array = (BLAST_HSPPtr PNTR) Malloc(hspcnt*sizeof(BLAST_HSPPtr));
   end_hsp_array = (BLAST_HSPPtr PNTR) Malloc(hspcnt*sizeof(BLAST_HSPPtr));

   MemCpy(score_hsp_array, hsp_array, hspcnt*sizeof(BLAST_HSPPtr));
   MemCpy(offset_hsp_array, hsp_array, hspcnt*sizeof(BLAST_HSPPtr));
   MemCpy(end_hsp_array, hsp_array, hspcnt*sizeof(BLAST_HSPPtr));
   HeapSort(offset_hsp_array, hspcnt, sizeof(BLAST_HSPPtr), fwd_compare_hsps);
   HeapSort(end_hsp_array, hspcnt, sizeof(BLAST_HSPPtr), end_compare_hsps);

   HeapSort(score_hsp_array, hspcnt, sizeof(BLAST_HSPPtr), sumscore_compare_hsps);
      
   head_hsp = NULL;
   for (index = 0; index < hspcnt && score_hsp_array[index]; ) {
      if (!head_hsp) {
         while (index<hspcnt && score_hsp_array[index] && 
                score_hsp_array[index]->linked_set)
            index++;
         if (index==hspcnt)
            break;
         head_hsp = score_hsp_array[index];
      }
      best_evalue = head_hsp->evalue;
      best_hsp = NULL;
      reverse_link = FALSE;
      
      if (head_hsp->linked_set)
         for (var_hsp = head_hsp, i=1; i<head_hsp->num; 
              var_hsp = var_hsp->next, i++);
      else
         var_hsp = head_hsp;

      index1 = hsp_binary_search(offset_hsp_array, hspcnt,
                                 var_hsp->query.end - WINDOW_SIZE, TRUE);
      while (index1 < hspcnt && offset_hsp_array[index1]->query.offset < 
             var_hsp->query.end + WINDOW_SIZE) {
         hsp = offset_hsp_array[index1++];
         /* If this is already part of a linked set, disregard it */
         if (hsp == var_hsp || hsp == head_hsp || 
             (hsp->linked_set && !hsp->start_of_chain))
            continue;
         /* Check if the subject coordinates are consistent with query */
         if (hsp->subject.offset < var_hsp->subject.end - WINDOW_SIZE || 
             hsp->subject.offset > var_hsp->subject.end + search->pbp->longest_intron)
            continue;
         /* Check if the e-value for the combined two HSPs is better than for
            one of them */
         if ((evalue = SumHSPEvalue(search, head_hsp, hsp, &sumscore)) < 
             MIN(best_evalue, hsp->evalue)) {
            best_hsp = hsp;
            best_evalue = evalue;
            best_sumscore = sumscore;
         }
      }
      index1 = hsp_binary_search(end_hsp_array, hspcnt,
                                 head_hsp->query.offset - WINDOW_SIZE, FALSE);
      while (index1 < hspcnt && end_hsp_array[index1]->query.end < 
             head_hsp->query.offset + WINDOW_SIZE) {
         hsp = end_hsp_array[index1++];

         /* Check if the subject coordinates are consistent with query */
         if (hsp == head_hsp || 
             hsp->subject.end > head_hsp->subject.offset + WINDOW_SIZE || 
             hsp->subject.end < head_hsp->subject.offset - search->pbp->longest_intron)
            continue;
         if (hsp->linked_set) {
            for (var_hsp = hsp, i=1; var_hsp->start_of_chain == FALSE; 
                 var_hsp = var_hsp->prev, i++);
            if (i<var_hsp->num || var_hsp == head_hsp)
               continue;
         } else
            var_hsp = hsp;
         /* Check if the e-value for the combined two HSPs is better than for
            one of them */
         if ((evalue = SumHSPEvalue(search, var_hsp, head_hsp, &sumscore)) < 
             MIN(var_hsp->evalue, best_evalue)) {
            best_hsp = hsp;
            best_evalue = evalue;
            best_sumscore = sumscore;
            reverse_link = TRUE;
         }
      }
         
      /* Link these HSPs together */
      if (best_hsp != NULL) {
         if (!reverse_link) {
            head_hsp->start_of_chain = TRUE;
            head_hsp->sumscore = best_sumscore;
            head_hsp->evalue = best_evalue;
            best_hsp->start_of_chain = FALSE;
            if (head_hsp->linked_set) 
               for (var_hsp = head_hsp, i=1; i<head_hsp->num; 
                    var_hsp = var_hsp->next, i++);
            else 
               var_hsp = head_hsp;
            var_hsp->next = best_hsp;
            best_hsp->prev = var_hsp;
            head_hsp->num += best_hsp->num;
         } else {
            best_hsp->next = head_hsp;
            head_hsp->prev = best_hsp;
            if (best_hsp->linked_set) {
               for (var_hsp = best_hsp; 
                    var_hsp->start_of_chain == FALSE; 
                    var_hsp = var_hsp->prev);
            } else
                  var_hsp = best_hsp;
            var_hsp->start_of_chain = TRUE;
            var_hsp->sumscore = best_sumscore;
            var_hsp->evalue = best_evalue;
            var_hsp->num += head_hsp->num;
            head_hsp->start_of_chain = FALSE;
         }
         
         head_hsp->linked_set = best_hsp->linked_set = TRUE;
         if (reverse_link)
            head_hsp = var_hsp;
      } else {
         head_hsp = NULL;
         index++;
      }
   }
   
   HeapSort(score_hsp_array, hspcnt, sizeof(BLAST_HSPPtr), sumscore_compare_hsps);
   /* Get the nucleotide subject sequence in Seq_code_ncbi4na */
   if (search->rdfp)
      length = readdb_get_sequence_ex(search->rdfp, search->subject_id,
                                      &subject_seq, &buf_len, FALSE);
   else /* Comes from two sequences BLAST */
      subject_seq = search->subject->sequence;

   hsp = head_hsp = score_hsp_array[0];
   for (index=0; index<hspcnt; hsp = hsp->next) {
      if (hsp->linked_set) {
         index1 = hsp->num;
         for (i=1; i < index1; i++, hsp = hsp->next) {
            hsp->next->evalue = hsp->evalue; 
            hsp->next->num = hsp->num;
            hsp->next->sumscore = hsp->sumscore;
            if (FindSpliceJunction(subject_seq, hsp, hsp->next)) {
               /* Kludge: ordering_method here would indicate existence of
                  splice junctions(s) */
               hsp->ordering_method++;
               hsp->next->ordering_method++;
            } else {
               hsp->ordering_method--;
               hsp->next->ordering_method--;
            }
         }
      } 
      while (++index < hspcnt)
         if (!score_hsp_array[index]->linked_set ||
             score_hsp_array[index]->start_of_chain)
            break;
      if (index == hspcnt) {
         hsp->next = NULL;
         break;
      }
      hsp->next = score_hsp_array[index];
   }

   MemFree(subject_seq);
   MemFree(score_hsp_array);
   MemFree(offset_hsp_array);
   MemFree(end_hsp_array);
   return head_hsp;
}

/*
	This function orders and "links" the HSP's.  It does this
	by first ordering them backwards (with "rev_compare_hsps") and 
	then (as the function moves forwards through the list of HSP's)
	comparing them with the previous HSP's. They then end up
	in the "correct" order.  

	The HSP hp_start is used as a "hook" into the chain of HSP's.  
	As HSP's are assigned to a set, they are removed from the linked 
	list, and further consideration.  hp_start always points to the first 
	"real" HSP remaining.

	Two attempts are made to order the HSP's:
	one has a maximum gap ("gap"), the other has no maximum.

	This function works with the HSP's resulting from one query 
	sequence and one subject sequence.

	******* Comments -cfj
	This function is, in the worst case, O(N^3) in the number of hsps, and often becomes
        the main bottleneck on large query strings.  	
	I've made a bunch of changes to try to speed up this function (by constant factors),
	while still producing the identical output.

	******* major changes -cfj
	- Use separate search (ie. separate frame loops) for each subject frame sign.
	- use lh_helper array to store most commonly used info.  This helps by ordering 
          accesses, and keeping them close.
	- For index=0, we can break out of loop when q_off gets large enough.
	- For index=1, we can break out of loop when remaining maxsum is small enough.
	- Keep track of which hsps are 'linked-to'.  When we pull out the high scoring 
  	  chain, if none of the removed hsp's are linked to by someone outside the change, 
	  then no other hsp's score will change, so we don't need to recompute scores, just 
	  find the new largest.
        - if max-path was unchanged, it is still the max, so don't recompute all scores
	- dynamically keep track of next hsp in chain with a larger score, this allows us to 
	    jump over low scores when walking down list
	- Whan computing the score for each hsp, if the linked_to (from last iter) has not changed, 
	  then this item is unchanged also.
	********
*/

static BLAST_HSPPtr
link_hsps(BlastSearchBlkPtr search, BLAST_HitListPtr hitlist, BLAST_HSPPtr PNTR hsp_array)
{
	BLAST_HSPPtr H, H2, best[2], first_hsp, last_hsp, hp_frame_start[3*2];
	BLAST_HSP hp_start;
	BLAST_KarlinBlkPtr PNTR kbp;
	BLAST_Score maxscore, cutoff[2];
	Boolean frame_change, linked_set, ignore_small_gaps;
	Nlm_FloatHi gap_decay_rate, gap_prob, prob[2];
	Int4 index, index1, ordering_method, num_links, frame_index, number_of_query_frames;
	Int4 hp_frame_number[3*2];
	Int4 gap_size, subject_length, number_of_hsps, total_number_of_hsps;
	VoidPtr link;
	Int4 H2_index,H_index;
	Int4 i;
	Int4 max_q_diff;
 	Int4 path_changed;  /* will be set if an element is removed that may change an existing path */
 	Int4 first_pass, use_current_max; 
	LinkHelpStruct *lh_helper=0;

	if (search == NULL || hitlist == NULL)
		return NULL;

        /* enlarge helper array if necessary */
        if (hitlist->lh_helper_size <= (hitlist->hspcnt+5)){
	  hitlist->lh_helper_size = MAX(1024,hitlist->hspcnt+5);
	  MemFree(hitlist->lh_helper);
	  hitlist->lh_helper = (LinkHelpStruct *) MemNew(sizeof(LinkHelpStruct)*hitlist->lh_helper_size);
	}
	lh_helper= hitlist->lh_helper;

	if (search->pbp->gapped_calculation && search->prog_number != blast_type_blastn)
	{
		kbp = search->sbp->kbp_gap;
	}
	else
	{
		kbp = search->sbp->kbp;
	}

	total_number_of_hsps = hitlist->hspcnt;
	subject_length = MAX((search->subject->length - search->length_adjustment), 1);
	
        if (StringCmp(search->prog_name, "tblastn") == 0
            || StringCmp(search->prog_name, "tblastx") == 0
            ||StringCmp(search->prog_name, "psitblastn") == 0)
	{
		subject_length /= 3;
	}
	subject_length = MAX(subject_length, 1);
	number_of_hsps = total_number_of_hsps;
	gap_size = search->pbp->gap_size;
	gap_prob = search->pbp->gap_prob;
	gap_decay_rate = search->pbp->gap_decay_rate;
/* Sort by (reverse) position. */
	HeapSort(hsp_array,total_number_of_hsps,sizeof(BLAST_HSPPtr), rev_compare_hsps_cfj);

	cutoff[0] = search->pbp->cutoff_s_second;
	cutoff[1] = search->pbp->cutoff_big_gap;
	ignore_small_gaps = search->pbp->ignore_small_gaps;
	
	if (StringICmp(search->prog_name, "blastn") == 0 || StringICmp(search->prog_name, "blastx") == 0 || StringICmp(search->prog_name, "tblastx") == 0)
	{
		number_of_query_frames = 2;
	}
	else
	{
		number_of_query_frames = 1;
	}

/* hook up the HSP's */
	hp_frame_start[0] = hsp_array[0];
	hp_frame_number[0] = hp_frame_number[1] = 0;
	hp_frame_number[2] = hp_frame_number[3] = 0;
	frame_change = FALSE;


	/* Put entries with different frame parity into separate 'query_frame's. -cfj */
	{
	  Int4 cur_frame=0;
	     for (index=0;index<number_of_hsps;index++) 
	     {
		H=hsp_array[index];
		hp_frame_number[cur_frame]++;

		H->prev= index ? hsp_array[index-1] : NULL;
		H->next= index<(number_of_hsps-1) ? hsp_array[index+1] : NULL;
		if (H->prev != NULL && 
		    ( (SIGN(H->query.frame) != SIGN(H->prev->query.frame))
		       || (SIGN(H->subject.frame) != SIGN(H->prev->subject.frame))   
		      
		      ))
		{ /* If frame switches, then start new list. */
		       hp_frame_number[cur_frame]--;
		       hp_frame_number[++cur_frame]++;
			hp_frame_start[cur_frame] = H;
			H->prev->next = NULL;
			H->prev = NULL;
			frame_change = TRUE;
		}
	     }
	     number_of_query_frames = cur_frame+1;
	}

	/* max_q_diff is the maximum amount q.offset can differ from q.offset_trim */
	/* This is used to break out of H2 loop early */
	if (search->pbp->old_stats == FALSE)
	{
	    for (index=0;index<number_of_hsps;index++) 
	    {
		H=hsp_array[index];
		H->query.offset_trim = H->query.offset + MIN(((H->query.length)/4), 5);
		H->query.end_trim = H->query.end - MIN(((H->query.length)/4), 5);
		H->subject.offset_trim = H->subject.offset + MIN(((H->subject.length)/4), 5);
		H->subject.end_trim = H->subject.end - MIN(((H->subject.length)/4), 5);
	     }	    
	    max_q_diff = 5;
	}
	else
	{
	    for (index=0;index<number_of_hsps;index++) 
	    {
		H=hsp_array[index];
		H->query.offset_trim = H->query.offset + (H->query.length)/8;
		H->query.end_trim = H->query.end - (H->query.length)/8;
		H->subject.offset_trim = H->subject.offset + (H->subject.length)/8;
		H->subject.end_trim = H->subject.end - (H->subject.length)/8;
		max_q_diff=MAX(max_q_diff,(H->query.length/8));
	     }	    
	}

	for (frame_index=0; frame_index<number_of_query_frames; frame_index++)
	{
	     MemFill(&hp_start, 0, sizeof(hp_start));
	     hp_start.next = hp_frame_start[frame_index];
	     hp_frame_start[frame_index]->prev = &hp_start;
	     number_of_hsps = hp_frame_number[frame_index];

	  lh_helper[0].ptr = &hp_start;
	  lh_helper[0].q_off_trim = 0;
	  lh_helper[0].s_off_trim = 0;
	  lh_helper[0].maxsum1  = -10000;
	  lh_helper[0].next_larger  = 0;

	  /* lh_helper[0]  = empty     = end marker that I added
	   * lh_helper[1]  = hsp_start = empty entry used in original code
	   * lh_helper[2]  = hsp_array->next = hsp_array[0]
	   * lh_helper[i]  = ... = hsp_array[i-2] (for i>=2) 
	   */
	  first_pass=1;    /* do full search */
	  path_changed=1;
	  for (H=hp_start.next; H!=NULL; H=H->next) 
	    H->hsp_link.changed=1;

	     while (number_of_hsps > 0)
	     {
	       Int4 last[3];
	       Int4 max[3];
	       last[0]=last[1]=last[2]=0;
	       max[0]=max[1]=max[2]=-10000;
	       /* Initialize the 'best' parameter */
	       best[0] = best[1] = NULL;


	       /* See if we can avoid recomputing all scores:
		*  - Find the max paths (based on old scores). 
		*  - If no paths were changed by removal of nodes (ie research==0) 
		*    then these max paths are still the best.
		*  - else if these max paths were unchanged, then they are still the best.
		*/
	        use_current_max=0;
		if (!first_pass){
		  Int4 max0,max1;
		  /* Find the current max sums */
		  if(!ignore_small_gaps){
		    max0 = -cutoff[0];
		    max1 = -cutoff[1];
		    for (H=hp_start.next; H!=NULL; H=H->next) {
		      Int4 sum0=H->hsp_link.sum[0];
		      Int4 sum1=H->hsp_link.sum[1];
		      if(sum0>=max0)
		      {
			max0=sum0;
			best[0]=H;
		      }
		      if(sum1>=max1)
		      {
			max1=sum1;
			best[1]=H;
		      }
		    }
		  }else{
		    maxscore = -cutoff[1];
		    for (H=hp_start.next; H!=NULL; H=H->next) {
		      Int4  sum=H->hsp_link.sum[1];
		      if(sum>=maxscore)
		      {
			maxscore=sum;
			best[1]=H;
		      }
		    }
		  }
		  if(path_changed==0){
		    /* No path was changed, use these max sums. */
		    use_current_max=1;
		  }
		  else{
		    /* If max path hasn't chaged, we can use it */
		    /* Walk down best, give up if we find a removed item in path */
		    use_current_max=1;
		    if(!ignore_small_gaps){
		      for (H=best[0]; H!=NULL; H=H->hsp_link.link[0]) 
			if (H->linked_to==-1000) {use_current_max=0; break;}
		    }
		    if(use_current_max)
		      for (H=best[1]; H!=NULL; H=H->hsp_link.link[1]) 
			if (H->linked_to==-1000) {use_current_max=0; break;}

		  }
		}

	       /* reset helper_info */
	       /* Inside this while loop, the linked list order never changes 
		* So here we initialize an array of commonly used info, 
		* and in this loop we access these arrays instead of the actual list 
		*/
		if(!use_current_max){
		for (H=&hp_start,H_index=1; H!=NULL; H=H->next,H_index++) {
		  Int4 s_frame = H->subject.frame;
		  Int4 s_off_t = H->subject.offset_trim;
		  Int4 q_off_t = H->query.offset_trim;
		  lh_helper[H_index].ptr = H;
		  lh_helper[H_index].q_off_trim = q_off_t;
		  lh_helper[H_index].s_off_trim = s_off_t;
		  for(i=0;i<BLAST_NUMBER_OF_ORDERING_METHODS;i++)
		    lh_helper[H_index].sum[i] = H->hsp_link.sum[i];
		  /* lh_helper[H_index].s_frame = SIGN(s_frame);
		   * lh_helper[H_index].prev_same = last[SIGN(s_frame)+1];
		   * last[SIGN(s_frame)+1]=H_index;
		   */
		  max[SIGN(s_frame)+1]=MAX(max[SIGN(s_frame)+1],H->hsp_link.sum[1]);
		  lh_helper[H_index].maxsum1 =max[SIGN(s_frame)+1];					   

		  /* set next_larger to link back to closest entry with a sum1 larger than this */
		  {
		    Int4 cur_sum=lh_helper[H_index].sum[1];
		    Int4 prev = H_index-1;
		    Int4 prev_sum = lh_helper[prev].sum[1];
		    while((cur_sum>=prev_sum) && (prev>0)){
		      prev=lh_helper[prev].next_larger;
		      prev_sum = lh_helper[prev].sum[1];
		    }
		    lh_helper[H_index].next_larger = prev;
		  }
		  H->linked_to = 0;
		}

		lh_helper[1].maxsum1 = -10000;


		/****** loop iter for index = 0  **************************/
	     	if(!ignore_small_gaps)
		{
		  index=0;
		     maxscore = -cutoff[index];
		     H_index = 2;
		     for (H=hp_start.next; H!=NULL; H=H->next,H_index++) 
		     {
		        Int4 H_hsp_num=0;
			Int4 H_hsp_sum=0;
			Nlm_FloatHi H_hsp_xsum=0.0;
			VoidPtr H_hsp_link=NULL;
			if (H->score > cutoff[index]) {
			  Int4 H_sign_sframe = SIGN(H->subject.frame);
			  Int4 H_query_etrim = H->query.end_trim;
			  Int4 H_sub_etrim = H->subject.end_trim;
			  Int4 H_q_et_gap = H_query_etrim+gap_size;
			  Int4 H_s_et_gap = H_sub_etrim+gap_size;

			  /* We only walk down hits with the same frame sign */
			  /* for (H2=H->prev; H2!=NULL; H2=H2->prev,H2_index--) */
			  for (H2_index=H_index-1; H2_index>1; H2_index=H2_index-1) 
			    {
			        Int4 b1,b2,b4,b5;
				Int4 q_off_t,s_off_t,sum;

				/* s_frame = lh_helper[H2_index].s_frame; */
				q_off_t = lh_helper[H2_index].q_off_trim;
				s_off_t = lh_helper[H2_index].s_off_trim;

				/* combine tests to reduce mispredicts -cfj */
				b1 = q_off_t <= H_query_etrim;
				b2 = s_off_t <= H_sub_etrim;
				/* b3 = s_frame - H_sign_sframe; */
				sum = lh_helper[H2_index].sum[index];

					
				b4 = ( q_off_t > H_q_et_gap ) ;
				b5 = ( s_off_t > H_s_et_gap ) ;

                               /* list is sorted by q_off, so q_off should only increase.
                                * q_off_t can only differ from q_off by max_q_diff
                                * So once q_off_t is large enough (ie it exceeds limit 
                                * by max_q_diff), we can stop.  -cfj 
                                */
				  if(q_off_t > (H_q_et_gap+max_q_diff))
				  break;

				if (b1|b2|b5|b4) continue;

				  
				if (sum>H_hsp_sum) 
				{
				        H2=lh_helper[H2_index].ptr; 
					H_hsp_num=H2->hsp_link.num[index];
					H_hsp_sum=H2->hsp_link.sum[index];
					H_hsp_xsum=H2->hsp_link.xsum[index];
					H_hsp_link=H2;

				}
			    } /* end for H2... */
			}
			{ 
			  BLAST_Score score=H->score;
			  Nlm_FloatHi new_xsum = H_hsp_xsum + (score*(kbp[H->context]->Lambda));
			  Int4 new_sum = H_hsp_sum + (score - cutoff[index]);

			  H->hsp_link.sum[index] = new_sum;
			  H->hsp_link.num[index] = H_hsp_num+1;
			  H->hsp_link.link[index] = H_hsp_link;
			  lh_helper[H_index].sum[index] = new_sum;
			  if (new_sum >= maxscore) 
			    {
			 	maxscore=new_sum;
				best[index]=H;
		    }
			  H->hsp_link.xsum[index] = new_xsum;
			  if(H_hsp_link)
			    ((BLAST_HSPPtr)H_hsp_link)->linked_to++;
			}
		     } /* end for H=... */
		}
		/****** loop iter for index = 1  **************************/
		  index=1;
		     maxscore = -cutoff[index];
		     H_index = 2;
		     for (H=hp_start.next; H!=NULL; H=H->next,H_index++) 
		     {
		        Int4 H_hsp_num=0;
			Int4 H_hsp_sum=0;
			Nlm_FloatHi H_hsp_xsum=0.0;
			VoidPtr H_hsp_link=NULL;

			H->hsp_link.changed=1;
			H2 = H->hsp_link.link[index];
			if ( (!first_pass) &&
			     ( (H2==0) || (H2->hsp_link.changed==0) )
			   )
			  {
			    /* If The best choice last time has not been changed, then it is still the 
			       best choice, so no need to walk down list. */
			    if(H2){
			      H_hsp_num=H2->hsp_link.num[index];
			      H_hsp_sum=H2->hsp_link.sum[index];
			      H_hsp_xsum=H2->hsp_link.xsum[index];
			    }
			    H_hsp_link=H2;
			    H->hsp_link.changed=0;
			  } else
			if (H->score > cutoff[index]) {
			  Int4 H_sign_sframe = SIGN(H->subject.frame);
			  Int4 H_query_etrim = H->query.end_trim;
			  Int4 H_sub_etrim = H->subject.end_trim;
			  Int4 H_q_et_gap = H_query_etrim+gap_size;
			  Int4 H_s_et_gap = H_sub_etrim+gap_size;


			  /* Here we look at what was the best choice last time (if it's still around)
			   * and set this to the initial choice.  By setting the best score to 
			   * a (potentially) large value initially, we can reduce the number of 
			   * hsps checked.  -cfj
			   */

			  /* Currently we set the best score to a value just less than the real value. This 
			   * is not really necessary, but doing this ensures that in the case of a tie, we 
			   * make the same selection the original code did.
			   */

#if 1
			  if(!first_pass&&H2&&H2->linked_to>=0){
			    if(1){
			      /* We set this to less than the real value to keep the original ordering
			       * in case of ties. */
			      H_hsp_sum=H2->hsp_link.sum[index]-1;
			    }else{
			      H_hsp_num=H2->hsp_link.num[index];
			      H_hsp_sum=H2->hsp_link.sum[index];
			      H_hsp_xsum=H2->hsp_link.xsum[index];
			      H_hsp_link=H2;
			    }
			  }
#endif

			  /* We now only walk down hits with the same frame sign */
			  /* for (H2=H->prev; H2!=NULL; H2=H2->prev,H2_index--) */
			  for (H2_index=H_index-1; H2_index>1;)
			    {
			        Int4 b0,b1,b2;
				Int4 q_off_t,s_off_t,sum,next_larger;
				LinkHelpStruct * H2_helper=&lh_helper[H2_index];
				sum = H2_helper->sum[index];
				next_larger = H2_helper->next_larger;

				s_off_t = H2_helper->s_off_trim;
				q_off_t = H2_helper->q_off_trim;

				b0 = sum <= H_hsp_sum;

				/* Compute the next H2_index */
				H2_index--;
				if(b0){	 /* If this sum is too small to beat H_hsp_sum, advance to a larger sum */
				  H2_index=next_larger;
				}

				/* combine tests to reduce mispredicts -cfj */
				b1 = q_off_t <= H_query_etrim;
				b2 = s_off_t <= H_sub_etrim;

				if(0) if(H2_helper->maxsum1<=H_hsp_sum)break;

				if (!(b0|b1|b2) )
				{
  				        H2 = H2_helper->ptr;

					H_hsp_num=H2->hsp_link.num[index];
					H_hsp_sum=H2->hsp_link.sum[index];
					H_hsp_xsum=H2->hsp_link.xsum[index];
					H_hsp_link=H2;
				}
				
			    } /* end for H2_index... */
			} /* end if(H->score>cuttof[]) */
			{ 
			  BLAST_Score score=H->score;
			  Nlm_FloatHi new_xsum = H_hsp_xsum + (score*(kbp[H->context]->Lambda));
			  Int4 new_sum = H_hsp_sum + (score - cutoff[index]);

			  H->hsp_link.sum[index] = new_sum;
			  H->hsp_link.num[index] = H_hsp_num+1;
			  H->hsp_link.link[index] = H_hsp_link;
			  lh_helper[H_index].sum[index] = new_sum;
			  lh_helper[H_index].maxsum1 = MAX(lh_helper[H_index-1].maxsum1, new_sum);
			  /* Update this entry's 'next_larger' field */
			  {
			    Int4 cur_sum=lh_helper[H_index].sum[1];
			    Int4 prev = H_index-1;
			    Int4 prev_sum = lh_helper[prev].sum[1];
			    while((cur_sum>=prev_sum) && (prev>0)){
			      prev=lh_helper[prev].next_larger;
			      prev_sum = lh_helper[prev].sum[1];
			    }
			    lh_helper[H_index].next_larger = prev;
			  }

			  if (new_sum >= maxscore) 
			    {
			 	maxscore=new_sum;
				best[index]=H;
			    }
			  H->hsp_link.xsum[index] = new_xsum;
			  if(H_hsp_link)
			    ((BLAST_HSPPtr)H_hsp_link)->linked_to++;
			}
		     }
		path_changed=0;
		first_pass=0;
		}
    /********************************/
	     lab1:
		if (search->pbp->old_stats == FALSE && search->pbp->use_large_gaps == FALSE)
		{
		  if (!ignore_small_gaps)
		  {
		    /* Select the best ordering method.
		    First we add back in the value cutoff[index] * the number 
		    of links, as this was subtracted out for purposes of the
		    comparison above. */
		    best[0]->hsp_link.sum[0] += (best[0]->hsp_link.num[0])*cutoff[0];
		    prob[0] = BlastSmallGapSumE(kbp[search->first_context], gap_size, gap_prob, gap_decay_rate, best[0]->hsp_link.num[0],best[0]->hsp_link.sum[0], best[0]->hsp_link.xsum[0], search->context[search->first_context].query->effective_length, subject_length, FALSE);
		    best[1]->hsp_link.sum[1] += (best[1]->hsp_link.num[1])*cutoff[1];
		    prob[1] = BlastLargeGapSumE(kbp[search->first_context], gap_prob, gap_decay_rate, best[1]->hsp_link.num[1],best[1]->hsp_link.sum[1], best[1]->hsp_link.xsum[1], search->context[search->first_context].query->effective_length, subject_length, FALSE);
		    ordering_method = prob[0]<=prob[1] ? 0:1;
		  }
		  else
		  {
		    /* We only consider the case of big gaps. */
		    best[1]->hsp_link.sum[1] += (best[1]->hsp_link.num[1])*cutoff[1];
		    /* gap_prob=0 here as small gaps are NOT considered. */
		    prob[1] = BlastLargeGapSumE(kbp[search->first_context], 0.0, gap_decay_rate, best[1]->hsp_link.num[1],best[1]->hsp_link.sum[1], best[1]->hsp_link.xsum[1], search->context[search->first_context].query->effective_length, subject_length, FALSE);
		    ordering_method = 1;
		  }
		}
		else
		{
		    /* We only consider the case of big gaps. */
		    best[1]->hsp_link.sum[1] += (best[1]->hsp_link.num[1])*cutoff[1];
		    /* gap_prob=0 here as small gaps are NOT considered. */
		    prob[1] = BlastLargeGapSumE(kbp[search->first_context], 0.0, gap_decay_rate, best[1]->hsp_link.num[1],best[1]->hsp_link.sum[1], best[1]->hsp_link.xsum[1], search->context[search->first_context].query->effective_length, subject_length, TRUE);
		    ordering_method = 1;
		}

		best[ordering_method]->start_of_chain = TRUE;
		
		prob[ordering_method] *= 
			((Nlm_FloatHi)search->searchsp_eff/((Nlm_FloatHi)subject_length*search->context[search->first_context].query->effective_length));
		best[ordering_method]->evalue = prob[ordering_method];

/* remove the links that have been ordered already. */
		if (best[ordering_method]->hsp_link.link[ordering_method])
		{
			linked_set = TRUE;
		}
		else
		{
			linked_set = FALSE;
		}
		if (best[ordering_method]->linked_to>0) path_changed=1;
		for (H=best[ordering_method]; H!=NULL;
			H=H->hsp_link.link[ordering_method]) 
		{
		  if (H->linked_to>1) path_changed=1;
		  H->linked_to=-1000;
		  H->hsp_link.changed=1;
			/* record whether this is part of a linked set. */
			H->linked_set = linked_set;
			if (ordering_method == 0)
				H->ordering_method = BLAST_SMALL_GAPS;
			else
				H->ordering_method = BLAST_LARGE_GAPS;
			H->evalue = prob[ordering_method];
			if (H->next)
				(H->next)->prev=H->prev;
			if (H->prev)
				(H->prev)->next=H->next;
			number_of_hsps--;
		}

	     } /* end while num_hsps... */
	} /* end for frame_index ... */


	HeapSort(hsp_array,total_number_of_hsps,sizeof(BLAST_HSPPtr), rev_compare_hsps);
/* Sort by starting position. */

	HeapSort(hsp_array, total_number_of_hsps,sizeof(BLAST_HSPPtr), fwd_compare_hsps);

	for (index=0, last_hsp=NULL;index<total_number_of_hsps; index++) 
	{
		H = hsp_array[index];
		H->prev = NULL;
		H->next = NULL;
	}

/* hook up the HSP's. */
	first_hsp = NULL;
	for (index=0, last_hsp=NULL;index<total_number_of_hsps; index++) 
	{
		H = hsp_array[index];

/* If this is not a single piece or the start of a chain, then Skip it. */
	     	if (H->linked_set == TRUE && H->start_of_chain == FALSE)
			continue;

/* If the HSP has no "link" connect the "next", otherwise follow the "link"
chain down, connecting them with "next" and "prev". */
		if (last_hsp == NULL)
			first_hsp = H;
		H->prev = last_hsp;
		ordering_method = H->ordering_method;
		if (H->hsp_link.link[ordering_method] == NULL)
		{
/* Grab the next HSP that is not part of a chain or the start of a chain */
/* The "next" pointers are not hooked up yet in HSP's further down array. */
		     index1=index;
		     H2 = index1<(total_number_of_hsps-1) ? hsp_array[index1+1] : NULL;
	     	     while (H2 && H2->linked_set == TRUE && 
				H2->start_of_chain == FALSE)
		     {
			index1++;
		     	H2 = index1<(total_number_of_hsps-1) ? hsp_array[index1+1] : NULL;
		     }
		     H->next= H2;
		}
		else
		{
			/* The first one has the number of links correct. */
			num_links = H->hsp_link.num[ordering_method];
			link = H->hsp_link.link[ordering_method];
			while (link)
			{
				H->num = num_links;
				H->sumscore = H->hsp_link.sum[ordering_method];
				H->next = (BLAST_HSPPtr) link;
				H->prev = last_hsp;
				last_hsp = H;
				H = H->next;
				if (H != NULL)
				    link = H->hsp_link.link[ordering_method];
				else
				    break;
			}
			/* Set these for last link in chain. */
			H->num = num_links;
			H->sumscore = H->hsp_link.sum[ordering_method];
/* Grab the next HSP that is not part of a chain or the start of a chain */
		     	index1=index;
		     	H2 = index1<(total_number_of_hsps-1) ? hsp_array[index1+1] : NULL;
	     	     	while (H2 && H2->linked_set == TRUE && 
				H2->start_of_chain == FALSE)
		     	{
			    index1++;
		     	    H2 = index1<(total_number_of_hsps-1) ? hsp_array[index1+1] : NULL;
			}
		     	H->next= H2;
			H->prev = last_hsp;
		}
		last_hsp = H;
	}
	
	return first_hsp;
}
/*
	Checks Hitlist's for an HSP (or set of HSP's) with the 
	minimum e-value.  Discards those that do not meet the
	standard.
*/

Int2 LIBCALL
BlastReapHitlistByEvalue (BlastSearchBlkPtr search)

{
	BLAST_HitListPtr hitlist;
	BLAST_HSPPtr hsp;
	BLAST_HSPPtr PNTR hsp_array;
	Boolean hsp_deleted=FALSE;
	Int4 hsp_cnt=0;
	Int4 index;
	Nlm_FloatHi cutoff;

	if (search == NULL)
		return 1;

	cutoff = search->pbp->cutoff_e;

	hitlist = search->current_hitlist;
	if (hitlist)
	{
		hitlist->hspcnt_max = hitlist->hspcnt;
		hsp_array = hitlist->hsp_array;
		for (index=0; index<hitlist->hspcnt; index++)
		{
			hsp = hsp_array[index];
			if (hsp->evalue > cutoff &&
				(search->pbp->no_check_score || search->pbp->cutoff_s > hsp->score))
			{
                            hsp_array[index]->gap_info = 
                               GapXEditBlockDelete(hsp_array[index]->gap_info);
                            hsp_array[index] = MemFree(hsp_array[index]);
                            hsp_deleted = TRUE;
			}
			else
			{
				hsp->pvalue = BlastKarlinEtoP(hsp->evalue);
				hsp_cnt++;
			}
		}
                if (hsp_deleted == TRUE)
		{
			HspArrayPurge(hitlist->hsp_array, hitlist->hspcnt, FALSE);
		}

		hitlist->hspcnt = hsp_cnt;
		hitlist->hspcnt_max = hitlist->hspcnt;
		if (hitlist->hspcnt == 0)
		{
			BlastHitListPurge(hitlist);
		}
		else
		{	
        		NlmMutexLockEx(&search->thr_info->callback_mutex);
			search->thr_info->number_of_pos_hits++;
            		NlmMutexUnlock(search->thr_info->callback_mutex);
			search->number_of_seqs_better_E++;
		}
	}
	search->current_hitlist = hitlist;
	return 0;
}

/*
	Checks Hitlist's for an HSP (or set of HSP's) with the 
	minimum e-value.  Discards those that do not meet the
	standard.
*/

Int2 LIBCALL
BlastGetNonSumStatsEvalue (BlastSearchBlkPtr search)
{
	BLAST_HitListPtr hitlist;
	BLAST_HSPPtr hsp;
	BLAST_HSPPtr PNTR hsp_array;
	BLAST_KarlinBlkPtr PNTR kbp;
	Int4 hsp_cnt;
	Int4 index;

	if (search == NULL)
		return 1;

	if (search->pbp->gapped_calculation && search->prog_number != blast_type_blastn)
	{
		kbp = search->sbp->kbp_gap;
	}
	else
	{
		kbp = search->sbp->kbp;
	}

	hitlist = search->current_hitlist;
	if (hitlist)
	{
		hsp_cnt = hitlist->hspcnt;
		hsp_array = hitlist->hsp_array;
		for (index=0; index<hsp_cnt; index++)
		{
			hsp = hsp_array[index];
                        if (!search->pbp->mb_params)
                           hsp->evalue = BlastKarlinStoE_simple(hsp->score, kbp[hsp->context], search->searchsp_eff);
                        else {
                           FloatHi searchsp_eff;
                           hsp->context = BinarySearchInt4(hsp->query.offset, search->query_context_offsets, (Int4) (search->last_context+1));
                           searchsp_eff = (FloatHi) search->dblen_eff *
                              (FloatHi) search->context[hsp->context].query->effective_length;
                           hsp->evalue = BlastKarlinStoE_simple(hsp->score, kbp[hsp->context], searchsp_eff);
                        }
		}
	}
	return 0;
}

Int2 LIBCALL
BlastTimeFillStructure(BlastTimeKeeperPtr btkp)

{
	CPUTimePtr	pTime;

	if (btkp == NULL)
	    return 1;

	pTime = CPUTimeMeasure();
	if (pTime == NULL)
	    return 1;

	btkp->user = (Nlm_FloatLo) CPUTimeGetUser(pTime);
	btkp->system = (Nlm_FloatLo) CPUTimeGetSys(pTime);
	btkp->total = btkp->user + btkp->system;

	CPUTimeFree(pTime);

	return 0;
}

/*
	starts the awake thread using static variables in this file.
*/

void
BlastStartAwakeThread(BlastThrInfoPtr thr_info)
{
    VoidPtr status=NULL;
    
    if (!thr_info->tick_callback)
       return;
    /* If awake_thr is running from the last search, then wait for the join. */
    /* This pointer is NULL on the first search ever. */
    if (thr_info->awake_thr) {
        NlmThreadJoin(thr_info->awake_thr, &status);
        thr_info->awake_thr = NULL;
    }
    
    if (NlmThreadsAvailable()) {
        thr_info->awake = TRUE;
        /* last tick is used by 'star_proc' */
        thr_info->awake_thr = 
            NlmThreadCreate(star_proc, thr_info);
    }

    return;
}

/* Change the awake flag.  This thread will die in one second. */
void 
BlastStopAwakeThread(BlastThrInfoPtr thr_info)
{
    thr_info->awake = FALSE;
    
}
