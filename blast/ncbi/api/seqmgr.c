/*  seqmgr.c
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
* File Name:  seqmgr.c
*
* Author:  James Ostell
*   
* Version Creation Date: 9/94
*
* $Revision: 6.214 $
*
* File Description:  Manager for Bioseqs and BioseqSets
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: seqmgr.c,v $
* Revision 6.214  2004/05/04 17:34:23  bollin
* initialize variables
*
* Revision 6.213  2004/02/24 20:57:44  kans
* added genesByLocusTag field, SeqMgrGetNextGeneByLocusTag function, now that locus_tag should be a unique identifier
*
* Revision 6.212  2003/12/12 20:59:48  kans
* added StrandsMatch to consolate code after TestForOverlap, allow both on candidate to be treated as plus
*
* Revision 6.211  2003/11/13 21:59:24  kans
* GetSeqIdForGI separates SEQID_OTHER from other types, so RefSeq takes higher priority over General
*
* Revision 6.210  2003/10/24 19:49:11  kans
* operon feature of equal range sorted before gene, mRNA, CDS
*
* Revision 6.209  2003/10/23 17:40:01  kans
* added SeqMgrGetOverlappingOperon and bspextra operonsByPos and numoperons fields
*
* Revision 6.208  2003/10/02 16:12:31  bollin
* added COMMON_INTERVAL overlap type to TestForOverlap
*
* Revision 6.207  2003/09/22 17:27:29  kans
* strip RNA- prefix, not just - on RNAs
*
* Revision 6.206  2003/09/22 16:13:20  kans
* LockFarComponentsEx takes new SeqLocPtr parameter
*
* Revision 6.205  2003/09/22 15:55:06  kans
* all rna context labels were searched for dash, now just trna
*
* Revision 6.204  2003/08/04 20:41:20  kans
* SeqMgrProcessNonIndexedBioseq needed to reset version to 0 each time through outer loop (EY)
*
* Revision 6.203  2003/04/03 22:40:09  kans
* feature index location problem now reports latest identifier in record to make it easier to find problem
*
* Revision 6.202  2003/03/10 20:55:08  kans
* LockAllSegments calls LockAllBioseqs if component was itself far delta or seg
*
* Revision 6.201  2003/02/12 14:20:46  kans
* added IsNonGappedLiteral, used to allow compressed deltas as (previously always raw) parts of segsets
*
* Revision 6.200  2003/01/29 22:01:16  kans
* corrected printing in CountGapsInDeltaSeq
*
* Revision 6.199  2003/01/21 20:15:00  kans
* support for SeqMgrSetSeqIdSetFunc and GetSeqIdSetForGI needed for validator
*
* Revision 6.198  2002/12/17 17:30:10  kans
* CountGapsInDeltaSeq calls NextLitLength to sum up adjacent non-gap SeqLits, used to break up long literals or isolate small runs of ambiguous (uncompressible) sequence
*
* Revision 6.197  2002/12/03 23:09:03  kans
* added TestFeatOverlap
*
* Revision 6.196  2002/11/21 16:28:50  kans
* fixed test for INTERVAL_OVERLAP
*
* Revision 6.195  2002/11/04 21:11:58  kans
* SIMPLE_OVERLAP check now is more inclusive
*
* Revision 6.194  2002/10/23 12:57:53  kans
* do not report cds or rna already set if gps
*
* Revision 6.193  2002/06/11 14:41:20  kans
* added support for locus_tag
*
* Revision 6.192  2002/06/10 13:50:42  kans
* added SeqMgrSetLenFunc, SeqMgrSetAccnVerFunc
*
* Revision 6.191  2002/06/07 21:24:57  kans
* update entityID/omdp index in BioseqReloadFunc, ObjMgrConnectFunc, ObjMgrDetachFunc
*
* Revision 6.190  2002/06/03 17:02:27  kans
* SeqMgrGetBestOverlappingFeat calls SeqLocMergeEx for CHECK_INTERVALS without fusing joints
*
* Revision 6.189  2002/05/08 18:58:09  kans
* itemID is Uint4
*
* Revision 6.188  2002/05/07 16:16:58  kans
* Uint4 counts so as not to allocate array based on Uint2 wrapped around number
*
* Revision 6.187  2002/04/12 20:42:37  kans
* LookupFarSeqIDs has separate parameter for alignments and history
*
* Revision 6.186  2002/03/18 22:38:23  kans
* added SeqMgrGetAllOverlappingFeatures
*
* Revision 6.185  2002/03/14 18:30:00  kans
* index featsByRev on the fly if not already done when requested
*
* Revision 6.184  2002/03/14 16:40:02  kans
* SeqMgrIndexFeaturesEx takes dorevfeats, SeqMgrExploreFeaturesRev and featsByRev added for asn2gb
*
* Revision 6.183  2002/03/07 16:12:29  kans
* TrimLocInSegment for asn2gb master style to constrain feature locations within the part sequence range used by the contig
*
* Revision 6.182  2002/02/27 23:21:51  kans
* do not SeqLocMerge to get left and right extremes - it was needed for trans splicing but broke (gene) features spanning the origin
*
* Revision 6.181  2002/02/14 19:10:44  kans
* added SeqMgrGetPROTgivenProduct, index protein->product
*
* Revision 6.180  2002/02/07 14:34:51  kans
* added LOCATION_SUBSET type, use in CDS-mRNA overlap check to subclassify problem
*
* Revision 6.179  2002/02/01 21:55:48  kans
* wrong test for hit already past location to check hierarchy
*
* Revision 6.178  2002/01/23 19:26:00  kans
* swap ivals if minus strand, check internal exon boundaries only if > 1 interval
*
* Revision 6.177  2002/01/22 21:22:52  kans
* CheckInternalExonBoundaries for CHECK_INTERVALS overlap test
*
* Revision 6.176  2002/01/07 19:28:25  kans
* pass result of SeqLocMerge to GetOffsetInNearBioseq to get left and right even with trans spliced feature
*
* Revision 6.175  2001/12/28 13:33:16  kans
* LookupFarSeqIDs uses smp->seq_id_precache_func
*
* Revision 6.174  2001/12/28 13:19:11  kans
* added seq_id_precache_func, SeqMgrSetPreCache, LookupFarSeqIDs
*
* Revision 6.173  2001/12/27 17:01:53  kans
* FetchFromSeqIdGiCache can take NULL sipp
*
* Revision 6.172  2001/12/26 15:29:22  kans
* FetchFromSeqIdGiCache and RecordInSeqIdGiCache are extern
*
* Revision 6.171  2001/12/17 12:48:06  kans
* sort by position callback tests for numivals > 0
*
* Revision 6.170  2001/12/14 20:04:31  kans
* do not fuse joints in ivals list, add fewer intervals test to sort, use sap itemID instead of pointer for sort, last sort test on sfp itemID
*
* Revision 6.169  2001/12/10 20:58:27  kans
* do not make ivals single interval for gene or pub
*
* Revision 6.168  2001/12/03 16:04:14  yaschenk
* Replacing HeapSort() for smp->BioseqIndex with sort-merge of 2 arrays
*
* Revision 6.167  2001/11/29 14:04:29  kans
* reverted GetThePointForOffset, deal with trans-splicing in feature indexing left/right extreme calculation itself
*
* Revision 6.166  2001/11/28 21:28:29  kans
* fixed SeqMgrDeleteIndexesInRecords, added SeqMgrReport (commented out)
*
* Revision 6.165  2001/11/28 12:31:11  kans
* SeqMgrDeleteIndexesInRecord only used if autoclean until bug is fixed
*
* Revision 6.164  2001/11/28 12:24:39  kans
* mark sets and seqs for deletion
*
* Revision 6.163  2001/11/15 18:15:48  kans
* set bsp->omdp at creation, SeqMgrDeleteIndexesInRecord sets omdp->bulkIndexFree
*
* Revision 6.162  2001/10/24 20:46:58  kans
* visit descriptors increment count only for those that pass the filter test
*
* Revision 6.161  2001/10/11 19:23:37  kans
* TestForOverlap returns diff, using SeqLocAinB for CHECK_INTERVALS, to get best mRNA for CDS
*
* Revision 6.160  2001/10/02 21:46:50  kans
* added SeqEntryAsnOut (JO)
*
* Revision 6.159  2001/10/01 15:31:53  yaschenk
* ObjMgrLookup() returns Int4, so results should be stored in Int4
*
* Revision 6.158  2001/10/01 11:30:37  kans
* added LockFarComponentsEx, which allows far feature products and locations to be locked as well as far seg or delta components
*
* Revision 6.157  2001/09/19 17:31:08  kans
* removed redundant line - found by TF
*
* Revision 6.156  2001/09/12 18:26:08  kans
* added bspIndex to bspextra, made SeqMgrGetBioseqContext much more efficient for large sets
*
* Revision 6.155  2001/08/06 23:59:45  kans
* added third party annotation SeqID support
*
* Revision 6.154  2001/07/18 20:11:00  kans
* feature by label array now sorted case insensitively
*
* Revision 6.153  2001/06/29 20:04:32  kans
* in get overlapping feature, get strand after seqlocmergeex
*
* Revision 6.152  2001/06/07 15:43:18  kans
* speed up alignment indexing within feature indexing to the original intention
*
* Revision 6.151  2001/06/05 19:31:46  kans
* if location filter is minus strand, and delta component is minus strand, will need to swap left and right
*
* Revision 6.150  2001/05/30 19:35:37  kans
* DoSegmentedProtein added to set best protein feature for segmented proteins and their parts - e.g., AH004145.1
*
* Revision 6.149  2001/05/17 19:02:16  kans
* feature context label strips mRNA-, etc., prefix from RNA feature labels
*
* Revision 6.148  2001/05/15 16:17:44  kans
* fixed bug in SeqMgrGetNextFeatureByLabel - index was incremented twice
*
* Revision 6.147  2001/05/15 16:00:30  kans
* feature indexing records strand after seqlocmerge, to catch minus strand on multiple parts
*
* Revision 6.146  2001/05/14 19:51:26  kans
* added SeqMgrGetNextFeatureByLabel
*
* Revision 6.145  2001/04/26 21:41:56  kans
* SeqMgrGetFeatureByLabel can take NULL context pointer
*
* Revision 6.144  2001/04/26 19:48:46  kans
* added SeqMgrGetFeatureByLabel, bspextra->featsByLabel
*
* Revision 6.143  2001/04/18 20:17:01  kans
* SeqMgrFindAlignByID was subtracting 1 incorrectly
*
* Revision 6.142  2001/04/04 15:41:03  kans
* free desclisthead with ValNodeFreeData, not ValNodeFree
*
* Revision 6.141  2001/03/29 23:42:51  kans
* added descriptor indexing - needed by large popset of segsets
*
* Revision 6.140  2001/03/26 18:58:16  kans
* added INTERVAL_OVERLAP choice for SeqMgrGetOverlappingFeature, if CDS completely within mRNA introns using this test, do not report validator error
*
* Revision 6.139  2001/01/23 22:06:26  kans
* SeqMgrExploreSegments now handles delta seqs as well as segmented seqs
*
* Revision 6.138  2000/11/28 18:33:53  kans
* locking far components no longer needs feature indexing, does not reload if already on list
*
* Revision 6.137  2000/11/21 14:35:10  kans
* feature sort order now uses coding region frame, if necessary, deals with intervals if both features are genes, RNAs, or CDSs
*
* Revision 6.136  2000/11/06 22:10:07  kans
* only seg parentBioseq and lastsegbsp if segmented, not delta
*
* Revision 6.135  2000/11/03 00:13:00  kans
* added from and to parameters to GenomePartToSegmentMap, needed because delta contig can refer to the same accession multiple times
*
* Revision 6.134  2000/11/02 16:14:27  kans
* RecordSegmentInBioseq makes fake loc for seqlit, same as DeltaSeqsToSeqLocs
*
* Revision 6.133  2000/11/01 23:48:30  kans
* far delta now works like segmented sequence for recording segments, mapping
*
* Revision 6.132  2000/10/31 16:16:25  madden
* Remove extra & for MemSet
*
* Revision 6.131  2000/10/30 21:26:10  shavirin
* Changes and fixes for some MT-safety related problems.
*
* Revision 6.130  2000/09/28 20:57:07  kans
* added LockFarComponents and UnlockFarComponents
*
* Revision 6.129  2000/08/09 17:15:33  kans
* SEQID_GI uses data.intvalue, not data.ptrvalue - this error showed up with 64-bit compilation
*
* Revision 6.128  2000/07/11 16:57:08  kans
* caching of refseq makes string with and without version
*
* Revision 6.127  2000/07/10 23:28:48  kans
* GenomePartToSegmentMap only takes two Bioseq parameters
*
* Revision 6.126  2000/07/10 15:17:07  kans
* added GenomePartToSegmentMap for acc->contig remapping
*
* Revision 6.125  2000/05/31 19:58:49  kans
* do not add last bsp a second time, speed up removal of NULLs from bspp array
*
* Revision 6.124  2000/05/25 20:04:13  dondosha
* Fixed bug in SeqIdIndexElementCmp
*
* Revision 6.123  2000/05/24 21:13:48  ostell
* added SeqMgrHoldIndexing() support
*
* Revision 6.122  2000/05/16 18:23:51  kans
* find appropriate bioseq subfunctions call BioseqFindCore, not BioseqFind
*
* Revision 6.121  2000/04/12 18:46:01  durand
* fixed a bug in RecordOneFeature (JK)
*
* Revision 6.120  2000/04/12 14:08:06  shavirin
* Removed ObjMgrReadLock() from SeqMgrDeleteFromBioseqIndex() function.
*
* Revision 6.119  2000/04/04 22:29:48  kans
* SeqMgrIndexFeaturesEx allows indexing on opposite strand for upside-down Web views - may need to allow both views simultaneously later
*
* Revision 6.118  2000/03/30 22:02:23  kans
* check for ObjMgrLookup return value < 0, do not dereference array in this case
*
* Revision 6.117  2000/03/26 19:13:08  kans
* BioseqLockById returns NULL on gnl|SeqLit|, which is generated by DeltaSeqsToSeqLoc during gather to allow older segmented code to work on delta seqs
*
* Revision 6.116  2000/03/08 18:32:42  kans
* get best overlap function now accepts Seq_strand_both
*
* Revision 6.115  2000/02/14 13:16:04  kans
* SeqLocMergeEx has merge_overlaps parameter separate from fuse_joints so indexed interval array records ribosomal stuttering
*
* Revision 6.114  2000/01/08 00:47:53  lewisg
* fixes to selection, update, color
*
* Revision 6.113  2000/01/07 22:58:03  kans
* added dnaStop field to map sig_peptide, etc., onto DNA coordinates to choose proper flatfile segment for display
*
* Revision 6.112  2000/01/07 02:32:31  kans
* added SeqMgrGetSfpProductList for access to bspextra->prodlisthead
*
* Revision 6.111  2000/01/07 02:17:44  kans
* removed featsBySap, added prodlisthead, to support gather by get_feats_product
*
* Revision 6.110  2000/01/06 00:45:10  kans
* scope within get best overlap to avoid far problems
*
* Revision 6.109  2000/01/06 00:33:40  kans
* improvements to SeqLocMerge and feature indexing to handle far locations with grace - but gene by overlap still fails if fetching enabled
*
* Revision 6.108  2000/01/05 19:16:46  kans
* added featsBySap field to be used to speed up targeted feature gather
*
* Revision 6.107  1999/12/28 21:04:30  kans
* backup one if already past and prev (not this) has overlap reference - binary search of value in the middle of two entries leaves R on the right, not the left, entry
*
* Revision 6.106  1999/12/28 20:27:13  kans
* if already past and no overlap parent, back up one
*
* Revision 6.105  1999/12/28 17:55:09  kans
* after binary search hits candidate R, do short linear search for smaller overlapping gene, then also go up hierarchy from R testing each parent overlap - no linear search for these, though
*
* Revision 6.104  1999/12/27 23:10:38  kans
* overlap not set value is -1, since 0 is a valid binary search slot
*
* Revision 6.103  1999/12/27 19:57:44  kans
* binary search needed precomputed index to handle overlapping gene (mRNA, CDS, etc.) features, so after binary search it can back up to the appropriate first feature to check in a linear scan
*
* Revision 6.102  1999/12/23 18:44:18  kans
*  SeqMgrGetOverlappingFeature takes choices for SIMPLE_OVERLAP, CONTAINED_WITHIN, and CHECK_INTERVALS
*
* Revision 6.101  1999/12/08 02:58:04  kans
* in BioseqFindFunc added a check for omdp == NULL before checking omdp->tempload != TL_CACHED
*
* Revision 6.100  1999/11/23 20:59:46  kans
* fixed typo in SeqMgrVisitFeatures
*
* Revision 6.99  1999/11/17 19:26:58  kans
* sort by sfp pointer also sorts ignored copies last
*
* Revision 6.98  1999/11/17 19:19:36  kans
* when sorting features, put ignored items last, so they will not be picked up in binary search by itemID
*
* Revision 6.97  1999/11/05 19:53:14  kans
* SeqMgrIndexAlignments called by SeqMgrIndexFeatures, but can now be called separately even if you do not index features
*
* Revision 6.96  1999/09/28 14:01:38  kans
* feature indexing now populates with lightweight GatherObjectsInEntity calls
*
* Revision 6.95  1999/09/26 00:16:41  kans
* calls AssignIDsInEntity
*
* Revision 6.94  1999/09/11 02:34:41  kans
* overlap test does better check for minus strand mismatch
*
* Revision 6.93  1999/08/25 22:07:59  kans
* made MakeReversedSeqIdString public
*
* Revision 6.92  1999/07/28 22:37:14  kans
* clear out context on calling stack in several places
*
* Revision 6.91  1999/07/22 16:20:53  kans
* added SeqMgrVisitDescriptors and SeqMgrVisitFeatures
*
* Revision 6.90  1999/07/21 23:53:55  kans
* added SeqMgrFindSeqAlignByID
*
* Revision 6.89  1999/04/01 20:44:13  kans
* Int2 lengths to Int4 to allow CountGapsInDeltaSeq with buffer > 32K
*
* Revision 6.88  1999/03/26 21:38:15  kans
* record sfp, sap, protFeat, cdsOrRnaFeat even if not TL_NOT_TEMP, clear when reaping
*
* Revision 6.87  1999/03/24 15:10:32  kans
* FetchFromSeqIdGiCache trims least recently used IDs if list grows too big
*
* Revision 6.86  1999/03/22 00:32:12  kans
* added SeqMgrGetProtXref
*
* Revision 6.85  1999/03/11 23:39:33  kans
* sprintf and sscanf casts
*
* Revision 6.84  1999/03/10 20:34:15  kans
* SeqMgrExploreFeatures location filter had incorrect binary search, would need array sorted primarily by rightmost coordinate to speed up, so commented out for now
*
* Revision 6.83  1999/03/04 17:10:13  kans
* check for tsip->release == NULL for showversion (JO)
*
* Revision 6.82  1999/02/24 17:09:46  kans
* speed up locationfilter in SeqMgrExploreFeatures
*
* Revision 6.81  1999/02/24 13:25:21  kans
* SeqMgrGetOverlappingFeature takes subtype and containedWithin parameters
*
* Revision 6.80  1999/02/16 22:18:36  kans
* fixed interval sorting
*
* Revision 6.79  1999/02/02 21:38:11  kans
* moved SHOWVERSION to header, SeqMgrAddIndexElement with and without version (JO)
*
* Revision 6.78  1999/01/29 16:13:51  kans
* SeqMgrExploreBioseqs/Segments/Descriptors/Features all return count of visits to callback
*
* Revision 6.77  1999/01/27 20:14:09  kans
* JustExamineBioseqs now sets nucs and prots to true if about to explore parts set
*
* Revision 6.76  1999/01/27 15:20:48  kans
* added SeqMgrGetBioseqContext
*
* Revision 6.75  1999/01/27 01:45:29  kans
* fixes to SeqMgrGetParentOfPart and JustExamineBioseqs
*
* Revision 6.74  1999/01/27 00:02:13  kans
* calculation of index in SeqMgrGetParentOfPart now uses partsByLoc, matching to partsBySeqId segpartptr
*
* Revision 6.73  1999/01/26 23:38:07  kans
* SeqMgrGetParentOfPart now takes SeqMgrSegmentContext PNTR parameter
*
* Revision 6.72  1999/01/26 22:54:41  kans
* added SeqMgrGetParentOfPart
*
* Revision 6.71  1999/01/26 20:44:29  kans
* numsegs field in bioseq context
*
* Revision 6.70  1999/01/26 18:26:25  kans
* BioseqFindFromSeqLoc is a public function that is better than GetBioseqGivenSeqLoc
*
* Revision 6.69  1999/01/14 21:50:04  kans
* added packaging level to descriptor context structure
*
* Revision 6.68  1998/12/09 22:08:49  kans
* fixed a typo that prevented cleanup of index on top bioseq set
*
* Revision 6.67  1998/12/07 16:55:08  kans
* fixed memory leak of extra block on top bioseqset when automatically cleaned up from objmgr
*
* Revision 6.66  1998/11/28 20:08:23  kans
* was not initializing or incrementing nummRNAs and numCDSs variables
*
* Revision 6.65  1998/11/24 22:21:24  kans
* index mRNA and CDS by position, allow arbitrary sorted feature array index
*
* Revision 6.64  1998/10/29 01:07:10  kans
* indexing makes new sep if not on top bsp or bssp
*
* Revision 6.63  1998/10/29 00:50:39  kans
* calls BasicSeqEntryCleanup at beginning of indexing
*
* Revision 6.62  1998/10/27 14:53:30  kans
* get overlapping gene/pub/source takes SeqLocPtr - pass in sfp->location
*
* Revision 6.61  1998/10/22 23:41:49  kans
* feat context has bsp, partial flags, far location flag, GetDesired functions can work on entity entity if using itemID
*
* Revision 6.60  1998/10/22 16:05:55  kans
* removed labeltype parameter from SeqMgrIndexFeatures, changed index parameter/field to Uint2
*
* Revision 6.59  1998/10/20 17:45:58  kans
* distinctive messages for whether far accession was handled or not
*
* Revision 6.58  1998/10/16 21:42:02  kans
* fixed comment on last sort criteria to parent seq-annot
*
* Revision 6.57  1998/10/16 21:36:37  kans
* feature sorting continues with internal intervals, subtype, label, and finally seqannot parent
*
* Revision 6.56  1998/10/13 20:59:48  kans
* SeqMgrGetOmdpForBioseq uses new field in Bioseq to avoid binary search lookup
*
* Revision 6.55  1998/10/09 15:45:13  kans
* more informative errors in SeqMgr indexing
*
* Revision 6.54  1998/09/30 14:49:37  kans
* set scope for FindAppropriateBioseq, FindFirstLocalBioseq, over and above gather scope, which does not apply to the above calls even from within a gather callback
*
* Revision 6.53  1998/09/29 20:06:07  kans
* FindFirstLocalBioseq and GetOffsetInFirstLocalBioseq to deal with far segments more gracefully than just not indexing the feature
*
* Revision 6.52  1998/09/29 15:07:17  kans
* corrected logic for seqDescFilter, seqFeatFilter, and featDefFilter in explore functions
*
* Revision 6.51  1998/09/23 16:41:07  kans
* added SeqMgrGetDesiredDescriptor
*
* Revision 6.50  1998/09/22 18:17:01  kans
* descriptor index flag now tracked properly, separate from itemID
*
* Revision 6.49  1998/09/22 18:01:25  kans
* had been skipping bssp for lastDescrItemID
*
* Revision 6.48  1998/09/22 16:55:51  kans
* added SeqMgrGetDesiredFeature and position index field
*
* Revision 6.47  1998/09/22 13:11:59  kans
* locationFilter parameter to explore features function
*
* Revision 6.46  1998/09/01 19:25:25  kans
* context parameter in get best protein, get cds/rna given product
*
* Revision 6.45  1998/08/24 18:27:09  kans
* removed solaris -v -fd warnings
*
* Revision 6.44  1998/08/21 21:32:36  kans
* populate ivals array (start/stop pairs) for indexed features
*
* Revision 6.43  1998/08/21 20:18:59  kans
* added SeqMgrExploreSegments, indexing features on segmented bioseq
*
* Revision 6.42  1998/08/19 16:26:48  kans
* MakeReversedSeqIdString called from original location of code, also finished support for biosource feature indexing
*
* Revision 6.41  1998/08/18 21:43:54  kans
* SeqIdWithinBioseq finds appropriate SeqID in bsp->id chain for use with SeqLocAinB, allowing multiple IDs on a protein bioseq
*
* Revision 6.40  1998/08/16 22:36:24  kans
* fixed direct map up from part to segmented bioseq
*
* Revision 6.39  1998/08/14 15:40:37  kans
* SeqMgrMapPartToSegmentedBioseq neede LIBCALL, speeded up function by adding map up on part if fetched
*
* Revision 6.38  1998/08/13 22:31:45  kans
* SeqMgrMapPartToSegmentedBioseq to speed up GetOffsetInBioseq, start of indexing segments, also index biosource by location for binary search (Wheelan)
*
* Revision 6.37  1998/08/12 22:16:29  kans
* sort seg-parts array by SeqId, handle seqloc_int and seqloc_whole
*
* Revision 6.36  1998/08/12 21:25:04  kans
* forgot to free allocated partslist array
*
* Revision 6.35  1998/08/12 21:10:37  kans
* added parts index to speed segmented bioseq mapping
*
* Revision 6.34  1998/07/23 17:30:41  kans
* get overlapping gene was nulling out sfp, then dereferencing
*
* Revision 6.33  1998/07/23 13:08:56  kans
* SeqMgrGetOverlappingGene and SeqMgrGetOverlappingPub take optional context pointer
*
* Revision 6.32  1998/07/23 01:15:19  kans
* minor fix to not return ignored feature
*
* Revision 6.31  1998/07/23 01:11:33  kans
* added SeqMgrGetOverlappingPub, gene overlap works even when gene spans circular origin
*
* Revision 6.30  1998/07/16 22:30:55  kans
* improved gene overlap function
*
* Revision 6.29  1998/07/16 16:56:38  kans
* added parent BioseqSetPtr field to SeqMgrBioseqContext, check most recent bioseq first when indexing features
*
* Revision 6.28  1998/07/06 16:15:17  kans
* fixed typo in explore bioseqs callback
*
* Revision 6.27  1998/07/06 15:57:29  kans
* SeqMgrExploreBioseqs takes entityID or ptr
*
* Revision 6.26  1998/07/06 15:30:15  kans
* scope on index explore, added SeqMgrExploreBioseqs
*
* Revision 6.25  1998/07/02 22:30:44  kans
* process product before indexing by location, ErrPostItem if cannot find bioseq for location
*
* Revision 6.24  1998/07/02 17:52:33  kans
* CreateBioseqExtraBlock was not being called for protein bioseq to link back to CDS, which was seen first
*
* Revision 6.23  1998/07/01 19:13:16  kans
* SMFeatBlock.data is allocated array of reasonable size
*
* Revision 6.22  1998/06/30 22:08:02  kans
* SeqMgrFeaturesAreIndexed takes entityID, returns time_t time stamp of latest indexing
*
* Revision 6.21  1998/06/30 14:28:00  kans
* changed GetSeqFeat, which collided with asn2ff4 for some linkers
*
* Revision 6.20  1998/06/30 14:20:16  kans
* changes to heap sort order to put genes first, then rnas, if ranges are equal
*
* Revision 6.19  1998/06/30 12:56:45  kans
* code fixes, public functions moved to explore.h
*
* Revision 6.18  1998/06/29 23:37:37  kans
* added context structure for all explores, index every bioseq in an entity
*
* Revision 6.17  1998/06/29 03:06:41  kans
* fixed two conditionals in ProcessFeatureProducts
*
* Revision 6.16  1998/06/29 02:29:44  kans
* new context get descriptor and feature functions now working, get gene not always working
*
* Revision 6.15  1998/06/29 01:33:27  kans
* added SeqMgrGetNextDescriptor and SeqMgrGetNextFeature
*
* Revision 6.14  1998/06/29 00:24:00  kans
* several changes to new indexing functions
*
* Revision 6.13  1998/06/28 03:44:18  kans
* omdp->parentptr is bssp, not omdp, so use ObjMgrFindByData to get higher descriptors
*
* Revision 6.12  1998/06/28 03:15:09  kans
* missing break statement caused features to be ignored
*
* Revision 6.11  1998/06/28 02:38:15  kans
* simplified filters, finished best gene, explore functions
*
* Revision 6.10  1998/06/27 22:23:49  kans
* improvements and further implementation of new indexing, exploration functions
*
* Revision 6.9  1998/06/27 00:03:45  kans
* fix feature heap sort, post increment feature insertion index, look for best prot
*   when setting cds back pointer, and merge descriptor count and feature collect callbacks
*
* Revision 6.8  1998/06/26 22:36:24  kans
* initial work on tracking sorted features, and cds and prot links, for rapid collection
*
* Revision 6.7  1998/05/01 16:13:13  kans
* caching of gi with NULL seqID allowed with protection against calling SeqIdDup
*
* Revision 6.6  1998/04/20 22:38:08  kans
* should prevent caching of gi with NULL seqID
*
* Revision 6.5  1998/04/08 16:52:08  kans
* casts to ValNodeLen calls
*
* Revision 6.4  1998/03/30 21:02:25  ostell
* removed check for parenttype != 0 on call to ObjMgrConnect in SeqMgrLinkSeqEntry
*   so that disconnects from sets would work as well as connects
*
* Revision 6.3  1997/11/19 22:14:42  ostell
* added support for multithreaded programs
*
* Revision 6.2  1997/09/25 18:20:14  tatiana
* fixing -1 bug for gaps in CountGapsInDeltaSeq
*
* Revision 6.1  1997/09/11 15:55:40  ostell
* Added support for SetColor messages
*
* Revision 6.0  1997/08/25 18:07:06  madden
* Revision changed to 6.0
*
* Revision 5.20  1997/07/31 16:06:49  kans
* BioseqLockById clears scope if first call to BioseqFindFunc fails, tries again
*
* Revision 5.19  1997/07/30 19:44:46  kans
* bug fix by Serge Bazhin
*
* Revision 5.18  1997/07/28 13:29:41  ostell
* Moved GetUniGeneIDForSeqId() to seqmgr.c
*
* Revision 5.17  1997/07/15 17:37:43  ostell
* fixed problems with duplicate Bioseqs in BioseqFindFunc
*
* Revision 5.16  1997/07/09 21:11:53  ostell
* added support for indexed seqid lookups of bioseqs
*
* Revision 5.15  1997/06/19 18:38:43  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.14  1997/06/17 17:59:09  kans
* GetSeqIdForGI now uses binary search in cache
*
* Revision 5.13  1997/06/17 16:33:57  kans
* first pass at cache in SeqIdForGi
*
* Revision 5.12  1997/03/26 14:01:37  ostell
* removed OMUserData from new copy of Bioseq when uncaching to stop
* memory leak
*
 * Revision 5.11  1997/02/24  21:46:17  ostell
 * in BioseqFindFunc, when checking with scope a failure occurs, now it
 *   checks again without scope.
 *
 * Revision 5.10  1997/01/23  22:38:21  ostell
 * added missing newline at end of file (sigh)
 *
 * Revision 5.9  1997/01/23  22:37:14  ostell
 * minor change to seqmgr.h for new indexing
 *
 * Revision 5.7  1997/01/08  22:48:50  tatiana
 * buf and buflen arguments added to CountGapsInDeltaSeq()
 *
 * Revision 5.6  1996/08/22  14:50:05  ostell
 * initialized static arrays in BioseqFindFunc
 *
 * Revision 5.5  1996/08/21  13:33:33  ostell
 * added cachig to BioseqFindFunc
 *
 * Revision 5.4  1996/08/06  19:56:03  kans
 * for SEQLOC_WHOLE, must call SeqIdFindBest on bsp->id
 *
 * Revision 5.3  1996/08/05  15:57:26  chappey
 * in BioseqReloadFunc, the OMUserDataPtr is passed to the new
 * ObjMgrDataPtr, and is not deleted anymore.
 *
 * Revision 5.2  1996/07/25  02:32:26  ostell
 * added CountGapsInDeltaSeq()
 *
 * Revision 5.1  1996/07/19  22:13:13  ostell
 * added SpreadGapsInDeltaSeq()
 *
 * Revision 5.0  1996/05/28  13:23:23  ostell
 * Set to revision 5.0
 *
 * Revision 4.7  1996/03/19  19:05:17  kans
 * SeqEntrySetScope now returns old scope, not new scope
 *
 * Revision 4.6  1996/01/23  14:44:38  kans
 * added Pointer casts to MemSet
 *
 * Revision 4.5  1995/12/22  14:43:59  ostell
 * added reload code to BioseqLockById
 * break out relad from cache code to be used as part of gather locking
 * with BioseqReload
 *
 * Revision 4.4  1995/12/09  23:12:41  kans
 * SeqEntryFind now can deal with a Seq-Submit ultimate parent
 *
 * Revision 4.3  1995/12/04  21:40:05  ostell
 * added GetSeqIdForGI() and GetGIForSeqId()
 *
 * Revision 4.2  1995/10/03  15:50:37  ostell
 * added support for selection by region.. now fully implemented
 *
 * Revision 4.1  1995/09/30  03:38:31  ostell
 * Changed ObjMgrMessage functions to pass a structure
 * Added support for selecting regions
 * Added ability to remove entity when no more views on it
 *
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.16  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*
*
* ==========================================================================
*/

/** for ErrPostEx() ****/

static char *this_module = "ncbiapi";
#define THIS_MODULE this_module
static char *this_file = __FILE__;
#define THIS_FILE this_file

/**********************/

#include <explore.h>       /* new public functions prototyped here */
#include <seqmgr.h>		   /* the interface */
#include <sequtil.h>       /* CLEAN THIS UP LATER? */
#include <gather.h>
#include <subutil.h>
#include <ncbithr.h>
#include <objfdef.h>
#include <sqnutils.h>
#include <seqport.h>
#include <edutil.h>

/*****************************************************************************
*
*   Bioseq Management
*
*****************************************************************************/

static BioseqPtr LIBCALLBACK BSFetchFunc PROTO((SeqIdPtr sid, Uint1 ld_type));
static BioseqPtr NEAR BioseqFindFunc PROTO((SeqIdPtr sid, Boolean reload_from_cache));
static Boolean NEAR SeqMgrGenericSelect PROTO((SeqLocPtr region, Int2 type,
                                             Uint1Ptr rgb));
static BioseqPtr NEAR BioseqReloadFunc PROTO((SeqIdPtr sid, ObjMgrDataPtr omdp));

static Boolean NEAR SeqMgrProcessNonIndexedBioseq PROTO((Boolean force_it));
static Boolean NEAR SeqMgrAddIndexElement PROTO((SeqMgrPtr smp, BioseqPtr bsp, CharPtr buf,
                                                  Boolean sort_now));
static void NEAR RevStringUpper PROTO((CharPtr str));
static BSFetchTop NEAR SeqMgrGetFetchTop (void);


/*****************************************************************************
*
*   Return the current SeqMgr
*   	SeqMgrGet is obsolete
*       SeqMgrReadLock, ReadUnlock, WriteLock, WriteUnlock are thread safe
*
*****************************************************************************/
static TNlmMutex smp_mutex = NULL;
static SeqMgrPtr global_smp = NULL;
static TNlmRWlock smp_RWlock = NULL;
static TNlmRWlock sgi_RWlock = NULL;

/*****************************************************************************
*
*   Return the current SeqMgr
*   	Initialize if not done already
*       This function will become obsolete
*
*****************************************************************************/
NLM_EXTERN SeqMgrPtr LIBCALL SeqMgrGet (void)
{
	Int4 ret;
	SeqMgrPtr smp;

	if (global_smp != NULL)
		return global_smp;

	ret = NlmMutexLockEx(&smp_mutex);  /* protect this section */
	if (ret)  /* error */
	{
		ErrPostEx(SEV_FATAL,0,0,"SeqMgrGet failed [%ld]", (long)ret);
		return NULL;
	}

	if (global_smp == NULL)  /* check again after mutex */
	{
	                             /*** have to initialize it **/
		smp = (SeqMgrPtr) MemNew (sizeof(SeqMgr));
		smp->bsfetch = BSFetchFunc;  /* BioseqFetch default */
		smp->fetch_on_lock = TRUE;	 /* fetch when locking */
		smp_RWlock = NlmRWinit();  /* initialize RW lock */
		sgi_RWlock = NlmRWinit();  /* initialize RW lock */
		global_smp = smp;       /* do this last for mutex safety */
	}

	NlmMutexUnlock(smp_mutex);

	return global_smp;
}

/*****************************************************************************
*
*   SeqMgrReadLock()
*   	Initialize if not done already
*       A thread can have only one read or write lock at a time
*       Many threads can have read locks
*       Only one thread can have a write lock
*       No other threads may have read locks if a write lock is granted
*       If another thread holds a write lock, this call blocks until write
*          is unlocked.
*
*****************************************************************************/
NLM_EXTERN SeqMgrPtr LIBCALL SeqMgrReadLock (void)
{
	SeqMgrPtr smp;
	Int4 ret;

	smp = SeqMgrGet();  /* ensure initialization */

	ret = NlmRWrdlock(smp_RWlock);
	if (ret != 0)
	{
		ErrPostEx(SEV_ERROR,0,0,"SeqMgrReadLock: RWrdlock error [%ld]",
			(long)ret);
		return NULL;
	}
	return smp;
}

/*****************************************************************************
*
*   SeqMgrWriteLock
*   	Initialize if not done already
*       A thread can have only one read or write lock at a time
*       Many threads can have read locks
*       Only one thread can have a write lock
*       No other threads may have read locks if a write lock is granted
*       If another thread holds a read or write lock, this call blocks until write
*          is unlocked.
*
*****************************************************************************/
NLM_EXTERN SeqMgrPtr LIBCALL SeqMgrWriteLock (void)
{
	SeqMgrPtr smp;
	Int4 ret;

	smp = SeqMgrGet();  /* ensure initialization */

	ret = NlmRWwrlock(smp_RWlock);
	if (ret != 0)
	{
		ErrPostEx(SEV_ERROR,0,0,"SeqMgrWriteLock: RWwrlock error [%ld]",
			(long)ret);
		return NULL;
	}
	smp->is_write_locked = TRUE;
	return smp;
}


/*****************************************************************************
*
*  SeqMgrUnlock()
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrUnlock (void)
{
	SeqMgrPtr smp;
	Int4 ret;

	smp = SeqMgrGet();  /* ensure initialization */

	ret = NlmRWunlock(smp_RWlock);
	if (ret != 0)
	{
		ErrPostEx(SEV_ERROR,0,0,"SeqMgrUnlock: RWunlock error [%ld]",
			(long)ret);
		return FALSE;
	}
	smp->is_write_locked = FALSE;  /* can't be write locked */
	return TRUE;
}

/****************************************************************************
*
*  RevStringUpper(str)
*    Up cases and reverses string
*      to get different parts early for SeqId StringCmps
*
*****************************************************************************/
static void NEAR RevStringUpper (CharPtr str)
{
	CharPtr nd;
	Char tmp;

		if (str == NULL)
			return;
    nd = str;
	while (*nd != '\0')
		nd++;
	nd--;

	while (nd > str)
	{
		tmp = TO_UPPER(*nd);
		*nd = TO_UPPER(*str);
		*str = tmp;
		nd--; str++;
	}

	if (nd == str)
		*nd = TO_UPPER(*nd);
	return;
}

NLM_EXTERN Boolean MakeReversedSeqIdString (SeqIdPtr sid, CharPtr buf, size_t len)

{
  Uint1         oldchoice;
  CharPtr       tmp;
  TextSeqIdPtr  tsip;

  if (sid == NULL || buf == NULL || len < 1) return FALSE;
  oldchoice = 0;
  switch (sid->choice) {
    case SEQID_GI:
      sprintf (buf, "%ld", (long)(sid->data.intvalue));
      break;
    case SEQID_EMBL:
    case SEQID_DDBJ:
      oldchoice = sid->choice;
      sid->choice = SEQID_GENBANK;
    case SEQID_GENBANK:
    case SEQID_PIR:
    case SEQID_OTHER:
    case SEQID_SWISSPROT:
    case SEQID_PRF:
    case SEQID_TPG:
    case SEQID_TPE:
    case SEQID_TPD:
      tsip = (TextSeqIdPtr) (sid->data.ptrvalue);
      if (tsip->accession != NULL) {
        tmp = tsip->name;
        tsip->name = NULL;
        SeqIdWrite (sid, buf, PRINTID_FASTA_SHORT, len);
        tsip->name = tmp;
      } else {
        SeqIdWrite (sid, buf, PRINTID_FASTA_SHORT, len);
      }
      if (oldchoice)
        sid->choice = oldchoice;
      break;
    default:
      SeqIdWrite (sid, buf, PRINTID_FASTA_SHORT, len);
      break;
  }
  RevStringUpper (buf);
  return TRUE;
}

/*****************************************************************************
*
*   SeqEntrySetScope(sep)
*   	scopes global seqentry searches to sep
*       setting sep=NULL, opens scope to all seqentries in memory
*       returns the current scope
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr LIBCALL SeqEntrySetScope(SeqEntryPtr sep)
{
	SeqEntryPtr curr = NULL;
	SeqMgrPtr smp;
	Int2 i, j;
	SMScopePtr smsp;
	TNlmThread thr;
	Boolean found;

	smp = SeqMgrWriteLock();
	if (smp == NULL) goto ret;
	thr = NlmThreadSelf();
	found = FALSE;
	for (i = 0, smsp = smp->scope; i < smp->num_scope; i++, smsp++)
	{
		if (NlmThreadCompare(thr, smsp->thr))
		{
			curr = smsp->SEscope;
			smsp->SEscope = sep;
			if (sep == NULL)  /* removing one? */
			{
				smp->num_scope--;
				j = smp->num_scope - i;  /* number to move */
				if (j)  /* not last one */
					MemCopy(smsp, (smsp+1), (size_t)(j * sizeof(SMScope)));
			}
			goto ret;    /* all done */
		}
	}

	              /* thread not on list */
	if (sep == NULL)
		goto ret;       /* nothing to do */

	i = smp->num_scope;
	j = smp->total_scope;
	if (j == i)  /* need more room */
	{
		j += 20;   /* new size */
		smsp = smp->scope;
		smp->scope = MemNew((size_t)(j * sizeof(SMScope)));
		MemCopy(smp->scope, smsp, (size_t)(i * sizeof(SMScope)));
		smp->total_scope = j;
		MemFree(smsp);
	}

	smp->scope[i].thr = thr;
	smp->scope[i].SEscope = sep;
	smp->num_scope++;

ret: SeqMgrUnlock();
	return curr;
}

/*****************************************************************************
*
*   SeqEntryGetScope(sep)
*       returns the current scope or NULL if none set
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr LIBCALL SeqEntryGetScope(void)
{
	SeqMgrPtr smp;
	SeqEntryPtr scope = NULL;
	Int2 i;
	SMScopePtr smsp;
	TNlmThread thr;

	smp = SeqMgrReadLock();
	if (smp == NULL) return FALSE;
	thr = NlmThreadSelf();
	for (i = 0, smsp = smp->scope; i < smp->num_scope; i++, smsp++)
	{
		if (NlmThreadCompare(thr, smsp->thr))
		{
			scope = smsp->SEscope;
			break;
		}
	}
	SeqMgrUnlock();
	return scope;
}

/*****************************************************************************
*
*   BioseqFind(SeqIdPtr)
*   	Just checks in object loaded memory
*       Will also restore a Bioseq that has been cached out
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqFind (SeqIdPtr sid)
{
	return BioseqFindFunc(sid, TRUE);
}

/*****************************************************************************
*
*   BioseqFindCore(sid)
*   	Finds a Bioseq in memory based on SeqId when only "core" elements needed
*   	Will NOT restore a Bioseq that has been cached out by SeqMgr
*       This function is for use ONLY by functions that only need the parts
*         of the Bioseq left when cached out. This includes the SeqId chain,
*         and non-pointer components of the Bioseq.
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqFindCore (SeqIdPtr sip)
{
	return BioseqFindFunc(sip, FALSE);
}

/*****************************************************************************
*
*   BioseqFindEntity(sid, itemIDptr)
*   	Finds a Bioseq in memory based on SeqId
*   	Will NOT restore a Bioseq that has been cached out by SeqMgr
*       returns EntityID if found, otherwise 0
*       itemIDptr is set to the value for itemID in ObjMgr functions
*       itemtype is OBJ_BIOSEQ of course
*
*****************************************************************************/
NLM_EXTERN Uint2 LIBCALL BioseqFindEntity (SeqIdPtr sip, Uint2Ptr itemIDptr)
{
	BioseqPtr bsp;
	Uint2 entityID = 0;

	*itemIDptr = 0;
	bsp = BioseqFindCore(sip);
	if (bsp == NULL) return entityID;  /* not found */
	entityID = ObjMgrGetEntityIDForPointer((Pointer)bsp);
	if (! entityID)
		return entityID;

	*itemIDptr = GatherItemIDByData(entityID, OBJ_BIOSEQ, (Pointer)bsp);
	return entityID;
}

/********************************************************************************
*
*   BioseqReload (omdp, lockit)
*     reloads the cached SeqEntry at top of omdp
*     if (lockit) locks the record
*
*********************************************************************************/

NLM_EXTERN ObjMgrDataPtr LIBCALL BioseqReload(ObjMgrDataPtr omdp, Boolean lockit)
{
	BioseqPtr bsp = NULL;
	ObjMgrDataPtr retval = NULL;
	Int4 j;
	ObjMgrPtr omp;

	if (omdp == NULL) return retval;
	if (! ((omdp->datatype == OBJ_BIOSEQ) || (omdp->datatype == OBJ_BIOSEQSET)))
		return retval;
	if (omdp->parentptr != NULL)
	{
		omp = ObjMgrReadLock();
		omdp = ObjMgrFindTop(omp, omdp);
		ObjMgrUnlock();
		if (omdp == NULL)
			return retval;
	}

	if (omdp->tempload == TL_CACHED)   /* only need to reload if cached */
	{
		bsp = BioseqReloadFunc (NULL, omdp);
		if (bsp == NULL)
			return retval;
		omp = ObjMgrReadLock();
		j = ObjMgrLookup(omp, (Pointer)bsp);
		if (j < 0) {

                    Char tmpbuff[256];

                    SeqIdWrite(bsp->id, tmpbuff, 
                               PRINTID_FASTA_LONG, sizeof(tmpbuff));

                    ErrPostEx(SEV_WARNING, 0, __LINE__, 
                              "ObjMgrLookup() returned negative value "
                              "id = %s, totobj = %d, currobj = %d, "
                              "HighestEntityID = %d", tmpbuff, omp->totobj,
                              omp->currobj, omp->HighestEntityID);
                    
                    ObjMgrUnlock();
                    return retval;
                }

		omdp = ObjMgrFindTop(omp, omp->datalist[j]);
		ObjMgrUnlock();
	}
	 
	if (lockit)
	{
		ObjMgrLock(omdp->datatype, omdp->dataptr, TRUE);
	}

	return omdp;
}

static BSFetchTop NEAR SeqMgrGetFetchTop (void)
{
	SeqMgrPtr smp;
	BSFetchTop bsftp=NULL;

	smp = SeqMgrReadLock();
	if (smp == NULL) return bsftp;
	bsftp = smp->bsfetch;
	SeqMgrUnlock();
	return bsftp;
}
	
static BioseqPtr NEAR BioseqReloadFunc (SeqIdPtr sid, ObjMgrDataPtr omdp)
{
    Int4 j;
	ObjMgrDataPtr oldomdp;
	OMUserDataPtr omudp, next;
	ObjMgrProcPtr ompp;
	OMProcControl ompc;
	BioseqPtr bsp= NULL;
	Int2 ret;
	ObjMgrPtr omp;
	BSFetchTop bsftp=NULL;

	ompp = NULL;
	omp = ObjMgrReadLock();
	for (omudp = omdp->userdata; omudp != NULL; omudp = omudp->next)
	{
		if (omudp->proctype == OMPROC_FETCH)  /* caching function */
		{
			ompp = ObjMgrProcFind(omp, omudp->procid, NULL, 0);
			if (ompp != NULL)
				break;
		}
	}
	ObjMgrUnlock();

	if (ompp == NULL)
		return bsp;
	if (ompp->outputtype != OBJ_BIOSEQ)
		return bsp;

	oldomdp = omdp;
	omdp = NULL;
	bsftp = SeqMgrGetFetchTop();
	if (bsftp != NULL)
	{
		if (ompp != NULL)	/* fetch proc left a signal */
		{                                 /* rerun fetch */
			MemSet((Pointer)(&ompc), 0, sizeof(OMProcControl));
			ompc.input_data = sid;
			ompc.input_entityID = oldomdp->EntityID;
			ompc.proc = ompp;
			ret = (* (ompp->func))((Pointer)&ompc);
			switch (ret)
			{
				case OM_MSG_RET_ERROR:
					ErrShow();
					break;
				case OM_MSG_RET_DEL:
					break;
				case OM_MSG_RET_OK:
					break;
				case OM_MSG_RET_DONE:
					omp = ObjMgrWriteLock();
					ObjMgrSetTempLoad (omp, ompc.output_data);
					ObjMgrUnlock();
					bsp = (BioseqPtr)(ompc.output_data);
					break;
				default:
					break;
			}
		}
		
		if (bsp == NULL)  /* nope, try regular fetch */
		{
			bsp = (*(bsftp))(sid, BSFETCH_TEMP);
		}

		if (bsp != NULL)
		{
			omp = ObjMgrReadLock();
			j = ObjMgrLookup(omp, (Pointer)bsp);
			if (j < 0) {
                            
                            Char tmpbuff[256];
                            
                            SeqIdWrite(bsp->id, tmpbuff, 
                                       PRINTID_FASTA_LONG, sizeof(tmpbuff));
                            
                            ErrPostEx(SEV_WARNING, 0, __LINE__, 
                                      "ObjMgrLookup() returned negative value "
                                      "id = %s, totobj = %d, currobj = %d, "
                                      "HighestEntityID = %d", tmpbuff, 
                                      omp->totobj,
                                      omp->currobj, omp->HighestEntityID);
                            ObjMgrUnlock();
                            return bsp;
                        }
			omdp = ObjMgrFindTop(omp, omp->datalist[j]);
			ObjMgrUnlock();
			ObjMgrDeleteIndexOnEntityID (omp, oldomdp->EntityID);
			omdp->EntityID = oldomdp->EntityID;
			oldomdp->EntityID = 0;
			ObjMgrAddIndexOnEntityID (omp, omdp->EntityID, omdp);

			omudp = omdp->userdata;
			while (omudp != NULL)
			{
				next = omudp->next;
				if (omudp->freefunc != NULL)
                                 (*(omudp->freefunc))(omudp->userdata.ptrvalue);
				MemFree(omudp);
				omudp = next;
			}
            omdp->userdata = oldomdp->userdata;
            oldomdp->userdata = NULL;

			if (oldomdp->choice != NULL)
				SeqEntryFree(oldomdp->choice);
			else
			{
				switch(oldomdp->datatype)
				{
					case OBJ_BIOSEQ:
						BioseqFree((BioseqPtr)(oldomdp->dataptr));
						break;
					case OBJ_BIOSEQSET:
						BioseqSetFree((BioseqSetPtr)(oldomdp->dataptr));
						break;
					default:
						ErrPostEx(SEV_ERROR,0,0,"BioseqFindFunc: delete unknown type [%d]",
							(int)(oldomdp->datatype));
						break;
				}
			}
		}
	}
	return bsp;
}
/** static func used internally **/

/*******************************************
*
*  WARNING: if you change BIOSEQ_CACHE_NUM, you have to change the
*   number of NULL in the initialization of the 2 static pointer arrays
*   below
*
*******************************************/
/* nb: this cache is cleared in SeqMgrDeleteFromBioseqIndex() */
#define BIOSEQ_CACHE_NUM 3
static SeqEntryPtr se_cache[BIOSEQ_CACHE_NUM] = {
	NULL, NULL, NULL};   /* for a few platforms */
static ObjMgrDataPtr omdp_cache[BIOSEQ_CACHE_NUM] = {
	NULL, NULL, NULL};   /* for a few platforms */
static TNlmMutex smp_cache_mutex = NULL;

static BioseqPtr NEAR BioseqFindFunc (SeqIdPtr sid, Boolean reload_from_cache)
{
    Int4 i, j, num, imin, imax, retval;
	SeqIdIndexElementPtr PNTR sipp;
	CharPtr tmp;
	Char buf[80];
	Boolean do_return;
	SeqMgrPtr smp;
	ObjMgrPtr omp;
	ObjMgrDataPtr omdp;
	BioseqPtr bsp = NULL, tbsp;
	SeqEntryPtr scope;

	if (sid == NULL)
		return NULL;

	SeqMgrReadLock();    /* make sure no other thread is writing */
	retval = NlmMutexLockEx(&smp_cache_mutex);  /* protect this section */
	SeqMgrUnlock();
	if (retval)  /* error */
	{
		ErrPostEx(SEV_FATAL,0,0,"BioseqFindFunc cache mutex failed [%ld]", (long)retval);
		return NULL;
	}

	do_return = FALSE;
	scope = SeqEntryGetScope();	   /* first check the cache */
	for (i = 0; i < BIOSEQ_CACHE_NUM; i++)
	{
		if (omdp_cache[i] == NULL)
			break;
		omdp = omdp_cache[i];
		if (omdp->datatype == OBJ_BIOSEQ)
		{
			if ((scope == NULL) || (scope == se_cache[i]))
			{
				bsp = (BioseqPtr)(omdp->dataptr);

				if (BioseqMatch(bsp, sid))
				{
					for (j = i; j > 0; j--)  /* shift to top of cache */
					{
						omdp_cache[j] = omdp_cache[j-1];
						se_cache[j] = se_cache[j-1];
					}
					omdp_cache[0] = omdp;
					se_cache[0] = scope;

					if (! reload_from_cache)
					{
						do_return = TRUE;
						goto done_cache;
					}

					omp = ObjMgrReadLock();
					omdp = ObjMgrFindTop(omp, omdp);
					ObjMgrUnlock();
					if (omdp == NULL || omdp->tempload != TL_CACHED)
					{
						do_return = TRUE;
						goto done_cache;
					}

					bsp = BioseqReloadFunc(sid, omdp);

					if (bsp == NULL)
					{
						
                        ErrPostEx(SEV_ERROR,0,0,"BioseqFindFunc: couldn't uncache");
					}
					do_return = TRUE;
					goto done_cache;
				}
			}
		}
	}
done_cache:
	NlmMutexUnlock(smp_cache_mutex);
	if (do_return)  /* all done */
	{
		return bsp;
	}

	bsp = NULL; /* resetting it */

	SeqMgrProcessNonIndexedBioseq(TRUE);	/* make sure all are indexed */

		/* stringify as in SeqMgrAdd */

	MakeReversedSeqIdString (sid, buf, 79); /* common function to make id, call RevStringUpper */


	imin = 0;
	smp = SeqMgrReadLock();
	imax = smp->BioseqIndexCnt - 1;
	sipp = smp->BioseqIndex;

	num = -1;

	while (imax >= imin)
	{
		i = (imax + imin)/2;
		tmp = sipp[i]->str;
		if ((j = StringCmp(tmp, buf)) > 0)
			imax = i - 1;
		else if (j < 0)
			imin = i + 1;
		else
		{
			num = i;
			break;
		}
	}

	if (num < 0)  /* couldn't find it */
	{
		/*
		Message(MSG_ERROR, "[1] Couldn't find [%s]", buf);
		*/
		bsp = NULL;
		goto ret;
	}


	if (scope != NULL)	/* check in scope */
	{
		tbsp = (BioseqPtr)(sipp[num]->omdp->dataptr);
		if (ObjMgrIsChild(scope->data.ptrvalue, tbsp))
		{
			bsp = tbsp;
			omdp = sipp[num]->omdp;
		}
		else
		{                  /* not in scope, could be duplicate SeqId */
			i = num-1;
			while ((i >= 0) && (bsp == NULL) && (! StringCmp(sipp[i]->str, buf)))  /* back up */
			{
			   tbsp = (BioseqPtr)(sipp[i]->omdp->dataptr);
			   if (ObjMgrIsChild(scope->data.ptrvalue, tbsp))
			   {
				   bsp = tbsp;
					omdp = sipp[i]->omdp;
			   }
			   i--;
			}
			i = num + 1;
			imax = smp->BioseqIndexCnt - 1;
			while ((bsp == NULL) && (i <= imax) && (! StringCmp(sipp[i]->str, buf)))
			{
			   tbsp = (BioseqPtr)(sipp[i]->omdp->dataptr);
			   if (ObjMgrIsChild(scope->data.ptrvalue, tbsp))
			   {
				   bsp = tbsp;
					omdp = sipp[i]->omdp;
			   }
			   i++;
			}
		}
	}
	else  /* no scope set */
	{
		omdp = sipp[num]->omdp;
		bsp = (BioseqPtr)(omdp->dataptr);
	}


	if (bsp == NULL)   /* not found */
	{
		/*
		Message(MSG_ERROR, "[2] Couldn't find [%s]", buf);
		*/
		goto ret;
	}

	retval = NlmMutexLockEx(&smp_cache_mutex);  /* protect this section */
	if (retval)  /* error */
	{
		ErrPostEx(SEV_FATAL,0,0,"BioseqFindFunc2 cache mutex failed [%ld]", (long)retval);
		bsp = NULL;
		goto ret;
	}

	for (j = (BIOSEQ_CACHE_NUM - 1); j > 0; j--)  /* shift to top of cache */
	{
		omdp_cache[j] = omdp_cache[j-1];
		se_cache[j] = se_cache[j-1];
	}
	omdp_cache[0] = omdp;
	se_cache[0] = scope;

	NlmMutexUnlock(smp_cache_mutex);

	if (! reload_from_cache)
		goto ret;

	omp = ObjMgrReadLock();
	omdp = ObjMgrFindTop(omp, omdp);
	ObjMgrUnlock();
	if (omdp == NULL)
	{
		bsp = NULL;
		goto ret;
	}
        if (omdp->tempload == TL_CACHED)
        {
                SeqMgrUnlock();
                bsp = BioseqReloadFunc(sid, omdp);
                goto ret2;
        }
ret:
        SeqMgrUnlock();
ret2:
        return bsp;
}

/*****************************************************************************
*
*   SeqMgrFreeCache()
*   	frees all cached SeqEntrys
*   	returns FALSE if any errors occurred
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrFreeCache(void)
{
	return ObjMgrFreeCache(OBJ_SEQENTRY);
}

/*****************************************************************************
*
*   BioseqLockById(SeqIdPtr)
*   	Finds the Bioseq and locks it
*       Makes sure appropriate BioseqContent is present
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqLockById (SeqIdPtr sid)
{
	BioseqPtr bsp = NULL;
	SeqMgrPtr smp;
	SeqEntryPtr oldscope = NULL;
	BSFetchTop bsftp;
	Boolean fetch_on_lock;
	DbtagPtr dbt;

	if (sid == NULL) return bsp;

	/* special case for DeltaSeqsToSeqLoc fake IDs - ignore */
	if (sid->choice == SEQID_GENERAL) {
		dbt = (DbtagPtr) sid->data.ptrvalue;
		if (dbt != NULL && StringCmp (dbt->db, "SeqLit") == 0) {
			return NULL;
		}
	}

	bsp = BioseqFindFunc(sid, TRUE);
	if (bsp == NULL)
	{
		smp = SeqMgrReadLock();
		if (smp == NULL) return bsp;
		fetch_on_lock = smp->fetch_on_lock;
		bsftp = smp->bsfetch;
		SeqMgrUnlock();

		if (fetch_on_lock)
		{
			oldscope = SeqEntrySetScope (NULL);
			if (oldscope != NULL) {
				bsp = BioseqFindFunc(sid, TRUE);
				SeqEntrySetScope (oldscope);
			}
			if (bsp == NULL && bsftp != NULL)
	            bsp = (*(bsftp))(sid, BSFETCH_TEMP);
		}
	}

	if (bsp == NULL) return NULL;

	ObjMgrLock(OBJ_BIOSEQ, (Pointer)bsp, TRUE);
	return bsp;
}

/*****************************************************************************
*
*   BioseqUnlockById(SeqIdPtr sip)
*   	Frees a Bioseq to be dumped from memory if necessary
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqUnlockById (SeqIdPtr sip)
{
	BioseqPtr bsp;

	if (sip == NULL) return FALSE;

	bsp = BioseqFindFunc(sip, FALSE);
	if (bsp == NULL)
		return FALSE;

	ObjMgrLock(OBJ_BIOSEQ, (Pointer)bsp, FALSE);
	return TRUE;
}

/*****************************************************************************
*
*   BioseqLock(BioseqPtr)
*   	Locks a Bioseq
*       Any cached data is returned to memory
*
*****************************************************************************/
NLM_EXTERN BioseqPtr LIBCALL BioseqLock (BioseqPtr bsp)
{
	if (bsp == NULL) return NULL;

	ObjMgrLock(OBJ_BIOSEQ, (Pointer)bsp, TRUE);

	return bsp;
}

/*****************************************************************************
*
*   BioseqUnlock(BioseqPtr)
*   	Frees a Bioseq to be dumped from memory if necessary
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BioseqUnlock (BioseqPtr bsp)
{
	if (bsp == NULL) return FALSE;

	if (ObjMgrLock(OBJ_BIOSEQ, (Pointer)bsp, FALSE) >= 0)
		return TRUE;
	else
		return FALSE;
}

/*****************************************************************************
*
*   BioseqFetch(SeqIdPtr, flag)
*   	loads bioseq into memory if possible
*   	first trys LocalLoad
*       they trys EntrezLoad
*
*****************************************************************************/
static BioseqPtr LIBCALLBACK BSFetchFunc (SeqIdPtr sid, Uint1 ld_type)
{
	BioseqPtr bsp = NULL;
	ObjMgrProcPtr ompp;
	OMProcControl ompc;
	Int2 ret;
	ObjMgrPtr omp;

	ompp = NULL;
	while ((ompp = ObjMgrProcFindNext(NULL, OMPROC_FETCH, OBJ_SEQID, OBJ_BIOSEQ, ompp)) != NULL)
	{
		MemSet((Pointer)(&ompc), 0, sizeof(OMProcControl));
		ompc.input_data = sid;
		ompc.proc = ompp;
		ret = (* (ompp->func))((Pointer)&ompc);
		switch (ret)
		{
			case OM_MSG_RET_ERROR:
				ErrShow();
				break;
			case OM_MSG_RET_DEL:
				break;
			case OM_MSG_RET_OK:
				break;
			case OM_MSG_RET_DONE:
				if (ld_type == BSFETCH_TEMP)
				{
					omp = ObjMgrWriteLock();
					ObjMgrSetTempLoad (omp, ompc.output_data);
					ObjMgrUnlock();
				}
				bsp = (BioseqPtr)(ompc.output_data);
				break;
			default:
				break;
		}
		if (bsp != NULL)  /* got one */
			break;
	}

	return bsp;
}


NLM_EXTERN BioseqPtr LIBCALL BioseqFetch (SeqIdPtr sid, Uint1 ld_type)
{
	BSFetchTop bsftp;
	BioseqPtr bsp;

	bsp = BioseqFindFunc(sid, TRUE);
	if (bsp != NULL) return bsp;
	
	bsftp = SeqMgrGetFetchTop();
	if (bsftp == NULL) return NULL;

	return (*(bsftp))(sid, ld_type);
}

/*****************************************************************************
*
*   GetGIForSeqId(SeqIdPtr)
*     returns the GI for a SeqId
*     returns 0 if can't find it
*
*****************************************************************************/
NLM_EXTERN Int4 LIBCALL GetGIForSeqId (SeqIdPtr sid)
{
	BioseqPtr bsp = NULL;
	ObjMgrProcPtr ompp;
	OMProcControl ompc;
	Int2 ret;
	SeqIdPtr sip;
	Int4 gi=0;


	if (sid == NULL)
		return gi;

	if (sid->choice == SEQID_GI)
		return sid->data.intvalue;

	bsp = BioseqFindCore(sid);
	if (bsp != NULL)
	{
		for (sip = bsp->id; sip != NULL; sip = sip->next)
		{
			if (sip->choice == SEQID_GI)
				return sip->data.intvalue;
		}
	}


	ompp = NULL;
	while ((ompp = ObjMgrProcFindNext(NULL, OMPROC_FETCH, OBJ_SEQID, OBJ_SEQID, ompp)) != NULL)
	{
		if ((ompp->subinputtype == 0) && (ompp->suboutputtype == SEQID_GI))
		{
			MemSet((Pointer)(&ompc), 0, sizeof(OMProcControl));
			ompc.input_data = sid;
			ompc.proc = ompp;
			ret = (* (ompp->func))((Pointer)&ompc);
			switch (ret)
			{
				case OM_MSG_RET_ERROR:
					ErrShow();
					break;
				case OM_MSG_RET_DEL:
					break;
				case OM_MSG_RET_OK:
					break;
				case OM_MSG_RET_DONE:
					sip = (SeqIdPtr)(ompc.output_data);
					if (sip != NULL)
					{
						if (sip->choice == SEQID_GI)
						{
							gi = sip->data.intvalue;
							SeqIdFree(sip);
							return gi;
						}
						SeqIdFree(sip);
					}
					break;
				default:
					break;
			}
		}
	}

	return gi;
}


/*****************************************************************************
*
*   GetSeqIdForGI(Int4)
*     returns the SeqId for a GI
*     returns NULL if can't find it
*     The returned SeqId is allocated. Caller must free it.
*
*****************************************************************************/
typedef struct seqidblock {
  Int4       uid;
  time_t     touch;
  SeqIdPtr   sip;
} SeqIdBlock, PNTR SeqIdBlockPtr;

static ValNodePtr seqidgicache = NULL;
static ValNodePtr PNTR seqidgiarray = NULL;
static Int2 seqidcount = 0;
static TNlmRWlock sid_RWlock = NULL;

NLM_EXTERN void RecordInSeqIdGiCache (Int4 gi, SeqIdPtr sip)

{
	ValNodePtr vnp, tmp;
	ValNodePtr PNTR prev;
	SeqIdBlockPtr sibp;
	Int4 retval;

	/* if (sip == NULL) return; okay to cache NULL because we protect against SeqIdDup */

	retval = NlmRWwrlock(sgi_RWlock);
	if (retval != 0)
	{
		ErrPostEx(SEV_ERROR,0,0,"RecSeqIdGi: RWwrlock error [%ld]",
			(long)retval);
		return;
	}

	
	vnp = ValNodeNew (NULL);
	if (vnp == NULL) goto ret;
	sibp = (SeqIdBlockPtr) MemNew (sizeof (SeqIdBlock));
	if (sibp == NULL) {
		MemFree (vnp);
		goto ret;
	}

	sibp->uid = gi;
	if (sip != NULL) {
		sibp->sip = SeqIdDup (sip);
		sibp->touch = GetSecs ();
	}
	vnp->data.ptrvalue = (Pointer) sibp;

	if (seqidgicache == NULL) {
		seqidgicache = vnp;
		goto ret;
	}

	seqidgiarray = MemFree (seqidgiarray);

	prev = (ValNodePtr PNTR) (&seqidgicache);
	tmp = seqidgicache;
	while (tmp != NULL) {
		sibp = (SeqIdBlockPtr) tmp->data.ptrvalue;
		if (sibp != NULL) {
			if (sibp->uid > gi) {
				if (prev != NULL) {
					vnp->next = *prev;
					*prev = vnp;
				}
				goto ret;
			} else if (sibp->uid == gi) {
				goto ret;
			} else {
				prev = (ValNodePtr PNTR) (& (tmp->next));
			}
		} else {
			prev = (ValNodePtr PNTR) (& (tmp->next));
		}
		tmp = tmp->next;
	}
	if (prev != NULL) {
		vnp->next = *prev;
		*prev = vnp;
	}
ret:
	retval = NlmRWunlock(sgi_RWlock);
	if (retval != 0)
	{
		ErrPostEx(SEV_ERROR,0,0,"RecSeqIdGiUnlock: RWunlock error [%ld]",
			(long)retval);
	}
	return;
}

/* trim list by sorting older nodes to end of list if list grew too large */

static int LIBCALLBACK SortSeqIdGiCacheTime (VoidPtr ptr1, VoidPtr ptr2)

{
  SeqIdBlockPtr  sibp1;
  SeqIdBlockPtr  sibp2;
  ValNodePtr     vnp1;
  ValNodePtr     vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  sibp1 = (SeqIdBlockPtr) vnp1->data.ptrvalue;
  sibp2 = (SeqIdBlockPtr) vnp2->data.ptrvalue;
  if (sibp1 == NULL || sibp2 == NULL) return 0;
  if (sibp1->touch > sibp2->touch) {
     return -1;
  } else if (sibp1->touch < sibp2->touch) {
    return 1;
  }
  return 0;
}

NLM_EXTERN Boolean FetchFromSeqIdGiCache (Int4 gi, SeqIdPtr PNTR sipp)

{
	ValNodePtr vnp;
	SeqIdBlockPtr sibp = NULL;
	Int2 i;
	Int2 left, right, mid;
	Int4 compare, ret;
	Boolean done = FALSE;

	if (sipp != NULL) {
	  *sipp = NULL;
	}
	if (seqidgicache == NULL) return done;

	if (seqidgiarray == NULL) {
	    ret = NlmRWwrlock(sgi_RWlock);
		if (ret != 0)
		{
			ErrPostEx(SEV_ERROR,0,0,"SeqIdGi: RWwrlock error [%ld]",
				(long)ret);
			return done;
		}

		if (seqidgiarray == NULL)
		{
			seqidcount = (Int2) ValNodeLen (seqidgicache);

			/* if list is too large, sort by touch time, cut least recently used ids */

			if (seqidcount > 32000) {
			  seqidgicache = ValNodeSort (seqidgicache, SortSeqIdGiCacheTime);
			  for (vnp = seqidgicache; vnp != NULL && seqidcount > 24000;
			       vnp = vnp->next, seqidcount--) continue;
			  if (vnp != NULL) {
			    vnp->next = ValNodeFreeData (vnp->next);
			  }
			}

			seqidcount = (Int2) ValNodeLen (seqidgicache);
			seqidgiarray = MemNew (sizeof (ValNodePtr) * (size_t) (seqidcount + 1));
			if (seqidgiarray != NULL) {
				i = 0;
				for (vnp = seqidgicache; vnp != NULL; vnp = vnp->next) {
					seqidgiarray [i] = vnp;
					i++;
				}
			}
		}
		ret = NlmRWunlock(sgi_RWlock);
		if (ret != 0)
		{
			ErrPostEx(SEV_ERROR,0,0,"SeqIdGiUnlock: RWunlock error [%ld]",
				(long)ret);
			return done;
		}

	}

	ret = NlmRWrdlock(sgi_RWlock);
    if (ret != 0)
	{
		ErrPostEx(SEV_ERROR,0,0,"SeqIdGi: RWrdlock error [%ld]",
			(long)ret);
		return done;
	}

	if (seqidgiarray != NULL) {
		left = 1;
		right = seqidcount;
		while (left <= right) {
			mid = (left + right) / 2;
			compare = 0;
			vnp = seqidgiarray [mid - 1];
			if (vnp != NULL) {
				sibp = (SeqIdBlockPtr) vnp->data.ptrvalue;
				if (sibp != NULL) {
					compare = gi - sibp->uid;
				}
			}
			if (compare <= 0) {
				right = mid - 1;
			}
			if (compare >= 0) {
				left = mid + 1;
			}
		}
		if (left > right + 1 && sibp != NULL) {
			if (sibp->sip != NULL) {
				if (sipp != NULL) {
					*sipp = SeqIdDup (sibp->sip);
				}
				sibp->touch = GetSecs ();
			}
			done = TRUE;
		}
	}


	ret = NlmRWunlock(sgi_RWlock);
	if (ret != 0)
	{
		ErrPostEx(SEV_ERROR,0,0,"SeqIdGiUnlock: RWunlock error [%ld]",
			(long)ret);
	}

	return done;
}

NLM_EXTERN SeqIdPtr LIBCALL GetSeqIdForGI (Int4 gi)
{
	BioseqPtr bsp = NULL;
	ObjMgrProcPtr ompp;
	OMProcControl ompc;
	Int2 ret;
	SeqIdPtr sip, sip2=NULL, otherh=NULL, otherl = NULL, gb=NULL;
	ValNode vn;


	if (gi <= 0)
		return sip2;

	vn.choice = SEQID_GI;
	vn.data.intvalue = gi;
	vn.next = NULL;

	bsp = BioseqFindCore(&vn);
	if (bsp != NULL)
	{
		for (sip = bsp->id; sip != NULL; sip = sip->next)
		{
		    switch (sip->choice) 
		    {
		        case SEQID_LOCAL:           /* object id */
      		  case SEQID_GIBBSQ:         
		        case SEQID_GIBBMT:
      		  case SEQID_PATENT:
		        case SEQID_GENERAL:
						otherl = sip;
						break;
					case SEQID_GI:
					   break;
		        case SEQID_GENBANK:
      		  case SEQID_EMBL:
		        case SEQID_PIR:
      		  case SEQID_SWISSPROT:
		        case SEQID_DDBJ:
				case SEQID_PRF:
				  case SEQID_PDB:
				  case SEQID_TPG:
				  case SEQID_TPE:
				  case SEQID_TPD:
						gb = sip;
						break;
					default:
						if (otherh == NULL)
							otherh = sip;
						break;
		    }
		}
	}


	if (gb != NULL)
		sip2 = gb;
	else if (otherh != NULL)
		sip2 = otherh;
	else if (otherl != NULL)
		sip2 = otherl;

	if (sip2 != NULL)
		return SeqIdDup(sip2);

	if (FetchFromSeqIdGiCache (gi, &sip2)) {
		return sip2;
	}

	ompp = NULL;
	while ((ompp = ObjMgrProcFindNext(NULL, OMPROC_FETCH, OBJ_SEQID, OBJ_SEQID, ompp)) != NULL)
	{
		if ((ompp->subinputtype == SEQID_GI) && (ompp->suboutputtype == 0))
		{
			MemSet((Pointer)(&ompc), 0, sizeof(OMProcControl));
			ompc.input_data = &vn;
			ompc.proc = ompp;
			ret = (* (ompp->func))((Pointer)&ompc);
			switch (ret)
			{
				case OM_MSG_RET_ERROR:
					ErrShow();
					break;
				case OM_MSG_RET_DEL:
					break;
				case OM_MSG_RET_OK:
					break;
				case OM_MSG_RET_DONE:
					sip2 = (SeqIdPtr)(ompc.output_data);
					if (sip2 != NULL)
                                          {
                                            RecordInSeqIdGiCache (gi, sip2);
                                            return sip2;
                                          }
					break;
				default:
					break;
			}
		}
	}

	RecordInSeqIdGiCache (gi, sip2);
	return sip2;
}

/*****************************************************************************
*
*   SeqEntryFind(sip)
*   	returns top level seqentry for sip
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr LIBCALL SeqEntryFind (SeqIdPtr sid)
{
	BioseqPtr bsp;
	ObjMgrDataPtr omdp;
	ObjMgrDataPtr PNTR omdpp;
	SeqEntryPtr result=NULL;
	SeqSubmitPtr ssp;
	Int4 i;
	ObjMgrPtr omp;

	bsp = BioseqFind(sid);
	if (bsp == NULL) return result;

	omp = ObjMgrReadLock();
	omdpp = omp->datalist;

	i = ObjMgrLookup(omp, (Pointer)bsp);
	if (i < 0) {
            Char tmpbuff[256];
            
            SeqIdWrite(bsp->id, tmpbuff, 
                       PRINTID_FASTA_LONG, sizeof(tmpbuff));
            
            ErrPostEx(SEV_WARNING, 0, __LINE__, 
                      "ObjMgrLookup() returned negative value "
                      "id = %s, totobj = %d, currobj = %d, "
                      "HighestEntityID = %d", tmpbuff, 
                      omp->totobj,
                      omp->currobj, omp->HighestEntityID);
            ObjMgrUnlock();
            return result;
        }

	omdp = omdpp[i];
	while (omdp->parentptr != NULL)
	{
		i = ObjMgrLookup(omp, (omdp->parentptr));
		if (i < 0) {
                    Char tmpbuff[256];
                    
                    SeqIdWrite(bsp->id, tmpbuff, 
                               PRINTID_FASTA_LONG, sizeof(tmpbuff));
                    
                    ErrPostEx(SEV_WARNING, 0, __LINE__, 
                              "ObjMgrLookup() returned negative value "
                              "id = %s, totobj = %d, currobj = %d, "
                              "HighestEntityID = %d", tmpbuff, 
                              omp->totobj,
                              omp->currobj, omp->HighestEntityID);
                    ObjMgrUnlock();
                    return result;
                }
		omdp = omdpp[i];
	}

	if (omdp->datatype == OBJ_SEQSUB) {
		ssp = (SeqSubmitPtr) omdp->dataptr;
		if (ssp != NULL && ssp->datatype == 1) {
			result = (SeqEntryPtr) ssp->data;
		}
	} else {
		result = omdp->choice;
	}
	ObjMgrUnlock();
	return result;
}

/*****************************************************************************
*
*   BioseqContextPtr BioseqContextNew (bsp)
*
*****************************************************************************/
NLM_EXTERN BioseqContextPtr LIBCALL BioseqContextNew (BioseqPtr bsp)
{
	ObjMgrDataPtr omdp;
	ObjMgrDataPtr PNTR omdpp;
	Int4	i;
	Int2    ctr=0;
	SeqEntryPtr seps[BIOSEQCONTEXTMAX];
	BioseqContextPtr bcp;
	ObjMgrPtr omp;

	if (bsp == NULL)
		return NULL;


	bcp = MemNew(sizeof(BioseqContext));
	bcp->bsp = bsp;
	bcp->se.choice = 1;   /* bioseq */
	bcp->se.data.ptrvalue = bsp;

	omp = ObjMgrReadLock();
	if (omp == NULL) return BioseqContextFree(bcp);
	omdpp = omp->datalist;

	i = ObjMgrLookup(omp, (Pointer)bsp);
	if (i < 0) {
            Char tmpbuff[256];
            
            SeqIdWrite(bsp->id, tmpbuff, 
                       PRINTID_FASTA_LONG, sizeof(tmpbuff));
            
            ErrPostEx(SEV_WARNING, 0, __LINE__, 
                      "ObjMgrLookup() returned negative value "
                      "id = %s, totobj = %d, currobj = %d, "
                      "HighestEntityID = %d", tmpbuff, 
                      omp->totobj,
                      omp->currobj, omp->HighestEntityID);
            ObjMgrUnlock();
            return NULL;
        }
	omdp = omdpp[i];

	if (omdp->choice != NULL)
	{
		seps[ctr] = omdp->choice;
		ctr++;

		while (omdp->parentptr != NULL)
		{
			i = ObjMgrLookup(omp, (omdp->parentptr));
			if (i < 0) {
                            Char tmpbuff[256];
                            
                            SeqIdWrite(bsp->id, tmpbuff, 
                                       PRINTID_FASTA_LONG, sizeof(tmpbuff));
                            
                            ErrPostEx(SEV_WARNING, 0, __LINE__, 
                                      "ObjMgrLookup() returned negative value "
                                      "id = %s, totobj = %d, currobj = %d, "
                                      "HighestEntityID = %d", tmpbuff, 
                                      omp->totobj,
                                      omp->currobj, omp->HighestEntityID);
                            ObjMgrUnlock();
                            return NULL;
                        }
			omdp = omdpp[i];
			if (omdp->choice != NULL)
			{
				if (ctr == BIOSEQCONTEXTMAX)
					ErrPostEx(SEV_ERROR, 0,0, "BioseqContextNew: more than %d levels",(int)ctr);
				else
				{
					seps[ctr] = omdp->choice;
					ctr++;
			    }
			}
		}

		bcp->count = ctr;
		for (i = 0; i < bcp->count; i++)
		{
			ctr--;
			bcp->context[i] = seps[ctr];
		}
	}

	if (omdp->tempload == TL_CACHED)
	{
		ErrPostEx(SEV_ERROR,0,0,"BioseqContextNew: bsp is TL_CACHED");
		bcp = BioseqContextFree(bcp);
	}

	ObjMgrUnlock();

	return bcp;
}

/*****************************************************************************
*
*   BioseqContextFree(bcp)
*
*****************************************************************************/
NLM_EXTERN BioseqContextPtr LIBCALL BioseqContextFree(BioseqContextPtr bcp)
{
	return MemFree(bcp);
}

/*****************************************************************************
*
*   BioseqContextGetSeqDescr(bcp, type, curr, SeqEntryPtr PNTR sep)
*       returns pointer to the next SeqDescr of this type
*       type gives type of Seq-descr
*       if (type == 0)
*          get them all
*       curr is NULL or previous node of this type found
*       moves up from bsp
*		if (sep != NULL) sep set to SeqEntryPtr containing the SeqDescr.
*
*****************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL BioseqContextGetSeqDescr (BioseqContextPtr bcp, Int2 type, ValNodePtr curr, SeqEntryPtr PNTR the_sep)    /* the last one you used */
{
	Int2 i;
	ValNodePtr tmp = NULL;
	Boolean found = FALSE;
	BioseqPtr bsp;
	BioseqSetPtr bssp;

	if (bcp == NULL) return NULL;
	
	if (the_sep != NULL)
		*the_sep = NULL;
		
	if (bcp->count == 0)   /* just a Bioseq */
	{
		tmp = BioseqGetSeqDescr(bcp->bsp, type, curr);
		if (the_sep != NULL) *the_sep = bcp->context[1];
		return tmp;
	}

	i = bcp->count - 1;
	if (curr != NULL)   /* find where we are */
	{
		while ((i >= 0) && (! found))
		{
			if (IS_Bioseq(bcp->context[i]))
			{
				bsp = (BioseqPtr)((bcp->context[i])->data.ptrvalue);
				tmp = bsp->descr;
			}
			else
			{
				bssp = (BioseqSetPtr)((bcp->context[i])->data.ptrvalue);
				tmp = bssp->descr;
			}
			while ((tmp != curr) && (tmp != NULL))
				tmp = tmp->next;
			if (tmp == curr)
			{
				found = TRUE;
				tmp = tmp->next;
			}
			else
				i--;
		}
		if (! found)   /* can't find it! */
			return NULL;
	}
	else              /* get first one */
	{
		tmp = bcp->bsp->descr;
	}
		
	while (i >= 0)
	{
		while (tmp != NULL)
		{
			if ((! type) || ((Int2)(tmp->choice) == type))
			{
				if (the_sep != NULL) *the_sep = bcp->context[i];
				return tmp;
			}
			tmp = tmp->next;
		}
		i--;
		if (i >= 0)
		{
			if (IS_Bioseq(bcp->context[i]))
			{
				bsp = (BioseqPtr)((bcp->context[i])->data.ptrvalue);
				tmp = bsp->descr;
			}
			else
			{
				bssp = (BioseqSetPtr)((bcp->context[i])->data.ptrvalue);
				tmp = bssp->descr;
			}
		}
	}
    return NULL;
}

/*****************************************************************************
*
*   BioseqContextGetSeqFeat(bcp, type, curr, sapp)
*       returns pointer to the next Seq-feat of this type
*       type gives type of Seq-descr
*       if (type == 0)
*          get them all
*       curr is NULL or previous node of this type found
*       moves up from bsp
*   	if (sapp != NULL) is filled with SeqAnnotPtr containing the SeqFeat
*   	in:
*   		0 = sfp->location only
*   		1 = sfp->product only
*   		2 = either of above
*
*****************************************************************************/
NLM_EXTERN SeqFeatPtr LIBCALL BioseqContextGetSeqFeat (BioseqContextPtr bcp, Int2 type,
	SeqFeatPtr curr, SeqAnnotPtr PNTR sapp, Int2 in)    /* the last one you used */
{
	SeqEntryPtr sep;

	if (bcp == NULL) return NULL;
	
	if (sapp != NULL)
		*sapp = NULL;

	if (bcp->count == 0)    /* just a BioseqSeq */
		sep = &(bcp->se);
	else
		sep = bcp->context[0];

	return SeqEntryGetSeqFeat (sep, type, curr, sapp, in, bcp->bsp);
}

typedef struct smgetseqfeat {
	Boolean hit;
	SeqFeatPtr last,
		this;
	SeqAnnotPtr sap;
	SeqLocPtr slp1, slp2;
	Int2 in, type;
} SMGetSeqFeat, PNTR GetSeqFeatPtr;

NLM_EXTERN void GetSeqFeatCallback (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);

/*****************************************************************************
*
*   SeqEntryGetSeqFeat(sep, type, curr, sapp)
*       returns pointer to the next Seq-feat of this type
*       type gives type of SeqFeat
*       if (type == 0)
*          get them all
*       curr is NULL or previous node of this type found
*       moves up from bsp
*   	if (sapp != NULL) is filled with SeqAnnotPtr containing the SeqFeat
*       if (bsp != NULL) then for that Bioseq match on location by "in"
*   	in:
*   		0 = sfp->location only
*   		1 = sfp->product only
*   		2 = either of above
*
*****************************************************************************/
NLM_EXTERN SeqFeatPtr LIBCALL SeqEntryGetSeqFeat (SeqEntryPtr sep, Int2 type,
	SeqFeatPtr curr, SeqAnnotPtr PNTR sapp, Int2 in, BioseqPtr bsp)    /* the last one you used */
{
	SMGetSeqFeat gsf;
	ValNode vn1, vn2;
	
	if (sep == NULL) return NULL;
	
	if (sapp != NULL)
		*sapp = NULL;

	if (curr == NULL)
		gsf.hit = TRUE;
	else
		gsf.hit = FALSE;
	gsf.last = curr;
	gsf.this = NULL;
	gsf.sap = NULL;
	gsf.type = type;
	gsf.in = in;
	if (bsp != NULL)   /* matching by Bioseq */
	{
		if ((bsp->repr == Seq_repr_seg) || (bsp->repr == Seq_repr_ref))
		{
			vn2.choice = SEQLOC_MIX;
			vn2.data.ptrvalue = bsp->seq_ext;
			gsf.slp2 = (SeqLocPtr)(&vn2);
		}
		else
			gsf.slp2 = NULL;

		vn1.choice = SEQLOC_WHOLE;
		vn1.data.ptrvalue = (Pointer) SeqIdFindBest (bsp->id, 0);
		gsf.slp1 = (SeqLocPtr)(&vn1);
	}
	else
		gsf.slp1 = NULL;

	SeqEntryExplore (sep, (Pointer)(&gsf), GetSeqFeatCallback);

	if (sapp != NULL)
		*sapp = gsf.sap;

	return gsf.this;
}

NLM_EXTERN void GetSeqFeatCallback (SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent)
{
	GetSeqFeatPtr gsfp;
	BioseqPtr bsp;
	BioseqSetPtr bssp;
	SeqAnnotPtr sap;
	SeqFeatPtr sfp, last;
	Boolean hit, gotit = FALSE;
	Uint1 type;
	SeqLocPtr slp1, slp2, slp;
	Int2 i, in, retval;

	gsfp = (GetSeqFeatPtr)data;
	if (gsfp->this != NULL)   /* got it */
		return;

	last = gsfp->last;
	hit = gsfp->hit;
	type = (Uint1)(gsfp->type);
	if (gsfp->slp1 != NULL)   /* matching by Bioseq */
	{
		slp1 = gsfp->slp1;
		slp2 = gsfp->slp2;
		in = gsfp->in;
	}
	else
		slp1 = NULL;

	if (IS_Bioseq(sep))
	{
		bsp = (BioseqPtr)(sep->data.ptrvalue);
		sap = bsp->annot;
	}
	else
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		sap = bssp->annot;
	}

	while (sap != NULL)
	{
		if (sap->type == 1)  /* feature table */
		{
			for (sfp = (SeqFeatPtr)(sap->data); sfp != NULL; sfp = sfp->next)
			{
				if (! hit)	   /* still looking */
				{
					if (sfp == last)
					{
						hit = TRUE;
						gsfp->hit = TRUE;
					}
				}
				else
				{
					if ((! type) || (type == sfp->data.choice))
					{
						if (slp1 != NULL)   /* look for feats on a bioseq */
						{
							for (i = 0; i < 2; i++)
							{
								if ((i == 0) && (in != 1))
									slp = sfp->location;
								else if ((i==1) && (in != 0))
									slp = sfp->product;
								else
									slp = NULL;
								if (slp != NULL)
								{
									retval = SeqLocCompare(slp, slp1);
									if (retval)
									{
										gotit = TRUE;
										break;
									}

									if (slp2 != NULL)
									{
										retval = SeqLocCompare(slp, slp2);
										if (retval)
										{
											gotit = TRUE;
											break;
										}
									}
								}
							}
						}
						else
							gotit = TRUE;
						if (gotit)
						{
							gsfp->this = sfp;
							gsfp->sap = sap;
							return;
						}
					}
				}
			}
		}

		sap = sap->next;
	}

	return;
}

/*****************************************************************************
*
*   BioseqContextGetTitle(bcp)
*     returns first title for Bioseq in this context
*
*****************************************************************************/
NLM_EXTERN CharPtr LIBCALL BioseqContextGetTitle(BioseqContextPtr bcp)
{
	CharPtr title = NULL;
	ValNodePtr vnp;

	vnp = BioseqContextGetSeqDescr(bcp, Seq_descr_title, NULL, NULL);
	if (vnp != NULL)
		title = (CharPtr)vnp->data.ptrvalue;
	return title;
}

/*****************************************************************************
*
*   SeqMgr Functions
*
*****************************************************************************/

/*****************************************************************************
*
*   SeqMgrSeqEntry(type, data, sep)
*   	Adds the SeqEntryPtr pointing directly to this Bioseq or BioseqSet
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrSeqEntry (Uint1 type, Pointer data, SeqEntryPtr sep)
{
	return ObjMgrSetChoice (OBJ_SEQENTRY, sep, data);
}

/*****************************************************************************
*
*   SeqMgrGetSeqEntryForData(data)
*   	returns SeqEntryPtr for a BioseqPtr or BioseqSetPtr
*       sep must have been put in SeqMgr using SeqMgrSeqEntry
*       the Bioseq/BioseqSets it is a part of must also be in SeqMgr
*       returns NULL on failure.
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr LIBCALL SeqMgrGetSeqEntryForData (Pointer data)
{
	return ObjMgrGetChoiceForData(data);
}

/*****************************************************************************
*
*   SeqMgrGetEntityIDForSeqEntry(sep)
*   	returns the EntityID for a SeqEntryPtr
*       sep must have been put in SeqMgr using SeqMgrSeqEntry
*       the Bioseq/BioseqSets it is a part of must also be in SeqMgr
*       This function will move up to the top of the SeqEntry tree it may be
*          in. If top level EntityID is 0, one is assigned at this point.
*       If an element is moved under a different hierarchy, its EntityID will
*          change.
*       returns 0 on failure.
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL SeqMgrGetEntityIDForSeqEntry (SeqEntryPtr sep)
{
	return ObjMgrGetEntityIDForChoice (sep);
}

/*****************************************************************************
*
*   SeqMgrGetSeqEntryForEntityID (id)
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr LIBCALL SeqMgrGetSeqEntryForEntityID (Int2 id)
{
	return ObjMgrGetChoiceForEntityID (id);
}

/*****************************************************************************
*
*   SeqMgrSetBSFetchTop (fetch, data)
*   	sets the BSFetchTop routine to "fetch"
*       returns previous value
*       set to NULL to turn off all fetching for that type
*
*       Current value can be called directly as BioseqFetch();
*   	Default is
*   		1) looks in memory
*   		2) looks locally if LocalBSFetch is set
*			3) looks remotely if RemoteBSFetch is set
*
*****************************************************************************/
NLM_EXTERN BSFetchTop LIBCALL SeqMgrSetBSFetchTop (BSFetchTop fetch, Pointer data)
{
	SeqMgrPtr smp;
	BSFetchTop tmp = NULL;

	smp = SeqMgrWriteLock();
	if (smp == NULL) return tmp;
	
	tmp = smp->bsfetch;
	smp->bsfetch = fetch;
	smp->TopData = data;
	SeqMgrUnlock();
	return tmp;
}

/*****************************************************************************
*
*   SeqMgrSetFetchOnLock(value)
*   	if value = TRUE, manager will try to fetch the bioseq if not in
*          memory, when BioseqLock is called
*   	if FALSE, BioseqLock will only look in memory
*       returns previous value of fetch_on_lock
*       default is to fetch_on_lock
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrSetFetchOnLock (Boolean value)
{
	Boolean tmp=FALSE;
	SeqMgrPtr smp;

	smp = SeqMgrWriteLock();
	if (smp == NULL) return tmp;

	tmp = smp->fetch_on_lock;
	smp->fetch_on_lock = value;
	SeqMgrUnlock();
	return tmp;
}

/*****************************************************************************
*
*   SeqMgrLinkSeqEntry(sep, parenttype, parentptr)
*      connects all component seq-entries by traversing the linked list
*        all calling SeqMgrConnect and SeqMgrSeqEntry appropriately
*        if parenttype != 0, then assumes seqentry is contained in parentptr
*           and should be connected to it
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrLinkSeqEntry (SeqEntryPtr sep, Uint2 parenttype, Pointer parentptr)
{
	SeqEntryPtr sep2;
	BioseqSetPtr bssp;
	Uint2 the_type;
	
	if (sep == NULL)
		return FALSE;

	if (IS_Bioseq(sep))
		the_type = OBJ_BIOSEQ;
	else
		the_type = OBJ_BIOSEQSET;

	SeqMgrSeqEntry((Uint1)the_type, sep->data.ptrvalue, sep);

	/**** if (parenttype != 0) ****/
	ObjMgrConnect(the_type, sep->data.ptrvalue, parenttype, parentptr);

	if (! IS_Bioseq(sep))
	{
		bssp = (BioseqSetPtr)(sep->data.ptrvalue);
		for (sep2 = bssp->seq_set; sep2 != NULL; sep2 = sep2->next)
		{
			SeqMgrLinkSeqEntry (sep2, the_type, sep->data.ptrvalue);
		}
	}
	return TRUE;
}
/*****************************************************************************
*
*   Selection Functions for data objects based on SeqLoc
*      See also general selection in objmgr.h
*
*****************************************************************************/

/*****************************************************************************
*
*   SeqMgrSelect(region)
*      region is a SeqLocPtr
*          It can only apply to one Bioseq
*          selected area will be extreme left and right ends
*          fuzziness is ignored
*      if something else selected, deselects it first, then selects requested
*        item
*      to select without deselecting something else, use SeqMgrAlsoSelect()
*      returns TRUE if item is now currently selected, FALSE if not
*      "region" is always copied. Caller is responsible for managment of
*         SeqLoc that is passed in.
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrSelect (SeqLocPtr region)
{
	return SeqMgrGenericSelect(region, 1, NULL);
}

NLM_EXTERN Boolean LIBCALL SeqMgrAlsoSelect (SeqLocPtr region)
{
	return SeqMgrGenericSelect(region, 2, NULL);
}

/*****************************************************************************
*
*   SeqMgrDeSelect(region)
*   	if this item was selected, then deselects and returns TRUE
*   	else returns FALSE
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrDeSelect (SeqLocPtr region)
{
	return SeqMgrGenericSelect(region, 3, NULL);
}

/*****************************************************************************
*
*   SeqMgrSetColor(region, rgb)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrSetColor (SeqLocPtr region, Uint1Ptr rgb)
{
	if (rgb == NULL) return FALSE;
        return SeqMgrGenericSelect(region, 4, rgb);
}

static Boolean NEAR SeqMgrGenericSelect (SeqLocPtr region, Int2 type,
                                           Uint1Ptr rgb)
{
	SeqInt si;
	ValNode vn;
	SeqIdPtr sip;
	Uint2 entityID;
	Uint2 itemID;

	if (region == NULL) return FALSE;

	sip = SeqLocId(region);
	if (sip == NULL) return FALSE;

	entityID = BioseqFindEntity(sip, &itemID);
	if (entityID == 0) return FALSE;

	MemSet((Pointer)(&si), 0, sizeof(SeqInt));
	MemSet((Pointer)(&vn), 0, sizeof(ValNode));

	si.id = sip;
	si.from = SeqLocStart(region);
	si.to = SeqLocStop(region);
	si.strand = SeqLocStrand(region);

	if ((si.from < 0) || (si.to < 0) || (si.from > si.to)) return FALSE;

	vn.choice = SEQLOC_INT;
	vn.data.ptrvalue = &si;

	switch (type)
	{
		case 1:
			return ObjMgrSelect(entityID, itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC, &vn);
		case 2:
			return ObjMgrAlsoSelect(entityID, itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC, &vn);
		case 3:
			return ObjMgrDeSelect(entityID, itemID, OBJ_BIOSEQ, OM_REGION_SEQLOC, &vn);
		case 4:
			return ObjMgrSetColor(entityID, itemID, OBJ_BIOSEQ,
                                 OM_REGION_SEQLOC, &vn, rgb);
		default:
			break;
	}

	return FALSE;
}

/*****************************************************************************
*
*   SpreadGapsInDeltaSeq(BioseqPtr bsp)
*      bsp must be a delta seq
*      function counts deltas with known lengths ( = known_len)
*               counts deltas which are gaps of unknown length ( = unk_count)
*                  these can delta of length 0, delta with fuzz = lim (unk),
*                    or SEQLOC_NULL
*               converts all unknown gaps to delta with fuzz = lim(unk)
*               sets length of all unknown gaps to
*                  (bsp->length - known_len)/unk_count
*                  any reminder spread over first few gaps
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SpreadGapsInDeltaSeq (BioseqPtr bsp)
{
	Boolean retval = FALSE;
	Int4 known_len = 0,
		 total_gap, gap_len,
		 unk_count = 0,
		 remainder;
	DeltaSeqPtr dsp;
	SeqLocPtr slocp;
	SeqLitPtr slp;
	IntFuzzPtr ifp;

	if (bsp == NULL) return retval;
	if ((bsp->repr != Seq_repr_delta) || (bsp->seq_ext == NULL))
		return retval;

	retval = TRUE;  /* can function */

	for (dsp = (DeltaSeqPtr)(bsp->seq_ext); dsp != NULL; dsp = dsp->next)
	{
		switch (dsp->choice)
		{
			case 1:	  /* SeqLocPtr */
				slocp = (SeqLocPtr)(dsp->data.ptrvalue);
				if (slocp == NULL) break;
				if (slocp->choice == SEQLOC_NULL)  /* convert it */
				{
					SeqLocFree(slocp);
					slp = SeqLitNew();
					dsp->choice = 2;
					dsp->data.ptrvalue = slp;
					ifp = IntFuzzNew();
					slp->fuzz = ifp;
					ifp->choice = 4;   /* lim - type unk */
					unk_count++;
				}
				else                               /* count length */
					known_len += SeqLocLen(slocp);
				break;
			case 2:   /* SeqLitPtr */
				slp = (SeqLitPtr)(dsp->data.ptrvalue);
				if (slp == NULL) break;
				if (slp->seq_data != NULL)         /* not a gap */
				{
					known_len += slp->length;
					break;
				}
				ifp = slp->fuzz;
				if (slp->length == 0)  /* unknown length */
				{
					unk_count++;
					if (ifp != NULL)
					{
						if (ifp->choice != 4)  /* not lim */
							ifp = IntFuzzFree(ifp);
						else if (ifp->a != 0)  /* not unk */
							ifp = IntFuzzFree(ifp);
					}
					if (ifp == NULL)
					{
						ifp = IntFuzzNew();
						ifp->choice = 4; /* lim - unk */
						slp->fuzz = ifp;
					}
				}
				else                      /* gap length was set */
				{
					if (ifp == NULL)  /* no fuzz - count length */
						known_len += slp->length;
					else              /* might be a guess */
					{
						if ((ifp->choice == 4) && (ifp->a == 0)) /* lim - unk */
							unk_count++;
						else
							known_len += slp->length;
					}
				}
				break;
			default:
				break;
		}

	}

	if (unk_count == 0)   /* no unknown gaps */
		return retval;

	total_gap = bsp->length - known_len;
	if (total_gap < 0)
		total_gap = 0;
	gap_len = total_gap / unk_count;
	remainder = total_gap - (gap_len * unk_count);

	for (dsp = (DeltaSeqPtr)(bsp->seq_ext); dsp != NULL; dsp = dsp->next)
	{
		switch (dsp->choice)
		{
			case 1:	  /* SeqLocPtr */
				break;
			case 2:   /* SeqLitPtr */
				slp = (SeqLitPtr)(dsp->data.ptrvalue);
				if (slp == NULL) break;
				if (slp->seq_data != NULL) break;
				ifp = slp->fuzz;
				if (ifp == NULL) break;
				if ((ifp->choice != 4) || (ifp->a != 0))
					break;
				slp->length = gap_len;
				if (remainder)
				{
					slp->length++;
					remainder--;
				}
				break;
			default:
				break;
		}
	}

	return retval;
}

/*****************************************************************************
*
*   CountGapsInDeltaSeq(BioseqPtr bsp, &num_segs, &num_gaps, &known_residues, &num_gaps_faked)
*      bsp must be a delta seq
*      function counts deltas and returns a profile
*          num_segs = total number of segments
*          num_gaps = total number of segments representing gaps
*          known_residues = number of real residues in the sequence (not gaps)
*          num_gaps_faked = number of gaps where real length is not known, but where
*                           a length was guessed by spreading the total gap length
*                           out over all gaps evenly.
*
*      NOTE: any of these pointers except bsp can be NULL
*
*      returns TRUE if values in argument were set.
*
*****************************************************************************/
static Boolean NextLitLength (DeltaSeqPtr next, Int4Ptr lenp)

{
  SeqLitPtr  slp;

  if (lenp == NULL) return FALSE;
  *lenp = 0;
  if (next == NULL || next->choice != 2) return FALSE;
  slp = (SeqLitPtr) next->data.ptrvalue;
  if (slp == NULL || slp->seq_data == NULL) return FALSE;
  *lenp = slp->length;
  return TRUE;
}

NLM_EXTERN Boolean LIBCALL CountGapsInDeltaSeq (BioseqPtr bsp, Int4Ptr num_segs, Int4Ptr num_gaps, Int4Ptr known_residues, Int4Ptr num_gaps_faked, CharPtr buf, Int4 buflen)
{
	Boolean retval = FALSE;
	Int4 residues = 0,
		segs = 0,
		gaps = 0,
		len = 0,
		fake_gaps = 0,
		from = 0, 
		tlen = 0,
		nxtlen;
	DeltaSeqPtr dsp, next;
	SeqLocPtr slocp;
	SeqLitPtr slp;
	IntFuzzPtr ifp;
	Boolean unk;
	static Char tmp[128];
	Int2 diff, blen;

	if (bsp == NULL) return retval;
	if ((bsp->repr != Seq_repr_delta) || (bsp->seq_ext == NULL))
		return retval;

	retval = TRUE;  /* can function */


	for (dsp = (DeltaSeqPtr)(bsp->seq_ext); dsp != NULL; dsp = next)
	{
		next = dsp->next;
		segs++;
		from = len + 1;
		switch (dsp->choice)
		{
			case 1:	  /* SeqLocPtr */
				slocp = (SeqLocPtr)(dsp->data.ptrvalue);
				if (slocp == NULL) break;
				if (slocp->choice == SEQLOC_NULL)  /* gap */
				{
					gaps++;
					sprintf(tmp, "* %ld %ld gap of unknown length~", (long) from, (long) len);
					blen = (Int2) MIN ((Int4) buflen, (Int4) sizeof (tmp));
					diff = LabelCopy(buf, tmp, blen);
					buflen -= diff;
					buf += diff;
				}
				else {                              /* count length */
					residues += SeqLocLen(slocp);
					if (buf != NULL) {
						tlen =  SeqLocLen(slocp);
						len  += tlen;
						sprintf(tmp, "* %8ld %8ld: contig of %ld bp in length~", (long) from, (long) len, (long) tlen);
						blen = (Int2) MIN ((Int4) buflen, (Int4) sizeof (tmp));
						diff = LabelCopy(buf, tmp, blen);
						buflen -= diff;
						buf += diff;
					}
				}
				break;
			case 2:   /* SeqLitPtr */
				slp = (SeqLitPtr)(dsp->data.ptrvalue);
				if (slp == NULL) break;
				tlen =  slp->length;
				len  += tlen;
				if (slp->seq_data != NULL)
				{
					residues += slp->length;
					while (NextLitLength (next, &nxtlen)) {
						tlen += nxtlen;
						len  += nxtlen;
						residues += nxtlen;
						next = next->next;
					}
					if (buf) {
						sprintf(tmp, "* %8ld %8ld: contig of %ld bp in length~", (long) from, (long) len, (long) tlen);
						blen = (Int2) MIN ((Int4) buflen, (Int4) sizeof (tmp));
						diff = LabelCopy(buf, tmp, blen);
						buflen -= diff;
						buf += diff;
					}
				}
				else
				{
					unk = FALSE;
					gaps++;
					ifp = slp->fuzz;
					if (ifp != NULL)
					{
						if ((ifp->choice == 4) && (ifp->a == 0)) {
							unk = TRUE;
							fake_gaps++;
							if (buf) {
								if (from > len) {
								sprintf(tmp, "*                    gap of unknown length~");
								} else {
								sprintf(tmp, "* %8ld %8ld: gap of unknown length~", (long) from, (long) len);
								}
								blen = (Int2) MIN ((Int4) buflen, (Int4) sizeof (tmp));
								diff = LabelCopy(buf, tmp, blen);
								buflen -= diff;
								buf += diff;
							}
						}
					}
					if (!unk && buf) {
						sprintf(tmp, "* %8ld %8ld: gap of %ld bp~", (long) from, (long) len, (long) tlen);
						blen = (Int2) MIN ((Int4) buflen, (Int4) sizeof (tmp));
						diff = LabelCopy(buf, tmp, blen);
						buflen -= diff;
						buf += diff;
					}
				}
				break;
			default:
				break;
		}
	}

	if (num_segs != NULL)
		*num_segs = segs;
	if (num_gaps != NULL)
		*num_gaps = gaps;
	if (known_residues != NULL)
		*known_residues = residues;
	if (num_gaps_faked != NULL)
		*num_gaps_faked = fake_gaps;

	return retval;
}


/*****************************************************************************
*
*   SeqMgrAdd(type, data)
*   	adds a Bioseq or BioseqSet to the sequence manager
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrAdd (Uint2 type, Pointer data)
{
    Boolean retval;
    
    SeqMgrWriteLock(); 
    retval = ObjMgrAdd(type, data);
    if (type != OBJ_BIOSEQ) {
        SeqMgrUnlock(); 
        return retval;
    }
    
    SeqMgrAddToBioseqIndex((BioseqPtr)data);
    
    SeqMgrUnlock(); 
    
    return retval;
}


/*****************************************************************************
*
*   SeqMgrDelete(type, data)
*   	deletes a Bioseq or BioseqSet from the sequence manager
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrDelete (Uint2 type, Pointer data)
{
	Boolean retval = FALSE;

	SeqMgrWriteLock();
	if (type == OBJ_BIOSEQ)  /* remove id indexes */
		SeqMgrDeleteFromBioseqIndex((BioseqPtr)data);

	retval = ObjMgrDelete(type, data);
	SeqMgrUnlock();
	return retval;
}



static Boolean NEAR SeqMgrAddIndexElement(SeqMgrPtr smp, BioseqPtr bsp, CharPtr buf, Boolean sort_now)
{
	SeqIdIndexElementPtr sip, PNTR sipp;
	SeqIdIndexBlockPtr sibp, prev;
	Int4 imin, imax, i, j;
	CharPtr tmp, newstr;
	ObjMgrDataPtr omdp;
	ObjMgrPtr omp;

	omp = ObjMgrReadLock();
	omdp = ObjMgrFindByData(omp, (Pointer)bsp);  /* caching protection */
	ObjMgrUnlock();
	if (omdp == NULL)
	{
		return FALSE;
	}

	sipp = smp->BioseqIndex;
	if (smp->BioseqIndexCnt >= smp->BioseqIndexNum)  /* expand space */
	{
	   prev = NULL;
	   for (sibp = smp->BioseqIndexData; sibp != NULL; sibp = sibp->next)
		   prev = sibp;
	   sibp = MemNew(sizeof(SeqIdIndexBlock));
	   if (prev != NULL)
		   prev->next = sibp;
	   else
		   smp->BioseqIndexData = sibp;

	   smp->BioseqIndex = MemNew((smp->BioseqIndexNum + 100) * 
				     sizeof(SeqIdIndexElementPtr));
	   MemCopy(smp->BioseqIndex, sipp, (smp->BioseqIndexNum * 
					    sizeof(SeqIdIndexElementPtr)));
	   MemFree(sipp);
	   smp->BioseqIndexNum += 100;
	   sipp = smp->BioseqIndex;
	   for (i = 0, j = smp->BioseqIndexCnt; i < 100; i++, j++)
		   sipp[j] = &(sibp->sid[i]);
	}

	i = smp->BioseqIndexCnt;   /* empties are at the end */
	sip = sipp[i];
	sip->omdp = omdp;       /* fill in the values */
	sip->str = StringSave(buf);
	newstr = sip->str;
	RevStringUpper(newstr);  /* try to avoid case check */

	if (! sort_now)
	{
		smp->BioseqIndexCnt++;     /* got one more */
		return TRUE;
	}

	imin = 0;                   /* find where it goes */
	imax = i-1;
	if (imax >= 0)
		tmp = sipp[imax]->str;
	if ((i) && (StringCmp(newstr, sipp[imax]->str) < 0))
	{
		i = (imax + imin) / 2;
		while (imax > imin)
		{
			tmp = sipp[i]->str;
			if ((j = StringCmp(newstr, tmp)) < 0)
				imax = i - 1;
			else if (j > 0)
				imin = i + 1;
			else
				break;
			i = (imax + imin)/2;
		}

		if (StringCmp(newstr, sipp[i]->str) > 0) /* check for off by 1 */
		{
			i++;
		}


		imax = smp->BioseqIndexCnt - 1;	 /* open the array */
		while (imax >= i)
		{
			sipp[imax+1] = sipp[imax];
			imax--;
		}
	}

	sipp[i] = sip;    /* put in the pointer in order */
	smp->BioseqIndexCnt++;     /* got one more */
	return TRUE;
}

/*****************************************************************************
*
*   SeqMgrHoldIndexing(Boolean hold)
*       stops sequence indexing to allow bulk loading if hold = TRUE
*       starts it when hold = FALSE;
*       uses a counter so you must call it the same number of times
*        with TRUE as with FALSE
*       when the counter decrements to 0, it will index what it has.
*
*****************************************************************************/
NLM_EXTERN void LIBCALL SeqMgrHoldIndexing (Boolean hold)
{
	SeqMgrPtr smp;

	smp = SeqMgrWriteLock();
	if (hold)
		smp->hold_indexing++;
	else
		smp->hold_indexing--;
	SeqMgrUnlock();

	if (! smp->hold_indexing)
		SeqMgrProcessNonIndexedBioseq(FALSE);

	return;
}

int LIBCALLBACK SeqIdIndexElementCmp (VoidPtr a, VoidPtr b);

int LIBCALLBACK SeqIdIndexElementCmp (VoidPtr a, VoidPtr b)
{
	return (int)(StringCmp((*(SeqIdIndexElementPtr PNTR)a)->str,
			       (*(SeqIdIndexElementPtr PNTR)b)->str));
}

/*****************************************************************************
*
*   SeqMgrProcessNonIndexedBioseq(Boolean force_it)
*   	Indexes a BioseqPtr by SeqId(s)
*       If ! force_it, respects the smp->don't index flag
*
*****************************************************************************/
static Boolean NEAR SeqMgrProcessNonIndexedBioseq(Boolean force_it)
{
	BioseqPtr PNTR bspp, bsp;
	Int4 i, total, k, old_BioseqIndexCnt;
	SeqIdPtr sip;
	Char buf[80];
	CharPtr tmp;
	Uint1 oldchoice;
	Boolean indexed;
	TextSeqIdPtr tsip;
	SeqMgrPtr smp;
	Int2 version;
	Boolean sort_now = TRUE;

	smp = SeqMgrReadLock();
	if ((! smp->NonIndexedBioseqCnt) ||           /* nothing to index */
	    ((! force_it) && (smp->hold_indexing)))   /* holding off on indexing */
	{
		SeqMgrUnlock();
		return TRUE;
	}
	SeqMgrUnlock();

	smp = SeqMgrWriteLock();
        if ((! smp->NonIndexedBioseqCnt) ||           /* nothing to index */
            ((! force_it) && (smp->hold_indexing)))   /* holding off on indexing */
	{
		SeqMgrUnlock();
		return TRUE;
	}

	total = smp->NonIndexedBioseqCnt;
	old_BioseqIndexCnt=smp->BioseqIndexCnt; /*** remember this one to do smart sort ****/

	if (total > 100)   /* heap sort is faster */
		sort_now = FALSE;

	bspp = smp->NonIndexedBioseq;
	for (i = 0; i < total; i++)
	{
		indexed = FALSE;
		bsp = bspp[i];
		if (bsp != NULL)
		{
			if (bsp->id != NULL)
			{
				indexed = TRUE;
				version = 0;
				for (sip = bsp->id; sip != NULL; sip = sip->next)
				{
					oldchoice = 0;
					switch (sip->choice)
					{
					case SEQID_GI:
						sprintf(buf, "%ld", (long)(sip->data.intvalue));
						SeqMgrAddIndexElement(smp, bsp, buf, sort_now);
						break;
					case SEQID_EMBL:
					case SEQID_DDBJ:
						oldchoice = sip->choice;
						sip->choice = SEQID_GENBANK;
					case SEQID_GENBANK:
					case SEQID_OTHER:
					case SEQID_TPG:
					case SEQID_TPE:
					case SEQID_TPD:
						tsip = (TextSeqIdPtr)(sip->data.ptrvalue);
						if (((tsip->version > 0) && (tsip->release == NULL))
						    && SHOWVERSION)
						  {
						    version = tsip->version;
						  }
					case SEQID_PIR:
					case SEQID_SWISSPROT:
					case SEQID_PRF:
						tsip = (TextSeqIdPtr)(sip->data.ptrvalue);
						if (tsip->name != NULL)
						{
							tmp = tsip->accession;
							tsip->accession = NULL;
							SeqIdWrite(sip, buf, PRINTID_FASTA_SHORT, 79);
							SeqMgrAddIndexElement(smp, bsp, buf,sort_now);
							tsip->accession = tmp;
						}
						tmp = tsip->name;
						tsip->name = NULL;
 						SeqIdWrite(sip, buf, PRINTID_FASTA_SHORT, 79);
						SeqMgrAddIndexElement(smp, bsp, buf, sort_now);
						if (version) {
						  tsip->version = 0;
						  SeqIdWrite(sip, buf, PRINTID_FASTA_SHORT, 79);
						  SeqMgrAddIndexElement(smp, bsp, buf, sort_now);
						  tsip->version = version;
						}
						tsip->name = tmp;
						if (oldchoice)
							sip->choice = oldchoice;
						break;
					default:
  						SeqIdWrite(sip, buf, PRINTID_FASTA_SHORT, 79);
						SeqMgrAddIndexElement(smp, bsp, buf, sort_now);
						break;
					}
				}
			}
		}
		if (indexed)
			bspp[i] = NULL;
	}

	/* faster single pass removal of NULLs */
	for (i = 0, k = 0; i < total; i++) {
		bsp = bspp [i];
		if (bsp != NULL) {
			bspp [k] = bsp;
			k++;
		}
	}
	total = k;

	/*
	for (i = 0; i < total; i++)
	{
		if (bspp[i] == NULL)
		{
		   total--;
		   for (k = i; k < total; k++)
			   bspp[k] = bspp[k+1];
		   i--;
		}
	}
	*/

	smp->NonIndexedBioseqCnt = total;

	if (! sort_now)   /* sort at the end */
	{
		if(   old_BioseqIndexCnt > 1000 /**** sorted part of the array is large ***/
                   && (old_BioseqIndexCnt*1.1 > smp->BioseqIndexCnt ) ){ /*** unsorted part of the array is < 10% ***/
			SeqIdIndexElementPtr PNTR	bsindex_buf;
			SeqIdIndexElementPtr		stack_buf[1024];
			int	i_o, i_n, i_w;
			int	unsorted_size= smp->BioseqIndexCnt - old_BioseqIndexCnt;

#if 1
			/****  sort unsorted part ****/
			HeapSort((VoidPtr) (smp->BioseqIndex+old_BioseqIndexCnt), (size_t) unsorted_size,
                                  sizeof(SeqIdIndexElementPtr), SeqIdIndexElementCmp);
			/**** move new part to an array ****/
			if(unsorted_size > 1024){
				bsindex_buf=Nlm_Malloc(sizeof(*bsindex_buf)*unsorted_size); 
			} else {
				bsindex_buf=stack_buf;
			}
			MemMove((VoidPtr)bsindex_buf,(VoidPtr)(smp->BioseqIndex+old_BioseqIndexCnt),
				sizeof(*bsindex_buf)*unsorted_size);

			/**** merge both arrays from the end ****/
			i_n=unsorted_size-1;      /**** new part index ****/
			i_o=old_BioseqIndexCnt-1; /**** old part index ***/
			i_w=smp->BioseqIndexCnt-1;/**** whole array index ***/
			i=0;
			while(i_n >= 0) {
				if(   i_o < 0
                                   || SeqIdIndexElementCmp((VoidPtr)(bsindex_buf+i_n),
							   (VoidPtr)(smp->BioseqIndex+i_o)) >= 0){
					/**** move new element ***/
					smp->BioseqIndex[i_w]=bsindex_buf[i_n];
					i_w--;i_n--;
				} else {
					/**** move old element ***/
					smp->BioseqIndex[i_w]=smp->BioseqIndex[i_o];
                                        i_w--;i_o--;
				}
				i++;
			}
			/*** cleanup *****/
			if(unsorted_size > 1024){
				MemFree(bsindex_buf);
			}
#else   
			HeapSort((VoidPtr) (smp->BioseqIndex), (size_t)(smp->BioseqIndexCnt),
                                  sizeof(SeqIdIndexElementPtr), SeqIdIndexElementCmp);
#endif
		} else { /** Heap Sort should be faster ***/
			HeapSort((VoidPtr) (smp->BioseqIndex), (size_t)(smp->BioseqIndexCnt),
				  sizeof(SeqIdIndexElementPtr), SeqIdIndexElementCmp);
		}
	}

	SeqMgrUnlock();

	return TRUE;
}



/*****************************************************************************
*
*   SeqMgrAddToBioseqIndex(bsp)
*   	Indexes a BioseqPtr by SeqId(s)
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrAddToBioseqIndex (BioseqPtr bsp)
{
	SeqMgrPtr smp;
	BioseqPtr PNTR bspp;

	if (bsp == NULL)
		return FALSE;

	smp = SeqMgrWriteLock();

	/* if this bsp was the last one added, no need to add it again */
	if (smp->NonIndexedBioseqCnt > 0 && smp->NonIndexedBioseq [smp->NonIndexedBioseqCnt - 1] == bsp) {
		SeqMgrUnlock();
		return TRUE;
	}

							  /* increase array as needed */
	if (smp->NonIndexedBioseqCnt >= smp->NonIndexedBioseqNum)
	{
		bspp = smp->NonIndexedBioseq;
		smp->NonIndexedBioseq = MemNew((smp->NonIndexedBioseqNum + 10) * sizeof (BioseqPtr));
		MemCopy(smp->NonIndexedBioseq, bspp, (smp->NonIndexedBioseqNum * sizeof(BioseqPtr)));
		MemFree(bspp);
		smp->NonIndexedBioseqNum += 10;
	}

	smp->NonIndexedBioseq[smp->NonIndexedBioseqCnt] = bsp;
	smp->NonIndexedBioseqCnt++;

	SeqMgrUnlock();

	SeqMgrProcessNonIndexedBioseq(FALSE);

	return TRUE;
}


/*****************************************************************************
*
*   SeqMgrDeleteDeleteFromBioseqIndex(bsp)
*   	Removes index on BioseqPtr SeqIds
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrDeleteFromBioseqIndex (BioseqPtr bsp)
{
	SeqMgrPtr smp;
	SeqIdIndexElementPtr PNTR sipp, sip;
	Int4 i, j, num;
	BioseqPtr PNTR bspp;
	ObjMgrDataPtr omdp;
	ObjMgrPtr omp;

	smp = SeqMgrWriteLock();

	/* bail if in bulk deletion of large record */
	if (bsp != NULL) {
		omdp = bsp->omdp;
		if (omdp != NULL && omdp->bulkIndexFree) {
			SeqMgrUnlock();
			return FALSE;
		}
	}
								/* check if not indexed yet */
	if (smp->NonIndexedBioseqCnt > 0)
	{
		num = smp->NonIndexedBioseqCnt;
		bspp = smp->NonIndexedBioseq;
		for (i = 0; i < num; i++)
		{
			if (bspp[i] == bsp)
			{
				num--;
				for (j = i; j < num; j++)
					 bspp[j] = bspp[j+1];
				i--;
			}
		}
		smp->NonIndexedBioseqCnt = num;
	}

	num = smp->BioseqIndexCnt;
	sipp = smp->BioseqIndex;

        /*	omp = ObjMgrReadLock();  */
        
        omp = ObjMgrGet();
	omdp = ObjMgrFindByData(omp, (Pointer)bsp);
        
	/* ObjMgrUnlock(); */

	for (i = 0; i < BIOSEQ_CACHE_NUM; i++)   /* remove from BioseqFind cache */
	{
		if (omdp_cache[i] == omdp)
		{
			omdp_cache[i] = NULL;
			se_cache[i] = NULL;
		}
	}

	for (i = 0; i < num; i++)
	{
	   if (sipp[i]->omdp == omdp)
	   {
		   sipp[i]->omdp = NULL;
		   sipp[i]->str = MemFree(sipp[i]->str);
		   sip = sipp[i];
		   for (j = i; j < (num-1); j++)
			   sipp[j] = sipp[j+1];
		   sipp[j] = sip;
		   num--; i--;
	   }
	}

	smp->BioseqIndexCnt = num;

	SeqMgrUnlock();

	return TRUE;
}

/*****************************************************************************
*
*   SeqMgrDeleteIndexesInRecord (sep)
*   	Bulk removal of SeqId index on entire entity prior to its deletion
*
*****************************************************************************/
static void MarkSeqForBulkDeletion (
  BioseqPtr bsp,
  Pointer userdata
)

{
  ObjMgrDataPtr  omdp;

  if (bsp == NULL) return;
  omdp = bsp->omdp;
  if (omdp == NULL) return;
  omdp->bulkIndexFree = TRUE;
}

static void MarkSetForBulkDeletion (
  BioseqSetPtr bssp,
  Pointer userdata
)

{
  ObjMgrDataPtr  omdp;
  ObjMgrPtr      omp;

  if (bssp == NULL) return;
  omp = ObjMgrWriteLock ();
  omdp = ObjMgrFindByData (omp, bssp); 
  if (omdp != NULL) {
    omdp->bulkIndexFree = TRUE;
  }
  ObjMgrUnlock ();
}

NLM_EXTERN Boolean LIBCALL SeqMgrDeleteIndexesInRecord (SeqEntryPtr sep)

{
  BioseqPtr                  bsp;
  BioseqPtr PNTR             bspp;
  Int4                       i, j, k, num;
  ObjMgrDataPtr              omdp;
  SeqIdIndexElementPtr PNTR  sipp;
  SeqMgrPtr                  smp;
  SeqIdIndexElementPtr PNTR  tmp;

  if (sep == NULL) return FALSE;

  smp = SeqMgrWriteLock ();

  VisitBioseqsInSep (sep, NULL, MarkSeqForBulkDeletion);
  VisitSetsInSep (sep, NULL, MarkSetForBulkDeletion);

  /* check if not indexed yet */

  if (smp->NonIndexedBioseqCnt > 0) {

    num = smp->NonIndexedBioseqCnt;
    bspp = smp->NonIndexedBioseq;

    for (i = 0; i < num; i++) {
      bsp = bspp [i];
      if (bsp != NULL) {
        omdp = bsp->omdp;
        if (omdp != NULL && omdp->bulkIndexFree) {
          num--;
          for (j = i; j < num; j++) {
             bspp [j] = bspp [j + 1];
          }
          i--;
        }
      }
    }

    smp->NonIndexedBioseqCnt = num;
  }

  /* remove from BioseqFind cache */

  for (i = 0; i < BIOSEQ_CACHE_NUM; i++) {
    omdp = omdp_cache [i];
    if (omdp != NULL && omdp->bulkIndexFree) {
      omdp_cache [i] = NULL;
      se_cache [i] = NULL;
    }
  }

  /* bulk free of indexes from sipp list */

  sipp = smp->BioseqIndex;
  if (sipp == NULL) {
    SeqMgrUnlock ();
    return FALSE;
  }

  num = smp->BioseqIndexCnt;
  tmp = (SeqIdIndexElementPtr PNTR) MemNew (sizeof (SeqIdIndexElementPtr) * (size_t) (num + 1));
  if (tmp != NULL) {

    /* null out dying indexes, compress list, move empties to end */

    for (i = 0, j = 0, k = 0; i < num; i++) {
      omdp = sipp [i]->omdp;
      if (omdp != NULL && omdp->bulkIndexFree) {
        sipp [i]->omdp = NULL;
        sipp [i]->str = MemFree (sipp [i]->str);
        tmp [k] = sipp [i];
        k++;
      } else {
        sipp [j] = sipp [i];
        j++;
      }
    }
    /* update count of remaining indexes */

    smp->BioseqIndexCnt = j;
    MemMove (sipp + j, tmp, sizeof (SeqIdIndexElementPtr) * (size_t) k);
  }
  MemFree (tmp);

  SeqMgrUnlock ();

  return TRUE;
}

/*****************************************************************************
*
*   SeqMgrReplaceInBioseqIndex(bsp)
*   	Replaces index on BioseqPtr SeqIds
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqMgrReplaceInBioseqIndex (BioseqPtr bsp)
{
	SeqMgrDeleteFromBioseqIndex(bsp);
	return SeqMgrAddToBioseqIndex(bsp);
}

/*****************************************************************************
*
*   GetUniGeneIDForSeqId(SeqIdPtr)
*     returns the UniGene ID for a SeqId
*     returns 0 if can't find it, or not a legal unigene id
*     This only applies to genomes division of entrez
*
*****************************************************************************/

/*****************************************************************
*
*	IT IS a KLUDGE!! Add 1,000,000 to the unigene id
*
*****************************************************************/
#define KLUDGE_UNIGENE 1000000	/*the kludge offset val add to unigene sequence*/
#define KLUDGE_FlyBase 2000000	/*the kludge offset for FlyBase*/
#define KLUDGE_JACKSON 3000000  /*the kludge offset for the Mouse data*/
#define KLUDGE_JRGP    4000000  /*the kludge offset for the rice data*/
#define KLUDGE_CESC    5000000  /*the kludge offset for the C. elegans data*/
#define KLUDGE_BSNR    6000000  /*the kludge offset for the B. subtilis data*/
#define KLUDGE_HUMGEN  7000000  /*the kludge offset for the Human genomic data*/
#define KLUDGE_YGG     8000000  /*the kludge offset for the yeast data*/
#define KLUDGE_NCBICG  9000000  /*the kludge offset for small genomes*/
#define KLUDGE_MAIZE  10000000  /*the kludge offset for corn*/

NLM_EXTERN Int4 LIBCALL GetUniGeneIDForSeqId (SeqIdPtr sip)
{
	DbtagPtr db_tag;
	ObjectIdPtr oip;

	if (sip == NULL)
		return 0;


	if(sip->choice != SEQID_GENERAL)
		return 0;

	db_tag = sip->data.ptrvalue;
	if(db_tag == NULL || db_tag->db == NULL)
		return 0;

	oip = db_tag->tag;
	if(oip == NULL || oip->id == 0)
		return 0;

	if(StringCmp(db_tag->db, "UNIGENE") == 0)
		return (KLUDGE_UNIGENE+ oip->id);
	if(StringCmp(db_tag->db, "UniGene") == 0)
		return (KLUDGE_UNIGENE+ oip->id);
	if(StringCmp(db_tag->db, "FlyBase") == 0)
		return (KLUDGE_FlyBase+ oip->id);
	if(StringCmp(db_tag->db, "JACKSON") == 0)
		return (KLUDGE_JACKSON+ oip->id);
	if(StringCmp(db_tag->db, "JRGP") == 0)
		return (KLUDGE_JRGP + oip->id);
	if(StringCmp(db_tag->db, "CESC") == 0)
		return (KLUDGE_CESC + oip->id);
	if(StringCmp(db_tag->db, "BSNR") == 0)
		return (KLUDGE_BSNR + oip->id);
        if(StringCmp(db_tag->db, "HUMGEN") == 0)
                return (KLUDGE_HUMGEN + oip->id);
        if(StringCmp(db_tag->db, "YGG") == 0)
                return (KLUDGE_YGG + oip->id);
        if(StringCmp(db_tag->db, "NCBICG") == 0)
                return (KLUDGE_NCBICG + oip->id);
        if(StringCmp(db_tag->db, "MAIZE") == 0)
                return (KLUDGE_MAIZE + oip->id);
	return 0;

}


/*****************************************************************************
*
*   BioseqExtra extensions to preindex for rapid retrieval
*
*****************************************************************************/

/*
*  remaining to be done are mapping tables for rapid coordinate conversion
*  between genome record and parts, genomic DNA and mRNA, and mRNA and protein
*/

static ObjMgrDataPtr SeqMgrGetOmdpForPointer (Pointer ptr)

{
  ObjMgrDataPtr  omdp;
  ObjMgrPtr      omp;

  if (ptr == NULL) return NULL;
  omp = ObjMgrWriteLock ();
  omdp = ObjMgrFindByData (omp, ptr);
  ObjMgrUnlock ();
  return omdp;
}

static ObjMgrDataPtr SeqMgrGetOmdpForBioseq (BioseqPtr bsp)

{
  ObjMgrDataPtr  omdp;
  ObjMgrPtr      omp;

  if (bsp == NULL) return NULL;
  omdp = (ObjMgrDataPtr) bsp->omdp;
  if (omdp != NULL) return omdp;
  omp = ObjMgrWriteLock ();
  omdp = ObjMgrFindByData (omp, bsp);
  ObjMgrUnlock ();
  bsp->omdp = (Pointer) omdp;
  return omdp;
}

static SeqEntryPtr SeqMgrGetTopSeqEntryForEntity (Uint2 entityID)

{
  ObjMgrDataPtr  omdp;
  SeqSubmitPtr   ssp;

  omdp = ObjMgrGetData (entityID);
  if (omdp == NULL) return FALSE;
  switch (omdp->datatype) {
    case OBJ_SEQSUB :
      ssp = (SeqSubmitPtr) omdp->dataptr;
      if (ssp != NULL && ssp->datatype == 1) {
        return (SeqEntryPtr) ssp->data;
      }
      break;
    case OBJ_BIOSEQ :
    case OBJ_BIOSEQSET :
      return (SeqEntryPtr) omdp->choice;
    default :
      break;
  }
  return NULL;
}

static Boolean SeqMgrClearBioseqExtraData (ObjMgrDataPtr omdp)

{
  BioseqExtraPtr  bspextra;
  SMFeatBlockPtr  currf;
  SMSeqIdxPtr     currp;
  Int2            i;
  SMFeatItemPtr   itemf;
  SMFeatBlockPtr  nextf;
  SMSeqIdxPtr     nextp;

  if (omdp == NULL) return FALSE;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return FALSE;

  /* free sorted arrays of pointers into data blocks */

  bspextra->descrsByID = MemFree (bspextra->descrsByID);
  bspextra->descrsBySdp = MemFree (bspextra->descrsBySdp);
  bspextra->descrsByIndex = MemFree (bspextra->descrsByIndex);

  bspextra->alignsByID = MemFree (bspextra->alignsByID);

  bspextra->featsByID = MemFree (bspextra->featsByID);
  bspextra->featsBySfp = MemFree (bspextra->featsBySfp);
  bspextra->featsByPos = MemFree (bspextra->featsByPos);
  bspextra->featsByRev = MemFree (bspextra->featsByRev);
  bspextra->featsByLabel = MemFree (bspextra->featsByLabel);

  bspextra->genesByPos = MemFree (bspextra->genesByPos);
  bspextra->mRNAsByPos = MemFree (bspextra->mRNAsByPos);
  bspextra->CDSsByPos = MemFree (bspextra->CDSsByPos);
  bspextra->pubsByPos = MemFree (bspextra->pubsByPos);
  bspextra->orgsByPos = MemFree (bspextra->orgsByPos);
  bspextra->operonsByPos = MemFree (bspextra->operonsByPos);

  bspextra->genesByLocusTag = MemFree (bspextra->genesByLocusTag);

  /* free list of descriptor information */

  bspextra->desclisthead = ValNodeFreeData (bspextra->desclisthead);

  /* free arrays to speed mapping from parts to segmented bioseq */

  bspextra->partsByLoc = MemFree (bspextra->partsByLoc);
  bspextra->partsBySeqId = MemFree (bspextra->partsBySeqId);

  /* free data blocks of feature information */

  currf = bspextra->featlisthead;
  while (currf != NULL) {
    nextf = currf->next;

    if (currf->data != NULL) {

      /* free allocated label strings within block items */

      for (i = 0; i < currf->index; i++) {
        itemf = &(currf->data [i]);
        MemFree (itemf->label);
        MemFree (itemf->ivals);
      }

      /* free array of SMFeatItems */

      MemFree (currf->data);
    }

    MemFree (currf);
    currf = nextf;
  }

  /* free data blocks of parts to segment mapping information */

  currp = bspextra->segparthead;
  while (currp != NULL) {
    nextp = currp->next;
    SeqLocFree (currp->slp);
    MemFree (currp->seqIdOfPart);
    MemFree (currp);
    currp = nextp;
  }

  /* free list of seqfeatptrs whose product points to the bioseq */

  bspextra->prodlisthead = ValNodeFree (bspextra->prodlisthead);

  /* clean interval list once implemented */

  bspextra->featlisthead = NULL;
  bspextra->featlisttail = NULL;
  bspextra->segparthead = NULL;

  bspextra->numaligns = 0;
  bspextra->numfeats = 0;
  bspextra->numgenes = 0;
  bspextra->nummRNAs = 0;
  bspextra->numCDSs = 0;
  bspextra->numpubs = 0;
  bspextra->numorgs = 0;
  bspextra->numoperons = 0;
  bspextra->numsegs = 0;

  bspextra->min = INT4_MAX;
  bspextra->blocksize = 50;

  bspextra->protFeat = NULL;
  bspextra->cdsOrRnaFeat = NULL;

  /* free genome - parts mapping arrays when they are added */

  return TRUE;
}

static Boolean DoSeqMgrFreeBioseqExtraData (ObjMgrDataPtr omdp)

{
  if (omdp == NULL) return FALSE;
  if (omdp->datatype != OBJ_BIOSEQ && omdp->datatype != OBJ_BIOSEQSET) return FALSE;
  if (omdp->extradata != NULL) {
    SeqMgrClearBioseqExtraData (omdp);
    omdp->extradata = MemFree (omdp->extradata);
    omdp->reapextra = NULL;
    omdp->reloadextra = NULL;
    omdp->freeextra = NULL;
  }
  return TRUE;
}

/* object manager callbacks to reap, reload, and free extra bioseq data */

NLM_EXTERN Pointer LIBCALLBACK SeqMgrReapBioseqExtraFunc (Pointer data)

{
  BioseqExtraPtr  bspextra;
  SMFeatBlockPtr  curr;
  Int2            i;
  SMFeatItemPtr   item;
  ObjMgrDataPtr   omdp;
  SMDescItemPtr   sdip;
  ValNodePtr      vnp;

  omdp = (ObjMgrDataPtr) data;
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;

  /* loop through data blocks of feature information */

  curr = bspextra->featlisthead;
  while (curr != NULL) {

    /* NULL out pointers to cached out feature and annot */

	if (curr->data != NULL) {
      for (i = 0; i < curr->index; i++) {
        item = &(curr->data [i]);
        item->sfp = NULL;
        item->sap = NULL;
      }
    }

    curr = curr->next;
  }

  /* these were originally only recorded if omdp->tempload == TL_NOT_TEMP */

  bspextra->protFeat = NULL;
  bspextra->cdsOrRnaFeat = NULL;

  /* NULL out pointers to cached out descriptors */

  for (vnp = bspextra->desclisthead; vnp != NULL; vnp = vnp->next) {
    sdip = (SMDescItemPtr) vnp->data.ptrvalue;
    if (sdip != NULL) {
      sdip->sdp = NULL;
      sdip->sep = NULL;
    }
  }

  return NULL;
}

/* !!! SeqMgrReloadBioseqExtraFunc is not yet implemented !!! */

NLM_EXTERN Pointer LIBCALLBACK SeqMgrReloadBioseqExtraFunc (Pointer data)

{
  return NULL;
}

NLM_EXTERN Pointer LIBCALLBACK SeqMgrFreeBioseqExtraFunc (Pointer data)

{
  DoSeqMgrFreeBioseqExtraData ((ObjMgrDataPtr) data);
  return NULL;
}

/*****************************************************************************
*
*   SeqMgrClearFeatureIndexes clears every bioseq in an entity
*
*****************************************************************************/

static void SeqMgrClearIndexesProc (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  ObjMgrDataPtr  omdp = NULL;
  BoolPtr        rsult;
  SeqAlignPtr    sal;
  SeqAnnotPtr    sap = NULL;

  if (sep == NULL || (! IS_Bioseq (sep))) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    omdp = SeqMgrGetOmdpForBioseq (bsp);
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    omdp = SeqMgrGetOmdpForPointer (bssp);
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 2) {
      sal = (SeqAlignPtr) sap->data;
      while (sal != NULL) {
        /* ! clean up seq-align indexes ! */
        sal = sal->next;
      }
    }
    sap = sap->next;
  }
  if (omdp != NULL && DoSeqMgrFreeBioseqExtraData (omdp)) {
    rsult = (BoolPtr) mydata;
    *rsult = TRUE;
  }
}

NLM_EXTERN Boolean LIBCALL SeqMgrClearFeatureIndexes (Uint2 entityID, Pointer ptr)

{
  ObjMgrDataPtr  omdp;
  Boolean        rsult = FALSE;
  SeqEntryPtr    sep;

  if (entityID == 0) {
    entityID = ObjMgrGetEntityIDForPointer (ptr);
  }
  if (entityID == 0) return FALSE;
  sep = SeqMgrGetTopSeqEntryForEntity (entityID);
  if (sep == NULL) return FALSE;
  SeqEntryExplore (sep, (Pointer) (&rsult), SeqMgrClearIndexesProc);

  /* clear out object manager time of indexing flag and master feature itemID list */

  omdp = ObjMgrGetData (entityID);
  if (omdp != NULL) {
    omdp->indexed = 0;
    SeqMgrClearBioseqExtraData (omdp);
    omdp->extradata = MemFree (omdp->extradata);
    omdp->reapextra = NULL;
    omdp->reloadextra = NULL;
    omdp->freeextra = NULL;
  }
  return rsult;
}

/*****************************************************************************
*
*   IsNonGappedLiteral(BioseqPtr bsp)
*      Returns TRUE if bsp is a delta seq is composed only of Seq-lits with
*      actual sequence data.  These are now made to allow optimal compression
*      of otherwise raw sequences with runs of ambiguous bases.
*
*****************************************************************************/

NLM_EXTERN Boolean IsNonGappedLiteral (BioseqPtr bsp)

{
  DeltaSeqPtr  dsp;
  SeqLitPtr    slitp;

  if (bsp == NULL || bsp->repr != Seq_repr_delta) return FALSE;
  if (bsp->seq_ext_type != 4 || bsp->seq_ext == NULL) return FALSE;

  for (dsp = (DeltaSeqPtr) bsp->seq_ext; dsp != NULL; dsp = dsp->next) {
    if (dsp->choice != 2) return FALSE; /* not Seq-lit */
    slitp = (SeqLitPtr) dsp->data.ptrvalue;
    if (slitp == NULL) return FALSE;
    if (slitp->seq_data == NULL || slitp->length == 0) return FALSE; /* gap */
  }

  return TRUE;
}

/*****************************************************************************
*
*   FindAppropriateBioseq finds the segmented bioseq if location is join on parts
*
*****************************************************************************/

static BioseqPtr FindAppropriateBioseq (SeqLocPtr loc, BioseqPtr tryfirst)

{
  BioseqPtr       bsp = NULL;
  BioseqExtraPtr  bspextra;
  BioseqSetPtr    bssp;
  ObjMgrDataPtr   omdp;
  BioseqPtr       part;
  SeqEntryPtr     sep;
  SeqIdPtr        sip;
  SeqLocPtr       slp;

  if (loc == NULL) return NULL;
  sip = SeqLocId (loc);
  if (sip != NULL) {
    if (tryfirst != NULL && SeqIdIn (sip, tryfirst->id)) {
      bsp = tryfirst;
    } else {
      bsp = BioseqFindCore (sip);
    }

    /* first see if this is raw local part of segmented bioseq */

    if (bsp != NULL && (bsp->repr == Seq_repr_raw || IsNonGappedLiteral (bsp))) {
      omdp = SeqMgrGetOmdpForBioseq (bsp);
      if (omdp != NULL && omdp->datatype == OBJ_BIOSEQ) {
        bspextra = (BioseqExtraPtr) omdp->extradata;
        if (bspextra != NULL) {
          if (bspextra->parentBioseq != NULL) {
            bsp = bspextra->parentBioseq;
          }
        }
      }
    }
    return bsp;
  }

  /* otherwise assume location is on multiple parts of a segmented set */

  slp = SeqLocFindNext (loc, NULL);
  if (slp == NULL) return NULL;
  sip = SeqLocId (slp);
  if (sip == NULL) return NULL;
  part = BioseqFindCore (sip);
  if (part == NULL) return NULL;
  omdp = SeqMgrGetOmdpForBioseq (part);
  while (omdp != NULL) {
    if (omdp->datatype == OBJ_BIOSEQSET) {
      bssp = (BioseqSetPtr) omdp->dataptr;
      if (bssp != NULL) {
        if (bssp->_class == BioseqseqSet_class_segset) {
          for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
            if (IS_Bioseq (sep)) {
              bsp = (BioseqPtr) sep->data.ptrvalue;
              if (bsp != NULL) {
                return bsp;
              }
            }
          }
        }
      }
    }
    omdp = SeqMgrGetOmdpForPointer (omdp->parentptr);
  }
  return NULL;
}

/*****************************************************************************
*
*   FindFirstLocalBioseq is called as a last resort if FindAppropriateBioseq
*     fails, and it scans the feature location to find the first local bioseq
*     referenced by a feature interval
*
*****************************************************************************/

static BioseqPtr FindFirstLocalBioseq (SeqLocPtr loc)

{
  BioseqPtr  bsp;
  SeqIdPtr   sip;
  SeqLocPtr  slp = NULL;

  if (loc == NULL) return NULL;

  while ((slp = SeqLocFindNext (loc, slp)) != NULL) {
    sip = SeqLocId (slp);
    if (sip != NULL) {
      bsp = BioseqFindCore (sip);
      if (bsp != NULL) return bsp;
    }
  }

  return NULL;
}

/*****************************************************************************
*
*   BioseqFindFromSeqLoc finds the segmented bioseq if location is join on parts,
*     and does so even if some of the intervals are far accessions.
*
*****************************************************************************/

NLM_EXTERN BioseqPtr BioseqFindFromSeqLoc (SeqLocPtr loc)

{
  BioseqPtr  bsp = NULL;

  if (loc == NULL) return NULL;
  bsp = FindAppropriateBioseq (loc, NULL);
  if (bsp == NULL) {
    bsp = FindFirstLocalBioseq (loc);
  }
  return bsp;
}

/*****************************************************************************
*
*   SeqMgrGetParentOfPart returns the segmented bioseq parent of a part bioseq,
*     and fills in the context structure.
*
*****************************************************************************/

NLM_EXTERN BioseqPtr LIBCALL SeqMgrGetParentOfPart (BioseqPtr bsp,
                                                    SeqMgrSegmentContext PNTR context)

{
  BioseqExtraPtr    bspextra;
  Char              buf [80];
  Int2              compare;
  Uint2             entityID;
  Int4              i;
  Int4              numsegs;
  ObjMgrDataPtr     omdp;
  BioseqPtr         parent;
  SMSeqIdxPtr PNTR  partsByLoc;
  SMSeqIdxPtr PNTR  partsBySeqId;
  SMSeqIdxPtr       segpartptr;
  SeqIdPtr          sip;
  SeqLocPtr         slp;
  Int2              L, R, mid;

  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrSegmentContext));
  }
  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL) return NULL;
  if (omdp->datatype != OBJ_BIOSEQ) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;
  entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);

  parent = bspextra->parentBioseq;
  if (parent == NULL) return NULL;

  /* now need parts list from extra data on parent */

  omdp = SeqMgrGetOmdpForBioseq (parent);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return parent;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return parent;

  partsBySeqId = bspextra->partsBySeqId;
  numsegs = bspextra->numsegs;
  if (partsBySeqId == NULL || numsegs < 1) return parent;

  sip = bsp->id;
  if (sip == NULL) return parent;

  /* binary search into array on segmented bioseq sorted by part seqID (reversed) string */

  while (sip != NULL) {
    if (MakeReversedSeqIdString (sip, buf, sizeof (buf) - 1)) {
      L = 0;
      R = numsegs - 1;
      while (L < R) {
        mid = (L + R) / 2;
        segpartptr = partsBySeqId [mid];
        compare = StringCmp (segpartptr->seqIdOfPart, buf);
        if (compare < 0) {
          L = mid + 1;
        } else {
          R = mid;
        }
      }
      segpartptr = partsBySeqId [R];
      if (segpartptr != NULL && StringCmp (segpartptr->seqIdOfPart, buf) == 0) {
        if (context != NULL) {
          slp = segpartptr->slp;
          context->entityID = entityID;
          context->itemID = segpartptr->itemID;
          context->slp = slp;
          context->parent = segpartptr->parentBioseq;
          context->cumOffset = segpartptr->cumOffset;
          context->from = segpartptr->from;
          context->to = segpartptr->to;
          context->strand = segpartptr->strand;
          context->userdata = NULL;
          context->omdp = (Pointer) omdp;
          context->index = 0;

          /* now find entry in partsByLoc list to set proper index */

          partsByLoc = bspextra->partsByLoc;
          if (partsByLoc != NULL) {
            i = 0;
            while (i < numsegs) {
              if (segpartptr == partsByLoc [i]) {
                context->index = i + 1;
              }
              i++;
            }
          }
        }
        return parent;
      }
    }
    sip = sip->next;
  }

  return parent;
}

/*****************************************************************************
*
*   SeqMgrGetBioseqContext fills in the context structure for any bioseq.
*
*****************************************************************************/

NLM_EXTERN Boolean LIBCALL SeqMgrGetBioseqContext (BioseqPtr bsp,
                                                   SeqMgrBioseqContext PNTR context)

{
  BioseqExtraPtr  bspextra;
  Uint2           entityID;
  ObjMgrDataPtr   omdp;
  SeqEntryPtr     sep;

  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrBioseqContext));
  }
  if (bsp == NULL || context == NULL) return FALSE;

  entityID = ObjMgrGetEntityIDForPointer (bsp);
  if (entityID == 0) return FALSE;

  sep = SeqMgrGetTopSeqEntryForEntity (entityID);
  if (sep == NULL) return FALSE;

  context->entityID = entityID;
  context->index = 0;
  context->userdata = NULL;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp != NULL && omdp->datatype == OBJ_BIOSEQ) {
    bspextra = (BioseqExtraPtr) omdp->extradata;
    if (bspextra != NULL) {
      context->itemID = bspextra->bspItemID;
      context->bsp = bsp;
      context->sep = bsp->seqentry;
      if (bsp->idx.parenttype == OBJ_BIOSEQSET) {
        context->bssp = (BioseqSetPtr) bsp->idx.parentptr;
      }
      context->numsegs = bspextra->numsegs;
      context->omdp = omdp;
      context->index = bspextra->bspIndex;
    }
  }

  return (Boolean) (context->index != 0);
}

/*****************************************************************************
*
*   GetOffsetInNearBioseq is called to get the intervals on near bioseqs
*
*****************************************************************************/

static Int4 GetOffsetInNearBioseq (SeqLocPtr loc, BioseqPtr in, Uint1 which_end)

{
  BioseqPtr  bsp;
  SeqLocPtr  first = NULL, last = NULL, slp = NULL;
  SeqIdPtr   sip;
  Uint1      strand;
  Int4       val;

  if (loc == NULL) return -1;

  /* first attempt should work if no far bioseqs */

  val = GetOffsetInBioseq (loc, in, which_end);
  if (val != -1) return val;

  /* now go through sublocs and find extremes that are not on far bioseqs */

  while ((slp = SeqLocFindNext (loc, slp)) != NULL) {
    sip = SeqLocId (slp);
    if (sip != NULL) {
      bsp = BioseqFind (sip);
      if (bsp != NULL) {
        last = slp;
        if (first == NULL) {
          first = slp;
        }
      }
    }
  }
  if (first == NULL) return -1;
  strand = SeqLocStrand (first);

  switch (which_end) {
    case SEQLOC_LEFT_END:
      if (strand == Seq_strand_minus) {
        return GetOffsetInBioseq (last, in, which_end);
      } else {
        return GetOffsetInBioseq (first, in, which_end);
      }
      break;
    case SEQLOC_RIGHT_END:
      if (strand == Seq_strand_minus) {
        return GetOffsetInBioseq (first, in, which_end);
      } else {
        return GetOffsetInBioseq (last, in, which_end);
      }
      break;
    case SEQLOC_START:
      return GetOffsetInBioseq (first, in, which_end);
      break;
    case SEQLOC_STOP:
      return GetOffsetInBioseq (last, in, which_end);
      break;
    default :
      break;
  }

  return -1;
}

/*
static Int4 GetOffsetInFirstLocalBioseq (SeqLocPtr loc, BioseqPtr in, Uint1 which_end)

{
  SeqLocPtr  slp = NULL;
  Int4       val;

  if (loc == NULL) return -1;

  while ((slp = SeqLocFindNext (loc, slp)) != NULL) {
    val = GetOffsetInBioseq (slp, in, which_end);
    if (val != -1) return val;
  }

  return -1;
}
*/

/*****************************************************************************
*
*   SeqMgrFindSMFeatItemPtr and SeqMgrFindSMFeatItemByID return SMFeatItemPtr
*     to access internal fields
*   SeqMgrGetDesiredDescriptor and SeqMgrGetDesiredFeature take an itemID,
*     position index, or SeqDescPtr or SeqFeatPtr, return the SeqDescPtr or
*     SeqFeatPtr, and fill in the context structure
*
*****************************************************************************/

NLM_EXTERN SMFeatItemPtr LIBCALL SeqMgrFindSMFeatItemPtr (SeqFeatPtr sfp)

{
  SMFeatItemPtr PNTR  array;
  BioseqPtr           bsp;
  BioseqExtraPtr      bspextra;
  SMFeatBlockPtr      curr;
  Int2                i;
  SMFeatItemPtr       item;
  Int4                L;
  Int4                mid;
  ObjMgrDataPtr       omdp;
  Int4                R;

  if (sfp == NULL) return NULL;
  bsp = FindAppropriateBioseq (sfp->location, NULL);
  if (bsp == NULL) {
    bsp = FindFirstLocalBioseq (sfp->location);
  }
  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;

  /* first try array sorted by SeqFeatPtr value */

  array = bspextra->featsBySfp;
  if (array != NULL && bspextra->numfeats > 0) {
    L = 0;
    R = bspextra->numfeats - 1;
    while (L < R) {
      mid = (L + R) / 2;
      item = array [mid];
      if (item != NULL && item->sfp < sfp) {
        L = mid + 1;
      } else {
        R = mid;
      }
    }

    item = array [R];
    if (item->sfp == sfp) return item;
  }

  /* now look in feature indices for cached feature information */

  curr = bspextra->featlisthead;
  while (curr != NULL) {

    if (curr->data != NULL) {
      for (i = 0; i < curr->index; i++) {
        item = &(curr->data [i]);
        if (item->sfp == sfp && (! item->ignore)) return item;
      }
    }

    curr = curr->next;
  }

  return NULL;
}

NLM_EXTERN SMFeatItemPtr LIBCALL SeqMgrFindSMFeatItemByID (Uint2 entityID, BioseqPtr bsp, Uint4 itemID)

{
  SMFeatItemPtr PNTR  array;
  BioseqExtraPtr      bspextra;
  SMFeatBlockPtr      curr;
  Int2                i;
  SMFeatItemPtr       item;
  Int4                L;
  Int4                mid;
  ObjMgrDataPtr       omdp;
  Int4                R;

  if (entityID > 0) {
    omdp = ObjMgrGetData (entityID);
    if (omdp == NULL) return NULL;
  } else {
    if (bsp == NULL) return NULL;
    omdp = SeqMgrGetOmdpForBioseq (bsp);
    if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;
  }
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;

  /* first try array sorted by itemID value */

  array = bspextra->featsByID;
  if (array != NULL && bspextra->numfeats > 0) {
    L = 0;
    R = bspextra->numfeats - 1;
    while (L < R) {
      mid = (L + R) / 2;
      item = array [mid];
      if (item != NULL && item->itemID < itemID) {
        L = mid + 1;
      } else {
        R = mid;
      }
    }

    item = array [R];
    if (item->itemID == itemID) return item;
  }

  /* now look in feature indices for cached feature information */

  curr = bspextra->featlisthead;
  while (curr != NULL) {

    if (curr->data != NULL) {
      for (i = 0; i < curr->index; i++) {
        item = &(curr->data [i]);
        if (item->itemID == itemID && (! item->ignore)) return item;
      }
    }

    curr = curr->next;
  }

  return NULL;
}

NLM_EXTERN SeqAlignPtr LIBCALL SeqMgrFindSeqAlignByID (Uint2 entityID, Uint2 itemID)

{
  BioseqExtraPtr  bspextra;
  ObjMgrDataPtr   omdp;

  if (entityID < 1) return NULL;
  omdp = ObjMgrGetData (entityID);
  if (omdp == NULL) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;

  if (bspextra->alignsByID != NULL && bspextra->numaligns > 0 &&
      itemID > 0 && itemID <= bspextra->numaligns) {
    return bspextra->alignsByID [itemID];
  }

  return NULL;
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetDesiredFeature (Uint2 entityID, BioseqPtr bsp,
                                                       Uint4 itemID, Uint4 index, SeqFeatPtr sfp,
                                                       SeqMgrFeatContext PNTR context)

{
  SMFeatItemPtr PNTR  array;
  BioseqExtraPtr      bspextra;
  SeqFeatPtr          curr;
  SMFeatItemPtr       item = NULL;
  ObjMgrDataPtr       omdp;

  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrFeatContext));
  }
  if (entityID > 0) {
    omdp = ObjMgrGetData (entityID);
    if (omdp == NULL) return NULL;
  } else {
    if (bsp == NULL) return NULL;
    omdp = SeqMgrGetOmdpForBioseq (bsp);
    if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;
  }
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;

  if (itemID > 0) {
    item = SeqMgrFindSMFeatItemByID (entityID, bsp, itemID);
  } else if (index > 0) {
    array = bspextra->featsByPos;
    if (array != NULL && bspextra->numfeats > 0 && index <= bspextra->numfeats) {
      item = array [index - 1];
    }
  } else if (sfp != NULL) {
    item = SeqMgrFindSMFeatItemPtr (sfp);
  }
  if (item == NULL) return NULL;

  entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);

  curr = item->sfp;
  if (curr != NULL && context != NULL && (! item->ignore)) {
    context->entityID = entityID;
    context->itemID = item->itemID;
    context->sfp = curr;
    context->sap = item->sap;
    context->bsp = item->bsp;
    context->label = item->label;
    context->left = item->left;
    context->right = item->right;
    context->dnaStop = item->dnaStop;
    context->partialL = item->partialL;
    context->partialR = item->partialR;
    context->farloc = item->farloc;
    context->strand = item->strand;
    if (curr != NULL) {
      context->seqfeattype = curr->data.choice;
    } else {
      context->seqfeattype = FindFeatFromFeatDefType (item->subtype);
    }
    context->featdeftype = item->subtype;
    context->numivals = item->numivals;
    context->ivals = item->ivals;
    context->userdata = NULL;
    context->omdp = (Pointer) omdp;
    context->index = item->index + 1;
  }
  return curr;
}

/*
static ValNodePtr DesiredDescriptorPerBioseq (SeqEntryPtr sep, BioseqPtr bsp,
                                              Uint2 itemID, Uint2 index, ValNodePtr sdp,
                                              SeqMgrDescContext PNTR context)

{
  BioseqSetPtr  bssp;
  ValNodePtr    curr = NULL;
  SeqEntryPtr   tmp;

  if (sep != NULL) {
    if (IS_Bioseq (sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp == NULL) return NULL;
    } else if (IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp == NULL) return NULL;
      for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
        curr = DesiredDescriptorPerBioseq (tmp, NULL, itemID, index, sdp, context);
        if (curr != NULL) return curr;
      }
      return NULL;
    }
  }

  if (bsp == NULL) return NULL;

  while ((curr = SeqMgrGetNextDescriptor (bsp, curr, 0, context)) != NULL) {
    if (itemID > 0 && itemID == context->itemID) return curr;
    if (index > 0 && index == context->index) return curr;
    if (sdp != NULL && sdp == curr) return curr;
  }

  return NULL;
}

NLM_EXTERN ValNodePtr LIBCALL SeqMgrGetDesiredDescriptor (Uint2 entityID, BioseqPtr bsp,
                                                          Uint2 itemID, Uint2 index, ValNodePtr sdp,
                                                          SeqMgrDescContext PNTR context)

{
  SeqMgrDescContext  dfaultcontext;
  SeqEntryPtr        sep;

  if (context == NULL) {
    context = &dfaultcontext;
  }
  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrDescContext));
  }

  if (entityID > 0) {
    sep = SeqMgrGetTopSeqEntryForEntity (entityID);
    if (sep == NULL) return NULL;
    return DesiredDescriptorPerBioseq (sep, NULL, itemID, index, sdp, context);
  } else if (bsp != NULL) {
    return DesiredDescriptorPerBioseq (NULL, bsp, itemID, index, sdp, context);
  }

  return NULL;
}
*/

static SMDescItemPtr SeqMgrFindSMDescItemByID (BioseqExtraPtr bspextra, Uint2 itemID)

{
  SMDescItemPtr PNTR  array;
  SMDescItemPtr       item;
  Int4                L;
  Int4                mid;
  Int4                R;

  if (bspextra == NULL) return NULL;

  array = bspextra->descrsByID;
  if (array != NULL && bspextra->numdescs > 0) {
    L = 0;
    R = bspextra->numdescs - 1;
    while (L < R) {
      mid = (L + R) / 2;
      item = array [mid];
      if (item != NULL && item->itemID < itemID) {
        L = mid + 1;
      } else {
        R = mid;
      }
    }

    item = array [R];
    if (item->itemID == itemID) return item;
  }

  return NULL;
}

static SMDescItemPtr SeqMgrFindSMDescItemBySdp (BioseqExtraPtr bspextra, SeqDescrPtr sdp)

{
  SMDescItemPtr PNTR  array;
  SMDescItemPtr       item;
  Int4                L;
  Int4                mid;
  Int4                R;

  if (bspextra == NULL) return NULL;

  array = bspextra->descrsBySdp;
  if (array != NULL && bspextra->numdescs > 0) {
    L = 0;
    R = bspextra->numdescs - 1;
    while (L < R) {
      mid = (L + R) / 2;
      item = array [mid];
      if (item != NULL && item->sdp < sdp) {
        L = mid + 1;
      } else {
        R = mid;
      }
    }

    item = array [R];
    if (item->sdp == sdp) return item;
  }

  return NULL;
}

static SMDescItemPtr SeqMgrFindSMDescItemByIndex (BioseqExtraPtr bspextra, Uint4 index)

{
  SMDescItemPtr PNTR  array;
  SMDescItemPtr       item;
  Int4                L;
  Int4                mid;
  Int4                R;

  if (bspextra == NULL) return NULL;

  array = bspextra->descrsByIndex;
  if (array != NULL && bspextra->numdescs > 0) {
    L = 0;
    R = bspextra->numdescs - 1;
    while (L < R) {
      mid = (L + R) / 2;
      item = array [mid];
      if (item != NULL && item->index < index) {
        L = mid + 1;
      } else {
        R = mid;
      }
    }

    item = array [R];
    if (item->index == index) return item;
  }

  return NULL;
}

NLM_EXTERN ValNodePtr LIBCALL SeqMgrGetDesiredDescriptor (Uint2 entityID, BioseqPtr bsp,
                                                          Uint4 itemID, Uint4 index, ValNodePtr sdp,
                                                          SeqMgrDescContext PNTR context)

{
  BioseqExtraPtr     bspextra;
  SeqMgrDescContext  dfaultcontext;
  ObjMgrDataPtr      omdp = NULL;
  SMDescItemPtr      sdip = NULL;
  SeqEntryPtr        sep;

  if (context == NULL) {
    context = &dfaultcontext;
  }
  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrDescContext));
  }

  if (entityID > 0) {
    sep = SeqMgrGetTopSeqEntryForEntity (entityID);
    if (sep != NULL) {
      omdp = SeqMgrGetOmdpForPointer (sep->data.ptrvalue);
    }
  } else if (bsp != NULL) {
    omdp = SeqMgrGetOmdpForBioseq (bsp);
    entityID = ObjMgrGetEntityIDForPointer (bsp);
  }

  if (omdp == NULL) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;

  if (itemID > 0) {
    sdip = SeqMgrFindSMDescItemByID (bspextra, itemID);
  } else if (index > 0) {
    sdip = SeqMgrFindSMDescItemByIndex (bspextra, index);
  } else if (sdp != NULL) {
    sdip = SeqMgrFindSMDescItemBySdp (bspextra, sdp);
  }

  if (sdip == NULL) return NULL;

  context->entityID = entityID;
  context->itemID = sdip->itemID;
  context->sdp = sdip->sdp;
  context->sep = sdip->sep;
  context->level = sdip->level;
  context->seqdesctype = sdip->seqdesctype;
  context->userdata = NULL;
  context->omdp = omdp;
  context->index = sdip->index;

  return sdip->sdp;
}

/*****************************************************************************
*
*   RecordFeaturesInBioseqs callback explores bioseqs, bioseq sets, and features,
*     keeping a running total of the descriptor item counts, and records specific
*     information about features on each bioseq
*
*****************************************************************************/

typedef struct extraindex {
  BioseqPtr       lastbsp;
  SeqAnnotPtr     lastsap;
  BioseqSetPtr    lastbssp;
  ValNodePtr      alignhead;
  ValNodePtr      lastalign;
  SMSeqIdxPtr     segpartail;
  Int4            cumulative;
  Uint4           bspcount;
  Uint4           aligncount;
  Uint4           descrcount;
  Uint4           featcount;
  Int4            seqlitid;
  Boolean         flip;
} ExtraIndex, PNTR ExtraIndexPtr;

static void SetDescriptorCounts (ValNodePtr sdp, ExtraIndexPtr exindx, Pointer thisitem, Uint2 thistype)

{
  Uint4          count = 0;
  ObjMgrDataPtr  omdp;

  /* count bioseq or bioseq set descriptors, to calculate omdp.lastDescrItemID */

  if (sdp == NULL || exindx == NULL) return;
  if (thistype == OBJ_BIOSEQ) {
    omdp = SeqMgrGetOmdpForBioseq ((BioseqPtr) thisitem);
  } else {
    omdp = SeqMgrGetOmdpForPointer (thisitem);
  }
  if (omdp == NULL) return;

  omdp->lastDescrItemID = exindx->descrcount;
  while (sdp != NULL) {
    count++;
    sdp = sdp->next;
  }
  exindx->descrcount += count;
}

static void CreateBioseqExtraBlock (ObjMgrDataPtr omdp, BioseqPtr bsp)

{
  BioseqExtraPtr  bspextra;

  if (omdp == NULL || omdp->extradata != NULL) return;

  bspextra = (BioseqExtraPtr) MemNew (sizeof (BioseqExtra));
  omdp->extradata = (Pointer) bspextra;
  if (bspextra == NULL) return;

  omdp->reapextra = SeqMgrReapBioseqExtraFunc;
  omdp->reloadextra = SeqMgrReloadBioseqExtraFunc;
  omdp->freeextra = SeqMgrFreeBioseqExtraFunc;

  bspextra->bsp = bsp;
  bspextra->omdp = omdp;
  bspextra->min = INT4_MAX;
}

static Boolean CountAlignmentsProc (GatherObjectPtr gop)

{
  ExtraIndexPtr  exindx;

  if (gop == NULL || gop->itemtype != OBJ_SEQALIGN) return TRUE;
  exindx = (ExtraIndexPtr) gop->userdata;
  if (exindx == NULL) return TRUE;
  (exindx->aligncount)++;
  return TRUE;
}

static Boolean CollectAlignsProc (GatherObjectPtr gop)

{
  SeqAlignPtr PNTR  alignsByID;

  if (gop == NULL || gop->itemtype != OBJ_SEQALIGN) return TRUE;
  alignsByID = (SeqAlignPtr PNTR) gop->userdata;
  if (alignsByID == NULL) return TRUE;
  alignsByID [gop->itemID] = (SeqAlignPtr) gop->dataptr;
  return TRUE;
}

NLM_EXTERN void LIBCALL SeqMgrIndexAlignments (Uint2 entityID)

{
  SeqAlignPtr PNTR  alignsByID;
  BioseqExtraPtr    bspextra;
  ExtraIndex        exind;
  Boolean           objMgrFilter [OBJ_MAX];
  ObjMgrDataPtr     omdp;

  if (entityID == 0) return;

  /* count alignments */

  exind.lastbsp = NULL;
  exind.lastsap = NULL;
  exind.lastbssp = NULL;
  exind.alignhead = NULL;
  exind.lastalign = NULL;
  exind.segpartail = NULL;
  exind.bspcount = 0;
  exind.aligncount = 0;
  exind.descrcount = 0;
  exind.featcount = 0;
  exind.seqlitid = 0;

  MemSet ((Pointer) objMgrFilter, 0, sizeof (objMgrFilter));
  objMgrFilter [OBJ_SEQALIGN] = TRUE;
  GatherObjectsInEntity (entityID, 0, NULL, CountAlignmentsProc, (Pointer) &exind, objMgrFilter);

  omdp = ObjMgrGetData (entityID);
  if (omdp != NULL) {

    CreateBioseqExtraBlock (omdp, NULL);
    bspextra = (BioseqExtraPtr) omdp->extradata;
    if (bspextra != NULL) {

      /* get rid of previous lookup array */

      bspextra->alignsByID = MemFree (bspextra->alignsByID);
      bspextra->numaligns = 0;

      /* alignment ID to SeqAlignPtr index always goes on top of entity */

      if (exind.aligncount > 0) {
        alignsByID = (SeqAlignPtr PNTR) MemNew (sizeof (SeqAlignPtr) * (exind.aligncount + 2));
        if (alignsByID != NULL) {

          /* copy SeqAlignPtr for each itemID */

          GatherObjectsInEntity (entityID, 0, NULL, CollectAlignsProc, (Pointer) alignsByID, objMgrFilter);

          bspextra->alignsByID = alignsByID;
          bspextra->numaligns = exind.aligncount;
        }
      }
    }
  }
}

static SeqIdPtr SeqIdWithinBioseq (BioseqPtr bsp, SeqLocPtr slp)

{
  SeqIdPtr  a;
  SeqIdPtr  b;

  if (bsp == NULL || slp == NULL) return NULL;
  a = SeqLocId (slp);
  if (a == NULL) return NULL;
  for (b = bsp->id; b != NULL; b = b->next) {
    if (SeqIdComp (a, b) == SIC_YES) return b;
  }
  return NULL;
}

static void FindGPS (BioseqSetPtr bssp, Pointer userdata)

{
  BoolPtr  is_gpsP;

  if (bssp == NULL || bssp->_class != BioseqseqSet_class_gen_prod_set) return;
  is_gpsP = (BoolPtr) userdata;
  *is_gpsP = TRUE;
}

static void ProcessFeatureProducts (SeqFeatPtr sfp, Uint2 itemID, GatherObjectPtr gop)

{
  BioseqPtr       bsp;
  BioseqExtraPtr  bspextra;
  Char            buf [81];
  CharPtr         ctmp;
  Int4            diff;
  CharPtr         loclbl;
  Int4            min;
  ObjMgrDataPtr   omdp;
  CharPtr         prodlbl;
  SeqFeatPtr      prt;
  CharPtr         ptmp;
  SeqAnnotPtr     sap;
  SeqIdPtr        sip;
  SeqLocPtr       slp;
  ValNode         vn;

  if (sfp == NULL || sfp->product == NULL) return;
  if (sfp->data.choice != SEQFEAT_CDREGION &&
      sfp->data.choice != SEQFEAT_RNA &&
      sfp->data.choice != SEQFEAT_PROT) return;

  sip = SeqLocId (sfp->product);
  if (sip == NULL) return;
  bsp = BioseqFind (sip);
  if (bsp == NULL) return;
  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return;

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) {
    CreateBioseqExtraBlock (omdp, bsp);
    bspextra = (BioseqExtraPtr) omdp->extradata;
  }
  if (bspextra == NULL) return;

  /* cds or rna reference stored in product bioseq's omdp.cdsOrRnaFeat */

  if (bspextra->cdsOrRnaFeat != NULL && bspextra->cdsOrRnaFeat != sfp) {
    FeatDefLabel (sfp, buf, sizeof (buf) - 1, OM_LABEL_CONTENT);
    ctmp = SeqLocPrint (sfp->location);
    loclbl = ctmp;
    if (loclbl == NULL) {
      loclbl = "?";
    }
    ptmp = SeqLocPrint (sfp->product);
    prodlbl = ptmp;
    if (prodlbl == NULL) {
      prodlbl = "?";
    }
    {
      GatherContext     gc;
      GatherContextPtr  gcp;
      Boolean           is_gps = FALSE;
      SeqEntryPtr       sep;
      MemSet ((Pointer) &gc, 0, sizeof (GatherContext));
      gcp = &gc;
      gc.entityID = gop->entityID;
      gc.itemID = gop->itemID;
      gc.thistype = gop->itemtype;
      sep = GetTopSeqEntryForEntityID (gop->entityID);
      VisitSetsInSep (sep, (Pointer) &is_gps, FindGPS);
      if (! is_gps) {
        ErrPostItem (SEV_WARNING, 0, 0,
                     "SeqMgr indexing cds or rna progenitor already set - Feature: %s - Location [%s] - Product [%s]",
                     buf, loclbl, prodlbl);
      }
    }
    MemFree (ctmp);
    MemFree (ptmp);
  }

  /* if (omdp->tempload == TL_NOT_TEMP) { */
    bspextra->cdsOrRnaFeat = sfp;
  /* } */

  /* add to prodlisthead list for gather by get_feats_product */

  ValNodeAddPointer (&(bspextra->prodlisthead), 0, (Pointer) sfp);

  if (sfp->data.choice == SEQFEAT_RNA || sfp->data.choice == SEQFEAT_PROT) return;

  /* if protFeat exists it was set by exhaustive gather on protein bioseq */

  if (bspextra->protFeat != NULL) return;

  /* calculate largest protein feature on cds's product bioseq */

  min = INT4_MAX;
  vn.choice = SEQLOC_WHOLE;
  vn.data.ptrvalue = (Pointer) bsp->id;
  vn.next = NULL;
  slp = (Pointer) (&vn);

  sap = bsp->annot;
  while (sap != NULL) {
    if (sap->type == 1) {
      prt = (SeqFeatPtr) sap->data;
      while (prt != NULL) {
        if (prt->data.choice == SEQFEAT_PROT) {

          /* get SeqId in bioseq that matches SeqId used for location */

          vn.data.ptrvalue = SeqIdWithinBioseq (bsp, prt->location);

          diff = SeqLocAinB (prt->location, slp);
          if (diff >= 0) {
            if (diff < min) {
              min = diff;
              /* if (omdp->tempload == TL_NOT_TEMP) { */
                bspextra->protFeat = prt;
              /* } */
            }
          }
        }
        prt = prt->next;
      }
    }
    sap = sap->next;
  }
}

static void RecordOneFeature (BioseqExtraPtr bspextra, ObjMgrDataPtr omdp,
                              BioseqPtr bsp, ExtraIndexPtr exindx, SeqFeatPtr sfp,
                              Int4 left, Int4 right, Uint4 itemID, Uint2 subtype,
                              Boolean farloc, Boolean ignore)

{
  Char            buf [129];
  SMFeatBlockPtr  curr;
  Int4            from;
  Int2            i;
  SMFeatItemPtr   item;
  Int4Ptr         ivals;
  SeqLocPtr       loc;
  SMFeatBlockPtr  next;
  Int2            numivals = 0;
  CharPtr         ptr;
  SeqIdPtr        sip;
  SeqLocPtr       slp = NULL;
  Uint1           strand;
  Int4            swap;
  Int4            to;

  if (bspextra == NULL || omdp == NULL || bsp == NULL || exindx == NULL || sfp == NULL) return;

  if (bspextra->featlisttail != NULL) {

    /* just in case blocksize should was not set for some reason */

    if (bspextra->blocksize < 1) {
      bspextra->blocksize = 5;
    }

    curr = bspextra->featlisttail;
    if (curr->index >= bspextra->blocksize) {

      /* allocate next chunk in linked list of blocks */

      next = (SMFeatBlockPtr) MemNew (sizeof (SMFeatBlock));
      curr->next = next;

      if (next != NULL) {
        bspextra->featlisttail = next;
        curr = next;
      }
    }

    if (curr->index < bspextra->blocksize) {

      /* allocate data block if not yet done for this chunk */

      if (curr->data == NULL) {
        curr->data = (SMFeatItemPtr) MemNew (sizeof (SMFeatItem) * (size_t) (bspextra->blocksize));
      }

      /* now record desired information about current feature */

      if (curr->data != NULL) {
        item = &(curr->data [curr->index]);
        /* if (omdp->tempload == TL_NOT_TEMP) { */
          item->sfp = sfp;
          item->sap = exindx->lastsap;
          item->bsp = bsp;
        /* } */
        FeatDefLabel (sfp, buf, sizeof (buf) - 1, OM_LABEL_CONTENT);
        ptr = buf;
        if (sfp->data.choice == SEQFEAT_RNA) {
          ptr = StringStr (buf, "RNA-");
          if (ptr != NULL) {
            ptr += 4;
          } else {
            ptr = buf;
          }
        }
        item->label = StringSaveNoNull (ptr);
        item->left = left;
        item->right = right;
        if (exindx->flip) {
          item->left = bsp->length - right;
          item->right = bsp->length - left;
        }
		item->dnaStop = -1;
        CheckSeqLocForPartial (sfp->location, &(item->partialL), &(item->partialR));
        item->farloc = farloc;
        /*
        item->strand = SeqLocStrand (sfp->location);
        if (exindx->flip) {
          item->strand = StrandCmp (item->strand);
        }
        */
        if (subtype == 0) {
          subtype = FindFeatDefType (sfp);
        }
        item->subtype = subtype;
        item->itemID = itemID;
        item->ignore = ignore;
        item->overlap = -1;

        /* record start/stop pairs of intervals on target bioseq */

        /*
        single_interval = (Boolean) (item->subtype == FEATDEF_GENE ||
                                     item->subtype == FEATDEF_PUB);
        */
        loc = SeqLocMergeEx (bsp, sfp->location, NULL, FALSE, FALSE, FALSE, FALSE);

        if (exindx->flip) {
          sip = SeqIdFindBest (bsp->id, 0);
          slp = SeqLocCopyRegion (sip, loc, bsp, 0, bsp->length - 1, Seq_strand_minus, FALSE);
          SeqLocFree (loc);
          loc = slp;
        }
        /* record strand relative to segmented parent */
        item->strand = SeqLocStrand (loc);
        if (exindx->flip) {
          item->strand = StrandCmp (item->strand);
        }
        strand = item->strand;

        slp = NULL;
        while ((slp = SeqLocFindNext (loc, slp)) != NULL) {
          numivals++;
        }
        if (numivals > 0) {
          ivals = MemNew (sizeof (Int4) * (numivals * 2));
          item->ivals = ivals;
          item->numivals = numivals;
          slp = NULL;
          i = 0;
          while ((slp = SeqLocFindNext (loc, slp)) != NULL) {
            from = SeqLocStart (slp);
            to = SeqLocStop (slp);
            if (strand == Seq_strand_minus) {
              swap = from;
              from = to;
              to = swap;
            }
            ivals [i] = from;
            i++;
            ivals [i] = to;
            i++;
          }
        }
        SeqLocFree (loc);
      }

      /* increment count on current block */

      (curr->index)++;

      /* count all features, per bioseq and per entity */

      (bspextra->numfeats)++;
      (exindx->featcount)++;

    }
  }
}

/* callback for recording features and descriptor, prot, and cdsOrRna information */

static Boolean RecordFeaturesInBioseqs (GatherObjectPtr gop)

{
  BioseqPtr       bsp = NULL;
  BioseqExtraPtr  bspextra;
  BioseqSetPtr    bssp = NULL;
  Char            buf [81];
  Int2            count;
  CharPtr         ctmp;
  Int4            diff;
  ExtraIndexPtr   exindx;
  Int4            left;
  CharPtr         loclbl;
  ObjMgrDataPtr   omdp;
  Int4            right;
  SeqAnnotPtr     sap = NULL;
  ValNodePtr      sdp = NULL;
  SeqFeatPtr      sfp = NULL;
  SeqAlignPtr     sal = NULL;
  SeqLocPtr       slp;
  Int4            swap;
  SeqFeatPtr      tmp;
  Boolean         usingLocalBsp = FALSE;
  ValNode         vn;
  ValNodePtr      vnp;

  switch (gop->itemtype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) gop->dataptr;
      if (bsp == NULL) return TRUE;
      sdp = bsp->descr;
      break;
    case OBJ_BIOSEQSET :
      bssp = (BioseqSetPtr) gop->dataptr;
      if (bssp == NULL) return TRUE;
      sdp = bssp->descr;
      break;
    case OBJ_SEQANNOT :
      sap = (SeqAnnotPtr) gop->dataptr;
      break;
    case OBJ_SEQFEAT :
      sfp = (SeqFeatPtr) gop->dataptr;
      break;
    case OBJ_SEQALIGN :
      sal = (SeqAlignPtr) gop->dataptr;
      break;
    default :
      return TRUE;
  }

  exindx = (ExtraIndexPtr) gop->userdata;
  if (exindx == NULL) return FALSE;

  /* save bspItemID to support bioseq explore functions */

  if (bsp != NULL) {

    (exindx->bspcount)++;

    /* save last BioseqPtr to check first for appropriate bioseq */

    exindx->lastbsp = bsp;

    /* blocksize for new block based only on features packaged on bioseq */

    exindx->lastbssp = NULL;

    omdp = SeqMgrGetOmdpForBioseq (bsp);
    if (omdp != NULL) {
      bspextra = (BioseqExtraPtr) omdp->extradata;
      if (bspextra == NULL) {
        CreateBioseqExtraBlock (omdp, bsp);
        bspextra = (BioseqExtraPtr) omdp->extradata;
      }
      if (bspextra != NULL) {
        bspextra->bspItemID = gop->itemID;
        bspextra->bspIndex = exindx->bspcount;
      }
    }
  }

  /* save last BioseqSetPtr to calculate blocksize from bioseq set and bioseq features,
     features on bioseq set presumably being CDS or mRNA and applying only to nucleotides */

  if (bssp != NULL) {
    exindx->lastbssp = bssp;
  }

  /* count bioseq or bioseq set descriptors, to calculate lastDescrItemID */

  if (sdp != NULL) {
    SetDescriptorCounts (sdp, exindx, gop->dataptr, gop->itemtype);
    return TRUE;
  }

  /* save SeqAnnotPtr containing next features to be gathered */

  if (sap != NULL) {
    exindx->lastsap = sap;
    return TRUE;
  }

  /* record SeqAlignPtr in val node list - expects all itemIDs to be presented */

  if (sal != NULL) {
    vnp = ValNodeAddPointer (&(exindx->lastalign), 0, (Pointer) sal);
    if (exindx->alignhead == NULL) {
      exindx->alignhead = exindx->lastalign;
    }
    exindx->lastalign = vnp;
    (exindx->aligncount)++;
    return TRUE;
  }

  /* otherwise index features on every bioseq in entity */

  if (sfp == NULL) return TRUE;

  /* cds or rna reference stored in product bioseq's omdp.cdsOrRnaFeat,
     best protein feature in omdp.protFeat (do before adding CDS) */

  if (sfp->product != NULL) {
    ProcessFeatureProducts (sfp, gop->itemID, gop);
  }

  bsp = FindAppropriateBioseq (sfp->location, exindx->lastbsp);

  /* failure here can be due to SeqLoc that references far accession */

  if (bsp == NULL) {

    /* if far accession, find first local bioseq on any location interval */

    bsp = FindFirstLocalBioseq (sfp->location);

    /* report whether far accession was able to be handled */

    FeatDefLabel (sfp, buf, sizeof (buf) - 1, OM_LABEL_CONTENT);
    ctmp = SeqLocPrint (sfp->location);
    loclbl = ctmp;
    if (loclbl == NULL) {
      loclbl = "?";
    }

    if (bsp == NULL) {
      {
        GatherContext     gc;
        GatherContextPtr  gcp;
        Char              lastbspid [41];
        SeqIdPtr          sip;
        MemSet ((Pointer) &gc, 0, sizeof (GatherContext));
        gcp = &gc;
        gc.entityID = gop->entityID;
        gc.itemID = gop->itemID;
        gc.thistype = gop->itemtype;
        lastbspid [0] = '\0';
        if (exindx->lastbsp != NULL) {
          sip = SeqIdFindBest (exindx->lastbsp->id, 0);
          if (sip != NULL) {
            SeqIdWrite (sip, lastbspid, PRINTID_FASTA_LONG, sizeof (lastbspid));
          }
        }
        ErrPostItem (SEV_WARNING, 0, 0,
                     "SeqMgr indexing feature location problem - Feature: %s - Location [%s] - Record [%s]",
                     buf, loclbl, lastbspid);
      }
    } else {
      /*
      ErrPostItem (SEV_INFO, 0, 0,
                   "SeqMgr indexing detected and handled far accession - Feature: %s - Location [%s]",
                   buf, loclbl);
      */
    }
    MemFree (ctmp);

    if (bsp == NULL) return TRUE;
    usingLocalBsp = TRUE;
  }

  /* assume subsequent features will be on this bioseq */

  exindx->lastbsp = bsp;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL) return TRUE;

  /* now prepare for adding feature to index */

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) {
    CreateBioseqExtraBlock (omdp, bsp);
    bspextra = (BioseqExtraPtr) omdp->extradata;
  }
  if (bspextra == NULL) return TRUE;

  /* get extreme left and right extents of feature location on near bioseqs */
  /* merge here to get correct extremes even in case of trans-splicing */

  /* but this broke the handling of genes spanning the origin, so cannot do */
  /*
  slp = SeqLocMergeEx (bsp, sfp->location, NULL, TRUE, TRUE, FALSE, FALSE);
  */
  slp = sfp->location;
  left = GetOffsetInNearBioseq (slp, bsp, SEQLOC_LEFT_END);
  right = GetOffsetInNearBioseq (slp, bsp, SEQLOC_RIGHT_END);
  /*
  SeqLocFree (slp);
  */
  if (left == -1 || right == -1) return TRUE;

  /* if indexing protein bioseq, store largest protein feature */

  if (sfp->data.choice == SEQFEAT_PROT) {
    vn.choice = SEQLOC_WHOLE;
    vn.data.ptrvalue = (Pointer) bsp->id;
    vn.next = NULL;
    slp = (Pointer) &vn;

    /* get SeqId in bioseq that matches SeqId used for location */

    vn.data.ptrvalue = (Pointer) SeqIdWithinBioseq (bsp, sfp->location);

    diff = SeqLocAinB (sfp->location, slp);
    if (diff >= 0) {
      if (diff < bspextra->min) {
        bspextra->min = diff;
        /* if (omdp->tempload == TL_NOT_TEMP) { */
          bspextra->protFeat = sfp;
        /* } */
      }
    }
  }

  /* add feature item to linked list of blocks */

  if (bspextra->featlisthead == NULL) {
    bspextra->featlisthead = (SMFeatBlockPtr) MemNew (sizeof (SMFeatBlock));

    /* for first feature indexed on this bioseq, quickly see if few or many
       additional features, since most features on a bioseq are packaged in
       the same list, and most proteins only have one bioseq */

    for (tmp = sfp, count = 0;
         tmp != NULL && count < 50;
         tmp = tmp->next, count++) continue;

    /* extend count if above features were packaged on a bioseq set (presumably CDS or mRNA) */

    if (exindx->lastbssp != NULL) {
      for (sap = bsp->annot; sap != NULL; sap = sap->next) {
        if (sap->type == 1) {

          for (tmp = (SeqFeatPtr) sap->data;
               tmp != NULL && count < 50;
               tmp = tmp->next, count++) continue;

        }
      }
    }

    bspextra->blocksize = count;
  }
  if (bspextra->featlisttail == NULL) {
    bspextra->featlisttail = bspextra->featlisthead;
  }

  if (bspextra->featlisttail != NULL) {

    /* if feature spans origin, record with left < 0 */

    if (left > right && bsp->topology == TOPOLOGY_CIRCULAR) {
      left -= bsp->length;
    }

    /* some trans-spliced locations can confound GetOffsetInNearBioseq, so normalize here */

    if (left > right) {
      swap = left;
      left = right;
      right = swap;
    }

    RecordOneFeature (bspextra, omdp, bsp, exindx, sfp, left,
                      right, gop->itemID, gop->subtype, usingLocalBsp, FALSE);

    /* record gene, publication, and biosource features twice if spanning the origin */

    if (left < 0 && bsp->topology == TOPOLOGY_CIRCULAR) {
      if (sfp->data.choice == SEQFEAT_GENE ||
          sfp->data.choice == SEQFEAT_PUB ||
          sfp->data.choice == SEQFEAT_BIOSRC) {

        RecordOneFeature (bspextra, omdp, bsp, exindx, sfp, left + bsp->length,
                          right + bsp->length, gop->itemID, gop->subtype, usingLocalBsp, TRUE);

      }
    }
  }

  return TRUE;
}

/*****************************************************************************
*
*   RecordSegmentsInBioseqs callback explores bioseq segments
*
*****************************************************************************/

static Boolean RecordSegmentsInBioseqs (GatherObjectPtr gop)

{
  BioseqPtr       bsp = NULL;
  BioseqExtraPtr  bspextra;
  Char            buf [80];
  Dbtag           db;
  DeltaSeqPtr     dsp;
  ExtraIndexPtr   exindx;
  Int4            from;
  Boolean         isSeg = FALSE;
  ObjectId        oi;
  ObjMgrDataPtr   omdp;
  SMSeqIdxPtr     segpartptr;
  SeqId           si;
  SeqIdPtr        sid;
  SeqInt          sint;
  SeqIntPtr       sipp;
  SeqLoc          sl;
  SeqLitPtr       slitp;
  SeqLocPtr       slp = NULL;
  Uint1           strand;
  Int4            to;

  exindx = (ExtraIndexPtr) gop->userdata;
  if (exindx == NULL) return FALSE;

  switch (gop->itemtype) {
    case OBJ_BIOSEQ :
      bsp = (BioseqPtr) gop->dataptr;
      if (bsp == NULL) return TRUE;
      break;
    case OBJ_BIOSEQ_SEG :
      isSeg = TRUE;
      slp = (SeqLocPtr) gop->dataptr;
      if (slp == NULL) return TRUE;
      break;
    case OBJ_BIOSEQ_DELTA :
      dsp = (DeltaSeqPtr) gop->dataptr;
      if (dsp == NULL) return TRUE;
      if (dsp->choice == 1) {
        slp = (SeqLocPtr) dsp->data.ptrvalue;
      } else if (dsp->choice == 2) {
        slitp = (SeqLitPtr) dsp->data.ptrvalue;
        if (slitp != NULL) {
          /* fake seqloc, same as in DeltaSeqsToSeqLocs */
          MemSet ((Pointer) &sl, 0, sizeof (SeqLoc));
          MemSet ((Pointer) &sint, 0, sizeof (SeqInt));
          MemSet ((Pointer) &si, 0, sizeof (SeqId));
          MemSet ((Pointer) &db, 0, sizeof (Dbtag));
          MemSet ((Pointer) &oi, 0, sizeof (ObjectId));
          sl.choice = SEQLOC_INT;
          sl.data.ptrvalue = (Pointer) &sint;
          sint.from = 0;
          sint.to = slitp->length - 1;
          si.choice = SEQID_GENERAL;
          si.data.ptrvalue = (Pointer) &db;
          db.db = "SeqLit";
          db.tag = &oi;
          (exindx->seqlitid)++;
          oi.id = exindx->seqlitid;
          sint.id = &si;
          slp = &sl;
        }
      }
      break;
    default :
      return TRUE;
  }

  if (bsp != NULL) {
    if (bsp->repr == Seq_repr_seg) {
      exindx->lastbsp = bsp;
    } else if (bsp->repr == Seq_repr_delta) {
      exindx->lastbsp = bsp;
    } else {
      exindx->lastbsp = NULL;
    }
    exindx->cumulative = 0;
    return TRUE;
  }

  if (slp == NULL) return TRUE;

  bsp = exindx->lastbsp;
  if (bsp == NULL) return TRUE;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL) return TRUE;

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) {
    CreateBioseqExtraBlock (omdp, bsp);
    bspextra = (BioseqExtraPtr) omdp->extradata;
  }
  if (bspextra == NULL) return TRUE;

  if (slp->choice == SEQLOC_INT && slp->data.ptrvalue != NULL) {
    sipp = (SeqIntPtr) (slp->data.ptrvalue);
    from = sipp->from;
    to = sipp->to;
    strand = sipp->strand;
  } else {
    from = 0;
    to = SeqLocLen (slp) - 1;
    strand = SeqLocStrand (slp);
  }

  if (to - from + 1 < 1) return TRUE;

  /* create and fill in SMSeqIdx element */

  segpartptr = MemNew (sizeof (SMSeqIdx));
  if (segpartptr != NULL) {
    sid = SeqLocId (slp);
    if (MakeReversedSeqIdString (sid, buf, sizeof (buf) - 1)) {
      segpartptr->slp = AsnIoMemCopy (slp,
                                      (AsnReadFunc) SeqLocAsnRead,
                                      (AsnWriteFunc) SeqLocAsnWrite);
      segpartptr->seqIdOfPart = StringSave (buf);
      if (isSeg) {

        /* only annotate parentBioseq for segmented, not delta bioseq */

        segpartptr->parentBioseq = bsp;
      } else {
        segpartptr->parentBioseq = NULL;
      }
      segpartptr->cumOffset = exindx->cumulative;
      segpartptr->from = from;
      segpartptr->to = to;
      segpartptr->strand = strand;
      segpartptr->itemID = gop->itemID;
    }
  }

  exindx->cumulative += (to - from + 1);

  /* link into segparthead list of parts IDs */

  if (bspextra->segparthead == NULL) {
    bspextra->segparthead = segpartptr;
    exindx->segpartail = segpartptr;
  } else if (exindx->segpartail != NULL) {
    exindx->segpartail->next = segpartptr;
    exindx->segpartail = segpartptr;
  }

  return TRUE;
}

/*****************************************************************************
*
*   SortFeatItemListByID callback sorts array into feature item table by itemID
*   SortFeatItemListBySfp sorts by feature pointer
*   SortFeatItemListByPos sorts by feature position
*   SortFeatItemListByRev sorts by reverse feature position
*
*****************************************************************************/

static int LIBCALLBACK SortFeatItemListByID (VoidPtr vp1, VoidPtr vp2)

{
  SMFeatItemPtr PNTR  spp1 = vp1;
  SMFeatItemPtr PNTR  spp2 = vp2;
  SMFeatItemPtr       sp1;
  SMFeatItemPtr       sp2;

  if (spp1 == NULL || spp2 == NULL) return 0;
  sp1 = *((SMFeatItemPtr PNTR) spp1);
  sp2 = *((SMFeatItemPtr PNTR) spp2);
  if (sp1 == NULL || sp2 == NULL) return 0;

  /* sort by feature itemID */

  if (sp1->itemID > sp2->itemID) {
    return 1;
  } else if (sp1->itemID < sp2->itemID) {
    return -1;

  /* for duplicated genes, etc., that cross origin, put ignored item last for binary search */

  } else if (sp1->ignore) {
    return 1;
  } else if (sp2->ignore) {
    return -1;
  }

  return 0;
}

static int LIBCALLBACK SortFeatItemListBySfp (VoidPtr vp1, VoidPtr vp2)

{
  SMFeatItemPtr PNTR  spp1 = vp1;
  SMFeatItemPtr PNTR  spp2 = vp2;
  SMFeatItemPtr       sp1;
  SMFeatItemPtr       sp2;

  if (spp1 == NULL || spp2 == NULL) return 0;
  sp1 = *((SMFeatItemPtr PNTR) spp1);
  sp2 = *((SMFeatItemPtr PNTR) spp2);
  if (sp1 == NULL || sp2 == NULL) return 0;

  /* sort by SeqFeatPtr value */

  if (sp1->sfp > sp2->sfp) {
    return 1;
  } else if (sp1->sfp < sp2->sfp) {
    return -1;

  /* for duplicated genes, etc., that cross origin, put ignored item last for binary search */

  } else if (sp1->ignore) {
    return 1;
  } else if (sp2->ignore) {
    return -1;
  }

  return 0;
}

static int LIBCALLBACK SortFeatItemListByLabel (VoidPtr vp1, VoidPtr vp2)

{
  int                 compare;
  SMFeatItemPtr PNTR  spp1 = vp1;
  SMFeatItemPtr PNTR  spp2 = vp2;
  SMFeatItemPtr       sp1;
  SMFeatItemPtr       sp2;

  if (spp1 == NULL || spp2 == NULL) return 0;
  sp1 = *((SMFeatItemPtr PNTR) spp1);
  sp2 = *((SMFeatItemPtr PNTR) spp2);
  if (sp1 == NULL || sp2 == NULL) return 0;

  /* sort by label value */

  compare = StringICmp (sp1->label, sp2->label);
  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  /* for duplicated genes, etc., that cross origin, put ignored item last for binary search */

  if (sp1->ignore) {
    return 1;
  } else if (sp2->ignore) {
    return -1;
  }

  return 0;
}

static int LIBCALLBACK SortFeatItemListByLocusTag (VoidPtr vp1, VoidPtr vp2)

{
  int                 compare;
  GeneRefPtr          grp1;
  GeneRefPtr          grp2;
  SeqFeatPtr          sfp1;
  SeqFeatPtr          sfp2;
  SMFeatItemPtr PNTR  spp1 = vp1;
  SMFeatItemPtr PNTR  spp2 = vp2;
  SMFeatItemPtr       sp1;
  SMFeatItemPtr       sp2;

  if (spp1 == NULL || spp2 == NULL) return 0;
  sp1 = *((SMFeatItemPtr PNTR) spp1);
  sp2 = *((SMFeatItemPtr PNTR) spp2);
  if (sp1 == NULL || sp2 == NULL) return 0;

  sfp1 = sp1->sfp;
  sfp2 = sp2->sfp;
  if (sfp1 == NULL || sfp2 == NULL) return 0;

  if (sfp1->data.choice != SEQFEAT_GENE || sfp2->data.choice != SEQFEAT_GENE) return 0;
  grp1 = (GeneRefPtr) sfp1->data.value.ptrvalue;
  grp2 = (GeneRefPtr) sfp2->data.value.ptrvalue;
  if (grp1 == NULL || grp2 == NULL) return 0;

  /* sort by locus_tag */

  compare = StringICmp (grp1->locus_tag, grp2->locus_tag);
  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  /* sort by locus if locus_tag is identical */

  compare = StringICmp (grp1->locus, grp2->locus);
  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  /* for duplicated genes that cross origin, put ignored item last for binary search */

  if (sp1->ignore) {
    return 1;
  } else if (sp2->ignore) {
    return -1;
  }

  return 0;
}

static int LIBCALLBACK SortFeatItemListByPos (VoidPtr vp1, VoidPtr vp2)

{
  Int2                compare;
  CdRegionPtr         crp1;
  CdRegionPtr         crp2;
  Int2                i;
  Int2                j;
  Int2                numivals;
  SeqAnnotPtr         sap1;
  SeqAnnotPtr         sap2;
  SMFeatItemPtr PNTR  spp1 = vp1;
  SMFeatItemPtr PNTR  spp2 = vp2;
  SMFeatItemPtr       sp1;
  SMFeatItemPtr       sp2;
  SeqFeatPtr          sfp1;
  SeqFeatPtr          sfp2;
  Uint1               subtype1;
  Uint1               subtype2;

  if (spp1 == NULL || spp2 == NULL) return 0;
  sp1 = *((SMFeatItemPtr PNTR) spp1);
  sp2 = *((SMFeatItemPtr PNTR) spp2);
  if (sp1 == NULL || sp2 == NULL) return 0;

  /* feature with smallest left extreme is first */

  if (sp1->left > sp2->left) {
    return 1;
  } else if (sp1->left < sp2->left) {
    return -1;

  /* reversing order so that longest feature is first */

  } else if (sp1->right > sp2->right) {
    return -1; /* was 1 */
  } else if (sp1->right < sp2->right) {
    return 1; /* was -1 */
  }

  /* given identical extremes, put operon features first */

  if (sp1->subtype == FEATDEF_operon && sp2->subtype != FEATDEF_operon) {
    return -1;
  } else if (sp2->subtype == FEATDEF_operon && sp1->subtype != FEATDEF_operon) {
    return 1;
  }

  /* then gene features */

  if (sp1->subtype == FEATDEF_GENE && sp2->subtype != FEATDEF_GENE) {
    return -1;
  } else if (sp2->subtype == FEATDEF_GENE && sp1->subtype != FEATDEF_GENE) {
    return 1;
  }

  /* then rna features */

  subtype1 = FindFeatFromFeatDefType (sp1->subtype);
  subtype2 = FindFeatFromFeatDefType (sp2->subtype);

  if (subtype1 == SEQFEAT_RNA && subtype2 != SEQFEAT_RNA) {
    return -1;
  } else if (subtype2 == SEQFEAT_RNA && subtype1 != SEQFEAT_RNA) {
    return 1;
  }

  /* then cds features */

  if (sp1->subtype == FEATDEF_CDS && sp2->subtype != FEATDEF_CDS) {
    return -1;
  } else if (sp2->subtype == FEATDEF_CDS && sp1->subtype != FEATDEF_CDS) {
    return 1;
  }

  /* next compare internal intervals */

  numivals = MIN (sp1->numivals, sp2->numivals);
  if (numivals > 0 && sp1->ivals != NULL && sp2->ivals != NULL) {
    for (i = 0, j = 0; i < numivals; i++) {

      /* check left interval */

      if (sp1->ivals [j] > sp2->ivals [j]) {
        return 1;
      } else if (sp1->ivals [j] < sp2->ivals [j]) {
        return -1;
      }
      j++;

      /* check right interval */

      if (sp1->ivals [j] > sp2->ivals [j]) {
        return -1; /* was 1 */
      } else if (sp1->ivals [j] < sp2->ivals [j]) {
        return 1; /* was -1 */
      }
      j++;
    }
  }

  /* one with fewer intervals goes first */

  if (sp1->numivals > sp2->numivals) {
    return 1;
  } else if (sp1->numivals < sp2->numivals) {
    return -1;
  }

  /* next compare other feature subtypes */

  if (sp1->subtype < sp2->subtype) {
    return -1;
  } else if (sp1->subtype > sp2->subtype) {
    return 1;
  }

  /* if identical cds ranges, compare codon_start */

  if (sp1->subtype == FEATDEF_CDS && sp2->subtype == FEATDEF_CDS) {
    sfp1 = sp1->sfp;
    sfp2 = sp2->sfp;
    if (sfp1 != NULL && sfp2 != NULL) {
      crp1 = (CdRegionPtr) sfp1->data.value.ptrvalue;
      crp2 = (CdRegionPtr) sfp2->data.value.ptrvalue;
      if (crp1 != NULL && crp2 != NULL) {
        if (crp1->frame > 1 || crp2->frame > 1) {
          if (crp1->frame < crp2->frame) {
            return -1;
          } else if (crp1->frame < crp2->frame) {
            return 1;
          }
        }
      }
    }
  }

  /* then compare feature label */

  compare = StringCmp (sp1->label, sp2->label);
  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  /* compare parent seq-annot by itemID (was sap pointer value) */

  sap1 = sp1->sap;
  sap2 = sp2->sap;
  if (sap1 != NULL && sap2 != NULL) {
    if (sap1->idx.itemID > sap2->idx.itemID) {
      return 1;
    } else if (sap1->idx.itemID < sap2->idx.itemID) {
      return -1;
    }
  }

  /* last comparison to make it absolutely deterministic */

  if (sp1->itemID > sp2->itemID) {
    return 1;
  } else if (sp1->itemID < sp2->itemID) {
    return -1;
  }

  return 0;
}

static int LIBCALLBACK SortFeatItemListByRev (VoidPtr vp1, VoidPtr vp2)

{
  Int2                compare;
  CdRegionPtr         crp1;
  CdRegionPtr         crp2;
  Int2                i;
  Int2                j;
  Int2                k;
  Int2                numivals;
  SeqAnnotPtr         sap1;
  SeqAnnotPtr         sap2;
  SMFeatItemPtr PNTR  spp1 = vp1;
  SMFeatItemPtr PNTR  spp2 = vp2;
  SMFeatItemPtr       sp1;
  SMFeatItemPtr       sp2;
  SeqFeatPtr          sfp1;
  SeqFeatPtr          sfp2;
  Uint1               subtype1;
  Uint1               subtype2;

  if (spp1 == NULL || spp2 == NULL) return 0;
  sp1 = *((SMFeatItemPtr PNTR) spp1);
  sp2 = *((SMFeatItemPtr PNTR) spp2);
  if (sp1 == NULL || sp2 == NULL) return 0;

  /* feature with largest right extreme is first */

  if (sp1->right < sp2->right) {
    return 1;
  } else if (sp1->right > sp2->right) {
    return -1;

  /* reversing order so that longest feature is first */

  } else if (sp1->left < sp2->left) {
    return -1;
  } else if (sp1->left > sp2->left) {
    return 1;
  }

  /* given identical extremes, put operon features first */

  if (sp1->subtype == FEATDEF_operon && sp2->subtype != FEATDEF_operon) {
    return -1;
  } else if (sp2->subtype == FEATDEF_operon && sp1->subtype != FEATDEF_operon) {
    return 1;
  }

  /* then gene features */

  if (sp1->subtype == FEATDEF_GENE && sp2->subtype != FEATDEF_GENE) {
    return -1;
  } else if (sp2->subtype == FEATDEF_GENE && sp1->subtype != FEATDEF_GENE) {
    return 1;
  }

  /* then rna features */

  subtype1 = FindFeatFromFeatDefType (sp1->subtype);
  subtype2 = FindFeatFromFeatDefType (sp2->subtype);

  if (subtype1 == SEQFEAT_RNA && subtype2 != SEQFEAT_RNA) {
    return -1;
  } else if (subtype2 == SEQFEAT_RNA && subtype1 != SEQFEAT_RNA) {
    return 1;
  }

  /* then cds features */

  if (sp1->subtype == FEATDEF_CDS && sp2->subtype != FEATDEF_CDS) {
    return -1;
  } else if (sp2->subtype == FEATDEF_CDS && sp1->subtype != FEATDEF_CDS) {
    return 1;
  }

  /* next compare internal intervals */

  numivals = MIN (sp1->numivals, sp2->numivals);
  if (numivals > 0 && sp1->ivals != NULL && sp2->ivals != NULL) {
    for (i = 0, j = sp1->numivals * 2, k = sp2->numivals * 2; i < numivals; i++) {

      /* check right interval */

      k--;
      j--;
      if (sp1->ivals [j] < sp2->ivals [k]) {
        return 1;
      } else if (sp1->ivals [j] > sp2->ivals [k]) {
        return -1;
      }

      /* check left interval */

      k--;
      j--;
      if (sp1->ivals [j] < sp2->ivals [k]) {
        return -1;
      } else if (sp1->ivals [j] > sp2->ivals [k]) {
        return 1;
      }
    }
  }

  /* one with fewer intervals goes first */

  if (sp1->numivals > sp2->numivals) {
    return 1;
  } else if (sp1->numivals < sp2->numivals) {
    return -1;
  }

  /* next compare other feature subtypes */

  if (sp1->subtype < sp2->subtype) {
    return -1;
  } else if (sp1->subtype > sp2->subtype) {
    return 1;
  }

  /* if identical cds ranges, compare codon_start */

  if (sp1->subtype == FEATDEF_CDS && sp2->subtype == FEATDEF_CDS) {
    sfp1 = sp1->sfp;
    sfp2 = sp2->sfp;
    if (sfp1 != NULL && sfp2 != NULL) {
      crp1 = (CdRegionPtr) sfp1->data.value.ptrvalue;
      crp2 = (CdRegionPtr) sfp2->data.value.ptrvalue;
      if (crp1 != NULL && crp2 != NULL) {
        if (crp1->frame > 1 || crp2->frame > 1) {
          if (crp1->frame < crp2->frame) {
            return -1;
          } else if (crp1->frame < crp2->frame) {
            return 1;
          }
        }
      }
    }
  }

  /* then compare feature label */

  compare = StringCmp (sp1->label, sp2->label);
  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }

  /* compare parent seq-annot by itemID (was sap pointer value) */

  sap1 = sp1->sap;
  sap2 = sp2->sap;
  if (sap1 != NULL && sap2 != NULL) {
    if (sap1->idx.itemID > sap2->idx.itemID) {
      return 1;
    } else if (sap1->idx.itemID < sap2->idx.itemID) {
      return -1;
    }
  }

  /* last comparison to make it absolutely deterministic */

  if (sp1->itemID > sp2->itemID) {
    return 1;
  } else if (sp1->itemID < sp2->itemID) {
    return -1;
  }

  return 0;
}

/*****************************************************************************
*
*   IndexSegmentedParts callback builds index to speed up mapping
*     of parts to segmented bioseqs
*
*****************************************************************************/

static int LIBCALLBACK SortSeqIdxArray (VoidPtr ptr1, VoidPtr ptr2)

{
  Int2              compare;
  SMSeqIdxPtr PNTR  partp1 = ptr1;
  SMSeqIdxPtr PNTR  partp2 = ptr2;
  SMSeqIdxPtr       part1, part2;

  if (partp1 == NULL || partp2 == NULL) return 0;
  part1 = *((SMSeqIdxPtr PNTR) partp1);
  part2 = *((SMSeqIdxPtr PNTR) partp2);
  if (part1 == NULL || part2 == NULL) return 0;
  compare = StringCmp (part1->seqIdOfPart, part2->seqIdOfPart);
  if (compare > 0) {
    return 1;
  } else if (compare < 0) {
    return -1;
  }
  if (part1->cumOffset > part2->cumOffset) {
    return 1;
  } else if (part1->cumOffset < part2->cumOffset) {
    return -1;
  }
  return 0;
}

static void IndexSegmentedParts (SeqEntryPtr sep, BioseqPtr PNTR lastsegbsp)

{
  BioseqPtr         bsp;
  BioseqExtraPtr    bspextra;
  BioseqSetPtr      bssp;
  Int2              i;
  Int2              numsegs = 0;
  Int4              cumulative = 0;
  ObjMgrDataPtr     omdp;
  SMSeqIdxPtr PNTR  partsByLoc;
  SMSeqIdxPtr PNTR  partsBySeqId;
  SMSeqIdxPtr       segpartptr;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      IndexSegmentedParts (sep, lastsegbsp);
    }
    if (bssp->_class == BioseqseqSet_class_segset && lastsegbsp != NULL) {
      *lastsegbsp = NULL;
    }
    return;
  }

  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;

  /* check for raw part packaged with segmented bioseq */

  if ((bsp->repr == Seq_repr_raw || IsNonGappedLiteral (bsp)) && lastsegbsp != NULL && *lastsegbsp != NULL) {
    omdp = SeqMgrGetOmdpForBioseq (bsp);
    if (omdp == NULL) return;

    bspextra = (BioseqExtraPtr) omdp->extradata;
    if (bspextra == NULL) {
      CreateBioseqExtraBlock (omdp, bsp);
      bspextra = (BioseqExtraPtr) omdp->extradata;
    }
    if (bspextra == NULL) return;

    /* now record segmented parent of raw part if all are packaged together */

    bspextra->parentBioseq = *lastsegbsp;
    return;
  }

  if (bsp->repr != Seq_repr_seg && bsp->repr != Seq_repr_delta) return;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL) return;

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) {
    CreateBioseqExtraBlock (omdp, bsp);
    bspextra = (BioseqExtraPtr) omdp->extradata;
  }
  if (bspextra == NULL) return;

  if (lastsegbsp != NULL && bsp->repr == Seq_repr_seg) {
    *lastsegbsp = bsp;
  }

  for (segpartptr = bspextra->segparthead;
       segpartptr != NULL;
       segpartptr = segpartptr->next) {
    numsegs++;
  }

  bspextra->numsegs = numsegs;
  segpartptr = bspextra->segparthead;
  if (numsegs < 1 || segpartptr == NULL) return;

  partsByLoc = (SMSeqIdxPtr PNTR) MemNew (sizeof (SMSeqIdxPtr) * (numsegs + 1));
  bspextra->partsByLoc = partsByLoc;

  if (partsByLoc != NULL) {
    i = 0;
    while (i < numsegs && segpartptr != NULL) {
      partsByLoc [i] = segpartptr;
      segpartptr = segpartptr->next;
      i++;
    }

    partsBySeqId = (SMSeqIdxPtr PNTR) MemNew (sizeof (SMSeqIdxPtr) * (numsegs + 1));
    bspextra->partsBySeqId = partsBySeqId;

    if (partsBySeqId != NULL) {
      for (i = 0; i < numsegs; i++) {
        partsBySeqId [i] = partsByLoc [i];
      }

      /* sort array by SeqId for binary search */

      HeapSort ((Pointer) partsBySeqId, numsegs, sizeof (SMSeqIdxPtr), SortSeqIdxArray);
    }

  }
}

/*****************************************************************************
*
*   IndexRecordedFeatures callback builds sorted arrays of features and genes
*
*****************************************************************************/

static void IndexRecordedFeatures (SeqEntryPtr sep, Boolean dorevfeats)

{
  BioseqPtr           bsp;
  BioseqExtraPtr      bspextra;
  BioseqSetPtr        bssp;
  SeqFeatPtr          cds;
  SMFeatBlockPtr      curr;
  SeqLocPtr           dnaloc;
  SMFeatItemPtr PNTR  featsByID;
  SMFeatItemPtr PNTR  featsBySfp;
  SMFeatItemPtr PNTR  featsByPos;
  SMFeatItemPtr PNTR  featsByRev;
  SMFeatItemPtr PNTR  featsByLabel;
  SMFeatItemPtr PNTR  genesByLocusTag;
  SMFeatItemPtr PNTR  genesByPos;
  Int4                i;
  Int4                j;
  SMFeatItemPtr       item;
  BioseqPtr           nuc;
  Int4                numfeats;
  Int4                numgenes;
  ObjMgrDataPtr       omdp;
  Int4                pt;
  SeqLocPtr           segloc;
  SeqFeatPtr          sfp;
  SeqLocPtr           slp;
  Int4                stop;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      IndexRecordedFeatures (sep, dorevfeats);
    }
    return;
  }

  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL) return;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return;

  numfeats = bspextra->numfeats;

  curr = bspextra->featlisthead;

  if (numfeats > 0 && curr != NULL) {

    /* build array of pointers into feature items */

    featsByID = (SMFeatItemPtr PNTR) MemNew (sizeof (SMFeatItemPtr) * (numfeats + 1));
    bspextra->featsByID = featsByID;

    if (featsByID != NULL) {
      i = 0;
      j = 0;
      while (i < numfeats && curr != NULL) {
        if (j >= curr->index || j >= bspextra->blocksize) {
          j = 0;
          curr = curr->next;
        }
        if (curr != NULL && j < curr->index && curr->data != NULL) {
          featsByID [i] = &(curr->data [j]);
          i++;
          j++;
        }
      }
      if (i < numfeats) {
        ErrPostEx (SEV_WARNING, 0, 0, "SeqMgr indexing feature table build problem");
      }

      featsBySfp = (SMFeatItemPtr PNTR) MemNew (sizeof (SMFeatItemPtr) * (numfeats + 1));
      bspextra->featsBySfp = featsBySfp;

      if (featsBySfp != NULL) {
        for (i = 0; i < numfeats; i++) {
          featsBySfp [i] = featsByID [i];
        }

        /* sort all features by SeqFeatPtr value */

        HeapSort ((VoidPtr) featsBySfp, (size_t) numfeats, sizeof (SMFeatItemPtr), SortFeatItemListBySfp);
      }

      featsByPos = (SMFeatItemPtr PNTR) MemNew (sizeof (SMFeatItemPtr) * (numfeats + 1));
      bspextra->featsByPos = featsByPos;

      if (featsByPos != NULL) {
        for (i = 0; i < numfeats; i++) {
          featsByPos [i] = featsByID [i];
        }

        /* sort all features by feature location on bioseq */

        HeapSort ((VoidPtr) featsByPos, (size_t) numfeats, sizeof (SMFeatItemPtr), SortFeatItemListByPos);

        for (i = 0; i < numfeats; i++) {
          item = featsByPos [i];
          if (item != NULL) {
            item->index = i;
          }
        }

        /* build arrays of sorted gene, mRNA, CDS, publication, and biosource features for lookup by overlap */

        bspextra->genesByPos = SeqMgrBuildFeatureIndex (bsp, &(bspextra->numgenes), 0, FEATDEF_GENE);
        bspextra->mRNAsByPos = SeqMgrBuildFeatureIndex (bsp, &(bspextra->nummRNAs), 0, FEATDEF_mRNA);
        bspextra->CDSsByPos = SeqMgrBuildFeatureIndex (bsp, &(bspextra->numCDSs), 0, FEATDEF_CDS);
        bspextra->pubsByPos = SeqMgrBuildFeatureIndex (bsp, &(bspextra->numpubs), 0, FEATDEF_PUB);
        bspextra->orgsByPos = SeqMgrBuildFeatureIndex (bsp, &(bspextra->numorgs), 0, FEATDEF_BIOSRC);
        bspextra->operonsByPos = SeqMgrBuildFeatureIndex (bsp, &(bspextra->numoperons), 0, FEATDEF_operon);
      }

      if (dorevfeats) {
        featsByRev = (SMFeatItemPtr PNTR) MemNew (sizeof (SMFeatItemPtr) * (numfeats + 1));
        bspextra->featsByRev = featsByRev;

        if (featsByRev != NULL) {
          for (i = 0; i < numfeats; i++) {
            featsByRev [i] = featsByID [i];
          }

          /* optionally sort all features by feature reverse location on bioseq */

          HeapSort ((VoidPtr) featsByRev, (size_t) numfeats, sizeof (SMFeatItemPtr), SortFeatItemListByRev);
        }
      }

      featsByLabel = (SMFeatItemPtr PNTR) MemNew (sizeof (SMFeatItemPtr) * (numfeats + 1));
      bspextra->featsByLabel = featsByLabel;

      if (featsByLabel != NULL) {
        for (i = 0; i < numfeats; i++) {
          featsByLabel [i] = featsByID [i];
        }

        /* sort all features by label value */

        HeapSort ((VoidPtr) featsByLabel, (size_t) numfeats, sizeof (SMFeatItemPtr), SortFeatItemListByLabel);
      }

      genesByPos = bspextra->genesByPos;
      numgenes = bspextra->numgenes;
      if (genesByPos != NULL && numgenes > 0) {

        genesByLocusTag = (SMFeatItemPtr PNTR) MemNew (sizeof (SMFeatItemPtr) * (numgenes + 1));
        bspextra->genesByLocusTag = genesByLocusTag;

        if (genesByLocusTag != NULL) {
          for (i = 0; i < numgenes; i++) {
            genesByLocusTag [i] = genesByPos [i];
          }

          /* sort by locus_tag value */

          HeapSort ((VoidPtr) genesByLocusTag, (size_t) numgenes, sizeof (SMFeatItemPtr), SortFeatItemListByLocusTag);
        }
      }
    }
  }

  if (numfeats < 1 || (! ISA_aa (bsp->mol))) return;
  cds = SeqMgrGetCDSgivenProduct (bsp, NULL);
  if (cds == NULL) return;
  nuc = BioseqFindFromSeqLoc (cds->location);
  if (nuc == NULL) return;

  for (i = 0; i < numfeats; i++) {
    item = featsByPos [i];
    if (item != NULL) {
      sfp = item->sfp;
      if (sfp != NULL) {

        /* map to dna (on parts if segmented) */

        dnaloc = aaFeatLoc_to_dnaFeatLoc (cds, sfp->location);
        if (dnaloc != NULL) {

          /* map to segmented bioseq coordinates if necessary */

          segloc = SeqLocMergeEx (nuc, dnaloc, NULL, FALSE, TRUE, FALSE, FALSE);

          SeqLocFree (dnaloc);
          if (segloc != NULL) {

            slp = NULL;
            stop = -1;

            /* now find where last point maps on nucleotide for flatfile */

            while ((slp = SeqLocFindNext (segloc, slp)) != NULL) {
              pt = SeqLocStop (slp);
              if (pt != -1) {
                stop = pt;
              }
            }
            item->dnaStop = stop;

            SeqLocFree (segloc);
          }
        }
      }
    }
  }
}

/*****************************************************************************
*
*   IndexFeaturesOnEntity makes feature pointers across all Bioseqs in entity
*
*****************************************************************************/

static void IndexFeaturesOnEntity (SeqEntryPtr sep, SMFeatItemPtr PNTR featsByID, Int4Ptr countP)

{
  BioseqPtr       bsp;
  BioseqExtraPtr  bspextra;
  BioseqSetPtr    bssp;
  Int4            count;
  Int4            i;
  Int4            numfeats;
  ObjMgrDataPtr   omdp;

  if (sep == NULL || featsByID == NULL || countP == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      IndexFeaturesOnEntity (sep, featsByID, countP);
    }
    return;
  }

  if (! IS_Bioseq (sep)) return;
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL) return;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return;

  numfeats = bspextra->numfeats;
  if (bspextra->featsByID != NULL || numfeats > 0) {
    count = *countP;

    for (i = 0; i < numfeats; i++, count++) {
      featsByID [count] = bspextra->featsByID [i];
    }

    *countP = count;
  }
}

/*****************************************************************************
*
*   SortDescItemListByID callback sorts by descriptor itemID
*   SortDescItemListBySdp sorts by descriptor pointer
*   SortDescItemListByIndex sorts by descriptor index
*
*****************************************************************************/

static int LIBCALLBACK SortDescItemListByID (VoidPtr vp1, VoidPtr vp2)

{
  SMDescItemPtr PNTR  spp1 = vp1;
  SMDescItemPtr PNTR  spp2 = vp2;
  SMDescItemPtr       sp1;
  SMDescItemPtr       sp2;

  if (spp1 == NULL || spp2 == NULL) return 0;
  sp1 = *((SMDescItemPtr PNTR) spp1);
  sp2 = *((SMDescItemPtr PNTR) spp2);
  if (sp1 == NULL || sp2 == NULL) return 0;

  /* sort by descriptor itemID */

  if (sp1->itemID > sp2->itemID) {
    return 1;
  } else if (sp1->itemID < sp2->itemID) {
    return -1;
  }

  return 0;
}

static int LIBCALLBACK SortDescItemListBySdp (VoidPtr vp1, VoidPtr vp2)

{
  SMDescItemPtr PNTR  spp1 = vp1;
  SMDescItemPtr PNTR  spp2 = vp2;
  SMDescItemPtr       sp1;
  SMDescItemPtr       sp2;

  if (spp1 == NULL || spp2 == NULL) return 0;
  sp1 = *((SMDescItemPtr PNTR) spp1);
  sp2 = *((SMDescItemPtr PNTR) spp2);
  if (sp1 == NULL || sp2 == NULL) return 0;

  /* sort by SeqDescrPtr value */

  if (sp1->sdp > sp2->sdp) {
    return 1;
  } else if (sp1->sdp < sp2->sdp) {
    return -1;
  }

  return 0;
}

static int LIBCALLBACK SortDescItemListByIndex (VoidPtr vp1, VoidPtr vp2)

{
  SMDescItemPtr PNTR  spp1 = vp1;
  SMDescItemPtr PNTR  spp2 = vp2;
  SMDescItemPtr       sp1;
  SMDescItemPtr       sp2;

  if (spp1 == NULL || spp2 == NULL) return 0;
  sp1 = *((SMDescItemPtr PNTR) spp1);
  sp2 = *((SMDescItemPtr PNTR) spp2);
  if (sp1 == NULL || sp2 == NULL) return 0;

  /* sort by descriptor index */

  if (sp1->index > sp2->index) {
    return 1;
  } else if (sp1->index < sp2->index) {
    return -1;
  }

  return 0;
}

/*****************************************************************************
*
*   RecordDescriptorsInBioseqs callback records list of relevant descriptors
*
*****************************************************************************/

static void RecordDescriptorsInBioseqs (BioseqPtr bsp, Pointer userdata)

{
  BioseqExtraPtr      bspextra;
  SeqMgrDescContext   context;
  ValNodePtr          head = NULL;
  ValNodePtr          last = NULL;
  Int4                numdescs = 0;
  ObjMgrDataPtr       omdp;
  SMDescItemPtr       sdip;
  SeqDescrPtr         sdp;
  ValNodePtr          vnp;

  if (bsp == NULL) return;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL) return;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, 0, &context);
  while (sdp != NULL) {

    numdescs++;
    sdip = (SMDescItemPtr) MemNew (sizeof (SMDescItem));
    if (sdip != NULL) {
      vnp = ValNodeNew (last);
      if (head == NULL) {
        head = vnp;
      }
      last = vnp;
      if (vnp != NULL) {
        vnp->data.ptrvalue = (Pointer) sdip;
      }
      sdip->sdp = sdp;
      sdip->sep = context.sep;
      sdip->itemID = context.itemID;
      sdip->index = context.index;
      sdip->level = context.level;
      sdip->seqdesctype = context.seqdesctype;
    }

    sdp = SeqMgrGetNextDescriptor (bsp, sdp, 0, &context);
  }

  bspextra->desclisthead = head;
  bspextra->numdescs = numdescs;
}

/*****************************************************************************
*
*   RecordDescriptorsOnTopSet callback records list of all descriptors
*
*****************************************************************************/

typedef struct descindex {
  ValNodePtr  deschead;
  ValNodePtr  lastdesc;
  Int4        numdescs;
} DescIndex, PNTR DescIndexPtr;

static void RecordAllDescsCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  DescIndexPtr   dxp;
  ObjValNodePtr  ovp;
  SMDescItemPtr  sdip;
  SeqDescrPtr    sdp = NULL;
  ValNodePtr     vnp;

  if (sep == NULL || mydata == NULL) return;
  dxp = (DescIndexPtr) mydata;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    sdp = bsp->descr;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    sdp = bssp->descr;
  } else return;

  while (sdp != NULL) {
    (dxp->numdescs)++;
    sdip = (SMDescItemPtr) MemNew (sizeof (SMDescItem));
    if (sdip != NULL) {
      vnp = ValNodeNew (dxp->lastdesc);
      if (dxp->deschead == NULL) {
        dxp->deschead = vnp;
      }
      dxp->lastdesc = vnp;
      if (vnp != NULL) {
        vnp->data.ptrvalue = (Pointer) sdip;
      }
      sdip->sdp = sdp;
      sdip->sep = sep;
      if (sdp->extended != 0) {
        ovp = (ObjValNodePtr) sdp;
        sdip->itemID = ovp->idx.itemID;
      }
      sdip->index = 0;
      sdip->level = indent;
      sdip->seqdesctype = sdp->choice;
    }
    sdp = sdp->next;
  }
}

static void RecordDescriptorsOnTopSet (SeqEntryPtr sep)

{
  BioseqExtraPtr  bspextra;
  BioseqSetPtr    bssp;
  DescIndex       dx;
  ObjMgrDataPtr   omdp;

  if (sep == NULL) return;
  if (! IS_Bioseq_set (sep)) return;

  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp == NULL) return;

  omdp = SeqMgrGetOmdpForPointer (bssp);
  if (omdp == NULL) return;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) {
    CreateBioseqExtraBlock (omdp, NULL);
    bspextra = (BioseqExtraPtr) omdp->extradata;
  }
  if (bspextra == NULL) return;

  dx.deschead = NULL;
  dx.lastdesc = NULL;
  dx.numdescs = 0;

  SeqEntryExplore (sep, (Pointer) &dx, RecordAllDescsCallback);

  bspextra->desclisthead = dx.deschead;
  bspextra->numdescs = dx.numdescs;
}

/*****************************************************************************
*
*   IndexRecordedDescriptors callback builds sorted arrays of descriptors
*
*****************************************************************************/

static void IndexRecordedDescriptors (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr           bsp;
  BioseqExtraPtr      bspextra;
  BioseqSetPtr        bssp;
  SMDescItemPtr PNTR  descrsByID;
  SMDescItemPtr PNTR  descrsBySdp;
  SMDescItemPtr PNTR  descrsByIndex;
  ValNodePtr          head;
  Int4                i;
  Int4                numdescs;
  ObjMgrDataPtr       omdp = NULL;
  SMDescItemPtr       sdip;
  ValNodePtr          vnp;

  if (sep == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return;
    omdp = SeqMgrGetOmdpForBioseq (bsp);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    omdp = SeqMgrGetOmdpForPointer (bssp);
  } else return;

  if (omdp == NULL) return;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return;

  head = bspextra->desclisthead;
  numdescs = bspextra->numdescs;

  if (head != NULL && numdescs > 0) {

    /* build array of pointers into descriptor items */

    descrsByID = (SMDescItemPtr PNTR) MemNew (sizeof (SMDescItemPtr) * (numdescs + 1));
    bspextra->descrsByID = descrsByID;

    descrsBySdp = (SMDescItemPtr PNTR) MemNew (sizeof (SMDescItemPtr) * (numdescs + 1));
    bspextra->descrsBySdp = descrsBySdp;

    descrsByIndex = (SMDescItemPtr PNTR) MemNew (sizeof (SMDescItemPtr) * (numdescs + 1));
    bspextra->descrsByIndex = descrsByIndex;

    if (descrsByID != NULL && descrsBySdp != NULL && descrsByIndex != NULL) {
      for (i = 0, vnp = head; i < numdescs && vnp != NULL; i++, vnp = vnp->next) {
        sdip = (SMDescItemPtr) vnp->data.ptrvalue;
        if (sdip != NULL) {
          descrsByID [i] = sdip;
          descrsBySdp [i] = sdip;
          descrsByIndex [i] = sdip;
        }
      }

      /* sort all descriptors by itemID, SeqDescrPtr value, or index */

      HeapSort ((VoidPtr) descrsByID, (size_t) numdescs, sizeof (SMDescItemPtr), SortDescItemListByID);
      HeapSort ((VoidPtr) descrsBySdp, (size_t) numdescs, sizeof (SMDescItemPtr), SortDescItemListBySdp);
      HeapSort ((VoidPtr) descrsByIndex, (size_t) numdescs, sizeof (SMDescItemPtr), SortDescItemListByIndex);
    }
  }
}

/*****************************************************************************
*
*   DoSegmentedProtein needed because SeqIdWithinBioseq may fail for segmented proteins
*
*****************************************************************************/

static void DoSegmentedProtein (BioseqPtr bsp, Pointer userdata)

{
  BioseqExtraPtr     bspextra;
  SeqMgrFeatContext  context;
  ObjMgrDataPtr      omdp;
  BioseqPtr          parent = NULL;
  SeqFeatPtr         sfp;

  if (! ISA_aa (bsp->mol)) return;

   if (bsp->repr != Seq_repr_seg) {
    parent = SeqMgrGetParentOfPart (bsp, NULL);
    if (parent == NULL) return;
  }

 omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL) return;

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return;

  /* if it already has a best protein feature, return */

  if (bspextra->protFeat != NULL) return;

  /* part of parent inherits best protein from parent */

  if (bsp->repr != Seq_repr_seg && parent != NULL) {
    sfp = SeqMgrGetBestProteinFeature (parent, NULL);
    bspextra->protFeat = sfp;
    return;
  }

  /* now check for full-length proteins on segmented parent */

  sfp = SeqMgrGetNextFeatureByLabel (bsp, NULL, SEQFEAT_PROT, 0, &context);
  while (sfp != NULL) {
    if (context.left == 0 && context.right == bsp->length - 1) {
      bspextra->protFeat = sfp;
    }

    sfp = SeqMgrGetNextFeatureByLabel (bsp, sfp, SEQFEAT_PROT, 0, &context);
  }
}

/*****************************************************************************
*
*   SeqMgrReindexBioseqExtraData refreshes internal indices for rapid retrieval
*
*****************************************************************************/

NLM_EXTERN Uint2 LIBCALL SeqMgrIndexFeaturesEx (Uint2 entityID, Pointer ptr, Boolean flip, Boolean dorevfeats)

{
  BioseqExtraPtr      bspextra;
  Int4                count;
  ExtraIndex          exind;
  SMFeatItemPtr PNTR  featsByID;
  BioseqPtr           lastsegbsp = NULL;
  Boolean             objMgrFilter [OBJ_MAX];
  SeqEntryPtr         oldscope;
  ObjMgrDataPtr       omdp;
  SeqEntryPtr         sep;

  if (entityID == 0) {
    entityID = ObjMgrGetEntityIDForPointer (ptr);
  }
  if (entityID == 0) return 0;

  /* reset any existing index data on all bioseqs in entity */

  SeqMgrClearFeatureIndexes (entityID, NULL);

  /* want to scope to bioseqs within the entity, to allow for colliding IDs */

  sep = SeqMgrGetTopSeqEntryForEntity (entityID);

  /* make top SeqEntry if only Bioseq or BioseqSet was read */

  if (sep == NULL) {
    omdp = ObjMgrGetData (entityID);
    if (omdp != NULL) {
      if (omdp->datatype == OBJ_BIOSEQ || omdp->datatype == OBJ_BIOSEQSET) {
        sep = SeqEntryNew ();
        if (sep != NULL) {
          if (omdp->datatype == OBJ_BIOSEQ) {
            sep->choice = 1;
            sep->data.ptrvalue = omdp->dataptr;
            SeqMgrSeqEntry (SM_BIOSEQ, omdp->dataptr, sep);
          } else {
            sep->choice = 2;
            sep->data.ptrvalue = omdp->dataptr;
            SeqMgrSeqEntry (SM_BIOSEQSET, omdp->dataptr, sep);
          }
        }
        sep = GetTopSeqEntryForEntityID (entityID);
      }
    }
  }

  if (sep == NULL) return 0;

  /* clean up many old-style ASN.1 problems without changing structure */

  BasicSeqEntryCleanup (sep);

  /* set gather/objmgr fields now present in several objects */

  AssignIDsInEntity (entityID, 0, NULL);

  /* set scope for FindAppropriateBioseq, FindFirstLocalBioseq */

  oldscope = SeqEntrySetScope (sep);

  /* gather all segmented locations */

  exind.lastbsp = NULL;
  exind.lastsap = NULL;
  exind.lastbssp = NULL;
  exind.alignhead = NULL;
  exind.lastalign = NULL;
  exind.segpartail = NULL;
  exind.bspcount = 0;
  exind.aligncount = 0;
  exind.descrcount = 0;
  exind.featcount = 0;
  exind.seqlitid = 0;
  exind.flip = flip;

  MemSet ((Pointer) objMgrFilter, 0, sizeof (objMgrFilter));
  objMgrFilter [OBJ_BIOSEQ] = TRUE;
  /* objMgrFilter [OBJ_BIOSEQSET] = TRUE not needed */
  objMgrFilter [OBJ_BIOSEQ_SEG] = TRUE;
  objMgrFilter [OBJ_BIOSEQ_DELTA] = TRUE;
  GatherObjectsInEntity (entityID, 0, NULL, RecordSegmentsInBioseqs, (Pointer) &exind, objMgrFilter);

  /* build indexes to speed mapping of parts to segmented bioseq */

  lastsegbsp = NULL;

  IndexSegmentedParts (sep, &lastsegbsp);

  /* now gather to get descriptor itemID counts on each bioseq or bioseq set,
     and record features on the bioseq indicated by the feature location */

  exind.lastbsp = NULL;
  exind.lastsap = NULL;
  exind.lastbssp = NULL;
  exind.alignhead = NULL;
  exind.lastalign = NULL;
  exind.segpartail = NULL;
  exind.bspcount = 0;
  exind.aligncount = 0;
  exind.descrcount = 0;
  exind.featcount = 0;
  exind.seqlitid = 0;
  exind.flip = flip;

  MemSet ((Pointer) objMgrFilter, 0, sizeof (objMgrFilter));
  objMgrFilter [OBJ_BIOSEQ] = TRUE;
  objMgrFilter [OBJ_BIOSEQSET] = TRUE;
  objMgrFilter [OBJ_SEQANNOT] = TRUE;
  objMgrFilter [OBJ_SEQFEAT] = TRUE;
  objMgrFilter [OBJ_SEQALIGN] = TRUE;
  GatherObjectsInEntity (entityID, 0, NULL, RecordFeaturesInBioseqs, (Pointer) &exind, objMgrFilter);

  /* finish building array of sorted features on each indexed bioseq */

  IndexRecordedFeatures (sep, dorevfeats);

  /* set best protein feature for segmented protein bioseqs and their parts */

  VisitBioseqsInSep (sep, NULL, DoSegmentedProtein);

  /* resetset scope used to limit FindAppropriateBioseq, FindFirstLocalBioseq */

  SeqEntrySetScope (oldscope);

  /* stamp top of entity with time of indexing */

  omdp = ObjMgrGetData (entityID);
  if (omdp != NULL) {
    omdp->indexed = GetSecs ();

    /* alignment ID to SeqAlignPtr index always goes on top of entity */

    SeqMgrIndexAlignments (entityID);

    /* master index of feature if top of entity is not a Bioseq */

    if (omdp->datatype != OBJ_BIOSEQ) {

      CreateBioseqExtraBlock (omdp, NULL);
      bspextra = (BioseqExtraPtr) omdp->extradata;
      if (bspextra != NULL) {

        /* make master index of features by itemID at top of entity */

        if (exind.featcount > 0) {
          featsByID = (SMFeatItemPtr PNTR) MemNew (sizeof (SMFeatItemPtr) * (exind.featcount + 1));
          if (featsByID != NULL) {
            count = 0;
            IndexFeaturesOnEntity (sep, featsByID, &count);

            /* sort all features on entity-wide list by itemID */

            HeapSort ((VoidPtr) featsByID, (size_t) count, sizeof (SMFeatItemPtr), SortFeatItemListByID);

            bspextra->featsByID = featsByID;
            bspextra->numfeats = count;
          }
        }
      }
    }
  }

  /* finish indexing list of descriptors on each indexed bioseq */

  VisitBioseqsInSep (sep, NULL, RecordDescriptorsInBioseqs);

  if (IS_Bioseq_set (sep)) {
    RecordDescriptorsOnTopSet (sep);
  }

  SeqEntryExplore (sep, NULL, IndexRecordedDescriptors);

  /* free chain of SeqAlignPtr now that index is built */

  ValNodeFree (exind.alignhead);

  return entityID;
}

NLM_EXTERN Uint2 LIBCALL SeqMgrIndexFeatures (Uint2 entityID, Pointer ptr)

{
  return SeqMgrIndexFeaturesEx (entityID, ptr, FALSE, FALSE);
}

/*****************************************************************************
*
*   SeqMgrIsBioseqIndexed checks for presence of time of indexing stamp
*
*****************************************************************************/

NLM_EXTERN time_t LIBCALL SeqMgrFeaturesAreIndexed (Uint2 entityID)

{
  ObjMgrDataPtr  omdp;

  if (entityID == 0) return 0;
  omdp = ObjMgrGetData (entityID);
  if (omdp == NULL) return 0;
  return omdp->indexed;
}

/*****************************************************************************
*
*   SeqMgrGetBestProteinFeature and SeqMgrGetCDSgivenProduct take a protein
*     bioseq to get the best protein feature or encoding CDS
*   SeqMgrGetRNAgivenProduct takes an mRNA (cDNA) bioseq and gets encoding mRNA
*     feature on the genomic bioseq
*
*****************************************************************************/

NLM_EXTERN ProtRefPtr LIBCALL SeqMgrGetProtXref (SeqFeatPtr sfp)

{
  ProtRefPtr      prp = NULL;
  SeqFeatXrefPtr  xref;

  if (sfp == NULL) return NULL;
  xref = sfp->xref;
  while (xref != NULL && xref->data.choice != SEQFEAT_PROT) {
    xref = xref->next;
  }
  if (xref != NULL) {
    prp = (ProtRefPtr) xref->data.value.ptrvalue;
  }
  return prp;
}

static void SetContextForFeature (SeqFeatPtr sfp, SeqMgrFeatContext PNTR context, ObjMgrDataPtr omdp)

{
  SMFeatItemPtr  best;
  SeqFeatPtr     bst;

  if (sfp == NULL || context == NULL || omdp == NULL) return;
  best = SeqMgrFindSMFeatItemPtr (sfp);
  if (best == NULL) return;
  bst = best->sfp;
  context->entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);
  context->itemID = best->itemID;
  context->sfp = bst;
  context->sap = best->sap;
  context->bsp = best->bsp;
  context->label = best->label;
  context->left = best->left;
  context->right = best->right;
  context->dnaStop = best->dnaStop;
  context->partialL = best->partialL;
  context->partialR = best->partialR;
  context->farloc = best->farloc;
  context->strand = best->strand;
  if (bst != NULL) {
    context->seqfeattype = bst->data.choice;
  } else {
    context->seqfeattype = FindFeatFromFeatDefType (best->subtype);
  }
  context->featdeftype = best->subtype;
  context->numivals = best->numivals;
  context->ivals = best->ivals;
  context->userdata = NULL;
  context->omdp = (Pointer) omdp;
  context->index = best->index + 1;
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetBestProteinFeature (BioseqPtr bsp,
                                                           SeqMgrFeatContext PNTR context)

{
  BioseqExtraPtr  bspextra;
  ObjMgrDataPtr   omdp;

  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrFeatContext));
  }
  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;
  SetContextForFeature (bspextra->protFeat, context, omdp);
  return bspextra->protFeat;
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetCDSgivenProduct (BioseqPtr bsp,
                                                        SeqMgrFeatContext PNTR context)

{
  BioseqExtraPtr  bspextra;
  ObjMgrDataPtr   omdp;
  SeqFeatPtr      sfp;

  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrFeatContext));
  }
  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;
  sfp = bspextra->cdsOrRnaFeat;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return NULL;
  SetContextForFeature (sfp, context, omdp);
  return sfp;
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetRNAgivenProduct (BioseqPtr bsp,
                                                        SeqMgrFeatContext PNTR context)

{
  BioseqExtraPtr  bspextra;
  ObjMgrDataPtr   omdp;
  SeqFeatPtr      sfp;

  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrFeatContext));
  }
  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;
  sfp = bspextra->cdsOrRnaFeat;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_RNA) return NULL;
  SetContextForFeature (sfp, context, omdp);
  return sfp;
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetPROTgivenProduct (BioseqPtr bsp,
                                                         SeqMgrFeatContext PNTR context)

{
  BioseqExtraPtr  bspextra;
  ObjMgrDataPtr   omdp;
  SeqFeatPtr      sfp;

  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrFeatContext));
  }
  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;
  sfp = bspextra->cdsOrRnaFeat;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_PROT) return NULL;
  SetContextForFeature (sfp, context, omdp);
  return sfp;
}

NLM_EXTERN ValNodePtr LIBCALL SeqMgrGetSfpProductList (BioseqPtr bsp)

{
  BioseqExtraPtr  bspextra;
  ObjMgrDataPtr   omdp;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;
  return bspextra->prodlisthead;
}

/*****************************************************************************
*
*   SeqMgrGetGeneXref, SeqMgrGeneIsSuppressed, SeqMgrGetProtXref,
*     SeqMgrGetOverlappingGene, and SeqMgrGetOverlappingPub
*
*****************************************************************************/

static Boolean HasNoText (CharPtr str)

{
  Char  ch;

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

NLM_EXTERN GeneRefPtr LIBCALL SeqMgrGetGeneXref (SeqFeatPtr sfp)

{
  GeneRefPtr      grp = NULL;
  SeqFeatXrefPtr  xref;

  if (sfp == NULL) return NULL;
  xref = sfp->xref;
  while (xref != NULL && xref->data.choice != SEQFEAT_GENE) {
    xref = xref->next;
  }
  if (xref != NULL) {
    grp = (GeneRefPtr) xref->data.value.ptrvalue;
  }
  return grp;
}

NLM_EXTERN Boolean LIBCALL SeqMgrGeneIsSuppressed (GeneRefPtr grp)

{
  if (grp == NULL) return FALSE;
  if (grp != NULL && HasNoText (grp->locus) && HasNoText (grp->allele) &&
      HasNoText (grp->desc) && HasNoText (grp->maploc) &&
      HasNoText (grp->locus_tag) && grp->db == NULL &&
      grp->syn == NULL) return TRUE;
  return FALSE;
}

static Boolean CheckInternalExonBoundaries (Int2 numivalsA, Int4Ptr ivalsA, Int2 numivalsB, Int4Ptr ivalsB)

{
  Int2  i;
  Int2  j;

  if (numivalsA > numivalsB) return FALSE;
  if (ivalsA == NULL || ivalsB == NULL) return TRUE;

  /* scan first exon-intron boundary against candidate start positions */

  for (i = 0; i <= numivalsB - numivalsA; i++) {
    if (ivalsA [1] == ivalsB [2 * i + 1]) break;
  }
  if (i > numivalsB - numivalsA) return FALSE;

  /* scan subsequent exon-intron and intron-exon boundaries */

  for (j = 2; j <= 2 * numivalsA - 2; j++) {
    if (ivalsA [j] != ivalsB [2 * i + j]) return FALSE;
  }

  return TRUE;
}

static Boolean StrandsMatch (Uint1 featstrand, Uint1 locstrand)

{
  if (featstrand == locstrand) return TRUE;
  if (locstrand == Seq_strand_unknown && featstrand != Seq_strand_minus) return TRUE;
  if (featstrand == Seq_strand_unknown && locstrand != Seq_strand_minus) return TRUE;
  if (featstrand == Seq_strand_both && locstrand != Seq_strand_minus) return TRUE;
  if (locstrand == Seq_strand_both) return TRUE;
  return FALSE;
}

static Int4 TestForOverlap (SMFeatItemPtr feat, SeqLocPtr slp,
                            Int4 left, Int4 right, Int2 overlapType,
                            Int2 numivals, Int4Ptr ivals)

{
  SeqLocPtr   a, b;
  Int4        diff;
  SeqFeatPtr  sfp;

  if (overlapType == SIMPLE_OVERLAP) {

    /* location must merely be overlapped by gene, etc., or either one inside the other */

    if (feat->right >= left && feat->left <= right) {
      diff = ABS (left - feat->left) + ABS (feat->right - right);
      return diff;
    }

    /*
    if ((feat->left <= left && feat->right > left) ||
        (feat->left < right && feat->right >= right)) {
      diff = ABS (left - feat->left) + ABS (feat->right - right);
      return diff;
    }
    */

  } else if (overlapType == CONTAINED_WITHIN) {

    /* requires location to be completely contained within gene, etc. */

    if (feat->left <= left && feat->right >= right) {
      diff = (left - feat->left) + (feat->right - right);
      return diff;
    }

  } else if (overlapType == LOCATION_SUBSET || overlapType == CHECK_INTERVALS) {

    /* requires individual intervals to be completely contained within gene, etc. */

    if (feat->left <= left && feat->right >= right) {
      sfp = feat->sfp;
      if (sfp != NULL) {
        diff = SeqLocAinB (slp, sfp->location);
        if (diff >= 0) {
          if (overlapType == LOCATION_SUBSET || numivals == 1 ||
              CheckInternalExonBoundaries (numivals, ivals, feat->numivals, feat->ivals)) {
            return diff;
          }
        }
      }
    }

  } else if (overlapType == INTERVAL_OVERLAP || overlapType == COMMON_INTERVAL) {

    /* requires overlap between at least one pair of intervals (INTERVAL_OVERLAP) */
    /* or one complete shared interval (COMMON_INTERVAL) */

    if (feat->right >= left && feat->left <= right) {
      sfp = feat->sfp;
      if (sfp != NULL) {
        a = SeqLocFindNext (slp, NULL);
        while (a != NULL) {
          b = SeqLocFindNext (sfp->location, NULL);
          while (b != NULL) {
            if ((overlapType == INTERVAL_OVERLAP
                && SeqLocCompare (a, b) != SLC_NO_MATCH) 
              || (overlapType == COMMON_INTERVAL
                && SeqLocCompare (a, b) == SLC_A_EQ_B))
            {
              diff = ABS (left - feat->left) + ABS (feat->right - right);
              return diff;
            }
            b = SeqLocFindNext (sfp->location, b);
          }
          a = SeqLocFindNext (slp, a);
        }
      }
    }
  }

  return -1;
}

static void SeqMgrBestOverlapSetContext (
  SMFeatItemPtr best,
  ObjMgrDataPtr omdp,
  Pointer userdata,
  SeqMgrFeatContext PNTR context
)

{
  SeqFeatPtr  bst;

  if (best != NULL && omdp != NULL && context != NULL) {
    bst = best->sfp;
    context->entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);
    context->itemID = best->itemID;
    context->sfp = best->sfp;
    context->sap = best->sap;
    context->bsp = best->bsp;
    context->label = best->label;
    context->left = best->left;
    context->right = best->right;
    context->dnaStop = best->dnaStop;
    context->partialL = best->partialL;
    context->partialR = best->partialR;
    context->farloc = best->farloc;
    context->strand = best->strand;
    if (bst != NULL) {
      context->seqfeattype = bst->data.choice;
    } else {
      context->seqfeattype = FindFeatFromFeatDefType (best->subtype);
    }
    context->featdeftype = best->subtype;
    context->numivals = best->numivals;
    context->ivals = best->ivals;
    context->userdata = userdata;
    context->omdp = (Pointer) omdp;
    context->index = best->index + 1;
  }
}

static SeqFeatPtr SeqMgrGetBestOverlappingFeat (SeqLocPtr slp, Uint2 subtype,
                                                SMFeatItemPtr PNTR array,
                                                Int4 num, Int4Ptr pos,
                                                Int2 overlapType,
                                                SeqMgrFeatContext PNTR context,
                                                Int2Ptr count,
                                                Pointer userdata,
                                                SeqMgrFeatExploreProc userfunc)

{
  SMFeatItemPtr   best = NULL;
  BioseqPtr       bsp;
  BioseqExtraPtr  bspextra;
  Int4            diff;
  Uint2           entityID;
  SMFeatItemPtr   feat;
  Int4            from;
  Boolean         goOn = TRUE;
  Int4            hier = -1;
  Int2            i;
  Uint4           index = 0;
  Int4Ptr         ivals = NULL;
  Int4            L;
  Int4            left;
  SeqLocPtr       loc;
  Int4            max = INT4_MAX;
  Int4            mid;
  Int2            numivals = 0;
  SeqEntryPtr     oldscope;
  ObjMgrDataPtr   omdp;
  SMFeatItemPtr   prev;
  Int4            R;
  Int4            right;
  SeqEntryPtr     sep;
  Uint1           strand;
  Int4            swap;
  SeqLocPtr       tmp;
  Int4            to;

  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrFeatContext));
  }
  if (pos != NULL) {
    *pos = 0;
  }
  if (count != NULL) {
    *count = 0;
  }
  if (slp == NULL) return NULL;

  bsp = FindAppropriateBioseq (slp, NULL);
  if (bsp == NULL) {
    bsp = FindFirstLocalBioseq (slp);
  }
  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;

  switch (subtype) {
    case FEATDEF_GENE :
      array = bspextra->genesByPos;
      num = bspextra->numgenes;
      break;
    case FEATDEF_CDS :
      array = bspextra->CDSsByPos;
      num = bspextra->numCDSs;
      break;
    case FEATDEF_mRNA :
      array = bspextra->mRNAsByPos;
      num = bspextra->nummRNAs;
      break;
    case FEATDEF_PUB :
      array = bspextra->pubsByPos;
      num = bspextra->numpubs;
      break;
    case FEATDEF_BIOSRC :
      array = bspextra->orgsByPos;
      num = bspextra->numorgs;
      break;
  	case FEATDEF_operon :
      array = bspextra->operonsByPos;
      num = bspextra->numoperons;
    default :
      break;
  }

  if (array == NULL || num < 1) return NULL;

  entityID = ObjMgrGetEntityIDForPointer (bsp);
  sep = SeqMgrGetTopSeqEntryForEntity (entityID);
  oldscope = SeqEntrySetScope (sep);

  left = GetOffsetInNearBioseq (slp, bsp, SEQLOC_LEFT_END);
  right = GetOffsetInNearBioseq (slp, bsp, SEQLOC_RIGHT_END);

  SeqEntrySetScope (oldscope);

  if (left == -1 || right == -1) return NULL;

  /* if feature spans origin, normalize with left < 0 */

  if (left > right && bsp->topology == TOPOLOGY_CIRCULAR) {
    left -= bsp->length;
  }

  /* binary search to leftmost candidate within the xxxByPos array */

  L = 0;
  R = num - 1;
  while (L < R) {
    mid = (L + R) / 2;
    feat = array [mid];
    if (feat != NULL && feat->right < left) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }

  feat = array [R];

  if (feat != NULL && feat->left > left && R > 0) {

    /* if hit is already past location, location was in between local hits */

    prev = array [R - 1];
    if (prev != NULL && prev->overlap != -1) {

      /* backup R by one to check appropriate overlap hierarchy */

      R--;
      feat = array [R];
    }
  }

  if (feat != NULL) {
    hier = feat->overlap;
  }

  loc = SeqLocMergeEx (bsp, slp, NULL, FALSE, /* TRUE */ FALSE, FALSE, FALSE);
  strand = SeqLocStrand (loc);
  if (overlapType == CHECK_INTERVALS) {
    tmp = NULL;
    while ((tmp = SeqLocFindNext (loc, tmp)) != NULL) {
      numivals++;
    }
    if (numivals > 0) {
      ivals = MemNew (sizeof (Int4) * (numivals * 2));
      if (ivals != NULL) {
        tmp = NULL;
        i = 0;
        while ((tmp = SeqLocFindNext (loc, tmp)) != NULL) {
          from = SeqLocStart (tmp);
          to = SeqLocStop (tmp);
          if (strand == Seq_strand_minus) {
            swap = from;
            from = to;
            to = swap;
          }
          ivals [i] = from;
          i++;
          ivals [i] = to;
          i++;
        }
      }
    }
  }
  SeqLocFree (loc);

  /* linear scan to smallest covering gene, publication, biosource, etc. */

  while (R < num && feat != NULL && feat->left <= right) {

    /* requires feature to be contained within gene, etc. */

    diff = TestForOverlap (feat, slp, left, right, overlapType, numivals, ivals);
    if (diff >= 0) {

      if (StrandsMatch (feat->strand, strand)) {

        if (userfunc != NULL && context != NULL && goOn) {
          SeqMgrBestOverlapSetContext (feat, omdp, userdata, context);
          if (! userfunc (feat->sfp, context)) {
            goOn = FALSE;
          }
          if (count != NULL) {
            (*count)++;
          }
        }

        /* diff = (left - feat->left) + (feat->right - right); */
        if (diff < max) {
          best = feat;
          index = R;
          max = diff;
        }
      }
    }
    R++;
    feat = array [R];
  }

  /* also will go up gene overlap hierarchy pointers from original R hit */

  while (hier != -1) {

    feat = array [hier];
    if (feat != NULL) {

      diff = TestForOverlap (feat, slp, left, right, overlapType, numivals, ivals);
      if (diff >= 0) {

        if (StrandsMatch (feat->strand, strand)) {

          if (userfunc != NULL && context != NULL && goOn) {
            SeqMgrBestOverlapSetContext (feat, omdp, userdata, context);
            if (! userfunc (feat->sfp, context)) {
              goOn = FALSE;
            }
            if (count != NULL) {
              (*count)++;
            }
          }

          /* diff = (left - feat->left) + (feat->right - right); */
          if (diff < max) {
            best = feat;
            index = hier;
            max = diff;
          }
        }
      }
      hier = feat->overlap;
    } else {
      hier = -1;
    }
  }

  if (ivals != NULL) {
    ivals = MemFree (ivals);
  }

  if (best != NULL) {
    if (pos != NULL) {
      *pos = index + 1;
    }
    if (context != NULL) {
      SeqMgrBestOverlapSetContext (best, omdp, userdata, context);
    }
    return best->sfp;
  }

  return NULL;
}

NLM_EXTERN Int4 TestFeatOverlap (SeqFeatPtr sfpA, SeqFeatPtr sfpB, Int2 overlapType)

{
  Int4           diff;
  SMFeatItemPtr  sfipA, sfipB;

  if (sfpA == NULL || sfpB == NULL) return -1;
  sfipA = SeqMgrFindSMFeatItemPtr (sfpA);
  sfipB = SeqMgrFindSMFeatItemPtr (sfpB);
  if (sfipA == NULL || sfipB == NULL) return -1;

  diff = TestForOverlap (sfipB, sfpA->location, sfipA->left, sfipA->right,
                         overlapType, sfipA->numivals, sfipA->ivals);
  if (diff < 0) return -1;

  if (StrandsMatch (sfipB->strand, sfipA->strand)) {
    return diff;
  }

  return -1;
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingGene (SeqLocPtr slp, SeqMgrFeatContext PNTR context)

{
  return SeqMgrGetBestOverlappingFeat (slp, FEATDEF_GENE, NULL, 0, NULL, CONTAINED_WITHIN, context, NULL, NULL, NULL);
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingmRNA (SeqLocPtr slp, SeqMgrFeatContext PNTR context)

{
  return SeqMgrGetBestOverlappingFeat (slp, FEATDEF_mRNA, NULL, 0, NULL, CONTAINED_WITHIN, context, NULL, NULL, NULL);
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingCDS (SeqLocPtr slp, SeqMgrFeatContext PNTR context)

{
  return SeqMgrGetBestOverlappingFeat (slp, FEATDEF_CDS, NULL, 0, NULL, CONTAINED_WITHIN, context, NULL, NULL, NULL);
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingPub (SeqLocPtr slp, SeqMgrFeatContext PNTR context)

{
  return SeqMgrGetBestOverlappingFeat (slp, FEATDEF_PUB, NULL, 0, NULL, CONTAINED_WITHIN, context, NULL, NULL, NULL);
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingSource (SeqLocPtr slp, SeqMgrFeatContext PNTR context)

{
  return SeqMgrGetBestOverlappingFeat (slp, FEATDEF_BIOSRC, NULL, 0, NULL, CONTAINED_WITHIN, context, NULL, NULL, NULL);
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingOperon (SeqLocPtr slp, SeqMgrFeatContext PNTR context)

{
  return SeqMgrGetBestOverlappingFeat (slp, FEATDEF_operon, NULL, 0, NULL, CONTAINED_WITHIN, context, NULL, NULL, NULL);
}

/*****************************************************************************
*
*   SeqMgrGetFeatureByLabel returns the a feature with the desired label
*   If desired, place a SeqMgrFeatContext data structure on the stack, and pass
*     in &context as the last parameter
*
*****************************************************************************/

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetFeatureByLabel (BioseqPtr bsp, CharPtr label,
                                                       Uint1 seqFeatChoice, Uint1 featDefChoice,
                                                       SeqMgrFeatContext PNTR context)

{
  SMFeatItemPtr PNTR  array;
  BioseqExtraPtr      bspextra;
  Uint2               entityID;
  SMFeatItemPtr       feat;
  Int4                L;
  Int4                mid;
  Int4                num;
  ObjMgrDataPtr       omdp;
  Int4                R;
  SeqFeatPtr          sfp;
  Uint1               seqfeattype;

  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrFeatContext));
  }

  if (bsp == NULL || StringHasNoText (label)) return NULL;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;
  array = bspextra->featsByLabel;
  num = bspextra->numfeats;
  if (array == NULL || num < 1) return NULL;

  entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);

  /* binary search to leftmost candidate within the featsByLabel array */

  L = 0;
  R = num - 1;
  while (L < R) {
    mid = (L + R) / 2;
    feat = array [mid];
    if (feat != NULL && StringICmp (feat->label, label) < 0) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }

  feat = array [R];

  /* linear scan to find desired label on desired feature type */

  while (R < num && feat != NULL && StringICmp (feat->label, label) == 0) {
    sfp = feat->sfp;
    if (sfp != NULL) {
      seqfeattype = sfp->data.choice;
      if ((seqFeatChoice == 0 || seqfeattype == seqFeatChoice) &&
          (featDefChoice == 0 || feat->subtype == featDefChoice) &&
          (! feat->ignore)) {
        if (context != NULL) {
          context->entityID = entityID;
          context->itemID = feat->itemID;
          context->sfp = sfp;
          context->sap = feat->sap;
          context->bsp = feat->bsp;
          context->label = feat->label;
          context->left = feat->left;
          context->right = feat->right;
          context->dnaStop = feat->dnaStop;
          context->partialL = feat->partialL;
          context->partialR = feat->partialR;
          context->farloc = feat->farloc;
          context->strand = feat->strand;
          context->seqfeattype = seqfeattype;
          context->featdeftype = feat->subtype;
          context->numivals = feat->numivals;
          context->ivals = feat->ivals;
          context->userdata = NULL;
          context->omdp = (Pointer) omdp;
          context->index = R + 1;
        }
        return sfp;
      }
    }

    R++;
    feat = array [R];
  }

  return NULL;
}

/*****************************************************************************
*
*   SeqMgrBuildFeatureIndex builds a sorted array index for any feature type
*     (including gene, mRNA, CDS, publication, and biosource built-in arrays)
*   SeqMgrGetOverlappingFeature uses the array, or a feature subtype (chocies
*     are FEATDEF_GENE, FEATDEF_CDS, FEATDEF_mRNA, FEATDEF_PUB, or FEATDEF_BIOSRC)
*     to find feature overlap, requiring either that the location be completely
*     contained within the feature intervals, contained within the feature extreme
*     range, or merely that it be overlapped by the feature, and returns the position
*     in the index
*   SeqMgrGetFeatureInIndex gets an arbitrary feature indexed by the array
*
*****************************************************************************/

NLM_EXTERN VoidPtr LIBCALL SeqMgrBuildFeatureIndex (BioseqPtr bsp, Int4Ptr num,
                                                    Uint1 seqFeatChoice, Uint1 featDefChoice)

{
  SMFeatItemPtr PNTR  array;
  BioseqExtraPtr      bspextra;
  SMFeatItemPtr PNTR  featsByPos;
  Int4                i;
  Int4                j;
  Int4                k;
  SMFeatItemPtr       item;
  Int4                numfeats;
  Int4                numitems;
  SMFeatItemPtr       nxtitem;
  ObjMgrDataPtr       omdp;
  Boolean             overlaps;
  Uint1               seqfeattype;

  if (num != NULL) {
    *num = 0;
  }
  if (bsp == NULL) return NULL;
  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;

  featsByPos = bspextra->featsByPos;
  numfeats = bspextra->numfeats;
  if (featsByPos == NULL || numfeats < 1) return NULL;

  for (i = 0, numitems = 0; i < numfeats; i++) {
    item = featsByPos [i];
    seqfeattype = FindFeatFromFeatDefType (item->subtype);
    if ((seqFeatChoice == 0 || seqfeattype == seqFeatChoice) &&
        (featDefChoice == 0 || item->subtype == featDefChoice)) {
      numitems++;
    }
  }
  if (numitems < 1) return NULL;

  array = (SMFeatItemPtr PNTR) MemNew (sizeof (SMFeatItemPtr) * (numitems + 1));
  if (array == NULL) return NULL;

  i = 0;
  j = 0;
  while (i < numfeats && j < numitems) {
    item = featsByPos [i];
    seqfeattype = FindFeatFromFeatDefType (item->subtype);
    if ((seqFeatChoice == 0 || seqfeattype == seqFeatChoice) &&
        (featDefChoice == 0 || item->subtype == featDefChoice)) {
      array [j] = item;
      j++;
    }
    i++;
  }

  if (num != NULL) {
    *num = numitems;
  }

  /* linear searches to mark overlapping features, now allows full length genes */

  for (j = 0; j < numitems - 1; j++) {
    item = array [j];
    for (k = j + 1, overlaps = TRUE; k < numitems && overlaps; k++) {
      nxtitem = array [k];
      if ((item->left <= nxtitem->left && item->right > nxtitem->left) ||
          (item->left < nxtitem->right && item->right >= nxtitem->right)) {

       /* after binary search, also go up the hierarchy chain to avoid traps */

        nxtitem->overlap = j;
      } else {
        overlaps = FALSE;
      }
    }
  }

  return (VoidPtr) array;
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingFeature (SeqLocPtr slp, Uint2 subtype,
                                                           VoidPtr featarray, Int4 numfeats,
                                                           Int4Ptr position, Int2 overlapType,
                                                           SeqMgrFeatContext PNTR context)

{
  return SeqMgrGetBestOverlappingFeat (slp, subtype, (SMFeatItemPtr PNTR) featarray,
                                       numfeats, position, overlapType, context, NULL, NULL, NULL);
}

NLM_EXTERN Int2 LIBCALL SeqMgrGetAllOverlappingFeatures (SeqLocPtr slp, Uint2 subtype,
                                                         VoidPtr featarray,
                                                         Int4 numfeats,
                                                         Int2 overlapType,
                                                         Pointer userdata,
                                                         SeqMgrFeatExploreProc userfunc)

{
  SeqMgrFeatContext  context;
  Int2               count;

  SeqMgrGetBestOverlappingFeat (slp, subtype, (SMFeatItemPtr PNTR) featarray,
                                numfeats, NULL, overlapType, &context, &count,
                                userdata, userfunc);

  return count;
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetFeatureInIndex (BioseqPtr bsp, VoidPtr featarray,
                                                       Int4 numfeats, Uint4 index,
                                                       SeqMgrFeatContext PNTR context)

{
  SMFeatItemPtr PNTR  array;
  SeqFeatPtr          curr;
  Uint2               entityID;
  SMFeatItemPtr       item = NULL;
  ObjMgrDataPtr       omdp;

  if (context != NULL) {
    MemSet ((Pointer) context, 0, sizeof (SeqMgrFeatContext));
  }
  if (bsp == NULL || featarray == NULL || numfeats < 1) return NULL;
  if (index < 1 || index > numfeats) return NULL;
  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;

  array = (SMFeatItemPtr PNTR) featarray;
  item = array [index - 1];
  if (item == NULL) return NULL;

  entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);

  curr = item->sfp;
  if (curr != NULL && context != NULL && (! item->ignore)) {
    context->entityID = entityID;
    context->itemID = item->itemID;
    context->sfp = curr;
    context->sap = item->sap;
    context->bsp = item->bsp;
    context->label = item->label;
    context->left = item->left;
    context->right = item->right;
    context->dnaStop = item->dnaStop;
    context->partialL = item->partialL;
    context->partialR = item->partialR;
    context->farloc = item->farloc;
    context->strand = item->strand;
    if (curr != NULL) {
      context->seqfeattype = curr->data.choice;
    } else {
      context->seqfeattype = FindFeatFromFeatDefType (item->subtype);
    }
    context->featdeftype = item->subtype;
    context->numivals = item->numivals;
    context->ivals = item->ivals;
    context->userdata = NULL;
    context->omdp = (Pointer) omdp;
    context->index = item->index + 1;
  }
  return curr;
}

/*****************************************************************************
*
*   SeqMgrGetNextDescriptor and SeqMgrGetNextFeature
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL SeqMgrGetNextDescriptor (BioseqPtr bsp, ValNodePtr curr,
                                                       Uint1 seqDescChoice,
                                                       SeqMgrDescContext PNTR context)

{
  BioseqSetPtr   bssp;
  Uint2          entityID;
  ObjMgrDataPtr  omdp;
  SeqEntryPtr    sep;
  ValNode        vn;

  if (context == NULL) return NULL;

  /* if curr is NULL, initialize context fields (in user's stack) */

  if (curr == NULL) {
    if (bsp == NULL) return NULL;
    omdp = SeqMgrGetOmdpForBioseq (bsp);
    if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;

    context->omdp = (Pointer) omdp;
    context->itemID = omdp->lastDescrItemID;
    context->index = 0;
    context->level = 0;

    /* start curr just before beginning of bioseq descriptor list */

    curr = &vn;
    vn.choice = 0;
    vn.data.ptrvalue = 0;
    vn.next = bsp->descr;
  }

  omdp = (ObjMgrDataPtr) context->omdp;
  if (omdp == NULL) return NULL;

  entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);
  sep = ObjMgrGetChoiceForData (omdp->dataptr);

  /* now look for next appropriate descriptor after curr in current chain */

  while (curr != NULL) {
    curr = curr->next;
    if (curr != NULL) {
      (context->itemID)++;
      (context->index)++;
      if (seqDescChoice == 0 || curr->choice == seqDescChoice) {
        context->entityID = entityID;
        context->sdp = curr;
        context->sep = sep;
        context->seqdesctype = curr->choice;
        context->userdata = NULL;
        context->omdp = (Pointer) omdp;
        return curr;
      }
    }
  }

  /* now go up omdp chain looking for next descriptor */

  while (curr == NULL) {
    omdp = SeqMgrGetOmdpForPointer (omdp->parentptr);
    if (omdp == NULL) return NULL;

    /* update current omdp in context */

    context->omdp = (Pointer) omdp;
    context->itemID = omdp->lastDescrItemID;

    switch (omdp->datatype) {
      case OBJ_BIOSEQ :
        bsp = (BioseqPtr) omdp->dataptr;
        curr = bsp->descr;
        break;
      case OBJ_BIOSEQSET :
        bssp = (BioseqSetPtr) omdp->dataptr;
        curr = bssp->descr;
        break;
      default :
        break;
    }

    sep = ObjMgrGetChoiceForData (omdp->dataptr);
    
    (context->level)++;

    /* now look for first appropriate descriptor in current chain */

    while (curr != NULL) {
      (context->itemID)++;
      (context->index)++;
      if (seqDescChoice == 0 || curr->choice == seqDescChoice) {
        context->entityID = entityID;
        context->sdp = curr;
        context->sep = sep;
        context->seqdesctype = curr->choice;
        context->userdata = NULL;
        context->omdp = (Pointer) omdp;
        return curr;
      }
      curr = curr->next;
    }
  }

  return curr;
}

static SeqFeatPtr LIBCALL SeqMgrGetNextFeatureEx (BioseqPtr bsp, SeqFeatPtr curr,
                                                  Uint1 seqFeatChoice, Uint1 featDefChoice,
                                                  SeqMgrFeatContext PNTR context,
                                                  Boolean byLabel, Boolean byLocusTag)

{
  SMFeatItemPtr PNTR  array = NULL;
  BioseqExtraPtr      bspextra;
  Uint2               entityID;
  Uint4               i;
  SMFeatItemPtr       item;
  Int4                num = 0;
  ObjMgrDataPtr       omdp;
  Uint1               seqfeattype;

  if (context == NULL) return NULL;

  /* if curr is NULL, initialize context fields (in user's stack) */


  if (curr == NULL) {
    if (bsp == NULL) return NULL;
    omdp = SeqMgrGetOmdpForBioseq (bsp);
    if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;

    context->omdp = (Pointer) omdp;
    context->index = 0;
  }

  omdp = (ObjMgrDataPtr) context->omdp;
  if (omdp == NULL) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;
  if (byLocusTag) {
    array = bspextra->genesByLocusTag;
    num = bspextra->numgenes;
  } else if (byLabel) {
    array = bspextra->featsByLabel;
    num = bspextra->numfeats;
  } else {
    array = bspextra->featsByPos;
    num = bspextra->numfeats;
  }
  if (array == NULL || num < 1) return NULL;

  entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);

  i = context->index;

  /* now look for next appropriate feature */

  while (i < num) {
    item = array [i];
    if (item != NULL) {
      curr = item->sfp;
      i++;
      if (curr != NULL) {
        seqfeattype = curr->data.choice;
        if ((seqFeatChoice == 0 || seqfeattype == seqFeatChoice) &&
            (featDefChoice == 0 || item->subtype == featDefChoice) &&
            (! item->ignore)) {
          context->entityID = entityID;
          context->itemID = item->itemID;
          context->sfp = curr;
          context->sap = item->sap;
          context->bsp = item->bsp;
          context->label = item->label;
          context->left = item->left;
          context->right = item->right;
          context->dnaStop = item->dnaStop;
          context->partialL = item->partialL;
          context->partialR = item->partialR;
          context->farloc = item->farloc;
          context->strand = item->strand;
          context->seqfeattype = seqfeattype;
          context->featdeftype = item->subtype;
          context->numivals = item->numivals;
          context->ivals = item->ivals;
          context->userdata = NULL;
          context->omdp = (Pointer) omdp;
          if (byLocusTag) {
            context->index = i;
          } else if (byLabel) {
            context->index = i;
          } else {
            context->index = item->index + 1;
          }
          return curr;
        }
      }
    }
  }

  return NULL;
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetNextFeature (BioseqPtr bsp, SeqFeatPtr curr,
                                                    Uint1 seqFeatChoice, Uint1 featDefChoice,
                                                    SeqMgrFeatContext PNTR context)

{
  return SeqMgrGetNextFeatureEx (bsp, curr, seqFeatChoice, featDefChoice, context, FALSE, FALSE);
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetNextFeatureByLabel (BioseqPtr bsp, SeqFeatPtr curr,
                                                           Uint1 seqFeatChoice, Uint1 featDefChoice,
                                                           SeqMgrFeatContext PNTR context)

{
  return SeqMgrGetNextFeatureEx (bsp, curr, seqFeatChoice, featDefChoice, context, TRUE, FALSE);
}

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetNextGeneByLocusTag (BioseqPtr bsp, SeqFeatPtr curr,
                                                           SeqMgrFeatContext PNTR context
)

{
  return SeqMgrGetNextFeatureEx (bsp, curr, SEQFEAT_GENE, 0, context, FALSE, TRUE);
}

/*****************************************************************************
*
*   SeqMgrExploreBioseqs, SeqMgrExploreSegments, SeqMgrExploreDescriptors,
*     SeqMgrExploreFeatures, SeqMgrVisitDescriptors, and SeqMgrVisitFeatures
*
*****************************************************************************/

static Boolean JustExamineBioseqs (SeqEntryPtr sep, BioseqSetPtr bssp,
                                   SeqMgrBioseqContextPtr context,
                                   SeqMgrBioseqExploreProc userfunc,
                                   Boolean nucs, Boolean prots, Boolean parts,
                                   Int4Ptr count)

{
  BioseqPtr       bsp;
  BioseqExtraPtr  bspextra;
  ObjMgrDataPtr   omdp;

  if (sep == NULL || context == NULL || userfunc == NULL) return FALSE;

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    if (bsp == NULL) return TRUE;

    /* check for desired molecule type */

    if (ISA_na (bsp->mol) && (! nucs)) return TRUE;
    if (ISA_aa (bsp->mol) && (! prots)) return TRUE;

    omdp = SeqMgrGetOmdpForBioseq (bsp);
    if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return TRUE;
    bspextra = (BioseqExtraPtr) omdp->extradata;
    if (bspextra == NULL) return TRUE;

    context->itemID = bspextra->bspItemID;
    context->bsp = bsp;
    context->sep = sep;
    context->bssp = bssp;
    context->numsegs = bspextra->numsegs;
    context->omdp = omdp;
    (context->index)++;

    if (count != NULL) {
      (*count)++;
    }
    /* continue until user function returns FALSE, then exit all recursions */

    if (! userfunc (bsp, context)) return FALSE;
    return TRUE;
  }

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return TRUE;

    /* check to see if parts should be explored */

    if (bssp->_class == BioseqseqSet_class_parts) {
      if (! parts) return TRUE;

      /* within the parts set we want to see individual component bioseqs */

      nucs = TRUE;
      prots = TRUE;
    }

    /* recursively explore bioseq set until user function returns FALSE */

    for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
      if (! JustExamineBioseqs (sep, bssp, context, userfunc, nucs, prots, parts, count)) return FALSE;
    }
  }

  return TRUE;
}

NLM_EXTERN Int4 LIBCALL SeqMgrExploreBioseqs (Uint2 entityID, Pointer ptr, Pointer userdata,
                                              SeqMgrBioseqExploreProc userfunc,
                                              Boolean nucs, Boolean prots, Boolean parts)

{
  SeqMgrBioseqContext  context;
  Int4                 count = 0;
  SeqEntryPtr          sep;

  if (entityID == 0) {
    entityID = ObjMgrGetEntityIDForPointer (ptr);
  }
  if (entityID == 0) return 0;
  sep = SeqMgrGetTopSeqEntryForEntity (entityID);
  if (sep == NULL) return 0;
  if (userfunc == NULL) return 0;

  context.entityID = entityID;
  context.index = 0;
  context.userdata = userdata;

  /* recursive call to explore SeqEntry and pass appropriate bioseqs to user */

  JustExamineBioseqs (sep, NULL, &context, userfunc, nucs, prots, parts, &count);

  return count;
}

NLM_EXTERN Int4 LIBCALL SeqMgrExploreSegments (BioseqPtr bsp, Pointer userdata,
                                               SeqMgrSegmentExploreProc userfunc)

{
  BioseqExtraPtr        bspextra;
  SeqMgrSegmentContext  context;
  Int4                  count = 0;
  Uint2                 entityID;
  Uint4                 i;
  ObjMgrDataPtr         omdp;
  SMSeqIdxPtr PNTR      partsByLoc;
  SMSeqIdxPtr           segpartptr;
  SeqLocPtr             slp;

  if (bsp == NULL) return 0;
  if (bsp->repr != Seq_repr_seg && bsp->repr != Seq_repr_delta) return 0;
  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return 0;
  if (userfunc == NULL) return 0;
  entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return 0;
  partsByLoc = bspextra->partsByLoc;
  if (partsByLoc == NULL || bspextra->numsegs < 1) return 0;

  for (i = 0; i < bspextra->numsegs; i++) {
    segpartptr = partsByLoc [i];
    if (segpartptr != NULL) {
      slp = segpartptr->slp;
      context.entityID = entityID;
      context.itemID = segpartptr->itemID;
      context.slp = slp;
      context.parent = segpartptr->parentBioseq;
      context.cumOffset = segpartptr->cumOffset;
      context.from = segpartptr->from;
      context.to = segpartptr->to;
      context.strand = segpartptr->strand;
      context.userdata = userdata;
      context.omdp = (Pointer) omdp;
      context.index = i + 1;

      count++;

      if (! userfunc (slp, &context)) return count;
    }
  }

  return count;
}

NLM_EXTERN Int4 LIBCALL SeqMgrExploreDescriptors (BioseqPtr bsp, Pointer userdata,
                                                  SeqMgrDescExploreProc userfunc,
                                                  BoolPtr seqDescFilter)

{
  BioseqSetPtr       bssp;
  SeqMgrDescContext  context;
  Int4               count = 0;
  Uint2              entityID;
  Uint4              itemID;
  ObjMgrDataPtr      omdp;
  ValNodePtr         sdp;
  SeqEntryPtr        sep;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return 0;
  if (userfunc == NULL) return 0;
  entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);

  context.index = 0;
  context.level = 0;
  while (omdp != NULL) {
    itemID = omdp->lastDescrItemID;
    sdp = NULL;
    switch (omdp->datatype) {
      case OBJ_BIOSEQ :
        bsp = (BioseqPtr) omdp->dataptr;
        sdp = bsp->descr;
        break;
      case OBJ_BIOSEQSET :
        bssp = (BioseqSetPtr) omdp->dataptr;
        sdp = bssp->descr;
        break;
      default :
        break;
    }

    sep = ObjMgrGetChoiceForData (omdp->dataptr);

    /* call for every appropriate descriptor in current chain */

    while (sdp != NULL) {
      itemID++;
      if (seqDescFilter == NULL || seqDescFilter [sdp->choice]) {
        context.entityID = entityID;
        context.itemID = itemID;
        context.sdp = sdp;
        context.sep = sep;
        context.seqdesctype = sdp->choice;
        context.userdata = userdata;
        context.omdp = (Pointer) omdp;
        (context.index)++;

        count++;

        if (! userfunc (sdp, &context)) return count;
      }
      sdp = sdp->next;
    }

    /* now go up omdp chain looking for next descriptor */

    omdp = SeqMgrGetOmdpForPointer (omdp->parentptr);
    (context.level)++;
  }
  return count;
}

static Int4 LIBCALL SeqMgrExploreFeaturesInt (BioseqPtr bsp, Pointer userdata,
                                              SeqMgrFeatExploreProc userfunc,
                                              SeqLocPtr locationFilter,
                                              BoolPtr seqFeatFilter,
                                              BoolPtr featDefFilter,
                                              Boolean doreverse)

{
  BioseqExtraPtr      bspextra;
  SeqMgrFeatContext   context;
  Int4                count = 0;
  Uint2               entityID;
  SMFeatItemPtr PNTR  featsByID;
  SMFeatItemPtr PNTR  featsByPos;
  SMFeatItemPtr PNTR  featsByRev;
  Uint4               i;
  SMFeatItemPtr       item;
  Int4                left = INT4_MIN;
  ObjMgrDataPtr       omdp;
  Int4                right = INT4_MAX;
  Uint1               seqfeattype;
  SeqFeatPtr          sfp;
  Uint4               start = 0;
  Int4                tmp;

  omdp = SeqMgrGetOmdpForBioseq (bsp);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return 0;
  if (userfunc == NULL) return 0;
  entityID = ObjMgrGetEntityIDForPointer (omdp->dataptr);

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return 0;

  if (doreverse) {
    if (bspextra->featsByRev == NULL) {

      /* index by reverse position if not already done */

      featsByRev = (SMFeatItemPtr PNTR) MemNew (sizeof (SMFeatItemPtr) * (bspextra->numfeats + 1));
      bspextra->featsByRev = featsByRev;

      if (featsByRev != NULL) {
        featsByID = bspextra->featsByID;
        for (i = 0; i < bspextra->numfeats; i++) {
          featsByRev [i] = featsByID [i];
        }

        /* sort all features by feature reverse location on bioseq */

        HeapSort ((VoidPtr) featsByRev, (size_t) bspextra->numfeats, sizeof (SMFeatItemPtr), SortFeatItemListByRev);
      }
    }

    featsByPos = bspextra->featsByRev;
  } else {
    featsByPos = bspextra->featsByPos;
  }
  if (featsByPos == NULL || bspextra->numfeats < 1) return 0;

  if (locationFilter != NULL) {
    left = GetOffsetInBioseq (locationFilter, bsp, SEQLOC_LEFT_END);
    if (left == -1) left = INT4_MIN;
    right = GetOffsetInBioseq (locationFilter, bsp, SEQLOC_RIGHT_END);
    if (right == -1) right = INT4_MAX;

    /* if far segmented or delta, and location (from explore
       segments) is minus strand, will need to swap */

    if (left > right) {
      tmp = left;
      left = right;
      right = tmp;
    }

    /*
    binary search to leftmost candidate would need featsByPos array
    variant sorted primarily by rightmost position, so comment this
    out for now, resurrect and add new array only if it turns out to
    be necessary when we support entrez fetch subrecord by location
    */

    /*
    L = 0;
    R = bspextra->numfeats - 1;
    while (L < R) {
      mid = (L + R) / 2;
      item = featsByPos [mid];
      if (item != NULL && item->right < left) {
        L = mid + 1;
      } else {
        R = mid;
      }
    }

    start = R;
    */
  }

  /* call for every appropriate feature in sorted list */

  for (i = start; i < bspextra->numfeats; i++) {
    item = featsByPos [i];
    if (item != NULL) {

      /* can exit once past rightmost limit */

      if (locationFilter != NULL && (! doreverse) && item->left > right) return count;
      if (locationFilter != NULL && (doreverse) && item->right < left) return count;

      sfp = item->sfp;
      if (sfp != NULL) {
        seqfeattype = sfp->data.choice;
      } else {
        seqfeattype = FindFeatFromFeatDefType (item->subtype);
      }
      if ((seqFeatFilter == NULL || seqFeatFilter [seqfeattype]) &&
          (featDefFilter == NULL || featDefFilter [item->subtype]) &&
          (locationFilter == NULL || (item->right >= left && item->left <= right)) &&
          (! item->ignore)) {
        context.entityID = entityID;
        context.itemID = item->itemID;
        context.sfp = sfp;
        context.sap = item->sap;
        context.bsp = item->bsp;
        context.label = item->label;
        context.left = item->left;
        context.right = item->right;
        context.dnaStop = item->dnaStop;
        context.partialL = item->partialL;
        context.partialR = item->partialR;
        context.farloc = item->farloc;
        context.strand = item->strand;
        context.seqfeattype = seqfeattype;
        context.featdeftype = item->subtype;
        context.numivals = item->numivals;
        context.ivals = item->ivals;
        context.userdata = userdata;
        context.omdp = (Pointer) omdp;
        context.index = item->index + 1;

        count++;

        if (! userfunc (sfp, &context)) return count;
      }
    }
  }
  return count;
}

NLM_EXTERN Int4 LIBCALL SeqMgrExploreFeatures (BioseqPtr bsp, Pointer userdata,
                                               SeqMgrFeatExploreProc userfunc,
                                               SeqLocPtr locationFilter,
                                               BoolPtr seqFeatFilter,
                                               BoolPtr featDefFilter)

{
  return SeqMgrExploreFeaturesInt (bsp, userdata, userfunc, locationFilter, seqFeatFilter, featDefFilter, FALSE);
}

NLM_EXTERN Int4 LIBCALL SeqMgrExploreFeaturesRev (BioseqPtr bsp, Pointer userdata,
                                                  SeqMgrFeatExploreProc userfunc,
                                                  SeqLocPtr locationFilter,
                                                  BoolPtr seqFeatFilter,
                                                  BoolPtr featDefFilter)

{
  return SeqMgrExploreFeaturesInt (bsp, userdata, userfunc, locationFilter, seqFeatFilter, featDefFilter, TRUE);
}

static Int2 VisitDescriptorsPerSeqEntry (Uint2 entityID, SeqEntryPtr sep,
                                         Pointer userdata, SeqMgrDescExploreProc userfunc,
                                         BoolPtr seqDescFilter)

{
  BioseqPtr          bsp;
  BioseqSetPtr       bssp = NULL;
  Uint2              count = 0;
  SeqMgrDescContext  context;
  Uint2              itemID;
  ObjMgrDataPtr      omdp = NULL;
  ValNodePtr         sdp = NULL;
  SeqEntryPtr        tmp;

  if (sep != NULL) {
    if (IS_Bioseq (sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (bsp == NULL) return 0;
      omdp = SeqMgrGetOmdpForBioseq (bsp);
      sdp = bsp->descr;
    } else if (IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp == NULL) return 0;
      omdp = SeqMgrGetOmdpForPointer (bssp);
      sdp = bssp->descr;
    }
  }
  if (omdp == NULL) return 0;
  itemID = omdp->lastDescrItemID;

  context.index = 0;
  context.level = 0;

  while (sdp != NULL) {
    itemID++;
    if (seqDescFilter == NULL || seqDescFilter [sdp->choice]) {
      context.entityID = entityID;
      context.itemID = itemID;
      context.sdp = sdp;
      context.sep = sep;
      context.seqdesctype = sdp->choice;
      context.userdata = userdata;
      context.omdp = (Pointer) omdp;

      count++;

      if (! userfunc (sdp, &context)) return count;
    }
    sdp = sdp->next;
  }

  if (bssp != NULL) {
    for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
      count += VisitDescriptorsPerSeqEntry (entityID, tmp, userdata, userfunc, seqDescFilter);
    }
  }

  return count;
}

NLM_EXTERN Int2 LIBCALL SeqMgrVisitDescriptors (Uint2 entityID, Pointer userdata,
                                                SeqMgrDescExploreProc userfunc,
                                                BoolPtr seqDescFilter)

{
  SeqEntryPtr  sep;

  if (entityID < 1 || userfunc == NULL) return 0;
  sep = SeqMgrGetTopSeqEntryForEntity (entityID);
  if (sep == NULL) return 0;

  return VisitDescriptorsPerSeqEntry (entityID, sep, userdata, userfunc, seqDescFilter);
}

NLM_EXTERN Int2 LIBCALL SeqMgrVisitFeatures (Uint2 entityID, Pointer userdata,
                                             SeqMgrFeatExploreProc userfunc,
                                             BoolPtr seqFeatFilter, BoolPtr featDefFilter)

{
  BioseqExtraPtr      bspextra;
  SeqMgrFeatContext   context;
  Int2                count = 0;
  SMFeatItemPtr PNTR  featsByID;
  Uint2               i;
  SMFeatItemPtr       item;
  ObjMgrDataPtr       omdp;
  Uint1               seqfeattype;
  SeqFeatPtr          sfp;

  omdp = ObjMgrGetData (entityID);
  if (omdp == NULL) return 0;
  if (userfunc == NULL) return 0;

  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return 0;
  featsByID = bspextra->featsByID;
  if (featsByID == NULL || bspextra->numfeats < 1) return 0;

  /* call for every appropriate feature in itemID order */

  for (i = 0; i < bspextra->numfeats; i++) {
    item = featsByID [i];
    if (item != NULL) {

      sfp = item->sfp;
      if (sfp != NULL) {
        seqfeattype = sfp->data.choice;
      } else {
        seqfeattype = FindFeatFromFeatDefType (item->subtype);
      }
      if ((seqFeatFilter == NULL || seqFeatFilter [seqfeattype]) &&
          (featDefFilter == NULL || featDefFilter [item->subtype]) &&
          (! item->ignore)) {
        context.entityID = entityID;
        context.itemID = item->itemID;
        context.sfp = sfp;
        context.sap = item->sap;
        context.bsp = item->bsp;
        context.label = item->label;
        context.left = item->left;
        context.right = item->right;
        context.dnaStop = item->dnaStop;
        context.partialL = item->partialL;
        context.partialR = item->partialR;
        context.farloc = item->farloc;
        context.strand = item->strand;
        context.seqfeattype = seqfeattype;
        context.featdeftype = item->subtype;
        context.numivals = item->numivals;
        context.ivals = item->ivals;
        context.userdata = userdata;
        context.omdp = (Pointer) omdp;
        context.index = 0;

        count++;

        if (! userfunc (sfp, &context)) return count;
      }
    }
  }
  return count;
}

/*****************************************************************************
*
*   SeqMgrMapPartToSegmentedBioseq can speed up sequtil's CheckPointInBioseq
*     for indexed part bioseq to segmented bioseq mapping
*
*****************************************************************************/

static SMSeqIdxPtr BinarySearchPartToSegmentMap (BioseqPtr in, Int4 pos, BioseqPtr bsp, SeqIdPtr sip, Boolean relaxed, Int4 from, Int4 to)

{
  BioseqExtraPtr    bspextra;
  Char              buf [80];
  Int2              compare;
  ObjMgrDataPtr     omdp;
  SMSeqIdxPtr PNTR  partsBySeqId;
  SMSeqIdxPtr       segpartptr;
  CharPtr           seqIdOfPart;
  Int2              L, R, mid;

  if (in == NULL) return NULL;
  omdp = SeqMgrGetOmdpForBioseq (in);
  if (omdp == NULL) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;

  partsBySeqId = bspextra->partsBySeqId;
  if (partsBySeqId == NULL || bspextra->numsegs < 1) return NULL;

  if (bsp != NULL) {
    sip = bsp->id;
  }
  if (sip == NULL) return NULL;

  /* binary search into array on segmented bioseq sorted by part seqID (reversed) string */

  while (sip != NULL) {
    if (MakeReversedSeqIdString (sip, buf, sizeof (buf) - 1)) {
      L = 0;
      R = bspextra->numsegs - 1;
      while (L < R) {
        mid = (L + R) / 2;
        segpartptr = partsBySeqId [mid];
        compare = StringCmp (segpartptr->seqIdOfPart, buf);
        if (compare < 0) {
          L = mid + 1;
        } else {
          R = mid;
        }
      }

      /* loop through all components with same seqID, get appropriate segment */

      segpartptr = partsBySeqId [R];
      seqIdOfPart = segpartptr->seqIdOfPart;
      while (R < bspextra->numsegs && StringCmp (seqIdOfPart, buf) == 0) {
        if (relaxed) {

          /* for genome mapping of portion not included in contig */

          if ((from >= segpartptr->from && from <= segpartptr->to) ||
              (to >= segpartptr->from && to <= segpartptr->to) ||
              (from < segpartptr->from && to > segpartptr->to) ||
              (to < segpartptr->from && from > segpartptr->to)) {

            return segpartptr;
          }

        } else if (pos >= segpartptr->from && pos <= segpartptr->to) {

          /* otherwise only map portion included in contig */

          return segpartptr;
        }

        R++;
        if (R < bspextra->numsegs) {
          segpartptr = partsBySeqId [R];
          seqIdOfPart = segpartptr->seqIdOfPart;
        } else {
          seqIdOfPart = NULL;
        }
      }
    }
    sip = sip->next;
  }

  return NULL;
}

NLM_EXTERN SMSeqIdxPtr GenomePartToSegmentMap (BioseqPtr in, BioseqPtr bsp, Int4 from, Int4 to)

{
  return BinarySearchPartToSegmentMap (in, 0, bsp, NULL, TRUE, from, to);
}

NLM_EXTERN Int4 LIBCALL SeqMgrMapPartToSegmentedBioseq (BioseqPtr in, Int4 pos, BioseqPtr bsp, SeqIdPtr sip)

{
  BioseqExtraPtr  bspextra;
  SMSeqIdxPtr     currp;
  SMSeqIdxPtr     nextp;
  ObjMgrDataPtr   omdp;
  SMSeqIdxPtr     segpartptr;

  if (in == NULL) return -1;

  /* first check to see if part has been loaded and single map up block installed */

  if (bsp != NULL) {
    omdp = SeqMgrGetOmdpForBioseq (bsp);
    if (omdp != NULL) {
      bspextra = (BioseqExtraPtr) omdp->extradata;
      if (bspextra != NULL) {

        /* no need for partsByLoc or partsBySeqId arrays, just use segparthead linked list */

        for (segpartptr = bspextra->segparthead; segpartptr != NULL; segpartptr = segpartptr->next) {
          if (segpartptr->parentBioseq == in) {
            if (pos >= segpartptr->from && pos <= segpartptr->to) {

              /* success, immediate return with mapped up value */

              if (segpartptr->strand == Seq_strand_minus) {
                return segpartptr->cumOffset + (segpartptr->to - pos);
              } else {
                return segpartptr->cumOffset + (pos - segpartptr->from);
              }
            }
          }
        }
      }
    }
  }

  /* otherwise do binary search on segmented bioseq mapping data */

  segpartptr = BinarySearchPartToSegmentMap (in, pos, bsp, sip, FALSE, 0, 0);
  if (segpartptr == NULL) return -1;

  if (pos >= segpartptr->from && pos <= segpartptr->to) {

    /* install map up block on part, if it has been loaded, to speed up next search */

    if (bsp != NULL) {
      omdp = SeqMgrGetOmdpForBioseq (bsp);
      if (omdp != NULL) {
        bspextra = (BioseqExtraPtr) omdp->extradata;
        if (bspextra == NULL) {
          CreateBioseqExtraBlock (omdp, bsp);
          bspextra = (BioseqExtraPtr) omdp->extradata;
        }
        if (bspextra != NULL) {

          /* clean up any old map up info on part */

          for (currp = bspextra->segparthead; currp != NULL; currp = nextp) {
            nextp = currp->next;
            SeqLocFree (currp->slp);
            MemFree (currp->seqIdOfPart);
            MemFree (currp);
          }
          bspextra->segparthead = NULL;
          bspextra->numsegs = 0;
          bspextra->partsByLoc = MemFree (bspextra->partsByLoc);
          bspextra->partsBySeqId = MemFree (bspextra->partsBySeqId);

          /* allocate single map up block */

          currp = MemNew (sizeof (SMSeqIdx));
          if (currp != NULL) {
            currp->slp = AsnIoMemCopy (segpartptr->slp,
                                       (AsnReadFunc) SeqLocAsnRead,
                                       (AsnWriteFunc) SeqLocAsnWrite);
            currp->seqIdOfPart = StringSave (segpartptr->seqIdOfPart);
            currp->parentBioseq = segpartptr->parentBioseq;
            currp->cumOffset = segpartptr->cumOffset;
            currp->from = segpartptr->from;
            currp->to = segpartptr->to;
            currp->strand = segpartptr->strand;
          }

          /* add new map up block to part */

          bspextra->segparthead = currp;
        }
      }
    }

    /* now return offset result */

    if (segpartptr->strand == Seq_strand_minus) {
      return segpartptr->cumOffset + (segpartptr->to - pos);
    } else {
      return segpartptr->cumOffset + (pos - segpartptr->from);
    }
  }
  return -1;
}

/*****************************************************************************
*
*   TrimLocInSegment takes a location on an indexed far segmented part and trims
*     trims it to the region referred to by the parent segmented or delta bioseq.
*
*     Only implemented for seqloc_int components, not seqloc_point
*
*****************************************************************************/

NLM_EXTERN SeqLocPtr TrimLocInSegment (
  BioseqPtr master,
  SeqLocPtr location,
  BoolPtr p5ptr,
  BoolPtr p3ptr
)

{
  BioseqPtr         bsp;
  BioseqExtraPtr    bspextra;
  Char              buf [80];
  Int2              compare;
  ObjMgrDataPtr     omdp;
  Boolean           partial5;
  Boolean           partial3;
  SMSeqIdxPtr PNTR  partsBySeqId;
  SeqLocPtr         rsult = NULL;
  SMSeqIdxPtr       segpartptr;
  CharPtr           seqIdOfPart;
  SeqIdPtr          sip;
  SeqIntPtr         sint;
  SeqLocPtr         slp;
  Uint1             strand;
  Int2              L, R, mid;
  Int4              start, stop, swap;

  if (master == NULL || location == NULL) return NULL;

  omdp = SeqMgrGetOmdpForBioseq (master);
  if (omdp == NULL || omdp->datatype != OBJ_BIOSEQ) return NULL;
  bspextra = (BioseqExtraPtr) omdp->extradata;
  if (bspextra == NULL) return NULL;

  partsBySeqId = bspextra->partsBySeqId;
  if (partsBySeqId == NULL || bspextra->numsegs < 1) return NULL;

  partial5 = FALSE;
  partial3 = FALSE;

  if (p5ptr != NULL) {
    partial5 = *p5ptr;
  }
  if (p3ptr != NULL) {
    partial3 = *p3ptr;
  }

  for (slp = SeqLocFindNext (location, NULL);
       slp != NULL;
       slp = SeqLocFindNext (location, slp)) {
    if (slp->choice != SEQLOC_INT) continue;
    sint = (SeqIntPtr) slp->data.ptrvalue;
    if (sint == NULL) continue;
    strand = sint->strand;

    bsp = BioseqFind (sint->id);
    if (bsp == NULL) continue;

    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (! MakeReversedSeqIdString (sip, buf, sizeof (buf) - 1)) continue;

      L = 0;
      R = bspextra->numsegs - 1;
      while (L < R) {
        mid = (L + R) / 2;
        segpartptr = partsBySeqId [mid];
        compare = StringCmp (segpartptr->seqIdOfPart, buf);
        if (compare < 0) {
          L = mid + 1;
        } else {
          R = mid;
        }
      }

      segpartptr = partsBySeqId [R];
      seqIdOfPart = segpartptr->seqIdOfPart;

      while (R < bspextra->numsegs && StringCmp (seqIdOfPart, buf) == 0) {

        start = sint->from;
        stop = sint->to;

        if ((sint->from <= segpartptr->from && sint->to > segpartptr->from) ||
            (sint->from < segpartptr->to && sint->to >= segpartptr->to)) {

          if (sint->from < segpartptr->from) {
            start = segpartptr->from;
            if (strand == Seq_strand_minus || strand == Seq_strand_both_rev) {
              partial3 = TRUE;
            } else {
              partial5 = TRUE;
            }
          }
          if (sint->to > segpartptr->to) {
            stop = segpartptr->to;
            if (strand == Seq_strand_minus || strand == Seq_strand_both_rev) {
              partial5 = TRUE;
            } else {
              partial3 = TRUE;
            }
          }

          if (strand == Seq_strand_minus || strand == Seq_strand_both_rev) {
            swap = start;
            start = stop;
            stop = swap;
          }

          rsult = AddIntervalToLocation (rsult, sint->id, start, stop, FALSE, FALSE);
        }

        R++;
        if (R < bspextra->numsegs) {
          segpartptr = partsBySeqId [R];
          seqIdOfPart = segpartptr->seqIdOfPart;
        } else {
          seqIdOfPart = NULL;
        }
      }
    }
  }

  if (p5ptr != NULL) {
    *p5ptr = partial5;
  }
  if (p3ptr != NULL) {
    *p3ptr = partial3;
  }

  return rsult;
}

static void LockAllBioseqs (BioseqPtr bsp, Pointer userdata);

static void LockAllSegments (SeqLocPtr slp, ValNodePtr PNTR vnpp)

{
  BioseqPtr   bsp;
  SeqLocPtr   loc;
  SeqIdPtr    sip;
  ValNodePtr  vnp;

  if (slp == NULL || vnpp == NULL) return;

  sip = SeqLocId (slp);
  if (sip == NULL) {
    loc = SeqLocFindNext (slp, NULL);
    if (loc != NULL) {
      sip = SeqLocId (loc);
    }
  }
  if (sip == NULL) return;

  /* if packaged in record, no need to fetch it */

  if (BioseqFindCore (sip) != NULL) return;

  /* check against currently locked records */

  for (vnp = *vnpp; vnp != NULL; vnp = vnp->next) {
    bsp = (BioseqPtr) vnp->data.ptrvalue;
    if (bsp == NULL) continue;
    if (SeqIdIn (sip, bsp->id)) return;
  }

  bsp = BioseqLockById (sip);
  ValNodeAddPointer (vnpp, 0, (Pointer) bsp);

  /* now recurse if component is also far delta or seg */

  if (bsp != NULL) {
    if (bsp->repr == Seq_repr_seg || bsp->repr == Seq_repr_delta) {
      LockAllBioseqs (bsp, (Pointer) vnpp);
    }
  }
}

static void LockAllBioseqs (BioseqPtr bsp, Pointer userdata)

{
  DeltaSeqPtr      dsp;
  SeqLocPtr        slp = NULL;
  ValNode          vn;
  ValNodePtr PNTR  vnpp;

  if (bsp == NULL) return;
  vnpp = (ValNodePtr PNTR) userdata;
  if (vnpp == NULL) return;

  if (bsp->repr == Seq_repr_seg) {
    vn.choice = SEQLOC_MIX;
    vn.extended = 0;
    vn.data.ptrvalue = bsp->seq_ext;
    vn.next = NULL;
    while ((slp = SeqLocFindNext (&vn, slp)) != NULL) {
      if (slp != NULL && slp->choice != SEQLOC_NULL) {
        LockAllSegments (slp, vnpp);
      }
    }
  } else if (bsp->repr == Seq_repr_delta) {
    for (dsp = (DeltaSeqPtr) (bsp->seq_ext); dsp != NULL; dsp = dsp->next) {
      if (dsp->choice == 1) {
        slp = (SeqLocPtr) dsp->data.ptrvalue;
        if (slp != NULL && slp->choice != SEQLOC_NULL) {
          LockAllSegments (slp, vnpp);
        }
      }
    }
  }
}

static void LockAllLocations (SeqFeatPtr sfp, Pointer userdata)

{
  SeqLocPtr        slp = NULL;
  ValNodePtr PNTR  vnpp;

  if (sfp == NULL) return;
  vnpp = (ValNodePtr PNTR) userdata;
  if (vnpp == NULL) return;

  while ((slp = SeqLocFindNext (sfp->location, slp)) != NULL) {
    if (slp != NULL && slp->choice != SEQLOC_NULL) {
      LockAllSegments (slp, vnpp);
    }
  }
}

static void LockAllProducts (SeqFeatPtr sfp, Pointer userdata)

{
  SeqLocPtr        slp = NULL;
  ValNodePtr PNTR  vnpp;

  if (sfp == NULL) return;
  vnpp = (ValNodePtr PNTR) userdata;
  if (vnpp == NULL) return;

  while ((slp = SeqLocFindNext (sfp->product, slp)) != NULL) {
    if (slp != NULL && slp->choice != SEQLOC_NULL) {
      LockAllSegments (slp, vnpp);
    }
  }
}

static void LockAllSublocs (SeqLocPtr loc, Pointer userdata)

{
  SeqLocPtr        slp = NULL;
  ValNodePtr PNTR  vnpp;

  if (loc == NULL) return;
  vnpp = (ValNodePtr PNTR) userdata;
  if (vnpp == NULL) return;

  while ((slp = SeqLocFindNext (loc, slp)) != NULL) {
    if (slp != NULL && slp->choice != SEQLOC_NULL) {
      LockAllSegments (slp, vnpp);
    }
  }
}

NLM_EXTERN ValNodePtr LockFarComponentsEx (SeqEntryPtr sep, Boolean components, Boolean locations, Boolean products, SeqLocPtr loc)

{
  ValNodePtr   bsplist = NULL;
  SeqEntryPtr  oldsep;

  if (sep == NULL) return NULL;
  oldsep = SeqEntrySetScope (sep);
  if (components) {
    VisitBioseqsInSep (sep, (Pointer) &bsplist, LockAllBioseqs);
  }
  if (locations) {
    VisitFeaturesInSep (sep, (Pointer) &bsplist, LockAllLocations);
  }
  if (products) {
    VisitFeaturesInSep (sep, (Pointer) &bsplist, LockAllProducts);
  }
  if (loc != NULL) {
    LockAllSublocs (sep, (Pointer) &bsplist);
  }
  SeqEntrySetScope (oldsep);
  return bsplist;
}

NLM_EXTERN ValNodePtr LockFarComponents (SeqEntryPtr sep)

{
  return LockFarComponentsEx (sep, TRUE, FALSE, FALSE, NULL);
}

NLM_EXTERN ValNodePtr UnlockFarComponents (ValNodePtr bsplist)

{
  BioseqPtr   bsp;
  ValNodePtr  vnp;

  if (bsplist == NULL) return NULL;
  for (vnp = bsplist; vnp != NULL; vnp = vnp->next) {
    bsp = (BioseqPtr) vnp->data.ptrvalue;
    if (bsp != NULL) {
      BioseqUnlock (bsp);
    }
  }
  return ValNodeFree (bsplist);
}

/*****************************************************************************
*
*   SeqMgrSetPreCache
*       registers the GiToSeqID precache function
*   LookupFarSeqIDs
*       calls any registered function to preload the cache
*
*****************************************************************************/

NLM_EXTERN void LIBCALL SeqMgrSetPreCache (SIDPreCacheFunc func)

{
  SeqMgrPtr  smp;

  smp = SeqMgrWriteLock ();
  if (smp == NULL) return;
  smp->seq_id_precache_func = func;
  SeqMgrUnlock ();
}

NLM_EXTERN Int4 LookupFarSeqIDs (
  SeqEntryPtr sep,
  Boolean components,
  Boolean locations,
  Boolean products,
  Boolean alignments,
  Boolean history
)

{
  SIDPreCacheFunc  func;
  SeqMgrPtr        smp;

  smp = SeqMgrReadLock ();
  if (smp == NULL) return 0;
  func = smp->seq_id_precache_func;
  SeqMgrUnlock ();
  if (func == NULL) return 0;
  return (*func) (sep, components, locations, products, alignments, history);
}

/*****************************************************************************
*
*   SeqMgrSetSeqIdSetFunc
*       registers the GiToSeqIdSet lookup function
*   GetSeqIdSetForGI
*       calls any registered function to lookup the set of SeqIds
*
*****************************************************************************/

NLM_EXTERN void LIBCALL SeqMgrSetSeqIdSetFunc (SeqIdSetLookupFunc func)

{
  SeqMgrPtr  smp;

  smp = SeqMgrWriteLock ();
  if (smp == NULL) return;
  smp->seq_id_set_lookup_func = func;
  SeqMgrUnlock ();
}

NLM_EXTERN SeqIdPtr LIBCALL GetSeqIdSetForGI (Int4 gi)

{
  SeqIdSetLookupFunc  func;
  SeqMgrPtr           smp;

  smp = SeqMgrReadLock ();
  if (smp == NULL) return 0;
  func = smp->seq_id_set_lookup_func;
  SeqMgrUnlock ();
  if (func == NULL) return 0;
  return (*func) (gi);
}

/*****************************************************************************
*
*   SeqMgrSetLenFunc
*       registers the GiToSeqLen lookup function
*   SeqMgrSetAccnVerFunc
*       registers the GiToAccnVer lookup function
*
*****************************************************************************/

NLM_EXTERN void LIBCALL SeqMgrSetLenFunc (SeqLenLookupFunc func)

{
  SeqMgrPtr  smp;

  smp = SeqMgrWriteLock ();
  if (smp == NULL) return;
  smp->seq_len_lookup_func = func;
  SeqMgrUnlock ();
}

NLM_EXTERN void LIBCALL SeqMgrSetAccnVerFunc (AccnVerLookupFunc func)

{
  SeqMgrPtr  smp;

  smp = SeqMgrWriteLock ();
  if (smp == NULL) return;
  smp->accn_ver_lookup_func = func;
  SeqMgrUnlock ();
}

/*******************************************************************
*
*   SeqEntryAsnOut()
*
*       dumps parts of SeqEntry from a memory object
*
*******************************************************************/

typedef struct ext_pack_data {
   SeqEntryPtr  sep [5];
   Uint2        minSapItemID;
   Uint2        maxSapItemID;
   ValNodePtr   descChain;
   ValNodePtr   featChain;
   ValNodePtr   lastVnp;
} ExtPackData, PNTR ExtPackPtr;

static void GetSapBounds (SeqAnnotPtr sap, Pointer userdata)

{
   ExtPackPtr  epp;

   epp = (ExtPackPtr) userdata;
   epp->minSapItemID = MIN (epp->minSapItemID, sap->idx.itemID);
   epp->maxSapItemID = MAX (epp->maxSapItemID, sap->idx.itemID);
}

NLM_EXTERN Boolean SeqEntryAsnOut (SeqEntryPtr sep, SeqIdPtr sip,
                                    Int2 retcode, AsnIoPtr aipout)

{
   BioseqPtr          bsp;
   BioseqSetPtr       bssp;
   SeqMgrFeatContext  context;
   Uint2              entityID;
   ExtPackData        epd;
   SeqEntryPtr        oldscope;
   BioseqSetPtr       parent;
   SeqAnnotPtr        sap;
   SeqDescrPtr        sdp;
   SeqFeatPtr         sfp;
   SeqEntryPtr        top;
   ValNodePtr         vnp;
   AsnOptionPtr       aopp_feat = NULL, aopp_desc = NULL;
   DataVal            dv;

   if (sep == NULL || sip == NULL || aipout == NULL) return FALSE;

   if (retcode > 4) {
     retcode = 0;
   }
   if (retcode < 0) {
     retcode = 0;
   }

   entityID = ObjMgrGetEntityIDForChoice (sep);
   if (entityID < 1) return FALSE;
   top = GetTopSeqEntryForEntityID (entityID);
   if (top == NULL) return FALSE;

   /* indexing sets idx fields, will find features outside of desired 
SeqEntry */

   if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
     SeqMgrIndexFeatures (entityID, NULL);
   }

   /* find Bioseq within entity given SeqId */

   oldscope = SeqEntrySetScope (top);
   bsp = BioseqFind (sip);
   SeqEntrySetScope (oldscope);
   if (bsp == NULL) return FALSE;

   MemSet ((Pointer) &epd, 0, sizeof (ExtPackData));

   /* get parent hierarchy */

   epd.sep [0] = top;
   epd.sep [1] = bsp->seqentry;

   if (bsp->idx.parenttype == OBJ_BIOSEQSET) {
     parent = (BioseqSetPtr) bsp->idx.parentptr;
     while (parent != NULL) {
       switch (parent->_class) {
         case BioseqseqSet_class_nuc_prot :
           epd.sep [3] = parent->seqentry;
           break;
         case BioseqseqSet_class_segset :
           epd.sep [2] = parent->seqentry;
           break;
         case BioseqseqSet_class_pub_set :
           epd.sep [4] = parent->seqentry;
           break;
         default :
           break;
       }
       if (parent->idx.parenttype == OBJ_BIOSEQSET) {
         parent = (BioseqSetPtr) parent->idx.parentptr;
       } else {
         parent = NULL;
       }
     }
   }

   /* get desired SeqEntry given retcode parameter */

   sep = NULL;
   while (retcode >= 0 && sep == NULL) {
     sep = epd.sep [retcode];
     retcode --;
   }
   if (sep == NULL) return FALSE;

   /* get immediate parent of SeqEntry to be returned */

   parent = NULL;
   if (IS_Bioseq (sep)) {
     bsp = (BioseqPtr) sep->data.ptrvalue;
     if (bsp == NULL) return FALSE;
     if (bsp->idx.parenttype == OBJ_BIOSEQSET) {
       parent = (BioseqSetPtr) bsp->idx.parentptr;
     }
   } else if (IS_Bioseq_set (sep)) {
     bssp = (BioseqSetPtr) sep->data.ptrvalue;
     if (bssp == NULL) return FALSE;
     if (bssp->idx.parenttype == OBJ_BIOSEQSET) {
       parent = (BioseqSetPtr) bssp->idx.parentptr;
     }
   }

   /* find itemID range of SeqAnnots within current SeqEntry */

   epd.minSapItemID = UINT2_MAX;
   epd.maxSapItemID = 0;
   VisitAnnotsInSep (sep, (Pointer) &epd, GetSapBounds);

   /* go up parent hierarchy, pointing to applicable descriptors */

   epd.lastVnp = NULL;
   while (parent != NULL) {
     for (sdp = parent->descr; sdp != NULL; sdp = sdp->next) {
       vnp = ValNodeAddPointer (&(epd.lastVnp), 0, (Pointer) sdp);
       if (epd.descChain == NULL) {
         epd.descChain = epd.lastVnp;
       }
       epd.lastVnp = vnp;
     }
     if (parent->idx.parenttype == OBJ_BIOSEQSET) {
       parent = (BioseqSetPtr) parent->idx.parentptr;
     } else {
       parent = NULL;
     }
   }

   /* find features indexed on Bioseq that are packaged outside 
current SeqEntry */

   epd.lastVnp = NULL;
   sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &context);
   while (sfp != NULL) {
     sap = context.sap;
     if (sap != NULL) {
       if (sap->idx.itemID < epd.minSapItemID || sap->idx.itemID > 
epd.maxSapItemID) {
         vnp = ValNodeAddPointer (&(epd.lastVnp), 0, (Pointer) sfp);
         if (epd.featChain == NULL) {
           epd.featChain = epd.lastVnp;
         }
         epd.lastVnp = vnp;
       }
     }
     sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &context);
   }

   /* also need to get features whose products point to the Bioseq */

   sfp = NULL;
   if (ISA_na (bsp->mol)) {
     sfp = SeqMgrGetRNAgivenProduct (bsp, &context);
   } else if (ISA_aa (bsp->mol)) {
     sfp = SeqMgrGetCDSgivenProduct (bsp, &context);
   }
   if (sfp != NULL) {
     sap = context.sap;
     if (sap != NULL) {
       if (sap->idx.itemID < epd.minSapItemID || sap->idx.itemID > 
epd.maxSapItemID) {
         vnp = ValNodeAddPointer (&(epd.lastVnp), 0, (Pointer) sfp);
         if (epd.featChain == NULL) {
           epd.featChain = epd.lastVnp;
         }
         epd.lastVnp = vnp;
       }
     }
   }

   /* now write sep, adding descriptors from descChain and features 
from featChain */

  MemSet(&dv, 0, sizeof(DataVal));  /* zero it out */
  if (epd.descChain)   /* have extra descriptors */
  {
    dv.ptrvalue = (Pointer)(epd.descChain);
    aopp_desc = AsnIoOptionNew(aipout, OP_NCBIOBJSEQ, CHECK_EXTRA_DESC, dv, NULL);
  } 

  if (epd.featChain)   /* have extra features */
  {
    dv.ptrvalue = (Pointer)(epd.featChain);
    aopp_feat = AsnIoOptionNew(aipout, OP_NCBIOBJSEQ, CHECK_EXTRA_FEAT, dv, NULL);
  }

  SeqEntryAsnWrite(sep, aipout, NULL);

   /* clean up valnode chains */

   ValNodeFree (epd.descChain);
   ValNodeFree (epd.featChain);

   return TRUE;
}

/*
static void SeqMgrReport (void)

{
  BioseqPtr                  bsp;
  BioseqPtr PNTR             bspp;
  Int4                       i, num;
  ObjMgrDataPtr              omdp;
  ObjMgrPtr                  omp;
  SeqIdIndexElementPtr PNTR  sipp;
  SeqMgrPtr                  smp;
  Char                       str [41];

  omp = ObjMgrGet ();
  if (omp != NULL) {
    printf ("Currobj %d, totobj %d\n", (int) omp->currobj, (int) omp->totobj);
    fflush (stdout);
  }
  smp = SeqMgrGet ();
  if (smp != NULL) {
    num = smp->BioseqIndexCnt;
    sipp = smp->BioseqIndex;
    printf ("BioseqIndexCnt %ld\n", (long) num);
    fflush (stdout);
    if (sipp == NULL) {
      printf ("sipp is NULL\n");
      fflush (stdout);
    } else {
      for (i = 0; i < num; i++) {
        omdp = sipp [i]->omdp;
        if (omdp != NULL && omdp->bulkIndexFree) {
          printf ("omdp %ld bulkIndexFree flag set\n", (long) i);
          fflush (stdout);
        }
        StringNCpy_0 (str, sipp [i]->str, sizeof (str));
        RevStringUpper (str);
        printf (" %3ld - %s\n", (long) i, str);
        fflush (stdout);
      }
      printf ("-\n");
      fflush (stdout);
      for (i = smp->BioseqIndexCnt; i < smp->BioseqIndexNum; i++) {
        StringNCpy_0 (str, sipp [i]->str, sizeof (str));
        RevStringUpper (str);
        if (! StringHasNoText (str)) {
          printf (" %3ld - %s\n", (long) i, str);
          fflush (stdout);
        }
      }
      printf ("-\n");
      fflush (stdout);
    }
    num = smp->NonIndexedBioseqCnt;
    bspp = smp->NonIndexedBioseq;
    printf ("NonIndexedBioseqCnt %ld\n", (long) num);
    fflush (stdout);
    if (bspp == NULL) {
      printf ("bspp is NULL\n");
      fflush (stdout);
      return;
    }
    for (i = 0; i < num; i++) {
      bsp = bspp [i];
      if (bsp != NULL) {
        SeqIdWrite (bsp->id, str, PRINTID_FASTA_LONG, sizeof (str) - 1);
        printf (" %3ld - %s\n", (long) i, str);
        fflush (stdout);
      } else {
        printf (" %3ld - (null)\n", (long) i);
        fflush (stdout);
      }
    }
  }
  printf ("\n");
  fflush (stdout);
}
*/




