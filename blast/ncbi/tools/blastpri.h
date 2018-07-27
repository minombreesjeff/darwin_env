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

File name: blastpri.h

Author: Tom Madden

Contents: prototypes for "private" BLAST functions, these should not be called
	by outside utilities. 

******************************************************************************/

/* $Revision: 6.112 $ 
* $Log: blastpri.h,v $
* Revision 6.112  2004/03/31 17:58:51  papadopo
* Mike Gertz' changes for length adjustment calculations
*
* Revision 6.111  2004/01/16 23:43:43  dondosha
* No more need for special argument for partial search: it is set in options
*
* Revision 6.110  2004/01/06 22:36:24  dondosha
* Added BLAST_HSPFree function that frees the edit block and the HSP
*
* Revision 6.109  2003/08/20 22:12:56  dondosha
* Added BlastPrintTabularResults with an extra boolean parameter for OOF alignments
*
* Revision 6.108  2003/08/04 16:19:16  dondosha
* Added effective HSP length (length adjustment) to other returns, so it can be reported in XML output
*
* Revision 6.107  2003/04/22 21:52:13  dondosha
* Added function OOFBlastHSPGetNumIdentical
*
* Revision 6.106  2003/03/24 19:42:14  madden
* Changes to support query concatenation for blastn and tblastn
*
* Revision 6.105  2002/11/04 22:47:02  dondosha
* Added prototype for BlastHSPGetNumIdentical
*
* Revision 6.104  2002/08/30 15:50:06  dondosha
* Several prototypes moved from mblast.h
*
* Revision 6.103  2002/08/01 20:45:35  dondosha
* Changed prototype of the BLASTPostSearchLogic function to make it
* more convenient
*
* Revision 6.102  2002/07/09 16:09:42  camacho
* Changed interface to BlastCreateVirtualOIDList
*
* Revision 6.101  2002/06/26 00:56:29  camacho
*
* 1. Fixed bug when searching a mixture of real and mask databases.
* 2. Clean up of code that calculates the number of sequences and database
*    length.
*
* Revision 6.100  2002/06/25 19:46:07  camacho
* Added PROTO around function declaration
*
* Revision 6.99  2002/06/25 19:39:38  camacho
* Made BlastCreateVirtualOIDList public for use by neighboring software
*
* Revision 6.98  2002/06/21 21:43:01  camacho
* Removed obsolete BlastSeqIdList structure and functions
*
* Revision 6.97  2002/06/19 22:50:33  dondosha
* Added all queries information for tabular output with multiple queries
*
* Revision 6.96  2002/06/11 14:44:48  dondosha
* Return status from some functions instead of search block pointer
*
* Revision 6.95  2002/04/18 16:18:20  dondosha
* Added BlastPrintTabulatedResultsEx with extra argument to keep track of progress
*
* Revision 6.94  2002/04/15 20:42:05  jianye
* Added getFastaStyleTitle(BioseqPtr bsp)
*
* Revision 6.93  2002/03/26 16:46:41  madden
* Move calculation of effective lengths to BlastCalculateEffectiveLengths
*
* Revision 6.92  2002/03/06 18:34:32  dondosha
* Pass the filtered locations back from the megablast engine to use in formatting
*
* Revision 6.91  2002/03/05 17:58:57  dondosha
* Set same offsets for the traceback as for preliminary extension for megablast with non-greedy extensions
*
* Revision 6.90  2002/01/04 20:16:12  dondosha
* Correction for single strand blastx with OOF gapping
*
* Revision 6.89  2001/11/13 18:17:27  dondosha
* Added BlastNtWordUngappedExtend for use in Mega BLAST
*
* Revision 6.88  2001/10/12 15:22:49  dondosha
* Added prototype for BLASTPostSearchLogic
*
* Revision 6.87  2001/09/07 14:46:44  dondosha
* Roll back removal of threshold_first from functions and structures
*
* Revision 6.86  2001/09/06 20:24:34  dondosha
* Removed threshold_first
*
* Revision 6.85  2001/08/20 21:13:41  dondosha
* Added two declarations for clustering hits
*
* Revision 6.84  2001/07/31 16:42:40  dondosha
* Added function FastaCheckDna
*
* Revision 6.83  2001/06/15 16:38:46  dondosha
* Correction to previous changes
*
* Revision 6.82  2001/06/14 22:09:15  dondosha
* Rearranged code for gi lists and oid masks processing to get rid of duplication
*
* Revision 6.81  2001/06/13 21:42:19  dondosha
* Added prototypes for functions needed to deal with gi lists
*
* Revision 6.80  2001/06/07 19:30:03  dondosha
* Pass believe query argument to BlastPrintTabulatedResults
*
* Revision 6.79  2001/06/06 21:22:43  dondosha
* Added (query) Bioseq and SeqLoc arguments to function BlastPrintTabulatedResults
*
* Revision 6.78  2001/05/11 22:04:42  dondosha
* Added prototype for BlastPrintTabulatedResults function
*
* Revision 6.77  2001/03/19 18:54:50  madden
* Added BlastSeqLocFillDoubleIntEx, changed BlastSeqLocFillDoubleIntRev
*
* Revision 6.76  2001/02/08 20:38:30  dondosha
* Added prototype for BLASTHspToStdSeg
*
* Revision 6.75  2001/02/07 21:08:33  dondosha
* 1. Added prototypes of callback functions to handle results
* 2. Two private auxiliary functions made public
*
* Revision 6.74  2001/01/23 20:25:43  dondosha
* 1. Renamed BlastParceInputString to BlastParseInputString
* 2. Recognize a double quoted string as an option value in
*    BlastParseInputString
*
* Revision 6.73  2000/12/19 18:39:50  madden
* Add function BlastSetUserErrorString and BlastDeleteUserErrorString
*
* Revision 6.72  2000/11/17 17:51:59  dondosha
* Removed is_megablast argument from BLASTSetUpSearchWithReadDbInternalEx since it is part of options
*
* Revision 6.71  2000/10/12 21:40:36  shavirin
* Added is_oofalign to definition of the function BLASTFilterOverlapRegions().
*
* Revision 6.70  2000/10/11 21:51:17  shavirin
* Added definition of the function BLASTFilterOverlapRegions()
*
* Revision 6.69  2000/08/31 16:29:11  shavirin
* Added definition of *QueryDNAP set of functions related to OOF.
*
* Revision 6.68  2000/08/28 21:51:53  shavirin
* Added definition of the function BlastOtherReturnsFree().
*
* Revision 6.67  2000/07/21 21:26:43  dondosha
* Added BLASTSetUpSearchWithReadDbInternalEx with Boolean argument is_megablast
*
* Revision 6.66  2000/07/08 20:44:11  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.65  2000/06/13 20:54:39  shavirin
* Added return of EFF_SEARCH_SPACE in the function BlastOtherReturnsPrepare
*
* Revision 6.64  2000/04/10 19:57:20  dondosha
* Added prototype for BlastSeqLocFillDoubleIntRev
*
* Revision 6.63  2000/03/30 21:46:08  madden
* prototypes for BLASTResultHitlistFreeEx and BlastDeleteHeap
*
* Revision 6.62  2000/03/29 22:16:08  dondosha
* Added edit_script argument to BlastSaveCurrentHspGapped
*
* Revision 6.61  2000/03/08 20:51:21  madden
* Add prototype for BlastGetAllowedGis
*
* Revision 6.60  2000/03/06 23:29:09  kans
* added prototype for BlastConvertDNASeqLoc
*
* Revision 6.59  2000/02/17 19:00:45  shavirin
* Removed theCacheSize parameter from everywhere.
*
* Revision 6.58  2000/02/01 18:08:59  dondosha
* Added prototype for BlastSaveCurrentHspGapped
*
* Revision 6.57  2000/01/11 17:04:48  shavirin
* Added parameter theCacheSize into BlastSearchBlkNew and BlastSearchBlkNewExtra
*
* Revision 6.56  1999/12/22 21:07:19  shavirin
* Added definition of the function BlastNewFindWordsEx()
*
* Revision 6.55  1999/12/14 15:35:13  madden
* Added BlastPrintFilterWarning
*
* Revision 6.54  1999/11/29 12:58:59  madden
* Added prototypes for BlastNTGetGappedScore, BlastNTPreliminaryGappedScore and BlastNtSaveCurrentHsp
*
* Revision 6.53  1999/11/02 15:24:03  madden
* Add BlastParseInputString and BlastGetLetterIndex
*
* Revision 6.52  1999/10/05 18:16:07  shavirin
* Functions tick_proc and get_db_chunk were renamed and become public.
*
* Revision 6.51  1999/09/22 20:59:56  egorov
* Add mask DB stuff
*
* Revision 6.50  1999/08/20 19:48:37  madden
* Changed call to BlastSearchBlkNew(Extra)
*
* Revision 6.49  1999/05/27 17:33:06  madden
* Fixed Int2 (should have been Int4) problem
*
* Revision 6.48  1999/04/15 13:25:07  madden
* RealBlastGetGappedAlignmentTraceback returns Int4
*
* Revision 6.47  1999/04/01 21:42:47  madden
* Fix memory leaks when gi list is used
*
* Revision 6.46  1999/03/17 16:49:10  madden
* Removed comment within comment
*
* Revision 6.45  1999/03/04 14:18:10  egorov
* Do correct filter masking when query is seqloc
* The only BlastMaskTheResidues() function is changed:
*
* Revision 6.44  1999/02/11 13:53:31  madden
* Added combine Boolean to HitRangeToSeqLoc
*
* Revision 6.43  1999/01/28 17:20:17  madden
* Added BlastGetNonSumStatsEvalue prototype
*
* Revision 6.42  1999/01/28 16:05:21  madden
* HspArrayPurge change
*
* Revision 6.41  1999/01/26 17:57:14  madden
* ContextToFrame prototype added
*
* Revision 6.40  1999/01/19 13:26:47  madden
* Change to HspArrayPurge
*
 * Revision 6.39  1999/01/08 22:08:43  madden
 * BlastScaleMatrix returns factor as FloatHi
 *
 * Revision 6.38  1998/12/18 16:19:58  madden
 * Make BLASTSetUpSearchWithReadDbInternal public, add BlastSearchBlkNewExtra
 *
 * Revision 6.37  1998/09/17 19:53:03  madden
 * Added fillCandLambda
 *
 * Revision 6.36  1998/09/16 18:59:55  madden
 * Added subset Boolean
 *
 * Revision 6.35  1998/09/14 15:11:16  egorov
 * Add support for Int8 length databases; remove unused variables
 *
 * Revision 6.34  1998/09/10 22:36:10  madden
 * Added convertSeqAlignListToValNodeList and convertValNodeListToSeqAlignList
 *
 * Revision 6.33  1998/09/09 21:18:10  madden
 * Added PrintKAParametersExtra
 *
 * Revision 6.32  1998/09/04 14:45:43  madden
 * Moved code from blast.c blastool.c
 *
 * Revision 6.31  1998/07/21 20:58:07  madden
 * Changes to allow masking at hash only
 *
 * Revision 6.30  1998/06/12 16:08:50  madden
 * BlastHitRange stuff
 *
 * Revision 6.29  1998/06/04 16:23:04  madden
 * BioseqSeg to MyBioseqSeg
 *
 * Revision 6.28  1998/05/17 16:28:44  madden
 * Allow changes to filter options and cc filtering.
 *
 * Revision 6.27  1998/05/05 14:05:36  madden
 * Added functions BlastStartAwakeThread and BlastStopAwakeThread
 *
 * Revision 6.26  1998/04/24 19:28:48  madden
 * Added BlastScaleMatrix (and other rescaling code moved from posit.c)
 *
 * Revision 6.25  1998/04/15 20:23:49  madden
 * offset arg removed from BlastMaskTheResidues
 *
 * Revision 6.24  1998/03/26 14:21:35  madden
 * Added GetScoreSetFromBlastResultHsp prototype
 *
 * Revision 6.23  1998/03/25 22:27:27  madden
 * Remove GetScoreSetFromBlastResultHsp prototype
 *
 * Revision 6.22  1998/03/24 15:38:24  madden
 * Use BlastDoubleInt4Ptr to keep track of gis and ordinal_ids
 *
 * Revision 6.21  1998/03/18 14:14:16  madden
 * Support random access by gi list
 *
 * Revision 6.20  1998/03/16 14:02:13  madden
 * Changed call to BlastSeqIdListDestruct
 *
 * Revision 6.19  1998/02/27 16:52:07  madden
 * Added BlastGetSequenceFromBioseq
 *
 * Revision 6.18  1998/02/27 14:34:51  madden
 * Added error message prototypes
 *
 * Revision 6.17  1998/02/26 22:34:25  madden
 * Changes for 16 bit windows
 *
 * Revision 6.16  1998/02/26 19:11:38  madden
 * Added prototypes for BlastNtFindWords BlastPopulateAllWordArrays
 *
 * Revision 6.15  1998/02/19 17:17:12  madden
 * Use of Int4 rather than Int2 when pruning SeqAlign
 *
 * Revision 6.14  1998/02/11 17:18:17  madden
 * Made BlastGetGappedAlignmentTraceback functions to BlastGetGapAlgnTbck (shorter than 32 chars)
 *
 * Revision 6.13  1998/01/05 16:46:54  madden
 * One or both strands can be searched, as opposed to only both, changes to number of contexts
 *
 * Revision 6.12  1997/12/31 17:53:11  madden
 * Removed BLAST_WordFinderNew and BLAST_WordFinderDestruct prototypes
 *
 * Revision 6.11  1997/12/23 18:12:38  madden
 * Changes for range-dependent blast
 *
 * Revision 6.10  1997/12/12 20:38:36  madden
 * ContextToFrame lost last parameter
 *
 * Revision 6.9  1997/11/28 18:19:37  madden
 * Changes to TxDfDbInfoNew
 *
 * Revision 6.8  1997/11/07 00:48:29  madden
 * Added TXMATRIX defintion
 *
 * Revision 6.7  1997/10/24 20:46:55  madden
 * Removed BLASTResultsStructNew prototype
 *
 * Revision 6.6  1997/10/24 19:09:19  madden
 * Removed BlastSetReadDB and BlastGetReadDB_ID, changed to ReadDBGetDb and ReadDBGetDbId
 *
 * Revision 6.5  1997/10/03 21:27:34  madden
 * Added BlastGetTypes
 *
 * Revision 6.4  1997/10/02 17:29:27  madden
 * Added PrintDbInformationBasic
 *
 * Revision 6.3  1997/09/18 22:22:09  madden
 * Added prune functions
 *
 * Revision 6.2  1997/09/16 16:31:33  madden
 * More changes for multiple db runs
 *
 * Revision 6.1  1997/09/11 18:49:28  madden
 * Changes to enable searches against multiple databases.
 *
 * Revision 6.0  1997/08/25 18:52:44  madden
 * Revision changed to 6.0
 *
 * Revision 1.48  1997/08/22 18:37:49  madden
 * Added function BlastOtherReturnsPrepare
 *
 * Revision 1.47  1997/07/24 20:34:50  madden
 * define change for masking
 *
 * Revision 1.46  1997/07/18 14:26:42  madden
 * call to AcknowledgeBlastQuery changed
 *
 * Revision 1.45  1997/07/17 20:27:11  madden
 * Changed defines to indicate frame
 *
 * Revision 1.44  1997/07/16 20:34:48  madden
 * Added function BlastConvertProteinSeqLoc
 *
 * Revision 1.43  1997/07/15 20:36:11  madden
 * Added BioseqSeg and SeqLocSeg
 *
 * Revision 1.42  1997/07/14 15:32:25  madden
 * prototype for BlastConstructErrorMessage
 *
 * Revision 1.41  1997/06/27 14:30:42  madden
 * prototypes for BlastAddSeqIdToList and BlastSeqIdListDestruct
 *
 * Revision 1.40  1997/06/06 21:29:36  madden
 * Added Boolean html to AcknowledgeBlastQuery and PrintDbInformation
 *
 * Revision 1.39  1997/06/06 19:50:58  madden
 * Added BlastMakeFakeBioseq and BlastDeleteFakeBioseq
 *
 * Revision 1.38  1997/05/27 20:20:08  madden
 * Added function BlastMaskTheResidues
 *
 * Revision 1.37  1997/04/23 21:56:07  madden
 * Changes in BlastGetGappedAlignmentTraceback for in-frame gapping tblastn.
 *
 * Revision 1.36  1997/04/11  21:18:45  madden
 * Added GetSequenceWithDenseSeg.
 *
 * Revision 1.35  1997/04/07  18:17:09  madden
 * Added prototype for BioseqBlastEngineCore
 *
 * Revision 1.34  1997/03/06  21:47:10  madden
 * Added FormatBlastParameters.
 *
 * Revision 1.33  1997/03/05  14:29:46  madden
 * Added prototype for BlastSaveCurrentHsp.
 *
 * Revision 1.32  1997/03/04  21:34:59  madden
 * Added in HspArrayPurge.
 *
 * Revision 1.31  1997/03/04  20:36:51  madden
 * *** empty log message ***
 *
 * Revision 1.30  1997/03/03  22:39:45  madden
 * Moved code from blast.c to blastutl.c.
 *
 * Revision 1.29  1997/03/03  21:48:52  madden
 * *** empty log message ***
 *
 * Revision 1.28  1997/03/01  18:25:33  madden
 * reverse flag added to BlastGetGappedAlignmentTraceback functions.
 *
 * Revision 1.27  1997/02/26  23:39:54  madden
 * Added Txdfline stuff.
 *
 * Revision 1.26  1997/02/12  22:19:08  madden
 * Added prototype for BlastNewFindWords.
 *
 * Revision 1.25  1997/02/11  19:30:54  madden
 * Added prototypes for BlastGetGappedScoreWithReaddb and BlastGetGapped
 *
 * Revision 1.24  1997/02/10  20:03:58  madden
 * Added specific to BlastAllWordNew.
 *
 * Revision 1.23  1997/02/07  22:43:03  madden
 * Moved BLAST_WordFinderNew and Destruct from blast.c to blastutl.c, made
 * non-static.
 *
 * Revision 1.22  1997/02/07  22:32:40  madden
 * Changed prototypes for BlastGetSubjectId and GetSeqAlignForResultHitList.
 *
 * Revision 1.21  1997/01/30  21:41:17  madden
 * Prototype for FormatBlastParameters added.
 *
 * Revision 1.20  1997/01/11  18:58:29  madden
 * Removed defunct PerformBlastSearch... functions.
 *
 * Revision 1.19  1997/01/07  20:40:29  madden
 * Added reverse Boolean to GetSeqAlignForResultHitList.
 *
 * Revision 1.18  1997/01/06  22:41:46  madden
 * Added prototype BlastGetSubjectId.
 *
 * Revision 1.17  1996/12/23  22:02:05  madden
 * Changes to allow two sequences to be compared.
 *
 * Revision 1.16  1996/12/20  15:31:39  madden
 * Removed prototype for Perform2PassBlastSearchWithReadDb.
 *
 * Revision 1.15  1996/12/20  14:22:48  madden
 * Added discontinuous Boolean to GetSeqAlignForResultHitList.
 *
 * Revision 1.14  1996/12/12  16:46:25  madden
 * Changed CONTAINED_IN_HSP.
 *
 * Revision 1.13  1996/12/08  15:19:59  madden
 * Added defines and prototypes for gapped alignments.
 *
 * Revision 1.12  1996/11/14  16:21:55  madden
 * changed CharPtr to Uint1Ptr in GetTranslation.
 *
 * Revision 1.11  1996/11/13  22:35:18  madden
 * Added prototype for GetTranslation.
 *
 * Revision 1.10  1996/11/05  23:19:08  madden
 * Changed BlastTranslateUnambiguousSequence prototype.
 *
 * Revision 1.9  1996/09/26  20:18:43  madden
 * Changed prototype for GetSeqAlignForResultHitList
 *
 * Revision 1.8  1996/09/12  21:12:23  madden
 * Removed prototypes for BLAST_WordFinderNew and BLAST_WordFinderDestruct.
 *
 * Revision 1.7  1996/09/11  22:22:12  madden
 * Added prototpe for BLASTPerformSearchWithReadDb.
 *
 * Revision 1.6  1996/08/26  17:24:19  shavirin
 * Added definition of function Win32TimeFill()
 *
 * Revision 1.5  1996/08/23  16:30:11  shavirin
 * Fixed NT compiler warnings type mismatch
 *
 * Revision 1.4  1996/08/15  18:58:49  madden
 * Changed context from Int2 to Int1
 *
 * Revision 1.3  1996/08/14  15:20:37  madden
 * Added prototype for BlastTranslateUnambiguousSequence.
 *
 * Revision 1.2  1996/08/07  14:24:15  madden
 * Removed functions that depend on BLAST0 structures.
 *
 * Revision 1.1  1996/08/05  19:46:53  madden
 * Initial revision
 *
 * Revision 1.32  1996/07/31  13:10:53  madden
 * Added BlastSearchBlkDuplicate prototype.
 *
 * Revision 1.31  1996/07/25  20:47:49  madden
 * Change to arguments of Perform2PassBlastSearchWithReadDb.
 *
 * Revision 1.30  1996/07/18  22:01:35  madden
 * Changed call to BlastFindWords
 *
 * Revision 1.29  1996/06/21  15:15:21  madden
 * Removed unused prototype
 *
 * Revision 1.28  1996/06/20  16:15:57  madden
 * Replaced int's with Int4's.
 *
 * Revision 1.27  1996/06/17  21:07:32  madden
 * Added prototype for GetSeqAlignForResultHitList and some LIBCALL's.
 *
 * Revision 1.26  1996/06/17  19:02:35  madden
 * Removed unused prototypes.
 *
 * Revision 1.25  1996/06/13  21:16:33  madden
 * removed BLAST_ExtendWordNew prototype.
 *
 * Revision 1.24  1996/06/11  17:58:31  madden
 * removed prototype for BLAST_ExtendWordDiagResize.
 *
 * Revision 1.23  1996/06/06  14:09:22  madden
 * Removed blast_set_parameters prototype (it became static).
 *
 * Revision 1.22  1996/06/06  13:54:51  madden
 * Removed defunct function BLAST_ParameterBlkFill
 *
 * Revision 1.21  1996/06/04  15:33:55  madden
 * Changed BlastHitList function prototypes.
 *
 * Revision 1.20  1996/05/29  12:44:40  madden
 * Added prototype for BlastTimeFillStructure.
 *
 * Revision 1.19  1996/05/16  19:51:09  madden
 * Added documentation block.
 *
 * Revision 1.18  1996/05/14  16:15:59  madden
 * Added protoytpe for BLASTResultsStruc functions.
 *
 * Revision 1.17  1996/05/01  14:59:41  madden
 * *** empty log message ***
 *
 * Revision 1.16  1996/04/03  19:14:08  madden
 * added functions PerformBlastSearchWithReadDb and Perform2PassBlastSearchWithReadDb.
 *
 * Revision 1.15  1996/03/29  21:28:20  madden
 * *** empty log message ***
 *
 * Revision 1.14  1996/03/29  14:09:37  madden
 * prototype for GetSeqAlignForSparseHitList added.
 *
 * Revision 1.13  1996/02/28  21:38:36  madden
 * Changed prototypes for discontiguous words.
 *
 * Revision 1.12  1996/02/05  18:46:57  madden
 * *** empty log message ***
 *
 * Revision 1.11  1996/02/02  19:25:32  madden
 * Changed BlastFindWords prototype.
 *
 * Revision 1.10  1996/01/31  22:28:46  madden
 * Added prototype for BlastReapHitlistByEvalue.
 *
 * Revision 1.9  1996/01/17  13:47:13  madden
 * *** empty log message ***
 *
 * Revision 1.8  1996/01/11  15:17:58  madden
 * Added prototype for do_MPblast_search.
 *
 * Revision 1.7  1996/01/10  17:51:09  madden
 * Added SortHitListByPvalue.
 *
 * Revision 1.6  1996/01/06  18:58:45  madden
 * Added prototype for BlastLinkHsps.
 *
 * Revision 1.5  1995/12/30  18:39:27  madden
 * Added prototype for GetBLAST0KABlk.
 *
 * Revision 1.4  1995/12/28  21:26:30  madden
 * Added in prototype for do_blast_search.
 *
 * Revision 1.3  1995/12/26  23:05:29  madden
 * Added prototype for blast_set_parameters.
 *
 * Revision 1.2  1995/12/21  23:11:11  madden
 *  BLAST_Score prototypes moved to blastkar.h.
 *
 * Revision 1.1  1995/12/19  22:31:17  madden
 * Initial revision
 *
 * Revision 1.1  1995/12/08  15:48:23  madden
 * Initial revision
 *
 * */
