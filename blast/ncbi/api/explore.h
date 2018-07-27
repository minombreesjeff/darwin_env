/*  explore.h
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
* File Name:  explore.h
*
* Author:  Jonathan Kans, Jinghui Zhang, James Ostell
*   
* Version Creation Date: 6/30/98
*
* $Revision: 6.46 $
*
* File Description:  Reengineered and optimized exploration functions
*                      to be used for future code
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*/

#ifndef _NCBI_Explore_
#define _NCBI_Explore_

#ifndef _NCBI_Seqset_
#include <objsset.h>
#endif

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   SeqMgrBioseqContext, SeqMgrSegmentContext, SeqMgrDescContext and SeqMgrFeatContext
*     are data structures supporting the collection of bioseqs, parts of segmented
*     bioseqs, descriptors, and features, respectively
*
*****************************************************************************/

typedef struct seqmgrbioseqcontext {
  Uint2         entityID;
  Uint4         itemID;
  BioseqPtr     bsp;
  SeqEntryPtr   sep;
  BioseqSetPtr  bssp;
  Int4          numsegs;
  Pointer       userdata;
                          /* the following fields are for internal use only */
  Pointer       omdp;
  Uint4         index;
} SeqMgrBioseqContext, PNTR SeqMgrBioseqContextPtr;

typedef struct seqmgrsegmentcontext {
  Uint2         entityID;
  Uint4         itemID;
  SeqLocPtr     slp;
  BioseqPtr     parent;
  Int4          cumOffset;
  Int4          from;
  Int4          to;
  Uint1         strand;
  Pointer       userdata;
                          /* the following fields are for internal use only */
  Pointer       omdp;
  Uint4         index;
} SeqMgrSegmentContext, PNTR SeqMgrSegmentContextPtr;

typedef struct seqmgrdesccontext {
  Uint2         entityID;
  Uint4         itemID;
  ValNodePtr    sdp;
  SeqEntryPtr   sep;
  Uint2         level;
  Uint1         seqdesctype;
  Pointer       userdata;
                          /* the following fields are for internal use only */
  Pointer       omdp;
  Uint4         index;
} SeqMgrDescContext, PNTR SeqMgrDescContextPtr;

typedef struct seqmgrfeatcontext {
  Uint2         entityID;
  Uint4         itemID;
  SeqFeatPtr    sfp;
  SeqAnnotPtr   sap;
  BioseqPtr     bsp;
  CharPtr       label;
  Int4          left;
  Int4          right;
  Int4          dnaStop;
  Boolean       partialL;
  Boolean       partialR;
  Boolean       farloc;
  Uint1         strand;
  Uint1         seqfeattype;
  Uint1         featdeftype;
  Int2          numivals;
  Int4Ptr       ivals;
  Pointer       userdata;
                          /* the following fields are for internal use only */
  Pointer       omdp;
  Uint4         index;
} SeqMgrFeatContext, PNTR SeqMgrFeatContextPtr;

/*****************************************************************************
*
*   SeqMgrIndexFeatures builds indices of sorted features for all bioseqs in an
*     entity, makes explicit connections from a protein bioseq to its best protein
*     feature and to the CDS feature encoding it, can be called given an entityID
*     or a BioseqPtr or SeqEntryPtr, and returns the entityID
*
*****************************************************************************/

NLM_EXTERN Uint2 LIBCALL SeqMgrIndexFeatures (
  Uint2 entityID,
  Pointer ptr
);


/*****************************************************************************
*
*   To find the best protein feature for a CDS, first call SeqMgrGetProtXref.
*     If it is NULL, call SeqMgrGetBestProteinFeature.
*   SeqMgrGetBestProteinFeature and SeqMgrGetCDSgivenProduct take a protein
*     bioseq to get the best protein feature or encoding CDS
*   SeqMgrGetRNAgivenProduct takes an mRNA (cDNA) bioseq and gets the encoding
*     mRNA feature on the genomic bioseq
*   SeqMgrGetPROTgivenProduct takes an protein bioseq and gets the encoding
*     processed protein feature on a precursor protein bioseq
*
*****************************************************************************/

