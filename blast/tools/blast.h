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

File name: blast.h

Author: Tom Madden

Contents: prototypes for "public" BLAST functions (ones that other utilitiles
	may safely call).

******************************************************************************/

/* $Revision: 6.48 $ 
* $Log: blast.h,v $
* Revision 6.48  2001/07/09 15:12:47  shavirin
* Functions BLbasicSmithWatermanScoreOnly() and BLSmithWatermanFindStart()
* used to calculate Smith-waterman alignments on low level become external.
*
* Revision 6.47  2001/06/18 16:09:25  dondosha
* Added prototype for PrintTabularOutputHeader
*
* Revision 6.46  2001/06/13 21:40:54  dondosha
* Moved GetGisFromFile declaration from mblast.h to blast.h
*
* Revision 6.45  2001/04/16 21:28:11  dondosha
* Added function BlastPruneSeqAlignByEvalueRange
*
* Revision 6.44  2001/04/12 21:34:50  dondosha
* Added function BlastPruneSeqAlignByGiList
*
* Revision 6.43  2001/04/06 18:15:08  madden
* Move UNIX-specific stuff (HeyIAmInMemory) to bqueue.[ch]
*
* Revision 6.42  2001/02/07 21:10:10  dondosha
* Added prototypes of Blast Engine functions with callback
*
* Revision 6.41  2001/01/16 23:16:51  dondosha
* Added 2 arguments and several options to parse_blast_options
*
* Revision 6.40  2001/01/09 20:10:39  shavirin
* Added sorting of all hits in result_struct for every element in
* results. Added function RPSResultHspScoreCmp.
*
* Revision 6.39  2000/11/02 20:16:34  dondosha
* Added prototypes for BlastTwoSequencesByLocWithCallback and BlastTwoSequencesWithCallback
*
* Revision 6.38  2000/10/31 16:30:58  shavirin
* Function BLASTSetUpSearchInternalByLoc became external.
*
* Revision 6.37  2000/10/26 18:52:42  dondosha
* Added prototype for MegaBlastPrintReference
*
* Revision 6.36  2000/10/24 19:00:39  dondosha
* Removed UniqueLocalId() prototype - goes to sequtil.h
*
* Revision 6.35  2000/10/24 18:57:22  dondosha
* Added prototype of UniqueLocalId(), removed from mblast.h
*
* Revision 6.34  2000/08/31 16:27:20  shavirin
* Added definition of the function BlastSequenceBlkDestruct().
*
* Revision 6.33  2000/07/12 23:07:30  kans
* reverse_seq moved from pseed3.c to blastool.c, placed in blast.h header, called by gapxdrop.c
*
* Revision 6.32  2000/07/07 21:20:07  vakatov
* Get all "#include" out of the 'extern "C" { }' scope!
*
* Revision 6.31  2000/06/20 15:50:45  shavirin
* Added new functions: BLASTAddBlastDBTitleToSeqAnnot and
* BLASTGetDatabaseTitleFromSeqAnnot().
*
* Revision 6.30  2000/06/14 22:21:57  dondosha
* Added prototypes for BlastQuerySequenceSetUp and BlastSequencesOnTheFlyEx
*
* Revision 6.29  2000/04/28 19:49:36  shavirin
* Added definition of the function DefineToFrame().
*
* Revision 6.28  2000/03/31 17:01:26  dondosha
* Added explanation for use of blastx in two sequences search
*
* Revision 6.27  2000/03/24 17:01:27  kans
* added BLASTUpdateSeqIdInSeqInt - needed to compile blastall.c on Mac, which requires prototypes
*
* Revision 6.26  2000/02/23 20:37:45  dondosha
* Added prototype for MegaBlastBuildLookupTable and BlastNtWordExtend
*
* Revision 6.25  2000/02/11 16:40:53  egorov
* The parse_blast_options is made public.
*
* Revision 6.24  2000/02/01 18:04:59  dondosha
* Added prototype for GreedyAlignMemAlloc
*
* Revision 6.23  2000/01/26 22:01:57  madden
* Add function BlastGetProgramName
*
* Revision 6.22  2000/01/14 18:27:45  shavirin
* Added definitions of WordExtend* functions.
*
* Revision 6.21  1999/12/29 18:55:29  shavirin
* Added definition of non-static function BlastSequenceAddSequence().
*
* Revision 6.20  1999/09/22 20:55:07  egorov
* Add time measure stuff
*
* Revision 6.19  1999/07/01 13:03:24  sicotte
* Updated for DenseDiag and Moved seqalign_reverse_strand from blastutl.c(blast.h) to SeqAlignListReverseStrand in salpedit.ch and fixed call in salutil.c
*
* Revision 6.18  1999/03/18 16:43:31  shavirin
* Added definition of the function Boolean HeyIAmInMemory(Int4 program)
*
* Revision 6.17  1999/03/17 16:49:11  madden
* Removed comment within comment
*
* Revision 6.16  1999/01/26 18:26:23  madden
* Add updateLambdaK prototype
*
* Revision 6.15  1998/09/22 16:56:12  egorov
* Add prototype for BlastErrorPrintExtra()
*
 * Revision 6.14  1998/09/14 15:11:14  egorov
 * Add support for Int8 length databases; remove unused variables
 *
 * Revision 6.13  1998/08/25 14:16:23  madden
 * Added BlastGetPhiReference and BlastPrintPhiReference
 *
 * Revision 6.12  1998/06/12 16:08:49  madden
 * BlastHitRange stuff
 *
 * Revision 6.11  1998/05/22 20:20:37  madden
 * Added BlastTwoSequencesByLocEx and BlastTwoSequencesEx
 *
 * Revision 6.10  1998/05/01 18:34:37  egorov
 * Add new parametes to BLASTOptionSetGapParam()
 *
 * Revision 6.9  1998/03/24 15:38:22  madden
 * Use BlastDoubleInt4Ptr to keep track of gis and ordinal_ids
 *
 * Revision 6.8  1998/03/18 14:14:14  madden
 * Support random access by gi list
 *
 * Revision 6.7  1998/03/14 18:28:08  madden
 * Added BioseqBlastEngineEx
 *
 * Revision 6.6  1998/02/26 19:09:28  madden
 * Removed AdjustOffSetsInSeqAlign prototype
 *
 * Revision 6.5  1998/01/05 22:41:44  madden
 * Added seqalign_reverse_strand
 *
 * Revision 6.4  1997/12/10 22:41:20  madden
 * prototype for BlastGetProgramNumber
 *
 * Revision 6.3  1997/12/01 22:07:15  madden
 * Changed call to BLASTOptionValidateEx
 *
 * Revision 6.2  1997/11/18 22:23:17  madden
 * Added BLASTOptionSetGapParams
 *
 * Revision 6.1  1997/10/02 17:28:55  madden
 * Added prototype for BlastPrintVersionInfoEx
 *
 * Revision 6.0  1997/08/25 18:52:29  madden
 * Revision changed to 6.0
 *
 * Revision 1.26  1997/07/22 17:21:55  madden
 * Added index callbacks to SetUp function prototypes
 *
 * Revision 1.25  1997/07/21 17:36:47  madden
 * Added BlastGetReleaseDate
 *
 * Revision 1.24  1997/07/18 20:55:25  madden
 * Added prototypes for BlastGetVersionNumber and BlastGetReference
 *
 * Revision 1.23  1997/07/14 16:15:09  madden
 * Added prototype for BlastErrorPrint
 *
 * Revision 1.22  1997/07/14 15:33:32  madden
 * Prototype for BLASTOptionValidateEx
 *
 * Revision 1.21  1997/07/11 19:29:08  madden
 * Added prototypes for BLASTSetUpSearchByLocWithReadDb and BioseqBlastEngineByLoc
 *
 * Revision 1.20  1997/06/20 13:11:53  madden
 * added prototype for AdjustOffSetsInSeqAlign
 *
 * Revision 1.19  1997/05/20 17:51:02  madden
 * Added prototypes for BLASTSetUpSearchByLoc, BlastTwoSequencesByLoc and BlastSequencesOnTheFlyByLoc
 *
 * Revision 1.18  1997/03/11 14:38:40  madden
 * Added BlastSequencesOnTheFly prototype.
 *
 * Revision 1.17  1997/03/07  21:58:36  madden
 * Added Boolean gapped argument to BLASTOptionNew.
 *
 * Revision 1.16  1997/03/03  21:48:52  madden
 * *** empty log message ***
 *
 * Revision 1.15  1997/03/03  14:48:57  madden
 * Changes prototype for SumBlastGetGappedAlignmentTraceback
 *
 * Revision 1.14  1997/02/26  20:37:31  madden
 * Added *error_returns to BioseqBlastEngine.
 *
 * Revision 1.13  1997/02/18  17:58:52  madden
 * Added BioseqBlastEngine.
 *
 * Revision 1.12  1997/02/10  20:03:58  madden
 * Added all_words to BLASTSetUpSearch.
 *
 * Revision 1.11  1997/02/05  19:54:59  madden
 * Removed defunct prototype.
 *
 * Revision 1.10  1997/02/03  13:02:12  madden
 * Added length to BLASTSubjectInfoNew call.
 *
 * Revision 1.9  1997/01/28  22:38:56  madden
 * Added function BLASTOptionValidate.
 *
 * Revision 1.8  1997/01/11  18:58:29  madden
 * Removed defunct PerformBlastSearch... functions.
 *
 * Revision 1.7  1996/12/23  22:02:05  madden
 * Changes to allow two sequences to be compared.
 *
 * Revision 1.6  1996/09/26  20:18:43  madden
 * Added prototype for ExperimentalLocalBlastSearch.
 *
 * Revision 1.5  1996/09/25  19:59:10  madden
 * Removed prototype for for GetParameterStack.
 *
 * Revision 1.4  1996/09/11  22:21:51  madden
 * *** empty log message ***
 *
 * Revision 1.3  1996/09/11  19:14:09  madden
 * Added BLAST_OptionsBlkPtr structure and use thereof.
 *
 * Revision 1.2  1996/08/23  16:30:54  shavirin
 * Fixed NT compiler warnings type mismatch
 *
 * Revision 1.1  1996/08/05  19:46:34  madden
 * Initial revision
 *
 * Revision 1.34  1996/08/02  14:20:06  madden
 * Add prototype for do_the_blast_run.
 *
 * Revision 1.33  1996/06/26  15:53:54  madden
 * Second dropoff score parameter added.
 *
 * Revision 1.32  1996/06/24  17:57:39  madden
 * Added dropoff_number_of_bits argument to SetUpBlastSearch.
 *
 * Revision 1.31  1996/06/20  16:15:57  madden
 * Replaced int's with Int4's.
 *
 * Revision 1.30  1996/06/19  14:19:12  madden
 * Changed prototypes for SetUpBlastSearch.
 *
 * Revision 1.29  1996/06/06  17:54:09  madden
 * number_of_bits added to SetUpBlastSearch and SetUpBlastSearchWithReadDb.
 *
 * Revision 1.28  1996/06/04  15:33:12  madden
 * Changed prototype for GetParameterStack.
 *
 * Revision 1.27  1996/05/28  14:12:53  madden
 * prototype for GetParameterStack changed.
 *
 * Revision 1.26  1996/05/16  19:50:15  madden
 * Added documentation block.
 *
 * Revision 1.25  1996/05/16  13:29:38  madden
 * Changed prototype for SetUpBlastSearchWithReadDb.
 *
 * Revision 1.24  1996/05/03  19:55:07  madden
 * *** empty log message ***
 *
 * Revision 1.23  1996/05/01  14:58:22  madden
 * Changed prototypes for SetUpBlastSearchWithReadDb
 *
 * Revision 1.22  1996/04/24  12:52:06  madden
 * wordsize new parameter for SetUpBlastSearch.
 *
 * Revision 1.21  1996/04/22  21:40:07  madden
 * New prototypes for performing blast searches.
 *
 * Revision 1.20  1996/04/03  19:15:28  madden
 * *** empty log message ***
 *
 * Revision 1.19  1996/03/29  21:26:01  madden
 * Added prototype for SortSeqAlignByPvalue.
 *
 * Revision 1.18  1996/03/29  14:08:40  madden
 * prototype for SetUpBlastSearchWithReadDb added.
 *
 * Revision 1.17  1996/03/27  23:19:24  madden
 * changed parameters for PerformBlastSearch and Perform2PassBlastSearch.
 *
 * Revision 1.16  1996/03/26  19:36:42  madden
 * Changes to read databases formatted with formatdb.
 *
 * Revision 1.15  1996/03/25  16:34:19  madden
 * Changes to mimic old statistics.
 *
 * Revision 1.14  1996/02/28  21:36:54  madden
 * changes for discontiguous words.
 *
 * Revision 1.13  1996/02/15  15:22:52  madden
 * renamed Perform2HitBlastSearch to Perform2PassBlastSearch.
 *
 * Revision 1.12  1996/02/09  13:50:45  madden
 * Added prototype for Perform2HitBlastSearch.
 *
 * Revision 1.11  1996/02/05  18:46:30  madden
 * Added second threshold value to SetUpBlastSearch.
 *
 * Revision 1.10  1996/01/29  21:11:38  madden
 * Changes for MultipleHits BLAST.
 *
 * Revision 1.9  1996/01/23  16:31:23  madden
 *  e_cutoff changed from BLAST_Score to double in SetUpBlastSearch.
 *
 * Revision 1.8  1996/01/17  23:18:01  madden
 * *** empty log message ***
 *
 * Revision 1.7  1996/01/17  17:00:24  madden
 * Added gap arguments to SetUpBlastSearch.
 *
 * Revision 1.6  1996/01/17  13:45:25  madden
 * Added "gap_decay_rate" to SetUpBlastSearch.
 *
 * Revision 1.5  1996/01/16  15:28:54  madden
 * Changed call to SetUpBlastSearch.
 *
 * Revision 1.4  1995/12/30  19:22:04  madden
 * Added prototype for PerformBlastSearch.
 *
 * Revision 1.3  1995/12/30  18:39:27  madden
 * Added prototype for SetUpBlastSearch.
 *
 * Revision 1.2  1995/12/19  22:31:05  madden
 * *** empty log message ***
 *
 * Revision 1.1  1995/12/08  15:48:23  madden
 * Initial revision
 *
 * */
