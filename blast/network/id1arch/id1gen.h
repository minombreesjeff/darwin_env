#ifndef _id1gen_ 
#define _id1gen_ 

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
*    Generated objects for Module NCBI-ID1Access
*    Generated using ASNCODE Revision: 6.0 at Jul 30, 1998  4:04 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
id1genAsnLoad PROTO((void));
typedef ValNodePtr ID1serverRequestPtr;
typedef ValNode ID1serverRequest;
#define ID1serverRequest_init 1
#define ID1serverRequest_getgi 2
#define ID1serverRequest_getsefromgi 3
#define ID1serverRequest_fini 4
#define ID1serverRequest_getseqidsfromgi 5
#define ID1serverRequest_getgihist 6
#define ID1serverRequest_getgirev 7
#define ID1serverRequest_getgistate 8


NLM_EXTERN ID1serverRequestPtr LIBCALL ID1serverRequestFree PROTO ((ID1serverRequestPtr ));
NLM_EXTERN ID1serverRequestPtr LIBCALL ID1serverRequestAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID1serverRequestAsnWrite PROTO (( ID1serverRequestPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID1serverMaxcomplex
*
**************************************************/
typedef struct struct_ID1server_maxcomplex {
   Int4   maxplex;
   Int4   gi;
   Int4   ent;
   CharPtr   sat;
} ID1serverMaxcomplex, PNTR ID1serverMaxcomplexPtr;


NLM_EXTERN ID1serverMaxcomplexPtr LIBCALL ID1serverMaxcomplexFree PROTO ((ID1serverMaxcomplexPtr ));
NLM_EXTERN ID1serverMaxcomplexPtr LIBCALL ID1serverMaxcomplexNew PROTO (( void ));
NLM_EXTERN ID1serverMaxcomplexPtr LIBCALL ID1serverMaxcomplexAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID1serverMaxcomplexAsnWrite PROTO (( ID1serverMaxcomplexPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID1SeqHist
*
**************************************************/
typedef struct struct_ID1Seq_hist {
   struct struct_ID1Seq_hist PNTR next;
   struct struct_Seq_hist PNTR   hist;
} ID1SeqHist, PNTR ID1SeqHistPtr;


NLM_EXTERN ID1SeqHistPtr LIBCALL ID1SeqHistFree PROTO ((ID1SeqHistPtr ));
NLM_EXTERN ID1SeqHistPtr LIBCALL ID1SeqHistNew PROTO (( void ));
NLM_EXTERN ID1SeqHistPtr LIBCALL ID1SeqHistAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID1SeqHistAsnWrite PROTO (( ID1SeqHistPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr ID1serverBackPtr;
typedef ValNode ID1serverBack;
#define ID1serverBack_init 1
#define ID1serverBack_error 2
#define ID1serverBack_gotgi 3
#define ID1serverBack_gotseqentry 4
#define ID1serverBack_gotdeadseqentry 5
#define ID1serverBack_fini 6
#define ID1serverBack_gistate 7
#define ID1serverBack_ids 8
#define ID1serverBack_gihist 9
#define ID1serverBack_girevhist 10


NLM_EXTERN ID1serverBackPtr LIBCALL ID1serverBackFree PROTO ((ID1serverBackPtr ));
NLM_EXTERN ID1serverBackPtr LIBCALL ID1serverBackAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID1serverBackAsnWrite PROTO (( ID1serverBackPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID1serverDebug
*
**************************************************/
typedef ValNode ID1serverDebug;
typedef ValNodePtr ID1serverDebugPtr;
#define ID1serverDebugNew() ValNodeNew(NULL) 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN ID1serverDebugPtr LIBCALL ID1serverDebugFree PROTO ((ID1serverDebugPtr ));
NLM_EXTERN ID1serverDebugPtr LIBCALL ID1serverDebugNew PROTO (( void ));
NLM_EXTERN ID1serverDebugPtr LIBCALL ID1serverDebugAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID1serverDebugAsnWrite PROTO (( ID1serverDebugPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _id1gen_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

