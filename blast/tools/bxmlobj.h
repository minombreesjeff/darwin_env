#ifndef _bxmlobj_ 
#define _bxmlobj_ 

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
*    Generated objects for Module NCBI-BlastOutput
*    Generated using ASNCODE Revision: 6.10 at May 3, 2001  1:46 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
bxmlobjAsnLoad PROTO((void));


/**************************************************
*
*    BlastOutput
*
**************************************************/
typedef struct struct_BlastOutput {
   CharPtr   program;
   CharPtr   version;
   CharPtr   reference;
   CharPtr   db;
   CharPtr   query_ID;
   CharPtr   query_def;
   Int4   query_len;
   CharPtr   query_seq;
   struct struct_Parameters PNTR   param;
   struct struct_Iteration PNTR   iterations;
} BlastOutput, PNTR BlastOutputPtr;


NLM_EXTERN BlastOutputPtr LIBCALL BlastOutputFree PROTO ((BlastOutputPtr ));
NLM_EXTERN BlastOutputPtr LIBCALL BlastOutputNew PROTO (( void ));
NLM_EXTERN BlastOutputPtr LIBCALL BlastOutputAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BlastOutputAsnWrite PROTO (( BlastOutputPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Parameters
*
**************************************************/
typedef struct struct_Parameters {
   CharPtr   matrix;
   FloatHi   expect;
   FloatHi   include;
   Int4   sc_match;
   Int4   sc_mismatch;
   Int4   gap_open;
   Int4   gap_extend;
   CharPtr   filter;
   CharPtr   pattern;
   CharPtr   entrez_query;
} Parameters, PNTR ParametersPtr;


NLM_EXTERN ParametersPtr LIBCALL ParametersFree PROTO ((ParametersPtr ));
NLM_EXTERN ParametersPtr LIBCALL ParametersNew PROTO (( void ));
NLM_EXTERN ParametersPtr LIBCALL ParametersAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ParametersAsnWrite PROTO (( ParametersPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Iteration
*
**************************************************/
typedef struct struct_Iteration {
   struct struct_Iteration PNTR next;
   Int4   iter_num;
   struct struct_Hit PNTR   hits;
   struct struct_Statistics PNTR   stat;
   CharPtr   message;
} Iteration, PNTR IterationPtr;


NLM_EXTERN IterationPtr LIBCALL IterationFree PROTO ((IterationPtr ));
NLM_EXTERN IterationPtr LIBCALL IterationNew PROTO (( void ));
NLM_EXTERN IterationPtr LIBCALL IterationAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL IterationAsnWrite PROTO (( IterationPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Hit
*
**************************************************/
typedef struct struct_Hit {
   struct struct_Hit PNTR next;
   Int4   num;
   CharPtr   id;
   CharPtr   def;
   CharPtr   accession;
   Int4   len;
   struct struct_Hsp PNTR   hsps;
} Hit, PNTR HitPtr;


NLM_EXTERN HitPtr LIBCALL HitFree PROTO ((HitPtr ));
NLM_EXTERN HitPtr LIBCALL HitNew PROTO (( void ));
NLM_EXTERN HitPtr LIBCALL HitAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL HitAsnWrite PROTO (( HitPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Statistics
*
**************************************************/
typedef struct struct_Statistics {
   Int4   db_num;
   Int4   db_len;
   Int4   hsp_len;
   FloatHi   eff_space;
   FloatHi   kappa;
   FloatHi   lambda;
   FloatHi   entropy;
} Statistics, PNTR StatisticsPtr;


NLM_EXTERN StatisticsPtr LIBCALL StatisticsFree PROTO ((StatisticsPtr ));
NLM_EXTERN StatisticsPtr LIBCALL StatisticsNew PROTO (( void ));
NLM_EXTERN StatisticsPtr LIBCALL StatisticsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL StatisticsAsnWrite PROTO (( StatisticsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Hsp
*
**************************************************/
typedef struct struct_Hsp {
   struct struct_Hsp PNTR next;
   Int4   num;
   FloatHi   bit_score;
   FloatHi   score;
   FloatHi   evalue;
   Int4   query_from;
   Int4   query_to;
   Int4   hit_from;
   Int4   hit_to;
   Int4   pattern_from;
   Int4   pattern_to;
   Int4   query_frame;
   Int4   hit_frame;
   Int4   identity;
   Int4   positive;
   Int4   gaps;
   Int4   align_len;
   Int4   density;
   CharPtr   qseq;
   CharPtr   hseq;
   CharPtr   midline;
} Hsp, PNTR HspPtr;


NLM_EXTERN HspPtr LIBCALL HspFree PROTO ((HspPtr ));
NLM_EXTERN HspPtr LIBCALL HspNew PROTO (( void ));
NLM_EXTERN HspPtr LIBCALL HspAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL HspAsnWrite PROTO (( HspPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _bxmlobj_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

