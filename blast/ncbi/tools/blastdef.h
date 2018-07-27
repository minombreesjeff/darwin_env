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

File name: blastdef.h

Author: Tom Madden

Contents: #defines and definitions for structures used by BLAST.

******************************************************************************/
/* $Revision: 6.162 $ 
* $Log: blastdef.h,v $
* Revision 6.162  2005/04/25 14:16:36  coulouri
* set db_chunk_size adaptively
*
* Revision 6.161  2005/01/10 18:52:29  coulouri
* fixes from morgulis to allow concatenation of >255 queries in [t]blastn
*
* Revision 6.160  2004/11/19 13:22:05  madden
* Remove no_check_score completely (from Mike Gertz)
*
* Revision 6.159  2004/09/28 16:02:29  papadopo
* From Michael Gertz: Changed the "sumscore" field of an HSP to
* "xsum" to represent a normalized sum score of linked HSPs;
* the normalized score is more appropriate/useful in doing linking.
*
* Revision 6.158  2004/06/30 12:29:00  madden
* Removed typedef for BlastPruneSapStruct and some defines, moved to blfmtutl.h
*
* Revision 6.157  2004/04/30 12:45:45  coulouri
* bump version to 2.2.9
*
* Revision 6.156  2004/04/13 21:02:52  madden
* Add ignore_gilist Boolean to Options for use in formatting
*
* Revision 6.155  2004/02/04 15:35:03  camacho
* Rollback to fix problems in release 2.2.7
*
* Revision 6.154  2004/01/27 20:46:06  dondosha
* Allow values 0, 1, 2 for no_traceback megablast option
*
* Revision 6.153  2004/01/05 22:09:26  madden
* Put back dashes in date
*
* Revision 6.152  2004/01/02 13:44:32  coulouri
* Revert to hardcoded BLAST_RELEASE_DATE
*
* Revision 6.151  2003/12/29 15:51:18  coulouri
* Bump version, use __DATE__ instead of hardcoded date
*
* Revision 6.150  2003/11/06 19:52:13  dondosha
* Added error MBTemplateType, so it can be returned when wordsize/template length combination is wrong
*
* Revision 6.149  2003/10/02 19:30:11  madden
* add field seAlign to SWResults for use in kappa.c
*
* Revision 6.148  2003/04/09 14:18:34  madden
* Update version and release-date
*
* Revision 6.147  2003/03/26 15:45:48  boemker
* Documented relationships among BLAST_OptionsBlk, BLAST_WizardOptionsBlk,
* and BLAST_WizardOptionsMask.
*
* Revision 6.146  2003/03/25 22:23:06  boemker
* Replaced cutoff_s2, which isn't used, with cutoff_s, which is.
* Added query_lcase_mask.
*
* Revision 6.145  2003/03/25 19:58:18  boemker
* Moved code to initialize search options from blastcgicmd.cpp to here, as
* BLAST_Wizard et al.
*
* Revision 6.144  2003/03/24 19:42:14  madden
* Changes to support query concatenation for blastn and tblastn
*
* Revision 6.143  2002/11/22 23:28:43  dondosha
* Use array of structures instead of array of pointers for initial offset pairs
*
* Revision 6.142  2002/11/16 17:12:55  madden
* Change version and date
*
* Revision 6.141  2002/11/04 22:51:13  dondosha
* Changed FloatHi pvalue to Int4 num_ident in HSP structures
*
* Revision 6.140  2002/09/13 19:11:02  camacho
* Added rps_qlen field
*
* Revision 6.139  2002/09/11 21:15:23  camacho
* Removed obsolete #define and comment about BlastSeqIdList structure
*
* Revision 6.138  2002/09/11 20:46:25  camacho
* Removed deprecated BlastSeqIdListPtr code
*
* Revision 6.137  2002/08/26 15:49:51  madden
* Change release date and version
*
* Revision 6.136  2002/08/09 19:39:20  camacho
* Added constants for some blast search parameters
*
* Revision 6.135  2002/06/21 21:43:01  camacho
* Removed obsolete BlastSeqIdList structure and functions
*
* Revision 6.134  2002/05/17 21:40:13  dondosha
* Added 2 optimal Mega BLAST word templates for length 21
*
* Revision 6.133  2002/05/14 22:20:20  dondosha
* Renamed maximal discontiguous template type into optimal
*
* Revision 6.132  2002/04/23 20:59:53  madden
* Change version and date for release
*
* Revision 6.131  2002/04/09 18:16:43  dondosha
* Added more options/parameters for megablast
*
* Revision 6.130  2002/03/28 18:53:18  madden
* Add ValNodePtr mask1 to BlastSearch structure
*
* Revision 6.129  2001/12/28 20:38:39  dondosha
* Moved Mega BLAST related parameters into a separate structure
*
* Revision 6.128  2001/12/28 18:01:26  dondosha
* Added field scoreThisAlign to SWResults to allow more tie-breaking options
*
* Revision 6.127  2001/12/14 22:05:40  madden
* Changed version and release date
*
* Revision 6.126  2001/09/11 14:28:31  madden
* Added timed_out Boolean to SearchBlk
*
* Revision 6.125  2001/09/07 14:46:44  dondosha
* Roll back removal of threshold_first from functions and structures
*
* Revision 6.124  2001/09/06 20:24:34  dondosha
* Removed threshold_first
*
* Revision 6.123  2001/08/06 12:50:51  madden
* Change release date
*
* Revision 6.122  2001/07/12 19:50:24  madden
* Changed release date
*
* Revision 6.121  2001/06/28 13:42:09  madden
* Fixes to prevent overflow on number of hits reporting
*
* Revision 6.120  2001/06/12 19:48:56  madden
* Introduce total_hsp_limit, check before making SeqAlign
*
* Revision 6.119  2001/04/13 20:56:08  madden
* Updated version to 2.2.1, changed date
*
* Revision 6.118  2001/04/11 20:56:21  madden
* Added scalingFactor for rpsblast, changed release date
*
* Revision 6.117  2001/03/30 21:58:18  madden
* Change release date and version
*
* Revision 6.116  2001/03/27 21:27:01  madden
* Minor efficiency in how lookup table is made
*
* Revision 6.115  2001/03/19 18:52:57  madden
* Add base_offset element to structure for BlastHitRange
*
* Revision 6.114  2001/02/07 21:05:33  dondosha
* Added an output stream to BlastOptionsBlk
*
* Revision 6.113  2000/12/21 22:28:17  dondosha
* Added option and parameter for percent identity cutoff
*
* Revision 6.112  2000/11/29 16:17:56  dondosha
* Added a definition of small structure BLASTHSPSegment
*
* Revision 6.111  2000/11/14 18:14:00  madden
* release date to Nov-13-2000
*
* Revision 6.110  2000/11/08 22:18:05  dondosha
* Added longest_intron integer option and parameter
*
* Revision 6.109  2000/11/07 16:30:25  madden
* Introduce intermediate score (before linking of HSPs) for blastx and tblastn
*
* Revision 6.108  2000/11/03 20:16:24  dondosha
* Changed one_line_results option and parameter to more meaningful no_traceback
*
* Revision 6.107  2000/11/01 16:25:56  madden
* Changes from Futamura for psitblastn
*
* Revision 6.106  2000/10/18 19:53:19  shavirin
* Empty log message.
*
* Revision 6.105  2000/10/18 19:17:56  shavirin
* Changed BLAST_ENGINE_VERSION and BLAST_RELEASE_DATE
*
* Revision 6.104  2000/10/05 19:50:49  dondosha
* Added mb_result_struct to the BlastSearchBlk to be used instead of result_struct in Mega BLAST
*
* Revision 6.103  2000/09/28 14:48:20  dondosha
* Added exact_match_array to hitlist structure for megablast initial hits
*
* Revision 6.102  2000/09/21 19:16:30  madden
* increase AWAKE_THR_MIN_SIZE by 100
*
* Revision 6.101  2000/08/29 19:35:49  madden
* Add gilist_not_owned to blast_gi_list
*
* Revision 6.100  2000/08/08 20:37:21  madden
* increase version number to 2.1.1 and release date
*
* Revision 6.99  2000/07/17 14:05:22  shavirin
* Added parameter Out-Of-Frame shift penalty and query DNAP sequence
*
* Revision 6.98  2000/07/11 18:38:02  madden
* decreased size of helper array, added prefetch to BlastGappedScoreInternal
*
* Revision 6.97  2000/07/11 17:16:20  shavirin
* Added new parameter is_ooframe for Out-Of-Frame gapping algorithm.
*
* Revision 6.96  2000/07/10 15:41:28  madden
* Add typedef for BLAST_HSP_helper
*
* Revision 6.95  2000/07/07 21:20:07  vakatov
* Get all "#include" out of the 'extern "C" { }' scope!
*
* Revision 6.94  2000/07/06 17:24:55  dondosha
* Added option and parameter megablast_full_deflines
*
* Revision 6.93  2000/06/30 17:52:45  madden
* Move AWAKE_THR_MIN_SIZE to blastdef.h
*
* Revision 6.92  2000/06/29 20:30:03  madden
* Update version and date
*
* Revision 6.91  2000/06/08 20:34:18  madden
* add explode_seqids option to show all ids in a defline
*
* Revision 6.90  2000/05/26 20:04:57  madden
* Raise version and date
*
* Revision 6.89  2000/05/12 19:40:59  dondosha
* Added qid_array element to BlastSearchBlk
*
* Revision 6.88  2000/05/01 19:04:31  shavirin
* Changed parameter level in BlastErrorMsg structure from Uint1 to Uint2.
*
* Revision 6.87  2000/04/21 20:48:05  madden
* Change version and date
*
* Revision 6.86  2000/04/06 14:47:10  madden
* Added original_expect_value
*
* Revision 6.85  2000/04/03 21:20:03  dondosha
* Added option and parameter is_neighboring
*
* Revision 6.84  2000/03/31 19:10:44  dondosha
* Changed some names related to MegaBlast
*
* Revision 6.83  2000/03/13 21:01:24  dondosha
* Added boolean option sort_gi_list to options block structure
*
* Revision 6.82  2000/02/29 18:17:23  shavirin
* Variable query_dna_mask changed to query_lcase_mask.
*
* Revision 6.81  2000/02/18 15:30:36  shavirin
* Added parameter query_dna_mask into options and parameters.
*
* Revision 6.80  2000/02/17 21:23:09  shavirin
* Added parameter is_rps_blast.
*
* Revision 6.79  2000/02/17 19:00:44  shavirin
* Removed theCacheSize parameter from everywhere.
*
* Revision 6.78  2000/02/15 19:06:09  shavirin
* Added parameter filter_string into BLAST_ParameterBlk structure.
*
* Revision 6.77  2000/02/02 18:21:51  madden
* Add LinkHelpStruct definition
*
* Revision 6.76  2000/02/02 16:52:43  dondosha
* Added option one_line_results to BLAST_OptionsBlk and BLAST_ParameterBlk
*
* Revision 6.75  2000/02/01 18:02:22  dondosha
* Added greedy alignment option to BLAST_OptionsBlk and query context offsets array to BlastSearchBlk
*
* Revision 6.74  2000/01/26 22:00:52  madden
* Added subject_index field to SWResults
*
* Revision 6.73  2000/01/20 19:12:00  madden
* Change BLAST version and date
*
* Revision 6.72  2000/01/13 18:10:43  madden
* Fix problem with incorrect stat values for blastn and missing hits
*
* Revision 6.71  2000/01/11 17:02:48  shavirin
* Added element theCacheSize into BLAST_OptionsBlk and BLAST_ParameterBlk.
*
* Revision 6.70  1999/12/31 14:23:19  egorov
* Add support for using mixture of real and maks database with gi-list files:
* 1. Change logic of creating rdfp list.
* 2. BlastGetDbChunk gets real databases first, then masks.
* 3. Propoper calculation of database sizes using alias files.
* 4. Change to CommonIndex to support using of mask databases.
* 5. Use correct gis in formated output (BlastGetAllowedGis()).
* 6. Other small changes
*
* Revision 6.69  1999/12/21 20:04:15  egorov
* gi_list now contains start position for corresponding database
*
* Revision 6.68  1999/11/30 18:23:08  shavirin
* Added parameter max_num_patterns to the BLAST_OptionsBlkPtr structure
*
* Revision 6.67  1999/11/15 22:03:31  madden
* added Boolean isFirstAlignment to SWResults
*
* Revision 6.66  1999/11/12 20:57:39  shavirin
* Added parameter use_best_align into BLAST_ParameterBlkPtr
*
* Revision 6.65  1999/11/12 16:37:30  shavirin
* Added new option use_best_align into Blast options.
*
* Revision 6.64  1999/10/26 20:45:19  madden
* Add use_real_db_size option
*
* Revision 6.63  1999/10/05 17:42:54  shavirin
* Removed global variables from blast.c
*
* Revision 6.62  1999/09/28 20:14:31  madden
* Joerg changes to mimize cache misses
*
* Revision 6.61  1999/08/31 13:42:23  madden
* Moved SWResults to blastdef.h from profiles.h
*
* Revision 6.60  1999/08/27 18:07:33  shavirin
* Passed parameter decline_align from top to the engine.
*
* Revision 6.59  1999/08/26 14:56:49  madden
* Raise version and date
*
* Revision 6.58  1999/08/26 14:55:16  madden
* Fixed Int8 problem
*
* Revision 6.57  1999/08/20 19:47:41  madden
* removed version element
*
* Revision 6.56  1999/08/17 18:37:12  shavirin
* Added phi_pattern element into options block.
*
* Revision 6.55  1999/08/17 14:02:34  madden
* add smith_waterman and tweak_parameters fields to Options
*
* Revision 6.54  1999/05/10 18:47:52  madden
* Changed version to 2.0.9
*
* Revision 6.53  1999/05/08 15:04:24  madden
* Changed version and release date
*
* Revision 6.52  1999/04/23 19:25:01  madden
* Fixes a prototype complaint
*
* Revision 6.51  1999/04/23 16:45:54  madden
* call BQ_IncSemaphore as callback
*
* Revision 6.50  1999/04/22 16:46:13  shavirin
* Added semaphore ID to the search_blk structure.
*
* Revision 6.49  1999/04/01 21:42:47  madden
* Fix memory leaks when gi list is used
*
* Revision 6.48  1999/03/18 21:13:32  egorov
* The "output" filed added to search block.  This is VoidPtr and an application can
* use it as stream, ASNIO, etc to output blast results.
*
* Revision 6.47  1999/03/17 16:49:11  madden
* Removed comment within comment
*
* Revision 6.46  1999/02/17 13:23:01  madden
* Added hsp_num_max
*
* Revision 6.45  1999/01/28 16:04:56  madden
* do_not_reallocate Boolean for HSPs
*
* Revision 6.44  1999/01/26 17:56:37  madden
* query_id added to HitRange
*
* Revision 6.43  1999/01/05 13:57:19  madden
* Changed version and release date
*
 * Revision 6.42  1998/12/31 18:17:03  madden
 * Added strand option
 *
 * Revision 6.41  1998/12/29 17:45:06  madden
 * Add do_sum_stats flag
 *
 * Revision 6.40  1998/12/21 13:09:53  madden
 * Changed version and release date
 *
 * Revision 6.39  1998/11/04 01:36:05  egorov
 * Add support for entrez-query and org-name to blast3
 *
 * Revision 6.38  1998/09/16 18:58:57  madden
 * Changed release number and date
 *
 * Revision 6.37  1998/09/14 15:11:15  egorov
 * Add support for Int8 length databases; remove unused variables
 *
 * Revision 6.36  1998/07/30 19:00:32  madden
 * Change to allow search of subset of database
 *
 * Revision 6.35  1998/07/28 21:17:59  madden
 * Added do_not_reevaluate
 *
 * Revision 6.34  1998/07/25 14:26:38  madden
 * Added comments
 *
 * Revision 6.33  1998/07/22 12:16:25  madden
 * Added handle_results
 *
 * Revision 6.32  1998/07/21 20:58:04  madden
 * Changes to allow masking at hash only
 *
 * Revision 6.31  1998/07/17 15:39:56  madden
 * Changes for Effective search space.
 *
 * Revision 6.30  1998/07/14 20:17:05  egorov
 * Add two new parameters (gilist and gifile) to BLAST_OptionsBlk
 *
 * Revision 6.29  1998/06/17 18:10:07  madden
 * Added isPatternSearch to Options
 *
 * Revision 6.28  1998/06/12 16:08:49  madden
 * BlastHitRange stuff
 *
 * Revision 6.27  1998/05/28 19:59:16  madden
 * Added typedef for BLASTHeapStruct
 *
 * Revision 6.26  1998/05/17 16:28:43  madden
 * Allow changes to filter options and cc filtering.
 *
 * Revision 6.25  1998/05/05 13:56:38  madden
 * Raised version to 2.0.5 and changed date
 *
 * Revision 6.24  1998/04/24 19:27:05  madden
 * Added BlastMatrixRescalePtr
 *
 * Revision 6.23  1998/04/01 22:47:14  madden
 * Added query_invalid flag
 *
 * Revision 6.22  1998/03/24 15:38:22  madden
 * Use BlastDoubleInt4Ptr to keep track of gis and ordinal_ids
 *
 * Revision 6.21  1998/03/18 14:14:20  madden
 * Support random access by gi list
 *
 * Revision 6.20  1998/03/14 18:29:21  madden
 * Added BlastSeqIdListPtr
 *
 * Revision 6.19  1998/02/26 22:34:37  madden
 * Changes for 16 bit windows
 *
 * Revision 6.18  1998/02/26 19:10:37  madden
 * Removed elements with BLAST_COLLECT_SPECIAL_STATS defines
 *
 * Revision 6.17  1998/02/24 22:46:29  madden
 * Added perform_culling Boolean and changed release date
 *
 * Revision 6.16  1998/02/19 17:17:10  madden
 * Use of Int4 rather than Int2 when pruning SeqAlign
 *
 * Revision 6.15  1998/01/05 16:46:52  madden
 * One or both strands can be searched, as opposed to only both, changes to number of contexts
 *
 * Revision 6.14  1997/12/23 19:14:14  madden
 * release version to 2.0.4
 *
 * Revision 6.13  1997/12/23 18:12:32  madden
 * Changes for range-dependent blast
 *
 * Revision 6.12  1997/12/12 20:38:02  madden
 * Fix to comments
 *
 * Revision 6.11  1997/12/11 22:20:16  madden
 * Corrected blast_type defines
 *
 * Revision 6.10  1997/12/10 22:41:40  madden
 * program number defines
 *
 * Revision 6.9  1997/11/14 21:30:16  madden
 * Changed version and date
 *
 * Revision 6.8  1997/10/26 17:26:59  madden
 * Changes for range dependent limits
 *
 * Revision 6.7  1997/10/01 13:35:28  madden
 * Changed BLAST_VERSION to BLAST_ENGINE_VERSION
 *
 * Revision 6.6  1997/09/22 17:36:24  madden
 * MACROS for position-specific matrices from Andy Neuwald
 *
 * Revision 6.5  1997/09/18 22:22:12  madden
 * Added prune functions
 *
 * Revision 6.4  1997/09/11 18:49:26  madden
 * Changes to enable searches against multiple databases.
 *
 * Revision 6.3  1997/09/10 21:27:57  madden
 * Changes to set CPU limits
 *
 * Revision 6.2  1997/09/03 19:06:35  madden
 * changed BLAST_VERSION and BLAST_RELEASE_DATE
 *
 * Revision 6.1  1997/08/27 14:46:48  madden
 * Changes to enable multiple DB searches
 *
 * Revision 6.0  1997/08/25 18:52:32  madden
 * Revision changed to 6.0
 *
 * Revision 1.63  1997/08/20 21:43:10  madden
 * Updated release date
 *
 * Revision 1.62  1997/07/21 17:37:15  madden
 * Added define for BLAST_RELEASE_DATE
 *
 * Revision 1.61  1997/07/18 20:55:45  madden
 * Added BLAST_VERSION
 *
 * Revision 1.60  1997/07/15 20:36:43  madden
 * Added ValNodePtr mask
 *
 * Revision 1.59  1997/07/14 15:33:00  madden
 * typedef for BlastErrorMsg
 *
 * Revision 1.58  1997/05/22 21:24:52  madden
 * Added support for final gapX dropoff value
 *
 * Revision 1.57  1997/05/20 17:51:33  madden
 * Added element SeqLocPtr query_slp to BlastSearch
 *
 * Revision 1.56  1997/05/06 22:19:35  madden
 * Added use_large_gaps and subject_length
 *
 * Revision 1.55  1997/04/09  20:01:53  madden
 * Added seqid_list to SearchBlk
 *
 * Revision 1.54  1997/04/03  19:48:13  madden
 * Changes to use effective database length instead of the length of each
 * sequence in statistical calculations.
 *
 * Revision 1.53  1997/03/31  17:07:57  madden
 * Added BLAST_COLLECT_STATS define.
 *
 * Revision 1.52  1997/03/20  22:56:24  madden
 * Added gap_info to hsp.
 *
 * Revision 1.51  1997/03/14  22:06:11  madden
 * fixed MT bug in BlastReevaluateWithAmbiguities.
 *
 * Revision 1.50  1997/03/08  16:52:16  madden
 * y
 * Added discontinuous option to ParameterBlk.
 *
 * Revision 1.49  1997/02/25  19:17:05  madden
 * Added discontinuous flag to options.
 *
 * Revision 1.48  1997/02/23  16:44:47  madden
 * GapAlignBlkPtr added to search structure.
 *
 * Revision 1.47  1997/02/20  18:38:34  madden
 * Added Int4 db_length to Options block.
 *
 * Revision 1.46  1997/02/18  21:03:00  madden
 * Added #define FILTER_NONE 0.
 *
 * Revision 1.45  1997/02/17  17:40:18  madden
 * Added seqalign to ResultHitlistptr
 *
 * Revision 1.44  1997/02/11  19:30:54  madden
 * Added program_name to Options.
 *
 * Revision 1.43  1997/02/10  20:27:01  madden
 * Changed some CharPtr's into Uint1Ptr's.
 *
 * Revision 1.42  1997/02/10  20:14:23  madden
 * replaced doubles by Nlm_FloatHi's.
 *
 * Revision 1.41  1997/02/10  20:03:58  madden
 * Added specific to BlastAllWordsPtr.
 *
 * Revision 1.40  1997/02/10  15:36:40  madden
 * added posConverged to the BlastSearchBlk.
 *
 * Revision 1.39  1997/02/06  14:27:15  madden
 * Addition of BlastAllWord structure.
 *
 * Revision 1.38  1997/02/03  13:02:12  madden
 * Added length to BLASTSubjectInfo.
 *
 * Revision 1.37  1997/01/17  17:41:44  madden
 * Added flags for position based BLAST.
 *
 * Revision 1.36  1997/01/13  15:37:05  madden
 * Changed prototypes for star_callback and tick_callback.
 *
 * Revision 1.35  1997/01/11  18:22:10  madden
 * Changes to allow S2 to be set.
 *
 * Revision 1.34  1997/01/09  17:44:35  madden
 * Added "bit_score" to BLASTResultHsp.
 *
 * Revision 1.33  1996/12/27  20:44:10  madden
 * Chnages to require that part of the query be included.
 *
 * Revision 1.32  1996/12/23  14:04:44  madden
 * Added gap_trigger.
 *
 * Revision 1.31  1996/12/20  21:11:40  madden
 * Changes to allow multiple hits runs only.
 *
 * Revision 1.30  1996/12/18  14:33:13  madden
 * Added high_score element.
 *
 * Revision 1.29  1996/12/17  17:27:03  madden
 * Count number of attempted gappings.
 *
 * Revision 1.28  1996/12/17  13:47:57  madden
 * Added star_proc.
 *
 * Revision 1.27  1996/12/16  14:35:48  madden
 * Added gapped_calculation Boolean
 *
 * Revision 1.26  1996/12/13  22:00:23  madden
 * Corrected starting point for gapped extension with traceback.
 *
 * Revision 1.25  1996/12/13  18:13:56  madden
 * Added tick callback functions
 *
 * Revision 1.24  1996/12/13  15:09:31  madden
 * Changes to parameters used for gapped extensions.
 *
 * Revision 1.23  1996/12/09  23:24:05  madden
 * Added parameters to control which sequences get a gapped alignment.
 *
 * Revision 1.22  1996/12/08  15:19:59  madden
 * Added parameters for gapped alignments.
 *
 * Revision 1.21  1996/11/27  21:56:57  madden
 * Removed define for XNU.
 *
 * Revision 1.20  1996/11/18  18:07:57  madden
 * *** empty log message ***
 *
 * Revision 1.19  1996/11/18  17:28:13  madden
 * Added BLAST_SEARCH_ALLOC_TRANS_INFO define.
 *
 * Revision 1.18  1996/11/18  15:45:40  madden
 * Defines for filter type added (by S. Shavirin),.
 *
 * Revision 1.17  1996/11/15  17:54:54  madden
 * Added support for alternate genetic codes for blastx, tblast[nx].
 *
 * Revision 1.16  1996/11/13  22:35:18  madden
 * Added genetic_code and db_genetic_code elements to blastdef.h
 *
 * Revision 1.15  1996/11/12  16:21:53  madden
 * Added context_factor
 *
 * Revision 1.14  1996/11/06  22:10:01  madden
 * translation_buffer changed from CharPtr to Uint1Ptr.
 *
 * Revision 1.13  1996/11/04  16:59:43  madden
 * Added translation_table and translation_table_rc elements
 * to BlastSearchBlk.
 *
 * Revision 1.12  1996/10/03  20:49:29  madden
 * Added xsum member to HSP_Link structure.
 * ,.
 *
 * Revision 1.11  1996/10/01  21:24:02  madden
 * Added e2.
 *
 * Revision 1.10  1996/09/26  13:02:32  madden
 * Removed ifdef for BLAST_COLLECT_STATS with counters.
 *
 * Revision 1.9  1996/09/12  21:13:46  madden
 * *** empty log message ***
 *
 * Revision 1.8  1996/09/11  22:21:51  madden
 * *** empty log message ***
 *
 * Revision 1.7  1996/09/11  19:14:09  madden
 * Added BLAST_OptionsBlkPtr structure and use thereof.
 *
 * Revision 1.6  1996/08/14  18:16:13  madden
 * removed frame from Context.
 *
 * Revision 1.5  1996/08/14  17:19:02  madden
 * Added frame to BlastSeqBlkPtr.
 *
 * Revision 1.4  1996/08/13  15:26:29  madden
 * Changes for tblastn.
 *
 * Revision 1.3  1996/08/09  22:11:12  madden
 * Added original_sequence to BlastSequenceBlk.
 *
 * Revision 1.2  1996/08/07  14:24:42  madden
 * Removed include for blast18p.h and objblst2.h
 *
 * Revision 1.1  1996/08/05  20:32:18  madden
 * Initial revision
 *
 * Revision 1.51  1996/08/02  14:20:06  madden
 * Removed multiproc strucutre.
 *
 * Revision 1.50  1996/07/31  13:09:17  madden
 * Changes for threaded blast.
 *
 * Revision 1.49  1996/07/24  12:01:28  madden
 * Changes for blastx
 *
 * Revision 1.48  1996/07/18  22:00:49  madden
 * Addition of BLAST_ExtendWordParams structure.
 *
 * Revision 1.47  1996/07/18  13:36:34  madden
 * Addition of the BLASTContextStructPtr.
 *
 * Revision 1.46  1996/07/16  14:37:42  madden
 * Removed _blast_link_structure .
 *
 * Revision 1.45  1996/07/11  16:03:58  madden
 * SaveCurrentHitlist keeps track of which set an HSP belongs to.
 *
 * Revision 1.44  1996/07/02  14:33:16  madden
 * Added hspcnt_max.
 *
 * Revision 1.43  1996/07/02  12:04:15  madden
 * HSP's saved on array, rather than linked list.
 *
 * Revision 1.42  1996/06/26  19:38:12  madden
 * Removed ifdef.
 *
 * Revision 1.41  1996/06/24  20:26:46  madden
 * Added dropoff_1st_pass and dropoff_2nd_pass to ParameterBlkPtr.
 *
 * Revision 1.40  1996/06/24  17:58:21  madden
 * Removed X_set parameter, added right and left dropoff's.
 *
 * Revision 1.39  1996/06/20  16:15:57  madden
 * Replaced int's with Int4's.
 *
 * Revision 1.38  1996/06/19  14:19:53  madden
 * Added define for BLASTSubjectInfoPtr.
 *
 * Revision 1.37  1996/06/17  19:03:07  madden
 * Rmoved unused structure.
 *
 * Revision 1.36  1996/06/14  17:58:13  madden
 * Changes to avoid nulling out arrays for every sequence.
 *
 * Revision 1.35  1996/06/13  21:03:06  madden
 * Added actual_window element to ExtendWord structure.
 *
 * Revision 1.34  1996/06/11  17:58:31  madden
 * Changes to allow shorter arrays for multiple hits type blast.
 *
 * Revision 1.33  1996/06/10  16:52:16  madden
 * Use bit-shifting and masking instead of dividing and remainder.
 *
 * Revision 1.32  1996/06/10  13:44:07  madden
 * Changes to reduce the size of the "already visited" array.
 *
 * Revision 1.31  1996/06/06  17:55:16  madden
 * Added number_of_bits to ParameterBlkPtr.
 *
 * Revision 1.30  1996/06/06  13:23:17  madden
 * Added elements cutoff_big_gap and ignore_small_gaps to ParameterBlkPt.
 *
 * Revision 1.29  1996/05/29  12:44:04  madden
 * Added structure BlastTimeKeeper.
 *
 * Revision 1.28  1996/05/28  14:16:32  madden
 * Added Int4's to collect statistics info.
 *
 * Revision 1.27  1996/05/23  21:55:04  madden
 * Removed unused variable initlen
 *
 * Revision 1.26  1996/05/23  21:48:23  madden
 * Removed unused defines.
 *
 * Revision 1.25  1996/05/16  19:51:09  madden
 * Added documentation block.
 *
 * Revision 1.24  1996/05/16  13:29:38  madden
 * Added defines for contiguous or discontiguous calls.
 *
 * Revision 1.23  1996/05/01  15:00:00  madden
 * Added BlastResults sturcture defs.
 *
 * Revision 1.22  1996/04/24  16:17:26  madden
 * Added new structure, BLAST_Link.
 *
 * Revision 1.21  1996/04/24  12:52:48  madden
 * ID's for sequences simplified.
 *
 * Revision 1.20  1996/04/03  19:14:35  madden
 * Removed defunct HSP ptr's.
 *
 * Revision 1.19  1996/03/29  21:27:43  madden
 * "hitlist" now kept on SeqAlign rather than HitList.
 *
 * Revision 1.17  1996/03/27  19:51:53  madden
 * "current_hitlist" added to Search Structure.
 *
 * Revision 1.16  1996/03/26  19:36:59  madden
 * Added  ReadDBFILEPtr to Search structure.
 *
 * Revision 1.15  1996/03/25  16:35:18  madden
 * Added old_stats.
 *
 * Revision 1.14  1996/02/28  21:37:43  madden
 * Added "trim" variables to segments for HSP.
 *
 * Revision 1.13  1996/02/06  22:51:13  madden
 * Added "prelim" to BlastSearch
 *
 * Revision 1.12  1996/02/02  19:25:32  madden
 * Added wfp_first and wfp_second to BlastParameterBlk for first and second pass.
 *
 * Revision 1.11  1996/01/29  21:12:07  madden
 * *** empty log message ***
 *
 * Revision 1.10  1996/01/23  16:31:47  madden
 * e_cutoff changed from BLAST_Score to double in ParameterBlk.
 *
 * Revision 1.9  1996/01/17  17:00:40  madden
 * Added gap parameters to ParameterBlk, dblen to SearchBlk.
 *
 * Revision 1.8  1996/01/17  13:45:58  madden
 * Added gap_prob and gap_decay_rate to ParameterBlk.
 *
 * Revision 1.7  1996/01/11  15:17:36  madden
 * Added process_num to ParameterBlk.
 *
 * Revision 1.6  1996/01/08  23:23:55  madden
 * removed "len" from HSP.
 *
 * Revision 1.5  1996/01/06  18:57:47  madden
 * Added BLAST_HSP_LINK structure.
 *
 * Revision 1.4  1995/12/28  21:26:05  madden
 * *** empty log message ***
 *
 * Revision 1.3  1995/12/26  23:04:14  madden
 * Added parameters to BlastParameterBlk.
 *
 * Revision 1.2  1995/12/21  23:10:41  madden
 * BLAST_Score prototypes moved to blastkar.h.
 *
 * Revision 1.1  1995/12/19  22:33:06  madden
 * Initial revision
 *
 * Revision 1.1  1995/12/08  15:48:23  madden
 * Initial revision
 *
 * */
