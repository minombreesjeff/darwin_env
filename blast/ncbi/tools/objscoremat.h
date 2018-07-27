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
*    Generated using ASNCODE Revision: 6.15 at Oct 7, 2004  2:31 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objscorematAsnLoad PROTO((void));


/**************************************************
*
*    Pssm
*
**************************************************/
typedef struct struct_Pssm {
   Uint1   isProtein;
   ObjectIdPtr identifier;
   Int4   numRows;
   Int4   numColumns;
   ValNodePtr   rowLabels;
   Uint1   byRow;
   ValNodePtr   query;
   struct struct_PssmIntermediateData PNTR   intermediateData;
   struct struct_PssmFinalData PNTR   finalData;
} Pssm, PNTR PssmPtr;


NLM_EXTERN PssmPtr LIBCALL PssmFree PROTO ((PssmPtr ));
NLM_EXTERN PssmPtr LIBCALL PssmNew PROTO (( void ));
NLM_EXTERN PssmPtr LIBCALL PssmAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PssmAsnWrite PROTO (( PssmPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    PssmIntermediateData
*
**************************************************/
typedef struct struct_PssmIntermediateData {
   ValNodePtr   resFreqsPerPos;
   ValNodePtr   weightedResFreqsPerPos;
   ValNodePtr   freqRatios;
} PssmIntermediateData, PNTR PssmIntermediateDataPtr;


NLM_EXTERN PssmIntermediateDataPtr LIBCALL PssmIntermediateDataFree PROTO ((PssmIntermediateDataPtr ));
NLM_EXTERN PssmIntermediateDataPtr LIBCALL PssmIntermediateDataNew PROTO (( void ));
NLM_EXTERN PssmIntermediateDataPtr LIBCALL PssmIntermediateDataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PssmIntermediateDataAsnWrite PROTO (( PssmIntermediateDataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    PssmFinalData
*
**************************************************/
typedef struct struct_PssmFinalData {
   ValNodePtr   scores;
   FloatHi   lambda;
   FloatHi   kappa;
   FloatHi   h;
   Int4   scalingFactor;
} PssmFinalData, PNTR PssmFinalDataPtr;


NLM_EXTERN PssmFinalDataPtr LIBCALL PssmFinalDataFree PROTO ((PssmFinalDataPtr ));
NLM_EXTERN PssmFinalDataPtr LIBCALL PssmFinalDataNew PROTO (( void ));
NLM_EXTERN PssmFinalDataPtr LIBCALL PssmFinalDataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PssmFinalDataAsnWrite PROTO (( PssmFinalDataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    PssmParameters
*
**************************************************/
typedef struct struct_PssmParameters {
   Int4   pseudocount;
   struct struct_FormatRpsDbParameters PNTR   rpsdbparams;
   struct struct_CoreDef PNTR   constraints;
} PssmParameters, PNTR PssmParametersPtr;


NLM_EXTERN PssmParametersPtr LIBCALL PssmParametersFree PROTO ((PssmParametersPtr ));
NLM_EXTERN PssmParametersPtr LIBCALL PssmParametersNew PROTO (( void ));
NLM_EXTERN PssmParametersPtr LIBCALL PssmParametersAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PssmParametersAsnWrite PROTO (( PssmParametersPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    PssmWithParameters
*
**************************************************/
typedef struct struct_PssmWithParameters {
   struct struct_Pssm PNTR   pssm;
   struct struct_PssmParameters PNTR   params;
} PssmWithParameters, PNTR PssmWithParametersPtr;


NLM_EXTERN PssmWithParametersPtr LIBCALL PssmWithParametersFree PROTO ((PssmWithParametersPtr ));
NLM_EXTERN PssmWithParametersPtr LIBCALL PssmWithParametersNew PROTO (( void ));
NLM_EXTERN PssmWithParametersPtr LIBCALL PssmWithParametersAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PssmWithParametersAsnWrite PROTO (( PssmWithParametersPtr , AsnIoPtr, AsnTypePtr));



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



/**************************************************
*
*    FormatRpsDbParameters
*
**************************************************/
typedef struct struct_FormatRpsDbParameters {
   CharPtr   matrixName;
   Int4   gapOpen;
   Int4   gapExtend;
} FormatRpsDbParameters, PNTR FormatRpsDbParametersPtr;


NLM_EXTERN FormatRpsDbParametersPtr LIBCALL FormatRpsDbParametersFree PROTO ((FormatRpsDbParametersPtr ));
NLM_EXTERN FormatRpsDbParametersPtr LIBCALL FormatRpsDbParametersNew PROTO (( void ));
NLM_EXTERN FormatRpsDbParametersPtr LIBCALL FormatRpsDbParametersAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL FormatRpsDbParametersAsnWrite PROTO (( FormatRpsDbParametersPtr , AsnIoPtr, AsnTypePtr));

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

