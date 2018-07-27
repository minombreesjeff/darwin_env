/*  sequtil.c
* ===========================================================================
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
* ===========================================================================
*
* File Name:  sequtil.c
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.175 $
*
* File Description:  Sequence Utilities for objseq and objsset
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: sequtil.c,v $
* Revision 6.175  2005/04/28 19:24:45  kans
* DR prefix moved from STS to EST
*
* Revision 6.174  2005/04/27 17:24:21  kans
* support seqid_gpipe in combination with a more public accession type
*
* Revision 6.173  2005/04/26 21:33:52  kans
* added SEQID_GPIPE
*
* Revision 6.172  2005/04/05 15:25:30  kans
* added DR as NCBI STS
*
* Revision 6.171  2005/03/29 14:46:42  papadopo
* use a different start point when changing StdSeg-type seqalign offsets, if the underlying SeqLoc specifes the minus strand
*
* Revision 6.170  2005/03/22 20:42:03  kans
* added DP and DQ accession prefixes
*
* Revision 6.169  2005/02/01 13:28:55  kans
* added DN as NCBI EST accession prefix
*
* Revision 6.168  2005/01/25 18:13:47  kans
* added CS as EMBL patent
*
* Revision 6.167  2004/12/08 19:44:52  kans
* added CZ as NCBI GSS
*
* Revision 6.166  2004/12/08 15:14:51  kans
* added ACCN_DDBJ_GSS, accession prefix DE
*
* Revision 6.165  2004/12/02 19:02:47  kans
* added CY accession prefix
*
* Revision 6.164  2004/11/22 20:50:15  kans
* added CX as NCBI EST
*
* Revision 6.163  2004/11/12 14:45:39  kans
* added DD as DDBJ patent prefix
*
* Revision 6.162  2004/11/08 18:19:44  madden
* GetScoreAndEvalue change to return number of hsps (in linked set) for most significant set.
*
* Revision 6.161  2004/10/13 16:46:52  kans
* added DA, DB, DC as DDBJ_EST
*
* Revision 6.160  2004/09/21 22:34:36  dondosha
* Get the number of linked HSPs for a DenDiag from the first segment, not from the largest linked set
*
* Revision 6.159  2004/09/15 13:02:02  kans
* added CW to WHICH_db_accession as NCBI GSS
*
* Revision 6.158  2004/08/20 18:55:27  kans
* SeqLocStrand skips NULL or EMPTY components of MIX to avoid giving incorrect unknown result
*
* Revision 6.157  2004/08/17 19:35:44  kans
* BSPack uses BSRead instead of BSGetByte for significant speed increase
*
* Revision 6.156  2004/08/06 17:15:50  kans
* added CV as NCBI EST
*
* Revision 6.155  2004/08/04 17:15:16  kans
* added AccnInUniProt - still need AccnIsSWISSPROT for old style
*
* Revision 6.154  2004/07/21 18:05:00  rsmith
* SeqLocStart and SeqLocStop were not handling SEQLOC_PACKED_PNTs properly
*
* Revision 6.153  2004/07/14 22:55:04  dondosha
* Add version in GetAccessionVersionFromSeqId only if it is > 0
*
* Revision 6.152  2004/07/14 22:46:08  dondosha
* Added GetAccessionVersionFromSeqId function to extract Accession.version from a Seq-id
*
* Revision 6.151  2004/07/14 19:09:19  kans
* added CP for ACCN_NCBI_GENOME in WHICH_db_accession
*
* Revision 6.150  2004/06/04 17:31:34  kans
* added CN and CO accession prefixes
*
* Revision 6.149  2004/05/27 15:37:31  kans
* fixed typo in WHICH_db_accession - new 12-character RefSeq test used wrong comparison for underscore test
*
* Revision 6.148  2004/05/25 20:46:18  kans
* WHICH_db_accession handles 12-character RefSeq accessions
*
* Revision 6.147  2004/03/30 20:29:33  kans
* in static std_order array within SeqIdBestRank, demoted gibbsq, gibbmt, and patent
*
* Revision 6.146  2004/03/16 22:08:31  kans
* added CL to WHICH_db_accession as NCBI EST
*
* Revision 6.145  2004/03/15 18:42:59  coulouri
* Handle memory allocation failure gracefully with BSRebuildDNA_4na
*
* Revision 6.144  2004/01/16 16:37:01  kans
* added CM as scaffold/CON record
*
* Revision 6.143  2003/12/18 19:35:53  kans
* added CQ and CR prefixes
*
* Revision 6.142  2003/12/16 16:03:04  kans
* added CL as ncbi gss
*
* Revision 6.141  2003/11/10 16:12:33  kans
* added CK as genbank est
*
* Revision 6.140  2003/10/31 20:51:24  kans
* added CI and CJ as DDBJ ESTs to WHICH_db_accession
*
* Revision 6.139  2003/10/24 14:36:12  kans
* added CH as GenBank CONN to WHICH_db_accession
*
* Revision 6.138  2003/09/09 20:08:18  kans
* SeqLocPartialCheck locks bioseq if seqloc_whole and far
*
* Revision 6.137  2003/09/02 15:11:50  kans
* WHICH_db_accession takes ZP_ with 8 digits as refseq_prot_predicted
*
* Revision 6.136  2003/08/11 13:45:18  kans
* added CG as ncbi gss
*
* Revision 6.135  2003/07/14 20:17:53  kans
* added CF as ncbi est to WHICH_db_accession
*
* Revision 6.134  2003/07/02 14:35:21  kans
* added CE as ncbi gss
*
* Revision 6.133  2003/05/20 22:15:24  yaschenk
* SeqIdSelect loops indefinitely on corrupted memory
*
* Revision 6.132  2003/04/30 16:40:41  kans
* added CD as GenBank EST
*
* Revision 6.131  2003/03/25 13:32:22  kans
* added CC as ncbi gss accession prefix
*
* Revision 6.130  2003/03/24 19:41:56  kans
* added tmsmart_order, use in SeqIdWrite to prevent TMSMART temporary ID from being used
*
* Revision 6.129  2003/02/20 19:05:31  ford
* Modified MakeNewProteinSeqIdExMT() to create an ID of maximum length 30 instead of 20.
*
* Revision 6.128  2003/01/21 17:06:57  kans
* implement PRINTID_FASTA_ALL SeqIdWrite
*
* Revision 6.127  2003/01/13 18:15:35  kans
* added CB as NCBI EST prefix
*
* Revision 6.126  2002/11/05 18:50:31  kans
* fixed bug in SeqLocPartialCheck
*
* Revision 6.125  2002/10/19 19:11:21  kans
* added CA as GenBank EST
*
* Revision 6.124  2002/10/03 16:18:35  kans
* added BZ as NCBI GSS
*
* Revision 6.123  2002/09/20 20:47:51  kans
* added BY as DDBJ EST prefix
*
* Revision 6.122  2002/08/28 13:28:54  kans
* added BX prefix
*
* Revision 6.121  2002/08/26 20:38:26  kans
* added BW as ddbj est prefix
*
* Revision 6.120  2002/08/19 15:57:28  kans
* BV is NCBI STS prefix
*
* Revision 6.119  2002/08/19 15:54:47  kans
* added BU as NCBI EST prefix
*
* Revision 6.118  2002/07/12 18:47:46  kans
* WHICH_db_accession was using the result of AccnIsSWISSPROT incorrectly
*
* Revision 6.117  2002/07/08 20:25:45  kans
* added BT as FLI_cDNA type
*
* Revision 6.116  2002/06/28 14:48:03  kans
* added BS as ddbj genome project prefix
*
* Revision 6.115  2002/06/20 18:38:43  kans
* added FAA and GAA, ACCN_NCBI_WGS_PROT, ACCN_EMBL_WGS_PROT, ACCN_DDBJ_WGS_PROT, and ACCN_IS_WGS
*
* Revision 6.114  2002/06/19 17:13:48  kans
* added ACCN_PDB, support for PDB in SeqIdFromAccessionDotVersion
*
* Revision 6.113  2002/06/10 18:06:16  kans
* SeqLocLen use of smp->seq_len_lookup_func first checks sip for NULL
*
* Revision 6.112  2002/06/10 14:07:12  kans
* SeqLocLen on whole tries new smp->seq_len_lookup_func registered function
*
* Revision 6.111  2002/05/29 19:19:53  bazhin
* Added support for new EAA-EZZ protein's WGS accessions.
*
* Revision 6.110  2002/04/24 17:11:03  kans
* added BR as DDBJ TPA accession prefix
*
* Revision 6.109  2002/04/02 18:19:56  kans
* SeqLocPartialCheck fixes
*
* Revision 6.108  2002/03/26 18:11:26  kans
* WHICH_db_accession WGS assignments - A*** NCBI, B*** DDBJ, C*** EMBL
*
* Revision 6.107  2002/03/12 17:08:32  kans
* added BQ as NCBI EST
*
* Revision 6.106  2002/02/14 18:30:27  kans
* SeqIdFromAccessionDotVersion defaults version to INT2_MIN
*
* Revision 6.105  2002/01/29 19:28:16  kans
* SeqIdParse can parse type|accession.ver with no trailing vertical bars for RefSeq and DNA database types
*
* Revision 6.104  2002/01/22 18:49:15  kans
* added ACCN_NCBI_WGS, ACCN_EMBL_WGS, and ACCN_DDBJ_WGS
*
* Revision 6.103  2002/01/17 13:49:32  kans
* BP added as DDBJ EST accession prefix
*
* Revision 6.102  2002/01/16 16:59:38  camacho
* Changed the type of buflen parameter in SeqIdWrite from Int2 to Uint4
*
* Revision 6.101  2001/11/29 14:04:29  kans
* reverted GetThePointForOffset, deal with trans-splicing in feature indexing left/right extreme calculation itself
*
* Revision 6.100  2001/11/14 13:55:45  kans
* added BN accession
*
* Revision 6.99  2001/11/08 13:49:45  kans
* GetThePointForOffset on left or right end now handles trans-spliced mixed strands properly - still need to fix SeqLocMerge
*
* Revision 6.98  2001/10/23 14:04:18  kans
* added BM and BL accession prefixes
*
* Revision 6.97  2001/09/28 22:42:49  vakatov
* Renamed "new" to "x_new" -- to avoid clash with the C++ "operator new"
*
* Revision 6.96  2001/09/28 14:31:00  madden
* Added functions BSCompressDNANew and GenericCompressDNAEx for long sequences with ambiguity
*
* Revision 6.95  2001/09/20 18:53:28  kans
* changed SeqLocCompare matrices to handle artificial frameshifts
*
* Revision 6.94  2001/08/07 18:12:05  kans
* added macros for EMBL and DDBJ TPA and TPA_PROT prefixes
*
* Revision 6.93  2001/08/06 22:13:12  kans
* using NUM_SEQID, added TPA ids to arrays
*
* Revision 6.92  2001/08/06 20:07:53  ostell
* added support for SEQID_TPG, TPE, TPD types
*
* Revision 6.91  2001/08/01 17:56:24  kans
* in SeqLocGetSegLens on mix or equiv, ctr += changed to ctr = to avoid multiple increments
*
* Revision 6.90  2001/07/03 21:42:01  kans
* added macros and accession prefixes for TPA (third-party annotation) records
*
* Revision 6.89  2001/06/06 17:53:42  kans
* added BI and BJ accession prefixes
*
* Revision 6.88  2001/06/05 16:28:40  kans
* restored logic of GetThePointForOffset, with comment that SeqLocStart returns sintp->from even on minus strand, different behavior than SEQLOC_START
*
* Revision 6.87  2001/06/04 19:31:14  kans
* PDB lower case chain now represented by double upper case letter, modified SeqIdWrite, SeqIdParse, SeqIdComp, removed DeltaSeqsToSeqLoc call and variables from CheckPointInBioseq
*
* Revision 6.86  2001/06/04 19:07:48  kans
* fixed buggy logic for minus strand LEFT_END/RIGHT_END in GetThePointForOffset
*
* Revision 6.85  2001/06/01 21:11:45  kans
* CheckPointInBioseq now works for far delta sequences, calls SeqMgrMapPartToSegmentedBioseq, allowing NG propagation onto NT genomic records
*
* Revision 6.84  2001/05/30 22:47:23  kans
* fixed Mac compiler warnings about unwanted assignments, moved function prototypes to header, removed unused variables
*
* Revision 6.83  2001/05/30 20:30:31  shoemake
* Fixed bug in SeqIdParse for VB chain in SEQID_PDBs.
*
* Revision 6.82  2001/05/14 19:42:37  sicotte
* Added BH prefix for ncbi GSS
*
* Revision 6.81  2001/05/04 22:09:45  dondosha
* Small modification in GetAccessionFromSeqId for local id
*
* Revision 6.80  2001/04/27 15:46:20  madden
* Add function RebuildDNA_4na
*
* Revision 6.79  2001/03/23 16:56:26  dondosha
* Correction in function GetAccessionFromSeqId
*
* Revision 6.78  2001/01/30 18:11:13  kans
* SeqIdParse for general and local counts digits, and any number > INT4_MAX will be stored as a string
*
* Revision 6.77  2000/12/20 20:59:58  sicotte
* bug fix for AccnnIsSWISSPROT
*
* Revision 6.76  2000/12/07 16:34:42  sicotte
* Updated WHICH_db_accession and corresponding macros: I* accessions can no longer be proteins (they were PIR) and have completed hardcoding of N000?? accessions which can belong to twoDB. Added SeqIdFromAccessionEx, ACCN_PIR_FORMAT, and AccnIsSWISSPROT functions
*
* Revision 6.75  2000/12/05 23:10:29  kans
* SeqIdParse does not override default pdb chain if no tokens[1] content
*
* Revision 6.74  2000/11/20 17:13:47  kans
* SeqIdParse uses SEQID_PARSE_BUF_SIZE instead of 40 character limit - needed to handle humongously long local IDs in genome annotation models, which will be removed when loaded into ID, but still need to be dealt with during processing
*
* Revision 6.73  2000/11/16 17:23:26  sicotte
* IS_protdb_accession is now true for any 3 letter accession and IS_ntdb_accession is now also true for any unknown accession-looking accession number
*
* Revision 6.72  2000/11/14 20:49:48  sicotte
* add XM_ refseq prefix
*
* Revision 6.71  2000/10/31 21:20:05  vakatov
* [WIN32] DLL'zation
*
* Revision 6.70  2000/10/27 20:10:57  shavirin
* Added new function MakeNewProteinSeqIdExMT for MT save operation.
*
* Revision 6.69  2000/10/26 15:28:58  dondosha
* Do not adjust offsets for empty SeqLocs in AdjustOffSetsInSeqAlign
*
* Revision 6.68  2000/10/24 19:04:18  dondosha
* Moved function UniqueLocalId from blast.c
*
* Revision 6.67  2000/10/11 21:59:35  kans
* added PRINTID_FASTA_GENERAL
*
* Revision 6.66  2000/10/11 18:33:36  kans
* SeqIdWrite fasta_order prefers SEQID_OTHER, then SEQID_GENERAL, then SEQID_LOCAL
*
* Revision 6.65  2000/10/03 16:51:07  cavanaug
* Added BF accession prefix to WHICH_db_accession
*
* Revision 6.64  2000/10/02 14:30:48  sicotte
* Added BE prefix for NCBI EST to WHICH_db_accession
*
* Revision 6.63  2000/10/02 14:29:16  sicotte
* Added BE prefix for NCBI EST to WHICH_db_accession
*
* Revision 6.62  2000/09/28 15:08:31  dondosha
* Corrected local id handling in GetAccessionFromSeqid - used in Mega BLAST
*
* Revision 6.61  2000/09/19 15:22:07  sicotte
* fix BD and BE prefix
*
* Revision 6.60  2000/09/12 20:20:13  cavanaug
* WHICH_db_accession : fixed missing paren
*
* Revision 6.59  2000/09/12 19:54:22  cavanaug
* WHICH_db_accession now knows about BE accessions
*
* Revision 6.58  2000/08/02 21:26:15  kans
* SeqIdWrite will not print version if release is used
*
* Revision 6.57  2000/07/11 15:01:50  kans
* added SeqIdFromAccessionDotVersion for genome mapping project
*
* Revision 6.56  2000/06/08 14:30:07  dondosha
* Fixed bug in GetAccessionFromSeqId for general id
*
* Revision 6.55  2000/05/22 17:37:01  sicotte
* add BD prefix to WHICH_db_accession
*
* Revision 6.54  2000/05/17 17:20:43  dondosha
* Added function GetAccessionFromSeqId, used in standalone programs blastclust and megablast
*
* Revision 6.53  2000/05/10 16:56:33  kans
* SeqIdParse stops looking for expected_tokens when done is TRUE, avoids reading past end of string for ref|xxx| with two bars
*
* Revision 6.52  2000/05/05 13:24:50  kans
* For SEQID_OTHER (RefSeq), SeqIdWrite will not print third vertical bar (originally for when there would be periodic releases - obsolete now that RefSeq is part of Entrez), and SeqIdParse will not upper case the tsip->name (LOCUS)
*
* Revision 6.51  2000/05/04 15:07:54  kans
* SeqIdParse sends SEV_INFO, not SEV_ERROR, since blast uses it to test IDs for validity
*
* Revision 6.50  2000/05/03 17:56:05  sicotte
* Added BC prefix ; changed refseq SeqID
*
* Revision 6.49  2000/03/29 15:53:50  sicotte
* Added BB predix for DDBJ EST to WHICH_db_accession
*
* Revision 6.48  2000/02/17 17:29:52  sicotte
* Added BA prefix for DDBJ CON division
*
* Revision 6.47  2000/02/11 21:09:52  madden
* Check for NULL SeqAlignPtr before dereferencing
*
* Revision 6.46  2000/02/04 16:13:16  kans
* added prefix parameter to MakeNewProteinSeqIdEx
*
* Revision 6.45  1999/12/22 15:46:19  sicotte
* Added prefix AZ
*
* Revision 6.44  1999/11/22 17:28:01  kans
* allow RefSeq ID to have two or (expected) three tokens
*
* Revision 6.43  1999/10/12 13:46:42  kans
* AX and AY added to WHICH_db_accession
*
* Revision 6.42  1999/08/31 20:49:03  shavirin
* Added case 5 (discontinous alignment) in function TxGetIdFromSeqAlign().
*
* Revision 6.41  1999/08/11 15:49:21  sicotte
* Add AW prefix for NCBI EST in WHICH_db_accession
*
* Revision 6.40  1999/08/05 20:52:25  sicotte
* fix in SeqIdFindBestAccession
*
* Revision 6.39  1999/08/03 20:00:34  sicotte
* Add SeqIdFindBestAccession
*
* Revision 6.38  1999/07/30 09:28:17  sicotte
* Transfered fns from salutil.c(chappey) AddSeqId,SeqIdDupList,SeqIdDupBestList,SeqIdListfromSeqLoc
*
* Revision 6.37  1999/06/24 20:39:21  kans
* SeqIdPrint just calls SeqIdWrite, PRINTID_TEXTID_LOCUS uses accession (no version) if no locus name
*
* Revision 6.36  1999/05/24 22:22:25  sicotte
* SeqIdFromAccession: Fix bug for embl/ddbj accessions
*
* Revision 6.35  1999/05/20 14:33:13  sicotte
* SeqIdFromAccession: remove tsp->name to accomodate LOCUS names
*
* Revision 6.34  1999/04/22 14:52:34  tatiana
* changes in MuskSeqIdWrite to print human chromosome Ids
*
* Revision 6.33  1999/04/20 14:34:53  sicotte
* Add AV accession prefix for DDBJ EST in WHICH_db_accession
*
* Revision 6.32  1999/04/08 14:11:16  sicotte
* Add SeqIdOrderInBioseqIdList (consider synomymous SeqIds)
*
* Revision 6.31  1999/04/02 17:31:59  vakatov
* Added NLM_EXTERN for FindNuc() and FindProt() proto
*
* Revision 6.30  1999/04/01 17:41:20  sicotte
* Added SeqIdInSeqLocList : Check if the Bioseq of SeqId sip is in list: may try to fetch the Bioseq
*
* Revision 6.29  1999/04/01 13:54:47  sicotte
* Added SeqIdOrderInList(To find the position of a SeqId in a Chain)
*       ExtractAccession ( To parse an accession into it's version and acc.)
*       SeqIdFromAccession (To make the proper type of SeqId given an
*                           accession string. (uses WHICH_db_accession)
*       moved IS_ntdb_accession, IS_protdb_accession, WHICH_db_accession
*          from accutils.ch to sequtil.ch
*
* Revision 6.28  1999/03/31 16:53:40  madden
* Added FindNuc and FindProt functions for SeqEntryExplore
*
* Revision 6.27  1999/03/11 23:32:06  kans
* sprintf casts
*
* Revision 6.26  1999/03/04 17:10:14  kans
* check for tsip->release == NULL for showversion (JO)
*
* Revision 6.25  1999/02/22 15:34:22  kans
* print version if > 0 (had erroneously been > 1)
*
* Revision 6.24  1999/02/09 20:15:00  kans
* initialized version, release, in stack textseqid (JO)
*
* Revision 6.23  1999/02/02 21:38:12  kans
* moved SHOWVERSION to header, SeqMgrAddIndexElement with and without version (JO)
*
* Revision 6.22  1999/02/01 18:41:02  ostell
* added SHOWVER define to stop printing versions until ready
*
* Revision 6.21  1999/02/01 16:24:45  ostell
* check for accession.version >0 not just non-zero
*
* Revision 6.20  1999/02/01 16:17:12  ostell
* added version support for SeqIdWrite and SeqIdParse
*
* Revision 6.19  1999/01/12 18:00:18  kans
* SeqIdComp now ignores version if < 1, and added PRINTID_TEXTID_ACC_VER and PRINTID_TEXTID_ACC_ONLY formats for SeqIdWrite
*
* Revision 6.18  1999/01/11 17:11:22  kans
* SeqIdComp now checks version numbers
*
* Revision 6.17  1999/01/05 18:30:26  kans
* SeqLocId ignores SEQLOC_NULL (Tatiana)
*
* Revision 6.16  1998/12/14 20:56:26  kans
* dnaLoc_to_aaLoc takes allowTerminator parameter to handle stop codons created by polyA tail
*
* Revision 6.15  1998/11/24 20:15:05  kans
* seqid other has better priority than local so refgene id is used preferentially
*
* Revision 6.14  1998/11/12 15:48:35  kans
* SEQID_OTHER now prints as "ref" instead of "oth", though the latter is still detected by SeqIdParse as a special case
*
* Revision 6.13  1998/10/19 15:49:08  kans
* SeqIdWrite now writes three fields for SEQID_OTHER in FASTA_SHORT
*
* Revision 6.12  1998/08/26 20:56:06  kans
* got SeqIdComp test for general ID db strings wrong the first time
*
* Revision 6.11  1998/08/26 20:29:45  kans
* SeqIdComp allows multiple general IDs with different db names
*
* Revision 6.10  1998/08/18 21:42:44  kans
* SeqIdIn does not return on SIC_NO in order to handle multiple IDs of the same class (e.g., general)
*
* Revision 6.9  1998/08/13 22:31:48  kans
* SeqMgrMapPartToSegmentedBioseq to speed up GetOffsetInBioseq, start of indexing segments, also index biosource by location for binary search (Wheelan)
*
* Revision 6.8  1998/07/28 16:41:08  kans
* added MakeNewProteinSeqIdEx for faster creation of many new protein products
*
* Revision 6.7  1998/05/05 15:54:39  kans
* SeqIdMatch now handles SEQID_OTHER (JO)
*
* Revision 6.6  1998/04/22 18:07:22  egorov
* Make possible the second paramter (SeqLocPtr slp2) of AdjustOffSetsInSeqAlign to be NULL
*
* Revision 6.5  1998/02/27 17:10:01  vakatov
* [WIN32 DLL]  Declared some functions as NLM_EXTERN(DLL-exportable)
*
* Revision 6.4  1998/02/26 19:14:03  madden
* Added AdjustOffSetsInSeqAlign
*
* Revision 6.3  1998/01/26 17:25:30  madden
* Added TxGetQueryIdFromSeqAlign function
*
* Revision 6.2  1997/10/15 14:43:18  ostell
* changed SeqIdPrint, SeqIdWrite, SeqIdParse to convert PDB chain id '|' into VB
*
* Revision 6.1  1997/10/06 14:01:23  zjing
* move TxGetSubjectId, GetScoreAndEvalue to sequtil.ch
*
* Revision 6.0  1997/08/25 18:07:20  madden
* Revision changed to 6.0
*
* Revision 5.43  1997/08/19 17:07:09  kans
* EntrezASN1Detected now explores all components
*
* Revision 5.42  1997/07/17 18:46:21  kans
* SeqLocStrand now treats mixed unknown and plus as plus
*
* Revision 5.41  1997/06/19 18:38:56  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.40  1997/05/19 19:25:26  shavirin
* Added function CorrectGeneFeatLocation()
*
 * Revision 5.39  1997/04/09  18:43:01  tatiana
 * a typo in MolTypeForGi description fixed
 *
 * Revision 5.38  1997/04/09  18:00:10  tatiana
 * added MolTypeForGI()
 *
 * Revision 5.37  1997/04/08  17:27:51  shavirin
 * Fixed typecast warnings.
 *
 * Revision 5.36  1997/04/04  17:24:44  shavirin
 * Fixed position calculation in BSRebuildDNA_4na() function
 *
 * Revision 5.35  1997/03/28  21:18:10  shavirin
 * Added function BSRebuildDNA_4na()
 *
 * Revision 5.34  1997/03/18  23:22:25  kans
 * changed return NULL to return FALSE in GenericCompressDNA (detected by
 * CodeWarrior)
 *
 * Revision 5.33  1997/03/18  19:16:30  shavirin
 * Added error handling in GenericCompressDNA() function
 *
 * Revision 5.32  1997/03/18  15:26:31  ostell
 * made SeqIdComp case insensitive
 *
 * Revision 5.31  1997/03/06  22:56:31  kans
 * added include <seqport.h> for dnaLoc_to_aaLoc
 *
 * Revision 5.30  1997/03/06  22:46:50  shavirin
 * Moved SPCompress functions to seqport.c
 *
 * Revision 5.29  1997/03/06  21:27:30  shavirin
 * Added new set of functions: SPCompressDNA(), SPRebuildDNA(), SPCompressNew(),
 * SPCompressFree()
 *
 * Revision 5.28  1997/03/04  21:58:48  shavirin
 * Added function GenericCompressDNA()
 *
 * Revision 5.27  1997/03/04  19:57:03  epstein
 * fix minus strand problem for gatherCodingRegions()
 *
 * Revision 5.26  1997/03/04  03:51:32  shavirin
 * Fixed memory leak in BSCompressDNA()
 *
 * Revision 5.25  1997/03/03  22:34:09  shavirin
 * Changed IO Buffer from 100/1 to 1024/1024 in BSCompressDNA
 *
 * Revision 5.23  1997/02/26  20:14:16  shavirin
 * Removed RandomSeed initialization in SeqMapTableFind()
 *
 * Revision 5.22  1997/02/26  19:24:16  shavirin
 * Optimized by increasing IO buffers function BSConvertSeq()
 *
 * Revision 5.21  1997/02/03  20:56:24  ostell
 * added additional check to MakeNewProteinSeqId() for an existing id in
 * GenBank locus form.
 *
 * Revision 5.20  1997/01/08  22:24:26  zjing
 * changes in MuskSeqIdWrite to Print THC Ids
 *
 * Revision 5.19  1996/12/10  19:17:39  kans
 * *** empty log message ***
 *
 * Revision 5.18  1996/11/18  20:37:37  shavirin
 * "Pack" set of functions now will produce ncbieaa Seq_code
 * for proteins.
 *
 * Revision 5.17  1996/10/15  15:44:08  shavirin
 * *** empty log message ***
 *
 * Revision 5.16  1996/10/15  14:47:18  shavirin
 * Added new functions: BSCompressDNA() and BSRebuildDNA() to handle
 * ambiguity characters.
 *
 * Revision 5.15  1996/10/11  21:08:28  chappey
 * backed out changes to add extra byte at end of bytestore
 *
 * Revision 5.14  1996/10/10  21:06:01  shavirin
 * Implemented random conversion of ASCII and ncbi4na encoding
 * to ncbi2na in case of ambiguous bases.
 *
 * Revision 5.13  1996/10/09  19:01:37  shavirin
 * Changed format of last byte in BSConvertSeq() function.
 *
 * Revision 5.12  1996/10/04  19:07:31  shavirin
 * Rewritten function BSPack() with more efficient algorithm
 *
 * Revision 5.11  1996/09/26  01:51:37  ostell
 * fixed logic error in BioseqConvert()
 *
 * Revision 5.10  1996/08/26  14:07:52  madden
 * Changed try to tax_try in for compatability with DEC OS.
 *
 * Revision 5.9  1996/08/07  20:13:04  epstein
 * move MuskSeqIdWrite, seqid_name, local_id_make and update_seq_loc from jzmisc to sequtil, to untangle the desktop and tools libraries from one another
 *
 * Revision 5.8  1996/07/15  19:03:57  epstein
 * adapt to dnaLoc_to_aaLoc() change
 *
 * Revision 5.7  1996/07/15  15:57:23  kans
 * fixed a call to ISA_na, removed an unused variable
 *
 * Revision 5.6  1996/07/15  14:41:53  epstein
 * add SeqEntryContainsSeqIdOfMolType() and FindCodingRegion() and add performance improvements to FindSpliceSites()
 *
 * Revision 5.5  1996/07/11  16:25:31  kans
 * MakeNewProteinSeqId default string is now tmpseq_
 *
 * Revision 5.4  1996/06/27  19:58:39  epstein
 * return NULL from FindSpliceSites if there are no splice sites
 *
 * Revision 5.3  1996/06/24  18:38:24  epstein
 * move splicing functionality to sequtil.c
 *
 * Revision 5.2  1996/06/16  15:19:31  ostell
 * added delta seq support to SeqEntryPack
 *
 * Revision 5.1  1996/06/12  18:29:14  epstein
 * move SeqLocIntNew() and SeqLocPntNew() from edutil to sequtil
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.18  1996/03/25  22:18:32  tatiana
 * *** empty log message ***
 *
 * Revision 4.17  1996/03/11  19:06:43  ostell
 * added MakeNewProteinSeqId()
 *
 * Revision 4.16  1996/02/28  04:53:06  ostell
 * fix for SeqLocFindNext and ..Part for head haveing next pointer
 *
 * Revision 4.15  1996/02/26  03:44:33  ostell
 * added support for SEQLOC_BOND to SeqLocStart, SeqLocStop, SeqLocID, SeqLocStrand
 *
 * Revision 4.14  1996/02/24  03:11:40  ostell
 * made GetResidueForSymbol ruturn INVALID=-RESIDUE when letter is 0
 *
 * Revision 4.13  1996/01/31  16:08:16  tatiana
 * StringForSeqTech() added by Tatiana
 *
 * Revision 4.12  1995/12/27  22:30:21  ostell
 * added support for delta seqs in BioseqGetSegs, ..GetGaps
 *
 * Revision 4.11  1995/12/22  20:18:10  ostell
 * fix to SeqIdWrite to fall though PRINTID_FASTA_LONG when no gi is present
 *
 * Revision 4.10  1995/12/05  21:43:13  ostell
 * changed SeqIdWrite to format a little nicer
 *
 * Revision 4.9  1995/10/19  02:30:31  ostell
 * fixed SeqLocFindNext() bug that caused early termination at the
 * end of nested locations.
 *
 * Revision 4.8  1995/10/17  04:15:37  ostell
 * added SeqLocFindPart()
 * made SeqLocFindNext() call it
 * added equiv_is_one to IS_one_loc()
 *
 * Revision 4.7  1995/10/12  18:11:23  kans
 * corrected SeqLocLen for SEQLOC_MIX (JO)
 *
 * Revision 4.6  1995/09/29  15:03:06  ostell
 * changed SeqIdParse() to retain first Seqid if failure only on second
 *
 * Revision 4.5  1995/09/28  20:51:13  tatiana
 * SeqLocLen changed to handle case SEQLOC_EQUIV
 *
 * Revision 4.3  1995/09/06  21:06:28  ostell
 * fixed logical condition in SeqLocPartialCheck() that looked at Int-fuzz
 * lim type >= 0 || <= 2 (for unk, gt, lt).. changed || to &&
 *
 * Revision 4.2  1995/08/24  20:15:31  tatiana
 * bug fixed in sequtil.c
 *
 * Revision 4.1  1995/08/09  17:00:15  tatiana
 * bug fixed in SeqIdWrite()
 *
 * Revision 2.80  1995/07/20  22:08:39  tatiana
 * change SeqIdprint to SeqIdWrite
 *
 * Revision 2.79  1995/07/18  19:53:22  tatiana
 * add SeqIdWrite() to replace SeqIdPrint()
 *
 * Revision 2.78  1995/06/27  20:39:58  kans
 * SeqIdForSameBioseq no longer fetches
 *
 * Revision 2.77  1995/06/14  16:31:59  kans
 * EntrezASN1Detected function added (to be used by Sequin and ID)
 *
 * Revision 2.76  1995/06/06  16:25:38  ostell
 * added support for MolInfo in SeqLocPartialCheck()
 *
 * Revision 2.75  1995/05/19  04:02:08  ostell
 * added SeqLocAinB()
 *
 * Revision 2.74  1995/05/09  18:10:09  ostell
 * changed to using NUM_SEQID
 *
 * Revision 2.74  1995/05/09  18:10:09  ostell
 * changed to using NUM_SEQID
* ==========================================================================
*/

/** for ErrPostEx() ****/

static char *this_module = "ncbiapi";
#define THIS_MODULE this_module
static char *this_file = __FILE__;
#define THIS_FILE this_file

/**********************/

#include <sequtil.h>
#include <gather.h>
#include <seqport.h>
#include <sqnutils.h> /* prototype for SeqIdFindWorst */
#include <edutil.h>


/****  Static variables used for randomized sequence conversions ****/

/* This array contains final residues for ncbi2na encoding. 
   Na42[4] - number of possible choises for ambiguous residues
   and these residues plased in Na42[0-3]     */

static Int1  Na42[16][5] = { 
  { 0, 1, 2, 3, 4} , { 0, 0, 0, 0, 1 }, { 1, 1, 1, 1, 1} , { 0, 1, 0, 1, 2},
  { 2, 2, 2, 2, 1} , { 0, 2, 0, 2, 2 }, { 1, 2, 1, 2, 2} , { 0, 1, 2, 2, 3},
  { 3, 3, 3, 3, 1} , { 0, 3, 0, 3, 2 }, { 1, 3, 1, 3, 2} , { 0, 1, 3, 3, 3},
  { 2, 3, 2, 3, 2} , { 0, 2, 3, 3, 3 }, { 1, 2, 3, 3, 3} , { 0, 1, 2, 3, 4}
};

/* This array contains check values if we can do direct conversion */

static Int1    Na42Set[16] = { -1,  0,  1, -1,  2, -1, -1, -1,
                                3, -1, -1, -1, -1, -1, -1, -1 };

/* Analog arrays for ASCII --> ncbi2na conversion 
   NOTE: dimensions for NaI2 are reversed to allocate it 
   dynamically */

static Int1    NaI2Set[256];
static Int1Ptr NaI2[5];

static Boolean NaI2InitOk = FALSE;  /* We will allocate it only ones */

/* Macros for random conversion */

#define CONVERT_42_RAND(from) Na42[from][(Nlm_RandomNum()>>8)%Na42[from][4]]
#define CONVERT_I2_RAND(from) NaI2[(Nlm_RandomNum()>>8)%NaI2[4][from]][from]

static Boolean InitNaI2Table(void);

/**********************************************************************/

/*   Defines for compression/rebuild DNA */

#define BSC_BUFF_CHUNK 1024
#define RES_OFFSET(x) x & 0xFFFFFF 
#define RES_VALUE(x)  x>>28 
#define RES_LEN(x)    (x>>24) & 0xF
#define RES_LEN_NEW(x)    (x>>16) & 0xFFF
#define LEN_STEP_MASK 0x1000000
#define LEN_STEP_MASK_NEW 0x10000

static NumberingPtr stdnum = NULL;  /* std Numbering object (start at 1) */

/* find the last nucleotide bioseq in the bioseqset */
/* Used by SeqEntryExplore. */
NLM_EXTERN void FindNuc(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
    BioseqPtr PNTR bp;
    BioseqPtr local_bsp;

    bp = (BioseqPtr PNTR) data;
    if (IS_Bioseq(sep))
    {
        local_bsp = (BioseqPtr) sep->data.ptrvalue;
        if (ISA_na(local_bsp->mol))
          *bp = local_bsp;
    }
}

/* find the last protein bioseq in the bioseqset */
/* Used by SeqEntryExplore. */
NLM_EXTERN void FindProt(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
    BioseqPtr PNTR bp;
    BioseqPtr local_bsp;

    bp = (BioseqPtr PNTR) data;
    if (IS_Bioseq(sep))
    {
        local_bsp = (BioseqPtr) sep->data.ptrvalue;
        if (ISA_aa(local_bsp->mol))
          *bp = local_bsp;
    }
}

/*****************************************************************************
*
*   Boolean BioseqMatch(bsp, seqid)
*       returns TRUE if bsp points to the Bioseq identified by seqid
*
*****************************************************************************/
NLM_EXTERN Boolean BioseqMatch (BioseqPtr bsp, SeqIdPtr seqid)
{
	if (bsp == NULL) return FALSE;
	return SeqIdIn(seqid, bsp->id);
}


typedef struct findse {
	SeqIdPtr sip;
	Boolean found;
	BioseqPtr bsp;
	Int4 indent;
} fse, PNTR fseptr;

typedef struct {
	SeqLocPtr slp;
	Boolean findOnProtein;
} SpliceInfo, *SpliceInfoPtr;

typedef struct {
	SeqIdPtr sip;
	Boolean isProtein;
	Boolean retval;
} SeqIdChecker, *SeqIdCheckerPtr;

typedef struct {
	SeqIdPtr sip;
	Int2 mtype;
} SeqIdMolType,  PNTR SeqIdMolTypePtr;

