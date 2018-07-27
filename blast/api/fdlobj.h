#ifndef _fdlobj_ 
#define _fdlobj_ 

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
*    Generated objects for Module NCBI-BlastDL
*    Generated using ASNCODE Revision: 6.12 at Nov 2, 2001  5:01 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
fdlobjAsnLoad PROTO((void));


/**************************************************
*
*    BlastDefLineSet
*
**************************************************/
typedef struct struct_Blast_def_line BlastDefLineSet;
typedef struct struct_Blast_def_line PNTR BlastDefLineSetPtr;
#define BlastDefLineSetNew() Blast_def_lineNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN BlastDefLineSetPtr LIBCALL BlastDefLineSetFree PROTO ((BlastDefLineSetPtr ));
NLM_EXTERN BlastDefLineSetPtr LIBCALL BlastDefLineSetNew PROTO (( void ));
NLM_EXTERN BlastDefLineSetPtr LIBCALL BlastDefLineSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BlastDefLineSetAsnWrite PROTO (( BlastDefLineSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    BlastDefLine
*
**************************************************/
typedef struct struct_Blast_def_line {
   struct struct_Blast_def_line PNTR next;
   CharPtr   title;
   ValNodePtr   seqid;
   Int4   taxid;
   ValNodePtr   memberships;
   ValNodePtr   links;
   ValNodePtr   other_info;
} BlastDefLine, PNTR BlastDefLinePtr;


NLM_EXTERN BlastDefLinePtr LIBCALL BlastDefLineFree PROTO ((BlastDefLinePtr ));
NLM_EXTERN BlastDefLinePtr LIBCALL BlastDefLineNew PROTO (( void ));
NLM_EXTERN BlastDefLinePtr LIBCALL BlastDefLineAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BlastDefLineAsnWrite PROTO (( BlastDefLinePtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _fdlobj_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

