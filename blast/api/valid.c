/*  valid.c
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
* File Name:  valid.c
*
* Author:  James Ostell
*   
* Version Creation Date: 1/1/94
*
* $Revision: 6.241 $
*
* File Description:  Sequence editing utilities
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: valid.c,v $
* Revision 6.241  2001/12/14 20:19:44  kans
* added ERR_SEQ_FEAT_MultiIntervalGene
*
* Revision 6.240  2001/12/11 22:05:34  kans
* NC drops no protein to warning, suppresses if top IS_Bioseq
*
* Revision 6.239  2001/12/11 20:31:24  kans
* relax or suppress some messages for NC as well as NT or GPS
*
* Revision 6.238  2001/12/11 14:08:56  kans
* added ERR_SEQ_FEAT_CollidingGeneNames
*
* Revision 6.237  2001/12/10 21:28:17  kans
* added ERR_SEQ_FEAT_AbuttingIntervals
*
* Revision 6.236  2001/12/07 15:15:48  kans
* do not complain about cds packaging problem if nc_ gps made redundant prior to splitting
*
* Revision 6.235  2001/12/06 20:51:28  kans
* check label qualifier for single token not just numbers
*
* Revision 6.234  2001/12/06 18:43:44  kans
* do not complain about molinfo.biomol other if XR_ RefSeq
*
* Revision 6.233  2001/12/04 19:29:18  kans
* common CheckForIllegalDbxref function handles extended list for GPS or RefSeq
*
* Revision 6.232  2001/11/27 13:31:59  kans
* check circular topology for SeqLocOrder problem, last ValidateSeqLoc tests only on Seq_repr_seg
*
* Revision 6.231  2001/11/21 22:02:20  kans
* check for bssp class other, gen prod set mRNA feature with local seqid far product
*
* Revision 6.230  2001/11/16 12:41:43  kans
* fixes to ValidateSeqLoc
*
* Revision 6.229  2001/10/15 20:48:06  kans
* suppress ERR_SEQ_INST_RnaDnaConflict - this is how we indicate an mRNA sequenced from a cDNA
*
* Revision 6.228  2001/10/12 21:20:48  kans
* validate orgref dbxref against legal list, suppress farloc warning if NC_ record
*
* Revision 6.227  2001/10/10 20:15:15  kans
* added ERR_SEQ_INST_RnaDnaConflict
*
* Revision 6.226  2001/10/09 18:16:43  kans
* Molinfo-biomol other is SEV_WARNING
*
* Revision 6.225  2001/10/01 11:30:02  kans
* removed unused LockAllBioseqs - replaced by seqmgr function
*
* Revision 6.224  2001/09/27 18:22:32  kans
* PUBSTATUS_aheadofprint suppresses some ERR_GENERIC_MissingPubInfo checks
*
* Revision 6.223  2001/09/21 18:38:34  kans
* warn if exception text set but exception flag is not
*
* Revision 6.222  2001/09/21 15:26:36  kans
* warn on anticodon not equal to 3 bases
*
* Revision 6.221  2001/09/21 14:15:08  kans
* corrected logic in tRNA anticodon checks
*
* Revision 6.220  2001/09/19 20:05:19  kans
* report obsolete (unconverted) descriptor types
*
* Revision 6.219  2001/09/18 21:42:45  kans
* check for inconsistent protein title was stuck in infinite loop - fixed
*
* Revision 6.218  2001/09/18 15:54:13  kans
* warn on tRNA with unparsed string extension
*
* Revision 6.217  2001/09/10 21:08:30  kans
* run through gnu-indent to clean up code formatting (EN)
*
* Revision 6.216  2001/09/05 14:26:02  bazhin
* Fixed a couple of typos while checking CDS's Imp-feats for
* translation presence.
*
* Revision 6.215  2001/08/31 21:38:13  kans
* test for zero gi, imp cds with translation or without pseudo (EN)
*
* Revision 6.214  2001/08/31 16:12:29  kans
* added ERR_SEQ_DESCR_InconsistentProteinTitle
*
* Revision 6.213  2001/08/30 18:37:36  kans
* check for unparsed anticodon qualifier
*
* Revision 6.212  2001/08/30 15:48:42  kans
* do not check rpt_unit string length if multi_rpt_unit in parentheses
*
* Revision 6.211  2001/08/28 19:35:23  kans
* RNA imp feats (not converted because of ifp->loc) now reported as SEV_ERROR, also checking misc_RNA and precursor_RNA
*
* Revision 6.210  2001/08/27 19:04:31  kans
* warn if rna imp feat should be converted to rna feature
*
* Revision 6.209  2001/08/24 19:32:49  kans
* artificial frameshift suppresses SpliceCheck on mRNA as well as CDS
*
* Revision 6.208  2001/08/24 19:29:13  kans
* added artificial frameshift exception text to suppress splice check
*
* Revision 6.207  2001/08/20 15:31:28  kans
* check for codon gbqual on coding region, report with WrongQualOnImpFeat, for now
*
* Revision 6.206  2001/08/14 19:43:08  kans
* added ERR_GENERIC_UnnecessaryPubEquiv and ERR_SEQ_FEAT_UnnecessaryCitPubEquiv
*
* Revision 6.205  2001/08/06 23:59:45  kans
* added third party annotation SeqID support
*
* Revision 6.204  2001/08/02 14:59:18  kans
* TrnaCodonWrong uses SEV_WARNING for selenocysteine
*
* Revision 6.203  2001/08/01 16:55:09  kans
* if is_nc, feature location on segmented bioseq goes to SEV_WARNING
*
* Revision 6.202  2001/07/19 14:41:04  kans
* check for obsolete one-of text in ifp->loc
*
* Revision 6.201  2001/07/18 16:21:53  kans
* only allow unpublished or submitted in cgp->cit
*
* Revision 6.200  2001/07/18 16:11:15  kans
* check citgen->cit for illegal text, call ValidatePubdesc only once for each pub
*
* Revision 6.199  2001/07/18 15:35:09  kans
* check for published journal article missing volume and pages
*
* Revision 6.198  2001/07/16 21:06:40  kans
* added ERR_SEQ_INST_BadSecondaryAccn
*
* Revision 6.197  2001/07/13 00:05:35  kans
* added CheckSegBspAgainstParts to report ERR_SEQ_INST_PartsOutOfOrder
*
* Revision 6.196  2001/07/10 23:00:32  kans
* added ERR_SEQ_FEAT_LocOnSegmentedBioseq
*
* Revision 6.195  2001/07/10 22:41:36  kans
* added ERR_SEQ_FEAT_ImpFeatBadLoc
*
* Revision 6.194  2001/07/10 22:00:09  kans
* calls SeqLocMixedStrands to check for mixed strand on segmented bioseq
*
* Revision 6.193  2001/07/10 15:25:46  kans
* SeqIdNameHasSpace is now SEV_REJECT
*
* Revision 6.192  2001/07/08 21:29:45  kans
* added ERR_SEQ_DESCR_BadSubSource and ERR_SEQ_DESCR_BadOrgMod
*
* Revision 6.191  2001/06/25 19:16:41  kans
* check crp->conflict, do not check length or splice
*
* Revision 6.190  2001/06/14 21:59:16  kans
* suppress overlapping peptide error if exception says alternative processing or alternate processing
*
* Revision 6.189  2001/05/30 20:51:27  kans
* for PDB records, suppress NoOrgFound anywhere on record, NoMolInfo, NoProtRefFound, and ShortSeq
*
* Revision 6.188  2001/05/27 22:51:12  kans
* added ERR_SEQ_FEAT_PolyAsiteNotPoint
*
* Revision 6.187  2001/05/25 22:26:46  kans
* added ERR_SEQ_FEAT_DuplicateInterval, protein EST and other techniques not allowed, segmented protein SeqID is 2 letters + 6 digits
*
* Revision 6.186  2001/04/17 14:08:51  kans
* cds mrna range check is warning if cds exception flag set
*
* Revision 6.185  2001/04/16 17:16:33  kans
* drop duplicate cds error to warning if gps or nt
*
* Revision 6.184  2001/04/13 21:19:09  kans
* SuppressTrailingXMessage if translation ends in * or partial at carboxy end
*
* Revision 6.183  2001/04/13 16:21:22  kans
* CdTransCheck translates without removing trailing ambiguity residues - consistent with other current behavior
*
* Revision 6.182  2001/04/07 18:05:56  kans
* supress cds product packaging problem if nt accession
*
* Revision 6.181  2001/04/05 20:17:16  kans
* added ERR_SEQ_FEAT_CDSproductPackagingProblem
*
* Revision 6.180  2001/03/26 18:58:17  kans
* added INTERVAL_OVERLAP choice for SeqMgrGetOverlappingFeature, if CDS completely within mRNA introns using this test, do not report validator error
*
* Revision 6.179  2001/03/02 01:35:33  kans
* trans splicing message turns off both mixed_strand and out_or_order errors, can be combined with other exception messages in string
*
* Revision 6.178  2001/02/28 22:45:11  kans
* ValidateSeqLoc handles SEQLOC_NULL so as not to miss mixed strands, ignores mixed strands on primer_bind, ignores interval order for publications
*
* Revision 6.177  2001/02/22 14:58:07  ostell
* moved duplicate feature from ERROR to WARN for misc_feat
*
* Revision 6.176  2001/02/14 20:58:18  kans
* check for ERR_SEQ_INST_BadSeqIdFormat in GenBank/EMBL/DDBJ records
*
* Revision 6.175  2001/02/13 23:32:24  kans
* trans splicing exception turns off mixed strand error
*
* Revision 6.174  2001/02/12 14:13:47  kans
* OverlappingGeneIsPseudo test for mRNA and CDS
*
* Revision 6.173  2001/02/09 22:21:39  kans
* duplicate feature with different comment or label is still severity error if GPSorNT - manually annotated records still use different strings to reduce severity, since these might really be intended to be separate features
*
* Revision 6.172  2001/02/09 16:11:16  kans
* check for GPSorNT to lower duplicate feature severity was accidentally only on CDS features, not all features
*
* Revision 6.171  2001/02/07 20:14:58  kans
* ValidateImpFeat GBQUAL_rpt_type test now deals with (DIRECT,TERMINAL) sets of values in parentheses
*
* Revision 6.170  2001/02/03 00:43:31  kans
* use SeqIdIn instead of SeqIdForSameBioseq to get appropriate phrap graphs
*
* Revision 6.169  2001/02/01 16:45:09  kans
* delta seq does not check for htgs tech if NT_ refseq
*
* Revision 6.168  2001/01/27 00:08:07  kans
* added ERR_SEQ_INST_TrailingX
*
* Revision 6.167  2001/01/25 15:44:14  kans
* handles multiple entries on a Seq-submit without a bioseq-set wrapper, suppresses some errors for patents
*
* Revision 6.166  2001/01/23 20:27:11  kans
* added ERR_SEQ_DESCR_MultipleChromosomes
*
* Revision 6.165  2001/01/16 20:53:37  kans
* SeqLocOrder not done for misc_recomb
*
* Revision 6.164  2001/01/09 20:54:01  kans
* DeltaOrFarSeg limits deltas to those without nuc-prot sets
*
* Revision 6.163  2001/01/09 00:25:23  kans
* DuplicateSegmentReferences now warning if not SEQLOC_WHOLE - unable to easily do more sophisticated test
*
* Revision 6.162  2001/01/03 17:10:43  kans
* if DeltaOrFarSeg, drop NoProtein from CDS message to SEV_WARNING
*
* Revision 6.161  2001/01/02 14:02:16  kans
* do not complain about SeqLocOrder for heterogen bonds
*
* Revision 6.160  2000/12/29 16:22:16  kans
* check for bad gene or mrna overlap is SEV_WARNING for NC_ records, which can be initially built on older records in ID that would no longer pass the current stricter validation
*
* Revision 6.159  2000/12/22 00:30:18  kans
* segmented bioseq now checked for bad seqloc order of features on components
*
* Revision 6.158  2000/12/21 18:02:25  kans
* exit CheckForCommonCDSProduct if cds == NULL
*
* Revision 6.157  2000/12/19 18:20:21  kans
* allele and mutation are obsolete
*
* Revision 6.156  2000/12/04 21:54:38  kans
* added NGorNT to suppress NoProtein error
*
* Revision 6.155  2000/11/30 16:43:20  kans
* added MrnaTransCheck
*
* Revision 6.154  2000/11/27 20:46:01  kans
* drop duplicate CDS error to warning if genomic product set, NT contig, or different frames
*
* Revision 6.153  2000/11/27 20:11:48  kans
* calls LockFarComponents and UnlockFarComponents instead of maintaining separate version
*
* Revision 6.152  2000/11/22 16:50:45  kans
* duplicate STS now SEV_WARNING
*
* Revision 6.151  2000/11/20 19:55:35  kans
* cyanelle is a plastid, as far as the flat file is concerned, so use genetic code 11
*
* Revision 6.150  2000/11/20 19:32:35  kans
* warn if nucleotide bioseq in nps within gps does not have mrna feature pointing to it - also added several classes of plastids to use genetic code 11
*
* Revision 6.149  2000/11/14 21:36:58  kans
* if biop->genome == GENOME_plastid, use genetic code 11
*
* Revision 6.148  2000/11/14 15:26:05  kans
* duplicate feature now SEV_WARNING for variation features - SNPs are not yet merged, and would prevent genome sequences from being loaded into ID
*
* Revision 6.147  2000/11/09 22:58:51  kans
* if genomic product set or NT_ contig, splice check severity is relaxed to SEV_WARNING - allows for occasional intron gap in model
*
* Revision 6.146  2000/11/06 21:15:56  kans
* CheckForCommonMRNAProduct checks for NULL mRNA before complaining if different than non-NULL sfp
*
* Revision 6.145  2000/11/06 17:39:50  kans
* added ERR_SEQ_FEAT_mRNAgeneRange
*
* Revision 6.144  2000/11/01 21:43:31  kans
* added DifferentDbxrefs check for FEATDEF_REGION (for same domain from different CDD data sources), relaxed severity for Regions
*
* Revision 6.143  2000/09/27 18:25:20  kans
* prelock components of far delta, just like components of far seg already did
*
* Revision 6.142  2000/09/25 00:08:21  kans
* mRNA and CDS features can have far RefSeq products if done by genome annotation, regular RefSeq nuc-prot sets still checked
*
* Revision 6.141  2000/09/24 22:22:42  kans
* show >350 KB message only if GenBank, EMBL, or DDBJ SeqID in record
*
* Revision 6.140  2000/09/24 00:07:28  kans
* if delta seq > 350 kb, but in genomic product set (genome annotation project, for now), do not post error message
*
* Revision 6.139  2000/09/21 18:22:18  kans
* ribosomal slippage allows translation check, does not allow splice check
*
* Revision 6.138  2000/09/19 14:54:12  kans
* if genomic product set, do not report multiple cds products (still needs work for contig after splitting to suppress far cds product warning) - and do not report missing pub if gps or refseq
*
* Revision 6.137  2000/09/01 23:42:10  kans
* validate genomic product set packaging message now includes mRNA->product Seq-loc
*
* Revision 6.136  2000/08/28 23:20:24  kans
* added ERR_SEQ_FEAT_MultipleMRNAproducts
*
* Revision 6.135  2000/08/28 23:04:27  kans
* added ERR_SEQ_PKG_GenomicProductPackagingProblem
*
* Revision 6.134  2000/08/04 12:52:32  kans
* change logic and message for ERR_SEQ_DESCR_BadOrganelle
*
* Revision 6.133  2000/08/02 22:31:46  kans
* changed ERR_SEQ_DESCR_BadLocation to ERR_SEQ_DESCR_BadOrganelle
*
* Revision 6.132  2000/08/02 22:27:33  kans
* added ERR_SEQ_DESCR_BadLocation
*
* Revision 6.131  2000/07/14 22:46:02  kans
* report position of first ACGT base with zero score and first N base with nonzero score
*
* Revision 6.130  2000/07/14 19:47:17  kans
* allow Phred Quality along with Phrap Quality
*
* Revision 6.129  2000/07/12 15:02:58  kans
* check score against 0 or 100 regardless of min or max, to catch bad bytes if the min or max value were also reported as bad
*
* Revision 6.128  2000/07/06 21:50:21  kans
* start lastloc at -1, set gcp->itemID for all messages
*
* Revision 6.127  2000/07/06 21:11:32  kans
* added ERR_SEQ_GRAPH_GraphOverlap
*
* Revision 6.126  2000/07/06 16:17:40  kans
* once again only counting seqlit with real data for numdsp - introduced bug when separating dsp and sgp counting
*
* Revision 6.125  2000/07/06 16:01:54  kans
* expanded phrap graph error messages
*
* Revision 6.124  2000/07/06 15:25:11  kans
* check for fa2htgs bug
*
* Revision 6.123  2000/07/05 17:02:12  kans
* added spp->gapIsZero, SeqPortSet_do_virtualEx, using ncbi4na with gap of 0 to distinguish quality scores under N versus quality scores under gap
*
* Revision 6.122  2000/07/03 21:22:47  kans
* changed some seqgraph error levels, preparing to figure out if nonzero score is below a gap, report as more severe error
*
* Revision 6.121  2000/07/03 17:11:43  kans
* gphlen should be either seqlen (seqlit sum) or bsplen, also check every value to be within reported range
*
* Revision 6.120  2000/07/03 16:37:28  kans
* multi seqlit deltas can have a single phrap seqgraph, downgrade some errors to warning (for now), may want to allow small positive score for N bases in future
*
* Revision 6.119  2000/06/26 12:58:15  kans
* look for Blast Type instead of Hist Seqalign to detect PowerBLAST alignment
*
* Revision 6.118  2000/06/21 21:57:02  kans
* fix to graph validation
*
* Revision 6.117  2000/06/21 18:01:39  kans
* check residues against quality values, report ACGT with 0 score and N with non-zero score
*
* Revision 6.116  2000/06/21 17:08:23  kans
* check each sgp->loc against running seqlit range
*
* Revision 6.115  2000/06/21 00:02:58  kans
* added ValidateGraphsOnBioseq, still need to look for runs of 0 that are not opposite runs of N
*
* Revision 6.114  2000/06/20 20:30:53  kans
* added ERR_SEQ_ALIGN_BlastAligns
*
* Revision 6.113  2000/06/13 15:14:08  kans
* change pre-locking to only lock remote genome components - otherwise got unlocking 0 lockcnt messages
*
* Revision 6.112  2000/06/12 14:55:19  kans
* added ERR_SEQ_FEAT_InvalidQualifierValue for /rpt_type and /rpt_unit
*
* Revision 6.111  2000/06/09 19:01:00  kans
* added ERR_SEQ_DESCR_BioSourceNeedsFocus
*
* Revision 6.110  2000/05/17 16:12:28  kans
* virion is no longer a legal feature
*
* Revision 6.109  2000/05/16 19:06:05  kans
* check for out-of-phase processed peptide now ignores partial ends
*
* Revision 6.108  2000/05/12 19:00:44  kans
* added ERR_SEQ_FEAT_PeptideFeatOutOfFrame
*
* Revision 6.107  2000/05/12 15:46:36  kans
* fixed typo-induced bug in CheckForCommonCDSProduct
*
* Revision 6.106  2000/05/11 16:14:45  kans
* MultipleCDSproduct check also aborts if sfp->product is NULL
*
* Revision 6.105  2000/05/11 16:12:13  kans
* Do not report ERR_SEQ_FEAT_MultipleCDSproducts if pseudo cds or contained by pseudo gene
*
* Revision 6.104  2000/05/10 18:09:29  kans
* added ERR_SEQ_FEAT_FocusOnBioSourceFeature
*
* Revision 6.103  2000/05/04 14:36:58  kans
* cleared up warnings found by gcc and clcc, and changed implementation of locking and unlocking remote genome segments
*
* Revision 6.102  2000/05/02 19:36:46  kans
* LockOrUnockAllSegments to speed up validation of remote genomes
*
* Revision 6.101  2000/05/02 19:12:06  kans
* added ERR_SEQ_FEAT_MultipleCDSproducts
*
* Revision 6.100  2000/03/14 13:33:33  kans
* NCBISubValidate sets indexing, adds AppProperty to shut off specific messages to be decided later
*
* Revision 6.99  2000/02/18 21:25:34  kans
* added ERR_SEQ_DESCR_SerialInComment and ERR_SEQ_FEAT_SerialInComment
*
* Revision 6.98  2000/02/14 15:00:19  kans
* added vsp->farIDsInAlignments for use by alignment validator
*
* Revision 6.97  2000/02/08 19:10:42  kans
* delta seq okay for htgs_3
*
* Revision 6.96  2000/01/26 23:14:46  kans
* added ERR_SEQ_INST_DuplicateSegmentReferences
*
* Revision 6.95  2000/01/14 21:14:02  kans
* added ERR_SEQ_FEAT_OverlappingPeptideFeat
*
* Revision 6.94  2000/01/11 17:01:46  kans
* changed Burma to Myanmar
*
* Revision 6.93  2000/01/03 20:18:02  kans
* suspicious CDS location message raised to SEV_ERROR if RefSeq NM_ accession
*
* Revision 6.92  1999/12/24 20:05:17  kans
* added ERR_SEQ_INST_IdOnMultipleBioseqs - for once scoping made an important test trivially easy
*
* Revision 6.91  1999/12/24 01:21:05  kans
* added validateAlignments flag controlling call to ValidateSeqAlignWithinValidator
*
* Revision 6.90  1999/12/23 19:07:24  kans
* for CDS, added CheckForBadGeneOverlap and CheckForBadMRNAOverlap
*
* Revision 6.89  1999/12/17 13:07:50  sirotkin
* false -> FALSE
*
* Revision 6.88  1999/12/16 21:57:33  kans
* added test for ERR_SEQ_FEAT_BothStrands
*
* Revision 6.87  1999/12/08 02:40:41  kans
* added ERR_SEQ_INST_SeqIdNameHasSpace
*
* Revision 6.86  1999/12/06 15:23:34  kans
* duplicate features in separate unnamed feature tables were not being detected, now are with slightly different error message
*
* Revision 6.85  1999/11/22 21:56:11  kans
* removed Galapagos Islands (part of Ecuador) and removed space in Cote d*Ivoire
*
* Revision 6.84  1999/11/12 16:51:34  kans
* cDNA-derived STS will not trigger ConflictingBiomolTech error
*
* Revision 6.83  1999/11/09 19:16:28  kans
* added ERR_SEQ_INST_ConflictingBiomolTech
*
* Revision 6.82  1999/11/04 00:14:09  kans
* added ERR_SEQ_DESCR_MissingLineage
*
* Revision 6.81  1999/10/28 20:27:29  kans
* added ERR_SEQ_INST_MolNuclAcid
*
* Revision 6.80  1999/10/01 20:09:50  kans
* fix to feature packaging check that handles segmented bioseqs
*
* Revision 6.79  1999/10/01 19:47:35  kans
* support for ERR_SEQ_PKG_FeaturePackagingProblem
*
* Revision 6.78  1999/10/01 14:42:15  kans
* changed SEV_FATAL to SEV_REJECT
*
* Revision 6.77  1999/09/27 21:04:08  kans
* report ERR_SEQ_DESCR_NoOrgFound if empty taxname and common name
*
* Revision 6.76  1999/09/06 21:36:03  kans
* ValidateSeqEntry sets scope
*
* Revision 6.75  1999/08/24 17:44:01  kans
* removed Wagad from country list
*
* Revision 6.74  1999/08/24 15:22:17  kans
* added Galapagos Islands and Wagad to the country list
*
* Revision 6.73  1999/08/18 20:24:49  kans
* self-recursive call of CheckForInconsistentBiosources was not using tmp, but original sep, resulting in stack overflow in complex records
*
* Revision 6.72  1999/08/17 19:46:12  kans
* ValidatePopSet posts ERR_SEQ_DESCR_InconsistentBioSources
*
* Revision 6.71  1999/08/03 00:13:02  kans
* vsp->suppressContext now causes simplified locations to be written, seqidworst fastashort no locus
*
* Revision 6.70  1999/07/29 15:41:48  kans
* changed Serbia and Montenegro to Yugoslavia
*
* Revision 6.69  1999/07/22 22:04:35  kans
* added suppressContext flag
*
* Revision 6.68  1999/07/15 22:37:32  kans
* ValidateBioSource called once per biosource, not once per bioseq
*
* Revision 6.67  1999/07/15 20:39:22  kans
* suppress no pub warning if seq-submit, which has a cit-sub
*
* Revision 6.66  1999/06/24 19:33:24  kans
* corrected country list
*
* Revision 6.65  1999/06/22 17:15:49  kans
* added ERR_SEQ_DESCR_NoTaxonID
*
* Revision 6.64  1999/06/18 20:57:46  kans
* using collab approved country list
*
* Revision 6.63  1999/06/18 20:21:04  kans
* implemented ERR_SEQ_DESCR_BadCountryCode, indexed descr callback sets proper itemtype, itemID for click responsiveness
*
* Revision 6.62  1999/06/15 20:04:03  kans
* no org or pub anywhere on record now reports context of first bioseq for batch processing
*
* Revision 6.61  1999/06/15 19:45:42  kans
* changed SequenceTooLong to SequenceExceeds350kbp
*
* Revision 6.60  1999/06/14 16:14:20  kans
* added ERR_SEQ_FEAT_TrnaCodonWrong check
*
* Revision 6.59  1999/06/11 18:31:16  kans
* added ERR_SEQ_FEAT_TranslExceptPhase
*
* Revision 6.58  1999/06/09 21:34:29  kans
* stop in protein message gives gene and protein name for reading report later
*
* Revision 6.57  1999/05/07 15:31:20  kans
* added ERR_SEQ_FEAT_UnnecessaryGeneXref
*
* Revision 6.56  1999/05/05 19:11:41  kans
* for no pubs or biosource anywhere, needed to set vsp->gcp for ValidErr/ErrPostItem
*
* Revision 6.55  1999/05/05 13:03:14  kans
* no org or pub anywhere after clearing error counts
*
* Revision 6.54  1999/05/03 20:06:35  kans
* if no pubs or no biosource, report only once, not once per bioseq
*
* Revision 6.53  1999/03/31 20:57:48  kans
* htgs phase 1 and 2 messages also check for phase 0
*
* Revision 6.52  1999/03/04 19:55:49  kans
* inconsistent create_date messages now sev_warning
*
* Revision 6.51  1999/02/25 21:53:58  kans
* relax duplicate feature severity to warning if label or comment are different, or if FEATDEF_PUB
*
* Revision 6.50  1999/02/16 22:19:02  kans
* fixed interval comparison in duplicate feature detection
*
* Revision 6.49  1999/02/02 16:39:10  kans
* added ERR_SEQ_FEAT_DuplicateFeat
*
* Revision 6.48  1999/01/05 23:20:50  kans
* SpliceCheckEx does not check exon junction if partial
*
* Revision 6.47  1998/12/14 22:27:28  kans
* CdTransCheck now deals with termination by polyA
*
* Revision 6.46  1998/12/07 20:00:56  kans
* meant to set bcp = NULL, not bsp = NULL, crashed with segmented protein
*
* Revision 6.45  1998/10/26 20:57:45  kans
* check gene and prot db fields for IllegalDbXref
*
* Revision 6.44  1998/10/23 15:25:57  kans
* added FarLocation warning
*
* Revision 6.43  1998/10/22 16:05:57  kans
* removed labeltype parameter from SeqMgrIndexFeatures, changed index parameter/field to Uint2
*
* Revision 6.42  1998/10/21 14:32:11  kans
* on invalid feature for bioseq, restore itemid itemid and itemtype to avoid weird(er) click association - need to rewrite valid with new index functions, which will give proper items
*
* Revision 6.41  1998/10/20 20:18:10  kans
* mRNA feature is invalid on an mRNA (cDNA) bioseq
*
* Revision 6.40  1998/10/20 18:12:54  kans
* invalid for type (e.g., intron on mRNA) now coerces gcp to have feature itemtype, itemID for selection
*
* Revision 6.39  1998/10/15 17:29:18  kans
* import feature of mat_, sig_, and transit_peptide now flagged as invalid for type
*
* Revision 6.38  1998/09/22 13:12:01  kans
* locationFilter parameter to explore features function
*
* Revision 6.37  1998/09/21 17:29:35  kans
* precursor rna can have intron feature
*
* Revision 6.36  1998/09/17 16:38:14  kans
* added ERR_SEQ_DESCR_NoMolInfoFound
*
* Revision 6.35  1998/09/01 19:25:27  kans
* context parameter in get best protein, get cds/rna given product
*
* Revision 6.34  1998/08/28 22:25:56  kans
* keep track of last biomol, tech, completeness in multiple molinfo descriptors
*
* Revision 6.33  1998/08/26 21:07:48  kans
* added check for ERR_SEQ_INST_ConflictingIdsOnBioseq
*
* Revision 6.32  1998/08/10 16:05:15  kans
* copy some old descriptor checks to Molinfo
*
* Revision 6.31  1998/07/23 14:25:38  kans
* intron and CAAT_signal are illegal on mRNA - first checks molinfo, then resorts to Seq_mol_rna as mRNA criterion
*
* Revision 6.30  1998/07/16 16:06:56  kans
* use ObjMgrGetEntityIDForChoice instead of ObjMgrGetEntityIDForPointer for SeqEntryPtr
*
* Revision 6.29  1998/07/14 18:10:33  kans
* invalid feature for nucleotide now says nucleotide, not protein
*
* Revision 6.28  1998/07/06 18:01:52  kans
* added LIBCALLBACK to SeqMgrExplore function callbacks
*
* Revision 6.27  1998/07/02 17:53:43  kans
* useSeqMgrIndexes field added to ValidStructPtr, validator can use either old (nested gathers) or new (SeqMgr indexing) method
*
* Revision 6.26  1998/06/24 18:49:15  kans
* added missing BioseqContextFree
*
* Revision 6.25  1998/06/22 20:13:21  kans
* gencode mismatch reports biosource and cds codes
*
* Revision 6.24  1998/06/12 20:05:53  kans
* fixed unix compiler warnings
*
* Revision 6.23  1998/04/16 15:12:15  kans
* slight fix to frame > 1 and not at splice site test
*
* Revision 6.22  1998/04/15 21:59:25  kans
* added ERR_SEQ_FEAT_IllegalDbXref
*
* Revision 6.21  1998/04/14 20:57:36  kans
* check for mixed bioseqs in segset, parts set, and for sets within parts set
*
* Revision 6.20  1998/04/14 19:11:25  kans
* improvements to PartialAtSpliceSite and frame > 1 check
*
* Revision 6.19  1998/04/14 18:55:56  kans
* cds frame > 1 but not 5prime partial now also checks for PartialAtSpliceSite
*
* Revision 6.18  1998/04/13 18:10:38  kans
* warn if CDS frame > 1 but not 5prime partial
*
* Revision 6.17  1998/04/02 15:45:51  kans
* MolInfoPtr had not been obtained for Seq_repr_raw for HTGS test on long sequences
*
* Revision 6.16  1998/03/30 17:35:22  kans
* check raw bioseq for htgs flags if greater than 350kb
*
* Revision 6.15  1998/03/18 20:41:50  kans
* SpliceCheck only on mRNA (not all RNAs) and CDS
*
* Revision 6.14  1998/03/09 17:48:46  kans
* OBJ_SEQSUB_CIT now satisfies need for publication
*
* Revision 6.13  1998/02/19 17:21:15  shavirin
* Added check for NULL in ValidErr() function
*
* Revision 6.12  1998/02/18 20:34:55  kans
* added ERR_GENERIC_MissingPubInfo
*
* Revision 6.11  1998/02/09 20:35:35  kans
* calls ERR_SEQ_FEAT_PseudoCdsHasProduct
*
* Revision 6.10  1998/01/30 21:05:54  kans
* check for ERR_SEQ_DESCR_MultipleBioSources
*
* Revision 6.9  1998/01/30 20:29:48  kans
* added PartialAtSpliceSite check
*
* Revision 6.8  1998/01/13 15:34:50  kans
* gbqual_citation satisfied by sfp->cit
*
* Revision 6.7  1998/01/10 00:05:36  kans
* added ValidateImpFeat
*
* Revision 6.6  1998/01/06 03:07:57  ostell
* in comparison of cdregion genetic code to biosource genetic code, set defaults
* to 0 instead of -1 to fix default behavior on building submission.
*
* Revision 6.5  1997/12/18 21:51:43  kans
* warn on cds/biosource genetic code conflict, rna type 0
*
* Revision 6.4  1997/11/14 17:10:13  kans
* added checks for bioseq length > 350K (based on Cavanaugh request)
*
* Revision 6.3  1997/08/27 20:11:02  kans
* order gene should in fact have partial flag set
*
* Revision 6.2  1997/08/27 19:48:32  kans
* print feature product seqloc
*
* Revision 6.1  1997/08/27 14:15:51  kans
* gene of order should not cause partial error
*
* Revision 6.0  1997/08/25 18:08:25  madden
* Revision changed to 6.0
*
* Revision 5.24  1997/08/13 18:52:51  kans
* new packaging errors set to SEV_REJECT
*
* Revision 5.23  1997/08/13 15:36:53  kans
* added NucProtNotSegSet and SegSetNotParts (Bazhin)
*
* Revision 5.22  1997/07/07 21:28:11  kans
* existing bad start codon check was being bypassed, so new one was added
*
* Revision 5.21  1997/07/07 15:00:28  kans
* signal or transit peptide do not need names
*
* Revision 5.20  1997/07/02 19:44:09  kans
* added check for et al, changed symbol names for empty gene and prot feature
*
* Revision 5.19  1997/06/24 16:39:12  kans
* fixed Digital Unix compiler complaint
*
* Revision 5.18  1997/06/19 18:39:51  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.17  1997/05/29 17:25:16  kans
* splice check and trans check not done if excpt
*
* Revision 5.16  1997/05/28 19:10:32  kans
* added check for empty protref
*
* Revision 5.15  1997/05/20 21:11:38  kans
* warnings for delta seq not htgs1 or 2, cds orf with product, gene with no fields, cds exception gbqual without excpt
*
* Revision 5.14  1997/04/24 20:39:20  kans
* invalid splice sites are warning level unless app property forces to error
*
 * Revision 5.13  1997/03/17  21:43:28  kans
 * added closing bracket to bioseq length indication
 *
 * Revision 5.12  1997/02/20  13:50:33  ostell
 * added length check on segmented sequence back
 *
 * Revision 5.11  1996/11/22  17:23:20  kans
 * splice errors on exon imp-feats are now severity warning, since there is
 * no way of knowing which are the unspliced ends of the first and last exon
 *
 * Revision 5.10  1996/11/04  16:29:55  kans
 * app property allows splice check for exon features, and rare GC splice
 * donor has separate warning message
 *
 * Revision 5.9  1996/10/16  20:31:16  ostell
 * added length check for delta sequences
 * added CdTrnsCheck for exception and pseudo
 *
 * Revision 5.8  1996/08/21  14:08:26  ostell
 * rmoved kludge for big sequences
 *
 * Revision 5.7  1996/08/19  02:45:49  ostell
 * added check in BioseqContect for more than 30n bioseqs to control
 * feature checkes
 *
 * Revision 5.6  1996/08/06  19:56:03  kans
 * for SEQLOC_WHOLE, must call SeqIdFindBest on bsp->id
 *
 * Revision 5.5  1996/08/01  18:58:00  kans
 * on pseudo cds, suppress CdTransCheck, SpliceCheck
 *
 * Revision 5.4  1996/06/19  00:35:32  ostell
 * added check for ragged end of CdRegion
 *
 * Revision 5.1  1996/06/16  04:16:05  ostell
 * added support for delta seq
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.19  1996/05/03  18:59:13  kans
 * up to 5 stops still allows mismatch report, which includes nuc position
 *
 * Revision 4.18  1996/04/01  16:31:47  ostell
 * fix to preserver eror message count between invocations
 *
 * Revision 4.17  1996/03/15  20:01:14  ostell
 * in SpliceCheck, give accession of sequence with bad junction
 *
 * Revision 4.16  1996/03/08  14:48:02  kans
 * fixed typos in ValidateSeqEntry scope memset, use as parameter
 *
 * Revision 4.15  1996/03/06  20:43:59  ostell
 * added scoping to validation
 *
 * Revision 4.14  1996/03/05  19:54:29  kans
 * added biosource to two switch statements
 *
 * Revision 4.13  1996/03/03  16:59:34  ostell
 * added SpellCheckPub() to look at more Pub types
 *
 * Revision 4.12  1996/03/02  03:41:43  ostell
 * fix to correctly identigy splice junctions on minus strand
 *
 * Revision 4.11  1996/02/26  22:06:37  ostell
 * finished gatherized version of spell check on descriptors
 *
 * Revision 4.10  1996/02/19  19:58:05  ostell
 * added support for Code-break and tRNA.anticodon
 *
 * Revision 4.9  1996/01/23  23:10:10  kans
 * implemented onlyspell and justwarnonspell code
 *
 * Revision 4.8  1995/12/07  01:55:37  ostell
 * fix to check for NULL on bioseqset parent
 *
 * Revision 4.7  1995/12/07  01:38:56  ostell
 * added Splice error flag
 *
 * Revision 4.6  1995/12/06  22:11:23  ostell
 * changed wording of SpliceCheck message
 *
 * Revision 4.5  1995/12/06  06:08:57  ostell
 * lowered warning levels on partial messages
 * added SpliceCheck()
 *
 * Revision 4.4  1995/08/16  18:21:52  epstein
 * correct declaration of static functions to be consistent with function prototypes
 *
 * Revision 4.3  1995/08/04  18:41:02  madden
 * removed "|SpellErr|" SpellCallBack.
 *
 * Revision 4.2  1995/08/03  12:45:56  madden
 * Set ValNodePtr in SpellCheckBioseqDescr; added "SpellErr" to ErrPosting.
 *
 * Revision 4.1  1995/08/02  22:21:50  madden
 * gatherized the spell functions.
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.14  1995/06/03  13:45:47  ostell
 * changes made in valid to use gather functions and ErrPostItem instead
 * of previous custom functions
 *
 * Revision 1.13  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

static char    *this_module = "valid";

#define THIS_MODULE this_module

static char    *this_file = __FILE__;

#define THIS_FILE this_file

#include <ncbi.h>
#include <objfdef.h>
#include <valid.h>
#include <validerr.h>
#include <sqnutils.h>
#include <gbftdef.h>
#include <gbfeat.h>
#include <objsub.h>
#include <asn2ffp.h>
#include <explore.h>
#include <subutil.h>
#include <tofasta.h>

/*****************************************************************************
*
*   NOTE: look at all the ValidErr calls with severity=0. Some should be
*   bumped up later. Look also for string "PARSER"
*
*****************************************************************************/



