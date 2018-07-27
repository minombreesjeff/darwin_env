#ifndef _objmim_ 
#define _objmim_ 

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
*    Generated objects for Module NCBI-Mim
*    Generated using ASNCODE Revision: 6.9 at Aug 16, 2000  6:14 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objmimAsnLoad PROTO((void));


/**************************************************
*
*    MimSet
*
**************************************************/
typedef struct struct_Mim_set {
   struct struct_Mim_date PNTR   releaseDate;
   struct struct_Mim_entry PNTR   mimEntries;
} MimSet, PNTR MimSetPtr;


NLM_EXTERN MimSetPtr LIBCALL MimSetFree PROTO ((MimSetPtr ));
NLM_EXTERN MimSetPtr LIBCALL MimSetNew PROTO (( void ));
NLM_EXTERN MimSetPtr LIBCALL MimSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimSetAsnWrite PROTO (( MimSetPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MimDate
*
**************************************************/
typedef struct struct_Mim_date {
   Int4   year;
   Int4   month;
   Int4   day;
} MimDate, PNTR MimDatePtr;


NLM_EXTERN MimDatePtr LIBCALL MimDateFree PROTO ((MimDatePtr ));
NLM_EXTERN MimDatePtr LIBCALL MimDateNew PROTO (( void ));
NLM_EXTERN MimDatePtr LIBCALL MimDateAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimDateAsnWrite PROTO (( MimDatePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MimEntry
*
**************************************************/
typedef struct struct_Mim_entry {
   struct struct_Mim_entry PNTR next;
   CharPtr   mimNumber;
   Int4   mimType;
   CharPtr   title;
   CharPtr   copyright;
   CharPtr   symbol;
   CharPtr   locus;
   ValNodePtr   synonyms;
   ValNodePtr   aliases;
   ValNodePtr   included;
   struct struct_Mim_cit PNTR   seeAlso;
   struct struct_Mim_text PNTR   text;
   struct struct_Mim_text PNTR   textfields;
   Uint1   hasSummary;
   struct struct_Mim_text PNTR   summary;
   struct struct_Mim_edit_item PNTR   summaryAttribution;
   struct struct_Mim_edit_item PNTR   summaryEditHistory;
   struct struct_Mim_edit_item PNTR   summaryCreationDate;
   struct struct_Mim_allelic_variant PNTR   allelicVariants;
   Uint1   hasSynopsis;
   struct struct_Mim_index_term PNTR   clinicalSynopsis;
   struct struct_Mim_edit_item PNTR   synopsisAttribution;
   struct struct_Mim_edit_item PNTR   synopsisEditHistory;
   struct struct_Mim_edit_item PNTR   synopsisCreationDate;
   struct struct_Mim_edit_item PNTR   editHistory;
   struct struct_Mim_edit_item PNTR   creationDate;
   struct struct_Mim_reference PNTR   references;
   struct struct_Mim_edit_item PNTR   attribution;
   Int4   numGeneMaps;
   struct struct_Mim_link PNTR   medlineLinks;
   struct struct_Mim_link PNTR   proteinLinks;
   struct struct_Mim_link PNTR   nucleotideLinks;
   struct struct_Mim_link PNTR   structureLinks;
   struct struct_Mim_link PNTR   genomeLinks;
} MimEntry, PNTR MimEntryPtr;


NLM_EXTERN MimEntryPtr LIBCALL MimEntryFree PROTO ((MimEntryPtr ));
NLM_EXTERN MimEntryPtr LIBCALL MimEntryNew PROTO (( void ));
NLM_EXTERN MimEntryPtr LIBCALL MimEntryAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimEntryAsnWrite PROTO (( MimEntryPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MimCit
*
**************************************************/
typedef struct struct_Mim_cit {
   struct struct_Mim_cit PNTR next;
   Int4   number;
   CharPtr   author;
   CharPtr   others;
   Int4   year;
} MimCit, PNTR MimCitPtr;


NLM_EXTERN MimCitPtr LIBCALL MimCitFree PROTO ((MimCitPtr ));
NLM_EXTERN MimCitPtr LIBCALL MimCitNew PROTO (( void ));
NLM_EXTERN MimCitPtr LIBCALL MimCitAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimCitAsnWrite PROTO (( MimCitPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MimText
*
**************************************************/
typedef struct struct_Mim_text {
   struct struct_Mim_text PNTR next;
   CharPtr   label;
   CharPtr   text;
   struct struct_Mim_link PNTR   neighbors;
} MimText, PNTR MimTextPtr;


NLM_EXTERN MimTextPtr LIBCALL MimTextFree PROTO ((MimTextPtr ));
NLM_EXTERN MimTextPtr LIBCALL MimTextNew PROTO (( void ));
NLM_EXTERN MimTextPtr LIBCALL MimTextAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimTextAsnWrite PROTO (( MimTextPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MimEditItem
*
**************************************************/
typedef struct struct_Mim_edit_item {
   struct struct_Mim_edit_item PNTR next;
   CharPtr   author;
   struct struct_Mim_date PNTR   modDate;
} MimEditItem, PNTR MimEditItemPtr;


NLM_EXTERN MimEditItemPtr LIBCALL MimEditItemFree PROTO ((MimEditItemPtr ));
NLM_EXTERN MimEditItemPtr LIBCALL MimEditItemNew PROTO (( void ));
NLM_EXTERN MimEditItemPtr LIBCALL MimEditItemAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimEditItemAsnWrite PROTO (( MimEditItemPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MimAllelicVariant
*
**************************************************/
typedef struct struct_Mim_allelic_variant {
   struct struct_Mim_allelic_variant PNTR next;
   CharPtr   number;
   CharPtr   name;
   ValNodePtr   aliases;
   struct struct_Mim_text PNTR   mutation;
   struct struct_Mim_text PNTR   description;
} MimAllelicVariant, PNTR MimAllelicVariantPtr;


NLM_EXTERN MimAllelicVariantPtr LIBCALL MimAllelicVariantFree PROTO ((MimAllelicVariantPtr ));
NLM_EXTERN MimAllelicVariantPtr LIBCALL MimAllelicVariantNew PROTO (( void ));
NLM_EXTERN MimAllelicVariantPtr LIBCALL MimAllelicVariantAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimAllelicVariantAsnWrite PROTO (( MimAllelicVariantPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MimIndexTerm
*
**************************************************/
typedef struct struct_Mim_index_term {
   struct struct_Mim_index_term PNTR next;
   CharPtr   key;
   ValNodePtr   terms;
} MimIndexTerm, PNTR MimIndexTermPtr;


NLM_EXTERN MimIndexTermPtr LIBCALL MimIndexTermFree PROTO ((MimIndexTermPtr ));
NLM_EXTERN MimIndexTermPtr LIBCALL MimIndexTermNew PROTO (( void ));
NLM_EXTERN MimIndexTermPtr LIBCALL MimIndexTermAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimIndexTermAsnWrite PROTO (( MimIndexTermPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MimReference
*
**************************************************/
typedef struct struct_Mim_reference {
   struct struct_Mim_reference PNTR next;
   Int4   number;
   Int4   origNumber;
   Uint2   type;
   /* following #defines are for enumerated type, not used by object loaders */
#define Mim_reference_type_not_set 0
#define Mim_reference_type_citation 1
#define Mim_reference_type_book 2
#define Mim_reference_type_personal_communication 3
#define Mim_reference_type_book_citation 4

   struct struct_Mim_author PNTR   authors;
   CharPtr   primaryAuthor;
   CharPtr   otherAuthors;
   CharPtr   citationTitle;
   Int4   citationType;
   CharPtr   bookTitle;
   struct struct_Mim_author PNTR   editors;
   CharPtr   volume;
   CharPtr   edition;
   CharPtr   journal;
   CharPtr   series;
   CharPtr   publisher;
   CharPtr   place;
   CharPtr   commNote;
   struct struct_Mim_date PNTR   pubDate;
   struct struct_Mim_page PNTR   pages;
   CharPtr   miscInfo;
   Int4   pubmedUID;
   Uint1   ambiguous;
   Uint1   noLink;
} MimReference, PNTR MimReferencePtr;


NLM_EXTERN MimReferencePtr LIBCALL MimReferenceFree PROTO ((MimReferencePtr ));
NLM_EXTERN MimReferencePtr LIBCALL MimReferenceNew PROTO (( void ));
NLM_EXTERN MimReferencePtr LIBCALL MimReferenceAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimReferenceAsnWrite PROTO (( MimReferencePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MimLink
*
**************************************************/
typedef struct struct_Mim_link {
   Int4   num;
   CharPtr   uids;
   Int4   numRelevant;
} MimLink, PNTR MimLinkPtr;


NLM_EXTERN MimLinkPtr LIBCALL MimLinkFree PROTO ((MimLinkPtr ));
NLM_EXTERN MimLinkPtr LIBCALL MimLinkNew PROTO (( void ));
NLM_EXTERN MimLinkPtr LIBCALL MimLinkAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimLinkAsnWrite PROTO (( MimLinkPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MimAuthor
*
**************************************************/
typedef struct struct_Mim_author {
   struct struct_Mim_author PNTR next;
   CharPtr   name;
   Int4   index;
} MimAuthor, PNTR MimAuthorPtr;


NLM_EXTERN MimAuthorPtr LIBCALL MimAuthorFree PROTO ((MimAuthorPtr ));
NLM_EXTERN MimAuthorPtr LIBCALL MimAuthorNew PROTO (( void ));
NLM_EXTERN MimAuthorPtr LIBCALL MimAuthorAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimAuthorAsnWrite PROTO (( MimAuthorPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MimPage
*
**************************************************/
typedef struct struct_Mim_page {
   struct struct_Mim_page PNTR next;
   CharPtr   from;
   CharPtr   to;
} MimPage, PNTR MimPagePtr;


NLM_EXTERN MimPagePtr LIBCALL MimPageFree PROTO ((MimPagePtr ));
NLM_EXTERN MimPagePtr LIBCALL MimPageNew PROTO (( void ));
NLM_EXTERN MimPagePtr LIBCALL MimPageAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MimPageAsnWrite PROTO (( MimPagePtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objmim_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