/*****************************************************************************
*
*   FindSE()
*      SeqEntryExplore function used by SeqEntryFind()
*
*****************************************************************************/
NLM_EXTERN void FindSE (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
NLM_EXTERN void FindSE (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	fseptr fep;
	BioseqPtr bsp;

	fep = (fseptr)data;
	if (fep->found)   /* already found it */
		return;

    if (! IS_Bioseq(sep))
        return;

	bsp = (BioseqPtr)(sep->data.ptrvalue);
	if (BioseqMatch(bsp, fep->sip))
	{
		fep->found = TRUE;
		fep->bsp = bsp;
		fep->indent = indent;
	}

	return;
}

/*****************************************************************************
*
*   BioseqFindInSeqEntry(sip, sep)
*   	Finds a Bioseq within a SeqEntry by SeqId
*
*****************************************************************************/
NLM_EXTERN BioseqPtr BioseqFindInSeqEntry(SeqIdPtr sip, SeqEntryPtr sep)
{
	BioseqPtr bsp = NULL;
	fse fe;

	if (sip == NULL) return bsp;
	if (sep == NULL) return bsp;

	fe.found = FALSE;
	fe.sip = sip;
	fe.bsp = NULL;

	SeqEntryExplore(sep, (Pointer)(&fe), FindSE);
	if (fe.found)
		return fe.bsp;
	else
		return bsp;
}

/*****************************************************************************
*
*   BioseqGetSeqDescr(bsp, type, curr)
*       returns pointer to the next SeqDescr of this type
*       type gives type of Seq-descr
*       if 0, gets them all
*       curr is NULL or previous node of this type found
*
*****************************************************************************/
NLM_EXTERN ValNodePtr BioseqGetSeqDescr (BioseqPtr bsp, Int2 type, ValNodePtr curr)    /* the last one you used */

{
	if (bsp == NULL) return NULL;
	
    if (curr == NULL)
            curr = bsp->descr;
    else
        curr = curr->next;     /* move past last one */

    while (curr != NULL)
    {
        if ((! type) || ((Int2)curr->choice == type))
            return curr;
        else
            curr = curr->next;
    }
    return NULL;
}

/*****************************************************************************
*
*   BioseqGetTitle(bsp)
*       returns pointer to the first title of this Bioseq
*
*****************************************************************************/
NLM_EXTERN CharPtr BioseqGetTitle (BioseqPtr bsp)

{
    ValNodePtr ptr;

    ptr = BioseqGetSeqDescr(bsp, Seq_descr_title, NULL);
    if (ptr != NULL)
        return (CharPtr)ptr->data.ptrvalue;
    else
        return NULL;
}

/*****************************************************************************
*
*   BioseqGetNumbering(bsp)
*   	Gets either user supplied, or default number for a Bioseq
*   	looks first for num Seqdescr, then in Pubdesc, then returns
*         default numbering
*
*****************************************************************************/
NLM_EXTERN NumberingPtr BioseqGetNumbering (BioseqPtr bsp)

{
	NumberingPtr np = NULL;
	ValNodePtr anp;
	PubdescPtr pdp;

	if (bsp == NULL)
		return NULL;

	anp = BioseqGetSeqDescr(bsp, Seq_descr_num, NULL);
	if (anp != NULL)    /* Numbering on this Bioseq */
		np = (NumberingPtr)anp->data.ptrvalue;
	else do				/* look for Pubdesc  */
	{
		anp = BioseqGetSeqDescr(bsp, Seq_descr_pub, anp);
		if (anp != NULL)
		{
			pdp = (PubdescPtr)anp->data.ptrvalue;
			np = pdp->num;
		}
	} while ((anp != NULL) && (np == NULL));

	if (np == NULL)   /* no numbering found */
		np = NumberingDefaultGet();   /* fallback position */

	return np;
}


/*****************************************************************************
*
*   Bioseq_repr (BioseqPtr bsp)
*
*****************************************************************************/
NLM_EXTERN Uint1 Bioseq_repr (BioseqPtr bsp)

{
    return bsp->repr;
}

/*****************************************************************************
*
*   Int4 BioseqGetLen (bsp)
*       returns total length of sequence in residues
*       if segmented:
*          includes length of virtual sequences with fixed length
*          does not include lengths of NULL gaps
*       returns -1 for error
*
*****************************************************************************/
NLM_EXTERN Int4 BioseqGetLen (BioseqPtr bsp)

{
    if (bsp == NULL)
        return -1;

    return bsp->length;
}

/*****************************************************************************
*
*   Int4 BioseqGetGaps (bsp)
*       returns total number of NULL gaps in sequence
*       virtual sequence with length set does not count as a gap
*       returns -1 for error
*
*****************************************************************************/
NLM_EXTERN Int4 BioseqGetGaps (BioseqPtr bsp)

{
    ValNodePtr anp;
    Int4 gaps = 0;
	Uint1 repr;

    if (bsp == NULL)
        return -1;

	repr = Bioseq_repr(bsp);

	switch (repr)
	{
		case  Seq_repr_seg:
		case Seq_repr_ref:
			anp = (ValNodePtr)bsp->seq_ext;
			 while (anp != NULL)    /* go through Seq-loc chain */
			{
				gaps = SeqLocGetSegLens((SeqLocPtr)anp, NULL, gaps, TRUE);
				anp = anp->next;
			}
			break;
		case Seq_repr_delta:
			anp = (ValNodePtr)bsp->seq_ext;
			 while (anp != NULL)    /* go through delta seq chain */
			{
				 if (anp->choice == 1)
					gaps = SeqLocGetSegLens((SeqLocPtr)(anp->data.ptrvalue), NULL, gaps, TRUE);
				anp = anp->next;
			}
			break;
		default:
			break;
	}

    return gaps;
}

/*****************************************************************************
*
*   Int4 BioseqGetSegLens (bsp, lens)
*       returns total number of segments in sequence including NULLS
*       returns -1 for error
*       if lens != NULL fills with lengths of segments, 0 = NULL
*
*****************************************************************************/
NLM_EXTERN Int4 BioseqGetSegLens (BioseqPtr bsp, Int4Ptr lens)

{
    ValNodePtr anp;
    Int4 segs = 0;
	Uint1 repr;
	SeqLitPtr slitp;

    if (bsp == NULL)
        return -1;

	repr = Bioseq_repr(bsp);

	switch (repr)
	{
		case  Seq_repr_seg:
		case Seq_repr_ref:
			anp = (ValNodePtr)bsp->seq_ext;
			 while (anp != NULL)    /* go through Seq-loc chain */
			{
				segs = SeqLocGetSegLens((SeqLocPtr)anp, lens, segs, FALSE);
				anp = anp->next;
			}
			break;
		case Seq_repr_delta:
			anp = (ValNodePtr)bsp->seq_ext;
			 while (anp != NULL)    /* go through delta seq chain */
			{
				 if (anp->choice == 1)
					segs = SeqLocGetSegLens((SeqLocPtr)(anp->data.ptrvalue), lens, segs, FALSE);
				 else
				 {
					 slitp = (SeqLitPtr)(anp->data.ptrvalue);
					 if (lens != NULL)
						 lens[segs] = slitp->length;
					 segs++;
				 }
				 anp = anp->next;
			}
			break;
		default:
			if (lens != NULL)
				lens[0] = BioseqGetLen(bsp);
			segs = 1;
			break;
	}
    return segs;
}

/*****************************************************************************
*
*   BioseqGetCode(bsp)
*       returns type of code for data in sequence
*       if not bioseq or not raw returns 0
*       otherwise returns #defines from objseq.h
*
*****************************************************************************/
NLM_EXTERN Uint1 BioseqGetCode (BioseqPtr bsp)

{
    if (bsp == NULL)
        return 0;

    if ((Bioseq_repr(bsp) == Seq_repr_raw) ||
		(Bioseq_repr(bsp) == Seq_repr_const))
	    return bsp->seq_data_type;
	else
		return 0;
}

/*****************************************************************************
*
*   Boolean BioseqConvert(bsp, newcode)
*      converts a raw or const bioseq or delta to a new sequence code
*
*****************************************************************************/
NLM_EXTERN Boolean BioseqConvert (BioseqPtr bsp, Uint1 newcode)

{
    ByteStorePtr to;
	ValNodePtr vnp;
	SeqLitPtr slp;

	if (bsp == NULL) return FALSE;
	
    if ((Bioseq_repr(bsp) == Seq_repr_raw) ||
		(Bioseq_repr(bsp) == Seq_repr_const))
        return BioseqRawConvert(bsp, newcode);

	if (Bioseq_repr(bsp) != Seq_repr_delta)
		return FALSE;

								/* go through the delta chain */
	for (vnp = (ValNodePtr)(bsp->seq_ext); vnp != NULL; vnp = vnp->next)
	{
	   if (vnp->choice == 2)   /* SeqLit */
	   {
		   slp = (SeqLitPtr)(vnp->data.ptrvalue);
		   if ((slp->length > 0) && (slp->seq_data != NULL))
		   {
				to = BSConvertSeq(slp->seq_data, newcode, slp->seq_data_type, slp->length);
			   if (to != NULL)
			   {
				   slp->seq_data = to;
				   slp->seq_data_type = newcode;
			   }
		   }
	   }
	}

	return TRUE;
}

/*****************************************************************************
*
*   Boolean BioseqRawPack(bsp)
*      converts a raw or const bioseq to it's densist possible code
*
*****************************************************************************/
NLM_EXTERN Boolean BioseqRawPack (BioseqPtr bsp)

{
  ByteStorePtr to;
  Uint1 newcode;
  
  if (bsp == NULL) return FALSE;
  
  if (! ((Bioseq_repr(bsp) == Seq_repr_raw) ||
         (Bioseq_repr(bsp) == Seq_repr_const)))
    return FALSE;

  if(! ISA_na(bsp->mol)) {    /* protein ? */
    if(!BioseqRawConvert (bsp, Seq_code_ncbieaa)) {
      return FALSE;
    }
  } else {
    if((to = BSPack(bsp->seq_data, 
                    BioseqGetCode(bsp), 
                    BioseqGetLen(bsp), 
                    &newcode)) == NULL) {
      return FALSE;
    }
    bsp->seq_data = to;
    bsp->seq_data_type = newcode;  
  }
  return TRUE;
}

/*****************************************************************************
*
*   Boolean BioseqRawConvert(bsp, newcode)
*      converts a raw or const bioseq to a new sequence code
*
*****************************************************************************/
NLM_EXTERN Boolean BioseqRawConvert (BioseqPtr bsp, Uint1 newcode)

{
    ByteStorePtr to;
    Int4 seqlen;
    Uint1 oldcode;

	if (bsp == NULL) return FALSE;
	
    if (! ((Bioseq_repr(bsp) == Seq_repr_raw) ||
		(Bioseq_repr(bsp) == Seq_repr_const)))
        return FALSE;

    oldcode = BioseqGetCode(bsp);
    if (! oldcode)   /* not a coded sequence */
        return FALSE;

    seqlen = BioseqGetLen(bsp);

    to = BSConvertSeq(bsp->seq_data, newcode, oldcode, seqlen);
    if (to == NULL)
        return FALSE;

    bsp->seq_data = to;
    bsp->seq_data_type = newcode;

    return TRUE;
}

/*****************************************************************************
*
*   Boolean BioseqPack(bsp)
*      converts a raw or const or delta bioseq to it's densist possible code
*
*****************************************************************************/
NLM_EXTERN Boolean BioseqPack (BioseqPtr bsp)

{
  ValNodePtr vnp;
  
  if (bsp == NULL) return FALSE;
  
  if ((Bioseq_repr(bsp) == Seq_repr_raw) ||
      (Bioseq_repr(bsp) == Seq_repr_const))
    return BioseqRawPack(bsp);
  
  if (Bioseq_repr(bsp) != Seq_repr_delta)
    return FALSE;
  
  /* go through the delta chain */
  
  for (vnp = (ValNodePtr)(bsp->seq_ext); vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 2)   /* SeqLit */
      SeqLitPack((SeqLitPtr)(vnp->data.ptrvalue));
  }
  return TRUE;
}

/****************************************************************************
*
*  Boolean SeqLitPack(slp)
*      Pack a SeqLit as dense as possible
*
*****************************************************************************/
NLM_EXTERN Boolean SeqLitPack (SeqLitPtr slp)
{
	ByteStorePtr to = NULL;
	Uint1 newcode = 0;

	if (slp == NULL) return FALSE;

	if ((slp->length == 0) || (slp->seq_data == NULL))
		return FALSE;

	to = BSPack(slp->seq_data, slp->seq_data_type, slp->length, &newcode);

	if (to != NULL)
	{
		slp->seq_data = to;
		slp->seq_data_type = newcode;
	}

	return TRUE;
}

/**************************************************************************
*
*  ByteStorePtr BSPack(from, oldcode, length, newcodeptr)
*
*     packs a bytestore containing a nucleic acid code as dense as possible
*     returns a new bytestoreptr and fills in newcodeptr if it can pack it
*     more. Otherwise returns null. length is number of residues.
*
*     if BSPack returns non-NULL, then it has already BSFree'd from.
*
***************************************************************************/
NLM_EXTERN ByteStorePtr BSPack (ByteStorePtr from, Uint1 oldcode, 
                     Int4 length, Uint1Ptr newcodeptr)
{
  Int4 i, seqlen;
  Uint1 newcode, byte;
  Char Code4na[256], CodeIna[256];
  Boolean remained;
  Int2 actual, j;
  Int4 cntr;
  Uint1 tmp [401];

  Uint1 set4na[16] = {17, 18, 20, 24,  33,  34,  36,  40,
                      65, 66, 68, 72, 129, 130, 132, 136}; 
  Uint1 setIna[4] = {65, 67, 71, 84};
  
  if ((! oldcode) || (! length) || (from == NULL))/* not a coded sequence */
    return NULL;
  
  if (oldcode == Seq_code_ncbi2na)   /* already packed */
    return NULL;

  MemSet ((Pointer) tmp, 0, sizeof (tmp));

  BSSeek(from, 0L, SEEK_SET);
  newcode = Seq_code_ncbi2na;    /* go for broke */

  switch (oldcode) {

  case Seq_code_ncbi4na:
    remained = length%2;
    seqlen = length/2;

    MemSet(Code4na, 1, sizeof(Code4na));
    for(i=0; i< 16; i++)
      Code4na[set4na[i]] = 0;

    cntr = (Int4) MIN ((Int4) seqlen, (Int4) (sizeof (tmp) - 1));
    actual = (Int2) BSRead (from, tmp, (Int4) cntr);
    j = 0;

    while(seqlen && actual > 0) {
      if (j == actual) {
        cntr = (Int4) MIN ((Int4) seqlen, (Int4) (sizeof (tmp) - 1));
        actual = (Int2) BSRead (from, tmp, (Int4) cntr);
        j = 0;
      }
      /* byte = (Uint1) BSGetByte(from); */
      byte = (Int2) (Uint1) tmp [j];
      j++;
      if(Code4na[byte]) {
        newcode = Seq_code_ncbi4na;
        if (newcodeptr != NULL) {
          *newcodeptr = newcode;
        }
        return BSConvertSeq(from, newcode, oldcode, length);
      }
      seqlen--;
    }
    if(remained) { /* one more uncompleted byte */
      byte = (Uint1) BSGetByte(from);    
      if(Code4na[byte+1])
        newcode = Seq_code_ncbi4na;
    }
    break;
  case Seq_code_iupacna:
    MemSet(CodeIna, 1, sizeof(CodeIna));
    for(i=0; i < 4; i++)
      CodeIna[setIna[i]] = 0;
    seqlen = length;

    cntr = (Int4) MIN ((Int4) seqlen, (Int4) (sizeof (tmp) - 1));
    actual = (Int2) BSRead (from, tmp, (Int4) cntr);
    j = 0;

    while(seqlen && actual > 0) {
      if (j == actual) {
        cntr = (Int4) MIN ((Int4) seqlen, (Int4) (sizeof (tmp) - 1));
        actual = (Int2) BSRead (from, tmp, (Int4) cntr);
        j = 0;
      }
      /* byte = (Uint1) BSGetByte(from); */
      byte = (Int2) (Uint1) tmp [j];
      j++;
      if(CodeIna[byte]) {
        newcode = Seq_code_ncbi4na;
        break;
      }
      seqlen--;
    }
    break;
  default:
    break;
  }
    if (newcodeptr != NULL) {
      *newcodeptr = newcode;
    }
    return BSConvertSeq(from, newcode, oldcode, length);
}


/*****************************************************************************
*
*   BSConvertSeq(bytestoreptr, newcode, oldcode, len)
*       converts a bytestore to a new sequence representation
*       frees old bytestore
*       returns pointer to new one, or NULL on fail.
*       len is residues
*
*****************************************************************************/

NLM_EXTERN ByteStorePtr BSConvertSeq (ByteStorePtr from, Uint1 newcode, 
                           Uint1 oldcode, Int4 len)

{
  ByteStorePtr to;
  Uint1 byte_from, residue_from, bitctr_from, mask_from;
  Uint1 lshift_from, rshift_from, bc_from, byte_to, bitctr_to;
  Uint1 lshift_to[5], bc_to, byte_tmp;
  SeqMapTablePtr smtp;
  Int4 storelen, in_index = 0, out_index = 0;
  Uint1Ptr out_buff, in_buff;

  if ((from == NULL) || (! oldcode) || (! newcode) || (len <= 0))
    return NULL;
  
  if (oldcode == newcode)
    return from;
  
  if ((smtp = SeqMapTableFind(newcode, oldcode)) == NULL)
    return NULL;

  if (newcode == Seq_code_ncbi2na)
    storelen = (len / 4) + 1;
  else if (newcode == Seq_code_ncbi4na)
    storelen = (len / 2) + 1;
  else
    storelen = len;

  if((to = BSNew((Uint4)storelen)) == NULL)
    return NULL;

  BSSeek(from, 0, 0);
  BSSeek(to, 0, 0);

  in_buff  = (Uint1Ptr)MemNew(BSC_BUFF_CHUNK);
  out_buff = (Uint1Ptr)MemNew(BSC_BUFF_CHUNK);

  switch (oldcode) {

  case Seq_code_ncbi2na:
    bc_from = 4;            /* bit shifts needed */
    rshift_from = 6;
    lshift_from = 2;
    mask_from = 192;
    break;

  case Seq_code_ncbi4na:
    bc_from = 2;
    rshift_from = 4;
    lshift_from = 4;
    mask_from = 240;
    break;

  default:
    bc_from = 1;
    rshift_from = 0;
    lshift_from = 0;
    mask_from = 255;
    break;
  }

  lshift_to[1] = 0;

  switch (newcode) {

  case Seq_code_ncbi2na:
    bc_to = 4;            /* bit shifts needed */
    lshift_to[2] = 2;
    lshift_to[3] = 4;
    lshift_to[4] = 6;
    break;

  case Seq_code_ncbi4na:
    bc_to = 2;
    lshift_to[2] = 4;
    break;

  default:
    bc_to = 1;
    break;
  }

  bitctr_to = bc_to;
  byte_to = 0;
  bitctr_from = 0;
  
  in_index = BSC_BUFF_CHUNK;

  while (len) {
    if (in_index == BSC_BUFF_CHUNK) {
      in_index = (Int2) BSRead(from, (VoidPtr)in_buff, (Int4)BSC_BUFF_CHUNK);
      in_index = 0;
    }
      
    if (! bitctr_from) {       /* need a new byte */        
      byte_from = in_buff[in_index];
      in_index++;
      bitctr_from = bc_from;
    }

    residue_from = byte_from & mask_from;
    residue_from >>= rshift_from;
    byte_from <<= lshift_from;
    bitctr_from--;
    
    byte_tmp = SeqMapTableConvert(smtp, residue_from);
    
    if (byte_tmp == INVALID_RESIDUE) {
      ErrPostEx(SEV_ERROR, 0, 0, "BSConvertSeq: invalid residue [%d=%c]",
                (int)residue_from, (char)residue_from);
      BSFree(to);
      MemFree(in_buff);
      MemFree(out_buff);
      return NULL;
    }
    
    byte_tmp <<= lshift_to[bitctr_to];
    byte_to |= byte_tmp;
    bitctr_to--;
    
    if (! bitctr_to) {
      if (out_index == BSC_BUFF_CHUNK) {
        
        /* Flush buffer if it is full */
        
        out_index = (Int2) BSWrite(to, (VoidPtr)out_buff, out_index);
        out_index = 0;
      }
      out_buff[out_index] = byte_to;
      out_index++;

      bitctr_to = bc_to;
      byte_to = 0;
    }      
    len--;
  }

  /* Now we will BSWrite() all recorded bytes in buffer */

  out_index = (Int2) BSWrite(to, (VoidPtr)out_buff, out_index);

 /* And finaly partial byte not written */

  if (bitctr_to != bc_to)     
    BSPutByte(to, byte_to);
    
  BSFree(from);
  MemFree(in_buff);
  MemFree(out_buff);

  return to;
}

/*****************************************************************************
*
*   BSRebuildDNA(bytestoreptr, len, lbytes)
*       restore ASCII sequence with abmiguity characters
*       lbytes[0] == length of this storage
*       frees old bytestore
*       returns pointer to new one, or NULL on fail.
*       len is residues
*       lbytes is pointer to ambiguity storage
*
*****************************************************************************/
NLM_EXTERN ByteStorePtr BSRebuildDNA (ByteStorePtr from, Int4 len, 
                           Uint4Ptr PNTR lbytes)

{
  Int4      i, am_num;
  Uint4Ptr  am_buff;
  Uint1     char_to;
  Int4     row_len, j;
  SeqMapTablePtr smtp;

  if(from == NULL || len <=0)
    return NULL;
  
  if(*lbytes == NULL)
    return from;

  if ((smtp = SeqMapTableFind(Seq_code_iupacna, 
                              Seq_code_ncbi4na)) == NULL)
    return NULL;
  
  am_num  = **lbytes;
  am_buff = *lbytes + 1;
  
  for(i = 0; i < am_num; i++) {
    char_to = (Uint1)RES_VALUE(am_buff[i]);
    row_len = (Int4)RES_LEN(am_buff[i]); 
    
    BSSeek(from, RES_OFFSET(am_buff[i]), SEEK_SET);
    for(j = 0; j <= row_len; j++)  
      BSPutByte(from, SeqMapTableConvert(smtp, char_to));    
  }
  return from;
}
/*****************************************************************************
*
*   RebuildDNA_4na(buffer, length, lbytes)
	works with Uint1 buffer, not ByteStore.
*       restore ncbi4na sequence with abmiguity characters
*       returns TRUE on success, FALSE on failure.
*       lbytes is pointer to ambiguity storage
*
*****************************************************************************/
NLM_EXTERN Boolean RebuildDNA_4na (Uint1Ptr buffer, Int4 length, Uint4Ptr lbytes)
     
{
    Boolean	new = FALSE;
    Int4        i;
    Uint4 	amb_num;
    Uint4Ptr  amb_buff;
    Uint1     char_l, char_r;
    Int4      row_len;
    Uint1     C_Mask[] = {0x0F, 0xF0};
    Int4      j, position = 0, pos =0 , rem =0 , index;
    
    if(buffer == NULL || length == 0)
        return FALSE;
    
    if(lbytes == NULL) 
        return TRUE;
    
    amb_num  = *lbytes;
    amb_buff = lbytes + 1;

    /* Check if highest order bit set. */
    if (amb_num & 0x80000000)
    {
	new = TRUE;
	amb_num &= 0x7FFFFFFF;
    }
    
    for(i = 0; i < amb_num; i++) {

	if (new)
	{
       		char_r    = (Uint1)(RES_VALUE(amb_buff[i]));
       		row_len   = (Int4)(RES_LEN_NEW(amb_buff[i])); 
        	position  =         amb_buff[i+1];
	}
	else
	{
       		char_r    = (Uint1)(RES_VALUE(amb_buff[i]));
       		row_len   = (Int4)(RES_LEN(amb_buff[i])); 
        	position  =         RES_OFFSET(amb_buff[i]);
	}
        
        pos = position/2;
        rem = position%2;  /* 0 or 1 */
        char_l = char_r << 4;
        
        for(index = pos, j =0; j <=row_len; j++) {
           
           	buffer[index] = (buffer[index] & C_Mask[rem]) + (rem ? char_r : char_l);
            	rem = !rem;
            
            	if(!rem) index++;
    	}

	if (new) /* for new format we have 8 bytes for each element. */
		i++;
    }
    
    return TRUE;
}
/*****************************************************************************
*
*   BSRebuildDNA_4na(bytestoreptr, lbytes)
*       restore ncbi4na sequence with abmiguity characters
*       lbytes[0] == length of this storage
*       frees old bytestore
*       returns pointer to new one, or NULL on fail.
*       lbytes is pointer to ambiguity storage
*
*****************************************************************************/
NLM_EXTERN ByteStorePtr BSRebuildDNA_4na (ByteStorePtr from, Uint4Ptr lbytes)
     
{
    Int4      bs_length;
    Uint1Ptr  buffer;
    Int4      position = 0, pos =0 , rem =0 , num_bytes;
    
    if(from == NULL)
        return NULL;
    
    if(lbytes == NULL)
        return from;
    
    bs_length = BSLen(from);
    buffer = (Uint1Ptr) Nlm_Malloc(bs_length);
    if (buffer == NULL)
        return NULL;

    BSSeek(from, 0, SEEK_SET);

    if((num_bytes = BSRead(from, buffer, bs_length)) != bs_length)
        return NULL;
    
    if (RebuildDNA_4na(buffer, bs_length, lbytes) == FALSE)
	return NULL;
    
    BSSeek(from, 0, SEEK_SET);
    BSWrite(from, buffer, bs_length);
    
    MemFree(buffer);
    return from;
}

/*****************************************************************************
*
*   Int4 BSCompressRead (Pointer data, Uint1Ptr buf, Int4 length)
*     Hook function to read "length" bytes from "data" into "buf"
*
*     NOTE!! This function must return number or residues, but returns   
*            twice number of returned bytes.
*            This function may be used ONLY if we know how many residues
*            in the sequence and pass this value to GenericCompressDNA()
*
*****************************************************************************/
static Int4 BSCompressRead (Pointer data, Uint1Ptr buf, Int4 length);
static Int4 BSCompressRead (Pointer data, Uint1Ptr buf, Int4 length)
{
  Int4 residues;
  
  residues = (Int4) BSRead((ByteStorePtr)data, (VoidPtr)buf, length);
  return residues*2;
}

/*****************************************************************************
*
*   Int4 BSCompressWrite (Pointer data, Uint1Ptr buf, Int4 length)
*     Hook function to write "length" bytes to "data" from "buf"
*
*     Returned number of bytes were written
*****************************************************************************/
static Int4 BSCompressWrite (Pointer data, Uint1Ptr buf, Int4 length);
static Int4 BSCompressWrite (Pointer data, Uint1Ptr buf, Int4 length)
{
  return (Int4) BSWrite((ByteStorePtr)data, (VoidPtr)buf, length);
}

/*****************************************************************************
*
*   BSCompressDNA(bytestoreptr, len, lbytes)
*       converts a ncbi4na bytestore into ncbi2na
*       returns pointer to ambiguity storage
*       lbytes[0] == length of this storage
*       frees old bytestore
*       returns pointer to new one, or NULL on fail.
*       len is residues
*
*****************************************************************************/
NLM_EXTERN ByteStorePtr BSCompressDNA(ByteStorePtr from, Int4 len, 
                              Uint4Ptr PNTR lbytes)
{
  ByteStorePtr to;
  to = BSNew((Uint4)len/4+1);

  BSSeek(from, 0, 0);
  BSSeek(to, 0, 0);
  
  if(!GenericCompressDNA((VoidPtr) from, (VoidPtr) to, 
                         (Uint4)len,
                         BSCompressRead, 
                         BSCompressWrite, 
                         lbytes
                         )) {
    return NULL;
  }
  
  BSFree(from);
  return to;
}

/*****************************************************************************
*
*   BSCompressDNANew(bytestoreptr, len, lbytes)
*       converts a ncbi4na bytestore into ncbi2na
*       returns pointer to ambiguity storage
*       lbytes[0] == length of this storage
*       frees old bytestore
*       returns pointer to new one, or NULL on fail.
*       len is residues
*
*	This function stores the ambiguity code in 8 bytes so
*	that there is no cutoff for sequences greater than 16 million bps.
*	as there is for BSCompressDNA.
*
*****************************************************************************/
NLM_EXTERN ByteStorePtr BSCompressDNANew(ByteStorePtr from, Int4 len, 
                              Uint4Ptr PNTR lbytes)
{
  ByteStorePtr to;
  to = BSNew((Uint4)len/4+1);

  BSSeek(from, 0, 0);
  BSSeek(to, 0, 0);
  
  if(!GenericCompressDNAEx((VoidPtr) from, (VoidPtr) to, 
                         (Uint4)len,
                         BSCompressRead, 
                         BSCompressWrite, 
                         lbytes, TRUE)) {
    return NULL;
  }
  
  BSFree(from);
  return to;
}

/*****************************************************************************
*
*   GenericCompressDNA()
*       converts from VoidPtr "from" in 4na encoding to 
*       VoidPtr "to" in 2Na encoding
*       returns pointer to ambiguity storage
*       lbytes[0] == length of this storage
*       returns TRUE if succeded, or FALSE on fail.
*       seq_len is maximum number of residues in sequence 
*       or ((Uint4) -1) if final length is unknown.
*       read_func and write_func - hook functions to read from "from"
*       and to write to "to"
*
*       NOTE! read_func must return number of residues read, that usualy
*             twice as much as returned number of bytes. Only last returned
*             byte may have only one residue and this will be handled by
*             seq_len value or returned value from read_func()    
*****************************************************************************/
NLM_EXTERN Boolean GenericCompressDNA(VoidPtr from, 
                           VoidPtr to,
                           Uint4 seq_len,
                           CompressRWFunc read_func, 
                           CompressRWFunc write_func,
                           Uint4Ptr PNTR lbytes)
{
	return GenericCompressDNAEx(from, to, seq_len, read_func, write_func, lbytes, FALSE);
}

NLM_EXTERN Boolean GenericCompressDNAEx(VoidPtr from, 
                           VoidPtr to,
                           Uint4 seq_len,
                           CompressRWFunc read_func, 
                           CompressRWFunc write_func,
                           Uint4Ptr PNTR lbytes,
                           Boolean x_new)
{
  Int4 total_read, chunk_used, seq_offset;
  Int4 in_index = 0, out_index = 0;
  Uint1Ptr out_buff, in_buff;
  Uint1 bc_from, rshift_from, lshift_from, mask_from;
  Uint1 bc_to, byte_tmp;
  Uint1 bitctr_to, byte_to, byte_from, bitctr_from, residue_from;
  Uint1 lshift_to[5] = {0, 0, 2, 4, 6 };
  
  Int4     row_len =0;
  Uint1    last_ambchar = INVALID_RESIDUE;
  Uint4Ptr ambchar;
  Int4     ambsize = 2*(BSC_BUFF_CHUNK/2); /* we need this to be a multiple of two for the new format. */
  
  if(from == NULL) /* Invalid ByteStore format */
    return FALSE;
  
  /* Translation tables Initialization  fot ncbi4na->ncbi2na*/
  
  in_buff  = (Uint1Ptr)MemNew(BSC_BUFF_CHUNK);
  out_buff = (Uint1Ptr)MemNew(BSC_BUFF_CHUNK);

  bc_from = 2;
  rshift_from = 4;
  lshift_from = 4;
  mask_from = 240;
  bc_to = 4;            /* bit shifts needed */

  bitctr_to = bc_to;
  byte_to = 0;
  bitctr_from = 0;

  ambchar = (Uint4Ptr) Nlm_Malloc(sizeof(Uint4)*(ambsize + 1)); /* all plus one */
  *ambchar = 0;
  
  seq_offset = chunk_used = in_index = total_read = 0;
  
  while(seq_offset != seq_len) {
    if (chunk_used == total_read) { 
      /* supposed, that in 4na total_read = in_index*2 or in_index*2-1 */
      if((total_read = read_func(from, in_buff, (Int4)BSC_BUFF_CHUNK)) == 0)
        break;
      if(total_read < 0) { /* ERROR!!! */
        MemFree(ambchar);
        MemFree(in_buff);
        MemFree(out_buff);
        return FALSE;
      }
      in_index = 0;
      chunk_used = 0;
    }
    
    if (!bitctr_from) {        /* need a new byte */
      byte_from = in_buff[in_index];
      bitctr_from = bc_from;
      in_index++;
    }
    residue_from = byte_from & mask_from;
    residue_from >>= rshift_from;
    byte_from <<= lshift_from;
    bitctr_from--;
    if(!Convert4NaRandom(residue_from, &byte_tmp)) {
      
      /* We have to handle invalid residues in a good way */
      
      if(*ambchar >= (Uint4)(ambsize-1)) { /* Reallocating buffer if necessary */
        ambsize += 2*(BSC_BUFF_CHUNK/2); /* we need this to be a multiple of two for the new format. */
        ambchar = (Uint4Ptr) Realloc(ambchar, (ambsize+1)*sizeof(Uint4));
      }
      
      /* Constructing integer as <1111.  1111.  11111111.11111111.11111111  
       *                         <char><length><--------- offset -------->
       * First interer in array will be length of array 
       */
      
      if (x_new && seq_len >= 0xFFFFFF)
      {
      	if(last_ambchar != residue_from || row_len == 0xFFF) {
	 if ((*ambchar) == 0)
       	 	(*ambchar)++;
	 else
	  	(*ambchar) += 2;
       	 ambchar[*ambchar] = 0;
       	 ambchar[*ambchar] += residue_from;
       	 ambchar[*ambchar] <<= 28;
	/* Put the seq_offset in the 2nd integer. */
       	 ambchar[(*ambchar)+1] = seq_offset;

       	 last_ambchar = residue_from;
       	 row_len = 0;
       	 /*  printf("Ambchar = %u(%u)(%u) : %u %u %u\n", 
            residue_from, row_len, total_len-len, 
            RES_VALUE(ambchar[*ambchar]),
            RES_LEN(ambchar[*ambchar]), 
            RES_OFFSET(ambchar[*ambchar])); */  
      	} else {
       	 (ambchar[*ambchar]) += LEN_STEP_MASK_NEW;
       	 row_len++;         
        /* printf("Ambchar = %u(%u)(%u) : %u %u %u\n", 
           residue_from, row_len, total_len-len, 
           RES_VALUE(ambchar[*ambchar]),
           RES_LEN(ambchar[*ambchar]), 
           RES_OFFSET(ambchar[*ambchar]));  */  
      	}
      }
      else
      {
      	if(last_ambchar != residue_from || row_len == 15) {
       	 (*ambchar)++;
       	 ambchar[*ambchar] = 0;
       	 ambchar[*ambchar] += residue_from;
       	 ambchar[*ambchar] <<= 28;
       	 ambchar[*ambchar] += seq_offset;

       	 last_ambchar = residue_from;
       	 row_len = 0;
       	 /*  printf("Ambchar = %u(%u)(%u) : %u %u %u\n", 
            residue_from, row_len, total_len-len, 
            RES_VALUE(ambchar[*ambchar]),
            RES_LEN(ambchar[*ambchar]), 
            RES_OFFSET(ambchar[*ambchar])); */  
      	} else {
       	 (ambchar[*ambchar]) += LEN_STEP_MASK;
       	 row_len++;         
        /* printf("Ambchar = %u(%u)(%u) : %u %u %u\n", 
           residue_from, row_len, total_len-len, 
           RES_VALUE(ambchar[*ambchar]),
           RES_LEN(ambchar[*ambchar]), 
           RES_OFFSET(ambchar[*ambchar]));  */  
      	}
       }
    } else {
      	last_ambchar = INVALID_RESIDUE; /* reset of last residue */
    }
    byte_tmp <<= lshift_to[bitctr_to];
    byte_to |= byte_tmp;
    bitctr_to--;
    if (! bitctr_to) {
      if (out_index == BSC_BUFF_CHUNK) {
        
        /* Flush buffer if it is full */
        
        out_index = write_func(to, out_buff, out_index);
        out_index = 0;
      }
      
      out_buff[out_index] = byte_to;
      out_index++;
      
      bitctr_to = bc_to;
      byte_to = 0;
    }
    chunk_used++;
    seq_offset++;
  } /* while TRUE */

  /* Now we will BSWrite() all recorded bytes in buffer */
  
  out_index = write_func(to, out_buff, out_index);
  
  if (bitctr_to != bc_to) {   /* partial byte not written */
    byte_to += (seq_len)%4;   /* last 2 bits will be remainder */
    write_func(to, &byte_to, 1);
  } else {
    write_func(to, &byte_to, 1);  /* NULLB anyway */
  }

  if(!*ambchar) { /* no ambiguous characters found */
    MemFree(ambchar);
    *lbytes = NULL;
  } else {
    if (x_new && seq_len >= 0xFFFFFF) 
    {
       	(*ambchar)++;
	*ambchar += 0x80000000;
    }
    *lbytes = (Uint4Ptr)ambchar;
  }
  MemFree(in_buff);
  MemFree(out_buff);
  return TRUE;
}

/*****************************************************************************
*                 --- To be deleted ---
*   BSCompressDNA(bytestoreptr, len, lbytes)
*       converts a ncbi4na bytestore into ncbi2na
*       returns pointer to ambiguity storage
*       lbytes[0] == length of this storage
*       frees old bytestore
*       returns pointer to new one, or NULL on fail.
*       len is residues
*
*****************************************************************************/
NLM_EXTERN ByteStorePtr BSCompressDNAOld(ByteStorePtr from, Int4 len, 
                              Uint4Ptr PNTR lbytes)
{
  ByteStorePtr to;
  Int4 total_len = len;
  Int4 tlen = BSC_BUFF_CHUNK;
  Int4 storelen = len/4 + 1, in_index = 0, out_index = 0;
  Uint1Ptr out_buff, in_buff;
  Uint1 bc_from, rshift_from, lshift_from, mask_from;
  Uint1 bc_to, byte_tmp;
  Uint1 bitctr_to, byte_to, byte_from, bitctr_from, residue_from;
  Uint1 lshift_to[5] = {0, 0, 2, 4, 6 };
  
  Uint1    row_len =0, last_ambchar = INVALID_RESIDUE;
  Uint4Ptr ambchar;
  Int4     ambsize = BSC_BUFF_CHUNK;
  
  if(from == NULL) /* Invalid ByteStore format */
    return NULL;
  
  /* Translation tables Initialization  fot ncbi4na->ncbi2na*/
  
  if((to = BSNew((Uint4)storelen)) == NULL)
    return NULL;
  
  BSSeek(from, 0, 0);
  BSSeek(to, 0, 0);

  in_buff  = (Uint1Ptr)MemNew(BSC_BUFF_CHUNK);
  out_buff = (Uint1Ptr)MemNew(BSC_BUFF_CHUNK);

  bc_from = 2;
  rshift_from = 4;
  lshift_from = 4;
  mask_from = 240;
  bc_to = 4;            /* bit shifts needed */

  bitctr_to = bc_to;
  byte_to = 0;
  bitctr_from = 0;

  ambchar = (Uint4Ptr) MemNew(sizeof(Uint4)*(ambsize + 1)); /* all plus one */
  *ambchar = 0;

  in_index = BSC_BUFF_CHUNK;
  
  while(len) {
    if (in_index == BSC_BUFF_CHUNK) {
      in_index = (Int2) BSRead(from, (VoidPtr)in_buff, (Int4)BSC_BUFF_CHUNK);
      in_index = 0;
    }

    if (! bitctr_from) {        /* need a new byte */
      byte_from = in_buff[in_index];
      in_index++;
      bitctr_from = bc_from;
    }
    residue_from = byte_from & mask_from;
    residue_from >>= rshift_from;
    byte_from <<= lshift_from;
    bitctr_from--;
    if(!Convert4NaRandom(residue_from, &byte_tmp)) {
      
      /* We have to handle invalid residues in a good way */
      
      if(*ambchar >= (Uint4)ambsize) { /* Reallocating buffer if necessary */
        ambsize += BSC_BUFF_CHUNK; 
        ambchar = (Uint4Ptr) Realloc(ambchar, (ambsize+1)*sizeof(Uint4));
      }
      
      /* Constructing integer as <1111.  1111.  11111111.11111111.11111111  
       *                         <char><length><--------- offset -------->
       * First interer in array will be length of array 
       */
      
      if(last_ambchar != residue_from || row_len == 15) {
        (*ambchar)++;
        ambchar[*ambchar] = 0;
        ambchar[*ambchar] += residue_from;
        ambchar[*ambchar] <<= 28;
        ambchar[*ambchar] += (total_len-len);

        last_ambchar = residue_from;
        row_len = 0;
        /*  printf("Ambchar = %u(%u)(%u) : %u %u %u\n", 
            residue_from, row_len, total_len-len, 
            RES_VALUE(ambchar[*ambchar]),
            RES_LEN(ambchar[*ambchar]), 
            RES_OFFSET(ambchar[*ambchar])); */  
      } else {
        (ambchar[*ambchar]) += LEN_STEP_MASK;
        row_len++;         
        /* printf("Ambchar = %u(%u)(%u) : %u %u %u\n", 
           residue_from, row_len, total_len-len, 
           RES_VALUE(ambchar[*ambchar]),
           RES_LEN(ambchar[*ambchar]), 
           RES_OFFSET(ambchar[*ambchar]));  */  
      }
    } else {
      last_ambchar = INVALID_RESIDUE; /* reset of last residue */
    }
    byte_tmp <<= lshift_to[bitctr_to];
    byte_to |= byte_tmp;
    bitctr_to--;
    if (! bitctr_to) {
      if (out_index == BSC_BUFF_CHUNK) {
        
        /* Flush buffer if it is full */
        
        out_index = (Int2) BSWrite(to, (VoidPtr)out_buff, out_index);
        out_index = 0;
      }

      out_buff[out_index] = byte_to;
      out_index++;
      
      bitctr_to = bc_to;
      byte_to = 0;
    }
    len--;
  }

  /* Now we will BSWrite() all recorded bytes in buffer */
  
  out_index = (Int2) BSWrite(to, (VoidPtr)out_buff, out_index);
  
  if (bitctr_to != bc_to) {   /* partial byte not written */
    byte_to += total_len%4;   /* last 2 bits will be remainder */
    BSPutByte(to, byte_to);
  } else {
    BSPutByte(to, byte_to);   /* NULLB anyway */
  }
  BSFree(from);

  if(!*ambchar) { /* no ambiguous characters found */
    MemFree(ambchar);
    *lbytes = NULL;
  } else {
    *lbytes = (Uint4Ptr)ambchar;
  }
  MemFree(in_buff);
  MemFree(out_buff);
  return to;
}

