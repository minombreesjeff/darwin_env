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
* $Revision: 6.589 $
*
* File Description:  Sequence editing utilities
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: valid.c,v $
* Revision 6.589  2005/04/26 21:33:52  kans
* added SEQID_GPIPE
*
* Revision 6.588  2005/04/22 13:58:53  kans
* added ERR_SEQ_DESCR_BioSourceInconsistency
*
* Revision 6.587  2005/04/21 18:00:46  kans
* do not need ValidateCDSmRNAoneToOne app property set to check for ambiguity between CDSs and mRNAs
*
* Revision 6.586  2005/04/21 17:19:37  kans
* added ERR_SEQ_FEAT_CDSwithMultipleMRNAs
*
* Revision 6.585  2005/04/20 16:37:24  kans
* added ERR_SEQ_DESCR_RefGeneTrackingOnNonRefSeq
*
* Revision 6.584  2005/04/12 12:44:13  kans
* CdTransCheck suppresses unnecessary exception for rearrangement required
*
* Revision 6.583  2005/04/08 16:21:55  kans
* if gene feature has allele gbqual and grp allele, report it - BSEC should remove identical copy
*
* Revision 6.582  2005/04/06 19:20:57  kans
* transgenic and environmental_sample should not both be present
*
* Revision 6.581  2005/04/05 20:35:36  kans
* check for strain in environmental sample
*
* Revision 6.580  2005/04/01 22:27:26  kans
* added ERR_SEQ_FEAT_ProteinNameEndsInBracket
*
* Revision 6.579  2005/03/28 20:52:43  kans
* added GapInProtein and BadProteinStart
*
* Revision 6.578  2005/03/26 18:59:10  kans
* CdTransCheck looks for artificial frameshift exception, suppresses unnecessary exception warning if present and no problems detected in this function
*
* Revision 6.577  2005/03/24 21:29:56  kans
* in ValidateExceptText, if reasons given in citation exception but no citation, report MissingQualOnFeature warning
*
* Revision 6.576  2005/03/22 17:15:37  kans
* added ERR_SEQ_FEAT_PolyATail
*
* Revision 6.575  2005/03/16 18:46:56  kans
* SeqDataNotFound raised to SEV_REJECT
*
* Revision 6.574  2005/03/16 15:34:49  kans
* in MrnaTransCheck, unclassified exception and refseq drops mismatch and transcript length error down to sev_warning
*
* Revision 6.573  2005/03/11 17:57:09  kans
* CheckForCommonCDSProduct reports MissingCDSproduct instead of MultipleCDSproducts if unable to find product
*
* Revision 6.572  2005/03/11 15:06:29  kans
* CheckForCommonCDSProduct was incorrectly bailing if no overlapping gene
*
* Revision 6.571  2005/03/10 22:30:33  kans
* CheckFeatPacking detects misplaced feature if feature packaged on set but bioseq not packaged in same set - it was giving false negative when it hit the top
*
* Revision 6.570  2005/03/08 22:12:34  kans
* artificial and not other genetic also needs to filter out peptide sequences
*
* Revision 6.569  2005/03/08 21:19:18  kans
* artificial should have other-genetic and synthetic construct
*
* Revision 6.568  2005/03/08 19:20:59  kans
* InvalidForType if BioSource.origin.artificial but not MolInfo.biomol.other-genetic
*
* Revision 6.567  2005/03/03 18:14:37  kans
* ERR_SEQ_FEAT_BadProductSeqId calls ValidateAccn on tsip->name to give specific message
*
* Revision 6.566  2005/03/03 17:51:01  kans
* restored ERR_SEQ_INST_SeqLitGapLength0, distinguish with and without unknown fuzz
*
* Revision 6.565  2005/03/03 17:05:00  kans
* suppress ERR_SEQ_INST_SeqLitGapLength0 if gap of unknown length indicated by fuzz
*
* Revision 6.564  2005/03/01 20:39:34  kans
* ERR_SEQ_FEAT_UTRdoesNotAbutCDS counts UTRs and only reports problems if fewer than 2
*
* Revision 6.563  2005/02/15 19:14:27  shomrat
* ERR_SEQ_FEAT_BadCharInAuthorName returned to SEV_ERROR; Allow St. in author's last name
*
* Revision 6.562  2005/02/15 18:41:57  shomrat
* Allow period in author's last name; ERR_SEQ_FEAT_BadCharInAuthorName is SEV_WARNING
*
* Revision 6.561  2005/02/09 21:13:58  shomrat
* Suppress gene on prot test if the prot bioseq is not provided
*
* Revision 6.560  2005/02/01 18:52:24  kans
* compare qualifier cannot be RefSeq accession if GenBank/EMBL/DDBJ record
*
* Revision 6.559  2005/01/31 17:43:11  kans
* ERR_SEQ_FEAT_BadCharInAuthorName is SEV_WARNING for RefSeq
*
* Revision 6.558  2005/01/26 23:38:46  kans
* added ValidateAccnDotVer, check /compare in flatfile generator and validator
*
* Revision 6.557  2005/01/26 22:44:16  kans
* added ERR_SEQ_FEAT_BadCharInAuthorName
*
* Revision 6.556  2005/01/26 20:45:54  kans
* CodonQualifierUsed goes to SEV_ERROR by request
*
* Revision 6.555  2005/01/18 15:55:58  kans
* changed to WrongQualOnFeature, MissingQualOnFeature, CodonQualifierUsed, UnknownFeatureQual
*
* Revision 6.554  2005/01/14 22:21:38  kans
* If circular molecule without complete sequence or complete genome in title, use CompleteCircleProblem at SEV_WARNING instead of UnwantedCompleteFlag
*
* Revision 6.553  2005/01/14 16:21:29  kans
* MolNuclAcid targets molinfo, and InconsistentBioSources targets popset
*
* Revision 6.552  2005/01/04 14:20:29  shomrat
* Dropped ERR_SEQ_FEAT_NestedSeqLocMix from REJECT to ERROR
*
* Revision 6.551  2004/12/30 22:03:37  kans
* added ERR_SEQ_FEAT_NestedSeqLocMix
*
* Revision 6.550  2004/12/30 15:39:37  kans
* changed SkipSerialPub to SkipSerialOrUIDPub to prevent NULL dereference
*
* Revision 6.549  2004/12/30 14:57:34  kans
* added ERR_SEQ_FEAT_FeatureCitationProblem
*
* Revision 6.548  2004/12/29 15:58:52  kans
* made LookForMultipleUnpubPubs robust against very long publication or author strings
*
* Revision 6.547  2004/12/29 15:32:39  kans
* added LookForMultipleUnpubPubs for non-part nucleotide - crude attempt at finding redundant pubs (descriptors only for now) that the flatfile suppresses
*
* Revision 6.546  2004/12/28 22:14:47  kans
* added ERR_SEQ_DESCR_MultipleTitles
*
* Revision 6.545  2004/12/27 22:37:35  kans
* add UnnecessaryGeneXref test on gene feature itself
*
* Revision 6.544  2004/12/27 18:08:43  kans
* added ValidateNonImpFeat for checking gbquals on non-imp features
*
* Revision 6.543  2004/12/23 20:50:51  kans
* added context field to new callback
*
* Revision 6.542  2004/12/23 17:13:41  kans
* CheckMultiIntervalGene calls GeneSpansOrigin if 2 intervals
*
* Revision 6.541  2004/12/23 16:16:16  kans
* minor fixes to location and product string sent to new callback
*
* Revision 6.540  2004/12/22 23:39:40  kans
* MultiIntervalGene skipped if 2 intervals (still need to test for spanning origin), warning severity otherwise if circular
*
* Revision 6.539  2004/12/22 21:56:40  kans
* CustValErr supports ValidErrorFunc callback for finer error reporting
*
* Revision 6.538  2004/12/20 22:57:16  kans
* added verbosityLevel argument - to be used for finer control over error reporting by asn2val
*
* Revision 6.537  2004/12/20 19:57:55  kans
* CompleteTitleProblem and CompleteCircleProblem double click to launch editor on MolInfo descriptor, not sequence instance
*
* Revision 6.536  2004/12/17 20:55:13  kans
* check for bsp NULL before printing SeqID or worst ID
*
* Revision 6.535  2004/12/16 15:49:43  kans
* added subsource qualifiers for Barcode of Life project
*
* Revision 6.534  2004/12/15 20:00:07  kans
* PID family, NID, GI not allowed as instantiated db_xref, GI made only by flatfile generator
*
* Revision 6.533  2004/12/14 21:27:19  kans
* SEQ_INST.TerminalNs uses SEV_WARNING for patent sequences
*
* Revision 6.532  2004/12/14 20:29:00  kans
* MissingGeneXref and UnnecessaryGeneXref use LOCATION_SUBSET instead of CONTAINED_WITHIN to handle multi-interval genes covering tRNAs with intron covering origin
*
* Revision 6.531  2004/12/13 19:26:17  kans
* MissingGeneXref and UnnecessaryGeneXref use CONTAINED_WITHIN overlap type when getting all overlapping genes
*
* Revision 6.530  2004/12/10 19:37:19  kans
* added ERR_SEQ_FEAT_MissingGeneXref
*
* Revision 6.529  2004/12/08 22:18:20  kans
* compare qualifier can now substitute for citation qualifier for conflict and old_sequence features
*
* Revision 6.528  2004/12/08 20:20:59  kans
* changed composite to composite-wgs-htgs
*
* Revision 6.527  2004/12/08 20:00:12  kans
* check MI_TECH_composite against bsp->mol
*
* Revision 6.526  2004/12/07 20:40:48  kans
* fixed text of unclassified 5 prime or 3 prime partial location problem in ValidateSeqFeat
*
* Revision 6.525  2004/11/17 15:01:35  kans
* using Chlorarachniophyceae for nucleomorph test
*
* Revision 6.524  2004/11/17 14:56:18  kans
* changed Chlorarchniophyta to Chlorarachniophyta
*
* Revision 6.523  2004/11/15 19:51:20  kans
* check citart.jour.imp for missing or bad publication date
*
* Revision 6.522  2004/11/15 15:21:07  kans
* added ERR_SEQ_FEAT_PseudoCdsViaGeneHasProduct
*
* Revision 6.521  2004/11/01 21:54:09  kans
* CheckFeatPacking looks for par == NULL || par->id != NULL to suppress errors on generated gap features
*
* Revision 6.520  2004/10/29 17:55:58  kans
* if unclassified translation exception, report InternalStop and TransLen as SEV_WARNING
*
* Revision 6.519  2004/10/19 21:10:47  kans
* check compare qualifier (single element only for now) for legal accession
*
* Revision 6.518  2004/10/19 19:58:11  kans
* drop MrnaTransFail on fetching transcript product to SEV_WARNING if ID is not GI
*
* Revision 6.517  2004/10/19 18:26:07  kans
* ERR_SEQ_FEAT_MultipleCDSproducts suppressed if genomic product set or genbank set on top of genomic product set
*
* Revision 6.516  2004/10/14 19:36:34  kans
* CreateDefLineExEx has extProtTitle argument, normally only use first protein name in defline
*
* Revision 6.515  2004/10/04 15:50:22  kans
* added vsp->justShowAccession for extremely terse output
*
* Revision 6.514  2004/09/28 19:36:06  kans
* unnecessary gene xref test also looks at locus_tag
*
* Revision 6.513  2004/09/23 16:26:45  kans
* report ShortSeq for proteins only if molinfo does not indicate partial
*
* Revision 6.512  2004/09/23 15:09:42  kans
* use GBQualsDiffer to set samelabel = FALSE for all features, set SEV_WARNING always for repeat_region
*
* Revision 6.511  2004/09/23 14:54:37  kans
* repeat region checks if GBQualsDiffer, does not report
*
* Revision 6.510  2004/09/23 14:33:46  kans
* ValidateCDSmRNAmatch takes gene xref label into account
*
* Revision 6.509  2004/09/22 17:56:47  kans
* added ReplaceQualsDiffer test for variations to suppress DuplicateFeat and FeatContentDup for SNPs
*
* Revision 6.508  2004/09/21 20:45:51  kans
* do not report InvalidQualifierValue when allele on variation does not match allele on gene
*
* Revision 6.507  2004/09/20 19:17:11  kans
* ERR_SEQ_FEAT_TranscriptMismatches not reported if no mismatches (polyA size adjustment caused 0 to be reported)
*
* Revision 6.506  2004/09/20 14:51:06  kans
* added ERR_SEQ_INST_BadHTGSeq
*
* Revision 6.505  2004/09/17 20:47:53  kans
* fixed typo in PartialAtSpliceSite in consensus splice sequence
*
* Revision 6.504  2004/09/14 21:15:03  kans
* removed REBASE from refseq dbxref special list because it is in asn2gnb6.c general refseq list
*
* Revision 6.503  2004/09/14 15:22:25  kans
* PartialAtSpliceSite uses stream cache instead of seqport
*
* Revision 6.502  2004/09/14 14:49:18  kans
* IS_HTG_IN_SEP drops missing affil on citsub down to warning if htg record
*
* Revision 6.501  2004/09/13 21:29:29  kans
* added ERR_SEQ_FEAT_MrnaTransFail, support mismatches in transcription exception
*
* Revision 6.500  2004/09/13 21:14:56  kans
* added mismatches in transcription and translation exception texts, allow certain CdTransCheck errors with unclassified or mismatch exceptions
*
* Revision 6.499  2004/09/13 15:58:48  kans
* added ValidateLocusTagGeneral, reports ERR_SEQ_FEAT_LocusTagProductMismatch
*
* Revision 6.498  2004/09/10 21:05:27  kans
* SpliceCheckEx needed to pass minus strand location to StreamCacheSetup for minus strand features
*
* Revision 6.497  2004/09/10 19:45:10  kans
* SpliceCheckEx uses StreamCache mechanism instead of SeqPort
*
* Revision 6.496  2004/09/10 18:59:48  kans
* prelock far components only if validationLimit is all, inst, or hist
*
* Revision 6.495  2004/09/10 17:52:05  kans
* changed ValidateLimit enum to Int2 defines
*
* Revision 6.494  2004/09/10 15:31:43  kans
* added farFetchMRNAproducts, locusTagGeneralMatch, validationLimit flags to vsp
*
* Revision 6.493  2004/09/08 17:46:23  kans
* MrnaTransCheck uses vsp->farFetchCDSproducts for far fetching
*
* Revision 6.492  2004/09/08 17:17:32  kans
* fixed bug in SortGmcByGenePtr
*
* Revision 6.491  2004/09/08 14:00:51  kans
* fixed CdTransCheck missing brackets, report (Far) for residue mismatches
*
* Revision 6.490  2004/09/07 19:33:12  kans
* MrnaTransCheck, CdTransCheck, and SpliceCheckEx suppress messages if specific exceptions, but do tests and report if no problem detected
*
* Revision 6.489  2004/09/03 21:06:51  kans
* ValidateCDSmRNAmatch always at SEV_WARNING
*
* Revision 6.488  2004/09/03 20:19:28  kans
* added ValidateCDSmRNAmatch reporting ERR_SEQ_FEAT_CDSmRNAmismatch
*
* Revision 6.487  2004/09/01 17:39:44  kans
* look for redundant or conflicting pmids or muids in a publication
*
* Revision 6.486  2004/08/30 20:36:11  kans
* set electronic_journal based on imp->pubstatus
*
* Revision 6.485  2004/08/30 20:16:30  kans
* added MolInfo.tech barcode support
*
* Revision 6.484  2004/08/20 19:10:53  kans
* implemented ERR_SEQ_FEAT_UnindexedFeature
*
* Revision 6.483  2004/08/19 16:19:00  kans
* except for replace, qualifier with just double quotes or spaces returns InvalidQualifierValue
*
* Revision 6.482  2004/08/13 19:57:08  kans
* in ValidateBioseqInst, suppress BadSeqIdFormat message for segmented DDBJ record
*
* Revision 6.481  2004/08/11 17:34:34  kans
* also check legalRefSeqDbXrefs maintained in asn2gnb code
*
* Revision 6.480  2004/08/06 20:09:38  kans
* added ERR_SEQ_INST_CompleteTitleProblem and ERR_SEQ_INST_CompleteCircleProblem
*
* Revision 6.479  2004/08/04 15:06:35  kans
* added ERR_SEQ_FEAT_TaxonDbxrefOnFeature
*
* Revision 6.478  2004/08/02 20:39:40  kans
* added ERR_GENERIC_MedlineEntryPub
*
* Revision 6.477  2004/07/30 21:32:55  kans
* ERR_GENERIC_MissingPubInfo missing cit-sub affil down to warning if refseq
*
* Revision 6.476  2004/07/29 20:11:32  kans
* TranscriptLen is warning if far, but back to error if RefSeq, since it may cause PartialProblem errors from CdTransCheck
*
* Revision 6.475  2004/07/29 14:15:58  kans
*  added ERR_SEQ_DESCR_TransgenicProblem
*
* Revision 6.474  2004/07/23 16:00:26  kans
* added four oceans to list of valid countries
*
* Revision 6.473  2004/07/19 15:01:50  kans
* CheckForBadGeneOverlap checks for overlapping operon before reporting mRNAgeneRange error
*
* Revision 6.472  2004/07/12 17:35:59  kans
* ERR_SEQ_INST_IdOnMultipleBioseqs is now SEV_REJECT level
*
* Revision 6.471  2004/07/09 19:23:43  kans
* suppress DuplicateFeat warning if partialL or partialR are different and the labels are different
*
* Revision 6.470  2004/07/06 13:26:42  kans
* added ERR_SEQ_FEAT_NoNameForProtein warning - protref description but no name
*
* Revision 6.469  2004/07/01 20:03:18  kans
* added ValidateAnticodon, now that we have an anticodon split by an intron
*
* Revision 6.468  2004/07/01 17:46:04  kans
* allow multi-interval anticodons
*
* Revision 6.467  2004/06/30 14:33:01  kans
* TranslExceptAndRnaEditing forgot to add test for crp->code_break != NULL
*
* Revision 6.466  2004/06/24 22:04:58  kans
* ERR_SEQ_INST_MissingGaps also does phase 3
*
* Revision 6.465  2004/06/24 21:53:10  kans
* added ERR_SEQ_FEAT_TranslExceptAndRnaEditing
*
* Revision 6.464  2004/06/24 21:13:26  kans
* added ERR_SEQ_INST_MissingGaps
*
* Revision 6.463  2004/06/24 17:49:09  kans
* lock far locations and products, not just far components
*
* Revision 6.462  2004/06/23 20:56:35  kans
* added check for DuplicateTranslExcept
*
* Revision 6.461  2004/06/17 14:30:27  kans
* case sensitive tests for FlyBase when raising severity for duplicate feature check
*
* Revision 6.460  2004/06/14 18:49:52  kans
* TGA can be used for Selenocysteine without needing modified codon recognition exception
*
* Revision 6.459  2004/06/10 19:04:29  kans
* ERR_SEQ_INST_GiWithoutAccession drops to WARNING if validator run in tbl2asnf
*
* Revision 6.458  2004/06/03 18:00:18  kans
* added LookForMultiplePubs, ERR_SEQ_DESCR_CollidingPublications
*
* Revision 6.457  2004/05/28 19:56:50  kans
* ifdef out section checking for length >350000
*
* Revision 6.456  2004/05/27 21:28:18  kans
* FlyBase joins FLYBASE as legal capitalization for dbxref
*
* Revision 6.455  2004/05/24 20:17:24  kans
* removed non-preferred variants ribosome slippage, trans splicing, alternate processing, and non-consensus splice site
*
* Revision 6.454  2004/05/24 17:28:26  kans
* ERR_SEQ_INST_BadSeqIdFormat allows 2 letters + underscore + 9 digits, ValidateAccn does the same
*
* Revision 6.453  2004/05/12 18:55:33  kans
* StreamCache takes SeqLocPtr as well as BioseqPtr optional arguments, slp version is equivalent of SeqPortNewByLoc
*
* Revision 6.452  2004/05/06 19:42:22  kans
* new function GetValidCountryList for access to country code list, which is now NULL terminated
*
* Revision 6.451  2004/05/03 12:20:23  kans
* use StreamCache in ValidateBioseqInst, CdTransCheck, latter also uses BSMerge into a buffer instead of many calls to BSGetByte
*
* Revision 6.450  2004/04/29 14:53:41  kans
* UTR check on minus strand for genomics (if one CDS), removed SeqLocIdForProduct and GetAccnVerFromServer unused static copies
*
* Revision 6.449  2004/04/26 20:59:27  kans
* implemented ERR_SEQ_FEAT_UTRdoesNotAbutCDS for genomic sequence with one CDS on minus strand
*
* Revision 6.448  2004/04/26 20:23:35  kans
* ERR_SEQ_FEAT_UTRdoesNotAbutCDS now done for genomic if single CDS, still need logic for minus strand test
*
* Revision 6.447  2004/04/26 19:33:55  kans
* NoProtRefFound reports CDS entityID/itemID/itemtype, not Bioseq-inst
*
* Revision 6.446  2004/04/26 18:55:22  kans
* do not report NoProtRefFound if virtual, or if a segmented part where the parent has a best protein feature
*
* Revision 6.445  2004/04/16 20:02:19  kans
* IllegalDbXref checks for capitalization difference
*
* Revision 6.444  2004/04/16 18:29:29  kans
* ERR_SEQ_INST_InternalNsInSeqLit gives more informative message
*
* Revision 6.443  2004/04/01 20:14:18  kans
* ERR_SEQ_DESCR_UnwantedCompleteFlag is WARNING if MI_TECH_htgs_3
*
* Revision 6.442  2004/03/31 21:26:53  kans
* clarify explanation of GraphAbove and GraphBelow criteria
*
* Revision 6.441  2004/03/31 18:26:23  kans
* Report GraphMax > 100 at WARNING level, < or = to 0 at ERROR level
*
* Revision 6.440  2004/03/25 18:52:15  shomrat
* report the gene feature on the mRNA
*
* Revision 6.439  2004/03/24 19:32:28  kans
* added support for ERR_SEQ_FEAT_GenesInconsistent
*
* Revision 6.438  2004/03/18 20:46:46  kans
* use ERR_SEQ_FEAT_PartialsInconsistent in two places
*
* Revision 6.437  2004/03/15 19:56:52  kans
* flags argument to SeqPortStream
*
* Revision 6.436  2004/03/10 22:40:51  kans
* Gene is invalid feature for a protein bioseq only if not in a nuc-prot, mut/phy/pop/eco or gen-prod set
*
* Revision 6.435  2004/03/10 19:39:24  kans
* suppress ERR_SEQ_FEAT_AltStartCodon for GenBank/EMBL/DDBJ and Local
*
* Revision 6.434  2004/03/09 19:52:33  kans
* implemented ERR_SEQ_DESCR_UnwantedCompleteFlag, suppress alternative start codon warning if genbank record
*
* Revision 6.433  2004/02/25 22:31:01  kans
* Do not complain about length > 350000 if NT or NC
*
* Revision 6.432  2004/02/25 21:27:07  kans
* switch to ERR_SEQ_FEAT_AltStartCodon
*
* Revision 6.431  2004/02/25 20:27:06  kans
* check for alternative start codon, suppress warning with specific exception text
*
* Revision 6.430  2004/02/25 14:54:23  kans
* added ERR_SEQ_FEAT_CollidingLocusTags check
*
* Revision 6.429  2004/02/24 16:21:47  kans
* Raise ERR_SEQ_INST_ZeroGiNumber to SEV_REJECT, also complain if gi is less than or equal to 0
*
* Revision 6.428  2004/02/18 18:51:47  kans
* check length of far delta component against sintp->to reference, report SeqDataLenWrong if beyond range
*
* Revision 6.427  2004/01/20 22:25:49  kans
* when run from tbl2asn, suppress MissingLineage, GenCodeMismatch, and NoTaxonID messages
*
* Revision 6.426  2004/01/20 22:09:36  kans
* tbl2asn sets vsp->seqSubmitParent flag, use this to drop MissingLineage from error to warning
*
* Revision 6.425  2004/01/16 17:10:50  kans
* implemented ERR_SEQ_FEAT_LocusTagProblem
*
* Revision 6.424  2004/01/13 18:33:26  kans
* ValidateCitSub checks ap->affil, set vsp->gcp fields for sbp->sub call to it
*
* Revision 6.423  2004/01/12 18:08:36  kans
* ValidateCitSub called from ValidatePubdesc, gives error if no affiliation fields set
*
* Revision 6.422  2004/01/07 20:47:21  kans
* validate dbxref with legalDbXrefs in asn2gnbi.h instead of old flatfile generator symbols
*
* Revision 6.421  2004/01/02 15:06:23  kans
* modified codon recognition exception text suppresses TrnaCodonWrong error
*
* Revision 6.420  2003/12/15 16:47:17  kans
* artificial, mutagenized, or synthetic suppress other-genetic message
*
* Revision 6.419  2003/12/11 20:49:09  kans
* allow multiple adjacent 3prime UTRs, used in segmented sequences
*
* Revision 6.418  2003/12/11 19:22:24  kans
* electronic journal suppresses no volume, no iso jta errors
*
* Revision 6.417  2003/12/10 18:24:10  kans
* Implemented ERR_SEQ_FEAT_BadConflictFlag and ERR_SEQ_FEAT_ConflictFlagSet
*
* Revision 6.416  2003/12/09 22:25:46  kans
* implemented ERR_SEQ_INST_SeqLocLength test
*
* Revision 6.415  2003/12/08 21:10:03  kans
* added ERR_SEQ_PKG_GraphPackagingProblem, now can only collect graphs on bioseq to validate
*
* Revision 6.414  2003/12/08 19:10:23  kans
* ValidateGraphsOnBioseq uses BSRead instead of BSGetByte for great speedup
*
* Revision 6.413  2003/12/05 18:52:12  kans
* ValidateSeqEntry does not set do_many if BioseqseqSet_class_genbank.  GatherSeqEntry visits everything, and should not be called in a loop.
*
* Revision 6.412  2003/12/04 22:30:43  kans
* ValidateGraphOnBioseq uses visit instead of nested gather
*
* Revision 6.411  2003/12/03 19:52:11  kans
* TerminalNs is warning if NC_ record, never error
*
* Revision 6.410  2003/12/02 19:54:49  kans
* ValidateSeqEntry does not need to AssignIDsInEntity every time through the loop, just at beginning
*
* Revision 6.409  2003/12/02 15:37:37  kans
* added vsp->seqSubmitParent for use by tbl2asn, which usually has a Seq-submit wrapper that is added on-the-fly and not indexed
*
* Revision 6.408  2003/11/19 18:54:42  kans
* segmented sequence does not need TPA user object, 5prime UTR only needs to abut first CDS, if mRNA is segmented
*
* Revision 6.407  2003/11/18 22:51:20  kans
* ValidStructClear saves/restores new alignFindRemoteBsp and doSeqHistAssembly fields
*
* Revision 6.406  2003/11/14 18:07:16  kans
* alignment parameters to find remote bsp, do seqhist assembly
*
* Revision 6.405  2003/11/13 21:51:42  kans
* added ERR_SEQ_INST_TpaAssmeblyProblem check
*
* Revision 6.404  2003/11/12 20:13:36  kans
* added East Timor to the legal country codes
*
* Revision 6.403  2003/10/24 21:31:00  kans
* added test for ERR_SEQ_FEAT_UTRdoesNotAbutCDS on mRNA
*
* Revision 6.402  2003/10/24 17:50:35  kans
* added ERR_SEQ_INST_SeqLitGapLength0
*
* Revision 6.401  2003/10/24 04:41:50  kans
* ValidateImpFeat warns if repeat_region /rpt_unit has same length as sfp->location but does not have matching sequence
*
* Revision 6.400  2003/10/23 20:29:38  kans
* warn about allele gbqual when inheriting allele from gene
*
* Revision 6.399  2003/10/20 19:44:47  kans
* added * Terminator codon
*
* Revision 6.398  2003/10/20 16:53:05  kans
* suppress validator warning for synthetic sequences with molinfo other genetic when origin is artificial
*
* Revision 6.397  2003/10/17 21:12:27  kans
* added ERR_SEQ_FEAT_OnlyGeneXrefs test
*
* Revision 6.396  2003/10/10 22:38:39  kans
* added tests for BadTrnaCodon and BadTrnaAA
*
* Revision 6.395  2003/10/06 16:19:26  kans
* commented out check on rpt_unit content - now any text will be allowed
*
* Revision 6.394  2003/10/01 19:46:39  kans
* suppress partial not at end warning for CDD region
*
* Revision 6.393  2003/09/30 20:35:30  kans
* fixed IsSynthetic to look at div if origin was not set
*
* Revision 6.392  2003/09/23 12:33:37  kans
* Check DeltaLitOnly and allow test for terminal Ns
*
* Revision 6.391  2003/09/18 18:28:53  kans
* fixed IsMicroRNA - was using continue statements in while loop instead of for loop
*
* Revision 6.390  2003/09/11 15:24:35  kans
* duplicate feat severity warning check was only done for cds, not mrna
*
* Revision 6.389  2003/09/10 14:29:13  kans
* IsMicroRNA feature test for molinfo-biomol.other, do mrnatrans base comparison even if polyA test fails
*
* Revision 6.388  2003/09/09 20:09:21  kans
* lower severity for far product partial inconsistency and mrnatranscheck, also check for 95% polyA
*
* Revision 6.387  2003/08/13 21:45:30  kans
*  added ERR_SEQ_DESCR_RefGeneTrackingWithoutStatus
*
* Revision 6.386  2003/08/11 15:08:08  kans
* REBASE is legal refseq dbxref
*
* Revision 6.385  2003/08/01 21:33:38  kans
* ERR_SEQ_INST_InternalNsInSeqLit dropped to warning, cutoff still at 80
*
* Revision 6.384  2003/08/01 21:30:28  kans
* added CountAdjacentNsInSeqLit for htgs 1 and 2
*
* Revision 6.383  2003/07/30 21:44:31  kans
* comment out archaic locations messages because TMSMART thinks it should not promote locations
*
* Revision 6.382  2003/07/29 15:59:11  kans
* use new ERR_SEQ_PKG_ArchaicFeatureLocation and ERR_SEQ_PKG_ArchaicFeatureProduct tokens, also do not complain if location or product is local and Bioseq has TMSMART or BankIt general
*
* Revision 6.381  2003/07/28 22:11:04  kans
* check for archaic feature locations and products
*
* Revision 6.380  2003/07/22 16:18:07  kans
* added Kerguelen Archipelago to country list
*
* Revision 6.379  2003/07/15 16:46:02  kans
* suppress BadDeltaSeq - HTGS 2 delta seq has no gaps and no graphs - if HTGS_ACTIVEFIN keyword present
*
* Revision 6.378  2003/07/07 15:35:01  kans
* ERR_SEQ_INST_TerminalNs is SEV_ERROR if 10 or more Ns at either end
*
* Revision 6.377  2003/07/02 19:36:47  kans
* added CheckCDSPartial to check cds->location partials against product molinfo
*
* Revision 6.376  2003/06/17 21:15:46  kans
* germline and rearranged are mutually exclusive - proviral and virion are because there is only one biop->genome
*
* Revision 6.375  2003/06/17 21:05:13  kans
* synthetic biosource should have molinfo biomol other
*
* Revision 6.374  2003/06/17 20:03:38  kans
* NT-036298 (dash instead of underscore) gives REJECT level BadSeqIdFormat error
*
* Revision 6.373  2003/06/02 21:42:11  kans
* allow 4 + 2 + 7 wgs master accessions
*
* Revision 6.372  2003/05/09 18:46:47  kans
* severity of first and last delta seq component is gap message lowered if not HTGS
*
* Revision 6.371  2003/05/02 19:19:28  kans
* added rearrangement exception to list that suppressed CdTransCheck
*
* Revision 6.370  2003/05/01 20:08:57  kans
* Serbia and Montenegro restored to list of countries, but Yugoslavia also remains
*
* Revision 6.369  2003/04/30 16:38:37  kans
* added CdsProductIdCheck
*
* Revision 6.368  2003/04/27 20:16:23  kans
* ribosomal slippage exception suppresses CDSmRNArange warning
*
* Revision 6.367  2003/04/24 19:51:47  kans
* rearrangement required okay for all records, not just refseq, at least for now
*
* Revision 6.366  2003/04/24 19:19:12  kans
* added support for new rearrangement required for product exception
*
* Revision 6.365  2003/04/23 16:56:16  kans
* ERR_GENERIC_BadPageNumbering dropped to SEV_WARNING
*
* Revision 6.364  2003/04/21 16:39:22  kans
* CheckRnaProductType was doing the wrong thing for rRNAs
*
* Revision 6.363  2003/04/10 15:45:30  kans
* LongLiteralSequence now ERROR level, fixed message to correctly indicate 500kb limit
*
* Revision 6.362  2003/04/09 20:16:29  kans
* gene feature is inappropriate on protein bioseq
*
* Revision 6.361  2003/04/08 21:03:57  kans
* UnnecessaryBioSourceFocus not done on proteins, segmented bioseqs, or segmented parts
*
* Revision 6.360  2003/04/07 16:16:46  kans
* UnnecessaryBioSourceFocus only for nucleotides
*
* Revision 6.359  2003/03/26 15:52:58  kans
* warn on molinfo.biomol = other-genetic, correct spelling of Nuclic
*
* Revision 6.358  2003/03/24 13:57:59  kans
* inadvertantly swapped MultipleBioSources and UnnecessaryBioSourceFocus error tags
*
* Revision 6.357  2003/03/12 22:59:08  kans
* duplicate feature at sev error if flybase dbxrefs
*
* Revision 6.356  2003/03/11 22:30:37  kans
* implemented ERR_SEQ_DESCR_UnnecessaryBioSourceFocus
*
* Revision 6.355  2003/03/06 20:02:55  kans
* new ERR_SEQ_FEAT_BadProductSeqId protein name code needed to loop on protBsp->id, since tsip might not be first id
*
* Revision 6.354  2003/03/06 19:48:30  kans
* do not dereference protBsp->id if protBsp is NULL - far product
*
* Revision 6.353  2003/03/05 18:47:16  ford
* Made IsNuclAcc() EXTERN.
*
* Revision 6.352  2003/03/04 17:18:07  ford
* Removed calls to ValidateAccn () and reverted back to simpler customized functions.
*
* Revision 6.351  2003/03/04 16:16:45  ford
* Used ValidateAccn() in ValidateSeqFeat() when checking for incorrect usage of the seqid name slot (BadProductSeqId error message).
*
* Revision 6.350  2003/03/03 22:37:35  ford
* Modified BadProductSeqId error message in ValidateSeqFeat() so that it only generates a warning when a new submission comes in with this problem.
*
* Revision 6.349  2003/02/20 17:19:29  kans
* separate ValidateIDSetAgainstDb function, since more development expected
*
* Revision 6.348  2003/02/20 14:17:45  kans
* fix in IsNoncuratedRefSeq
*
* Revision 6.347  2003/02/19 19:39:36  kans
* fixes to validateIDSet code
*
* Revision 6.346  2003/02/18 20:19:28  kans
* added vsp->validateIDSet, initial work on validating update against ID set in database
*
* Revision 6.345  2003/02/14 16:34:51  kans
* lowered rare GC splice donor to info
*
* Revision 6.344  2003/02/11 19:01:28  kans
* population study check for inconsistent organisms drops severity if first mismatch is same up to sp.
*
* Revision 6.343  2003/02/11 15:31:56  kans
* ddbj segmented sequence reports missing accession as warning, not reject
*
* Revision 6.342  2003/01/31 18:55:50  kans
* implemeneted ERR_SEQ_INST_TerminalNs
*
* Revision 6.341  2003/01/31 16:54:02  kans
* SameAsCDS checks partial gene or mRNA for CDS with proper intervals, drops severity to INFO
*
* Revision 6.340  2003/01/03 19:48:22  kans
* BadSeqIdFormat REJECT for nucleotide hasGi and tsip->name with no tsip->accession
*
* Revision 6.339  2002/12/27 19:42:51  kans
* warn on protein_id or transcript_id as gbqual on CDS or mRNA feature
*
* Revision 6.338  2002/12/23 15:38:06  kans
* do not complain about protein TPA not having Seq-hist.assembly, use same IsTpa function as asn2gnbk in deciding whether to complain about Seq-id type
*
* Revision 6.337  2002/12/18 19:09:05  kans
* added ERR_SEQ_INST_HistAssemblyMissing
*
* Revision 6.336  2002/12/17 18:33:02  kans
* suppress GiWithoutAccession if Seq_repr_virtual, comment out WGS > 350 KB message
*
* Revision 6.335  2002/12/17 18:21:03  kans
* use NextLitLength to handle catenated Seq-literal with sequence data for lining up to Graphs
*
* Revision 6.334  2002/11/27 20:56:35  kans
* empty pir/swissprot/prf seqid block gives reject error
*
* Revision 6.333  2002/11/25 22:06:52  kans
* added ERR_SEQ_FEAT_RnaProductMismatch in CheckRnaProductType
*
* Revision 6.332  2002/11/15 18:20:44  kans
* implemented ERR_SEQ_FEAT_BadProductSeqId test
*
* Revision 6.331  2002/11/08 19:23:33  kans
* Multi-interval CDS on mRNA Bioseq gives InvalidForType, currently WARNING on RefSeq, ERROR on everything else, awaiting final severity decision
*
* Revision 6.330  2002/11/05 15:41:59  kans
* changed SEV_FATAL to SEV_REJECT in BadSeqIdFormat 0 version check
*
* Revision 6.329  2002/11/01 22:33:51  kans
* if GenBank/EMBL/DDBJ or TPA record has GI but no version, give FATAL BadSeqIdFormat error
*
* Revision 6.328  2002/11/01 20:43:51  kans
* do not report StructuredSourceNote if previous character is note whitespace and not semicolon
*
* Revision 6.327  2002/11/01 17:47:52  kans
* DescrSame was not doing null checks properly
*
* Revision 6.326  2002/10/31 22:42:38  kans
* consortium is valid author
*
* Revision 6.325  2002/10/30 04:12:54  kans
* new refseq badseqidformat test should only be done for seqid_other
*
* Revision 6.324  2002/10/29 20:36:17  kans
* validate refseq accession, including new NZ_ wgs type
*
* Revision 6.323  2002/10/28 19:30:33  kans
* added farFetchCDSproducts to vsp
*
* Revision 6.322  2002/10/22 14:15:32  kans
* suppress printing of feature indexing messages
*
* Revision 6.321  2002/10/22 03:16:58  kans
* do not report CDSproductPackagingProblem if GPS regardless of ID type
*
* Revision 6.320  2002/09/30 16:31:36  kans
* support for new BioseqseqSet_class_wgs_set
*
* Revision 6.319  2002/09/19 22:12:23  kans
* BadDeltaSeq for htgs phase 2 with no gaps and no graphs
*
* Revision 6.318  2002/08/23 20:03:19  kans
* dup seg ref message does not have newline at end
*
* Revision 6.317  2002/08/15 22:37:58  kans
* gi without accession message suppressed if isPDB
*
* Revision 6.316  2002/08/08 13:35:33  kans
* raised limit of non-htg delta literal to 500K
*
* Revision 6.315  2002/08/02 19:43:20  kans
* report identical features in different named feature tables if the names are the same
*
* Revision 6.314  2002/07/26 19:45:14  kans
* added taxon to structured modifiers in note check, removed space after colons
*
* Revision 6.313  2002/07/16 17:13:08  kans
* added sourceQualTags to vsp, ERR_SEQ_DESCR_StructuredSourceNote by finite state machine text search
*
* Revision 6.312  2002/07/12 17:34:35  kans
* WormBase is now legal dbxref for all records, not just RefSeq
*
* Revision 6.311  2002/07/10 16:28:45  kans
* warn on obsolete biosource location or subsource - transposon and insertion sequence
*
* Revision 6.310  2002/07/08 21:13:03  kans
* added ERR_SEQ_INST_GiWithoutAccession and ERR_SEQ_INST_MultipleAccessions
*
* Revision 6.309  2002/07/08 18:28:55  kans
* BadDeltaSeq does not complain if technique 0 or 1
*
* Revision 6.308  2002/07/08 15:26:17  kans
* bad page numbering does not complain if page starts with a letter, and all RefSeqs drop to warning severity
*
* Revision 6.307  2002/06/26 13:21:40  kans
* added ERR_GENERIC_BadPageNumbering
*
* Revision 6.306  2002/06/20 20:38:00  kans
* conflict feature in RefSeq does not require sfp->cit if it has sfp->comment
*
* Revision 6.305  2002/06/19 20:08:52  kans
* added ERR_SEQ_PKG_InconsistentMolInfoBiomols
*
* Revision 6.304  2002/06/19 18:44:01  kans
* look for inconsistent molinfos within segset
*
* Revision 6.303  2002/06/18 21:06:04  kans
* pub features with same intervals but different labels are now suppressed
*
* Revision 6.302  2002/06/12 13:44:03  kans
* warn if htgs 1 or 2 delta has no gaps
*
* Revision 6.301  2002/06/12 13:18:05  kans
* warn on adjacent gaps in delta seqs
*
* Revision 6.300  2002/06/11 21:18:33  kans
* report gaps at beginning and end of delta seq
*
* Revision 6.299  2002/06/11 19:33:21  kans
* orgmod variety should only be in PLN division - plants and fungi
*
* Revision 6.298  2002/06/11 14:41:20  kans
* added support for locus_tag
*
* Revision 6.297  2002/06/11 13:17:01  kans
* added missing break before seqid_general case in ValidateBioseqInst
*
* Revision 6.296  2002/06/10 19:35:36  kans
* BioSourceNeedsFocus satisfied with either focus or transgenic
*
* Revision 6.295  2002/06/10 18:34:50  kans
* BankIt ID enables TPA comment, PRIMARY block, does not give warning of non-TPA ID type
*
* Revision 6.294  2002/06/07 19:55:39  kans
* missing accession suppressed for segmentd bioseq unless has gi
*
* Revision 6.293  2002/06/07 17:40:39  kans
* LookForNC to drop missing journal info to warning
*
* Revision 6.292  2002/06/07 15:17:12  kans
* wgs error only on curated refseqs
*
* Revision 6.291  2002/06/06 20:00:47  kans
* warn if TpaAssembly user object used for non-TPA record
*
* Revision 6.290  2002/06/05 18:54:03  kans
* addition to ERR_SEQ_INST_BadSeqIdFormat
*
* Revision 6.289  2002/06/04 20:29:16  kans
* IsNoncuratedRefSeq allows NM, NP, NG, NR to have SEV_WARNING if no pubs anywhere
*
* Revision 6.288  2002/05/13 19:39:39  kans
* do not complain about wgs tech without wgs accession if no collaborator or refseq id
*
* Revision 6.287  2002/05/13 18:20:49  kans
* primer_bind should not set unmarked_strand
*
* Revision 6.286  2002/05/09 17:32:27  kans
* fixed ValidateSeqFeat partial CDS location messages
*
* Revision 6.285  2002/05/07 15:16:53  kans
* NW_ also suppresses NoProtein error
*
* Revision 6.284  2002/05/06 21:30:36  kans
* validate wgs accession type against mip->tech wgs
*
* Revision 6.283  2002/04/30 21:20:46  kans
* always report missing molinfo except in patents
*
* Revision 6.282  2002/04/30 18:06:31  kans
* unmarked_strand is mixed_strand plus/unknown, uses SEV_WARNING
*
* Revision 6.281  2002/04/24 20:28:51  kans
* validate exception text uses GPSorRefSeq to allow RefSeq specific messages
*
* Revision 6.280  2002/04/23 20:08:35  kans
* added ValidateBioseqHist
*
* Revision 6.279  2002/04/22 14:10:58  kans
* validate qualifier value uses SEV_WARNING if NC_ or NT_
*
* Revision 6.278  2002/04/17 21:26:02  kans
* loc on segmented bioseq not if far seg
*
* Revision 6.277  2002/04/09 20:10:08  kans
* suppress BadDeltaSeq if genomic product set
*
* Revision 6.276  2002/04/02 18:19:55  kans
* SeqLocPartialCheck fixes
*
* Revision 6.275  2002/04/02 18:15:11  kans
* NW_ in GPS suppresses CDSproductPackagingProblem
*
* Revision 6.274  2002/04/01 21:02:47  kans
* nonconsensus splice site, and hyphenated variant, added to list of legal exception strings
*
* Revision 6.273  2002/04/01 20:55:06  kans
* non-consensus splice site exception suppresses splice check
*
* Revision 6.272  2002/03/25 21:42:47  ford
* Added consenses splice site warning msg for CDS partials.
*
* Revision 6.271  2002/03/15 20:01:10  kans
* check for classified partial location and unclassified partial product
*
* Revision 6.270  2002/03/12 19:01:25  kans
* added alwaysRequireIsoJTA flag
*
* Revision 6.269  2002/03/06 23:08:09  kans
* check for iso_jta if inPress
*
* Revision 6.268  2002/03/05 21:06:25  kans
* refseq_exception_strings separate list for unclassified transcription/translation discrepancies
*
* Revision 6.267  2002/03/05 19:27:45  kans
* added bypass_mrna_trans_check, ribosomal slippage inhibits mrna splice check
*
* Revision 6.266  2002/03/05 19:06:21  kans
* bypass_cds_trans_check gives exception strings that exit CdTransCheck
*
* Revision 6.265  2002/03/05 16:26:06  kans
* added unclassified transcription and translation discrepancies
*
* Revision 6.264  2002/03/05 14:42:54  kans
* allow multiple clauses in sfp->except_text
*
* Revision 6.263  2002/03/05 13:56:58  kans
* ribosome slippage had been accidentally removed
*
* Revision 6.262  2002/02/28 15:32:40  kans
* validate cons_splice, sfp->except_text against lists of legal string values
*
* Revision 6.261  2002/02/25 20:15:01  kans
* special case drosophila whether or not duplicate features have same label
*
* Revision 6.260  2002/02/20 20:47:55  kans
* added support for MI_TECH_wgs
*
* Revision 6.259  2002/02/19 14:57:11  kans
* trim extremely long printed seqloc to avoid overflowing validerr buffer
*
* Revision 6.258  2002/02/14 19:17:31  kans
* calls SeqMgrGetPROTgivenProduct if SeqMgrGetCDSgivenProduct failed, now that we are preparing to instantiate processed protein bioseqs
*
* Revision 6.257  2002/02/14 17:47:33  kans
* added ERR_SEQ_FEAT_FeatContentDup, distinct from DuplicateFeat
*
* Revision 6.256  2002/02/13 20:32:44  kans
* CheckMultiGeneInterval does not complain if Bioseq is circular
*
* Revision 6.255  2002/02/13 19:03:37  kans
* drop no splice on any refseq to warning
*
* Revision 6.254  2002/02/13 18:58:26  kans
* lower not splice to warning if NG_
*
* Revision 6.253  2002/02/07 15:03:48  kans
* AbuttingIntervals to SEV_WARNING if sfp->excpt set
*
* Revision 6.252  2002/02/07 14:34:52  kans
* added LOCATION_SUBSET type, use in CDS-mRNA overlap check to subclassify problem
*
* Revision 6.251  2002/01/30 18:42:42  kans
* if gpsntornc but Drosophila melanogaster, dup feat is warning
*
* Revision 6.250  2002/01/28 20:28:32  kans
* added GenBank, EMBL, and DDBJ as (temporarily) legal RefSeq db_xrefs
*
* Revision 6.249  2002/01/22 18:49:58  kans
* allow 4 letter + 8 digit accession for GENBANK/EMBL/DDBJ
*
* Revision 6.248  2002/01/18 15:58:01  kans
* allow Online Publication in cgp->cit
*
* Revision 6.247  2002/01/17 17:19:37  kans
* GenCodeMismatch specific message for plastid using code 11
*
* Revision 6.246  2002/01/14 20:29:36  kans
* dropped ISO jta error to warning
*
* Revision 6.245  2002/01/14 20:27:38  kans
* check cap->cjp for missing iso-jta is muid or pmuid
*
* Revision 6.244  2002/01/14 16:48:40  kans
* Published Only in DataBase is valid cgp->cit string
*
* Revision 6.243  2002/01/08 20:54:50  kans
* BadDelta suppressed if NT_ or NC_
*
* Revision 6.242  2001/12/21 18:48:06  kans
* ERR_SEQ_FEAT_MultiIntervalGene is SEV_WARNING for NC_ records
*
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
#include <asn2gnbi.h>
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
static void     ValidateBioseqHist (GatherContextPtr gcp);
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
static void     CdConflictCheck (ValidStructPtr vsp, SeqFeatPtr sfp);
static void     SpliceCheckEx (ValidStructPtr vsp, SeqFeatPtr sfp, Boolean checkAll);
static void     CdsProductIdCheck (ValidStructPtr vsp, SeqFeatPtr sfp);
static void     ValidateBioSource (ValidStructPtr vsp, GatherContextPtr gcp, BioSourcePtr biop, SeqFeatPtr sfp);
static void     ValidatePubdesc (ValidStructPtr vsp, GatherContextPtr gcp, PubdescPtr pdp);
static void     LookForMultiplePubs (ValidStructPtr vsp, GatherContextPtr gcp, SeqDescrPtr sdp);
static void     ValidateSfpCit (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp);