#ifndef __BLASTSTR__
#define __BLASTSTR__

#include <ncbi.h>
#include <lookup.h>
#include <blastkar.h>
#include <objalign.h>
#include <sequtil.h>
#include <readdb.h>
#include <gapxdrop.h>
#include <mbalign.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Defines for program numbers. (Translated in BlastGetProgramNumber). */
#define blast_type_undefined 0
#define blast_type_blastn 1
#define blast_type_blastp 2
#define blast_type_blastx 3
#define blast_type_tblastn 4
#define blast_type_tblastx 5
#define blast_type_psitblastn 6


/* defines for strand_option, determines which strand of query to compare. */
#define BLAST_TOP_STRAND 1
#define BLAST_BOTTOM_STRAND 2
#define BLAST_BOTH_STRAND 3

/* Defines that specify whether or not BLAST should delete some memory, or
	leave it up to the caller.
*/
#define BLAST_OWN 0
#define BLAST_NOT_OWN 1

/* Specifies minimum search space size for an awak thread. */
#define AWAKE_THR_MIN_SIZE 2000000000000.0

#ifndef _BLASTCONCAT_
#include "blastconcat.h"
#endif
/* --KM concat */

/* Some default values (used when creating blast options block and for
 * command-line program defaults. When changing these defaults, please
 * remember to update the defaults in the command-line programs */