/*****************************************************************************
 *
 *   void CorrectGeneFeatLocation(sep, data, n, m)
 *
 *	Correct gene location for mRNA sequences, i.e.
 *   puts start = 0, end = total_length_of_sequence - 1.
 *
 *****************************************************************************/
NLM_EXTERN void CorrectGeneFeatLocation(SeqEntryPtr sep, Pointer data, 
                             Int4 n, Int2 m)
{
    BioseqPtr	bsp;
    ValNodePtr	vnp;
    MolInfoPtr	mip;
    SeqAnnotPtr	sap;
    SeqFeatPtr	sfp;
    SeqIntPtr	sip;
    
    if(sep == NULL)
        return;
    
    /* We need only Bioseqs
     */
    if(IS_Bioseq(sep) != TRUE)
        return;
    
    bsp = sep->data.ptrvalue;
    if(bsp == NULL)
        return;
    
    /* Looks at nucleic acids with the non-zero length only
     */
    if(ISA_na(bsp->mol) != TRUE || bsp->length == 0)
        return;
    
    /* Checks bioseq if it is mRNA
     */
    for(vnp = bsp->descr; vnp != NULL; vnp = vnp->next) {
        if(vnp->choice != Seq_descr_molinfo)
            continue;
        mip = vnp->data.ptrvalue;
        if(mip == NULL || mip->biomol != 3)	/* not mRNA */
            continue;
        break;
    }
    
    /* If bioseq is not mRNA, does nothing, just return
     */
    if(vnp == NULL)
        return;
    
    /* Otherwise go ahead
     */
    for(sap = bsp->annot; sap != NULL; sap = sap->next) {
        if(sap->type != 1)
            continue;
        
        for(sfp = sap->data; sfp != NULL; sfp = sfp->next) {
            /* Is it gene feature ?
             */
            if(sfp->data.choice != SEQFEAT_GENE)
                continue;
            
            /* If so, is it not empty ?
             */
            if(sfp->data.value.ptrvalue == NULL)
                continue;
            
            /* Then correct location
             */
            for(vnp = sfp->location; vnp != NULL; vnp = vnp->next) {
                if(vnp->choice != SEQLOC_INT)
                    continue;
                sip = vnp->data.ptrvalue;
                if(sip == NULL)
                    continue;
                if(sip->from != 0 || sip->to != bsp->length - 1) {
                    ErrPostEx(SEV_WARNING, 0, 0, 
                              "Incorrect gene location: [%d..%d] "
                              "instead of [0..%d]. Fixed.", 
                              sip->from, sip->to, bsp->length - 1);
                    sip->from = 0;
                    sip->to = bsp->length - 1;
                }
            }
        }
    }
}

/*****************************************************************************
*
*   Int4 NumberingOffset(np, value)
*       returns an offset to the sequence based on value
*       returns -1 if invalid
*       does NOT deal with Num-ref types
*       does NOT deal with specified ranges on the sequence
*
*****************************************************************************/
NLM_EXTERN Int4 NumberingOffset (NumberingPtr np, DataValPtr vp)

{
    Int4 offset = -1, i, num;
    NumContPtr ncp;
    NumEnumPtr nep;
    NumRealPtr nrp;
    CharPtr PNTR ptr;
    CharPtr name;
    FloatHi foffset;

	if ((np == NULL) || (vp == NULL)) return -1;
	
    switch (np->choice)
    {
        case Numbering_cont:
            ncp = (NumContPtr)np->data.ptrvalue;
            if (ncp->ascending)
            {
                offset = vp->intvalue - ncp->refnum;
                if ((ncp->refnum  < 0) && (! ncp->has_zero) &&
                    (vp->intvalue > 0))
                    offset--;
            }
            else
            {
                offset = ncp->refnum - vp->intvalue;
                if ((ncp->refnum > 0) && (! ncp->has_zero) &&
                    (vp->intvalue < 0))
                    offset--;
            }
            break;
        case Numbering_enum:
            nep = (NumEnumPtr)np->data.ptrvalue;
            name = (CharPtr)vp->ptrvalue;
            num = nep->num;
            ptr = nep->names;
            for (i = 0; i < num; i++, ptr++)
            {
                if (! StringCmp(name, *ptr))
                {
                    offset = i;
                    break;
                }
            }
            break;
        case Numbering_ref_source:
		case Numbering_ref_align:
            ErrPostEx(SEV_ERROR, 0,0, "Num-ref not supported yet");
            break;
        case Numbering_real:
            nrp = (NumRealPtr)np->data.ptrvalue;
            foffset = (vp->realvalue - nrp->b) / nrp->a;
            offset = (Int4) foffset;
            if ((foffset - (FloatHi)offset) >= 0.5)
                offset++;
            break;
    }
    return offset;
}

/*****************************************************************************
*
*   NumberingValue (np, offset, value)
*       fills value with the display value of offset
*       return type indicates type of value
*       0 = failed
*       1 = intvalue
*       2 = realvalue
*       3 = ptrvalue (string)
*
*****************************************************************************/
NLM_EXTERN Int2 NumberingValue (NumberingPtr np, Int4 offset, DataValPtr vp)

{
    NumContPtr ncp;
    NumEnumPtr nep;
    NumRealPtr nrp;
    Int2 type = 0;
    Int4 intval;
    FloatHi fval;

	if ((np == NULL) || (vp == NULL)) return -1;
	
    switch (np->choice)
    {
        case Numbering_cont:
            ncp = (NumContPtr)np->data.ptrvalue;
            if (ncp->ascending)
            {
                intval = offset + ncp->refnum;
                if ((ncp->refnum  < 0) && (! ncp->has_zero) &&
                    (intval >= 0))
                    intval++;
            }
            else
            {
                intval = ncp->refnum - offset;
                if ((ncp->refnum > 0) && (! ncp->has_zero) &&
                    (intval <= 0))
                    intval--;
            }
            vp->intvalue = intval;
            type = 1;
            break;
        case Numbering_enum:
            nep = (NumEnumPtr)np->data.ptrvalue;
            if (offset < nep->num)
            {
                vp->ptrvalue = nep->names[offset];
                type = 3;
            }
            break;
        case Numbering_ref_source:
		case Numbering_ref_align:
            ErrPostEx(SEV_ERROR, 0,0, "Num-ref not supported yet");
            break;
        case Numbering_real:
            nrp = (NumRealPtr)np->data.ptrvalue;
            fval = ((FloatHi)offset * nrp->a) + nrp->b;
            type = 2;
            vp->realvalue = fval;
            break;
    }

    return type;
}

/*****************************************************************************
*
*   NumberingValueBySeqId(sip, offset, vp)
*
*****************************************************************************/
NLM_EXTERN Int2 NumberingValueBySeqId (SeqIdPtr sip, Int4 offset, DataValPtr vp)

{
	BioseqPtr bsp;
	NumberingPtr np = NULL;

	if ((sip == NULL) || (vp == NULL)) return -1;
	
	bsp = BioseqFind(sip);
	if (bsp == NULL)
		np = NumberingDefaultGet();
	else
		np = BioseqGetNumbering(bsp);

	return NumberingValue(np, offset, vp);
}

/*****************************************************************************
*
*   NumberingDefaultLoad()
*
*****************************************************************************/
NLM_EXTERN void NumberingDefaultLoad (void)

{
    NumContPtr ncp;

    if (stdnum != NULL)
        return;

    stdnum = ValNodeNew(NULL);   /* set up numbering from 1 */
    stdnum->choice = Numbering_cont;
    ncp = NumContNew();
    ncp->refnum = 1;   /* number from one */
    ncp->ascending = TRUE;
    stdnum->data.ptrvalue = (Pointer) ncp;
    return;
}

/*****************************************************************************
*
*   NumberingDefaultGet()
*       returns a default numbering object (start at 1, ascending, no 0)
*
*****************************************************************************/
NLM_EXTERN NumberingPtr NumberingDefaultGet (void)

{
    if (stdnum == NULL)
        NumberingDefaultLoad();
    return stdnum;
}

/*****************************************************************************
*
*   SeqCodeTablePtr SeqCodeTableFind(code)
*   	Sequence codes defined in objseq.h
*
*****************************************************************************/
NLM_EXTERN SeqCodeTablePtr LIBCALL SeqCodeTableFind (Uint1 code)
{
	return SeqCodeTableFindObj (code);
}

/*****************************************************************************
*
*   OneLetterCode(sctp)
*   	returns TRUE if sequence code table sctp uses one letter symbols
*
*****************************************************************************/
NLM_EXTERN Boolean OneLetterCode (SeqCodeTablePtr sctp)
{
	if (sctp == NULL) return FALSE;
	return sctp->one_letter;
}

/*****************************************************************************
*
*   FirstResidueInCode(sctp)
*   	returns first valid residue code in sequence code table
*
*****************************************************************************/
NLM_EXTERN Uint1 FirstResidueInCode (SeqCodeTablePtr sctp)
{
	if (sctp == NULL) return INVALID_RESIDUE;
	return sctp->start_at;
}

/*****************************************************************************
*
*   LastResidueInCode(sctp)
*      returns last valid residue code in sequence code table
*      nb: some codes have "holes", a range of invalid values between first
*      and last.
*
*****************************************************************************/
NLM_EXTERN Uint1 LastResidueInCode (SeqCodeTablePtr sctp)
{
	if (sctp == NULL) return INVALID_RESIDUE;
	return (Uint1)((int)(sctp->start_at) + (int)(sctp->num) - 1);
}

/*****************************************************************************
*
*   GetIndexForResidue(sctp, residue)
*   	gets index into sctp structs for residue
*   	returns INVALID_RESIDUE if no good
*
*****************************************************************************/
NLM_EXTERN Uint1 GetIndexForResidue(SeqCodeTablePtr sctp, Uint1 residue);
NLM_EXTERN Uint1 GetIndexForResidue(SeqCodeTablePtr sctp, Uint1 residue)
{
	if (sctp == NULL) return INVALID_RESIDUE;
	if (residue < sctp->start_at) return INVALID_RESIDUE;
	residue -= sctp->start_at;
	if (residue >= sctp->num) return INVALID_RESIDUE;
	return residue;
}


/*****************************************************************************
*
*   GetSymbolForResidue(sctp, residue)
*   	returns the ONE LETTER symbol for residue if sequence code has one
*       letter symbols. returns INVALID_RESIDUE if not a valid residue or if
*       sequence code uses multi-letter symbols
*
*****************************************************************************/
NLM_EXTERN Uint1 GetSymbolForResidue (SeqCodeTablePtr sctp, Uint1 residue)
{
	Uint1 offset;

	offset = GetIndexForResidue (sctp, residue);
	if (offset == INVALID_RESIDUE) return offset;
	if (! sctp->one_letter) return INVALID_RESIDUE;
	if (sctp->letters[offset] == '\0') return INVALID_RESIDUE;
	return (Uint1)(sctp->letters[offset]);
}

/*****************************************************************************
*
*   GetResidueForSymbol(sctp, residue)
*   	returns the residue for a ONE LETTER if sequence code has one
*       letter symbols. returns INVALID_RESIDUE if not a valid symbol or if
*       sequence code uses multi-letter symbols
*       CASE matters
*
*****************************************************************************/
NLM_EXTERN Uint1 GetResidueForSymbol (SeqCodeTablePtr sctp, Uint1 symbol)
{
	Int2 ctr;
	CharPtr letters;

	if (sctp == NULL) return INVALID_RESIDUE;
	if (! sctp->one_letter) return INVALID_RESIDUE;

	letters = sctp->letters;
	for (ctr = 0; ctr < (Int2)sctp->num; ctr++, letters++)
	{
		if ((Char)symbol == *letters)
			return ((Uint1)ctr + sctp->start_at);
	}
	return INVALID_RESIDUE;
}

/*****************************************************************************
*
*   GetLongSymbolForResidue(sctp, symbol)
*   	returns string symbol for residue if sequence code has string 
*       symbols. returns NULL if not a valid residue or if
*       sequence code uses One letter symbols
*
*****************************************************************************/
NLM_EXTERN const char * GetLongSymbolForResidue (SeqCodeTablePtr sctp, Uint1 residue)
{
	Uint1 offset;

	offset = GetIndexForResidue (sctp, residue);
	if (offset == INVALID_RESIDUE) return NULL;
	if (sctp->one_letter) return NULL;
	
	return (const char *)(sctp->symbols[offset]);

}

/*****************************************************************************
*
*   GetResidueForLongSymbol(sctp, symbol)
*   	returns the residue for a STRING symbol if sequence code has string
*       symbols. returns INVALID_RESIDUE if not a valid symbol or if
*       sequence code uses one-letter symbols
*       CASE matters
*
*****************************************************************************/
NLM_EXTERN Uint1 GetResidueForLongSymbol (SeqCodeTablePtr sctp, CharPtr symbol)
{
	Int2 ctr;
	CharPtr PNTR symbols;

	if ((sctp == NULL) || (symbol == NULL)) return INVALID_RESIDUE;
	if (sctp->one_letter) return INVALID_RESIDUE;

	symbols = sctp->symbols;
	for (ctr = 0; ctr < (Int2)sctp->num; ctr++, symbols++)
	{
		if (! StringCmp(*symbols, symbol))
			return ((Uint1)ctr + sctp->start_at);
	}
	return INVALID_RESIDUE;
}

/*****************************************************************************
*
*   const char * GetNameForResidue (sctp, residue)
*      returns the descriptive name (eg. "Leucine") for a residue in the
*      sequence code defined by sctp
*      returns NULL if not a valid code in the alphabet
*      nb: some codes have "holes" in them, regions of values that are
*       invalid.
*
*****************************************************************************/
NLM_EXTERN const char * GetNameForResidue (SeqCodeTablePtr sctp, Uint1 residue)
{
	Uint1 offset;

	offset = GetIndexForResidue (sctp, residue);
	if (offset == INVALID_RESIDUE) return NULL;
	
	return (const char *)(sctp->names[offset]);

}

/*****************************************************************************
*
*   SeqMapTablePtr SeqMapTableFind(to, from)
*      Map from sequence code "from" to sequence code "to"
*      Sequence codes defined in objseq.h
*      For to == ncbi2na initialize Random generator and for
*      Seq_code_iupacna --> Seq_code_ncbi2na initialize conversion table
*****************************************************************************/
NLM_EXTERN SeqMapTablePtr LIBCALL SeqMapTableFind (Uint1 to, Uint1 from)
{
  
  /* If we want to convert iupacna to ncbi4na initialize 
     randomize conversion table */

  if(to == Seq_code_ncbi2na) {
   /* Nlm_RandomSeed(Nlm_GetSecs()); */
    
    if(from == Seq_code_iupacna && !NaI2InitOk) {      
      if(!InitNaI2Table())
        return NULL;
    }
  }
  return SeqMapTableFindObj (to, from);
}

/*****************************************************************************
 *
*   void NaI2TableFree(void)
*      Free allocated memory for
*      Seq_code_iupacna --> Seq_code_ncbi2na transfer
*****************************************************************************/
NLM_EXTERN void NaI2TableFree(void)
{
  Int4 i;
  for(i=0; i < 5; i++)
    MemFree(NaI2[i]);
}

/*****************************************************************************
*
*   Boolean InitNaI2Table(void)
*      Initialize random conversion table for
*      Seq_code_iupacna --> Seq_code_ncbi2na transfer
*****************************************************************************/
static Boolean InitNaI2Table(void)
{
  SeqMapTablePtr smtp;
  register Int4 i, j;
  Uint1 ch;
  
  /* Initialization of random function by some long value */
  
  if((smtp = SeqMapTableFindObj(Seq_code_iupacna, 
                                Seq_code_ncbi4na)) == NULL)
    return FALSE;
  
  for(i = 0; i < 5; i++) {
    NaI2[i] = (Int1Ptr) MemNew(256);
    MemSet((CharPtr) NaI2[i], -1, 256);
  }

  MemSet((CharPtr)NaI2Set, -1, sizeof(NaI2Set));  

  for(i = 0 ; i < 16; i ++) {
    NaI2Set[ch = (Uint1)SeqMapTableConvert(smtp, (Uint1)i)] = Na42Set[i];
    for(j = 0; j < 5; j++)
      NaI2[j][ch] = Na42[i][j];
  }
  NaI2InitOk = TRUE;
  return TRUE;
}

/*****************************************************************************
*
*   Convert4NaRandom(from, to)
*       Converts Seq_code_ncbi4na "from" to  Seq_code_ncbi2na "to" 
*       with random conversions
*       Return TRUE if conversion done without randomization
*       Nlm_RandomSeed(Nlm_GetSecs()); recommended in calling function
*****************************************************************************/
NLM_EXTERN Boolean Convert4NaRandom(Uint1 from, Uint1 PNTR to)
{
  Boolean retvalue;

  *to = (Uint1) (retvalue = (Na42Set[from] >= 0)) ? 
    Na42Set[from] : CONVERT_42_RAND(from);
  return retvalue;
}

/*****************************************************************************
*
*   SeqMapTableConvert(smtp, from)
*       returns conversion of "from" using SeqMapTable smtp
*       To to == Seq_code_ncbi2na use random conversion table
*
*****************************************************************************/
NLM_EXTERN Uint1 SeqMapTableConvert (SeqMapTablePtr smtp, Uint1 from)

{
  Int2 index;

  if (smtp == NULL) return (Uint1)(INVALID_RESIDUE);
  
  /* For conversions into ncbi2na encoding we will use randomized
     generation of residues */

  if(smtp->to == Seq_code_ncbi2na) {
    if(smtp->from ==  Seq_code_ncbi4na)
      return (Uint1) (Na42Set[from] < 0) ? 
        CONVERT_42_RAND(from) : Na42Set[from];
    else if(smtp->from == Seq_code_iupacna)
      return (Uint1) (NaI2Set[from] < 0) ? 
        CONVERT_I2_RAND(from) : NaI2Set[from];
  }

  /* This will handle all other cases */

  index = (Int2)from - (Int2)(smtp->start_at);
  if ((index >= 0) && (index < (Int2)(smtp->num)))
    return (Uint1)(smtp->table[index]);
  else
    return (Uint1)(INVALID_RESIDUE);
}

/*****************************************************************************
*
*   SeqCodeTableComp(sctp, residue)
*       returns complement of residue if possible
*       or residue, if not
*       assumes residue is in the same code as sctp
*
*****************************************************************************/
NLM_EXTERN Uint1 SeqCodeTableComp (SeqCodeTablePtr sctp, Uint1 residue)

{
	Int2 index;
	
    if ((sctp == NULL) || (sctp->comps == NULL))   /* no complement table */
        return INVALID_RESIDUE;

	index = (Int2)residue - (Int2)(sctp->start_at);
	if ((index < 0 ) || (index >= (Int2)(sctp->num)))
		return INVALID_RESIDUE;
	else
        return sctp->comps[index];
}

/*****************************************************************************
*
*   SeqEntryList(sep, mydata, mycallback, index, indent)
*       traverses all Seq-entry nodes beginning with sep
*       calls mycallback() at each node
*
*****************************************************************************/
NLM_EXTERN Int4 SeqEntryList (SeqEntryPtr sep, Pointer mydata, SeqEntryFunc mycallback, Int4 index, Int2 indent)

{
    if (sep == NULL)
        return index;

    if (mycallback != NULL)
        (*mycallback)(sep, mydata, index, indent);
    index++;

    if (IS_Bioseq(sep))    /* bioseq, no contained sequences */
        return index;

    sep = ((BioseqSetPtr)sep->data.ptrvalue)->seq_set;
    indent++;
    while (sep != NULL)
    {
        index = SeqEntryList(sep, mydata, mycallback, index, indent);
        sep = sep->next;
    }
    return index;
}

/*****************************************************************************
*
*   BioseqList(sep, mydata, mycallback, index, indent)
*       traverses all Seq-entry nodes beginning with sep
*       calls mycallback() at each node that is a Bioseq
*       Does NOT enter BioseqSets of _class "parts"
*       Does NOT increment indent
*
*****************************************************************************/
NLM_EXTERN Int4 BioseqList (SeqEntryPtr sep, Pointer mydata, SeqEntryFunc mycallback, Int4 index, Int2 indent)

{
    if (sep == NULL)
        return index;

    if (IS_Bioseq(sep))    /* bioseq, no contained sequences */
    {
        if (mycallback != NULL)
            (*mycallback)(sep, mydata, index, indent);
        return index+1;
    }

    if (Bioseq_set_class(sep) == 4)    /* parts, do not enter */
        return index;

    sep = ((BioseqSetPtr)sep->data.ptrvalue)->seq_set;
    while (sep != NULL)
    {
        index = BioseqList(sep, mydata, mycallback, index, indent);
        sep = sep->next;
    }
    return index;
}

/*****************************************************************************
*
*   SeqEntryGetSeqDescr(sep, type, curr)
*       returns pointer to the next SeqDescr of this type
*       type gives type of Seq-descr
*        if 0, gives all types
*       curr is NULL or previous node of this type found
*
*****************************************************************************/
NLM_EXTERN ValNodePtr SeqEntryGetSeqDescr (SeqEntryPtr sep, Int2 type, ValNodePtr curr)    /* the last one you used */

{

	if (sep == NULL) return NULL;
	
    if (curr == NULL)
    {
        if (IS_Bioseq(sep))
            curr = ((BioseqPtr)sep->data.ptrvalue)->descr;
        else
            curr = ((BioseqSetPtr)sep->data.ptrvalue)->descr;
    }
    else
        curr = curr->next;     /* move past last one */

    while (curr != NULL)
    {
        if ((! type) || ((Int2)curr->choice == type))
            return curr;
        else
            curr = curr->next;
    }
    return NULL;
}
/*****************************************************************************
*
*   SeqEntryGetTitle(sep)
*       returns pointer to the first title of this SeqEntry
*
*****************************************************************************/
NLM_EXTERN CharPtr SeqEntryGetTitle (SeqEntryPtr sep)

{
    ValNodePtr ptr;

    ptr = SeqEntryGetSeqDescr(sep, Seq_descr_title, NULL);
    if (ptr != NULL)
        return (CharPtr)ptr->data.ptrvalue;
    else
        return NULL;
}

/*****************************************************************************
*
*   Bioseq_set_class (SeqEntryPtr sep)
*       returns class of set as is enumerated in ASN.1 spec
*       returns 0 if not a Bioseq-set
*
*****************************************************************************/
NLM_EXTERN Uint1 Bioseq_set_class (SeqEntryPtr sep)

{
	if (sep == NULL) return 0;
	
    if (IS_Bioseq_set(sep))
        return ((BioseqSetPtr)sep->data.ptrvalue)->_class;
    else
        return 0;
}

/*****************************************************************************
*
*   SeqEntryDoConvert(sep, newcode, index, indent)
*       converts a seqentry which is a raw bioseq to newcode
*       callback used by SeqEntryConvert()
*
*****************************************************************************/
NLM_EXTERN void SeqEntryDoConvert (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
NLM_EXTERN void SeqEntryDoConvert (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)

{
    if (! IS_Bioseq(sep))
        return;

	if (((Uint1Ptr)data)[0] != 0)
	{
	    if (BioseqConvert((BioseqPtr)sep->data.ptrvalue, * ((Uint1Ptr)data)))
    	    ((Uint1Ptr)data)[1]++;
	}
	else
	{
		if (BioseqPack((BioseqPtr)sep->data.ptrvalue))
			((Uint1Ptr)data)[1]++;
	}
    return;
}

/*****************************************************************************
*
*   SeqEntryConvert(sep, newcode)
*       converts any seqentry to newcode
*   	if (newcode == 0)
*   		calls BioseqRawPack instead of BioseqRawConvert
*
*****************************************************************************/
NLM_EXTERN Boolean SeqEntryConvert (SeqEntryPtr sep, Uint1 newcode)

{
    Uint1 tbuf[2];
    tbuf[0] = newcode;
    tbuf[1] = 0;

	if (sep == NULL) return FALSE;
	
    SeqEntryExplore(sep, (Pointer)tbuf, SeqEntryDoConvert);
    if (tbuf[1])
        return TRUE;    /* at least one success */
    else
        return FALSE;
}

/*****************************************************************************
*
*   SeqIdBestRank(buf, num)
*   	fill buf of length num with std ranks used by SeqIdFindBest
*   	returns full length of list (useful if num is too small)
*   	std ranks always between 50 and 100
*   	rank < 50 guarantees SeqIdSelect() chooses over std rank
*   	rank > 100 guarantees SeqIdSelect() never chooses over std rank
*   	rank = 255 guarantees SeqIdSelect() never choses
*   	if buf == NULL, just returns count of supported Seq-ids
*
*****************************************************************************/
NLM_EXTERN Int2 SeqIdBestRank (Uint1Ptr buf, Int2 num)
{
	static Uint1 std_order[NUM_SEQID] = {
 	83,  /* 0 = not set */
	80,  /* 1 = local Object-id */
	70,  /* 2 = gibbsq */
	70,  /* 3 = gibbmt */
	70,  /* 4 = giim Giimport-id */
	60,  /* 5 = genbank */
	60,  /* 6 = embl */
	60,  /* 7 = pir */
	60,  /* 8 = swissprot */
	67,  /* 9 = patent */
	65,  /* 10 = other TextSeqId */
	80,  /* 11 = general Dbtag */
	51,  /* 12 = gi */
	60,  /* 13 = ddbj */
	60,  /* 14 = prf */
	60,  /* 15 = pdb */
	60,  /* 16 = tpg */
	60,  /* 17 = tpe */
	62,  /* 18 = tpd */
	68   /* 19 = gpp */
	};

	if (buf == NULL) return NUM_SEQID;

	if (num > NUM_SEQID)
		num = NUM_SEQID;
	MemCopy(buf, std_order, (size_t)(num * sizeof(Uint1)));
	return NUM_SEQID;
}

/*****************************************************************************
*
*   SeqIdFindBest(sip)
*       Find the most reliable SeqId in a chain
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr SeqIdFindBest (SeqIdPtr sip, Uint1 target)
{
	Uint1 order[NUM_SEQID];

	if (sip == NULL)
		return NULL;

	SeqIdBestRank(order, NUM_SEQID);
	if ((target > 0) && (target < NUM_SEQID))
		order[target] = 0;    /* select target */
	else if (target >= NUM_SEQID)
		ErrPostEx(SEV_ERROR, 0, 0, "SeqIdFindBest: target [%d] out of range [%d]",
			(int)target, (int)NUM_SEQID);

	return SeqIdSelect (sip, order, NUM_SEQID);
}
/*****************************************************************************
*
*   SeqIdFindBestAccn(sip)
*       Find the most reliable Accession SeqId in a chain
*       else returns gi;
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr SeqIdFindBestAccession (SeqIdPtr sip)
{
	Uint1 order[NUM_SEQID];

	if (sip == NULL)
		return NULL;
	SeqIdBestRank(order, NUM_SEQID);
        order[SEQID_GI]=order[SEQID_LOCAL]+1;
	return SeqIdSelect (sip, order, NUM_SEQID);
}

/*****************************************************************************
*
*   SeqIdPtr SeqIdLocate (sip, order, num)
*   	Given a SeqId (sip):
*   		Locates the Bioseq in memory or cached
*   		Then calls SeqIdSelect with the Bioseq.id chain to find the
*             SeqId type you want.
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr SeqIdLocate (SeqIdPtr sip, Uint1Ptr order, Int2 num)
{
	BioseqPtr bsp;
	SeqIdPtr res = NULL;
	Boolean locked = FALSE;

	bsp = BioseqFindCore(sip);
	if (bsp == NULL)
	{
		bsp = BioseqLockById(sip);
		if (bsp != NULL)
			locked = TRUE;
		else
			return res;
	}
	res = SeqIdSelect(bsp->id, order, num);
	if (locked)
		BioseqUnlock(bsp);
	return res;
}

/*****************************************************************************
*
*   SeqIdPtr SeqIdSelect (sip, order, num)
*   	takes an array (order) num long.
*   	goes down chain starting with sip.
*       finds lowest value of order[sip->choice] and returns it.
*       if order[] == 255, it is skipped.
*       if nothing is found < 255, NULL is returned
*   	ErrorMessage if sip->choice >= num
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr SeqIdSelect (SeqIdPtr sip, Uint1Ptr order, Int2 num)
{
    SeqIdPtr bestid;

	if ((sip == NULL) || (order == NULL))
		return NULL;

    for ( bestid = NULL; sip != NULL; sip = sip -> next) 
    {
		if ((Int2)sip->choice < num)
		{
			if (order[sip->choice] < 255)
			{
				if (bestid == NULL)
					bestid = sip;
				else if (order[sip->choice] < order[bestid->choice])
					bestid = sip;
			}
		} else {
			ErrPostEx(SEV_ERROR, 0,0, "SeqIdSelect: choice [%d] out of range [%d]",
				(int)(sip->choice), (int)num);
			if(sip->choice > NUM_SEQID) /*** something is really wrong ***/
				return NULL;
		}
    }

    return bestid;
}

	static char * delim = "|";
	static char * txtid [NUM_SEQID] = {		  /* FASTA_LONG formats */
		"???" ,		/* not-set = ??? */
		"lcl",		/* local = lcl|integer or string */
		"bbs",      /* gibbsq = bbs|integer */
		"bbm",		/* gibbmt = bbm|integer */
		"gim",		/* giim = gim|integer */
		"gb",		/* genbank = gb|accession|locus */
		"emb",		/* embl = emb|accession|locus */
		"pir",		/* pir = pir|accession|name */
		"sp",		/* swissprot = sp|accession|name */
		"pat",		/* patent = pat|country|patent number (string)|seq number (integer) */
		"ref",		/* other = ref|accession|name|release - changed from oth to ref */
		"gnl",		/* general = gnl|database(string)|id (string or number) */
		"gi",		/* gi = gi|integer */
		"dbj",		/* ddbj = dbj|accession|locus */
		"prf",		/* prf = prf|accession|name */
		"pdb",		/* pdb = pdb|entry name (string)|chain id (char) */
		"tpg",      /* tpg = tpg|accession|name */
		"tpe",      /* tpe = tpe|accession|name */
		"tpd",      /* tpd = tpd|accession|name */
		"gpp"};     /* gpp = gpp|accession|name */

/*****************************************************************************
*
*   SeqIdPrint(sip, buf, format)
*   	PRINTID_FASTA_LONG treats sip as a chain, printing gi|other id
*   		other id is as given in the comments for txtid. Empty fields
*   		do not eliminate | delimiters
*   	PRINTID_FASTA_SHORT prints only the sip.
*   		same format as FASTA_LONG (for other id)
*
*   	PRINTID_TEXTID_LOCUS or ACCESSION
*  --------------------------------------------------------
*  | OLDWAY:                                              |
*  |      TextSeqId types- fills request or first char in |
*  |      buffer \0 if cannot be filled                   |
*  |        gibbmt, gibbsq = fills with _M or _S [number] |
*  |      other types- fills in as FASTA_SHORT            |
*  --------------------------------------------------------
*      CURRENTLY:
*      for SEQID_GENBANK,SEQID_EMBL,SEQID_DDBJ, takes accession
*        or locus field; for SEQID_LOCAL, takes str 
*              as accession only
*       ALL others as FASTA_SHORT
*
*       PRINTID_REPORT- similar to FASTA_SHORT but removes extra optional
*         fields and | to make more human readable (but less parseable)
*   
*   if format is in the range ' ' to 127 (32-12y) ASCII, then the character
*     given is used as a separator instead of '|' and the format is
*     PRINTID_FASTA_SHORT. 127 is translated as TAB (ASCII 9)
*     This makes this function flexible for bulk
*     data processing. Note that this invalidates SeqIdParse() and may create
*     conflicts with names. Use with caution.
*     
*   return value points to \0 at end of buf
*   
*****************************************************************************/
NLM_EXTERN CharPtr SeqIdPrint (SeqIdPtr isip, CharPtr buf, Uint1 format)

{
	return SeqIdWrite (isip, buf, format, 255); /* no knowledge of buffer size */
}

/*****************************************************************************
*
*   SeqIdWrite (isip, buf, format, buflen)
*   	Similar to SeqIdPrint, has additional argument buflen, 
*   	checks the buflen, writes up to buflen chars, 
*		makes the last character '>'
*   	always puts one '\0' to terminate the string in buf
*   	buf MUST be one character longer than buflen to leave room for the
*   		last '\0' 
*
*****************************************************************************/
NLM_EXTERN CharPtr SeqIdWrite (SeqIdPtr isip, CharPtr buf, Uint1 format, Uint4 buflen)

