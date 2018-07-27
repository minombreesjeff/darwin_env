#ifndef _objegkludge_ 
#define _objegkludge_ 

#include <objgen.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" { /* } */
#endif

/* allows asncode-generated Entrezgene object loaders to use hand-coded Dbtag reader/writer */

typedef struct struct_EGDbtag {
   struct struct_EGDbtag PNTR next;
   CharPtr      db;
   ObjectIdPtr  tag;
} EGDbtag, PNTR EGDbtagPtr;


NLM_EXTERN EGDbtagPtr LIBCALL EGDbtagFree PROTO ((EGDbtagPtr ));
NLM_EXTERN EGDbtagPtr LIBCALL EGDbtagNew PROTO (( void ));
NLM_EXTERN EGDbtagPtr LIBCALL EGDbtagAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL EGDbtagAsnWrite PROTO (( EGDbtagPtr , AsnIoPtr, AsnTypePtr));



#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objegkludge_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