#ifndef __BLASTPRI__
#define __BLASTPRI__

#include <blast.h>
#include <blastkar.h>
#include <posit.h>
#include <seed.h>
#include <ffprint.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct _txdbinfo {
   struct _txdbinfo PNTR next;
   Boolean   is_protein;
   CharPtr   name;
   CharPtr   definition;
   CharPtr   date;
   Int8   total_length;
   Int4   number_seqs;
   Boolean subset;	/* Print the subset message. */
} TxDfDbInfo, *TxDfDbInfoPtr;
	

/*
	Defines for the return values in "other_returns".
*/

#define SEQLOC_MASKING_NOTSET 0
#define SEQLOC_MASKING_PLUS1 1
#define SEQLOC_MASKING_PLUS2 2
#define SEQLOC_MASKING_PLUS3 3
#define SEQLOC_MASKING_MINUS1 4
#define SEQLOC_MASKING_MINUS2 5
#define SEQLOC_MASKING_MINUS3 6
#define TXDBINFO 10
#define TXKABLK_NOGAP 12
#define TXKABLK_GAP 13
#define TXPARAMETERS 14
#define TXMATRIX 15
#define EFF_SEARCH_SPACE 16
#define EFF_HSP_LENGTH 17
	
/*
	Allocates memory for TxDfDbInfoPtr.
	Link up new (returned) value to 'old', if non-NULL.
*/
TxDfDbInfoPtr LIBCALL TxDfDbInfoNew PROTO((TxDfDbInfoPtr old));