#ifdef VAR_ARGS
#include <varargs.h>
#else
#include <stdarg.h>
#endif

static ValidStructPtr globalvsp;        /* for spell checker */

NLM_EXTERN void CDECL ValidErr VPROTO ((ValidStructPtr vsp, int severity, int code1, int code2, const char *fmt, ...));
static void     ValidateBioseqInst (GatherContextPtr gcp);
static void     ValidateBioseqContext (GatherContextPtr gcp);
static void     ValidateBioseqSet (GatherContextPtr gcp);
static void     ValidateGraphsOnBioseq (GatherContextPtr gcp);
static void     SpellCheckSeqDescr (GatherContextPtr gcp);
NLM_EXTERN void CdTransCheck (ValidStructPtr vsp, SeqFeatPtr sfp);
NLM_EXTERN void MrnaTransCheck (ValidStructPtr vsp, SeqFeatPtr sfp);
NLM_EXTERN void ValidateSeqFeat (GatherContextPtr gcp);
NLM_EXTERN void ValidateSeqLoc (ValidStructPtr vsp, SeqLocPtr slp, CharPtr prefix);
NLM_EXTERN Boolean PatchBadSequence (BioseqPtr bsp);
NLM_EXTERN CharPtr FindIDForEntry (SeqEntryPtr sep, CharPtr buf);
NLM_EXTERN void SpellCheckSeqFeat (GatherContextPtr gcp);
NLM_EXTERN void SpellCheckString (ValidStructPtr vsp, CharPtr str);
NLM_EXTERN void SpliceCheck (ValidStructPtr vsp, SeqFeatPtr sfp);
static void     SpliceCheckEx (ValidStructPtr vsp, SeqFeatPtr sfp, Boolean checkAll);
static void     ValidateBioSource (ValidStructPtr vsp, GatherContextPtr gcp, BioSourcePtr biop);
static void     ValidatePubdesc (ValidStructPtr vsp, GatherContextPtr gcp, PubdescPtr pdp);
static void     ValidateSfpCit (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp);

/* alignment validator */
NLM_EXTERN Boolean ValidateSeqAlignWithinValidator (ValidStructPtr vsp, SeqEntryPtr sep);

/*****************************************************************************
*
*   Perform Validation Checks on a SeqEntry
*
*****************************************************************************/

NLM_EXTERN void ValidStructClear (ValidStructPtr vsp)
{                               /* 0 out a ValidStruct */
  CharPtr         errbuf;
  Int2            cutoff;
  Boolean         patch_seq;
  SpellCheckFunc  spellfunc;
  SpellCallBackFunc spellcallback;
  Boolean         onlyspell;
  Boolean         justwarnonspell;
  Boolean         useSeqMgrIndexes;
  Boolean         suppressContext;
  Boolean         validateAlignments;
  Boolean         farIDsInAlignments;

  if (vsp == NULL)
    return;

  errbuf = vsp->errbuf;
  cutoff = vsp->cutoff;
  patch_seq = vsp->patch_seq;
  spellfunc = vsp->spellfunc;
  spellcallback = vsp->spellcallback;
  onlyspell = vsp->onlyspell;
  justwarnonspell = vsp->justwarnonspell;
  useSeqMgrIndexes = vsp->useSeqMgrIndexes;
  suppressContext = vsp->suppressContext;
  validateAlignments = vsp->validateAlignments;
  farIDsInAlignments = vsp->farIDsInAlignments;
  MemSet ((VoidPtr) vsp, 0, sizeof (ValidStruct));
  vsp->errbuf = errbuf;
  vsp->cutoff = cutoff;
  vsp->patch_seq = patch_seq;
  vsp->spellfunc = spellfunc;
  vsp->spellcallback = spellcallback;
  vsp->onlyspell = onlyspell;
  vsp->justwarnonspell = justwarnonspell;
  vsp->useSeqMgrIndexes = useSeqMgrIndexes;
  vsp->suppressContext = suppressContext;
  vsp->validateAlignments = validateAlignments;
  vsp->farIDsInAlignments = farIDsInAlignments;
  return;
}

NLM_EXTERN ValidStructPtr ValidStructNew (void)
{
  ValidStructPtr  vsp;

  vsp = (ValidStructPtr) MemNew (sizeof (ValidStruct));
  return vsp;
}

NLM_EXTERN ValidStructPtr ValidStructFree (ValidStructPtr vsp)
{
  if (vsp == NULL)
    return vsp;

  MemFree (vsp->errbuf);
  return (ValidStructPtr) MemFree (vsp);
}

/*****************************************************************************
*
*   ValidErr()
*
*****************************************************************************/

static void ChangeSeqIdToBestID (SeqIdPtr sip)
{
  BioseqPtr       bsp;
  SeqIdPtr        id;
  Pointer         pnt;

  if (sip == NULL)
    return;
  bsp = BioseqFindCore (sip);
  if (bsp == NULL)
    return;
  id = SeqIdDup (SeqIdFindWorst (bsp->id));
  if (id == NULL)
    return;
  /* now remove SeqId contents to reuse SeqId valnode */
  pnt = sip->data.ptrvalue;
  switch (sip->choice) {
  case SEQID_LOCAL:            /* local */
    ObjectIdFree ((ObjectIdPtr) pnt);
    break;
  case SEQID_GIBBSQ:           /* gibbseq */
  case SEQID_GIBBMT:           /* gibbmt */
    break;
  case SEQID_GIIM:             /* giimid */
    GiimFree ((GiimPtr) pnt);
    break;
  case SEQID_GENBANK:          /* genbank */
  case SEQID_EMBL:             /* embl */
  case SEQID_PIR:              /* pir   */
  case SEQID_SWISSPROT:        /* swissprot */
  case SEQID_OTHER:            /* other */
  case SEQID_DDBJ:
  case SEQID_PRF:
  case SEQID_TPG:
  case SEQID_TPE:
  case SEQID_TPD:
    TextSeqIdFree ((TextSeqIdPtr) pnt);
    break;
  case SEQID_PATENT:           /* patent seq id */
    PatentSeqIdFree ((PatentSeqIdPtr) pnt);
    break;
  case SEQID_GENERAL:          /* general */
    DbtagFree ((DbtagPtr) pnt);
    break;
  case SEQID_GI:               /* gi */
    break;
  case SEQID_PDB:
    PDBSeqIdFree ((PDBSeqIdPtr) pnt);
    break;
  }
  sip->choice = id->choice;
  sip->data.ptrvalue = id->data.ptrvalue;
  SeqIdStripLocus (sip);
}

static void ChangeSeqLocToBestID (SeqLocPtr slp)
{
  SeqLocPtr       loc;
  PackSeqPntPtr   psp;
  SeqBondPtr      sbp;
  SeqIntPtr       sinp;
  SeqIdPtr        sip;
  SeqPntPtr       spp;

  while (slp != NULL) {
    switch (slp->choice) {
    case SEQLOC_NULL:
      break;
    case SEQLOC_EMPTY:
    case SEQLOC_WHOLE:
      sip = (SeqIdPtr) slp->data.ptrvalue;
      ChangeSeqIdToBestID (sip);
      break;
    case SEQLOC_INT:
      sinp = (SeqIntPtr) slp->data.ptrvalue;
      if (sinp != NULL) {
        sip = sinp->id;
        ChangeSeqIdToBestID (sip);
      }
      break;
    case SEQLOC_PNT:
      spp = (SeqPntPtr) slp->data.ptrvalue;
      if (spp != NULL) {
        sip = spp->id;
        ChangeSeqIdToBestID (sip);
      }
      break;
    case SEQLOC_PACKED_PNT:
      psp = (PackSeqPntPtr) slp->data.ptrvalue;
      if (psp != NULL) {
        sip = psp->id;
        ChangeSeqIdToBestID (sip);
      }
      break;
    case SEQLOC_PACKED_INT:
    case SEQLOC_MIX:
    case SEQLOC_EQUIV:
      loc = (SeqLocPtr) slp->data.ptrvalue;
      while (loc != NULL) {
        ChangeSeqLocToBestID (loc);
        loc = loc->next;
      }
      break;
    case SEQLOC_BOND:
      sbp = (SeqBondPtr) slp->data.ptrvalue;
      if (sbp != NULL) {
        spp = (SeqPntPtr) sbp->a;
        if (spp != NULL) {
          sip = spp->id;
          ChangeSeqIdToBestID (sip);
        }
        spp = (SeqPntPtr) sbp->b;
        if (spp != NULL) {
          sip = spp->id;
          ChangeSeqIdToBestID (sip);
        }
      }
      break;
    case SEQLOC_FEAT:
      break;
    default:
      break;
    }
    slp = slp->next;
  }
}

static Int2 WorstBioseqLabel (BioseqPtr bsp, CharPtr buffer, Int2 buflen, Uint1 content)
{
  CharPtr         tmp;
  Char            label[40];
  Int2            diff, len;
  SeqIdPtr        sip;
  AsnModulePtr    amp;
  AsnTypePtr      ratp, matp;

  if ((bsp == NULL) || (buflen < 1))
    return 0;

  len = buflen;
  label[0] = '\0';

  if (content != OM_LABEL_TYPE) {
    sip = SeqIdStripLocus (SeqIdDup (SeqIdFindWorst (bsp->id)));
    SeqIdWrite (sip, label, PRINTID_FASTA_SHORT, 39);
    SeqIdFree (sip);
    if (content == OM_LABEL_CONTENT)
      return LabelCopy (buffer, label, buflen);

    diff = LabelCopyExtra (buffer, label, buflen, NULL, ": ");
    buflen -= diff;
    buffer += diff;
  }

  amp = AsnAllModPtr ();
  ratp = AsnTypeFind (amp, "Seq-inst.repr");
  matp = AsnTypeFind (amp, "Seq-inst.mol");

  label[0] = '\0';
  tmp = label;
  tmp = StringMove (tmp, AsnEnumTypeStr (ratp, (Int2) (bsp->repr)));
  tmp = StringMove (tmp, ", ");
  tmp = StringMove (tmp, AsnEnumTypeStr (matp, (Int2) (bsp->mol)));
  sprintf (tmp, " len= %ld", (long) (bsp->length));
  diff = LabelCopy (buffer, label, buflen);
  buflen -= diff;
  buffer += diff;

  if (content != OM_LABEL_SUMMARY)
    return (len - buflen);

  return (len - buflen);        /* SUMMARY not done yet */
}

#ifdef VAR_ARGS
NLM_EXTERN void CDECL ValidErr (vsp, severity, code1, code2, fmt, va_alist)
     ValidStructPtr vsp;
     int severity;
     int code1;
     int code2;
     const char     *fmt;
     va_dcl
#else
NLM_EXTERN void CDECL ValidErr (ValidStructPtr vsp, int severity, int code1, int code2, const char *fmt, ...)
#endif
{
  va_list         args;
  GatherContextPtr gcp;
  CharPtr         tmp, ctmp;
  Int2            buflen, diff;
  BioseqPtr       bsp;
  SeqIdPtr        sip;
  SeqLocPtr       loc = NULL;

  if (vsp == NULL || severity < vsp->cutoff)
    return;

  if (vsp->errbuf == NULL) {
    vsp->errbuf = MemNew (1024);
    if (vsp->errbuf == NULL)
      AbnormalExit (1);
  }
  tmp = vsp->errbuf;

  vsp->errors[severity]++;

#ifdef VAR_ARGS
  va_start (args);
#else
  va_start (args, fmt);
#endif

  gcp = vsp->gcp;
  buflen = 1023;
  vsprintf (tmp, fmt, args);
  while (*tmp != '\0') {
    buflen--;
    tmp++;
  }

  va_end (args);

  if (vsp->sfp != NULL) {
    diff = LabelCopy (tmp, " FEATURE: ", buflen);
    buflen -= diff;
    tmp += diff;

    diff = FeatDefLabel (vsp->sfp, tmp, buflen, OM_LABEL_BOTH);
    buflen -= diff;
    tmp += diff;

    if (vsp->suppressContext) {
      loc = AsnIoMemCopy (vsp->sfp->location, (AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
      ChangeSeqLocToBestID (loc);
      ctmp = SeqLocPrint (loc);
      SeqLocFree (loc);
    } else {
      ctmp = SeqLocPrint (vsp->sfp->location);
    }
    if (ctmp != NULL) {
      diff = LabelCopyExtra (tmp, ctmp, buflen, " [", "]");
      buflen -= diff;
      tmp += diff;
      MemFree (ctmp);
    }

    if (!vsp->suppressContext) {
      sip = SeqLocId (vsp->sfp->location);
      if (sip != NULL) {
        bsp = BioseqFind (sip);
        if (bsp != NULL) {
          diff = LabelCopy (tmp, " [", buflen);
          buflen -= diff;
          tmp += diff;

          diff = BioseqLabel (bsp, tmp, buflen, OM_LABEL_BOTH);
          buflen -= diff;
          tmp += diff;

          diff = LabelCopy (tmp, "]", buflen);
          buflen -= diff;
          tmp += diff;
        }
      }
    }
    if (vsp->sfp->product != NULL) {
      if (vsp->suppressContext) {
        loc = AsnIoMemCopy (vsp->sfp->product, (AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
        ChangeSeqLocToBestID (loc);
        ctmp = SeqLocPrint (loc);
        SeqLocFree (loc);
      } else {
        ctmp = SeqLocPrint (vsp->sfp->product);
      }
      if (ctmp != NULL) {
        diff = LabelCopyExtra (tmp, ctmp, buflen, " -> [", "]");
        buflen -= diff;
        tmp += diff;
        MemFree (ctmp);
      }
    }
  } else if (vsp->descr != NULL) {
    diff = LabelCopy (tmp, " DESCRIPTOR: ", buflen);
    buflen -= diff;
    tmp += diff;

    diff = SeqDescLabel (vsp->descr, tmp, buflen, OM_LABEL_BOTH);
    buflen -= diff;
    tmp += diff;
  }

  /*
     if (vsp->suppressContext)
     {
     }
     else */
  if (vsp->sfp == NULL) {       /* sfp adds its own context */
    if (vsp->bsp != NULL) {
      diff = LabelCopy (tmp, " BIOSEQ: ", buflen);
      buflen -= diff;
      tmp += diff;

      if (vsp->suppressContext) {
        diff = WorstBioseqLabel (vsp->bsp, tmp, buflen, OM_LABEL_CONTENT);
      } else {
        diff = BioseqLabel (vsp->bsp, tmp, buflen, OM_LABEL_BOTH);
      }
      buflen -= diff;
      tmp += diff;
    } else if (vsp->bssp != NULL) {
      diff = LabelCopy (tmp, " BIOSEQ-SET: ", buflen);
      buflen -= diff;
      tmp += diff;

      if (vsp->suppressContext) {
        diff = BioseqSetLabel (vsp->bssp, tmp, buflen, OM_LABEL_CONTENT);
      } else {
        diff = BioseqSetLabel (vsp->bssp, tmp, buflen, OM_LABEL_BOTH);
      }
      buflen -= diff;
      tmp += diff;
    }
  }

  ErrPostItem ((ErrSev) (severity), code1, code2, "%s", vsp->errbuf);
  vsp->errbuf[0] = '\0';

  return;
}

/*****************************************************************************
*
*   Valid1GatherProc(gcp)
*     top level gather callback
*     dispatches to other levels
*
*****************************************************************************/
static Boolean Valid1GatherProc (GatherContextPtr gcp)
{
  ValidStructPtr  vsp;
  AnnotDescrPtr   desc;
  SeqAnnotPtr     sap;
  ObjectIdPtr     oip;
  Boolean         is_blast_align;
  SeqFeatPtr      sfp;
  ValNodePtr      sdp;
  BioSourcePtr    biop;
  PubdescPtr      pdp;

  vsp = (ValidStructPtr) (gcp->userdata);
  vsp->gcp = gcp;               /* needed for ValidErr */

  switch (gcp->thistype) {
  case OBJ_BIOSEQ:
    if (!vsp->onlyspell) {
      ValidateBioseqInst (gcp);
      ValidateBioseqContext (gcp);
      ValidateGraphsOnBioseq (gcp);
    }
    break;
  case OBJ_BIOSEQSET:
    if (!vsp->onlyspell) {
      ValidateBioseqSet (gcp);
    }
    break;
  case OBJ_SEQANNOT:
    if (!vsp->onlyspell) {
      sap = (SeqAnnotPtr) gcp->thisitem;
      if (sap != NULL && sap->type == 2) {
        is_blast_align = FALSE;
        desc = NULL;
        while ((desc = ValNodeFindNext (sap->desc, desc, Annot_descr_user)) != NULL) {
          if (desc->data.ptrvalue != NULL) {
            oip = ((UserObjectPtr) desc->data.ptrvalue)->type;
            if (oip != NULL && StringCmp (oip->str, "Blast Type") == 0) {
              is_blast_align = TRUE;
            }
          }
        }
        if (is_blast_align) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_ALIGN_BlastAligns, "Record contains BLAST alignments");
        }
      }
    }
    break;
  case OBJ_SEQFEAT:
    if (!vsp->onlyspell) {
      ValidateSeqFeat (gcp);
      sfp = (SeqFeatPtr) (gcp->thisitem);
      if (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_BIOSRC) {
          biop = (BioSourcePtr) sfp->data.value.ptrvalue;
          ValidateBioSource (vsp, gcp, biop);
        }
        if (sfp->data.choice == SEQFEAT_PUB) {
          pdp = (PubdescPtr) sfp->data.value.ptrvalue;
          ValidatePubdesc (vsp, gcp, pdp);
        }
        if (sfp->cit != NULL) {
          ValidateSfpCit (vsp, gcp, sfp);
        }
      }
    }
    SpellCheckSeqFeat (gcp);
    break;
  case OBJ_SEQDESC:
    SpellCheckSeqDescr (gcp);
                        /**
			ValidateSeqDescr (gcp);
		    **/
    sdp = (ValNodePtr) (gcp->thisitem);
    if (sdp != NULL) {
      if (sdp->choice == Seq_descr_source) {
        biop = (BioSourcePtr) sdp->data.ptrvalue;
        ValidateBioSource (vsp, gcp, biop);
      }
      if (sdp->choice == Seq_descr_pub) {
        pdp = (PubdescPtr) sdp->data.ptrvalue;
        ValidatePubdesc (vsp, gcp, pdp);
      }
      if (sdp->choice == Seq_descr_mol_type) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "MolType descriptor is obsolete");
      }
      if (sdp->choice == Seq_descr_modif) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Modif descriptor is obsolete");
      }
      if (sdp->choice == Seq_descr_method) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Method descriptor is obsolete");
      }
      if (sdp->choice == Seq_descr_org) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "OrgRef descriptor is obsolete");
      }
    }
    break;
  default:
    break;

  }
  return TRUE;
}

static void LookForAnyPubAndOrg (SeqEntryPtr sep, BoolPtr no_pub, BoolPtr no_biosrc)
{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  SeqAnnotPtr     sap = NULL;
  ValNodePtr      sdp = NULL;
  SeqFeatPtr      sfp;
  SeqEntryPtr     tmp;

  if (sep == NULL || no_pub == NULL || no_biosrc == NULL)
    return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL)
      return;
    sap = bsp->annot;
    sdp = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL)
      return;
    for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
      LookForAnyPubAndOrg (tmp, no_pub, no_biosrc);
    }
    sap = bssp->annot;
    sdp = bssp->descr;
  } else
    return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_PUB) {
          *no_pub = FALSE;
        } else if (sfp->data.choice == SEQFEAT_BIOSRC) {
          *no_biosrc = FALSE;
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_pub) {
      *no_pub = FALSE;
    } else if (sdp->choice == Seq_descr_source) {
      *no_biosrc = FALSE;
    }
    sdp = sdp->next;
  }
}

static void CheckFeatPacking (BioseqPtr bsp, SeqFeatPtr sfp, Uint4Ptr num_misplaced_features)
{
  SeqAnnotPtr     sap;
  BioseqSetPtr    bssp, parent;
  BioseqPtr       par;

  if (sfp->idx.parenttype == OBJ_SEQANNOT) {
    sap = (SeqAnnotPtr) sfp->idx.parentptr;
    if (sap == NULL)
      return;
    if (sap->idx.parenttype == OBJ_BIOSEQ) {
      /* if feature packaged on bioseq, must be target bioseq */
      par = (BioseqPtr) sap->idx.parentptr;
      if (par != bsp && SeqMgrGetParentOfPart (par, NULL) != bsp) {
        (*num_misplaced_features)++;
      }
      return;
    }
    if (sap->idx.parenttype == OBJ_BIOSEQSET) {
      /* if feature packaged on set, set must contain bioseq */
      bssp = (BioseqSetPtr) sap->idx.parentptr;
      if (bssp == NULL)
        return;
      if (bsp->idx.parenttype == OBJ_BIOSEQSET) {
        parent = (BioseqSetPtr) bsp->idx.parentptr;
        while (parent != NULL) {
          if (parent == bssp)
            return;
          if (parent->idx.parenttype != OBJ_BIOSEQSET)
            return;
          parent = (BioseqSetPtr) parent->idx.parentptr;
        }
        (*num_misplaced_features)++;
      }
    }
  }
}

static Boolean LIBCALLBACK CountMisplacedFeatures (BioseqPtr bsp, SeqMgrBioseqContextPtr bcontext)
{
  Uint4Ptr        num_misplaced_features;
  SeqFeatPtr      sfp;
  SeqMgrFeatContext fcontext;

  num_misplaced_features = (Uint4Ptr) bcontext->userdata;
  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL) {
    CheckFeatPacking (bsp, sfp, num_misplaced_features);
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }

  return TRUE;
}

static Boolean IsRefSeq (BioseqPtr bsp)
{
  SeqIdPtr        sip;

  if (bsp == NULL)
    return FALSE;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER)
      return TRUE;
  }
  return FALSE;
}

NLM_EXTERN Boolean ValidateSeqEntry (SeqEntryPtr sep, ValidStructPtr vsp)
{
  Uint2           entityID = 0;
  GatherScope     gs;
  BioseqSetPtr    bssp;
  SeqSubmitPtr    ssp;
  Boolean         do_many = FALSE;
  Boolean         mult_subs = FALSE;
  Boolean         first = TRUE;
  Int2            errors[6], i;
  Boolean         suppress_no_pubs = TRUE;
  Boolean         suppress_no_biosrc = TRUE;
  Uint4           num_misplaced_features = 0;
  GatherContextPtr gcp = NULL;
  GatherContext   gc;
  SeqEntryPtr     fsep;
  BioseqPtr       fbsp = NULL;
  ObjMgrDataPtr   omdp;
  SeqEntryPtr     oldsep;
  SeqEntryPtr     topsep;
  SeqEntryPtr     tmp;
  ValNodePtr      bsplist;
  SeqIdPtr        sip;
  Boolean         isGPS = FALSE;
  Boolean         isPatent = FALSE;
  Boolean         isPDB = FALSE;

  for (i = 0; i < 6; i++)       /* keep errors between clears */
    errors[i] = 0;

  if (vsp->useSeqMgrIndexes) {
    entityID = ObjMgrGetEntityIDForChoice (sep);

    if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
      SeqMgrIndexFeatures (entityID, NULL);
    }
    SeqMgrExploreBioseqs (entityID, NULL, (Pointer) &num_misplaced_features, CountMisplacedFeatures, TRUE, TRUE, TRUE);
  } else {

    /* if not using indexing, still need feature->idx.subtype now */

    entityID = ObjMgrGetEntityIDForChoice (sep);
    AssignIDsInEntity (entityID, 0, NULL);
  }

  /* Seq-submit can have multiple entries with no Bioseq-set wrapper */

  omdp = ObjMgrGetData (entityID);
  if (omdp != NULL && omdp->datatype == OBJ_SEQSUB) {
    ssp = (SeqSubmitPtr) omdp->dataptr;
    if (ssp != NULL && ssp->data != NULL) {
      if (sep->next != NULL) {
        do_many = TRUE;
        mult_subs = TRUE;
      }
    }
  }

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) (sep->data.ptrvalue);
    switch (bssp->_class) {
    case BioseqseqSet_class_genbank:
    case BioseqseqSet_class_pir:
    case BioseqseqSet_class_gibb:
    case BioseqseqSet_class_gi:
    case BioseqseqSet_class_swissprot:
      sep = bssp->seq_set;
      do_many = TRUE;
      break;
    case BioseqseqSet_class_gen_prod_set:
      isGPS = TRUE;
    default:
      break;
    }
  }

  /* if no pubs or biosource, only one message, not one per bioseq */

  if (mult_subs) {
    for (tmp = sep; tmp != NULL; tmp = tmp->next) {
      LookForAnyPubAndOrg (tmp, &suppress_no_pubs, &suppress_no_biosrc);
    }
  } else {
    LookForAnyPubAndOrg (sep, &suppress_no_pubs, &suppress_no_biosrc);
  }

  globalvsp = vsp;              /* for spell checker */

  while (sep != NULL) {
    MemSet (&gs, 0, sizeof (GatherScope));
    gs.scope = sep;             /* default is to scope to this set */

    ValidStructClear (vsp);
    vsp->sep = sep;

    MemSet ((Pointer) &gc, 0, sizeof (GatherContext));
    gcp = &gc;
    gc.entityID = ObjMgrGetEntityIDForChoice (sep);
    gc.itemID = 1;
    if (IS_Bioseq (sep)) {
      gc.thistype = OBJ_BIOSEQ;
    } else {
      gc.thistype = OBJ_BIOSEQSET;
    }
    vsp->gcp = gcp;             /* above needed for ValidErr */
    vsp->suppress_no_pubs = suppress_no_pubs;
    vsp->suppress_no_biosrc = suppress_no_biosrc;

    /* build seqmgr feature indices if not already done */

    bsplist = NULL;
    if (vsp->useSeqMgrIndexes) {
      entityID = ObjMgrGetEntityIDForChoice (sep);

      if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
        SeqMgrIndexFeatures (entityID, NULL);
      }

      /* lock all remote genome components in advance */

      bsplist = LockFarComponents (sep);
    }

    fsep = FindNthBioseq (sep, 1);
    fbsp = NULL;
    if (fsep != NULL && IS_Bioseq (fsep)) {
      fbsp = (BioseqPtr) fsep->data.ptrvalue;
      /* report context as first bioseq */
      vsp->bsp = fbsp;
    }

    for (sip = fbsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_PATENT) {
        isPatent = TRUE;
      } else if (sip->choice == SEQID_PDB) {
        isPDB = TRUE;
      }
    }

    if (first) {
      if (suppress_no_pubs) {
        omdp = ObjMgrGetData (gc.entityID);
        if (omdp == NULL || omdp->datatype != OBJ_SEQSUB) {
          if ((!isGPS) && (!IsRefSeq (fbsp))) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_NoPubFound, "No publications anywhere on this entire record.");
          }
        }
      }
      if (suppress_no_biosrc) {
        if ((!isPatent) && ((!isPDB))) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_NoOrgFound, "No organism name anywhere on this entire record.");
        }
      }

      if (num_misplaced_features > 1) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_FeaturePackagingProblem, "There are %d mispackaged features in this record.", (int) num_misplaced_features);
      } else if (num_misplaced_features == 1) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_FeaturePackagingProblem, "There is %d mispackaged feature in this record.", (int) num_misplaced_features);
      }
      first = FALSE;
    }

    vsp->bsp = NULL;

    topsep = GetTopSeqEntryForEntityID (gc.entityID);
    oldsep = SeqEntrySetScope (topsep);

    AssignIDsInEntity (gc.entityID, 0, NULL);

    GatherSeqEntry (sep, (Pointer) vsp, Valid1GatherProc, &gs);

    if (vsp->validateAlignments) {
      vsp->gcp = NULL;
      ValidateSeqAlignWithinValidator (vsp, sep);
      vsp->gcp = NULL;
    }

    SeqEntrySetScope (oldsep);

    if (vsp->useSeqMgrIndexes) {

      /* unlock all pre-locked remote genome components */

      bsplist = UnlockFarComponents (bsplist);
    }

    if (do_many) {
      for (i = 0; i < 6; i++)
        errors[i] += vsp->errors[i];
      sep = sep->next;
    } else
      sep = NULL;
  }

  if (do_many) {
    for (i = 0; i < 6; i++)
      vsp->errors[i] = errors[i];
  }

  return TRUE;
}

static void ValidateSetContents (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
  BioseqPtr       bsp;
  ValidStructPtr  vsp;

  vsp = (ValidStructPtr) data;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) (sep->data.ptrvalue);
    if (ISA_aa (bsp->mol))
      vsp->protcnt++;
    else
      vsp->nuccnt++;
    if (bsp->repr == Seq_repr_seg)
      vsp->segcnt++;

  }
  return;
}