#ifndef __BLAST__
#define __BLAST__

#include <ncbi.h>
#include <blastdef.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
	Call this function to allocate the "options" structure.  The
	fields will be filled in with the default values, which depend
	on the program.
*/
BLAST_OptionsBlkPtr LIBCALL BLASTOptionNew PROTO((CharPtr progname, Boolean gapped));

BLAST_OptionsBlkPtr LIBCALL BLASTOptionDelete PROTO((BLAST_OptionsBlkPtr));

BLAST_OptionsBlkPtr LIBCALL BLASTOptionValidate PROTO((BLAST_OptionsBlkPtr options, CharPtr progname));

Int2 LIBCALL BLASTOptionValidateEx PROTO((BLAST_OptionsBlkPtr options, CharPtr progname, ValNodePtr PNTR error_return));

Int2 LIBCALL BLASTOptionSetGapParams PROTO((BLAST_OptionsBlkPtr options, CharPtr matrix, Int4 open, Int4 extended));


/* 
	the setup functions, call before running blast.
*/

BlastSearchBlkPtr LIBCALL BLASTSetUpSearchWithReadDb PROTO((BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, CharPtr dbname, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *index_callback)PROTO((Int4 done, Int4 positives))));

BlastSearchBlkPtr LIBCALL BLASTSetUpSearchWithReadDbEx PROTO((BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, CharPtr dbname, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *index_callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total));

