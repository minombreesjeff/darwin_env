#ifndef _objcdd_ 
#define _objcdd_ 
#ifndef NLM_GENERATED_CODE_PROTO
#define NLM_GENERATED_CODE_PROTO
#endif

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" { /* } */
#endif


/**************************************************
*
*    Generated objects for Module NCBI-Cdd
*    Generated using ASNCODE Revision: 6.15 at Oct 12, 2004 11:03 AM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objcddAsnLoad PROTO((void));
typedef ValNodePtr CddIdPtr;
typedef ValNode CddId;
#define CddId_uid 1
#define CddId_gid 2


NLM_EXTERN CddIdPtr LIBCALL CddIdFree PROTO ((CddIdPtr ));
NLM_EXTERN CddIdPtr LIBCALL CddIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddIdAsnWrite PROTO (( CddIdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddIdSet
*
**************************************************/
typedef ValNode CddIdSet;
typedef ValNodePtr CddIdSetPtr;
#define CddIdSetNew() ValNodeNew(NULL) 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN CddIdSetPtr LIBCALL CddIdSetFree PROTO ((CddIdSetPtr ));
NLM_EXTERN CddIdSetPtr LIBCALL CddIdSetNew PROTO (( void ));
NLM_EXTERN CddIdSetPtr LIBCALL CddIdSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddIdSetAsnWrite PROTO (( CddIdSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    Cdd
*
**************************************************/
typedef struct struct_Cdd {
   struct struct_Cdd PNTR next;
   CharPtr   name;
   ValNodePtr   id;
   ValNodePtr   description;
   struct seqannot PNTR   seqannot;
   struct struct_Biostruc_annot_set PNTR   features;
   ValNodePtr   sequences;
   struct seqint PNTR   profile_range;
   struct bioseq PNTR   trunc_master;
   struct struct_Matrix PNTR   posfreq;
   struct struct_Matrix PNTR   scoremat;
   struct struct_Triangle PNTR   distance;
   ValNodePtr   parent;
   ValNodePtr   children;
   ValNodePtr   siblings;
   ValNodePtr   neighbors;
   struct struct_Update_align PNTR   pending;
   struct struct_Reject_id PNTR   rejects;
   ValNodePtr   master3d;
   struct struct_Align_annot PNTR   alignannot;
   struct struct_Cn3d_style_dictionary PNTR   style_dictionary;
   struct struct_Cn3d_user_annotations PNTR   user_annotations;
   struct struct_Domain_parent PNTR   ancestors;
   struct struct_PssmWithParameters PNTR   scoreparams;
   struct struct_Sequence_tree PNTR   seqtree;
} Cdd, PNTR CddPtr;


NLM_EXTERN CddPtr LIBCALL CddFree PROTO ((CddPtr ));
NLM_EXTERN CddPtr LIBCALL CddNew PROTO (( void ));
NLM_EXTERN CddPtr LIBCALL CddAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddAsnWrite PROTO (( CddPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddSet
*
**************************************************/
typedef struct struct_Cdd CddSet;
typedef struct struct_Cdd PNTR CddSetPtr;
#define CddSetNew() CddNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN CddSetPtr LIBCALL CddSetFree PROTO ((CddSetPtr ));
NLM_EXTERN CddSetPtr LIBCALL CddSetNew PROTO (( void ));
NLM_EXTERN CddSetPtr LIBCALL CddSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddSetAsnWrite PROTO (( CddSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    CddTree
*
**************************************************/
typedef struct struct_Cdd_tree {
   struct struct_Cdd_tree PNTR next;
   CharPtr   name;
   ValNodePtr   id;
   ValNodePtr   description;
   ValNodePtr   parent;
   ValNodePtr   children;
   ValNodePtr   siblings;
   ValNodePtr   neighbors;
} CddTree, PNTR CddTreePtr;


NLM_EXTERN CddTreePtr LIBCALL CddTreeFree PROTO ((CddTreePtr ));
NLM_EXTERN CddTreePtr LIBCALL CddTreeNew PROTO (( void ));
NLM_EXTERN CddTreePtr LIBCALL CddTreeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddTreeAsnWrite PROTO (( CddTreePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddTreeSet
*
**************************************************/
typedef struct struct_Cdd_tree CddTreeSet;
typedef struct struct_Cdd_tree PNTR CddTreeSetPtr;
#define CddTreeSetNew() Cdd_treeNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN CddTreeSetPtr LIBCALL CddTreeSetFree PROTO ((CddTreeSetPtr ));
NLM_EXTERN CddTreeSetPtr LIBCALL CddTreeSetNew PROTO (( void ));
NLM_EXTERN CddTreeSetPtr LIBCALL CddTreeSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddTreeSetAsnWrite PROTO (( CddTreeSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    CddPrefNodes
*
**************************************************/
typedef struct struct_Cdd_pref_nodes {
   struct struct_Cdd_org_ref PNTR   preferred_nodes;
   struct struct_Cdd_org_ref PNTR   model_organisms;
   struct struct_Cdd_org_ref PNTR   optional_nodes;
   ValNodePtr   description;
} CddPrefNodes, PNTR CddPrefNodesPtr;


NLM_EXTERN CddPrefNodesPtr LIBCALL CddPrefNodesFree PROTO ((CddPrefNodesPtr ));
NLM_EXTERN CddPrefNodesPtr LIBCALL CddPrefNodesNew PROTO (( void ));
NLM_EXTERN CddPrefNodesPtr LIBCALL CddPrefNodesAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddPrefNodesAsnWrite PROTO (( CddPrefNodesPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddProject
*
**************************************************/
typedef struct struct_Cdd_Project {
   struct struct_Cdd PNTR   cds;
   ValNodePtr   cdcolor;
   struct struct_Cdd_Viewer PNTR   viewers;
   CharPtr   log;
   struct struct_Cdd_Script PNTR   scripts;
} CddProject, PNTR CddProjectPtr;


NLM_EXTERN CddProjectPtr LIBCALL CddProjectFree PROTO ((CddProjectPtr ));
NLM_EXTERN CddProjectPtr LIBCALL CddProjectNew PROTO (( void ));
NLM_EXTERN CddProjectPtr LIBCALL CddProjectAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddProjectAsnWrite PROTO (( CddProjectPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddOrgRef
*
**************************************************/
typedef struct struct_Cdd_org_ref {
   struct struct_Cdd_org_ref PNTR next;
   OrgRefPtr   reference;
   Uint1   active;
   Int4   parent_tax_id;
   CharPtr   rank;
} CddOrgRef, PNTR CddOrgRefPtr;


NLM_EXTERN CddOrgRefPtr LIBCALL CddOrgRefFree PROTO ((CddOrgRefPtr ));
NLM_EXTERN CddOrgRefPtr LIBCALL CddOrgRefNew PROTO (( void ));
NLM_EXTERN CddOrgRefPtr LIBCALL CddOrgRefAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddOrgRefAsnWrite PROTO (( CddOrgRefPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddOrgRefSet
*
**************************************************/
typedef struct struct_Cdd_org_ref CddOrgRefSet;
typedef struct struct_Cdd_org_ref PNTR CddOrgRefSetPtr;
#define CddOrgRefSetNew() Cdd_org_refNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN CddOrgRefSetPtr LIBCALL CddOrgRefSetFree PROTO ((CddOrgRefSetPtr ));
NLM_EXTERN CddOrgRefSetPtr LIBCALL CddOrgRefSetNew PROTO (( void ));
NLM_EXTERN CddOrgRefSetPtr LIBCALL CddOrgRefSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddOrgRefSetAsnWrite PROTO (( CddOrgRefSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */

typedef ValNodePtr CddPrefNodeDescrPtr;
typedef ValNode CddPrefNodeDescr;
#define CddPrefNodeDescr_create_date 1
#define CddPrefNodeDescr_description 2


NLM_EXTERN CddPrefNodeDescrPtr LIBCALL CddPrefNodeDescrFree PROTO ((CddPrefNodeDescrPtr ));
NLM_EXTERN CddPrefNodeDescrPtr LIBCALL CddPrefNodeDescrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddPrefNodeDescrAsnWrite PROTO (( CddPrefNodeDescrPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddPrefNodeDescrSet
*
**************************************************/
typedef ValNode CddPrefNodeDescrSet;
typedef ValNodePtr CddPrefNodeDescrSetPtr;
#define CddPrefNodeDescrSetNew() ValNodeNew(NULL) 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN CddPrefNodeDescrSetPtr LIBCALL CddPrefNodeDescrSetFree PROTO ((CddPrefNodeDescrSetPtr ));
NLM_EXTERN CddPrefNodeDescrSetPtr LIBCALL CddPrefNodeDescrSetNew PROTO (( void ));
NLM_EXTERN CddPrefNodeDescrSetPtr LIBCALL CddPrefNodeDescrSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddPrefNodeDescrSetAsnWrite PROTO (( CddPrefNodeDescrSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    GlobalId
*
**************************************************/
typedef struct struct_Global_id {
   CharPtr   accession;
   CharPtr   release;
   Int4   version;
   CharPtr   database;
} GlobalId, PNTR GlobalIdPtr;


NLM_EXTERN GlobalIdPtr LIBCALL GlobalIdFree PROTO ((GlobalIdPtr ));
NLM_EXTERN GlobalIdPtr LIBCALL GlobalIdNew PROTO (( void ));
NLM_EXTERN GlobalIdPtr LIBCALL GlobalIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GlobalIdAsnWrite PROTO (( GlobalIdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddRepeat
*
**************************************************/
typedef struct struct_Cdd_repeat {
   Int4   count;
   ValNodePtr   location;
   Int4   avglen;
} CddRepeat, PNTR CddRepeatPtr;


NLM_EXTERN CddRepeatPtr LIBCALL CddRepeatFree PROTO ((CddRepeatPtr ));
NLM_EXTERN CddRepeatPtr LIBCALL CddRepeatNew PROTO (( void ));
NLM_EXTERN CddRepeatPtr LIBCALL CddRepeatAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddRepeatAsnWrite PROTO (( CddRepeatPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddBookRef
*
**************************************************/
typedef struct struct_Cdd_book_ref {
   CharPtr   bookname;
   Uint2   textelement;
   /* following #defines are for enumerated type, not used by object loaders */
#define Cdd_book_ref_textelement_unassigned 0
#define Cdd_book_ref_textelement_section 1
#define Cdd_book_ref_textelement_figgrp 2
#define Cdd_book_ref_textelement_table 3
#define Cdd_book_ref_textelement_chapter 4
#define Cdd_book_ref_textelement_biblist 5
#define Cdd_book_ref_textelement_box 6
#define Cdd_book_ref_textelement_glossary 7
#define Cdd_book_ref_textelement_appendix 8
#define Cdd_book_ref_textelement_other 255

   Int4   elementid;
   Int4   subelementid;
} CddBookRef, PNTR CddBookRefPtr;


NLM_EXTERN CddBookRefPtr LIBCALL CddBookRefFree PROTO ((CddBookRefPtr ));
NLM_EXTERN CddBookRefPtr LIBCALL CddBookRefNew PROTO (( void ));
NLM_EXTERN CddBookRefPtr LIBCALL CddBookRefAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddBookRefAsnWrite PROTO (( CddBookRefPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr CddDescrPtr;
typedef ValNode CddDescr;
#define CddDescr_othername 1
#define CddDescr_category 2
#define CddDescr_comment 3
#define CddDescr_reference 4
#define CddDescr_create_date 5
#define CddDescr_tax_source 6
#define CddDescr_source 7
#define CddDescr_status 8
#define CddDescr_update_date 9
#define CddDescr_scrapbook 10
#define CddDescr_source_id 11
#define CddDescr_repeats 12
#define CddDescr_old_root 13
#define CddDescr_curation_status 14
#define CddDescr_readonly_status 15
#define CddDescr_book_ref 16


NLM_EXTERN CddDescrPtr LIBCALL CddDescrFree PROTO ((CddDescrPtr ));
NLM_EXTERN CddDescrPtr LIBCALL CddDescrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddDescrAsnWrite PROTO (( CddDescrPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddDescrSet
*
**************************************************/
typedef ValNode CddDescrSet;
typedef ValNodePtr CddDescrSetPtr;
#define CddDescrSetNew() ValNodeNew(NULL) 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN CddDescrSetPtr LIBCALL CddDescrSetFree PROTO ((CddDescrSetPtr ));
NLM_EXTERN CddDescrSetPtr LIBCALL CddDescrSetNew PROTO (( void ));
NLM_EXTERN CddDescrSetPtr LIBCALL CddDescrSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddDescrSetAsnWrite PROTO (( CddDescrSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    Matrix
*
**************************************************/
typedef struct struct_Matrix {
   Int4   ncolumns;
   Int4   nrows;
   ValNodePtr   row_labels;
   Int4   scale_factor;
   ValNodePtr   columns;
} Matrix, PNTR MatrixPtr;


NLM_EXTERN MatrixPtr LIBCALL MatrixFree PROTO ((MatrixPtr ));
NLM_EXTERN MatrixPtr LIBCALL MatrixNew PROTO (( void ));
NLM_EXTERN MatrixPtr LIBCALL MatrixAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MatrixAsnWrite PROTO (( MatrixPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Triangle
*
**************************************************/
typedef struct struct_Triangle {
   Int4   nelements;
   ScorePtr   scores;
   ValNodePtr   div_ranks;
} Triangle, PNTR TrianglePtr;


NLM_EXTERN TrianglePtr LIBCALL TriangleFree PROTO ((TrianglePtr ));
NLM_EXTERN TrianglePtr LIBCALL TriangleNew PROTO (( void ));
NLM_EXTERN TrianglePtr LIBCALL TriangleAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TriangleAsnWrite PROTO (( TrianglePtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr UpdateCommentPtr;
typedef ValNode UpdateComment;
#define UpdateComment_comment 1
#define UpdateComment_addthis 2
#define UpdateComment_replaces 3
#define UpdateComment_reject_loc 4
#define UpdateComment_reference 5


NLM_EXTERN UpdateCommentPtr LIBCALL UpdateCommentFree PROTO ((UpdateCommentPtr ));
NLM_EXTERN UpdateCommentPtr LIBCALL UpdateCommentAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL UpdateCommentAsnWrite PROTO (( UpdateCommentPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    UpdateAlign
*
**************************************************/
typedef struct struct_Update_align {
   struct struct_Update_align PNTR next;
   ValNodePtr   description;
   struct seqannot PNTR   seqannot;
   Int4   type;
} UpdateAlign, PNTR UpdateAlignPtr;


NLM_EXTERN UpdateAlignPtr LIBCALL UpdateAlignFree PROTO ((UpdateAlignPtr ));
NLM_EXTERN UpdateAlignPtr LIBCALL UpdateAlignNew PROTO (( void ));
NLM_EXTERN UpdateAlignPtr LIBCALL UpdateAlignAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL UpdateAlignAsnWrite PROTO (( UpdateAlignPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    RejectId
*
**************************************************/
typedef struct struct_Reject_id {
   struct struct_Reject_id PNTR next;
   ValNodePtr   description;
   ValNodePtr   ids;
} RejectId, PNTR RejectIdPtr;


NLM_EXTERN RejectIdPtr LIBCALL RejectIdFree PROTO ((RejectIdPtr ));
NLM_EXTERN RejectIdPtr LIBCALL RejectIdNew PROTO (( void ));
NLM_EXTERN RejectIdPtr LIBCALL RejectIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL RejectIdAsnWrite PROTO (( RejectIdPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr FeatureEvidencePtr;
typedef ValNode FeatureEvidence;
#define FeatureEvidence_comment 1
#define FeatureEvidence_reference 2
#define FeatureEvidence_bsannot 3
#define FeatureEvidence_seqfeat 4
#define FeatureEvidence_book_ref 5


NLM_EXTERN FeatureEvidencePtr LIBCALL FeatureEvidenceFree PROTO ((FeatureEvidencePtr ));
NLM_EXTERN FeatureEvidencePtr LIBCALL FeatureEvidenceAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL FeatureEvidenceAsnWrite PROTO (( FeatureEvidencePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AlignAnnot
*
**************************************************/
typedef struct struct_Align_annot {
   struct struct_Align_annot PNTR next;
   ValNodePtr   location;
   CharPtr   description;
   ValNodePtr   evidence;
} AlignAnnot, PNTR AlignAnnotPtr;


NLM_EXTERN AlignAnnotPtr LIBCALL AlignAnnotFree PROTO ((AlignAnnotPtr ));
NLM_EXTERN AlignAnnotPtr LIBCALL AlignAnnotNew PROTO (( void ));
NLM_EXTERN AlignAnnotPtr LIBCALL AlignAnnotAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AlignAnnotAsnWrite PROTO (( AlignAnnotPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AlignAnnotSet
*
**************************************************/
typedef struct struct_Align_annot AlignAnnotSet;
typedef struct struct_Align_annot PNTR AlignAnnotSetPtr;
#define AlignAnnotSetNew() AlignAnnotNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN AlignAnnotSetPtr LIBCALL AlignAnnotSetFree PROTO ((AlignAnnotSetPtr ));
NLM_EXTERN AlignAnnotSetPtr LIBCALL AlignAnnotSetNew PROTO (( void ));
NLM_EXTERN AlignAnnotSetPtr LIBCALL AlignAnnotSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AlignAnnotSetAsnWrite PROTO (( AlignAnnotSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    DomainParent
*
**************************************************/
typedef struct struct_Domain_parent {
   struct struct_Domain_parent PNTR next;
   Int4   parent_type;
   ValNodePtr   parentid;
   struct seqannot PNTR   seqannot;
} DomainParent, PNTR DomainParentPtr;


NLM_EXTERN DomainParentPtr LIBCALL DomainParentFree PROTO ((DomainParentPtr ));
NLM_EXTERN DomainParentPtr LIBCALL DomainParentNew PROTO (( void ));
NLM_EXTERN DomainParentPtr LIBCALL DomainParentAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL DomainParentAsnWrite PROTO (( DomainParentPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    SequenceTree
*
**************************************************/
typedef struct struct_Sequence_tree {
   CharPtr   cdAccession;
   struct struct_Algorithm_type PNTR   algorithm;
   Uint1   isAnnotated;
   struct struct_SeqTree_node PNTR   root;
} SequenceTree, PNTR SequenceTreePtr;


NLM_EXTERN SequenceTreePtr LIBCALL SequenceTreeFree PROTO ((SequenceTreePtr ));
NLM_EXTERN SequenceTreePtr LIBCALL SequenceTreeNew PROTO (( void ));
NLM_EXTERN SequenceTreePtr LIBCALL SequenceTreeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL SequenceTreeAsnWrite PROTO (( SequenceTreePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AlgorithmType
*
**************************************************/
typedef struct struct_Algorithm_type {
   Int4   scoring_Scheme;
   Int4   clustering_Method;
   Int4   score_Matrix;
   Int4   gapOpen;
   Int4   gapExtend;
   Int4   gapScaleFactor;
   Int4   nTerminalExt;
   Int4   cTerminalExt;
} AlgorithmType, PNTR AlgorithmTypePtr;


NLM_EXTERN AlgorithmTypePtr LIBCALL AlgorithmTypeFree PROTO ((AlgorithmTypePtr ));
NLM_EXTERN AlgorithmTypePtr LIBCALL AlgorithmTypeNew PROTO (( void ));
NLM_EXTERN AlgorithmTypePtr LIBCALL AlgorithmTypeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL AlgorithmTypeAsnWrite PROTO (( AlgorithmTypePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    SeqTreeNode
*
**************************************************/
typedef struct struct_SeqTree_node {
   struct struct_SeqTree_node PNTR next;
   Uint1   isAnnotated;
   CharPtr   name;
   FloatHi   distance;
   ValNodePtr   Children_children;
   struct struct_Node_annotation PNTR   annotation;
} SeqTreeNode, PNTR SeqTreeNodePtr;


NLM_EXTERN SeqTreeNodePtr LIBCALL SeqTreeNodeFree PROTO ((SeqTreeNodePtr ));
NLM_EXTERN SeqTreeNodePtr LIBCALL SeqTreeNodeNew PROTO (( void ));
NLM_EXTERN SeqTreeNodePtr LIBCALL SeqTreeNodeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL SeqTreeNodeAsnWrite PROTO (( SeqTreeNodePtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Children_childrenPtr;
typedef ValNode Children_children;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Children_children_children 1
#define Children_children_Children_Footprint 2

#ifdef NLM_GENERATED_CODE_PROTO

static Children_childrenPtr LIBCALL Children_childrenFree PROTO ((Children_childrenPtr ));
static Children_childrenPtr LIBCALL Children_childrenAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Children_childrenAsnWrite PROTO (( Children_childrenPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    Children_footprint
*
**************************************************/

#ifdef NLM_GENERATED_CODE_PROTO

typedef struct struct_Children_Footprint {
   struct seqint PNTR   seqRange;
   Int4   rowId;
} Children_footprint, PNTR Children_footprintPtr;
#endif /* NLM_GENERATED_CODE_PROTO */

#ifdef NLM_GENERATED_CODE_PROTO

static Children_footprintPtr LIBCALL Children_footprintFree PROTO ((Children_footprintPtr ));
static Children_footprintPtr LIBCALL Children_footprintNew PROTO (( void ));
static Children_footprintPtr LIBCALL Children_footprintAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Children_footprintAsnWrite PROTO (( Children_footprintPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    NodeAnnotation
*
**************************************************/
typedef struct struct_Node_annotation {
   CharPtr   presentInChildCD;
   CharPtr   note;
} NodeAnnotation, PNTR NodeAnnotationPtr;


NLM_EXTERN NodeAnnotationPtr LIBCALL NodeAnnotationFree PROTO ((NodeAnnotationPtr ));
NLM_EXTERN NodeAnnotationPtr LIBCALL NodeAnnotationNew PROTO (( void ));
NLM_EXTERN NodeAnnotationPtr LIBCALL NodeAnnotationAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL NodeAnnotationAsnWrite PROTO (( NodeAnnotationPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddViewerRect
*
**************************************************/
typedef struct struct_Cdd_Viewer_Rect {
   Int4   top;
   Int4   left;
   Int4   width;
   Int4   height;
} CddViewerRect, PNTR CddViewerRectPtr;


NLM_EXTERN CddViewerRectPtr LIBCALL CddViewerRectFree PROTO ((CddViewerRectPtr ));
NLM_EXTERN CddViewerRectPtr LIBCALL CddViewerRectNew PROTO (( void ));
NLM_EXTERN CddViewerRectPtr LIBCALL CddViewerRectAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddViewerRectAsnWrite PROTO (( CddViewerRectPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddViewer
*
**************************************************/
typedef struct struct_Cdd_Viewer {
   struct struct_Cdd_Viewer PNTR next;
   Int4   ctrl;
   struct struct_Cdd_Viewer_Rect PNTR   rect;
   ValNodePtr   accessions;
} CddViewer, PNTR CddViewerPtr;


NLM_EXTERN CddViewerPtr LIBCALL CddViewerFree PROTO ((CddViewerPtr ));
NLM_EXTERN CddViewerPtr LIBCALL CddViewerNew PROTO (( void ));
NLM_EXTERN CddViewerPtr LIBCALL CddViewerAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddViewerAsnWrite PROTO (( CddViewerPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CddScript
*
**************************************************/
typedef struct struct_Cdd_Script {
   struct struct_Cdd_Script PNTR next;
   Int4   type;
   CharPtr   name;
   CharPtr   commands;
} CddScript, PNTR CddScriptPtr;


NLM_EXTERN CddScriptPtr LIBCALL CddScriptFree PROTO ((CddScriptPtr ));
NLM_EXTERN CddScriptPtr LIBCALL CddScriptNew PROTO (( void ));
NLM_EXTERN CddScriptPtr LIBCALL CddScriptAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddScriptAsnWrite PROTO (( CddScriptPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objcdd_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