static CharPtr GetBioseqSetClass (Uint1 cl)
{
  if (cl == BioseqseqSet_class_nuc_prot)
    return ("nuc-prot");
  if (cl == BioseqseqSet_class_segset)
    return ("segset");
  if (cl == BioseqseqSet_class_conset)
    return ("conset");
  if (cl == BioseqseqSet_class_parts)
    return ("parts");
  if (cl == BioseqseqSet_class_gibb)
    return ("gibb");
  if (cl == BioseqseqSet_class_gi)
    return ("gi");
  if (cl == BioseqseqSet_class_genbank)
    return ("genbank");
  if (cl == BioseqseqSet_class_pir)
    return ("pir");
  if (cl == BioseqseqSet_class_pub_set)
    return ("pub-set");
  if (cl == BioseqseqSet_class_equiv)
    return ("equiv");
  if (cl == BioseqseqSet_class_swissprot)
    return ("swissprot");
  if (cl == BioseqseqSet_class_pdb_entry)
    return ("pdb-entry");
  if (cl == BioseqseqSet_class_mut_set)
    return ("mut-set");
  if (cl == BioseqseqSet_class_pop_set)
    return ("pop-set");
  if (cl == BioseqseqSet_class_phy_set)
    return ("phy-set");
  if (cl == BioseqseqSet_class_other)
    return ("other");
  return ("not-set");
}

static void IfInGPSmustBeMrnaProduct (ValidStructPtr vsp, BioseqPtr bsp)
{
  BioseqSetPtr    bssp;
  SeqEntryPtr     sep;

  /* see if in genomic product */

  sep = vsp->sep;
  if (sep != NULL && IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set) {
      if (SeqMgrGetRNAgivenProduct (bsp, NULL) == NULL) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_PKG_GenomicProductPackagingProblem, "Nucleotide bioseq should be product of mRNA feature on contig, but is not");
      }
    }
  }
}

static void ValidateNucProtSet (BioseqSetPtr bssp, ValidStructPtr vsp)
{
  SeqEntryPtr     sep;
  BioseqPtr       bsp;
  BioseqSetPtr    bssp1;

  if (bssp->_class != BioseqseqSet_class_nuc_prot)
    return;

  for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
    if (IS_Bioseq (sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp != NULL && ISA_na (bsp->mol)) {
        IfInGPSmustBeMrnaProduct (vsp, bsp);
      }
    }

    if (!IS_Bioseq_set (sep))
      continue;

    bssp1 = sep->data.ptrvalue;
    if (bssp1 == NULL)
      continue;

    if (bssp1->_class != BioseqseqSet_class_segset) {
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_NucProtNotSegSet,
                "Nuc-prot Bioseq-set contains wrong Bioseq-set, its class is \"%s\".", GetBioseqSetClass (bssp1->_class));
      break;
    }
  }
}

static void ValidateSegmentedSet (BioseqSetPtr bssp, ValidStructPtr vsp)
{
  SeqEntryPtr     sep;
  BioseqSetPtr    bssp1;
  BioseqPtr       bsp;
  Uint1           mol = 0;

  if (bssp->_class != BioseqseqSet_class_segset)
    return;

  for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
    if (IS_Bioseq (sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp != NULL) {
        if (mol == 0 || mol == Seq_mol_other) {
          mol = bsp->mol;
        } else if (bsp->mol != Seq_mol_other) {
          if (ISA_na (bsp->mol) != ISA_na (mol)) {
            ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_SegSetMixedBioseqs, "Segmented set contains mixture of nucleotides and proteins");
          }
        }
      }
    }

    if (!IS_Bioseq_set (sep))
      continue;

    bssp1 = sep->data.ptrvalue;
    if (bssp1 == NULL)
      continue;

    if (bssp1->_class != BioseqseqSet_class_parts) {
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_SegSetNotParts,
                "Segmented set contains wrong Bioseq-set, its class is \"%s\".", GetBioseqSetClass (bssp1->_class));
      break;
    }
  }
}

static void ValidatePartsSet (BioseqSetPtr bssp, ValidStructPtr vsp)
{
  SeqEntryPtr     sep;
  BioseqSetPtr    bssp1;
  BioseqPtr       bsp;
  Uint1           mol = 0;

  if (bssp->_class != BioseqseqSet_class_parts)
    return;

  for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
    if (IS_Bioseq (sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp != NULL) {
        if (mol == 0 || mol == Seq_mol_other) {
          mol = bsp->mol;
        } else if (bsp->mol != Seq_mol_other) {
          if (ISA_na (bsp->mol) != ISA_na (mol)) {
            ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_PartsSetMixedBioseqs, "Parts set contains mixture of nucleotides and proteins");
            break;
          }
        }
      }
    }
  }

  for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
    if (IS_Bioseq_set (sep)) {
      bssp1 = sep->data.ptrvalue;
      if (bssp1 == NULL)
        continue;

      ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_PartsSetHasSets,
                "Parts set contains unwanted Bioseq-set, its class is \"%s\".", GetBioseqSetClass (bssp1->_class));
      break;
    }
  }
}

static Boolean CheckForInconsistentBiosources (SeqEntryPtr sep, ValidStructPtr vsp, OrgRefPtr PNTR orpp)
{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  SeqEntryPtr     tmp;
  ValNodePtr      sdp;
  SeqFeatPtr      sfp;
  SeqMgrDescContext dcontext;
  SeqMgrFeatContext fcontext;
  BioSourcePtr    biop;
  OrgRefPtr       orp;
  OrgRefPtr       firstorp;
  GatherContextPtr gcp;
  Uint2           entityID = 0, oldEntityID;
  Uint2           itemID = 0, oldItemID;
  Uint2           itemtype = 0, oldItemtype;

  if (sep == NULL || vsp == NULL || orpp == NULL)
    return FALSE;
  gcp = vsp->gcp;

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL)
      return FALSE;
    for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
      if (CheckForInconsistentBiosources (tmp, vsp, orpp))
        return TRUE;
    }
    return FALSE;
  }

  if (!IS_Bioseq (sep))
    return FALSE;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL)
    return FALSE;

  biop = NULL;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop = (BioSourcePtr) sdp->data.ptrvalue;
    entityID = dcontext.entityID;
    itemID = dcontext.itemID;
    itemtype = OBJ_SEQDESC;
  } else {
    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext);
    if (sfp != NULL) {
      biop = (BioSourcePtr) sfp->data.value.ptrvalue;
      entityID = fcontext.entityID;
      itemID = fcontext.itemID;
      itemtype = OBJ_SEQFEAT;
    }
  }
  if (biop == NULL)
    return FALSE;
  orp = biop->org;
  if (orp == NULL)
    return FALSE;

  firstorp = *orpp;
  if (firstorp == NULL) {
    *orpp = orp;
    return FALSE;
  }

  if (StringNICmp (orp->taxname, "Influenza virus ", 16) == 0 &&
      StringNICmp (firstorp->taxname, "Influenza virus ", 16) == 0 && StringNICmp (orp->taxname, firstorp->taxname, 17) == 0) {
    return FALSE;
  }

  if (StringICmp (orp->taxname, firstorp->taxname) == 0)
    return FALSE;

  oldEntityID = gcp->entityID;
  oldItemID = gcp->itemID;
  oldItemtype = gcp->thistype;

  gcp->entityID = entityID;
  gcp->itemID = itemID;
  gcp->thistype = itemtype;

  /* only report the first one that doesn't match */

  ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InconsistentBioSources, "Population set contains inconsistent organisms.");

  gcp->entityID = oldEntityID;
  gcp->itemID = oldItemID;
  gcp->thistype = oldItemtype;

  return TRUE;
}

static void ValidatePopSet (BioseqSetPtr bssp, ValidStructPtr vsp)
{
  OrgRefPtr       orp = NULL;
  SeqEntryPtr     sep;

  if (bssp->_class != BioseqseqSet_class_pop_set)
    return;

  for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
    if (CheckForInconsistentBiosources (sep, vsp, &orp))
      return;
  }
}

static void ValidateGenProdSet (BioseqSetPtr bssp, ValidStructPtr vsp)
{
  BioseqPtr       bsp;
  BioseqPtr       cdna;
  SeqMgrFeatContext fcontext;
  GatherContextPtr gcp = NULL;
  CharPtr         loc = NULL;
  SeqFeatPtr      mrna;
  Uint2           olditemtype = 0;
  Uint2           olditemid = 0;
  SeqEntryPtr     sep;
  SeqIdPtr        sip;

  if (bssp->_class != BioseqseqSet_class_gen_prod_set)
    return;

  sep = bssp->seq_set;
  if (!IS_Bioseq (sep))
    return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL)
    return;

  gcp = vsp->gcp;
  if (gcp == NULL)
    return;
  olditemid = gcp->itemID;
  olditemtype = gcp->thistype;

  if (vsp->useSeqMgrIndexes) {
    mrna = SeqMgrGetNextFeature (bsp, NULL, 0, FEATDEF_mRNA, &fcontext);
    while (mrna != NULL) {
      cdna = BioseqFindFromSeqLoc (mrna->product);
      if (cdna == NULL) {
        gcp->itemID = mrna->idx.itemID;
        gcp->thistype = OBJ_SEQFEAT;
        loc = SeqLocPrint (mrna->product);
        if (loc == NULL) {
          loc = StringSave ("?");
        }
        sip = SeqLocId (mrna->product);
        /* okay to have far RefSeq product */
        if (sip == NULL || sip->choice != SEQID_OTHER) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_PKG_GenomicProductPackagingProblem, "Product of mRNA feature (%s) not packaged in genomic product set", loc);
        }
        MemFree (loc);
      }
      mrna = SeqMgrGetNextFeature (bsp, mrna, 0, FEATDEF_mRNA, &fcontext);
    }
  }

  gcp->itemID = olditemid;
  gcp->thistype = olditemtype;
}

static void ValidateBioseqSet (GatherContextPtr gcp)
{
  BioseqSetPtr    bssp;
  ValidStructPtr  vsp;
  SeqEntryPtr     sep;

  vsp = (ValidStructPtr) (gcp->userdata);
  bssp = (BioseqSetPtr) (gcp->thisitem);
  vsp->bssp = bssp;
  vsp->bsp = NULL;
  vsp->descr = NULL;
  vsp->sfp = NULL;

  if (vsp->non_ascii_chars) {   /* non_ascii chars in AsnRead step */
    ValidErr (vsp, SEV_ERROR, ERR_GENERIC_NonAsciiAsn, "Non-ascii chars in input ASN.1 strings");
    vsp->non_ascii_chars = FALSE;       /* only do once */
  }

  vsp->nuccnt = 0;
  vsp->segcnt = 0;
  vsp->protcnt = 0;

  sep = gcp->sep;

  SeqEntryExplore (sep, (Pointer) vsp, ValidateSetContents);

  switch (bssp->_class) {
  case 1:                      /* nuc-prot */
    if (vsp->nuccnt == 0)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_PKG_NucProtProblem, "No nucleotides in nuc-prot set");
    if (vsp->protcnt == 0)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_PKG_NucProtProblem, "No proteins in nuc-prot set");
    ValidateNucProtSet (bssp, vsp);
    break;
  case 2:                      /* seg set */
    if (vsp->segcnt == 0)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_PKG_SegSetProblem, "No segmented Bioseq in segset");
    ValidateSegmentedSet (bssp, vsp);
    break;
  case 4:                      /* seg set */
    ValidatePartsSet (bssp, vsp);
    break;
  case BioseqseqSet_class_pop_set:     /* population set */
    ValidatePopSet (bssp, vsp);
    break;
  case BioseqseqSet_class_gen_prod_set:        /* genomic product set */
    ValidateGenProdSet (bssp, vsp);
    break;
  case BioseqseqSet_class_other:
    ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_GenomicProductPackagingProblem, "Genomic product set class incorrectly set to other");
    break;
  default:
    if (!((vsp->nuccnt) || (vsp->protcnt)))
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_PKG_EmptySet, "No Bioseqs in this set");
    break;
  }
  return;
}

static void LookForGEDseqID (BioseqPtr bsp, Pointer userdata)
{
  BoolPtr         isGEDPtr;
  SeqIdPtr        sip;

  isGEDPtr = (BoolPtr) userdata;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
    case SEQID_GENBANK:
    case SEQID_EMBL:
    case SEQID_DDBJ:
    case SEQID_TPG:
    case SEQID_TPE:
    case SEQID_TPD:
      *isGEDPtr = TRUE;
      return;
    default:
      break;
    }
  }
}

static Boolean SuppressTrailingXMessage (BioseqPtr bsp)
{
  ByteStorePtr    bs;
  SeqFeatPtr      cds;
  Boolean         hasstar;
  Int4            len;
  MolInfoPtr      mip;
  SeqDescrPtr     sdp;
  CharPtr         str;

  cds = SeqMgrGetCDSgivenProduct (bsp, NULL);
  if (cds != NULL) {
    bs = ProteinFromCdRegionEx (cds, TRUE, FALSE);
    if (bs != NULL) {
      str = BSMerge (bs, NULL);
      BSFree (bs);
      hasstar = FALSE;
      if (str != NULL) {
        len = StringLen (str);
        if (len > 1 && str[len - 1] == '*') {
          hasstar = TRUE;
        }
      }
      MemFree (str);
      return hasstar;
    }
  }
  sdp = BioseqGetSeqDescr (bsp, Seq_descr_molinfo, NULL);
  if (sdp != NULL) {
    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      if (mip->completeness == 4 || mip->completeness == 5)
        return TRUE;
    }
  }
  return FALSE;
}

static void LookForSecondaryConflict (ValidStructPtr vsp, GatherContextPtr gcp, CharPtr accn, ValNodePtr extra_acc)
{
  CharPtr         str;
  ValNodePtr      vnp;

  if (vsp == NULL || gcp == NULL)
    return;
  if (StringHasNoText (accn))
    return;
  for (vnp = extra_acc; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringHasNoText (str))
      continue;
    if (StringICmp (accn, str) == 0) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadSecondaryAccn, "%s used for both primary and secondary accession", accn);
    }
  }
}

static void CheckSegBspAgainstParts (ValidStructPtr vsp, GatherContextPtr gcp, BioseqPtr bsp)
{
  BioseqSetPtr    bssp;
  BioseqPtr       part;
  SeqEntryPtr     sep;
  SeqIdPtr        sip;
  SeqLocPtr       slp;

  if (vsp == NULL || gcp == NULL || bsp == NULL)
    return;
  if (!vsp->useSeqMgrIndexes)
    return;

  if (bsp->repr != Seq_repr_seg || bsp->seq_ext_type != 1 || bsp->seq_ext == NULL)
    return;

  sep = bsp->seqentry;
  if (sep == NULL)
    return;
  sep = sep->next;
  if (sep == NULL)
    return;
  if (!IS_Bioseq_set (sep))
    return;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL)
    return;
  if (bssp->_class != BioseqseqSet_class_parts)
    return;

  sep = bssp->seq_set;
  for (slp = (ValNodePtr) bsp->seq_ext; slp != NULL; slp = slp->next) {
    if (slp->choice == SEQLOC_NULL)
      continue;
    if (sep == NULL) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_PartsOutOfOrder, "Parts set does not contain enough Bioseqs");
      return;
    }
    if (IS_Bioseq (sep)) {
      part = (BioseqPtr) sep->data.ptrvalue;
      sip = SeqLocId (slp);
      if (sip != NULL && part != NULL) {
        if (!SeqIdIn (sip, part->id)) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_PartsOutOfOrder, "Segmented bioseq seq_ext does not correspond to parts packaging order");
          return;
        }
      }
    } else {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_PartsOutOfOrder, "Parts set component is not Bioseq");
      return;
    }
    sep = sep->next;
  }
  if (sep != NULL) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_PartsOutOfOrder, "Parts set contains too many Bioseqs");
  }
}

/*****************************************************************************
*
*   ValidateBioseqInst(gcp)
*      Validate one Bioseq Seq-inst
*
*****************************************************************************/
static void ValidateBioseqInst (GatherContextPtr gcp)
{
  Boolean         retval = TRUE;
  Int2            i, start_at, num;
  Boolean         errors[4], check_alphabet;
  static char    *repr[8] = {
    "virtual", "raw", "segmented", "constructed",
    "reference", "consensus", "map", "delta"
  };
  SeqPortPtr      spp;
  Int2            residue, x, termination;
  Int4            len, divisor = 1, len2;
  ValNode         head;
  ValNodePtr      vnp, vnp2, idlist;
  BioseqContextPtr bcp;
  Boolean         got_partial, is_invalid;
  int             seqtype, terminations;
  ValidStructPtr  vsp;
  BioseqPtr       bsp, bsp2;
  SeqIdPtr        sip1, sip2;
  Char            buf1[41], buf2[41];
  SeqLitPtr       slitp;
  SeqCodeTablePtr sctp;
  MolInfoPtr      mip;
  Boolean         litHasData;
  SeqMgrDescContext context;
  SeqFeatPtr      cds;
  GeneRefPtr      grp;
  SeqFeatPtr      gene;
  SeqMgrFeatContext genectxt;
  CharPtr         genelbl;
  SeqFeatPtr      prot;
  SeqMgrFeatContext protctxt;
  CharPtr         protlbl;
  TextSeqIdPtr    tsip;
  CharPtr         ptr, last, str, title, buf;
  Uint1           lastchoice;
  Char            ch;
  Boolean         multitoken;
  Boolean         isGenBankEMBLorDDBJ;
  Boolean         isPatent = FALSE;
  Boolean         isPDB = FALSE;
  Boolean         isNT = FALSE;
  Int2            trailingX = 0;
  Int2            numletters, numdigits;
  Boolean         letterAfterDigit, badIDchars;
  GBBlockPtr      gbp;
  EMBLBlockPtr    ebp;
  SeqDescrPtr     sdp;
  SeqMgrDescContext dcontext;
  size_t          buflen = 1001;
  ItemInfo        ii;
  Uint1           tech;
  Uint2           olditemtype = 0;
  Uint2           olditemid = 0;
  ObjValNodePtr   ovp;
  BioseqSetPtr    bssp;

  /* set up data structures */

  vsp = (ValidStructPtr) (gcp->userdata);
  bsp = (BioseqPtr) (gcp->thisitem);
  vsp->bsp = bsp;
  vsp->descr = NULL;
  vsp->sfp = NULL;
  vsp->bssp = (BioseqSetPtr) (gcp->parentitem);
  vsp->bsp_partial_val = 0;

  if (vsp->non_ascii_chars) {   /* non_ascii chars in AsnRead step */
    ValidErr (vsp, SEV_REJECT, ERR_GENERIC_NonAsciiAsn, "Non-ascii chars in input ASN.1 strings");
    vsp->non_ascii_chars = FALSE;       /* only do once */
  }

  if (bsp->id == NULL) {
    ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_NoIdOnBioseq, "No ids on a Bioseq");
    return;
  }

  for (sip1 = bsp->id; sip1 != NULL; sip1 = sip1->next) {
    if (sip1->choice == SEQID_OTHER) {
      tsip = (TextSeqIdPtr) sip1->data.ptrvalue;
      if (tsip != NULL && tsip->accession != NULL) {
        if (StringNICmp (tsip->accession, "NT_", 3) == 0) {
          isNT = TRUE;
        }
      }
    }

    for (sip2 = sip1->next; sip2 != NULL; sip2 = sip2->next) {
      if (SeqIdComp (sip1, sip2) != SIC_DIFF) {
        SeqIdWrite (sip1, buf1, PRINTID_FASTA_SHORT, 40);
        SeqIdWrite (sip2, buf2, PRINTID_FASTA_SHORT, 40);
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_ConflictingIdsOnBioseq, "Conflicting ids on a Bioseq: (%s - %s)", buf1, buf2);
      }
    }
  }

  for (sip1 = bsp->id; sip1 != NULL; sip1 = sip1->next) {
    switch (sip1->choice) {
    case SEQID_GENBANK:
    case SEQID_EMBL:
    case SEQID_DDBJ:
    case SEQID_TPG:
    case SEQID_TPE:
    case SEQID_TPD:
      tsip = (TextSeqIdPtr) sip1->data.ptrvalue;
      if (tsip != NULL && tsip->accession != NULL) {
        numletters = 0;
        numdigits = 0;
        letterAfterDigit = FALSE;
        badIDchars = FALSE;
        for (ptr = tsip->accession, ch = *ptr; ch != '\0'; ptr++, ch = *ptr) {
          if (IS_UPPER (ch)) {
            numletters++;
            if (numdigits > 0) {
              letterAfterDigit = TRUE;
            }
          } else if (IS_DIGIT (ch)) {
            numdigits++;
          } else {
            badIDchars = TRUE;
          }
        }
        if (letterAfterDigit || badIDchars) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadSeqIdFormat, "Bad accession", tsip->accession);
        } else if (numletters == 1 && numdigits == 5 && ISA_na (bsp->mol)) {
        } else if (numletters == 2 && numdigits == 6 && ISA_na (bsp->mol)) {
        } else if (numletters == 3 && numdigits == 5 && ISA_aa (bsp->mol)) {
        } else if (numletters == 2 && numdigits == 6 && ISA_aa (bsp->mol) && bsp->repr == Seq_repr_seg) {
        } else {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadSeqIdFormat, "Bad accession", tsip->accession);
        }
        if (vsp->useSeqMgrIndexes) {
          vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &context);
          if (vnp != NULL) {
            gbp = (GBBlockPtr) vnp->data.ptrvalue;
            if (gbp != NULL) {
              LookForSecondaryConflict (vsp, gcp, tsip->accession, gbp->extra_accessions);
            }
          }
          vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_embl, &context);
          if (vnp != NULL) {
            ebp = (EMBLBlockPtr) vnp->data.ptrvalue;
            if (ebp != NULL) {
              LookForSecondaryConflict (vsp, gcp, tsip->accession, ebp->extra_acc);
            }
          }
        }
      }
      /* and keep going with further test */
    case SEQID_OTHER:
      tsip = (TextSeqIdPtr) sip1->data.ptrvalue;
      if (tsip != NULL && tsip->name != NULL) {
        multitoken = FALSE;
        for (ptr = tsip->name, ch = *ptr; ch != '\0'; ptr++, ch = *ptr) {
          if (IS_WHITESP (ch)) {
            multitoken = TRUE;
          }
        }
        if (multitoken) {
          ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_SeqIdNameHasSpace, "Seq-id.name '%s' should be a single word without any spaces", tsip->name);
        }
      }
      break;
    case SEQID_PATENT:
      isPatent = TRUE;
      break;
    case SEQID_PDB:
      isPDB = TRUE;
      break;
    case SEQID_GI:
      if (sip1->data.intvalue == 0) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_ZeroGiNumber, "Invalid GI number");
      }
    default:
      break;
    }
  }

  for (sip1 = bsp->id; sip1 != NULL; sip1 = sip1->next) {
    bsp2 = BioseqFindCore (sip1);
    if (bsp2 == NULL) {
      if (!isPatent) {
        SeqIdWrite (sip1, buf1, PRINTID_FASTA_SHORT, 40);
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_IdOnMultipleBioseqs, "BioseqFind (%s) unable to find itself - possible internal error", buf1);
      }
    } else if (bsp2 != bsp) {
      SeqIdWrite (sip1, buf1, PRINTID_FASTA_SHORT, 40);
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_IdOnMultipleBioseqs, "SeqID %s is present on multiple Bioseqs in record", buf1);
    }
  }

  for (i = 0; i < 4; i++)
    errors[i] = FALSE;

  switch (bsp->repr) {
  case Seq_repr_virtual:
    if ((bsp->seq_ext_type) || (bsp->seq_ext != NULL))
      errors[0] = TRUE;
    if ((bsp->seq_data_type) || (bsp->seq_data != NULL))
      errors[3] = TRUE;
    break;
  case Seq_repr_map:
    if ((bsp->seq_ext_type != 3) || (bsp->seq_ext == NULL))
      errors[1] = TRUE;
    if ((bsp->seq_data_type) || (bsp->seq_data != NULL))
      errors[3] = TRUE;
    break;
  case Seq_repr_ref:
    if ((bsp->seq_ext_type != 2) || (bsp->seq_ext == NULL))
      errors[1] = TRUE;
    if ((bsp->seq_data_type) || (bsp->seq_data != NULL))
      errors[3] = TRUE;
    break;
  case Seq_repr_seg:
    if ((bsp->seq_ext_type != 1) || (bsp->seq_ext == NULL))
      errors[1] = TRUE;
    if ((bsp->seq_data_type) || (bsp->seq_data != NULL))
      errors[3] = TRUE;
    break;
  case Seq_repr_raw:
  case Seq_repr_const:
    if ((bsp->seq_ext_type) || (bsp->seq_ext != NULL))
      errors[0] = TRUE;
    if ((bsp->seq_data_type < 1) || (bsp->seq_data_type > 11)
        || (bsp->seq_data == NULL))
      errors[2] = TRUE;
    break;
  case Seq_repr_delta:
    if ((bsp->seq_ext_type != 4) || (bsp->seq_ext == NULL))
      errors[1] = TRUE;
    if ((bsp->seq_data_type) || (bsp->seq_data != NULL))
      errors[3] = TRUE;
    break;
  default:
    ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_ReprInvalid, "Invalid Bioseq->repr = %d", (int) (bsp->repr));
    return;
  }

  if (errors[0] == TRUE) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_ExtNotAllowed, "Bioseq-ext not allowed on %s Bioseq", repr[bsp->repr - 1]);
    retval = FALSE;
  }

  if (errors[1] == TRUE) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_ExtBadOrMissing, "Missing or incorrect Bioseq-ext on %s Bioseq", repr[bsp->repr - 1]);
    retval = FALSE;
  }

  if (errors[2] == TRUE) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_SeqDataNotFound, "Missing Seq-data on %s Bioseq", repr[bsp->repr - 1]);
    retval = FALSE;
  }

  if (errors[3] == TRUE) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_SeqDataNotAllowed, "Seq-data not allowed on %s Bioseq", repr[bsp->repr - 1]);
    retval = FALSE;
  }

  if (!retval)
    return;

  if (ISA_aa (bsp->mol)) {
    if (bsp->topology > 1) {    /* not linear */
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_CircularProtein, "Non-linear topology set on protein");
    }
    if (bsp->strand > 1) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_DSProtein, "Protein not single stranded");
    }

  } else {
    if (!bsp->mol)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_MolNotSet, "Bioseq.mol is 0");
    else if (bsp->mol == Seq_mol_other)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_MolOther, "Bioseq.mol is type other");
    else if (bsp->mol == Seq_mol_na)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_MolNuclAcid, "Bioseq.mol is type na");
  }
  /* check sequence alphabet */
  if ((bsp->repr == Seq_repr_raw) || (bsp->repr == Seq_repr_const)) {
    if (bsp->fuzz != NULL) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_FuzzyLen, "Fuzzy length on %s Bioseq", repr[bsp->repr - 1]);
    }

    if (bsp->length < 1) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_InvalidLen, "Invalid Bioseq length [%ld]", (long) bsp->length);
    }

    seqtype = (int) (bsp->seq_data_type);
    switch (seqtype) {
    case Seq_code_iupacna:
    case Seq_code_ncbi2na:
    case Seq_code_ncbi4na:
    case Seq_code_ncbi8na:
    case Seq_code_ncbipna:
      if (ISA_aa (bsp->mol)) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidAlphabet, "Using a nucleic acid alphabet on a protein sequence");
        return;
      }
      break;
    case Seq_code_iupacaa:
    case Seq_code_ncbi8aa:
    case Seq_code_ncbieaa:
    case Seq_code_ncbipaa:
    case Seq_code_ncbistdaa:
      if (ISA_na (bsp->mol)) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidAlphabet, "Using a protein alphabet on a nucleic acid");
        return;
      }
      break;
    default:
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidAlphabet, "Using illegal sequence alphabet [%d]", (int) bsp->seq_data_type);
      return;
    }

    check_alphabet = FALSE;
    switch (seqtype) {
    case Seq_code_iupacaa:
    case Seq_code_iupacna:
    case Seq_code_ncbieaa:
    case Seq_code_ncbistdaa:
      check_alphabet = TRUE;

    case Seq_code_ncbi8na:
    case Seq_code_ncbi8aa:
      divisor = 1;
      break;

    case Seq_code_ncbi4na:
      divisor = 2;
      break;

    case Seq_code_ncbi2na:
      divisor = 4;
      break;

    case Seq_code_ncbipna:
      divisor = 5;
      break;

    case Seq_code_ncbipaa:
      divisor = 21;
      break;
    }

    len = bsp->length;
    if (len % divisor)
      len += divisor;
    len /= divisor;
    len2 = BSLen (bsp->seq_data);
    if (len > len2) {
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_SeqDataLenWrong, "Bioseq.seq_data too short [%ld] for given length [%ld]", (long) (len2 * divisor),
                (long) bsp->length);
      return;
    } else if (len < len2) {
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_SeqDataLenWrong, "Bioseq.seq_data is larger [%ld] than given length [%ld]", (long) (len2 * divisor),
                (long) bsp->length);
    }

    if (check_alphabet) {       /* check 1 letter alphabets */
      switch (seqtype) {
      case Seq_code_iupacaa:
      case Seq_code_ncbieaa:
        termination = '*';
        break;
      case Seq_code_ncbistdaa:
        termination = 25;
        break;
      default:
        termination = '\0';
        break;
      }
      spp = SeqPortNew (bsp, 0, -1, 0, 0);
      if (spp == NULL) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_SeqPortFail, "Can't open SeqPort");
        return;
      }

      i = 0;
      terminations = 0;
      trailingX = 0;
      for (len = 0; len < bsp->length; len++) {
        residue = SeqPortGetResidue (spp);
        if (!IS_residue (residue)) {
          i++;
          if (i > 10) {
            ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidResidue, "More than 10 invalid residues. Checking stopped");
            SeqPortFree (spp);
            if (vsp->patch_seq)
              PatchBadSequence (bsp);
            return;
          } else {
            BSSeek (bsp->seq_data, len, SEEK_SET);
            x = BSGetByte (bsp->seq_data);
            if (bsp->seq_data_type == Seq_code_ncbistdaa)
              ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidResidue, "Invalid residue [%d] in position [%ld]", (int) x, (long) (len + 1));
            else
              ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidResidue, "Invalid residue [%c] in position [%ld]", (char) x, (long) (len + 1));
          }
        } else if (residue == termination) {
          terminations++;
          trailingX = 0;        /* suppress if followed by terminator */
        } else if (residue == 'X') {
          trailingX++;
        } else {
          trailingX = 0;
        }
      }
      SeqPortFree (spp);
      if (trailingX > 0 && SuppressTrailingXMessage (bsp)) {
        /* suppress if cds translation ends in '*' or 3' partial */
      } else if (trailingX > 1) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_TrailingX, "Sequence ends in %d trailing Xs", (int) trailingX);
      } else if (trailingX > 0) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_TrailingX, "Sequence ends in %d trailing X", (int) trailingX);
      }
      if (terminations) {
        cds = SeqMgrGetCDSgivenProduct (bsp, NULL);
        grp = SeqMgrGetGeneXref (cds);
        genelbl = NULL;
        if (grp == NULL && cds != NULL) {
          gene = SeqMgrGetOverlappingGene (cds->location, &genectxt);
          if (gene != NULL) {
            grp = (GeneRefPtr) gene->data.value.ptrvalue;
          }
        }
        if (grp != NULL && (!SeqMgrGeneIsSuppressed (grp))) {
          if (grp->locus != NULL)
            genelbl = (grp->locus);
          else if (grp->desc != NULL)
            genelbl = (grp->desc);
          else if (grp->syn != NULL)
            genelbl = (CharPtr) (grp->syn->data.ptrvalue);
        }
        prot = SeqMgrGetBestProteinFeature (bsp, &protctxt);
        protlbl = protctxt.label;
        if (StringHasNoText (genelbl)) {
          genelbl = "gene?";
        }
        if (StringHasNoText (protlbl)) {
          protlbl = "prot?";
        }
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_StopInProtein, "[%d] termination symbols in protein sequence (%s - %s)", terminations, genelbl, protlbl);
        if (!i)
          return;
      }
      if (i) {
        if (vsp->patch_seq)
          PatchBadSequence (bsp);
        return;
      }

    }
  }

  if ((bsp->repr == Seq_repr_seg) || (bsp->repr == Seq_repr_ref)) {     /* check segmented sequence */
    head.choice = SEQLOC_MIX;
    head.data.ptrvalue = bsp->seq_ext;
    head.next = NULL;
    ValidateSeqLoc (vsp, (SeqLocPtr) & head, "Segmented Bioseq");
    /* check the length */
    len = 0;
    vnp = NULL;
    while ((vnp = SeqLocFindNext (&head, vnp)) != NULL) {
      len2 = SeqLocLen (vnp);
      if (len2 > 0)
        len += len2;
    }
    if (bsp->length > len) {
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_SeqDataLenWrong, "Bioseq.seq_data too short [%ld] for given length [%ld]", (long) (len), (long) bsp->length);
    } else if (bsp->length < len) {
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_SeqDataLenWrong, "Bioseq.seq_data is larger [%ld] than given length [%ld]", (long) (len), (long) bsp->length);
    }

    vnp = NULL;
    idlist = NULL;
    while ((vnp = SeqLocFindNext (&head, vnp)) != NULL) {
      sip1 = SeqLocId (vnp);
      if (sip1 != NULL) {
        SeqIdWrite (sip1, buf1, PRINTID_FASTA_SHORT, 40);
        ValNodeCopyStr (&idlist, vnp->choice, buf1);
      }
    }
    if (idlist != NULL) {
      idlist = ValNodeSort (idlist, SortVnpByString);
      last = (CharPtr) idlist->data.ptrvalue;
      lastchoice = (Uint1) idlist->choice;
      vnp = idlist->next;
      while (vnp != NULL) {
        str = (CharPtr) vnp->data.ptrvalue;
        if (StringICmp (last, str) == 0) {
          if (vnp->choice == lastchoice && lastchoice == SEQLOC_WHOLE) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_DuplicateSegmentReferences, "Segmented sequence has multiple references to %s\n", str);
          } else {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_DuplicateSegmentReferences,
                      "Segmented sequence has multiple references to %s that are not SEQLOC_WHOLE\n", str);
          }
        } else {
          last = (CharPtr) vnp->data.ptrvalue;
          lastchoice = (Uint1) vnp->choice;
        }
        vnp = vnp->next;
      }
      ValNodeFreeData (idlist);
    }

    vsp->bsp_partial_val = SeqLocPartialCheck ((SeqLocPtr) (&head));
    if ((vsp->bsp_partial_val) && (ISA_aa (bsp->mol))) {
      bcp = NULL;
      vnp = NULL;
      got_partial = FALSE;
      if (vsp->useSeqMgrIndexes) {
        vnp = SeqMgrGetNextDescriptor (bsp, vnp, Seq_descr_modif, &context);
      } else {
        bcp = BioseqContextNew (bsp);
        vnp = BioseqContextGetSeqDescr (bcp, Seq_descr_modif, vnp, NULL);
      }
      while (vnp != NULL) {
        for (vnp2 = (ValNodePtr) (vnp->data.ptrvalue); vnp2 != NULL; vnp2 = vnp2->next) {
          switch (vnp2->data.intvalue) {
          case 10:             /* partial */
            got_partial = TRUE;
            break;
          case 16:             /* no-left */
            if (!(vsp->bsp_partial_val & SLP_START))
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_PartialInconsistent, "GIBB-mod no-left inconsistent with segmented SeqLoc");
            got_partial = TRUE;
            break;
          case 17:             /* no-right */
            if (!(vsp->bsp_partial_val & SLP_STOP))
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_PartialInconsistent, "GIBB-mod no-right inconsistent with segmented SeqLoc");
            got_partial = TRUE;
            break;

          }
        }
        if (vsp->useSeqMgrIndexes) {
          vnp = SeqMgrGetNextDescriptor (bsp, vnp, Seq_descr_modif, &context);
        } else {
          vnp = BioseqContextGetSeqDescr (bcp, Seq_descr_modif, vnp, NULL);
        }
      }
      if (!vsp->useSeqMgrIndexes) {
        BioseqContextFree (bcp);
      }
      if (!got_partial)
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_PartialInconsistent, "Partial segmented sequence without GIBB-mod");
    }
  }

  mip = NULL;

  if (bsp->repr == Seq_repr_delta) {
    len = 0;
    for (vnp = (ValNodePtr) (bsp->seq_ext); vnp != NULL; vnp = vnp->next) {
      if (vnp->data.ptrvalue == NULL)
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_SeqDataLenWrong, "NULL pointer in delta seq_ext valnode");
      else {
        switch (vnp->choice) {
        case 1:                /* SeqLocPtr */
          len2 = SeqLocLen ((SeqLocPtr) (vnp->data.ptrvalue));
          if (len2 < 0)
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_SeqDataLenWrong, "-1 length on seq-loc of delta seq_ext");
          else
            len += len2;
          break;
        case 2:                /* SeqLitPtr */
          slitp = (SeqLitPtr) (vnp->data.ptrvalue);
          if (slitp->seq_data != NULL) {
            sctp = SeqCodeTableFind (slitp->seq_data_type);
            if (sctp == NULL) {
              ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidAlphabet, "Using illegal sequence alphabet [%d] in SeqLitPtr", (int) slitp->seq_data_type);
              len += slitp->length;
              break;
            }

            start_at = (Int2) (sctp->start_at);
            num = (Int2) (sctp->num);

            switch (slitp->seq_data_type) {
            case Seq_code_iupacaa:
            case Seq_code_iupacna:
            case Seq_code_ncbieaa:
            case Seq_code_ncbistdaa:
              BSSeek (slitp->seq_data, 0, SEEK_SET);
              for (len2 = 1; len2 <= (slitp->length); len2++) {
                is_invalid = FALSE;
                residue = BSGetByte (slitp->seq_data);
                i = residue - start_at;
                if ((i < 0) || (i >= num))
                  is_invalid = TRUE;
                else if (*(sctp->names[i]) == '\0')
                  is_invalid = TRUE;
                if (is_invalid) {
                  if (slitp->seq_data_type == Seq_code_ncbistdaa)
                    ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidResidue, "Invalid residue [%d] in position [%ld]", (int) residue, (long) (len + len2));
                  else
                    ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidResidue, "Invalid residue [%c] in position [%ld]", (char) residue, (long) (len + len2));
                }
              }
              break;
            default:
              break;
            }
          }
          len += slitp->length;
          break;
        default:
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_ExtNotAllowed, "Illegal choice [%d] in delta chain", (int) (vnp->choice));
          break;
        }
      }
    }
    if (bsp->length > len) {
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_SeqDataLenWrong, "Bioseq.seq_data too short [%ld] for given length [%ld]", (long) (len), (long) bsp->length);
    } else if (bsp->length < len) {
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_SeqDataLenWrong, "Bioseq.seq_data is larger [%ld] than given length [%ld]", (long) (len), (long) bsp->length);
    }
    vnp = NULL;
    if (vsp->useSeqMgrIndexes) {
      vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &context);
    } else {
      bcp = BioseqContextNew (bsp);
      vnp = BioseqContextGetSeqDescr (bcp, Seq_descr_molinfo, NULL, NULL);
      BioseqContextFree (bcp);
    }
    if (vnp != NULL) {
      mip = (MolInfoPtr) vnp->data.ptrvalue;
      if (mip != NULL) {
        if ((!isNT) && mip->tech != MI_TECH_htgs_0 && mip->tech != MI_TECH_htgs_1 && mip->tech != MI_TECH_htgs_2 && mip->tech != MI_TECH_htgs_3) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadDeltaSeq, "Delta seq technique should not be [%d]", (int) (mip->tech));
        }
      }
    }
  }

  if (ISA_aa (bsp->mol)) {
    if ((bsp->length <= 3) && (bsp->length >= 0) && (!isPDB)) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_ShortSeq, "Sequence only %ld residues", (long) (bsp->length));
    }

  } else {
    if ((bsp->length <= 10) && (bsp->length >= 0) && (!isPDB)) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_ShortSeq, "Sequence only %ld residues", (long) (bsp->length));
    }
  }

  if (bsp->length > 350000) {
    if (bsp->repr == Seq_repr_delta) {
      isGenBankEMBLorDDBJ = FALSE;
      /* suppress this for data from genome annotation project */
      VisitBioseqsInSep (vsp->sep, (Pointer) &isGenBankEMBLorDDBJ, LookForGEDseqID);
      if (mip != NULL && isGenBankEMBLorDDBJ) {
        if (mip->tech == MI_TECH_htgs_0 || mip->tech == MI_TECH_htgs_1 || mip->tech == MI_TECH_htgs_2) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_LongHtgsSequence, "Phase 0, 1 or 2 HTGS sequence exceeds 350kbp limit");
        } else if (mip->tech == MI_TECH_htgs_3) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_SequenceExceeds350kbp, "Phase 3 HTGS sequence exceeds 350kbp limit");
        } else {
          len = 0;
          litHasData = FALSE;
          for (vnp = (ValNodePtr) (bsp->seq_ext); vnp != NULL; vnp = vnp->next) {
            if (vnp->choice == 2) {
              slitp = (SeqLitPtr) (vnp->data.ptrvalue);
              if (slitp != NULL) {
                if (slitp->seq_data != NULL) {
                  litHasData = TRUE;
                }
                len += slitp->length;
              }
            }
          }
          if (len > 350000 && litHasData) {
            ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_LongLiteralSequence, "Length of sequence literals exceeds 350kbp limit");
          }
        }
      }
    } else if (bsp->repr == Seq_repr_raw) {
      vnp = NULL;
      if (vsp->useSeqMgrIndexes) {
        vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &context);
      } else {
        bcp = BioseqContextNew (bsp);
        vnp = BioseqContextGetSeqDescr (bcp, Seq_descr_molinfo, NULL, NULL);
        BioseqContextFree (bcp);
      }
      if (vnp != NULL) {
        mip = (MolInfoPtr) vnp->data.ptrvalue;
      }
      if (mip != NULL) {
        if (mip->tech == MI_TECH_htgs_0 || mip->tech == MI_TECH_htgs_1 || mip->tech == MI_TECH_htgs_2) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_LongHtgsSequence, "Phase 0, 1 or 2 HTGS sequence exceeds 350kbp limit");
        } else if (mip->tech == MI_TECH_htgs_3) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_SequenceExceeds350kbp, "Phase 3 HTGS sequence exceeds 350kbp limit");
        } else {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_SequenceExceeds350kbp, "Length of sequence exceeds 350kbp limit");
        }
      } else {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_SequenceExceeds350kbp, "Length of sequence exceeds 350kbp limit");
      }
    } else {
      /* Could be a segset header bioseq that is > 350kbp */
      /* No-op for now? Or generate a warning? */
    }
  }

  if (bsp->repr == Seq_repr_seg) {
    CheckSegBspAgainstParts (vsp, gcp, bsp);
  }

  if (ISA_aa (bsp->mol) && vsp->useSeqMgrIndexes) {
    vnp = BioseqGetSeqDescr (bsp, Seq_descr_title, NULL);
    if (vnp != NULL) {
      if (bsp->idx.parenttype == OBJ_BIOSEQSET) {
        bssp = (BioseqSetPtr) bsp->idx.parentptr;
        while (bssp != NULL && bssp->_class != BioseqseqSet_class_nuc_prot) {
          if (bssp->idx.parenttype == OBJ_BIOSEQSET) {
            bssp = (BioseqSetPtr) bssp->idx.parentptr;
          } else {
            bssp = NULL;
          }
        }
        if (bssp != NULL && bssp->_class == BioseqseqSet_class_nuc_prot) {
          title = (CharPtr) vnp->data.ptrvalue;
          tech = 0;
          sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
          if (sdp != NULL) {
            mip = (MolInfoPtr) sdp->data.ptrvalue;
            if (mip != NULL) {
              tech = mip->tech;
            }
          }
          buf = MemNew (sizeof (Char) * (buflen + 1));
          MemSet ((Pointer) (&ii), 0, sizeof (ItemInfo));
          if (buf != NULL && CreateDefLineEx (&ii, bsp, buf, buflen, tech, NULL, NULL, TRUE)) {
            if (StringICmp (buf, title) != 0) {
              olditemid = gcp->itemID;
              olditemtype = gcp->thistype;
              if (vnp->extended != 0) {
                ovp = (ObjValNodePtr) vnp;
                gcp->itemID = ovp->idx.itemID;
                gcp->thistype = OBJ_SEQDESC;
              }
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_InconsistentProteinTitle, "Instantiated protein title does not match automatically generated title");
              gcp->itemID = olditemid;
              gcp->thistype = olditemtype;
            }
          }
          MemFree (buf);
        }
      }
    }
  }

  return;
}