BlastSearchBlkPtr LIBCALL BLASTSetUpSearchByLocWithReadDb PROTO((SeqLocPtr slp, CharPtr prog_name, Int4 qlen, CharPtr dbname, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *index_callback)PROTO((Int4 done, Int4 positives))));

BlastSearchBlkPtr LIBCALL BLASTSetUpSearchByLocWithReadDbEx PROTO((SeqLocPtr slp, CharPtr prog_name, Int4 qlen, CharPtr dbname, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *index_callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total));

BlastSearchBlkPtr LIBCALL BLASTSetUpSearch PROTO((BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, Int8 dblen, BlastAllWordPtr all_words, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *index_callback)PROTO((Int4 done, Int4 positives))));

BlastSearchBlkPtr LIBCALL BLASTSetUpSearchByLoc PROTO((SeqLocPtr query_slp, CharPtr prog_name, Int4 qlen, Int8 dblen, BlastAllWordPtr all_words, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *index_callback)PROTO((Int4 done, Int4 positives))));

Int2 LIBCALL BLASTSetUpSearchInternalByLoc  PROTO((BlastSearchBlkPtr search, SeqLocPtr query_slp, BioseqPtr query_bsp, CharPtr prog_name, Int4 qlen, BLAST_OptionsBlkPtr options, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives))));
/*
	Use these function to perform the search.
*/
Int2 LIBCALL BLASTPerform2PassSearch PROTO((BlastSearchBlkPtr search, Int4 subject_length, Uint1Ptr subject_seq));

