#ifndef _objscoremat_ 
#define _objscoremat_ 

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
*    Generated objects for Module NCBI-ScoreMat
*    Generated using ASNCODE Revision: 6.14 at Aug 26, 2003  5:18 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objscorematAsnLoad PROTO((void));


/**************************************************
*
*    ScoreMatrix
*
**************************************************/
typedef struct struct_Score_matrix {
   Uint1   is_protein;
   ObjectIdPtr   identifier;
   ValNodePtr   comments;
   Int4   nrows;
   Int4   ncolumns;
   ValNodePtr   row_labels;
   ValNodePtr   scores;
   Int4   score_scale_factor;
   ValNodePtr   posFreqs;
   Int4   posFreqs_scale_factor;
   Int4   pseudocounts;
   Int4   freq_Ratios;
   Int4   gapOpen;
   Int4   gapExtend;
   ValNodePtr   query;
   ValNodePtr   rawFreqs;
   Int4   rawFreqs_scale_factor;
   ValNodePtr   weights;
   Int4   weights_scale_factor;
   Uint1   byrow;
} ScoreMatrix, PNTR ScoreMatrixPtr;


NLM_EXTERN ScoreMatrixPtr LIBCALL ScoreMatrixFree PROTO ((ScoreMatrixPtr ));
NLM_EXTERN ScoreMatrixPtr LIBCALL ScoreMatrixNew PROTO (( void ));
NLM_EXTERN ScoreMatrixPtr LIBCALL ScoreMatrixAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ScoreMatrixAsnWrite PROTO (( ScoreMatrixPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ScoreMatrixParameters
*
**************************************************/
typedef struct struct_Score_matrix_parameters {
   struct struct_Score_matrix PNTR   matrix;
   FloatHi   lambda;
   FloatHi   kappa;
   FloatHi   h;
   struct struct_CoreDef PNTR   constraints;
} ScoreMatrixParameters, PNTR ScoreMatrixParametersPtr;


NLM_EXTERN ScoreMatrixParametersPtr LIBCALL ScoreMatrixParametersFree PROTO ((ScoreMatrixParametersPtr ));
NLM_EXTERN ScoreMatrixParametersPtr LIBCALL ScoreMatrixParametersNew PROTO (( void ));
NLM_EXTERN ScoreMatrixParametersPtr LIBCALL ScoreMatrixParametersAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ScoreMatrixParametersAsnWrite PROTO (( ScoreMatrixParametersPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BlockProperty
*
**************************************************/
typedef struct struct_BlockProperty {
   struct struct_BlockProperty PNTR next;
   Int4   type;
   Int4   intvalue;
   CharPtr   textvalue;
} BlockProperty, PNTR BlockPropertyPtr;


NLM_EXTERN BlockPropertyPtr LIBCALL BlockPropertyFree PROTO ((BlockPropertyPtr ));
NLM_EXTERN BlockPropertyPtr LIBCALL BlockPropertyNew PROTO (( void ));
NLM_EXTERN BlockPropertyPtr LIBCALL BlockPropertyAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BlockPropertyAsnWrite PROTO (( BlockPropertyPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CoreBlock
*
**************************************************/
typedef struct struct_CoreBlock {
   struct struct_CoreBlock PNTR next;
   Int4   start;
   Int4   stop;
   Int4   minstart;
   Int4   maxstop;
   struct struct_BlockProperty PNTR   property;
} CoreBlock, PNTR CoreBlockPtr;


NLM_EXTERN CoreBlockPtr LIBCALL CoreBlockFree PROTO ((CoreBlockPtr ));
NLM_EXTERN CoreBlockPtr LIBCALL CoreBlockNew PROTO (( void ));
NLM_EXTERN CoreBlockPtr LIBCALL CoreBlockAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CoreBlockAsnWrite PROTO (( CoreBlockPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    LoopConstraint
*
**************************************************/
typedef struct struct_LoopConstraint {
   struct struct_LoopConstraint PNTR next;
   Int4   minlength;
   Int4   maxlength;
} LoopConstraint, PNTR LoopConstraintPtr;


NLM_EXTERN LoopConstraintPtr LIBCALL LoopConstraintFree PROTO ((LoopConstraintPtr ));
NLM_EXTERN LoopConstraintPtr LIBCALL LoopConstraintNew PROTO (( void ));
NLM_EXTERN LoopConstraintPtr LIBCALL LoopConstraintAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL LoopConstraintAsnWrite PROTO (( LoopConstraintPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    CoreDef
*
**************************************************/
typedef struct struct_CoreDef {
   Int4   nblocks;
   struct struct_CoreBlock PNTR   blocks;
   struct struct_LoopConstraint PNTR   loops;
} CoreDef, PNTR CoreDefPtr;


NLM_EXTERN CoreDefPtr LIBCALL CoreDefFree PROTO ((CoreDefPtr ));
NLM_EXTERN CoreDefPtr LIBCALL CoreDefNew PROTO (( void ));
NLM_EXTERN CoreDefPtr LIBCALL CoreDefAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL CoreDefAsnWrite PROTO (( CoreDefPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objscoremat_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