/*****************************************************************************
*
*   ValidatePubdesc(gcp)
*      Check pubdesc for missing information
*
*****************************************************************************/
static Boolean HasNoText (CharPtr str)
{
  Char            ch;

  if (str != NULL) {
    ch = *str;
    while (ch != '\0') {
      if (ch > ' ') {
        return FALSE;
      }
      str++;
      ch = *str;
    }
  }
  return TRUE;
}

static Boolean HasNoName (ValNodePtr name)
{
  AuthorPtr       ap;
  NameStdPtr      nsp;
  PersonIdPtr     pid;

  if (name != NULL) {
    ap = name->data.ptrvalue;
    if (ap != NULL) {
      pid = ap->name;
      if (pid != NULL) {
        if (pid->choice == 2) {
          nsp = pid->data;
          if (nsp != NULL) {
            if (!HasNoText (nsp->names[0])) {
              return FALSE;
            }
          }
        }
      }
    }
  }
  return TRUE;
}

static void ValidatePubdesc (ValidStructPtr vsp, GatherContextPtr gcp, PubdescPtr pdp)
{
  AuthListPtr     alp;
  CitArtPtr       cap;
  CitGenPtr       cgp;
  CitJourPtr      cjp;
  Boolean         hasName, hasTitle;
  ImprintPtr      imp;
  Boolean         noVol, noPages;
  ValNodePtr      name;
  ValNodePtr      title;
  ValNodePtr      vnp;

  if (vsp == NULL || pdp == NULL)
    return;
  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
    case PUB_Gen:
      cgp = (CitGenPtr) vnp->data.ptrvalue;
      if (cgp != NULL) {
        if (!StringHasNoText (cgp->cit)) {
          if (StringNICmp (cgp->cit, "submitted", 8) == 0 || StringNICmp (cgp->cit, "unpublished", 11) == 0     /* ||
                                                                                                                   StringNICmp (cgp->cit, "in press", 8) == 0 ||
                                                                                                                   StringNICmp (cgp->cit, "to be published", 15) == 0 */ ) {
          } else {
            ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MissingPubInfo, "Unpublished citation text invalid");
          }
        }
      }
      break;
    case PUB_Article:
      cap = (CitArtPtr) vnp->data.ptrvalue;
      hasName = FALSE;
      hasTitle = FALSE;
      if (cap != NULL) {
        for (title = cap->title; title != NULL; title = title->next) {
          if (!HasNoText ((CharPtr) title->data.ptrvalue)) {
            hasTitle = TRUE;
          }
        }
        if (!hasTitle) {
          ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MissingPubInfo, "Publication has no title");
        }
        alp = cap->authors;
        if (alp != NULL) {
          if (alp->choice == 1) {
            for (name = alp->names; name != NULL; name = name->next) {
              if (!HasNoName (name)) {
                hasName = TRUE;
              }
            }
          } else if (alp->choice == 2 || alp->choice == 3) {
            for (name = alp->names; name != NULL; name = name->next) {
              if (!HasNoText ((CharPtr) name->data.ptrvalue)) {
                hasName = TRUE;
              }
            }
          }
        }
        if (!hasName) {
          ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MissingPubInfo, "Publication has no author names");
        }
      }

      switch (cap->from) {
      case 1:
        cjp = (CitJourPtr) cap->fromptr;
        if (cjp != NULL) {
          hasTitle = FALSE;
          for (title = cjp->title; title != NULL; title = title->next) {
            if (!HasNoText ((CharPtr) title->data.ptrvalue)) {
              hasTitle = TRUE;
            }
          }
          if (!hasTitle) {
            ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MissingPubInfo, "Journal title missing");
          }
          imp = cjp->imp;
          if (imp != NULL) {
            if (imp->prepub == 0 && imp->pubstatus != PUBSTATUS_aheadofprint) {
              noVol = StringHasNoText (imp->volume);
              noPages = StringHasNoText (imp->pages);
              if (noVol && noPages) {
                ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MissingPubInfo, "Journal volume and pages missing");
              } else if (noVol) {
                ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MissingPubInfo, "Journal volume missing");
              } else if (noPages) {
                ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MissingPubInfo, "Journal pages missing");
              }
            }
          }
        }
        break;
      default:
        break;
      }
      break;
    case PUB_Equiv:
      ValidErr (vsp, SEV_WARNING, ERR_GENERIC_UnnecessaryPubEquiv, "Publication has unexpected internal Pub-equiv");
      break;
    default:
      break;
    }
  }
}

static void ValidateSfpCit (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp)
{
  ValNodePtr      ppr;
  ValNodePtr      psp;

  if (vsp == NULL || sfp == NULL || sfp->cit == NULL)
    return;
  psp = sfp->cit;
  if (psp == NULL)
    return;
  for (ppr = (ValNodePtr) psp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
    if (ppr->choice == PUB_Equiv) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnnecessaryCitPubEquiv, "Citation on feature has unexpected internal Pub-equiv");
      return;
    }
  }
}

typedef struct bioseqvalid
{
  ValidStructPtr  vsp;
  Boolean         is_aa;        /* bioseq is protein? */
  Boolean         is_mrna;      /* molinfo is mrna? */
  Boolean         is_prerna;    /* molinfo is precursor rna? */
  Boolean         got_a_pub;
  int             last_na_mol, last_na_mod, last_organelle, last_partialness, last_left_right, last_biomol, last_tech, last_completeness, num_full_length_src_feat,     /* number full length src feats */
                  num_full_length_prot_ref;
  ValNodePtr      last_gb, last_embl, last_prf, last_pir, last_sp, last_pdb, last_create, last_update, last_biosrc, last_orgref;
  OrgRefPtr       last_org;
  GatherContextPtr gcp;
}
BioseqValidStr , PNTR BioseqValidStrPtr;

/*****************************************************************************
*
*   ValidateSeqFeatContext(gcp)
*      Gather callback helper function for validating context on a Bioseq
*
*****************************************************************************/
static Boolean ValidateSeqFeatCommon (SeqFeatPtr sfp, BioseqValidStrPtr bvsp, ValidStructPtr vsp,
                                      Int4 left, Int4 right, Uint2 featitemid, Boolean farloc, BioseqPtr bsp)
{
  GatherContextPtr gcp = NULL;
  ImpFeatPtr      ifp;
  Uint2           olditemtype = 0;
  Uint2           olditemid = 0;
  RnaRefPtr       rrp;
  CharPtr         str;
  SeqLocPtr       slp;
  SeqIdPtr        sip;
  TextSeqIdPtr    tsip;
  Boolean         on_seg = FALSE;
  Boolean         is_nc = FALSE;
  ErrSev          sev = SEV_ERROR;


  vsp->descr = NULL;
  vsp->sfp = sfp;

  if (featitemid > 0) {
    gcp = vsp->gcp;
    if (gcp != NULL) {
      olditemid = gcp->itemID;
      olditemtype = gcp->thistype;
      gcp->itemID = featitemid;
      gcp->thistype = OBJ_SEQFEAT;
    }
  }

  if (bsp != NULL) {
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_OTHER) {
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL && tsip->accession != NULL) {
          if (StringNICmp (tsip->accession, "NT_", 3) == 0) {
            is_nc = TRUE;
          }
        }
      }
    }
  }

  if (bvsp->is_aa) {
    if (sfp->data.choice == SEQFEAT_PROT) {
      if ((left == 0) && (right == ((vsp->bsp->length) - 1)))
        bvsp->num_full_length_prot_ref++;
    }

    switch (sfp->data.choice) {
    case SEQFEAT_CDREGION:
    case SEQFEAT_RNA:
    case SEQFEAT_RSITE:
    case SEQFEAT_TXINIT:
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidForType, "Invalid feature for a protein Bioseq.");
      break;
    default:
      break;
    }

  } else {
    switch (sfp->data.choice) {
    case SEQFEAT_PROT:
    case SEQFEAT_PSEC_STR:
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidForType, "Invalid feature for a nucleotide Bioseq.");
      break;
    default:
      break;
    }

  }

  if (bvsp->is_mrna) {
    switch (sfp->data.choice) {
    case SEQFEAT_RNA:
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp != NULL && rrp->type == 2) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidForType, "mRNA feature is invalid on an mRNA (cDNA) Bioseq.");
      }
      break;
    case SEQFEAT_IMP:
      ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
      if (ifp != NULL && ifp->key != NULL && (!HasNoText (ifp->key))) {
        if (StringCmp (ifp->key, "intron") == 0 || StringCmp (ifp->key, "CAAT_signal") == 0) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidForType, "Invalid feature for an mRNA Bioseq.");
        }
      }
      break;
    default:
      break;
    }
  } else if (bvsp->is_prerna) {
    switch (sfp->data.choice) {
    case SEQFEAT_IMP:
      ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
      if (ifp != NULL && ifp->key != NULL && (!HasNoText (ifp->key))) {
        if (StringCmp (ifp->key, "CAAT_signal") == 0) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidForType, "Invalid feature for an pre-RNA Bioseq.");
        }
      }
      break;
    default:
      break;
    }
  }

  if (farloc && (! is_nc)) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_FarLocation, "Feature has 'far' location - accession not packaged in record");
  }

  if ((sfp->data.choice == SEQFEAT_PUB) || (sfp->cit != NULL))
    bvsp->got_a_pub = TRUE;

  str = (CharPtr) sfp->comment;
  if (SerialNumberInString (str)) {
    ValidErr (vsp, SEV_INFO, ERR_SEQ_FEAT_SerialInComment,
              "Feature comment may refer to reference by serial number - attach reference specific comments to the reference REMARK instead.");
  }

  if (bsp != NULL && bsp->repr == Seq_repr_seg) {
    slp = SeqLocFindNext (sfp->location, NULL);
    while (slp != NULL) {
      sip = SeqLocId (slp);
      if (sip != NULL) {
        if (SeqIdIn (sip, bsp->id)) {
          on_seg = TRUE;
        }
      }
      slp = SeqLocFindNext (sfp->location, slp);
    }
    if (on_seg) {
      if (is_nc) {
        sev = SEV_WARNING;
      }
      ValidErr (vsp, sev, ERR_SEQ_FEAT_LocOnSegmentedBioseq, "Feature location on segmented bioseq, not on parts");
    }
  }

  if (gcp != NULL) {
    gcp->itemID = olditemid;
    gcp->thistype = olditemtype;
  }

  return TRUE;
}

static void CheckMultiIntervalGene (SeqFeatPtr sfp, SeqMgrFeatContextPtr context, ValidStructPtr vsp, GatherContextPtr gcp)

{
  Uint2           olditemtype = 0;
  Uint2           olditemid = 0;

  if (sfp == NULL || context == NULL || vsp == NULL) return;
  if (SeqLocId (sfp->location) == NULL) return;
  if (context->numivals < 2) return;

  if (gcp != NULL) {
    olditemid = gcp->itemID;
    olditemtype = gcp->thistype;
    gcp->itemID = context->itemID;
    gcp->thistype = OBJ_SEQFEAT;
  }

  ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MultiIntervalGene, "Gene feature on non-segmented sequence should not have multiple intervals");

  if (gcp != NULL) {
    gcp->itemID = olditemid;
    gcp->thistype = olditemtype;
  }
}

static Boolean LIBCALLBACK ValidateSeqFeatIndexed (SeqFeatPtr sfp, SeqMgrFeatContextPtr context)
{
  ValidStructPtr  vsp;
  BioseqValidStrPtr bvsp;

  bvsp = (BioseqValidStrPtr) context->userdata;
  vsp = bvsp->vsp;

  if (sfp->data.choice == SEQFEAT_GENE) {
    CheckMultiIntervalGene (sfp, context, vsp, vsp->gcp);
  }

  return ValidateSeqFeatCommon (sfp, bvsp, vsp, context->left, context->right, context->itemID, context->farloc, context->bsp);
}

static void ValidateSeqFeatContext (GatherContextPtr gcp)
{
  ValidStructPtr  vsp;
  BioseqValidStrPtr bvsp;
  SeqFeatPtr      sfp;

  bvsp = (BioseqValidStrPtr) (gcp->userdata);
  vsp = bvsp->vsp;
  sfp = (SeqFeatPtr) (gcp->thisitem);

  ValidateSeqFeatCommon (sfp, bvsp, vsp, gcp->extremes.left, gcp->extremes.right, 0, FALSE, NULL);
}

/*****************************************************************************
*
*   CountryIsValid(name)
*      Validates subsource country against official country names
*
*****************************************************************************/

static CharPtr  countrycodes[] = {
  "Afghanistan",
  "Albania",
  "Algeria",
  "American Samoa",
  "Andorra",
  "Angola",
  "Anguilla",
  "Antarctica",
  "Antigua and Barbuda",
  "Argentina",
  "Armenia",
  "Aruba",
  "Ashmore and Cartier Islands",
  "Australia",
  "Austria",
  "Azerbaijan",
  "Bahamas",
  "Bahrain",
  "Baker Island",
  "Bangladesh",
  "Barbados",
  "Bassas da India",
  "Belarus",
  "Belgium",
  "Belize",
  "Benin",
  "Bermuda",
  "Bhutan",
  "Bolivia",
  "Bosnia and Herzegovina",
  "Botswana",
  "Bouvet Island",
  "Brazil",
  "British Virgin Islands",
  "Brunei",
  "Bulgaria",
  "Burkina Faso",
  "Burundi",
  "Cambodia",
  "Cameroon",
  "Canada",
  "Cape Verde",
  "Cayman Islands",
  "Central African Republic",
  "Chad",
  "Chile",
  "China",
  "Christmas Island",
  "Clipperton Island",
  "Cocos Islands",
  "Colombia",
  "Comoros",
  "Cook Islands",
  "Coral Sea Islands",
  "Costa Rica",
  "Cote d'Ivoire",
  "Croatia",
  "Cuba",
  "Cyprus",
  "Czech Republic",
  "Democratic Republic of the Congo",
  "Denmark",
  "Djibouti",
  "Dominica",
  "Dominican Republic",
  "Ecuador",
  "Egypt",
  "El Salvador",
  "Equatorial Guinea",
  "Eritrea",
  "Estonia",
  "Ethiopia",
  "Europa Island",
  "Falkland Islands (Islas Malvinas)",
  "Faroe Islands",
  "Fiji",
  "Finland",
  "France",
  "French Guiana",
  "French Polynesia",
  "French Southern and Antarctic Lands",
  "Gabon",
  "Gambia",
  "Gaza Strip",
  "Georgia",
  "Germany",
  "Ghana",
  "Gibraltar",
  "Glorioso Islands",
  "Greece",
  "Greenland",
  "Grenada",
  "Guadeloupe",
  "Guam",
  "Guatemala",
  "Guernsey",
  "Guinea",
  "Guinea-Bissau",
  "Guyana",
  "Haiti",
  "Heard Island and McDonald Islands",
  "Honduras",
  "Hong Kong",
  "Howland Island",
  "Hungary",
  "Iceland",
  "India",
  "Indonesia",
  "Iran",
  "Iraq",
  "Ireland",
  "Isle of Man",
  "Israel",
  "Italy",
  "Jamaica",
  "Jan Mayen",
  "Japan",
  "Jarvis Island",
  "Jersey",
  "Johnston Atoll",
  "Jordan",
  "Juan de Nova Island",
  "Kazakhstan",
  "Kenya",
  "Kingman Reef",
  "Kiribati",
  "Kuwait",
  "Kyrgyzstan",
  "Laos",
  "Latvia",
  "Lebanon",
  "Lesotho",
  "Liberia",
  "Libya",
  "Liechtenstein",
  "Lithuania",
  "Luxembourg",
  "Macau",
  "Macedonia",
  "Madagascar",
  "Malawi",
  "Malaysia",
  "Maldives",
  "Mali",
  "Malta",
  "Marshall Islands",
  "Martinique",
  "Mauritania",
  "Mauritius",
  "Mayotte",
  "Mexico",
  "Micronesia",
  "Midway Islands",
  "Moldova",
  "Monaco",
  "Mongolia",
  "Montserrat",
  "Morocco",
  "Mozambique",
  "Myanmar",
  "Namibia",
  "Nauru",
  "Navassa Island",
  "Nepal",
  "Netherlands",
  "Netherlands Antilles",
  "New Caledonia",
  "New Zealand",
  "Nicaragua",
  "Niger",
  "Nigeria",
  "Niue",
  "Norfolk Island",
  "North Korea",
  "Northern Mariana Islands",
  "Norway",
  "Oman",
  "Pakistan",
  "Palau",
  "Palmyra Atoll",
  "Panama",
  "Papua New Guinea",
  "Paracel Islands",
  "Paraguay",
  "Peru",
  "Philippines",
  "Pitcairn Islands",
  "Poland",
  "Portugal",
  "Puerto Rico",
  "Qatar",
  "Republic of the Congo",
  "Reunion",
  "Romania",
  "Russia",
  "Rwanda",
  "Saint Helena",
  "Saint Kitts and Nevis",
  "Saint Lucia",
  "Saint Pierre and Miquelon",
  "Saint Vincent and the Grenadines",
  "Samoa",
  "San Marino",
  "Sao Tome and Principe",
  "Saudi Arabia",
  "Senegal",
  "Seychelles",
  "Sierra Leone",
  "Singapore",
  "Slovakia",
  "Slovenia",
  "Solomon Islands",
  "Somalia",
  "South Africa",
  "South Georgia and the South Sandwich Islands",
  "South Korea",
  "Spain",
  "Spratly Islands",
  "Sri Lanka",
  "Sudan",
  "Suriname",
  "Svalbard",
  "Swaziland",
  "Sweden",
  "Switzerland",
  "Syria",
  "Taiwan",
  "Tajikistan",
  "Tanzania",
  "Thailand",
  "Togo",
  "Tokelau",
  "Tonga",
  "Trinidad and Tobago",
  "Tromelin Island",
  "Tunisia",
  "Turkey",
  "Turkmenistan",
  "Turks and Caicos Islands",
  "Tuvalu",
  "Uganda",
  "Ukraine",
  "United Arab Emirates",
  "United Kingdom",
  "Uruguay",
  "USA",
  "Uzbekistan",
  "Vanuatu",
  "Venezuela",
  "Viet Nam",
  "Virgin Islands",
  "Wake Island",
  "Wallis and Futuna",
  "West Bank",
  "Western Sahara",
  "Yemen",
  "Yugoslavia",
  "Zambia",
  "Zimbabwe"
};

static Boolean CountryIsValid (CharPtr name)
{
  Int2            L, R, mid;
  CharPtr         ptr;
  Char            str[256];

  if (StringHasNoText (name))
    return FALSE;
  StringNCpy_0 (str, name, sizeof (str));
  ptr = StringChr (str, ':');
  if (ptr != NULL) {
    *ptr = '\0';
  }

  L = 0;
  R = sizeof (countrycodes) / sizeof (countrycodes[0]);

  while (L < R) {
    mid = (L + R) / 2;
    if (StringICmp (countrycodes[mid], str) < 0) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }

  if (StringICmp (countrycodes[R], str) == 0) {
    return TRUE;
  }

  return FALSE;
}

/*****************************************************************************
*
*   ValidateSeqDescrContext(gcp)
*      Gather callback helper function for validating context on a Bioseq
*
*****************************************************************************/
static void ValidateBioSource (ValidStructPtr vsp, GatherContextPtr gcp, BioSourcePtr biop)
{
  Boolean         chromconf = FALSE;
  Int2            chromcount = 0;
  SubSourcePtr    chromosome = NULL;
  CharPtr         countryname;
  ValNodePtr      db;
  DbtagPtr        dbt;
  Int2            i;
  Int4            id;
  OrgNamePtr      onp;
  OrgModPtr       omp;
  OrgRefPtr       orp;
  SubSourcePtr    ssp;

  if (biop == NULL)
    return;
  ssp = biop->subtype;
  while (ssp != NULL) {
    if (ssp->subtype == SUBSRC_country) {
      if (!CountryIsValid (ssp->name)) {
        countryname = ssp->name;
        if (StringHasNoText (countryname)) {
          countryname = "?";
        }
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadCountryCode, "Bad country name [%s]", countryname);
      }
    } else if (ssp->subtype == SUBSRC_chromosome) {
      chromcount++;
      if (chromosome != NULL) {
        if (StringICmp (ssp->name, chromosome->name) != 0) {
          chromconf = TRUE;
        }
      } else {
        chromosome = ssp;
      }
    } else if (ssp->subtype == 0) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadSubSource, "Unknown subsource subtype %d", (int) (ssp->subtype));
    }
    ssp = ssp->next;
  }
  if (chromcount > 1) {
    if (chromconf) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_MultipleChromosomes, "Multiple conflicting chromosome qualifiers");
    } else {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_MultipleChromosomes, "Multiple identical chromosome qualifiers");
    }
  }
  orp = biop->org;
  if (orp == NULL || (StringHasNoText (orp->taxname) && StringHasNoText (orp->common))) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_NoOrgFound, "No organism name has been applied to this Bioseq.");
  }
  if (orp == NULL)
    return;
  onp = orp->orgname;
  if (onp == NULL || StringHasNoText (onp->lineage)) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_MissingLineage, "No lineage for this BioSource.");
  } else {
    if (biop->genome == GENOME_kinetoplast) {
      if (StringStr (onp->lineage, "Kinetoplastida") == 0) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadOrganelle, "Only Kinetoplastida have kinetoplasts");
      }
    } else if (biop->genome == GENOME_nucleomorph) {
      if (StringStr (onp->lineage, "Chlorarchniophyta") == 0 && StringStr (onp->lineage, "Cryptophyta") == 0) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadOrganelle, "Only Chlorarchniophyta and Cryptophyta have nucleomorphs");
      }
    }
  }
  if (onp != NULL) {
    omp = onp->mod;
    while (omp != NULL) {
      if (omp->subtype == 0 || omp->subtype == 1) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadOrgMod, "Unknown orgmod subtype %d", (int) (omp->subtype));
      }
      omp = omp->next;
    }
  }

  for (db = orp->db; db != NULL; db = db->next) {
    id = -1;
    dbt = (DbtagPtr) db->data.ptrvalue;
    if (dbt != NULL && dbt->db != NULL) {
      for (i = 0; i < DBNUM; i++) {
        if (StringCmp (dbt->db, dbtag [i]) == 0) {
          id = i;
          break;
        }
      }
      if (id == -1 || id < 4) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_IllegalDbXref, "Illegal db_xref type %s", dbt->db);
      }
    }
  }

  if (GetAppProperty ("InternalNcbiSequin") == NULL) return;

  for (db = orp->db; db != NULL; db = db->next) {
    dbt = (DbtagPtr) db->data.ptrvalue;
    if (dbt != NULL) {
      if (StringICmp (dbt->db, "taxon") == 0)
        return;
    }
  }
  ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_NoTaxonID, "BioSource is missing taxon ID");
}