Int2 LIBCALL BLASTPerformFinalSearch PROTO((BlastSearchBlkPtr search, Int4 subject_length, Uint1Ptr subject_seq));


BLASTSubjectInfoPtr LIBCALL BLASTSubjectInfoNew PROTO((SeqIdPtr sip, CharPtr defline, Int4 length));

BLASTSubjectInfoPtr LIBCALL BLASTSubjectInfoDestruct PROTO((BLASTSubjectInfoPtr subject_info));

void LIBCALL do_the_blast_run PROTO((BlastSearchBlkPtr search));

Int2 LIBCALL BlastSequenceAddSequence PROTO((BlastSequenceBlkPtr sequence_blk, Uint1Ptr sequence, Uint1Ptr sequence_start, Int4 length, Int4 original_seq, Int4 effective_length));

BlastSequenceBlkPtr LIBCALL
BlastSequenceBlkDestruct PROTO((BlastSequenceBlkPtr seq_blk));

void BLASTUpdateSeqIdInSeqInt(SeqLocPtr mask, SeqIdPtr sip);

/*
	Blast two sequences and return a SeqAlign. For blastx program first
	sequence must be nucleotide, second protein.
*/
SeqAlignPtr LIBCALL BlastTwoSequences PROTO((BioseqPtr bsp1, BioseqPtr bsp2, CharPtr progname, BLAST_OptionsBlkPtr options));