/*
	Deallocates memory (including strings for name, definition, and date).
*/
TxDfDbInfoPtr LIBCALL TxDfDbInfoDestruct PROTO((TxDfDbInfoPtr dbinfo));

/*
Print a summary of the query.
*/
Boolean LIBCALL AcknowledgeBlastQuery PROTO((BioseqPtr bsp, Int4 line_length, FILE *outfp, Boolean believe_query, Boolean html));

/*
	Print a report of the database used.
*/
Boolean LIBCALL PrintDbReport PROTO((TxDfDbInfoPtr dbinfo, Int4 line_length, FILE *outfp));

/*
	Print a warning about the filtering used.
*/
Boolean LIBCALL BlastPrintFilterWarning PROTO((CharPtr filter_string, Int4 line_length, FILE *outfp, Boolean html));


/*
	print out some of the Karlin-Altschul parameters.
*/
Boolean LIBCALL PrintKAParameters PROTO((Nlm_FloatHi Lambda, Nlm_FloatHi K, Nlm_FloatHi H, Int4 line_length, FILE *outfp, Boolean gapped));
Boolean LIBCALL PrintKAParametersExtra PROTO((Nlm_FloatHi Lambda, Nlm_FloatHi K, Nlm_FloatHi H, Nlm_FloatHi C, Int4 line_length, FILE *outfp, Boolean gapped));