{
	SeqIdPtr sip;
    char localbuf[32];    /* for MS Windows */
	char *ldelim;
	char d [2];
    CharPtr tmp;
	static Uint1 fasta_order[NUM_SEQID] = {  /* order for other id FASTA_LONG */
 	33, /* 0 = not set */
	20, /* 1 = local Object-id */
	15,  /* 2 = gibbsq */
	16,  /* 3 = gibbmt */
	30, /* 4 = giim Giimport-id */
	10, /* 5 = genbank */
	10, /* 6 = embl */
	10, /* 7 = pir */
	10, /* 8 = swissprot */
	15,  /* 9 = patent */
	12, /* 10 = other TextSeqId */
	13, /* 11 = general Dbtag */
	255,  /* 12 = gi */
	10, /* 13 = ddbj */
	10, /* 14 = prf */
	12,  /* 15 = pdb */
    10,  /* 16 = tpg */
    10,  /* 17 = tpe */
    10,  /* 18 = tpd */
    15   /* 19 = gpp */
    };
	static Uint1 tmsmart_order[NUM_SEQID] = {  /* order for other id FASTA_LONG */
 	33, /* 0 = not set */
	20, /* 1 = local Object-id */
	15,  /* 2 = gibbsq */
	16,  /* 3 = gibbmt */
	30, /* 4 = giim Giimport-id */
	10, /* 5 = genbank */
	10, /* 6 = embl */
	10, /* 7 = pir */
	10, /* 8 = swissprot */
	15,  /* 9 = patent */
	12, /* 10 = other TextSeqId */
	29, /* 11 = general Dbtag */
	255,  /* 12 = gi */
	10, /* 13 = ddbj */
	10, /* 14 = prf */
	12,  /* 15 = pdb */
    10,  /* 16 = tpg */
    10,  /* 17 = tpe */
    10,  /* 18 = tpd */
    15   /* 19 = gpp */
    };
	static Uint1 general_order[NUM_SEQID] = {  /* order for other id FASTA_LONG */
 	33, /* 0 = not set */
	20, /* 1 = local Object-id */
	15,  /* 2 = gibbsq */
	16,  /* 3 = gibbmt */
	30, /* 4 = giim Giimport-id */
	10, /* 5 = genbank */
	10, /* 6 = embl */
	10, /* 7 = pir */
	10, /* 8 = swissprot */
	15,  /* 9 = patent */
	13, /* 10 = other TextSeqId */
	12, /* 11 = general Dbtag */
	255,  /* 12 = gi */
	10, /* 13 = ddbj */
	10, /* 14 = prf */
	12,  /* 15 = pdb */
    10,  /* 16 = tpg */
    10,  /* 17 = tpe */
    10,  /* 18 = tpd */
    15   /* 19 = gpp */
    };
    Boolean useGeneral = FALSE;
    TextSeqIdPtr tsip;
	PDBSeqIdPtr psip;
	ObjectIdPtr oip;
	PatentSeqIdPtr patsip;
	Boolean got_gi = FALSE;
	Boolean got_tmsmart = FALSE;
	DbtagPtr dbt;
	Char chainbuf[3];
	Char versionbuf[10];
	Int2 version = 0;

	buf[0] = '\0';
	buflen--;
	tmp = buf;
	if (isip == NULL)
		return tmp;

	d [0] = *delim;
	d [1] = '\0';
	ldelim = &(d [0]);
	if ((format >= ' ') && (format <= 127))  /* change delimiter */
	{
		if (format == 127)
			d [0] = '\t';
		else
			d [0] = (char) format;
		format = PRINTID_FASTA_SHORT;
	}

	if (format == PRINTID_FASTA_GENERAL) {
		useGeneral = TRUE;
		format = PRINTID_FASTA_LONG;
	}

	if (format == PRINTID_FASTA_ALL) {
		Char allbuf [41];
		ValNodePtr vnp, head = NULL;
		size_t len = 0;
		CharPtr str;
		Boolean notfirst;

		for (sip = isip; sip != NULL; sip = sip->next) {
			SeqIdWrite (sip, allbuf, PRINTID_FASTA_SHORT, sizeof (allbuf) - 1);
			ValNodeCopyStr (&head, 0, allbuf);
		}
		for (vnp = head; vnp != NULL; vnp = vnp->next) {
		  str = (CharPtr) vnp->data.ptrvalue;
		  if (! StringHasNoText (str)) {
		    len += StringLen (str) + 1;
		  }
		}
		if (len < 1) return buf;
		tmp = MemNew (len + 2);
		if (tmp == NULL) return buf;
		notfirst = FALSE;
		for (vnp = head; vnp != NULL; vnp = vnp->next) {
		  str = (CharPtr) vnp->data.ptrvalue;
		  if (! StringHasNoText (str)) {
		    if (notfirst) {
		      StringCat (tmp, "|");
		    }
		    StringCat (tmp, str);
		    notfirst = TRUE;
		  }
		}
		ValNodeFreeData (head);
		StringNCpy_0 (buf, tmp, buflen + 1);
		MemFree (tmp);
		return buf;
	}
	
	localbuf[0] = '\0';
							/* error on input, return ??? */
	if ( (! (isip -> choice)) || (format < PRINTID_FASTA_SHORT)
		|| (format > PRINTID_REPORT))
	{
		Nlm_LabelCopyNext(&tmp, txtid[0], &buflen);
		return tmp;
	}

	if (format == PRINTID_FASTA_LONG)   /* find the ids in the chain */
	{
		for (sip = isip; sip != NULL; sip = sip->next)  /* GI present? */
		{
			if (sip->choice == SEQID_GI)
			{
				sprintf(localbuf, "%s%s%ld", txtid[SEQID_GI], ldelim,
					(long)(sip->data.intvalue));
				Nlm_LabelCopyNext(&tmp, localbuf, &buflen);
				got_gi = TRUE;
			} else if (sip->choice == SEQID_GENERAL) {
				dbt = (DbtagPtr) sip->data.ptrvalue;
				if (dbt != NULL && StringICmp (dbt->db, "TMSMART") == 0) {
					got_tmsmart = TRUE;
				}
			}
		}
		if (useGeneral) {
			sip = SeqIdSelect(isip, general_order, NUM_SEQID);
		} else if (got_tmsmart) {
			sip = SeqIdSelect(isip, tmsmart_order, NUM_SEQID);
		} else {
			sip = SeqIdSelect(isip, fasta_order, NUM_SEQID);
		}
		if (sip == NULL)   /* only GI */
			return tmp;
		else if (got_gi)
		{
			if (sip->choice == SEQID_GIIM)   /* don't show GIIM with GI */
				return tmp;

			Nlm_LabelCopyNext(&tmp, ldelim, &buflen);
		}
		format = PRINTID_FASTA_SHORT; /* put on second (or only) SeqId in this format */
	}
	else
		sip = isip;          /* only one id processed */

							 /* deal with LOCUS and ACCESSION */
	if ((format == PRINTID_TEXTID_ACCESSION) || (format == PRINTID_TEXTID_LOCUS) ||
		(format == PRINTID_TEXTID_ACC_VER) || (format == PRINTID_TEXTID_ACC_ONLY))
	{
		if (format == PRINTID_TEXTID_ACCESSION) {
			format = PRINTID_TEXTID_ACC_ONLY;     /* current default */
		}
		switch (sip->choice)   /* get the real TextSeqId types */
		{
	        case SEQID_GENBANK:
    	    case SEQID_EMBL:
    	    case SEQID_DDBJ:
			case SEQID_PIR:
			case SEQID_SWISSPROT:
			case SEQID_PRF:
			case SEQID_OTHER:
			case SEQID_TPG:
			case SEQID_TPE:
			case SEQID_TPD:
		    case SEQID_GPIPE:
    	        tsip = (TextSeqIdPtr)sip->data.ptrvalue;
				if ((format == PRINTID_TEXTID_LOCUS) && (tsip->name != NULL)) {
					Nlm_LabelCopyNext(&tmp, tsip->name, &buflen);
					return tmp;
                } else if ((format == PRINTID_TEXTID_ACC_ONLY || format == PRINTID_TEXTID_LOCUS) 
					&& (tsip->accession != NULL)) {
					Nlm_LabelCopyNext(&tmp, tsip->accession, &buflen);
					return tmp;
                } else if ((format == PRINTID_TEXTID_ACC_VER) 
					&& (tsip->accession != NULL)) {
					if (tsip->version > 0 && tsip->release == NULL) {
						sprintf(localbuf, "%s.%d", tsip->accession,
							(int)(tsip->version));
					} else {
						sprintf(localbuf, "%s", tsip->accession);
					}
					Nlm_LabelCopyNext(&tmp, localbuf, &buflen);
					return tmp;
                }
				break;
			default:
				break;
		}
	}

	if (format == PRINTID_FASTA_SHORT)
	{
		Nlm_LabelCopyNext(&tmp, txtid[sip->choice], &buflen);
		Nlm_LabelCopyNext(&tmp, ldelim, &buflen);
	}

    switch (sip->choice) 
    {
        case SEQID_LOCAL:           /* object id */
            if ((((ObjectIdPtr)sip->data.ptrvalue)->str) == NULL)
			{
                sprintf(localbuf, "%ld", 
							(long)((ObjectIdPtr)sip->data.ptrvalue)->id);
				Nlm_LabelCopyNext(&tmp, localbuf, &buflen);
			}
            else
				Nlm_LabelCopyNext(&tmp, 
						((ObjectIdPtr)sip->data.ptrvalue)->str, &buflen);
            break;
        case SEQID_GIBBSQ:         
        case SEQID_GIBBMT:
		case SEQID_GI:
			sprintf(localbuf, "%ld", (long)sip->data.intvalue);
			Nlm_LabelCopyNext(&tmp, localbuf, &buflen);
            break;
        case SEQID_GIIM:
            sprintf(localbuf, "%ld", (long)((GiimPtr)sip->data.ptrvalue)->id);
			Nlm_LabelCopyNext(&tmp, localbuf, &buflen);
            break;
        case SEQID_GENBANK:
        case SEQID_EMBL:
        case SEQID_DDBJ:
        case SEQID_OTHER:
        case SEQID_TPG:
        case SEQID_TPE:
        case SEQID_TPD:
        case SEQID_GPIPE:
           tsip = (TextSeqIdPtr)(sip->data.ptrvalue);
	   if (((tsip->version > 0) && (tsip->release == NULL)) && SHOWVERSION)
		version = tsip->version;  /* show versions */
	   sprintf(versionbuf, ".%d", (int)version);
        case SEQID_PIR:
        case SEQID_SWISSPROT:
        case SEQID_PRF:
            tsip = (TextSeqIdPtr)sip->data.ptrvalue;
            if (tsip->accession != NULL)
			{
			   Nlm_LabelCopyNext(&tmp, tsip->accession, &buflen);
                           if (version)
				Nlm_LabelCopyNext(&tmp, versionbuf,&buflen);
			   if (format != PRINTID_FASTA_SHORT)
			 	break;
			}
			if (format == PRINTID_FASTA_SHORT)
				Nlm_LabelCopyNext(&tmp, ldelim, &buflen);
			if (tsip->name != NULL)
				Nlm_LabelCopyNext(&tmp, tsip->name, &buflen);
			/*
			if (sip->choice == SEQID_OTHER) {
				Nlm_LabelCopyNext(&tmp, ldelim, &buflen);
				if (tsip->release != NULL)
					Nlm_LabelCopyNext(&tmp, tsip->release, &buflen);
			}
			*/
            break;
        case SEQID_PATENT:
			patsip = (PatentSeqIdPtr)(sip->data.ptrvalue);
			Nlm_LabelCopyNext(&tmp, patsip->cit->country, &buflen);
			if (format == PRINTID_FASTA_SHORT)
				Nlm_LabelCopyNext(&tmp, ldelim, &buflen);
			Nlm_LabelCopyNext(&tmp, patsip->cit->number, &buflen);
			if (format == PRINTID_FASTA_SHORT)
				Nlm_LabelCopyNext(&tmp, ldelim, &buflen);
			else
				Nlm_LabelCopyNext(&tmp, "_", &buflen);
			sprintf(localbuf, "%d", (int)patsip->seqid);
			Nlm_LabelCopyNext(&tmp, localbuf, &buflen);
            break;
        case SEQID_GENERAL:
			oip = ((DbtagPtr)sip->data.ptrvalue)->tag;
			if((format == PRINTID_FASTA_SHORT) || (format == PRINTID_REPORT))
				Nlm_LabelCopyNext(&tmp, 
					((DbtagPtr)sip->data.ptrvalue)->db, &buflen);
			if (format == PRINTID_FASTA_SHORT)
				Nlm_LabelCopyNext(&tmp, ldelim, &buflen);
			else if (format == PRINTID_REPORT)
				Nlm_LabelCopyNext(&tmp, ":", &buflen);

			if (oip->str == NULL)
			{
				sprintf(localbuf, "%ld", (long) oip->id);
				Nlm_LabelCopyNext(&tmp, localbuf, &buflen);
			}
			else
				Nlm_LabelCopyNext(&tmp, oip->str, &buflen);
            break;
        case SEQID_PDB:
			psip = (PDBSeqIdPtr) sip->data.ptrvalue;
			chainbuf[0] = TO_UPPER (psip->chain);
			chainbuf[1] = '\0';
			chainbuf[2] = '\0';
			if (IS_LOWER (psip->chain)) {
			  chainbuf[1] = chainbuf [0];
			}
			Nlm_LabelCopyNext(&tmp, psip->mol, &buflen);
			if (format == PRINTID_FASTA_SHORT)
			{
				Nlm_LabelCopyNext(&tmp, ldelim, &buflen);
				if (chainbuf[0] == '|') /* special */
					Nlm_LabelCopyNext(&tmp, "VB",&buflen);
				else if (chainbuf[0] != '\0')
					Nlm_LabelCopyNext(&tmp,chainbuf, &buflen);
				else
					Nlm_LabelCopyNext(&tmp, " ", &buflen);
			}
			else if (psip->chain > ' ')
			{
				Nlm_LabelCopyNext(&tmp, "_", &buflen);
				Nlm_LabelCopyNext(&tmp,chainbuf, &buflen);
			}
            break;
		default:
			Nlm_LabelCopyNext(&tmp, txtid[0], &buflen);
			break;

    }
    return tmp;
}

/* The following function finds either an integer or a string id from
   SeqIdPtr */

Boolean GetAccessionFromSeqId(SeqIdPtr sip, Int4Ptr gi, CharPtr PNTR id)
{
   return GetAccessionVersionFromSeqId(sip, gi, id, FALSE);
}

/* Maximal length of a version number in Accession.version identifiers */
#define MAX_VERSION_LENGTH 10

Boolean GetAccessionVersionFromSeqId(SeqIdPtr sip, Int4Ptr gi, 
                                     CharPtr PNTR id, Boolean get_version)
{
   Boolean numeric_id_type = FALSE;
   Int2 id_len;
   GiimPtr gip;
   ObjectIdPtr oip;
   TextSeqIdPtr textsip;
   DbtagPtr dbtag;
   PatentSeqIdPtr psip;
   PDBSeqIdPtr pdbsip;

   *id = NULL;
   *gi = 0;

   switch (sip->choice) {
   case SEQID_GI: case SEQID_GIBBSQ: case SEQID_GIBBMT:
      *gi = sip->data.intvalue;
      numeric_id_type = TRUE;
      break;
   case SEQID_GIIM:
      gip = (GiimPtr) sip->data.ptrvalue;
      *gi = gip->id;
      numeric_id_type = TRUE;
      break;
   case SEQID_LOCAL:
      oip = (ObjectIdPtr) sip->data.ptrvalue;
      
      if (oip->str) {
         id_len = StringLen(oip->str);
         *id = (CharPtr) MemNew(id_len+1);
         sprintf(*id, "%s", oip->str);
      } else {
         *gi = oip->id;
         numeric_id_type = TRUE;
      }
      break;
   case SEQID_GENBANK:
   case SEQID_EMBL:
   case SEQID_PIR: 
   case SEQID_SWISSPROT:
   case SEQID_DDBJ:
   case SEQID_PRF: 
   case SEQID_OTHER:
   case SEQID_TPG:
   case SEQID_TPE:
   case SEQID_TPD:
   case SEQID_GPIPE:
      textsip = (TextSeqIdPtr)sip->data.ptrvalue;
      if (textsip->accession) {
         if (get_version && textsip->version > 0) {
            /* Assume versions are no longer than MAX_VERSION_LENGTH digits */
            id_len = StringLen(textsip->accession) + MAX_VERSION_LENGTH + 1;
            *id = (CharPtr) MemNew(id_len+1);
            sprintf(*id, "%s.%ld", textsip->accession, textsip->version);
         } else {
            id_len = StringLen(textsip->accession);
            *id = (CharPtr) MemNew(id_len+1);
            sprintf(*id, "%s", textsip->accession);
         }
      } else if (textsip->name) {
         id_len = StringLen(textsip->name);
         *id = (CharPtr) MemNew(id_len+1);
         sprintf(*id, "%s", textsip->name);
      }
      break;
   case SEQID_GENERAL:
      dbtag = (DbtagPtr) sip->data.ptrvalue;
      if (dbtag->tag->str == NULL) {
	 numeric_id_type = TRUE;
	 *gi = dbtag->tag->id;
      } else {
	 id_len = StringLen(dbtag->tag->str);
	 *id = (CharPtr) MemNew(id_len+1);
	 sprintf(*id, "%s", dbtag->tag->str);
      }
      break;
   case SEQID_PATENT:
      psip = (PatentSeqIdPtr) sip->data.ptrvalue;
      *gi = (Int4) psip->seqid;
      numeric_id_type = TRUE;
      break;
   case SEQID_PDB:
      pdbsip = (PDBSeqIdPtr) sip->data.ptrvalue;
      id_len = StringLen(pdbsip->mol);
      *id = (CharPtr) MemNew(id_len+4);
      sprintf(*id, "%s%d", pdbsip->mol, pdbsip->chain);
      break;
   default: break;
   }
 
   return numeric_id_type;
}

/*****************************************************************************
*
*   SeqIdPtr SeqIdParse(buf)
*   	parses a string containing SeqIds formated by SeqIdPrint using
*   	FASTA_LONG or FASTA_SHORT, separated by |
*   	returns a SeqId linked list for them
*   	or NULL on failure for any SeqId
*
*****************************************************************************/
#define SEQID_PARSE_BUF_SIZE 200
NLM_EXTERN SeqIdPtr SeqIdParse(CharPtr buf)
{
	char localbuf[SEQID_PARSE_BUF_SIZE + 2];
	char * tmp, *strt, * tokens[6], *chain;
	char d;
	long num;
	CharPtr tp;
	Int2 numtoken, i, type = 0, j, ctr=0, numdigits; /* ctr is number of OK ids done */
	SeqIdPtr sip = NULL, head = NULL, last = NULL, tmpsip;
	ObjectIdPtr oip;
	DbtagPtr dp;
	TextSeqIdPtr tsip;
	PatentSeqIdPtr patsip;
	IdPatPtr ipp;
	PDBSeqIdPtr psip;
	GiimPtr gim;
	Boolean done = FALSE;
	static Uint1 expect_tokens[NUM_SEQID] = {  /* number of tokens to expect */
 	0, /* 0 = not set */
	1, /* 1 = local Object-id */
	1,  /* 2 = gibbsq */
	1,  /* 3 = gibbmt */
	1, /* 4 = giim Giimport-id */
	2, /* 5 = genbank */
	2, /* 6 = embl */
	2, /* 7 = pir */
	2, /* 8 = swissprot */
	3,  /* 9 = patent */
	3, /* 10 = other TextSeqId */
	2, /* 11 = general Dbtag */
	1,  /* 12 = gi */
	2, /* 13 = ddbj */
	2, /* 14 = prf */
	2,  /* 15 = pdb */
    2,  /* 16 = tpg */
    2,  /* 17 = tpe */
    2,  /* 18 = tpd */
    2   /* 19 = gpp */
	};

	if ((buf == NULL) || (*buf == '\0'))
		return NULL;

	d = *delim;   /* delimiter */
	while (! done)
	{
						/* set all tokens pointing to \0 */
		localbuf[SEQID_PARSE_BUF_SIZE + 1] = '\0';
		for (i = 0; i < 6; i++)
			tokens[i] = &localbuf[SEQID_PARSE_BUF_SIZE + 1];
		tp = buf;		/* save start of string */
						/* copy and tokenize - token\0token\0\n */
		for (tmp=localbuf, i=0; ((*buf != d) && (*buf != '\0') && (i < SEQID_PARSE_BUF_SIZE));
				i++,buf++,tmp++)
			*tmp = *buf;
		if (*buf != d) goto erret;  /* didn't get delimiter */
		*tmp = '\0';
		tmp++;
		buf++;
		for (j = 0, type = 0; j < NUM_SEQID; j++)
		{
			if (! StringCmp(localbuf, txtid[j]))
			{
				type = j;
				break;
			}
		}

		/* oth now ref, but still want to parse old style */
		if ((! type) && (! StringCmp(localbuf, "oth"))) {
			type = SEQID_OTHER;
		}

		if (! type) goto erret;

						/* copy and tokenize - token\0token\0\n */
		for (numtoken=0, strt=tmp;
			((i < SEQID_PARSE_BUF_SIZE) && (numtoken < (Int2)(expect_tokens[type])) && (! done));
			i++,buf++,tmp++)
		{
			if ((*buf == d) || (*buf == '\0'))
			{
				*tmp = '\0';
				tokens[numtoken] = strt;
				numtoken++;
				if (*buf == '\0')
				{
					if (type == SEQID_OTHER && (numtoken == 2 || numtoken == 1))
						done = TRUE;
					else if ((type == SEQID_GENBANK || type == SEQID_EMBL ||
							type == SEQID_DDBJ || type == SEQID_TPG ||
							type == SEQID_TPE || type == SEQID_TPD ||
							type == SEQID_GPIPE) && numtoken == 1)
						done = TRUE;
					else if (numtoken < (Int2)(expect_tokens[type]))
						goto erret;
					else
						done = TRUE;
				}
				strt = tmp+1;
			}
			else
				*tmp = *buf;
		}
		if (i == SEQID_PARSE_BUF_SIZE) goto erret;

		sip = ValNodeNew(head);
		if (head == NULL) head = sip;
		sip->choice = (Uint1) type;
		switch (type)
    	{
        	case SEQID_LOCAL:           /* object id */
				if (*tokens[0] == '\0') goto erret;
				oip = ObjectIdNew();
				sip->data.ptrvalue = oip;
				for (tmp = tokens[0], numdigits = 0; *tmp != '\0'; tmp++, numdigits++)
				{
					if (! IS_DIGIT(*tmp))   /* string type */
					{
						oip->str = StringSave(tokens[0]);
						break;
					}
				}
				if (oip->str == NULL)
				{
					sscanf(tokens[0], "%ld", &num);
					oip->id = (Int4)num;
					if (numdigits < 10 ||
						(numdigits == 10 && StringCmp (tokens [0], "2147483647") <= 0)) {
						sscanf(tokens[0], "%ld", &num);
						oip->id = (Int4)num;
					} else {
						oip->str = StringSave(tokens[0]);
					}
				}
				break;
	        case SEQID_GIBBSQ:         
    	    case SEQID_GIBBMT:
			case SEQID_GI:
				if (! IS_DIGIT(*tokens[0]))
					goto erret;
				sscanf(tokens[0], "%ld", &num);
				sip->data.intvalue = (Int4)num;
    	        break;
        	case SEQID_GIIM:
				if (! IS_DIGIT(*tokens[0])) goto erret;
				gim = GiimNew();
				sip->data.ptrvalue = gim;
				sscanf(tokens[0], "%ld", &num);
				gim->id = (Int4)num;
	            break;
    	    case SEQID_GENBANK:
        	case SEQID_EMBL:
	        case SEQID_PIR:
    	    case SEQID_SWISSPROT:
        	case SEQID_DDBJ:
			case SEQID_PRF:
    	    case SEQID_OTHER:
    	    case SEQID_TPG:
    	    case SEQID_TPE:
            case SEQID_TPD:
    	    case SEQID_GPIPE:
				if ((*tokens[0] == '\0') && (*tokens[1] == '\0'))
					goto erret;
	            tsip = TextSeqIdNew();
				sip->data.ptrvalue = tsip;
				if (*tokens[0] != '\0')
				{
                                        tmp = tokens[0]; /* check for version */
                                        while (*tmp != '\0')
					{
						if (*tmp == '.')
						{
                                                   if (IS_DIGIT(*(tmp+1)))
                                                   {
							*tmp = '\0';
                                                        sscanf((tmp+1),"%ld",&num);
                                                        tsip->version =(Int2)num;
						   }
						   else
							tmp++;
						}
						else
						  tmp++;
					}
					tsip->accession = StringSave(tokens[0]);
					*(tsip->accession) = TO_UPPER(*(tsip->accession));
				}
				if (*tokens[1] != '\0')
				{
					tsip->name = StringSave(tokens[1]);
					if (type != SEQID_OTHER) {
						tmp = tsip->name;
						while (*tmp != '\0')
						{
							*tmp = TO_UPPER(*tmp);
							tmp++;
						}
					}
				}
        	    break;
	        case SEQID_PATENT:
				if ((*tokens[0] == '\0') || (*tokens[1] == '\0')) goto erret;
				if (! IS_DIGIT(*tokens[2])) goto erret;
				patsip = PatentSeqIdNew();
				sip->data.ptrvalue = patsip;
				ipp = IdPatNew();
				patsip->cit = ipp;
				ipp->country = StringSave(tokens[0]);
				ipp->number = StringSave(tokens[1]);
				sscanf(tokens[2], "%ld", &num);
				patsip->seqid = (Int2)num;
            	break;
	        case SEQID_GENERAL:
				if ((*tokens[0] == '\0') || (*tokens[1] == '\0')) goto erret;
				dp = DbtagNew();
				sip->data.ptrvalue = dp;
				oip = ObjectIdNew();
				dp->tag = oip;
				dp->db = StringSave(tokens[0]);
				for (tmp = tokens[1], numdigits = 0; *tmp != '\0'; tmp++, numdigits++)
				{
					if (! IS_DIGIT(*tmp))   /* string type */
					{
						oip->str = StringSave(tokens[1]);
						break;
					}
				}
				if (oip->str == NULL)
				{
					if (numdigits < 10 ||
						(numdigits == 10 && StringCmp (tokens [1], "2147483647") <= 0)) {
						sscanf(tokens[1], "%ld", &num);
						oip->id = (Int4)num;
					} else {
						oip->str = StringSave(tokens[1]);
					}
				}
        	    break;
	        case SEQID_PDB:
				if (*tokens[0] == '\0') goto erret;
				psip = PDBSeqIdNew();
				sip->data.ptrvalue = psip;
				psip->mol = StringSave(tokens[0]);
				tmp = psip->mol;
				while (*tmp != '\0')
				{
					*tmp = TO_UPPER(*tmp);
					tmp++;
				}
				chain = tokens [1];
				if ((! StringICmp(tokens[1], "VB")) ||
                                    *(buf-1) == d)
					psip->chain = '|';
				else if (! StringHasNoText (tokens[1]))
					psip->chain = *tokens[1];
				/* double letter for chain indicates lower case */
				if (StringLen (chain) == 2 && TO_UPPER (chain [0]) == TO_UPPER (chain [1])) {
					psip->chain = TO_LOWER(psip->chain);
				} else {
					psip->chain = TO_UPPER(psip->chain);
				}
        	    break;
	    }
		last = sip;
		sip = NULL;
		ctr++;
	}
ret:
	return head;
erret:
	StringNCpy(localbuf, tp, SEQID_PARSE_BUF_SIZE);
	localbuf[SEQID_PARSE_BUF_SIZE] = '\0';
	ErrPostEx(SEV_INFO, 0,0, "SeqIdParse Failure at %s", localbuf);
	if (sip == head)
		head = NULL;
	else
	{
		if (last != NULL)
			last->next = NULL;
		if (! ctr)     /* no good SeqIds */
			head = SeqIdSetFree(head);
		else	       /* at least one good SeqId.. keep it */
		{
			tmpsip = head;
			last = NULL;
			for (i = 0; i < ctr; i++)
			{
				last = tmpsip;
				tmpsip = tmpsip->next;
			}
			if (last != NULL)
				last->next = NULL;
			SeqIdSetFree(tmpsip);
		}
	}
	ValNodeFree(sip);
	goto ret;
}


/*****************************************************************************
*
*   Boolean SeqIdMatch(a, b)
*   	returns TRUE if SeqIds could be compared and are the same
*       returns FALSE both if SeqIds could not be compared OR if they were
*                        compared but are different
*
*   WARNING!!!! use SeqIdComp() instead of SeqIdMatch() in most cases
*
*  The code here must work the same is in two idloader
*  context: function id_flatten_seq_obj (idsybase.c)
*  and proc id_id_flatten_seq_obj
*
*****************************************************************************/
NLM_EXTERN Boolean SeqIdMatch (SeqIdPtr a, SeqIdPtr b)
{
	Uint1 retval;

	retval = SeqIdComp(a, b);
	if (retval == SIC_YES)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
*
*   SeqIdComp(a, b)
*   	Compares a to b and returns
*
*   SIC_DIFF   = different types, could not be compared
*   SIC_NO     = types could be compared, and ids are different
*   SIC_YES    = types could be compared, and ids are the same
*
*****************************************************************************/
NLM_EXTERN Uint1 SeqIdComp (SeqIdPtr a, SeqIdPtr b)
{
    Uint1 choice;
    TextSeqIdPtr at, bt;

    if ((a == NULL) || (b == NULL))
        return SIC_DIFF;

	choice = a->choice;
	if (choice != b->choice)
	{
		switch (choice)
		{
			case SEQID_GENBANK:          /* these could be confused */
			case SEQID_EMBL:
			case SEQID_DDBJ:
			case SEQID_TPG:
			case SEQID_TPE:
			case SEQID_TPD:
		    case SEQID_GPIPE:
				switch (b->choice)
				{
					case SEQID_GENBANK:   /* its ok */
					case SEQID_EMBL:
					case SEQID_DDBJ:
					case SEQID_TPG:
					case SEQID_TPE:
					case SEQID_TPD:
				    case SEQID_GPIPE:
						break;  
					default:
						return SIC_DIFF;
				}
				break;
			default:
				return SIC_DIFF;
		}
	}

    switch (choice)
    {
        case SEQID_NOT_SET:
            return SIC_DIFF;
        case SEQID_LOCAL:   
            if (ObjectIdMatch((ObjectIdPtr)a->data.ptrvalue, (ObjectIdPtr)b->data.ptrvalue))
				return SIC_YES;
			else
				return SIC_NO;
        case SEQID_GIBBSQ:   /* gibbsq */
        case SEQID_GIBBMT:   /* gibbmt */
        case SEQID_GI:  /* gi */
            if (a->data.intvalue == b->data.intvalue)
				return SIC_YES;
			else
				return SIC_NO;
        case SEQID_GIIM:   /* giim */
            if (((GiimPtr)a->data.ptrvalue)->id == ((GiimPtr)b->data.ptrvalue)->id)
				return SIC_YES;
			else
				return SIC_NO;
        case SEQID_PATENT:   /* patent seq */
            if (((PatentSeqIdPtr)a->data.ptrvalue)->seqid !=
                ((PatentSeqIdPtr)b->data.ptrvalue)->seqid)
                return SIC_NO;
            if (IdPatMatch(((PatentSeqIdPtr)a->data.ptrvalue)->cit,
                ((PatentSeqIdPtr)b->data.ptrvalue)->cit))
				return SIC_YES;
			else
				return SIC_NO;
		case SEQID_PDB:     /* pdb */
            if ( StringICmp(((PDBSeqIdPtr)a->data.ptrvalue)->mol,
                ((PDBSeqIdPtr)b->data.ptrvalue)->mol))
                return SIC_NO;
            /*
            if (TO_UPPER(((PDBSeqIdPtr)a->data.ptrvalue)->chain) !=
                TO_UPPER(((PDBSeqIdPtr)b->data.ptrvalue)->chain))
                return SIC_NO;
			*/
            if (((PDBSeqIdPtr)a->data.ptrvalue)->chain !=
                ((PDBSeqIdPtr)b->data.ptrvalue)->chain)
                return SIC_NO;
			return SIC_YES;
		case SEQID_GENERAL:  /* general */
			if (DbtagMatch((DbtagPtr)a->data.ptrvalue,
				(DbtagPtr)b->data.ptrvalue))
				return SIC_YES;
			else if (StringICmp(((DbtagPtr)a->data.ptrvalue)->db,
				((DbtagPtr)b->data.ptrvalue)->db))
				return SIC_DIFF; /* db strings do not match, okay */
			else
				return SIC_NO;

        case SEQID_GENBANK:
		case SEQID_EMBL:
		case SEQID_DDBJ:
		case SEQID_PIR:
		case SEQID_SWISSPROT:
		case SEQID_PRF:
		case SEQID_OTHER:
		case SEQID_TPG:
		case SEQID_TPE:
		case SEQID_TPD:
        case SEQID_GPIPE:

            at = (TextSeqIdPtr)a->data.ptrvalue;
            bt = (TextSeqIdPtr)b->data.ptrvalue;
            if ((at->accession != NULL) && (bt->accession != NULL))
            {
                if (! StringICmp(at->accession, bt->accession)) {
                    if (at->version > 0 &&
                        bt->version > 0 &&
                        at->version != bt->version) {
                        return SIC_NO;
                    }
                    return SIC_YES;
                } else {
                    return SIC_NO;
                }
            }
            else if ((at->name != NULL) && (bt->name != NULL))
            {
                if (! StringICmp(at->name, bt->name)) {
                    if (at->version > 0 &&
                        bt->version > 0 &&
                        at->version != bt->version) {
                        return SIC_NO;
                    }
                    return SIC_YES;
                } else {
                    return SIC_NO;
                }
            }
            else
                return SIC_DIFF;
		default:
			ErrPostEx(SEV_ERROR, 0,0, "SeqIdComp: unsupported type [%d]",
				(int)choice);
			return SIC_DIFF;
     }
}

/*****************************************************************************
*
*   Boolean SeqIdIn(a, b)
*     Looks for single SeqId, "a" in chain of SeqIds, "b"
*
*****************************************************************************/
NLM_EXTERN Boolean SeqIdIn (SeqIdPtr a, SeqIdPtr b)

{
	SeqIdPtr now;
	Uint1 retval;

	if (a == NULL)
	    return FALSE;

	for (now =b; now != NULL; now = now -> next)
	{
        retval = SeqIdComp(a, now);
		switch (retval)
		{
			case SIC_YES:
				return TRUE;
			case SIC_NO:
				return FALSE;
		}
    }
    return FALSE;
}

/*****************************************************************************
*
*   SeqIdForSameBioseq(a,b)
*
*****************************************************************************/
NLM_EXTERN Boolean SeqIdForSameBioseq (SeqIdPtr a, SeqIdPtr b)

{
    BioseqPtr bsp;
	Uint1 retval;
	Boolean res = FALSE;
	/*
	Boolean locked = FALSE;
	*/

	if ((a == NULL) || (b == NULL)) return FALSE;

    retval = SeqIdComp(a,b);   /* if match, all set */
	switch (retval)
	{
		case SIC_YES:
			return TRUE;
		case SIC_NO:
			return FALSE;
	}

    bsp = BioseqFindCore(a);
	if (bsp == NULL)
	{
		return FALSE;
		/*
		bsp = BioseqLockById(a);
		if (bsp != NULL)
			locked = TRUE;
		else
			return res;
		*/
	}

    res = SeqIdIn(b, bsp->id);
    /*
    if (locked)
		BioseqUnlock(bsp);
	*/

    return res;
}

/*****************************************************************************
*
*   MakeNewProteinSeqId(SeqLocPtr slp, SeqIdPtr sip)
*   	Makes a new protein SeqId of attempting to keep it unique
*       Trys to match it to the input seqid type
*       slp is the location on the DNA of the coding region making the protein
*       sip is the SeqId of the DNA coding for the protein
*       if (sip != NULL) uses it for a "base" first
*       else if (slp != NULL) uses a SeqId from it for a base
*       else base is the string tmpseq
*
*       id is then base_X where X is a number assigned as a serial number
*       the returned id is guaranteed to be unique among all Bioseqs currently
*       loaded in memory. 
*   	
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr LIBCALL MakeNewProteinSeqIdExMT (SeqLocPtr slp, SeqIdPtr sip, CharPtr prefix, Int2Ptr ctrptr, Boolean is_MT_safe)
{
    Char buf[40];
    CharPtr tmp;
    Int2 ctr;
    Int2 start = 1;
    SeqLocPtr tslp;
    ValNodePtr newid;
    ObjectIdPtr oid;
    ValNode vn;
    TextSeqId tsi;
    ValNodePtr altid;
    size_t len;
    static Uint4 counter;
    static TNlmMutex lock = NULL;
    
    
    if (lock == NULL) {
        NlmMutexInit(&lock);
    }
    
    /* create a possible GenBankStyle id as well */
    altid = &vn;
    vn.choice = SEQID_GENBANK;
    vn.next = NULL;
    vn.data.ptrvalue = &tsi;
    tsi.name = NULL;
    tsi.accession = NULL;
    tsi.version = INT2_MIN;
    tsi.release = NULL;
    
    if ((sip == NULL) && (slp != NULL)) {
        tslp = NULL;
        while ((tslp = SeqLocFindNext(slp, tslp)) != NULL) {
            sip = SeqLocId(tslp);
            if (sip != NULL)
                break;
        }
    }
    
    if (sip != NULL) {
        SeqIdWrite(sip, buf, PRINTID_TEXTID_ACCESSION, 30);
        tmp = buf;
        while (*tmp != '\0')
            tmp++;
        if (*(tmp-1) == '>')
            tmp--;
        *tmp = '_';
        tmp++;
        *tmp = '\0';
    } else {
        len = StringLen (prefix);
        if (len > 0 && len < 32) {
            tmp = StringMove(buf, prefix);
        } else {
            tmp = StringMove(buf, "tmpseq_");
        }
    }
    
    newid = ValNodeNew(NULL);
    oid = ObjectIdNew();
    oid->str = buf;   /* allocate this later */
    newid->choice = SEQID_LOCAL;
    newid->data.ptrvalue = oid;
    
    tsi.name = buf;   /* check for alternative form */
    
    if (ctrptr != NULL) {
        start = *ctrptr;
    }
    if (start < 1) {
        start = 1;
    }

    /* Very dangerous way to create new id - don't use if you can */

    if(is_MT_safe == FALSE) {
	for (ctr = start; ctr < 32000; ctr++) {
            sprintf(tmp, "%d", (int)ctr);
            if ((BioseqFindCore(newid) == NULL) && (BioseqFindCore(altid) == NULL)) {
                oid->str = StringSave(buf);
                if (ctrptr != NULL) {
                    *ctrptr = ctr + 1;
                }
                return newid;
            }
	}
    }
    
    NlmMutexLock(lock);
    
    sprintf(tmp, "%d", (int)counter);
    oid->str = StringSave(buf);
    if (ctrptr != NULL) {
        *ctrptr = ctr + 1;
    }
    
    counter++;
    NlmMutexUnlock(lock);
    
    return newid;
}

NLM_EXTERN SeqIdPtr LIBCALL MakeNewProteinSeqIdEx (SeqLocPtr slp, SeqIdPtr sip, CharPtr prefix, Int2Ptr ctrptr)
{
    return MakeNewProteinSeqIdExMT (slp, sip, prefix, ctrptr, FALSE);
}

NLM_EXTERN SeqIdPtr LIBCALL MakeNewProteinSeqId (SeqLocPtr slp, SeqIdPtr sip)
{
    return MakeNewProteinSeqIdEx (slp, sip, NULL, NULL);
}

NLM_EXTERN ObjectIdPtr UniqueLocalId(void)
{
    static TNlmMutex lock = NULL;
    static long count = 0;
    ObjectIdPtr oip;
    long l;
    Char buf[128];
    
    if (lock == NULL) {
        NlmMutexInit(&lock);
    }
    NlmMutexLock(lock);
    l = count;
    if (++count < 0) {
        count = 0;
    }
    NlmMutexUnlock(lock);
    sprintf(buf, "lcl|unique%08ld", l);
    oip = ObjectIdNew();
    oip->str = StringSave(buf);
    return oip;
}

/*****************************************************************************
*
*   Traversal routine for SeqLocFindNext
*
*****************************************************************************/
static SeqLocPtr SeqLocNext (SeqLocPtr seqlochead, SeqLocPtr currseqloc, Uint1 equiv_status, BoolPtr founditptr)

{
    SeqLocPtr currloc, retval;
	Boolean equiv_is_one, foundit=FALSE;

	switch (equiv_status)
	{
		case EQUIV_IS_ONE:
			equiv_is_one = TRUE;
			break;
		case FIRST_EQUIV_IS_MANY:
			equiv_status = EQUIV_IS_ONE;
		case EQUIV_IS_MANY:
		default:
			equiv_is_one = FALSE;
			break;
	}

    while (seqlochead != NULL)
    {
        if (IS_one_loc(seqlochead, equiv_is_one))
        {
            if (currseqloc == NULL)
                return seqlochead;
            else if (currseqloc == seqlochead)   /* found it */
			{
				*founditptr = TRUE;
				if (seqlochead -> next != NULL)
				{
					if (IS_one_loc(seqlochead->next, equiv_is_one))
						return seqlochead->next;
					else
						return SeqLocNext(seqlochead->next, NULL, equiv_status, &foundit);
				}
				else
				{
					return NULL;
				}
			}
        }
        else
        {
            currloc = (SeqLocPtr)seqlochead->data.ptrvalue;
            if (currloc != NULL)
            {
                if ((retval = SeqLocNext(currloc, currseqloc, equiv_status, &foundit)) != NULL)
                    return retval;
				else
					if (foundit)
						currseqloc = NULL;   /* no need to keep looking */
            }
        }

        seqlochead = seqlochead->next;
    }
    return NULL;
}

/*****************************************************************************
*
*   SeqLocFindNext(seqlochead, currseqloc)
*       finds the next Seq-loc after currseqloc
*       seqlochead is the first of a chain of Seq-locs
*       treats SEQLOC_EQUIV as multiple seq-locs
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr SeqLocFindNext (SeqLocPtr seqlochead, SeqLocPtr currseqloc)
{
	return SeqLocFindPart(seqlochead, currseqloc, EQUIV_IS_MANY);
}

/*****************************************************************************
*
*   SeqLocFindPart(seqlochead, currseqloc, equiv_status)
*       finds the next Seq-loc after currseqloc
*       seqlochead is the first of a chain of Seq-locs
*       equiv_status defines how to treat SEQLOC_EQUIV 
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr SeqLocFindPart (SeqLocPtr seqlochead, SeqLocPtr currseqloc, Uint1 equiv_status)
{
	SeqLocPtr tmp, oldnext;
	Boolean equiv_is_one, foundit=FALSE;

	if (seqlochead == NULL) return NULL;

	if (equiv_status == EQUIV_IS_ONE)
		equiv_is_one = TRUE;
	else
		equiv_is_one = FALSE;

	if (IS_one_loc(seqlochead, equiv_is_one))    /* not a chain */
	{
		if (currseqloc == NULL)	   /* first call */
			return seqlochead;
		else if (currseqloc == seqlochead)	 /* second call */
			return NULL;
		else						   /* oops */
			goto erret;
	}

	if (currseqloc != NULL)
	{
		if (! IS_one_loc(currseqloc, equiv_is_one)) /* oops */
			goto erret;
		tmp = currseqloc->next;
		if (tmp != NULL)
		{
			if (IS_one_loc(tmp, equiv_is_one))
				return tmp;
		}
	}
   
	oldnext = seqlochead->next;	   /* protect from accidental chains */
	seqlochead->next = NULL;

    tmp = SeqLocNext(seqlochead, currseqloc, equiv_status, &foundit);

	seqlochead->next = oldnext;
	return tmp;

