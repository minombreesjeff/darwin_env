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
*    Generated using ASNCODE Revision: 6.12 at Oct 17, 2001  2:17 PM
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
   ValNodePtr   parents;
   ValNodePtr   children;
   ValNodePtr   siblings;
   ValNodePtr   neighbors;
   struct struct_Update_align PNTR   pending;
   struct struct_Reject_id PNTR   rejects;
   ValNodePtr   master3d;
   struct struct_Align_annot PNTR   alignannot;
   struct struct_Cn3d_style_dictionary PNTR   style_dictionary;
   struct struct_Cn3d_user_annotations PNTR   user_annotations;
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
   ValNodePtr   parents;
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
typedef struct struct_CddTree CddTreeSet;
typedef struct struct_CddTree PNTR CddTreeSetPtr;
#define CddTreeSetNew() CddTreeNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN CddTreeSetPtr LIBCALL CddTreeSetFree PROTO ((CddTreeSetPtr ));
NLM_EXTERN CddTreeSetPtr LIBCALL CddTreeSetNew PROTO (( void ));
NLM_EXTERN CddTreeSetPtr LIBCALL CddTreeSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CddTreeSetAsnWrite PROTO (( CddTreeSetPtr , AsnIoPtr, AsnTypePtr));

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

