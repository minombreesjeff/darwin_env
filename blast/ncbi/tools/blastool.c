static char const rcsid[] = "$Id: blastool.c,v 6.280 2005/02/15 21:10:47 dondosha Exp $";

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
* $Revision: 6.280 $
* $Log: blastool.c,v $
* Revision 6.280  2005/02/15 21:10:47  dondosha
* Set X-dropoff for the traceback in MegaBlastPrintAlignInfo to final X-dropoff parameter
*
* Revision 6.279  2004/12/09 16:21:21  camacho
* Do not do the traceback for non-Smith-Waterman blastpgp runs [by Mike Gertz]
*
* Revision 6.278  2004/11/19 13:22:05  madden
* Remove no_check_score completely (from Mike Gertz)
*
* Revision 6.277  2004/11/17 13:03:56  madden
* Always set no_score_check to TRUE
*
* Revision 6.276  2004/11/01 20:43:15  camacho
* + BlastErrorToString
*
* Revision 6.275  2004/08/31 17:07:14  dondosha
* For PRINT_SEQUENCES option in tabular output, always show query on forward strand; reverse complement subject is necessary
*
* Revision 6.274  2004/08/26 19:01:35  dondosha
* Retrieve Bioseq before attempting to calculate number of identities in alignment segments
*
* Revision 6.273  2004/08/16 19:37:26  dondosha
* Enabled uneven gap HSP linking for blastx
*
* Revision 6.272  2004/07/15 21:00:17  dondosha
* Print Accession.Version in megablast tabular output subject ids; print number of sequences in # Query comment if multiple queries
*
* Revision 6.271  2004/06/30 13:42:27  kans
* include <blfmtutl.h> to clear up Mac compiler missing prototype errors
*
* Revision 6.270  2004/06/30 12:29:39  madden
* Moved some functions to blfmtutl.c
*
* Revision 6.269  2004/06/24 21:16:34  dondosha
* Boolean argument in ScoreAndEvalueToBuffers changed to Uint1, so pass 0 instead of FALSE
*
* Revision 6.268  2004/05/21 13:53:04  dondosha
* Use BLAST_HSPFree to free BLAST_HSP structures, hence no need to call GapXEditBlockDelete in multiple places
*
* Revision 6.267  2004/05/14 15:38:11  dondosha
* Use newly public function ScoreAndEvalueToBuffers from txalign.h instead of a static function
*
* Revision 6.266  2004/05/14 14:41:03  bealer
* - Er. I mean .002, as per blastpgp.
*
* Revision 6.265  2004/05/14 14:39:45  bealer
* - Adjust ethresh to .001 for PSI blast.
*
* Revision 6.264  2004/04/30 15:25:20  dondosha
* Added argument in call to BXMLGetHspFromSeqAlign
*
* Revision 6.263  2004/04/28 20:32:55  bealer
* - Fix BLAST_Wizard defaults for PSI-Blast: comp-based-stats=TRUE, E=0.005.
*
* Revision 6.262  2004/03/22 22:04:43  dondosha
* Do not reassign kbp_gap pointers for megablast traceback, as they are already set due to recent changes
*
* Revision 6.261  2004/03/22 15:24:56  dondosha
* Do not set gapped-search-specific options for tblastx
*
* Revision 6.260  2004/02/26 15:52:30  papadopo
* Mike Gertz' modifications to unify handling of gapped Karlin blocks between protein and nucleotide searches
*
* Revision 6.259  2004/02/24 14:07:42  camacho
* BlastAdjustDbNumbers is DEPRECATED
*
* Revision 6.258  2003/12/30 15:14:40  camacho
* Fix to MergeDbGiListsWithOIDLists:
* When searching gi lists for matches in rdfp linked list, isolate each
* element of the linked list first.
*
* Revision 6.257  2003/12/23 23:28:27  dondosha
* Use number of identities from the seqalign to calculate percent identity in tabular output
*
* Revision 6.256  2003/12/12 16:01:23  madden
* Change to signature of BlastCutoffs, remove BlastCutoffs_simple
*
* Revision 6.255  2003/11/10 16:59:42  dondosha
* Print the seqid in AcknowledgeBlastQuery for any non-local id
*
* Revision 6.254  2003/10/29 17:46:59  dondosha
* Allow 2-stage greedy extension in megablast
*
* Revision 6.253  2003/09/12 16:02:06  dondosha
* If gap open is non-zero for megablast, require gap extension option to be non-zero too
*
* Revision 6.252  2003/08/21 16:06:37  dondosha
* Correction to previous change
*
* Revision 6.251  2003/08/20 22:13:18  dondosha
* Added BlastPrintTabularResults with an extra boolean parameter for OOF alignments
*
* Revision 6.250  2003/07/15 20:33:23  madden
* set is_rps_blast if service is rpsblast
*
* Revision 6.249  2003/07/15 14:33:43  dondosha
* Added a #define for fprintf substitute, needed for gzip compression of Web BLAST results
*
* Revision 6.248  2003/06/12 16:47:40  madden
* Fix so all query info on one line for tab. report
*
* Revision 6.247  2003/06/11 20:19:16  madden
* Fixes to PrintTabularOutputHeader
*
* Revision 6.246  2003/05/30 17:25:36  coulouri
* add rcsid
*
* Revision 6.245  2003/05/22 21:39:23  dondosha
* Correction to previous change
*
* Revision 6.244  2003/05/22 20:45:27  dondosha
* Fix in BlastCreateVirtualOIDList when rdfps with oidlist exist before rdfps with gi lists
*
* Revision 6.243  2003/05/13 16:02:53  coulouri
* make ErrPostEx(SEV_FATAL, ...) exit with nonzero status
*
* Revision 6.242  2003/05/06 15:19:19  dondosha
* Removed extra memory freeing statement for megablast -D3 option
*
* Revision 6.241  2003/05/05 16:48:40  camacho
* Removed warning about gi list restrictions
*
* Revision 6.240  2003/04/25 18:56:14  camacho
* Updated MergeDbGiListsWithOIDLists to use gilists as opposed to gifiles
*
* Revision 6.239  2003/04/24 17:10:15  dondosha
* Fixed FastaCheckDna: check only one sequence at a time
*
* Revision 6.238  2003/04/23 23:34:07  boemker
* Bug fixes: BLAST_Wizard ignored mask->use_best_align,
* mask->use_real_db_size, mask->window_size.
*
* Revision 6.237  2003/04/23 23:22:37  boemker
* Bug fix: BLAST_Wizard ignored strand_option.
*
* Revision 6.236  2003/04/23 23:11:16  boemker
* Bug fix: BLAST_Wizard ignored pseudoCountConst.
*
* Revision 6.235  2003/04/23 22:55:27  boemker
* Bug fixes: BLAST_Wizard ignored first_db_seq, final_db_seq.
*
* Revision 6.234  2003/04/23 22:48:25  boemker
* Bug fix: BLAST_Wizard ignored is_ooframe.
*
* Revision 6.233  2003/04/22 21:52:13  dondosha
* Added function OOFBlastHSPGetNumIdentical
*
* Revision 6.232  2003/04/17 20:51:33  camacho
* Rolled back previous changes
*
* Revision 6.230  2003/04/14 19:53:18  camacho
* Use ISAMUninitSearch to avoid running into memory problems
*
* Revision 6.229  2003/04/04 20:02:38  dondosha
* ABR bug fix in BlastPrintTabulatedResultsEx
*
* Revision 6.228  2003/03/26 20:29:50  boemker
* By default, set required_end = 0 to match behavior of blastcgicmd.cpp
*
* Revision 6.227  2003/03/26 15:47:52  boemker
* sizeof x -> sizeof(x)
*
* Revision 6.226  2003/03/26 15:46:18  boemker
* In BLAST_OptionsBlkInit, use memset to clear structure.
*
* Revision 6.225  2003/03/26 13:36:06  boemker
* Corrected misspelling (opts => options) in BLAST_WizardOptionsBlkInit.
*
* Revision 6.224  2003/03/26 02:45:25  boemker
* Rewrote BLAST_WizardOptionsBlkInit to reflect changes in
* BLAST_WizardOptionsBlk.
*
* Revision 6.223  2003/03/25 22:25:57  boemker
* Replaced cutoff_s2, which isn't used, with cutoff_s, which is.  Added
* query_lcase_mask.  Made alignments and descriptions (arguments of function
* BLAST_Wizard) optional.  Rewrote to avoid use of snprintf, which isn't
* portable.
*
* Revision 6.222  2003/03/25 19:58:18  boemker
* Moved code to initialize search options from blastcgicmd.cpp to here, as
* BLAST_Wizard et al.
*
* Revision 6.221  2003/03/24 19:42:14  madden
* Changes to support query concatenation for blastn and tblastn
*
* Revision 6.220  2003/02/28 18:34:41  dondosha
* Added a possibility to print sequences in the -D3 output
*
* Revision 6.219  2003/02/18 19:50:09  madden
* Correctly report number of sequences better than expect value for gapped search
*
* Revision 6.218  2003/01/31 17:57:27  camacho
* Fix to MergeDbGiFilesWithOIDLists
*
* Revision 6.217  2003/01/15 19:06:30  dondosha
* Correction in copying defline in BlastPrintTabulatedResultsEx
*
* Revision 6.216  2003/01/14 20:28:53  madden
* New function BLASTAddBlastDBTitleToSeqAnnotEx
*
* Revision 6.215  2002/12/04 23:32:49  camacho
* Do not set use_this_gi with nucleotide dbs (redundant)
*
* Revision 6.214  2002/11/26 23:02:34  madden
* Make ErrorMessage optional in ParseBlastOptions
*
* Revision 6.213  2002/11/21 21:28:27  camacho
* Mask the entire volume if no gis in rdfp->gilist belong to rdfp
*
* Revision 6.212  2002/11/08 20:52:01  dondosha
* After the traceback, reduce hitlist size back to the original
*
* Revision 6.211  2002/11/04 22:53:32  dondosha
* 1. Memory leak fixed in BlastClusterHitsFromSeqAlign
* 2. Removed redundant score sets from the seqaligns produced for translated searches
* 3. Added BlastHSPGetNumIdentical function
*
* Revision 6.210  2002/09/23 16:49:00  dondosha
* Include subject gis in megablast tabular output with full seqids option
*
* Revision 6.209  2002/09/19 18:12:46  dondosha
* Do not move around Karlin blocks if there are no results for some query in megablast
*
* Revision 6.208  2002/09/13 19:12:53  camacho
* Use correct query length in FormatBlastParameters
*
* Revision 6.207  2002/09/11 16:54:29  dondosha
* Removed erroneous assignment of cutoff_s for megablast in BLASTOptionNewEx that was restored in revision 6.204
*
* Revision 6.206  2002/08/22 12:36:36  madden
* Removed unused variables
*
* Revision 6.205  2002/08/19 18:24:20  camacho
* Removed unused variables
*
* Revision 6.204  2002/08/09 19:39:20  camacho
* Added constants for some blast search parameters
*
* Revision 6.203  2002/08/07 02:06:07  camacho
* Fixed problem when merging lists of gis
*
* Revision 6.202  2002/08/06 15:43:20  dondosha
* Set window size to 0 for megablast in BLASTOptionNewEx
*
* Revision 6.201  2002/08/06 15:41:49  camacho
* Changed return type of MergeDbGiFilesWithOIDLists to void
*
* Revision 6.200  2002/08/01 20:45:34  dondosha
* Changed prototype of the BLASTPostSearchLogic function to make it
* more convenient
*
* Revision 6.199  2002/07/24 21:11:38  kans
* reverted ncbi URL
*
* Revision 6.198  2002/07/24 15:38:07  dondosha
* Corrections in BlastPostSearchLogic for processing multiple-query intermediate results from megablast
*
* Revision 6.197  2002/07/23 16:50:04  kans
* changed www.ncbi.nlm.nih.gov to www.ncbi.nih.gov
*
* Revision 6.196  2002/07/15 17:34:18  camacho
* Fixed little/big endian problem in IntersectDoubleInt4ListWithOIDLists
*
* Revision 6.195  2002/07/09 16:09:43  camacho
* Changed interface to BlastCreateVirtualOIDList
*
* Revision 6.194  2002/07/02 17:08:01  dondosha
* Reverse previous change - not needed
*
* Revision 6.193  2002/07/02 01:36:40  dondosha
* For megablast use larger window in CheckStartForGappedAlignment
*
* Revision 6.192  2002/06/26 00:56:30  camacho
*
* 1. Fixed bug when searching a mixture of real and mask databases.
* 2. Clean up of code that calculates the number of sequences and database
*    length.
*
* Revision 6.191  2002/06/25 19:39:38  camacho
* Made BlastCreateVirtualOIDList public for use by neighboring software
*
* Revision 6.190  2002/06/23 06:57:45  camacho
* Minor fix to previous commit
*
* Revision 6.189  2002/06/21 21:48:19  camacho
* Reorganized BlastProcessGiLists
*
* Revision 6.188  2002/06/19 22:50:33  dondosha
* Added all queries information for tabular output with multiple queries
*
* Revision 6.187  2002/06/13 20:48:45  dondosha
* Do not assign cutoff_s for megablast in BLASTOptionNewEx - it is not needed anyway
*
* Revision 6.186  2002/06/06 22:03:06  camacho
*
* 1. Fixed bug in BlastDbGiListToOidList
* 2. Removed statement in BlastProcessGiLists that sets num_seqs to 0
*
* Revision 6.185  2002/05/28 21:41:03  dondosha
* Correction for megablast -D3 output with non-affine extension when database has a gi list
*
* Revision 6.184  2002/05/10 22:38:49  dondosha
* Always do preliminary and then traceback gapped extension if dynamic programming extension is used
*
* Revision 6.183  2002/05/09 15:35:51  dondosha
* Added BLASTOptionNewEx function with an extra argument for megablast
*
* Revision 6.182  2002/04/18 19:05:08  camacho
* Modified BlastProcessGiLists to deal with multi-volume databases with empty oidlists
*
* Revision 6.181  2002/04/18 16:18:21  dondosha
* Added BlastPrintTabulatedResultsEx with extra argument to keep track of progress
*
* Revision 6.180  2002/04/15 20:51:38  jianye
* Added getFastaStyleTitle(BioseqPtr bsp)
*
* Revision 6.179  2002/04/10 17:42:02  madden
* Fix for blastn matrix check (last checkin)
*
* Revision 6.178  2002/04/10 15:50:15  dondosha
* Added validity checks for some megablast options
*
* Revision 6.177  2002/04/09 18:44:44  madden
* Set matrix for blastn correctly
*
* Revision 6.176  2002/03/22 17:05:09  dondosha
* For megablast tabular formatting of seqalign, count number of alignments for each query separately
*
* Revision 6.175  2002/02/27 20:13:56  dondosha
* Added checks for NULL pointer in BlastInsertList2Heap
*
* Revision 6.174  2002/02/14 20:37:38  dondosha
* Bug fix in callback for blastn tabular output when only reverse strand is searched
*
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
#include <vecscrn.h>
#include <blfmtutl.h>