static Boolean IsXr (ValNodePtr sdp)

{
  BioseqPtr      bsp;
  ObjValNodePtr  ovp;
  SeqIdPtr       sip;
  TextSeqIdPtr   tsip;

  if (sdp->extended == 0) return FALSE;
  ovp = (ObjValNodePtr) sdp;
  if (ovp->idx.parenttype != OBJ_BIOSEQ) return FALSE;
  bsp = (BioseqPtr) ovp->idx.parentptr;
  if (bsp == NULL) return FALSE;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice != SEQID_OTHER) continue;
    tsip = (TextSeqIdPtr) sip->data.ptrvalue;
    if (tsip == NULL) continue;
    if (StringNICmp (tsip->accession, "XR_", 3) == 0) return TRUE;
  }
  return FALSE;
}

static Boolean ValidateSeqDescrCommon (ValNodePtr sdp, BioseqValidStrPtr bvsp, ValidStructPtr vsp, Uint2 descitemid)
{
  ValNodePtr      vnp, vnp2;
  OrgRefPtr       this_org = NULL, that_org = NULL;
  int             tmpval;
  Char            buf1[20], buf2[20];
  PubdescPtr      pdp;
  MolInfoPtr      mip;
  Uint2           olditemtype = 0;
  Uint2           olditemid = 0;
  BioSourcePtr    biop;
  GatherContextPtr gcp = NULL;
  CharPtr         str;
  static char    *badmod = "Inconsistent GIBB-mod [%d] and [%d]";

  vsp->sfp = NULL;
  vnp = sdp;
  vsp->descr = vnp;

  if (descitemid > 0) {
    gcp = vsp->gcp;
    if (gcp != NULL) {
      olditemid = gcp->itemID;
      olditemtype = gcp->thistype;
      gcp->itemID = descitemid;
      gcp->thistype = OBJ_SEQDESC;
    }
  }

  switch (vnp->choice) {
  case Seq_descr_mol_type:
    tmpval = (int) (vnp->data.intvalue);
    switch (tmpval) {
    case 8:                    /* peptide */
      if (!bvsp->is_aa)
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Nuclic acid with GIBB-mol = peptide");
      break;
    case 0:                    /* unknown */
    case 255:                  /* other */
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "GIBB-mol unknown or other used");
      break;
    default:                   /* the rest are nucleic acid */
      if (bvsp->is_aa) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "GIBB-mol [%d] used on protein", tmpval);
      } else {
        if (bvsp->last_na_mol) {
          if (bvsp->last_na_mol != (int) vnp->data.intvalue) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Inconsistent GIBB-mol [%d] and [%d]", bvsp->last_na_mol, tmpval);
          }
        } else
          bvsp->last_na_mol = tmpval;
      }
      break;
    }
    break;
  case Seq_descr_modif:
    for (vnp2 = (ValNodePtr) (vnp->data.ptrvalue); vnp2 != NULL; vnp2 = vnp2->next) {
      tmpval = (int) (vnp2->data.intvalue);
      switch (tmpval) {
      case 0:                  /* dna */
      case 1:                  /* rna */
        if (bvsp->is_aa) {      /* only temporarily on 0 */
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Nucleic acid GIBB-mod [%d] on protein", tmpval);
        } else if (bvsp->last_na_mod) {
          if (tmpval != bvsp->last_na_mod) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, badmod, bvsp->last_na_mod, tmpval);
          }
        } else
          bvsp->last_na_mod = tmpval;
        break;
      case 4:                  /* mitochondria */
      case 5:                  /* chloroplast */
      case 6:                  /* kinetoplast */
      case 7:                  /* cyanelle */
      case 18:                 /* macronuclear */
        if (bvsp->last_organelle) {
          if (tmpval != bvsp->last_na_mod) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, badmod, bvsp->last_organelle, tmpval);
          }
        } else
          bvsp->last_organelle = tmpval;
        break;
      case 10:                 /* partial */
      case 11:                 /* complete */
        if (bvsp->last_partialness) {
          if (tmpval != bvsp->last_partialness) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, badmod, bvsp->last_partialness, tmpval);
          }
        } else
          bvsp->last_partialness = tmpval;
        if ((bvsp->last_left_right) && (tmpval == 11)) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, badmod, bvsp->last_left_right, tmpval);
        }
        break;
      case 16:                 /* no left */
      case 17:                 /* no right */
        if (bvsp->last_partialness == 11) {     /* complete */
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, badmod, bvsp->last_partialness, tmpval);
        }
        bvsp->last_left_right = tmpval;
        break;
      case 255:                /* other */
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Unknown, "GIBB-mod = other used");
        break;
      default:
        break;

      }
    }
    break;
  case Seq_descr_method:
    if (!bvsp->is_aa) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Nucleic acid with protein sequence method");
    }
    break;
  case Seq_descr_comment:
    str = (CharPtr) vnp->data.ptrvalue;
    if (SerialNumberInString (str)) {
      ValidErr (vsp, SEV_INFO, ERR_SEQ_DESCR_SerialInComment,
                "Comment may refer to reference by serial number - attach reference specific comments to the reference REMARK instead.");
    }
    break;
  case Seq_descr_genbank:
    if (bvsp->last_gb != NULL)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Multiple GenBank blocks");
    else
      bvsp->last_gb = vnp;
    break;
  case Seq_descr_embl:
    if (bvsp->last_embl != NULL)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Multiple EMBL blocks");
    else
      bvsp->last_embl = vnp;
    break;
  case Seq_descr_pir:
    if (bvsp->last_pir != NULL)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Multiple PIR blocks");
    else
      bvsp->last_pir = vnp;
    break;
  case Seq_descr_sp:
    if (bvsp->last_sp != NULL)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Multiple SWISS-PROT blocks");
    else
      bvsp->last_sp = vnp;
    break;
  case Seq_descr_pdb:
    if (bvsp->last_pdb != NULL)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Multiple PDB blocks");
    else
      bvsp->last_pdb = vnp;
    break;
  case Seq_descr_prf:
    if (bvsp->last_prf != NULL)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Multiple PRF blocks");
    else
      bvsp->last_prf = vnp;
    break;
  case Seq_descr_create_date:
    if (bvsp->last_create != NULL) {
      tmpval = (int) DateMatch ((DatePtr) vnp->data.ptrvalue, (DatePtr) (bvsp->last_create->data.ptrvalue), FALSE);
      if (tmpval) {
        DatePrint ((DatePtr) (vnp->data.ptrvalue), buf1);
        DatePrint ((DatePtr) (bvsp->last_create->data.ptrvalue), buf2);
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_Inconsistent, "Inconsistent create_dates [%s] and [%s]", buf1, buf2);
      }
    } else
      bvsp->last_create = vnp;
    if (bvsp->last_update != NULL) {
      tmpval = (int) DateMatch ((DatePtr) vnp->data.ptrvalue, (DatePtr) (bvsp->last_update->data.ptrvalue), FALSE);
      if (tmpval == 1) {
        DatePrint ((DatePtr) (vnp->data.ptrvalue), buf1);
        DatePrint ((DatePtr) (bvsp->last_update->data.ptrvalue), buf2);
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_Inconsistent, "Inconsistent create_date [%s] and update_date [%s]", buf1, buf2);
      }
    }
    break;
  case Seq_descr_update_date:
    if (bvsp->last_create != NULL) {
      tmpval = (int) DateMatch ((DatePtr) bvsp->last_create->data.ptrvalue, (DatePtr) (vnp->data.ptrvalue), FALSE);
      if (tmpval == 1) {
        DatePrint ((DatePtr) (bvsp->last_create->data.ptrvalue), buf1);
        DatePrint ((DatePtr) (vnp->data.ptrvalue), buf2);
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_Inconsistent, "Inconsistent create_date [%s] and update_date [%s]", buf1, buf2);
      }
    }
    if (bvsp->last_update == NULL)
      bvsp->last_update = vnp;
    break;
  case Seq_descr_source:
    biop = (BioSourcePtr) vnp->data.ptrvalue;
    /* ValidateBioSource (vsp, gcp, biop); */
    this_org = biop->org;
    /* fall into Seq_descr_org */
  case Seq_descr_org:
    if (this_org == NULL)
      this_org = (OrgRefPtr) (vnp->data.ptrvalue);
    if (bvsp->last_org != NULL) {
      if ((this_org->taxname != NULL) && (bvsp->last_org->taxname != NULL)) {
        if (StringCmp (this_org->taxname, bvsp->last_org->taxname)) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Inconsistent taxnames [%s] and [%s]", this_org->taxname, bvsp->last_org->taxname);
        }
      }
    } else
      bvsp->last_org = this_org;
    for (vnp2 = vnp->next; vnp2 != NULL; vnp2 = vnp2->next) {
      if (vnp2->choice == Seq_descr_source || vnp2->choice == Seq_descr_org) {
        that_org = NULL;
        if (vnp2->choice == Seq_descr_source) {
          that_org = ((BioSourcePtr) (vnp2->data.ptrvalue))->org;
        }
        if (that_org == NULL) {
          that_org = (OrgRefPtr) (vnp2->data.ptrvalue);
        }
        if (that_org != NULL) {
          if ((this_org->taxname != NULL) && (that_org->taxname != NULL) && StringCmp (this_org->taxname, that_org->taxname) == 0) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_MultipleBioSources, "Undesired multiple source descriptors");
          }
        }
      }
    }
    break;
  case Seq_descr_pub:
    bvsp->got_a_pub = TRUE;
    pdp = (PubdescPtr) vnp->data.ptrvalue;
    /*
       ValidatePubdesc (vsp, pdp);
     */
    break;
  case Seq_descr_molinfo:
    mip = (MolInfoPtr) vnp->data.ptrvalue;
    if (mip != NULL) {
      switch (mip->biomol) {
      case MOLECULE_TYPE_PEPTIDE:      /* peptide */
        if (!bvsp->is_aa) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Nuclic acid with Molinfo-biomol = peptide");
        }
        break;
      case 0:                  /* unknown */
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Molinfo-biomol unknown used");
        break;
      case 255:                /* other */
        if (! IsXr (vnp)) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_InvalidForType, "Molinfo-biomol other used");
        }
        break;
      default:                 /* the rest are nucleic acid */
        if (bvsp->is_aa) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Molinfo-biomol [%d] used on protein", (int) mip->biomol);
        } else {
          if (bvsp->last_biomol) {
            if (bvsp->last_biomol != (int) mip->biomol) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Inconsistent Molinfo-biomol [%d] and [%d]", bvsp->last_biomol, (int) mip->biomol);
            }
          } else {
            bvsp->last_biomol = (int) mip->biomol;
          }
        }
        break;
      }
      if (!bvsp->is_aa) {
        switch (mip->tech) {
        case MI_TECH_concept_trans:
        case MI_TECH_seq_pept:
        case MI_TECH_both:
        case MI_TECH_seq_pept_overlap:
        case MI_TECH_seq_pept_homol:
        case MI_TECH_concept_trans_a:
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Nucleic acid with protein sequence method");
          break;
        default:
          break;
        }
      } else {
        switch (mip->tech) {
        case MI_TECH_est:
        case MI_TECH_sts:
        case MI_TECH_genemap:
        case MI_TECH_physmap:
        case MI_TECH_htgs_1:
        case MI_TECH_htgs_2:
        case MI_TECH_htgs_3:
        case MI_TECH_fli_cdna:
        case MI_TECH_htgs_0:
        case MI_TECH_htc:
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Protein with nucleic acid sequence method");
          break;
        default:
          break;
        }
      }
      if (bvsp->last_tech) {
        if (bvsp->last_tech != (int) mip->tech) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Inconsistent Molinfo-tech [%d] and [%d]", bvsp->last_tech, (int) mip->tech);
        }
      } else {
        bvsp->last_tech = (int) mip->tech;
      }
      if (bvsp->last_completeness) {
        if (bvsp->last_completeness != (int) mip->completeness) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Inconsistent Molinfo-completeness [%d] and [%d]",
                    bvsp->last_completeness, (int) mip->completeness);
        }
      } else {
        bvsp->last_completeness = (int) mip->completeness;
      }
    }
    break;
  default:
    break;
  }


  if (gcp != NULL) {
    gcp->itemID = olditemid;
    gcp->thistype = olditemtype;
  }

  return TRUE;
}

static Boolean LIBCALLBACK ValidateSeqDescrIndexed (ValNodePtr sdp, SeqMgrDescContextPtr context)
{
  ValidStructPtr  vsp;
  BioseqValidStrPtr bvsp;

  bvsp = (BioseqValidStrPtr) context->userdata;
  vsp = bvsp->vsp;

  return ValidateSeqDescrCommon (sdp, bvsp, vsp, context->itemID);
}

static void ValidateSeqDescrContext (GatherContextPtr gcp)
{
  ValidStructPtr  vsp;
  BioseqValidStrPtr bvsp;
  ValNodePtr      sdp;

  bvsp = (BioseqValidStrPtr) (gcp->userdata);
  vsp = bvsp->vsp;
  sdp = (ValNodePtr) (gcp->thisitem);

  ValidateSeqDescrCommon (sdp, bvsp, vsp, 0);
}

/*****************************************************************************
*
*   ValidateBioseqContextGather(gcp)
*      Gather callback for validating context on a Bioseq
*
*****************************************************************************/
static void ValidateCitSub (ValidStructPtr vsp, CitSubPtr csp)
{
  AuthListPtr     alp;
  ValNodePtr      name;
  Boolean         hasName = FALSE;

  if (vsp == NULL || csp == NULL)
    return;
  alp = csp->authors;
  if (alp != NULL) {
    if (alp->choice == 1) {
      for (name = alp->names; name != NULL; name = name->next) {
        if (!HasNoName (name)) {
          hasName = TRUE;
        }
      }
    } else if (alp->choice == 2 || alp->choice == 3) {
      for (name = alp->names; name != NULL; name = name->next) {
        if (!HasNoText ((CharPtr) name->data.ptrvalue)) {
          hasName = TRUE;
        }
      }
    }
  }
  if (!hasName) {
    ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MissingPubInfo, "Submission citation has no author names");
  }
}

static Boolean DifferentDbxrefs (ValNodePtr dbxref1, ValNodePtr dbxref2)
{
  DbtagPtr        dbt1, dbt2;
  ObjectIdPtr     oip1, oip2;

  if (dbxref1 == NULL || dbxref2 == NULL)
    return FALSE;
  dbt1 = (DbtagPtr) dbxref1->data.ptrvalue;
  dbt2 = (DbtagPtr) dbxref2->data.ptrvalue;
  if (dbt1 == NULL || dbt2 == NULL)
    return FALSE;
  if (StringICmp (dbt1->db, dbt2->db) != 0)
    return TRUE;
  oip1 = dbt1->tag;
  oip2 = dbt2->tag;
  if (oip1 == NULL || oip2 == NULL)
    return FALSE;
  if (oip1->str == NULL && oip2->str == NULL) {
    if (oip1->id != oip2->id)
      return TRUE;
  } else {
    if (StringICmp (oip1->str, oip2->str) != 0)
      return TRUE;
  }
  return FALSE;
}

static Boolean GPSorNTorNC (SeqEntryPtr sep, SeqLocPtr location)
{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  SeqIdPtr        sip;
  TextSeqIdPtr    tsip;

  if (sep != NULL && IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set) {
      return TRUE;
    }
  }
  bsp = BioseqFindFromSeqLoc (location);
  if (bsp != NULL) {
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_OTHER) {
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL && tsip->accession != NULL) {
          if (StringNICmp (tsip->accession, "NT_", 3) == 0) {
            return TRUE;
          } else if (StringNICmp (tsip->accession, "NC_", 3) == 0) {
            return TRUE;
          }
        }
      }
    }
  }
  return FALSE;
}

static Boolean NGorNT (SeqEntryPtr sep, SeqLocPtr location, BoolPtr is_nc)
{
  BioseqPtr       bsp;
  SeqIdPtr        sip;
  TextSeqIdPtr    tsip;

  if (is_nc != NULL) {
    *is_nc = FALSE;
  }
  bsp = BioseqFindFromSeqLoc (location);
  if (bsp != NULL) {
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_OTHER) {
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL && tsip->accession != NULL) {
          if (StringNICmp (tsip->accession, "NT_", 3) == 0) {
            return TRUE;
          } else if (StringNICmp (tsip->accession, "NG_", 3) == 0) {
            return TRUE;
          } else if (StringNICmp (tsip->accession, "NC_", 3) == 0 && is_nc != NULL) {
            *is_nc = TRUE;
          }
        }
      }
    }
  }
  return FALSE;
}

static Boolean GPSorRefSeq (SeqEntryPtr sep, SeqLocPtr location)
{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  SeqIdPtr      sip;

  if (sep != NULL && IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set) {
      return TRUE;
    }
  }
  bsp = BioseqFindFromSeqLoc (location);
  if (bsp != NULL) {
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_OTHER) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

static void CheckForNucProt (BioseqSetPtr bssp, Pointer userdata)
{
  BoolPtr         hasPartsP;

  if (bssp->_class == BioseqseqSet_class_nuc_prot) {
    hasPartsP = (BoolPtr) userdata;
    *hasPartsP = TRUE;
  }
}

static void CheckForParts (BioseqSetPtr bssp, Pointer userdata)
{
  BoolPtr         hasPartsP;

  if (bssp->_class == BioseqseqSet_class_parts) {
    hasPartsP = (BoolPtr) userdata;
    *hasPartsP = TRUE;
  }
}

static Boolean DeltaOrFarSeg (SeqEntryPtr sep, SeqLocPtr location)
{
  BioseqPtr       bsp;
  Boolean         hasParts = FALSE;

  bsp = BioseqFindFromSeqLoc (location);
  if (bsp != NULL) {
    if (bsp->repr == Seq_repr_delta) {
      VisitSetsInSep (sep, (Pointer) &hasParts, CheckForNucProt);
      if (!hasParts)
        return TRUE;
    }
    if (bsp->repr == Seq_repr_seg) {
      VisitSetsInSep (sep, (Pointer) &hasParts, CheckForParts);
      if (!hasParts)
        return TRUE;
    }
  }
  return FALSE;
}

static Boolean IsNC (SeqEntryPtr sep, SeqLocPtr location)
{
  BioseqPtr       bsp;
  SeqIdPtr        sip;
  TextSeqIdPtr    tsip;

  bsp = BioseqFindFromSeqLoc (location);
  if (bsp != NULL) {
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_OTHER) {
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL && tsip->accession != NULL) {
          if (StringNICmp (tsip->accession, "NC_", 3) == 0) {
            return TRUE;
          }
        }
      }
    }
  }
  return FALSE;
}

static Boolean NotPeptideException (SeqFeatPtr sfp, SeqFeatPtr last)
{
  if (sfp != NULL && sfp->excpt) {
    if (StringICmp (sfp->except_text, "alternative processing") == 0 || StringICmp (sfp->except_text, "alternate processing") == 0)
      return FALSE;
  }
  if (last != NULL && last->excpt) {
    if (StringICmp (last->except_text, "alternative processing") == 0 || StringICmp (last->except_text, "alternate processing") == 0)
      return FALSE;
  }
  return TRUE;
}

static Boolean ValidateBioseqContextIndexed (BioseqPtr bsp, BioseqValidStrPtr bvsp)
{
  ValidStructPtr  vsp;
  CitSubPtr       csp;
  ObjMgrDataPtr   omdp;
  SeqSubmitPtr    ssp;
  SubmitBlockPtr  sbp;
  GatherContextPtr gcp;
  SeqFeatPtr      sfp;
  SeqMgrFeatContext fcontext;
  Uint2           featdeftype = 0;
  SeqFeatPtr      last = NULL;
  Boolean         leave;
  CharPtr         label = NULL;
  CharPtr         comment = NULL;
  Int4            left = 0;
  Int4            right = 0;
  Uint1           strand = 0;
  Int2            numivals = 0;
  Int4Ptr         ivals = NULL;
  Boolean         ivalssame;
  SeqAnnotPtr     sap = NULL;
  Uint2           olditemtype = 0;
  Uint2           olditemid = 0;
  CharPtr         lastLabel;
  CharPtr         message;
  Int2            i;
  Int2            j;
  CdRegionPtr     crp;
  Uint1           frame;
  int             severity;
  int             overlapPepSev;

  gcp = bvsp->gcp;
  vsp = bvsp->vsp;
  vsp->descr = NULL;
  vsp->sfp = NULL;
  vsp->gcp = gcp;               /* needed for ValidErr */

  SeqMgrExploreFeatures (bsp, (Pointer) bvsp, ValidateSeqFeatIndexed, NULL, NULL, NULL);

  overlapPepSev = SEV_WARNING;
  if (GetAppProperty ("SpliceValidateAsError") != NULL) {
    overlapPepSev = SEV_ERROR;
  }

  if (gcp != NULL) {
    olditemid = gcp->itemID;
    olditemtype = gcp->thistype;
  }

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL) {
    leave = TRUE;
    if (last != NULL) {
      if (fcontext.left == left && fcontext.right == right && fcontext.featdeftype == featdeftype) {
        if (fcontext.strand == strand || strand == Seq_strand_unknown || fcontext.strand == Seq_strand_unknown) {
          ivalssame = TRUE;
          if (fcontext.numivals != numivals || fcontext.ivals == NULL || ivals == NULL) {
            ivalssame = FALSE;
          } else {
            for (i = 0, j = 0; i < numivals; i++, j += 2) {
              if (fcontext.ivals[j] != ivals[j]) {
                ivalssame = FALSE;
              }
              if (fcontext.ivals[j + 1] != ivals[j + 1]) {
                ivalssame = FALSE;
              }
            }
          }
          if (ivalssame &&      /* StringICmp (fcontext.label, label) == 0 && */
              (fcontext.sap == sap || (fcontext.sap->desc == NULL && sap->desc == NULL))) {
            if (gcp != NULL) {
              gcp->itemID = fcontext.itemID;
              gcp->thistype = OBJ_SEQFEAT;
            }
            vsp->descr = NULL;
            vsp->sfp = sfp;
            severity = SEV_ERROR;
            if (featdeftype == FEATDEF_PUB ||
                featdeftype == FEATDEF_REGION || featdeftype == FEATDEF_misc_feature || featdeftype == FEATDEF_STS || featdeftype == FEATDEF_variation) {
              severity = SEV_WARNING;
            } else if (StringICmp (fcontext.label, label) != 0 || StringICmp (sfp->comment, comment) != 0) {
              if (! GPSorNTorNC (vsp->sep, sfp->location)) {
                severity = SEV_WARNING;
              }
            }
            /* if different CDS frames, lower to warning */
            if (sfp->data.choice == SEQFEAT_CDREGION) {
              crp = (CdRegionPtr) sfp->data.value.ptrvalue;
              if (crp != NULL) {
                if (frame > 1 || crp->frame > 1) {
                  if (frame != crp->frame) {
                    severity = SEV_WARNING;
                  }
                }
              }
              if (GPSorNTorNC (vsp->sep, sfp->location)) {
                severity = SEV_WARNING;
              }
            }
            if (featdeftype == FEATDEF_REGION && DifferentDbxrefs (last->dbxref, sfp->dbxref)) {
              /* do not report if both have dbxrefs and they are different */
            } else if (fcontext.sap == sap) {
              ValidErr (vsp, severity, ERR_SEQ_FEAT_DuplicateFeat, "Possible duplicate feature");
            } else {
              ValidErr (vsp, severity, ERR_SEQ_FEAT_DuplicateFeat, "Possible duplicate feature (packaged in different feature table)");
            }
            vsp->sfp = NULL;
            if (gcp != NULL) {
              gcp->itemID = olditemid;
              gcp->thistype = olditemtype;
            }
          }
        }
      }
      if (fcontext.featdeftype == FEATDEF_mat_peptide_aa ||
          fcontext.featdeftype == FEATDEF_sig_peptide_aa || fcontext.featdeftype == FEATDEF_transit_peptide_aa) {
        if (featdeftype == FEATDEF_mat_peptide_aa || featdeftype == FEATDEF_sig_peptide_aa || featdeftype == FEATDEF_transit_peptide_aa) {
          if (fcontext.left <= right && NotPeptideException (sfp, last)) {
            if (gcp != NULL) {
              gcp->itemID = fcontext.itemID;
              gcp->thistype = OBJ_SEQFEAT;
            }
            vsp->descr = NULL;
            vsp->sfp = sfp;
            ValidErr (vsp, overlapPepSev, ERR_SEQ_FEAT_OverlappingPeptideFeat, "Signal, Transit, or Mature peptide features overlap");
            vsp->sfp = NULL;
            if (gcp != NULL) {
              gcp->itemID = olditemid;
              gcp->thistype = olditemtype;
            }
          }
        }
      }
    }
    if (leave) {
      last = sfp;
      left = fcontext.left;
      right = fcontext.right;
      label = fcontext.label;
      comment = sfp->comment;
      strand = fcontext.strand;
      featdeftype = fcontext.featdeftype;
      numivals = fcontext.numivals;
      ivals = fcontext.ivals;
      sap = fcontext.sap;
      frame = 0;
      if (sfp->data.choice == SEQFEAT_CDREGION) {
        crp = (CdRegionPtr) sfp->data.value.ptrvalue;
        if (crp != NULL) {
          frame = crp->frame;
        }
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }

  lastLabel = NULL;
  sfp = SeqMgrGetNextFeatureByLabel (bsp, NULL, SEQFEAT_GENE, 0, &fcontext);
  while (sfp != NULL) {
    label = fcontext.label;
    if (lastLabel != NULL) {
      message = NULL;
      if (StringCmp (lastLabel, label) == 0) {
        message = "Colliding names in gene features";
      } else if (StringICmp (lastLabel, label) == 0) {
        message = "Colliding names (with different capitalization) in gene features";
      }
      if (message != NULL) {
        if (gcp != NULL) {
          gcp->itemID = fcontext.itemID;
          gcp->thistype = OBJ_SEQFEAT;
        }
        vsp->descr = NULL;
        vsp->sfp = sfp;
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_CollidingGeneNames, "%s", message);
        vsp->sfp = NULL;
        if (gcp != NULL) {
          gcp->itemID = olditemid;
          gcp->thistype = olditemtype;
        }
      }
    }
    lastLabel = label; 
    sfp = SeqMgrGetNextFeatureByLabel (bsp, sfp, SEQFEAT_GENE, 0, &fcontext);
  }

  SeqMgrExploreDescriptors (bsp, (Pointer) bvsp, ValidateSeqDescrIndexed, NULL);

  omdp = ObjMgrGetData (gcp->entityID);
  if (omdp != NULL && omdp->datatype == OBJ_SEQSUB) {
    ssp = (SeqSubmitPtr) omdp->dataptr;
    if (ssp != NULL) {
      sbp = ssp->sub;
      if (sbp != NULL) {
        bvsp->got_a_pub = TRUE;
        csp = sbp->cit;
        /* csp = (CitSubPtr) gcp->thisitem; */
        ValidateCitSub (vsp, csp);
      }
    }
  }

  return TRUE;
}

static Boolean ValidateBioseqContextGather (GatherContextPtr gcp)
{
  ValidStructPtr  vsp;
  BioseqValidStrPtr bvsp;
  CitSubPtr       csp;

  bvsp = (BioseqValidStrPtr) (gcp->userdata);
  vsp = bvsp->vsp;
  vsp->descr = NULL;
  vsp->sfp = NULL;
  vsp->gcp = gcp;               /* needed for ValidErr */

  switch (gcp->thistype) {
  case OBJ_SEQFEAT:
    ValidateSeqFeatContext (gcp);
    break;
  case OBJ_SEQDESC:
    ValidateSeqDescrContext (gcp);
    break;
  case OBJ_SEQSUB_CIT:
    bvsp->got_a_pub = TRUE;
    csp = (CitSubPtr) gcp->thisitem;
    ValidateCitSub (vsp, csp);
    break;
  default:
    break;
  }
  return TRUE;
}

/*****************************************************************************
*
*   ValidateBioseqContext(gcp)
*      Validate one Bioseq for descriptors, features, and context
*      This is done as a second Gather, focussed on the Bioseq in
*        question.
*
*****************************************************************************/
static void ValidateBioseqContext (GatherContextPtr gcp)
{
  ValidStructPtr  vsp;
  BioseqPtr       bsp;
  GatherScope     gs;
  BioseqValidStr  bvs;
  SeqFeatPtr      sfp;
  ValNode         fake_whole;
  SeqIdPtr        sip;
  ValNodePtr      vnp = NULL;
  MolInfoPtr      mip = NULL;
  SeqMgrDescContext context;
  BioseqContextPtr bcp;
  ObjMgrDataPtr   omdp;
  PatentSeqIdPtr  psip;
  IdPatPtr        ipp;
  Boolean         isPDB = FALSE;

  vsp = (ValidStructPtr) (gcp->userdata);
  bsp = (BioseqPtr) (gcp->thisitem);
  vsp->bsp = bsp;
  vsp->descr = NULL;
  vsp->sfp = NULL;
  vsp->bssp = (BioseqSetPtr) (gcp->parentitem);

  MemSet (&gs, 0, sizeof (GatherScope));
  fake_whole.choice = SEQLOC_WHOLE;
  sip = SeqIdFindBest (bsp->id, 0);

  fake_whole.data.ptrvalue = sip;

  fake_whole.next = NULL;
  gs.target = &fake_whole;
  gs.get_feats_location = TRUE;
  gs.nointervals = TRUE;
  MemSet ((Pointer) (gs.ignore), (int) TRUE, (size_t) (sizeof (Boolean) * OBJ_MAX));
  gs.ignore[OBJ_SEQDESC] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.ignore[OBJ_SUBMIT_BLOCK] = FALSE;
  gs.ignore[OBJ_SEQSUB_CIT] = FALSE;

  gs.scope = vsp->sep;

  MemSet (&bvs, 0, sizeof (BioseqValidStr));
  bvs.vsp = vsp;

  /* now looking for molinfo on every bioseq (okay on segset) */
  if (bsp != NULL) {
    vnp = NULL;
    if (vsp->useSeqMgrIndexes) {
      vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &context);
    } else {
      bcp = BioseqContextNew (bsp);
      vnp = BioseqContextGetSeqDescr (bcp, Seq_descr_molinfo, NULL, NULL);
      BioseqContextFree (bcp);
    }
    if (vnp != NULL) {
      mip = (MolInfoPtr) vnp->data.ptrvalue;
    }
  }

  bvs.is_mrna = FALSE;
  bvs.is_prerna = FALSE;
  if (bsp != NULL && ISA_na (bsp->mol)) {
    if (mip != NULL) {
      if (mip->biomol == MOLECULE_TYPE_MRNA) {
        bvs.is_mrna = TRUE;
      } else if (mip->biomol == MOLECULE_TYPE_PRE_MRNA) {
        bvs.is_prerna = TRUE;
      }
      if (mip->biomol >= MOLECULE_TYPE_PRE_MRNA && mip->biomol <= MOLECULE_TYPE_SCRNA && bsp->mol == Seq_mol_dna) {
        /* - this is how we indicate an mRNA sequenced from a cDNA, so no error
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_RnaDnaConflict, "MolInfo says RNA, Bioseq says DNA");
        */
      }
    } else if (bsp->mol == Seq_mol_rna) {
      bvs.is_mrna = TRUE;       /* if no molinfo, assume rna is mrna */
    }
  }

  if (mip != NULL) {
    if (mip->tech == MI_TECH_sts ||
        mip->tech == MI_TECH_survey ||
        mip->tech == MI_TECH_htgs_0 || mip->tech == MI_TECH_htgs_1 || mip->tech == MI_TECH_htgs_2 || mip->tech == MI_TECH_htgs_3) {
      if (mip->tech == MI_TECH_sts && bsp->mol == Seq_mol_rna && mip->biomol == MOLECULE_TYPE_MRNA) {
        /* there are some STS sequences derived from cDNAs, so do not report these */
      } else if (mip->biomol != MOLECULE_TYPE_GENOMIC) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_ConflictingBiomolTech, "HTGS/STS/GSS sequence should be genomic");
      } else if (bsp == NULL || (bsp->mol != Seq_mol_dna && bsp->mol != Seq_mol_na)) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_ConflictingBiomolTech, "HTGS/STS/GSS sequence should not be RNA");
      }
    }
  }

  if (ISA_aa (bsp->mol)) {
    bvs.is_aa = TRUE;
    /* check proteins in nuc-prot set have a CdRegion */
    if (vsp->bssp != NULL) {
      if (vsp->bssp->_class == 1) {     /* in a nuc-prot set */
        if (vsp->useSeqMgrIndexes) {
          sfp = SeqMgrGetCDSgivenProduct (bsp, NULL);
        } else {
          sfp = SeqEntryGetSeqFeat (vsp->sep, 3, NULL, NULL, 1, bsp);
        }
        if (sfp == NULL)        /* no CdRegion points to this bsp */
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_PKG_NoCdRegionPtr, "No CdRegion in nuc-prot set points to this protein");
      }
    }
  }

  if (vsp->useSeqMgrIndexes) {
    bvs.gcp = gcp;
    ValidateBioseqContextIndexed (bsp, &bvs);
  } else {
    GatherSeqEntry (vsp->sep, &bvs, ValidateBioseqContextGather, &gs);
  }

  vsp->gcp = gcp;               /* reset the gcp pointer changed in previous gather */
  vsp->descr = NULL;
  vsp->sfp = NULL;

  if ((!bvs.got_a_pub) && (!vsp->suppress_no_pubs)) {
    omdp = NULL;
    if (gcp != NULL) {
      omdp = ObjMgrGetData (gcp->entityID);
    }
    if (omdp == NULL || omdp->datatype != OBJ_SEQSUB) {
      if (!IsRefSeq (bsp)) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_NoPubFound, "No publications refer to this Bioseq.");
      }
    }
  }

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_PATENT) {
      psip = (PatentSeqIdPtr) sip->data.ptrvalue;
      if (psip != NULL) {
        ipp = psip->cit;
        if (ipp != NULL && StringICmp (ipp->country, "US") == 0)
          return;
      }
      return;
    } else if (sip->choice == SEQID_PDB) {
      isPDB = TRUE;
    }
  }

  if ((!bvs.last_org) && (!vsp->suppress_no_biosrc))
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_NoOrgFound, "No organism name has been applied to this Bioseq.");


  if ((bvs.is_aa) && (!bvs.num_full_length_prot_ref) && (!isPDB))
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_NoProtRefFound, "No full length Prot-ref feature applied to this Bioseq");

  /* for now only flag missing molinfo in Sequin */
  if (mip == NULL && GetAppProperty ("SpliceValidateAsError") != NULL && (!isPDB)) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_NoMolInfoFound, "No Mol-info applies to this Bioseq");
  }

  return;

}