erret:
    ErrPostEx(SEV_ERROR,0,0, "Invalid arguments to SeqLocFindNext");
    return NULL;
}

/*****************************************************************************
*
*   IS_one_loc(anp, equiv_is_one)
*       returns TRUE if is a sequence location which refers to one piece
*   	of sequence
*       used for moving through complicated Seq-locs
*      if equiv_is_one == TRUE, then considers a SEQ_LOC_EQUIV a single
*        location. If FALSE, does not.
*
*****************************************************************************/
NLM_EXTERN Boolean IS_one_loc (SeqLocPtr anp, Boolean equiv_is_one)      /* a SeqLoc */

{
	Boolean retval = FALSE;

	if (anp == NULL) return FALSE;
	
	switch (anp->choice)
    {
        case SEQLOC_NULL:      /* null - not a valid single region */
        case SEQLOC_EMPTY:      /* empty */
        case SEQLOC_WHOLE:      /* whole */
        case SEQLOC_INT:      /* int */
        case SEQLOC_PNT:      /* pnt */
        case SEQLOC_PACKED_PNT:      /* packed-pnt   */
        case SEQLOC_BOND:      /* bond */
			retval = TRUE;
			break;

        case SEQLOC_EQUIV:     /* equiv */
			retval = equiv_is_one;
			break;

        case SEQLOC_PACKED_INT:      /* packed seqint */
        case SEQLOC_MIX:      /* mix */
        case SEQLOC_FEAT:
			retval = FALSE;
            break;

		default:
			ErrPostEx(SEV_ERROR,0,0, "IS_one_seq: unsupported seqloc [%d]",
				(int)(anp->choice));
			retval = TRUE;
            break;
	}
	return retval;
}
/*****************************************************************************
*
*   SeqLocId(loc)
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr SeqLocId (SeqLocPtr anp)

{
    SeqIdPtr seqid = NULL, currseqid = NULL;
	SeqLocPtr loc;

	if (anp == NULL) return NULL;
	
    switch (anp->choice)
    {
        case SEQLOC_NULL:    /* NULL */
        case SEQLOC_FEAT:   /* feat -- can't track yet */
			break;
        case SEQLOC_BOND:   /* bond -- 2 seqs */
			if (((SeqBondPtr)(anp->data.ptrvalue))->a != NULL)
				seqid = ((SeqBondPtr)(anp->data.ptrvalue))->a->id;
			break;
        case SEQLOC_EMPTY:    /* empty */
        case SEQLOC_WHOLE:    /* whole */
            seqid = (SeqIdPtr)anp->data.ptrvalue;
            break;
        case SEQLOC_INT:    /* int */
            seqid = ((SeqIntPtr)anp->data.ptrvalue)->id;
			break;
        case SEQLOC_PACKED_INT:    /* packed int */
        case SEQLOC_MIX:    /* mix -- could be more than one seq */
        case SEQLOC_EQUIV:    /* equiv -- ditto */
			loc = (SeqLocPtr)anp->data.ptrvalue;
			while (loc != NULL)
			{
				if (loc->choice == SEQLOC_NULL) {
					loc = loc->next;
					continue;
				}
				currseqid =	SeqLocId(loc);
				if (seqid == NULL)
					seqid = currseqid;
				else
				{
					if (! SeqIdMatch(seqid, currseqid))
					{
						seqid = NULL;
						loc = NULL;
						break;
					}
				}
				loc = loc->next;
			}
            break;
        case SEQLOC_PNT:    /* pnt */
            seqid = ((SeqPntPtr)anp->data.ptrvalue)->id;
            break;
        case SEQLOC_PACKED_PNT:    /* packed pnt */
            seqid = ((PackSeqPntPtr)anp->data.ptrvalue)->id;
            break;
        default:
            break;
    }
    return seqid;
}

/*****************************************************************************
*
*   SeqLocStart(loc)
*   	returns lowest number position for Seq-loc all on one bioseq
*   	returns -1 if impossible to meet that condition
*
*****************************************************************************/
NLM_EXTERN Int4 SeqLocStart (SeqLocPtr anp)   /* seqloc */

{
    Int4 pos = -1L, tpos, numpnt;
	SeqIdPtr sip;
	SeqLocPtr slp;

    if (anp == NULL)
        return pos;

    switch (anp->choice)
    {
        case SEQLOC_BOND:   /* bond -- 2 seqs */
			if (((SeqBondPtr)(anp->data.ptrvalue))->a != NULL)
				pos =  ((SeqBondPtr)(anp->data.ptrvalue))->a->point;
			break;
        case SEQLOC_FEAT:   /* feat -- can't track yet */
        case SEQLOC_NULL:    /* NULL */
        case SEQLOC_EMPTY:    /* empty */
			break;
        case SEQLOC_WHOLE:    /* whole */
			pos = 0L;
			break;
        case SEQLOC_MIX:    /* mix -- more than one seq */
        case SEQLOC_EQUIV:    /* equiv -- ditto */
        case SEQLOC_PACKED_INT:    /* packed int */
			sip = SeqLocId(anp);
			if (sip != NULL)      /* all on one Bioseq */
			{
				slp = (SeqLocPtr)anp->data.ptrvalue;
				while (slp != NULL)
				{
					tpos = SeqLocStart(slp);
					if (pos < 0)
						pos = tpos;
					else if (tpos < pos)
						pos = tpos;
					slp = slp->next;
				}
			}
            break;
        case SEQLOC_INT:    /* int */
            pos = ((SeqIntPtr)anp->data.ptrvalue)->from;
            break;
        case SEQLOC_PNT:    /* pnt */
            pos = ((SeqPntPtr)anp->data.ptrvalue)->point;
            break;
        case SEQLOC_PACKED_PNT:    /* packed pnt */
			numpnt = PackSeqPntNum((PackSeqPntPtr)anp->data.ptrvalue);
			while (numpnt)
			{
				numpnt--;
				tpos = PackSeqPntGet((PackSeqPntPtr)anp->data.ptrvalue, numpnt);
				if (pos < 0)
					pos = tpos;
				else if (tpos < pos)
					pos = tpos;
			}
            break;
        default:
            break;
    }
    return pos;
}

/*****************************************************************************
*
*   SeqLocStop(loc)
*   	looks for highest position number on loc if on one Bioseq
*   	if fails, returns -1
*
*****************************************************************************/
NLM_EXTERN Int4 SeqLocStop (SeqLocPtr anp)   /* seqloc */

{
    BioseqPtr bsp;
    Int4 pos = -1L, tpos, numpnt;
	SeqIdPtr sip;
	SeqLocPtr slp;
	Boolean locked = FALSE;


    if (anp == NULL)
        return pos;

    switch (anp->choice)
    {
        case SEQLOC_BOND:   /* bond -- 2 seqs */
			if (((SeqBondPtr)(anp->data.ptrvalue))->b != NULL)
				pos =  ((SeqBondPtr)(anp->data.ptrvalue))->b->point;
			else if (((SeqBondPtr)(anp->data.ptrvalue))->a != NULL)
				pos =  ((SeqBondPtr)(anp->data.ptrvalue))->a->point;
			break;
        case SEQLOC_FEAT:   /* feat -- can't track yet */
        case SEQLOC_NULL:    /* NULL */
        case SEQLOC_EMPTY:    /* empty */
			break;
        case SEQLOC_WHOLE:    /* whole */
            bsp = BioseqFindCore((SeqIdPtr)anp->data.ptrvalue);
				if (bsp == NULL)
				{
					bsp = BioseqLockById((SeqIdPtr)anp->data.ptrvalue);
					if (bsp != NULL)
						locked = TRUE;
				}
            pos = BioseqGetLen(bsp) - 1;
				if (locked)
					BioseqUnlock(bsp);
			break;
        case SEQLOC_MIX:    /* mix -- more than one seq */
        case SEQLOC_EQUIV:    /* equiv -- ditto */
        case SEQLOC_PACKED_INT:    /* packed int */
			sip = SeqLocId(anp);
			if (sip != NULL)      /* all on one Bioseq */
			{
				slp = (SeqLocPtr)anp->data.ptrvalue;
				while (slp != NULL)
				{
					tpos = SeqLocStop(slp);
					if (pos < 0)
						pos = tpos;
					else if (tpos > pos)
						pos = tpos;
					slp = slp->next;
				}
			}
            break;
        case SEQLOC_INT:    /* int */
            pos = ((SeqIntPtr)anp->data.ptrvalue)->to;
            break;
        case SEQLOC_PNT:    /* pnt */
            pos = ((SeqPntPtr)anp->data.ptrvalue)->point;
            break;
        case SEQLOC_PACKED_PNT:    /* packed pnt */
			numpnt = PackSeqPntNum((PackSeqPntPtr)anp->data.ptrvalue);
			while (numpnt)
			{
				numpnt--;
				tpos = PackSeqPntGet((PackSeqPntPtr)anp->data.ptrvalue, numpnt);
				if (pos < 0)
					pos = tpos;
				else if (tpos > pos)
					pos = tpos;
			}
            break;
        default:
            break;
    }
    return pos;
}

/*****************************************************************************
*
*   SeqLocStrand(loc)
*       see objloc.h for strand value defines
*       returns Seq_strand_other when series of locs on different strands
*
*****************************************************************************/
NLM_EXTERN Uint1 SeqLocStrand (SeqLocPtr anp)   /* seqloc */

{
	SeqIdPtr sip;
	SeqLocPtr slp;
    Uint1 strand = Seq_strand_unknown, tstrand;

    if (anp == NULL)
        return strand;

    switch (anp->choice)
    {
        case SEQLOC_BOND:   /* bond -- 2 seqs */
			if (((SeqBondPtr)(anp->data.ptrvalue))->a != NULL)
				strand =  ((SeqBondPtr)(anp->data.ptrvalue))->a->strand;
			break;
        case SEQLOC_FEAT:   /* feat -- can't track yet */
        case SEQLOC_NULL:    /* NULL */
        case SEQLOC_EMPTY:    /* empty */
			break;
        case SEQLOC_WHOLE:    /* whole */
			strand = Seq_strand_both;
			break;
        case SEQLOC_MIX:    /* mix -- more than one seq */
        case SEQLOC_EQUIV:    /* equiv -- ditto */
        case SEQLOC_PACKED_INT:    /* packed int */
			sip = SeqLocId(anp);
			if (sip != NULL)      /* all on one Bioseq */
			{
				for (slp = (SeqLocPtr)anp->data.ptrvalue, 
						strand = SeqLocStrand(slp), slp = slp -> next;
						slp != NULL ; slp = slp->next)
				{
				    if (slp->choice == SEQLOC_NULL || slp->choice == SEQLOC_EMPTY) continue;
					tstrand = SeqLocStrand(slp);
					if (strand == Seq_strand_unknown && tstrand == Seq_strand_plus) {
						strand = Seq_strand_plus;
					}
					if (strand == Seq_strand_plus && tstrand == Seq_strand_unknown) {
						tstrand = Seq_strand_plus;
					}
					if (strand != tstrand)
					{
						strand = Seq_strand_other;
						break;
					}
				}
			}
            break;
        case SEQLOC_INT:    /* int */
            strand = ((SeqIntPtr)anp->data.ptrvalue)->strand;
            break;
        case SEQLOC_PNT:    /* pnt */
            strand = ((SeqPntPtr)anp->data.ptrvalue)->strand;
            break;
        case SEQLOC_PACKED_PNT:    /* packed pnt */
			strand = ((PackSeqPntPtr)anp->data.ptrvalue)->strand;
            break;
        default:
            break;
    }
    return strand;
}

/*****************************************************************************
*
*   Int4 SeqLocGetSegLens (slp, lens, ctr, gaps)
*       returns total number of segments in SeqLoc including NULLS
*       returns -1 for error
*       if lens != NULL fills with lengths of segments, 0 = NULL
*
*****************************************************************************/
NLM_EXTERN Int4 SeqLocGetSegLens (SeqLocPtr slp, Int4Ptr lens, Int4 ctr, Boolean gaps)
{
	SeqLocPtr slp2;
	BioseqPtr bsp;
	Boolean locked = FALSE;

	if (slp == NULL)
		return -1;

    switch (slp->choice)
    {
        case SEQLOC_BOND:   /* bond -- 2 seqs */
        case SEQLOC_FEAT:   /* feat -- can't track yet */
			break;
        case SEQLOC_NULL:    /* NULL */
        case SEQLOC_EMPTY:    /* empty */
			if (lens != NULL)
				lens[ctr] = 0;
			ctr++;
			break;
        case SEQLOC_WHOLE:    /* whole */
			if (gaps)
				break;
			if (lens != NULL)
			{
	            bsp = BioseqFindCore((SeqIdPtr)slp->data.ptrvalue);
					if (bsp == NULL)
					{
						bsp = BioseqLockById((SeqIdPtr)slp->data.ptrvalue);
						if (bsp != NULL)
							locked = TRUE;
					}
    	        lens[ctr] = BioseqGetLen(bsp);
				  if (locked)
						BioseqUnlock(bsp);
			}
			ctr++;
			break;
        case SEQLOC_MIX:    /* mix -- more than one seq */
        case SEQLOC_EQUIV:    /* equiv -- ditto */
        case SEQLOC_PACKED_INT:    /* packed int */
			slp2 = (SeqLocPtr)slp->data.ptrvalue;
			while (slp2 != NULL)
			{
				ctr = SeqLocGetSegLens(slp2, lens, ctr, gaps);
				slp2 = slp2->next;
			}
            break;
        case SEQLOC_INT:    /* int */
			if (gaps) break;
			if (lens != NULL)
            	lens[ctr] = ((SeqIntPtr)slp->data.ptrvalue)->to - ((SeqIntPtr)slp->data.ptrvalue)->from + 1;
			ctr++;
            break;
        case SEQLOC_PNT:    /* pnt */
			if (gaps) break;
			if (lens != NULL)
	            lens[ctr] = 1;
			ctr++;
            break;
        case SEQLOC_PACKED_PNT:    /* packed pnt */
			if (gaps) break;
			if (lens != NULL)
				lens[ctr] = SeqLocStop(slp) - SeqLocStart(slp) + 1;
			ctr++;
            break;
        default:
            break;
    }
    return ctr;
}

/*****************************************************************************
*
*   SeqLocLen(loc)
*   	returns total length in residues of loc
*   	if fails, returns -1
*
*****************************************************************************/
NLM_EXTERN Int4 SeqLocLen (SeqLocPtr anp)   /* seqloc */

{
    BioseqPtr bsp;
    Int4 len = -1L, tmp;
	SeqLocPtr slp;
	Boolean locked = FALSE;
	Boolean average = FALSE;
    Int2 num;
    SeqIdPtr sip;
    Int4 gi;
    SeqMgrPtr smp;
    SeqLenLookupFunc func;


    if (anp == NULL)
        return len;

    switch (anp->choice)
    {
        case SEQLOC_BOND:   /* bond -- 2 seqs */
        case SEQLOC_FEAT:   /* feat -- can't track yet */
			break;
        case SEQLOC_NULL:    /* NULL */
        case SEQLOC_EMPTY:    /* empty */
			len = 0;
			break;
        case SEQLOC_WHOLE:    /* whole */
			sip = (SeqIdPtr) anp->data.ptrvalue;
			bsp = BioseqFindCore(sip);
			if (bsp == NULL) {
				if (sip != NULL && sip->choice == SEQID_GI) {
					gi = (Int4) sip->data.intvalue;
					/* try registered service for rapid length lookup */
					smp = SeqMgrWriteLock ();
					if (smp != NULL) {
						func = smp->seq_len_lookup_func;
						SeqMgrUnlock ();
						if (func != NULL) {
							len = (*func) (gi);
							if (len > 0) break;
						}
					}
				}
				bsp = BioseqLockById(sip);
				if (bsp != NULL)
					locked = TRUE;
			}
			len = BioseqGetLen(bsp);
			if (locked)
				BioseqUnlock(bsp);
			break;
        case SEQLOC_EQUIV:    /* equiv -- ditto */
        	average = TRUE;
        case SEQLOC_MIX:    /* mix -- more than one seq */
        case SEQLOC_PACKED_INT:    /* packed int */
			slp = (SeqLocPtr)anp->data.ptrvalue;
			len = 0;
			num = 0;
			while (slp != NULL)
			{
				tmp = SeqLocLen(slp);
				if (tmp == -1)
					return -1;
				len += tmp;
				num++;
				slp = slp->next;
			}
			if (average) {
				len /= num;
			}
            break;
        case SEQLOC_INT:    /* int */
            len = ((SeqIntPtr)anp->data.ptrvalue)->to - ((SeqIntPtr)anp->data.ptrvalue)->from + 1;
            break;
        case SEQLOC_PNT:    /* pnt */
            len = 1;
            break;
        case SEQLOC_PACKED_PNT:    /* packed pnt */
			len = SeqLocStop(anp) - SeqLocStart(anp) + 1;
            break;
        default:
            break;
    }
    return len;
}

/*****************************************************************************
*
*   SeqLocRevCmp(loc)
*   	reverse complements a SeqLoc
*   	NO Check to be sure its on a nucleic acid
*
*****************************************************************************/
NLM_EXTERN Boolean SeqLocRevCmp (SeqLocPtr anp)   /* seqloc */

{
	SeqLocPtr slp, first, curr, prev;
	SeqPntPtr spp;


    if (anp == NULL)
        return FALSE;

    switch (anp->choice)
    {
        case SEQLOC_BOND:   /* bond -- 2 seqs */
			spp = ((SeqBondPtr)anp->data.ptrvalue)->a;
			spp->strand = StrandCmp(spp->strand);
			spp = ((SeqBondPtr)anp->data.ptrvalue)->b;
			if (spp != NULL)
				spp->strand = StrandCmp(spp->strand);
			break;
        case SEQLOC_FEAT:   /* feat -- can't track yet */
        case SEQLOC_NULL:    /* NULL */
        case SEQLOC_EMPTY:    /* empty */
        case SEQLOC_WHOLE:    /* whole */
			break;
        case SEQLOC_MIX:    /* mix -- more than one seq */
        case SEQLOC_EQUIV:    /* equiv -- ditto */
        case SEQLOC_PACKED_INT:    /* packed int */
			slp = (SeqLocPtr)anp->data.ptrvalue;
			while (slp != NULL)
			{
				SeqLocRevCmp(slp);     /* RevCmp subparts */
				slp = slp->next;
			}
			first = NULL;
			curr = NULL;
			prev = (SeqLocPtr)anp->data.ptrvalue;
			while (prev != NULL)  /* reverse order of parts */
			{					  /* no effect on meaning of SEQLOC_EQUIV */
				slp = (SeqLocPtr)anp->data.ptrvalue;
				prev = NULL;
				while (slp->next != NULL)
				{
					prev = slp;
					slp = slp->next;
				}
				if (prev != NULL) 
		   			prev->next = NULL;
				if (first == NULL)
					first = slp;
				else
					curr->next = slp;
				slp->next = NULL;
				curr = slp;
			}
			anp->data.ptrvalue = first;
            break;
        case SEQLOC_INT:    /* int */
            ((SeqIntPtr)anp->data.ptrvalue)->strand = StrandCmp(((SeqIntPtr)anp->data.ptrvalue)->strand);
            break;
        case SEQLOC_PNT:    /* pnt */
            ((SeqPntPtr)anp->data.ptrvalue)->strand = StrandCmp(((SeqPntPtr)anp->data.ptrvalue)->strand);
            break;
        case SEQLOC_PACKED_PNT:    /* packed pnt */
            ((PackSeqPntPtr)anp->data.ptrvalue)->strand = StrandCmp(((PackSeqPntPtr)anp->data.ptrvalue)->strand);
            break;
        default:
            return FALSE;
    }
    return TRUE;
}

/*****************************************************************************
*
*   Uint1 StrandCmp(strand)
*   	returns the complement of a Strand
*
*****************************************************************************/
NLM_EXTERN Uint1 StrandCmp (Uint1 strand)

{
	switch(strand)
	{
		case Seq_strand_unknown:     /* default to plus for this */
		case Seq_strand_plus:
			return (Uint1) Seq_strand_minus;
		case Seq_strand_minus:
			return (Uint1) Seq_strand_plus;
		case Seq_strand_both:
			return (Uint1) Seq_strand_both_rev;
		case Seq_strand_both_rev:
			return (Uint1) Seq_strand_both;
	}
	return strand;
}

/*****************************************************************************
*
*   SeqLocCompare(a, b)
*   	returns
*   	0 = no overlap
*   	1 = a is completely contained in b
*   	2 = b is completely contained in a
*   	3 = a == b
*   	4 = a and b overlap, but neither completely contained in the other
*   
*
*****************************************************************************/
NLM_EXTERN Int2 SeqLocCompare (SeqLocPtr a, SeqLocPtr b)   /* seqloc */

{
    BioseqPtr bsp;
    Int4 len = -1L, i, j, num, num2, point, hits;
	SeqLocPtr slp, slp2;
	ValNode tmp;
	SeqBondPtr sbp;
	SeqIntPtr sip, sip2;
	SeqIdPtr sidp;
	PackSeqPntPtr pspp, pspp2;
	Boolean got_one, missed_one, locked = FALSE;
	Int2 retval = SLC_NO_MATCH,
		 retval2 = SLC_NO_MATCH;
	static Uint1 rettable [5][5] = {      /* for developing return values */
		{ 0,4,2,2,4 } ,					  /* when a is longer than b */
		{ 4,1,4,1,4 } ,
		{ 2,4,2,2,4 } ,
		{ 2,1,2,3,4 } ,
		{ 4,4,4,4,4 }};
	static Uint1 rettable2 [5][5] = {	  /* for developing return values */
		{ 0,1,4,1,4 } ,					  /* when b is longer than a */
		{ 1,1,4,1,1 } ,
		{ 4,4,2,2,4 } ,
		{ 1,1,4,3,4 } ,
		{ 4,1,4,4,4 }};

    if ((a == NULL) || (b == NULL))
        return retval;

    switch (a->choice)
    {
        case SEQLOC_MIX:    /* mix -- more than one seq */
        case SEQLOC_EQUIV:    /* equiv -- ditto */
        case SEQLOC_PACKED_INT:    /* packed int */
			if ((b->choice == SEQLOC_MIX) ||  /* check for identity */
				(b->choice == SEQLOC_EQUIV) ||
				(b->choice == SEQLOC_PACKED_INT))
			{
				got_one = FALSE;   /* for any overlap */
				slp = (SeqLocPtr)a->data.ptrvalue;  /* check for identity */
				slp2 = (SeqLocPtr)b->data.ptrvalue;
				retval = SeqLocCompare(slp, slp2);
				slp = slp->next;
				slp2 = slp2->next;
				while ((slp != NULL) && (slp2 != NULL) && (retval == SLC_A_EQ_B))
				{
					retval = SeqLocCompare(slp, slp2);
					slp = slp->next;
					slp2 = slp2->next;
				}
				if ((slp == NULL) && (slp2 == NULL) && (retval == SLC_A_EQ_B))
					return retval;

				slp = (SeqLocPtr)a->data.ptrvalue;	/* check for a in b */
				slp2 = (SeqLocPtr)b->data.ptrvalue;
				while ((slp != NULL) && (slp2 != NULL))
				{
					retval2 = SeqLocCompare(slp, slp2);
					if (retval2 > SLC_NO_MATCH)
						got_one = TRUE;
					switch (retval2)
					{
						case SLC_NO_MATCH:
							slp2 = slp2->next;
							break;
						case SLC_A_EQ_B:
							slp2 = slp2->next;
							slp = slp->next;
							break;
						case SLC_A_IN_B:
							slp = slp->next;
							break;
						case SLC_B_IN_A:
						case SLC_A_OVERLAP_B:
							slp2 = NULL;
							break;
					}
				}
				if (slp == NULL)    /* a all in b */
					return SLC_A_IN_B;

				slp2 = (SeqLocPtr)a->data.ptrvalue;	/* check for b in a */
				slp = (SeqLocPtr)b->data.ptrvalue;
				while ((slp != NULL) && (slp2 != NULL))
				{
					retval2 = SeqLocCompare(slp, slp2);
					if (retval2 > SLC_NO_MATCH)
						got_one = TRUE;
					switch (retval2)
					{
						case SLC_NO_MATCH:
							slp2 = slp2->next;
							break;
						case SLC_A_EQ_B:
							slp2 = slp2->next;
							slp = slp->next;
							break;
						case SLC_A_IN_B:
							slp = slp->next;
							break;
						case SLC_B_IN_A:
						case SLC_A_OVERLAP_B:
							slp2 = NULL;
							break;
					}
				}
				if (slp == NULL)    /* a all in b */
					return SLC_B_IN_A;

				if (got_one)
					return SLC_A_OVERLAP_B;
			}

			slp = (SeqLocPtr)a->data.ptrvalue; /* check for any overlap */
			retval = SeqLocCompare(slp, b);
			slp = slp->next;
			while (slp != NULL)
			{
				retval2 = SeqLocCompare(slp, b);
				retval = (Int2) rettable[retval][retval2];
				slp = slp->next;
			}
            return retval;
		default:
			break;
	}
    switch (b->choice)
    {
        case SEQLOC_MIX:    /* mix -- more than one seq */
		case SEQLOC_EQUIV:    /* equiv -- ditto */
        case SEQLOC_PACKED_INT:    /* packed int */
			slp = (SeqLocPtr)b->data.ptrvalue;
			retval = SeqLocCompare(a, slp);
			slp = slp->next;
			while (slp != NULL)
			{
				retval2 = SeqLocCompare(a, slp);
				retval = (Int2)rettable2[retval][retval2];
				slp = slp->next;
			}
		    return retval;
		default:
			break;
	}

	tmp.next = NULL;
	switch (a->choice)
	{
        case SEQLOC_NULL:    /* NULL, can't match */
			if (b->choice == SEQLOC_NULL)
				retval = SLC_A_EQ_B;
			break;
        case SEQLOC_FEAT:   /* feat -- can't track yet */
			break;
        case SEQLOC_EMPTY:    /* empty */
			if (b->choice == SEQLOC_EMPTY)
			{
				if (SeqIdForSameBioseq((SeqIdPtr)a->data.ptrvalue, (SeqIdPtr)b->data.ptrvalue))
					retval = SLC_A_EQ_B;
			}
			break;
        case SEQLOC_BOND:   /* bond -- 2 seqs */
			sbp = (SeqBondPtr)a->data.ptrvalue;
			tmp.choice = SEQLOC_PNT;    /* check the points */
			tmp.data.ptrvalue = (Pointer)sbp->a;
			retval = SeqLocCompare(&tmp, b);
			if (sbp->b != NULL)
			{
				tmp.data.ptrvalue = (Pointer)sbp->b;
				retval2 = SeqLocCompare(&tmp, b);
				retval = (Int2) rettable[retval][retval2];
			}
			break;
        case SEQLOC_WHOLE:    /* whole */
			sidp = (SeqIdPtr)a->data.ptrvalue;
			switch (b->choice)
			{
		        case SEQLOC_BOND:   /* bond -- 2 seqs */
					sbp = (SeqBondPtr)b->data.ptrvalue;
					if (SeqIdForSameBioseq(sbp->a->id, sidp))
						retval = SLC_B_IN_A;
					if (sbp->b != NULL)
					{
						if (SeqIdForSameBioseq(sbp->b->id, sidp))
							retval2 = SLC_B_IN_A;
						retval = (Int2) rettable[retval][retval2];
					}
					break;
        		case SEQLOC_WHOLE:    /* whole */
					if (SeqIdForSameBioseq(sidp, (SeqIdPtr)b->data.ptrvalue))
						retval = SLC_A_EQ_B;
					break;
        		case SEQLOC_INT:    /* int */
					sip = (SeqIntPtr)b->data.ptrvalue;
					if (SeqIdForSameBioseq(sidp, sip->id))
					{
						retval = SLC_B_IN_A;
            		bsp = BioseqFindCore(sidp);
						if (bsp == NULL)
						{
							bsp = BioseqLockById(sidp);
							if (bsp != NULL)
								locked = TRUE;
						}
						if (bsp != NULL)
						{
						    len = BioseqGetLen(bsp);
							if ((sip->from == 0) && (sip->to == (len - 1)))
								retval = SLC_A_EQ_B;
						}
						if (locked)
							BioseqUnlock(bsp);
					}
		            break;
        		case SEQLOC_PNT:    /* pnt */
					if (SeqIdForSameBioseq(sidp, ((SeqPntPtr)b->data.ptrvalue)->id))
						retval = SLC_B_IN_A;
		            break;
        		case SEQLOC_PACKED_PNT:    /* packed pnt */
					if (SeqIdForSameBioseq(sidp, ((PackSeqPntPtr)b->data.ptrvalue)->id))
						retval = SLC_B_IN_A;
		            break;
        		default:
		            break;
			}
			break;
        case SEQLOC_INT:    /* int */
			sip = (SeqIntPtr)a->data.ptrvalue;
			sidp = sip->id;
			switch (b->choice)
			{
		        case SEQLOC_BOND:   /* bond -- 2 seqs */
					sbp = (SeqBondPtr)b->data.ptrvalue;
					if (SeqIdForSameBioseq(sbp->a->id, sidp))
					{
						if ((sip->from <= sbp->a->point) &&
							(sip->to >= sbp->a->point))
							retval = SLC_B_IN_A;
					}
					if (sbp->b != NULL)
					{
						if (SeqIdForSameBioseq(sbp->b->id, sidp))
						{
							if ((sip->from <= sbp->b->point) &&
								(sip->to >= sbp->b->point))
								retval = SLC_B_IN_A;
						}
						retval = (Int2) rettable[retval][retval2];
					}
					break;
        		case SEQLOC_WHOLE:    /* whole */
					if (SeqIdForSameBioseq(sidp, (SeqIdPtr)b->data.ptrvalue))
					{
						retval = SLC_A_IN_B;
						bsp = BioseqFindCore((SeqIdPtr)b->data.ptrvalue);
						if (bsp == NULL)
						{
							bsp = BioseqLockById((SeqIdPtr)b->data.ptrvalue);
							if (bsp != NULL)
								locked = TRUE;
						}
						if (bsp != NULL)
						{
							len = BioseqGetLen(bsp);
							if ((sip->from == 0) && (sip->to == (len - 1)))
								retval = SLC_A_EQ_B;
						}
						if (locked)
							BioseqUnlock(bsp);
					}
					break;
        		case SEQLOC_INT:    /* int */
					sip2 = (SeqIntPtr)b->data.ptrvalue;
					if (SeqIdForSameBioseq(sidp, sip2->id))
					{
						if ((sip->from == sip2->from) && (sip->to == sip2->to))
							retval = SLC_A_EQ_B;
						else if ((sip->from <= sip2->from) && (sip->to >= sip2->to))
							retval = SLC_B_IN_A;
						else if ((sip->from >= sip2->from) && (sip->to <= sip2->to))
							retval = SLC_A_IN_B;
						else if ((sip->from >= sip2->from) && (sip->from <= sip2->to))
							retval = SLC_A_OVERLAP_B;
						else if ((sip->to >= sip2->from) && (sip->to <= sip2->to))
							retval = SLC_A_OVERLAP_B;
					}
		            break;
        		case SEQLOC_PNT:    /* pnt */
					if (SeqIdForSameBioseq(sidp, ((SeqPntPtr)b->data.ptrvalue)->id))
					{
						point = ((SeqPntPtr)b->data.ptrvalue)->point;
						if ((point >= sip->from) && (point <= sip->to))
							retval = SLC_B_IN_A;
					}
		            break;
        		case SEQLOC_PACKED_PNT:    /* packed pnt */
					pspp = (PackSeqPntPtr)b->data.ptrvalue;
					if (SeqIdForSameBioseq(sidp, pspp->id))
					{
						got_one = FALSE;
						missed_one = FALSE;
						num = PackSeqPntNum(pspp);
						for (i = 0; i < num; i++)
						{
							point = PackSeqPntGet(pspp, i);
							if ((point < sip->from) || (point > sip->to))
							{
								missed_one = TRUE;
								if (got_one)
									i = num;
							}
							else
							{
								got_one = TRUE;
								if (missed_one)
									i = num;
							}
						}
						if (got_one)
						{
							if (missed_one)
								retval = SLC_A_OVERLAP_B;
							else
								retval = SLC_B_IN_A;
						}
					}
		            break;
        		default:
		            break;
			}
            break;
        case SEQLOC_PNT:    /* pnt */
			sidp = ((SeqPntPtr)a->data.ptrvalue)->id;
			point = ((SeqPntPtr)a->data.ptrvalue)->point;
			switch (b->choice)
			{
		        case SEQLOC_BOND:   /* bond -- 2 seqs */
					sbp = (SeqBondPtr)b->data.ptrvalue;
					if (SeqIdForSameBioseq(sbp->a->id, sidp))
					{
						if (point == sbp->a->point)
							retval = SLC_A_EQ_B;
					}
					if (sbp->b != NULL)
					{
						if (SeqIdForSameBioseq(sbp->b->id, sidp))
						{
							if (point == sbp->b->point)
								retval = SLC_A_EQ_B;
						}
						retval = (Int2) rettable[retval][retval2];
					}
					break;
        		case SEQLOC_WHOLE:    /* whole */
					if (SeqIdForSameBioseq(sidp, (SeqIdPtr)b->data.ptrvalue))
						retval = SLC_A_IN_B;
					break;
        		case SEQLOC_INT:    /* int */
					sip2 = (SeqIntPtr)b->data.ptrvalue;
					if (SeqIdForSameBioseq(sidp, sip2->id))
					{
						if ((point == sip2->from) && (point == sip2->to))
							retval = SLC_A_EQ_B;
						else if ((point >= sip2->from) && (point <= sip2->to))
							retval = SLC_A_IN_B;
					}
		            break;
        		case SEQLOC_PNT:    /* pnt */
					if (SeqIdForSameBioseq(sidp, ((SeqPntPtr)b->data.ptrvalue)->id))
					{
						if (point == ((SeqPntPtr)b->data.ptrvalue)->point)
							retval = SLC_A_EQ_B;
					}
		            break;
        		case SEQLOC_PACKED_PNT:    /* packed pnt */
					pspp = (PackSeqPntPtr)b->data.ptrvalue;
					if (SeqIdForSameBioseq(sidp, pspp->id))
					{
						num = PackSeqPntNum(pspp);
						for (i = 0; i < num; i++)
						{
							if (point == PackSeqPntGet(pspp, i))
							{
								retval = SLC_A_IN_B;
								i = num;     /* only check one */
							}
						}
					}
		            break;
        		default:
		            break;
			}
            break;
        case SEQLOC_PACKED_PNT:    /* packed pnt */
			pspp = (PackSeqPntPtr)a->data.ptrvalue;
			num = PackSeqPntNum(pspp);
			sidp = pspp->id;
			switch (b->choice)
			{
		        case SEQLOC_BOND:   /* bond -- 2 seqs */
					sbp = (SeqBondPtr)b->data.ptrvalue;
					if (SeqIdForSameBioseq(sbp->a->id, sidp))
					{
						point = sbp->a->point;
						for (i = 0; i < num; i++)
						{
							if (point == PackSeqPntGet(pspp, i))
							{
								retval = SLC_B_IN_A;
								i = num;
							}
						}
					}
					if (sbp->b != NULL)
					{
						if (SeqIdForSameBioseq(sbp->b->id, sidp))
						{
							point = sbp->b->point;
							for (i = 0; i < num; i++)
							{
								if (point == PackSeqPntGet(pspp, i))
								{
									if (retval != SLC_B_IN_A)
										retval = SLC_A_OVERLAP_B;
									i = num + 1;
								}
							}
							if ((i != num) && (retval == SLC_B_IN_A))
								retval = SLC_A_OVERLAP_B;
						}
					}
					break;
        		case SEQLOC_WHOLE:    /* whole */
					if (SeqIdForSameBioseq(sidp, (SeqIdPtr)b->data.ptrvalue))
						retval = SLC_A_IN_B;
					break;
        		case SEQLOC_INT:    /* int */
					sip = (SeqIntPtr)b->data.ptrvalue;
					if (SeqIdForSameBioseq(sidp, sip->id))
					{
						got_one = FALSE;
						missed_one = FALSE;
						for (i = 0; i < num; i++)
						{
							point = PackSeqPntGet(pspp, i);
							if ((point < sip->from) || (point > sip->to))
							{
								missed_one = TRUE;
								if (got_one)
									i = num + 1;
							}
							else
							{
								got_one = TRUE;
								if (missed_one)
									i = num + 1;
							}
						}
						if (got_one)
						{
							if (missed_one)
								retval = SLC_A_OVERLAP_B;
							else
								retval = SLC_A_IN_B;
						}
					}
		            break;
        		case SEQLOC_PNT:    /* pnt */
					if (SeqIdForSameBioseq(sidp, ((SeqPntPtr)b->data.ptrvalue)->id))
					{
						point = ((SeqPntPtr)b->data.ptrvalue)->point;
						for (i = 0; i < num; i++)
						{
							if (point == PackSeqPntGet(pspp, i))
							{
								retval = SLC_B_IN_A;
								i = num + 1;
							}
						}
					}
		            break;
        		case SEQLOC_PACKED_PNT:    /* packed pnt */
					pspp2 = (PackSeqPntPtr)b->data.ptrvalue;
					if (SeqIdForSameBioseq(sidp, pspp->id))
					{
						num2 = PackSeqPntNum(pspp2);
						if (num == num2)   /* check for identity */
						{
							for (i = 0; i < num; i++)
							{
								if ( PackSeqPntGet(pspp, i) !=
									 PackSeqPntGet(pspp2, i))
									i = num + 1;
							}
							if (i == num)
								retval = SLC_A_EQ_B;
						}
						if (retval != SLC_A_EQ_B)
						{
							hits = 0;
							for (i = 0; i < num; i++)
							{
								point = PackSeqPntGet(pspp, i);
								for (j = 0; j < num2; j++)
								{
									if (point == PackSeqPntGet(pspp2, j))
										hits++;
								}
							}
							if (hits == num)
								retval = SLC_A_IN_B;
							else if (hits == num2)
								retval = SLC_B_IN_A;
						}
					}
		            break;
        		default:
		            break;
			}
            break;
        default:
            break;
    }
    return retval;
}

/*****************************************************************************
*
*   SeqLocAinB(a, b)
*      if a is completely contained in b, a positive number is returned
*         if 0, a is identical with b
*         if not 0, is the number of residues bigger b is than a
*      if a negative number is returned, a is not contained in b
*         could overlap or not
*      used to find features contained in genes
*
*****************************************************************************/
NLM_EXTERN Int4 SeqLocAinB (SeqLocPtr a, SeqLocPtr b)
{
	Int4 diff = -1;
	Int2 res;

	if ((a == NULL) || (b == NULL))
		return diff;

	res = SeqLocCompare(a, b);
	switch (res)
	{
		case SLC_A_EQ_B:
			diff = 0;
			break;
		case SLC_A_IN_B:
			diff = (SeqLocLen(b) - SeqLocLen(a));
			break;
		default:
			break;
	}
	return diff;
}