/* alignment validator */
NLM_EXTERN Boolean ValidateSeqAlignWithinValidator (ValidStructPtr vsp, SeqEntryPtr sep, Boolean find_remote_bsp, Boolean do_hist_assembly);

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
  Boolean         alignFindRemoteBsp;
  Boolean         doSeqHistAssembly;
  Boolean         alwaysRequireIsoJTA;
  Boolean         farFetchCDSproducts;
  Boolean         farFetchMRNAproducts;
  Boolean         locusTagGeneralMatch;
  Boolean         validateIDSet;
  Boolean         seqSubmitParent;
  Boolean         justShowAccession;
  Int2            validationLimit;
  TextFsaPtr      sourceQualTags;
  ValidErrorFunc  errfunc;
  Pointer         userdata;
  Boolean         convertGiToAccn;

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
  alignFindRemoteBsp = vsp->alignFindRemoteBsp;
  doSeqHistAssembly = vsp->doSeqHistAssembly;
  alwaysRequireIsoJTA = vsp->alwaysRequireIsoJTA;
  farFetchCDSproducts = vsp->farFetchCDSproducts;
  farFetchMRNAproducts = vsp->farFetchMRNAproducts;
  locusTagGeneralMatch = vsp->locusTagGeneralMatch;
  validateIDSet = vsp->validateIDSet;
  seqSubmitParent = vsp->seqSubmitParent;
  justShowAccession = vsp->justShowAccession;
  validationLimit = vsp->validationLimit;
  sourceQualTags = vsp->sourceQualTags;
  errfunc = vsp->errfunc;
  userdata = vsp->userdata;
  convertGiToAccn = vsp->convertGiToAccn;
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
  vsp->alignFindRemoteBsp = alignFindRemoteBsp;
  vsp->doSeqHistAssembly = doSeqHistAssembly;
  vsp->alwaysRequireIsoJTA = alwaysRequireIsoJTA;
  vsp->farFetchCDSproducts = farFetchCDSproducts;
  vsp->farFetchMRNAproducts = farFetchMRNAproducts;
  vsp->locusTagGeneralMatch = locusTagGeneralMatch;
  vsp->validateIDSet = validateIDSet;
  vsp->seqSubmitParent = seqSubmitParent;
  vsp->justShowAccession = justShowAccession;
  vsp->validationLimit = validationLimit;
  vsp->sourceQualTags = sourceQualTags;
  vsp->errfunc = errfunc;
  vsp->userdata = userdata;
  vsp->convertGiToAccn = convertGiToAccn;
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
  TextFsaFree (vsp->sourceQualTags);
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
  case SEQID_GPIPE:
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
  Char            label[60];
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

static CharPtr categoryLabel [] = {
  NULL, "SEQ_INST", "SEQ_DESCR", "GENERIC", "SEQ_PKG", "SEQ_FEAT", "SEQ_ALIGN", "SEQ_GRAPH"
};

NLM_EXTERN CharPtr GetValidCategoryName (int errcode)

{
  if (errcode >= 1 && errcode < sizeof (categoryLabel)) return categoryLabel [errcode];
  return NULL;
}

static CharPtr err1Label [] = {
  NULL,
  "ExtNotAllowed",
  "ExtBadOrMissing",
  "SeqDataNotFound",
  "SeqDataNotAllowed",
  "ReprInvalid",
  "CircularProtein",
  "DSProtein",
  "MolNotSet",
  "MolOther",
  "FuzzyLen",
  "InvalidLen",
  "InvalidAlphabet",
  "SeqDataLenWrong",
  "SeqPortFail",
  "InvalidResidue",
  "StopInProtein",
  "PartialInconsistent",
  "ShortSeq",
  "NoIdOnBioseq",
  "BadDeltaSeq",
  "LongHtgsSequence",
  "LongLiteralSequence",
  "SequenceExceeds350kbp",
  "ConflictingIdsOnBioseq",
  "MolNuclAcid",
  "ConflictingBiomolTech",
  "SeqIdNameHasSpace",
  "IdOnMultipleBioseqs",
  "DuplicateSegmentReferences",
  "TrailingX",
  "BadSeqIdFormat",
  "PartsOutOfOrder",
  "BadSecondaryAccn",
  "ZeroGiNumber",
  "RnaDnaConflict",
  "HistoryGiCollision",
  "GiWithoutAccession",
  "MultipleAccessions",
  "HistAssemblyMissing",
  "TerminalNs",
  "UnexpectedIdentifierChange",
  "InternalNsInSeqLit",
  "SeqLitGapLength0",
  "TpaAssmeblyProblem",
  "SeqLocLength",
  "MissingGaps",
  "CompleteTitleProblem",
  "CompleteCircleProblem",
  "BadHTGSeq",
  "GapInProtein",
  "BadProteinStart"
};

static CharPtr err2Label [] = {
  NULL,
  "BioSourceMissing",
  "InvalidForType",
  "FileOpenCollision",
  "Unknown",
  "NoPubFound",
  "NoOrgFound",
  "MultipleBioSources",
  "NoMolInfoFound",
  "BadCountryCode",
  "NoTaxonID",
  "InconsistentBioSources",
  "MissingLineage",
  "SerialInComment",
  "BioSourceNeedsFocus",
  "BadOrganelle",
  "MultipleChromosomes",
  "BadSubSource",
  "BadOrgMod",
  "InconsistentProteinTitle",
  "Inconsistent",
  "ObsoleteSourceLocation",
  "ObsoleteSourceQual",
  "StructuredSourceNote",
  "UnnecessaryBioSourceFocus",
  "RefGeneTrackingWithoutStatus",
  "UnwantedCompleteFlag",
  "CollidingPublications",
  "TransgenicProblem",
  "TaxonomyLookupProblem",
  "MultipleTitles",
  "RefGeneTrackingOnNonRefSeq",
  "BioSourceInconsistency"
};

static CharPtr err3Label [] = {
  NULL,
  "NonAsciiAsn",
  "Spell",
  "AuthorListHasEtAl",
  "MissingPubInfo",
  "UnnecessaryPubEquiv",
  "BadPageNumbering",
  "MedlineEntryPub"
};

static CharPtr err4Label [] = {
  NULL,
  "NoCdRegionPtr",
  "NucProtProblem",
  "SegSetProblem",
  "EmptySet",
  "NucProtNotSegSet",
  "SegSetNotParts",
  "SegSetMixedBioseqs",
  "PartsSetMixedBioseqs",
  "PartsSetHasSets",
  "FeaturePackagingProblem",
  "GenomicProductPackagingProblem",
  "InconsistentMolInfoBiomols",
  "ArchaicFeatureLocation",
  "ArchaicFeatureProduct",
  "GraphPackagingProblem"
};

static CharPtr err5Label [] = {
  NULL,
  "InvalidForType",
  "PartialProblem",
  "InvalidType",
  "Range",
  "MixedStrand",
  "SeqLocOrder",
  "CdTransFail",
  "StartCodon",
  "InternalStop",
  "NoProtein",
  "MisMatchAA",
  "TransLen",
  "NoStop",
  "TranslExcept",
  "NoProtRefFound",
  "NotSpliceConsensus",
  "OrfCdsHasProduct",
  "GeneRefHasNoData",
  "ExceptInconsistent",
  "ProtRefHasNoData",
  "GenCodeMismatch",
  "RNAtype0",
  "UnknownImpFeatKey",
  "UnknownImpFeatQual",
  "WrongQualOnImpFeat",
  "MissingQualOnImpFeat",
  "PseudoCdsHasProduct",
  "IllegalDbXref",
  "FarLocation",
  "DuplicateFeat",
  "UnnecessaryGeneXref",
  "TranslExceptPhase",
  "TrnaCodonWrong",
  "BothStrands",
  "CDSgeneRange",
  "CDSmRNArange",
  "OverlappingPeptideFeat",
  "SerialInComment",
  "MultipleCDSproducts",
  "FocusOnBioSourceFeature",
  "PeptideFeatOutOfFrame",
  "InvalidQualifierValue",
  "MultipleMRNAproducts",
  "mRNAgeneRange",
  "TranscriptLen",
  "TranscriptMismatches",
  "CDSproductPackagingProblem",
  "DuplicateInterval",
  "PolyAsiteNotPoint",
  "ImpFeatBadLoc",
  "LocOnSegmentedBioseq",
  "UnnecessaryCitPubEquiv",
  "ImpCDShasTranslation",
  "ImpCDSnotPseudo",
  "MissingMRNAproduct",
  "AbuttingIntervals",
  "CollidingGeneNames",
  "MultiIntervalGene",
  "FeatContentDup",
  "BadProductSeqId",
  "RnaProductMismatch",
  "MissingCDSproduct",
  "BadTrnaCodon",
  "BadTrnaAA",
  "OnlyGeneXrefs",
  "UTRdoesNotAbutCDS",
  "BadConflictFlag",
  "ConflictFlagSet",
  "LocusTagProblem",
  "CollidingLocusTags",
  "AltStartCodon",
  "PartialsInconsistent",
  "GenesInconsistent",
  "DuplicateTranslExcept",
  "TranslExceptAndRnaEditing",
  "NoNameForProtein",
  "TaxonDbxrefOnFeature",
  "UnindexedFeature",
  "CDSmRNAmismatch",
  "UnnecessaryException",
  "LocusTagProductMismatch",
  "MrnaTransFail",
  "PseudoCdsViaGeneHasProduct",
  "MissingGeneXref",
  "FeatureCitationProblem",
  "NestedSeqLocMix",
  "WrongQualOnFeature",
  "MissingQualOnFeature",
  "CodonQualifierUsed",
  "UnknownFeatureQual",
  "BadCharInAuthorName",
  "PolyATail",
  "ProteinNameEndsInBracket",
  "CDSwithMultipleMRNAs"
};

static CharPtr err6Label [] = {
  NULL,
  "SeqIdProblem",
  "StrandRev",
  "DensegLenStart",
  "StartLessthanZero",
  "StartMorethanBiolen",
  "EndLessthanZero",
  "EndMorethanBiolen",
  "LenLessthanZero",
  "LenMorethanBiolen",
  "SumLenStart",
  "AlignDimSeqIdNotMatch",
  "SegsDimSeqIdNotMatch",
  "FastaLike",
  "NullSegs",
  "SegmentGap",
  "SegsDimOne",
  "AlignDimOne",
  "Segtype",
  "BlastAligns"
};

static CharPtr err7Label [] = {
  NULL,
  "GraphMin",
  "GraphMax",
  "GraphBelow",
  "GraphAbove",
  "GraphByteLen",
  "GraphOutOfOrder",
  "GraphBioseqLen",
  "GraphSeqLitLen",
  "GraphSeqLocLen",
  "GraphStartPhase",
  "GraphStopPhase",
  "GraphDiffNumber",
  "GraphACGTScore",
  "GraphNScore",
  "GraphGapScore",
  "GraphOverlap"
};

NLM_EXTERN CharPtr GetValidErrorName (int errcode, int subcode)

{
  if (errcode < 1 || errcode >= sizeof (categoryLabel)) return NULL;
  switch (errcode) {
    case 1 :
      if (subcode >= 1 && subcode < sizeof (err1Label)) return err1Label [subcode];
      break;
    case 2 :
      if (subcode >= 1 && subcode < sizeof (err2Label)) return err2Label [subcode];
      break;
    case 3 :
      if (subcode >= 1 && subcode < sizeof (err3Label)) return err3Label [subcode];
      break;
    case 4 :
      if (subcode >= 1 && subcode < sizeof (err4Label)) return err4Label [subcode];
      break;
    case 5 :
      if (subcode >= 1 && subcode < sizeof (err5Label)) return err5Label [subcode];
      break;
    case 6 :
      if (subcode >= 1 && subcode < sizeof (err6Label)) return err6Label [subcode];
      break;
    case 7 :
      if (subcode >= 1 && subcode < sizeof (err7Label)) return err7Label [subcode];
      break;
    default :
      break;
  }
  return NULL;
}

static void CustValErr (ValidStructPtr vsp, ErrSev severity, int errcode, int subcode)

{
  CharPtr         accession = NULL, context = NULL, label = NULL, location = NULL,
                  message = NULL, objtype = NULL, product = NULL;
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  Int2            buflen, diff;
  CharPtr         ctmp, tmp;
  ValidErrorFunc  errfunc;
  Char            id [64];
  ObjValNodePtr   ovp;
  SeqDescrPtr     sdp;
  SeqEntryPtr     sep;
  SeqFeatPtr      sfp;
  SeqIdPtr        sip;
  SeqLocPtr       slp;

  if (vsp == NULL) return;
  errfunc = vsp->errfunc;
  if (errfunc == NULL) return;

  if (severity < SEV_NONE || severity > SEV_MAX) {
    severity = SEV_MAX;
  }

  sip = NULL;
  if (vsp->sfp != NULL) {
	sfp = vsp->sfp;
	bsp = BioseqFindFromSeqLoc (sfp->location);
	if (bsp != NULL) {
	  sip = SeqIdFindWorst (bsp->id);
	}
  } else if (vsp->descr != NULL) {
	sdp = vsp->descr;
	if (sdp != NULL && sdp->extended != 0) {
	  ovp = (ObjValNodePtr) sdp;
	  if (ovp->idx.parenttype == OBJ_BIOSEQ) {
		bsp = (BioseqPtr) ovp->idx.parentptr;
		if (bsp != NULL) {
		  sip = SeqIdFindWorst (bsp->id);
		}
	  } else if (ovp->idx.parenttype == OBJ_BIOSEQSET) {
		bssp = (BioseqSetPtr) ovp->idx.parentptr;
		if (bssp != NULL) {
		  sep = bssp->seqentry;
		  if (sep != NULL) {
			sep = FindNthBioseq (sep, 1);
			if (sep != NULL) {
			  bsp = (BioseqPtr) sep->data.ptrvalue;
			  if (bsp != NULL) {
				sip = SeqIdFindWorst (bsp->id);
			  }
			}
		  }
		}
	  }
	}
  } else if (vsp->bsp != NULL) {
	bsp = vsp->bsp;
	sip = SeqIdFindWorst (bsp->id);
  } else if (vsp->bssp != NULL) {
	bssp = vsp->bssp;
	sep = bssp->seqentry;
	if (sep != NULL) {
	  sep = FindNthBioseq (sep, 1);
	  if (sep != NULL) {
		bsp = (BioseqPtr) sep->data.ptrvalue;
		if (bsp != NULL) {
		  sip = SeqIdFindWorst (bsp->id);
		}
	  }
	}
  }
  if (sip != NULL) {
	SeqIdWrite (sip, id, PRINTID_REPORT, sizeof (id) - 1);
	accession = id;
  }

  if (vsp->sfp != NULL) {
    objtype = "FEATURE";
  } else if (vsp->descr != NULL) {
    objtype = "DESCRIPTOR";
  } else if (vsp->bsp != NULL) {
    objtype = "BIOSEQ";
  } else if (vsp->bssp != NULL) {
    objtype = "BIOSEQ-SET";
  }

  message = vsp->errbuf;

  tmp = vsp->errbuf;
  buflen = 4000;
  while (*tmp != '\0') {
    buflen--;
    tmp++;
  }
  tmp++;
  *tmp = '\0';

  if (vsp->sfp != NULL) {
    label = tmp;
    diff = FeatDefLabel (vsp->sfp, tmp, buflen, OM_LABEL_BOTH);
    buflen -= diff;
    tmp += diff;
    *tmp = '\0';
    tmp++;
    *tmp = '\0';
  } else if (vsp->descr != NULL) {
    label = tmp;
    diff = SeqDescLabel (vsp->descr, tmp, buflen, OM_LABEL_BOTH);
    buflen -= diff;
    tmp += diff;
    *tmp = '\0';
    tmp++;
    *tmp = '\0';
  } else if (vsp->bsp != NULL) {
    label = tmp;
    if (vsp->convertGiToAccn) {
      diff = WorstBioseqLabel (vsp->bsp, tmp, buflen, OM_LABEL_CONTENT);
    } else {
      diff = BioseqLabel (vsp->bsp, tmp, buflen, OM_LABEL_BOTH);
    }
    buflen -= diff;
    tmp += diff;
    *tmp = '\0';
    tmp++;
    *tmp = '\0';
  } else if (vsp->bssp != NULL) {
    label = tmp;
    diff = BioseqSetLabel (vsp->bssp, tmp, buflen, OM_LABEL_BOTH);
    buflen -= diff;
    tmp += diff;
    *tmp = '\0';
    tmp++;
    *tmp = '\0';
  }

  if (vsp->sfp != NULL) {
    sfp = vsp->sfp;
  
    if (sfp->location != NULL) {
      ctmp = NULL;
      slp = NULL;
      if (vsp->convertGiToAccn) {
        slp = AsnIoMemCopy (sfp->location, (AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
        ChangeSeqLocToWorstID (slp);
        ctmp = SeqLocPrint (slp);
        SeqLocFree (slp);
      } else {
        ctmp = SeqLocPrint (sfp->location);
      }
      if (ctmp != NULL) {
        if (StringLen (ctmp) > 800) {
          StringCpy (ctmp + 797, "...");
        }
        location = tmp;
        diff = LabelCopyExtra (tmp, ctmp, buflen, "[", "]");
        buflen -= diff;
        tmp += diff;
        MemFree (ctmp);
        *tmp = '\0';
        tmp++;
        *tmp = '\0';

        sip = SeqLocId (sfp->location);
        if (sip != NULL) {
          bsp = BioseqFind (sip);
          if (bsp != NULL) {
            context = tmp;
            diff = LabelCopy (tmp, "[", buflen);
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
        *tmp = '\0';
        tmp++;
        *tmp = '\0';
      }
    }
  
    if (sfp->product != NULL) {
      ctmp = NULL;
      slp = NULL;
      if (vsp->convertGiToAccn) {
        slp = AsnIoMemCopy (sfp->product, (AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
        ChangeSeqLocToWorstID (slp);
        ctmp = SeqLocPrint (slp);
        SeqLocFree (slp);
      } else {
        ctmp = SeqLocPrint (sfp->product);
      }
      if (ctmp != NULL) {
        if (StringLen (ctmp) > 800) {
          StringCpy (ctmp + 797, "...");
        }
        product = tmp;
        diff = LabelCopyExtra (tmp, ctmp, buflen, "[", "]");
        buflen -= diff;
        tmp += diff;
        *tmp = '\0';
        tmp++;
        *tmp = '\0';
        MemFree (ctmp);
      }
    }
  } else if (vsp->descr != NULL) {
    if (vsp->bsp != NULL) {
      context = tmp;
      diff = LabelCopy (tmp, "BIOSEQ: ", buflen);
      buflen -= diff;
      tmp += diff;
      if (vsp->suppressContext || vsp->convertGiToAccn) {
        diff = WorstBioseqLabel (vsp->bsp, tmp, buflen, OM_LABEL_CONTENT);
      } else {
        diff = BioseqLabel (vsp->bsp, tmp, buflen, OM_LABEL_BOTH);
      }
      buflen -= diff;
      tmp += diff;
      *tmp = '\0';
      tmp++;
      *tmp = '\0';
    } else if (vsp->bssp != NULL) {
      context = tmp;
      diff = LabelCopy (tmp, "BIOSEQ-SET: ", buflen);
      buflen -= diff;
      tmp += diff;

      if (vsp->suppressContext || vsp->convertGiToAccn) {
        diff = BioseqSetLabel (vsp->bssp, tmp, buflen, OM_LABEL_CONTENT);
      } else {
        diff = BioseqSetLabel (vsp->bssp, tmp, buflen, OM_LABEL_BOTH);
      }
      buflen -= diff;
      tmp += diff;
      *tmp = '\0';
      tmp++;
      *tmp = '\0';
    }
  }

  (*errfunc) (severity, errcode, subcode, accession, message,
              objtype, label, context, location, product, vsp->userdata);
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
  va_list           args;
  BioseqPtr         bsp;
  BioseqSetPtr      bssp;
  Int2              buflen, diff;
  CharPtr           ctmp, tmp;
  GatherContextPtr  gcp;
  Char              id [64];
  SeqLocPtr         loc = NULL;
  ObjValNodePtr     ovp;
  SeqDescrPtr       sdp;
  SeqEntryPtr       sep;
  SeqFeatPtr        sfp;
  SeqIdPtr          sip;

  if (vsp == NULL || severity < vsp->cutoff)
    return;

  if (vsp->errbuf == NULL) {
    vsp->errbuf = MemNew (4096);
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

  if (vsp->errfunc != NULL) {
    CustValErr (vsp, (ErrSev) (severity), code1, code2);
    vsp->errbuf[0] = '\0';
    return;
  }

  if (vsp->justShowAccession) {
    vsp->errbuf[0] = '\0';
    tmp = vsp->errbuf;
    sip = NULL;

    if (vsp->sfp != NULL) {
      sfp = vsp->sfp;
      bsp = BioseqFindFromSeqLoc (sfp->location);
      if (bsp != NULL) {
        sip = SeqIdFindWorst (bsp->id);
      }
    } else if (vsp->descr != NULL) {
      sdp = vsp->descr;
      if (sdp != NULL && sdp->extended != 0) {
        ovp = (ObjValNodePtr) sdp;
        if (ovp->idx.parenttype == OBJ_BIOSEQ) {
          bsp = (BioseqPtr) ovp->idx.parentptr;
          if (bsp != NULL) {
            sip = SeqIdFindWorst (bsp->id);
          }
        } else if (ovp->idx.parenttype == OBJ_BIOSEQSET) {
          bssp = (BioseqSetPtr) ovp->idx.parentptr;
          if (bssp != NULL) {
            sep = bssp->seqentry;
            if (sep != NULL) {
              sep = FindNthBioseq (sep, 1);
              if (sep != NULL) {
                bsp = (BioseqPtr) sep->data.ptrvalue;
                if (bsp != NULL) {
                  sip = SeqIdFindWorst (bsp->id);
                }
              }
            }
          }
        }
      }
    } else if (vsp->bsp != NULL) {
      bsp = vsp->bsp;
      sip = SeqIdFindWorst (bsp->id);
    } else if (vsp->bssp != NULL) {
      bssp = vsp->bssp;
      sep = bssp->seqentry;
      if (sep != NULL) {
        sep = FindNthBioseq (sep, 1);
        if (sep != NULL) {
          bsp = (BioseqPtr) sep->data.ptrvalue;
          if (bsp != NULL) {
            sip = SeqIdFindWorst (bsp->id);
          }
        }
      }
    }

    if (sip != NULL) {
      SeqIdWrite (sip, id, PRINTID_REPORT, sizeof (id) - 1);
      diff = LabelCopy (tmp, id, buflen);
      buflen -= diff;
      tmp += diff;
    }

    ErrPostItem ((ErrSev) (severity), code1, code2, "%s", vsp->errbuf);
    vsp->errbuf[0] = '\0';
    return;
  }

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
    if (ctmp != NULL && StringLen (ctmp) > 800) {
      StringCpy (ctmp + 797, "...");
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
      if (ctmp != NULL && StringLen (ctmp) > 800) {
        StringCpy (ctmp + 797, "...");
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

      if (vsp->bsp == NULL) {
        diff = LabelCopy (tmp, "??", buflen);
      } else if (vsp->suppressContext) {
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
  ValidStructPtr     vsp;
  AnnotDescrPtr      desc;
  SeqAnnotPtr        sap;
  ObjectIdPtr        oip;
  Boolean            is_blast_align;
  Int2               limit;
  SeqFeatPtr         sfp;
  ValNodePtr         sdp;
  BioSourcePtr       biop;
  PubdescPtr         pdp;
  SeqMgrFeatContext  context;

  vsp = (ValidStructPtr) (gcp->userdata);
  vsp->gcp = gcp;               /* needed for ValidErr */

  limit = vsp->validationLimit;

  switch (gcp->thistype) {
  case OBJ_BIOSEQ:
    if (!vsp->onlyspell) {
      if (limit == VALIDATE_ALL || limit == VALIDATE_INST) {
        ValidateBioseqInst (gcp);
      }
      if (limit == VALIDATE_ALL || limit == VALIDATE_CONTEXT) {
        ValidateBioseqContext (gcp);
      }
      if (limit == VALIDATE_ALL || limit == VALIDATE_INST) {
        ValidateBioseqHist (gcp);
      }
      if (limit == VALIDATE_ALL || limit == VALIDATE_GRAPH) {
        ValidateGraphsOnBioseq (gcp);
      }
    }
    break;
  case OBJ_BIOSEQSET:
    if (!vsp->onlyspell) {
      if (limit == VALIDATE_ALL || limit == VALIDATE_SET) {
        ValidateBioseqSet (gcp);
      }
    }
    break;
  case OBJ_SEQANNOT:
    if (!vsp->onlyspell) {
      if (limit == VALIDATE_ALL) {
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
    }
    break;
  case OBJ_SEQFEAT:
    if (!vsp->onlyspell) {
      if (limit == VALIDATE_ALL || limit == VALIDATE_FEAT) {
        ValidateSeqFeat (gcp);
        sfp = (SeqFeatPtr) (gcp->thisitem);
        if (sfp != NULL) {
          if (sfp->data.choice == SEQFEAT_BIOSRC) {
            biop = (BioSourcePtr) sfp->data.value.ptrvalue;
            ValidateBioSource (vsp, gcp, biop, sfp);
          }
          if (sfp->data.choice == SEQFEAT_PUB) {
            pdp = (PubdescPtr) sfp->data.value.ptrvalue;
            ValidatePubdesc (vsp, gcp, pdp);
          }
          if (sfp->cit != NULL) {
            ValidateSfpCit (vsp, gcp, sfp);
          }
          if (vsp->useSeqMgrIndexes) {
            if (SeqMgrGetDesiredFeature (gcp->entityID, NULL, 0, 0, sfp, &context) == NULL) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_UnindexedFeature, "Feature is not indexed");
            }
          }
        }
      }
    }
    if (limit == VALIDATE_ALL || limit == VALIDATE_FEAT) {
      SpellCheckSeqFeat (gcp);
    }
    break;
  case OBJ_SEQDESC:
    if (limit == VALIDATE_ALL || limit == VALIDATE_DESC) {
      SpellCheckSeqDescr (gcp);
                          /**
			  ValidateSeqDescr (gcp);
		      **/
      sdp = (ValNodePtr) (gcp->thisitem);
      if (sdp != NULL) {
        if (sdp->choice == Seq_descr_source) {
          biop = (BioSourcePtr) sdp->data.ptrvalue;
          ValidateBioSource (vsp, gcp, biop, NULL);
        }
        if (sdp->choice == Seq_descr_pub) {
          pdp = (PubdescPtr) sdp->data.ptrvalue;
          ValidatePubdesc (vsp, gcp, pdp);
          LookForMultiplePubs (vsp, gcp, sdp);
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

typedef struct ftprob {
  Uint4  num_misplaced_features;
  Uint4  num_archaic_locations;
  Uint4  num_archaic_products;
  Uint4  num_misplaced_graphs;
  Uint4  num_gene_feats;
  Uint4  num_gene_xrefs;
  Uint4  num_tpa_with_hist;
  Uint4  num_tpa_without_hist;
} FeatProb, PNTR FeatProbPtr;

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
        /* generated gap feature is an exception */
        if (par == NULL || par->id != NULL) {
          (*num_misplaced_features)++;
        }
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
          if (parent->idx.parenttype != OBJ_BIOSEQSET) {
            (*num_misplaced_features)++;
            return;
          }
          parent = (BioseqSetPtr) parent->idx.parentptr;
        }
        (*num_misplaced_features)++;
      }
    }
  }
}

static Boolean IdIsArchaic (SeqIdPtr sip)

{
  BioseqPtr  bsp;
  DbtagPtr   dbt;
  SeqIdPtr   id;

  if (sip == NULL) return FALSE;
  if (sip->choice != SEQID_LOCAL && sip->choice != SEQID_GENERAL) return FALSE;
  bsp = BioseqFind (sip);
  if (bsp == NULL) return FALSE;
  for (id = bsp->id; id != NULL; id = id->next) {
    switch (id->choice) {
      case SEQID_GENERAL :
        if (sip->choice == SEQID_LOCAL) {
          dbt = (DbtagPtr) id->data.ptrvalue;
          if (dbt != NULL) {
            if (StringICmp (dbt->db, "TMSMART") != 0 && StringICmp (dbt->db, "BankIt") != 0) {
              return TRUE;
            }
          }
        }
        break;
      case SEQID_GI :
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_PATENT :
      case SEQID_OTHER :
      case SEQID_DDBJ :
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
      case SEQID_GPIPE :
        return TRUE;
      default :
        break;
    }
  }
  return FALSE;
}

static void CheckFeatLocAndProd (SeqFeatPtr sfp, FeatProbPtr fpp)

{
  SeqLocPtr  slp;

  if (sfp == NULL || fpp == NULL) return;
  if (sfp->product != NULL && IdIsArchaic (SeqLocId (sfp->product))) {
    (fpp->num_archaic_products)++;
  }
  slp = SeqLocFindNext (sfp->location, NULL);
  while (slp != NULL) {
    if (IdIsArchaic (SeqLocId (slp))) {
      (fpp->num_archaic_locations)++;
      return;
    }
    slp = SeqLocFindNext (sfp->location, slp);
  }
}

static void CheckGraphPacking (SeqGraphPtr sgp, Pointer userdata)

{
  BioseqPtr    bsp;
  FeatProbPtr  fpp;
  SeqAnnotPtr  sap;
  BioseqPtr    par;

  if (sgp == NULL || userdata == NULL) return;
  fpp = (FeatProbPtr) userdata;
  bsp = BioseqFindFromSeqLoc (sgp->loc);
  if (sgp->idx.parenttype == OBJ_SEQANNOT) {
    sap = (SeqAnnotPtr) sgp->idx.parentptr;
    if (sap == NULL) return;
    if (sap->idx.parenttype == OBJ_BIOSEQ) {
      /* if graph packaged on bioseq, must be target bioseq */
      par = (BioseqPtr) sap->idx.parentptr;
      if (par != bsp && SeqMgrGetParentOfPart (par, NULL) != bsp) {
        (fpp->num_misplaced_graphs)++;
      }
      return;
    }
    (fpp->num_misplaced_graphs)++;
  }
}

static Boolean LIBCALLBACK CountMisplacedFeatures (BioseqPtr bsp, SeqMgrBioseqContextPtr bcontext)
{
  FeatProbPtr     fpp;
  SeqFeatPtr      sfp;
  SeqMgrFeatContext fcontext;

  fpp = (FeatProbPtr) bcontext->userdata;
  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL) {
    CheckFeatPacking (bsp, sfp, &(fpp->num_misplaced_features));
    CheckFeatLocAndProd (sfp, fpp);
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }

  return TRUE;
}

static void CountGeneXrefs (SeqFeatPtr sfp, Pointer userdata)

{
  FeatProbPtr  fpp;
  GeneRefPtr   grp;

  if (sfp == NULL || userdata == NULL) return;
  fpp = (FeatProbPtr) userdata;

  if (sfp->data.choice == SEQFEAT_GENE) {
    (fpp->num_gene_feats)++;
  }

  grp = SeqMgrGetGeneXref (sfp);
  if (grp == NULL || SeqMgrGeneIsSuppressed (grp)) return;

  (fpp->num_gene_xrefs)++;
}

static Boolean HasTpaUserObject (BioseqPtr bsp)

{
  SeqMgrDescContext  context;
  UserObjectPtr      uop;
  ObjectIdPtr        oip;
  ValNodePtr         vnp;

  if (bsp == NULL) return FALSE;
  vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &context);
  while (vnp != NULL) {
    uop = (UserObjectPtr) vnp->data.ptrvalue;
    if (uop != NULL) {
      oip = uop->type;
      if (oip != NULL && StringICmp (oip->str, "TpaAssembly") == 0) return TRUE;
    }
    vnp = SeqMgrGetNextDescriptor (bsp, vnp, Seq_descr_user, &context);
  }
  return FALSE;
}

static void CheckTpaHist (BioseqPtr bsp, Pointer userdata)

{
  FeatProbPtr  fpp;
  SeqHistPtr   shp;

  if (bsp == NULL || userdata == NULL) return;
  fpp = (FeatProbPtr) userdata;
  if (! HasTpaUserObject (bsp)) return;
  shp = bsp->hist;
  if (shp != NULL && shp->assembly != NULL) {
    (fpp->num_tpa_with_hist)++;
  } else {
    (fpp->num_tpa_without_hist)++;
  }
}

static Boolean IsNoncuratedRefSeq (BioseqPtr bsp, ErrSev *sev)

{
  SeqIdPtr      sip;
  TextSeqIdPtr  tsip;

  if (bsp == NULL) return FALSE;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER) {
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
      if (tsip != NULL && tsip->accession != NULL) {
        if (StringNCmp (tsip->accession, "NM_", 3) == 0 ||
            StringNCmp (tsip->accession, "NP_", 3) == 0 ||
            StringNCmp (tsip->accession, "NG_", 3) == 0 ||
            StringNCmp (tsip->accession, "NR_", 3) == 0) {
          *sev = SEV_WARNING;
          return FALSE;
        }
        return TRUE;
      }
    }
  }
  return FALSE;
}

typedef struct vfcdata {
  ValNodePtr      uids;
  ValNodePtr      unpub;
  ValNodePtr      publshd;
  ValidStructPtr  vsp;
} VfcData, PNTR VfcPtr;

static Boolean SkipSerialOrUIDPub (ValNodePtr vnp)

{
  CitGenPtr  cgp;

  if (vnp == NULL || vnp->next == NULL) return FALSE;
  if (vnp->choice == PUB_Muid || vnp->choice == PUB_Muid) return TRUE;
  if (vnp->choice != PUB_Gen) return FALSE;
  cgp = (CitGenPtr) vnp->data.ptrvalue;
  if (cgp == NULL) return FALSE;
  if (StringNICmp ("BackBone id_pub", cgp->cit, 15) == 0) return FALSE;
  if (cgp->cit == NULL && cgp->journal == NULL && cgp->date == NULL && cgp->serial_number) return TRUE;
  return FALSE;
}

static void MakePubTags (PubdescPtr pdp, Pointer userdata)

{
  Char        buf [1024];
  Int4        muid = 0, pmid = 0;
  VfcPtr      vfp;
  ValNodePtr  vnp;

  if (pdp == NULL || userdata == NULL) return;
  vfp = (VfcPtr) userdata;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == PUB_Muid) {
      muid = vnp->data.intvalue;
    } else if (vnp->choice == PUB_PMid) {
      pmid = vnp->data.intvalue;
    }
  }

  if (pmid != 0) {
    vnp = ValNodeNew (NULL);
    if (vnp != NULL) {
      vnp->choice = 1;
      vnp->data.intvalue = pmid;
      vnp->next = vfp->uids;
      vfp->uids = vnp;
    }
  }
  if (muid != 0) {
    vnp = ValNodeNew (NULL);
    if (vnp != NULL) {
      vnp->choice = 2;
      vnp->data.intvalue = muid;
      vnp->next = vfp->uids;
      vfp->uids = vnp;
    }
  }

  vnp = pdp->pub;
  while (vnp != NULL && SkipSerialOrUIDPub (vnp)) {
    vnp = vnp->next;
  }
  if (vnp != NULL && PubLabelUnique (vnp, buf, sizeof (buf) - 1, OM_LABEL_CONTENT, TRUE) > 0) {
    vnp = ValNodeCopyStr (NULL, 0, buf);
    if (vnp != NULL) {
      if (pmid != 0 || muid != 0) {
        vnp->next = vfp->publshd;
        vfp->publshd = vnp;
      } else {
        vnp->next = vfp->unpub;
        vfp->unpub = vnp;
      }
    }
  }
}

static void CheckOneCit (SeqFeatPtr sfp, ValNodePtr ppr, VfcPtr vfp)

{
  Char              buf [1024];
  GatherContextPtr  gcp;
  size_t            len, lgth;
  CharPtr           str;
  Int4              uid;
  ValNodePtr        vnp;
  ValidStructPtr    vsp;

  if (sfp == NULL || ppr == NULL || vfp == NULL) return;
  vsp = vfp->vsp;
  if (vsp == NULL) return;
  gcp = vsp->gcp;

  if (gcp != NULL) {
    gcp->entityID = sfp->idx.entityID;
    gcp->itemID = sfp->idx.itemID;
    gcp->thistype = OBJ_SEQFEAT;
  }
  vsp->sfp = sfp;

  if (ppr->choice == PUB_PMid || ppr->choice == PUB_Muid) {
    uid = ppr->data.intvalue;
    for (vnp = vfp->uids; vnp != NULL; vnp = vnp->next) {
      if (uid == vnp->data.intvalue) return;
    }
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_FeatureCitationProblem,
              "Citation on feature refers to uid [%ld] not on a publication in the record", (long) uid);
    vsp->sfp = NULL;

  } else if (ppr->choice == PUB_Equiv) {
    return;
  
  } else {
    PubLabelUnique (ppr, buf, sizeof (buf) - 1, OM_LABEL_CONTENT, TRUE);
    lgth = StringLen (buf);
    if (lgth > 0 && buf [lgth - 1] == '>') {
      buf [lgth - 1] = '\0';
     lgth--;
    }
    for (vnp = vfp->unpub; vnp != NULL; vnp = vnp->next) {
      str = (CharPtr) vnp->data.ptrvalue;
      if (StringHasNoText (str)) continue;
      len = MIN (lgth, StringLen (str));
      if (StringNICmp (str, buf, len) == 0) return;
    }
    for (vnp = vfp->publshd; vnp != NULL; vnp = vnp->next) {
      str = (CharPtr) vnp->data.ptrvalue;
      if (StringHasNoText (str)) continue;
      len = MIN (lgth, StringLen (str));
      if (StringNICmp (str, buf, len) == 0) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_FeatureCitationProblem,
                  "Citation on feature needs to be updated to published uid");
        vsp->sfp = NULL;
        return;
      }
    }
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_FeatureCitationProblem,
              "Citation on feature refers to a publication not in the record");
    vsp->sfp = NULL;
  }
}