/*****************************************************************************
*
*   ValidateSeqFeat(gcp)
*
*****************************************************************************/
static Boolean EmptyOrNullString (CharPtr str)
{
  Char            ch;

  if (str == NULL)
    return TRUE;
  ch = *str;
  while (ch != '\0') {
    if (ch > ' ' && ch <= '~')
      return FALSE;
    str++;
    ch = *str;
  }
  return TRUE;
}

static void CheckPeptideOnCodonBoundary (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp, CharPtr key)
{
  SeqFeatPtr      cds;
  CdRegionPtr     crp;
  SeqLocPtr       first = NULL, last = NULL, slp = NULL;
  Boolean         partial5, partial3;
  Int4            pos1, pos2, adjust = 0, mod1, mod2;

  cds = SeqMgrGetOverlappingCDS (sfp->location, NULL);
  if (cds == NULL)
    return;
  crp = (CdRegionPtr) cds->data.value.ptrvalue;
  if (crp == NULL)
    return;
  if (crp->frame == 2) {
    adjust = 1;
  } else if (crp->frame == 3) {
    adjust = 2;
  }

  while ((slp = SeqLocFindNext (sfp->location, slp)) != NULL) {
    last = slp;
    if (first == NULL) {
      first = slp;
    }
  }
  if (first == NULL || last == NULL)
    return;

  pos1 = GetOffsetInLoc (first, cds->location, SEQLOC_START) - adjust;
  pos2 = GetOffsetInLoc (last, cds->location, SEQLOC_STOP) - adjust;
  mod1 = pos1 % 3;
  mod2 = pos2 % 3;

  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  if (partial5) {
    mod1 = 0;
  }
  if (partial3) {
    mod2 = 2;
  }

  if (mod1 != 0 && mod2 != 2) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PeptideFeatOutOfFrame, "Start and stop of %s are out of frame with CDS codons", key);
  } else if (mod1 != 0) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PeptideFeatOutOfFrame, "Start of %s is out of frame with CDS codons", key);
  } else if (mod2 != 2) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PeptideFeatOutOfFrame, "Stop of %s is out of frame with CDS codons", key);
  }
}

static CharPtr  legal_repeat_types[] = {
  "tandem", "inverted", "flanking", "terminal",
  "direct", "dispersed", "other", NULL
};

static void ValidateImpFeat (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp, ImpFeatPtr ifp)
{
  Char            ch;
  Boolean         failed;
  Boolean         found;
  GBQualPtr       gbqual;
  SeqMgrFeatContext gcontext;
  SeqFeatPtr      gene;
  GeneRefPtr      grp;
  Int2            i;
  Int2            index;
  CharPtr         key;
  Boolean         multi_rpt_unit;
  Boolean         no_white_space;
  Boolean         only_digits;
  CharPtr         ptr;
  Int2            qual;
  Char            range[32];
  SeqIntPtr       sint;
  SeqLocPtr       slp;
  CharPtr         str;
  CharPtr         tmp;
  Int2            val;

  if (vsp == NULL || gcp == NULL || sfp == NULL || ifp == NULL)
    return;
  if (StringCmp (ifp->key, "-") == 0) {
    key = StringSave ("misc_feature");
  } else {
    key = StringSaveNoNull (ifp->key);
  }
  index = GBFeatKeyNameValid (&key, FALSE);
  if (index == -1) {
    if (key != NULL) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnknownImpFeatKey, "Unknown feature key %s", key);
    } else {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnknownImpFeatKey, "NULL feature key");
    }
  } else if (StringICmp (key, "virion") == 0 || StringICmp (key, "mutation") == 0 || StringICmp (key, "allele") == 0) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_UnknownImpFeatKey, "Feature key %s is no longer legal", key);
  } else if (StringICmp (key, "polyA_site") == 0) {
    if (SeqLocStart (sfp->location) != SeqLocStop (sfp->location)) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PolyAsiteNotPoint, "PolyA_site should be a single point");
    }
  } else if (StringICmp (key, "mat_peptide") == 0 || StringICmp (key, "sig_peptide") == 0 || StringICmp (key, "transit_peptide") == 0) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_InvalidForType, "Peptide processing feature should be converted to the appropriate protein feature subtype");
    CheckPeptideOnCodonBoundary (vsp, gcp, sfp, key);
  } else if (StringICmp (key, "mRNA") == 0 ||
             StringICmp (key, "tRNA") == 0 ||
             StringICmp (key, "rRNA") == 0 ||
             StringICmp (key, "snRNA") == 0 ||
             StringICmp (key, "scRNA") == 0 || StringICmp (key, "snoRNA") == 0 || StringICmp (key, "misc_RNA") == 0 || StringICmp (key, "precursor_RNA") == 0) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidForType,
              "RNA feature should be converted to the appropriate RNA feature subtype, location should be converted manually");
  } else if (StringICmp (key, "CDS") == 0) {
    failed = TRUE;              /* impfeat CDS must be pseudo; fail if not */
    if (sfp->pseudo) {
      failed = FALSE;
    } else {
      grp = SeqMgrGetGeneXref (sfp);
      if (grp != NULL && grp->pseudo) {
        failed = FALSE;
      } else {
        gene = SeqMgrGetOverlappingGene (sfp->location, &gcontext);
        if (gene != NULL) {
          if (gene->pseudo) {
            failed = FALSE;
          } else {
            grp = (GeneRefPtr) gene->data.value.ptrvalue;
            if (grp != NULL && grp->pseudo) {
              failed = FALSE;
            }
          }
        }
      }
    }
    for (gbqual = sfp->qual; gbqual != NULL; gbqual = gbqual->next) {
      if (StringCmp (gbqual->qual, "translation") == 0) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_ImpCDShasTranslation, "ImpFeat CDS with /translation found");
      }
    }
    if (failed) {
      ValidErr (vsp, SEV_INFO, ERR_SEQ_FEAT_ImpCDSnotPseudo, "ImpFeat CDS should be pseudo");
    }
  }
  for (gbqual = sfp->qual; gbqual != NULL; gbqual = gbqual->next) {
    if (StringCmp (gbqual->qual, "gsdb_id") == 0) {
      continue;
    }
    val = GBQualNameValid (gbqual->qual);
    if (val == -1) {
      if (gbqual->qual != NULL) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnknownImpFeatQual, "Unknown qualifier %s", gbqual->qual);
      } else {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnknownImpFeatQual, "NULL qualifier");
      }
    } else if (index != -1) {
      found = FALSE;
      for (i = 0; i < ParFlat_GBFeat[index].opt_num; i++) {
        qual = ParFlat_GBFeat[index].opt_qual[i];
        if (qual == val) {
          found = TRUE;
          break;
        }
      }
      if (!found) {
        for (i = 0; i < ParFlat_GBFeat[index].mand_num; i++) {
          qual = ParFlat_GBFeat[index].mand_qual[i];
          if (qual == val) {
            found = TRUE;
            break;
          }
        }
        if (!found) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_WrongQualOnImpFeat, "Wrong qualifier %s for feature %s", gbqual->qual, key);
        }
      }
      if (gbqual->val != NULL) {
        if (val == GBQUAL_rpt_type) {
          failed = FALSE;
          tmp = StringSave (gbqual->val);
          str = tmp;
          if (*str == '(') {
            str++;
          }
          while (!StringHasNoText (str)) {
            ptr = StringChr (str, ',');
            if (ptr == NULL) {
              ptr = StringChr (str, ')');
            }
            if (ptr != NULL) {
              *ptr = '\0';
              ptr++;
            }
            found = FALSE;
            for (i = 0; legal_repeat_types[i] != NULL; i++) {
              if (StringICmp (str, legal_repeat_types[i]) == 0) {
                found = TRUE;
                break;
              }
            }
            if (!found) {
              failed = TRUE;
            }
            str = ptr;
          }
          MemFree (tmp);
          if (failed) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "%s is not legal a legal value for qualifier %s", gbqual->val, gbqual->qual);
          }
        } else if (val == GBQUAL_rpt_unit) {
          found = FALSE;
          multi_rpt_unit = TRUE;
          for (ptr = gbqual->val, ch = *ptr; ch != '\0'; ptr++, ch = *ptr) {
            if (ch <= ' ') {
              found = TRUE;
            } else if (ch == '(' || ch == ')' || ch == ',' || ch == '.' || IS_DIGIT (ch)) {
            } else {
              multi_rpt_unit = FALSE;
            }
          }
          if (found) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "Illegal value for qualifier %s", gbqual->qual);
          } else if ((!multi_rpt_unit) && StringLen (gbqual->val) > 48) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "Illegal value for qualifier %s", gbqual->qual);
          }
        } else if (val == GBQUAL_label) {
          no_white_space = TRUE;
          only_digits = TRUE;
          for (ptr = gbqual->val, ch = *ptr; ch != '\0'; ptr++, ch = *ptr) {
            if (IS_WHITESP (ch)) {
              no_white_space = FALSE;
            }
            if (! IS_DIGIT (ch)) {
              only_digits = FALSE;
            }
          }
          if (only_digits || (! no_white_space)) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "Illegal value for qualifier %s", gbqual->qual);
          }
        }
      }
    }
  }
  if (index != -1 && ParFlat_GBFeat[index].mand_num > 0) {
    for (i = 0; i < ParFlat_GBFeat[index].mand_num; i++) {
      found = FALSE;
      qual = ParFlat_GBFeat[index].mand_qual[i];
      for (gbqual = sfp->qual; gbqual != NULL; gbqual = gbqual->next) {
        val = GBQualNameValid (gbqual->qual);
        if (qual == val) {
          found = TRUE;
          break;
        }
      }
      if (!found) {
        if (qual == GBQUAL_citation && sfp->cit != NULL) {
          found = TRUE;
        }
      }
      if (!found) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_MissingQualOnImpFeat, "Missing qualifier %s for feature %s", ParFlat_GBQual_names[qual].name, key);
      }
    }
  }
  if (!StringHasNoText (ifp->loc)) {
    slp = sfp->location;
    if (StringStr (ifp->loc, "one-of") != NULL) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_ImpFeatBadLoc, "ImpFeat loc %s has obsolete 'one-of' text for feature %s", ifp->loc, key);
    } else if (slp != NULL && slp->choice == SEQLOC_INT) {
      sint = (SeqIntPtr) slp->data.ptrvalue;
      if (sint != NULL && sint->strand != Seq_strand_minus) {
        sprintf (range, "%ld..%ld", (long) (sint->from + 1), (long) (sint->to + 1));
        if (StringCmp (ifp->loc, range) != 0) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_ImpFeatBadLoc, "ImpFeat loc %s does not equal feature location %s for feature %s", ifp->loc, range, key);
        }
      }
    }
  }
  MemFree (key);
}

/* PartialAtSpliceSite uses code taken from SpliceCheckEx */
static Boolean PartialAtSpliceSite (SeqLocPtr head, Uint2 slpTag)
{
  BioseqPtr       bsp;
  Int2            residue1, residue2;
  Boolean         rsult = FALSE;
  SeqIdPtr        sip;
  SeqLocPtr       slp = NULL, first = NULL, last = NULL;
  SeqPortPtr      spp = NULL;
  Uint1           strand;
  Int4            strt, stp, donor, acceptor, len;

  if (slpTag != SLP_NOSTART && slpTag != SLP_NOSTOP)
    return FALSE;
  while ((slp = SeqLocFindPart (head, slp, EQUIV_IS_ONE)) != NULL) {
    if (first == NULL) {
      first = slp;
    }
    last = slp;
  }
  if (first == NULL)
    return FALSE;

  strand = SeqLocStrand (first);
  if (SeqLocStrand (last) != strand)
    return FALSE;

  if (slpTag == SLP_NOSTART) {
    slp = first;
  } else {
    slp = last;
  }
  sip = SeqLocId (slp);
  if (sip == NULL)
    return FALSE;
  acceptor = SeqLocStart (slp);
  donor = SeqLocStop (slp);
  bsp = BioseqLockById (sip);
  if (bsp == NULL)
    return FALSE;
  len = bsp->length;
  spp = SeqPortNew (bsp, 0, -1, strand, Seq_code_ncbi4na);
  BioseqUnlock (bsp);
  if (spp == NULL)
    return FALSE;

  if (strand != Seq_strand_minus) {
    strt = acceptor;
    stp = donor;
  } else {
    strt = donor;
    donor = acceptor;
    acceptor = strt;
    stp = len - donor - 1;
    strt = len - acceptor - 1;
  }

  if (slpTag == SLP_NOSTOP && stp < len - 2) {
    SeqPortSeek (spp, (stp + 1), SEEK_SET);
    residue1 = SeqPortGetResidue (spp);
    residue2 = SeqPortGetResidue (spp);
    if (IS_residue (residue1) && IS_residue (residue2)) {
      if ((residue1 & 4) && (residue2 & 8)) {
        rsult = TRUE;
      } else if ((residue1 & 4) && (residue2 & 2)) {
        rsult = TRUE;
      }
    }
  } else if (slpTag == SLP_NOSTART && strt > 1) {
    SeqPortSeek (spp, (strt - 2), SEEK_SET);
    residue1 = SeqPortGetResidue (spp);
    residue2 = SeqPortGetResidue (spp);
    if (IS_residue (residue1) && IS_residue (residue2)) {
      if ((residue1 & 1) && (residue2 & 4)) {
        rsult = TRUE;
      }
    }
  }

  spp = SeqPortFree (spp);
  return rsult;
}

static void CheckTrnaCodons (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp, tRNAPtr trp)
{
  Uint1           aa;
  BioseqPtr       bsp;
  Int2            code;
  CharPtr         codes = NULL;
  Uint1           from;
  GeneticCodePtr  gncp;
  Int2            j;
  SeqEntryPtr     sep;
  ErrSev          sev = SEV_ERROR;
  SeqMapTablePtr  smtp;
  Uint1           taa;
  ValNodePtr      vnp;

  if (vsp == NULL || gcp == NULL || sfp == NULL || trp == NULL)
    return;
  for (j = 0; j < 6; j++) {
    if (trp->codon[j] < 64) {
      if (codes == NULL) {
        bsp = GetBioseqGivenSeqLoc (sfp->location, gcp->entityID);
        sep = GetBestTopParentForData (gcp->entityID, bsp);
        code = SeqEntryToGeneticCode (sep, NULL, NULL, 0);
        gncp = GeneticCodeFind (code, NULL);
        if (gncp == NULL) {
          gncp = GeneticCodeFind (1, NULL);
        }
        if (gncp == NULL)
          return;
        for (vnp = (ValNodePtr) gncp->data.ptrvalue; vnp != NULL; vnp = vnp->next) {
          if (vnp->choice == 3) {
            codes = (CharPtr) vnp->data.ptrvalue;
          }
        }
      }
      if (codes == NULL)
        return;
      taa = codes[trp->codon[j]];
      aa = 0;
      if (trp->aatype == 2) {
        aa = trp->aa;
      } else {
        from = 0;
        switch (trp->aatype) {
        case 0:
          from = 0;
          break;
        case 1:
          from = Seq_code_iupacaa;
          break;
        case 2:
          from = Seq_code_ncbieaa;
          break;
        case 3:
          from = Seq_code_ncbi8aa;
          break;
        case 4:
          from = Seq_code_ncbistdaa;
          break;
        default:
          break;
        }
        smtp = SeqMapTableFind (Seq_code_ncbieaa, from);
        if (smtp != NULL) {
          aa = SeqMapTableConvert (smtp, trp->aa);
        }
      }
      if (aa > 0 && aa != 255) {
        if (taa != aa) {
          if (aa == 'U') {
            sev = SEV_WARNING;
          }
          ValidErr (vsp, sev, ERR_SEQ_FEAT_TrnaCodonWrong, "tRNA codon does not match genetic code");
        }
      }
    }
  }
}

static BioseqSetPtr GetParentNPS (BioseqPtr bsp)
{
  BioseqSetPtr    bssp;

  if (bsp == NULL)
    return NULL;
  if (bsp->idx.parenttype != OBJ_BIOSEQSET)
    return NULL;
  bssp = (BioseqSetPtr) bsp->idx.parentptr;
  while (bssp != NULL && bssp->_class != BioseqseqSet_class_nuc_prot && bssp->idx.parenttype == OBJ_BIOSEQSET) {
    bssp = (BioseqSetPtr) bssp->idx.parentptr;
  }
  if (bssp->_class == BioseqseqSet_class_nuc_prot)
    return bssp;
  return NULL;
}

static Boolean NucAndProtNotInNPS (BioseqPtr nuc, BioseqPtr prot)
{
  BioseqSetPtr    bssp;

  if (nuc == NULL || prot == NULL)
    return FALSE;
  bssp = GetParentNPS (nuc);
  if (bssp == NULL)
    return TRUE;
  if (GetParentNPS (prot) != bssp)
    return TRUE;
  return FALSE;
}

static void CheckForCommonCDSProduct (ValidStructPtr vsp, SeqFeatPtr sfp)
{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  SeqFeatPtr      cds;
  CdRegionPtr     crp;
  SeqFeatPtr      gene;
  GeneRefPtr      grp;
  Boolean         is_nc = FALSE;
  Boolean         is_nc_gps = FALSE;
  Boolean         is_nt = FALSE;
  BioseqPtr       nuc;
  SeqEntryPtr     sep;
  SeqIdPtr        sip;
  TextSeqIdPtr    tsip;

  if (sfp == NULL || sfp->pseudo)
    return;
  if (!vsp->useSeqMgrIndexes)
    return;
  crp = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (crp != NULL && crp->orf)
    return;
  grp = SeqMgrGetGeneXref (sfp);
  if (grp == NULL || (!SeqMgrGeneIsSuppressed (grp))) {
    gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
    if (gene == NULL || gene->pseudo)
      return;
    grp = (GeneRefPtr) gene->data.value.ptrvalue;
    if (grp != NULL && grp->pseudo)
      return;
  }
  if (sfp->product == NULL)
    return;
  bsp = BioseqFindFromSeqLoc (sfp->product);
  if (bsp == NULL) {
    sip = SeqLocId (sfp->product);
    /* okay to have far RefSeq product... */
    if (sip == NULL || sip->choice != SEQID_OTHER) {
      sep = vsp->sep;
      if (sep != NULL && IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        /* but only if genomic product set */
        if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set)
          return;
      }
      /* or just a bioseq */
      if (sep != NULL && IS_Bioseq (sep))
        return;
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_MultipleCDSproducts, "Unable to find product Bioseq from CDS feature");
    }
    return;
  }
  nuc = BioseqFindFromSeqLoc (sfp->location);
  if (nuc != NULL) {
    for (sip = nuc->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_OTHER) {
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL && tsip->accession != NULL) {
          if (StringNICmp (tsip->accession, "NT_", 3) == 0) {
            is_nt = TRUE;
          } else if (StringNICmp (tsip->accession, "NC_", 3) == 0) {
            is_nc = TRUE;
          }
        }
      }
    }
    if (is_nc && nuc->idx.parenttype == OBJ_BIOSEQSET) {
      bssp = (BioseqSetPtr) nuc->idx.parentptr;
      if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set) {
        is_nc_gps = TRUE;
      }
    }
    if (NucAndProtNotInNPS (nuc, bsp) && (! is_nt) && (! is_nc_gps)) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_CDSproductPackagingProblem, "Protein product not packaged in nuc-prot set with nucleotide");
    }
  }
  cds = SeqMgrGetCDSgivenProduct (bsp, NULL);
  if (cds == NULL)
    return;
  if (cds != sfp) {
    /* if genomic product set, with one cds on contig and one on cdna, do not report */
    sep = vsp->sep;
    if (sep != NULL && IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set) {
        /* feature packaging test will do final contig vs. cdna check */
        if (BioseqFindFromSeqLoc (cds->location) != BioseqFindFromSeqLoc (sfp->location))
          return;
      }
    }

    ValidErr (vsp, SEV_REJECT, ERR_SEQ_FEAT_MultipleCDSproducts, "Same product Bioseq from multiple CDS features");
  }
}

static void CheckForCommonMRNAProduct (ValidStructPtr vsp, SeqFeatPtr sfp)
{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  SeqFeatPtr      gene;
  GeneRefPtr      grp;
  SeqFeatPtr      mrna;
  SeqEntryPtr     oldscope;
  SeqEntryPtr     sep;
  SeqIdPtr        sip;

  if (sfp == NULL || sfp->pseudo)
    return;
  if (!vsp->useSeqMgrIndexes)
    return;
  grp = SeqMgrGetGeneXref (sfp);
  if (grp == NULL || (!SeqMgrGeneIsSuppressed (grp))) {
    gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
    if (gene == NULL || gene->pseudo)
      return;
    grp = (GeneRefPtr) gene->data.value.ptrvalue;
    if (grp != NULL && grp->pseudo)
      return;
  }
  if (sfp->product == NULL)
    return;

  oldscope = SeqEntrySetScope (vsp->sep);
  bsp = BioseqFindFromSeqLoc (sfp->product);
  SeqEntrySetScope (oldscope);
  if (bsp == NULL) {
    sip = SeqLocId (sfp->product);
    if (sip != NULL && sip->choice == SEQID_LOCAL) {
      sep = vsp->sep;
      if (sep != NULL && IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL) {
          if (bssp->_class == BioseqseqSet_class_gen_prod_set ||
              bssp->_class == BioseqseqSet_class_other) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MissingMRNAproduct,
            "Product Bioseq of mRNA feature is not packaged in the record");
          }
        }
      }
    }
    return;
  }

  mrna = SeqMgrGetRNAgivenProduct (bsp, NULL);
  if (mrna == NULL)
    return;
  if (mrna != sfp) {
    ValidErr (vsp, SEV_REJECT, ERR_SEQ_FEAT_MultipleMRNAproducts, "Same product Bioseq from multiple mRNA features");
  }
}

static void CheckForBadGeneOverlap (ValidStructPtr vsp, SeqFeatPtr sfp)
{
  SeqMgrFeatContext fcontext;
  SeqFeatPtr      gene;
  GeneRefPtr      grp;
  ErrSev          sev = SEV_ERROR;

  if (sfp == NULL)
    return;
  grp = SeqMgrGetGeneXref (sfp);
  if (grp != NULL)
    return;
  gene = SeqMgrGetOverlappingGene (sfp->location, &fcontext);
  if (gene != NULL)
    return;
  gene = SeqMgrGetOverlappingFeature (sfp->location, FEATDEF_GENE, NULL, 0, NULL, SIMPLE_OVERLAP, &fcontext);
  if (gene == NULL)
    return;
  if (IsNC (vsp->sep, sfp->location)) {
    sev = SEV_WARNING;
  }
  if (sfp->data.choice == SEQFEAT_CDREGION) {
    ValidErr (vsp, sev, ERR_SEQ_FEAT_CDSgeneRange, "gene overlaps CDS but does not completely contain it");
  } else if (sfp->data.choice == SEQFEAT_RNA) {
    ValidErr (vsp, sev, ERR_SEQ_FEAT_mRNAgeneRange, "gene overlaps mRNA but does not completely contain it");
  }
}

static void CheckForBadMRNAOverlap (ValidStructPtr vsp, SeqFeatPtr sfp)
{
  SeqMgrFeatContext fcontext;
  SeqFeatPtr      mrna;
  ErrSev          sev = SEV_ERROR;

  if (sfp == NULL)
    return;
  mrna = SeqMgrGetOverlappingFeature (sfp->location, FEATDEF_mRNA, NULL, 0, NULL, SIMPLE_OVERLAP, &fcontext);
  if (mrna == NULL)
    return;
  mrna = SeqMgrGetOverlappingFeature (sfp->location, FEATDEF_mRNA, NULL, 0, NULL, CHECK_INTERVALS, &fcontext);
  if (mrna != NULL)
    return;
  mrna = SeqMgrGetOverlappingFeature (sfp->location, FEATDEF_mRNA, NULL, 0, NULL, INTERVAL_OVERLAP, &fcontext);
  if (mrna == NULL)
    return;
  if (IsNC (vsp->sep, sfp->location)) {
    sev = SEV_WARNING;
  }
  if (sfp->excpt) {
    sev = SEV_WARNING;
  }
  ValidErr (vsp, sev, ERR_SEQ_FEAT_CDSmRNArange, "mRNA overlaps or contains CDS but does not completely contain intervals");
}

static void CheckForBothStrands (ValidStructPtr vsp, SeqFeatPtr sfp)
{
  Boolean         bothstrands = FALSE;
  SeqLocPtr       location, slp = NULL;

  if (sfp == NULL)
    return;
  location = sfp->location;
  if (location == NULL)
    return;
  while ((slp = SeqLocFindNext (location, slp)) != NULL) {
    if (SeqLocStrand (slp) == Seq_strand_both) {
      bothstrands = TRUE;
    }
  }
  if (bothstrands) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_BothStrands, "mRNA or CDS may not be on both strands");
  }
}

static Boolean OverlappingGeneIsPseudo (SeqFeatPtr sfp)
{
  SeqFeatPtr      gene;
  GeneRefPtr      grp;

  if (sfp == NULL)
    return FALSE;
  grp = SeqMgrGetGeneXref (sfp);
  if (grp != NULL) {
    if (grp->pseudo)
      return TRUE;
    return FALSE;
  }
  gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
  if (gene != NULL) {
    if (gene->pseudo)
      return TRUE;
    grp = (GeneRefPtr) gene->data.value.ptrvalue;
    if (grp != NULL) {
      if (grp->pseudo)
        return TRUE;
    }
  }
  return FALSE;
}

static CharPtr legalDbXrefOnRefSeq [] = {
  "WormBase",
  NULL
};

static void CheckForIllegalDbxref (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp, ValNodePtr dbxref)

{
  DbtagPtr    db;
  Int2        i;
  Int4        id;
  ValNodePtr  vnp;

  for (vnp = dbxref; vnp != NULL; vnp = vnp->next) {
    id = -1;
    db = vnp->data.ptrvalue;
    if (db != NULL && db->db != NULL) {
      for (i = 0; i < DBNUM; i++) {
        if (StringCmp (db->db, dbtag[i]) == 0) {
          id = i;
          break;
        }
      }
      if (id == -1 && GPSorRefSeq (vsp->sep, sfp->location)) {
        for (i = 0; legalDbXrefOnRefSeq [i] != NULL; i++) {
          if (StringCmp (db->db, legalDbXrefOnRefSeq [i]) == 0) return;
        }
      }
      if (id == -1 || (sfp->data.choice != SEQFEAT_CDREGION && id < 4)) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_IllegalDbXref, "Illegal db_xref type %s", db->db);
      }
    }
  }
}