/*
	Print a CharPtr (VisibleString), printing a new line every time
	a tilde is encountered.
*/
Boolean LIBCALL PrintTildeSepLines PROTO((CharPtr buffer, Int4 line_length, FILE *outfp));

/* How many interations should be done in the bisection. */
#define BLAST_SAVE_ITER_MAX 20


/*
	TRUE if c is between a and b; f between d and f.  Determines if the
	coordinates are already in an HSP that has been evaluated. 
*/
#define CONTAINED_IN_HSP(a,b,c,d,e,f) (((a <= c && b >= c) && (d <= f && e >= f)) ? TRUE : FALSE)

Int2 LIBCALL BlastFindWords PROTO((BlastSearchBlkPtr search, Int4 start, Int4 len, BLAST_Score threshold, Int1 context_index));


Int2 LIBCALL BlastNewFindWords PROTO((BlastSearchBlkPtr search, Int4 start, Int4 len, BLAST_Score threshold, Int1 context_index));

Int2 BlastNewFindWordsEx(LookupTablePtr lookup, BLAST_ScorePtr PNTR posMatrix, Int4 start, Int4 len, BlastAllWordPtr all_words, BLAST_Score threshold, Int4 wordsize, Int1 context_index);
    
Int2 LIBCALL BlastLinkHsps PROTO ((BlastSearchBlkPtr search));

