#ifndef _objpubme_ 
#define _objpubme_ 

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
*    Generated objects for Module NCBI-PubMed
*    Generated using ASNCODE Revision: 5.2 at Feb 4, 1997  9:49 AM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objpubmeAsnLoad PROTO((void));


/**************************************************
*
*    PubmedEntry
*
**************************************************/
typedef struct struct_Pubmed_entry {
   Int4   pmid;
   struct struct_Medline_entry PNTR   medent;
   CharPtr   publisher;
   struct struct_Pubmed_url PNTR   urls;
   CharPtr   pubid;
} PubmedEntry, PNTR PubmedEntryPtr;


NLM_EXTERN PubmedEntryPtr LIBCALL PubmedEntryFree PROTO ((PubmedEntryPtr ));
NLM_EXTERN PubmedEntryPtr LIBCALL PubmedEntryNew PROTO (( void ));
NLM_EXTERN PubmedEntryPtr LIBCALL PubmedEntryAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PubmedEntryAsnWrite PROTO (( PubmedEntryPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    PubmedUrl
*
**************************************************/
typedef struct struct_Pubmed_url {
   struct struct_Pubmed_url PNTR next;
   CharPtr   location;
   CharPtr   url;
} PubmedUrl, PNTR PubmedUrlPtr;


NLM_EXTERN PubmedUrlPtr LIBCALL PubmedUrlFree PROTO ((PubmedUrlPtr ));
NLM_EXTERN PubmedUrlPtr LIBCALL PubmedUrlNew PROTO (( void ));
NLM_EXTERN PubmedUrlPtr LIBCALL PubmedUrlAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL PubmedUrlAsnWrite PROTO (( PubmedUrlPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objpubme_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