static void CheckFeatCits (SeqFeatPtr sfp, Pointer userdata)

{
  ValNodePtr  ppr, vnp;
  VfcPtr      vfp;

  if (sfp == NULL || sfp->cit == NULL || userdata == NULL) return;
  vfp = (VfcPtr) userdata;

  vnp = sfp->cit;
  for (ppr = vnp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
    CheckOneCit (sfp, ppr, vfp);
  }
}

static void ValidateFeatCits (SeqEntryPtr sep, ValidStructPtr vsp)

{
  GatherContext  gc;
  VfcData        vfd;

  if (vsp == NULL || sep == NULL) return;
  vsp->gcp = &gc;
  vsp->bssp = NULL;
  vsp->bsp = NULL;
  vsp->sfp = NULL;
  vsp->descr = NULL;
  MemSet ((Pointer) &gc, 0, sizeof (GatherContext));
  MemSet ((Pointer) &vfd, 0, sizeof (VfcData));
  vfd.vsp = vsp;

  VisitPubdescsInSep (sep, (Pointer) &vfd, MakePubTags);

  VisitFeaturesInSep (sep, (Pointer) &vfd, CheckFeatCits);

  ValNodeFree (vfd.uids);
  ValNodeFreeData (vfd.unpub);
  ValNodeFreeData (vfd.publshd);
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
  FeatProb        featprob;
  GatherContextPtr gcp = NULL;
  GatherContext   gc;
  SeqEntryPtr     fsep;
  BioseqPtr       fbsp = NULL;
  Int2            limit;
  SeqEntryPtr     oldsep;
  ErrSev          oldsev;
  ObjMgrDataPtr   omdp;
  SeqEntryPtr     topsep;
  SeqEntryPtr     tmp;
  ValNodePtr      bsplist;
  ErrSev          sev;
  SeqIdPtr        sip;
  Boolean         isGPS = FALSE;
  Boolean         isPatent = FALSE;
  Boolean         isPDB = FALSE;

  for (i = 0; i < 6; i++)       /* keep errors between clears */
    errors[i] = 0;

  MemSet ((Pointer) &featprob, 0, sizeof (FeatProb));

  if (vsp->useSeqMgrIndexes) {
    entityID = ObjMgrGetEntityIDForChoice (sep);

    if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
      oldsev = ErrSetMessageLevel (SEV_MAX);
      SeqMgrIndexFeatures (entityID, NULL);
      ErrSetMessageLevel (oldsev);
    }
    SeqMgrExploreBioseqs (entityID, NULL, (Pointer) &featprob, CountMisplacedFeatures, TRUE, TRUE, TRUE);

    topsep = GetTopSeqEntryForEntityID (entityID);
    VisitGraphsInSep (topsep, (Pointer) &featprob, CheckGraphPacking);
    VisitFeaturesInSep (topsep, (Pointer) &featprob, CountGeneXrefs);
    VisitBioseqsInSep (topsep, (Pointer) &featprob, CheckTpaHist);
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
    /* case BioseqseqSet_class_genbank: */
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
        oldsev = ErrSetMessageLevel (SEV_MAX);
        SeqMgrIndexFeatures (entityID, NULL);
        ErrSetMessageLevel (oldsev);
      }

      /* lock all remote genome components, locations, and products in advance */

      limit = vsp->validationLimit;
      if (limit == VALIDATE_ALL || limit == VALIDATE_INST || limit == VALIDATE_HIST) {
        bsplist = LockFarComponentsEx (sep, TRUE, TRUE, TRUE, NULL);
      }
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
      if (suppress_no_pubs && (! vsp->seqSubmitParent)) {
        omdp = ObjMgrGetData (gc.entityID);
        if (omdp == NULL || omdp->datatype != OBJ_SEQSUB) {
          sev = SEV_ERROR;
          if ((!isGPS) && (!IsNoncuratedRefSeq (fbsp, &sev))) {
            ValidErr (vsp, sev, ERR_SEQ_DESCR_NoPubFound, "No publications anywhere on this entire record.");
          }
        }
      }
      if (suppress_no_biosrc) {
        if ((!isPatent) && ((!isPDB))) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_NoOrgFound, "No organism name anywhere on this entire record.");
        }
      }

      if (featprob.num_misplaced_features > 1) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_FeaturePackagingProblem, "There are %d mispackaged features in this record.", (int) featprob.num_misplaced_features);
      } else if (featprob.num_misplaced_features == 1) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_FeaturePackagingProblem, "There is %d mispackaged feature in this record.", (int) featprob.num_misplaced_features);
      }

      if (featprob.num_misplaced_graphs > 1) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_GraphPackagingProblem, "There are %d mispackaged graphs in this record.", (int) featprob.num_misplaced_graphs);
      } else if (featprob.num_misplaced_graphs == 1) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_PKG_GraphPackagingProblem, "There is %d mispackaged graph in this record.", (int) featprob.num_misplaced_graphs);
      }

      /*
      if (featprob.num_archaic_locations > 1) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_PKG_ArchaicFeatureLocation, "There are %d archaic feature locations in this record.", (int) featprob.num_archaic_locations);
      } else if (featprob.num_archaic_locations == 1) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_PKG_ArchaicFeatureLocation, "There is %d archaic feature location in this record.", (int) featprob.num_archaic_locations);
      }

      if (featprob.num_archaic_products > 1) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_PKG_ArchaicFeatureProduct, "There are %d archaic feature products in this record.", (int) featprob.num_archaic_products);
      } else if (featprob.num_archaic_products == 1) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_PKG_ArchaicFeatureProduct, "There is %d archaic feature product in this record.", (int) featprob.num_archaic_products);
      }
      */

      if (featprob.num_gene_feats == 0 && featprob.num_gene_xrefs > 0) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_OnlyGeneXrefs, "There are %ld gene xrefs and no gene features in this record.", (long) featprob.num_gene_xrefs);
      }

      if (featprob.num_tpa_with_hist > 0 && featprob.num_tpa_without_hist > 0) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_TpaAssmeblyProblem, "There are %ld TPAs with history and %ld without history in this record.",
                  (long) featprob.num_tpa_with_hist, (long) featprob.num_tpa_without_hist);
      }

      first = FALSE;
    }

    vsp->bsp = NULL;

    topsep = GetTopSeqEntryForEntityID (gc.entityID);
    oldsep = SeqEntrySetScope (topsep);

    /* AssignIDsInEntity (gc.entityID, 0, NULL); */

    GatherSeqEntry (sep, (Pointer) vsp, Valid1GatherProc, &gs);

    vsp->gcp = NULL;
    ValidateFeatCits (sep, vsp);
    vsp->gcp = NULL;

    if (vsp->validateAlignments) {
      vsp->gcp = NULL;
      ValidateSeqAlignWithinValidator (vsp, sep, vsp->alignFindRemoteBsp, vsp->doSeqHistAssembly);
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
  if (cl == BioseqseqSet_class_eco_set)
    return ("eco-set");
  if (cl == BioseqseqSet_class_gen_prod_set)
    return ("gen-prod-set");
  if (cl == BioseqseqSet_class_wgs_set)
    return ("wgs-set");
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

typedef struct incons {
  Boolean     diffs;
  MolInfoPtr  mip;
} Incons, PNTR InconsPtr;

static void FindInconsistMolInfos (SeqDescrPtr sdp, Pointer userdata)

{
  InconsPtr   icp;
  MolInfoPtr  mip;

  if (sdp == NULL || sdp->choice != Seq_descr_molinfo) return;
  icp = (InconsPtr) userdata;
  mip = (MolInfoPtr) sdp->data.ptrvalue;
  if (icp == NULL || mip == NULL) return;
  if (icp->mip == NULL) {
    icp->mip = mip;
  } else {
    if (icp->mip->biomol != mip->biomol) {
      icp->diffs = TRUE;
    }
  }
}

static void ValidateSegmentedSet (BioseqSetPtr bssp, ValidStructPtr vsp)
{
  SeqEntryPtr     sep;
  BioseqSetPtr    bssp1;
  BioseqPtr       bsp;
  Incons          inc;
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

  inc.diffs = FALSE;
  inc.mip = NULL;
  VisitDescriptorsInSet (bssp, (Pointer) &inc, FindInconsistMolInfos);
  if (inc.diffs) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_PKG_InconsistentMolInfoBiomols, "Segmented set contains inconsistent MolInfo biomols");
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

static Boolean CheckForInconsistentBiosources (SeqEntryPtr sep, ValidStructPtr vsp, OrgRefPtr PNTR orpp, BioseqSetPtr top)
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
  size_t          len;
  ErrSev          sev;
  CharPtr         sp;

  if (sep == NULL || vsp == NULL || orpp == NULL)
    return FALSE;
  gcp = vsp->gcp;

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL)
      return FALSE;
    for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
      if (CheckForInconsistentBiosources (tmp, vsp, orpp, top))
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

  sev = SEV_ERROR;
  sp = StringStr (orp->taxname, " sp. ");
  if (sp != NULL) {
    len = sp - orp->taxname + 5;
    if (StringNCmp (orp->taxname, firstorp->taxname, len) == 0) {
      sev = SEV_WARNING;
    }
  }

  oldEntityID = gcp->entityID;
  oldItemID = gcp->itemID;
  oldItemtype = gcp->thistype;

  gcp->entityID = entityID;
  gcp->itemID = itemID;
  gcp->thistype = itemtype;

  if (top != NULL) {
    gcp->entityID = top->idx.entityID;
    gcp->itemID = top->idx.itemID;
    gcp->thistype = OBJ_BIOSEQSET;
  }

  /* only report the first one that doesn't match - but might be lower severity if not all are sp. */

  ValidErr (vsp, sev, ERR_SEQ_DESCR_InconsistentBioSources, "Population set contains inconsistent organisms.");

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
    if (CheckForInconsistentBiosources (sep, vsp, &orp, bssp))
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
*   ValidateBioseqHist(gcp)
*      Validate one Bioseq Seq-hist
*
*****************************************************************************/
static void ValidateBioseqHist (GatherContextPtr gcp)

{
  BioseqPtr       bsp;
  Int4            gi = 0;
  SeqHistPtr      hist;
  SeqIdPtr        sip;
  ValidStructPtr  vsp;

  if (gcp == NULL) return;
  vsp = (ValidStructPtr) (gcp->userdata);
  bsp = (BioseqPtr) (gcp->thisitem);
  vsp->bsp = bsp;
  vsp->descr = NULL;
  vsp->sfp = NULL;
  vsp->bssp = (BioseqSetPtr) (gcp->parentitem);
  vsp->bsp_partial_val = 0;

  if (bsp == NULL) return;
  hist = bsp->hist;
  if (hist == NULL) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GI) {
      gi = (Int4) sip->data.intvalue;
    }
  }
  if (gi == 0) return;

  if (hist->replaced_by_ids != NULL && hist->replaced_by_date != NULL) {

    for (sip = hist->replaced_by_ids; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_GI) {
        if (gi == (Int4) sip->data.intvalue) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_HistoryGiCollision, "Replaced by gi (%ld) is same as current Bioseq", (long) gi);
        }
      }
    }
  }

  if (hist->replace_ids != NULL && hist->replace_date != NULL) {

    for (sip = hist->replace_ids; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_GI) {
        if (gi == (Int4) sip->data.intvalue) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_HistoryGiCollision, "Replaces gi (%ld) is same as current Bioseq", (long) gi);
        }
      }
    }
  }
}

/*****************************************************************************
*
*   ValidateBioseqInst(gcp)
*      Validate one Bioseq Seq-inst
*
*****************************************************************************/
static Boolean IsTpa (
  BioseqPtr bsp,
  Boolean has_tpa_assembly,
  BoolPtr isRefSeqP
)

{
  DbtagPtr  dbt;
  Boolean   has_bankit = FALSE;
  Boolean   has_genbank = FALSE;
  Boolean   has_gi = FALSE;
  Boolean   has_local = FALSE;
  Boolean   has_refseq = FALSE;
  Boolean   has_smart = FALSE;
  Boolean   has_tpa = FALSE;
  SeqIdPtr  sip;

  if (bsp == NULL || bsp->id == NULL) return FALSE;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_LOCAL :
        has_local = TRUE;
        break;
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_DDBJ :
        has_genbank = TRUE;
        break;
      case SEQID_OTHER :
        has_refseq = TRUE;
        if (isRefSeqP != NULL) {
          *isRefSeqP = TRUE;
        }
        break;
      case SEQID_GI :
        has_gi = TRUE;
        break;
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
        has_tpa = TRUE;
        break;
      case SEQID_GENERAL :
        dbt = (DbtagPtr) sip->data.ptrvalue;
        if (dbt != NULL) {
          if (StringICmp (dbt->db, "BankIt") == 0) {
            has_bankit = TRUE;
          }
          if (StringICmp (dbt->db, "TMSMART") == 0) {
            has_smart = TRUE;
          }
        }
        break;
      case SEQID_GPIPE :
        break;
      default :
        break;
    }
  }

  if (has_genbank) return FALSE;
  if (has_tpa) return TRUE;
  if (has_refseq) return FALSE;
  if (has_bankit && has_tpa_assembly) return TRUE;
  if (has_smart && has_tpa_assembly) return TRUE;
  if (has_gi) return FALSE;
  if (has_local && has_tpa_assembly) return TRUE;

  return FALSE;
}

static void ValidateIDSetAgainstDb (GatherContextPtr gcp, ValidStructPtr vsp, BioseqPtr bsp)

{
  SeqIdPtr        sip, sipset;
  SeqIdPtr        gbId = NULL;
  SeqIdPtr        dbGbId;
  DbtagPtr        generalID = NULL;
  DbtagPtr        dbGeneralID;
  Int4            gi = 0;
  Int4            dbGI;
  Boolean         hasGi = FALSE;
  Char            oldGenID [128], newGenID [128];

  if (gcp != NULL && vsp != NULL && bsp != NULL && vsp->validateIDSet) {
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      switch (sip->choice) {
        case SEQID_GENBANK:
          gbId = sip;
          break;
        case SEQID_GI :
          gi = (Int4) sip->data.intvalue;
          break;
        case SEQID_GENERAL :
          generalID = (DbtagPtr) sip->data.ptrvalue;
          break;
        default :
          break;
      }
    }
    if (gi == 0 && gbId != NULL) {
      gi = GetGIForSeqId (gbId);
    }
    if (gi > 0) {
      sipset = GetSeqIdSetForGI (gi);
      if (sipset != NULL) {
        dbGI = 0;
        dbGbId = NULL;
        dbGeneralID = NULL;
        oldGenID [0] = '\0';
        newGenID [0] = '\0';
        for (sip = sipset; sip != NULL; sip = sip->next) {
          switch (sip->choice) {
            case SEQID_GI :
              dbGI = (Int4) sip->data.intvalue;
              break;
            case SEQID_GENBANK:
              dbGbId = sip;
              break;
            case SEQID_GENERAL :
              dbGeneralID = (DbtagPtr) sip->data.ptrvalue;
              break;
            default :
              break;
          }
        }
        if (dbGI != gi) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_UnexpectedIdentifierChange, "New gi number (%ld) does not match old one (%ld)", (long) gi, (long) dbGI);
        }
        if (gbId != NULL && dbGbId != NULL) {
          if (! SeqIdMatch (gbId, dbGbId)) {
            SeqIdWrite (dbGbId, oldGenID, PRINTID_FASTA_SHORT, sizeof (oldGenID));
            SeqIdWrite (gbId, newGenID, PRINTID_FASTA_SHORT, sizeof (newGenID));
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_UnexpectedIdentifierChange, "New accession (%s) does not match old one (%s) on gi (%ld)", newGenID, oldGenID, (long) gi);
          }
        } else if (gbId != NULL) {
          SeqIdWrite (gbId, newGenID, PRINTID_FASTA_SHORT, sizeof (newGenID));
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_UnexpectedIdentifierChange, "Gain of accession (%s) on gi (%ld)", newGenID, (long) gi);
        } else if (dbGbId != NULL) {
          SeqIdWrite (dbGbId, oldGenID, PRINTID_FASTA_SHORT, sizeof (oldGenID));
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_UnexpectedIdentifierChange, "Loss of accession (%s) on gi (%ld)", oldGenID, (long) gi);
        }
        if (generalID != NULL && dbGeneralID != NULL) {
          if (! DbtagMatch (generalID, dbGeneralID)) {
            DbtagLabel (dbGeneralID, oldGenID, sizeof (oldGenID));
            DbtagLabel (generalID, newGenID, sizeof (newGenID));
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_UnexpectedIdentifierChange, "New general ID (%s) does not match old one (%s) on gi (%ld)", newGenID, oldGenID, (long) gi);
          }
        } else if (generalID != NULL) {
          DbtagLabel (generalID, newGenID, sizeof (newGenID));
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_UnexpectedIdentifierChange, "Gain of general ID (%s) on gi (%ld)", newGenID, (long) gi);
        } else if (dbGeneralID != NULL) {
          DbtagLabel (dbGeneralID, oldGenID, sizeof (oldGenID));
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_UnexpectedIdentifierChange, "Loss of general ID (%s) on gi (%ld)", oldGenID, (long) gi);
        }
      }
      SeqIdSetFree (sipset);
    }
  }
}

typedef struct enrun {
  Int4  ncount;
  Int4  maxrun;
} RunOfNs, PNTR RunOfNsPtr;

static void LIBCALLBACK CountAdjacentProc (CharPtr sequence, Pointer userdata)

{
  Char        ch;
  RunOfNsPtr  ronp;
  CharPtr     str;

  ronp = (RunOfNsPtr) userdata;
  if (sequence == NULL || ronp == NULL) return;

  str = sequence;
  ch = *str;
  while (ch != '\0') {
    if (ch == 'N') {
      (ronp->ncount)++;
      if (ronp->ncount > ronp->maxrun) {
        ronp->maxrun = ronp->ncount;
      }
    } else {
      ronp->ncount = 0;
    }
    str++;
    ch = *str;
  }
}

static Int4 CountAdjacentNsInSeqLit (SeqLitPtr slitp, Boolean is_na)

{
  BioseqPtr  bsp;
  RunOfNs    ron;

  if (slitp == NULL || slitp->length < 1 || slitp->seq_data == NULL) return 0;

  bsp = BioseqNew ();
  if (bsp == NULL) return 0;

  if (slitp->seq_data != NULL) {
    bsp->repr = Seq_repr_raw;
  } else {
    bsp->repr = Seq_repr_virtual;
  }
  if (is_na) {
    bsp->mol = Seq_mol_dna;
  } else {
    bsp->mol = Seq_mol_aa;
  }
  bsp->seq_data_type = slitp->seq_data_type;
  bsp->seq_data = slitp->seq_data;
  bsp->length = slitp->length;
  bsp->id = SeqIdParse ("lcl|countseqlitns");

  ron.ncount = 0;
  ron.maxrun = 0;

  SeqPortStream (bsp, STREAM_EXPAND_GAPS, (Pointer) &ron, CountAdjacentProc);

  bsp->seq_data = NULL;

  BioseqFree (bsp);

  return ron.maxrun;
}

static Boolean DeltaLitOnly (
  BioseqPtr bsp
)

{
  ValNodePtr  vnp;

  if (bsp == NULL || bsp->repr != Seq_repr_delta) return FALSE;
  for (vnp = (ValNodePtr)(bsp->seq_ext); vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 1) return FALSE;
  }
  return TRUE;
}