Int2 LIBCALL BlastReapHitlistByEvalue PROTO ((BlastSearchBlkPtr search));

Int4 LIBCALL BlastSaveCurrentHitlist PROTO((BlastSearchBlkPtr search));

Int2 LIBCALL BLASTPerformSearchWithReadDb PROTO((BlastSearchBlkPtr search, Int4 sequence_number));

Int2 LIBCALL BLASTPerformSearch PROTO((BlastSearchBlkPtr search, Int4 subject_length, Uint1Ptr subject_seq));

BLASTResultsStructPtr LIBCALL BLASTResultsStructDelete PROTO((BLASTResultsStructPtr result_struct));

SeqAlignPtr LIBCALL GetSeqAlignForResultHitList PROTO((BlastSearchBlkPtr search, Boolean getdensediag, Boolean ordinal_number, Boolean discontinuous, Boolean reverse, Boolean get_redundant_seq));

Int2 LIBCALL BlastTimeFillStructure PROTO((BlastTimeKeeperPtr btkp));

BlastSearchBlkPtr LIBCALL BlastSearchBlkDuplicate PROTO((BlastSearchBlkPtr search));

BlastSearchBlkPtr LIBCALL BlastSearchBlkNew PROTO((Int2 wordsize, Int4 qlen, CharPtr dbname, Boolean multiple_hits, BLAST_Score threshold_first, BLAST_Score threshold_second, Int4 result_size, CharPtr prog_name, BlastAllWordPtr all_words, Int2 first_context, Int2 last_context, Int4 window_size));

/* Allocates a search Block, except it only attaches to the rdfp, does not allocate it. */
BlastSearchBlkPtr LIBCALL BlastSearchBlkNewExtra PROTO((Int2 wordsize, Int4 qlen, CharPtr dbname, Boolean multiple_hits, BLAST_Score threshold_first, BLAST_Score threshold_second, Int4 result_size, CharPtr prog_name, BlastAllWordPtr all_words, Int2 first_context, Int2 last_context, ReadDBFILEPtr rdfp, Int4 window_size));

BlastSearchBlkPtr LIBCALL BlastSearchBlkDestruct PROTO((BlastSearchBlkPtr search));

BlastSearchBlkPtr BLASTSetUpSearchWithReadDbInternal PROTO((SeqLocPtr query_slp, BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, CharPtr dbname, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total, ReadDBFILEPtr rdfp));

BlastSearchBlkPtr BLASTSetUpSearchWithReadDbInternalMult PROTO((SeqLocPtr query_slp, BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, CharPtr dbname, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total, ReadDBFILEPtr rdfp, QueriesPtr mult_queries));
/* --KM added mult_queries param */

BlastSearchBlkPtr LIBCALL
BLASTSetUpSearchWithReadDbInternalEx PROTO((SeqLocPtr query_slp, BioseqPtr query_bsp,
				      CharPtr prog_name, Int4 qlen, CharPtr
				      dbname, BLAST_OptionsBlkPtr options, int
				      (LIBCALLBACK *callback)PROTO((Int4 done,
								    Int4
								    positives)),
				      SeqIdPtr seqid_list, BlastDoubleInt4Ptr
				      gi_list, Int4 gi_list_total, ReadDBFILEPtr
				      rdfp));

Int4 LIBCALL BlastTranslateUnambiguousSequence PROTO((BlastSearchBlkPtr search, Int4 length, Uint1Ptr prot_seq, Uint1Ptr nt_seq, Int2 frame));

Uint1Ptr LIBCALL GetTranslation PROTO((Uint1Ptr query_seq, Int4 nt_length, Int2 frame, Int4Ptr length, CharPtr genetic_code));

SeqAlignPtr LIBCALL BlastGetGapAlgnTbck PROTO((BlastSearchBlkPtr search, Int4 hit_number, Boolean reverse, Boolean ordinal_number, Uint1Ptr subject, Int4 subject_length, Uint1Ptr rev_subject, Int4 rev_subject_length));

SeqAlignPtr LIBCALL BlastGetGapAlgnTbckWithReaddb PROTO((BlastSearchBlkPtr search, Int4 hit_number, Boolean ordinal_number));

Int2 LIBCALL BlastGetGappedScoreWithReaddb PROTO((BlastSearchBlkPtr search, Int4 sequence_number));