int (*blastool_fprintf)(FILE*, const char *, ...) = fprintf;
#define fprintf blastool_fprintf

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

	if (pbp->gapped_calculation)
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
           Int8 qlen;
           if (search->pbp->is_rps_blast) {
               qlen = (Int8) search->rps_qlen;
           } else {
               qlen = (Int8)
                   search->context[search->first_context].query->length;
           }
           sprintf(buffer, "length of query: %s", Nlm_Int8tostr(qlen, 1));
           add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
        }
        sprintf(buffer, "length of database: %s", Nlm_Int8tostr ((Int8) search->dblen, 1));
	add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);

	sprintf(buffer, "effective HSP length: %ld", (long) search->length_adjustment);
	add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);

        if (search->last_context <= 1) {
           Int8 eff_qlen;
           if (search->pbp->is_rps_blast) {
               eff_qlen = (Int8) search->rps_qlen - search->length_adjustment;
            } else {
               eff_qlen = (Int8)
               search->context[search->first_context].query->effective_length;
           }

           sprintf(buffer, "effective length of query: %s", 
                   Nlm_Int8tostr(eff_qlen, 1));
           add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
        }
	sprintf(buffer, "effective length of database: %s", Nlm_Int8tostr ((Int8) search->dblen_eff, 1));
	add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
        if (search->last_context <= 1) {
           Int8 eff_qlen;
           if (search->pbp->is_rps_blast) {
               eff_qlen = (Int8) search->rps_qlen - search->length_adjustment;
            } else {
               eff_qlen = (Int8)
               search->context[search->first_context].query->effective_length;
           }
           sprintf(buffer, "effective search space: %8.0f", ((Nlm_FloatHi)
                       search->dblen_eff)*((Nlm_FloatHi) eff_qlen));
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
           sprintf(buffer, "X1: %ld (%4.1f bits)", (long) (-search->pbp->dropoff_2nd_pass), ((-search->pbp->dropoff_2nd_pass)*(search->sbp->kbp[search->first_context]->Lambda/NCBIMATH_LN2)));
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
          if (search->pbp->gapped_calculation == FALSE) {
            /* AM: Changed to support query concatenation. */
            if( !search->mult_queries ) {
              Nlm_FloatHi search_sp =
                search->context[search->first_context]
                .query->effective_length *
                (Nlm_FloatHi) search->dblen_eff;
              BlastCutoffs(&cutoff, &evalue,
                           search->sbp->kbp[search->first_context],
                           search_sp, FALSE, 0.0 );
            } else {
              Nlm_FloatHi search_sp =
                search->mult_queries->MinLenEff *
                (Nlm_FloatHi) search->dblen_eff;
              BlastCutoffs(&cutoff, &evalue,
                           search->sbp->kbp[search->first_context],
                           search_sp, FALSE, 0.0 );
            }
            
            sprintf(buffer, "S2: %ld (%4.1f bits)", (long) cutoff, (((cutoff)*(search->sbp->kbp[search->first_context]->Lambda))-(search->sbp->kbp[search->first_context]->logK))/NCBIMATH_LN2);
          } else {
            if( !search->mult_queries ) {
              Nlm_FloatHi search_sp =
                search->context[search->first_context]
                .query->effective_length *
                (Nlm_FloatHi) search->dblen_eff;
              BlastCutoffs(&cutoff, &evalue,
                           search->sbp->kbp_gap[search->first_context],
                           search_sp, FALSE, 0.0  );
            } else {
              Nlm_FloatHi search_sp =
                search->mult_queries->MinLenEff *
                (Nlm_FloatHi)search->dblen_eff;
              BlastCutoffs( &cutoff, &evalue, 
                            search->sbp->kbp_gap[search->first_context],
                            search_sp, FALSE, 0.0 );
            }
            sprintf(buffer, "S2: %ld (%4.1f bits)", (long) cutoff, (((cutoff)*(search->sbp->kbp_gap[search->first_context]->Lambda))-(search->sbp->kbp_gap[search->first_context]->logK))/NCBIMATH_LN2);
          }
          add_string_to_buffer(buffer, &ret_buffer, &ret_buffer_length);
        }
        return ret_buffer;
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
				ErrPostEx(SEV_FATAL, 1, 0, "%s", error_msg->msg);
				break;
			default:
				ErrPostEx(SEV_WARNING, 0, 0, "Unknown BLAST error level");
				break;
		}
		error_return = error_return->next;
	}
	return;
	
}

CharPtr LIBCALL BlastErrorToString(ValNodePtr error_return)
{
	BlastErrorMsgPtr error_msg;
    ValNodePtr tmp = NULL;
    Uint4 length = 0;
    CharPtr retval = NULL;

	if ( !error_return ) {
		return NULL;
    }

    /* First, determine the length of the error messages */
    for (tmp = error_return; tmp; tmp = tmp->next) {
        error_msg = (BlastErrorMsgPtr) tmp->data.ptrvalue;
        length += StringLen(error_msg->msg);
        switch (error_msg->level) {
        case 0: length += 4; break;     /* INFO */
        case 1: length += 7; break;     /* WARNING */
        case 2: length += 5; break;     /* ERROR */
        case 3: length += 5; break;     /* FATAL */
        default: break;
        }
        length += 2;        /* for space and ':' */
        length += 2;        /* for newline/NULL byte */
    }

    retval = (CharPtr) Malloc(sizeof(Char)*length);
    if ( !retval ) {
        return NULL;
    }
    *retval = NULLB;

    for (tmp = error_return; tmp; tmp = tmp->next) {
        Char buffer[BUFFER_LENGTH] = { NULLB };
        error_msg = (BlastErrorMsgPtr) tmp->data.ptrvalue;

		switch (error_msg->level)
		{
        case 0:
            sprintf(buffer, "%s: %s\n", "INFO", error_msg->msg);
            break;
        case 1:
            sprintf(buffer, "%s: %s\n", "WARNING", error_msg->msg);
            break;
        case 2:
            sprintf(buffer, "%s: %s\n", "ERROR", error_msg->msg);
            break;
        case 3:
            sprintf(buffer, "%s: %s\n", "FATAL", error_msg->msg);
            break;
        default:
            ErrPostEx(SEV_WARNING, 0, 0, "Unknown BLAST error level");
            break;
		}
        StringNCat(retval, buffer, length);
	}
	return retval;
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
   return BLASTOptionNewEx(progname, gapped, FALSE);
}

BLAST_OptionsBlkPtr LIBCALL 
BLASTOptionNewEx(CharPtr progname, Boolean gapped, Boolean is_megablast)