NLM_EXTERN ProtRefPtr LIBCALL SeqMgrGetProtXref (
  SeqFeatPtr sfp
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetBestProteinFeature (
  BioseqPtr bsp,
  SeqMgrFeatContext PNTR context
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetCDSgivenProduct (
  BioseqPtr bsp,
  SeqMgrFeatContext PNTR context
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetRNAgivenProduct (
  BioseqPtr bsp,
  SeqMgrFeatContext PNTR context
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetPROTgivenProduct (
  BioseqPtr bsp,
  SeqMgrFeatContext PNTR context
);

/*****************************************************************************
*
*   To find the best gene feature, first call SeqMgrGetGeneXref, and if it is not
*     NULL call SeqMgrGeneIsSuppressed, otherwise call SeqMgrGetOverlappingGene,
*     passing sfp->location
*   If desired, place a SeqMgrFeatContext data structure on the stack, and pass
*     in &context as the second parameter to SeqMgrGetOverlappingGene
*
*****************************************************************************/

NLM_EXTERN GeneRefPtr LIBCALL SeqMgrGetGeneXref (
  SeqFeatPtr sfp
);

NLM_EXTERN Boolean LIBCALL SeqMgrGeneIsSuppressed (
  GeneRefPtr grp
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingGene (
  SeqLocPtr slp,
  SeqMgrFeatContext PNTR context
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingOperon (
  SeqLocPtr slp,
  SeqMgrFeatContext PNTR context
);

/*****************************************************************************
*
*   SeqMgrGetOverlappingXXX returns the overlapping mRNA/CDS/publication/biosource
*     feature
*   If desired, place a SeqMgrFeatContext data structure on the stack, and pass
*     in &context as the second parameter
*
*****************************************************************************/

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingmRNA (
  SeqLocPtr slp,
  SeqMgrFeatContext PNTR context
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingCDS (
  SeqLocPtr slp,
  SeqMgrFeatContext PNTR context
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingPub (
  SeqLocPtr slp,
  SeqMgrFeatContext PNTR context
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingSource (
  SeqLocPtr slp,
  SeqMgrFeatContext PNTR context
);

/*****************************************************************************
*
*   Replacements for BioseqContext functions using bioseq feature indices, returning
*     the next (sorted) feature or (bioseq to highest set) descriptor pointer
*   The SeqMgrDescContext or SeqMgrFeatContext data structures should be on the
*     calling function's stack, and are passed as &context to the context function.
*   Passing NULL for curr in the first call initializes the context structure, and
*     the functions return NULL at the end of the list
*   If the choice parameters are 0, every feature or descriptor is returned
*   It is expected that these calls would be flanked by BioseqLock and BioseqUnlock,
*     so object manager reload could ensure that pointers are valid within the loop,
*     since the pointers are what drive these functions
*   The Explore functions below offer more flexibility than these Context functions
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL SeqMgrGetNextDescriptor (
  BioseqPtr bsp,
  ValNodePtr curr,
  Uint1 seqDescChoice,
  SeqMgrDescContext PNTR context
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetNextFeature (
  BioseqPtr bsp,
  SeqFeatPtr curr,
  Uint1 seqFeatChoice,
  Uint1 featDefChoice,
  SeqMgrFeatContext PNTR context
);

/*****************************************************************************
*
*   Callback types for SeqMgrExploreBioseqs, SeqMgrExploreSegments,
*     SeqMgrExploreDescriptors, and SeqMgrExploreFeatures
*
*****************************************************************************/

typedef Boolean (LIBCALLBACK *SeqMgrBioseqExploreProc) (
  BioseqPtr bsp,
  SeqMgrBioseqContextPtr context
);

typedef Boolean (LIBCALLBACK *SeqMgrSegmentExploreProc) (
  SeqLocPtr slp,
  SeqMgrSegmentContextPtr context
);

typedef Boolean (LIBCALLBACK *SeqMgrDescExploreProc) (
  ValNodePtr sdp,
  SeqMgrDescContextPtr context
);

typedef Boolean (LIBCALLBACK *SeqMgrFeatExploreProc) (
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr context
);

/*****************************************************************************
*
*   SeqMgrExploreBioseqs, SeqMgrExploreSegments, SeqMgrExploreDescriptors, and
*     SeqMgrExploreFeatures use the bioseq feature indices to quickly present
*     desired items to the user-supplied callback function, stopping if the callback
*     returns FALSE
*   In contrast to the SeqMgrGetNext functions, the SeqMgrExplore function callbacks
*     pass a pointer to the SeqMgr[Bioseq/Segment/Desc/Feat]Context data structures
*     held by the explore function, not on the calling function's stack
*   If the filter parameters are NULL, every feature or descriptor is returned,
*     otherwise the array lengths should be SEQDESCR_MAX, SEQFEAT_MAX, and
*     FEATDEF_MAX, and the elements are from the Seq_descr_, SEQFEAT_, and FEATDEF_
*     lists
*   It is expected that these calls would be flanked by BioseqLock and BioseqUnlock,
*     so object manager reload could ensure that pointers are valid within the loop,
*     but these explore functions can work on cached-out records
*   All of these functions return the number of times the callback was called
*
*****************************************************************************/

NLM_EXTERN Int4 LIBCALL SeqMgrExploreBioseqs (
  Uint2 entityID,
  Pointer ptr,
  Pointer userdata,
  SeqMgrBioseqExploreProc userfunc,
  Boolean nucs,
  Boolean prots,
  Boolean parts
);

NLM_EXTERN Int4 LIBCALL SeqMgrExploreSegments (
  BioseqPtr bsp,
  Pointer userdata,
  SeqMgrSegmentExploreProc userfunc
);

NLM_EXTERN Int4 LIBCALL SeqMgrExploreDescriptors (
  BioseqPtr bsp,
  Pointer userdata,
  SeqMgrDescExploreProc userfunc,
  BoolPtr seqDescFilter
);

NLM_EXTERN Int4 LIBCALL SeqMgrExploreFeatures (
  BioseqPtr bsp,
  Pointer userdata,
  SeqMgrFeatExploreProc userfunc,
  SeqLocPtr locationFilter,
  BoolPtr seqFeatFilter,
  BoolPtr featDefFilter
);

/*****************************************************************************
*
*   SeqMgrGetFeatureByLabel returns the a feature with the desired label
*   If desired, place a SeqMgrFeatContext data structure on the stack, and pass
*     in &context as the last parameter
*   SeqMgrGetNextFeatureByLabel works like SeqMgrGetNextFeature, except that
*     the order is by alphabetized label, not by chromosomal position
*
*****************************************************************************/

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetFeatureByLabel (
  BioseqPtr bsp,
  CharPtr label,
  Uint1 seqFeatChoice,
  Uint1 featDefChoice,
  SeqMgrFeatContext PNTR context
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetNextFeatureByLabel (
  BioseqPtr bsp,
  SeqFeatPtr curr,
  Uint1 seqFeatChoice,
  Uint1 featDefChoice,
  SeqMgrFeatContext PNTR context
);

/*****************************************************************************
*
*   SeqMgrGetNextGeneByLocusTag works like SeqMgrGetNextFeatureByLabel, except
*     that it returns only genes ordered by locus_tag
*
*****************************************************************************/

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetNextGeneByLocusTag (
  BioseqPtr bsp,
  SeqFeatPtr curr,
  SeqMgrFeatContext PNTR context
);

/*****************************************************************************
*
*   SeqMgrGetDesiredDescriptor and SeqMgrGetDesiredFeature return a descriptor
*     or feature given either an itemID, a position index, or the feature or
*     descriptor pointer itself, using whichever parameter is not 0 (or NULL)
*   In order to obtain index information associated with the desired descriptor
*     or feature, place a SeqMgrDescContext or SeqMgrFeatContext data structure
*     on the stack, and pass in &context as the last parameter
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL SeqMgrGetDesiredDescriptor (
  Uint2 entityID,
  BioseqPtr bsp,
  Uint4 itemID,
  Uint4 index,
  ValNodePtr sdp,
  SeqMgrDescContext PNTR context
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetDesiredFeature (
  Uint2 entityID,
  BioseqPtr bsp,
  Uint4 itemID,
  Uint4 index,
  SeqFeatPtr sfp,
  SeqMgrFeatContext PNTR context
);

/*****************************************************************************
*
*   SeqMgrVisitDescriptors and SeqMgrVisitFeatures visit all descriptors or
*     features in an entity in order of itemID, which is assigned by order of
*     packaging within the record
*   Both of these functions return the number of times the callback was called
*
*****************************************************************************/

NLM_EXTERN Int2 LIBCALL SeqMgrVisitDescriptors (
  Uint2 entityID,
  Pointer userdata,
  SeqMgrDescExploreProc userfunc,
  BoolPtr seqDescFilter
);

NLM_EXTERN Int2 LIBCALL SeqMgrVisitFeatures (
  Uint2 entityID,
  Pointer userdata,
  SeqMgrFeatExploreProc userfunc,
  BoolPtr seqFeatFilter,
  BoolPtr featDefFilter
);

/*****************************************************************************
*
*   BioseqFindFromSeqLoc finds the segmented bioseq if location is join on parts,
*     and does so even if some of the intervals are far accessions.
*
*****************************************************************************/

NLM_EXTERN BioseqPtr BioseqFindFromSeqLoc (
  SeqLocPtr loc
);

/*****************************************************************************
*
*   SeqMgrGetParentOfPart returns the segmented bioseq parent of a part bioseq,
*     and fills in the context structure.
*
*****************************************************************************/

NLM_EXTERN BioseqPtr LIBCALL SeqMgrGetParentOfPart (
  BioseqPtr bsp,
  SeqMgrSegmentContext PNTR context
);

/*****************************************************************************
*
*   SeqMgrGetBioseqContext fills in the context structure for any bioseq.
*
*****************************************************************************/

NLM_EXTERN Boolean LIBCALL SeqMgrGetBioseqContext (
  BioseqPtr bsp,
  SeqMgrBioseqContext PNTR context
);

/*****************************************************************************
*
*   SeqMgrBuildFeatureIndex builds a sorted array index for any feature type
*     (gene, mRNA, CDS, publication, and biosource have built-in arrays)
*   SeqMgrGetOverlappingFeature uses the array, or a feature subtype (chocies
*     are FEATDEF_GENE, FEATDEF_CDS, FEATDEF_mRNA, FEATDEF_PUB, or FEATDEF_BIOSRC)
*     to find feature overlap, requiring either that the location be completely
*     contained within the feature range, or merely that it be overlapped by the
*     feature, and returns the position in the index
*   SeqMgrGetFeatureInIndex gets an arbitrary feature indexed by the array
*   SeqMgrGetAllOverlappingFeatures returns all features that overlap with the
*     indicated overlap specificity
*   TestFeatOverlap checks to see if feature A is in feature B with the
*     indicated overlap specificity, -1 return value on failure
*
*****************************************************************************/

#define SIMPLE_OVERLAP   0 /* any overlap of extremes */
#define CONTAINED_WITHIN 1 /* contained within extremes */
#define LOCATION_SUBSET  2 /* SeqLocAinB must be satisfied, no boundary checking */
#define CHECK_INTERVALS  3 /* SeqLocAinB plus internal exon-intron boundaries must match */
#define INTERVAL_OVERLAP 4 /* at least one pair of intervals must overlap */
#define COMMON_INTERVAL  5 /* at least one pair of intervals must match */

NLM_EXTERN VoidPtr LIBCALL SeqMgrBuildFeatureIndex (
  BioseqPtr bsp,
  Int4Ptr num,
  Uint1 seqFeatChoice,
  Uint1 featDefChoice
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetOverlappingFeature (
  SeqLocPtr slp,
  Uint2 subtype,
  VoidPtr featarray,
  Int4 numfeats,
  Int4Ptr position,
  Int2 overlapType,
  SeqMgrFeatContext PNTR context
);

NLM_EXTERN SeqFeatPtr LIBCALL SeqMgrGetFeatureInIndex (
  BioseqPtr bsp,
  VoidPtr featarray,
  Int4 numfeats,
  Uint4 index,
  SeqMgrFeatContext PNTR context
);

NLM_EXTERN Int2 LIBCALL SeqMgrGetAllOverlappingFeatures (
  SeqLocPtr slp,
  Uint2 subtype,
  VoidPtr featarray,
  Int4 numfeats,
  Int2 overlapType,
  Pointer userdata,
  SeqMgrFeatExploreProc userfunc
);

NLM_EXTERN Int4 TestFeatOverlap (
  SeqFeatPtr sfpA,
  SeqFeatPtr sfpB,
  Int2 overlapType
);

/* the following functions are not frequently called by applications */

/*****************************************************************************
*
*   SeqMgrFeaturesAreIndexed returns the last time feature indices were built,
*     with 0 meaning that indices are not present on the entity
*
*****************************************************************************/

NLM_EXTERN time_t LIBCALL SeqMgrFeaturesAreIndexed (
  Uint2 entityID
);

/*****************************************************************************
*
*   SeqMgrClearFeatureIndexes clears feature indices for an entity given an
*     entityID or a BioseqPtr or SeqEntryPtr
*
*****************************************************************************/

NLM_EXTERN Boolean LIBCALL SeqMgrClearFeatureIndexes (
  Uint2 entityID,
  Pointer ptr
);

/*****************************************************************************
*
*   SeqMgrIndexFeaturesEx allows indexing on opposite strand for upside-down Web views
*
*****************************************************************************/

NLM_EXTERN Uint2 LIBCALL SeqMgrIndexFeaturesEx (
  Uint2 entityID,
  Pointer ptr,
  Boolean flip,
  Boolean dorevfeats
);

/*****************************************************************************
*
*   If indexed with dorevfeats TRUE, SeqMgrExploreFeaturesRev presents features
*     in reverse order, used for asn2gb master style on far genomic contigs that
*     point to the minus strand of a component
*
*****************************************************************************/

NLM_EXTERN Int4 LIBCALL SeqMgrExploreFeaturesRev (
  BioseqPtr bsp,
  Pointer userdata,
  SeqMgrFeatExploreProc userfunc,
  SeqLocPtr locationFilter,
  BoolPtr seqFeatFilter,
  BoolPtr featDefFilter
);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _NCBI_Explore_ */