SeqAlignPtr LIBCALL BlastTwoSequencesByLoc PROTO((SeqLocPtr slp1, SeqLocPtr slp2, CharPtr progname, BLAST_OptionsBlkPtr options));


SeqAlignPtr LIBCALL BlastTwoSequencesByLocEx PROTO((SeqLocPtr slp1, SeqLocPtr slp2, CharPtr progname, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns));

SeqAlignPtr LIBCALL BlastTwoSequencesByLocWithCallback PROTO((SeqLocPtr slp1, SeqLocPtr slp2, CharPtr progname, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *handle_results)PROTO((VoidPtr srch))));

SeqAlignPtr LIBCALL BlastTwoSequencesEx PROTO((BioseqPtr bsp1, BioseqPtr bsp2, CharPtr progname, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns));

SeqAlignPtr LIBCALL BlastTwoSequencesWithCallback PROTO((BioseqPtr bsp1, BioseqPtr bsp2, CharPtr progname, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *handle_results)PROTO((VoidPtr srch))));

SeqAlignPtr LIBCALL BlastSequencesOnTheFly PROTO((BlastSearchBlkPtr search, BioseqPtr subject_bsp));

SeqAlignPtr LIBCALL BlastSequencesOnTheFlyByLoc PROTO((BlastSearchBlkPtr search, SeqLocPtr subject_slp));

