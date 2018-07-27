/* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  alignmgr.c
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   7/99
*
* $Revision: 6.178 $
*
* File Description: SeqAlign indexing and messaging functions
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: alignmgr.c,v $
* Revision 6.178  2001/11/09 17:22:34  wheelan
* fixed bug in TruncateSeqAlign
*
* Revision 6.177  2001/08/07 14:39:34  wheelan
* added am_cleanupsalp
*
* Revision 6.176  2001/07/10 16:44:01  wheelan
* added AlnMgrMakeFakeMultipleEx for AlnMgrIndexIndexedSet
*
* Revision 6.175  2001/07/10 11:12:23  wheelan
* added AlnMgrIndexIndexedChain
*
* Revision 6.174  2001/05/30 12:13:58  wheelan
* AlnMsgNew and AlnMsgReNew initialize from_m and to_m
*
* Revision 6.173  2001/04/30 17:51:58  wheelan
* minor bug fix
*
* Revision 6.172  2001/04/19 17:59:58  wheelan
* added protection against NULL strands in AlnMgrIndexSingleChildSeqAlign
*
* Revision 6.171  2001/03/21 19:59:21  hurwitz
* remove AlnMgrMergeNeighbors call from AlnMgrMakeMultByIntersectOnMaster
*
* Revision 6.170  2001/03/08 21:04:39  hurwitz
* rolled back AlnMgrMakeMultByIntersectOnMaster to rev 6.156
*
* Revision 6.169  2001/03/08 17:07:10  wheelan
* added AlnMgrGetParent and structure to support it
*
* Revision 6.168  2001/03/01 19:15:15  wheelan
* fixed bug in MapBioseqToSeqAlign
*
* Revision 6.167  2001/02/16 13:29:38  wheelan
* Added AMFreeAllIndexes
*
* Revision 6.166  2001/02/07 12:04:19  wheelan
* bug fix in AlnMgrGetNthUnalignedForNthRow
*
* Revision 6.165  2001/02/05 13:21:17  wheelan
* bug fix in AlnMgrGetNthUnalignedForNthRow
*
* Revision 6.164  2001/02/01 00:39:18  lewisg
* fix uninitialized variable bugs
*
* Revision 6.163  2001/01/29 12:29:16  wheelan
* fixed bug which missed residues in AlnMgrGetNthUnalignedForNthRow
*
* Revision 6.162  2001/01/25 14:05:11  wheelan
* fixed bug in AlnMgrSetUnalignedLengths
*
* Revision 6.161  2001/01/23 13:35:20  wheelan
* bug fix in AlnMgrConstructOverlaps
*
* Revision 6.160  2001/01/19 03:08:15  bauer
* commented-out debug printfs
*
* Revision 6.159  2001/01/18 19:09:00  wheelan
* added functions to better handle jagged-edged segmented master-slave alignments
*
* Revision 6.158  2001/01/12 20:58:25  wheelan
* backed out prev changes
*
* Revision 6.157  2001/01/12 19:00:29  wheelan
* changes in AlnMgrGetNthUnalignedForNthRow to avoid problems when flanking seqalign regions are NULL for that row
*
* Revision 6.156  2001/01/09 23:18:55  lewisg
* fix memory leaks
*
* Revision 6.155  2001/01/05 20:02:32  wheelan
* fixed some memory leaks
*
* Revision 6.154  2000/10/06 10:34:20  wheelan
* changed behavior of AlnMgrGetSubAlign
*
* Revision 6.153  2000/10/02 13:52:31  wheelan
* fixed memory leak in SAIndexFree
*
* Revision 6.152  2000/09/26 16:10:58  kans
* removed const from AlnMgrCompareSortStruct heapsort callback - error caught by Mac compiler
*
* Revision 6.151  2000/09/26 14:23:49  lewisg
* use AlnMgrSortbyID instead of AlnMgrSortSeqAligns
*
* Revision 6.150  2000/09/25 15:25:36  wheelan
* bug fixes in AlnMgrMapBioseqToSeqAlign
*
* Revision 6.149  2000/09/20 12:20:16  wheelan
* bug fixes in AlnMgrMakeSegmentedMasterSlave to guide better preservation of input row structure
*
* Revision 6.148  2000/09/14 19:37:13  wheelan
* *** empty log message ***
*
* Revision 6.147  2000/09/14 19:32:21  wheelan
* bug fix in AlnMgrMapBioseqToSeqAlign
*
* Revision 6.146  2000/09/14 18:29:46  wheelan
* fixed binary search in MapBioseqToSeqAlign, took out merge behavior of AlnMgrGetSubAlignSpecial
*
* Revision 6.145  2000/09/08 20:34:31  lewisg
* hacks to speed up bioseq to align coord computation
*
* Revision 6.144  2000/09/07 04:53:42  sicotte
* fix alignment calls, bad matrix calls, and misc alignments problems for sequence update
*
* Revision 6.142  2000/09/05 22:28:06  lewisg
* PLEASE DO NOT DELETE THE STARTSIZE FIELD
*
* Revision 6.141  2000/08/30 10:33:55  wheelan
* fixed gcc compiler warnings
*
* Revision 6.140  2000/08/29 20:12:09  lewisg
* speed up color by alignment
*
* Revision 6.139  2000/08/28 16:18:20  sicotte
* moved AlnMgrSeqAlignMergeTwoPairwiseEx AlnMgrSeqAlignMergeTwoPairwise AlnMgrSeqAlignMergePairwiseSet to actutils.c
*
* Revision 6.138  2000/08/28 13:39:00  sicotte
* Get around Indexing bug in AlnMgrSeqAlignMergePairwiseSet
*
* Revision 6.137  2000/08/25 19:24:32  sicotte
* Add many functions to deal with merging alignment to go from pairwise sets to a single global (or local) alignment
*
* Revision 6.136  2000/08/23 20:01:07  hurwitz
* fixed bug in AlnMgrGetMaxUnalignedLength
*
* Revision 6.135  2000/08/18 14:20:50  lewisg
* add startsize field to AMAlignIndex so that lnMgrCopyIndexedParentIntoSap knows how big starts is
*
* Revision 6.134  2000/08/14 14:40:58  lewisg
* bug fixes for mixed alignment
*
* Revision 6.133  2000/08/11 12:53:57  wheelan
* bug fixes in AlnMgrMakeMultipleByScoreExEx
*
* Revision 6.132  2000/08/10 19:09:37  wheelan
* bug fixes in AlnMgrMakeMultipleByScoreExEx
*
* Revision 6.131  2000/07/27 19:38:40  hurwitz
* fixes split block bug
*
* Revision 6.130  2000/07/26 17:26:25  lewisg
* fix code for c++ inclusion
*
* Revision 6.129  2000/07/26 16:48:48  sicotte
* Fix bug and Memory leaks in AlnMgrGetSubAlign wrt SeqIds
*
* Revision 6.128  2000/07/26 14:58:13  sicotte
* bug fixes to AlnMgrGetNextAlnBit. bug fix (overlapping fuzz) in AlnMgrMakeMultipleByScore, Added AlnMgrMakeMultipleByScoreExEx and AlnMgrRemoveInconsistentEx and AlnMgrDeleteHiddenEx to allow optional deletion of sealigns when converting indexes to seqaligns
*
* Revision 6.127  2000/07/25 18:55:53  sicotte
* Added AlnMgrDeleteHiddenEx and AlnMgrRemoveInconsistentFromPairwiseSetEx to make optional deleting of SeqAligns. Needed for Sequence Update
*
* Revision 6.126  2000/07/24 19:07:56  sicotte
* Fix Master-Slave bugs in AlnMgrMakeFakeMultiple and alignment coordinate bugs in AlnMgrGetNextAlnBit
*
* Revision 6.125  2000/07/21 21:36:20  sicotte
* fix bug for sequence update in sequin when the alignment was two
* discontinous seqaligns. Fixed AlnMgrMakeFakeMultiple.
*
* Revision 6.124  2000/07/21 21:07:43  hurwitz
* bug fix when deleting last block and block preceeding it has just one aligned column
*
* Revision 6.123  2000/07/20 22:27:41  hurwitz
* working on bug fixes
*
* Revision 6.122  2000/06/29 23:15:13  hurwitz
* leave single space between aligned blocks with no unaligned sequence between them, no auto-merge of adjacent aligned blocks
*
* Revision 6.121  2000/06/15 14:15:45  wheelan
* alignmgr.c
*
* Revision 6.120  2000/06/02 18:37:45  wheelan
* bug fix in am_is_consistent (for editing)
*
* Revision 6.119  2000/06/01 17:37:46  wheelan
* various bug fixes
*
* Revision 6.118  2000/06/01 14:18:10  wheelan
* added AlnMgrCheckOrdered and AlnMgrMakeRowsForOrdered
*
* Revision 6.117  2000/05/24 15:46:53  wheelan
* added AlnMgrRemoveInconsistentFromPairwiseSet and AlnMgrSortAlnSetByNthRowPos
*
* Revision 6.116  2000/05/23 22:00:14  hurwitz
* working on launch of DDE from DDV
*
* Revision 6.115  2000/05/19 17:52:07  wheelan
* fixed incorrect strands in AlnMgrGetSubAlign
*
* Revision 6.114  2000/05/18 20:54:32  wheelan
* bug fix in AlnMgrIsEditable
*
* Revision 6.113  2000/05/18 11:29:19  wheelan
* finished AlnMgrIsIBMable and AlnMgrIsEditable
*
* Revision 6.112  2000/05/16 17:14:46  wheelan
* added AlnMgrIsIBMable, AlnMgrIsEditable; made am_guess_numrows extern
*
* Revision 6.111  2000/05/15 13:12:21  wheelan
* fixes to AlnMgrAddBlock to allow creation of a new block in the tail of an alignment with only one block
*
* Revision 6.110  2000/05/14 22:28:32  wheelan
* added am_is_new_row to fix row numbering problems in IntersectOnMaster function
*
* Revision 6.109  2000/05/10 16:46:48  wheelan
* bug fix in IntersectByMaster
*
* Revision 6.108  2000/05/10 15:40:24  wheelan
* bug fixes in IntersectOnMaster
*
* Revision 6.107  2000/05/10 13:09:36  wheelan
* bug fix in am_is_consistent; added am_is_ok_block to check newly edited blocks
*
* Revision 6.106  2000/05/09 18:42:49  wheelan
* fixes for editing
*
* Revision 6.105  2000/05/09 14:23:00  wheelan
* added AlnMgrMakeMultipleByScoreEx
*
* Revision 6.104  2000/05/08 13:17:05  wheelan
* added AlnMgrGetNumAlnBlocks and AlnMgrGetNthBlockRange; fixed memory leaks
*
* Revision 6.103  2000/05/05 12:48:12  wheelan
* fixed crash when deleting last block of an alignment
*
* Revision 6.102  2000/05/05 11:53:39  wheelan
* bug fix in AlnMgrMapBioseqToSeqAlign
*
* Revision 6.101  2000/05/04 14:07:45  wheelan
* several changes to correctly merge blocks when edits remove an unaligned region
*
* Revision 6.100  2000/05/03 19:30:37  wheelan
* fixed bugs in NULL alignment handling
*
* Revision 6.99  2000/05/02 19:50:38  hurwitz
* fixed some bugs with launching DDE from DDV, added new alnMgr fn for positioning DDE on proper column
*
* Revision 6.98  2000/05/02 12:00:03  wheelan
* added SASeqDatFree and fixed more memory leaks
*
* Revision 6.97  2000/05/01 19:54:51  wheelan
* fixed memory leaks
*
* Revision 6.96  2000/05/01 13:58:17  wheelan
* fixed am_is_consistent to recognize row rearrangments
*
* Revision 6.95  2000/05/01 12:12:17  wheelan
* fixes in AlnMgrMapBioseqToSeqAlign
*
* Revision 6.94  2000/04/26 21:53:21  hurwitz
* added save function to tell AlnMgr about edits made in DDE
*
* Revision 6.93  2000/04/22 15:54:09  wheelan
* added AlnMgrIndexLite; several assorted bug fixes
*
* Revision 6.92  2000/04/17 17:03:33  wheelan
* fixes in AlnMgrNeatlyIndex and AlnMgrIntersectByMaster
*
* Revision 6.91  2000/04/10 19:35:15  wheelan
* added AlnMgrIsSAPNULL, bug fixes in AlnMgrMakeFakeMultiple, added ability to deal with NULL alignments, completed AlnMgrReplaceBlock and AlnMgrAddBlock
*
* Revision 6.90  2000/04/07 13:21:17  wheelan
* bug fixes in MapBioseqToAlnCoords and free functions
*
* Revision 6.89  2000/04/05 17:41:29  wheelan
* added AlnMgrAddBlock, AlnMgrReplaceBlock, and fixed AlnMgrGetSubAlignSpecial
*
* Revision 6.88  2000/04/04 13:39:14  wheelan
* fixed bug in mapping bioseq coords for segmented alignments
*
* Revision 6.87  2000/04/03 17:20:22  wheelan
* finished AlnMgrGetSubAlignSpecial, bug fix in AlnMgrGetNextAlnBit
*
* Revision 6.86  2000/04/03 12:50:31  wheelan
* bug fixes for partial alignments
*
* Revision 6.85  2000/03/17 14:25:24  wheelan
* changes to AlnMgrGetSubAlign
*
* Revision 6.84  2000/03/16 19:53:32  wheelan
* fixed bug which allowed all-gap columns after AlnMgrForceMasterSlave
*
* Revision 6.83  2000/03/16 15:07:15  wheelan
* bug fix in AlnMgrTruncateByOverlap
*
* Revision 6.82  2000/03/15 20:40:19  lewisg
* bug fixes for AlnMgrCarefulIndex
*
* Revision 6.81  2000/03/10 18:47:01  lewisg
* add show/hide
*
* Revision 6.80  2000/03/09 20:24:20  wheelan
* bug fixes in AlnMgrSetUnalignedLengths and IBM
*
* Revision 6.79  2000/03/07 18:32:22  wheelan
* miscellaneous bug fixes
*
* Revision 6.78  2000/03/03 19:58:35  wheelan
* added AlnMgrDupTopNByScore
*
* Revision 6.77  2000/03/02 20:00:33  wheelan
* bug fixes to more gracefully handle a sequence aligned with itself
*
* Revision 6.76  2000/02/29 18:02:34  wheelan
* added AlnMgrMergeNeighbors to get rid of unaligned regions of length 0 after intersection; bug fixes in copy functions
*
* Revision 6.75  2000/02/28 17:18:14  wheelan
* Added AlnMgrTossNeatRows for Cn3D
*
* Revision 6.74  2000/02/28 14:49:44  wheelan
* added AlnMgrSetUnalignedLengths
*
* Revision 6.73  2000/02/24 18:07:33  wheelan
* bug fixes in alignment truncation function
*
* Revision 6.72  2000/02/23 20:06:05  thiessen
* added missing pointer assignment
*
* Revision 6.71  2000/02/23 18:45:17  wheelan
* added AlnMgrNeatlyIndex for structure alignments, added more careful row indexing, finished AlnMgrMapBioseqToSeqAlign function
*
* Revision 6.70  2000/02/16 15:47:57  wheelan
* changed behavior of AlnMgrMakeMultByIntersectOnMaster
*
* Revision 6.69  2000/02/11 17:30:50  kans
* AlnMgrForcePairwiseContinuous moved to tools/actutils (SW)
*
* Revision 6.68  2000/02/10 19:13:13  wheelan
* bug fixes for IntersectOnMaster
*
* Revision 6.67  2000/02/10 15:20:17  lewisg
* sarah's fixes for < 0 indices
*
* Revision 6.66  2000/02/09 20:23:03  wheelan
* finished AlnMgrDeleteNthRow
*
* Revision 6.65  2000/02/07 16:15:50  wheelan
* added AlnMgrTruncateSAP and several helper functions
*
* Revision 6.64  2000/02/04 22:57:01  kans
* changed BioseqUnlockById to BioseqUnlock to avoid scoping problem
*
* Revision 6.63  2000/02/02 17:55:50  wheelan
*  bug fixes
*
* Revision 6.62  2000/02/02 14:37:30  wheelan
* added AlnMgrGetNthAlignedSegInNthRow and AlnMgrGetNthSegmentRange to make alignment editing easier
*
* Revision 6.60  2000/02/01 13:14:24  wheelan
* took out debugging AsnWrite, bug fixes in AlnMgrGetNextAlnBit and GetNthUnaligned
*
* Revision 6.59  2000/01/31 21:00:53  kans
* changes to AlnMgrForcePairwiseContinuous and AlnMgrGetNthSeqRangeInSA to support Update Sequence with very long sequences in Sequin (SW)
*
* Revision 6.58  2000/01/31 16:08:33  wheelan
* added unpacking functions, and AlnMgrMakeMultByIntersectOnMaster (does not work yet)
*
* Revision 6.57  2000/01/29 14:03:15  wheelan
* added AlnMgrDeleteHidden and AlnMgrForceContinuous (uses bandalign) plus a couple utilities for these, plus many bug fixes
*
* Revision 6.56  2000/01/19 15:45:09  wheelan
* many, many bug fixes in AlnMgrGetSubAlign and AlnMgrGetNextAlnBit
*
* Revision 6.55  2000/01/14 18:50:36  wheelan
* fixed bug in AlnMgrGetSubAlign
*
* Revision 6.54  2000/01/12 17:43:19  wheelan
* added AlnMgrGetNumSegments, AlnMgrDeleteRow
*
* Revision 6.53  1999/12/02 20:31:59  lewisg
* put seqentries into bioseqset and fix calling convention in alignmgr.c
*
* Revision 6.52  1999/11/30 14:36:39  wheelan
* added AlnMgrMakeMultipleByScore; bug fixes
*
* Revision 6.51  1999/11/26 15:42:19  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 6.50  1999/11/24 11:29:52  wheelan
* added missing return values
*
* Revision 6.49  1999/11/18 19:30:33  wheelan
* added AlnMgrDeleteChildByPointer, bug fixes
*
* Revision 6.48  1999/11/03 12:47:05  wheelan
* added code to correctly handle internal gaps in segmented master-slave alignments
*
* Revision 6.47  1999/11/02 12:38:38  wheelan
* bug fixes when only one child
*
* Revision 6.46  1999/10/25 18:17:23  wheelan
* Added AlnMgrGetUniqueSeqs, fixed merge function to handle single child seqalign correctly
*
* Revision 6.45  1999/10/19 19:27:03  wheelan
* added static defines; changed behavior of AlnMgrGetNextNthSeqRange; rewrote AlnMgrMakeSegmentedMasterSlave to handle more cases
*
* Revision 6.44  1999/10/15 21:51:02  durand
* add AlnMgrIsSAPDiscAli()
*
* Revision 6.43  1999/10/15 18:19:05  wheelan
* added rudimentary ability to default to master-slave type if possible
*
* Revision 6.42  1999/10/15 13:48:47  wheelan
* added AlnMgrGetNthRowTail, extended capability of AlnMgrGetNthStrand
*
* Revision 6.41  1999/10/14 16:10:30  kans
* new includes and prototypes added
*
* Revision 6.40  1999/10/13 19:29:03  wheelan
* added speedup for segmented master-slave creation
*
* Revision 6.39  1999/10/07 13:37:16  wheelan
* added AlnMgrIndexSingleSeqAlign, which only indexes the first seqalign in a list; also added automatic computation of max length of unaligned regions for time savings
*
* Revision 6.38  1999/10/06 19:35:09  wheelan
* added several viewer and editor management functions; fixed many bugs in AlnMgrGetNextAlnBit
*
* Revision 6.37  1999/10/05 15:15:31  wheelan
* added AlnMgrGetNthUnalignedForNthRow
*
* Revision 6.36  1999/10/05 14:02:31  wheelan
* bug fixes in AlnMgrGetNextAlnBit
*
* Revision 6.35  1999/10/04 14:58:08  wheelan
* bug fixes; added AlnMgrMapBioseqToSeqAlign
*
* Revision 6.34  1999/09/24 15:04:55  lewisg
* AlnMgrGetNextAlnBit: amp->to_m changed when calling child
*
* Revision 6.33  1999/09/24 14:29:58  wheelan
* changed behavior of AlnMgrGetNextLengthBit to mimic other GetNext functions, completed functionality of AlnMgrGetSubAlign, bug fixes
*
* Revision 6.32  1999/09/23 16:03:32  wheelan
* Added structures and functions to support segmented master-slave alignments
*
* Revision 6.31  1999/09/22 13:19:15  wheelan
* made AlnMsg row_num field 1-based, added AlnMgrGetNextNthSeqRange, started adding functions to handle a segmented master-slave alignment
*
* Revision 6.30  1999/09/21 19:15:28  wheelan
* changed AlnMgrGetNextAlnBit to return FALSE if called once more past the end; various bug fixes; implemented part of AlnMgrGetSubAlign
*
* Revision 6.29  1999/09/20 12:12:58  wheelan
* added safety checks in case input seqalign has no strand or score information
*
* Revision 6.28  1999/09/20 11:58:52  wheelan
* modified AlnMgrGetNthSeqRange to use new row information structures
*
* Revision 6.27  1999/09/17 16:55:33  wheelan
* bug fixes, added AlnMgrPropagateSeqIdsBySapList to correctly associate seqids with rows
*
* Revision 6.26  1999/09/14 15:48:50  kans
* AlnMgrMapRowCoords returns -1 on failure at end of function
*
* Revision 6.25  1999/09/13 19:57:10  sicotte
* Make AlnMgrMapBsqCoord work for continous alignments
*
* Revision 6.24  1999/09/13 19:43:09  sicotte
* bug fixes
*
* Revision 6.23  1999/09/13 14:33:24  wheelan
* added support for row numbers in AlnMgrGetNextAlnBit
*
* Revision 6.22  1999/09/08 13:36:16  wheelan
* fixed bugs found by Patrick Durand
*
* Revision 6.21  1999/09/08 11:55:35  sicotte
* fix bug that was missing end segments
*
* Revision 6.20  1999/09/08 11:49:13  wheelan
* added capability to return length of unaligned regions
*
* Revision 6.19  1999/09/07 12:11:17  wheelan
* fixed bugs pointed out by Hugues
*
* Revision 6.18  1999/09/06 16:37:44  wheelan
* added AlnMgrGetNextLengthBit and associated function
*
* Revision 6.17  1999/09/06 15:55:55  wheelan
* IndexSeqAlign now makes the fake multiple if possible
*
* Revision 6.16  1999/09/06 15:52:25  wheelan
* added row management functions, made most functions minus-strand compliant, added smarter test for master-slave vs partial
*
* Revision 6.15  1999/09/01 20:11:56  wheelan
* added new merge function and the typedef for the structure it uses
*
* Revision 6.14  1999/09/01 14:40:06  wheelan
* added AlnMgrGetStrand, fixed bugs in GetNextAlnBit, added more cases to AlnMgrIndexSeqAlign
*
* Revision 6.13  1999/08/30 19:28:06  wheelan
* modified AlnMgrGetNextAlnBit to handle master-slave alignments
*
* Revision 6.12  1999/08/26 20:35:21  wheelan
* added parent indexing and pairwise-to-multiple functions
*
* Revision 6.11  1999/08/20 11:23:53  wheelan
* fixed AlnMgrGetNthSeqRange for minus strands
*
* Revision 6.10  1999/08/19 19:30:26  wheelan
* made case for SAT_PARTIAL in AlnMgrGetNextAlnBit
*
* Revision 6.9  1999/08/19 17:24:50  wheelan
* changed AMAlignIndex structure, added more api functions
*
* Revision 6.8  1999/08/12 20:56:56  vakatov
* [WIN32] Added missed LIBCALLBACK
*
* Revision 6.7  1999/08/12 12:41:53  wheelan
* added comments, and functions to index the parent
*
* Revision 6.6  1999/08/06 18:31:19  wheelan
* fixed compiler error
*
* Revision 6.5  1999/08/06 16:38:43  kans
* fixed Mac compiler complaints
*
* Revision 6.4  1999/08/06 13:44:14  wheelan
* added several functions; changed all function names to AlnMgr..
*
* Revision 6.3  1999/07/30 14:17:52  wheelan
* fixes to keep Mac compiler happy
*
* Revision 6.2  1999/07/30 14:08:37  wheelan
* added api functions to access indexes
*
* Revision 6.1  1999/07/29 12:56:25  wheelan
* initial checkin
*

* ==========================================================================
*/



#include <alignmgr.h>
#include <needleman.h>
#include <stdlib.h>

/***************************************************************************
*
*  static functions
*
***************************************************************************/
static void heapsort_with_userdata (VoidPtr b, size_t nel, size_t width, int (LIBCALLBACK *compar)PROTO((VoidPtr, VoidPtr, VoidPtr)), VoidPtr userdata);
static void heapify_with_userdata(CharPtr base0, CharPtr base, CharPtr lim, CharPtr last, size_t width, int(LIBCALLBACK *compar)PROTO((VoidPtr, VoidPtr, VoidPtr)), VoidPtr userdata);
static void AlnMgrSetUnalignedLengths(SeqAlignPtr sap);
static Boolean am_get_nth_range_for_partial(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop, Int4Ptr where, BoolPtr is_aligned, Boolean unaligned);
static AMmsmsPtr am_sort_ammsms(AMmsmsPtr ams_head, Int4 n);
static AMmsmsPtr am_sort_masterams(AMmsmsPtr ams_head, Int4 n);
static Int4 am_get_first_rsp_for_sip(SeqIdPtr sip, AMsiplistPtr siplist);
static int LIBCALLBACK AMCompareAlignInfoProc(VoidPtr ptr1, VoidPtr ptr2);
static int LIBCALLBACK AMCompareStarts(VoidPtr ptr1, VoidPtr ptr2);
static Int4 AlnMgrMapSegmentCoords(SeqAlignPtr sap, Uint4 pos, Int4 row, SeqIdPtr master, Int4Ptr len);
static void am_set_master(SeqAlignPtr sap, SeqIdPtr sip);
static SeqIdPtr am_find_master(SeqAlignPtr sap);
static AMmsmsPtr AlnMgrFindOverlapOnMaster(SeqAlignPtr sap);
static void AlnMgrMergeNeighbors(SeqAlignPtr salp);
static Boolean am_is_new_row(SeqIdPtr sip1, SeqIdPtr sip2);
static Int4Ptr am_save_rowinfo(SeqAlignPtr sap, Int4 numrows);
static void am_set_rows(AMmsmsPtr ams, Int4Ptr rowarray, Int4 numrows);
static AMmsmsPtr am_create_overlap(SeqAlignPtr sap);
static void am_densediag_reverse(DenseDiagPtr ddp);
static AMmsmsPtr AlnMgrTruncateByOverlap(SeqAlignPtr sap, AMmsmsPtr ams_head);
static void am_compare_alignids(AMmsmsPtr ams_prev, AMmsmsPtr ams);
static Boolean AlnMgrCarefulIndex(SeqAlignPtr sap, AMmsmsPtr ams_head, AMmsmsPtr *ams_mhead, Boolean allinblock, Int4 numrows);
static Boolean am_make_null_alignment(SeqAlignPtr sap);
static void am_trim_master(AMmsmsPtr PNTR ams_mhead, AMmsmsPtr ams_head, Int4 numrows);
static Boolean am_check_gaps(SeqAlignPtr sap);
static void am_fix_empty_columns(SeqAlignPtr sap);
static Int4 am_translate_row_num (AMAlignIndexPtr amaip, Int4 n, Int4 row);
static Boolean am_is_consistent(SeqAlignPtr sap, SeqAlignPtr sap_new, Int4Ptr block_num);
static Boolean am_is_ok_block(DenseSegPtr dsp);
static void am_do_merge (AMAlignIndexPtr amaip, Int4 left, Int4 right);
static Boolean am_merge_after_edit (SeqAlignPtr sap);
static Boolean am_same_ids(SeqIdPtr sip1, SeqIdPtr sip2);
static AMmsmsPtr AlnMgrConstructOverlaps(AMmsmsPtr ams_head);
static Boolean AlnMgrJaggedIndex(SeqAlignPtr sap, AMmsmsPtr ams_head, AMmsmsPtr *ams_mhead, Int4 numrows);
static Boolean AlnMgrMakeFakeMultipleEx(SeqAlignPtr sap, Boolean forcestraightms);





/*******************************************************************
*
*  all the memory allocation/deallocation functions
*
*******************************************************************/

NLM_EXTERN SeqAlignIndexPtr SeqAlignIndexNew(void)
{
   return (SeqAlignIndexPtr)(MemNew(sizeof(SeqAlignIndex)));
}

static Boolean LIBCALLBACK SAIndexFreeFunc(VoidPtr index)
{
    return SAIndexFree(index);
}

NLM_EXTERN SAIndexPtr SAIndexNew(void)
{
   SAIndexPtr  saip;

   saip = (SAIndexPtr)MemNew(sizeof(SAIndex));
   saip->master = -1;
   saip->freefunc = (SeqAlignIndexFreeFunc)(SAIndexFreeFunc);
   return saip;
}

NLM_EXTERN Boolean SAIndexFree(VoidPtr index)
{
   Int4        i;
   Boolean     retval;
   SAIndexPtr  saip;

   retval = FALSE;
   if (!index)
      return retval;
   saip = (SAIndexPtr)index;
   if (saip->indextype != INDEX_SEGS)
      return retval;
   MemFree(saip->aligncoords);
   for (i=0; i<saip->numseqs; i++)
   {
      SASeqDatFree(saip->ssdp[i]);
   }
   MemFree(saip->ssdp);
   MemFree(saip);
   retval = TRUE;
   return retval;
}

NLM_EXTERN SASeqDatPtr SASeqDatNew(void)
{
   return (SASeqDatPtr)(MemNew(sizeof(SASeqDat)));
}

NLM_EXTERN void SASeqDatFree(SASeqDatPtr ssdp)
{
   if (ssdp == NULL)
      return;
   if (ssdp->sect != NULL)
      MemFree(ssdp->sect);
   if (ssdp->unsect != NULL)
      MemFree(ssdp->unsect);
   MemFree(ssdp);
}

NLM_EXTERN RowSourcePtr RowSourceNew(void)
{
   return (RowSourcePtr)(MemNew(sizeof(RowSource)));
}

NLM_EXTERN RowSourcePtr RowSourceFree(RowSourcePtr rsp)
{
   if (rsp == NULL)
      return NULL;
   rsp->id = SeqIdSetFree(rsp->id);
   MemFree(rsp->which_saps);
   MemFree(rsp->num_in_sap);
   MemFree(rsp);
   return NULL;
}

static Boolean LIBCALLBACK AMAlignIndexFreeFunc (VoidPtr data)
{
    return AMAlignIndexFree(data);
}


NLM_EXTERN AMAlignIndexPtr AMAlignIndexNew(void)
{
   AMAlignIndexPtr  amaip;

   amaip = (AMAlignIndexPtr)MemNew(sizeof(AMAlignIndex));
   amaip->freefunc = (SeqAlignIndexFreeFunc)(AMAlignIndexFreeFunc);
   amaip->master = -2;
   amaip->indextype = INDEX_PARENT;
   return amaip;
}

NLM_EXTERN Boolean AMAlignIndexFree(VoidPtr index)
{
   AMAlignIndexPtr  amaip;         
   Int4             i;
   Boolean          retval;

   retval = FALSE;
   amaip = (AMAlignIndexPtr)(index);
   if (!amaip)
      return retval;
   if (amaip->indextype != INDEX_PARENT)
      return retval;
   if (amaip->mstype == AM_NEATINDEX)
   {
       MemFree(amaip->saps);
       MemFree(amaip);
       return TRUE;
   }
   amaip->ids = SeqIdSetFree(amaip->ids);
   for (i=0; i<(amaip->numbsqs); i++)
   {
      amaip->amadp[i] = AMAlignDatFree(amaip->amadp[i]);
   }
   if (amaip->saps != NULL)
      MemFree(amaip->saps);
   if (amaip->amadp != NULL)
      MemFree(amaip->amadp);
   if (amaip->aligncoords != NULL)
      MemFree(amaip->aligncoords);
   if (amaip->lens != NULL)
      MemFree(amaip->lens);
   if (amaip->ulens != NULL)
      MemFree(amaip->ulens);
   if (amaip->starts != NULL)
      MemFree(amaip->starts);
   if (amaip->rowsource != NULL)
   {
      for (i=0; i<(amaip->numrows); i++)
      {
         amaip->rowsource[i] = RowSourceFree(amaip->rowsource[i]);
      }
      MemFree(amaip->rowsource);
   }
   MemFree(amaip);
   retval = TRUE;
   return retval;
}

NLM_EXTERN AMAlignDatPtr AMAlignDatNew(void)
{
   return (AMAlignDatPtr)(MemNew(sizeof(AMAlignDat)));
}

NLM_EXTERN AMAlignDatPtr AMAlignDatFree(AMAlignDatPtr amadp)
{
   if (amadp == NULL)
      return NULL;
   SeqIdFree(amadp->sip);
   MemFree(amadp->saps);
   MemFree(amadp->segments);
   MemFree(amadp);
   return NULL;
}

NLM_EXTERN void AMFreeAllIndexes(SeqAlignPtr sap)
{
   SeqAlignPtr  salp;

   if (sap->saip->indextype == INDEX_PARENT)
   {
      salp = (SeqAlignPtr)(sap->segs);
      while (salp != NULL)
      {
         SAIndexFree((Pointer)(salp->saip));
         salp->saip = NULL;
         salp = salp->next;
      }
      AMAlignIndexFree((Pointer)(sap->saip));
      sap->saip = NULL;
   } else
   {
      while (sap != NULL)
      {
         SAIndexFree((Pointer)(sap->saip));
         sap->saip = NULL;
         sap = sap->next;
      }
   }
}

NLM_EXTERN AlnMsgPtr AlnMsgNew(void)
{
   AlnMsgPtr  amp;

   amp = (AlnMsgPtr)MemNew(sizeof(AlnMsg));
   amp->to_m = -1;
   amp->send_space = FALSE;
   amp->row_num = -1;
   amp->prev = -2;
   amp->prev_sap = -2;
   amp->place = 0;
   amp->flag = FALSE;
   amp->which_bsq = NULL;
   return amp;
}

NLM_EXTERN AlnMsgPtr AlnMsgFree(AlnMsgPtr amp)
{
   return ((AlnMsgPtr)MemFree(amp));
}

NLM_EXTERN AlnMsgPtr AlnMsgReNew(AlnMsgPtr amp)
{
   amp->from_m = 0;
   amp->to_m = -1;
   amp->send_space = FALSE;
   amp->row_num = -1;
   amp->prev = -2;
   amp->prev_sap = -2;
   amp->place = 0;
   amp->flag = FALSE;
   amp->which_bsq = NULL;
   return amp;
}

/********************************************************************************
*
*  AlnMgrIndexSingleSeqAlign indexes (in place) only the first seqalign or
*  seqalign set in the chain that is passed in.  It will extensively
*  rearrange the first seqalign given.
*
********************************************************************************/
NLM_EXTERN Boolean AlnMgrIndexSingleSeqAlign(SeqAlignPtr sap)
{
   SeqAlignPtr  sap_next;

   if (sap == NULL)
      return TRUE;
   sap_next = NULL;
   if (sap->next)
      sap_next = sap->next;
   sap->next = NULL;
   AlnMgrIndexSeqAlign(sap);
   sap->next = sap_next;
   if (sap->saip)
      return TRUE;
   else
      return FALSE;
}

NLM_EXTERN Boolean AlnMgrIndexSingleChildSeqAlign(SeqAlignPtr sap)
{
   DenseSegPtr  dsp;
   Int4         i;
   SeqAlignPtr  sap_next;

   if (sap == NULL)
      return FALSE;
   if (sap->segtype == SAS_DISC)
      return FALSE;
   sap_next = NULL;
   if (sap->next)
      sap_next = sap->next;
   sap->next = NULL;
   if (sap->saip != NULL)
   {
      if (sap->saip->indextype == INDEX_SEGS)
         SAIndexFree(sap->saip);
   }
   if (sap->segtype == SAS_DENSEG)
      AlnMgrIndexLinkedSegs(sap);
   else if (sap->segtype == SAS_DENDIAG)
      AlnMgrIndexSingleSeqAlign(sap);
   dsp = (DenseSegPtr)(sap->segs);
   if (dsp->strands == NULL)
   {
      dsp->strands = (Uint1Ptr)MemNew((dsp->dim*dsp->numseg)*sizeof(Uint1));
      for (i=0; i<dsp->dim*dsp->numseg; i++)
      {
         dsp->strands[i] = Seq_strand_plus;
      }
   }
   sap->next = sap_next;
   if (sap->saip)
      return TRUE;
   else
      return FALSE;
}

/********************************************************************************
*
*  AlnMgrReIndexSeqAlign frees the parent indexes, indexes any child
*  seqaligns that are not indexed (it assumes that any indexed child 
*  seqaligns are correctly indexed), and reindexes the set.
*
********************************************************************************/
NLM_EXTERN Boolean AlnMgrReIndexSeqAlign(SeqAlignPtr sap)
{
   SeqAlignPtr      sap_tmp;
   SeqAlignPtr      tmp_next;

   if (sap == NULL)
      return FALSE;
   if (sap->segtype != SAS_DISC) /*  we don't know what we're dealing with */
      return FALSE;
   if (!AMAlignIndexFree((Pointer)sap->saip))
      return FALSE;
   sap->saip = NULL;
   sap_tmp = (SeqAlignPtr)sap->segs;
   while (sap_tmp)
   {
      if (sap_tmp->saip == NULL)
      {
         tmp_next = sap_tmp->next;
         sap_tmp->next = NULL;
         if (!AlnMgrIndexLinkedSegs(sap_tmp))
            return FALSE;
         sap_tmp->next = tmp_next;
      }
      sap_tmp = sap_tmp->next;
   }
   if (!AlnMgrIndexParentSA(sap))
      return FALSE;
   if (!AlnMgrMakeFakeMultiple(sap))
      return FALSE;
   return TRUE;
}

/********************************************************************************
*
*  AlnMgrIndexSeqAlign indexes (in place) the ENTIRE chain of seqaligns
*  and seqalign sets passed in, and extensively rearranges the seqalign.
*
********************************************************************************/
NLM_EXTERN Boolean AlnMgrIndexSeqAlign(SeqAlignPtr sap)
{
   SAIndexPtr   saip;
   SeqAlignPtr  salp;

   if (!sap)
      return FALSE;
   if (sap->saip != NULL)
   {
      return TRUE;
   }
   if (!AlnMgrUnpackSeqAlign(sap))
      return FALSE;
   if (!AlnMgrRearrangeUnpacked(sap))
      return FALSE;
   if (!AlnMgrIndexLinkedSegs((SeqAlignPtr)(sap->segs)))
      return FALSE;
   if (!AlnMgrIndexParentSA(sap))
      return FALSE;
   if (!AlnMgrMakeFakeMultiple(sap))
      return FALSE;
   salp = (SeqAlignPtr)(sap->segs);
   while (salp != NULL)
   {
      saip = (SAIndexPtr)(salp->saip);
      saip->parent = sap;
      salp = salp->next;
   }
   return TRUE;
}

/***************************************************************************
*
*  AlnMgrIndexIndexedChain takes a linked list of indexed seqaligns and
*  indexes them as a set.
*
***************************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgrIndexIndexedChain(SeqAlignPtr sap)
{
   SAIndexPtr   saip;
   SeqAlignPtr  sap_new;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype == INDEX_PARENT)
      return NULL;
   sap_new = SeqAlignNew();
   sap_new->segtype = SAS_DISC;
   sap_new->segs = (Pointer)(sap);
   if (!AlnMgrIndexParentSA(sap_new))
      return NULL;
   if (!AlnMgrMakeFakeMultipleEx(sap_new, TRUE))
      return NULL;
   sap = (SeqAlignPtr)(sap_new->segs);
   while (sap != NULL)
   {
      saip = (SAIndexPtr)(sap->saip);
      saip->parent = sap_new;
      sap = sap->next;
   }
   return sap_new;
}



/**********************************************************************
*
*  AlnMgrIndexLite disassembles the input alignment, indexes all child
*  alignments, and then puts them in the amaip->saps array.  It does
*  not attempt to create alignment coordinates across the whole set.
*  This is useful to keep sets of child alignments together (managing
*  BLAST hits, for example) when creating an overall alignment is
*  unnecessary.  This alignment can be freed normally, but many 
*  alignmgr functions will not work on the parent alignment (they 
*  will work on the child alignments).
*
**********************************************************************/
NLM_EXTERN Boolean AlnMgrIndexLite(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   Int4             i;
   SAIndexPtr       saip;
   SeqAlignPtr      sap_tmp;

   if (!sap)
      return FALSE;
   if (sap->saip != NULL)
   {
      return TRUE;
   }
   if (!AlnMgrUnpackSeqAlign(sap))
      return FALSE;
   if (!AlnMgrRearrangeUnpacked(sap))
      return FALSE;
   if (!AlnMgrIndexLinkedSegs((SeqAlignPtr)(sap->segs)))
      return FALSE;
   amaip = AMAlignIndexNew();
   amaip->mstype = AM_LITE;
   i = 0;
   sap_tmp = (SeqAlignPtr)(sap->segs);
   while (sap_tmp != NULL)
   {
      sap_tmp = sap_tmp->next;
      i++;
   }
   amaip->saps = (SeqAlignPtr PNTR)MemNew(i*sizeof(SeqAlignPtr));
   amaip->numsaps = i;
   amaip->parent = sap;
   sap_tmp = (SeqAlignPtr)(sap->segs);
   for (i=0; i<amaip->numsaps; i++)
   {
      amaip->saps[i] = sap_tmp;
      saip = (SAIndexPtr)(sap_tmp->saip);
      saip->parent = sap;
      sap_tmp = sap_tmp->next;
   }
   sap->saip = (Pointer)amaip;
   return TRUE;
}

NLM_EXTERN SeqAlignPtr AlnMgrGetParent(SeqAlignPtr sap)
{
   SAIndexPtr  saip;

   if (sap->saip->indextype == INDEX_PARENT)
      return sap;
   saip = (SAIndexPtr)sap->saip;
   return saip->parent;
}


/***************************************************************************
*
*  AlnMgrUnpackSeqAlign rearranges any seqalign (except alignments with
*  more than two levels of nested discontinuous alignments) to a simple
*  discontinuous alignment or a linked list of alignments.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgrUnpackSeqAlign(SeqAlignPtr sap)
{
   SeqAlignPtr  sap_new;
   SeqAlignPtr  sap_next;
   SeqAlignPtr  sap_segs;
   SeqAlignPtr  sap_segs_head;
   SeqAlignPtr  sap_segs_prev;

   if (sap == NULL)
      return FALSE;
   if (sap->segtype == SAS_DISC)
   {
      sap_segs_head = (SeqAlignPtr)(sap->segs);
      if (sap_segs_head->segtype == SAS_DISC)
      {
         sap_segs_prev = (SeqAlignPtr)(sap_segs_head->segs);
         sap_segs_head->segs = NULL;
         sap_next = sap_segs_head->next;
         sap_segs_head->next = NULL;
         SeqAlignFree(sap_segs_head);
         sap_segs_head = sap_segs_prev;
         sap->segs = (Pointer)(sap_segs_head);
         while (sap_segs_prev->next)
         {
            sap_segs_prev = sap_segs_prev->next;
            if (sap_segs_prev->segtype == SAS_DISC)
               return FALSE;
         }
         sap_segs_prev->next = sap_next;
         sap_segs = sap_next;
      } else
         sap_segs = sap_segs_head->next;
      while (sap_segs)
      {
         if (sap_segs->segtype == SAS_DISC)
         {
            sap_next = sap_segs->next;
            sap_segs->next = NULL;
            sap_segs_prev->next = (SeqAlignPtr)(sap_segs->segs);
            sap_segs->segs = NULL;
            SeqAlignFree(sap_segs);
            while (sap_segs_prev->next)
            {
               sap_segs_prev = sap_segs_prev->next;
               if (sap_segs_prev->segtype == SAS_DISC)
                  return FALSE;
            }
            sap_segs_prev->next = sap_next;
            sap_segs = sap_next;
         } else
            sap_segs = sap_segs->next;
      }
   } else
   {
      sap_new = SeqAlignNew();
      sap_new->type = SAT_GLOBAL;
      sap_new->segtype = sap->segtype;
      sap_new->dim = sap->dim;
      sap_new->segs = sap->segs;
      sap_new->master = sap->master;
      sap_new->bounds = sap->bounds;
      sap_new->next = sap->next;
      sap_new->score = sap->score;
      sap->next = NULL;
      sap->segtype = SAS_DISC;
      sap->type = 0;
      sap->dim = 0;
      sap->master = NULL;
      sap->bounds = NULL;
      sap->score = NULL;
      sap->segs = (Pointer)sap_new;
      sap_segs_prev = sap_new;
      sap_segs = sap_new->next;
      while (sap_segs)
      {
         if (sap_segs->segtype == SAS_DISC)
         {
            sap_next = sap_segs->next;
            sap_segs->next = NULL;
            sap_segs_prev->next = (SeqAlignPtr)(sap_segs->segs);
            sap_segs->segs = NULL;
            SeqAlignFree(sap_segs);
            while (sap_segs_prev->next)
            {
               sap_segs_prev = sap_segs_prev->next;
               if (sap_segs_prev->segtype == SAS_DISC)
                  return FALSE;
            }
            sap_segs_prev->next = sap_next;
            sap_segs = sap_next;
         } else
            sap_segs = sap_segs->next;
      }
   }
   return TRUE;
}

/***************************************************************************
*
*  AlnMgrRearrangeUnpacked transforms all child seqaligns into dense-seg
*  types, requiring some rearrangement for dense-diag sets.  This function
*  presumes that AlnMgrUnpackSeqAlign has already been called on the
*  alignment.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgrRearrangeUnpacked(SeqAlignPtr sap)
{
   DenseDiagPtr  ddp;
   DenseDiagPtr  ddp_prev;
   DenseSegPtr   dsp;
   Int4          i;
   SeqAlignPtr   salp;
   SeqAlignPtr   salp_tmp;
   SeqAlignPtr   sap_head;
   SeqAlignPtr   sap_new;
   SeqAlignPtr   sap_prev;
   StdSegPtr     ssp;
   StdSegPtr     ssp_next;

   if (sap == NULL || sap->segtype != SAS_DISC)
      return FALSE;
   salp = (SeqAlignPtr)(sap->segs);
   sap_head = sap_prev = NULL;
   while (salp)
   {
      if (salp->segtype < 1)
      {
         return FALSE;
      } else if (salp->segtype == SAS_DENDIAG)
      {
         ddp = (DenseDiagPtr)salp->segs;
         while (ddp)
         {
            sap_new = SeqAlignNew();
            sap_new->type = SAT_GLOBAL;
            sap_new->segtype = SAS_DENSEG;
            sap_new->dim = ddp->dim;
            dsp = DenseSegNew();
            dsp->dim = sap_new->dim;
            dsp->numseg = 1;
            dsp->starts = ddp->starts;
            ddp->starts = NULL;
            dsp->lens = (Int4Ptr)MemNew(sizeof(Int4));
            dsp->lens[0] = ddp->len;
            ddp->len = 0;
            dsp->scores = ddp->scores;
            ddp->scores = NULL;
            dsp->strands = ddp->strands;
            ddp->strands = NULL;
            if (dsp->strands == NULL)
            {
               dsp->strands = (Uint1Ptr)MemNew(dsp->dim * sizeof(Uint1));
               for (i=0; i<dsp->dim; i++)
               {
                  dsp->strands[i] = Seq_strand_plus;
               }
            }
            dsp->ids = SeqIdDupList(ddp->id);
            sap_new->segs = (Pointer)dsp;
            if (dsp->scores)
               sap_new->score = ScoreDup(dsp->scores);
            if (!sap_head)
            {
               sap_head = sap_prev = sap_new;
            } else
            {
               sap_prev->next = sap_new;
               sap_prev = sap_new;
            }
            ddp_prev = ddp;
            ddp = ddp->next;
            DenseDiagFree(ddp_prev);
         }
         salp_tmp = salp->next;
         sap_prev->next = salp_tmp;
         salp->next = NULL;
         salp->segs = NULL;
         SeqAlignFree(salp);
         salp = salp_tmp;
      } else if (salp->segtype == SAS_DENSEG)
      {
         if (!sap_head)
            sap_head = sap_prev = salp;
         else
         {
            sap_prev->next = salp;
            sap_prev = salp;
         }
         dsp = (DenseSegPtr)salp->segs;
         if (dsp->strands == NULL)
         {
            dsp->strands = (Uint1Ptr)MemNew((dsp->dim)*(dsp->numseg)* sizeof(Uint1));
            for (i=0; i<(dsp->dim)*(dsp->numseg); i++)
            {
               dsp->strands[i] = Seq_strand_plus;
            }
         }
         salp = salp->next;
      } else if (salp->segtype == SAS_STD)
      {
         sap_prev = sap_head = NULL;
         ssp = (StdSegPtr)salp->segs;
         while (ssp)
         {
            sap_new = SeqAlignNew();
            if (sap_head)
            {
               sap_prev->next = sap_new;
               sap_prev = sap_new;
            } else
            {
               sap_head = sap_prev = sap_new;
            }
            sap_new->segtype = SAS_STD;
            sap_new->type = SAT_GLOBAL;
            sap_new->segs = (Pointer)ssp;
            ssp_next = ssp->next;
            ssp->next = NULL;
            ssp = ssp_next;
         }
         salp_tmp = salp->next;
         salp->next = NULL;
         salp->segs = NULL;
         SeqAlignFree(salp);
         salp = (Pointer)sap_head;
         sap_prev->next = salp_tmp;
         salp = salp_tmp;
      }
   }
   sap->segs = (Pointer)sap_head;
   return TRUE;
}


/***************************************************************************
*
*  AlnMgrAnythingToSeg takes any SeqAlign and does an in-place transformation
*  to the parent-child structure.  Each dense-seg, dense-diag and std-seg
*  is put into its own seqalign, and the child seqaligns are linked
*  together in no particular order and put in the sap->segs field of the
*  new parent (which takes over the pointer passed in).  The parent
*  has segtype SAS_DISC, and each child has segtype SAS_DENSEG or SAS_STD.
*  Each child, then, is a continuous, nonoverlapping alignment and therefore
*  may be indexed.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgrAnythingToSeg (SeqAlignPtr sap)
{
   DenseDiagPtr  ddp;
   DenseDiagPtr  ddp_prev;
   DenseSegPtr   dsp;
   Int4          i;
   Boolean       retval;
   SeqAlignPtr   salp;
   SeqAlignPtr   salp_tmp;
   SeqAlignPtr   sap_head;
   SeqAlignPtr   sap_new;
   SeqAlignPtr   sap_prev;
   StdSegPtr     ssp;
   StdSegPtr     ssp_next;

   retval = FALSE;
   if (!sap)
      return retval;
   sap_new = SeqAlignNew();
   sap_new->type = SAT_GLOBAL;
   sap_new->segtype = sap->segtype;
   sap_new->dim = sap->dim;
   sap_new->segs = sap->segs;
   sap_new->master = sap->master;
   sap_new->bounds = sap->bounds;
   sap_new->next = sap->next;
   sap_new->score = sap->score;
   sap->next = NULL;
   sap->segtype = SAS_DISC;
   sap->type = 0;
   sap->dim = 0;
   sap->master = NULL;
   sap->bounds = NULL;
   sap->score = NULL;
   salp = sap_new;
   sap_head = sap_prev = NULL;
   while (salp)
   {
      if (salp->segtype < 1)
      {
         return retval;
      } else if (salp->segtype == SAS_DENDIAG)
      {
         ddp = (DenseDiagPtr)salp->segs;
         while (ddp)
         {
            sap_new = SeqAlignNew();
            sap_new->type = SAT_GLOBAL;
            sap_new->segtype = SAS_DENSEG;
            sap_new->dim = ddp->dim;
            dsp = DenseSegNew();
            dsp->dim = sap_new->dim;
            dsp->numseg = 1;
            dsp->starts = ddp->starts;
            ddp->starts = NULL;
            dsp->lens = (Int4Ptr)MemNew(sizeof(Int4));
            dsp->lens[0] = ddp->len;
            ddp->len = 0;
            dsp->scores = ddp->scores;
            ddp->scores = NULL;
            dsp->strands = ddp->strands;
            ddp->strands = NULL;
            if (dsp->strands == NULL)
            {
               dsp->strands = (Uint1Ptr)MemNew(dsp->dim * sizeof(Uint1));
               for (i=0; i<dsp->dim; i++)
               {
                  dsp->strands[i] = Seq_strand_plus;
               }
            }
            dsp->ids = SeqIdDupList(ddp->id);
            sap_new->segs = (Pointer)dsp;
            if (dsp->scores)
               sap_new->score = ScoreDup(dsp->scores);
            if (!sap_head)
            {
               sap_head = sap_prev = sap_new;
            } else
            {
               sap_prev->next = sap_new;
               sap_prev = sap_new;
            }
            ddp_prev = ddp;
            ddp = ddp->next;
            DenseDiagFree(ddp_prev);
         }
         salp_tmp = salp->next;
         sap_prev->next = salp_tmp;
         salp = salp_tmp;
         retval = TRUE;
      } else if (salp->segtype == SAS_DENSEG)
      {
         if (!sap_head)
            sap_head = sap_prev = salp;
         else
         {
            sap_prev->next = salp;
            sap_prev = salp;
         }
         dsp = (DenseSegPtr)salp->segs;
         if (dsp->strands == NULL)
         {
            dsp->strands = (Uint1Ptr)MemNew((dsp->dim)*(dsp->numseg)* sizeof(Uint1));
            for (i=0; i<(dsp->dim)*(dsp->numseg); i++)
            {
               dsp->strands[i] = Seq_strand_plus;
            }
         }
         salp = salp->next;
         retval = TRUE;
      } else if (salp->segtype == SAS_STD)
      {
         sap_prev = sap_head = NULL;
         ssp = (StdSegPtr)salp->segs;
         while (ssp)
         {
            sap_new = SeqAlignNew();
            if (sap_head)
            {
               sap_prev->next = sap_new;
               sap_prev = sap_new;
            } else
            {
               sap_head = sap_prev = sap_new;
            }
            sap_new->segtype = SAS_STD;
            sap_new->type = SAT_GLOBAL;
            sap_new->segs = (Pointer)ssp;
            ssp_next = ssp->next;
            ssp->next = NULL;
            ssp = ssp_next;
         }
         salp_tmp = salp->next;
         salp = (Pointer)sap_head;
         sap_prev->next = salp_tmp;
         salp = salp_tmp;
      }
   }
   sap->segs = (Pointer)sap_head;
   return retval;
}


/***********************************************************************
*
*  AlnMgrIndexLinkedSegs and AlnMgrIndexParentSA create and fill in the
*  SASeqIndex and AMAlignIndex structures on the children and the parent,
*  respectively.  IndexLinkedSegs is called on the sap->segs field of
*  the parent, so that the pointer of the first child in the list
*  gets passed in.  AlnMgrIndexParentSA is called on the parent, and
*  the children must already be indexed (the function does check) in order
*  for it to work.  AlnMgrIndexParentSA calls AlnMgrPropagateUpSeqIdPtrs
*  to create a list of all SeqIdPtrs present in all the children (each
*  is only listed once, in the order that its AMAlignDat structure occurs
*  in).
*
***********************************************************************/
NLM_EXTERN Boolean AlnMgrIndexLinkedSegs (SeqAlignPtr sap)
{ /* all the Uint2's may have to be changed to Uint4's */
   Int4         currseq;
   DenseSegPtr  dsp;
   Uint2        i;
   Uint4        qlen;
   Boolean      retval;
   SAIndexPtr   saip;
   SASeqDatPtr  ssdp;

   retval = FALSE;
   while (sap)
   {
      if (sap->segtype == SAS_DENSEG)
      {
         dsp = (DenseSegPtr)sap->segs;
         saip = SAIndexNew();
         saip->aligncoords = (Uint4Ptr)MemNew((dsp->numseg+1)*sizeof(Uint4));
         qlen = 0;
         saip->ssdp = (SASeqDatPtr PNTR)MemNew((dsp->dim+1)*sizeof(SASeqDatPtr));
         saip->numseqs = dsp->dim+1;
         for (i = 0; i<(dsp->dim); i++)
         {
            ssdp = SASeqDatNew();
            saip->ssdp[i] = ssdp;
         }
         for (i = 0; i<(dsp->numseg); i++)
         {
            saip->aligncoords[i] = qlen;
            qlen += dsp->lens[i];
            for (currseq = 0; currseq<(dsp->dim); currseq++)
            {
               if ((dsp->starts[dsp->dim*i+currseq]) != -1)
               {
                  saip->ssdp[currseq]->numsect++;
               }
            }
         }
         for (currseq = 0; currseq<(dsp->dim); currseq++)
         {
            saip->ssdp[currseq]->sect = (Uint2Ptr)MemNew((saip->ssdp[currseq]->numsect)*sizeof(Uint2));
            saip->ssdp[currseq]->unsect = (Uint2Ptr)MemNew((dsp->numseg - saip->ssdp[currseq]->numsect)*sizeof(Uint2));
            saip->ssdp[currseq]->numsect = 0;
         }
         for (i=0; i<(dsp->numseg); i++)
         {
            for (currseq=0; currseq<(dsp->dim); currseq++)
            {
               if ((dsp->starts[dsp->dim*i+currseq]) != -1)
               {
                  saip->ssdp[currseq]->sect[saip->ssdp[currseq]->numsect] = i;
                  saip->ssdp[currseq]->numsect++;
               } else
               {
                  saip->ssdp[currseq]->unsect[saip->ssdp[currseq]->numunsect]=i;
                  saip->ssdp[currseq]->numunsect++;
               }
            }
         }
         saip->indextype = INDEX_SEGS;
         sap->saip = (SeqAlignIndexPtr)saip;
      }
      sap = sap->next;
      retval = TRUE;
   }
   return retval;
}

NLM_EXTERN Boolean AlnMgrIndexParentSA(SeqAlignPtr sap)
{
   AMAlignDatPtr    amadp;
   AMAlignIndexPtr  amaip;
   Int4             count;
   Boolean          done;
   Int4             i;
   Int4             notfound;
   Int4             numsap;
   Boolean          retval;
   SeqAlignPtr      salp;
   SeqIdPtr         sip;

   retval = FALSE;
   if (!sap)
      return retval;
   if (sap->segtype != SAS_DISC)
      return retval;
   if (((SeqAlignPtr)(sap->segs))->saip == NULL)
   {
      if (!AlnMgrIndexLinkedSegs((SeqAlignPtr)(sap->segs)))
         return retval;
   }
   amaip = (AMAlignIndexPtr)sap->saip;
   if (amaip)
      sap->saip = (Pointer)AMAlignIndexFree(amaip);
   sap->saip = NULL;
   amaip = AMAlignIndexNew();
   count = 0;
   amaip->indextype = INDEX_PARENT;
   amaip->ids = AlnMgrPropagateUpSeqIdPtrs(sap, &count);
   sip = amaip->ids;
   amaip->numbsqs = count;
   amaip->amadp = (AMAlignDatPtr PNTR)MemNew((count+1)*sizeof(AMAlignDatPtr));
   for (count = 0; count < amaip->numbsqs; count++)
   {
      amadp = AMAlignDatNew();
      amaip->amadp[count] = amadp;
      numsap = 0;
      /*amadp->saps = AlnMgrSortSeqAligns((SeqAlignPtr)sap->segs, AlnMgrCompareIncreasingBySeqIdPtr, sip, &numsap);*/
      amadp->saps = AlnMgrSortbyID((SeqAlignPtr)sap->segs, sip, &numsap);
      done = FALSE;
      notfound = 0;
      for (i=0; i<numsap && !done; i++)
      {
         if (AlnMgrGetNForSip(amadp->saps[i], sip) < 0)
         {
            notfound++;
         } else
         {
            done = TRUE;
         }
      }
      amadp->numsaps = numsap - notfound;
      for (i=0; i<(numsap - notfound); i++)
      {
         amadp->saps[i] = amadp->saps[i+notfound];
      }
      for (i=(numsap - notfound); i<numsap; i++)
      {
         amadp->saps[i] = NULL;
      }
      amadp->sip = SeqIdDup(sip);
      sip = sip->next;
   }
   i = 0;
   salp = (SeqAlignPtr)sap->segs;
   while (salp)
   {
      i++;
      salp = salp->next;
   }
   amaip->numsaps = i;
   amaip->parent = sap;
   sap->saip = (Pointer)amaip;
   retval = TRUE;
   return retval;
}

NLM_EXTERN SeqIdPtr AlnMgrPropagateUpSeqIdPtrs(SeqAlignPtr sap, Int4Ptr num)
{
   Int4             count;
   DenseSegPtr      dsp;
   Boolean          found;
   SeqAlignPtr      salp;
   SeqIdPtr         sip_head;
   SeqIdPtr         sip_list;
   SeqIdPtr         sip_tmp;
   SeqIdPtr         sip_tmp2;

   if (!sap)
      return NULL;
   if (sap->segtype == SAS_DISC)
      salp = (SeqAlignPtr)(sap->segs);
   else
      salp = sap;
   count = 0;
   sip_list = sip_head = NULL;
   while (salp)
   {
      dsp = (DenseSegPtr)salp->segs;
      sip_tmp = dsp->ids;
      if (!sip_list)
      {
         sip_head = sip_list = SeqIdDup(sip_tmp);
         sip_tmp = sip_tmp->next;
         count++;
      }
      while (sip_tmp)
      {
         sip_tmp2 = sip_head;
         found = FALSE;
         while (sip_tmp2 && !found)
         {
            if (SeqIdComp(sip_tmp, sip_tmp2) == SIC_YES)
               found = TRUE;
            sip_tmp2 = sip_tmp2->next;
         }
         if (!found)
         {
            sip_list->next = SeqIdDup(sip_tmp);
            sip_list = sip_list->next;
            sip_list->next = NULL;
            count++;
         }
         sip_tmp = sip_tmp->next;
      }
      salp = salp->next;
   }
   if (num)
      *num = count;
   return sip_head;
}

NLM_EXTERN SeqIdPtr AlnMgrPropagateSeqIdsBySapList(AMAlignIndexPtr amaip)
{
   DenseSegPtr  dsp;
   Int4         i;
   Int4         j;
   SAIndexPtr   saip;
   SeqAlignPtr  salp;
   SeqIdPtr     sip;
   SeqIdPtr     sip_head;
   SeqIdPtr     sip_tmp;
   SeqIdPtr     sip_tmp2;

   if (amaip == NULL)
      return NULL;
   if (amaip->saps == NULL)
      return NULL;
   sip_head = NULL;
   for (i=0; i<(amaip->alnsaps); i++)
   {
      j=1;
      salp = amaip->saps[i];
      saip = (SAIndexPtr)salp->saip;
      dsp = (DenseSegPtr)(salp->segs);
      sip_tmp = dsp->ids;
      while (j<saip->master)
      {
         sip_tmp = sip_tmp->next;
         j++;
      }
      if (sip_head == NULL)
         sip_head = sip = SeqIdDup(sip_tmp);
      sip_tmp = dsp->ids;
      j=0;
      while (sip_tmp)
      {
         j++;
         if (j!=saip->master)
         {
            sip_tmp2 = SeqIdDup(sip_tmp);
            sip->next = sip_tmp2;
            sip = sip->next;
         }
         sip_tmp = sip_tmp->next; 
      }
   }
   return sip_head;
}

NLM_EXTERN SeqIdPtr  AlnMgrPropagateSeqIdsByRow(AMAlignIndexPtr amaip)
{
   Int4      i;
   SeqIdPtr  sip;
   SeqIdPtr  sip_head;
   SeqIdPtr  sip_tmp;

   if (amaip->rowsource == NULL)
      return NULL;
   sip_head = sip = SeqIdDup(amaip->rowsource[0]->id);
   for (i=1; i<amaip->numrows; i++)
   {
      sip_tmp = SeqIdDup(amaip->rowsource[i]->id);
      sip->next = sip_tmp;
      sip = sip->next;
   }
   return sip_head;
}

/***************************************************************************
*
*  AlnMgrRemoveInconsistentFromPairwiseSet is a greedy function to make
*  a consistent (nonoverlapping, linear) subset of alignments from a
*  set of pairwise alignments (often BLAST output, gapped or ungapped).
*  The input seqalign should either not be indexed or indexed using
*  AlnMgrIndexLite (just call it on the BLAST output).  fuzz specifies
*  how much overlap, if any, is allowed between alignments that are kept (for
*  example, if fuzz = 5, any alignments that overlap by 5 or less are
*  considered consistent).  If fuzz is less than 0, this will force spaces
*  between alignments (not sure why someone would want to do that, but 
*  it is allowed).
*
*  The "Ex" version also returns the Discarded SeqAligns separated into two
*     lists according to the reason for their rejection.
*  
*
***************************************************************************/
NLM_EXTERN void AlnMgrRemoveInconsistentFromPairwiseSet(SeqAlignPtr sap, Int4 fuzz) {
    AlnMgrRemoveInconsistentFromPairwiseSetEx(sap, fuzz,NULL,NULL,NULL);
}


NLM_EXTERN void AlnMgrRemoveInconsistentFromPairwiseSetEx(SeqAlignPtr sap, Int4 fuzz, SeqAlignPtr PNTR wrong_strand, SeqAlignPtr PNTR overlaps_m,SeqAlignPtr PNTR overlaps_s)
{
   AMAlignInfoPtr   aip_list;
   AMAlignIndexPtr  amaip;
   FloatHi          bit_score;
   Boolean          conflict;
   FloatHi          evalue;
   Int4             i;
   Int4             j;
   Int4             number;
   SAIndexPtr       saip1;
   SAIndexPtr       saip2;
   SeqAlignPtr      salp;
   Int4             score;
   Int4             start1;
   Int4             start2;
   Int4             startm1;
   Int4             startm2;
   Int4             stop1;
   Int4             stop2;
   Int4             stopm1;
   Int4             stopm2;
   Uint1            strand;
   Uint1            strand_curr;
   Uint1            conflict_type;
   if(wrong_strand)
       *wrong_strand = NULL;
   if(overlaps_m)
       *overlaps_m=NULL;
   if(overlaps_s)
       *overlaps_s=NULL;
   if (sap == NULL || (sap->saip != NULL && sap->saip->indextype != INDEX_PARENT))
      return;
   if (sap->saip == NULL)
   {
      if (!AlnMgrIndexLite(sap))
         return;
   }
   amaip = (AMAlignIndexPtr)(sap->saip);
   if (amaip->numbsqs > 2)
      return;

   salp = (SeqAlignPtr)(sap->segs);
   aip_list = (AMAlignInfoPtr)MemNew((amaip->numsaps)*sizeof(AMAlignInfo));
   for (i=0; i<amaip->numsaps && salp != NULL; i++)
   {
      aip_list[i].align = salp;
      GetScoreAndEvalue(salp, &score, &bit_score, &evalue, &number);
      aip_list[i].align_len = score;
      salp = salp->next;
   }
   HeapSort(aip_list, amaip->numsaps, sizeof(AMAlignInfo), AMCompareAlignInfoProc);
   saip1 = (SAIndexPtr)aip_list[0].align->saip;
   if (saip1 == NULL)
      return;
   strand = AlnMgrGetNthStrand(aip_list[0].align, 2);
   if (strand != Seq_strand_minus)
      strand = Seq_strand_plus;
   amaip->alnsaps = 0;
   for (i=0; i<amaip->numsaps; i++)
   {
      if ((saip1 = (SAIndexPtr)aip_list[i].align->saip) == NULL)
         return;
      AlnMgrGetNthSeqRangeInSA(aip_list[i].align, 1, &startm1, &stopm1);
      AlnMgrGetNthSeqRangeInSA(aip_list[i].align, 2, &start1, &stop1);
      strand_curr = AlnMgrGetNthStrand(aip_list[i].align, 2);
      if (strand_curr != Seq_strand_minus)
         strand_curr = Seq_strand_plus;
      if (strand_curr != strand) {
         conflict = TRUE;
         conflict_type = 1; /* wrong strand */
      } else
         conflict = FALSE;
      for (j=0; j<amaip->alnsaps && !conflict; j++)
      {
         if ((saip2 = (SAIndexPtr)(amaip->saps[j]->saip)) == NULL)
            return;
         AlnMgrGetNthSeqRangeInSA(amaip->saps[j], 1, &startm2, &stopm2);
         AlnMgrGetNthSeqRangeInSA(amaip->saps[j], 2, &start2, &stop2);
         if (startm1 < startm2)
         {
             if (stopm1 >= startm2 + fuzz) {
               conflict = TRUE;
               conflict_type = 2 ; /* overlap on master */
             } else if (strand == Seq_strand_minus)
             {
                if (start1 <= stop2 - fuzz) {
                  conflict = TRUE;
                  conflict_type = 3 ; /*overlap on query */
                }
             } else
             {
                if (stop1 >= start2 + fuzz) {
                  conflict = TRUE;
                  conflict_type = 3 ; /*overlap on query */
                }
             }
         } else if (startm1 > startm2)
         {
             if (startm1 <= stopm2 - fuzz) {
               conflict = TRUE;
               conflict_type = 2 ; /* overlap on master */
             } else if (strand == Seq_strand_minus)
             {
                if (stop1 >= start2 + fuzz) {
                  conflict = TRUE;
                  conflict_type = 3 ; /*overlap on query */
                }
             } else
             {
                 if (stop2 >= start1 + fuzz) {
                     conflict = TRUE;
                     conflict_type = 3 ; /*overlap on query */
                 }
             }
         } else if (startm1 == startm2) {
            conflict = TRUE;
            conflict_type = 2 ; /*overlap on master */
         }
      }
      if (!conflict)
      {
         amaip->saps[amaip->alnsaps] = aip_list[i].align;
         amaip->alnsaps++;
      } else {
          switch(conflict_type) {
          case 1:
              if(wrong_strand) {
                  if(*wrong_strand) {
                      aip_list[i].align->next = *wrong_strand;
                  }
                  *wrong_strand = aip_list[i].align;
              } else {
                  SeqAlignFree(aip_list[i].align);
              }
              break;
          case 2:
              if(overlaps_m) {
                  if(*overlaps_m) {
                      aip_list[i].align->next = *overlaps_m;
                  }
                  *overlaps_m = aip_list[i].align;
              } else {
                  SeqAlignFree(aip_list[i].align);
              }
              break;
          case 3:
              if(overlaps_s) {
                  if(*overlaps_s) {
                      aip_list[i].align->next = *overlaps_s;
                  }
                  *overlaps_s = aip_list[i].align;
              } else {
                  SeqAlignFree(aip_list[i].align);
              }
              break;
          default:
              SeqAlignFree(aip_list[i].align);
          }
      }
   }
   MemFree(aip_list);
   AlnMgrDeleteHiddenEx(sap, FALSE,FALSE);
   {
       /* Revert the SeqAlign order, to reorder them by score */
       SeqAlignPtr salp_next,salp_last=NULL;
       if(wrong_strand && *wrong_strand) {
           salp_next = (*wrong_strand)->next;
           while(salp_next!=NULL) {
               (*wrong_strand)->next = salp_last;
               salp_last = *wrong_strand;
               *wrong_strand = salp_next;
               salp_next = (*wrong_strand)->next;
           }
       }
       if(overlaps_m && *overlaps_m) {
           salp_next = (*overlaps_m)->next;
           while(salp_next!=NULL) {
               (*overlaps_m)->next = salp_last;
               salp_last = *overlaps_m;
               *overlaps_m = salp_next;
               salp_next = (*overlaps_m)->next;
           }
       }
       if(overlaps_s && *overlaps_s) {
           salp_next = (*overlaps_s)->next;
           while(salp_next!=NULL) {
               (*overlaps_s)->next = salp_last;
               salp_last = *overlaps_s;
               *overlaps_s = salp_next;
               salp_next = (*overlaps_s)->next;
           }
       }

   }
}


NLM_EXTERN Boolean AlnMgrMakeMultipleByScore(SeqAlignPtr sap)
{
   return (AlnMgrMakeMultipleByScoreExEx(sap, 0,NULL,NULL,NULL));
}

NLM_EXTERN Boolean AlnMgrMakeMultipleByScoreEx(SeqAlignPtr sap, Int4 fuzz) {
   return (AlnMgrMakeMultipleByScoreExEx(sap, fuzz,NULL,NULL,NULL));
}


NLM_EXTERN Boolean AlnMgrMakeMultipleByScoreExEx(SeqAlignPtr sap, Int4 fuzz,SeqAlignPtr PNTR wrong_strand, SeqAlignPtr PNTR overlaps_m,SeqAlignPtr PNTR overlaps_s)
{
   AMAlignIndexPtr  amaip;
   FloatHi          bit_score;
   Boolean          conflict;
   FloatHi          evalue;
   Int4             i;
   Int4             j;
   Int4             n;
   Int4             number;
   SAIndexPtr       saip1;
   SAIndexPtr       saip2;
   SeqAlignPtr      salp;
   AMAlignInfoPtr   salp_list;
   SeqAlignPtr      PNTR saparray;
   Int4             score;
   SeqIdPtr         sip;
   Int4             start1;
   Int4             start2;
   Int4             startm1;
   Int4             startm2;
   Int4             stop1;
   Int4             stop2;
   Int4             stopm1;
   Int4             stopm2;
   Int4             fuzzstop;
   Uint1            strand;
   Uint1            strand_curr;
   AMTinyInfoPtr    PNTR tiparray;
   Uint1            conflict_type;
   
   if(wrong_strand)
       *wrong_strand = NULL;
   if(overlaps_m)
       *overlaps_m=NULL;
   if(overlaps_s)
       *overlaps_s=NULL;

   if (sap == NULL)
      return FALSE;
   i = AlnMgrCheckAlignForParent(sap);
   if (i != AM_PARENT)
      return FALSE;
   amaip = (AMAlignIndexPtr)sap->saip;
   if (amaip == NULL)
      return FALSE;
   if (amaip->numbsqs > 2)
      return FALSE;
   if (sap->master == NULL)
         return FALSE;
   salp = (SeqAlignPtr)sap->segs;
   n = amaip->numsaps;
   if(n==1)
       return TRUE;

   salp_list = Calloc(n, sizeof (AMAlignInfo));
   for (i=0; i<n; i++, salp=salp->next)
   {
      salp_list[i].align=salp;
      GetScoreAndEvalue(salp, &score, &bit_score, &evalue, &number);
      salp_list[i].align_len = score;
   }
   HeapSort (salp_list, n, sizeof (AMAlignInfo), AMCompareAlignInfoProc);
   saip1 = (SAIndexPtr)salp_list[0].align->saip;
   if (saip1 == NULL)
      return FALSE;
   strand = AlnMgrGetNthStrand(salp_list[0].align, 3-saip1->master);
   if (strand != Seq_strand_minus)
      strand = Seq_strand_plus;
   amaip->alnsaps = 0;
   for (i=0; i<n; i++)
   {
      if ((saip1 = (SAIndexPtr)salp_list[i].align->saip) == NULL)
         return FALSE;
      AlnMgrGetNthSeqRangeInSA(salp_list[i].align, saip1->master, &startm1, &stopm1);
      AlnMgrGetNthSeqRangeInSA(salp_list[i].align, 3-saip1->master, &start1, &stop1);
      strand_curr = AlnMgrGetNthStrand(salp_list[i].align, 3-saip1->master);
      if (strand_curr != Seq_strand_minus)
         strand_curr = Seq_strand_plus;
      if (strand_curr != strand) {
         conflict = TRUE;
         conflict_type = 1;
      } else
         conflict = FALSE;
      for (j=0; j<amaip->alnsaps && !conflict; j++)
      {
         if ((saip2 = (SAIndexPtr)(amaip->saps[j]->saip)) == NULL)
            return FALSE;
         AlnMgrGetNthSeqRangeInSA(amaip->saps[j], saip2->master, &startm2, &stopm2);
         AlnMgrGetNthSeqRangeInSA(amaip->saps[j], 3-saip2->master, &start2, &stop2);
         if (startm1 < startm2)
         {
             fuzzstop = stopm1-fuzz;
             if (fuzzstop<startm1)
                 fuzzstop = startm1;
             if (fuzzstop >= startm2) {
               conflict = TRUE;
               conflict_type = 2; /* overlap on master */
             } else if (strand == Seq_strand_minus)
             {
                 fuzzstop = stop2-fuzz;
                 if (fuzzstop<start2)
                     fuzzstop = start2;
                 if (start1 <= fuzzstop) {
                     conflict = TRUE;
                     conflict_type = 3; /* overlap on subject */
                 }
            } else
            {
                 fuzzstop = stop1-fuzz;
                 if (fuzzstop<start1)
                     fuzzstop = start1;
                if (fuzzstop >= start2) {
                    conflict = TRUE;
                    conflict_type = 3; /* overlap on subject */
                }
            }
         } else if (startm1 > startm2)
         {
             fuzzstop = stopm2-fuzz;
             if (fuzzstop<startm2)
                 fuzzstop = startm2;
             if (startm1 <= fuzzstop) {
                 conflict = TRUE;
                 conflict_type = 2; /* overlap on master */
             } else if (strand == Seq_strand_minus)
             {
                 fuzzstop = stop1-fuzz;
                 if (fuzzstop<start1)
                     fuzzstop = start1;
                 if (fuzzstop >= start2) {
                     conflict = TRUE;
                     conflict_type = 3; /* overlap on subject */
                 }
            } else
            {
                fuzzstop = stop2-fuzz;
                if (fuzzstop<start2)
                    fuzzstop = start2;
                if (fuzzstop >= start1) {
                    conflict = TRUE;
                    conflict_type = 3; /* overlap on subject */
                }
            }
         } else if (startm1 == startm2) {
            conflict = TRUE;
            conflict_type = 2; /* overlap on master */
         }
      }
      if (!conflict)
      {
         amaip->saps[amaip->alnsaps] = salp_list[i].align;
         amaip->alnsaps++;
      } else {
          switch(conflict_type) {
          case 1:
              if(wrong_strand) {
                  if(*wrong_strand) {
                      (salp_list[i].align)->next = *wrong_strand;
                  }
                  *wrong_strand = salp_list[i].align;
              } else {
                  SeqAlignFree(salp_list[i].align);
              }
              break;
          case 2:
              if(overlaps_m) {
                  if(*overlaps_m) {
                      (salp_list[i].align)->next = *overlaps_m;
                  }
                  *overlaps_m = salp_list[i].align;
              } else {
                  SeqAlignFree(salp_list[i].align);
              }
              break;
          case 3:
              if(overlaps_s) {
                  if(*overlaps_s) {
                      (salp_list[i].align)->next = *overlaps_s;
                  }
                  *overlaps_s = salp_list[i].align;
              } else {
                  SeqAlignFree(salp_list[i].align);
              }
              break;
          default:
              SeqAlignFree(salp_list[i].align);
          }
      }
   }
   tiparray = (AMTinyInfoPtr PNTR)MemNew((amaip->alnsaps)*sizeof(AMTinyInfoPtr));
   for (i=0; i<amaip->alnsaps; i++)
       {
           saip1 = (SAIndexPtr)amaip->saps[i]->saip;
           AlnMgrGetNthSeqRangeInSA(amaip->saps[i], saip1->master, &start1, &stop1);
           tiparray[i] = (AMTinyInfoPtr)MemNew(sizeof(AMTinyInfo));
           tiparray[i]->start = start1;
           tiparray[i]->stop = stop1;
           tiparray[i]->numgap = saip1->master;
           tiparray[i]->numsap = i;
       }
   HeapSort((Pointer)tiparray, (size_t)(amaip->alnsaps), sizeof(AMTinyInfoPtr), AlnMgrCompareTips);
   saparray = (SeqAlignPtr PNTR)(MemNew((amaip->alnsaps)*sizeof(SeqAlignPtr)));
   for (i=0; i<amaip->alnsaps; i++)
       {
           saparray[i] = amaip->saps[i];
       }
   for (i=0; i<amaip->alnsaps; i++)
       {
           amaip->saps[i] = saparray[tiparray[i]->numsap];
           tiparray[i]->numsap = i;
       }
   MemFree(saparray);
   amaip->numseg = amaip->alnsaps;
   amaip->aligncoords = (Uint4Ptr)MemNew((amaip->alnsaps)*sizeof(Uint4));
   amaip->lens = (Int4Ptr)MemNew((amaip->alnsaps)*sizeof(Int4));
   amaip->rowsource = (RowSourcePtr PNTR)MemNew(2*sizeof(RowSourcePtr));
   amaip->rowsource[0] = (RowSourcePtr)MemNew(sizeof(RowSource));
   amaip->rowsource[0]->id = SeqIdDup(sap->master);
   amaip->rowsource[0]->which_saps = (Uint4Ptr)MemNew((amaip->alnsaps+1)*sizeof(Uint4));
   amaip->rowsource[0]->num_in_sap = (Uint4Ptr)MemNew((amaip->alnsaps+1)*sizeof(Uint4));
   amaip->rowsource[1] = (RowSourcePtr)MemNew(sizeof(RowSource));
   sip = AlnMgrGetNthSeqIdPtr(amaip->saps[0], 3-((SAIndexPtr)amaip->saps[0]->saip)->master);
   amaip->rowsource[1]->id = sip;
   amaip->rowsource[1]->which_saps = (Uint4Ptr)MemNew((amaip->alnsaps+1)*sizeof(Uint4));
   amaip->rowsource[1]->num_in_sap = (Uint4Ptr)MemNew((amaip->alnsaps+1)*sizeof(Uint4));
   for (i=0; i<amaip->alnsaps; i++)
   {
      amaip->rowsource[0]->which_saps[i] = amaip->rowsource[1]->which_saps[i] = tiparray[i]->numsap + 1;
      amaip->rowsource[0]->num_in_sap[i] = tiparray[i]->numgap;
      amaip->rowsource[1]->num_in_sap[i] = 3-tiparray[i]->numgap;
      amaip->lens[i] = AlnMgrGetAlnLength(amaip->saps[tiparray[i]->numsap], FALSE);
      if (i>0)
         amaip->aligncoords[i] = amaip->aligncoords[i-1] + amaip->lens[i-1];
      else
         amaip->aligncoords[i] = 0;
   }
   amaip->rowsource[0]->numsaps = amaip->rowsource[1]->numsaps = amaip->alnsaps;
   amaip->master = 1;
   amaip->numrows = 2;
   for (i=0; i<amaip->alnsaps; i++)
   {
      MemFree(tiparray[i]);
   }
   MemFree(tiparray);
   sap->segs = (Pointer)(amaip->saps[0]);
   for (i=1; i<amaip->alnsaps; i++)
   {
      amaip->saps[i-1]->next = amaip->saps[i];
   }
   amaip->saps[amaip->alnsaps-1]->next = NULL;
   /*    amaip->numsaps = amaip->alnsaps; */
   MemFree(salp_list);
   sap->type = SAT_MASTERSLAVE;
   amaip->mstype = AM_SEGMENTED_MASTERSLAVE;
   AlnMgrDeleteHiddenEx(sap, FALSE,FALSE);   
   return TRUE;
}

NLM_EXTERN SeqAlignPtr AlnMgrDupTopNByScore(SeqAlignPtr sap, Int4 n)
{
   AMAlignIndexPtr  amaip;
   FloatHi          bit_score;
   FloatHi          evalue;
   Int4             i;
   Int4             num;
   Int4             number;
   SeqAlignPtr      salp;
   SeqAlignPtr      salp_head;
   AMAlignInfoPtr   salp_list;
   SeqAlignPtr      salp_prev;
   Int4             score;

   if (sap == NULL)
      return FALSE;
   i = AlnMgrCheckAlignForParent(sap);
   if (i != AM_PARENT)
      return FALSE;
   amaip = (AMAlignIndexPtr)sap->saip;
   if (amaip == NULL)
      return FALSE;
   salp = (SeqAlignPtr)sap->segs;
   num = amaip->numsaps;
   if (n > num || n < 0)
      n = num;
   salp_list = Calloc(num, sizeof (AMAlignInfo));
   for (i=0; i<num; i++, salp=salp->next)
   {
      salp_list[i].align=salp;
      GetScoreAndEvalue(salp, &score, &bit_score, &evalue, &number);
      salp_list[i].align_len = score;
   }
   HeapSort (salp_list, num, sizeof (AMAlignInfo), AMCompareAlignInfoProc);
   salp_head = salp_prev = SeqAlignDup(salp_list[0].align);
   for (i=1; i<n; i++)
   {
      salp_prev->next = SeqAlignDup(salp_list[i].align);
      salp_prev = salp_prev->next;
   }
   return salp_head;
}

static int LIBCALLBACK AMCompareAlignInfoProc(VoidPtr ptr1, VoidPtr ptr2)
{
   AMAlignInfoPtr aip_1;
   AMAlignInfoPtr  aip_2;
   if (ptr1 != NULL && ptr2 != NULL)
   {
      aip_1 = (AMAlignInfoPtr) ptr1;
      aip_2 = (AMAlignInfoPtr) ptr2;
      if(aip_1->align_len > aip_2->align_len)
         return -1;
      else if(aip_1->align_len < aip_2->align_len)
         return 1;
      else
         return 0;
  }
  return 0;
}

/***************************************************************************
*
*  AlnMgrSortAlnSetByNthRowPos sorts a set of alignments so that they
*  are in (increasing) order along the specified row (to make sense, this
*  set of alignments should all have the same rows).
*
***************************************************************************/
NLM_EXTERN void AlnMgrSortAlnSetByNthRowPos(SeqAlignPtr sap, Int4 row)
{
   AMAlignIndexPtr  amaip;
   AMBittyPtr       bit;
   Int4             i;
   SeqAlignPtr      PNTR saparray;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   amaip = (AMAlignIndexPtr)(sap->saip);
   bit = (AMBittyPtr)MemNew((amaip->numsaps)*sizeof(AMBitty));
   saparray = (SeqAlignPtr PNTR)MemNew((amaip->numsaps)*sizeof(SeqAlignPtr));
   for (i=0; i<amaip->numsaps; i++)
   {
      bit[i].num2 = i;
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i], row, &bit[i].num1, NULL);
      saparray[i] = amaip->saps[i];
   }
   HeapSort(bit, amaip->numsaps, sizeof(AMBitty), AMCompareStarts);
   for (i=0; i<amaip->numsaps; i++)
   {
      amaip->saps[i] = saparray[bit[i].num2];
   }
   MemFree(saparray);
   MemFree(bit);
}

static int LIBCALLBACK AMCompareStarts(VoidPtr ptr1, VoidPtr ptr2)
{
   AMBittyPtr  bit1;
   AMBittyPtr  bit2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      bit1 = (AMBittyPtr)ptr1;
      bit2 = (AMBittyPtr)ptr2;
      if (bit1->num1 < bit2->num1)
         return -1;
      else if (bit1->num1 > bit2->num1)
         return 1;
      else
         return 0;
   }
   return 0;
}

NLM_EXTERN void am_print_seqalign_indexes(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   DenseSegPtr      dsp;
   Int4             i;
   Int4             j;
   SAIndexPtr       saip;

   if (!sap)
      return;
   if (!sap->saip)
      return;
   while (sap)
   {
      if (sap->segtype == SAS_DENSEG && sap->saip)
      {
         dsp = (DenseSegPtr)sap->segs;
         if (sap->saip->indextype == INDEX_SEGS)
            saip = (SAIndexPtr)(sap->saip);
         printf("\naligncoords: ");
         for (i=0; i<(dsp->numseg); i++)
         {
            printf("%d ", saip->aligncoords[i]);
         }
         fflush(stdout);
         for (i=0; i<(dsp->dim); i++)
         {
            printf("\n");
            printf("Sequence %d:", i);
            for (j=0; j<(saip->ssdp[i]->numsect); j++)
            {
               printf("%d ", saip->ssdp[i]->sect[j]);
            }
            fflush(stdout);
         }
      } else if (sap->segtype == SAS_DISC && sap->saip)
      {
         if (sap->saip->indextype == INDEX_PARENT)
            amaip = (AMAlignIndexPtr)(sap->saip);
         if (sap->type == SAT_PARTIAL)
            printf("SAT_PARTIAL\n");
         else if (sap->type == SAT_MASTERSLAVE)
            printf("SAT_MASTERSLAVE\n");
         printf("Parent info:\n");
         printf("numbsqs = %d\n", amaip->numbsqs);
         printf("numsaps = %d\n", amaip->numsaps);
         printf("alnsaps = %d\n", amaip->alnsaps);
         printf("numseg = %d\n", amaip->numseg);
         fflush(stdout);
         for (i=0; i<amaip->numbsqs; i++)
         {
            printf("Sequence %d:", i);
            printf(" %d saps\n", amaip->amadp[i]->numsaps);
            fflush(stdout);
         }
         printf("Starts: ");
         if (sap->type == SAT_MASTERSLAVE && amaip->mstype == AM_SEGMENTED_MASTERSLAVE)
         {
            printf("Segmented\n");
         } else 
         {
            for (i=0; i<(amaip->numseg*amaip->numsaps); i++)
            {
               printf("%d ", amaip->starts[i]);
               if (!fmod(i+1, amaip->numsaps))
                  printf("\n");
               fflush(stdout);
            }
         }
         fflush(stdout);
         printf("\nTotal Length: %d \n", AlnMgrGetAlnLength(sap, TRUE));
         printf("Alignment Length: %d\n", AlnMgrGetAlnLength(sap, FALSE));
         if (amaip->lens)
         {
            printf("lens: ");
            for (i=0; i<amaip->numseg; i++)
            {
               printf("%i ", amaip->lens[i]);
            }
            printf("\n");
            fflush(stdout);
            printf("aligncoords: ");
            for (i=0; i<amaip->numseg; i++)
            {
               printf("%i ", amaip->aligncoords[i]);
            }
            printf("\n");
            fflush(stdout);
         }
         if (amaip->saps)
         {
            for (i=0; i<amaip->numbsqs; i++)
            {
               printf("Segments: ");
               for (j=0; j<(amaip->amadp[i]->numseg); j++)
               {
                  printf("%d ", amaip->amadp[i]->segments[j]);
               }
               printf("\n");
               fflush(stdout);
            }
         }
         if (amaip->rowsource)
         {
            printf("Rowsource arrays:\n");
            for (i=0; i<(amaip->numrows); i++)
            {
               printf("row %d  ", (i+1));
               for (j=0; j<(amaip->rowsource[i]->numsaps); j++)
               {
                  printf("%d: %d   ", amaip->rowsource[i]->which_saps[j], amaip->rowsource[i]->num_in_sap[j]);
               }
               printf("\n");
            }
         }
         am_print_seqalign_indexes((SeqAlignPtr)sap->segs);
      }
      sap = sap->next;
   }
   return;
}

/*CHECK*/
NLM_EXTERN Int4 AlnMgrCheckAlignForParent(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;

   if (sap->segtype == SAS_DISC)
   {
      if (!sap->saip)
      {
         if (!AlnMgrIndexSeqAlign(sap))
            return -1;
         else
            return AM_PARENT;
      } else if (sap->saip->indextype == INDEX_PARENT)
      {
         amaip = (AMAlignIndexPtr)(sap->saip);
         if (amaip->mstype != AM_LITE)
            return AM_PARENT;
         else
            return -1;
      } else
      {
         return -1;
      }
   } else if (sap->segtype == SAS_DENSEG)
   {
      if (!sap->saip)
      {
         if (sap->segs == NULL)
            return -1;
         AlnMgrAnythingToSeg(sap);
         if (!AlnMgrIndexLinkedSegs((SeqAlignPtr)sap->segs))
            return -1;
         return AM_PARENT;
      } else if (sap->saip->indextype == INDEX_SEGS)
      {
         return AM_CHILD;
      } else
      {
         return -1;
      }
   }
   return -1;
}

/* check to see if a seqalign contains a sip, return row */
static Int4 AlnMgrContainsID(SeqAlignPtr sap, SeqId *sip)
{
   DenseSegPtr  dsp;
   SeqId *sip_tmp;
   Int4 count;

   if (!sap || !sip) return FALSE;
   dsp = (DenseSegPtr)sap->segs;
   if (!dsp) return 0;

   for(sip_tmp = dsp->ids, count = 1; sip_tmp;
       sip_tmp = sip_tmp->next, count++)
       if (SeqIdComp(sip_tmp, sip) == SIC_YES) return count;
   return 0;
}

/* used to pass in values for sorting using AlnMgrCompareSortStruct() */
typedef struct _AlnMgrSortStruct
{
    SeqId *sip;
    SeqAlign *sap;
    Int4 row, start, stop;
    Uint2 strand;
} AlnMgrSortStruct;

/* compare  two seqaligns by position on a particular sequence.  Assumes both
   seqaligns contains the sequence */
static int LIBCALLBACK AlnMgrCompareSortStruct(void *e1, void *e2)
{ 
   AlnMgrSortStruct *amss1, *amss2;

   if (!e1||!e2) return 0;
   amss1 = (AlnMgrSortStruct *)e1;
   amss2 = (AlnMgrSortStruct *)e2;
   if(!amss1->sap || !amss2->sap) return 0;

   if (amss1->strand == 0)
      amss1->strand = Seq_strand_plus;
   if (amss2->strand == 0)
      amss2->strand = Seq_strand_plus;

   if ((amss1->strand == amss2->strand) && amss1->strand != Seq_strand_minus)
   {
      if (amss1->start < amss2->start)
         return -1;
      else if (amss2->start < amss1->start)
         return 1;
      else if (amss1->start == amss2->start)
      {
        if (amss1->stop < amss2->stop)
           return -1;
        else if (amss2->stop < amss1->stop)
           return 1;
        else
           return 0;
      }
   } else if ((amss1->strand == amss2->strand) && amss1->strand ==  Seq_strand_minus)
   {
      if (amss1->start > amss2->start)
         return -1;
      else if (amss2->start > amss1->start)
         return 1;
      else if (amss1->start == amss2->start)
      {
         if (amss1->stop < amss2->stop)
            return -1;
         else if (amss2->stop < amss1->stop)
            return 1;
         else
            return 0;
      }
   }
   return 0;
}

/* same functionality as AlnMgrSortSeqAligns */
NLM_EXTERN SeqAlignPtr PNTR AlnMgrSortbyID (SeqAlignPtr sap, SeqId *sip, Int4Ptr numsap)
{
   SeqAlignPtr  PNTR head;
   Int4         i, j;
   SeqAlignPtr  tmp;
   AlnMgrSortStruct *amss;
   Int4 row;

   if(!sap || !sap || !numsap) return NULL;

   /* count number of seqaligns and init return array */
   for(tmp = sap; tmp; tmp=tmp->next) (*numsap)++;
   head = MemNew((*numsap)*sizeof(SeqAlignPtr));
   amss = MemNew((*numsap)*sizeof(AlnMgrSortStruct));

   /* separate out the seqaligns that don't contain the sip */
   for(tmp = sap, i = j = 0; tmp; tmp = tmp->next) {
       row = AlnMgrContainsID(tmp, sip);
       if(!row) {
           head[i] = tmp;
           i++;
       } else {
           amss[j].sip = sip;
           amss[j].sap = tmp;
           amss[j].row = row;
           AlnMgrGetNthSeqRangeInSA(tmp, row, &(amss[j].start), &(amss[j].stop));
           amss[j].strand = AlnMgrGetNthStrand(tmp, row);           
           j++;
       }
   }

/*   qsort(amss, j, sizeof(AlnMgrSortStruct), AlnMgrCompareSortStruct); */
   HeapSort((Pointer)amss, (size_t)j, sizeof(AlnMgrSortStruct), AlnMgrCompareSortStruct);

   for(i = 0; i < j; i++) head[*numsap - j + i] = amss[i].sap;

   MemFree(amss);
   return head;
}

/***********************************************************************
*
*  AlnMgrSortSeqAligns is a variant of the ValNodeSort function, and
*  calls very similar heapsort functions.  It can take a comparison
*  function that needs userdata, so more specific sorts are possible
*  without defining special structures for every type of sort.
*
***********************************************************************/
NLM_EXTERN SeqAlignPtr PNTR AlnMgrSortSeqAligns (SeqAlignPtr sap, int (LIBCALLBACK *compar)(VoidPtr, VoidPtr, VoidPtr), VoidPtr userdata, Int4Ptr numsap)
{
   SeqAlignPtr  PNTR head;
   Int4         i;
   Int4         num;
   SeqAlignPtr  tmp;

   if (!sap)
      return NULL;
   tmp = sap;
   num = 0;
   while (tmp)
   {
      num++;
      tmp = tmp->next;
   }
   head = MemNew(((size_t) num + 1)*sizeof(SeqAlignPtr));
   tmp = sap;

   for (i = 0; i<num; i++)
   {
      head[i]=tmp;
      tmp = tmp->next;
      if (!tmp)
         break;
   }
   heapsort_with_userdata(head, (size_t)num, sizeof(SeqAlignPtr), compar, userdata);
   if (numsap)
      *numsap = num;
   return head;
}

static void heapsort_with_userdata (VoidPtr b, size_t nel, size_t width, int (LIBCALLBACK *compar)PROTO((VoidPtr, VoidPtr, VoidPtr)), VoidPtr userdata)
{
   register CharPtr base = (CharPtr)b;
   register size_t i;
   register char ch;
   register CharPtr base0=(CharPtr)base, lim, basef;

   if (nel<2)
      return;
   lim = &base[((nel-2)/2)*width];
   basef = &base[(nel-1)*width];
   i = nel/2;
   for (base = &base0[(i-1)*width]; i>0; base=base-width)
   {
      heapify_with_userdata(base0, base, lim, basef, width, compar, userdata);
      i--;
   }
   for (base=&base0[(nel-1)*width]; base>base0; base -= width)
   {
      for (i = 0; i<width; i++)
      {
         ch = base0[i];
         base0[i] = base[i];
         base[i] = ch;
      }
      lim = base0 + ((base-base0)/2 - width);
      if (base> (base0+width))
         heapify_with_userdata(base0, base0, lim, base-width, width, compar, userdata);
   }
   return;
}

static void heapify_with_userdata(CharPtr base0, CharPtr base, CharPtr lim, CharPtr last, size_t width, int(LIBCALLBACK *compar)PROTO((VoidPtr, VoidPtr, VoidPtr)), VoidPtr userdata)
{
   register size_t i;
   register char ch;
   register CharPtr left_son, large_son;

   left_son = base0 + 2*(base-base0) + width;
   while (base<=lim)
   {
      if (left_son == last)
      {
         large_son = left_son;
      } else
      {
         if((*compar)(left_son, left_son+width, userdata) >= 0)
            large_son = left_son;
         else
            large_son = left_son + width;
      }
      if ((*compar)(base, large_son, userdata) < 0)
      {
         for (i = 0; i<width; i++)
         {
            ch = base[i];
            base[i] = large_son[i];
            large_son[i] = ch;
         }
         base = large_son;
         left_son = base0 + 2*(base-base0) + width;
      } else
      {
         break;
      }
   }
   return;
}

/*************************************************************************
*
*  sorting comparison functions
*
*************************************************************************/
/**********************************************************************
*
*  AlnMgrCompareIncreasingBySeqIdPtr takes a SeqIdPtr as userdata,
*  and sorts the alignments in increasing order according to the
*  region of the bioseq indicated that is contained in the alignment.
*  If the bioseq is not in the alignment, the alignment will be put
*  first, so all alignments in which the given bioseq does not
*  participate occur at the beginning of the list, making it easy to
*  check for them and remove them.
*
**********************************************************************/
NLM_EXTERN int LIBCALLBACK AlnMgrCompareIncreasingBySeqIdPtr (VoidPtr base, VoidPtr large_son, VoidPtr userdata)
{
   Boolean      done;
   DenseSegPtr  dsp1;
   DenseSegPtr  dsp2;
   Int4         n1;
   Int4         n2;
   SeqAlignPtr  sap1;
   SeqAlignPtr  sap2;
   SeqIdPtr     sip;
   SeqIdPtr     sip_tmp;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;
   Uint2        strand1;
   Uint2        strand2;

   sap1 = *((SeqAlignPtr PNTR) base);
   sip = (SeqIdPtr)userdata;
   if (!sap1||!sip) return 0;
   dsp1 = (DenseSegPtr)sap1->segs;
   if (!dsp1) return 0;
   n1 = 0;
   done = FALSE;
   sip_tmp = dsp1->ids;
   while (sip_tmp)
   {
      n1++;
      if (SeqIdComp(sip_tmp, sip) == SIC_YES) {
          done = TRUE;
          break;
      }
      sip_tmp = sip_tmp->next;
   }
   if (!done) return -1;
   sap2 = *((SeqAlignPtr PNTR) large_son);
   if (!sap2) return 0;
   dsp2 = (DenseSegPtr)sap2->segs;
   if (!dsp2) return 0;
   n2 = 0;
   done = FALSE;
   sip_tmp = dsp2->ids;
   while (sip_tmp)
   {
      n2++;
      if (SeqIdComp(sip_tmp, sip) == SIC_YES) {
          done = TRUE;
          break;
      }
      sip_tmp = sip_tmp->next;
   }
   if (!done)
      return 1;
   AlnMgrGetNthSeqRangeInSA(sap1, n1, &start1, &stop1);
   AlnMgrGetNthSeqRangeInSA(sap2, n2, &start2, &stop2);
   strand1 = AlnMgrGetNthStrand(sap1, n1);
   strand2 = AlnMgrGetNthStrand(sap2, n2);
   if (strand1 == 0)
      strand1 = Seq_strand_plus;
   if (strand2 == 0)
      strand2 = Seq_strand_plus;
   if ((strand1 == strand2) && strand1 != Seq_strand_minus)
   {
      if (start1 < start2)
         return -1;
      else if (start2 < start1)
         return 1;
      else if (start1 == start2)
      {
        if (stop1 < stop2)
           return -1;
        else if (stop2 < stop1)
           return 1;
        else
           return 0;
      }
   } else if ((strand1 == strand2) && strand1 ==  Seq_strand_minus)
   {
      if (start1 > start2)
         return -1;
      else if (start2 > start1)
         return 1;
      else if (start1 == start2)
      {
         if (stop1 < stop2)
            return -1;
         else if (stop2 < stop1)
            return 1;
         else
            return 0;
      }
   }
   else
      return 0;
   return 0;
}

/*********************************************************************
*
*  AlnMgrFindFirst is crucial to the AlnMgrMakeFakeMultiple function;
*  it uses the sorted order of the seqaligns in each AMAlignDat
*  structure to guide a heapsort of all the seqaligns.
*
*********************************************************************/
NLM_EXTERN int LIBCALLBACK AlnMgrFindFirst(VoidPtr base, VoidPtr large_son, VoidPtr userdata)
{
   AMAlignDatPtr    amadp;
   AMAlignIndexPtr  amaip;
   Int4             i;
   SeqAlignPtr      sap1;
   SeqAlignPtr      sap2;
   Int4             x;
   Int4             y;
   Int4             z;

   amaip = (AMAlignIndexPtr)userdata;
   if (amaip == NULL || base == NULL || large_son == NULL)
      return 0;
   sap1 = *((SeqAlignPtr PNTR) base);
   sap2 = *((SeqAlignPtr PNTR) large_son);
   if (base == large_son)
      return 0;
   x = y = -1;
   z = amaip->numbsqs;
   while (z)
   {
      amadp = amaip->amadp[(amaip->numbsqs - z)];
      for (i=0; i<(amadp->numsaps); i++)
      {
         if (amadp->saps[i] == sap1)
            x=i;
         else if (amadp->saps[i] == sap2)
            y=i;
      }
      if (x!=-1 && y!=-1)
      {
         if (x < y)
            return -1;
         else if (y < x)
            return 1;
      }
      z--;
   }
   return 0;
}

NLM_EXTERN int LIBCALLBACK AlnMgrCompareTips(VoidPtr base, VoidPtr large_son)
{
   AMTinyInfoPtr  tip1;
   AMTinyInfoPtr  tip2;

   tip1 = *((AMTinyInfoPtr PNTR) base);
   tip2 = *((AMTinyInfoPtr PNTR) large_son);
   if (tip1 == NULL || tip2 == NULL)
      return 0;
   if (tip1->start < tip2->start)
      return -1;
   else if (tip1->start > tip2->start)
      return 1;
   else
   {
      if (tip1->which < tip2->which)
         return -1;
      else if (tip1->which > tip2->which)
         return 1;
      else if(tip1->stop>tip2->stop) 
          return -1; /* put longer segments first */
      else if (tip1->stop<tip2->stop)
          return 1;
      else
         return 0;
   }
}


/************************************************************************
*
*  AlnMgrGetNextLengthBit should be called iteratively on an alignment
*  to return the lengths of all the aligned and unaligned pieces in
*  the alignment.  Don't change the value in r, just pass in a pointer 
*  to an allocated Int4 set to 0 initially.  The lengths of the unaligned
*  pieces are precomputed using AlnMgrGetMaxUnalignedLength; if no
*  precomputed values are found, this function is used to compute the
*  lengths on the fly.
*
************************************************************************/
NLM_EXTERN Boolean AlnMgrGetNextLengthBit(SeqAlignPtr sap, Int4Ptr length, Int4Ptr r)
{
   AMAlignIndexPtr  amaip;
   Int4             i;
   RowSourcePtr     rsp;

   if (sap == NULL || length == NULL || r == NULL)
      return FALSE;
   i = AlnMgrCheckAlignForParent(sap);
   if (i == AM_CHILD)
   {
      if (*r == 1)
         return FALSE;
      *length = AlnMgrGetAlnLength(sap, FALSE);
      *r = 1;
      return TRUE;
   } else if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)sap->saip;
      if (amaip->mstype == AM_LITE)
         return FALSE;
      if (sap->type == SAT_PARTIAL)
      {
         if (*r < 0)
         {
            if ((-*r) >= amaip->numsaps)
               return FALSE;
            if (amaip->ulens == NULL)
               AlnMgrSetUnalignedLengths(sap);
            *length = -(amaip->ulens[(-*r)-1]);
            *r = -(*r);
            return TRUE;
         } else
         {
            if (*r >= amaip->numsaps)
               return FALSE;
            *length = AlnMgrGetAlnLength(amaip->saps[*r], FALSE);
            *r = -((*r)+1);
            return TRUE;
         }
      } else if (sap->type == SAT_MASTERSLAVE)
      {
         if (amaip->mstype == AM_MASTERSLAVE)
         {
            if (*r == 1)
               return FALSE;
            *length = amaip->aligncoords[amaip->numseg-1] + amaip->lens[amaip->numseg-1];
            *r = 1;
            return TRUE;
         } else if (amaip->mstype == AM_SEGMENTED_MASTERSLAVE || amaip->mstype == AM_NULL)
         {
            if (*r < 0)
            {
               if ((-*r) >= amaip->numseg)
                  return FALSE;
               rsp = amaip->rowsource[amaip->master-1];
               if (amaip->ulens == NULL)
                  AlnMgrSetUnalignedLengths(sap);
               *length = -(amaip->ulens[(-*r)-1]);
               *r = -(*r);
               return TRUE;
            } else
            {
               if (*r >= amaip->numseg)
               {
                  if (amaip->numseg > 0)
                     return FALSE;
                  else
                  {
                     *length = -(amaip->ulens[0]);
                     *r = -((*r)+1);
                     return TRUE;
                  }
               }
               rsp = amaip->rowsource[amaip->master-1];
               *length = AlnMgrGetAlnLength(amaip->saps[rsp->which_saps[*r]-1], FALSE);
               *r = -((*r)+1);
               return TRUE;
            }
         }
      }
   }
   return FALSE;
}

NLM_EXTERN Int4 AlnMgrGetMaxUnalignedLength(SeqAlignPtr sap1, SeqAlignPtr sap2)
{
   Int4         max;
   Int4         n1, n1max;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;
   SeqId *sip1, *sip2;

   if (sap1 == NULL || sap2 == NULL)
      return 0;
   max = 0;
   n1max = AlnMgrGetNumRows(sap1);
   if(n1max != AlnMgrGetNumRows(sap2)) return 0;
   for (n1 = 1; n1 <= n1max; n1++)
   {
       sip1 = AlnMgrGetNthSeqIdPtr(sap1, n1);
       sip2 = AlnMgrGetNthSeqIdPtr(sap2, n1);
       if(SeqIdComp(sip1, sip2) != SIC_YES) {
           SeqIdFree(sip1);
           SeqIdFree(sip2);
           return 0;
           }
      SeqIdFree(sip1);
      SeqIdFree(sip2);
      AlnMgrGetNthSeqRangeInSA(sap1, n1, &start1, &stop1);
      if (n1 >= 0)
      {
         AlnMgrGetNthSeqRangeInSA(sap2, n1, &start2, &stop2);
         if (start2 > stop1)
         {
            if (start2 - stop1 - 1 > max)
               max = start2 - stop1 - 1;
         } else
         {
            if (start1 - stop2 - 1 > max)
               max = start1 - stop2 - 1;
         }
      }
   }
   return max;
}

static void AlnMgrSetUnalignedLengths(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   Int4             i;
   Int4             l;
   Int4             max;
   Int4             n;
   RowSourcePtr     rsp;
   SeqAlignPtr      sap1;
   SeqAlignPtr      sap2;
   Int4             start1;
   Int4             start2;
   Int4             stop1;
   Int4             stop2;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   amaip = (AMAlignIndexPtr)(sap->saip);
   if (amaip->ulens != NULL)
      MemFree(amaip->ulens);
   amaip->ulens = (Int4Ptr)MemNew((amaip->numsaps)*sizeof(Int4));
   for (i=1; i<amaip->numseg; i++)
   {
      max = 0;
      for (n=0; n<amaip->numrows; n++)
      {
         rsp = amaip->rowsource[n];
         if (rsp != NULL)
         {
            if (rsp->which_saps[i-1] != 0 && rsp->which_saps[i] != 0 && rsp->which_saps[i-1] <= amaip->numsaps && rsp->which_saps[i] <= amaip->numsaps)
            {
               sap1 = amaip->saps[rsp->which_saps[i-1]-1];
               sap2 = amaip->saps[rsp->which_saps[i]-1];
               AlnMgrGetNthSeqRangeInSA(sap1, rsp->num_in_sap[i-1], &start1, &stop1);
               AlnMgrGetNthSeqRangeInSA(sap2, rsp->num_in_sap[i], &start2, &stop2);
               if (rsp->strand != Seq_strand_minus)
                  l = start2 - stop1 - 1;
               else
                  l = start1 - stop2 - 1;
               if (l > max)
                  max = l;
            }
         }
      }
      amaip->ulens[i-1] = max;
   }
   return;
}

/*************************************************************************
*
*  AlnMgrGetNextAlnBit takes an AlnMsgPtr, with (at the minimum) the
*  which_bsq field filled in to indicate which bioseq should be returned.
*  The function returns the segments of the bioseq which span the region
*  of the alignment indicated, and can return them according to either
*  alignment coordinates (if which_master is NULL) or a master coordinate
*  system (need to fill in the SeqIdPtr of the master).  The function
*  returns TRUE if there are more segments of the bioseq to retrieve,
*  and FALSE if not.  It uses the two binary search functions to quickly
*  retrieve the required data from the indexes.  (NEXT)
*
*************************************************************************/
NLM_EXTERN Boolean AlnMgrGetNextAlnBit (SeqAlignPtr sap, AlnMsgPtr amp)
{
   AMAlignIndexPtr  amaip;
   DenseSegPtr      dsp;
   Int4             endoffset;
   Boolean          found;
   Int4             i;
   Int4             len;
   Int4             offset;
   Boolean          retval;
   Int4             rf_tmp;
   SAIndexPtr       saip;
   SASeqDatPtr      ssdp;
   Int4             start_b;
   Uint4            start_m;
   Uint4            start_tmp;
   Uint4            stop_m;
   Uint4            stop_tmp;

   retval = FALSE;
   if (!sap)
      return retval;
   if (!amp)
      return retval;
   i = AlnMgrCheckAlignForParent(sap);
   if (i == AM_CHILD)
   {
      saip = (SAIndexPtr)sap->saip;
      dsp = (DenseSegPtr)sap->segs;
      if (!dsp)
         return retval;
      if (!amp->which_master)
      {
         if (amp->place == 1)
            return retval;
         len = AlnMgrGetAlnLength(sap, FALSE);
         if (amp->to_m > len-1) /* len-1 was len : HS 7/24/00 */
            return retval;
         if (amp->to_m < 0)
            amp->to_m = len - 1;
         if (amp->row_num == -1)
         {
            if (!amp->which_bsq)
               return retval;
            amp->row_num = AlnMgrGetNForSip(sap, amp->which_bsq) - 1;
            if (amp->row_num == -1)
               return retval;
         }
         if (amp->prev != -2)
         {
            start_m = amp->prev;
         } else
         {
            start_m = binary_search_on_uint4_list(saip->aligncoords, amp->from_m, dsp->numseg);
            amp->real_from = amp->from_m;
         }
         stop_m = binary_search_on_uint4_list(saip->aligncoords, amp->to_m, dsp->numseg);
         ssdp = saip->ssdp[amp->row_num-1];
         offset = amp->real_from - saip->aligncoords[start_m];
         start_b = binary_search_on_uint2_list(ssdp->sect, start_m, ssdp->numsect);

         /* bug fix -- Dave & Lewis 7/20/00 */
         if (start_b == -1) {
           amp->strand = Seq_strand_plus;
         }
         /* end of fix */
         else {
           if (dsp->strands != NULL && dsp->strands[start_b*(dsp->dim)+amp->row_num-1] == Seq_strand_minus)
              amp->strand = Seq_strand_minus;
           else
              amp->strand = Seq_strand_plus;
         }
         if ((stop_m - start_m) > 0)
         {
            retval = TRUE;
            if (start_b >= 0)
            {
               if (amp->strand != Seq_strand_minus)
               {
                  amp->from_b = dsp->starts[start_b*(dsp->dim)+amp->row_num-1] + offset;
                  amp->to_b = dsp->starts[start_b*(dsp->dim)+amp->row_num-1] + dsp->lens[start_b] - 1;
               } else
               {
                  amp->from_b = dsp->starts[start_b*(dsp->dim)+amp->row_num-1];
                  amp->to_b = amp->from_b + dsp->lens[start_b] - 1 - offset;
               }
               amp->gap = 0;
            } else
            {
               amp->from_b = amp->real_from;
               amp->to_b = saip->aligncoords[start_m + 1] - 1;
               amp->gap = 1; 
            }
            amp->real_from = saip->aligncoords[start_m + 1];
            amp->prev = start_m + 1;
         } else
         {
            amp->place = 1;
            endoffset = amp->to_m - saip->aligncoords[start_m];
            if (start_b >= 0)
            {
               if (amp->strand != Seq_strand_minus)
               {
                  amp->from_b = dsp->starts[start_b*(dsp->dim)+amp->row_num-1] + offset;
                  amp->to_b = dsp->starts[start_b*(dsp->dim)+amp->row_num-1] + endoffset;
               } else
               {
                  amp->from_b = dsp->starts[start_b*(dsp->dim)+amp->row_num-1] + dsp->lens[start_b]  - endoffset - 1;
                  amp->to_b = amp->from_b + amp->to_m - amp->real_from;
               }
               amp->gap = 0;
            } else
            {
               amp->from_b = amp->real_from;
               amp->to_b = amp->to_m;
               amp->gap = 1;
            }
            amp->real_from = 0;
            amp->prev = -2;
            retval = TRUE;
         }
      }
   } else if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)(sap->saip);
      if (amaip && (!amaip->saps || amaip->mstype == AM_LITE || amaip->mstype == AM_NEATINDEX || amaip->mstype == AM_NULL))
         return retval;
      if (amp->place == 1)
         return retval;
      if (!amp->which_bsq && amp->row_num==-1)
         return retval;
      if (sap->type == SAT_PARTIAL && amp->which_master == NULL)
      {
         len = AlnMgrGetAlnLength(sap, FALSE);
         if (amp->to_m < 0)
            amp->to_m = len-1;
         if (amp->to_m > len-1)
            return FALSE;
         if (amp->prev_sap != -2)
         {
            start_m = amp->prev_sap;
            amp->len_left = amp->to_m - amp->real_from + 1;
         } else
         {
            start_m = binary_search_on_uint4_list(amaip->aligncoords, amp->from_m, amaip->alnsaps);
            amp->real_from = amp->from_m;
            amp->prev_sap = start_m;
            amp->len_left = amp->to_m - amp->from_m + 1;
         }
         stop_m = binary_search_on_uint4_list(amaip->aligncoords, amp->to_m, amaip->alnsaps);
         offset = amp->real_from - amaip->aligncoords[start_m];
         if (amp->len_left > (amaip->lens[start_m]-offset))
         {
            endoffset = amaip->lens[start_m] - offset;
         } else
         {
            endoffset = amp->len_left;
         }
         stop_tmp = amp->to_m;
         start_tmp = amp->from_m;
         if ((stop_m - start_m) == 0)
         {
            amp->from_m = offset + amaip->starts[start_m];
            amp->to_m = amp->from_m + endoffset - 1;
            /* amp->prev = -2; HS 7/24/00 removed */
            rf_tmp = amp->real_from;
            AlnMgrGetNextAlnBit((amaip->saps[start_m]), amp);
            /* HS 7/24/00           amp->len_left = amp->len_left - (amp->to_b - amp->from_b + 1); */
            amp->len_left = amp->len_left - (amp->to_m - amp->from_m + 1); 
            amp->to_m = stop_tmp;
            amp->from_m = start_tmp;
            if (amp->len_left == 0)
            {
               amp->real_from = amp->to_m + 1;
               amp->prev_sap = -2;
               amp->place = 1;
               amp->send_space = 1;
            } else
            {
               amp->real_from = rf_tmp + (amp->to_b - amp->from_b + 1);
               amp->place = 0;
            }
            retval = TRUE;
         } else
         {
            retval = TRUE;
            amp->from_m = offset + amaip->starts[start_m];
            amp->to_m = amp->from_m + endoffset - 1;
            AlnMgrGetNextAlnBit((amaip->saps[start_m]), amp);
            amp->len_left = amp->len_left - (amp->to_m - amp->from_m + 1);
            amp->to_m = stop_tmp;
            amp->real_from = amp->to_m - amp->len_left + 1;
            amp->from_m = start_tmp;
            if (amp->place == 1)
            {
               amp->prev_sap += 1;
               amp->send_space = TRUE;
               if (amp->len_left > 0)
                  amp->place = 0;
            }
            /* bug fix -- Dave 7/21/00 */
            /* if (amp->len_left == 0 || amp->real_from >= amp->to_m) */
            if (amp->len_left == 0 || amp->real_from > amp->to_m)
            {
               amp->place = 1;
               retval = FALSE;
               amp->prev_sap = -2;
            }
         }
      } else if (sap->type == SAT_MASTERSLAVE && amaip->mstype == AM_MASTERSLAVE && amp->which_master == NULL)
      {
         int j;
         if (amp->place == 1)
            return retval;
         len = AlnMgrGetAlnLength(sap, FALSE);
         if (amp->to_m > len-1)
            return retval;
         if (amp->to_m < 0)
            amp->to_m = len-1;
         if (amp->row_num == -1) 
         { 
            if(!amp->which_bsq)
                return retval;
            else 
            {
                amp->row_num = AlnMgrGetNForSip(sap,amp->which_bsq);
                if(amp->row_num == -1)
                    return retval;
            }
         }
         if (amp->row_num == amaip->master)
         {
            amp->strand = Seq_strand_plus;
            if (amp->prev != -2)
            {
               amp->prev += 1;
               start_m = amp->prev;
            } else
            {
               start_m = binary_search_on_uint4_list(amaip->aligncoords, amp->from_m, amaip->numseg);
               amp->real_from = amp->from_m;
               amp->prev = start_m;
            }
            offset = amp->real_from - amaip->aligncoords[start_m];
            endoffset = amaip->lens[start_m] - offset - (amp->to_m - amp->real_from + 1);
            if (endoffset < 0 && (start_m+1) < amaip->numseg)
               retval = TRUE;
            else
            {
               retval = TRUE;
               amp->place = 1;
               amp->row_num = -1;
               amp->prev = -2;
            }
            j=0;
            found = FALSE;
            while (!found && j < amaip->numsaps)
            {
               if (amaip->starts[j+(amaip->numsaps)*start_m] >= 0)
                  found = TRUE;
               else if (amaip->starts[j+(amaip->numsaps)*start_m] == -3)
                  found = TRUE;
               else
                  j++;
            }
            if (amaip->starts[j+(amaip->numsaps)*start_m] == -3)
            {
               found = FALSE;
               j=0;
               while (!found && j < amaip->numsaps)
               {
                  if (amaip->starts[j+(amaip->numsaps)*(start_m-1)] >= 0)
                     found = TRUE;
                  else
                     j++;
               }
               amp->from_b = AlnMgrMapToBsqCoords(amaip->saps[j], amaip->starts[j+(amaip->numsaps)*(start_m-1)]+offset, NULL, NULL) + amaip->lens[start_m - 1];
               if (endoffset >=0)
                  amp->to_b = amp->from_b + amaip->lens[start_m] - 1 - offset  - endoffset;
               else
                  amp->to_b = amp->from_b + amaip->lens[start_m] - offset - 1;
               amp->gap = 0;
               amp->real_from += amp->to_b - amp->from_b + 1;
            } else if (j<amaip->numsaps)
            {
               amp->from_b = AlnMgrMapToBsqCoords(amaip->saps[j], amaip->starts[j+(amaip->numsaps)*start_m]+offset, NULL, NULL);
               if (amp->from_b >= 0)
               {
                  if (endoffset >=0)
                     amp->to_b = amp->from_b + amaip->lens[start_m] - 1 - offset  - endoffset;
                  else
                     amp->to_b = amp->from_b + amaip->lens[start_m] - offset - 1;
                  amp->gap = 0;
               } else
               { 
                  amp->from_b = amp->real_from;
                  amp->gap = 1;
                  if (endoffset >= 0)
                     amp->to_b = amp->from_b + amaip->lens[start_m] - 1 - offset  - endoffset;
                  else
                     amp->to_b = amp->from_b + amaip->lens[start_m] - offset - 1;
               }
               amp->real_from += amp->to_b - amp->from_b + 1;
            }
         } else
         {
            if (amp->prev != -2)
            {
               amp->prev += 1;
               start_m = amp->prev;
            } else
            {
               start_m = binary_search_on_uint4_list(amaip->aligncoords, amp->from_m, amaip->numseg);
               amp->real_from = amp->from_m;
               amp->prev = start_m;
            }
            if (amp->prev_sap == -2)
               amp->prev_sap=amaip->rowsource[amp->row_num-1]->which_saps[0];
            j = amp->prev_sap-1;
            amp->strand = AlnMgrGetNthStrand(amaip->saps[j], amaip->rowsource[amp->row_num-1]->num_in_sap[0]);
            offset = amp->real_from - amaip->aligncoords[start_m];
            endoffset = amaip->lens[start_m] - offset - (amp->to_m - amp->real_from + 1);
            if (endoffset <= 0 && (start_m + 1) < amaip->numseg)
               retval = TRUE;
            else
            {
               retval = TRUE;
               amp->place = 1;
               amp->prev = amp->prev_sap = -2;
            }
            if (amaip->starts[j+(amaip->numsaps)*start_m] < 0)
               amp->from_b = -1;
            else
               amp->from_b = AlnMgrMapRowCoords(amaip->saps[j], amaip->starts[j+(amaip->numsaps)*start_m]+offset, amaip->rowsource[amp->row_num-1]->num_in_sap[0], NULL);
            if (amp->from_b >= 0)
            {
               if (amp->strand != Seq_strand_minus)
               {
                  if (endoffset >=0)
                     amp->to_b = amp->from_b + amaip->lens[start_m] - offset - 1 -
endoffset;
                  else
                     amp->to_b = amp->from_b + amaip->lens[start_m] - offset -1;
               } else
               {
                  amp->to_b = amp->from_b;
                  if (endoffset >= 0)
                     amp->from_b = amp->to_b - amaip->lens[start_m] + 1 + endoffset
;
                  else
                     amp->from_b = amp->to_b - amaip->lens[start_m] + 1;
               }
               amp->gap = 0;
            } else
            {
               amp->from_b = amp->real_from;
               amp->gap = 1;
               if (endoffset >= 0)
                  amp->to_b = amp->from_b + amaip->lens[start_m] - offset - 1 - endoffset;
               else
                  amp->to_b = amp->from_b + amaip->lens[start_m] - offset - 1;
            }
            amp->real_from += amp->to_b - amp->from_b + 1;
            if (amp->real_from > amp->to_m)
            {
               retval = TRUE;
               amp->place = 1;
               amp->row_num = -1;
               amp->prev = -2;
            }
         }
      } else if (sap->type == SAT_MASTERSLAVE && amp->which_master)
      {
      } else if (sap->type == SAT_DIAGS && amp->which_master)
      {
      } else if (sap->type == SAT_MASTERSLAVE && amaip->mstype == AM_SEGMENTED_MASTERSLAVE && amp->which_master == NULL)
      {
         if (amp->place == 1)
            return retval;
         len = AlnMgrGetAlnLength(sap, FALSE);
         if (amp->to_m > len-1)
            return retval;
         if (amp->to_m < 0)
            amp->to_m = len-1;
         if (amp->row_num == -1)
         {
            if(!amp->which_bsq)
                return retval;
            else
            {
                amp->row_num = AlnMgrGetNForSip(sap,amp->which_bsq);
                if(amp->row_num == -1)
                    return retval;
            }
         }
         if (amp->prev == -2)
         {
            start_m = binary_search_on_uint4_list(amaip->aligncoords, amp->from_m, amaip->numseg);
            amp->real_from = amp->from_m;
            amp->prev = start_m;
         } else
            start_m = amp->prev;
         offset = amp->real_from - amaip->aligncoords[start_m];
         if (offset < 0)
            offset = 0;
         if (amaip->rowsource[amp->row_num-1]->which_saps[start_m] == 0)
         {
            len = amaip->lens[start_m];
            amp->from_b = amaip->aligncoords[start_m]+offset;
            amp->gap = 2;
            amp->strand = Seq_strand_unknown;
         } else
         {
            len = 0;
            amp->strand = AlnMgrGetNthStrand(amaip->saps[amaip->rowsource[amp->row_num-1]->which_saps[start_m]-1], amaip->rowsource[amp->row_num-1]->num_in_sap[start_m]);
            amp->from_b = AlnMgrMapSegmentCoords(amaip->saps[amaip->rowsource[amp->row_num-1]->which_saps[start_m]-1], offset, amaip->rowsource[amp->row_num-1]->num_in_sap[start_m], NULL, &len);
            if (amp->from_b == -1)
            {
               amp->from_b = amaip->aligncoords[start_m]+offset;
               amp->gap = 1;
            } else
               amp->gap = 0;
         }
         endoffset = amp->to_m - (amaip->aligncoords[start_m] + len + offset -1);
         if (endoffset <= 0)
         {
            amp->send_space = 1;
            amp->place = 1;
         } else if (len >= amaip->lens[start_m] - offset)
         {
            amp->prev++;
            amp->send_space = 1;
         } else
            amp->send_space = 0;
         if (endoffset <= 0)
            amp->to_b = amp->from_b + len + endoffset -1;
         else
         {
            amp->to_b = amp->from_b + len - 1;
            amp->real_from = amp->real_from + amp->to_b - amp->from_b + 1;
         }
         if (amp->strand == Seq_strand_minus && amp->gap == 0)
         {
            offset = amp->to_b - amp->from_b;
            amp->to_b = amp->from_b;
            amp->from_b = amp->to_b - offset;
         }
         retval = TRUE;
      }
   }
   return retval;
}

NLM_EXTERN Uint4 binary_search_on_uint4_list(Uint4Ptr list, Uint4 pos, Uint4 listlen)
{
   Uint4  L;
   Uint4  mid;
   Uint4  R;

   if (list == NULL || listlen == 0)
      return 0;
   L = 0;
   R = listlen - 1;
   while (L < R)
   {
      mid = (L+R)/2;
      if (list[mid + 1] <= pos)
      {
         L = mid + 1;
      } else
      {
         R = mid;
      }
   }
   return R;
}

NLM_EXTERN Int4 binary_search_on_uint2_list(Uint2Ptr list, Uint2 ele, Uint2 listlen)
{
   Uint2  L;
   Uint2  mid;
   Uint2  R;

   if (list == NULL || listlen == 0)
      return -1;
   L = 0;
   R = listlen - 1;
   while (L < R)
   {
      mid = (L+R)/2;
      if (ele <= list[mid])
      {
         R = mid;
      } else
      {
         L = mid+1;
      }
   }
   if (ele == list[R])
      return list[R];
   else
      return -1;
}

NLM_EXTERN Int4 binary_search_by_chunk(Int4Ptr list, Int4 ele, Int4 listlen, Int4 chunksize, Int4 offset)
{
   Int4  L;
   Int4  mid;
   Int4  R;

   if (list == NULL || listlen == 0)
      return -1;
   L = 0;
   R = (listlen/chunksize) - 1;
   while (L < R)
   {
      mid = (L+R)/2;
      if (ele <= list[(mid)*chunksize + offset] && list[(mid)*chunksize + offset] >= 0)
      {
         R = mid;
      } else
      {
         L = mid + 1;
      }
   }
   return R;
}

NLM_EXTERN Int4 binary_search_segment_array(SASeqDatPtr ssdp, Int4 pos, Int4 numseq, Int4 offset, DenseSegPtr dsp)
{
   Int4  L;
   Int4  mid;
   Int4  R;

   if (ssdp == NULL || numseq == 0)
      return -1;
   L = 0;
   R = ssdp->numsect - 1;
   while (L < R)
   {
      mid = (L+R)/2;
      if (pos <= (dsp->starts[(ssdp->sect[mid])*numseq + offset]))
      {
         R = mid;
      } else
      {
         L = mid+1;
      }
   }
   return (ssdp->sect[R]);
}

/************************************************************************
*
*  These are several utility functions which get needed data from the
*  indexes.  "N" refers to row number.
*
************************************************************************/
NLM_EXTERN Int4 AlnMgrGetAlnLength(SeqAlignPtr sap, Boolean fill_in)
{
   AMAlignIndexPtr  amaip;
   DenseSegPtr      dsp;
   Int4             i;
   Int4             length;
   SAIndexPtr       saip;

   if (!sap)
      return 0;
   i = AlnMgrCheckAlignForParent(sap);
   if (i<0)
   {
      return 0;
   } else if (i == AM_CHILD)
   {
      dsp = (DenseSegPtr)sap->segs;
      if (!dsp)
         return 0;
      saip = (SAIndexPtr)sap->saip;
      return ((saip->aligncoords[dsp->numseg-1])+dsp->lens[dsp->numseg-1]);
   } else if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)sap->saip;
      if (!amaip || amaip->mstype == AM_LITE)
         return 0;
      if (!amaip->saps)
      {
         if (!AlnMgrMakeFakeMultiple(sap))
            return 0;
      }
      if (fill_in)
      {
         if (sap->type == SAT_MASTERSLAVE && (amaip->mstype == AM_MASTERSLAVE || amaip->mstype == AM_NULL))
            return (amaip->lens[(amaip->numseg)-1] + amaip->aligncoords[amaip->numseg-1]);
         else if (sap->type == SAT_PARTIAL)
         {
            length = 0;
            for (i=0; i<(amaip->numsaps-1); i++)
            {
               length += AlnMgrGetMaxUnalignedLength(amaip->saps[i], amaip->saps[i+1]);
            }
            return (length + amaip->lens[(amaip->numseg)-1] + amaip->aligncoords[amaip->numseg-1]);
         }
      } else
      {
         if (amaip->mstype == AM_NULL)
            return 0;
         return (amaip->lens[(amaip->numseg)-1] + amaip->aligncoords[amaip->numseg-1]);
      }
   }
   return 0;
}

NLM_EXTERN Int4 AlnMgrGetNumSeqs(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   DenseSegPtr      dsp;
   Int4             i;

   if (!sap)
      return 0;
   i = AlnMgrCheckAlignForParent(sap);
   if (i<0)
      return 0;
   if (i == AM_CHILD)
   {
      dsp = (DenseSegPtr)sap->segs;
      if (!dsp)
         return 0;
      return (dsp->dim);
   } else if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)(sap->saip);
      if (!amaip || amaip->mstype == AM_LITE)
         return 0;
      return (amaip->numbsqs);
   }
   return 0;
}

NLM_EXTERN SeqIdPtr AlnMgrGetUniqueSeqs(SeqAlignPtr sap, Int4Ptr n)
{
   AMAlignIndexPtr  amaip;
   Int4             c;
   DenseSegPtr      dsp;
   Boolean          found;
   Int4             i;
   Int4             m;
   SeqIdPtr         sip;
   SeqIdPtr         sip_head;
   SeqIdPtr         sip_prev;
   SeqIdPtr         sip_tmp;

   if (sap == NULL)
      return 0;
   i = AlnMgrCheckAlignForParent(sap);
   if (i<0)
      return 0;
   sip_head = sip_prev = NULL;
   if (i == AM_CHILD)
   {
      dsp = (DenseSegPtr)sap->segs;
      if (dsp == NULL)
         return 0;
      sip = dsp->ids;
      m = 0;
      while (sip)
      {
         sip_tmp = sip_head;
         found = FALSE;
         while (!found && sip_tmp != NULL)
         {
            if (SAM_OrderSeqID(sip, sip_tmp) == 0)
               found = TRUE;
            sip_tmp = sip_tmp->next;
         }
         if (!found)
         {
            m++;
            if (sip_head == NULL)
            {
               sip_head = sip_prev = SeqIdDup(sip);
            } else
            {
               sip_prev->next = SeqIdDup(sip);
               sip_prev = sip_prev->next;
            }
         }
         sip = sip->next;
      }
      if (n)
         *n = m;
      return sip_head;
   } else if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)(sap->saip);
      if (amaip == NULL || amaip->mstype == AM_LITE)
         return 0;
      m = 0;
      if (amaip->alnsaps == 1)
      {
         return (AlnMgrGetUniqueSeqs((SeqAlignPtr)sap->segs, n));
      }
      for (c=0; c<amaip->numrows; c++)
      {
         sip = amaip->rowsource[c]->id;
         sip_tmp = sip_head;
         found = FALSE;
         while (!found && sip_tmp != NULL)
         {
            if (SAM_OrderSeqID(sip, sip_tmp) == 0)
               found = TRUE;
            sip_tmp = sip_tmp->next;
         }
         if (!found)
         {
            m++;
            if (sip_head == NULL)
            {
               sip_head = sip_prev = SeqIdDup(sip);
            } else
            {
               sip_prev->next = SeqIdDup(sip);
               sip_prev = sip_prev->next;
            }
         }
      }
      if (n)
         *n = m;
      return sip_head;
   }
   return NULL;
}

NLM_EXTERN SeqIdPtr AlnMgrGetNthSeqIdPtr(SeqAlignPtr sap, Int4 n)
{
   AMAlignIndexPtr  amaip;
   Int4             count;
   DenseSegPtr      dsp;
   Int4             i;
   SeqIdPtr         sip;

   if (!sap)
      return NULL;
   i = AlnMgrCheckAlignForParent(sap);
   if (i<0)
      return NULL;
   else if (i == AM_CHILD)
   {
      dsp = (DenseSegPtr)sap->segs;
      if (!dsp)
         return NULL;
      sip = dsp->ids;
      count = 0;
      while (sip)
      {
         count++;
         if (count == n)
            return (SeqIdDup(sip));
         sip = sip->next;
      }
   } else if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)(sap->saip);
      if (amaip->mstype == AM_LITE)
         return NULL;
      if (n <= amaip->numrows)
      {
         return (SeqIdDup(amaip->rowsource[n-1]->id));
      } else
      {
         if (AlnMgrIsSAPNULL(sap))
         {
            sip = amaip->ids;
            count = 1;
            while (sip != NULL && count < n)
            {
               sip = sip->next;
               count++;
            }
            return SeqIdDup(sip);
         } else
            return NULL;
      }
   }
   return NULL;
}

/* (RANGE) */
NLM_EXTERN void AlnMgrGetNthSeqRangeInSA(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   AMAlignDatPtr    amadp;
   AMAlignIndexPtr  amaip;
   Uint2            beg;
   Int4             bsq;
   DenseSegPtr      dsp;
   Uint2            end;
   Int4             i;
   Int4             j;
   RowSourcePtr     rsp;
   SAIndexPtr       saip;
   SeqIdPtr         sip;
   Uint2            strand;
   Int4             tmp_beg;
   Int4             tmp_end;
   Int4             tmp_start;
   Int4             tmp_stop;

   if (!sap)
      return;
   i = AlnMgrCheckAlignForParent(sap);
   if (i < 0)
   {
      return;
   } else if (i == AM_CHILD)
   {
      if (n<1)
         return;
      saip = (SAIndexPtr)(sap->saip);
      if(!saip)
          return;
      bsq = n-1;
      dsp = (DenseSegPtr)sap->segs;
      if (n > dsp->dim)
         return;
      if (!dsp)
         return;
      strand = dsp->strands[bsq];
      if (strand != Seq_strand_minus)
      {
         if (saip->ssdp[bsq]->numsect == 0)
         {
            if (start)
               *start = -1;
            if (stop)
               *stop = -1;
            return;
         }
         beg = saip->ssdp[bsq]->sect[0];
         if (start)
            *start = dsp->starts[beg*(dsp->dim)+bsq];
         end = saip->ssdp[bsq]->sect[(saip->ssdp[bsq]->numsect)-1];
         if (stop)
            *stop = (dsp->starts[end*(dsp->dim)+bsq] + dsp->lens[end] - 1);
         return;
      } else
      {
         if (saip->ssdp[bsq]->numsect == 0)
         {
            if (start)
               *start = -1;
            if (stop)
               *stop = -1;
            return;
         }
         beg = saip->ssdp[bsq]->sect[(saip->ssdp[bsq]->numsect)-1];
         if (start)
            *start = dsp->starts[beg*(dsp->dim)+bsq];
         end = saip->ssdp[bsq]->sect[0];
         if (stop)
            *stop = (dsp->starts[end*(dsp->dim)+bsq] + dsp->lens[end] - 1);
         return;
      }
   } else if (i == AM_PARENT)
   {
      if (n<1)
         return;
      bsq = n-1;
      amaip = (AMAlignIndexPtr)(sap->saip);
      if (amaip->mstype == AM_LITE)
         return;
      if (amaip->numseg == 0)
      {
         if (start)
            *start = -1;
         if (stop)
            *stop = -1;
         return;
      }
      if (amaip->rowsource == NULL)
      {
         amadp = amaip->amadp[bsq];
         sip = amaip->ids;
         for (j = 0; j<bsq; j++)
         {
            sip = sip->next;
            if (sip == NULL)
               return;
         }
         for (j = 0; j<(amadp->numsaps); j++)
         {
            tmp_start = tmp_stop = 0;
            AlnMgrGetNthSeqRangeInSA(amadp->saps[j], AlnMgrGetNForSip(amadp->saps[j], sip), &tmp_start, &tmp_stop);
            if (j == 0)
            {
               tmp_beg = tmp_start;
               tmp_end = tmp_stop;
            } else
            {
               if (tmp_start < tmp_beg)
                  tmp_beg = tmp_start;
               if (tmp_stop > tmp_end)
                  tmp_end = tmp_stop;
            }
         }
         if (start)
            *start = tmp_beg;
         if (stop)
            *stop = tmp_end;
         return;
      } else
      {
         sip = amaip->ids;
         if (n > amaip->numrows)
            return;
         rsp = (RowSourcePtr)amaip->rowsource[n-1];
         for (j=0; j<(rsp->numsaps); j++)
         {
            tmp_start = tmp_stop = 0;
            if(rsp->which_saps[j] == 0)
            {
                if (start)
                    *start = 0;
                if (stop)
                    *stop = 0;
                return;
            }
            AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[j]-1], rsp->num_in_sap[j], &tmp_start, &tmp_stop);

            if (j==0)
            {
               tmp_beg = tmp_start;
               tmp_end = tmp_stop;
            } else
            {
               if (tmp_start < tmp_beg)
                  tmp_beg = tmp_start;
               if (tmp_stop > tmp_end)
                  tmp_end = tmp_stop;
            }
         }
         if (start)
            *start = tmp_beg;
         if (stop)
            *stop = tmp_end;
         return;
      }
   }
   return;
}

NLM_EXTERN Int4 AlnMgrGetNumSegments(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   DenseSegPtr      dsp;
   Int4             i;

   if (sap == NULL)
      return -1;
   i = AlnMgrCheckAlignForParent(sap);
   if (i == AM_CHILD)
   {
      dsp = (DenseSegPtr)(sap->segs);
      return (dsp->numseg);
   } else if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)(sap->saip);
      return (amaip->numseg);
   } else
      return -1;
}

/***************************************************************************
*
*  AlnMgrGetNumAlnBlocks returns the number of separate aligned regions
*  in the seqalign.  A return value of -1 indicates an error; a return
*  value of 0 indicates a NULL alignment (only one unaligned region and
*  no aligned regions.
*
***************************************************************************/
NLM_EXTERN Int4 AlnMgrGetNumAlnBlocks(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;

   if (sap == NULL || sap->saip == NULL)
      return -1;
   if (sap->saip->indextype == INDEX_SEGS)
      return 1;
   else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndexPtr)(sap->saip);
      if (amaip->mstype == AM_LITE || amaip->mstype == AM_NEATINDEX)
         return -1;
      else if (amaip->mstype == AM_NULL)
         return 0;
      else if (amaip->mstype == AM_MASTERSLAVE)
         return 1;
      else if (amaip->mstype == AM_SEGMENTED_MASTERSLAVE)
      {
         if (amaip->rowsource == NULL || amaip->master < 1 || amaip->master > amaip->numrows)
            return -1;
         return (amaip->rowsource[amaip->master-1]->numsaps);
      } else if (sap->type == SAT_PARTIAL)
         return amaip->alnsaps;
   } else
      return -1;
   return -1;
}

NLM_EXTERN Boolean AlnMgrGetNthBlockRange(SeqAlignPtr sap, Int4 n, Int4Ptr from, Int4Ptr to)
{
   AMAlignIndexPtr  amaip;
   Int4             i;
   Int4             len;

   if (sap == NULL || sap->saip == NULL)
      return FALSE;
   if (sap->saip->indextype == INDEX_SEGS)
   {
      len = AlnMgrGetAlnLength(sap, FALSE);
      if (from)
         *from = 0;
      if (to)
         *to = len - 1;
      return TRUE;
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      i = AlnMgrGetNumAlnBlocks(sap);
      if (n > i || i < 1)
         return FALSE;
      amaip = (AMAlignIndexPtr)(sap->saip);
      if (i == 1)
      {
         len = AlnMgrGetAlnLength(sap, FALSE);
         if (from)
            *from = 0;
         if (to)
            *to = len - 1;
         return TRUE;
      } else
         return (AlnMgrGetNthSegmentRange(sap, n, from, to));
   } else
      return FALSE;
}


/***************************************************************************
*
*  AlnMgrGetNthAlignedSegInNthRow is similar to AlnMgrGetNextAlignBit,
*  but it takes an extra argument -- the number (1-based) of the segment
*  for which you want the alignment.  Fill in the AlnMsg structure as for
*  AlnMgrGetNextAlignBit, but leave out the from_m and to_m; and as usual,
*  don't modify the AlnMsg structure in between calls, and call AlnMsgReNew
*  for a new segment.  The from_b and to_b fields will work as in 
*  AlnMgrGetNextAlnBit -- if there is a gap, these are alignment coordinates;
*  otherwise they're sequence coordinates.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgrGetNthAlignedSegInNthRow(SeqAlignPtr sap, AlnMsgPtr amp, Int4 segnum)
{
   Int4     n;
   Boolean  retval;

   if (sap == NULL || amp == NULL)
      return FALSE;
   if (amp->flag == FALSE)
   {
      n = AlnMgrGetNumSegments(sap);
      if (segnum > n)
         return FALSE;
      if (!AlnMgrGetNthSegmentRange(sap, segnum, &amp->from_m, &amp->to_m))
         return FALSE;
      amp->flag = TRUE;
   }
   retval = AlnMgrGetNextAlnBit(sap, amp);
   if (retval == FALSE)
      amp->flag = FALSE;
   return retval;
}


NLM_EXTERN Boolean AlnMgrGetNthSegmentRange(SeqAlignPtr sap, Int4 n, Int4Ptr from, Int4Ptr to)
{
   AMAlignIndexPtr  amaip;
   Int4             i;
   Boolean          last;
   Int4             num;
   SAIndexPtr       saip;

   if (sap == NULL)
      return FALSE;
   i = AlnMgrCheckAlignForParent(sap);
   num = AlnMgrGetNumSegments(sap);
   if (n > num)
      return FALSE;
   last = FALSE;
   if (n == num)
      last = TRUE;
   if (i == AM_CHILD)
   {
      saip = (SAIndexPtr)(sap->saip);
      if (!last)
      {
         if (from)
            *from = saip->aligncoords[n-1];
         if (to)
            *to = saip->aligncoords[n] - 1;
         return TRUE;
      } else
      {
         if (from)
            *from = saip->aligncoords[n-1];
         if (to)
            *to = AlnMgrGetAlnLength(sap, FALSE);
         return TRUE;
      }
   } else if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)(sap->saip);
      if (!last)
      {
         if (from)
            *from = amaip->aligncoords[n-1];
         if (to)
            *to = amaip->aligncoords[n] - 1;
         return TRUE;
      } else
      {
         if (from)
            *from = amaip->aligncoords[n-1];
         if (to)
            *to = amaip->aligncoords[n-1] + amaip->lens[n-1] - 1;
         return TRUE;
      }
   } else
      return FALSE;
}


/********************************************************************************
*
*  AlnMgrGetNextNthSeqRange is called recursively to return the lengths of
*  all aligned and all internal unaligned regions of any row in a seqalign.
*  If there is an error, or if the function is called past the last block,
*  the function returns FALSE.  Set where to point to an allocated integer
*  equal to 0 on the first call and don't change it during the loop.  Set
*  the boolean unaligned to FALSE to get only the aligned regions, and TRUE to
*  get the aligned regions plus all internal unaligned regions.  For unaligned
*  regions, *is_aligned will be FALSE.
*
********************************************************************************/
NLM_EXTERN Boolean AlnMgrGetNextNthSeqRange(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop, Int4Ptr where, BoolPtr is_aligned, Boolean unaligned)
{
   if (sap == NULL || n <= 0)
      return FALSE;
   if (sap->saip == NULL)
      return FALSE;
   if (sap->saip->indextype == INDEX_PARENT && sap->type == SAT_PARTIAL)
   {
      return (am_get_nth_range_for_partial(sap, n, start, stop, where, is_aligned, unaligned));
   } else
   {
      if (*where == 0)
      {
         AlnMgrGetNthSeqRangeInSA(sap, n, start, stop);
         *where = 1;
         return TRUE;
      } else
         return FALSE;
   }
}

static Boolean am_get_nth_range_for_partial(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop, Int4Ptr where, BoolPtr is_aligned, Boolean unaligned)
{
   AMAlignIndexPtr  amaip;
   RowSourcePtr     rsp;
   Uint2            strand;
   Int4             tmp_start;
   Int4             tmp_stop;
   Int4             tmp_where;

   amaip = (AMAlignIndexPtr)sap->saip;
   if (amaip->mstype == AM_LITE)
      return FALSE;
   rsp = amaip->rowsource[n-1];
   tmp_where = *where;
   if (tmp_where >= 0)
   {
      if (tmp_where >= rsp->numsaps)
         return FALSE;
      if (is_aligned)
         *is_aligned = TRUE;
      AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[tmp_where]-1], rsp->num_in_sap[tmp_where], start, stop);
      if (unaligned && (sap->type == SAT_PARTIAL || (sap->type == SAT_MASTERSLAVE && amaip->mstype == AM_SEGMENTED_MASTERSLAVE)))
         tmp_where = -(tmp_where+1);
      else
         tmp_where += 1;
   } else if (tmp_where < 0 && unaligned == TRUE)
   {
      if (-tmp_where >= rsp->numsaps)
         return FALSE;
      if (is_aligned)
         *is_aligned = FALSE;
      strand = AlnMgrGetNthStrand(amaip->saps[rsp->which_saps[(-tmp_where)]-1], n);
      tmp_start = tmp_stop = 0;
      if (start)
      { 
         if (strand == Seq_strand_minus)
         {
            AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[(-tmp_where)]-1], rsp->num_in_sap[(-tmp_where)], &tmp_start, NULL);
            *start = tmp_start + 1;
         } else
         {
             AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[(-tmp_where)-1]-1], rsp->num_in_sap[(-tmp_where)-1], NULL, &tmp_start);
             *start = tmp_start + 1;
         }
      }
      if (stop)
      {
         if (strand == Seq_strand_minus)
         {
            AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[(-tmp_where)-1]-1], rsp->num_in_sap[(-tmp_where)-1], NULL, &tmp_stop);
            *stop = tmp_stop - 1;
         } else
         {
            AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[-tmp_where]-1], rsp->num_in_sap[-tmp_where], &tmp_stop, NULL);
            *stop = tmp_stop - 1;
         }
      }
      if (tmp_start + 1 > tmp_stop - 1)
      {
         if (start)
            *start = -1;
         if (stop)
            *stop = -1;
      }
      tmp_where = -tmp_where;
   }
   *where = tmp_where;
   return TRUE;
}

/********************************************************************************
*
*  AlnMgrGetNthRowTail retrieves the blocks of sequence on either end of the
*  alignment, by row.  which_tail is LEFT_TAIL to retrieve the ends which come
*  before alignment coordinate 0, and RIGHT_TAIL to retrieve the other ends.
*  The function returns TRUE if successful, FALSE for an error.
*
********************************************************************************/
NLM_EXTERN Boolean AlnMgrGetNthRowTail(SeqAlignPtr sap, Int4 n, Uint1 which_tail, Int4Ptr start, Int4Ptr stop, Uint1Ptr strand)
{
   BioseqPtr  bsp;
   SeqIdPtr   sip = NULL;
   Int4       tmp_start;
   Int4       tmp_stop;
   Uint1      tmp_strand;

   if (sap == NULL || n < 1 || sap->segs == NULL)
      return FALSE;
   tmp_start = tmp_stop = -1;
   AlnMgrGetNthSeqRangeInSA(sap, n, &tmp_start, &tmp_stop);
   if (tmp_start == -1 || tmp_stop == -1)
      return FALSE;
   tmp_strand = AlnMgrGetNthStrand(sap, n);
   if (which_tail == LEFT_TAIL)
   {
      if (tmp_strand == Seq_strand_minus)
      {
         sip = AlnMgrGetNthSeqIdPtr(sap, n);
         bsp = BioseqLockById(sip);
         if (tmp_stop == bsp->length-1 || stop == NULL)
         {
            if (start)
               *start = -1;
            if (stop)
               *stop = -1;
         } else
         {
            if (bsp == NULL)
               return FALSE;
            if (start)
               *start = tmp_stop-1;
            if (stop)
               *stop = bsp->length-1;
         }
         BioseqUnlock(bsp);
         if (strand)
            *strand = tmp_strand;
      } else
      {
         if (tmp_start >= 1)
         {
            if (start)
               *start = 0;
            if (stop)
               *stop = tmp_start - 1;
         } else
         {
            if (start)
               *start = -1;
            if (stop)
               *stop = -1;
         }
         if (strand)
            *strand = tmp_strand;
      }
   } else if (which_tail == RIGHT_TAIL)
   {
      if (tmp_strand == Seq_strand_minus)
      {
         if (tmp_start >= 1)
         {
            if (start)
               *start = 0;
            if (stop)
               *stop = tmp_start - 1;
         } else
         {
            if (start)
               *start = -1;
            if (stop)
               *stop = -1;
         }
         if (strand)
            *strand = tmp_strand;
      } else
      {
         sip = AlnMgrGetNthSeqIdPtr(sap, n);
         bsp = BioseqLockById(sip);
         if (bsp == NULL)
            return FALSE;
         if (bsp->length-1 == tmp_stop)
         {
            if (start)
               *start = -1;
            if (stop)
               *stop = -1;
         } else
         {
            if (start)
               *start = tmp_stop + 1;
            if (stop)
               *stop = bsp->length-1;
         }
         if (strand)
            *strand = tmp_strand;
         BioseqUnlock(bsp);
      }
   } else
      return FALSE;
   SeqIdFree(sip);
   return TRUE;
}

NLM_EXTERN Int4 AlnMgrGetMaxTailLength (SeqAlignPtr sap, Uint1 which_tail)
{
   Int4  i;
   Int4  max;
   Int4  n;
   Int4  start;
   Int4  stop;

   if (sap == NULL)
      return -1;
   n = AlnMgrGetNumRows(sap);
   max = -1;
   for (i=0; i<n; i++)
   {
      if (!AlnMgrGetNthRowTail (sap, i+1, which_tail, &start, &stop, NULL))
         return -1;
      if (stop - start + 1 > max)
         max = stop - start + 1;
   }
   return max;
}

NLM_EXTERN Boolean AlnMgrGetNthUnalignedForNthRow(SeqAlignPtr sap, Int4 unaligned, Int4 row, Int4Ptr start, Int4Ptr stop)
{
   AMAlignIndexPtr  amaip;
   Int4             beg;
   BioseqPtr        bsp;
   Int4             end;
   Boolean          go = TRUE;
   Int4             i;
   Int4             j;
   RowSourcePtr     rsp;
   SeqIdPtr         sip;
   Uint2            strand;
   Int4             tmp_start;
   Int4             tmp_stop;

   i = AlnMgrCheckAlignForParent(sap);
   if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)(sap->saip);
      if (amaip->numseg > 0 && row > amaip->numrows)
         return FALSE;
      if (sap->type == SAT_PARTIAL || (sap->type == SAT_MASTERSLAVE && (amaip->mstype == AM_SEGMENTED_MASTERSLAVE || amaip->mstype == AM_NULL)))
      {
         if (amaip->numseg == 0)
         {
            if (row > amaip->numbsqs)
               return FALSE;
            sip = amaip->ids;
            for (j=1; j<row; j++)
            {
               sip = sip->next;
            }
            bsp = BioseqLockById(sip);
            if (bsp == NULL)
               return FALSE;
            if (start != NULL)
               *start = 0;
            if (stop != NULL)
               *stop = bsp->length-1;
            BioseqUnlock(bsp);
            return TRUE;
         } else if (unaligned > amaip->numseg - 1)
            return FALSE;
         tmp_start = tmp_stop = 0;
         rsp = amaip->rowsource[row-1];
         if (rsp->which_saps[unaligned-1] == 0 || rsp->which_saps[unaligned] == 0)
         {
            beg = unaligned-1;
            while (rsp->which_saps[beg] == 0 && beg >= 0)
            {
               beg--;
            }
            if (beg<0)
               go = FALSE;
            end = unaligned;
            if (rsp->which_saps[end] == 0)
               go = FALSE;
         } else
         {
            beg = unaligned-1;
            end = unaligned;
         }
         if (go)
            strand = AlnMgrGetNthStrand(amaip->saps[rsp->which_saps[end]-1], row);
         if (start && go)
         {
            if (strand == Seq_strand_minus)
            {
               AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[end]-1], rsp->num_in_sap[end], NULL, &tmp_start);
               *start = tmp_start + 1;
            } else
            {
               AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[beg]-1], rsp->num_in_sap[beg], NULL, &tmp_start);
               *start = tmp_start + 1;
            }
         }
         if (stop && go)
         {
            if (strand == Seq_strand_minus)
            {
               AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[beg]-1], rsp->num_in_sap[beg], &tmp_stop, NULL);
               *stop = tmp_stop - 1;
            } else
            {
               AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[end]-1], rsp->num_in_sap[end], &tmp_stop, NULL);
               *stop = tmp_stop - 1;
            }
         }
         if (tmp_start + 1 > tmp_stop - 1)
         {
            if (start)
               *start = -1;
            if (stop)
               *stop = -1;
         }
         return TRUE;
      } else
         return FALSE;
   } else
      return FALSE;
}


NLM_EXTERN Uint1 AlnMgrGetStrand(SeqAlignPtr sap, SeqIdPtr sip)
{
   Int4  i;

   i = AlnMgrGetNForSip(sap, sip);
   return (AlnMgrGetNthStrand(sap, i));
}

NLM_EXTERN Uint1 AlnMgrGetNthStrand(SeqAlignPtr sap, Int4 n)
{
   AMAlignIndexPtr  amaip;
   Int4             c;
   DenseSegPtr      dsp;
   Int4             m;
   SeqAlignPtr      salp;

   if (!sap || n < 1 || sap->segs == NULL)
      return 0;
   if (sap->segtype != SAS_DENSEG)
   {
      if (sap->saip == NULL)
         return 0;
      amaip = (AMAlignIndexPtr)sap->saip;
      if (n > amaip->numrows)
         return 0;
      c = 0;
      while (amaip->rowsource[n-1]->which_saps[c] == 0)
      {
         c++;
         if (c >= amaip->alnsaps)
            return (Seq_strand_unknown);
      }
      salp = amaip->saps[amaip->rowsource[n-1]->which_saps[c]-1];
      dsp = (DenseSegPtr)salp->segs;
      m = amaip->rowsource[n-1]->num_in_sap[0];
      if (m > dsp->dim)
         return 0;
      return (dsp->strands[m-1]);
   } else
   {
      dsp = (DenseSegPtr)sap->segs;
      if (!dsp)
         return 0;
      if (n==0)
         return 0;
      if (dsp->strands)
         return (dsp->strands[n-1]);
      else
         return (Seq_strand_plus);
   }
}

NLM_EXTERN Int4 AlnMgrGetNForSip(SeqAlignPtr sap, SeqIdPtr sip)
{
   AMAlignIndexPtr  amaip;
   DenseSegPtr      dsp;
   Int4             i;
   Int4             n;
   SeqIdPtr         sip_tmp;

   i = AlnMgrCheckAlignForParent(sap);
   if (i<0)
      return -1;
   if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)(sap->saip);
      sip_tmp = amaip->ids;
      n = 0;
      while (sip_tmp)
      {
         n++;
         if (SeqIdComp(sip_tmp, sip) == SIC_YES)
            return n;
         sip_tmp = sip_tmp->next;
      }
   } else if (i == AM_CHILD)
   {
      dsp = (DenseSegPtr)(sap->segs);
      sip_tmp = dsp->ids;
      n = 0;
      while (sip_tmp)
      {
         n++;
         if (SeqIdComp(sip_tmp, sip) == SIC_YES)
            return n;
         sip_tmp = sip_tmp->next;
      }
   }
   return -1;
}

NLM_EXTERN Int4 AlnMgrGetNForSap(AMAlignIndexPtr amaip, SeqAlignPtr sap)
{
   Int4  i;

   if (sap == NULL || amaip == NULL)
      return -1;
   if (sap->saip->indextype != INDEX_SEGS)
      return -1;
   i = 0;
   while (i<amaip->alnsaps)
   {
      if (amaip->saps[i] == sap)
         return (i+1);
      i++;
   }
   return -1;
}


/********************************************************************************
*
*  AlnMgrGetAllNForSip is called in a while loop to return all the rows that a
*  seqid appears in in a given seqalign.  Use n = 0 to start, and then on
*  return, if the return is TRUE, n will be the row number of the next row
*  that the seqid appears in.  If the return is FALSE, either there was an
*  error or there are no (more) rows containing that seqid.
*
********************************************************************************/
NLM_EXTERN Boolean AlnMgrGetAllNForSip(SeqAlignPtr sap, SeqIdPtr sip, Int4Ptr n)
{
   AMAlignIndexPtr  amaip;
   DenseSegPtr      dsp;
   Int4             i;
   SeqIdPtr         sip_tmp;

   if (sap == NULL || sip == NULL || n == NULL)
      return FALSE;
   if (sap->saip == NULL)
      return FALSE;
   if (sap->saip->indextype == INDEX_SEGS)
   {
      i = 1;
      dsp = (DenseSegPtr)sap->segs;
      sip_tmp = dsp->ids;
      while (i <= *n)
      {
         sip_tmp = sip_tmp->next;
         i++;
      }
      while (sip_tmp)
      {
         if (SeqIdComp(sip_tmp, sip) == SIC_YES)
         {
            *n = i;
            return TRUE;
         }
         i++;
         sip_tmp = sip_tmp->next;
      }
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndexPtr)sap->saip;
      if (amaip->mstype == AM_LITE)
         return FALSE;
      i = *n + 1;
      while (i <= amaip->numrows)
      {
         if (SeqIdComp(amaip->rowsource[i-1]->id, sip) == SIC_YES)
         {
            *n = i;
            return TRUE;
         }
         i++;
      }
   }
   return FALSE;
}

NLM_EXTERN Int4 AlnMgrGetSapForSip(AMAlignIndexPtr amaip, SeqIdPtr sip, Int4 which)
{
   Int4  i;
   Int4  j;
   Int4  n;

   i = 0;
   for (n=0; n<(amaip->numsaps); n++)
   {
      j = AlnMgrGetNForSip(amaip->saps[n], sip);
      if (j != -1)
      {
         if (i==which)
            return n;
         else
            i++;
      }
   }
   return -1;
}

/********************************************************************************
*
*  AlnMgrMapToBsqCoords returns the bioseq coordinate for an alignment
*  position.  If master is NULL, the alignment position is taken to be from
*  a flattened alignment; otherwise, the function returns the corresponding
*  position in the given master.
*
********************************************************************************/

NLM_EXTERN Int4 AlnMgrMapToBsqCoords(SeqAlignPtr sap, Uint4 pos, SeqIdPtr sip, SeqIdPtr master)
{
   DenseSegPtr  dsp;
   Int4         n;
   Int4         offset;
   SAIndexPtr   saip;
   Int4         start;

   if (!sap)
      return -1;
   if (sap->segtype == SAS_DENSEG)
   {
      saip = (SAIndexPtr)(sap->saip);
      dsp = (DenseSegPtr)(sap->segs);
      if (sip == NULL)
         n = saip->master;
      else
         n = AlnMgrGetNForSip(sap, sip);
      if (!master)
      {
         start = binary_search_on_uint4_list(saip->aligncoords, pos, dsp->numseg);
         offset = pos - saip->aligncoords[start];
         if (dsp->starts[(dsp->dim*start) + n - 1] == -1)
            return -1;
         else
            if (dsp->strands[(dsp->dim*start) + n - 1] != Seq_strand_minus)
               return (dsp->starts[(dsp->dim*start) + n - 1] + offset);
            else
               return (dsp->starts[(dsp->dim*start) + n - 1] + dsp->lens[start] - 1 - offset);
      } else
      {
      }
   } else if (sap->segtype == SAS_DISC)
   {
       SeqAlignPtr salp;
       salp = (SeqAlignPtr)sap->segs;
       if(salp->next==NULL)
           return AlnMgrMapToBsqCoords(salp, pos, sip, master);
   }
   return -1;
}

static Int4 AlnMgrMapSegmentCoords(SeqAlignPtr sap, Uint4 pos, Int4 row, SeqIdPtr master, Int4Ptr len)
{
   DenseSegPtr      dsp;
   Int4             offset;
   SAIndexPtr       saip;
   Int4             start;

   if (sap == NULL || row < 0 || len == NULL)
      return -1;
   if (sap->saip == NULL)
      return -1;
   if (sap->saip->indextype == INDEX_SEGS)
   {
      saip = (SAIndexPtr)sap->saip;
      dsp = (DenseSegPtr)sap->segs;
      if (master == NULL)
      {
         start = binary_search_on_uint4_list(saip->aligncoords, pos, dsp->numseg);
         offset = pos - saip->aligncoords[start];
         *len = dsp->lens[start]-offset;
         if (dsp->starts[(dsp->dim*start) + row - 1] == -1)
            return -1;
         else
            if (dsp->strands[(dsp->dim*start) + row - 1] != Seq_strand_minus)
               return (dsp->starts[(dsp->dim*start) + row - 1] + offset);
            else
               return (dsp->starts[(dsp->dim*start) + row - 1] + dsp->lens[start] - 1 - offset);
      } else
      {
      }
   }
   return -1;
}


/********************************************************************************
*
*  AlnMgrMapRowCoords maps a position in a given row to the bioseq coordinate
*  of that row.  If master is NULL, the alignment is taken to be flattened; 
*  otherwise it is an alignment according to that master (this will change the
*  correspondence between row coordinates and bioseq coordinates).  The return
*  value will be either a positive bioseq coordinate, or -1 if the bioseq is
*  gapped at that row position.
*
********************************************************************************/
NLM_EXTERN Int4 AlnMgrMapRowCoords(SeqAlignPtr sap, Uint4 pos, Int4 row, SeqIdPtr master)
{
   AMAlignIndexPtr  amaip;
   AlnMsg           amp;
   DenseSegPtr      dsp;
   Boolean          more;
   Int4             offset;
   SAIndexPtr       saip;
   Int4             start;

   if (sap == NULL || row < 0)
      return -1;
   if (sap->saip == NULL)
      return -1;
   if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndexPtr) sap->saip;
      if (row > amaip->numrows)
         return -1;

      AlnMsgReNew(&amp);
      amp.row_num = row;
      amp.which_master = master;
      amp.from_m = pos;
      amp.to_m = pos;
      more = AlnMgrGetNextAlnBit(sap, &amp);
      if (more && amp.gap == 0)
         start = amp.from_b;
      else
         start = -1;
      return start;
   } else if (sap->saip->indextype == INDEX_SEGS)
   {
      saip = (SAIndexPtr)sap->saip;
      dsp = (DenseSegPtr)sap->segs;
      if (master == NULL)
      {
         start = binary_search_on_uint4_list(saip->aligncoords, pos, dsp->numseg);
         offset = pos - saip->aligncoords[start];
         if (dsp->starts[(dsp->dim*start) + row - 1] == -1)
            return -1;
         else
            if (dsp->strands[(dsp->dim*start) + row - 1] != Seq_strand_minus)
               return (dsp->starts[(dsp->dim*start) + row - 1] + offset);
            else
               return (dsp->starts[(dsp->dim*start) + row - 1] + dsp->lens[start] - 1 - offset);
      } else
      {
      }
   }
   return -1;
}


/********************************************************************************
*
*  AlnMgrMapBioseqToSeqAlign takes a position in bioseq coordinates in a
*  row and maps it to seqalign coordinates, using the given master as
*  the alignment master (if master is NULL the alignment is flat).  A
*  return value of -1 indicates an error; a return value of -2 indicates
*  that the given bioseq coordinates are not contained in the alignment
*  specified.
*
********************************************************************************/
NLM_EXTERN Int4 AlnMgrMapBioseqToSeqAlign(SeqAlignPtr sap, Int4 pos, Int4 row_num, SeqIdPtr master)
{
   Int4             start, stop;

   if (sap == NULL || row_num < 0)
      return -1;
   AlnMgrGetNthSeqRangeInSA(sap, row_num, &start, &stop);
   if (pos < start || pos > stop)
      return -2;
   return AlnMgrMapBioseqToSeqAlignEx(sap, pos, row_num, master, NULL);
}

NLM_EXTERN Int4 AlnMgrMapBioseqToSeqAlignEx(SeqAlignPtr sap, Int4 pos, Int4 row_num, SeqIdPtr master,
                                                Int4 *oldj)
{
   AMAlignIndexPtr  amaip;
   Boolean          done;
   DenseSegPtr      dsp;
   Int4             i;
   Int4             j;
   Int4             k;
   Int2             L;
   Int4             m;
   Int4             mid;
   Uint1            n;
   Int4             offset;
   Int2             R;
   SAIndexPtr       saip;
   Int4             seg;
   SASeqDatPtr      ssdp;

   if (sap == NULL || row_num < 0)
      return -1;
   i = AlnMgrCheckAlignForParent(sap);
   if (i == AM_CHILD)
   {
      if (master == NULL)
      {
         saip = (SAIndexPtr)sap->saip;
         ssdp = saip->ssdp[row_num-1];
         if (ssdp == NULL)
            return -1;
         dsp = (DenseSegPtr)sap->segs;
         L = 0;
         R = ssdp->numsect - 1;
         n = AlnMgrGetNthStrand(sap, row_num);
         if (n != Seq_strand_minus)
         {
            while (L < R)
            {
               mid = (L + R)/2;
               if (dsp->starts[dsp->dim*ssdp->sect[mid+1]+row_num-1] <= pos)
                  L = mid+1;
               else
                  R = mid;
            }
         } else
         {
            while (L < R)
            {
               mid = (L + R)/2;
               if (dsp->starts[dsp->dim*ssdp->sect[mid]+row_num-1] > pos)
                  L = mid+1;
               else
                  R = mid;
            }
         }
         seg = L;
         offset = pos - dsp->starts[dsp->dim*ssdp->sect[seg]+row_num-1];
         if (n!=Seq_strand_minus)
            return (saip->aligncoords[ssdp->sect[seg]] + offset);
         else
            return (saip->aligncoords[ssdp->sect[seg]] + dsp->lens[ssdp->sect[seg]] - offset -1);
      } else
      {
      }
   } else if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)sap->saip;
      if (row_num > amaip->numrows)
         return -1;
      if (amaip->mstype != AM_SEGMENTED_MASTERSLAVE)
      {
         j = k = 0;
         m = -1;
         done = FALSE;
         while (!done && j < amaip->numseg)
         {
            k = AlnMgrMapRowCoords(sap, amaip->aligncoords[j], row_num, master);
            if (k == -1)
               j++;
            else if (k > pos)
               done = TRUE;
            else if (k <= pos)
            {
               m = j;
               offset = pos - k;
               j++;
            }
         }
         if (m == -1 || offset >= amaip->lens[m])
            return -2;
         n = AlnMgrGetNthStrand(sap, row_num);
         if (n != Seq_strand_minus)
         {
            return (amaip->aligncoords[m] + offset);
         } else
         {
            return (amaip->aligncoords[m] + amaip->lens[m] - 1 - offset);
         }
      } else
      {
         if(oldj) j = *oldj;
         else j = 0;
         k = 0;
         done = FALSE;
         while (!done && j < amaip->aligncoords[amaip->numseg-1]+amaip->lens[amaip->numseg-1])
         {
            k = AlnMgrMapRowCoords(sap, j, row_num, master);
            if (k == -1 || k < pos)
               j++;
            else if (k > pos)
               return -2;
            else if (k == pos)
               return j;
         }
         if (!done)
            return -2;
      }
   } else
      return -1;
   return -1;
}


/***********************************************************************
*
*  AlnMgrMakeFakeMultiple calls AlnMgrCheckOverlapping to decide whether
*  an alignment is linear.  Then, if possible, it calls AlnMgrMakeAlignCoords
*  to create alignment coordinates across all children contained in the
*  parent.  (MULT)
*
***********************************************************************/
NLM_EXTERN Boolean AlnMgrMakeFakeMultiple(SeqAlignPtr sap)
{
   return AlnMgrMakeFakeMultipleEx(sap, FALSE);
}

static Boolean AlnMgrMakeFakeMultipleEx(SeqAlignPtr sap, Boolean forcestraightms)
{
   AMAlignDatPtr    amadp;
   AMAlignIndexPtr  amaip;
   Int4             i;
   Int4             j;
   Boolean          ms;
   Int4             n;
   Boolean          nogap;
   Boolean          retval;
   int              unaligned=0;

   retval = FALSE;
   if (!sap)
      return retval;
   i = AlnMgrCheckAlignForParent(sap);
   if (i<0)
   {
      return retval;
   }
   if (i==AM_PARENT)
   {
      n = AlnMgrCheckOrdered(sap);
      if (n == AM_ORDERED)
      {
         sap->type = SAT_PARTIAL;
         amaip = (AMAlignIndexPtr)sap->saip;
         amaip->alnsaps = amaip->numsaps;
         amaip->startsize = (amaip->alnsaps)*(amaip->alnsaps);
         amaip->starts = (Int4Ptr)MemNew((amaip->alnsaps)*(amaip->alnsaps)*sizeof(Int4));
         amaip->lens = (Int4Ptr)MemNew((amaip->alnsaps)*sizeof(Int4));
         amaip->ulens = (Int4Ptr)MemNew((amaip->alnsaps)*sizeof(Int4));
         amaip->numseg = amaip->alnsaps;
         for (j=0; j<(amaip->alnsaps); j++)
         {
            amaip->lens[j] = AlnMgrGetAlnLength(amaip->saps[j], FALSE);
            amaip->starts[j] = 0;
         }
         AlnMgrMakeAlignCoords(sap);
         if (!AlnMgrMakeRowsForOrdered(sap))
            return FALSE;
         for (j=0; j<(amaip->alnsaps-1); j++)
         {
            amaip->ulens[j] = AlnMgrGetMaxUnalignedLength(amaip->saps[j], amaip->saps[j+1]);
            unaligned += amaip->ulens[j];
         }
         { /* HS 07/24/00 */
             sap->master = AlnMgrFindMaster(sap);
             ms = FALSE;
             ms = AlnMgrCheckRealMaster(sap, sap->master);
             if (sap->master && ms == TRUE)
                 {
                     sap->type = SAT_MASTERSLAVE;
                     /* if there's more than 1 denseseg, call it segmented, DIH, 7/27/00 */
                     if (amaip->numseg > 1) {
                     /* if (unaligned > 0) { */
                         amaip->mstype = AM_SEGMENTED_MASTERSLAVE;
                     } else {
                         amaip->mstype = AM_MASTERSLAVE;
                     }
                     AlnMgrSetMaster(sap, sap->master);
                     AlnMgrMakeMasterPlus(sap);
                     n = AlnMgrGetNForSip(sap, sap->master);
                     sap->type = SAT_MASTERSLAVE;
                     amaip->master = n;
                 } else {
                     sap->master=NULL;
                 }
         }
         return TRUE;
      }
      n = AlnMgrCheckOverlapping(sap);
      nogap = am_check_gaps(sap);
      if (n == NO_OVERLAP)
      {
         sap->type = SAT_PARTIAL;
         amaip = (AMAlignIndexPtr)sap->saip;
         if (amaip->saps)
            MemFree(amaip->saps);
         amaip->saps = AlnMgrSortSeqAligns((SeqAlignPtr)(sap->segs), AlnMgrFindFirst, amaip, &amaip->numsaps);
         amaip->alnsaps = amaip->numsaps;
         amaip->startsize = (amaip->alnsaps)*(amaip->alnsaps);
         amaip->starts = (Int4Ptr)MemNew((amaip->alnsaps)*(amaip->alnsaps)*sizeof(Int4));
         amaip->lens = (Int4Ptr)MemNew((amaip->alnsaps)*sizeof(Int4));
         amaip->ulens = (Int4Ptr)MemNew((amaip->alnsaps)*sizeof(Int4));
         amaip->numseg = amaip->alnsaps;
         for (j=0; j<(amaip->alnsaps); j++)
         {
            amaip->lens[j] = AlnMgrGetAlnLength(amaip->saps[j], FALSE);
            amaip->starts[j] = 0;
         }
         AlnMgrMakeAlignCoords(sap);
         if (!AlnMgrGetRowsForPartial(sap))
            return retval;
         for (j=0; j<(amaip->alnsaps-1); j++)
         {
            amaip->ulens[j] = AlnMgrGetMaxUnalignedLength(amaip->saps[j], amaip->saps[j+1]);
            unaligned += amaip->ulens[j];
         }
         { /* HS */
             sap->master = AlnMgrFindMaster(sap);
             ms = FALSE;
             ms = AlnMgrCheckRealMaster(sap, sap->master);
             if (sap->master && ms == TRUE)
                 {
                     sap->type = SAT_MASTERSLAVE;
                     if(unaligned>0) {
                         amaip->mstype = AM_SEGMENTED_MASTERSLAVE;
                     } else {
                         amaip->mstype = AM_MASTERSLAVE;
                     }
                     AlnMgrSetMaster(sap, sap->master);
                     AlnMgrMakeMasterPlus(sap);
                     n = AlnMgrGetNForSip(sap, sap->master);
                     sap->type = SAT_MASTERSLAVE;
                     amaip->master = n;
                 } else {
                     sap->master=NULL;
                 }
         }
         retval = TRUE;
      } else /*should add function to check for pairwise multiple vs. diags*/
      {
         amaip = (AMAlignIndexPtr)sap->saip;
         if (amaip->saps)
            MemFree(amaip->saps);
         sap->master = AlnMgrFindMaster(sap);
         amaip->alnsaps = amaip->numsaps;
         ms = FALSE;
         ms = AlnMgrCheckRealMaster(sap, sap->master);
         if (sap->master && ms == TRUE)
         {
            retval = TRUE;
            AlnMgrSetMaster(sap, sap->master);
            AlnMgrMakeMasterPlus(sap);
            n = AlnMgrGetNForSip(sap, sap->master);
            sap->type = SAT_MASTERSLAVE;
            amaip->master = n; /* HS 7/24/00 was ==1 */
            amaip->numseg = AlnMgrGetMaxSegments((SeqAlignPtr)(sap->segs));
            amaip->alnsaps = amaip->numsaps;
            amaip->lens = (Int4Ptr)MemNew((amaip->numseg)*sizeof(Int4));
            amadp = amaip->amadp[n-1];
            amaip->saps = (SeqAlignPtr PNTR)MemNew((amaip->numsaps)*sizeof(SeqAlignPtr));
            for (j=0; j<amaip->numsaps; j++)
            {
               amaip->saps[j] = amadp->saps[j];
            }
            if (forcestraightms || amaip->numsaps < amaip->numbsqs)
            {
               amaip->ids = SeqIdSetFree(amaip->ids);
               amaip->ids = AlnMgrPropagateSeqIdsBySapList(amaip);
               if (!AlnMgrMergeIntoMSMultByMaster(amaip, amaip->lens, &amaip->numseg))
                  retval = FALSE;
               amaip->startsize = (amaip->numseg)*(amaip->numsaps);
               amaip->starts = (Int4Ptr)MemNew((amaip->numseg)*(amaip->numsaps)*sizeof(Int4));
               amaip->aligncoords = (Uint4Ptr)MemNew((amaip->numseg)*sizeof(Uint4));
               if (!AlnMgrFillInStarts(amadp->saps, amaip->starts, amaip->numseg, amaip->lens, amaip->numsaps, amaip->aligncoords))
                  retval = FALSE;
               if (amaip->numseg > 1)
                  amaip->numseg -= 1;
               if (!AlnMgrMakeMultSegments(amaip))
                  retval = FALSE;
               if (!AlnMgrGetRowsForMasterSlave(sap))
                  retval = FALSE;
            } else
               retval = FALSE;
         }
         if (retval == FALSE && sap->master != NULL)
         {
            if (AlnMgrMakeSegmentedMasterSlave(sap))
            {
               sap->type = SAT_MASTERSLAVE;
               amaip->ids = SeqIdSetFree(amaip->ids);
               amaip->ids = AlnMgrPropagateSeqIdsByRow(amaip);
               retval = TRUE;
            } else
            {
               if (AlnMgrForceMasterSlave(sap))
               {
                  amaip->ids = SeqIdSetFree(amaip->ids);
                  amaip->ids = AlnMgrPropagateSeqIdsByRow(amaip);
                  amaip->mstype = AM_MASTERSLAVE;
                  retval = TRUE;
               }
            }
         } else
         {
            amaip->mstype = AM_MASTERSLAVE;
         }
      }
   }
   return retval;
}

/**********************************************************************
*
*  AlnMgrNeatlyIndex is a very specialized function for structure
*  alignments and other alignments that have an implied row order.
*  It puts an index on the first seqalign in the set, and that index
*  only designates which seqaligns belong in which rows.  The input
*  alignments are not changed at all, and only the first alignment is
*  indexed.  This alignment cannot be displayed or accessed; it's only
*  a way to designate rows.
*
**********************************************************************/
NLM_EXTERN Boolean AlnMgrNeatlyIndex(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   Int4             numrows;
   SeqAlignPtr      salp;

   if (sap == NULL)
      return FALSE;
   if (sap->saip != NULL)
   {
      if (sap->saip->indextype == INDEX_PARENT)
         AMAlignIndexFree((Pointer)(sap->saip));
      else if (sap->saip->indextype == INDEX_SEGS)
         SAIndexFree((Pointer)(sap->saip));
   }
   sap->saip = NULL;
   if (sap->next != NULL)
      salp = sap;
   else if (sap->segtype == SAS_DISC)
      salp = (SeqAlignPtr)(sap->segs);
   else
      salp = sap;
   numrows = 0;
   while (salp != NULL)
   {
      numrows++;
      salp = salp->next;
   }
   amaip = AMAlignIndexNew();
   sap->saip = (SeqAlignIndexPtr)(amaip);
   amaip->mstype = AM_NEATINDEX;
   amaip->master = 1;
   if (sap->next != NULL)
      salp = sap;
   else if (sap->segtype == SAS_DISC)
      salp = (SeqAlignPtr)(sap->segs);
   else
      salp = sap;
   amaip->saps = (SeqAlignPtr PNTR)MemNew(numrows*sizeof(SeqAlignPtr));
   numrows = 0;
   while (salp != NULL)
   {
      amaip->saps[numrows] = salp;
      numrows++;
      salp = salp->next;
   }
   amaip->numrows = numrows;
   amaip->master = 1;
   return TRUE;
}

static void AlnMgrMergeNeighbors(SeqAlignPtr salp)
{
   DenseDiagPtr  ddp;
   DenseDiagPtr  ddp_prev;
   Boolean       gap;
   Int4          i;

   if (salp == NULL)
      return;
   if (salp->segtype == SAS_DENDIAG)
   {
      ddp_prev = (DenseDiagPtr)(salp->segs);
      while (ddp_prev && ddp_prev->next != NULL)
      {
         ddp = ddp_prev->next;
         if (ddp_prev->dim != ddp->dim)
            return;
         gap = FALSE;
         for (i=0; i<ddp->dim && !gap; i++)
         {
            if (ddp->strands != NULL && ddp->strands[i] == Seq_strand_minus)
            {
               if (ddp->starts[i] + ddp->len < ddp_prev->starts[i])
                  gap = TRUE;
            } else
            {
               if (ddp_prev->starts[i] + ddp_prev->len < ddp->starts[i])
                  gap = TRUE;
            }
         }
         if (gap == FALSE)
         {
            if (ddp->strands != NULL && ddp->strands[i] == Seq_strand_minus)
            {
               ddp_prev->len += ddp->len;
               for (i=0; i<ddp->dim; i++)
               {
                  ddp_prev->starts[i] = ddp->starts[i];
               }
            } else
               ddp_prev->len += ddp->len;
            ddp_prev->next = ddp->next;
            ddp->next = NULL;
            DenseDiagFree(ddp);
         } else
            ddp_prev = ddp_prev->next;
      }
   } else if (salp->segtype == SAS_DISC)
   {
      return;
   } else
      return;
   return;
}

/**********************************************************************
*
*  AlnMgrTossNeatRows is called to create a subset of the NeatlyIndexed
*  alignment, only containing certain rows.  The array throwarray, of 
*  length len, contains the (1-based) numbers of the rows to be left
*  out.  The function returns a duplicated alignment, which is not
*  yet indexed.  This function assumes a very strict row structure --
*  each row is represented by a single seqalign or seqalign set, and
*  the first row is the master.
*
**********************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgrTossNeatRows(SeqAlignPtr sap, Int4Ptr throwarray, Int4 len)
{
   AMAlignIndexPtr  amaip;
   Int4             i;
   Int4             n;
   SeqAlignPtr      salp;
   SeqAlignPtr      sap_head;
   SeqAlignPtr      sap_new;
   SeqAlignPtr      sap_prev;
   Boolean          toss;

   if (sap == NULL || throwarray == NULL)
      return NULL;
   if (sap->saip == NULL)
      return NULL;
   if (sap->next != NULL)
      salp = sap;
   else if (sap->segtype == SAS_DISC)
      salp = (SeqAlignPtr)(sap->segs);
   else
      salp = sap;
   amaip = (AMAlignIndexPtr)(sap->saip);
   if (amaip->mstype != AM_NEATINDEX)
      return NULL;
   i = 2;
   sap_head = NULL;
   while (salp != NULL)
   {
      toss = FALSE;
      for (n=0; n<len; n++)
      {
         if (throwarray[n] == i)
            toss = TRUE;
      }
      if (toss == FALSE)
      {
         sap_new = SeqAlignDup(salp);
         if (sap_head != NULL)
         {
            sap_prev->next = sap_new;
            sap_prev = sap_new;
         } else
            sap_prev = sap_head = sap_new;
      }
      salp = salp->next;
      i++;
   }
   return sap_head;
}

/***************************************************************************
*
*   AlnMgrMakeMultByIntersectOnMaster is a specialized function that 
*   truncates the segments of segmented master-slave alignments to
*   force them to line up:
*
*   Master   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
*   seq1        XXXXXXXX       XXXXXXXXXXXXXX
*   seq2       XXXXXXXXXX        XXXXXXX
*   seq3          XXXXXXXXXXXXXXXXXXXXXXX
*
*   becomes
*
*   Master        XXXXXX         XXXXXXX
*   seq1          XXXXXX         XXXXXXX
*   seq2          XXXXXX         XXXXXXX
*   seq3          XXXXXX         XXXXXXX
*
*   This indexing is different from the other type -- the input
*   alignments may actually be truncated, instead of just rearranged.
*   If allinblock is TRUE, then the function throws away any blocks that
*   have a missing sequence.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgrMakeMultByIntersectOnMaster(SeqAlignPtr sap, Boolean allinblock)
{
   AMmsmsPtr    ams;
   AMmsmsPtr    ams_master;
   AMmsmsPtr    ams_tmp;
   Int4         numrows;
   Int4Ptr      rowarray;
   SeqAlignPtr  salp;
   SeqIdPtr     sip_tmp;

   if (sap == NULL)
      return FALSE;
   if (sap->saip != NULL)
   {
      if (sap->saip->indextype == INDEX_PARENT)
         AMAlignIndexFree((Pointer)(sap->saip));
      else if (sap->saip->indextype == INDEX_SEGS)
         SAIndexFree((Pointer)(sap->saip));
   }
   sap->saip = NULL;
   if (sap->segtype == SAS_DISC)
      salp = (SeqAlignPtr)(sap->segs);
   else
      salp = sap;
   while (salp != NULL)
   {
      /* AlnMgrMergeNeighbors(salp); */   /* dih - 3/21/01 */
      salp = salp->next;
   }
   rowarray = NULL;
   if (allinblock)
   {
      numrows = am_guess_numrows(sap);
      rowarray = am_save_rowinfo(sap, numrows-1);
   } else
      numrows = 0;
   if (!AlnMgrUnpackSeqAlign(sap))
      return FALSE;
   if (!AlnMgrRearrangeUnpacked(sap))
      return FALSE;
   if (!AlnMgrIndexLinkedSegs((SeqAlignPtr)(sap->segs)))
      return FALSE;
   if (sap->master == NULL)  /* if a master is already set, believe it */
      sip_tmp = am_find_master(sap);  /* if not, find it */
   if (sip_tmp == NULL)
      return FALSE;
   am_set_master(sap, sip_tmp);
   SeqIdFree(sip_tmp);
   ams = AlnMgrFindOverlapOnMaster(sap);
   if (ams == NULL)
      return FALSE;
   if (allinblock)
      am_set_rows(ams, rowarray, numrows);
   ams_master = AlnMgrTruncateByOverlap(sap, ams);
   if (ams_master == NULL)
      return FALSE;
   if (!AlnMgrCarefulIndex(sap, ams, &ams_master, allinblock, numrows))
      return FALSE;
   while (ams)
   {
      ams_tmp = ams->next;
      ams->next = NULL;
      ams->sip = NULL;
      ams->sap = NULL;
      MemFree(ams);
      ams = ams_tmp;
   }
   while (ams_master)
   {
      ams_tmp = ams_master->next;
      ams_master->next = NULL;
      ams_master->sip = NULL;
      ams_master->sap = NULL;
      MemFree(ams_master);
      ams_master = ams_tmp;
   }
   if (rowarray != NULL)
      MemFree(rowarray);
   return TRUE;
}

static Boolean am_is_new_row(SeqIdPtr sip1, SeqIdPtr sip2)
{
   if (sip1 == NULL || sip2 == NULL)
      return FALSE;
   while (sip1 != NULL && sip2 != NULL)
   {
      if (SeqIdComp(sip1, sip2) != SIC_YES)
         return TRUE;
      sip1 = sip1->next;
      sip2 = sip2->next;
   }
   if (sip1 != NULL || sip2 != NULL)
      return TRUE;
   return FALSE;
}

NLM_EXTERN Int4 am_guess_numrows(SeqAlignPtr sap)
{
   DenseDiagPtr  ddp;
   Boolean       disc;
   DenseSegPtr   dsp;
   SeqIdPtr      id_prev;
   Int4          n;
   SeqAlignPtr   salp;

   if (sap == NULL)
      return 0;
   disc = FALSE;
   if (sap->segtype == SAS_DISC)
   {
      disc = TRUE;
      salp = (SeqAlignPtr)(sap->segs);
      sap = sap->next;
   } else
      salp = sap;
   n = 0;
   id_prev = NULL;
   while (salp)
   {
      if (salp->segtype == SAS_DENSEG)
      {
         dsp = (DenseSegPtr)(salp->segs);
         if (am_is_new_row(dsp->ids, id_prev) || salp->next == NULL)
            n+= dsp->dim - 1;
         id_prev = dsp->ids;
      } else if (salp->segtype == SAS_DENDIAG)
      {
         ddp = (DenseDiagPtr)(salp->segs);
         n+=ddp->dim;
         n = n-1;
         id_prev = ddp->id;
      } else if (salp->segtype == SAS_DISC)
         n++;
      salp = salp->next;
      if (salp == NULL)
      {
         if (disc && sap!=NULL)
         {
            if (sap->segtype == SAS_DISC)
            {
               salp = (SeqAlignPtr)(sap->segs);
               sap = sap->next;
            }
         }
      }
   }
   return (n+1);
}

static Int4Ptr am_save_rowinfo(SeqAlignPtr sap, Int4 numrows)
{
   DenseDiagPtr  ddp;
   Int4          dim_prev;
   Boolean       disc;
   DenseSegPtr   dsp;
   Int4          i;
   SeqIdPtr      id_prev;
   Int4          n;
   Int4          num;
   Int4Ptr       rowarray;
   SeqAlignPtr   salp;
   SeqAlignPtr   sap2;

   if (sap == NULL)
      return 0;
   disc = FALSE;
   if (sap->segtype == SAS_DISC)
   {
      disc = TRUE;
      salp = (SeqAlignPtr)(sap->segs);
      sap = sap->next;
   } else
      salp = sap;
   n = 0;
   id_prev = NULL;
   dim_prev = 0;
   num = 1;
   rowarray = (Int4Ptr)MemNew((numrows+1)*sizeof(Int4));
   while (salp)
   {
      if (salp->segtype == SAS_DENSEG)
      {
         dsp = (DenseSegPtr)(salp->segs);
         if (am_is_new_row(dsp->ids, id_prev) || salp->next == NULL)
         {
            for (i=n; i<(n+dim_prev-1); i++)
            {
               rowarray[i] = num;
            }
            n += dim_prev-1;
            num = 1;
         } else
            num++;
         id_prev = dsp->ids;
         dim_prev = dsp->dim;
      } else if (salp->segtype == SAS_DENDIAG)
      {
         ddp = (DenseDiagPtr)(salp->segs);
         num = 0;
         while (ddp)
         {
            num++;
            ddp = ddp->next;
         }
         ddp = (DenseDiagPtr)(salp->segs);
         for (i=n; i<(n+ddp->dim-1); i++)
         {
            rowarray[i] = num;
         }
         n+=ddp->dim;
         id_prev = ddp->id;
         n = n-1;
      } else if (salp->segtype == SAS_DISC)
      {
         sap2 = (SeqAlignPtr)(salp->segs);
         num = 0;
         while (sap2 != NULL)
         {
            num++;
            sap2 = sap2->next;
         }
         sap2 = (SeqAlignPtr)(salp->segs);
         for (i=n; i<(n+sap2->dim-1); i++)
         {
            rowarray[i] = num;
         }
         n+=sap2->dim;
         n = n - 1;
      }
      salp = salp->next;
      if (salp == NULL)
      {
         if (disc && sap!=NULL)
         {
            if (sap->segtype == SAS_DISC)
            {
               salp = (SeqAlignPtr)(sap->segs);
               sap = sap->next;
            }
         }
      }
   }
   return rowarray;
}

static void am_set_rows(AMmsmsPtr ams, Int4Ptr rowarray, Int4 numrows)
{
   Int4  i;
   Int4  n;

   if (ams == NULL || rowarray == NULL)
      return;
   for (n=0; n<numrows; n++)
   {
      if (ams == NULL)
         return;
      ams->stop = 1;
      for (i=0; i<rowarray[n]; i++)
      {
         ams = ams->next;
      }
   }
   return;
}

static void am_set_master(SeqAlignPtr sap, SeqIdPtr sip)
{
   SeqAlignPtr  salp;

   if (sap == NULL || sip == NULL)
      return;
   if (sap->segtype == SAS_DISC)
   {
      if (sap->master != NULL)
         SeqIdSetFree(sap->master);
      sap->master = SeqIdDup(sip);
      salp = (SeqAlignPtr)(sap->segs);
   } else
      salp = sap;
   while (salp)
   {
      if (salp->master != NULL)
         SeqIdSetFree(salp->master);
      salp->master = SeqIdDup(sip);
      salp = salp->next;
   }
   return;
}

static SeqIdPtr am_find_master(SeqAlignPtr sap)
{
   DenseDiagPtr  ddp;
   Boolean       done;
   DenseSegPtr   dsp;
   Boolean       found;
   Boolean       here;
   SeqAlignPtr   salp;
   SeqAlignPtr   sap_tmp;
   SeqIdPtr      sip;
   SeqIdPtr      sip_head;
   SeqIdPtr      sip_tmp;

   if (sap == NULL)
      return NULL;
   if (sap->segtype == SAS_DISC)
      salp = (SeqAlignPtr)(sap->segs);
   else
      salp = sap;
   if (salp->segtype == SAS_DENSEG)
   {
      dsp = (DenseSegPtr)(salp->segs);
      sip_head = dsp->ids;
   } else if (salp->segtype == SAS_DENDIAG)
   {
      ddp = (DenseDiagPtr)(salp->segs);
      sip_head = ddp->id;
   }
   sip = sip_head;
   done = FALSE;
   while (sip && !done)
   {
      sap_tmp = salp->next;
      found = TRUE;
      while (sap_tmp != NULL && found)
      {
         if (sap_tmp->segtype == SAS_DENSEG)
         {
            dsp = (DenseSegPtr)(sap_tmp->segs);
            sip_tmp = dsp->ids;
         } else if (sap_tmp->segtype == SAS_DENDIAG)
         {
            ddp = (DenseDiagPtr)(sap_tmp->segs);
            sip_tmp = ddp->id;
         }
         here = FALSE;
         while (sip_tmp != NULL && !here)
         {
            if (SAM_OrderSeqID(sip_tmp, sip) == 0)
               here = TRUE;
            sip_tmp = sip_tmp->next;
         }
         if (!here)
            found = FALSE;
         sap_tmp = sap_tmp->next;
      }
      if (found)
         done = TRUE;
      else
         sip = sip->next;
   }
   if (!done)
      return NULL;
   else
      return (SeqIdDup(sip));
}

static AMmsmsPtr AlnMgrFindOverlapOnMaster(SeqAlignPtr sap)
{
   AMmsmsPtr    ams;
   AMmsmsPtr    ams_head;
   AMmsmsPtr    ams_prev;
   SeqAlignPtr  salp;
   SeqAlignPtr  salp_tmp;

   if (sap == NULL)
      return NULL;
   if (sap->master == NULL)
      return NULL;
   ams_head = ams_prev = NULL;
   while (sap)
   {
      if (sap->segtype == SAS_DISC)
      {
         salp = (SeqAlignPtr)(sap->segs);
         while (salp)
         {
            if (salp->segtype == SAS_DISC)
            {
               salp_tmp = (SeqAlignPtr)(sap->segs);
               while (salp_tmp)
               {
                  ams = am_create_overlap(salp_tmp);
                  if (ams == NULL)
                     return NULL;
                  if (ams_head != NULL)
                  {
                     ams_prev->next = ams;
                     while (ams->next)
                     {
                        ams = ams->next;
                     }
                     ams_prev = ams;
                  } else
                  {
                     ams_head = ams;
                     while (ams->next)
                     {
                        ams = ams->next;
                     }
                     ams_prev = ams;
                  }
               }
            } else
            {
               ams = am_create_overlap(salp);
               if (ams == NULL)
                  return NULL;
               if (ams_head != NULL)
               {
                  ams_prev->next = ams;
                  while (ams->next)
                  {
                     ams = ams->next;
                  }
                  ams_prev = ams;
               } else
               {
                  ams_head = ams;
                  while (ams->next)
                  {
                     ams = ams->next;
                  }
                  ams_prev = ams;
               }
            }
            salp = salp->next;
         }
      } else
      {
         ams = am_create_overlap(sap);
         if (ams == NULL)
            return NULL;
         if (ams_head != NULL)
         {
            ams_prev->next = ams;
            while (ams->next)
            {
               ams = ams->next;
            }
            ams_prev = ams;
         } else
         {
            ams_head = ams;
            while (ams->next)
            {
               ams = ams->next;
            }
            ams_prev = ams;
         }
      }
      sap = sap->next;
   }
   return ams_head;
}

static AMmsmsPtr am_create_overlap(SeqAlignPtr sap)
{
   AMmsmsPtr     ams;
   AMmsmsPtr     ams_head;
   AMmsmsPtr     ams_prev;
   DenseDiagPtr  ddp;
   DenseSegPtr   dsp;
   Boolean       found;
   SeqIdPtr      id;
   Int4          n;
   SeqAlignPtr   sap_tmp;
   Int4          start;
   Int4          stop;

   if (sap == NULL || sap->master == NULL)
      return NULL;
   ams_head = ams_prev = NULL;
   if (sap->segtype == SAS_DENSEG)
   {
      dsp = (DenseSegPtr)(sap->segs);
      id = (dsp->ids);
      found = FALSE;
      n = 0;
      while (id!=NULL && !found)
      {
         n++;
         if (SAM_OrderSeqID(id, sap->master)== 0)
            found = TRUE;
         id = id->next;
      }
      if (!found || n>dsp->dim)
         return NULL;
      if (dsp->strands != NULL)
      {
         if (dsp->strands[n-1] == Seq_strand_minus)
         {
            sap_tmp = sap->next;
            sap->next = NULL;
            SeqAlignListReverseStrand(sap);
            sap->next = sap_tmp;
         }
      }
      if (!AlnMgrIndexSingleChildSeqAlign(sap))
         return NULL;
      AlnMgrGetNthSeqRangeInSA(sap, n, &start, &stop);
      ams = (AMmsmsPtr)MemNew(sizeof(AMmsms));
      ams->sap = sap;
      ams->n = n;
      ams->sstart = start;
      ams->sstop = stop;
      ams->count = AlnMgrGetNumRows(sap);
      ams->count = ams->count - 1;
      return ams;
   } else if (sap->segtype == SAS_DENDIAG)
   {
      ddp = (DenseDiagPtr)(sap->segs);
      while (ddp)
      {
         id = ddp->id;
         found = FALSE;
         n = 0;
         while (id!=NULL && !found)
         {
            n++;
            if (SeqIdComp(id, sap->master) == SIC_YES)
               found = TRUE;
            id = id->next;
         }
         if (!found || n > ddp->dim)
         {
            while (ams_head != NULL)
            {
               ams = ams_head->next;
               ams_head->next = NULL;
               MemFree(ams_head);
               ams_head = ams;
            }
            return NULL;
         }
         if (ddp->strands != NULL)
         {
            if (ddp->strands[n-1] == Seq_strand_minus)
               am_densediag_reverse(ddp);
         }
         start = ddp->starts[n-1];
         stop = start + ddp->len-1;
         ams = (AMmsmsPtr)MemNew(sizeof(AMmsms));
         ams->sip = id;
         ams->sap = sap;
         ams->n = n;
         ams->sstart = start;
         ams->sstop = stop;
         ams->count = AlnMgrGetNumRows(sap);
         ams->count = ams->count - 1;
         if (ams_head != NULL)
         {
            ams_prev->next = ams;
            ams_prev = ams;
         } else
            ams_head = ams_prev = ams;
         ddp = ddp->next;
      }
      return ams_head;
   } else
      return NULL;
}

static void am_densediag_reverse(DenseDiagPtr ddp)
{
   Int4  i;

   if (ddp == NULL || ddp->strands == NULL)
      return;
   for (i=0; i<ddp->dim; i++)
   {
      if (ddp->strands[i] == Seq_strand_minus)
         ddp->strands[i] = Seq_strand_plus;
      else
         ddp->strands[i] = Seq_strand_minus;
   }
}

static AMmsmsPtr AlnMgrConstructOverlaps(AMmsmsPtr ams_head)
{
   AMmsmsPtr      ams;
   AMmsmsPtr      ams_master;
   AMmsmsPtr      ams_mhead;
   AMmsmsPtr      ams_mprev;
   AMmsmsPtr      ams_new;
   Boolean        found;
   Int4           i;
   Int4           n;
   Int4           open;
   Int4           start;
   AMTinyInfoPtr  tip;
   AMTinyInfoPtr  PNTR tiparray;
   AMTinyInfoPtr  tip_head;
   AMTinyInfoPtr  tip_prev;

   if (ams_head == NULL)
      return NULL;
   n = 0;
   tip_head = NULL;
   ams = ams_head;
   while (ams != NULL)
   {
      tip = (AMTinyInfoPtr)MemNew(sizeof(AMTinyInfo));
      tip->start = ams->sstart;
      tip->which = 1;
      if (tip_head != NULL)
      {
         tip_prev->next = tip;
         tip_prev = tip;
      } else
         tip_prev = tip_head = tip;
      tip = (AMTinyInfoPtr)MemNew(sizeof(AMTinyInfo));
      tip->start = ams->sstop;
      tip->which = 2;
      tip_prev->next = tip;
      tip_prev = tip;
      n += 2;
      ams = ams->next;
   }
   tiparray = (AMTinyInfoPtr PNTR)MemNew(n*sizeof(AMTinyInfoPtr));
   tip = tip_head;
   i = 0;
   while (tip != NULL)
   {
      tiparray[i] = tip;
      tip = tip->next;
      i++;
   }
   HeapSort((Pointer)tiparray, (size_t)n, sizeof(AMTinyInfoPtr), AlnMgrCompareTips);
   tip_head = tip_prev = tiparray[0];
   tip_head->numsap = 1;
   for (i=1; i<n; i++)
   {
      if (tiparray[i]->start == tip_prev->start && tiparray[i]->which == tip_prev->which)
      {
         tip_prev->numsap++;
         MemFree(tiparray[i]);
      } else
      {
         tip_prev->next = tiparray[i];
         tiparray[i]->numsap = 1;
         tip_prev = tiparray[i];
      }
   }
   tip_prev->next = NULL;
   MemFree(tiparray);
   open = 0;
   ams_mhead = NULL;
   tip = tip_head;
   while (tip != NULL)
   {
      if (open > 0 && ((tip->which==1 && start <=tip->start-1)||(tip->which==2 && start <=tip->start)))
      {
         ams_master = (AMmsmsPtr)MemNew(sizeof(AMmsms));
         ams_master->sstart = start;
         if (tip->which == 1)
            ams_master->sstop = tip->start-1;
         else
            ams_master->sstop = tip->start;
         if (ams_mhead != NULL)
         {
            ams_mprev->next = ams_master;
            ams_mprev = ams_master;
         } else
            ams_mhead = ams_mprev = ams_master;
      }
      if (tip->which == 1)
      {
         start = tip->start;
         open += tip->numsap;
      } else
      {
         start = tip->start+1;
         open -= tip->numsap;
      }
      tip = tip->next;
   }
   while (tip_head != NULL)
   {
      tip = tip_head->next;
      MemFree(tip_head);
      tip_head = tip;
   }
   ams = ams_head;
   while (ams)
   {
      ams_master = ams_mhead;
      found = FALSE;
      while (ams_master != NULL && !found)
      {
         if (ams->sstart >= ams_master->sstart && ams->sstart <= ams_master->sstop)
         {
            found = TRUE;
            ams->sstop = ams_master->sstop;
            if (AlnMgrTruncateSAP(ams->sap, ams_master->sstart, ams_master->sstop, ams->n))
            {
               ams_new = (AMmsmsPtr)MemNew(sizeof(AMmsms));
               ams_new->sap = ams->sap->next;
               ams->sap->next = NULL;
               AlnMgrGetNthSeqRangeInSA(ams_new->sap, ams->n, &ams_new->sstart, &ams_new->sstop);
               ams_new->next = ams->next;
               ams->next = ams_new;
               ams_new->n = ams->n;
               ams_new->count = ams->count;
            }
            ams_master->sap = ams->sap;
            ams_master->n = ams->n;
         }
         ams_master = ams_master->next;
      }
      ams = ams->next;
   }
   return ams_mhead;
}

static AMmsmsPtr AlnMgrTruncateByOverlap(SeqAlignPtr sap, AMmsmsPtr ams_head)
{
   AMmsmsPtr      ams;
   AMmsmsPtr      ams_master;
   AMmsmsPtr      ams_mhead;
   AMmsmsPtr      ams_mprev;
   AMmsmsPtr      ams_new;
   AMmsmsPtr      ams_prev;
   Boolean        found;
   Int4           i;
   Int4           n;
   SeqAlignPtr    salp_prev;
   AMTinyInfoPtr  tip;
   AMTinyInfoPtr  PNTR tiparray;
   AMTinyInfoPtr  tip_head;
   AMTinyInfoPtr  tip_prev;

   if (sap == NULL || ams_head == NULL)
      return NULL;
   ams_mhead = ams_mprev = NULL;
   n = 0;
   ams = ams_head;
   tip_head = NULL;
   while (ams != NULL) /* create linked list of starts and stops */
   {
      tip = (AMTinyInfoPtr)MemNew(sizeof(AMTinyInfo));
      tip->start = ams->sstart;
      tip->which = 1;
      if (tip_head != NULL)
      {
         tip_prev->next = tip;
         tip_prev = tip;
      } else
         tip_head = tip_prev = tip;
      tip = (AMTinyInfoPtr)MemNew(sizeof(AMTinyInfo));
      tip->start = ams->sstop;
      tip->which = 2;
      tip_prev->next = tip;
      tip_prev = tip;
      n+=2;
      ams = ams->next;
   }
  /* sort the list */
   tiparray = (AMTinyInfoPtr PNTR)MemNew(n*sizeof(AMTinyInfoPtr));
   tip = tip_head;
   for (i=0; i<n; i++)
   {
      if (tip == NULL)
         return FALSE;
      tiparray[i] = tip;
      tip = tip->next;
   }
   HeapSort((Pointer)tiparray, (size_t)(n), sizeof(AMTinyInfoPtr), AlnMgrCompareTips);
   tip_head = tiparray[0];
   for (i=0; i<n-1; i++)
   {
      tiparray[i]->next = tiparray[i+1];
      tiparray[i+1]->next = NULL;
   }
 /* now look for start-stop pairs -- these are the blocks */
   tip = tip_head;
   while (tip->next != NULL)
   {
      if (tip->which == 1 && tip->next->which == 2)
      {
         ams_master = (AMmsmsPtr)MemNew(sizeof(AMmsms));
         ams_master->sstart = tip->start;
         ams_master->sstop = tip->next->start;
         if (ams_mhead != NULL)
         {
            ams_mprev->next = ams_master;
            ams_mprev = ams_master;
         } else
            ams_mhead = ams_mprev = ams_master;
      }
      tip = tip->next;
   }
   for (i=0; i<n; i++)
   {
      MemFree(tiparray[i]);
   }
   MemFree(tiparray);
 /* now compare the ams with the master blocks and truncate as needed */
   ams = ams_head;
   ams_prev = NULL;
   while (ams)
   {
      ams_master = ams_mhead;
      found = FALSE;
      while (ams_master && !found)
      {
         if (ams->sstart <= ams_master->sstart && ams->sstop >= ams_master->sstart)
         {
            found = TRUE;
            ams->sstart = ams_master->sstart;
            ams->sstop = ams_master->sstop;
            if (AlnMgrTruncateSAP(ams->sap, ams_master->sstart, ams_master->sstop, ams->n))
            {
               ams_new = (AMmsmsPtr)MemNew(sizeof(AMmsms));
               ams_new->sap = ams->sap->next;
               AlnMgrGetNthSeqRangeInSA(ams_new->sap, ams->n, &ams_new->sstart, &ams_new->sstop);
               ams_new->next = ams->next;
               ams_new->n = ams->n;
               ams_new->count = ams->count;
               ams->next = ams_new;
            }
            ams_master->sap = ams->sap;
            ams_master->masternum = ams->n;
            ams_prev = ams;
            ams = ams->next;
         } else if (ams->sstart <= ams_master->sstart && ams->sstop < ams_master->sstart)
         {  /*this one isn't going to get used -- free it */
            found = TRUE;
            if (ams_prev != NULL)
            {
               salp_prev = ams_prev->sap;
               salp_prev->next = ams->sap->next;
               ams_prev->next = ams->next;
               ams->next = NULL;
               ams->sap->next = NULL;
               SeqAlignFree(ams->sap);
               MemFree(ams);
               ams = ams_prev->next;
            } else
            {
               ams->sap->next = NULL;
               SeqAlignFree(ams->sap);
               ams_prev = ams;
               ams = ams->next;
               MemFree(ams_prev);
               ams_prev = NULL;
            }
         }
         ams_master = ams_master->next;
      }
      if (!found)
      {
         found = TRUE;
         if (ams_prev != NULL)
         {
            salp_prev = ams_prev->sap;
            salp_prev->next = ams->sap->next;
            ams_prev->next = ams->next;
            ams->next = NULL;
            ams->sap->next = NULL;
            SeqAlignFree(ams->sap);
            MemFree(ams);
            ams = ams_prev->next;
         } else
         {
            ams->sap->next = NULL;
            SeqAlignFree(ams->sap);
            ams_prev = ams;
            ams = ams->next;
            MemFree(ams_prev);
            ams_prev = NULL;
         }
      }
   }
   return ams_mhead;
}

/***************************************************************************
*
*   AlnMgrTruncateSAP truncates a given seqalign to contain only the
*   bioseq coordinates from start to stop on the indicated row.  Anything
*   before those coordinates is discarded; anything remaining afterwards
*   is made into another seqalign and put in sap->next (the original next,
*   if any, is now at sap->next->next).  Doesn't work on parent seqaligns.
*   The function returns TRUE if the orignal alignment extended past stop.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgrTruncateSAP(SeqAlignPtr sap, Int4 start, Int4 stop, Int4 row)
{
   DenseDiagPtr  ddp;
   DenseDiagPtr  ddp2;
   DenseSegPtr   dsp;
   Int4          from;
   Int4          i;
   Int4          mstart;
   Int4          mstop;
   SeqAlignPtr   sap1;
   SeqAlignPtr   sap2;
   Int4          tmp;
   Int4          to;

   if (sap == NULL || stop<start || row < 1)
      return FALSE;
   if (sap->segtype == SAS_DENSEG)
   {
      if (sap->saip == NULL)
         AlnMgrIndexSingleChildSeqAlign(sap);
      AlnMgrGetNthSeqRangeInSA(sap, row, &mstart, &mstop);
      if (mstart > start || mstop < stop)
         return FALSE;
      if (mstart == start)
      {
         if (mstop == stop)
            return FALSE;
         else if (mstop > stop)
         {
            from = AlnMgrMapBioseqToSeqAlign(sap, start, row, NULL);
            to = AlnMgrMapBioseqToSeqAlign(sap, stop, row, NULL);
            if (to < from)
            {
               tmp = to;
               to = from;
               from = tmp;
            }
            sap1 = AlnMgrGetSubAlign(sap, NULL, from, to);
            AlnMgrIndexSingleChildSeqAlign(sap1);
            from = AlnMgrMapBioseqToSeqAlign(sap, stop+1, row, NULL);
            if (from < 0)
               return FALSE;
            to = AlnMgrMapBioseqToSeqAlign(sap, mstop, row, NULL);
            if (to < from)
            {
               tmp = to;
               to = from;
               from = tmp;
            }
            sap2 = AlnMgrGetSubAlign(sap, NULL, from, to);
            sap2->next = sap->next;
            sap->next = sap2;
            dsp = sap->segs;
            sap->segs = (Pointer)(sap1->segs);
            sap1->segs = NULL;
            DenseSegFree(dsp);
            SeqAlignFree(sap1);
            AlnMgrIndexSingleChildSeqAlign(sap);
            AlnMgrIndexSingleChildSeqAlign(sap2);
            return TRUE;
         }
      } else if (mstart < start) /* throw away the first part */
      {
         from = AlnMgrMapBioseqToSeqAlign(sap, start, row, NULL);
         to = AlnMgrMapBioseqToSeqAlign(sap, stop, row, NULL);
         if (to < from)
         {
            tmp = to;
            to = from;
            from = tmp;
         }
         sap1 = AlnMgrGetSubAlign(sap, NULL, from, to);
         if (mstop == stop) /* done */
         {
            dsp = sap->segs;
            sap->segs = (Pointer)(sap1->segs);
            sap1->segs = NULL;
            DenseSegFree(dsp);
            SeqAlignFree(sap1);
            AlnMgrIndexSingleChildSeqAlign(sap);
            return TRUE;
         } else if (mstop > stop)
         {
            from = AlnMgrMapBioseqToSeqAlign(sap, stop+1, row, NULL);
            if (from < 0)
               return FALSE;
            to = AlnMgrMapBioseqToSeqAlign(sap, mstop, row, NULL);
            if (to < from)
            {
               tmp = to;
               to = from;
               from = tmp;
            }
            sap2 = AlnMgrGetSubAlign(sap, NULL, from, to);
            sap2->next = sap->next;
            sap->next = sap2;
            AlnMgrIndexSingleChildSeqAlign(sap2);
            dsp = sap->segs;
            sap->segs = (Pointer)(sap1->segs);
            sap1->segs = NULL;
            DenseSegFree(dsp);
            SeqAlignFree(sap1);
            AlnMgrIndexSingleChildSeqAlign(sap);
            return TRUE;
         }
      }
   } else if (sap->segtype == SAS_DENDIAG)
   {
      ddp = (DenseDiagPtr)(sap->segs);
      if (ddp->dim < row)
         return FALSE;
      mstart = ddp->starts[row-1];
      mstop = mstart + ddp->len - 1;
      if (mstart > start || mstop < stop)
         return FALSE;
      if (mstart == start)
      {
         if (mstop == stop)
            return FALSE;
         else if (mstop > stop)
         {
            ddp2 = DenseDiagNew();
            ddp2->dim = ddp->dim;
            ddp2->starts = (Int4Ptr)MemNew((ddp->dim)*sizeof(Int4));
            ddp2->id = SeqIdDupList(ddp->id);
            ddp2->strands = (Uint1Ptr)MemNew((ddp->dim)*sizeof(Uint1));
            ddp2->scores = ScoreDup(ddp->scores);
            for (i=0; i<ddp->dim; i++)
            {
               ddp2->starts[i] = ddp->starts[i] + ddp->len - (mstop - stop);
               ddp2->strands[i] = ddp->strands[i];
            }
            ddp2->len = mstop - stop;
            ddp->len = ddp->len - (mstop - stop);
            sap2 = SeqAlignNew();
            sap2->type = SAT_PARTIAL;
            sap2->segtype = SAS_DENSEG;
            sap2->segs = (Pointer)ddp2;
            sap2->next = sap->next;
            sap->next = sap2;
            AlnMgrIndexSingleChildSeqAlign(sap2);
            return TRUE;
         }
      } else if (mstart < start)
      {
         for (i=0; i<ddp->dim; i++)
         {
            ddp->starts[i] = ddp->starts[i] + start - mstart;
         }
         ddp->len = ddp->len - (start - mstart);
         AlnMgrIndexSingleChildSeqAlign(sap);
         if (mstop == stop)
            return FALSE;
         else if (mstop > stop)
         {
            ddp2 = DenseDiagNew();
            ddp2->dim = ddp->dim;
            ddp2->starts = (Int4Ptr)MemNew((ddp->dim)*sizeof(Int4));
            ddp2->id = SeqIdDupList(ddp->id);
            ddp2->strands = (Uint1Ptr)MemNew((ddp->dim)*sizeof(Uint1));
            ddp2->scores = ScoreDup(ddp->scores);
            for (i=0; i<ddp->dim; i++)
            {
               ddp2->starts[i] = ddp->starts[i] + ddp->len - (mstop - stop);
               ddp2->strands[i] = ddp->strands[i];
            }
            ddp2->len = mstop - stop;
            ddp->len = ddp->len - (mstop - stop);
            sap2 = SeqAlignNew();
            sap2->type = SAT_PARTIAL;
            sap2->segtype = SAS_DENSEG;
            sap2->segs = (Pointer)ddp2;
            sap2->next = sap->next;
            sap->next = sap2;
            AlnMgrIndexSingleChildSeqAlign(sap2);
            return TRUE;
         }   
      }
   } else
      return FALSE;
   return FALSE;
}

static void am_compare_alignids(AMmsmsPtr ams_prev, AMmsmsPtr ams)
{
   DenseSegPtr  dsp1;
   DenseSegPtr  dsp2;
   Boolean      found;
   SeqIdPtr     sip;
   SeqIdPtr     sip1;
   SeqIdPtr     sip2;

   if (ams_prev == NULL || ams == NULL)
      return;
   dsp1 = (DenseSegPtr)(ams_prev->sap->segs);
   dsp2 = (DenseSegPtr)(ams->sap->segs);
   sip1 = dsp1->ids;
   sip2 = dsp2->ids;
   while (sip1 != NULL)
   {
      sip = sip2;
      found = FALSE;
      while (sip != NULL && !found)
      {
         if (SeqIdComp(sip1, sip) == SIC_YES)
            found = TRUE;
         sip = sip->next;
      }
      if (!found)
      {
         ams->stop = 1;
         return;
      }
      sip1 = sip1->next;
   }
   return;
}

static Boolean AlnMgrJaggedIndex(SeqAlignPtr sap, AMmsmsPtr ams_head, AMmsmsPtr *ams_mhead, Int4 numrows)
{
   AMAlignIndexPtr  amaip;
   AMmsmsPtr        ams;
   AMmsmsPtr        ams_master;
   AMmsmsPtr        ams_prev;
   Boolean          found;
   Int4             i;
   Int4             j;
   Int4             n;
   Int4             numblocks;
   RowSourcePtr     rsp;
   RowSourcePtr     rsp_curr_head;
   RowSourcePtr     rsp_head;
   RowSourcePtr     rsp_prev;
   SeqAlignPtr      PNTR saparray;
   Boolean          usethis;

   if (sap == NULL || ams_head == NULL || ams_mhead == NULL)
      return FALSE;
   if (!AlnMgrIndexParentSA(sap))
      return FALSE;
   AlnMgrSetMaster(sap, sap->master);
   ams_master = *ams_mhead;
   numblocks = 0;
   while (ams_master != NULL)
   {
      numblocks++;
      ams_master->count = numblocks;
      ams_master = ams_master->next;
   }
   ams = ams_head;
   j = 0;
   while (ams != NULL)
   {
      ams->count = j;
      ams = ams->next;
      j++;
   }
   saparray = (SeqAlignPtr PNTR)MemNew(j*sizeof(SeqAlignPtr));
   ams = ams_head;
   j = 0;
   while (ams != NULL)
   {
      saparray[j] = ams->sap;
      ams = ams->next;
      j++;
   }
   ams_master = *ams_mhead;
   rsp_head = RowSourceNew();
   rsp_head->which_saps = (Uint4Ptr)MemNew(numblocks*sizeof(Uint4));
   rsp_head->num_in_sap = (Uint4Ptr)MemNew(numblocks*sizeof(Uint4));
   rsp_head->id = SeqIdDup(sap->master);
   rsp_head->strand = Seq_strand_plus;
   rsp_head->numsaps = 0;
   while (ams_master != NULL)
   {
      found = FALSE;
      for (i=0; i<j && !found; i++)
      {
         if (saparray[i] == ams_master->sap)
            found = TRUE;
      }
      if (!found)
         return FALSE;
      rsp_head->which_saps[rsp_head->numsaps] = i;
      rsp_head->num_in_sap[rsp_head->numsaps] = ams_master->n;
      rsp_head->numsaps++;
      ams_master = ams_master->next;
   }
   rsp_prev = rsp_head;
   ams = ams_head;
   amaip = (AMAlignIndexPtr)(sap->saip);
   amaip->numsaps = amaip->alnsaps = j;
   amaip->saps = saparray;
   amaip->mstype = AM_SEGMENTED_MASTERSLAVE;
   amaip->numrows = 1;
   sap->type = SAT_MASTERSLAVE;
   ams_prev = ams_head = NULL;
   while (ams)
   {
      ams_master = *ams_mhead;
      n = AlnMgrGetNumRows(ams->sap);
      usethis = FALSE;
      while (!usethis && ams_master != NULL)
      {
         if (ams->sstart == ams_master->sstart && ams->sstop == ams_master->sstop)
            usethis = TRUE;
         else
            ams_master = ams_master->next;
      }
      if (usethis)
      {
         if (ams_prev != NULL && ams->sstart > ams_prev->sstart && ams->stop != 1)
         {
            rsp = rsp_curr_head;
            for (i=0; i<n; i++)
            {
               if (i+1 != ams->n)
               {
                  if (rsp == NULL)
                     return FALSE;
                  rsp->which_saps[ams_master->count-1] = ams->count+1;
                  rsp->num_in_sap[ams_master->count-1] = i+1;
                  rsp = rsp->next;
               }
            }
         } else  /* new row */
         {
            found = FALSE;
            for (i=0; i<n; i++)
            {
               if (i+1 != ams->n)
               {
                  rsp = RowSourceNew();
                  amaip->numrows++;
                  rsp->which_saps = (Uint4Ptr)MemNew(numblocks*sizeof(Uint4));
                  rsp->num_in_sap = (Uint4Ptr)MemNew(numblocks*sizeof(Uint4));
                  rsp->id = AlnMgrGetNthSeqIdPtr(ams->sap, i+1);
                  rsp->strand = AlnMgrGetNthStrand(ams->sap, i+1);
                  rsp->which_saps[ams_master->count-1] = ams->count + 1;
                  rsp->num_in_sap[ams_master->count-1] = i+1;
                  rsp->numsaps = numblocks;
                  rsp_prev->next = rsp;
                  rsp_prev = rsp;
                  if (!found)
                  {
                     rsp_curr_head = rsp;
                     found = TRUE;
                  }
               }
            }
         }
         ams_prev = ams;
      }
      ams = ams->next;
   }
   amaip->master = 1;
   amaip->rowsource = (RowSourcePtr PNTR)MemNew((amaip->numrows)*sizeof(RowSourcePtr));
   rsp = rsp_head;
   for (i=0; i<amaip->numrows; i++)
   {
      if (rsp == NULL)
         return FALSE;
      amaip->rowsource[i] = rsp;
      rsp = rsp->next;
   }
   rsp = rsp_head;
   amaip->numseg = rsp->numsaps;
   amaip->lens = (Int4Ptr)MemNew((amaip->numseg)*sizeof(Int4));
   amaip->aligncoords = (Uint4Ptr)MemNew((amaip->numseg+1)*sizeof(Uint4));
   amaip->aligncoords[0] = 0;
   for (i=0; i<amaip->numseg; i++)
   {
      sap = amaip->saps[rsp->which_saps[i]-1];
      amaip->lens[i] = AlnMgrGetAlnLength(sap, FALSE);
      amaip->aligncoords[i+1] = amaip->aligncoords[i] + amaip->lens[i];
   }
   amaip->parent = sap;
   return TRUE;
}

static Boolean AlnMgrCarefulIndex(SeqAlignPtr sap, AMmsmsPtr ams_head, AMmsmsPtr *ams_mhead, Boolean allinblock, Int4 numrows)
{
   AMAlignIndexPtr  amaip;
   AMmsmsPtr        ams;
   AMmsmsPtr        ams_master;
   AMmsmsPtr        ams_prev;
   AMmsmsPtr        currmaster;
   Boolean          found;
   Int4             i;
   Int4             j;
   Int4             n;
   Int4             numblocks;
   RowSourcePtr     rsp;
   RowSourcePtr     rsp_curr_head;
   RowSourcePtr     rsp_head;
   RowSourcePtr     rsp_prev;
   SeqAlignPtr      PNTR saparray;
   Boolean          usethis;

   if (sap == NULL || ams_head == NULL || ams_mhead == NULL)
      return FALSE;
   if (!AlnMgrIndexParentSA(sap))
      return FALSE;
   AlnMgrSetMaster(sap, sap->master);
   if (allinblock)
      am_trim_master(ams_mhead, ams_head, numrows);
   ams_master = *ams_mhead;
   if (ams_master == NULL) /* these alignments do not overlap at all */
   {
      return (am_make_null_alignment(sap));
   }
   numblocks = 0;
   while (ams_master != NULL)
   {
      numblocks++;
      ams_master->count = numblocks;
      ams_master = ams_master->next;
   }
   ams = ams_head;
   j = 0;
   while (ams)
   {
      j++;
      ams = ams->next;
   }
   saparray = (SeqAlignPtr PNTR)MemNew(j*sizeof(SeqAlignPtr));
   ams = ams_head;
   j = 0;
   currmaster = *ams_mhead;
   rsp_head = RowSourceNew();
   rsp_head->which_saps = (Uint4Ptr)MemNew(numblocks*sizeof(Uint4));
   rsp_head->num_in_sap = (Uint4Ptr)MemNew(numblocks*sizeof(Uint4));
   rsp_head->id = SeqIdDup(sap->master);
   rsp_head->strand = Seq_strand_plus;
   rsp_head->numsaps = 0;
   while (ams)
   {
      if (currmaster && currmaster->sstart < ams->sstart)
      {
         while (currmaster && currmaster->sstart < ams->sstart)
         {
            currmaster = currmaster->next;
         }
         if (currmaster == NULL)
            currmaster = *ams_mhead;
      } else if (currmaster && currmaster->sstart > ams->sstart)
         currmaster = *ams_mhead;
      if (currmaster && currmaster->sstart == ams->sstart && currmaster->sstop == ams->sstop && rsp_head->numsaps < numblocks && rsp_head->which_saps[currmaster->count-1] == 0)
      {
         rsp_head->which_saps[currmaster->count-1] = j+1;
         rsp_head->num_in_sap[currmaster->count-1] = ams->n;
         rsp_head->numsaps++;
         if (rsp_head->numsaps < numblocks)
            currmaster = *ams_mhead;
      }
      saparray[j] = ams->sap;
      ams->count = j;
      j++;
      ams_prev = ams;
      ams = ams->next;
      if (!allinblock)
         am_compare_alignids(ams_prev, ams);
   }
   rsp_prev = rsp_head;
   ams = ams_head;
   amaip = (AMAlignIndexPtr)(sap->saip);
   amaip->numsaps = amaip->alnsaps = j;
   amaip->saps = saparray;
   amaip->mstype = AM_SEGMENTED_MASTERSLAVE;
   amaip->numrows = 1;
   sap->type = SAT_MASTERSLAVE;
   ams_prev = ams_head = NULL;
   while (ams)
   {
      ams_master = *ams_mhead;
      n = AlnMgrGetNumRows(ams->sap);
      usethis = FALSE;
      while (!usethis && ams_master != NULL)
      {
         if (ams->sstart == ams_master->sstart && ams->sstop == ams_master->sstop)
            usethis = TRUE;
         else
            ams_master = ams_master->next;
      }
      if (usethis)
      {
         if (ams_prev != NULL && ams->sstart > ams_prev->sstart && ams->stop != 1)
         {
            rsp = rsp_curr_head;
            for (i=0; i<n; i++)
            {
               if (i+1 != ams->n)
               {
                  if (rsp == NULL)
                     return FALSE;
                  rsp->which_saps[ams_master->count-1] = ams->count+1;
                  rsp->num_in_sap[ams_master->count-1] = i+1;
                  rsp = rsp->next;
               }
            }
         } else  /* new row */
         {
            found = FALSE;
            for (i=0; i<n; i++)
            {
               if (i+1 != ams->n)
               {
                  rsp = RowSourceNew();
                  amaip->numrows++;
                  rsp->which_saps = (Uint4Ptr)MemNew(numblocks*sizeof(Uint4));
                  rsp->num_in_sap = (Uint4Ptr)MemNew(numblocks*sizeof(Uint4));
                  rsp->id = AlnMgrGetNthSeqIdPtr(ams->sap, i+1);
                  rsp->strand = AlnMgrGetNthStrand(ams->sap, i+1);
                  rsp->which_saps[ams_master->count-1] = ams->count + 1;
                  rsp->num_in_sap[ams_master->count-1] = i+1;
                  rsp->numsaps = numblocks;
                  rsp_prev->next = rsp;
                  rsp_prev = rsp;
                  if (!found)
                  {
                     rsp_curr_head = rsp;
                     found = TRUE;
                  }
               }
            }
         }
         ams_prev = ams;
      }
      ams = ams->next;
   }
   amaip->master = 1;
   amaip->rowsource = (RowSourcePtr PNTR)MemNew((amaip->numrows)*sizeof(RowSourcePtr));
   rsp = rsp_head;
   for (i=0; i<amaip->numrows; i++)
   {
      if (rsp == NULL)
         return FALSE;
      amaip->rowsource[i] = rsp;
      rsp = rsp->next;
   }
   rsp = rsp_head;
   amaip->numseg = rsp->numsaps;
   amaip->lens = (Int4Ptr)MemNew((amaip->numseg)*sizeof(Int4));
   amaip->aligncoords = (Uint4Ptr)MemNew((amaip->numseg+1)*sizeof(Uint4));
   amaip->aligncoords[0] = 0;
   for (i=0; i<amaip->numseg; i++)
   {
      sap = amaip->saps[rsp->which_saps[i]-1];
      amaip->lens[i] = AlnMgrGetAlnLength(sap, FALSE);
      amaip->aligncoords[i+1] = amaip->aligncoords[i] + amaip->lens[i];
   }
   amaip->parent = sap;
   return TRUE;
}

static Boolean am_make_null_alignment(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   BioseqPtr        bsp;
   Int4             count;
   SeqAlignPtr      salp;
   SeqAlignPtr      salp_tmp;
   SeqIdPtr         sip;

   amaip = (AMAlignIndexPtr)(sap->saip);
   amaip->numsaps = amaip->alnsaps = 0;
   amaip->mstype = AM_NULL;
   amaip->ids = AlnMgrPropagateUpSeqIdPtrs(sap, &(count));
   if (count <= 0)
      return FALSE;
   else
      amaip->numbsqs = count;
   amaip->numrows = 0;
   amaip->numseg = 0;
   sap->type = SAT_MASTERSLAVE;
   sap->dim = amaip->numrows;
   salp = (SeqAlignPtr)(sap->segs);
   sap->segs = NULL;
   while (salp != NULL)
   {
      salp_tmp = salp->next;
      salp->next = NULL;
      SeqAlignFree(salp);
      salp = salp_tmp;
   }
   amaip->ulens = (Int4Ptr)MemNew(sizeof(Int4));
   amaip->ulens[0] = 0;
   sip = amaip->ids;
   while (sip != NULL)
   {
      bsp = BioseqLockById(sip);
      if (bsp->length > amaip->ulens[0])
         amaip->ulens[0] = bsp->length;
      BioseqUnlock(bsp);
      sip = sip->next;
   }
   amaip->parent = sap;
   return TRUE;
}

static void am_trim_master(AMmsmsPtr PNTR ams_mhead, AMmsmsPtr ams_head, Int4 numrows)
{
   AMmsmsPtr  ams;
   AMmsmsPtr  ams_master;
   AMmsmsPtr  ams_prev;
   Int4       n;

   if (ams_mhead == NULL || ams_head == NULL || numrows < 1)
      return;
   ams_master = *ams_mhead;
   ams_prev = NULL;
   while (ams_master)
   {
      ams = ams_head;
      n = 1;
      while (ams)
      {
         if (ams_master->sstart == ams->sstart && ams_master->sstop == ams->sstop)
            n+=ams->count;
         ams = ams->next;
      }
      if (n < numrows)
      {
         if (ams_prev != NULL)
         {
            ams_prev->next = ams_master->next;
            ams_master->next = NULL;
            ams_master->sap = NULL;
            ams_master->sip = NULL;
            MemFree(ams_master);
            ams_master = ams_prev->next;
         } else
         {
            *ams_mhead = ams_master->next;
            ams_master->next = NULL;
            ams_master->sap = NULL;
            ams_master->sip = NULL;
            MemFree(ams_master);
            ams_master = *ams_mhead;
         }
      } else
      {
         ams_prev = ams_master;
         ams_master = ams_master->next;
      }
   }
   return;
}


NLM_EXTERN void AlnMgrMakeAlignCoords(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   Int4             i;
   Int4             j;

   i = AlnMgrCheckAlignForParent(sap);
   if (i < 0 || i==AM_CHILD)
      return;
   amaip = (AMAlignIndexPtr)(sap->saip);
   if (!amaip->saps)
      return;
   amaip->aligncoords = (Uint4Ptr)MemNew((amaip->alnsaps)*sizeof(Uint4));
   amaip->aligncoords[0] = 0;
   for (j=0; j<((amaip->alnsaps)-1); j++)
   {
      amaip->aligncoords[j+1] = AlnMgrGetAlnLength(amaip->saps[j], FALSE) + amaip->aligncoords[j];
   }
   return;
}

NLM_EXTERN Boolean AlnMgrMergeIntoMSMultByMaster(AMAlignIndexPtr amaip, Int4Ptr lens, Uint4Ptr numseg)
{
   Uint4          count;
   DenseSegPtr    dsp;
   Int4           gap;
   Int4           i;
   Int4           j;
   Int4           n;
   Boolean        retval;
   SAIndexPtr     saip;
   AMTinyInfoPtr  tip;
   AMTinyInfoPtr  PNTR tiparray;

   retval = FALSE;
   if (numseg == NULL)
      return retval;
   tiparray = (AMTinyInfoPtr PNTR)MemNew((*numseg+1)*sizeof(AMTinyInfoPtr));
   j = 0;
   count = 0;
   for (i=0; i<(amaip->alnsaps); i++)
   {
      dsp = (DenseSegPtr)(amaip->saps[i]->segs);
      saip = (SAIndexPtr)amaip->saps[i]->saip;
      gap = 0;
      for (n=0; n<(dsp->numseg); n++)
      {
         if (dsp->starts[n*(dsp->dim)+saip->master-1] != -1)
         {
            tip = (AMTinyInfoPtr)MemNew(sizeof(AMTinyInfo));
            tip->start = dsp->starts[n*(dsp->dim)+saip->master-1];
            tip->which = i+1;
            tip->numgap = gap;
            tiparray[j] = tip;
            j++;
            count++;
            gap = 0;
         } else
         {
            gap++;
         }
      }
      tip = (AMTinyInfoPtr)MemNew(sizeof(AMTinyInfo));
      AlnMgrGetNthSeqRangeInSA(amaip->saps[i], saip->master, NULL, &tip->start);
      tip->start += 1;
      tip->which = i+1;
      tip->numgap = gap;
      tiparray[j] = tip;
      j++;
      count++;
   }
   *numseg = count;
   HeapSort((Pointer)tiparray, (size_t)(*numseg), sizeof(AMTinyInfoPtr), AlnMgrCompareTips);
   *numseg = count-1;
   count = 0;
   for (i=0; i<=(*numseg); i++)
   {
      if (count!=0 && (tiparray[i]->start == lens[count-1])) 
         count--;
      for (j=1; j<=(tiparray[i]->numgap); j++)
      {
         lens[count] = -(tiparray[i]->which);
         count++;
      }
      lens[count] = tiparray[i]->start;
      count++;  
   }
   for (i=0; i<=(*numseg); i++)
   {
      MemFree(tiparray[i]);
   }
   MemFree(tiparray);
   *numseg = count;
   return TRUE;
}

NLM_EXTERN Boolean AlnMgrMergeSegments(Int4Ptr lens, SeqAlignPtr sap, SeqIdPtr master, Int4Ptr where, Int4 which)
{
   DenseSegPtr  dsp;
   Boolean      found;
   Int4         i;
   Int4         j;
   Int4         n;
   Int4         num;
   Int4         r;
   Boolean      retval;
   Int4         s;
   SAIndexPtr   saip;
   Int4Ptr      tmp;
   Int4         z;

   retval = FALSE;
   if (!sap || !master || !lens)
      return retval;
   if (!where)
      return retval;
   n = AlnMgrGetNForSip(sap, master);
   if (n<0)
      return retval;
   if (sap->segtype == SAS_DENSEG)
   {
      dsp = (DenseSegPtr)(sap->segs);
      if (!dsp)
         return retval;
      saip = (SAIndexPtr)(sap->saip);
      if (!saip)
         return retval;
   } else
   {
      return retval;
   }
   if (*where == 0)
   {
      for(j=0; j<(dsp->numseg); j++)
      {
         if (dsp->starts[(j*(dsp->dim)) + n - 1] < 0)
         {
            s = -(which);
         } else
         {
            s = dsp->starts[(j*(dsp->dim)) + n - 1];
         }
         lens[*where] = s;
         *where = *where + 1;
      }
      AlnMgrGetNthSeqRangeInSA(sap, saip->master, NULL, &lens[dsp->numseg]);
      lens[dsp->numseg] += 1;
      *where = *where + 1;
   } else
   {
      tmp = (Int4Ptr)MemNew((dsp->numseg+1)*sizeof(Int4));
      for(j=0; j<(dsp->numseg); j++)
      {
         if (dsp->starts[(j*(dsp->dim)) + n - 1] < 0)
         {
            s = -(which);
         } else
         {
            s = dsp->starts[(j*(dsp->dim)) + n - 1];
         }
         tmp[j] = s;
      }
      AlnMgrGetNthSeqRangeInSA(sap, saip->master, NULL, &tmp[dsp->numseg]);
      tmp[dsp->numseg] += 1;
      s = 0;
      for (j=0; j<=(dsp->numseg); j++)
      {
         num = 0;
         while (tmp[j] < 0 && num<(dsp->numseg))
         {
            num++;
            j++;
         }
         num++;
         found = FALSE;
         for (i=s; !found && i<*where; i++)
         {
            r = 0;
            if (lens[i] < 0)
            {
            } else if (tmp[j] < lens[i])
            {
               if (i>0)
               {
                  while (((i-r-1)>=0) && (lens[i-r-1] < 0))
                  {
                     r++;
                  }
               }
               s = i;
               for (z = *where-1; z >= i-r; z--)
               {
                  lens[z+num] = lens[z];
               }
               for (z = num; z > 0; z--)
               {
                  lens[i-r] = tmp[j-z+1];
                  i++;
               }
               found = TRUE;
               *where = *where + num;
            } else if (tmp[j] == lens[i])
            {
               s = i;
               for (z = *where-1; z >= i; z--)
               {
                  lens[z+num-1] = lens[z];
               }
               for (z = num-1; z > 0; z--)
               {
                  lens[i] = tmp[j-z];
                  i++;
               }
               found = TRUE;
               *where = *where + num - 1;
            }
         }
         if (!found)
         {
            s = *where;
            for (z = *where+num-1; z >= *where; z--)
            {
               lens[z+num] = lens[z];
            }
            for (z = num-1; z >= 0; z--)
            {
               lens[i] = tmp[j-z];
               i++;
            }
            found = TRUE;
            *where = *where + num;
         }
      }
      MemFree(tmp);
   }
   retval = TRUE;
   return retval;
}


NLM_EXTERN Boolean AlnMgrFillInStarts(SeqAlignPtr PNTR saparray, Int4Ptr starts, Int4 numseg, Int4Ptr lens, Int4 numsaps, Uint4Ptr aligncoords)
{
   Int4Ptr  alnlen;
   Boolean  done;
   Int4     gap_pos;
   Int4     i;
   Int4     j;
   Int4     length;
   Boolean  retval;

   retval = FALSE;
   for (i=0; i<numsaps; i++)
   {
      gap_pos = 0;
      for (j=0; j<numseg; j++)
      {
         if(lens[j] >= 0)
         {
            starts[(numsaps*j)+i] = AlnMgrGetStartFromMaster(saparray[i], lens[j]);
         } else
         {
            if (lens[j] == -(i+1))
            {
               starts[(numsaps*j)+i] = AlnMgrGetMasterGapStartForSeg(saparray[i], gap_pos, &aligncoords[j]);
               gap_pos += 1;
            } else
            {
               starts[(numsaps*j)+i] = -1;
            }
         }
      }
   }
   if (!AlnMgrReconcileGaps(lens, aligncoords, numseg))
      return retval;
   alnlen = (Int4Ptr)MemNew(numsaps*sizeof(Int4));
   for (i=0; i<numsaps; i++)
   {
      alnlen[i] = AlnMgrGetAlnLength(saparray[i], FALSE);
   }
   for (i=0; i<numsaps; i++)
   {
      length = 0;
      done = FALSE;
      for (j=0; j<numseg; j++)
      {
         if (starts[(numsaps*j)+i] == -2)
         {
            if (length > 0)
            {
               if (lens[j]+length-1 < alnlen[i])
               {
                  starts[(numsaps*j)+i] = length;
                  length += lens[j];
               } else
               {
                  done = TRUE;
               }
            }
         } else if (starts[(numsaps*j)+i] == -1)
         {
            if (length == 0)
               starts[(numsaps*j)+i] = -2;
            else if (done)
               starts[(numsaps*j)+i] = -2;
         } else
         {
            length = starts[(numsaps*j)+i] + lens[j];
         }
      }
   }
   j = 0;
   numseg -= 1;
   done = FALSE;
   if (numseg != 0)
      done = FALSE;
   else
      done = TRUE;
   for (i=(numsaps*(numseg-1)); (!done && i<(numsaps*numseg)); i++)
   {
      if (starts[i] >= 0)
      {
         done = TRUE;
         lens[numseg-1] = alnlen[j]-starts[i];
      }
      else
         j++;
   }
   MemFree(alnlen);
   retval = TRUE;
   return retval;
}

NLM_EXTERN Int4 AlnMgrGetStartFromMaster(SeqAlignPtr sap, Int4 pos)
{
   DenseSegPtr  dsp;
   SAIndexPtr   saip;
   Int4         start;

   saip = (SAIndexPtr)(sap->saip);
   dsp = (DenseSegPtr)(sap->segs);
   start = binary_search_segment_array(saip->ssdp[saip->master-1], pos, dsp->dim, saip->master - 1, (DenseSegPtr)sap->segs);
   if (dsp->starts[(start*dsp->dim)+saip->master-1] != pos)
   {
      return -2;
   } else
   {
      return (saip->aligncoords[start]);
   }
}

NLM_EXTERN Uint4 AlnMgrGetMasterGapStartForSeg(SeqAlignPtr sap, Int4 which_gap, Uint4Ptr aligncoord)
{
   DenseSegPtr  dsp;
   SAIndexPtr   saip;

   saip = (SAIndexPtr)(sap->saip);
   dsp = (DenseSegPtr)(sap->segs);
   if (which_gap >= saip->ssdp[saip->master-1]->numunsect)
   {
      if (aligncoord)
         *aligncoord = dsp->lens[dsp->numseg-1];
      return saip->aligncoords[dsp->numseg-1];
   }
   if (aligncoord)
      *aligncoord = dsp->lens[saip->ssdp[saip->master-1]->unsect[which_gap]];
   return saip->aligncoords[saip->ssdp[saip->master-1]->unsect[which_gap]];
}


NLM_EXTERN Boolean AlnMgrReconcileGaps(Int4Ptr lens, Uint4Ptr aligncoords, Int4 num)
{
   Int4  i;
   Int4  j;
   Int4  r;

   for (i=0; i<num; i++)
   {
      if (lens[i] < 0)
      {
         r = 1;
         while (lens[i+r] < 0)
         {
            r++;
         }
         lens[i] = lens[i+r];
         for (j=i+1; j<num; j++)
         {
            if (lens[j] >= 0)
               lens[j] = lens[j] + aligncoords[i];
         }
      }
   }
   for (i=0; i<num; i++)
   {
      aligncoords[i] = lens[i] - lens[0];
   }
   for (i=0; i<num-1; i++)
   {
      lens[i] = lens[i+1] - lens[i];
   }
   return TRUE;
}

NLM_EXTERN Boolean AlnMgrMakeMultSegments(AMAlignIndexPtr amaip)
{
   Int4      i;
   Int4      j;
   Uint2     n;
   Boolean   retval;
   Uint2Ptr  segments;
   Uint2Ptr  tmp;

   retval = FALSE;
   tmp = (Uint2Ptr)MemNew((amaip->numseg)*sizeof(Uint2));
   for (i=0; i<amaip->numsaps; i++)
   {
      n = 0;
      for (j=0; j<amaip->numseg; j++)
      {
         if (amaip->starts[((amaip->numsaps)*j)+i] >= 0)
         {
            tmp[n] = j;
            n++;
         }
      }
      segments = (Uint2Ptr)MemNew(n*sizeof(Uint2));
      for (j=0; j<n; j++)
      {
         segments[j] = tmp[j];
      }
      if (!amaip->amadp[i])
         return retval;
      amaip->amadp[i]->segments = segments;
      amaip->amadp[i]->numseg = n;
      amaip->amadp[i]->numseg = n;
   }
   MemFree(tmp);
   retval = TRUE;
   return retval;
}

NLM_EXTERN Int4 AlnMgrCheckOrdered(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   Int4             dim;
   DenseSegPtr      dsp;
   Int4             i;
   Int4             n;
   SeqAlignPtr      salp;
   SeqIdPtr         sip;
   SeqIdPtr         sip_prev;
   SeqIdPtr         sip_tmp;
   Int4             start1;
   Int4             start2;
   Int4             stop1;
   Int4             stop2;
   Uint1Ptr         strands;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return CHECK_ERROR;
   amaip = (AMAlignIndexPtr)(sap->saip);
   salp = (SeqAlignPtr)(sap->segs);
   if (salp->next == NULL)  /* only one child alignment */
      return AM_NOTORDERED;
   sip_prev = NULL;
   dim = -1;
   n = 0;
   strands = NULL;
   while (salp)
   {
      n++;
      dsp = (DenseSegPtr)(salp->segs);
      if (dim != -1)
      {
         if (dsp->dim != dim)
            return AM_NOTORDERED;
      } else
         dim = dsp->dim;
      sip = dsp->ids;
      if (sip_prev)
      {
         sip_tmp = sip_prev;
         while (sip && sip_tmp)
         {
            if (SeqIdComp(sip_tmp, sip) != SIC_YES)
               return AM_NOTORDERED;
            sip = sip->next;
            sip_tmp = sip_tmp->next;
         }
         if (sip || sip_tmp)
            return AM_NOTORDERED;
      } else
         sip_prev = sip;
      if (strands)
      {
         if (dsp->strands)
         {
            for (i=0; i<dsp->dim; i++)
            {
               if ((dsp->strands[i] == Seq_strand_minus && strands[i] != Seq_strand_minus) || (strands[i] == Seq_strand_minus && dsp->strands[i] != Seq_strand_minus))
                  return AM_NOTORDERED;
            }
         }
      } else
         strands = dsp->strands;
      salp = salp->next;
   }
   if (amaip->saps)
      MemFree(amaip->saps);
   amaip->saps = (SeqAlignPtr PNTR)MemNew(n*sizeof(SeqAlignPtr));
   amaip->numrows = dim;
   salp = (SeqAlignPtr)(sap->segs);
   n = 0;
   while (salp)
   {
      amaip->saps[n] = salp;
      salp = salp->next;
      n++;
   }
   amaip->numsaps = n;
   AlnMgrSortAlnSetByNthRowPos(sap, 1);
   for (n=0; n<amaip->numsaps-1; n++)
   {
      for (i=0; i<dim; i++)
      {
         AlnMgrGetNthSeqRangeInSA(amaip->saps[n], i+1, &start1, &stop1);
         AlnMgrGetNthSeqRangeInSA(amaip->saps[n+1], i+1, &start2, &stop2);
         if (AlnMgrGetNthStrand(amaip->saps[n], i+1) == Seq_strand_minus)
         {
            if (start1 <= stop2)
               return AM_NOTORDERED;
         } else
         {
            if (stop1 >= start2)
               return AM_NOTORDERED;
         }
      }
   }
   return AM_ORDERED;
}

NLM_EXTERN Int4 AlnMgrCheckOverlapping(SeqAlignPtr sap)
{
   AMAlignDatPtr    amadp;
   AMAlignIndexPtr  amaip;
   Int4             end;
   Int4             c;
   Int4             i;
   Int4             j;
   Int4             n;
   Int4             prevstrand;
   SeqIdPtr         sip;
   Int4             start;
   Int4             stop;
   Uint2            strand;

   i = AlnMgrCheckAlignForParent(sap);
   if (i<0)
      return CHECK_ERROR;
   else if (i==AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)sap->saip;
      if (amaip->numsaps == 1)
         return 1;
      sip = amaip->ids;
      for (j=0; j<(amaip->numbsqs); j++)
      {
         end = -1;
         amadp = amaip->amadp[j];
         prevstrand = -1;
         for (c=0; c<(amadp->numsaps); c++)
         {
            n = AlnMgrGetNForSip(amadp->saps[c], sip);
            strand = AlnMgrGetNthStrand(amadp->saps[c], n);
            if (strand == 0)
            {
               if (prevstrand != -1)
                  strand = prevstrand;
            } else if (prevstrand != -1)
            {
               if (strand != prevstrand)
                  return j;
            } else
               prevstrand = strand;
            AlnMgrGetNthSeqRangeInSA(amadp->saps[c], n, &start, &stop);
            if (strand != Seq_strand_minus)
            {
               if (start <= end && end != -1)
                  return j;
               else
                  end = stop;
            } else
            {
               if (end != -1 && stop >= end)
                  return j;
               else
                  end = start;
            }
         }
         sip = sip->next;
      }
   } else if (i==AM_CHILD)
   {
      return NO_OVERLAP;
   }
   return NO_OVERLAP;
}

static Boolean am_check_gaps(SeqAlignPtr sap)
{
   DenseSegPtr  dsp;
   SeqAlignPtr  salp;

   salp = (SeqAlignPtr)(sap->segs);
   while (salp)
   {
      dsp = (DenseSegPtr)(salp->segs);
      if (dsp->dim > 1)
         return FALSE;
      salp = salp->next;
   }
   return TRUE;
}

/*****************************************************************************
*
*  AlnMgrGetMaxSegments simply adds up the number of segments for each
*  SeqAlign in a linked list, to get the maximum number of segments 
*  for the merge of the list (for memory allocation in AlnMgrMakeFakeMultiple).
*
******************************************************************************/

NLM_EXTERN Int4 AlnMgrGetMaxSegments(SeqAlignPtr sap)
{
   DenseSegPtr  dsp;
   Int4         ernie; /* the running total, also a happy hamster */

   ernie = 0;
   while (sap)
   {
      if (sap->segtype == SAS_DENSEG)
      {
         dsp = (DenseSegPtr)(sap->segs);
         ernie += dsp->numseg;
      } else if (sap->segtype == SAS_STD)
      {
         ernie += 1;
      } else
         return 0;
      sap = sap->next;
      ernie += 1;
   }
   return ernie;
}

/*******************************************************************************
*
*  Row Management functions:
*
*******************************************************************************/
NLM_EXTERN Int4 AlnMgrGetNumRows(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   DenseSegPtr      dsp;

   if (sap == NULL || sap->saip == NULL)
      return -1;
   if (sap->saip->indextype == INDEX_SEGS)
   {
      dsp = (DenseSegPtr)sap->segs;
      if (dsp == NULL)
         return -1;
      return (dsp->dim);
   } else if (sap->saip->indextype == INDEX_PARENT)
   {
      if ((amaip = (AMAlignIndexPtr)sap->saip) == NULL)
         return -1;
      if (amaip->numseg == 0)
         return (amaip->numbsqs);
      if (amaip->numrows)
         return (amaip->numrows);
   }
   return 0;
}

NLM_EXTERN Int4 AlnMgrGetMaxRowsForParentPartial(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   Int4             i;
   Int4             j;
   Int4             max;

   if (sap == NULL || sap->saip == NULL)
      return -1;
   max = -1;
   if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndexPtr)sap->saip;
      for (i=0; i<(amaip->alnsaps); i++)
      {
         j = AlnMgrGetNumRows(amaip->saps[i]);
         if (j==-1)
            return -1;
         if (j>max)
            max = j;
      }
   }
   return max;
}

NLM_EXTERN Boolean AlnMgrMakeRowsForOrdered(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   DenseSegPtr      dsp;
   Int4             i;
   SeqIdPtr         id;
   Int4             n;
   RowSourcePtr     PNTR rowsource;
   RowSourcePtr     rsp;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return FALSE;
   amaip = (AMAlignIndexPtr)(sap->saip);
   rowsource = (RowSourcePtr PNTR)MemNew((amaip->numrows)*sizeof(RowSourcePtr));
   dsp = (DenseSegPtr)(((SeqAlignPtr)(sap->segs))->segs);
   id = dsp->ids;
   for (i=0; i<amaip->numrows; i++)
   {
      rsp = RowSourceNew();
      rsp->id = SeqIdDup(id);
      id = id->next;
      rsp->which_saps = (Uint4Ptr)MemNew((amaip->numsaps)*sizeof(Uint4));
      rsp->num_in_sap = (Uint4Ptr)MemNew((amaip->numsaps)*sizeof(Uint4));
      rsp->numsaps = amaip->numsaps;
      if (dsp->strands)
         rsp->strand = dsp->strands[i];
      for (n=0; n<amaip->numsaps; n++)
      {
         rsp->which_saps[n] = n+1;
         rsp->num_in_sap[n] = i+1;
      }
      rowsource[i] = rsp;
   }
   amaip->rowsource = rowsource;
   amaip->master = -2;
   return TRUE;
}

NLM_EXTERN Boolean AlnMgrGetRowsForPartial(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   Int4             curr;
   DenseSegPtr      dsp;
   Boolean          found;
   Int4             i;
   Int4             j;
   Int4             k;
   Boolean          retval;
   RowSourcePtr     PNTR rowsource;
   RowSourcePtr     rsp;
   SeqAlignPtr      salp;
   SeqIdPtr         sip;

   retval = FALSE;
   if (sap == NULL || sap->saip == NULL)
      return retval;
   if (sap->saip->indextype != INDEX_PARENT)
      return retval;
   if (sap->type != SAT_PARTIAL)
      return retval;
   amaip = (AMAlignIndexPtr)sap->saip;
   i = AlnMgrGetMaxRowsForParentPartial(sap);
   if (i < 0)
      return retval;
   else
      amaip->numrows = i;
   rowsource = (RowSourcePtr PNTR)MemNew((amaip->numrows)*sizeof(RowSourcePtr));
   curr = -1;
   for (i=0; i<(amaip->alnsaps); i++)
   {
      salp = amaip->saps[i];
      dsp = (DenseSegPtr)salp->segs;
      sip = dsp->ids;
      for (j=0; j<(dsp->dim); j++)
      {
         found = FALSE;
         k = 0;
         while (!found && k <= curr)
         {
            if (SeqIdComp(sip, rowsource[k]->id) == SIC_YES)
               found = TRUE;
            else
               k++;
         }
         if (!found)
         {
            curr++;
            rsp = RowSourceNew();
            rsp->which_saps = (Uint4Ptr)MemNew((amaip->alnsaps)*sizeof(Uint4));
            rsp->num_in_sap = (Uint4Ptr)MemNew((amaip->alnsaps)*sizeof(Uint4));
            rowsource[curr] = rsp;
            rsp->id = SeqIdDup(sip);
            rsp->which_saps[rsp->numsaps] = i+1;
            rsp->num_in_sap[rsp->numsaps] = AlnMgrGetNForSip(salp, sip);
            (rsp->numsaps)++;
         } else
         {
            rowsource[k]->which_saps[rowsource[k]->numsaps] = i+1;
            rowsource[k]->num_in_sap[rowsource[k]->numsaps] = AlnMgrGetNForSip(salp, sip);
            (rowsource[k]->numsaps)++;
         }
         sip = sip->next;
      }
   }
   amaip->numrows = curr+1;
   amaip->rowsource = rowsource;
   amaip->master = -2;
   return TRUE;
}

NLM_EXTERN Boolean AlnMgrGetRowsForMasterSlave(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   DenseSegPtr      dsp;
   Int4             i;
   Int4             j;
   Int4             k;
   Boolean          retval;
   RowSourcePtr     PNTR rowsource;
   RowSourcePtr     rsp;
   SAIndexPtr       saip;
   SeqAlignPtr      salp;
   SeqIdPtr         sip;

   retval = FALSE;
   if (sap == NULL || sap->saip == NULL)
      return retval;
   if (sap->saip->indextype != INDEX_PARENT)
      return retval;
   if (sap->type != SAT_MASTERSLAVE)
      return retval;
   amaip = (AMAlignIndexPtr)sap->saip;
   i = 1;
   salp = (SeqAlignPtr)sap->segs;
   while (salp)
   {
      j = AlnMgrGetNumRows(salp);
      if (j < 0)
         return retval;
      else
         i += (j-1); /*don't count the master over and over*/
      salp = salp->next;
   }
   rowsource = (RowSourcePtr PNTR)MemNew((i+1)*sizeof(RowSourcePtr));
   rsp = RowSourceNew();
   rsp->id = SeqIdDup(sap->master);
   rsp->which_saps = (Uint4Ptr)MemNew((amaip->alnsaps)*sizeof(Uint4));
   rsp->num_in_sap = (Uint4Ptr)MemNew((amaip->alnsaps)*sizeof(Uint4));
   rsp->numsaps = amaip->alnsaps;
   rowsource[0] = rsp;
   amaip->numrows = 1;
   for (j=0; j<(amaip->alnsaps); j++)
   {
      salp = amaip->saps[j];
      saip = (SAIndexPtr)salp->saip;
      dsp = (DenseSegPtr)(salp->segs);
      sip = dsp->ids;
      k=1;
      while (sip)
      {
         if (k != saip->master)
         {
            rsp = RowSourceNew();
            rsp->id = SeqIdDup(sip);
            rsp->which_saps = (Uint4Ptr)MemNew(sizeof(Uint4));
            rsp->num_in_sap = (Uint4Ptr)MemNew(sizeof(Uint4));
            rsp->numsaps = 1;
            rsp->which_saps[0] = j+1;
            rsp->num_in_sap[0] = k;
            rowsource[amaip->numrows] = rsp;
            amaip->numrows++;
         } else
         {
            rowsource[0]->which_saps[j] = j+1;
            rowsource[0]->num_in_sap[j] = k;
            amaip->master = 1;
         }
         k++;
         sip = sip->next;
      }
   }
   amaip->rowsource = rowsource;
   return TRUE;
}


/*******************************************************************************
*
*  AlnMgrFindMaster returns the (duplicated) SeqIdPtr of the first bioseq 
*  that is present in every child alignment, unless the sap->master field 
*  is set in the child alignments, in which case that SeqIdPtr is returned 
*  (if it's the same in all children).
*
*******************************************************************************/

NLM_EXTERN SeqIdPtr AlnMgrFindMaster(SeqAlignPtr sap)
{
   AMAlignDatPtr    amadp;
   AMAlignIndexPtr  amaip;
   Boolean          done;
   Int4             i;
   SeqAlignPtr      salp;
   SeqIdPtr         sip;

   i = AlnMgrCheckAlignForParent(sap);
   if (i<0)
      return NULL;
   else if (i==AM_CHILD)
   {
      return SeqIdDup(sap->master);
   } else if (i==AM_PARENT)
   {
      salp = (SeqAlignPtr)(sap->segs);
      sip = NULL;
      done = FALSE;
      while (salp && !done)
      {
         if (salp->master)
            return (SeqIdDup(salp->master));
         salp = salp->next;
      }
      amaip = (AMAlignIndexPtr)(sap->saip);
      sip = amaip->ids;
      for (i=0; i<(amaip->numbsqs); i++)
      {
         amadp = amaip->amadp[i];
         if (!amadp || !sip)
            return NULL;
         else 
         {
            if (amadp->numsaps == amaip->numsaps)
               return (SeqIdDup(sip));
         }
         sip = sip->next;
      }
      return NULL;
   }
   return NULL;
}


/*******************************************************************************
*
*  AlnMgrCheckRealMaster makes sure that the master seqid given appears
*  once and only once in each seqalign in the set if a parent is given,
*  or once and only one in the seqalign if a child is given.
*
*******************************************************************************/
NLM_EXTERN Boolean AlnMgrCheckRealMaster(SeqAlignPtr sap, SeqIdPtr master)
{
   DenseSegPtr  dsp;
   Int4         i;
   Boolean      retval;
   SeqAlignPtr  salp;
   SeqIdPtr     sip;

   retval = FALSE;
   if (!sap || !master)
      return retval;
   if (sap->segtype == SAS_DISC)
   {
      salp = (SeqAlignPtr)sap->segs;
      while (salp)
      {
         dsp = (DenseSegPtr)salp->segs;
         sip = dsp->ids;
         i = 0;
         while (sip)
         {
            if (SeqIdComp(sip, master) == SIC_YES)
            {
               i++;
               if (i > 1)
                  return retval;
            }
            sip = sip->next;
         }
         salp = salp->next;
      }
   } else if (sap->segtype == SAS_DENSEG)
   {
      dsp = (DenseSegPtr)sap->segs;
      sip = dsp->ids;
      i = 0;
      while (sip)
      {
         if (SeqIdComp(sip, master) == SIC_YES)
         {
            i++;
            if (i > 1)
               return retval;
         }
         sip = sip->next;
      }
   }
   return TRUE;
}

NLM_EXTERN Boolean AlnMgrMakeSegmentedMasterSlave(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   AMmsmsPtr        ams;
   AMmsmsPtr        PNTR amsarray;
   AMmsmsPtr        ams_head;
   AMmsmsPtr        ams_master;
   AMmsmsPtr        ams_mtmp;
   AMmsmsPtr        ams_tmp;
   Int4             c;
   Boolean          done;
   DenseSegPtr      dsp;
   Boolean          found;
   Int4             i;
   Int4             j;
   Int4             n;
   Int4             max;
   Boolean          ok;
   RowSourcePtr     rsp;
   Int4             rspnum;
   SAIndexPtr       saip;
   SeqAlignPtr      salp;
   SeqIdPtr         sip;
   AMsiplistPtr     siplist;
   AMsiplistPtr     siplist_new;
   AMsiplistPtr     siplist_tmp;
   Int4             sstart;
   Int4             sstop;
   Int4             start;
   Int4             stop;
   Int4Ptr          tmparray;

   if (sap == NULL)
      return FALSE;
   amaip = (AMAlignIndexPtr)sap->saip;
   if (amaip == NULL)
      return FALSE;
   if (amaip->master < 0)
      return FALSE;
   ams_head = NULL;
   n = 0;
   salp = (SeqAlignPtr)(sap->segs);
   for (i=0; i<(amaip->numsaps); i++)
   {
      amaip->saps[i] = salp;
      saip = (SAIndexPtr)(salp->saip);
      if (saip->master < 0)
         return FALSE;
      AlnMgrGetNthSeqRangeInSA(salp, saip->master, &start, &stop);
      dsp = (DenseSegPtr)salp->segs;
      sip = dsp->ids;
      j = 1;
      while (sip != NULL)
      {
         if (j != saip->master)
         {
            n++;
            ams = (AMmsmsPtr)MemNew(sizeof(AMmsms));
            ams->start = start; 
            ams->stop = stop;
            ams->sap = salp;
            ams->nsap = i+1;
            ams->sip = sip;
            ams->n = j;
            ams->j = i;
            AlnMgrGetNthSeqRangeInSA(salp, j, &sstart, &sstop);
            ams->sstart = sstart;
            ams->sstop = sstop;
            ams->strand = AlnMgrGetNthStrand(salp, j);
            if (ams_head == NULL)
            {
               ams_head = ams_tmp = ams;
            } else
            {
               ams_tmp->next = ams;
               ams_tmp = ams;
            }
         }
         sip = sip->next;
         j++;
      }
      salp = salp->next;
   }
   ams_head = am_sort_ammsms(ams_head, n);
   ams_master = NULL;
   ams = ams_head;
   n = 0;
   while (ams)
   {
      if (ams_master)
      {
         ams_mtmp = ams_master;
         found = FALSE;
         while (!found && ams_mtmp)
         {
            if (ams->start == ams_mtmp->start && ams->stop == ams_mtmp->stop)
            {
               found = TRUE;
               ams->masternum = ams_mtmp->masternum;
               ams_mtmp->count++;
            }
            else
               ams_mtmp = ams_mtmp->next;
         }
         if (!found)
         {
            n++;
            ams_tmp = (AMmsmsPtr)MemNew(sizeof(AMmsms));
            ams_tmp->start = ams->start;
            ams_tmp->stop = ams->stop;
            ams_tmp->sap = ams->sap;
            ams_tmp->nsap = ams->nsap;
            ams_tmp->sip = sap->master;
            ams_tmp->count = 1;
            ams_tmp->masternum = ams->masternum = n;
            saip = (SAIndexPtr)(ams->sap->saip);
            ams_tmp->n = saip->master;
            ams_tmp->next = ams_master;
            ams_tmp->j = ams->n;
            ams_master = ams_tmp;
         }
      } else
      {
         n++;
         ams_tmp = (AMmsmsPtr)MemNew(sizeof(AMmsms));
         ams_tmp->start = ams->start;
         ams_tmp->stop = ams->stop;
         ams_tmp->sap = ams->sap;
         ams_tmp->nsap = ams->nsap;
         ams_tmp->sip = sap->master;
         ams_tmp->count = 1;
         ams_tmp->masternum = ams->masternum = n;
         saip = (SAIndexPtr)(ams->sap->saip);
         ams_tmp->n = saip->master;
         ams_tmp->j = ams->n;
         ams_master = ams_tmp;
      }
      ams = ams->next;
   }
   ams_master = am_sort_masterams(ams_master, n);
   max = c = 0;
   ams = ams_master;
   ams_tmp = NULL;
   amsarray = (AMmsmsPtr PNTR)MemNew((n+1)*sizeof(AMmsmsPtr));
   while (ams)
   {
      amsarray[c] = ams;
      if (ams_tmp)
      {
         if (ams->start <= ams_tmp->stop)
         {
            MemFree(amsarray);
            return FALSE; /* add code here to compress all lines??? */
         }
      }
      max += ams->count;
      c++;
      ams_tmp = ams;
      ams = ams->next;
   }
   amaip->mstype = AM_SEGMENTED_MASTERSLAVE;
   amaip->rowsource = (RowSourcePtr PNTR)MemNew((max+1)*sizeof(RowSourcePtr));
   if (amaip->aligncoords)
      MemFree(amaip->aligncoords);
   amaip->aligncoords = (Uint4Ptr)MemNew((c+1)*sizeof(Uint4));
   amaip->lens = (Int4Ptr)MemNew((c+1)*sizeof(Int4));
   amaip->numseg = c;
   tmparray = (Int4Ptr)MemNew((c+1)*sizeof(Int4));
   ams = ams_master;
   for (j=0; ams && j < c; j++)
   {
      amaip->lens[j] = AlnMgrGetAlnLength(ams->sap, FALSE);
      amaip->aligncoords[j+1] = amaip->aligncoords[j] + amaip->lens[j];
      tmparray[ams->masternum] = j;
      ams = ams->next;
   }
   rsp = RowSourceNew();
   rsp->id = SeqIdDup(ams_master->sip);
   rsp->which_saps = (Uint4Ptr)MemNew(c*sizeof(Uint4));
   rsp->num_in_sap = (Uint4Ptr)MemNew(c*sizeof(Uint4));
   rsp->numsaps = 0;
   ams = ams_master;
   while (ams)
   {
      rsp->which_saps[rsp->numsaps] = ams->nsap;
      rsp->num_in_sap[rsp->numsaps] = ams->n;
      rsp->numsaps++;
      ams = ams->next;
   }
   amaip->rowsource[0] = rsp;
   amaip->numrows = 1;
   siplist = (AMsiplistPtr)MemNew(sizeof(AMsiplist));
   siplist->sip = rsp->id;
   siplist->first_row = 0;
   siplist_tmp = siplist;
   ams = ams_head;
   rsp = RowSourceNew();
   rsp->which_saps = (Uint4Ptr)MemNew(c*sizeof(Uint4));
   rsp->num_in_sap = (Uint4Ptr)MemNew(c*sizeof(Uint4));
   amaip->rowsource[amaip->numrows] = rsp;
   amaip->numrows++;
   while (ams && amaip->numrows <= max)
   {
      if (rsp->id == NULL) /* new rsp */
      {
         rsp->id = SeqIdDup(ams->sip);
         rsp->strand = ams->strand;
         rsp->which_saps[tmparray[ams->masternum]] = ams->nsap;
         rsp->num_in_sap[tmparray[ams->masternum]] = ams->n;
         rsp->numsaps = c;
         rspnum = am_get_first_rsp_for_sip(ams->sip, siplist);
         if (rspnum == -1) /* need to add to seqid list */
         {
            siplist_new = (AMsiplistPtr)MemNew(sizeof(AMsiplist));
            siplist_new->sip = ams->sip;
            siplist_new->first_row = amaip->numrows-1;
            siplist_tmp->next = siplist_new;
            siplist_tmp = siplist_new;
         }
      } else /* some fields already filled -- check for conflicts or new row */
      {
         n = SeqIdComp(rsp->id, ams->sip);
         if (n == SIC_YES && ams->strand == rsp->strand) /* could be same row -- check for conflicts */
         {
            ok = FALSE;
            if (rsp->which_saps[tmparray[ams->masternum]] == 0) /* put in same row */
            {
               done = FALSE;
               i = 0;
               while (!done && i<c)
               {
                  if (rsp->which_saps[i] != 0)
                     done = TRUE;
                  else
                     i++;
               }
               if (done)
               {
                  AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[i] - 1], rsp->num_in_sap[i], &start, &stop);
                  if (ams->strand == Seq_strand_minus)
                  {
                     if (tmparray[ams->masternum] < i)
                     {
                        if (stop >= ams->sstart)
                           ok = FALSE;
                        else
                           ok = TRUE;
                     } else
                     {
                        if (start <= ams->sstop)
                           ok = FALSE;
                        else
                           ok = TRUE;
                     }
                  } else
                  {
                     if (tmparray[ams->masternum] < i)
                     {
                        if (start <= ams->sstop)
                           ok = FALSE;
                        else
                           ok = TRUE; 
                     } else
                     {
                        if (stop >= ams->sstart)
                           ok = FALSE;
                        else
                           ok = TRUE;
                     }
                  }
               }
            }
            if (ok)
            {
               rsp->which_saps[tmparray[ams->masternum]] = ams->nsap;
               rsp->num_in_sap[tmparray[ams->masternum]] = ams->n;
               rsp->numsaps=c;
            } else
            {
               rspnum = am_get_first_rsp_for_sip(ams->sip, siplist);
               if (rspnum == -1) /* make a new row */
               {
                  rsp = RowSourceNew();
                  rsp->strand = ams->strand;
                  rsp->which_saps = (Uint4Ptr)MemNew(c*sizeof(Uint4));
                  rsp->num_in_sap = (Uint4Ptr)MemNew(c*sizeof(Uint4));
                  amaip->rowsource[amaip->numrows] = rsp;
                  amaip->numrows++;
                  rsp->id = SeqIdDup(ams->sip);
                  rsp->which_saps[tmparray[ams->masternum]] = ams->nsap;
                  rsp->num_in_sap[tmparray[ams->masternum]] = ams->n;
                  rsp->numsaps = c;
                  siplist_new = (AMsiplistPtr)MemNew(sizeof(AMsiplist));
                  siplist_new->sip = ams->sip;
                  siplist_new->first_row = amaip->numrows-1;
                  siplist_tmp->next = siplist_new;
                  siplist_tmp = siplist_new;
               } else
               {
                  done = FALSE;
                  while (rspnum < amaip->numrows && !done && SAM_OrderSeqID(ams->sip, amaip->rowsource[rspnum]->id) == 0)
                  {
                     rsp = amaip->rowsource[rspnum];
                     if (rsp->which_saps[tmparray[ams->masternum]] == 0) /* fits here */
                     {
                        done = TRUE;
                        found = FALSE;
                        i = 0;
                        while (!found && i<c)
                        {
                           if (rsp->which_saps[i] != 0)
                              found = TRUE;
                           else
                              i++;
                        }
                        if (found)
                        {
                           AlnMgrGetNthSeqRangeInSA(amaip->saps[rsp->which_saps[i] - 1], rsp->num_in_sap[i], &start, &stop);
                           if (ams->strand == Seq_strand_minus)
                           {
                              if (tmparray[ams->masternum] < i)
                              {
                                 if (stop >= ams->sstart)
                                    ok = FALSE;
                                 else
                                    ok = TRUE;
                              } else
                              {
                                 if (start <= ams->sstop)
                                    ok = FALSE;
                                 else
                                    ok = TRUE;
                              }
                           } else
                           {
                              if (tmparray[ams->masternum] < i)
                              {
                                 if (start <= ams->sstop)
                                    ok = FALSE;
                                 else
                                    ok = TRUE;
                              } else
                              {
                                 if (stop >= ams->sstart)
                                    ok = FALSE;
                                 else
                                    ok = TRUE;
                              }
                           }
                        }
                        if (ok && found)
                        {
                           rsp->which_saps[tmparray[ams->masternum]] = ams->nsap;
                           rsp->num_in_sap[tmparray[ams->masternum]] = ams->n;
                           rsp->numsaps = c;
                        } else
                        {
                           rsp = RowSourceNew();
                           rsp->which_saps = (Uint4Ptr)MemNew(c*sizeof(Uint4));
                           rsp->num_in_sap = (Uint4Ptr)MemNew(c*sizeof(Uint4));
                           rsp->strand = ams->strand;
                           amaip->rowsource[amaip->numrows] = rsp;
                           amaip->numrows++;
                           rsp->id = SeqIdDup(ams->sip);
                           rsp->which_saps[tmparray[ams->masternum]] = ams->nsap;
                           rsp->num_in_sap[tmparray[ams->masternum]] = ams->n;
                           rsp->numsaps = c;
                        }
                     }
                     rspnum++;
                  }
                  if (!done) /* didn't fit */
                  {
                     rsp = RowSourceNew();
                     rsp->which_saps = (Uint4Ptr)MemNew(c*sizeof(Uint4));
                     rsp->num_in_sap = (Uint4Ptr)MemNew(c*sizeof(Uint4));
                     amaip->rowsource[amaip->numrows] = rsp;
                     amaip->numrows++;
                     rsp->id = SeqIdDup(ams->sip);
                     rsp->which_saps[tmparray[ams->masternum]] = ams->nsap;
                     rsp->num_in_sap[tmparray[ams->masternum]] = ams->n;
                     rsp->numsaps=c;
                     rsp->strand = ams->strand;
                  }
               }
            }
         } else  /* make a new row */
         {
            rsp = RowSourceNew();
            rsp->which_saps = (Uint4Ptr)MemNew(c*sizeof(Uint4));
            rsp->num_in_sap = (Uint4Ptr)MemNew(c*sizeof(Uint4));
            amaip->rowsource[amaip->numrows] = rsp;
            amaip->numrows++;
            rsp->id = SeqIdDup(ams->sip);
            rsp->strand = ams->strand;
            rsp->which_saps[tmparray[ams->masternum]] = ams->nsap;
            rsp->num_in_sap[tmparray[ams->masternum]] = ams->n;
            rsp->numsaps=c;
            siplist_new = (AMsiplistPtr)MemNew(sizeof(AMsiplist));
            siplist_new->sip = ams->sip;
            siplist_new->first_row = amaip->numrows-1;
            siplist_tmp->next = siplist_new;
            siplist_tmp = siplist_new;
         }
      }
      ams = ams->next;
   }
   siplist_tmp = siplist;
   while (siplist_tmp)
   {
      siplist_new = siplist_tmp->next;
      siplist_tmp->sip = NULL;
      siplist_tmp->next = NULL;
      MemFree(siplist_tmp);
      siplist_tmp = siplist_new;
   }
   ams = ams_master;
   while (ams)
   {
      ams_tmp = ams->next;
      ams->sap = NULL;
      ams->sip = NULL;
      ams->next = NULL;
      MemFree(ams);
      ams = ams_tmp;
   }
   ams = ams_head;
   while (ams)
   {
      ams_tmp = ams->next;
      ams->sap = NULL;
      ams->sip = NULL;
      ams->next = NULL;
      MemFree(ams);
      ams = ams_tmp;
   }
   MemFree(amsarray);
   MemFree(tmparray);
   amaip->startsize = amaip->numseg;
   amaip->starts = (Int4Ptr)MemNew(amaip->numseg*sizeof(Int4));
   return TRUE;
}

static Int4 am_get_first_rsp_for_sip(SeqIdPtr sip, AMsiplistPtr siplist)
{
   AMsiplistPtr  siplist_tmp;

   if (sip == NULL || siplist == NULL)
      return -1;
   siplist_tmp = siplist;
   while (siplist_tmp)
   {
      if (SeqIdComp(sip, siplist_tmp->sip) == SIC_YES)
      {
         return (siplist_tmp->first_row);
      }
      siplist_tmp = siplist_tmp->next;
   }
   return -1;
}

static AMmsmsPtr am_sort_ammsms(AMmsmsPtr ams_head, Int4 n)
{
   AMmsmsPtr        ams;
   AMmsmsPtr        ams_tmp;
   AMmsmsPtr PNTR   ams_array;
   Int4             i;

   if (ams_head == NULL || n == 0)
      return NULL;
   if (n == 1)
      return ams_head;
   ams_array = (AMmsmsPtr PNTR)MemNew((n+1)*sizeof(AMmsmsPtr));
   ams = ams_head;
   for (i=0; ams!=NULL && i<n; i++)
   {
      ams_array[i] = ams;
      ams = ams->next;
   }
   HeapSort((Pointer)ams_array, (size_t)(n), sizeof(AMmsmsPtr), AlnMgrCompareAMS);
   ams_tmp = NULL;
   for (i=0; i<n; i++)
   {
      if (ams_tmp != NULL)
      {
         ams->next = ams_array[i];
         ams = ams->next;
         ams->next = NULL;
      } else
      {
         ams_tmp = ams = ams_array[i];
         ams_tmp->next = NULL;
      }
   }
   MemFree(ams_array);
   return ams_tmp;
}

NLM_EXTERN int LIBCALLBACK AlnMgrCompareAMS(VoidPtr base, VoidPtr large_son)
{
   AMmsmsPtr  ams1;
   AMmsmsPtr  ams2;
   Int4       r;

   ams1 = *((AMmsmsPtr PNTR) base);
   ams2 = *((AMmsmsPtr PNTR) large_son);
   if (ams1 == NULL || ams2 == NULL)
      return 0;
   r = SAM_OrderSeqID(ams1->sip, ams2->sip);
   if (r == 0)
   {
      if (ams1->sstart < ams2->sstart)
         return -1;
      else
         return 1;
   } else
      return r;
}

static AMmsmsPtr am_sort_masterams(AMmsmsPtr ams_head, Int4 n)
{
   AMmsmsPtr        ams;
   AMmsmsPtr        ams_tmp;
   AMmsmsPtr PNTR   ams_array;
   Int4             i;

   if (ams_head == NULL || n == 0)
      return NULL;
   if (n == 1)
      return ams_head;
   ams_array = (AMmsmsPtr PNTR)MemNew((n+1)*sizeof(AMmsmsPtr));
   ams = ams_head;
   for (i=0; ams!=NULL && i<n; i++)
   {
      ams_array[i] = ams;
      ams = ams->next;
   }
   HeapSort((Pointer)ams_array, (size_t)(n), sizeof(AMmsmsPtr), AlnMgrCompareMasterAMS);
   ams_tmp = NULL;
   for (i=0; i<n; i++)
   {
      if (ams_tmp != NULL)
      {
         ams->next = ams_array[i];
         ams = ams->next;
         ams->next = NULL;
      } else
      {
         ams_tmp = ams = ams_array[i];
         ams_tmp->next = NULL;
      }
   }
   return ams_tmp;
}

NLM_EXTERN int LIBCALLBACK AlnMgrCompareMasterAMS(VoidPtr base, VoidPtr large_son)
{
   AMmsmsPtr  ams1;
   AMmsmsPtr  ams2;

   ams1 = *((AMmsmsPtr PNTR) base);
   ams2 = *((AMmsmsPtr PNTR) large_son);
   if (ams1 == NULL || ams2 == NULL)
      return 0;
   if (ams1->start < ams2->start)
      return -1;
   else if (ams1->start > ams2->start)
      return 1;
   else if (ams1->stop < ams2->stop)
      return -1;
   else if (ams1->j < ams2->j)
      return -1;
   else
      return 1;
}


NLM_EXTERN void AlnMgrSetMaster(SeqAlignPtr sap, SeqIdPtr master)
{
   SAIndexPtr   saip;
   SeqAlignPtr  salp;

   if (sap->segtype != SAS_DISC || !master)
      return;
   sap->master = SeqIdDup(master);
   salp = (SeqAlignPtr)(sap->segs);
   while (salp)
   {
      if (!salp->saip)
         return;
      if (salp->master != NULL)
         SeqIdFree(salp->master);
      salp->master = SeqIdDup(master);
      if (salp->saip != NULL)
      {
         saip = (SAIndexPtr)(salp->saip);
         saip->master = AlnMgrGetNForSip(salp, master);
      }
      salp = salp->next;
   }
   return;
}

NLM_EXTERN void AlnMgrMakeMasterPlus(SeqAlignPtr sap)
{
   DenseSegPtr      dsp;
   Int4             i;
   Int4             master;
   SAIndexPtr       saip;
   SeqAlignPtr      sap_tmp;

   i = AlnMgrCheckAlignForParent(sap);
   if (i==AM_CHILD)
   {
      saip = (SAIndexPtr)(sap->saip);
      if (saip->master < 0)
         return;
      else
         master = saip->master;
      dsp = (DenseSegPtr)(sap->segs);
      if (dsp->strands[saip->master-1] == Seq_strand_minus)
      {
         sap_tmp = sap;
         sap = sap->next;
         sap_tmp->next = NULL;
         sap_tmp = SeqAlignListReverseStrand(sap_tmp);
         if (!AlnMgrIndexSingleChildSeqAlign(sap_tmp))
            return;
         saip = (SAIndexPtr)(sap_tmp->saip);
         saip->master = master;
         sap_tmp->next = sap;
         sap = sap_tmp;
      }
   } else if (i==AM_PARENT)
   {
      sap_tmp = (SeqAlignPtr)(sap->segs);
      while (sap_tmp)
      {
         AlnMgrMakeMasterPlus(sap_tmp);
         sap_tmp = sap_tmp->next;
      }
   }
   return;
}

NLM_EXTERN Boolean AlnMgrForceMasterSlave(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   AMAlignDatPtr    amadp;
   Int4             j;
   Int4             n;

   if (sap == NULL || sap->master == NULL || sap->saip == NULL)
      return FALSE;
   amaip = (AMAlignIndexPtr)sap->saip;
   n = AlnMgrGetNForSip(sap, sap->master);
   if (n < 1)
      return FALSE;
   amadp = amaip->amadp[n-1];
   if (amaip->saps == NULL)
   {
      amaip->saps = (SeqAlignPtr PNTR)MemNew((amaip->numsaps)*sizeof(SeqAlignPtr));
      for (j=0; j<amaip->numsaps; j++)
      {
         amaip->saps[j] = amadp->saps[j];
      }
      n = AlnMgrGetMaxSegments((SeqAlignPtr)(sap->segs));
      amaip->lens = (Int4Ptr)MemNew(n*sizeof(Int4));
      AlnMgrSetMaster(sap, sap->master);
      amaip->numseg = n;
   }
   if (!AlnMgrMergeIntoMSMultByMaster(amaip, amaip->lens, &amaip->numseg))
      return FALSE;
   amaip->startsize = (amaip->numseg)*(amaip->numsaps);
   amaip->starts = (Int4Ptr)MemNew((amaip->numseg)*(amaip->numsaps)*sizeof(Int4));
   amaip->aligncoords = (Uint4Ptr)MemNew((amaip->numseg)*sizeof(Uint4));
   if (!AlnMgrFillInStarts(amadp->saps, amaip->starts, amaip->numseg, amaip->lens, amaip->numsaps, amaip->aligncoords))
      return FALSE;
   if (amaip->numseg > 1)
      amaip->numseg -= 1;
   sap->type = SAT_MASTERSLAVE;
   if (!AlnMgrGetRowsForMasterSlave(sap))
      return FALSE;
   am_fix_empty_columns(sap);
   return TRUE;
}

static void am_fix_empty_columns(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   Boolean          found;
   Int4             i;
   Int4             j;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   amaip = (AMAlignIndexPtr)sap->saip;
   for (i=0; i<amaip->numseg; i++)
   {
      found = FALSE;
      for (j=0; j<amaip->numsaps && !found; j++)
      {
         if (amaip->starts[(i*amaip->numsaps)+j] >= 0)
            found = TRUE;
      }
      if (!found)
      {
         for (j=0; j<amaip->numsaps && !found; j++)
         {
            amaip->starts[(i*amaip->numsaps)+j] = -3;
         }
      }
   }
}

/* a little kludge function to tide us over until the new */
/* alignment manager arrives. */

static SeqAlignPtr am_cleanupsalp(SeqAlignPtr salp)
{
   Int4         badseg;
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_new;
   Boolean      found;
   Int4         i;
   Int4         j;
   Int4         n;

   dsp = (DenseSegPtr)(salp->segs);
   badseg = 0;
   for (i=0; i<dsp->numseg; i++)
   {
      found = FALSE;
      for (j=0; found==FALSE && j<dsp->dim; j++)
      {
         if (dsp->starts[dsp->dim*i + j] != -1)
            found = TRUE;
      }
      if (!found)
         badseg++;
   }
   if (badseg == 0)
      return salp;
   dsp_new = DenseSegNew();
   dsp_new->numseg = dsp->numseg-badseg;
   dsp_new->dim = dsp->dim;
   dsp_new->ids = dsp->ids;
   dsp->ids = NULL;
   dsp_new->lens = (Int4Ptr)MemNew(dsp_new->numseg*sizeof(Int4));
   dsp_new->starts = (Int4Ptr)MemNew(dsp_new->numseg*dsp_new->dim*sizeof(Int4));
   dsp_new->strands = (Uint1Ptr)MemNew(dsp_new->numseg*dsp_new->dim*sizeof(Int4));
   n = 0;
   for (i=0; i<dsp->numseg; i++)
   {
      found = FALSE;
      for (j=0; found==FALSE && j<dsp->dim; j++)
      {
         if (dsp->starts[dsp->dim*i + j] != -1)
            found = TRUE;
      }
      if (found)
      {
         for (j=0; j<dsp->dim; j++)
         {
            dsp_new->starts[dsp->dim*n+j] = dsp->starts[dsp->dim*i+j];
            dsp_new->strands[dsp->dim*n+j] = dsp->strands[dsp->dim*i+j];
         }
         dsp_new->lens[n] = dsp->lens[i];
         n++;
      }
   }
   DenseSegFree(dsp);
   salp->segs = (Pointer)dsp_new;
   return salp;
}

/***************************************************************************
*
*  AlnMgrGetSubAlign returns a flattened multiple or pairwise alignment
*  corresponding to the indexed input alignment.  To get the entire
*  alignment, set from = 0 and to = -1.  (SUBALIGN)
*
***************************************************************************/
NLM_EXTERN SeqAlignPtr AlnMgrGetSubAlign(SeqAlignPtr sap, SeqIdPtr which_master, Int4 from, Int4 to)
{
   AMAlignIndexPtr  amaip;
   AlnMsgPtr        amp;
   Int4             c;
   DenseSegPtr      dsp;
   DenseSegPtr      dsp_orig;
   Int4             i;
   Int4             j;
   Boolean          more;
   Uint4            n;
   Int4             numaln;
   SeqAlignPtr      salp;
   SeqAlignPtr      salp_head;
   SeqAlignPtr      salp_prev;
   SeqAlignPtr      sap_parent;
   SeqIdPtr         sip;
   SeqIdPtr         sip_curr,sip_next;
   SeqIdPtr         sip_prev;
   Int4Ptr          trackarray;

   i = AlnMgrCheckAlignForParent(sap);
   if (i == AM_CHILD)
   {
      salp = SeqAlignNew();
      amp = AlnMsgNew();
      amp->row_num = 1;
      amp->which_master = which_master;
      amp->from_m = from;
      amp->to_m = to;
      dsp_orig = (DenseSegPtr)(sap->segs);
      dsp = DenseSegNew();
      dsp->ids = SeqIdDupList(dsp_orig->ids);
      while ((Boolean) (more = AlnMgrGetNextAlnBit(sap, amp)))
      {
         dsp->numseg++;  /* what to do if a row has all gaps?? */
      }
      dsp->dim = dsp_orig->dim;
      dsp->starts = (Int4Ptr)MemNew((dsp->numseg*dsp_orig->dim)*sizeof(Int4));
      dsp->strands = (Uint1Ptr)MemNew((dsp->numseg*dsp_orig->dim)*sizeof(Int4));
      dsp->lens = (Int4Ptr)MemNew((dsp->numseg)*sizeof(Int4));
      for (j=0; j<dsp_orig->dim; j++)
      {
         amp = AlnMsgReNew(amp);
         amp->row_num = j+1;
         amp->which_master = which_master;
         amp->from_m = from;
         amp->to_m = to;
         c = 0;
         while ((Boolean) (more = AlnMgrGetNextAlnBit(sap, amp)))
         {
            dsp->lens[c] = amp->to_b - amp->from_b + 1;
            if (amp->gap == 0)
               dsp->starts[(c*dsp->dim)+j] = amp->from_b;
            else
               dsp->starts[(c*dsp->dim)+j] = -1;
            c++;
         }
      }
      for (c=0; c<(dsp->dim*dsp->numseg); c++)
      {
         dsp->strands[c] = dsp_orig->strands[c];
      }
      salp->type = SAT_PARTIAL;
      salp->segtype = SAS_DENSEG;
      salp->segs = (Pointer)dsp;
      AlnMsgFree(amp);
      salp = am_cleanupsalp(salp);
      return salp;
   } else if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)(sap->saip);
      if (amaip == NULL)
         return NULL;
      if (sap->type == SAT_MASTERSLAVE && amaip->mstype == AM_MASTERSLAVE)
      {
         salp = SeqAlignNew();
         salp->type = SAT_MASTERSLAVE;
         salp->segtype = SAS_DENSEG;
         salp->dim = amaip->numrows;
         dsp = DenseSegNew();
         dsp->dim = amaip->numrows;
         dsp->numseg = amaip->numseg;
         dsp->starts = (Int4Ptr)MemNew((amaip->numseg+1)*(amaip->numrows)*sizeof(Int4));
         dsp->lens = (Int4Ptr)MemNew((amaip->numseg+1)* sizeof(Int4));
         dsp->strands = (Uint1Ptr)MemNew((amaip->numseg+1)*(amaip->numrows)*sizeof(Uint1));
         sip_curr = NULL;
         for (j=0; j<amaip->numrows; j++)
         {
            sip = AlnMgrGetNthSeqIdPtr(sap, j+1);
            if (sip_curr != NULL)
            {
               sip_prev->next = sip;
               sip_prev = sip;
            } else
               sip_curr = sip_prev = sip;
         }
         dsp->ids = SeqIdDupList(sip_curr);
         amp = AlnMsgNew();
         for (j=0; j<(amaip->numrows); j++)
         {
            if (j == amaip->master - 1)
               salp->master = SeqIdDup(sip_curr);
            sip_next = sip_curr->next;
            SeqIdFree(sip_curr);
            sip_curr = sip_next;
            amp->which_master = which_master;
            amp->from_m = from;
            amp->to_m = to;
            amp->row_num = j + 1;
            more = TRUE;
            n = 0;
            while (more)
            {
               more = AlnMgrGetNextAlnBit(sap, amp);
               if (amp->gap == 0)
               {
                  dsp->starts[n*(dsp->dim) + j] = amp->from_b;
               } else
               {
                  dsp->starts[n*(dsp->dim) + j] = -1;
               }
               if (j == 0)
                  dsp->lens[n] = amp->to_b - amp->from_b + 1;
               dsp->strands[n*(dsp->dim) + j] = amp->strand;
               n++;
            }
            amp = AlnMsgReNew(amp);
         }
         AlnMsgFree(amp);
         salp->segs = (Pointer)dsp;
         salp = am_cleanupsalp(salp);
         return salp;
      } else if (sap->type == SAT_PARTIAL || (sap->type == SAT_MASTERSLAVE && amaip->mstype == AM_SEGMENTED_MASTERSLAVE))
      {
         amp = AlnMsgNew();
         amp->which_master = which_master;
         amp->from_m = from;
         amp->to_m = to;
         amp->row_num = 1;
         trackarray = (Int4Ptr)MemNew((amaip->numseg+1)*sizeof(Int4));
         numaln = 0;
         while ((Boolean) (more = AlnMgrGetNextAlnBit(sap, amp)))
         {
            if (amp->send_space)
            {
               numaln++;
               amp->send_space = FALSE;
            } else
               trackarray[numaln]++;
         }
         salp_head = NULL;
         sip_curr = NULL;
         for (j=0; j<amaip->numrows; j++)
         {
            sip = AlnMgrGetNthSeqIdPtr(sap, j+1);
            if (sip_curr != NULL)
            {
               sip_prev->next = sip;
               sip_prev = sip;
            } else
               sip_curr = sip_prev = sip;
         }
         for (j=0; j<numaln; j++)
         {
            salp = SeqAlignNew();
            if (salp_head != NULL)
            {
               salp_prev->next = salp;
               salp_prev = salp;
            } else
               salp_prev = salp_head = salp;
            salp->type = SAT_PARTIAL;
            salp->segtype = SAS_DENSEG;
            salp->dim = amaip->numrows;
            dsp = DenseSegNew();
            dsp->dim = amaip->numrows;
            dsp->numseg = trackarray[j]+1;
            dsp->starts = (Int4Ptr)MemNew((dsp->dim)*(trackarray[j]+1)*sizeof(Int4));
            dsp->lens = (Int4Ptr)MemNew((dsp->dim)*(trackarray[j]+1)*sizeof(Int4));
            dsp->strands = (Uint1Ptr)MemNew((dsp->dim)*(trackarray[j]+1)*sizeof(Uint1));
            dsp->ids = SeqIdDupList(sip_curr);
            salp->segs = (Pointer)dsp;
         }
         amp = AlnMsgReNew(amp);
         for (j=0; j<(amaip->numrows); j++)
         {
            salp = salp_head;
            dsp = (Pointer)(salp->segs);
            if (j == amaip->master - 1)
               salp->master = SeqIdDup(sip_curr);
            sip_next = sip_curr->next;
            SeqIdFree(sip_curr);
            sip_curr = sip_next;
            amp->which_master = which_master;
            amp->from_m = from;
            amp->to_m = to;
            amp->row_num = j + 1;
            more = TRUE;
            n = 0;
            while ((more = AlnMgrGetNextAlnBit(sap, amp)) == TRUE)
            {
               if (amp->gap == 0)
               {
                  dsp->starts[n*(dsp->dim) + j] = amp->from_b;
               } else
               {
                  dsp->starts[n*(dsp->dim) + j] = -1;
               }
               if (j == 0)
                  dsp->lens[n] = amp->to_b - amp->from_b + 1;
               dsp->strands[n*(dsp->dim) + j] = amp->strand;
               n++;
               if (amp->send_space == TRUE && amp->place != 1)
               {
                  salp = salp->next;
                  dsp = (DenseSegPtr)(salp->segs);
                  amp->send_space = FALSE;
                  n=0;
               }
            }
            amp = AlnMsgReNew(amp);
         }
         MemFree(trackarray);
         AlnMsgFree(amp);
         salp = am_cleanupsalp(salp);
         return salp_head;
      } else if (sap->type == SAT_DIAGS)
      {
         salp = SeqAlignDup(sap);
         return salp;
      }
   }
   return NULL;
}

NLM_EXTERN SeqAlignPtr AlnMgrGetSubAlignSpecial(SeqAlignPtr sap, Int4 master, Int4 from, Int4 to)
{
   AMAlignIndexPtr  amaip;
   AlnMsgPtr        amp1;
   AlnMsgPtr        amp2;
   DenseDiagPtr     ddp_head;
   DenseDiagPtr     ddp_new;
   DenseDiagPtr     ddp_prev;
   Int4             i;
   Boolean          more1;
   Boolean          more2;
   SeqAlignPtr      sap_head;
   SeqAlignPtr      sap_new;
   SeqAlignPtr      sap_prev;
 
   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return NULL;
   amaip = (AMAlignIndexPtr)(sap->saip);
   if (master > amaip->numrows)
      return NULL;
   amp1 = AlnMsgNew();
   amp2 = AlnMsgNew();
   sap_head = sap_prev = NULL;
   for (i=0; i<amaip->numrows; i++)
   {
      if ((i+1)!=master)
      {
         sap_new = SeqAlignNew();
         sap_new->type = SAT_PARTIAL;
         sap_new->segtype = SAS_DENDIAG;
         sap_new->dim = 2;
         amp1 = AlnMsgReNew(amp1);
         amp2 = AlnMsgReNew(amp2);
         amp1->row_num = master;
         amp2->row_num = i+1;
         amp1->from_m = amp2->from_m = from;
         amp1->to_m = amp2->to_m = to;
         ddp_head = ddp_prev = NULL;
         while ((Boolean) (more1 = AlnMgrGetNextAlnBit(sap, amp1)) && (Boolean)(more2 = AlnMgrGetNextAlnBit(sap, amp2)))
         {
            if (amp1->gap == 0 && amp2->gap == 0)
            {
               ddp_new = DenseDiagNew();
               ddp_new->dim = 2;
               ddp_new->starts = (Int4Ptr)MemNew(2*sizeof(Int4));
               ddp_new->len = amp1->to_b - amp1->from_b + 1;
               ddp_new->starts[0] = amp1->from_b;
               ddp_new->starts[1] = amp2->from_b;
               ddp_new->strands = (Uint1Ptr)MemNew(2*sizeof(Uint1));
               ddp_new->strands[0] = amp1->strand;
               ddp_new->strands[1] = amp2->strand;
               ddp_new->id = AlnMgrGetNthSeqIdPtr(sap, master);
               ddp_new->id->next = AlnMgrGetNthSeqIdPtr(sap, i+1);
               if (ddp_head != NULL)
               {
                  ddp_prev->next = ddp_new;
                  ddp_prev = ddp_new;
               } else
                  ddp_head = ddp_prev = ddp_new;
            }
         }
         sap_new->segs = (Pointer)ddp_head;
        /* AlnMgrMergeNeighbors(sap_new); */
         if (sap_head != NULL)
         {
            sap_prev->next = sap_new;
            sap_prev = sap_new;
         } else
            sap_head = sap_prev = sap_new;
      }
   }
   amp1 = AlnMsgFree(amp1);
   amp2 = AlnMsgFree(amp2);
   return sap_head;
}


/********************************************************************************
*
*   viewer and editor management functions
*
********************************************************************************/

NLM_EXTERN SeqAlignPtr AlnMgrCopyIndexedParentSeqAlign(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   AMAlignIndexPtr  amaip_new;
   Boolean          found;
   Int4             i;
   Int4Ptr          orderarray;
   Int4             r;
   SeqAlignPtr      sap_new;
   SeqAlignPtr      sap_tmp;
   SeqAlignPtr      seg_head;
   SeqAlignPtr      seg_new;
   SeqAlignPtr      seg_prev;
   SeqAlignPtr      seg_tmp;

   if (sap->saip == NULL)
      return NULL;
   if (sap->saip->indextype != INDEX_PARENT)
      return NULL;
   amaip = (AMAlignIndexPtr)sap->saip;
   amaip_new = AMAlignIndexNew();
   sap_new = SeqAlignDup(sap);
   sap_new->saip = (SeqAlignIndexPtr)amaip_new;
   amaip_new->indextype = amaip->indextype;
   amaip_new->freefunc = amaip->freefunc;
   amaip_new->mstype = amaip->mstype;
   amaip_new->aligncoords = (Uint4Ptr)MemNew((amaip->numseg+1)*sizeof(Uint4));
   amaip_new->numseg = amaip->numseg;
   amaip_new->lens = (Int4Ptr)MemNew((amaip->numseg+1)*sizeof(Int4));
   for (i=0; i<amaip->numseg; i++)
   {
      amaip_new->aligncoords[i] = amaip->aligncoords[i];
      amaip_new->lens[i] = amaip->lens[i];
   }
   amaip_new->starts = (Int4Ptr)MemNew(amaip->startsize*sizeof(Int4));
   amaip_new->startsize = amaip->startsize;
   MemCpy(amaip_new->starts, amaip->starts, amaip->startsize*sizeof(Int4));
   amaip_new->alnsaps = amaip->alnsaps;
   amaip_new->numsaps = amaip->numsaps;
   amaip_new->ids = SeqIdDupList(amaip->ids);
   amaip_new->numbsqs = amaip->numbsqs;
   amaip_new->rowsource = (RowSourcePtr PNTR)MemNew((amaip->numrows+1)*sizeof(RowSourcePtr));
   for (i=0; i<amaip->numrows; i++)
   {
      amaip_new->rowsource[i] = AlnMgrCopyRowSource(amaip->rowsource[i]);
   }
   amaip_new->numrows = amaip->numrows;
   amaip_new->master = amaip->master;
   seg_head = NULL;
   sap_tmp = (SeqAlignPtr)sap->segs;
   while (sap_tmp != NULL)
   {
      seg_new = SeqAlignDup(sap_tmp);
      if (seg_head != NULL)
      {
         seg_prev->next = seg_new;
         seg_prev = seg_new;
      } else
         seg_head = seg_prev = seg_new;
      sap_tmp = sap_tmp->next;
   }
   sap_new->segs = (Pointer)seg_head;
   i = 0;
   orderarray = (Int4Ptr)MemNew((amaip->numsaps)*sizeof(Int4));
   seg_new = seg_head;
   sap_tmp = (SeqAlignPtr)sap->segs;
   while (sap_tmp != NULL && seg_new != NULL)
   {
      seg_new->saip = AlnMgrCopyIndexesForChildSeqAlign(sap_tmp);
      found = FALSE;
      r = 0;
      while (!found && r < amaip->numsaps)
      {
         if (sap_tmp == amaip->saps[r])
         {
            orderarray[i] = r;
            found = TRUE;
         }
         r++;
      }
      i++;
      seg_new = seg_new->next;
      sap_tmp = sap_tmp->next;
   }
   amaip_new->saps = (SeqAlignPtr PNTR)MemNew((amaip->numsaps+1)*sizeof(SeqAlignPtr));
   seg_tmp = (SeqAlignPtr)sap_new->segs;
   i = 0;
   while (seg_tmp)
   {
      amaip_new->saps[orderarray[i]] = seg_tmp;
      i++;
      seg_tmp = seg_tmp->next;
   }
   sap_tmp = (SeqAlignPtr)sap->segs;
   amaip_new->amadp = (AMAlignDatPtr PNTR)MemNew((amaip->numbsqs+1)*sizeof(AMAlignDatPtr));
   seg_head = (SeqAlignPtr)sap_new->segs;
   for (i=0; i<amaip->numbsqs; i++)
   {
      amaip_new->amadp[i] = AlnMgrCopyamadp(amaip->amadp[i], sap_tmp, seg_head);
   }
   MemFree(orderarray);
   return sap_new;
}

NLM_EXTERN RowSourcePtr AlnMgrCopyRowSource(RowSourcePtr rsp)
{
   Int4          i;
   RowSourcePtr  rsp_new;

   rsp_new = RowSourceNew();
   rsp_new->id = SeqIdDup(rsp->id);
   rsp_new->which_saps = (Uint4Ptr)MemNew((rsp->numsaps+1)*sizeof(Uint4));
   rsp_new->num_in_sap = (Uint4Ptr)MemNew((rsp->numsaps+1)*sizeof(Uint4));
   for (i=0; i<rsp->numsaps; i++)
   {
      rsp_new->which_saps[i] = rsp->which_saps[i];
      rsp_new->num_in_sap[i] = rsp->num_in_sap[i];
   }
   rsp_new->numsaps = rsp->numsaps;
   return rsp_new;
}

NLM_EXTERN AMAlignDatPtr AlnMgrCopyamadp(AMAlignDatPtr amadp, SeqAlignPtr sap_tmp, SeqAlignPtr seg_head)
{
   AMAlignDatPtr  amadp_new;
   Boolean        found;
   Int4           i;
   Int4           j;
   Int4Ptr        orderarray;
   SeqAlignPtr    sap_old;
   SeqAlignPtr    sap_new;

   if (sap_tmp == NULL || amadp == NULL || seg_head == NULL)
      return NULL;
   amadp_new = AMAlignDatNew();
   amadp_new->sip = SeqIdDup(amadp->sip);
   amadp_new->numsaps = amadp->numsaps;
   amadp_new->saps = (SeqAlignPtr PNTR)MemNew((amadp->numsaps+1)*sizeof(SeqAlignPtr));
   orderarray = (Int4Ptr)MemNew((amadp->numsaps+1)*sizeof(Int4));
   sap_old = sap_tmp;
   j = 0;
   while (sap_old)
   {
      i=0;
      found = FALSE;
      while (!found && i<amadp->numsaps)
      {
         if (sap_old == amadp->saps[i])
         {
            orderarray[i] = j;
            found = TRUE;
         }
         i++;
      }
      sap_old = sap_old->next;
      j++;
   }
   for (i=0; i<amadp->numsaps; i++)
   {
      sap_new = seg_head;
      j=0;
      while (j<orderarray[i])
      {
         sap_new = sap_new->next;
         j++;
      }
      amadp_new->saps[i] = sap_new;
   }
   amadp_new->segments = (Uint2Ptr)MemNew((amadp->numseg+1)*sizeof(Uint2));
   for (i=0; i<amadp->numseg; i++)
   {
      amadp_new->segments[i] = amadp->segments[i];
   }
   amadp_new->numseg = amadp->numseg;
   MemFree(orderarray);
   return amadp_new;
}

NLM_EXTERN SeqAlignIndexPtr AlnMgrCopyIndexesForChildSeqAlign(SeqAlignPtr sap)
{
   DenseSegPtr  dsp;
   Int4         i;
   SAIndexPtr   saip;
   SAIndexPtr   saip_new;

   if (sap == NULL || sap->saip == NULL)
      return NULL;
   dsp = (DenseSegPtr)sap->segs;
   saip = (SAIndexPtr)sap->saip;
   saip_new = SAIndexNew();
   saip_new->indextype = saip->indextype;
   saip_new->freefunc = saip->freefunc;
   saip_new->master = saip->master;
   saip_new->aligncoords = (Uint4Ptr)MemNew((dsp->numseg + 1)*sizeof(Uint4));
   for (i=0; i<dsp->numseg; i++)
   {
      saip_new->aligncoords[i] = saip->aligncoords[i];
   }
   saip_new->ssdp = (SASeqDatPtr PNTR)MemNew((dsp->dim+1)*sizeof(SASeqDatPtr));
   for (i=0; i<dsp->dim; i++)
   {
      saip_new->ssdp[i] = AlnMgrCopySASeqDat(saip->ssdp[i]);
   }
   saip_new->numseqs = dsp->dim;
   return (SeqAlignIndexPtr)saip_new;
}

NLM_EXTERN SASeqDatPtr  AlnMgrCopySASeqDat(SASeqDatPtr ssdp)
{
   Int4         i;
   SASeqDatPtr  ssdp_new;

   if (ssdp == NULL)
      return NULL;
   ssdp_new = SASeqDatNew();
   ssdp_new->numsect = ssdp->numsect;
   ssdp_new->numunsect = ssdp->numunsect;
   ssdp_new->sect = (Uint2Ptr)MemNew((ssdp->numsect+1)*sizeof(Uint2));
   for (i=0; i<ssdp->numsect; i++)
   {
      ssdp_new->sect[i] = ssdp->sect[i];
   }
   ssdp_new->unsect = (Uint2Ptr)MemNew((ssdp->numunsect+1)*sizeof(Uint2));
   for (i=0; i<ssdp->numunsect; i++)
   {
      ssdp_new->unsect[i] = ssdp->unsect[i];
   }
   return ssdp_new;
}

NLM_EXTERN SeqAlignPtr AlnMgrCopyAndIndexSingleAlignment(SeqAlignPtr sap)
{
   SeqAlignPtr  sap_new;

   if (sap == NULL)
      return NULL;
   sap_new = SeqAlignDup(sap);
   sap_new->type = SAT_MASTERSLAVE;
   sap_new->saip = AlnMgrCopyIndexesForChildSeqAlign(sap);
   return sap_new;
}

NLM_EXTERN Boolean AlnMgrCopyIndexedParentIntoSap(SeqAlignPtr sap, SeqAlignPtr target)
{
   AMAlignIndexPtr  amaip;
   AMAlignIndexPtr  amaip_new;
   DenseDiagPtr     ddp;
   DenseDiagPtr     ddp_next;
   DenseSegPtr      dsp;
   Boolean          found;
   Int4             i;
   Int4Ptr          orderarray;
   Int4             r;
   SeqAlignPtr      salp;
   SeqAlignPtr      sap_tmp;
   SeqAlignPtr      seg_head;
   SeqAlignPtr      seg_new;
   SeqAlignPtr      seg_prev;
   SeqAlignPtr      seg_tmp;

   if (sap->saip == NULL || target == NULL)
      return FALSE;
   if (sap->saip->indextype != INDEX_PARENT)
      return FALSE;
   AMAlignIndexFree((Pointer)target->saip);
   target->saip = NULL;
   amaip = (AMAlignIndexPtr)sap->saip;
   amaip_new = AMAlignIndexNew();
   target->type = sap->type;
   target->dim = sap->dim;
   target->score = ScoreSetFree(target->score);
   target->score = ScoreDup(sap->score);
   target->master = SeqIdFree(target->master);
   target->master = SeqIdDup(sap->master);
   SeqAlignSetFree(target->next);
   target->next = NULL;
   if (target->segtype == SAS_DISC)
   {
      salp = (SeqAlignPtr)(target->segs);
      SeqAlignSetFree(salp);
   } else if (target->segtype == SAS_DENSEG)
   {
      dsp = (DenseSegPtr)(target->segs);
      DenseSegFree(dsp);
   } else if (target->segtype == SAS_DENDIAG)
   {
      ddp = (DenseDiagPtr)(target->segs);
      while (ddp != NULL)
      {
         ddp_next = ddp->next;
         ddp->next = NULL;
         DenseDiagFree(ddp);
         ddp = ddp_next;
      }
   }
   target->segs = NULL;
   target->segtype = sap->segtype;
   target->saip = (SeqAlignIndexPtr)amaip_new;
   amaip_new->indextype = amaip->indextype;
   amaip_new->freefunc = amaip->freefunc;
   amaip_new->mstype = amaip->mstype;
   amaip_new->aligncoords = (Uint4Ptr)MemNew((amaip->numseg+1)*sizeof(Uint4));
   amaip_new->numseg = amaip->numseg;
   amaip_new->lens = (Int4Ptr)MemNew((amaip->numseg+1)*sizeof(Int4));
   for (i=0; i<amaip->numseg; i++)
   {
      amaip_new->aligncoords[i] = amaip->aligncoords[i];
      amaip_new->lens[i] = amaip->lens[i];
   }
   if (amaip->numseg == 0)
   {
      amaip_new->ulens = (Int4Ptr)MemNew(sizeof(Int4));
      amaip_new->ulens[0] = amaip->ulens[0];
   } else if(amaip->ulens != NULL) {
      amaip_new->ulens = (Int4Ptr)MemNew(sizeof(Int4)*amaip->numseg);
      MemCopy(amaip_new->ulens, amaip->ulens, sizeof(Int4)*amaip->numseg);
   }
      
   if (amaip->starts != NULL)
   {
       amaip_new->starts = (Int4Ptr)MemNew(amaip->startsize*sizeof(Int4));
       amaip_new->startsize = amaip->startsize;
       MemCpy(amaip_new->starts, amaip->starts, amaip->startsize*sizeof(Int4));
   }
   amaip_new->alnsaps = amaip->alnsaps;
   amaip_new->numsaps = amaip->numsaps;
   amaip_new->ids = SeqIdDupList(amaip->ids);
   amaip_new->numbsqs = amaip->numbsqs;
   amaip_new->rowsource = (RowSourcePtr PNTR)MemNew((amaip->numrows+1)*sizeof(RowSourcePtr));
   for (i=0; i<amaip->numrows; i++)
   {
      amaip_new->rowsource[i] = AlnMgrCopyRowSource(amaip->rowsource[i]);
   }
   amaip_new->numrows = amaip->numrows;
   amaip_new->master = amaip->master;
   sap_tmp = (SeqAlignPtr)sap->segs;
   seg_head = NULL;
   while (sap_tmp)
   {
      seg_new = SeqAlignDup(sap_tmp);
      sap_tmp = sap_tmp->next;
      if (seg_head != NULL)
      {
         seg_prev->next = seg_new;
         seg_prev = seg_new;
      } else
         seg_head = seg_prev = seg_new;
   }
   sap_tmp = (SeqAlignPtr)sap->segs;
   i = 0;
   orderarray = (Int4Ptr)MemNew((amaip->numsaps)*sizeof(Int4));
   target->segs = (SeqAlignPtr)seg_head;
   seg_new = seg_head;
   seg_head = NULL;
   while (sap_tmp && seg_new)
   {
      seg_new->saip = AlnMgrCopyIndexesForChildSeqAlign(sap_tmp);
      found = FALSE;
      r = 0;
      while (!found && r < amaip->numsaps)
      {
         if (sap_tmp == amaip->saps[r])
         {
            orderarray[i] = r;
            found = TRUE;
         }
         r++;
      }
      i++;
      seg_new = seg_new->next;
      sap_tmp = sap_tmp->next;
   }
   amaip_new->saps = (SeqAlignPtr PNTR)MemNew((amaip->numsaps+1)*sizeof(SeqAlignPtr));
   seg_tmp = (SeqAlignPtr)target->segs;
   i = 0;
   while (seg_tmp)
   {
      amaip_new->saps[orderarray[i]] = seg_tmp;
      i++;
      seg_tmp = seg_tmp->next;
   }
   sap_tmp = (SeqAlignPtr)sap->segs;
   amaip_new->amadp = (AMAlignDatPtr PNTR)MemNew((amaip->numbsqs+1)*sizeof(AMAlignDatPtr));
   seg_head = (SeqAlignPtr)target->segs;
   for (i=0; i<amaip->numbsqs; i++)
   {
      amaip_new->amadp[i] = AlnMgrCopyamadp(amaip->amadp[i], sap_tmp, seg_head);
   }
   MemFree(orderarray);
   return TRUE;
}


/**********************************************************************
*
*  AlnMgrDeleteChildByPointer removes the specified child seqalign
*  from the set.  Note that this function does not reindex the seqalign;
*  the calling function must do that if the return is TRUE (use 
*  AlnMgrReIndexSeqAlign).
*
**********************************************************************/
NLM_EXTERN Boolean AlnMgrDeleteChildByPointer(SeqAlignPtr parent, SeqAlignPtr child)
{
   Boolean      found;
   Int4         i;
   SeqAlignPtr  salp;
   SeqAlignPtr  salp_head;
   SeqAlignPtr  salp_prev;

   if (parent == NULL || child == NULL)
      return FALSE;
   i = AlnMgrCheckAlignForParent(parent);
   if (i != INDEX_PARENT)
      return FALSE;
   salp_head = salp_prev = NULL;
   salp = (SeqAlignPtr)(parent->segs);
   found = FALSE;
   while (salp && !found)
   {
      if (salp == child)
         found = TRUE;
      else
      {
         if (salp_head)
            salp_prev = salp;
         else
            salp_head = salp_prev = salp;
         salp = salp->next;
      }
   }
   if (!found)
      return FALSE;
   if (salp_head != NULL)
   {
      salp_prev->next = salp->next;
      salp->next = NULL;
      SeqAlignFree(salp);
   } else
   {
      salp_head = salp->next;
      salp->next = NULL;
      SeqAlignFree(salp);
   }
   parent->segs = (Pointer)salp_head;
   return TRUE;
}

static AMBittyPtr AlnMgrFreeBitty(AMBittyPtr abp)
{
    AMBitty *abp_temp;
    while(abp) {
        abp_temp = abp->next;
        MemFree(abp);
        abp = abp_temp;
    }
    return NULL;
}

/**********************************************************************
*
*  AlnMgrDeleteNthRow deletes the specified row from either a parent
*  or a child seqalign.  Note that with a parent seqalign, this may
*  result in the deletion of a child seqalign.  The function does NOT
*  reindex the alignment; the calling function must do that upon a
*  TRUE return (use AlnMgrReIndexSeqAlign).
*
**********************************************************************/
NLM_EXTERN Boolean AlnMgrDeleteNthRow(SeqAlignPtr sap, Int4 row)
{
   AMBittyPtr       abp;
   AMBittyPtr       abp_head;
   AMBittyPtr       abp_next;
   AMBittyPtr       abp_prev;
   AMAlignIndexPtr  amaip;
   DenseSegPtr      dsp;
   DenseSegPtr      dsp_new;
   Boolean          empty;
   Boolean          found;
   Int4             i;
   SeqIdPtr         id;
   Int4             j;
   Int4             n1;
   Int4             n2;
   Int4             numsame;
   Int4             r;
   RowSourcePtr     rsp;
   SAIndexPtr       saip;
   Boolean          same;
   SeqIdPtr         sip;
   SeqIdPtr         sip_head;
   SeqIdPtr         sip_prev;

   if (sap == NULL || row < 1)
      return FALSE;
   abp_head = NULL;
   i = AlnMgrCheckAlignForParent(sap);
   if (i == AM_PARENT)
   {
      amaip = (AMAlignIndexPtr)sap->saip;
      if (amaip == NULL)
         return FALSE;
      if (row > amaip->numrows)
         return FALSE;
      rsp = amaip->rowsource[row-1];
      for (i=0; i<rsp->numsaps; i++)
      {
         if (rsp->which_saps[i] != 0)
         {
            abp = (AMBittyPtr)MemNew(sizeof(AMBitty));
            abp->num1 = rsp->which_saps[i]-1;
            abp->num2 = rsp->num_in_sap[i];
            if (abp_head != NULL)
            {
               abp_prev->next = abp;
               abp_prev = abp;
            } else
               abp_head = abp_prev = abp;
         }
      }
      abp = abp_head;
      abp_prev = NULL;
      while (abp != NULL)
      {
         abp_next = abp->next;
         found = FALSE;
         while (abp_next != NULL && !found)
         {
            if (abp_next->num1 == abp->num1)
               found = TRUE;
            abp_next = abp_next->next;
         }
         if (found)
         {
            if (abp_prev != NULL)
            {
               abp_prev->next = abp->next;
               abp->next = NULL;
               MemFree(abp);
               abp = abp_prev->next;
            } else
            {
               abp_head = abp->next;
               abp->next = NULL;
               MemFree(abp);
               abp = abp_head;
            }
         } else
           abp = abp->next;
      }
      abp = abp_head;
      while (abp != NULL)
      {
         if (AlnMgrGetNumRows(amaip->saps[abp->num1]) > 2)
         {
             if (!AlnMgrDeleteNthRow(amaip->saps[abp->num1], abp->num2)) {
               abp = AlnMgrFreeBitty(abp);
               return FALSE;
             }
         } else
         {
             if (!AlnMgrDeleteChildByPointer(sap, amaip->saps[abp->num1])) {
               abp = AlnMgrFreeBitty(abp);
               return FALSE;
             }
         }
         abp = abp->next;
      }
      abp = AlnMgrFreeBitty(abp);
      return TRUE;
   } else if (i == AM_CHILD)
   {
      saip = (SAIndexPtr)(sap->saip);
      dsp = (DenseSegPtr)(sap->segs);
      if (dsp->dim <=2 || row > dsp->dim)
          return FALSE;
      for (i=0; i<=dsp->numseg-1; i++)
      {
         same = TRUE;
         j = 0;
         empty = TRUE;
         while (j<dsp->dim)
         {
            if (j!=row-1)
            {
               n1 = binary_search_on_uint2_list(saip->ssdp[j]->sect, i, saip->ssdp[j]->numsect);
               if (i != dsp->numseg-1)
                  n2 = binary_search_on_uint2_list(saip->ssdp[j]->sect, i+1, saip->ssdp[j]->numsect);
               else
                  n2 = -2;
               if (n1 != -1)
               {
                  empty = FALSE;
                  n1 = 1;
               }
               if (n2 != -1)
                  n2 = 1;
               if ((n1 != n2) || (dsp->numseg == 1))
                  same = FALSE;
            }
            j++;
         }
         if (empty)
         {
            abp = (AMBittyPtr)MemNew(sizeof(AMBitty));
            abp->num1 = i;
            abp->num2 = 1;
            if (abp_head != NULL)
            {
               abp_prev->next = abp;
               abp_prev = abp;
            } else
               abp_head = abp_prev = abp;
         } else if (same)
         {
            abp = (AMBittyPtr)MemNew(sizeof(AMBitty));
            abp->num1 = i;
            if (abp_head != NULL)
            {
               abp_prev->next = abp;
               abp_prev = abp;
            } else
               abp_head = abp_prev = abp;
         }
      }
      abp = abp_head;
      numsame = 0;
      empty = FALSE;
      while (abp != NULL)
      {
         numsame++;
         abp = abp->next;
      }
      dsp_new = DenseSegNew();
      dsp_new->numseg = dsp->numseg - numsame;
      dsp_new->dim = dsp->dim - 1;
      dsp_new->starts = (Int4Ptr)MemNew((dsp_new->numseg)*(dsp_new->dim)*sizeof(Int4));
      dsp_new->strands = (Uint1Ptr)MemNew((dsp_new->numseg)*(dsp_new->dim)*sizeof(Uint1));
      dsp_new->lens = (Int4Ptr)MemNew((dsp_new->numseg)*sizeof(Int4));
      abp = abp_head;
      j = i = 0;
      while (i<dsp->numseg)
      {
         n1 = 0;
         empty = FALSE;
         if (abp != NULL)
         {
            if (abp->num1 == i && abp->num2 == 1)
            {
               empty = TRUE;
               abp = abp->next;
            } else if (abp->num1 == i)
            {
               n1 = 1;
               while (abp->next != NULL && abp->next->num1 == abp->num1 + 1)
               {
                  n1++;
                  abp = abp->next;
               }
               abp = abp->next;
            }
         }
         if (!empty)
         {
            n2 = 0;
            for (r=0; r<dsp->dim; r++)
            {
               if (r != row - 1)
               {
                  dsp_new->starts[j*(dsp_new->dim) + n2] = dsp->starts[i*(dsp->dim) + r];
                  dsp_new->strands[j*(dsp_new->dim) + n2] = dsp->strands[i*(dsp->dim) + r];
                  n2++;
               }
            }
            for (r=0; r<=n1; r++)
            {
               dsp_new->lens[j] += dsp->lens[i+r];
            }
            j++;
         }
         i = i + n1 + 1;
      }
      id = dsp->ids;
      sip_head = NULL;
      for (j=0; j<dsp->dim; j++)
      {
         if (j != row-1)
         {
            sip = SeqIdDup(id);
            if (sip_head != NULL)
            {
               sip_prev->next = sip;
               sip_prev = sip;
            } else
               sip_head = sip_prev = sip;
         }
         id = id->next;
      }
      dsp_new->ids = sip_head;
      DenseSegFree(dsp);
      sap->segs = (Pointer)dsp_new;
      sap->dim = dsp_new->dim;
      SAIndexFree(sap->saip);
      sap->saip = NULL;
      abp = abp_head;
      abp_head = AlnMgrFreeBitty(abp_head);
      return TRUE;
   }
   return FALSE;
}

/*
  AlnMgrDeleteHiddenEx

  Reads the Index, and updates the ->segs SeqAligns according
  to the content of the index. If DeleteSalp then either 
  Frees unused SeqAligns.. OR 
  deletes them from the Object Manager.
  Be careful to have saved the SeqAligns PNTRS before calling this function with
  DeleteSalp=FALSE.. otherwise will have dangling pointers.

  AlnMgrDeleteHidden  calls the EX function with DeleteSalp=TRUE;
  
 */


NLM_EXTERN void AlnMgrDeleteHiddenEx(SeqAlignPtr sap, Boolean UseOM, Boolean DeleteSalp)
{
   AMAlignIndexPtr  amaip;
   AMAlnKeeperPtr   PNTR amkp;
   Int4             i;
   Int4             j;
   SeqAlignPtr      salp;
   SeqAlignPtr      salp_next;

   if (sap == NULL || sap->saip == NULL)
      return;
   if (sap->saip->indextype != INDEX_PARENT)
      return;
   amaip = (AMAlignIndexPtr)(sap->saip);
   if (amaip->numsaps == amaip->alnsaps)
      return;
   amkp = (AMAlnKeeperPtr PNTR)MemNew((amaip->numsaps)*sizeof(AMAlnKeeperPtr));
   salp = (SeqAlignPtr)(sap->segs);
   i = 0;
   while (salp)
   {
      salp_next = salp->next;
      salp->next = NULL;
      amkp[i] = (AMAlnKeeperPtr)MemNew(sizeof(AMAlnKeeper));
      amkp[i]->align = salp;
      amkp[i]->am_delete = TRUE;
      salp = salp_next;
      i++;
   }
   amaip->numsaps = i;  /* should be equal already! */
   for (i=0; i<amaip->numsaps; i++)
   {
      j = 0;
      while (j<amaip->alnsaps && (amkp[i]->am_delete))
      {
         if (amaip->saps[j] == amkp[i]->align)
            amkp[i]->am_delete = FALSE;
         j++;
      }
   }
   for (i=0; i<amaip->numsaps; i++)
   {
      if (amkp[i]->am_delete)
      {
          if(DeleteSalp) {
              if (UseOM)
                  ObjMgrDelete(OBJ_SEQALIGN, (Pointer)(amkp[i]->align));
              else
                  SeqAlignFree(amkp[i]->align);
          }
      }
   }
   for (i=0; i<amaip->alnsaps-1; i++)
   {
      amaip->saps[i]->next = amaip->saps[i+1];
   }
   for (i=0; i<amaip->numsaps; i++)
   {
      MemFree(amkp[i]);
   }
   MemFree(amkp);
   amaip->numsaps = amaip->alnsaps;
   sap->segs = (Pointer)amaip->saps[0];
   AlnMgrReIndexSeqAlign(sap);
   return;
}

NLM_EXTERN void AlnMgrDeleteHidden(SeqAlignPtr sap, Boolean UseOM) {
    AlnMgrDeleteHiddenEx(sap, UseOM, TRUE);
    return;
}


static Int4 am_translate_row_num (AMAlignIndexPtr amaip, Int4 n, Int4 row)
{
   RowSourcePtr  rsp;

   if (amaip == NULL || row == 0)
      return 0;
   rsp = amaip->rowsource[row-1];
   if (n >= rsp->numsaps)
      return 0;
   return (rsp->num_in_sap[n]);
}

static Boolean am_is_consistent(SeqAlignPtr sap, SeqAlignPtr sap_new, Int4Ptr block_num)
{
   AMAlignIndexPtr  amaip;
   Boolean          done;
   DenseSegPtr      dsp;
   DenseSegPtr      dsp_new;
   Int4             i;
   Int4             j;
   Int4             n;
   Int4             m_start;
   Int4             m_stop;
   SeqAlignPtr      sap_tmp;
   SeqIdPtr         sip1;
   SeqIdPtr         sip2;
   Uint1            strand1;
   Uint1            strand2;
   Boolean          switched;
   Int4             u_start;
   Int4             u_stop;

   if (block_num == NULL)
      return FALSE;
   amaip = (AMAlignIndexPtr)(sap->saip);
   dsp = (DenseSegPtr)(sap_new->segs);
   sip2 = dsp->ids;
   for (i=0; i<amaip->numrows; i++)
   {
      sip1 = AlnMgrGetNthSeqIdPtr(sap, i+1);
      if (SAM_OrderSeqID(sip1, sip2) != 0)
         return FALSE;
      SeqIdFree(sip1);
      sip2 = sip2->next;
   }
   for (i=0; i<amaip->numrows; i++)
   {
      strand1 = AlnMgrGetNthStrand(sap, i+1);
      strand2 = dsp->strands[i];
      if ((strand1 == Seq_strand_minus && strand2 != Seq_strand_minus) || (strand2 == Seq_strand_minus && strand1 != Seq_strand_minus))
         return FALSE;
   }
   switched = FALSE;
   for (i=0; i<amaip->numrows && !switched; i++)
   {
      j = am_translate_row_num (amaip, 0, i+1);
      if (j!= i+1)
         switched = TRUE;
   }
   if (switched)
   {
      dsp_new = DenseSegNew();
      dsp_new->dim = dsp->dim;
      dsp_new->numseg = dsp->numseg;
      dsp_new->starts = (Int4Ptr)MemNew((dsp->numseg)*(dsp->dim)*sizeof(Int4));
      dsp_new->strands = (Uint1Ptr)MemNew((dsp->numseg)*(dsp->dim)*(sizeof(Uint1)));
      dsp_new->lens = (Int4Ptr)MemNew(dsp->numseg*sizeof(Int4));
      for (i=0; i<amaip->numrows; i++)
      {
         j = am_translate_row_num (amaip, 0, i+1);
         for (n=0; n<dsp->numseg; n++)
         {
            dsp_new->lens[n] = dsp->lens[n];
            dsp_new->starts[(dsp->dim)*n + i] = dsp->starts[(dsp->dim)*n + j - 1];
            dsp_new->strands[(dsp->dim)*n + i] = dsp->strands[(dsp->dim)*n + j - 1];
         }
      }
      dsp_new->scores = dsp->scores;
      dsp->scores = NULL;
      dsp_new->ids = SeqIdDupList(((DenseSegPtr)(amaip->saps[0]->segs))->ids);;
      DenseSegFree(dsp);
      dsp = dsp_new;
      sap_new->segs = (Pointer)dsp;
      SAIndexFree(sap_new->saip);
      sap_new->saip = NULL;
      AlnMgrIndexSingleChildSeqAlign(sap_new);
   }
   AlnMgrGetNthSeqRangeInSA(sap_new, 1, &m_start, &m_stop);
   if (*block_num < 0) /* adding a block -- make sure it doesn't conflict            */
   {                  /* and that all the new pieces fit the same way into the rows */
      AlnMgrGetNthSeqRangeInSA(sap, 1, &u_start, &u_stop);
      if (m_stop < u_start) /* in left tail */
      {
         for (i=2; i<=amaip->numrows; i++)
         {
            AlnMgrGetNthSeqRangeInSA(sap_new, i, &m_start, &m_stop);
            AlnMgrGetNthRowTail(sap, i, LEFT_TAIL, &u_start, &u_stop, NULL);
            if (m_start < u_start && m_stop > u_stop)
               return FALSE;
         }
      } else if (m_start > u_stop) /* in right tail */
      {
         for (i=2; i<=amaip->numrows; i++)
         {
            AlnMgrGetNthSeqRangeInSA(sap_new, i, &m_start, &m_stop);
            AlnMgrGetNthRowTail(sap, i, RIGHT_TAIL, &u_start, &u_stop, NULL);
            if (m_start < u_start && m_stop > u_stop)
               return FALSE;
         }
      } else /* in the middle somewhere */
      {
         done = FALSE;
         for (i=1; i<amaip->numsaps && !done; i++)
         {
            if (!AlnMgrGetNthUnalignedForNthRow(sap, i, 1, &u_start, &u_stop))
               return FALSE;
            if (m_start >= u_start && m_stop <= u_stop)
            {
               n = i;
               done = TRUE;
            }
         }
         for (i=2; i<=amaip->numrows; i++)
         {
            AlnMgrGetNthSeqRangeInSA(sap_new, i, &m_start, &m_stop);
            AlnMgrGetNthUnalignedForNthRow(sap, n, i, &u_start, &u_stop);
            if (m_start < u_start && m_stop > u_stop)
               return FALSE;
         }
      }
   } else  /* replacing a block -- make sure it only conflicts with replaced */
   {       /* block and that the pieces fit correctly into the rows          */
      if (*block_num == 0) /* no block number specified -- take a guess */
      {
         done = FALSE;
         for (i=0; !done && i<amaip->numsaps; i++)
         {
            sap_tmp = amaip->saps[i];
            AlnMgrGetNthSeqRangeInSA(sap_tmp, 1, &u_start, &u_stop);
            if (m_start <= u_start)
            {
               if (m_stop >= u_start)
               {
                  done = TRUE;
                  n = i+1;
               }
            } else if (m_start > u_start && m_start <= u_stop)
            {
               done = TRUE;
               n = i+1;
            }
         }
         if (!done)
            return FALSE;
         else
            *block_num = n;
      } else
         n = *block_num;
      if (n == 1 && amaip->numsaps <=1)
         return TRUE;
      if (n == 1 && amaip->numsaps > 1) /* first block */
      {
         sap_tmp = amaip->saps[1];
         for (i=2; i<=amaip->numrows; i++)
         {
            strand1 = AlnMgrGetNthStrand(sap_new, i);
            AlnMgrGetNthSeqRangeInSA(sap_new, i, &m_start, &m_stop);
            AlnMgrGetNthSeqRangeInSA(sap_tmp, i, &u_start, &u_stop);
            if (strand1 != Seq_strand_minus)
            {
               if (m_stop >= u_start)
                  return FALSE;
            } else
            {
               if (m_start <= u_stop)
                  return FALSE;
            }
         }
      } else if (n == amaip->numsaps) /* last block */
      {
         sap_tmp = amaip->saps[amaip->numsaps-2];
         for (i=2; i<=amaip->numrows; i++)
         {
            strand1 = AlnMgrGetNthStrand(sap_new, i);
            AlnMgrGetNthSeqRangeInSA(sap_new, i, &m_start, &m_stop);
            AlnMgrGetNthSeqRangeInSA(sap_tmp, i, &u_start, &u_stop);
            if (strand1 != Seq_strand_minus)
            {
               if (m_start <= u_stop)
                  return FALSE;
            } else
            {
               if (m_stop >= u_start)
                  return FALSE;
            }
         }
      } else /* one in the middle */
      {
         for (i=2; i<=amaip->numrows; i++)
         {
            sap_tmp = amaip->saps[n-2];
            strand1 = AlnMgrGetNthStrand(sap_new, i);
            AlnMgrGetNthSeqRangeInSA(sap_new, i, &m_start, &m_stop);
            AlnMgrGetNthSeqRangeInSA(sap_tmp, i, &u_start, &u_stop);
            if (strand1 != Seq_strand_minus)
            {
               if (m_start <= u_stop)
                  return FALSE;
            } else
            {
               if (m_stop >= u_start)
                  return FALSE;
            }
            sap_tmp = amaip->saps[n];
            AlnMgrGetNthSeqRangeInSA(sap_tmp, i, &u_start, &u_stop);
            if (strand1 != Seq_strand_minus)
            {
               if (m_stop >= u_start)
                  return FALSE;
            } else
            {
               if (m_start <= u_stop)
                  return FALSE;
            }
         }
      }
   }
   return TRUE;
}

static Boolean am_is_ok_block(DenseSegPtr dsp)
{
   Boolean   allgap;
   Int4      i;
   Int4      j;
   Int4Ptr   prev;
   SeqIdPtr  sip;
   Uint1Ptr  strands;

   if (dsp == NULL)
      return TRUE;
   sip = dsp->ids;
   for (i=0; i<dsp->dim; i++)
   {
      if (sip == NULL)
         return FALSE;
      sip = sip->next;
   }
   prev = (Int4Ptr)MemNew((dsp->dim)*sizeof(Int4));
   strands = (Uint1Ptr)MemNew((dsp->dim)*sizeof(Uint1));
   for (i=0; i<dsp->dim; i++)
   {
      prev[i] = -2;
      if (dsp->strands != NULL)
         strands[i] = dsp->strands[i];
      else
         strands[i] = Seq_strand_plus;
   }
   for (i=0; i<dsp->numseg; i++)
   {
      allgap = TRUE;
      for (j=0; j<dsp->dim; j++)
      {
         if (dsp->strands != NULL)
         {
            if ((dsp->strands[(dsp->dim)*i + j] == Seq_strand_minus && strands[j] != Seq_strand_minus) || (strands[j] == Seq_strand_minus && dsp->strands[(dsp->dim)*i + j] != Seq_strand_minus))
            {
               MemFree(prev);
               MemFree(strands);
               return FALSE;
            }
         }
         if (dsp->starts[(dsp->dim)*i + j] != -1)
         {
            allgap = FALSE;
            if (prev[j] == -2)
            {
               if (strands[i] != Seq_strand_minus)
                  prev[j] = dsp->starts[(dsp->dim)*i + j] + dsp->lens[i];
               else
                  prev[j] = dsp->starts[(dsp->dim)*i + j];
            } else
            {
               if (strands[i] != Seq_strand_minus)
               {
                  if (dsp->starts[(dsp->dim)*i + j] != prev[j])
                  {
                     MemFree(prev);
                     MemFree(strands);
                     return FALSE;
                  }
                  prev[j] = dsp->starts[(dsp->dim)*i + j] + dsp->lens[i];
               } else
               {
                  if (dsp->starts[(dsp->dim)*i + j] + dsp->lens[i] != prev[j])
                  {
                     MemFree(prev);
                     MemFree(strands);
                     return FALSE;
                  }
                  prev[j] = dsp->starts[(dsp->dim)*i + j];
               }
            }
         }
      }
      if (allgap)
      {
         MemFree(prev);
         MemFree(strands);
         return FALSE;
      }
   }
   for (i=0; i<dsp->dim; i++)
   {
      if (prev[i] == -2) /*row with all gaps*/
      {
         MemFree(prev);
         MemFree(strands);
         return FALSE;
      }
   }
   MemFree(prev);
   MemFree(strands);
   return TRUE;
}

/***************************************************************************
*
*  Both AlnMgrReplaceBlock and AlnMgrAddBlock require a flattened multiple
*  alignment plus a dense-seg structure (with the same number of rows
*  as the multiple alignment.  Discontinuous multiple alignments are fine,
*  and gapped alignments can also be edited; multiple pairwise alignments
*  cannot be edited (first call AlnMgrGetSubAlign to flatten the alignment,
*  or only edit one of the alignments in the set).  If the addition,
*  removal, or replacement of a block causes an unaligned region to 
*  disappear, the functions will merge adjacent blocks to get rid of
*  unaligned regions of length 0.
*
***************************************************************************/
NLM_EXTERN Boolean AlnMgrReplaceBlock(SeqAlignPtr sap, DenseSegPtr new_block, Int4 block_num)
{
   AMAlignIndexPtr  amaip;
   Int4             i;
   SeqAlignPtr      sap_new;
   SeqAlignPtr      sap_head;
   SeqAlignPtr      sap_tmp;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return FALSE;
   if (!(am_is_ok_block(new_block)))
      return FALSE;
   amaip = (AMAlignIndexPtr)(sap->saip);
   if (sap->type != SAT_PARTIAL)
   {
      if (sap->type == SAT_MASTERSLAVE && (amaip->mstype != AM_MASTERSLAVE && amaip->mstype != AM_SEGMENTED_MASTERSLAVE && amaip->mstype != AM_NULL))
         return FALSE;
      else if (sap->type != SAT_MASTERSLAVE)
         return FALSE;
   }
   if (amaip->numseg < block_num)
   {
      if (amaip->numseg == 0) /* making a block in a NULL alignment */
      {
         sap_new = SeqAlignNew();
         sap_new->segtype = SAS_DENSEG;
         sap_new->segs = (Pointer)(new_block);
         sap_new->dim = new_block->dim;
         sap->segs = (Pointer)(sap_new);
         return (AlnMgrReIndexSeqAlign(sap));
      } else
         return FALSE;
   }
   if (new_block != NULL) /* replacing a block -- check for merge */
   {
      if (amaip->numrows != new_block->dim)
      return FALSE;
      sap_new = SeqAlignNew();
      sap_new->segtype = SAS_DENSEG;
      sap_new->segs = (Pointer)(new_block);
      sap_new->dim = new_block->dim;
      AlnMgrIndexSingleChildSeqAlign(sap_new);
      if (!am_is_consistent(sap, sap_new, &block_num))
      {
         sap_new->segs = NULL;
         SeqAlignFree(sap_new);
         return FALSE;
      }
      sap_tmp = amaip->saps[block_num - 1];
      amaip->saps[block_num - 1] = sap_new;
      sap_tmp->next = NULL;
      SeqAlignFree(sap_tmp);
      for (i=0; i<amaip->numsaps-1; i++)
      {
         sap_tmp = amaip->saps[i];
         sap_tmp->next = amaip->saps[i+1];
         sap_tmp->next->next = NULL;
      }
      sap->segs = (Pointer)(amaip->saps[0]);
      if (!AlnMgrReIndexSeqAlign(sap))
         return FALSE;
      /* no auto-merge */
      /* return (am_merge_after_edit(sap)); */
      return TRUE;
   } else  /* delete block */
   {
      if (block_num == 0 || block_num > amaip->numsaps)
         return FALSE;
      sap_tmp = amaip->saps[block_num - 1];
      sap_tmp->next = NULL;
      SeqAlignFree(sap_tmp);
      if (block_num == 1)
      {
         sap_head = amaip->saps[1];
         for (i=1; i<amaip->numsaps-1; i++)
         {
            amaip->saps[i]->next = amaip->saps[i+1];
            amaip->saps[i+1]->next = NULL;
         }
      } else
      {
         sap_head = amaip->saps[0];
         for (i=0; i<block_num-1; i++)
         {
            sap_tmp = amaip->saps[i];
            sap_tmp->next = amaip->saps[i+1];
            sap_tmp->next->next = NULL;
         }

         /* bug fix -- Dave & Lewis 7/20/00 */
         if (block_num == amaip->numsaps)
           sap_tmp->next = NULL;
         /* end of fix */
         else {
           sap_tmp->next = amaip->saps[block_num];
         }
         if (block_num < amaip->numsaps)
            sap_tmp->next->next = NULL;
         for (i=block_num; i<amaip->numsaps-1; i++)
         {
            sap_tmp = amaip->saps[i];
            sap_tmp->next = amaip->saps[i+1];
            sap_tmp->next->next = NULL;
         }
      }
      sap->segs = (Pointer)(sap_head);
      return (AlnMgrReIndexSeqAlign(sap));
   }
}

NLM_EXTERN Boolean AlnMgrAddBlock(SeqAlignPtr sap, DenseSegPtr new_block)
{
   AMAlignIndexPtr  amaip;
   Int4             i;
   SeqAlignPtr      sap_new;
   SeqAlignPtr      sap_tmp;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT || new_block == NULL)
      return FALSE;
   if (!(am_is_ok_block(new_block)))
      return FALSE;
   amaip = (AMAlignIndexPtr)(sap->saip);
   if (sap->type != SAT_PARTIAL)
   {
      if (sap->type == SAT_MASTERSLAVE && (amaip->mstype != AM_MASTERSLAVE && amaip->mstype != AM_SEGMENTED_MASTERSLAVE && amaip->mstype != AM_NULL))
         return FALSE;
      else if (sap->type != SAT_MASTERSLAVE)
         return FALSE;
   }
   sap_new = SeqAlignNew();
   sap_new->segtype = SAS_DENSEG;
   sap_new->segs = (Pointer)(new_block);
   sap_new->dim = new_block->dim;
   AlnMgrIndexSingleChildSeqAlign(sap_new);
   i = -1;
   if (!am_is_consistent(sap, sap_new, &i))
   {
      sap_new->segs = NULL;
      SeqAlignFree(sap_new);
      return FALSE;
   }
   sap_tmp = (SeqAlignPtr)(sap->segs);
   sap_new->next = sap_tmp;
   sap->segs = (Pointer)(sap_new);
   if (!AlnMgrReIndexSeqAlign(sap))
      return FALSE;
   /* no auto-merge */
   /* return (am_merge_after_edit(sap)); */
   return TRUE;
}

static void am_do_merge (AMAlignIndexPtr amaip, Int4 left, Int4 right)
{
   Boolean      consistent;
   DenseSegPtr  dsp;
   DenseSegPtr  dsp1;
   DenseSegPtr  dsp2;
   Int4         i;
   Int4         n;
   Int4         n1;
   SeqAlignPtr  sap1;
   SeqAlignPtr  sap2;
   SeqAlignPtr  sap_new;
   Int4         startseg;

   sap1 = amaip->saps[left];
   sap2 = amaip->saps[right];
   dsp1 = (DenseSegPtr)(sap1->segs);
   dsp2 = (DenseSegPtr)(sap2->segs);
   if (dsp1->dim != dsp2->dim)
      return;
   dsp = DenseSegNew();
   n = dsp1->numseg + dsp2->numseg;
   dsp->dim = dsp1->dim;
   dsp->starts = (Int4Ptr)MemNew((dsp1->dim)*n*sizeof(Int4));
   dsp->lens = (Int4Ptr)MemNew(n*sizeof(Int4));
   dsp->strands = (Uint1Ptr)MemNew((dsp1->dim)*n*sizeof(Uint1));
   dsp->ids = dsp1->ids;
   dsp1->ids = NULL;
   for (i=0; i<amaip->numrows; i++)
   {
      for (n=0; n<dsp1->numseg; n++)
      {
         dsp->lens[n] = dsp1->lens[n];
         dsp->starts[(dsp1->dim)*n + i] = dsp1->starts[(dsp1->dim)*n + i];
         dsp->strands[(dsp1->dim)*n + i] = dsp1->strands[(dsp1->dim)*n + i];
      }
   }
   consistent = TRUE;
   for (i=0; i<amaip->numrows && consistent; i++)
   {
      if (dsp->starts[(dsp1->dim)*dsp1->numseg + i] == -1)
      {
         if (dsp2->starts[(dsp1->dim) + i] != -1)
            consistent = FALSE;
      } else
      {
         if (dsp2->starts[(dsp1->dim) + i] == -1)
            consistent = FALSE;
      }
   }
   startseg = 0;
   if (consistent)
   {
      startseg = 1;
      for (i=0; i<amaip->numrows; i++)
      {
         if (dsp->strands[i] == Seq_strand_minus)
            dsp->starts[(dsp1->dim)*(dsp1->numseg-1)+i] = dsp2->starts[i];
      }
      dsp->lens[dsp1->numseg-1] += dsp2->lens[0];
   }
   for (i=0; i<amaip->numrows; i++)
   {
      for (n=startseg; n<dsp2->numseg; n++)
      {
         n1 = n+dsp1->numseg;
         dsp->lens[n] = dsp2->lens[n];
         dsp->starts[(dsp2->dim)*n1 + i] = dsp2->starts[(dsp2->dim)*n + i];
         dsp->strands[(dsp2->dim)*n1 + i] = dsp2->strands[(dsp2->dim)*n + i];
      }
   }
   dsp->numseg = dsp1->numseg + dsp2->numseg - startseg;
   sap_new = SeqAlignNew();
   sap_new->type = SAT_PARTIAL;
   sap_new->segtype = SAS_DENSEG;
   sap_new->dim = dsp->dim;
   sap_new->segs = (Pointer)(dsp);
   AlnMgrIndexSingleChildSeqAlign(sap_new);
   amaip->saps[left]->next = NULL;
   amaip->saps[right]->next = NULL;
   SeqAlignFree(amaip->saps[left]);
   SeqAlignFree(amaip->saps[right]);
   if (left == 0)
   {
      amaip->saps[left] = sap_new;
      amaip->saps[right] = NULL;
   } else
   {
      amaip->saps[right] = sap_new;
      amaip->saps[left] = NULL;
   }
}

static Boolean am_merge_after_edit (SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;
   Boolean          found;
   Int4             i;
   Int4             j;
   RowSourcePtr     rsp;
   SeqAlignPtr      sap_prev;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return FALSE;
   AlnMgrSetUnalignedLengths(sap);
   amaip = (AMAlignIndexPtr)(sap->saip);
   found = FALSE;
   rsp = amaip->rowsource[0];
   for (i=1; i<amaip->numsaps && !found; i++)
   {
      if (amaip->ulens[i-1] == 0)
      {
         found = TRUE;
         am_do_merge (amaip, rsp->which_saps[i-1]-1, rsp->which_saps[i]-1);
         sap_prev = amaip->saps[0];
         for (j=1; j<amaip->numsaps; j++)
         {
            if (amaip->saps[j] != NULL)
            {
               sap_prev->next = amaip->saps[j];
               sap_prev = amaip->saps[j];
            }
         }
         sap->segs = (Pointer)(amaip->saps[0]);
      }
   }
   if (!found)
      return TRUE;
   if (!AlnMgrReIndexSeqAlign(sap))
      return FALSE;
   return (am_merge_after_edit(sap));
}

/*******************************************************************************

  Function : AlnMgrIsSAPDiscAli()
  
  Purpose : check if a SeqAlign is discontinuous
  
  Parameters : SeqAlignPtr
  
  Return value : TRUE if discontinous, FALSE otherwise

*******************************************************************************/
NLM_EXTERN  Boolean AlnMgrIsSAPDiscAli(SeqAlignPtr sap)
{
AMAlignIndexPtr  amaip;
Boolean          bRet=FALSE;

	if (!sap || !sap->saip) return(bRet);

	if (sap->saip->indextype == INDEX_PARENT){
		amaip = (AMAlignIndexPtr)sap->saip;
		if (sap->type == SAT_PARTIAL || (sap->type == SAT_MASTERSLAVE && 
			amaip->mstype == AM_SEGMENTED_MASTERSLAVE)){
			bRet=TRUE;
		}
	}
	
	return(bRet);
}

NLM_EXTERN Boolean AlnMgrIsSAPNULL(SeqAlignPtr sap)
{
   AMAlignIndexPtr  amaip;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return FALSE;
   amaip = (AMAlignIndexPtr)(sap->saip);
   if (amaip->mstype == AM_NULL)
       return TRUE;
   if (sap->segs == NULL)
      return TRUE;
   return FALSE;
}

static int LIBCALLBACK am_compare_diags(VoidPtr ptr1, VoidPtr ptr2)
{
   DenseDiagPtr  ddp1;
   DenseDiagPtr  ddp2;

   ddp1 = *((DenseDiagPtr PNTR) ptr1);
   ddp2 = *((DenseDiagPtr PNTR) ptr2);
   if (ddp1 == NULL || ddp2 == NULL)
      return 0;
   if (ddp1->starts[0] < ddp2->starts[0])
      return -1;
   else if (ddp1->starts[0] > ddp2->starts[0])
      return 1;
   else
      return 0;
}

NLM_EXTERN Int4 AlnMgrIsIBMable(SeqAlignPtr sap)
{
   Boolean       changed;
   Int4          dim;
   DenseDiagPtr  ddp;
   DenseDiagPtr  PNTR ddparray;
   DenseSegPtr   dsp;
   Int4          i;
   Int4          j;
   SeqIdPtr      ids;
   SeqAlignPtr   salp;

   if (sap == NULL)
      return AM_ERROR;
   if (sap->segtype == SAS_DISC)
      salp = (SeqAlignPtr)(sap->segs);
   else
      salp = sap;
   changed = FALSE;
   while (salp != NULL)
   {
      if (salp->segtype == SAS_DISC)
         return AM_NOIBM;
      else if (salp->segtype == SAS_DENSEG)
      {
         dsp = (DenseSegPtr)(salp->segs);
         if (dsp->numseg > 1)
            return AM_NOIBM;
      } else if (salp->segtype == SAS_DENDIAG)
      {
         ddp = (DenseDiagPtr)(salp->segs);
         dim = ddp->dim;
         ids = ddp->id;
         i = 0;
         while (ddp != NULL)
         {
            if (ddp->dim != dim)
               return AM_NOIBM;
            if (am_is_new_row(ddp->id, ids))
               return AM_NOIBM;
            i++;
            ddp = ddp->next;
         }
         ddparray = (DenseDiagPtr PNTR)MemNew(i*sizeof(DenseDiagPtr));
         ddp = (DenseDiagPtr)(salp->segs);
         for (j=0; j<i && ddp!=NULL; j++)
         {
            ddparray[j] = ddp;
            ddp = ddp->next;
         }
         HeapSort (ddparray, i, sizeof(DenseDiagPtr), am_compare_diags);
         for (j=0; j<i-1; j++)
         {
            if (ddparray[j]->starts[0]+ddparray[j]->len > ddparray[j+1]->starts[0])
               return AM_NOIBM;
            if (ddparray[j]->next != ddparray[j+1])
               changed = TRUE;
            ddparray[j]->next = ddparray[j+1];
         }
         ddparray[i-1]->next = NULL;
         salp->segs = (Pointer)ddparray[0];
      } else
         return AM_ERROR;
      salp = salp->next;
   }
   if (changed)
      return AM_IBMCHANGE;
   else
      return AM_IBMNOCHANGE;
}

static Boolean am_same_ids(SeqIdPtr sip1, SeqIdPtr sip2)
{
   Boolean   found;
   SeqIdPtr  sip_tmp;
   
   while (sip2 != NULL)
   {
      sip_tmp = sip1;
      found = FALSE;
      while (sip_tmp != NULL && !found)
      {
         if (SeqIdComp(sip_tmp, sip2) == SIC_YES)
            found = TRUE;
         else
            sip_tmp = sip_tmp->next;
      }
      if (!found)
         return FALSE;
      sip2 = sip2->next;
   }
   return TRUE;
}

NLM_EXTERN Int4 AlnMgrIsEditable(SeqAlignPtr sap)
{
   DenseDiagPtr  ddp;
   DenseSegPtr   dsp;
   Boolean       gapped;
   Int4          ibm;
   SeqIdPtr      id_prev;
   SeqAlignPtr   salp;
   
   if (sap == NULL)
      return AM_ERROR;
   if (sap->segtype == SAS_DISC)
      salp = (SeqAlignPtr)(sap->segs);
   else
      salp = sap;
   id_prev = NULL;
   gapped = FALSE;
   while (salp != NULL)
   {
      if (salp->segtype == SAS_DISC)
         return AM_ERROR;
      else if (salp->segtype == SAS_DENSEG)
      {
         dsp = (DenseSegPtr)(salp->segs);
         if (dsp->numseg > 1)
            gapped = TRUE;
         if (am_same_ids(dsp->ids, id_prev))
            return AM_NOEDIT;
         id_prev = dsp->ids;
      }  else if (salp->segtype == SAS_DENDIAG)
      {
         ddp = (DenseDiagPtr)(salp->segs);
         while (ddp != NULL)
         {
            if (am_same_ids(ddp->id, id_prev))
               return AM_NOEDIT;
            id_prev = ddp->id;
            ddp = ddp->next;
         }
      }
      salp = salp->next;
   }
   if (gapped)
      return AM_EDITGAPS;
   else
      return AM_EDITNOGAP;
}

/*
  Function to map the Bioseq Coordinates of one sequence to the
  bioseq coordinate of the base that aligns with it in the other sequence.
  a return value of -2 indicates that the bioseq coordinate is not in the alignment,
  while a return value of -1 indicates that the coordinate maps to a gap.
  if(GetNextNonGap==TRUE) .. then function will try to find the next aligned
  base in the alignment after the gap. In this case the position of the next base
  after the gap is in PostGap.. unless it is and end-gap.. in which case PostGap==-1.
   .. For "-" strand, this would thus return the next aligned block.. thus
      the previous base in the subject sequence.

 */
NLM_EXTERN Int4  AlnMgrMapBioseqToBioseq(SeqAlignPtr salp,Int4 pos,Int4 source_row,Int4 target_row,Boolean GetNextNonGap,Int4Ptr PostGap) {
    Int4 aln_coord,pos_target;
    AlnMsgPtr amp1;
    Boolean gap,status;
    if(!salp)
        return -1;
    aln_coord = AlnMgrMapBioseqToSeqAlign(salp, pos, source_row, NULL);
    if(aln_coord>=0) { /* 
                          If that coord exist in SeqAlign 
                          even gaps are alignment coordinates.
                       */
        pos_target = AlnMgrMapRowCoords(salp, aln_coord, target_row, NULL);
        if(pos_target>=0 || !GetNextNonGap)
            return pos_target;
        else {
            Uint1 gap;
            amp1 = AlnMsgNew();
            amp1->from_m = aln_coord; 
            amp1->to_m = -1;
            amp1->row_num = target_row;
            amp1->which_master = 0; /* align coordinates */
            status = AlnMgrGetNextAlnBit(salp, amp1);
            gap = amp1->gap;
            /*
              Search for 1st aligned base after gap
            */
            
            while(status && amp1->gap) { /* Search for 1st non-gap bit */
                AlnMgrGetNextAlnBit(salp, amp1);
            }
            if(amp1->gap) {
                /*
                  XXX Should only happen for alignment with end-gaps of
                  if there is a bug  in AlnMgrMapBioseqToSeqAlign 
                */
                *PostGap = -1;
                Free(amp1);
                return -1;
            } else {
                *PostGap = amp1->from_b;
                Free(amp1);
                return -1;
            }
        }
    } else
        return aln_coord;
}
   /*
     convert a global densediag alignment into a single denseseg
     no error checking done.
   */
NLM_EXTERN DenseSegPtr DenseDiagToGlobalDenseSeg(DenseDiagPtr ddp_head) {
    DenseDiagPtr ddp;
    DenseSegPtr dsp;
    Int4 numseg=0,dim=0,i,j;

    if(!ddp_head)
        return NULL;

    ddp = ddp_head;
    numseg=0;
    while (ddp) {
        numseg++;
        ddp=ddp->next;
    }

    ddp = ddp_head;
    i=0;
    dim = ddp->dim;
    dsp = DenseSegNew();
    dsp->dim = dim;
    dsp->numseg = numseg;
    dsp->starts = MemNew(dim*numseg*sizeof(Int4));
    dsp->lens = MemNew(numseg*sizeof(Int4));
    dsp->strands = MemNew(dim*numseg*sizeof(Uint1));
    dsp->ids = SeqIdDupList(ddp->id);
    while (ddp) {
        for(j=0;j<dim;j++) 
            dsp->starts[j+i*dim] = ddp->starts[j];
        for(j=0;j<dim;j++) 
            dsp->strands[j+i*dim] = ddp->strands[j];
        dsp->lens[i] = ddp->len;
        ddp = ddp->next;
        i++;
    }
    return dsp;
}
/*
  Make a DenseDiag Linked List out of an Interval on a seqalign 
  (interval specified in alignment coordinates )
 */

NLM_EXTERN DenseDiagPtr AlnMgrSeqAlignToDDP(SeqAlignPtr sap,Int4 aln_cut_from,Int4 aln_cut_to,Int4Ptr numseg_ptr) {
    AlnMsgPtr amp1;
    Int4 numseg=0,curr_m,from_q,from_s,to_q,to_s,len,i;
    DenseDiagPtr ddp,ddp_head=NULL,ddp_last=NULL;
    SeqAlignPtr salp;
    Boolean gap,status;

   i = AlnMgrCheckAlignForParent(sap);
   if (i == AM_PARENT) {
       salp = (SeqAlignPtr) sap->segs;
   } else {
       salp = sap;
   }
   if((i != AM_PARENT || !salp->next) && salp->segtype == SAS_DENSEG) {
           Int4 dim,j;
           DenseSegPtr dsp;
           Int4 alnlen=0,newlen;
           dsp = (DenseSegPtr) salp->segs;
           numseg = dsp->numseg;
           dim = dsp->dim;
           i=0;
           while(i<numseg) {
               len = dsp->lens[i];
               newlen = alnlen+len;
               if(newlen > aln_cut_from && alnlen<=aln_cut_to) {
                   Int4 beglen,endlen;
                   ddp =DenseDiagNew();
                   ddp->starts = MemNew(dim*sizeof(Int4));
                   ddp->strands = MemNew(dim*sizeof(Uint1));
                   for(j=0;j<dim;j++) 
                       ddp->strands[j]=dsp->strands[j+i*dim];
                   if(alnlen<aln_cut_from && newlen>aln_cut_from ) {
                       beglen = aln_cut_from-alnlen;
                   } else
                       beglen = 0;
                   if(alnlen<=aln_cut_to && newlen>aln_cut_to ) {
                       endlen = newlen-aln_cut_to-1;
                   } else
                       endlen =0;
                   ddp->len = len -beglen-endlen;
                   for(j=0;j<dim;j++) {
                       if(dsp->starts[j+i*dim]!=-1) {
                           if(dsp->strands[j]!=Seq_strand_minus)
                               ddp->starts[j]=dsp->starts[j+i*dim]+beglen;
                           else
                               ddp->starts[j]=dsp->starts[j+i*dim]-endlen;
                       } else
                           ddp->starts[j]=-1;
                   }
                   ddp->id = SeqIdDupList(SeqAlignId(salp,0));
                   ddp->dim=dim;
                   if(ddp_head) {
                       ddp_last->next = ddp;
                   } else {
                       ddp_head = ddp;
                   }
                   ddp_last = ddp;
                   alnlen=newlen;
               }
               i++;
           }
   } else if ((i != AM_PARENT || !salp->next) && salp->segtype == SAS_DENDIAG) {
       Int4 dim,j;
       Int4 alnlen=0,newlen;
       DenseDiagPtr ddp0 = (DenseDiagPtr)salp->segs;
       i=0;
       while(ddp0) {
           len = ddp0->len;
           newlen = alnlen+len;
           dim = ddp0->dim;
           if(newlen > aln_cut_from && alnlen<=aln_cut_to) {
               Int4 beglen,endlen;
               ddp =DenseDiagNew();
               ddp->starts = MemNew(dim*sizeof(Int4));
               ddp->strands = MemNew(dim*sizeof(Uint1));
               dim = ddp0->dim;
               len = ddp0->len;
               for(j=0;j<dim;j++) 
                   ddp->strands[j]=ddp0->strands[j];
               
               if(alnlen<aln_cut_from && newlen>aln_cut_from ) {
                   beglen = aln_cut_from-alnlen;
               } else
                   beglen = 0;
               if(alnlen<=aln_cut_to && newlen>aln_cut_to ) {
                   endlen = newlen-aln_cut_to-1;
               } else
                   endlen =0;
               ddp->len = len -beglen-endlen;
               for(j=0;j<dim;j++) {
                   if(ddp0->starts[j]!=-1) {
                       if(ddp0->strands[j]!=Seq_strand_minus)
                           ddp->starts[j]=ddp0->starts[j]+beglen;
                       else
                           ddp->starts[j]=ddp0->starts[j]-endlen;
                   } else
                       ddp->starts[j]=-1;
               }
               
               ddp->id = SeqIdDupList(SeqAlignId(salp,0));
               ddp->dim=dim;
               if(ddp_head) {
                   ddp_last->next = ddp;
               } else {
                   ddp_head = ddp;
               }
               ddp_last = ddp;
               ddp0=ddp0->next;
           }
       }
   } else {
       /* XXX
          this will NOT work for SeqAligns with end gaps due to
          a bug in AlnMgrGetNextAlnBit
       */
        amp1 = AlnMsgNew();
        amp1->from_m = aln_cut_from;
        amp1->to_m = aln_cut_to;
        amp1->row_num = 1;
        amp1->which_master = 0;
        curr_m = 0;
        status = TRUE;
        while(status) {
            status = AlnMgrGetNextAlnBit(salp, amp1);
            gap = amp1->gap;
            from_q = amp1->from_b;
            to_q = amp1->to_b;
            len = to_q-from_q; /* Either sequence of gap length */
            numseg++;
            ddp = DenseDiagNew();
            ddp->len = len;
            ddp->starts = MemNew(2*sizeof(Int4));
            ddp->strands = MemNew(2*sizeof(Uint1));
            ddp->strands[0]=amp1->strand;
            ddp->id = SeqIdDupList(SeqAlignId(salp,0));
            ddp->dim=2;
            if(!gap)
                ddp->starts[0]=from_q;
            else
                ddp->starts[0]=-1;
            if(ddp_head) {
                ddp_last->next = ddp;
            } else {
                ddp_head = ddp;
            }
            ddp_last = ddp;
            curr_m+=len;
        }
        Free(amp1);
        amp1 = AlnMsgNew();
        amp1->from_m = aln_cut_from; 
        amp1->to_m = aln_cut_to;
        amp1->row_num = 2;
        amp1->which_master = 0;
        status = TRUE;
        ddp=ddp_head;
        while(status) {
            status = AlnMgrGetNextAlnBit(salp, amp1);
            gap = amp1->gap;
            if(!gap)
                ddp->starts[1]=amp1->from_b;
            else
                ddp->starts[1]=-1;
            ddp->strands[1]=amp1->strand;
            ddp=ddp->next;
        }
        Free(amp1);
    }

    if(numseg_ptr)
        *numseg_ptr = numseg;
    return ddp_head;
}

/* 
   Merge 3 SeqAligns.
   the master_cut_pos1 = position where the salp_merging SeqAlign starts
   the master_cut_pos2 = position where the salp_merging SeqAlign ends

   Take 3 SeqAligns and makes a single denseseg seqalign.
   If the end-seqaligns are PURE gaps they do not need to be indexed.
   (as the alignment manager doesn't yet deal well with purely gaps alignments,
   a bypass has been coded.. but the last end-gap seqalign has
   to be of the correct length.. as no check will be performed)

*/

NLM_EXTERN SeqAlignPtr AlnMgrMerge3OverlappingSeqAligns(SeqAlignPtr salp1,SeqAlignPtr salp_merging,SeqAlignPtr salp2,Int4 master_cut_pos1, Int4 master_cut_pos2) {
    Int4 diff;
    Int4 i,aln_cut_pos1,aln_cut_pos2;
    Int4 numseg=0,this_numseg;
    SeqIdPtr sip_q,sip_s;
    DenseDiagPtr ddp,ddp_head=NULL,ddp_last,ddp_next;
    DenseSegPtr dsp;
    Boolean gap,sstrand_plus,qstrand_plus;
    SeqAlignPtr sap=NULL;


    if(!salp1)
        return NULL;
    if(salp1) {
        if(salp1->segtype == SAS_DENSEG && (dsp=(DenseSegPtr) salp1->segs)!=NULL && dsp->numseg==1 &&( dsp->starts[0]==-1  ||  dsp->starts[1]==-1 )) {
            ddp_head = DenseDiagNew();
            ddp_head->dim = dsp->dim;
            ddp_head->starts = (Int4Ptr)MemNew((dsp->dim)*sizeof(Int4));
            ddp_head->id = SeqIdDupList(dsp->ids);
            ddp_head->strands = (Uint1Ptr)MemNew((dsp->dim)*sizeof(Uint1));
            ddp_head->scores = ScoreDup(dsp->scores);
            for (i=0; i<dsp->dim; i++)
            {
               ddp_head->starts[i] = dsp->starts[i];
               ddp_head->strands[i] = dsp->strands[i];
            }
            ddp_head->len = dsp->lens[0];
            /* XXX HS Risky.. bug potential.
               trust that beginning-gap-seqalign is correctly computed..
               so don't need to trim according to master_cut_pos1
            */
            numseg=1;
        } else {
            aln_cut_pos1 = AlnMgrMapBioseqToSeqAlign(salp1,master_cut_pos1,1, NULL);
            ddp_head = AlnMgrSeqAlignToDDP(salp1,0,aln_cut_pos1,&numseg);
        }
        ddp_last=ddp_head;
        while(ddp_last!=NULL && ddp_last->next!=NULL) {
            ddp_last = ddp_last->next;
        }
    }
    if(salp_merging) {
        ddp = AlnMgrSeqAlignToDDP(salp_merging,0,AlnMgrGetAlnLength(salp_merging,FALSE)-1,&this_numseg);
        
        numseg+=this_numseg;
        if(ddp_last) {
            ddp_last->next = ddp;
        } else {
            ddp_head = ddp;
        }
        while(ddp_last!=NULL && ddp_last->next!=NULL) {
            ddp_last = ddp_last->next;
        }

    }
    if(salp2) {
        if(salp2->segtype == SAS_DENSEG && (dsp=(DenseSegPtr) salp2->segs)!=NULL && dsp->numseg==1 &&( dsp->starts[0]==-1  ||  dsp->starts[1]==-1 )) {
            ddp = DenseDiagNew();
            ddp->dim = dsp->dim;
            ddp->starts = (Int4Ptr)MemNew((dsp->dim)*sizeof(Int4));
            ddp->id = SeqIdDupList(dsp->ids);
            ddp->strands = (Uint1Ptr)MemNew((dsp->dim)*sizeof(Uint1));
            ddp->scores = ScoreDup(dsp->scores);
            for (i=0; i<dsp->dim; i++)
            {
               ddp->starts[i] = dsp->starts[i];
               ddp->strands[i] = dsp->strands[i];
            }
            ddp->len = dsp->lens[0];
            /* XXX HS Risky.. bug potential.
               trust that end-gap-seqalign is correctly computed..
               so don't need to trim according to master_cut_pos1
            */
            numseg++;
        } else {

            aln_cut_pos2 = AlnMgrMapBioseqToSeqAlign(salp2,master_cut_pos2,1, NULL);
            this_numseg=0;
            ddp= AlnMgrSeqAlignToDDP(salp2,aln_cut_pos2,AlnMgrGetAlnLength(salp2,FALSE)-1,&this_numseg);
            numseg += this_numseg;
        }
    
        if(ddp_last) {
            ddp_last->next = ddp;
        } else {
            ddp_head = ddp;
        }
    }
    ddp = ddp_head;
    if(ddp) {
        qstrand_plus = (ddp->strands[0]!=Seq_strand_minus);
        sstrand_plus = (ddp->strands[1]!=Seq_strand_minus);
    }
    /* Merge consecutive ddp of the same type (e.g. full, gap-subject,query-gap)
       Assuming that Consecutive blocks represent a GLOBAL alignment.
       Except that Gaps may be missing.

       If did error correction...
       By Scanning "left-to-right", insure that only have to worry about
       the interval connecting two consecutived DenseDiags.. 
       the "beginning/start" of the first block is always guaranteed to be OK.
     */
    while(ddp!=NULL && ddp->next!=NULL) {
        if(ddp->starts[0]==-1) {
            if(ddp->next->starts[0]==-1) { 
                /* Merge two consecutive query gaps */
                if(!sstrand_plus) {
                    ddp->starts[1]=ddp->next->starts[1];
                    ddp->len = ddp->next->starts[1]-ddp->starts[1]+ddp->len;
                } else
                    ddp->len = ddp->next->starts[1]-ddp->starts[1]+ddp->next->len;
                ddp_next = ddp->next;
                ddp->next = ddp->next->next;
                DenseDiagFree(ddp_next);
                numseg--;
            } else if (ddp->next->starts[1]==-1) {
                /* alternating gaps  */
            } else { 
                /* query Gap followed by block */
            }
        } else if (ddp->starts[1]==-1) {
            if(ddp->next->starts[0]==-1) {
                /* alternating gaps */
            } else if (ddp->next->starts[1]==-1) {
                /* Merge two consecutive subject gaps */
                ddp->len = ddp->next->starts[0]-ddp->starts[0]+ddp->next->len;
                if(!qstrand_plus) {
                    ddp->starts[0]=ddp->next->starts[0];
                    ddp->len = ddp->len + ddp->next->starts[0] - ddp->starts[0];
                }
                ddp_next = ddp->next;
                ddp->next = ddp->next->next;
                DenseDiagFree(ddp_next);
                numseg--;
            } else {
                /* gap in subject followed by whole block */
            }

        } else {/* whole block, no gaps in first block */
            if(ddp->next->starts[0]==-1) {
                
            } else if (ddp->next->starts[1]==-1) {
                
            } else {
                if(ddp->starts[0]-ddp->starts[1]!=ddp->next->starts[0]-ddp->next->starts[1]) {
                    ErrPostEx(SEV_ERROR,0,0,"Bug in Alignment Merging: Consecutive blocks incompatible \n");
                } else {
                    ddp->len = ddp->len+ddp->next->len;
                    if(!qstrand_plus) {
                        ddp->starts[0]=ddp->next->starts[0];
                    }
                    if(!sstrand_plus) {
                        ddp->starts[1]=ddp->next->starts[1];
                    }
                    ddp_next = ddp->next;
                    ddp->next = ddp_next->next;
                    DenseDiagFree(ddp_next);
                    numseg--;
                }
            }
        }
        ddp = ddp->next;
    }

    if(ddp_head) {
        dsp = DenseDiagToGlobalDenseSeg(ddp_head);
        sap = SeqAlignNew();
        sap->type = SAT_PARTIAL;
        sap->segtype = SAS_DENSEG;
        sap->dim = dsp->dim;
        sap->segs = (Pointer)dsp;
        if (dsp->scores)
            sap->score = ScoreDup(dsp->scores);
        while(ddp_head) {
            ddp = ddp_head;
            ddp_head = ddp_head->next;
            DenseDiagFree(ddp);
        }
    }
    return sap;
}
