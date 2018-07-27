#ifndef _objtseq_ 
#define _objtseq_ 

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
*    Generated objects for Module NCBI-TSeq
*    Generated using ASNCODE Revision: 6.14 at Jun 28, 2002 12:46 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objtseqAsnLoad PROTO((void));


/**************************************************
*
*    TSeq
*
**************************************************/
typedef struct struct_TSeq {
   struct struct_TSeq PNTR next;
   Uint4 OBbits__;
   Uint2   seqtype;
   /* following #defines are for enumerated type, not used by object loaders */
#define TSeq_seqtype_nucleotide 1
#define TSeq_seqtype_protein 2

#define OB__TSeq_gi 0

   Int4   gi;
   CharPtr   accver;
   CharPtr   sid;
   CharPtr   local;
#define OB__TSeq_taxid 1

   Int4   taxid;
   CharPtr   orgname;
   CharPtr   defline;
   Int4   length;
   CharPtr   sequence;
} TSeq, PNTR TSeqPtr;


NLM_EXTERN TSeqPtr LIBCALL TSeqFree PROTO ((TSeqPtr ));
NLM_EXTERN TSeqPtr LIBCALL TSeqNew PROTO (( void ));
NLM_EXTERN TSeqPtr LIBCALL TSeqAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TSeqAsnWrite PROTO (( TSeqPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    TSeqSet
*
**************************************************/
typedef struct struct_TSeq TSeqSet;
typedef struct struct_TSeq PNTR TSeqSetPtr;
#define TSeqSetNew() TSeqNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN TSeqSetPtr LIBCALL TSeqSetFree PROTO ((TSeqSetPtr ));
NLM_EXTERN TSeqSetPtr LIBCALL TSeqSetNew PROTO (( void ));
NLM_EXTERN TSeqSetPtr LIBCALL TSeqSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TSeqSetAsnWrite PROTO (( TSeqSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */

/* manually added */

#include <objseq.h>

NLM_EXTERN TSeqPtr BioseqToTSeq (BioseqPtr bsp);
NLM_EXTERN Boolean BioseqAsnWriteAsTSeq (BioseqPtr bsp, AsnIoPtr aip, AsnTypePtr orig);

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objtseq_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