Int2 LIBCALL BlastGetGappedScore PROTO((BlastSearchBlkPtr search, Int4 subject_length, Uint1Ptr subject, Int2 frame));

Int2 LIBCALL BlastNTGetGappedScore PROTO((BlastSearchBlkPtr search, Int4 subject_length, Uint1Ptr subject));


SeqIdPtr LIBCALL BlastGetSubjectId PROTO((BlastSearchBlkPtr search, Int4 hit_number, Boolean ordinal_number, ValNodePtr *vnpp));



BlastAllWordPtr LIBCALL BlastAllWordNew PROTO((Int4 num_of_cols, Int4 wordsize, Boolean rows_allocated, Boolean specific));

BlastAllWordPtr LIBCALL BlastAllWordDestruct PROTO((BlastAllWordPtr all_words));


BLAST_HitListPtr LIBCALL BlastHitListDestruct PROTO((BLAST_HitListPtr hitlist));

BLAST_HitListPtr LIBCALL BlastHitListNew PROTO((BlastSearchBlkPtr search));



void LIBCALL BlastExtendWordExit PROTO((BlastSearchBlkPtr search));



Boolean LIBCALL FilterDNA PROTO((BioseqPtr bsp, Int4 filter));

Boolean LIBCALL FilterWithSeg PROTO((Uint1Ptr sequence, Int4 length, Uint1 alphabet));

BLASTResultHitlistPtr LIBCALL BLASTResultHitlistFree PROTO((BLASTResultHitlistPtr result));
BLASTResultHitlistPtr LIBCALL BLASTResultHitlistFreeEx PROTO((BlastSearchBlkPtr search, BLASTResultHitlistPtr result));

BLASTResultHitlistPtr LIBCALL BLASTResultHitlistNew PROTO((Int4 hspcnt));

Nlm_FloatHi LIBCALL GetDbSubjRatio PROTO((BlastSearchBlkPtr search, Int4 subject_length));

Int2 LIBCALL BlastPreliminaryGappedScore PROTO((BlastSearchBlkPtr search, Uint1Ptr subject, Int4 subject_length, Int2 frame));

Int2 LIBCALL BlastNTPreliminaryGappedScore PROTO((BlastSearchBlkPtr search, Uint1Ptr subject, Int4 subject_length));


Int2 LIBCALL BlastHitListPurge PROTO((BLAST_HitListPtr hitlist));

Int4 LIBCALL HspArrayPurge PROTO((BLAST_HSPPtr PNTR hsp_array, Int4 hspcnt, Boolean clear_num));


void BlastSaveCurrentHsp PROTO((BlastSearchBlkPtr search, BLAST_Score score, Int4 q_offset, Int4 s_offset, Int4 length, Int2 context));

void BlastSaveCurrentHspGapped PROTO((BlastSearchBlkPtr search, BLAST_Score
				      score, Int4 q_offset, Int4 s_offset, Int4
				      q_length, Int4 s_length, Int2 context,
				      GapXEditScriptPtr esp));
void BlastNtSaveCurrentHspGapped PROTO((BlastSearchBlkPtr search, BLAST_Score
				      score, Int4 q_offset, Int4 s_offset, Int4
				      q_length, Int4 s_length, 
                                      Int4 q_gapped_start, Int4 s_gapped_start,
                                      Int2 context, GapXEditScriptPtr esp));

void BlastNtSaveCurrentHsp PROTO((BlastSearchBlkPtr search, BLAST_Score score, Int4 q_offset, Int4 s_offset, Int4 length, Int2 context, Int4 query_gap_start, Int4 subject_gap_start));

CharPtr FormatBlastParameters PROTO((BlastSearchBlkPtr search));


SeqAlignPtr LIBCALL BioseqBlastEngineCore PROTO((BlastSearchBlkPtr search, BLAST_OptionsBlkPtr options, Int4Ptr *pos_matrix));

Uint1Ptr GetSequenceWithDenseSeg PROTO((DenseSegPtr dsp, Boolean query, Int4Ptr start, Int4Ptr length));

void BlastMaskTheResidues PROTO((Uint1Ptr buffer, Int4 max_length, Uint1 mask_residue, SeqLocPtr mask_slp, Boolean reverse, Int4 offset));

BioseqPtr LIBCALL BlastMakeFakeBioseq PROTO((BioseqPtr bsp, CharPtr name));

BioseqPtr LIBCALL BlastDeleteFakeBioseq PROTO((BioseqPtr fake_bsp));

Boolean BlastAddSeqIdToList PROTO((BlastSearchBlkPtr search, Int4 ordinal_id, SeqIdPtr sip));

ValNodePtr BlastConstructErrorMessage PROTO((CharPtr function, CharPtr message, Uint1 level, ValNodePtr PNTR vnpp));

BlastErrorMsgPtr BlastDestroyErrorMessage PROTO((BlastErrorMsgPtr error_msg));

ValNodePtr BlastErrorChainDestroy PROTO((ValNodePtr vnp));

ValNodePtr LIBCALL BlastOtherReturnsPrepare PROTO((BlastSearchBlkPtr search));
void LIBCALL BlastOtherReturnsFree PROTO((ValNodePtr other_returns));

SeqLocPtr BlastBioseqFilter PROTO((BioseqPtr bsp, CharPtr instructions));

SeqLocPtr BlastSeqLocFilter PROTO((SeqLocPtr slp, CharPtr instructions));

SeqLocPtr BlastBioseqFilterEx PROTO((BioseqPtr bsp, CharPtr instructions, BoolPtr mask_at_hash));

SeqLocPtr BlastSeqLocFilterEx PROTO((SeqLocPtr slp, CharPtr instructions, BoolPtr mask_at_hash));

SeqLocPtr MyBioseqSeg PROTO((BioseqPtr bsp_unfilter));

SeqLocPtr SeqLocSeg PROTO((SeqLocPtr slp));

Boolean BlastConvertProteinSeqLoc PROTO((SeqLocPtr slp, Int2 frame, Int4 full_length));

Boolean BlastConvertDNASeqLoc (SeqLocPtr slp, Int2 frame, Int4 full_length);

BlastPruneSapStructPtr BlastPruneSapStructDestruct PROTO((BlastPruneSapStructPtr prune));

BlastPruneSapStructPtr BlastPruneHitsFromSeqAlign PROTO((SeqAlignPtr sap, Int4 number, BlastPruneSapStructPtr prune));

Uint1 LIBCALL BlastGetTypes PROTO((CharPtr blast_program, Boolean PNTR query_is_na, Boolean PNTR db_is_na));

BLASTResultsStructPtr BLASTResultsStructNew PROTO((Int4 results_size, Int4 max_pieces, Int4 range_max));

Int2 BlastNtFindWords PROTO((BlastSearchBlkPtr search, Int4 start, Int4 len, Int1 context_index));