BlastSearchBlkPtr LIBCALL
BlastQuerySequenceSetUp PROTO((BioseqPtr bsp, CharPtr progname,  
			       BLAST_OptionsBlkPtr options));

BlastSearchBlkPtr LIBCALL
BlastSequencesOnTheFlyEx PROTO((BlastSearchBlkPtr search, BioseqPtr subject_bsp));
   
SeqAlignPtr LIBCALL SumBlastGetGappedAlignmentTraceback PROTO((BlastSearchBlkPtr search, Int4 hit_number, Boolean reverse, Boolean ordinal_number, Uint1Ptr subject, Int4 subject_length));


/*
	Performs a complete BLAST search and returns a SeqAnlign.
*/

SeqAlignPtr LIBCALL BioseqBlastEngine PROTO((BioseqPtr bsp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives))));

SeqAlignPtr LIBCALL BioseqBlastEngineWithCallback PROTO((BioseqPtr bsp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), int (LIBCALLBACK *handle_results)PROTO((VoidPtr srch))));

SeqAlignPtr LIBCALL BioseqBlastEngineEx PROTO((BioseqPtr bsp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total));

SeqAlignPtr LIBCALL BioseqBlastEngineByLoc PROTO((SeqLocPtr slp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives))));

SeqAlignPtr LIBCALL BioseqBlastEngineByLocEx PROTO((SeqLocPtr slp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total));

SeqAlignPtr LIBCALL BioseqBlastEngineByLocWithCallback PROTO((SeqLocPtr slp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total, int (LIBCALLBACK *handle_results)PROTO((VoidPtr srch))));
/*
	Prints error messages. 
*/

void LIBCALL BlastErrorPrint PROTO((ValNodePtr error_return));
void LIBCALL BlastErrorPrintExtra PROTO((ValNodePtr error_return,  Boolean errpostex, FILE* fp));

/*
	Prints some header information.
*/

CharPtr LIBCALL BlastGetVersionNumber PROTO((void));

CharPtr LIBCALL BlastGetReference PROTO((Boolean html));

Boolean LIBCALL BlastPrintReference PROTO((Boolean html, Int4 line_length, FILE *outfp));
Boolean LIBCALL MegaBlastPrintReference PROTO((Boolean html, Int4 line_length, FILE *outfp));

CharPtr LIBCALL BlastGetPhiReference PROTO((Boolean html));

Boolean LIBCALL BlastPrintPhiReference PROTO((Boolean html, Int4 line_length, FILE *outfp));

Boolean BlastPrintVersionInfo PROTO((CharPtr program, Boolean html, FILE *outfp));
Boolean BlastPrintVersionInfoEx PROTO((CharPtr program, Boolean html, CharPtr version, CharPtr date, FILE *outfp));

CharPtr LIBCALL BlastGetReleaseDate PROTO((void));

Uint1 LIBCALL BlastGetProgramNumber PROTO((CharPtr blast_program));
CharPtr LIBCALL BlastGetProgramName PROTO((Uint1 number));



BlastHitRangePtr LIBCALL BlastHitRangeDestruct PROTO((BlastHitRangePtr old));
BlastHitRangePtr LIBCALL BlastHitRangeNew PROTO((Int4 total));

BlastHitRangePtr LIBCALL BioseqHitRangeEngineCore PROTO((BlastSearchBlkPtr search, BLAST_OptionsBlkPtr options));

SeqLocPtr LIBCALL BioseqHitRangeEngineByLoc PROTO((SeqLocPtr slp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total));

SeqLocPtr LIBCALL BioseqHitRangeEngine PROTO((BioseqPtr bsp, CharPtr progname, CharPtr database, BLAST_OptionsBlkPtr options, ValNodePtr *other_returns, ValNodePtr *error_returns, int (LIBCALLBACK *callback)PROTO((Int4 done, Int4 positives)), SeqIdPtr seqid_list, BlastDoubleInt4Ptr gi_list, Int4 gi_list_total));