/*****************************************************************************
*
*   Boolean SeqIntCheck(sip)
*       checks that a seq interval is valid
*
*****************************************************************************/
NLM_EXTERN Boolean SeqIntCheck (SeqIntPtr sip)

{
    Int4 len = INT4_MAX;
    BioseqPtr bsp;
	 Boolean locked = FALSE;

	if (sip == NULL) return TRUE;  /* makes it ok to pass a NULL */
	
    bsp = BioseqFindCore(sip->id);
	 if (bsp == NULL)
	 {
	 	bsp = BioseqLockById(sip->id);
	 	if (bsp != NULL)
	 		locked = TRUE;
	 }
    if (bsp != NULL)
        len = BioseqGetLen(bsp);

    if (locked)
		BioseqUnlock(bsp);
    if ((sip->from < 0) || (sip->from > sip->to) || (sip->to >= len))
    {
        return FALSE;
    }
    else
        return TRUE;
}

/*****************************************************************************
*
*   Boolean SeqPntCheck(SeqPntPtr spp)
*       checks that a seq point is valid
*
*****************************************************************************/
NLM_EXTERN Boolean SeqPntCheck (SeqPntPtr spp)

{
    Int4 len = INT4_MAX;
    BioseqPtr bsp;
	 Boolean locked = FALSE;

	if (spp == NULL) return TRUE;   /* cant compare */
	
    bsp = BioseqFindCore(spp->id);
	 if (bsp == NULL)
	 {
	 	bsp = BioseqLockById(spp->id);
	 	if (bsp != NULL)
	 		locked = TRUE;
	 }
    if (bsp != NULL)
        len = BioseqGetLen(bsp);

	 if (locked)
		BioseqUnlock(bsp);
    if ((spp->point < 0) || (spp->point >= len))
    {
        return FALSE;
    }
    else
        return TRUE;
}

/*****************************************************************************
*
*   PackSeqPntCheck (pspp)
*
*****************************************************************************/
NLM_EXTERN Boolean PackSeqPntCheck (PackSeqPntPtr pspp)
{
	Int4 len = INT4_MAX;
	BioseqPtr bsp;
	Int4 num, index, point;
	Boolean locked = FALSE;

	if (pspp == NULL) return TRUE;   /* cant compare */
	
    bsp = BioseqFindCore(pspp->id);
	 if (bsp == NULL)
	 {
	 	bsp = BioseqLockById(pspp->id);
	 	if (bsp != NULL)
	 		locked = TRUE;
	 }
    if (bsp != NULL)
        len = BioseqGetLen(bsp);

	 if (locked)
		BioseqUnlock(bsp);
	num = PackSeqPntNum(pspp);   /* total number of points */
    for (index = 0; index < num; index++)
    {
        point = PackSeqPntGet(pspp, index);

	    if ((point < 0) || (point >= len))
        	return FALSE;
	}

    return TRUE;

}


/*****************************************************************************
*
*   SeqLocCheck (slp)
*
*****************************************************************************/
NLM_EXTERN Uint1 SeqLocCheck (SeqLocPtr slp)
{
	SeqLocPtr tmp;
	Uint1 thisstrand, laststrand=0;
	Boolean first = TRUE;
	Uint1 retval = SEQLOCCHECK_OK;

	if (slp == NULL) return TRUE;

	tmp = NULL;
	while ((tmp = SeqLocFindNext(slp, tmp)) != NULL)
	{
		thisstrand = SeqLocStrand(tmp);
		if (! first)
		{
			if (thisstrand != laststrand)
			{
				ErrPostEx(SEV_WARNING,0,0,"Mixed strand location");
				retval = SEQLOCCHECK_WARNING;
			}
			first = FALSE;
		}
		laststrand = thisstrand;

		switch (tmp->choice)
		{
			case SEQLOC_INT:
				if (! SeqIntCheck ((SeqIntPtr)(tmp->data.ptrvalue)))
					return SEQLOCCHECK_ERROR;
				break;
			case SEQLOC_PNT:
				if (! SeqPntCheck ((SeqPntPtr)(tmp->data.ptrvalue)))
					return SEQLOCCHECK_ERROR;
				break;
			case SEQLOC_PACKED_PNT:
				if (! PackSeqPntCheck ((PackSeqPntPtr)(tmp->data.ptrvalue)))
					return SEQLOCCHECK_ERROR;
				break;
			default:
				break;
		}
	}

	return retval;
}


/*****************************************************************************
*
*   SeqLocPartialCheck(head)
*       sets bits for incomplete location and/or errors
*       incomplete defined as Int-fuzz on start or stop with
*         lim.unk, lim.gt, or lim.lt set
*   
*   returns defined in header file
*   
*****************************************************************************/
NLM_EXTERN Uint2 SeqLocPartialCheck(SeqLocPtr head)
{
	SeqLocPtr slp = NULL, first = NULL, last = NULL;
	Uint2 retval = 0;
	BioseqPtr bsp;
	SeqIntPtr sip;
	SeqPntPtr spp;
	PackSeqPntPtr pspp;
	IntFuzzPtr ifp;
	Boolean miss_end;
	BioseqContextPtr bcp;
	ValNodePtr vnp, vnp2;
	Boolean locked, found_molinfo;
	MolInfoPtr mip;

	if (head == NULL) return retval;

	while ((slp = SeqLocFindNext(head, slp)) != NULL)
	{
		if (first == NULL)
			first = slp;
		last = slp;
	}

	if (first == NULL) return retval;

	slp = NULL;
	while ((slp = SeqLocFindNext(head, slp)) != NULL)
	{
		switch (slp->choice)
		{
			case SEQLOC_NULL:
				if (slp == first)
					retval |= SLP_START;
				else if (slp == last)
					retval |= SLP_STOP;
				else
					retval |= SLP_INTERNAL;
				break;
			case SEQLOC_INT:
				sip = (SeqIntPtr)(slp->data.ptrvalue);
				ifp = sip->if_from;
				if (ifp != NULL)
				{
					if (ifp->choice == 4)  /* lim */
					{
						if (ifp->a == 1)       /* gt */
							retval |= SLP_LIM_WRONG;
						else if ((ifp->a == 2) || (ifp->a == 0)) /* lt,unk */
						{
							if (sip->strand == Seq_strand_minus) /* stop */
							{
								if (slp == last)
									retval |= SLP_STOP;
								else
									retval |= SLP_INTERNAL;
								if (sip->from != 0)
								{
									if (slp == last)
										retval |= SLP_NOSTOP;
									else
										retval |= SLP_NOINTERNAL;
								}
							}
							else                                /* start */
							{
								if (slp == first)
									retval |= SLP_START;
								else
									retval |= SLP_INTERNAL;
								if (sip->from != 0)
								{
									if (slp == first)
										retval |= SLP_NOSTART;
									else
										retval |= SLP_NOINTERNAL;
								}
							}
						}
					}

				}
				ifp = sip->if_to;
				if (ifp != NULL)
				{
					if (ifp->choice == 4)  /* lim */
					{
						if (ifp->a == 2)       /* lt */
							retval |= SLP_LIM_WRONG;
						else if ((ifp->a == 1) || (ifp->a == 0)) /* gt,unk */
						{
							locked = FALSE;
							bsp = BioseqFindCore(sip->id);
							 if (bsp == NULL)
							 {
							 	bsp = BioseqLockById(sip->id);
							 	if (bsp != NULL)
							 		locked = TRUE;
							 }
							miss_end = FALSE;
							if (bsp != NULL)
							{
								if (sip->to != (bsp->length - 1))
									miss_end = TRUE;
							}
							if (locked)
								BioseqUnlock(bsp);
							if (sip->strand == Seq_strand_minus) /* start */
							{
								if (slp == first)
									retval |= SLP_START;
								else
									retval |= SLP_INTERNAL;
								if (miss_end)
								{
									if (slp == first /* was last */)
										retval |= SLP_NOSTART;
									else
										retval |= SLP_NOINTERNAL;
								}
							}
							else                                /* stop */
							{
								if (slp == last)
									retval |= SLP_STOP;
								else
									retval |= SLP_INTERNAL;
								if (miss_end)
								{
									if (slp == last)
										retval |= SLP_NOSTOP;
									else
										retval |= SLP_NOINTERNAL;
								}
							}
						}
					}
				}
				break;
			case SEQLOC_PNT:
				spp = (SeqPntPtr)(slp->data.ptrvalue);
				ifp = spp->fuzz;
				if (ifp != NULL)
				{
					if (ifp->choice == 4)  /* lim */
					{
						if ((ifp->a >= 0) && (ifp->a <= 2))  /* gt, lt,unk */
						{
							if (slp == first)
								retval |= SLP_START;
							if (slp == last)
								retval |= SLP_STOP;
							if ((slp != first) && (slp != last))
								retval |= SLP_INTERNAL;
						}
					}
				}
				break;
			case SEQLOC_PACKED_PNT:
				pspp = (PackSeqPntPtr)(slp->data.ptrvalue);
				ifp = pspp->fuzz;
				if (ifp != NULL)
				{
					if (ifp->choice == 4)  /* lim */
					{
						if ((ifp->a >= 0) && (ifp->a <= 2)) /* gt, lt, unk */
						{
							if (slp == first)
								retval |= SLP_START;
							if (slp == last)
								retval |= SLP_STOP;
							if ((slp != first) && (slp != last))
								retval |= SLP_INTERNAL;
						}
					}
				}
				break;
			case SEQLOC_WHOLE:
				found_molinfo = FALSE;
				locked = FALSE;
				bsp = BioseqFindCore((SeqIdPtr)(slp->data.ptrvalue));
				if (bsp == NULL)
				{
					bsp = BioseqLockById((SeqIdPtr)(slp->data.ptrvalue));
					if (bsp != NULL)
						locked = TRUE;
				}
				if (bsp == NULL) break;
				bcp = BioseqContextNew(bsp);
				if (bcp != NULL) {
					vnp = NULL;
					while ((vnp = BioseqContextGetSeqDescr(bcp, Seq_descr_molinfo, vnp, NULL)) != NULL)
					{
						found_molinfo = TRUE;
						mip = (MolInfoPtr)(vnp->data.ptrvalue);
						switch (mip->completeness)
						{
							case 3:	/* no left */
								if (slp == first)
									retval |= SLP_START;
								else
									retval |= SLP_INTERNAL;
								break;
							case 4:    /* no right */
								if (slp == last)
									retval |= SLP_STOP;
								else
									retval |= SLP_INTERNAL;
								break;
							case 2:    /* partial */
								retval |= SLP_OTHER;
								break;
							case 5:    /* no ends */
								retval |= SLP_START;
								retval |= SLP_STOP;
								break;
							default:
								break;
						}
					}
					if (! found_molinfo)
					{
						while ((vnp = BioseqContextGetSeqDescr(bcp, Seq_descr_modif, vnp, NULL)) != NULL)
						{
							for (vnp2 = (ValNodePtr)(vnp->data.ptrvalue); vnp2 != NULL; vnp2 = vnp2->next)
							{
								switch (vnp2->data.intvalue)
								{
						
									case 16:	/* no left */
							
										if (slp == first)
								
											retval |= SLP_START;
									
										else
											retval |= SLP_INTERNAL;
										break;
									case 17:    /* no right */
										if (slp == last)
											retval |= SLP_STOP;
										else
											retval |= SLP_INTERNAL;
										break;
									case 10:    /* partial */
										retval |= SLP_OTHER;
										break;
								}
							}
						}
					}
					BioseqContextFree(bcp);
				}
				if (locked)
					BioseqUnlock (bsp);
				break;
			default:
				break;
			
		}
	}

	return retval;
}

/*****************************************************************************
*
*   StringForSeqMethod(Int2 method)
*   	returns a descriptive string for sequencing method.
*
*****************************************************************************/
NLM_EXTERN CharPtr StringForSeqMethod (Int2 method)
{
#define MAX_METHOD 6
	static char * methods[MAX_METHOD] = {
		"conceptual translation",
		"direct peptide sequencing",
		"conceptual translation with partial peptide sequencing",
		"sequenced peptide, ordered by overlap",
		"sequenced peptide, ordered by homology",
		"conceptual translation supplied by author" };

	if ((method < 1) || (method > MAX_METHOD))
		return NULL;

	return methods[method - 1];
}

/*****************************************************************************
*
*   StringForSeqTech(Int2 tech)
*   	returns a descriptive string for sequencing method.
*		uses MolInfo from asn spec 4.0
*****************************************************************************/
NLM_EXTERN CharPtr StringForSeqTech (Int2 tech)
{
#define MAX_TECH 13
	static char * techs[MAX_TECH] = {
		NULL,    /*"standard sequencing", */
		NULL,  /*"Expressed Sequence Tag", */
		NULL, /*"Sequence Tagged Site", */
		NULL, /*"one-pass genomic sequence", */
		NULL, /*"from genetic mapping techniques", */
		NULL, /*"from physical mapping techniques", */
		NULL, /*"derived from other data, not a primary entity", */
		"conceptual translation",
		"direct peptide sequencing",
		"conceptual translation with partial peptide sequencing",
		"sequenced peptide, ordered by overlap",
		"sequenced peptide, ordered by homology",
		"conceptual translation supplied by author" };

	if ((tech < 1) || (tech > MAX_TECH))
		return NULL;

	return techs[tech - 1];
}

static Boolean GetThePointForOffset(SeqLocPtr of, SeqPntPtr target, Uint1 which_end);
static Int4 CheckOffsetInLoc(SeqLocPtr in, Int4 pos, BioseqPtr bsp, SeqIdPtr the_id);
NLM_EXTERN Int4 CheckPointInBioseq(SeqPntPtr sp, BioseqPtr in);

/*****************************************************************************
*
* Int4 GetOffsetInLoc (SeqLocPtr of, SeqLocPtr in, Uint1 which_end)
*   returns -1 if of not in, in
*
*****************************************************************************/
NLM_EXTERN Int4 GetOffsetInLoc (SeqLocPtr of, SeqLocPtr in, Uint1 which_end)
{
	SeqPnt sp;
	BioseqPtr bsp;
	Boolean locked = FALSE;
	Int4 result;

	if ((in == NULL) || (of == NULL))
		return -1L;

	if (! GetThePointForOffset(of, &sp, which_end))
		return -1L;

	if (! IS_one_loc(in, FALSE))    /* optimize for multiple hits */
	{
		bsp = BioseqFindCore(sp.id);  /* only need SeqIds */
		 if (bsp == NULL)
		 {
	 		bsp = BioseqLockById(sp.id);
		 	if (bsp != NULL)
		 		locked = TRUE;
		 }
	}
	else
		bsp = NULL;

	result = CheckOffsetInLoc(in, sp.point, bsp, sp.id);
	if (locked)
		BioseqUnlock(bsp);
	return result;
}

/*****************************************************************************
*
* Int4 GetOffsetInBioseq (SeqLocPtr of, BioseqPtr in, Uint1 which_end)
*   return -1 if of not in "in"
*
*****************************************************************************/
NLM_EXTERN Int4 GetOffsetInBioseq (SeqLocPtr of, BioseqPtr in, Uint1 which_end)
{
	SeqPnt sp;

	if ((of == NULL) || (in == NULL))
		return -1;

	if (! GetThePointForOffset(of, &sp, which_end))
		return -1L;

	return CheckPointInBioseq(&sp, in);
}

/*****************************************************************************
*
*   CheckPointInBioseq(pnt, in)
*
*****************************************************************************/
NLM_EXTERN Int4 CheckPointInBioseq (SeqPntPtr sp, BioseqPtr in)
{
	ValNode sl;
	BioseqPtr bsp;
	Int4 retval = -1;
	SeqLocPtr slp = NULL, curr;
	Int4 offset, offset2, strt, stp;
	SeqIdPtr sip;
	Boolean locked = FALSE;

	if (SeqIdIn(sp->id, in->id))   /* in this one */
		return sp->point;

	switch (in->repr)
	{
		case Seq_repr_virtual:
		case Seq_repr_raw:
		case Seq_repr_const:
		case Seq_repr_map:
			return -1;    /* nothing more can be done */

		case Seq_repr_ref:
			slp = (ValNodePtr) in->seq_ext;
			break;

		case Seq_repr_seg:
			sl.choice = SEQLOC_MIX;
			sl.data.ptrvalue = in->seq_ext;
			slp = &sl;
			break;

		case Seq_repr_delta:
			break;

		default:
			return -1;
	}

	bsp = BioseqFindCore(sp->id);   /* only need SeqIds */
	 if (bsp == NULL)
	 {
	 	bsp = BioseqLockById(sp->id);
	 	if (bsp != NULL)
	 		locked = TRUE;
	 }
	if (in->repr == Seq_repr_seg || in->repr == Seq_repr_delta) {
		retval = SeqMgrMapPartToSegmentedBioseq (in, sp->point, bsp, sp->id);
	}
	if (retval == -1) {
		retval = CheckOffsetInLoc(slp, sp->point, bsp, sp->id);
	}

	if (locked)
		BioseqUnlock(bsp);

	if (retval >= 0) return retval;     /* got it on segments */

	                                    /* look for segmented segments */
	offset = 0;
	curr = NULL;
	while ((curr = SeqLocFindNext(slp, curr)) != NULL)
	{
		sip = SeqLocId(curr);
		if (sip != NULL)
		{
			bsp = BioseqLockById(sip);
			if (bsp != NULL)
			{
				switch (bsp->repr)
				{
					case Seq_repr_ref:   /* could have more levels */
					case Seq_repr_seg:
						offset2 = CheckPointInBioseq(sp, bsp);
						if (offset2 >= 0)   /* got it */
						{
							strt = SeqLocStart(curr);
							stp = SeqLocStop(curr);
							if ((offset2 >= strt) && (offset2 <= stp))
							{
								if (SeqLocStrand(curr) == Seq_strand_minus)
									offset2 = stp - offset2;
								else
									offset2 -= strt;
								retval = offset2 + offset;
								return retval;
							}
						}
						break;
					default:		   /* one level, already checked */
						break;
				}
				BioseqUnlock(bsp);
			}
		}
		offset += SeqLocLen(curr);
	}

	return retval;    /* all failed */
}

/*****************************************************************************
*
*   Boolean GetThePointForOffset(SeqLocPtr of, SeqPntPtr target, Uint1 which_end)
*
*****************************************************************************/
Boolean GetThePointForOffset(SeqLocPtr of, SeqPntPtr target, Uint1 which_end)
{
	SeqLocPtr tmp, pnt, first=NULL, last=NULL;
	Uint1 ofstrand;
	Boolean getstart;

	pnt = NULL;    /* get first or last single span type in "of"*/
	tmp = NULL;
	while ((pnt = SeqLocFindNext(of, pnt)) != NULL)
	{
		last = pnt;
		if (first == NULL)
			first = pnt;
	}				   /* otherwise, get last */
	if (first == NULL)
		return FALSE;
	ofstrand = SeqLocStrand(first);
	getstart = TRUE;   /* assume we are getting SeqLocStart() */
	switch (which_end)
	{
		case SEQLOC_LEFT_END:
			if (ofstrand == Seq_strand_minus)
				tmp = last;
			else
				tmp = first;
			break;
		case SEQLOC_RIGHT_END:
			if (ofstrand == Seq_strand_minus)
				tmp = first;
			else
				tmp = last;
			getstart = FALSE;
			break;
		case SEQLOC_START:
			tmp = first;
			if (ofstrand == Seq_strand_minus)
				getstart = FALSE;
			break;
		case SEQLOC_STOP:
			tmp = last;
			if (ofstrand != Seq_strand_minus)
				getstart = FALSE;
			break;
		default:
			return FALSE;   /* error */
	}

	/* SeqLocStart returns 'from', and SeqLocStop returns 'to', regardless of strand! */

	if (getstart)
		target->point = SeqLocStart(tmp);
	else
		target->point = SeqLocStop(tmp);
	target->id = SeqLocId(tmp);

	if ((target->point < 0) || (target->id == NULL))
		return FALSE;

	return TRUE;
}

/*****************************************************************************
*
*   CheckOffsetInLoc()
*
*****************************************************************************/
static Int4 CheckOffsetInLoc(SeqLocPtr in, Int4 pos, BioseqPtr bsp, SeqIdPtr the_id)
{
	SeqIdPtr tsip, sip;
	SeqLocPtr tmp;
	SeqIntPtr sipp;
	Boolean checkin, doit;
	Int4 ctr = 0, len;

	if (bsp != NULL)
	{
		tsip = bsp->id;
		checkin = 1;
	}
	else
	{
		tsip = the_id;
		checkin = 0;
	}

	tmp = NULL;
	while ((tmp = SeqLocFindNext(in, tmp)) != NULL)
	{
		sip = SeqLocId(tmp);
		if (checkin)    /* optimizer */
			doit = SeqIdIn(sip, tsip);
		else
			doit = SeqIdForSameBioseq(sip, tsip);
		switch (tmp->choice)
		{
			case SEQLOC_PNT:
				if (doit)
				{
					if (pos == ((SeqPntPtr)(tmp->data.ptrvalue))->point)
						return ctr;
				}
				ctr++;
				break;
			case SEQLOC_INT:
				sipp = (SeqIntPtr)(tmp->data.ptrvalue);
				if (doit)
				{
					if ((pos >= sipp->from) && (pos <= sipp->to))
					{
						if (sipp->strand == Seq_strand_minus)
							ctr += (sipp->to - pos);
						else
							ctr += (pos - sipp->from);
						return ctr;
					}
				}
				ctr += (sipp->to - sipp->from + 1);
				break;
			case SEQLOC_WHOLE:
				if (doit)
				{
					ctr += pos;
					return ctr;
				}
			default:
				len = SeqLocLen(tmp);
				if (len > 0) ctr += len;
				break;
		}
	}

	return -1;    /* failed */
}


/*****************************************************************************
*
*   Int2 SeqLocOrder(SeqLocPtr a, SeqLocPtr b, BioseqPtr in);
*   	This function is used to sort SeqLocs into ascending order by
*   location on a Bioseq (segmented or otherwise)
*   	The first position is the point sorted on.
*   Returns
*   	0   a and b start at same offset
*   	1   a > b
*   	-1  a < b
*   	-2  a in bsp, b not
*       2   b in bsp, a not
*       3   neither a nor b in bsp
*        This function will attempt to sort locs not in bsp to the end of
*   the list.  Values -2,2,3 can also be used to detect error conditions.
*
*****************************************************************************/
NLM_EXTERN Int2 SeqLocOrder (SeqLocPtr a, SeqLocPtr b, BioseqPtr in)
{
	Int4 aoffset, boffset;


	if ((a == NULL) || (b == NULL) || (in == NULL))
		return 3;

	aoffset = GetOffsetInBioseq(a, in, SEQLOC_LEFT_END);
	boffset = GetOffsetInBioseq(b, in, SEQLOC_LEFT_END);

	if ((aoffset == -1) && (boffset >= 0))
		return 2;
	else if ((aoffset >= 0) && (boffset == -1))
		return -2;
	else if ((aoffset == -1) && (boffset == -1))
		return 3;
	else if (aoffset == boffset)
		return 0;
	else if (aoffset < boffset)
		return -1;
	else
		return 1;
}

/*****************************************************************************
*
*   Int2 SeqLocMol(seqloc)
*   	returns Seq-inst.mol for all Bioseqs this seqloc points to.
*   	if all Seq-inst.mol the same, returns that.
*   	if mixed dna,rna, returns na
*   	if mixed na,aa or can't find any Bioseq, or bsp->mol = 0, or 255
*   			returns 0
*
*****************************************************************************/
NLM_EXTERN Int2 SeqLocMol (SeqLocPtr seqloc)
{
	SeqLocPtr slp = NULL;
	SeqIdPtr sip;
	static Uint1 cases[5][4] = {
		{ 1,2,3,4 } ,    /* was 0, not-set */
		{ 1,4,0,4 } ,    /* was 1, dna */
		{ 4,2,0,4 } ,    /* was 2, rna */
		{ 0,0,3,0 } ,    /* was 3, aa */
		{ 4,4,0,4 }};    /* was 4, na */
	Int2 the_mol = 0, tmp;
	BioseqPtr bsp;
	Boolean locked = FALSE;

	while ((slp = SeqLocFindNext(seqloc, slp)) != NULL)
	{
		sip = SeqLocId(slp);
		if (sip != NULL)
		{
			bsp = BioseqFindCore(sip);
			 if (bsp == NULL)
			 {
			 	bsp = BioseqLockById(sip);
	 			if (bsp != NULL)
			 		locked = TRUE;
			 }
			if (bsp == NULL)
				return 0;
			
			tmp = (Int2)bsp->mol;
			if (locked)
				BioseqUnlock(bsp);
			if ((tmp == 0) || (tmp == Seq_mol_other))
				return 0;
			the_mol = (Int2)cases[the_mol][tmp-1];
			if (! the_mol)
				return 0;
		}
	}
	return the_mol;
}

static SeqIdPtr SeqLocPrintProc(SeqLocPtr slp, ByteStorePtr bsp, Boolean first, SeqIdPtr lastid);
static void BSstring(ByteStorePtr bsp, CharPtr str);

/*****************************************************************************
*
*   SeqLocPrint(slp)
*
*****************************************************************************/
NLM_EXTERN CharPtr SeqLocPrint(SeqLocPtr slp)
{
	ByteStorePtr bsp;
	CharPtr str;
	SeqLocPtr tmp;

	if (slp == NULL) return NULL;

	bsp = BSNew(80);

	tmp = slp->next;    /* save possible chain */
	slp->next = NULL;   /* take out of possible chain */

	SeqLocPrintProc(slp, bsp, TRUE, NULL);

	slp->next = tmp;    /* replace possible chain */
	str = (CharPtr)BSMerge(bsp, NULL);
	BSFree(bsp);

	return str;
}

NLM_EXTERN SeqIdPtr SeqPointWrite(SeqPntPtr spp, CharPtr buf, SeqIdPtr lastid, Int2 buflen);
NLM_EXTERN SeqIdPtr SeqPointPrint(SeqPntPtr spp, CharPtr buf, SeqIdPtr lastid);
NLM_EXTERN void IntFuzzPrint(IntFuzzPtr ifp, Int4 pos, CharPtr buf, Boolean right);
static char strandsymbol[5] = { '\0', '\0', 'c', 'b', 'r' };


/*****************************************************************************
*
*   SeqLocPrintProc(slp, bsp, first, lastid)
*   	print traversal routine
*   	goes down slp chain
*
*****************************************************************************/
static SeqIdPtr SeqLocPrintProc(SeqLocPtr slp, ByteStorePtr bsp, Boolean first, SeqIdPtr lastid)
{
	Char buf[41];
	SeqBondPtr sbp;
	PackSeqPntPtr pspp;
	SeqIntPtr sip;
	IntFuzzPtr ifp1, ifp2;
	Int4 from, to;
	Int2 delim, delim2;

	while (slp != NULL)
	{
		if (! first)
		{
			BSPutByte(bsp, ',');
			BSPutByte(bsp, ' ');
		}
		first = FALSE;

		delim = 0;
	    switch (slp->choice)
    	{
        	case SEQLOC_BOND:   /* bond -- 2 seqs */
				sbp = (SeqBondPtr)(slp->data.ptrvalue);
				if (sbp->a != NULL)
				{
					lastid = SeqPointWrite(sbp->a, buf, lastid, 40);
					BSstring(bsp, buf);
				}
				else
					BSPutByte(bsp, '?');

				BSPutByte(bsp, '=');

				if (sbp->b != NULL)
				{
					lastid = SeqPointWrite(sbp->b, buf, lastid, 40);
					BSstring(bsp, buf);
				}
				else
					BSPutByte(bsp, '?');
				break;
    	    case SEQLOC_FEAT:   /* feat -- can't track yet */
				BSstring(bsp, "(feat)");
				break;
	        case SEQLOC_NULL:    /* NULL */
				BSPutByte(bsp, '~');
				break;
	        case SEQLOC_EMPTY:    /* empty */
				BSPutByte(bsp, '{');
				SeqIdWrite((SeqIdPtr)(slp->data.ptrvalue), 
							buf, PRINTID_FASTA_SHORT, 40);
				BSstring(bsp, buf);
				BSPutByte(bsp, '}');
				break;
	        case SEQLOC_WHOLE:    /* whole */
				SeqIdWrite((SeqIdPtr)(slp->data.ptrvalue), 
						buf, PRINTID_FASTA_SHORT, 40);
				BSstring(bsp, buf);
				break;
	        case SEQLOC_MIX:    /* mix -- more than one seq */
    	    case SEQLOC_PACKED_INT:    /* packed int */
				delim = '(';
				delim2 = ')';
	        case SEQLOC_EQUIV:    /* equiv -- ditto */
				if (! delim)
				{
					delim = '[';
					delim2 = ']';
				}
				BSPutByte(bsp, delim);
				lastid = SeqLocPrintProc((SeqLocPtr)(slp->data.ptrvalue), bsp, TRUE, lastid);
				BSPutByte(bsp, delim2);
	            break;
    	    case SEQLOC_INT:    /* int */
				sip = (SeqIntPtr)(slp->data.ptrvalue);
				if (! SeqIdMatch(sip->id, lastid))
				{
					SeqIdWrite(sip->id, buf, PRINTID_FASTA_SHORT, 40);
					BSstring(bsp, buf);
					BSPutByte(bsp, ':');
				}
				lastid = sip->id;
				if (strandsymbol[sip->strand])
					BSPutByte(bsp, (Int2)strandsymbol[sip->strand]);
				if ((sip->strand == Seq_strand_minus) ||
					(sip->strand == Seq_strand_both_rev))
				{
					ifp1 = sip->if_to;
					ifp2 = sip->if_from;
					to = sip->from;
					from = sip->to;
				}
				else
				{
					ifp1 = sip->if_from;
					ifp2 = sip->if_to;
					to = sip->to;
					from = sip->from;

				}
				IntFuzzPrint(ifp1, from, buf, FALSE);
				BSstring(bsp, buf);
				BSPutByte(bsp, '-');
				IntFuzzPrint(ifp2, to, buf, TRUE);
				BSstring(bsp, buf);

        	    break;
	        case SEQLOC_PNT:    /* pnt */
				lastid = SeqPointWrite((SeqPntPtr)(slp->data.ptrvalue), 
					buf, lastid, 40);
				BSstring(bsp, buf);
	            break;
    	    case SEQLOC_PACKED_PNT:    /* packed pnt */
				pspp = (PackSeqPntPtr)(slp->data.ptrvalue);
				if (pspp != NULL)
					BSstring(bsp, "PackSeqPnt");
	            break;
    	    default:
            	BSstring(bsp, "(\?\?)");
				break;
	    }
		slp = slp->next;
	}
	return lastid;
}


/*****************************************************************************
*
*   BSstring(bsp, str)
*
*****************************************************************************/
static void BSstring(ByteStorePtr bsp, CharPtr str)
{
	BSWrite(bsp, str, (Int4)(StringLen(str)));
	return;
}

/*****************************************************************************
*
*   SeqPointPrint(spp, buf, lastid)
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr SeqPointPrint(SeqPntPtr spp, CharPtr buf, SeqIdPtr lastid)
{
	CharPtr tmp;

	if ((spp == NULL) || (buf == NULL)) return NULL;
	
	tmp = buf;
	*tmp = '\0';
	if (! SeqIdMatch(spp->id, lastid))
	{
		SeqIdPrint(spp->id, tmp, PRINTID_FASTA_SHORT);
		while (*tmp != '\0') tmp++;
		*tmp = ':';
		tmp++; *tmp = '\0';
	}
	if (strandsymbol[spp->strand])
	{
		*tmp = strandsymbol[spp->strand];
		tmp++; *tmp = '\0';
	}
	IntFuzzPrint(spp->fuzz, spp->point, tmp, TRUE);
	
	return spp->id;
}

/*****************************************************************************
*
*   SeqPointWrite(spp, buf, lastid, buflen)
*
*****************************************************************************/
NLM_EXTERN SeqIdPtr SeqPointWrite(SeqPntPtr spp, CharPtr buf, SeqIdPtr lastid, Int2 buflen)
{
	CharPtr tmp;

	if ((spp == NULL) || (buf == NULL)) return NULL;
	
	tmp = buf;
	*tmp = '\0';
	if (! SeqIdMatch(spp->id, lastid))
	{
		SeqIdWrite(spp->id, tmp, PRINTID_FASTA_SHORT, buflen);
		while (*tmp != '\0') tmp++;
		*tmp = ':';
		tmp++; *tmp = '\0';
	}
	if (strandsymbol[spp->strand])
	{
		*tmp = strandsymbol[spp->strand];
		tmp++; *tmp = '\0';
	}
	IntFuzzPrint(spp->fuzz, spp->point, tmp, TRUE);
	
	return spp->id;
}

/*****************************************************************************
*
*   IntFuzzPrint(ifp, pos, buf, right)
*
*****************************************************************************/
NLM_EXTERN void IntFuzzPrint(IntFuzzPtr ifp, Int4 pos, CharPtr buf, Boolean right)
{
	Char lim=0;
	CharPtr tmp;
	Char tbuf[40];

	if (buf == NULL) return;
	pos++;  /* number from 1 */
	tmp = buf;
	*tmp = '\0';
	*tbuf = '\0';
	if (ifp != NULL)
	{
		switch (ifp->choice)
		{
			case 1:     /* plus minus */
				sprintf(tbuf, "<+-%ld>", (long)ifp->a);
				break;
			case 2:     /* range */
				sprintf(tbuf, "<%ld.%ld>", (long)ifp->b, (long)ifp->a);
				break;
			case 3:     /* percent */
				sprintf(tbuf, "<%ld%%>", (long)ifp->a);
				break;
			case 4:     /* limit */
				switch (ifp->a)
				{
					case 0:    /* unknown */
					case 255:  /* other */
						sprintf(tbuf, "<?>");
						break;
					case 1:    /* gt */
						lim = '>';
						break;
					case 2:    /* lt */
						lim = '<';
						break;
					case 3:
						lim = 'r';
						break;
					case 4:
						lim = '^';
						break;
				}
				break;
		}
	}

	if ((lim) && (lim != 'r'))
	{
		*tmp = lim;
		tmp++; *tmp = '\0';
		lim = 0;
	}

	if (right)
	{
		sprintf(tmp, "%ld", (long)pos);
		while (*tmp != '\0') tmp++;
	}
	if (lim == 'r')
	{
		*tmp = '^';
		tmp++;
		*tmp = '\0';
	}
	if (tbuf[0] != '\0')
	{
		tmp = StringMove(tmp, tbuf);
	}
	if (! right)
		sprintf(tmp, "%ld", (long)pos);

	return;

}
/*****************************************************************************
*
*   TaxNameFromCommon(common)
*
*****************************************************************************/
typedef struct sturct_Nlm_taxcommon {
	char * common;
	char * taxname;
} Nlm_TaxCommon, PNTR Nlm_TaxCommonPtr;

NLM_EXTERN CharPtr TaxNameFromCommon (CharPtr common)
{
	CharPtr taxname = NULL;
	CharPtr query = (CharPtr)MemNew(StringLen(common) + 2);
	int tax_try, dex;

	static Nlm_TaxCommon taxcommon[40] = {
	"Chinese hamsters", "Cricetulus sp."
	,"Syrian golden hamsters", "Mesocricetus auratus"
	,"Syrian hamsters", "Mesocricetus sp."
	,"barley", "Hordeum sp."
	,"carrots", "Daucus sp."
	,"cats", "Felis sp."
	,"cattles", "Bos sp."
	,"chickens", "Gallus sp."
	,"chimpanzees", "Pan sp."
	,"chimpanzes", "Pan sp."
	,"corn", "Zea sp."
	,"cucumber", "Cucumis sativus"
	,"dogs", "Canis sp."
	,"goats", "Capra sp."
	,"gorillas", "Gorilla sp."
	,"guinea pigs", "Cavia sp."
	,"hamsters", "Cricetidae gen. sp."
	,"horses", "Equus sp."
	,"humans", "Homo sapiens"
	,"maize", "Zea sp."
	,"mice", "Mus sp."
	,"mouse", "Mus sp."
	,"peas", "Pisum sp."
	,"potatoes", "Solanum sp."
	,"potato", "Solanum sp."
	,"quails", "Phasianidae gen. sp."
	,"rabbits", "Oryctolagus sp."
	,"rats", "Rattus sp."
	,"rices", "Oryza sp."
	,"sheeps", "Ovis sp."
	,"sorghums", "Sorghum sp."
	,"soybeans", "Glycine sp."
	,"spinach", "Spinacia sp."
	,"swine", "Sus sp."
	,"tobacco", "Nicotiania sp."
	,"tomatoes", "Lycopersicon sp."
	,"tomato", "Lycopersicon sp."
	,"turkeys", "Meleagris sp."
	,"wheat", "Triticum sp."
	,"zebrafish", "Brachydanio sp."
};

	if (common == NULL) return NULL;
	
	StringCpy(query,common);  /* space for 's' is at end */
	for (tax_try = 0; tax_try < 2; tax_try ++){
		for (dex = 0; dex < 40; dex ++ ){
			if (StringICmp(query,taxcommon[dex].common) == 0)
				break;
		}
		if ( dex < 40)
			break;
		if (tax_try == 0)
			StringCat(query,"s");
	}
	MemFree (query);
	if (dex < 40)
		taxname = StringSave (taxcommon[dex].taxname);

	return taxname;
}

/*****************************************************************************
*
*   QualLocCreate(from, to)
*   	creates a UserObject of _class NCBI, type 1
*       adds a field of type "qual_loc"
*       puts the from and to numbers in
*       no range check, no strand, no seqid
*       this just carries locations for the qualifiers anticodon and rpt_unit
*       Intended to go on SeqFeat.ext
*
*****************************************************************************/
NLM_EXTERN UserObjectPtr QualLocCreate (Int4 from, Int4 to)
{
	UserObjectPtr usop;
	UserFieldPtr ufp;
	ObjectIdPtr oip;
	Int4Ptr ints;

	usop = UserObjectNew();
	usop->_class = StringSave("NCBI");
	oip = ObjectIdNew();
	oip->id = 1;
	usop->type = oip;

	ufp = UserFieldNew();
        usop->data = ufp;
	oip = ObjectIdNew();
	oip->str = StringSave("qual_loc");
	ufp->label = oip;
	ufp->num = 2;
	ufp->choice = 8;   /* ints */

	ints = (Int4Ptr) MemNew((size_t)(sizeof(Int4) * 2));
	ints[0] = from;
	ints[1] = to;
	ufp->data.ptrvalue = (Pointer)ints;

	return usop;

}

