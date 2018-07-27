static char const rcsid[] = "$Id: blastutl.c,v 6.459 2005/04/25 14:16:36 coulouri Exp $";

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

File name: blastutl.c

Author: Tom Madden

Contents: Utilities for BLAST

$Revision: 6.459 $

******************************************************************************/
/*
 *
* $Log: blastutl.c,v $
* Revision 6.459  2005/04/25 14:16:36  coulouri
* set db_chunk_size adaptively
*
* Revision 6.458  2005/04/04 20:44:27  camacho
* Do not overwrite the effective search space in Pssm2Sequences if specified in the options structure
*
* Revision 6.457  2005/02/07 15:30:08  dondosha
* Removed restriction on the value of longest intron option
*
* Revision 6.456  2005/01/24 20:37:37  camacho
* Added conditional compilation to structs need for BLAST_CLUSTER_HITS
*
* Revision 6.455  2005/01/18 14:54:13  camacho
* Change in tie-breakers for score comparison, suggestion by Mike Gertz
*
* Revision 6.454  2004/12/20 15:22:16  camacho
* Calculate kbp_ideal values rather than loading them from pre-computed values
*
* Revision 6.453  2004/12/01 17:24:15  coulouri
* do not dereference null pointer
*
* Revision 6.452  2004/11/22 16:10:11  dondosha
* Minor fix to make sure that "evalue" score type is always used when hsp is not part of a linked set
*
* Revision 6.451  2004/11/04 15:51:55  bealer
* - bl2seq should use dblen as average length if database is not available.
*
* Revision 6.450  2004/11/01 14:07:56  madden
* From Mike Gertz:
*
*    - In query_offset_compare_hsp and query_end_compare_hsp, use the
*      subject query/offset as a tie-breaker.  Without this tie-breaker
*      CheckGappedAlignmentsForOverlap won't work properly.
*
*    - In CheckGappedAlignmentsForOverlap check that hsp_array, rather
*      than *hsp_array, is not nil.
*
*    - In BlastSaveCurrentHsp, rewrote the binary search to use
*      score_compare_hsps, so that the answers are consistent with the
*      heap code used in the algo/blast/core code.
*
*    - In BlastGappedScoreInternal delete gapped extensions that don't
*      reach the cutoff score (cutoff_s1).
*
* Revision 6.449  2004/10/25 18:36:17  papadopo
* From Michael Gertz: remove unneeded decrement of alignment offsets in BlastNtSaveCurrentHsp
*
* Revision 6.448  2004/10/19 19:42:17  dondosha
* Optimized algorithm in BlastPruneSeqAlignByGiList to make it up to 25 times faster; Added new function BlastPruneSeqAlignBySortedGiList
*
* Revision 6.447  2004/10/18 13:02:41  madden
* Changes from Mike Gertz:
*         - In score_compare_hsps, query_offset_compare_hsp and
*           query_end_compare_hsp, change the comparison tests so that
*           nil HSPs are less than any non-nil HSP.  Previously, these
*           comparison functions would return 0 if either HSP was nil,
*           which would result in sort routines terminating before the
*           non-nil HSPs in the list were fully sorted.
*
*         - In score_compare_hsps, copied the set of tie-breakers from
*           the corresponding routine in algo/blast/core/blast_hits.c.
*
*         - In RealBlastGetGappedAlignmentTraceback, the HSP list must
*           be sorted before BLASTCheckHSPInclusion is invoked.
*
* Revision 6.446  2004/09/28 16:05:32  papadopo
* From Michael Gertz: In BlastGappedScoreInternal, changed a
* reference to the sumscore field of an HSP to a reference to the
* xsum field of an HSP.
*
* Revision 6.445  2004/08/23 17:05:42  papadopo
* From Michael Gertz: make CopyResultHspToHSP public
*
* Revision 6.444  2004/08/16 19:37:26  dondosha
* Enabled uneven gap HSP linking for blastx
*
* Revision 6.443  2004/08/05 21:52:28  camacho
* Gracefully handle inability to calculate ungapped lambda for PSSM in psiblast2sequences
*
* Revision 6.442  2004/07/24 18:55:29  camacho
* Fix to GetSequenceWithDenseSeg when sequence cannot be found
*
* Revision 6.441  2004/07/19 17:05:36  papadopo
* specify (unused) 'output-to-scoremat' parameter
*
* Revision 6.440  2004/06/30 12:29:39  madden
* Moved some functions to blfmtutl.c
*
* Revision 6.439  2004/06/22 14:16:55  camacho
* Changed invocation of posFreqsToMatrix to conform with new signature
*
* Revision 6.438  2004/06/01 20:34:06  dondosha
* Fix in previous change; memory leak fix
*
* Revision 6.437  2004/05/27 17:36:24  dondosha
* Minor fix for previous 2 changes
*
* Revision 6.436  2004/05/25 21:42:47  dondosha
* Fix in previous change: in some cases edit block should not be freed when BLAST_HSP is freed
*
* Revision 6.435  2004/05/21 13:53:04  dondosha
* Use BLAST_HSPFree to free BLAST_HSP structures, hence no need to call GapXEditBlockDelete in multiple places
*
* Revision 6.434  2004/04/22 16:40:32  dondosha
* Set search->subject_id to correct ordinal id, needed for finding splice junctions in HSP links at traceback stage
*
* Revision 6.433  2004/03/22 22:10:38  dondosha
* Use kbp_gap instead of kbp pointers in megablast traceback
*
* Revision 6.432  2004/02/26 15:52:30  papadopo
* Mike Gertz' modifications to unify handling of gapped Karlin blocks between protein and nucleotide searches
*
* Revision 6.431  2004/02/04 15:35:03  camacho
* Rollback to fix problems in release 2.2.7
*
* Revision 6.429  2004/01/30 16:54:45  dondosha
* Check if HSP needs to be deleted after reevaluation with ambiguities, after greedy traceback
*
* Revision 6.428  2004/01/28 16:54:03  dondosha
* Restored the code that shifts subject coordinates for blastn traceback with long subject sequences
*
* Revision 6.427  2004/01/25 05:06:21  dondosha
* Translate only relevant parts of long subject sequences for tblastn traceback
*
* Revision 6.426  2004/01/16 23:43:44  dondosha
* No more need for special argument for partial search: it is set in options
*
* Revision 6.425  2004/01/14 17:01:06  dondosha
* Gapped alignment is position based only if posMatrix exists
*
* Revision 6.424  2004/01/09 18:13:24  dondosha
* In [Get,Check]StartForGappedAlignment: if posMatrix not available, use square matrix for calculations
*
* Revision 6.423  2004/01/06 22:37:40  dondosha
* Use BLAST_HSPfree function; in particular fixes a bug with wrong memory being freed
*
* Revision 6.422  2003/12/11 23:46:28  dondosha
* Correction in setting hit ranges after repeats filtering
*
* Revision 6.421  2003/12/10 17:05:28  dondosha
* Added function ReevaluateScoreWithAmbiguities to reevaluate score for one HSP; use it after greedy traceback
*
* Revision 6.420  2003/11/24 22:06:41  madden
* Tblastn optimization, only fetch part of sequence needed
*
* Revision 6.419  2003/10/30 18:37:19  dondosha
* Fix for megablast with non-greedy traceback
*
* Revision 6.418  2003/10/29 17:46:59  dondosha
* Allow 2-stage greedy extension in megablast
*
* Revision 6.417  2003/08/20 22:14:08  dondosha
* Little correction in call to OOFBlastHSPGetNumIdentical
*
* Revision 6.416  2003/08/04 16:19:16  dondosha
* Added effective HSP length (length adjustment) to other returns, so it can be reported in XML output
*
* Revision 6.415  2003/05/30 17:25:36  coulouri
* add rcsid
*
* Revision 6.414  2003/05/23 22:12:11  camacho
* Fix memory leak in PsiBlast2Sequences
*
* Revision 6.413  2003/04/22 21:52:13  dondosha
* Added function OOFBlastHSPGetNumIdentical
*
* Revision 6.412  2003/04/10 19:21:16  dondosha
* Memory leak fix for megablast with limited number of HSPs per hit
*
* Revision 6.411  2003/03/24 19:42:14  madden
* Changes to support query concatenation for blastn and tblastn
*
* Revision 6.410  2003/03/11 14:33:48  madden
* Sort HSPs after array is no longer reallocated
*
* Revision 6.409  2003/02/21 02:52:16  madden
* Ensure stable sorting in score_compare_hsp (change from Morgulis)
*
* Revision 6.408  2003/01/24 22:26:03  camacho
* RPSInit is deprecated, use RPSInitEx instead
*
* Revision 6.407  2002/12/09 17:22:16  dondosha
* When alignment jumps beyond a strand boundary, keep the part of it where initial word is
*
* Revision 6.406  2002/12/04 23:32:50  camacho
* Do not set use_this_gi with nucleotide dbs (redundant)
*
* Revision 6.405  2002/12/04 18:42:22  camacho
* Minor change to previous commit
*
* Revision 6.404  2002/12/04 18:38:58  camacho
* Use correct effective search space in B2SPssmMultipleQueries
*
* Revision 6.403  2002/12/04 17:08:33  camacho
* Minor change to B2SPssmCleanUpSearch
*
* Revision 6.402  2002/11/27 15:41:51  dondosha
* Added -t, -g and -n megablast options to parse_blast_options
*
* Revision 6.401  2002/11/26 23:02:07  madden
* Add w option to parse_blast_options (OOF for blastx)
*
* Revision 6.400  2002/11/25 19:57:30  dondosha
* Further fix to the HSP limit (-H) megablast option
*
* Revision 6.399  2002/11/22 23:31:43  dondosha
* 1. Use array of structures instead of array of pointers for initial offset pairs;
* 2. Sort the HSP array when maximal number of HSPs is reached for a sequence
*
* Revision 6.398  2002/11/13 23:23:53  dondosha
* Correction for getting number of identities in tblastn
*
* Revision 6.397  2002/11/07 22:25:34  dondosha
* Correction in calculating number of identities for very long database sequences
*
* Revision 6.396  2002/11/04 23:00:54  dondosha
* Calculate number of identities while computing the traceback, and save it in the seqalign
*
* Revision 6.395  2002/10/22 21:03:42  camacho
* Calculate the effective search space correctly for rpsblast in BlastOtherReturnsPrepare
*
* Revision 6.394  2002/10/22 17:57:48  camacho
* Changes to B2SPssmMultipleQueries
*
* Revision 6.393  2002/10/22 15:28:45  kans
* SeqAlignCompare takes LIBCALLBACK
*
* Revision 6.392  2002/10/21 23:13:36  camacho
* Added B2SPssmOnTheFly functions
*
* Revision 6.391  2002/10/18 15:08:28  dondosha
* Correction in SaveCurrentHsp functions when maximal number of HSPs is reached
*
* Revision 6.390  2002/10/17 14:33:12  dondosha
* Correction for the maximal number of HSPs option
*
* Revision 6.389  2002/09/19 22:22:18  camacho
* Sanity checks in BlastTwoSequencesByLocWithCallback
*
* Revision 6.388  2002/09/16 15:54:59  camacho
* Turn off RedoAlignmentCore from psi-bl2seq
*
* Revision 6.387  2002/09/13 20:05:43  camacho
* Set the dbseq_num to 1 in BlastTwoSequencesByLocWithCallback
*
* Revision 6.386  2002/09/11 20:46:25  camacho
* Removed deprecated BlastSeqIdListPtr code
*
* Revision 6.385  2002/09/03 14:22:45  camacho
* Changes to pacify mac compiler
*
* Revision 6.384  2002/09/02 21:54:41  camacho
* Correction to previous revision
*
* Revision 6.383  2002/09/02 20:44:56  camacho
* Allow pssm rescaling if scalingFactor is non-zero
*
* Revision 6.382  2002/08/30 15:42:49  dondosha
* In blastn, use ewp structure only for the first context
*
* Revision 6.381  2002/08/29 19:22:20  camacho
* Save karlinK parameter when rescaling pssm
*
* Revision 6.380  2002/08/29 16:23:42  camacho
* Removed debugging code
*
* Revision 6.379  2002/08/29 15:49:56  camacho
* Added matrix rescaling code for psi-blast2sequences
*
* Revision 6.378  2002/08/26 16:55:52  madden
* Fix for scaling with translated searches
*
* Revision 6.376  2002/08/05 20:07:37  dondosha
* Correction for bl2seq with megablast option: convert gap info to seqalign after search
*
* Revision 6.375  2002/08/02 21:49:56  vakatov
* + LIBCALL
*
* Revision 6.374  2002/08/01 21:33:12  madden
* Do not put p-value and small_gap into SeqAlign
*
* Revision 6.373  2002/08/01 20:45:34  dondosha
* Changed prototype of the BLASTPostSearchLogic function to make it
* more convenient
*
* Revision 6.372  2002/07/18 19:40:45  dondosha
* Added an option to restrict number of HSPs per database sequence
*
* Revision 6.371  2002/07/11 22:31:54  camacho
* Added sanity check to BlastTwoSequencesByLocWithCallback with PSSM
*
* Revision 6.370  2002/07/02 17:08:01  dondosha
* Reverse previous change - not needed
*
* Revision 6.369  2002/07/02 01:41:31  dondosha
* Typo fix
*
* Revision 6.368  2002/07/02 01:36:40  dondosha
* For megablast use larger window in CheckStartForGappedAlignment
*
* Revision 6.367  2002/06/21 21:43:01  camacho
* Removed obsolete BlastSeqIdList structure and functions
*
* Revision 6.366  2002/06/13 16:51:41  madden
* BlastTwoSequencesCore and BlastTwoSequencesCoreEx return status instead of SearchBlk
*
* Revision 6.365  2002/06/12 20:34:50  coulouri
* Don't dereference possibly NULL pointer
*
* Revision 6.364  2002/06/11 20:40:05  dondosha
* Correction to previous change
*
* Revision 6.363  2002/06/11 14:44:46  dondosha
* Return status from some functions instead of search block pointer
*
* Revision 6.362  2002/05/31 16:06:20  kans
* changed MemSet (..., NULL, ...) to MemSet (..., 0, ...) for Mac compiler
*
* Revision 6.361  2002/05/29 17:14:49  dondosha
* Check whether an id found by SeqIdFindBest is indeed a gi
*
* Revision 6.360  2002/05/28 22:00:12  camacho
* *** empty log message ***
*
* Revision 6.359  2002/05/13 13:51:32  dondosha
* Made two functions public
*
* Revision 6.358  2002/05/08 22:51:11  dondosha
* Do the starting positions check for final gapped alignment in Mega BLAST case as well
*
* Revision 6.357  2002/04/23 20:41:21  dondosha
* In case of non-affine extension in megablast, check percent identity cutoff after the traceback is obtained
*
* Revision 6.356  2002/04/19 17:26:07  madden
* Fix for last update
*
* Revision 6.355  2002/04/18 20:16:52  madden
* Fix problem with FUM for SeqLoc
*
* Revision 6.354  2002/04/17 20:42:23  madden
* Fix typo for mask1
*
* Revision 6.353  2002/04/04 21:19:15  dondosha
* Corrections for megablast with non-greedy extensions
*
* Revision 6.352  2002/03/28 18:51:39  madden
* All threads get access to (query) masking seqloc, merge overlapping segments for seg
*
* Revision 6.351  2002/03/26 23:18:00  dondosha
* Duplicate mb_endpoint_results structure on all threads
*
* Revision 6.350  2002/03/26 16:49:33  madden
* Use scaled up/down Lambda
*
* Revision 6.349  2002/03/14 16:11:40  camacho
* Extended BlastTwoSequences to allow comparison between sequence and PSSM
*
* Revision 6.348  2002/03/05 17:58:56  dondosha
* Set same offsets for the traceback as for preliminary extension for megablast with non-greedy extensions
*
* Revision 6.347  2002/02/15 23:36:22  dondosha
* Correction for megablast with non-greedy extensions
*
* Revision 6.346  2002/01/11 20:14:28  madden
* Put the use_this_gi into the SeqAlign
*
* Revision 6.345  2002/01/07 23:16:00  dondosha
* Fixed several memory leaks and allocation/freeing bugs in multithreaded megablast
*
* Revision 6.344  2001/12/28 20:38:40  dondosha
* Moved Mega BLAST related parameters into a separate structure
*
* Revision 6.343  2001/12/13 16:06:54  dondosha
* Use separate mb_endpoint_results list for each of multiple threads
*
* Revision 6.342  2001/11/26 20:19:25  madden
* Add call to BLASTOptionValidateEx to BlastTwoSequencesWithCallback
*
* Revision 6.341  2001/11/16 15:44:26  dondosha
* In BlastPruneSeqAlignByGiList: retrieve bioseq only if seqid in seqalign is not a gi
*
* Revision 6.340  2001/11/14 00:31:44  camacho
* Updated BlastGetAllowedGis and BlastGetFirstGiofSubset functions
* to return the correct seqid's when dealing with the new database
* format and mask (subset) databases.
*
* Revision 6.339  2001/11/13 18:20:33  dondosha
* Use GapxEditScript structure instead of edit_script_t in higher level function calls
*
* Revision 6.338  2001/10/12 16:10:07  dondosha
* 1. Made BLASTResultFreeHsp public
* 2. Added BioseqBlastEngineCoreEx with partial search option
*
* Revision 6.337  2001/10/05 18:10:29  madden
* Add threshold_second to parse_blast_options
*
* Revision 6.336  2001/09/19 17:24:17  kans
* removed extra parameter from BioseqMegaBlastEngineCore
*
* Revision 6.335  2001/09/07 14:46:43  dondosha
* Roll back removal of threshold_first from functions and structures
*
* Revision 6.334  2001/09/06 20:24:33  dondosha
* Removed threshold_first
*
* Revision 6.333  2001/07/27 20:04:09  dondosha
* Small correction in passing effective db length for two sequences engine
*
* Revision 6.332  2001/07/26 18:19:03  dondosha
* Added a few more letter options in parse_blast_options
*
* Revision 6.331  2001/07/20 18:55:58  dondosha
* 1. Use effective db length option in 2 sequences engine
* 2. Create diagonal array for megablast when needed
*
* Revision 6.330  2001/07/09 14:17:24  madden
* Fix PC-lint complaints from R. Williams
*
* Revision 6.329  2001/07/09 13:12:03  madden
* Removed unused variables
*
* Revision 6.328  2001/06/25 18:30:24  madden
* Add define for NLM_GENERATED_CODE_PROTO to get prototypes in fdlobj.h
*
* Revision 6.327  2001/06/25 16:03:31  madden
* Comment out CheckGappedAlignmentsForOverlap
*
* Revision 6.326  2001/06/12 19:48:55  madden
* Introduce total_hsp_limit, check before making SeqAlign
*
* Revision 6.325  2001/06/04 21:29:42  dondosha
* Add message about deleted hits with e-value below the low threshold
*
* Revision 6.324  2001/05/07 13:18:24  madden
* Fix to really remove deleted HSPs from (culling) heap
*
* Revision 6.323  2001/05/04 19:50:45  dondosha
* Improved error message when all queries are shorter than word size
*
* Revision 6.322  2001/05/03 21:48:28  dondosha
* Handle some cases when memory allocation fails
*
* Revision 6.321  2001/04/16 21:28:11  dondosha
* Added function BlastPruneSeqAlignByEvalueRange
*
* Revision 6.320  2001/04/12 21:34:50  dondosha
* Added function BlastPruneSeqAlignByGiList
*
* Revision 6.319  2001/04/12 17:17:15  madden
* Fixes core-dump for small query
*
* Revision 6.318  2001/04/12 15:01:25  madden
* change repeat filtering db
*
* Revision 6.317  2001/04/11 20:56:06  madden
* Added scalingFactor for rpsblast
*
* Revision 6.316  2001/04/11 18:22:13  dondosha
* Copy query_slp in BlastSearchBlkDuplicate for all programs
*
* Revision 6.315  2001/04/03 21:59:49  dondosha
* Implemented tabulated output for non-megablast bl2seq
*
* Revision 6.314  2001/03/28 21:05:23  dondosha
* Set dbinfo->is_protein in other returns
*
* Revision 6.313  2001/03/27 21:27:01  madden
* Minor efficiency in how lookup table is made
*
* Revision 6.312  2001/03/27 21:13:56  dondosha
* Do not print error if OID list exists without CommonIndex
*
* Revision 6.311  2001/03/27 20:35:10  dondosha
* Small bug fix
*
* Revision 6.310  2001/03/26 15:03:25  madden
* Fix number warnings and two bugs found by PC compiler
*
* Revision 6.309  2001/03/21 15:46:32  dondosha
* Added missing parentheses in previous change
*
* Revision 6.308  2001/03/20 20:06:13  dondosha
* Added protection from crossing strand boundary for blastn
*
* Revision 6.307  2001/03/19 18:51:39  madden
* HitRangeToSeqLoc returns values appropriate for subsequences
*
* Revision 6.306  2001/03/12 14:53:46  dondosha
* Uninitialized variable corrections
*
* Revision 6.305  2001/03/08 22:05:48  dondosha
* Split very long database sequences in all BLAST programs
*
* Revision 6.304  2001/02/16 18:45:39  dondosha
* Fixed minor purify errors
*
* Revision 6.303  2001/02/08 20:41:16  dondosha
* Implemented tabulated output for all translated programs
*
* Revision 6.302  2001/02/07 21:12:05  dondosha
* 1. Added Blast Engine functions with callback argument
* 2. Pass output stream from options block to search
*
* Revision 6.301  2001/01/29 22:23:00  madden
* Do not recreate hsp_array
*
* Revision 6.300  2001/01/26 17:43:09  madden
* Comment out unneeded memset
*
* Revision 6.299  2001/01/23 20:25:43  dondosha
* 1. Renamed BlastParceInputString to BlastParseInputString
* 2. Recognize a double quoted string as an option value in
*    BlastParseInputString
*
* Revision 6.298  2001/01/23 18:23:57  madden
* Fix memory leak
*
* Revision 6.297  2001/01/19 16:49:37  madden
* Added helper array to BlastNtGappedScoreInternal
*
* Revision 6.296  2001/01/16 23:16:51  dondosha
* Added 2 arguments and several options to parse_blast_options
*
* Revision 6.295  2001/01/16 20:32:46  kans
* included simutil.h to suppress Mac error
*
* Revision 6.294  2001/01/12 17:10:04  dondosha
* If subject SeqLoc is on a single strand and query on both, swap the strands
*
* Revision 6.293  2001/01/11 18:34:20  dondosha
* Changed error level for nonexistent database from ERROR to FATAL
*
* Revision 6.292  2001/01/09 20:16:27  dondosha
* Implemented from-to location options for both sequences in bl2seq
*
* Revision 6.291  2001/01/05 17:12:48  dondosha
* Correction in previous memory leak fix
*
* Revision 6.290  2001/01/04 15:01:25  dondosha
* Fix for tblastx in blast two sequences engine
*
* Revision 6.289  2001/01/03 21:45:30  dondosha
* Fixed a memory leak - some edit blocks not freed in megablast
*
* Revision 6.288  2000/12/28 18:23:05  madden
* Add -P and -A to parse_blast_options
*
* Revision 6.287  2000/12/19 15:52:47  dondosha
* Forbid reversing query and subject for two sequences megablast
*
* Revision 6.286  2000/12/19 14:52:59  dondosha
* Previous change wrong
*
* Revision 6.285  2000/12/15 15:38:38  dondosha
* Call AdjustOffSetsInSeqAlign with correct query and subject SeqLocs
*
* Revision 6.284  2000/12/15 14:25:41  madden
* Optimization to BlastTranslateUnambiguousSequence
*
* Revision 6.283  2000/12/15 14:23:34  madden
* Use readdb_get_sequence_ex to get sequence faster
*
* Revision 6.282  2000/12/13 22:26:44  dondosha
* Free the ncbi4na-encoded subject sequence after search in two sequences megablast engine
*
* Revision 6.281  2000/12/13 13:51:35  madden
* Free SeqLocPtr in BlastSequencesOnTheFly
*
* Revision 6.280  2000/12/07 17:46:56  dondosha
* Call AdjustOffSetsInSeqAlign for for megablast too
*
* Revision 6.279  2000/12/04 18:51:24  madden
* Fix memory leaks
*
* Revision 6.278  2000/11/29 23:05:00  dondosha
* Keep ncbi4na-encoded subject sequence in search->subject for megablast
*
* Revision 6.277  2000/11/16 19:15:31  dondosha
* Pass back endpoint results in other_returus for Mega BLAST with no traceback
*
* Revision 6.276  2000/11/09 17:28:35  dondosha
* Set block_width to 0 for Mega BLAST in BlastTwoSequences engine
*
* Revision 6.275  2000/11/08 22:21:33  dondosha
* Enabled new tblastn by adding a longest_intron option
*
* Revision 6.274  2000/11/08 20:20:31  dondosha
* Do not free subject in BlastTwoSequencesCore for new tblastn - done elsewhere
*
* Revision 6.273  2000/11/07 16:30:27  madden
* Introduce intermediate score (before linking of HSPs) for blastx and tblastn
*
* Revision 6.272  2000/11/03 20:15:19  dondosha
* Pass the subject sequence to new_link_hsps from two sequences engine
*
* Revision 6.271  2000/11/02 20:15:38  dondosha
* Added functions BlastTwoSequencesByLocWithCallback and BlastTwoSequencesWithCallback
*
* Revision 6.270  2000/11/02 16:36:12  madden
* Fixed another minor problem from merge
*
* Revision 6.269  2000/11/02 16:12:37  madden
* fix Errors during merge of code
*
* Revision 6.268  2000/11/01 16:25:57  madden
* Changes from Futamura for psitblastn
*
* Revision 6.267  2000/10/31 17:51:44  dondosha
* Copy the necessary search block data for multi-threaded megablast
*
* Revision 6.266  2000/10/23 22:17:54  shavirin
* Added creation of "no database found" message in case if database is
* not found.
*
* Revision 6.265  2000/10/18 19:46:29  dondosha
* Fixed bug in BlastTwoSequencesCore for partial subject sequence search
*
* Revision 6.264  2000/10/16 19:34:16  shavirin
* Added possibility to run RPS Blast search from function BioseqBlastEngineByLocEx().
*
* Revision 6.263  2000/10/13 17:32:50  shavirin
* Adjusted calls to readdb_get_header for ASN.1 structured deflines.
*
* Revision 6.262  2000/10/13 16:05:44  shavirin
* Fixed minir bug with reporting database name.
*
* Revision 6.261  2000/10/12 14:45:34  madden
* Break out of loop if hsp is freed
*
* Revision 6.260  2000/10/11 17:14:02  dondosha
* For tblastn traceback convert subject sequence to ncbi4na encoding in BlastTwoSequencesCore
*
* Revision 6.259  2000/10/10 16:11:15  shavirin
* Added check for NULL in the function BLASTCheckHSPInclusion().
*
* Revision 6.258  2000/10/06 19:32:02  shavirin
* Added call to SeqMgrAddToBioseqIndex() for created fake Bioseq.
*
* Revision 6.257  2000/10/05 22:43:10  dondosha
* Use mb_result_struct for Mega BLAST results in two sequences functions
*
* Revision 6.256  2000/10/05 19:57:08  dondosha
* In Mega BLAST, results are saved in and freed from mb_result_struct, not result_struct
*
* Revision 6.255  2000/10/03 21:28:54  shavirin
* Added check for search->pbp for not NULL in BlastSearchBlkDestruct().
*
* Revision 6.254  2000/09/29 21:14:47  shavirin
* Added additional check for inclusion of HSPs after traceback for
* OOF gapped alignment case.
*
* Revision 6.253  2000/09/28 14:57:50  dondosha
* Initialize exact match array for megablast in BlastHitListNew
*
* Revision 6.252  2000/09/25 15:43:36  madden
* Fix for rpsblast, too high expect values getting through
*
* Revision 6.251  2000/09/14 15:05:46  dondosha
* For new tblastn, reset evalues to individual ones before relinking HSPs
*
* Revision 6.250  2000/09/07 13:41:42  madden
* Fix if first start is -1 in DenseSeg
*
* Revision 6.249  2000/09/01 18:29:12  dondosha
* Removed calls to ReadDBFreeSharedInfo and ReadDBCloseMHdrAndSeqFiles
*
* Revision 6.248  2000/08/31 18:37:21  shavirin
* Added check for NULL in BlastMakeCopyQueryDNAP().
*
* Revision 6.247  2000/08/31 16:55:17  shavirin
* Fixed problem with OOF alignment of negative starnd HSPs.
*
* Revision 6.246  2000/08/28 21:53:12  shavirin
* Added function BlastOtherReturnsFree(). Cleaned memory in case of
* tweak_parameters = TRUE. (Freed SeqAlign calculated before RedoAlignmentCore.
*
* Revision 6.245  2000/08/22 20:02:27  dondosha
* Previous change not quite right: use real subject length for all programs
*
* Revision 6.244  2000/08/22 19:42:25  dondosha
* Divide search->subject->length by 3 for tblastn in RealBlastGetGappedAlignmentTraceback
*
* Revision 6.243  2000/08/18 21:27:59  madden
* undo change 6.240 when smith_waterman is not set, the extra alignment is needed when only tweak_parameters is set
*
* Revision 6.242  2000/08/18 20:12:29  dondosha
* Do not use search->query_id in megablast, use only qid_array
*
* Revision 6.241  2000/08/08 21:43:35  shavirin
* Initialized GapAlignBlkPtr for the value of discontinuous parametrers.
*
* Revision 6.240  2000/08/03 22:25:36  shavirin
* Removed redundant gapped Traceback in case when tweak_parameters or
* smith_waterman is set.
*
* Revision 6.239  2000/07/31 23:08:13  dondosha
* Do not go over the end of the HSP in subject sequence when computing start for gapped alignment
*
* Revision 6.238  2000/07/25 18:12:03  shavirin
* WARNING: This is no-turning-back changed related to S&W Blast from
* Alejandro Schaffer
*
* Revision 6.237  2000/07/25 16:54:26  shavirin
* Corrected functions initializing gap_align in case of OOF gapping.
*
* Revision 6.236  2000/07/18 22:33:02  shavirin
* Adjusted start for gapped alignment in OOF case.
*
* Revision 6.235  2000/07/17 14:26:08  shavirin
* Added support for Out of frame gapping.
*
* Revision 6.234  2000/07/13 18:33:28  madden
* Fix for exploded hits with pdb
*
* Revision 6.233  2000/07/11 18:38:02  madden
* decreased size of helper array, added prefetch to BlastGappedScoreInternal
*
* Revision 6.232  2000/07/10 15:23:30  dondosha
* Moved check query_invalid from BlastTwoSequencesCoreEx to BlastTwoSequencesCore
*
* Revision 6.231  2000/07/10 15:06:23  madden
* Use helper array in BlastGappedScoreInternal to reduce cache misses
*
* Revision 6.230  2000/06/30 17:52:44  madden
* Move AWAKE_THR_MIN_SIZE to blastdef.h
*
* Revision 6.229  2000/06/29 21:27:02  dondosha
* Fixed memory leaks in culling by similarity
*
* Revision 6.228  2000/06/29 19:19:39  madden
* Fix minus strand offset in BlastConvertDNASeqLoc
*
* Revision 6.227  2000/06/26 20:15:34  shavirin
* Fixed coordinates transfer in the function BlastConvertDNASeqLoc().
*
* Revision 6.226  2000/06/23 20:17:42  madden
* Optimization for CheckGappedAlignmentsForOverlap (remove n-squared hsp check)
*
* Revision 6.225  2000/06/23 15:22:43  madden
* Fix problem with removing translated hits with different frames
*
* Revision 6.224  2000/06/21 18:02:25  dondosha
* In BlastSaveCurrentHspGapped no need to allocate new memory for hsp_array
*
* Revision 6.223  2000/06/21 15:10:27  madden
* efficiency in BlastGappedScoreInternal
*
* Revision 6.222  2000/06/21 12:53:22  madden
* Do each frame separately in CheckGappedScoreInternal for efficiency
*
* Revision 6.221  2000/06/20 16:45:36  dondosha
* Fixed a minor bug in revision 6.219
*
* Revision 6.220  2000/06/19 20:07:19  madden
* Skip transferring sequence to blastna format
*
* Revision 6.219  2000/06/19 19:16:19  dondosha
* Optimized reallocation of hsp array when it is overflowing
*
* Revision 6.218  2000/06/15 15:31:26  dondosha
* Added two sequences BLAST functions returning SearchBlk instead of SeqAlign;added code to cluster hits and keep only one hit per cluster - disabled so far; enabled two sequences BLAST for tblastn
*
* Revision 6.217  2000/06/13 20:54:38  shavirin
* Added return of EFF_SEARCH_SPACE in the function BlastOtherReturnsPrepare
*
* Revision 6.216  2000/06/08 20:34:15  madden
* add explode_seqids option to show all ids in a defline
*
* Revision 6.215  2000/05/24 20:53:48  dondosha
* Fixed a bug in previous change
*
* Revision 6.214  2000/05/24 19:49:07  dondosha
* Create qid_array for the new search in BlastSearchDuplicate, if megablast
*
* Revision 6.213  2000/05/22 19:49:35  dondosha
* Initialize vnp to NULL in BlastSeqLocFilterEx
*
* Revision 6.212  2000/05/16 20:00:02  madden
* fix for formatting db names
*
* Revision 6.211  2000/05/12 19:41:54  dondosha
* Free qid_array in BlastSearchBlkDestruct
*
* Revision 6.210  2000/05/05 20:10:22  madden
* Add vecscreen filtering capability
*
* Revision 6.209  2000/04/29 18:55:53  wheelan
* temporary fix for BlastTwoSequences NULL return problem
*
* Revision 6.208  2000/04/28 16:52:31  madden
* Fix for ungapped search of subset databases
*
* Revision 6.207  2000/04/10 17:26:28  madden
* Add BLASTResultFreeHsp to free memory as it is no longer needed
*
* Revision 6.206  2000/04/10 15:24:49  dondosha
* Enabled use of MegaBlast for BlastTwoSequences
*
* Revision 6.205  2000/04/07 16:57:45  shavirin
* Transfered queue parameters in BlastSearchBlkDuplicate() function.
*
* Revision 6.204  2000/04/06 17:33:57  madden
* Check if pointer is NULL in BlastGetAllowedGis
*
* Revision 6.203  2000/04/03 21:23:18  dondosha
* Do not construct ewp_params and ewp for MegaBlast search
*
* Revision 6.202  2000/04/03 20:05:27  madden
* Free lh_helper on tmp_hitlist, fixes leak
*
* Revision 6.201  2000/03/31 19:11:06  dondosha
* Changed some names related to MegaBlast
*
* Revision 6.200  2000/03/31 16:45:43  dondosha
* Enabled blastx for BlastTwoSequences search
*
* Revision 6.199  2000/03/30 21:44:22  madden
* Add BLASTResultHitlistFreeEx that checks Heap integrity
*
* Revision 6.198  2000/03/29 22:18:02  dondosha
* Moved adjustment of offsets in blastn to BlastSaveCurrentHitlist, added gap info processing for MegaBlast
*
* Revision 6.197  2000/03/22 17:58:54  dondosha
* Duplicate entire list of query_ids in BlastSearchBlkDuplicate
*
* Revision 6.196  2000/03/08 20:34:30  madden
* Add BlastGetFirstGiofSubset, BlastGetAllowedGis returns primary SeqId
*
* Revision 6.195  2000/03/03 18:15:52  dondosha
* Fixed bugs and memory leaks in MegaBlast related code
*
* Revision 6.194  2000/03/03 17:58:23  shavirin
* Added new function BlastConvertDNASeqLoc()
*
* Revision 6.193  2000/03/01 14:37:45  dondosha
* Adjust query offsets after search for all 3 versions of blastn
*
* Revision 6.192  2000/02/29 18:06:07  dondosha
* In case of MegaBlast save correct query ids in seqaligns
*
* Revision 6.191  2000/02/24 23:21:27  dondosha
* Adjust context offsets before gapped alignment to avoid strand crossover
*
* Revision 6.190  2000/02/23 20:51:05  dondosha
* Modifications for blastn to concatenate strands - handling of query offsets
*
* Revision 6.189  2000/02/17 21:23:10  shavirin
* Added parameter is_rps_blast.
*
* Revision 6.188  2000/02/17 19:02:09  shavirin
* Removed all references to absolete theCacheSize variable.
*
* Revision 6.187  2000/02/17 18:30:56  shavirin
* Added translated DNA filtering for RPS Blast
*
* Revision 6.186  2000/02/17 14:38:27  madden
* Duplicate filter_string for multiple threads
*
* Revision 6.185  2000/02/16 21:49:16  shavirin
* Fixed some memory leaks.
*
* Revision 6.184  2000/02/15 19:16:26  shavirin
* MemFree(pbp->filter_string) in BlastSearchBlkDestruct
*
* Revision 6.183  2000/02/14 16:15:50  madden
* Revert to 6.179
*
* Revision 6.182  2000/02/11 22:03:03  shavirin
* Returned back previous change.
*
* Revision 6.181  2000/02/11 21:25:58  shavirin
* Removed call to BlastLinkHsps() function for tblastn program.
*
* Revision 6.180  2000/02/11 20:45:54  dondosha
* Adjust the second strand offsets after blastn search
*
* Revision 6.179  2000/02/11 16:40:53  egorov
* The parse_blast_options is made public.
*
* Revision 6.178  2000/02/04 22:31:38  kans
* test subject_bsp for NULL before dereferencing in BlastTwoSequencesByLocEx
*
* Revision 6.177  2000/02/04 16:13:15  shavirin
* Returned changes done in Revision 6.172.
*
* Revision 6.176  2000/02/02 18:22:05  madden
* Free memory for LinkHelpStruct
*
* Revision 6.175  2000/02/01 22:13:26  dondosha
* Added code related to greedy basic gapped alignment
*
* Revision 6.174  2000/01/28 16:45:53  madden
* HitRangeToSeqLoc called with combine TRUE
*
* Revision 6.173  2000/01/26 22:01:56  madden
* Add function BlastGetProgramName
*
* Revision 6.172  2000/01/14 18:28:11  shavirin
* Some WordExtention* function mad external.
*
* Revision 6.171  2000/01/12 21:46:19  dondosha
* Minor memory leak clean-up (routine BlastSeqLocFilterEx)
*
* Revision 6.170  2000/01/12 18:54:44  madden
* Do not free bestid to fix problem
*
* Revision 6.169  2000/01/11 17:12:51  shavirin
* Added handling of the new parameter theCacheSize.
*
* Revision 6.168  2000/01/11 15:32:47  dondosha
* Fixed memory leaks in opening shared header and sequence file memory maps
*
* Revision 6.167  2000/01/04 21:56:59  madden
* Add NULLB to both ends of db sequence before gap extend, use dynamic buffer for blast options in repeat filtering
*
* Revision 6.166  2000/01/03 17:38:33  shavirin
* Added check for rdfp in BlastGetAllowedGis() function.
*
* Revision 6.165  1999/12/31 14:23:20  egorov
* Add support for using mixture of real and maks database with gi-list files:
* 1. Change logic of creating rdfp list.
* 2. BlastGetDbChunk gets real databases first, then masks.
* 3. Propoper calculation of database sizes using alias files.
* 4. Change to CommonIndex to support using of mask databases.
* 5. Use correct gis in formated output (BlastGetAllowedGis()).
* 6. Other small changes
*
* Revision 6.164  1999/12/22 22:00:35  dondosha
* Destruct the header and sequence memory maps separately before destructing the search structure
*
* Revision 6.163  1999/12/22 21:08:36  shavirin
* Rewritten function BlastNewFindWords() added function BlastNewFindWordsEx()
*
* Revision 6.160  1999/12/21 20:02:45  egorov
* Fix memory leak.
*
* Revision 6.159  1999/12/17 22:22:57  madden
* New masking parameters from Wojtek
*
* Revision 6.158  1999/12/16 19:08:36  egorov
* Check rdfp for NULL before using.  Bug reported by Patrick and Sergei Sh.
*
* Revision 6.157  1999/12/15 17:42:26  egorov
* Change BlastGetAllowedGis() to handle gi's belonged to a database alias.
*
* Revision 6.156  1999/12/13 21:53:02  madden
* Some fixes for repeat masking
*
* Revision 6.155  1999/11/26 22:11:26  madden
* Added BlastNT functions for nucl. extensions
*
* Revision 6.154  1999/11/24 15:21:38  egorov
* Avoid GCC warning
*
* Revision 6.153  1999/11/09 14:14:12  madden
* Start alive thread for masking only if query is above min size
*
* Revision 6.152  1999/11/02 15:32:36  madden
* Allow setting of repeat filtering options and database
*
* Revision 6.151  1999/11/01 20:18:22  egorov
* New format of filter_string
*
* Revision 6.150  1999/10/27 21:33:02  madden
* Use housekeeping threads only for larger sequences
*
* Revision 6.149  1999/10/18 20:06:52  shavirin
* evalue_compare_hits() : In case of equal scores and E-values order
* will be determined by subject id
*
* Revision 6.148  1999/10/18 16:15:04  egorov
* Bug fixed
*
* Revision 6.147  1999/10/15 20:52:10  shavirin
* Fixed bug with seq_id_list initialization
*
* Revision 6.146  1999/10/12 21:50:47  shavirin
* Added intialization of db_chunk_size in BlastThrInfoNew().
*
* Revision 6.145  1999/10/05 17:42:55  shavirin
* Removed global variables from blast.c
*
* Revision 6.144  1999/10/01 18:26:56  madden
* Check for search->rdfp before search->rdfp->oidlist
*
* Revision 6.143  1999/09/28 20:14:33  madden
* Joerg changes to mimize cache misses
*
* Revision 6.142  1999/09/22 20:58:49  egorov
* OID list change
*
* Revision 6.141  1999/09/16 16:55:12  madden
* Changes for long words in blastn
*
* Revision 6.140  1999/09/03 17:23:25  madden
* Fixed bug in CheckStartForGappedAlignment
*
* Revision 6.139  1999/09/01 19:21:06  shavirin
* Added propagation of the score for discontinuous alignment in
* functions: RealBlastGetGappedAlignmentTraceback() and BioseqBlastEngineCore()
*
* Revision 6.138  1999/08/27 18:07:34  shavirin
* Passed parameter decline_align from top to the engine.
*
* Revision 6.137  1999/08/20 20:54:12  madden
* place sentinel byte at beginning of nt sequence for ALIGN
*
* Revision 6.136  1999/08/20 19:48:13  madden
* Changed call to BlastSearchBlkNew(Extra), removed use of version array
*
* Revision 6.135  1999/08/20 16:35:25  shavirin
* Added protection against invalid program name in BlastGetTypes().
*
* Revision 6.134  1999/08/06 18:53:57  madden
* Added calls to lookup_position_aux_destruct
*
* Revision 6.133  1999/08/05 19:01:29  madden
* Add check for NULL search or invalid query in BlastTwoSequencesCore
*
* Revision 6.132  1999/07/01 13:03:24  sicotte
* Updated for DenseDiag and Moved seqalign_reverse_strand from blastutl.c(blast.h) to SeqAlignListReverseStrand in salpedit.ch and fixed call in salutil.c
*
* Revision 6.131  1999/06/24 17:24:12  madden
* Fix bug in GetSeqAlignCount when SeqAlignPtr is NULL
*
* Revision 6.130  1999/06/18 21:17:58  madden
* Check that an exact match gives a positive value when making words for blast2seqs
*
* Revision 6.129  1999/06/14 15:20:26  madden
* Produce temporary BLAST_HitList to fix blastx core-dump
*
* Revision 6.128  1999/05/27 17:33:05  madden
* Fixed Int2 (should have been Int4) problem
*
* Revision 6.127  1999/05/25 13:37:49  madden
* Make smallest float 1.0e-180
*
* Revision 6.126  1999/05/19 12:44:00  madden
* Change in longest_db_seq for multiple db search
*
* Revision 6.125  1999/05/13 13:48:11  madden
* Only filter out hits if on same strand
*
* Revision 6.124  1999/04/15 13:24:35  madden
* Fix for sum stats problems
*
* Revision 6.123  1999/04/13 19:16:47  madden
* Check that two HSPs are on same strand before deleting one
*
* Revision 6.122  1999/04/12 20:24:54  egorov
* Fix MT problem
*
* Revision 6.121  1999/04/01 21:42:46  madden
* Fix memory leaks when gi list is used
*
* Revision 6.120  1999/04/01 14:18:58  madden
* Fixed memory leaks with gi_list
*
* Revision 6.119  1999/03/31 15:46:52  madden
* Removed unused code and variables
*
* Revision 6.118  1999/03/17 13:21:06  madden
* Fix comment in comment problem
*
* Revision 6.117  1999/03/16 19:27:36  egorov
* More type castings
*
* Revision 6.116  1999/03/12 17:19:59  egorov
* More type casting fixes
*
* Revision 6.115  1999/03/12 15:03:45  egorov
* Add proper Int4-long type casting
*
* Revision 6.114  1999/03/04 14:18:09  egorov
* Do correct filter masking when query is seqloc
* The only BlastMaskTheResidues() function is changed:
*
* Revision 6.113  1999/02/22 21:59:05  madden
* binary search in GetAllowedGis function
*
* Revision 6.112  1999/02/22 17:32:46  madden
* Fix memory leak
*
* Revision 6.111  1999/02/18 21:18:23  madden
* Optimization
*
* Revision 6.110  1999/02/17 13:23:01  madden
* Added hsp_num_max
*
* Revision 6.109  1999/02/11 13:53:46  madden
* Added combine Boolean to HitRangeToSeqLoc, fixed mem leak
*
* Revision 6.108  1999/01/28 17:20:57  madden
* Check do_sum_stats for linking, Int2 to Int4, UMR
*
* Revision 6.107  1999/01/28 16:05:49  madden
* HspArrayPurge change, HSPs saved more efficiently
*
* Revision 6.106  1999/01/26 18:27:23  madden
* handle delta sequences correctly
*
* Revision 6.105  1999/01/26 17:59:26  madden
* ContextToFrame no longer static
*
* Revision 6.104  1999/01/25 21:31:25  madden
* Check for illegal chars when nucl. query is translated
*
* Revision 6.103  1999/01/25 19:04:37  madden
* prevent core-dump when query is empty
*
* Revision 6.102  1999/01/20 21:05:33  madden
* Look for repeats on both strands
*
* Revision 6.101  1999/01/19 13:29:24  madden
* Change to HspArrayPurge
*
 * Revision 6.100  1998/12/31 18:17:08  madden
 * Added strand option
 *
 * Revision 6.99  1998/12/31 15:36:07  victorov
 * filtering internals is now based on SeqLoc instead of Bioseq
 *
 * Revision 6.98  1998/12/18 16:20:18  madden
 * efficiencies
 *
 * Revision 6.97  1998/12/15 14:11:29  madden
 * Change to permit an arbitrary number of HSPs
 *
 * Revision 6.96  1998/11/30 15:58:20  madden
 * Added CheckStartForGappedAlignment
 *
 * Revision 6.95  1998/11/27 15:24:12  madden
 * Duplicated handle_results and query_id if SearchBlk duplicated
 *
 * Revision 6.94  1998/11/16 17:39:23  kans
 * added FALSE for new paramter to FilterCC
 *
 * Revision 6.93  1998/11/06 14:13:01  madden
 * Added call to AdjustOffSetsInSeqAlign in BioseqBlastEngineByLocEx
 *
 * Revision 6.92  1998/10/21 13:44:16  madden
 * Fixed UMR found by purify
 *
 * Revision 6.91  1998/10/20 19:57:21  madden
 * Run dust if filtering is selected for nt
 *
 * Revision 6.90  1998/10/13 20:37:53  madden
 * Use IS_residue after call to SeqPortGetResidue
 *
 * Revision 6.89  1998/09/24 15:26:38  egorov
 * Fix lint complaints
 *
 * Revision 6.88  1998/09/16 19:00:16  madden
 * Added subset Boolean
 *
 * Revision 6.87  1998/09/15 13:12:29  madden
 * Fixed memory leak
 *
 * Revision 6.86  1998/09/14 15:11:18  egorov
 * Add support for Int8 length databases; remove unused variables
 *
 * Revision 6.85  1998/09/04 20:48:48  madden
 * typo fix (= instead of ==)
 *
 * Revision 6.84  1998/09/03 20:23:42  madden
 * Copied seq_ext and seq_ext_type in MakeFakeBioseq
 *
 * Revision 6.83  1998/09/03 19:41:09  madden
 * do not switch sequences for Blast2Sequences if filtering is performed
 *
 * Revision 6.82  1998/08/24 14:59:59  madden
 * readdb_get_sequence_ex function
 *
 * Revision 6.81  1998/07/30 19:00:56  madden
 * Fix memory leak
 *
 * Revision 6.80  1998/07/29 21:29:45  madden
 * Fixed UMR with longest_db_seq that showed up in Blast 2 sequences
 *
 * Revision 6.79  1998/07/28 21:18:35  madden
 * Change to BLAST_ExtendWordParamsNew saves memory
 *
 * Revision 6.78  1998/07/24 14:58:53  madden
 * Jinqhuis call to SeqLocRevCmp put back
 *
 * Revision 6.77  1998/07/22 20:31:51  madden
 * Replaced cutvalue of 1000000 with INT4_MAX
 *
 * Revision 6.76  1998/07/22 12:17:03  madden
 * Added BioseqHitRange call for repeat filtering
 *
 * Revision 6.75  1998/07/21 20:58:10  madden
 * Changes to allow masking at hash only
 *
 * Revision 6.74  1998/07/20 15:51:28  zjing
 * add a check for plus-minus before SeqLocRevCmp
 *
 * Revision 6.73  1998/07/17 15:39:59  madden
 * Changes for Effective search space.
 *
 * Revision 6.72  1998/07/14 21:31:43  madden
 * Fix for incorrectly sorted HSP bug and speed-up of CheckHspOverlap
 *
 * Revision 6.71  1998/07/06 13:39:04  madden
 * Fixed improper use of Int4 in parse_seg_options
 *
 * Revision 6.70  1998/07/02 21:00:39  egorov
 * Remove memory leak in threaded version
 *
 * Revision 6.69  1998/06/12 22:09:14  madden
 * Added call to SegParamsFree
 *
 * Revision 6.68  1998/06/12 16:08:51  madden
 * BlastHitRange stuff
 *
 * Revision 6.67  1998/06/08 15:07:32  madden
 * Fixed bug in BlastConvertProteinSeqLoc
 *
 * Revision 6.66  1998/06/04 16:23:17  madden
 * Use new seg
 *
 * Revision 6.65  1998/05/28 19:59:58  madden
 * Zhengs new culling code
 *
 * Revision 6.64  1998/05/22 20:20:38  madden
 * Added BlastTwoSequencesByLocEx and BlastTwoSequencesEx
 *
 * Revision 6.63  1998/05/18 17:58:31  madden
 * fixed parsing of coil-coil options, added parsing of dust options
 *
 * Revision 6.62  1998/05/17 16:28:41  madden
 * Allow changes to filter options and cc filtering.
 *
 * Revision 6.61  1998/05/05 14:05:35  madden
 * Added functions BlastStartAwakeThread and BlastStopAwakeThread
 *
 * Revision 6.60  1998/04/28 21:04:19  madden
 * Reset number of HSPs to zero if relinking
 *
 * Revision 6.59  1998/04/24 21:52:09  madden
 * Protection against NULL pointers
 *
 * Revision 6.58  1998/04/24 19:10:59  egorov
 * Fix bug when if wordsize == 2 blastall produces extra alignments
 *
 * Revision 6.57  1998/04/23 21:15:09  egorov
 * Show exact matching even if score is below threshold (case of two sequences)
 *
 * Revision 6.56  1998/04/15 20:24:54  madden
 * BlastMaskTheResidues optimized
 *
 * Revision 6.55  1998/04/10 17:46:58  madden
 * Changed FALSE to NULL in BioseqSeg
 *
 * Revision 6.54  1998/04/02 21:12:55  madden
 * Properly set value for linking HSPs in blastx and tblastn
 *
 * Revision 6.53  1998/04/01 22:47:35  madden
 * Check for query_invalid flag
 *
 * Revision 6.52  1998/03/26 14:20:20  madden
 * Changed GetScoreSetFromBlastResultHsp1 from static to LIBCALL
 *
 * Revision 6.51  1998/03/25 22:28:16  madden
 * Changes to allow random access BLAST by gi
 *
 * Revision 6.50  1998/03/24 15:38:25  madden
 * Use BlastDoubleInt4Ptr to keep track of gis and ordinal_ids
 *
 * Revision 6.49  1998/03/19 22:16:24  madden
 * Changes to allow blasting by gi list
 *
 * Revision 6.48  1998/03/18 14:14:11  madden
 * Support random access by gi list
 *
 * Revision 6.47  1998/03/16 17:41:59  madden
 * Fixed leaks
 *
 * Revision 6.46  1998/03/14 18:28:10  madden
 * Added BioseqBlastEngineEx
 *
 * Revision 6.45  1998/03/09 16:35:10  madden
 * Fixed bug with tblastn and blastx gapped searches
 *
 * Revision 6.44  1998/02/27 14:32:33  madden
 * Functions moved to blastool.c
 *
 * Revision 6.43  1998/02/26 22:34:27  madden
 * Changes for 16 bit windows
 *
 * Revision 6.42  1998/02/26 19:12:39  madden
 *  Removed AdjustOffSetsInSeqAlign, added BlastNtFindWords BlastPopulateAllWordArrays BlastFindWords and BlastNewFindWords
 *
 * Revision 6.41  1998/02/24 22:47:06  madden
 * Fixed problem with Option validation
 *
 * Revision 6.40  1998/02/23 16:09:57  madden
 * Corrected from offset for subject in tblastx search
 *
 * Revision 6.39  1998/02/19 17:17:05  madden
 * Use of Int4 rather than Int2 when pruning SeqAlign
 *
 * Revision 6.38  1998/02/12 21:50:39  madden
 * protection against NULL hitlist in blastx and tblastn
 *
 * Revision 6.37  1998/02/11 17:18:19  madden
 * Made BlastGetGappedAlignmentTraceback functions to BlastGetGapAlgnTbck (shorter than 32 chars)
 *
 * Revision 6.36  1998/01/31 21:34:09  madden
 * Fix to SeqAlign pruning
 *
 * Revision 6.35  1998/01/06 18:26:22  madden
 * Use SeqLocLen rather than bsp->length, wordsize done properly for nucl
 *
 * Revision 6.34  1998/01/05 22:41:40  madden
 * Added seqalign_reverse_strand
 *
 * Revision 6.33  1998/01/05 20:53:16  madden
 * Added ability to align minus-minus or plus-minus in BlastTwoSeqsByLoc
 *
 * Revision 6.32  1998/01/05 16:46:55  madden
 * One or both strands can be searched, as opposed to only both, changes to number of contexts
 *
 * Revision 6.31  1997/12/31 17:52:09  madden
 * Change to BLAST_WordFinderNew
 *
 * Revision 6.30  1997/12/23 19:16:52  madden
 * Minor efficiency in ExtendWordExit
 *
 * Revision 6.29  1997/12/23 18:12:34  madden
 * Changes for range-dependent blast
 *
 * Revision 6.28  1997/12/12 20:38:55  madden
 * ContextToFrame lost last parameter, fix to sprintf
 *
 * Revision 6.27  1997/12/11 22:22:24  madden
 * Proper casting of variables
 *
 * Revision 6.26  1997/12/10 22:43:09  madden
 * proper casting
 *
 * Revision 6.25  1997/12/01 22:07:10  madden
 * Changed call to BLASTOptionValidateEx
 *
 * Revision 6.24  1997/11/28 18:19:33  madden
 * Changes to TxDfDbInfoNew
 *
 * Revision 6.23  1997/11/18 22:23:20  madden
 * Added BLASTOptionSetGapParams
 *
 * Revision 6.22  1997/11/14 17:15:29  madden
 * Realign matches when they contain ambiguities in blastx/tblastn
 *
 * Revision 6.21  1997/11/07 00:49:02  madden
 * Added call to BLAST_MatrixFill
 *
 * Revision 6.20  1997/10/29 22:11:13  madden
 * ABS value of frames
 *
 * Revision 6.19  1997/10/24 20:44:52  madden
 * Removed BlastSetReadDB and BlastGetReadDB_ID
 *
 * Revision 6.18  1997/10/22 21:46:34  madden
 * Changed default values
 *
 * Revision 6.17  1997/10/21 20:39:18  madden
 * Fix for more alignments than descriptions.
 *
 * Revision 6.16  1997/10/21 19:50:00  madden
 * Fix for no valid query sequence and hitlist_max of 1
 *
 * Revision 6.15  1997/10/03 21:27:28  madden
 * Added BlastGetTypes
 *
 * Revision 6.14  1997/10/02 17:29:29  madden
 * Added PrintDbInformationBasic
 *
 * Revision 6.13  1997/10/01 13:35:31  madden
 * Changed BLAST_VERSION to BLAST_ENGINE_VERSION
 *
 * Revision 6.12  1997/09/30 20:03:07  madden
 * Saved db filename in dbinfo
 *
 * Revision 6.11  1997/09/24 22:36:35  madden
 * Fixes for MT multidb searches
 *
 * Revision 6.10  1997/09/23 16:43:41  madden
 * removed unneeded DenseSegPtr
 *
 * Revision 6.9  1997/09/22 18:18:35  madden
 * Added umlaut to Schaffer in reference
 *
 * Revision 6.8  1997/09/18 22:22:03  madden
 * Added prune functions
 *
 * Revision 6.7  1997/09/16 16:54:09  kans
 * return FASLE instead of NULL for Boolean value
 *
 * Revision 6.6  1997/09/16 16:31:28  madden
 * More changes for multiple db runs
 *
 * Revision 6.5  1997/09/11 18:49:31  madden
 * Changes to enable searches against multiple databases.
 *
 * Revision 6.4  1997/09/10 21:28:00  madden
 * Changes to set CPU limits
 *
 * Revision 6.3  1997/09/08 16:25:32  madden
 * Fixed bug that did not mask low-complexity regions at the end of a query
 *
 * Revision 6.2  1997/08/27 14:46:51  madden
 * Changes to enable multiple DB searches
 *
 * Revision 6.1  1997/08/26 15:05:26  madden
 * Fix for negative effective search space
 *
 * Revision 6.0  1997/08/25 18:52:49  madden
 * Revision changed to 6.0
 *
 * Revision 1.105  1997/08/22 18:37:43  madden
 * Added function BlastOtherReturnsPrepare
 *
 * Revision 1.104  1997/08/20 21:43:34  madden
 * Added page numbers
 *
 * Revision 1.103  1997/08/14 21:07:08  madden
 * ignored gapped for tblastx
 *
 * Revision 1.102  1997/08/14 14:30:35  madden
 * BlastNewFindWords called with range set for ranged blast
 *
 * Revision 1.101  1997/07/31 21:18:11  madden
 * Removed left-over file from seg
 *
 * Revision 1.100  1997/07/30 16:39:30  madden
 * Print gap existence and extension parameters for blastn
 *
 * Revision 1.99  1997/07/30 16:31:37  madden
 * tblastx prepares StdSeg
 *
 * Revision 1.98  1997/07/29 17:07:27  madden
 * better tblastx error messages.
 *
 * Revision 1.97  1997/07/25 15:39:49  madden
 * Corrected citation
 *
 * Revision 1.96  1997/07/25 13:47:46  madden
 * Made buffer longer to avoid ABR
 *
 * Revision 1.95  1997/07/23 20:59:02  madden
 * Changed blastn defaults for gap opening and extension
 *
 * Revision 1.94  1997/07/22 17:22:41  madden
 * Added NULL arg (for index callback) to BLASTSetUpSearch funcs
 *
 * Revision 1.93  1997/07/21 17:36:42  madden
 * Added BlastGetReleaseDate
 *
 * Revision 1.92  1997/07/18 20:57:02  madden
 * Added functions BlastGetVersionNumber and BlastGetReference
 *
 * Revision 1.91  1997/07/18 14:26:20  madden
 * call to AcknowledgeBlastQuery changed, SeqId no longer deleted there.
 *
 * Revision 1.90  1997/07/16 20:34:35  madden
 * Added function BlastConvertProteinSeqLoc
 *
 * Revision 1.89  1997/07/15 20:36:14  madden
 * Added BioseqSeg and SeqLocSeg
 *
 * Revision 1.88  1997/07/14 20:11:10  madden
 * Removed unused variables
 *
 * Revision 1.87  1997/07/14 16:15:41  madden
 * call to BLASTOptionValidateEx in BlastBioseqEngine
 *
 * Revision 1.86  1997/07/14 15:31:49  madden
 * Added BlastErrorMessage functions
 *
 * Revision 1.85  1997/07/11 19:29:37  madden
 * Added function BioseqBlastEngineByLoc
 *
 * Revision 1.84  1997/07/10 20:35:43  madden
 * Changed parameter output
 *
 * Revision 1.83  1997/07/02 20:18:39  madden
 * Made continuous SeqAlign the default
 *
 * Revision 1.82  1997/07/02 18:31:39  madden
 * changed defaults
 *
 * Revision 1.81  1997/07/01 19:15:44  madden
 * More changes to FormatBlastParameters
 *
 * Revision 1.80  1997/07/01 17:51:36  madden
 * changed gap_decay rate, gap_prob
 *
 * Revision 1.79  1997/07/01 15:44:44  madden
 * Changes to FormatBlastParameters per S. Altschul
 *
 * Revision 1.78  1997/06/30 15:50:06  madden
 * Changes to FormatBlastParameters
 *
 * Revision 1.77  1997/06/27 22:18:51  madden
 * Updated default parameters
 *
 * Revision 1.76  1997/06/27 14:31:08  madden
 * Added functions BlastAddSeqIdToList and BlastSeqIdListDestruct
 *
 * Revision 1.75  1997/06/24 13:51:27  madden
 * Fixed SeqLoc leak
 *
 * Revision 1.74  1997/06/23 20:49:31  madden
 * BLASTOptionValidate checks for proper gapping parameters
 *
 * Revision 1.73  1997/06/20 13:11:33  madden
 * Made AdjustOffSetsInSeqAlign non-static, Fixed purify error
 *
 * Revision 1.72  1997/06/06 21:29:48  madden
 * Added Boolean html to AcknowledgeBlastQuery and PrintDbInformation
 *
 * Revision 1.71  1997/06/06 19:49:46  madden
 * Added BlastMakeFakeBioseq and BlastDeleteFakeBioseq
 *
 * Revision 1.70  1997/05/30 21:05:59  madden
 * corrected call to readdb_new
 *
 * Revision 1.69  1997/05/27 20:20:02  madden
 * Added function BlastMaskTheResidues
 *
 * Revision 1.68  1997/05/22 21:24:55  madden
 * Added support for final gapX dropoff value
 *
 * Revision 1.67  1997/05/20 17:52:58  madden
 * Added functions BlastTwoSequencesByLoc and BlastSequencesOnTheFlyByLoc
 *
 * Revision 1.66  1997/05/12 21:34:16  madden
 * readdb_new allows indeterminate database type
 *
 * Revision 1.65  1997/05/06 22:17:59  madden
 * Duplicate dblen_eff, dbseq_num, and length_adjustment
 *
 * Revision 1.64  1997/05/01  15:53:19  madden
 * Addition of extra KarlinBlk's for psi-blast
 *
 * Revision 1.63  1997/04/29  14:07:45  madden
 * Fixed problem with hits failing PreliminaryGapping; fixed UMR.
 *
 * Revision 1.62  1997/04/25  20:23:06  madden
 * Freed SeqPort to clear mem leak.
 *
 * Revision 1.61  1997/04/24  14:43:07  madden
 * Fix for minus strand (ungapped) tblastn runs.
 *
 * Revision 1.60  1997/04/23  21:56:07  madden
 * Changes in BlastGetGappedAlignmentTraceback for in-frame gapping tblastn.
 *
 * Revision 1.59  1997/04/22  14:00:14  madden
 * Removed unused variables.
 *
 * Revision 1.58  1997/04/22  13:04:19  madden
 * Changes for in-frame blastx gapping.
 *
 * Revision 1.57  1997/04/21  15:35:26  madden
 * Fixes for 'gapped' StdSegs.
 *
 * Revision 1.56  1997/04/18  17:08:35  madden
 * Corrected printing of threshold values.
 *
 * Revision 1.55  1997/04/17  22:12:43  madden
 * Fix for offset in GetStartForGappedAlignment.
 *
 * Revision 1.54  1997/04/17  22:07:48  madden
 * Changes to allow in-frame gapped tblastn.
 *
 * Revision 1.53  1997/04/15  22:02:59  madden
 * Set original_length1 for translating searches.
 *
 * Revision 1.52  1997/04/14  21:31:58  madden
 * Checking for NULL pointer.
 *
 * Revision 1.51  1997/04/14  15:59:47  madden
 * Changes for ungapped psi-blast.
 *
 * Revision 1.50  1997/04/11  21:18:45  madden
 * Added GetSequenceWithDenseSeg.
 *
 * Revision 1.49  1997/04/11  19:02:49  madden
 * Changes for in-frame blastx, tblastn gapping.
 *
 * Revision 1.48  1997/04/09  20:01:53  madden
 * Copied seqid_list from search structure to duplicate, for use on threads.
 *
 * Revision 1.47  1997/04/08  16:27:28  madden
 * Fixed leaks; fix for blastn formatting of parameters.
 *
 * Revision 1.46  1997/04/07  21:42:56  madden
 * Freed SeqLocPtr used for dust.
 *
 * Revision 1.45  1997/04/07  18:17:09  madden
 * Formatted parameters for Stephen.
 *
 * Revision 1.44  1997/04/04  20:44:09  madden
 * Added check for NULL return.
 *
 * Revision 1.43  1997/04/04  20:42:35  madden
 * Added function BioseqBlastEngineCore.
 *
 * Revision 1.42  1997/04/03  19:50:56  madden
 * Changes to use effective database length instead of the length of each
 * sequence in statistical calculations.
 *
 * Revision 1.41  1997/03/27  22:30:51  madden
 * Correctly checked for overlapping HSP's.
 *
 * Revision 1.40  1997/03/20  22:56:24  madden
 * Added gap_info to hsp.
 *
 * Revision 1.39  1997/03/20  21:52:10  madden
 * Fix for segmented query BioseqPtr when gapped alignment is performed.
 *
 * Revision 1.39  1997/03/20  21:52:10  madden
 * Fix for segmented query BioseqPtr when gapped alignment is performed.
 *
 * Revision 1.38  1997/03/14  22:06:11  madden
 * fixed MT bug in BlastReevaluateWithAmbiguities.
 *
 * Revision 1.37  1997/03/14  15:57:23  madden
 * Removed superfluous call to SeqAlignNew
 *
 * Revision 1.36  1997/03/14  15:22:11  madden
 * Fixed UMR of seqalign in BlastTwoSequencesCore.
 *
 * Revision 1.35  1997/03/11  14:38:40  madden
 * Added BlastSequencesOnTheFly and BlastTwoSequencesCore.
 *
 * Revision 1.34  1997/03/07  22:35:54  madden
 * Fix for BLASTOptionNew.
 *
 * Revision 1.33  1997/03/07  21:58:36  madden
 * Added Boolean gapped argument to BLASTOptionNew.
 *
 * Revision 1.32  1997/03/07  21:11:22  madden
 * Added in check for blastn on gapped calculations.
 *
 * Revision 1.31  1997/03/06  21:47:27  madden
 * Made FormatBlastParameters non-static.
 *
 * Revision 1.30  1997/03/05  18:16:16  madden
 * SeqIdFree replaced by SeqIdSetFree, fixed memory leak.
 *
 * Revision 1.29  1997/03/05  14:29:46  madden
 * Moved BlastSaveCurrentHsp from blast.c; Added function CheckHspOverlap.
 *
 * Revision 1.28  1997/03/04  21:34:59  madden
 * Added in HspArrayPurge.
 *
 * Revision 1.27  1997/03/04  20:08:19  madden
 * Moved gapped alignment code from blast.c to blastutl.c
 *
 * Revision 1.26  1997/03/03  22:39:45  madden
 * Moved code from blast.c to blastutl.c.
 *
 * Revision 1.25  1997/03/03  21:47:22  madden
 * Moved functions from blast.c to blastutl.c for 16-bit windows.
 *
 * Revision 1.24  1997/03/03  20:58:09  madden
 * Fixed offsets for minus strands.
 *
 * Revision 1.23  1997/03/03  17:30:21  madden
 * Set SeqAlignPtr to NULL in BlastTwoSequences and BlastBioseqEngine, possible UMR.
 *
 * Revision 1.22  1997/03/01  18:25:33  madden
 * reverse flag added to BlastGetGappedAlignmentTraceback functions.
 *
 * Revision 1.21  1997/02/27  22:47:07  madden
 * Replaced tblastx with tblastn in BioseqBlastEngine.
 *
 * Revision 1.20  1997/02/26  23:39:54  madden
 * Added Txdfline stuff.
 *
 * Revision 1.19  1997/02/26  20:37:31  madden
 * Added *error_returns to BioseqBlastEngine.
 *
 * Revision 1.18  1997/02/25  19:17:05  madden
 * Changes to BioseqBlastEngine.
 *
 * Revision 1.17  1997/02/20  23:00:34  madden
 * Checked for NULL return in BlastTwoSequences.
 *
 * Revision 1.16  1997/02/20  18:38:34  madden
 * Set Default db_length to zero in Options.
 *
 * Revision 1.15  1997/02/19  16:25:22  madden
 * Reset gapped_calculation for blastn; returned proper SeqAlign for blastx, tblastn
 * in BioseqBlastEngine.
 *
 * Revision 1.14  1997/02/19  13:45:13  madden
 * replaced zero in call to BlastGetGappedAlignmentTraceback with FALSE.
 *
 * Revision 1.13  1997/02/18  22:09:02  madden
 * Removed unused variable.
 *
 * Revision 1.12  1997/02/18  21:03:00  madden
 * Changes to BioseqBlastEngine for gapped calculations.
 *
 * Revision 1.11  1997/02/18  18:31:34  madden
 * Used SeqIdFindBest in BlastTwoSequences.
 *
 * Revision 1.10  1997/02/18  17:58:52  madden
 * Added BioseqBlastEngine.
 *
 * Revision 1.9  1997/02/14  17:17:59  madden
 * Changes to default options and BlastTwoSequences for nucl.
 * sequences with ambiguites.
 *
 * Revision 1.8  1997/02/13  18:23:56  madden
 * Fixed ID type from BlastTwoSequences.
 *
 * Revision 1.7  1997/02/11  19:30:54  madden
 * Changes to BlastTwoSequences for gapped alignments.
 *
 * Revision 1.6  1997/02/10  20:03:58  madden
 * BlastTwoSequences indexes only the subject.
 *
 * Revision 1.5  1997/02/10  15:24:26  madden
 * Removed unused variable.
 *
 * Revision 1.4  1997/02/07  22:43:03  madden
 * Moved BLAST_WordFinderNew and Destruct from blast.c to blastutl.c, made
 * non-static.
 *
 * Revision 1.3  1997/02/07  22:32:40  madden
 * Changed prototypes for BlastGetSubjectId and GetSeqAlignForResultHitList.
 *
 * Revision 1.2  1997/02/05  13:36:48  madden
 * Removed Unused variable.
 *
 * Revision 1.1  1997/02/04  18:23:58  madden
 * Initial revision
 *
*/

#define NLM_GENERATED_CODE_PROTO
#include <ncbi.h>
#include <blast.h>
#include <blastpri.h>
#include <objcode.h>
#include <objseq.h>
#include <sequtil.h>
#include <tofasta.h>
#include <seqport.h>
#include <readdb.h>
#include <ncbithr.h>
#include <dust.h>
#include <urkpcc.h>
#include <txalign.h>
#include <seg.h>
#include <salpedit.h>
#include <mbalign.h>
#include <mblast.h>
#include <vecscrn.h>
#include <rpsutil.h>
#include <simutil.h>
#include <blfmtutl.h>

#include "blastaltivec.h"


typedef struct _pgp_blast_options {
    BLAST_OptionsBlkPtr options;
    CharPtr blast_database;
    BioseqPtr query_bsp, fake_bsp;
    Int4 number_of_descriptions, number_of_alignments;
    FILE *infp, *outfp;
    AsnIoPtr aip_out;
    Boolean html;
    Boolean believe_query;
    Uint4 align_options, print_options;
  /* PHI-PSI Blast variables */
    Uint1 featureOrder[FEATDEF_ANY];
    Uint1 groupOrder[FEATDEF_ANY];
    Int4 program_flag;
    CharPtr patfile;
    FILE *patfp;
    seedSearchItems *seedSearch;
} PGPBlastOptions, PNTR PGPBlastOptionsPtr;

/* Window size used to scan HSP for highest score region, where gapped
extension starts. */
#define HSP_MAX_WINDOW 11

#define BLASTFILTER_DIR "/usr/ncbi/blast/filter"

static SeqIdPtr
BlastGetFirstGiofSubset(ReadDBFILEPtr rdfp, Int4 ordinal_id, Int2 aliasfilebit)
{
    Boolean	not_done = TRUE;
    SeqIdPtr	bestid = NULL, tmp_seqid, seqid=NULL;
    Uint4	header_index = 0;
    Int4	gi = 0;
    Int4	alias_mask;
    BlastDefLinePtr bdfp;
    
    if (!rdfp->cih && rdfp->formatdb_ver < FORMATDB_VER) {
        /* FORMATDB_VER_TEXT version requires the common index
         * to determine the subset databases */
        ErrPostEx(SEV_ERROR, 0, 0, "Database mask cannot be used without CommonIndex");
        return NULL;
    }
    
    alias_mask = (0x1 << rdfp->aliasfilebit);
    
    bdfp = NULL;
    if(rdfp->formatdb_ver > FORMATDB_VER_TEXT) {
        bdfp = FDReadDeflineAsn(rdfp, ordinal_id);
        if(bdfp == NULL) {
            ErrPostEx(SEV_ERROR, 0, 0, "Failure to read defline ASN for %d", 
                      ordinal_id);
            return NULL;
        }

        bestid = SeqIdFindBest(bdfp->seqid, SEQID_GI);
        if (bestid->choice == SEQID_GI) {
            gi = bestid->data.intvalue;
            ValNodeAddInt(&seqid, SEQID_GI, gi);
        }
        bdfp = BlastDefLineSetFree(bdfp);

        return seqid;
    }
    
    while (not_done) {
        CommonIndexPtr	cigi;

        /* get seqid from database headers file */
        not_done = readdb_get_header (rdfp, ordinal_id, &header_index, &tmp_seqid, NULL);

        if (not_done == FALSE)
            break;

        if (not_done) {
            /* get gi number */
            bestid = SeqIdFindBest(tmp_seqid, SEQID_GI);
            if (bestid->choice != SEQID_GI) {
                tmp_seqid = SeqIdSetFree(tmp_seqid);
                break;
            }
            gi = bestid->data.intvalue;
            
            /* get database commonindex mask */
            cigi = rdfp->cih->ci + gi;
            if (alias_mask & SwapUint4(cigi->dbmask)) {
                ValNodeAddInt(&seqid, SEQID_GI, gi);
                break;
            }
            tmp_seqid = SeqIdSetFree(tmp_seqid);
        }
    }
    tmp_seqid = SeqIdSetFree(tmp_seqid);

    return seqid;
}

#define BLAST_ITER_MAX 30

/*
  Goes through the list of gi's/ordinal id's looking for matches
  to the ordinal ID.  Returns those acceptable gi's as SeqIdPtr's.
*/
SeqIdPtr
BlastGetAllowedGis (BlastSearchBlkPtr search, Int4 ordinal_id, SeqIdPtr PNTR seqid)
{
    BlastGiListPtr blast_gi_list;
    Boolean found=FALSE;
    BlastDoubleInt4Ptr *gi_list_pointer;
    Int4 index, total, first, last, current;
    ValNodePtr gi_list=NULL;
    
    if (seqid)
        *seqid = NULL;
    gi_list = NULL;
    if (search->thr_info->blast_gi_list) {
        blast_gi_list = search->thr_info->blast_gi_list;
        total = blast_gi_list->total;
        found = FALSE;
        gi_list_pointer = blast_gi_list->gi_list_pointer;
        first = 0;
        last = total;
        for (index=0; index<BLAST_ITER_MAX; index++) {
            current = (first+last)/2;
            if (ordinal_id < gi_list_pointer[current]->ordinal_id)
                last = current;
            else if (ordinal_id > gi_list_pointer[current]->ordinal_id)
                first = current;
            else {	/* back up looking for all gi's associated with this oid. */
                while (current > 0 && 
                       ordinal_id == gi_list_pointer[current-1]->ordinal_id)
                    current--;
                found = TRUE;
                break;
            }
        }
        
        if (found) {
            while (current < total) {
                if (ordinal_id == gi_list_pointer[current]->ordinal_id) {
                    ValNodeAddInt(&gi_list, SEQID_GI, blast_gi_list->gi_list_pointer[current]->gi);
                } else {
                    break;
                }
                current++;
            }
        }

        if (seqid && search->rdfp && search->rdfp->aliasfilebit != 0) {
            *seqid = BlastGetFirstGiofSubset(search->rdfp, ordinal_id, search->rdfp->aliasfilebit);
        }
        return (SeqIdPtr) gi_list;
    } else  if (search->rdfp != NULL && search->rdfp->oidlist != NULL) {
        /* if we have at least one mask, then we need print only those gis, which
           are in the database list (reals and masks) */
        
        Boolean	not_done = TRUE;
        SeqIdPtr	bestid = NULL, tmp_seqid = NULL;
        Uint4	header_index = 0;
        Int4	gi = 0;
        Int4	mask;
        Int2	firstpos, curfirstpos;
        ReadDBFILEPtr	rdfp = search->rdfp, tmprdfp;
        BlastDefLinePtr bdfp, bdfp_head;
        
        if (!rdfp->cih && rdfp->formatdb_ver < FORMATDB_VER) {
            /* FORMATDB_VER_TEXT version requires the common index
             * to determine the subset databases */
           /*ErrPostEx(SEV_ERROR, 0, 0, "Database mask cannot be used without CommonIndex");*/
            return NULL;
        }

        /* kludge: only protein databases are non-redundant */
        if (readdb_is_prot(search->rdfp) == FALSE)
            return NULL;
        
        bdfp = NULL; bdfp_head = NULL;
        if(rdfp->formatdb_ver > FORMATDB_VER_TEXT) {
            /* just chain the seqid's returned, as they are filtered in 
             * FDReadDeflineAsn according to the membership_bit in the 
             * rdfp */
            bdfp = FDReadDeflineAsn(rdfp, ordinal_id);
            if(bdfp == NULL) {
                ErrPostEx(SEV_ERROR, 0, 0, "Failure to read defline ASN for %d", ordinal_id);
                return NULL;
            }
            for (bdfp_head = bdfp; bdfp; bdfp = bdfp->next) {
                bestid = SeqIdFindBest(bdfp->seqid, SEQID_GI);
                if (bestid->choice == SEQID_GI) {
                    gi = bestid->data.intvalue;
                    ValNodeAddInt(&gi_list, SEQID_GI, gi);
                }
            }

            BlastDefLineSetFree(bdfp_head);
            
        } else {
        
            while (not_done) {
                CommonIndexPtr	cigi;

                /* get seqid from database headers file */
                not_done = readdb_get_header (search->rdfp, ordinal_id, &header_index, &tmp_seqid, NULL);

                if (not_done == FALSE)
                    break;

                if (not_done) {
                    /* get gi number */
                    bestid = SeqIdFindBest(tmp_seqid, SEQID_GI);
                    if (bestid->choice != SEQID_GI) {
                        tmp_seqid = SeqIdSetFree(tmp_seqid);
                        break;
                    }
                    gi = bestid->data.intvalue;
                    
                    /* get database commonindex mask */
                    cigi = search->rdfp->cih->ci + gi;
                    mask = SwapUint4(cigi->dbmask);

                    firstpos = 0;
                    while (((curfirstpos = bit_engine_firstbit(mask)) != -1)) {
                        CharPtr		dbname;

                        firstpos += curfirstpos;

                        dbname = DBName(search->rdfp->cih->num_of_DBs,
                                        search->rdfp->cih->dbids, firstpos);

                        /* search in rdfp list this database */
                        tmprdfp = search->rdfp;
                        while (tmprdfp) {
                            if (tmprdfp->aliasfilename) {
                                /* use mask name, if exists */
                                if (!StrCmp(dbname, tmprdfp->aliasfilename)) {
                                    ValNodeAddInt(&gi_list, SEQID_GI, gi);
                                }
                            } else {
                                /* use real file name */
                                if (!StrCmp(dbname, tmprdfp->filename)) {
                                    ValNodeAddInt(&gi_list, SEQID_GI, gi);
                                }
                            }
                            tmprdfp = tmprdfp->next;
                        }
                        mask >>= (curfirstpos + 1);
                        firstpos++;
                    }
                }
                
                if (tmp_seqid) {
                    tmp_seqid = SeqIdSetFree(tmp_seqid);
                }
            }
        }
        if (seqid)
            *seqid = BlastGetFirstGiofSubset(search->rdfp, ordinal_id, search->rdfp->aliasfilebit);

        return (SeqIdPtr) gi_list;
    }
        
    return NULL;
}

/* 
	SOME FUNCTIONS TO PRODUCE A SeqAlign from the BLAST results.
*/

/*****************************************************************************

	Finds the best SeqId for the SeqAlign.  Looks for the GI, then takes
	anything if that's not found and makes up a local ID if no ID is
	found at all.
*****************************************************************************/

SeqIdPtr
GetTheSeqAlignID(SeqIdPtr seq_id)
{
	SeqIdPtr new_id, ret_id;
	ObjectIdPtr obidp;
	
	ret_id = NULL;
	if (seq_id)
	{
		/* Get the gi from the chain, if it's there. */
		new_id = SeqIdFindBest(seq_id, SEQID_GI);
		if (new_id)
		{
			ret_id = SeqIdDup(new_id);
		}
		else
		{	/* No Gi was found, use any ID. */
			ret_id = SeqIdDup(seq_id);
		}
	}

	if (ret_id == NULL)
	{	/* make up an ID. */
		obidp = ObjectIdNew();
		obidp->str = StringSave("lcl|unknown");
		ValNodeAddPointer(&ret_id, SEQID_LOCAL, obidp);
	}

	return ret_id;
}
static SeqAlignPtr 
FillInSegsInfo(SeqAlignPtr sap_head, StdSegPtr ssp_head, DenseDiagPtr ddp_head)

{
	SeqAlignPtr sap;

	if (ddp_head || ssp_head)
	{
		if (sap_head)
		{
			sap = sap_head;
			while (sap->next)
				sap = sap->next;
			sap->next = SeqAlignNew();
			sap = sap->next;
		}
		else
		{
			sap_head = sap = SeqAlignNew();
		}

		if (ddp_head)
		{
			sap->type = 2;
			sap->segs = ddp_head;
			sap->segtype = 1;
		}
		else if (ssp_head)
		{
			sap->type = 2;
			sap->segs = ssp_head;
			sap->segtype = 3;
		}
	}
	return sap_head;
}


/*************************************************************************
*
*	This function fills in the DenseDiag Information from the variable
*	hsp.  On the first call to this function *old should be
*	NULL, after that pass in the head of the DenseDiagPtr chain.
*	The newest DenseDiagPtr is returned.
*
************************************************************************/

static DenseDiagPtr
FillInDenseDiagInfo(DenseDiagPtr PNTR old, BLASTResultHspPtr hsp, Boolean reverse, Int4 query_length, Int4 subject_length, SeqIdPtr gi_list)

{
	DenseDiagPtr		ddp, new;

	new = DenseDiagNew();
	
	new->dim = 2;	/* Only 2 is supported in spec. */
	new->len = hsp->query_length;
	new->starts = (Int4Ptr) MemNew(2 * sizeof(Int4));
	new->strands = (Uint1Ptr) MemNew(2 * sizeof(Uint1));
	if (reverse)
	{
		if (hsp->subject_frame >= 0)
		{
			new->strands[0] = Seq_strand_plus;
			new->starts[0] = hsp->subject_offset;
		}
		else
		{
			new->strands[0] = Seq_strand_minus;
			new->starts[0] = subject_length - hsp->subject_offset - hsp->subject_length;
		}
		if (hsp->query_frame >= 0)
		{
			new->strands[1] = Seq_strand_plus;
			new->starts[1] = hsp->query_offset;
		}
		else
		{
			new->strands[1] = Seq_strand_minus;
			new->starts[1] = query_length - hsp->query_offset - hsp->query_length;
		}
	}
	else
	{
		if (hsp->query_frame >= 0)
		{
			new->strands[0] = Seq_strand_plus;
			new->starts[0] = hsp->query_offset;
		}
		else
		{
			new->strands[0] = Seq_strand_minus;
			new->starts[0] = query_length - hsp->query_offset - hsp->query_length;
		}
		if (hsp->subject_frame >= 0)
		{
			new->strands[1] = Seq_strand_plus;
			new->starts[1] = hsp->subject_offset;
		}
		else
		{
			new->strands[1] = Seq_strand_minus;
			new->starts[1] = subject_length - hsp->subject_offset - hsp->subject_length;
		}
	}
	new->scores = GetScoreSetFromBlastResultHsp(hsp, gi_list);

/* Go to the end of the chain, and then attach "new" */
	if (*old)
	{
		ddp = *old;
		while (ddp->next)
			ddp = ddp->next;
		ddp->next = new;
	}
	else
	{
		*old = new;
	}

	new->next = NULL;

	return new;
}

/*************************************************************************
*
*	This function fills in the StdSeg Information from the variable
*	hsp.  On the first call to this function *old should be
*	NULL, after that pass in the head of the DenseDiagPtr chain.
*	The newest StdSegPtr is returned.
*
************************************************************************/
static StdSegPtr
FillInStdSegInfo(BlastSearchBlkPtr search, Int4 subject_id, Int4 length, StdSegPtr PNTR old, BLASTResultHspPtr hsp, SeqIdPtr sip, Boolean reverse, SeqIdPtr gi_list)

{
	Int4			subject_length;
	StdSegPtr		ssp, new;
	SeqIdPtr		query_sip, subject_sip;
	SeqIntPtr		seq_int1, seq_int2;
	SeqLocPtr		slp=NULL;

	new = StdSegNew();
/* Duplicate the id and split it up into query and subject parts */
	query_sip = SeqIdDup(sip);
	subject_sip = SeqIdDup(sip->next);
	
	new->dim = 2;	/* Only 2 is supported in spec. */
	seq_int1 = SeqIntNew();
	if (hsp->query_frame == 0)
	{
		seq_int1->from = hsp->query_offset;
		seq_int1->to = hsp->query_offset + hsp->query_length - 1;
		seq_int1->strand = Seq_strand_unknown;
	}
	else if (hsp->query_frame < 0)
	{
		seq_int1->to = search->context[hsp->context].query->original_length - CODON_LENGTH*hsp->query_offset + hsp->query_frame;
		seq_int1->from = search->context[hsp->context].query->original_length - CODON_LENGTH*(hsp->query_offset+hsp->query_length) + hsp->query_frame + 1;
		seq_int1->strand = Seq_strand_minus;
	}
	else if (hsp->query_frame > 0)
	{
		seq_int1->from = CODON_LENGTH*(hsp->query_offset) + hsp->query_frame - 1;
		seq_int1->to = CODON_LENGTH*(hsp->query_offset+hsp->query_length) + hsp->query_frame - 2;
		seq_int1->strand = Seq_strand_plus;
	}
	seq_int1->id = query_sip;
	seq_int2 = SeqIntNew();
	if (hsp->subject_frame == 0)
	{
		seq_int2->from = hsp->subject_offset;
		seq_int2->to = hsp->subject_offset + hsp->subject_length - 1;
		seq_int2->strand = Seq_strand_unknown;
	} 
	else if (hsp->subject_frame < 0)
	{
	    	if (search->rdfp)
			subject_length = readdb_get_sequence_length(search->rdfp, subject_id);
                else
                   subject_length = length;

		seq_int2->from = subject_length - CODON_LENGTH*(hsp->subject_offset + hsp->subject_length) + hsp->subject_frame + 1;
		seq_int2->to = subject_length - CODON_LENGTH*(hsp->subject_offset) + hsp->subject_frame;
		seq_int2->strand = Seq_strand_minus;
	}
	else if (hsp->subject_frame > 0)
	{
		seq_int2->from = CODON_LENGTH*(hsp->subject_offset) + hsp->subject_frame - 1;
		seq_int2->to = CODON_LENGTH*(hsp->subject_offset + hsp->subject_length) + hsp->subject_frame - 2;
		seq_int2->strand = Seq_strand_plus;
	}
	seq_int2->id = subject_sip;

	if (reverse)
	{
		ValNodeAddPointer(&slp, SEQLOC_INT, seq_int2); 
		ValNodeAddPointer(&slp, SEQLOC_INT, seq_int1); 
	}
	else
	{
		ValNodeAddPointer(&slp, SEQLOC_INT, seq_int1); 
		ValNodeAddPointer(&slp, SEQLOC_INT, seq_int2); 
	}
	new->loc = slp;

        search->subject->sequence = MemFree(search->subject->sequence);
	new->scores = GetScoreSetFromBlastResultHsp(hsp, gi_list);

/* Go to the end of the chain, and then attach "new" */
	if (*old)
	{
		ssp = *old;
		while (ssp->next)
			ssp = ssp->next;
		ssp->next = new;
	}
	else
	{
		*old = new;
	}

	new->next = NULL;

	return new;
}

/************************************************************************
*
*	This function assembles all the components of the Seq-align from
*	a "sparse" BLAST HitList.  "sparse" means that the hitlist 
*	may contain no sequence and not even a descriptor.  It is only 
*	required to contain the sequence_number that readdb refers to
*	and scoring/alignment information.
*
*	If dbname is non-NULL, then only a general ("gnl") ID is 
*	issued, with the ordinal number of the subject sequence in
*	the ObjectIdPtr.
*
*	Boolean reverse: reverse the query and db order in SeqAlign.
*
************************************************************************/
SeqAlignPtr LIBCALL
GetSeqAlignForResultHitList(BlastSearchBlkPtr search, Boolean getdensediag, Boolean ordinal_number, Boolean discontinuous, Boolean reverse, Boolean get_redundant_seqs)

{
	BLASTResultHspPtr	hsp;
	BLASTResultHitlistPtr	results;
	BLASTResultsStructPtr	result_struct;
	DenseDiagPtr		ddp_head=NULL, ddp;
	SeqIdPtr		gi_list=NULL, sip, sip_subject,
	   sip_subject_start, query_id, new_sip;
	StdSegPtr		ssp_head=NULL, ssp;
	SeqAlignPtr		last, seqalign_head, seqalign, sap_head;
	Int4 			hsp_cnt, index, index2, hspset_cnt_old, i;
	Int4			hitlist_count;
	Int4			subject_length;
	ValNodePtr		vnp, vnp_start;

	ddp_head = NULL;
	ssp_head = NULL;
	sap_head = NULL;
	seqalign_head = NULL;

        /* discontinuous = FALSE; */
	result_struct = search->result_struct;
	hitlist_count = result_struct->hitlist_count;

	last = NULL;
	sip = NULL;
	sip_subject_start = NULL;
	for (index=0; index<hitlist_count; index++)
	{
	    results = result_struct->results[index];
	    sip_subject_start = NULL;
	    if (get_redundant_seqs)
	    {
		vnp = NULL;
	    	sip = BlastGetSubjectId(search, index, ordinal_number, &vnp);
		vnp_start = vnp;
		while (vnp)
		{
			sip = GetTheSeqAlignID(vnp->data.ptrvalue);
			SeqIdFree(vnp->data.ptrvalue);
			if (sip_subject_start == NULL)
			{
				sip_subject_start = sip;
			}
			else
			{
				sip_subject = sip_subject_start;
				while (sip_subject->next)
					sip_subject = sip_subject->next;
				sip_subject->next = sip;
			}
			vnp = vnp->next;
		}
		vnp_start = vnp = ValNodeFree(vnp_start);
	    }
	    else
	    {
	    	sip = BlastGetSubjectId(search, index, ordinal_number, NULL);
	    	sip_subject_start = sip_subject = GetTheSeqAlignID(sip);
	    	sip = SeqIdSetFree(sip);
	    }

	    results = result_struct->results[index];
	    if (search->rdfp)
		subject_length = readdb_get_sequence_length(search->rdfp, results->subject_id);
	    else if (results->subject_info)
			subject_length = results->subject_info->length;
	    else
			subject_length = 0;

	gi_list = BlastGetAllowedGis(search, results->subject_id, &new_sip);
	/* right now sip_subject should only contain one ID.  At some
	point it will contain multiple ID's for identical sequences. */
            if (new_sip != NULL)
               sip_subject = new_sip;
            else
               sip_subject = sip_subject_start;
	    while (sip_subject)
	    {
	    	seqalign = SeqAlignNew();
	    	seqalign->type = 2;		/* alignment is diags */
	    	if (last == NULL)	/* First sequence. */
			seqalign_head = seqalign;
	    	else
			last->next = seqalign;

	    	last = seqalign;
		
		hspset_cnt_old = -1;
		hsp_cnt = results->hspcnt;
		for (index2=0; index2<hsp_cnt; index2++)
		{
			hsp = &(results->hsp_array[index2]);
			if (discontinuous && hspset_cnt_old != hsp->hspset_cnt)
			{
			    hspset_cnt_old = hsp->hspset_cnt;
			    if (index2 != 0)
			    { /* nothing to save on first pass. */
				if (getdensediag)
				{
					sap_head = FillInSegsInfo(sap_head, NULL, ddp_head);
					ddp_head = NULL;
				}
				else
				{
					sap_head = FillInSegsInfo(sap_head, ssp_head, NULL);
					ssp_head = NULL;
				}
			    }
			}

			query_id = search->query_id;
			if (search->prog_number==blast_type_blastn) {
			   for (i=0; i<hsp->context/2; i++)
			      query_id = query_id->next;
			}
			if (reverse)
			{
				sip = SeqIdDup(sip_subject);
	    			sip->next = GetTheSeqAlignID(query_id);
			}
			else
			{
	    			sip = GetTheSeqAlignID(query_id);
				sip->next = SeqIdDup(sip_subject);
			}

			if (getdensediag)
			{
		    		ddp = FillInDenseDiagInfo(&ddp_head, hsp, reverse, search->context[hsp->context].query->length, subject_length, gi_list);
		    		ddp->id = sip;
			}
			else
			{
                            Int4 length = 0;

                            if (results->subject_info)
                                length = results->subject_info->length;

                            ssp = FillInStdSegInfo(search, results->subject_id, length, &ssp_head, hsp, sip, reverse, gi_list);
                            ssp->ids = sip;
			}
			sip = NULL; /* This SeqIdPtr is now on the SeqAlign. */
		}

		if (discontinuous)
		{
			if (getdensediag)
			{
				sap_head = FillInSegsInfo(sap_head, NULL, ddp_head);
				ddp_head = NULL;
			}
			else
			{
				sap_head = FillInSegsInfo(sap_head, ssp_head, NULL);
				ssp_head = NULL;
			}
	        	seqalign->segs = sap_head;
	        	seqalign->segtype = 5;	/* Discontinuous */
		}
		else
		{
			if (getdensediag)
			{
				seqalign->segs = ddp_head;
                                seqalign->segtype = 1;  /* DenseDiag */
				ddp_head = NULL;
			}
			else
			{
                                seqalign->segs = ssp_head;
                                seqalign->segtype = 3;  /* StdSeg */
				ssp_head = NULL;
			}
		}

		sap_head = NULL;

		sip_subject = sip_subject->next;
	     }
	     if (sip_subject_start)
			sip_subject_start = SeqIdFree(sip_subject_start);
	     if (new_sip)
			new_sip = SeqIdFree(new_sip);
	     gi_list = SeqIdSetFree(gi_list);
	}

	return seqalign_head;
}

/*
	"Core" function to compare two sequences, for use by 
	BlastTwoSequences and BlastSequencesOnTheFly.

	The subject_bsp is redundant with the subject_seq_start and
	subject_length (or visa-versa), but the subject must be
	extracted from the subject_bsp for BlastTwoSequences anyway, while
	the title and ID are needed from subject_bsp.
*/
static Int2
BlastTwoSequencesCoreEx (BlastSearchBlkPtr search, BioseqPtr subject_bsp, Uint1Ptr subject_seq, Int4 subject_length)
{
	Int2 status=0;

	search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
    if (!search->handle_results)
       search->subject_info = BLASTSubjectInfoNew(SeqIdDup(SeqIdFindBest(subject_bsp->id, SEQID_GI)), StringSave(BioseqGetTitle(subject_bsp)), subject_length);
    else
       search->subject_info = BLASTSubjectInfoNew(SeqIdSetDup(subject_bsp->id), StringSave(BioseqGetTitle(subject_bsp)), subject_length);

    /*CC: is search->sbp->posMatrix, we're comparing a pssm with a subject
     * sequence, thus we need to do some set up */
    if (search->sbp->posMatrix && search->prog_number == blast_type_blastp) {
        Int4 hitlist_max;
        BLAST_ScoreBlkPtr sbp = search->sbp;
        BLAST_ParameterBlkPtr pbp = search->pbp;

        search->positionBased = TRUE;
        sbp->kbp = sbp->kbp_psi;
        sbp->kbp_gap = sbp->kbp_gap_psi;
        hitlist_max = search->result_struct->hitlist_max;
        search->result_struct =
            BLASTResultsStructDelete(search->result_struct);
		search->result_struct = BLASTResultsStructNew(hitlist_max, 
            pbp->max_pieces, pbp->hsp_range_max);

        if (search->allocated & BLAST_SEARCH_ALLOC_WFP_FIRST) {
            search->wfp_first = BLAST_WordFinderDestruct(search->wfp_first);
		    search->wfp_first = BLAST_WordFinderNew(sbp->alphabet_size,
                    search->all_words->wordsize, 1, FALSE);
		}

		if (search->allocated & BLAST_SEARCH_ALLOC_WFP_SECOND) {
		    search->wfp_second = BLAST_WordFinderDestruct(search->wfp_second);
		    search->wfp_second = BLAST_WordFinderNew(sbp->alphabet_size,
                    search->all_words->wordsize, 1, FALSE);
		}

		/* threshold_first is defunct ! */
        search->wfp = search->wfp_first;
		if (search->whole_query == TRUE)
            BlastNewFindWords(search, 0, search->context[0].query->length, 
                    pbp->threshold_second, (Uint1) 0);
		else
           	BlastNewFindWords(search, search->required_start, 
                    search->required_end, pbp->threshold_second, (Uint1) 0);
        lookup_position_aux_destruct(search->wfp->lookup);
        search->wfp_second = search->wfp_first;
    }
	status = BLASTPerformSearch(search, subject_length, subject_seq);
        
        if (status) {
		BlastConstructErrorMessage("BlastTwoSequencesCoreEx", "non-zero status", 2, &(search->error_return));
		return status;
        }

        if (search->prog_number == blast_type_tblastn &&
            search->pbp->longest_intron > 0) {
           Uint1 rem;
           Uint1Ptr seq_4na, seq_2na, subject;
           Int4 i;
           /* Need to convert from ncbi2na to ncbi4na encoding */
           subject = (Uint1Ptr) MemNew(subject_length + 1);
           seq_4na = subject;
           seq_2na = subject_seq;
           rem = 3;
           for (i=0; i<subject_length; i++) {
              *seq_4na = (Uint1) (1 << READDB_UNPACK_BASE_N(*seq_2na, rem));
              seq_4na++;
              if (rem>0) rem--;
              else {
                 rem = 3;
                 seq_2na++;
              }
           }
           BlastSequenceAddSequence(search->subject, NULL, subject-1, subject_length, subject_length, 0);
           status = BlastLinkHsps(search);
        }

	if (StringCmp(search->prog_name, "blastn") == 0 || search->pbp->gapped_calculation == FALSE)
	{
            if (search->pbp->do_sum_stats == TRUE && 
		!search->pbp->mb_params)
                status = BlastLinkHsps(search);
            else
                status = BlastGetNonSumStatsEvalue(search);
	}
        if (search->pbp->mb_params) {
           search->subject->sequence = subject_seq;
           MegaBlastReevaluateWithAmbiguities(search);
        }
        status = BlastReapHitlistByEvalue(search);

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

        search->subject->sequence = NULL;
        search->subject->sequence_start = NULL;
	if (search->prog_number==blast_type_blastn) {
	   /* Unconcatenate the strands by adjusting the query offsets in
	      all hsps */
	   search->context[search->first_context].query->length = 
	      search->query_context_offsets[search->first_context+1] - 1;
	}

	return status;
}

static BLAST_ScorePtr *RPS2SeqImpalaStatCorrections
        (BlastSearchBlkPtr search, Uint1Ptr subject_seq, Int4 subject_length)
{
    BLAST_ScorePtr *retval = NULL;
    Nlm_FloatHi *scoreArray; /*array of score probabilities*/
    Nlm_FloatHi *resProb; /*array of probabilities for each residue*/
    BLAST_ScoreFreqPtr this_sfp, return_sfp; /*score frequency pointers to compute lambda*/
    BLAST_ScorePtr *posMatrix; /* position-specific matrix. */
    Nlm_FloatHi initialUngappedLambda, scaledInitialUngappedLambda, 
                  correctUngappedLambda, scalingFactor, lambdaRatio;
    Nlm_FloatHi temp1; /*intermediate variable for adjusting matrix*/
    Int4 temp2; /*intermediate variable for adjusting matrix*/
    Int4 seqlength; /* length of posMatrix (or target sequence). */
    Int4 i, j; /* loop indices */

    if (search == NULL)
 	   return retval;

    posMatrix = search->sbp->posMatrix;
    scalingFactor = search->pbp->scalingFactor;
 
    resProb = (Nlm_FloatHi *) MemNew (PRO_ALPHABET_SIZE * sizeof(Nlm_FloatHi));
    scoreArray = (Nlm_FloatHi *) MemNew(scoreRange * sizeof(Nlm_FloatHi));
    return_sfp = (BLAST_ScoreFreqPtr) MemNew(1 * sizeof(BLAST_ScoreFreq));
 
    seqlength = search->sbp->query_length;
 
    IMPALAfillResidueProbability(subject_seq, subject_length, resProb);
    this_sfp = IMPALAfillSfp(posMatrix, seqlength, resProb, scoreArray, 
                     return_sfp, scoreRange);
    initialUngappedLambda = IMPALAfindUngappedLambda(search->sbp->name);
    scaledInitialUngappedLambda = initialUngappedLambda/scalingFactor;
    correctUngappedLambda = impalaKarlinLambdaNR(this_sfp, scaledInitialUngappedLambda);
    if(correctUngappedLambda == -1.0) {
        ErrPostEx(SEV_ERROR, 0, 0, 
                  "RPS2SeqImpalaStatCorrections: Could not calculate ungapped "
                  "lambda for PSSM");
        MemFree(resProb);
        MemFree(scoreArray);
        MemFree(return_sfp);
        return retval;
    }
 
    lambdaRatio = correctUngappedLambda/scaledInitialUngappedLambda;
 
    retval = (BLAST_Score **) MemNew((seqlength+1) * sizeof(BLAST_Score *));
    for (i = 0; i < seqlength+1; i++)
        retval[i] = (BLAST_Score *)MemNew(PRO_ALPHABET_SIZE * 
                sizeof(BLAST_Score));
 
    for (i = 0; i < seqlength+1; i++) {
        for (j = 0; j < PRO_ALPHABET_SIZE; j++) {
            if ((posMatrix[i][j] == BLAST_SCORE_MIN) || (Xchar == j))
                retval[i][j] = posMatrix[i][j];
            else {
                temp1 = ((Nlm_FloatHi) (posMatrix[i][j]));
                temp1 = temp1 * (lambdaRatio);
                temp2 = Nlm_Nint(temp1);
                retval[i][j] = temp2;
            }
        }
    }

    resProb = MemFree(resProb);
    scoreArray = MemFree(scoreArray);
    return_sfp = MemFree(return_sfp);

    return retval;
}

static SeqAlignPtr 
BlastTwoSequencesCore (BlastSearchBlkPtr search, SeqLocPtr slp, Uint1Ptr subject_seq, Int4 subject_length, Boolean reverse)

{
	BLASTResultsStructPtr result_struct;
	BioseqPtr subject_bsp;
	Int2 status;
	Int4 index, hitlist_count, rev_subject_length=0;
	SeqAlignPtr seqalign=NULL;
	SeqPortPtr spp;
	Uint1 residue;
	Uint1Ptr sequence, sequence_start, rev_subject=NULL;
	SeqIdPtr sip;
    BLAST_ScorePtr *scaledMatrix = NULL, *copyMatrix = NULL;

	if (search == NULL || search->query_invalid)
		return NULL;

	sip = SeqLocId(slp);
	subject_bsp = BioseqLockById(sip);

    /* Save subject sequence location for tabulated output */
    if (search->handle_results && SeqLocLen(slp) < subject_bsp->length)
       search->query_slp->next = slp;

	status = BlastTwoSequencesCoreEx(search, subject_bsp, subject_seq,
					 subject_length);

	if (status == 0) {
        /*CC: if we're emulating rpsblast, do the impala style matrix
         * rescaling */
        if (search->positionBased && search->pbp->scalingFactor != 0.0) {
            scaledMatrix = RPS2SeqImpalaStatCorrections(search, subject_seq, 
                    subject_length);
            if ( !scaledMatrix ) {
                BioseqUnlock(subject_bsp);
                return NULL;
            }
            copyMatrix = search->sbp->posMatrix;
            search->sbp->posMatrix = scaledMatrix;
            
            if (search->sbp->karlinK != 0.0)
                search->sbp->kbp_gap[0]->K =
                    PRO_K_MULTIPLIER*search->sbp->karlinK;
            search->sbp->kbp_gap[0]->logK = log(search->sbp->kbp_gap[0]->K);
            search->sbp->kbp_gap[0]->Lambda /= search->pbp->scalingFactor;
        }
	  if (search->pbp->mb_params && !search->pbp->mb_params->no_traceback
              && !search->pbp->mb_params->use_dyn_prog) {
             seqalign = MegaBlastGapInfoToSeqAlign(search, 0, 0);
	  } else if (StringCmp(search->prog_name, "blastn") == 0 &&
		   search->pbp->gapped_calculation == TRUE) {
             result_struct = search->result_struct;
             hitlist_count = result_struct->hitlist_count;
             if (hitlist_count > 0)
	     {
                spp = SeqPortNewByLoc(slp, Seq_code_ncbi4na);
                if (subject_bsp->repr == Seq_repr_delta) 
                   SeqPortSet_do_virtual(spp, TRUE);
                
                /* make one longer to "protect" ALIGN. */
                sequence_start = MemNew((2+SeqLocLen(slp))*sizeof(Uint1));
                sequence_start[0] = ncbi4na_to_blastna[0];
                sequence = sequence_start+1;
                index=0;
                while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF)
                {
                   if (IS_residue(residue))
		   {
                      sequence[index] = ncbi4na_to_blastna[residue];
                      index++;
                   }
                }
                /* Gap character in last space. */
                sequence[index] = ncbi4na_to_blastna[0];
                
                if (!search->pbp->mb_params) {
                   /* Traditional Blastn */
                   seqalign = SumBlastGetGappedAlignmentTraceback(
                                 search, 0, reverse, FALSE, sequence, 
                                 SeqLocLen(slp));
                } else if (!search->pbp->mb_params->no_traceback) {
                   /* Mega BLAST with non-greedy extension */
                   SumBlastGetGappedAlignmentEx(search, 0, FALSE, FALSE, 
                      sequence, SeqLocLen(slp), TRUE, &seqalign, NULL, 0);
                }

                sequence_start = MemFree(sequence_start);
                spp = SeqPortFree(spp);
             }
	  }
	  else if (search->pbp->gapped_calculation == TRUE)
	  {
        result_struct = search->result_struct;
        hitlist_count = result_struct->hitlist_count;
		if (hitlist_count > 0) {

                   if (!StringCmp(search->prog_name, "tblastn")
                       || !StringCmp(search->prog_name, "psitblastn")) {
                      Uint1Ptr subject = NULL;
                      SeqPortPtr rev_spp;
                      if (slp->choice == SEQLOC_WHOLE) {
                         spp = SeqPortNew(subject_bsp, 0, -1, Seq_strand_plus, 
                                          Seq_code_ncbi4na);
                         rev_spp = SeqPortNew(subject_bsp, 0, -1, Seq_strand_minus,
                                              Seq_code_ncbi4na);
                      } else {
                         spp = SeqPortNew(subject_bsp, SeqLocStart(slp), 
                                          SeqLocStop(slp), Seq_strand_plus, 
                                          Seq_code_ncbi4na);
                         rev_spp = SeqPortNew(subject_bsp, SeqLocStart(slp), 
                                              SeqLocStop(slp), Seq_strand_minus,
                                              Seq_code_ncbi4na);
                      }
		      /* make one longer to "protect" ALIGN. */
		      subject = (Uint1Ptr) MemNew((1+subject_length)*sizeof(Uint1));
                      rev_subject = (Uint1Ptr) MemNew((1+subject_length)*sizeof(Uint1));
		      for (index=0; index<subject_length; index++) {
                         subject[index] = SeqPortGetResidue(spp);
			 rev_subject[index] = SeqPortGetResidue(rev_spp);
                      }
		      /* Gap character in last space. */
                      subject[subject_length] = NULLB;
		      rev_subject[subject_length] = NULLB;
		      rev_subject_length = subject_length;
		      spp = SeqPortFree(spp);
		      rev_spp = SeqPortFree(rev_spp);
		   		      
		   
                      seqalign = BlastGetGapAlgnTbck(search, 0, reverse,
                          FALSE, subject, subject_length, 
                          rev_subject, rev_subject_length);

                      if (search->pbp->longest_intron <= 0)
                         MemFree(subject);
                      MemFree(rev_subject);
                   } else {
                      seqalign = BlastGetGapAlgnTbck(search, 0, reverse,
                          FALSE, subject_seq, subject_length, 
                          rev_subject, rev_subject_length);
                      result_struct->results[0]->seqalign = seqalign;
                   }
                }
	  }
	  else /* Ungapped case, any program */
	  {
             if (search->prog_number == blast_type_blastn || 
                 search->prog_number == blast_type_blastp)
                seqalign = GetSeqAlignForResultHitList(search, TRUE, FALSE,
                              search->pbp->discontinuous, reverse, FALSE);
             else
                seqalign = GetSeqAlignForResultHitList(search, FALSE, FALSE,
                              search->pbp->discontinuous, reverse, FALSE); 
	  }
      /*CC: Revert changes done for psi-blast2sequences */
      if (search->positionBased && search->pbp->scalingFactor != 0.0) {
          if (scaledMatrix) {
              for (index = 0; index < search->sbp->query_length + 1; index++)
                  MemFree(scaledMatrix[index]);
              MemFree(scaledMatrix);
              search->sbp->posMatrix = copyMatrix;
          }
          if (search->sbp->karlinK != 0.0)
              search->sbp->kbp_gap[0]->K = search->sbp->karlinK;
          search->sbp->kbp_gap[0]->logK = log(search->sbp->kbp_gap[0]->K);
      }
        }
	BioseqUnlock(subject_bsp);

	return seqalign;
}

BlastSearchBlkPtr LIBCALL
BlastQuerySequenceSetUp(BioseqPtr bsp, CharPtr progname, 
			    BLAST_OptionsBlkPtr options)
{
   BlastSearchBlkPtr search;
   SeqLocPtr slp=NULL;

   if (bsp == NULL)
      return NULL;

   ValNodeAddPointer(&slp, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBest(bsp->id, SEQID_GI)));
   if (progname == NULL && options == NULL)
      return NULL;
   
   if (progname == NULL)
      progname = options->program_name;

   if (!StringCmp(progname, "blastp") || 
       !StringCmp(progname, "blastx")) {
      if (options->gapped_calculation == TRUE) { 
	 options->two_pass_method  = FALSE;
	 options->multiple_hits_only  = TRUE;
      }
   }
	
   search = BLASTSetUpSearchByLoc(slp, progname, bsp->length, 0, NULL, options, NULL);
   
   search->allocated += BLAST_SEARCH_ALLOC_QUERY_SLP;

   if (search == NULL)
      return NULL;
	
   return search;
}

/*
	Runs blast between two sequences
*/
SeqAlignPtr LIBCALL
BlastTwoSequencesByLocEx(SeqLocPtr slp1, SeqLocPtr slp2, CharPtr progname, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns)
{
   return BlastTwoSequencesByLocWithCallback(slp1, slp2, progname, options,
           other_returns, error_returns, NULL, NULL);
}

/************************************************************************/
/*        PSIBLAST2Sequences API                                        */
/************************************************************************/

static BLAST_ScorePtr *B2SAllocateScoreMatrix(Int4 rows, Int4 cols)
{
    BLAST_ScorePtr *matrix = NULL;
    Int4 i;

    if (!(matrix = (BLAST_ScorePtr *) MemNew(rows*sizeof(BLAST_ScorePtr)))) {
        return NULL;
    }

    for (i = 0; i < rows; i++) {
        matrix[i] = (BLAST_ScorePtr) MemNew(cols*sizeof(BLAST_Score));
        if (matrix[i] == NULL) {
            while (--i >= 0)
                MemFree(matrix[i]);
            MemFree(matrix);
            return NULL;
        }
    }
    return matrix;
}

/* Convert a set of residue frequencies into a scaled PSSM (using
 * scalingFactor). */
static BLAST_ScorePtr *B2SCalculateScaledPSSM(BlastSearchBlkPtr search,
        Nlm_FloatHiPtr *posFreqs, compactSearchItems *compactSearch,
        Nlm_FloatHiPtr karlinK)
{
    BLAST_ScorePtr *retval = NULL;
    posSearchItems *posSearch = NULL;
    Int4 qlen, alphabet_sz, rv;
    Nlm_FloatHi scalingFactor = search->pbp->scalingFactor;
    BLAST_ScoreBlkPtr sbp = NULL;
    ValNodePtr error_return;
    Int4 i, gap_open, gap_extend;

    if (!search || !compactSearch || !posFreqs)
        return NULL;

    if (!(posSearch = (posSearchItems *)MemNew(sizeof(posSearchItems)))) {
        ErrPostEx(SEV_ERROR, 0, 0, "B2SCalculateScaledPSSM: Out of memory");
        return NULL;
    }

    qlen = compactSearch->qlength;
    alphabet_sz = compactSearch->alphabetSize;
    gap_open = search->pbp->gap_open / scalingFactor;
    gap_extend = search->pbp->gap_extend / scalingFactor;

    if (!(sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa, 1))) {
        ErrPostEx(SEV_ERROR, 0, 0, "B2SCalculateScaledPSSM: Out of memory");
        MemFree(posSearch);
        return NULL;
    }
    sbp->read_in_matrix = TRUE;
    sbp->protein_alphabet = TRUE;
    sbp->posMatrix = NULL;
    sbp->number_of_contexts = 1;
    BlastScoreBlkMatFill(sbp, search->sbp->name);
    compactSearch->matrix = sbp->matrix;
    compactSearch->gapped_calculation = TRUE;
    compactSearch->pseudoCountConst = search->pbp->pseudoCountConst;
    compactSearch->ethresh = 0.001;
    BlastScoreBlkFill(sbp, (CharPtr) compactSearch->query, qlen, 0);
    
    sbp->kbp_gap_std[0] = BlastKarlinBlkCreate();
    rv = BlastKarlinBlkGappedCalc(sbp->kbp_gap_std[0], gap_open, gap_extend, 
            sbp->name, &error_return);
    if (rv == 1) {
        BlastErrorPrint(error_return);
        BLAST_ScoreBlkDestruct(sbp);
        MemFree(posSearch);
        return NULL;
    }
    sbp->kbp_gap_psi[0] = BlastKarlinBlkCreate();
    rv = BlastKarlinBlkGappedCalc(sbp->kbp_gap_psi[0], gap_open, gap_extend, 
            sbp->name, &error_return);
    if (rv == 1) {
        BlastErrorPrint(error_return);
        BLAST_ScoreBlkDestruct(sbp);
        MemFree(posSearch);
        return NULL;
    }

    if (sbp->kbp_ideal == NULL)
        sbp->kbp_ideal = BlastKarlinBlkStandardCalcEx(sbp);
    compactSearch->lambda =  sbp->kbp_gap_std[0]->Lambda;
    compactSearch->kbp_std = sbp->kbp_std;
    compactSearch->kbp_psi = sbp->kbp_psi;
    compactSearch->kbp_gap_psi = sbp->kbp_gap_psi;
    compactSearch->kbp_gap_std = sbp->kbp_gap_std;
    compactSearch->lambda_ideal = sbp->kbp_ideal->Lambda;
    compactSearch->K_ideal = sbp->kbp_ideal->K;

    /* Initialize the posSearch structure */
    posSearch->posFreqs = posFreqs;
    posSearch->posMatrix = B2SAllocateScoreMatrix(qlen+1, alphabet_sz);
    posSearch->posPrivateMatrix = B2SAllocateScoreMatrix(qlen+1, alphabet_sz);
    if (!posSearch->posMatrix || !posSearch->posPrivateMatrix) {
        ErrPostEx(SEV_ERROR, 0, 0, "B2SCalculateScaledPSSM: Out of memory");
        BLAST_ScoreBlkDestruct(sbp);
        MemFree(posSearch->posMatrix); MemFree(posSearch->posPrivateMatrix);
        MemFree(posSearch);
        return NULL;
    }

    posFreqsToMatrix(posSearch, compactSearch);
    impalaScaling(posSearch, compactSearch, scalingFactor, TRUE);
    if (karlinK)
        *karlinK = compactSearch->kbp_gap_psi[0]->K;

    for (i = 0; i <= qlen; i++)
        MemFree(posSearch->posMatrix[i]);
    MemFree(posSearch->posMatrix);
    BLAST_ScoreBlkDestruct(sbp);
    retval = posSearch->posPrivateMatrix;
    MemFree(posSearch);

    return retval;
}

/* Calculates the PSSM for a given SeqLocPtr */
static BLAST_ScorePtr *B2SCalculatePSSM(SeqLocPtr slp, BlastSearchBlkPtr search,
        BLAST_MatrixPtr matrix, Nlm_FloatHiPtr karlinK)
{
    BLAST_ScorePtr *posMatrix = NULL;
    compactSearchItems *compactSearch = NULL;
    Boolean replaced_sequence = FALSE;
    Int4 query_length, full_query_length;
    SeqLocPtr filter_slp = NULL, full_slp = NULL;
    Uint1Ptr sequence = NULL;
    BlastSequenceBlk bseq;
    Nlm_FloatHi scalingFactor = search->pbp->scalingFactor;

    query_length = SeqLocLen(slp);

    /* if the slp is not the whole sequence, retrieve the whole sequence and
     * use it to compute the pssm */
    if (matrix->rows != (query_length+1)) {
        SeqPortPtr spp = NULL;
        SeqIdPtr sip = NULL;
        Uint1 residue;
        BioseqPtr bsp = NULL;
        Char tmp[256];
        Int4 index = 0;

        sip = SeqLocId(slp);
        if ((bsp = BioseqLockById(SeqIdFindBest(sip, SEQID_GI))) == NULL) {
            SeqIdWrite(SeqLocId(slp),tmp,PRINTID_FASTA_LONG, 
                    sizeof(tmp));

            ErrPostEx(SEV_ERROR,0,0,"Could not retrieve full bioseq "
                    "for %s",tmp);
            BioseqUnlock(bsp);
            return NULL;
        }
                    
        /* get full sequence to be used in WposComputation */
        spp = SeqPortNew(bsp, FIRST_RESIDUE, LAST_RESIDUE, Seq_strand_unknown,
                Seq_code_ncbistdaa);

        full_query_length = bsp->length;
        sequence = (Uint1Ptr) MemNew(2*((bsp->length)+2)*sizeof(Char));
        BioseqUnlock(bsp);

        sequence[index++] = NULLB;
        while ((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF) {
            if (IS_residue(residue)) {
                if (residue == 24) { /* change selenocysteine to X */
                    residue = 21;
                    ErrPostEx(SEV_WARNING,0,0, "Selenocysteine (U) at "
                        "position %ld replaced by X", (long) index+1);
                }
                sequence[index++] = residue;
            }
        }
        sequence[index] = NULLB;
        spp = SeqPortFree(spp);

        /* Filter the sequence if necessary */
        ValNodeAddPointer(&full_slp, SEQLOC_WHOLE, SeqIdDup(SeqLocId(slp)));
        filter_slp = BlastSeqLocFilter(full_slp, search->pbp->filter_string);
        if(search->pbp->query_lcase_mask != NULL)
            filter_slp = blastMergeFilterLocs(filter_slp, 
                    search->pbp->query_lcase_mask, FALSE, 0, 0);

        BlastMaskTheResidues(sequence+1, full_query_length, 21, filter_slp, 
                FALSE, SeqLocStart(full_slp));

        /* Save the current query sequence */
        MemCpy(&bseq, search->context[0].query, sizeof(BlastSequenceBlk));

		BlastSequenceAddSequence(search->context[0].query, NULL, sequence, 
                                 full_query_length, full_query_length, 0);

        SeqLocSetFree(full_slp);
        SeqLocSetFree(filter_slp);
        replaced_sequence = TRUE;
    }

    compactSearch = compactSearchNew(compactSearch);
    copySearchItems(compactSearch, search, search->sbp->name);
    compactSearch->pseudoCountConst = search->pbp->pseudoCountConst;
    if (scalingFactor != 0.0 && scalingFactor != 1.0) {
        /* build pssm {make,copy}mat/rpsblast style */
        posMatrix = B2SCalculateScaledPSSM(search, search->sbp->posFreqs,
                compactSearch, karlinK);
    } else {
        /* build pssm psiblast style */
        posMatrix = WposComputation(compactSearch, NULL, search->sbp->posFreqs);
    }
    compactSearchDestruct(compactSearch);

    if (replaced_sequence) {
        MemCpy(search->context[0].query, &bseq, sizeof(BlastSequenceBlk));
        MemFree(sequence);
    }

    return posMatrix;
}

/* Checks if the dimensions of the pssm attached to the search->sbp are
 * consistent with the length of the master query (slp), and trims the matrix
 * if necessary */
static Boolean B2SVerifyPSSM(SeqLocPtr slp, BlastSearchBlkPtr search,
        BLAST_MatrixPtr matrix)
{
    Int4 i, query_length = SeqLocLen(slp);

    if ((query_length+1) > matrix->rows) {
        ErrPostEx(SEV_WARNING,0,0,"Ignoring PSSM because it seems not to "
            "correspond to query sequence (query length  = %ld, PSSM's "
            "number of rows = %ld)", query_length+1, matrix->rows);
        search->positionBased = FALSE;

        if (matrix->matrix == NULL) {
            BLAST_ScorePtr *posMatrix = search->sbp->posMatrix;

            for (i = 0; i < matrix->rows; i++)
                posMatrix[i] = MemFree(posMatrix[i]);
            posMatrix = MemFree(posMatrix);
        }
        search->sbp->posMatrix = NULL;
        search->sbp->posFreqs = NULL;
        return FALSE;
    } else if ((query_length+1) < matrix->rows) {
        /* Assume BLAST_Matrix corresponds to the entire sequence, so trim
         * it */
        Int4 from, to, i, j, alphabet_sz;
        BLAST_ScorePtr *pssm = NULL;

        if (slp->choice != SEQLOC_INT) {
            ErrPostEx(SEV_ERROR,0,0,"B2SVerifyPSSM: SeqLocPtr is not a "
                    "SEQLOC_INT, cannot trim matrix");
            return FALSE;
        }

        from = SeqLocStart(slp);
        to = SeqLocStop(slp);
        alphabet_sz = matrix->columns;
        
        /* Adjust the pssm */
        pssm = (BLAST_ScorePtr *)MemNew(sizeof(BLAST_ScorePtr) *
                (query_length+1));
        for (i = 0; i <= query_length; i++) {
            pssm[i] = (BLAST_ScorePtr)MemNew(sizeof(BLAST_Score) *
                    alphabet_sz);
        }

        for (i = from; i <= to; i++) {
            for (j = 0; j < alphabet_sz; j++)
                pssm[(i-from)][j] = search->sbp->posMatrix[i][j];
        }
        for (j = 0; j < alphabet_sz; j++)
            pssm[query_length][j] = BLAST_SCORE_MIN;

        if (matrix->matrix == NULL) {
            /* Free the matrix we calculated originally */
            BLAST_ScorePtr *posMatrix = search->sbp->posMatrix;

            for (i = 0; i < matrix->rows; i++)
                posMatrix[i] = MemFree(posMatrix[i]);
            posMatrix = MemFree(posMatrix);
        }
        search->sbp->posMatrix = pssm;

    }
    return TRUE;
}

/* psi-blast2sequences setup: matrix must contain at least the residue
 * frequencies to calculate the PSSM. Otherwise, if the PSSM is given, that
 * will be used. */
Boolean LIBCALL B2SPssmSetupSearch(BlastSearchBlkPtr search, 
        SeqLocPtr pssm_slp, BLAST_MatrixPtr matrix)
{
    Nlm_FloatHi karlinK = 0.0;
    Int4 npos, alphabet_size;

    if (!search || !matrix)
        return FALSE;

    if (search->prog_number != blast_type_blastp) {
        ErrPostEx(SEV_ERROR, 0, 0, "B2SPssmSetupSearch: only blastp is "
                "supported");
        return FALSE;
    }

    search->positionBased = TRUE;
    npos = SeqLocLen(pssm_slp);
    alphabet_size = search->sbp->alphabet_size;

    if (npos <= 0) {
        ErrPostEx(SEV_ERROR, 0, 0, "B2SPssmSetupSearch: length of pssm_slp "
                "must be positive");
        return FALSE;
    }

    /* save the residue frequencies, we might need them later */
    if (matrix->posFreqs) {
        search->sbp->posFreqs = allocatePosFreqs(npos, alphabet_size);
        copyPosFreqs(matrix->posFreqs, search->sbp->posFreqs, npos,
                alphabet_size);
    }

    if (matrix->posFreqs && !matrix->matrix) {
        search->sbp->posMatrix = B2SCalculatePSSM(pssm_slp, search, matrix,
                &karlinK);
        /* if we calculated the pssm, and use did not provide one, save it*/
        if (matrix->karlinK == 0.0 && karlinK != 0.0)
            matrix->karlinK = karlinK;
    } else {
        search->sbp->posMatrix = matrix->matrix;
    }

    search->sbp->mat_dim1 = search->sbp->query_length + 1;
    search->sbp->mat_dim2 = search->sbp->alphabet_size;

    /* Sanity check */
    if (!search->sbp->posMatrix) {
        ErrPostEx(SEV_ERROR, 0, 0, "B2SPssmSetupSearch: "
                "Could not create or obtain PSSM! Please verify "
                "BLAST_Matrix parameter");
        search->positionBased = FALSE;
        return FALSE;
    }

    /* Make sure the BLAST_Matrix number of rows is consistent with
     * pssm_slp */
    B2SVerifyPSSM(pssm_slp, search, matrix);

    if (matrix->karlinK != 0.0) {
        search->sbp->karlinK = matrix->karlinK;
        search->sbp->kbp_gap_psi[0]->K = matrix->karlinK;
        search->sbp->kbp_gap_psi[0]->logK = log(matrix->karlinK);
    }

    return TRUE;
}

/* clean up psi-blast2sequences */
Boolean LIBCALL B2SPssmCleanUpSearch(BlastSearchBlkPtr search, 
        BLAST_MatrixPtr matrix)
{
    Int4 i, rows = search->sbp->query_length + 1;
    BLAST_ScorePtr *posMatrix = search->sbp->posMatrix;
    Nlm_FloatHiPtr *posFreqs = search->sbp->posFreqs;

    if (!matrix)
        return FALSE;
    
    if ((matrix->matrix == NULL) || /* B2SPssmSetupSearch created PSSM */
        (posMatrix != matrix->matrix)) { /* B2SVerifyPSSM trimmed PSSM */
        for (i = 0; i < rows; i++)
            posMatrix[i] = MemFree(posMatrix[i]);
        posMatrix = MemFree(posMatrix);
    }
    if (matrix->posFreqs) {
        for (i = 0; i < rows; i++)
            posFreqs[i] = MemFree(posFreqs[i]);
        posFreqs = MemFree(posFreqs);
    }
    search->sbp->posMatrix = NULL;
    search->sbp->posFreqs = NULL;
    search->positionBased = FALSE;
    return TRUE;
}

SeqAlignPtr LIBCALL B2SPssmOnTheFlyByLoc(BlastSearchBlkPtr search, 
            SeqLocPtr subj_slp) 
{
    Int4 index, subject_length;
    SeqAlignPtr seqalign = NULL;
    Uint1Ptr subject_seq = NULL, subject_seq_start = NULL;
    SeqPortPtr spp;
    Uint1 residue;

    if (!search || search->query_invalid || !subj_slp)
        return NULL;

    if (search->result_struct)
        search->result_struct = BLASTResultsStructDelete(search->result_struct);
    search->result_struct = BLASTResultsStructNew(search->result_size, 
                 search->pbp->max_pieces, search->pbp->hsp_range_max);
    BlastHitListPurge(search->current_hitlist);

    subject_length = SeqLocLen(subj_slp);

    if (search->prog_number == blast_type_blastp) {
        subject_seq_start = (Uint1Ptr) MemNew(
                ((subject_length)+2)*sizeof(Uint1));
        /* The first residue is the sentinel. */
        subject_seq_start[0] = NULLB;
        subject_seq = subject_seq_start+1;
        index = 0;
        spp = SeqPortNewByLoc(subj_slp, Seq_code_ncbistdaa);
        while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
            if (IS_residue(residue))
                subject_seq[index++] = residue;
        }
        subject_seq[index] = NULLB;
        spp = SeqPortFree(spp);
    } else {
        return NULL;
    }

    seqalign = BlastTwoSequencesCore(search, subj_slp, subject_seq, 
            subject_length, FALSE);

    MemFree(subject_seq_start);
    AdjustOffSetsInSeqAlign(seqalign, search->query_slp, subj_slp);

    return seqalign;
}

SeqAlignPtr LIBCALL B2SPssmOnTheFly(BlastSearchBlkPtr search, 
        BioseqPtr subj_bsp) 
{
    SeqAlignPtr salp = NULL;
    SeqLocPtr slp = NULL;

    if (!search || search->query_invalid || !subj_bsp)
        return NULL;

    ValNodeAddPointer(&slp, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBest(subj_bsp->id,
                    SEQID_GI)));
    salp = B2SPssmOnTheFlyByLoc(search, slp);
    SeqLocFree(slp);
    return salp;
}

SeqAlignPtr * LIBCALL B2SPssmMultipleQueries(SeqLocPtr pssm_slp,
        BLAST_MatrixPtr matrix, SeqLocPtr *target_seqs, Int4 ntargets,
        BLAST_OptionsBlkPtr options)
{
    SeqAlignPtr *sa_array = NULL;
    BlastSearchBlkPtr search = NULL;
    Int4 i;

    if (!matrix || !pssm_slp || !target_seqs || ntargets <= 0 || !options)
        return NULL;

    /* Set up search structure */
    search = BLASTSetUpSearchByLoc(pssm_slp, options->program_name,
            SeqLocLen(pssm_slp), 0, NULL, options, NULL);
    B2SPssmSetupSearch(search, pssm_slp, matrix);

    /* Allocate memory for return value */
    if (!(sa_array = (SeqAlignPtr*)MemNew(sizeof(SeqAlignPtr)*ntargets))) {
        ErrPostEx(SEV_ERROR, 0, 0, "B2SPssmMultipleQueries: Out of memory");
        BlastSearchBlkDestruct(search);
        return NULL;
    }


    /* Iterate over seqlocs in target_seqs, using effective search space in
     * rpsblast style */
    for (i = 0; i < ntargets; i++) {
        Int8 dblen = (options->db_length != 0) ? 
                        options->db_length : SeqLocLen(pssm_slp);
        Int4 nseqs = (options->dbseq_num != 0) ?  options->dbseq_num : 1;

        /* If search space has been specified in the options structure, the it
         * must have been set in BLASTSetUpSearchEx, so don't overwrite it */
        if ( ! (options->searchsp_eff > 0) ) {
            search->searchsp_eff  = BLASTCalculateSearchSpace(options, nseqs, 
                    dblen, SeqLocLen(target_seqs[i]));
        }
        sa_array[i] = B2SPssmOnTheFlyByLoc(search, target_seqs[i]);
    }

    /* Clean up */
    B2SPssmCleanUpSearch(search, matrix);
    BlastSearchBlkDestruct(search);

    return sa_array;
}

/************************************************************************/
/* END    PSIBLAST2Sequences API                                        */
/************************************************************************/

/* Note that the matrix parameter should correspond to the full master
 * sequence */
SeqAlignPtr LIBCALL
BlastTwoSequencesByLocWithCallback(SeqLocPtr slp1, SeqLocPtr slp2, CharPtr
        progname, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns,
        ValNodePtr *error_returns, int (LIBCALLBACK
            *handle_results)PROTO((VoidPtr srch)), BLAST_MatrixPtr matrix)
{
	BlastAllWordPtr all_words;
	BlastSearchBlkPtr search;
	BioseqPtr subject_bsp;
	Boolean complement=FALSE, reverse, reverse_forbidden, options_alloc;
	Int2 status;
	Int4 index, subject_length, num_of_cols;
	SeqAlignPtr seqalign=NULL;
	SeqLocPtr query_slp, subject_slp;
	SeqPortPtr spp;
	SPCompressPtr spc=NULL;
	Uint1 residue;
	Uint1Ptr subject_seq, subject_seq_start;
	Uint1Ptr *array;

	if (slp1 == NULL || slp2 == NULL)
		return NULL;

	if (error_returns)
	{
		*error_returns = NULL;
	}

	if (other_returns)
	{
		*other_returns = NULL;
	}

	if (progname == NULL && options == NULL)
		return NULL;

	/* If filtering is performed, do not reverse the sequence.  
           In this case the wrong sequence would be filtered. */
	reverse_forbidden = FALSE;
	if ((options && ((options->filter_string &&
                        StringCmp(options->filter_string, "F")) ||
                        options->is_megablast_search)) ||
                        matrix != NULL)
	{
		reverse_forbidden = TRUE;
	}

	/* Select the shorter sequence as the query, provided they are 
	   of the same type. */
	if ((StringCmp(progname, "blastn") && StringCmp(progname, "blastp")) ||
	    (reverse_forbidden || SeqLocLen(slp1) < SeqLocLen(slp2)))
	{
		query_slp = slp1;
		subject_slp = slp2;
		reverse = FALSE;
	}
	else
	{
		query_slp = slp2;
		subject_slp = slp1;
		reverse = TRUE;
	}

    /* Make sure strands are handled correctly */
    if (!StringCmp(progname, "blastn") &&
        SeqLocStrand(query_slp) != Seq_strand_both && 
        SeqLocStrand(subject_slp) == Seq_strand_both) {
       Change_Loc_Strand(subject_slp, SeqLocStrand(query_slp));
       Change_Loc_Strand(query_slp, Seq_strand_both);
    }

	if (progname == NULL)
	{
		progname = options->program_name;
	}

	/* If the subject strand is minus, turn it into plus for blastn. */
	/* Complement the other strand to keep things straight. */
	if (StringCmp(progname, "blastn") == 0 && SeqLocStrand(subject_slp) == Seq_strand_minus)
	{
		complement = TRUE;
                if(SeqLocStrand(query_slp) == Seq_strand_plus ||
			SeqLocStrand(query_slp) == Seq_strand_minus)
				SeqLocRevCmp(query_slp);
		SeqLocRevCmp(subject_slp);
	}

	subject_seq_start = subject_seq = NULL;

    /* Allocate default options if none are allocated yet. */
    options_alloc = FALSE;
    if (options == NULL)
    {
            options = BLASTOptionNew(progname, FALSE);
            options_alloc = TRUE;
    }

    status = BLASTOptionValidateEx(options, progname, error_returns);
    if (status != 0)
    {       /* error messages in other_returns? */
            return NULL;
    }

	all_words = NULL;

	subject_length = SeqLocLen(subject_slp);

	if (!StringCmp(progname, "blastp") || 
	    !StringCmp(progname, "blastx"))
	{
		subject_seq_start = (Uint1Ptr) MemNew(((subject_length)+2)*sizeof(Uint1));
		/* The first residue is the sentinel. */
		subject_seq_start[0] = NULLB;
		subject_seq = subject_seq_start+1;
		index = 0;
		spp = SeqPortNewByLoc(subject_slp, Seq_code_ncbistdaa);
		while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF)
		{
			if (IS_residue(residue))
			{
				subject_seq[index] = residue;
				index++;
			}
		}
		subject_seq[index] = NULLB;

		num_of_cols = subject_length+1-options->wordsize;
		all_words = BlastAllWordNew(num_of_cols, options->wordsize, FALSE, TRUE);
		array = (Uint1Ptr *) MemNew(num_of_cols*sizeof(Uint1Ptr));
		for (index=0; index<num_of_cols; index++)
		{
			array[index] = subject_seq+index;
		}
		all_words->array = array;
		spp = SeqPortFree(spp);
		if (options->gapped_calculation == TRUE)
		{ 
			options->two_pass_method  = FALSE;
			options->multiple_hits_only  = TRUE;
		}
	}
	else if (!StringCmp(progname, "blastn") || 
		 !StringCmp(progname, "tblastn") ||
                 !StringCmp(progname, "psitblastn") ||
		 !StringCmp(progname, "tblastx"))
	{
		spp = SeqPortNewByLoc(subject_slp, Seq_code_ncbi4na);
                subject_bsp = BioseqFindCore(SeqLocId(subject_slp));
                if (subject_bsp != NULL && subject_bsp->repr == Seq_repr_delta)
                        SeqPortSet_do_virtual(spp, TRUE);
		spc = SPCompressDNA(spp);
		if (spc == NULL)
			return NULL;
		subject_seq_start = subject_seq = spc->buffer;
		spp = SeqPortFree(spp);
	}
	else /* Impossible! */
	{
	        return NULL;
	}
	
    if (options->is_megablast_search)
        /* This has a different meaning in Mega BLAST and must be 0 */
        options->block_width = 0;

    if (options->db_length == 0)
        options->db_length = subject_length;

    options->dbseq_num = 1;

    search = BLASTSetUpSearchByLoc(query_slp, progname, SeqLocLen(query_slp), subject_length, all_words, options, NULL);

	if (search == NULL) 
        return NULL;

    if (search->query_invalid) {
        search = BlastSearchBlkDestruct(search);
		return NULL;
    }

        if (!StringCmp(progname, "tblastn") ||
            !StringCmp(progname, "tblastx") ||
            !StringCmp(progname, "psitblastn")) {
	   MemFree(search->translation_buffer);
	   search->translation_buffer = MemNew((3+(subject_length/3))*sizeof(Uint1));
	   search->translation_buffer_size = 1+(subject_length/3);
	}

    B2SPssmSetupSearch(search, slp1, matrix);

    search->handle_results = handle_results;
    search->output = options->output;

	seqalign = BlastTwoSequencesCore(search, subject_slp, subject_seq, subject_length, reverse);

#if 0
    /* This is turned off for right now */
    if (options->tweak_parameters || options->smith_waterman) {
        Boolean free_subject_info = FALSE;

        if (search->subject_info == NULL) {
            search->subject_info = BLASTSubjectInfoNew(
                    SeqIdSetDup(SeqLocId(subject_slp)), NULL, subject_length);
            free_subject_info = TRUE;
        }

        /* Use composition-based statistics */
        seqalign = RedoAlignmentCore(search, options, 
                search->result_struct->hitlist_count,
                options->tweak_parameters, options->smith_waterman);
        for (index = 0; index<search->result_struct->hitlist_count; index++)
            BLASTResultFreeHsp(search->result_struct->results[index]);
        for (index = 0; index<search->result_struct->hitlist_count; index++)
            SeqAlignSetFree(search->result_struct->results[index]->seqalign);

        if (free_subject_info)
            search->subject_info =
                BLASTSubjectInfoDestruct(search->subject_info);
    }
#endif

	if (complement)
	{
		seqalign = SeqAlignListReverseStrand(seqalign);
		SeqLocRevCmp(query_slp);
		SeqLocRevCmp(subject_slp);
	}

	if (spc)
	{
		SPCompressFree(spc);
		spc = NULL;
	}
	else
	{
		subject_seq_start = MemFree(subject_seq_start);
	}
	
	if (search->error_return)
	{
		ValNodeLink(error_returns, search->error_return);
		search->error_return = NULL;
	}

	if (other_returns)
	{ /* format dbinfo etc.  */
		*other_returns = BlastOtherReturnsPrepare(search);
	}

    if (options_alloc)
        options = BLASTOptionDelete(options);

    AdjustOffSetsInSeqAlign(seqalign, slp1, slp2);

    B2SPssmCleanUpSearch(search, matrix);

	search = BlastSearchBlkDestruct(search);

	return seqalign;
}

SeqAlignPtr LIBCALL
BlastTwoSequencesByLoc(SeqLocPtr slp1, SeqLocPtr slp2, CharPtr progname, BLAST_OptionsBlkPtr options)
{
	return BlastTwoSequencesByLocEx(slp1, slp2, progname, options, NULL, NULL);
}

SeqAlignPtr LIBCALL
BlastTwoSequencesEx(BioseqPtr bsp1, BioseqPtr bsp2, CharPtr progname, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns)
{
   return BlastTwoSequencesWithCallback(bsp1, bsp2, progname, options,
           other_returns, error_returns, NULL);
}

SeqAlignPtr LIBCALL
BlastTwoSequencesWithCallback(BioseqPtr bsp1, BioseqPtr bsp2, CharPtr progname, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *handle_results)PROTO((VoidPtr search)))
{
	SeqAlignPtr seqalign;
	SeqLocPtr slp1=NULL, slp2=NULL;

	if (bsp1 == NULL || bsp2 == NULL)
		return NULL;

	slp1 = NULL;
	slp2 = NULL;
    if (!handle_results) {
       ValNodeAddPointer(&slp1, SEQLOC_WHOLE,
                         SeqIdDup(SeqIdFindBest(bsp1->id, SEQID_GI)));
       ValNodeAddPointer(&slp2, SEQLOC_WHOLE,
                         SeqIdDup(SeqIdFindBest(bsp2->id, SEQID_GI)));
    } else {
       ValNodeAddPointer(&slp1, SEQLOC_WHOLE, 
                         SeqIdDup(SeqIdFindBestAccession(bsp1->id)));
       ValNodeAddPointer(&slp2, SEQLOC_WHOLE, 
                         SeqIdDup(SeqIdFindBestAccession(bsp2->id)));
    }
    seqalign = BlastTwoSequencesByLocWithCallback(slp1, slp2, progname,
            options, other_returns, error_returns, handle_results, NULL);

    slp1 = SeqLocFree(slp1);
    slp2 = SeqLocFree(slp2);
    
    return seqalign;
}

SeqAlignPtr LIBCALL
BlastTwoSequences(BioseqPtr bsp1, BioseqPtr bsp2, CharPtr progname, BLAST_OptionsBlkPtr options)
{
	return BlastTwoSequencesEx(bsp1, bsp2, progname, options, NULL, NULL);
}

/*
	Runs blast on the fly between the query BioseqPtr (specified with a
	call to BLASTSetUpSearch) and the subject BioseqPtr.
*/


BlastSearchBlkPtr LIBCALL
BlastSequencesOnTheFlyEx(BlastSearchBlkPtr search, BioseqPtr subject_bsp)
{
	Int4 index, subject_length;
	SeqPortPtr spp;
	SPCompressPtr spc=NULL;
	Uint1Ptr subject_seq, subject_seq_start;
	Uint1 residue;

	if (subject_bsp == NULL)
		return NULL;

	if (search == NULL || search->query_invalid)
		return NULL;

        if (!search->pbp->mb_params) {
           if (search->result_struct)
              search->result_struct =
                 BLASTResultsStructDelete(search->result_struct);
           search->result_struct = 
              BLASTResultsStructNew(search->result_size, 
                 search->pbp->max_pieces, search->pbp->hsp_range_max);
        } else {
           if (search->mb_result_struct && search->mb_result_struct[0])
              search->mb_result_struct[0] = 
                 BLASTResultsStructDelete(search->mb_result_struct[0]);
           if (!search->mb_result_struct)
              search->mb_result_struct = (BLASTResultsStructPtr PNTR) 
                 MemNew(sizeof(BLASTResultsStructPtr));
        }

	BlastHitListPurge(search->current_hitlist);

	subject_seq_start = subject_seq = NULL;

	subject_length = subject_bsp->length;

	if (StringCmp(search->prog_name, "blastp") == 0)
	{
		subject_seq_start = (Uint1Ptr) MemNew(((subject_length)+2)*sizeof(Uint1));
		/* The first residue is the sentinel. */
		subject_seq_start[0] = NULLB;
		subject_seq = subject_seq_start+1;
		index = 0;
		spp = SeqPortNew(subject_bsp, FIRST_RESIDUE, LAST_RESIDUE,
				 0, Seq_code_ncbistdaa);
		while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF)
		{
			if (IS_residue(residue))
			{
				subject_seq[index] = residue;
				index++;
			}
		}
		subject_seq[index] = NULLB;
		spp = SeqPortFree(spp);
	}
	else if (StringCmp(search->prog_name, "blastn") == 0)
	{
		spp = SeqPortNew(subject_bsp, FIRST_RESIDUE, LAST_RESIDUE,
				 0, Seq_code_ncbi4na);
		spc = SPCompressDNA(spp);
		subject_seq = spc->buffer;
		spp = SeqPortFree(spp);
	}
	else
	{
		return NULL;
	}

	BlastTwoSequencesCoreEx(search, subject_bsp, subject_seq, 
					 subject_length);

	if (spc)
	{
		SPCompressFree(spc);
		spc = NULL;
	}
	else
	{
		subject_seq_start = MemFree(subject_seq_start);
	}
	
   return search;
}

SeqAlignPtr LIBCALL
BlastSequencesOnTheFlyByLoc(BlastSearchBlkPtr search, SeqLocPtr subject_slp)
{
	Int4 index, subject_length;
	SeqAlignPtr seqalign=NULL;
	SeqPortPtr spp;
	SPCompressPtr spc=NULL;
	Uint1Ptr subject_seq, subject_seq_start;
	Uint1 residue;

	if (subject_slp == NULL)
		return NULL;

	if (search == NULL || search->query_invalid)
		return NULL;


        if (!search->pbp->mb_params) {
           if (search->result_struct)
              search->result_struct = BLASTResultsStructDelete(search->result_struct);
           search->result_struct = 
              BLASTResultsStructNew(search->result_size, 
                 search->pbp->max_pieces, search->pbp->hsp_range_max);
        } else {
           if (search->mb_result_struct && search->mb_result_struct[0])
              search->mb_result_struct[0] = 
                 BLASTResultsStructDelete(search->mb_result_struct[0]);
           if (!search->mb_result_struct)
              search->mb_result_struct = (BLASTResultsStructPtr PNTR) 
                 MemNew(sizeof(BLASTResultsStructPtr));
        }
	BlastHitListPurge(search->current_hitlist);

	subject_seq_start = subject_seq = NULL;

	subject_length = SeqLocLen(subject_slp);

	if (StringCmp(search->prog_name, "blastp") == 0)
	{
		subject_seq_start = (Uint1Ptr) MemNew(((subject_length)+2)*sizeof(Uint1));
		/* The first residue is the sentinel. */
		subject_seq_start[0] = NULLB;
		subject_seq = subject_seq_start+1;
		index = 0;
		spp = SeqPortNewByLoc(subject_slp, Seq_code_ncbistdaa);
		while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF)
		{
			if (IS_residue(residue))
			{
				subject_seq[index] = residue;
				index++;
			}
		}
		subject_seq[index] = NULLB;
		spp = SeqPortFree(spp);
	}
	else if (StringCmp(search->prog_name, "blastn") == 0)
	{
		spp = SeqPortNewByLoc(subject_slp, Seq_code_ncbi4na);
		spc = SPCompressDNA(spp);
		subject_seq = spc->buffer;
		spp = SeqPortFree(spp);
	}
	else
	{
		return NULL;
	}

	seqalign = BlastTwoSequencesCore(search, subject_slp, subject_seq, subject_length, FALSE);

	if (spc)
	{
		SPCompressFree(spc);
		spc = NULL;
	}
	else
	{
		subject_seq_start = MemFree(subject_seq_start);
	}
	
	AdjustOffSetsInSeqAlign(seqalign, search->query_slp, subject_slp);

	return seqalign;
}

SeqAlignPtr LIBCALL
BlastSequencesOnTheFly(BlastSearchBlkPtr search, BioseqPtr subject_bsp)
{
	SeqAlignPtr seqalign;
	SeqLocPtr slp;

	slp = NULL;
	ValNodeAddPointer(&slp, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBest(subject_bsp->id, SEQID_GI)));
	seqalign = BlastSequencesOnTheFlyByLoc(search, slp);
	SeqLocFree(slp);
	return seqalign;
}
/*
	Translate a nucleotide sequence without ambiguity codes.
	This is used for the first-pass translation of the database.
	
	BlastSearchBlkPtr search: overall BLAST structure.
	Int4 length: length of the nucl. sequence
	Uint1Ptr prot_seq: the (translated) protein sequence, with NULLB
		sentinels on either end.  This array should be allocated
		with sufficient memory before the function is called.
	Uint1Ptr nt_seq: the original nucl. sequence.
	
	The genetic code to be used is determined by the translation_table
	on the BlastSearchBlkPtr.

	This function translates a packed (ncbi2na) nucl. alphabet.  It
	views a basepair as being in one of four sets of 2-bits:

	|0|1|2|3||0|1|2|3||0|1|2|3||...

	1st byte | 2 byte | 3rd byte...

	A codon that starts at the beginning of the above sequence starts in
	state "0" and includes basepairs 0, 1, and 2.  The next codon, in the
	same frame, after that starts in state "3" and includes 3, 0, and 1.

	** Optimization:
	  changed the single main loop to 
	     - advance to state 0, 
	     - optimized inner loop does two (3 byte->4 codon) translation per iteration
	           (loads are moved earlier so they can be done in advance.)
	     - do remainder
*/

Int4 LIBCALL
BlastTranslateUnambiguousSequence(BlastSearchBlkPtr search, Int4 length, Uint1Ptr prot_seq, Uint1Ptr nt_seq, Int2 frame)

{
	register int state;
	Int2 total_remainder;
	Int4 prot_length;
	register int byte_value, codon=0;
	Uint1 last_remainder, last_byte, remainder;
	register Uint1Ptr translation, nt_seq_end, nt_seq_start;
	Uint1Ptr prot_seq_start;
	int byte_value1,byte_value2,byte_value3,byte_value4,byte_value5;
  
	prot_length=0;
	if (nt_seq == NULL || prot_seq == NULL || (length-ABS(frame)+1) < CODON_LENGTH)
	return prot_length;

	*prot_seq = NULLB;
	prot_seq++;

/* record to determine protein length. */
	prot_seq_start = prot_seq;
  
	if (frame > 0)
		translation = search->translation_table;
	else
		translation = search->translation_table_rc;

	remainder = length%4;

	if (frame > 0)
	{
		nt_seq_end = nt_seq + (length)/4 - 1;
		last_remainder = (4*(length/4) - frame + 1)%CODON_LENGTH;
		total_remainder = last_remainder+remainder;
			
		state = frame-1;
		byte_value = *nt_seq;

		/* If there's lots to do, advance to state 0, then enter fast loop */
		while (nt_seq < nt_seq_end)
		{
			switch (state)
			{
				case 0:
					codon = (byte_value >> 2);
					*prot_seq = translation[codon];
					prot_seq++;
				/* do state = 3 now, break is NOT missing. */
				case 3:
					codon = ((byte_value & 3) << 4);
					nt_seq++;
					byte_value = *nt_seq;	
					codon += (byte_value >> 4);
					*prot_seq = translation[codon];
					prot_seq++;
					if (nt_seq >= nt_seq_end)
					{
						state = 2;
						break;
					}
				/* Go on to state = 2 if not at end. */
				case 2:
					codon = ((byte_value & 15) << 2);
					nt_seq++;
					byte_value = *nt_seq;	
					codon += (byte_value >> 6);
					*prot_seq = translation[codon];
					prot_seq++;
					if (nt_seq >= nt_seq_end)
					{
						state = 1;
						break;
					}
				/* Go on to state = 1 if not at end. */
				case 1:
					codon = byte_value & 63;
					*prot_seq = translation[codon];
					prot_seq++;
					nt_seq++;
					byte_value = *nt_seq;	
					state = 0;
					break;
			} /* end switch */
			/* switch ends at state 0, except when at end */


			/********************************************/
			/* optimized loop: start in state 0. continue til near end */
			while (nt_seq < (nt_seq_end-10))
			  {
			    byte_value1 = *(++nt_seq);
			    byte_value2 = *(++nt_seq);
			    byte_value3 = *(++nt_seq);
			    /* case 0: */
			    codon = (byte_value >> 2);
			    *prot_seq = translation[codon];
			    prot_seq++;

			    /* case 3: */
			    codon = ((byte_value & 3) << 4);
			    codon += (byte_value1 >> 4);
			    *prot_seq = translation[codon];
			    prot_seq++;

			    byte_value4 = *(++nt_seq);
			    /* case 2: */
			    codon = ((byte_value1 & 15) << 2);

			    codon += (byte_value2 >> 6);
			    *prot_seq = translation[codon];
			    prot_seq++;
			    /* case 1: */
			    codon = byte_value2 & 63;
			    byte_value5 = *(++nt_seq);
			    *prot_seq = translation[codon];
			    prot_seq++;

			    /* case 0: */
			    codon = (byte_value3 >> 2);
			    *prot_seq = translation[codon];
			    prot_seq++;
			    /* case 3: */
			    byte_value = *(++nt_seq);
			    codon = ((byte_value3 & 3) << 4);
			    codon += (byte_value4 >> 4);
			    *prot_seq = translation[codon];
			    prot_seq++;
			    /* case 2: */
			    codon = ((byte_value4 & 15) << 2);
			    codon += (byte_value5 >> 6);
			    *prot_seq = translation[codon];
			    prot_seq++;
			    /* case 1: */
			    codon = byte_value5 & 63;
			    *prot_seq = translation[codon];
			    prot_seq++;
			    state=0;
			  } /* end optimized while */
		/********************************************/
		} /* end while */


		if (state == 1)
		{ 
		/* This doesn't get done above, DON't do the state = 0
		   below if this is done. */
			byte_value = *nt_seq;
			codon = byte_value & 63;
			state = 0;
			*prot_seq = translation[codon];
			prot_seq++;
		}
		else if (state == 0)
		{ /* This one doesn't get done above. */
			byte_value = *nt_seq;
			codon = ((byte_value) >> 2);
			state = 3;
			*prot_seq = translation[codon];
			prot_seq++;
		}

		if (total_remainder >= CODON_LENGTH)
		{
			byte_value = *(nt_seq_end);
			last_byte = *(nt_seq_end+1);
			if (state == 0)
			{
				codon = (last_byte >> 2);
			}
			else if (state == 2)
			{
				codon = ((byte_value & 15) << 2);
				codon += (last_byte >> 6);
			}
			else if (state == 3)
			{
				codon = ((byte_value & 3) << 4);
				codon += (last_byte >> 4);
			}
			*prot_seq = translation[codon];
			prot_seq++;
		}
		*prot_seq = NULLB;
	}
	else
	{
		nt_seq_start = nt_seq;
		nt_seq += length/4;
		state = remainder+frame;
	/* Do we start in the last byte?  This one has the lowest order
	bits set to represent the remainder, hence the odd coding here. */
		if (state >= 0)
		{
			last_byte = *nt_seq;
			nt_seq--;
			if (state == 0)
			{
				codon = (last_byte >> 6);
				byte_value = *nt_seq;
				codon += ((byte_value & 15) << 2);
				state = 1;
			}
			else if (state == 1)
			{
				codon = (last_byte >> 4);
				byte_value = *nt_seq;
				codon += ((byte_value & 3) << 4);
				state = 2;
			}
			else if (state == 2)
			{
				codon = (last_byte >> 2);
				state = 3;
			}
			*prot_seq = translation[codon];
			prot_seq++;

		}
		else
		{
			state = 3 + (remainder + frame + 1);
			nt_seq--;
		}

		byte_value = *nt_seq;	

		/* If there's lots to do, advance to state 3, then enter fast loop */
		while (nt_seq > nt_seq_start)
		{
			switch (state)
			{
				case 3:
					codon = (byte_value & 63);
					*prot_seq = translation[codon];
					prot_seq++;
				/* do state = 0 now, break is NOT missing. */
				case 0:
					codon = (byte_value >> 6);
					nt_seq--;
					byte_value = *nt_seq;	
					codon += ((byte_value & 15) << 2);
					*prot_seq = translation[codon];
					prot_seq++;
					if (nt_seq <= nt_seq_start)
					{
						state = 1;
						break;
					}
				/* Go on to state = 2 if not at end. */
				case 1:
					codon = (byte_value >> 4);
					nt_seq--;
					byte_value = *nt_seq;
					codon += ((byte_value & 3) << 4);
					*prot_seq = translation[codon];
					prot_seq++;
					if (nt_seq <= nt_seq_start)
					{
						state = 2;
						break;
					}
				/* Go on to state = 2 if not at end. */
				case 2:
					codon = (byte_value >> 2);
					*prot_seq = translation[codon];
					prot_seq++;
					nt_seq--;
					byte_value = *nt_seq;	
					state = 3;
					break;
			} /* end switch */
			/* switch ends at state 3, except when at end */


			/********************************************/
			/* optimized area: start in state 0. continue til near end */
			while (nt_seq > (nt_seq_start+10))
			  {
			    byte_value1 = *(--nt_seq);	
			    byte_value2 = *(--nt_seq);
			    byte_value3 = *(--nt_seq);

			    codon = (byte_value & 63);
			    *prot_seq = translation[codon];
			    prot_seq++;
			    codon = (byte_value >> 6);
			    codon += ((byte_value1 & 15) << 2);
			    *prot_seq = translation[codon];
			    prot_seq++;
			    byte_value4 = *(--nt_seq);
			    codon = (byte_value1 >> 4);
			    codon += ((byte_value2 & 3) << 4);
			    *prot_seq = translation[codon];
			    prot_seq++;
			    codon = (byte_value2 >> 2);
			    *prot_seq = translation[codon];
			    prot_seq++;
			    byte_value5 = *(--nt_seq);

			    codon = (byte_value3 & 63);
			    *prot_seq = translation[codon];
			    prot_seq++;
			    byte_value = *(--nt_seq);
			    codon = (byte_value3 >> 6);
			    codon += ((byte_value4 & 15) << 2);
			    *prot_seq = translation[codon];
			    prot_seq++;
			    codon = (byte_value4 >> 4);
			    codon += ((byte_value5 & 3) << 4);
			    *prot_seq = translation[codon];
			    prot_seq++;
			    codon = (byte_value5 >> 2);
			    *prot_seq = translation[codon];
			    prot_seq++;
			  } /* end optimized while */
			/********************************************/

		} /* end while */

		byte_value = *nt_seq;
		if (state == 3)
		{
			codon = (byte_value & 63);
			*prot_seq = translation[codon];
			prot_seq++;
		}
		else if (state == 2)
		{
			codon = (byte_value >> 2);
			*prot_seq = translation[codon];
			prot_seq++;
		}
	}

	*prot_seq = NULLB;

	return (prot_seq - prot_seq_start);
}	/* BlastTranslateUnambiguousSequence */



/*
	Gets an appropriate ID for the database (subject) sequence.
	Int4 hit_number is the index into the BLASTResultHitlistPtr,
	Boolean ordinal_number specifies whether an ordinal number (the
	db sequence number) or a real ID should be used.
*/
SeqIdPtr LIBCALL
BlastGetSubjectIdEx(BlastSearchBlkPtr search, Int4 hit_number, Boolean ordinal_number, ValNodePtr *vnpp, Int2 query_number)
{
    BLASTResultHitlistPtr   results;
    DbtagPtr dbtagptr;
    ObjectIdPtr obidp;
    SeqIdPtr subject_id=NULL, sip;
    Uint4	header;
    BLASTResultsStructPtr result_struct;
    
    if (search->pbp->mb_params)
       result_struct = search->mb_result_struct[query_number];
    else
       result_struct = search->result_struct;

    results = result_struct->results[hit_number];
    if (ordinal_number) {
        
        obidp = ObjectIdNew();
        obidp->str = NULL;
        obidp->id = results->subject_id;
        dbtagptr = DbtagNew();
        if (search->rdfp) {
            dbtagptr->db = StringSave(search->rdfp->filename);
        }
        dbtagptr->tag = obidp;
        ValNodeAddPointer(&subject_id, SEQID_GENERAL, dbtagptr);
    }  else if (search->rdfp) {
        if (vnpp == NULL) {
            readdb_get_descriptor(search->rdfp, results->subject_id, &subject_id, NULL);
        } else {
            header = 0;
            sip = NULL;
            
            if(search->rdfp->formatdb_ver == FORMATDB_VER_TEXT) {
                while (readdb_get_header(search->rdfp, results->subject_id, &header, &sip, NULL) == TRUE)
                    ValNodeAddPointer(vnpp, 0, sip);
            } else {
                BlastDefLinePtr bdfp, bdfp_head;
                
                bdfp_head = FDReadDeflineAsn(search->rdfp, results->subject_id);
                
                if(bdfp_head == NULL) {
                    ErrPostEx(SEV_ERROR, 0, 0, "Failure to read defline ASN for %d", results->subject_id);
                    return NULL;
                }

                for(bdfp = bdfp_head; bdfp != NULL; bdfp = bdfp->next) {
                    sip = SeqIdSetDup(bdfp->seqid);
                    ValNodeAddPointer(vnpp, 0, sip);
                }

                BlastDefLineSetFree(bdfp_head);
            }
        }
    } else {
        if (results->subject_info)
            subject_id = SeqIdDup(results->subject_info->sip);
    }
    
    return subject_id;
}

SeqIdPtr LIBCALL
BlastGetSubjectId(BlastSearchBlkPtr search, Int4 hit_number, Boolean ordinal_number, ValNodePtr *vnpp)
{
   return BlastGetSubjectIdEx(search, hit_number, ordinal_number, vnpp, 0);
}

/*
	Use by HeapSort (in BioseqBlastEngine) to rank Hitlist's.
*/

int LIBCALLBACK
evalue_compare_hits(VoidPtr v1, VoidPtr v2)

{
    BLASTResultHitlistPtr h1, h2;
    BLASTResultHitlistPtr *hp1, *hp2;
    
    hp1 = (BLASTResultHitlistPtr *) v1;
    hp2 = (BLASTResultHitlistPtr *) v2;
    h1 = *hp1;
    h2 = *hp2;
    
    /* Sort first by evalue, then by score in case all evalues are zero. */

    if (h1->best_evalue < h2->best_evalue)
        return -1;
    if (h1->best_evalue > h2->best_evalue)
        return 1;
    if (h1->high_score > h2->high_score)
        return -1;
    if (h1->high_score < h2->high_score)
        return 1;
    
    /* In case of equal scores and E-values order will be determined by
       subject id */
    
    if (h1->subject_id > h2->subject_id)
        return -1;
    if (h1->subject_id < h2->subject_id)
        return 1;
    
    return 0;
}

/* Code in BLAST_CLUSTER_HITS is not currently in use */

#ifdef BLAST_CLUSTER_HITS        
typedef struct _blast_result_with_subject_id {
   BLASTResultHspPtr hsp;
   Int4 hitlist_index, hsp_index;
} BlastResultHspWithId, PNTR BlastResultHspWithIdPtr;

static int LIBCALLBACK BLASTResultHspScoreCmp(VoidPtr v1, VoidPtr v2)
{
   BLASTResultHspPtr h1, h2;
   
   h1 = (*(BlastResultHspWithIdPtr PNTR) v1)->hsp;
   h2 = (*(BlastResultHspWithIdPtr PNTR) v2)->hsp;

   if (h1->score < h2->score)
      return 1;
   else if (h1->score > h2->score)
      return -1;
   else return 0;
}

static int LIBCALLBACK ResultHspWithIdIndexCmp(VoidPtr v1, VoidPtr v2)
{
   BlastResultHspWithIdPtr h1, h2;

   h1 = *(BlastResultHspWithIdPtr PNTR) v1;
   h2 = *(BlastResultHspWithIdPtr PNTR) v2;

   if (h1->hitlist_index < h2->hitlist_index)
      return -1;
   else if (h1->hitlist_index > h2->hitlist_index)
      return 1;
   else if (h1->hsp_index < h2->hsp_index)
      return -1;
   else if (h1->hsp_index > h2->hsp_index)
      return 1;
   else /* Should never happen */
      return 0;
}
#endif

#define CLUSTER_LENGTH_THRESH 0.1
#define CLUSTER_OVERLAP_THRESH 0.9
#define CLUSTER_SCORE_THRESH 1.6

SeqAlignPtr LIBCALL
BioseqBlastEngineCore(BlastSearchBlkPtr search, BLAST_OptionsBlkPtr options,
                        Int4Ptr *pos_matrix)
{
	Int4 hitlist_max;
	SeqAlignPtr head, seqalign;
#ifdef BLAST_CLUSTER_HITS
        BLASTResultHspPtr hsp, hsp1;
        BlastResultHspWithIdPtr PNTR hspp;
        BLASTResultsStructPtr result_struct;
        BLASTResultHitlistPtr   result_hitlist;
        Int4 hspcnt, index, index1, index2;
        Int4 q_overlap;
        BioseqPtr bsp1, bsp2, PNTR bspp;
        BlastSearchBlkPtr search1;
        BLAST_KarlinBlkPtr kbp;
        FloatHi bit_score;
#endif

	head = seqalign = NULL;

	if (search == NULL || search->query_invalid)
		return NULL;


	/* If pos_matrix is not NULL, then psi-blast iterations are being 
	performed.  The first psi-blast iteration should be with normal
	blast. */
	if (pos_matrix)
	{
		search->sbp->posMatrix = pos_matrix;
		search->positionBased = TRUE;
                search->sbp->kbp = search->sbp->kbp_psi;
                search->sbp->kbp_gap = search->sbp->kbp_gap_psi;
		hitlist_max = search->result_struct->hitlist_max;
                search->result_struct = BLASTResultsStructDelete(search->result_struct);
		search->result_struct = BLASTResultsStructNew(hitlist_max, search->pbp->max_pieces, search->pbp->hsp_range_max);
                if (search->allocated & BLAST_SEARCH_ALLOC_WFP_FIRST)
		{
                       search->wfp_first = BLAST_WordFinderDestruct(search->wfp_first);
		       search->wfp_first = BLAST_WordFinderNew(search->sbp->alphabet_size,options->wordsize,1, FALSE);
		}

		if (search->allocated & BLAST_SEARCH_ALLOC_WFP_SECOND)
		{
		       search->wfp_second = BLAST_WordFinderDestruct(search->wfp_second);
		       search->wfp_second = BLAST_WordFinderNew(search->sbp->alphabet_size,options->wordsize,1, FALSE);
		}

		/* Only find words once if thresholds are the same. */
                 search->wfp = search->wfp_first;
		 if (search->whole_query == TRUE)
                 	BlastNewFindWords(search, 0, search->context[search->first_context].query->length, search->pbp->threshold_second, (Uint1) 0);
		 else
                 	BlastNewFindWords(search, search->required_start, search->required_end, search->pbp->threshold_second, (Uint1) 0);
                 lookup_position_aux_destruct(search->wfp->lookup);
                 search->wfp_second = search->wfp_first;
	}

	/* Starting awake thread if multithreaded. */
	if (search->searchsp_eff > AWAKE_THR_MIN_SIZE)
		BlastStartAwakeThread(search->thr_info);

        /* THE BLAST SEARCH IS HERE */
	do_the_blast_run(search);

#ifdef BLAST_CLUSTER_HITS        
        if (!search->pbp->mb_params) {
	/* Cluster hits by region within the query */
	/* Assume that hits are already sorted in each hitlist by score */
           ValNodePtr mask;
           result_struct = search->result_struct;
           hspcnt = 0;
           /* Collect all HSPs in one array */
           
           bspp = (BioseqPtr PNTR) Malloc(result_struct->hitlist_count*
                                          sizeof(BioseqPtr));
           for (index=0; index<result_struct->hitlist_count; index++) {
              hspcnt += result_struct->results[index]->hspcnt;
              bspp[index] = readdb_get_bioseq(search->rdfp, 
                                              result_struct->results[index]->subject_id);
           }
           
           hspp = (BlastResultHspWithIdPtr PNTR)
              Malloc(hspcnt*sizeof(BlastResultHspWithIdPtr)); 
           index2 = 0;
           for (index=0; index<result_struct->hitlist_count; index++) {
              result_hitlist = result_struct->results[index];
              for (index1=0; index1<result_hitlist->hspcnt; index1++) {
                 hspp[index2] = (BlastResultHspWithIdPtr) 
                    Malloc(sizeof(BlastResultHspWithId));
                 hspp[index2]->hitlist_index = index;
                 hspp[index2]->hsp_index = index1;
                 hspp[index2++]->hsp = &(result_hitlist->hsp_array[index1]);
              }
           }
           /* Sort by score */
           HeapSort((VoidPtr)hspp, hspcnt, sizeof(BLASTResultHspPtr), 
                    BLASTResultHspScoreCmp);
           index = 0;
           while (index<hspcnt) {
              hsp = hspp[index]->hsp;
              index2 = 0;
              
              result_hitlist = 
                 search->result_struct->results[hspp[index]->hitlist_index];
              bsp1 = bspp[hspp[index]->hitlist_index];
              
              search1 = 
                 BlastQuerySequenceSetUp(bsp1, search->prog_name, 
                                         options);
              for (index1=index+1; index1<hspcnt; index1++) {
                 /* Check if the next hit passes a simple test to be a
                    candidate to belong to this cluster */
                 if (hspp[index1]->hsp==NULL)
                    continue;
                 hsp1 = hspp[index1]->hsp;
                 result_hitlist = 
                    search->result_struct->results[hspp[index1]->hitlist_index];
                 bsp2 = bspp[hspp[index1]->hitlist_index];
                 if (((FloatHi)ABS(bsp1->length - bsp2->length)) / 
                     MIN(bsp1->length, bsp2->length) > CLUSTER_LENGTH_THRESH)
                    continue;
                 q_overlap = 
                    MIN(hsp->query_offset+hsp->query_length, 
                        hsp1->query_offset+hsp1->query_length) - 
                    MAX(hsp->query_offset, hsp1->query_offset);
                 if (((FloatHi)q_overlap) / 
                     MAX(hsp->query_length, hsp1->query_length) <
                     CLUSTER_OVERLAP_THRESH)
                    continue;
                 
                 /* We have a candidate for attaching to the cluster */
                 if (hspp[index]->hitlist_index == hspp[index1]->hitlist_index) {
                    /* Almost identical hit from same subject in the same 
                       area of the query - remove! */
                    result_hitlist = 
                       search->result_struct->results[hspp[index1]->hitlist_index];
                    hspp[index1]->hsp = NULL;
                 }
                 
                 /* Do the two sequences search to determine whether this 
                    candidate in fact belongs to this cluster */
                 search1 = BlastSequencesOnTheFlyEx(search1, bsp2); 
                 
                 if (search1 && search1->result_struct->results[0]) {
                    if (search1->pbp->gapped_calculation)
                       kbp = search1->sbp->kbp_gap[search1->first_context];
                    else
                       kbp = search1->sbp->kbp[search1->first_context]; 
                    bit_score = ((search1->result_struct->results[0]->high_score *
                                  kbp->Lambda) - kbp->logK)/NCBIMATH_LN2;
                    if (bit_score > CLUSTER_SCORE_THRESH * 
                        MAX(bsp1->length, bsp2->length)) {
                       /* remove the respective hit */
                       hspp[index1]->hsp = NULL;
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
              for (++index; index<hspcnt && hspp[index]->hsp==NULL; index++);
           }
           
           for (index=0; index<result_struct->hitlist_count; index++)
              BioseqFree(bspp[index]);
           MemFree(bspp);
           /* Remove all NULLs from hspp array */
           for (index=0, index1=0; index<hspcnt; index++) {
              if (hspp[index]->hsp != NULL) {
                 if (index != index1)
                    hspp[index1] = hspp[index];
                 index1++;
              } else
                 hspp[index] = MemFree(hspp[index]);
           }
           hspcnt = index1;
           /* Sort according to original hitlist and hsp indices */
           HeapSort((VoidPtr)hspp, hspcnt, sizeof(BLASTResultHspPtr), 
                    ResultHspWithIdIndexCmp);
           
           /* Rearrange the hsp_arrays for all hitlists */
           index = 0;
           for (index2=0; index2<result_struct->hitlist_count; index2++) {
              index1 = 0;
              while (index<hspcnt && hspp[index]->hitlist_index == index2) {
                 result_struct->results[index2]->hsp_array[index1] = 
                    *(hspp[index]->hsp);
                 index++;
                 index1++;
              }
              result_struct->results[index2]->hspcnt = index1;
           }
           
           for (index=0; index<hspcnt; index++) 
              hspp[index] = MemFree(hspp[index]);
           hspp = MemFree(hspp);
	}
#endif  /* Clustering hits */

    if (options->no_traceback) {
       BlastStopAwakeThread(search->thr_info);
       return NULL;
    }

    BLASTPostSearchLogic(search, options, &head, TRUE);

	/* Stop the awake thread. */
	BlastStopAwakeThread(search->thr_info);

	return head;
}

/*
	Deallocates all memory involved with the BlastHitRangePtr.
*/

BlastHitRangePtr LIBCALL
BlastHitRangeDestruct(BlastHitRangePtr old)

{
	if (old == NULL)
		return NULL;

	MemFree(old->range_list);
	MemFree(old->range_list_pointer);

	return MemFree(old);
}

/*
	Allocates a a BlastHitRangePtr, with two 'total' 
	BlastDoubleInt4Ptr's.
*/

BlastHitRangePtr LIBCALL
BlastHitRangeNew(Int4 total)

{
	BlastHitRangePtr bhrp;
	Int4 index;

	bhrp = MemNew(sizeof(BlastHitRange));

	bhrp->range_list = (BlastDoubleInt4Ptr) MemNew(total*sizeof(BlastDoubleInt4));
	bhrp->range_list_pointer = (BlastDoubleInt4Ptr PNTR) MemNew(total*sizeof(BlastDoubleInt4Ptr));
	for (index=0; index<total; index++)
	{
		bhrp->range_list_pointer[index] = &(bhrp->range_list[index]);
	}

	bhrp->current = 0;
	bhrp->total = total;

	return bhrp;
}

static int LIBCALLBACK
bhrp_compare(VoidPtr v1, VoidPtr v2)

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

BlastHitRangePtr LIBCALL
BioseqHitRangeEngineCore(BlastSearchBlkPtr search, BLAST_OptionsBlkPtr options)

{
	BlastHitRangePtr bhrp=NULL;
	BLASTResultsStructPtr result_struct;
	Int4 hitlist_count, index, total_hsps;
	Int4 sequence_length, length;
	Uint1Ptr sequence;

	if (search == NULL || search->query_invalid)
		return NULL;

	/* Starting awake thread if multithreaded. */
	if (search->searchsp_eff > AWAKE_THR_MIN_SIZE)
		BlastStartAwakeThread(search->thr_info);

	do_the_blast_run(search);

	if (search->prog_number==blast_type_blastn) {
	   /* Unconcatenate the strands by adjusting the query offsets in
	      all hsps */
	   search->context[search->first_context].query->length = 
	      search->query_context_offsets[search->first_context+1] - 1;
	   /*BlastAdjustHitOffsets(search);*/
	}
	
	if (StringCmp(search->prog_name, "blastn") == 0 && 
		search->pbp->gapped_calculation)
        {
		search->pbp->gap_open = options->gap_open;
		search->pbp->gap_extend = options->gap_extend;
/*
		search->pbp->gap_x_dropoff = (BLAST_Score) (options->gap_x_dropoff*NCBIMATH_LN2 / search->sbp->kbp_gap[search->first_context]->Lambda);
		search->pbp->gap_x_dropoff_final = (BLAST_Score) (options->gap_x_dropoff_final*NCBIMATH_LN2 / search->sbp->kbp_gap[search->first_context]->Lambda);
*/


		result_struct = search->result_struct;
                hitlist_count = result_struct->hitlist_count;
		total_hsps = 0;
		for (index=0; index<hitlist_count; index++)
		{
			total_hsps += result_struct->results[index]->hspcnt;
		}
		bhrp = BlastHitRangeNew(total_hsps);
		bhrp->query_id = search->query_id;
		
		result_struct = search->result_struct;
       		hitlist_count = result_struct->hitlist_count;

		sequence=NULL;
		sequence_length=0;
		
		for (index=0; index<hitlist_count; index++)
		{
			length = readdb_get_sequence_ex(search->rdfp, result_struct->results[index]->subject_id, &sequence, &sequence_length, TRUE);
			SumBlastGetGappedAlignmentEx(search, index, FALSE, FALSE, sequence+1, length, FALSE, NULL, bhrp, 0);
		}
		sequence = MemFree(sequence);
	}
	else
	{
		return NULL;
	}

	HeapSort(bhrp->range_list_pointer, bhrp->current, sizeof(BlastHitRangePtr PNTR), bhrp_compare);

	/* Stop the awake thread. */
	BlastStopAwakeThread(search->thr_info);

	return bhrp;
}

SeqAlignPtr LIBCALL
BioseqBlastEngineEx(BioseqPtr bsp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total)

{
	SeqLocPtr slp;
	SeqAlignPtr seqalign;

	slp = NULL;
	ValNodeAddPointer(&slp, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBest(bsp->id, SEQID_GI)));
	seqalign = BioseqBlastEngineByLocEx(slp, progname, database, options, other_returns, error_returns, callback, seqid_list, gi_list, gi_list_total);
	SeqLocFree(slp);
	
	return seqalign;
}

SeqAlignPtr LIBCALL
BioseqBlastEngine(BioseqPtr bsp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)))
{
   /* --KM added NULL mult_queries param to call */ 
   return BioseqBlastEngineWithCallbackMult(bsp, progname, database, options, other_returns, error_returns, callback, NULL, NULL);
}

SeqAlignPtr LIBCALL 
BioseqBlastEngineWithCallback(BioseqPtr bsp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), int (LIBCALLBACK *handle_results)PROTO((VoidPtr srch)))
{
   return BioseqBlastEngineWithCallbackMult(bsp, progname, database, options, other_returns, error_returns, callback, NULL, NULL);
}

/* --KM added mult_queries parameter */
SeqAlignPtr LIBCALL 
BioseqBlastEngineWithCallbackMult(BioseqPtr bsp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), int (LIBCALLBACK *handle_results)PROTO((VoidPtr srch)), QueriesPtr mult_queries)
{
	SeqLocPtr slp;
	SeqAlignPtr seqalign;

	slp = NULL;
	ValNodeAddPointer(&slp, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBest(bsp->id, SEQID_GI)));
	seqalign = BioseqBlastEngineByLocWithCallbackMult(slp, progname, database, options, other_returns, error_returns, callback, NULL, NULL, 0, handle_results, mult_queries);/* --KM pass mult_queries */
	SeqLocFree(slp);
	
	return seqalign;
}



SeqAlignPtr LIBCALL
BioseqBlastEngineByLoc(SeqLocPtr slp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)))

{
	return BioseqBlastEngineByLocEx(slp, progname, database, options, other_returns, error_returns, callback, NULL, NULL, 0);

}

SeqAlignPtr LIBCALL
BioseqBlastEngineByLocEx(SeqLocPtr slp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total)

{
   return BioseqBlastEngineByLocWithCallback(slp, progname, database, options, other_returns, error_returns, callback, seqid_list, gi_list, gi_list_total, NULL); /* --KM pass NULL mult_queries */
}

SeqAlignPtr LIBCALL
BioseqBlastEngineByLocWithCallback(SeqLocPtr slp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total, int (LIBCALLBACK *handle_results)PROTO((VoidPtr srch)))
{
	return BioseqBlastEngineByLocWithCallbackMult(slp, progname, database, options, other_returns, error_returns, callback, seqid_list, gi_list, gi_list_total, handle_results, NULL);
}

/* --KM added mult_queries param */
SeqAlignPtr LIBCALL
BioseqBlastEngineByLocWithCallbackMult(SeqLocPtr slp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total, int (LIBCALLBACK *handle_results)PROTO((VoidPtr srch)), QueriesPtr mult_queries)
{
	Boolean options_allocated=FALSE;
	BlastSearchBlkPtr search;
	Int2 status;
	SeqAlignPtr head;
	SeqLocPtr whole_slp=NULL;
		/* Futamura */
        posSearchItems *posSearch;
        compactSearchItems *compactSearch = NULL;
        Boolean  checkReturn = FALSE;

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
		options = BLASTOptionNew(progname, FALSE);
		options_allocated = TRUE;
	}

	status = BLASTOptionValidateEx(options, progname, error_returns);
	if (status != 0)
	{	/* error messages in other_returns? */
		return NULL;
	}

	if (slp == NULL || database == NULL)
		return NULL;

    if(options->is_rps_blast) {
        RPSInfoPtr rpsinfo;
        BioseqPtr bsp, fake_bsp;
        Boolean query_is_na;
        
        if((bsp = BioseqLockById(SeqLocId(slp))) == NULL)
            return NULL;
        
        /* RPS Blast discard program name and use specific RPS Blast
           logic for this */    
        
        if(bsp->mol == Seq_mol_aa) {
            query_is_na = FALSE;
            progname = "blastp";
        } else {
            query_is_na = TRUE;
            progname = "tblastn";
        }
        if((rpsinfo = RPSInitEx(database, !query_is_na, options)) == NULL) {
        
            ErrPostEx(SEV_ERROR, 0, 0, "Failure to initialize RPS: %s %s",
                      progname, database);
            return NULL;
        }
        /* Update size of the database in accordance with RPS Database size */
        RPSUpdateDbSize(options, rpsinfo, bsp->length);
        
        if(!query_is_na)
            fake_bsp = bsp;
        else {
            options->db_genetic_code = options->genetic_code;
            fake_bsp = createFakeProtein();
        }
        search = BLASTSetUpSearch (fake_bsp, progname, fake_bsp->length, 0, 
                                   NULL, options, NULL);
        
        if (search == NULL)
            return NULL;
        
        search->thr_info->tick_callback = NULL;
        search->thr_info->star_callback = NULL;
        
        head = RPSBlastSearch(search, bsp, rpsinfo);
        
        if(query_is_na)
            BioseqFree(fake_bsp);
        BioseqUnlock(bsp);
        RPSClose(rpsinfo);
    } else {
        
        search = BLASTSetUpSearchByLocWithReadDbEx(slp, progname, SeqLocLen(slp), database, options, NULL, seqid_list, gi_list, gi_list_total, mult_queries);
	/* --KM pass mult_queries */
        
        if (search == NULL) {
           /* We need to veryfy if database name is wrong and to set error
               returns correctly */
            Boolean is_prot;
            BlastErrorMsgPtr error_msg;
            CharPtr chptr;
            ReadDBFILEPtr rdfp=NULL;

            if(!StringICmp(progname, "blastp") ||
               !StringICmp(progname, "blastx")) {
                is_prot = TRUE;
            } else {
                is_prot = FALSE;
            }

            rdfp = readdb_new(database, is_prot);
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
        search->handle_results = handle_results;
        search->output = options->output;

        /* Futamura psitblastn */
        if (options->recoverCheckpoint)
          search->positionBased = TRUE;
        else
          search->positionBased = FALSE;

        if (options->recoverCheckpoint) {
          posSearch = (posSearchItems *) MemNew(1 * sizeof(posSearchItems));
          compactSearch = compactSearchNew(compactSearch);
          copySearchItems(compactSearch, search, options->matrix);
          posInitializeInformation(posSearch,search);
          /*AAS*/

          checkReturn = posReadCheckpoint(posSearch, compactSearch,
                                          options->CheckpointFileName,
                                          NO_SCOREMAT_IO,
                                          &(search->error_return));
          search->sbp->posMatrix = posSearch->posMatrix;
          if (NULL == search->sbp->posFreqs)
            search->sbp->posFreqs =  allocatePosFreqs(compactSearch->qlength,
                                                      compactSearch->alphabetSize);
          copyPosFreqs(posSearch->posFreqs,search->sbp->posFreqs,
                       compactSearch->qlength, compactSearch->alphabetSize);

          if (!checkReturn) {
		BlastConstructErrorMessage("BioseqBlastEngineByLocEx",
			"Error recovering from checkpoint", 3, error_returns);
		return NULL;
          }
        }

        /* ----- Here is real BLAST search done ------- */
        if (search->positionBased)
          head = BioseqBlastEngineCore(search, options, search->sbp->posMatrix);
        else if (options->is_megablast_search) {
           SeqAlignPtr PNTR seqalignp;
           seqalignp = BioseqMegaBlastEngineCore(search, options);
           head = *seqalignp;
        } else
	  head = BioseqBlastEngineCore(search, options, NULL);
	/* end Futamura */
	
    }

    if (search->error_return) {
        ValNodeLink(error_returns, search->error_return);
        search->error_return = NULL;
    }
    
    if (other_returns) { /* format dbinfo etc.  */
        *other_returns = BlastOtherReturnsPrepare(search);
    }
    
    if (options_allocated) {
        options = BLASTOptionDelete(options);
    }
    
    search = BlastSearchBlkDestruct(search);

    if(!options->is_rps_blast) {
    
        /* Adjsut the offset if the query does not cover the entire sequence. */
        if (slp->choice != SEQLOC_WHOLE) {
            ValNodeAddPointer(&whole_slp, SEQLOC_WHOLE, SeqIdFindBest(SeqLocId(slp), SEQID_GI));
            if (SeqLocAinB(whole_slp, slp) != 0) {
                AdjustOffSetsInSeqAlign(head, slp, NULL);
            }
            ValNodeFree(whole_slp);
        }
    }

    return head;
}

SeqLocPtr LIBCALL
BioseqHitRangeEngine(BioseqPtr bsp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total)

{
	SeqLocPtr slp;

	slp = NULL;
	ValNodeAddPointer(&slp, SEQLOC_WHOLE, SeqIdDup(SeqIdFindBest(bsp->id, SEQID_GI)));
	return BioseqHitRangeEngineByLoc(slp, progname, database, options, other_returns, error_returns, callback, seqid_list, gi_list, gi_list_total);
}

SeqLocPtr 
HitRangeToSeqLoc(BlastHitRangePtr bhrp, Int4 link_value, Boolean combine)

{
	Boolean make_seqloc, start=TRUE;
	Int4 index, total, start_pos=0, stop_pos, largest_stop_pos=0;
	SeqIntPtr sint;
	SeqLocPtr retval=NULL;

	if (bhrp == NULL)
		return NULL;

	total = bhrp->current;
	index=0;
	while (index < total)
	{
	   if (combine)
	   {
		if (start == TRUE)
		{
			start_pos = bhrp->range_list_pointer[index]->gi + bhrp->base_offset;
			start = FALSE;
			largest_stop_pos = 0;
		}
		else
		{
			/* Keep track of largest stop position. */
			largest_stop_pos = MAX(largest_stop_pos, bhrp->range_list_pointer[index]->ordinal_id + bhrp->base_offset);
			make_seqloc = FALSE;
			if (index == total-1)	/* Last one. */
			{
				stop_pos = bhrp->range_list_pointer[index]->ordinal_id + bhrp->base_offset;
				start = TRUE;
				make_seqloc = TRUE;
			}
			else if (largest_stop_pos+link_value < bhrp->range_list_pointer[index+1]->gi + bhrp->base_offset)
			{ /* Check overlap with next one. */
				stop_pos = bhrp->range_list_pointer[index]->ordinal_id + bhrp->base_offset;
				start = TRUE;
				make_seqloc = TRUE;
			}
			
			if (make_seqloc)
			{
				sint = SeqIntNew();
				sint->from = start_pos;
				sint->to = MAX(largest_stop_pos, stop_pos);
				sint->strand = Seq_strand_plus;
				sint->id = SeqIdDup(SeqIdFindBest(bhrp->query_id, SEQID_GI));
				ValNodeAddPointer(&retval, SEQLOC_INT, sint);
			}
			index++;
		}
	   }
	   else
	   {
		sint = SeqIntNew();
		sint->from = bhrp->range_list_pointer[index]->gi + bhrp->base_offset;
		sint->to = bhrp->range_list_pointer[index]->ordinal_id + bhrp->base_offset;
		sint->strand = Seq_strand_plus;
		sint->id = SeqIdDup(SeqIdFindBest(bhrp->query_id, SEQID_GI));
		ValNodeAddPointer(&retval, SEQLOC_INT, sint);
		index++;
	   }
	}

	return retval;
}

#define HITRANGE_LINKVALUE 5

SeqLocPtr LIBCALL
BioseqHitRangeEngineByLoc(SeqLocPtr slp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total)

{
	Boolean options_allocated=FALSE;
	BlastHitRangePtr bhrp;
	BlastSearchBlkPtr search;
	Int2 status;
	SeqLocPtr seqloc, whole_slp=NULL;

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
		options = BLASTOptionNew(progname, FALSE);
		options_allocated = TRUE;
	}

	status = BLASTOptionValidateEx(options, progname, error_returns);
	if (status != 0)
	{	/* error messages in other_returns? */
		return NULL;
	}

	if (slp == NULL || database == NULL)
		return NULL;

	search = BLASTSetUpSearchByLocWithReadDbEx(slp, progname, SeqLocLen(slp), database, options, NULL, seqid_list, gi_list, gi_list_total, NULL); /* --KM pass NULL mult_queries */

	if (search == NULL)
	{
		return NULL;
	}

	search->thr_info->tick_callback = callback;
	search->thr_info->star_callback = callback;

	bhrp = BioseqHitRangeEngineCore(search, options);
	if (bhrp == NULL) /* can happen for invalid queries. */
		return NULL;

        if (slp->choice != SEQLOC_WHOLE) {
        	ValNodeAddPointer(&whole_slp, SEQLOC_WHOLE, SeqIdFindBest(SeqLocId(slp), SEQID_GI));
		bhrp->base_offset = GetOffsetInLoc(slp, whole_slp, SEQLOC_START);
		ValNodeFree(whole_slp);
	}
	
	seqloc = HitRangeToSeqLoc(bhrp, HITRANGE_LINKVALUE, TRUE);
	bhrp = BlastHitRangeDestruct(bhrp);
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

	return seqloc;
}

void LIBCALL BlastOtherReturnsFree(ValNodePtr other_returns)
{
    BLAST_KarlinBlkPtr ka_params;
    BLAST_MatrixPtr matrix;
    CharPtr params_buffer;
    TxDfDbInfoPtr dbinfo;
    ValNodePtr  mask_loc, mask_loc_start, vnp;
    
    mask_loc = NULL;
    
    for (vnp=other_returns; vnp; vnp = vnp->next) {
        switch (vnp->choice) {
        case TXDBINFO:
            dbinfo = vnp->data.ptrvalue;
            dbinfo = TxDfDbInfoDestruct(dbinfo);
            break;
        case TXKABLK_NOGAP:
            ka_params = vnp->data.ptrvalue;
            MemFree(ka_params);
            break;
        case TXKABLK_GAP:
            ka_params = vnp->data.ptrvalue;
            MemFree(ka_params);
            break;
        case TXPARAMETERS:
            params_buffer = vnp->data.ptrvalue;
            MemFree(params_buffer);
            break;
        case TXMATRIX:
            matrix = vnp->data.ptrvalue;
            matrix = BLAST_MatrixDestruct(matrix);
            
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
    
    mask_loc_start = mask_loc;
    while (mask_loc) {
        SeqLocSetFree(mask_loc->data.ptrvalue);
        mask_loc = mask_loc->next;
    }
    ValNodeFree(mask_loc_start);

    other_returns = ValNodeFree(other_returns);

    return;
}

ValNodePtr LIBCALL
BlastOtherReturnsPrepare(BlastSearchBlkPtr search)

{
    BLAST_KarlinBlkPtr ka_params;
    BLAST_MatrixPtr blast_matrix;
    CharPtr parameters, chptr;
    ReadDBFILEPtr rdfp_var;
    TxDfDbInfoPtr dbinfo, head, dbinfo_var=NULL;
    ValNodePtr other_returns=NULL;
    
    head = NULL;
    if (search->thr_info->blast_gi_list) {
        dbinfo = MemNew(sizeof(TxDfDbInfo));
        dbinfo->total_length = search->dblen;
        dbinfo->number_seqs = search->dbseq_num;
        dbinfo->subset = TRUE;
        head = dbinfo;
        dbinfo_var = dbinfo;
    }
    
    rdfp_var = search->rdfp;
    while (rdfp_var) {
        dbinfo = MemNew(sizeof(TxDfDbInfo));
        dbinfo->name = StringSave(readdb_get_filename(rdfp_var));	

        if((chptr = readdb_get_title(rdfp_var)) == NULL)
            chptr = readdb_get_filename(rdfp_var);
        dbinfo->definition = StringSave(chptr);	
        
        dbinfo->date = StringSave(readdb_get_date(rdfp_var));	

        dbinfo->is_protein = readdb_is_prot(rdfp_var);

        if (rdfp_var->aliaslen)
            dbinfo->total_length = rdfp_var->aliaslen;
        else
            dbinfo->total_length = readdb_get_dblen(rdfp_var);
        if (rdfp_var->aliasnseq)
            dbinfo->number_seqs = rdfp_var->aliasnseq;
        else
            dbinfo->number_seqs = readdb_get_num_entries(rdfp_var);
        if (head == NULL) {
            head = dbinfo;
            dbinfo_var = dbinfo;
        } else {
            dbinfo_var->next = dbinfo;
            dbinfo_var = dbinfo_var->next;
        }
        rdfp_var = rdfp_var->next;
    }
    if (head)
        ValNodeAddPointer (&other_returns, TXDBINFO, head);
    
    if (search->sbp->kbp && search->sbp->kbp[search->first_context]) {
        ka_params = BlastKarlinBlkCreate();
        ka_params->Lambda = search->sbp->kbp[search->first_context]->Lambda;
        ka_params->K = search->sbp->kbp[search->first_context]->K;
        ka_params->H = search->sbp->kbp[search->first_context]->H;
        ValNodeAddPointer (&other_returns, TXKABLK_NOGAP, ka_params);
    }
    
    if (search->pbp->gapped_calculation == TRUE) {
        if (search->sbp->kbp_gap && search->sbp->kbp_gap[search->first_context]) {
                ka_params = BlastKarlinBlkCreate();
                ka_params->Lambda = search->sbp->kbp_gap[search->first_context]->Lambda;
                ka_params->K = search->sbp->kbp_gap[search->first_context]->K;
                ka_params->H = search->sbp->kbp_gap[search->first_context]->H;
                ValNodeAddPointer (&other_returns, TXKABLK_GAP, ka_params);
        }
    }
    
    if (search->query_invalid == FALSE) {
        parameters = FormatBlastParameters(search);
        ValNodeAddPointer (&other_returns, TXPARAMETERS, parameters);
    }
    
    blast_matrix = BLAST_MatrixFill(search->sbp, search->positionBased);
    ValNodeAddPointer (&other_returns, TXMATRIX, blast_matrix);
    
    if (search->mask)
        ValNodeLink(&other_returns, search->mask);
    
    if (search->pbp->is_rps_blast) {
        ValNodeAddFloat(&other_returns, EFF_SEARCH_SPACE,
            ((Nlm_FloatHi) search->dblen_eff)*
            ((Nlm_FloatHi) (search->rps_qlen - search->length_adjustment)));
    } else {
        ValNodeAddFloat(&other_returns, EFF_SEARCH_SPACE,
            ((Nlm_FloatHi) search->dblen_eff)*
            ((Nlm_FloatHi) search->context[search->first_context].query->effective_length));
    }
    ValNodeAddInt(&other_returns, EFF_HSP_LENGTH, search->length_adjustment);
 
    /* If Mega BLAST endpoint results, save them here */
    if (search->mb_endpoint_results && search->pbp->mb_params && 
        search->pbp->mb_params->no_traceback)
       /* Here 21 = BlastResponse_mbalign (see file objblst3.h) */
       ValNodeAddPointer(&other_returns, 21, 
                         search->mb_endpoint_results->data.ptrvalue);
       
    return other_returns;
}


/*
	Deallocates memory for BLAST_ExtendWordParamsPtr
	
*/

static BLAST_ExtendWordParamsPtr
BLAST_ExtendWordParamsDestruct (BLAST_ExtendWordParamsPtr ewp_params)

{
	ewp_params = MemFree(ewp_params);

	return ewp_params;
}


/*
	Allocates memory for the BLAST_ExtendWordParamsPtr.  

	This function also sets many of the parametes such as min_diag_length etc.

	Int4 qlen: length of the query.
	Boolean multiple_hits: specifies whether multiple hits method is used.
	Int4 window_size: the max. distance between two hits that are extended.
*/

BLAST_ExtendWordParamsPtr
BLAST_ExtendWordParamsNew (Int4 qlen, Boolean multiple_hits, Int4 window_size)

{
	BLAST_ExtendWordParamsPtr ewp_params;
	Int4 min_diag_length, bits_to_shift;

	ewp_params= MemNew(sizeof(BLAST_ExtendWordParams));

	if (ewp_params)
	{
		min_diag_length = 1;
		bits_to_shift = 0;
		/* What power of 2 is just longer than the query? */
		while (min_diag_length < (qlen+window_size))
		{
			min_diag_length = min_diag_length << 1;
			bits_to_shift++;
		}
		/* These are used in the word finders to shift and mask 
		rather than dividing and taking the remainder. */
		ewp_params->bits_to_shift = bits_to_shift;
		ewp_params->min_diag_length = min_diag_length;
		ewp_params->min_diag_mask = min_diag_length-1;
		ewp_params->multiple_hits = multiple_hits;
		ewp_params->offset = window_size;
                ewp_params->window = window_size;
	}
	return ewp_params;
}

/*
	Deallocates memory for the BLAST_ExtendWordPtr.

*/
BLAST_ExtendWordPtr LIBCALL 
BLAST_ExtendWordDestruct (BLAST_ExtendWordPtr ewp)

{
	if (ewp)
	{
		if (ewp->_buffer)
			ewp->_buffer = MemFree(ewp->_buffer);

		ewp = MemFree(ewp);
	}

	return ewp;

}

/*
	Allocates memory for the BLAST_ExtendWordPtr.  

	All of the memory for the arrays is allocated in one chunk
	called "_buffer".  If multiple_hits is specified them room
	for "diag_level", "last_hit", and "version" is allocated and
	pointers into the array for these are set.  If multiple_hits
	is not set, then only room for diag_level and version is allocated;
	last_hit is not needed.

	Int4 qlen, dblen: length of the query and the LONGEST subject sequence.
	Boolean multiple_hits: specifies whether multiple hits method is used.

	** CFJ
	** - previously buffer contained diag_level array, last_hit array, and version array
	**   change to contain array of struct {dl,lh,v}.
	**
	** - Now that version is no longer used, combining the remaining 2 is probably not a big win.

*/
BLAST_ExtendWordPtr
BLAST_ExtendWordNew (BLAST_ExtendWordParamsPtr ewp_params)

{
	BLAST_ExtendWordPtr ewp;
	int i;

	ewp = MemNew(sizeof(BLAST_ExtendWord));

	if (ewp)
	{
		/* Allocate the buffer to be used for Combo array. */
    	        ewp->_buffer = (Int4Ptr) MemNew(ewp_params->min_diag_length*sizeof(CfjModStruct));

		if (ewp->_buffer == NULL)
		{
			ewp = BLAST_ExtendWordDestruct(ewp);
			return NULL;
		}

		ewp->combo_array= (CfjModStruct *) ewp->_buffer;
		ewp_params->offset=0;
		for(i=0;i<ewp_params->min_diag_length;i++){
		  ewp->combo_array[i].diag_level=0;
		  ewp->combo_array[i].last_hit = -ewp_params->window;
		}
	}

	return ewp;
}

/*****************************************************************************
*
*	Zeroe's out the memory in the array _buffer, if offset is greater than
*	INT4_MAX/2.  The first "min_diag_length" spaces in the array are used 
*	by the array "diag_level", the second "min_diag_length" spaces are used 
*	by "last_hit".  All of these are zeroed out.  The last "min_diag_length" 
*	spaces are used by "version"; these are not zeroed out.
*
*	If offset is not greater than INT4_MAX/2, then the memory is not
*	zeroed out.  Rather "offset" is used as a "zero-point" that is
*	always greater than the next possible value when the word finder
*	starts working on a new subject sequence.
*
******************************************************************************/
void LIBCALL
BlastExtendWordExit(BlastSearchBlkPtr search)

{
	BLAST_ExtendWordPtr ewp;
	BLAST_ExtendWordParamsPtr ewp_params;
	Int2 index;
	Int4 i, min_diag_length;

	ewp_params = search->ewp_params;

	for (index=search->first_context; index<=search->last_context; index++)
	{

		if (ewp_params->offset >= INT4_MAX/2)
		{
			ewp = search->context[index].ewp;
                        if (ewp) {
                           min_diag_length = ewp_params->min_diag_length;
                           for(i=0;i<min_diag_length;i++)
			   {
			        ewp->combo_array[i].diag_level=0;
			        ewp->combo_array[i].last_hit = -ewp_params->window;
                           }
                        }
                }
	}

	if (ewp_params->offset < INT4_MAX/2)
	{
		ewp_params->offset += search->subject->length + ewp_params->window ;
	}
	else
	{
		ewp_params->offset = 0;
	}
}


BlastSequenceBlkPtr LIBCALL
BlastSequenceBlkDestruct(BlastSequenceBlkPtr seq_blk)

{

	if (seq_blk == NULL)
		return NULL;

	/* Free from the start of sequence if it's filled in. */
	if (seq_blk->sequence_start != NULL)
	{
		seq_blk->sequence_start = MemFree(seq_blk->sequence_start);
	}
	else
	{	
		seq_blk->sequence = MemFree(seq_blk->sequence);
	}

	seq_blk = MemFree(seq_blk);

	return seq_blk;
}



static BLASTContextStructPtr 
BLASTContextFree(BLASTContextStructPtr context, Int2 number)

{
	Int2 index;

	if (context == NULL)
	  return NULL;

	for (index=0; index<number; index++)
	{
		context[index].ewp = BLAST_ExtendWordDestruct(context[index].ewp);
		if (context[index].query_allocated == TRUE)
		{
			context[index].query = BlastSequenceBlkDestruct(context[index].query);
		}
	}
	context = MemFree(context);

	return context;
}

void BlastThrInfoFree(BlastThrInfoPtr thr_info)
{
    VoidPtr status=NULL;

    if (thr_info == NULL)
	return;

    if (thr_info->index_thr)
    {
		NlmThreadJoin(thr_info->index_thr, &status);
		thr_info->index_thr = NULL;
    }

    if (thr_info->awake_thr) 
    {
        NlmThreadJoin(thr_info->awake_thr, &status);
        thr_info->awake_thr = NULL;
        if (thr_info->callback_mutex)
        {
                NlmMutexDestroy(thr_info->callback_mutex);
                thr_info->callback_mutex = NULL;
        }
    }
    BlastGiListDestruct(thr_info->blast_gi_list, TRUE);
    
    NlmMutexDestroy(thr_info->db_mutex);
    NlmMutexDestroy(thr_info->results_mutex);
    NlmMutexDestroy(thr_info->callback_mutex);

    MemFree(thr_info);
    
    return;
}

BlastThrInfoPtr BlastThrInfoNew(void)
{
    BlastThrInfoPtr thr_info;
    
    thr_info = MemNew(sizeof(BlastThrInfo));
    
    return thr_info;
}


/* 
	Allocates space for a copy of the BlastSearchBlk for use in
	multi-processing BLAST.
*/

BlastSearchBlkPtr LIBCALL
BlastSearchBlkDuplicate (BlastSearchBlkPtr search)

{

	BlastSearchBlkPtr new_search;
	Int2 index;

	if (search == NULL)
		return NULL;

	new_search = (BlastSearchBlkPtr) MemNew(sizeof(BlastSearchBlk));
	if (new_search == NULL)
		return NULL;

	/* What's allocated here? */
	new_search->allocated = 0;	
	new_search->allocated += BLAST_SEARCH_ALLOC_SUBJECT;
	new_search->allocated += BLAST_SEARCH_ALLOC_PBP;
	new_search->allocated += BLAST_SEARCH_ALLOC_CONTEXT;
	new_search->allocated += BLAST_SEARCH_ALLOC_READDB;
	new_search->allocated += BLAST_SEARCH_ALLOC_EWPPARAMS;
		
        /* AM: Support for query multiplexing. */
	if( search->mult_queries )
	  new_search->mult_queries = BlastDuplicateMultQueries( search->mult_queries );
		
	/* Duplicate the rfdp struct, but not the contents. */
	new_search->rdfp = readdb_attach(search->rdfp);
	if (new_search->rdfp == NULL)
	{
		new_search = BlastSearchBlkDestruct(new_search);
		return NULL;
	}

	new_search->positionBased = search->positionBased;

	/* Changes, need to allocate. */
	new_search->pbp = MemDup(search->pbp, sizeof(BLAST_ParameterBlk));
	if (search->pbp->mb_params)
	  new_search->pbp->mb_params = 
	    MemDup(search->pbp->mb_params, sizeof(MegaBlastParameterBlk));
	new_search->pbp->filter_string = StringSave(search->pbp->filter_string);
	new_search->sbp = search->sbp;
	new_search->wfp_first = search->wfp_first;
	if (search->prog_number==blast_type_blastn && 
	    search->pbp->mb_params) {
	   new_search->wfp_second = 
	      MemDup(search->wfp_second, sizeof(BLAST_WordFinder));
	   new_search->wfp_second->lookup = 
	      MegaBlastLookupTableDup(search->wfp_second->lookup);
           new_search->wfp = new_search->wfp_second;
	} else
	   new_search->wfp_second = search->wfp_second;
	new_search->prog_name = StringSave(search->prog_name);
	new_search->prog_number = search->prog_number;
	new_search->first_context = search->first_context;
	new_search->last_context = search->last_context;
        new_search->query_slp = search->query_slp;
	if (search->prog_number==blast_type_blastn) {
	   new_search->query_context_offsets =
	      MemDup(search->query_context_offsets, 
		     (search->last_context-search->first_context+2)*sizeof(Int4));
	}
	if (search->ewp_params)
	   new_search->ewp_params = MemDup(search->ewp_params, sizeof(BLAST_ExtendWordParams));
	new_search->dblen = search->dblen;
	new_search->dblen_eff = search->dblen_eff;
	new_search->dblen_eff_real = search->dblen_eff_real;
	new_search->dbseq_num = search->dbseq_num;
	new_search->length_adjustment = search->length_adjustment;
	new_search->searchsp_eff = search->searchsp_eff;

	/* Allocate last_context+1 elements, even if there are only last_context-first_context
	being used. */
	new_search->context = (BLASTContextStructPtr) MemNew((search->last_context+1)*sizeof(BLASTContextStruct));
	for (index=new_search->first_context; index<=new_search->last_context; index++)
	{
	   if (new_search->ewp_params)
	      new_search->context[index].ewp = BLAST_ExtendWordNew(new_search->ewp_params);
		new_search->context[index].query = search->context[index].query;
		new_search->context[index].query->frame = ContextToFrame(new_search, index);
		new_search->context[index].query_allocated = FALSE;
	}

	new_search->context_factor = search->context_factor;

	new_search->subject = (BlastSequenceBlkPtr) MemNew(sizeof(BlastSequenceBlk));
	/* 100 is the size limit in the present BLAST for hsp's. */
	new_search->hsp_array_size = search->hsp_array_size;
	/* The results are held here. */
	new_search->result_struct = search->result_struct;
        new_search->mb_result_struct = search->mb_result_struct;
        new_search->result_size = search->result_size;
	new_search->worst_evalue = DBL_MAX;

	new_search->translation_table = search->translation_table;
	new_search->translation_table_rc = search->translation_table_rc;
	new_search->genetic_code = search->genetic_code;
	new_search->db_genetic_code = search->db_genetic_code;

	if (search->translation_buffer_size > 0)
	{	/* two extra for the NULLB's on end. */
		new_search->translation_buffer = MemNew((2+search->translation_buffer_size)*sizeof(Uint1));
		new_search->translation_buffer_size = search->translation_buffer_size;
	}

	new_search->gap_align = NULL;	/* Allocated automatically. */

	new_search->whole_query = search->whole_query;
	new_search->required_start = search->required_start;
	new_search->required_end = search->required_end;

	new_search->handle_results = search->handle_results;
	if (!search->pbp->mb_params) 
           new_search->query_id = SeqIdSetDup(search->query_id);
        else {
	   new_search->qid_array = (SeqIdPtr PNTR) 
	      Malloc((search->last_context/2 + 1)*sizeof(SeqIdPtr));
           
	   for (index=0; index<=search->last_context/2; index++)
	      new_search->qid_array[index] = SeqIdSetDup(search->qid_array[index]);
	}

        /* Duplicating DNAP sequence used in OOF search */
        if(search->pbp->is_ooframe)
            new_search->query_dnap = BlastMakeCopyQueryDNAP(search->query_dnap);

        new_search->thr_info = search->thr_info;
        new_search->queue_callback = search->queue_callback;
        new_search->semid = search->semid;
        
#ifdef BLAST_COLLECT_STATS
	new_search->first_pass_hits = 0;
	new_search->second_pass_hits = 0;
	new_search->second_pass_trys = 0;
	new_search->first_pass_extends = 0;
	new_search->second_pass_extends = 0;
	new_search->first_pass_good_extends = 0;
	new_search->second_pass_good_extends = 0;
	new_search->number_of_seqs_better_E = 0;
	new_search->prelim_gap_no_contest = 0;
	new_search->prelim_gap_passed = 0;
	new_search->prelim_gap_attempts = 0;
	new_search->real_gap_number_of_hsps = 0;
#endif
	new_search->output = search->output;

	if (search->abmp) {
	   new_search = GreedyAlignMemAlloc(new_search);
           if (new_search->abmp == NULL) {
              new_search = BlastSearchBlkDestruct(new_search);
              return NULL;
           }
        }
        if (search->mb_endpoint_results) {
           new_search->mb_endpoint_results = ValNodeNew(NULL);
           new_search->mb_endpoint_results->data.ptrvalue = 
              search->mb_endpoint_results->data.ptrvalue;
        }
	new_search->mask1 = search->mask1;

	return new_search;
}
/* 
	Allocates space for the new BlastSearchBlk and some sturctures
	attached to it.
*/

BlastSearchBlkPtr LIBCALL
BlastSearchBlkNew (Int2 wordsize, Int4 qlen, CharPtr dbname, Boolean multiple_hits, BLAST_Score threshold_first, BLAST_Score threshold_second, Int4 result_size, CharPtr prog_name, BlastAllWordPtr all_words, Int2 first_context, Int2 last_context, Int4 window_size)

{
	return BlastSearchBlkNewExtra(wordsize, qlen, dbname, multiple_hits, threshold_first, threshold_second, result_size, prog_name, all_words, first_context, last_context, NULL, window_size);

}

/* 
	Allocates space for the new BlastSearchBlk and some sturctures
	attached to it.
*/

BlastSearchBlkPtr LIBCALL
BlastSearchBlkNewExtra (Int2 wordsize, Int4 qlen, CharPtr dbname, Boolean multiple_hits, BLAST_Score threshold_first, BLAST_Score threshold_second, Int4 result_size, CharPtr prog_name, BlastAllWordPtr all_words, Int2 first_context, Int2 last_context, ReadDBFILEPtr rdfp, Int4 window_size)

{

	BlastSearchBlkPtr search;
	BLASTContextStructPtr context;
	Uint1 is_prot;
	Int2 index;
	Uint1 alphabet;
	Int4 longest_db_seq=INT4_MAX;
	ReadDBFILEPtr rdfp_var;
        Int4 last_ewp_index;

	search = (BlastSearchBlkPtr) MemNew(sizeof(BlastSearchBlk));

	if (search != NULL)
	{
		search->allocated = 0;	/* everything's allocated here. */
		search->allocated += BLAST_SEARCH_ALLOC_QUERY;
		search->allocated += BLAST_SEARCH_ALLOC_SUBJECT;
		search->allocated += BLAST_SEARCH_ALLOC_PBP;
		search->allocated += BLAST_SEARCH_ALLOC_SBP;
		search->allocated += BLAST_SEARCH_ALLOC_EWPPARAMS;
		search->allocated += BLAST_SEARCH_ALLOC_CONTEXT;
		search->allocated += BLAST_SEARCH_ALLOC_RESULTS;
		search->allocated += BLAST_SEARCH_ALLOC_READDB;
		search->allocated += BLAST_SEARCH_ALLOC_ALL_WORDS;
                search->allocated += BLAST_SEARCH_ALLOC_THRINFO;
                search->allocated += BLAST_SEARCH_ALLOC_MASK1;
		
		search->positionBased = FALSE;

		if (StringCmp(prog_name, "blastn") == 0)
		{
			alphabet = BLASTNA_SEQ_CODE;
		}
		else
		{
			alphabet = Seq_code_ncbistdaa;
		}

		if (dbname != NULL)
		{
			
			if (rdfp == NULL)
			{
				if (StringCmp(prog_name, "blastp") == 0 || StringCmp(prog_name, "blastx") == 0)
				{ /* Protein DB for blastp and blastx. */
					is_prot = READDB_DB_IS_PROT;
				}
				else
				{
					is_prot = READDB_DB_IS_NUC;
				}
			
				if ((search->rdfp=readdb_new(dbname, is_prot)) == NULL)
				{
					return NULL;
				}
			}
			else
			{	/* Attaches to the rdfp, rather than reallocating it. */
				search->rdfp = readdb_attach(rdfp);
			}

			rdfp_var = search->rdfp;
			longest_db_seq = 0;
			while (rdfp_var)
			{
				longest_db_seq = MAX(longest_db_seq, readdb_get_maxlen(rdfp_var));
				rdfp_var = rdfp_var->next;
			}
		}

		search->first_context = first_context;
		search->last_context = last_context;

		search->pbp = 
		   (BLAST_ParameterBlkPtr) MemNew(sizeof(BLAST_ParameterBlk));

		search->sbp = BLAST_ScoreBlkNew(alphabet, last_context+1);

		/* Only allocate these if thresholds are above zero, i.e. they will be used. */
		if (StringCmp(prog_name, "blastn") != 0)
		{
			if (threshold_second > 0)
			{
				search->wfp_first = BLAST_WordFinderNew(search->sbp->alphabet_size, wordsize, 1, FALSE);
				search->allocated += BLAST_SEARCH_ALLOC_WFP_FIRST;
		/* Only allocate a new WFP if 2nd th differs from 1st. */
                                search->wfp_second = search->wfp_first;
			}
		}
		else
		{
			if (multiple_hits)
				search->wfp_second = BLAST_WordFinderNew(256, wordsize, READDB_COMPRESSION_RATIO, FALSE);
			else
				search->wfp_second = BLAST_WordFinderNew(256, wordsize, READDB_COMPRESSION_RATIO, TRUE);
			search->allocated += BLAST_SEARCH_ALLOC_WFP_SECOND;
		}

		search->prog_name = StringSave(prog_name);
		search->prog_number = BlastGetProgramNumber(prog_name);
		if (qlen > 0)
		   search->ewp_params = BLAST_ExtendWordParamsNew(qlen, multiple_hits, window_size);
		else
		   search->ewp_params = NULL;
		context = search->context = (BLASTContextStructPtr)
                   MemNew((1+search->last_context)*sizeof(BLASTContextStruct));
                if (search->prog_number != blast_type_blastn)
                   last_ewp_index = search->last_context;
                else /* All queries (Mega BLAST) and strands are concatenated
                        in a single sequence */
                   last_ewp_index = search->first_context;

		for (index=search->first_context; index<=search->last_context; index++)
		{
		   if (search->ewp_params && index <= last_ewp_index)
                      context[index].ewp = BLAST_ExtendWordNew(search->ewp_params);
                   context[index].query = (BlastSequenceBlkPtr) MemNew(sizeof(BlastSequenceBlk));
                   context[index].query->frame = ContextToFrame(search, index);
                   context[index].query_allocated = TRUE;
		}

		search->subject = (BlastSequenceBlkPtr) MemNew(sizeof(BlastSequenceBlk));
		/* 100 is the size limit in the present BLAST for hsp's. */
		search->hsp_array_size = 100;
		/* The results are held here. */
		search->result_size = result_size;
/*
		search->result_struct = BLASTResultsStructNew(result_size, search->pbp->max_pieces, search->pbp->hsp_range_max);
*/

		search->worst_evalue = DBL_MAX;

		search->whole_query = TRUE;
		search->required_start = 0;
		search->required_end = -1;

		search->all_words = all_words;

                search->thr_info = BlastThrInfoNew();
#ifdef BLAST_COLLECT_STATS
		search->first_pass_hits = 0;
		search->second_pass_hits = 0;
		search->second_pass_trys = 0;
		search->first_pass_extends = 0;
		search->second_pass_extends = 0;
		search->first_pass_good_extends = 0;
		search->second_pass_good_extends = 0;
		search->number_of_seqs_better_E = 0;
		search->prelim_gap_no_contest = 0;
		search->prelim_gap_passed = 0;
		search->prelim_gap_attempts = 0;
		search->real_gap_number_of_hsps = 0;
#endif
	}

	return search;
}

/*
	Deallocates memory associated with the BlastSearchBlkPtr.
*/

BlastSearchBlkPtr LIBCALL 
BlastSearchBlkDestruct (BlastSearchBlkPtr search)

{

    if (search != NULL) {
        if (search->allocated & BLAST_SEARCH_ALLOC_QUERY)
            search->original_seq = MemFree(search->original_seq);
        
        if (search->allocated & BLAST_SEARCH_ALLOC_SUBJECT)
            search->subject = BlastSequenceBlkDestruct(search->subject);
        
        if (search->allocated & BLAST_SEARCH_ALLOC_SBP)
            search->sbp = BLAST_ScoreBlkDestruct(search->sbp);
        
        if (search->allocated & BLAST_SEARCH_ALLOC_WFP_FIRST)
            search->wfp_first = BLAST_WordFinderDestruct(search->wfp_first);
        
        if (search->allocated & BLAST_SEARCH_ALLOC_WFP_SECOND) {
            search->wfp_second = BLAST_WordFinderDestruct(search->wfp_second);
        } else if (search->prog_number==blast_type_blastn && 
                   search->pbp->mb_params) {
            search->wfp_second = 
                MegaBlastWordFinderDeallocate(search->wfp_second);
        }  
        
        /* Freeing DNAP sequence used in OOF */
        
        if(search->pbp != NULL && search->pbp->is_ooframe) {
            BlastFreeQueryDNAP(search->query_dnap);
            search->query_dnap = NULL;
        }
        
        if (search->allocated & BLAST_SEARCH_ALLOC_EWPPARAMS) {
            search->ewp_params = BLAST_ExtendWordParamsDestruct(search->ewp_params);
        }
        
        if (search->allocated & BLAST_SEARCH_ALLOC_CONTEXT) {
            search->context = BLASTContextFree(search->context, 1+search->last_context);
        }
        
        if (search->allocated & BLAST_SEARCH_ALLOC_RESULTS) {
           if (!search->pbp->mb_params)
              search->result_struct =
                 BLASTResultsStructDelete(search->result_struct);
           else {
              Int2 index;
              for (index=0; index<=search->last_context/2; index++)
                 search->mb_result_struct[index] = 
                    BLASTResultsStructDelete(search->mb_result_struct[index]);
              search->mb_result_struct = MemFree(search->mb_result_struct);
           }
        }
        
        if (search->allocated & BLAST_SEARCH_ALLOC_PBP) {
	    search->pbp->mb_params = MemFree(search->pbp->mb_params);
            MemFree(search->pbp->filter_string);
            search->pbp = MemFree(search->pbp);
        }
        
        if (search->allocated & BLAST_SEARCH_ALLOC_READDB) {
            search->rdfp = readdb_destruct(search->rdfp);
        }
        
        if (search->current_hitlist) {
            search->current_hitlist = BlastHitListDestruct(search->current_hitlist);
        }
        search->subject_info = BLASTSubjectInfoDestruct(search->subject_info);
        
        
        if (search->prog_name) {
            search->prog_name = MemFree(search->prog_name);
        }
        
        if (search->query_id) {
            search->query_id = SeqIdSetFree(search->query_id);
        }
        if (search->qid_array) {
            Int4 index;
            for (index=0; index<=search->last_context/2; index++)
                SeqIdSetFree(search->qid_array[index]);
            search->qid_array = MemFree(search->qid_array);
        }
        if (search->translation_buffer_size > 0) {
            search->translation_buffer = MemFree(search->translation_buffer);
        }
        
        if (search->allocated & BLAST_SEARCH_ALLOC_TRANS_INFO) {
            
            if (search->translation_table) {
                search->translation_table = MemFree(search->translation_table);
            }
            
            if (search->translation_table_rc) {
                search->translation_table_rc = MemFree(search->translation_table_rc);
            }
        }
        
        if (search->allocated & BLAST_SEARCH_ALLOC_ALL_WORDS) {
            search->all_words = BlastAllWordDestruct(search->all_words);
        }
        
        search->gap_align = GapAlignBlkDelete(search->gap_align);
        
        if (search->allocated & BLAST_SEARCH_ALLOC_QUERY_SLP) {
            if (search->query_slp)
                search->query_slp = SeqLocFree(search->query_slp);
        }
        
        
        if(search->allocated & BLAST_SEARCH_ALLOC_THRINFO)
            BlastThrInfoFree(search->thr_info);
        
        if (search->abmp)
            search->abmp = GreedyAlignMemFree(search->abmp);
        
        search->query_context_offsets = MemFree(search->query_context_offsets);
        
        MemFree(search->mb_endpoint_results);

	if (search->allocated & BLAST_SEARCH_ALLOC_MASK1)
	{
		if (search->mask1)	
		{
			SeqLocSetFree(search->mask1->data.ptrvalue);
			search->mask1 = ValNodeFree(search->mask1);
		}
	}

        search = MemFree(search);
    }
    
    return search;
}


/* 
	Deallocates all the memory associated with the BlastAllWordPtr.
*/

BlastAllWordPtr LIBCALL
BlastAllWordDestruct(BlastAllWordPtr all_words)

{
	if (all_words == NULL)
		return NULL;

	if (all_words->array)
	{
		all_words->array = MemFree(all_words->array);
	}

	if (all_words->rows_allocated && all_words->array_storage)
	{
		all_words->array_storage = MemFree(all_words->array_storage);
	}

	MemFree(all_words);

	return NULL;
}

/*
	Allocates the BlastAllWordPtr and sets some flags.
*/
BlastAllWordPtr LIBCALL
BlastAllWordNew(Int4 num_of_cols, Int4 wordsize, Boolean rows_allocated, Boolean specific)

{
	BlastAllWordPtr all_words;

	all_words = MemNew(sizeof(BlastAllWord));
	if (all_words)
	{
		all_words->rows_allocated = rows_allocated;
		all_words->specific = specific;
		all_words->num_of_cols = num_of_cols;
		all_words->wordsize = wordsize;
	}

	return all_words;
}

BLAST_HitListPtr LIBCALL
BlastHitListDestruct(BLAST_HitListPtr hitlist)
{
        BLAST_HSPPtr PNTR hsp_array;
        Int4 hspcnt_max, index;

        if (hitlist == NULL)
                return NULL;

        hspcnt_max = hitlist->hspcnt_max;
        hsp_array = hitlist->hsp_array;

        for (index=0; index<hspcnt_max; index++)
        {
           hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
        }

        hitlist->hsp_array = MemFree(hsp_array);
	hitlist->lh_helper = MemFree(hitlist->lh_helper);

        MemFree(hitlist->exact_match_array);
           
        hitlist = MemFree(hitlist);

        return hitlist;
}

/****************************************************************

        Functions to allocate and destroy the BLAST_HitList.

***************************************************************/
BLAST_HitListPtr LIBCALL
BlastHitListNew(BlastSearchBlkPtr search)
{
        BLAST_HitListPtr hitlist;

        hitlist = (BLAST_HitListPtr) MemNew(sizeof(BLAST_HitList));

        if (hitlist == NULL)
                return hitlist;

        hitlist->hspmax = search->hsp_array_size;
        hitlist->hsp_array = (BLAST_HSPPtr PNTR) MemNew(hitlist->hspmax*sizeof
(BLAST_HSPPtr));

        if (hitlist->hsp_array == NULL)
        {
                hitlist = BlastHitListDestruct(hitlist);
                return NULL;
        }

        if (search->pbp->mb_params) {
           hitlist->exact_match_array = (MegaBlastExactMatchPtr) 
              MemNew(hitlist->hspmax*sizeof(MegaBlastExactMatch));
           hitlist->exact_match_max = hitlist->hspmax;
        }

        return hitlist;
}


/*
	This function translates the context number of a context into
	the frame of the sequence.

	Arguments:
	
	BlastSearchBlkPtr search: search structure,
	Int2 context_number: context number used by BLASTContextStruct array
	Boolean is_query: if TRUE, refers to query, otherwise the subject.
*/

Int2
ContextToFrame(BlastSearchBlkPtr search, Int2 context_number)

{
	Int2 frame=255;
	Uint1 prog_number = search->prog_number;

	if (prog_number == blast_type_blastn)
	{
		if (context_number % 2 == 0)
			frame = 1;
		else
			frame = -1;
	}
        else if (prog_number == blast_type_blastp ||
                 prog_number == blast_type_tblastn ||
                 prog_number == blast_type_psitblastn)
	{	/* Query and subject are protein, no frame. */
		frame = 0;
	}
	else if (prog_number == blast_type_blastx || prog_number == blast_type_tblastx)
	{
		frame = context_number < 3 ? context_number+1 : -context_number+2;
	}

	return frame;
}

/*
	Allocates and fills in the BLASTSubjectInfo structure.
*/

BLASTSubjectInfoPtr LIBCALL
BLASTSubjectInfoNew(SeqIdPtr sip, CharPtr defline, Int4 length)

{
	BLASTSubjectInfoPtr subject_info;

	subject_info = (BLASTSubjectInfoPtr) MemNew(sizeof(BLASTSubjectInfo));	

	if (subject_info == NULL)
		return NULL;

	subject_info->sip = sip;
	subject_info->defline = defline;
	subject_info->length = length;

	return subject_info;
}

/*
	Deallocates the BLASTSubjectInfo structure and the
	SeqIdPtr, as well as the defline.
*/

BLASTSubjectInfoPtr LIBCALL
BLASTSubjectInfoDestruct(BLASTSubjectInfoPtr subject_info)

{

	if (subject_info == NULL)
		return NULL;

	SeqIdFree(subject_info->sip);
	MemFree(subject_info->defline);
	subject_info = MemFree(subject_info);

	return subject_info;
}



/*
	Destroys BLASTResultsStructure and associated memory.
*/

BLASTResultsStructPtr LIBCALL
BLASTResultsStructDelete(BLASTResultsStructPtr result_struct)

{
	Int4 index;
	BLASTResultHitlistPtr PNTR results;
	BLASTHeapPtr hp, hpt;

	if (result_struct == NULL)
		return NULL;
	
	results = result_struct->results;
	for (index=0; index<result_struct->hitlist_max; index++)
	{
		if (results[index])
		{
			results[index] = BLASTResultHitlistFree(results[index]);
		}
	}


	for (hp = result_struct->heap_ptr; hp; ) 
	{
	  hpt = hp->next;
	  hp->heap = MemFree(hp->heap);
	  hp = MemFree(hp);
	  hp = hpt;
	}
	result_struct->results = MemFree(result_struct->results);
	result_struct = MemFree(result_struct);

	return result_struct;
}

/*
	returns BLASTResultsStruct.
*/

BLASTResultsStructPtr
BLASTResultsStructNew(Int4 results_size, Int4 max_pieces, Int4 range_max)

{
	BLASTResultsStructPtr new;
	Int4 index;

	new = MemNew(sizeof(BLASTResultsStruct));
	new->results = (BLASTResultHitlistPtr PNTR) MemNew(results_size*sizeof(BLASTResultHitlistPtr));

	for (index=0; index<results_size; index++)
		new->results[index] = NULL;

	new->hitlist_max = results_size;
	new->hitlist_count = 0;
	new->max_pieces = max_pieces;
        if (range_max > 0) {
           new->heap_ptr = (BLASTHeapPtr) MemNew(sizeof(BLASTHeapStruct));
           new->heap_ptr->cutvalue = INT4_MAX;
           new->heap_ptr->num_in_heap = new->heap_ptr->num_of_ref = 0;
           new->heap_ptr->prev = new->heap_ptr->next = NULL;
           new->heap_ptr->heap = (BLASTResultHspPtr PNTR) MemNew(sizeof(BLASTResultHspPtr)*range_max);
        }
	return new;
}


Uint1 AAForCodon (Uint1Ptr codon, CharPtr codes);

/*
	GetTranslation to get the translation of the nucl. sequence in the
	appropriate frame and with the appropriate GeneticCode.

	The function return an allocated CharPtr, the caller must delete this.
	The first and last spaces of this CharPtr contain NULLB's.
*/

Uint1Ptr LIBCALL
GetTranslation(Uint1Ptr query_seq, Int4 nt_length, Int2 frame, Int4Ptr length, CharPtr genetic_code)
{
	Uint1 codon[CODON_LENGTH];
	Int4 index, index_prot;
	SeqMapTablePtr smtp;
	Uint1 residue, new_residue;
	Uint1Ptr prot_seq;

	smtp = SeqMapTableFind(Seq_code_ncbistdaa, Seq_code_ncbieaa);

	/* Allocate two extra spaces for NULLB's at beginning and end of seq. */
	prot_seq = (Uint1Ptr) MemNew((2+(nt_length+2)/CODON_LENGTH)*sizeof(Uint1));

	/* The first character in the protein is the NULLB sentinel. */
	prot_seq[0] = NULLB;
	index_prot = 1;
	for (index=ABS(frame)-1; index<nt_length-2; index += CODON_LENGTH)
	{
		codon[0] = query_seq[index];
		codon[1] = query_seq[index+1];
		codon[2] = query_seq[index+2];
		residue = AAForCodon(codon, genetic_code);
		new_residue = SeqMapTableConvert(smtp, residue);
		if (IS_residue(new_residue))
		{
			prot_seq[index_prot] = new_residue;
		}
		index_prot++;
	}
	prot_seq[index_prot] = NULLB;
	*length = index_prot-1;
	
	return prot_seq;
}


/*************************************************************************
*
*	MaskTheResidues masks up to max_length residues in buffer.
*	The residue to be used for masking (generally 'N' for nucleotides
*	and 'X' for proteins) is mask_residue.  offset tells how far
*	along the sequence the first residue in buffer is.  mask_slp
*	specifies which parts of the sequence to mask.  'max_length is
*	the total length of the sequence.
*
*************************************************************************/

void
BlastMaskTheResidues(Uint1Ptr buffer, Int4 max_length, Uint1 mask_residue, SeqLocPtr mask_slp, Boolean reverse, Int4 offset)

{
	SeqLocPtr slp=NULL;
        Int4 index, start, stop;
       
	while (mask_slp)
	{
		slp=NULL;
        	while((slp = SeqLocFindNext(mask_slp, slp))!=NULL)
        	{
			if (reverse)
			{
				start = max_length - 1 - SeqLocStop(slp);
				stop = max_length - 1 - SeqLocStart(slp);
			}
			else
			{
              			start = SeqLocStart(slp);
              			stop = SeqLocStop(slp);
			}

			start -= offset;
			stop  -= offset;

			for (index=start; index<=stop; index++)
			{
				buffer[index] = mask_residue;
			}
        	}
		mask_slp = mask_slp->next;
	}

}

Boolean LIBCALL FilterDNA(BioseqPtr bsp, Int4 filter)
{
  SeqLocPtr slp = NULL, dust_slp=NULL, dust_slp_start=NULL;
  Uint1 mask_residue = 'N';
  Int4 index, start, stop;
  Uint1 oldcode;
  CharPtr buffer;

  switch(filter) {
    
  case FILTER_DUST:
    oldcode = bsp->seq_data_type;
    dust_slp = BioseqDust(bsp, 0, -1, -1, -1, -1, -1); 
    dust_slp_start = dust_slp;
    BioseqConvert(bsp, Seq_code_iupacna);
    buffer = MemNew(bsp->length);
    BSSeek(bsp->seq_data, 0, SEEK_SET);    
    BSRead(bsp->seq_data, buffer, bsp->length);
    while (dust_slp) {
      slp=NULL;
      while((slp = SeqLocFindNext(dust_slp, slp))!=NULL) {
        start = SeqLocStart(slp);
        stop = SeqLocStop(slp);
        for (index=0; index < bsp->length; index++) {
          if (index >= start) {
            if (index <= stop)
              buffer[index] = mask_residue;
            else if (index > stop)
              break;
          }
        }
      }
      dust_slp = dust_slp->next;
    }
    dust_slp_start = SeqLocSetFree(dust_slp_start);
    BSSeek(bsp->seq_data, 0, SEEK_SET);
    BSWrite(bsp->seq_data, buffer, bsp->length);
    BioseqConvert(bsp, oldcode);
    MemFree(buffer);
    break;

  default:
    return FALSE;  /* wrong type of filter used */
  }
  return TRUE;
}

/*
	COnverts a protein (translated) SeqLocPtr from the protein
	coordinates to the nucl. coordinates.

	Only works on a SeqLocPtr of type SeqIntPtr right now.
*/

Boolean
BlastConvertProteinSeqLoc(SeqLocPtr slp, Int2 frame, Int4 full_length)

{
	SeqIntPtr seq_int;
	Int4 from, to;

	if (slp == NULL)
		return TRUE;

	if (slp->choice == SEQLOC_PACKED_INT)
		slp = slp->data.ptrvalue;

	while (slp)
	{
		if (slp->choice != SEQLOC_INT)
			return FALSE;

		seq_int = slp->data.ptrvalue;
		from = seq_int->from;
		to = seq_int->to;

		if (frame < 0)
		{
			seq_int->to = full_length - CODON_LENGTH*from + frame;
			seq_int->from = full_length - CODON_LENGTH*to + frame + 1;
			seq_int->strand = Seq_strand_minus;
		}
		else
		{
			seq_int->from = CODON_LENGTH*from + frame - 1;
			seq_int->to = CODON_LENGTH*to + frame - 1;
			seq_int->strand = Seq_strand_plus;
		}
		slp = slp->next;
	}
	
	return TRUE;
}

/*
  COnverts a DNA SeqLocPtr from the nucl. coordinates to 
  the protein (translated) coordinates.
  Only works on a SeqLocPtr of type SEQLOC_INT or SEQLOC_PACKED_INT right now.
*/

Boolean
BlastConvertDNASeqLoc(SeqLocPtr slp, Int2 frame, Int4 full_length) 
{
    SeqIntPtr seq_int;
    Int4 from, to;
    
    if (slp == NULL)
        return TRUE;
    
    if (slp->choice == SEQLOC_PACKED_INT)
        slp = slp->data.ptrvalue;
    
    while (slp) {
        if (slp->choice != SEQLOC_INT)
            return FALSE;
        
        seq_int = slp->data.ptrvalue;
        from = seq_int->from;
        to = seq_int->to;
        
        if (frame < 0) {
            seq_int->from = (full_length + frame - to)/CODON_LENGTH;
            seq_int->to = (full_length + frame - from)/CODON_LENGTH;
            seq_int->strand = Seq_strand_minus;
        } else {
            seq_int->from = (from - frame + 1)/CODON_LENGTH;
            seq_int->to = (to-frame + 1)/CODON_LENGTH;
            seq_int->strand = Seq_strand_plus;
        }
        slp = slp->next;
    }
    
    return TRUE;
}

SeqLocPtr
BioseqSegEx(BioseqPtr bsp_unfilter, CharPtr options)

{
	BioseqPtr bsp_filter;
	Boolean mask_state;
	Char cmd_buf[2*PATH_MAX], temp_file[PATH_MAX];
	CharPtr filter_dir;
	Int4 index, mask_begin=0;
	SeqEntryPtr sep;
	SeqLocPtr slp_mask;
	SeqPortPtr spp_filter, spp_unfilter;
	Uint1 res_filter, res_unfilter;
	FILE *fp;


	if (bsp_unfilter == NULL)
		return NULL;

#ifdef OS_UNIX

	TmpNam(temp_file);
	fp = FileOpen(temp_file, "w");
	if (BioseqToFasta(bsp_unfilter, fp, FALSE) == FALSE)
	{
		BioseqUnlock(bsp_unfilter);
		FileClose(fp);
		return NULL;
	}
	FileClose(fp);

	filter_dir = getenv("BLASTFILTER");
	if (filter_dir == NULL)
		filter_dir = BLASTFILTER_DIR;

	if (options != NULL)
		sprintf(cmd_buf, "%s%s%s%s %s%s", filter_dir, DIRDELIMSTR, "seg ", temp_file, options, " -x");
	else
		sprintf(cmd_buf, "%s%s%s%s%s", filter_dir, DIRDELIMSTR, "seg ", temp_file, " -x");

	fp = popen(cmd_buf, "r");
	if (fp == NULL)
	{
		ErrPostEx(SEV_WARNING, 0, 0, "Call to seg failed.");
		return NULL;
	}
	
	sep = FastaToSeqEntry(fp, FALSE);
	FileClose(fp);
	if (sep == NULL)
	{
		ErrPostEx(SEV_WARNING, 0, 0, "Call to seg failed.");
		return NULL;
	}
	bsp_filter = sep->data.ptrvalue;

	spp_filter = SeqPortNew(bsp_filter, 0, -1, Seq_strand_plus, Seq_code_ncbistdaa);
	spp_unfilter = SeqPortNew(bsp_unfilter, 0, -1, Seq_strand_plus, Seq_code_ncbistdaa);

	mask_state = FALSE;
	index = 0;
	slp_mask = NULL;
	while ((res_filter=SeqPortGetResidue(spp_filter)) != SEQPORT_EOF)
	{
		res_unfilter=SeqPortGetResidue(spp_unfilter);
		if (res_filter != res_unfilter)
		{
			if (mask_state == FALSE)
			{
				mask_begin = index;
				mask_state = TRUE;
			}
		}
		else if (mask_state == TRUE)
		{
			ValNodeLink(&slp_mask, SeqLocIntNew(mask_begin, index-1, Seq_strand_plus, bsp_filter->id));
			mask_state = FALSE;
		}
		index++;
	}

	/* If the last portion of the sequence was masked. */
	if (mask_state == TRUE)
	{
		ValNodeLink(&slp_mask, SeqLocIntNew(mask_begin, index-1, Seq_strand_plus, bsp_filter->id));
	}

	sep = SeqEntryFree(sep);
	SeqPortFree(spp_filter);
	SeqPortFree(spp_unfilter);

	pclose(fp);
	FileRemove(temp_file);

	return slp_mask;
#else
	return NULL;
#endif
}

/*
	Runs seg and obtains a SeqLocPtr from it.
*/
static SeqLocPtr 
SeqLocSegEx(SeqLocPtr slp, CharPtr instructions)

{
	BioseqPtr bsp_unfilter;
	SeqLocPtr slp_mask;
	SeqIdPtr sip;


	if (slp == NULL)
		return NULL;

	sip = SeqIdFindBest(SeqLocId(slp), SEQID_GI);
	bsp_unfilter = BioseqLockById(sip);
	slp_mask = BioseqSegEx(bsp_unfilter, instructions);

	BioseqUnlock(bsp_unfilter);

	return slp_mask;
}

SeqLocPtr 
SeqLocSeg(SeqLocPtr slp)

{
	return SeqLocSegEx(slp, NULL);
}

SeqLocPtr
MyBioseqSeg(BioseqPtr bsp_unfilter)

{
	return BioseqSegEx(bsp_unfilter, NULL);
}

#define BLASTSEQLOC_BUFFER_SIZE 128

Boolean
parse_blast_options(BLAST_OptionsBlkPtr options, CharPtr string_options, 
                    CharPtr PNTR error_message, CharPtr PNTR database, 
                    Int4Ptr descriptions, Int4Ptr alignments)
{
    	CharPtr opt_str = "GErqeWdyXZPAIvbYzcFsSpfwtgn", *values;
	Int4 index;

	if (options == NULL)
		return FALSE;

	if(!BlastParseInputString(string_options, opt_str, &values, error_message)) 
	{
	    return FALSE;
	}

	/* -G  gap open cost */

	index = BlastGetLetterIndex(opt_str, 'G');
	if(values[index] != NULL) {
	    options->gap_open = atoi(values[index]);
	}

	/* -E gap extend cost */

	index = BlastGetLetterIndex(opt_str, 'E');
	if(values[index] != NULL) {
	    options->gap_extend = atoi(values[index]);
	}

	/* -q penalty for nucleotide mismatch. */

	index = BlastGetLetterIndex(opt_str, 'q');
	if(values[index] != NULL) {
	    options->penalty = atoi(values[index]);
	}

	/* -r reward for nucleotide match. */

	index = BlastGetLetterIndex(opt_str, 'r');
	if(values[index] != NULL) {
	    options->reward = atoi(values[index]);
	}

	/* -e expect value. */

	index = BlastGetLetterIndex(opt_str, 'e');
	if(values[index] != NULL) {
	    options->expect_value = atof(values[index]);
	}

	/* -W wordsize. */

	index = BlastGetLetterIndex(opt_str, 'W');
	if(values[index] != NULL) {
	    options->wordsize = atoi(values[index]);
	}

	/* -d database. */
        if (database) {
           index = BlastGetLetterIndex(opt_str, 'd');
           if(values[index] != NULL) {
              *database = values[index];
              values[index] = NULL;
           }
        }

	/* -y  Dropoff (X) for blast extensions in bits (default if zero) */

	index = BlastGetLetterIndex(opt_str, 'y');
	if(values[index] != NULL) {
	    options->dropoff_2nd_pass = atof(values[index]);
	}

	/* -X  X dropoff value for gapped alignment (in bits) */

	index = BlastGetLetterIndex(opt_str, 'X');
	if(values[index] != NULL) {
	    options->gap_x_dropoff = atof(values[index]);
	}

	/* -Z  final X dropoff value for gapped alignment (in bits) */

	index = BlastGetLetterIndex(opt_str, 'Z');
	if(values[index] != NULL) {
	    options->gap_x_dropoff_final = atof(values[index]);
	}

	/* -P multiple hits/two-pass. */

        index = BlastGetLetterIndex(opt_str, 'P');
        if(values[index] != NULL) {
           if (atoi(values[index]) == 0) 
           {
                 options->two_pass_method  = FALSE;
                 options->multiple_hits_only  = TRUE;
           }
           else if (atoi(values[index]) == 1)
           {
                 options->two_pass_method  = FALSE;
                 options->multiple_hits_only  = FALSE;
           }
           else
	   {
                 options->two_pass_method  = TRUE;
                 options->multiple_hits_only  = FALSE;
           }
        }

	/* -A window size. */

	index = BlastGetLetterIndex(opt_str, 'A');
	if(values[index] != NULL) {
	    options->window_size = atoi(values[index]);
	}

        /* -I Hitlist size */
        index = BlastGetLetterIndex(opt_str, 'I');
        if (values[index] != NULL)
           options->hitlist_size = atoi(values[index]);

        /* -v Number of descriptions */
        if (descriptions) {
           *descriptions = -1;
           index = BlastGetLetterIndex(opt_str, 'v');
           if (values[index] != NULL) {
              *descriptions = atoi(values[index]);
              options->hitlist_size = 
                 MAX(options->hitlist_size, *descriptions);
           }
        }

        /* -b Number of alignments */
        if (alignments) {
           *alignments = -1;
           index = BlastGetLetterIndex(opt_str, 'b');
           if (values[index] != NULL) {
              *alignments = atoi(values[index]);
              options->hitlist_size = 
                 MAX(options->hitlist_size, *alignments);
           }
        }

        /* -Y Effective search space */
        index = BlastGetLetterIndex(opt_str, 'Y');
        if (values[index] != NULL)
           options->searchsp_eff = atof(values[index]);

        /* -z Effective database length */
        index = BlastGetLetterIndex(opt_str, 'z');
        if (values[index] != NULL) {
           const char **dummy=NULL;
           options->db_length =  StringToInt8(values[index], dummy);
        }

        /* -c Constant in pseudocounts for multipass version */
        index = BlastGetLetterIndex(opt_str, 'c');
        if (values[index] != NULL)
           options->pseudoCountConst = atoi(values[index]);
        
        /* -F Filter string */
        index = BlastGetLetterIndex(opt_str, 'F');
        if (values[index] != NULL)
           options->filter_string = values[index];

        /* -s Score cut off for megablast */
        index = BlastGetLetterIndex(opt_str, 's');
        if (values[index] != NULL)
           options->cutoff_s2 = atoi(values[index]);

        /* -S Strand option */
        index = BlastGetLetterIndex(opt_str, 'S');
        if (values[index] != NULL)
           options->strand_option = (Uint1) atoi(values[index]);
        
        /* -p Percentage of identity cut-off */
        index = BlastGetLetterIndex(opt_str, 'p');
        if (values[index] != NULL)
           options->perc_identity = (FloatLo) atof(values[index]);

	/* -f  threshold for hits */

	index = BlastGetLetterIndex(opt_str, 'f');
	if(values[index] != NULL) {
	    options->threshold_second = atoi(values[index]);
	}

	/* -w  Frame shift penalty (OOF algorithm for blastx) */

	index = BlastGetLetterIndex(opt_str, 'w');
	if(values[index] != NULL) {
	    options->shift_pen = atoi(values[index]);
	    options->is_ooframe = TRUE;
	}

	/* -t  Discontiguous word template length for megablast;
               Longest intron length for sum statistics in tblastn */

	index = BlastGetLetterIndex(opt_str, 't');
	if(values[index] != NULL) {
           if (options->is_megablast_search)
              options->mb_template_length = atoi(values[index]);
           else 
              options->longest_intron = atoi(values[index]);
	}

	/* -g  Scan every base of the database for megablast */

	index = BlastGetLetterIndex(opt_str, 'g');
	if(values[index] != NULL) {
	    options->mb_one_base_step = (TO_UPPER(*values[index]) == 'T');
	}

	/* -n  Use dynamic programming algorithm in megablast for gapped 
               extensions instead of greedy algorithm */

	index = BlastGetLetterIndex(opt_str, 'n');
	if(values[index] != NULL) {
	    options->mb_use_dyn_prog = (TO_UPPER(*values[index]) == 'T');
	}

	values = MemFree(values);

	return TRUE;
}

static Boolean
parse_dust_options(CharPtr ptr, Int4Ptr level, Int4Ptr window, Int4Ptr cutoff, Int4Ptr linker)

{
	Char buffer[BLASTSEQLOC_BUFFER_SIZE];
	Int4 arg, index, index1, window_pri=-1, linker_pri=-1, level_pri=-1, cutoff_pri=-1;
	long	tmplong;

	arg = 0;
	index1 = 0;
	for (index=0; index<BLASTSEQLOC_BUFFER_SIZE; index++)
	{
		if (*ptr == ' ' || *ptr == NULLB)
		{
			buffer[index1] = NULLB;
			index1 = 0;
			switch(arg) {
				case 0:
					sscanf(buffer, "%ld", &tmplong);
					level_pri = tmplong;
					break;
				case 1:
					sscanf(buffer, "%ld", &tmplong);
					window_pri = tmplong;
					break;
				case 2:
					sscanf(buffer, "%ld", &tmplong);
					cutoff_pri = tmplong;
					break;
				case 3:
					sscanf(buffer, "%ld", &tmplong);
					linker_pri = tmplong;
					break;
				default:
					break;
			}

			arg++;
			while (*ptr == ' ')
				ptr++;

			/* end of the buffer. */
			if (*ptr == NULLB)
				break;
		}
		else
		{
			buffer[index1] = *ptr; ptr++;
			index1++;
		}
	}

	*level = level_pri; 
	*window = window_pri; 
	*cutoff = cutoff_pri; 
	*linker = linker_pri; 

	return TRUE;
}


static Boolean
parse_seg_options(CharPtr ptr, Int4Ptr window, FloatHiPtr locut, FloatHiPtr hicut)

{
	Char buffer[BLASTSEQLOC_BUFFER_SIZE];
	Int4 arg, index, index1; 
	long	tmplong;
	FloatHi	tmpdouble;

	arg = 0;
	index1 = 0;
	for (index=0; index<BLASTSEQLOC_BUFFER_SIZE; index++)
	{
		if (*ptr == ' ' || *ptr == NULLB)
		{
			buffer[index1] = NULLB;
			index1 = 0;
			switch(arg) {
				case 0:
					sscanf(buffer, "%ld", &tmplong);
					*window = tmplong;
					break;
				case 1:
					sscanf(buffer, "%le", &tmpdouble);
					*locut = tmpdouble;
					break;
				case 2:
					sscanf(buffer, "%le", &tmpdouble);
					*hicut = tmpdouble;
					break;
				default:
					break;
			}

			arg++;
			while (*ptr == ' ')
				ptr++;

			/* end of the buffer. */
			if (*ptr == NULLB)
				break;
		}
		else
		{
			buffer[index1] = *ptr; ptr++;
			index1++;
		}
	}

	return TRUE;
}

static Boolean
parse_cc_options(CharPtr ptr, Int4Ptr window, FloatHiPtr cutoff, Int4Ptr linker)

{
	Char buffer[BLASTSEQLOC_BUFFER_SIZE];
	Int4 arg, index, index1;
	long	tmplong;
	FloatHi	tmpdouble;

	arg = 0;
	index1 = 0;
	for (index=0; index<BLASTSEQLOC_BUFFER_SIZE; index++)
	{
		if (*ptr == ' ' || *ptr == NULLB)
		{
			buffer[index1] = NULLB;
			index1 = 0;
			switch(arg) {
				case 0:
					sscanf(buffer, "%ld", &tmplong);
					*window = tmplong;
					break;
				case 1:
					sscanf(buffer, "%le", &tmpdouble);
					*cutoff = tmpdouble;
					break;
				case 2:
					sscanf(buffer, "%ld", &tmplong);
					*linker = tmplong;
					break;
				default:
					break;
			}

			arg++;
			while (*ptr == ' ')
				ptr++;

			/* end of the buffer. */
			if (*ptr == NULLB)
				break;
		}
		else
		{
			buffer[index1] = *ptr; ptr++;
			index1++;
		}
	}

	return TRUE;
}

CharPtr
load_options_to_buffer(CharPtr instructions, CharPtr buffer)
{
	Boolean not_started=TRUE;
	CharPtr buffer_ptr, ptr;
	Int4 index;

	ptr = instructions;
	buffer_ptr = buffer;
	for (index=0; index<BLASTSEQLOC_BUFFER_SIZE && *ptr != NULLB; index++)
	{
		if (*ptr == ';')
		{
			ptr++;
			break;
		}
		/* Remove blanks at the beginning. */
		if (not_started && *ptr == ' ')
		{
			ptr++;
		}
		else
		{
			not_started = FALSE;
			*buffer_ptr = *ptr;
			buffer_ptr++; ptr++;
		}
	}

	*buffer_ptr = NULLB;

	if (not_started == FALSE)
	{	/* Remove trailing blanks. */
		buffer_ptr--;
		while (*buffer_ptr == ' ' && buffer_ptr > buffer)
		{
			*buffer_ptr = NULLB;
			buffer_ptr--;
		}
	}

	return ptr;
}

#define CC_WINDOW 22
#define CC_CUTOFF 40.0
#define CC_LINKER 32

/*
	This function parses the 'instructions' string and then calls the appopriate
	filtering functions.
*/
SeqLocPtr
BlastBioseqFilter(BioseqPtr bsp, CharPtr instructions)

{
	return BlastBioseqFilterEx(bsp, instructions, NULL);
}

SeqLocPtr
BlastBioseqFilterEx(BioseqPtr bsp, CharPtr instructions, BoolPtr mask_at_hash)

{
	SeqLocPtr slp = NULL;
	SeqLocPtr slp_mask;
	
	ValNodeAddPointer(&slp, SEQLOC_WHOLE,
		SeqIdDup(SeqIdFindBest(bsp->id, SEQID_GI)));
	slp_mask = BlastSeqLocFilterEx(slp, instructions, mask_at_hash);
	slp = SeqLocFree(slp);
	return slp_mask;
}

SeqLocPtr
BlastSeqLocFilter(SeqLocPtr slp, CharPtr instructions)

{
	return BlastSeqLocFilterEx(slp, instructions, NULL);
}

SeqLocPtr
BlastSeqLocFilterEx(SeqLocPtr slp, CharPtr instructions, BoolPtr mask_at_hash)

{
	BioseqPtr bsp;
	BLAST_OptionsBlkPtr repeat_options, vs_options;
	Boolean do_all=FALSE, do_seg=FALSE, do_coil_coil=FALSE, do_dust=FALSE, do_repeats=FALSE, do_vecscreen=FALSE;
	Boolean myslp_allocated;
	CharPtr buffer=NULL;
	CharPtr ptr, repeat_database=NULL, vs_database=NULL, error_msg;
	Int2 seqloc_num;
	Int4 window_cc, linker_cc, window_dust, level_dust, minwin_dust, linker_dust;
	SeqLocPtr cc_slp=NULL, dust_slp=NULL, seg_slp=NULL, seqloc_head=NULL, repeat_slp=NULL, vs_slp=NULL;
	PccDatPtr pccp;
	Nlm_FloatHiPtr scores;
	Nlm_FloatHi cutoff_cc;
	SegParamsPtr sparamsp=NULL;
	SeqAlignPtr seqalign;
	SeqIdPtr sip;
	SeqLocPtr myslp, seqloc_var, seqloc_tmp;
	ValNodePtr vnp=NULL, vnp_var;

	cutoff_cc = CC_CUTOFF;

	if (instructions == NULL || StringICmp(instructions, "F") == 0)
		return NULL;

	/* FALSE is the default right now. */
	if (mask_at_hash)
		*mask_at_hash = FALSE;

	/* parameters for dust. */
	/* -1 indicates defaults. */
	level_dust = -1;
	window_dust = -1;
	minwin_dust = -1;
	linker_dust = -1;
	if (StringICmp(instructions, "T") == 0)
	{ /* do_all actually means seg for proteins and dust for nt. */
		do_all = TRUE;
	}
	else
	{
		buffer = MemNew(StringLen(instructions)*sizeof(Char));
		ptr = instructions;
		/* allow old-style filters when m cannot be followed by the ';' */
		if (*ptr == 'm' && ptr[1] == ' ')
		{
			if (mask_at_hash)
				*mask_at_hash = TRUE;
			ptr += 2;
		}
		while (*ptr != NULLB)
		{
			if (*ptr == 'S')
			{
				sparamsp = SegParamsNewAa();
				sparamsp->overlaps = TRUE;	/* merge overlapping segments. */
				ptr = load_options_to_buffer(ptr+1, buffer);
				if (buffer[0] != NULLB)
				{
					parse_seg_options(buffer, &sparamsp->window, &sparamsp->locut, &sparamsp->hicut);
				}
				do_seg = TRUE;
			}
			else if (*ptr == 'C')
			{
				ptr = load_options_to_buffer(ptr+1, buffer);
				window_cc = CC_WINDOW;
				cutoff_cc = CC_CUTOFF;
				linker_cc = CC_LINKER;
				if (buffer[0] != NULLB)
					parse_cc_options(buffer, &window_cc, &cutoff_cc, &linker_cc);
				do_coil_coil = TRUE;
			}
			else if (*ptr == 'D')
			{
				ptr = load_options_to_buffer(ptr+1, buffer);
				if (buffer[0] != NULLB)
					parse_dust_options(buffer, &level_dust, &window_dust, &minwin_dust, &linker_dust);
				do_dust = TRUE;
			}
			else if (*ptr == 'R')
			{
				repeat_options = BLASTOptionNew("blastn", TRUE);
				repeat_options->expect_value = 0.1;
				repeat_options->penalty = -1;
				repeat_options->wordsize = 11;
				repeat_options->gap_x_dropoff_final = 90;
				repeat_options->dropoff_2nd_pass = 40;
				repeat_options->gap_open = 2;
				repeat_options->gap_extend = 1;
				ptr = load_options_to_buffer(ptr+1, buffer);
				if (buffer[0] != NULLB)
                                   parse_blast_options(repeat_options,
                                      buffer, &error_msg, &repeat_database,
                                      NULL, NULL);
				if (repeat_database == NULL)
                                   repeat_database = StringSave("humlines.lib humsines.lib retrovir.lib");
				do_repeats = TRUE;
			}
			else if (*ptr == 'V')
			{
				vs_options = VSBlastOptionNew();
				ptr = load_options_to_buffer(ptr+1, buffer);
				if (buffer[0] != NULLB)
                                   parse_blast_options(vs_options, buffer,
                                      &error_msg, &vs_database, NULL, NULL); 
				vs_options = BLASTOptionDelete(vs_options);
				if (vs_database == NULL)
                                   vs_database = StringSave("UniVec_Core");
				do_vecscreen = TRUE;
			}
			else if (*ptr == 'L')
			{ /* do low-complexity filtering; dust for blastn, otherwise seg.*/
				do_all = TRUE;
				ptr++;
			}
			else if (*ptr == 'm')
			{
				if (mask_at_hash)
					*mask_at_hash = TRUE;
				ptr++;
			}
			else
			{	/* Nothing applied. */
				ptr++;
			}
		}
		buffer = MemFree(buffer);
	}

	seqloc_num = 0;
	seqloc_head = NULL;
	sip = SeqLocId(slp);
	bsp = BioseqLockById(SeqIdFindBest(sip, SEQID_GI));
	if (ISA_aa(bsp->mol))
	{
		if (do_all || do_seg)
		{
			seg_slp = SeqlocSegAa(slp, sparamsp);
			SegParamsFree(sparamsp);
			sparamsp = NULL;
			seqloc_num++;
		}
		if (do_coil_coil)
		{
			pccp = PccDatNew ();
			pccp->window = window_cc;
			ReadPccData (pccp);
			/*scores = PredictCCBioseq(bsp, 0, bsp->length-1, pccp);*/
			scores = PredictCCSeqLoc(slp, pccp);
			cc_slp = FilterCC(scores, cutoff_cc, SeqLocLen(slp), linker_cc, SeqIdDup(sip), FALSE);
			MemFree(scores);
			PccDatFree (pccp);
			seqloc_num++;
		}
	}
	else
	{
		if (do_all || do_dust)
		{
			dust_slp = SeqLocDust(slp, level_dust, window_dust, minwin_dust, linker_dust);
			seqloc_num++;
		}
		if (do_repeats)
		{
		/* Either the SeqLocPtr is SEQLOC_WHOLE (both strands) or SEQLOC_INT (probably 
one strand).  In that case we make up a double-stranded one as we wish to look at both strands. */
			myslp_allocated = FALSE;
			if (slp->choice == SEQLOC_INT)
			{
				myslp = SeqLocIntNew(SeqLocStart(slp), SeqLocStop(slp), Seq_strand_both, SeqLocId(slp));
				myslp_allocated = TRUE;
			}
			else
			{
				myslp = slp;
			}
start_timer;
			repeat_slp = BioseqHitRangeEngineByLoc(myslp, "blastn", repeat_database, repeat_options, NULL, NULL, NULL, NULL, NULL, 0);
stop_timer("after repeat filtering");
			repeat_options = BLASTOptionDelete(repeat_options);
			repeat_database = MemFree(repeat_database);
			if (myslp_allocated)
				SeqLocFree(myslp);
			seqloc_num++;
		}
		if (do_vecscreen)
		{
		/* Either the SeqLocPtr is SEQLOC_WHOLE (both strands) or SEQLOC_INT (probably 
one strand).  In that case we make up a double-stranded one as we wish to look at both strands. */
			myslp_allocated = FALSE;
			if (slp->choice == SEQLOC_INT)
			{
				myslp = SeqLocIntNew(SeqLocStart(slp), SeqLocStop(slp), Seq_strand_both, SeqLocId(slp));
				myslp_allocated = TRUE;
			}
			else
			{
				myslp = slp;
			}
			VSScreenSequenceByLoc(myslp, NULL, vs_database, &seqalign, &vnp, NULL, NULL);
			vnp_var = vnp;
			while (vnp_var)
			{
				seqloc_tmp = vnp_var->data.ptrvalue;
				if (vs_slp == NULL)
				{
					vs_slp = seqloc_tmp;
				}
				else
				{
					seqloc_var = vs_slp;
					while (seqloc_var->next)
						seqloc_var = seqloc_var->next;
					seqloc_var->next = seqloc_tmp;
				}
				vnp_var->data.ptrvalue = NULL;
				vnp_var = vnp_var->next;
			}
			vnp = ValNodeFree(vnp);
			seqalign = SeqAlignSetFree(seqalign);
			vs_database = MemFree(vs_database);
			if (myslp_allocated)
				SeqLocFree(myslp);
			seqloc_num++;
		}
	}

	if (seqloc_num == 0)
	{ /* nothing. */
		;
	} 
	else if (seqloc_num == 1)
	{
		if (seg_slp)
			seqloc_head = seg_slp;
		if (cc_slp)
			seqloc_head = cc_slp;
		if (dust_slp)
			seqloc_head = dust_slp;
		if (repeat_slp)
			seqloc_head = repeat_slp;
		if (vs_slp)
			seqloc_head = vs_slp;
	}
	else
	{
		if (seg_slp)
			ValNodeAddPointer(&seqloc_head, SEQLOC_MIX, seg_slp);
		if (cc_slp)
			ValNodeAddPointer(&seqloc_head, SEQLOC_MIX, cc_slp);
		if (dust_slp)
			ValNodeAddPointer(&seqloc_head, SEQLOC_MIX, dust_slp);
		if (repeat_slp)
			ValNodeAddPointer(&seqloc_head, SEQLOC_MIX, repeat_slp);
		if (vs_slp)
			ValNodeAddPointer(&seqloc_head, SEQLOC_MIX, vs_slp);
	}

	BioseqUnlock(bsp);
	return seqloc_head;
}

/*
	Program to run seg on a sequence.  Note that this program only
	really works in UNIX systems.
*/
Boolean LIBCALL
FilterWithSeg (Uint1Ptr sequence, Int4 length, Uint1 alphabet)

{

#ifdef OS_UNIX

	BioseqPtr bsp;
	Char cmd_buf[2*PATH_MAX], temp_file[PATH_MAX];
	CharPtr filter_dir;
	FILE PNTR fp;
	Int4 byte_store_length;
	Nlm_ByteStorePtr byte_store;
	SeqEntryPtr sep;

	if (sequence == NULL || length == 0)
		return FALSE;

	byte_store = Nlm_BSNew(length);

	byte_store_length = Nlm_BSWrite(byte_store, (VoidPtr) sequence, length);
	if (length != byte_store_length)
	{
		Nlm_BSDelete(byte_store, length);
		return FALSE;
	}

	bsp = BioseqNew();
	bsp->seq_data = byte_store;
	bsp->length = length;
	bsp->seq_data_type = alphabet;
	bsp->mol = Seq_mol_aa;
	bsp->repr = Seq_repr_raw;

	TmpNam(temp_file);
	fp = FileOpen(temp_file, "w");
	if (BioseqToFasta(bsp, fp, FALSE) == FALSE)
	{
		bsp = BioseqFree(bsp);
		return FALSE;
	}
	FileClose(fp);

	bsp = BioseqFree(bsp);

	filter_dir = getenv("BLASTFILTER");
	if (filter_dir != NULL)
		sprintf(cmd_buf, "%s%s%s%s%s", filter_dir, DIRDELIMSTR, "seg ", temp_file, " -x");
	else
		sprintf(cmd_buf, "%s%s%s%s%s", BLASTFILTER_DIR, DIRDELIMSTR, "seg ", temp_file, " -x");

	fp = popen(cmd_buf, "r");
	if (fp == NULL)
	{
		ErrPostEx(SEV_WARNING, 0, 0, "Call to seg failed.");
		return FALSE;
	}
	
	sep = FastaToSeqEntry(fp, FALSE);
	if (sep == NULL)
	{
		ErrPostEx(SEV_WARNING, 0, 0, "Call to seg failed.");
		return FALSE;
	}

	pclose(fp);

	bsp = sep->data.ptrvalue;
	BioseqRawConvert(bsp, Seq_code_ncbistdaa);

	BSSeek(bsp->seq_data, 0, SEEK_SET);
	Nlm_BSRead(bsp->seq_data, (VoidPtr) sequence, length);

	SeqEntryFree(sep);

	FileRemove(temp_file);

	return TRUE;
#else
	return FALSE;
#endif
}


BLAST_HSPPtr BLAST_HSPFree(BLAST_HSPPtr hsp)
{
if (hsp)
   hsp->gap_info = GapXEditBlockDelete(hsp->gap_info);

return (BLAST_HSPPtr) MemFree(hsp);
}

/* 
	Frees memory used for HSP's on the ResultHitlist.
	Should be called as the SeqAlignPtr for a hitlist
	is produced to save memory.
*/

void
BLASTResultFreeHsp(BLASTResultHitlistPtr result)

{
  	BLASTResultHspPtr hsp;
	Int4 index;

	if (result == NULL || result->hsp_array == NULL)
		return;

        for(index=0; index < result->hspcnt; index++) {
          hsp = &result->hsp_array[index];
	  if (hsp)
	     hsp->gap_info = GapXEditBlockDelete(hsp->gap_info);
        }

	if (result->hspcnt != 0)
		result->hsp_array = MemFree(result->hsp_array);

	result->hspcnt = 0;

	return;
}

/* 
	Free's the hitlist without performing a check 
	on the integrity of the heap (used for culling).
*/
BLASTResultHitlistPtr LIBCALL
BLASTResultHitlistFree(BLASTResultHitlistPtr result)

{
	return BLASTResultHitlistFreeEx(NULL, result);
	
}


BLASTResultHitlistPtr LIBCALL
BLASTResultHitlistFreeEx(BlastSearchBlkPtr search, BLASTResultHitlistPtr result)

{
        BLASTHeapPtr hp;
        Int4 index;
  	register Int4 subject_id;
	
	if (result == NULL)
		return NULL;


	/* 
	Check the integrity of the heap used for culling.  Occassionally
	HSP's that have been saved (in the heap before the start of
	the HSP) are missed. 
	Only do this if the BlastSearchBlkPtr was provided.
	*/
	if (search && search->pbp->perform_culling == TRUE && result->num_ref > 0)
	{
		subject_id = result->subject_id;

		/* result->num_ref can change in the loop. */
		for (hp = search->result_struct->heap_ptr; hp && result->num_ref>0; hp = hp->next)
		{
                	index=0; /* Note that hp->num_in_heap can change in the loop */
			while (index < hp->num_in_heap)
			{
				if (hp->heap[index]->point_back->subject_id == subject_id)
				{		
					BlastDeleteHeap(hp, index);
				}
				else
					index++;
			}
		}
	}

	/* In case it was not freed before. */
	BLASTResultFreeHsp(result);

	BLASTSubjectInfoDestruct(result->subject_info);	

	result = MemFree(result);

	return result;
}

/*
	Creates a new BLASTResultHitlist, with the an hsp-array of length hspcnt.  If the
	allocation fails, then NULL is returned.
*/

BLASTResultHitlistPtr LIBCALL
BLASTResultHitlistNew(Int4 hspcnt)

{

	BLASTResultHitlistPtr new;

	new = (BLASTResultHitlistPtr) MemNew(sizeof(BLASTResultHitlist));
	if (new == NULL)
		return NULL;

	new->hsp_array = (BLASTResultHspPtr) MemNew(hspcnt*sizeof(BLASTResultHsp));
	if (new->hsp_array == NULL)
	{
		new = BLASTResultHitlistFree(new);
		return NULL;
	}
	new->hspcnt = hspcnt;

	return new; 
}


static Boolean 
CopyHSPToResultHsp(BLAST_KarlinBlkPtr kbp, BLAST_HSPPtr hsp, BLASTResultHspPtr result_hsp)
{
	if (result_hsp == NULL || hsp == NULL)
		return FALSE;

	result_hsp->ordering_method = hsp->ordering_method;
	result_hsp->number = hsp->num;
	result_hsp->score = hsp->score;
	result_hsp->bit_score = ((hsp->score*kbp->Lambda) - kbp->logK)/NCBIMATH_LN2;
	result_hsp->e_value = hsp->evalue;
	result_hsp->num_ident = hsp->num_ident;
	result_hsp->query_offset = hsp->query.offset;
	result_hsp->query_length = hsp->query.length;
	result_hsp->query_frame = hsp->query.frame;
	result_hsp->query_gapped_start = hsp->query.gapped_start;
	result_hsp->subject_offset = hsp->subject.offset;
	result_hsp->subject_length = hsp->subject.length;
	result_hsp->subject_frame = hsp->subject.frame;
	result_hsp->subject_gapped_start = hsp->subject.gapped_start;
	result_hsp->context = hsp->context;
	result_hsp->gap_info = hsp->gap_info;
	/* Not set in the other type of HSP? */
	result_hsp->hspset_cnt = 0;

	return TRUE;
}

Boolean LIBCALL
CopyResultHspToHSP(BLASTResultHspPtr result_hsp, BLAST_HSPPtr hsp)
{
	if (result_hsp == NULL || hsp == NULL)
		return FALSE;

	hsp->ordering_method = result_hsp->ordering_method;
	hsp->num = result_hsp->number;
	hsp->score = result_hsp->score;
	hsp->evalue = result_hsp->e_value;
	hsp->num_ident = result_hsp->num_ident;
	hsp->query.offset = result_hsp->query_offset;
	hsp->query.length = result_hsp->query_length;
	hsp->query.end = result_hsp->query_offset + result_hsp->query_length;
	hsp->query.frame = result_hsp->query_frame;
	hsp->query.gapped_start = result_hsp->query_gapped_start;
	hsp->subject.offset = result_hsp->subject_offset;
	hsp->subject.length = result_hsp->subject_length;
	hsp->subject.end = result_hsp->subject_offset + result_hsp->subject_length;
	hsp->subject.frame = result_hsp->subject_frame;
	hsp->subject.gapped_start = result_hsp->subject_gapped_start;
	hsp->context = result_hsp->context;

	return TRUE;
}

/* Same as FillInStdSegInfo, only taking BLAST_HSPPtr argument instead of
   BlastResultHspPtr */
StdSegPtr
BLASTHspToStdSeg(BlastSearchBlkPtr search, Int4 subject_length, BLAST_HSPPtr hsp, SeqIdPtr sip, Boolean reverse, SeqIdPtr gi_list)
{
   StdSegPtr ssp = NULL;
   BLASTResultHspPtr result_hsp = 
      (BLASTResultHspPtr) Malloc(sizeof(BLASTResultHsp));

   CopyHSPToResultHsp(search->sbp->kbp[search->first_context], 
                      hsp, result_hsp);
   ssp = FillInStdSegInfo(search, search->subject_id, subject_length, &ssp, 
                             result_hsp, sip, reverse, gi_list);
   MemFree(result_hsp);   
   return ssp;
}

/*
	Sort the HSP's by score.
*/

int LIBCALLBACK
score_compare_hsps(VoidPtr v1, VoidPtr v2)

{
    BLAST_HSPPtr hsp1, hsp2;    /* the HSPs to be compared */
    int result = 0;             /* the result of the comparison */

    hsp1 = *((BLAST_HSPPtr PNTR) v1);
    hsp2 = *((BLAST_HSPPtr PNTR) v2);

    /* Null HSPs are "greater" than any non-null ones, so they go to the end
       of a sorted list. */
    if (!hsp1 && !hsp2)
        return 0;
    else if (!hsp1)
        return 1;
    else if (!hsp2)
        return -1;

    if (0 == (result = BLAST_CMP(hsp2->score,          hsp1->score)) &&
        0 == (result = BLAST_CMP(hsp1->subject.offset, hsp2->subject.offset)) &&
        0 == (result = BLAST_CMP(hsp2->subject.end,    hsp1->subject.end)) &&
        0 == (result = BLAST_CMP(hsp1->query  .offset, hsp2->query  .offset))) {
        /* if all other test can't distinguish the HSPs, then the final
           test is the result */
        result = BLAST_CMP(hsp2->query.end, hsp1->query.end);
    }
    return result;
}

/*
	Function to look for the highest scoring window (of size HSP_MAX_WINDOW)
	in an HSP and return the middle of this.  Used by the gapped-alignment
	functions to start the gapped alignments.
*/

Int4 GetStartForGappedAlignment (BlastSearchBlkPtr search, BLAST_HSPPtr hsp, Uint1Ptr query, Uint1Ptr subject, Int4Ptr PNTR matrix)
{
    Int4 index1, max_offset, score, max_score, hsp_end;
    Uint1Ptr query_var, subject_var;
    Boolean positionBased = (search->positionBased && search->sbp->posMatrix);
    
    if (hsp->query.length <= HSP_MAX_WINDOW) {
        max_offset = hsp->query.offset + hsp->query.length/2;
        return max_offset;
    }

    hsp_end = hsp->query.offset + HSP_MAX_WINDOW;
    query_var = query + hsp->query.offset;
    subject_var = subject + hsp->subject.offset;
    score=0;
    if (!positionBased) {
       for (index1=hsp->query.offset; index1<hsp_end; index1++) {
          score += matrix[*query_var][*subject_var];
          query_var++; subject_var++;
       }
    } else {
       for (index1=hsp->query.offset; index1<hsp_end; index1++) {
          score += search->sbp->posMatrix[index1][*subject_var];
          query_var++; subject_var++;
       }
    }
    max_score = score;
    max_offset = hsp_end - 1;
    hsp_end = hsp->query.end - 
        MAX(0, hsp->query.length - hsp->subject.length);
    for (index1=hsp->query.offset + HSP_MAX_WINDOW; index1<hsp_end; index1++) {
        if (!positionBased) {
            score -= matrix[*(query_var-HSP_MAX_WINDOW)][*(subject_var-HSP_MAX_WINDOW)];
            score += matrix[*query_var][*subject_var];
        } else {
            score -= search->sbp->posMatrix[index1-HSP_MAX_WINDOW][*(subject_var-HSP_MAX_WINDOW)];
            score += search->sbp->posMatrix[index1][*subject_var];
        }
        if (score > max_score) {
            max_score = score;
            max_offset = index1;
        }
        query_var++; subject_var++;
    }
    if (max_score > 0)
       max_offset -= HSP_MAX_WINDOW/2;
    else 
       max_offset = hsp->query.offset;

    return max_offset;
}

/*
	Function to check that the highest scoring region in an HSP still gives a positive
	score.  This value was originally calcualted by GetStartForGappedAlignment but it
	may have changed due to the introduction of ambiguity characters.  Such a change
	can lead to 'strange' results from ALIGN. 
*/
Boolean
CheckStartForGappedAlignment (BlastSearchBlkPtr search, BLAST_HSPPtr hsp, Uint1Ptr query, Uint1Ptr subject, Int4Ptr PNTR matrix)
{
	Int4 index1, score, start, end, width;
	Uint1Ptr query_var, subject_var;
        Boolean positionBased = 
           (search->positionBased && search->sbp->posMatrix);

	width = MIN((hsp->query.gapped_start-hsp->query.offset), HSP_MAX_WINDOW/2);
	start = hsp->query.gapped_start - width;
	end = MIN(hsp->query.end, hsp->query.gapped_start + width);
	/* Assures that the start of subject is above zero. */
	if ((hsp->subject.gapped_start + start - hsp->query.gapped_start) < 0)
		start -= hsp->subject.gapped_start + (start - hsp->query.gapped_start);
	query_var = query + start;
	subject_var = subject + hsp->subject.gapped_start + (start - hsp->query.gapped_start);
	score=0;
	for (index1=start; index1<end; index1++)
	{
	  if (!positionBased)
	    score += matrix[*query_var][*subject_var];
	  else
	    score += search->sbp->posMatrix[index1][*subject_var];
	  query_var++; subject_var++;
	}
	if (score <= 0)
		return FALSE;

	return TRUE;
}
/*
	Gets the ratio used to change an evalue calculated with the subject
	sequence length to one with a db length.
*/

Nlm_FloatHi LIBCALL
GetDbSubjRatio(BlastSearchBlkPtr search, Int4 subject_length)
{
	Nlm_FloatHi db_subj_ratio;

	db_subj_ratio = (search->context_factor)*(search->dblen)/(subject_length);
        if (StringCmp(search->prog_name, "tblastn") == 0 ||
            StringCmp(search->prog_name, "tblastx") == 0 ||
            StringCmp(search->prog_name, "psitblastn") == 0)
	{
		db_subj_ratio *= 3;
	}
	
	return db_subj_ratio;
}

/* The following value should be divisible by 3, to make sure that frames stay
   the same when translations are restricted to partial sequence. */
#define SUBJECT_ADJUSTMENT 2100
SeqAlignPtr LIBCALL 
BlastGetGapAlgnTbckWithReaddb (BlastSearchBlkPtr search, Int4 hit_number, Boolean ordinal_number)

{
	BLASTResultHitlistPtr   result_hitlist;
	BioseqPtr subject_bsp;
	Boolean subject_allocated = FALSE;
	Int4 index1, subject_length, rev_subject_length;
	Int4 subject_start, subject_end;
	Int4 hsp_count;
	BLASTResultHspPtr hsp_array;
	SeqAlignPtr seqalign;
	SeqPortPtr spp;
	Uint1Ptr subject, rev_subject;

        result_hitlist = search->result_struct->results[hit_number];

        if (StringCmp(search->prog_name, "tblastn") == 0 ||
            StringCmp(search->prog_name, "psitblastn") == 0)
	{
		subject_bsp = readdb_get_bioseq(search->rdfp, result_hitlist->subject_id);
    		spp = SeqPortNew(subject_bsp, 0, -1, Seq_strand_plus, Seq_code_ncbi4na);
		/* make one longer to "protect" ALIGN. */
		subject = MemNew((1+subject_bsp->length)*sizeof(Uint1));
		hsp_array = result_hitlist->hsp_array;
		hsp_count = result_hitlist->hspcnt;
		for (index1=0; index1<hsp_count; index1++)
		{
			if (hsp_array[index1].subject_frame > 0)
			{ /* Get subsequence corresponding to this hsp. */
				Int4 offset;

				subject_start = 3*hsp_array[index1].subject_offset;
				subject_end = subject_start + 3*hsp_array[index1].subject_length;

				/* add SUBJECT_ADJUSTMENT bases to either end. */
				subject_start = MAX(subject_start - SUBJECT_ADJUSTMENT, 0);
				subject_end = MIN(subject_end + SUBJECT_ADJUSTMENT, subject_bsp->length);

				SeqPortSeek(spp, subject_start, SEEK_SET);

				for (offset=subject_start; offset<subject_end; offset++)
					subject[offset] = SeqPortGetResidue(spp);

				if (subject_start == 0 && subject_end == subject_bsp->length)
					break;    /* entire sequence has been fetched. */
			}
		}
		/* Gap character in last space. */
		subject[subject_bsp->length] = NULLB;
		subject_length = subject_bsp->length;
		spp = SeqPortFree(spp);

    		spp = SeqPortNew(subject_bsp, 0, -1, Seq_strand_minus, Seq_code_ncbi4na);
		/* make one longer to "protect" ALIGN. */
		rev_subject = MemNew((1+subject_bsp->length)*sizeof(Uint1));
		hsp_array = result_hitlist->hsp_array;
		hsp_count = result_hitlist->hspcnt;
		for (index1=0; index1<hsp_count; index1++)
		{
			if (hsp_array[index1].subject_frame < 0)
			{ /* Get subsequence corresponding to this hsp. */
				Int4 offset;

				subject_start = 3*hsp_array[index1].subject_offset;
				subject_end = subject_start + 3*hsp_array[index1].subject_length;

				/* add SUBJECT_ADJUSTMENT bases to either end. */
				subject_start = MAX(subject_start - SUBJECT_ADJUSTMENT, 0);
				subject_end = MIN(subject_end + SUBJECT_ADJUSTMENT, subject_bsp->length);

				SeqPortSeek(spp, subject_start, SEEK_SET);

				for (offset=subject_start; offset<subject_end; offset++)
					rev_subject[offset] = SeqPortGetResidue(spp);

				if (subject_start == 0 && subject_end == subject_bsp->length)
					break;    /* entire sequence has been fetched. */
			}
		}
		/* Gap character in last space. */
		rev_subject[subject_bsp->length] = NULLB;
		rev_subject_length = subject_bsp->length;
		spp = SeqPortFree(spp);
		subject_bsp = BioseqFree(subject_bsp);
		subject_allocated = TRUE;
	}
	else
	{
		subject_length = readdb_get_sequence(search->rdfp, result_hitlist->subject_id, (Uint1Ptr PNTR) &subject);
		rev_subject = NULL;
		rev_subject_length = 0;
	}

	seqalign = BlastGetGapAlgnTbck (search, hit_number,  FALSE, ordinal_number, subject, subject_length, rev_subject, rev_subject_length);
		
	if (subject_allocated)
	{
		subject = MemFree(subject);
		rev_subject = MemFree(rev_subject);
	}

	return seqalign;
}

int LIBCALLBACK
query_offset_compare_hsp(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

    if (h1 == NULL) {
        return (h2 == NULL) ? 0 : 1;
    } else if (h2 == NULL) {
      return -1;
    }

	if (h1->query.offset < h2->query.offset)
		return -1;
	if (h1->query.offset > h2->query.offset)
		return 1;

	if (h1->subject.offset < h2->subject.offset)
		return -1;
	if (h1->subject.offset > h2->subject.offset)
		return 1;

	return 0;
}

int LIBCALLBACK
query_end_compare_hsp(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

    if (h1 == NULL) {
        return (h2 == NULL) ? 0 : 1;
    } else if (h2 == NULL) {
      return -1;
    }

	if (h1->query.end < h2->query.end)
		return -1;
	if (h1->query.end > h2->query.end)
		return 1;

	if (h1->subject.end < h2->subject.end)
		return -1;
	if (h1->subject.end > h2->subject.end)
		return 1;

	return 0;
}
/*
	Check the gapped alignments for an overlap of two different alignments.
	A sufficient overlap is when two alignments have the same start values
	of have the same final values.

	The number of valid alignments remaining is returned.
*/

static Int4
CheckGappedAlignmentsForOverlap(BlastSearchBlkPtr search, BLAST_HSPPtr *hsp_array, Int4 hsp_count, Int2 frame)

{
	Int4 index1, index, increment;

	if (search == NULL || hsp_array == NULL || hsp_count == 0)
		return 0;

    	HeapSort(hsp_array, hsp_count, sizeof(BLAST_HSPPtr), query_offset_compare_hsp);
	index=0;
	increment=1;
	while (index < hsp_count-increment)
	{ /* Check if both HSP's start on or end on the same digonal. */
		if (hsp_array[index+increment] == NULL)
		{
			increment++;
			continue;
		}

		if (frame != 0 && hsp_array[index+increment]->subject.frame != frame)
                                break;

		if (hsp_array[index] && hsp_array[index]->query.offset == hsp_array[index+increment]->query.offset &&
			  hsp_array[index]->subject.offset == hsp_array[index+increment]->subject.offset &&
			    SIGN(hsp_array[index]->query.frame) == SIGN(hsp_array[index+increment]->query.frame))
		{
			if (hsp_array[index]->score > hsp_array[index+increment]->score)
			{
				hsp_array[index+increment] = 
                                   BLAST_HSPFree(hsp_array[index+increment]);
				increment++;
			}
			else
			{
				hsp_array[index] = 
                                   BLAST_HSPFree(hsp_array[index]);
				index++;
				increment = 1;
			}
		}
		else
		{
			index++;
			increment = 1;
		}
	}

    	HeapSort(hsp_array, hsp_count, sizeof(BLAST_HSPPtr), query_end_compare_hsp);
	index=0;
	increment=1;
	while (index < hsp_count-increment)
	{ /* Check if both HSP's start on or end on the same digonal. */
		if (hsp_array[index+increment] == NULL)
		{
			increment++;
			continue;
		}

		if (frame != 0 && hsp_array[index+increment]->subject.frame != frame)
                                break;

		if (hsp_array[index] &&
			hsp_array[index]->query.end == hsp_array[index+increment]->query.end &&
			  hsp_array[index]->subject.end == hsp_array[index+increment]->subject.end &&
			    SIGN(hsp_array[index]->query.frame) == SIGN(hsp_array[index+increment]->query.frame))
		{
			if (hsp_array[index]->score > hsp_array[index+increment]->score)
			{
				hsp_array[index+increment] = 
                                   BLAST_HSPFree(hsp_array[index+increment]);
				increment++;
			}
			else
			{
				hsp_array[index] = 
                                   BLAST_HSPFree(hsp_array[index]);
				index++;
				increment = 1;
			}
		}
		else
		{
			index++;
			increment = 1;
		}
	}

    	HeapSort(hsp_array,hsp_count,sizeof(BLAST_HSPPtr), score_compare_hsps);

	index1 = 0;
	for (index=0; index<hsp_count; index++)
	{
		if (hsp_array[index] != NULL)
			index1++;
	}


	return index1;

}

/*
	Sort the HSP's by frame.
*/

int LIBCALLBACK
frame_compare_hsp_m3(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (h1->subject.frame == -3 && h2->subject.frame != -3)
		return -1;
	if (h2->subject.frame == -3 && h1->subject.frame != -3)
		return 1;

	return 0;
}
int LIBCALLBACK
frame_compare_hsp_m2(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (h1->subject.frame == -2 && h2->subject.frame != -2)
		return -1;
	if (h2->subject.frame == -2 && h1->subject.frame != -2)
		return 1;

	return 0;
}

int LIBCALLBACK
frame_compare_hsp_m1(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (h1->subject.frame == -1 && h2->subject.frame != -1)
		return -1;
	if (h2->subject.frame == -1 && h1->subject.frame != -1)
		return 1;

	return 0;
}
int LIBCALLBACK
frame_compare_hsp_p1(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (h1->subject.frame == 1 && h2->subject.frame != 1)
		return -1;
	if (h2->subject.frame == 1 && h1->subject.frame != 1)
		return 1;

	return 0;
}
int LIBCALLBACK
frame_compare_hsp_p2(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (h1->subject.frame == 2 && h2->subject.frame != 2)
		return -1;
	if (h2->subject.frame == 2 && h1->subject.frame != 2)
		return 1;

	return 0;
}
int LIBCALLBACK
frame_compare_hsp_p3(VoidPtr v1, VoidPtr v2)

{
	BLAST_HSPPtr h1, h2;
	BLAST_HSPPtr PNTR hp1, PNTR hp2;

	hp1 = (BLAST_HSPPtr PNTR) v1;
	hp2 = (BLAST_HSPPtr PNTR) v2;
	h1 = *hp1;
	h2 = *hp2;

	if (h1->subject.frame == 3 && h2->subject.frame != 3)
		return -1;
	if (h2->subject.frame == 3 && h1->subject.frame != 3)
		return 1;

	return 0;
}
/*
	Engine to get the gapped scores from an array of HSP's.
*/
static BLAST_HSPPtr PNTR
BlastGappedScoreInternal(BlastSearchBlkPtr search, Uint1Ptr subject, Int4 subject_length, GapAlignBlkPtr gap_align, BLAST_HSPPtr *hsp_array, Int4Ptr hspcnt, Int4Ptr hspcnt_max, Int4 hspmax, Int2 frame)

{
	BLAST_HSPPtr hsp, hsp1=NULL;
	BLAST_HSPPtr PNTR hsp_array_new;
	BLAST_HSP_helperPtr helper;
	Boolean hsp_start_is_contained, hsp_end_is_contained;
	Int4 hsp_cnt=0, index, index1;
	Int4 max_offset = 0, next_offset;
	Int4 query_num; /* AM: Added to support query concatenation */

	/* helper contains most frequently used information to speed up access. */
	helper = Malloc((*hspcnt)*sizeof(BLAST_HSP_helper));
	for (index=0; index<(*hspcnt); index++)
	{
		hsp_start_is_contained = FALSE;
		hsp_end_is_contained = FALSE;
		hsp = hsp_array[index];
	/* This prefetches this value for the test below. */
		next_offset = hsp->query.offset;

		if (frame != 0 && hsp->subject.frame != frame)
			break;

		for (index1=0; index1<index; index1++)
		{
			hsp_start_is_contained = FALSE;
			hsp_end_is_contained = FALSE;

			hsp1 = hsp_array[index1];
			if (hsp1 == NULL)
				continue;

			/* Check with the helper array whether further
				tests are warranted.  Having only two ints
				in the helper array speeds up access. */
			if (helper[index1].qoffset <= next_offset &&
				helper[index1].qend >= next_offset)
			{
			   if (CONTAINED_IN_HSP(hsp1->query.offset, hsp1->query.end, hsp->query.offset, hsp1->subject.offset, hsp1->subject.end, hsp->subject.offset) == TRUE)

			   {	/* Check that it's on diff. strands. */
				if (SIGN(hsp1->query.frame) == SIGN(hsp->query.frame) &&
					SIGN(hsp1->subject.frame) == SIGN(hsp->subject.frame))
					hsp_start_is_contained = TRUE;
			   }
			   if (hsp_start_is_contained && CONTAINED_IN_HSP(hsp1->query.offset, hsp1->query.end, hsp->query.end, hsp1->subject.offset, hsp1->subject.end, hsp->subject.end) == TRUE)

			   {	/* Check that it's on diff. strands. */
				if (SIGN(hsp1->query.frame) == SIGN(hsp->query.frame) &&
					SIGN(hsp1->subject.frame) == SIGN(hsp->subject.frame))
					hsp_end_is_contained = TRUE;
				if (hsp_start_is_contained && hsp_end_is_contained && hsp->score <= hsp1->score)
				{
					break;
				}
			   }
			}
		}
                
		if (hsp_start_is_contained == FALSE ||
                    hsp_end_is_contained   == FALSE || 
                    (hsp1 == NULL) || (hsp->score > hsp1->score))
		{
			gap_align->include_query = 0;

                        if(!search->pbp->is_ooframe) {
                            max_offset = GetStartForGappedAlignment(search, hsp, search->context[hsp->context].query->sequence, subject, search->sbp->matrix);
                        }
                        
#ifdef BLAST_COLLECT_STATS
			search->real_gap_number_of_hsps++;
#endif
			Nlm_MemSet((VoidPtr) &(hsp_array[index]->hsp_link), 0, sizeof(BLAST_HSP_LINK));
			hsp_array[index]->linked_set = FALSE;
			hsp_array[index]->start_of_chain = FALSE;
			hsp_array[index]->num = 0;
			hsp_array[index]->xsum = 0.0;

                        if(search->pbp->is_ooframe) {
                            gap_align->is_ooframe = TRUE;
                            gap_align->query = subject;
                            if(hsp->query.frame > 0) {
                                gap_align->subject = search->query_dnap[0]->sequence;
                                gap_align->subject_length = search->query_dnap[0]->length;
                            } else {
                                gap_align->subject = search->query_dnap[1]->sequence;
                                gap_align->subject_length = search->query_dnap[1]->length;
                            }

                            gap_align->query_length = subject_length;

                            gap_align->q_start = hsp->subject.offset;
                            gap_align->s_start = hsp->query.offset; 

                            hsp->query.gapped_start = gap_align->s_start;
                            hsp->subject.gapped_start = gap_align->q_start;

                        } else {
                            gap_align->query = search->context[hsp->context].query->sequence;
                            gap_align->query_length = search->context[hsp->context].query->length;
                            gap_align->q_start = max_offset;
                            gap_align->s_start = 
                               (hsp->subject.offset - hsp->query.offset) + max_offset;
                            hsp->query.gapped_start = gap_align->q_start;
                            hsp->subject.gapped_start = gap_align->s_start;

                               gap_align->subject = subject;
                               gap_align->subject_length = subject_length;
                        }
                                               
                        /* For out-of frame gapping - query is protein
                           and subject is DNA translated into 3 frames */

			PerformGappedAlignment(gap_align);

                        if(search->pbp->is_ooframe) {
                            hsp->query.offset = gap_align->subject_start;
                            hsp->subject.offset = gap_align->query_start;
                            /* The end is one further for BLAST than for the gapped align. */
                            hsp->query.end = gap_align->subject_stop + 1;
                            hsp->subject.end = gap_align->query_stop + 1;
                        } else {
                            hsp->query.offset = gap_align->query_start;
                            hsp->query.end = gap_align->query_stop + 1;
                            hsp->subject.offset = gap_align->subject_start;
                            hsp->subject.end = gap_align->subject_stop + 1;
                            /* The end is one further for BLAST than for the gapped align. */
                        }

                        hsp->query.length = hsp->query.end - hsp->query.offset;
                        hsp->subject.length = hsp->subject.end - hsp->subject.offset;
			hsp->score = gap_align->score;
            if( hsp->score >= search->pbp->cutoff_s1 ) {
                /* AM: Changed to support query concatenation */
                if( !search->mult_queries )
                    hsp->evalue =
                        BlastKarlinStoE_simple(hsp->score,
                                               search->sbp->
                                               kbp_gap[search->first_context],
                                               search->searchsp_eff);
                else {
                    query_num = GetQueryNum( search->mult_queries,
                                             hsp->query.offset,
                                             hsp->query.end,
                                             hsp->query.frame );
                    hsp->evalue =
                        BlastKarlinStoE_simple(hsp->score,
                                               search->sbp->
                                               kbp_gap[search->first_context],
                                               search->mult_queries->
                                               SearchSpEff[query_num]);
                }

                hsp_cnt++;
                /* Fill in the helper structure. */
                helper[index].qoffset = hsp->query.offset;
                helper[index].qend = hsp->query.end;
            } else {
                /* Score of the gapped extension is below the required
                   cutoff, delete this hsp */
                hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
            }
        }
		else
		{ /* Contained within another HSP, delete. */
			hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
		}
	}
	helper = MemFree(helper);

	hsp_cnt = CheckGappedAlignmentsForOverlap(search, hsp_array, *hspcnt, frame);

	if (hsp_cnt < (*hspcnt))
	{
/* Save HSP's again, discarding those that have been NULLed out. */
		hsp_array_new = MemNew(hspmax*sizeof(BLAST_HSPPtr));
		index1 = 0;
		for (index=0; index<(*hspcnt_max); index++)
		{
			if (hsp_array[index] != NULL)
			{
				hsp_array_new[index1] = hsp_array[index];
				index1++;
			}
		}

		hsp_array = MemFree(hsp_array);

		*hspcnt = index1;	
		*hspcnt_max = index1;	
		hsp_array = hsp_array_new;
	}
	*hspcnt = hsp_cnt;

	return hsp_array;
}

/*
	Engine to get the gapped scores from an array of HSP's.
*/
static Boolean
BlastNtGappedScoreInternal(BlastSearchBlkPtr search, Uint1Ptr subject, Int4 subject_length, GapAlignBlkPtr gap_align, BLAST_HSPPtr *hsp_array, Int4Ptr hspcnt, Int4Ptr hspcnt_max, Int4 hspmax)

{
	BLAST_HSPPtr hsp, hsp1=NULL;
	BLAST_HSP_helperPtr helper;
	Boolean hsp_start_is_contained, hsp_end_is_contained;
	Int4 hsp_cnt=0, index, index1, next_offset, query_length;
	/* AM: Added to support query concatenation. */
	Int4 query_num;

	/* helper contains most frequently used information to speed up access. */
        helper = Malloc((*hspcnt)*sizeof(BLAST_HSP_helper));

	for (index=0; index<(*hspcnt); index++)
	{
		hsp_start_is_contained = FALSE;
		hsp_end_is_contained = FALSE;
		hsp = hsp_array[index];
        /* This prefetches this value for the test below. */
                next_offset = hsp->query.offset;

		for (index1=0; index1<index; index1++)
		{
			hsp_start_is_contained = FALSE;
			hsp_end_is_contained = FALSE;

			hsp1 = hsp_array[index1];
			if (hsp1 == NULL)
				continue;


                        /* Check with the helper array whether further
                                tests are warranted.  Having only two ints
                                in the helper array speeds up access. */
                        if (helper[index1].qoffset <= next_offset &&
                                helper[index1].qend >= next_offset)
                        {

			if (CONTAINED_IN_HSP(hsp1->query.offset, hsp1->query.end, hsp->query.offset, hsp1->subject.offset, hsp1->subject.end, hsp->subject.offset) == TRUE)
			{	/* Check that it's on diff. strands. */
					hsp_start_is_contained = TRUE;
			}
			if (hsp_start_is_contained && CONTAINED_IN_HSP(hsp1->query.offset, hsp1->query.end, hsp->query.end, hsp1->subject.offset, hsp1->subject.end, hsp->subject.end) == TRUE)
			{	/* Check that it's on diff. strands. */
					hsp_end_is_contained = TRUE;
			}
			if (hsp_start_is_contained && hsp_end_is_contained && hsp->score <= hsp1->score)
			{
				break;
			}
			}
		}

		if (hsp_start_is_contained == FALSE ||
			 hsp_end_is_contained == FALSE || 
				hsp->score > hsp1->score)
		{
			gap_align->include_query = 0;
#ifdef BLAST_COLLECT_STATS
			search->real_gap_number_of_hsps++;
#endif
/*
			Nlm_MemSet((VoidPtr) &(hsp_array[index]->hsp_link), 0, sizeof(BLAST_HSP_LINK));
			hsp_array[index]->linked_set = FALSE;
			hsp_array[index]->start_of_chain = FALSE;
			hsp_array[index]->num = 0;
			hsp_array[index]->sumscore = 0;
*/

			gap_align->query = search->context[hsp->context].query->sequence;
			gap_align->query_length = search->context[hsp->context].query->length;
			gap_align->q_start = hsp->query.gapped_start;
			gap_align->s_start = hsp->subject.gapped_start;

                        gap_align->subject = subject;
                        gap_align->subject_length = subject_length;

			if (hsp->subject.gapped_start >= 0) {
                           if (!PerformNtGappedAlignment(gap_align))
                              return FALSE;
                        }

                        query_length =
                           search->query_context_offsets[search->first_context+1] - 1;
                        if (gap_align->query_start / query_length != 
                            (gap_align->query_stop - 1) / query_length) {
                           if (gap_align->q_start < query_length) {
                              gap_align->subject_stop -= 
                                 (gap_align->query_stop - query_length + 1);
                              gap_align->query_stop = query_length - 1;
                           } else {
                              gap_align->subject_start += 
                                 (query_length + 1 - gap_align->query_start);
                              gap_align->query_start = query_length + 1;
                           }
                        }
			hsp->query.offset = gap_align->query_start;
			hsp->subject.offset = gap_align->subject_start;
	/* The end is one further for BLAST than for the gapped align. */
			hsp->query.end = gap_align->query_stop + 1;
			hsp->subject.end = gap_align->subject_stop + 1;
			hsp->query.length = hsp->query.end - hsp->query.offset;
			hsp->subject.length = hsp->subject.end - hsp->subject.offset;
			hsp->score = gap_align->score;
/* TLM */
                        /* AM: Changed to support query concatenation. */
			if( !search->mult_queries )
			  hsp->evalue = BlastKarlinStoE_simple(hsp->score, search->sbp->kbp[search->first_context], search->searchsp_eff);
                        else
			{
			  query_num = GetQueryNum( search->mult_queries,
			                           hsp->query.offset,
						   hsp->query.end,
						   hsp->query.frame );
	       	          hsp->evalue = BlastKarlinStoE_simple( hsp->score,
		                                                search->sbp->kbp[search->first_context],
						                search->mult_queries->SearchSpEff[query_num] );
			}

			hsp_cnt++;
                        /* Fill in the helper structure. */
                        helper[index].qoffset = hsp->query.offset;
                        helper[index].qend = hsp->query.end;
		}
		else
		{ /* Contained within another HSP, delete. */
			hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
		}
	}
	helper = MemFree(helper);

/*
	hsp_cnt = CheckGappedAlignmentsForOverlap(search, hsp_array, *hspcnt, 0);
*/

	if (hsp_cnt < (*hspcnt))
	{
/* Save HSP's again, discarding those that have been NULLed out. */
		index1 = 0;
		for (index=0; index<(*hspcnt); index++)
		{
			if (hsp_array[index] != NULL)
			{
				hsp_array[index1] = hsp_array[index];
				index1++;
			}
		}

	}
	*hspcnt = hsp_cnt;

	return TRUE;
}

/*
	Loads the HSP's into the BlastHitRangePtr.
*/
static Boolean
BlastHitRangeLoad (BlastSearchBlkPtr search, BLAST_HSPPtr *hsp_array, Int4 hspcnt, BlastHitRangePtr bhrp)

{
	BlastDoubleInt4Ptr tmp;
	BLAST_HSPPtr hsp;
	Int4 index, query_length;

	if (bhrp->current+hspcnt > bhrp->total)
		return FALSE;

	if (hspcnt <= 0)
		return TRUE;

	tmp = bhrp->range_list;

	tmp += bhrp->current;
	
	for (index=0; index<hspcnt; index++)
	{
		hsp = hsp_array[index];
		query_length = search->context[hsp->context].query->length;
		if (hsp->query.frame >= 0)
		{
			tmp->gi = hsp->query.offset;
			tmp->ordinal_id = hsp->query.end - 1;
		}
		else
		{
			tmp->gi = query_length - hsp->query.end;
			tmp->ordinal_id = query_length - hsp->query.offset - 1;
		}
		tmp++;
	}

	bhrp->current += hspcnt;

	return TRUE;
}

static void rpsFilterSequenceByMask(ValNodePtr mask, Uint1Ptr sequence, Int4 length, Int4 frame, Int4 dna_length)
{
    SeqLocPtr filter_slp = NULL;
    ValNodePtr vnp;
    
    if(mask == NULL)
        return;
    
    for(vnp = mask; vnp != NULL; vnp = vnp->next) {
        
        if(vnp->choice == FrameToDefine(frame)) {
            filter_slp = (SeqLocPtr) vnp->data.ptrvalue;
            break;
        }
    }
    
    if(filter_slp != NULL)
        BlastMaskTheResidues(sequence+1, length, 21, filter_slp, FALSE, 0);

    /* BlastConvertProteinSeqLoc(filter_slp, frame, dna_length); */
    
    return;
}

static void BLASTCheckHSPInclusion(BLAST_HSPPtr *hsp_array, Int4 hspcnt, 
                                   Boolean is_ooframe)
{
    Int4 index, index1;
    BLAST_HSPPtr hsp, hsp1;
    
    for (index = 0; index < hspcnt; index++) {
        
        hsp = hsp_array[index];
        
        if (hsp == NULL)
            continue;
        
        for (index1 = 0; index1 < index; index1++) {
            
            hsp1 = hsp_array[index1];
            
            if (hsp1 == NULL)
                continue;
            
            if(is_ooframe) {
                if (SIGN(hsp1->query.frame) != SIGN(hsp->query.frame))
                    continue;
            } else {
                if (hsp->context != hsp1->context)
                    continue;
            }
            
            /* Check of the start point of this HSP */
            if (CONTAINED_IN_HSP(hsp1->query.offset, hsp1->query.end, hsp->query.offset, hsp1->subject.offset, hsp1->subject.end, hsp->subject.offset) == TRUE) {
                /* Check of the end point of this HSP */
                if (CONTAINED_IN_HSP(hsp1->query.offset, hsp1->query.end, hsp->query.end, hsp1->subject.offset, hsp1->subject.end, hsp->subject.end) == TRUE) {
                    /* Now checking correct strand */
                    if (SIGN(hsp1->query.frame) == SIGN(hsp->query.frame) &&
                        SIGN(hsp1->subject.frame) == SIGN(hsp->subject.frame)){
                        
                        /* If we come here through all these if-s - this
                           mean, that current HSP should be removed. */

                        if(hsp_array[index] != NULL) {
                           hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
                           break;
                        }
                    }
                }
            }
        }
    }
    return;
}


/*
	Take a BLAST_HSPPtr (array of HSP's) and get a traceback for them.
*/

Int4
RealBlastGetGappedAlignmentTraceback(BlastSearchBlkPtr search, Uint1Ptr subject, Int4 subject_length, Uint1Ptr rev_subject, Int4 rev_subject_length, SeqIdPtr subject_id, BLAST_HSPPtr *hsp_array, Int4 hspcnt, SeqAlignPtr *head, BlastHitRangePtr bhrp, Int4 min_score_to_keep, Boolean reverse, Int4 ordinal_id, Boolean do_traceback)

{
    BLAST_HSPPtr hsp, hsp1, hsp2;
    BLAST_ParameterBlkPtr pbp;
    BLASTResultHsp       	result_hsp;
    Boolean hsp_start_is_contained, hsp_end_is_contained, keep;
    Boolean do_not_do;
    GapAlignBlkPtr gap_align;
    Int4 new_hspcnt=0;
    Int4 index, index1, index2, query_length, max_offset;
    Int4Ptr translated_subject_length=NULL; 
    Int4Ptr translated_subject_length_orig=NULL;
    SeqAlignPtr seqalign, seqalign_var, *seqalign_array;
    Uint1Ptr query, PNTR translated_subject=NULL, PNTR translated_subject_orig=NULL;
    ValNodePtr gi_list=NULL;
    BLAST_HitListPtr tmp_hitlist;
    BLAST_HitListPtr real_hitlist;
    SeqIdPtr query_id, new_subject_seqid = NULL, seqid_tmp;
    Int4 max_start = MAX_DBSEQ_LEN / 2, start_shift;
    Int4 align_length;
    Int4 query_num; /* AM: Added to support query concatenation. */
    Boolean partial_translation;
    Int4 translation_length;

    pbp = search->pbp;
    MemSet(&result_hsp, 0, sizeof(BLASTResultHsp));
    
    seqalign=NULL;
    if (do_traceback)
        seqalign_array = MemNew(hspcnt*sizeof(SeqAlignPtr));
    
    if (search->gap_align == NULL) {
        search->gap_align = GapAlignBlkNew(1, 1);
    }

    gap_align = search->gap_align;
    
    gi_list = BlastGetAllowedGis(search, ordinal_id, &new_subject_seqid);
    
#if 1
    if (gi_list) {
        /* change subject's gi with this 'use_this_gi' gi */
        subject_id->data.intvalue = gi_list->data.intvalue;
    }
#endif
    
    gap_align->is_ooframe = pbp->is_ooframe; /* For OOF: blastx and tblastn */
    gap_align->shift_pen = pbp->shift_pen; 
    
    gap_align->discontinuous = pbp->discontinuous;
    gap_align->positionBased = 
       (search->positionBased && search->sbp->posMatrix);
    gap_align->gap_open = pbp->gap_open;
    gap_align->gap_extend = pbp->gap_extend;
    gap_align->decline_align = pbp->decline_align;
    gap_align->x_parameter = pbp->gap_x_dropoff_final;
    gap_align->matrix = search->sbp->matrix;
    gap_align->posMatrix = search->sbp->posMatrix;
    partial_translation = (subject_length > SUBJECT_ADJUSTMENT);

    for (index=0; index<hspcnt; index++) {
        hsp_start_is_contained = FALSE;
        hsp_end_is_contained = FALSE;
        hsp = hsp_array[index];
        
        for (index1=0; index1<index; index1++) {
            hsp_start_is_contained = FALSE;
            hsp_end_is_contained = FALSE;
            
            hsp1 = hsp_array[index1];
            if (hsp1 == NULL)
                continue;
            
            if(pbp->is_ooframe) {
                if (SIGN(hsp1->query.frame) != SIGN(hsp->query.frame))
                    continue;
            } else {
                if (hsp->context != hsp1->context)
                    continue;
            }

            if (CONTAINED_IN_HSP(hsp1->query.offset, hsp1->query.end, hsp->query.offset, hsp1->subject.offset, hsp1->subject.end, hsp->subject.offset) == TRUE) {
		if (SIGN(hsp1->query.frame) == SIGN(hsp->query.frame) &&
                    SIGN(hsp1->subject.frame) == SIGN(hsp->subject.frame))
                    hsp_start_is_contained = TRUE;
            }
            if (CONTAINED_IN_HSP(hsp1->query.offset, hsp1->query.end, hsp->query.end, hsp1->subject.offset, hsp1->subject.end, hsp->subject.end) == TRUE) {
		if (SIGN(hsp1->query.frame) == SIGN(hsp->query.frame) &&
                    SIGN(hsp1->subject.frame) == SIGN(hsp->subject.frame))
                    hsp_end_is_contained = TRUE;
            }
            if (hsp_start_is_contained && hsp_end_is_contained && hsp->score <= hsp1->score) {
                break;
            }
        }
        
        do_not_do = FALSE;
        /* Check whether this part of query has already been covered. */
        /* Commented out by TLM as this seems buggy. 
           if (bhrp) {
           total = bhrp->current;
           for (index1=0; index1<total; index1++) {
           if (hsp->query.offset >= bhrp->range_list_pointer[index1]->gi &&
           hsp->query.end <= bhrp->range_list_pointer[index1]->ordinal_id) {
           do_not_do = TRUE;
           break;
           }
           }
           }
        */
        if (do_not_do == FALSE && (hsp_start_is_contained == FALSE || hsp_end_is_contained == FALSE || 
                                   hsp->score > hsp1->score)) {
            query = (Uint1Ptr) search->context[hsp->context].query->sequence;
            query_length = search->context[hsp->context].query->length;
            
            gap_align->include_query = 0;
            
            
            if(search->pbp->is_ooframe) {
                gap_align->is_ooframe = TRUE;
                gap_align->query = subject;
                
                if(hsp->query.frame > 0) {
                    gap_align->subject = search->query_dnap[0]->sequence;
                    gap_align->subject_length = search->query_dnap[0]->length;
                } else {
                    gap_align->subject = search->query_dnap[1]->sequence;
                    gap_align->subject_length = search->query_dnap[1]->length;
                }

                gap_align->query_frame = hsp->subject.frame;
                gap_align->subject_frame = ContextToFrame(search, hsp->context);
                gap_align->query_length = subject_length;
            } else {
                gap_align->query_frame = ContextToFrame(search, hsp->context);
                gap_align->query = query;
                
                gap_align->subject_frame = hsp->subject.frame;
                gap_align->subject = subject;
                
                gap_align->query_length = query_length;
                gap_align->subject_length = subject_length;
            }
            
            gap_align->translate1 = FALSE;
            gap_align->translate2 = FALSE;
            if (StringCmp(search->prog_name, "blastx") == 0) {
                gap_align->translate1 = TRUE;
                gap_align->translate2 = FALSE;
            }

            start_shift = 0;

            if (StringCmp(search->prog_name, "tblastn") == 0 ||
                StringCmp(search->prog_name, "psitblastn") == 0) {
                gap_align->translate1 = FALSE;
                gap_align->translate2 = TRUE;
                if (translated_subject == NULL) {
                    translated_subject_orig = MemNew(8*sizeof(Uint1Ptr));
                    translated_subject = translated_subject_orig + 3;
                    translated_subject_length_orig = MemNew(8*sizeof(Int4));
                    translated_subject_length = translated_subject_length_orig + 3;
                } 
                if (partial_translation) {
                   translated_subject[hsp->subject.frame] = 
                      MemFree(translated_subject[hsp->subject.frame]);
                   /* NB: since SUBJECT_ADJUSTMENT is divisible by 3, the frame
                      will remain the same.
                   */
                   start_shift = 
                      MAX(0, 3*hsp->subject.offset - SUBJECT_ADJUSTMENT);
                   translation_length =
                      MIN(3*hsp->subject.end + SUBJECT_ADJUSTMENT, subject_length)
                      - start_shift;
                   if (hsp->subject.frame > 0) {
                      translated_subject[hsp->subject.frame] =
                         GetTranslation(subject+start_shift, translation_length, hsp->subject.frame, &translated_subject_length[hsp->subject.frame], search->db_genetic_code);
                   } else {
                      translated_subject[hsp->subject.frame] =
                         GetTranslation(rev_subject+start_shift, translation_length, hsp->subject.frame, &translated_subject_length[hsp->subject.frame], search->db_genetic_code);
                   }
                   /* Below, the start_shift will be used for the protein
                      coordinates, so need to divide it by 3 */
                   start_shift /= CODON_LENGTH;
                   hsp->subject.offset -= start_shift;
                   hsp->subject.gapped_start -= start_shift;

                } else if (translated_subject[hsp->subject.frame] == NULL) {
                    if (hsp->subject.frame > 0) {
                        translated_subject[hsp->subject.frame] =
                            GetTranslation(subject, subject_length, hsp->subject.frame, &translated_subject_length[hsp->subject.frame], search->db_genetic_code);
                    } else {
                        translated_subject[hsp->subject.frame] =
                            GetTranslation(rev_subject, rev_subject_length, hsp->subject.frame, &translated_subject_length[hsp->subject.frame], search->db_genetic_code);
                    }
                    /* For RPS Blast filtering if needed */
                    if(search->pbp->is_rps_blast && search->pbp->filter_string != NULL && StringICmp(search->pbp->filter_string, "F")) {
                        rpsFilterSequenceByMask(search->mask, translated_subject[hsp->subject.frame], translated_subject_length[hsp->subject.frame], hsp->subject.frame, (hsp->subject.frame > 0) ? subject_length : rev_subject_length);
                    }
                }
                
                gap_align->subject = translated_subject[hsp->subject.frame] + 1;
                gap_align->subject_length = translated_subject_length[hsp->subject.frame];
            }
            
            /* these should both only be zero for blastn. */
            if (!search->pbp->is_ooframe &&
                (((hsp->query.gapped_start == 0 && hsp->subject.gapped_start == 0) ||
                CheckStartForGappedAlignment(search, hsp, gap_align->query, gap_align->subject, search->sbp->matrix) == FALSE))) {
                max_offset = GetStartForGappedAlignment(search, hsp, gap_align->query, gap_align->subject, search->sbp->matrix);
                gap_align->q_start = max_offset;
                gap_align->s_start = (hsp->subject.offset - hsp->query.offset) + max_offset;
                hsp->query.gapped_start = gap_align->q_start;
                hsp->subject.gapped_start = gap_align->s_start;
            } else {
                if(search->pbp->is_ooframe) {
                    /* Code above should be investigated for possible
                       optimization for OOF */
                    gap_align->q_start = hsp->subject.gapped_start;
                    gap_align->s_start = hsp->query.gapped_start;
                    gap_align->subject_start = 0;
                    gap_align->query_start = 0;
                } else {
                    gap_align->q_start = hsp->query.gapped_start;
                    gap_align->s_start = hsp->subject.gapped_start;
                }
            }

            if (search->prog_number == blast_type_blastn) {
               /* For blastn, use only part of a long subject sequence, 
                  because the placeholders for the gapped alignment 
                  information have only been allocated for at most a 
                  certain length */
               if (gap_align->s_start > max_start) {
                  start_shift = (gap_align->s_start / max_start) * max_start;
                  gap_align->subject = gap_align->subject + start_shift;
                  
                  gap_align->s_start %= max_start;
               } else
                  start_shift = 0;
               
               gap_align->subject_length =
                  MIN(gap_align->subject_length - start_shift, 
                      gap_align->s_start + hsp->subject.length + max_start);
            }

            if (do_traceback) {
               if (!search->pbp->mb_params || 
                   search->pbp->mb_params->use_dyn_prog) {
                  PerformGappedAlignmentWithTraceback(gap_align);
               } else {
                  PerformGreedyAlignmentWithTraceback(gap_align, search->abmp,
                                                      search->sbp);
               }
            } else {
                PerformGappedAlignment(gap_align);
            }

            if (gap_align->score >= min_score_to_keep) {
               
                if(search->pbp->is_ooframe) {
                    hsp->query.offset = gap_align->subject_start + start_shift;
                    hsp->subject.offset = gap_align->query_start;
                    /* The end is one further for BLAST than for the gapped align. */
                    hsp->query.end = gap_align->subject_stop + 1 + start_shift;
                    hsp->subject.end = gap_align->query_stop + 1;
                } else {
                    hsp->query.offset = gap_align->query_start;
                    hsp->subject.offset = gap_align->subject_start + start_shift;
                    /* The end is one further for BLAST than for the gapped align. */
                    hsp->query.end = gap_align->query_stop + 1;
                    hsp->subject.end = gap_align->subject_stop + 1 + start_shift;
                }
                
                if (gap_align->edit_block && start_shift > 0) {
                   gap_align->edit_block->start2 += start_shift;
                   gap_align->edit_block->length2 += start_shift;
                }
                hsp->query.length = hsp->query.end - hsp->query.offset;
                hsp->subject.length = hsp->subject.end - hsp->subject.offset;
                hsp->score = gap_align->score;

                if (do_traceback) {
                    hsp->gap_info = gap_align->edit_block;
                }

		keep = TRUE;
                /* If greedy alignment was used for traceback, we still need
                   to reevaluate the score with ambiguity information */
                if (search->pbp->mb_params &&
                    !search->pbp->mb_params->use_dyn_prog &&
                    ReevaluateScoreWithAmbiguities(search, subject, hsp)) {
                   /* HSP became below the cutoff after reevaluation */
                   keep = FALSE;
                }

                if (keep && (search->prog_number == blast_type_blastp ||
                    search->prog_number == blast_type_blastn)) {
                   if (search->pbp->mb_params) {
                      FloatHi searchsp_eff = (FloatHi) search->dblen_eff *
                         (FloatHi) search->context[hsp->context].query->effective_length;
                      
                      hsp->evalue = BlastKarlinStoE_simple(hsp->score,
                                                           search->sbp->kbp_gap[hsp->context],
                                                           searchsp_eff);
                   } else {
		      /* AM: Changed to support query concatenation. */
		      if( !search->mult_queries )
                        hsp->evalue = BlastKarlinStoE_simple(hsp->score,
                                                           search->sbp->kbp_gap[search->first_context], search->searchsp_eff);
                      else
		      {
		        /* AM: First determine which query to use, then use the
			       corresponding SearchSpEff element in the call to 
			       BlastKarlinStoE_simple() */
			query_num = GetQueryNum( search->mult_queries,
			                         hsp->query.offset,
						 hsp->query.end,
						 hsp->query.frame );
			hsp->evalue = BlastKarlinStoE_simple( hsp->score,
			                                      search->sbp->kbp_gap[search->first_context],
							      search->mult_queries->SearchSpEff[query_num] );
		      }
                   }
                   /*hsp->pvalue = BlastKarlinEtoP(hsp->evalue);*/
		    if (hsp->evalue > search->pbp->cutoff_e) /* put in for comp. based stats. */
			keep = FALSE;
#if 0 /* Not needed any more: percent identity computed for all programs in 
         the same way now, just below */
                    if (search->pbp->mb_params &&
                        search->pbp->mb_params->use_dyn_prog) {
                       search->subject->sequence_start = gap_align->subject - 1;
                       if (MegaBlastGetHspPercentIdentity(search, hsp) < 
                           search->pbp->mb_params->perc_identity) {
                          keep = FALSE;
                       }
                       search->subject->sequence_start = NULL;
                    }
#endif
                }

                if (keep) {
                   if (search->pbp->is_ooframe) {
                      OOFBlastHSPGetNumIdentical(gap_align->query, 
                                                 gap_align->subject-start_shift, hsp, NULL, 
                                                 &hsp->num_ident, &align_length);
                   } else {
                      search->subject->sequence_start = 
                         gap_align->subject - start_shift - 1;
                      BlastHSPGetNumIdentical(search, hsp, NULL, &hsp->num_ident,
                                              &align_length);
                   }
                   if (search->pbp->mb_params &&
                       search->pbp->mb_params->use_dyn_prog) {
                      if (hsp->num_ident * 100 < 
                          align_length * search->pbp->mb_params->perc_identity) {
                         keep = FALSE;
                      }
                   }
                   search->subject->sequence_start = NULL;

                   if (search->pbp->scalingFactor != 0.0 && search->pbp->scalingFactor != 1.0)
                      /* Scale down score for blastp and tblastn. */
                      hsp->score = (hsp->score+(0.5*search->pbp->scalingFactor))/search->pbp->scalingFactor;
                   
				/* only one alignment considered for blast[np]. */
				/* This may be changed by LinkHsps for blastx or tblastn. */
                   hsp->num = 1;
                   if ((search->prog_number == blast_type_tblastn ||
                        search->prog_number == blast_type_psitblastn) && 
                       search->pbp->longest_intron > 0) 
                      hsp->evalue = BlastKarlinStoE_simple(hsp->score, 
                                       search->sbp->kbp_gap[search->first_context], search->searchsp_eff);
                }

                for (index2=0; index2<index && keep == TRUE; index2++) {
                    hsp2 = hsp_array[index2];
                    if (hsp2 == NULL)
                        continue;
                    
                    /* Check if both HSP's start or end on the same diagonal (and are on same strands). */
                    if (((hsp->query.offset == hsp2->query.offset &&
                          hsp->subject.offset == hsp2->subject.offset) ||
                         (hsp->query.end == hsp2->query.end &&
                          hsp->subject.end == hsp2->subject.end))  &&
                        hsp->context == hsp2->context &&
			hsp->subject.frame == hsp2->subject.frame) {
                        if (hsp2->score > hsp->score) {
                            keep = FALSE;
                            break;
                        } else {
                            new_hspcnt--;
                            if (do_traceback) {
                                seqalign_array[index2] = 
                                   SeqAlignFree(seqalign_array[index2]);
                            }
                            hsp_array[index2] = 
                               BLAST_HSPFree(hsp_array[index2]);
                        }
                    }
                }
                
                if (keep) {
                    new_hspcnt++;
                } else {
                   hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
                }
            } else {	/* Should be kept? */
                hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
            }
        } else { /* Contained within another HSP, delete. */
            hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
        }
    }
    if (search->pbp->scalingFactor != 0.0 && search->pbp->scalingFactor != 1.0)
    {	/* Rescale Lambda. */
		search->sbp->kbp_gap[0]->Lambda *= search->pbp->scalingFactor;
    }

    /* Now for OOF alignment we try to detect simular alignments */

    HeapSort(hsp_array,hspcnt,sizeof(BLAST_HSPPtr), score_compare_hsps);
    BLASTCheckHSPInclusion(hsp_array, hspcnt, pbp->is_ooframe);

    /* Make up fake hitlist, relink and rereap. */

    if (StringCmp(search->prog_name, "blastx") == 0 ||
        StringCmp(search->prog_name, "tblastn") == 0 ||
        StringCmp(search->prog_name, "psitblastn") == 0) {
        hspcnt = HspArrayPurge(hsp_array, hspcnt, FALSE);
        tmp_hitlist = (BLAST_HitListPtr) MemNew(sizeof(BLAST_HitList));
        real_hitlist = search->current_hitlist;
        
        search->current_hitlist = tmp_hitlist;
        tmp_hitlist->hsp_array = hsp_array;
        tmp_hitlist->hspcnt = hspcnt;
        tmp_hitlist->hspmax = hspcnt;
        
        /* Use real subject length for all programs - it will be adjusted inside 
           the functions that need it */
        search->subject->length = subject_length;
        
        if (search->prog_number == blast_type_tblastn &&
            search->pbp->longest_intron > 0) {
           BlastSequenceAddSequence(search->subject, NULL, subject-1, 
                                    subject_length, subject_length, 0);
           search->subject_id = ordinal_id;
        }
 
        if (!search->pbp->do_sum_stats || search->pbp->longest_intron > 0)
           BlastGetNonSumStatsEvalue(search);

	/* AM: Changed to support query concatenation. */
        if (search->pbp->do_sum_stats == TRUE)
	{
	    if( search->mult_queries ) search->mult_queries->use_mq = FALSE;

            BlastLinkHsps(search);
        }
        
        if (search->prog_number == blast_type_tblastn &&
            search->pbp->longest_intron > 0)
           search->subject->sequence_start = search->subject->sequence = NULL;
        
        BlastReapHitlistByEvalue(search);
        
        hspcnt = search->current_hitlist->hspcnt;
        search->current_hitlist = real_hitlist;
	tmp_hitlist->lh_helper = MemFree(tmp_hitlist->lh_helper);
        MemFree(tmp_hitlist);
    }
    
    new_hspcnt = HspArrayPurge(hsp_array, hspcnt, FALSE);
    
    HeapSort(hsp_array,new_hspcnt,sizeof(BLAST_HSPPtr), score_compare_hsps);
    
    /* Remove extra HSPs if there is a user proveded limit on the number 
       of HSPs per database sequence */
    if (search->pbp->hsp_num_max > new_hspcnt) {
       for (index=new_hspcnt; index<search->pbp->hsp_num_max; ++index) {
          hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
       }
       new_hspcnt = MIN(new_hspcnt, search->pbp->hsp_num_max);
    }

    if (do_traceback) {
        for (index=0; index<new_hspcnt; index++) {
            hsp = hsp_array[index];
            hsp->gap_info->reverse = reverse;
            hsp->gap_info->original_length1 = search->context[hsp->context].query->original_length;
            hsp->gap_info->original_length2 = subject_length;
	    if (search->pbp->mb_params) {
               query_id = search->qid_array[hsp->context/2];
	    } else {
               query_id = search->query_id;
            }
            CopyHSPToResultHsp(search->sbp->kbp_gap[search->first_context], 
                               hsp, &result_hsp);

	    if (new_subject_seqid) {
                if (search->pbp->explode_seqids)
                    seqid_tmp = gi_list;
                else
                    seqid_tmp = new_subject_seqid;
	    } else {
		seqid_tmp = subject_id;
            }
            
	    while (seqid_tmp) {
                if(search->pbp->is_ooframe) {
                    seqalign = OOFGapXEditBlockToSeqAlign(hsp->gap_info, seqid_tmp, query_id, hsp->query.frame > 0 ? search->query_dnap[0]->length : search->query_dnap[1]->length);
                } else {
                    seqalign = GapXEditBlockToSeqAlign(hsp->gap_info, seqid_tmp, query_id); 
                }
                
            	seqalign->score = GetScoreSetFromBlastResultHsp(&result_hsp, gi_list);
                
		if (seqalign_array[index] == NULL)
                    seqalign_array[index] = seqalign;
		else {
                    seqalign_var = seqalign_array[index];
                    while (seqalign_var->next)
                        seqalign_var = seqalign_var->next;
                    seqalign_var->next = seqalign;
		}
		seqid_tmp = seqid_tmp->next;
	    }
	}
        
        *head = NULL;
        for (index=0; index<new_hspcnt; index++) {
            if (seqalign_array[index] != NULL) {
                if (*head == NULL) {
                    *head = seqalign_array[index];
                } else {
                    for (seqalign_var=*head; seqalign_var->next != NULL;) {
                        seqalign_var = seqalign_var->next;
                    }
                    seqalign_var->next = seqalign_array[index];
                }
            }
        }
        
        seqalign_array = MemFree(seqalign_array);
    } else {
        if (bhrp)
            BlastHitRangeLoad(search, hsp_array, new_hspcnt, bhrp);
    }
    
    gi_list = SeqIdSetFree(gi_list);
    if (new_subject_seqid)
        new_subject_seqid = SeqIdSetFree(new_subject_seqid);
    
    if ((StringCmp(search->prog_name, "tblastn") == 0 ||
         StringCmp(search->prog_name, "psitblastn") == 0)&&
        translated_subject_orig) {
        for (index=0; index<8; index++) {
            MemFree(translated_subject_orig[index]);
        }
        MemFree(translated_subject_orig);
        MemFree(translated_subject_length_orig);
    }
    
    return new_hspcnt;
}


/*
	find the traceback for a gapped alignment.  Do this by 
	organizing the list of HSP's by sum group, then order
	these groups by score.  Then attempt to perform the alignment
	by using the highest scoring HSP of every sum group, then the
	2nd highest scoring HSP, etc. until all the HSP's of a sum
	group have been examined.  Then move onto the next sum group.
*/
SeqAlignPtr LIBCALL
SumBlastGetGappedAlignmentTraceback (BlastSearchBlkPtr search, Int4 hit_number, Boolean reverse, Boolean ordinal_number, Uint1Ptr subject, Int4 subject_length)

{
	SeqAlignPtr seqalign;

	SumBlastGetGappedAlignmentEx(search, hit_number, reverse, ordinal_number, subject, subject_length, TRUE, &seqalign, NULL, 0);

	return seqalign;
}

Boolean LIBCALL
SumBlastGetGappedAlignmentEx (BlastSearchBlkPtr search, Int4 hit_number, Boolean reverse, Boolean ordinal_number, Uint1Ptr subject, Int4 subject_length, Boolean do_traceback, SeqAlignPtr PNTR seqalignP, BlastHitRangePtr bhrp, Int2 query_number)

{
	BLAST_HSPPtr PNTR hsp_array;
	BLASTResultHitlistPtr   result_hitlist;
        BLASTResultHspPtr       result_hsp_array=NULL, hsp;
	Boolean not_done;
	Int4 hspcnt=0, new_hspcnt=0, hspset_cnt_old;
	Int4 index, index1, high_score=0, ordinal_id, next_start, start, stop;
	SeqAlignPtr seqalign=NULL;
	SeqIdPtr subject_id=NULL, sip, subject_id_var;
	Nlm_FloatHi current_evalue=DBL_MAX;
	ValNodePtr vnp, vnp_start;
   BLASTResultsStructPtr result_struct;
   Boolean is_megablast = (search->pbp->mb_params != NULL);

	if (search == NULL)
		return FALSE;

        if (is_megablast) {
           result_struct = search->mb_result_struct[query_number];
        } else {
           result_struct = search->result_struct;
        }
        result_hitlist = result_struct->results[hit_number];
        hspcnt = result_hitlist->hspcnt;

	if (search->pbp->explode_seqids)
	{ /* Obtain and connect all SeqId's if explode demanded. */
		vnp = NULL;
                if (is_megablast)
                   BlastGetSubjectIdEx(search, hit_number, 
                                       ordinal_number, &vnp, query_number);
                else
                   BlastGetSubjectId(search, hit_number, ordinal_number, &vnp);
		vnp_start = vnp;
		while (vnp)
		{
			sip = GetTheSeqAlignID(vnp->data.ptrvalue);
			SeqIdFree(vnp->data.ptrvalue);
			if (subject_id == NULL)
			{
				subject_id = sip;
			}
			else
			{
				subject_id_var = subject_id;
				while (subject_id_var->next)
					subject_id_var = subject_id_var->next;
				subject_id_var->next = sip;
			}
			vnp = vnp->next;
		}
		vnp_start = vnp = ValNodeFree(vnp_start);
	}
	else
	{
		sip = BlastGetSubjectIdEx(search, hit_number, ordinal_number,
                                          NULL, query_number);
		subject_id = GetTheSeqAlignID(sip);
	    	sip = SeqIdSetFree(sip);
	}
	ordinal_id = result_hitlist->subject_id;

	hsp_array = MemNew(hspcnt*sizeof(BLAST_HSPPtr));
	not_done = TRUE;
	start=0;
	next_start=0;
	while (not_done)
	{
		hsp = &(result_hitlist->hsp_array[start]);
		hspset_cnt_old = hsp->hspset_cnt;
		for (index=start; index<hspcnt; index++)
		{
			hsp = &(result_hitlist->hsp_array[index]);
			if(hspset_cnt_old != hsp->hspset_cnt)
			{
				hspset_cnt_old = hsp->hspset_cnt;
				stop = index;
				next_start = stop;
				break;
			}
		}

		if (index == hspcnt)
		{
			stop = hspcnt;
			not_done = FALSE;
		}

		index1=0;
		for (index=start; index<stop; index++)
		{
			hsp_array[index] = MemNew(sizeof(BLAST_HSP));
			CopyResultHspToHSP(&(result_hitlist->hsp_array[index]), hsp_array[index]);
			index1++;
		}

		/* heap sort the last sum group */
		HeapSort(hsp_array+start,(stop-start),sizeof(BLAST_HSPPtr), score_compare_hsps);
		start = next_start;
	}

	new_hspcnt = RealBlastGetGappedAlignmentTraceback(search, subject, subject_length, NULL, 0, subject_id, hsp_array, hspcnt, &seqalign, bhrp, search->pbp->cutoff_s, reverse, ordinal_id, do_traceback);

/* Save HSP's again, discarding those that have been NULLed out. */
/* If no HSP's were valid, best_evalue is set to DBL_MAX. */
	index1 = 0;
	if (new_hspcnt > 0)
	{
		result_hsp_array = MemNew((new_hspcnt)*sizeof(BLASTResultHsp));
		index1 = 0;
		for (index=0; index<hspcnt; index++)
		{
			if (hsp_array[index] != NULL)
			{
				if (current_evalue > hsp_array[index]->evalue)
					current_evalue = hsp_array[index]->evalue;
				if (high_score < hsp_array[index]->score)
       	                        	high_score = hsp_array[index]->score;
				CopyHSPToResultHsp(search->sbp->kbp_gap[search->first_context], hsp_array[index], &(result_hsp_array[index1]));
				index1++;
                                /* Do not free edit block, just the 
                                   BLAST_HSP structure. */
				hsp_array[index] = MemFree(hsp_array[index]);
			}
		}
	}
	hsp_array = MemFree(hsp_array);

	result_hitlist->hspcnt = index1;	
	if (result_hitlist->hsp_array)
		MemFree(result_hitlist->hsp_array);
	result_hitlist->hsp_array = result_hsp_array;
	result_hitlist->best_evalue = current_evalue;
	result_hitlist->high_score = high_score;

	subject_id = SeqIdSetFree(subject_id);

	if (seqalignP)
	*	seqalignP = seqalign;

	return TRUE;
}

/*
	Performs a gapped alignment on the HSP's in a hitlist.
	Discards those that do not meet the standard.
*/

SeqAlignPtr LIBCALL 
BlastGetGapAlgnTbck (BlastSearchBlkPtr search, Int4 hit_number, Boolean reverse, Boolean ordinal_number, Uint1Ptr subject, Int4 subject_length, Uint1Ptr rev_subject, Int4 rev_subject_length)

{
	BLAST_HSPPtr PNTR hsp_array;
	BLASTResultHitlistPtr   result_hitlist;
        BLASTResultHspPtr       result_hsp_array=NULL;
	Int4 hspcnt=0, new_hspcnt=0;
	Int4 index, index1, high_score=0, ordinal_id;
	SeqAlignPtr seqalign, head, seqalign_var;
	SeqIdPtr subject_id=NULL, sip, subject_id_var;
	Nlm_FloatHi current_evalue=DBL_MAX;
	ValNodePtr vnp, vnp_start;

	if (search == NULL)
		return NULL;

        result_hitlist = search->result_struct->results[hit_number];
        hspcnt = result_hitlist->hspcnt;
	ordinal_id = result_hitlist->subject_id;

	if (search->pbp->explode_seqids)
	{ /* Obtain and connect all SeqId's if explode demanded. */
		vnp = NULL;
		BlastGetSubjectId(search, hit_number, ordinal_number, &vnp);
		vnp_start = vnp;
		while (vnp)
		{
			sip = GetTheSeqAlignID(vnp->data.ptrvalue);
			SeqIdFree(vnp->data.ptrvalue);
			if (subject_id == NULL)
			{
				subject_id = sip;
			}
			else
			{
				subject_id_var = subject_id;
				while (subject_id_var->next)
					subject_id_var = subject_id_var->next;
				subject_id_var->next = sip;
			}
			vnp = vnp->next;
		}
		vnp_start = vnp = ValNodeFree(vnp_start);
	}
	else
	{
		sip = BlastGetSubjectId(search, hit_number, ordinal_number, NULL);
		subject_id = GetTheSeqAlignID(sip);
	    	sip = SeqIdSetFree(sip);
	}

	head = NULL;

	hsp_array = MemNew(hspcnt*sizeof(BLAST_HSPPtr));
	for (index=0; index<hspcnt; index++)
	{
		hsp_array[index] = MemNew(sizeof(BLAST_HSP));
		CopyResultHspToHSP(&(result_hitlist->hsp_array[index]), 
                                   hsp_array[index]);
	}
	HeapSort(hsp_array,hspcnt,sizeof(BLAST_HSPPtr), score_compare_hsps);

	new_hspcnt = RealBlastGetGappedAlignmentTraceback(search, subject, subject_length, rev_subject, rev_subject_length, subject_id, hsp_array, hspcnt, &seqalign, NULL, 0, reverse, ordinal_id, TRUE);
	if (seqalign != NULL)
	{
		if (head == NULL)
		{
			head = seqalign;
		}
		else
		{
			for (seqalign_var=head; seqalign_var->next != NULL;)
			{
				seqalign_var = seqalign_var->next;
			}
			seqalign_var->next = seqalign;
		}
	}

/* Save HSP's again, discarding those that have been NULLed out. */
	result_hsp_array = MemNew((new_hspcnt)*sizeof(BLASTResultHsp));
	index1 = 0;
	for (index=0; index<hspcnt; index++)
	{
		if (hsp_array[index] != NULL)
		{
			if (current_evalue > hsp_array[index]->evalue)
				current_evalue = hsp_array[index]->evalue;
			if (high_score < hsp_array[index]->score)
                               	high_score = hsp_array[index]->score;

			CopyHSPToResultHsp(search->sbp->kbp_gap[search->first_context], hsp_array[index], &(result_hsp_array[index1]));
			index1++;
                        /* Do not free edit block, just the BLAST_HSP 
                           structure */
			hsp_array[index] = MemFree(hsp_array[index]);
		}
	}
	hsp_array = MemFree(hsp_array);

	if (result_hitlist->hsp_array) {
           /* Delete any edit blocks from a previous traceback. */
           for (index=0; index< result_hitlist->hspcnt; ++index)
              GapXEditBlockDelete(result_hitlist->hsp_array[index].gap_info);

           MemFree(result_hitlist->hsp_array);
        }
	result_hitlist->hspcnt = index1;	
	result_hitlist->hsp_array = result_hsp_array;
	result_hitlist->best_evalue = current_evalue;
	result_hitlist->high_score = high_score;

	subject_id = SeqIdSetFree(subject_id);

	return head;
}

/*
	Performs a gapped alignment on the HSP's in a hitlist.
	Discards those that do not meet the standard.
*/

Int2 LIBCALL
BlastPreliminaryGappedScore (BlastSearchBlkPtr search, Uint1Ptr subject, Int4 subject_length, Int2 frame)

{
	BLAST_HitListPtr hitlist;
	BLAST_HSPPtr hsp;
	BLAST_HSPPtr PNTR hsp_array;
	GapAlignBlkPtr gap_align;
	Int2 status;
	Int4 index, max_offset = 0, query_length, min_score;
	BLAST_ParameterBlkPtr pbp;

	if (search == NULL)
		return 1;

	pbp = search->pbp;

	if (search->gap_align == NULL)
	{
		search->gap_align = GapAlignBlkNew(1, 1);
	}
	gap_align = search->gap_align;

	min_score = search->pbp->cutoff_s1;

	status = 0;
	hitlist = search->current_hitlist;
	if (hitlist && hitlist->hspcnt > 0)
	{
		query_length = search->context[search->first_context].query->length;

		hitlist->hspcnt_max = hitlist->hspcnt;
		hsp_array = hitlist->hsp_array;
		if (frame != 0)
		{
			for (index=0; index<hitlist->hspcnt; index++)
			{
				hsp = hsp_array[index];
				if (frame == hsp->subject.frame)
					break;
			}
			if (frame != hsp->subject.frame)
				return 0;
		}
		else
		{ /* The first HSP has the highest score. */
			hsp = hsp_array[0];
		}

		/* The first HSP has the highest score. */
/*
		e_value = BlastKarlinStoE_simple(hsp->score, search->sbp->kbp_gap[search->first_context], search->searchsp_eff);
*/
		if (hsp->score >= min_score)
		{
#ifdef BLAST_COLLECT_STATS
			search->prelim_gap_no_contest++;
#endif
			hitlist->further_process = TRUE;
			return 1;
		}
                gap_align->is_ooframe = pbp->is_ooframe;
                gap_align->shift_pen = pbp->shift_pen; 
                gap_align->discontinuous = pbp->discontinuous;
		gap_align->positionBased = 
                   (search->positionBased && search->sbp->posMatrix);
		gap_align->include_query = 0;
		gap_align->gap_open = pbp->gap_open;
		gap_align->gap_extend = pbp->gap_extend;
                gap_align->decline_align = pbp->decline_align;
		gap_align->x_parameter = pbp->gap_x_dropoff;
		gap_align->matrix = search->sbp->matrix;
		gap_align->posMatrix = search->sbp->posMatrix;
		for (index=0; index<hitlist->hspcnt; index++)
		{
			hsp = hsp_array[index];
			if (frame != 0)
			{
				if (frame != hsp->subject.frame)
					continue;
			}

			if (hsp->score < search->pbp->gap_trigger)
			{	/* Stop looking, we're below the cutoff. */
				status = 0;
				break;
			}

#ifdef BLAST_COLLECT_STATS
				search->prelim_gap_attempts++;
#endif
			gap_align->score = 0;

                        if(!search->pbp->is_ooframe) {
                            max_offset = GetStartForGappedAlignment(search, hsp, search->context[hsp->context].query->sequence, subject, search->sbp->matrix);
                        }

                        if(search->pbp->is_ooframe) {
                            gap_align->is_ooframe = TRUE;
                            gap_align->query = subject;

                            if(hsp->query.frame > 0) {
                                gap_align->subject = search->query_dnap[0]->sequence;
                                gap_align->subject_length = search->query_dnap[0]->length;
                            } else {
                                gap_align->subject = search->query_dnap[1]->sequence;
                                gap_align->subject_length = search->query_dnap[1]->length;
                            }
                            
                            gap_align->query_frame = hsp->subject.frame;
                            gap_align->subject_frame = ContextToFrame(search, hsp->context);

                            gap_align->query_length = subject_length;
                            gap_align->q_start = hsp->subject.offset;
                            gap_align->s_start = hsp->query.offset;
                        } else {
                            gap_align->query = search->context[hsp->context].query->sequence;
                            gap_align->subject = subject;
                            gap_align->query_length = search->context[hsp->context].query->length;
                            gap_align->subject_length = subject_length;
                            gap_align->q_start = max_offset;
                            gap_align->s_start = (hsp->subject.offset - hsp->query.offset) + max_offset;
                        }
                        
			gap_align->include_query = 0;
                        
	/* Perform only if the query's required start corresponds to a point after the start of the subject. */
			if (gap_align->s_start >= 0)
				PerformGappedAlignment(gap_align);
/*
			e_value = BlastKarlinStoE_simple(gap_align->score, search->sbp->kbp_gap[search->first_context], search->searchsp_eff);
*/
			if (gap_align->score >= min_score)
			{	/* Found one, stop looking. */
				hitlist->further_process = TRUE;
				status = 1;
#ifdef BLAST_COLLECT_STATS
				search->prelim_gap_passed++;
#endif
				break;
			}
		}
	}

	return status;
}

/*
	Performs a gapped alignment on the HSP's in a hitlist.
	This is to be used with blastn assuming the database sequence
	will be unpacked on the fly.
	Discards those that do not meet the standard.
*/

Int2 LIBCALL
BlastNTPreliminaryGappedScore (BlastSearchBlkPtr search, Uint1Ptr subject, Int4 subject_length)

{
	BLAST_HitListPtr hitlist;
	BLAST_HSPPtr hsp;
	BLAST_HSPPtr PNTR hsp_array;
	GapAlignBlkPtr gap_align;
	Int2 status;
	Int4 index;
	Nlm_FloatHi e_value;
	BLAST_ParameterBlkPtr pbp;
	/* AM: To support query concatenation. */
	Int4 query_num;

	if (search == NULL)
		return -1;

	pbp = search->pbp;

	if (search->gap_align == NULL)
	{
		search->gap_align = GapAlignBlkNew(1, 1);
	}
	gap_align = search->gap_align;

	status = 0;
	hitlist = search->current_hitlist;
	if (hitlist && hitlist->hspcnt > 0)
	{

		hitlist->hspcnt_max = hitlist->hspcnt;
		hsp_array = hitlist->hsp_array;

		/* The first HSP has the highest score. */
		hsp = hsp_array[0];

                /* AM: Changed to support query concatenation. */
		/* The first HSP has the highest score. */
		if( !search->mult_queries )
		  e_value = BlastKarlinStoE_simple(hsp->score, search->sbp->kbp[search->first_context], search->searchsp_eff);
                else
		{
		  /* AM: First determine which query to use, then use the
		         corresponding SearchSpEff element in the call to 
		         BlastKarlinStoE_simple() */
		  query_num = GetQueryNum( search->mult_queries,
		                           hsp->query.offset,
					   hsp->query.end,
					   hsp->query.frame );
	       	  e_value = BlastKarlinStoE_simple( hsp->score,
		                                    search->sbp->kbp[search->first_context],
						    search->mult_queries->SearchSpEff[query_num] );
		}

		if (e_value <= pbp->cutoff_e)
		{
#ifdef BLAST_COLLECT_STATS
			search->prelim_gap_no_contest++;
#endif
			hitlist->further_process = TRUE;
			return 1;
		}

                gap_align->is_ooframe = pbp->is_ooframe;
                gap_align->shift_pen = pbp->shift_pen; 
		gap_align->positionBased = search->positionBased;
                gap_align->discontinuous = pbp->discontinuous;
		gap_align->include_query = 0;
		gap_align->gap_open = pbp->gap_open;
		gap_align->gap_extend = pbp->gap_extend;
                gap_align->decline_align = pbp->decline_align;
		gap_align->x_parameter = pbp->gap_x_dropoff;
		gap_align->matrix = search->sbp->matrix;
		gap_align->posMatrix = search->sbp->posMatrix;
		for (index=0; index<hitlist->hspcnt; index++)
		{
			hsp = hsp_array[index];

			if (hsp->score < search->pbp->gap_trigger)
			{	/* Stop looking, we're below the cutoff. */
				status = 0;
				break;
			}

#ifdef BLAST_COLLECT_STATS
				search->prelim_gap_attempts++;
#endif
			gap_align->score = 0;
			gap_align->query = search->context[hsp->context].query->sequence;
			gap_align->subject = subject;
			gap_align->query_length = search->context[hsp->context].query->length;
			gap_align->subject_length = subject_length;
			gap_align->include_query = 0;
			gap_align->q_start = hsp->query.gapped_start;
			gap_align->s_start = hsp->subject.gapped_start;
	/* Perform only if the query's required start corresponds to a point after the start of the subject. */
			if (gap_align->s_start >= 0) {
                           if (!PerformNtGappedAlignment(gap_align))
                              return -1;
                        }

			/* AM: Change to support query concatenation */
			if( !search->mult_queries )
			  e_value = BlastKarlinStoE_simple(gap_align->score, search->sbp->kbp[search->first_context], search->searchsp_eff);
                        else
			{
			  query_num = GetQueryNum( search->mult_queries,
			                           hsp->query.offset,
						   hsp->query.end,
						   hsp->query.frame );
			  e_value = BlastKarlinStoE_simple(gap_align->score, 
			                                   search->sbp->kbp[search->first_context], 
							   search->mult_queries->SearchSpEff[query_num]);
			}

			if (e_value <= pbp->cutoff_e)
			{	/* Found one, stop looking. */
				hitlist->further_process = TRUE;
				status = 1;
#ifdef BLAST_COLLECT_STATS
				search->prelim_gap_passed++;
#endif
				break;
			}
		}
	}

	return status;
}

/*
	Performs a gapped alignment on the HSP's in a hitlist.
	Discards those that do not meet the standard.
	Do this by obtaining the sequence from readdb and calling
	BlastGetGappedScore.
*/

Int2 LIBCALL
BlastGetGappedScoreWithReaddb (BlastSearchBlkPtr search, Int4 sequence_number)

{
	BLAST_HitListPtr hitlist;
	Int2 retval;
	Int4 subject_length;
	Uint1Ptr subject;

	if (search == NULL)
		return 1;

	retval=0;
	hitlist = search->current_hitlist;
	if (hitlist && hitlist->hspcnt > 0)
	{
		if (hitlist->further_process == FALSE)
		{
			BlastHitListPurge(hitlist);
			return 0;
		}
		subject_length = readdb_get_sequence(search->rdfp, sequence_number, &subject);
		retval = BlastGetGappedScore(search, subject_length, subject, 0);
	}

	return retval;
}


/*
	Performs a gapped alignment on the HSP's in a hitlist.
	Discards those that do not meet the standard.
*/

Int2 LIBCALL
BlastGetGappedScore (BlastSearchBlkPtr search, Int4 subject_length, Uint1Ptr subject, Int2 frame)

{
	BLAST_HitListPtr hitlist;
	BLAST_HSPPtr PNTR hsp_array, PNTR hsp_array_new;
	BLAST_ParameterBlkPtr pbp;
	GapAlignBlkPtr gap_align;
	Int2 status=0;
	Int4 hsp_cnt=0, hspcnt_max;
	Int4 index, index1;

	if (search == NULL)
		return 1;

	pbp = search->pbp;


	if (search->gap_align == NULL)
	{
		search->gap_align = GapAlignBlkNew(1, 1);
	}
	gap_align = search->gap_align;

	hitlist = search->current_hitlist;
	if (hitlist && hitlist->hspcnt > 0)
	{
		if (hitlist->further_process == FALSE)
		{
			BlastHitListPurge(hitlist);
			return 0;
		}

		
		hsp_array = hitlist->hsp_array;
		if (hitlist->hspcnt != hitlist->hspcnt_max)
		{
/* Save HSP's again, discarding those that have been NULLed out. */
			hsp_array_new = MemNew((hitlist->hspmax)*sizeof(BLAST_HSPPtr));
			index1 = 0;
			for (index=0; index<hitlist->hspcnt_max; index++)
			{
				if (hsp_array[index] != NULL)
				{
					hsp_array_new[index1] = hsp_array[index];
					index1++;
				}
			}
			hsp_array = MemFree(hsp_array);
			hsp_array = hsp_array_new;
			hitlist->hsp_array = hsp_array_new;
			hitlist->hspcnt = index1;
			hitlist->hspcnt_max = index1;
		}

                gap_align->is_ooframe = pbp->is_ooframe;
                gap_align->shift_pen = pbp->shift_pen; 
                gap_align->discontinuous = pbp->discontinuous;
		gap_align->positionBased = 
                   (search->positionBased && search->sbp->posMatrix);
		gap_align->include_query = 0;
		gap_align->gap_open = pbp->gap_open;
		gap_align->gap_extend = pbp->gap_extend;
                gap_align->decline_align = pbp->decline_align;
		gap_align->x_parameter = pbp->gap_x_dropoff;
		gap_align->matrix = search->sbp->matrix;
		gap_align->posMatrix = search->sbp->posMatrix;

		if (frame != 0)
		{
			hsp_array = hitlist->hsp_array;
			switch (frame) {
			case -3:
				HeapSort(hsp_array, hitlist->hspcnt, sizeof(BLAST_HSPPtr), frame_compare_hsp_m3);
				break;
			case -2:
				HeapSort(hsp_array, hitlist->hspcnt, sizeof(BLAST_HSPPtr), frame_compare_hsp_m2);
				break;
			case -1:
				HeapSort(hsp_array, hitlist->hspcnt, sizeof(BLAST_HSPPtr), frame_compare_hsp_m1);
				break;
			case 1:
				HeapSort(hsp_array, hitlist->hspcnt, sizeof(BLAST_HSPPtr), frame_compare_hsp_p1);
				break;
			case 2:
				HeapSort(hsp_array, hitlist->hspcnt, sizeof(BLAST_HSPPtr), frame_compare_hsp_p2);
				break;
			case 3:
				HeapSort(hsp_array, hitlist->hspcnt, sizeof(BLAST_HSPPtr), frame_compare_hsp_p3);
				break;
			default:
				break;
			}

			for (index=0; index<hitlist->hspcnt; index++)
			{
				if (hsp_array[index]->subject.frame != frame)
					break;
			}
			HeapSort(hsp_array,index,sizeof(BLAST_HSPPtr), score_compare_hsps);
		}
		else
		{
			HeapSort(hsp_array,hitlist->hspcnt,sizeof(BLAST_HSPPtr), score_compare_hsps);
		}
		hitlist->hspcnt_max = hitlist->hspcnt;
		hsp_array = hitlist->hsp_array;
		hspcnt_max = hitlist->hspcnt;
		hsp_cnt = hitlist->hspcnt;

start_timer;
		hsp_array = BlastGappedScoreInternal(search, subject, subject_length, gap_align, hsp_array, &hsp_cnt, &hspcnt_max, hitlist->hspmax, frame);
stop_timer("after BlastGappedScoreInternal");
		hitlist->hspcnt = hsp_cnt;	
		hitlist->hspcnt_max = hspcnt_max;	
		hitlist->hsp_array = hsp_array;
	}

	return status;
}

/*
	Performs a gapped alignment on the HSP's in a hitlist.
	Discards those that do not meet the standard.
*/

Int2 LIBCALL
BlastNTGetGappedScore (BlastSearchBlkPtr search, Int4 subject_length, Uint1Ptr subject)

{
	BLAST_HitListPtr hitlist;
	BLAST_HSPPtr PNTR hsp_array, PNTR hsp_array_new;
	BLAST_ParameterBlkPtr pbp;
	GapAlignBlkPtr gap_align;
	Int2 status=0;
	Int4 hsp_cnt=0, hspcnt_max;
	Int4 index, index1;

	if (search == NULL)
		return -1;

	pbp = search->pbp;


	if (search->gap_align == NULL)
	{
		search->gap_align = GapAlignBlkNew(1, 1);
	}
	gap_align = search->gap_align;

	hitlist = search->current_hitlist;
	if (hitlist && hitlist->hspcnt > 0)
 	{
                if (hitlist->further_process == FALSE) 
                {
                        BlastHitListPurge(hitlist);
                        return 0;
                } 
		
		hsp_array = hitlist->hsp_array;
		if (hitlist->hspcnt != hitlist->hspcnt_max)
		{
/* Save HSP's again, discarding those that have been NULLed out. */
			hsp_array_new = MemNew((hitlist->hspmax)*sizeof(BLAST_HSPPtr));
			index1 = 0;
			for (index=0; index<hitlist->hspcnt_max; index++)
			{
				if (hsp_array[index] != NULL)
				{
					hsp_array_new[index1] = hsp_array[index];
					index1++;
				}
			}
			hsp_array = MemFree(hsp_array);
			hsp_array = hsp_array_new;
			hitlist->hsp_array = hsp_array_new;
			hitlist->hspcnt = index1;
			hitlist->hspcnt_max = index1;
		}

                gap_align->is_ooframe = pbp->is_ooframe;
                gap_align->shift_pen = pbp->shift_pen; 
                gap_align->discontinuous = pbp->discontinuous;
		gap_align->positionBased = search->positionBased;
		gap_align->include_query = 0;
		gap_align->gap_open = pbp->gap_open;
		gap_align->gap_extend = pbp->gap_extend;
                gap_align->decline_align = pbp->decline_align;
		gap_align->x_parameter = pbp->gap_x_dropoff;
		gap_align->matrix = search->sbp->matrix;
		gap_align->posMatrix = search->sbp->posMatrix;

		HeapSort(hsp_array,hitlist->hspcnt,sizeof(BLAST_HSPPtr), score_compare_hsps);
		hitlist->hspcnt_max = hitlist->hspcnt;
		hsp_array = hitlist->hsp_array;
		hspcnt_max = hitlist->hspcnt;
		hsp_cnt = hitlist->hspcnt;

		if (!BlastNtGappedScoreInternal(search, subject, subject_length, gap_align, hsp_array, &hsp_cnt, &hspcnt_max, hitlist->hspmax))
                   /* Gapped extension failed */
                   return -1;
		hitlist->hspcnt = hsp_cnt;	
		hitlist->hspcnt_max = hspcnt_max;	
		hitlist->hsp_array = hsp_array;
	}

	return status;
}


/******************************************************************

	Purges (i.e., cleans) the HitList for reuse.

*******************************************************************/

Int2 LIBCALL
BlastHitListPurge(BLAST_HitListPtr hitlist)

{
	BLAST_HSPPtr PNTR hsp_array;
	Int4 hspcnt_max, index;
	
	if (hitlist == NULL)
		return 1;

	hsp_array = hitlist->hsp_array;

	if (hitlist->hspcnt > hitlist->hspcnt_max)
		hspcnt_max = hitlist->hspcnt;
	else
		hspcnt_max = hitlist->hspcnt_max;

	for (index=0; index<hspcnt_max; index++) {
           hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
        }	

	hitlist->hspcnt = 0;
	hitlist->hspcnt_max = 0;
	hitlist->further_process = FALSE;

	return 0;
}

/*
	Cleans out the NULLed out HSP's from the HSP array,
	moving the BLAST_HSPPtr's up to fill in the gaps.

	returns the number of valid HSP's.
*/

Int4 LIBCALL
HspArrayPurge (BLAST_HSPPtr PNTR hsp_array, Int4 hspcnt, Boolean clear_num)

{
	Int4 index, index1;

	if (hspcnt == 0 || hsp_array == NULL)
		return 0;

	index1 = 0;
	for (index=0; index<hspcnt; index++)
	{
		if (hsp_array[index] != NULL)
		{
			hsp_array[index1] = hsp_array[index];
			if (clear_num)
				hsp_array[index1]->num = 0;
			index1++;
		}
	}

	for (index=index1; index<hspcnt; index++)
	{
		hsp_array[index] = NULL;
	}

	hspcnt = index1;

	return index1;
}


#if 0
/* TODO: function is not used in this file */
/*
	Compares two HSP's looking for overlap in both the
	query and the subject sequence.  

	if hsp1 should be deleted, then -1 is returned,
	if hsp2 should be deleted, then +1 is returned,
	if neither should be deleted, then 0 is returned.
*/

static Int2 
CheckHspOverlap (BLAST_HSPPtr PNTR hsp_array, BLAST_HSPPtr hsp2, Int4 hspcnt, Boolean PNTR hsp_deleted)

{
	BLAST_HSPPtr hsp1;
	BLAST_SegPtr query, subject;
	Int4 index;

	if (hsp_array == NULL || hsp2 == NULL)
		return 0;


	*hsp_deleted = FALSE;
	query = &(hsp2->query);
	subject = &(hsp2->subject);

	for (index=0; index<hspcnt; index++)
	{
		hsp1 = hsp_array[index];
		if (hsp1 == NULL)
			continue;

		if (SIGN(hsp1->query.frame) != SIGN(query->frame))
			continue;

		if (SIGN(hsp1->subject.frame) != SIGN(subject->frame))
			continue;

		if (hsp1->query.offset > query->offset && 
			hsp1->query.end > query->end) 
			continue;

		if (hsp1->query.offset < query->offset && 
			hsp1->query.end < query->end) 
			continue;

		if (hsp1->subject.offset > subject->offset && 
			hsp1->subject.end > subject->end) 
			continue;

		if (hsp1->subject.offset < subject->offset && 
			hsp1->subject.end < subject->end) 
			continue;

		if (hsp1->score > hsp2->score)
		{
			if (*hsp_deleted == FALSE)
			{
				return 1;
			}
		}
		else
		{
			hsp_array[index] = BLAST_HSPFree(hsp_array[index]);
			*hsp_deleted = TRUE;
		}
	}

	return 0;
}
#endif

static void OOF_TranslateHspToDNAP(BLAST_HSPPtr hspp, Int4 length)
{
    Int4 from, to, frame;
            
    from = hspp->query.offset;
    to = hspp->query.end;
    frame = abs(hspp->query.frame);
    
    hspp->query.offset = CODON_LENGTH*from + frame - 1;
    hspp->query.end = CODON_LENGTH*to + frame - 1;
    hspp->query.length = hspp->query.end - hspp->query.offset + 1;
    hspp->query.gapped_start = CODON_LENGTH*hspp->query.gapped_start + frame - 1;
        
    return;
}	
/**************************************************************************
*
*	Save the current HSP in the appropriate ranking.
*
**************************************************************************/

#define BLAST_HSP_ADD 100

void 
BlastSaveCurrentHsp(BlastSearchBlkPtr search, BLAST_Score score, Int4 q_offset, Int4 s_offset, Int4 length, Int2 context)

{
	BLAST_HitListPtr current_hitlist;
	BLAST_HSPPtr PNTR hsp_array, new_hsp;
	Int4 hspcnt, hspmax, index, high_index, low_index;

	current_hitlist = search->current_hitlist;
	hsp_array = current_hitlist->hsp_array;
	hspcnt = current_hitlist->hspcnt;
	hspmax = current_hitlist->hspmax;

        /* Check if need to create a new list */
        if (hspmax == 0 && current_hitlist->do_not_reallocate == FALSE) {
           hsp_array = (BLAST_HSPPtr PNTR) Malloc(BLAST_HSP_ADD*sizeof(BLAST_HSPPtr));
           hspmax = current_hitlist->hspmax = BLAST_HSP_ADD;
        }

	/* Check if list is already full, then reallocate. */
	if (hspcnt >= hspmax && current_hitlist->do_not_reallocate == FALSE)
	{
		hsp_array = (BLAST_HSPPtr PNTR) Realloc(hsp_array, current_hitlist->hspmax*2*sizeof(BLAST_HSPPtr));
		if (hsp_array == NULL)
		{
			ErrPostEx(SEV_WARNING, 0, 0, "UNABLE to reallocate in BlastSaveCurrentHsp for ordinal id %ld, continuing with fixed array of %ld HSP's", (long) search->subject_id, (long) hspmax);
			current_hitlist->do_not_reallocate = TRUE; 
		} 
		else
		{
			current_hitlist->hsp_array = hsp_array;
			current_hitlist->hspmax *= 2;
			hspmax = current_hitlist->hspmax;
			/* Prohibit future allocations. */
			if (search->pbp->hsp_num_max != 0 && current_hitlist->hspmax >= search->pbp->hsp_num_max)
			{
				ErrPostEx(SEV_WARNING, 0, 0, "Reached max %ld HSPs in BlastSaveCurrentHsp, continuing with this limit",
					(long) hspmax);
				current_hitlist->do_not_reallocate = TRUE; 
              			/* HSPs must be now sorted */ 
              			HeapSort(hsp_array, hspcnt, sizeof(BLAST_HSPPtr), score_compare_hsps);
			}
		}
	}

	new_hsp = (BLAST_HSPPtr) MemNew(sizeof(BLAST_HSP));
	new_hsp->score = score;
	new_hsp->query.offset = q_offset;
	new_hsp->subject.offset = s_offset;
	new_hsp->query.length = length;
	new_hsp->subject.length = length;
	new_hsp->query.end = q_offset + length;
	new_hsp->subject.end = s_offset + length;
	new_hsp->context = context;
	new_hsp->query.frame = ContextToFrame(search, context);
	new_hsp->subject.frame = search->subject->frame;

        /* HACK */
        new_hsp->query.gapped_start = q_offset;
        new_hsp->subject.gapped_start = s_offset;

        /* For out-of frame gapping - subject is protein
           and query is DNA translated into 3 frames
           so we have to adjust DNA sequence and
           coordinates */
        
        if(search->pbp->is_ooframe) {
            OOF_TranslateHspToDNAP(new_hsp, new_hsp->query.frame > 0 ? search->query_dnap[0]->length : search->query_dnap[1]->length);
        }

        if (!search->pbp->gapped_calculation && 
            search->prog_number != blast_type_blastn) {
           Int4 align_length;
           BlastHSPGetNumIdentical(search, new_hsp, NULL, &new_hsp->num_ident, 
              &align_length);
        }

/* If we are saving ALL HSP's, simply save and sort later. */
	if (current_hitlist->do_not_reallocate == FALSE)
	{
		hsp_array[current_hitlist->hspcnt] = new_hsp;
		(current_hitlist->hspcnt)++;
		return;
	}

/* Use a binary search to insert the HSP. */
        low_index  = 0;
        high_index = hspcnt;
        while(low_index < high_index) {
            Int4 next_index = (low_index + high_index)/2;
            if( score_compare_hsps(&new_hsp, &hsp_array[next_index]) > 0 ) {
                low_index = next_index  + 1;
            } else {
                high_index = next_index;
            }
        }

	if (hspcnt >= hspmax)
	{
            if (low_index >= hspcnt) {
                /* this HSP is less significant than others on a full list.*/
                new_hsp = BLAST_HSPFree(new_hsp);
                return;
            } else {
                /* Delete the last HPS on the list. */
                hspcnt = --current_hitlist->hspcnt;
                hsp_array[hspcnt] = BLAST_HSPFree(hsp_array[hspcnt]);
            }
	}
        /* Move existing elements out of the way */
        Nlm_MemMove(&hsp_array[low_index] + 1, &hsp_array[low_index],
                    (hspcnt-low_index)*sizeof(hsp_array[0]));
        hspcnt = ++current_hitlist->hspcnt;

        /* Insert the new HSP */
        hsp_array[low_index] = new_hsp;
	return;
}

void 
BlastSaveCurrentHspGapped(BlastSearchBlkPtr search, BLAST_Score score, 
			  Int4 q_offset, Int4 s_offset, Int4 q_length, 
			  Int4 s_length, Int2 context, GapXEditScriptPtr esp)
{
   BlastNtSaveCurrentHspGapped(search, score, q_offset, s_offset, q_length, 
                               s_length, q_offset, s_offset, context, esp);

}

void 
BlastNtSaveCurrentHspGapped(BlastSearchBlkPtr search, BLAST_Score score, 
                            Int4 q_offset, Int4 s_offset, Int4 q_length, 
                            Int4 s_length, Int4 q_gapped_start, 
                            Int4 s_gapped_start, Int2 context, 
                            GapXEditScriptPtr esp)
{
	BLAST_HitListPtr current_hitlist;
	BLAST_HSPPtr PNTR hsp_array, new_hsp;
	BLAST_Score highscore, lowscore;
	Int4 hspcnt, hspmax, index, new_index, high_index, old_index, low_index;
        Int4 new_hspmax;

	current_hitlist = search->current_hitlist;
	hsp_array = current_hitlist->hsp_array;
	hspcnt = current_hitlist->hspcnt;
	hspmax = current_hitlist->hspmax;

	/* Check if list is already full, then reallocate. */
	if (hspcnt >= hspmax && current_hitlist->do_not_reallocate == FALSE)
	{
           new_hspmax = 2*current_hitlist->hspmax;
           if (search->pbp->hsp_num_max && search->last_context <= 1)
              /* The HSP limit can only be applied here in case of a 
                 single query sequence; even then, save twice as many HSPs
                 so far, to accommodate for possible inclusion check 
                 failures and score changes because of ambiguities */
              new_hspmax = MIN(new_hspmax, 2*search->pbp->hsp_num_max);
           if (new_hspmax > current_hitlist->hspmax) {
		hsp_array = (BLAST_HSPPtr PNTR) Realloc(hsp_array, current_hitlist->hspmax*2*sizeof(BLAST_HSPPtr));
		if (hsp_array == NULL)
		{
			ErrPostEx(SEV_WARNING, 0, 0, "UNABLE to reallocate in BlastSaveCurrentHsp for ordinal id %ld, continuing with fixed array of %ld HSP's", (long) search->subject_id, (long) hspmax);
			current_hitlist->do_not_reallocate = TRUE; 
		} 
		else
		{
			current_hitlist->hsp_array = hsp_array;
			current_hitlist->hspmax = new_hspmax;
			hspmax = new_hspmax;
                }
           } else {
              /*ErrPostEx(SEV_WARNING, 0, 0, 
                        "Sequence %ld: reached max %ld HSPs",
                        search->subject_id, (long) hspmax);*/
              current_hitlist->do_not_reallocate = TRUE; 
           }
           if (current_hitlist->do_not_reallocate) {
              /* HSPs must be now sorted */ 
              HeapSort(hsp_array, hspcnt, sizeof(BLAST_HSPPtr), 
                       score_compare_hsps);
           }
	}

	new_hsp = (BLAST_HSPPtr) MemNew(sizeof(BLAST_HSP));
	new_hsp->score = score;
	new_hsp->query.offset = q_offset;
	new_hsp->subject.offset = s_offset;
	new_hsp->query.length = q_length;
	new_hsp->subject.length = s_length;
	new_hsp->query.end = q_offset + q_length;
	new_hsp->subject.end = s_offset + s_length;
	new_hsp->context = context;
	new_hsp->query.frame = ContextToFrame(search, context);
	new_hsp->subject.frame = search->subject->frame;

	new_hsp->query.gapped_start = q_gapped_start;
	new_hsp->subject.gapped_start = s_gapped_start;
	
	if (esp)
	   MegaBlastFillHspGapInfo(new_hsp, esp);

/* If we are saving ALL HSP's, simply save and sort later. */
	if (current_hitlist->do_not_reallocate == FALSE)
	{
		hsp_array[current_hitlist->hspcnt] = new_hsp;
		(current_hitlist->hspcnt)++;
		return;
	}

/* Use a binary search to insert the HSP. */

	if (hspcnt != 0)
	{
		highscore = hsp_array[0]->score;
		lowscore = hsp_array[hspcnt-1]->score;
	}
	else
	{
		highscore = 0;
		lowscore = 0;
	}

	if (score >= highscore)
	{
		new_index = 0;
	}
	else if (score <= lowscore)
	{
		new_index = hspcnt;
	}
	else
	{
		low_index = 0;
		high_index = hspcnt-1;
		new_index = (low_index+high_index)/2;
		old_index = new_index;

		for (index=0; index<BLAST_SAVE_ITER_MAX; index++)
		{
			if (score > hsp_array[new_index]->score)
			{
				high_index = new_index;
			}
			else
			{
				low_index = new_index;
			}
			new_index = (low_index+high_index)/2;
                        if (new_index == old_index)
                        { /* Perform this check as new_index get rounded DOWN a
bove.*/
                                if (score < hsp_array[new_index]->score)
                                {
                                        new_index++;
                                }
                                break;
                        }
                        old_index = new_index;
		}
	}

	if (hspcnt >= hspmax)
	{
		if (new_index >= hspcnt)
		{ /* this HSP is less significant than others on a full list.*/
                   new_hsp = BLAST_HSPFree(new_hsp);
                   return;
		}
		else
		{ /* Delete the last HSP on the list. */
                   hspcnt = --current_hitlist->hspcnt;
                   hsp_array[hspcnt] = BLAST_HSPFree(hsp_array[hspcnt]);
		}
	}
	current_hitlist->hspcnt++;
	Nlm_MemMove((hsp_array+new_index+1), (hsp_array+new_index), (hspcnt-new_index)*sizeof(hsp_array[0]));
	hsp_array[new_index] = new_hsp;

	return;
}

void 
BlastNtSaveCurrentHsp(BlastSearchBlkPtr search, BLAST_Score score, Int4 q_offset, Int4 s_offset, Int4 length, Int2 context, Int4 query_gap_start, Int4 subject_gap_start)

{
	BLAST_HitListPtr current_hitlist;
	BLAST_HSPPtr PNTR hsp_array, PNTR hsp_array_new, new_hsp;
	BLAST_Score highscore, lowscore;
	Int4 hspcnt, hspmax, index, new_index, high_index, old_index, low_index;

	current_hitlist = search->current_hitlist;
	hsp_array = current_hitlist->hsp_array;
	hspcnt = current_hitlist->hspcnt;
	hspmax = current_hitlist->hspmax;


	/* Check if list is already full, then reallocate. */
	if (hspcnt >= hspmax && current_hitlist->do_not_reallocate == FALSE)
	{
		hsp_array_new = (BLAST_HSPPtr PNTR) MemNew((current_hitlist->hspmax+BLAST_HSP_ADD)*sizeof(BLAST_HSPPtr));
		if (hsp_array_new == NULL)
		{
			ErrPostEx(SEV_WARNING, 0, 0, "UNABLE to reallocate in BlastSaveCurrentHsp for ordinal id %ld, continuing with fixed array of %ld HSP's", (long) search->subject_id, (long) hspmax);
			current_hitlist->do_not_reallocate = TRUE; 
		} 
		else
		{
			Nlm_MemCopy(hsp_array_new, hsp_array, current_hitlist->hspmax*sizeof(BLAST_HSPPtr));
			current_hitlist->hsp_array = MemFree(current_hitlist->hsp_array);
			current_hitlist->hsp_array = hsp_array_new;
			current_hitlist->hspmax += BLAST_HSP_ADD;
			hspmax = current_hitlist->hspmax;
			hsp_array = hsp_array_new;
			/* Prohibit future allocations. */
			if (search->pbp->hsp_num_max != 0 && current_hitlist->hspmax >= 2*search->pbp->hsp_num_max)
			{
				ErrPostEx(SEV_WARNING, 0, 0, "Reached max %ld HSPs in BlastSaveCurrentHsp, continuing with this limit",
					(long) hspmax);
				current_hitlist->do_not_reallocate = TRUE; 
			}
		}
                if (current_hitlist->do_not_reallocate) {
                   HeapSort(hsp_array, hspcnt, sizeof(BLAST_HSPPtr), 
                            score_compare_hsps);
                }
	}

	new_hsp = (BLAST_HSPPtr) MemNew(sizeof(BLAST_HSP));
	new_hsp->score = score;
	new_hsp->query.offset = q_offset;
	new_hsp->subject.offset = s_offset;
	new_hsp->query.length = length;
	new_hsp->subject.length = length;
	new_hsp->query.end = q_offset + length;
	new_hsp->subject.end = s_offset + length;
	new_hsp->context = context;
	new_hsp->query.frame = ContextToFrame(search, context);
	new_hsp->subject.frame = search->subject->frame;

    new_hsp->query.gapped_start   = query_gap_start;
    new_hsp->subject.gapped_start = subject_gap_start;

/* If we are saving ALL HSP's, simply save and sort later. */
	if (current_hitlist->do_not_reallocate == FALSE)
	{
		hsp_array[current_hitlist->hspcnt] = new_hsp;
		(current_hitlist->hspcnt)++;
		return;
	}

/* Use a binary search to insert the HSP. */

	if (hspcnt != 0)
	{
		highscore = hsp_array[0]->score;
		lowscore = hsp_array[hspcnt-1]->score;
	}
	else
	{
		highscore = 0;
		lowscore = 0;
	}

	if (score >= highscore)
	{
		new_index = 0;
	}
	else if (score <= lowscore)
	{
		new_index = hspcnt;
	}
	else
	{
		low_index = 0;
		high_index = hspcnt-1;
		new_index = (low_index+high_index)/2;
		old_index = new_index;

		for (index=0; index<BLAST_SAVE_ITER_MAX; index++)
		{
			if (score > hsp_array[new_index]->score)
			{
				high_index = new_index;
			}
			else
			{
				low_index = new_index;
			}
			new_index = (low_index+high_index)/2;
                        if (new_index == old_index)
                        { /* Perform this check as new_index get rounded DOWN a
bove.*/
                                if (score < hsp_array[new_index]->score)
                                {
                                        new_index++;
                                }
                                break;
                        }
                        old_index = new_index;
		}
	}

	if (hspcnt >= hspmax)
	{
		if (new_index >= hspcnt)
		{ /* this HSP is less significant than others on a full list.*/
			new_hsp = MemFree(new_hsp);
			return;
		}
		else
		{ /* Delete the last HPS on the list. */
			hspcnt = --current_hitlist->hspcnt;
			hsp_array[hspcnt] = BLAST_HSPFree(hsp_array[hspcnt]);
		}
	}
	current_hitlist->hspcnt++;
	Nlm_MemMove((hsp_array+new_index+1), (hsp_array+new_index), (hspcnt-new_index)*sizeof(hsp_array[0]));
	hsp_array[new_index] = new_hsp;

	return;
}

Uint1Ptr
GetSequenceWithDenseSeg(DenseSegPtr dsp, Boolean query, Int4Ptr start, Int4Ptr length)

{
	BioseqPtr bsp;
	Int4 index, offset;
	SeqIdPtr id;
	SeqPortPtr spp;
	Uint1Ptr buffer;
	Boolean startSet = FALSE;

	if (dsp == NULL)
		return NULL;

	if (query == TRUE)
	{
		offset = 0;
		id = dsp->ids;
	}
	else
	{
		offset = 1;
		id = dsp->ids->next;
	}

	*length = 0;
	for (index=0; index<dsp->numseg; index++)
	{
	  if (dsp->starts[offset+2*index] != -1) {
	    *length += dsp->lens[index];
	    if (!startSet) {
	      *start = dsp->starts[offset + 2*index];
	      startSet = TRUE;
	    }
	  }
	}

	bsp = BioseqLockById(id);
    if (bsp == NULL) {
        Char buf[1024];
        StringCpy(buf, "Failed to retrieve sequence ");
        SeqIdWrite(id, &buf[StringLen(buf)], PRINTID_FASTA_LONG,
                   sizeof(buf)-StringLen(buf)-1);
        ErrPostEx(SEV_WARNING, 0, 0, buf);
        return NULL;
    }

	spp = SeqPortNew(bsp, *start, (*start)+(*length)-1, Seq_strand_unknown, Seq_code_ncbistdaa);

	buffer = MemNew((*length)*sizeof(Uint1));

	for (index=0; index<*length; index++)
		buffer[index] = SeqPortGetResidue(spp);

	spp = SeqPortFree(spp);
	BioseqUnlock(bsp);

	return buffer;
}

/* 
Produces a 'fake' BioseqPtr, for use with BLAST when the
ID of the original BioseqPtr cannot be trusted.  Note that
the ID of the original BioseqPtr is removed. 
*/

BioseqPtr LIBCALL
BlastMakeFakeBioseq(BioseqPtr bsp, CharPtr name)

{
	BioseqPtr fake_bsp;
	ObjectIdPtr obidp;

	if (bsp == NULL)
		return NULL;

        fake_bsp = BioseqNew();
        fake_bsp->descr = bsp->descr;
        fake_bsp->repr = bsp->repr;
        fake_bsp->mol = bsp->mol;
        fake_bsp->length = bsp->length;
        fake_bsp->strand = bsp->strand;
        fake_bsp->seq_data_type = bsp->seq_data_type;
        fake_bsp->seq_ext_type = bsp->seq_ext_type;
        fake_bsp->seq_data = bsp->seq_data;
        fake_bsp->seq_ext = bsp->seq_ext;

        obidp = ObjectIdNew();
	if (name)
   	     obidp->str = StringSave(name);
	else
   	     obidp->str = StringSave("QUERY");
        ValNodeAddPointer(&(fake_bsp->id), SEQID_LOCAL, obidp);

        SeqMgrAddToBioseqIndex (fake_bsp);

	return fake_bsp;
}

BioseqPtr LIBCALL
BlastDeleteFakeBioseq(BioseqPtr fake_bsp)

{
	if (fake_bsp == NULL)
		return NULL;

         fake_bsp->descr = NULL;
         fake_bsp->length = 0;
         fake_bsp->seq_data = NULL;
         fake_bsp->seq_ext = NULL;

	return BioseqFree(fake_bsp);
}

/* Comparison function for sorting gi list */
static int Int4Compare(const void* i, const void* j)
{
  if (*(Int4Ptr)i > *(Int4Ptr)j)
    return (1);
  if (*(Int4Ptr)i < *(Int4Ptr)j)
    return (-1);
  return (0);
}

/* Remove hits from a SeqAlignPtr that are not from a gi list. The function
 * is optimized with an assumption that the incoming gi list is not sorted.
 * Since sorting of the gi list may be expensive, the hit gis are found 
 * and sorted. Then for each gi in the (presumably large) incoming gi list,
 * a binary search is performed to check if it is present in the list of hit
 * gis. This procedure is linear in the gi list size. 
 */
SeqAlignPtr 
BlastPruneSeqAlignByGiList(SeqAlignPtr seqalign, Int4Ptr gi_list, 
                           Int4 gi_list_total, Int4 hitlist_size)
{
   SeqAlignPtr head = NULL, last_sap = NULL, next_sap, sap;
   SeqIdPtr sip;
   BioseqPtr bsp;
   Int4 gi = 0, index;
   Int4* hit_gis;
   Int4 num_hit_gis, gi_index;
   Boolean* good_gis;
   Boolean good_gi = FALSE;

   if (!gi_list || gi_list_total <= 0)
      return NULL;

   /* If the size of the gi list is small, sort it and use a different
      routine, which takes a sorted list argument. */
      
   if (hitlist_size >= gi_list_total) {
      qsort((void*)gi_list, gi_list_total, sizeof(Int4), Int4Compare);
      return BlastPruneSeqAlignBySortedGiList(seqalign, gi_list, 
                                              gi_list_total);
   }

   hit_gis = (Int4*) MemNew(hitlist_size*sizeof(Int4));

   gi = 0;
   index = 0;
   /* Find all subject gis in the Seq-align chain */
   for (sap = seqalign; sap; sap = sap->next) {
      sip = SeqAlignId(sap, 1);
      if (sip->choice != SEQID_GI) {
         bsp = BioseqLockById(sip);
         if (bsp) {
            sip = SeqIdFindBest(bsp->id, SEQID_GI);
            BioseqUnlock(bsp);
         }
      }
      if (sip->choice == SEQID_GI) {
         /* Save this gi if the previous value of gi 
            is different from the current value. */
         if (gi != sip->data.intvalue) {
           gi = sip->data.intvalue;
           hit_gis[index] = gi;
           ++index;
         }
      }
   }
   num_hit_gis = index;
   qsort((void*)hit_gis, num_hit_gis, sizeof(Int4), 
         Int4Compare);
   good_gis = (Boolean*) MemNew(num_hit_gis*sizeof(Boolean));

   for (index = 0; index < gi_list_total; ++index) {
      gi_index = BinarySearchInt4(gi_list[index], hit_gis, num_hit_gis);
      if (hit_gis[gi_index] == gi_list[index])
         good_gis[gi_index] = TRUE;
   }

   for (sap = seqalign; sap; sap = next_sap) {
      next_sap = sap->next;
      sip = SeqAlignId(sap, 1);
      if (sip->choice != SEQID_GI) {
         bsp = BioseqLockById(sip);
         if (bsp) {
            sip = SeqIdFindBest(bsp->id, SEQID_GI);
            BioseqUnlock(bsp);
         }
      }
      if (sip->choice == SEQID_GI) {
         /* Do the following check only if the previous value of gi 
            is different from the current value. */
         if (gi != sip->data.intvalue) {
            gi = sip->data.intvalue;
            index = BinarySearchInt4(gi, hit_gis, num_hit_gis);
            good_gi = good_gis[index];
         }
      } else {
         good_gi = FALSE;
      }
      if (good_gi) {
         /* Advance the pointer to the last link in the pruned chain to 
            the current Seq-align. */
         if (head == NULL)
            head = last_sap = sap;
         else {
            last_sap = sap;
         }
      } else {
         /* Link last Seq-align in the pruned chain to the next Seq-align
            in the original chain. */
         if (last_sap)
            last_sap->next = sap->next;
         sap->next = NULL;
         /* Free this Seq-align, since it's no longer needed. */
         sap = SeqAlignFree(sap);
      }
   }


   return head;
}

/* Remove hits from a SeqAlignPtr that are not from a sorted gi list.
 * No check is made that incoming gi list is sorted. User must make
 * sure that it is that way. The pruning is done by a single pass over the
 * list of Seq-aligns, in which a binary search is performed for any new 
 * subject gi to check if it is present in the gi list.
 */
SeqAlignPtr 
BlastPruneSeqAlignBySortedGiList(SeqAlignPtr seqalign, Int4Ptr gi_list, 
                                 Int4 gi_list_total)
{
   SeqAlignPtr head = NULL, last_sap = NULL, next_sap, sap;
   SeqIdPtr sip;
   BioseqPtr bsp;
   Int4 gi = 0;
   Boolean good_gi = FALSE;

   if (!gi_list || gi_list_total <= 0)
      return NULL;

   /* Find all subject gis in the Seq-align chain */
   for (sap = seqalign; sap; sap = next_sap) {
      next_sap = sap->next;
      sip = SeqAlignId(sap, 1);
      if (sip->choice != SEQID_GI) {
         bsp = BioseqLockById(sip);
         if (bsp) {
            sip = SeqIdFindBest(bsp->id, SEQID_GI);
            BioseqUnlock(bsp);
         }
      }
      if (sip->choice == SEQID_GI) {
         /* Do the following check only if the previous value of gi is
            different from the current value. Otherwise the "good_gi" 
            variable is left with its previous value. */
         if (gi != sip->data.intvalue) {
            Int4 index;
            gi = sip->data.intvalue;
            index = BinarySearchInt4(gi, gi_list, gi_list_total);
            good_gi = (gi_list[index] == gi); 
         }
      } else {
         good_gi = FALSE;
      }

      if (good_gi) {
         /* Advance the pointer to the last link in the pruned chain to 
            the current Seq-align. */
         if (head == NULL)
            head = last_sap = sap;
         else {
            last_sap = sap;
         }
      } else {
         /* Link last Seq-align in the pruned chain to the next Seq-align
            in the original chain. */
         if (last_sap)
            last_sap->next = sap->next;
         sap->next = NULL;
         /* Free this Seq-align, since it's no longer needed. */
         sap = SeqAlignFree(sap);
      }
   }

   return head;
}

/* 
   Remove hits from a SeqAlignPtr that are not within an expect 
   value range
*/

SeqAlignPtr 
BlastPruneSeqAlignByEvalueRange(SeqAlignPtr seqalign, FloatHi expect_low,
                           FloatHi expect_high)
{
   SeqAlignPtr head = NULL, last_sap = NULL, sap, next_sap;
   Int4 score, number;
   FloatHi evalue, bit_score;
   SeqIdPtr sip = NULL;

   for (sap = seqalign; sap; sap = next_sap) {
      next_sap = sap->next;
      GetScoreAndEvalue(sap, &score, &bit_score, &evalue, &number);
      if (evalue >= expect_low && evalue <= expect_high) {
         /* Leave this Seq-align */
         if (head == NULL)
            head = last_sap = sap;
         else {
            last_sap = sap;
         }

         if (sip && SeqIdComp(TxGetSubjectIdFromSeqAlign(sap), sip) 
             == SIC_YES) {
            /* Add message about deleted high scoring hits */
            MakeBlastScore(&sap->score, "warning", expect_low, 0);
         }
         sip = NULL;

      } else {
         if (evalue < expect_low && sip == NULL) {
            sip = TxGetSubjectIdFromSeqAlign(sap);
         }
         /* Remove this Seq-align: link last Seq-align in the pruned
            chain to the next Seq-align in the original chain. */
         if (last_sap)
            last_sap->next = sap->next;
         sap->next = NULL;
         /* Free this Seq-align, since it's no longer needed. */
         sap = SeqAlignFree(sap);
      }

   }
   return head;
}

/*
	Returns the program name for a given program number.
	The caller must delete the returned string.
*/

CharPtr LIBCALL
BlastGetProgramName(Uint1 number)

{
	CharPtr string=NULL;

	switch (number) {

		case blast_type_blastn:
			string = StringSave("blastn");
		break;
		case blast_type_blastp:
			string = StringSave("blastp");
		break;
		case blast_type_blastx:
			string = StringSave("blastx");
		break;
		case blast_type_tblastn:
			string = StringSave("tblastn");
		break;
		case blast_type_tblastx:
			string = StringSave("tblastx");
		break;
	        case blast_type_psitblastn:
                        string = StringSave("psitblastn");
		break;
		default:
			string = NULL;
		break;
	}

	return string;
}

/*
	Returns the program number for a string containing the
	program name.
*/

Uint1 LIBCALL
BlastGetProgramNumber(CharPtr blast_program)

{
	if (blast_program == NULL)
		return blast_type_undefined;

	if (StringICmp("blastn", blast_program) == 0)
	{
		return blast_type_blastn;
	}
	else if (StringICmp("blastp", blast_program) == 0)
	{
		return blast_type_blastp;
	}
	else if (StringICmp("blastx", blast_program) == 0)
	{
		return blast_type_blastx;
	}
	else if (StringICmp("tblastn", blast_program) == 0)
	{
		return blast_type_tblastn;
	}
        else if (StringICmp("psitblastn", blast_program) == 0)
	  {
                return blast_type_psitblastn;
	  }
	else if (StringICmp("tblastx", blast_program) == 0)
	{
		return blast_type_tblastx;
	}

	return blast_type_undefined;
}

/*
	returns information aobut the db and query types (protein or dna)
	as well as the 'align_type' that should be attached to the SeqAnnot
	for formatting.

	If an invalid program is entered, then 0 is returned.
*/

Uint1 LIBCALL
BlastGetTypes(CharPtr blast_program, Boolean PNTR query_is_na, Boolean PNTR db_is_na)

{
	Uint1 align_type=0;

	align_type = BlastGetProgramNumber(blast_program);
        if(align_type == blast_type_undefined) {
            ErrPostEx(SEV_ERROR, 0,0, "Program name undefined: \"%s\"",
                      blast_program); 
            return blast_type_undefined;
        }

	if (align_type == blast_type_blastn)
	{
		*query_is_na = TRUE;
		*db_is_na = TRUE;
	}
	else if (align_type == blast_type_blastp)
	{
		*query_is_na = FALSE;
		*db_is_na = FALSE;
	}
	else if (align_type == blast_type_blastx)
	{
		*query_is_na = TRUE;
		*db_is_na = FALSE;
	}
	else if (align_type == blast_type_tblastn)
	{
		*query_is_na = FALSE;
		*db_is_na = TRUE;
	}
        else if (align_type == blast_type_psitblastn)
	{
                *query_is_na = FALSE;
                *db_is_na = TRUE;
	}
	else if (align_type == blast_type_tblastx)
	{
		*query_is_na = TRUE;
		*db_is_na = TRUE;
	}

	return align_type;
}
 

#if BLAST_ALTIVEC

typedef union SignedLongVecs {
	vector signed int   v[2];
	Int4				i[8];
} SignedLongVecs;

Int2
BlastNtFindWords_vec_wlen8(BlastSearchBlkPtr search, Int4 start, Int4 len, Int1 context_index) 
{
	register Int4 			offset, initial_wordsize, reduced_wordsize;
	Boolean 				found_ambig, saved_index=FALSE;
	BLAST_ScorePtr PNTR		matrix;
	BLAST_WordFinderPtr		wfp;
	Int4 					end, index, index_addition, lookup_index, stop;
	LookupTablePtr			lookup;
	Uint1Ptr 				str;
	ValNodePtr     			vnp, vnp_start=NULL;
	Int4					nucleotideSize;
	Int4					indices_per_vector = 8;
	vector unsigned char	vInLeft, vInRight;
	vector unsigned char	vIn;
	vector unsigned char	vOnlyUsed;
	vector unsigned char	vCurrent;
	vector unsigned char	vLoadPerm;
	vector signed short		vecUnpacked1, vecUnpacked2, vecUnpacked3, vecUnpacked4;
	vector signed int		vecMerged1, vecMerged2, vecMerged3, vecMerged4;
	SignedLongVecs			vLookupEntries;
	
	matrix = search->sbp->matrix;
	
	if (search == NULL)
	{
		return -1;
	}
	
	wfp = search->wfp;
	if (wfp == NULL)
	{
		return -2;
	}
	
	lookup = wfp->lookup;
	if (lookup == NULL)
	{
		return -3;
	}
	
	initial_wordsize = (lookup->wordsize)*(wfp->compression_ratio);
	reduced_wordsize = (lookup->reduced_wordsize);
	
	vnp = search->context[context_index].location;
	if (vnp == NULL)
	{
		ValNodeAddInt(&vnp, 1, -1);
		vnp_start = vnp;
		ValNodeAddInt(&vnp, 0, len);
	}
	
    while (vnp)
	{
		if (vnp->choice == 1)
		{
			start = vnp->data.intvalue + 1;
			vnp = vnp->next;
			if (vnp == NULL)
				end = len;		
		}
		if (vnp && vnp->choice == 0)
		{
			end = vnp->data.intvalue - initial_wordsize;
			vnp = vnp->next;
		}
		
		end = MIN(end, len-initial_wordsize);
		
		str = (Uint1Ptr) search->context[context_index].query->sequence + start;
		
		for (offset=start; offset<end-15; offset+=8, str+=8)
		{
			vInLeft 	= vec_lvx(0, (vector unsigned char*)str);
			vInRight 	= vec_lvx(16, (vector unsigned char*)str);
			vLoadPerm 	= vec_lvsl(0, str);
			
			vIn = vec_perm(vInLeft, vInRight, vLoadPerm);
			
			// do 1
			vecUnpacked1 	= (vector signed short)vec_mergeh((vector unsigned char)(0), vIn);
			vecMerged1 		= vec_msum(vecUnpacked1, (vector signed short)(1<<14, 1<<12, 1<<10, 1<<8, 1<<6, 1<<4, 1<<2, 1<<0), (vector signed int)(0));
			vecMerged1		= vec_sums(vecMerged1, (vector signed int)(0));										
			
			// do 2
			vecUnpacked2 	= (vector signed short)vec_mergeh((vector unsigned char)(0), vec_sld(vIn, vIn, 1));
			vecMerged2 		= vec_msum(vecUnpacked2, (vector signed short)(1<<14, 1<<12, 1<<10, 1<<8, 1<<6, 1<<4, 1<<2, 1<<0), (vector signed int)(0));
			vecMerged2		= vec_sums(vecMerged2, (vector signed int)(0));										
			
			// do 3
			vecUnpacked3 	= (vector signed short)vec_mergeh((vector unsigned char)(0), vec_sld(vIn, vIn, 2));
			vecMerged3 		= vec_msum(vecUnpacked3, (vector signed short)(1<<14, 1<<12, 1<<10, 1<<8, 1<<6, 1<<4, 1<<2, 1<<0), (vector signed int)(0));
			vecMerged3		= vec_sums(vecMerged3, (vector signed int)(0));										
			
			// do 4
			vecUnpacked4 	= (vector signed short)vec_mergeh((vector unsigned char)(0), vec_sld(vIn, vIn, 3));
			vecMerged4 		= vec_msum(vecUnpacked4, (vector signed short)(1<<14, 1<<12, 1<<10, 1<<8, 1<<6, 1<<4, 1<<2, 1<<0), (vector signed int)(0));
			vecMerged4		= vec_sums(vecMerged4, (vector signed int)(0));										
			
			vec_st(vec_mergel(vec_mergel(vecMerged1, vecMerged3), vec_mergel(vecMerged2, vecMerged4)), 0, &vLookupEntries.v[0]);
			
			//////////////////////////////////////////////
			
			// do 5
			vecUnpacked1 	= (vector signed short)vec_mergeh((vector unsigned char)(0), vec_sld(vIn, vIn, 4));
			vecMerged1 		= vec_msum(vecUnpacked1, (vector signed short)(1<<14, 1<<12, 1<<10, 1<<8, 1<<6, 1<<4, 1<<2, 1<<0), (vector signed int)(0));
			vecMerged1		= vec_sums(vecMerged1, (vector signed int)(0));										
			
			// do 6
			vecUnpacked2 	= (vector signed short)vec_mergeh((vector unsigned char)(0), vec_sld(vIn, vIn, 5));
			vecMerged2 		= vec_msum(vecUnpacked2, (vector signed short)(1<<14, 1<<12, 1<<10, 1<<8, 1<<6, 1<<4, 1<<2, 1<<0), (vector signed int)(0));
			vecMerged2		= vec_sums(vecMerged2, (vector signed int)(0));										
			
			// do 7
			vecUnpacked3 	= (vector signed short)vec_mergeh((vector unsigned char)(0), vec_sld(vIn, vIn, 6));
			vecMerged3 		= vec_msum(vecUnpacked3, (vector signed short)(1<<14, 1<<12, 1<<10, 1<<8, 1<<6, 1<<4, 1<<2, 1<<0), (vector signed int)(0));
			vecMerged3		= vec_sums(vecMerged3, (vector signed int)(0));										
			
			// do 8
			vecUnpacked4 	= (vector signed short)vec_mergeh((vector unsigned char)(0), vec_sld(vIn, vIn, 7));
			vecMerged4 		= vec_msum(vecUnpacked4, (vector signed short)(1<<14, 1<<12, 1<<10, 1<<8, 1<<6, 1<<4, 1<<2, 1<<0), (vector signed int)(0));
			vecMerged4		= vec_sums(vecMerged4, (vector signed int)(0));										
			
			vec_st(vec_mergel(vec_mergel(vecMerged1, vecMerged3), vec_mergel(vecMerged2, vecMerged4)), 0, &vLookupEntries.v[1]);
			
			for (index = 0; index<8; index++) {
				vector unsigned char	vFirst8 = vec_mergeh(vIn, vIn);
				if (vec_all_lt(vFirst8, (vector unsigned char)(4))) {
					Int4	entry = vLookupEntries.i[index];
					lookup_add_index(lookup, entry, index+offset+(reduced_wordsize*(wfp->compression_ratio)), context_index);
					saved_index = TRUE;
				}
				
				vIn = vec_sld(vIn, vIn, 1);
			}
		}
	}
	
	if (vnp_start)
	{
		vnp_start = ValNodeFree(vnp_start);
	}
	
	if (saved_index == FALSE)
		return 1;
	
	return 0;
}
#endif // BLAST_ALTIVEC





/*
	Find the word hits for a nucl. query.  No neighbors are found here.
	If no indices are saved, then return 1, indicating that the
	search should not be performed.

*/

Int2
BlastNtFindWords(BlastSearchBlkPtr search, Int4 start, Int4 len, Int1 context_index) 
{
	register Int4 offset, initial_wordsize, reduced_wordsize;
	Boolean found_ambig, saved_index=FALSE;
	BLAST_WordFinderPtr	wfp;
	Int4 end, index, index_addition, lookup_index, stop;
	LookupTablePtr		lookup;
	Uint1Ptr str;
	ValNodePtr              vnp, vnp_start=NULL;


	if (search == NULL)
	{
		return -1;
	}

	wfp = search->wfp;
	if (wfp == NULL)
	{
		return -2;
	}

	lookup = wfp->lookup;
	if (lookup == NULL)
	{
		return -3;
	}

	initial_wordsize = (lookup->wordsize)*(wfp->compression_ratio);
	reduced_wordsize = (lookup->reduced_wordsize);

	//#if BLAST_ALTIVEC
#if 0
	if (lookup->true_wordsize_bits == 8 || initial_wordsize == 8) {
		return BlastNtFindWords_vec_wlen8(search, start, len, context_index);		
	}
#endif	

	vnp = search->context[context_index].location;
	if (vnp == NULL)
	{
		ValNodeAddInt(&vnp, 1, -1);
		vnp_start = vnp;
		ValNodeAddInt(&vnp, 0, len);
	}

        while (vnp)
	{
	    if (vnp->choice == 1)
	    {
	    	start = vnp->data.intvalue + 1;
	    	vnp = vnp->next;
	    	if (vnp == NULL)
	    		end = len;
	    }
	    if (vnp && vnp->choice == 0)
	    {
	    	end = vnp->data.intvalue - initial_wordsize;
	    	vnp = vnp->next;
	    }

	    end = MIN(end, len-initial_wordsize);

	    str = (Uint1Ptr) search->context[context_index].query->sequence + start;
		
	    for (offset=start; offset<end; offset++, str++)
	    {
		found_ambig= FALSE;
		lookup_index = 0;
		stop = reduced_wordsize;
		index_addition = 0;
		for (index=0; index<stop; index++)
		{
			if (*(str+index_addition) > 3 || *(str+index_addition+1) > 3 || *(str+index_addition+2) > 3 || *(str+index_addition+3) > 3)
			{
				found_ambig = TRUE;
				break;
			}

			lookup_index += (*(str+index_addition)   << 6);
			lookup_index += (*(str+1+index_addition) << 4);
			lookup_index += (*(str+2+index_addition) << 2);
			lookup_index += *(str+3+index_addition);

			if (index != stop-1)
			{	/* 8 bits/byte */
					lookup_index <<= 8;
			  		index_addition += 4;
			}
		}
			
		if (found_ambig == FALSE)
		{
			lookup_add_index(lookup, (Int4) lookup_index, offset+(reduced_wordsize*(wfp->compression_ratio)), context_index);
			saved_index = TRUE;
		}
	    }
	}

	if (vnp_start)
	{
		vnp_start = ValNodeFree(vnp_start);
	}

	if (saved_index == FALSE)
		return 1;

	return 0;
}

/*
	This functions finds the words.
	return values:
		0: success, words saved
		1: no words saved, no error
		-1: error

*/
		    
	
Int2 LIBCALL
BlastFindWords(BlastSearchBlkPtr search, Int4 start, Int4 len, BLAST_Score threshold, Int1 context_index) 

{
	register Uint1		last_char, last_char2;
	Uint1Ptr		words, PNTR array;
	Uint1Ptr		s_string_start, s_string;
	register Uint1Ptr	str;
	BLAST_Score		best_total, delta_score, diff, diff2, first_score;
	BLAST_Score		second_score, start_score, start_score2, score;
	BLAST_ScoreBlkPtr 	sbp;
	register BLAST_ScorePtr PNTR	matrix;
	BLAST_WordFinderPtr	wfp;
	Boolean			exact_match, saved_index=FALSE;
	LookupTablePtr		lookup;
	register Int4		index1, index3, offset;
	register Int1		index2;
	Int4			num_of_cols, alphabet_size, wordsize;
	Int4 			loop_increment, loop_increment2, stop;
	SeqCodeTablePtr 	sctp;
	ValNodePtr		vnp, vnp_start;

	sbp = search->sbp;
	matrix = sbp->matrix;
	str = (Uint1Ptr) search->context[context_index].query->sequence + start;
	wfp = search->wfp;
	if (wfp == NULL)
		return -2;
	lookup = wfp->lookup;
	if (lookup == NULL)
		return -3;
	wordsize = wfp->wordsize;


	sctp = SeqCodeTableFindObj(sbp->alphabet_code);
	alphabet_size=sctp->num;
	if (search->all_words == NULL)
	{
		search->all_words = BlastPopulateAllWordArrays(wordsize, alphabet_size);
		if (search->all_words == NULL)
		{
			return -1;
		}
		num_of_cols = search->all_words->num_of_cols;
		array = search->all_words->array;
	}
	else
	{
		num_of_cols = search->all_words->num_of_cols;
		array = search->all_words->array;
	}

	/* Index a specific small set, such as one db sequence. */
	if (search->all_words->specific)
	{
		len -= (wordsize-1);
		for (offset=start; offset<len; offset++, str++)
		{
			for (index1=0; index1<num_of_cols; index1++)
			{
				Boolean		ExactMatch = TRUE;
				words = array[index1];
				score = 0;
				for (index2=0; index2<wordsize; index2++)
				{
					score += matrix[*(str+index2)][*(words+index2)];
					if (*(str+index2) != *(words+index2))
					    ExactMatch = FALSE;
				}	
	/* If score is above threshold or an exact match gives a non-zero value. */
		     	     	if (score >= threshold || (ExactMatch && score > 0))
			     	{
					lookup_add(lookup, (CharPtr) words, offset+wordsize-1, context_index);
					saved_index = TRUE;
				}
			}
		}

		if (saved_index)
		    return 0;
		else
		    return 1;
	}

	s_string_start = s_string = MemNew((wordsize+2)*sizeof(Uint1));

	if (s_string_start == NULL)
		return -1;

/* Amounts to advance loops if the same character is to be checked again. */
	loop_increment=(long) (Nlm_Powi((Nlm_FloatHi)alphabet_size,(wordsize-2)));
	loop_increment2=loop_increment/alphabet_size;
/* Shorten len so up to the last complete word is checked. */
	len -= (wordsize-1);

	vnp_start = NULL;
	vnp = search->context[context_index].location;
	if (vnp == NULL)
	{
		ValNodeAddInt(&vnp, 1, -1);
		ValNodeAddInt(&vnp, 0, len+wordsize);
		vnp_start = vnp;
	}

    while (vnp)
	{
		if (vnp->choice == 1)
		{
			start = vnp->data.intvalue + 1;
			vnp = vnp->next;
			if (vnp == NULL)
				stop = len;
		}
		if (vnp && vnp->choice == 0)
		{
			stop = vnp->data.intvalue - (wordsize-1);
			vnp = vnp->next;
		}

		stop = MIN(stop, len);

		str = (Uint1Ptr) search->context[context_index].query->sequence + start;
		
	for (offset=start; offset<stop; offset++, str++)
	{
/* Put query into the lookup table, after checking that word would give
a positive value. */
	/* These are the exact matches. */
		best_total=0;
		for (index1=0; index1<wordsize; index1++)
		{
		    best_total += matrix[(Int4) *(str+index1)][(Int4) *(str+index1)];
		}
		if (best_total > 0)
		{
			lookup_add(lookup, (CharPtr) str, offset+wordsize-1, context_index);
			saved_index = TRUE;
		}

/* Check if a match with a non-identical word could give a score above T. */
		best_total=0;
		for (index1=0; index1<wordsize; index1++)
		{
			best_total += sbp->maxscore[str[index1]];
		}

		if (best_total < threshold)
		{	/* no chance of a match! */
			continue;
		}

		delta_score = best_total-threshold;

/* pick a last_char that is at end of the array, could this be improved? */
		last_char=array[num_of_cols-1][wordsize-2];
		last_char2=array[num_of_cols-1][wordsize-2];

		for (index1=0; index1<num_of_cols; index1++)
		{
			words = array[index1];

/* 
only do this check if the letter has changed from last time.  See if
the new letter, matched with the first letter of the word, changes the
total possible score to below threshold.  If so, move ahead to the next letter.
This is repeated with the second letter in the word.

The order of the letters in the first and second columns of array is
important here!								
*/
			if (last_char != *words)
			{
				last_char = *words;
				first_score = matrix[(Int4) *str][(Int4) *words];
				diff = delta_score + first_score - sbp->maxscore[*str];
				if (diff < 0)
				{ 
/* index1 should be advanced by loop_increment, decrement by one as the "for" 
loop above increments by one.	*/
					index1 += loop_increment;
					index1--;
					continue;
				}
				start_score = first_score;
			}

			if (wordsize > 2 && last_char2 != *(words+1) && wordsize != 1)
			{
				last_char2 = *(words+1);
				second_score = matrix[(Int4) *(str+1)][(Int4) *(words+1)];
				diff2 =  second_score - sbp->maxscore[*(str+1)];
				diff2 += diff;
				if (diff2 < 0)
				{
/* index1 should be advanced by loop_increment2, decrement by one as the "for" 
loop above increments by one.	*/
					index1 += loop_increment2;
					index1--;
					continue;
				}
				start_score = second_score+first_score;
			}

			start_score2 = start_score;

			for (index2=2; index2<wordsize-1; index2++)
			{
				start_score2 += matrix[(Int4) *(str+index2)][*(words+index2)];
			}

			for (index2=0; index2<alphabet_size; index2++)
			{
			     score = start_score2;
			     score += matrix[(Int4) *(str+wordsize-1)][index2];

		     	     if (score >= threshold)
			     {
				exact_match=TRUE;
				for (index3=0; index3<wordsize-1; index3++)
				{
					if (*(str+index3) != *(words+index3))
					{
						exact_match=FALSE;
						break;
					}
				}
				if (*(str+wordsize-1) != index2)
				{
					exact_match=FALSE;
				}
						
/* Exact matches were done above, exclude here.  Is this really needed? */
/* Could exact matches just be done here? */
				if (exact_match == FALSE)
				{
					s_string = s_string_start;
					for (index3=0; index3<wordsize-1; index3++)
					{
						*s_string = *(words+index3);
						s_string++;
					}
					*s_string = index2;
					lookup_add(lookup, (CharPtr) s_string_start, offset+wordsize-1, context_index);
					saved_index = TRUE;
				}
			     }
			}
		}
	}
	}

	if (vnp_start)
	{
		vnp_start = ValNodeFree(vnp_start);
	}

	s_string_start = MemFree(s_string_start);

	if (saved_index == FALSE)
		return 1;

	return 0;
}

/*AAS position-based version of BlastFindWords*/
Int2 LIBCALL
BlastNewFindWords_Old(BlastSearchBlkPtr search, Int4 start, Int4 len, BLAST_Score threshold, Int1 context_index) 

{
	register Uint1		last_char, last_char2;
	Uint1Ptr		words, PNTR array;
	Uint1Ptr		s_string_start, s_string;
	register Uint1Ptr	str;
	BLAST_Score		best_total, delta_score, diff, diff2, first_score;
	BLAST_Score		second_score, start_score, start_score2, score;
	BLAST_ScoreBlkPtr 	sbp;
	BLAST_WordFinderPtr	wfp;
	Boolean			exact_match;
	LookupTablePtr		lookup;
	register Int4		index1, index3, offset;
	register Int1		index2;
	Int4			num_of_cols, alphabet_size, wordsize;
	Int4 			loop_increment, loop_increment2;
	SeqCodeTablePtr 	sctp;

	sbp = search->sbp;
	str = (Uint1Ptr) search->context[context_index].query->sequence + start;
	wfp = search->wfp;
	lookup = wfp->lookup;
	wordsize = wfp->wordsize;

	sctp = SeqCodeTableFindObj(sbp->alphabet_code);
	alphabet_size=sctp->num;
	if (search->all_words == NULL)
	{
		search->all_words = BlastPopulateAllWordArrays(wordsize, alphabet_size);
		if (search->all_words == NULL)
		{
			return -1;
		}
		num_of_cols = search->all_words->num_of_cols;
		array = search->all_words->array;
	}
	else
	{
		num_of_cols = search->all_words->num_of_cols;
		array = search->all_words->array;
	}

	/* Index a specific small set, such as one db sequence. */
	if (search->all_words->specific)
	{
		len -= (wordsize-1);
		for (offset=start; offset<len; offset++, str++)
		{
			for (index1=0; index1<num_of_cols; index1++)
			{
				words = array[index1];
				score = 0;
				for (index2=0; index2<wordsize; index2++)
				{
					score += MtrxScorePosSearch(search->sbp,
							offset + index2,
							*(words+index2));
				}	
		     	     	if (score >= threshold)
			     	{
					lookup_add(lookup, (CharPtr) words, offset+wordsize-1, context_index);
				}
			}
		}
		return 0;
	}

	s_string_start = s_string = MemNew((wordsize+2)*sizeof(Uint1));

	if (s_string_start == NULL)
		return 1;

/* Amounts to advance loops if the same character is to be checked again. */
	loop_increment=(long) (Nlm_Powi((Nlm_FloatHi)alphabet_size,(wordsize-2)));
	loop_increment2=loop_increment/alphabet_size;
/* Shorten len so up to the last complete word is checked. */
	len -= (wordsize-1);
	for (offset=start; offset<len; offset++, str++)
	{
/* Put query into the lookup table, after checking that word would give
a positive value. */
		best_total=0;
		for (index1=0; index1<wordsize; index1++)
		{
		    best_total += MtrxScorePosSearch(search->sbp,
					offset+index1,(Int4) *(str+index1));
		}
		if (best_total > 0)
			lookup_add(lookup, (CharPtr) str, offset+wordsize-1, context_index);

/* Check if a match with a non-identical word could give a score above T. */
		best_total=0;
		for (index1=0; index1<wordsize; index1++)
		{
			best_total += sbp->maxscore[str[index1]];
		}

		if (best_total < threshold)
		{	/* no chance of a match! */
			continue;
		}

		delta_score = best_total-threshold;

/* pick a last_char that is at end of the array, could this be improved? */
		last_char=array[num_of_cols-1][wordsize-2];
		last_char2=array[num_of_cols-1][wordsize-2];

		for (index1=0; index1<num_of_cols; index1++)
		{
			words = array[index1];

/* 
only do this check if the letter has changed from last time.  See if
the new letter, matched with the first letter of the word, changes the
total possible score to below threshold.  If so, move ahead to the next letter.
This is repeated with the second letter in the word.

The order of the letters in the first and second columns of array is
important here!								
*/
			if (last_char != *words)
			{
				last_char = *words;
				first_score = MtrxScorePosSearch(search->sbp,
					offset,(Int4) *words);
				diff = delta_score + first_score - sbp->maxscore[*str];
				if (diff < 0)
				{ 
/* index1 should be advanced by loop_increment, decrement by one as the "for" 
loop above increments by one.	*/
					index1 += loop_increment;
					index1--;
					continue;
				}
				start_score = first_score;
			}

			if (last_char2 != *(words+1) && wordsize != 1)
			{
				last_char2 = *(words+1);
				second_score = MtrxScorePosSearch(search->sbp,
						offset+1,(Int4) *(words+1));
				diff2 =  second_score - sbp->maxscore[*(str+1)];
				diff2 += diff;
				if (diff2 < 0)
				{
/* index1 should be advanced by loop_increment2, decrement by one as the "for" 
loop above increments by one.	*/
					index1 += loop_increment2;
					index1--;
					continue;
				}
				start_score = second_score+first_score;
			}

			start_score2 = start_score;

			for (index2=2; index2<wordsize-1; index2++)
			{
				start_score2 += MtrxScorePosSearch(search->sbp,
						offset+index2,*(words+index2));
			}

			for (index2=0; index2<alphabet_size; index2++)
			{
			     score = start_score2;
			     score += MtrxScorePosSearch(search->sbp,
					offset+wordsize-1,index2);

		     	     if (score >= threshold)
			     {
				exact_match=TRUE;
				for (index3=0; index3<wordsize-1; index3++)
				{
					if (*(str+index3) != *(words+index3))
					{
						exact_match=FALSE;
						break;
					}
				}
				if (*(str+wordsize-1) != index2)
				{
					exact_match=FALSE;
				}
						
				if (exact_match == FALSE)
				{
					s_string = s_string_start;
					for (index3=0; index3<wordsize-1; index3++)
					{
						*s_string = *(words+index3);
						s_string++;
					}
					*s_string = index2;
					lookup_add(lookup, (CharPtr) s_string_start, offset+wordsize-1, context_index);

				}
			     }
			}
		}
	}

	s_string_start = MemFree(s_string_start);
	return 0;
}

/* SSH position-based version of BlastFindWords 
   Lookup structure should be allocated at this point*/

Int2 BlastNewFindWordsEx(LookupTablePtr lookup, BLAST_ScorePtr PNTR posMatrix,
                         Int4 start, Int4 len, BlastAllWordPtr all_words, 
                         BLAST_Score threshold, Int4 wordsize, 
                         Int1 context_index)
{
    register Uint1	last_char, last_char2;
    Uint1Ptr		words, PNTR array;
    Uint1Ptr		s_string_start, s_string;
    BLAST_Score		best_total, delta_score, diff, diff2, first_score;
    BLAST_Score		second_score, start_score, start_score2, score;
    register Int4	index1, index3, offset;
    register Int1	index2;
    Int4		num_of_cols, alphabet_size;
    Int4 		loop_increment, loop_increment2;
    Boolean all_words_allocated = FALSE;
    BLAST_ScorePtr      maxscore;
    
    if(lookup == NULL || posMatrix == NULL)
        return -1;
    
    alphabet_size = PSI_ALPHABET_SIZE; /* 26 */
    
    if (all_words == NULL) {
        all_words = BlastPopulateAllWordArrays(wordsize, alphabet_size);
        if (all_words == NULL) {
            return -1;
        }
        
        all_words_allocated = TRUE;
        num_of_cols = all_words->num_of_cols;
        array = all_words->array;
    } else {
        num_of_cols = all_words->num_of_cols;
        array = all_words->array;
    }
    
    /* Index a specific small set, such as one db sequence. 
       This is used when all_words actually a subset of "all_words" found
       in some sequence. Used for ex. in BlastTwoSequences */
    
    if (all_words->specific) {
        len -= (wordsize-1);
        for (offset = start; offset < len; offset++) {
            for (index1 = 0; index1 < num_of_cols; index1++) {
                words = array[index1];
                score = 0;
                for (index2 = 0; index2 < wordsize; index2++)
                    score += posMatrix[offset + index2][*(words+index2)];
                if (score >= threshold) {
                    lookup_add(lookup, (CharPtr) words, offset + wordsize - 1,
                               context_index);
                }
            }
        }

        if(all_words_allocated)
            BlastAllWordDestruct(all_words);

        return 0;
    }
    /* ----------- End of "specific" word finding ------------ */

    /* maxscore matrix will be used position-specific -
       of length = (len - start) */

    maxscore = BlastPSIMaxScoreGet(posMatrix, start, len);
    
    s_string_start = s_string = MemNew((wordsize+2)*sizeof(Uint1));
    
    if (s_string_start == NULL)
        return 1;
    
    /* Amounts to advance loops if the same character is to be checked again. */
    loop_increment=(long) (Nlm_Powi((Nlm_FloatHi)alphabet_size,(wordsize-2)));
    loop_increment2=loop_increment/alphabet_size;

    /* Shorten len so up to the last complete word is checked. */
    len -= (wordsize-1);
    for (offset = start; offset < len; offset++) {
        
        /* Check if a match with a non-identical word could give a score above T. */
        best_total = 0;
        for (index1 = 0; index1 < wordsize; index1++) {
            best_total += maxscore[offset+index1];
        }
        
        if (best_total < threshold) {	/* no chance of a match! */
            continue;
        }
        
        delta_score = best_total - threshold;
        
        /* pick a last_char that is at end of the array, could this be improved? */
        last_char=array[num_of_cols-1][wordsize-2];
        last_char2=array[num_of_cols-1][wordsize-2];
        
        for (index1 = 0; index1 < num_of_cols; index1++) {
            words = array[index1];
            
/* 
only do this check if the letter has changed from last time.  See if
the new letter, matched with the first letter of the word, changes the
total possible score to below threshold.  If so, move ahead to the next letter.
This is repeated with the second letter in the word.

The order of the letters in the first and second columns of array is
important here!								
*/
            if (last_char != *words) {
                last_char = *words;
                
                first_score = posMatrix[offset][*words];
                diff = delta_score + first_score - maxscore[offset];
                
                if (diff < 0) { 
/* index1 should be advanced by loop_increment, decrement by one as the "for" 
loop above increments by one.	*/
                    index1 += loop_increment;
                    index1--;
                    continue;
                }
                start_score = first_score;
            }
            
            if (last_char2 != *(words+1) && wordsize != 1) {
                last_char2 = *(words+1);
                second_score = posMatrix[offset+1][*(words+1)];
                
                diff2 =  second_score - maxscore[offset+1];
                diff2 += diff;
                if (diff2 < 0) {
/* index1 should be advanced by loop_increment2, decrement by one as the "for" 
loop above increments by one.	*/
                    index1 += loop_increment2;
                    index1--;
                    continue;
                }
                start_score = second_score + first_score;
            }
            
            start_score2 = start_score;
            
            for (index2 = 2; index2 < wordsize - 1; index2++) {
                start_score2 += posMatrix[offset+index2][*(words+index2)];
            }
            
            for (index2 = 0; index2 < alphabet_size; index2++) {
                score = start_score2;
                score += posMatrix[offset+wordsize-1][index2];
                
                if (score >= threshold) {
                    s_string = s_string_start;
                    for (index3=0; index3 < wordsize-1; index3++) {
                        *s_string = *(words+index3);
                        s_string++;
                    }
                    *s_string = index2;
                    lookup_add(lookup, (CharPtr) s_string_start, 
                               offset+wordsize-1, context_index);
                }
            }
        }
    }
    
    s_string_start = MemFree(s_string_start);
    
    if(all_words_allocated)
        BlastAllWordDestruct(all_words);
    
    MemFree(maxscore);
    
    return 0;
}
/* SSH position-based version of BlastFindWords*/
Int2 LIBCALL
BlastNewFindWords(BlastSearchBlkPtr search, Int4 start, Int4 len, BLAST_Score threshold, Int1 context_index) 
{
    Int2 status;
    
    status = BlastNewFindWordsEx(search->wfp->lookup, search->sbp->posMatrix,
                                 start, len, search->all_words, threshold, 
                                 search->wfp->wordsize, context_index);
    return status;
}

/*******************************************************************

This function allocates and populates an array containing every possible
letter combination for an alphabet of size alphabet_size for the first
wordsize-1 letters.   The last letter of the word is done on the fly.
The approach is best described with a table that demonstrates
the strategy with a two-letter alphabet and a wordsize of three:

	index   1	2
	col.	1	0

		A	A
		A	B
		B	A
		B	B
		A	A
		A	B
		B	A
		B	B

col. 0: basic pattern repeated N**(W-1) times, where N is the size of the
	alphabet and W is the wordsize.
col. 1: AABB repeated N**(W-2) times.

Each pattern is repeated N**(W-1-C) times, where "C" is the column number.
The number of repeats of a given letter is N**C.
The total number of rows in the array is then N**(W-1-C) * N**C * N = N**W,
as we expect.

NOTE:  The order of the columns is important, as it is used in
BlastWFContextNeighborhoodAdd above.  In particular it is useful to have
all the letters grouped together.
*********************************************************************/

BlastAllWordPtr
BlastPopulateAllWordArrays(Int4 wordsize, Int4 alphabet_size)

{
	BlastAllWordPtr all_words;
	Uint1Ptr *array_ptr, *array;
	Uint1Ptr array_storage;
	register Int4 index, index1, index3, num_of_cols, times, repeat_num;
	register Int1 index2;
	num_of_cols = (Int4) Nlm_Powi((Nlm_FloatHi)alphabet_size, wordsize-1);
	array = (Uint1Ptr *) MemNew(num_of_cols*sizeof(Uint1Ptr));

	array_storage = (Uint1Ptr) MemNew(num_of_cols*(wordsize-1)*sizeof(Uint1));
	for (index=0; index<num_of_cols; index++)
	{
	    array[index] = array_storage+(index*(wordsize-1));
	}

	for (index=0; index<wordsize-1; index++)
	{
	    array_ptr = array;
	    repeat_num= (Int4) Nlm_Powi((Nlm_FloatHi)alphabet_size,(wordsize-2-index));
	    times = (Int4) Nlm_Powi((Nlm_FloatHi)alphabet_size, index);
	    for (index1=0; index1<times; index1++)
	    {
	    	for (index2=0; index2<alphabet_size; index2++)
	    	{
	    	    for (index3=0; index3<repeat_num; index3++)
		    {
			(*array_ptr)[index] = index2;
			array_ptr++;
		    }
		 }
	     }
	}

	all_words = BlastAllWordNew(num_of_cols, wordsize, TRUE, FALSE);
	if (all_words)
	{
		all_words->array = array;
		all_words->array_storage = array_storage;
	}

	return all_words;
}

/**************************************************************************
*
*	Get the "ScoreSet" ScorePtr from the BLAST data, which is provided
*	by "hsp".  "score_set" should be NULL, a chain of scores is added.
**************************************************************************/

ScorePtr LIBCALL
GetScoreSetFromBlastResultHsp(BLASTResultHspPtr hsp, SeqIdPtr gi_list)

{
	ScorePtr	score_set=NULL;
	Nlm_FloatHi	prob;
	Int4		score;
	CharPtr		scoretype;
	
	score = hsp->score;
	if (score > 0)
		MakeBlastScore(&score_set, "score", 0.0, score);

	score = hsp->number;
	scoretype = "sum_n";

	if (score > 1)
		MakeBlastScore(&score_set, scoretype, 0.0, score);

	prob = hsp->e_value;
	if (hsp->number <= 1)
	{
		scoretype = "e_value";
	}
	else 
	{
		scoretype = "sum_e";
	}
	if (prob >= 0.)
	{
		if (prob < 1.0e-180)
			prob = 0.0;
		MakeBlastScore(&score_set, scoretype, prob, 0);
	}

	prob = hsp->bit_score;
	if (prob >= 0.)
		MakeBlastScore(&score_set, "bit_score", prob, 0);

        if (hsp->num_ident > 0)
           MakeBlastScore(&score_set, "num_ident", 0.0, hsp->num_ident);

	if (hsp->number > 1 && hsp->ordering_method == BLAST_SMALL_GAPS)
	{
		MakeBlastScore(&score_set, "small_gap", 0.0, 1);
	} else if (hsp->ordering_method > 3) { 
           /* In new tblastn this means splice junction was found */
           MakeBlastScore(&score_set, "splice_junction", 0.0, 1);
        }

	while (gi_list)
	{
		MakeBlastScore(&score_set, "use_this_gi", 0.0, gi_list->data.intvalue);
		gi_list = gi_list->next;
	}

	return score_set;
}