#define WINDOW_SIZE_PROT 40
#define WINDOW_SIZE_NUCL 0
#define WINDOW_SIZE_MEGABLAST 0

#define WORDSIZE_PROT 3
#define WORDSIZE_NUCL 11
#define WORDSIZE_MEGABLAST 28

/* Protein gap costs are the defaults for the BLOSUM62 scoring matrix.
 * More gap costs are listed in BLASTOptionSetGapParams */
#define GAP_OPEN_PROT 11
#define GAP_OPEN_NUCL 5
#define GAP_OPEN_MEGABLAST 0

#define GAP_EXTN_PROT 1
#define GAP_EXTN_NUCL 2
#define GAP_EXTN_MEGABLAST 0

#define WORD_THRESHOLD_BLASTP 11
#define WORD_THRESHOLD_BLASTN 0
#define WORD_THRESHOLD_BLASTX 12
#define WORD_THRESHOLD_TBLASTN 13
#define WORD_THRESHOLD_TBLASTX 13
#define WORD_THRESHOLD_MEGABLAST 0

#define UNGAPPED_X_DROPOFF_PROT 7
#define UNGAPPED_X_DROPOFF_NUCL 20
#define UNGAPPED_X_DROPOFF_MEGABLAST 10

#define GAP_X_DROPOFF_PROT 15
#define GAP_X_DROPOFF_NUCL 30 
#define GAP_X_DROPOFF_MEGABLAST 20 
#define GAP_X_DROPOFF_TBLASTX 0