NLM_EXTERN void ValidateSeqFeat (GatherContextPtr gcp)
{
  Int2            type, i, j;
  static char    *parterr[2] = { "PartialProduct", "PartialLocation" };
  static char    *parterrs[4] = {
    "Start does not include first/last residue of sequence",
    "Stop does not include first/last residue of sequence",
    "Internal partial intervals do not include first/last residue of sequence",
    "Improper use of partial (greater than or less than)"
  };
  Uint2           partials[2], errtype;
  Char            buf[80];
  CharPtr         tmp;
  ValidStructPtr  vsp;
  SeqFeatPtr      sfp;
  CdRegionPtr     crp;
  CodeBreakPtr    cbp;
  RnaRefPtr       rrp;
  tRNAPtr         trp;
  GBQualPtr       gbq;
  Boolean         pseudo, excpt, conflict, codonqual, anticodonqual;
  ImpFeatPtr      ifp;
  GeneRefPtr      grp;
  ProtRefPtr      prp;
  ValNodePtr      vnp;
  BioseqPtr       bsp;
  BioseqContextPtr bcp = NULL;
  BioSourcePtr    biop, dbiop;
  OrgNamePtr      onp;
  OrgRefPtr       orp, dorp;
  Int2            biopgencode;
  Int2            cdsgencode;
  GeneticCodePtr  gc;
  PubdescPtr      pdp;
  DbtagPtr        db = NULL;
  Int4            id = -1;
  SeqMgrDescContext context;
  GeneRefPtr      grpx;
  SeqFeatPtr      sfpx;
  Boolean         redundantgenexref;
  SeqMgrFeatContext fcontext;
  CharPtr         syn1, syn2, label = NULL;
  Uint2           oldEntityID, oldItemID;

  vsp = (ValidStructPtr) (gcp->userdata);
  sfp = (SeqFeatPtr) (gcp->thisitem);
  vsp->descr = NULL;
  vsp->sfp = sfp;
  type = (Int2) (sfp->data.choice);

  ValidateSeqLoc (vsp, sfp->location, "Location");

  ValidateSeqLoc (vsp, sfp->product, "Product");

  partials[0] = SeqLocPartialCheck (sfp->product);
  partials[1] = SeqLocPartialCheck (sfp->location);
  if ((partials[0] != SLP_COMPLETE) || (partials[1] != SLP_COMPLETE) || (sfp->partial)) {       /* partialness */
    /* a feature on a partial sequence should be partial -- if often isn't */
    if ((!sfp->partial) && (partials[1] != SLP_COMPLETE) && (sfp->location->choice == SEQLOC_WHOLE)) {
      ValidErr (vsp, SEV_INFO, ERR_SEQ_FEAT_PartialProblem, "On partial Bioseq, SeqFeat.partial should be TRUE");
    }
    /* a partial feature, with complete location, but partial product */
    else if ((sfp->partial) && (sfp->product != NULL) && (partials[1] == SLP_COMPLETE) && (sfp->product->choice == SEQLOC_WHOLE)
             && (partials[0] != SLP_COMPLETE)) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PartialProblem, "When SeqFeat.product is a partial Bioseq, SeqFeat.location should also be partial");
    }
    /* gene on segmented set is now 'order', should also be partial */
    else if (type == SEQFEAT_GENE && sfp->product == NULL && partials[1] == SLP_INTERNAL) {
      if (!sfp->partial) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PartialProblem, "Gene of 'order' with otherwise complete location should have partial flag set");
      }
    }
    /* inconsistent combination of partial/complete product,location,partial flag */
    else if (((partials[0] == SLP_COMPLETE) && (sfp->product != NULL)) || (partials[1] == SLP_COMPLETE) || (!sfp->partial)) {
      tmp = StringMove (buf, "Inconsistent: ");
      if (sfp->product != NULL) {
        tmp = StringMove (tmp, "Product= ");
        if (partials[0])
          tmp = StringMove (tmp, "partial, ");
        else
          tmp = StringMove (tmp, "complete, ");
      }
      tmp = StringMove (tmp, "Location= ");
      if (partials[1])
        tmp = StringMove (tmp, "partial, ");
      else
        tmp = StringMove (tmp, "complete, ");
      tmp = StringMove (tmp, "Feature.partial= ");
      if (sfp->partial)
        tmp = StringMove (tmp, "TRUE");
      else
        tmp = StringMove (tmp, "FALSE");
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PartialProblem, buf);
    }

    /* may have other error bits set as well */
    for (i = 0; i < 2; i++) {
      errtype = SLP_NOSTART;
      for (j = 0; j < 4; j++) {
        if (partials[i] & errtype) {
          if (i == 1 && j < 2 && PartialAtSpliceSite (sfp->location, errtype)) {
            ValidErr (vsp, SEV_INFO, ERR_SEQ_FEAT_PartialProblem, "%s: %s (but is at consensus splice site)", parterr[i], parterrs[j]);
          } else {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PartialProblem, "%s: %s", parterr[i], parterrs[j]);
          }
        }
        errtype <<= 1;
      }
    }

  }

  CheckForIllegalDbxref (vsp, gcp, sfp, sfp->dbxref);
  /*
  for (vnp = sfp->dbxref; vnp != NULL; vnp = vnp->next) {
    id = -1;
    db = vnp->data.ptrvalue;
    if (db && db->db) {
      for (i = 0; i < DBNUM; i++) {
        if (StringCmp (db->db, dbtag[i]) == 0) {
          id = i;
          break;
        }
      }
      if (id == -1 || (type != SEQFEAT_CDREGION && id < 4)) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_IllegalDbXref, "Illegal db_xref type %s", db->db);
      }
    }
  }
  */

  switch (type) {
  case 1:                      /* Gene-ref */
    grp = (GeneRefPtr) (sfp->data.value.ptrvalue);
    if (grp != NULL) {
      if (EmptyOrNullString (grp->locus) &&
          EmptyOrNullString (grp->allele) && EmptyOrNullString (grp->desc) && EmptyOrNullString (grp->maploc) && grp->db == NULL && grp->syn == NULL) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_GeneRefHasNoData, "There is a gene feature where all fields are empty");
      }
      CheckForIllegalDbxref (vsp, gcp, sfp, grp->db);
      /*
      for (vnp = grp->db; vnp != NULL; vnp = vnp->next) {
        id = -1;
        db = vnp->data.ptrvalue;
        if (db && db->db) {
          for (i = 0; i < DBNUM; i++) {
            if (StringCmp (db->db, dbtag[i]) == 0) {
              id = i;
              break;
            }
          }
          if (id == -1 || (type != SEQFEAT_CDREGION && id < 4)) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_IllegalDbXref, "Illegal db_xref type %s", db->db);
          }
        }
      }
      */
    }
    break;
  case 2:                      /* Org-ref */
    break;
  case 3:                      /* Cdregion */
    pseudo = sfp->pseudo;       /* now also uses new feature pseudo flag */
    excpt = FALSE;
    conflict = FALSE;
    codonqual = FALSE;
    crp = (CdRegionPtr) (sfp->data.value.ptrvalue);
    if (crp != NULL) {
      conflict = crp->conflict;
    }
    gbq = sfp->qual;
    while (gbq != NULL) {
      if (StringICmp (gbq->qual, "pseudo") == 0) {
        pseudo = TRUE;
      }
      if (StringICmp (gbq->qual, "exception") == 0) {
        excpt = TRUE;
      }
      if (StringICmp (gbq->qual, "codon") == 0) {
        codonqual = TRUE;
      }
      gbq = gbq->next;
    }
    if (OverlappingGeneIsPseudo (sfp)) {
      pseudo = TRUE;
    }
    if ((!pseudo) && (!conflict)) {
      CdTransCheck (vsp, sfp);
      if (sfp->excpt && (StringICmp (sfp->except_text, "ribosomal slippage") == 0 ||
                         StringICmp (sfp->except_text, "ribosome slippage") == 0 || StringICmp (sfp->except_text, "artificial frameshift") == 0)) {
        /* suppress splice check */
      } else {
        SpliceCheck (vsp, sfp);
      }
    }
    crp = (CdRegionPtr) (sfp->data.value.ptrvalue);
    if (crp != NULL) {
      for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
        i = SeqLocCompare (cbp->loc, sfp->location);
        if ((i != SLC_A_IN_B) && (i != SLC_A_EQ_B))
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_Range, "Code-break location not in coding region");
      }
      if (excpt && (!sfp->excpt)) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_ExceptInconsistent, "Exception flag should be set in coding region");
      }
      if (crp->orf && sfp->product != NULL) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_OrfCdsHasProduct, "An ORF coding region should not have a product");
      }
      if (pseudo && sfp->product != NULL) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PsuedoCdsHasProduct, "A pseudo coding region should not have a product");
      }
      if (codonqual) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_WrongQualOnImpFeat, "Use the proper genetic code, if available, or set transl_excepts on specific codons");
      }
      biopgencode = 0;
      cdsgencode = 0;
      bsp = GetBioseqGivenSeqLoc (sfp->location, gcp->entityID);
      if (bsp != NULL) {
        vnp = NULL;
        if (vsp->useSeqMgrIndexes) {
          vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &context);
        } else {
          bcp = BioseqContextNew (bsp);
          vnp = BioseqContextGetSeqDescr (bcp, Seq_descr_source, NULL, NULL);
        }
        if (vnp != NULL && vnp->data.ptrvalue != NULL) {
          biop = (BioSourcePtr) vnp->data.ptrvalue;
          orp = biop->org;
          if (orp != NULL && orp->orgname != NULL) {
            onp = orp->orgname;
            if (biop->genome == 4 || biop->genome == 5) {
              biopgencode = onp->mgcode;
            } else if (biop->genome == GENOME_chloroplast ||
                       biop->genome == GENOME_chromoplast ||
                       biop->genome == GENOME_plastid ||
                       biop->genome == GENOME_cyanelle ||
                       biop->genome == GENOME_apicoplast || biop->genome == GENOME_leucoplast || biop->genome == GENOME_proplastid) {
              biopgencode = 11;
            } else {
              biopgencode = onp->gcode;
            }
            gc = crp->genetic_code;
            if (gc != NULL) {
              for (vnp = gc->data.ptrvalue; vnp != NULL; vnp = vnp->next) {
                if (vnp->choice == 2) {
                  cdsgencode = (Int2) vnp->data.intvalue;
                }
              }
            }
            if (biopgencode != cdsgencode) {
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_GenCodeMismatch,
                        "Genetic code conflict between CDS (code %d) and BioSource (code %d)", (int) cdsgencode, (int) biopgencode);
            }
          }
        }
        if (!vsp->useSeqMgrIndexes) {
          BioseqContextFree (bcp);
        }
      }
    }
    CheckForBothStrands (vsp, sfp);
    CheckForBadGeneOverlap (vsp, sfp);
    CheckForBadMRNAOverlap (vsp, sfp);
    CheckForCommonCDSProduct (vsp, sfp);
    break;
  case 4:                      /* Prot-ref */
    prp = (ProtRefPtr) (sfp->data.value.ptrvalue);
    if (prp != NULL) {
      if (prp->processed != 3 && prp->processed != 4) {
        vnp = prp->name;
        if ((vnp == NULL || EmptyOrNullString ((CharPtr) vnp->data.ptrvalue)) &&
            EmptyOrNullString (prp->desc) && prp->ec == NULL && prp->activity == NULL && prp->db == NULL) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_ProtRefHasNoData, "There is a protein feature where all fields are empty");
        }
      }
      CheckForIllegalDbxref (vsp, gcp, sfp, prp->db);
      /*
      for (vnp = prp->db; vnp != NULL; vnp = vnp->next) {
        id = -1;
        db = vnp->data.ptrvalue;
        if (db && db->db) {
          for (i = 0; i < DBNUM; i++) {
            if (StringCmp (db->db, dbtag[i]) == 0) {
              id = i;
              break;
            }
          }
          if (id == -1 || (type != SEQFEAT_CDREGION && id < 4)) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_IllegalDbXref, "Illegal db_xref type %s", db->db);
          }
        }
      }
      */
    }
    break;
  case 5:                      /* RNA-ref */
    rrp = (RnaRefPtr) (sfp->data.value.ptrvalue);
    if (rrp->type == 2) {       /* mRNA */
      pseudo = sfp->pseudo;
      if (OverlappingGeneIsPseudo (sfp)) {
        pseudo = TRUE;
      }
      if (!pseudo) {
        MrnaTransCheck (vsp, sfp);      /* transcription check */
        if (sfp->excpt && (StringICmp (sfp->except_text, "artificial frameshift") == 0)) {
          /* suppress splice check */
        } else {
          SpliceCheck (vsp, sfp);
        }
      }
      CheckForBothStrands (vsp, sfp);
      CheckForBadGeneOverlap (vsp, sfp);
      CheckForCommonMRNAProduct (vsp, sfp);
    }
    if (rrp->ext.choice == 2) { /* tRNA */
      trp = (tRNAPtr) (rrp->ext.value.ptrvalue);
      if (trp->anticodon != NULL) {
        i = SeqLocCompare (trp->anticodon, sfp->location);
        if ((i != SLC_A_IN_B) && (i != SLC_A_EQ_B)) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_Range, "Anticodon location not in tRNA");
        }
        if (SeqLocLen (trp->anticodon) != 3) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_Range, "Anticodon is not 3 bases in length");
        }
      }
      CheckTrnaCodons (vsp, gcp, sfp, trp);
    }
    if (rrp->type == 3) {       /* tRNA */
      anticodonqual = FALSE;
      gbq = sfp->qual;
      while (gbq != NULL) {
        if (StringICmp (gbq->qual, "anticodon") == 0) {
          anticodonqual = TRUE;
        }
        gbq = gbq->next;
      }
      if (anticodonqual) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "Unparsed anticodon qualifier in tRNA");
      }
    }
    if (rrp->type == 3 && rrp->ext.choice == 1) { /* tRNA with string extension */
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "Unparsed product qualifier in tRNA");
    }
    if (rrp->type == 0) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_RNAtype0, "RNA type 0 (unknown) not supported");
    }
    break;
  case 6:                      /* Pub */
    pdp = (PubdescPtr) sfp->data.value.ptrvalue;
    /*
       ValidatePubdesc (vsp, pdp);
     */
    break;
  case 7:                      /* Seq */
    break;
  case 8:                      /* Imp-feat */
    ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
    if (GetAppProperty ("ValidateExons") != NULL) {

      if (ifp != NULL && StringICmp (ifp->key, "exon") == 0) {
        SpliceCheckEx (vsp, sfp, TRUE);
      }
    }
    if (ifp != NULL) {
      ValidateImpFeat (vsp, gcp, sfp, ifp);
    }
    break;
  case 9:                      /* Region */
    break;
  case 10:                     /* Comment */
    break;
  case 11:                     /* Bond */
    break;
  case 12:                     /* Site */
    break;
  case 13:                     /* Rsite-ref */
    break;
  case 14:                     /* User-object */
    break;
  case 15:                     /* TxInit */
    break;
  case 16:                     /* Numbering */
    break;
  case 17:                     /* Secondary Structure */
    break;
  case 18:                     /* NonStdRes */
    break;
  case 19:                     /* Heterogen */
    break;
  case 20:                     /* BioSource */
    biop = (BioSourcePtr) sfp->data.value.ptrvalue;
    if (biop != NULL && biop->is_focus) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_FocusOnBioSourceFeature, "Focus must be on BioSource descriptor, not BioSource feature.");
    }
    if (biop != NULL) {
      orp = biop->org;
      if (orp != NULL) {
        bsp = GetBioseqGivenSeqLoc (sfp->location, gcp->entityID);
        if (bsp != NULL) {
          vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &context);
          if (vnp != NULL) {
            dbiop = (BioSourcePtr) vnp->data.ptrvalue;
            if (dbiop != NULL) {
              dorp = dbiop->org;
              if (dorp != NULL) {
                if (!StringHasNoText (orp->taxname)) {
                  if (StringICmp (orp->taxname, dorp->taxname) != 0) {
                    if (!dbiop->is_focus) {
                      oldEntityID = gcp->entityID;
                      oldItemID = gcp->itemID;

                      gcp->entityID = context.entityID;
                      gcp->itemID = context.itemID;
                      gcp->thistype = OBJ_SEQDESC;

                      ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_BioSourceNeedsFocus,
                                "BioSource descriptor must have focus when BioSource feature with different taxname is present.");

                      gcp->entityID = oldEntityID;
                      gcp->itemID = oldItemID;
                      gcp->thistype = OBJ_SEQFEAT;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    /*
       ValidateBioSource (vsp, gcp, biop);
     */
    break;
  default:
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidType, "Invalid SeqFeat type [%d]", (int) (type));
    break;
  }
  if ((! sfp->excpt) && (! StringHasNoText (sfp->except_text))) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_ExceptInconsistent, "Exception text is set, but exception flag is not set");
  }
  if (type != SEQFEAT_GENE) {
    grp = SeqMgrGetGeneXref (sfp);
    if (grp == NULL || SeqMgrGeneIsSuppressed (grp))
      return;
    sfpx = SeqMgrGetOverlappingGene (sfp->location, &fcontext);
    if (sfpx == NULL || sfpx->data.choice != SEQFEAT_GENE)
      return;
    grpx = (GeneRefPtr) sfpx->data.value.ptrvalue;
    if (grpx == NULL)
      return;
    redundantgenexref = FALSE;
    label = fcontext.label;
    if ((!StringHasNoText (grp->locus)) && (!StringHasNoText (grpx->locus))) {
      if ((StringICmp (grp->locus, grpx->locus) == 0)) {
        redundantgenexref = TRUE;
        label = grp->locus;
      }
    } else if (grp->syn != NULL && grpx->syn != NULL) {
      syn1 = (CharPtr) grp->syn->data.ptrvalue;
      syn2 = (CharPtr) grpx->syn->data.ptrvalue;
      if ((!StringHasNoText (syn1)) && (!StringHasNoText (syn2))) {
        if ((StringICmp (syn1, syn2) == 0)) {
          redundantgenexref = TRUE;
          label = syn1;
        }
      }
    }
    if (redundantgenexref) {
      if (StringHasNoText (label)) {
        label = "?";
      }
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnnecessaryGeneXref, "Unnecessary gene cross-reference %s", label);
    }
  }
  return;
}

/*****************************************************************************
*
*   MrnaTransCheck (sfp, vsp)
*
*****************************************************************************/

NLM_EXTERN void MrnaTransCheck (ValidStructPtr vsp, SeqFeatPtr sfp)
{
  Int4            mismatch, total;
  CharPtr         mrseq, pdseq;
  Int4            mlen, plen;
  CharPtr         ptr1, ptr2;
  SeqFeat         sf;
  SeqIdPtr        sip;
  ValNode         vn;

  if (sfp == NULL)
    return;
  if (sfp->excpt)
    return;
  if (sfp->pseudo)
    return;
  if (sfp->product == NULL)
    return;

  sip = SeqLocId (sfp->product);
  if (sip == NULL)
    return;

  mrseq = GetSequenceByFeature (sfp);
  if (mrseq == NULL)
    return;

  /* coerced feature on whole product for GetSequenceByFeature */

  MemSet ((Pointer) &sf, 0, sizeof (SeqFeat));
  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  sf.location = &vn;
  vn.choice = SEQLOC_WHOLE;
  vn.data.ptrvalue = sip;

  pdseq = GetSequenceByFeature (&sf);
  if (pdseq != NULL) {
    mlen = StringLen (mrseq);
    plen = StringLen (pdseq);
    if (mlen != plen) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_TranscriptLen, "Transcript length [%ld] does not match product length [%ld]", (long) mlen, (long) plen);
    } else if (mlen > 0 && StringICmp (mrseq, pdseq) != 0) {
      mismatch = 0;
      total = 0;
      ptr1 = mrseq;
      ptr2 = pdseq;
      while (total < mlen) {
        if (*ptr1 != *ptr2) {
          mismatch++;
        }
        ptr1++;
        ptr2++;
        total++;
      }
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_TranscriptMismatches,
                "There are %ld mismatches out of %ld bases between the transcript and product sequence", (long) mismatch, (long) total);
    }
    MemFree (pdseq);
  }

  MemFree (mrseq);
}

/*****************************************************************************
*
*   CdTransCheck(sfp)
*   	Treatment of terminal 'X'
*          If either the protein or the translation end in 'X' (usually
*          due to partial last codon) it is ignored to minimize conflicts
*          between approaches to add the X or not in this case.
*
*****************************************************************************/
static CharPtr MapToNTCoords (SeqFeatPtr sfp, SeqIdPtr protID, Int4 pos)
{
  SeqLocPtr       nslp;
  SeqLocPtr       pslp;
  CharPtr         rsult;
  SeqPntPtr       spntp;

  rsult = NULL;
  if (sfp != NULL && protID != NULL && pos >= 0) {
    spntp = SeqPntNew ();
    pslp = ValNodeNew (NULL);
    pslp->choice = SEQLOC_PNT;
    pslp->data.ptrvalue = (Pointer) spntp;
    spntp->point = pos;
    spntp->id = SeqIdDup (protID);
    nslp = aaLoc_to_dnaLoc (sfp, pslp);
    if (nslp != NULL) {
      rsult = SeqLocPrint (nslp);
    }
    SeqLocFree (pslp);
    SeqLocFree (nslp);
  }
  return rsult;
}

static Boolean Loc_is_RefSeq (SeqLocPtr location)
{
  BioseqPtr       bsp;
  SeqIdPtr        sip;
  TextSeqIdPtr    tsip;

  if (location == NULL)
    return FALSE;
  sip = SeqLocId (location);
  if (sip == NULL)
    return FALSE;
  bsp = BioseqFind (sip);
  if (bsp == NULL)
    return FALSE;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER) {
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
      if (tsip != NULL) {
        if (StringNICmp (tsip->accession, "NM_", 3) == 0) {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

NLM_EXTERN void CdTransCheck (ValidStructPtr vsp, SeqFeatPtr sfp)
{
  ByteStorePtr    newprot = NULL;
  BioseqPtr       prot1seq = NULL, prot2seq = NULL;
  Int4            prot1len = 0, prot2len, i, len;
  CdRegionPtr     crp;
  SeqIdPtr        protid = NULL;
  Int2            residue1, residue2, stop_count = 0, mismatch = 0, ragged = 0;
  Boolean         got_stop = FALSE;
  SeqPortPtr      spp = NULL;
  Uint2           part_loc = 0, part_prod = 0;
  Boolean         no_end = FALSE, no_beg = FALSE, show_stop = FALSE, got_dash = FALSE, done;
  GBQualPtr       gb;
  ValNodePtr      vnp, code;
  int             gccode = 0;
  Boolean         transl_except = FALSE, prot_ok = TRUE, is_nc = FALSE;
  CharPtr         nuclocstr;
  CodeBreakPtr    cbp;
  Int4            pos1, pos2, pos;
  SeqLocPtr       tmp;
  ErrSev          sev;
  SeqEntryPtr     sep;


  if (sfp == NULL)
    return;

  if (sfp->excpt && StringICmp (sfp->except_text, "ribosomal slippage") != 0 && StringICmp (sfp->except_text, "ribosome slippage") != 0)        /* biological exception */
    return;

  for (gb = sfp->qual; gb != NULL; gb = gb->next) {     /* pseuogene */
    if (!StringICmp ("pseudo", gb->qual))
      return;
  }

  crp = (CdRegionPtr) (sfp->data.value.ptrvalue);
  if (crp->code_break == NULL) {        /* check for unparsed transl_except */
    for (gb = sfp->qual; gb != NULL; gb = gb->next) {
      if (!StringCmp (gb->qual, "transl_except")) {
        transl_except = TRUE;
        break;
      }
    }
  }

  if (crp->genetic_code != NULL) {
    for (vnp = crp->genetic_code->data.ptrvalue; ((vnp != NULL) && (!gccode)); vnp = vnp->next) {
      switch (vnp->choice) {
      case 0:
        break;
      case 1:                  /* name */
        code = GeneticCodeFind (0, (CharPtr) (vnp->data.ptrvalue));
        if (code != NULL) {
          for (vnp = code->data.ptrvalue; ((vnp != NULL) && (!gccode)); vnp = vnp->next) {
            if (vnp->choice == 2)       /* id */
              gccode = (int) (vnp->data.intvalue);
          }
        }
        break;
      case 2:                  /* id */
        gccode = (int) (vnp->data.intvalue);
        break;
      default:
        gccode = 255;
        break;
      }
    }
  }

  newprot = ProteinFromCdRegionEx (sfp, TRUE, FALSE);   /* include stop codons, do not remove trailing X/B/Z */
  if (newprot == NULL) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_CdTransFail, "Unable to translate");
    prot_ok = FALSE;
    goto erret;
  }

  part_loc = SeqLocPartialCheck (sfp->location);
  part_prod = SeqLocPartialCheck (sfp->product);
  if ((part_loc & SLP_STOP) || (part_prod & SLP_STOP))
    no_end = TRUE;
  else {                        /* complete stop, so check for ragged end */

    len = SeqLocLen (sfp->location);
    if (crp->frame > 1)
      len -= (Int4) (crp->frame - 1);
    ragged = (Int2) (len % (Int4) (3));
    if (ragged) {
      len = SeqLocLen (sfp->location);
      cbp = crp->code_break;
      while (cbp != NULL) {
        pos1 = INT4_MAX;
        pos2 = -10;
        tmp = NULL;
        while ((tmp = SeqLocFindNext (cbp->loc, tmp)) != NULL) {
          pos = GetOffsetInLoc (tmp, sfp->location, SEQLOC_START);
          if (pos < pos1)
            pos1 = pos;
          pos = GetOffsetInLoc (tmp, sfp->location, SEQLOC_STOP);
          if (pos > pos2)
            pos2 = pos;
        }
        pos = pos2 - pos1;      /* codon length */
        if (pos >= 0 && pos <= 1 && pos2 == len - 1)
        {                       /*  a codon */
          /* allowing a partial codon at the end */
          ragged = 0;
        }

        cbp = cbp->next;
      }
    }
  }

  /* check for code break not on a codon */
  len = SeqLocLen (sfp->location);
  cbp = crp->code_break;
  while (cbp != NULL) {
    pos1 = INT4_MAX;
    pos2 = -10;
    tmp = NULL;
    while ((tmp = SeqLocFindNext (cbp->loc, tmp)) != NULL) {
      pos = GetOffsetInLoc (tmp, sfp->location, SEQLOC_START);
      if (pos < pos1)
        pos1 = pos;
      pos = GetOffsetInLoc (tmp, sfp->location, SEQLOC_STOP);
      if (pos > pos2)
        pos2 = pos;
    }
    pos = pos2 - pos1;          /* codon length */
    /* check for code break not on a codon */
    if (pos == 2 || (pos >= 0 && pos <= 1 && pos2 == len - 1)) {
      if (crp->frame == 2)
        pos = 1;
      else if (crp->frame == 3)
        pos = 2;
      else
        pos = 0;
      if ((pos1 % 3) != pos) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_TranslExceptPhase, "transl_except qual out of frame.");
      }
    }


    cbp = cbp->next;
  }

  if (crp->frame > 1) {
    if (!(part_loc & SLP_START)) {
      sev = SEV_WARNING;
      if (Loc_is_RefSeq (sfp->location)) {
        sev = SEV_ERROR;
      }
      ValidErr (vsp, sev, ERR_SEQ_FEAT_PartialProblem, "Suspicious CDS location - frame > 1 but not 5' partial");
    } else if ((part_loc & SLP_NOSTART) && (!PartialAtSpliceSite (sfp->location, SLP_NOSTART))) {
      sev = SEV_INFO;
      if (Loc_is_RefSeq (sfp->location)) {
        sev = SEV_ERROR;
      }
      ValidErr (vsp, sev, ERR_SEQ_FEAT_PartialProblem, "Suspicious CDS location - frame > 1 and not at consensus splice site");
    }
  }

  if ((part_loc & SLP_START) || (part_prod & SLP_START))
    no_beg = TRUE;

  prot2len = BSLen (newprot);
  len = prot2len;
  BSSeek (newprot, 0, SEEK_SET);
  for (i = 0; i < len; i++) {
    residue1 = BSGetByte (newprot);
    if ((i == 0) && (residue1 == '-'))
      got_dash = TRUE;
    if (residue1 == '*') {
      if (i == (len - 1))
        got_stop = TRUE;
      else
        stop_count++;
    }
  }

  if (stop_count) {
    if (got_dash)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_StartCodon,
                "Illegal start codon and %ld internal stops. Probably wrong genetic code [%d]", (long) stop_count, gccode);
    else
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InternalStop, "%ld internal stops. Genetic code [%d]", (long) stop_count, gccode);
    prot_ok = FALSE;
    if (stop_count > 5)
      goto erret;
  } else if (got_dash) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_StartCodon, "Illegal start codon used. Wrong genetic code [%d] or protein should be partial", gccode);
  }

  show_stop = TRUE;

  protid = SeqLocId (sfp->product);
  if (protid != NULL) {
    prot1seq = BioseqFind (protid);
    if (prot1seq != NULL)
      prot1len = prot1seq->length;
  }

  if (prot1seq == NULL) {
    if (prot2len > 6) {
      if (! NGorNT (vsp->sep, sfp->location, &is_nc)) {
        sev = SEV_ERROR;
        if (DeltaOrFarSeg (vsp->sep, sfp->location)) {
          sev = SEV_WARNING;
        }
        if (is_nc) {
          sev = SEV_WARNING;
          sep = vsp->sep;
          if (sep != NULL && IS_Bioseq (sep)) {
            sev = SEV_NONE;
          }
        }
        if (sev != SEV_NONE) {
          ValidErr (vsp, sev, ERR_SEQ_FEAT_NoProtein, "No protein Bioseq given");
        }
      }
    }
    goto erret;
  }

  len = prot2len;

  if ((got_stop) && (len == (prot1len + 1))) {  /* ok, got stop */
    len--;
  }

  spp = SeqPortNew (prot1seq, 0, -1, 0, Seq_code_ncbieaa);
  if (spp == NULL)
    goto erret;

  /* ignore terminal 'X' from partial last codon if present */

  done = FALSE;
  while ((!done) && (prot1len)) {
    SeqPortSeek (spp, (prot1len - 1), SEEK_SET);
    residue1 = SeqPortGetResidue (spp);
    if (residue1 == 'X')        /* remove terminal X */
      prot1len--;
    else
      done = TRUE;
  }
  done = FALSE;
  while ((!done) && (len)) {
    BSSeek (newprot, (len - 1), SEEK_SET);
    residue2 = BSGetByte (newprot);
    if (residue2 == 'X')
      len--;
    else
      done = TRUE;
  }

  if (len == prot1len) {        /* could be identical */
    SeqPortSeek (spp, 0, SEEK_SET);
    BSSeek (newprot, 0, SEEK_SET);
    for (i = 0; i < len; i++) {
      residue1 = BSGetByte (newprot);
      residue2 = SeqPortGetResidue (spp);
      if (residue1 != residue2) {
        prot_ok = FALSE;
        if (residue2 == INVALID_RESIDUE)
          residue2 = '?';
        if (mismatch == 10) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MisMatchAA, "More than 10 mismatches. Genetic code [%d]", gccode);
          break;
        } else if (i == 0) {
          if ((sfp->partial) && (!no_beg) && (!no_end)) /* ok, it's partial */
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "Start of location should probably be partial");
          else if (residue1 == '-')
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_StartCodon, "Illegal start codon used. Wrong genetic code [%d] or protein should be partial", gccode);
          else {
            nuclocstr = MapToNTCoords (sfp, protid, i);
            if (nuclocstr != NULL) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MisMatchAA,
                        "Residue %ld in protein [%c] != translation [%c] at %s", (long) (i + 1), (char) residue2, (char) residue1, nuclocstr);
            } else {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MisMatchAA,
                        "Residue %ld in protein [%c] != translation [%c]", (long) (i + 1), (char) residue2, (char) residue1);
            }
            MemFree (nuclocstr);
          }
        } else {
          nuclocstr = MapToNTCoords (sfp, protid, i);
          if (nuclocstr != NULL) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MisMatchAA,
                      "Residue %ld in protein [%c] != translation [%c] at %s", (long) (i + 1), (char) residue2, (char) residue1, nuclocstr);
          } else {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MisMatchAA,
                      "Residue %ld in protein [%c] != translation [%c]", (long) (i + 1), (char) residue2, (char) residue1);
          }
          MemFree (nuclocstr);
        }
        mismatch++;
      }
    }
    spp = SeqPortFree (spp);
  } else {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_TransLen, "Given protein length [%ld] does not match translation length [%ld]", prot1len, len);
  }

  if ((sfp->partial) && (!mismatch)) {
    if ((!no_beg) && (!no_end)) {       /* just didn't label */
      if (!got_stop) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "End of location should probably be partial");
      } else {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "This SeqFeat should not be partial");
      }
      show_stop = FALSE;
    }
  }


erret:
  if (show_stop) {
    if ((!got_stop) && (!no_end)) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_NoStop, "Missing stop codon");
    } else if ((got_stop) && (no_end)) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "Got stop codon, but 3'end is labeled partial");
    } else if ((got_stop) && (!no_end) && (ragged)) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_TransLen, "Coding region extends %d base(s) past stop codon", (int) ragged);
    }
  }

  if (!prot_ok) {
    if (transl_except)
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_TranslExcept, "Unparsed transl_except qual. Skipped");
  }

  if (prot2seq != NULL)
    BioseqFree (prot2seq);
  else
    BSFree (newprot);
  SeqPortFree (spp);
  return;
}

/*****************************************************************************
*
*   SpliceCheck(sfp)
*      checks for GT/AG rule at splice junctions
*
*****************************************************************************/
#define NOVALUE 0
#define HADGT 1
#define NOGT 2

static void SpliceCheckEx (ValidStructPtr vsp, SeqFeatPtr sfp, Boolean checkAll)
{
  SeqLocPtr       slp, nxt, head;
  Uint1           strand = Seq_strand_unknown;
  SeqPortPtr      spp = NULL;
  SeqIdPtr        last_sip = NULL, sip, id;
  Int2            total, ctr;
  BioseqPtr       bsp = NULL;
  Int4            strt, stp, len = 0, donor, acceptor;
  Int2            residue1, residue2;
  Char            tbuf[40];
  Boolean         reportAsError, first, last, firstPartial, lastPartial;
  int             severity;
  Uint2           partialflag;
  Boolean         gpsOrNTorNC = FALSE;
  SeqEntryPtr     sep;
  BioseqSetPtr    bssp;
  TextSeqIdPtr    tsip;

  if (sfp == NULL)
    return;

  if (GetAppProperty ("NcbiSubutilValidation") != NULL)
    return;                     /* suppress if NCBISubValidate */


  if (sfp->excpt)               /* biological exception */
    return;

  head = sfp->location;
  if (head == NULL)
    return;

  reportAsError = FALSE;
  if (GetAppProperty ("SpliceValidateAsError") != NULL) {
    reportAsError = TRUE;
  }

  slp = NULL;
  total = 0;
  while ((slp = SeqLocFindPart (head, slp, EQUIV_IS_ONE)) != NULL) {
    total++;
    if (slp->choice == SEQLOC_EQUIV)
      return;                   /* bail on this one */
    if (total == 1)
      strand = SeqLocStrand (slp);
    else {
      if (strand != SeqLocStrand (slp)) /* bail on mixed strand */
        return;
    }
  }

  if ((!checkAll) && total < 2)
    return;
  if (total < 1)
    return;

  slp = NULL;
  ctr = 0;

  first = TRUE;
  last = FALSE;
  firstPartial = FALSE;
  lastPartial = FALSE;

  /* genomic product set or NT_ contig always relaxes to SEV_WARNING */

  sep = vsp->sep;
  if (sep != NULL && IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set) {
      gpsOrNTorNC = TRUE;
    }
  }

  slp = SeqLocFindPart (head, slp, EQUIV_IS_ONE);
  while (slp != NULL) {
    nxt = SeqLocFindPart (head, slp, EQUIV_IS_ONE);
    last = (Boolean) (nxt == NULL);
    partialflag = SeqLocPartialCheck (slp);
    firstPartial = (Boolean) (first && (partialflag & SLP_START));
    lastPartial = (Boolean) (last && (partialflag & SLP_STOP));
    ctr++;
    sip = SeqLocId (slp);
    if (sip == NULL)
      break;

    /* genomic product set or NT_ contig always relaxes to SEV_WARNING */
    bsp = BioseqFind (sip);
    if (bsp != NULL) {
      for (id = bsp->id; id != NULL; id = id->next) {
        if (id->choice == SEQID_OTHER) {
          tsip = (TextSeqIdPtr) id->data.ptrvalue;
          if (tsip != NULL && tsip->accession != NULL) {
            if (StringNICmp (tsip->accession, "NT_", 3) == 0) {
              gpsOrNTorNC = TRUE;
            } else if (StringNICmp (tsip->accession, "NC_", 3) == 0) {
              gpsOrNTorNC = TRUE;
            }
          }
        }
      }
    }

    if ((ctr == 1) || (!SeqIdMatch (sip, last_sip))) {
      spp = SeqPortFree (spp);
      bsp = BioseqLockById (sip);
      if (bsp == NULL)
        break;
      len = bsp->length;
      spp = SeqPortNew (bsp, 0, -1, strand, Seq_code_ncbi4na);
      BioseqUnlock (bsp);
      if (spp == NULL)
        break;
      last_sip = sip;
    }
    acceptor = SeqLocStart (slp);
    donor = SeqLocStop (slp);

    if (strand != Seq_strand_minus) {
      strt = acceptor;
      stp = donor;
    } else {
      strt = donor;
      donor = acceptor;
      acceptor = strt;
      stp = len - donor - 1;    /* orient to reverse complement seqport */
      strt = len - acceptor - 1;
    }

    if (((checkAll && (!lastPartial)) || ctr < total) && (stp < (len - 2))) {   /* check donor on all but last exon and on sequence */
      SeqPortSeek (spp, (stp + 1), SEEK_SET);
      residue1 = SeqPortGetResidue (spp);
      residue2 = SeqPortGetResidue (spp);
      if (IS_residue (residue1) && IS_residue (residue2)) {
        if ((!(residue1 & 4)) ||        /* not G or */
            (!(residue2 & 8))) {        /* not T */
          if ((residue1 & 4) && (residue2 & 2)) {       /* GC minor splice site */
            tbuf[39] = '\0';
            if (vsp->suppressContext) {
              WorstBioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
            } else {
              BioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
            }
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_NotSpliceConsensus,
                      "Rare splice donor consensus (GC) found instead of (GT) after exon ending at position %ld of %s", (long) (donor + 1), tbuf);
          } else {
            if (gpsOrNTorNC) {
              severity = SEV_WARNING;
            } else if (checkAll) {
              severity = SEV_WARNING;
            } else if (reportAsError) {
              severity = SEV_ERROR;
            } else {
              severity = SEV_WARNING;
            }
            tbuf[39] = '\0';
            if (vsp->suppressContext) {
              WorstBioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
            } else {
              BioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
            }
            ValidErr (vsp, severity, ERR_SEQ_FEAT_NotSpliceConsensus,
                      "Splice donor consensus (GT) not found after exon ending at position %ld of %s", (long) (donor + 1), tbuf);
          }
        }
      }
    }

    if (((checkAll && (!firstPartial)) || ctr != 1) && (strt > 1)) {
      SeqPortSeek (spp, (strt - 2), SEEK_SET);
      residue1 = SeqPortGetResidue (spp);
      residue2 = SeqPortGetResidue (spp);
      if (IS_residue (residue1) && IS_residue (residue2)) {
        if ((!(residue1 & 1)) ||        /* not A or */
            (!(residue2 & 4))) {        /* not G */
          if (gpsOrNTorNC) {
            severity = SEV_WARNING;
          } else if (checkAll) {
            severity = SEV_WARNING;
          } else if (reportAsError) {
            severity = SEV_ERROR;
          } else {
            severity = SEV_WARNING;
          }
          tbuf[39] = '\0';
          if (vsp->suppressContext) {
            WorstBioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
          } else {
            BioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
          }
          ValidErr (vsp, severity, ERR_SEQ_FEAT_NotSpliceConsensus,
                    "Splice acceptor consensus (AG) not found before exon starting at position %ld of %s", (long) (acceptor + 1), tbuf);
        }
      }
    }
    first = FALSE;
    slp = nxt;
  }

  SeqPortFree (spp);
  return;
}