/*****************************************************************************
*
*   QualLocWrite(uop, buf)
*   	Checks a SeqFeat.ext to see if it is
*   		1) not null
*           2) has a UserObject of _class NCBI, type 1
*           3) has a field of label "qual_loc"
*           4) if so, prints the two integers as a qualifier location
*               from..to and returns a pointer to the \0 after "to"
*       If any of the above fail, returns NULL
*
*****************************************************************************/
NLM_EXTERN CharPtr QualLocWrite(UserObjectPtr uop, CharPtr buf)
{
	CharPtr tmp=NULL;
	UserFieldPtr ufp;
	Int4Ptr ints;

	if ((uop == NULL) || (buf == NULL))
		return tmp;

	if (StringCmp(uop->_class, "NCBI"))
		return tmp;

	if (uop->type->id != 1)
		return tmp;

	for (ufp = uop->data; ufp != NULL; ufp = ufp->next)
	{
		if (! StringCmp(ufp->label->str, "qual_loc"))
		{
			if (ufp->choice != 8)  /* not ints */
				return NULL;
			if (ufp->num < 2)	   /* not enough */
				return NULL;
			ints = (Int4Ptr)(ufp->data.ptrvalue);
			if (ints == NULL)
				return tmp;
			tmp = buf;
			sprintf(tmp, "%ld..%ld", (long)(ints[0]+1),
                                                          (long)(ints[1]+1));
			while (*tmp != '\0')
				tmp++;
			return tmp;
		}
	}

	return tmp;
}

/*****************************************************************************
*
*   EntrezASN1Detected detects records retrieved from Entrez, which should
*       not be edited by Sequin and replaced into ID.
*
*****************************************************************************/

static void EntrezAsnCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  SeqDescrPtr  descr;
  SeqDescrPtr  sdp;
  BoolPtr      rsult;
  CharPtr      str;

  if (sep == NULL || sep->data.ptrvalue == NULL || mydata == NULL) return;
  rsult = (BoolPtr) mydata;
  descr = (IS_Bioseq (sep)) ?
         ((BioseqPtr) sep->data.ptrvalue)->descr :
         ((BioseqSetPtr) sep->data.ptrvalue)->descr;
  if (descr == NULL) return;
  sdp = NULL;
  while ((sdp = ValNodeFindNext (descr, sdp, Seq_descr_user)) != NULL) {
    if (sdp->data.ptrvalue != NULL) {
      str = ((UserObjectPtr) sdp->data.ptrvalue)->_class;
      if (StringCmp (str, "gbfix") == 0 || StringCmp (str, "pdbfix") == 0) {
        *rsult = TRUE;
      }
    }
  }
}

NLM_EXTERN Boolean EntrezASN1Detected (SeqEntryPtr sep)

{
  Boolean  rsult;

  rsult = FALSE;
  SeqEntryExplore (sep, (Pointer) &rsult, EntrezAsnCallback);
  return rsult;
}

/*****************************************************************************
*
*   SeqLocIntNew(Int4 from, Int4 to, Uint1 strand, SeqIdPtr sip)
*      makes copy of incoming SeqId
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocIntNew (Int4 from, Int4 to, Uint1 strand, SeqIdPtr sip)
{
	SeqIntPtr sintp;
	SeqLocPtr slp;

	if (sip == NULL) return NULL;
	sintp = SeqIntNew();
	sintp->id = SeqIdDup(sip);
	sintp->from = from;
	sintp->to = to;
	sintp->strand = strand;

	slp = ValNodeNew(NULL);
	slp->choice = SEQLOC_INT;
	slp->data.ptrvalue = (Pointer)sintp;

	return slp;
}

/*****************************************************************************
*
*   SeqLocPntNew(Int4 pos, Uint1 strand, SeqIdPtr sip, Boolean is_fuzz)
*      makes copy of incoming SeqId
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL SeqLocPntNew(Int4 pos, Uint1 strand, SeqIdPtr sip, Boolean is_fuzz)
{
	SeqLocPtr slp;
	SeqPntPtr spp;
	IntFuzzPtr ifp;

	slp = ValNodeNew(NULL);
	slp->choice = SEQLOC_PNT;
	spp = SeqPntNew();
	spp->point = pos;
	spp->strand = strand;
	spp->id = SeqIdDup(sip);
	if(is_fuzz)
	{
		ifp = IntFuzzNew();
		ifp->choice = 4;
		ifp->a = 0;	/*unknown value*/
		spp->fuzz = ifp;
	}
	slp->data.ptrvalue = spp;

	return slp;

}

/* a "gather routine" which collects information about the coding region
   features */
static Boolean gatherCodingRegions(GatherContextPtr gcp)
{
	SpliceInfoPtr sip;
	SeqFeatPtr sfp;
	SeqLocPtr slp, current, next, protSlp, chain;
	Uint1 strand;

	if (gcp == NULL || gcp->thisitem == NULL) return TRUE;

	/* although we had to include higher-level types in GatherScope's
	   "ignore" parameter so that Gather could "see" features, we're
	   really only interested in features */
	if (gcp->thistype != OBJ_SEQFEAT) return TRUE;

	sip = (SpliceInfoPtr) gcp->userdata;

	if (sip == NULL || sip->slp == NULL) return TRUE;

	sfp = (SeqFeatPtr) gcp->thisitem;

	/* we're only interested in coding regions */
	if (sfp->data.choice != SEQFEAT_CDREGION || sfp->product == NULL) return TRUE;

	/* traverse all (but the last) intervals of the coding region feature */
	for (current = SeqLocFindNext(sfp->location, NULL);
	     (next = SeqLocFindNext(sfp->location, current)) != NULL;
	     current = next) {
		/* consider the last DNA base on the interval */
		strand = SeqLocStrand(current);
		slp = SeqLocPntNew(strand == Seq_strand_minus ?
			 SeqLocStart(current) : SeqLocStop(current), strand,
			 SeqLocId(current), FALSE);
		if (sip->findOnProtein)
		{
		    /* find the corresponding location on the protein */
		    protSlp = dnaLoc_to_aaLoc(sfp, slp, TRUE, NULL, FALSE);

		    protSlp->next = NULL;
		    SeqLocFree(slp);

		    if (sip->slp->data.ptrvalue == NULL)
		    {
		        sip->slp->data.ptrvalue = protSlp;
		    } else {
		        for (chain = (SeqLocPtr) sip->slp->data.ptrvalue;
			     chain->next != NULL; chain = chain->next) {
		        }
		        chain->next = protSlp;
		    }
		} else {
		    if (sip->slp->data.ptrvalue == NULL)
		    {
		        sip->slp->data.ptrvalue = slp;
		    } else {
		        for (chain = (SeqLocPtr) sip->slp->data.ptrvalue;
			     chain->next != NULL; chain = chain->next) {
		        }
		        chain->next = slp;
		    }
		}
	}

	return TRUE;
}

/*****************************************************************************
*
*   SeqLocPtr FindSpliceSites(SeqEntryPtr sep, Boolean findOnProtein)
*      Finds the splice sites on this SeqEntry and returns them as a
*      SeqLoc.
*
*****************************************************************************/
NLM_EXTERN SeqLocPtr LIBCALL FindSpliceSites(SeqEntryPtr sep, Boolean findOnProtein)
{
	SpliceInfo si;
	GatherScope gs;
	Int2 i;
	SeqLocPtr slp;

            MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
            gs.get_feats_location = FALSE;
            for (i = 0; i < OBJ_MAX; i++)
                gs.ignore[i] = TRUE;
            gs.ignore[OBJ_SEQFEAT] = FALSE;
            gs.ignore[OBJ_SEQANNOT] = FALSE;
	    slp = ValNodeNew(NULL);
	    slp->choice = SEQLOC_EQUIV;
	    slp->data.ptrvalue = NULL; /* to be filled in within Gather */
            si.slp = slp;
            si.findOnProtein = findOnProtein;
            GatherSeqEntry(sep, &si, gatherCodingRegions, &gs);
	    if (slp->data.ptrvalue == NULL)
	    {
		SeqLocFree(slp);
		return NULL;
	    }

	    return slp;
}


/* a "gather routine" which collects information about the coding region
   feature (based upon the assumption that there is only one) */
static Boolean gatherTheCodingRegion(GatherContextPtr gcp)
{
	SeqFeatPtr PNTR sffp;
	SeqFeatPtr sfp;

	if (gcp == NULL || gcp->thisitem == NULL) return TRUE;

	/* although we had to include higher-level types in GatherScope's
	   "ignore" parameter so that Gather could "see" features, we're
	   really only interested in features */
	if (gcp->thistype != OBJ_SEQFEAT) return TRUE;

	sffp = (SeqFeatPtr PNTR) gcp->userdata;

	if (sffp == NULL) return FALSE;

	sfp = (SeqFeatPtr) gcp->thisitem;

	/* we're only interested in coding regions */
	if (sfp->data.choice != SEQFEAT_CDREGION || sfp->product == NULL) return TRUE;

	*sffp = sfp;

	return FALSE;
}

/*****************************************************************************
*
*   SeqFeatPtr FindCodingRegion(SeqEntryPtr sep)
*      Finds the coding region feature on this protein SeqEntry and
*      returns a copy of it.
*
*****************************************************************************/
NLM_EXTERN SeqFeatPtr LIBCALL FindCodingRegion(SeqEntryPtr sep)
{
	SeqFeatPtr sfp = NULL;
	GatherScope gs;
	Int2 i;

            MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
            gs.get_feats_location = FALSE;
            for (i = 0; i < OBJ_MAX; i++)
                gs.ignore[i] = TRUE;
            gs.ignore[OBJ_SEQFEAT] = FALSE;
            gs.ignore[OBJ_SEQANNOT] = FALSE;
            GatherSeqEntry(sep, &sfp, gatherTheCodingRegion, &gs);
	    /* make a copy of it */
	    if (sfp != NULL)
	        sfp = (SeqFeatPtr)AsnIoMemCopy((Pointer)sfp, (AsnReadFunc)SeqFeatAsnRead, (AsnWriteFunc)SeqFeatAsnWrite);
	    return sfp;
}

static Boolean gatherMolTypeCheck(GatherContextPtr gcp)
{
	SeqIdCheckerPtr sicp;
	BioseqPtr bsp;

	if (gcp == NULL || gcp->thisitem == NULL) return TRUE;

	sicp = (SeqIdCheckerPtr) gcp->userdata;
	bsp = (BioseqPtr) gcp->thisitem;

	if (bsp == NULL) return TRUE;
	if (sicp == NULL) return FALSE;

	/* check for mol-type mismatch */
	if (ISA_na(bsp->mol) == sicp->isProtein) return TRUE;

	if (sicp->sip != NULL)
	{
	    if (SeqIdComp(bsp->id, sicp->sip) != SIC_YES)
		return TRUE;
	}

	sicp->retval = TRUE;

	/* no need to examine other Bioseqs */
	return FALSE;
}

/*****************************************************************************
*
*   Boolean SeqEntryContainsSeqIdOfMolType(SeqEntryPtr sep, SeqIdPtr sip, Boolean isProtein)
*      Tests to see if this SeqEntry contains a bioseq of the specified moltype
*        (protein or DNA)
*      if sip != NULL then it also insists upon finding a bioseq of the
*        specified moltype where the SeqIds match
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqEntryContainsSeqIdOfMolType(SeqEntryPtr sep, SeqIdPtr sip, Boolean isProtein)
{
    Boolean retval = FALSE;
	SeqIdChecker sic;
	GatherScope gs;
	Int2 i;

            MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
            gs.get_feats_location = FALSE;
            for (i = 0; i < OBJ_MAX; i++)
                gs.ignore[i] = TRUE;
            gs.ignore[OBJ_BIOSEQ] = FALSE;
	    sic.sip = sip;
	    sic.isProtein = isProtein;
	    sic.retval = FALSE;
            GatherSeqEntry(sep, &sic, gatherMolTypeCheck, &gs);

	    return sic.retval;
}

static Boolean GIMolTypeCheck(GatherContextPtr gcp)
{
	SeqIdMolTypePtr sicp;
	BioseqPtr bsp;

	if (gcp == NULL || gcp->thisitem == NULL) return TRUE;

	sicp = (SeqIdMolTypePtr) gcp->userdata;
	bsp = (BioseqPtr) gcp->thisitem;

	if (bsp == NULL) return TRUE;
	if (sicp == NULL) return FALSE;

	if (sicp->sip != NULL) {
	    if (SeqIdIn(sicp->sip, bsp->id) == FALSE)
		return TRUE;
	}
	if (ISA_na(bsp->mol)) {
		sicp->mtype = 2;
	} else {
		sicp->mtype = 1;
	}

	/* no need to examine other Bioseqs */
	return FALSE;
}

/*****************************************************************************
*
*      Tests to see if this SeqEntry contains a bioseq of the specified uid
*      returns moltype of the bioseq where the SeqIds match
*			  0     id not found in this SeqEntry 
*		      1     Amino Acid sequence 
*			  2     Nucleotide sequence 
*	  
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL MolTypeForGI(SeqEntryPtr sep, Int4 uid)
{
	SeqIdMolType sic;
	SeqIdPtr sip;
	GatherScope gs;

	MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
	MemSet ((Pointer) (gs.ignore), (int)(TRUE), 
			(size_t) (OBJ_MAX * sizeof(Boolean)));
	gs.get_feats_location = FALSE;
	gs.ignore[OBJ_BIOSEQ] = FALSE;
	sip = ValNodeNew(NULL);
	sip->choice = SEQID_GI;
	sip->data.intvalue = uid;
	sic.sip = sip;
	sic.mtype = 0;
	GatherSeqEntry(sep, &sic, GIMolTypeCheck, &gs);
	ValNodeFree(sip);
	
	return sic.mtype;
}

/******************************************************************
***
*	local_id_make(): make SeqIdPtr with SEQID_LOCAL choice
*
*******************************************************************
***/
NLM_EXTERN SeqIdPtr local_id_make(CharPtr name)
{
   SeqIdPtr new_id;
   ObjectIdPtr obj_id;


	new_id=(SeqIdPtr)ValNodeNew(NULL);
	new_id->choice = SEQID_LOCAL;

	obj_id = ObjectIdNew();
	obj_id->str = StringSave(name);
	new_id->data.ptrvalue = obj_id;

	return new_id;
}



/*************************************************************
*
*	MuskSeqIdWrite (sip, buf, buflen, format, do_find, do_entrez_find)
*	print Seq-id to the buffer with the chromoscope format
*	sip, buf, buflen, format is similar to what is defined in 
*	SeqIdWrite. 
*	do_find, if TRUE, find the most printable id
*	do_entrez_find. if TRUE, if the id is a gi, find the printable id
*
***************************************************************/	

/* a kludge function to make the special formatting for 
   TIGRs THC sequences
*/
static Boolean format_tigr_thc (SeqIdPtr sip, CharPtr buf, Int2 buflen)
{
	DbtagPtr db_tag;
	Char temp[101];
	ObjectIdPtr oip;

	while(sip)
	{
		if(sip->choice == SEQID_GENERAL)
		{
			db_tag = (DbtagPtr) sip->data.ptrvalue;
			if(db_tag->db && StringCmp(db_tag->db, "THC") == 0)
			{
				oip = db_tag->tag;
				if(oip->id != 0)
				{
					sprintf(temp, "THC%ld", (long) oip->id);
					StringNCpy(buf, temp, buflen);
					return TRUE;
				}
			}
		}

		sip = sip->next;
	}

	return FALSE;
}

			

static Boolean format_human_map_id (SeqIdPtr sip, CharPtr buf, Int2 buflen)
{
	DbtagPtr db_tag;
	Char temp[101];
	ObjectIdPtr oip;

	for (; sip; sip = sip->next) {
		if(sip->choice == SEQID_GENERAL) {
			break;
		}
	}
	if (sip == NULL) {
		return FALSE;
	}
	db_tag = (DbtagPtr) sip->data.ptrvalue;
	if (db_tag->db == NULL) {
		return FALSE;
	}
	if (StringCmp(db_tag->db, "MIT") == 0 || StringCmp(db_tag->db, "GENETHON") == 0 || StringCmp(db_tag->db, "CHLC") == 0 || StringCmp(db_tag->db, "GDB") == 0 || StringCmp(db_tag->db, "Stanford") == 0 || StringCmp(db_tag->db, "NCBI") == 0) {
		oip = db_tag->tag;
		if (oip->str != 0) {
			sprintf(temp, "%s", oip->str);
			StringNCpy(buf, temp, buflen);
			return TRUE;
		}
	}
	
	return FALSE;
}

NLM_EXTERN Boolean MuskSeqIdWrite (SeqIdPtr sip, CharPtr buf, Int2 buflen, Uint1 format, Boolean do_find, Boolean do_entrez_find)
{
	SeqIdPtr entrez_id;
	Boolean retval;
	BioseqPtr bsp;
	Boolean bsp_found = FALSE;

	if(sip == NULL || buf == NULL)
		return FALSE;
	
	if(do_find)
	{
		if(sip->next == NULL)
		{
			bsp = BioseqFindCore(sip);
			if(bsp !=NULL)
			{
				bsp_found = TRUE;
				sip = bsp->id;
			}
		}
		sip = SeqIdFindWorst(sip);
	}
	if(sip->choice == SEQID_GI && do_entrez_find && !bsp_found)
	{
		entrez_id = GetSeqIdForGI(sip->data.intvalue);
		if(entrez_id !=NULL)
		{
			retval = MuskSeqIdWrite(entrez_id, buf, buflen, format, TRUE, FALSE);
			SeqIdSetFree(entrez_id);
			return retval;
		}
	}

	if(format_tigr_thc (sip, buf, buflen))
	{	/* give special format to the THC sequence*/
		return TRUE;
	}
	if(format_human_map_id (sip, buf, buflen))
	{	/* special format for human map ids */
		return TRUE;
	}
	SeqIdWrite(sip, buf,format, buflen);	/*in case this function can not work*/

	if(buf[0] == '\0')
		LabelCopy(buf, "Unidentified", buflen);

	return TRUE;
}


/***********************************************************************
***
*	seqid_name(): return the most informative name from a SeqIdPtr
*
************************************************************************
***/
NLM_EXTERN Boolean seqid_name(SeqIdPtr hsip, CharPtr name, Boolean use_locus, Boolean check_chain)
{
	Uint1 format;

	if(use_locus)
		format = PRINTID_TEXTID_LOCUS;
	else
		format = PRINTID_TEXTID_ACCESSION;
	return MuskSeqIdWrite (hsip, name, 20, format, check_chain, check_chain);
}

/***********************************************************************
***
*	update_seq_loc(): update the start, stop, strand info in SeqLoc
*
************************************************************************
***/
NLM_EXTERN SeqLocPtr update_seq_loc(Int4 start, Int4 stop, Uint1 strand, SeqLocPtr loc)
{
   SeqIntPtr sint;
   SeqPntPtr spp;

	if(loc->choice == SEQLOC_INT)
	{
		sint = (SeqIntPtr) loc->data.ptrvalue;
		if(start != -1)
			sint->from = start;
		if(stop != -1)
			sint->to = stop;
		if(strand != 0)
			sint->strand = strand;
		loc->data.ptrvalue = sint;
	}
	else if(loc->choice == SEQLOC_PNT)
	{
		spp = (SeqPntPtr)(loc->data.ptrvalue);
		spp->point = start;
		spp->strand = strand;
		loc->data.ptrvalue = spp;
	}

	return loc;

}


/*
	Gets the SeqIdPtr for the subject or query sequence from the first SeqAlign.
	The SeqIdPtr is not saved  and should not be deleted.
*/
static SeqIdPtr LIBCALL
TxGetIdFromSeqAlign(SeqAlignPtr seqalign, Boolean subject)

{
    DenseDiagPtr ddp;
    DenseSegPtr dsp;
    StdSegPtr ssp;
    SeqIdPtr sip;

    if (seqalign == NULL)
	return NULL;
    
    sip = NULL;
    switch (seqalign->segtype) {
    case 1: /*Dense-diag*/
        ddp = seqalign->segs;
        if (subject == TRUE)
            sip = ddp->id->next;
        else
            sip = ddp->id;
        break;
    case 2: /*Dense-seq */
        dsp = seqalign->segs;
        if (subject == TRUE)
            sip = dsp->ids->next;
        else
            sip = dsp->ids;
        break;
    case 3: /* Std-seg */
        ssp = seqalign->segs;
        if(ssp && ssp->loc && ssp->loc->next) {
            if (subject == TRUE)
                sip = SeqLocId(ssp->loc->next);
            else
                sip = SeqLocId(ssp->loc);
        }
        break;
    case 5: /* Discontinuous alignment */

        sip = TxGetIdFromSeqAlign(seqalign->segs, subject);
        break;
    default:
        break;
    }

    return sip;
}

/* 
	Obtains the query (i.e., the first) SeqIdPtr from
	the first SeqAlignPtr.
*/
NLM_EXTERN SeqIdPtr LIBCALL
TxGetQueryIdFromSeqAlign(SeqAlignPtr seqalign)

{
	return TxGetIdFromSeqAlign(seqalign, FALSE);
}

/* 
	Obtains the subject (i.e., the second) SeqIdPtr from
	the first SeqAlignPtr.
*/
NLM_EXTERN SeqIdPtr LIBCALL
TxGetSubjectIdFromSeqAlign(SeqAlignPtr seqalign)

{
	return TxGetIdFromSeqAlign(seqalign, TRUE);
}

static Boolean
GetBestScoreAndEvalueFromScorePtr(ScorePtr sp, Int4 *score, Nlm_FloatHi *bit_score, Nlm_FloatHi *evalue, Int4 *number)

{
	Boolean score_set=FALSE, evalue_set=FALSE, sum_set=FALSE, bit_set=FALSE;
	ObjectIdPtr obid;
	ScorePtr scrp;

	for (scrp=sp; scrp; scrp = scrp->next)
	{
		obid = scrp->id;
		if(obid && obid->str)
		{
			if (StringICmp(obid->str, "score") == 0)
			{
				if (*score < scrp->value.intvalue)
				{
					score_set = TRUE;
					*score = scrp->value.intvalue;
				}
				continue;
			}
			else if (StringICmp(obid->str, "e_value") == 0 || StringICmp(obid->str, "sum_e") == 0)
			{
				if (*evalue > scrp->value.realvalue)
				{
					evalue_set = TRUE;
					*evalue = scrp->value.realvalue;
				}
				continue;
			}
			else if (StringICmp(obid->str, "sum_n") == 0)
			{
				if (*number < scrp->value.intvalue)
				{
					sum_set = TRUE;
					*number = scrp->value.intvalue;
				}
				continue;
			}
			else if (StringICmp(obid->str, "bit_score") == 0)
			{
				if (*bit_score < scrp->value.realvalue)
				{
					bit_set = TRUE;
					*bit_score = scrp->value.realvalue;
				}
				continue;
			}
		}
	}

	/* Don't check for 'sum_set', as it's not always there. */
	if (score_set && evalue_set && bit_set)
		return TRUE;
	else if(score_set && evalue_set)
	{
		*bit_score = (FloatHi)(*score);
		return TRUE;
	}

	return FALSE;
}


NLM_EXTERN Boolean LIBCALL
GetScoreAndEvalue(SeqAlignPtr seqalign, Int4 *score, Nlm_FloatHi *bit_score, Nlm_FloatHi *evalue, Int4 *number)

{
	Boolean local_retval, retval=FALSE;
        ScorePtr        sp;
        DenseDiagPtr ddp;
        DenseSegPtr dsp;
        StdSegPtr ssp;
	
	*score = 0;
	*bit_score = 0.0;
	*number = 1;
	*evalue = DBL_MAX;

	sp = seqalign->score;
	if (sp == NULL)
	{
		switch (seqalign->segtype)
		{
			case 1: /*Dense-diag*/
                        {
                                Nlm_FloatHi best_evalue = *evalue;
				ddp = seqalign->segs;
				while (ddp)
				{
                                   Int4 number_tmp = 1;
                                   local_retval =
                                      GetBestScoreAndEvalueFromScorePtr(ddp->scores, score, 
                                         bit_score, evalue, &number_tmp);	
                                   /* Use number corresponding to best evalue. */
                                   if (*evalue < best_evalue)
                                   {
                                      best_evalue = *evalue;
                                      *number = number_tmp;
                                   }
                                   if (local_retval == TRUE)
                                      retval = TRUE;
                                   ddp = ddp->next;
				}
				break;
                        }
			case 2:
				dsp = seqalign->segs;
				if (dsp)
				{
					retval = GetBestScoreAndEvalueFromScorePtr(dsp->scores, score, bit_score, evalue, number);	
				}
				break;
			case 3:
				ssp = seqalign->segs;
				while (ssp)
				{
					local_retval = GetBestScoreAndEvalueFromScorePtr(ssp->scores, score, bit_score, evalue, number);	
					if (local_retval == TRUE)
						retval = TRUE;
					ssp = ssp->next;
				}
				break;
			default:
				break;
		}
	}
	else
	{
		retval = GetBestScoreAndEvalueFromScorePtr(sp, score, bit_score, evalue, number);
	}

	return retval;
}

/***********************************************************************
*
*	Adjust the Offset in the SeqAlign to correspond to the beginning
*	of the sequence and not where BLAST started. 
*
**********************************************************************/

NLM_EXTERN void LIBCALL
AdjustOffSetsInSeqAlign(SeqAlignPtr salp, SeqLocPtr slp1, SeqLocPtr slp2)

{
	CharPtr err_string1, err_string2;
	DenseDiagPtr ddp;
	DenseSegPtr dsp;
	Int4 offset1=0, offset2=0, index;
	SeqIdPtr sip1=NULL, sip2=NULL;
	SeqIntPtr seq_int;
	SeqLocPtr seqloc, whole_slp;
	StdSegPtr ssp;
	
        while (salp)
        {
            if (salp->segtype == 1)
            {
                      ddp = salp->segs;
                      while (ddp)
                      {	/* Get the offset on the first call. */
			if (sip1 == NULL)
			{
			     sip1 = ddp->id;
			     whole_slp = 
				ValNodeAddPointer(NULL, SEQLOC_WHOLE, sip1);
			     if(SeqLocStrand(slp1) == Seq_strand_minus)
			     	offset1 = GetOffsetInLoc(slp1, whole_slp, SEQLOC_STOP);
			     else
			     	offset1 = GetOffsetInLoc(slp1, whole_slp, SEQLOC_START);
			     if (offset1 == -1)
			     {
				err_string1 = SeqLocPrint(slp1);
				err_string2 = SeqLocPrint(whole_slp);
        			ErrPostEx(SEV_ERROR, 0, 0, "AdjustOffSetInSeqAnnot: %s not in %s", err_string1, err_string2);
			     }	
			     whole_slp = ValNodeFree(whole_slp);
			}
			if (sip2 == NULL && slp2)
			{
			     sip2 = ddp->id->next;
			     whole_slp = 
				ValNodeAddPointer(NULL, SEQLOC_WHOLE, sip2);
			     if(SeqLocStrand(slp2) == Seq_strand_minus)
			     	offset2 = GetOffsetInLoc(slp2, whole_slp, SEQLOC_STOP);
			     else
			     	offset2 = GetOffsetInLoc(slp2, whole_slp, SEQLOC_START);
			     if (offset2 == -1)
			     {
				err_string1 = SeqLocPrint(slp2);
				err_string2 = SeqLocPrint(whole_slp);
        			ErrPostEx(SEV_ERROR, 0, 0, "AdjustOffSetInSeqAnnot: %s not in %s", err_string1, err_string2);
			     }	
			     whole_slp = ValNodeFree(whole_slp);
			}
			ddp->starts[0] += offset1;
			ddp->starts[1] += offset2;
                        ddp = ddp->next;
                      }
           }   
	   else if (salp->segtype == 2)
	   {
		dsp = salp->segs;
		if (sip1 == NULL)
		{
		     sip1 = dsp->ids;
		     whole_slp = 
			ValNodeAddPointer(NULL, SEQLOC_WHOLE, sip1);
		     if(SeqLocStrand(slp1) == Seq_strand_minus)
		     	offset1 = GetOffsetInLoc(slp1, whole_slp, SEQLOC_STOP);
		     else
		     	offset1 = GetOffsetInLoc(slp1, whole_slp, SEQLOC_START);
		     if (offset1 == -1)
		     {
			err_string1 = SeqLocPrint(slp1);
			err_string2 = SeqLocPrint(whole_slp);
        		ErrPostEx(SEV_ERROR, 0, 0, "AdjustOffSetInSeqAnnot: %s not in %s", err_string1, err_string2);
		     }	
		     whole_slp = ValNodeFree(whole_slp);
		}
		if (sip2 == NULL && slp2)
		{
		     sip2 = dsp->ids->next;
		     whole_slp = 
			ValNodeAddPointer(NULL, SEQLOC_WHOLE, sip2);
		     offset2 = 
			GetOffsetInLoc(slp2, whole_slp, SEQLOC_START);
		     if(SeqLocStrand(slp2) == Seq_strand_minus)
		     	offset2 = GetOffsetInLoc(slp2, whole_slp, SEQLOC_STOP);
		     else
		     	offset2 = GetOffsetInLoc(slp2, whole_slp, SEQLOC_START);
		     if (offset2 == -1)
		     {
			err_string1 = SeqLocPrint(slp2);
			err_string2 = SeqLocPrint(whole_slp);
        		ErrPostEx(SEV_ERROR, 0, 0, "AdjustOffSetInSeqAnnot: %s not in %s", err_string1, err_string2);
		     }	
		     whole_slp = ValNodeFree(whole_slp);
		}

		for (index=0; index<dsp->numseg; index++)
		{
			if (dsp->starts[2*index] != -1)
				dsp->starts[2*index] += offset1;
			if (dsp->starts[2*index+1] != -1)
				dsp->starts[2*index+1] += offset2;
		}
           }   
	   else if (salp->segtype == 3)
	   {
		ssp = salp->segs;
		while (ssp)
		{
			if (sip1 == NULL)
			{
			     sip1 = ssp->ids;
			     whole_slp = 
				ValNodeAddPointer(NULL, SEQLOC_WHOLE, sip1);
                             if(SeqLocStrand(slp1) == Seq_strand_minus)
			        offset1 = GetOffsetInLoc(slp1, whole_slp, 
                                                         SEQLOC_STOP);
                             else
			        offset1 = GetOffsetInLoc(slp1, whole_slp, 
                                                         SEQLOC_START);

			     if (offset1 == -1)
			     {
				err_string1 = SeqLocPrint(slp1);
				err_string2 = SeqLocPrint(whole_slp);
        			ErrPostEx(SEV_ERROR, 0, 0, "AdjustOffSetInSeqAnnot: %s not in %s", err_string1, err_string2);
			     }	
			     whole_slp = ValNodeFree(whole_slp);
			}
			if (sip2 == NULL && slp2)
			{
			     sip2 = ssp->ids->next;
			     whole_slp = 
				ValNodeAddPointer(NULL, SEQLOC_WHOLE, sip2);
                             if(SeqLocStrand(slp2) == Seq_strand_minus)
			        offset2 = GetOffsetInLoc(slp2, whole_slp, 
                                                         SEQLOC_STOP);
                             else
			        offset2 = GetOffsetInLoc(slp2, whole_slp, 
                                                         SEQLOC_START);

			     if (offset2 == -1)
			     {
				err_string1 = SeqLocPrint(slp2);
				err_string2 = SeqLocPrint(whole_slp);
        			ErrPostEx(SEV_ERROR, 0, 0, "AdjustOffSetInSeqAnnot: %s not in %s", err_string1, err_string2);
			     }	
			     whole_slp = ValNodeFree(whole_slp);
			}
			seqloc = ssp->loc;
                        if (seqloc->choice == SEQLOC_INT) {
			    seq_int = seqloc->data.ptrvalue;
			    seq_int->from += offset1;
			    seq_int->to += offset1;
                        }
			seqloc = ssp->loc->next;
                        if (seqloc->choice == SEQLOC_INT) {
			    seq_int = seqloc->data.ptrvalue;
			    seq_int->from += offset2;
			    seq_int->to += offset2;
                        }
                        ssp = ssp->next;
		 }
              }
              salp = salp->next;
         }   
}


/*****************************************************************************
*
*   Boolean SeqIdOrderInList(a, b)
*     Looks for single SeqId, "a" in chain of SeqIds, "b"
*     returns the position (>0) if found.. else returns 0;
*
*****************************************************************************/

NLM_EXTERN Uint4 LIBCALL SeqIdOrderInList (SeqIdPtr a, SeqIdPtr list) {
        SeqIdPtr now;
        Uint4 order;
        Uint1 retval;

        if (a == NULL)
            return 0;

        for (now =list,order=1; now != NULL; now = now -> next,order++)
        {
            retval = SeqIdComp(a, now);
            if(retval==SIC_YES)
                return order;
        }
        return 0;
}

/*****************************************************************************
*
*   Boolean SeqIdOrderInBioseqIdList(a, b)
*     Looks for single SeqId, "a" in chain of SeqIds, "b"
*              and looks at all synonymous SeqIds of the Bioseq "b"
*     returns the position (>0) if found.. else returns 0;
*
*****************************************************************************/

NLM_EXTERN Uint4 LIBCALL SeqIdOrderInBioseqIdList (SeqIdPtr a, SeqIdPtr list) {
        SeqIdPtr now;
        Uint4 order;

        if (a == NULL)
            return 0;

        for (now =list,order=1; now != NULL; now = now -> next,order++)
        {
            if(SeqIdForSameBioseq(a, now)) 
                return order;
        }
        return 0;
}

/* Function to extract the Accession and version number from
   a \usedin GBQual string. 
   (works for a plain Accession Number with version too)
   User must provide string buffers for answer.
   and make sure that last Character of Accession is not a ')'
   using a statement like
   if ((ptr = StringChr (accession, ')')) != NULL) *ptr = '\0';
   and user must StringTok the GBQual for ',' and repeatedly call this.
   */

NLM_EXTERN void LIBCALL ExtractAccession(CharPtr accn,CharPtr accession,CharPtr version) {
    CharPtr verptr;
    if(accn!=NULL) {
        if (*accn == '(') {
                accn++;
        }
        verptr = StrChr(accn,'.');
        if(verptr==NULL) {
            if(version!=NULL)
                version[0]='\0';
            if(accession!=NULL) {
                StringCpy(accession,accn);
            }
        } else {
            Int4 len;
            if(version!=NULL)
                StringCpy(version,verptr+1);
            len = verptr-accn;
            if(accession!=NULL) {
                StringNCpy(accession,accn,len);
                accession[len]=NULLB;
            }
        }
    } else {
        if(accession)
            accession[0]=NULLB;
        if(version)
            version[0]=NULLB;
    }
}


/*
  Hugues Sicotte:
  Function to make a proper type SeqId given a string that represents
  an accession Number. 
  If version number is unknown, set version=0 for latest.
  name is ignored because it is not always consistently used in databases.
  User may need to Call ExtractAccession to parse out accession and version.

  *** WARNING *** In the non-network mode, this function depends on hardcoded
  accession prefix list to guess at the right prefix type.

  There is an inherent conflict in name space between pir proteins and
  nucleotide genbank accessions  (or swissprot. )
        There is a VERY low probability of conflict between pir and swissprot..
        .. so this codes ignores it. (no known cases).
  so Refseq, swissprot proteins and non-swissprot proteins have an independent name space.

  - some PIR names(locus-name looking) have no conflicts 
             ([A-Z][0-9,A-Z]{3,5}) with nucleotide accession.
  - some PIR accessions have conflicts with 1+5 nucleotide accession, but the
          2+5 nucleotide accession have no conflict with pir.

  The Boolean flag AllowPIR: If TRUE, allows that accessions may be PIR.
  The Boolean flag Permissive, 
             if FALSE, 
                 - completely ignores PIR accessions,
                 - doesn't guess at unnassigned accessions prefix.
                    (even if they look like accession)
                 - the network will NOT be used.
             if TRUE
                 - allows unassigned accessions (as long as they fit the
                     accession patterns)
                 - allows for PIR accessions if AllowPIR==TRUE;
                 - allow for Network Access if UseNetwork==TRUE to resolve conflicts.
                 - if UseNetwork == FALSE, uses the boolean flag FavorNucleotide
                          to resolve conflicts.

  The Boolean flag FavorNucleotide chooses to believe that the conflicts are best
  resolved by believing that the sequence is a nucleotide (unless UseNetwork is set).

  The Boolean flag UseNetwork supersedes FavorNucleotide, and uses the network to
  resolve conflict and for 'unknown' or 'unnassigned' accessions.

  ***  .. Assumes that any new accession type is of nucleotide type
             (in permissive mode)

  ***  Using UseNetwork will not prevent unknown (even not in database)
            from resulting in a valid seqid.

*/
NLM_EXTERN  SeqIdPtr LIBCALL SeqIdFromAccession(CharPtr accession, Uint4 version,CharPtr name) {
    Boolean Permissive = TRUE;
    Boolean UseNetwork = FALSE;
    Boolean FavorNucleotide = TRUE;
    Boolean AllowPIR = FALSE;
    return SeqIdFromAccessionEx(accession,version,name,Permissive, AllowPIR,UseNetwork,FavorNucleotide);
}