#define GAP_X_DROPOFF_FINAL_PROT 25
#define GAP_X_DROPOFF_FINAL_NUCL 50 
#define GAP_X_DROPOFF_FINAL_TBLASTX 0

/* reward and penalty only apply to blastn/megablast */
#define PENALTY -3
#define REWARD 1

/********************************************************************
*
*	define for collecting BLAST stats.
*
***********************************************************************/

#define BLAST_COLLECT_STATS

/********************************************************************
*
*	Structure to save timing info. in.  Right now this only
*	works for UNIX.
*
********************************************************************/

typedef struct _blast_time_keeper {
                FloatLo	user, 	/* CPU time in user space of the process. */
			system, /* CPU time used by system. */
			total;	/* total CPU time (i.e., both of the above). */
        } BlastTimeKeeper, PNTR BlastTimeKeeperPtr;


/***************************************************************************
  Macros added by Andy Neuwald in order to allow easy modification of matrices.
***************************************************************************/

#define  MtrxScorePosSearch(S,x,y)	((S)->posMatrix[(x)][(y)])
#define  PtrMtrxScorePosSearch(S,x)	((S)->posMatrix[(x)])

/*****
#define  MtrxScorePosSearchi2(S,x,y)	\
	((S)->posMatrix[( (x) %(S)->query_length)][(y)])
#define  PtrMtrxScorePosSearch2(S,x)	\
	((S)->posMatrix[( (x) %(S)->query_length)])
*****/

/********************************************************************

	Defines for discontiguous word hits on 1st and 2nd pass.

********************************************************************/

#define BLAST_NO_PASS_DISCONTIG 0
#define BLAST_1ST_PASS_DISCONTIG 1
#define BLAST_2ND_PASS_DISCONTIG 2
#define BLAST_BOTH_PASS_DISCONTIG 3

#define CODON_LENGTH 3  /* three is always the codon length. */

#define BLAST_SMALL_GAPS 0
#define BLAST_LARGE_GAPS 1
#define MAX_INTRON_LENGTH 4000
#define MAX_DBSEQ_LEN 5000000

/*********************************************************************
    Filter types definitions
*********************************************************************/

#define FILTER_NONE 0
#define FILTER_DUST 1
#define FILTER_SEG  2

typedef enum {
   MB_WORD_CODING = 0,
   MB_WORD_OPTIMAL = 1,
   MB_TWO_TEMPLATES = 2
} MBDiscWordType;

/**********************************************************************
	Structure for the blast options (available to user/programmer).
	This should be filled in by the "Main" program before blast
	is called.

    If changes are made to this structure, corresponding changes should
    likely be made to BLAST_WizardOptionsBlk and BLAST_WizardOptionsMask.
***********************************************************************/

typedef struct _blast_optionsblk {
	Nlm_FloatHi gap_decay_rate,	/* decay rate. */
		    gap_prob;	/* Prob of decay. */
        Int4	    gap_size,	/* Small gap size. */
		    window_size,/* Multiple Hits window size (zero for single hit algorithm) */
		threshold_first, /* Threshold for extending hits (preliminary pass), zero if one-pass algorithm is used. */ 
		threshold_second;/* Threshold for extending hits (second pass) */
	Nlm_FloatHi	expect_value, 	/* Expectation value (E) */
			e2; 	  	/* Expect value for a single HSP */
	/* These two scores are zero, unless they've been set, then they set
	the expect_value and e2 above. */
	Int4		cutoff_s,	/* score corresponds to expect_value above.*/
			cutoff_s2;	/* score corresponds to e2 above. */
	Boolean two_pass_method; /* should two passes be used? */
	Boolean	multiple_hits_only; /* Only the multiple hits alg. used. */
	Int4	hitlist_size;	/* How many hits should be returned. */
	Nlm_FloatHi number_of_bits; /* Number of bits to initiate 2nd pass (default is used if zero) */
	Int4	dropoff_1st_pass, /* dropoff ("X") used for 1st pass. */
                dropoff_2nd_pass; /* dropoff ("X") used for 2nd pass. */
	Int2	number_of_cpus;	/* How many CPU's. */
	CharPtr matrix;		/* name of matrix to use. */
	Boolean old_stats; /* Use old stats (option may disappear later) */
	Boolean do_sum_stats;   /* Should sum statistics be used? */
	Boolean use_large_gaps; /* Use only large gaps for linking HSP's with sum stats. */
	Int2	wordsize;	/* size of word used to find hits. */
	Int2	penalty, reward; /* penalty and reward, only for blastn */
	/* The ID numbers from gc.prt are used for the genetic codes. */
	Int4	genetic_code,		/* genetic code for query (blastx, tblastx) */
		db_genetic_code;	/* genetic code for db (tblast[nx]). */
        Int4 filter;          /* filter type 0 mean no filter
                                 non-zero value indicate filer type */
	CharPtr filter_string;	/* String specifying the type of filtering and filter options. */
	Boolean		gapped_calculation; /* Is a gapped calc. being done? */
	/* The next three are used ONLY for gapped alignments. */
	Int4		gap_open,	/* Cost to open a gap (NO extension). */
			gap_extend,	/* Cost to extend a gap one letter. */
			gap_x_dropoff,	/* X-dropoff (in bits) used by Gapped align routine. */
			gap_x_dropoff_final;	/* X-dropoff (in bits) used by Gapped align routine for FINAL alignment. */
        Int4            decline_align;  /* Cost for declining alignment */
	Nlm_FloatHi	gap_trigger; /* Score (in bits) to gap, if an HSP gaps well. */

	Boolean		discontinuous;	/* Should the SeqAlign be discontinuous.*/
	/* What region of the query is required for the alignment.  If start is
	zero and end is -1 (the entire query), then these are not checked. */
	Int4		required_start,
			required_end;
	Int8		db_length;	/* database size used for stat. calcul. */
	Int4		dbseq_num;	/* number of database sequences used for stat. calcul. */
	Nlm_FloatHi	searchsp_eff;	/* Effective search space to be used. */
			
	/* Options for postion based blast. */
	Nlm_FloatHi	ethresh;
	Int4		maxNumPasses,
			pseudoCountConst;
	CharPtr program_name;		/* program name, for reference. */
	Int4 cpu_limit;	/* timeout total. */
	/* Used for region-dependent limits when storing hits. */
        Int4    hsp_range_max,          /* maximum hits for a range */
                block_width;            /* width of a block */
	Boolean perform_culling;	/* Should results be culled at all? */
        Boolean isPatternSearch;        /* Is this a use of PHI-BLAST?*/
	CharPtr		gifile;		/* name of file containing list of gis on server */
	ValNodePtr	gilist;		/* list of gis specified by client */
	Boolean		do_not_reevaluate;	/* Don't perform BlastReevaluateWithAmbiguities. */
	/* These options allow a subset of the database to be examined.  IF they
		are set to zero, then the entire database is examined. */
	Int4		first_db_seq,		/* 1st sequence in db to be compared. */
			final_db_seq;		/* Final sequence to be compared. */
	CharPtr		entrez_query;	/* user specified Entrez query to make selection from databases */
        CharPtr		org_name;	/* user specified name of organizm;  corresponding .gil file will be used */
	Uint1		strand_option;	/* BLAST_TOP_STRAND, BLAST_BOTTOM_STRAND, or BLAST_BOTH_STRAND.  used by blast[nx] and tblastx */
	Int4		hsp_num_max;	/* maximum number of HSP's allowed.  Zero indicates no limit. */
	Boolean		tweak_parameters, /* For impala related stuff. */
			smith_waterman;
        CharPtr         phi_pattern;      /* Pattern for PHI-Blast search */
	Boolean		use_real_db_size; /* Use real DB size.  meant for use if a list of gis' is submitted, 
					but statistics should be based upon the real database. */
        Boolean         use_best_align;   /* option is to use alignments choosen by user in PSM computation API (used in WWW PSI-Blast); */
        Int4            max_num_patterns; /* Maximum number of patterns to be used in PHI-Blast search */
        Boolean         is_megablast_search; /* Is this a MegaBlast search? */
        Uint1         no_traceback;    /* No traceback in MegaBLAST extension */
        Boolean         is_rps_blast;     /* If this RPS Blast ? */
        SeqLocPtr       query_lcase_mask; /* Masking of input DNA regions */
        Boolean         sort_gi_list;     /* Should the gi list be sorted? */
        Boolean         is_neighboring;   /* Is this a neighboring task? */
	Nlm_FloatHi original_expect_value;/* Used to change the E-value threshold for 
			part of a BLAST-KAPPA run and to restore it to the original threshold for the final output. */
	Boolean		explode_seqids;	/* make one SeqAlign for every gi on a
					   redundant sequence. */
        Boolean         megablast_full_deflines; /* Print full deflines in
						   megablast one-line output */
        Boolean         is_ooframe;  /* Use Out-Of-Frame gapping algorithm */
        Int4            shift_pen;   /* Out-Of-Frame shift penalty */
	Boolean		gilist_already_calculated; /* translation of gis to ordinalID's already done (used for neighboring). */
        Boolean  recoverCheckpoint;  /* For psitblastn */
        Boolean  freqCheckpoint;     /* For psitblastn */
        CharPtr  CheckpointFileName; /* For psitblastn */
        Int4     longest_intron;     /* the length of longest intron for linking HSPs */
        FloatLo  perc_identity;      /* Identity percentage cut-off */
        VoidPtr  output;             /* Output stream to put results to */
	FloatHi	scalingFactor;	     /* scaling factor used when constructing pssm for rpsblast. */ 
	Int4	total_hsp_limit;	/* total number of HSP's that will be processed to SeqAligns, zero means no limit. */
        Boolean mb_one_base_step; /* Scan every base of the database */
        Int2 mb_template_length;  /* Length of the discontiguous word */
        Boolean mb_use_dyn_prog;  /* Use dynamic programming gapped extension in
                                     megablast with affine gap scores */ 
        MBDiscWordType mb_disc_type;
	Uint4 NumQueries;		/*--KM for query concatenation in [t]blastn */
        Boolean ignore_gilist;    /* Used in traceback stage to not lookup gi's */
      } BLAST_OptionsBlk, PNTR BLAST_OptionsBlkPtr;