BLAST_ExtendWordParamsPtr BLAST_ExtendWordParamsNew (Int4 qlen, Boolean multiple_hits, Int4 window_size);
BLAST_ExtendWordPtr BLAST_ExtendWordNew (BLAST_ExtendWordParamsPtr ewp_params);
BLAST_ExtendWordPtr LIBCALL BLAST_ExtendWordDestruct (BLAST_ExtendWordPtr ewp);


void LIBCALL updateLambdaK PROTO((BlastMatrixRescalePtr matrix_rescale, Boolean position_dependent));

BlastSearchBlkPtr GreedyAlignMemAlloc PROTO((BlastSearchBlkPtr search));

Boolean parse_blast_options(BLAST_OptionsBlkPtr options, CharPtr string_options, CharPtr PNTR error_message, CharPtr PNTR database, Int4Ptr descriptions, Int4Ptr alignments);

Int2
BlastNtWordExtend PROTO((BlastSearchBlkPtr search, Int4 q_off, Int4 s_off, BLAST_Diag real_diag, Int2 context));

Boolean MegaBlastBuildLookupTable PROTO((BlastSearchBlkPtr search));

Int2 DefineToFrame PROTO((Uint1 define));

CharPtr BLASTGetDatabaseTitleFromSeqAnnot PROTO((SeqAnnotPtr seqannot));
void BLASTAddBlastDBTitleToSeqAnnot PROTO((SeqAnnotPtr seqannot, 
                                           CharPtr title));

Int4 reverse_seq (Uint1 *seq, Uint1 *pos, Uint1 *target);

int LIBCALLBACK RPSResultHspScoreCmp(VoidPtr v1, VoidPtr v2);

SeqAlignPtr 
BlastPruneSeqAlignByGiList PROTO((SeqAlignPtr seqalign, Int4Ptr gi_list, Int4 gi_list_total));
SeqAlignPtr 
BlastPruneSeqAlignByEvalueRange PROTO((SeqAlignPtr seqalign, FloatHi expect_low, FloatHi expect_high));

BlastDoubleInt4Ptr GetGisFromFile PROTO((CharPtr file_name, Int4Ptr gi_list_size));
void PrintTabularOutputHeader PROTO((CharPtr blast_database, BioseqPtr query_bsp,
                              SeqLocPtr query_slp, CharPtr blast_program,
                              Int4 iteration, Boolean believe_query,
                                     FILE *outfp));

/* ------ Functions related to Smith-Waterman algorithm ------ */

Nlm_FloatHi BLbasicSmithWatermanScoreOnly(Uint1 * matchSeq, Int4 matchSeqLength, Uint1 *query, Int4 queryLength, BLAST_Score **matrix, Int4 gapOpen, Int4 gapExtend,  Int4 *matchSeqEnd, Int4 *queryEnd, Int4 *score, BLAST_KarlinBlkPtr kbp, Nlm_FloatHi effSearchSpace, Boolean positionSpecific);

Int4 BLSmithWatermanFindStart(Uint1 * matchSeq, Int4 matchSeqLength, Uint1 *query, Int4 queryLength, BLAST_Score **matrix, Int4 gapOpen, Int4 gapExtend,  Int4 matchSeqEnd, Int4 queryEnd, Int4 score, Int4 *matchSeqStart, Int4 *queryStart, Boolean positionSpecific);

/* ----------------------------------------------------------- */

/* DEBUG */
/* time mesuaring utilities */

/* #define BLAST_TIMER */

#ifdef BLAST_TIMER
clock_t last_clock;
#define	start_timer	last_clock = clock();

#define	stop_timer(msg) { \
    clock_t	current_clock = clock(), since; \
    since = current_clock - last_clock; \
    fprintf(stderr, "TIME [%s] - %5.2f\n", msg, ((double) since) / CLOCKS_PER_SEC); \
    last_clock = current_clock; \
}
#else
#define	start_timer ;
#define	stop_timer ;
#endif
/* end of DEBUG block */


#ifdef __cplusplus
}
#endif
#endif /* !__BLAST__ */
