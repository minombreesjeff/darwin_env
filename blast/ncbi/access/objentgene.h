#ifndef _objentgene_ 
#define _objentgene_ 

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
*    Generated objects for Module NCBI-Entrezgene
*    Generated using ASNCODE Revision: 6.16 at Apr 19, 2005 10:58 AM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objentgeneAsnLoad PROTO((void));


/**************************************************
*
*    Entrezgene
*
**************************************************/
typedef struct struct_Entrezgene {
   struct struct_Entrezgene PNTR next;
   struct struct_Gene_track PNTR   track_info;
   Int4   type;
   struct struct_BioSource PNTR   source;
   struct struct_Gene_ref PNTR   gene;
   struct struct_Prot_ref PNTR   prot;
   struct struct_RNA_ref PNTR   rna;
   CharPtr   summary;
   struct struct_Maps PNTR   location;
   struct struct_Gene_source PNTR   gene_source;
   struct struct_Gene_commentary PNTR   locus;
   struct struct_Gene_commentary PNTR   properties;
   struct struct_Gene_commentary PNTR   refgene;
   struct struct_Gene_commentary PNTR   homology;
   struct struct_Gene_commentary PNTR   comments;
   struct struct_Dbtag PNTR   unique_keys;
   ValNodePtr   xtra_index_terms;
   struct struct_Xtra_Terms PNTR   xtra_properties;
   struct struct_Xtra_Terms PNTR   xtra_iq;
   struct struct_Dbtag PNTR   non_unique_keys;
} Entrezgene, PNTR EntrezgenePtr;