/*  --------------------------------------------------------------------
 *
 *  BLAST_WizardOptionsBlk contains those fields of BLAST_OptionsBlk
 *  that a user can set.
 *
 *  BLAST_WizardOptionsMask contains a Boolean for each field defined in
 *  BLAST_WizardOptionsBlk, except those holding pointers.  TRUE means
 *  that the corresponding field in BLAST_WizardOptionsBlk is set.
 *
 *  These structures are used only in conjunction with BLAST_Wizard.
 *
 *  --------------------------------------------------------------------
 */

struct _blast_wizardoptionsblk {
    Int4            block_width;
    Int4            cutoff_s;
    Int4            db_genetic_code;
    CharPtr         entrez_query;
    Nlm_FloatHi     ethresh;
    Nlm_FloatHi     expect_value;
    CharPtr         filter_string;
    Int4            first_db_seq;
    Int4            final_db_seq;
    Int4            gap_extend;
    Int4            gap_open;
    Boolean         gapped_calculation;
    Int4            genetic_code;
    ValNodePtr      gilist;
    Int4            hitlist_size;
    Int4            hsp_range_max;
    Boolean         is_ooframe;
    CharPtr         matrix;
    MBDiscWordType  mb_disc_type;
    Int2            mb_template_length;
    Uint1           no_traceback;
    Int2            penalty;
    FloatLo         perc_identity;
    Boolean         perform_culling;
    CharPtr         phi_pattern;
    Int4            pseudoCountConst;
    SeqLocPtr       query_lcase_mask;
    Int4            required_end;
    Int4            required_start;
    Int2            reward;
    Nlm_FloatHi     searchsp_eff;
    Boolean         smith_waterman;
    Uint1           strand_option;
    Int4            threshold_first;
    Int4            threshold_second;
    Boolean         tweak_parameters;
    Boolean         use_best_align;
    Boolean         use_real_db_size;
    Int4            window_size;
    Int2            wordsize;

    Boolean         two_hits;
    CharPtr         string_options;
};

typedef struct _blast_wizardoptionsblk
            BLAST_WizardOptionsBlk,
    PNTR    BLAST_WizardOptionsBlkPtr;

struct _blast_wizardoptionsmask {
    Boolean         block_width;
    Boolean         cutoff_s;
    Boolean         db_genetic_code;
    Boolean         ethresh;
    Boolean         expect_value;
    Boolean         first_db_seq;
    Boolean         final_db_seq;
    Boolean         gap_extend;
    Boolean         gap_open;
    Boolean         gapped_calculation;
    Boolean         genetic_code;
    Boolean         hitlist_size;
    Boolean         hsp_range_max;
    Boolean         is_ooframe;
    Boolean         mb_disc_type;
    Boolean         mb_template_length;
    Boolean         no_traceback;
    Boolean         penalty;
    Boolean         perc_identity;
    Boolean         perform_culling;
    Boolean         pseudoCountConst;
    Boolean         required_end;
    Boolean         required_start;
    Boolean         reward;
    Boolean         searchsp_eff;
    Boolean         smith_waterman;
    Boolean         strand_option;
    Boolean         threshold_first;
    Boolean         threshold_second;
    Boolean         tweak_parameters;
    Boolean         use_best_align;
    Boolean         use_real_db_size;
    Boolean         window_size;
    Boolean         wordsize;

    Boolean         two_hits;
};

typedef struct _blast_wizardoptionsmask
            BLAST_WizardOptionsMask,
    PNTR    BLAST_WizardOptionsMaskPtr;

typedef enum {
   TEMPL_11_16 = 0,
   TEMPL_12_16 = 1,
   TEMPL_11_18 = 2,
   TEMPL_12_18 = 3,
   TEMPL_11_21 = 4,
   TEMPL_12_21 = 5,
   TEMPL_11_16_OPT = 6,
   TEMPL_12_16_OPT = 7,
   TEMPL_11_18_OPT = 8,
   TEMPL_12_18_OPT = 9,
   TEMPL_11_21_OPT = 10,
   TEMPL_12_21_OPT = 11,
   TEMPL_ERROR = -1
} MBTemplateType;

typedef struct _mb_parameter_blk_ {
   Uint1 no_traceback;    /* No traceback in greedy extension */
   Boolean is_neighboring;  /* Is this a neighboring task? */ 
   Boolean full_seqids;     /* Print full seqids in tabular output? */
   FloatLo perc_identity;   /* Identity percentage cut-off */
   Int4    max_positions;   /* Maximal number of positions in query of a given word */
   Boolean disc_word;       /* Use a discontiguous word template to find initial 
                               matches */
   Boolean one_base_step;   /* Form words for every position in the database
                               sequence (default is every 4th position) */
   Int2    word_weight;     /* Number of identical nucleotides in a word match */
   Int2    template_length; /* Length of a discontiguous word template */
   Boolean use_dyn_prog;    /* Use dynamic programming extension for affine gap
                               scores */
   MBTemplateType template_type; /* Type of a discontiguous template */
   Boolean use_two_templates;
} MegaBlastParameterBlk, PNTR MegaBlastParameterBlkPtr;

/****************************************************************************

	PARAMETER BLOCK: parameters for the BLAST search entered by on
	command line by user.

*****************************************************************************/