BlastAllWordPtr BlastPopulateAllWordArrays PROTO((Int4 wordsize, Int4 alphabet_size));

Uint1Ptr BlastGetSequenceFromBioseq PROTO((BioseqPtr bsp, Int4Ptr length));

#if 0 /* deprecated */
BlastSeqIdListPtr BlastSeqIdListNew PROTO((void));
BlastSeqIdListPtr BlastSeqIdListDestruct PROTO((BlastSeqIdListPtr seqid_list));
#endif

/* Retrieve the virtual oidlist from rdfp_chain */
OIDListPtr LIBCALL BlastGetVirtualOIDList PROTO((ReadDBFILEPtr rdfp_chain));

Boolean BlastAdjustDbNumbers PROTO((ReadDBFILEPtr rdfp_list, Int8Ptr db_length, Int4Ptr db_number, SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, OIDListPtr oidlist, BlastDoubleInt4Ptr PNTR gi_list_pointers, Int4 gi_list_total));

BlastGiListPtr BlastGiListDestruct PROTO((BlastGiListPtr blast_gi_list, Boolean contents));
BlastGiListPtr BlastGiListNew PROTO((BlastDoubleInt4Ptr gi_list, Int4 total));


ScorePtr LIBCALL GetScoreSetFromBlastResultHsp PROTO((BLASTResultHspPtr hsp, SeqIdPtr gi_list));

Nlm_FloatHi BlastScaleMatrix PROTO((BlastMatrixRescalePtr matrix_rescale, Boolean position_dependent));

BlastMatrixRescalePtr BlastMatrixRescaleNew PROTO((Int4 alphabet_size, Int4 query_length, Uint1Ptr query,  Nlm_FloatHiPtr standardProb, Int4Ptr *matrix, Int4Ptr *private_matrix, BLAST_KarlinBlkPtr *kbp_std, BLAST_KarlinBlkPtr *kbp_psi, BLAST_KarlinBlkPtr *kbp_gap_std, BLAST_KarlinBlkPtr *kbp_gap_psi, Nlm_FloatHi lambda_ideal,  Nlm_FloatHi K_ideal));

BlastMatrixRescalePtr BlastMatrixRescaleDestruct PROTO((BlastMatrixRescalePtr matrix_rescale));

/*
        starts the awake thread using static variables in this file.
*/

void BlastStartAwakeThread PROTO((BlastThrInfoPtr thr_info));

/* Change the awake flag.  This thread will die in one second. */
void BlastStopAwakeThread PROTO((BlastThrInfoPtr thr_info));

SeqLocPtr HitRangeToSeqLoc PROTO((BlastHitRangePtr bhrp, Int4 link_value, Boolean combine));


ValNodePtr BlastSeqLocFillDoubleInt PROTO((SeqLocPtr mask_slp, Int4 max_length, Boolean reverse));
ValNodePtr BlastSeqLocFillDoubleIntEx PROTO((SeqLocPtr mask_slp, Int4 full_query_length, Int4 max_length, Boolean reverse, Int4 offset));
ValNodePtr BlastSeqLocFillDoubleIntRev PROTO((ValNodePtr location, SeqLocPtr mask_slp, Int4 full_query_length, Int4 max_length, Int4 offset));


Int2 BlastInsertList2Heap PROTO((BlastSearchBlkPtr search, BLASTResultHitlistPtr result_hitlist));

void BlastFreeHeap PROTO((BlastSearchBlkPtr search, BLASTResultHitlistPtr result_hitlist));

ValNodePtr convertSeqAlignListToValNodeList(SeqAlignPtr seqAlignList, SeqAlignPtr * lastSeqAligns, Int4 numLastSeqAligns);

SeqAlignPtr convertValNodeListToSeqAlignList(ValNodePtr seqAlignDoubleList, SeqAlignPtr ** lastSeqAligns, Int4 * numLastSeqAligns);

void LIBCALL fillCandLambda(seedSearchItems * seedSearch, Char *matrixName, BLAST_OptionsBlkPtr options);

Int4 RealBlastGetGappedAlignmentTraceback(BlastSearchBlkPtr search, Uint1Ptr subject, Int4 subject_length, Uint1Ptr rev_subject, Int4 rev_subject_length, SeqIdPtr subject_id, BLAST_HSPPtr *hsp_array, Int4 hspcnt, SeqAlignPtr *head, BlastHitRangePtr bhrp, Int4 min_score_to_keep, Boolean reverse, Int4 ordinal_id, Boolean do_traceback);

Int2 ContextToFrame PROTO((BlastSearchBlkPtr search, Int2 context_number));

int LIBCALLBACK score_compare_hsps PROTO((VoidPtr v1, VoidPtr v2));

Int2 LIBCALL BlastGetNonSumStatsEvalue PROTO((BlastSearchBlkPtr search));

Boolean BlastGetDbChunk(ReadDBFILEPtr rdfp, Int4Ptr start, Int4Ptr stop, 
                        Int4Ptr id_list, Int4Ptr id_list_number, 
                        BlastThrInfoPtr thr_info);
void BlastTickProc(Int4 sequence_number, BlastThrInfoPtr thr_info);

Boolean BlastParseInputString(CharPtr string, CharPtr letters, CharPtr PNTR *values_in, CharPtr PNTR ErrorMessage);

Int4 BlastGetLetterIndex(CharPtr letters, Char ch);

SeqIdPtr BlastGetAllowedGis PROTO((BlastSearchBlkPtr search, Int4 ordinal_id, SeqIdPtr PNTR seqid));

Int4 BlastDeleteHeap PROTO((BLASTHeapPtr which_heap, Int4 position));

/* Functions used for printing error messages. */

Uint1 BlastSetUserErrorString(CharPtr string, SeqIdPtr sip, Boolean use_id);
void BlastDeleteUserErrorString(Uint1 err_id);



/* Functions used in OOF calculations */

BlastSequenceBlkPtr PNTR LIBCALL
BlastMakeCopyQueryDNAP PROTO((BlastSequenceBlkPtr PNTR bsbpp_in));

void LIBCALL BlastFreeQueryDNAP PROTO((BlastSequenceBlkPtr PNTR bsbpp));

    BlastSequenceBlkPtr PNTR LIBCALL
BlastCreateQueryDNAP PROTO((BlastSearchBlkPtr search, Int4 length));

/* -----------------------------------------------------------------
   This function will filter given SeqAlignPtr for overlaping
   regions for the same query/subject pair. Another input parameter is
   percentage of overlapping required for the alignment to be removed
   from SeqAlignPtr
   ---------------------------------------------------------------- */