static void ValidateBioseqInst (GatherContextPtr gcp)
{
  Boolean         retval = TRUE;
  Int2            i, start_at, num;
  Boolean         errors[4], check_alphabet;
  static char    *repr[8] = {
    "virtual", "raw", "segmented", "constructed",
    "reference", "consensus", "map", "delta"
  };
  /*
  SeqPortPtr      spp;
  */
  Int2            residue, x, termination, gapchar;
  Boolean         gapatstart;
  Int4            len, divisor = 1, len2, len3;
  ValNode         head, vn;
  ValNodePtr      vnp, vnp2, idlist;
  BioseqContextPtr bcp;
  Boolean         got_partial, is_invalid;
  int             seqtype, terminations, dashes;
  ValidStructPtr  vsp;
  BioseqPtr       bsp, bsp2;
  SeqIdPtr        sip1, sip2, sip3;
  SeqLocPtr       slp;
  SeqIntPtr       sintp;
  Char            buf1[41], buf2[41];
  SeqLitPtr       slitp;
  SeqCodeTablePtr sctp;
  MolInfoPtr      mip;
  SeqMgrDescContext context;
  SeqFeatPtr      cds;
  GBBlockPtr      gbp;
  GeneRefPtr      grp;
  SeqFeatPtr      gene;
  SeqMgrFeatContext genectxt;
  CharPtr         genelbl = NULL;
  SeqFeatPtr      prot;
  SeqMgrFeatContext protctxt;
  CharPtr         protlbl = NULL;
  TextSeqIdPtr    tsip;
  CharPtr         ptr, last, str, title, buf;
  Uint1           lastchoice;
  Char            ch;
  Boolean         multitoken;
  Boolean         hasGi = FALSE;
  SeqHistPtr      hist;
  IntFuzzPtr      ifp;
  Boolean         isActiveFin = FALSE;
  Boolean         isGB = FALSE;
  Boolean         isPatent = FALSE;
  Boolean         isPDB = FALSE;
  Boolean         isNC = FALSE;
  Boolean         isNTorNC = FALSE;
  Boolean         isNZ;
  Boolean         is_gps = FALSE;
  Boolean         isRefSeq = FALSE;
  Boolean         last_is_gap;
  Boolean         non_interspersed_gaps;
  Int2            num_adjacent_gaps;
  Int2            num_gaps;
  SeqFeatPtr      sfp;
  SeqEntryPtr     sep;
  ErrSev          sev;
  Int2            trailingX = 0;
  Int2            numletters, numdigits, numunderscores;
  Boolean         letterAfterDigit, badIDchars;
  EMBLBlockPtr    ebp;
  SeqDescrPtr     sdp;
  SeqMgrDescContext dcontext;
  Uint2           oldEntityID, oldItemID, oldItemtype;
  size_t          buflen = 1001;
  ItemInfo        ii;
  Uint1           tech;
  Uint2           olditemtype = 0;
  Uint2           olditemid = 0;
  ObjValNodePtr   ovp;
  BioseqSetPtr    bssp;
  UserObjectPtr   uop;
  UserFieldPtr    ufp;
  ObjectIdPtr     oip;
  Boolean         hasRefTrackStatus;
  Int2            accn_count = 0;
  Int2            gi_count = 0;
  Int4            runsofn;
  Int4            segnum;
  StreamCache     sc;

  /* set up data structures */

  vsp = (ValidStructPtr) (gcp->userdata);
  bsp = (BioseqPtr) (gcp->thisitem);
  vsp->bsp = bsp;
  vsp->descr = NULL;
  vsp->sfp = NULL;
  vsp->bssp = (BioseqSetPtr) (gcp->parentitem);
  vsp->bsp_partial_val = 0;

  sep = vsp->sep;

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
      isRefSeq = TRUE;
      tsip = (TextSeqIdPtr) sip1->data.ptrvalue;
      if (tsip != NULL && tsip->accession != NULL) {
        if (StringNICmp (tsip->accession, "NT_", 3) == 0) {
          isNTorNC = TRUE;
        } else if (StringNICmp (tsip->accession, "NC_", 3) == 0) {
          isNTorNC = TRUE;
          isNC = TRUE;
        }
      }
    } else if (sip1->choice == SEQID_GI) {
      hasGi = TRUE;
    } else if (sip1->choice == SEQID_GENBANK) {
      isGB = TRUE;
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
    case SEQID_TPG:
    case SEQID_TPE:
    case SEQID_TPD:
      hist = bsp->hist;
      if (hist == NULL || hist->assembly == NULL) {
        if (ISA_na (bsp->mol) && bsp->repr != Seq_repr_seg) {
          SeqIdWrite (bsp->id, buf1, PRINTID_FASTA_SHORT, 40);
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_HistAssemblyMissing, "TPA record %s should have Seq-hist.assembly for PRIMARY block", buf1);
        }
      }
      /* continue falling through */
    case SEQID_GENBANK:
    case SEQID_EMBL:
    case SEQID_DDBJ:
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
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadSeqIdFormat, "Bad accession %s", tsip->accession);
        } else if (numletters == 1 && numdigits == 5 && ISA_na (bsp->mol)) {
        } else if (numletters == 2 && numdigits == 6 && ISA_na (bsp->mol)) {
        } else if (numletters == 3 && numdigits == 5 && ISA_aa (bsp->mol)) {
        } else if (numletters == 2 && numdigits == 6 && ISA_aa (bsp->mol) && bsp->repr == Seq_repr_seg) {
        } else if (numletters == 4 && numdigits == 8 && ISA_na (bsp->mol) &&
                   (sip1->choice == SEQID_GENBANK || sip1->choice == SEQID_EMBL || sip1->choice == SEQID_DDBJ)) {
        } else if (numletters == 4 && numdigits == 9 && ISA_na (bsp->mol) &&
                   (sip1->choice == SEQID_GENBANK || sip1->choice == SEQID_EMBL || sip1->choice == SEQID_DDBJ)) {
        } else {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadSeqIdFormat, "Bad accession %s", tsip->accession);
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
        if (hasGi) {
          if (tsip->version == 0) {
            ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_BadSeqIdFormat, "Accession %s has 0 version", tsip->accession);
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
      if (tsip != NULL && tsip->accession != NULL && sip1->choice == SEQID_OTHER) {
        numletters = 0;
        numdigits = 0;
        numunderscores = 0;
        letterAfterDigit = FALSE;
        badIDchars = FALSE;
        ptr = tsip->accession;
        isNZ = (Boolean) (StringNCmp (ptr, "NZ_", 3) == 0);
        if (isNZ) {
          ptr += 3;
        }
        for (ch = *ptr; ch != '\0'; ptr++, ch = *ptr) {
          if (IS_UPPER (ch)) {
            numletters++;
            if (numdigits > 0 || numunderscores > 0) {
              letterAfterDigit = TRUE;
            }
          } else if (IS_DIGIT (ch)) {
            numdigits++;
          } else if (ch == '_') {
            numunderscores++;
            if (numdigits > 0 || numunderscores > 1) {
              letterAfterDigit = TRUE;
            }
          } else {
            badIDchars = TRUE;
          }
        }
        if (letterAfterDigit || badIDchars) {
          ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_BadSeqIdFormat, "Bad accession %s", tsip->accession);
        } else if (isNZ && numletters == 4 && numdigits == 8 && numunderscores == 0) {
        } else if (numletters == 2 && numdigits == 6 && numunderscores == 1) {
        } else if (numletters == 2 && numdigits == 8 && numunderscores == 1) {
        } else if (numletters == 2 && numdigits == 9 && numunderscores == 1) {
        } else {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadSeqIdFormat, "Bad accession %s", tsip->accession);
        }
      }
      if (hasGi && tsip != NULL && tsip->accession == NULL && (! StringHasNoText (tsip->name))) {
        if (sip1->choice == SEQID_DDBJ && bsp->repr == Seq_repr_seg) {
          sev = SEV_WARNING;
          /*
          ValidErr (vsp, sev, ERR_SEQ_INST_BadSeqIdFormat, "Missing accession for %s", tsip->name);
          */
        } else {
          sev = SEV_REJECT;
          ValidErr (vsp, sev, ERR_SEQ_INST_BadSeqIdFormat, "Missing accession for %s", tsip->name);
        }
      }
      /* and keep going with additional test */
    case SEQID_PIR:
    case SEQID_SWISSPROT:
    case SEQID_PRF:
      tsip = (TextSeqIdPtr) sip1->data.ptrvalue;
      if (tsip != NULL && StringHasNoText (tsip->accession) && ISA_na (bsp->mol)) {
        if (bsp->repr != Seq_repr_seg || hasGi) {
          if (sip1->choice != SEQID_DDBJ || bsp->repr != Seq_repr_seg) {
            SeqIdWrite (bsp->id, buf1, PRINTID_FASTA_LONG, sizeof (buf1) - 1);
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadSeqIdFormat, "Missing accession for %s", buf1);
          }
        }
      }
      if (tsip != NULL && StringHasNoText (tsip->accession) &&
          StringHasNoText (tsip->name) && ISA_aa (bsp->mol)) {
        if (sip1->choice == SEQID_PIR || sip1->choice == SEQID_SWISSPROT || sip1->choice == SEQID_PRF) {
          SeqIdWrite (bsp->id, buf1, PRINTID_FASTA_LONG, sizeof (buf1) - 1);
          ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_BadSeqIdFormat, "Missing identifier for %s", buf1);
        }
      }
      accn_count++;
      break;
    case SEQID_GPIPE:
      break;
    case SEQID_PATENT:
      isPatent = TRUE;
      break;
    case SEQID_PDB:
      isPDB = TRUE;
      break;
    case SEQID_GI:
      if (sip1->data.intvalue <= 0) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_ZeroGiNumber, "Invalid GI number");
      }
      gi_count++;
      break;
    case SEQID_GENERAL:
      break;
    default:
      break;
    }
  }

  if (gi_count > 0 && accn_count == 0 && (! isPDB) && bsp->repr != Seq_repr_virtual) {
    if (vsp->seqSubmitParent) {
      sev = SEV_WARNING;
    } else {
      sev = SEV_ERROR;
    }
    ValidErr (vsp, sev, ERR_SEQ_INST_GiWithoutAccession, "No accession on sequence with gi number");
  }
  if (gi_count > 0 && accn_count > 1) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_MultipleAccessions, "Multiple accessions on sequence with gi number");
  }

  /* optionally check IDs against older version in database */

  if (vsp->validateIDSet) {
    ValidateIDSetAgainstDb (gcp, vsp, bsp);
  }

  if (vsp->useSeqMgrIndexes) {
    vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &context);
    while (vnp != NULL) {
      uop = (UserObjectPtr) vnp->data.ptrvalue;
      if (uop != NULL) {
        oip = uop->type;
        if (oip != NULL && StringICmp (oip->str, "TpaAssembly") == 0) {
          if (! IsTpa (bsp, TRUE, &isRefSeq)) {
            olditemid = gcp->itemID;
            olditemtype = gcp->thistype;
            gcp->itemID = context.itemID;
            gcp->thistype = OBJ_SEQDESC;
            SeqIdWrite (bsp->id, buf1, PRINTID_FASTA_SHORT, 40);
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Non-TPA record %s should not have TpaAssembly object", buf1);
            gcp->itemID = olditemid;
            gcp->thistype = olditemtype;
          }
        } else if (oip != NULL && StringICmp (oip->str, "RefGeneTracking") == 0) {
          hasRefTrackStatus = FALSE;
          for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
            oip = ufp->label;
            if (oip != NULL && StringCmp (oip->str, "Status") == 0) {
              hasRefTrackStatus = TRUE;
            }
          }
          if (! hasRefTrackStatus) {
            olditemid = gcp->itemID;
            olditemtype = gcp->thistype;
            gcp->itemID = context.itemID;
            gcp->thistype = OBJ_SEQDESC;
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_RefGeneTrackingWithoutStatus, "RefGeneTracking object needs to have Status set");
            gcp->itemID = olditemid;
            gcp->thistype = olditemtype;
          }
          if (! isRefSeq) {
            olditemid = gcp->itemID;
            olditemtype = gcp->thistype;
            gcp->itemID = context.itemID;
            gcp->thistype = OBJ_SEQDESC;
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_RefGeneTrackingOnNonRefSeq, "RefGeneTracking object should only be in RefSeq record");
            gcp->itemID = olditemid;
            gcp->thistype = olditemtype;
          }
        }
      }
      vnp = SeqMgrGetNextDescriptor (bsp, vnp, Seq_descr_user, &context);
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
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_IdOnMultipleBioseqs, "SeqID %s is present on multiple Bioseqs in record", buf1);
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
    ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_SeqDataNotFound, "Missing Seq-data on %s Bioseq", repr[bsp->repr - 1]);
    retval = FALSE;
  }

  if (errors[3] == TRUE) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_SeqDataNotAllowed, "Seq-data not allowed on %s Bioseq", repr[bsp->repr - 1]);
    retval = FALSE;
  }

  if (!retval)
    return;

  oldEntityID = gcp->entityID;
  oldItemID = gcp->itemID;
  oldItemtype = gcp->thistype;

  if (SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext) != NULL) {
    gcp->entityID = dcontext.entityID;
    gcp->itemID = dcontext.itemID;
    gcp->thistype = OBJ_SEQDESC;
  }

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

  gcp->entityID = oldEntityID;
  gcp->itemID = oldItemID;
  gcp->thistype = oldItemtype;

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
        gapchar = '-';
        break;
      case Seq_code_ncbistdaa:
        termination = 25;
        gapchar = 0;
        break;
      default:
        termination = '\0';
        gapchar = '\0';
        break;
      }
      if (! StreamCacheSetup (bsp, NULL, STREAM_EXPAND_GAPS, &sc)) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_SeqPortFail, "Can't open StreamCache");
        return;
      }
      /*
      spp = SeqPortNew (bsp, 0, -1, 0, 0);
      if (spp == NULL) {
        ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_SeqPortFail, "Can't open SeqPort");
        return;
      }
      */
      i = 0;
      terminations = 0;
      dashes = 0;
      gapatstart = FALSE;
      trailingX = 0;
      for (len = 0; len < bsp->length; len++) {
        residue = StreamCacheGetResidue (&sc);
        /*
        residue = SeqPortGetResidue (spp);
        */
        if (!IS_residue (residue)) {
          i++;
          if (i > 10) {
            ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidResidue, "More than 10 invalid residues. Checking stopped");
            /*
            SeqPortFree (spp);
            */
            if (vsp->patch_seq)
              PatchBadSequence (bsp);
            return;
          } else {
            BSSeek (bsp->seq_data, len, SEEK_SET);
            x = BSGetByte (bsp->seq_data);
            if (bsp->seq_data_type == Seq_code_ncbistdaa) {
              ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidResidue, "Invalid residue [%d] in position [%ld]", (int) x, (long) (len + 1));
            } else if (IS_ALPHANUM (x)) {
              ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidResidue, "Invalid residue '%c' in position [%ld]", (char) x, (long) (len + 1));
            } else {
              ValidErr (vsp, SEV_REJECT, ERR_SEQ_INST_InvalidResidue, "Invalid residue [%d] in position [%ld]", (int) x, (long) (len + 1));
            }
          }
        } else if (residue == termination) {
          terminations++;
          trailingX = 0;        /* suppress if followed by terminator */
        } else if (residue == gapchar) {
          dashes++;
          if (len == 0) {
            gapatstart = TRUE;
          }
        } else if (residue == 'X') {
          trailingX++;
        } else {
          trailingX = 0;
        }
      }
      /*
      SeqPortFree (spp);
      */
      if (trailingX > 0 && SuppressTrailingXMessage (bsp)) {
        /* suppress if cds translation ends in '*' or 3' partial */
      } else if (trailingX > 1) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_TrailingX, "Sequence ends in %d trailing Xs", (int) trailingX);
      } else if (trailingX > 0) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_TrailingX, "Sequence ends in %d trailing X", (int) trailingX);
      }
      if (terminations > 0 || dashes > 0) {
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
          else if (grp->locus_tag != NULL)
            genelbl = (grp->locus_tag);
          else if (grp->desc != NULL)
            genelbl = (grp->desc);
          else if (grp->syn != NULL)
            genelbl = (CharPtr) (grp->syn->data.ptrvalue);
        }
        prot = SeqMgrGetBestProteinFeature (bsp, &protctxt);
        protlbl = protctxt.label;
      }
      if (StringHasNoText (genelbl)) {
        genelbl = "gene?";
      }
      if (StringHasNoText (protlbl)) {
        protlbl = "prot?";
      }
      if (dashes > 0) {
        if (gapatstart && dashes == 1) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadProteinStart, "gap symbol at start of protein sequence (%s - %s)", genelbl, protlbl);
        } else if (gapatstart) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadProteinStart, "gap symbol at start of protein sequence (%s - %s)", genelbl, protlbl);
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_GapInProtein, "[%d] internal gap symbols in protein sequence (%s - %s)", (dashes - 1), genelbl, protlbl);
        } else {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_GapInProtein, "[%d] internal gap symbols in protein sequence (%s - %s)", dashes, genelbl, protlbl);
        }
      }
      if (terminations) {
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
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_DuplicateSegmentReferences, "Segmented sequence has multiple references to %s", str);
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

  if (bsp->repr == Seq_repr_delta || bsp->repr == Seq_repr_raw) {

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

    vnp = NULL;
    if (vsp->useSeqMgrIndexes) {
      vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &context);
    } else {
      bcp = BioseqContextNew (bsp);
      vnp = BioseqContextGetSeqDescr (bcp, Seq_descr_genbank, NULL, NULL);
      BioseqContextFree (bcp);
    }
    if (vnp != NULL) {
      gbp = (GBBlockPtr) vnp->data.ptrvalue;
      if (gbp != NULL) {
        for (vnp = gbp->keywords; vnp != NULL; vnp = vnp->next) {
          str = (CharPtr) vnp->data.ptrvalue;
          if (StringICmp (str, "HTGS_ACTIVEFIN") == 0) {
            isActiveFin = TRUE;
          }
        }
      }
    }
  }

  if (bsp->repr == Seq_repr_delta) {
    len = 0;
    for (vnp = (ValNodePtr) (bsp->seq_ext), segnum = 0; vnp != NULL; vnp = vnp->next, segnum++) {
      if (vnp->data.ptrvalue == NULL)
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_SeqDataLenWrong, "NULL pointer in delta seq_ext valnode");
      else {
        switch (vnp->choice) {
        case 1:                /* SeqLocPtr */
          slp = (SeqLocPtr) (vnp->data.ptrvalue);
          len2 = SeqLocLen (slp);
          if (len2 < 0)
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_SeqDataLenWrong, "-1 length on seq-loc of delta seq_ext");
          else
            len += len2;
          sip3 = SeqLocId (slp);
          if (sip3 != NULL && sip3->choice == SEQID_GI && slp != NULL && slp->choice == SEQLOC_INT) {
            sintp = (SeqIntPtr) slp->data.ptrvalue;
            if (sintp != NULL) {
              vn.choice = SEQLOC_WHOLE;
              vn.data.ptrvalue = sip3;
              vn.next = NULL;
              len3 = SeqLocLen (&vn);
              if (sintp->to >= len3) {
                ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_SeqDataLenWrong,
                          "Seq-loc extent (%ld) greater than length of gi %ld (%ld)",
                          (long) (sintp->to + 1), (long) sip3->data.intvalue, (long) len3);
              }
            }
          }
          if (len2 <= 10) {
            str = SeqLocPrint ((SeqLocPtr) (vnp->data.ptrvalue));
            if (str == NULL) {
              str = StringSave ("?");
            }
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_SeqLocLength, "Short length (%ld) on seq-loc (%s) of delta seq_ext", (long) len2, str);
            MemFree (str);
          }
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
            if (mip != NULL) {
              if (mip->tech == MI_TECH_htgs_1 || mip->tech == MI_TECH_htgs_2) {
                runsofn = CountAdjacentNsInSeqLit (slitp, (Boolean) ISA_na (bsp->mol));
                if (runsofn > 80) {
                  ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_InternalNsInSeqLit, "Run of %ld Ns in delta component %ld that starts at base %ld", (long) runsofn, (int) segnum, (long) len);
                }
              }
            }
          } else if (slitp->length == 0) {
            ifp = slitp->fuzz;
            if (ifp == NULL || ifp->choice != 4 || ifp->a != 0) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_SeqLitGapLength0, "Gap of length 0 in delta chain");
            } else {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_SeqLitGapLength0, "Gap of length 0 with unknown fuzz in delta chain");
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
    if (mip != NULL) {
      is_gps = FALSE;
      sep = vsp->sep;
      if (sep != NULL && IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set) {
          is_gps = TRUE;
        }
      }
      if ((!isNTorNC) && (! is_gps) && mip->tech != MI_TECH_htgs_0 && mip->tech != MI_TECH_htgs_1 &&
          mip->tech != MI_TECH_htgs_2 && mip->tech != MI_TECH_htgs_3 && mip->tech != MI_TECH_wgs &&
          mip->tech != MI_TECH_unknown && mip->tech != MI_TECH_standard) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadDeltaSeq, "Delta seq technique should not be [%d]", (int) (mip->tech));
      }
    }
  }

  sev = SEV_ERROR;
  if (mip != NULL) {
    if (mip->tech != MI_TECH_htgs_0 && mip->tech != MI_TECH_htgs_1 &&
        mip->tech != MI_TECH_htgs_2 && mip->tech != MI_TECH_htgs_3) {
      sev = SEV_WARNING;
    }
  }

  if (bsp->repr == Seq_repr_delta && bsp->seq_ext_type == 4 && bsp->seq_ext != NULL) {
    vnp = (DeltaSeqPtr) bsp->seq_ext;
    if (vnp != NULL && vnp->choice == 2) {
      slitp = (SeqLitPtr) vnp->data.ptrvalue;
      if (slitp != NULL && slitp->seq_data == NULL) {
        ValidErr (vsp, sev, ERR_SEQ_INST_BadDeltaSeq, "First delta seq component is a gap");
      }
    }
    last_is_gap = FALSE;
    num_adjacent_gaps = 0;
    num_gaps = 0;
    non_interspersed_gaps = FALSE;
    while (vnp->next != NULL) {
      vnp = vnp->next;
      if (vnp != NULL && vnp->choice == 2) {
        slitp = (SeqLitPtr) vnp->data.ptrvalue;
        if (slitp != NULL && slitp->seq_data == NULL) {
          if (last_is_gap) {
            num_adjacent_gaps++;
          }
          last_is_gap = TRUE;
          num_gaps++;
        } else {
          if (! last_is_gap) {
            non_interspersed_gaps = TRUE;
          }
          last_is_gap = FALSE;
        }
      } else {
        if (! last_is_gap) {
          non_interspersed_gaps = TRUE;
        }
        last_is_gap = FALSE;
      }
    }
    if (non_interspersed_gaps && (! hasGi) && mip != NULL &&
        (mip->tech == MI_TECH_htgs_0 || mip->tech == MI_TECH_htgs_1 || mip->tech == MI_TECH_htgs_2 || mip->tech == MI_TECH_htgs_3)) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_MissingGaps, "HTGS delta seq should have gaps between all sequence runs");
    }
    if (num_adjacent_gaps > 1) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadDeltaSeq, "There are %d adjacent gaps in delta seq", (int) num_adjacent_gaps);
    } else if (num_adjacent_gaps > 0) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_BadDeltaSeq, "There is %d adjacent gap in delta seq", (int) num_adjacent_gaps);
    }
    if (vnp != NULL && vnp->choice == 2) {
      slitp = (SeqLitPtr) vnp->data.ptrvalue;
      if (slitp != NULL && slitp->seq_data == NULL) {
        ValidErr (vsp, sev, ERR_SEQ_INST_BadDeltaSeq, "Last delta seq component is a gap");
      }
    }
    if (num_gaps == 0 && mip != NULL) {
      if (/* mip->tech == MI_TECH_htgs_1 || */ mip->tech == MI_TECH_htgs_2) {
        if (VisitGraphsInSep (sep, NULL, NULL) == 0) {
          if (! isActiveFin) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_BadHTGSeq, "HTGS 2 delta seq has no gaps and no graphs");
          }
        }
      }
    }
  }

  if (bsp->repr == Seq_repr_raw) {
    if (mip != NULL) {
      if (/* mip->tech == MI_TECH_htgs_1 || */ mip->tech == MI_TECH_htgs_2) {
        if (VisitGraphsInSep (sep, NULL, NULL) == 0) {
          if (! isActiveFin) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_BadHTGSeq, "HTGS 2 raw seq has no gaps and no graphs");
          }
        }
      }
    }
  }

  if (ISA_aa (bsp->mol)) {
    if ((bsp->length <= 3) && (bsp->length >= 0) && (!isPDB)) {
      if (mip == NULL || mip->completeness < 2 || mip->completeness > 5) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_ShortSeq, "Sequence only %ld residues", (long) (bsp->length));
      }
    }

  } else {
    if ((bsp->length <= 10) && (bsp->length >= 0) && (!isPDB)) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_ShortSeq, "Sequence only %ld residues", (long) (bsp->length));
    }
  }

#if 0
  if (bsp->length > 350000 && (! isNTorNC)) {
    Boolean         isGenBankEMBLorDDBJ;
    Boolean         litHasData;
    if (bsp->repr == Seq_repr_delta) {
      isGenBankEMBLorDDBJ = FALSE;
      /* suppress this for data from genome annotation project */
      VisitBioseqsInSep (vsp->sep, (Pointer) &isGenBankEMBLorDDBJ, LookForGEDseqID);
      if (mip != NULL && isGenBankEMBLorDDBJ) {
        if (mip->tech == MI_TECH_htgs_0 || mip->tech == MI_TECH_htgs_1 || mip->tech == MI_TECH_htgs_2) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_LongHtgsSequence, "Phase 0, 1 or 2 HTGS sequence exceeds 350kbp limit");
        } else if (mip->tech == MI_TECH_htgs_3) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_SequenceExceeds350kbp, "Phase 3 HTGS sequence exceeds 350kbp limit");
        } else if (mip->tech == MI_TECH_wgs) {
          /*
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_SequenceExceeds350kbp, "WGS sequence exceeds 350kbp limit");
          */
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
          if (len > 500000 && litHasData) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_LongLiteralSequence, "Length of sequence literals exceeds 500kbp limit");
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
        } else if (mip->tech == MI_TECH_wgs) {
          /*
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_SequenceExceeds350kbp, "WGS sequence exceeds 350kbp limit");
          */
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
#endif

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
          /* check generated protein defline with first prp->name - new convention */
          if (buf != NULL && CreateDefLineExEx (&ii, bsp, buf, buflen, tech, NULL, NULL, TRUE, FALSE)) {
            if (StringICmp (buf, title) != 0) {
              /* also check generated protein defline with all prp->names - old convention */
              if (CreateDefLineExEx (&ii, bsp, buf, buflen, tech, NULL, NULL, TRUE, TRUE)) {
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
            }
          }
          MemFree (buf);
        }
      }
    }
  }

  vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &context);
  if (vnp != NULL) {
    mip = (MolInfoPtr) vnp->data.ptrvalue;
    if (mip != NULL) {
      if (mip->completeness != 1 && isGB) {
        buf = MemNew (sizeof (Char) * (4097));
        if (buf != NULL && CreateDefLine (NULL, bsp, buf, 4096, mip->tech, NULL, NULL)) {
          if (StringStr (buf, "complete genome") != NULL) {
            olditemid = gcp->itemID;
            olditemtype = gcp->thistype;
            if (vnp->extended != 0) {
              ovp = (ObjValNodePtr) vnp;
              gcp->itemID = ovp->idx.itemID;
              gcp->thistype = OBJ_SEQDESC;
            }
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_CompleteTitleProblem, "Complete genome in title without complete flag set");
            gcp->itemID = olditemid;
            gcp->thistype = olditemtype;
          }
        }
        MemFree (buf);
      }
      if (mip->completeness != 1 && bsp->topology == 2) {
        olditemid = gcp->itemID;
        olditemtype = gcp->thistype;
        if (vnp->extended != 0) {
          ovp = (ObjValNodePtr) vnp;
          gcp->itemID = ovp->idx.itemID;
          gcp->thistype = OBJ_SEQDESC;
        }
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_CompleteCircleProblem, "Circular topology without complete flag set");
        gcp->itemID = olditemid;
        gcp->thistype = olditemtype;
      }
    }
  }

  if (ISA_na (bsp->mol) && (bsp->repr == Seq_repr_raw || (bsp->repr == Seq_repr_delta && DeltaLitOnly (bsp))) && bsp->length > 10) {
    /* check for N bases at start or stop of sequence */
    sfp = (SeqFeatPtr) MemNew (sizeof (SeqFeat));
    if (sfp == NULL) return;
    sfp->data.choice = SEQFEAT_COMMENT;

    sfp->location = AddIntervalToLocation (NULL, bsp->id, 0, 9, FALSE, FALSE);
    str = GetSequenceByFeature (sfp);
    if (str != NULL) {
      if (str [0] == 'n' || str [0] == 'N') {
        if (isNC || isPatent) {
          sev = SEV_WARNING;
        } else if (StringICmp (str, "NNNNNNNNNN") == 0) {
          sev = SEV_ERROR;
        } else {
          sev = SEV_WARNING;
        }
        ValidErr (vsp, sev, ERR_SEQ_INST_TerminalNs, "N at beginning of sequence");
      }
    }
    MemFree (str);
    sfp->location = SeqLocFree (sfp->location);

    sfp->location = AddIntervalToLocation (NULL, bsp->id, bsp->length - 10, bsp->length - 1, FALSE, FALSE);
    str = GetSequenceByFeature (sfp);
    len = StringLen (str);
    if (str != NULL && len > 0) {
      if (str [len - 1] == 'n' || str [len - 1] == 'N') {
        if (isNC || isPatent) {
          sev = SEV_WARNING;
        } else if (StringICmp (str, "NNNNNNNNNN") == 0) {
          sev = SEV_ERROR;
        } else {
          sev = SEV_WARNING;
        }
        ValidErr (vsp, sev, ERR_SEQ_INST_TerminalNs, "N at end of sequence");
      }
    }
    MemFree (str);
    sfp->location = SeqLocFree (sfp->location);

    MemFree (sfp);
  }
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
        } else if (pid->choice == 5) {
          /* consortium */
          if (!HasNoText ((CharPtr) pid->data)) {
            return FALSE;
          }
        }
      }
    }
  }
  return TRUE;
}

static void LookForNC (BioseqPtr bsp, Pointer userdata)

{
  BoolPtr       is_ncp;
  SeqIdPtr      sip;
  TextSeqIdPtr  tsip;

  if (bsp == NULL || userdata == NULL) return;
  is_ncp = (BoolPtr) userdata;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER) {
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
      if (tsip != NULL && tsip->accession != NULL) {
        /*
        if (StringNICmp (tsip->accession, "NC_", 3) == 0) {
          *is_ncp = TRUE;
        }
        */
        *is_ncp = TRUE; /* any refseq now drops pubdesc message severity */
      }
    }
  }
}

static Boolean IS_REFSEQ_IN_SEP (SeqEntryPtr sep)

{
  Boolean  is_nc = FALSE;

  if (sep == NULL) return FALSE;
  VisitBioseqsInSep (sep, (Pointer) &is_nc, LookForNC);

  return is_nc;
}

static void LookForHTG (SeqDescrPtr sdp, Pointer userdata)

{
  BoolPtr     is_htgp;
  MolInfoPtr  mip;

  if (sdp == NULL || userdata == NULL) return;
  if (sdp->choice != Seq_descr_molinfo) return;

  mip = (MolInfoPtr) sdp->data.ptrvalue;
  if (mip == NULL) return;

  if (mip->tech == MI_TECH_htgs_1 ||
      mip->tech == MI_TECH_htgs_2 ||
      mip->tech == MI_TECH_htgs_3 ||
      mip->tech == MI_TECH_htgs_0) {

    is_htgp = (BoolPtr) userdata;
    *is_htgp = TRUE; /* any htg now drops citsub missing affil message severity */
  }
}

static Boolean IS_HTG_IN_SEP (SeqEntryPtr sep)

{
  Boolean  is_htg = FALSE;

  if (sep == NULL) return FALSE;
  VisitDescriptorsInSep (sep, (Pointer) &is_htg, LookForHTG);

  return is_htg;
}

static void ValidateCitSub (ValidStructPtr vsp, CitSubPtr csp)
{
  AffilPtr        ap;
  AuthListPtr     alp;
  ValNodePtr      name;
  Boolean         hasAffil = FALSE;
  Boolean         hasName = FALSE;
  ErrSev          sev;

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
    ap = alp->affil;
    if (ap != NULL) {
      if (ap->affil == NULL && ap->div == NULL && ap->street == NULL && ap->city == NULL &&
           ap->sub == NULL && ap->postal_code == NULL && ap->country == NULL &&
           ap->phone == NULL && ap->fax == NULL && ap->email == NULL) {
        /* no affiliation */
      } else {
        hasAffil = TRUE;
      }
    }
  }
  if (!hasName) {
    ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MissingPubInfo, "Submission citation has no author names");
  }
  sev = SEV_ERROR;
  if (IS_REFSEQ_IN_SEP (vsp->sep)) {
    sev = SEV_WARNING;
  }
  if (IS_HTG_IN_SEP (vsp->sep)) {
    sev = SEV_WARNING;
  }
  if (!hasAffil) {
    ValidErr (vsp, sev, ERR_GENERIC_MissingPubInfo, "Submission citation has no affiliation");
  }
}

static void LookForMultiplePubs (ValidStructPtr vsp, GatherContextPtr gcp, SeqDescrPtr sdp)

{
  Bioseq       bs;
  Boolean      collision, otherpub;
  Int4         muid, pmid;
  SeqDescrPtr  nextpub;
  PubdescPtr   pdp;
  ValNodePtr   vnp;


  if (sdp != NULL && sdp->choice == Seq_descr_pub && sdp->extended != 0 && vsp != NULL && gcp != NULL) {
    MemSet ((Pointer) &bs, 0, sizeof (Bioseq));
    pdp = (PubdescPtr) sdp->data.ptrvalue;
    if (pdp != NULL) {
      otherpub = FALSE;
      muid = 0;
      pmid = 0;
      for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
        if (vnp->choice == PUB_Muid) {
          muid = vnp->data.intvalue;
        } else if (vnp->choice == PUB_PMid) {
          pmid = vnp->data.intvalue;
        } else {
          otherpub = TRUE;
        }
      }
      if (otherpub) {
        if (muid > 0 || pmid > 0) {
          collision = FALSE;
          nextpub = GetNextDescriptorUnindexed (&bs, Seq_descr_pub, sdp);
          while (nextpub != NULL) {
            pdp = (PubdescPtr) nextpub->data.ptrvalue;
            if (pdp != NULL) {
              for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
                if (vnp->choice == PUB_Muid) {
                  if (muid > 0 && muid == vnp->data.intvalue) {
                    collision = TRUE;
                  }
                } else if (vnp->choice == PUB_PMid) {
                  if (pmid > 0 && pmid == vnp->data.intvalue) {
                    collision = TRUE;
                  }
                }
              }
            }
            nextpub = GetNextDescriptorUnindexed (&bs, Seq_descr_pub, nextpub);
          }
          if (collision) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_CollidingPublications, "Multiple publications with same identifier");
          }
        }
      }
    }
  }
}

static void LookForMultipleUnpubPubs (ValidStructPtr vsp, GatherContextPtr gcp, BioseqPtr bsp)

{
  AuthListPtr        alp;
  Char               buf [2048];
  CitGenPtr          cgp;
  CharPtr            consortium, last, str, tmp;
  SeqMgrDescContext  dcontext;
  ValNodePtr         list = NULL, next, vnp;
  PubdescPtr         pdp;
  SeqDescrPtr        sdp;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pub, &dcontext);
  while (sdp) {
    pdp = (PubdescPtr) sdp->data.ptrvalue;
    if (pdp != NULL) {
      vnp = pdp->pub;

      /* skip over just serial number */

      if (vnp != NULL && vnp->choice == PUB_Gen && vnp->next != NULL) {
        cgp = (CitGenPtr) vnp->data.ptrvalue;
        if (cgp != NULL) {
          if (StringNICmp ("BackBone id_pub", cgp->cit, 15) != 0) {
            if (cgp->cit == NULL && cgp->journal == NULL && cgp->date == NULL && cgp->serial_number) {
              vnp = vnp->next;
            }
          }
        }
      }

      if (PubLabelUnique (vnp, buf, sizeof (buf) - 1, OM_LABEL_CONTENT, TRUE) > 0) {
        alp = GetAuthListPtr (pdp, NULL);
        if (alp != NULL) {
          consortium = NULL;
          str = GetAuthorsString (GENBANK_FMT, alp, &consortium, NULL, NULL);
          tmp = MemNew (StringLen (buf) + StringLen (str) + StringLen (consortium) + 10);
          if (tmp != NULL) {
            StringCpy (tmp, buf);
            if (StringDoesHaveText (str)) {
              StringCat (tmp, "; ");
              StringCat (tmp, str);
            }
            if (StringDoesHaveText (consortium)) {
              StringCat (tmp, "; ");
              StringCat (tmp, consortium);
            }
            ValNodeAddStr (&list, 0, tmp);
          }
          MemFree (str);
          MemFree (consortium);
        }
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_pub, &dcontext);
  }

  if (list == NULL) return;

  list = ValNodeSort (list, SortVnpByString);
  last = (CharPtr) list->data.ptrvalue;
  vnp = list->next;
  while (vnp != NULL) {
    next = vnp->next;
    str = (CharPtr) vnp->data.ptrvalue;
    if (StringICmp (last, str) == 0) {
      StringNCpy_0 (buf, str, sizeof (buf));
      StringCpy (buf + 100, "...");
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_CollidingPublications,
                "Multiple equivalent publications annotated on this sequence [%s]", buf);
    } else {
      last = (CharPtr) vnp->data.ptrvalue;
    }
    vnp = next;
  }

  ValNodeFreeData (list);
}

static Boolean BadCharsInAuth (CharPtr str, CharPtr PNTR badauthor, Boolean allowcomma, Boolean allowperiod, Boolean last)
{
  Char     ch;
  CharPtr  ptr, stp = NULL;

  if (StringHasNoText (str)) return FALSE;
  if (last) {
      stp = StringISearch(str, "St.");
      if (stp == str) {
          stp += 2;  /* point to the period */
      }
  }

  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    /* success on any of these tests are allowed values */
    if (IS_ALPHA (ch)) {
    } else if (ch == '-' || ch == '\'' || ch == ' ') {
    } else if (ch == ',' && allowcomma) {
    } else if (ch == '.' && (allowperiod || stp == ptr)) {
    } else {
      /* bad character found */
      *badauthor = str;
      return TRUE;
    }
    ptr++;
    ch = *ptr;
  }

  return FALSE;
}

static Boolean BadCharsInName (ValNodePtr name, CharPtr PNTR badauthor)

{
  AuthorPtr    ap;
  NameStdPtr   nsp;
  PersonIdPtr  pid;

  if (name == NULL) return FALSE;
  ap = name->data.ptrvalue;
  if (ap == NULL) return FALSE;
  pid = ap->name;
  if (pid == NULL) return FALSE;

  if (pid->choice == 2) {
    nsp = pid->data;
    if (nsp == NULL) return FALSE;
    if (StringICmp (nsp->names [0], "et al.") == 0) return FALSE;
    if (BadCharsInAuth (nsp->names [0], badauthor, FALSE, FALSE, TRUE)) return TRUE; /* last    */
    if (BadCharsInAuth (nsp->names [1], badauthor, FALSE, FALSE, FALSE)) return TRUE; /* first    */
    if (BadCharsInAuth (nsp->names [4], badauthor, FALSE, TRUE, FALSE)) return TRUE;  /* initials */
    if (BadCharsInAuth (nsp->names [5], badauthor, FALSE, TRUE, FALSE)) return TRUE;  /* suffix */
  }

  return FALSE;
}

