static char const rcsid[] = "$Id: spidey.c,v 6.71 2005/04/26 21:34:39 kans Exp $";

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
* File Name:  spidey.c
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   5/01
*
* $Revision: 6.71 $
*
* File Description: mrna-to-genomic alignment algorithms and functions
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: spidey.c,v $
* Revision 6.71  2005/04/26 21:34:39  kans
* added SEQID_GPIPE
*
* Revision 6.70  2005/02/22 17:41:59  kskatz
* fixed potential dividing by zero in SPI_is_acceptor* and SPI_is_donor* probability calculations
*
* Revision 6.69  2004/04/09 16:05:21  kskatz
* Added sanity check (must be 3 intervals to go through the loop) to SPI_CheckMrnaOrder()
*
* Revision 6.68  2004/03/25 21:20:03  kskatz
* All SPI_is_acceptor_* functions have been corrected: 'N' no longer contributes to nor subtracts from the score, log odds are calculated and the scores added; they are however all antilogged because there are too many places in the code where the score is expected to be between 0 and 1.  Also, corrected sequence frequency determination in SPI_is_acceptor_user and SPI_is_donor_user, as well as correcting for 'N'. Finally, and this all began with, I added matrices for Dictyostelium - command line -r -m
*
* Revision 6.67  2003/12/12 21:25:26  kskatz
* Fixed bug in SPI_CheckForPolyAExon() where multiple SeqAlignPtr's to the same object were not handled carefully: one of the ptr's was being accessed when the object was freed via the other ptr.
*
* Revision 6.66  2003/12/12 17:57:04  kskatz
* Fixed a potential array bounds read error in SPI_CheckMrnaOrder()
*
* Revision 6.65  2003/12/10 16:53:22  kskatz
* Ensured that 'ovl' when used is never negative once set in SPI_AdjustOverlaps() [see revision 6.57]
*
* Revision 6.64  2003/10/21 15:26:17  kans
* fixed typo of SPI_IvalPt to SPI_IvalPtr
*
* Revision 6.63  2003/10/21 15:14:19  kskatz
 * Added SPI_CheckMrnaOrder(): Called by GetRegionForSAP() after the ivals for building a region are sorted in genomic order, this function merely checks that the mrna invterals are minimally colinear.
*
* Revision 6.62  2003/10/06 14:11:20  kskatz
* Changed the 'version' number printed by SPI_PrintResult() to '1.40' since it has been '1.35' for so long - mostly to avoid confusion when users report the version number
*
* Revision 6.61  2003/10/06 14:04:09  kskatz
* Correctly! commented out a temporary fix in SPI_AlignInWindows() [line 3880]
*
* Revision 6.60  2003/09/17 20:39:01  kskatz
* Commented out a temporary fix in SPI_AlignInWindows() [line 3880]
*
* Revision 6.59  2003/09/17 19:53:27  kskatz
* Added a check in SPI_FindBestAlnByDotPlot() that both seqs be 2-bit encoded (ncbi2na) in order to meet that implicit requirement of DOT_. If either one is not ncbi2na, SPI_FindBestAlnByDotPlot() will simply return NULL.
*
* Revision 6.58  2003/08/18 18:17:51  kskatz
* Just removing some unused vars
*
* Revision 6.57  2003/08/18 18:11:39  kskatz
* Fixed dynamic allocation of buf in SPI_AdjustOverlaps() - 'ovl' can be negative
*
* Revision 6.56  2003/08/15 15:23:50  kskatz
* Created Choose2LooseMrnaOvLap(), called by SPI_AdjustForSplice(): returns the SeqAlignPtr * to delete *. The choice is based on score + splice donor/acceptor existencefor that 'exon'. Also made buf2 in SPI_AdjustOverlaps() dynamically allocated as it was (obviously) crashing when the overlaps was > 200 bases.
*
* Revision 6.55  2003/06/30 15:01:29  whlavina
* Correct minus strand handling in CreaeContinuousAln functions; previous
* code could corrupt alignments (stop2-start1>1 would imply len<-2 if
* ExtendAlnRight ever gets called).
*
* Revision 6.54  2003/05/30 17:25:38  coulouri
* add rcsid
*
* Revision 6.53  2003/04/04 19:42:56  kskatz
* Added a new command line option (-R) to allow external users to point spidey to a repeat database that it can pass on to blast for filtering repeats
*
* Revision 6.52  2002/11/14 17:20:38  johnson
* fixed nasty memory misallocation bug in SPI_CheckSplicesForRevComp
*
* Revision 6.51  2002/11/04 19:48:35  kskatz
* wasn't correcting for strand when reporting summary mismatch information in SPI_PrintResults()
*
* Revision 6.50  2002/10/10 19:39:45  kskatz
* Added 'mismatches' to output in SPI_PrintResult(), as well as commented out several unused variables, and two syntax fixes to avoid compiler warning
*
* Revision 6.49  2002/10/02 16:47:11  kskatz
* clarifying the explanation of the -L option
*
* Revision 6.48  2002/10/02 16:12:53  kskatz
* Added a new option to SPI_Options (bigintron_size) that holds a user-supplied maximum size (default = 220000) for introns and requires the option (bool) bigintron to be set to 'TRUE'; The functions affected are SPI_mRNAPtr SPI_AdjustForSplice(), SPI_is_consistent(), and SPI_FindPiece(); note that the default for bigintron_size is not set in SPI_OptionsNew() (yet)
*
* Revision 6.47  2002/08/28 17:02:51  kskatz
* Simplified the loop in SPI_PrintResults() that prints out the 5' splice site and allowed minimum of 2 bases; also fixed more deadly access errors by setting the SPI_RegionInfoPtr *srip to NULL when all regions fall below spot->idcutoff in SPI_SortRegionsByScore()
*
* Revision 6.46  2002/08/26 20:00:05  kskatz
* Fixed off-by-one error in my fix to SPI_PrintResult
*
* Revision 6.45  2002/08/20 21:07:12  kskatz
* Fixed several NULL pointer access errors caused when -c results in the deletion of all regions in SPI_SortRegionsByScore(); also fixed bugs in SPI_PrintResult() caused by not checking to see if a minus strand alignment had start at the end of the sequence when printing out the 10-base buffer of the splice region
*
* Revision 6.44  2002/08/17 03:08:16  kskatz
* allowed to & from to be handled independently in SPI_AlnSinglemRNAToGen()
*
* Revision 6.43  2002/08/16 22:31:55  kskatz
* oops again - changed c++ style comments to c style since this is the c toolkit
*
* Revision 6.42  2002/08/16 21:15:50  kskatz
* oops - this is C toolkit: int -> Int4
*
* Revision 6.41  2002/08/16 21:03:12  kskatz
* SPI_OptionsNew() now sets strand = Seq_strand_both as default, otherwise blast results are hosed; SPI_AlnSinglemRNAToGen() now correctly limits initial blast to user supplied to/from and includes a little sanity check
*
* Revision 6.40  2002/07/22 13:40:55  wheelan
* changes to splice matrices, bug fix in CDS computation
*
* Revision 6.39  2002/06/27 12:59:34  kans
* fix in call to GetScoreAndEvalue
*
* Revision 6.38  2002/06/27 11:52:53  wheelan
* various bug fixes -- fixed off-by-one splice site errors and more
*
* Revision 6.37  2002/05/07 19:15:09  wheelan
* fixed minor bug in splice boundary arithmetic
*
* Revision 6.36  2002/05/07 18:42:56  wheelan
* changes to support user-defined splice matrices
*
* Revision 6.35  2002/04/04 17:18:20  wheelan
* numerous bug fixes and little changes; added SPI_CheckForPolyAExon
*
* Revision 6.34  2002/01/30 19:09:05  wheelan
* better support for revcomp, plus changes for new alignment manager funcs
*
* Revision 6.33  2001/12/18 18:00:01  wheelan
* bug fix for NULL segs in RemoveTeenyAln
*
* Revision 6.32  2001/12/13 12:28:51  wheelan
* fixed bug in multiple printing, bug in ConnectAln
*
* Revision 6.31  2001/12/10 15:58:04  wheelan
* fixed dereferencing of null variable in ConnectAln
*
* Revision 6.30  2001/12/10 14:42:36  wheelan
* bug fix in ConnectAln -- no more using freed pointers
*
* Revision 6.29  2001/12/05 12:29:37  wheelan
* changed to version 1.2
*
* Revision 6.28  2001/11/30 12:15:03  wheelan
* subtle but very important bug fix in SPI_GetNthSeqRangeInSASet
*
* Revision 6.27  2001/11/20 12:13:24  wheelan
* made SPI_GetProteinFrommRNA EXTERN
*
* Revision 6.26  2001/11/05 16:17:11  wheelan
* added option to print multiple alignment to a file
*
* Revision 6.25  2001/11/02 14:00:52  wheelan
* fixed memory access errors in splice printing code
*
* Revision 6.24  2001/10/26 13:12:07  wheelan
* changes to polyA handling, plus bulletproofing
*
* Revision 6.23  2001/10/18 15:45:56  wheelan
* bug fix in ConnectAln
*
* Revision 6.22  2001/10/18 15:12:22  wheelan
* fixed polyAtail alignment problems, fixed score calculation
*
* Revision 6.21  2001/10/17 16:16:21  wheelan
* changes in region sorting plus mrna model gap handling
*
* Revision 6.20  2001/10/08 17:16:44  wheelan
* bug fix in revcmp, made cds 1-based coords, fixed polyA bug
*
* Revision 6.19  2001/10/04 12:36:21  wheelan
* implemented bigintron option; made SPI_ConnectAln run through twice to pick up more pieces
*
* Revision 6.18  2001/10/03 18:09:54  wheelan
* changed AM_LITE define for new alnmgr
*
* Revision 6.17  2001/10/03 14:19:53  wheelan
* change names of all alignmgr calls, plus add profile-making code
*
* Revision 6.15  2001/09/07 12:15:25  wheelan
* small fix for reverse complement translation
*
* Revision 6.14  2001/09/07 12:05:17  wheelan
* moved protein translation for convenient use on web
*
* Revision 6.13  2001/09/07 11:47:32  wheelan
* fixed coordinates and translation for reverse complement cases
*
* Revision 6.12  2001/09/04 13:46:47  wheelan
* made SPI_RemoveInconsistentAlnsFromSet and SPI_flip_sa_list extern
*
* Revision 6.11  2001/08/24 23:27:15  wheelan
* removed unwanted semicolon
*
* Revision 6.10  2001/08/24 13:45:20  wheelan
* better region sorting (better scores), plus different printing options added
*
* Revision 6.9  2001/08/20 21:28:34  wheelan
* improved relative scoring of initial regions, added seqid types
*
* Revision 6.8  2001/07/20 10:31:10  wheelan
* fixed uninitialized variable plus another polyA mistake
*
* Revision 6.7  2001/07/19 18:22:36  wheelan
* better handling of polyA tails
*
* Revision 6.6  2001/07/11 17:56:53  wheelan
* added more functions to deal with making multiple alignments
*
* Revision 6.5  2001/07/10 16:44:53  wheelan
* added functions to make a multiple alignment
*
* Revision 6.4  2001/07/06 10:27:21  wheelan
* fixed minor things pointed out by D. Vakatov
*
* Revision 6.3  2001/06/25 17:00:47  wheelan
* frame fix in GetProteinFrommRNA
*
* Revision 6.2  2001/06/22 20:54:49  wheelan
* spidey now tries to make as many alignments as requested, even if that means throwing away the "best" regions if they have no alignment
*
* Revision 6.1  2001/05/24 16:28:10  wheelan
* initial checkin
*
*
* ==========================================================================
*/

#include <spidey.h>


static int LIBCALLBACK SPI_CompareAlnPosForMult(VoidPtr ptr1, VoidPtr ptr2);
static Boolean spi_overlaps(SeqAlignPtr sap, SPI_BlockPtr sbp);
static void SPI_BeautifySMP(SPI_RegionInfoPtr srip);
static void SPI_RemoveOutsideBounds(SeqAlignPtr sap, SPI_OptionsPtr spot);
static void SPI_PadRegions(SPI_RegionInfoPtr srip, Int4 bsplen);
static void SPI_TossRegions(SPI_RegionInfoPtr PNTR srip, SPI_OptionsPtr spot);
static int LIBCALLBACK SPI_CompareInitialRegions(VoidPtr ptr1, VoidPtr ptr2);
static int LIBCALLBACK SPI_compare_aln_score(VoidPtr ptr1, VoidPtr ptr2);
static void SPI_SortRegionsByScore(SPI_RegionInfoPtr PNTR srip, SPI_OptionsPtr spot);
static int LIBCALLBACK SPI_CompareRegions(VoidPtr ptr1, VoidPtr ptr2);
static void SPI_PrintAce(FILE *ofp, SPI_RegionInfoPtr srip, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna, Boolean is_cds);
static void SPI_PrintResult(FILE *ofp, FILE *ofp2, SPI_RegionInfoPtr srip, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna, SPI_OptionsPtr spot, Boolean isitCDS);
static void SPI_PrintHerdResult(FILE *ofp, FILE *ofp2, SPI_mRNAToHerdPtr herd, SPI_OptionsPtr spot, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna);
static void spi_print_mismatch_line(FILE *ofp, Int4 exonnum, Int4 start, Int4 len, SPI_ExonProfPtr epp, Int4 gstart);
static SeqAlignPtr SPI_CreateContinuousAln(SeqAlignPtr PNTR saps, Int4 numsaps);
static void SPI_ExtendAlnRight(SeqAlignPtr sap, Int4 which_row, Int4 start, Int4 stop);
static SPI_mRNAToHerdPtr SPI_GetHerdInfo(SPI_FragHerdPtr sfhp, BioseqPtr bsp_mrna, SPI_OptionsPtr spot);
static SPI_RegionInfoPtr SPI_FindWindows(SeqAlignPtr sap, SPI_OptionsPtr spot);
static int LIBCALLBACK SPI_compare_aln_score(VoidPtr ptr1, VoidPtr ptr2);
static SPI_RegionInfoPtr SPI_SortRegions(SPI_RegionInfoPtr srip_head);
static int LIBCALLBACK SPI_SortSrips(VoidPtr ptr1, VoidPtr ptr2);
static SPI_RegionInfoPtr SPI_AssembleRegions(SPI_AlnInfoPtr PNTR spip_list, Int4 num, SPI_RegionInfoPtr PNTR head_srip, SPI_OptionsPtr spot);
static SPI_RegionInfoPtr SPI_GetRegionForSAP(SPI_IvalPtr PNTR siip_list, Int4 num, SeqAlignPtr sap, SPI_OptionsPtr spot);
static Int2 SPI_is_consistent(SPI_IvalPtr siip, SPI_RegionInfoPtr srip, SPI_OptionsPtr spot);
static int LIBCALLBACK SPI_compare_genomic_loc(VoidPtr ptr1, VoidPtr ptr2);
static void SPI_ExcludeOverlaps(SPI_IvalPtr PNTR siip_list, Int4 num, SPI_RegionInfoPtr srip);
static void SPI_AlignInWindows(SPI_RegionInfoPtr PNTR head_srip, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna, SPI_OptionsPtr spot);
static void SPI_DoAln(SPI_RegionInfoPtr srip, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna, SPI_OptionsPtr spot);
static Boolean SPI_ConnectAln(SeqAlignPtr sap, SPI_OptionsPtr spot, SPI_RegionInfoPtr srip, Boolean do_ends, Boolean firsttime);
static SeqAlignPtr SPI_ProcessNewAlns(SeqAlignPtr sap);
static Int4 SPI_IsItPolyA(SeqIdPtr sip);
static SeqAlignPtr SPI_FillInIntron(SeqIdPtr sip1, SeqIdPtr sip2, Int4 start1, Int4 stop1, Int4 start2, Int4 stop2, Uint1 strand2, SPI_OptionsPtr spot);
static Int4 spi_isa_gap(Int4 start, Int4 prevstop, Uint1 strand);
static Int4 SPI_GetNthSeqLenInSASet(SeqAlignPtr sap, Int4 n, Int4Ptr numsaps);
static void SPI_GetNthSeqRangeInSASet(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop);
static SeqAlignPtr SPI_FindPiece(SeqIdPtr sip1, SeqIdPtr sip2, Int4 start_m, Int4 stop_m, Uint1 strand, Int4 start_g, Int2 which_end, SPI_OptionsPtr spot);
static SPI_mRNAPtr SPI_AdjustForSplice(SeqAlignPtr sap, SPI_OptionsPtr spot, SPI_RegionInfoPtr srip);
static Int4 SPI_GetExonInfo(SPI_mRNAPtr smp, Int4 n, Int4Ptr start, Int4Ptr stop, Int4Ptr mis, SPI_OptionsPtr spot);
static void SPI_AdjustOverlaps(SeqAlignPtr sap1, SeqAlignPtr sap2, Int4 n, SPI_mRNAPtr smp, SPI_OptionsPtr spot);
static void SPI_RemoveTeenyAlns(SeqAlignPtr sap, Int4 len);
static void SPI_ExtendAlnAlg(SeqAlignPtr sap, Int4 ovl, Int4 which_side, Uint1 strand);
static void SPI_ExtendAlnAlgDumb(SeqAlignPtr sap, Int4 ovl, Int4 which_side, Uint1 strand);
static void SPI_GetAcceptorScore(BioseqPtr bsp, Int4 pos1, Int4 pos2, Uint1 strand, FloatHiPtr score, Int4 spllen, SPI_OptionsPtr spot);
static Int4 spi_get_overlap (SeqAlignPtr sap1, SeqAlignPtr sap2);
static void SPI_AddToAln(SeqAlignPtr sap, Int4 offset, Int2 which_end, Uint1 strand);
static SeqAlignPtr SPI_MergeAlignments(SeqAlignPtr sap1, SeqAlignPtr sap2);
static SeqAlignPtr SPI_FillInLastmRNAHoles(SeqAlignPtr sap, SeqIdPtr sip_genomic, SeqIdPtr sip_mrna, Int4 start_g, Int4 stop_g, Int4 start_m, Int4 stop_m, Uint1 strand);
static SeqAlignPtr SPI_FindBestAlnByDotPlot(SeqLocPtr slp1, SeqLocPtr slp2);
static int LIBCALLBACK SPI_comp_aln_pos(VoidPtr ptr1, VoidPtr ptr2);
static void SPI_RegionFree (SPI_RegionInfoPtr srip);
static void SPI_FreeExonProf(SPI_ExonProfPtr epp);
static void SPI_FreeExonProfList(SPI_ExonProfPtr epp);
static void SPI_GetDonorSpliceInfo (Int4 org, Int4Ptr spllen, Int4Ptr boundary, SPI_OptionsPtr spot);
static void SPI_is_donor_user(Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score, SPI_OptionsPtr spot);
static void SPI_is_donor_vert (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score);
static void SPI_is_donor_fly (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score);
static void SPI_is_donor_plant (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score);
static void SPI_is_donor_cele (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score);
static void SPI_is_donor_dicty (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score);
static void SPI_GetAcceptorSpliceInfo (Int4 org, Int4Ptr spllen, Int4Ptr boundary, SPI_OptionsPtr spot);
static void SPI_is_acceptor_user(Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score, SPI_OptionsPtr spot);
static void SPI_is_acceptor_vert (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score);
static void SPI_is_acceptor_fly (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score);
static void SPI_is_acceptor_plant (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score);
static void SPI_is_acceptor_cele (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score);
static void SPI_is_acceptor_dicty (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score);
static void SPI_RemoveConflictsAmongPieces(SPI_FragHerdPtr sfhp, Int4 fuzz);
static void SPI_OrderPieces(SPI_FragHerdPtr sfhp, BioseqPtr bsp_mrna);
static int LIBCALLBACK SPI_CompareFragInfo(VoidPtr ptr1, VoidPtr ptr2);
static Boolean SPI_ConnectAlnPieces(SPI_FragHerdPtr sfhp, BioseqPtr bsp_contig, BioseqPtr bsp_mrna, SPI_OptionsPtr spot);
static void SPI_CleanupAndGetNewmRNARange(SPI_FragPtr PNTR sfpnearby, Int4 n, Int4Ptr start, Int4Ptr stop);
static Int4 SPI_GetNearbyFrags(SPI_FragPtr sfptarget, Int4 n, SPI_FragPtr ** ptrptr, SPI_FragHerdPtr sfhp, Boolean minus);
static void SPI_AdjustSplicesInPieces(SPI_FragHerdPtr sfhp, BioseqPtr bsp_genomic, SPI_OptionsPtr spot);
static void SPI_AdjustEndsOfPieces(SPI_FragPtr sfp1, SPI_FragPtr sfp2, BioseqPtr bsp_genomic, SPI_OptionsPtr spot);
static SeqAlignPtr SPI_GetNthSAByRow(SeqAlignPtr sap, Int4 row, Int4 n);
static SPI_FragSplPtr SPI_GetPossibleSites(SeqAlignPtr sap, BioseqPtr bsp_genomic, SPI_OptionsPtr spot, Boolean donor, Int4 ovl);
static void SPI_FragSplFree(SPI_FragSplPtr fsp);
static SeqLocPtr SPI_SeqLocListDup(SeqLocPtr slp);
static int LIBCALLBACK SPI_CompareSpins(VoidPtr ptr1, VoidPtr ptr2);
static void SPI_OrderInternally(SPI_FragHerdPtr sfhp);
static int LIBCALLBACK SPI_CompareAlnPos(VoidPtr ptr1, VoidPtr ptr2);
static SPI_RegionInfoPtr SPI_GetResultsForCDS(SPI_RegionInfoPtr srip_mrna, BioseqPtr bsp_mrna, SPI_OptionsPtr spot);
static void SPI_FillInUTRInfo(SPI_RegionInfoPtr srip_cds, SPI_RegionInfoPtr srip_mrna, Int4 len, Int4 exonstart, Int4 exonstop);
static Boolean LIBCALLBACK SPI_GetCDS(SeqFeatPtr sfp, SeqMgrFeatContextPtr context);
static void SPI_ShiftAlignment(SeqAlignPtr sap, Int4 offset, Int4 row);
static Boolean LIBCALLBACK SPI_GetCDSFeat(SeqFeatPtr sfp, SeqMgrFeatContextPtr context);
static Int4 SPI_FindLongestProt(CharPtr seq, Int4Ptr pos);
static Boolean SPI_GetAccessionFromSeqId(SeqIdPtr sip, Int4Ptr gi, CharPtr PNTR id);
static void SPI_CheckSplicesForRevComp(SPI_RegionInfoPtr srip, SPI_OptionsPtr spot, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna);
static ACTProfilePtr SPI_ProfileNew(Boolean nuc);
static ACTProfilePtr SPI_ProfileFree(ACTProfilePtr app);
static ACTProfilePtr SPI_ProfileSetFree(ACTProfilePtr app);
static void SPI_BuildProfile(SeqLocPtr slp, ACTProfilePtr PNTR app, Int4Ptr count, Int4 length);
static FloatHi SPI_ScoreProfile(BioseqPtr bsp, Int4 pos, Uint1 strand, ACTProfilePtr app);
static ACTProfilePtr SPI_MakeProfileFromSA(SeqAlignPtr sap);
static int SPI_Choose2LooseMrnaOvLap (const SeqAlignPtr sap1, const SeqAlignPtr sap2, const SPI_mRNAPtr smp, const int ptr1offset);     
static void SPI_CheckMrnaOrder(SPI_IvalPtr PNTR spi_pp, const int num);


/***************************************************************************
*  
*  SPI_AlnmRNAToGenomic is available to outside programs; just pass in the two
*  bioseqs and options (to use default options, just pass in NULL, and to use 
*  other options, call SPI_OptionsNew to get an initialized options pointer and
*  make the desired changes).  If options are passed in, they should be freed
*  using SPI_OptionsFree.  SPI_AlignmRNAToGenomic returns a linked list of
*  SPI_mRNAPtrs, one per gene model (default is to only return one gene model).
*  Each SPI_mRNAPtr (see spidey.h) has arrays specifying the exon boundaries in
*  genomic and mRNA coordinates as well as information about splice sites,
*  percent identity, number of gaps, etc.  The SPI_mRNAPtr also has one alignment
*  per exon as well as a single alignment (smp->continuous) that covers the entire
*  gene, with big gaps in the mRNA for the genomic introns.  The SPI_mRNAPtr should
*  be freed by the calling function, using SPI_mRNAFree.
*
*  SPI_AlnmRNAToGenomic should only be used on finished sequence; it can handle
*  interspecies comparisons but doesn't work on draft sequence.
*
***************************************************************************/
NLM_EXTERN SPI_mRNAPtr SPI_AlignmRNAToGenomic(BioseqPtr bsp_genomic, BioseqPtr bsp_mrna, SPI_OptionsPtr spot)
{
   Int4               a;
   Int4               i;
   Boolean            lcl;
   SeqAlignPtr        parent;
   SPI_Progress       progress;
   SeqAlignPtr        PNTR saparray;
   SPI_mRNAPtr        smp_head;
   SPI_mRNAPtr        smp_prev;
   SPI_bsinfoPtr      spig;
   SPI_bsinfoPtr      spim;
   SPI_OptionsPtr     spot_lcl;
   SPI_RegionInfoPtr  srip;
   SPI_RegionInfoPtr  srip_head;

   if (bsp_genomic == NULL || bsp_mrna == NULL)
      return NULL;
   a = SPI_IsItPolyA(bsp_mrna->id);
   if (spot == NULL)
   {
      spot_lcl = SPI_OptionsNew();
      lcl = TRUE;
   } else
   {
      spot_lcl = spot;
      lcl = FALSE;
   }
   spig = (SPI_bsinfoPtr)MemNew(sizeof(SPI_bsinfo));
   spig->bsp = bsp_genomic;
   spim = (SPI_bsinfoPtr)MemNew(sizeof(SPI_bsinfo));
   spim->bsp = bsp_mrna;
   srip_head = SPI_AlnSinglemRNAToGen(spig, spim, NULL, NULL, spot_lcl);
   if (spot_lcl->callback != NULL)
   {
      progress.percentdone = 100;
      progress.returncode = SPI_FINISHED;
      if (!spot_lcl->callback(&progress))
         return NULL;
   }
   smp_head = smp_prev = NULL;
   srip = srip_head;
   while (srip != NULL)
   {
      if (srip->smp->polyAtail == 0)
         srip->smp->polyAtail = 0-a;
      srip->smp->revcomp = srip->revcomp;
      if (smp_head != NULL)
      {
         smp_prev->next = srip->smp;
         smp_prev = srip->smp;
      } else
         smp_head = smp_prev = srip->smp;
      saparray = (SeqAlignPtr PNTR)MemNew((srip->smp->numexons)*sizeof(SeqAlignPtr));
      for (i=0; i<srip->smp->numexons; i++)
      {
         saparray[i] = SeqAlignDup(srip->smp->saps[i]);
         AlnMgr2IndexSingleChildSeqAlign(saparray[i]);
      }
      srip->smp->continuous = SPI_CreateContinuousAln(srip->smp->saps, srip->smp->numexons);
      for (i=0; i<srip->smp->numexons; i++)
      {
         SeqAlignFree(srip->smp->saps[i]);
         srip->smp->saps[i] = saparray[i];
         if (i < srip->smp->numexons-1)
         {
            srip->smp->saps[i]->next = saparray[i+1];
            saparray[i+1]->next = NULL;
         }
      }
      MemFree(saparray);
      parent = SeqAlignNew();
      parent->segtype = SAS_DISC;
      parent->segs = (Pointer)(srip->smp->saps[0]);
      AlnMgr2IndexLite(parent);
      srip->smp->parent = parent;
      srip->polyAtail = srip->smp->polyAtail;
      srip->smp->fallsoff = srip->fallsoff;
      if (srip->smp->protein == NULL)
         srip->smp->protein = SPI_GetProteinFrommRNA(spim->bsp, &srip->smp->transstart);
      srip->smp = NULL; /* so that smp doesn't get freed */
      srip = srip->next;
   }
   SPI_bsinfoFreeList(spig);
   SPI_bsinfoFreeList(spim);
   SPI_RegionListFree(srip_head);
   if (lcl)
      SPI_OptionsFree(spot_lcl);
   return (smp_head);
}

/***************************************************************************
*
*  SPI_AlnSinglemRNAToGen is called by Main() as well as by
*  SPI_AlignmRNAToGenomic. It does the initial BLAST
*  (high stringency) and makes sure that all the alignments
*  are on the plus strand of the genomic sequence. It then calls the
*  functions to create the windows, align in the windows, and sort the
*  final alignments. Finally, it calls the functions to print the 
*  information for each alignment, fetch the CDS and create its 
*  alignment if requested, and create a continuous alignment if the
*  user wishes to print an asn.1 seqalign. When everything is done, it
*  frees the SPI_RegionInfoPtrs if the program is run as the
*  standalone version, or returns them to the calling function.
*
***************************************************************************/
NLM_EXTERN SPI_RegionInfoPtr SPI_AlnSinglemRNAToGen(SPI_bsinfoPtr spig, SPI_bsinfoPtr spim, FILE *ofp, FILE *ofp2, SPI_OptionsPtr spot)
{
   Char                 rep_buf[1024] = "m L;R";
   Int4                 i;
   BLAST_OptionsBlkPtr  options;
   SPI_Progress         progress;
   SeqAlignPtr          salp;
   SeqAlignPtr          salp_tmp;
   SeqAlignPtr          sap;
   SeqAlignPtr          sap_tmp;
   SeqAlignPtr          sap_tmp2;
   SeqLocPtr            slp1;
   SeqLocPtr            slp2;
   SPI_RegionInfoPtr    srip;
   SPI_RegionInfoPtr    srip_cds;
   SPI_RegionInfoPtr    srip_tmp;
   Boolean              standalone;
   Uint1                strand;

   if (spig == NULL || spim == NULL)
      return NULL;
   /*sanity checks for to & from*/
   if (spot->to == 0){
       spot->to = spig->bsp->length - 1;
   }
   else if (spot->to < spot->from){
       Int4 new_from = spot->to;
       spot->to = spot->from;
       spot->from = new_from;
   }
   if (spot->from == spot->to){
       return NULL;
   }
   /***
   if (spot->to < spot->from)
      return NULL;
   ***/
   if (ofp == NULL)
      standalone = FALSE;
   else
      standalone = TRUE;
   spot->printheader = TRUE;
   options = BLASTOptionNew("blastn", FALSE);
   
   /* KSK added to allow user defined repeat db path */
   /* options->filter_string = StringSave("m L;R"); */
   if (spot->repeat_db_file){
        strcat(rep_buf, " -d ");
        strcat(rep_buf, spot->repeat_db_file);
   }
   options->filter_string = StringSave(rep_buf);
   /*end of adding repeat db path */
   options->expect_value = spot->firstpasseval;
   options->query_lcase_mask = spot->lcaseloc;
   if (spot->interspecies)
   {
      options->gap_x_dropoff_final = 100;
      options->gap_open = 5;
      options->gap_extend = 1;
      options->penalty = -1;
   }
   /* do the BLAST with the mRNA as the query, for speed */
   if (spot->callback != NULL)
   {
      progress.percentdone = 5;
      progress.returncode = SPI_START;
      if (!spot->callback(&progress))
         return NULL;
   }
   /*** not checking for to/from, so it's gotta be just as easy
   to send it directly to ..ByLoc()***
   if (spot->strand == Seq_strand_both)
    sap = BlastTwoSequences(spim->bsp, spig->bsp, "blastn", options);
   else
   {
   ***/
   slp1 = SeqLocIntNew(0, spim->bsp->length-1, Seq_strand_plus, spim->bsp->id);
   slp2 = SeqLocIntNew(spot->from, spot->to, spot->strand, spig->bsp->id);
   /* slp2 = SeqLocIntNew(0, spig->bsp->length-1, spot->strand, spig->bsp->id); */
   sap = BlastTwoSequencesByLoc(slp1, slp2, "blastn", options);
   /* } */
    
   if (spot->callback != NULL)
   {
      progress.percentdone = 30;
      progress.returncode = SPI_PROGRESS;
      if (!spot->callback(&progress))
         return NULL;
   }
   if (sap == NULL)
   {
      if (standalone)
         SPI_PrintResult(ofp, ofp2, NULL, spig->bsp, spim->bsp, spot, FALSE);
      BLASTOptionDelete(options);
      return NULL;
   }
   if (!AlnMgr2IndexLite(sap))
   {
      if (standalone)
         SPI_PrintResult(ofp, ofp2, NULL, spig->bsp, spim->bsp, spot, FALSE);
      ErrPostEx(SEV_ERROR, 0, 0, "Alignment indexing error\n");
      SeqAlignSetFree(sap);
      BLASTOptionDelete(options);
      return NULL;
   }
   salp = (SeqAlignPtr)(sap->segs);
   /* since the mRNA was the query, need to flip all the alignments */
   /* so that the mRNA will end up as the second row                */
   SPI_flip_sa_list(salp);
   /* now make sure that everything is on the plus strand of the genomic sequence */
   while (salp != NULL)
   {
      strand = AlnMgr2GetNthStrand(salp, 1);
      if (strand == Seq_strand_minus)
      {
         salp_tmp = salp->next;
         salp->next = NULL;
         SAIndex2Free2(salp->saip);
         salp->saip = NULL;
         salp = SeqAlignListReverseStrand(salp);
         AlnMgr2IndexSingleChildSeqAlign(salp);
         salp->next = salp_tmp;
      }
      salp = salp->next;
   }
   SPI_RemoveOutsideBounds(sap, spot);
   BLASTOptionDelete(options);
   srip = SPI_FindWindows(sap, spot);
   /*SPI_TossRegions(&srip, spot);*/
   SPI_PadRegions(srip, spig->bsp->length);
   /* once the windows are found, throw out the original alignment */
   /* and carefully align in each window                           */
   SeqAlignSetFree(sap);
   SPI_AlignInWindows(&srip, spig->bsp, spim->bsp, spot);
   if (spot->callback != NULL)
   {
      progress.percentdone = 95;
      progress.returncode = SPI_PROGRESS;
      if (!spot->callback(&progress))
         return NULL;
   }
   SPI_SortRegionsByScore(&srip, spot);
   SPI_BeautifySMP(srip);
   SPI_CheckSplicesForRevComp(srip, spot, spig->bsp, spim->bsp);
   srip_cds = NULL;
   /* if the CDS alignment is desired, fetch the CDS information and */
   /* compute the CDS alignment by truncating the mRNA alignments    */
   if (standalone)
   {
      srip_tmp = srip;
      for (i=0; i<spot->numreturns; i++)
      {
         spot->printheader = FALSE;
         if (spot->fetchcds)
         {
            srip_cds = SPI_GetResultsForCDS(srip_tmp, spim->bsp, spot);
            SPI_BeautifySMP(srip_cds);
         }
         if (srip_cds != NULL && spot->ace == FALSE)
         {
            spot->printheader = TRUE;
            SPI_PrintResult(ofp, ofp2, srip_cds, spig->bsp, spim->bsp, spot, TRUE);
            SPI_RegionListFree(srip_cds);
         } else if (srip_cds != NULL && spot->ace == TRUE)
            SPI_PrintAce(ofp, srip_cds, spig->bsp, spim->bsp, TRUE);
         if (spot->printheader) /* print the version info only once per mRNA/CDS */
            spot->printheader = FALSE;
         else
            spot->printheader = TRUE;
         /** KSK bug fix - access of null srip_tmp->smp that
          results from SPI_SortRegionsByScore() removing
         all below -c threshold **/
         if (srip_tmp != NULL && srip_tmp->smp != NULL && srip_tmp->smp->protein == NULL)
             srip_tmp->smp->protein = SPI_GetProteinFrommRNA(spim->bsp, &srip_tmp->smp->transstart);
         if (spot->ace == FALSE)
         {
            if (srip_tmp == NULL)
               SPI_PrintResult(ofp, ofp2, srip_tmp, spig->bsp, spim->bsp, spot, FALSE);
            else
            {
               SPI_PrintResult(ofp, ofp2, srip_tmp, spig->bsp, spim->bsp, spot, FALSE);
               srip_tmp = srip_tmp->next;
            }
         } else
            SPI_PrintAce(ofp, srip_tmp, spig->bsp, spim->bsp, FALSE);
      }
      if (spot->printasn && srip != NULL && srip->smp != NULL)
      {
         sap_tmp = SPI_CreateContinuousAln(srip->smp->saps, srip->smp->numexons);
         if (*(spot->sap_head) == NULL)
            *(spot->sap_head) = sap_tmp;
         else
         {
            sap_tmp2 = *(spot->sap_head);
            while (sap_tmp2->next != NULL)
            {
               sap_tmp2 = sap_tmp2->next;
            }
            sap_tmp2->next = sap_tmp;
         }
      }
      if (spot->makemult == FALSE)
         SPI_RegionListFree(srip);
      else
         return srip;
   } else
   {
      if (srip_cds != NULL)
      {
         srip_cds->next = srip;
         srip = srip_cds;
      }
      return srip;
   }
   spot->printheader = FALSE;
   return NULL;
}

/***************************************************************************
*
*  SPI_CompareAlnPosForMult is the callback for the HeapSort in
*  SPI_MakeMultipleAlignment. It simply puts the alignments in order
*  along the genomic sequence, from least to greatest if the alignments
*  are on the plus strand, greatest to least otherwise.
*
***************************************************************************/
static int LIBCALLBACK SPI_CompareAlnPosForMult(VoidPtr ptr1, VoidPtr ptr2)
{
   Int4         from1;
   Int4         from2;
   SeqAlignPtr  sap1;
   SeqAlignPtr  sap2;
   Uint1        strand;
   Int4         to1;
   Int4         to2;

   sap1 = *((SeqAlignPtr PNTR)ptr1);
   sap2 = *((SeqAlignPtr PNTR)ptr2);
   strand = AlnMgr2GetNthStrand(sap1, 1);
   AlnMgr2GetNthSeqRangeInSA(sap1, 1, &from1, &to1);
   AlnMgr2GetNthSeqRangeInSA(sap2, 1, &from2, &to2);
   if (strand == Seq_strand_minus)
   {
      if (from1 < from2)
         return 1;
      if (from2 < from1)
         return -1;
      if (to1 < to2)
         return 1;
      if (to2 < to1)
         return -1;
   } else
   {
      if (from1 < from2)
         return -1;
      if (from2 < from1)
         return 1;
      if (to1 < to2)
         return -1;
      if (to2 < to1)
         return 1;
   }
   return 0;
}

/***************************************************************************
*
*  SPI_OrderBlocksPlus is a callback for SPI_MakeMultipleAlignment. It
*  is used to order the blocks along the genomic sequence when the genomic
*  strand is plus.
*
***************************************************************************/
static int LIBCALLBACK SPI_OrderBlocksPlus(VoidPtr ptr1, VoidPtr ptr2)
{
   SPI_BlockPtr  sbp1;
   SPI_BlockPtr  sbp2;

   sbp1 = *((SPI_BlockPtr PNTR)ptr1);
   sbp2 = *((SPI_BlockPtr PNTR)ptr2);
   if (sbp1->from_g < sbp2->from_g)
      return -1;
   if (sbp2->from_g < sbp1->from_g)
      return 1;
   if (sbp1->to_g < sbp2->to_g)
      return -1;
   if (sbp1->to_g > sbp2->to_g)
      return 1;
   return 0;
}

/***************************************************************************
*
*  SPI_OrderBlocksMinus is a callback for SPI_MakeMultipleAlignment. It
*  is used to order the blocks along the genomic sequence when the genomic
*  strand is minus.
*
***************************************************************************/
static int LIBCALLBACK SPI_OrderBlocksMinus(VoidPtr ptr1, VoidPtr ptr2)
{
   SPI_BlockPtr  sbp1;
   SPI_BlockPtr  sbp2;

   sbp1 = *((SPI_BlockPtr PNTR)ptr1);
   sbp2 = *((SPI_BlockPtr PNTR)ptr2);
   if (sbp1->from_g < sbp2->from_g)
      return 1;
   if (sbp2->from_g < sbp1->from_g)
      return -1;
   if (sbp1->to_g < sbp2->to_g)
      return 1;
   if (sbp1->to_g > sbp2->to_g)
      return -1;
   return 0;
}

/***************************************************************************
*
*  SPI_CompareSecondRow is the HeapSort callback for SPI_RearrangeAlns;
*  it orders two seqaligns in lexical order of the seqid of their second
*  row. (All seqaligns involved have only two rows anyway).
*
***************************************************************************/
static int LIBCALLBACK SPI_CompareSecondRow(VoidPtr ptr1, VoidPtr ptr2)
{
   Int4         ret;
   SeqAlignPtr  sap1;
   SeqAlignPtr  sap2;
   SeqIdPtr     sip1;
   SeqIdPtr     sip2;

   sap1 = *((SeqAlignPtr PNTR)ptr1);
   sap2 = *((SeqAlignPtr PNTR)ptr2);
   sip1 = AlnMgr2GetNthSeqIdPtr(sap1, 2);
   sip2 = AlnMgr2GetNthSeqIdPtr(sap2, 2);
   ret = SAM_OrderSeqID(sip1, sip2);
   SeqIdFree(sip1);
   SeqIdFree(sip2);
   return ret;
}

/***************************************************************************
*
*  SPI_RearrangeAlns arranges the alignments in a linked list in lexical
*  order of their second seqid, so that all blocks will have their
*  rows in the same order.
*
***************************************************************************/
static SeqAlignPtr SPI_RearrangeAlns(SeqAlignPtr sap_head)
{
   Int4         i;
   Int4         j;
   SeqAlignPtr  sap;
   SeqAlignPtr  PNTR saparray;

   i = 0;
   sap = sap_head;
   while (sap != NULL)
   {
      i++;
      sap = sap->next;
   }
   if (i == 1)
      return sap_head;
   saparray = (SeqAlignPtr PNTR)MemNew(i*sizeof(SeqAlignPtr));
   sap = sap_head;
   i = 0;
   while (sap != NULL)
   {
      saparray[i] = sap;
      i++;
      sap = sap->next;
   }
   HeapSort(saparray, i, sizeof(SeqAlignPtr), SPI_CompareSecondRow);
   for (j=0; j<i-1; j++)
   {
      saparray[j]->next = saparray[j+1];
   }
   saparray[i-1]->next = NULL;
   sap = saparray[0];
   MemFree(saparray);
   return sap;
}

/***************************************************************************
*
*  SPI_MakeMultipleAlignment takes all exons returned from all mRNAs, all
*  regions, and makes multiple alignments out of them (after grouping them
*  into blocks. All blocks will not necessarily contain all mRNAs.
*
***************************************************************************/
NLM_EXTERN void SPI_MakeMultipleAlignment(SPI_RegionInfoPtr srip_head)
{
   SPI_BlockPtr       PNTR blockarray;
   Int4               i;
   Int4               j;
   Int4               minus;
   Int4               numblocks;
   Int4               numsmps;
   Int4               plus;
   SeqAlignPtr        sap;
   SeqAlignPtr        sap_head;
   SeqAlignPtr        sap_prev;
   SeqAlignPtr        sap_tmp;
   SeqAlignPtr        PNTR saparray;
   SPI_BlockPtr       sbp;
   SPI_BlockPtr       sbp_head;
   SPI_BlockPtr       sbp_prev;
   SPI_MultPtr        smu;
   SPI_RegionInfoPtr  srip;
   Uint1              strand;
   Uint1              strand_tmp;
   SeqAlignPtr        sub_sap;

   if (srip_head->next == NULL)  /* only one alignment here */
      return;
   i = 0;
   sap_head = sap_prev = NULL;
   numsmps = 0;
   minus = plus = 0;
   srip = srip_head;
   while (srip != NULL)
   {
      if (srip->smp != NULL)
      {
         if (srip->smp->strand == Seq_strand_minus)
            minus++;
         else
            plus++;
         for (j=0; j<srip->smp->numexons; j++)
         {
            sap = SeqAlignDup(srip->smp->saps[j]);
            AlnMgr2IndexSingleChildSeqAlign(sap);
            if (sap_head != NULL)
            {
               sap_prev->next = sap;
               sap_prev = sap;
            } else
               sap_head = sap_prev = sap;
            i++;
         }
         numsmps++;
      }
      srip = srip->next;
   }
   if (numsmps <= 1)
   {
      SeqAlignSetFree(sap_head);
      return;
   }
   if (minus > plus)
      strand = Seq_strand_minus;
   else
      strand = Seq_strand_plus;
   saparray = (SeqAlignPtr PNTR)MemNew(i*sizeof(SeqAlignPtr));
   sap = sap_head;
   for (j=0; j<i; j++)
   {
      strand_tmp = AlnMgr2GetNthStrand(sap, 1);
      if ((strand_tmp == Seq_strand_minus && strand != Seq_strand_minus) || (strand_tmp != Seq_strand_minus && strand == Seq_strand_minus))
      {
         sap_tmp = sap->next;
         sap->next = NULL;
         SeqAlignListReverseStrand(sap);
         sap->next = sap_tmp;
      }
      saparray[j] = sap;
      sap = sap->next;
   }
   HeapSort(saparray, i, sizeof(SeqAlignPtr), SPI_CompareAlnPosForMult);
   for (j=0; j<i; j++)
   {
      saparray[j]->next = NULL;
   }
   sbp_head = sbp_prev = NULL;
   for (j=0; j<i; j++)
   {
      if (sbp_head == NULL)
      {
         sbp = (SPI_BlockPtr)MemNew(sizeof(SPI_Block));
         AlnMgr2GetNthSeqRangeInSA(saparray[j], 1, &sbp->from_g, &sbp->to_g);
         sbp->sap = saparray[j];
         saparray[j] = NULL;
         sbp_head = sbp_prev = sbp;
      } else
      {
         if (spi_overlaps(saparray[j], sbp))
         {
            sap_tmp = sbp->sap;
            while (sap_tmp->next != NULL)
            {
               sap_tmp = sap_tmp->next;
            }
            sap_tmp->next = saparray[j];
            saparray[j] = NULL;
         } else
         {
            sbp = (SPI_BlockPtr)MemNew(sizeof(SPI_Block));
            AlnMgr2GetNthSeqRangeInSA(saparray[j], 1, &sbp->from_g, &sbp->to_g);
            sbp->sap = saparray[j];
            saparray[j] = NULL;
            sbp_prev->next = sbp;
            sbp_prev = sbp;
         }
      }
   }
   MemFree(saparray);
   sbp = sbp_head;
   numblocks = 0;
   while (sbp)
   {
      numblocks++;
      sbp->sap = SPI_RearrangeAlns(sbp->sap);
      AlnMgr2IndexIndexedChain(sbp->sap);
      sub_sap = AlnMgr2GetSubAlign(sbp->sap, 0, -1, 0, TRUE);
      SeqAlignSetFree(sbp->sap);
      sbp->sap = sub_sap;
      if (strand == Seq_strand_minus)
         sbp->sap = SeqAlignListReverseStrand(sbp->sap);
      AlnMgr2IndexSingleChildSeqAlign(sub_sap);
      sbp = sbp->next;
   }
   blockarray = (SPI_BlockPtr PNTR)MemNew(numblocks*sizeof(SPI_BlockPtr));
   sbp = sbp_head;
   j = 0;
   while (sbp != NULL)
   {
      blockarray[j] = sbp;
      j++;
      sbp = sbp->next;
   }
   if (strand == Seq_strand_minus)
      HeapSort(blockarray, numblocks, sizeof(SPI_BlockPtr), SPI_OrderBlocksMinus);
   else
      HeapSort(blockarray, numblocks, sizeof(SPI_BlockPtr), SPI_OrderBlocksPlus);
   saparray = (SeqAlignPtr PNTR)MemNew(numblocks*sizeof(SeqAlignPtr));
   for (j=0; j<numblocks; j++)
   {
      saparray[j] = blockarray[j]->sap;
      MemFree(blockarray[j]);
   }
   MemFree(blockarray);
   smu = (SPI_MultPtr)MemNew(sizeof(SPI_Mult));
   smu->exons = saparray;
   smu->numexons = numblocks;
   srip_head->smu = smu;
}

/***************************************************************************
*
*  SPI_WriteAlnLine prints out the specified row of an alignment, between
*  the alignment coordinates specified. It allocates the charptr itself;
*  this must be freed later by the calling function.
*
***************************************************************************/
static CharPtr SPI_WriteAlnLine(Int4 row, Int4 from, Int4 to, SeqAlignPtr sap)
{
   AlnMsg2Ptr   amp;
   BioseqPtr   bsp;
   Uint1       buf[SPI_LINE+2];
   Int4        ctr;
   Int4        i;
   Boolean     more;
   Int4        n;
   SeqIdPtr    sip;
   SeqPortPtr  spp;
   CharPtr     string;

   n = AlnMgr2GetNumRows(sap);
   if (row > n || row < 1)
      return NULL;
   string = (CharPtr)MemNew((SPI_LINE+2)*sizeof(Char));
   for (n=0; n<(SPI_LINE+2); n++)
   {
      string[n] = '\0';
   }
   sip = AlnMgr2GetNthSeqIdPtr(sap, row);
   bsp = BioseqLockById(sip);
   amp = AlnMsgNew2();
   amp->row_num = row;
   amp->from_aln = from;
   amp->to_aln = to;
   if (amp->to_aln < 0)
      amp->to_aln = -1;
   n = 0;
   while ((more = AlnMgr2GetNextAlnBit(sap, amp)) == TRUE)
   {
      if (amp->to_row - amp->from_row > amp->to_aln - amp->from_aln) /* kludge */
      {
         if (amp->strand == Seq_strand_minus)
            amp->from_row = amp->to_row - (amp->to_aln - amp->from_aln);
         else
            amp->to_row = amp->from_row + (amp->to_aln - amp->from_aln);
      }
      if (amp->type == AM_SEQ)
      {
         spp = SeqPortNew(bsp, amp->from_row, amp->to_row, amp->strand, Seq_code_iupacna);
         ctr = SeqPortRead(spp, buf, (amp->to_row - amp->from_row + 1));
         SeqPortFree(spp);
         for (i=n; i<n+ctr; i++)
         {
            string[i] = buf[i-n];
         }
         n += ctr;
      } else
      {
         for (i=n; i<(n+amp->to_row-amp->from_row+1); i++)
         {
            string[i] = '-';
         }
         n += amp->to_row-amp->from_row+1;
      }
   }
   AlnMsgFree2(amp);
   SeqIdFree(sip);
   return string;
}

/***************************************************************************
*
*  SPI_MapRowCoords finds the first non-gap character in a row and
*  returns its sequence position. If the row consists only of gaps, it
*  returns -1. If direction is RIGHT, the function searches upwards in
*  alignment coordinates; otherwise it searches the other direction.
*
***************************************************************************/
static Int4 SPI_MapRowCoords(SeqAlignPtr sap, Int4 from, Int4 to, Int4 row, Uint1 direction)
{
   Int4  pos;

   if (direction == SPI_RIGHT)
   {
      pos = AlnMgr2MapSeqAlignToBioseq(sap, from, row);
      from++;
      while (pos < 0 && from <= to)
      {
         pos = AlnMgr2MapSeqAlignToBioseq(sap, from, row);
         from++;
      }
   } else
   {
      pos = AlnMgr2MapSeqAlignToBioseq(sap, to, row);
      to--;
      while (pos < 0 && to >= from)
      {
         pos = AlnMgr2MapSeqAlignToBioseq(sap, to, row);
         to--;
      }
   }
   if (pos < 0)
      return -1;
   return pos;
}

/***************************************************************************
*
*  spi_get_num_places calculates the number of digits in a number, for
*  display neatness purposes.
*
***************************************************************************/
static Int4 spi_get_num_places(Int4 num)
{
   FloatHi  f;
   Int4     i;
   Int4     x;

   x = 10;
   for (i=1; i<21; i++)
   {
      f = (FloatHi)num/(FloatHi)x;
      if (f < 1)
      {
         if (num < 0)
            return (i+1);
         else
            return i;
      }
      x = x*10;
   }
   if (num < 0)
      i++;
   return i;
}

/***************************************************************************
*
*  SPI_IsItMult looks over the alignments in smu->exons to see whether
*  there are any aligments with dim > 2; if so, it returns SPI_MULT, and
*  if not, it returns SPI_NOTMULT.
*
***************************************************************************/
static Int4 SPI_IsItMult(SPI_MultPtr smu)
{
   Int4  i;

   if (smu == NULL)
      return SPI_NOTMULT;
   for (i=0; i<smu->numexons; i++)
   {
      if (AlnMgr2GetNumRows(smu->exons[i]) > 2)
         return SPI_MULT;
   }
   return SPI_NOTMULT;
}

/***************************************************************************
*
*  SPI_PrintMultipleAlignment prints a text or html report of the alignment
*  computed by SPI_MakeMultipleAlignment.
*
***************************************************************************/
NLM_EXTERN void SPI_PrintMultipleAlignment(SPI_RegionInfoPtr srip, Boolean html, BioseqPtr bsp, FILE * ofp)
{
   Char         accsite[SPI_PSPLICE+2];
   Int4         c;
   Int4Ptr      coord;
   Int4         ctr;
   Int4         d;
   Char         don[SPI_PSPLICE+2];
   Int4         from;
   Int4         i;
   Int4         j;
   Int4         len;
   Boolean      local;
   Int4         n;
   Int4         ret;
   SeqAlignPtr  sap;
   SeqIdPtr     sip;
   SPI_MultPtr  smu;
   Int4         spacer;
   SeqPortPtr   spp;
   Uint1        strand;
   CharPtr      PNTR stringptr;
   Char         textid[42];
   Int4         to;

   if (srip == NULL || srip->smu == NULL)
      return;
   smu = srip->smu;
   if (ofp == NULL)
   {
      local = TRUE;
      ofp = FileOpen("stdout", "w");
   } else
      local = FALSE;
   fprintf(ofp, "\n\n");
   if (html)
      fprintf(ofp, "<h1><center>");
   fprintf(ofp, "Multiple Alignments\n");
   ret = SPI_IsItMult(smu);
   if (ret == SPI_NOTMULT)
   {
      fprintf(ofp, "None of the alignments in the set appears to be a multiple alignment.\n");
      return;
   }
   if (html)
   {
      fprintf(ofp, "<br></center></h1>\n");
      fprintf(ofp, "<table cellspacing=\"8\" cellpadding=\"5\" border=\"0\" width=\"600\">\n");
   }
   spacer = SPI_SPACER;
   for (i=0; i<smu->numexons; i++)
   {
      sap = smu->exons[i];
      n = AlnMgr2GetNumRows(sap);
      if (html)
      {
         fprintf(ofp, "<tr><td bgcolor=%s width=\"600\">", (i%2)?"#FFFFFF":"#FFFFCC");
         fprintf(ofp, "<a name=Block%d></a><h4>Block %d\n</h4><pre>", i+1, i+1);
      } else
         fprintf(ofp, "Block %d\n", i+1);
      for (j=0; j<n; j++)
      {
         if (j > 0)
            sip = AlnMgr2GetNthSeqIdPtr(sap, j+1);
         else
            sip = bsp->id;
         SeqIdWrite(sip, textid, PRINTID_FASTA_LONG, 41);
         AlnMgr2GetNthSeqRangeInSA(sap, j+1, &from, &to);
         strand = AlnMgr2GetNthStrand(sap, j+1);
         if (html)
            fprintf(ofp, "<font color=%s>%s: %d to %d</font> %s\n", (j==0)?"#336699":"#800080", textid, from+1, to+1, (strand == Seq_strand_minus)?"<font color=#FF0033>minus strand</font>":"");
         else
            fprintf(ofp, "%s: %d to %d %s\n", textid, from+1, to+1, (strand == Seq_strand_minus)?"minus strand":"");
         if (j > 0)
            SeqIdFree(sip);
      }
      fprintf(ofp, "\n");
      len = AlnMgr2GetAlnLength(sap, FALSE);
      /* get donor and acceptor sites */
      strand = AlnMgr2GetNthStrand(sap, 1);
      AlnMgr2GetNthSeqRangeInSA(sap, 1, &from, &to);
      if (strand == Seq_strand_minus)
      {
         spp = SeqPortNew(bsp, to+1, MIN(bsp->length-1, to+SPI_PSPLICE), Seq_strand_minus, Seq_code_iupacna);
         ctr = MIN(bsp->length-1, to+SPI_PSPLICE)-(to+1)+1;
      } else
      {
         spp = SeqPortNew(bsp, MAX(0, from-SPI_PSPLICE), from-1, Seq_strand_plus, Seq_code_iupacna);
         ctr = from-1-MAX(0, from-SPI_PSPLICE)+1;
      }
      ctr = SeqPortRead(spp, (Uint1Ptr)accsite, ctr);
      accsite[ctr] = '\0';
      SeqPortFree(spp);
      if (strand == Seq_strand_minus)
      {
         spp = SeqPortNew(bsp, MAX(0, from-SPI_PSPLICE), from-1, Seq_strand_minus, Seq_code_iupacna);
         ctr = from-1-MAX(0, from-SPI_PSPLICE)+1;
      } else
      {
         spp = SeqPortNew(bsp, to+1, MIN(to+SPI_PSPLICE, bsp->length-1), Seq_strand_plus, Seq_code_iupacna);
         ctr = MIN(to+SPI_PSPLICE, bsp->length-1)-(to+1)+1;
      }
      ctr = SeqPortRead(spp, (Uint1Ptr)don, ctr);
      don[ctr] = '\0';
      SeqPortFree(spp);
      StringLower(accsite);
      StringLower(don);
      fprintf(ofp, "%s<-flank\n", accsite);
      stringptr = (CharPtr PNTR)MemNew(n*sizeof(CharPtr));
      coord = (Int4Ptr)MemNew(n*sizeof(Int4));
      for (c=0; c<len; c+=SPI_LINE-10)
      {
         for (j=0; j<n; j++)
         {
            stringptr[j] = SPI_WriteAlnLine(j+1, c, MIN(c+SPI_LINE-10-1, len-1), sap);
            coord[j] = SPI_MapRowCoords(sap, c, MIN(c+SPI_LINE-10-1, len-1), j+1, SPI_RIGHT);
            if (coord[j] >= 0)
               coord[j]++;
         }
         for (j=0; j<n; j++)
         {
            if (html)
               fprintf(ofp, "<font color=%s>", (j==0)?"#336699":"#800080");
            fprintf(ofp, "%d", coord[j]);
            if (html)
               fprintf(ofp, "</font>");
            /* KSK */
            /* d = spi_get_num_places(coord[j]); */
            for ( d = spi_get_num_places(coord[j]); 
                  d < spacer; d++) 
            {
               fprintf(ofp, " ");
            }
            if (j == 0)
               fprintf(ofp, "%s", stringptr[j]);
            else
            {
               for (ctr=0; ctr<MIN(SPI_LINE-10, len-c); ctr++)
               {
                  if (stringptr[j][ctr] == stringptr[0][ctr])
                     fprintf(ofp, ".");
                  else
                  {
                     if (html && stringptr[0][ctr] != '-' && stringptr[j][ctr] != '-')
                        fprintf(ofp, "<font color=#FF0033>");
                     fprintf(ofp, "%c", stringptr[j][ctr]);
                     if (html && stringptr[0][ctr] != '-' && stringptr[j][ctr] != '-')
                        fprintf(ofp, "</font>");
                  }
               }
            }
            fprintf(ofp, "\n");
            if (j > 0)
               MemFree(stringptr[j]);
         }
         MemFree(stringptr[0]);
         if (c+SPI_LINE-10 < len)
            fprintf(ofp, "\n");
      }
      for (j=0; j<ctr+spacer-7; j++)
      {
         fprintf(ofp, " ");
      }
      fprintf(ofp, "flank->%s\n\n", don);
      if (html)
         fprintf(ofp, "<a href=#TOP>Top</a>\n");
      MemFree(stringptr);
      MemFree(coord);
      if (html)
         fprintf(ofp, "</pre></td></tr>\n");
   }
   if (html)
      fprintf(ofp, "</table>");
   if (local)
      FileClose(ofp);
}

/***************************************************************************
*
*  spi_overlaps decides whether a new seqalign overlaps the already
*  established range of a block. If it does, the coordinates are checked
*  to see if it extends that range; if so, the block range is widened.
*  If there is no overlap, the function returns FALSE.
*
***************************************************************************/
static Boolean spi_overlaps(SeqAlignPtr sap, SPI_BlockPtr sbp)
{
   Int4  from;
   Int4  to;

   AlnMgr2GetNthSeqRangeInSA(sap, 1, &from, &to);
   if ((from <= sbp->from_g && to >= sbp->from_g) || (from <= sbp->to_g && to >= sbp->to_g))
   {
      if (from < sbp->from_g)
         sbp->from_g = from;
      if (to > sbp->to_g)
         sbp->to_g = to;
      return TRUE;
   }
   return FALSE;
}

/***************************************************************************
*
*  SPI_BeautifySMP converts all coordinates to 1-based from 0-based, and
*  runs through the mRNA to see whether any mRNA is missing; if so, it
*  flags that alignment with holes=TRUE.
*
***************************************************************************/
static void SPI_BeautifySMP(SPI_RegionInfoPtr srip)
{
   BioseqPtr    bsp;
   Int4         i;
   SeqIdPtr     sip;
   SPI_mRNAPtr  smp;

   /** KSK bug fix for when smp is null because
       SPI_SortRegionsByScore() removed everyone below
       -c threshold ***/
   while (srip != NULL && srip->smp != NULL)
   {
      smp = srip->smp;
      if (srip->polyAtail == 0 && smp->numexons > 1)
      {
         sip = AlnMgr2GetNthSeqIdPtr(smp->saps[0], 2);
         bsp = BioseqLockById(sip);
         srip->polyAtail = SPI_IsItPolyA(sip);
         BioseqUnlock(bsp);
         SeqIdFree(sip);
      }
      smp->holes = FALSE;
      if (smp->strand == Seq_strand_minus)
      {
         for (i=smp->numexons-2; i>0 && !smp->holes; i--)
         {
            if (smp->mstarts[i] != smp->mstops[i+1]+1)
               smp->holes = TRUE;
         }
      } else
      {
         for (i=1; i<smp->numexons && !smp->holes; i++)
         {
            if (smp->mstarts[i] != smp->mstops[i-1]+1)
               smp->holes = TRUE;
         }
      }
      for (i=0; i<smp->numexons; i++)
      {
         smp->mstarts[i]++;
         smp->mstops[i]++;
         smp->gstarts[i]++;
         smp->gstops[i]++;
      }
      srip = srip->next;
   }
}

/***************************************************************************
*
*  SPI_RemoveOutsideBounds removes alignments that fall outside the
*  spot->from and spot->to bounds, so that regions won't be created
*  outside these boundaries.
*
***************************************************************************/
static void SPI_RemoveOutsideBounds(SeqAlignPtr sap, SPI_OptionsPtr spot)
{
   SeqAlignPtr  salp;
   SeqAlignPtr  salp_head;
   SeqAlignPtr  salp_next;
   SeqAlignPtr  salp_prev;
   Int4         start;
   Int4         stop;

   if (sap == NULL || spot == NULL)
      return;
   salp = (SeqAlignPtr)(sap->segs);
   salp_head = salp_prev = NULL;
   while (salp != NULL)
   {
      salp_next = salp->next;
      salp->next = NULL;
      AlnMgr2GetNthSeqRangeInSA(salp, 1, &start, &stop);
      if (start >= spot->from || stop <= spot->to)
      {
         if (salp_head != NULL)
         {
            salp_prev->next = salp;
            salp_prev = salp;
         } else
            salp_head = salp_prev = salp;
      } else
         SeqAlignFree(salp);
      salp = salp_next;
   }
   sap->segs = (Pointer)(salp_head);
}

/***************************************************************************
*
*  SPI_PadRegions takes a linked list of regions and adds SPI_PADDING
*  to either end of each region.
*
***************************************************************************/
static void SPI_PadRegions(SPI_RegionInfoPtr srip, Int4 bsplen)
{
   while (srip != NULL)
   {
      srip->gstart = srip->gstart - SPI_PADDING;
      if (srip->gstart < 0)
         srip->gstart = 0;
      srip->gstop = srip->gstop + SPI_PADDING;
      if (srip->gstop > bsplen - 1)
         srip->gstop = bsplen - 1;
      srip = srip->next;
   }
}

/***************************************************************************
*
*  SPI_TossRegions sorts the regions returned by SPI_FindWindows, and 
*  removes all but the n best regions, where n is the number of regions
*  requested by the user. The regions are sorted by alignment score, then
*  by sequence coverage.
*
***************************************************************************/
static void SPI_TossRegions(SPI_RegionInfoPtr PNTR srip, SPI_OptionsPtr spot)
{
   Boolean            done;
   Int4               i;
   Int4               j;
   Int4               n;
   SPI_RegionInfoPtr  PNTR sriparray;
   SPI_RegionInfoPtr  srip_tmp;

   if (srip == NULL || spot == NULL || *srip == NULL)
      return;
   i = 0;
   srip_tmp = *srip;
   while (srip_tmp != NULL)
   {
      srip_tmp = srip_tmp->next;
      i++;
   }
   sriparray = (SPI_RegionInfoPtr PNTR)MemNew(i*sizeof(SPI_RegionInfoPtr));
   srip_tmp = *srip;
   for (j=0; j<i; j++)
   {
      sriparray[j] = srip_tmp;
      srip_tmp = srip_tmp->next;
   }
   HeapSort(sriparray, i, sizeof(SPI_RegionInfoPtr), SPI_CompareInitialRegions);
   if (spot->numreturns > 0)
      n = spot->numreturns;
   else
      n = 1;
   done = FALSE;
   sriparray[0]->next = NULL;
   for (j=1; j<n && j<i && !done; j++)
   {
      sriparray[j-1]->next = sriparray[j];
      if (sriparray[j] != NULL)
         sriparray[j]->next = NULL;
      else
         done = TRUE;
   }
   for (j=n; j<i; j++)
   {
      MemFree(sriparray[j]);
   }
   *srip = sriparray[0];
   MemFree(sriparray);
}

/***************************************************************************
*
*  SPI_CompareInitialRegions is the HeapSort callback for SPI_TossRegions.
*  It compares the regions first by score (this is the sum of the 
*  alignment scores), and second by sequence coverage.
*
***************************************************************************/
static int LIBCALLBACK SPI_CompareInitialRegions(VoidPtr ptr1, VoidPtr ptr2)
{
   SPI_RegionInfo  srip1;
   SPI_RegionInfo  srip2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      srip1 = **((SPI_RegionInfoPtr PNTR)ptr1);
      srip2 = **((SPI_RegionInfoPtr PNTR)ptr2);
      if (srip1.coverage > srip2.coverage)
         return -1;
      else if (srip1.coverage < srip2.coverage)
         return 1;
      else
      {
         if (srip1.score > srip2.score)
            return -1;
         else if (srip1.score < srip2.score)
            return 1;
      }
   }
   return 0;
}

/***************************************************************************
*
*  SPI_SortRegionsByScore is called after SPI_AlignInWindows to sort the
*  final regions from best to worst for printing. Since there is now a
*  complete mRNA alignment in each region, the regions can be more
*  thoroughly assessed, and the regions are sorted by mRNA coverage,
*  number of mismatches, and finally by genomic start position.
*
***************************************************************************/
static void SPI_SortRegionsByScore(SPI_RegionInfoPtr PNTR srip, SPI_OptionsPtr spot)
{
   Int4               i;
   Int4               j;
   SPI_RegionInfoPtr  PNTR srip_array;
   SPI_RegionInfoPtr  srip_head;
   SPI_RegionInfoPtr  srip_prev;
   SPI_RegionInfoPtr  srip_tmp;

   if (srip == NULL || *srip == NULL)
      return;
   srip_tmp = *srip;
   i = 0;
   while (srip_tmp != NULL)
   {
      i++;
      srip_tmp = srip_tmp->next;
   }
   srip_array = (SPI_RegionInfoPtr PNTR)MemNew(i*sizeof(SPI_RegionInfoPtr));
   srip_tmp = *srip;
   j = 0;
   while (srip_tmp != NULL && j < i)
   {
      srip_array[j] = srip_tmp;
      srip_tmp = srip_tmp->next;
      j++;
   }
   HeapSort(srip_array, i, sizeof(SPI_RegionInfoPtr), SPI_CompareRegions);
   for (j=0; j<i; j++)  /* remove the ones that don't score above the cutoffs */
   {
      srip_tmp = srip_array[j];
      if (srip_tmp->smp == NULL || (srip_tmp->smp->mRNAcoverage < spot->lencutoff && !srip_tmp->fallsoff)|| srip_tmp->smp->mismatch > 100-spot->idcutoff)
      {
         SPI_RegionFree(srip_tmp);
         srip_array[j] = NULL;
      }
   }
   srip_head = srip_prev = NULL;
   for (j=0; j<i; j++)
   {
      if (srip_array[j] != NULL)
      {
         if (srip_head != NULL)
         {
            srip_prev->next = srip_array[j];
            srip_array[j]->next = NULL;
            srip_prev = srip_array[j];
         } else
         {
            srip_head = srip_prev = srip_array[j];
            srip_head->next = NULL;
         }
      }
   }
   srip_tmp = srip_prev = srip_head;
   i = 1;
   /** KSK fix for when all are null **/
   if (srip_tmp != NULL && srip_tmp->next != NULL){
       srip_tmp = srip_tmp->next; /* know we need the first one at least */
       while (srip_tmp != NULL){
           if (i+1>spot->numreturns){ /* this guy is one too many */
               srip_prev->next = NULL;
               SPI_RegionListFree(srip_tmp);
               srip_tmp = NULL;
           } else {
               srip_prev = srip_tmp;
               srip_tmp = srip_tmp->next;
               i++;
           }
       }
       *srip = srip_head;
       MemFree(srip_array);
   }
   else {
       *srip = srip_head;
   }
}

/***************************************************************************
*
*  SPI_CompareRegions is the HeapSort callback for SPI_SortRegionsByScore.
*  It sorts the regions first by mRNA coverage, then by the number of
*  mismatches in the mRNA-to-genomic alignment, and finally by the
*  start position on the genomic sequence.
*
***************************************************************************/
static int LIBCALLBACK SPI_CompareRegions(VoidPtr ptr1, VoidPtr ptr2)
{
   SPI_RegionInfoPtr  srip1;
   SPI_RegionInfoPtr  srip2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      srip1 = *((SPI_RegionInfoPtr PNTR)ptr1);
      srip2 = *((SPI_RegionInfoPtr PNTR)ptr2);
      if (srip1->smp == NULL)
         return 1;
      if (srip2->smp == NULL)
         return -1;
      if (srip1->smp->mRNAcoverage > srip2->smp->mRNAcoverage)
         return -1;
      else if (srip1->smp->mRNAcoverage < srip2->smp->mRNAcoverage)
         return 1;
      else
      {
         if (srip1->smp->mismatch < srip2->smp->mismatch)
            return -1;
         else if (srip1->smp->mismatch > srip2->smp->mismatch)
            return 1;
         else
         {
            if (srip1->smp->gstarts[0] < srip2->smp->gstarts[0])
               return -1;
            else if (srip1->smp->gstarts[0] > srip2->smp->gstarts[0])
               return 1;
            else
               return 0;
         }
      }
   }
   return 0;
}

/***************************************************************************
*
*  SPI_PrintAce prints the spidey results in ACEDB format for compatibility
*  with Jean Thierry-Mieg's Acembly software.
*
***************************************************************************/
static void SPI_PrintAce(FILE *ofp, SPI_RegionInfoPtr srip, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna, Boolean is_cds)
{
   Int4         gi;
   Int4         i;
   CharPtr      id1;
   CharPtr      id2;
   Int4         n;
   SPI_mRNAPtr  smp;

   smp = srip->smp;
   SPI_GetAccessionFromSeqId(bsp_genomic->id, &gi, &id1);
   SPI_GetAccessionFromSeqId(bsp_mrna->id, &gi, &id2);
   fprintf(ofp, "Sequence %s\n", id1);
   fprintf(ofp, "Subsequence Sp_%s.%s", id2, (is_cds == TRUE)?"cds":"mrna");
   if (smp->strand == Seq_strand_minus)
      fprintf(ofp, "\t%d\t%d\n\n", smp->gstops[smp->numexons-1]+1, smp->gstarts[0] + 1);
   else
      fprintf(ofp, "\t%d\t%d\n\n", smp->gstarts[0]+1, smp->gstops[smp->numexons-1]+1);
   fprintf(ofp, "Sequence Sp_%s.%s\n", id2, (is_cds == TRUE)?"cds":"mrna");
   fprintf(ofp, "Method Spidey\n");
   n = 1;
   if (is_cds)
      fprintf(ofp, "CDS\n");
   if (smp->strand != Seq_strand_minus)
   {
      for (i=0; i<smp->numexons; i++)
      {
         fprintf(ofp, "Source_Exons\t%d\t%d\n", n, n + smp->gstops[i] - smp->gstarts[i]);
         n += smp->gstops[i] - smp->gstarts[i];
         if (i < smp->numexons-1)
            n += smp->gstarts[i+1] - smp->gstops[i];
      }
   } else
   {
      for (i = smp->numexons-1; i>=0; i--)
      {
         fprintf(ofp, "Source_Exons\t%d\t%d\n", n, n + smp->gstops[i] - smp->gstarts[i]);
         n += smp->gstops[i] - smp->gstarts[i];
         if (i > 0)
            n += smp->gstarts[i] - smp->gstops[i-1];
      }
   }
   fprintf(ofp, "DNA_Homol %s\n", id2);
   if (smp->missingends == SPI_LEFT)
      fprintf(ofp, "Start_not_found\n");
   else if (smp->missingends == SPI_RIGHT)
      fprintf(ofp, "Stop_not_found\n");
   else if (smp->missingends == SPI_BOTH)
      fprintf(ofp, "Start_not_found\nStop_not_found\n");
   fprintf(ofp, "\n\n");
}

/***************************************************************************
*
*  SPI_PrintResult prints the summary report and (if requested) the
*  text alignment. Since the exons are stored in the order of the
*  genomic sequence, not the mRNA, they must be reversed to print the
*  mRNA from 5' to 3'. The SPI_ExonProfPtr holds the information about
*  the location of the gaps and mismatches, so this structure is sent
*  to spi_print_mismatch_line, which interprets the information in the
*  ExonProfPtr and creates the mismatch line (vertical bars for identity,
*  nothing for gaps or mismatches).   (PRRESULT)
*
***************************************************************************/
static void SPI_PrintResult(FILE *ofp, FILE *ofp2, SPI_RegionInfoPtr srip, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna, SPI_OptionsPtr spot, Boolean isitCDS)
{
   AlnMsg2Ptr        amp;
   Boolean          begin;
   BioseqPtr        bsp;
   Char             buf[61];
   Int4             c;
   Char             ch;
   Int4             counter;
   Int4             ctr;
   Boolean          done;
   Boolean          end;
   Int4             endctr;
   CharPtr          endstr;
   SPI_ExonProfPtr  epp_curr;
   Int4             gstart, gbuflen = 0;
   Int4             i;
   Boolean          is_splice;
   Int4             j;
   Int4             l;
   Int4             len;
   Int4             maxline;
   Int4             minline;
   Boolean          more;
   Boolean          ng;
   Int4             offset;
   CharPtr          p;
   Int4             pos;
   Char             prot[SPI_LINE+5];
   Int4             r;
   Int4             s;
   SeqAlignPtr      sap;
   Int4             splice;
   SeqPortPtr       spp = NULL;
   Int4             start;
   Boolean          start_prot;
   Int4             PNTR starts;
   Int4             stop;
   Int4             PNTR stops;
   Uint1            strand;
   Boolean          term;
   CharPtr          text;
   Char             text1[200];
   Char             text2[200];
   Char             textid1[42];
   Char             textid2[42];
   Int4             tmp;
   CharPtr          tmpstring;

   if (bsp_genomic == NULL || bsp_mrna == NULL)
      return;
   if (spot->printaln != 3 && ofp == NULL)
      return;
   if (spot->printaln >= 2 && ofp2 == NULL)
      return;
   if (srip != NULL && srip->revcomp)
      BioseqRevComp(bsp_mrna);
   FastaDefLine (bsp_genomic, text1, 200, NULL, NULL, 0);
   SeqIdWrite(bsp_genomic->id, textid1, PRINTID_FASTA_LONG, 41);
   FastaDefLine (bsp_mrna, text2, 200, NULL, NULL, 0);
   SeqIdWrite(bsp_mrna->id, textid2, PRINTID_FASTA_LONG, 41);
   if (spot->printaln != 2)
   {
      if (spot->printheader)
         fprintf(ofp, "--SPIDEY version 1.40--\n");
      fprintf(ofp, "Genomic: %s ", textid1);
      fprintf(ofp, "%s, ", text1);
      fprintf(ofp, "%d bp\n", bsp_genomic->length);
      if (isitCDS)
      {
         tmpstring = StringSave("CDS");
         fprintf(ofp, "CDS: %s ", textid2);
      } else
      {
         tmpstring = StringSave("mRNA");
         fprintf(ofp, "mRNA: %s ", textid2);
      }
      fprintf(ofp, "%s, ", text2);
      if (isitCDS)
      {
         fprintf(ofp, "%d - %d, %d bp\n", srip->mstart+1, srip->mstop+1, srip->mlen);
         offset = srip->mstart; /* this is where the CDS starts */
      } else
      {
         fprintf(ofp, "%d bp\n", bsp_mrna->length);
         offset = 0;
      }
      if (srip == NULL || srip->smp == NULL)
      {
         fprintf(ofp, "No alignment found.\n\n");
         fflush(ofp);
         return;
      }
      if (srip->smp->strand == Seq_strand_minus)
         fprintf(ofp, "Strand: minus");
      else
         fprintf(ofp, "Strand: plus");
      if (srip->revcomp)
         fprintf(ofp, "  Reverse complement\n");
      else
         fprintf(ofp, "\n");
      fprintf(ofp, "Number of exons: %d\n", srip->smp->numexons);
      splice = 0;
      
      for (i=0; i < srip->smp->numexons; i++){
          if (srip->smp->strand == Seq_strand_minus){
              c = srip->smp->numexons - i - 1;
          }
          else {
              c = i;
          }
          splice += srip->smp->splicedon[i];
          epp_curr = srip->smp->epp;
          /* KSK to get correct exon info to report mismatches 
             have to get the exon ptr to the right one */
          while (epp_curr != NULL && epp_curr->exonnum != c + 1){
              epp_curr = epp_curr->next;
          }
          if (srip->revcomp){
              fprintf(ofp, "Exon %d: %d-%d (gen)  %d-%d (%s)  id %.1f%%   mismatches %d  gaps %d  splice site (d  a): %d  %d", i+1, srip->smp->gstarts[c], srip->smp->gstops[c], bsp_mrna->length-srip->smp->mstarts[c]+1, bsp_mrna->length-srip->smp->mstops[c]+1, tmpstring, srip->smp->exonid[c], (epp_curr != NULL ? epp_curr->nummismatches : 0), srip->smp->exongaps[c], srip->smp->splicedon[c], srip->smp->spliceacc[c]);
          }
          else {
              fprintf(ofp, "Exon %d%s: %d-%d (gen)  %d-%d (%s)  id %.1f%% mismatches %d gaps %d  splice site (d  a): %d  %d", i+1, srip->smp->strand == Seq_strand_minus?"(-)":"", srip->smp->gstarts[c], srip->smp->gstops[c], srip->smp->mstarts[c], srip->smp->mstops[c], tmpstring, srip->smp->exonid[c], (epp_curr != NULL ? epp_curr->nummismatches : 0), srip->smp->exongaps[c], srip->smp->splicedon[c], srip->smp->spliceacc[c]);
          }
          if (i > 0 && i<srip->smp->numexons-1 && srip->smp->splicedon[c] == 0 && srip->smp->spliceacc[c] == 0){
              fprintf(ofp, "   uncertain\n");
          }
          else {
              fprintf(ofp, "\n");
          }
      }
      fprintf(ofp, "Number of splice sites: %d\n", splice);
      fprintf(ofp, "%s coverage: %d%%\n", tmpstring, srip->smp->mRNAcoverage);
      fprintf(ofp, "overall percent identity: %.1f%%\n", (FloatHi)(100) - srip->smp->mismatch);
      if (srip->smp->missingends == SPI_BOTH)
         text = StringSave("both");
      else if (srip->smp->missingends == SPI_NEITHER)
         text = StringSave("neither");
      else if (srip->smp->missingends == SPI_LEFT)
         text = StringSave("left");
      else if (srip->smp->missingends == SPI_RIGHT)
         text = StringSave("right");
      else
         text = StringSave("error");
      fprintf(ofp, "Missing %s ends: %s ", tmpstring, text);
      if (srip->fallsoff)
         fprintf(ofp, " -- may fall off end");
      fprintf(ofp, "\n");
      if (!isitCDS) /* print poly(A) tail information for mRNAs */
      {
         if (srip->polyAtail > 0)
            fprintf(ofp, "Non-aligning poly(A)+ tail length: %d\n", srip->polyAtail);
         else if (srip->polyAtail < 0)
               fprintf(ofp, "Aligning poly(A)+ tail length: %d\n", -srip->polyAtail);
      } else /* print UTR %id information for CDSs */
      {
         if (srip->strand != Seq_strand_minus)
         {
            if (srip->utr.left != -1)
               fprintf(ofp, "5' UTR id %.1f%%\n", srip->utr.left);
            if (srip->utr.right != -1)
               fprintf(ofp, "3' UTR id %.1f%%\n", srip->utr.right);
         } else
         {
            if (srip->utr.right != -1)
               fprintf(ofp, "5' UTR id %.1f%%\n", srip->utr.right);
            if (srip->utr.left != -1)
               fprintf(ofp, "3' UTR id %.1f%%\n", srip->utr.left);
         }
         if (srip->gstart == 1)
            fprintf(ofp, "5' partial\n");
         if (srip->gstop == 1)
            fprintf(ofp, "3' partial\n");
      }
      fprintf(ofp, "\n");
      fflush(ofp);
   }
   p = NULL;
   if (spot->printaln != 1)  /* print alignment too */
   {
      if (spot->printaln == 0)
         ofp2 = ofp;
      p = srip->smp->protein;
      pos = srip->smp->transstart;
      epp_curr = srip->smp->epp;
      if (isitCDS && spot->printaln == 2)
         tmpstring = StringSave("CDS");
      else if (spot->printaln == 2)
         tmpstring = StringSave("mRNA");
      fprintf(ofp2, "Genomic: %s %s\n", textid1, text1);
      fprintf(ofp2, "%s: %s %s\n", tmpstring, textid2, text2);
      if (spot->printaln > 0)
      {
         if (srip == NULL || srip->smp == NULL)
         {
            fprintf(ofp2, "No alignment found.\n\n");
            fflush(ofp2);
            return;
         }
         if (srip->smp->strand == Seq_strand_minus)
            fprintf(ofp2, "Strand: minus");
         else
            fprintf(ofp2, "Strand: plus");
         if (srip->revcomp)
            fprintf(ofp2, "  Reverse complement\n");
         else
            fprintf(ofp2, "\n");
         fprintf(ofp2, "Number of exons: %d\n", srip->smp->numexons);
      }
      amp = AlnMsgNew2();
      sap = NULL;
      for (i=0; i<srip->smp->numexons; i++)
      {
         if (sap != NULL)
            SeqAlignFree(sap);
         sap = NULL;
         if (srip->strand == Seq_strand_minus)
         {
            c = srip->smp->numexons - i - 1;
            sap = SeqAlignDup(srip->smp->saps[c]);
            SeqAlignListReverseStrand(sap);
            AlnMgr2IndexSingleChildSeqAlign(sap);
            starts = srip->smp->gstops;
            stops = srip->smp->gstarts;
            epp_curr = srip->smp->epp;
            while (epp_curr != NULL && epp_curr->exonnum != c+1)
            {
               epp_curr = epp_curr->next;
            }
            if (epp_curr != NULL && epp_curr->exonnum == c+1)  /* need to change the mismatch positions now */
            {
               l = AlnMgr2GetAlnLength(sap, FALSE);
               for (j=0; j<epp_curr->nummismatches; j++)
               {
                  epp_curr->mismatches[j] = l - epp_curr->mismatches[j] - 1;
               }
               for (j=0; j<ceil(epp_curr->nummismatches/2); j++)
               {
                  tmp = epp_curr->mismatches[j];
                  epp_curr->mismatches[j] = epp_curr->mismatches[epp_curr->nummismatches-j-1];
                  epp_curr->mismatches[epp_curr->nummismatches-j-1] = tmp;
               }
            }
         } else
         {
            c = i;
            starts = srip->smp->gstarts;
            stops = srip->smp->gstops;
            epp_curr = srip->smp->epp;
            while (epp_curr != NULL && epp_curr->exonnum != c+1)
            {
               epp_curr = epp_curr->next;
            }
         }
         if (srip->revcomp)
            fprintf(ofp2, "Exon %d: %d-%d (gen)  %d-%d (%s)\n", i+1, starts[c], stops[c], bsp_mrna->length-srip->smp->mstarts[c]+1, bsp_mrna->length-srip->smp->mstops[c]+1, tmpstring);
         else
            fprintf(ofp2, "Exon %d: %d-%d (gen)  %d-%d (%s)\n", i+1, starts[c], stops[c], srip->smp->mstarts[c], srip->smp->mstops[c], tmpstring);
         if (sap == NULL)
         {
            sap = SeqAlignDup(srip->smp->saps[c]);
            AlnMgr2IndexSingleChildSeqAlign(sap);
         }
         strand = AlnMgr2GetNthStrand(sap, 1);
         len = AlnMgr2GetAlnLength(sap, FALSE);
         AlnMgr2GetNthSeqRangeInSA(sap, 1, &gstart, NULL);
         end = FALSE;
         ng = FALSE;
         term = FALSE;
         is_splice = FALSE;
         begin = TRUE;
         endstr = NULL;
         for (l=0; l<len+10; l+= SPI_LINE)
         {
            start_prot = TRUE;
            if (l == SPI_LINE)
               l = SPI_LINE - 10;   /* kludge to print genomic splice on 1st line */
            minline = maxline = -1; /* reset mRNA bounds */
            for (j=1; l<len && j<=2; j++)
            {
               fprintf(ofp2, "\n");
               if (j == 1)
               {
                  fprintf(ofp2, "\n");
                  bsp = bsp_genomic;
               } else
               {
                  bsp = bsp_mrna;
                  /* retrieve mismatch information, print the line of vertical bars */
                  spi_print_mismatch_line(ofp2, c+1, l, len-1, epp_curr, gstart);
               }
               AlnMsgReNew2(amp);
               amp->from_aln = l;
               if (l != 0)
               {
                  if (l+SPI_LINE-1 >= len-1)
                  {
                     end = TRUE;
                     amp->to_aln = -1;
                  } else
                     amp->to_aln = l+SPI_LINE-1;
               } else
               {
                  if (begin == FALSE)
                  {
                     if (l+SPI_LINE-1 >= len-1)
                        end = TRUE;
                  } else
                  {
                     if (l+SPI_LINE-1-10 >= len-1)
                        end = TRUE;
                  }
                  amp->to_aln = MIN(SPI_LINE-1-10, len-1);
               }
               amp->row_num = j;
               done = FALSE;
               if (is_splice == TRUE)
                  is_splice = FALSE;
               r=0;
               counter = 0;
               if (j == 2)
                 begin = FALSE;
               while ((Boolean)(more = AlnMgr2GetNextAlnBit(sap, amp)))
               {
                  if (j == 1 && amp->type == AM_SEQ)
                     counter += amp->to_row - amp->from_row + 1;
                  if (j == 2)
                  {
                     if (minline == -1 && amp->type == AM_SEQ)
                        minline = amp->from_row;
                     if (maxline == -1)
                        maxline = amp->to_row;
                      /*  maxline = AlnMgr2MapSeqAlignToBioseq(sap, amp->to_aln, 2);*/
                  }
                  /* print splice site */
                  /** KSK fix for when minus strand is
                      at the end, and simplified this loop  ***/
                  if (l==0 && j==1 && !done){
                      if (amp->strand != Seq_strand_minus){
                          if (amp->from_row < 10){
                              start = 0;
                              gbuflen = amp->from_row;
                              stop = gbuflen - 1;
                          }
                          else {
                              start = amp->from_row - 10;
                              stop = amp->from_row - 1;
                              gbuflen = 10;
                          }
                      } else {
                          if (amp->to_row + 10 < bsp_genomic->length-1){
                              stop = amp->to_row + 10;
                              start = amp->to_row + 1;
                              gbuflen = 10;
                          }
                          else {
                              stop = bsp_genomic->length-1;
                              gbuflen  = (bsp_genomic->length - 1)
                                  - (amp->to_row + 1) + 1;
                              start = amp->to_row + 1;
                          }
                      }
                      /** KSK fix continues so that only as many
                          bases as exist up to 10
                          will be read for the intron
                          buffer **/
                      
                      if (gbuflen > 1){
                          spp = SeqPortNew(bsp_genomic, start, stop, amp->strand, Seq_code_iupacna);
                          ctr =  SeqPortRead(spp, (Uint1Ptr)buf, gbuflen);
                      }
                      else if (gbuflen <= 1){
                          ctr = 0;
                          spp = NULL;
                      }
                      buf[ctr] = '\0';
                      while (ctr < 10){
                          fprintf(ofp2, " ");
                          ctr++;
                      }
                      fprintf(ofp2, buf);
                      if (spp){
                          SeqPortFree(spp);
                      }
                      /** end of region of KSK fix **/
                      
                      done = TRUE;
                  } else if (l==0 && j==2 && !done)
                  {
                     fprintf(ofp2, "          "); /* 10 spaces for splice site */
                     done = TRUE;
                     is_splice = TRUE;
                  } 
                  /**** used no more ***
                  else if (l==0 && j==1 && !done && gstart < 10)
                  {
                     spp = SeqPortNew(bsp_genomic, 0, gstart, amp->strand, Seq_code_iupacna);
                     ctr = SeqPortRead(spp, (Uint1Ptr)buf, gstart);
                     buf[ctr] = '\0';
                     while (ctr < 10)
                     {
                        fprintf(ofp2, " ");
                        ctr++;
                     }
                     fprintf(ofp2, buf);
                  }
                  **********************/
                  if (amp->type == AM_SEQ)
                  {
                     spp = SeqPortNew(bsp, amp->from_row, amp->to_row, amp->strand, Seq_code_iupacna);
                     ctr = SeqPortRead(spp, (Uint1Ptr)buf, SPI_LINE);
                     if (ctr > 0)
                        buf[ctr] = '\0';
                     fprintf(ofp2, "%s", buf);
                     SeqPortFree(spp);
                  } else /* print dashes for gaps */
                  {
                     for (ctr=0; ctr<(amp->to_row - amp->from_row+1); ctr++)
                     {
                        fprintf(ofp2, "-");
                     }
                  }
                  if (j==1 && end && counter >= amp->to_aln - amp->from_aln)
                  {
                     AlnMgr2GetNthSeqRangeInSA(sap, 1, &start, &stop);
                     if (strand != Seq_strand_minus)
                     {
                        if (stop > bsp_genomic->length - 11)
                           start = bsp_genomic->length;
                        else
                           start = stop + SPI_PSPLICE;
                        spp = SeqPortNew(bsp_genomic, stop+1, start, strand, Seq_code_iupacna);
                     } else
                     {
                        if (start < SPI_PSPLICE)
                           stop = 0;
                        else
                           stop = start - SPI_PSPLICE;
                        spp = SeqPortNew(bsp_genomic, stop, start-1, strand, Seq_code_iupacna);
                     }
                     endctr = ctr;
                     ctr = SeqPortRead(spp, (Uint1Ptr)buf, SPI_PSPLICE);
                     if (ctr > 0)
                        buf[ctr] = '\0';
                     for (ctr=0; endctr+ctr <= SPI_LINE-1 && ctr<SPI_PSPLICE; ctr++)
                     {
                        fprintf(ofp2, "%c", buf[ctr]);
                     }
                     endstr = NULL;
                     if (ctr < SPI_PSPLICE)
                        endstr = StringSave(&buf[ctr]);
	                 SeqPortFree(spp);
                  }
                  if (pos <= maxline && amp->type == AM_SEQ && p != NULL && j == 2) /* at least part of this is coding --   */
                  {                                     /* print the protein sequence underneath*/
                     if (is_splice)
                     {
                        is_splice = FALSE;
                        for (s=r; s<r+11; s++)
                        {
                           prot[s] = ' ';
                        }
                        r = s-1;
                     }
                     if (start_prot)
                     {
                        for (ctr = 0; ctr < pos-minline; ctr++)
                        {
                           prot[r] = ' ';
                           r++;
                        }
                        start_prot = FALSE;
                     }
                     if (pos >= amp->from_row-1 && pos <= amp->to_row)
                     {
                        ng = TRUE;
                        if (pos == minline-1)
                        {
                           ch = *p;
                           prot[r] = ch;
                           r++;
                           prot[r] = ' ';
                           r++;
                           if (*p == '*')
                              term = TRUE;
                           p++;
                           pos+=3;
                        }
                        for (ctr = pos; ctr < maxline && *p != '\0' && !term; ctr += 3)
                        {
                           ch = *p;
                           prot[r] = ' ';
                           r++;
                           prot[r] = ch;
                           r++;
                           prot[r] = ' ';
                           r++;
                           if (*p == '*')
                              term = TRUE;
                           p++;
                           pos+=3;
                        }
                     }
                  } else if (j == 2 && amp->type == AM_SEQ && p != NULL && pos>=minline && pos <= maxline)
                  {
                     for (s=0; s<(amp->to_row - amp->from_row+1); s++)
                     {
                        prot[r] = ' ';
                        r++;
                     }
                  }
               }
               if (j == 2 && ng == TRUE)
               {
                  prot[r] = '\0';
                  fprintf(ofp2, "\n%s\n", prot);
               }
            }
         }
         if (endstr == NULL) /* genomic sequence and overhang fit on the same line */
            fprintf(ofp2, "\n\n");
         else /* there's some extra genomic overhang sequence, print it on the next line */
         {
            fprintf(ofp2, "\n\n%s\n\n", endstr);
            MemFree(endstr);
         }
      }
      AlnMsgFree2(amp);
   }
   if (srip->revcomp)
      BioseqRevComp(bsp_mrna);
}

/***************************************************************************
*
*  SPI_PrintHerdResult is analogous to SPI_PrintResult; it prints a
*  summary of the mRNA-to-draft alignment and, if requested, it also
*  prints a text alignment. Since the exons are already in order of the
*  mRNA sequence, printing the text of the alignments is pretty
*  straightforward.
*
***************************************************************************/
static void SPI_PrintHerdResult(FILE *ofp, FILE *ofp2, SPI_mRNAToHerdPtr herd, SPI_OptionsPtr spot, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna)
{
   AlnMsg2Ptr        amp;
   BioseqPtr        bsp;
   Char             buf[61];
   Int4             ctr;
   Boolean          done;
   Boolean          end;
   Int4             endctr;
   CharPtr          endstr;
   SPI_ExonProfPtr  epp_curr;
   Int4             gstart;
   Int4             i;
   Int4             j;
   Int4             l;
   Int4             len;
   Boolean          more;
   Int4             polyAtail;
   SeqAlignPtr      sap;
   SeqPortPtr       spp;
   Int4             start;
   Int4Ptr          starts;
   Int4             stop;
   Int4Ptr          stops;
   Uint1            strand;
   CharPtr          text;
   Char             text1[200];
   Char             text2[200];
   Char             textid1[42];
   Char             textid2[42];

   if (ofp == NULL || herd == NULL || bsp_genomic == NULL || bsp_mrna == NULL)
      return;
   fprintf(ofp, "--SPIDEY version 1.35--\n");
   FastaDefLine (bsp_genomic, text1, 200, NULL, NULL, 0);
   SeqIdWrite(bsp_genomic->id, textid1, PRINTID_FASTA_LONG, 41);
   fprintf(ofp, "Genomic: %s ", textid1);
   fprintf(ofp, "%s, ", text1);
   fprintf(ofp, "%d bp\n", bsp_genomic->length);
   FastaDefLine (bsp_mrna, text2, 200, NULL, NULL, 0);
   SeqIdWrite(bsp_mrna->id, textid2, PRINTID_FASTA_LONG, 41);
   fprintf(ofp, "mRNA: %s ", textid2);
   fprintf(ofp, "%s, ", text2);
   fprintf(ofp, "%d bp\n", bsp_mrna->length);
   if (herd->numpieces == 0)
   {
      fprintf(ofp, "No alignment found.\n\n");
      fflush(ofp);
      return;
   }
   fprintf(ofp, "Number of exons: %d\n", herd->numexons);
   fprintf(ofp, "Number of pieces: %d\n", herd->numpieces);
   for (i=0; i<herd->numpieces; i++)
   {
      fprintf(ofp, "Fragment %d Exon %d: %d-%d (gen)  %d-%d (mRNA)  id %.1f%%  gaps %d  splice site (d  a): %d  %d  ", herd->fragments[i], herd->exons[i], herd->gstarts[i], herd->gstops[i], herd->mstarts[i], herd->mstops[i], (100)*(1-(FloatHi)herd->pmismatch[i]/(FloatHi)herd->lens[i]), herd->pgaps[i], herd->splicedon[i], herd->spliceacc[i]);
      if (herd->strands[i] != Seq_strand_minus)
         fprintf(ofp, "Strand: plus\n");
      else
         fprintf(ofp, "Strand: minus\n");
      if (herd->fallsoff[i] != SPI_NEITHER)
      {
         if (herd->fallsoff[i] == SPI_LEFT)
            fprintf(ofp, "May fall off left side\n");
         else if (herd->fallsoff[i] == SPI_RIGHT)
            fprintf(ofp, "May fall off right side\n");
         else if (herd->fallsoff[i] == SPI_BOTH)
            fprintf(ofp, "May fall off both sides\n");
      }
   }
   fprintf(ofp, "mRNA coverage: %.1f%%\n", herd->mRNAcoverage);
   fprintf(ofp, "overall percent identity: %.1f%%\n", (FloatHi)(100) - herd->mismatch);
   if (herd->missingends == SPI_BOTH)
      text = StringSave("both");
   else if (herd->missingends == SPI_NEITHER)
      text = StringSave("neither");
   else if (herd->missingends == SPI_LEFT)
      text = StringSave("left");
   else if (herd->missingends == SPI_RIGHT)
      text = StringSave("right");
   else
      text = StringSave("error");
   fprintf(ofp, "Missing mRNA ends: %s\n", text);
   polyAtail = SPI_IsItPolyA(bsp_mrna->id);
   if (polyAtail >= SPI_MINPOLYASIZE)
      fprintf(ofp, "Poly(A)+ tail length: %d\n", polyAtail);
   fprintf(ofp, "\n");
   fflush(ofp);
   if (spot->printaln && ofp2 != NULL)  /* print alignment too */
   {
      epp_curr = herd->epp;
      fprintf(ofp2, "Genomic: %s %s\n", textid1, text1);
      fprintf(ofp2, "mRNA: %s %s\n", textid2, text2);
      amp = AlnMsgNew2();
      for (i=0; i<herd->numpieces; i++)
      {
         starts = herd->gstarts;
         stops = herd->gstops;
         epp_curr = herd->epp;
         while (epp_curr != NULL && epp_curr->exonnum != i+1)
         {
            epp_curr = epp_curr->next;
         }
         fprintf(ofp2, "Fragment %d Exon %d: %d-%d (gen)  %d-%d (mRNA)\n", herd->fragments[i], herd->exons[i], starts[i], stops[i], herd->mstarts[i], herd->mstops[i]);
         sap = herd->saps[i];
         strand = AlnMgr2GetNthStrand(sap, 1);
         len = AlnMgr2GetAlnLength(sap, FALSE);
         AlnMgr2GetNthSeqRangeInSA(sap, 1, &gstart, NULL);
         end = FALSE;
         for (l=0; l<len; l+= SPI_LINE)
         {
            if (l == SPI_LINE)
               l = SPI_LINE - 1 - 10;   /* kludge to print genomic splice on 1st line */
            for (j=1; j<=2; j++)
            {
               fprintf(ofp2, "\n");
               if (j == 1)
               {
                  fprintf(ofp2, "\n");
                  bsp = bsp_genomic;
               } else
               {
                  bsp = bsp_mrna;
                  spi_print_mismatch_line(ofp2, i+1, l, len-1, epp_curr, gstart);
               }
               AlnMsgReNew2(amp);
               amp->from_aln = l;
               if (l != 0)
               {
                  if (l+SPI_LINE-1 >= len-1)
                  {
                     end = TRUE;
                     amp->to_aln = -1;
                  } else
                     amp->to_aln = l+SPI_LINE-1;
               } else
               {
                  if (l+SPI_LINE-1 >= len-1)
                     end = TRUE;
                  amp->to_aln = MIN(SPI_LINE-1-10, len-1);
               }
               amp->row_num = j;
               done = FALSE;
               while ((Boolean)(more = AlnMgr2GetNextAlnBit(sap, amp)))
               {
                  if (l==0 && j==1 && !done)  /* print splice site */
                  {
                     if (amp->from_row != 0 && amp->from_row != bsp_genomic->length-1)
                     {
                        if (amp->strand != Seq_strand_minus)
                        {
                           if (amp->from_row < 10)
                              start = 0;
                           else
                              start = amp->from_row - 10;
                           stop = amp->from_row - 1;
                        } else
                        {
                           if (amp->to_row + 10 < bsp_genomic->length-1)
                              stop = amp->to_row + 10;
                           else
                              stop = bsp_genomic->length-1;
                           start = amp->to_row+1;
                        }
                        spp = SeqPortNew(bsp_genomic, start, stop, amp->strand, Seq_code_iupacna);
                        ctr = SeqPortRead(spp, (Uint1Ptr)buf, 10);
                        if (ctr > 0)
                           buf[ctr] = '\0';
                        fprintf(ofp2, buf);
                        SeqPortFree(spp);
                     }
                     done = TRUE;
                  } else if (l==0 && j==2 && !done)
                  {
                     fprintf(ofp2, "          "); /* 10 spaces for splice site */
                     done = TRUE;
                  }
                  if (amp->type == AM_SEQ)
                  {
                     spp = SeqPortNew(bsp, amp->from_row, amp->to_row, amp->strand, Seq_code_iupacna);
                     ctr = SeqPortRead(spp, (Uint1Ptr)buf, SPI_LINE);
                     if (ctr > 0)
                        buf[ctr] = '\0';
                     fprintf(ofp2, "%s", buf);
                     SeqPortFree(spp);
                  } else
                  {
                     for (ctr=0; ctr<(amp->to_row - amp->from_row+1); ctr++)
                     {
                        fprintf(ofp2, "-");
                     }
                  }
                  if (j==1 && end)
                  {
                     AlnMgr2GetNthSeqRangeInSA(sap, 1, &start, &stop);
                     if (strand != Seq_strand_minus)
                     {
                        if (stop > bsp_genomic->length - 11)
                           start = bsp_genomic->length;
                        else
                           start = stop + SPI_PSPLICE;
                        spp = SeqPortNew(bsp_genomic, stop+1, start, strand, Seq_code_iupacna);
                     } else
                     {
                        if (start < SPI_PSPLICE)
                           stop = 0;
                        else
                           stop = start - SPI_PSPLICE;
                        spp = SeqPortNew(bsp_genomic, stop, start-1, strand, Seq_code_iupacna);
                     }
                     endctr = ctr;
                     ctr = SeqPortRead(spp, (Uint1Ptr)buf, SPI_PSPLICE);
                     if (ctr > 0)
                        buf[ctr] = '\0';
                     for (ctr=0; endctr+ctr <= SPI_LINE-1 && ctr<SPI_PSPLICE; ctr++)
                     {
                        fprintf(ofp2, "%c", buf[ctr]);
                     }
                     endstr = NULL;
                     if (ctr < SPI_PSPLICE)
                        endstr = StringSave(&buf[ctr]);
	                 SeqPortFree(spp);
                  }
               }
            }
         }
         if (endstr == NULL)
            fprintf(ofp2, "\n\n");
         else
         {
            fprintf(ofp2, "\n\n%s\n\n", endstr);
            MemFree(endstr);
         }
      }
      AlnMsgFree2(amp);
   }
}

/***************************************************************************
*
*  spi_print_mismatch_line takes a SPI_ExonProf structure and interprets
*  the mismatch locations into a line of vertical bars for identity,
*  nothing for mismatches or gaps. spi_print_mismatch_line only goes from
*  start to start+len-1 each time; it does not interpret the entire structure.
*
***************************************************************************/
static void spi_print_mismatch_line(FILE *ofp, Int4 exonnum, Int4 start, Int4 len, SPI_ExonProfPtr epp, Int4 gstart)
{
   Int4  i;
   Int4  j;
   Int4  length;
   
   if (ofp == NULL)
      return;
   if (start == 0)
   {
      length = MIN(SPI_LINE-10, len+1);
      fprintf(ofp, "          "); /* 10 spaces for splice site */
   } else
      length = MIN(SPI_LINE, len-start+1);
   if (epp != NULL && epp->exonnum == exonnum)
   {
      j = 0;
      while (j<epp->nummismatches && epp->mismatches[j] < start)
      {
         j++;
      }
      for (i=0; i<length; i++)
      {
         if (j<epp->nummismatches && epp->mismatches[j] == start+i) /* here's a mismatch */
         {
            fprintf(ofp, " ");
            j++;
         } else /* not a mismatch */
            fprintf(ofp, "|");
      }
   } else /* there are no mismatches at all in this exon, so just print |s */
   {
      for (i=0; i<length; i++)
      {
         fprintf(ofp, "|");
      }
   }
   fprintf(ofp, "\n");
}

/***************************************************************************
*
*  SPI_CreateContinuousAln creates a single dense-seg seqalign from a
*  set of mRNA-to-genomic alignments. The introns are represented simply
*  as gaps in the mRNA sequence. SPI_CreateContinuousAln calls
*  SPI_ExtendAlnRight to extend each of the exon alignments across the intron,
*  then it merges the exon alignments together to create a single seqalign
*  spanning the entire mRNA-to-genomic alignment.
*
***************************************************************************/
static SeqAlignPtr SPI_CreateContinuousAln(SeqAlignPtr PNTR saps, Int4 numsaps)
{
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_tmp;
   Int4         i;
   Int4         j;
   Int4         n1;
   Int4         n2;
   Int4         numseg;
   SeqAlignPtr  salp;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;
   Uint1        strand;
   
   for (i=0; i<numsaps-1; i++)
   {
      AlnMgr2GetNthSeqRangeInSA(saps[i], 1, &start1, &stop1);
      AlnMgr2GetNthSeqRangeInSA(saps[i+1], 1, &start2, &stop2);
      if (start2 - stop1 > 1)  /* genomic gap */
         SPI_ExtendAlnRight(saps[i], 1, stop1+1, start2-1);
      AlnMgr2GetNthSeqRangeInSA(saps[i], 2, &start1, &stop1);
      AlnMgr2GetNthSeqRangeInSA(saps[i+1], 2, &start2, &stop2);
      strand = AlnMgr2GetNthStrand(saps[i], 2);
      if (strand == Seq_strand_minus)
      {
         if (start1 - stop2 > 1)
            SPI_ExtendAlnRight(saps[i], 2, stop2+1, start1-1);
      } else
      {
         if (start2 - stop1 > 1)
            SPI_ExtendAlnRight(saps[i], 2, stop1+1, start2-1);
      }
   }
   numseg = 0;
   for (i=0; i<numsaps; i++)
   {
      dsp_tmp = (DenseSegPtr)(saps[i]->segs);
      numseg += dsp_tmp->numseg;
   }
   /* now make a new seqalign across the whole set */
   dsp = DenseSegNew();
   dsp->dim = 2;
   dsp->numseg = numseg;
   dsp->starts = (Int4Ptr)MemNew(2*numseg*sizeof(Int4));
   dsp->lens = (Int4Ptr)MemNew(numseg*sizeof(Int4));
   dsp->strands = (Uint1Ptr)MemNew(2*numseg*sizeof(Uint1));
   n1 = n2 = 0;
   for (i=0; i<numsaps; i++)
   {
      dsp_tmp = (DenseSegPtr)(saps[i]->segs);
      if (dsp->ids == NULL)
         dsp->ids = SeqIdDupList(dsp_tmp->ids);
      for (j=0; j<2*dsp_tmp->numseg; j++)
      {
         dsp->starts[n1+j] = dsp_tmp->starts[j];
         dsp->strands[n1+j] = dsp_tmp->strands[j];
      }
      for (j=0; j<dsp_tmp->numseg; j++)
      {
         dsp->lens[n2+j] = dsp_tmp->lens[j];
      }
      n1 += 2*dsp_tmp->numseg;
      n2 += dsp_tmp->numseg;
   }
   salp = SeqAlignNew();
   salp->type = SAT_PARTIAL;
   salp->segtype = SAS_DENSEG;
   salp->dim = 2;
   salp->segs = (Pointer)(dsp);
   AlnMgr2IndexSingleChildSeqAlign(salp);
   return salp;
}

/***************************************************************************
*
*  SPI_ExtendAlnRight is used by SPI_CreateContinuousAln to extend each
*  exon alignment across the intron. SPI_ExtendAlnRight simply adds a
*  segment to the exon alignment (or extends an existing segment, if
*  possible) that has a gap in the mRNA sequence. SPI_ExtendAlnRight assumes
*  that the input alignment is a child seqalign with two rows.
*
***************************************************************************/
static void SPI_ExtendAlnRight(SeqAlignPtr sap, Int4 which_row, Int4 start, Int4 stop)
{
   DenseSegPtr  dsp;
   Int4         i;
   Int4Ptr      lens;
   Int4Ptr      starts;
   Uint1Ptr     strands;
   
   if (sap == NULL)
      return;
   if (which_row > 2)
      return;
   dsp = (DenseSegPtr)(sap->segs);
   if (dsp->starts[2*(dsp->numseg-1) + which_row - 1] == -1 || dsp->starts[2*(dsp->numseg-1) + (2-which_row)] != -1)
   {
      starts = (Int4Ptr)MemNew((dsp->numseg+1)*2*sizeof(Int4));
      strands = (Uint1Ptr)MemNew((dsp->numseg+1)*2*sizeof(Uint1));
      lens = (Int4Ptr)MemNew((dsp->numseg+1)*sizeof(Int4));
      for (i=0; i<dsp->numseg; i++)
      {
         lens[i] = dsp->lens[i];
      }
      for (i=0; i<=(dsp->dim)*(dsp->numseg-1)+1; i++)
      {
         starts[i] = dsp->starts[i];
         strands[i] = dsp->strands[i];
      }
      lens[dsp->numseg] = stop - start + 1;
      if (dsp->strands[which_row-1] != Seq_strand_minus)
         starts[(dsp->dim)*(dsp->numseg) + which_row - 1] = start;
      else
         starts[(dsp->dim)*(dsp->numseg) + which_row - 1] = stop;
      starts[(dsp->dim)*(dsp->numseg) + (2-which_row)] = -1;
      strands[(dsp->dim)*(dsp->numseg) + which_row - 1] = dsp->strands[which_row-1];
      strands[(dsp->dim)*(dsp->numseg) + (2-which_row)] = dsp->strands[2-which_row];
      MemFree(dsp->starts);
      MemFree(dsp->lens);
      MemFree(dsp->strands);
      dsp->numseg++;
      dsp->starts = starts;
      dsp->strands = strands;
      dsp->lens = lens;
   } else
   {
      dsp->lens[dsp->numseg-1] += stop - start + 1;
      if (dsp->strands[which_row-1] == Seq_strand_minus)
         dsp->starts[(dsp->dim)*(dsp->numseg-1) + which_row - 1] = stop;
   }
   SAIndex2Free2(sap->saip);
   sap->saip = NULL;
   AlnMgr2IndexSingleChildSeqAlign(sap);
}

/***************************************************************************
*
*  SPI_AlnSinglemRNAToPieces is the entry point for the mRNA-to-draft
*  sequence functions of spidey, which create an alignment between mRNAs
*  and a series of ordered, unordered, oriented, or unoriented (often
*  a mixture of all of the above) fragments. SPI_AlnSinglemRNAToPieces
*  reads in a tab-delimited file that has information about the order
*  and orientation of the fragments. For example:

*  ctg name  start   stop  fragment number accession.version
*    |        |       |        | fragment code |   start  stop  strand
*  9/ctg119 775986   784968	137	D	AC020712.4	96692	105674	-
*  9/ctg119	784969	810517	138	D	AC022758.3	103385	128933	+
*  9/ctg119	810518	810880	139	D	AC020712.4	54074	54436	+
*  9/ctg119	810881	822654	140	D	AC022758.3	16691	28464	+
*  9/ctg119	822655	822754	141	N	100	fragment	yes	
*  9/ctg119	822755	823638	142	D	AC020712.4	153248	154131	-
*  9/ctg119 823639   823738   143   N  100   fragment no
*  9/ctg119 823739   824581   144   F  AC021710.5  1728  2570  +

*  The fragment code indicates whether the fragment is draft quality (D),
*  finished (F), predraft (P), or a gap (N). If the fragment is a gap 
*  (which consists of 100 Ns), the yes/no field indicates whether the
*  adjoining fragments are ordered across the gap. In spidey, a group
*  is a set of fragments which all reside between the same two gaps. 
*  Linked groups, or lgroups, are two groups spanning a gap with a "yes"
*  indicating that the groups have known order. Each fragment has an
*  order within the group, as well.
*  In this example, there are two lgroups (the last fragment is in its
*  own lgroup) and three groups.
*  SPI_AlnSinglemRNAToPieces reads in the tab-delimited file and creates
*  a SPI_Pos structure for each fragment, indicating which group, lgroup,
*  order, and original fragment number this fragment belongs to. Since 
*  the gap fragments are not used, the number of SPI_FragPtrs will be
*  less than the number of fragments, so the original fragment numbers
*  must be stored. This function also does the initial high-stringency
*  BLAST alignment of the mRNA and the draft sequence; the alignments
*  are put on the correct strand and then sent to other functions to
*  order the alignments, make them consistent, connect them together,
*  adjust the ends to splice sites, and finally to get summary
*  statistics for printing.
*
***************************************************************************/
NLM_EXTERN SPI_mRNAToHerdPtr SPI_AlnSinglemRNAToPieces(SPI_bsinfoPtr spig_head, SPI_bsinfoPtr spim, FILE *ofp, FILE *ofp2, SPI_OptionsPtr spot)
{
   AMAlignIndex2Ptr      amaip;
   Int4                 c;
   CharPtr              field[SPI_NUMCOLS];
   FILE                 *fp;
   Int4                 group;
   SPI_mRNAToHerdPtr    herd;
   Int4                 i;
   Char                 line[200];
   Boolean              linked;
   Int4                 lgroup;
   Int4                 numFields;
   BLAST_OptionsBlkPtr  options;
   Int4                 order;
   SPI_PosPtr           posp;
   CharPtr              ptr;
   Char                 token;
   SeqAlignPtr          salp;
   SeqAlignPtr          salp_prev;
   SeqAlignPtr          salp_tmp;
   SeqAlignPtr          sap;
   SeqAlignPtr          sap1;
   SeqAlignPtr          sap2;
   SPI_FragPtr          sfp;
   SPI_FragPtr          sfp_head;
   SPI_FragPtr          sfp_prev;
   SPI_FragHerdPtr      sfhp;
   SeqLocPtr            slp1;
   SeqLocPtr            slp2;
   Int4                 start;
   Int4                 stop;
   Uint1                strand;

   if (spot->draftfile == NULL)
      return NULL;
   if (spot->to < spot->from)
      return NULL;
   fp = FileOpen(spot->draftfile, "r");
   if (fp == NULL)
      return NULL;
   sfhp = (SPI_FragHerdPtr)MemNew(sizeof(SPI_FragHerd));
   sfp_head = sfp_prev = NULL;
   group = 0;
   order = 0;
   lgroup = 0;
   linked = FALSE;
   while (fgets(line, sizeof (line), fp) != NULL)
   {
      memset(field, 0, sizeof (field));
      ptr = line;
      if ((ptr = strchr(ptr, '\t')) == NULL)
         token = ' ';
      else
         token = '\t';
      ptr = line;
      for (numFields=0; numFields < SPI_NUMCOLS && ptr != NULL; numFields++)
      {
         if (numFields == 0)
            ptr = strtok(ptr, &token);
         else
            ptr = strtok(NULL, &token);
         field[numFields] = ptr;
      }
      if (!StringICmp(field[4], "N")) /* gap */
      {
         order = 0;
         if (!StringNICmp(field[7], "yes", 3*sizeof(Char))) /* ordered across gap */
         {
            linked = TRUE;
            if (sfp_prev != NULL)
            {
               if (sfp_prev->position_orig->lgroup != 0)
                  lgroup = sfp_prev->position_orig->lgroup;
               else
               {
                  lgroup++;
                  sfp_prev->position_orig->lgroup = lgroup;
               }
            } else
               lgroup++;
         } else
            linked = FALSE;
      } else
      {
         order++;
         sfp = (SPI_FragPtr)MemNew(sizeof(SPI_Frag));
         sfp->start = atol(field[1]);
         sfp->stop = atol(field[2]);
         sfp->fragnum = atol(field[3]);
         posp = (SPI_PosPtr)MemNew(sizeof(SPI_Pos));
         if (linked)
            posp->lgroup = lgroup;
         if (order > 2)
            posp->group = group;
         else if (order == 2)
         {
            if (sfp_prev != NULL)
               sfp_prev->position_orig->group = group;
         } else
            group++;
         posp->group = group;
         posp->order = order;
         sfp->position_orig = posp;
         if (sfp_head != NULL)
         {
            sfp_prev->next = sfp;
            sfp_prev = sfp;
         } else
            sfp_head = sfp_prev = sfp;
         sfhp->numfrags++;
      }
   }
   sfhp->sfparray = (SPI_FragPtr PNTR)MemNew((sfhp->numfrags)*sizeof(SPI_FragPtr));
   sfp = sfp_head;
   for (i=0; i<sfhp->numfrags; i++)
   {
      sfhp->sfparray[i] = sfp;
      sfp = sfp->next;
   }
   sfhp->polyAtail = SPI_IsItPolyA(spim->bsp->id);
   /* search genomic against both strands of mRNA */
   if (spot->from == spot->to == 0)
      spot->to = spig_head->bsp->length-1;
   slp2 = SeqLocIntNew(0, spim->bsp->length-1-sfhp->polyAtail, Seq_strand_minus, spim->bsp->id);
   slp1 = SeqLocIntNew(spot->from, spot->to, Seq_strand_plus, spig_head->bsp->id);
   options = BLASTOptionNew("blastn", FALSE);
   options->filter_string = StringSave("m L");
   options->expect_value = spot->secpasseval;
   options->query_lcase_mask = spot->lcaseloc;
   if (spot->interspecies)
   {
      options->gap_x_dropoff_final = 100;
      options->gap_open = 5;
      options->gap_extend = 1;
      options->penalty = -1;
   }
   sap1 = BlastTwoSequencesByLoc(slp2, slp1, "blastn", options);
   SeqLocFree(slp2);
   BLASTOptionDelete(options);
   slp2 = SeqLocIntNew(0, spim->bsp->length-1-sfhp->polyAtail, Seq_strand_minus, spim->bsp->id);
   options = BLASTOptionNew("blastn", FALSE);
   options->filter_string = StringSave("m L");
   options->expect_value = spot->secpasseval;
   options->query_lcase_mask = spot->lcaseloc;
   if (spot->interspecies)
   {
      options->gap_x_dropoff_final = 100;
      options->gap_open = 5;
      options->gap_extend = 1;
      options->penalty = -1;
   }
   sap2 = BlastTwoSequencesByLoc(slp2, slp1, "blastn", options);
   SeqLocFree(slp1);
   SeqLocFree(slp2);
   BLASTOptionDelete(options);
   AlnMgr2IndexLite(sap1);
   AlnMgr2IndexLite(sap2);
   sap = NULL;
   if (sap1 != NULL && sap2 != NULL)
   {
      salp = (SeqAlignPtr)(sap1->segs);
      while (salp->next != NULL)
      {
         salp = salp->next;
      }
      salp->next = (SeqAlignPtr)(sap2->segs);
      sap2->segs = NULL;
      SeqAlignFree(sap2);
      AMAlignIndex2Free2(sap1->saip);
      sap1->saip = NULL;
      AlnMgr2IndexLite(sap1);
      sap = sap1;
   } else if (sap1 == NULL)
      sap = sap2;
   else
      sap = sap1;
   if (sap == NULL)
      return NULL;
   SPI_flip_sa_list((SeqAlignPtr)(sap->segs));
   AlnMgr2SortAlnSetByNthRowPos(sap, 1);
   c = 0;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 1, &start, &stop);
   for (i=0; i<sfhp->numfrags && c<amaip->numsaps; i++)
   {
      salp_tmp = salp_prev = NULL;
      while (sfhp->sfparray[i]->start <= start && sfhp->sfparray[i]->stop >= start && c<amaip->numsaps)
      {
         if (salp_tmp == NULL)
            salp_tmp = salp_prev = SeqAlignDup(amaip->saps[c]);
         else
         {
            salp_prev->next = SeqAlignDup(amaip->saps[c]);
            salp_prev = salp_prev->next;
         }
         c++;
         if (c<amaip->numsaps)
            AlnMgr2GetNthSeqRangeInSA(amaip->saps[c], 1, &start, &stop);
      }
      if (salp_tmp != NULL)
      {
         AlnMgr2IndexLite(salp_tmp);
         SPI_RemoveInconsistentAlnsFromSet(salp_tmp, SPI_TEENYEXON, 1, SPI_LEFT);
         sfhp->sfparray[i]->sap = salp_tmp;
         /* change all alignments to be on the plus strand of the mRNA */
         strand = AlnMgr2GetNthStrand((SeqAlignPtr)(salp_tmp->segs), 2);
         if (strand == Seq_strand_minus)
            SeqAlignListReverseStrand((SeqAlignPtr)(salp_tmp->segs));
      }
   }
   SeqAlignSetFree(sap);
   SPI_OrderInternally(sfhp);
   /* take out overlaps */
   SPI_RemoveConflictsAmongPieces(sfhp, SPI_TEENYEXON);
   /* do an initial ordering */
   SPI_OrderPieces(sfhp, spim->bsp);
   /* then look for missing pieces */
   if (!SPI_ConnectAlnPieces(sfhp, spig_head->bsp, spim->bsp, spot))
      return NULL;
   SPI_OrderInternally(sfhp);
   /* take out any remaining overlaps */
   SPI_RemoveConflictsAmongPieces(sfhp, SPI_TEENYEXON);
   /* then do the final ordering */
   SPI_OrderPieces(sfhp, spim->bsp);
   SPI_AdjustSplicesInPieces(sfhp, spig_head->bsp, spot);
   herd = SPI_GetHerdInfo(sfhp, spim->bsp, spot);
   SPI_PrintHerdResult(ofp, ofp2, herd, spot, spig_head->bsp, spim->bsp);
   return herd;
}

/***************************************************************************
*
*  SPI_GetHerdInfo fills in a SPI_mRNAToHerd structure with all the 
*  appropriate information about mRNA and genomic starts, stops, and strands;
*  presence of splice donor and acceptor sites; number of mismatches and
*  gaps for each exon; and one alignment for each exon. SPI_GetHerdInfo
*  first decides how many exons there are and allocates one ExonHerdInfo
*  structure per exon to store the necessary information. The SPI_mRNAToHerd
*  structure is then allocated, and for each exon, SPI_GetExonInfo is called
*  to retrieve the number of gaps, the number of mismatches, and the
*  mismatch line for printing. After all the exons' information is filled
*  in, the function goes through again and checks to see whether any two
*  exons are close to the edges of their respective fragments and abut
*  each other on the mRNA. If so, these "exons" are probably a single exon
*  and are assigned the same exon number. Finally, the alignments are
*  checked to see whether small pieces at the 5' and 3' ends have been
*  omitted; if so, these pieces are added to the alignments.
*
***************************************************************************/
static SPI_mRNAToHerdPtr SPI_GetHerdInfo(SPI_FragHerdPtr sfhp, BioseqPtr bsp_mrna, SPI_OptionsPtr spot)
{
   AMAlignIndex2Ptr      amaip;
   Int4                 b;
   Int4                 c;
   SPI_ExonHerdInfoPtr  ehi;
   SPI_ExonHerdInfoPtr  ehi_head;
   SPI_ExonHerdInfoPtr  ehi_prev;
   Int4                 end;
   SPI_mRNAToHerdPtr    herd;
   Int4                 i;
   Int4                 j;
   Int4                 k;
   Int4                 l;
   Int4                 last;
   Int4                 len;
   Int4                 len_last;
   Int4                 max;
   Int4                 min;
   Int4                 mis;
   Int4                 offset;
   SeqAlignPtr          salp;
   SeqAlignPtr          salp_tmp;
   SPI_FragPtr          sfp;
   SPI_mRNAPtr          smp_fake;
   Uint1                strand;
   
   herd = (SPI_mRNAToHerdPtr)MemNew(sizeof(SPI_mRNAToHerd));
   ehi_head = ehi_prev = NULL;
   for (i=0; i<sfhp->numfrags; i++)
   {
      sfp = sfhp->sfparray[i];
      if (sfp->sap != NULL)
      {
         amaip = (AMAlignIndex2Ptr)(sfp->sap->saip);
         strand = AlnMgr2GetNthStrand(amaip->saps[0], 1);
         if (strand == Seq_strand_minus)
         {
            k = amaip->numsaps-1;
            l = -1;
         } else
         {
            k = 0;
            l = 1;
         }
         for (j=k; j<amaip->numsaps && j > -1; j+=l)
         {
            ehi = (SPI_ExonHerdInfoPtr)MemNew(sizeof(SPI_ExonHerdInfo));
            ehi->sfpnum = i;
            herd->numpieces++;
            ehi->sap = amaip->saps[j];
            ehi->fragmentnum = sfp->fragnum;
            if (amaip->numsaps == 1)
            {
               ehi->acceptor = sfp->acceptor;
               ehi->donor = sfp->donor;
            } else if (j == 0)
            {
               ehi->acceptor = sfp->acceptor;
               ehi->donor = sfp->smp->splicedon[j];
            } else if (j == amaip->numsaps - 1)
            {
               ehi->donor = sfp->donor;
               ehi->acceptor = sfp->smp->spliceacc[j];
            } else
            {
               ehi->donor = sfp->smp->splicedon[j];
               ehi->acceptor = sfp->smp->spliceacc[j];
            }
            if (ehi_head != NULL)
            {
               ehi_prev->next = ehi;
               ehi_prev = ehi;
            } else
               ehi_head = ehi_prev = ehi;
         }
      }
   }
   ehi = ehi_head;
   if (ehi == NULL)
      return NULL;
   herd->fragments = (Int4Ptr)MemNew((herd->numpieces)*sizeof(Int4));
   herd->sfpnum = (Int4Ptr)MemNew((herd->numpieces)*sizeof(Int4));
   herd->exons = (Int4Ptr)MemNew((herd->numpieces)*sizeof(Int4));
   herd->mstarts = (Int4Ptr)MemNew((herd->numpieces)*sizeof(Int4));
   herd->mstops = (Int4Ptr)MemNew((herd->numpieces)*sizeof(Int4));
   herd->gstarts = (Int4Ptr)MemNew((herd->numpieces)*sizeof(Int4));
   herd->gstops = (Int4Ptr)MemNew((herd->numpieces)*sizeof(Int4));
   herd->lens = (Int4Ptr)MemNew((herd->numpieces)*sizeof(Int4));
   herd->strands = (Uint1Ptr)MemNew((herd->numpieces)*sizeof(Uint1));
   herd->splicedon = (Uint1Ptr)MemNew((herd->numpieces)*sizeof(Uint1));
   herd->spliceacc = (Uint1Ptr)MemNew((herd->numpieces)*sizeof(Uint1));
   herd->pmismatch = (Int4Ptr)MemNew((herd->numpieces)*sizeof(Int4));
   herd->pgaps = (Int4Ptr)MemNew((herd->numpieces)*sizeof(Int4));
   herd->saps = (SeqAlignPtr PNTR)MemNew((herd->numpieces)*sizeof(SeqAlignPtr));
   herd->fallsoff = (Uint1Ptr)MemNew((herd->numpieces)*sizeof(Uint1));
   i = 0;
   smp_fake = (SPI_mRNAPtr)MemNew(sizeof(SPI_mRNA));
   smp_fake->mstarts = (Int4Ptr)MemNew(sizeof(Int4));
   smp_fake->mstops = (Int4Ptr)MemNew(sizeof(Int4));
   smp_fake->gstarts = (Int4Ptr)MemNew(sizeof(Int4));
   smp_fake->gstops = (Int4Ptr)MemNew(sizeof(Int4));
   smp_fake->exonid = (FloatHiPtr)MemNew(sizeof(FloatHi));
   smp_fake->exongaps = (Int4Ptr)MemNew(sizeof(Int4));
   smp_fake->saps = (SeqAlignPtr PNTR)MemNew(sizeof(SeqAlignPtr));
   mis = 0;
   len = 0;
   len_last = 0;
   while (ehi != NULL) /* fill in info for each piece */
   {
      herd->splicedon[i] = ehi->donor;
      herd->spliceacc[i] = ehi->acceptor;
      herd->strands[i] = AlnMgr2GetNthStrand(ehi->sap, 1);
      herd->saps[i] = ehi->sap;
      smp_fake->saps[0] = ehi->sap;
      last = mis;
      len += SPI_GetExonInfo(smp_fake, 0, &b, &c, &mis, spot);
      herd->pmismatch[i] = mis - last;
      herd->lens[i] = len - len_last;
      herd->mstarts[i] = b;
      herd->mstops[i] = c;
      herd->gstarts[i] = smp_fake->gstarts[0];
      herd->gstops[i] = smp_fake->gstops[0];
      herd->pgaps[i] = smp_fake->exongaps[0];
      sfp = sfhp->sfparray[ehi->sfpnum];
      herd->fragments[i] = ehi->fragmentnum;
      herd->sfpnum[i] = ehi->sfpnum;
      if (herd->gstarts[i] < sfp->start + SPI_FUZZ)
      {
         if (herd->gstops[i] > sfp->stop - SPI_FUZZ)
            herd->fallsoff[i] = SPI_BOTH;
         else
            herd->fallsoff[i] = SPI_LEFT;
      } else
      {
         if (herd->gstops[i] > sfp->stop - SPI_FUZZ)
            herd->fallsoff[i] = SPI_RIGHT;
         else
            herd->fallsoff[i] = SPI_NEITHER;
      }
      ehi_prev = ehi;
      ehi = ehi->next;
      ehi_prev->next = NULL;
      MemFree(ehi_prev);
      i++;
   }
   herd->mismatch = (100)*(FloatHi)mis/(FloatHi)len;
   herd->epp = smp_fake->epp;
   herd->mRNAcoverage = 100*((FloatHi)len/(FloatHi)bsp_mrna->length);
   /* now run through to see whether any two pieces should be */
   /* merged into a single exon (both near fragment edges)    */
   b = 1;
   for (i=0; i<herd->numpieces-1; i++)
   {
      if (((herd->fallsoff[i] == SPI_RIGHT && herd->strands[i] == Seq_strand_plus)
        || (herd->fallsoff[i] == SPI_LEFT && herd->strands[i] == Seq_strand_minus) 
        || (herd->fallsoff[i] == SPI_BOTH)) && 
           ((herd->fallsoff[i+1] == SPI_LEFT && herd->strands[i+1] == Seq_strand_plus)
        || (herd->fallsoff[i+1] == SPI_RIGHT && herd->strands[i+1] == Seq_strand_minus)
        || (herd->fallsoff[i+1] == SPI_BOTH)))
         {
            if (herd->mstarts[i+1] < herd->mstops[i] - SPI_TEENYEXON)
               herd->exons[i] = b;
         }
      else
      {
         herd->exons[i] = b;
         if (herd->fallsoff[i] == SPI_RIGHT)
            herd->fallsoff[i] = SPI_NEITHER;
         if (herd->fallsoff[i] == SPI_BOTH)
            herd->fallsoff[i] = SPI_LEFT;
         if (herd->fallsoff[i+1] == SPI_LEFT)
            herd->fallsoff[i+1] = SPI_NEITHER;
         if (herd->fallsoff[i+1] == SPI_BOTH)
            herd->fallsoff[i+1] = SPI_RIGHT;
         b++;
      }
   }
   herd->exons[i] = b;
   herd->numexons = b;
   /* now get %id per exon, #gaps per exon */
   i = 0;
   herd->exonid = (FloatHiPtr)MemNew((herd->numexons)*sizeof(FloatHi));
   herd->exongaps = (Int4Ptr)MemNew((herd->numexons)*sizeof(Int4));
   while (i<herd->numpieces)
   {
      min = herd->mstarts[i];
      b = herd->pmismatch[i];
      c = herd->pgaps[i];
      while (i<herd->numpieces-1 && herd->exons[i] == herd->exons[i+1])
      {
         i++;
         b += herd->pmismatch[i];
         c += herd->pgaps[i];
      }
      max = herd->mstops[i];
      herd->exongaps[herd->exons[i]-1] = c;
      herd->exonid[herd->exons[i]-1] = (FloatHi)(max - min + 1 - c - b)/(FloatHi)(max - min + 1 - c);
      i++;
   }
   /* check -- does the alignment leave off a tiny piece of the beginning */
   /* or end of the mRNA (ignoring the polyA tail) ? */
   /* first check the beginning */
   end = bsp_mrna->length - 1 - sfhp->polyAtail;
   if (herd->mstarts[0] > 0 && herd->mstarts[0] < SPI_TEENYEXON)
   {
      strand = AlnMgr2GetNthStrand(herd->saps[0], 1);
      if (strand == Seq_strand_minus)
      {
         salp = (SeqAlignPtr)(herd->saps[0]);
         salp_tmp = salp->next;
         salp->next = NULL;
         SAIndex2Free2(salp->saip);
         salp->saip = NULL;
         SeqAlignListReverseStrand(salp);
         AlnMgr2IndexSingleChildSeqAlign(salp);
         salp->next = salp_tmp;
      }
      sfp = sfhp->sfparray[herd->sfpnum[0]];
      offset = herd->mstarts[0];
      herd->mstarts[0] = 0;
      if (strand == Seq_strand_minus)
         herd->gstops[0] += offset;
      else
         herd->gstarts[0] -= offset;
      if (herd->gstarts[0] < sfp->start + herd->mstarts[0])
      {
         herd->mstarts[0] = herd->mstarts[0] - (herd->gstarts[0] - sfp->start);
         herd->gstarts[0] = sfp->start;
         offset = herd->gstarts[0] - sfp->start;
      } else if (herd->gstops[0] > sfp->stop - herd->mstarts[0])
      {
         herd->mstarts[0] = herd->mstarts[0] - (sfp->stop - herd->gstops[0]);
         herd->gstops[0] = sfp->stop;
         offset = sfp->stop - herd->gstops[0];
      }
      SPI_AddToAln(herd->saps[0], offset, SPI_LEFT, strand);
      if (strand == Seq_strand_minus)
      {
         salp = (SeqAlignPtr)(herd->saps[0]);
         salp_tmp = salp->next;
         salp->next = NULL;
         SAIndex2Free2(salp->saip);
         salp->saip = NULL;
         SeqAlignListReverseStrand(salp);
         AlnMgr2IndexSingleChildSeqAlign(salp);
         salp->next = salp_tmp;
      }
   }
   /* now check the end */
   if (herd->mstops[herd->numpieces-1] > end - SPI_TEENYEXON && herd->mstops[herd->numpieces-1] != end)
   {
      strand = AlnMgr2GetNthStrand(herd->saps[herd->numpieces-1], 1);
      if (strand == Seq_strand_minus)
      {
         salp = (SeqAlignPtr)(herd->saps[herd->numpieces-1]);
         salp_tmp = salp->next;
         salp->next = NULL;
         SAIndex2Free2(salp->saip);
         salp->saip = NULL;
         SeqAlignListReverseStrand(salp);
         AlnMgr2IndexSingleChildSeqAlign(salp);
         salp->next = salp_tmp;
      }
      sfp = sfhp->sfparray[herd->sfpnum[herd->numpieces-1]];
      offset = end - herd->mstops[herd->numpieces-1];
      herd->mstops[herd->numpieces-1] = bsp_mrna->length-1;
      if (strand == Seq_strand_minus)
         herd->gstarts[herd->numpieces-1] -= offset;
      else
         herd->gstops[herd->numpieces-1] += offset;
      if (herd->gstarts[herd->numpieces-1] < sfp->start + offset)
      {
         offset = herd->gstops[herd->numpieces-1] - sfp->start;
         herd->mstops[herd->numpieces-1] = herd->mstops[herd->numpieces-1] + offset;
         herd->gstarts[herd->numpieces-1] = sfp->start;
      } else if (herd->gstops[herd->numpieces-1] > sfp->stop - offset)
      {
         offset = sfp->stop - herd->gstops[herd->numpieces-1];
         herd->mstops[herd->numpieces-1] = herd->mstops[herd->numpieces-1] - offset;
         herd->gstops[herd->numpieces-1] = sfp->stop;
      }
      SPI_AddToAln(herd->saps[herd->numpieces-1], offset, SPI_RIGHT, strand);
      if (strand == Seq_strand_minus)
      {
         salp = (SeqAlignPtr)(herd->saps[herd->numpieces-1]);
         salp_tmp = salp->next;
         salp->next = NULL;
         SAIndex2Free2(salp->saip);
         salp->saip = NULL;
         SeqAlignListReverseStrand(salp);
         AlnMgr2IndexSingleChildSeqAlign(salp);
         salp->next = salp_tmp;
      }
   }
   if (herd->mstarts[0] > 0)
   {
      if (herd->mstops[herd->numpieces-1] < bsp_mrna->length-1)
         herd->missingends = SPI_BOTH;
      else
         herd->missingends = SPI_LEFT;
   } else
   {
      if (herd->mstops[herd->numpieces-1] < bsp_mrna->length-1)
         herd->missingends = SPI_RIGHT;
      else
         herd->missingends = SPI_NEITHER;
   }
   MemFree(smp_fake->mstarts);
   MemFree(smp_fake->mstops);
   MemFree(smp_fake->gstarts);
   MemFree(smp_fake->gstops);
   MemFree(smp_fake->exonid);
   MemFree(smp_fake->exongaps);
   MemFree(smp_fake->saps);
   MemFree(smp_fake);
   return herd;
}

/***************************************************************************
*
*  SPI_FindWindows first sorts all the alignments by score, and then
*  sends the array to SPI_AssembleRegions, which puts together
*  nonoverlapping regions containing one or more alignments each. The idea:
*
*  0-----100----200----300----400----500----600----700----800---- (genomic)
*  0  \        \                       \        \
*  50  \        \                                 \
*  100  \                                          \
*  150               \                               \
*  200                \                               \
*  (mRNA)
*    <-1->    <---2---->              <-3->    <---4--->
*  This set of initial alignments defines 4 windows; the best n windows
*  will be chosen and the alignment will be refined in those windows.
*
***************************************************************************/
static SPI_RegionInfoPtr SPI_FindWindows(SeqAlignPtr sap, SPI_OptionsPtr spot)
{
   AMAlignIndex2Ptr    amaip;
   /* FloatHi            bit_score; */
   /* FloatHi            evalue; */   
   Int4               i;
   /* Int4               number; */
   SeqAlignPtr        salp;
   /* Int4               score; */
   SPI_AlnInfoPtr     PNTR spip_list;
   SPI_RegionInfoPtr  srip_head;
FloatHi  s, s1;
Int4  s2, tmp;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return NULL;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   spip_list = (SPI_AlnInfoPtr PNTR)MemNew((amaip->numsaps)*sizeof(SPI_AlnInfoPtr));
   for (i=0; i<amaip->numsaps; i++)
   {
      salp = amaip->saps[i];
      spip_list[i] = (SPI_AlnInfoPtr)MemNew(sizeof(SPI_AlnInfo));
      spip_list[i]->sap = salp;
      salp->next = NULL;
/*
      spip_list[i]->bit_score = AlnMgr2ComputeScoreForSeqAlign(salp);*/
      tmp = spip_list[i]->bit_score;
      GetScoreAndEvalue(salp, &tmp, &s, &s1, &s2);
      spip_list[i]->bit_score = tmp;
   }
   HeapSort(spip_list, i, sizeof(SPI_AlnInfoPtr), SPI_compare_aln_score);
   srip_head = NULL;
   srip_head = SPI_AssembleRegions(spip_list, amaip->numsaps, &srip_head, spot);
   srip_head = SPI_SortRegions(srip_head);
   for (i=0; i<amaip->numsaps; i++)
   {
      spip_list[i]->sap = NULL;
      MemFree(spip_list[i]);
      if (i < amaip->numsaps-1)
         amaip->saps[i]->next = amaip->saps[i+1];
   }
   sap->segs = (Pointer)(amaip->saps[0]);
   MemFree(spip_list);
   return srip_head;
}

/***************************************************************************
*
*  SPI_compare_aln_score is the callback for the HeapSort in
*  SPI_FindWindows; it simply compares the scores of two alignments.
*
***************************************************************************/
static int LIBCALLBACK SPI_compare_aln_score(VoidPtr ptr1, VoidPtr ptr2)
{
   SPI_AlnInfoPtr  spip1;
   SPI_AlnInfoPtr  spip2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      spip1 = *((SPI_AlnInfoPtr PNTR)ptr1);
      spip2 = *((SPI_AlnInfoPtr PNTR)ptr2);
      if (spip1->bit_score > spip2->bit_score)
         return -1;
      else if (spip1->bit_score < spip2->bit_score)
         return 1;
      else
         return 0;
   }
   return 0;
}

/***************************************************************************
*
*  SPI_SortRegions takes a linked list of new regions (no alignments) and
*  makes sure that they are in order by score, to ensure that the first
*  region analyzed is the region with the most potential.
*
***************************************************************************/
static SPI_RegionInfoPtr SPI_SortRegions(SPI_RegionInfoPtr srip_head)
{
   Int4               i;
   Int4               j;
   SPI_RegionInfoPtr  srip;
   SPI_RegionInfoPtr  PNTR sriparray;

   srip = srip_head;
   i = 0;
   while (srip != NULL)
   {
      i++;
      srip = srip->next;
   }
   sriparray = (SPI_RegionInfoPtr PNTR)MemNew(i*sizeof(SPI_RegionInfoPtr));
   i = 0;
   srip = srip_head;
   while (srip != NULL)
   {
      sriparray[i] = srip;
      i++;
      srip = srip->next;
   }
   HeapSort(sriparray, i, sizeof(SPI_RegionInfoPtr), SPI_SortSrips);
   for (j=0; j<i-1; j++)
   {
      sriparray[j]->next = sriparray[j+1];
   }
   sriparray[i-1]->next = NULL;
   srip = sriparray[0];
   MemFree(sriparray);
   return srip;
}

/***************************************************************************
*
*  SPI_SortSrips is the HeapSort callback for SPI_SortRegions. It simply
*  orders the regions by score.
*
***************************************************************************/
static int LIBCALLBACK SPI_SortSrips(VoidPtr ptr1, VoidPtr ptr2)
{
   SPI_RegionInfoPtr  srip1;
   SPI_RegionInfoPtr  srip2;

   srip1 = *((SPI_RegionInfoPtr PNTR)ptr1);
   srip2 = *((SPI_RegionInfoPtr PNTR)ptr2);
   if (srip1->score > srip2->score)
      return -1;
   if (srip2->score > srip1->score)
      return 1;
   if (srip1->coverage > srip2->coverage)
      return -1;
   if (srip2->coverage > srip1->coverage)
      return 1;
   return 0;
}

/***************************************************************************
*
*  SPI_AssembleRegions is a recursive function which clusters the
*  alignments into consistent, nonoverlapping windows. On the first pass,
*  all the alignments are sent to the function SPI_GetRegionForSAP, and
*  only the consistent ones are put into the first region. Since the first
*  alignment is the highest-scoring alignment, this first region is
*  usually the best region. On each subsequent pass, an unused alignment
*  is assigned a genomic interval that does not overlap with any other
*  previously defined region, and that alignment and other alignments in
*  the same interval are sent to SPI_GetRegionForSAP to weed out
*  inconsistent alignments. This process is repeated until no alignments
*  are left -- all have either been assigned to a region or designated
*  impossible to assign, since they overlap with a defined region but are
*  inconsistent with other alignments in that region.
*
***************************************************************************/
static SPI_RegionInfoPtr SPI_AssembleRegions(SPI_AlnInfoPtr PNTR spip_list, Int4 num, SPI_RegionInfoPtr PNTR head_srip, SPI_OptionsPtr spot)
{
    /* FloatHi            bit_score; */
    /* FloatHi            evalue; */
   Boolean            found;
   Int4               i;
   Int4               j;
   Int4               lim_left;
   Int4               lim_right;
   Int4               n;
   /* Int4               number; */
   SeqAlignPtr        sap;
   SPI_IvalPtr        siip;
   SPI_IvalPtr        siip_head;
   SPI_IvalPtr        siip_prev;
   SPI_IvalPtr        PNTR siip_list;
   SPI_RegionInfoPtr  srip;
   SPI_RegionInfoPtr  srip_tmp;
   Int4               start;
   Int4               stop;

   if (spip_list == NULL || head_srip == NULL)
      return NULL;
   found = FALSE;
   i = 0;
   while (i<num && !found)
   {
      if (spip_list[i]->used == 0)
         found = TRUE;
      else
         i++;
   }
   if (!found)
      return *head_srip;
   sap = spip_list[i]->sap;
   AlnMgr2GetNthSeqRangeInSA(sap, 1, &start, &stop);
   srip_tmp = *head_srip;
   lim_left = -1;
   lim_right = -1;
   /* figure out what (unused) genomic interval this alignment is in */
   while (srip_tmp != NULL)
   {
      if (srip_tmp->gstop < start)
      {
         if (srip_tmp->gstop > lim_left)
            lim_left = srip_tmp->gstop;
      }
      if (srip_tmp->gstart > stop)
      {
         if (lim_right == -1 || srip_tmp->gstart < lim_right)
            lim_right = srip_tmp->gstart;
      }
      srip_tmp = srip_tmp->next;
   }
   siip_head = siip_prev = NULL;
   n = 0;
   for (j=0; j<num; j++)
   {
      if (spip_list[j]->used == 0)
      {
         AlnMgr2GetNthSeqRangeInSA(spip_list[j]->sap, 1, &start, &stop);
        /* if this unused alignment is in the same interval as the one */
        /* being looked at, put it in the array */
         if (start > lim_left && (stop < lim_right || lim_right == -1))
         {
            siip = (SPI_IvalPtr)MemNew(sizeof(SPI_Ival));
            if (j == i)
               siip->used = 1;
            siip->n = j;
            siip->gstart = start;
            siip->gstop = stop;
            AlnMgr2GetNthSeqRangeInSA(spip_list[j]->sap, 2, &siip->mstart, &siip->mstop);
            siip->strand = AlnMgr2GetNthStrand(spip_list[j]->sap, 2);
            siip->sap = spip_list[j]->sap;
            siip->score = AlnMgr2ComputeScoreForSeqAlign(siip->sap);
            if (siip_head != NULL)
            {
               siip_prev->next = siip;
               siip_prev = siip;
            } else
               siip_head = siip_prev = siip;
            n++;
         }
      }
   }
   siip_list = (SPI_IvalPtr PNTR)MemNew(n*sizeof(SPI_IvalPtr));
   siip = siip_head;
   for (j=0; j<n && siip != NULL; j++)
   {
      siip_list[j] = siip;
      siip = siip->next;
   }
   /* send the array of unused alignments to SPI_GetRegionForSAP to */
   /* weed out inconsistent alignments                              */
   srip = SPI_GetRegionForSAP(siip_list, n, sap, spot);
   if (srip != NULL)
   {
      if (*head_srip == NULL)
         *head_srip = srip;
      else
      {
         srip_tmp = *head_srip;
         while (srip_tmp->next != NULL)
         {
            srip_tmp = srip_tmp->next;
         } 
         srip_tmp->next = srip;
      }
      /* update the information about which alignments have been used */
      /* and which alignments are impossible                          */
      for (j=0; j<n; j++)
      {
         spip_list[siip_list[j]->n]->used = siip_list[j]->used;
         if (((siip_list[j]->gstart > srip->gstart + SPI_FUZZ && siip_list[j]->gstart < srip->gstop - SPI_FUZZ) || (siip_list[j]->gstop > srip->gstart + SPI_FUZZ && siip_list[j]->gstop < srip->gstop - SPI_FUZZ)) && siip_list[j]->used == 0)
         {
            siip_list[j]->used = -1;
            spip_list[siip_list[j]->n]->used = -1;
         }
      }
   }
   for (j=0; j<n; j++)
   {
      MemFree(siip_list[j]);
   }
   MemFree(siip_list);
   /* recursive call to self*/
   srip = SPI_AssembleRegions(spip_list, num, head_srip, spot);
   return srip;
}


/***************************************************************************
*
*  SPI_GetRegionForSAP takes a list of SPI_IvalPtrs, each of which carries
*  the information for one alignment, and a seqalign, which is the
*  "anchor" alignment for the new interval and which is higher-scoring than
*  any other seqalign in the set. The function checks all of the SPI_Ivals
*  to see whether they're consistent with the anchor alignment, and marks
*  each SPI_Ival as used, not used, or impossible.
*
***************************************************************************/
static SPI_RegionInfoPtr SPI_GetRegionForSAP(SPI_IvalPtr PNTR siip_list, Int4 num, SeqAlignPtr sap, SPI_OptionsPtr spot)
{
   Boolean            done;
   Boolean            found;
   Int4               i;
   Int2               j = SPI_UNKNOWN;
   Int4               n;
   SPI_RegionInfoPtr  srip;

   if (siip_list == NULL || num == 0 || sap == NULL)
      return NULL;
   /* sort the alignments along the genomic sequence */
   HeapSort(siip_list, num, sizeof(SPI_IvalPtr), SPI_compare_genomic_loc);
   SPI_CheckMrnaOrder(siip_list, num);
   found = FALSE;
   n = 0;
   /* figure out which one is the anchor alignment */
   while (!found && n<num)
   {
      if (sap == siip_list[n]->sap)
         found = TRUE;
      else
         n++;
   }
   if (!found)
      return NULL;
   /* make a new region with this alignment */
   srip = (SPI_RegionInfoPtr)MemNew(sizeof(SPI_RegionInfo));
   AlnMgr2GetNthSeqRangeInSA(sap, 1, &srip->gstart, &srip->gstop);
   AlnMgr2GetNthSeqRangeInSA(sap, 2, &srip->mstart, &srip->mstop);
   srip->coverage = abs(srip->mstop - srip->mstart) + 1;
   srip->score = siip_list[n]->score;
   srip->strand = AlnMgr2GetNthStrand(sap, 2);
   /* search to the right for consistent alignments */
   done = FALSE;
   for (i=n+1; i<num && !done; i++)
   {
      j = SPI_is_consistent(siip_list[i], srip, spot);
      if (j == SPI_CONSISTENT)
      {
         srip->coverage = abs(siip_list[i]->mstop - siip_list[i]->mstart) + srip->coverage + 1;
         siip_list[i]->used = 1;
         srip->score += siip_list[i]->score;
      } 
      else if (j == SPI_IMPOSSIBLE)
         siip_list[i]->used = -1;
      else if (j == SPI_DONE1)
      {
         siip_list[i]->used = 1;
         done = TRUE;
      } else if (j == SPI_DONE2)
         done = TRUE;
   }
   /* search to the left for consistent alignments */
   done = FALSE;
   for (i=n-1; i>=0 && !done; i--)
   {
      j = SPI_is_consistent(siip_list[i], srip, spot);
      if (j == SPI_CONSISTENT)
      {
         srip->coverage = abs(siip_list[i]->mstop - siip_list[i]->mstart) + srip->coverage + 1;
         siip_list[i]->used = 1;
         srip->score += siip_list[i]->score;
      }
      else if (j == SPI_IMPOSSIBLE)
         siip_list[i]->used = -1;
      else if (j == SPI_DONE1)
      {
         siip_list[i]->used = 1;
         done = TRUE;
      } else if (j == SPI_DONE2)
         done = TRUE;
   }
   SPI_ExcludeOverlaps(siip_list, num, srip);
   return srip;
}

/***************************************************************************
*
*  SPI_is_consistent is the workhorse of SPI_GetRegionForSAP. Given an
*  SPI_Ival and a region, the function decides whether the alignment in
*  the SPI_Ival is consistent with the rest of the region. Alignments that
*  overlap by more than SPI_FUZZ are labeled impossible; alignments that
*  are consistent in both the genomic and mRNA coordinates and that overlap
*  by less than SPI_FUZZ are labeled consistent. Alignments that do not
*  overlap the region but which are not consistent in either the mRNA or
*  genomic coordinates are labeled unknown.
*
***************************************************************************/
static Int2 SPI_is_consistent(SPI_IvalPtr siip, SPI_RegionInfoPtr srip, SPI_OptionsPtr spot)
{
   Int4  intronsize;

   if (siip == NULL || srip == NULL)
      return 0;
   if ((siip->strand == Seq_strand_minus && srip->strand != Seq_strand_minus) || (srip->strand == Seq_strand_minus && siip->strand != Seq_strand_minus))
      return SPI_UNKNOWN;
   /*KSK*/
   if (spot->bigintron){
       intronsize = (spot->bigintron_size > SPI_INTRONSIZEXL 
                     ? spot->bigintron_size : SPI_INTRONSIZEXL);
   }
   else{
       intronsize = SPI_INTRONSIZE;
   }
   /*end KSK*/
   /* first look for overlaps -- exclude these from the set       */
   /* since we search outward from a core hit, there shouldn't be */
   /* any overlaps anyway.                                        */
   if (siip->gstart > srip->gstart + SPI_FUZZ && siip->gstart < srip->gstop - SPI_FUZZ)
      return SPI_IMPOSSIBLE;
   if (siip->gstart > srip->gstop - SPI_FUZZ && siip->gstart < srip->gstop + intronsize)
   {
      if (siip->strand == Seq_strand_minus)
      {
         if (siip->mstop < srip->mstart + SPI_FUZZ)
         {
            srip->gstop = siip->gstop;
            srip->mstart = siip->mstart;
            return SPI_CONSISTENT;
         } else
            return SPI_UNKNOWN;
      } else
      {
         if (siip->mstart > srip->mstop - SPI_FUZZ)
         {
            srip->gstop = siip->gstop;
            srip->mstop = siip->mstop;
            return SPI_CONSISTENT;
         } else
            return SPI_UNKNOWN;
      }
   } else if (siip->gstop <= srip->gstart + SPI_FUZZ && siip->gstop > srip->gstart - intronsize)
   {
      if (siip->strand == Seq_strand_minus)
      {
         if (siip->mstart > srip->mstop - SPI_FUZZ)
         {
            srip->gstart = siip->gstart;
            srip->mstop = siip->mstop;
            return SPI_CONSISTENT;
         } else
            return SPI_UNKNOWN;
      } else
      {
         if (siip->mstop < srip->mstart + SPI_FUZZ)
         {
            srip->gstart = siip->gstart;
            srip->mstart = siip->mstart;
            return SPI_CONSISTENT;
         } else
            return SPI_UNKNOWN;
      }
   }
   return SPI_UNKNOWN;
}

/***************************************************************************
*
*  SPI_compare_genomic_loc is the callback for the HeapSort in 
*  SPI_GetRegionForSAP. It simply orders two SPI_Ival structures
*  by their genomic start coordinates.
*
***************************************************************************/
static int LIBCALLBACK SPI_compare_genomic_loc(VoidPtr ptr1, VoidPtr ptr2)
{
   SPI_IvalPtr  siip1;
   SPI_IvalPtr  siip2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      siip1 = *((SPI_IvalPtr PNTR)ptr1);
      siip2 = *((SPI_IvalPtr PNTR)ptr2);
      if (siip1->gstart < siip2->gstart)
         return -1;
      else if (siip1->gstart > siip2->gstart)
         return 1;
      else
         return 0;
   }
   return 0;
}

/***************************************************************************
*
*  SPI_ExcludeOverlaps takes a completed region and examines all SPI_Ivals
*  for conflicts. Intervals which overlap the completed region are 
*  marked impossible.
*
***************************************************************************/
static void SPI_ExcludeOverlaps(SPI_IvalPtr PNTR siip_list, Int4 num, SPI_RegionInfoPtr srip)
{
   Int4  i;

   for (i=0; i<num; i++)
   {
       if (siip_list[i] == 0) /* KSK changed from '==' to '!=' */
      {
         if ((siip_list[i]->gstart >= srip->gstart && siip_list[i]->gstart <= srip->gstop) || (siip_list[i]->gstop >= srip->gstart && siip_list[i]->gstop <= srip->gstop))
            siip_list[i]->used = -1;
      }
   }
}

/***************************************************************************
*
*  SPI_AlignInWindows organizes the regions into an array, sends each
*  region to SPI_DoAln for careful alignment, and then takes all the
*  regions that have alignments and puts them into a linked list,
*  freeing those regions that do not have alignments.
*
***************************************************************************/
static void SPI_AlignInWindows(SPI_RegionInfoPtr PNTR head_srip, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna, SPI_OptionsPtr spot)
{
   Int4               nalign;
   Int4               score;
   SPI_RegionInfoPtr  srip;
   SPI_RegionInfoPtr  srip_head;
   SPI_RegionInfoPtr  srip_next;
   SPI_RegionInfoPtr  srip_prev;
   SPI_RegionInfoPtr  srip_tmp;

   if (head_srip == NULL || *head_srip == NULL)
      return;
   srip = *head_srip;
   score = srip->coverage;
   nalign = 0;
   /* KSK temp fix was to go through all srips regardless ***
    *  '(while (srip!= NULL)) but the increased time is too much */   
   while (srip != NULL && (nalign < spot->numreturns+1 || srip->coverage >= score/2)){
       SPI_DoAln(srip, bsp_genomic, bsp_mrna, spot);
       if (srip->smp != NULL){
           nalign++;
       }
       srip = srip->next;
   }
   srip_head = srip_prev = NULL;
   /* make a linked list of regions that have alignments */
   srip_tmp = *head_srip;
   while (srip_tmp != NULL)
   {
      srip_next = srip_tmp->next;
      srip_tmp->next = NULL;
      if (srip_tmp->smp != NULL)
      {
         if (srip_head != NULL)
         {
            srip_prev->next = srip_tmp;
            srip_prev = srip_tmp;
         } else
            srip_head = srip_prev = srip_tmp;
      } else
         MemFree(srip_tmp);
      srip_tmp = srip_next;
   }
   *head_srip = srip_head;
}

static void SPI_CheckAlnEnds(SeqAlignPtr sap, Int4Ptr left, Int4Ptr right)
{
   AMFreqPtr  afp;
   Boolean    found;
   Int4       i;
   Int4       j;
   Int4       len;
   Int4       length;
   Int4       mismatch;
  
   if (sap == NULL || sap->saip == NULL)
      return;
   len = AlnMgr2GetAlnLength(sap, FALSE);
   afp = AlnMgr2ComputeFreqMatrix(sap, 0, MIN(len, SPI_MAXBADEXON), 0);
   mismatch = 0;
   *left = *right = 0;
   for (i=0; i<SPI_MINBADEXON/2; i++)
   {
      found = FALSE;
      for (j=0; !found && j<afp->size; j++)
      {
         if (afp->freq[j][i] == 1 && afp->freq[5][i] != 1)
            found = TRUE;
      }
      if (found)
         mismatch++;
   }
   if (mismatch > 0)
      *left = i+1;
   while (i<MIN(afp->len, SPI_MAXBADEXON) && mismatch*100/i >= SPI_BADEXONTHRESH)
   {
      found = FALSE;
      for (j=0; !found && j<afp->size; j++)
      {
         if (afp->freq[j][i] == 1 && afp->freq[5][i] != 1)
            found = TRUE;
      }
      if (found)
         *left = MIN(i+1, len);
      i++;
   }
   afp = AlnMgr2ComputeFreqMatrix(sap, MAX(0, len-1-SPI_MAXBADEXON), len-1, 0);
   length = len-1-(MAX(0, len-1-SPI_MAXBADEXON))+1;
   mismatch = 0;
   for (i=0; i<SPI_MINBADEXON/2; i++)
   {
      found = FALSE;
      for (j=0; !found && j<afp->size; j++)
      {
         if (afp->freq[j][length-1-i] == 1 && afp->freq[5][length-1-i] != 1)
            found = TRUE;
      }
      if (found)
         mismatch++;
   }
   if (mismatch > 0)
      *right = i+1;
   while (i<MIN(length, SPI_MAXBADEXON) && mismatch*100/i >= SPI_BADEXONTHRESH)
   {
      found = FALSE;
      for (j=0; !found && j<afp->size; j++)
      {
         if (afp->freq[j][length-1-i] == 1 && afp->freq[5][length-1-i] != 1)
            found = TRUE;
      }
      if (found)
         *right = MIN(i+1, len);
      i++;
   }
}

static Int4 BufStrStr(Uint1Ptr buf1, Int4 buf1len, Uint1Ptr buf2, Int4 buf2len)
{
   Boolean  found;
   Int4     i;
   Int4     j;
   Int4     mismatch;

   if (buf1 == NULL || buf2 == NULL)
      return -1;
   found = FALSE;
   mismatch = 0;
   for (i=0; i<buf1len-buf2len && !found; i++)
   {
      found = TRUE;
      for (j=0; found && j<buf2len; j++)
      {
         if (buf1[i] != buf2[j])
            mismatch++;
         if (j>2 && mismatch*100/(j+1) > SPI_BADEXONTHRESH)
            found = FALSE;
      }
   }
   if (!found)
      return -1;
   else
      return i;
}

/***************************************************************************
*
*  SPI_PlaceLittleExons looks for small, badly aligned regions on the ends
*  of aligned exons, and tries to align those by themselves as extra-small
*  (teeny) exons.
*
***************************************************************************/
static Boolean SPI_PlaceLittleExons(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   BioseqPtr         bsp1;
   BioseqPtr         bsp2;
   Uint1             buf1[SPI_MAXSEQPORT];
   Uint1             buf2[SPI_MAXBADEXON];
   DenseSegPtr       dsp;
   Boolean           found;
   Int4              i;
   Int4              j;
   Int4              l;
   Int4              lastleft;
   Int4              lastright;
   Int4              left;
   Int4              pos;
   Int4              right;
   SeqAlignPtr       sap_new;
   SeqAlignPtr       sap_new_head;
   SeqAlignPtr       sap_new_prev;
   SeqIdPtr          sip1;
   SeqIdPtr          sip2;
   SeqPortPtr        spp1;
   SeqPortPtr        spp2;
   Int4              start1;
   Int4              start2;
   Int4              stop1;
   Int4              stop2;
   Uint1             strand1;
   Uint2             strand2;

   if (sap == NULL)
      return FALSE;
   sap_new_head = NULL;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   strand1 = AlnMgr2GetNthStrand(amaip->saps[0], 1);
   strand2 = AlnMgr2GetNthStrand(amaip->saps[0], 2);
   lastright = lastleft = 0;
   for (i=0; i<amaip->numsaps; i++)
   {
      SPI_CheckAlnEnds(amaip->saps[i], &right, &left);
      if (strand2 == Seq_strand_minus)
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, NULL, &start2);
         start2 -= right;
      } else
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, &stop2, NULL);
         stop2 += left;
      }
      start1 = stop1 = -1;
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, &stop1, NULL);
      stop1 += left;
      if ((strand2 == Seq_strand_plus && left + lastright >= SPI_TEENYEXON && left+lastright <= SPI_MAXBADEXON) || (strand2 == Seq_strand_minus && right+lastleft >= SPI_TEENYEXON && right+lastleft <= SPI_MAXBADEXON))
      {
         if (i > 0)
         {
            if (strand2 == Seq_strand_minus)
            {
               AlnMgr2GetNthSeqRangeInSA(amaip->saps[i-1], 2, &stop2, NULL);
               stop2 += lastleft;
            } else
            {
               AlnMgr2GetNthSeqRangeInSA(amaip->saps[i-1], 2, NULL, &start2);
               start2 -= lastright;
            }
            AlnMgr2GetNthSeqRangeInSA(amaip->saps[i-1], 1, NULL, &start1);
            start1 -= lastright;
         } else
         {
            if (strand2 == Seq_strand_minus)
               stop2 = 0;
            else
               start2 = 0;
            start1 = 0;
         }
         sip1 = AlnMgr2GetNthSeqIdPtr(amaip->saps[i], 1);
         sip2 = AlnMgr2GetNthSeqIdPtr(amaip->saps[i], 2);
         bsp1 = BioseqLockById(sip1);
         bsp2 = BioseqLockById(sip2);
         spp2 = SeqPortNew(bsp2, start2, stop2, Seq_strand_plus, Seq_code_ncbi4na);
         SeqPortRead(spp2, buf2, stop2-start2+1);
         found = FALSE;
         for (l=0; l<stop1-start1+1 && !found; l+=SPI_MAXSEQPORT)
         {
            spp1 = SeqPortNew(bsp1, start1+l, MIN(stop1, start1+1+SPI_MAXSEQPORT), strand1, Seq_code_ncbi4na);
            SeqPortRead(spp1, buf1, MIN(stop1, start1+1+SPI_MAXSEQPORT) - start1 - 1);
            j = BufStrStr(buf1, MIN(stop1, start1+1+SPI_MAXSEQPORT) - start1 - 1, buf2, stop2-start2+1);
            if (j>0)
               found = TRUE;
         }
         if (found)
            pos = j+l+start1;
         BioseqUnlock(bsp1);
         BioseqUnlock(bsp2);
         if (found)
         {
            dsp = DenseSegNew();
            dsp->dim = 2;
            dsp->numseg = 1;
            dsp->ids = sip1;
            dsp->ids->next = sip2;
            dsp->starts = (Int4Ptr)MemNew(2*sizeof(Int4));
            dsp->starts[0] = pos;
            dsp->starts[1] = start2;
            dsp->lens = (Int4Ptr)MemNew(sizeof(Int4));
            dsp->lens[1] = stop2-start2+1;
            dsp->strands = (Uint1Ptr)MemNew(2*sizeof(Uint1));
            dsp->strands[0] = strand1;
            dsp->strands[1] = Seq_strand_plus;
            sap_new = SeqAlignNew();
            sap_new->segtype = SAS_DENSEG;
            sap_new->segs = (Pointer)dsp;
            if (sap_new_head != NULL)
            {
               sap_new_prev->next = sap_new;
               sap_new_prev = sap_new;
            } else
               sap_new_head = sap_new_prev = sap_new;
         }
      }
      lastright = right;
      lastleft = left;
   }
   /* if there are any new saps, put them in with the others in the right order */
   if (sap_new_head == NULL)
      return FALSE;
   for (i=0; i<amaip->numsaps-1; i++)
   {
      amaip->saps[i]->next = amaip->saps[i+1];
   }
   amaip->saps[amaip->numsaps-1]->next = sap_new_head;
   sap->segs = (Pointer)(amaip->saps[0]);
   AMAlignIndexFreeEitherIndex(sap);
   AlnMgr2IndexLite(sap);
   AlnMgr2SortAlnSetByNthRowPos(sap, 1);
   return TRUE; /* alert that saps have changed */
}

/***************************************************************************
*
*  SPI_DoAln first re-BLASTs the mRNA against the genomic interval
*  specified by the region. If the mRNA is truncated in the interval,
*  the function pads the appropriate side to encourage a complete
*  alignment. The function then calls other functions to remove
*  inconsistent alignments from the set, extend the alignments so that
*  they completely span the mRNA, and adjust the alignments to the
*  most appropriate splice sites.
*
***************************************************************************/
static void SPI_DoAln(SPI_RegionInfoPtr srip, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna, SPI_OptionsPtr spot)
{
   Int4                 eoff;
   BLAST_OptionsBlkPtr  options;
   SPI_Progress         progress;
   SeqAlignPtr          sap;
   SeqLocPtr            slp_g;
   SeqLocPtr            slp_m;
   Int4                 soff;

   if (srip == NULL)
      return;
   if (srip->mstart == 0)
      soff = 0;
   else if (srip->mstart < 50)
      soff = 2*(srip->mstart);
   else
      soff = 3*(srip->mstart);
   if (soff > srip->gstart)
      soff = srip->gstart;
   if (srip->mstop == bsp_mrna->length - 1)
      eoff = 0;
   else if (bsp_mrna->length - 1 - srip->mstop < 50)
      eoff = 2*(bsp_mrna->length - 1 - srip->mstop);
   else
      eoff = 3*(bsp_mrna->length - 1 - srip->mstop);
   if (srip->gstop + eoff > bsp_genomic->length - 1)
       eoff = bsp_genomic->length - 1 - srip->gstop;
   slp_g = SeqLocIntNew(MAX(srip->gstart-soff, spot->from), MIN(srip->gstop+eoff, spot->to), Seq_strand_plus, bsp_genomic->id);
   slp_m = SeqLocIntNew(0, bsp_mrna->length-1, srip->strand, bsp_mrna->id);
   if (slp_g == NULL || slp_m == NULL)
   {
      ErrPostEx(SEV_ERROR, 0, 0, "Error in SPI_DoAln\n");
      return;
   }
   options = BLASTOptionNew("blastn", TRUE);
   options->filter_string = StringSave("m L");
   options->expect_value = spot->secpasseval;
   options->wordsize = 7; /*minimum BLAST wordsize */
   if (spot->interspecies)
   {
      options->gap_x_dropoff_final = 100;
      options->gap_open = 5;
      options->gap_extend = 1;
      options->penalty = -1;
   }
   options->query_lcase_mask = spot->lcaseloc;
   /* use mRNA as the query to speed up BLAST */
   sap = BlastTwoSequencesByLoc(slp_m, slp_g, "blastn", options);
   if (spot->callback != NULL)
   {
      progress.percentdone = 50;
      progress.returncode = SPI_PROGRESS;
      if (!spot->callback(&progress))
         return;
   }
   SeqLocFree(slp_m);
   SeqLocFree(slp_g);
   BLASTOptionDelete(options);
   if (sap == NULL)
   {
      ErrPostEx(SEV_ERROR, 0, 0, "Error in SPI_DoAln\n");
      return;
   }
   
   if (!AlnMgr2IndexLite(sap))
      return;
   /* flip alignments so genomic sequence is the first row */
   SPI_flip_sa_list((SeqAlignPtr)(sap->segs));
   /* remove alignments that overlap by more than 2*SPI_TEENYEXON or that */
   /* are not consistent along genomic or mRNA coordinates         */
   SPI_RemoveInconsistentAlnsFromSet(sap, 2*SPI_TEENYEXON, 1, SPI_LEFT);
   if (spot->interspecies == FALSE) /* extend to both ends of mRNA */
   {
      if (!SPI_ConnectAln(sap, spot, srip, TRUE, TRUE))
         return;
   } else /* for interspecies alignments, don't try to extend to the ends */
   {
      if (!SPI_ConnectAln(sap, spot, srip, FALSE, TRUE))
         return;
   }
   if (spot->callback != NULL)
   {
      progress.percentdone = 75;
      progress.returncode = SPI_PROGRESS;
      if (!spot->callback(&progress))
         return;
   }
   srip->smp = SPI_AdjustForSplice(sap, spot, srip);
   /*if (SPI_PlaceLittleExons(sap))
      srip->smp = SPI_AdjustForSplice(sap, spot, srip);*/
}

/***************************************************************************
*
*  SPI_CheckForPolyAExon looks at the 3' terminal exon and checks to see
*  whether it consists only of polyAs. If so, the exon is deleted.
*
***************************************************************************/
static void SPI_CheckForPolyAExon(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   BioseqPtr         bsp;
   Int4              i;
   Int4              len;
   Int4              polya;
   SeqAlignPtr       salp;
   SeqAlignPtr       salp_prev;
   SeqAlignPtr       sap_target;
   SeqIdPtr          sip;
   Int4              start;
   Int4              stop;
   Uint1             strand;

   amaip = (AMAlignIndex2Ptr)(sap->saip);
   for (i=0; i<amaip->numsaps-1; i++)
   {
      amaip->saps[i]->next = amaip->saps[i+1];
   }
   amaip->saps[i]->next = NULL;
   strand = AlnMgr2GetNthStrand(amaip->saps[0], 2);
   sip = AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 2);
   bsp = BioseqLockById(sip);
   len = bsp->length;
   BioseqUnlock(bsp);
   if (strand == Seq_strand_minus)
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 2, &start, &stop);
   else
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[amaip->numsaps-1], 2, &start, &stop);
   polya = SPI_IsItPolyA(sip);
   SeqIdFree(sip);
   if (len - start > polya)
      return;
   if (amaip->numsaps == 1)
      return;
   if (strand == Seq_strand_minus)
      sap_target = amaip->saps[0];
   else
      sap_target = amaip->saps[amaip->numsaps-1];
   salp = (SeqAlignPtr)(sap->segs);
   salp_prev = NULL;
   while (salp != NULL){
       if (salp == sap_target){
           if (salp_prev == NULL){
               sap->segs = (Pointer)(sap_target->next);
           }
           else {
               salp_prev->next = sap_target->next;
           }
           SeqAlignFree(sap_target);
           sap_target = 0;
           salp = 0;
       }
       else {
           salp_prev = salp;
           salp = salp->next;
       }
   }
   AMAlignIndexFreeEitherIndex(sap);
   AlnMgr2IndexLite(sap);
   AlnMgr2SortAlnSetByNthRowPos(sap, 1);
}

/***************************************************************************
*
*  SPI_ConnectAln looks through all the alignments in a set and fills in
*  the gaps on the mRNA sequence. If do_ends is TRUE, the function will
*  try to fill in the alignments until they extend to both ends of the 
*  mRNA; otherwise the function only fills in internal gaps. If a gap is
*  greater than the size of the smallest possible exon (SPI_TEENYEXON),
*  the function checks to see whether there's also a gap in the genomic
*  sequence. If both sequences have different-sized gaps bigger than 
*  SPI_FUZZ, the function calls SPI_FillInIntron to fill in the gaps. 
*  If both sequences have gaps larger than SPI_FUZZ and of similar sizes, 
*  the function calls Fasika Aklilu's tree-based alignment
*  functions via SPI_FindBestAlnByDotPlot. SPI_ConnectAln does
*  check to see whether a non-matching mRNA 3' tail is actually a poly(A)
*  tail; if so, the tail is left unaligned.
*
***************************************************************************/
static Boolean SPI_ConnectAln(SeqAlignPtr sap, SPI_OptionsPtr spot, SPI_RegionInfoPtr srip, Boolean do_ends, Boolean firsttime)
{
   AMAlignIndex2Ptr  amaip;
   BioseqPtr        bsp1;
   BioseqPtr        bsp2;
   Int4             currstart2;
   Int4             end2;
   Int4             gap1;
   Int4             gap2;
   Int4             i;
   Boolean          internal;
   Int4             j;
   Int4             len1;
   Int4             len2;
   SeqAlignPtr      newsaps;
   SeqAlignPtr      newsaps_prev;
   Int4             prevstop1;
   Int4             prevstop2;
   SeqAlignPtr      sap_new;
   SeqAlignPtr      sap_prev;
   SeqAlignPtr      sap_tmp;
   SeqIdPtr         sip1;
   SeqIdPtr         sip2;
   SeqLocPtr        slp1;
   SeqLocPtr        slp2;
   Int4             start1;
   Int4             start2;
   Int4             stop1;
   Int4             stop2;
   Uint1            strand1;
   Uint1            strand2;
   Int4             tail;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return FALSE;
   AlnMgr2SortAlnSetByNthRowPos(sap, 1);
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   if (amaip->numsaps < 1)
      return FALSE;
   if (amaip->numsaps == 1 && !do_ends)
      return TRUE;
   sip1 = AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 1);
   sip2 = AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 2);
   bsp1 = BioseqLockById(sip1);
   if (bsp1 == NULL)
      return FALSE;
   bsp2 = BioseqLockById(sip2);
   if (bsp2 == NULL)
   {
      BioseqUnlock(bsp1);
      return FALSE;
   }
   strand1 = AlnMgr2GetNthStrand(amaip->saps[0], 1);
   strand2 = AlnMgr2GetNthStrand(amaip->saps[0], 2);
   if (do_ends)
   {
      prevstop1 = prevstop2 = -1;
      if (strand2 == Seq_strand_minus)
         prevstop2 = bsp2->length-1;
   } else
   {
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 1, &prevstop1, NULL);
      prevstop1--;
      if (strand2 == Seq_strand_minus)
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 2, NULL, &prevstop2);
         prevstop2++;
      } else
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 2, &prevstop2, NULL);
         prevstop2--;
      }
   }
   internal = FALSE;
   newsaps = newsaps_prev = NULL;
   for (i=0; i<amaip->numsaps; i++)
   {
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, &start1, &stop1);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, &start2, &stop2);
      /* make the mRNA a little 'smaller' to force overlaps between adjacent exons */
      start2 = start2 + 2;
      stop2 = stop2 - 2;
      if (strand2 == Seq_strand_minus)
         currstart2 = stop2;
      else
         currstart2 = start2;
      if ((gap2 = spi_isa_gap(currstart2, prevstop2, strand2)) > SPI_TEENYEXON)
      {
         gap1 = spi_isa_gap(start1, prevstop1, strand1);
         if (gap1 < (gap2 - 2*SPI_FUZZ) && gap1 > SPI_FUZZ)
         {
            if (internal == TRUE && i<amaip->numsaps-1 && spot->interspecies == FALSE)
            {
               /* gap in mRNA but not genomic - possibly not the right region */
               len1 = AlnMgr2GetAlnLength(amaip->saps[i-1], FALSE);
               len2 = AlnMgr2GetAlnLength(amaip->saps[i], FALSE);
               if (len1 > len2)
               {
                  SeqAlignFree(amaip->saps[i]);
                  amaip->saps[i] = NULL;
                  j = i+1;
                  /* reset the boundaries */
                  stop1 = prevstop1;
                  start2 = stop2 = prevstop2;
               } else
               {
                  SeqAlignFree(amaip->saps[i-1]);
                  amaip->saps[i-1] = NULL;
                  j = i;
               }
               for ( ; j < amaip->numsaps; j++)
               {
                  amaip->saps[j-1] = amaip->saps[j];
                  amaip->saps[j] = NULL;
               }
               for (j=0; j<amaip->numsaps-1; j++)
               {
                  amaip->saps[j]->next  = amaip->saps[j+1];
               }
               amaip->numsaps--;
               amaip->saps[amaip->numsaps-1]->next = NULL;
               i--;
            } else if (internal == FALSE)    /* mRNA continues past beginning of genomic sequence */
            {
               sap_new = SPI_FindPiece(sip1, sip2, prevstop2, currstart2, strand2, start1, SPI_LEFT, spot);
               sap_new = SPI_ProcessNewAlns(sap_new);
               if (sap_new != NULL && newsaps == NULL)
                  newsaps = newsaps_prev = sap_new;
               else if (sap_new != NULL)
                  newsaps_prev->next = sap_new;
               while (sap_new != NULL && newsaps_prev->next != NULL)
               {
                  newsaps_prev = newsaps_prev->next;
               }
            }
         } else if ((gap2-2*SPI_FUZZ <= gap1) && (gap1 <= gap2 + 2*SPI_FUZZ) && gap1 > SPI_FUZZ) /* same size gaps */
         {
            slp1 = SeqLocIntNew(prevstop1+1, start1-1, strand1, sip1);
            if (strand2 != Seq_strand_minus)
               slp2 = SeqLocIntNew(prevstop2+1, currstart2-1, strand2, sip2);
            else
               slp2 = SeqLocIntNew(currstart2+1, prevstop2-1, strand2, sip2);
            sap_new = SPI_FindBestAlnByDotPlot(slp1, slp2);
            sap_new = SPI_ProcessNewAlns(sap_new);
            SeqLocFree(slp1);
            SeqLocFree(slp2);
            if (sap_new != NULL && newsaps == NULL)
               newsaps = newsaps_prev = sap_new;
            else if (sap_new != NULL)
               newsaps_prev->next = sap_new;
            while (sap_new != NULL && newsaps_prev->next != NULL)
            {
               newsaps_prev = newsaps_prev->next;
            }
         } else if (gap1 >= SPI_FUZZ && gap2 >= SPI_FUZZ) /* gaps are different sizes -- intron? */
         {
            if (internal)
            {
               sap_new = SPI_FillInIntron(sip1, sip2, prevstop1, start1, prevstop2, currstart2, strand2, spot);
               sap_new = SPI_ProcessNewAlns(sap_new);
               if (sap_new != NULL && newsaps == NULL)
                  newsaps = newsaps_prev = sap_new;
               else if (sap_new != NULL)
                  newsaps_prev->next = sap_new;
               while (sap_new != NULL && newsaps_prev->next != NULL)
               {
                  newsaps_prev = newsaps_prev->next;
               }
            } else /* first or last part of mRNA hasn't matched */
            {
               if (strand2 == Seq_strand_minus && do_ends) /* last exon -- check for polyA */
               {
                  srip->polyAtail = SPI_IsItPolyA(sip2);
                  if (srip->polyAtail >= SPI_MINPOLYASIZE) 
                  {
                     if (srip->polyAtail < prevstop2)
                        prevstop2 = prevstop2 - srip->polyAtail; /* don't align tail */
                     else
                     {
                        if (prevstop2 >= SPI_MINPOLYASIZE)
                        {
                           srip->polyAtail = prevstop2;
                           prevstop2 = 0;
                        } else
                           srip->polyAtail = 0;
                     }
                  } else
                     srip->polyAtail = 0;
               }
               sap_new = SPI_FindPiece(sip1, sip2, prevstop2, currstart2, strand2, start1, SPI_LEFT, spot);
               sap_new = SPI_ProcessNewAlns(sap_new);
               if (sap_new != NULL && newsaps == NULL)
                  newsaps = newsaps_prev = sap_new;
               else if (sap_new != NULL)
                  newsaps_prev->next = sap_new;
               while (sap_new != NULL && newsaps_prev->next != NULL)
               {
                  newsaps_prev = newsaps_prev->next;
               }
            }
         }
      }
      internal = TRUE;
      prevstop1 = stop1;
      if (strand2 == Seq_strand_minus)
         prevstop2 = start2;
      else
         prevstop2 = stop2;
   }
   sap_tmp = amaip->saps[0];
   for (j=1; j<amaip->numsaps; j++)
   {
      sap_tmp->next = amaip->saps[j];
      sap_tmp = amaip->saps[j];
   }
   sap_tmp->next = newsaps;
   sap->segs = (Pointer)(amaip->saps[0]);
   if (do_ends)
   {
      if (strand2 != Seq_strand_minus) /* last exon -- check for polyA */
      {
         srip->polyAtail = SPI_IsItPolyA(sip2);
         if (srip->polyAtail >= SPI_MINPOLYASIZE)
            end2 = bsp2->length - 1 - srip->polyAtail;
         else
         {
            srip->polyAtail = 0;
            end2 = bsp2->length-1;
         }
         if (srip->polyAtail != 0 && prevstop2 > end2)
            srip->polyAtail = srip->polyAtail - (prevstop2-end2+1);
         if (srip->polyAtail < SPI_MINPOLYASIZE)
            srip->polyAtail = 0;
      } else
         end2 = -1;
      if (spi_isa_gap(end2, prevstop2, strand2))
      {
         sap_new = SPI_FindPiece(sip1, sip2, prevstop2, end2, strand2, prevstop1, SPI_RIGHT, spot);
         sap_new = SPI_ProcessNewAlns(sap_new);
         sap_tmp = (SeqAlignPtr)(sap->segs);
         while (sap_tmp != NULL && sap_tmp->next != NULL)
         {
            sap_tmp = sap_tmp->next;
         }
         sap_tmp->next = sap_new;
      }
   }
   sap_tmp = (SeqAlignPtr)(sap->segs);
   /* check whether last exon is all polya */
   tail = SPI_IsItPolyA(sip2);
   if (strand2 == Seq_strand_minus)
   {
      AlnMgr2GetNthSeqRangeInSA(sap_tmp, 2, &start2, &stop2);
      if (stop2 < SPI_TEENYEXON + tail)
      {
         sap->segs = (Pointer)(sap_tmp->next);
         sap_tmp->next = NULL;
         SeqAlignFree(sap_tmp);
      }
   } else
   {
      sap_prev = NULL;
      while (sap_tmp->next != NULL)
      {
         sap_prev = sap_tmp;
         sap_tmp = sap_tmp->next;
      }
      AlnMgr2GetNthSeqRangeInSA(sap_tmp, 2, &start2, &stop2);
      if (bsp2->length - 1 - start2 < SPI_TEENYEXON + tail)
      {
         if (sap_prev != NULL)
            sap_prev->next = NULL;
         SeqAlignFree(sap_tmp);
         if (sap_prev == NULL)
            sap->segs = NULL;
      }
   }
   sap_tmp = (SeqAlignPtr)(sap->segs);
   i = 0;
   while (sap_tmp != NULL)
   {
      i++;
      sap_tmp = sap_tmp->next;
   }
   amaip->numsaps = i;
   MemFree(amaip->saps);
   amaip->saps = (SeqAlignPtr PNTR)MemNew(i*sizeof(SeqAlignPtr));
   sap_tmp = (SeqAlignPtr)(sap->segs);
   i = 0;
   while (sap_tmp != NULL)
   {
      amaip->saps[i] = sap_tmp;
      i++;
      sap_tmp = sap_tmp->next;
   }
   BioseqUnlock (bsp1);
   BioseqUnlock (bsp2);
   SeqIdFree(sip1);
   SeqIdFree(sip2);
   if (firsttime) /* reconnect to pick up last pieces */
      SPI_ConnectAln(sap, spot, srip, do_ends, FALSE);
   AlnMgr2SortAlnSetByNthRowPos(sap, 1);
   SPI_CheckForPolyAExon(sap);
   return TRUE;
}

/***************************************************************************
*
*  SPI_ProcessNewAlns takes a linked list of child-type alignments and
*  runs SPI_RemoveInconsistentAlnsFromSet on a "fake" parent alignment
*  that it temporarily attaches to the children.
*
***************************************************************************/
static SeqAlignPtr SPI_ProcessNewAlns(SeqAlignPtr sap)
{
   SeqAlignPtr  sap_head;

   if (sap == NULL)
      return NULL;
   sap_head = SeqAlignNew();
   sap_head->segtype = SAS_DISC;
   sap_head->segs = (Pointer)sap;
   AlnMgr2IndexLite(sap_head);
   SPI_RemoveInconsistentAlnsFromSet(sap_head, SPI_TEENYEXON, 1, SPI_LEFT);
   sap = (SeqAlignPtr)(sap_head->segs);
   sap_head->segs = NULL;
   MemFree(sap_head);
   return sap;
}

/***************************************************************************
*
*  SPI_IsItPolyA is a utility function which returns the length of
*  the poly(A) tail of a sequence. The poly(A) tail must be at least
*  SPI_MINPOLYASIZE long and can have non-A residues up to the SPI_LINKERSIZE
*  position from the end (linker used to clone the cDNA may sometimes be left
*  on the end of the poly(A) tail).
*
***************************************************************************/
static Int4 SPI_IsItPolyA(SeqIdPtr sip)
{
   BioseqPtr   bsp;
   Uint1       buf[SPI_MAXPOLYASIZE];
   Int4        count;
   Boolean     done;
   Int4        i;
   Int4        j;
   Uint1       res;
   SeqPortPtr  spp;
   Int4        start;
   
   bsp = BioseqLockById(sip);
   spp = SeqPortNew (bsp, bsp->length - SPI_MAXPOLYASIZE, bsp->length-1, Seq_strand_minus, Seq_code_ncbi4na);
   /* port on the minus strand -- shows up earlier if it's not polyA */
   done = FALSE;
   i = 0;
   j = 0;
   start = 0;
   count = 0;
   SeqPortRead(spp, buf, SPI_MAXPOLYASIZE);
   while (((res = buf[j]) != SEQPORT_EOF) && !done)
   {
      if (res != 8)
      {
         if (count >= SPI_LINKERSIZE) /* can have non-A bases up to the SPI_LINKERSIZE position */
            done = TRUE;
         else
            start++;
      } else
         i++;
      count++;
      j++;
   }
   if (start > 0)
   {
      if (i-start < SPI_MINPOLYASIZE) /* "tail" is too short */
      {
         SeqPortFree(spp);
         return 0;
      }
   }
   SeqPortFree(spp);
   BioseqUnlock(bsp);
   return i;
}

/***************************************************************************
*
*  SPI_FillInIntron is able to fill in internal gaps for SPI_ConnectAln.
*  Given mRNA and genomic boundaries, SPI_FillInIntron first does a low-
*  stringency BLAST, then removes inconsistent and overlapping alignments
*  from the resulting alignment set, and finally calls
*  SPI_FillInLastmRNAHoles to internally connect the new alignments.
*
***************************************************************************/
static SeqAlignPtr SPI_FillInIntron(SeqIdPtr sip1, SeqIdPtr sip2, Int4 start1, Int4 stop1, Int4 start2, Int4 stop2, Uint1 strand2, SPI_OptionsPtr spot)
{
   BLAST_OptionsBlkPtr  options;
   SeqAlignPtr          sap;
   SeqLocPtr            slp1;
   SeqLocPtr            slp2;
   Int4                 start;
   Int4                 stop;

   if (stop1 - start1 < SPI_MINBLASTSIZE)
      return NULL;
   slp1 = SeqLocIntNew(start1, stop1, Seq_strand_plus, sip1);
   if (strand2 == Seq_strand_minus)
   {
      start = stop2;
      stop = start2;
   } else
   {
      start = start2;
      stop = stop2;
   }
   if (stop - start < SPI_MINBLASTSIZE)
   {
      SeqLocFree(slp1);
      return NULL;
   }
   slp2 = SeqLocIntNew(start, stop, strand2, sip2);
   options = BLASTOptionNew("blastn", FALSE);
   options->filter_string = StringSave("m L");
   options->expect_value = spot->thirdpasseval;
   options->query_lcase_mask = spot->lcaseloc;
   options->wordsize = 7;
   if (spot->interspecies)
   {
      options->gap_x_dropoff_final = 100;
      options->gap_open = 5;
      options->gap_extend = 1;
      options->penalty = -1;
   }
   sap = BlastTwoSequencesByLoc(slp2, slp1, "blastn", options);
  

   SeqLocFree(slp1);
   SeqLocFree(slp2);
   AlnMgr2IndexLite(sap);
   if (sap != NULL)
   {
      SPI_flip_sa_list((SeqAlignPtr)(sap->segs));
      AMAlignIndex2Free2(sap->saip);
      sap->saip = NULL;
   }
   AlnMgr2IndexLite(sap);
   SPI_RemoveInconsistentAlnsFromSet(sap, SPI_TEENYEXON/2, 1, SPI_LEFT);
   BLASTOptionDelete(options);
   sap = SPI_FillInLastmRNAHoles(sap, sip1, sip2, start1, stop1, start, stop, strand2);
   return sap;
}

/***************************************************************************
*
*  spi_isa_gap is an often-called utility function that returns the size
*  of the difference between two sequence positions, given the strand. If
*  the sequence positions overlap or abut exactly, the function returns
*  0. The strand is important:
*
*  13---------40   50--------60 plus strand--> gap of 8
*              |    |
*          prevstop start
*              |    |
*  60---------50   40-------13 minus strand--> gap of 8
*
***************************************************************************/
static Int4 spi_isa_gap(Int4 start, Int4 prevstop, Uint1 strand)
{
   if (prevstop == -1)
      prevstop = 0;
   if (start == -1)
      start = 0;
   if (strand != Seq_strand_minus)
   {
      if (start > prevstop+1)
         return (start - prevstop+1);
      else
         return 0;
   } else
   {
      if (prevstop > start+1)
         return (prevstop - (start+1));
      else
         return 0;
   }
}

/***************************************************************************
*
*  SPI_GetNthSeqLenInSASet assumes that the alignment given is a
*  set of alignments that all have the same rows. The function returns
*  the length of the Nth row that is covered by the alignment set.
*
***************************************************************************/
static Int4 SPI_GetNthSeqLenInSASet(SeqAlignPtr sap, Int4 n, Int4Ptr numsaps)
{
   Int4         len;
   Int4         num;
   SeqAlignPtr  salp;
   Int4         start_tmp;
   Int4         stop_tmp;
   
   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return -1;
   salp = (SeqAlignPtr)(sap->segs);
   start_tmp = stop_tmp = -1;
   len = 0;
   num = 0;
   while (salp != NULL)
   {
      num++;
      if (n > salp->dim)
         return 0;
      AlnMgr2GetNthSeqRangeInSA(salp, n, &start_tmp, &stop_tmp);
      len += (stop_tmp - start_tmp + 1);
      salp = salp->next;
   }
   if (numsaps)
      *numsaps = num;
   return len;
}

/***************************************************************************
*
*  SPI_GetNthSeqRangeInSASet is used to get the 5' and 3' boundaries
*  of a sequence across a set of alignments. 'N' refers to row number,
*  and all the alignments are assumed to have the same rows. Note that
*  this function says nothing about the coverage of the specified
*  sequence.
*
***************************************************************************/
static void SPI_GetNthSeqRangeInSASet(SeqAlignPtr sap, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   Int4         numrows;
   SeqAlignPtr  salp;
   Int4         start_tmp;
   Int4         stop_tmp;
   Int4         tmp1;
   Int4         tmp2;
   
   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   salp = (SeqAlignPtr)(sap->segs);
   start_tmp = stop_tmp = -1;
   while (salp != NULL)
   {
      numrows = AlnMgr2GetNumRows(salp);
      if (n > numrows)
      {
         if (start)
            *start = -1;
         if (stop)
            *stop = -1;
         return;
      }
      AlnMgr2GetNthSeqRangeInSA(salp, n, &tmp1, &tmp2);
      if (tmp1 < start_tmp || start_tmp == -1)
         start_tmp = tmp1;
      if (tmp2 > stop_tmp)
         stop_tmp = tmp2;
      salp = salp->next;
   }
   if (start)
      *start = start_tmp;
   if (stop)
      *stop = stop_tmp;
}

/***************************************************************************
*
*  SPI_FindPiece is used to align a piece of mRNA with a tail of genomic
*  sequence:
*  which_end            start_g               start_m-stop_m
*   SPI_LEFT    0<-------4000  genomic           X 30-61 mRNA (plus strand)
*   SPI_RIGHT            7000----->3'end genomic X 79-40 mRNA (minus strand)
*
*  SPI_FindPiece first does a low-stringency BLAST search to try to align
*  the desired piece, and then calls functions to fill out the alignment
*  so that the new alignment is well-connected with the other pieces and
*  internally complete.
*  Since the strands and orientation are constrained by the other
*  alignments in the set, the BLAST search is only done within the 
*  specified boundaries and for the specified strand.
*
***************************************************************************/
static SeqAlignPtr SPI_FindPiece(SeqIdPtr sip1, SeqIdPtr sip2, Int4 start_m, Int4 stop_m, Uint1 strand, Int4 start_g, Int2 which_end, SPI_OptionsPtr spot)
{
   Int4                 bigintron;
   BioseqPtr            bsp1;
   Int4                 gstart;
   Int4                 gstop;
   Int4                 mstart;
   Int4                 mstop;
   BLAST_OptionsBlkPtr  options;
   SeqAlignPtr          sap;
   SeqAlignPtr          sap_new;
   SeqAlignPtr          sap_new2;
   SeqLocPtr            slp1;
   SeqLocPtr            slp2;
   Int4                 start;
   Int4                 stop;

   if (sip1 == NULL || sip2 == NULL)
      return NULL;
   /*KSK*/
   if (spot->bigintron){
       bigintron = MAX(SPI_BIGINTRONXL, spot->bigintron_size);
   }
   else {
       bigintron = SPI_BIGINTRON;
   }
   if ((strand == Seq_strand_minus && start_m - stop_m < 7) || (strand != Seq_strand_minus && stop_m - start_m < 7))
      return NULL;
   if (start_m < 0)
      start_m = 0;
   if (start_m == -1)
      start_m = 0;
   if (stop_m == -1)
      stop_m = 0;
   if (start_g == -1)
      start_g = 0;
   if (which_end == SPI_LEFT)
   {
      if (start_g < SPI_FUZZ)
         return NULL;
      if (strand == Seq_strand_minus)
      {
         if (start_m - stop_m > start_g + 2*SPI_FUZZ)
            return NULL;
         start = MAX(0, start_g - bigintron);
         slp1 = SeqLocIntNew(MAX(start, spot->from), MIN(start_g, spot->to), Seq_strand_plus, sip1);
         slp2 = SeqLocIntNew(stop_m, start_m, strand, sip2);
         options = BLASTOptionNew("blastn", FALSE);
         options->wordsize = 7;
         options->filter_string = StringSave("m L");
         options->expect_value = spot->secpasseval;
         options->query_lcase_mask = spot->lcaseloc;
         if (spot->interspecies)
         {
            options->gap_x_dropoff_final = 100;
            options->gap_open = 5;
            options->gap_extend = 1;
            options->penalty = -1;
         }
         sap = BlastTwoSequencesByLoc(slp2, slp1, "blastn", options);
         if (sap == NULL)
            return NULL;
         AlnMgr2IndexLite(sap);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
         SPI_flip_sa_list((SeqAlignPtr)(sap->segs));
         AMAlignIndex2Free2(sap->saip);
         sap->saip = NULL;
         AlnMgr2IndexLite(sap);
         SPI_RemoveInconsistentAlnsFromSet(sap, SPI_TEENYEXON/2, 1, SPI_RIGHT);
         SPI_GetNthSeqRangeInSASet(sap, 1, &gstart, &gstop);
         SPI_GetNthSeqRangeInSASet(sap, 2, &mstart, &mstop);
         BLASTOptionDelete(options);
         if (spot->draftfile != NULL)
            return sap;
         if (mstart - stop_m <= 3*SPI_FUZZ)
         {
            sap_new = sap;
            if (sap_new->segtype == SAS_DISC)
            {
               sap_new2 = (SeqAlignPtr)(sap_new->segs);
               sap_new->segs = NULL;
               SeqAlignFree(sap_new);
               sap_new = sap_new2;
               while (sap_new2 != NULL)
               {
                  AlnMgr2IndexSingleChildSeqAlign(sap_new2);
                  sap_new2 = sap_new2->next;
               }
            } else
               SPI_AddToAln(sap_new, mstart - stop_m, SPI_RIGHT, strand);
            return sap_new;
         } else
         {
            sap_new = SPI_FillInLastmRNAHoles(sap, sip1, sip2, gstart, start_g-1, stop_m+1, mstop, strand);
            return sap_new;
         }
      } else
      {
         if (stop_m - start_m > start_g + 2*SPI_FUZZ)
            return NULL;
         start = MAX(0, start_g - bigintron);
         slp1 = SeqLocIntNew(MAX(start, spot->from), MIN(start_g, spot->to), Seq_strand_plus, sip1);
         slp2 = SeqLocIntNew(start_m, stop_m, strand, sip2);
         options = BLASTOptionNew("blastn", FALSE);
         options->wordsize = 7;
         options->filter_string = StringSave("m L");
         options->expect_value = spot->secpasseval;
         options->query_lcase_mask = spot->lcaseloc;
         if (spot->interspecies)
         {
            options->gap_x_dropoff_final = 100;
            options->gap_open = 5;
            options->gap_extend = 1;
            options->penalty = -1;
         }
         sap = BlastTwoSequencesByLoc(slp2, slp1, "blastn", options);
         if (sap == NULL)
            return NULL;
         AlnMgr2IndexLite(sap);
         SPI_flip_sa_list((SeqAlignPtr)(sap->segs));
         AMAlignIndex2Free2(sap->saip);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
         sap->saip = NULL;
         AlnMgr2IndexLite(sap);
         SPI_RemoveInconsistentAlnsFromSet(sap, SPI_TEENYEXON/2, 1, SPI_RIGHT);
         SPI_GetNthSeqRangeInSASet(sap, 1, &gstart, &gstop);
         SPI_GetNthSeqRangeInSASet(sap, 2, &mstart, &mstop);
         BLASTOptionDelete(options);
         if (spot->draftfile != NULL)
            return sap;         
         if (stop_m - mstop <= 3*SPI_FUZZ)
         {
            sap_new = sap;
            if (sap_new->segtype == SAS_DISC)
            {
               sap_new2 = (SeqAlignPtr)(sap_new->segs);
               sap_new->segs = NULL;
               SeqAlignFree(sap_new);
               sap_new = sap_new2;
               while (sap_new2 != NULL)
               {
                  AlnMgr2IndexSingleChildSeqAlign(sap_new2); 
                  sap_new2 = sap_new2->next;
               }
            } else
               SPI_AddToAln(sap_new, stop_m - mstop, SPI_RIGHT, strand);
            return sap_new;
         } else
         {
            sap_new = SPI_FillInLastmRNAHoles(sap, sip1, sip2, gstart, start_g-1, mstart, stop_m+1, strand);
            return sap_new;
         }
      }
   } else if (which_end == SPI_RIGHT)
   {
      bsp1 = BioseqLockById(sip1);
      if (bsp1 == NULL)
         return NULL;
      if (start_g > bsp1->length - SPI_FUZZ)
         return NULL;
      if (strand == Seq_strand_minus)
      {
         if (start_m - stop_m > bsp1->length - start_g - 2*SPI_FUZZ)
            return NULL;
         stop = MIN(bsp1->length-1, start_g + bigintron);
         slp1 = SeqLocIntNew(MAX(start_g, spot->from), MIN(stop, spot->to), Seq_strand_plus, sip1);
         slp2 = SeqLocIntNew(stop_m, start_m, strand, sip2);
         options = BLASTOptionNew("blastn", FALSE);
         options->wordsize = 7;
         options->filter_string = StringSave("m L");
         options->expect_value = spot->secpasseval;
         options->query_lcase_mask = spot->lcaseloc;
         if (spot->interspecies)
         {
            options->gap_x_dropoff_final = 100;
            options->gap_open = 5;
            options->gap_extend = 1;
            options->penalty = -1;
         }
         sap = BlastTwoSequencesByLoc(slp2, slp1, "blastn", options);
         if (sap == NULL)
            return NULL;
         AlnMgr2IndexLite(sap);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
         SPI_flip_sa_list((SeqAlignPtr)(sap->segs));
         AMAlignIndex2Free2(sap->saip);
         sap->saip = NULL;
         AlnMgr2IndexLite(sap);
         SPI_RemoveInconsistentAlnsFromSet(sap, SPI_TEENYEXON/2, 1, SPI_LEFT);
         SPI_GetNthSeqRangeInSASet(sap, 1, &gstart, &gstop);
         SPI_GetNthSeqRangeInSASet(sap, 2, &mstart, &mstop);
         BLASTOptionDelete(options);
         if (spot->draftfile != NULL)
            return sap;
         if (start_m - mstop <= 3*SPI_FUZZ)
         {
            sap_new = sap;
            if (sap_new->segtype == SAS_DISC)
            {
               sap_new2 = (SeqAlignPtr)(sap_new->segs);
               sap_new->segs = NULL;
               SeqAlignFree(sap_new);
               sap_new = sap_new2;
               while (sap_new2 != NULL)
               {
                  AlnMgr2IndexSingleChildSeqAlign(sap_new2); 
                  sap_new2 = sap_new2->next;
               }
            } else
               SPI_AddToAln(sap_new, start_m - mstop, SPI_LEFT, strand);
            return sap_new;
         } else
         {
            sap_new = SPI_FillInLastmRNAHoles(sap, sip1, sip2, start_g+1, gstop, mstart, start_m-1, strand);
            return sap_new;
         }
      } else
      {
         if (stop_m - start_m > bsp1->length - start_g - 2*SPI_FUZZ)
            return NULL;
         stop = MIN(bsp1->length-1, start_g + bigintron);
         slp1 = SeqLocIntNew(MAX(start_g, spot->from), MIN(stop, spot->to), Seq_strand_plus, sip1);
         slp2 = SeqLocIntNew(start_m, stop_m, strand, sip2);
         options = BLASTOptionNew("blastn", FALSE);
         options->wordsize = 7;
         options->filter_string = StringSave("m L");
         options->expect_value = spot->secpasseval;
         options->query_lcase_mask = spot->lcaseloc;
         if (spot->interspecies)
         {
            options->gap_x_dropoff_final = 100;
            options->gap_open = 5;
            options->gap_extend = 1;
            options->penalty = -1;
         }
         sap = BlastTwoSequencesByLoc(slp2, slp1, "blastn", options);
         if (sap == NULL)
            return NULL;
         AlnMgr2IndexLite(sap);
         SPI_flip_sa_list((SeqAlignPtr)(sap->segs));
         AMAlignIndex2Free2(sap->saip);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
         sap->saip = NULL;
         AlnMgr2IndexLite(sap);
         SPI_RemoveInconsistentAlnsFromSet(sap, SPI_TEENYEXON/2, 1, SPI_LEFT);
         SPI_GetNthSeqRangeInSASet(sap, 1, &gstart, &gstop);
         SPI_GetNthSeqRangeInSASet(sap, 2, &mstart, &mstop);
         BLASTOptionDelete(options);
         if (spot->draftfile != NULL)
            return sap;
         if (mstart - start_m <= 3*SPI_FUZZ)
         {
            sap_new = sap;
            if (sap_new->segtype == SAS_DISC)
            {
               sap_new2 = (SeqAlignPtr)(sap_new->segs);
               sap_new->segs = NULL;
               SeqAlignFree(sap_new);
               sap_new = sap_new2;
               while (sap_new2 != NULL)
               {
                  AlnMgr2IndexSingleChildSeqAlign(sap_new2); 
                  sap_new2 = sap_new2->next;
               }
            } else
               SPI_AddToAln(sap_new, mstart - start_m, SPI_LEFT, strand);
            return sap_new;
         } else
         {
            sap_new = SPI_FillInLastmRNAHoles(sap, sip1, sip2, start_g+1, gstop, start_m+1, mstop, strand);
            return sap_new;
         }
      }
   }
   return NULL;
}


/* added by KSK for SPI_AdjustForSplice() when mRNA regions overlap */
static int SPI_Choose2LooseMrnaOvLap (const SeqAlignPtr sap1, const SeqAlignPtr sap2, 
                                      const SPI_mRNAPtr smp, const int ptr1offset)
{
    Int4 p1_sites = 0, p2_sites = 0;
    Int4 score1 = 0, score2 = 0;
    float margin = 0;
    
    if (sap1 == NULL || sap2 == NULL || smp == NULL){
        return -1;
    }
   
    score1 = AlnMgr2ComputeScoreForSeqAlign(sap1);
    score2 = AlnMgr2ComputeScoreForSeqAlign(sap2);
    
    if (score1 >= score2){
        margin = (float)score1/5;
        if ((float)score1 >= (((float)(score2)) + margin)){
            return ptr1offset + 1;
        }
    }
    else if (score1 <= score2){
        margin = (float)score2/5;
        if ((float)score2 >= (((float)(score1)) + margin)){
            return ptr1offset;
        }
    }

    p1_sites = smp->splicedon[ptr1offset] + smp->spliceacc[ptr1offset];
    p2_sites = smp->splicedon[ptr1offset + 1] + smp->spliceacc[ptr1offset + 1];
    
    if (p1_sites > p2_sites){
        return ptr1offset + 1;
    }
    else if (p2_sites > p1_sites){
        return ptr1offset;
    }
    return (score1 >= score2 ? ptr1offset + 1 : ptr1offset);
}




/***************************************************************************
*
*  SPI_AdjustForSplice adjusts the boundaries of all the alignments in
*  the set so that they abut each other and are at the optimal splice
*  sites. SPI_AdjustForSplice also fills in the mismatch, %id, #gaps
*  and other information for each exon. The function first allocates a new
*  SPI_mRNA structure, then makes sure that the set of alignments doesn't
*  miss tiny pieces on the 5' or 3' end of the mRNA. Next, the alignments
*  are sent in pairs to SPI_AdjustOverlaps, which adjusts the alignment
*  boundaries so that they are adjacent to each other and to good splice
*  sites. The function then checks to see whether any two alignments are
*  adjacent on both the genomic and mRNA sequences; if so, these alignments
*  are merged. Each alignment (now each alignment is exactly one exon) is
*  sent to SPI_GetExonInfo to get the %id, #gaps, etc.; the overall
*  % coverage is computed and the alignments are examined to see whether
*  one or both ends of the mRNA are missing, and then all the information
*  is returned to the calling function.
*
***************************************************************************/
static SPI_mRNAPtr SPI_AdjustForSplice(SeqAlignPtr sap, SPI_OptionsPtr spot, SPI_RegionInfoPtr srip)
{
   AMAlignIndex2Ptr  amaip;
   Int4             b;
   BioseqPtr        bsp;
   Int4             c;
   Int4             count, sap2delete = 0;
   Int4             gstart1;
   Int4             gstart2;
   Int4             gstop1;
   Int4             gstop2;
   Int4             i;
   Int4             intronsize;
   Int4             len;
   Int4             len1;
   Int4             len2;
   Int4             min;
   Int4             mis;
   Int4             max;
   Int4             mstart1;
   Int4             mstart2;
   Int4             mstop1;
   Int4             mstop2;
   Int4             n;
   SeqAlignPtr      PNTR saparray;
   SeqIdPtr         sip;
   SPI_mRNAPtr      smp;
   SPI_mRNAPtr      smp_new;
   Uint1            strand;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return NULL;
   if (spot->bigintron){
       intronsize = (spot->bigintron_size > SPI_INTRONSIZEXL 
                     ? spot->bigintron_size : SPI_INTRONSIZEXL);
       /*intronsize = SPI_INTRONSIZEXL;*/
   }
   else{
       intronsize = SPI_INTRONSIZE;
   }
   /*end KSK*/
   AlnMgr2SortAlnSetByNthRowPos(sap, 1);
   SPI_RemoveTeenyAlns(sap, SPI_TEENYEXON);
   if (sap->segs == NULL)
   {
      SeqAlignFree(sap);
      return NULL;
   }
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   strand = AlnMgr2GetNthStrand(amaip->saps[0], 2);
   /* first allocate a new SPI_mRNA structure to hold all the information */
   smp = (SPI_mRNAPtr)MemNew(sizeof(SPI_mRNA));
   smp->numexons = amaip->numsaps;
   smp->exonid = (FloatHiPtr)MemNew((smp->numexons)*sizeof(FloatHi));
   smp->splicedon = (Uint1Ptr)MemNew((smp->numexons)*sizeof(Uint1));
   smp->spliceacc = (Uint1Ptr)MemNew((smp->numexons)*sizeof(Uint1));
   smp->exongaps = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
   smp->saps = (SeqAlignPtr PNTR)MemNew((smp->numexons)*sizeof(SeqAlignPtr));
   smp->mstarts = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
   smp->mstops = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
   smp->gstarts = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
   smp->gstops = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
   smp->strand = strand;
   /* make sure the alignment doesn't leave out little bits on the ends */
   sip = AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 2);
   bsp = BioseqLockById(sip);
   len1 = bsp->length;
   len1 = len1 - srip->polyAtail;
   BioseqUnlock(bsp);
   SeqIdFree(sip);
   sip = AlnMgr2GetNthSeqIdPtr(amaip->saps[0], 1);
   bsp = BioseqLockById(sip);
   len2 = bsp->length;
   BioseqUnlock(bsp);
   SeqIdFree(sip);
   if (strand != Seq_strand_minus)
   {
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 1, &gstart1, &gstop1);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 2, &mstart1, &mstop1);
      if (mstart1 > 0 && mstart1 <= SPI_ENDFUZZ && gstart1 >= mstart1)
         SPI_AddToAln(amaip->saps[0], mstart1, SPI_LEFT, strand);
      else if (mstart1 > 0 && mstart1 <= SPI_ENDFUZZ && gstart1 < mstart1)
         SPI_AddToAln(amaip->saps[0], gstart1, SPI_LEFT, strand);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[amaip->numsaps-1], 2, &mstart2, &mstop2);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[amaip->numsaps-1], 1, &gstart2, &gstop2);
      if (len1 - srip->polyAtail - mstop2-1 > 0 && len1 - mstop2-1 <= SPI_ENDFUZZ && len2-gstop2 >= len1-mstop2)
         SPI_AddToAln(amaip->saps[amaip->numsaps-1], len1-mstop2-1, SPI_RIGHT, strand);
      else if (len1-mstop2-1 - srip->polyAtail > 2 && len1 - mstop2-1 <= SPI_ENDFUZZ && len1-mstop2 > len2 - gstop2)
         SPI_AddToAln(amaip->saps[amaip->numsaps-1], len2-gstop2-1, SPI_RIGHT, strand);
   } else
   {
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 1, &gstart1, &gstop1);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 2, &mstart2, &mstop2);
      if (len1 - mstop2-1-srip->polyAtail > 0 && len1 - mstop2-1 <= 2*SPI_TEENYEXON && gstart1 >= (len1 - mstop2 - 1))
         SPI_AddToAln(amaip->saps[0], len1-mstop2-1, SPI_LEFT, strand);
      else if (len1 - mstop2-1-srip->polyAtail > 0 && len1 - mstop2-1 <= 2*SPI_TEENYEXON && gstart1 < (len1 - mstop2 - 1))
         SPI_AddToAln(amaip->saps[0], gstart1, SPI_LEFT, strand);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[amaip->numsaps-1], 2, &mstart1, &mstop1);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[amaip->numsaps-1], 1, &gstart2, &gstop2);
      if (mstart1 > 0 && mstart1 <= SPI_ENDFUZZ && len2 - gstop2 -1> mstart1)
         SPI_AddToAln(amaip->saps[amaip->numsaps-1], mstart1, SPI_RIGHT, strand);
      else if (mstart1 > 0 && mstart1 <= SPI_ENDFUZZ && len2 - gstop2 -1<= mstart1)
          SPI_AddToAln(amaip->saps[amaip->numsaps-1], len2-gstop2-1, SPI_RIGHT, strand);
   }
   /* send the alignments in pairs to be adjusted to good splice sites */
   for (i=0; i<amaip->numsaps - 1; i++)
   {
      SPI_AdjustOverlaps(amaip->saps[i], amaip->saps[i+1], i, smp, spot);
   }
   n = 0;
   for (i=0; i<amaip->numsaps-1; i++) /* merge adjacent alignments */
   {
      
      amaip->saps[i]->next = NULL;
      amaip->saps[i+1]->next = NULL;
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, &gstart1, &gstop1);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 1, &gstart2, &gstop2);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, &mstart1, &mstop1);
      AlnMgr2GetNthSeqRangeInSA(amaip->saps[i+1], 2, &mstart2, &mstop2);
      /* if (gstart2 >= gstop1 - SPI_EXONMERGESIZE && gstart2 <= gstop1 + SPI_EXONMERGESIZE) */
      if (gstart2 >= gstop1 - SPI_EXONMERGESIZE && gstart2 <= gstop1 + SPI_EXONMERGESIZE){
          if ((mstart2 >= mstop1 - SPI_EXONMERGESIZE && mstart2 <= mstop1 + SPI_EXONMERGESIZE) 
              || (mstart1 >= mstop2 - SPI_EXONMERGESIZE && mstart1 <= mstop2 + SPI_EXONMERGESIZE)){
              amaip->saps[i+1] = SPI_MergeAlignments(amaip->saps[i], amaip->saps[i+1]);
              SeqAlignFree(amaip->saps[i]);
              amaip->saps[i] = NULL;
              n++;
          }
      }
      /** KSK added this 'else if' block in case there are overlapping **
       ** mrna sequences shared by these seqaligns **/

      else if ((strand == Seq_strand_minus && (mstop2 <= mstop1 && mstop2 >= mstart1))
               || (strand == Seq_strand_plus && (mstop1 <= mstop2 && mstop1 >= mstart2))){
          /* fixes in case the MRNA portion of different regions overlap */
          /* first, if one is subsumed */
          if (mstart1 >= mstart2 && mstop1 <= mstop2){
              SeqAlignFree(amaip->saps[i]);
              amaip->saps[i] = NULL;
              n++;
          }
          else if (mstart2 <= mstart1 && mstop2 >= mstop1){
              SeqAlignFree(amaip->saps[i+1]);
              amaip->saps[i+1] = amaip->saps[i];
              amaip->saps[i] = NULL;
              n++;
          }
          /* now if mRNA region overlaps - simply choose the highest score */
          /* unless the lower has splice donor & acceptor and the higher */
          /* does not */
          else {
              if ((sap2delete = SPI_Choose2LooseMrnaOvLap(amaip->saps[i], amaip->saps[i+1],
                                                          smp, i)) != -1) {
                  if (sap2delete == i){
                      SeqAlignFree(amaip->saps[i]);
                      amaip->saps[i] = NULL;
                      n++;
                  }
                  else if (sap2delete == (i+1)){
                      SeqAlignFree(amaip->saps[i+1]);
                      amaip->saps[i+1] = amaip->saps[i];
                      amaip->saps[i] = NULL;
                      n++;
                  }
              }
          }
      }
   }             
   
   if (n > 0) /* some alignments were merged; need a new smp */
   {
      saparray = (SeqAlignPtr PNTR)MemNew((amaip->numsaps-n)*sizeof(SeqAlignPtr));
      count = 0;
      smp_new = (SPI_mRNAPtr)MemNew(sizeof(SPI_mRNA));
      smp_new->numexons = amaip->numsaps-n;
      smp_new->exonid = (FloatHiPtr)MemNew((smp_new->numexons)*sizeof(FloatHi));
      smp_new->splicedon = (Uint1Ptr)MemNew((smp_new->numexons)*sizeof(Uint1));
      smp_new->spliceacc = (Uint1Ptr)MemNew((smp_new->numexons)*sizeof(Uint1));
      smp_new->exongaps = (Int4Ptr)MemNew((smp_new->numexons)*sizeof(Int4));
      smp_new->saps = (SeqAlignPtr PNTR)MemNew((smp_new->numexons)*sizeof(SeqAlignPtr));
      smp_new->mstarts = (Int4Ptr)MemNew((smp_new->numexons)*sizeof(Int4));
      smp_new->mstops = (Int4Ptr)MemNew((smp_new->numexons)*sizeof(Int4));
      smp_new->gstarts = (Int4Ptr)MemNew((smp_new->numexons)*sizeof(Int4));
      smp_new->gstops = (Int4Ptr)MemNew((smp_new->numexons)*sizeof(Int4));
      smp_new->strand = strand;
      n = 0;
      for (i=0; i<amaip->numsaps; i++)
      {
         if (amaip->saps[i] != NULL)
         {
            saparray[n] = amaip->saps[i];
            n++;
            if (strand == Seq_strand_minus)
            {
               if (i != 0)
                  smp_new->splicedon[count] = smp->splicedon[i-1];
               smp_new->spliceacc[count] = smp->spliceacc[i];
            } else
            {
               smp_new->splicedon[count] = smp->splicedon[i];
               if (i != 0)
                  smp_new->spliceacc[count] = smp->spliceacc[i-1];
            }
            smp_new->saps[count] = smp->saps[i];
            count++;
         }
         smp->saps[i] = NULL;
      }
      MemFree(amaip->saps);
      amaip->saps = saparray;
      amaip->numsaps = n;
      SPI_mRNAFree(smp);
      smp = smp_new;
   }
   len = 0;
   max = 0;
   mis = 0;
   /* now get the %id, #mismatches, #gaps, etc. for each exon */
   for (i=0; i<amaip->numsaps; i++)
   {
      smp->saps[i] = amaip->saps[i];
      len += SPI_GetExonInfo(smp, i, &b, &c, &mis, spot);
      if (i == 0)
         min = b;
      else
      {
         if (b < min)
            min = b;
      }
      if (c > max)
         max = c;
   }
   sip = AlnMgr2GetNthSeqIdPtr(smp->saps[0], 1);
   bsp = BioseqLockById(sip);
   len2 = bsp->length;
   BioseqUnlock(bsp);
   SeqIdFree(sip);
   /* decide whether either end is left out of the alignments */
   if (min != 0)
   {
      if (max < len1 - 1)
         smp->missingends = SPI_BOTH;
      else
         smp->missingends = SPI_LEFT;
   } else
   {
      if (max < len1 - 1)
         smp->missingends = SPI_RIGHT;
      else
         smp->missingends = SPI_NEITHER;
   }
   if (srip->strand == Seq_strand_minus)
   {
      if ((smp->missingends == SPI_RIGHT || smp->missingends == SPI_BOTH) && smp->gstarts[0] < intronsize)
         srip->fallsoff = TRUE;
      if ((smp->missingends == SPI_LEFT || smp->missingends == SPI_BOTH) && smp->gstops[smp->numexons-1] > len2 - intronsize)
         srip->fallsoff = TRUE;
   } else
   {
      if ((smp->missingends == SPI_LEFT  || smp->missingends == SPI_BOTH) && smp->gstops[smp->numexons-1] > len2 - intronsize)
         srip->fallsoff = TRUE;
      if ((smp->missingends == SPI_RIGHT || smp->missingends == SPI_BOTH) && smp->gstarts[0] < intronsize)
         srip->fallsoff = TRUE;
   }
   smp->mRNAcoverage = (100*len)/len1;
   smp->mismatch = (FloatHi)(100*mis)/len;
   amaip->saps = smp->saps;
   for (i=0; i<amaip->numsaps-1; i++)
   {
      amaip->saps[i]->next = amaip->saps[i+1];
      amaip->saps[i+1]->next = NULL;
   }
   sap->segs = (Pointer)(amaip->saps[0]);
   smp->parent = sap;
   return smp;
}

/***************************************************************************
*
*  SPI_GetExonInfo creates a profile of the indicated exon, then
*  walks through the profile structure to find mismatches and gaps.
*  The gap positions are recorded (if the alignment is going to be
*  printed -- otherwise the number of gaps is recorded, but not the
*  positions) in the SPI_TinyInfo structures. If the alignment is going
*  to be printed, a SPI_ExonProf structure is created to hold all the
*  mismatch positions, and the exon profile is put into the smp. Regardless,
*  the number of gaps and the number of mismatches are recorded. The length
*  of the exon is returned.
*
***************************************************************************/
static Int4 SPI_GetExonInfo(SPI_mRNAPtr smp, Int4 n, Int4Ptr start, Int4Ptr stop, Int4Ptr mis, SPI_OptionsPtr spot)
{
   ACTProfilePtr    app;
   ACTProfilePtr    app_head;
   Int4             c;
   Int4             counter;
   SPI_ExonProfPtr  epp;
   SPI_ExonProfPtr  epp_tmp;
   Boolean          found;
   Int4             i;
   Int4             j;
   Int4             mismatch;
   SPI_TinyInfoPtr  spit;
   SPI_TinyInfoPtr  spit_head;
   SPI_TinyInfoPtr  spit_prev;

   AlnMgr2GetNthSeqRangeInSA(smp->saps[n], 2, start, stop);
   smp->mstarts[n] = *start;
   smp->mstops[n] = *stop;
   AlnMgr2GetNthSeqRangeInSA(smp->saps[n], 1, &smp->gstarts[n], &smp->gstops[n]);
   app_head = SPI_MakeProfileFromSA(smp->saps[n]);
   smp->exongaps[n] = 0;
   mismatch = 0;
   app = app_head;
   spit_head = spit_prev = NULL;
   counter = 0;
   while (app != NULL)
   {
      for (i=0; i<app->len; i++)
      {
         found = FALSE;
         c = 0;
         for (j=0; j<ACT_NUCLEN; j++)
         {
            if (app->freq[j][i] == 1 && !found)
            {
               /*if (app->freq[4][i] == 0)  not an N 
               {*/
                  mismatch += 1;
                  found = TRUE;
                  if (spot->printaln != 1) /* if it's going to be printed, save up the mismatches */
                  {
                     spit = (SPI_TinyInfoPtr)MemNew(sizeof(SPI_TinyInfo));
                     spit->n = counter;
                     if (spit_head != NULL)
                     {
                        spit_prev->next = spit;
                        spit_prev = spit;
                     } else
                        spit_head = spit_prev = spit;
                  }
              /* }*/
            }
            c += app->freq[j][i];
         }
         if (c != 2)
            smp->exongaps[n]++;
         counter++;
      }
      app = app->next;
   }
   SPI_ProfileSetFree(app_head);
   smp->exonid[n] = (FloatHi)(100) - ((FloatHi)(100*mismatch))/(FloatHi)(*stop - *start + 1);
   if (mismatch > 0 && smp->exonid[n] > 99.9)
      smp->exonid[n] = 99.9;
   *mis += mismatch;
   /* if there are mismatches, and the alignment is going to be printed, then */
   /* create an ExonProf structure to hold the mismatch positions.            */
   if (spot->printaln != 1 && spit_head != NULL)
   {
      spit = spit_head;
      i = 0;
      while (spit != NULL)
      {
         i++;
         spit = spit->next;
      }
      epp = (SPI_ExonProfPtr)MemNew(sizeof(SPI_ExonProf));
      epp->exonnum = n+1;
      epp->nummismatches = i;
      epp->mismatches = (Int4Ptr)MemNew(i*sizeof(Int4));
      i = 0;
      spit = spit_head;
      while (spit != NULL)
      {
         epp->mismatches[i] = spit->n;
         spit_prev = spit->next;
         MemFree(spit);
         spit = spit_prev;
         i++;
      }
      if (smp->epp != NULL)
      {
         epp_tmp = smp->epp;
         while (epp_tmp->next != NULL)
         {
            epp_tmp = epp_tmp->next;
         }
         epp_tmp->next = epp;
      } else
         smp->epp = epp;
   }
   return (*stop - *start + 1);
}

/***************************************************************************
*
*  SPI_AdjustOverlaps takes two adjacent alignments and adjusts their
*  boundaries so that they abut each other and so that they are adjacent
*  to good splice donor and acceptor sites. The function fills in the
*  appropriate splicedon and spliceacc fields in the smp structure to
*  indicate whether each alignment has a splice donor or acceptor site.
*  Since the splice matrices are organism-specific, this function
*  needs the spot->organism information.
*  SPI_AdjustOverlaps first gets the donor and acceptor splice information
*  (length of consensus sequence and the boundary, which is the offset into
*  the consensus sequence for the intron-exon boundary). Then a window
*  around the right end of sap1 is examined for donor sites. The top
*  SPI_NUMSITES donor sites are examined more carefully: each is evaluated
*  as to whether it has a good acceptor site in sap2, how much it affects
*  the boundaries of sap1 and sap2, and how good its donor site score is.
*  The donor site with the best acceptor site, that affects the alignments
*  the least and has the best donor score, is the site that is chosen. If
*  no good site is found, sap1 is unchanged and sap2 is truncated or
*  extended as needed. If a good site is found, sap1 and sap2 are both
*  truncated or extended as needed.
*
*  For the plus strand models (mRNA and genomic sequence in same orientation):
*
*  start1--------------------stop1          start2----------------stop2
*                   |                   |
*                   <- look from here ->
*                     to here for a donor splice site -- the range is
*  stop1 - ovl - fluff - boundary to stop1 + spllen + fluff.
*  When a sequence matches the consensus splice sequence, its offset
*  into the range is recorded. The new stop position is then
*  stop1 - ovl - fluff - boundary + offset + (spllen - boundary).
*
*
*  For the minus strand models:
*
*  start1--------------------stop1          start2-----------------stop2
*  mstop1--------------------mstart1        mstop2---------------- mstart2
*                                   |                |
*                                   <- search this ->
*                                    interval on the minus strand of the
*  genomic sequence for a donor splice site -- the range is
*  start2 - fluff - spllen to start2 + ovl + fluff + boundary. When
*  the offset of a match is computed, the new mRNA stop position is
*  stop2 - ovl - boundary - fluff + offset + (spllen - boundary).
*
*
*  xxxGTxxxxx <- vertebrate splice donor consensus, GT are the first two
*  residues of the intron.  Here spllen is 10 and boundary is 8 -- when
*  counting from the 3' end, the 8th residue is the first residue of the
*  exon.
*
***************************************************************************/
static void SPI_AdjustOverlaps(SeqAlignPtr sap1, SeqAlignPtr sap2, Int4 n, SPI_mRNAPtr smp, SPI_OptionsPtr spot)
{
   Boolean     both;
   Int4        boundary;
   Int4        boundary_a;
   BioseqPtr   bsp;
   Uint1Ptr    buf;
   /** Uint1       buf2[200]; **/
   Uint1Ptr    buf2;
   Uint1Ptr    buf3;
   Int4        c;
   Int4        f;
   Int4        fluff;
   Int4        gstart;
   Int4        i;
   FloatHi     maxsc;
   Int4        offset;
   Int4        ovl;
   Int4        pos;
   Uint1       res;
   FloatHi     score;
   SeqIdPtr    sip;
   SPI_Splice  splarray[SPI_NUMSITES];
   Int4        spllen;
   Int4        spllen_a;
   SeqPortPtr  spp;
   Int4        spp_start;
   Int4        spp_end;
   Int4        start;
   Int4        start1;
   Int4        start2;
   Int4        stop1;
   Int4        stop2;
   Uint1       strand;
   Int4        tmp;

   strand = AlnMgr2GetNthStrand(sap1, 2);
   AlnMgr2GetNthSeqRangeInSA(sap1, 1, &start1, &stop1);
   AlnMgr2GetNthSeqRangeInSA(sap2, 1, &start2, &stop2);
   sip = AlnMgr2GetNthSeqIdPtr(sap1, 1);
   bsp = BioseqLockById(sip);
   if (strand == Seq_strand_minus)
   {
      gstart = stop1;
      start = start2;
   } else
   {
      gstart = start2;
      start = stop1;
   }
   /* retrieve the organism-specific donor and acceptor information */
   SPI_GetDonorSpliceInfo(spot->organism, &spllen, &boundary, spot);
   SPI_GetAcceptorSpliceInfo(spot->organism, &spllen_a, &boundary_a, spot);
   /* get the overlap between the alignments */
   ovl = spi_get_overlap(sap1, sap2);
   if (-ovl > 2*SPI_TEENYEXON)
      return;
   if (ovl < 0 && -ovl <= 2*SPI_TEENYEXON) /* extend alignments until they do overlap, to get the splice site right */
   {
      SPI_ExtendAlnAlgDumb(sap2, -ovl+2, SPI_LEFT, strand);
      SPI_ExtendAlnAlgDumb(sap1, -ovl+2, SPI_RIGHT, strand);
      AlnMgr2GetNthSeqRangeInSA(sap1, 1, &start1, &stop1);
      AlnMgr2GetNthSeqRangeInSA(sap2, 1, &start2, &stop2);
      if (strand == Seq_strand_minus)
      {
         gstart = stop1;
         start = start2;
      } else
      {
         gstart = start2;
         start = stop1;
      }
      ovl = -ovl;
   }
   ovl = MIN(abs(ovl), abs(start2-stop1));
   if (spot->interspecies == TRUE)
      fluff = SPI_FLUFF;
   else
      fluff = 0;
   if (ovl != 0)
   {
      /* open a seqport in a window around the end of donor sap, and look for donor sites */
      if (strand != Seq_strand_minus)
      {
         spp_start = start - ovl - fluff - (spllen - boundary);
         spp_end = start + spllen + ovl + fluff;
         if (start-ovl-fluff-spllen+boundary < 0)
            spp_start = 0;
         if (start+spllen+fluff+ovl > bsp->length-1)
            spp_end = bsp->length-1;
         spp = SeqPortNew(bsp, spp_start, spp_end, strand, Seq_code_ncbi4na);
      } else
      {
         spp_start = start - fluff - spllen;
         spp_end = start + ovl + fluff + spllen - boundary;
         if (spp_start < 0)
            spp_start = 0;
         if (spp_end > bsp->length-1)
            spp_end = bsp->length - 1;
         spp = SeqPortNew(bsp, spp_start, spp_end, strand, Seq_code_ncbi4na);
      }
      i = 0;
      buf = (Uint1Ptr)MemNew((2*fluff+ovl+spllen+2)*sizeof(Uint1));
      buf2 = (Uint1Ptr)MemNew((2*fluff+ovl+spllen+2)*sizeof(Uint1));
      SeqPortRead(spp, buf2, 2*fluff+ovl+spllen+2);
      for (f=0; f<SPI_NUMSITES; f++)
      {
         splarray[f].i = 0;
         splarray[f].score = -2;
      }
      while (((res = buf2[i]) != SEQPORT_EOF) && i<(2*fluff+ovl+spllen+1))
      {
         if (res == 1)
            buf[i] = 0;
         else if (res == 2)
            buf[i] = 1;
         else if (res == 4)
            buf[i] = 2;
         else if (res == 8)
            buf[i] = 3;
         else
            buf[i] = 4;
         i++;
      }
      SeqPortFree(spp);
      MemFree(buf2);
      for (i=0; i<2*fluff+ovl+1; i++)
      {
         if (spot->dsplicejunc > 0)
            SPI_is_donor_user(buf+i, spllen, &score, spot);
         else
            SPI_is_donor(buf+i, spllen, &score, spot->organism);
         c = 0;
         if (score > 0.000001)
         {
            for (f=0; f<SPI_NUMSITES; f++)
            {
               if (f == 0)
                  maxsc = splarray[f].score;
               else if (splarray[f].score < maxsc)
               {
                  maxsc = splarray[f].score;
                  c = f;
               }
            }
            if (score > splarray[c].score)
            {
               splarray[c].score = score;
               splarray[c].i = i;
            }
         }
      }
      AlnMgr2GetNthSeqRangeInSA(sap1, 2, &start1, &stop1);
      AlnMgr2GetNthSeqRangeInSA(sap2, 2, &start2, &stop2);
      maxsc = 0;
      /* for the SPI_NUMSITES best donor sites, get the corresponding acceptor */
      /* site score and record how much each alignment would be changed if     */
      /* the alignments were truncated/extended to this sites                  */
      /* pos is the coordinate of the last residue of the donor exon           */
      for (f=0; f<SPI_NUMSITES; f++)
      {
         if (strand == Seq_strand_minus)
         {
            pos = stop2 - ovl - fluff + splarray[f].i;
            if (stop2 - pos < 0)
               splarray[f].diff = pos - stop2;
            else
               splarray[f].diff = stop2 - pos;
            if (start1 - pos < 0)
            {
               if (pos - start1 < splarray[f].diff)
                  splarray[f].diff = pos - start1;
            } else
            {
               if (start1 - pos < splarray[f].diff)
                  splarray[f].diff = start1 - pos;
            }
            tmp = gstart + start1 - (pos + 1);
            SPI_GetAcceptorScore(bsp, tmp - (spllen_a - boundary_a), tmp + boundary_a, strand, &splarray[f].score2, spllen_a, spot);
         } else
         {
            pos = stop1 - ovl - fluff + splarray[f].i;
            if (stop1 - pos < 0)
               splarray[f].diff = pos - stop1;
            else
               splarray[f].diff = stop1 - pos;
            if (start2 - pos < 0)
            {
               if (pos - start2 < splarray[f].diff)
                  splarray[f].diff = pos - start2;
            } else
            {
               if (start2 - pos < splarray[f].diff)
                  splarray[f].diff = start2 - pos;
            }
            tmp = gstart + start2 - (pos + 1);
            SPI_GetAcceptorScore(bsp, tmp - boundary_a + (spllen_a - boundary_a), tmp, strand, &splarray[f].score2, spllen_a, spot);
         }
         if (splarray[f].diff > maxsc)
            maxsc = splarray[f].diff;
      }
      i = 0;
      both = FALSE;
      for (f=0; f<SPI_NUMSITES && !both; f++)
      {
         if (splarray[f].score > 0.000001 && splarray[f].score2 > 0.00000002)
            both = TRUE;
      }
      /* look for the position that has a good acceptor (if one of them does have both */
      /* a good donor and acceptor) and that changes the alignments the least          */
      /* with the highest donor score                                                  */
      offset = 0;
      for (f=0; f<SPI_NUMSITES; f++)
      {
         if ((both && splarray[f].score2 > 0.0000000002) || both == FALSE)
         {
            if(splarray[f].score >= splarray[i].score)
            {
               maxsc = splarray[f].diff;
               offset = splarray[f].i;
               i = f;
            }
         }
      }
      if (strand == Seq_strand_minus)
         pos = stop2 - ovl - fluff + offset;
      else
         pos = stop1 - ovl - fluff + offset;
      MemFree(buf);
      if (splarray[i].score >= 0.00001 && pos > 0)
      {
         if (strand == Seq_strand_minus)
            smp->splicedon[n+1] = 1;
         else
            smp->splicedon[n] = 1;
      } else /* if don't find a good site, don't change the alignment */
         offset = ovl + fluff;
      if (strand == Seq_strand_minus)
         pos = stop2 - ovl - fluff + offset;
      else
         pos = stop1 - ovl - fluff + offset;
      if (splarray[i].score2 > 0.0000000002)
      {
         if (strand == Seq_strand_minus)
            smp->spliceacc[n] = 1;
         else
            smp->spliceacc[n+1] = 1;
      }
      SeqIdFree(sip);
      BioseqUnlock(bsp);
   } else
   {
      AlnMgr2GetNthSeqRangeInSA(sap1, 2, &start1, &stop1);
      AlnMgr2GetNthSeqRangeInSA(sap2, 2, &start2, &stop2);
      if (strand == Seq_strand_minus)
         pos = stop2;
      else
         pos = stop1;
      if (strand != Seq_strand_minus)
      {
         spp_start = start - fluff - (spllen - boundary);
         spp_end = start + spllen + fluff;
         if (start-ovl-fluff-spllen+boundary < 0)
            spp_start = 0;
         if (start+spllen+fluff > bsp->length-1)
            spp_end = bsp->length-1;
         spp = SeqPortNew(bsp, spp_start, spp_end, strand, Seq_code_ncbi4na);
      } else
      {
         spp_start = start - fluff - spllen;
         spp_end = start + fluff + spllen - boundary;
         if (spp_start < 0)
            spp_start = 0;
         if (spp_end > bsp->length-1)
            spp_end = bsp->length - 1;
         spp = SeqPortNew(bsp, spp_start, spp_end, strand, Seq_code_ncbi4na);
      }
      i = 0;
      buf = (Uint1Ptr)MemNew((spp_end-spp_start+1)*sizeof(Uint1));
      buf3 = (Uint1Ptr)MemNew((spp_end-spp_start+1)*sizeof(Uint1));
      SeqPortRead(spp, buf3, spp_end-spp_start+1);
      while (i<(spp_end-spp_start+1) && ((res = buf3[i]) != SEQPORT_EOF))
      {
         if (res == 1)
            buf[i] = 0;
         else if (res == 2)
            buf[i] = 1;
         else if (res == 4)
            buf[i] = 2;
         else if (res == 8)
            buf[i] = 3;
         else
            buf[i] = 4;
         i++;
      }
      SeqPortFree(spp);
      MemFree(buf3);
      if (spot->dsplicejunc > 0)
         SPI_is_donor_user(buf, spllen, &score, spot);
      else
         SPI_is_donor(buf, spllen, &score, spot->organism);
      if (score >= 0.00001)
      {
         if (strand == Seq_strand_minus)
            smp->splicedon[n+1] = 1;
         else
            smp->splicedon[n] = 1;
      }
      if (strand == Seq_strand_minus)
      {
         tmp = gstart + start1 - (pos + 1);
         SPI_GetAcceptorScore(bsp, tmp - (spllen_a - boundary_a), tmp + boundary_a, strand, &score, spllen_a, spot);
      } else
      {
         tmp = gstart + start2 - (pos + 1);
         SPI_GetAcceptorScore(bsp, tmp - boundary_a + (spllen_a - boundary_a), tmp, strand, &score, spllen_a, spot);
      }
      if (score > 0.0000000002)
      {
         if (strand == Seq_strand_minus)
            smp->spliceacc[n] = 1;
         else
            smp->spliceacc[n+1] = 1;
      }
      MemFree(buf);
   }
   /* extend or truncate sap1 and sap2 to abut each other exactly and to */
   /* be adjacent to the chosen splice site                              */
   if (strand == Seq_strand_minus)
   {
      if (pos < stop2)
      {
         if (AlnMgr2TruncateSeqAlign(sap2, start2, pos, 2))
         {
            sap2->next->next = NULL;
            SeqAlignFree(sap2->next);
            sap2->next = NULL;
         }
      } else if (pos > stop2)
         SPI_AddToAln(sap2, MIN(pos-stop2, abs(gstart-start)), SPI_LEFT, strand);
      if (start1 == pos + 1)
         return;
      else if (start1 < pos + 1)
      {
         if (AlnMgr2TruncateSeqAlign(sap1, pos+1, stop1, 2))
         {
            sap1->next->next = NULL;
            SeqAlignFree(sap1->next);
            sap1->next = NULL;
         }
         return;
      } else if (start1 > pos + 1)
      {
         SPI_AddToAln(sap1, MIN(start1-pos-1, abs(gstart-start)), SPI_RIGHT, strand);
         return;
      }
   } else
   {
      if (pos < stop1)
      {
         if (AlnMgr2TruncateSeqAlign(sap1, start1, pos, 2))
         {
            sap1->next->next = NULL;
            SeqAlignFree(sap1->next);
            sap1->next = NULL;
         }
      } else if (pos > stop1)
         SPI_AddToAln(sap1, MIN(pos - stop1, abs(gstart-start)), SPI_RIGHT, strand);
      if (start2 == pos + 1)
         return;
      else if (start2 < pos + 1)
      {
         if (AlnMgr2TruncateSeqAlign(sap2, pos+1, stop2, 2))
         {
            sap2->next->next = NULL;
            SeqAlignFree(sap2->next);
            sap2->next = NULL;
         }
         return;
      } else if (start2 > pos + 1)
      {
         SPI_AddToAln(sap2, MIN(start2-pos-1, abs(gstart-start)), SPI_LEFT, strand);
         return;
      }
   }
}

/***************************************************************************
*
*  SPI_RemoveTeenyAlns removes all alignments in a set that are less
*  than len in length.
*
***************************************************************************/
static void SPI_RemoveTeenyAlns(SeqAlignPtr sap, Int4 len)
{
   Int4             alnlen;
   AMAlignIndex2Ptr  amaip;
   Int4             i;
   SeqAlignPtr      sap_head;
   SeqAlignPtr      sap_prev;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   sap_head = sap_prev = NULL;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   for (i=0; i<amaip->numsaps; i++)
   {
      amaip->saps[i]->next = NULL;
      alnlen = AlnMgr2GetAlnLength(amaip->saps[i], FALSE);
      if (alnlen >= len)
      {
         if (sap_head != NULL)
         {
            sap_prev->next = amaip->saps[i];
            sap_prev = amaip->saps[i];
         } else
            sap_head = sap_prev = amaip->saps[i];
      } else
         SeqAlignFree(amaip->saps[i]);
   }
   sap->segs = (Pointer)(sap_head);
   AMAlignIndex2Free2(amaip);
   sap->saip = NULL;
   if (sap->segs != NULL)
      AlnMgr2IndexLite(sap);
}

static void SPI_ExtendAlnAlgDumb(SeqAlignPtr sap, Int4 ovl, Int4 which_side, Uint1 strand)
{
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_new;
   Int4         i;
   Int4         j;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;

   dsp = (DenseSegPtr)(sap->segs);
   dsp_new = DenseSegNew();
   dsp_new->dim = 2;
   dsp_new->numseg = dsp->numseg+1;
   dsp_new->starts = (Int4Ptr)MemNew(2*dsp_new->numseg*sizeof(Int4));
   dsp_new->strands = (Uint1Ptr)MemNew(2*dsp_new->numseg*sizeof(Uint1));
   dsp_new->lens = (Int4Ptr)MemNew(dsp_new->numseg*sizeof(Int4));
   dsp_new->ids = dsp->ids;
   dsp->ids = NULL;
   i = 0;
   AlnMgr2GetNthSeqRangeInSA(sap, 1, &start1, &stop1);
   AlnMgr2GetNthSeqRangeInSA(sap, 2, &start2, &stop2);
   if (which_side == SPI_LEFT)
   {
      dsp_new->starts[0] = start1-ovl;
      if (strand == Seq_strand_minus)
         dsp_new->starts[1] = stop2+1;
      else
         dsp_new->starts[1] = start2-ovl;
      dsp_new->strands[0] = Seq_strand_plus;
      dsp_new->strands[1] = strand;
      dsp_new->lens[0] = ovl;
      i++;
   }
   for (j=0; j<dsp->numseg; j++)
   {
      dsp_new->starts[2*(j+i)] = dsp->starts[2*j];
      dsp_new->starts[2*(j+i)+1] = dsp->starts[2*j+1];
      dsp_new->strands[2*(j+i)] = Seq_strand_plus;
      dsp_new->strands[2*(j+i)+1] = strand;
      dsp_new->lens[i+j] = dsp->lens[j];
   }
   if (which_side == SPI_RIGHT)
   {
      dsp_new->starts[2*(dsp_new->numseg-1)] = stop1+1;
      if (strand == Seq_strand_minus)
         dsp_new->starts[2*(dsp_new->numseg-1)+1] = start2-ovl;
      else
         dsp_new->starts[2*(dsp_new->numseg-1)+1] = stop2+1;
      dsp_new->strands[2*(dsp_new->numseg-1)] = Seq_strand_plus;
      dsp_new->strands[2*(dsp_new->numseg-1)+1] = strand;
      dsp_new->lens[dsp_new->numseg-1] = ovl;
   }
   DenseSegFree(dsp);
   sap->segs = (Pointer)dsp_new;
   SAIndex2Free2(sap->saip);
   sap->saip = NULL;
   AlnMgr2IndexSingleChildSeqAlign(sap);
}

/***************************************************************************
*
*  SPI_ExtendAlnAlg 'intelligently' extends an alignment by the given
*  amount, by figuring out where the newly added bit should align to on
*  the genomic sequence. The 'alignment' is only a string search, so no
*  gaps are allowed.
*
***************************************************************************/
static void SPI_ExtendAlnAlg(SeqAlignPtr sap, Int4 ovl, Int4 which_side, Uint1 strand)
{
   BioseqPtr    bsp1;
   BioseqPtr    bsp2;
   Uint1        buf1[20];
   Uint1        buf2[20];
   DenseSegPtr  dsp;
   DenseSegPtr  dsp_new;
   Int4         gap;
   Int4         i;
   Int4         j;
   Boolean      mismatch;
   SeqIdPtr     sip1;
   SeqIdPtr     sip2;
   SeqPortPtr   spp;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;

   if (sap == NULL || ovl == 0 || ovl >= 20-SPI_MAXGAP)
      return;
   AlnMgr2GetNthSeqRangeInSA(sap, 1, &start1, &stop1);
   AlnMgr2GetNthSeqRangeInSA(sap, 2, &start2, &stop2);
   sip1 = AlnMgr2GetNthSeqIdPtr(sap, 1);
   sip2 = AlnMgr2GetNthSeqIdPtr(sap, 2);
   bsp1 = BioseqLockById(sip1);
   bsp2 = BioseqLockById(sip2);
   if (which_side == SPI_LEFT)
   {
      spp = SeqPortNew(bsp1, start1-(SPI_MAXGAP+ovl), start1-1, Seq_strand_plus, Seq_code_ncbi4na);
      SeqPortRead(spp, buf1, 20);
      SeqPortFree(spp);
      if (strand == Seq_strand_minus)
      {
         spp = SeqPortNew(bsp2, stop2+1, stop2 + 1 + ovl, strand, Seq_code_ncbi4na);
         SeqPortRead(spp, buf2, 20);
         SeqPortFree(spp); 
      } else
      {
         spp = SeqPortNew(bsp2, start2-1-ovl, start2-1, strand, Seq_code_ncbi4na);
         SeqPortRead(spp, buf2, 20);
         SeqPortFree(spp);
      }
      gap = -1;
      for (i=0; i<SPI_MAXGAP; i++)
      {
         mismatch = FALSE;
         for (j=0; j<ovl; j++)
         {
            if (buf2[j] != buf1[i+j])
               mismatch = TRUE;
         }
         if (mismatch == FALSE)
            gap = SPI_MAXGAP-i;
      }
      if (gap > 0)
      {
         dsp = (DenseSegPtr)(sap->segs);
         dsp_new = DenseSegNew();
         dsp_new->dim = 2;
         dsp_new->numseg = dsp->numseg+2;
         dsp_new->ids = dsp->ids;
         dsp->ids = NULL;
         dsp_new->starts = (Int4Ptr)MemNew((dsp_new->numseg)*2*sizeof(Int4));
         dsp_new->lens = (Int4Ptr)MemNew((dsp_new->numseg)*sizeof(Int4));
         dsp_new->strands = (Uint1Ptr)MemNew((dsp_new->numseg)*2*sizeof(Uint1));
         for (i=2; i<dsp_new->numseg; i++)
         {
            dsp_new->starts[i*2] = dsp->starts[(i-2)*2];
            dsp_new->starts[i*2+1] = dsp->starts[(i-2)*2+1];
            dsp_new->strands[i+2] = dsp->strands[(i-2)*2];
            dsp_new->strands[i*2+1] = dsp->strands[(i-2)*2+1];
            dsp_new->lens[i] = dsp->lens[i-2];
         }
         dsp_new->starts[0] = dsp->starts[0] - gap - ovl;
         dsp_new->starts[2] = dsp_new->starts[0] + ovl;
         dsp_new->starts[3] = -1;
         dsp_new->strands[0] = dsp_new->strands[2] = dsp->strands[0];
         dsp_new->strands[1] = dsp_new->strands[2] = dsp->strands[1];
         dsp_new->lens[0] = ovl;
         dsp_new->lens[1] = gap;
         if (strand == Seq_strand_minus)
            dsp_new->starts[1] = stop2 + 1;
         else
            dsp_new->starts[1] = start2 - ovl;
         sap->segs = (Pointer)dsp_new;
         DenseSegFree(dsp);
         SAIndex2Free2(sap->saip);
         sap->saip = NULL;
         AlnMgr2IndexSingleChildSeqAlign(sap);
      } else
         SPI_AddToAln(sap, ovl, SPI_LEFT, strand);
   } else if (which_side == SPI_RIGHT)
   {
      spp = SeqPortNew(bsp1, stop1, stop1+SPI_MAXGAP, Seq_strand_plus, Seq_code_ncbi4na);
      SeqPortRead(spp, buf1, 20);
      SeqPortFree(spp);
      if (strand == Seq_strand_minus)
      {
         spp = SeqPortNew(bsp2, start2-ovl, start2-1-ovl, strand, Seq_code_ncbi4na);
         SeqPortRead(spp, buf2, 20);
         SeqPortFree(spp);
      } else
      {
         spp = SeqPortNew(bsp2, stop2+1, stop2+1+ovl, strand, Seq_code_ncbi4na);
         SeqPortRead(spp, buf2, 20);
         SeqPortFree(spp);
      }
      gap = -1;
      for (i=0; i<SPI_MAXGAP; i++)
      {
         mismatch = FALSE;
         for (j=0; j<ovl; j++)
         {
            if (buf1[i+j] != buf2[j])
               mismatch = TRUE;
         }
         if (mismatch == FALSE && gap == -1)
            gap = i;
      }
      if (gap > 0)
      {
         dsp = (DenseSegPtr)(sap->segs);
         dsp_new = DenseSegNew();
         dsp_new->dim = 2;
         dsp_new->numseg = dsp->numseg+2;
         dsp_new->ids = dsp->ids;
         dsp->ids = NULL;
         dsp_new->starts = (Int4Ptr)MemNew((dsp_new->numseg)*2*sizeof(Int4));
         dsp_new->lens = (Int4Ptr)MemNew((dsp_new->numseg)*sizeof(Int4));
         dsp_new->strands = (Uint1Ptr)MemNew((dsp_new->numseg)*2*sizeof(Uint1));
         for (i=0; i<dsp->numseg; i++)
         {
            dsp_new->starts[i*2] = dsp->starts[i*2];
            dsp_new->starts[i*2+1] = dsp->starts[i*2+1];
            dsp_new->strands[i+2] = dsp->strands[i*2];
            dsp_new->strands[i*2+1] = dsp->strands[i*2+1];
            dsp_new->lens[i] = dsp->lens[i];
         }
         dsp_new->strands[2*dsp->numseg] = dsp_new->strands[2*dsp->numseg+2] = dsp->strands[0];
         dsp_new->strands[2*dsp->numseg+1] = dsp_new->strands[2*dsp->numseg+3] = dsp->strands[1];
         dsp_new->lens[dsp->numseg] = gap;
         dsp_new->lens[dsp->numseg+1] = ovl;
         dsp_new->starts[2*dsp->numseg] = dsp_new->starts[2*(dsp->numseg-1)]+dsp_new->lens[dsp->numseg-1];
         dsp_new->starts[2*dsp->numseg+2] = dsp_new->starts[2*dsp->numseg] + gap;
         if (strand == Seq_strand_minus)
            dsp_new->starts[2*dsp->numseg+3] = start2 - ovl;
         else
            dsp_new->starts[2*dsp->numseg+3] = stop2 + 1;
         dsp_new->starts[2*dsp->numseg+1] = -1;
         sap->segs = (Pointer)dsp_new;
         DenseSegFree(dsp);
         SAIndex2Free2(sap->saip);
         sap->saip = NULL;
         AlnMgr2IndexSingleChildSeqAlign(sap);
      } else
         SPI_AddToAln(sap, ovl, SPI_RIGHT, strand);
   }
   SeqIdFree(sip1);
   SeqIdFree(sip2);
   BioseqUnlock(bsp1);
   BioseqUnlock(bsp2);
}

/***************************************************************************
*
*  SPI_GetAcceptorScore evaluates a given position in a given bioseq
*  for an acceptor splice site.
*
***************************************************************************/
static void SPI_GetAcceptorScore(BioseqPtr bsp, Int4 pos1, Int4 pos2, Uint1 strand, FloatHiPtr score, Int4 spllen, SPI_OptionsPtr spot)
{
   Uint1Ptr    buf;
   Uint1       buf2[100];
   Int4        i;
   Uint1       res;
   SeqPortPtr  spp;

   if (pos1 < 0)
      pos1 = 0;
   if (pos2 > bsp->length-1)
      pos2 = bsp->length-1;
   spp = SeqPortNew (bsp, pos1, pos2, strand, Seq_code_ncbi4na);
   i = 0;
   buf = (Uint1Ptr)MemNew((spllen+2)*sizeof(Uint1));
   SeqPortRead(spp, buf2, spllen+2);
   while (i<(pos2-pos1+1) && ((res = buf2[i]) != SEQPORT_EOF) && i<spllen+1)
   {
      if (res == 1)
         buf[i] = 0;
      else if (res == 2)
         buf[i] = 1;
      else if (res == 4)
         buf[i] = 2;
      else if (res == 8)
         buf[i] = 3;
      else
         buf[i] = 4;
      i++;
   }
   SeqPortFree(spp);
   if (spot->asplicejunc > 0)
      SPI_is_acceptor_user(buf, spllen, score, spot);
   else
      SPI_is_acceptor (buf, spllen, score, spot->organism);
   MemFree(buf);
}

/***************************************************************************
*
*  spi_get_overlap returns the amount of overlap (on the second, or mRNA
*  sequence) between two given alignments. A negative value means no
*  overlap.
*
***************************************************************************/
static Int4 spi_get_overlap (SeqAlignPtr sap1, SeqAlignPtr sap2)
{
   Int4   overlap;
   Int4   start1;
   Int4   stop1;
   Int4   start2;
   Int4   stop2;
   Uint1  strand;

   strand = AlnMgr2GetNthStrand (sap1, 2);
   AlnMgr2GetNthSeqRangeInSA (sap1, 2, &start1, &stop1);
   AlnMgr2GetNthSeqRangeInSA (sap2, 2, &start2, &stop2);
   if (strand == Seq_strand_minus)
      overlap = stop2 - start1 + 1;
   else
      overlap = stop1 - start2 + 1;
   return overlap;
}

/***************************************************************************
*
*  SPI_AddToAln adds the amount "offset" to the specified end of an
*  alignment by adding a segment of length "offset" to both sequences
*  in the alignment. The function assumes that the alignment has two
*  rows, that the first row is on the plus strand, that the second row
*  is on the strand specified, and that adding the amount "offset" will
*  not go past either end of either sequence. This function is used
*  to adjust alignment boundaries to splice sites and to add small
*  pieces onto alignments to make them abut the next adjacent alignment.
*  If the first or last segment (depending on which_end specified) does
*  not have gaps in either row, that segment is simply extended; otherwise,
*  a new segment must be added onto whichever end is to be extended.
*
***************************************************************************/
static void SPI_AddToAln(SeqAlignPtr sap, Int4 offset, Int2 which_end, Uint1 strand)
{
   DenseSegPtr  dsp;
   Int4Ptr      lens;
   Int4         i;
   Int4         j;
   Int4Ptr      starts;
   Uint1Ptr     strands;

   if (sap == NULL || offset == 0)
      return;
   dsp = (DenseSegPtr)(sap->segs);
   if (which_end == SPI_LEFT)
   {
      if (dsp->starts[0] != -1 && dsp->starts[1] != -1) /* neither sequence is gapped */
      {
         dsp->starts[0] -= offset;
         if (strand != Seq_strand_minus)
            dsp->starts[1] -= offset;
         dsp->lens[0] += offset;
      } else /* one of the sequences is gapped -> add a new segment */
      {
         starts = (Int4Ptr)MemNew(2*(dsp->numseg+1)*sizeof(Int4));
         lens = (Int4Ptr)MemNew((dsp->numseg+1)*sizeof(Int4));
         strands = (Uint1Ptr)MemNew(2*(dsp->numseg+1)*sizeof(Uint1));
         AlnMgr2GetNthSeqRangeInSA(sap, 1, &i, &j);
         starts[0] = i - offset;
         AlnMgr2GetNthSeqRangeInSA(sap, 1, &i, &j);
         if (strand == Seq_strand_minus)
            starts[1] = j + 1;
         else
            starts[1] = i - offset;
         lens[0] = offset;
         strands[0] = Seq_strand_plus;
         strands[1] = strand;
         for (i=0; i<dsp->numseg; i++)
         {
           starts[i+1] = dsp->starts[i];
           starts[2*(i+1)] = dsp->starts[2*i];
           lens[i+1] = dsp->lens[i];
           strands[i+1] = dsp->strands[i];
           strands[2*(i+1)] = dsp->strands[2*i];
         }
         dsp->numseg++;
         MemFree(dsp->starts);
         MemFree(dsp->lens);
         MemFree(dsp->strands);
         dsp->starts = starts;
         dsp->lens = lens;
         dsp->strands = strands;
      }
   } else if (which_end == SPI_RIGHT)
   {
      if (dsp->starts[2*(dsp->numseg-1)] != -1 && dsp->starts[2*(dsp->numseg-1)+1] != -1)
      {
         dsp->lens[dsp->numseg-1] += offset;
         if (strand == Seq_strand_minus)
            dsp->starts[2*(dsp->numseg-1)+1] -= offset;
      } else /* one of the sequences is gapped -> add a new segment */
      {
         starts = (Int4Ptr)MemNew(2*(dsp->numseg+1)*sizeof(Int4));
         lens = (Int4Ptr)MemNew((dsp->numseg+1)*sizeof(Int4));
         strands = (Uint1Ptr)MemNew(2*(dsp->numseg+1)*sizeof(Uint1));
         AlnMgr2GetNthSeqRangeInSA(sap, 1, &i, &j);
         starts[2*(dsp->numseg)-1] = i+1;
         AlnMgr2GetNthSeqRangeInSA(sap, 2, &i, &j);
         if (strand == Seq_strand_minus)
           starts[2*(dsp->numseg)] = i - offset;
         else
           starts[2*(dsp->numseg)] = j + 1;
         lens[dsp->numseg] = offset;
         strands[2*(dsp->numseg)-1] = Seq_strand_plus;
         strands[2*(dsp->numseg)] = strand;
         for (i=0; i<dsp->numseg; i++)
         {
            starts[i] = dsp->starts[i];
            starts[2*i] = dsp->starts[2*i];
            lens[i] = dsp->lens[i];
            strands[i] = dsp->strands[i];
            strands[2*i] = dsp->strands[2*i];
         }
         dsp->numseg++;
         MemFree(dsp->starts);
         MemFree(dsp->lens);
         MemFree(dsp->strands);
         dsp->starts = starts;
         dsp->lens = lens;
         dsp->strands = strands;
      }
   }
   /* free the old index and reindex the alignment */
   SAIndex2Free2(sap->saip);
   sap->saip = NULL;
   AlnMgr2IndexSingleChildSeqAlign(sap);
}

/***************************************************************************
*
*  SPI_MergeAlignments takes two dense-seg seqaligns, each with the
*  same two rows, and merges them into a single alignment, with sap1
*  on the left and sap2 on the right. The function does not check to make
*  sure that sap2 belongs after sap1. If sap1 and sap2 are not linearly 
*  consistent, the function extends and truncates the alignments as needed.
*
***************************************************************************/
static SeqAlignPtr SPI_MergeAlignments(SeqAlignPtr sap1, SeqAlignPtr sap2)
{
   DenseSegPtr  dsp1;
   DenseSegPtr  dsp2;
   Int4         glen;
   Int4         gstart1;
   Int4         gstart2;
   Int4         gstop1;
   Int4         gstop2;
   Int4         i;
   Int4         j;
   Int4Ptr      lens;
   Int4         mlen;
   Int4         mstart1;
   Int4         mstart2;
   Int4         mstop1;
   Int4         mstop2;
   Int4         n;
   Int4         offset;
   Int4Ptr      starts;
   Uint1Ptr     strands;

   AlnMgr2GetNthSeqRangeInSA(sap1, 1, &gstart1, &gstop1);
   AlnMgr2GetNthSeqRangeInSA(sap2, 1, &gstart2, &gstop2);
   glen = mlen = 0;
   if (gstart2 <= gstop1)
   {
      AlnMgr2TruncateSeqAlign(sap1, gstart1, gstart2-1, 1);
      gstop1 = gstart2-1;
   }
   AlnMgr2GetNthSeqRangeInSA(sap1, 2, &mstart1, &mstop1);
   AlnMgr2GetNthSeqRangeInSA(sap2, 2, &mstart2, &mstop2);
   if (mstop2 > mstop1)
   {
      if (mstart2 <= mstop1)
         AlnMgr2TruncateSeqAlign(sap2, mstop1+1, mstop2, 2);
   } else
   {
      if (mstart1 <= mstop2)
         AlnMgr2TruncateSeqAlign(sap2, mstart2, mstart1-1, 2);
   }
   AlnMgr2GetNthSeqRangeInSA(sap1, 1, &gstart1, &gstop1);
   AlnMgr2GetNthSeqRangeInSA(sap2, 1, &gstart2, &gstop2);
   AlnMgr2GetNthSeqRangeInSA(sap1, 2, &mstart1, &mstop1);
   AlnMgr2GetNthSeqRangeInSA(sap2, 2, &mstart2, &mstop2);
   glen = gstart2 - gstop1 - 1;
   if (mstop2 > mstop1)
      mlen = mstart2 - mstop1 - 1;
   else
      mlen = mstart1 - mstop2 - 1;
   dsp1 = (DenseSegPtr)(sap1->segs);
   dsp2 = (DenseSegPtr)(sap2->segs);
   n = dsp1->numseg + dsp2->numseg + 2;
   starts = (Int4Ptr)MemNew(2*n*sizeof(Int4));
   lens = (Int4Ptr)MemNew(n*sizeof(Int4));
   strands = (Uint1Ptr)MemNew(2*n*sizeof(Uint1));
   for (i=0; i<2*(dsp1->numseg); i++)
   {
      starts[i] = dsp1->starts[i];
      strands[i] = dsp1->strands[i];
   }
   for (i=0; i<dsp1->numseg; i++)
   {
      lens[i] = dsp1->lens[i];
   }
   j = dsp1->numseg;
   offset = 0;
   if (glen > 0)
   {
      starts[2*j] = gstop1+1;
      starts[2*j+1] = -1;
      lens[j] = glen;
      j += 1;
      offset++;
   }
   if (mlen > 0)
   {
      starts[2*j] = -1;
      if (mstop2 > mstop1)
         starts[2*j+1] = mstop1+1;
      else
         starts[2*j+1] = mstop2+1;
      lens[j] = mlen;
      j += 1;
      offset++;
   }
   j = 2*(dsp1->numseg+offset);
   for (i=0; i<2*(dsp2->numseg); i++, j++)
   {
      starts[j] = dsp2->starts[i];
      strands[j] = dsp2->strands[i];
   }
   j = dsp1->numseg+offset;
   for (i=0; i<dsp2->numseg; i++, j++)
   {
      lens[j] = dsp2->lens[i];
   }
   MemFree(dsp2->starts);
   MemFree(dsp2->strands);
   MemFree(dsp2->lens);
   dsp2->starts = starts;
   dsp2->strands = strands;
   dsp2->lens = lens;
   dsp2->numseg = j;
   SAIndex2Free2(sap2->saip);
   sap2->saip = NULL;
   AlnMgr2IndexSingleChildSeqAlign(sap2);
   return (sap2);
}

/***************************************************************************
*
*  SPI_flip_sa_list takes the head of a list of seqaligns and switches
*  the first and second row of every alignment (alignments should all have
*  two rows). Then, the indexes are freed and the alignments are reindexed.
*
***************************************************************************/
NLM_EXTERN void SPI_flip_sa_list (SeqAlignPtr sap)
{
   DenseSegPtr  dsp;
   Int4         i;
   SeqIdPtr     sip;
   SeqIdPtr     sip_next;
   Int4         tmp_start;
   Uint1        tmp_strand;

   if (sap == NULL || sap->segtype != SAS_DENSEG)
      return;
   while (sap != NULL)
   {
      dsp = (DenseSegPtr)(sap->segs);
      if (dsp->dim == 2) /* skip anything with more than 2 rows */
      {
         /* first switch the ids */
         sip = dsp->ids;
         sip_next = sip->next;
         sip_next->next = sip;
         sip->next = NULL;
         dsp->ids = sip_next;
         /* then switch the starts and strands */
         for (i = 0; i<dsp->numseg; i++)
         {
            tmp_start = dsp->starts[2*i];
            dsp->starts[2*i] = dsp->starts[2*i+1];
            dsp->starts[2*i+1] = tmp_start;
            tmp_strand = dsp->strands[2*i];
            dsp->strands[2*i] = dsp->strands[2*i+1];
            dsp->strands[2*i+1] = tmp_strand;
         }
      }
      if (sap->saip != NULL) /* free indexes, reindex */
      {
         SAIndex2Free2(sap->saip);
         sap->saip = NULL;
         AlnMgr2IndexSingleChildSeqAlign(sap);
      }
      sap = sap->next;
   }
}

/***************************************************************************
*
*  SPI_FillInLastmRNAHoles mimics the logic of SPI_ConnectAln; it
*  goes through a set of alignments and fills in any missing pieces.
*  Its arguments include the mRNA and genomic boundaries of the alignment,
*  so that the function knows how far to extend the set of alignments.
*  When a hole is found, SPI_FindBestAlnByDotPlot is called to fill
*  in the gap.
*
***************************************************************************/
static SeqAlignPtr SPI_FillInLastmRNAHoles(SeqAlignPtr sap, SeqIdPtr sip_genomic, SeqIdPtr sip_mrna, Int4 start_g, Int4 stop_g, Int4 start_m, Int4 stop_m, Uint1 strand)
{
   AMAlignIndex2Ptr  amaip;
   Int4             currstart2;
   Int4             end2;
   Int4             gap1;
   Int4             gap2;
   Int4             i;
   Boolean          internal;
   Int4             prevstop1;
   Int4             prevstop2;
   SeqAlignPtr      sap_new;
   SeqAlignPtr      sap_tmp;
   SeqLocPtr        slp1;
   SeqLocPtr        slp2;
   Int4             start1;
   Int4             start2;
   Int4             stop1;
   Int4             stop2;

   if (sip_genomic == NULL || sip_mrna == NULL)
      return NULL;
   start1 = stop1 = start2 = stop2 = 0;
   if (sap != NULL)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      HeapSort(amaip->saps, amaip->numsaps, sizeof(SeqAlignPtr), SPI_comp_aln_pos);
      prevstop1 = start_g;
      if (strand == Seq_strand_minus)
         prevstop2 = stop_m;
      else
         prevstop2 = start_m;
      internal = FALSE;
      for (i=0; i<amaip->numsaps; i++)
      {
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 1, &start1, &stop1);
         AlnMgr2GetNthSeqRangeInSA(amaip->saps[i], 2, &start2, &stop2);
         if (strand == Seq_strand_minus)
            currstart2 = stop2;
         else
            currstart2 = start2;
         if ((gap2 = spi_isa_gap(currstart2, prevstop2, strand)) >= SPI_TEENYEXON)
         {
            if ((gap1 = spi_isa_gap(start1, prevstop1, Seq_strand_plus)) >= SPI_TEENYEXON || (prevstop1 == -1))
            {
               slp1 = SeqLocIntNew(prevstop1+1, start1-1, Seq_strand_plus, sip_genomic);
               if (strand != Seq_strand_minus)
                  slp2 = SeqLocIntNew(prevstop2+1, currstart2-1, strand, sip_mrna);
               else
                  slp2 = SeqLocIntNew(currstart2+1, prevstop2-1, strand, sip_mrna);
               sap_new = SPI_FindBestAlnByDotPlot(slp1, slp2);
               SeqLocFree(slp1);
               SeqLocFree(slp2);
               sap_tmp = (SeqAlignPtr)(sap->segs);
               while (sap_tmp->next != NULL)
               {
                  sap_tmp = sap_tmp->next;
               }
               sap_tmp->next = sap_new;
            }
         }
         internal = TRUE;
         prevstop1 = stop1;
         if (strand == Seq_strand_minus)
            prevstop2 = start2;
         else
            prevstop2 = stop2;
      }
      if (strand != Seq_strand_minus)
         end2 = stop_m;
      else
      {
         end2 = prevstop2-1;
         prevstop2 = -1;
      }
      gap1 = spi_isa_gap(prevstop1, stop_g, Seq_strand_plus);
      gap2 = spi_isa_gap(end2, prevstop2, strand);
      if (gap1 >= SPI_TEENYEXON && gap2 >= SPI_TEENYEXON)
      {
         slp1 = SeqLocIntNew(prevstop1+1, stop_g, Seq_strand_plus, sip_genomic);
         if (strand == Seq_strand_minus)
            slp2 = SeqLocIntNew(end2, prevstop2+1, strand, sip_mrna);
         else
            slp2 = SeqLocIntNew(prevstop2+1, end2, strand, sip_mrna);
         sap_new = SPI_FindBestAlnByDotPlot(slp1, slp2);
         SeqLocFree(slp1);
         SeqLocFree(slp2);
         sap_tmp = (SeqAlignPtr)(sap->segs);
         while (sap_tmp->next != NULL)
         {
            sap_tmp = sap_tmp->next;
         }
         sap_tmp->next = sap_new;
      }
      sap_tmp = (SeqAlignPtr)(sap->segs);
      i = 0;
      while (sap_tmp != NULL)
      {
         i++;
         sap_tmp = sap_tmp->next;
      }
      amaip->numsaps = i;
      MemFree(amaip->saps);
      amaip->saps = (SeqAlignPtr PNTR)MemNew(i*sizeof(SeqAlignPtr));
      sap_tmp = (SeqAlignPtr)(sap->segs);
      i = 0;
      while (sap_tmp != NULL)
      {
         amaip->saps[i] = sap_tmp;
         i++;
         sap_tmp = sap_tmp->next;
      }
      if (sap == NULL)
         return NULL;
      SPI_RemoveInconsistentAlnsFromSet(sap, SPI_TEENYEXON, 1, SPI_LEFT);
      sap_tmp = (SeqAlignPtr)(sap->segs);
      sap->segs = NULL;
      SeqAlignFree(sap);
      return sap_tmp;
   } else
   {
      slp1 = SeqLocIntNew(start_g, stop_g, Seq_strand_plus, sip_genomic);
      slp2 = SeqLocIntNew(start_m, stop_m, strand, sip_mrna);
      sap_tmp = SPI_FindBestAlnByDotPlot(slp1, slp2);
      SeqLocFree(slp1);
      SeqLocFree(slp2);
      return sap_tmp;
   }
}

/***************************************************************************
*
*  SPI_FindBestAlnByDotPlot is spidey's interface to Fasika Aklilu's
*  tree-based string-matching functions. Given two seqlocs, it sends them
*  to Fasika's function and gets a DOTMainDataPtr in return. The
*  DOTMainDataPtr contains all the information for the hits, so by
*  cycling through this data and copying it into dense-seg seqalign
*  structures, SPI_FindBestAlnByDotPlot builds up a set of alignments
*  that specify the relationship between the two seqlocs. These alignments
*  are then pruned to make a consistent, nonoverlapping set.
*
***************************************************************************/
static SeqAlignPtr SPI_FindBestAlnByDotPlot(SeqLocPtr slp1, SeqLocPtr slp2)
{
   DOTDiagPtr      ddp;
   DenseSegPtr     dsp;
   Int4            i;
   DOTMainDataPtr  mip;
   SeqAlignPtr     sap;
   SeqAlignPtr     sap_head;
   SeqAlignPtr     sap_prev;
   ScorePtr        scp;
   Int4            start1;
   Int4            start2;
   Uint1           strand;

   BioseqPtr      bsp1 = NULL, bsp2 = NULL;
   SeqIdPtr       sidp1 = NULL, sidp2 = NULL;

   /** KSK: this protects spidey from the implicit requirement of DOT_
       that *BOTH* seqs are ncbi2na encoded ****/
   
   if (slp1 != NULL && slp2 != NULL){
       sidp1 = SeqLocId(slp1);
       sidp2 = SeqLocId(slp2);
       if (sidp1 != NULL && sidp2 != NULL){
           bsp1 = BioseqFind(sidp1);
           bsp2 = BioseqFind(sidp2);
           if (bsp1 != NULL && bsp2 != NULL){
               if (bsp1->seq_data_type != Seq_code_ncbi2na
                   || bsp2->seq_data_type !=  Seq_code_ncbi2na){
                   return NULL;
               }
           }
           else {
               return NULL;
           }
       }
       else {
           return NULL;
       }
   }
   else {
       return NULL;
   }
   

   mip = DOT_CreateAndStorebyLoc (slp1, slp2, SPI_TEENYEXON, 10);
   sap = sap_head = sap_prev = NULL;
   if (mip == NULL || mip->hitlist == NULL)
      return NULL;
   i = 0;
   ddp = mip->hitlist[i];
   start1 = SeqLocStart(slp1);
   start2 = SeqLocStart(slp2);
   strand = SeqLocStrand(slp2);
   /* copy each ddp (a single ungapped alignment) into a one-segment dense-seg alignment */
   while (ddp != NULL && i < mip->index)
   {
      ddp = mip->hitlist[i];
      i++;
      sap = SeqAlignNew();
      dsp = DenseSegNew();
      sap->type = SAT_PARTIAL;
      sap->segtype = SAS_DENSEG;
      sap->dim = 2;
      dsp->dim = 2;
      dsp->numseg = 1;
      dsp->ids = SeqIdDup(SeqLocId(slp1));
      dsp->ids->next = SeqIdDup(SeqLocId(slp2));
      dsp->strands = (Uint1Ptr)MemNew(2*sizeof(Uint1));
      dsp->strands[0] = SeqLocStrand(slp1);
      dsp->strands[1] = SeqLocStrand(slp2);
      dsp->starts = (Int4Ptr)MemNew(2*sizeof(Int4));
      dsp->lens = (Int4Ptr)MemNew(sizeof(Int4));
      dsp->starts[0] = ddp->q_start;
      if (dsp->strands[1] == Seq_strand_minus)
         dsp->starts[1] = ddp->s_start - ddp->length + 1;
      else
         dsp->starts[1] = ddp->s_start;
      if (ddp->length > SeqLocLen(slp2))
         dsp->lens[0] = SeqLocLen(slp2);
      else
         dsp->lens[0] = ddp->length - 1;
      scp = ScoreNew();
      scp->id = ObjectIdNew();
      scp->id->str = StringSave("score");
      scp->choice = 1;
      scp->value.intvalue = ddp->score;
      dsp->scores = scp;
      sap->segs = (Pointer)(dsp);
      if (sap_head != NULL)
      {
         sap_prev->next = sap;
         sap_prev = sap;
      } else
         sap_head = sap_prev = sap;
   }
   if (sap_head == NULL)
      return NULL;
   AlnMgr2IndexLite(sap_head);
   SPI_RemoveInconsistentAlnsFromSet(sap_head, SPI_TEENYEXON, 1, SPI_LEFT);
   sap = (SeqAlignPtr)(sap_head->segs);
   sap_head->segs = NULL;
   SeqAlignFree(sap_head);
   MemFree(mip->matrix);
   MemFree(mip->qseq);
   MemFree(mip->sseq);
   MemFree(mip->qname);
   MemFree(mip->sname);
   i = 0;
   while (ddp != NULL && i < mip->index)
   {
      ddp = mip->hitlist[i];
      MemFree(ddp);
      i++;
   }
   MemFree(mip->hitlist);
   return sap;
}

/***************************************************************************
*
*  SPI_comp_aln_pos is the HeapSort callback for SPI_FillInLastmRNAHoles.
*  It compares the genomic intervals covered by two seqaligns, and sorts
*  them according to the 5'-most start position.
*
***************************************************************************/
static int LIBCALLBACK SPI_comp_aln_pos(VoidPtr ptr1, VoidPtr ptr2)
{
   SeqAlignPtr  sap1;
   SeqAlignPtr  sap2;
   Int4         start1;
   Int4         start2;
   Int4         stop1;
   Int4         stop2;

   start1 = start2 = stop1 = stop2 = 0;
   if (ptr1 != NULL && ptr2 != NULL)
   {
      sap1 = *((SeqAlignPtr PNTR) ptr1);
      sap2 = *((SeqAlignPtr PNTR) ptr2);
      AlnMgr2GetNthSeqRangeInSA(sap1, 1, &start1, &stop1);
      AlnMgr2GetNthSeqRangeInSA(sap2, 1, &start2, &stop2);
      if (stop1 < start2)
         return -1;
      else if (stop2 < start1)
         return 1;
      else if (start1 < start2)
         return -1;
      else if (start2 < start1)
         return 1;
      else
         return 0;
  }
  return 0;
}

NLM_EXTERN void SPI_bsinfoFreeList (SPI_bsinfoPtr spi)
{
   SPI_bsinfoPtr  spi_next;

   while (spi != NULL)
   {
      spi_next = spi->next;
      spi->next = NULL;
      SeqLocSetFree(spi->lcaseloc);
      MemFree(spi);
      spi = spi_next;
   }
}

static void SPI_RegionFree (SPI_RegionInfoPtr srip)
{
   if (srip == NULL)
      return;
   if (srip->smp){
       SPI_mRNAFree(srip->smp);
   }
   srip->smp = NULL;
   srip->next = NULL;
   MemFree(srip);
}

NLM_EXTERN void SPI_mRNAFree (SPI_mRNAPtr smp)
{
   AMAlignIndex2Ptr  amaip;
   Int4             i;

   if (smp == NULL)
      return;
   MemFree(smp->exonid);
   MemFree(smp->exongaps);
   MemFree(smp->splicedon);
   MemFree(smp->spliceacc);
   MemFree(smp->mstarts);
   MemFree(smp->mstops);
   MemFree(smp->gstarts);
   MemFree(smp->gstops);
   if (smp->saps != NULL)
   {
      for (i=0; i<smp->numexons; i++)
      {
         SeqAlignFree(smp->saps[i]);
      }
   }
   if (smp->parent != NULL)
   {
      smp->parent->segs = NULL;
      amaip = (AMAlignIndex2Ptr)(smp->parent->saip);
      amaip->saps = NULL;
      SeqAlignFree(smp->parent);
   }
   MemFree(smp->saps);
   if (smp->continuous != NULL)
      SeqAlignFree(smp->continuous);
   if (smp->epp != NULL)
      SPI_FreeExonProfList(smp->epp);
   if (smp->protein!=NULL)
      MemFree(smp->protein);
}

static void SPI_FreeExonProf(SPI_ExonProfPtr epp)
{
   if (epp == NULL)
      return;
   MemFree(epp->mismatches);
   MemFree(epp);
}

static void SPI_FreeExonProfList(SPI_ExonProfPtr epp)
{
   SPI_ExonProfPtr  epp_next;
   
   while (epp != NULL)
   {
      epp_next = epp->next;
      SPI_FreeExonProf(epp);
      epp = epp_next;
   }
}

NLM_EXTERN void SPI_RegionListFree (SPI_RegionInfoPtr srip)
{
   SPI_RegionInfoPtr  srip_tmp;

   if (srip == NULL)
      return;
   while (srip != NULL)
   {
      srip_tmp = srip->next;
      SPI_RegionFree(srip);
      srip = srip_tmp;
   }
}

NLM_EXTERN SPI_OptionsPtr SPI_OptionsNew(void)
{
   SPI_OptionsPtr  spot;

   spot = (SPI_OptionsPtr)MemNew(sizeof(SPI_Options));
   spot->firstpasseval = 0.00001;
   spot->secpasseval = 0.001;
   spot->thirdpasseval = 10;
   spot->organism = SPI_VERTEBRATE;
   spot->numreturns = 1;
   spot->idcutoff = 0;
   spot->lencutoff = 0;
   spot->interspecies = FALSE;
   spot->printaln = FALSE;
   spot->printasn = FALSE;
   /* if strand set to 'unknown' BlastTwoSequences() 
      screws-up returned seqalign strand */
   spot->strand = Seq_strand_both; 
   /* lets have defaults for to & from */
   spot->to = 0;
   spot->from = 0;
   spot->bigintron = 0; 
   spot->bigintron_size = 0;  /* added by KSK*/
   spot->repeat_db_file = 0; /* added by KSK */
   return spot;
}

NLM_EXTERN void SPI_OptionsFree (SPI_OptionsPtr spot)
{
   MemFree(spot);
}

/***************************************************************************
*
*  SPI_GetDonorSpliceInfo fills in the length of the consensus sequence
*  of the donor splice site for the given organism. The boundary is the
*  location of the exon-intron boundary within the consensus sequence.
*
***************************************************************************/
static void SPI_GetDonorSpliceInfo (Int4 org, Int4Ptr spllen, Int4Ptr boundary, SPI_OptionsPtr spot)
{
   Int4               i;
   SPI_SpliceInfoPtr  ssp;

   if (spot->dsplicejunc != 0)
   {
      ssp = spot->dssp_head;
      i = 0;
      while (ssp != NULL)
      {
         i++;
         ssp = ssp->next;
      }
      *spllen = i;
      *boundary = *spllen-spot->dsplicejunc+1;
      return;
   }
   if (org == SPI_VERTEBRATE)
   {
      *spllen = 10;
      *boundary = 8;
   } else if (org == SPI_FLY)
   {
      *spllen = 15;
      *boundary = 11;
   } else if (org == SPI_PLANT)
   {
      *spllen = 9;
      *boundary = 7;
   } else if (org == SPI_CELEGANS)
   {
      *spllen = 15;
      *boundary = 11;
   }
   else if (org == SPI_DICTY){
       *spllen = 8;
       *boundary = 7;
   }
}

/***************************************************************************
*
*  SPI_is_donor is a general interface to the organism-specific donor
*  splice site evaluation functions. It simply passes on the sequence,
*  sequence length, and score pointer to the appropriate organism-
*  specific function.
*  The organism-specific functions all work exactly the same way, but have
*  different splice matrices. They evaluate P(Site|Sequence), which is:
*
*    P(Site|Sequence) = P(Sequence|Site)*P(Site)/P(Sequence)
*
*  Since P(Site) is constant (and unknown), it is ignored; only
*   P(Sequence|Site)/P(Sequence) is calculated, and these values are
*  compared to each other. P(Sequence|Site) is calculated by multiplying
*  the values in the splice site frequency matrix according to the
*  sequence specified. P(Sequence) is the probability of this specific
*  sequence, using the A, T, G, and C frequences specified in the sequence.
*
*  N.B. Ken Katz changed this so that they generate log-odd scores:
*  log[P(X)/F(X)] + log[P(X)/F(X)]....but then generate the antilog
*  since there are too many places in the code where the expected value
*  is the antilog. 
*
***************************************************************************/
NLM_EXTERN void SPI_is_donor (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score, Int4 org)
{
    if (org == SPI_VERTEBRATE){
        SPI_is_donor_vert(sequence, seqlen, score);
    }
    else if (org == SPI_FLY){
        SPI_is_donor_fly(sequence, seqlen, score);
    }
    else if (org == SPI_PLANT){
        SPI_is_donor_plant(sequence, seqlen, score);
    }
    else if (org == SPI_CELEGANS){
        SPI_is_donor_cele(sequence, seqlen, score);
    }
    else if (org == SPI_DICTY){
         SPI_is_donor_dicty(sequence, seqlen, score);
    }
}

static void SPI_is_donor_user(Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score, SPI_OptionsPtr spot)
{
   Int4               acgt[4] = {0, 0, 0, 0};
   Int4               j;
   FloatHi            prob_seqgsite = 0;
   SPI_SpliceInfoPtr  ssp;

   if (sequence == NULL || score == NULL){
       return;
   }

   /* get the frequencies first */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4){
           acgt[sequence[j]]++;
       }
   }
   *score = 0;
   ssp = spot->dssp_head;
   /* now calculate for each base the log, adding values to get the score */
   for (j=0; j<seqlen; j++){
       if (sequence[j] == 0 && ssp->a > 0){
           prob_seqgsite += 
               log10((ssp->a/((FloatHi)acgt[sequence[j]])/seqlen));
       }
       else if (sequence[j] == 1 && ssp->c > 0){
           prob_seqgsite +=
               log10((ssp->c/((FloatHi)acgt[sequence[j]])/seqlen));
       }
       else if (sequence[j] == 2 && ssp->g > 0){
           prob_seqgsite += 
               log10((ssp->g/((FloatHi)acgt[sequence[j]])/seqlen));
       }
       else if (sequence[j] == 3 && ssp->t > 0){
           prob_seqgsite +=
               log10((ssp->t/((FloatHi)acgt[sequence[j]])/seqlen));
       }
       ssp = ssp->next;
   }
   *score = pow(10, prob_seqgsite);
}

/***************************************************************************
*
*  See the comment for SPI_is_donor for an explanation of how this
*  function works. The splice site frequency matrix is derived from
*  a nonredundant set of vertebrate splice sites provided by Chris Burge.
*
***************************************************************************/
static void SPI_is_donor_vert (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score)
{
    Int4     acgt[4] = {0, 0, 0, 0};
    Int4     j = 0;
    FloatHi  d[10][4] = {
        {0.3361, 0.3587, 0.1882, 0.1170},
        {0.5986, 0.1306, 0.1413, 0.1295},
        {0.0867, 0.0321, 0.8034, 0.0778},
        {0.0000, 0.0000, 1.0000, 0.0000},
        {0.0000, 0.0100, 0.0000, 1.0000},
        {0.4976, 0.0267, 0.4507, 0.0249},
        {0.7162, 0.0730, 0.1223, 0.0885},
        {0.0677, 0.0517, 0.8331, 0.0475},
        {0.1586, 0.1681, 0.2185, 0.4549},
        {0.2559, 0.2120, 0.3593, 0.1728}};
    
    FloatHi  prob_seqgsite = 0;
  
    if (sequence == NULL || score == NULL){
      return;
    }
    *score = 0;
    if (seqlen < 10){
        return; 
    }
    prob_seqgsite = 0;
  
    /* first get the freqs */
    for (j=0; j<seqlen; j++){
        if (sequence[j] != 4){
            acgt[sequence[j]]++;
        }
    }
    /* now calculate for each base the log, adding values to get the score */
    for (j=0; j<seqlen; j++){
        if (sequence[j] != 4){
            if (d[j][sequence[j]] > 0){
                prob_seqgsite += 
                    log10((d[j][sequence[j]])/((FloatHi)acgt[sequence[j]]/(FloatHi)seqlen));
            }
        }
    }
    *score = pow(10, prob_seqgsite);
}


/***************************************************************************
*
*  See the comment for SPI_is_donor for an explanation of how this
*  function works. The splice site frequency matrix is derived from
*  a nonredundant set of Drosophila splice sites provided by Chris Burge.
*
***************************************************************************/
static void SPI_is_donor_fly (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score)
{
   Int4     acgt[4] = {0, 0, 0, 0};
   FloatHi  d[15][4] = {
       {0.3103, 0.2105, 0.1951, 0.2834},
       {0.3045, 0.2335, 0.2131, 0.2482},
       {0.3512, 0.2905, 0.2028, 0.1548},
       {0.5374, 0.1523, 0.1567, 0.1529},
       {0.1216, 0.0685, 0.6935, 0.1158},
       {0.0001, 0.0000, 0.9936, 0.0000},
       {0.0000, 0.0000, 0.0000, 0.9878},
       {0.5886, 0.0115, 0.3506, 0.0486},
       {0.7639, 0.0505, 0.1004, 0.0845},
       {0.0480, 0.0102, 0.8861, 0.0550},
       {0.1190, 0.1068, 0.0537, 0.7198},
       {0.3455, 0.1388, 0.1849, 0.3301},
       {0.2700, 0.2258, 0.1804, 0.3231},
       {0.3353, 0.2092, 0.1612, 0.2930},
       {0.2873, 0.2278, 0.1727, 0.3116}};
   Int4     j;
   FloatHi  prob_seqgsite = 0;
  
   if (sequence == NULL || score == NULL){
      return;
   }
   *score = 0;
   if (seqlen < 15){
       return;
   }
   /* first get the freqs */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4){
           acgt[sequence[j]]++;
       }
   }
   /* now calculate for each base the log, adding values to get the score */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4){
           if (d[j][sequence[j]] > 0){
               prob_seqgsite += 
                   log10((d[j][sequence[j]])/((FloatHi)acgt[sequence[j]]/seqlen));
           }
       }
   }
   *score = pow(10, prob_seqgsite);
}

/***************************************************************************
*
*  See the comment for SPI_is_donor for an explanation of how this
*  function works. The splice site frequency matrix is derived from
*  a nonredundant set of Arabidopsis splice sites provided by Chris Burge.
*
***************************************************************************/
static void SPI_is_donor_plant (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score)
{
   Int4     acgt[4] = {0, 0, 0, 0};
   FloatHi  d[9][4] = {
       {0.3563, 0.3526, 0.1840, 0.1068},
       {0.6559, 0.1103, 0.0765, 0.1571},
       {0.0887, 0.0328, 0.7876, 0.0907},
       {0.0001, 0.0000, 0.9930, 0.0000},
       {0.0000, 0.0000, 0.0000, 0.9838},
       {0.6607, 0.0452, 0.1195, 0.1744},
       {0.5407, 0.1394, 0.0546, 0.2650},
       {0.1975, 0.0929, 0.5193, 0.1901},
       {0.2368, 0.1405, 0.1040, 0.5182}};
        
   Int4     j;
   FloatHi  prob_seqgsite = 0;
 
   if (sequence == NULL || score == NULL){
       return;
   }
   *score = 0;
   if (seqlen < 9){
       return;
   }
   /* first get the freqs */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4){
           acgt[sequence[j]]++;
       }
   }
   /* now calculate for each base the log, adding values to get the score */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4){
           if (d[j][sequence[j]] > 0){
               prob_seqgsite += 
                   log10((d[j][sequence[j]])/((FloatHi)acgt[sequence[j]]/seqlen));
           }
       }
   }
   *score = pow(10, prob_seqgsite);
}

/***************************************************************************
*
*  See the comment for SPI_is_donor for an explanation of how this
*  function works. The splice site frequency matrix is derived from
*  a nonredundant set of C. elegans splice sites provided by Chris Burge.
*
***************************************************************************/
static void SPI_is_donor_cele (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score)
{
   Int4     acgt[4] = {0, 0, 0, 0};
   FloatHi  d[15][4] = {
       {0.3575, 0.1537, 0.1605, 0.3284},
       {0.3541, 0.1838, 0.1662, 0.2959},
       {0.3825, 0.2481, 0.1987, 0.1706},
       {0.5792, 0.1445, 0.0955, 0.1808},
       {0.1828, 0.0609, 0.6046, 0.1517},
       {0.0001, 0.0000, 0.9963, 0.0000},
       {0.0000, 0.0000, 0.0000, 0.9919},
       {0.5904, 0.0146, 0.2400, 0.1550},
       {0.6713, 0.0660, 0.0877, 0.1750},
       {0.0904, 0.0457, 0.7441, 0.1198},
       {0.1896, 0.1077, 0.0850, 0.6178},
       {0.2661, 0.0911, 0.1371, 0.5058},
       {0.2620, 0.0995, 0.1344, 0.5041},
       {0.2840, 0.1141, 0.1039, 0.4980},
       {0.2986, 0.1239, 0.1215, 0.4560}};
   Int4     j;
   FloatHi  prob_seqgsite = 0;
   
   if (sequence == NULL || score == NULL){
       return;
   }
   *score = 0;
   if (seqlen < 15){
       return;
   }
   /* first get the freqs */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4){
           acgt[sequence[j]]++;
       }
   }
   /* now calculate for each base the log, adding values to get the score */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4){
           if (d[j][sequence[j]] > 0){
               prob_seqgsite += 
                   log10((d[j][sequence[j]])/((FloatHi)acgt[sequence[j]]/seqlen));
           }
       }
   }
   *score = pow(10, prob_seqgsite);
}

/***************************************************************************
*
*  See the comment for SPI_is_donor for an explanation of how this
*  function works. Note that the Dicty info is NOT corrected for current
*  sequence composition because the log(likehood)matrix is itself corrected
*  for dicty genome composition. The data were retrieved
*  from the geneid Dd parameter file and used  with the permission of 
*  Roderic Guigo. Values were simply translated from log base 2 to log base 10
*
***************************************************************************/
static void SPI_is_donor_dicty (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score){
    
    /*  acgt */ 
    FloatHi  d[8][4] = {
        {0.1825, -0.2014, -0.0136, -0.1440},
        {0.0487, -0.3298,  0.0742, -0.0633},
        {-99999, -99999,   0.6020, -99999},
        {-99999, -99999,    -99999, 0.6020},
        {0.4783, -0.9030, -1.0634, -0.9673},
        {0.3026, -1.4202, -0.7392, -0.0150},
        {-0.3356, -1.3914, 0.8111, -0.5090},
        {-0.7937, -1.0333, -0.5721, 0.4315}};
    Int4 j = 0;

    if (sequence == NULL || score == NULL){
        return;
    }
    *score = 0;
    if (seqlen < 8){
        return;
    }
    *score = 0;
    for (j = 0; j < seqlen; j++){
        if (sequence[j] != 4){
            *score += d[j][sequence[j]];
        }
    }
    *score = pow(10, *score);
}


/***************************************************************************
*
*  SPI_GetAcceptorSpliceInfo fills in the length of the consensus sequence
*  of the acceptor splice site for the given organism. The boundary is the
*  location of the exon-intron boundary within the consensus sequence.
*
***************************************************************************/
static void SPI_GetAcceptorSpliceInfo (Int4 org, Int4Ptr spllen, Int4Ptr boundary, SPI_OptionsPtr spot)
{
   Int4               i;
   SPI_SpliceInfoPtr  ssp;

   if (spot->asplicejunc != 0)
   {
      i = 0;
      ssp = spot->assp_head;
      while (ssp != NULL)
      {
         i++;
         ssp = ssp->next;
      }
      *spllen = i;
      *boundary = spot->asplicejunc+1;
      return;
   }
   if (org == SPI_VERTEBRATE)
   {
      *spllen = 21;
      *boundary = 20;
   } else if (org == SPI_FLY)
   {
      *spllen = 18;
      *boundary = 15;
   } else if (org == SPI_PLANT)
   {
      *spllen = 40;
      *boundary = 36;
   } else if (org == SPI_CELEGANS)
   {
      *spllen = 18;
      *boundary = 15;
   }
   else if (org == SPI_DICTY){
       *spllen = 15;
       *boundary = 15;
   }
}

/***************************************************************************
*
*  SPI_is_acceptor is a general interface to the organism-specific acceptor
*  splice site evaluation functions. It simply passes on the sequence,
*  sequence length, and score pointer to the appropriate organism-
*  specific function.
*  The organism-specific functions all work exactly the same way, but have
*  different splice matrices. They evaluate P(Site|Sequence), which is:
*
*    P(Site|Sequence) = P(Sequence|Site)*P(Site)/P(Sequence)
*
*  Since P(Site) is constant (and unknown), it is ignored; only
*   P(Sequence|Site)/P(Sequence) is calculated, and these values are
*  compared to each other. P(Sequence|Site) is calculated by multiplying
*  the values in the splice site frequency matrix according to the
*  sequence specified. P(Sequence) is the probability of this specific
*  sequence, using the A, T, G, and C frequences specified in the sequence.
*
***************************************************************************/
NLM_EXTERN void SPI_is_acceptor (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score, Int4 org)
{
    if (org == SPI_VERTEBRATE){
        SPI_is_acceptor_vert(sequence, seqlen, score);
    }
    else if (org == SPI_FLY){
        SPI_is_acceptor_fly(sequence, seqlen, score);
    }
    else if (org == SPI_PLANT){
        SPI_is_acceptor_plant(sequence, seqlen, score);
    }
    else if (org == SPI_CELEGANS){
        SPI_is_acceptor_cele(sequence, seqlen, score);
    }
    else if (org == SPI_DICTY){
        SPI_is_acceptor_dicty(sequence, seqlen, score);
    }
}

static void SPI_is_acceptor_user(Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score, SPI_OptionsPtr spot)
{
   Int4               acgt[4] = {0, 0, 0, 0};
   Int4               j;
   FloatHi            prob_seqgsite = 0;
   SPI_SpliceInfoPtr  ssp;

   if (sequence == NULL || score == NULL){
       return;
   }
   /* get the frequencies first */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4){
           acgt[sequence[j]]++;
       }
   }
   *score = 0;
   ssp = spot->assp_head;
   /* now calculate for each base the log, adding values to get the score */
   for (j=0; j<seqlen; j++){
       if (sequence[j] == 0 && ssp->a > 0){
           prob_seqgsite += 
               log10((ssp->a/((FloatHi)acgt[sequence[j]])/seqlen));
       }
       else if (sequence[j] == 1 && ssp->c > 0){
           prob_seqgsite +=
               log10((ssp->c/((FloatHi)acgt[sequence[j]])/seqlen));
       }
       else if (sequence[j] == 2 && ssp->g > 0){
           prob_seqgsite += 
               log10((ssp->g/((FloatHi)acgt[sequence[j]])/seqlen));
       }
       else if (sequence[j] == 3 && ssp->t > 0){
           prob_seqgsite +=
               log10((ssp->t/((FloatHi)acgt[sequence[j]])/seqlen));
       }
       ssp = ssp->next;
   }
   *score = pow(10, prob_seqgsite);
}

/***************************************************************************
*
*  See the comment for SPI_is_acceptor for an explanation of how this
*  function works. The splice site frequency matrix is derived from
*  a nonredundant set of vertebrate splice sites provided by Chris Burge.
*
***************************************************************************/
static void SPI_is_acceptor_vert (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score)
{
   FloatHi  a[21][4] = {
       {0.1823, 0.3135, 0.1485, 0.3557},
       {0.1568, 0.3319, 0.1681, 0.3432},
       {0.1461, 0.3379, 0.1520, 0.3640},
       {0.1271, 0.3290, 0.1710, 0.3729},
       {0.1342, 0.3593, 0.1366, 0.3700},
       {0.1152, 0.3676, 0.1188, 0.3985},
       {0.0926, 0.3688, 0.1235, 0.4151},
       {0.0879, 0.3426, 0.1205, 0.4489},
       {0.0808, 0.3557, 0.1182, 0.4454},
       {0.0790, 0.3224, 0.1128, 0.4857},
       {0.0748, 0.3581, 0.1075, 0.4596},
       {0.0814, 0.3866, 0.1152, 0.4169},
       {0.0849, 0.4186, 0.1235, 0.3729},
       {0.0867, 0.4240, 0.0849, 0.4044},
       {0.0665, 0.4561, 0.0618, 0.4157},
       {0.0736, 0.3996, 0.0564, 0.4703},
       {0.2251, 0.3409, 0.2126, 0.2215},
       {0.0404, 0.7357, 0.0018, 0.2221},
       {1.0000, 0.0010, 0.0010, 0.0010},
       {0.0010, 0.0010, 1.0000, 0.0010},
       {0.2375, 0.1318, 0.5350, 0.0956}};
   Int4        acgt[4] = {0, 0, 0, 0};
   Int4        j;
   FloatHi     prob_seqgsite = 0;
  

   if (sequence == NULL || score == NULL){
       return;
   }
   *score = 0;
   if (seqlen < 21){
       return;
   }
   /* first get the freqs */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4){
           acgt[sequence[j]]++;
       }
   }
   /* now calculate for each base the log, adding values to get the score */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4 && a[j][sequence[j]] > 0 ){
           prob_seqgsite +=
               log10((a[j][sequence[j]])/((FloatHi)acgt[sequence[j]]/seqlen));
       }
   }
   *score = pow(10, prob_seqgsite);
   /* if (sequence[18] == 0 && sequence[19] == 2){
     *score += 0.5;
   }
   */
}

/***************************************************************************
*
*  See the comment for SPI_is_acceptor for an explanation of how this
*  function works. The splice site frequency matrix is derived from
*  a nonredundant set of Drosophila splice sites provided by Chris Burge.
*
***************************************************************************/
static void SPI_is_acceptor_fly (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score)
{
    FloatHi  a[18][4] = {
        {0.2497, 0.2446, 0.1044, 0.4014},
        {0.2132, 0.2369, 0.1063, 0.4437},
        {0.1946, 0.2196, 0.1082, 0.4776},
        {0.2170, 0.2017, 0.0973, 0.4840},
        {0.1946, 0.2170, 0.0858, 0.5026},
        {0.2004, 0.2433, 0.0858, 0.4706},
        {0.2004, 0.2727, 0.0967, 0.4302},
        {0.2106, 0.2708, 0.0864, 0.4321},
        {0.1876, 0.3035, 0.0608, 0.4481},
        {0.1114, 0.2522, 0.0679, 0.5685},
        {0.1178, 0.2164, 0.0461, 0.6197},
        {0.2830, 0.1639, 0.2913, 0.2618},
        {0.0467, 0.7049, 0.0045, 0.2439},
        {0.9923, 0.0032, 0.0013, 0.0032},
        {0.0032, 0.0038, 0.9910, 0.0019},
        {0.3073, 0.1997, 0.3675, 0.1255},
        {0.2260, 0.1927, 0.1709, 0.4104},
        {0.2574, 0.2855, 0.2279, 0.2292}};
    Int4        acgt[4] = {0, 0, 0, 0};
    Int4        j;
    FloatHi     prob_seqgsite = 0;
    
    if (sequence == NULL || score == NULL){
        return;
    }
    *score = 0;
    if (seqlen < 18){
        return;
    }
    /* first get the freqs */
    for (j=0; j<seqlen; j++){
        if (sequence[j] != 4){
            acgt[sequence[j]]++;
        }
    }
    /* now calculate for each base the log, adding values to get the score */
    for (j=0; j<seqlen; j++){
        if (sequence[j] != 4 && a[j][sequence[j]] > 0){
            prob_seqgsite +=
                log10((a[j][sequence[j]])/((FloatHi)acgt[sequence[j]]/seqlen));
        }
    }
    *score = pow(10, prob_seqgsite);
    /* if (sequence[12] == 0 && sequence[13] == 2)
       *score += 0.5;
       */
}

/***************************************************************************
*
*  See the comment for SPI_is_acceptor for an explanation of how this
*  function works. The splice site frequency matrix is derived from
*  a nonredundant set of Arabidopsis splice sites provided by Chris Burge.
*
***************************************************************************/
static void SPI_is_acceptor_plant (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score)
{
   FloatHi  a[40][4] = {
       {0.2959, 0.1512, 0.1632, 0.3896},
       {0.2845, 0.1490, 0.1648, 0.4017},
       {0.2660, 0.1528, 0.1742, 0.4071},
       {0.2843, 0.1346, 0.1744, 0.4067},
       {0.2714, 0.1512, 0.1624, 0.4150},
       {0.2806, 0.1451, 0.1661, 0.4082},
       {0.2753, 0.1486, 0.1650, 0.4111},
       {0.2753, 0.1460, 0.1532, 0.4255},
       {0.2775, 0.1497, 0.1648, 0.4080},
       {0.2898, 0.1429, 0.1543, 0.4130},
       {0.2793, 0.1486, 0.1545, 0.4174},
       {0.2834, 0.1429, 0.1576, 0.4161},
       {0.2725, 0.1471, 0.1517, 0.4285},
       {0.2614, 0.1521, 0.1519, 0.4347},
       {0.2515, 0.1497, 0.1639, 0.4347},
       {0.2408, 0.1460, 0.1619, 0.4513},
       {0.2266, 0.1431, 0.1652, 0.4650},
       {0.2218, 0.1403, 0.1639, 0.4738},
       {0.2122, 0.1292, 0.1661, 0.4926},
       {0.1886, 0.1460, 0.1694, 0.4961},
       {0.1919, 0.1368, 0.1711, 0.5002},
       {0.1921, 0.1375, 0.1641, 0.5063},
       {0.1838, 0.1331, 0.1558, 0.5273},
       {0.1809, 0.1307, 0.1622, 0.5260},
       {0.1694, 0.1364, 0.1761, 0.5181},
       {0.2177, 0.1357, 0.1864, 0.4602},
       {0.2109, 0.1388, 0.1552, 0.4952},
       {0.2150, 0.1300, 0.1538, 0.5011},
       {0.1989, 0.1252, 0.1766, 0.4993},
       {0.1849, 0.1407, 0.1464, 0.5280},
       {0.1554, 0.0997, 0.1069, 0.6381},
       {0.2664, 0.0846, 0.3851, 0.2640},
       {0.0597, 0.6512, 0.0026, 0.2863},
       {0.9937, 0.0017, 0.0024, 0.0022},
       {0.0022, 0.0042, 0.9921, 0.0015},
       {0.2367, 0.0968, 0.5553, 0.1112},
       {0.2281, 0.1534, 0.1766, 0.4419},
       {0.2957, 0.1438, 0.2218, 0.3387},
       {0.2614, 0.1923, 0.2904, 0.2559},
       {0.2950, 0.1777, 0.2205, 0.3068}};
   Int4        acgt[4] = {0, 0, 0, 0};
   Int4        j;
   FloatHi     prob_seqgsite = 0;

   if (sequence == NULL || score == NULL){
       return;
   }
   *score = 0;
   if (seqlen < 40){
       return;
   }
   /* first get the freqs */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4){
           acgt[sequence[j]]++;
       }
   }
   /* now calculate for each base the log, adding values to get the score */
   for (j=0; j<seqlen; j++){
       if (sequence[j] != 4 && a[j][sequence[j]] > 0){
           prob_seqgsite += 
               log10((a[j][sequence[j]])/((FloatHi)acgt[sequence[j]]/seqlen));
       }
   }
   *score = pow(10, prob_seqgsite);
   /* if (sequence[33] == 0 && sequence[34] == 2)
       *score += 0.5;
       */
}

/***************************************************************************
*
*  See the comment for SPI_is_acceptor for an explanation of how this
*  function works. The splice site frequency matrix is derived from
*  a nonredundant set of C. elegans splice sites provided by Chris Burge.
*
***************************************************************************/
static void SPI_is_acceptor_cele (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score)
{
    FloatHi  a[18][4] = {
        {0.4365, 0.1293, 0.0650, 0.3689},
        {0.3719, 0.1415, 0.0826, 0.4037},
        {0.3550, 0.1374, 0.0883, 0.4190},
        {0.3428, 0.1418, 0.0910, 0.4240},
        {0.3465, 0.1499, 0.0711, 0.4321},
        {0.3594, 0.1492, 0.0707, 0.4203},
        {0.3976, 0.1191, 0.0728, 0.4102},
        {0.4139, 0.0795, 0.0687, 0.4376},
        {0.2812, 0.0799, 0.0690, 0.5695},
        {0.0589, 0.0379, 0.0156, 0.8873},
        {0.0102, 0.0132, 0.0047, 0.9716},
        {0.0975, 0.1391, 0.0917, 0.6714},
        {0.0321, 0.8257, 0.0020, 0.1398},
        {0.9953, 0.0010, 0.0017, 0.0017},
        {0.0020, 0.0020, 0.9946, 0.0010},
        {0.3990, 0.1553, 0.3154, 0.1299},
        {0.2995, 0.1780, 0.1628, 0.3594},
        {0.2975, 0.2288, 0.1878, 0.2856}};
    Int4        acgt[4] = {0, 0, 0, 0};
    Int4        j;
    FloatHi     prob_seqgsite = 0;
    
    if (sequence == NULL || score == NULL){
        return;
    }
    *score = 0;
    if (seqlen < 18){
        return;
    }
    /* first get the freqs */
    for (j=0; j<seqlen; j++){
        if (sequence[j] != 4){
            acgt[sequence[j]]++;
        }
    }
    /* now calculate for each base the log, adding values to get the score */
    for (j=0; j<seqlen; j++){
        if (sequence[j] != 4 && a[j][sequence[j]] > 0){
            prob_seqgsite += 
                log10((a[j][sequence[j]])/((FloatHi)acgt[sequence[j]]/seqlen));
        }
    }
    *score = pow(10, prob_seqgsite);
    /* if (sequence[13] == 0 && sequence[14] == 2)
        *score += 0.5;
        */
}


/***************************************************************************
*
*  See the comment for SPI_is_acceptor for an explanation of how this
*  function works. Note that the Dicty info is NOT corrected for current
*  sequence composition because the log(likehood)matrix itself corrected
*  for dicty genome composition.  The data were retrieved
*  from the geneid Dd parameter file and used  with the permission of 
*  Roderic Guigo.Values were simply translated from log base 2 to log base 10
*
***************************************************************************/
static void SPI_is_acceptor_dicty (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score){

    /*  acgt */ 
    Int4 j = 0;
    FloatHi  a[15][4] = {
        {-0.2171, -0.4463, -0.9154, 0.2974},
        {-0.0984, -0.3965, -1.3635, 0.2574},
        {0.0201, -0.5770, -1.5528, 0.3159},
        {-0.0880, -0.6470, -1.0716, 0.2993},
        {-0.0483, -0.4077, -0.8955, 0.2326},
        {0.1091, -0.4041, -0.9030, 0.2262},
        {0.0672, -0.3973, -1.0649, 0.1682},
        {0.0592, -0.5731, -1.0634, 0.1480},
        {0.1707, -0.9122, -1.0757, 0.1658},
        {0.0343, -0.6659, -1.4012, 0.2365},
        {0.1407, -0.4903, -1.3757, 0.0521},
        {0.1901,-0.7647,-0.9314, 0.1395},
        {-0.2300, -0.7210, -0.9030, 0.4531},
        {0.6020, -9999, -9999, -9999},
        {-9999,-9999, 0.6020, -9999}};
    
    if (sequence == NULL || score == NULL){
        return;
    }
    *score = 0;
    if (seqlen < 15){
        return;
    }
    for (j = 0; j < seqlen; j++){
        if (sequence[j] != 4){
            *score += a[j][sequence[j]];
        }
    }
    *score = pow(10, *score);
    /* if (sequence[13] == 0 && sequence[14] == 2){
      *score += 0.5;
    }
    */
}



/***************************************************************************
*
*  SPI_RemoveConflictsAmongPieces looks at all the alignments for all
*  the fragments and removes overlapping alignment sets. The alignment set
*  with the greatest sequence range will be kept. The function cycles
*  through all the fragments, and if a fragment has an alignment, the
*  range of that alignment is compared with the range of all subsequent
*  fragments' alignments, and if there's an overlap, one of the overlapping
*  alignment sets is deleted. While not the most efficient design, this
*  function works well because most overlaps get deleted early and because
*  the searches are done with repeat masking, reducing the number of
*  overlaps.
*
***************************************************************************/
static void SPI_RemoveConflictsAmongPieces(SPI_FragHerdPtr sfhp, Int4 fuzz)
{
   Boolean  conflict;
   Boolean  done;
   Int4     i;
   Int4     j;
   Int4     len1;
   Int4     len2;
   Int4     start1;
   Int4     start2;
   Int4     stop1;
   Int4     stop2;

   i = 0;
   while (i<sfhp->numfrags)
   {
      if (sfhp->sfparray[i]->sap != NULL)
      {
         SPI_GetNthSeqRangeInSASet(sfhp->sfparray[i]->sap, 2, &start1, &stop1);
         done = FALSE;
         for (j=i+1; j<sfhp->numfrags && !done; j++)
         {
            if (sfhp->sfparray[j]->sap != NULL)
            {
               SPI_GetNthSeqRangeInSASet(sfhp->sfparray[j]->sap, 2, &start2, &stop2);
               conflict = FALSE;
               if (start2 > start1 && stop2 < stop1)
                  conflict = TRUE;
               else if (stop2 > start1 + fuzz && start2 < start1 + fuzz)
                  conflict = TRUE;
               else if (start2 < stop1 - fuzz && stop2 > stop1 - fuzz)
                  conflict = TRUE;
               else if (start2 < start1 && stop2 > stop1)
                  conflict = TRUE;
               if (conflict)
               {
                  len1 = stop1 - start1 + 1;
                  len2 = stop2 - start2 + 1;
                  if (len2 > len1)
                  {
                     done = TRUE;
                     SeqAlignSetFree(sfhp->sfparray[i]->sap);
                     sfhp->sfparray[i]->sap = NULL;
                  } else if (len1 >= len2)
                  {
                     SeqAlignSetFree(sfhp->sfparray[j]->sap);
                     sfhp->sfparray[j]->sap = NULL;
                  }
               }
            }
         }
      }
      i++;
   }
}

/***************************************************************************
*
*  SPI_OrderPieces sorts the fragments according to their alignment
*  position (position on the mRNA sequence) as well as by their original
*  fragment order (lgroup, group, and order). Fragments without
*  alignments are placed at the beginning of the set. At the end of the
*  sort, all the initial fragments have no alignments and all the fragments
*  at the end are in order along the mRNA sequence, making filling in the
*  holes in the alignment much easier.
*
***************************************************************************/
static void SPI_OrderPieces(SPI_FragHerdPtr sfhp, BioseqPtr bsp_mrna)
{
   Int4             i;
   Int4             j;
   SPI_FragInfoPtr  sfi;
   SPI_FragInfoPtr  PNTR sfi_array;
   SPI_FragPtr      sfp;
   SPI_FragPtr      PNTR sfp_array;
   
   if (sfhp == NULL || bsp_mrna == NULL)
      return;
   sfi_array = (SPI_FragInfoPtr PNTR)MemNew((sfhp->numfrags)*sizeof(SPI_FragInfo));
   for (i=0; i<sfhp->numfrags; i++)
   {
      sfi = (SPI_FragInfoPtr)MemNew(sizeof(SPI_FragInfo));
      sfp = sfhp->sfparray[i];
      if (sfp->sap != NULL)
      {
         if (sfp->sap->saip == NULL)
            AlnMgr2IndexLite(sfp->sap);
         SPI_GetNthSeqRangeInSASet(sfp->sap, 2, &sfi->mrnastart, &sfi->mrnastop);
      } else
         sfi->mrnastart = sfi->mrnastop = -1;
      sfi->sfpnum = i;
      sfi->position_orig = sfp->position_orig;
      sfi->fragnum = sfp->fragnum;
      sfi_array[i] = sfi;
   }
   HeapSort(sfi_array, i, sizeof(SPI_FragInfoPtr), SPI_CompareFragInfo);
   j = 0;
   for (i=0; i<sfhp->numfrags; i++)
   {
      if (sfi_array[i]->mrnastart != -1)
      {
         sfhp->sfparray[sfi_array[i]->sfpnum]->position_mrna = j;
         j++;
      } else
         sfhp->sfparray[sfi_array[i]->sfpnum]->position_mrna = -1;
   }
   sfp_array = (SPI_FragPtr PNTR)MemNew((sfhp->numfrags)*sizeof(SPI_FragPtr));
   for (i=0; i<sfhp->numfrags; i++)
   {
      sfp_array[i] = sfhp->sfparray[sfi_array[i]->sfpnum];
   }
   MemFree(sfhp->sfparray);
   sfhp->sfparray = sfp_array;
   for (i=0; i<sfhp->numfrags; i++)
   {
      MemFree(sfi_array[i]);
   }
   MemFree(sfi_array);
}

/***************************************************************************
*
*  SPI_CompareFragInfo is the HeapSort callback for SPI_OrderPieces. It
*  compares the alignments of two fragments and puts the fragment that
*  is most 5' on the mRNA first. Fragments without alignments are put in
*  their original order, before all the fragments with alignments. If two
*  fragments have the same mRNA position, they are sorted secondarily by
*  their original fragment position.
*
***************************************************************************/
static int LIBCALLBACK SPI_CompareFragInfo(VoidPtr ptr1, VoidPtr ptr2)
{
   SPI_FragInfoPtr  sfi1;
   SPI_FragInfoPtr  sfi2;

   if (ptr1 != NULL && ptr2 != NULL)
   {
      sfi1 = *((SPI_FragInfoPtr PNTR)ptr1);
      sfi2 = *((SPI_FragInfoPtr PNTR)ptr2);
      /* this function orders by mRNA position, secondarily by original position */
      if (sfi1->mrnastart != -1 && sfi2->mrnastart != -1)
      {
         if (sfi1->mrnastart < sfi2->mrnastart)
            return -1;
         else if (sfi1->mrnastart > sfi2->mrnastart)
            return 1;
         else if (sfi1->mrnastop > sfi2->mrnastop)
            return -1;
         else if (sfi1->mrnastop < sfi2->mrnastop)
            return 1;
         else
            return 0;
      }
      /* put things with no mRNA order first */
      if (sfi1->mrnastart != -1 && sfi2->mrnastart == -1)
         return 1;
      if (sfi1->mrnastart == -1 && sfi2->mrnastart != -1)
         return -1;
      if (sfi1->position_orig->lgroup != 0 && sfi1->position_orig->lgroup == sfi2->position_orig->lgroup)
      {
         if (sfi1->position_orig->group < sfi2->position_orig->group)
            return -1;
         else if (sfi1->position_orig->group > sfi2->position_orig->group)
            return 1;
         else
         {
            if (sfi1->position_orig->order < sfi2->position_orig->order)
               return -1;
            else
               return 1;
         }
      }
      /* if fragments are in the same group, keep them in order */
      if (sfi1->position_orig->group == sfi2->position_orig->group)
      {
         if (sfi1->position_orig->order < sfi2->position_orig->order)
            return -1;
         else
            return 1;
      }
      if (sfi1->position_orig->group < sfi2->position_orig->group)
         return -1;
      else if (sfi2->position_orig->group > sfi1->position_orig->group)
         return 1;
      if (sfi1->fragnum < sfi2->fragnum)
         return -1;
      else
         return 1;
   }
   return 0;
}

/***************************************************************************
*
*  SPI_ConnectAlnPieces is analogous to SPI_ConnectAln for finished
*  sequence; it fills in the gaps in the mRNA-to-draft alignment. Since
*  the genomic sequence is in fragments, the job is a little trickier
*  here. The function first calls SPI_ConnectAln on each set of
*  alignments for each fragment, to fill in internal gaps between those
*  alignments. Then the alignment sets are all examined (they should not
*  overlap at this point, but they usually have gaps between them) and any
*  gaps between the alignment sets are filled in by first searching in
*  the fragments containing alignments adjacent to the gaps, then by
*  looking in all "nearby" fragments (as defined by SPI_GetNearbyFrags), and
*  finally by looking in all fragments. Since many of the spidey functions
*  assume that the genomic sequence is always the plus strand, and the
*  draft sequence functions all deal with alignments on the plus strand
*  of the mRNA and either strand of the genomic sequence, there are many
*  places in this function where the strands of an alignment must be
*  reversed before and after a function call if the alignment is on the
*  minus strand of the genomic sequence.
*
***************************************************************************/
static Boolean SPI_ConnectAlnPieces(SPI_FragHerdPtr sfhp, BioseqPtr bsp_contig, BioseqPtr bsp_mrna, SPI_OptionsPtr spot)
{
   AMAlignIndex2Ptr      amaip;
   Int4                 c;
   Int4                 curr;
   Boolean              done;
   Boolean              found;
   Int4                 gapsize;
   Int4                 i;
   Int4                 j;
   Boolean              minus;
   Int4                 n;
   BLAST_OptionsBlkPtr  options;
   Int4                 orderedstart;
   Int4                 prevstart;
   SeqAlignPtr          salp;
   SeqAlignPtr          salp_tmp;
   SeqAlignPtr          salp_prev;
   SeqAlignPtr          sap;
   SeqAlignPtr          sap_b1;
   SeqAlignPtr          sap_b2;
   SeqAlignPtr          sap_new1;
   SeqAlignPtr          sap_new2;
   SeqAlignPtr          sap_tmp;
   SPI_FragPtr          sfp;
   SPI_FragPtr          sfpcurr;
   SPI_FragPtr          PNTR sfpnearby;
   SPI_FragPtr          sfpprev;
   SeqLocPtr            slp_gen;
   SeqLocPtr            slp_mrna;
   Int4                 start_b;
   Int4                 start1;
   Int4                 start2;
   Int4                 start3;
   Int4                 start4;
   Int4                 stop_b;
   Int4                 stop1;
   Int4                 stop2;
   Int4                 stop3;
   Int4                 stop4;
   Uint1                strand;
   SPI_FragPtr          PNTR tmparray;
   
   i = 0;
   orderedstart = -1;
   /* figure out which sequences have ordering information (by mRNA position) so far */
   while (orderedstart == -1 && i < sfhp->numfrags)
   {
      if (sfhp->sfparray[i]->position_mrna != -1)
         orderedstart = i;
      i++;
   }
   if (orderedstart == -1) /* no fragment has alignments */
      return FALSE;
   /* fill in internal gaps for each contig-to-mRNA alignment */
   for (i=orderedstart; i<sfhp->numfrags; i++)
   {
      if (sfhp->sfparray[i]->sap != NULL)
      {
         if ((AlnMgr2GetNthStrand(sfhp->sfparray[i]->sap, 1)) == Seq_strand_minus)
         {
            minus = TRUE;
            salp = (SeqAlignPtr)(sfhp->sfparray[i]->sap->segs);
            while (salp != NULL)
            {
               salp_tmp = salp->next;
               salp->next = NULL;
               SAIndex2Free2(salp->saip);
               salp->saip = NULL;
               SeqAlignListReverseStrand(salp);
               AlnMgr2IndexSingleChildSeqAlign(salp);
               salp->next = salp_tmp;
               salp = salp_tmp;
            }
         } else
            minus = FALSE;
         if (!SPI_ConnectAln(sfhp->sfparray[i]->sap, spot, NULL, FALSE, TRUE))
         {
            SeqAlignSetFree(sfhp->sfparray[i]->sap);
            sfhp->sfparray[i]->sap = NULL;
         }
         if (minus && sfhp->sfparray[i]->sap != NULL)
         {
            salp = (SeqAlignPtr)(sfhp->sfparray[i]->sap);
            while (salp != NULL)
            {
               salp_tmp = salp->next;
               salp->next = NULL;
               SAIndex2Free2(salp->saip);
               salp->saip = NULL;
               SeqAlignListReverseStrand(salp);
               AlnMgr2IndexSingleChildSeqAlign(salp);
               salp->next = salp_tmp;
               salp = salp_tmp;
            }
         }
      }
      if (i != sfhp->numfrags-1)
         sfhp->sfparray[i]->next = sfhp->sfparray[i+1];
   }
   tmparray = (SPI_FragPtr PNTR)MemNew((sfhp->numfrags)*sizeof(SPI_FragPtr));
   prevstart = -1;
   done = FALSE;
   sfpcurr = sfhp->sfparray[orderedstart];
   sfpprev = NULL;
   curr = orderedstart;
   start3 = stop3 = -1;
   sfpnearby = NULL;
   /* fill in gaps between contig alignments */
   while (!done)
   {
      sap = sfpcurr->sap;
      if (sap != NULL)
      {
         SPI_GetNthSeqRangeInSASet(sap, 2, &start2, &stop2);
         if ((gapsize = spi_isa_gap(start2, prevstart, Seq_strand_plus)) >= SPI_TEENYEXON)
         {
            /* first look in the same piece and the ones that are supposed to be adjacent */
            strand = AlnMgr2GetNthStrand(sfpcurr->sap, 1);
            if (strand == Seq_strand_minus)
            {
               minus = TRUE;
               salp = (SeqAlignPtr)(sfpcurr->sap->segs);
               while (salp != NULL)
               {
                  salp_tmp = salp->next;
                  salp->next = NULL;
                  SAIndex2Free2(salp->saip);
                  salp->saip = NULL;
                  SeqAlignListReverseStrand(salp);
                  AlnMgr2IndexSingleChildSeqAlign(salp);
                  salp->next = salp_tmp;
                  salp = salp_tmp;
               }   
            } else
               minus = FALSE;
            SPI_GetNthSeqRangeInSASet(sfpcurr->sap, 1, &start1, &stop1);
            sap_new1 = sap_new2 = NULL;
            if (!minus)
               sap_new1 = SPI_FillInIntron(bsp_contig->id, bsp_mrna->id, sfpcurr->start, start1, prevstart, start2, Seq_strand_minus, spot);
            else
               sap_new1 = SPI_FillInIntron(bsp_contig->id, bsp_mrna->id, sfpcurr->start, start1, start2, prevstart, Seq_strand_plus, spot);
            if (sap_new1 != NULL)
            {
               SPI_GetNthSeqRangeInSASet(sap_new1, 2, &start2, &stop2);
               sap_new1->next = (SeqAlignPtr)(sfpcurr->sap->segs);
               sfpcurr->sap->segs = (Pointer)(sap_new1);
               AlnMgr2ReIndexSeqAlign(sfpcurr->sap);
               SPI_RemoveInconsistentAlnsFromSet(sfpcurr->sap, SPI_TEENYEXON, 2, SPI_LEFT);
            }
            if ((spi_isa_gap(start2, prevstart, Seq_strand_plus)) > SPI_TEENYEXON)
            /* look in fragments in the same group or lgroup, */
            /* up to the ones that already have hits */
            {
               if (sfpnearby != NULL)
               {
                  MemFree(sfpnearby);
                  sfpnearby = NULL;
               }
               j = SPI_GetNearbyFrags(sfpcurr, curr, &sfpnearby, sfhp, minus);
               found = FALSE;
               for (n=0; n<j && !found; n++)
               {
                  if (sfpnearby[n]->sap != NULL)
                  {
                     found = TRUE;
                     strand = AlnMgr2GetNthStrand(sfpnearby[n]->sap, 1);
                     if (strand == Seq_strand_minus)
                     {
                        salp = (SeqAlignPtr)(sfpnearby[n]->sap->segs);
                        while (salp != NULL)
                        {
                           salp_tmp = salp->next;
                           salp->next = NULL;
                           SAIndex2Free2(salp->saip);
                           salp->saip = NULL;
                           SeqAlignListReverseStrand(salp);
                           AlnMgr2IndexSingleChildSeqAlign(salp);
                           salp->next = salp_tmp;
                           salp = salp_tmp;
                        }
                        SPI_GetNthSeqRangeInSASet(sfpnearby[n]->sap, 1, &start3, &stop3);
                        SPI_GetNthSeqRangeInSASet(sfpnearby[n]->sap, 2, &start4, &stop4);
                        sap_new1 = SPI_FillInIntron(bsp_contig->id, bsp_mrna->id, sfpnearby[n]->start, start4, stop4, start2, strand, spot);
                        if (sap_new1 != NULL)
                        {
                           sap_new1->next = (SeqAlignPtr)(sfpnearby[n]->sap->segs);
                           sfpnearby[n]->sap->segs = (Pointer)sap_new1;
                           AMAlignIndex2Free2(sfpnearby[n]->sap->saip);
                           sfpnearby[n]->sap->saip = NULL;
                           AlnMgr2IndexLite(sfpnearby[n]->sap);
                           SPI_RemoveInconsistentAlnsFromSet(sfpnearby[n]->sap, SPI_TEENYEXON, 2, SPI_LEFT);
                        }
                        salp = (SeqAlignPtr)(sfpnearby[n]->sap->segs);
                        while (salp != NULL)
                        {
                           salp_tmp = salp->next;
                           salp->next = NULL;
                           SAIndex2Free2(salp->saip);
                           salp->saip = NULL;
                           SeqAlignListReverseStrand(salp);
                           AlnMgr2IndexSingleChildSeqAlign(salp);
                           salp->next = salp_tmp;
                           salp = salp_tmp;
                        }
                     } else
                     {
                        SPI_GetNthSeqRangeInSASet(sfpnearby[n]->sap, 1, &start3, &stop3);
                        SPI_GetNthSeqRangeInSASet(sfpnearby[n]->sap, 2, &start4, &stop4);
                        sap_new1 = SPI_FillInIntron(bsp_contig->id, bsp_mrna->id, stop4, sfpnearby[n]->stop, stop3, start2, Seq_strand_plus, spot);
                        if (sap_new1 != NULL)
                        {
                           sap_new1->next = (SeqAlignPtr)(sfpnearby[n]->sap->segs);
                           sfpnearby[n]->sap->segs = (Pointer)sap_new1;
                           AMAlignIndex2Free2(sfpnearby[n]->sap->saip);
                           sfpnearby[n]->sap->saip = NULL;
                           AlnMgr2IndexLite(sfpnearby[n]->sap);
                           SPI_RemoveInconsistentAlnsFromSet(sfpnearby[n]->sap, SPI_TEENYEXON, 2, SPI_LEFT);
                        }
                     }
                  } else
                  {
                     sap_new1 = SPI_FillInIntron(bsp_contig->id, bsp_mrna->id, sfpnearby[n]->start, sfpnearby[n]->stop, prevstart, start2, Seq_strand_plus, spot);
                     if (sap_new1 != NULL)
                     {
                        sap_tmp = sap_new1;
                        while (sap_tmp->next != NULL)
                        {
                           sap_tmp = sap_tmp->next;
                        }
                        sap_tmp->next = SPI_FillInIntron(bsp_contig->id, bsp_mrna->id, sfpnearby[n]->start, sfpnearby[n]->stop, start2, prevstart, Seq_strand_minus, spot);
                     } else
                        sap_new1 = SPI_FillInIntron(bsp_contig->id, bsp_mrna->id, sfpnearby[n]->start, sfpnearby[n]->stop, start2, prevstart, Seq_strand_minus, spot);
                     if (sap_new1 != NULL)
                     {
                        AMAlignIndex2Free2(sap_new1->saip);
                        sap_new1->saip = NULL;
                        AlnMgr2IndexLite(sap_new1);
                        sfpnearby[n]->sap = sap_new1;
                        SPI_RemoveInconsistentAlnsFromSet(sfpnearby[n]->sap, SPI_TEENYEXON, 2, SPI_LEFT);
                     }
                  }
               }
            }
            SPI_CleanupAndGetNewmRNARange(sfpnearby, j, &start3, &stop3);
            if (start3 != -1 && stop3 != -1)
            {
               start2 = start3;
               stop2 = stop3;
            }
            MemFree(sfpnearby);
            if ((spi_isa_gap(start2, prevstart, Seq_strand_plus)) > SPI_MINBLASTSIZE + 2)
            /* now look in all the fragments that don't have hits yet */
            {
               slp_mrna = SeqLocIntNew(prevstart+1, start2-1, Seq_strand_plus, bsp_mrna->id);
               slp_gen = SeqLocIntNew(spot->from, spot->to, Seq_strand_plus, bsp_contig->id);
               options = BLASTOptionNew("blastn", TRUE);
               options->wordsize = 7;
               options->filter_string = StringSave("m L");
               options->expect_value = spot->secpasseval;
               options->query_lcase_mask = spot->lcaseloc;
               if (spot->interspecies)
               {
                  options->gap_x_dropoff_final = 100;
                  options->gap_open = 5;
                  options->gap_extend = 1;
                  options->penalty = -1;
               }
               sap_b1 = BlastTwoSequencesByLoc(slp_mrna, slp_gen, "blastn", options);
               BLASTOptionDelete(options);
               SeqLocFree(slp_gen);
               slp_gen = SeqLocIntNew(spot->from, spot->to, Seq_strand_minus, bsp_contig->id);
               options = BLASTOptionNew("blastn", TRUE);
               options->wordsize = 7;
               options->filter_string = StringSave("m L");
               options->expect_value = spot->secpasseval;
               options->query_lcase_mask = spot->lcaseloc;
               if (spot->interspecies)
               {
                  options->gap_x_dropoff_final = 100;
                  options->gap_open = 5;
                  options->gap_extend = 1;
                  options->penalty = -1;
               }
               sap_b2 = BlastTwoSequencesByLoc(slp_mrna, slp_gen, "blastn", options);
               BLASTOptionDelete(options);
               SeqAlignListReverseStrand(sap_b2);
               SeqLocFree(slp_gen);
               SeqLocFree(slp_mrna);
               if (sap_b1 != NULL)
               {
                  sap_tmp = sap_b1;
                  while (sap_tmp->next != NULL)
                  {
                     sap_tmp = sap_tmp->next;
                  }
                  sap_tmp->next = sap_b2;
               } else
                  sap_b1 = sap_b2;
               SPI_flip_sa_list(sap_b1);
               if (sap_b1 != NULL)
               {
                  AlnMgr2SortAlnSetByNthRowPos(sap_b1, 1);
                  c = 0;
                  amaip = (AMAlignIndex2Ptr)(sap_b1->saip);
                  AlnMgr2GetNthSeqRangeInSA(amaip->saps[0], 1, &start_b, &stop_b);
                  for (i=0; i<orderedstart && c<amaip->numsaps; i++)
                  {
                     salp_tmp = salp_prev = NULL;
                     sfp = sfhp->sfparray[i];
                     while (sfp->start <= start_b && sfp->stop >= start_b && c<amaip->numsaps)
                     {
                        if (salp_tmp == NULL)
                           salp_tmp = salp_prev = SeqAlignDup(amaip->saps[c]);
                        else
                        {
                           salp_prev->next = SeqAlignDup(amaip->saps[c]);
                           salp_prev = salp_prev->next;
                        }
                        c++;
                        if (c<amaip->numsaps)
                           AlnMgr2GetNthSeqRangeInSA(amaip->saps[c], 1, &start_b, &stop_b);
                     }
                     if (salp_tmp != NULL)
                     {
                        AlnMgr2IndexLite(salp_tmp);
                        SPI_RemoveInconsistentAlnsFromSet(salp_tmp, SPI_FUZZ, 2, SPI_LEFT);
                        SeqAlignSetFree(sfp->sap);
                        sfp->sap = salp_tmp;
                        if (!SPI_ConnectAln(sfp->sap, spot, NULL, FALSE, TRUE))
                           return FALSE;
                        /* change all alignments to be on the plus strand of the mRNA */
                        strand = AlnMgr2GetNthStrand((SeqAlignPtr)(salp_tmp->segs), 2);
                        if (strand == Seq_strand_minus)
                           SeqAlignListReverseStrand((SeqAlignPtr)(salp_tmp->segs));
                     }
                  }
               }
               for (i=0; i<orderedstart; i++)
               {
                  if (sfhp->sfparray[i]->sap != NULL)
                     j++;
               }
               sfpnearby = (SPI_FragPtr PNTR)MemNew(j*sizeof(SPI_FragPtr));
               j = 0;
               for (i=0; i<orderedstart; i++)
               {
                  if (sfhp->sfparray[i]->sap != NULL)
                  {
                     sfpnearby[j] = sfhp->sfparray[i];
                     j++;
                  }
               }
               SPI_CleanupAndGetNewmRNARange(sfpnearby, j, &start3, &stop3);
            }
         }
         prevstart = stop2;
      }
      sfpprev = sfpcurr;
      curr++;
      if (curr == sfhp->numfrags)
         done = TRUE;
      else
         sfpcurr = sfhp->sfparray[curr];
   }
   return TRUE;
}

/***************************************************************************
*
*  SPI_CleanupAndGetNewmRNARange looks through all alignment sets of a
*  group of fragments and removes overlapping alignment sets. Once the
*  group of fragments is consistent, SPI_CleanupAndGetNewmRNARange gets
*  the range of the mRNA sequence covered by all alignment sets of the
*  fragment group.
*
***************************************************************************/
static void SPI_CleanupAndGetNewmRNARange(SPI_FragPtr PNTR sfpnearby, Int4 n, Int4Ptr start, Int4Ptr stop)
{
   Boolean      conflict;
   Boolean      done;
   Int4         i;
   Int4         len1;
   Int4         len2;
   Int4         numconsistent;
   Int4         numsaps1;
   Int4         numsaps2;
   SPI_FragPtr  sfp;
   SPI_FragPtr  sfp_head;
   SPI_FragPtr  sfp_prev;
   Int4         start_m;
   Int4         start_m1;
   Int4         stop_m;
   Int4         stop_m1;
   Int4         tmpstart;
   Int4         tmpstop;
   
   numconsistent = 0;
   sfp_head = sfp_prev = NULL;
   for (i=0; i<n; i++) /* first make the set self-consistent by removing overlapping */
   {                   /* sets of alignments among the fragments                     */
      sfpnearby[i]->next = NULL;
      sfp = sfp_head;
      if (sfpnearby[i]->sap != NULL)
      {
         SPI_GetNthSeqRangeInSASet(sfpnearby[i]->sap, 2, &start_m, &stop_m);
         done = FALSE;
         conflict = FALSE;
         while (sfp != NULL && !done)
         {
            SPI_GetNthSeqRangeInSASet(sfp->sap, 2, &start_m1, &stop_m1);
            if (start_m1 > start_m && stop_m1 < stop_m)
               conflict = TRUE;
            else if (stop_m1 > start_m + SPI_TEENYEXON && start_m1 < start_m)
               conflict = TRUE;
            else if (start_m1 < stop_m - SPI_TEENYEXON && stop_m1 > stop_m)
               conflict = TRUE;
            else if (start_m1 < start_m && stop_m1 > stop_m)
               conflict = TRUE;
            if (conflict == TRUE) /* keep the longer of the two alignment sets */
            {
               done = TRUE;
               len1 = SPI_GetNthSeqLenInSASet(sfpnearby[i]->sap, 2, &numsaps1);
               len2 = SPI_GetNthSeqLenInSASet(sfp->sap, 2, &numsaps2);
               if (len1 > len2)
               {
                  SeqAlignSetFree(sfp->sap);
                  sfp->sap = NULL;
                  /* new sfp takes the place of the conflicting one */
                  if (sfp_prev != NULL)
                  {
                     sfpnearby[i]->next = sfp_prev->next;
                     sfp_prev->next = sfpnearby[i];
                  } else
                  {
                     sfpnearby[i]->next = sfp_head->next;
                     sfp_head = sfpnearby[i];
                  }
               } else /* new one gets its seqalign deleted */
               {
                  SeqAlignSetFree(sfpnearby[i]->sap);
                  sfpnearby[i]->sap = NULL;
               }
            } else
            {
               sfp_prev = sfp;
               sfp = sfp->next;
            }
         }
         if (!conflict) /* add the new one to the list */
         {
            sfpnearby[i]->next = sfp_head;
            sfp_head = sfpnearby[i];
            numconsistent++;
         }
      }
   }
   /* then get the start and stop of the mRNA across the set */
   if (numconsistent == 0) /* shouldn't ever happen! */
   {
      *start = -1;
      *stop = -1;
   } else
   {
      sfp = sfp_head;
      *start = -1;
      *stop = -1;
      while (sfp != NULL)
      {
         if (sfp->sap != NULL)
         {
            SPI_GetNthSeqRangeInSASet(sfp->sap, 2, &tmpstart, &tmpstop);
            if (tmpstart < *start || *start == -1)
               *start = tmpstart;
            if (tmpstop > *stop)
               *stop = tmpstop;
         }
         sfp = sfp->next;
      }
      sfpnearby[0] = sfp_head;
   }
}

/***************************************************************************
*
*  SPI_GetNearbyFrags takes a fragment herd, a fragment which is the 
*  target (n is the number of the target fragment in the herd) and a
*  SPI_FragPtr **, and fills in the ptrptr with a linked list of fragments
*  that are in the same group or lgroup as the target fragment. These
*  fragments are supposedly near the target fragment in the genomic
*  sequence, and should be searched first for pieces missing from the
*  alignment in the target fragment.
*
***************************************************************************/
static Int4 SPI_GetNearbyFrags(SPI_FragPtr sfptarget, Int4 n, SPI_FragPtr ** ptrptr, SPI_FragHerdPtr sfhp, Boolean minus)
{
   Boolean      found;
   Int4         i;
   Int4         j;
   SPI_FragPtr  sfp;
   SPI_FragPtr  sfp_curr;
   SPI_FragPtr  sfp_head;
   SPI_FragPtr  sfp_prev;
   SPI_FragPtr  PNTR sfpnearby;

   j = 0;
   sfp_head = NULL;
   for (i=0; i<sfhp->numfrags; i++)
   {
      if (i != n)
      {
         sfp = sfhp->sfparray[i];
         sfp->next = NULL;
         if (sfp->position_orig->lgroup == sfptarget->position_orig->lgroup)
         {
            if (!minus)
            {
               if (sfp->position_orig->group < sfptarget->position_orig->group && sfp->position_orig->lgroup != 0)
               {
                  j++;
                  sfp_prev = NULL;
                  if (sfp_head == NULL)
                     sfp_head = sfp;
                  else
                  {
                     sfp_prev = NULL;
                     sfp_curr = sfp_head;
                     found = FALSE;
                     while (sfp_curr != NULL && !found)
                     {
                        if (sfp->position_orig->group > sfp_curr->position_orig->group || (sfp->position_orig->group == sfp_curr->position_orig->group && sfp->position_orig->order > sfp_curr->position_orig->order))
                           found = TRUE;
                        else
                        {
                           sfp_prev = sfp_curr;
                           sfp_curr = sfp_curr->next;
                        }
                     }
                     if (sfp_prev != NULL)
                     {
                        sfp->next = sfp_prev->next;
                        sfp_prev->next = sfp;
                     } else
                     {
                        sfp->next = sfp_head;
                        sfp_head = sfp;
                     }
                  }
               } else if (sfp->position_orig->group == sfptarget->position_orig->group && sfp->position_orig->order < sfptarget->position_orig->order)
               {
                  j++;
                  if (sfp_head == NULL)
                     sfp_head = sfp;
                  else
                  {
                     sfp_prev = NULL;
                     sfp_curr = sfp_head;
                     found = FALSE;
                     while (sfp_curr != NULL && !found)
                     {
                        if (sfp->position_orig->group > sfp_curr->position_orig->group || (sfp->position_orig->group == sfp_curr->position_orig->group && sfp->position_orig->order > sfp_curr->position_orig->order))
                           found = TRUE;
                        else
                        {
                           sfp_prev = sfp_curr;
                           sfp_curr = sfp_curr->next;
                        }
                     }
                     if (sfp_prev != NULL)
                     {
                        sfp->next = sfp_prev->next;
                        sfp_prev->next = sfp;
                     } else
                     {
                        sfp->next = sfp_head;
                        sfp_head = sfp;
                     }
                  }
               }
            } else
            {
               if (sfp->position_orig->group > sfptarget->position_orig->group && sfp->position_orig->lgroup != 0)
               {
                  j++;
                  sfp_prev = NULL;
                  if (sfp_head == NULL)
                     sfp_head = sfp;
                  else
                  {
                     sfp_prev = NULL;
                     sfp_curr = sfp_head;
                     found = FALSE;
                     while (sfp_curr != NULL && !found)
                     {
                        if (sfp->position_orig->group < sfp_curr->position_orig->group || (sfp->position_orig->group == sfp_curr->position_orig->group && sfp->position_orig->order < sfp_curr->position_orig->order))
                           found = TRUE;
                        else
                        {
                           sfp_prev = sfp_curr;
                           sfp_curr = sfp_curr->next;
                        }
                     }
                     if (sfp_prev != NULL)
                     {
                        sfp->next = sfp_prev->next;
                        sfp_prev->next = sfp;
                     } else
                     {
                        sfp->next = sfp_head;
                        sfp_head = sfp;
                     }
                  }
               } else if (sfp->position_orig->group == sfptarget->position_orig->group && sfp->position_orig->order > sfptarget->position_orig->order)
               {
                  j++;
                  if (sfp_head == NULL)
                     sfp_head = sfp;
                  else
                  {
                     sfp_prev = NULL;
                     sfp_curr = sfp_head;
                     found = FALSE;
                     while (sfp_curr != NULL && !found)
                     {
                        if (sfp->position_orig->group < sfp_curr->position_orig->group || (sfp->position_orig->group == sfp_curr->position_orig->group && sfp->position_orig->order < sfp_curr->position_orig->order))
                           found = TRUE;
                        else
                        {
                           sfp_prev = sfp_curr;
                           sfp_curr = sfp_curr->next;
                        }
                     }
                     if (sfp_prev != NULL)
                     {
                        sfp->next = sfp_prev->next;
                        sfp_prev->next = sfp;
                     } else
                     {
                        sfp->next = sfp_head;
                        sfp_head = sfp;
                     }
                  }
               }
            }
         }
      }
   }
   if (j == 0)
      return 0;
   sfpnearby = (SPI_FragPtr PNTR)MemNew(j*sizeof(SPI_FragPtr));
   for (i=0, sfp = sfp_head; i<j && sfp!=NULL; i++, sfp = sfp->next)
   {
      sfpnearby[i] = sfp;
   }
   *ptrptr = sfpnearby;
   return j;
}

/***************************************************************************
*
*  SPI_AdjustSplicesInPieces first calls SPI_AdjustForSplice on each
*  fragment's alignment set (first reversing those fragment's alignments
*  that are on the minus strand of the genomic sequence, as the draft
*  functions expect the mRNA to be on the plus strand but the finished
*  functions expect the genomic sequence to be on the plus strand). Next,
*  it calls SPI_AdjustEndsOfPieces for each adjacent pair of fragments;
*  this function adjusts the initial and terminal exons of the adjacent
*  pieces so that they abut exactly on the mRNA and they are next to
*  acceptable splice sites.
*
***************************************************************************/
static void SPI_AdjustSplicesInPieces(SPI_FragHerdPtr sfhp, BioseqPtr bsp_genomic, SPI_OptionsPtr spot)
{
   Int4               i;
   Int4               j;
   Boolean            minus;
   SeqAlignPtr        salp;
   SeqAlignPtr        salp_tmp;
   SPI_mRNAPtr        smp;
   SPI_RegionInfoPtr  srip;
   Uint1              tmp_acc;
   Uint1              tmp_don;
   
   /* first use standard functions to adjust internal splices */
   srip = (SPI_RegionInfoPtr)MemNew(sizeof(SPI_RegionInfo));
   for (i=0; i<sfhp->numfrags; i++)
   {
      if (sfhp->sfparray[i]->sap != NULL && ((SeqAlignPtr)(sfhp->sfparray[i]->sap->segs))->next != NULL)
      {
         salp_tmp = (SeqAlignPtr)(sfhp->sfparray[i]->sap->segs);
         srip->strand = AlnMgr2GetNthStrand(salp_tmp, 1);
         if (srip->strand == Seq_strand_minus)
         {
            minus = TRUE;
            salp = (SeqAlignPtr)(sfhp->sfparray[i]->sap->segs);
            while (salp != NULL)
            {
               salp_tmp = salp->next;
               salp->next = NULL;
               SAIndex2Free2(salp->saip);
               salp->saip = NULL;
               SeqAlignListReverseStrand(salp);
               AlnMgr2IndexSingleChildSeqAlign(salp);
               salp->next = salp_tmp;
               salp = salp_tmp;
            }
         } else
            minus = FALSE;
         if (sfhp->sfparray[i]->sap->saip == NULL)
            AlnMgr2IndexLite(sfhp->sfparray[i]->sap);
         smp = SPI_AdjustForSplice(sfhp->sfparray[i]->sap, spot, srip);
         sfhp->sfparray[i]->smp = smp;
         if (srip->strand == Seq_strand_minus) /* the exons will be in the wrong order now */
         {
            for (j=0; j<smp->numexons/2; j++)
            {
               tmp_don = smp->splicedon[smp->numexons-j-1];
               smp->splicedon[smp->numexons-j-1] = smp->splicedon[j];
               smp->splicedon[j] = tmp_don;
               tmp_acc = smp->spliceacc[smp->numexons-j-1];
               smp->spliceacc[smp->numexons-j-1] = smp->spliceacc[j];
               smp->spliceacc[j] = tmp_acc;
            }
            salp = (SeqAlignPtr)(sfhp->sfparray[i]->sap->segs);
            while (salp != NULL)
            {
               salp_tmp = salp->next;
               salp->next = NULL;
               SAIndex2Free2(salp->saip);
               salp->saip = NULL;
               SeqAlignListReverseStrand(salp);
               AlnMgr2IndexSingleChildSeqAlign(salp);
               salp->next = salp_tmp;
               salp = salp_tmp;
            }
         }
      }
   }
   MemFree(srip);
   /* now adjust the splice sites between fragments */
   for (i=0; i<sfhp->numfrags-1; i++)
   {
      if (sfhp->sfparray[i]->sap != NULL)
      {
         j = i+1;
         while (j<sfhp->numfrags && sfhp->sfparray[j]->sap == NULL)
         {
            j++;
         }
         if (sfhp->sfparray[j]->sap != NULL)
            SPI_AdjustEndsOfPieces(sfhp->sfparray[i], sfhp->sfparray[j], bsp_genomic, spot);
      }
   }
}

/***************************************************************************
*
*  SPI_AdjustEndsOfPieces takes the last exon in the alignment of sfp1 and
*  the first exon in the alignment of sfp2 and adjusts the boundaries so
*  that the two exons abut exactly on the mRNA (if possible -- if a piece
*  is missing, both exon boundaries are separately adjusted to good splice
*  sites) and so that they are adjacent to good splice sites. After
*  getting the possible splice sites, SPI_AdjustEndsOfPieces looks through
*  the sites to determine which is the highest-scoring site that changes
*  the alignments the least. If no pieces are missing (continuous is TRUE)
*  then both alignments are truncated or extended to the splice site; if 
*  continuous is FALSE, the second alignment is adjusted separately to a 
*  good acceptor site that changes the alignment the least.
*
***************************************************************************/
static void SPI_AdjustEndsOfPieces(SPI_FragPtr sfp1, SPI_FragPtr sfp2, BioseqPtr bsp_genomic, SPI_OptionsPtr spot)
{
   Boolean         continuous;
   Int4            f;
   SPI_FragSplPtr  fsp1;
   SPI_FragSplPtr  fsp2;
   Int4            i;
   FloatHi         maxsc;
   Int4            offset;
   Int4            ovl;
   Int4            pos;
   SeqAlignPtr     sap1;
   SeqAlignPtr     sap2;
   Int4            start1;
   Int4            start2;
   Int4            stop1;
   Int4            stop2;
   Uint1           strand1;
   Uint1           strand2;
   
   sap1 = SPI_GetNthSAByRow(sfp1->sap, 2, -1);
   sap2 = SPI_GetNthSAByRow(sfp2->sap, 2, 1);
   AlnMgr2GetNthSeqRangeInSA(sap1, 2, &start1, &stop1);
   AlnMgr2GetNthSeqRangeInSA(sap2, 2, &start2, &stop2);
   strand1 = AlnMgr2GetNthStrand(sap1, 1);
   strand2 = AlnMgr2GetNthStrand(sap2, 1);
   fsp1 = NULL;
   fsp2 = NULL;
   if (start2 - stop1 <= SPI_TEENYEXON) /* make mRNA continuous, nonoverlapping */
   {
      if (start2 - stop1 < 0)
         ovl = stop1 - start2;
      else
         ovl = start2 - stop1;
      if (ovl < SPI_TEENYEXON)
         ovl = SPI_TEENYEXON;
      fsp1 = SPI_GetPossibleSites(sap1, bsp_genomic, spot, TRUE, ovl);
      continuous = TRUE;
   } else /* just adjust ends to good splice sites, don't worry about continuity */
   {
      ovl = SPI_FUZZ;
      fsp1 = SPI_GetPossibleSites(sap1, bsp_genomic, spot, TRUE, SPI_FUZZ);
      fsp2 = SPI_GetPossibleSites(sap2, bsp_genomic, spot, FALSE, SPI_FUZZ);
      continuous = FALSE;
   }
   maxsc = 0;
   for (f=0; f<SPI_NUMSITES; f++)
   {
      pos = stop1 - ovl + fsp1->splarray[f].i + fsp1->spllen - fsp1->boundary;
      if (stop1 - pos < 0)
         fsp1->splarray[f].diff = pos - stop1;
      else
         fsp1->splarray[f].diff = stop1 - pos;
      if (continuous)
      {
         if (start2 - pos < 0)
         {
            if (pos - start2 > fsp1->splarray[f].diff)
               fsp1->splarray[f].diff = pos - start2;
         } else
         {
            if (start2 - pos > fsp1->splarray[f].diff)
               fsp1->splarray[f].diff = start2 - pos;
         }
      }
      if (pos - start1 <= SPI_TEENYEXON)
      {
         fsp1->splarray[f].score = 0;
         fsp1->splarray[f].diff = -1;
      }
      if (fsp1->splarray[f].diff > maxsc)
         maxsc = fsp1->splarray[f].diff;
   }
   offset = ovl - fsp1->spllen + fsp1->boundary;
   i = 0;
   for (f=0; f<SPI_NUMSITES; f++)
   {
      if (fsp1->splarray[f].diff <= maxsc && fsp1->splarray[f].score > 0 && fsp1->splarray[f].diff >= 0)
      {
         maxsc = fsp1->splarray[f].diff;
         offset = fsp1->splarray[f].i;
         i = f;
      }
   }
   if (fsp1->splarray[i].score >= 0.00001)
      sfp1->donor = 1;
   else  /* if don't find a good site, don't change the alignment */
      offset = ovl - fsp1->spllen + fsp1->boundary;
   pos = stop1 - ovl + offset + fsp1->spllen - fsp1->boundary;
   if (strand1 == Seq_strand_minus)
   {
      sap1->next = NULL;
      SAIndex2Free2(sap1->saip);
      sap1->saip = NULL;
      SeqAlignListReverseStrand(sap1);
      AlnMgr2IndexSingleChildSeqAlign(sap1);
      if (pos < stop1)
      {
         if (AlnMgr2TruncateSeqAlign(sap1, start1, pos, 2))
         {
            sap1->next->next = NULL;
            SeqAlignFree(sap1->next);
            sap1->next = NULL;
         }
      } else if (pos > stop1)
         SPI_AddToAln(sap1, pos - stop1, SPI_LEFT, strand1);
      sap1->next = NULL;
      SAIndex2Free2(sap1->saip);
      sap1->saip = NULL;
      SeqAlignListReverseStrand(sap1);
      AlnMgr2IndexSingleChildSeqAlign(sap1);
   } else
   {
      if (pos < stop1)
      {
         if (AlnMgr2TruncateSeqAlign(sap1, start1, pos, 2))
         {
            sap1->next->next = NULL;
            SeqAlignFree(sap1->next);
            sap1->next = NULL;
         }
      } else if (pos > stop1)
         SPI_AddToAln(sap1, pos - stop1, SPI_RIGHT, strand1);
   }
   if (!continuous) /* find a decent acceptor site among the ones returned */
   {
      maxsc = 0;
      for (f=0; f<SPI_NUMSITES; f++)
      {
         pos = start2 - ovl + fsp2->splarray[f].i + fsp2->spllen - fsp2->boundary;
         if (start2 - pos < 0)
            fsp1->splarray[f].diff = pos - start2;
         else
            fsp1->splarray[f].diff = start2 - pos;
         if (pos - stop2 <= SPI_TEENYEXON || stop2 - pos <= SPI_TEENYEXON)
         {
            fsp2->splarray[f].score = 0;
            fsp2->splarray[f].diff = -1;
         }
         if (fsp2->splarray[f].diff > maxsc)
            maxsc = fsp2->splarray[f].diff;
      }
      offset = ovl - fsp2->spllen + fsp2->boundary;
      i = 0;
      for (f=0; f<SPI_NUMSITES; f++)
      {
         if (fsp2->splarray[f].diff <= maxsc && fsp2->splarray[f].score > 0 && fsp2->splarray[f].diff >= 0)
         {
            maxsc = fsp2->splarray[f].diff;
            offset = fsp2->splarray[f].i;
            i = f;
         }
      }
      if (fsp2->splarray[i].score >= 0.0000002)
         sfp2->acceptor = 1;
      else  /* if don't find a good site, don't change the alignment */
         offset = ovl - fsp2->spllen + fsp2->boundary;
      pos = start2 - ovl + offset + fsp2->spllen - fsp2->boundary;
   }
   if (strand2 == Seq_strand_minus)
   {
      sap2->next = NULL;
      SAIndex2Free2(sap2->saip);
      sap2->saip = NULL;
      SeqAlignListReverseStrand(sap2);
      AlnMgr2IndexSingleChildSeqAlign(sap2);
      if (start2 < pos + 1)
      {
         if (AlnMgr2TruncateSeqAlign(sap2, pos+1, stop2, 2))
         {
            sap2->next->next = NULL;
            SeqAlignFree(sap2->next);
            sap2->next = NULL;
         }
      } else if (start2 > pos + 1)
         SPI_AddToAln(sap2, start2-pos-1, SPI_RIGHT, strand2);
      sap2->next = NULL;
      SAIndex2Free2(sap2->saip);
      sap2->saip = NULL;
      SeqAlignListReverseStrand(sap2);
      AlnMgr2IndexSingleChildSeqAlign(sap2);
   } else
   {
      if (start2 < pos + 1)
      {
         if (AlnMgr2TruncateSeqAlign(sap2, pos+1, stop2, 2))
         {
            sap2->next->next = NULL;
            SeqAlignFree(sap2->next);
            sap2->next = NULL;
         }
      } else if (start2 > pos + 1)
         SPI_AddToAln(sap2, start2-pos-1, SPI_LEFT, strand2);
   }
   if (continuous) /* check to see whether current breakpoint has a good acceptor site */
   {
      fsp2 = SPI_GetPossibleSites(sap2, bsp_genomic, spot, FALSE, 0);
      if (fsp2->splarray[0].score >= 0.0000002)
         sfp2->acceptor = 1;
   }
   SPI_FragSplFree(fsp1);
   SPI_FragSplFree(fsp2);
}

/***************************************************************************
*
*  SPI_GetNthSAByRow is a useful utility function that sorts a set
*  of alignments by position on the 'row'th row and then retrieves the
*  nth of those alignments. If n is -1, the last alignment is
*  retrieved.
*
***************************************************************************/
static SeqAlignPtr SPI_GetNthSAByRow(SeqAlignPtr sap, Int4 row, Int4 n)
{
/* n = 1 is first alignment, n = -1 is last alignment */
   AMAlignIndex2Ptr  amaip;
   Int4             i;
   SeqAlignPtr      sap_place;
   SeqAlignPtr      PNTR saparray;
   SeqAlignPtr      PNTR saparray_tmp;

   if (sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return NULL;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   if (n > amaip->numsaps)
      return NULL;
   saparray = (SeqAlignPtr PNTR)MemNew(amaip->numsaps*sizeof(SeqAlignPtr));
   saparray_tmp = amaip->saps;
   for (i=0; i<amaip->numsaps; i++)
   {
      saparray[i] = amaip->saps[i];
   }
   amaip->saps = saparray;
   AlnMgr2SortAlnSetByNthRowPos(sap, row);
   if (n > 0)
      sap_place = amaip->saps[n-1];
   else
      sap_place = amaip->saps[amaip->numsaps-1];
   amaip->saps = saparray_tmp;
   MemFree(saparray);
   return sap_place;
}

/***************************************************************************
*
*  SPI_GetPossibleSites returns the SPI_NUMSITES best donor or acceptor
*  splice sites for an exon (defined by an alignment), within a range
*  defined by the variable ovl. First, the donor or acceptor site
*  consensus length and position of the splice junction is retrieved
*  for the appropriate organism. Then, the interval around the 5' or 3'
*  end of the alignment (dictated by whether the site is a donor (5') or
*  acceptor (3') site) is examined and the SPI_NUMSITES best sites are
*  stored in the SPI_FragSpl structure and returned.
*
***************************************************************************/
static SPI_FragSplPtr SPI_GetPossibleSites(SeqAlignPtr sap, BioseqPtr bsp_genomic, SPI_OptionsPtr spot, Boolean donor, Int4 ovl)
{
   Int4            boundary;
   Uint1Ptr        buf;
   Int4            c;
   Int4            f;
   SPI_FragSplPtr  fsp;
   Int4            i;
   FloatHi         maxsc;
   Uint1           res;
   FloatHi         score;
   SPI_SplicePtr   splarray;
   Int4            spllen;
   SeqPortPtr      spp;
   Int4            start;
   Int4            stop;
   Uint1           strand;
   
   strand = AlnMgr2GetNthStrand(sap, 1);
   fsp = (SPI_FragSplPtr)MemNew(sizeof(SPI_FragSpl));
   AlnMgr2GetNthSeqRangeInSA(sap, 1, &start, &stop);
   if (donor)
      SPI_GetDonorSpliceInfo(spot->organism, &spllen, &boundary, spot);
   else
      SPI_GetAcceptorSpliceInfo(spot->organism, &spllen, &boundary, spot);
   if (strand != Seq_strand_minus)
   {
      if (donor)
         spp = SeqPortNew(bsp_genomic, stop-ovl, stop+ovl+spllen, strand, Seq_code_ncbi4na);
      else
         spp = SeqPortNew(bsp_genomic, start-ovl-spllen, start+ovl, strand, Seq_code_ncbi4na);
   } else
   {
      if (donor)
         spp = SeqPortNew(bsp_genomic, start-ovl-spllen, start+ovl, strand, Seq_code_ncbi4na);
      else
         spp = SeqPortNew(bsp_genomic, stop-ovl, stop+ovl+spllen, strand, Seq_code_ncbi4na);
   }
   i = 0;
   buf = (Uint1Ptr)MemNew((2*ovl+spllen+(spllen-boundary))*sizeof(Uint1));
   splarray = (SPI_SplicePtr)MemNew(SPI_NUMSITES*sizeof(SPI_Splice));
   for (f=0; f<SPI_NUMSITES; f++)
   {
      splarray[f].i = 0;
      splarray[f].score = -2;
   }
   while (((res = SeqPortGetResidue(spp)) != SEQPORT_EOF) && i<(2*ovl+1+spllen))
   {
      if (res == 1)
         buf[i] = 0;
      else if (res == 2)
         buf[i] = 1;
      else if (res == 4)
         buf[i] = 2;
      else if (res == 8)
         buf[i] = 3;
      else
         buf[i] = 4;
      i++;
   }
   SeqPortFree(spp);
   for (i=0; i<2*ovl+(spllen-boundary); i++)
   {
      if (donor)
         SPI_is_donor(buf+i, spllen, &score, spot->organism);
      else
         SPI_is_acceptor(buf+i, spllen, &score, spot->organism);
      c = 0;
      if (score > 0.000001)
      {
         for (f=0; f<SPI_NUMSITES; f++)
         {
            if (f == 0)
               maxsc = splarray[f].score;
            else if (splarray[f].score < maxsc)
            {
               maxsc = splarray[f].score;
               c = f;
            }
         }
         if (score > splarray[c].score)
         {
            splarray[c].score = score;
            splarray[c].i = i;
         }
      }
   }
   MemFree(buf);
   fsp->splarray = splarray;
   fsp->spllen = spllen;
   fsp->boundary = boundary;
   return fsp;
}

static void SPI_FragSplFree(SPI_FragSplPtr fsp)
{   
   if (fsp == NULL)
      return;
   MemFree(fsp->splarray);
   MemFree(fsp);
}

static SeqLocPtr SPI_SeqLocListDup(SeqLocPtr slp)
{
   SeqLocPtr slp_new;

   if (slp == NULL)
      return NULL;
   slp_new = (SeqLocPtr)AsnIoMemCopy((Pointer)slp, (AsnReadFunc)SeqLocAsnRead, (AsnWriteFunc)SeqLocAsnWrite);
   return slp_new;
}

/***************************************************************************
*
*  SPI_RemoveInconsistentAlnsFromSet is a greedy algorithm that first
*  sorts the alignments by score, then takes the highest-scoring 
*  alignment and compares it to the next-highest-scoring alignment, which
*  is deleted if it is contained; on subsequent loops each next-highest-
*  scoring alignment is compared to the set of alignments that have
*  been kept. The alignments can be sorted along the first or
*  second sequence; the alignments will be reversed so that they are
*  all on the plus strand of the sequence to be examined.
*  The input alignment must be indexed at least at the LITE level; 
*  conflicting child alignments will be deleted, not hidden, by this
*  function.  This function assumes that all children have the same two
*  rows. The 'compact' parameter tells the function whether to try to
*  keep alignments that are more to the left in genomic coordinates, or
*  more to the right.
*
***************************************************************************/
NLM_EXTERN void SPI_RemoveInconsistentAlnsFromSet(SeqAlignPtr sap, Int4 fuzz, Int4 n, Int4 compact)
{
   AMAlignIndex2Ptr  amaip;
   Boolean          conflict;
   Int4             curr;
   Int4             i;
   Int4             indextype;
   SeqAlignPtr      salp;
   SeqAlignPtr      salp_head;
   SeqAlignPtr      salp_prev;
   SPI_nPtr         PNTR spin;
   Int4             start;
   Int4             stop;
   Int4             strand;

   if (sap == NULL || sap->saip == NULL || sap->saip->indextype != INDEX_PARENT)
      return;
   if (n > 2)
      return;
   amaip = (AMAlignIndex2Ptr)(sap->saip);
   indextype = amaip->alnstyle;
   /* make sure that everything is on the plus strand of the nth sequence */
   for (i=0; i<amaip->numsaps; i++)
   {
      salp = amaip->saps[i];
      strand = AlnMgr2GetNthStrand(salp, n);
      if (strand == Seq_strand_minus)
      {
         SAIndex2Free2(salp->saip);
         salp->saip = NULL;
         salp->next = NULL;
         SeqAlignListReverseStrand(salp);
         AlnMgr2IndexSingleChildSeqAlign(salp);
      }
   }
   /* spin structure: n1 = which alignment, n2 = start on first row, n3 =
      alignment length on 1st row, n4 = start on 2nd row, n5 = 2nd strand */
   spin = (SPI_nPtr PNTR)MemNew((amaip->numsaps)*sizeof(SPI_nPtr));
   for (i=0; i<amaip->numsaps; i++)
   {
      spin[i] = (SPI_nPtr)MemNew(sizeof(SPI_n));
      salp = amaip->saps[i];
      spin[i]->n1 = i;
      AlnMgr2GetNthSeqRangeInSA(salp, n, &start, &stop);
      spin[i]->n3 = stop - start;
      spin[i]->n2 = start;
      AlnMgr2GetNthSeqRangeInSA(salp, 3-n, &start, &stop);
      spin[i]->n4 = start;
      strand = AlnMgr2GetNthStrand(salp, 3-n);
      if (strand == Seq_strand_minus)
         spin[i]->n5 = -1;
      else
         spin[i]->n5 = 1;
      spin[i]->n6 = compact;
   }
   HeapSort((Pointer)spin, (size_t)(amaip->numsaps), sizeof(SPI_nPtr), SPI_CompareSpins);
   strand = spin[0]->n5;
   for (i=1; i<amaip->numsaps; i++)
   {
      if (spin[i]->n5 != strand)
      {
         salp = amaip->saps[spin[i]->n1];
         salp->next = NULL;
         SeqAlignFree(salp);
         amaip->saps[spin[i]->n1] = NULL;
         spin[i]->n1 = -1;
      }
   }
   for (curr=0; curr<amaip->numsaps; curr++)
   {
      if (spin[curr]->n1 != -1)
      {
         for (i=curr+1; i<amaip->numsaps; i++)
         {
            if (spin[i]->n1 != -1)
            {
               conflict = FALSE;
            /* check first for conflict on first row */
               if (spin[i]->n2 + spin[i]->n3 - 1 >= spin[curr]->n2 + fuzz)
               {
                  if (spin[i]->n2 <= spin[curr]->n2 + fuzz)
                     conflict = TRUE;
               }
               if (spin[i]->n2 <= spin[curr]->n2 + spin[curr]->n3 - 1 - fuzz)
               {
                  if (spin[i]->n2 + spin[i]->n3 - 1 >= spin[curr]->n2 + spin[curr]->n3 - 1)
                     conflict = TRUE;
               }
               if (spin[i]->n2 >= spin[curr]->n2)
               {
                  if (spin[i]->n2 + spin[i]->n3 - 1 <= spin[curr]->n2 + spin[curr]->n3 - 1)
                     conflict = TRUE;
               }
            /* then check for conflict and consistency on second row */
               if (spin[i]->n4 + spin[i]->n3-1 >= spin[curr]->n4 + fuzz)
               {
                  if (spin[i]->n4 <= spin[curr]->n4 + fuzz)
                     conflict = TRUE;
               }
               if (spin[i]->n4 <= spin[curr]->n4 + spin[curr]->n3 - 1 - fuzz)
               {
                  if (spin[i]->n4 + spin[i]->n3 - 1 > spin[curr]->n4 + fuzz)
                     conflict = TRUE;
               }
               if (spin[i]->n4 >= spin[curr]->n4)
               {
                  if (spin[i]->n4 + spin[i]->n3 - 1 <= spin[curr]->n4 + spin[curr]->n3 - 1)
                     conflict = TRUE;
               }
               if (spin[i]->n2 + spin[i]->n3 - 1 <= spin[curr]->n2 + fuzz)
               {
                  if (strand == 1)
                  {
                     if (spin[i]->n4 + spin[i]->n3 - 1 >= spin[curr]->n4 + fuzz)
                        conflict = TRUE;
                  } else if (strand == -1)
                  {
                     if (spin[curr]->n4 + spin[curr]->n3 - 1 - fuzz >= spin[i]->n4)
                        conflict = TRUE;
                  }
               } else
               {
                  if (strand == 1)
                  {
                     if (spin[i]->n4 <= spin[curr]->n4 + spin[curr]->n3 - fuzz)
                        conflict = TRUE;
                  } else if (strand == -1)
                  {
                     if (spin[i]->n4 + spin[i]->n3 - 1 - fuzz >= spin[curr]->n4)
                        conflict = TRUE;
                  }
               }
               if (conflict)
               {
                  salp = amaip->saps[spin[i]->n1];
                  salp->next = NULL;
                  SeqAlignFree(salp);
                  amaip->saps[spin[i]->n1] = NULL;
                  spin[i]->n1 = -1;
               }
            }
         }
      }
   }
   salp_head = salp_prev = NULL;
   for (i=0; i<amaip->numsaps; i++)
   {
      MemFree(spin[i]);
      if (amaip->saps[i] != NULL)
      {
         amaip->saps[i]->next = NULL;
         if (salp_prev != NULL)
         {
            salp_prev->next = amaip->saps[i];
            salp_prev = salp_prev->next;
         } else
            salp_head = salp_prev = amaip->saps[i];
      }
   }
   sap->segs = (Pointer)(salp_head);
   if (indextype == AM2_LITE)
   {
      AMAlignIndex2Free2(sap->saip);
      sap->saip = NULL;
      AlnMgr2IndexLite(sap);
   } else
      AlnMgr2ReIndexSeqAlign(sap);
   MemFree(spin);
}

/***************************************************************************
*
*  SPI_CompareSpins is the HeapSort callback for
*  SPI_RemoveInconsistentAlnsFromSet. It compares first the alignment
*  length on the first row, then the alignment start on the first row.
*
***************************************************************************/
static int LIBCALLBACK SPI_CompareSpins(VoidPtr ptr1, VoidPtr ptr2)
{
   SPI_nPtr  spin1;
   SPI_nPtr  spin2;

   spin1 = *((SPI_nPtr PNTR) ptr1);
   spin2 = *((SPI_nPtr PNTR) ptr2);
   if (spin1 == NULL || spin2 == NULL)
      return 0;
   if (spin1->n3 > spin2->n3)
      return -1;
   if (spin1->n3 < spin2->n3)
      return 1;
   if (spin1->n6 == SPI_RIGHT)
   {
      if (spin1->n2 > spin2->n2)
         return -1;
      if (spin1->n2 < spin2->n2)
         return 1;
   } else if (spin1->n6 == SPI_LEFT)
   {
      if (spin1->n2 < spin2->n2)
         return -1;
      if (spin1->n2 > spin2->n2)
         return 1;
   }
   return 0;
}

/***************************************************************************
*
*  SPI_OrderInternally takes a herd of fragments and their alignments
*  and sorts the alignments for each fragment by their start positions
*  on the mRNA sequence.
*
***************************************************************************/
static void SPI_OrderInternally(SPI_FragHerdPtr sfhp)
{
   AMAlignIndex2Ptr  amaip;
   Int4             i;
   Int4             j;
   SeqAlignPtr      salp;
   Uint1            strand;

   for (i=0; i<sfhp->numfrags; i++)
   {
      if (sfhp->sfparray[i]->sap != NULL)
      {
         amaip = (AMAlignIndex2Ptr)(sfhp->sfparray[i]->sap->saip);
         salp = (SeqAlignPtr)(sfhp->sfparray[i]->sap->segs);
         while (salp != NULL)
         {
            strand = AlnMgr2GetNthStrand(salp, 2);
            if (strand == Seq_strand_minus)
            {
               SAIndex2Free2(salp->saip);
               salp->saip = NULL;
               salp->next = NULL;
               SeqAlignListReverseStrand(salp);
               AlnMgr2IndexSingleChildSeqAlign(salp);
            }
            salp = salp->next;
         }
         if (amaip->numsaps > 1)
         {
            HeapSort((Pointer)(amaip->saps), (size_t)(amaip->numsaps), sizeof(SeqAlignPtr), SPI_CompareAlnPos);
            for (j=0; j<amaip->numsaps-1; j++)
            {
               amaip->saps[j]->next = amaip->saps[j+1];
               amaip->saps[j+1]->next = NULL;
            }
            sfhp->sfparray[i]->sap->segs = (Pointer)(amaip->saps[0]);
         }
      }
   }
}

/***************************************************************************
*
*  SPI_CompareAlnPos is the callback for the HeapSort in
*  SPI_OrderInternally. It compares the start positions on the mRNA
*  sequence of two alignments, and puts the 5'-most alignment first.
*
***************************************************************************/
static int LIBCALLBACK SPI_CompareAlnPos(VoidPtr ptr1, VoidPtr ptr2)
{
   SeqAlignPtr  sap1;
   SeqAlignPtr  sap2;
   Int4         start1;
   Int4         start2;

   sap1 = *((SeqAlignPtr PNTR) ptr1);
   sap2 = *((SeqAlignPtr PNTR) ptr2);
   if (sap1 == NULL || sap2 == NULL)
      return 0;
   AlnMgr2GetNthSeqRangeInSA(sap1, 2, &start1, NULL);
   AlnMgr2GetNthSeqRangeInSA(sap2, 2, &start2, NULL);
   if (start1 <= start2)
      return -1;
   else if (start2 > start1)
      return 1;
   return 0;
}

/***************************************************************************
*
*  SPI_GetResultsForCDS takes a completed mRNA-to-genomic alignment,
*  extracts the CDS annotation for the mRNA, then truncates the mRNA
*  alignment appropriately to create a CDS alignment.  Most of the
*  mRNA information (splice sites, etc) can simply be duplicated, but
*  the first and last exons are often truncated, so their information
*  must be recomputed. Since the CDS is known, the UTRs are known, so
*  the 5' and 3' UTR %identities are calculated as well.
*
***************************************************************************/
static SPI_RegionInfoPtr SPI_GetResultsForCDS(SPI_RegionInfoPtr srip_mrna, BioseqPtr bsp_mrna, SPI_OptionsPtr spot)
{
   Int4               b;
   Int4               c;
   SeqMgrFeatContext  context;
   BoolPtr            featDefFilter;
   Int4               i;
   Int4               mis;
   Int4               len;
   Int4               offset;
   SeqAlignPtr        sap;
   SPI_mRNAPtr        smp;
   SPI_nPtr           spin;
   SPI_RegionInfoPtr  srip_cds;
   Int4               start_cds;
   Int4               stop_cds;
   Int4Ptr            tmpmstarts;
   Int4Ptr            tmpmstops;
   Int4               tmp1;
   Int4               tmp2;
   Int4               tmp3;

   if (srip_mrna == NULL || srip_mrna->revcomp == TRUE)
      return NULL;
   SeqMgrIndexFeatures(0, (Pointer)bsp_mrna);
   featDefFilter = (BoolPtr)MemNew((FEATDEF_MAX)*sizeof(Boolean));
   featDefFilter[FEATDEF_CDS] = TRUE;
   spin = (SPI_nPtr)MemNew(sizeof(SPI_n));
   SeqMgrExploreFeatures(bsp_mrna, (Pointer)spin, SPI_GetCDS, NULL, NULL, featDefFilter);
   MemFree(featDefFilter);
   context.left = spin->n1;
   context.right = spin->n2;
   if (context.right == 0)
      return NULL;
   start_cds = stop_cds = -1;
   offset = context.left;
   if (srip_mrna->smp->numexons == 1 || srip_mrna->smp->mstarts[0] < srip_mrna->smp->mstarts[1])
   {
      tmp1 = srip_mrna->smp->mstarts[0]-1;
      tmp2 = srip_mrna->smp->mstops[srip_mrna->smp->numexons-1]-1;
   } else
   {
      tmp1 = srip_mrna->smp->mstarts[srip_mrna->smp->numexons-1]-1;
      tmp2 = srip_mrna->smp->mstops[0]-1;
   }
   if (tmp2 < tmp1)
   {
      tmp3 = tmp2;
      tmp2 = tmp1;
      tmp1 = tmp3;
   }
   if (context.left > tmp2 || context.right < tmp1) /* cds not contained in model */
      return NULL;
   if (srip_mrna->smp->strand != Seq_strand_minus)
   {
      for (i=0; i<srip_mrna->smp->numexons; i++)
      {
         if (context.left >= srip_mrna->smp->mstarts[i]-1 && context.left <= srip_mrna->smp->mstops[i]-1)
            start_cds = i;
         if (context.right >= srip_mrna->smp->mstarts[i]-1 && context.right <= srip_mrna->smp->mstops[i]-1)
            stop_cds = i;
      }
      smp = (SPI_mRNAPtr)MemNew(sizeof(SPI_mRNA));
      smp->fallsoff = SPI_NEITHER;
      if (start_cds == -1) /* mRNA alignment doesn't include beginning of CDS */
      {
         start_cds = 0;
         smp->fallsoff = SPI_LEFT;
      }
      if (stop_cds == -1) /* mRNA alignment doesn't include end of CDS */
      {
         stop_cds = srip_mrna->smp->numexons-1;
         if (smp->fallsoff == SPI_LEFT)
            smp->fallsoff = SPI_BOTH;
         else
            smp->fallsoff = SPI_RIGHT;
      }
      srip_cds = (SPI_RegionInfoPtr)MemNew(sizeof(SPI_RegionInfo));
      srip_cds->smp = smp;
      srip_cds->mlen = abs(context.left - context.right) + 1;
      smp->numexons = stop_cds - start_cds + 1;
      smp->strand = srip_mrna->smp->strand;
      smp->exonid = (FloatHiPtr)MemNew((smp->numexons)*sizeof(FloatHi));
      smp->exongaps = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->splicedon = (Uint1Ptr)MemNew((smp->numexons)*sizeof(Uint1));
      smp->spliceacc = (Uint1Ptr)MemNew((smp->numexons)*sizeof(Uint1));
      smp->mstarts = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->mstops = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->gstarts = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->gstops = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->saps = (SeqAlignPtr PNTR)MemNew((smp->numexons)*sizeof(SeqAlignPtr));
      if (srip_mrna->smp->mstarts[start_cds]-1 < offset)
         smp->mstarts[0] = 0;
      else
         smp->mstarts[0] = srip_mrna->smp->mstarts[start_cds]-1 - offset;
      if (smp->numexons > 1)
         smp->mstops[0] = srip_mrna->smp->mstops[start_cds]-1 - offset;
      for (i=1; i<smp->numexons-1; i++)
      {
         smp->mstarts[i] = srip_mrna->smp->mstarts[i+start_cds]-1 - offset;
         smp->mstops[i] = srip_mrna->smp->mstops[i+start_cds]-1 - offset;
      }
      if (smp->numexons > 1)
         smp->mstarts[smp->numexons-1] = srip_mrna->smp->mstarts[smp->numexons-1 + start_cds]-1 - offset;
      if (smp->fallsoff == SPI_NEITHER || smp->fallsoff == SPI_LEFT)
         smp->mstops[smp->numexons-1] = context.right - offset;
      else
         smp->mstops[smp->numexons-1] = srip_mrna->smp->mstops[smp->numexons-1 + start_cds]-1 - offset;
      smp->polyAtail = 0; /* no polyA on a CDS */
      /* now copy the splice information and truncate the alignments */
      for (i=0; i<smp->numexons; i++)
      {
         smp->splicedon[i] = srip_mrna->smp->splicedon[i+start_cds];
         smp->spliceacc[i] = srip_mrna->smp->spliceacc[i+start_cds];
      }
      if (smp->numexons > 1)
      {
         smp->saps[0] = SeqAlignDup(srip_mrna->smp->saps[start_cds]);
         if (smp->fallsoff == SPI_NEITHER || smp->fallsoff == SPI_RIGHT)
         {
            if (AlnMgr2TruncateSeqAlign(smp->saps[0], smp->mstarts[0] + offset, srip_mrna->smp->mstops[start_cds]-1, 2))
            {
               SeqAlignFree(smp->saps[0]->next);
               smp->saps[0]->next = NULL;
            }
         }
         smp->saps[smp->numexons-1] = SeqAlignDup(srip_mrna->smp->saps[stop_cds]);
         if (smp->fallsoff == SPI_NEITHER || smp->fallsoff == SPI_LEFT)
         {
            if (AlnMgr2TruncateSeqAlign(smp->saps[smp->numexons-1], srip_mrna->smp->mstarts[stop_cds]-1, smp->mstops[smp->numexons-1]+offset, 2))
            {
               SeqAlignFree(smp->saps[smp->numexons-1]->next);
               smp->saps[smp->numexons-1]->next = NULL;
            }
         }
         for (i=1; i<smp->numexons-1; i++)
         {
            smp->saps[i] = SeqAlignDup(srip_mrna->smp->saps[i+start_cds]);
         }
      } else
      {
         smp->saps[0] = SeqAlignDup(srip_mrna->smp->saps[start_cds]);
         if (smp->fallsoff != SPI_BOTH)
         {
            if (AlnMgr2TruncateSeqAlign(smp->saps[0], smp->mstarts[0] + offset, smp->mstops[0] + offset, 2))
            {
               SeqAlignFree(smp->saps[0]->next);
               smp->saps[0]->next = NULL;
            }
         }
      }
      mis = 0;
      len = 0;
      tmpmstarts = smp->mstarts;
      tmpmstops = smp->mstops;
      smp->mstarts = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->mstops = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      for (i=0; i<smp->numexons; i++)
      {
         AlnMgr2IndexSingleChildSeqAlign(smp->saps[i]);
         len += SPI_GetExonInfo(smp, i, &b, &c, &mis, spot);
         SAIndex2Free2(smp->saps[i]->saip);
         smp->saps[i]->saip = NULL;
      /*   SPI_ShiftAlignment(smp->saps[i], -offset, 2);*/
         AlnMgr2IndexSingleChildSeqAlign(smp->saps[i]);
      }
      MemFree(smp->mstarts);
      MemFree(smp->mstops);
      smp->mstarts = tmpmstarts;
      smp->mstops = tmpmstops;
      smp->mRNAcoverage = (100*len)/(context.right - context.left);
      smp->mismatch = (FloatHi)(100*mis)/len;
   } else
   {
      for (i=0; i<srip_mrna->smp->numexons; i++)
      {
         if (context.right >= srip_mrna->smp->mstarts[i]-1 && context.right <= srip_mrna->smp->mstops[i]-1)
            start_cds = i;
         if (context.left >= srip_mrna->smp->mstarts[i]-1 && context.left <= srip_mrna->smp->mstops[i]-1)
            stop_cds = i;
      }
      smp = (SPI_mRNAPtr)MemNew(sizeof(SPI_mRNA));
      smp->fallsoff = SPI_NEITHER;
      if (start_cds == -1)
      {
         start_cds = 0;
         smp->fallsoff = SPI_RIGHT;
      }
      if (stop_cds == -1)
      {
         if (srip_mrna->smp->mstarts[srip_mrna->smp->numexons-1] > context.left)
         {
            stop_cds = srip_mrna->smp->numexons-1;
            if (smp->fallsoff == SPI_RIGHT)
               smp->fallsoff = SPI_BOTH;
            else
               smp->fallsoff = SPI_LEFT;
         } else
         {
            for (i=0; i<srip_mrna->smp->numexons; i++)
            {
               if (srip_mrna->smp->mstarts[i] > context.left)
                  stop_cds = i;
            }
            if (smp->fallsoff == SPI_RIGHT)
               smp->fallsoff = SPI_BOTH;
            else
               smp->fallsoff = SPI_LEFT;
         }
      }
      srip_cds = (SPI_RegionInfoPtr)MemNew(sizeof(SPI_RegionInfo));
      srip_cds->smp = smp;
      srip_cds->mlen = abs(context.left - context.right) + 1;
      smp->numexons = stop_cds - start_cds + 1;
      smp->strand = srip_mrna->smp->strand;
      smp->exonid = (FloatHiPtr)MemNew((smp->numexons)*sizeof(FloatHi));
      smp->exongaps = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->splicedon = (Uint1Ptr)MemNew((smp->numexons)*sizeof(Uint1));
      smp->spliceacc = (Uint1Ptr)MemNew((smp->numexons)*sizeof(Uint1));
      smp->mstarts = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->mstops = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->gstarts = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->gstops = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->saps = (SeqAlignPtr PNTR)MemNew((smp->numexons)*sizeof(SeqAlignPtr));
      if (smp->numexons > 1)
         smp->mstarts[0] = srip_mrna->smp->mstarts[start_cds]-1 - offset;
      if (smp->fallsoff == SPI_NEITHER || smp->fallsoff == SPI_LEFT)
         smp->mstops[0] = context.right - offset;
      else
         smp->mstops[0] = srip_mrna->smp->mstops[start_cds]-1 - offset;
      for (i=1; i<smp->numexons-1; i++)
      {
         smp->mstarts[i] = srip_mrna->smp->mstarts[i+start_cds]-1 - offset;
         smp->mstops[i] = srip_mrna->smp->mstops[i+start_cds]-1 - offset;
      }
      if (smp->numexons > 1)
         smp->mstops[smp->numexons-1] = srip_mrna->smp->mstops[smp->numexons-1+start_cds]-1 - offset;
      if (smp->fallsoff == SPI_NEITHER || smp->fallsoff == SPI_RIGHT)
         smp->mstarts[smp->numexons-1] = 0;
      else
         smp->mstarts[smp->numexons-1] = srip_mrna->smp->mstarts[smp->numexons-1+start_cds]-1 - offset;
      smp->polyAtail = 0;
      /* now copy the splice site info and truncate the alignments */
      for (i=0; i<smp->numexons; i++)
      {
         smp->splicedon[i] = srip_mrna->smp->splicedon[i+start_cds];
         smp->spliceacc[i] = srip_mrna->smp->spliceacc[i+start_cds];
      }
      if (smp->numexons > 1)
      {
         smp->saps[0] = SeqAlignDup(srip_mrna->smp->saps[start_cds]);
         if (smp->fallsoff == SPI_NEITHER || smp->fallsoff == SPI_LEFT)
         {
            if (AlnMgr2TruncateSeqAlign(smp->saps[0], smp->mstarts[0] + offset, smp->mstops[0] + offset, 2))
            {
               SeqAlignFree(smp->saps[0]->next);
               smp->saps[0]->next = NULL;
            }
         }
         smp->saps[smp->numexons-1] = SeqAlignDup(srip_mrna->smp->saps[smp->numexons-1+start_cds]);
         if (smp->fallsoff == SPI_NEITHER || smp->fallsoff == SPI_RIGHT)
         {
            if (AlnMgr2TruncateSeqAlign(smp->saps[smp->numexons-1], smp->mstarts[smp->numexons-1] + offset, srip_mrna->smp->mstops[smp->numexons-1+start_cds]-1, 2))
            {
               SeqAlignFree(smp->saps[smp->numexons-1]->next);
               smp->saps[smp->numexons-1]->next = NULL;
            }
         }
         for (i=1; i<smp->numexons-1; i++)
         {
            smp->saps[i] = SeqAlignDup(srip_mrna->smp->saps[i+start_cds]);
         }
      } else
      {
         smp->saps[0] = SeqAlignDup(srip_mrna->smp->saps[start_cds]);
         if (smp->fallsoff != SPI_BOTH)
         {
            if (AlnMgr2TruncateSeqAlign(smp->saps[0], smp->mstarts[0] + offset, smp->mstops[0] + offset, 2))
            {
               SeqAlignFree(smp->saps[0]->next);
               smp->saps[0]->next = NULL;
            }
         }
      }
      mis = 0;
      len = 0;
      tmpmstarts = smp->mstarts;
      tmpmstops = smp->mstops;
      smp->mstarts = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      smp->mstops = (Int4Ptr)MemNew((smp->numexons)*sizeof(Int4));
      for (i=0; i<smp->numexons; i++)
      {
         AlnMgr2IndexSingleChildSeqAlign(smp->saps[i]);
         len += SPI_GetExonInfo(smp, i, &b, &c, &mis, spot);
         SAIndex2Free2(smp->saps[i]->saip);
         smp->saps[i]->saip = NULL;
   /*      SPI_ShiftAlignment(smp->saps[i], -offset, 2);*/
         AlnMgr2IndexSingleChildSeqAlign(smp->saps[i]);
      }
      MemFree(smp->mstarts);
      MemFree(smp->mstops);
      smp->mstarts = tmpmstarts;
      smp->mstops = tmpmstops;
      smp->mRNAcoverage = (100*len)/(context.right - context.left);
      smp->mismatch = (FloatHi)(100*mis)/len;
   }
   for (i=0; i<smp->numexons-1; i++)
   {
      smp->saps[i]->next = smp->saps[i+1];
      smp->saps[i+1]->next = NULL;
   }
   sap = SeqAlignNew();
   sap->segtype = SAS_DISC;
   sap->segs = (Pointer)(smp->saps[0]);
   sap->dim = 2;
   AlnMgr2IndexLite(sap);
   smp->parent = sap;
   srip_cds->strand = srip_mrna->strand;
   /* fill in srip_cds->mstart and mstop with the CDS boundaries for printing */
   srip_cds->mstart = context.left;
   srip_cds->mstop = context.right;
   srip_cds->smp->missingends = srip_cds->smp->fallsoff;
   SPI_FillInUTRInfo(srip_cds, srip_mrna, bsp_mrna->length, start_cds, stop_cds);
   if (spin->n3 == 1)  /* CDS is 5' partial */
      srip_cds->gstart = 1;
   else
      srip_cds->gstart = 0;
   if (spin->n4 == 1)  /* CDS is 3' partial */
      srip_cds->gstop = 1;
   else
      srip_cds->gstop = 0;
   MemFree(spin);
   return srip_cds;
}

/***************************************************************************
*
*  SPI_FillInUTRInfo is called by SPI_GetResultsForCDS to figure out the
*  %id of the 5' and 3' UTRs. Since the UTRs usually do not exactly
*  coincide with exon boundaries, this is not a trivial task:
*
*  ----********  *****  ****---  ------  4 exons, *=CDS, -=UTR
*  In this example, the number of mismatches in the 5' UTR is the number
*  of mismatches in exon 1 minus the number of mismatches in exon 1 of
*  the CDS. The number of mismatches in the 3' UTR is the number of
*  mismatches in exon 4 of the mRNA, plus the number of mismatches in
*  exon 3 of the mRNA, minus the number of mismatches in exon 3 of the CDS.
*  The most complicated example is when a single-exon CDS does not quite
*  reach the edges of the corresponding mRNA exon:
*
*   ------- ----*******----- ----- ------ 4 exons
*  In this example, the function actually needs to make a small
*  alignment corresponding to the last part of exon 2 of the mRNA, so
*  that the mismatches for just that piece can be computed.
*
***************************************************************************/
static void SPI_FillInUTRInfo(SPI_RegionInfoPtr srip_cds, SPI_RegionInfoPtr srip_mrna, Int4 len, Int4 exonstart, Int4 exonstop)
{
   ACTProfilePtr  app1;
   ACTProfilePtr  app2;
   Boolean        found;
   Int4           i;
   Int4           j;
   FloatHi        mismatch_cds;
   FloatHi        mismatch_misc;
   FloatHi        mismatch_mrna_l;
   FloatHi        mismatch_mrna_r;
   SeqAlignPtr    sap_tmp;

   mismatch_mrna_l = mismatch_mrna_r = 0;
   for (i=0; i<=exonstart; i++)
   {
      mismatch_mrna_l += (FloatHi)(1-srip_mrna->smp->exonid[i]/100)*(srip_mrna->smp->mstops[i] - srip_mrna->smp->mstarts[i] + 1);
   }
   for (i=exonstop; i<srip_mrna->smp->numexons; i++)
   {
      mismatch_mrna_r += (FloatHi)(1-srip_mrna->smp->exonid[i]/100)*(srip_mrna->smp->mstops[i] - srip_mrna->smp->mstarts[i] + 1);
   }
   if (exonstart != exonstop)
   {
      mismatch_cds = (FloatHi)(1-srip_cds->smp->exonid[0]/100)*(srip_cds->smp->mstops[0] - srip_cds->smp->mstarts[0] + 1);
      mismatch_mrna_l -= mismatch_cds;
      mismatch_cds = (FloatHi)(1-srip_cds->smp->exonid[srip_cds->smp->numexons-1]/100)*(srip_cds->smp->mstops[srip_cds->smp->numexons-1] - srip_cds->smp->mstarts[srip_cds->smp->numexons-1] + 1);
      mismatch_mrna_r -= mismatch_cds;
      if (srip_cds->strand != Seq_strand_minus)
      {
         if (srip_cds->mstart > 0)
            srip_cds->utr.left = (FloatHi)(100)*(1-(mismatch_mrna_l/(srip_cds->mstart)));
         else
            srip_cds->utr.left = -1;
         if (len-1-srip_cds->mstop > 0)
            srip_cds->utr.right = (FloatHi)(100)*(1-(mismatch_mrna_r/(len-1-srip_cds->mstop)));
         else
            srip_cds->utr.right = -1;
      } else
      {
         if (srip_cds->mstart > 0)
            srip_cds->utr.right = (FloatHi)(100)*(1-(mismatch_mrna_r/srip_cds->mstart));
         else
            srip_cds->utr.right = -1;
         if (len-1-srip_cds->mstop > 0)
            srip_cds->utr.left = (FloatHi)(100)*(1-(mismatch_mrna_l/(len-1-srip_cds->mstop)));
         else
            srip_cds->utr.left = -1;
      }
   } else /* have to figure out how many mismatches are on each side of the exon now */
   {
      sap_tmp = SeqAlignDup(srip_mrna->smp->saps[exonstart]);
      mismatch_misc = 0;
      if (AlnMgr2TruncateSeqAlign(sap_tmp, srip_cds->mstart, srip_cds->mstop, 2))
      {
         app1 = SPI_MakeProfileFromSA(sap_tmp->next);
         app2 = app1;
         while (app2 != NULL)
         {
            for (i=0; i<app2->len-1; i++)
            {
               found = FALSE;
               for (j=0; j<ACT_NUCLEN; j++)
               {
                  if (app2->freq[j][i] == 1 && !found)
                  {
                     if (app2->freq[4][i] == 0) /* not an N */
                     {
                        mismatch_misc += 1;
                        found = TRUE;
                     }
                  }
               }
            }
            app1 = app2->next;
            MemFree(app2);
            app2 = app1;
         }
         mismatch_cds = (FloatHi)(1-srip_cds->smp->exonid[0]/100)*(srip_cds->smp->mstops[0] - srip_cds->smp->mstarts[0] + 1);
         mismatch_mrna_l -= (mismatch_cds + mismatch_misc);
         mismatch_mrna_r -= ((FloatHi)(1-srip_mrna->smp->exonid[exonstart]/100)*(srip_mrna->smp->mstops[exonstart] - srip_mrna->smp->mstarts[exonstart] + 1) - mismatch_misc);
      } else /* nothing on the right side, so all the extra mismatches are on the left */
      {
         mismatch_cds = (1-srip_cds->smp->exonid[0]/100)*(srip_cds->smp->mstops[0] - srip_cds->smp->mstarts[0] + 1);
         mismatch_mrna_l -= mismatch_cds;
         mismatch_mrna_r -= (FloatHi)(1-srip_mrna->smp->exonid[exonstart]/100)*(srip_mrna->smp->mstops[exonstart] - srip_mrna->smp->mstarts[exonstart] + 1);
      }
      SeqAlignSetFree(sap_tmp);
      if (srip_cds->strand != Seq_strand_minus)
      {
         if (srip_cds->mstart > 0)
            srip_cds->utr.left = (FloatHi)(100)*(1-(mismatch_mrna_l/(srip_cds->mstart)));
         else
            srip_cds->utr.left = -1;
         if (len-1-srip_cds->mstop > 0)
            srip_cds->utr.right = (FloatHi)(100)*(1-(mismatch_mrna_r/(len-1-srip_cds->mstop)));
         else
            srip_cds->utr.right = -1;
      } else
      {
         if (srip_cds->mstart > 0)
            srip_cds->utr.right = (FloatHi)(100)*(1-(mismatch_mrna_r/srip_cds->mstart));
         else
            srip_cds->utr.right = -1;
         if (len-1-srip_cds->mstop > 0)
            srip_cds->utr.left = (FloatHi)(100)*(1-(mismatch_mrna_l/(len-1-srip_cds->mstop
)));
         else
            srip_cds->utr.left = -1;
      }
   }
}

/***************************************************************************
*
*  SPI_GetCDS is the callback for the SeqEntryExplore call in
*  SPI_GetResultsForCDS.  It simply records the left and right-most
*  boundaries of the coding region found.
*
***************************************************************************/
static Boolean LIBCALLBACK SPI_GetCDS(SeqFeatPtr sfp, SeqMgrFeatContextPtr context)
{
   Boolean   p3;
   Boolean   p5;
   SPI_nPtr  spin;

   if (sfp == NULL)
      return FALSE;
   spin = (SPI_nPtr)context->userdata;
   if (context->seqfeattype == SEQFEAT_CDREGION && context->strand != Seq_strand_minus)
   {
      spin->n1 = context->left;
      spin->n2 = context->right;
      CheckSeqLocForPartial(sfp->location, &p5, &p3);
      if (p5)
         spin->n3 = 1;
      if (p3)
         spin->n4 = 1;
   }
   return TRUE;
}

/***************************************************************************
*
*  SPI_ShiftAlignment was originally used by SPI_GetResultsForCDS to shift
*  the CDS-genomic alignment to CDS coordinates. Now, that alignment is left
*  in mRNA coordinates so that it will still be a valid alignment. If
*  necessary, this function may be used to create a valid alignment for a
*  CDS bioseq, so it stays here for now.
*
***************************************************************************/
static void SPI_ShiftAlignment(SeqAlignPtr sap, Int4 offset, Int4 row)
{
   DenseSegPtr  dsp;
   Int4         i;

   dsp = (DenseSegPtr)(sap->segs);
   if (row > dsp->dim)
      return;
   for (i=0; i<dsp->numseg; i++)
   {
      if (dsp->starts[(dsp->dim*i)+row-1] != -1)
      {
         dsp->starts[(dsp->dim*i)+row-1] += offset;
      }
   }
}

/***************************************************************************
*
*  SPI_GetProteinFrommRNA takes an mRNA bioseq and returns a string
*  which is the best protein translation of the mRNA. First, the function
*  looks to see whether there are any annotated CDSs, and if so, it uses
*  the translation of the annotated CDS. If not, the function translates
*  the mRNA in all 3 reading frames and looks for the frame with the 
*  longest protein, then returns that protein.
*
***************************************************************************/
NLM_EXTERN CharPtr SPI_GetProteinFrommRNA(BioseqPtr bsp_mrna, Int4Ptr start)
{
   ByteStorePtr  bs;
   CharPtr       c1;
   CharPtr       c2;
   CharPtr       c3;
   Int4          c1len;
   Int4          c2len;
   Int4          c3len;
   Int4          c1start;
   Int4          c2start;
   Int4          c3start;
   BoolPtr       featDefFilter;
   CharPtr       seq;
   SeqLocPtr     slp;
   SPI_SeqPtr    ssp;
   CharPtr       tmp;

   if (bsp_mrna == NULL)
      return NULL;
   SeqMgrIndexFeatures(0, (Pointer)bsp_mrna);
   featDefFilter = (BoolPtr)MemNew((FEATDEF_MAX)*sizeof(Boolean));
   featDefFilter[FEATDEF_CDS] = TRUE;
   ssp = (SPI_SeqPtr)MemNew(sizeof(SPI_Seq));
   SeqMgrExploreFeatures(bsp_mrna, (Pointer)ssp, SPI_GetCDSFeat, NULL, NULL, featDefFilter);
   seq = ssp->seq;
   *start = ssp->start;
   MemFree(featDefFilter);
   if (seq == NULL) /* no annotated CDS, have to translate to figure out the protein */
   {
      slp = SeqLocIntNew(0, bsp_mrna->length-1, Seq_strand_plus, bsp_mrna->id);
      bs = TransTableTranslateSeqLoc(NULL, slp, 1, 1, TRUE, TRUE);
      c1 = BSMerge(bs, NULL);
      BSFree(bs);
      bs = TransTableTranslateSeqLoc(NULL, slp, 1, 2, TRUE, TRUE);
      c2 = BSMerge(bs, NULL);
      BSFree(bs);
      bs = TransTableTranslateSeqLoc(NULL, slp, 1, 3, TRUE, TRUE);
      c3 = BSMerge(bs, NULL);
      BSFree(bs);
      c1len = SPI_FindLongestProt(c1, &c1start);
      c2len = SPI_FindLongestProt(c2, &c2start);
      c3len = SPI_FindLongestProt(c3, &c3start);
      if (c1len >= c2len && c1len >= c3len)
      {
         *start = 3*c1start;
         tmp = c1;
         tmp += c1start;
         seq = StringSave(tmp);
      } else if (c2len >= c1len && c2len >= c3len)
      {
         *start = 1+3*c2start;
         tmp = c2;
         tmp += c2start;
         seq = StringSave(tmp);
      } else if (c3len >= c1len && c3len >= c2len)
      {
         *start = 2+3*c3start;
         tmp = c3;
         tmp += c3start;
         seq = StringSave(tmp);
      }
      MemFree(c1);
      MemFree(c2);
      MemFree(c3);
      SeqLocFree(slp);
   }
   MemFree(ssp);
   return seq;
}

/***************************************************************************
*
*  SPI_GetCDSFeat is the SeqMgrExplore callback for SPI_GetProteinFrommRNA.
*  When a CDS feature is found, the function gets the protein byte store
*  corresponding to that feature, then converts the byte store into a
*  string representing the protein sequence.
*
***************************************************************************/
static Boolean LIBCALLBACK SPI_GetCDSFeat(SeqFeatPtr sfp, SeqMgrFeatContextPtr context)
{
   ByteStorePtr  bs;
   SPI_SeqPtr    ssp;

   ssp = (SPI_SeqPtr)(context->userdata);
   if (context->seqfeattype == SEQFEAT_CDREGION)
   {
      bs = ProteinFromCdRegionEx(sfp, TRUE, TRUE);
      ssp->seq = BSMerge(bs, NULL);
      ssp->start = context->left;
      BSFree(bs);
   }
   return TRUE;
}

/***************************************************************************
*
*  SPI_FindLongestProt looks through a string representing a protein
*  sequence (with stop codons), and returns the length of the longest
*  sub-protein (no stops) in the sequence, as well as the position at
*  which the longest protein starts.
*
***************************************************************************/
static Int4 SPI_FindLongestProt(CharPtr seq, Int4Ptr pos)
{
   Int4     i;
   Int4     j;
   Int4     len;
   Int4     max;
   CharPtr  p;

   if (seq == NULL)
      return 0;
   p = seq;
   len = max = 0;
   i = 0;
   j = 0;
   *pos = 0;
   while (*p != '\0')
   {
      if (*p == '*')
      {
         if (len > max)
         {
            max = len;
            *pos = j;
         }
         len = 0;
         j = i+1;
      } else
         len++;
      p++;
      i++;
   }
   return max;
}

static Boolean SPI_GetAccessionFromSeqId(SeqIdPtr sip, Int4Ptr gi, CharPtr PNTR id)
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
         *id = (CharPtr) MemNew(6);
         sprintf(*id, "%d", oip->id);
      }
      break;
   case SEQID_GENBANK: case SEQID_EMBL: case SEQID_PIR: case SEQID_TPG: case SEQID_TPE: case SEQID_TPD:
   case SEQID_SWISSPROT: case SEQID_DDBJ: case SEQID_PRF:
   case SEQID_OTHER: case SEQID_GPIPE:
      textsip = (TextSeqIdPtr)sip->data.ptrvalue;
      id_len = StringLen(textsip->accession);
      *id = (CharPtr) MemNew(id_len+1);
      if (textsip->version > 0)
         sprintf(*id, "%s.%d", textsip->accession, textsip->version);
      else
         sprintf(*id, "%s", textsip->accession);
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

static void SPI_CheckSplicesForRevComp(SPI_RegionInfoPtr srip_head, SPI_OptionsPtr spot, BioseqPtr bsp_genomic, BioseqPtr bsp_mrna)
{
   Int4               c;
   Int4               i;
   SPI_RegionInfoPtr  revcmp;
   SPI_bsinfoPtr      sbp1;
   SPI_bsinfoPtr      sbp2;
   Int4               sites;
   Int4               sites2;
   SPI_RegionInfoPtr  srip;

   srip = srip_head;
   i = 0;
   while (srip != NULL)
   {
      i++;
      srip = srip->next;
   }
   srip = srip_head;
   c = 0;
   /** KSK bug fix - access of null srip->smp after
       SPI_SortRegionsByScore() removed all below -c 
       threshold **/
   while (srip != NULL && srip->smp != NULL)
   {
      if (spot->revcomp == FALSE)
      {
         srip->revcmp_try = TRUE;
         sites = 0;
         for (i=0; i<srip->smp->numexons; i++)
         {
            sites += srip->smp->splicedon[i];
         }
         if ((sites*100)/srip->smp->numexons < SPI_REVCOMPTHRESH)
         {
             BioseqRevComp(bsp_mrna);
             ErrSetMessageLevel(SEV_MAX);
             sbp1 = (SPI_bsinfoPtr)MemNew(sizeof(SPI_bsinfo));
             sbp1->bsp = bsp_genomic;
             sbp2 = (SPI_bsinfoPtr)MemNew(sizeof(SPI_bsinfo));
             sbp2->bsp = bsp_mrna;
             if (spot->lcaseloc){ /* fixes an ABW ? */
                sbp2->lcaseloc = spot->lcaseloc;
             }
            /* sbp2->lcaseloc = spot->lcaseloc; */
            spot->revcomp = TRUE;
            revcmp = SPI_AlnSinglemRNAToGen(sbp1, sbp2, NULL, NULL, spot);
            spot->revcomp = FALSE;
            if (revcmp != NULL && revcmp->smp != NULL)
            {
               if (revcmp != NULL && revcmp->smp != NULL)
               {
                  MemFree(revcmp->smp->protein);
                  revcmp->smp->protein = SPI_GetProteinFrommRNA(bsp_mrna, &revcmp->smp->transstart);
               }
            }
            BioseqRevComp(bsp_mrna);
            if (revcmp != NULL && revcmp->smp != NULL)
            {
               sites2 = 0;
               for (i=0; i<revcmp->smp->numexons; i++)
               {
                  sites2 += revcmp->smp->splicedon[i];
               }
               if ((sites2*100)/revcmp->smp->numexons > (sites*100)/srip->smp->numexons)
               {
                  if (revcmp->smp->mRNAcoverage > srip->smp->mRNAcoverage - SPI_COVERDIFF)
                  {
                     if (revcmp->smp->mismatch < srip->smp->mismatch + SPI_MISMTCHDIFF)
                     {
                        SPI_mRNAFree(srip->smp);
                        srip->smp = revcmp->smp;
                        srip->revcomp = TRUE;
                        srip->mstart = revcmp->mstart;
                        srip->mstop = revcmp->mstop;
                        srip->strand = revcmp->strand;
                        srip->coverage = revcmp->coverage;
                        srip->score = revcmp->score;
                        srip->polyAtail = revcmp->polyAtail;
                        srip->fallsoff = revcmp->fallsoff;
                        srip->utr = revcmp->utr;
                        MemFree(revcmp);
                     }
                  }
               }
            }
         }
      }
      srip = srip->next;
      c++;
   }
}

static ACTProfilePtr SPI_ProfileNew(Boolean nuc)
{
   ACTProfilePtr  app;
   FloatHiPtr     PNTR freq;

   app = (ACTProfilePtr)MemNew(sizeof(ACTProfile));
   if (nuc)
   {
      freq = (FloatHiPtr PNTR)MemNew(ACT_NUCLEN*sizeof(FloatHiPtr));
      app->freq = freq;
      app->nuc = TRUE;
   } else
   {
      freq = (FloatHiPtr PNTR)MemNew(ACT_PROTLEN*sizeof(FloatHiPtr));
      app->freq = freq;
      app->nuc = FALSE;
   }
   return app;
}

static ACTProfilePtr SPI_ProfileFree(ACTProfilePtr app)
{
   Int4  i;
   Int4  j;

   if (app == NULL)
      return NULL;
   if (app->nuc)
      j = ACT_NUCLEN;
   else
      j = ACT_PROTLEN;
   for (i=0; i<j; i++)
   {
      MemFree(app->freq[i]);
   }
   MemFree(app->freq);
   app->next = NULL;
   MemFree(app);
   return NULL;
}

static ACTProfilePtr SPI_ProfileSetFree(ACTProfilePtr app)
{
   ACTProfilePtr  app_next;

   while (app != NULL)
   {
      app_next = app->next;
      app->next = NULL;
      SPI_ProfileFree(app);
      app = app_next;
   }
   return NULL;
}

static void SPI_BuildProfile(SeqLocPtr slp, ACTProfilePtr PNTR app, Int4Ptr count, Int4 length)
{
   Int4        i;
   Int4        len;
   Uint1       res;
   SeqPortPtr  spp;

   if (app == NULL)
      return;
   if (slp == NULL)
   {
      if (*count == 0)
         (*app)->numseq++;
      *count = *count+length;
      if ((*app)->len <= *count)
      {
         *count = 0;
         *app = (*app)->next;
      }
      return;
   }
   len = SeqLocLen(slp);
   if (len <= 0)
      return;
   if ((*app)->len == 0)
   {
      (*app)->len = len;
      if ((*app)->nuc)
      {
         for (i=0; i<ACT_NUCLEN; i++)
         {
            (*app)->freq[i] = (FloatHiPtr)MemNew((*app)->len*sizeof(FloatHi));
         }
      } else
      {
         for (i=0; i<ACT_PROTLEN; i++)
         {
            (*app)->freq[i] = (FloatHiPtr)MemNew((*app)->len*sizeof(FloatHi));
         }
      }
   } else
   {
      if (len > (*app)->len) /* seqloc is longer than the */
         return;          /* existing profile -- don't add it     */
   }
   if ((*app)->nuc)
      spp = SeqPortNewByLoc(slp, Seq_code_ncbi4na);
   else
      spp = SeqPortNewByLoc(slp, Seq_code_ncbistdaa);
   if (spp == NULL)
      return;
   if (*count == 0)
     (*app)->numseq++;
   i=0;
   if ((*app)->nuc == FALSE)
   {
      while ((res = SeqPortGetResidue(spp)) != SEQPORT_EOF && i+*count<((*app)->len))
      {
         (*app)->freq[res][i+*count]++;
         i++;
      }
   } else
   {
      while ((res = SeqPortGetResidue(spp)) != SEQPORT_EOF && i+*count<((*app)->len))
      {
         if (res == 1)
         {
            (*app)->freq[0][i+*count]++;
         } else if (res == 2)
         {
            (*app)->freq[1][i+*count]++;
         } else if (res == 4)
         {
            (*app)->freq[2][i+*count]++;
         } else if (res == 8)
         {
            (*app)->freq[3][i+*count]++;
         } else
         {
            (*app)->freq[4][i+*count]++;
         }
         i++;
      }
   }
   SeqPortFree(spp);
   if (len+*count == (*app)->len)
   {
      *app = (*app)->next;
      *count = 0;
   } else
      *count = *count + len;
   return;
}

static FloatHi SPI_ScoreProfile(BioseqPtr bsp, Int4 pos, Uint1 strand, ACTProfilePtr app)
{
   Int4        i;
   Uint1       res;
   FloatHi     retval;
   SeqPortPtr  spp;

   retval = 0;
   if (bsp == NULL || app == NULL || pos < 0)
      return retval;
   if (pos + app->len-1 >= bsp->length)
      return retval;
   if (ISA_na(bsp->mol))
   {
      spp = SeqPortNew(bsp, pos, pos+app->len-1, strand, Seq_code_ncbi4na);
      if (spp == NULL)
         return retval;
      i = 0;
      while ((res = SeqPortGetResidue(spp)) != SEQPORT_EOF && i<app->len)
      {
         if (res == 1)
         {
            retval += app->freq[0][i];
         } else if (res == 2)
         {
            retval += app->freq[1][i];
         } else if (res == 4)
         {
            retval += app->freq[2][i];
         } else if (res == 8)
         {
            retval += app->freq[3][i];
         } else
         {
            retval += app->freq[4][i];
         }
         i++;
      }
      retval = retval / app->len;
      return retval;
   } else
   {
      spp = SeqPortNew(bsp, pos, pos+app->len-1, strand, Seq_code_ncbistdaa);
      if (spp == NULL)
         return retval;
      i = 0;
      while ((res = SeqPortGetResidue(spp)) != SEQPORT_EOF && i<app->len)
      {
         retval += app->freq[res][i];
         i++;
      }
      retval = retval / app->len;
      return retval;
   }
}

static ACTProfilePtr SPI_MakeProfileFromSA(SeqAlignPtr sap)
{
   AMAlignIndex2Ptr  amaip;
   AlnMsg2Ptr       amp;
   ACTProfilePtr    app;
   ACTProfilePtr    app_head;
   ACTProfilePtr    app_prev;
   BioseqPtr        bsp;
   Int4             count;
   Int4             i;
   Int4             j;
   Boolean          more;
   Int4             n;
   Boolean          nuc;
   Int4             numseg;
   Int4             numrows;
   SeqIdPtr         sip;
   SeqLocPtr        slp;
   Int4             start;
   Int4             stop;

   if (sap == NULL)
      return NULL;
   if (sap->saip == NULL)
      return NULL;
   if (sap->saip->indextype == INDEX_PARENT)
   {
      amaip = (AMAlignIndex2Ptr)(sap->saip);
      if (amaip->alnstyle == AM2_LITE)
         return NULL;
   }
   sip = AlnMgr2GetNthSeqIdPtr(sap, 1);
   bsp = BioseqLockById(sip);
   if (bsp == NULL)
      return NULL;
   if (ISA_na(bsp->mol))
      nuc = TRUE;
   else
      nuc = FALSE;
   BioseqUnlockById(sip);
   sip = SeqIdFree(sip);
   amp = AlnMsgNew2();
   amp->to_aln = -1;
   amp->row_num = 1;
   app_head = NULL;
   numseg = AlnMgr2GetNumSegs(sap);
   for (i=0; i<numseg; i++)
   {
      app = SPI_ProfileNew(nuc);
      AlnMgr2GetNthSegmentRange(sap, i+1, &start, &stop);
      app->len = stop - start + 1;
      if (nuc)
      {
        for (j=0; j<ACT_NUCLEN; j++)
        {
           app->freq[j] = (FloatHiPtr)MemNew(app->len*sizeof(FloatHi));
        }
      } else
      {
        for (j=0; j<ACT_PROTLEN; j++)
        {
           app->freq[j] = (FloatHiPtr)MemNew(app->len*sizeof(FloatHi));
        }
      }
      if (app_head != NULL)
      {
        app_prev->next = app;
        app_prev = app;
      } else
        app_head = app_prev = app;
   }
   numrows = AlnMgr2GetNumRows(sap);
   for (i=1; i<=numrows; i++)
   {
      app = app_head;
      for (n=0; n<numseg; n++)
      {
         AlnMsgReNew2(amp);
         AlnMgr2GetNthSegmentRange(sap, n+1, &amp->from_aln, &amp->to_aln);
         amp->row_num = i;
         sip = AlnMgr2GetNthSeqIdPtr(sap, i);
         bsp = BioseqLockById(sip);
         count = 0;
         while ((Boolean) (more = AlnMgr2GetNextAlnBit(sap, amp)) && app != NULL)
         {
            if (amp->type == AM_SEQ && bsp != NULL)
            {
               slp = SeqLocIntNew(amp->from_row, amp->to_row, amp->strand, sip);
               SPI_BuildProfile(slp, &app, &count, 0);
               SeqLocFree(slp);
            } else if (amp->type == AM_GAP)
               SPI_BuildProfile(NULL, &app, &count, (amp->to_row - amp->from_row + 1));
         }
         BioseqUnlockById(sip);
         sip = SeqIdFree(sip);
      }
   }
   AlnMsgFree2(amp);
   return app_head;
}

/***********************************************************
* SPI_CheckMrnaOrder
* 
* After the ivals for building a region are sorted in genomic 
* order this function merely checks that the mrna invterals 
* are minimally colinear: if the stop of one interval overlaps
* or 'jumps' by more than 20 bases the start of the next interval,
* the one with greatest score is retained and the one with lesser
* score set to 'impossible.
 ***********************************************************/
static void SPI_CheckMrnaOrder(SPI_IvalPtr PNTR spi_pp, const int num)
{
    SPI_IvalPtr  ival = 0, ival2 = 0, ival3 = 0;
    int x = 0;
    
    if (num >=3){
        for (x = 0, ival = spi_pp[x], ival2 = spi_pp[x + 1]; 
             x < num && ival != 0 && ival2 != 0; 
             ++x, ival = spi_pp[x], 
                 ival2 = (x + 1 < num ? spi_pp[x + 1] : 0)){
            if (x < num - 2){ /* three to window */
                ival3 = spi_pp[x + 2];
                if ((ival->strand == Seq_strand_plus == ival2->strand
                     && ival3->strand == ival->strand 
                     && (ival->mstop > ival2->mstart + SPI_FUZZ
                         && ival->mstop < ival3->mstart + SPI_FUZZ))
                    || (ival->strand == Seq_strand_minus == ival2->strand
                        && ival3->strand == ival->strand 
                        && (ival->mstop + SPI_FUZZ < ival2->mstart  
                            && ival->mstop + SPI_FUZZ > ival3->mstart))){
                    if (ival->score > ival2->score){
                        ival2->used = -1;
                    }
                    else if (ival2->score > ival->score){
                        ival->used = -1;
                    }
                }
            }
            else if (x < num - 1){ /* two to window */
                if ((ival->strand == Seq_strand_plus == ival2->strand
                     && ival->mstop > ival2->mstart) 
                    || (ival->strand == Seq_strand_plus == ival2->strand
                        && ival->mstop < ival2->mstart)){
                    if (ival2->score > ival->score){
                        ival->used = -1;
                    }
                    else if (ival->score > ival2->score){
                        ival2->used = -1;
                    }
                }
            }
        }
    }
}