NLM_EXTERN EntrezgenePtr LIBCALL EntrezgeneFree PROTO ((EntrezgenePtr ));
NLM_EXTERN EntrezgenePtr LIBCALL EntrezgeneNew PROTO (( void ));
NLM_EXTERN EntrezgenePtr LIBCALL EntrezgeneAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL EntrezgeneAsnWrite PROTO (( EntrezgenePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    EntrezgeneSet
*
**************************************************/
typedef struct struct_Entrezgene EntrezgeneSet;
typedef struct struct_Entrezgene PNTR EntrezgeneSetPtr;
#define EntrezgeneSetNew() EntrezgeneNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN EntrezgeneSetPtr LIBCALL EntrezgeneSetFree PROTO ((EntrezgeneSetPtr ));
NLM_EXTERN EntrezgeneSetPtr LIBCALL EntrezgeneSetNew PROTO (( void ));
NLM_EXTERN EntrezgeneSetPtr LIBCALL EntrezgeneSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL EntrezgeneSetAsnWrite PROTO (( EntrezgeneSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    GeneTrack
*
**************************************************/
typedef struct struct_Gene_track {
   Int4   geneid;
   Int4   status;
   struct struct_Dbtag PNTR   current_id;
   struct struct_Date PNTR   create_date;
   struct struct_Date PNTR   update_date;
   struct struct_Date PNTR   discontinue_date;
} GeneTrack, PNTR GeneTrackPtr;


NLM_EXTERN GeneTrackPtr LIBCALL GeneTrackFree PROTO ((GeneTrackPtr ));
NLM_EXTERN GeneTrackPtr LIBCALL GeneTrackNew PROTO (( void ));
NLM_EXTERN GeneTrackPtr LIBCALL GeneTrackAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GeneTrackAsnWrite PROTO (( GeneTrackPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Maps
*
**************************************************/
typedef struct struct_Maps {
   struct struct_Maps PNTR next;
   CharPtr   display_str;
   struct struct_method PNTR   Method_method;
} Maps, PNTR MapsPtr;


NLM_EXTERN MapsPtr LIBCALL MapsFree PROTO ((MapsPtr ));
NLM_EXTERN MapsPtr LIBCALL MapsNew PROTO (( void ));
NLM_EXTERN MapsPtr LIBCALL MapsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MapsAsnWrite PROTO (( MapsPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Method_methodPtr;
typedef ValNode Method_method;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Method_method_proxy 1
#define Method_method_map_type 2
/* following #defines are for enumerated type, not used by object loaders */
#define method_map_type_cyto 0
#define method_map_type_bp 1
#define method_map_type_cM 2
#define method_map_type_cR 3
#define method_map_type_min 4


#ifdef NLM_GENERATED_CODE_PROTO

static Method_methodPtr LIBCALL Method_methodFree PROTO ((Method_methodPtr ));
static Method_methodPtr LIBCALL Method_methodAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Method_methodAsnWrite PROTO (( Method_methodPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    GeneSource
*
**************************************************/
typedef struct struct_Gene_source {
   CharPtr   src;
   Int4   src_int;
   CharPtr   src_str1;
   CharPtr   src_str2;
   Uint1   gene_display;
   Uint1   locus_display;
   Uint1   extra_terms;
} GeneSource, PNTR GeneSourcePtr;


NLM_EXTERN GeneSourcePtr LIBCALL GeneSourceFree PROTO ((GeneSourcePtr ));
NLM_EXTERN GeneSourcePtr LIBCALL GeneSourceNew PROTO (( void ));
NLM_EXTERN GeneSourcePtr LIBCALL GeneSourceAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GeneSourceAsnWrite PROTO (( GeneSourcePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    GeneCommentary
*
**************************************************/
typedef struct struct_Gene_commentary {
   struct struct_Gene_commentary PNTR next;
   Int4   type;
   CharPtr   heading;
   CharPtr   label;
   CharPtr   text;
   CharPtr   accession;
   Int4   version;
   struct struct_Xtra_Terms PNTR   xtra_properties;
   ValNodePtr   refs;
   struct struct_Other_source PNTR   source;
   ValNodePtr   genomic_coords;
   ValNodePtr   seqs;
   struct struct_Gene_commentary PNTR   products;
   struct struct_Gene_commentary PNTR   properties;
   struct struct_Gene_commentary PNTR   comment;
   struct struct_Date PNTR   create_date;
   struct struct_Date PNTR   update_date;
} GeneCommentary, PNTR GeneCommentaryPtr;


NLM_EXTERN GeneCommentaryPtr LIBCALL GeneCommentaryFree PROTO ((GeneCommentaryPtr ));
NLM_EXTERN GeneCommentaryPtr LIBCALL GeneCommentaryNew PROTO (( void ));
NLM_EXTERN GeneCommentaryPtr LIBCALL GeneCommentaryAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL GeneCommentaryAsnWrite PROTO (( GeneCommentaryPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    XtraTerms
*
**************************************************/
typedef struct struct_Xtra_Terms {
   struct struct_Xtra_Terms PNTR next;
   CharPtr   tag;
   CharPtr   value;
} XtraTerms, PNTR XtraTermsPtr;


NLM_EXTERN XtraTermsPtr LIBCALL XtraTermsFree PROTO ((XtraTermsPtr ));
NLM_EXTERN XtraTermsPtr LIBCALL XtraTermsNew PROTO (( void ));
NLM_EXTERN XtraTermsPtr LIBCALL XtraTermsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL XtraTermsAsnWrite PROTO (( XtraTermsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    OtherSource
*
**************************************************/
typedef struct struct_Other_source {
   struct struct_Other_source PNTR next;
   struct struct_Dbtag PNTR   src;
   CharPtr   pre_text;
   CharPtr   anchor;
   CharPtr   url;
   CharPtr   post_text;
} OtherSource, PNTR OtherSourcePtr;


NLM_EXTERN OtherSourcePtr LIBCALL OtherSourceFree PROTO ((OtherSourcePtr ));
NLM_EXTERN OtherSourcePtr LIBCALL OtherSourceNew PROTO (( void ));
NLM_EXTERN OtherSourcePtr LIBCALL OtherSourceAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL OtherSourceAsnWrite PROTO (( OtherSourcePtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objentgene_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