static void ValidatePubdesc (ValidStructPtr vsp, GatherContextPtr gcp, PubdescPtr pdp)
{
  AuthListPtr     alp;
  CharPtr         badauthor;
  CitArtPtr       cap = NULL;
  CitGenPtr       cgp;
  CitJourPtr      cjp = NULL;
  CitSubPtr       csp;
  DatePtr         dp;
  Boolean         hasName, hasTitle, hasIsoJTA = FALSE,
                  inPress = FALSE, electronic_journal = FALSE,
                  conflicting_pmids = FALSE, redundant_pmids = FALSE,
                  conflicting_muids = FALSE, redundant_muids = FALSE,
                  is_nc = FALSE;
  ImprintPtr      imp;
  Int4            muid = 0;
  Boolean         noVol, noPages;
  ValNodePtr      name;
  Int4            pmid = 0;
  CharPtr         ptr;
  ErrSev          sev;
  Int4            start;
  Int4            stop;
  Char            temp [64];
  ValNodePtr      title;
  Int4            uid = 0;
  long int        val;
  ValNodePtr      vnp;

  if (vsp == NULL || pdp == NULL)
    return;
  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
    case PUB_Gen:
      cgp = (CitGenPtr) vnp->data.ptrvalue;
      if (cgp != NULL) {
        if (!StringHasNoText (cgp->cit)) {
          if (StringNICmp (cgp->cit, "submitted", 8) == 0 ||
              StringNICmp (cgp->cit, "unpublished", 11) == 0 ||
              StringNICmp (cgp->cit, "Online Publication", 18) == 0 ||
              StringNICmp (cgp->cit, "Published Only in DataBase", 26) == 0) {
          } else {
            ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MissingPubInfo, "Unpublished citation text invalid");
          }
        }
      }
      break;
    case PUB_Muid:
      if (pmid == 0) {
        pmid = vnp->data.intvalue;
      } else if (pmid != vnp->data.intvalue) {
        conflicting_pmids = TRUE;
      } else {
        redundant_pmids = TRUE;
      }
      if (uid == 0) {
        uid = vnp->data.intvalue;
      }
      break;
    case PUB_PMid:
      if (muid == 0) {
        muid = vnp->data.intvalue;
      } else if (muid != vnp->data.intvalue) {
        conflicting_muids = TRUE;
      } else {
        redundant_muids = TRUE;
      }
      if (uid == 0) {
        uid = vnp->data.intvalue;
      }
      break;
    case PUB_Sub :
      csp = (CitSubPtr) vnp->data.ptrvalue;
      if (csp != NULL) {
        ValidateCitSub (vsp, csp);
      }
      break;
    case PUB_Medline:
      ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MedlineEntryPub, "Publication is medline entry");
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
            if (title->choice == Cit_title_iso_jta) {
              hasIsoJTA = TRUE;
            }
            if (!HasNoText ((CharPtr) title->data.ptrvalue)) {
              hasTitle = TRUE;
              if (title->choice == Cit_title_name) {
                if (StringNCmp ((CharPtr) title->data.ptrvalue, "(er)", 4) == 0) {
                  electronic_journal = TRUE;
                }
              }
            }
          }
          if (!hasTitle) {
            ValidErr (vsp, SEV_ERROR, ERR_GENERIC_MissingPubInfo, "Journal title missing");
          }
          imp = cjp->imp;
          if (imp != NULL) {
            if (imp->pubstatus == PUBSTATUS_epublish || imp->pubstatus == PUBSTATUS_aheadofprint) {
              electronic_journal = TRUE;
            }
            if (imp->prepub == 2) {
              inPress = TRUE;
            }
            if (imp->prepub == 0 && imp->pubstatus != PUBSTATUS_aheadofprint) {
              noVol = StringHasNoText (imp->volume);
              noPages = StringHasNoText (imp->pages);
              sev = SEV_ERROR;
              if (IS_REFSEQ_IN_SEP (vsp->sep)) {
                sev = SEV_WARNING;
              }
              if (noVol && noPages) {
                ValidErr (vsp, sev, ERR_GENERIC_MissingPubInfo, "Journal volume and pages missing");
              } else if (noVol) {
                if (! electronic_journal) {
                  ValidErr (vsp, sev, ERR_GENERIC_MissingPubInfo, "Journal volume missing");
                }
              } else if (noPages) {
                ValidErr (vsp, sev, ERR_GENERIC_MissingPubInfo, "Journal pages missing");
              }
              if (! noPages) {
                sev = SEV_WARNING;
                StringNCpy_0 (temp, imp->pages, sizeof (temp));
                ptr = StringChr (temp, '-');
                if (ptr != NULL) {
                  *ptr = '\0';
                  ptr++;
                  if (sscanf (temp, "%ld", &val) == 1) {
                    start = (Int4) val;
                    if (sscanf (ptr, "%ld", &val) == 1) {
                      stop = (Int4) val;
                      if (start == 0 || stop == 0) {
                        ValidErr (vsp, sev, ERR_GENERIC_BadPageNumbering, "Page numbering has zero value");
                      } else if (start < 0 || stop < 0) {
                        ValidErr (vsp, sev, ERR_GENERIC_BadPageNumbering, "Page numbering has negative value");
                      } else if (start > stop) {
                        ValidErr (vsp, sev, ERR_GENERIC_BadPageNumbering, "Page numbering out of order");
                      } else if (stop > start + 50) {
                        ValidErr (vsp, sev, ERR_GENERIC_BadPageNumbering, "Page numbering greater than 50");
                      }
                    } else {
                      ValidErr (vsp, sev, ERR_GENERIC_BadPageNumbering, "Page numbering stop looks strange");
                    }
                  } else if (! IS_ALPHA (temp [0])) {
                    ValidErr (vsp, sev, ERR_GENERIC_BadPageNumbering, "Page numbering start looks strange");
                  }
                }
              }
              dp = imp->date;
              if (dp == NULL) {
                ValidErr (vsp, SEV_WARNING, ERR_GENERIC_MissingPubInfo, "Publication date missing");
              } else if (dp->str != NULL) {
                if (StringCmp (dp->str, "?") == 0) {
                  ValidErr (vsp, SEV_WARNING, ERR_GENERIC_MissingPubInfo, "Publication date marked as '?'");
                }
              } else if (dp->data [1] == 0) {
                ValidErr (vsp, SEV_WARNING, ERR_GENERIC_MissingPubInfo, "Publication date not set");
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

  if (conflicting_pmids) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_CollidingPublications, "Multiple conflicting pmids in a single publication");
  } else if (redundant_pmids) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_CollidingPublications, "Multiple redundant pmids in a single publication");
  }
  if (conflicting_muids) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_CollidingPublications, "Multiple conflicting muids in a single publication");
  } else if (redundant_muids) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_CollidingPublications, "Multiple redundant muids in a single publication");
  }

  if (cap != NULL && cjp != NULL && (uid > 0 || inPress || vsp->alwaysRequireIsoJTA)) {
    if (! hasIsoJTA) {
      if (! electronic_journal) {
        ValidErr (vsp, SEV_WARNING, ERR_GENERIC_MissingPubInfo, "ISO journal title abbreviation missing");
      }
    }
  }

  alp = GetAuthListPtr (pdp, NULL);
  if (alp != NULL) {
    sev = SEV_ERROR;
    if (IS_REFSEQ_IN_SEP (vsp->sep)) {
      sev = SEV_WARNING;
    }
    if (alp->choice == 1) {
      for (name = alp->names; name != NULL; name = name->next) {
        badauthor = NULL;
        if (BadCharsInName (name, &badauthor)) {
          if (StringHasNoText (badauthor)) {
            badauthor = "?";
          }
          ValidErr (vsp, sev, ERR_SEQ_FEAT_BadCharInAuthorName, "Bad characters in author %s", badauthor);
        }
      }
    } else if (alp->choice == 2 || alp->choice == 3) {
      for (name = alp->names; name != NULL; name = name->next) {
        badauthor = NULL;
        if (BadCharsInAuth ((CharPtr) name->data.ptrvalue, &badauthor, TRUE, TRUE, FALSE)) {
          if (StringHasNoText (badauthor)) {
            badauthor = "?";
          }
          ValidErr (vsp, sev, ERR_SEQ_FEAT_BadCharInAuthorName, "Bad characters in author%s", badauthor);
        }
      }
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
  Boolean         is_aa;         /* bioseq is protein? */
  Boolean         is_mrna;       /* molinfo is mrna? */
  Boolean         is_prerna;     /* molinfo is precursor rna? */
  Boolean         is_artificial; /* biosource origin is artificial? */
  Boolean         is_syn_constr; /* is organism name synthetic construct? */
  Boolean         got_a_pub;
  int             last_na_mol, last_na_mod, last_organelle, last_partialness, last_left_right, last_biomol, last_tech, last_completeness, num_full_length_src_feat,     /* number full length src feats */
                  num_full_length_prot_ref;
  ValNodePtr      last_gb, last_embl, last_prf, last_pir, last_sp, last_pdb, last_create, last_update, last_biosrc, last_orgref;
  OrgRefPtr       last_org;
  GatherContextPtr gcp;
  BioseqPtr        bsp;
}
BioseqValidStr , PNTR BioseqValidStrPtr;

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

/*****************************************************************************
*
*   ValidateSeqFeatContext(gcp)
*      Gather callback helper function for validating context on a Bioseq
*
*****************************************************************************/
static Boolean ValidateSeqFeatCommon (SeqFeatPtr sfp, BioseqValidStrPtr bvsp, ValidStructPtr vsp,
                                      Int4 left, Int4 right, Int2 numivals, Uint2 featitemid, Boolean farloc, BioseqPtr bsp)
{
  BioseqSetPtr    bssp;
  Boolean         do_error;
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
  Boolean         is_refseq = FALSE;
  ErrSev          sev;


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
        is_refseq = TRUE;
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
    case SEQFEAT_GENE:
        if (bsp != NULL) {
          do_error = FALSE;
          if (bsp->idx.parenttype == OBJ_BIOSEQSET) {
            bssp = (BioseqSetPtr) bsp->idx.parentptr;
            while (bssp != NULL) {
              switch (bssp->_class) {
              case BioseqseqSet_class_nuc_prot :
              case BioseqseqSet_class_mut_set :
              case BioseqseqSet_class_pop_set :
              case BioseqseqSet_class_phy_set :
              case BioseqseqSet_class_eco_set :
              case BioseqseqSet_class_gen_prod_set :
                do_error = TRUE;
                break;
              default :
                break;
              }
              if (bssp->idx.parenttype == OBJ_BIOSEQSET) {
                bssp = (BioseqSetPtr) bssp->idx.parentptr;
              } else {
                bssp = NULL;
              }
            }
          }
          if (do_error) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidForType, "Invalid feature for a protein Bioseq.");
          }
        }
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
    case SEQFEAT_CDREGION:
      if (numivals > 1) {
        if ((! sfp->excpt) ||
            (StringISearch (sfp->except_text, "ribosomal slippage") == NULL)) {
          sev = SEV_ERROR;
          if (is_refseq) {
            sev = SEV_WARNING;
          }
          ValidErr (vsp, sev, ERR_SEQ_FEAT_InvalidForType, "Multi-interval CDS feature is invalid on an mRNA (cDNA) Bioseq.");
        }
      }
      break;
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
      sev = SEV_ERROR;
      if (is_nc) {
        sev = SEV_WARNING;
      }
      if (! DeltaOrFarSeg (vsp->sep, sfp->location)) {
        ValidErr (vsp, sev, ERR_SEQ_FEAT_LocOnSegmentedBioseq, "Feature location on segmented bioseq, not on parts");
      }
    }
  }

  if (gcp != NULL) {
    gcp->itemID = olditemid;
    gcp->thistype = olditemtype;
  }

  return TRUE;
}

static Boolean GeneSpansOrigin (SeqMgrFeatContextPtr context, Int4 bsplength)

{
  Int4Ptr  ivals;

  if (context == NULL || bsplength < 1) return FALSE;
  ivals = context->ivals;
  if (ivals == NULL || context->numivals != 2) return FALSE;
  if (context->strand == Seq_strand_minus) {
    if (ivals [1] == 0 && ivals [2] == bsplength - 1) return TRUE;
  } else {
    if (ivals [2] == 0 && ivals [1] == bsplength - 1) return TRUE;
  }
  return FALSE;
}

static void CheckMultiIntervalGene (SeqFeatPtr sfp, SeqMgrFeatContextPtr context, ValidStructPtr vsp, GatherContextPtr gcp)

{
  BioseqPtr     bsp;
  Uint2         olditemtype = 0;
  Uint2         olditemid = 0;
  ErrSev        sev = SEV_ERROR;
  SeqIdPtr      sip;
  TextSeqIdPtr  tsip;

  if (sfp == NULL || context == NULL || vsp == NULL) return;
  if (SeqLocId (sfp->location) == NULL) return;
  if (context->numivals < 2) return;

  bsp = context->bsp;
  if (bsp != NULL) {
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_OTHER) {
        tsip = (TextSeqIdPtr) sip->data.ptrvalue;
        if (tsip != NULL && tsip->accession != NULL) {
          if (StringNICmp (tsip->accession, "NC_", 3) == 0) {
            sev = SEV_WARNING;
          }
        }
      }
    }
    if (bsp->topology == 2) {
      if (context->numivals == 2 && GeneSpansOrigin (context, bsp->length)) return;
      sev = SEV_WARNING;
    }
  }

  if (gcp != NULL) {
    olditemid = gcp->itemID;
    olditemtype = gcp->thistype;
    gcp->itemID = context->itemID;
    gcp->thistype = OBJ_SEQFEAT;
  }

  ValidErr (vsp, sev, ERR_SEQ_FEAT_MultiIntervalGene,
            "Gene feature on non-segmented sequence should not have multiple intervals");

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

  return ValidateSeqFeatCommon (sfp, bvsp, vsp, context->left, context->right, context->numivals, context->itemID, context->farloc, context->bsp);
}

static void ValidateSeqFeatContext (GatherContextPtr gcp)
{
  ValidStructPtr  vsp;
  BioseqValidStrPtr bvsp;
  SeqFeatPtr      sfp;

  bvsp = (BioseqValidStrPtr) (gcp->userdata);
  vsp = bvsp->vsp;
  sfp = (SeqFeatPtr) (gcp->thisitem);

  ValidateSeqFeatCommon (sfp, bvsp, vsp, gcp->extremes.left, gcp->extremes.right, 0, 0, FALSE, NULL);
}

/*****************************************************************************
*
*   CountryIsValid(name)
*      Validates subsource country against official country names
*
*****************************************************************************/

static CharPtr  Nlm_valid_country_codes [] = {
  "Afghanistan",
  "Albania",
  "Algeria",
  "American Samoa",
  "Andorra",
  "Angola",
  "Anguilla",
  "Antarctica",
  "Antigua and Barbuda",
  "Arctic Ocean",
  "Argentina",
  "Armenia",
  "Aruba",
  "Ashmore and Cartier Islands",
  "Atlantic Ocean",
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
  "East Timor",
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
  "Indian Ocean",
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
  "Kerguelen Archipelago",
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
  "Pacific Ocean",
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
  "Serbia and Montenegro",
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
  "Zimbabwe",
  NULL
};

NLM_EXTERN CharPtr PNTR GetValidCountryList (void)

{
  return (CharPtr PNTR) Nlm_valid_country_codes;
}

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
  R = sizeof (Nlm_valid_country_codes) / sizeof (Nlm_valid_country_codes[0]) - 1; /* -1 because now NULL terminated */

  while (L < R) {
    mid = (L + R) / 2;
    if (StringICmp (Nlm_valid_country_codes[mid], str) < 0) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }

  if (StringICmp (Nlm_valid_country_codes[R], str) == 0) {
    return TRUE;
  }

  return FALSE;
}

static CharPtr source_qual_prefixes [] = {
  "acronym:",
  "anamorph:",
  "authority:",
  "biotype:",
  "biovar:",
  "breed:",
  "cell_line:",
  "cell_type:",
  "chemovar:",
  "chromosome:",
  "clone:",
  "clone_lib:",
  "collected_by:",
  "collection_date:",
  "common:",
  "country:",
  "cultivar:",
  "dev_stage:",
  "dosage:",
  "ecotype:",
  "endogenous_virus_name:",
  "environmental_sample:",
  "forma:",
  "forma_specialis:",
  "frequency:",
  "genotype:",
  "germline:",
  "group:",
  "haplotype:",
  "identified_by:",
  "insertion_seq_name:",
  "isolate:",
  "isolation_source:",
  "lab_host:",
  "lat_lon:"
  "left_primer:",
  "map:",
  "nat_host:",
  "pathovar:",
  "plasmid_name:",
  "plastid_name:",
  "pop_variant:",
  "rearranged:",
  "right_primer:",
  "segment:",
  "serogroup:",
  "serotype:",
  "serovar:",
  "sex:",
  "specimen_voucher:",
  "strain:",
  "subclone:",
  "subgroup:",
  "substrain:",
  "subtype:",
  "sub_species:",
  "synonym:",
  "taxon:",
  "teleomorph:",
  "tissue_lib:",
  "tissue_type:",
  "transgenic:",
  "transposon_name:",
  "type:",
  "variety:",
  NULL
};

static void InitializeSourceQualTags (ValidStructPtr vsp)

{
  Int2  i;

  vsp->sourceQualTags = TextFsaNew ();
  for (i = 0; source_qual_prefixes [i] != NULL; i++) {
    TextFsaAdd (vsp->sourceQualTags, source_qual_prefixes [i]);
  }
}

static void ValidateSourceQualTags (ValidStructPtr vsp, GatherContextPtr gcp, BioSourcePtr biop, CharPtr str)

{
  Char        ch;
  CharPtr     hit;
  Boolean     okay;
  CharPtr     ptr;
  CharPtr     tmp;
  Int2        state;
  ValNodePtr  matches;

  if (vsp->sourceQualTags == NULL || StringHasNoText (str)) return;
  state = 0;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    matches = NULL;
    state = TextFsaNext (vsp->sourceQualTags, state, ch, &matches);
    if (matches != NULL) {
      hit = (CharPtr) matches->data.ptrvalue;
      if (StringHasNoText (hit)) {
        hit = "?";
      }
      okay = TRUE;
      tmp = ptr - StringLen (hit);
      if (tmp > str) {
        ch = *tmp;
        if ((! IS_WHITESP (ch)) && ch != ';') {
          okay = FALSE;
        }
      }
      if (okay) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_StructuredSourceNote,
                  "Source note has structured tag %s", hit);
      }
    }
    ptr++;
    ch = *ptr;
  }
}

/*****************************************************************************
*
*   ValidateSeqDescrContext(gcp)
*      Gather callback helper function for validating context on a Bioseq
*
*****************************************************************************/
static void ValidateBioSource (ValidStructPtr vsp, GatherContextPtr gcp, BioSourcePtr biop, SeqFeatPtr sfp)
{
  CharPtr         casecounts;            
  Boolean         chromconf = FALSE;
  Int2            chromcount = 0;
  SubSourcePtr    chromosome = NULL;
  CharPtr         countryname;
  ValNodePtr      db;
  DbtagPtr        dbt;
  Boolean         germline = FALSE;
  Boolean         has_strain = FALSE;
  Int2            i;
  Int4            id;
  Boolean         is_env_sample = FALSE;
  Boolean         is_transgenic = FALSE;
  OrgNamePtr      onp;
  OrgModPtr       omp;
  OrgRefPtr       orp;
  Boolean         rearranged = FALSE;
  SubSourcePtr    ssp;

  if (vsp->sourceQualTags == NULL) {
    InitializeSourceQualTags (vsp);
  }
  if (biop == NULL)
    return;
  if (biop->genome == GENOME_transposon || biop->genome == GENOME_insertion_seq) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_ObsoleteSourceLocation,
              "Transposon and insertion sequence are no longer legal locations");
  }
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
    } else if (ssp->subtype == SUBSRC_transposon_name || ssp->subtype == SUBSRC_insertion_seq_name) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_ObsoleteSourceQual,
                "Transposon name and insertion sequence name are no longer legal qualifiers");
    } else if (ssp->subtype == 0) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadSubSource, "Unknown subsource subtype %d", (int) (ssp->subtype));
    } else if (ssp->subtype == SUBSRC_other) {
      ValidateSourceQualTags (vsp, gcp, biop, ssp->name);
    } else if (ssp->subtype == SUBSRC_germline) {
      germline = TRUE;
    } else if (ssp->subtype == SUBSRC_rearranged) {
      rearranged = TRUE;
    } else if (ssp->subtype == SUBSRC_transgenic) {
      is_transgenic = TRUE;
    } else if (ssp->subtype == SUBSRC_environmental_sample) {
      is_env_sample = TRUE;
    } else if (ssp->subtype == SUBSRC_plasmid_name) {
      if (biop->genome != GENOME_plasmid) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BioSourceInconsistency, "Plasmid subsource but not plasmid location");
      }
    }
    ssp = ssp->next;
  }
  if (germline && rearranged) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadSubSource, "Germline and rearranged should not both be present");
  }
  if (is_transgenic && is_env_sample) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadSubSource, "Transgenic and environmental sample should not both be present");
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
    if (! vsp->seqSubmitParent) { /* suppress when validator run from tbl2asn */
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_MissingLineage, "No lineage for this BioSource.");
    }
  } else {
    if (biop->genome == GENOME_kinetoplast) {
      if (StringStr (onp->lineage, "Kinetoplastida") == 0) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadOrganelle, "Only Kinetoplastida have kinetoplasts");
      }
    } else if (biop->genome == GENOME_nucleomorph) {
      if (StringStr (onp->lineage, "Chlorarachniophyceae") == 0 && StringStr (onp->lineage, "Cryptophyta") == 0) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadOrganelle, "Only Chlorarachniophyceae and Cryptophyta have nucleomorphs");
      }
    }
  }
  if (onp != NULL) {
    omp = onp->mod;
    while (omp != NULL) {
      if (omp->subtype == 0 || omp->subtype == 1) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadOrgMod, "Unknown orgmod subtype %d", (int) (omp->subtype));
      } else if (omp->subtype == ORGMOD_strain) {
        has_strain = TRUE;
      } else if (omp->subtype == ORGMOD_variety) {
        if ((! StringHasNoText (onp->div)) && StringICmp (onp->div, "PLN") != 0) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadOrgMod, "Orgmod variety should only be in plants or fungi");
        }
      } else if (omp->subtype == ORGMOD_other) {
        ValidateSourceQualTags (vsp, gcp, biop, omp->subname);
      }
      omp = omp->next;
    }
  }
  if (is_env_sample && has_strain) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_BadOrgMod, "Strain should not be present in an environmental sample");
  }

  for (db = orp->db; db != NULL; db = db->next) {
    id = -1;
    dbt = (DbtagPtr) db->data.ptrvalue;
    if (dbt != NULL && dbt->db != NULL) {
      casecounts = NULL;
      for (i = 0; legalDbXrefs [i] != NULL; i++) {
        if (StringCmp (dbt->db, legalDbXrefs [i]) == 0) {
          id = i;
          break;
        } else if (StringICmp (dbt->db, legalDbXrefs [i]) == 0) {
          casecounts = legalDbXrefs [i];
        }
      }
      if (id == -1 || id < 4) {
        if (StringDoesHaveText (casecounts)) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_IllegalDbXref, "Illegal db_xref type %s, legal capitalization is %s", dbt->db, casecounts);
        } else {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_IllegalDbXref, "Illegal db_xref type %s", dbt->db);
        }
      }
    }
  }

  if (sfp != NULL) {
    for (db = sfp->dbxref; db != NULL; db = db->next) {
      dbt = (DbtagPtr) db->data.ptrvalue;
      if (dbt != NULL) {
        if (StringICmp (dbt->db, "taxon") == 0) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_TaxonDbxrefOnFeature, "BioSource feature has taxon xref in common feature db_xref list");
        }
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
  if (! vsp->seqSubmitParent) { /* suppress when validator run from tbl2asn */
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_NoTaxonID, "BioSource is missing taxon ID");
  }
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

static Boolean IsSynthetic (BioseqPtr bsp)

{
  BioSourcePtr       biop;
  SeqMgrDescContext  dcontext;
  OrgNamePtr         onp;
  OrgRefPtr          orp;
  SeqDescrPtr        sdp;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp == NULL) return FALSE;
  biop = (BioSourcePtr) sdp->data.ptrvalue;
  if (biop == NULL) return FALSE;
  if (biop->origin == 5) return TRUE;
  orp = biop->org;
  if (orp == NULL) return FALSE;
  onp = orp->orgname;
  if (onp == NULL) return FALSE;
  if (StringICmp (onp->div, "SYN") == 0) return TRUE;
  return FALSE;
}

static Boolean IsMicroRNA (BioseqPtr bsp)

{
  SeqMgrFeatContext  fcontext;
  RnaRefPtr          rrp;
  SeqFeatPtr         sfp;
  CharPtr            str;

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, FEATDEF_otherRNA, &fcontext);
  while (sfp != NULL) {
    if (sfp->data.choice == SEQFEAT_RNA) {
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp != NULL && rrp->ext.choice == 1) {
        str = (CharPtr) rrp->ext.value.ptrvalue;
        if (StringStr (str, "microRNA") != NULL) return TRUE;
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, FEATDEF_otherRNA, &fcontext);
  }
  return FALSE;
}

static Boolean IsOtherDNA (BioseqPtr bsp)

{
  SeqMgrDescContext  dcontext;
  MolInfoPtr         mip;
  SeqDescrPtr        sdp;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp == NULL) return FALSE;
  mip = (MolInfoPtr) sdp->data.ptrvalue;
  if (mip == NULL) return FALSE;
  if (mip->biomol == 255) return TRUE;
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
  SeqFeatPtr      sfp;
  BioseqPtr       bsp;
  SeqMgrFeatContext  fcontext;
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
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Nucleic acid with GIBB-mol = peptide");
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
    bsp = bvsp->bsp;
    if (biop != NULL && biop->is_focus && bsp != NULL) {
      if (ISA_aa (bsp->mol) || bsp->repr == Seq_repr_seg || SeqMgrGetParentOfPart (bsp, NULL) != NULL) {
        /* skip proteins, segmented bioseqs, or segmented parts */
      } else {
        sfp = SeqMgrGetNextFeature (bvsp->bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext);
        if (sfp == NULL) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_UnnecessaryBioSourceFocus, "BioSource descriptor has focus, but no BioSource feature");
        }
      }
    }
    if (biop != NULL && biop->origin == 5) {
      bsp = bvsp->bsp;
      if (! IsOtherDNA (bsp)) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_InvalidForType, "Molinfo-biomol other should be used if Biosource-location is synthetic");
      }
    }
    /* ValidateBioSource (vsp, gcp, biop, NULL); */
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
  case Seq_descr_title:
    for (vnp2 = vnp->next; vnp2 != NULL; vnp2 = vnp2->next) {
      if (vnp2->choice == Seq_descr_title) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_MultipleTitles, "Undesired multiple title descriptors");
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
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Nucleic acid with Molinfo-biomol = peptide");          
        }
        break;
      case MOLECULE_TYPE_OTHER_GENETIC_MATERIAL:
        if (! bvsp->is_artificial) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_InvalidForType, "Molinfo-biomol = other genetic");
        }
        break;
      case 0:                  /* unknown */
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_InvalidForType, "Molinfo-biomol unknown used");
        break;
      case 255:                /* other */
        if (! IsXr (vnp)) {
          bsp = bvsp->bsp;
          if (! IsSynthetic (bsp)) {
            if (! IsMicroRNA (bsp)) {
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_InvalidForType, "Molinfo-biomol other used");
            }
          }
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
      if (mip->biomol != MOLECULE_TYPE_OTHER_GENETIC_MATERIAL && mip->biomol != MOLECULE_TYPE_PEPTIDE && bvsp->is_artificial) {
        if (! bvsp->is_syn_constr) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_InvalidForType, "artificial should have other-genetic and synthetic construct");
        }
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
        case MI_TECH_wgs:
        case MI_TECH_barcode:
        case MI_TECH_composite_wgs_htgs:
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

static Boolean FlybaseDbxrefs (ValNodePtr vnp)

{
  DbtagPtr  dbt;

  while (vnp != NULL) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL) {
      if (StringCmp (dbt->db, "FLYBASE") == 0 || StringCmp (dbt->db, "FlyBase") == 0) {
        return TRUE;
      }
    }
    vnp = vnp->next;
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
          } else if (StringNICmp (tsip->accession, "NW_", 3) == 0) {
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

static Boolean IsNCorNT (SeqEntryPtr sep, SeqLocPtr location)
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
          } else if (StringNICmp (tsip->accession, "NT_", 3) == 0) {
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
    if (StringISearch (sfp->except_text, "alternative processing") != NULL)
      return FALSE;
  }
  if (last != NULL && last->excpt) {
    if (StringISearch (last->except_text, "alternative processing") != NULL)
      return FALSE;
  }
  return TRUE;
}

static Boolean DescsSame (AnnotDescrPtr adp1, AnnotDescrPtr adp2)

{
  if (adp1 == NULL || adp2 == NULL) return TRUE;
  if (adp1->choice != adp2->choice) return FALSE;
  if (adp1->choice == Annot_descr_name || adp1->choice == Annot_descr_title) {
    if (StringICmp ((CharPtr) adp1->data.ptrvalue, (CharPtr) adp2->data.ptrvalue) == 0) return TRUE;
  }
  return FALSE;
}

typedef struct gmcdata {
  SeqFeatPtr  gene;
  SeqFeatPtr  feat;
} GmcData, PNTR GmcDataPtr;

static int LIBCALLBACK SortGmcByGenePtr (
  VoidPtr vp1,
  VoidPtr vp2
)

{
  GmcDataPtr gdp1, gdp2;

  if (vp1 == NULL || vp2 == NULL) return 0;
  gdp1 = (GmcDataPtr) vp1;
  gdp2 = (GmcDataPtr) vp2;
  if (gdp1 == NULL || gdp2 == NULL) return 0;

  if (gdp1->gene > gdp2->gene) return -1;
  if (gdp1->gene < gdp2->gene) return 1;

  if (gdp1->feat > gdp2->feat) return -1;
  if (gdp1->feat < gdp2->feat) return 1;

  return 0;
}

static void ValidateLocusTagGeneral (ValidStructPtr vsp, BioseqPtr bsp)

{
  DbtagPtr           dbt;
  SeqMgrFeatContext  fcontext;
  GatherContextPtr   gcp;
  SeqFeatPtr         gene;
  GeneRefPtr         grp;
  ObjectIdPtr        oip;
  Uint2              olditemtype = 0;
  Uint2              olditemid = 0;
  BioseqPtr          prod;
  CharPtr            ptr;
  SeqFeatPtr         sfp;
  SeqIdPtr           sip;
  Char               tmp [64];

  if (vsp == NULL || bsp == NULL) return;
  if (! ISA_na (bsp->mol)) return;

  gcp = vsp->gcp;
  if (gcp != NULL) {
    olditemid = gcp->itemID;
    olditemtype = gcp->thistype;
  }

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL) {
    if (sfp->idx.subtype == FEATDEF_CDS || sfp->idx.subtype == FEATDEF_mRNA) {
      grp = SeqMgrGetGeneXref (sfp);
      if (! SeqMgrGeneIsSuppressed (grp)) {
        if (grp == NULL) {
          gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
          if (gene != NULL) {
            grp = (GeneRefPtr) sfp->data.value.ptrvalue;
          }
        }
        if (grp != NULL && StringDoesHaveText (grp->locus_tag)) {
          prod = BioseqFindFromSeqLoc (sfp->product);
          if (prod != NULL) {
            for (sip = prod->id; sip != NULL; sip = sip->next) {
              if (sip->choice != SEQID_GENERAL) continue;
              dbt = (DbtagPtr) sip->data.ptrvalue;
              if (dbt == NULL) continue;
              if (StringICmp (dbt->db, "TMSMART") == 0) continue;
              if (StringICmp (dbt->db, "BankIt") == 0) continue;
              oip = dbt->tag;
              if (oip == NULL) continue;
              if (StringHasNoText (oip->str)) continue;
              StringNCpy_0 (tmp, oip->str, sizeof (tmp));
              ptr = StringChr (tmp, '-');
              if (ptr != NULL) {
                *ptr = '\0';
              }
              if (StringICmp (grp->locus_tag, tmp) != 0) {
                if (gcp != NULL) {
                  gcp->itemID = sfp->idx.itemID;
                  gcp->thistype = OBJ_SEQFEAT;
                }
                vsp->descr = NULL;
                vsp->sfp = sfp;
                ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_LocusTagProductMismatch, "Gene locus_tag does not match general ID of product");
              }
            }
          }
        }
      }
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }

  if (gcp != NULL) {
    gcp->itemID = olditemid;
    gcp->thistype = olditemtype;
  }
}

static Boolean LIBCALLBACK DummyCM121Proc (
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr context
)

{
  return TRUE;
}

static void ValidateCDSmRNAmatch (ValidStructPtr vsp, BioseqPtr bsp)

{
  SeqMgrFeatContext  fcontext;
  GatherContextPtr   gcp;
  GmcDataPtr         gdp, head;
  SeqFeatPtr         gene;
  GeneRefPtr         grp;
  Int2               i, j, k, numgene, numcds, nummrna, numfeats, count;
  Boolean            cdsMrnaOneToOne = TRUE;
  Uint2              olditemtype = 0;
  Uint2              olditemid = 0;
  ErrSev             sev = /* SEV_INFO */ SEV_WARNING;
  SeqFeatPtr         sfp;
  SeqIdPtr           sip;

  if (vsp == NULL || bsp == NULL) return;
  if (! ISA_na (bsp->mol)) return;

  gcp = vsp->gcp;
  if (gcp != NULL) {
    olditemid = gcp->itemID;
    olditemtype = gcp->thistype;
  }

  /*
  if (GetAppProperty ("ValidateCDSmRNAoneToOne") != NULL) {
    cdsMrnaOneToOne = TRUE;
  }
  */

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER) {
      sev = SEV_WARNING;
    }
  }

  numgene = 0;
  numcds = 0;
  nummrna = 0;

  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL) {
    switch (sfp->idx.subtype) {
      case FEATDEF_GENE :
        numgene++;
        break;
      case FEATDEF_CDS :
        numcds++;
        break;
      case FEATDEF_mRNA :
        nummrna++;
        break;
      default :
        break;
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }

  if (numgene > 0 && numcds > 0 && nummrna > 0) {
    numfeats = numcds + nummrna;
    head = (GmcDataPtr) MemNew (sizeof (GmcData) * (size_t) (numfeats + 1));
    if (head != NULL) {
      gdp = head;
      sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
      while (sfp != NULL) {
        if (sfp->idx.subtype == FEATDEF_CDS || sfp->idx.subtype == FEATDEF_mRNA) {
          gdp->feat = sfp;
          grp = SeqMgrGetGeneXref (sfp);
          if (grp == NULL) {
            gdp->gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
          } else if (! SeqMgrGeneIsSuppressed (grp)) {
            if (StringDoesHaveText (grp->locus)) {
              gdp->gene = SeqMgrGetFeatureByLabel (bsp, grp->locus, SEQFEAT_GENE, 0, NULL);
            } else if (StringDoesHaveText (grp->locus_tag)) {
              gdp->gene = SeqMgrGetFeatureByLabel (bsp, grp->locus_tag, SEQFEAT_GENE, 0, NULL);
            }
          }
          gdp++;
        }
        sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
      }
      HeapSort (head, (size_t) numfeats, sizeof (GmcData), SortGmcByGenePtr);
      for (i = 0; i < numfeats; i += j) {
        gene = head [i].gene;
        for (j = 1; i + j < numfeats && gene == head [i + j].gene; j++) continue;
        if (j > 1 && gene != NULL) {
          /* is alt splicing */
          numcds = 0;
          nummrna = 0;
          for (k = 0; k < j; k++) {
            sfp = head [i + k].feat;
            if (sfp == NULL) continue;
            if (sfp->idx.subtype == FEATDEF_CDS) {
              numcds++;
            }
            if (sfp->idx.subtype == FEATDEF_mRNA) {
              nummrna++;
            }
          }
          if (numcds > 0 && nummrna > 1 && numcds != nummrna) {

            if (gcp != NULL) {
              gcp->itemID = gene->idx.itemID;
              gcp->thistype = OBJ_SEQFEAT;
            }
            vsp->descr = NULL;
            vsp->sfp = gene;
            ValidErr (vsp, sev, ERR_SEQ_FEAT_CDSmRNAmismatch, "mRNA count (%d) does not match CDS (%d) count for gene", (int) nummrna, (int) numcds);
          }
        }
      }
    }
    MemFree (head);
  }

  if (cdsMrnaOneToOne) {
    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &fcontext);
    while (sfp != NULL) {
      count = SeqMgrGetAllOverlappingFeatures (sfp->location, FEATDEF_mRNA, NULL, 0,
                                               CHECK_INTERVALS, NULL, DummyCM121Proc);
      if (count > 1) {
        if (gcp != NULL) {
          gcp->itemID = sfp->idx.itemID;
          gcp->thistype = OBJ_SEQFEAT;
        }
        vsp->descr = NULL;
        vsp->sfp = sfp;
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_CDSwithMultipleMRNAs, "CDS overlapped by %d mRNAs", (int) count);
      }
      sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_CDREGION, 0, &fcontext);
    }
  }

  if (gcp != NULL) {
    gcp->itemID = olditemid;
    gcp->thistype = olditemtype;
  }
}

static Boolean ReplaceQualsDiffer (GBQualPtr sfpqual, GBQualPtr lastqual)

{
  if (sfpqual == NULL || lastqual == NULL) return FALSE;

  while (sfpqual != NULL && StringICmp (sfpqual->qual, "replace") != 0) {
    sfpqual = sfpqual->next;
  }
  while (lastqual != NULL && StringICmp (lastqual->qual, "replace") != 0) {
    lastqual = lastqual->next;
  }
  if (sfpqual == NULL || lastqual == NULL) return FALSE;

  if (StringICmp (sfpqual->val, lastqual->val) != 0) return TRUE;

  return FALSE;
}

static Boolean GBQualsDiffer (GBQualPtr sfpqual, GBQualPtr lastqual)