typedef struct _blast_parameterblk {
        BLAST_Score     threshold,      /* threshold for extending a word hit*/
        		threshold_first, /* threshold for 1st pass. */
        		threshold_second, /* threshold for 2nd pass. */
                        X,              /* drop-off score for extension. */
			dropoff_1st_pass, /* dropoff ("X") used for 1st pass. */
			dropoff_2nd_pass, /* dropoff ("X") used for 2nd pass. */
                        cutoff_s,	/* Final Score to report a hit. */
                        cutoff_s1,	/* Score to save an HSP after a gapped extension. */
                        cutoff_s2,	/* Score to save an HSP after an ungapped extension. */
			cutoff_s_first, /* Score (S2) to use on 1st pass */
			cutoff_s_second, /* Score (S2) to use on 2nd pass and
			   for "small" gaps in link_hsps (in blast.c) */
	/* Max value of s2, used if s2 is set or s2 becomes larger than s. */
			cutoff_s2_max,	
			cutoff_big_gap; /* cutoff value for a "big" gap in
			   link_hsps (in blast.c). */
	Nlm_FloatHi	cutoff_e,	/* Expect value to report a hit. */
                        cutoff_e2,	/* Expect value to report a hsp. */
			number_of_bits; /* number of bits of significance, used
			   to calculate cutoff_s_first (above). */
	Boolean		threshold_set, /*TRUE if threshold set on command-line*/
			cutoff_s_set,	/* TRUE if cutoff score set on c-l */
			cutoff_s2_set,	/* TRUE if cutoff score2 set on c-l */
			cutoff_e_set,	/* TRUE if cutoff expect set on c-l */
			cutoff_e2_set,	/* TRUE if cutoff expect2 set on c-l */
			ignore_small_gaps, /* ignore small gaps if TRUE, set by
			   CalculateSecondCutoffScore in blast.c if the search 
			   space is smalled than 8*gap_size*gap_size. */
			window_size_set;/* TRUE if window size set for MHBLAST*/
        Boolean         sump_option;    /* TRUE if sump is used. */
	Int4		gap_size,	/* max. gap allowed for small gaps.*/
			window_size;	/* used for multiple hits BLAST. */
	Nlm_FloatHi	gap_prob; 	/* prob. of gap of size "gap" (above).*/
	Nlm_FloatHi	gap_decay_rate; /* prob. of only one HSP */
	Int2		process_num;	/* max # processrs permitted (for MP).*/
	Boolean		old_stats;	/* Use "old" stats if TRUE. */
	Boolean 	do_sum_stats;   /* Should sum statistics be used? */
	Boolean         use_large_gaps; /* Use only large gaps for linking HSP's with sum stats. */
	Boolean		two_pass_method; /* should two passes be used? */
	Boolean		multiple_hits_only; /* Only the multiple hits alg. used. */
	Boolean		discontinuous;	/* Should discontinuous SeqAlign's be produced? */
	Boolean		gapped_calculation; /* Is a gapped calc. being done? */
	Boolean		do_not_reevaluate;	/* Don't perform BlastReevaluateWithAmbiguities. */
	/* The next three are used ONLY for gapped alignments. */
	Int4		gap_open,	/* Cost to open a gap (NO extension). */
			gap_extend,	/* Cost to extend a gap one letter. */
			gap_x_dropoff,	/* X-dropoff used by Gapped align routine. */
			gap_x_dropoff_final;	/* X-dropoff (in bits) used by Gapped align routine for FINAL alignment. */
        Int4            decline_align;  /* Cost for declining alignment */

	Nlm_FloatHi	gap_trigger; /* Score (in bits) to gap, if an HSP gaps well.*/

	/* Options for postion based blast. */
	Nlm_FloatHi	ethresh;
	Int4		maxNumPasses,
			pseudoCountConst;
	Int4 cpu_limit;	/* timeout total. */
        Int4    hsp_range_max,          /* maximum hits for a range */
		max_pieces;		/* Max number of pieces allowed (query_length/block_width) */
	Boolean perform_culling;	/* determines whether culling should be used or not.
					If not, then hsp_range_max, block_width, and max_pieces are ignored. */
	/* These options allow a subset of the database to be examined.  IF they
		are set to zero, then the entire database is examined. */
	Int4		first_db_seq,		/* 1st sequence in db to be compared. */
			final_db_seq;		/* Final sequence to be compared. */
	Int4		hsp_num_max;	/* maximum number of HSP's allowed.  Zero indicates no limit. */
        Boolean   use_best_align;   /* option is to use alignments choosen by user in PSM computation API (used in WWW PSI-Blast); */
        MegaBlastParameterBlkPtr mb_params;  /* Is this a MegaBlast search? */
        CharPtr filter_string;  /* String specifying the type of filtering and filter options. - used with Translated RPS Blast */
        Boolean is_rps_blast;      /* If this RPS Blast ? */
        SeqLocPtr  query_lcase_mask; /* Masking of input DNA regions */
	Boolean		explode_seqids;	/* make one SeqAlign for every gi on a
					   redundant sequence. */
        Boolean         is_ooframe;  /* Use Out-Of-Frame gapping algorithm */
        Int4            shift_pen;  /* Out-Of-Frame shift penalty */
        Int4    longest_intron;     /* the length of longest intron for linking HSPs */
	FloatHi	scalingFactor;	     /* scaling factor used when constructing pssm for rpsblast. */ 
	Int4	total_hsp_limit;	/* total number of HSP's that will be processed to SeqAligns, zero means no limit. */
        } BLAST_ParameterBlk, PNTR BLAST_ParameterBlkPtr;

typedef Nlm_Int4	BLAST_Diag, PNTR BLAST_DiagPtr;

/* Structure to keep track of the last hit and diag level. */

typedef struct cfj_mod_struct{
    Int4 last_hit;
    Int4 diag_level;
} CfjModStruct;
/*
	BLAST_ExtendWord contains information about which diagonals
	have been extended over (i.e., which diagonals have been 
	tested).  This structure will be duplicated once for each
	context as every context is different.
*/
typedef struct _blast_extend_word {
		Int4Ptr	_buffer; /* The "real" buffer for diag_level, version,
				and last_hit arrays. */
                CfjModStruct *combo_array;
                Int4Ptr version; /* still needed?? */
		Int4	actual_window; /* The actual window used if the multiple
				hits method was used and a hit was found. */	
	} BLAST_ExtendWord, PNTR BLAST_ExtendWordPtr;

/*
	BLAST_ExtendWordParams contains parameters about the extensions.
	Only one copy of this structure is needed, regardless of how many
	contexts there are.
*/
typedef struct _blast_extend_word_params {
		Int4	bits_to_shift; /* how many bits should the diagonal be
				shifted to get the "version" */
		Int4	min_diag_length, /* Min. length of diagonal, actuall
				2**bits_to_shift. */
			min_diag_mask; /* Used to mask off everything above
				min_diag_length (mask = min_diag_length-1). */
		Int4	offset; /* "offset" added to query and subject position
				so that "diag_level" and "last_hit" don't have
				to be zeroed out every time. */
		Int4	window;	/* The "window" size, within which two (or more)
				hits must be found in order to be extended. */
		/* Used by BLAST_ExtendWordNew to decide whether or not
		to prepare the structure for multiple-hit type searches.
		If TRUE, multiple hits are not neccessary, but possible. */
		Boolean multiple_hits;  
	} BLAST_ExtendWordParams, PNTR BLAST_ExtendWordParamsPtr;
/*
	Data block to describe a single sequence.
*/

typedef struct blast_sequence_block {
	Uint1Ptr	sequence,	/* Actual (perhaps transl.) sequence. */
		sequence_start; /* Start of sequence, used if the sequence is preceded by a NULLB.  Sequences
				starting with a NULLB are used by BlastWordExtend_L1. */
	Int4	length,		/* length of sequence. */
		original_length,/* length before translation. */
		effective_length;/* effective length, used only by query. */
	Int2 frame;		/* frame of the sequence. */
} BlastSequenceBlk, PNTR BlastSequenceBlkPtr;


typedef struct _blast_seg {
		Int2		frame;
		Int4		offset;	/* start of hsp */
		Int4		length;	/* length of hsp */
		Int4		end;	/* end of HSP */
		Int4		offset_trim;	/* start of trimmed hsp */
		Int4		end_trim;	/* end of trimmed HSP */
		/* Where the gapped extension (with X-dropoff) started. */
		Int4		gapped_start;
	} BLAST_Seg, PNTR BLAST_SegPtr;

#define BLAST_NUMBER_OF_ORDERING_METHODS 2

/*
	The following structure is used in "link_hsps" to decide between
	two different "gapping" models.  Here link is used to hook up
	a chain of HSP's (this is a VoidPtr as _blast_hsp is not yet
	defined), num is the number of links, and sum is the sum score.
	Once the best gapping model has been found, this information is
	transferred up to the BLAST_HSP.  This structure should not be
	used outside of the function link_hsps.
*/
typedef struct _blast_hsp_link {
		/* Used to order the HSP's (i.e., hook-up w/o overlapping). */ 
	VoidPtr	link[BLAST_NUMBER_OF_ORDERING_METHODS]; 
		/* number of HSP in the ordering. */
	Int2	num[BLAST_NUMBER_OF_ORDERING_METHODS];
		/* Sum-Score of HSP. */
	Int4	sum[BLAST_NUMBER_OF_ORDERING_METHODS]; 
		/* Sum-Score of HSP, multiplied by the appropriate Lambda. */
	Nlm_FloatHi	xsum[BLAST_NUMBER_OF_ORDERING_METHODS]; 
	Int4 changed;
	} BLAST_HSP_LINK, PNTR BLAST_HSP_LINKPtr;
/*
	BLAST_NUMBER_OF_ORDERING_METHODS tells how many methods are used
	to "order" the HSP's.
*/

typedef struct _blast_hsp {
		struct _blast_hsp PNTR next, /* the next HSP */
				  PNTR prev; /* the previous one. */
		BLAST_HSP_LINK	hsp_link;
/* Is this HSp part of a linked set? */
		Boolean		linked_set;
/* which method (max or no max for gaps) was used? */
		Int2		ordering_method; 
/* how many HSP's make up this (sum) segment */
		Int4		num;
/* normalized score of a set of "linked" HSP's */
        Nlm_FloatHi xsum;
		/* If TRUE this HSP starts a chain along the "link" pointer. */
		Boolean 	start_of_chain;
		BLAST_Score	score;
		Int4    	num_ident;
		Nlm_FloatHi	evalue;
		BLAST_Seg query,	/* query sequence info. */
			subject;	/* subject sequence info. */
		Int2		context;	/* Context number of query */
                GapXEditBlockPtr gap_info; /* ALL gapped alignment is here */
		Int4 num_ref;
		Int4 linked_to;
	} BLAST_HSP, PNTR BLAST_HSPPtr;

/* The helper arrays contains the info used frequently in the inner for loops. -cfj
 * One array of helpers will be allocated for each thread. See comments preceding
 * link_hsps in blast.c for more info.
 */

typedef struct link_help_struct{
  BLAST_HSPPtr ptr;
  Int4 q_off_trim;
  Int4 s_off_trim;
  Int4 sum[BLAST_NUMBER_OF_ORDERING_METHODS];
  Int4 maxsum1;
  Int4 next_larger;
} LinkHelpStruct;

/* Orders information for HSP accesses. */
typedef struct hsp_helper{
	Int4 	qoffset,
		qend;
} BLAST_HSP_helper, PNTR BLAST_HSP_helperPtr;
		

typedef struct _exact_match {
   Int4 q_off;
   Int4 s_off;
} MegaBlastExactMatch, PNTR MegaBlastExactMatchPtr;

typedef struct _blast_hitlist {
	struct _blast_hitlist	PNTR next;
	BLAST_HSPPtr PNTR	hsp_array; /* head of linked list of HSPs */
	Int4		hspmax, /* max no. of HSPs allowed per hit list */
			hspcnt, /* no. of HSPs in hit list */
			hspcnt_max; /* no. of HSPs in hitlist, before reaping */
	Boolean		further_process; /* This sequence has been found interesting,
					    it should be further processed by a gapped
					    alignment etc. */
	Boolean		do_not_reallocate; /* Don't reallocate the HSP's, probably because
                       		   there is no more memory for this. */
        	/* added -cfj */
        LinkHelpStruct *lh_helper;
        Int4 lh_helper_size;
        MegaBlastExactMatchPtr exact_match_array; /* Array to hold initial
                                                          exact match hits */
        Int4 exact_match_max;
	} BLAST_HitList, PNTR BLAST_HitListPtr;