{
	BLAST_OptionsBlkPtr options;

	options = (BLAST_OptionsBlkPtr) MemNew(sizeof(BLAST_OptionsBlk));

	options->perform_culling = FALSE;	/* Results should not be culled at all right now. */

	options->program_name = StringSave(progname);
	options->required_start = 0;
	options->required_end = -1;	/* -1 indicates the end of the query. */
	options->db_length = 0;		/* zero means that real size will be used. */
	options->searchsp_eff = 0;	/* zero means that real size will be used. */

	options->hsp_range_max = 100;
	options->entrez_query = NULL;
	options->gifile = NULL;
	options->gilist = NULL;
	options->sort_gi_list = TRUE;
    options->gilist_already_calculated = FALSE;

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
		options->gap_size = 40;
		options->threshold_second = WORD_THRESHOLD_BLASTN;
		options->expect_value  = 10;
		options->hitlist_size = 500;
		options->two_pass_method  = FALSE;
		options->multiple_hits_only  = FALSE;
		options->number_of_bits  = 0.0;
		/* 1st pass not done for blastn. */
		options->dropoff_2nd_pass  = UNGAPPED_X_DROPOFF_NUCL;
		options->matrix  = NULL;
		options->old_stats  = FALSE;
		options->penalty  = PENALTY;
		options->reward  = REWARD;
		options->e2 = 0.05;
		/* Used in the post-process gapping of the blastn result. */
                if (!is_megablast) {
                   options->wordsize  = WORDSIZE_NUCL;
                   options->gap_open  = GAP_OPEN_NUCL;
                   options->gap_extend  = GAP_EXTN_NUCL;
                   options->block_width = 20;
                   options->window_size = WINDOW_SIZE_NUCL;
                   options->cutoff_s = 0;
                   options->cutoff_s2 = 0;
                   options->gap_x_dropoff  = GAP_X_DROPOFF_NUCL;
                } else {
                   options->is_megablast_search = TRUE;
                   options->wordsize = WORDSIZE_MEGABLAST;
                   options->gap_open  = GAP_OPEN_MEGABLAST;
                   options->gap_extend  = GAP_EXTN_MEGABLAST;
                   options->cutoff_s2 = 28;
                   options->window_size = WINDOW_SIZE_MEGABLAST;
                   options->gap_x_dropoff  = GAP_X_DROPOFF_MEGABLAST;
                   options->dropoff_2nd_pass = UNGAPPED_X_DROPOFF_MEGABLAST;
                }
                options->decline_align = INT2_MAX;
                options->gap_x_dropoff_final  = GAP_X_DROPOFF_FINAL_NUCL;
                options->gap_trigger  = 25.0;
                options->strand_option  = BLAST_BOTH_STRAND;
                if (gapped)
                   options->do_sum_stats = FALSE;
                else
                   options->do_sum_stats = TRUE;
	}
	else
	{
		options->gap_size = 40;
		options->window_size = WINDOW_SIZE_PROT;
		options->expect_value  = 10;
		options->hitlist_size = 500;
		options->two_pass_method  = FALSE;
		options->multiple_hits_only  = TRUE;
		options->number_of_bits  = 0.0;
		options->dropoff_1st_pass  = 7;
		options->dropoff_2nd_pass  = UNGAPPED_X_DROPOFF_PROT;
		options->matrix  = StringSave("BLOSUM62");
		options->old_stats  = FALSE;
		options->wordsize  = WORDSIZE_PROT;
		options->penalty  = 0;
		options->reward  = 0;
		options->gap_decay_rate = 0.5;
		options->gap_prob = 0.5;

		options->gap_open  = GAP_OPEN_PROT;
		options->gap_extend  = GAP_EXTN_PROT;
        options->decline_align = INT2_MAX;
		options->gap_x_dropoff  = GAP_X_DROPOFF_PROT;
		options->gap_x_dropoff_final  = GAP_X_DROPOFF_FINAL_PROT;
		options->gap_trigger  = 22.0;

		if (StringICmp(progname, "blastp") == 0)
		{
			options->e2 = BLAST_SMALLEST_EVALUE;
			options->threshold_second = WORD_THRESHOLD_BLASTP;
		}
		else if (StringICmp(progname, "blastx") == 0)
		{
			options->e2 = 1.0;
			options->genetic_code = 1;
			options->threshold_second = WORD_THRESHOLD_BLASTX;
			options->do_sum_stats = TRUE;
			options->strand_option  = BLAST_BOTH_STRAND;
		}

        else if ( StringICmp(progname, "tblastn") == 0
			 || StringICmp(progname, "psitblastn") == 0 )
		{
			options->e2 = 1.0;
			options->db_genetic_code = 1;
			options->threshold_second = WORD_THRESHOLD_TBLASTN;
			options->do_sum_stats = TRUE;
		}
		else if (StringICmp(progname, "tblastx") == 0)
		{
			options->e2 = BLAST_SMALLEST_EVALUE;
			options->genetic_code = 1;
			options->db_genetic_code = 1;
			options->threshold_second = WORD_THRESHOLD_TBLASTX;
			options->gap_open  = 0;
			options->gap_extend  = 0;
			options->gap_x_dropoff  = 0;
			options->gap_x_dropoff_final  = 0;
			options->gapped_calculation = gapped = FALSE;
			options->do_sum_stats = TRUE;
			options->strand_option  = BLAST_BOTH_STRAND;
			options->hsp_num_max = 100;
		}
		if (gapped)
		{
			options->two_pass_method = FALSE;
			options->multiple_hits_only  = TRUE;
			options->dropoff_2nd_pass  = options->dropoff_1st_pass;
			options->gap_decay_rate = 0.1;
			options->gap_prob = 1.0;
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

               if (options->is_megablast_search && options->gap_open > 0
                   && options->gap_extend == 0) 
               {
                  BlastConstructErrorMessage("BLASTOptionValidateEx", 
                     "Gap extension penalty must be non-zero if gap open"
                     " penalty specified", 1, error_return);
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

        /* Check the validity of the megablast specific options */
        if (options->is_megablast_search) {
           if (options->mb_template_length > 0) {
              if (options->mb_template_length != 16 &&
                  options->mb_template_length != 18 &&
                  options->mb_template_length != 21) {
                 BlastConstructErrorMessage("BLASTOptionValidateEx",
                     "Only discontiguous template lengths 16, 18 and 21 are supported", 
                                            1, error_return);
                 return 1;
              }

              if (options->wordsize < 11 || options->wordsize > 12) {
                 BlastConstructErrorMessage("BLASTOptionValidateEx",
                     "Only discontiguous word sizes 11 and 12 are supported", 
                                            1, error_return);
                 return 1;
              }

              if (options->mb_disc_type > MB_TWO_TEMPLATES) {
                 BlastConstructErrorMessage("BLASTOptionValidateEx",
                     "Allowed discontiguous word types are: 0 - coding, 1 - maximal, 2 - two templates", 
                                            1, error_return);
                 return 1;
              }
           }
        }

        /* --KM check -B option: query concatenation only with certain
           BLAST flavors */
        if (options->NumQueries>0 &&
        !( (StringICmp("blastn",  progname) == 0) ||
        (StringICmp("tblastn", progname) == 0)   ) ) {

                BlastConstructErrorMessage("BLASTOptionValidateEx",
                    "Query concatenation can only be used with blastn and tblastn",
                    1, error_return);
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
    Int4 threshold, window_size;
    
    if (options == NULL || matrix_name == NULL)
        return -1;
    
/* Save these in case we need to restore (for blastn). */
    threshold =  options->threshold_second;
    window_size =  options->window_size;

/* We check for protein matrices also for blastn in case a nucl.-nucl. matrix really
has been specified. */
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
    
    
    /* blastn and megablast are different. */
    if (StringICmp("blastn", options->program_name) == 0) {
    	options->threshold_second = threshold;
    	options->window_size = window_size;
        if (!options->is_megablast_search) {
           options->gap_open  = 5;
           options->gap_extend  = 2;
           if (found_matrix == FALSE) {
               /* probably not a protein-protein matrix. */
               options->matrix = StringSave(matrix_name);
	       }
        } else {
           options->gap_open   = GAP_OPEN_MEGABLAST;
           options->gap_extend = GAP_EXTN_MEGABLAST;
        }
	return 0;
    }

    if (matrix_name) 
    {
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

    06/24/2002: This function returns the number of sequences and number of
    bases/residues based on the information on the rdfp parameter (all others
    are ignored). This function should be called after BlastProcessGiLists and
    before calculating the effective search space.

    02/12/2004: This function is *deprecated*
*/

Boolean
BlastAdjustDbNumbers (ReadDBFILEPtr rdfp, Int8Ptr db_length, Int4Ptr db_number, SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, OIDListPtr oidlist, BlastDoubleInt4Ptr PNTR gi_list_pointers, Int4 gi_list_total)

{
    return readdb_get_totals_ex2(rdfp, db_length, db_number, FALSE, TRUE);
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

BlastGiListPtr BlastGiListNew(BlastDoubleInt4Ptr gi_list, Int4 tot)
{
	BlastGiListPtr blast_gi_list = NULL;
    register Int4 i;

	if (gi_list == NULL || tot == 0)
		return NULL;

	if ((blast_gi_list = MemNew(sizeof(BlastGiList))) == NULL)  {
        ErrPostEx(SEV_ERROR,0,0,"BlastGiListNew: Out of memory\n");
        return NULL;
    }

    blast_gi_list->gi_list = MemNew(sizeof(BlastDoubleInt4)*tot);
    blast_gi_list->gi_list_pointer = MemNew(sizeof(BlastDoubleInt4Ptr)*tot);
    if ((blast_gi_list->gi_list == NULL) || 
        (blast_gi_list->gi_list_pointer == NULL)) {
        ErrPostEx(SEV_ERROR,0,0,"BlastGiListNew: Out of memory\n");
        return NULL;
    }

    for (i = 0; i < tot; i++) {
        MemCpy(&(blast_gi_list->gi_list[i]),&gi_list[i],
                sizeof(BlastDoubleInt4));
        blast_gi_list->gi_list_pointer[i] = &(blast_gi_list->gi_list[i]);
    }
	blast_gi_list->total = tot;
	blast_gi_list->current = 0;
    blast_gi_list->gilist_not_owned = FALSE; /* yes, we own this */
	
	return blast_gi_list;
}

/* This is a simple callback comparison function for sorting */
static int LIBCALLBACK
blast_double_int_oid_compare(VoidPtr v1, VoidPtr v2)
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
    if (h1->start == h2->start) { /* break the tie with gis */
        if (h1->gi < h2->gi)
            return -1;
        if (h1->gi > h2->gi)
            return 1;
    }

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
static int LIBCALLBACK
blast_double_int_start_compare(VoidPtr v1, VoidPtr v2)
{
	BlastDoubleInt4Ptr h1, h2;
	BlastDoubleInt4Ptr *hp1, *hp2;

	hp1 = (BlastDoubleInt4Ptr PNTR) v1;
	hp2 = (BlastDoubleInt4Ptr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (h1->start < h2->start)
		return -1;
	if (h1->start > h2->start)
		return 1;
    if (h1->start == h2->start) { /* break the tie with oids */
        if (h1->ordinal_id < h2->ordinal_id)
            return -1;
        if (h1->ordinal_id > h2->ordinal_id)
            return 1;
    }

	return 0;
}

/* Returns the intersection of list1 and list2. Caller is reponsible to
 * deallocate the return value */
static BlastGiListPtr IntersectBlastGiLists(
                        BlastDoubleInt4Ptr list1, Int4 total1,
                        BlastDoubleInt4Ptr list2, Int4 total2)
{
    BlastGiListPtr retval = NULL;
    BlastDoubleInt4Ptr new_list = NULL;
    BlastDoubleInt4Ptr *list_ptrs1 = NULL, *list_ptrs2 = NULL;
    Int4 idx1, idx2, new_idx = 0, short_list_sz = 0, gi;

    if ((!list1 && !list2) || (total1 <= 0 && total2 <= 0)) {
        ErrPostEx(SEV_WARNING, 0, 0, "IntersectBlastGiLists: No lists to "
                "combine?");
        return NULL;
    }
    if (!list1) total1 = 0;
    if (!list2) total2 = 0;

    if (total1 > total2)
        short_list_sz = total2;
    else
        short_list_sz = total1;

    new_list = (BlastDoubleInt4Ptr)
        MemNew(short_list_sz*sizeof(BlastDoubleInt4));
    if (!new_list) {
        ErrPostEx(SEV_WARNING, 0, 0, "IntersectBlastGiLists: Failed to "
                "allocate memory for gi list");
        return NULL;
    }

    /* sort the two lists by gi to facilitate intersection */
    list_ptrs1 = (BlastDoubleInt4Ptr *)
                 Malloc(sizeof(BlastDoubleInt4Ptr)*total1);
    list_ptrs2 = (BlastDoubleInt4Ptr *)
                 Malloc(sizeof(BlastDoubleInt4Ptr)*total2);

    for (idx1 = 0; idx1 < total1; idx1++)
        list_ptrs1[idx1] = &(list1[idx1]);
    for (idx2 = 0; idx2 < total2; idx2++)
        list_ptrs2[idx2] = &(list2[idx2]);
    HeapSort(list_ptrs1, total1, sizeof(BlastDoubleInt4Ptr *),
             blast_double_int_gi_compare);
    HeapSort(list_ptrs2, total2, sizeof(BlastDoubleInt4Ptr *),
             blast_double_int_gi_compare);

    /* Intersect! */
    for (idx1 = 0, idx2 = 0; idx1 < total1; idx1++) {
        gi = list_ptrs1[idx1]->gi;

        for (; idx2 < total2 && list_ptrs2[idx2]->gi < gi; idx2++);

        if (idx2 < total2 && list_ptrs2[idx2]->gi == gi) {
            MemCpy((VoidPtr)&new_list[new_idx], list_ptrs1[idx1],
                   sizeof(BlastDoubleInt4));
            new_idx++;
        }
    }

    /* Save the intersection and return it */
    if (new_idx > 0)
        retval = BlastGiListNew(new_list, new_idx);

    if (list_ptrs1)
        MemFree(list_ptrs1);
    if (list_ptrs2)
        MemFree(list_ptrs2);
    MemFree(new_list);

    return retval;
}

/* Return the union of list1 and list2. Size of the return value will be
 * total1 + total2. This function does not eliminate repeated entries.
 * Caller is responsible for deallocating the return value.*/
static
BlastGiListPtr CombineDoubleInt4Lists(
                   BlastDoubleInt4Ptr list1, Int4 total1, 
                   BlastDoubleInt4Ptr list2, Int4 total2)
{
    BlastGiListPtr retval = NULL;
    BlastDoubleInt4Ptr new_list;
 
    if ((!list1 && !list2) || (total1 <= 0 && total2 <= 0))
        return NULL;
 
    if (!list1) total1 = 0;
    if (!list2) total2 = 0;
 
    new_list = (BlastDoubleInt4Ptr) 
               Malloc((total1+total2)*sizeof(BlastDoubleInt4));
    if (!new_list) {
        ErrPostEx(SEV_WARNING, 0, 0, "CombineDoubleInt4Lists: Failed to "
                "allocate memory for gi list");
        return NULL;
    }
    MemCpy(new_list, list1, total1*sizeof(BlastDoubleInt4));
    MemCpy(&(new_list[total1]), list2, total2*sizeof(BlastDoubleInt4));

    retval = BlastGiListNew(new_list, total1+total2);
    MemFree(new_list);

    return retval;
}

/* Intersects (or creates) oidlists and attaches them to individual rdfp's in
 * the rdfp_chain. If any rdfp->oidlist is populated before calling this
 * function, it will be freed and replaced by the intersection of the gilist
 * parameter and itself. */
static 
Boolean IntersectDoubleInt4ListWithOIDLists(
           BlastDoubleInt4Ptr gilist, Int4 total, 
           ReadDBFILEPtr rdfp_chain)
{
    OIDListPtr new_oidlist = NULL, lcl_oidlist = NULL;
    ReadDBFILEPtr rdfp = NULL;
    BlastDoubleInt4Ptr *gilist_ptrs = NULL;
    Int4 gilist_idx = 0; /* index into gilist_ptrs */
    Int4 i, new_oidlistsz = 0, new_oidlist_count = 0;
    Int4 maxoid, lcl_mask_index, lcl_oid; 
    Uint4 lcl_oid_bit = 0, lcl_mask = 0;

    gilist_ptrs = (BlastDoubleInt4Ptr *) 
                    Malloc(total*sizeof(BlastDoubleInt4Ptr));
    for (i=0; i < total; i++)
        gilist_ptrs[i] = &(gilist[i]);

    /* Sort by start field */
    HeapSort(gilist_ptrs, total, 
              sizeof(BlastDoubleInt4Ptr PNTR), blast_double_int_start_compare);

   /* merge gis from gilist and oids as you walk through rdfp_chain */
   for (rdfp = rdfp_chain; rdfp; rdfp = rdfp->next) {

       lcl_oidlist = rdfp->oidlist;

       while (gilist_idx < total &&
              gilist_ptrs[gilist_idx]->start == rdfp->start) {

           if (!new_oidlist) {
               /* determine the maxoid for this rdfp */
               for (i = gilist_idx; 
                    i < total && gilist_ptrs[i]->start == rdfp->start; 
                    i++);

               if (i == total && rdfp->start != gilist_ptrs[i-1]->start)
                   break;
               maxoid = gilist_ptrs[i-1]->ordinal_id - rdfp->start;
               
               new_oidlist = (OIDListPtr) MemNew(sizeof(OIDList));
               new_oidlist->total = maxoid + 1;
               new_oidlistsz = maxoid/MASK_WORD_SIZE + 2;
               new_oidlist->list = (Uint4Ptr) MemNew(
                                   (new_oidlistsz)*sizeof(Uint4));
               new_oidlist->memory = new_oidlist->list;
           }

           lcl_oid = gilist_ptrs[gilist_idx]->ordinal_id - rdfp->start;

           if (lcl_oid >= 0) {

               lcl_mask_index = lcl_oid/MASK_WORD_SIZE;
               lcl_oid_bit = 0x1 << (MASK_WORD_SIZE - 1 - 
                                       lcl_oid % MASK_WORD_SIZE);

               /* In a database with volumes and oidlists we might have empty 
                * masks (ie: month est subset in a multi-volume est real 
                * database), so reset the mask index to avoid accessing 
                * outside the bounds of the lcl_oidlist->list */
               if (lcl_oidlist) {
                   if (lcl_oidlist->total == 0)
                       lcl_mask_index = 0;
                   /* also remember the mask */
                   lcl_mask = SwapUint4(lcl_oidlist->list[lcl_mask_index]);
               }

               /* if there is an oidlist already, make sure this gi is in both
                * the gilist and the oidlist */
               if (!lcl_oidlist || 
                   (lcl_oidlist && lcl_oidlist->list &&
                   (lcl_oid < new_oidlist->total) && (lcl_mask & lcl_oid_bit))) 
               {
                   new_oidlist->list[lcl_mask_index] |= lcl_oid_bit;
                   new_oidlist_count++;
               }
           }
           gilist_idx++;
       }
       /* Save the newly created (or combined) oidlist */
       if (new_oidlist_count != 0) {
           for (i = 0; i < new_oidlistsz; i++)
               new_oidlist->list[i] = SwapUint4(new_oidlist->list[i]);
           rdfp->oidlist = OIDListFree(rdfp->oidlist);
           rdfp->oidlist = new_oidlist;
           new_oidlist = NULL;
           new_oidlist_count = 0;
       } else {
           new_oidlist = OIDListFree(new_oidlist);
       }
   }

   MemFree(gilist_ptrs);
    
   return TRUE;
}

/* The purpose of this function is to merge (or convert) any rdfp->gifile(s)
   in the rdfp_chain to rdfp->oidlist(s). These oidlists would be local to
   each rdfp (ie: if one gi list is given for a multivolume database, 
   multiple oidlists could be created corresponding to each of the rdfp in 
   the rdfp_chain).
   If there are any ordinal id lists present in the rdfp_chain, these will be
   intersected with the oidlists created from the gilists.
   The gis that were listed in each rdfp->gilist that belong to this database
   are returned in the bglp. Caller is responsible for deallocating this
   value.
*/
static
void MergeDbGiListsWithOIDLists(ReadDBFILEPtr rdfp_chain, ValNodePtr *err_ret,
        BlastGiListPtr *bglpp) 
{
    ReadDBFILEPtr rdfp = NULL;  /* holds an individual element of the
                                   rdfp_chain linked list */
    ReadDBFILEPtr rdfp_tmp = NULL; /* holds the rest of the rdfp linked list
                                      when searching for gis in a rdfp element
                                      */
    BlastDoubleInt4Ptr	list=NULL, global_list = NULL;
    BlastGiListPtr tmp_list = NULL;
    Int4	i, index, ngis = 0, total_num_gis = 0, start;
    Char buf[256];
 
    /* If this argument is not passed in, any gis from rdfp->gifile will
     * be lost for formatting purposes */
    if (!bglpp)
        return;

    /** 
     * Gather all gis from all rdfp->gilist(s).
     */
    for (rdfp = rdfp_chain; rdfp; rdfp = rdfp->next) {
        if (!rdfp->gilist)
           continue;
  
        ngis = rdfp->gilist->count;
        list = (BlastDoubleInt4Ptr) Malloc(sizeof(BlastDoubleInt4)*ngis);
        if (!list) {
            ErrPostEx(SEV_FATAL, 1, 0, "Out of memory");
            return;
        }

        /* Isolate the current rdfp element */
        rdfp_tmp = rdfp->next;
        rdfp->next = NULL;
           
        /* See which gis in rdfp->gilist belong to this rdfp */
        for (index=0, i=0; i < ngis; i++) {
            list[index].ordinal_id = readdb_gi2seq(rdfp_chain, 
                    rdfp->gilist->i[i], &start);
            if (list[index].ordinal_id >= 0) {
                list[index].gi = rdfp->gilist->i[i];
                list[index].start = start;
                index++;
            }
        }

        /* Restore the rdfp_chain linked list */
        rdfp->next = rdfp_tmp;

        /* After this point we don't really need the rdfp->gilist */
        rdfp->gilist = Int4ListFree(rdfp->gilist);

        /* No gis in rdfp->gilist belong to this rdfp, so restrict this entire
         * rdfp (mask all of its sequences) */
        if ( (ngis = index) == 0) {
            Int4 maxoid = rdfp->stop - rdfp->start;

            rdfp->oidlist = (OIDListPtr) MemNew(sizeof(OIDList));
            rdfp->oidlist->total = maxoid + 1;
            rdfp->oidlist->list = (Uint4Ptr)
                MemNew(sizeof(Uint4)*(maxoid/MASK_WORD_SIZE + 2));
            rdfp->oidlist->memory = rdfp->oidlist->list;
            sprintf(buf,"%s database restricted with gis not present in the "
                    "database. Restricting the entire database", 
                    FileNameFind(rdfp->filename));
            ErrPostEx(SEV_INFO, 0, 0, buf);
            /*BlastConstructErrorMessage("MergeDbGiListsWithOIDLists", buf, 
                    SEV_WARNING, err_ret);*/
            list = MemFree(list);
            continue;
        }
        
        tmp_list = CombineDoubleInt4Lists(global_list, total_num_gis, 
                                          list, ngis);
        if (tmp_list) {
            global_list = MemFree(global_list);
            global_list = tmp_list->gi_list;
            tmp_list->gilist_not_owned = TRUE;
            BlastGiListDestruct(tmp_list, TRUE);
            total_num_gis += ngis;
        }
        list = MemFree(list);

    }
    
    /**
     * If there are any lists, create (or intersect) oidlists for the 
     * appropriate rdfp's
     */
    if (global_list && total_num_gis > 0) {
 
        if (IntersectDoubleInt4ListWithOIDLists(global_list, total_num_gis,
                    rdfp_chain) == FALSE) {
            ErrPostEx(SEV_WARNING, 0, 0, "MergeDbGiListsWithOIDLists: Could "
                    "not intersect gi lists with oidlists");
            MemFree(global_list);
            return;
        }
  
        *bglpp = BlastGiListNew(global_list, total_num_gis);
        MemFree(global_list);
    }
    return;
}

/* Purpose: Create the virtual oidlist to limit this blast search.
   Parameters:
   bglp contains the list of gis to limit the search with. It is freed by this
   function.
   rdfp_chain must be the head of the linked list of rdfps attached to the 
   BlastSearchBlk structure. 
   oidlist_forall_rdfp determines whether the gis in bglp mask only
   certain rdfps or mask the entire rdfp_chain. 
   Finally, in the options parameter, the the gilist_already_calculated 
   field is a flag to determine whether the ordinal ids for the bglp should 
   be calculated. This is not the case for the neighboring software 
   (ie: nabrd.cpp), so we do not calculate them again. This assumes that *
   only* real databases are used and that the virtual oidlist is freed for 
   every subsequent invocation of the BLASTSetUpSearch function.
   The options->sort_gi_list flag is also used by the neighboring software
   where no sorting of the gi list (bglp parameter) is needed.

   The bglp parameter is used to construct the virtual oidlist. If there are
   any local oidlists in the rdfp_chain, these are intersected with the bglp.
   If the bglp is NULL, any oidlists in the rdfp will be consolidated into
   the virtual oidlist but these will not be part of the return value. If 
   there are no oidlists and bglp is NULL, the search is not limited at all.
*/
BlastGiListPtr 
BlastCreateVirtualOIDList(BlastGiListPtr bglp, ReadDBFILEPtr rdfp_chain,
                Boolean oidlist_forall_rdfp, BLAST_OptionsBlkPtr options)
{
    BlastDoubleInt4Ptr gilist, real_gilist = NULL, *real_gilist_ptrs = NULL;
    ReadDBFILEPtr rdfp, mask_rdfp = NULL; 
    OIDListPtr virtual_oidlist = NULL, lcl_oidlist = NULL;
    Int4 start, maxoid = 0, virtual_oidlistsz, real_ngis = 0, real_idx = 0;
    Int4 virtual_oid, virtual_mask_index, virtual_oid_bit;
    Int4 lcl_mask_index, lcl_bit, lcl_oid;
    Uint4 lcl_mask = 0;
    register Int4 i;

    /* initialize the start and oid fields of gilist, as well as maxoid */
    if (bglp) {
        gilist = bglp->gi_list;
        for (i = 0; i < bglp->total; i++) {
            if (!options->gilist_already_calculated && /* nabrd does this */
                gilist[i].ordinal_id <= 0) { /* assumes gilist was
                                                memset'd to zero */
                gilist[i].ordinal_id = readdb_gi2seq(rdfp_chain, gilist[i].gi, 
                                                 &start);
                gilist[i].start = start;
            }
            maxoid = MAX(maxoid, gilist[i].ordinal_id);
            if (gilist[i].ordinal_id != -1)
                real_ngis++; /* keep track of how many gis are actually found */
        }
    } else {
        for (rdfp = rdfp_chain; rdfp; rdfp = rdfp->next) {
            if (rdfp->oidlist)
                maxoid = MAX(maxoid, rdfp->oidlist->total+rdfp->start-1);
        }
    }

    if (maxoid == 0) /* no bglp or oidlists ? don't restrict the search */ {
        bglp = BlastGiListDestruct(bglp, TRUE);
        return NULL;
    }

    /* Allocate the virtual oidlist */
    if (!(virtual_oidlist = (OIDListPtr) MemNew(sizeof(OIDList)))) {
        ErrPostEx(SEV_WARNING, 0, 0,"BlastCreateVirtualOIDList: Could not "
                "allocate memory for global oidlist");
        return NULL;
    }
    virtual_oidlist->total = maxoid + 1;
    virtual_oidlistsz = maxoid/MASK_WORD_SIZE+2;
    virtual_oidlist->list = (Uint4Ptr) MemNew(virtual_oidlistsz*sizeof(Uint4));
    virtual_oidlist->memory = virtual_oidlist->list;
    if (!virtual_oidlist->list) {
        ErrPostEx(SEV_WARNING, 0, 0,"BlastCreateVirtualOIDList: Could not "
                "allocate memory for global oidlist");
        return NULL;
    }

    /* real_gilist will store the gilist that corresponds to the intersection
     * of the bglp and the oidlists in the rdfp_chain */
    if (real_ngis > 0) {
        real_gilist = (BlastDoubleInt4Ptr)
            MemNew(sizeof(BlastDoubleInt4)*real_ngis);
        real_gilist_ptrs = (BlastDoubleInt4Ptr *)
            MemNew(sizeof(BlastDoubleInt4 *)*real_ngis);
        if (!real_gilist || !real_gilist_ptrs) {
            ErrPostEx(SEV_WARNING, 0, 0, "BlastCreateVirtualOIDList: Out of "
                    "memory");
            MemFree(virtual_oidlist);
            return NULL;
        }
    }

    if (bglp) {
       Boolean first_gi_found = FALSE;
       Int4 j, virtual_oid1;


       /* Iterate through the gilist, initializing the virtual oidlist */
        for (i = 0; i < bglp->total; i++) {
            if ((virtual_oid = gilist[i].ordinal_id) < 0) 
                continue;

            for (rdfp = rdfp_chain; rdfp; rdfp = rdfp->next) {
               if (!first_gi_found && rdfp->start < gilist[i].start) {
                  if ((lcl_oidlist = rdfp->oidlist) && !oidlist_forall_rdfp) {

                     for (j = 0; j < lcl_oidlist->total; j++) {
                        lcl_mask_index = j/MASK_WORD_SIZE;
                        lcl_bit = 0x1 << 
                           (MASK_WORD_SIZE - 1 - j % MASK_WORD_SIZE);
                        virtual_oid1 = j + rdfp->start;
                        virtual_mask_index = virtual_oid1/MASK_WORD_SIZE;
                        virtual_oid_bit = 0x1 << 
                           (MASK_WORD_SIZE - 1 - virtual_oid1 % MASK_WORD_SIZE);

                        lcl_mask = 
                           SwapUint4(lcl_oidlist->list[lcl_mask_index]);
                        if (lcl_mask & lcl_bit) {
                           virtual_oidlist->list[virtual_mask_index] |=
                              virtual_oid_bit;
                        }
                     }
                  }
               } else if (rdfp->start == gilist[i].start) {
                  first_gi_found = TRUE;
                  break;
               }
            }
            if (!rdfp) continue;

            virtual_mask_index = virtual_oid/MASK_WORD_SIZE;
            virtual_oid_bit = 0x1 << (MASK_WORD_SIZE - 1 -
                                      virtual_oid % MASK_WORD_SIZE);
            if ((lcl_oidlist = rdfp->oidlist)) {
                lcl_oid = gilist[i].ordinal_id - gilist[i].start;
                lcl_mask_index = lcl_oid/MASK_WORD_SIZE;
                lcl_bit = 0x1 << (MASK_WORD_SIZE - 1 -
                                      lcl_oid % MASK_WORD_SIZE);
                if (lcl_oidlist->total == 0)
                    lcl_mask_index = 0;
            }

            if (!lcl_oidlist ||
                (lcl_oidlist && lcl_oidlist->list &&
                 lcl_oid < lcl_oidlist->total &&
                 (SwapUint4(lcl_oidlist->list[lcl_mask_index])&lcl_bit))) 
            {
                virtual_oidlist->list[virtual_mask_index] |= virtual_oid_bit;
                MemCpy((VoidPtr) &real_gilist[real_idx], (VoidPtr) &gilist[i],
                        sizeof(BlastDoubleInt4));
                real_idx++;
            }
        }
    } else {
        /* Create virtual oidlist by combining existing oidlists */

        for (rdfp = rdfp_chain; rdfp; rdfp = rdfp->next) {

            if (!(lcl_oidlist = rdfp->oidlist) || oidlist_forall_rdfp)
                continue;

            for (i = 0; i < lcl_oidlist->total; i++) {
                lcl_mask_index = i/MASK_WORD_SIZE;
                lcl_bit = 0x1 << (MASK_WORD_SIZE - 1 - i % MASK_WORD_SIZE);
                virtual_oid = i + rdfp->start;
                virtual_mask_index = virtual_oid/MASK_WORD_SIZE;
                virtual_oid_bit = 0x1 << (MASK_WORD_SIZE - 1 -
                        virtual_oid % MASK_WORD_SIZE);

                lcl_mask = SwapUint4(lcl_oidlist->list[lcl_mask_index]);
                if (lcl_mask & lcl_bit) {
                    virtual_oidlist->list[virtual_mask_index] |=
                        virtual_oid_bit;
                }
            }
        }
    }
    for (i = 0; i < virtual_oidlistsz; i++)
        virtual_oidlist->list[i] = SwapUint4(virtual_oidlist->list[i]);
    
    /* Determine the first rdfp with an oidlist, and free the local oidlists */
    for (rdfp = rdfp_chain; rdfp; rdfp = rdfp->next) {
        if (rdfp->oidlist && !mask_rdfp)
            mask_rdfp = rdfp;
        rdfp->oidlist = OIDListFree(rdfp->oidlist);
    }

    /* attach mask to appropriate place */
    if (oidlist_forall_rdfp && real_ngis > 0) {
        rdfp_chain->oidlist = virtual_oidlist;
    } else {
        if (mask_rdfp)
            mask_rdfp->oidlist = virtual_oidlist;
        else {
            /* Should never happen */
            ErrPostEx(SEV_ERROR, 0, 0, "BlastCreateVirtualOIDList: Missing "
                    "oidlists to attach virtual_oidlist");
            OIDListFree(virtual_oidlist);
            return NULL;
        }
    }

    /* Discard bglp and use real_gilist to store the gilist that corresponds
     * to the virtual oidlist */
    if (bglp) {
        bglp = BlastGiListDestruct(bglp, TRUE);
        for (i = 0; i < real_idx; i++)
            real_gilist_ptrs[i] = &(real_gilist[i]);
        if (options->sort_gi_list == TRUE) {
            /* sort by oid to allow quick searches in BlastGetAllowedGis */
            HeapSort(real_gilist_ptrs, real_idx,
                 sizeof(BlastDoubleInt4Ptr PNTR), blast_double_int_oid_compare);
        }
        gilist = (BlastDoubleInt4Ptr)MemNew(real_idx*sizeof(BlastDoubleInt4));
        for (i = 0; i < real_idx; i++)
            MemCpy((VoidPtr)&gilist[i], real_gilist_ptrs[i],
                    sizeof(BlastDoubleInt4));
        bglp = BlastGiListNew(gilist, real_idx);
        MemFree(real_gilist);
        MemFree(real_gilist_ptrs);
        MemFree(gilist);
    }

    return bglp;
}

/* These are the criteria that can restrict a blast search:
   a) OIDListPtr rdfp->oidlist;
   b) Int4ListPtr rdfp->gilist;
   c) ValNodePtr options->gilist;
   d) CharPtr options->gifile;
   e) BlastDoubleInt4Ptr gi_list;
   
   The policy to restrict blast searches follows:
   All rdfp->gilist(s) are merged (union). The resulting gilist is then 
   intersected with any rdfp->oidlist(s) in (a)
   (see MergeDbGiListsWithOIDLists). Gis from rdfp->gilist(s) will be returned
   so that they can be added to the search->thr_info->blast_gi_list structure.

   If non-NULL, (c), (d), and (e) are intersected and this result is 
   intersected with the gilist obtained previously.

   A single 'virtual' oidlist is created and attached to the first rdfp in the
   search->rdfp chain that has been restricted (see BlastCreateVirtualOIDList).
    
   The resulting gi list from (b), (c), (d), and (e) is attached to the 
   BlastGiListPtr search->thr_info->blast_gi_list field. The definite source
   for the actual search resides in the virtual oidlist (if any).
*/
void
BlastProcessGiLists(BlastSearchBlkPtr search, BLAST_OptionsBlkPtr options,
                    BlastDoubleInt4Ptr gi_list, Int4 gi_list_size)
{
    BlastGiListPtr bglp = NULL, bglp_tmp = NULL;
    BlastDoubleInt4Ptr tmp_list = NULL;
    ValNodePtr vnp;
    Int4 ngis = 0;
    /* determine if final oidlist should cover all rdfps or it should start
     * with the first rdfp that had a rdfp->oidlist or a rdfp->gilist */
    Boolean oidlist_forall_rdfp = FALSE; 

    /* if any of the following parameters is non-NULL, we need to create an 
     * oidlist that covers all rdfp(s) in search->rdfp. */
    if (gi_list || options->gifile || options->gilist)
        oidlist_forall_rdfp = TRUE;

    /* Create individual oidlists for those databases which have gi lists */
    MergeDbGiListsWithOIDLists(search->rdfp, &search->error_return, &bglp);

    if (gi_list)
        bglp = CombineDoubleInt4Lists(gi_list, gi_list_size, NULL, 0);

    if (options->gifile) {

        if ((tmp_list = GetGisFromFile(options->gifile, &ngis))) {
            if (bglp) {
                bglp_tmp = IntersectBlastGiLists(tmp_list, ngis,
                            bglp->gi_list, bglp->total);
            } else {
                bglp_tmp = CombineDoubleInt4Lists(tmp_list, ngis, NULL, 0);
            }

            BlastGiListDestruct(bglp, TRUE);
            bglp = bglp_tmp;
            bglp_tmp = NULL;
            tmp_list = (BlastDoubleInt4Ptr) MemFree(tmp_list);
            ngis = 0;
        }
    }

    /* this is very inefficient, needs to be changed in ASN.1 spec? */
    if (options->gilist) {
        Int4 alloc_chunk = 1024, alloc = 1024; 

        tmp_list = (BlastDoubleInt4Ptr) MemNew(sizeof(BlastDoubleInt4)*alloc);
        if (!tmp_list) {
            ErrPostEx(SEV_WARNING, 0, 0, "BlastProcessGiLists: Out of memory");
            BlastGiListDestruct(bglp, TRUE);
            return ;
        }

        for (vnp = options->gilist; vnp; vnp = vnp->next) {
            if (ngis >= alloc) {
                alloc += alloc_chunk;
                tmp_list = (BlastDoubleInt4Ptr) 
                    Realloc(tmp_list, sizeof(BlastDoubleInt4)*alloc);
                MemSet(&tmp_list[ngis], 0, alloc_chunk*sizeof(BlastDoubleInt4));
            }
            tmp_list[ngis++].gi = vnp->data.intvalue;
        }

        if (bglp) {
            bglp_tmp = IntersectBlastGiLists(tmp_list, ngis,
                       bglp->gi_list, bglp->total);
        } else {
            /* (options->gifile intersection gi_list) resulted in an empty
             * set, then we don't recreate bglp. */
            if (!options->gifile && !gi_list) 
                bglp_tmp = CombineDoubleInt4Lists(tmp_list, ngis, NULL, 0);
        }

        BlastGiListDestruct(bglp, TRUE);
        bglp = bglp_tmp;
        bglp_tmp = NULL;
        tmp_list = (BlastDoubleInt4Ptr) MemFree(tmp_list);
    }

    /* Save the final gi list for formatting purposes */
    search->thr_info->blast_gi_list = 
        BlastCreateVirtualOIDList(bglp, search->rdfp, oidlist_forall_rdfp,
                options);
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
           ErrPostEx(SEV_FATAL, 1, 0, "Matrix has positive average score");
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
      if (hp && (hp->num_in_heap < hsp_range_max || small(hp->heap[0], hsp))) {
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
      if (hp && (hp->num_in_heap < hsp_range_max || small(hp->heap[0], hsp))) {
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
    Int4 index;
    Int4 begin, end, i, hspcnt, query_length;
    Int2 context;
    BLASTHeapPtr hp;

    if (search->pbp->perform_culling == FALSE || 
        search->pbp->mb_params)  /* Culling is turned off. */
        return;

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
      if (hp)
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
    ErrPostEx(SEV_FATAL, 1, 0, "The combination %d for gap opening cost and %d for gap extension is not supported in PHI-BLAST with matrix %s\n", options->gap_open, options->gap_extend, matrixName);
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
      ErrPostEx(SEV_FATAL, 1, 0, "The combination %d for gap opening cost and %d for gap extension is not supported in PHI-BLAST with matrix %s\n", options->gap_open, options->gap_extend, matrixName);
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
	ErrPostEx(SEV_FATAL, 1, 0, "The combination %d for gap opening cost and %d for gap extension is not supported in PHI-BLAST with matrix %s\n", options->gap_open, options->gap_extend, matrixName);
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
	  ErrPostEx(SEV_FATAL, 1, 0, "The combination %d for gap opening cost and %d for gap extension is not supported in PHI-BLAST with matrix %s\n", options->gap_open, options->gap_extend, matrixName);
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
	    ErrPostEx(SEV_FATAL, 1, 0, "The combination %d for gap opening cost and %d for gap extension is not supported in PHI-BLAST with matrix %s\n", options->gap_open, options->gap_extend, matrixName);

	  }
	  else {
	    ErrPostEx(SEV_FATAL, 1, 0, "Matrix %s not allowed in PHI-BLAST\n", matrixName);
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
	    if (ErrorMessage)
	    	*ErrorMessage = StringSave(message);
	    return FALSE;
	} else {
	    chptr++;
	}

	/* checking index in options table */

	if((index = BlastGetLetterIndex(letters, *chptr)) < 0) {
	    sprintf(message, "Character \'%c\' is not a valid option", 
		    *chptr);
	    if (ErrorMessage)
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


void BLASTAddBlastDBTitleToSeqAnnotEx(SeqAnnotPtr seqannot, CharPtr title, Boolean is_na)
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
    ufp->data.boolvalue = is_na; /* Nucleotide if TRUE. */
    
    ValNodeAddPointer(&(seqannot->desc), Annot_descr_user, (Pointer)uop);
}

/* BLASTAddBlastDBTitleToSeqAnnotEx is preferred if the type of database is known. */

void BLASTAddBlastDBTitleToSeqAnnot(SeqAnnotPtr seqannot, CharPtr title)
{
	BLASTAddBlastDBTitleToSeqAnnotEx(seqannot, title, TRUE);
	return;
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
                                                       is_ooframe, NULL);

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
		StringNCpy_0(ptr, string, BLAST_ERROR_BULEN);

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
	    	StringNCpy_0(ptr, textid, BLAST_ERROR_BULEN-1);
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

/* 
   Function to print results in tab-delimited format, given a SeqAlign list.
   q_shift and s_shift are the offsets in query and subject in case of a
   subsequence search 
*/
void BlastPrintTabulatedResults(SeqAlignPtr seqalign, BioseqPtr query_bsp,
                                SeqLocPtr query_slp, Int4 num_alignments, 
                                CharPtr blast_program, Boolean is_ungapped, 
                                Boolean believe_query, Int4 q_shift, 
                                Int4 s_shift, FILE *fp,
                                Boolean print_query_info)
{
   BlastPrintTabulatedResultsEx(seqalign, query_bsp, query_slp, num_alignments,
                                blast_program, is_ungapped, believe_query,
                                q_shift, s_shift, fp, NULL, print_query_info);
}

void BlastPrintTabulatedResultsEx(SeqAlignPtr seqalign, BioseqPtr query_bsp,
                                SeqLocPtr query_slp, Int4 num_alignments, 
                                CharPtr blast_program, Boolean is_ungapped, 
                                Boolean believe_query, Int4 q_shift, 
                                Int4 s_shift, FILE *fp, 
                                int *num_formatted, Boolean print_query_info)
{
   BlastPrintTabularResults(seqalign, query_bsp, query_slp, num_alignments,
      blast_program, is_ungapped, FALSE, believe_query,
      q_shift, s_shift, fp, num_formatted, print_query_info);
}

void BlastPrintTabularResults(SeqAlignPtr seqalign, BioseqPtr query_bsp,
        SeqLocPtr query_slp, Int4 num_alignments, CharPtr blast_program, 
        Boolean is_ungapped, Boolean is_ooframe, Boolean believe_query, 
        Int4 q_shift, Int4 s_shift, FILE *fp, int *num_formatted, 
        Boolean print_query_info)
{
   SeqAlignPtr sap, sap_tmp = NULL;
   FloatHi perc_ident, bit_score, evalue;
   Int4 numseg, num_gap_opens, num_mismatches, num_ident, score;
   Int4 number, align_length, index, i;
   Int4 q_start, q_end, s_start, s_end;
   Char bit_score_buff[10];
   CharPtr eval_buff;
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
   Int4 alignments_count;

   is_translated = (StringCmp(blast_program, "blastn") &&
                    StringCmp(blast_program, "blastp"));
   
   if (is_translated) {
      asp = MemNew(sizeof(AlignSum));
      asp->matrix = load_default_matrix();
      asp->is_aa = TRUE;
      asp->ooframe = is_ooframe;
   }

   if (is_ungapped)
      sap_tmp = SeqAlignNew();

   slp = query_slp;
   if (query_bsp)
      query_id = query_bsp->id;

   /* Evalue buffer is dynamically allocated to avoid compiler warnings 
      in calls to ScoreAndEvalueToBuffers. */
   eval_buff = Malloc(10);

   for (sap = seqalign; sap; sap = sap->next) {
      if (query_slp)
         query_id = TxGetQueryIdFromSeqAlign(sap);
      if (SeqIdComp(query_id, old_query_id) != SIC_YES) {
         if (old_query_id && num_formatted)
            (*num_formatted)++;
         alignments_count = num_alignments;
         /* New query: find the corresponding SeqLoc */
         while (slp && SeqIdComp(query_id, SeqLocId(slp)) != SIC_YES)
            slp = slp->next;
         if (slp != NULL) {
            query_id = old_query_id = SeqLocId(slp);
            /* Print new query information */
            if (print_query_info)
               PrintTabularOutputHeader(NULL, NULL, slp, NULL, 0, 
                                        believe_query, fp);
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
               StringNCpy_0(query_buffer, defline, BUFFER_LENGTH);
               defline = MemFree(defline);
            } else
               StringCpy(query_buffer, defline+4);
            defline = MemFree(defline);
         } else
            StringCpy(query_buffer, defline+4);
      } else
         query_id = old_query_id;      

      subject_id = TxGetSubjectIdFromSeqAlign(sap);

      if (SeqIdComp(subject_id, old_subject_id) != SIC_YES) {
         /* New subject sequence has been found in the seqalign list */
         if (--alignments_count < 0)
            continue;
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
      
      perc_ident = 0;
      align_length = 0;
      num_gap_opens = 0;
      num_mismatches = 0;

      GetScoreAndEvalue(sap, &score, &bit_score, &evalue, &number);

      /* Do not allow knocking off digit in evalue buffer, so parsers are 
         not confused. */
      ScoreAndEvalueToBuffers(bit_score, evalue, 
                              bit_score_buff, &eval_buff, 0);

      /* Loop on segments within this seqalign (in ungapped case) */
      while (TRUE) {
         if (sap->segtype == SAS_DENSEG) {
            Boolean get_num_ident = TRUE;
            dsp = (DenseSegPtr) sap->segs;
            numseg = dsp->numseg;
            /* Query Bioseq is needed for calculating number of identities.
               NB: even if number of identities is already filled in the 
               seqalign score list, that is not enough here, because we need to
               know number of identities in each segment in order to calculate
               number of mismatches correctly. */
            if (!query_bsp) {
               query_bsp = BioseqLockById(query_id);
            }

            for (i=0; i<numseg; i++) {
               align_length += dsp->lens[i];
               if (dsp->starts[2*i] != -1 && dsp->starts[2*i+1] != -1) {
                  if (get_num_ident) {
                     num_ident = BlastBioseqGetNumIdentical(query_bsp, subject_bsp, 
                                    dsp->starts[2*i], dsp->starts[2*i+1], 
                                    dsp->lens[i], dsp->strands[2*i], 
                                    dsp->strands[2*i+1]);
                     perc_ident += num_ident;
                     num_mismatches += dsp->lens[i] - num_ident;
                  }
               } else {
                  num_gap_opens++;
               }
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
                                       bit_score_buff, &eval_buff, 0);
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
                                    bit_score_buff, &eval_buff, 0);

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
            /* Query Bioseq is needed for calculating number of identities.
               NB: even if number of identities is already filled in the 
               seqalign score list, that is not enough here, because we need to
               know number of identities in each segment in order to calculate
               number of mismatches correctly. */
            if (!query_bsp) {
               query_bsp = BioseqLockById(query_id);
            }

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

   eval_buff = MemFree(eval_buff);

   if (is_ungapped)
      sap_tmp = MemFree(sap_tmp);

   if (is_translated) {
      free_default_matrix(asp->matrix);
      MemFree(asp);
   }

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
   Char bit_score_buff[10];
   CharPtr eval_buff = NULL;
   Int4 length=0, query_length, subject_length=0, rev_subject_length=0;
   Int4 q_start, q_end, s_start, s_end, q_shift=0, s_shift=0;
   CharPtr subject_descr = NULL;
   Int4 cutoff_s;
   Boolean is_na, is_translated, db_is_na;
   Int4 hspcnt, number, numseg;
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
   
   hsp_array = search->current_hitlist->hsp_array;

   subject_id = GetTheSeqAlignID(sip);
   sip = SeqIdSetFree(sip);
   if (search->prog_number != blast_type_tblastx && 
       search->pbp->gapped_calculation)
      RealBlastGetGappedAlignmentTraceback(search, subject_seq, subject_length, rev_subject, rev_subject_length, subject_id, hsp_array, hspcnt, &seqalign, NULL, cutoff_s, FALSE, search->subject_id, TRUE);
   else {
      SeqIdPtr new_sip, gi_list=NULL;
      StdSegPtr ssp;

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

   if (is_translated || !search->pbp->gapped_calculation) {
      asp = MemNew(sizeof(AlignSum));
      asp->matrix = NULL;
      asp->matrix = load_default_matrix();
      asp->is_aa = !is_na;
      if (is_translated)
         AdjustOffSetsInSeqAlign(seqalign, search->query_slp, subject_slp);
   }

   /* Evalue buffer is dynamically allocated to avoid compiler warnings 
      in calls to ScoreAndEvalueToBuffers. */
   eval_buff = Malloc(10);

   /* Now print the tab-delimited fields, using seqalign */
   for (sap = seqalign; sap; sap = sap->next) {
      perc_ident = 0;
      align_length = 0;
      num_gap_opens = 0;
      num_mismatches = 0;

      GetScoreAndEvalue(sap, &score, &bit_score, &evalue, &number);

      /* Do not allow knocking off digit in evalue buffer, so parsers are 
         not confused. */
      ScoreAndEvalueToBuffers(bit_score, evalue, 
                              bit_score_buff, &eval_buff, 0);

      query_seq = search->context[search->first_context].query->sequence;

      if (!is_translated && search->pbp->gapped_calculation) {
         DenseSegPtr dsp = (DenseSegPtr) sap->segs;
         Boolean reverse = (dsp->strands[0] != dsp->strands[1]);
         numseg = dsp->numseg;
         
         for (i=0; i<numseg; i++) {
            align_length += dsp->lens[i];
            if (dsp->starts[2*i] != -1 && dsp->starts[2*i+1] != -1) {
               if (reverse) {
                  q_start = 
                     query_length - dsp->starts[2*i] - dsp->lens[i] + 1;
               } else {
                  q_start = dsp->starts[2*i];
               }
               num_ident = BlastGetNumIdentical(query_seq, subject_seq, 
                  q_start, dsp->starts[2*i+1], dsp->lens[i], FALSE);
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
         } else if (reverse) {
            q_start = dsp->starts[0];
            q_end = dsp->starts[2*numseg-2] + dsp->lens[numseg-1] - 1;
            s_end = dsp->starts[2*numseg-1] + dsp->lens[numseg-1];
            s_start = dsp->starts[1]+1;
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

   eval_buff = MemFree(eval_buff);

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

static void 
FillSequenceBuffers(Uint1* query_seq, Uint1* subject_seq, 
                    Int4* starts, Int4* lengths, Int4 numseg,
                    char* query_buffer, char* subject_buffer, 
                    Boolean reverse)
{
   Int4 index, index1;
   const char* blastna_to_iupacna     = "ACGTRYMKWSBDHVN-";
   const char* blastna_to_iupacna_rev = "TGCAYRKMSWVHDBN-"; 
   Uint1* query_ptr;
   Uint1* subject_ptr;
   Int4 offset;
   char* buffer;

   offset = 0;
   if (!reverse) {
      for (index = 0; index < numseg; ++index) {
         buffer = &query_buffer[offset];
         if (starts[2*index] != -1) {
            query_ptr = &query_seq[starts[2*index]];
            for (index1 = 0; index1 < lengths[index]; ++index1) {
               *buffer = blastna_to_iupacna[*query_ptr];
               buffer++;
               query_ptr++;
            }
         } else {
            memset(buffer, '-', lengths[index]);
         }
         buffer = &subject_buffer[offset];
         if (starts[2*index+1] != -1) {
            subject_ptr = &subject_seq[starts[2*index+1]];
            for (index1 = 0; index1 < lengths[index]; ++index1) {
               *buffer = blastna_to_iupacna[*subject_ptr];
               buffer++;
               subject_ptr++;
            }
         } else {
            memset(buffer, '-', lengths[index]);
         }
         offset += lengths[index];
      }
   } else {
      for (index = numseg-1; index >=0; --index) {
         buffer = &query_buffer[offset];
         if (starts[2*index] != -1) {
            query_ptr = &query_seq[starts[2*index]+lengths[index]-1];
            for (index1 = 0; index1 < lengths[index]; ++index1) {
               *buffer = blastna_to_iupacna_rev[*query_ptr];
               buffer++;
               query_ptr--;
            }
         } else {
            memset(buffer, '-', lengths[index]);
         }
         buffer = &subject_buffer[offset];
         if (starts[2*index+1] != -1) {
            subject_ptr = &subject_seq[starts[2*index+1]+lengths[index]-1];
            for (index1 = 0; index1 < lengths[index]; ++index1) {
               *buffer = blastna_to_iupacna_rev[*subject_ptr];
               buffer++;
               subject_ptr--;
            }
         } else {
            memset(buffer, '-', lengths[index]);
         }
         offset += lengths[index];
      }
   }
}

int LIBCALLBACK
MegaBlastPrintAlignInfo(VoidPtr ptr)
{
   BlastSearchBlkPtr search = (BlastSearchBlkPtr) ptr;
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
   Char bit_score_buff[10];
   CharPtr eval_buff = NULL;
   GapXEditScriptPtr esp;
   Int4 q_start, q_end, s_start, s_end, query_length, numseg;
   Int4 q_off, q_shift = 0, s_off, s_shift = 0;
   Int4Ptr length, start;
   Uint1Ptr strands;
   CharPtr subject_descr=NULL, subject_buffer, buffer;
   Boolean numeric_sip_type = FALSE;
   FILE *fp = (FILE *) search->output;
   CharPtr query_seq_buffer, subject_seq_buffer;
   Uint1Ptr query_ptr, subject_ptr;
   Boolean print_sequences;

   if (search->current_hitlist == NULL || search->current_hitlist->hspcnt <= 0) {
      search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
      return 0;
   }

   print_sequences = (getenv("PRINT_SEQUENCES") != NULL);

   subject_seq = search->subject->sequence_start + 1;

   if (search->rdfp) {
      readdb_get_descriptor(search->rdfp, search->subject_id, &sip,
                            &subject_descr);
   } else 
      sip = SeqIdSetDup(search->subject_info->sip);

   if (!search->pbp->mb_params->full_seqids)
      subject_id = SeqIdFindBestAccession(sip);
   else 
      subject_id = sip;
   
   if (subject_id->choice == SEQID_GI) {
      SeqIdPtr new_subject_seqid = NULL;
      ValNodePtr gi_list = 
         BlastGetAllowedGis(search, search->subject_id, &new_subject_seqid);
      
      if (gi_list) {
         /* change subject's gi with this 'use_this_gi' gi */
         subject_id->data.intvalue = gi_list->data.intvalue;
      }
   }

   if (subject_id->choice != SEQID_GENERAL ||
       StringCmp(((DbtagPtr)subject_id->data.ptrvalue)->db, "BL_ORD_ID")) {
      if (search->pbp->mb_params->full_seqids) { 
         subject_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         SeqIdWrite(subject_id, subject_buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
      } else {
         numeric_sip_type = 
            GetAccessionVersionFromSeqId(subject_id, &subject_gi, 
                                         &subject_buffer, TRUE);
      }
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

   /* Evalue buffer is dynamically allocated to avoid compiler warnings 
      in calls to ScoreAndEvalueToBuffers. */
   eval_buff = Malloc(10);

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

      query_length = search->query_context_offsets[context+1] -
         search->query_context_offsets[context] - 1;

      query_seq = search->context[context].query->sequence;
      kbp = search->sbp->kbp[context];

      /* If traceback hasn't been done yet, do it now */
      if (!hsp->gap_info) {
         /* This must have already been allocated */
         GapAlignBlkPtr gap_align = search->gap_align; 
         FloatHi searchsp_eff;
         Int4 max_offset, max_start = MAX_DBSEQ_LEN / 2, start_shift;

         /* Set the X-dropoff to the final X dropoff parameter. */
         gap_align->x_parameter = search->pbp->gap_x_dropoff_final;
         gap_align->query = query_seq;
         gap_align->query_length = query_length;
         gap_align->subject = subject_seq;
         gap_align->subject_length = search->subject->length;
         if ((hsp->query.gapped_start == 0 && hsp->subject.gapped_start == 0) ||
             CheckStartForGappedAlignment(search, hsp, gap_align->query, gap_align->subject, search->sbp->matrix) == FALSE) {
            max_offset = GetStartForGappedAlignment(search, hsp, gap_align->query, gap_align->subject, search->sbp->matrix);
            gap_align->q_start = max_offset;
            gap_align->s_start = (hsp->subject.offset - hsp->query.offset) + max_offset;
            hsp->query.gapped_start = gap_align->q_start;
            hsp->subject.gapped_start = gap_align->s_start;
         } else {
            gap_align->q_start = hsp->query.gapped_start;
            gap_align->s_start = hsp->subject.gapped_start;
         }
            
         if (gap_align->s_start > max_start) {
            start_shift = (gap_align->s_start / max_start) * max_start;
            gap_align->subject = gap_align->subject + start_shift;
            
            gap_align->s_start %= max_start;
         } else
            start_shift = 0;
         
         gap_align->subject_length =
            MIN(gap_align->subject_length - start_shift, 
                gap_align->s_start + hsp->subject.length + max_start);
         PerformGappedAlignmentWithTraceback(gap_align);
         hsp->query.offset = gap_align->query_start;
         hsp->subject.offset = gap_align->subject_start + start_shift;
         /* The end is one further for BLAST than for the gapped align. */
         hsp->query.end = gap_align->query_stop + 1;
         hsp->subject.end = gap_align->subject_stop + 1 + start_shift;
         if (gap_align->edit_block && start_shift > 0) {
            gap_align->edit_block->start2 += start_shift;
            gap_align->edit_block->length2 += start_shift;
         }
         hsp->query.length = hsp->query.end - hsp->query.offset;
         hsp->subject.length = hsp->subject.end - hsp->subject.offset;
         hsp->score = gap_align->score;
         hsp->gap_info = gap_align->edit_block;
         searchsp_eff = (FloatHi) search->dblen_eff *
            (FloatHi) search->context[hsp->context].query->effective_length;
         
         hsp->evalue = 
            BlastKarlinStoE_simple(hsp->score, kbp, searchsp_eff);
         if (hsp->evalue > search->pbp->cutoff_e) {
            continue;
         }
      }

      if (query_id->choice == SEQID_LOCAL && 
          search->pbp->mb_params->full_seqids) {
         Int4 local_index;
         if (GetAccessionVersionFromSeqId(query_id, &local_index,
                                          &query_buffer, TRUE)) {
            /* If local id is numeric, it is a fake id, so try to retrieve the
               real one */
            BioseqPtr query_bsp = BioseqLockById(query_id);
            title = StringSave(BioseqGetTitle(query_bsp));
            if (title)
               query_buffer = StringTokMT(title, " ", &title);
            else /* Failed to find any string for this id */
               query_buffer = StringSave("QUERY");
            BioseqUnlock(query_bsp);
         }
      } else {
         query_buffer = (CharPtr) Malloc(BUFFER_LENGTH + 1);
         if (!search->pbp->mb_params->full_seqids) {
            SeqIdWrite(SeqIdFindBestAccession(query_id), query_buffer,
                       PRINTID_TEXTID_ACC_VER, BUFFER_LENGTH);
         } else {
            SeqIdWrite(query_id, query_buffer, PRINTID_FASTA_LONG,
                       BUFFER_LENGTH);
         }
      }

      bit_score = (hsp->score*kbp->Lambda - kbp->logK) / NCBIMATH_LN2;

      esp = hsp->gap_info->esp;
        
      for (numseg=0; esp; esp = esp->next, numseg++);

      q_off = hsp->query.offset;
      s_off = hsp->subject.offset;

      GXECollectDataForSeqalign(hsp->gap_info, hsp->gap_info->esp, numseg,
				&start, &length, &strands, 
				&q_off, &s_off);

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
	 if (start[2*i] != -1 && start[2*i+1] != -1) {
	    num_ident = MegaBlastGetNumIdentical(query_seq, subject_seq, 
                                                 start[2*i], start[2*i+1], 
                                                 length[i], FALSE);
            perc_ident += num_ident;
            num_mismatches += length[i] - num_ident;
         } else {
            num_gap_opens++;
         }
         align_length += length[i];
      }
      perc_ident = perc_ident / align_length * 100;
      /* Avoid printing 100.00 when the hit is not an exact match */
      if (perc_ident >= 99.995 && perc_ident < 100.00)
         perc_ident = 99.99;

      if (perc_ident < search->pbp->mb_params->perc_identity) {
         MemFree(start);
         MemFree(length);
         MemFree(strands);
         MemFree(query_buffer);
         continue;
      }

      if (print_sequences) {
         /* Fill the query and subject sequence buffers */
         query_seq_buffer = MemNew((align_length+1));
         subject_seq_buffer = MemNew((align_length+1));
         FillSequenceBuffers(query_seq, subject_seq, start, length, numseg, 
                             query_seq_buffer, subject_seq_buffer, 
                             (context%2 == 1));
      }

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

      /* Do not allow knocking off digit in evalue buffer, so parsers are 
         not confused. */
      ScoreAndEvalueToBuffers(bit_score, hsp->evalue, 
                              bit_score_buff, &eval_buff, 0);

      if (print_sequences) {
         if (numeric_sip_type) {
            fprintf(fp, 
       "%s\t%ld\t%.2f\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%s\t%s\t%s\t%s\n",
               query_buffer, (long) subject_gi, perc_ident, 
               (long) align_length, (long) num_mismatches, 
               (long) num_gap_opens, (long) q_start, (long) q_end, 
               (long) s_start, (long) s_end, eval_buff, bit_score_buff,
               query_seq_buffer, subject_seq_buffer);
         } else {
            fprintf(fp, 
        "%s\t%s\t%.2f\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%s\t%s\t%s\t%s\n",
               query_buffer, subject_buffer, perc_ident, 
               (long) align_length, (long) num_mismatches, 
               (long) num_gap_opens, (long) q_start, (long) q_end, 
               (long) s_start, (long) s_end, eval_buff, bit_score_buff,
               query_seq_buffer, subject_seq_buffer);
         }
         MemFree(query_seq_buffer);
         MemFree(subject_seq_buffer);
      } else {
         if (numeric_sip_type) {
            fprintf(fp, 
               "%s\t%ld\t%.2f\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%s\t%s\n",
               query_buffer, (long) subject_gi, perc_ident, 
               (long) align_length, (long) num_mismatches, 
               (long) num_gap_opens, (long) q_start, (long) q_end, 
               (long) s_start, (long) s_end, eval_buff, bit_score_buff);
         } else {
            fprintf(fp, 
               "%s\t%s\t%.2f\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\t%s\t%s\n",
               query_buffer, subject_buffer, perc_ident, 
               (long) align_length, (long) num_mismatches, 
               (long) num_gap_opens, (long) q_start, (long) q_end, 
               (long) s_start, (long) s_end, eval_buff, bit_score_buff);
         }
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
   MemFree(eval_buff);

   sip = SeqIdSetFree(sip);
   fflush(fp);
   return 0;
}

void PrintTabularOutputHeader(CharPtr blast_database, BioseqPtr query_bsp,
                              SeqLocPtr query_slp, CharPtr blast_program,
                              Int4 iteration, Boolean believe_query,
                              FILE *outfp)
{
   Char buffer[BUFFER_LENGTH+1];
   Boolean unlock_bioseq = FALSE;

   asn2ff_set_output(outfp, NULL);
   
   ff_StartPrint(0, 0, BUFFER_LENGTH, NULL);

   if (blast_program) {
      CharPtr program = StringSave(blast_program);
      Nlm_StrUpper(program);
      sprintf(buffer, "# %s %s [%s]", program, BlastGetVersionNumber(),
              BlastGetReleaseDate());
      MemFree(program);
      ff_AddString(buffer);
      NewContLine();
   }

   if (iteration > 0) {
      ff_AddString("# Iteration: ");
      ff_AddString(Ltostr((long) iteration, 1));
      NewContLine();
   }

   if (query_bsp || query_slp) {
      CharPtr title;
      const CharPtr str = "# Query: ";
      Int4 string_length = StrLen(str);

      ff_AddString(str);

      if (!query_bsp) {
         Int4 num_queries = ValNodeLen(query_slp);
         if (num_queries > 1) {
            /* Multiple queries: just print the number, without deflines. */
            sprintf(buffer, "%ld sequences", (long)num_queries);
            ff_AddString(buffer);
         } else {
            query_bsp = BioseqLockById(SeqLocId(query_slp));
            unlock_bioseq = TRUE;
         }
      }
      if (query_bsp) {
         if (query_bsp->id && believe_query) {
            SeqIdWrite(query_bsp->id, buffer, PRINTID_FASTA_LONG, 
                       BUFFER_LENGTH);
            if (StringNCmp(buffer, "lcl|", 4) == 0) {
               ff_AddString(buffer+4);
            } else {
               ff_AddString(buffer);
            }
            string_length += StrLen(buffer);
            ff_AddChar(' ');
            string_length++; /* to account for the space above. */
         }

         if ((title = BioseqGetTitle(query_bsp)) != NULL) { 
            /* We do this to keep the entire title on one line 
               (of length BUFFER_LENGTH). */
            StrNCpy(buffer, title, BUFFER_LENGTH - string_length);
            buffer[BUFFER_LENGTH - string_length] = NULLB;
            ff_AddString(buffer);
         }

         if (unlock_bioseq)
            BioseqUnlock(query_bsp);
      }
      NewContLine();
   }
   if (blast_database) {
      ff_AddString("# Database: ");
      ff_AddString(blast_database);
      NewContLine();
   }
   if (getenv("PRINT_SEQUENCES")) {
         ff_AddString("# Fields: Query id, Subject id, % identity, alignment length, mismatches, gap openings, q. start, q. end, s. start, s. end, e-value, bit score, query seq., subject seq.");
   } else {
         ff_AddString("# Fields: Query id, Subject id, % identity, alignment length, mismatches, gap openings, q. start, q. end, s. start, s. end, e-value, bit score");
   }

   ff_EndPrint();
}

Boolean FastaCheckDna(CharPtr seq)
{
    Int2 len = 100;
    CharPtr ptr = NULL;

    if (*seq == '>') {
        for (++seq; *seq != NULLB && *seq != '\n'; seq++) ;
    }

    if ((ptr = StrChr(seq, '>')) != NULL)
       len = MIN(len, ptr - seq);
    else 
       len = MIN(len, StringLen(seq));

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
               if (search1->pbp->gapped_calculation)
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
      if (search1) {
         mask = search1->mask;
         while (mask) {
            SeqLocSetFree(mask->data.ptrvalue);
            mask = mask->next;
         }
         ValNodeFree(search1->mask);
         search1 = BlastSearchBlkDestruct(search1);
      }
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
         /* Save the ids of sequences in the cluster in the 'master' field of
            SeqAlign structure (kludge) */
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

/* This function does everything necessary to produce a SeqAlign from the
   intermediate BLAST results structures;
   In case of multiple queries, the SeqAlign array must be allocated outside.
*/
void
BLASTPostSearchLogic(BlastSearchBlkPtr search, BLAST_OptionsBlkPtr options,
                     SeqAlignPtr PNTR seqalignp, Boolean single_chain)
{
   BLASTResultHspPtr hsp;
   GapXEditBlockPtr edit_block;
   SeqIdPtr gi_list=NULL,subject_id;
   SeqAlignPtr head,seqalign,tail;
   BLASTResultsStructPtr result_struct;
   BLASTResultHitlistPtr   result_hitlist;
   Uint1Ptr sequence;
   Int4 length,hitlist_count,sequence_length,index,index1,total_num_hsp=0,hspcnt;
   Char buffer[512];
   Int2 num_queries, query_index;

   /* AM: Support for query multiplexing. */
   MQ_DivideResultsInfoPtr subjects = NULL;
   Int4 index2;
   Int4 nhlists = 0;
   
   head = NULL;
   tail = NULL;
   if (single_chain)
      *seqalignp = NULL;
   
   if (search->prog_number==blast_type_blastn) {
      /* Unconcatenate the strands by adjusting the query offsets in
         all hsps */
      search->context[search->first_context].query->length = 
         search->query_context_offsets[search->first_context+1] - 1;
   }
   
   if (StringCmp(search->prog_name, "blastn") == 0 && 
       search->pbp->gapped_calculation)
   {
      num_queries = search->last_context / 2 + 1;

      for (query_index=0; query_index<num_queries; query_index++) {
         if (options) {
            search->pbp->gap_open = options->gap_open;
            search->pbp->gap_extend = options->gap_extend;
         }
         /*
           search->pbp->gap_x_dropoff = (BLAST_Score) (options->gap_x_dropoff*NCBIMATH_LN2 / search->sbp->kbp_gap[search->first_context]->Lambda);
           search->pbp->gap_x_dropoff_final = (BLAST_Score) (options->gap_x_dropoff_final*NCBIMATH_LN2 / search->sbp->kbp_gap[search->first_context]->Lambda);
         */
         if (!search->pbp->mb_params) {
            result_struct = search->result_struct;
         } else {
            result_struct = search->mb_result_struct[query_index];
         }
         
         if (!result_struct)
            continue;
         hitlist_count = result_struct->hitlist_count;
         
         sequence=NULL;
         sequence_length=0;
         
         for (index=0; index<hitlist_count; index++)
         {
            if (search->pbp->mb_params && !search->pbp->mb_params->no_traceback
                && !search->pbp->mb_params->use_dyn_prog) {
               /* Traceback has already been computed */
               seqalign = 
                  MegaBlastGapInfoToSeqAlign(search, index, query_index);
            } else {
               length = readdb_get_sequence_ex(search->rdfp, 
                           result_struct->results[index]->subject_id,
                           &sequence, &sequence_length, TRUE);
               
               if (!search->pbp->mb_params) {
                  /* Traditional Blastn */
                  seqalign = SumBlastGetGappedAlignmentTraceback(
                                search, index, FALSE, FALSE, 
                                sequence+1, length);
               } else {
                  /* Mega BLAST with non-greedy extension */
                  SumBlastGetGappedAlignmentEx(search, index, FALSE, FALSE, 
                                               sequence+1, length, TRUE,
                                               &seqalign, NULL, query_index);
               }
            }
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
         sequence_length = 0;
         
         HeapSort(result_struct->results, hitlist_count, sizeof(BLASTResultHitlistPtr), evalue_compare_hits);

         /* 
            The next loop organizes the SeqAligns (and the alignments
            in the BLAST report) in the same order as the deflines.
         */
         if (!single_chain) {
            head = tail = NULL;
         }

         /* AM: Support for query multiplexing. */
	 if( search->mult_queries )
	 {
           subjects = (MQ_DivideResultsInfoPtr)MemNew( sizeof( MQ_DivideResultsInfo )*hitlist_count );
	   nhlists = hitlist_count;
         }
	 else nhlists = MIN(hitlist_count, options->hitlist_size);

         for (index=0, index2 = 0; index<nhlists; index++) {
            seqalign = result_struct->results[index]->seqalign;
            if (seqalign) {

              /* AM: Support for query multiplexing. */
	      if( search->mult_queries )
	      {
	        subjects[index2].evalue
		  = result_struct->results[index]->best_evalue;
	        subjects[index2++].subject_id 
	          = result_struct->results[index]->subject_id;
              }

               if (head == NULL) {
                  head = seqalign;
               } else {
                  for (; tail->next; tail = tail->next);
                  tail->next = seqalign;
               }
               tail = seqalign;
            }
         }

         /* AM: Support for query multiplexing. */
	 if( search->mult_queries )
	 {
	   search->mult_queries->sap_array_data->sap_array
	     = DivideSeqAligns( options, head, 
	                        search->mult_queries, subjects );
	   subjects = MemFree( subjects );
         }

         /* Free the extra seqaligns */
         for ( ; index < hitlist_count; index++)
            result_struct->results[index]->seqalign = 
               SeqAlignSetFree(result_struct->results[index]->seqalign);
         
         if (!single_chain)
            seqalignp[query_index] = head;
      }
      if (!single_chain)
         head = NULL;
   }
   else if (search->pbp->gapped_calculation)
      /* Non-blastn programs */
   {
      result_struct = search->result_struct;
      hitlist_count = result_struct->hitlist_count;

      if (!options || !(options->smith_waterman ||
                        options->tweak_parameters)) {

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

      if (options && options->tweak_parameters) {
         /*restore settings of number of matches and E-value threshold
           to what user requested*/
         options->hitlist_size = options->hitlist_size/2;
         options->expect_value = search->pbp->cutoff_e = options->original_expect_value;
      }
      
      if (options && 
          ((options->tweak_parameters) || (options->smith_waterman))) {
         head = RedoAlignmentCore(search, options, hitlist_count, 
                   options->tweak_parameters, options->smith_waterman);
      }

      for (index=0; index<hitlist_count; index++)
      {
         BLASTResultFreeHsp(result_struct->results[index]);
      }
      
      index = 0;
      if (!options || 
          (!(options->tweak_parameters) && !(options->smith_waterman))) {

         /* AM: Support for query multiplexing. */
	 if( search->mult_queries )
	 {
           subjects = (MQ_DivideResultsInfoPtr)MemNew( sizeof( MQ_DivideResultsInfo )*hitlist_count );
	   nhlists = hitlist_count;
         }
	 else nhlists = MIN(hitlist_count, options->hitlist_size);

         /* Only save alignments up to the original hitlist size */
         for ( index2 = 0; index<nhlists; index++) {
            seqalign = result_struct->results[index]->seqalign;

            /* AM: Support for query multiplexing. */
	    if( search->mult_queries )
	    {
	      subjects[index2].evalue
	        = result_struct->results[index]->best_evalue;
	      subjects[index2++].subject_id 
	        = result_struct->results[index]->subject_id;
            }

            if (seqalign) {
               if (head == NULL) {
                  head = seqalign;
               } else {
                  for ( ; tail->next; tail = tail->next);
                  tail->next = seqalign;
               }
               tail = seqalign;
            }
         }

         /* AM: Support for query multiplexing. */
	 if( search->mult_queries )
	 {
	   search->mult_queries->sap_array_data->sap_array
	     = DivideSeqAligns( options, head, 
	                        search->mult_queries, subjects );
	   subjects = MemFree( subjects );
         }
      } 
      
      /* Free the unused seqaligns (all in case of PSI BLAST, 
         extra ones otherwise */
      for ( ; index<hitlist_count; index++) {
         result_struct->results[index]->seqalign = 
            SeqAlignSetFree(result_struct->results[index]->seqalign);
      }
      search->number_of_seqs_better_E = MIN(hitlist_count, options->hitlist_size);
   } else {
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
               
               if (head == NULL) {
                  head = seqalign;
               } else {
                  for ( ; tail->next; tail = tail->next);
                  tail->next = seqalign;
               }
               tail = seqalign;
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
         Boolean discontinuous = ((options != NULL) && options->discontinuous);
         if (StringCmp("blastn", search->prog_name) == 0 || StringCmp("blastp", search->prog_name) == 0)
            head = GetSeqAlignForResultHitList(search, TRUE, FALSE, discontinuous, FALSE, FALSE);
         else
            head = GetSeqAlignForResultHitList(search, FALSE, FALSE, discontinuous, FALSE, FALSE);
      }
   }
   
   gi_list = SeqIdSetFree(gi_list);

   if (head)
      *seqalignp = head;
}

/*return query fasta style title(id+title). New memory was allocated for this title*/
CharPtr getFastaStyleTitle(BioseqPtr bsp){
  
  CharPtr titleBuf=NULL;

  if(bsp){
    
    if (bsp->id)
      {
	Char idBuf[BUFFER_LENGTH+1];
	CharPtr seqTitle, temp=NULL;

	SeqIdWrite(bsp->id, idBuf, PRINTID_FASTA_LONG, BUFFER_LENGTH);
	temp=idBuf;
	if (StringNCmp(idBuf, "lcl|", 4) == 0)
	  temp=StrStr(idBuf, "gi|");
		
	titleBuf=MemNew(sizeof(Char)*(BUFFER_LENGTH+1));
	StringCpy(titleBuf,temp); 
	seqTitle= BioseqGetTitle(bsp);
	StringNCat(titleBuf, seqTitle, BUFFER_LENGTH-StringLen(temp));
       
      }
  }
   return titleBuf;
}

Int2
BlastHSPGetNumIdentical(BlastSearchBlkPtr search, BLAST_HSPPtr hsp,
                        BLASTResultHspPtr result_hsp,
                        Int4Ptr num_ident_ptr, Int4Ptr align_length_ptr)
{
   Int4 i, num_ident, align_length, q_off, s_off;
   Int2 context;
   Uint1Ptr query_seq, subject_seq = NULL, q, s;
   GapXEditBlockPtr gap_info;
   GapXEditScriptPtr esp;

   gap_info = (hsp ? hsp->gap_info : result_hsp->gap_info);

   if (!gap_info && search->pbp->gapped_calculation)
      return -1;

   context = (hsp ? hsp->context : result_hsp->context);
   q_off = (hsp ? hsp->query.offset : result_hsp->query_offset);
   s_off = (hsp ? hsp->subject.offset : result_hsp->subject_offset);

   if (search->pbp->is_ooframe)
     q_off /= CODON_LENGTH;
   
   query_seq = search->context[context].query->sequence;
   if (search->subject->sequence_start)
      subject_seq = search->subject->sequence_start + 1;
   else 
      subject_seq = search->subject->sequence;

   if (!subject_seq)
      return -1;

   q = &query_seq[q_off];
   s = &subject_seq[s_off];

   num_ident = 0;
   align_length = 0;


   if (!gap_info) {
      /* Ungapped case */
      align_length = (hsp ? hsp->query.length : result_hsp->query_length); 
      for (i=0; i<align_length; i++) {
         if (*q++ == *s++)
            num_ident++;
      }
   } else {
      for (esp = gap_info->esp; esp; esp = esp->next) {
         align_length += esp->num;
         switch (esp->op_type) {
         case GAPALIGN_SUB:
            for (i=0; i<esp->num; i++) {
               if (*q++ == *s++)
                  num_ident++;
            }
            break;
         case GAPALIGN_DEL:
            s += esp->num;
            break;
         case GAPALIGN_INS:
            q += esp->num;
            break;
         default: 
            s += esp->num;
            q += esp->num;
            break;
         }
      }
   }

   *align_length_ptr = align_length;
   *num_ident_ptr = num_ident;
   return 0;
}

Int2
OOFBlastHSPGetNumIdentical(Uint1Ptr query_seq, Uint1Ptr subject_seq, 
   BLAST_HSPPtr hsp, BLASTResultHspPtr result_hsp,
   Int4Ptr num_ident_ptr, Int4Ptr align_length_ptr)
{
   Int4 i, num_ident, align_length, q_off, s_off;
   Int2 context;
   Uint1Ptr q, s;
   GapXEditBlockPtr gap_info;
   GapXEditScriptPtr esp;

   gap_info = (hsp ? hsp->gap_info : result_hsp->gap_info);

   if (!gap_info)
      return -1;

   context = (hsp ? hsp->context : result_hsp->context);
   s_off = (hsp ? hsp->query.offset : result_hsp->query_offset);
   q_off = (hsp ? hsp->subject.offset : result_hsp->subject_offset);

   if (!subject_seq || !query_seq)
      return -1;

   q = &query_seq[q_off];
   s = &subject_seq[s_off];

   num_ident = 0;
   align_length = 0;


   for (esp = gap_info->esp; esp; esp = esp->next) {
      switch (esp->op_type) {
      case 3: /* Substitution */
         align_length += esp->num;
         for (i=0; i<esp->num; i++) {
            if (*q == *s)
               num_ident++;
            ++q;
            s += CODON_LENGTH;
         }
         break;
      case 6: /* Insertion */
         align_length += esp->num;
         s += esp->num * CODON_LENGTH;
         break;
      case 0: /* Deletion */
         align_length += esp->num;
         q += esp->num;
         break;
      case 1: /* Gap of two nucleotides. */
         s -= 2;
         break;
      case 2: /* Gap of one nucleotide. */
         s -= 1;
         break;
      case 4: /* Insertion of one nucleotide. */
         s += 1;
         break;
      case 5: /* Insertion of two nucleotides. */
         s += 2;
         break;
      default: 
         s += esp->num * CODON_LENGTH;
         q += esp->num;
         break;
      }
   }

   *align_length_ptr = align_length;
   *num_ident_ptr = num_ident;
   return 0;
}


/*  --------------------------------------------------------------------
 *
 *  BLAST_Wizard & related functions
 *
 *  --------------------------------------------------------------------
 */

void
BLAST_WizardOptionsBlkInit(BLAST_WizardOptionsBlkPtr options)
{
    memset(options, 0, sizeof(BLAST_WizardOptionsBlk));
}

void
BLAST_WizardOptionsBlkDone(BLAST_WizardOptionsBlkPtr options)
{
    if(options->entrez_query) {
        MemFree(options->entrez_query);
        options->entrez_query = 0;
    }
    if(options->filter_string) {
        MemFree(options->filter_string);
        options->filter_string = 0;
    }
    ValNodeFree(options->gilist);
    options->gilist = 0;
    if(options->matrix) {
        MemFree(options->matrix);
        options->matrix = 0;
    }
    if(options->phi_pattern) {
        MemFree(options->phi_pattern);
        options->phi_pattern = 0;
    }
    if(options->query_lcase_mask) {
        ValNodeFreeData(options->query_lcase_mask);
        options->query_lcase_mask = 0;
    }
    if(options->string_options) {
        MemFree(options->string_options);
        options->string_options = 0;
    }
}

void
BLAST_WizardOptionsMaskInit(BLAST_WizardOptionsMaskPtr mask)
{
    mask->block_width               = FALSE;
    mask->cutoff_s                  = FALSE;
    mask->db_genetic_code           = FALSE;
    mask->ethresh                   = FALSE;
    mask->expect_value              = FALSE;
    mask->first_db_seq              = FALSE;
    mask->final_db_seq              = FALSE;
    mask->gap_extend                = FALSE;
    mask->gap_open                  = FALSE;
    mask->gapped_calculation        = FALSE;
    mask->genetic_code              = FALSE;
    mask->hitlist_size              = FALSE;
    mask->hsp_range_max             = FALSE;
    mask->is_ooframe                = FALSE;
    mask->mb_disc_type              = FALSE;
    mask->mb_template_length        = FALSE;
    mask->no_traceback              = FALSE;
    mask->penalty                   = FALSE;
    mask->perc_identity             = FALSE;
    mask->perform_culling           = FALSE;
    mask->pseudoCountConst          = FALSE;
    mask->required_end              = FALSE;
    mask->required_start            = FALSE;
    mask->reward                    = FALSE;
    mask->searchsp_eff              = FALSE;
    mask->smith_waterman            = FALSE;
    mask->strand_option             = FALSE;
    mask->threshold_first           = FALSE;
    mask->threshold_second          = FALSE;
    mask->tweak_parameters          = FALSE;
    mask->use_best_align            = FALSE;
    mask->use_real_db_size          = FALSE;
    mask->window_size               = FALSE;
    mask->wordsize                  = FALSE;

    mask->two_hits                  = FALSE;
}

BLAST_OptionsBlkPtr
BLAST_Wizard(
    const char*                 program,
    const char*                 service,
    BLAST_WizardOptionsBlkPtr   options,
    BLAST_WizardOptionsMaskPtr  mask,
    int*                        alignments,
    int*                        descriptions,
    char**                      error)
{
    int dummy_alignments = -1;
    int dummy_descriptions = -1;
    BLAST_OptionsBlkPtr out = 0;
    Boolean ungapped_alignment;
    *error = 0;
    if(!alignments)
        alignments = &dummy_alignments;
    if(!descriptions)
        descriptions = &dummy_descriptions;
    if(mask->expect_value && mask->cutoff_s) {
        *error = StringSave("can't specify both expect_value and cutoff_s");
        goto end;
    }
    ungapped_alignment = mask->gapped_calculation ?
        !options->gapped_calculation :
        FALSE;
    if(!StringCmp(service, "vecscreen")) {
        out = VSBlastOptionNew();
        out->gapped_calculation = !ungapped_alignment;
    }
    else {
        Boolean is_megablast_search = !StringICmp(service, "megablast");
        out = BLASTOptionNewEx(
            (char*) program, !ungapped_alignment, is_megablast_search);

        /* set some defaults for backward compat. with blastcgicmd.cpp */
        if(!StringCmp(service, "psi"))
            out->ethresh = 0.002;
	else if (!StringCmp(service, "rpsblast"))
            out->is_rps_blast = TRUE;

        /* ignore some fields for backward compat. with blastcgicmd.cpp */
        if(is_megablast_search)
            mask->block_width = FALSE;
        else {
            mask->mb_disc_type = FALSE;
            mask->mb_template_length = FALSE;
            mask->no_traceback = FALSE;
            mask->two_hits = FALSE;
        }
    }
    if(out->is_megablast_search) {
        out->no_traceback = mask->no_traceback ?
            options->no_traceback :
            FALSE;
    }
    if(mask->is_ooframe)
        out->is_ooframe = options->is_ooframe;
    if(mask->first_db_seq)
        out->first_db_seq = options->first_db_seq;
    if(mask->final_db_seq)
        out->final_db_seq = options->final_db_seq;
    if(mask->pseudoCountConst)
        out->pseudoCountConst = options->pseudoCountConst;
    if(mask->strand_option)
        out->strand_option = options->strand_option;
    if(mask->use_best_align)
        out->use_best_align = options->use_best_align;
    if(mask->use_real_db_size)
        out->use_real_db_size = options->use_real_db_size;
    if(mask->window_size)
        out->window_size = options->window_size;
    if(mask->expect_value)
        out->expect_value = options->expect_value;
    if(mask->cutoff_s)
        out->cutoff_s = options->cutoff_s;
    if(mask->threshold_first)
        out->threshold_first = options->threshold_first;
    if(mask->threshold_second)
        out->threshold_second = options->threshold_second;
    if(mask->perc_identity)
        out->perc_identity = options->perc_identity;
    if(out->is_megablast_search) {
        if (out->perc_identity >= 95.0) {
            out->reward = 1;
            out->penalty = -3;
        }
        else if(out->perc_identity >= 85.0
                || out->perc_identity == 0.0) {
            out->reward = 1;
            out->penalty = -2;
        }
        else if (out->perc_identity >= 80.0) {
            out->reward = 2;
            out->penalty = -3;
        }
        else if (out->perc_identity >= 75.0) {
            out->reward = 4;
            out->penalty = -5;
        }
        else if (out->perc_identity >= 60.0) {
            out->reward = 2;
            out->penalty = -2;
        }
    }
    if(mask->penalty)
        out->penalty = options->penalty;
    if(mask->reward)
        out->reward = options->reward;
    if(mask->wordsize)
        out->wordsize = options->wordsize;
    if(mask->searchsp_eff)
        out->searchsp_eff = options->searchsp_eff;
    if(mask->hsp_range_max)
        out->hsp_range_max = options->hsp_range_max;
    if(!out->is_megablast_search) {
        if(mask->block_width)
            out->block_width = options->block_width;
    }
    else {
        if(mask->mb_template_length)
            out->mb_template_length = options->mb_template_length;
        if(mask->two_hits && options->two_hits)
            out->window_size = 40;
        if(mask->mb_disc_type)
            out->mb_disc_type = options->mb_disc_type;
        if(out->mb_template_length > 0
           && out->wordsize <= 12
           && out->wordsize >= 11
           && out->perc_identity < 90.0) {
            if(out->perc_identity >= 85.0) {
                out->gap_open = 4;
                out->gap_extend = 1;
            }
            else if(out->perc_identity >= 80.0) {
                out->gap_open = 5;
                out->gap_extend = 2;
            }
            else if(out->perc_identity >= 75) {
                out->gap_open = 7;
                out->gap_extend = 2;
            }
            else if(out->perc_identity >= 60) {
                out->gap_open = 3;
                out->gap_extend = 1;
            }
        }
    }
    if(!out->perform_culling)
        out->perform_culling = mask->perform_culling ?
            options->perform_culling :
            FALSE;
    if(mask->gap_open)
        out->gap_open = options->gap_open;
    if(mask->gap_extend)
        out->gap_extend = options->gap_extend;
    if(mask->genetic_code)
        out->genetic_code = options->genetic_code;
    if(mask->db_genetic_code)
        out->db_genetic_code = options->db_genetic_code;
    if(!out->tweak_parameters)
        out->tweak_parameters = mask->tweak_parameters ?
            options->tweak_parameters :
            FALSE;
    if(!strcmp(service, "psi")) {
        out->ethresh = mask->ethresh ?
            options->ethresh :
            0.002;
        out->tweak_parameters = mask->tweak_parameters ?
            options->tweak_parameters :
            TRUE;
    }
    if(options->entrez_query) {
        out->entrez_query = options->entrez_query; /* take ownership */
        options->entrez_query = 0;
        out->gifile = 0;
    }
    if(options->matrix) {
        out->matrix = options->matrix; /* take ownership */
        options->matrix = 0;
    }
    if(options->phi_pattern) {
        out->phi_pattern = options->phi_pattern; /* take ownership */
        options->phi_pattern = 0;
    }
    if(out->is_megablast_search) {
        if(out->wordsize < 8)
            out->wordsize = 8;
        out->cutoff_s2 = out->wordsize * out->reward;
    }
    out->hitlist_size = -1;
    if(mask->hitlist_size) {
        out->hitlist_size = options->hitlist_size;
        *alignments = MIN(out->hitlist_size, *alignments);
        *descriptions = MIN(out->hitlist_size, *descriptions);
    }
    else
        out->hitlist_size = -1;
    if(options->string_options) {
        int other_adv_descriptions = -1;
        int other_adv_alignments = -1;
        parse_blast_options(
            out,
            options->string_options,
            error,
            NULL,
            &other_adv_descriptions,
            &other_adv_alignments);
        if(*error)
            goto end;
        if (other_adv_alignments != -1) {
            *alignments = other_adv_alignments;
            out->hitlist_size =
                MAX(out->hitlist_size, other_adv_alignments);
        }
        if (other_adv_descriptions != -1) {
            *descriptions = other_adv_descriptions;
            out->hitlist_size =
                MAX(out->hitlist_size, other_adv_descriptions);
        }
        if(!StringCmp(out->filter_string, "T")) {
            MemFree(out->filter_string);
            if(StringICmp(program, "blastn"))
                out->filter_string = StringSave("S");
            else
                out->filter_string = StringSave("D");
        }
    }
    /* append ; for backward compat. with blastcgicmd.cpp */
    if(out->filter_string) {
        char buf[10240];
        if(strlen(out->filter_string)+2 > sizeof buf) {
            *error = StringSave("internal error: buffer too small");
            goto end;
        }
        sprintf(buf, "%s;", out->filter_string);
        MemFree(out->filter_string);
        out->filter_string = StringSave(buf);
    }
    if(options->filter_string) {
        char buf[10240];
        char* s = out->filter_string ? out->filter_string : "";
        if(strlen(s)+strlen(options->filter_string)+1 > sizeof buf) {
            *error = StringSave("internal error: buffer too small");
            goto end;
        }
        sprintf(buf, "%s%s", s, options->filter_string);
        if(out->filter_string)
            MemFree(out->filter_string);
        out->filter_string = StringSave(buf);
    }
    if(out->filter_string)
        out->filter = TRUE;
    if(out->hitlist_size != -1) {
        if(*descriptions == -1)
            *descriptions = MIN(100, out->hitlist_size);
        if(*alignments == -1)
            *alignments = MIN(100, out->hitlist_size);
    } else {
        if(*descriptions == -1)
            *descriptions = 100;
        if(*alignments == -1)
            *alignments = 100;
        out->hitlist_size = MAX(*descriptions, *alignments);
    }
    if(!strcmp(service, "vecscreen"))
        out->hitlist_size = 1000;
    if(mask->required_start)
        out->required_start = options->required_start;
    if(mask->required_end)
        out->required_end = options->required_end;
	else
		out->required_end = 0; /* backward compat. with blastcgicmd.cpp */
    if(options->gilist) {
        out->gilist = options->gilist; /* take ownership */
        options->gilist = 0;
    }
end:
    if(*error) {
        BLASTOptionDelete(out);
        out = 0;
    }
    return out;
}