NLM_EXTERN void SpliceCheck (ValidStructPtr vsp, SeqFeatPtr sfp)
{
  SpliceCheckEx (vsp, sfp, FALSE);
}

/*****************************************************************************
*
*   ValidateSeqLoc(vsp, slp, prefix)
*
*****************************************************************************/
NLM_EXTERN void ValidateSeqLoc (ValidStructPtr vsp, SeqLocPtr slp, CharPtr prefix)
{
  SeqLocPtr       tmp, prev;
  Boolean         retval = TRUE, tmpval, mixed_strand = FALSE, ordered = TRUE, adjacent = FALSE, circular = FALSE;
  CharPtr         ctmp;
  Uint1           strand2, strand1;
  SeqIntPtr       sip1, sip2, prevsip;
  SeqPntPtr       spp;
  PackSeqPntPtr   pspp;
  SeqIdPtr        id1 = NULL, id2;
  BioseqPtr       bsp;
  SeqFeatPtr      sfp;

  if (slp == NULL)
    return;

  bsp = BioseqFindFromSeqLoc (slp);
  if (bsp != NULL && bsp->topology == 2) {
    circular = TRUE;
  }

  tmp = NULL;
  prev = NULL;
  sip1 = NULL;
  prevsip = NULL;
  strand1 = Seq_strand_other;
  while ((tmp = SeqLocFindNext (slp, tmp)) != NULL) {
    tmpval = TRUE;
    switch (tmp->choice) {
    case SEQLOC_INT:
      sip1 = prevsip;
      sip2 = (SeqIntPtr) (tmp->data.ptrvalue);
      strand2 = sip2->strand;
      id2 = sip2->id;
      tmpval = SeqIntCheck (sip2);
      if ((tmpval) && (sip1 != NULL) && (ordered) && (! circular)) {
        if (SeqIdForSameBioseq (sip1->id, sip2->id)) {
          if (strand2 == Seq_strand_minus) {
            if (sip1->to < sip2->to)
              ordered = FALSE;
            if (sip2->to + 1 == sip1->from)
              adjacent = TRUE;
          } else {
            if (sip1->to > sip2->to)
              ordered = FALSE;
            if (sip1->to + 1 == sip2->from)
              adjacent = TRUE;
          }
        }
      }
      if (prevsip != NULL) {
        if (SeqIdForSameBioseq (prevsip->id, sip2->id)) {
          if (prevsip->strand == sip2->strand && prevsip->from == sip2->from && prevsip->to == sip2->to) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_DuplicateInterval, "Duplicate exons in location");
          }
        }
      }
      prevsip = sip2;
      break;
    case SEQLOC_PNT:
      spp = (SeqPntPtr) (tmp->data.ptrvalue);
      strand2 = spp->strand;
      id2 = spp->id;
      tmpval = SeqPntCheck (spp);
      prevsip = NULL;
      break;
    case SEQLOC_PACKED_PNT:
      pspp = (PackSeqPntPtr) (tmp->data.ptrvalue);
      strand2 = pspp->strand;
      id2 = pspp->id;
      tmpval = PackSeqPntCheck (pspp);
      prevsip = NULL;
      break;
    case SEQLOC_NULL:
      break;
    default:
      strand2 = Seq_strand_other;
      id2 = NULL;
      prevsip = NULL;
      break;
    }
    if (!tmpval) {
      retval = FALSE;
      ctmp = SeqLocPrint (tmp);
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_FEAT_Range, "%s: SeqLoc [%s] out of range", prefix, ctmp);
      MemFree (ctmp);

    }

    if (tmp->choice != SEQLOC_NULL) {
      if ((strand1 != Seq_strand_other) && (strand2 != Seq_strand_other)) {
        if (SeqIdForSameBioseq (id1, id2)) {
          if (strand1 != strand2)
            mixed_strand = TRUE;
        }
      }

      strand1 = strand2;
      id1 = id2;
    }
  }

  if (vsp->sfp != NULL) {

    /* Publication intervals ordering does not matter */

    if (vsp->sfp->idx.subtype == FEATDEF_PUB) {
      ordered = TRUE;
      adjacent = FALSE;
    }

    /* ignore ordering of heterogen bonds */

    if (vsp->sfp->data.choice == SEQFEAT_HET) {
      ordered = TRUE;
      adjacent = FALSE;
    }

    /* misc_recomb intervals SHOULD be in reverse order */

    if (vsp->sfp->idx.subtype == FEATDEF_misc_recomb) {
      ordered = TRUE;
    }

    /* primer_bind intervals MAY be in on opposite strands */

    if (vsp->sfp->idx.subtype == FEATDEF_primer_bind) {
      mixed_strand = FALSE;
      ordered = TRUE;
    }
  }

  if (adjacent) {
    ctmp = SeqLocPrint (slp);
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_AbuttingIntervals, "%s: Adjacent intervals in SeqLoc [%s]", prefix, ctmp);
    MemFree (ctmp);
  }

  if (vsp->sfp != NULL) {
    sfp = vsp->sfp;
    if (sfp->excpt) {
      /* trans splicing exception turns off both mixed_strand and out_of_order messages */
      if (StringStr (sfp->except_text, "trans splicing") != NULL || StringStr (sfp->except_text, "trans-splicing") != NULL) {
        return;
      }
    }
  }

  if ((mixed_strand) || (!ordered)) {
    ctmp = SeqLocPrint (slp);
    if (mixed_strand) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MixedStrand, "%s: Mixed strands in SeqLoc [%s]", prefix, ctmp);
    }
    if (!ordered)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_SeqLocOrder, "%s: Intervals out of order in SeqLoc [%s]", prefix, ctmp);
    MemFree (ctmp);
    return;
  }

  if (vsp->sfp != NULL) {

    /* ignore special case features here as well */

    if (vsp->sfp->idx.subtype == FEATDEF_PUB ||
        vsp->sfp->data.choice == SEQFEAT_HET ||
        vsp->sfp->idx.subtype == FEATDEF_misc_recomb ||
        vsp->sfp->idx.subtype == FEATDEF_primer_bind)
      return;
  }

  /* newer check for intervals out of order on segmented bioseq */

  if (bsp == NULL || bsp->repr != Seq_repr_seg) return;

  if (SeqLocBadSortOrder (bsp, slp)) {
    ctmp = SeqLocPrint (slp);
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_SeqLocOrder, "%s: Intervals out of order in SeqLoc [%s]", prefix, ctmp);
    MemFree (ctmp);
  }

  /* newer check for mixed strand on segmented bioseq */

  if (SeqLocMixedStrands (bsp, slp)) {
    ctmp = SeqLocPrint (slp);
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MixedStrand, "%s: Mixed strands in SeqLoc [%s]", prefix, ctmp);
    MemFree (ctmp);
  }
}

/*****************************************************************************
*
*   SeqGraph validation section
*
*****************************************************************************/

typedef struct gphgetdata
{
  ValNodePtr      vnp;
  BioseqPtr       bsp;
}
GphGetData     , PNTR GphGetPtr;

typedef struct grphitem
{
  SeqGraphPtr     sgp;
  Int4            left;
  Int4            right;
  Int2            index;
}
GrphItem       , PNTR GrphItemPtr;

static Boolean GetGraphsProc (GatherObjectPtr gop)
{
  GphGetPtr       ggp;
  GrphItemPtr     gip;
  SeqGraphPtr     sgp;

  if (gop == NULL || gop->itemtype != OBJ_SEQGRAPH)
    return TRUE;
  ggp = (GphGetPtr) gop->userdata;
  sgp = (SeqGraphPtr) gop->dataptr;
  if (ggp == NULL || sgp == NULL)
    return TRUE;
  /* only phrap or gap4 currently allowed */
  if (StringICmp (sgp->title, "Phrap Quality") == 0 || StringICmp (sgp->title, "Phred Quality") == 0 || StringICmp (sgp->title, "Gap4") == 0) {
    /* data type must be bytes */
    if (sgp->flags[2] == 3) {
      if (SeqIdIn (SeqLocId (sgp->loc), ggp->bsp->id)) {
        gip = (GrphItemPtr) MemNew (sizeof (GrphItem));
        if (gip == NULL)
          return TRUE;
        gip->sgp = sgp;
        gip->left = GetOffsetInBioseq (sgp->loc, ggp->bsp, SEQLOC_LEFT_END);
        gip->right = GetOffsetInBioseq (sgp->loc, ggp->bsp, SEQLOC_RIGHT_END);
        ValNodeAddPointer (&(ggp->vnp), 0, (Pointer) gip);
      }
    }
  }
  return TRUE;
}

static int LIBCALLBACK SortSeqGraphProc (VoidPtr ptr1, VoidPtr ptr2)
{
  GrphItemPtr     gip1, gip2;
  ValNodePtr      vnp1, vnp2;

  if (ptr1 == NULL || ptr2 == NULL)
    return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL)
    return 0;
  gip1 = (GrphItemPtr) vnp1->data.ptrvalue;
  gip2 = (GrphItemPtr) vnp2->data.ptrvalue;
  if (gip1 == NULL || gip2 == NULL)
    return 0;
  if (gip1->left > gip2->left) {
    return 1;
  } else if (gip1->left < gip2->left) {
    return -1;
  } else if (gip1->right > gip2->right) {
    return -1;
  } else if (gip2->right < gip2->right) {
    return 1;
  }
  return 0;
}

/* gets valnode list of sorted graphs in GrphItem structures */

static ValNodePtr GetSeqGraphsOnBioseq (Uint2 entityID, BioseqPtr bsp)
{
  GphGetData      ggd;
  GrphItemPtr     gip;
  Int2            index;
  Boolean         objMgrFilter[OBJ_MAX];
  ValNodePtr      vnp;

  ggd.vnp = NULL;
  ggd.bsp = bsp;
  MemSet ((Pointer) &objMgrFilter, 0, sizeof (objMgrFilter));
  objMgrFilter[OBJ_SEQGRAPH] = TRUE;
  GatherObjectsInEntity (entityID, 0, NULL, GetGraphsProc, (Pointer) &ggd, objMgrFilter);
  for (vnp = ggd.vnp, index = 1; vnp != NULL; vnp = vnp->next, index++) {
    gip = (GrphItemPtr) vnp->data.ptrvalue;
    if (gip != NULL) {
      gip->index = index;
    }
  }
  ggd.vnp = ValNodeSort (ggd.vnp, SortSeqGraphProc);
  return ggd.vnp;
}

static void ValidateGraphsOnBioseq (GatherContextPtr gcp)
{
  Byte            bases[400];
  ByteStorePtr    bs;
  BioseqPtr       bsp;
  Int2            ctr, i, val, index;
  Int4            curroffset = 0, gphlen = 0, seqlen = 0, slplen,
    bslen, min = INT4_MAX, max = INT4_MIN, j, lastloc = -1, NsWithScore, GapsWithScore, ACGTsWithoutScore, valsBelowMin, valsAboveMax, firstN, firstACGT, pos;
  DeltaSeqPtr     dsp;
  Uint2           entityID, olditemid = 0, olditemtype = 0, numdsp = 0, numsgp = 0, firstsgitemid = 0;
  GrphItemPtr     gip;
  ValNodePtr      head, vnp;
  Boolean         outOfOrder = FALSE, fa2htgsBug = FALSE, overlaps = FALSE;
  Uint1           residue;
  SeqGraphPtr     sgp;
  SeqIntPtr       sintp;
  SeqLocPtr       slocp;
  SeqLitPtr       slp;
  SeqPortPtr      spp;
  ValidStructPtr  vsp;

  vsp = (ValidStructPtr) gcp->userdata;
  bsp = (BioseqPtr) gcp->thisitem;
  if (vsp == NULL || bsp == NULL)
    return;
  if (!ISA_na (bsp->mol))
    return;

  vsp->bsp = bsp;
  vsp->descr = NULL;
  vsp->sfp = NULL;
  vsp->bssp = (BioseqSetPtr) gcp->parentitem;

  if (SeqMgrGetParentOfPart (bsp, NULL) != NULL)
    return;

  entityID = ObjMgrGetEntityIDForPointer (bsp);
  head = GetSeqGraphsOnBioseq (entityID, bsp);
  if (head == NULL)
    return;

  olditemid = gcp->itemID;
  olditemtype = gcp->thistype;
  gcp->thistype = OBJ_SEQGRAPH;

  for (vnp = head, index = 1; vnp != NULL; vnp = vnp->next, index++) {
    gip = (GrphItemPtr) vnp->data.ptrvalue;
    if (gip == NULL)
      continue;

    sgp = gip->sgp;
    if (sgp == NULL)
      continue;
    gcp->itemID = sgp->idx.itemID;
    if (firstsgitemid == 0) {
      firstsgitemid = sgp->idx.itemID;
    }

    if (gip->index != index) {
      outOfOrder = TRUE;
      if (gip->index == 129 && index == 2) {
        fa2htgsBug = TRUE;
      }
    }
    if (gip->left <= lastloc) {
      overlaps = TRUE;
    }
    lastloc = gip->right;
    min = MIN ((Int4) min, (Int4) sgp->min.intvalue);
    max = MAX ((Int4) max, (Int4) sgp->max.intvalue);

    if (sgp->min.intvalue < 0 || sgp->min.intvalue > 100) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_GRAPH_GraphMin, "Graph min (%ld) out of range", (long) sgp->min.intvalue);
    }

    if (sgp->max.intvalue < 0 || sgp->max.intvalue > 100) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_GRAPH_GraphMax, "Graph max (%ld) out of range", (long) sgp->max.intvalue);
    }

    gphlen += sgp->numval;
    bs = (ByteStorePtr) sgp->values;
    if (bs != NULL) {
      bslen = BSLen (bs);
      if (sgp->numval != bslen) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphByteLen, "SeqGraph (%ld) and ByteStore (%ld) length mismatch", (long) sgp->numval, (long) bslen);
      }
    }
  }
  if (outOfOrder) {
    gcp->itemID = firstsgitemid;
    if (fa2htgsBug) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphOutOfOrder, "Graph components are out of order - probably caused by old fa2htgs bug");
    } else {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_GRAPH_GraphOutOfOrder, "Graph components are out of order - may be a software bug");
    }
  }
  if (overlaps) {
    gcp->itemID = firstsgitemid;
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphOverlap, "Graph components overlap, with multiple scores for a single base");
  }

  if (bsp->repr == Seq_repr_raw) {
    seqlen = bsp->length;
  } else if (bsp->repr == Seq_repr_delta) {
    for (dsp = (DeltaSeqPtr) (bsp->seq_ext); dsp != NULL; dsp = dsp->next) {
      switch (dsp->choice) {
      case 1:
        slocp = (SeqLocPtr) dsp->data.ptrvalue;
        if (slocp == NULL)
          break;
        if (slocp->choice != SEQLOC_NULL) {
          seqlen += SeqLocLen (slocp);
        }
        break;
      case 2:
        slp = (SeqLitPtr) dsp->data.ptrvalue;
        if (slp == NULL || slp->seq_data == NULL)
          break;
        seqlen += slp->length;
        break;
      default:
        break;
      }
    }
  }

  if (seqlen != gphlen && bsp->length != gphlen) {
    gcp->itemID = firstsgitemid;
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphBioseqLen, "SeqGraph (%ld) and Bioseq (%ld) length mismatch", (long) gphlen, (long) seqlen);
  }

  if (bsp->repr == Seq_repr_delta) {
    if (head != NULL && head->next != NULL) {
      for (dsp = (DeltaSeqPtr) (bsp->seq_ext), vnp = head; dsp != NULL && vnp != NULL; dsp = dsp->next) {
        gip = (GrphItemPtr) vnp->data.ptrvalue;
        if (gip == NULL)
          continue;
        sgp = gip->sgp;
        if (sgp == NULL)
          continue;
        switch (dsp->choice) {
        case 1:
          slocp = (SeqLocPtr) dsp->data.ptrvalue;
          if (slocp != NULL && slocp->choice != SEQLOC_NULL) {
            slplen = SeqLocLen (slocp);
            curroffset += slplen;
            if (sgp->numval != slplen) {
              gcp->itemID = sgp->idx.itemID;
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_GRAPH_GraphSeqLocLen, "SeqGraph (%ld) and SeqLoc (%ld) length mismatch", (long) sgp->numval, (long) slplen);
            }
            numdsp++;
            if (vnp != NULL) {
              vnp = vnp->next;
              numsgp++;
            }
          }
          break;
        case 2:
          slp = (SeqLitPtr) dsp->data.ptrvalue;
          if (slp != NULL && slp->seq_data != NULL) {
            if (sgp->numval != slp->length) {
              gcp->itemID = sgp->idx.itemID;
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphSeqLitLen, "SeqGraph (%ld) and SeqLit (%ld) length mismatch",
                        (long) sgp->numval, (long) slp->length);
            }
            slocp = sgp->loc;
            if (slocp != NULL && slocp->choice == SEQLOC_INT) {
              sintp = (SeqIntPtr) slocp->data.ptrvalue;
              if (sintp != NULL) {
                if (sintp->from != curroffset) {
                  gcp->itemID = sgp->idx.itemID;
                  ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphStartPhase, "SeqGraph (%ld) and SeqLit (%ld) start do not coincide",
                            (long) sintp->from, (long) curroffset);
                }
                if (sintp->to != slp->length + curroffset - 1) {
                  gcp->itemID = sgp->idx.itemID;
                  ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphStopPhase, "SeqGraph (%ld) and SeqLit (%ld) stop do not coincide",
                            (long) sintp->to, (long) (slp->length + curroffset - 1));
                }
              }
            }
            numdsp++;
            if (vnp != NULL) {
              vnp = vnp->next;
              numsgp++;
            }
          }
          if (slp != NULL) {
            curroffset += slp->length;
          }
          break;
        default:
          break;
        }
      }
      for (dsp = (DeltaSeqPtr) (bsp->seq_ext), numdsp = 0; dsp != NULL; dsp = dsp->next) {
        switch (dsp->choice) {
        case 1:
          slocp = (SeqLocPtr) dsp->data.ptrvalue;
          if (slocp != NULL && slocp->choice != SEQLOC_NULL) {
            numdsp++;
          }
          break;
        case 2:
          slp = (SeqLitPtr) dsp->data.ptrvalue;
          if (slp != NULL && slp->seq_data != NULL) {
            numdsp++;
          }
          break;
        default:
          break;
        }
      }
      for (vnp = head, numsgp = 0; vnp != NULL; vnp = vnp->next, numsgp++)
        continue;
      if (numdsp != numsgp) {
        gcp->itemID = firstsgitemid;
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphDiffNumber, "Different number of SeqGraph (%d) and SeqLit (%d) components", (int) numsgp, (int) numdsp);
      }
    }
  }

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    gip = (GrphItemPtr) vnp->data.ptrvalue;
    if (gip == NULL)
      continue;
    sgp = gip->sgp;
    if (sgp == NULL)
      continue;
    spp = SeqPortNewByLoc (sgp->loc, Seq_code_ncbi4na);
    if (spp == NULL)
      continue;
    slplen = SeqLocLen (sgp->loc);
    if (bsp->repr == Seq_repr_delta || bsp->repr == Seq_repr_virtual) {
      SeqPortSet_do_virtualEx (spp, TRUE, TRUE);        /* sets gapIsZero */
    }

    bs = (ByteStorePtr) sgp->values;
    BSSeek (bs, 0, SEEK_SET);
    j = 0;
    val = 0;

    ctr = SeqPortRead (spp, bases, sizeof (bases));
    i = 0;
    residue = (Uint1) bases[i];

    NsWithScore = 0;
    GapsWithScore = 0;
    ACGTsWithoutScore = 0;
    valsBelowMin = 0;
    valsAboveMax = 0;
    firstN = -1;
    firstACGT = -1;
    pos = gip->left;

    while (residue != SEQPORT_EOF && j < sgp->numval) {
      if (IS_residue (residue)) {
        val = (Int2) BSGetByte (bs);
        if (val < sgp->min.intvalue || val < 0) {
          valsBelowMin++;
        }
        if (val > sgp->max.intvalue || val > 100) {
          valsAboveMax++;
        }
        j++;
        switch (residue) {
        case 0:
          if (val > 0) {
            GapsWithScore++;
          }
          break;
        case 1:
        case 2:
        case 4:
        case 8:
          if (val == 0) {
            ACGTsWithoutScore++;
            if (firstACGT == -1) {
              firstACGT = pos;
            }
          }
          break;
        case 15:
          if (val > 0) {
            NsWithScore++;
            if (firstN == -1) {
              firstN = pos;
            }
          }
          break;
        default:
          break;
        }
      }
      i++;
      if (i >= ctr) {
        i = 0;
        ctr = SeqPortRead (spp, bases, sizeof (bases));
        if (ctr < 0) {
          bases[0] = -ctr;
        } else if (ctr < 1) {
          bases[0] = SEQPORT_EOF;
        }
      }
      residue = (Uint1) bases[i];
      pos++;
    }

    gcp->itemID = sgp->idx.itemID;
    if (ACGTsWithoutScore > 0) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_GRAPH_GraphACGTScore, "%ld ACGT bases have zero score value - first one at position %ld",
                (long) ACGTsWithoutScore, (long) firstACGT);
    }
    if (NsWithScore > 0) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_GRAPH_GraphNScore, "%ld N bases have positive score value - first one at position %ld",
                (long) NsWithScore, (long) firstN);
    }
    if (GapsWithScore > 0) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphGapScore, "%ld gap bases have positive score value", (long) GapsWithScore);
    }
    if (valsBelowMin > 0) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_GRAPH_GraphBelow, "%ld quality scores have values below the reported minimum", (long) valsBelowMin);
    }
    if (valsAboveMax > 0) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_GRAPH_GraphAbove, "%ld quality scores have values above the reported maximum", (long) valsAboveMax);
    }

    SeqPortFree (spp);
  }

  gcp->itemID = olditemid;
  gcp->thistype = olditemtype;

  ValNodeFreeData (head);
}

/*****************************************************************************
*
*   PatchBadSequence(bsp)
*
*****************************************************************************/
NLM_EXTERN Boolean PatchBadSequence (BioseqPtr bsp)
{
  ByteStorePtr    newseq;
  SeqPortPtr      spp;
  Boolean         is_na;
  Uint1           seqcode;
  Int2            repchar, residue;
  Int4            i, len;

  if (bsp == NULL)
    return FALSE;
  if (!((bsp->repr == Seq_repr_raw) || (bsp->repr == Seq_repr_const)))
    return FALSE;

  is_na = ISA_na (bsp->mol);
  if (is_na) {
    seqcode = Seq_code_iupacna;
    repchar = (Int2) 'N';       /* N */
  } else {
    seqcode = Seq_code_iupacaa;
    repchar = (Int2) 'X';
  }

  spp = SeqPortNew (bsp, 0, -1, 0, seqcode);
  if (spp == NULL)
    return FALSE;

  len = bsp->length;
  newseq = BSNew (len);
  if (newseq == NULL) {
    SeqPortFree (spp);
    return FALSE;
  }

  for (i = 0; i < len; i++) {
    residue = SeqPortGetResidue (spp);
    if (residue == INVALID_RESIDUE) {
      residue = repchar;
    }
    BSPutByte (newseq, residue);
  }

  SeqPortFree (spp);
  BSFree (bsp->seq_data);
  bsp->seq_data = newseq;
  bsp->seq_data_type = seqcode;

  BioseqRawPack (bsp);

  return TRUE;
}

static void FindABioseq (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
  BioseqPtr PNTR  bp;
  BioseqPtr       bsp;

  bp = (BioseqPtr PNTR) data;
  if (*bp != NULL)              /* already got one */
    return;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) (sep->data.ptrvalue);
    *bp = bsp;
  }
  return;
}

NLM_EXTERN CharPtr FindIDForEntry (SeqEntryPtr sep, CharPtr buf)
{
  BioseqPtr       bsp = NULL;

  if ((sep == NULL) || (buf == NULL))
    return NULL;

  *buf = '\0';
  SeqEntryExplore (sep, (Pointer) (&bsp), FindABioseq);

  if (bsp == NULL)
    return NULL;

  SeqIdPrint (bsp->id, buf, PRINTID_FASTA_LONG);
  return buf;
}

static CharPtr TrimSpacesOnEitherSide (CharPtr str)
{
  Uchar           ch;
  CharPtr         dst;
  CharPtr         ptr;

  if (str != NULL && str[0] != '\0') {
    dst = str;
    ptr = str;
    ch = *ptr;
    while (ch != '\0' && ch <= ' ') {
      ptr++;
      ch = *ptr;
    }
    while (ch != '\0') {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
    }
    *dst = '\0';
    dst = NULL;
    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      if (ch != ' ') {
        dst = NULL;
      } else if (dst == NULL) {
        dst = ptr;
      }
      ptr++;
      ch = *ptr;
    }
    if (dst != NULL) {
      *dst = '\0';
    }
  }
  return str;
}

static void CopyLetters (CharPtr dest, CharPtr source, size_t maxsize)
{
  Char            ch;
  CharPtr         tmp;

  if (dest == NULL || maxsize < 1)
    return;
  *dest = '\0';
  if (source == NULL)
    return;
  maxsize--;
  tmp = dest;
  ch = *source;
  while (maxsize > 1 && ch != '\0') {
    if (ch != '.') {
      *dest = ch;
      dest++;
      maxsize--;
    }
    source++;
    ch = *source;
  }
  *dest = '\0';
  TrimSpacesOnEitherSide (tmp);
}

static void LookForEtAl (ValidStructPtr vsp, ValNodePtr tmp)
{
  AuthorPtr       ap;
  AuthListPtr     authors = NULL;
  CitArtPtr       cap;
  CitBookPtr      cbp;
  CitGenPtr       cgp;
  CitSubPtr       csp;
  Char            first[64];
  Char            initials[16];
  Char            last[64];
  ValNodePtr      names;
  NameStdPtr      nsp;
  PersonIdPtr     pid;

  if (vsp == NULL || tmp == NULL)
    return;
  switch (tmp->choice) {
  case PUB_Article:
    cap = (CitArtPtr) (tmp->data.ptrvalue);
    authors = cap->authors;
    break;
  case PUB_Man:
  case PUB_Book:
  case PUB_Proc:
    cbp = (CitBookPtr) (tmp->data.ptrvalue);
    authors = cbp->authors;
    break;
  case PUB_Gen:
    cgp = (CitGenPtr) (tmp->data.ptrvalue);
    authors = cgp->authors;
    break;
  case PUB_Sub:
    csp = (CitSubPtr) (tmp->data.ptrvalue);
    authors = csp->authors;
    break;
  default:
    break;
  }
  if (authors == NULL || authors->choice != 1)
    return;
  for (names = authors->names; names != NULL; names = names->next) {
    ap = names->data.ptrvalue;
    if (ap != NULL) {
      pid = ap->name;
      if (pid != NULL && pid->choice == 2) {
        nsp = pid->data;
        if (nsp != NULL && nsp->names[0] != NULL) {
          CopyLetters (last, nsp->names[0], sizeof (last));
          CopyLetters (first, nsp->names[1], sizeof (first));
          CopyLetters (initials, nsp->names[4], sizeof (initials));
          if ((StringICmp (last, "et al") == 0) || (StringCmp (initials, "al") == 0 && StringCmp (last, "et") == 0 && first[0] == '\0')) {
            if (names->next == NULL) {
              ValidErr (vsp, SEV_WARNING, ERR_GENERIC_AuthorListHasEtAl, "Author list ends in et al.");
            } else {
              ValidErr (vsp, SEV_WARNING, ERR_GENERIC_AuthorListHasEtAl, "Author list contains et al.");
            }
          }
        }
      }
    }
  }
}

static void SpellCheckPub (ValidStructPtr vsp, ValNodePtr tmp)
{
  CitArtPtr       cap;
  CitBookPtr      cbp;
  CitGenPtr       cgp;
  ValNodePtr      titles = NULL;

  if ((vsp == NULL) || (tmp == NULL))
    return;

  switch (tmp->choice) {
  case PUB_Article:
    cap = (CitArtPtr) (tmp->data.ptrvalue);
    titles = cap->title;
    break;
  case PUB_Man:
  case PUB_Book:
  case PUB_Proc:
    cbp = (CitBookPtr) (tmp->data.ptrvalue);
    titles = cbp->title;
    break;
  case PUB_Gen:
    cgp = (CitGenPtr) (tmp->data.ptrvalue);
    if (cgp->cit != NULL)
      SpellCheckString (vsp, cgp->cit);
    if (cgp->title != NULL)
      SpellCheckString (vsp, cgp->title);
    break;
  default:
    break;
  }

  if (titles != NULL) {
    for (; titles != NULL; titles = titles->next) {
      if (titles->choice == Cit_title_name)
        SpellCheckString (vsp, (CharPtr) (titles->data.ptrvalue));
    }
  }

  return;
}

static void SpellCheckSeqDescr (GatherContextPtr gcp)
{
  PubdescPtr      pdp;
  ValNodePtr      tmp, vnp;
  ValidStructPtr  vsp;

  vsp = (ValidStructPtr) (gcp->userdata);
  if (vsp == NULL)
    return;

  vnp = (ValNodePtr) (gcp->thisitem);
  if (vnp == NULL)
    return;

  vsp->descr = vnp;
  vsp->sfp = NULL;

  if (vnp->choice == Seq_descr_pub) {
    pdp = (PubdescPtr) (vnp->data.ptrvalue);
    for (tmp = pdp->pub; tmp != NULL; tmp = tmp->next) {
      LookForEtAl (vsp, tmp);
    }
  }

  if (vsp->spellfunc == NULL)
    return;

  switch (vnp->choice) {
  case Seq_descr_title:
  case Seq_descr_region:
  case Seq_descr_comment:
    SpellCheckString (vsp, (CharPtr) (vnp->data.ptrvalue));
    break;
  case Seq_descr_pub:
    pdp = (PubdescPtr) (vnp->data.ptrvalue);
    for (tmp = pdp->pub; tmp != NULL; tmp = tmp->next) {
      SpellCheckPub (vsp, tmp);
    }
    break;
  default:
    break;
  }
  return;
}

NLM_EXTERN void SpellCheckSeqFeat (GatherContextPtr gcp)
{
  PubdescPtr      pdp;
  SeqFeatPtr      sfp;
  ProtRefPtr      prp;
  ValidStructPtr  vsp;
  ValNodePtr      vnp;

  vsp = (ValidStructPtr) (gcp->userdata);
  if (vsp == NULL)
    return;

  sfp = (SeqFeatPtr) (gcp->thisitem);
  if (sfp == NULL)
    return;

  vsp->descr = NULL;
  vsp->sfp = sfp;

  if (sfp->data.choice == SEQFEAT_PUB) {
    pdp = (PubdescPtr) (sfp->data.value.ptrvalue);
    for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
      LookForEtAl (vsp, vnp);
    }
  }

  if (vsp->spellfunc == NULL)
    return;

  SpellCheckString (vsp, sfp->comment);

  switch (sfp->data.choice) {
  case 1:                      /* Gene-ref */
    break;
  case 2:                      /* Org-ref */
    break;
  case 3:                      /* Cdregion */
    break;
  case 4:                      /* Prot-ref */
    prp = (ProtRefPtr) (sfp->data.value.ptrvalue);
    for (vnp = prp->name; vnp != NULL; vnp = vnp->next)
      SpellCheckString (vsp, (CharPtr) (vnp->data.ptrvalue));
    SpellCheckString (vsp, prp->desc);
    break;
  case 5:                      /* RNA-ref */
    break;
  case 6:                      /* Pub */
    pdp = (PubdescPtr) (sfp->data.value.ptrvalue);
    for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
      SpellCheckPub (vsp, vnp);
    }
    break;
  case 7:                      /* Seq */
    break;
  case 8:                      /* Imp-feat */
    break;
  case 9:                      /* Region */
    SpellCheckString (vsp, (CharPtr) (sfp->data.value.ptrvalue));
    break;
  case 10:                     /* Comment */
    break;
  case 11:                     /* Bond */
    break;
  case 12:                     /* Site */
    break;
  case 13:                     /* Rsite-ref */
    break;
  case 14:                     /* User-object */
    break;
  case 15:                     /* TxInit */
    break;
  case 16:                     /* Numbering */
    break;
  case 17:                     /* Secondary Structure */
    break;
  case 18:                     /* NonStdRes */
    break;
  case 19:                     /* Heterogen */
    break;
  case 20:                     /* BioSource */
    break;
  default:
    break;
  }

  return;
}

NLM_EXTERN void SpellCheckString (ValidStructPtr vsp, CharPtr str)
{
  if ((vsp == NULL) || (str == NULL))
    return;

  if (vsp->spellfunc == NULL)
    return;

  (*(vsp->spellfunc)) ((char *) str, (vsp->spellcallback));

  return;
}

NLM_EXTERN void SpellCallBack (char *str)
{
  ErrSev          sev;

  sev = SEV_ERROR;
  if (globalvsp != NULL && globalvsp->justwarnonspell) {
    sev = SEV_WARNING;
  }
  ValidErr (globalvsp, sev, ERR_GENERIC_Spell, "[ %s ]", (CharPtr) str);
  return;
}