/*
	The next two structures are the final output produced by BLAST.  Formatters should then
	convert the data into SeqAligns or the BLAST ASN.1 spec.  
*/

typedef struct _blast_results_hsp {
		Int2		ordering_method;/* determines whether large or small gap was used. */
		Int4 		number;	/* number of HSP's used to calculate the p-value. */
		BLAST_Score	score;	/* score of this HSP. */
		Nlm_FloatHi	e_value,/* expect value of this set of HSP's. */
				bit_score; /* above score * lambda/ln2 */
		Int4		num_ident;/* number of identities in this HSP. */
		Int2		context;	/* context number of query. */
		Int2		query_frame, /* frame of query, non-zero if transl. */
				subject_frame; /* frame of subject, non-zero if transl. */
		Int4 		query_offset,	/* Start of the query HSP. */
				query_length,	/* Length of the query HSP. */
				subject_offset,	/* Start of the subject HSP. */
				subject_length, /* Length of the subject HSP.*/
				hspset_cnt;	/* which set of HSP's? */
	/* Starting points (on original HSP) for a gapped extension with X dropoff. */
		Int4		query_gapped_start,
				subject_gapped_start;

                GapXEditBlockPtr gap_info; /* ALL gapped alignment is here */
		struct _blast_result_hitlist PNTR point_back;
		struct _blast_heap_struct PNTR back_left, PNTR back_right;
		} BLASTResultHsp, PNTR BLASTResultHspPtr;

/*
	The following structure contains the subject info, if the readdb
	facility is not being used.  Then the subject information is
	kept here.  Otherwise this structure is NULL.
*/
typedef struct _blast_subject_info {
		SeqIdPtr sip;	/* ID of the subject. */
		CharPtr defline; /* Defline of the subject. */
		Int4 length; 	/* untranslated length of the database sequence. */
		} BLASTSubjectInfo, PNTR BLASTSubjectInfoPtr;

typedef struct _blast_result_hitlist {
		BLASTResultHspPtr hsp_array;	/* An array holding the HSP's. */
		Nlm_FloatHi	best_evalue;	/* best evalue in all the HSP's. */
		Int4	high_score; 	/* HSP with highest score. */
		Int4	hspcnt,		/* Number of HSP's. */
			subject_id;	/* ID of the subject. */
		Int2    db_id;          /* ID (0,1,2...) of the db if multiple db's searched. */
		Int4    subject_length; /* length of the database sequence. */
		BLASTSubjectInfoPtr subject_info; /* Subject info if the readdb facility is not being used. */
		SeqAlignPtr seqalign; /* alignment, if this a gapped calculation. */
		Int4 num_ref;
		} BLASTResultHitlist, PNTR BLASTResultHitlistPtr;


typedef struct _blast_heap_struct {
  Int4 cutvalue;	/* start of a region? */
  BLASTResultHspPtr PNTR heap;
  Int4 num_in_heap;	/* Number in 'heap' */
  Int4 num_of_ref;
  struct _blast_heap_struct PNTR next, PNTR prev;
} BLASTHeapStruct, PNTR BLASTHeapPtr;

/*
	Holds the results already saved.
*/

typedef struct _blast_results_struct {

		BLASTResultHitlistPtr PNTR results;
		Int4	hitlist_count,	/* Number of hitlists saved on results array already. */
			hitlist_max, 	/* Length of results array. */
			max_pieces;	/* For range-dependent limits. */
		BLASTResultHspPtr **heap;
        	Int4 *num_in_heap;
		BLASTHeapPtr heap_ptr;
		} BLASTResultsStruct, PNTR BLASTResultsStructPtr;

/*
	Holds the data for all possible words that might be used by BLAST.
*/

typedef struct _blast_all_words {
		Uint1Ptr *array,	/* All the possible words */
			 array_storage; /* Storage for the words in array. */
		Int4 	num_of_cols, 
			wordsize;
		Boolean rows_allocated,	/* are the rows (of length the wordsize) alloc.*/
			specific;	/* specific (limited) words are to be indexed. */
	} BlastAllWord, *BlastAllWordPtr;
		
/*
	Contains gi and ordinal number for use by random access BLAST.
*/
typedef struct _double_int4 {
        Int4    gi,
                ordinal_id,
		start;
} BlastDoubleInt4, *BlastDoubleInt4Ptr;


typedef struct _blast_gi_list {
    BlastDoubleInt4Ptr gi_list;	/* List of gi's. */
    BlastDoubleInt4Ptr *gi_list_pointer;	/* Pointer to above list. */
    Int4 current;	       /* Current position in gi list. */
    Int4 total;		       /* total number of gi's. */
    Boolean gilist_not_owned; /* do not delete gilist at end. */
} BlastGiList, *BlastGiListPtr;
        
/*
	used for keeping start and stop of hits to query, for ALU filtering.
*/
typedef struct _blast_hit_range {
	BlastDoubleInt4Ptr      range_list;        /* ranges. */
        BlastDoubleInt4Ptr      *range_list_pointer;       /* Pointer to above list. */
	Int4		current,	/* current position in list. */
			total;		/* total number in list. */
	SeqIdPtr	query_id;	/* ID to be put on SeqLoc's that are produced. */
	Int4		base_offset;	/* used if a SeqLoc is searched and it does not start at begining
					of sequence. */
	} BlastHitRange, *BlastHitRangePtr;

/*
	Contains BLAST error messages.
*/

typedef struct _blast_error_msg {
    Uint2 level;/* corresponds to levels of ErrPostEx [none(0), info(1), warn(2), error(3) and fatal(4)] */
    CharPtr msg;
} BlastErrorMsg, *BlastErrorMsgPtr;

/*
  Holds data for each "context" (which is generally equal to
  one frame of the query).  blastx would have six contexts,
  blastp would have one.
  */

typedef struct _blast_context_structure {
    Boolean query_allocated;/* The BlastSequenceBlkPtr IS allocated. */
    BlastSequenceBlkPtr query;  /* query sequence. */
    BLAST_ExtendWordPtr ewp;/* keep track of diagonal etc. for each frame */
    ValNodePtr location;    /* Where to start/stop masking. */
} BLASTContextStruct, PNTR BLASTContextStructPtr;
    
    /* Structure used for full Smith-Waterman results. */
    
typedef struct SWResults {
    Uint1Ptr seq;
    Int4 seqStart;
    Int4 seqEnd;
    Int4 queryStart;
    Int4 queryEnd;
    Int4 *reverseAlignScript;
    BLAST_Score score;
    BLAST_Score scoreThisAlign;
    Nlm_FloatHi eValue;
    Nlm_FloatHi eValueThisAlign;
    Nlm_FloatHi Lambda;
    Nlm_FloatHi logK;
    SeqIdPtr subject_id;  /*used to display the sequence in alignment*/
    struct SWResults *next;
    Boolean isFirstAlignment;
    Int4 subject_index;  /* needed to break ties on rare occasions */
    SeqAlignPtr seqAlign; /*needed when seqAlign is already computed*/
} SWResults;
    
/* Average sizes of protein and nucl. sequences. */
#define BLAST_AA_AVGLEN 300
#define BLAST_NT_AVGLEN 1000

/* How many ticks should be emitted total. */
#define BLAST_NTICKS 50

/* period of sending out a star/message. */
#define STAR_MSG_PERIOD 60

typedef struct _BlastThrInfo {

    TNlmMutex db_mutex;  /*lock for access to database*/
    TNlmMutex results_mutex; /*lock for storing results */
    TNlmMutex callback_mutex; /*lock for issuing update ticks on the screen*/
    /* Mutex for recalculation of ambiguities, in BlastReevaluateWithAmbiguities */
    TNlmMutex ambiguities_mutex;

    /*
      GI List to be used if database will be searched by GI.
      current is the current element in the array being worked on.
      global_gi_being_used specifies that it will be used.
      */
    Int4 gi_current;
    BlastGiListPtr blast_gi_list;
    
    /* Number of database sequences for each thread to process. */
    Int4 db_chunk_size;

    /* The last db sequence to be assigned.  Used only in get_db_chunk after
       the acquisition of the "db_mutex" (above). */
    Int4 db_chunk_last;

    /* the last sequence in the database to be compared against. */
    Int4 final_db_seq;
    Int4 number_seqs_done;  /*number of sequences already tested*/
    Int4 db_incr;  /*size of a database chunk to get*/
    Int4 last_db_seq;

    /* How many positive hits were found (set by ReapHitlist, read by tick_proc
       and star_proc). */
    Int4 number_of_pos_hits;

    /* Use by star_proc to determine whether to emit a star. */
    time_t last_tick;
    
    /* tells star_proc to check that a star should be emitted. */
    TNlmThread awake_thr;
    Boolean awake;
    
    /* tells index_proc to check that a message should be emitted. */
    TNlmThread index_thr;
    Boolean awake_index;
    
    /*
      Callback functions to indicate progress, or lack thereof.
      */
    int (LIBCALLBACK *tick_callback)PROTO((Int4 done, Int4 positives));
    int (LIBCALLBACK *star_callback)PROTO((Int4 done, Int4 positives));
    int (LIBCALLBACK *index_callback)PROTO((Int4 done, Int4 positives));

    /* whether real databases are done */
    Boolean	realdb_done;

} BlastThrInfo, PNTR BlastThrInfoPtr;
    
/*
	Structure used for matrix rescaling. 
*/

typedef struct _blast_matrix_rescale {
	Int4 		alphabet_size,
			query_length;	/* length of query. */
	Uint1Ptr	query;
	Nlm_FloatHi 	*standardProb;
	Int4Ptr  	*matrix;
	Int4Ptr  	*private_matrix;
	BLAST_KarlinBlkPtr 	*kbp_std, 
				*kbp_psi, 
				*kbp_gap_std, 
				*kbp_gap_psi;
	Nlm_FloatHi	lambda_ideal,
                	K_ideal;
} BlastMatrixRescale, *BlastMatrixRescalePtr;
	
		
/*
	The central structure for the BLAST search.  This structure
	should contain data (or pointers to data) for all the
	information in a BLAST search.
*/


