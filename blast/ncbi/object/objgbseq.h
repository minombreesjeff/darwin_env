#ifndef _objgbseq_ 
#define _objgbseq_ 

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
*    Generated objects for Module NCBI-GBSeq
*    Generated using ASNCODE Revision: 6.14 at Apr 9, 2004 11:58 AM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objgbseqAsnLoad PROTO((void));


/**************************************************
*
*    GBSeq
*
**************************************************/
typedef struct struct_GBSeq {
   struct struct_GBSeq PNTR next;
   Uint4 OBbits__;
   CharPtr   locus;
   Int4   length;
   Int4   strandedness;
   Int4   moltype;
   Int4   topology;
   CharPtr   division;
   CharPtr   update_date;
   CharPtr   create_date;
   CharPtr   update_release;
   CharPtr   create_release;
   CharPtr   definition;
   CharPtr   primary_accession;
   CharPtr   entry_version;
   CharPtr   accession_version;
   ValNodePtr   other_seqids;
   ValNodePtr   secondary_accessions;
   ValNodePtr   keywords;
   CharPtr   segment;
   CharPtr   source;
   CharPtr   organism;
   CharPtr   taxonomy;
   struct struct_GBReference PNTR   references;
   CharPtr   comment;
   CharPtr   primary;
   CharPtr   source_db;
   CharPtr   database_reference;
   struct struct_GBFeature PNTR   feature_table;
   CharPtr   sequence;
   CharPtr   contig;
} GBSeq, PNTR GBSeqPtr;


NLM_EXTERN GBSeqPtr LIBCALL GBSeqFree PROTO ((GBSeqPtr ));
NLM_EXTERN GBSeqPtr LIBCALL GBSeqNew PROTO (( void ));
NLM_EXTERN GBSeqPtr LIBCALL GBSeqAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GBSeqAsnWrite PROTO (( GBSeqPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    GBReference
*
**************************************************/
typedef struct struct_GBReference {
   struct struct_GBReference PNTR next;
   Uint4 OBbits__;
   CharPtr   reference;
   ValNodePtr   authors;
   CharPtr   consortium;
   CharPtr   title;
   CharPtr   journal;
#define OB__GBReference_medline 0

   Int4   medline;
#define OB__GBReference_pubmed 1

   Int4   pubmed;
   CharPtr   remark;
} GBReference, PNTR GBReferencePtr;


NLM_EXTERN GBReferencePtr LIBCALL GBReferenceFree PROTO ((GBReferencePtr ));
NLM_EXTERN GBReferencePtr LIBCALL GBReferenceNew PROTO (( void ));
NLM_EXTERN GBReferencePtr LIBCALL GBReferenceAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GBReferenceAsnWrite PROTO (( GBReferencePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    GBFeature
*
**************************************************/
typedef struct struct_GBFeature {
   struct struct_GBFeature PNTR next;
   Uint4 OBbits__;
   CharPtr   key;
   CharPtr   location;
   struct struct_GBInterval PNTR   intervals;
   struct struct_GBQualifier PNTR   quals;
} GBFeature, PNTR GBFeaturePtr;


NLM_EXTERN GBFeaturePtr LIBCALL GBFeatureFree PROTO ((GBFeaturePtr ));
NLM_EXTERN GBFeaturePtr LIBCALL GBFeatureNew PROTO (( void ));
NLM_EXTERN GBFeaturePtr LIBCALL GBFeatureAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GBFeatureAsnWrite PROTO (( GBFeaturePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    GBInterval
*
**************************************************/
typedef struct struct_GBInterval {
   struct struct_GBInterval PNTR next;
   Uint4 OBbits__;
#define OB__GBInterval_from 0

   Int4   from;
#define OB__GBInterval_to 1

   Int4   to;
#define OB__GBInterval_point 2

   Int4   point;
   CharPtr   accession;
} GBInterval, PNTR GBIntervalPtr;


NLM_EXTERN GBIntervalPtr LIBCALL GBIntervalFree PROTO ((GBIntervalPtr ));
NLM_EXTERN GBIntervalPtr LIBCALL GBIntervalNew PROTO (( void ));
NLM_EXTERN GBIntervalPtr LIBCALL GBIntervalAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GBIntervalAsnWrite PROTO (( GBIntervalPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    GBQualifier
*
**************************************************/
typedef struct struct_GBQualifier {
   struct struct_GBQualifier PNTR next;
   Uint4 OBbits__;
   CharPtr   name;
   CharPtr   value;
} GBQualifier, PNTR GBQualifierPtr;


NLM_EXTERN GBQualifierPtr LIBCALL GBQualifierFree PROTO ((GBQualifierPtr ));
NLM_EXTERN GBQualifierPtr LIBCALL GBQualifierNew PROTO (( void ));
NLM_EXTERN GBQualifierPtr LIBCALL GBQualifierAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GBQualifierAsnWrite PROTO (( GBQualifierPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    GBSet
*
**************************************************/
typedef struct struct_GBSeq GBSet;
typedef struct struct_GBSeq PNTR GBSetPtr;
#define GBSetNew() GBSeqNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN GBSetPtr LIBCALL GBSetFree PROTO ((GBSetPtr ));
NLM_EXTERN GBSetPtr LIBCALL GBSetNew PROTO (( void ));
NLM_EXTERN GBSetPtr LIBCALL GBSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GBSetAsnWrite PROTO (( GBSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objgbseq_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