{
  if (sfpqual == NULL || lastqual == NULL) return FALSE;

  /* depends upon sorted order of gbquals imposed by BasicSeqEntryCleanup */

  while (sfpqual != NULL && lastqual != NULL) {
    if (StringICmp (sfpqual->qual, lastqual->qual) != 0) return TRUE;
    if (StringICmp (sfpqual->val, lastqual->val) != 0) return TRUE;
    sfpqual = sfpqual->next;
    lastqual = lastqual->next;
  }

  if (sfpqual != NULL || lastqual != NULL) return TRUE;

  return FALSE;
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
  Boolean         firstCDS;
  GeneRefPtr      grp, genomicgrp;
  SeqFeatPtr      last = NULL;
  Boolean         leave;
  CharPtr         label = NULL;
  CharPtr         comment = NULL;
  Int4            left = 0;
  Boolean         partialL = FALSE;
  Boolean         partialR = FALSE;
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
  Boolean         samelabel;
  int             severity;
  int             overlapPepSev;
  BioSourcePtr    biop = NULL;
  OrgRefPtr       orp = NULL;
  Int4            fiveUTRright;
  Int4            cdsRight;
  Int4            threeUTRright;
  Int4            cdscount, utr5count, utr3count;
  SeqFeatPtr      mrna, gene;

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
              (fcontext.sap == sap || (fcontext.sap->desc == NULL && sap->desc == NULL) || DescsSame (fcontext.sap->desc, sap->desc))) {
            if (gcp != NULL) {
              gcp->itemID = fcontext.itemID;
              gcp->thistype = OBJ_SEQFEAT;
            }
            vsp->descr = NULL;
            vsp->sfp = sfp;
            severity = SEV_ERROR;
            samelabel = TRUE;
            if (StringICmp (fcontext.label, label) != 0 || StringICmp (sfp->comment, comment) != 0) {
              samelabel = FALSE;
            }
            if (GBQualsDiffer (sfp->qual, last->qual)) {
              samelabel = FALSE;
            }
            if (featdeftype == FEATDEF_PUB ||
                featdeftype == FEATDEF_REGION || featdeftype == FEATDEF_misc_feature || featdeftype == FEATDEF_STS || featdeftype == FEATDEF_variation) {
              severity = SEV_WARNING;
            } else {
              if (! GPSorNTorNC (vsp->sep, sfp->location)) {
                severity = SEV_WARNING;
              } else {
                if (orp == NULL) {
                  SeqEntryToBioSource (vsp->sep, NULL, NULL, 0, &biop);
                  if (biop != NULL) {
                    orp = biop->org;
                  }
                }
                if (orp != NULL) {
                  /* curated fly source still has duplicate features */
                  if (StringICmp (orp->taxname, "Drosophila melanogaster") == 0) {
                    severity = SEV_WARNING;
                  }
                }
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
            }
            if (GPSorNTorNC (vsp->sep, sfp->location)) {
              severity = SEV_WARNING;
            }
            if (FlybaseDbxrefs (last->dbxref) || FlybaseDbxrefs (sfp->dbxref)) {
              severity = SEV_ERROR;
            }
            if (featdeftype == FEATDEF_repeat_region) {
              severity = SEV_WARNING;
            }
            if (featdeftype == FEATDEF_REGION && DifferentDbxrefs (last->dbxref, sfp->dbxref)) {
              /* do not report if both have dbxrefs and they are different */
            } else if (featdeftype == FEATDEF_variation && ReplaceQualsDiffer (sfp->qual, last->qual)) {
              /* do not report if both have replace quals and they are different */
            } else if (fcontext.sap == sap) {
              if (samelabel) {
                ValidErr (vsp, severity, ERR_SEQ_FEAT_FeatContentDup, "Duplicate feature");
              } else if (featdeftype != FEATDEF_PUB) {
                if (fcontext.partialL != partialL || fcontext.partialR != partialR) {
                  /* do not report if partial flags are different */
                } else {
                  ValidErr (vsp, severity, ERR_SEQ_FEAT_DuplicateFeat, "Features have identical intervals, but labels differ");
                }
              }
            } else {
              if (samelabel) {
                ValidErr (vsp, severity, ERR_SEQ_FEAT_FeatContentDup, "Duplicate feature (packaged in different feature table)");
              } else if (featdeftype != FEATDEF_PUB) {
                ValidErr (vsp, severity, ERR_SEQ_FEAT_DuplicateFeat, "Features have identical intervals, but labels differ (packaged in different feature table)");
              }
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
      partialL = fcontext.partialL;
      partialR = fcontext.partialR;
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

  lastLabel = NULL;
  sfp = SeqMgrGetNextGeneByLocusTag (bsp, NULL, &fcontext);
  while (sfp != NULL) {
    label = NULL;
    if (sfp->data.choice == SEQFEAT_GENE) {
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      if (grp != NULL) {
        label = grp->locus_tag;
      }
    }
    if (lastLabel != NULL) {
      message = NULL;
      if (StringCmp (lastLabel, label) == 0) {
        message = "Colliding locus_tags in gene features";
      } else if (StringICmp (lastLabel, label) == 0) {
        message = "Colliding locus_tags (with different capitalization) in gene features";
      }
      if (message != NULL) {
        if (gcp != NULL) {
          gcp->itemID = fcontext.itemID;
          gcp->thistype = OBJ_SEQFEAT;
        }
        vsp->descr = NULL;
        vsp->sfp = sfp;
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_CollidingLocusTags, "%s", message);
        vsp->sfp = NULL;
        if (gcp != NULL) {
          gcp->itemID = olditemid;
          gcp->thistype = olditemtype;
        }
      }
    }
    lastLabel = label; 
    sfp = SeqMgrGetNextGeneByLocusTag (bsp, sfp, &fcontext);
  }

  /* do UTR vs. CDS check on genomic if only one CDS, still need separate minus strand logic */
  cdscount = 0;
  utr5count = 0;
  utr3count = 0;
  strand = Seq_strand_plus;
  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL && cdscount < 2) {
    if (sfp->idx.subtype == FEATDEF_CDS) {
      strand = fcontext.strand;
      cdscount++;
    } else if (sfp->idx.subtype == FEATDEF_5UTR) {
      utr5count++;
    } else if (sfp->idx.subtype == FEATDEF_3UTR) {
      utr3count++;
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }
  if (bvsp->is_mrna || cdscount == 1) {
    if (bvsp->is_mrna) {
      strand = Seq_strand_plus;
    }
    fiveUTRright = 0;
    cdsRight = 0;
    threeUTRright = 0;
    firstCDS = TRUE;

    if (strand == Seq_strand_minus) {

      sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
      while (sfp != NULL) {
        if (sfp->idx.subtype == FEATDEF_3UTR && utr3count < 2) {
          if (fcontext.strand != Seq_strand_minus) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UTRdoesNotAbutCDS, "3'UTR is not on minus strand");
          } else if (threeUTRright > 0) {
            if (threeUTRright + 1 != fcontext.left) {
              if (gcp != NULL) {
                gcp->itemID = fcontext.itemID;
                gcp->thistype = OBJ_SEQFEAT;
              }
              vsp->descr = NULL;
              vsp->sfp = sfp;
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UTRdoesNotAbutCDS, "Previous 3'UTR does not abut next 3'UTR");
              if (gcp != NULL) {
                gcp->itemID = olditemid;
                gcp->thistype = olditemtype;
              }
            }
          }
          threeUTRright = fcontext.right;
        } else if (sfp->idx.subtype == FEATDEF_CDS) {
          cdsRight = fcontext.right;
          if (threeUTRright > 0 && firstCDS) {
            if (threeUTRright + 1 != fcontext.left) {
              if (gcp != NULL) {
                gcp->itemID = fcontext.itemID;
                gcp->thistype = OBJ_SEQFEAT;
              }
              vsp->descr = NULL;
              vsp->sfp = sfp;
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UTRdoesNotAbutCDS, "CDS does not abut 3'UTR");
              vsp->sfp = NULL;
              if (gcp != NULL) {
                gcp->itemID = olditemid;
                gcp->thistype = olditemtype;
              }
            }
          }
          firstCDS = FALSE;
        } else if (sfp->idx.subtype == FEATDEF_5UTR && utr5count < 2) {
          if (fcontext.strand != Seq_strand_minus) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UTRdoesNotAbutCDS, "5'UTR is not on minus strand");
          } else if (cdsRight > 0) {
            if (cdsRight + 1 != fcontext.left) {
              if (gcp != NULL) {
                gcp->itemID = fcontext.itemID;
                gcp->thistype = OBJ_SEQFEAT;
              }
              vsp->descr = NULL;
              vsp->sfp = sfp;
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UTRdoesNotAbutCDS, "5'UTR does not abut CDS");
              if (gcp != NULL) {
                gcp->itemID = olditemid;
                gcp->thistype = olditemtype;
              }
            }
          }
          threeUTRright = fcontext.right;
        }
        sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
      }

    } else {

      sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
      while (sfp != NULL) {
        if (sfp->idx.subtype == FEATDEF_5UTR && utr5count < 2) {
          if (fcontext.strand == Seq_strand_minus) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UTRdoesNotAbutCDS, "5'UTR is not on plus strand");
          }
          fiveUTRright = fcontext.right;
        } else if (sfp->idx.subtype == FEATDEF_CDS) {
          cdsRight = fcontext.right;
          if (fiveUTRright > 0 && firstCDS) {
            if (fiveUTRright + 1 != fcontext.left) {
              if (gcp != NULL) {
                gcp->itemID = fcontext.itemID;
                gcp->thistype = OBJ_SEQFEAT;
              }
              vsp->descr = NULL;
              vsp->sfp = sfp;
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UTRdoesNotAbutCDS, "5'UTR does not abut CDS");
              vsp->sfp = NULL;
              if (gcp != NULL) {
                gcp->itemID = olditemid;
                gcp->thistype = olditemtype;
              }
            }
          }
          firstCDS = FALSE;
        } else if (sfp->idx.subtype == FEATDEF_3UTR && utr3count < 2) {
          if (fcontext.strand == Seq_strand_minus) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UTRdoesNotAbutCDS, "3'UTR is not on plus strand");
          } else if (threeUTRright > 0) {
            if (threeUTRright + 1 != fcontext.left) {
              if (gcp != NULL) {
                gcp->itemID = fcontext.itemID;
                gcp->thistype = OBJ_SEQFEAT;
              }
              vsp->descr = NULL;
              vsp->sfp = sfp;
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UTRdoesNotAbutCDS, "Previous 3'UTR does not abut next 3'UTR");
              if (gcp != NULL) {
                gcp->itemID = olditemid;
                gcp->thistype = olditemtype;
              }
            }
          } else if (cdsRight > 0) {
            if (cdsRight + 1 != fcontext.left) {
              if (gcp != NULL) {
                gcp->itemID = fcontext.itemID;
                gcp->thistype = OBJ_SEQFEAT;
              }
              vsp->descr = NULL;
              vsp->sfp = sfp;
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UTRdoesNotAbutCDS, "CDS does not abut 3'UTR");
              if (gcp != NULL) {
                gcp->itemID = olditemid;
                gcp->thistype = olditemtype;
              }
            }
          }
          threeUTRright = fcontext.right;
        }
        sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
      }
    }
  }

  mrna = SeqMgrGetRNAgivenProduct (bsp, &fcontext);
  if (mrna != NULL) {
    genomicgrp = SeqMgrGetGeneXref (mrna);
    if (genomicgrp == NULL) {
      gene = SeqMgrGetOverlappingGene (mrna->location, NULL);
      if (gene != NULL) {
        genomicgrp = (GeneRefPtr) gene->data.value.ptrvalue;
      }
    }
    if (genomicgrp != NULL) {
      gene = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_GENE, 0, &fcontext);
      if (gene != NULL) {
        grp = (GeneRefPtr) gene->data.value.ptrvalue;
        if (grp != NULL) {
          if (StringCmp (grp->locus, genomicgrp->locus) != 0 ||
              StringCmp (grp->allele, genomicgrp->allele) != 0 ||
              StringCmp (grp->desc, genomicgrp->desc) != 0 ||
              StringCmp (grp->locus_tag, genomicgrp->locus_tag) != 0) {
            if (gcp != NULL) {
              gcp->itemID = fcontext.itemID;
              gcp->thistype = OBJ_SEQFEAT;
            }
            vsp->descr = NULL;
            vsp->sfp = gene;
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_GenesInconsistent, "Gene on mRNA bioseq does not match gene on genomic bioseq");
            if (gcp != NULL) {
              gcp->itemID = olditemid;
              gcp->thistype = olditemtype;
            }
          }
        }
      }
    }
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
        if (gcp != NULL) {
          gcp->itemID = 1;
          gcp->thistype = OBJ_SEQSUB_CIT;
        }
        vsp->descr = NULL;
        vsp->sfp = NULL;
        vsp->bssp = NULL;
        ValidateCitSub (vsp, csp);
        if (gcp != NULL) {
          gcp->itemID = olditemid;
          gcp->thistype = olditemtype;
        }
      }
    }
  }

  ValidateCDSmRNAmatch (vsp, bsp);

  if (vsp->locusTagGeneralMatch) {
    ValidateLocusTagGeneral (vsp, bsp);
  }

  if (ISA_na (bsp->mol) && SeqMgrGetParentOfPart (bsp, NULL) == NULL) {
    LookForMultipleUnpubPubs (vsp, gcp, bsp);
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
  size_t          acclen;
  ValidStructPtr  vsp;
  BioseqPtr       bsp;
  GatherScope     gs;
  BioseqValidStr  bvs;
  SeqFeatPtr      sfp;
  ValNode         fake_whole;
  SeqIdPtr        sip;
  ValNodePtr      vnp = NULL;
  MolInfoPtr      mip = NULL;
  SeqMgrDescContext dcontext;
  SeqMgrFeatContext fcontext;
  BioseqContextPtr bcp;
  Uint2           oldEntityID, oldItemID, oldItemtype;
  Uint2           mipEntityID = 0, mipItemID = 0, mipItemtype = 0;
  ObjMgrDataPtr   omdp;
  BioseqPtr       parent;
  PatentSeqIdPtr  psip;
  IdPatPtr        ipp;
  Boolean         isPDB = FALSE;
  Boolean         is_wgs = FALSE;
  Boolean         is_gb = FALSE;
  Boolean         is_transgenic = FALSE;
  ErrSev          sev;
  SubSourcePtr    ssp;
  CharPtr         str;
  TextSeqIdPtr    tsip;
  BioSourcePtr    biop;
  OrgRefPtr       orp;

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
      vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
      if (vnp != NULL) {
        mipEntityID = dcontext.entityID;
        mipItemID = dcontext.itemID;
        mipItemtype = OBJ_SEQDESC;
      }
    } else {
      bcp = BioseqContextNew (bsp);
      vnp = BioseqContextGetSeqDescr (bcp, Seq_descr_molinfo, NULL, NULL);
      BioseqContextFree (bcp);
    }
    if (vnp != NULL) {
      mip = (MolInfoPtr) vnp->data.ptrvalue;
    }
    vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
    if (vnp != NULL) {
      biop = (BioSourcePtr) vnp->data.ptrvalue;
      if (biop != NULL) {
        orp = biop->org;
        if (orp != NULL) {
          if (StringICmp (orp->taxname, "synthetic construct") == 0) {
            bvs.is_syn_constr = TRUE;
          }
        }
        if (biop->origin == ORG_ARTIFICIAL || biop->origin == ORG_MUT || biop->origin == ORG_SYNTHETIC) {
          bvs.is_artificial = TRUE;
        }
        for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
          if (ssp->subtype == SUBSRC_transgenic) {
            is_transgenic = TRUE;
          }
        }
        if (is_transgenic && ISA_na (bsp->mol)) {
          if (SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext) == NULL) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_DESCR_TransgenicProblem, "Transgenic source descriptor requires presence of source feature");
          }
        }
      }
    }
  }

  bvs.is_mrna = FALSE;
  bvs.is_prerna = FALSE;
  if (bsp != NULL && ISA_na (bsp->mol)) {
    if (mip != NULL) {
      if (mip->biomol == MOLECULE_TYPE_GENOMIC && mip->completeness == 1) {
        sev = SEV_ERROR;
        if (mip->tech == MI_TECH_htgs_3) {
          sev = SEV_WARNING;
        }
        for (sip = bsp->id; sip != NULL; sip = sip->next) {
          if (sip->choice == SEQID_GENBANK) {
            is_gb = TRUE;
          }
        }
        if (is_gb) {
          vnp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_title, &dcontext);
          if (vnp != NULL) {
            str = (CharPtr) vnp->data.ptrvalue;
            if (! StringHasNoText (str)) {
              if (StringISearch (str, "complete sequence") == NULL &&
                  StringISearch (str, "complete genome") == NULL) {

                oldEntityID = gcp->entityID;
                oldItemID = gcp->itemID;
                oldItemtype = gcp->thistype;

                gcp->entityID = mipEntityID;
                gcp->itemID = mipItemID;
                gcp->thistype = mipItemtype;

                if (bsp->topology == 2) {
                  ValidErr (vsp, SEV_WARNING, ERR_SEQ_INST_CompleteCircleProblem, "Circular topology has complete flag set, but title should say complete sequence or complete genome");
                } else {
                  ValidErr (vsp, sev, ERR_SEQ_DESCR_UnwantedCompleteFlag, "Suspicious use of complete");
                }

                gcp->entityID = oldEntityID;
                gcp->itemID = oldItemID;
                gcp->thistype = oldItemtype;
              }
            }
          }
        }
      } else if (mip->biomol == MOLECULE_TYPE_MRNA) {
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
        mip->tech == MI_TECH_wgs ||
        mip->tech == MI_TECH_htgs_0 || mip->tech == MI_TECH_htgs_1 ||
        mip->tech == MI_TECH_htgs_2 || mip->tech == MI_TECH_htgs_3) {
      if (mip->tech == MI_TECH_sts && bsp->mol == Seq_mol_rna && mip->biomol == MOLECULE_TYPE_MRNA) {
        /* there are some STS sequences derived from cDNAs, so do not report these */
      } else if (mip->biomol != MOLECULE_TYPE_GENOMIC) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_ConflictingBiomolTech, "HTGS/STS/GSS/WGS sequence should be genomic");
      } else if (bsp == NULL || (bsp->mol != Seq_mol_dna && bsp->mol != Seq_mol_na)) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_INST_ConflictingBiomolTech, "HTGS/STS/GSS/WGS sequence should not be RNA");
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
          if (sfp == NULL) {
            sfp = SeqMgrGetPROTgivenProduct (bsp, NULL); /* now instantiating and indexing products of protein processing */
          }
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
    bvs.bsp = bsp;
    ValidateBioseqContextIndexed (bsp, &bvs);
  } else {
    GatherSeqEntry (vsp->sep, &bvs, ValidateBioseqContextGather, &gs);
  }

  vsp->gcp = gcp;               /* reset the gcp pointer changed in previous gather */
  vsp->descr = NULL;
  vsp->sfp = NULL;

  if ((!bvs.got_a_pub) && (!vsp->suppress_no_pubs) && (! vsp->seqSubmitParent)) {
    omdp = NULL;
    if (gcp != NULL) {
      omdp = ObjMgrGetData (gcp->entityID);
    }
    if (omdp == NULL || omdp->datatype != OBJ_SEQSUB) {
      sev = SEV_ERROR;
      if (!IsNoncuratedRefSeq (bsp, &sev)) {
        ValidErr (vsp, sev, ERR_SEQ_DESCR_NoPubFound, "No publications refer to this Bioseq.");
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
    } else if (sip->choice == SEQID_GENBANK ||
               sip->choice == SEQID_EMBL ||
               sip->choice == SEQID_DDBJ) {
      is_gb = TRUE;
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
      if (tsip != NULL && tsip->accession != NULL) {
        acclen = StringLen (tsip->accession);
        if (acclen == 12) {
          is_wgs = TRUE;
        } else if (acclen == 13) {
          is_wgs = TRUE;
        }
      }
    } else if (sip->choice == SEQID_OTHER) {
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
      if (tsip != NULL && tsip->accession != NULL) {
        if (StringNCmp (tsip->accession, "NM_", 3) == 0 ||
            StringNCmp (tsip->accession, "NP_", 3) == 0 ||
            StringNCmp (tsip->accession, "NG_", 3) == 0 ||
            StringNCmp (tsip->accession, "NR_", 3) == 0) {
          is_gb = TRUE;
        }
      }
    }
  }
  if (is_wgs) {
    if (mip == NULL || mip->tech != MI_TECH_wgs) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "WGS accession should have Mol-info.tech of wgs");
    }
  } else if (mip != NULL && mip->tech == MI_TECH_wgs && is_gb) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_Inconsistent, "Mol-info.tech of wgs should have WGS accession");
  }

  if ((!bvs.last_org) && (!vsp->suppress_no_biosrc))
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_NoOrgFound, "No organism name has been applied to this Bioseq.");


  if ((bvs.is_aa) && (!bvs.num_full_length_prot_ref) && (!isPDB) && (bsp->repr != Seq_repr_virtual)) {
    parent = SeqMgrGetParentOfPart (bsp, NULL);
    if (parent == NULL || SeqMgrGetBestProteinFeature (bsp, NULL) == NULL) {

      oldEntityID = gcp->entityID;
      oldItemID = gcp->itemID;
      oldItemtype = gcp->thistype;

      if (SeqMgrGetCDSgivenProduct (bsp, &fcontext) != NULL) {
        gcp->entityID = fcontext.entityID;
        gcp->itemID = fcontext.itemID;
        gcp->thistype = OBJ_SEQFEAT;
      }

      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_NoProtRefFound, "No full length Prot-ref feature applied to this Bioseq");

      gcp->entityID = oldEntityID;
      gcp->itemID = oldItemID;
      gcp->thistype = oldItemtype;
    }
  }

  /* now flag missing molinfo even if not in Sequin */
  if (mip == NULL && (!isPDB)) {
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

static CharPtr legal_cons_splice_strings [] = {
  "(5'site:YES, 3'site:YES)",
  "(5'site:YES, 3'site:NO)",
  "(5'site:YES, 3'site:ABSENT)",
  "(5'site:NO, 3'site:YES)",
  "(5'site:NO, 3'site:NO)",
  "(5'site:NO, 3'site:ABSENT)",
  "(5'site:ABSENT, 3'site:YES)",
  "(5'site:ABSENT, 3'site:NO)",
  "(5'site:ABSENT, 3'site:ABSENT)",
  NULL
};

static void ValidateImpFeat (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp, ImpFeatPtr ifp)
{
  Int2            adv;
  BioseqPtr       bsp;
  Char            ch;
  Boolean         failed;
  Boolean         found;
  GBQualPtr       gbqual;
  SeqMgrFeatContext gcontext;
  SeqFeatPtr      gene;
  GeneRefPtr      grp;
  Int2            i;
  Int2            index;
  Boolean         isGenBankEMBLorDDBJ;
  Boolean         just_nuc_letters;
  CharPtr         key;
  Boolean         multi_compare;
  Boolean         multi_rpt_unit;
  Boolean         no_white_space;
  Boolean         only_digits;
  CharPtr         ptr;
  Int2            qual;
  Char            range[32];
  SeqIntPtr       sint;
  SeqIdPtr        sip;
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
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "%s is not a legal value for qualifier %s", gbqual->val, gbqual->qual);
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
          /*
          if (found) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "Illegal value for qualifier %s", gbqual->qual);
          } else if ((!multi_rpt_unit) && StringLen (gbqual->val) > 48) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "Illegal value for qualifier %s", gbqual->qual);
          }
          */
          if (StringICmp (key,"repeat_region") == 0) {
            if (! multi_rpt_unit) {
              if (StringLen (gbqual->val) == SeqLocLen (sfp->location)) {
                just_nuc_letters = TRUE;
                for (ptr = gbqual->val, ch = *ptr; ch != '\0'; ptr++, ch = *ptr) {
                  if (StringChr ("ACGTNacgtn", ch) == NULL) {
                    just_nuc_letters = FALSE;
                  }
                }
                if (just_nuc_letters) {
                  tmp = GetSequenceByFeature (sfp);
                  if (tmp != NULL) {
                    if (StringICmp (tmp, gbqual->val) != 0) {
                      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_InvalidQualifierValue, "repeat_region /rpt_unit and underlying sequence do not match");
                    }
                    MemFree (tmp);
                  }
                }
              }
            }
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
        } else if (val == GBQUAL_cons_splice) {
          found = FALSE;
          for (i = 0; legal_cons_splice_strings[i] != NULL; i++) {
            if (StringICmp (gbqual->val, legal_cons_splice_strings[i]) == 0) {
              found = TRUE;
              break;
            }
          }
          if (!found) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "%s is not a legal value for qualifier %s", gbqual->val, gbqual->qual);
          }
        } else if (val == GBQUAL_compare) {
          multi_compare = FALSE;
          ptr = gbqual->val;
          ch = *ptr;
          if (ch == '(') {
            multi_compare = TRUE;
          }
          if (! multi_compare) {
            adv = ValidateAccnDotVer (gbqual->val);
            if (adv == -5) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "%s accession missing version for qualifier %s", gbqual->val, gbqual->qual);
            } else if (adv != 0) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "%s is not a legal accession for qualifier %s", gbqual->val, gbqual->qual);
            } else if (StringChr (gbqual->val, '_') != NULL) {
              isGenBankEMBLorDDBJ = FALSE;
              VisitBioseqsInSep (vsp->sep, (Pointer) &isGenBankEMBLorDDBJ, LookForGEDseqID);
              if (isGenBankEMBLorDDBJ) {
                ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "RefSeq accession %s cannot be used for qualifier %s", gbqual->val, gbqual->qual);
              }
            }
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
        if (qual == GBQUAL_citation) {
          if (sfp->cit != NULL) {
            found = TRUE;
          } else if (! StringHasNoText (sfp->comment)) {
            /* RefSeq allows conflict with accession in comment instead of sfp->cit */
            if (StringICmp (key, "conflict") == 0) {
              bsp = BioseqFindFromSeqLoc (sfp->location);
              if (bsp != NULL) {
                for (sip = bsp->id; sip != NULL; sip = sip->next) {
                  if (sip->choice == SEQID_OTHER) {
                    found = TRUE;
                  }
                }
              }
            }
          }
        }
      }
      if (!found) {
        if (StringICmp (key, "conflict") == 0 || StringICmp (key, "old_sequence") == 0) {
          /* compare qualifier can now substitute for citation qualifier for conflict and old_sequence */
          for (gbqual = sfp->qual; gbqual != NULL; gbqual = gbqual->next) {
            if (StringICmp (gbqual->qual, "compare") == 0 && StringDoesHaveText (gbqual->val)) {
              found = TRUE;
            }
          }
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

static void ValidateNonImpFeat (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp)
{
  Int2            adv;
  BioseqPtr       bsp;
  Char            ch;
  Boolean         failed;
  Boolean         found;
  GBQualPtr       gbqual;
  Int2            i;
  Int2            index;
  Boolean         isGenBankEMBLorDDBJ;
  Boolean         just_nuc_letters;
  CharPtr         key;
  Boolean         multi_compare;
  Boolean         multi_rpt_unit;
  Boolean         no_white_space;
  Boolean         only_digits;
  CharPtr         ptr;
  Int2            qual;
  SeqIdPtr        sip;
  CharPtr         str;
  CharPtr         tmp;
  Int2            val;

  if (vsp == NULL || gcp == NULL || sfp == NULL)
    return;
  key = StringSaveNoNull (FeatDefTypeLabel (sfp));
  index = GBFeatKeyNameValid (&key, FALSE);
  for (gbqual = sfp->qual; gbqual != NULL; gbqual = gbqual->next) {
    if (StringCmp (gbqual->qual, "gsdb_id") == 0) {
      continue;
    }
    val = GBQualNameValid (gbqual->qual);
    if (val == -1) {
      if (gbqual->qual != NULL) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnknownFeatureQual, "Unknown qualifier %s", gbqual->qual);
      } else {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnknownFeatureQual, "NULL qualifier");
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
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_WrongQualOnFeature, "Wrong qualifier %s for feature %s", gbqual->qual, key);
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
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "%s is not a legal value for qualifier %s", gbqual->val, gbqual->qual);
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
          /*
          if (found) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "Illegal value for qualifier %s", gbqual->qual);
          } else if ((!multi_rpt_unit) && StringLen (gbqual->val) > 48) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "Illegal value for qualifier %s", gbqual->qual);
          }
          */
          if (StringICmp (key,"repeat_region") == 0) {
            if (! multi_rpt_unit) {
              if (StringLen (gbqual->val) == SeqLocLen (sfp->location)) {
                just_nuc_letters = TRUE;
                for (ptr = gbqual->val, ch = *ptr; ch != '\0'; ptr++, ch = *ptr) {
                  if (StringChr ("ACGTNacgtn", ch) == NULL) {
                    just_nuc_letters = FALSE;
                  }
                }
                if (just_nuc_letters) {
                  tmp = GetSequenceByFeature (sfp);
                  if (tmp != NULL) {
                    if (StringICmp (tmp, gbqual->val) != 0) {
                      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_InvalidQualifierValue, "repeat_region /rpt_unit and underlying sequence do not match");
                    }
                    MemFree (tmp);
                  }
                }
              }
            }
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
        } else if (val == GBQUAL_cons_splice) {
          found = FALSE;
          for (i = 0; legal_cons_splice_strings[i] != NULL; i++) {
            if (StringICmp (gbqual->val, legal_cons_splice_strings[i]) == 0) {
              found = TRUE;
              break;
            }
          }
          if (!found) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "%s is not a legal value for qualifier %s", gbqual->val, gbqual->qual);
          }
        } else if (val == GBQUAL_compare) {
          multi_compare = FALSE;
          ptr = gbqual->val;
          ch = *ptr;
          if (ch == '(') {
            multi_compare = TRUE;
          }
          if (! multi_compare) {
            adv = ValidateAccnDotVer (gbqual->val);
            if (adv == -5) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "%s accession missing version for qualifier %s", gbqual->val, gbqual->qual);
            } else if (adv != 0) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "%s is not a legal accession for qualifier %s", gbqual->val, gbqual->qual);
            } else if (StringChr (gbqual->val, '_') != NULL) {
              isGenBankEMBLorDDBJ = FALSE;
              VisitBioseqsInSep (vsp->sep, (Pointer) &isGenBankEMBLorDDBJ, LookForGEDseqID);
              if (isGenBankEMBLorDDBJ) {
                ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidQualifierValue, "RefSeq accession %s cannot be used for qualifier %s", gbqual->val, gbqual->qual);
              }
            }
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
        if (qual == GBQUAL_citation) {
          if (sfp->cit != NULL) {
            found = TRUE;
          } else if (! StringHasNoText (sfp->comment)) {
            /* RefSeq allows conflict with accession in comment instead of sfp->cit */
            if (StringICmp (key, "conflict") == 0) {
              bsp = BioseqFindFromSeqLoc (sfp->location);
              if (bsp != NULL) {
                for (sip = bsp->id; sip != NULL; sip = sip->next) {
                  if (sip->choice == SEQID_OTHER) {
                    found = TRUE;
                  }
                }
              }
            }
          }
        }
      }
      if (!found) {
        if (StringICmp (key, "conflict") == 0 || StringICmp (key, "old_sequence") == 0) {
          /* compare qualifier can now substitute for citation qualifier for conflict and old_sequence */
          for (gbqual = sfp->qual; gbqual != NULL; gbqual = gbqual->next) {
            if (StringICmp (gbqual->qual, "compare") == 0 && StringDoesHaveText (gbqual->val)) {
              found = TRUE;
            }
          }
        }
      }
      if (!found) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_MissingQualOnFeature, "Missing qualifier %s for feature %s", ParFlat_GBQual_names[qual].name, key);
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
  /*
  SeqPortPtr      spp = NULL;
  */
  Uint1           strand;
  Int4            strt, stp, donor, acceptor, len;
  StreamCache     sc;
  SeqInt          sint;
  ValNode         vn;

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
  if (strand != Seq_strand_minus) {
    if (! StreamCacheSetup (bsp, NULL, STREAM_EXPAND_GAPS, &sc)) {
      BioseqUnlock (bsp);
      return FALSE;
    }
  } else {
    sint.from = 0;
    sint.to = len - 1;
    sint.strand = strand;
    sint.id = sip;
    vn.choice = SEQLOC_INT;
    vn.data.ptrvalue = (Pointer) &sint;
    vn.next = NULL;
    if (! StreamCacheSetup (NULL, &vn, STREAM_EXPAND_GAPS, &sc)) {
      BioseqUnlock (bsp);
      return FALSE;
    }
  }
  /* spp = SeqPortNew (bsp, 0, -1, strand, Seq_code_ncbi4na); */
  BioseqUnlock (bsp);
  /*
  if (spp == NULL)
    return FALSE;
  */

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
    StreamCacheSetPosition (&sc, stp + 1);
    residue1 = StreamCacheGetResidue (&sc);
    residue2 = StreamCacheGetResidue (&sc);
    /*
    SeqPortSeek (spp, (stp + 1), SEEK_SET);
    residue1 = SeqPortGetResidue (spp);
    residue2 = SeqPortGetResidue (spp);
    */
    if (IS_residue (residue1) && IS_residue (residue2)) {
      if ((residue1 == 'G') && (residue2  == 'T')) {
        rsult = TRUE;
      } else if ((residue1 == 'G') && (residue2 == 'C')) {
        rsult = TRUE;
      }
    }
  } else if (slpTag == SLP_NOSTART && strt > 1) {
    StreamCacheSetPosition (&sc, strt - 2);
    residue1 = StreamCacheGetResidue (&sc);
    residue2 = StreamCacheGetResidue (&sc);
    /*
    SeqPortSeek (spp, (strt - 2), SEEK_SET);
    residue1 = SeqPortGetResidue (spp);
    residue2 = SeqPortGetResidue (spp);
    */
    if (IS_residue (residue1) && IS_residue (residue2)) {
      if ((residue1 == 'A') && (residue2 == 'G')) {
        rsult = TRUE;
      }
    }
  }

  /* spp = SeqPortFree (spp); */
  return rsult;
}

static void CheckTrnaCodons (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp, tRNAPtr trp)
{
  Uint1           aa = 0;
  BioseqPtr       bsp;
  Int2            code;
  CharPtr         codes = NULL;
  Uint1           from;
  GeneticCodePtr  gncp;
  Uint2           idx;
  Int2            j;
  SeqEntryPtr     sep;
  ErrSev          sev = SEV_ERROR;
  Uint1           shift;
  SeqMapTablePtr  smtp;
  Uint1           taa;
  ValNodePtr      vnp;

  if (vsp == NULL || gcp == NULL || sfp == NULL || trp == NULL)
    return;

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
      if (aa > 0 && aa != 255) {
        if (taa != aa) {
          if (aa == 'U') {
            sev = SEV_WARNING;
          }
          if (aa == 'U' && taa == '*' && trp->codon [j] == 14) {
            /* selenocysteine normally uses TGA (14), so ignore without requiring exception in record */
            /* TAG (11) is used for pyrrolysine in archaebacteria */
            /* TAA (10) is not yet known to be used for an exceptional amino acid */
          } else if (StringISearch (sfp->except_text, "modified codon recognition") == NULL) {
            ValidErr (vsp, sev, ERR_SEQ_FEAT_TrnaCodonWrong, "tRNA codon does not match genetic code");
          }
        }
      }
    } else if (trp->codon [j] < 255) {
      ValidErr (vsp, sev, ERR_SEQ_FEAT_BadTrnaCodon, "tRNA codon value %d is greater than maximum 63", (int) trp->codon [j]);
    }
  }

  if (aa > 0 && aa != 255) {
    if (aa <= 74) {
      shift = 0;
    } else if (aa > 79) {
      shift = 2;
    } else {
      shift = 1;
    }
    if (aa != '*') {
      idx = aa - (64 + shift);
    } else {
      idx = 25;
    }
    if (idx > 0 && idx < 26) {
      /* valid trna amino acid */
    } else {
      ValidErr (vsp, sev, ERR_SEQ_FEAT_BadTrnaAA, "Invalid tRNA amino acid");
    }
  }
}

static void CheckRnaProductType (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp, RnaRefPtr rrp)

{
  BioseqPtr          bsp;
  SeqMgrDescContext  context;
  MolInfoPtr         mip;
  SeqDescrPtr        sdp;

  if (vsp == NULL || gcp == NULL || sfp == NULL || rrp == NULL) return;
  bsp = BioseqFindFromSeqLoc (sfp->product);
  if (bsp == NULL) return;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &context);
  if (sdp == NULL) return;
  mip = (MolInfoPtr) sdp->data.ptrvalue;
  if (mip == NULL) return;
  switch (rrp->type) {
    case 2 : /* mRNA */
      if (mip->biomol == MOLECULE_TYPE_MRNA) return;
      break;
    case 3 : /* tRNA */
      if (mip->biomol == MOLECULE_TYPE_TRNA) return;
      break;
    case 4 : /* rRNA */
      if (mip->biomol == MOLECULE_TYPE_RRNA) return;
      break;
    default :
      return;
  }
  ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_RnaProductMismatch, "Type of RNA does not match MolInfo of product Bioseq");
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

static void CheckCDSPartial (ValidStructPtr vsp, SeqFeatPtr sfp)

{
  BioseqPtr          bsp;
  SeqMgrDescContext  context;
  MolInfoPtr         mip;
  Boolean            partial5;
  Boolean            partial3;
  SeqDescrPtr        sdp;

  if (vsp == NULL || sfp == NULL) return;
  if (sfp->product == NULL) return;
  if (!vsp->useSeqMgrIndexes) return;
  bsp = BioseqFindFromSeqLoc (sfp->product);
  if (bsp == NULL) return;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &context);
  if (sdp == NULL) return;
  mip = (MolInfoPtr) sdp->data.ptrvalue;
  if (mip == NULL) return;
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  switch (mip->completeness) {
    case 0 : /* unknown */
      break;
    case 1 : /* complete */
      if (partial5 || partial3) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "CDS is partial but protein is complete");
      }
      break;
    case 2 : /* partial */
      break;
    case 3 : /* no-left */
      if (! partial5) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "CDS is 5' complete but protein is NH2 partial");
      }
      if (partial3) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "CDS is 3' partial but protein is NH2 partial");
      }
      break;
    case 4 : /* no-right */
      if (! partial3) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "CDS is 3' complete but protein is CO2 partial");
      }
      if (partial5) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "CDS is 5' partial but protein is CO2 partial");
      }
      break;
    case 5 : /* no-ends */
      if (partial5 && partial3) {
      } else if (partial5) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "CDS is 5' partial but protein has neither end");
      } else if (partial3) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "CDS is 3' partial but protein has neither end");
      } else {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "CDS is complete but protein has neither end");
      }
      break;
    case 6 : /* has-left */
      break;
    case 7 : /* has-right */
      break;
    default :
      break;
  }
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
  Boolean         is_nw = FALSE;
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
    if (gene != NULL) {
      if (gene->pseudo) return;
      grp = (GeneRefPtr) gene->data.value.ptrvalue;
      if (grp != NULL && grp->pseudo) return;
    }
  }
  if (sfp->product == NULL) return;
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
        if (bssp != NULL && bssp->_class == BioseqseqSet_class_genbank) {
          sep = bssp->seq_set;
          if (sep != NULL && IS_Bioseq_set (sep)) {
            bssp = (BioseqSetPtr) sep->data.ptrvalue; 
            if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set)
              return;
          }
        }
      }
      /* or just a bioseq */
      if (sep != NULL && IS_Bioseq (sep))
        return;
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_MissingCDSproduct, "Unable to find product Bioseq from CDS feature");
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
          } else if (StringNICmp (tsip->accession, "NW_", 3) == 0) {
            is_nw = TRUE;
          }
        }
      }
    }
    if (/* (is_nc || is_nw) && */ nuc->idx.parenttype == OBJ_BIOSEQSET) {
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
  if (cds == NULL) return;
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
      if (bssp != NULL && bssp->_class == BioseqseqSet_class_genbank) {
        sep = bssp->seq_set;
        if (sep != NULL && IS_Bioseq_set (sep)) {
          bssp = (BioseqSetPtr) sep->data.ptrvalue; 
          if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set)
            if (BioseqFindFromSeqLoc (cds->location) != BioseqFindFromSeqLoc (sfp->location))
              return;
        }
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
  SeqFeatPtr      gene, operon;
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
  if (IsNCorNT (vsp->sep, sfp->location)) {
    sev = SEV_WARNING;
  }
  if (sfp->data.choice == SEQFEAT_CDREGION) {
    ValidErr (vsp, sev, ERR_SEQ_FEAT_CDSgeneRange, "gene overlaps CDS but does not completely contain it");
  } else if (sfp->data.choice == SEQFEAT_RNA) {
    operon = SeqMgrGetOverlappingOperon (sfp->location, &fcontext);
    if (operon != NULL)
      return;
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
  if (IsNCorNT (vsp->sep, sfp->location)) {
    sev = SEV_WARNING;
  }
  if (sfp->excpt) {
    sev = SEV_WARNING;
  }
  mrna = SeqMgrGetOverlappingFeature (sfp->location, FEATDEF_mRNA, NULL, 0, NULL, LOCATION_SUBSET, &fcontext);
  if (mrna != NULL) {
    if (StringISearch (sfp->except_text, "ribosomal slippage") == NULL) {
      ValidErr (vsp, sev, ERR_SEQ_FEAT_CDSmRNArange, "mRNA contains CDS but internal intron-exon boundaries do not match");
    }
  } else {
    ValidErr (vsp, sev, ERR_SEQ_FEAT_CDSmRNArange, "mRNA overlaps or contains CDS but does not completely contain intervals");
  }
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
  "GenBank",
  "EMBL",
  "DDBJ",
  NULL
};

static CharPtr badDbXref [] = {
  "PIDe",
  "PIDd",
  "PIDg",
  "PID",
  "NID",
  "GI",
  NULL
};

static void CheckForIllegalDbxref (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp, ValNodePtr dbxref)

{
  CharPtr     casecounts;            
  DbtagPtr    db;
  Int2        i;
  Int4        id;
  ValNodePtr  vnp;

  for (vnp = dbxref; vnp != NULL; vnp = vnp->next) {
    id = -1;
    db = (DbtagPtr) vnp->data.ptrvalue;
    if (db != NULL && db->db != NULL) {
      casecounts = NULL;
      for (i = 0; legalDbXrefs [i] != NULL; i++) {
        if (StringCmp (db->db, legalDbXrefs [i]) == 0) {
          id = i;
          break;
        } else if (StringICmp (db->db, legalDbXrefs [i]) == 0) {
          casecounts = legalDbXrefs [i];
        }
      }
      if (id == -1 && GPSorRefSeq (vsp->sep, sfp->location)) {
        for (i = 0; legalDbXrefOnRefSeq [i] != NULL; i++) {
          if (StringCmp (db->db, legalDbXrefOnRefSeq [i]) == 0) return;
        }
        for (i = 0; legalRefSeqDbXrefs [i] != NULL; i++) {
          if (StringCmp (db->db, legalRefSeqDbXrefs [i]) == 0) return;
        }
      }
      if (id == -1 || (sfp->data.choice != SEQFEAT_CDREGION && id < 4)) {
        if (StringDoesHaveText (casecounts)) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_IllegalDbXref, "Illegal db_xref type %s, legal capitalization is %s", db->db, casecounts);
        } else {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_IllegalDbXref, "Illegal db_xref type %s", db->db);
        }
      } else {
        for (i = 0; badDbXref [i] != NULL; i++) {
          if (StringICmp (db->db, badDbXref [i]) == 0) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_IllegalDbXref,
                      "db_xref type %s is only created by the flatfile generator, and should not be in the record as a separate xref", db->db);
          }
        }
      }
    }
  }
}