#define BLAST_SEARCH_ALLOC_QUERY 1
#define BLAST_SEARCH_ALLOC_SUBJECT 2
#define BLAST_SEARCH_ALLOC_PBP 4
#define BLAST_SEARCH_ALLOC_SBP 8
#define BLAST_SEARCH_ALLOC_WFP_FIRST 16
#define BLAST_SEARCH_ALLOC_WFP_SECOND 32
#define BLAST_SEARCH_ALLOC_EWPPARAMS 64
#define BLAST_SEARCH_ALLOC_CONTEXT 128
#define BLAST_SEARCH_ALLOC_RESULTS 256
#define BLAST_SEARCH_ALLOC_READDB 512
#define BLAST_SEARCH_ALLOC_TRANS_INFO 1024
#define BLAST_SEARCH_ALLOC_ALL_WORDS 2048
#define BLAST_SEARCH_ALLOC_QUERY_SLP 4096
#define BLAST_SEARCH_ALLOC_THRINFO 8192
#define BLAST_SEARCH_ALLOC_MASK1 16384

typedef struct blast_search_block {
    Int4 allocated; 
/* bit fields specify which structures from below are allocated.  If 
a field is allocated, then it's bit is non-zero. 

		structure     		bit-field (define)
		-----------------------------------------
		query			BLAST_SEARCH_ALLOC_QUERY
		subject			BLAST_SEARCH_ALLOC_SUBJECT
		pbp			BLAST_SEARCH_ALLOC_PBP
		sbp			BLAST_SEARCH_ALLOC_SBP
		wfp_first       	BLAST_SEARCH_ALLOC_WFP_FIRST
		wfp_second      	BLAST_SEARCH_ALLOC_WFP_SECOND
		ewp_params		BLAST_SEARCH_ALLOC_EWPPARAMS
		context			BLAST_SEARCH_ALLOC_CONTEXT
		result_struct		BLAST_SEARCH_ALLOC_RESULTS
		rdfp	        	BLAST_SEARCH_ALLOC_READDB
		translation_table       BLAST_SEARCH_ALLOC_TRANS_INFO
		translation_table_rc
		all_words		BLAST_SEARCH_ALLOC_ALL_WORDS
		query_slp		BLAST_SEARCH_ALLOC_QUERY_SLP
		mask1			BLAST_SEARCH_ALLOC_MASK1
*/

/*
  Specifies whether the search is position based or not.
  */
    Boolean positionBased;
    Boolean posConverged;
    /*
      Specifies that the query sequence was invalid (e.g., XXXXXXXXXXXXXXXXXXXXXX).
      */
    Boolean query_invalid;
    /* Specifies that the search timed out (i.e., cpu time limit was reached). */
    Boolean timed_out;
    /*
      The BLASTContextStructPtr is an array and each element contains
      information about the query sequence and the frame number.
      If there are six frames (e.g., blastx) then the BLASTContextStructPtr
      is six elements long; if there's one frame (e.g., blastp) then
      BLASTContextStructPtr is one element long.
      
      number_of_contexts states how long the context array is.
      */	
    BLASTContextStructPtr context;
    Int2	first_context,
        last_context;
    /* 
       The GapAlignBlkPtr used by ALIGN (in gapxdrop.c) for gapped alignments.
       */
    
    GapAlignBlkPtr gap_align;
    
    /*
      All the possible words.
      */
    BlastAllWordPtr all_words;
    /*
        Set the context_factor, which specifies how many different 
        ways the query or db is examined (e.g., blastn looks at both
        stands of query, context_factor is 2).
        */
    Int2 context_factor;
    
    /*
      What type of search (e.g., blastp, blastx, etc.)?
      */
    CharPtr prog_name;
    Uint1 prog_number;
    /*
      translation_table and translation_table_rc holds the translation
      from ncbi2na to ncbistdaa for normal and reverse-complement
      translations.  Only used and initialized with tblast[nx].
      Initialized by GetPrivatTranslationTable
      */
    Uint1Ptr translation_table,
        translation_table_rc;
    
    /*
      ValNodePtr containing error messages. 
      */
    ValNodePtr error_return;
    
    /*
      ValNodePtr containing masking SeqLocPtr's
      */
    ValNodePtr mask;
    ValNodePtr mask1;
    /*
      What genetic codes are we using to translate the query or database
      when needed.  Based upon NCBI genetic codes.
      */
    CharPtr genetic_code,		/* genetic code used for query. */
        db_genetic_code;	/* genetic code used for database. */
    
    /* 	
	The BlastSequenceBlk's subject hold info about the subject.  
	Info about the original sequence is in original_seq.  This will
	be NULL if the sequence was not translated. 
        */
    Uint1Ptr translation_buffer;	/* Buffer for (tblast[nx]) db translations*/
    Int4 translation_buffer_size;	/* size of translation_buffer. */
    CharPtr original_seq;	/* Original (i.e.,  untransl.) sequence. */
    BlastSequenceBlkPtr	subject;/* subject sequence. */
    

    /* KM-- info about individual queries from a concatenated query in
       blastn or tblastn */
    struct queries PNTR mult_queries;	/* struct defined in blastconcat.h */ 


    /*
      SeqLocPtr for the query, owned by the called and not by BLAST.
      */
    SeqLocPtr query_slp;
    
    /* Id's for the query and subject. */
    SeqIdPtr		query_id;	/* ID for the query, any form. */
    Int4			subject_id;	/* the number of the subject, in the DB. */
    BLAST_ParameterBlkPtr pbp;	/* options selected. */
    BLAST_ScoreBlkPtr sbp;		/* info on scoring. */
    BLAST_ExtendWordParamsPtr ewp_params; /* parameters for extensions.*/
    
    /* 	For the two-pass method two BLAST_WordFinderPtr's are required.
	The actual wfp's are in wfp_first and wfp_second.  "wfp" is just
	a pointer to one of those two.  If they have been allocated (at all)
	is signified by setting the bit-fields above. 
        */
    BLAST_WordFinderPtr     wfp, 	/* find initial words. */
        wfp_first, /* words for first pass. */
        wfp_second;/* words for second pass. */
    /*	For the two-pass this should be set to TRUE on the first (preliminary)
	pass and FALSE on the second pass.
        */
    Boolean			prelim;
/*
  The "current" hit, that is the one being worked on right now.  
  If a hitlist is deemed significant, then "current_hitlist" is 
  moved to "seqalign".  current_hitlist_purge specifies 
  whether the hitlist should be purged after each call to a
  WordFinder; it will generally be purged except for non-initial
  frames of tblast[nx].
  */
    Boolean			current_hitlist_purge;
    BLAST_HitListPtr	current_hitlist;

    BlastSequenceBlkPtr	PNTR query_dnap; /* query DNAP sequence. */

    /*
      The worst evalue seen by this thread so far.
      Only filled in if the hitlist is already full, otherwise
      it should be DBL_MAX.
      */
    Nlm_FloatHi	worst_evalue;
    /*
      Size of the HSP array on the "current_hitlist"
      */
    Int4 hsp_array_size;
    /*
      Contains hits that are significant. 
      */
    Int4			result_size;
    BLASTResultsStructPtr	result_struct;
    
    Int8			dblen;	/* total length of the database. */
    Int8                    dblen_eff;      /* effective length of the database. */
    Int8                    dblen_eff_real;      /* effective length of the database. */
    Int4                    dbseq_num;      /* number of sequences in the database. */
    Int4                    length_adjustment; /* amount removed from end of query and db sequences. */
    Nlm_FloatHi		searchsp_eff;	/* Effective search space (used for statistics). */
    Int4            rps_qlen; /* original query sequence length (RPS-BLAST only) */
    ReadDBFILEPtr		rdfp, /* I/O PTR for database files. */
        rdfp_list;	/* linked rdfp list of all databases. */
    /* The subject info (id and defline) is kept here for the current sequence
       if the readdb facility is not used.  This structure should only
       be used if rdfp is NULL.
       */
    BLASTSubjectInfoPtr subject_info;
    
    /* Data used in threads - previously global variables */

    BlastThrInfoPtr thr_info;
    
    /*
      start and stop of query that must be included for an alignment
      to be counted.  The Boolean whole_query specifies whether these
      are valid (i.e., have been set) or not.
      */
    Boolean whole_query;
    Int4 required_start, required_end;
    
    /*
      Callback functions to indicate progress, or lack thereof.
*/
    /* int (LIBCALLBACK *tick_callback)PROTO((Int4 done, Int4 positives)); */
    /* int (LIBCALLBACK *star_callback)PROTO((Int4 done, Int4 positives)); */
    /*
      Callback function to handle results (e.g., print them out for neighboring)
      in place of BlastSaveCurrentHitlist.
      */
    int (LIBCALLBACK *handle_results)PROTO((VoidPtr search));
    /*	
	Output stream to put results to
        */
    VoidPtr		output;
    /*
      These "counters" keep track of how often certain operations
      were performed.
      
      This counting is performed only if BLAST_COLLECT_STATS is defined.
      */
    Int8	first_pass_hits,	/* no. of hits on 1st pass. */
        second_pass_hits,	/* no. of hits on 2nd pass. */
        second_pass_trys,	/* no. of seqs that made it to 2nd pass. */
        first_pass_extends,	/* no. extended on 1st pass. */
        second_pass_extends,	/* no. extended on 2nd pass. */
        first_pass_good_extends,/* no. successfully extended on 1st pass. */
        second_pass_good_extends,/* no. successfully extended on 2nd pass. */
        number_of_seqs_better_E,/* how many sequences were better than E. */
        prelim_gap_no_contest,	/* No. of HSP's under E=10 alone. */
        prelim_gap_passed,	/* No. of HSP's that passed prelim gapping. */
        prelim_gap_attempts,	/* No. of HSP's we attempted to gap. */
        real_gap_number_of_hsps, /* How many HSP's were gapped in BlastGetGappedScore. */
        semid;                  /* Here will be stored ID of load-ballance semaphore */
    /* Callback for the queueing system. */
    int (LIBCALLBACK *queue_callback)PROTO((int semid, int num, int num_cpu));
    GreedyAlignMemPtr abmp; /* Memory for megablast greedy extension */
    Int4 PNTR query_context_offsets; /* offsets for all queries and strands in a 
                                        concatenated sequence */
    SeqIdPtr PNTR qid_array; /* Ids of all queries in Mega BLAST search */
    BLASTResultsStructPtr PNTR mb_result_struct; /* one result struct per query
                                                    for Mega BLAST */
    ValNodePtr mb_endpoint_results; /* Points to linked list of results  */
} BlastSearchBlk, PNTR BlastSearchBlkPtr;
    
typedef struct _blast_hsp_segment {
   Int4 q_start, q_end;
   Int4 s_start, s_end;
   struct _blast_hsp_segment PNTR next;
} BLASTHSPSegment, PNTR BLASTHSPSegmentPtr;

#ifdef __cplusplus
}
#endif
#endif /* !__BLASTSTR__ */