NLM_EXTERN  SeqIdPtr LIBCALL SeqIdFromAccessionEx(CharPtr accession, Uint4 version,CharPtr name,Boolean Permissive, Boolean AllowPIR,Boolean UseNetwork,Boolean FavorNucleotide) {
    SeqIdPtr sip;
    BioseqPtr bsp=NULL;
    TextSeqIdPtr tsp;
    Uint4 status;
    if(accession==NULL || accession[0]=='\0')
        return NULL;
    sip=NULL;
    status = WHICH_db_accession(accession);
    if(!(ACCN_IS_UNKNOWN(status))) {
        Boolean formally_assigned;
        formally_assigned = !(ACCN_IS_UNASSIGNED(status));
        if(formally_assigned || Permissive) {
        	/* new support for PDB */
        	if (status == ACCN_PDB) {
        		Char pdbstr [41];
        		if (StringLen (accession) < 8) {
        			sprintf (pdbstr, "pdb|%s", accession);
        			sip = SeqIdParse (pdbstr);
        			return sip;
        		}
        		return NULL;
        	}
            sip = ValNodeNew(NULL);
            tsp = TextSeqIdNew();
            tsp->accession = StringSave(accession);
            sip->data.ptrvalue = tsp;
            tsp->name = NULL;
            tsp->version = version;
            if(ACCN_IS_REFSEQ(status)) {
                sip->choice = SEQID_OTHER;
            } else if(ACCN_IS_SWISSPROT(status)) {
                sip->choice = SEQID_SWISSPROT;
            } else {
                Boolean PIR=FALSE;
                if(Permissive && AllowPIR) {
                    /* In this loop.. can only be PIR of type 1+5 accession */
                    if( ACCN_PIR_FORMAT(accession) && ((!FavorNucleotide) || UseNetwork)) {
                        if(UseNetwork) {
                            sip->choice = SEQID_GENBANK;
                            bsp = BioseqLockById(sip);
                            if(bsp) {
                                if(bsp->mol==Seq_mol_aa)
                                    PIR=TRUE;
                                BioseqUnlock(bsp);
                            } else {
                                sip->choice = SEQID_PIR;
                                bsp = BioseqLockById(sip);
                                if(bsp) {
                                    if(bsp->mol==Seq_mol_aa)
                                        PIR=TRUE;
                                    BioseqUnlock(bsp);
                                } else if (!FavorNucleotide) {
                                    PIR = TRUE;
                                }
                            }
                        } else if(!FavorNucleotide) {
                            PIR = TRUE;
                        }
                    }
                }
                if(PIR) {
                    sip->choice = SEQID_PIR;
                } else {
                    if(ACCN_IS_GENBANK(status)) {
                        sip->choice = SEQID_GENBANK;
                    } else if (ACCN_IS_EMBL(status)) {
                        sip->choice = SEQID_EMBL;
                    } else if (ACCN_IS_DDBJ(status)) {
                        sip->choice = SEQID_DDBJ;
                    } else if (ACCN_IS_TPA(status)) {
                    	if (status == ACCN_NCBI_TPA || status == ACCN_NCBI_TPA_PROT) {
                        	sip->choice = SEQID_TPG;
                    	} else if (status == ACCN_EMBL_TPA || status == ACCN_EMBL_TPA_PROT) {
                        	sip->choice = SEQID_TPE;
                    	} else if (status == ACCN_DDBJ_TPA || status == ACCN_DDBJ_TPA_PROT) {
                        	sip->choice = SEQID_TPD;
                    	} else { /* default TPA */
                        	sip->choice = SEQID_TPG;
                    	}
                    } else /* default */
                        sip->choice = SEQID_GENBANK;
                }
            }
        }
    } else if(Permissive) {
        /* can only be a locus name type accession.
           (i.e. an arbitrary string .. or a completely
           new type/format of accession)
           .. any 1+5, 2+6 3+5 refseq accession.. are
           handled above.
        */
        Boolean PIR = FALSE;
        sip = ValNodeNew(NULL);
        tsp = TextSeqIdNew();
        tsp->accession = StringSave(accession);
        sip->data.ptrvalue = tsp;
        tsp->name = NULL;
        tsp->version = version;
        sip->choice = SEQID_GENBANK; /* default */
        if(AllowPIR && ACCN_PIR_FORMAT(accession) && ( UseNetwork || !FavorNucleotide )) {
            if(UseNetwork) { /* Only if user application has
                                        allowed ID1 bioseq Fetching
                                        ID1Init();ID1BioseqFetchEnable("prog",TRUE);
                                     */
                bsp = BioseqLockById(sip);
                if(bsp) {
                    if(bsp->mol==Seq_mol_aa) {
                        SeqIdPtr sip2;
                        ErrPostEx(SEV_WARNING,0,0,"%s Should NOT be a protein but IS\n",accession);
                        sip2 = SeqIdFindBestAccession(bsp->id);
                        sip->choice = sip2->choice;
                        /* when fetching .. allow for the possibility
                           of new protein prefix of non PIR type */
                        if(sip->choice == SEQID_PIR) {
                            tsp->name = tsp->accession;
                            tsp->accession = NULL;
                            PIR=TRUE;
                        }
                    } else {
                        SeqIdPtr sip2;
                        sip2 = SeqIdFindBestAccession(bsp->id);
                        sip->choice = sip2->choice;
                        if(StringCmp(accession,((TextSeqIdPtr)(sip2->data.ptrvalue))->name)==0) {
                            /*
                               --> "accession" is the LOCUS
                            */
                            tsp->name = tsp->accession;
                            tsp->accession = NULL;
                        } /* else  unknown(not hardcoded) accession type (not locus)                             
                           */
                    }
                } else {
                    sip->choice = SEQID_PIR;
                    tsp->name = tsp->accession;
                    tsp->accession = NULL;
                    bsp = BioseqLockById(sip);
                    if(bsp) {
                        if(bsp->mol==Seq_mol_aa) {
                            SeqIdPtr sip2;
                            sip2 = SeqIdFindBestAccession(bsp->id);
                            if(sip->choice != SEQID_PIR) {
                                ErrPostEx(SEV_WARNING,0,0,"PIR SeqId retrieve non-PIR sequence!\n");
                            } else 
                                PIR=TRUE;
                        } else {
                            ErrPostEx(SEV_WARNING,0,0,"PIR SeqId retrieve non-amino-acid sequence!\n");
                        }
                    }
                    if(!PIR) {
                        /* revert to original accession <-> name  order 
                         */
                        tsp->accession = tsp->name;
                        tsp->name = NULL;
                    }
                }
                if(!bsp) { /* No network was available .
                              or SeqIdFetch failed */
                    if(!FavorNucleotide) {
                        PIR = TRUE;
                    }
                    if(PIR) {
                        sip->choice = SEQID_PIR;
                        tsp->name = tsp->accession;
                        tsp->accession = NULL;
                    } else { /* LOCUS NAME  SeqId */
                        sip->choice = SEQID_GENBANK;
                        tsp->name = tsp->accession;
                        tsp->accession = NULL;
                    }
                } else
                    BioseqUnlock(bsp);
            } else { /* !UseNetwork */
                if(!FavorNucleotide && !UseNetwork) {
                    PIR = TRUE;
                } else if(FavorNucleotide && !UseNetwork) {
                    PIR = FALSE; /* XXX Should never be called */
                }
                if(PIR) {
                    sip->choice = SEQID_PIR;
                    tsp->name = tsp->accession;
                    tsp->accession = NULL;
                } else { /* LOCUS NAME  SeqId */
                    sip->choice = SEQID_GENBANK;
                    tsp->name = tsp->accession;
                    tsp->accession = NULL;
                }
            }
        } else {
            /* Permissive .. but 
               FavorNucleotide && !UseNetwork  OR 
               it doesn't look like a PIR  (so it will be assumed it's a NUC                .. Independent of FavorNucleotide is.)
            */
            if(UseNetwork) {
                /*
                  Use network to decide if it is genbank, embl or ddbj 
                */
                sip->choice = SEQID_GENBANK;
                bsp = BioseqLockById(sip);
                if(bsp) {
                        SeqIdPtr sip2;
                        sip2 = SeqIdFindBestAccession(bsp->id);
                        sip->choice = sip2->choice;
                        if(StringCmp(accession,((TextSeqIdPtr)(sip2->data.ptrvalue))->name)==0) {
                            /*
                               --> "accession" is the LOCUS
                            */
                            tsp->name = tsp->accession;
                            tsp->accession = NULL;
                        }
                        BioseqUnlock(bsp);
                } /* .. if not found .. Make it anyways */
            }
        }
    }
    return sip;
}


/* Variant of SeqIdFromAccession that works on accession.version string (JK) */

NLM_EXTERN SeqIdPtr SeqIdFromAccessionDotVersion (CharPtr accession)

{
  Char      accn [41];
  CharPtr   ptr;
  long int  ver = INT2_MIN;

  StringNCpy_0 (accn, accession, sizeof (accn));
  ptr = StringChr (accn, '.');
  if (ptr != NULL) {
    *ptr = '\0';
    ptr++;
    if (sscanf (ptr, "%ld", &ver) != 1) {
      ver = INT2_MIN;
    }
  }
  return SeqIdFromAccession (accn, (Uint4) ver, NULL);
}


/* N* GSDB accession numbers were made secondary to
   genbank or embl or ddbj or genbank records  
   .. but some of these N numbers had already been assigned by 
   embl OR ddbj OR genbank.

   The net result is that N numbers can belong to either 3 databases,
   and the same N-numbers can point to two completely different sequences.
   .. One which was an N* from GSDB, the other one from one of the
   major databases.

   status as of 12/2000 :  using the [ACCN] field in Entrez.
   Maintenance by H. Sicotte and M. Cavanaug

*/
static CharPtr gb_N_numbers = "00008/00013/00018/00019/00027/00041/00046/00048/00052/00054/18624/";
static CharPtr embl_N_numbers = "00060/00064/";
static CharPtr ddbj_N_numbers = "00028/00035/00037/00053/00061/00062/00063/00065/00066/00067/00068/00069/00078/00079/00083/00088/00090/00091/00092/00093/00094/";
static CharPtr embl_ddbj_N_numbers = "00070/";
static CharPtr embl_gb_N_numbers = "00001/00002/00011/00057/";
static CharPtr embl_gb_ddbj_N_numbers = "00005/00009/00012/00020/00022/00025/00058/";
/* No N_* accession assigned for these and N00095 .. N0****
   .. and only N18624 assigned in the N1**** range.
   N2*..N9* are genbank EST's.
   .. all other numbers (below 0have been assigned to BOTH ddbj and genbank.

 */
static CharPtr nonexistant_N_numbers = "00071/00072/00073/00074/00075/00076/00077/00080/00081/00082/00084/00085/00086/00087/00089/00095/";

/*    N00004 was replaced by another ID () which was withdrawn 
 */
static CharPtr gb_ddbj_N_numbers = "00003/00004/00006/00007/00010/00014/00015/00016/00017/00021/00023/00024/00026/00029/00030/00031/00032/00033/00034/00036/00038/00039/00040/00042/00043/00044/00045/00047/00049/00050/00051/00055/00056/00059/";


static Uint4 LIBCALL N_accession (CharPtr s) {
    Uint4 retcode=ACCN_UNKNOWN;
    if(s && (*s=='N' || *s == 'n')) {
        Int4 id;
        id = atoi(s+1);
        if(id>20000) {
            retcode = ACCN_NCBI_EST;
        } else {
            if(id==0 || (id>=95 && id !=18624))
                retcode = ACCN_UNKNOWN;
            else if(StringStr(embl_N_numbers,s+1)!=NULL)
                retcode = ACCN_EMBL_OTHER;
            else if (StringStr(ddbj_N_numbers,s+1)!=NULL)
                retcode = ACCN_DDBJ_OTHER;
            else if (StringStr(gb_N_numbers,s+1)!=NULL)
                retcode = ACCN_NCBI_OTHER;
            else if (StringStr(nonexistant_N_numbers,s+1)!=NULL) 
                retcode = ACCN_UNKNOWN;
            else if (StringStr(embl_gb_N_numbers,s+1)!=NULL) 
                retcode = ACCN_EMBL_GB;
            else if (StringStr(embl_ddbj_N_numbers,s+1)!=NULL) 
                retcode = ACCN_EMBL_DDBJ;
            else if (StringStr(gb_ddbj_N_numbers,s+1)!=NULL) 
                retcode = ACCN_GB_DDBJ;
            else if (StringStr(embl_gb_ddbj_N_numbers,s+1)!=NULL) 
                retcode = ACCN_EMBL_GB_DDBJ;
            else {
                ErrPostEx(SEV_WARNING,0,0,"sequtil::N_accession: Missing N-accession, not accounted for: %s\n",s);
                retcode = ACCN_UNKNOWN;
            }
        }
    } else {
        ErrPostEx(SEV_WARNING,0,0,"sequtil::N_accession: Function called with non-N accession: %s\n",s == NULL ? "NULL Accession" : s);
        retcode = ACCN_UNKNOWN;
                
    }
    return retcode;
}


/*
  functions N_ACCN_IS_GENBANK()
  take an N-accession number and returns TRUE if
  it from the proper database.
  Take into account that N-accession can belong to many databases.
*/

NLM_EXTERN Boolean LIBCALL NAccnIsGENBANK (CharPtr s) {
    Boolean retstatus;
    Int4 id;
    id = atoi(s+1);
    if(*s != 'n' || *s != 'N')
        return FALSE;
    if(id == 0) {
        retstatus = FALSE;
    } else if(id>=20000) {
        retstatus = TRUE;
    } else {
        if(StringStr(gb_N_numbers,s+1)!=NULL 
           || StringStr(embl_gb_N_numbers,s+1)!=NULL 
           || StringStr(embl_gb_ddbj_N_numbers,s+1)!=NULL
           || StringStr(gb_ddbj_N_numbers,s+1)!=NULL)
            retstatus = TRUE;
        else
            retstatus = FALSE;
    }
    return retstatus;
}

NLM_EXTERN Boolean LIBCALL NAccnIsEMBL (CharPtr s) {
    Boolean retstatus;
    Int4 id;
    id = atoi(s+1);
    if(*s != 'n' || *s != 'N')
        return FALSE;
    if(id == 0 || id>20000) {
        retstatus = FALSE;
    } else {
        if(StringStr(embl_N_numbers,s+1)!=NULL 
           || StringStr(embl_gb_N_numbers,s+1)!=NULL 
           || StringStr(embl_ddbj_N_numbers,s+1)!=NULL
           || StringStr(embl_gb_ddbj_N_numbers,s+1)!=NULL)
            retstatus = TRUE;
        else
            retstatus = FALSE;
    }
    return retstatus;
}

NLM_EXTERN Boolean LIBCALL NAccnIsDDBJ (CharPtr s) {
    Boolean retstatus;
    Int4 id;
    id = atoi(s+1);
    if(*s != 'n' || *s != 'N')
        return FALSE;
    if(id == 0 || id>20000) {
        retstatus = FALSE;
    } else {
        if(StringStr(ddbj_N_numbers,s+1)!=NULL 
           || StringStr(embl_ddbj_N_numbers,s+1)!=NULL 
           || StringStr(embl_gb_ddbj_N_numbers,s+1)!=NULL
           || StringStr(gb_ddbj_N_numbers,s+1)!=NULL)
            retstatus = TRUE;
        else
            retstatus = FALSE;

    }
    return retstatus;
}

NLM_EXTERN Boolean LIBCALL AccnIsSWISSPROT( CharPtr s) {
     Boolean retstatus = FALSE;
     if(s && *s && *(s+1) && *(s+2) && *(s+3) && *(s+4) && *(s+5) && *(s+6) ==NULLB) {
         if(*s == 'o' || *s == 'O' ||
            *s == 'p' || *s == 'P' ||
            *s == 'q' || *s == 'Q') {
             if(IS_DIGIT(*(s+1))) {
                 if(IS_ALPHA(*(s+2)) || IS_DIGIT(*(s+2))) {
                     if(IS_ALPHA(*(s+3)) || IS_DIGIT(*(s+3))) {
                         if(IS_ALPHA(*(s+4)) || IS_DIGIT(*(s+4))) {
                             if(IS_DIGIT(*(s+5))) {
                                 retstatus = TRUE;
                             }
                         }
                     }
                 }
             }
         }
     }

     return retstatus;
}
 
NLM_EXTERN Boolean LIBCALL AccnIsUniProt (CharPtr s)
 
{
  Char  ch;

  if (StringLen (s) != 6) return FALSE;

  ch = *s;
  if (! IS_ALPHA (ch)) return FALSE;

  s++;
  ch = *s;
  if (! IS_DIGIT (ch)) return FALSE;

  s++;
  ch = *s;
  if (! IS_ALPHA (ch)) return FALSE;

  s++;
  ch = *s;
  if (! (IS_ALPHA (ch) || IS_DIGIT (ch))) return FALSE;

  s++;
  ch = *s;
  if (! (IS_ALPHA (ch) || IS_DIGIT (ch))) return FALSE;

  s++;
  ch = *s;
  if (! IS_DIGIT (ch)) return FALSE;

   return TRUE;
}

 /*
   function to tell if an accession is in the format
   of a PIR accession number.
   (either a 1+5 accession, or a locus name of length 4-6 alphanumerics)
  */
 NLM_EXTERN Boolean LIBCALL ACCN_PIR_FORMAT( CharPtr s) {
     Boolean retstatus = FALSE;
     if(s) {
         Int4 i,l;
         l = StringLen(s);
         if(*s && *(s+1) && *(s+2) && *(s+3) && l>=4 && l<=6) {
             if(IS_ALPHA(*s)) {
                 retstatus = TRUE;
                 for(i=1;i<l;i++) {
                     if(!(IS_ALPHA(*(s+i)) || IS_DIGIT(*(s+i))))
                         retstatus = FALSE;
                 }
             }
         }
     }

     return retstatus;
 }


 NLM_EXTERN Boolean LIBCALL ACCN_1_5_FORMAT( CharPtr s) {
     Boolean retstatus = FALSE;
     if(s) {
         Int4 i;
         if(*s && StringLen(s) ==6) {
             if(IS_ALPHA(*s)) {
                 retstatus = TRUE;
                 for(i=1;i<6;i++) {
                     if(!(IS_DIGIT(*(s+i))))
                         retstatus = FALSE;
                 }
             }
         }
     }
     return retstatus;
 }


/*****************************************************************************
*
*  Function:    WHICH_db_accession
*
*  Description: Returns a non-zero code if the input string is a validly 
*               formatted database accession number 
*               The return code can be used to infer known infor
*               mation about which database this accession belongs to.
*               using a set of macros in accutils.h
*               (GenBank, EMBL, DDBJ, Swissprot)
*  *****WARNING****
*
*   this function must be maintained.
*  *****WARNING****
*
*  Arguments:   s : CharPtr; pointer to accession number string.
*                   Must be null terminated.
*
*  Author:      Mark Cavanaug, Hugues Sicotte (3/99)
*  Date:        7/96(IS_ntdb_accession),3/99(WHICH_db_accession)
*
*  WARNING:     WHICH_db_accession() does not communicate with any central
*               resource about accession numbers. So there's no way to
*               inform it automatically about new accession number prefixes.
*
*               Version Number ".integer" MUST have been stripped out
*               before calling this function.
*****************************************************************************/
NLM_EXTERN Uint4 LIBCALL WHICH_db_accession (CharPtr s)
{
  Uint4 retcode = 0;
  Boolean retval = TRUE;
  Boolean first = TRUE;
  size_t len;
  Char temp [16];
 
  if (s == NULL || ! *s)
    return FALSE;

  len = StringLen (s);

  if (IS_DIGIT (*s)) {
    if (len == 4 || (len > 4 && s [4] == '|')) {
      return ACCN_PDB;
    }
    return ACCN_UNKNOWN;
  }

  switch (len) {

  case 6:                       /* Old-style 6-character accession */
    if (AccnIsUniProt (s)) {
      return ACCN_SWISSPROT;
    }
    while (*s) {
      if (retval == FALSE)
        break;
 
      if (first) {
        if (! IS_ALPHA(*s)) {
          retval = FALSE;
          break;
        }
 
        switch (TO_UPPER(*s)) {

/* Protein SWISS-PROT accessions */
        case 'O': case 'P': case 'Q':  
            if (AccnIsSWISSPROT(s)) {
            	retcode = ACCN_SWISSPROT;
            }
            break;

/* GenBank : EST */
        case 'H':  case 'R': case 'T': case 'W': 
            retcode = ACCN_NCBI_EST;
            break;
        case 'N':
            retcode = N_accession(s);
            break;
            /* GenBank : non-EST */
        case 'B': 
            retcode = ACCN_NCBI_GSS;
            break;
        case 'G': 
            retcode = ACCN_NCBI_STS;
            break;
        case 'S': 
            retcode = ACCN_NCBI_BACKBONE; /* Scanned journal articles */
            break;
        case 'U': 
            retcode = ACCN_NCBI_EST;
            break;

            /* GenBank : before NCBI */
        case 'J': case 'K': case 'L': case 'M':      
            retcode = ACCN_GSDB_DIRSUB;
            break;
            
            /* EMBL */
        case 'A':
            retcode = ACCN_EMBL_PATENT;
            break;
        case 'F': 
            retcode = ACCN_EMBL_EST;
            break;
        case 'V': case 'X': case 'Y': case 'Z':  
            retcode =  ACCN_EMBL_DIRSUB;
            break;
            
            /* DDBJ */
        case 'C': 
            retcode =  ACCN_DDBJ_EST;
            break;
        case 'D': 
            retcode = ACCN_DDBJ_DIRSUB;
            break;
        case 'E':
            retcode = ACCN_DDBJ_PATENT;
            break;

            /* Case I can be confused with pir accessions which 
             use the I* protein namespace 
            */
            
        case 'I' : /* NCBI patent */
            retcode = ACCN_NCBI_PATENT;
            break;
        default: /* should not happen.. all A-Z assigned */
            retcode = ACCN_IS_NT;
            ErrPostEx(SEV_WARNING,0,0,"sequtil:WHICH_db_accession : Bug in IS_ALPHA macro or memory trashing!!!; accession %s \n",s ==NULL ? "NULL Accession" : s);
            break;
        }
      first = FALSE;
      } else {
          switch (retcode) {
             case ACCN_SWISSPROT:
                 break;
             default:
                 if (! IS_DIGIT(*s)) {
                     retval = FALSE;
                 }
          }
      }  
      s++;
    }
    break;
    case 8: /* New 8-character accession, two letters + 6 digits */
            /* OR three letters + 5 digits for proteins */
        /* Check that have 3 letters */
      if(!IS_ALPHA(*s) || !IS_ALPHA(*(s+1)))
          break;
      if(IS_ALPHA(*(s+2))) {
          /* New(1999) 8-character protein accession, three letters + 5 digits */
          temp[0] = *s; s++;
          temp[1] = *s; s++;
          temp[2] = *s; s++;
          temp[3] = '\0';
          
          if ((StringICmp(temp,"AAA") >= 0) && (StringICmp(temp,"AZZ") <= 0)) { 
              retcode = ACCN_NCBI_PROT;
          } else  if ((StringICmp(temp,"BAA") >= 0) && (StringICmp(temp,"BZZ") <= 0)) { 
              retcode = ACCN_DDBJ_PROT;
          } else if ((StringICmp(temp,"CAA") >= 0) && (StringICmp(temp,"CZZ") <= 0)) { 
              retcode = ACCN_EMBL_PROT;
          } else if ((StringICmp(temp,"DAA") >= 0) && (StringICmp(temp,"DZZ") <= 0)) { 
              retcode = ACCN_NCBI_TPA_PROT;
          } else if ((StringICmp(temp,"EAA") >= 0) && (StringICmp(temp,"EZZ") <= 0)) { 
              retcode = ACCN_NCBI_WGS_PROT;
          } else  if ((StringICmp(temp,"FAA") >= 0) && (StringICmp(temp,"FZZ") <= 0)) { 
              retcode = ACCN_DDBJ_TPA_PROT;
          } else  if ((StringICmp(temp,"GAA") >= 0) && (StringICmp(temp,"GZZ") <= 0)) { 
              retcode = ACCN_DDBJ_WGS_PROT;
          } else {
              retcode = ACCN_IS_PROTEIN;
              retval = TRUE;
              break;
          }
      } else if (IS_DIGIT(*(s+2))) {
          /* New 8-character accession, two letters + 6 digits */
          temp[0] = *s; s++;
          temp[1] = *s; s++;
          temp[2] = '\0';
          
          if ((StringICmp(temp,"AA") == 0) ||
	      (StringICmp(temp,"AI") == 0) || 
	      (StringICmp(temp,"AW") == 0) || 
	      (StringICmp(temp,"BE") == 0) || 
	      (StringICmp(temp,"BF") == 0) || 
	      (StringICmp(temp,"BI") == 0) || 
	      (StringICmp(temp,"BM") == 0) || 
	      (StringICmp(temp,"BQ") == 0) || 
	      (StringICmp(temp,"BU") == 0) || 
	      (StringICmp(temp,"CA") == 0) || 
	      (StringICmp(temp,"CB") == 0) || 
	      (StringICmp(temp,"CD") == 0) || 
	      (StringICmp(temp,"CF") == 0) || 
	      (StringICmp(temp,"CK") == 0) || 
	      (StringICmp(temp,"CN") == 0) || 
	      (StringICmp(temp,"CO") == 0) || 
	      (StringICmp(temp,"CV") == 0) || 
	      (StringICmp(temp,"CX") == 0) || 
	      (StringICmp(temp,"DN") == 0) || 
	      (StringICmp(temp,"DR") == 0) ) {                /* NCBI EST */
              retcode = ACCN_NCBI_EST;
          } else if ((StringICmp(temp,"BV") == 0)) {      /* NCBI STS */
              retcode = ACCN_NCBI_STS;
          } else if ((StringICmp(temp,"AC") == 0) ||
                     (StringICmp(temp,"DP") == 0)) {      /* NCBI HTGS */
              retcode = ACCN_NCBI_HTGS;
          } else if ((StringICmp(temp,"AF") == 0) ||
                     (StringICmp(temp,"AY") == 0) ||
                     (StringICmp(temp,"DQ") == 0)) {      /* NCBI direct submission */
              retcode = ACCN_NCBI_DIRSUB;
          } else if ((StringICmp(temp,"AE") == 0) ||
                     (StringICmp(temp,"CP") == 0) ||
                     (StringICmp(temp,"CY") == 0)) {      /* NCBI genome project data */
              retcode = ACCN_NCBI_GENOME;
          } else if ((StringICmp(temp,"AH") == 0) ||
                     (StringICmp(temp,"CH") == 0) ||      /* NCBI segmented set header Bioseq */
                     (StringICmp(temp,"CM") == 0)) {
              retcode = ACCN_NCBI_SEGSET | ACCN_AMBIGOUS_MOL; /* A few segmented
                                                                 proteins are AH */
          } else if ((StringICmp(temp,"AS") == 0)) {      /* NCBI "other" */
              retcode = ACCN_NCBI_OTHER;
          } else if ((StringICmp(temp,"AD") == 0)) {      /* NCBI accessions assigned to GSDB entries */
              retcode = ACCN_NCBI_GSDB;
          } else if ((StringICmp(temp,"AQ") == 0) ||
                     (StringICmp(temp,"AZ") == 0) ||
                     (StringICmp(temp,"BH") == 0) ||
                     (StringICmp(temp,"BZ") == 0) ||
                     (StringICmp(temp,"CC") == 0) ||
                     (StringICmp(temp,"CE") == 0) ||
                     (StringICmp(temp,"CG") == 0) ||
                     (StringICmp(temp,"CL") == 0) ||
                     (StringICmp(temp,"CW") == 0) ||
                     (StringICmp(temp,"CZ") == 0) )  {     /* NCBI GSS */
              retcode = ACCN_NCBI_GSS;
          } else if ((StringICmp(temp,"AR") == 0)) {      /* NCBI patent */
              retcode = ACCN_NCBI_PATENT;
          } else if((StringICmp(temp,"BC")==0)) {         /* NCBI long cDNA project : MGC */
              retcode = ACCN_NCBI_cDNA;
          } else if((StringICmp(temp,"BT")==0)) {         /* NCBI FLI_cDNA */
              retcode = ACCN_NCBI_cDNA;
          } else if((StringICmp(temp,"BK")==0) ||         /* NCBI third-party annotation */
                    (StringICmp(temp,"BL") == 0)) {
              retcode = ACCN_NCBI_TPA;
          } else if ((StringICmp(temp,"BN") == 0)) {      /* EMBL third-party annotation */
              retcode = ACCN_EMBL_TPA;
          } else if ((StringICmp(temp,"BR") == 0)) {      /* DDBJ third-party annotation */
              retcode = ACCN_DDBJ_TPA;
          } else if ((StringICmp(temp,"AJ") == 0) ||
                     (StringICmp(temp,"AM") == 0)) {     /* EMBL direct submission */
              retcode = ACCN_EMBL_DIRSUB;
          } else if ((StringICmp(temp,"AL") == 0) ||
                     (StringICmp(temp,"BX") == 0)||
                     (StringICmp(temp,"CR") == 0)) {      /* EMBL genome project data */
              retcode = ACCN_EMBL_GENOME;
          } else if ((StringICmp(temp,"AN") == 0)) {      /* EMBL CON division */
              retcode = ACCN_EMBL_CON;
          } else if ((StringICmp(temp,"AX") == 0) ||
                     (StringICmp(temp,"CQ") == 0) ||
                     (StringICmp(temp,"CS") == 0)) {      /* EMBL patent division */
              retcode = ACCN_EMBL_PATENT;
          } else if ((StringICmp(temp,"AT") == 0) || 
                     (StringICmp(temp,"AU") == 0) ||
                     (StringICmp(temp,"AV") == 0) ||
                     (StringICmp(temp,"BB") == 0) ||
                     (StringICmp(temp,"BJ") == 0) ||
                     (StringICmp(temp,"BP") == 0) ||
                     (StringICmp(temp,"BW") == 0) ||
                     (StringICmp(temp,"BY") == 0) ||
                     (StringICmp(temp,"CI") == 0) ||
                     (StringICmp(temp,"CJ") == 0) ||
                     (StringICmp(temp,"DA") == 0) ||
                     (StringICmp(temp,"DB") == 0) ||
                     (StringICmp(temp,"DC") == 0)) {      /* DDBJ EST's */
              retcode = ACCN_DDBJ_EST;
          } else if ((StringICmp(temp,"AB") == 0)) {      /* DDBJ direct submission */
              retcode = ACCN_DDBJ_DIRSUB;
          } else if ((StringICmp(temp,"AG") == 0) || 
                     (StringICmp(temp,"AP") == 0) || 
                     (StringICmp(temp,"BS") == 0)) {      /* DDBJ genome project data */
              retcode = ACCN_DDBJ_GENOME;
          } else if ((StringICmp(temp,"AK") == 0))  {     /* DDBJ HTGS */
              retcode = ACCN_DDBJ_HTGS;
          } else if ((StringICmp(temp,"BA") == 0)) {      /* DDBJ CON division */
              retcode = ACCN_DDBJ_CON;
          } else if ((StringICmp(temp,"BD") == 0) ||
                     (StringICmp(temp,"DD") == 0)) {      /* DDBJ patent division */
              retcode = ACCN_DDBJ_PATENT;
          } else if ((StringICmp(temp,"DE") == 0)) {      /* DDBJ GSS */
              retcode = ACCN_DDBJ_GSS;
          } else {
              retcode = ACCN_IS_NT;
              break;
          }
      
          while (*s) {
              if (! IS_DIGIT(*s)) {
                  retval = FALSE;
                  break;
              }
              s++;
          }  
          break;
      } else {
          retval = FALSE;
          break;
      }
      break;
    case 9: /* New 9-character accession, two letters +"_"+ 6 digits */
      if(!IS_ALPHA(*s) || !IS_ALPHA(*(s+1)))
          break;
      if(*(s+2)!='_')
          break;
      /* New(1999) 8-character protein accession, three letters + 5 digits */
      temp[0] = *s; s++;
      temp[1] = *s; s++;
      temp[2] = NULLB; s++;
      
      if ((StringICmp(temp,"NP") == 0)) { 
          retcode = ACCN_REFSEQ_PROT;
      } else if ((StringICmp(temp,"NM") == 0)) { 
          retcode = ACCN_REFSEQ_mRNA;
      } else if ((StringICmp(temp,"NT") == 0)) { 
          retcode = ACCN_REFSEQ_CONTIG;
      } else if ((StringICmp(temp,"NC") == 0)) { 
          retcode = ACCN_REFSEQ_CHROMOSOME;
      } else if ((StringICmp(temp,"XM") == 0)) { 
          retcode = ACCN_REFSEQ_mRNA_PREDICTED;
      } else if ((StringICmp(temp,"XP") == 0)) { 
          retcode = ACCN_REFSEQ_PROT_PREDICTED;
      } else if ((StringICmp(temp,"NG") == 0)) { 
          retcode = ACCN_REFSEQ_GENOMIC;
      } else if (IS_ALPHA(*temp) && IS_ALPHA(*(temp+1))) {
          retcode =ACCN_REFSEQ | ACCN_AMBIGOUS_MOL;
      } else
          retval = FALSE;
      while (*s) {
          if (! IS_DIGIT(*s)) {
              retval = FALSE;
              break;
          }
          s++;
      }
      break;
    case 11: /* New 11-character accession, two letters +"_"+ 8 digits */
      if(!IS_ALPHA(*s) || !IS_ALPHA(*(s+1)))
          break;
      if(*(s+2)!='_')
          break;
      temp[0] = *s; s++;
      temp[1] = *s; s++;
      temp[2] = NULLB; s++;
      
      if ((StringICmp(temp,"ZP") == 0)) { 
          retcode = ACCN_REFSEQ_PROT_PREDICTED;
      } else
          retval = FALSE;
      while (*s) {
          if (! IS_DIGIT(*s)) {
              retval = FALSE;
              break;
          }
          s++;
      }
      break;
    case 12:
      if(IS_ALPHA(*s) && IS_ALPHA(*(s+1)) && IS_ALPHA(*(s+2)) && IS_ALPHA(*(s+3))) {
        /* whole genome shotgun 12-character accession, four letters + 8 digits */
        temp[0] = *s; s++;
        temp[1] = *s; s++;
        temp[2] = *s; s++;
        temp[3] = *s; s++;
        temp[4] = '\0';
        if ((StringNICmp(temp,"A", 1) == 0)) { 
          retcode = ACCN_NCBI_WGS;
        } else if ((StringNICmp(temp,"B", 1) == 0)) {
          retcode = ACCN_DDBJ_WGS;
        } else if ((StringNICmp(temp,"C", 1) == 0)) {
          retcode = ACCN_EMBL_WGS;
        } else
          retval = FALSE;
        while (*s) {
          if (! IS_DIGIT(*s)) {
              retval = FALSE;
              break;
          }
          s++;
        }
      } else if(IS_ALPHA(*s) && IS_ALPHA(*(s+1)) && (*(s+2)=='_')) {
        /* New 12-character accession, two letters +"_"+ 9 digits */
        temp[0] = *s; s++;
        temp[1] = *s; s++;
        temp[2] = NULLB; s++;
      
        if ((StringICmp(temp,"NP") == 0)) { 
          retcode = ACCN_REFSEQ_PROT;
        } else if ((StringICmp(temp,"NM") == 0)) { 
          retcode = ACCN_REFSEQ_mRNA;
        } else if (IS_ALPHA(*temp) && IS_ALPHA(*(temp+1))) {
          retcode =ACCN_REFSEQ | ACCN_AMBIGOUS_MOL;
        } else
          retval = FALSE;
        while (*s) {
          if (! IS_DIGIT(*s)) {
              retval = FALSE;
              break;
          }
          s++;
        }
      }
      break;
  default:   
    retval = FALSE;
    break;
  }                     /* Endswitch, StringLen(s) */
 
  return (retval ? retcode : ACCN_UNKNOWN);
}

/****************************************************************************
*
*  Function:    IS_ntdb_accession
*
*  Description: Return TRUE if the input string is a validly formatted
*               nucleotide database accession number (GenBank, EMBL, DDBJ, REFSEQ)
*  ***WARNING*** DOES NO network access, relies on hardcoding in WHICH_db_accession.
*
*  Arguments:   s : CharPtr; pointer to accession number string.
*                   Must be null terminated.
*
*  Author:      Mark Cavanaugh, Hugues Sicotte
*  Date:        7/96,HS 12/2000
*
*  WARNING:     IS_ntdb_accession() does not communicate with any central
*               resource about accession numbers. So there's no way to
*               inform it automatically about new accession number prefixes.
*
*               Version Number ".integer" MUST have been stripped out
*               before calling this function.
*****************************************************************************/

NLM_EXTERN Boolean LIBCALL IS_ntdb_accession (CharPtr s) {
    Uint4 status;
    status = WHICH_db_accession(s);
    return (Boolean)(ACCN_IS_NUC(status));
}

/*****************************************************************************
*
*  Function:    IS_protdb_accession
*
*  Description: Return TRUE if the input string is a validly formatted
*               protein database accession number (SWISS-PROT)
*               or the new 3 letter protein ID.
* 
*  ***WARNING*** DOES NO network access, relies on hardcoding in WHICH_db_accession.
*
*  Arguments:   s : CharPtr; pointer to accession number string.
*                   Must be null terminated.
*
*  Author:      Mark Cavanaugh, Hugues Sicotte (3/99)
*  Date:        8/96, 3/99HS,12/2000
*
*  WARNING:     IS_protdb_accession() does not communicate with any central
*               resource about accession numbers. So there's no way to
*               inform it automatically about new accession number prefixes.
*
*               Version Number ".integer" MUST have been stripped out
*               before calling this function.
*****************************************************************************/

NLM_EXTERN Boolean LIBCALL IS_protdb_accession (CharPtr s) {
    Uint4 status;
    status = WHICH_db_accession(s);
    return (Boolean)(ACCN_IS_PROT(status));
}

/*
  Try to Find if the Bioseq represented by a SeqId is a SeqLoc List;
  May fetch the Bioseq to get all the synonymous SeqIds.
 */

NLM_EXTERN Boolean LIBCALL SeqIdInSeqLocList(SeqIdPtr sip, ValNodePtr list) {
  ValNodePtr vnptmp;
  SeqIdPtr   siptmp;
  SeqLocPtr slp;

  for (vnptmp=list; vnptmp!=NULL; vnptmp=vnptmp->next)
  {
     siptmp = SeqLocId((SeqLocPtr)vnptmp->data.ptrvalue);
     if (siptmp!=NULL) {
        if (SeqIdForSameBioseq(sip, siptmp))
           return TRUE;
     } else if((slp=(SeqLocPtr)vnptmp->data.ptrvalue)!=NULL && (
               slp->choice == SEQLOC_PACKED_INT || 
               slp->choice == SEQLOC_MIX || 
               slp->choice == SEQLOC_EQUIV)) {
         slp = (SeqLocPtr)slp->data.ptrvalue;
         while(slp!=NULL) {
             siptmp = SeqLocId(slp);
             if (siptmp!=NULL) {
                 if (SeqIdForSameBioseq(sip, siptmp))
                     return TRUE;
             }
             slp=slp->next;
         }
     }
  }
  return FALSE;
}

/*********************************************************
***
***    AddSeqId  : create a new seqid and add at the end
***                of the list starting with sip_head
***
**********************************************************/
NLM_EXTERN SeqIdPtr AddSeqId (SeqIdPtr *sip_head, SeqIdPtr sip)
{
  SeqIdPtr sip_tmp, 
           sip_copy;

  sip_copy = SeqIdDup (sip);
  sip_tmp = sip_copy->next;
  sip_copy->next = NULL;
  if (sip_tmp!=NULL)
     SeqIdFree (sip_tmp);
  if ( (sip_tmp = *sip_head) != NULL ) {
     while (sip_tmp->next != NULL) 
        sip_tmp = sip_tmp->next; 
     sip_tmp->next = sip_copy;
  }  
  else {
     *sip_head = sip_copy;
  }
  return (*sip_head);

}

/*******************************************************
***
***   SeqIdDupList : duplicate a list of SeqIdPtr
***
*******************************************************/
NLM_EXTERN SeqIdPtr SeqIdDupList (SeqIdPtr id_list)
{
  SeqIdPtr     sip=NULL;
  SeqIdPtr     sid;

  for (sid = id_list; sid != NULL; sid = sid->next) {
         sip = AddSeqId (&sip, sid);  
  }
  return sip;
}

NLM_EXTERN SeqIdPtr SeqIdDupBestList (SeqIdPtr id_list)
{
  SeqIdPtr     sip=NULL;
  SeqIdPtr     sid, sid2;
  BioseqPtr    bsp;

  for (sid = id_list; sid != NULL; sid = sid->next) {
     sid2 = NULL;
     bsp = BioseqLockById (sid);
     if (bsp!=NULL) {
        sid2 = SeqIdFindBest(bsp->id, 0);
        BioseqUnlock (bsp);
     }
     if (sid2!=NULL)
        sip = AddSeqId (&sip, sid2);
     else 
        sip = AddSeqId (&sip, sid);  
  }
  return sip;
}

NLM_EXTERN SeqIdPtr SeqIdListfromSeqLoc (ValNodePtr vnpslp)
{
  SeqIdPtr     sip=NULL, siptmp;
  ValNodePtr   vnp=NULL;
  Int2         j = 0, k;
  for (vnp = vnpslp; vnp != NULL; vnp = vnp->next)
  {
         sip = AddSeqId (&sip, SeqLocId ((SeqLocPtr) vnp->data.ptrvalue));  
         j++;
  }
  if (sip!=NULL) {
     for (siptmp=sip, k=0; k<j-1; siptmp=siptmp->next, k++) continue;
     siptmp->next = NULL;
  }
  return sip;
}