static CharPtr plastidtxt [] = {
  "",
  "",
  "chloroplast",
  "chromoplast",
  "",
  "",
  "plastid",
  "",
  "",
  "",
  "",
  "",
  "cyanelle",
  "",
  "",
  "",
  "apicoplast",
  "leucoplast",
  "proplastid",
  "",
};

static CharPtr legal_exception_strings [] = {
  "RNA editing",
  "reasons given in citation",
  "ribosomal slippage",
  "trans-splicing",
  "alternative processing",
  "artificial frameshift",
  "nonconsensus splice site",
  "rearrangement required for product",
  "modified codon recognition",
  "alternative start codon",
  NULL
};

static CharPtr refseq_exception_strings [] = {
  "unclassified transcription discrepancy",
  "unclassified translation discrepancy",
  "mismatches in transcription",
  "mismatches in translation",
  NULL
};

static void ValidateExceptText (ValidStructPtr vsp, GatherContextPtr gcp, SeqFeatPtr sfp)

{
  Boolean  found;
  Int2     i;
  CharPtr  ptr;
  Boolean  reasons_given_except = FALSE;
  ErrSev   sev = SEV_ERROR;
  CharPtr  str;
  CharPtr  tmp;

  str = StringSave (sfp->except_text);
  if (str == NULL) return;
  tmp = str;
  while (! StringHasNoText (tmp)) {
    ptr = StringChr (tmp, ',');
    if (ptr != NULL) {
      *ptr = '\0';
      ptr++;
    }
    TrimSpacesAroundString (tmp);
    found = FALSE;
    for (i = 0; legal_exception_strings[i] != NULL; i++) {
      if (StringICmp (tmp, legal_exception_strings[i]) == 0) {
        found = TRUE;
        if (StringICmp (tmp, "reasons given in citation") == 0) {
          reasons_given_except = TRUE;
        }
        break;
      }
    }
    if (!found) {
      if (GPSorRefSeq (vsp->sep, sfp->location)) {
        for (i = 0; refseq_exception_strings[i] != NULL; i++) {
          if (StringICmp (tmp, refseq_exception_strings[i]) == 0) {
            found = TRUE;
            break;
          }
        }
      }
      if (! found) {
        if (IsNCorNT (vsp->sep, sfp->location)) {
          sev = SEV_WARNING;
        }
        ValidErr (vsp, sev, ERR_SEQ_FEAT_InvalidQualifierValue, "%s is not a legal exception explanation", tmp);
      }
    }
    tmp = ptr;
  }
  MemFree (str);
  if (reasons_given_except && sfp->cit == NULL) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_MissingQualOnFeature, "Reasons given in citation exception does not have the required citation");
  }
}

typedef struct samecds {
  Boolean               found;
  SeqMgrFeatContextPtr  gcontext;
  Uint1                 subtype;
} SameCds, PNTR SameCdsPtr;

static Boolean LIBCALLBACK FindSameCDS (SeqFeatPtr sfp, SeqMgrFeatContextPtr ccontext)

{
  SeqMgrFeatContextPtr  gcontext;
  Int2                  i;
  SameCdsPtr            same;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  same = (SameCdsPtr) ccontext->userdata;
  gcontext = same->gcontext;
  if (gcontext == NULL || gcontext->sfp == NULL ||
      gcontext->ivals == NULL || ccontext->ivals == NULL) return TRUE;
  if (gcontext->strand == ccontext->strand ||
      (ccontext->strand == Seq_strand_unknown && gcontext->strand != Seq_strand_minus) ||
      (gcontext->strand == Seq_strand_unknown && ccontext->strand != Seq_strand_minus) ||
      ccontext->strand == Seq_strand_both) {
    /* test for strands from SeqMgrGetBestOverlappingFeat, keep going if okay */
  } else {
    return TRUE;
  }
  if (same->subtype == FEATDEF_GENE) {
    if (gcontext->left == ccontext->left &&
        gcontext->right == ccontext->right) {
      same->found = TRUE;
      return FALSE;
    }
  } else if (same->subtype == FEATDEF_mRNA) {
    if (gcontext->left == ccontext->left &&
        gcontext->right == ccontext->right &&
        gcontext->numivals == ccontext->numivals) {
      if (SeqLocAinB (sfp->location, gcontext->sfp->location) >= 0) {
        if (gcontext->numivals == 1) {
          same->found = TRUE;
          return FALSE;
        } else {
          for (i = 0; i < gcontext->numivals; i++) {
            if (gcontext->ivals [2 * i] != ccontext->ivals [2 * i]) return TRUE;
            if (gcontext->ivals [2 * i + 1] != ccontext->ivals [2 * i + 1]) return TRUE;
          }
          same->found = TRUE;
          return FALSE;
        }
      }
    }
  }
  return TRUE;
}

static Boolean SameAsCDS (SeqFeatPtr sfp)

{
  BioseqPtr          bsp;
  Boolean            cdsFilt [SEQFEAT_MAX];
  SeqMgrFeatContext  gcontext;
  SameCds            same;

  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp == NULL) return FALSE;
  if (SeqMgrGetDesiredFeature (0, bsp, 0, 0, sfp, &gcontext) != sfp) return FALSE;
  same.found = FALSE;
  same.gcontext = &gcontext;
  same.subtype = sfp->idx.subtype;
  MemSet ((Pointer) &cdsFilt, 0, sizeof (cdsFilt));
  cdsFilt [SEQFEAT_CDREGION] = TRUE;
  SeqMgrExploreFeatures (bsp, (Pointer) &same, FindSameCDS, sfp->location, cdsFilt, NULL);
  return same.found;
}

static Boolean HasUnderscore (CharPtr str)
{
  if (StringChr(str, '_') != NULL)
    return TRUE;
  else
    return FALSE;
}

static Boolean IsUpperCaseChar (Char ch)
{
  if (StringChr("ABCDEFGHIJKLMNOPQRSTUVWXYZ",ch) != NULL)
    return TRUE;
  else
    return FALSE;
}

static Boolean IsNumericChar (Char ch)
{
  if (StringChr("0123456789",ch) != NULL)
    return TRUE;
  else
    return FALSE;
}

NLM_EXTERN Boolean IsNuclAcc (CharPtr name)

{
  if (!IsUpperCaseChar (name[0]))
    return FALSE;

  if (!HasUnderscore (name))
    return FALSE;

  return TRUE;
}

static Boolean IsCddFeat (
  SeqFeatPtr sfp
)

{
  DbtagPtr    dbt;
  ValNodePtr  vnp;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_REGION) return FALSE;

  for (vnp = sfp->dbxref; vnp != NULL; vnp = vnp->next) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL && StringCmp (dbt->db, "CDD") == 0) return TRUE;
  }

  return FALSE;
}

/* derived from ValidateSeqLoc */
static void ValidateAnticodon (ValidStructPtr vsp, SeqLocPtr slp)
{
  SeqLocPtr       tmp;
  Boolean         retval = TRUE, tmpval, mixed_strand = FALSE, unmarked_strand = FALSE,
                  ordered = TRUE, adjacent = FALSE, circular = FALSE, exception = FALSE;
  CharPtr         ctmp;
  Uint1           strand1 = Seq_strand_other, strand2 = Seq_strand_other;
  Int4            from1 = -1, from2 = -1, to1 = -1, to2 = -1;
  SeqIntPtr       sip;
  SeqPntPtr       spp;
  SeqIdPtr        id1 = NULL, id2 = NULL;
  BioseqPtr       bsp;
  SeqFeatPtr      sfp = NULL;

  if (slp == NULL)
    return;

  sfp = vsp->sfp;

  bsp = BioseqFindFromSeqLoc (slp);
  if (bsp != NULL && bsp->topology == 2) {
    circular = TRUE;
  }

  tmp = NULL;

  for (tmp = SeqLocFindNext (slp, NULL); tmp != NULL; tmp = SeqLocFindNext (slp, tmp)) {
    tmpval = TRUE;
    switch (tmp->choice) {
    case SEQLOC_INT:
      sip = (SeqIntPtr) (tmp->data.ptrvalue);
      if (sip == NULL) continue;
      strand2 = sip->strand;
      id2 = sip->id;
      from2 = sip->from;
      to2 = sip->to;
      break;
    case SEQLOC_PNT:
      spp = (SeqPntPtr) (tmp->data.ptrvalue);
      if (spp == NULL) continue;
      strand2 = spp->strand;
      id2 = spp->id;
      from2 = spp->point;
      to2 = spp->point;
      break;
    case SEQLOC_NULL:
      continue;
    default:
      break;
    }

    if (id1 != NULL && id2 != NULL) {
      if (SeqIdForSameBioseq (id1, id2)) {
        if ((ordered) /* && (! circular) */) {
          if (strand2 == Seq_strand_minus) {
            if (to1 < to2)
              ordered = FALSE;
            if (to2 + 1 == from1)
              adjacent = TRUE;
          } else {
            if (to1 > to2)
              ordered = FALSE;
            if (to1 + 1 == from2)
              adjacent = TRUE;
          }
        }
        if (strand1 == strand2 && from1 == from2 && to1 == to2) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_DuplicateInterval, "Duplicate anticodon exons in location");
        }
      }
    }

    if (!tmpval) {
      retval = FALSE;
      ctmp = SeqLocPrint (tmp);
      if (ctmp != NULL && StringLen (ctmp) > 800) {
        StringCpy (ctmp + 797, "...");
      }
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_Range, "Anticodon location [%s] out of range", ctmp);
      MemFree (ctmp);
    }

    if ((strand1 != Seq_strand_other) && (strand2 != Seq_strand_other)) {
      if (SeqIdForSameBioseq (id1, id2)) {
        if (strand1 != strand2) {
          if (strand1 == Seq_strand_plus && strand2 == Seq_strand_unknown) {
            unmarked_strand = TRUE;
          } else if (strand1 == Seq_strand_unknown && strand2 == Seq_strand_plus) {
            unmarked_strand = TRUE;
          } else {
            mixed_strand = TRUE;
          }
        }
      }
    }

    from1 = from2;
    to1 = to2;
    id1 = id2;
    strand1 = strand2;
  }

  if (sfp != NULL && sfp->excpt) {
    exception = TRUE;
  }

  if (adjacent) {
    ctmp = SeqLocPrint (slp);
    /*
    if (exception) {
      sev = SEV_WARNING;
    } else {
      sev = SEV_ERROR;
    }
    */
    if (ctmp != NULL && StringLen (ctmp) > 800) {
      StringCpy (ctmp + 797, "...");
    }
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_AbuttingIntervals, "Adjacent intervals in Anticodon [%s]", ctmp);
    MemFree (ctmp);
  }

  if (exception) {
    /* trans splicing exception turns off both mixed_strand and out_of_order messages */
    if (StringISearch (sfp->except_text, "trans-splicing") != NULL) {
      return;
    }
  }

  if (mixed_strand || unmarked_strand || (!ordered)) {
    ctmp = SeqLocPrint (slp);
    if (ctmp != NULL && StringLen (ctmp) > 800) {
      StringCpy (ctmp + 797, "...");
    }
    if (mixed_strand) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_MixedStrand, "Mixed strands in Anticodon [%s]", ctmp);
    } else if (unmarked_strand) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_MixedStrand, "Mixed plus and unknown strands in Anticodon [%s]", ctmp);
    }
    if (!ordered)
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_SeqLocOrder, "Intervals out of order in Anticodon [%s]", ctmp);
    MemFree (ctmp);
    return;
  }

  /* newer check for intervals out of order on segmented bioseq */

  if (bsp == NULL || bsp->repr != Seq_repr_seg) return;

  if (SeqLocBadSortOrder (bsp, slp)) {
    ctmp = SeqLocPrint (slp);
    if (ctmp != NULL && StringLen (ctmp) > 800) {
      StringCpy (ctmp + 797, "...");
    }
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_SeqLocOrder, "Intervals out of order in Anticodon [%s]", ctmp);
    MemFree (ctmp);
  }

  /* newer check for mixed strand on segmented bioseq */

  if (SeqLocMixedStrands (bsp, slp)) {
    ctmp = SeqLocPrint (slp);
    if (ctmp != NULL && StringLen (ctmp) > 800) {
      StringCpy (ctmp + 797, "...");
    }
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_MixedStrand, "Mixed strands in Anticodon [%s]", ctmp);
    MemFree (ctmp);
  }
}

static Boolean JustQuotes (CharPtr str)

{
  Char  ch;

  if (str == NULL) return FALSE;

  ch = *str;
  while (ch != '\0') {
    if (ch != '"' && ch != ' ') return FALSE;
    str++;
    ch = *str;
  }

  return TRUE;
}

typedef struct dummysmfedata {
  Int4  max;
  Int4  num_at_max;
} DummySmfeData, PNTR DummySmfePtr;

static Boolean LIBCALLBACK DummySMFEProc (
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr context
)


{
  DummySmfePtr  dsp;
  Int4          len;

  if (sfp == NULL || context == NULL) return TRUE;
  dsp = context->userdata;
  if (dsp == NULL) return TRUE;

  len = SeqLocLen (sfp->location);
  if (len < dsp->max) {
    dsp->max = len;
    dsp->num_at_max = 1;
  } else if (len == dsp->max) {
    (dsp->num_at_max)++;
  }

  return TRUE;
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
  CodeBreakPtr    cbp, prevcbp;
  CharPtr         ctmp;
  RnaRefPtr       rrp;
  tRNAPtr         trp;
  GBQualPtr       gbq;
  Boolean         pseudo, excpt, conflict, codonqual,
                  anticodonqual, protidqual, transidqual, ovgenepseudo;
  ImpFeatPtr      ifp;
  GeneRefPtr      grp;
  ProtRefPtr      prp;
  ValNodePtr      vnp;
  BioseqPtr       bsp;
  BioseqContextPtr bcp = NULL;
  BioSourcePtr    biop, dbiop;
  OrgNamePtr      onp;
  OrgRefPtr       orp, dorp;
  SubSourcePtr    ssp;
  Boolean         transgenic;
  Int2            biopgencode;
  Int2            cdsgencode;
  Boolean         plastid;
  GeneticCodePtr  gc;
  PubdescPtr      pdp;
  DbtagPtr        db = NULL;
  Int4            id = -1;
  SeqMgrDescContext context;
  GeneRefPtr      grpx;
  SeqFeatPtr      sfpx;
  SeqFeatPtr      operon;
  Boolean         redundantgenexref;
  SeqMgrFeatContext fcontext;
  CharPtr         syn1, syn2, label = NULL;
  Uint2           oldEntityID, oldItemID;
  SeqIdPtr        sip;
  TextSeqIdPtr    tsip;
  BioseqPtr       protBsp;
  ErrSev          sev;
  Boolean         multitoken;
  Char            ch;
  CharPtr         ptr;
  Int4            anticodonlen;
  Boolean         badanticodon;
  SeqLocPtr       slp;
  Int2            count;
  DummySmfeData   dsd;
  CharPtr         str;
  size_t          len;

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
    /* inconsistent combination of partial/complete product,location,partial flag - part 1 */
    else if (((partials[0] == SLP_COMPLETE) && (sfp->product != NULL))) {
      sev = SEV_WARNING;
      bsp = GetBioseqGivenSeqLoc (sfp->product, gcp->entityID);
      /* if not local bioseq product, lower severity */
      if (bsp == NULL) {
        sev = SEV_INFO;
      }
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
      ValidErr (vsp, sev, ERR_SEQ_FEAT_PartialsInconsistent, buf);
    /* inconsistent combination of partial/complete product,location,partial flag - part 2 */
    } else if ((partials[1] == SLP_COMPLETE) || (!sfp->partial)) {
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
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PartialsInconsistent, buf);
    }
    /* 5' or 3' partial location giving unclassified partial product */
    else if (((partials [1] & SLP_START) != 0 || ((partials [1] & SLP_STOP) != 0)) && ((partials [0] & SLP_OTHER) != 0) && sfp->partial) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PartialProblem, "5' or 3' partial location should not have unclassified partial in product molinfo descriptor");
    }

    /* may have other error bits set as well */
    for (i = 0; i < 2; i++) {
      errtype = SLP_NOSTART;
      for (j = 0; j < 4; j++) {
        if (partials[i] & errtype) {
          if (i == 1 && j < 2 && IsCddFeat (sfp)) {
            /* suppresses  warning */
          } else if (i == 1 && j < 2 && PartialAtSpliceSite (sfp->location, errtype)) {
            ValidErr (vsp, SEV_INFO, ERR_SEQ_FEAT_PartialProblem,
		      "%s: %s (but is at consensus splice site)",
		      parterr[i], parterrs[j]);
          } else if (i == 1 && j < 2 &&
                     (sfp->data.choice == SEQFEAT_GENE || sfp->idx.subtype == FEATDEF_mRNA) &&
                     SameAsCDS (sfp)) {
            ValidErr (vsp, SEV_INFO, ERR_SEQ_FEAT_PartialProblem,
		      "%s: %s",
		      parterr[i], parterrs[j]);
          } else if (sfp->data.choice == SEQFEAT_CDREGION && j == 0) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PartialProblem,
		      "%s: %s", parterr[i], "5' partial is not at start AND"
		      " is not at consensus splice site");
          } else if (sfp->data.choice == SEQFEAT_CDREGION && j == 1) {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PartialProblem,
		      "%s: %s", parterr[i], "3' partial is not at stop AND"
		      " is not at consensus splice site");
          } else {
            ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PartialProblem,
		      "%s: %s", parterr[i], parterrs[j]);
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
          EmptyOrNullString (grp->allele) && EmptyOrNullString (grp->desc) &&
          EmptyOrNullString (grp->maploc) && EmptyOrNullString (grp->locus_tag) &&
          grp->db == NULL && grp->syn == NULL) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_GeneRefHasNoData, "There is a gene feature where all fields are empty");
      }
      if (! StringHasNoText (grp->locus_tag)) {
        multitoken = FALSE;
        for (ptr = grp->locus_tag, ch = *ptr; ch != '\0'; ptr++, ch = *ptr) {
          if (IS_WHITESP (ch)) {
            multitoken = TRUE;
          }
        }
        if (multitoken) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_LocusTagProblem, "Gene locus_tag '%s' should be a single word without any spaces", grp->locus_tag);
        }
      }
      CheckForIllegalDbxref (vsp, gcp, sfp, grp->db);
      if (StringDoesHaveText (grp->allele)) {
        for (gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
          if (StringCmp (gbq->qual, "allele") == 0 && StringDoesHaveText (gbq->val)) {
            if (StringICmp (gbq->val, grp->allele) == 0) {
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_WrongQualOnFeature, "Redundant allele qualifier (%s) on gene", gbq->val);
            } else if (sfp->idx.subtype != FEATDEF_variation) {
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_WrongQualOnFeature, "Hidden allele qualifier (%s) on gene", gbq->val);
            }
          }
        }
      }
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
    protidqual = FALSE;
    transidqual = FALSE;
    ovgenepseudo = FALSE;
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
      if (StringICmp (gbq->qual, "protein_id") == 0) {
        protidqual = TRUE;
      }
      if (StringICmp (gbq->qual, "transcript_id") == 0) {
        transidqual = TRUE;
      }
      gbq = gbq->next;
    }
    if (protidqual) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_WrongQualOnFeature, "protein_id should not be a gbqual on a CDS feature");
    }
    if (transidqual) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_WrongQualOnFeature, "transcript_id should not be a gbqual on a CDS feature");
    }
    if (OverlappingGeneIsPseudo (sfp)) {
      pseudo = TRUE;
      ovgenepseudo = TRUE;
    }
    if ((!pseudo) && (!conflict)) {
      CdTransCheck (vsp, sfp);
      SpliceCheck (vsp, sfp);
    } else if (conflict) {
      CdConflictCheck (vsp, sfp);
    }
    CdsProductIdCheck (vsp, sfp);
    crp = (CdRegionPtr) (sfp->data.value.ptrvalue);
    if (crp != NULL) {
      if (crp->code_break != NULL && StringISearch (sfp->except_text,  "RNA editing") != NULL) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_TranslExceptAndRnaEditing, "CDS has both RNA editing /exception and /transl_except qualifiers");
      }
      prevcbp = NULL;
      for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
        i = SeqLocCompare (cbp->loc, sfp->location);
        if ((i != SLC_A_IN_B) && (i != SLC_A_EQ_B)) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_Range, "Code-break location not in coding region");
        }
        if (prevcbp != NULL) {
          i = SeqLocCompare (cbp->loc, prevcbp->loc);
          if (i == SLC_A_EQ_B) {
            ctmp = SeqLocPrint (cbp->loc);
            if (ctmp != NULL) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_DuplicateTranslExcept, "Multiple code-breaks at same location [%s]", ctmp);
            } else {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_DuplicateTranslExcept, "Multiple code-breaks at same location");
            }
            MemFree (ctmp);
          }
        }
        prevcbp = cbp;
      }
      if (excpt && (!sfp->excpt)) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_ExceptInconsistent, "Exception flag should be set in coding region");
      }
      if (crp->orf && sfp->product != NULL) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_OrfCdsHasProduct, "An ORF coding region should not have a product");
      }
      if (pseudo && sfp->product != NULL) {
        if (ovgenepseudo) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PseudoCdsViaGeneHasProduct, "A coding region overlapped by a pseudogene should not have a product");
        } else {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_PseudoCdsHasProduct, "A pseudo coding region should not have a product");
        }
      }
      if (codonqual) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_CodonQualifierUsed, "Use the proper genetic code, if available, or set transl_excepts on specific codons");
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
          plastid = FALSE;
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
                       biop->genome == GENOME_apicoplast ||
                       biop->genome == GENOME_leucoplast ||
                       biop->genome == GENOME_proplastid) {
              biopgencode = 11;
              plastid = TRUE;
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
              if (! vsp->seqSubmitParent) { /* suppress when validator run from tbl2asn */
                if (plastid) {
                  ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_GenCodeMismatch,
                            "Genetic code conflict between CDS (code %d) and BioSource.genome biological context (%s) (uses code 11)", (int) cdsgencode, plastidtxt [biop->genome]);
                } else {
                  ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_GenCodeMismatch,
                            "Genetic code conflict between CDS (code %d) and BioSource (code %d)", (int) cdsgencode, (int) biopgencode);
                }
              }
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
    CheckCDSPartial (vsp, sfp);
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
        if (vnp != NULL) {
          str = (CharPtr) vnp->data.ptrvalue;
          if (StringDoesHaveText (str)) {
            len = StringLen (str);
            if (len > 1) {
              if (str [len - 1] == ']') {
                ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_ProteinNameEndsInBracket, "Protein name ends with bracket and may contain organism name");
              }
            }
          }
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
      if (prp->name == NULL && StringDoesHaveText (prp->desc)) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_NoNameForProtein, "Protein feature has description but no name");
      }
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
        SpliceCheck (vsp, sfp);
      }
      CheckForBothStrands (vsp, sfp);
      CheckForBadGeneOverlap (vsp, sfp);
      CheckForCommonMRNAProduct (vsp, sfp);
      protidqual = FALSE;
      transidqual = FALSE;
      gbq = sfp->qual;
      while (gbq != NULL) {
        if (StringICmp (gbq->qual, "protein_id") == 0) {
          protidqual = TRUE;
        }
        if (StringICmp (gbq->qual, "transcript_id") == 0) {
          transidqual = TRUE;
        }
        gbq = gbq->next;
      }
      if (protidqual) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_WrongQualOnFeature, "protein_id should not be a gbqual on an mRNA feature");
      }
      if (transidqual) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_WrongQualOnFeature, "transcript_id should not be a gbqual on an mRNA feature");
      }
    }
    if (rrp->ext.choice == 2) { /* tRNA */
      trp = (tRNAPtr) (rrp->ext.value.ptrvalue);
      if (trp->anticodon != NULL) {
        badanticodon = FALSE;
        anticodonlen = 0;
        slp = SeqLocFindNext (trp->anticodon, NULL);
        while (slp != NULL) {
          anticodonlen += SeqLocLen (slp);
          i = SeqLocCompare (slp, sfp->location);
          if ((i != SLC_A_IN_B) && (i != SLC_A_EQ_B)) {
            badanticodon = TRUE;
          }
          slp = SeqLocFindNext (trp->anticodon, slp);
        }
        if (badanticodon) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_Range, "Anticodon location not in tRNA");
        }
        if (anticodonlen != 3) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_Range, "Anticodon is not 3 bases in length");
        }
        ValidateAnticodon (vsp, trp->anticodon);
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
    if (sfp->product != NULL) {
      CheckRnaProductType (vsp, gcp, sfp, rrp);
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
                      transgenic = FALSE;
                      for (ssp = dbiop->subtype; ssp != NULL; ssp = ssp->next) {
                        if (ssp->subtype == SUBSRC_transgenic) {
                          transgenic = TRUE;
                        }
                      }
                      if (! transgenic) {
                        oldEntityID = gcp->entityID;
                        oldItemID = gcp->itemID;

                        gcp->entityID = context.entityID;
                        gcp->itemID = context.itemID;
                        gcp->thistype = OBJ_SEQDESC;

                        ValidErr (vsp, SEV_ERROR, ERR_SEQ_DESCR_BioSourceNeedsFocus,
                                  "BioSource descriptor must have focus or transgenic when BioSource feature with different taxname is present.");

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
    }
    /*
       ValidateBioSource (vsp, gcp, biop, sfp);
     */
    break;
  default:
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_InvalidType, "Invalid SeqFeat type [%d]", (int) (type));
    break;
  }
  if (type != 8) {
    ValidateNonImpFeat (vsp, gcp, sfp);
  }
  if ((! sfp->excpt) && (! StringHasNoText (sfp->except_text))) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_ExceptInconsistent, "Exception text is set, but exception flag is not set");
  }
  if (! StringHasNoText (sfp->except_text)) {
    ValidateExceptText (vsp, gcp, sfp);
  }

  for (gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
  	if (StringICmp (gbq->qual, "replace") != 0) {
      if (JustQuotes (gbq->val)) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_InvalidQualifierValue, "Qualifier other than replace has just quotation marks");
      }
  	}
  }

  if (sfp->product != NULL) {
    sip = SeqLocId (sfp->product);
    if (sip != NULL) {
      switch (sip->choice) {
        case SEQID_LOCAL :
	  break;
        case SEQID_GENBANK :
        case SEQID_EMBL :
        case SEQID_DDBJ :
        case SEQID_OTHER :
        case SEQID_TPG :
        case SEQID_TPE :
        case SEQID_TPD :
        case SEQID_GPIPE :
          tsip = (TextSeqIdPtr) sip->data.ptrvalue;
          if (tsip != NULL) {
            if (tsip->accession == NULL && (! StringHasNoText (tsip->name))) {
              if (ValidateAccn (tsip->name)) {
                ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_BadProductSeqId,
                         "Feature product should not put an accession in the Textseq-id 'name' slot");
              } else {
                ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_BadProductSeqId,
                         "Feature product should not use Textseq-id 'name' slot");
              }
            }
          }
          break;
        default :
          break;
      }
    }
    protBsp = BioseqFindFromSeqLoc (sfp->product);
    if (protBsp != NULL && protBsp->id != NULL) {
      for (sip = protBsp->id; sip != NULL; sip = sip->next) {
        switch (sip->choice) {
          case SEQID_GENBANK :
          case SEQID_EMBL :
          case SEQID_DDBJ :
          case SEQID_OTHER :
          case SEQID_TPG :
          case SEQID_TPE :
          case SEQID_TPD :
          case SEQID_GPIPE:
            tsip = (TextSeqIdPtr) sip->data.ptrvalue;
            if (tsip != NULL) {
              if (tsip->accession == NULL && (! StringHasNoText (tsip->name))) {
                if (ValidateAccn (tsip->name)) {
                  ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_BadProductSeqId,
                            "Protein bioseq has Textseq-id 'name' that looks"
                            " like it is derived from a nucleotide accession");
                } else {
                  ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_BadProductSeqId,
                            "Protein bioseq has Textseq-id 'name' and no accession");
                }
              }
            }
            break;
          default :
            break;
        }
      }
    }
    
  }
  if (type != SEQFEAT_GENE) {
    grp = SeqMgrGetGeneXref (sfp);
    if (grp == NULL) {
      sfpx = SeqMgrGetOverlappingGene (sfp->location, &fcontext);
      if (sfpx != NULL) {
        grp = (GeneRefPtr) sfpx->data.value.ptrvalue;
      }
    }
    if (grp != NULL && (! SeqMgrGeneIsSuppressed (grp))) {
      if (! StringHasNoText (grp->allele)) {
        for (gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
          if (StringCmp (gbq->qual, "allele") == 0 && StringDoesHaveText (gbq->val)) {
            if (StringICmp (gbq->val, grp->allele) == 0) {
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_InvalidQualifierValue, "Redundant allele qualifier (%s) on gene and feature", gbq->val);
            } else if (sfp->idx.subtype != FEATDEF_variation) {
              ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_InvalidQualifierValue, "Mismatched allele qualifier on gene (%s) and feature (%s)", grp->allele, gbq->val);
            }
          }
        }
      }
    }
    grp = SeqMgrGetGeneXref (sfp);
    if (grp != NULL && SeqMgrGeneIsSuppressed (grp)) return;

    if (grp == NULL) {
      sfpx = SeqMgrGetOverlappingGene (sfp->location, &fcontext);
      if (sfpx == NULL || sfpx->data.choice != SEQFEAT_GENE) return;
      MemSet ((Pointer) &dsd, 0, sizeof (DummySmfeData));
      dsd.max = INT4_MAX;
      dsd.num_at_max = 0;
      count = SeqMgrGetAllOverlappingFeatures (sfp->location, FEATDEF_GENE, NULL, 0,
                                               LOCATION_SUBSET, (Pointer) &dsd, DummySMFEProc);
      if (dsd.num_at_max > 1) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_MissingGeneXref,
                  "Feature overlapped by %d identical-length genes but has no cross-reference", (int) dsd.num_at_max);
      }
      return;
    }

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
    } else if (StringDoesHaveText (grp->locus_tag) && StringDoesHaveText (grp->locus_tag)) {
      if ((StringICmp (grp->locus_tag, grpx->locus_tag) == 0)) {
        redundantgenexref = TRUE;
        label = grp->locus_tag;
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
      MemSet ((Pointer) &dsd, 0, sizeof (DummySmfeData));
      dsd.max = INT4_MAX;
      dsd.num_at_max = 0;
      count = SeqMgrGetAllOverlappingFeatures (sfp->location, FEATDEF_GENE, NULL, 0,
                                               LOCATION_SUBSET, (Pointer) &dsd, DummySMFEProc);
      if (dsd.num_at_max > 1) {
        redundantgenexref = FALSE;
      }
    }
    if (redundantgenexref) {
      if (StringHasNoText (label)) {
        label = "?";
      }
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnnecessaryGeneXref, "Unnecessary gene cross-reference %s", label);
    }
  } else {
    grp = SeqMgrGetGeneXref (sfp);
    if (grp != NULL) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnnecessaryGeneXref, "Gene feature has gene cross-reference");
    }
    operon = SeqMgrGetOverlappingOperon (sfp->location, &fcontext);
    if (operon != NULL) {
      if (SeqMgrGetDesiredFeature (sfp->idx.entityID, 0, 0, 0, sfp, &fcontext) == sfp) {
        if (! StringHasNoText (fcontext.label)) {
          for (gbq = operon->qual; gbq != NULL; gbq = gbq->next) {
            if (StringCmp (gbq->qual, "operon") == 0) {
              if (StringICmp (gbq->val, fcontext.label) == 0) {
                ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_InvalidQualifierValue, "Operon is same as gene - %s", gbq->val);
              }
            }
          }
        }
      }
    }
  }
}

/*****************************************************************************
*
*   MrnaTransCheck (sfp, vsp)
*
*****************************************************************************/

static CharPtr bypass_mrna_trans_check [] = {
  "RNA editing",
  "reasons given in citation",
  "artificial frameshift",
  "unclassified transcription discrepancy",
  "mismatches in transcription",
  NULL
};

NLM_EXTERN void MrnaTransCheck (ValidStructPtr vsp, SeqFeatPtr sfp)
{
  BioseqPtr       bsp;
  Char            ch;
  Int4            counta, countnona;
  CharPtr         farstr = "";
  ErrSev          fetchsev;
  GatherContextPtr  gcp;
  Boolean         has_errors = FALSE, unclassified_except = FALSE, mismatch_except = FALSE;
  Int2            i;
  Boolean         is_refseq = FALSE;
  Int4            mismatch, total;
  CharPtr         mrseq, pdseq;
  Int4            mlen, plen;
  CharPtr         ptr1, ptr2;
  Boolean         report_errors = TRUE;
  ErrSev          sev;
  SeqFeat         sf;
  SeqIdPtr        sip, sip2;
  Boolean         unlockProd = FALSE;
  ValNode         vn;

  if (sfp == NULL)
    return;
  if (sfp->pseudo)
    return;
  if (sfp->product == NULL)
    return;

  if (sfp->excpt && (! StringHasNoText (sfp->except_text))) {
    for (i = 0; bypass_mrna_trans_check [i] != NULL; i++) {
      if (StringISearch (sfp->except_text,  bypass_mrna_trans_check [i]) != NULL) {
        report_errors = FALSE;  /* biological exception */
      }
    }
    if (StringStr (sfp->except_text, "unclassified transcription discrepancy") != NULL) {
      unclassified_except = TRUE;
    }
    if (StringStr (sfp->except_text, "mismatches in transcription") != NULL) {
      mismatch_except = TRUE;
      report_errors = TRUE;
    }
  }

  sip = SeqLocId (sfp->product);
  if (sip == NULL)
    return;

  mrseq = GetSequenceByFeature (sfp);
  if (mrseq == NULL) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MrnaTransFail, "Unable to transcribe mRNA");
    return;
  }

  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp != NULL) {
    for (sip2 = bsp->id; sip2 != NULL; sip2 = sip2->next) {
      if (sip2->choice == SEQID_OTHER) {
        is_refseq = TRUE;
      }
    }
  }

  sev = SEV_ERROR;
  gcp = vsp->gcp;
  if (gcp != NULL) {
    bsp = GetBioseqGivenSeqLoc (sfp->product, gcp->entityID);
    if (bsp == NULL) {
      /* if not local bioseq product, lower severity */
      sev = SEV_WARNING;
      if (is_refseq) {
        /* if refseq, restore higher severity */
        sev = SEV_ERROR;
      }
    }
    if (bsp == NULL && vsp->farFetchMRNAproducts) {
      bsp = BioseqLockById (sip);
      if (bsp != NULL) {
        unlockProd = TRUE;
        farstr = "(far) ";
      }
    }
  }
  if (is_refseq && unclassified_except) {
    /* if unclassified exception, drop back down to warning */
    sev = SEV_WARNING;
  }

  /* coerced feature on whole product for GetSequenceByFeature */

  MemSet ((Pointer) &sf, 0, sizeof (SeqFeat));
  MemSet ((Pointer) &vn, 0, sizeof (ValNode));
  sf.location = &vn;
  vn.choice = SEQLOC_WHOLE;
  vn.data.ptrvalue = sip;

  pdseq = GetSequenceByFeature (&sf);
  if (pdseq == NULL) {
    has_errors = TRUE;
    if (report_errors || unclassified_except) {
      fetchsev = SEV_ERROR;
      if (sip->choice != SEQID_GI) {
        fetchsev = SEV_WARNING;
      }
      ValidErr (vsp, fetchsev, ERR_SEQ_FEAT_MrnaTransFail, "Unable to fetch mRNA transcript");
    }
  }
  if (pdseq != NULL) {
    mlen = StringLen (mrseq);
    plen = StringLen (pdseq);
    if (mlen != plen) {
      if (mlen < plen) {
        ptr1 = pdseq + mlen;
        counta = 0;
        countnona = 0;
        ch = *ptr1;
        while (ch != '\0') {
          if (ch == 'A' || ch == 'a') {
            counta++;
          } else {
            countnona++;
          }
          ptr1++;
          ch = *ptr1;
        }
        if (counta < 19 * countnona) {
          has_errors = TRUE;
          if (report_errors) {
            ValidErr (vsp, sev, ERR_SEQ_FEAT_TranscriptLen, "Transcript length [%ld] less than %sproduct length [%ld], and tail < 95%s polyA", (long) mlen, farstr, (long) plen, "%");
          }
          plen = mlen; /* even if it fails polyA test, allow base-by-base comparison on common length */
        } else if (counta > 0 && countnona == 0) {
          has_errors = TRUE;
          if (report_errors) {
            ValidErr (vsp, SEV_INFO, ERR_SEQ_FEAT_PolyATail, "Transcript length [%ld] less than %sproduct length [%ld], but tail is 100%s polyA", (long) mlen, farstr, (long) plen, "%");
          }
          plen = mlen; /* if it passes polyA test, allow base-by-base comparison on common length */
        } else {
          has_errors = TRUE;
          if (report_errors) {
            ValidErr (vsp, SEV_INFO, ERR_SEQ_FEAT_PolyATail, "Transcript length [%ld] less than %sproduct length [%ld], but tail >= 95%s polyA", (long) mlen, farstr, (long) plen, "%");
          }
          plen = mlen; /* if it passes polyA test, allow base-by-base comparison on common length */
        }
      } else {
        has_errors = TRUE;
        if (report_errors) {
          ValidErr (vsp, sev, ERR_SEQ_FEAT_TranscriptLen, "Transcript length [%ld] greater than %sproduct length [%ld]", (long) mlen, farstr, (long) plen);
        }
      }
    }
    if (mlen == plen && mlen > 0 && StringICmp (mrseq, pdseq) != 0) {
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
      if (mismatch > 0) {
        has_errors = TRUE;
        if (report_errors && (! mismatch_except)) {
          ValidErr (vsp, sev, ERR_SEQ_FEAT_TranscriptMismatches,
                    "There are %ld mismatches out of %ld bases between the transcript and %sproduct sequence", (long) mismatch, (long) total, farstr);
        }
      }
    }
    MemFree (pdseq);
  }

  MemFree (mrseq);

  if (unlockProd) {
    BioseqUnlock (bsp);
  }

  if (! report_errors) {
    if (! has_errors) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnnecessaryException, "mRNA has exception but passes transcription test");
    }
  }
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