SeqAlignPtr BLASTFilterOverlapRegions(SeqAlignPtr sap, Int4 pct, 
                                      Boolean subject_is_aa, 
                                      Boolean is_ooframe,
                                      Boolean sort_array);

/* End of functions used in OOF calculations */

Int4
BlastGetNumIdentical PROTO((Uint1Ptr query, Uint1Ptr subject, Int4 q_start, 
                         Int4 s_start, Int4 length, Boolean reverse));

Int2
BlastHSPGetNumIdentical PROTO((BlastSearchBlkPtr search, BLAST_HSPPtr hsp,
   BLASTResultHspPtr result_hsp, Int4Ptr num_ident, Int4Ptr align_length));
Int2
OOFBlastHSPGetNumIdentical PROTO((Uint1Ptr query_seq, Uint1Ptr subject_seq,
   BLAST_HSPPtr hsp, BLASTResultHspPtr result_hsp, Int4Ptr num_ident, 
   Int4Ptr align_length));

SeqIdPtr GetTheSeqAlignID PROTO((SeqIdPtr seq_id));
StdSegPtr BLASTHspToStdSeg PROTO((BlastSearchBlkPtr search, Int4 subject_length, BLAST_HSPPtr hsp, SeqIdPtr sip, Boolean reverse, SeqIdPtr gi_list));

/* Callbacks to handle results instead of saving hit lists */
#define BUFFER_LENGTH 255
#define LARGE_BUFFER_LENGTH 1024

int LIBCALLBACK BlastPrintAlignInfo PROTO((VoidPtr srch));
int LIBCALLBACK MegaBlastPrintAlignInfo PROTO((VoidPtr srch));
void BlastPrintTabulatedResults PROTO((SeqAlignPtr seqalign, BioseqPtr query_bsp, SeqLocPtr query_slp, Int4 num_alignments, CharPtr blast_program, Boolean is_ungapped, Boolean believe_query, Int4 q_shift, Int4 s_shift, FILE *fp, Boolean print_query_info));
void BlastPrintTabulatedResultsEx PROTO((SeqAlignPtr seqalign, BioseqPtr query_bsp, SeqLocPtr query_slp, Int4 num_alignments, CharPtr blast_program, Boolean is_ungapped, Boolean believe_query, Int4 q_shift, Int4 s_shift, FILE *fp, int *num_formatted, Boolean print_query_info));

void BlastPrintTabularResults(SeqAlignPtr seqalign, BioseqPtr query_bsp,
        SeqLocPtr query_slp, Int4 num_alignments, CharPtr blast_program, 
        Boolean is_ungapped, Boolean is_ooframe, Boolean believe_query, 
        Int4 q_shift, Int4 s_shift, FILE *fp, int *num_formatted, 
        Boolean print_query_info);

void
BlastProcessGiLists PROTO((BlastSearchBlkPtr search, 
            BLAST_OptionsBlkPtr options, BlastDoubleInt4Ptr gi_list, 
            Int4 gi_list_size));

/* Purpose: Create the virtual oidlist that ultimately will limit the 
 * blast search being set up. This will be an intersection of the bglp
 * parameter as well as any oidlists attached to the rdfp_chain.
 *
 * Parameters: 
 * bglp: The gi list to limit the search (this will be freed this by this
 * function)
 * rdfp_chain: The head of the rdfp list that will be searched.
 * oidlist_for_all_rdfp: if true, the virtual oidlist will be attached to the
 * beginning of the rdfp_chain, else it will be attached to the first rdfp in
 * rdfp_chain that had an oidlist (and therefore covering all remaining
 * rdfps).
 * options: the blast options structure (see comments in blastool.c).
 * 
 * Return value:
 * The intersection of the bglp parameter and any oidlists attached to the
 * rdfp_chain. */
BlastGiListPtr BlastCreateVirtualOIDList PROTO((BlastGiListPtr bglp, 
                                         ReadDBFILEPtr rdfp_chain,
                                         Boolean oidlist_forall_rdfp, 
                                         BLAST_OptionsBlkPtr options));
Boolean FastaCheckDna PROTO((CharPtr seq));
BLASTHSPSegmentPtr BLASTHSPSegmentFromSeqAlign PROTO((SeqAlignPtr sap));
SeqAlignPtr BlastClusterHitsFromSeqAlign PROTO((SeqAlignPtr seqalign, CharPtr prog_name, CharPtr database, BLAST_OptionsBlkPtr options, FloatHi length_thresh, FloatHi score_thresh, FloatHi overlap_thresh, Boolean two_sided));
SeqAlignPtr RedoAlignmentCore PROTO((BlastSearchBlkPtr search,   
  BLAST_OptionsBlkPtr options, Int4 hitlist_count, Boolean adjustParameters,
  Boolean SmithWaterman));
BLAST_HSPPtr BLAST_HSPFree PROTO((BLAST_HSPPtr hsp));
void BLASTResultFreeHsp PROTO((BLASTResultHitlistPtr result));
void BLASTPostSearchLogic PROTO((BlastSearchBlkPtr search, BLAST_OptionsBlkPtr
                                 options, SeqAlignPtr PNTR seqalignp,
                                 Boolean single_chain));
Boolean
BlastNtWordUngappedExtend PROTO((BlastSearchBlkPtr search, Int4 q_off, 
                                 Int4 s_off, Int4 cutoff));
SeqLocPtr blastMergeFilterLocs PROTO((SeqLocPtr filter_slp, 
                                      SeqLocPtr lcmask, Boolean translate,
                                      Int2 frame, Int4 length));

/*
        function to calculate effective query length and
        effective db length.
*/

/*return query fasta style title(id+title). New memory was allocated for this title*/
CharPtr getFastaStyleTitle(BioseqPtr bsp);

CharPtr load_options_to_buffer(CharPtr instructions, CharPtr buffer);

VoidPtr index_proc PROTO((VoidPtr dummy));

CharPtr BlastConstructFilterString PROTO((Int4 filter_value));

int LIBCALLBACK evalue_compare_hits PROTO((VoidPtr v1, VoidPtr v2));

Uint1 FrameToDefine PROTO((Int2 frame));

Uint1Ptr
GetPrivatTranslationTable PROTO((CharPtr genetic_code, 
                                 Boolean reverse_complement));
Int2 blast_set_parameters PROTO((BlastSearchBlkPtr search, Int4
                                 dropoff_number_of_bits_1st_pass, Int4
                                 dropoff_number_of_bits_2nd_pass, Nlm_FloatHi
                                 avglen, Nlm_FloatHi searchsp, Int4 window));
Boolean 
BlastGetFirstAndLastContext PROTO((CharPtr prog_name, SeqLocPtr query_slp, Int2Ptr first_context, Int2Ptr last_context, Uint1 strand_options));

#ifdef __cplusplus
}
#endif
#endif /* !__BLASTPRI__ */