static Boolean Loc_is_GEDL (SeqLocPtr location)
{
  BioseqPtr  bsp;
  SeqIdPtr   sip;

  if (location == NULL)
    return FALSE;
  sip = SeqLocId (location);
  if (sip == NULL)
    return FALSE;
  bsp = BioseqFind (sip);
  if (bsp == NULL)
    return FALSE;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GENBANK) return TRUE;
    if (sip->choice == SEQID_EMBL) return TRUE;
    if (sip->choice == SEQID_DDBJ) return TRUE;
    if (sip->choice == SEQID_LOCAL) return TRUE;
  }
  return FALSE;
}

static void CdConflictCheck (ValidStructPtr vsp, SeqFeatPtr sfp)

{
  ByteStorePtr  bs;
  BioseqPtr     bsp;
  CharPtr       str1, str2;

  if (sfp == NULL || vsp == NULL) return;

  bsp = BioseqFindFromSeqLoc (sfp->product);
  str1 = GetSequenceByBsp (bsp);
  bs = TransTableTranslateCdRegion (NULL, sfp, FALSE, FALSE, TRUE);
  str2 = (CharPtr) BSMerge (bs, NULL);
  BSFree (bs);

  if (str1 != NULL && str2 != NULL && StringCmp (str1, str2) == 0) {
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_BadConflictFlag, "Coding region conflict flag should not be set");
  } else {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_ConflictFlagSet, "Coding region conflict flag is set");
  }

  MemFree (str1);
  MemFree (str2);
}

static CharPtr bypass_cds_trans_check [] = {
  "RNA editing",
  "reasons given in citation",
  "artificial frameshift",
  "rearrangement required for product",
  "unclassified translation discrepancy",
  "mismatches in translation",
  NULL
};

NLM_EXTERN void CdTransCheck (ValidStructPtr vsp, SeqFeatPtr sfp)
{
  ByteStorePtr    newprot = NULL;
  CharPtr         protseq = NULL;
  BioseqPtr       prot1seq = NULL, prot2seq = NULL;
  Int4            prot1len = 0, prot2len, i, len;
  CdRegionPtr     crp;
  SeqIdPtr        protid = NULL;
  Int2            residue1, residue2, stop_count = 0, mismatch = 0, ragged = 0;
  Boolean         got_stop = FALSE;
  /*
  SeqPortPtr      spp = NULL;
  */
  Uint2           part_loc = 0, part_prod = 0;
  Boolean         no_end = FALSE, no_beg = FALSE, show_stop = FALSE,
                  got_dash = FALSE, alt_start = FALSE, done;
  GBQualPtr       gb;
  ValNodePtr      vnp, code;
  int             gccode = 0;
  Boolean         transl_except = FALSE, prot_ok = TRUE, is_nc = FALSE,
                  has_errors = FALSE, report_errors = TRUE,
                  unclassified_except = FALSE, mismatch_except = FALSE,
                  frameshift_except = FALSE, rearrange_except = FALSE;
  CharPtr         nuclocstr, farstr = "";
  CodeBreakPtr    cbp;
  Int4            pos1, pos2, pos;
  SeqLocPtr       tmp;
  ErrSev          sev;
  SeqEntryPtr     sep;
  Boolean         unlockProd = FALSE;
  StreamCache     sc;


  if (sfp == NULL)
    return;

  if (sfp->excpt && (! StringHasNoText (sfp->except_text))) {
    for (i = 0; bypass_cds_trans_check [i] != NULL; i++) {
      if (StringISearch (sfp->except_text,  bypass_cds_trans_check [i]) != NULL) {
        report_errors = FALSE;  /* biological exception */
      }
    }
    if (StringStr (sfp->except_text, "unclassified translation discrepancy") != NULL) {
      unclassified_except = TRUE;
    }
    if (StringStr (sfp->except_text, "mismatches in translation") != NULL) {
      mismatch_except = TRUE;
      report_errors = TRUE;
    }
    if (StringStr (sfp->except_text, "artificial frameshift") != NULL) {
      frameshift_except = TRUE;
      report_errors = TRUE;
    }
    if (StringStr (sfp->except_text, "rearrangement required for product") != NULL) {
      rearrange_except = TRUE;
    }
  }

  for (gb = sfp->qual; gb != NULL; gb = gb->next) {     /* pseuogene */
    if (!StringICmp ("pseudo", gb->qual))
      return;
  }

  crp = (CdRegionPtr) (sfp->data.value.ptrvalue);
  if (crp->code_break == NULL) {        /* check for unparsed transl_except */
    for (gb = sfp->qual; gb != NULL; gb = gb->next) {
      if (StringCmp (gb->qual, "transl_except") == 0) {
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

  newprot = ProteinFromCdRegionExEx (sfp, TRUE, FALSE, &alt_start);   /* include stop codons, do not remove trailing X/B/Z */
  if (newprot == NULL) {
    has_errors = TRUE;
    if (report_errors || unclassified_except) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_CdTransFail, "Unable to translate");
    }
    prot_ok = FALSE;
    goto erret;
  }

  if (alt_start && gccode == 1) {
    sev = SEV_WARNING;
    if (Loc_is_RefSeq (sfp->location)) {
      sev = SEV_ERROR;
    } else if (Loc_is_GEDL (sfp->location)) {
      sev = SEV_NONE;
    }
    if (sfp->excpt && (! StringHasNoText (sfp->except_text))) {
      if (StringStr (sfp->except_text, "alternative start codon") != NULL) {
        sev = SEV_NONE;
      }
    }
    if (sev > SEV_NONE) {
      has_errors = TRUE;
      if (report_errors) {
        ValidErr (vsp, sev, ERR_SEQ_FEAT_AltStartCodon, "Alternative start codon used");
      }
    }
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
        has_errors = TRUE;
        if (report_errors) {
          ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_TranslExceptPhase, "transl_except qual out of frame.");
        }
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
      has_errors = TRUE;
      if (report_errors) {
        ValidErr (vsp, sev, ERR_SEQ_FEAT_PartialProblem, "Suspicious CDS location - frame > 1 but not 5' partial");
      }
    } else if ((part_loc & SLP_NOSTART) && (!PartialAtSpliceSite (sfp->location, SLP_NOSTART))) {
      sev = SEV_INFO;
      if (Loc_is_RefSeq (sfp->location)) {
        sev = SEV_ERROR;
      }
      has_errors = TRUE;
      if (report_errors) {
        ValidErr (vsp, sev, ERR_SEQ_FEAT_PartialProblem, "Suspicious CDS location - frame > 1 and not at consensus splice site");
      }
    }
  }

  if ((part_loc & SLP_START) || (part_prod & SLP_START))
    no_beg = TRUE;

  protseq = BSMerge (newprot, NULL);
  prot2len = StringLen (protseq);
  if (protseq != NULL) {
    len = prot2len;
    for (i = 0; i < len; i++) {
      residue1 = protseq [i];
      if ((i == 0) && (residue1 == '-'))
        got_dash = TRUE;
      if (residue1 == '*') {
        if (i == (len - 1))
          got_stop = TRUE;
        else
          stop_count++;
      }
    }
  }

  /*
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
  */

  if (stop_count > 0) {
    if (got_dash) {
      has_errors = TRUE;
      sev = SEV_ERROR;
      if (unclassified_except) {
        sev = SEV_WARNING;
      }
      if (report_errors || unclassified_except) {
        ValidErr (vsp, sev, ERR_SEQ_FEAT_StartCodon,
                  "Illegal start codon and %ld internal stops. Probably wrong genetic code [%d]", (long) stop_count, gccode);
      }
    } else {
      has_errors = TRUE;
      sev = SEV_ERROR;
      if (unclassified_except) {
        sev = SEV_WARNING;
      }
      if (report_errors || unclassified_except) {
        ValidErr (vsp, sev, ERR_SEQ_FEAT_InternalStop, "%ld internal stops. Genetic code [%d]", (long) stop_count, gccode);
      }
    }
    prot_ok = FALSE;
    if (stop_count > 5)
      goto erret;
  } else if (got_dash) {
    has_errors = TRUE;
    if (report_errors) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_StartCodon, "Illegal start codon used. Wrong genetic code [%d] or protein should be partial", gccode);
    }
  }

  show_stop = TRUE;

  protid = SeqLocId (sfp->product);
  if (protid != NULL) {
    prot1seq = BioseqFind (protid);
    if (prot1seq == NULL && vsp->farFetchCDSproducts) {
      prot1seq = BioseqLockById (protid);
      if (prot1seq != NULL) {
        unlockProd = TRUE;
        farstr = "(Far) ";
      }
    }
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
          has_errors = TRUE;
          if (report_errors) {
            ValidErr (vsp, sev, ERR_SEQ_FEAT_NoProtein, "No protein Bioseq given");
          }
        }
      }
    }
    goto erret;
  }

  len = prot2len;

  if ((got_stop) && (len == (prot1len + 1))) {  /* ok, got stop */
    len--;
  }

  if (! StreamCacheSetup (prot1seq, NULL, STREAM_EXPAND_GAPS, &sc)) {
    goto erret;
  }
  /*
  spp = SeqPortNew (prot1seq, 0, -1, 0, Seq_code_ncbieaa);
  if (spp == NULL)
    goto erret;
  */

  /* ignore terminal 'X' from partial last codon if present */

  done = FALSE;
  if ((!done) && (prot1len)) {
    /* prime the cache at a reasonable position near the end */
    if (prot1len > 4000) {
      StreamCacheSetPosition (&sc, prot1len - 2000);
    }
    residue1 = StreamCacheGetResidue (&sc);
  }
  while ((!done) && (prot1len)) {
    StreamCacheSetPosition (&sc, prot1len - 1);
    residue1 = StreamCacheGetResidue (&sc);
    /*
    SeqPortSeek (spp, (prot1len - 1), SEEK_SET);
    residue1 = SeqPortGetResidue (spp);
    */
    if (residue1 == 'X')        /* remove terminal X */
      prot1len--;
    else
      done = TRUE;
  }
  done = FALSE;
  while ((!done) && (len)) {
    /*
    BSSeek (newprot, (len - 1), SEEK_SET);
    residue2 = BSGetByte (newprot);
    */
    residue2 = protseq [len - 1];
    if (residue2 == 'X')
      len--;
    else
      done = TRUE;
  }

  if (len == prot1len) {        /* could be identical */
    StreamCacheSetPosition (&sc, 0);
    /*
    SeqPortSeek (spp, 0, SEEK_SET);
    BSSeek (newprot, 0, SEEK_SET);
    */
    for (i = 0; i < len; i++) {
      residue1 = protseq [i];
      residue2 = StreamCacheGetResidue (&sc);
      /*
      residue1 = BSGetByte (newprot);
      residue2 = SeqPortGetResidue (spp);
      */
      if (residue1 != residue2) {
        prot_ok = FALSE;
        if (residue2 == INVALID_RESIDUE)
          residue2 = '?';
        if (mismatch == 10) {
          has_errors = TRUE;
          if (report_errors && (! mismatch_except)) {
            ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MisMatchAA, "More than 10 mismatches. Genetic code [%d]", gccode);
          }
          break;
        } else if (i == 0) {
          if ((sfp->partial) && (!no_beg) && (!no_end)) { /* ok, it's partial */
            has_errors = TRUE;
            if (report_errors) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "Start of location should probably be partial");
            }
          } else if (residue1 == '-') {
            has_errors = TRUE;
            if (report_errors) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_StartCodon, "Illegal start codon used. Wrong genetic code [%d] or protein should be partial", gccode);
            }
          } else {
            nuclocstr = MapToNTCoords (sfp, protid, i);
            if (nuclocstr != NULL) {
              has_errors = TRUE;
              if (report_errors && (! mismatch_except)) {
                ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MisMatchAA,
                          "%sResidue %ld in protein [%c] != translation [%c] at %s", farstr, (long) (i + 1), (char) residue2, (char) residue1, nuclocstr);
              }
            } else {
              has_errors = TRUE;
              if (report_errors && (! mismatch_except)) {
                ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MisMatchAA,
                          "%sResidue %ld in protein [%c] != translation [%c]", farstr, (long) (i + 1), (char) residue2, (char) residue1);
              }
            }
            MemFree (nuclocstr);
          }
        } else {
          nuclocstr = MapToNTCoords (sfp, protid, i);
          if (nuclocstr != NULL) {
            has_errors = TRUE;
            if (report_errors && (! mismatch_except)) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MisMatchAA,
                        "%sResidue %ld in protein [%c] != translation [%c] at %s", farstr, (long) (i + 1), (char) residue2, (char) residue1, nuclocstr);
            }
          } else {
            has_errors = TRUE;
            if (report_errors && (! mismatch_except)) {
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MisMatchAA,
                        "%sResidue %ld in protein [%c] != translation [%c]", farstr, (long) (i + 1), (char) residue2, (char) residue1);
            }
          }
          MemFree (nuclocstr);
        }
        mismatch++;
      }
    }
    /*
    spp = SeqPortFree (spp);
    */
  } else {
    has_errors = TRUE;
    if (report_errors) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_TransLen, "Given protein length [%ld] does not match %stranslation length [%ld]", prot1len, farstr, len);
    }
  }

  if ((sfp->partial) && (!mismatch)) {
    if ((!no_beg) && (!no_end)) {       /* just didn't label */
      if (!got_stop) {
        has_errors = TRUE;
        if (report_errors) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "End of location should probably be partial");
        }
      } else {
        has_errors = TRUE;
        if (report_errors) {
          ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "This SeqFeat should not be partial");
        }
      }
      show_stop = FALSE;
    }
  }

  if (unlockProd) {
    BioseqUnlock (prot1seq);
  }


erret:
  if (show_stop) {
    if ((!got_stop) && (!no_end)) {
      has_errors = TRUE;
      if (report_errors) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_NoStop, "Missing stop codon");
      }
    } else if ((got_stop) && (no_end)) {
      has_errors = TRUE;
      if (report_errors) {
        ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_PartialProblem, "Got stop codon, but 3'end is labeled partial");
      }
    } else if ((got_stop) && (!no_end) && (ragged)) {
      has_errors = TRUE;
      sev = SEV_ERROR;
      if (unclassified_except) {
        sev = SEV_WARNING;
      }
      if (report_errors || unclassified_except) {
        ValidErr (vsp, sev, ERR_SEQ_FEAT_TransLen, "Coding region extends %d base(s) past stop codon", (int) ragged);
      }
    }
  }

  if (!prot_ok) {
    if (transl_except) {
      has_errors = TRUE;
      if (report_errors) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_TranslExcept, "Unparsed transl_except qual. Skipped");
      }
    }
  }

  if (prot2seq != NULL)
    BioseqFree (prot2seq);
  else
    BSFree (newprot);
  /*
  SeqPortFree (spp);
  */
  MemFree (protseq);

  if (! report_errors) {
    if (! has_errors) {
      if ((! frameshift_except) && (! rearrange_except)) {
        ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnnecessaryException, "CDS has exception but passes translation test");
      }
    }
  }
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
  /*
  SeqPortPtr      spp = NULL;
  */
  SeqIdPtr        last_sip = NULL, sip, id;
  Int2            total, ctr;
  BioseqPtr       bsp = NULL;
  Int4            strt, stp, len = 0, donor, acceptor;
  Int2            residue1, residue2;
  Char            tbuf[40];
  Boolean         reportAsError, first, last, firstPartial, lastPartial, has_errors = FALSE, report_errors = TRUE;
  int             severity;
  Uint2           partialflag;
  Boolean         gpsOrRefSeq = FALSE;
  SeqEntryPtr     sep;
  BioseqSetPtr    bssp;
  TextSeqIdPtr    tsip;
  StreamCache     sc;
  SeqInt          sint;
  ValNode         vn;

  if (sfp == NULL)
    return;

  if (GetAppProperty ("NcbiSubutilValidation") != NULL)
    return;                     /* suppress if NCBISubValidate */


  /* specific biological exceptions suppress check */

  if (sfp->excpt) {
    if (StringISearch (sfp->except_text, "ribosomal slippage") != NULL||
        StringISearch (sfp->except_text, "artificial frameshift") != NULL ||
        StringISearch (sfp->except_text, "nonconsensus splice site") != NULL) {
      report_errors = FALSE;
    }
  }

  MemSet ((Pointer) &sint, 0, sizeof (SeqInt));
  MemSet ((Pointer) &vn, 0, sizeof (ValNode));

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
      gpsOrRefSeq = TRUE;
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
            /*
            if (StringNICmp (tsip->accession, "NT_", 3) == 0) {
              gpsOrRefSeq = TRUE;
            } else if (StringNICmp (tsip->accession, "NC_", 3) == 0) {
              gpsOrRefSeq = TRUE;
            } else if (StringNICmp (tsip->accession, "NG_", 3) == 0) {
              gpsOrRefSeq = TRUE;
            } else if (StringNICmp (tsip->accession, "NM_", 3) == 0) {
              gpsOrRefSeq = TRUE;
            } else if (StringNICmp (tsip->accession, "NR_", 3) == 0) {
              gpsOrRefSeq = TRUE;
            }
            */
            gpsOrRefSeq = TRUE;
          }
        }
      }
    }

    if ((ctr == 1) || (!SeqIdMatch (sip, last_sip))) {
      /* spp = SeqPortFree (spp); */
      bsp = BioseqLockById (sip);
      if (bsp == NULL)
        break;
      len = bsp->length;
      if (strand != Seq_strand_minus) {
        if (! StreamCacheSetup (bsp, NULL, STREAM_EXPAND_GAPS, &sc)) {
          BioseqUnlock (bsp);
          break;
        }
      } else {
        sint.from = 0;
        sint.to = len - 1;
        sint.strand = strand;
        sint.id = sip;
        vn.choice = SEQLOC_INT;
        vn.data.ptrvalue = (Pointer) &sint;
        vn.next = NULL;
        if (! StreamCacheSetup (NULL, &vn, STREAM_EXPAND_GAPS, &sc)) {
          BioseqUnlock (bsp);
          break;
        }
      }
      /* spp = SeqPortNew (bsp, 0, -1, strand, Seq_code_ncbi4na); */
      BioseqUnlock (bsp);
      /*
      if (spp == NULL)
        break;
      */
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
      StreamCacheSetPosition (&sc, stp + 1);
      residue1 = StreamCacheGetResidue (&sc);
      residue2 = StreamCacheGetResidue (&sc);
      /*
      SeqPortSeek (spp, (stp + 1), SEEK_SET);
      residue1 = SeqPortGetResidue (spp);
      residue2 = SeqPortGetResidue (spp);
      */
      if (IS_residue (residue1) && IS_residue (residue2)) {
        if (residue1 != 'G' || residue2 != 'T') {        /* not T */
          if (residue1 == 'G' && residue2 == 'C') {       /* GC minor splice site */
            tbuf[0] = '\0';
            if (bsp == NULL) {
              StringCpy (tbuf, "?");
            } else if (vsp->suppressContext || vsp->convertGiToAccn) {
              WorstBioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
            } else {
              BioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
            }
            tbuf[39] = '\0';
            has_errors = TRUE;
            if (report_errors) {
              ValidErr (vsp, SEV_INFO, ERR_SEQ_FEAT_NotSpliceConsensus,
                        "Rare splice donor consensus (GC) found instead of (GT) after exon ending at position %ld of %s", (long) (donor + 1), tbuf);
            }
          } else {
            if (gpsOrRefSeq) {
              severity = SEV_WARNING;
            } else if (checkAll) {
              severity = SEV_WARNING;
            } else if (reportAsError) {
              severity = SEV_ERROR;
            } else {
              severity = SEV_WARNING;
            }
            tbuf[0] = '\0';
            if (bsp == NULL) {
              StringCpy (tbuf, "?");
            } else if (vsp->suppressContext || vsp->convertGiToAccn) {
              WorstBioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
            } else {
              BioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
            }
            tbuf[39] = '\0';
            has_errors = TRUE;
            if (report_errors) {
              ValidErr (vsp, severity, ERR_SEQ_FEAT_NotSpliceConsensus,
                        "Splice donor consensus (GT) not found after exon ending at position %ld of %s", (long) (donor + 1), tbuf);
            }
          }
        }
      }
    }

    if (((checkAll && (!firstPartial)) || ctr != 1) && (strt > 1)) {
      StreamCacheSetPosition (&sc, strt - 2);
      residue1 = StreamCacheGetResidue (&sc);
      residue2 = StreamCacheGetResidue (&sc);
      /*
      SeqPortSeek (spp, (strt - 2), SEEK_SET);
      residue1 = SeqPortGetResidue (spp);
      residue2 = SeqPortGetResidue (spp);
      */
      if (IS_residue (residue1) && IS_residue (residue2)) {
        if (residue1 != 'A' || residue2 != 'G') {
          if (gpsOrRefSeq) {
            severity = SEV_WARNING;
          } else if (checkAll) {
            severity = SEV_WARNING;
          } else if (reportAsError) {
            severity = SEV_ERROR;
          } else {
            severity = SEV_WARNING;
          }
          tbuf[0] = '\0';
          if (bsp == NULL) {
            StringCpy (tbuf, "?");
          } else if (vsp->suppressContext || vsp->convertGiToAccn) {
            WorstBioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
          } else {
            BioseqLabel (bsp, tbuf, 39, OM_LABEL_CONTENT);
          }
          tbuf[39] = '\0';
          has_errors = TRUE;
          if (report_errors) {
            ValidErr (vsp, severity, ERR_SEQ_FEAT_NotSpliceConsensus,
                      "Splice acceptor consensus (AG) not found before exon starting at position %ld of %s", (long) (acceptor + 1), tbuf);
          }
        }
      }
    }
    first = FALSE;
    slp = nxt;
  }

  /* SeqPortFree (spp); */

  if (! report_errors) {
    if (! has_errors) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_UnnecessaryException, "feature has exception but passes splice site test");
    }
  }
}

NLM_EXTERN void SpliceCheck (ValidStructPtr vsp, SeqFeatPtr sfp)
{
  SpliceCheckEx (vsp, sfp, FALSE);
}

/*****************************************************************************
*
*   CdsProductIdCheck (vsp, sfp)
*      code taken from asn2gnbk.c - release mode expects CDS product Bioseqs
*
*****************************************************************************/
static void CdsProductIdCheck (ValidStructPtr vsp, SeqFeatPtr sfp)

{
  SeqFeatPtr         gene;
  GeneRefPtr         grp;
  Boolean            juststop = FALSE;
  Boolean            okay = FALSE;
  SeqEntryPtr        oldscope;
  Boolean            partial5;
  Boolean            partial3;
  Boolean            pseudo = FALSE;
  SeqEntryPtr        sep;

   /* non-pseudo CDS must have /product */
   if (sfp->pseudo) {
	 pseudo = TRUE;
   }
   grp = SeqMgrGetGeneXref (sfp);
   if (grp == NULL) {
	 sep = GetTopSeqEntryForEntityID (sfp->idx.entityID);
	 oldscope = SeqEntrySetScope (sep);
	 gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
	 SeqEntrySetScope (oldscope);
	 if (gene != NULL) {
	   grp = (GeneRefPtr) gene->data.value.ptrvalue;
	   if (gene->pseudo) {
		 pseudo = TRUE;
	   }
	 }
   }
   if (grp != NULL && grp->pseudo) {
	 pseudo = TRUE;
   }
   if (sfp->location != NULL) {
	 if (CheckSeqLocForPartial (sfp->location, &partial5, &partial3)) {
	   if (partial5 && (! partial3)) {
		 if (SeqLocLen (sfp->location) <= 5) {
		   juststop = TRUE;
		 }
	   }
	 }
   }
   if (pseudo || juststop) {
	 okay = TRUE;
   } else if (sfp->product != NULL) {
	 okay = TRUE;
   } else {
	 if (sfp->excpt && (! StringHasNoText (sfp->except_text))) {
	   if (StringStr (sfp->except_text, "rearrangement required for product") != NULL) {
		 okay = TRUE;
	   }
	 }
   }
   if (! okay) {
    ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_MissingCDSproduct, "Expected CDS product absent");
   }
}

/*****************************************************************************
*
*   ValidateSeqLoc(vsp, slp, prefix)
*
*****************************************************************************/

static Int2 SeqLocMixCount (SeqLocPtr slp)

{
  Int2       count = 0;
  SeqLocPtr  loc;

  if (slp == NULL) return 0;

  while (slp != NULL) {
    if (slp->choice == SEQLOC_MIX) {
      count++;
      loc = (SeqLocPtr) slp->data.ptrvalue;
      count += SeqLocMixCount (loc);
    }
    slp = slp->next;
  }

  return count;
}

NLM_EXTERN void ValidateSeqLoc (ValidStructPtr vsp, SeqLocPtr slp, CharPtr prefix)
{
  SeqLocPtr       tmp, prev;
  Boolean         retval = TRUE, tmpval, mixed_strand = FALSE, unmarked_strand = FALSE,
                  ordered = TRUE, adjacent = FALSE, circular = FALSE, exception = FALSE;
  CharPtr         ctmp;
  Uint1           strand2, strand1;
  ErrSev          sev;
  SeqIntPtr       sip1, sip2, prevsip;
  SeqPntPtr       spp;
  PackSeqPntPtr   pspp;
  SeqIdPtr        id1 = NULL, id2;
  BioseqPtr       bsp;
  SeqFeatPtr      sfp = NULL;

  if (slp == NULL)
    return;

  sfp = vsp->sfp;

  bsp = BioseqFindFromSeqLoc (slp);
  if (bsp != NULL && bsp->topology == 2) {
    circular = TRUE;
  }

  if (SeqLocMixCount (slp) > 1) {
      retval = FALSE;
      ctmp = SeqLocPrint (slp);
      if (ctmp != NULL && StringLen (ctmp) > 800) {
        StringCpy (ctmp + 797, "...");
      }
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_NestedSeqLocMix, "%s: SeqLoc [%s] has nested SEQLOC_MIX elements", prefix, ctmp);
      MemFree (ctmp);
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
      if (ctmp != NULL && StringLen (ctmp) > 800) {
        StringCpy (ctmp + 797, "...");
      }
      ValidErr (vsp, SEV_REJECT, ERR_SEQ_FEAT_Range, "%s: SeqLoc [%s] out of range", prefix, ctmp);
      MemFree (ctmp);

    }

    if (tmp->choice != SEQLOC_NULL) {
      if ((strand1 != Seq_strand_other) && (strand2 != Seq_strand_other)) {
        if (SeqIdForSameBioseq (id1, id2)) {
          if (strand1 != strand2) {
            if (strand1 == Seq_strand_plus && strand2 == Seq_strand_unknown) {
              unmarked_strand = TRUE;
            } else if (strand1 == Seq_strand_unknown && strand2 == Seq_strand_plus) {
              unmarked_strand = TRUE;
            } else {
              mixed_strand = TRUE;
            }
          }
        }
      }

      strand1 = strand2;
      id1 = id2;
    }
  }

  if (sfp != NULL) {

    /* Publication intervals ordering does not matter */

    if (sfp->idx.subtype == FEATDEF_PUB) {
      ordered = TRUE;
      adjacent = FALSE;
    }

    /* ignore ordering of heterogen bonds */

    if (sfp->data.choice == SEQFEAT_HET) {
      ordered = TRUE;
      adjacent = FALSE;
    }

    /* misc_recomb intervals SHOULD be in reverse order */

    if (sfp->idx.subtype == FEATDEF_misc_recomb) {
      ordered = TRUE;
    }

    /* primer_bind intervals MAY be in on opposite strands */

    if (sfp->idx.subtype == FEATDEF_primer_bind) {
      mixed_strand = FALSE;
      unmarked_strand = FALSE;
      ordered = TRUE;
    }

    if (sfp->excpt) {
      exception = TRUE;
    }
  }

  if (adjacent) {
    ctmp = SeqLocPrint (slp);
    if (exception) {
      sev = SEV_WARNING;
    } else {
      sev = SEV_ERROR;
    }
    if (ctmp != NULL && StringLen (ctmp) > 800) {
      StringCpy (ctmp + 797, "...");
    }
    ValidErr (vsp, sev, ERR_SEQ_FEAT_AbuttingIntervals, "%s: Adjacent intervals in SeqLoc [%s]", prefix, ctmp);
    MemFree (ctmp);
  }

  if (exception) {
    /* trans splicing exception turns off both mixed_strand and out_of_order messages */
    if (StringISearch (sfp->except_text, "trans-splicing") != NULL) {
      return;
    }
  }

  if (mixed_strand || unmarked_strand || (!ordered)) {
    ctmp = SeqLocPrint (slp);
    if (ctmp != NULL && StringLen (ctmp) > 800) {
      StringCpy (ctmp + 797, "...");
    }
    if (mixed_strand) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_MixedStrand, "%s: Mixed strands in SeqLoc [%s]", prefix, ctmp);
    } else if (unmarked_strand) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_FEAT_MixedStrand, "%s: Mixed plus and unknown strands in SeqLoc [%s]", prefix, ctmp);
    }
    if (!ordered)
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_SeqLocOrder, "%s: Intervals out of order in SeqLoc [%s]", prefix, ctmp);
    MemFree (ctmp);
    return;
  }

  if (sfp != NULL) {

    /* ignore special case features here as well */

    if (sfp->idx.subtype == FEATDEF_PUB ||
        sfp->data.choice == SEQFEAT_HET ||
        sfp->idx.subtype == FEATDEF_misc_recomb ||
        sfp->idx.subtype == FEATDEF_primer_bind)
      return;
  }

  /* newer check for intervals out of order on segmented bioseq */

  if (bsp == NULL || bsp->repr != Seq_repr_seg) return;

  if (SeqLocBadSortOrder (bsp, slp)) {
    ctmp = SeqLocPrint (slp);
    if (ctmp != NULL && StringLen (ctmp) > 800) {
      StringCpy (ctmp + 797, "...");
    }
    ValidErr (vsp, SEV_ERROR, ERR_SEQ_FEAT_SeqLocOrder, "%s: Intervals out of order in SeqLoc [%s]", prefix, ctmp);
    MemFree (ctmp);
  }

  /* newer check for mixed strand on segmented bioseq */

  if (SeqLocMixedStrands (bsp, slp)) {
    ctmp = SeqLocPrint (slp);
    if (ctmp != NULL && StringLen (ctmp) > 800) {
      StringCpy (ctmp + 797, "...");
    }
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

static void GetGraphsProc (SeqGraphPtr sgp, Pointer userdata)
{
  GphGetPtr       ggp;
  GrphItemPtr     gip;

  ggp = (GphGetPtr) userdata;
  if (ggp == NULL || sgp == NULL) return;
  /* only phrap or gap4 currently allowed */
  if (StringICmp (sgp->title, "Phrap Quality") == 0 || StringICmp (sgp->title, "Phred Quality") == 0 || StringICmp (sgp->title, "Gap4") == 0) {
    /* data type must be bytes */
    if (sgp->flags[2] == 3) {
      if (SeqIdIn (SeqLocId (sgp->loc), ggp->bsp->id)) {
        gip = (GrphItemPtr) MemNew (sizeof (GrphItem));
        if (gip == NULL) return;
        gip->sgp = sgp;
        gip->left = GetOffsetInBioseq (sgp->loc, ggp->bsp, SEQLOC_LEFT_END);
        gip->right = GetOffsetInBioseq (sgp->loc, ggp->bsp, SEQLOC_RIGHT_END);
        ValNodeAddPointer (&(ggp->vnp), 0, (Pointer) gip);
      }
    }
  }
  return;
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
  GphGetData   ggd;
  GrphItemPtr  gip;
  Int2         index;
  ValNodePtr   vnp;

  ggd.vnp = NULL;
  ggd.bsp = bsp;
  VisitGraphsOnBsp (bsp, (Pointer) &ggd, GetGraphsProc);
  for (vnp = ggd.vnp, index = 1; vnp != NULL; vnp = vnp->next, index++) {
    gip = (GrphItemPtr) vnp->data.ptrvalue;
    if (gip != NULL) {
      gip->index = index;
    }
  }
  ggd.vnp = ValNodeSort (ggd.vnp, SortSeqGraphProc);
  return ggd.vnp;
}

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

static void ValidateGraphsOnBioseq (GatherContextPtr gcp)
{
  Byte            bases[400], scores [400];
  ByteStorePtr    bs;
  BioseqPtr       bsp;
  Int2            ctr, i, k, val, index, scount;
  Int4            curroffset = 0, gphlen = 0, seqlen = 0, slplen,
                  bslen, min = INT4_MAX, max = INT4_MIN, j, lastloc = -1,
                  NsWithScore, GapsWithScore, ACGTsWithoutScore, valsBelowMin,
                  valsAboveMax, firstN, firstACGT, pos, litlen, nxtlen;
  DeltaSeqPtr     dsp, next;
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

    if (sgp->max.intvalue > 100) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_GRAPH_GraphMax, "Graph max (%ld) out of range", (long) sgp->max.intvalue);
    }
    if (sgp->max.intvalue <= 0) {
      ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphMax, "Graph max (%ld) out of range", (long) sgp->max.intvalue);
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
      for (dsp = (DeltaSeqPtr) (bsp->seq_ext), vnp = head; dsp != NULL && vnp != NULL; dsp = next) {
        next = dsp->next;
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
          litlen = 0;
          if (slp != NULL) {
            litlen = slp->length;
          }
          if (slp != NULL && slp->seq_data != NULL) {
            while (NextLitLength (next, &nxtlen)) {
              litlen += nxtlen;
              next = next->next;
            }
            if (sgp->numval != litlen) {
              gcp->itemID = sgp->idx.itemID;
              ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphSeqLitLen, "SeqGraph (%ld) and SeqLit (%ld) length mismatch",
                        (long) sgp->numval, (long) litlen);
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
                if (sintp->to != litlen + curroffset - 1) {
                  gcp->itemID = sgp->idx.itemID;
                  ValidErr (vsp, SEV_ERROR, ERR_SEQ_GRAPH_GraphStopPhase, "SeqGraph (%ld) and SeqLit (%ld) stop do not coincide",
                            (long) sintp->to, (long) (litlen + curroffset - 1));
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
            curroffset += litlen;
          }
          break;
        default:
          break;
        }
      }
      for (dsp = (DeltaSeqPtr) (bsp->seq_ext), numdsp = 0; dsp != NULL; dsp = next) {
        next = dsp->next;
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
            while (NextLitLength (next, &nxtlen)) {
              next = next->next;
            }
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

    scount = (Int2) BSRead (bs, scores, sizeof (scores));
    k = 0;

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
        /* val = (Int2) BSGetByte (bs); */
        if (k >= scount) {
          if (scount > 0) {
            scount = (Int2) BSRead (bs, scores, sizeof (scores));
          }
          k = 0;
        }
        if (scount > 0) {
          val = (Int2) scores [k];
          k++;
        } else {
          val = 0;
        }
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
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_GRAPH_GraphBelow, "%ld quality scores have values below the reported minimum or 0", (long) valsBelowMin);
    }
    if (valsAboveMax > 0) {
      ValidErr (vsp, SEV_WARNING, ERR_SEQ_GRAPH_GraphAbove, "%ld quality scores have values above the reported maximum or 100", (long) valsAboveMax);
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
