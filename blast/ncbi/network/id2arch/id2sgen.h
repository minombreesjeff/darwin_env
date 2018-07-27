#ifndef _id2sgen_ 
#define _id2sgen_ 

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
*    Generated objects for Module NCBI-Seq-split
*    Generated using ASNCODE Revision: 6.0 at May 3, 2004  6:18 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
id2sgenAsnLoad PROTO((void));


/**************************************************
*
*    ID2SSeqAnnotInfo
*
**************************************************/
typedef struct struct_ID2S_Seq_annot_Info {
   CharPtr   name;
   Uint1   align;
   Uint1   graph;
   struct struct_ID2S_Feat_type_Info PNTR   feat;
   ValNodePtr   seq_loc;
} ID2SSeqAnnotInfo, PNTR ID2SSeqAnnotInfoPtr;


NLM_EXTERN ID2SSeqAnnotInfoPtr LIBCALL ID2SSeqAnnotInfoFree PROTO ((ID2SSeqAnnotInfoPtr ));
NLM_EXTERN ID2SSeqAnnotInfoPtr LIBCALL ID2SSeqAnnotInfoNew PROTO (( void ));
NLM_EXTERN ID2SSeqAnnotInfoPtr LIBCALL ID2SSeqAnnotInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SSeqAnnotInfoAsnWrite PROTO (( ID2SSeqAnnotInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SSplitInfo
*
**************************************************/
typedef struct struct_ID2S_Split_Info {
   struct struct_ID2S_Bioseqs_Info PNTR   bioseqs_info;
   struct struct_ID2S_Chunk_Info PNTR   chunks;
} ID2SSplitInfo, PNTR ID2SSplitInfoPtr;


NLM_EXTERN ID2SSplitInfoPtr LIBCALL ID2SSplitInfoFree PROTO ((ID2SSplitInfoPtr ));
NLM_EXTERN ID2SSplitInfoPtr LIBCALL ID2SSplitInfoNew PROTO (( void ));
NLM_EXTERN ID2SSplitInfoPtr LIBCALL ID2SSplitInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SSplitInfoAsnWrite PROTO (( ID2SSplitInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SBioseqsInfo
*
**************************************************/
typedef struct struct_ID2S_Bioseqs_Info {
   struct struct_ID2S_Bioseqs_Info PNTR next;
   struct struct_ID2S_Bioseq_Info PNTR   info;
   struct struct_ID2_Id_Range PNTR   bioseqs;
} ID2SBioseqsInfo, PNTR ID2SBioseqsInfoPtr;


NLM_EXTERN ID2SBioseqsInfoPtr LIBCALL ID2SBioseqsInfoFree PROTO ((ID2SBioseqsInfoPtr ));
NLM_EXTERN ID2SBioseqsInfoPtr LIBCALL ID2SBioseqsInfoNew PROTO (( void ));
NLM_EXTERN ID2SBioseqsInfoPtr LIBCALL ID2SBioseqsInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SBioseqsInfoAsnWrite PROTO (( ID2SBioseqsInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SChunkInfo
*
**************************************************/
typedef struct struct_ID2S_Chunk_Info {
   struct struct_ID2S_Chunk_Info PNTR next;
   Int4   id;
   ValNodePtr   content;
} ID2SChunkInfo, PNTR ID2SChunkInfoPtr;


NLM_EXTERN ID2SChunkInfoPtr LIBCALL ID2SChunkInfoFree PROTO ((ID2SChunkInfoPtr ));
NLM_EXTERN ID2SChunkInfoPtr LIBCALL ID2SChunkInfoNew PROTO (( void ));
NLM_EXTERN ID2SChunkInfoPtr LIBCALL ID2SChunkInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SChunkInfoAsnWrite PROTO (( ID2SChunkInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SBioseqInfo
*
**************************************************/
typedef struct struct_ID2S_Bioseq_Info {
   Int4   gap_count;
   Uint1   seq_map_has_ref;
   struct struct_ID2S_Sequence_Split_Info PNTR   sequence_split;
} ID2SBioseqInfo, PNTR ID2SBioseqInfoPtr;


NLM_EXTERN ID2SBioseqInfoPtr LIBCALL ID2SBioseqInfoFree PROTO ((ID2SBioseqInfoPtr ));
NLM_EXTERN ID2SBioseqInfoPtr LIBCALL ID2SBioseqInfoNew PROTO (( void ));
NLM_EXTERN ID2SBioseqInfoPtr LIBCALL ID2SBioseqInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SBioseqInfoAsnWrite PROTO (( ID2SBioseqInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2IdRange
*
**************************************************/
typedef struct struct_ID2_Id_Range {
   struct struct_ID2_Id_Range PNTR next;
   Int4   start;
   Int4   count;
} ID2IdRange, PNTR ID2IdRangePtr;


NLM_EXTERN ID2IdRangePtr LIBCALL ID2IdRangeFree PROTO ((ID2IdRangePtr ));
NLM_EXTERN ID2IdRangePtr LIBCALL ID2IdRangeNew PROTO (( void ));
NLM_EXTERN ID2IdRangePtr LIBCALL ID2IdRangeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2IdRangeAsnWrite PROTO (( ID2IdRangePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SSequenceSplitInfo
*
**************************************************/
typedef struct struct_ID2S_Sequence_Split_Info {
   Int4   block_size;
   Int4   chunk_start;
   ValNodePtr   chunk_blocks;
} ID2SSequenceSplitInfo, PNTR ID2SSequenceSplitInfoPtr;


NLM_EXTERN ID2SSequenceSplitInfoPtr LIBCALL ID2SSequenceSplitInfoFree PROTO ((ID2SSequenceSplitInfoPtr ));
NLM_EXTERN ID2SSequenceSplitInfoPtr LIBCALL ID2SSequenceSplitInfoNew PROTO (( void ));
NLM_EXTERN ID2SSequenceSplitInfoPtr LIBCALL ID2SSequenceSplitInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SSequenceSplitInfoAsnWrite PROTO (( ID2SSequenceSplitInfoPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr ID2SChunkContentPtr;
typedef ValNode ID2SChunkContent;
#define ID2SChunkContent_seq_descr 1
#define ID2SChunkContent_seq_annot 2
#define ID2SChunkContent_seq_assembly 3
#define ID2SChunkContent_seq_map 4
#define ID2SChunkContent_seq_data 5
#define ID2SChunkContent_seq_annot_place 6


NLM_EXTERN ID2SChunkContentPtr LIBCALL ID2SChunkContentFree PROTO ((ID2SChunkContentPtr ));
NLM_EXTERN ID2SChunkContentPtr LIBCALL ID2SChunkContentAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SChunkContentAsnWrite PROTO (( ID2SChunkContentPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SSeqDescrInfo
*
**************************************************/
typedef struct struct_ID2S_Seq_descr_Info {
   Int4   type_mask;
   struct struct_ID2_Id_Range PNTR   bioseqs;
   struct struct_ID2_Id_Range PNTR   bioseq_sets;
} ID2SSeqDescrInfo, PNTR ID2SSeqDescrInfoPtr;


NLM_EXTERN ID2SSeqDescrInfoPtr LIBCALL ID2SSeqDescrInfoFree PROTO ((ID2SSeqDescrInfoPtr ));
NLM_EXTERN ID2SSeqDescrInfoPtr LIBCALL ID2SSeqDescrInfoNew PROTO (( void ));
NLM_EXTERN ID2SSeqDescrInfoPtr LIBCALL ID2SSeqDescrInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SSeqDescrInfoAsnWrite PROTO (( ID2SSeqDescrInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SSeqAssemblyInfo
*
**************************************************/
typedef struct struct_ID2S_Seq_assembly_Info {
   struct struct_ID2_Id_Range PNTR   bioseqs;
} ID2SSeqAssemblyInfo, PNTR ID2SSeqAssemblyInfoPtr;


NLM_EXTERN ID2SSeqAssemblyInfoPtr LIBCALL ID2SSeqAssemblyInfoFree PROTO ((ID2SSeqAssemblyInfoPtr ));
NLM_EXTERN ID2SSeqAssemblyInfoPtr LIBCALL ID2SSeqAssemblyInfoNew PROTO (( void ));
NLM_EXTERN ID2SSeqAssemblyInfoPtr LIBCALL ID2SSeqAssemblyInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SSeqAssemblyInfoAsnWrite PROTO (( ID2SSeqAssemblyInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SSeqMapInfo
*
**************************************************/
#define ID2SSeqMapInfo  ValNode
#define ID2SSeqMapInfoPtr  ValNodePtr
#define ID2SSeqMapInfoFree  ValNodeFree
#define ID2SSeqMapInfoNew  ValNodeNew
#define ID2SSeqMapInfoAsnRead  ValNodeAsnRead
#define ID2SSeqMapInfoAsnWrite  ValNodeAsnWrite


/**************************************************
*
*    ID2SSeqDataInfo
*
**************************************************/
#define ID2SSeqDataInfo  ValNode
#define ID2SSeqDataInfoPtr  ValNodePtr
#define ID2SSeqDataInfoFree  ValNodeFree
#define ID2SSeqDataInfoNew  ValNodeNew
#define ID2SSeqDataInfoAsnRead  ValNodeAsnRead
#define ID2SSeqDataInfoAsnWrite  ValNodeAsnWrite


/**************************************************
*
*    ID2SSeqAnnotPlaceInfo
*
**************************************************/
typedef struct struct_ID2S_Seq_annot_place_Info {
   CharPtr   name;
   struct struct_ID2_Id_Range PNTR   bioseqs;
   struct struct_ID2_Id_Range PNTR   bioseq_sets;
} ID2SSeqAnnotPlaceInfo, PNTR ID2SSeqAnnotPlaceInfoPtr;


NLM_EXTERN ID2SSeqAnnotPlaceInfoPtr LIBCALL ID2SSeqAnnotPlaceInfoFree PROTO ((ID2SSeqAnnotPlaceInfoPtr ));
NLM_EXTERN ID2SSeqAnnotPlaceInfoPtr LIBCALL ID2SSeqAnnotPlaceInfoNew PROTO (( void ));
NLM_EXTERN ID2SSeqAnnotPlaceInfoPtr LIBCALL ID2SSeqAnnotPlaceInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SSeqAnnotPlaceInfoAsnWrite PROTO (( ID2SSeqAnnotPlaceInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SFeatTypeInfo
*
**************************************************/
typedef struct struct_ID2S_Feat_type_Info {
   struct struct_ID2S_Feat_type_Info PNTR next;
   Int4   type;
   ValNodePtr   subtypes;
} ID2SFeatTypeInfo, PNTR ID2SFeatTypeInfoPtr;


NLM_EXTERN ID2SFeatTypeInfoPtr LIBCALL ID2SFeatTypeInfoFree PROTO ((ID2SFeatTypeInfoPtr ));
NLM_EXTERN ID2SFeatTypeInfoPtr LIBCALL ID2SFeatTypeInfoNew PROTO (( void ));
NLM_EXTERN ID2SFeatTypeInfoPtr LIBCALL ID2SFeatTypeInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SFeatTypeInfoAsnWrite PROTO (( ID2SFeatTypeInfoPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr ID2SeqLocPtr;
typedef ValNode ID2SeqLoc;
#define ID2SeqLoc_gi_whole 1
#define ID2SeqLoc_interval 2
#define ID2SeqLoc_packed_ints 3
#define ID2SeqLoc_gi_whole_range 4
#define ID2SeqLoc_loc_set 5
#define ID2SeqLoc_seq_loc 6


NLM_EXTERN ID2SeqLocPtr LIBCALL ID2SeqLocFree PROTO ((ID2SeqLocPtr ));
NLM_EXTERN ID2SeqLocPtr LIBCALL ID2SeqLocAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SeqLocAsnWrite PROTO (( ID2SeqLocPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SChunk
*
**************************************************/
typedef struct struct_ID2S_Chunk {
   struct struct_ID2S_Chunk_Data PNTR   data;
} ID2SChunk, PNTR ID2SChunkPtr;


NLM_EXTERN ID2SChunkPtr LIBCALL ID2SChunkFree PROTO ((ID2SChunkPtr ));
NLM_EXTERN ID2SChunkPtr LIBCALL ID2SChunkNew PROTO (( void ));
NLM_EXTERN ID2SChunkPtr LIBCALL ID2SChunkAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SChunkAsnWrite PROTO (( ID2SChunkPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SChunkData
*
**************************************************/
typedef struct struct_ID2S_Chunk_Data {
   struct struct_ID2S_Chunk_Data PNTR next;
   ValNodePtr   Id_id;
   ValNodePtr   descrs;
   struct struct_Seq_annot PNTR   annots;
   struct struct_Seq_align PNTR   assembly;
   struct struct_Seq_literal PNTR   seq_map;
   struct struct_Seq_literal PNTR   seq_data;
} ID2SChunkData, PNTR ID2SChunkDataPtr;


NLM_EXTERN ID2SChunkDataPtr LIBCALL ID2SChunkDataFree PROTO ((ID2SChunkDataPtr ));
NLM_EXTERN ID2SChunkDataPtr LIBCALL ID2SChunkDataNew PROTO (( void ));
NLM_EXTERN ID2SChunkDataPtr LIBCALL ID2SChunkDataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SChunkDataAsnWrite PROTO (( ID2SChunkDataPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Id_idPtr;
typedef ValNode Id_id;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Id_id_bioseq_set 1
#define Id_id_gi 2

#ifdef NLM_GENERATED_CODE_PROTO

static Id_idPtr LIBCALL Id_idFree PROTO ((Id_idPtr ));
static Id_idPtr LIBCALL Id_idAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Id_idAsnWrite PROTO (( Id_idPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ID2Interval
*
**************************************************/
typedef struct struct_ID2_Interval {
   Int4   gi;
   Int4   start;
   Int4   length;
} ID2Interval, PNTR ID2IntervalPtr;


NLM_EXTERN ID2IntervalPtr LIBCALL ID2IntervalFree PROTO ((ID2IntervalPtr ));
NLM_EXTERN ID2IntervalPtr LIBCALL ID2IntervalNew PROTO (( void ));
NLM_EXTERN ID2IntervalPtr LIBCALL ID2IntervalAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2IntervalAsnWrite PROTO (( ID2IntervalPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2PackedSeqInts
*
**************************************************/
typedef struct struct_ID2_Packed_Seq_ints {
   Int4   gi;
   struct struct_ID2_Seq_range PNTR   intervals;
} ID2PackedSeqInts, PNTR ID2PackedSeqIntsPtr;


NLM_EXTERN ID2PackedSeqIntsPtr LIBCALL ID2PackedSeqIntsFree PROTO ((ID2PackedSeqIntsPtr ));
NLM_EXTERN ID2PackedSeqIntsPtr LIBCALL ID2PackedSeqIntsNew PROTO (( void ));
NLM_EXTERN ID2PackedSeqIntsPtr LIBCALL ID2PackedSeqIntsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2PackedSeqIntsAsnWrite PROTO (( ID2PackedSeqIntsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SeqRange
*
**************************************************/
typedef struct struct_ID2_Seq_range {
   struct struct_ID2_Seq_range PNTR next;
   Int4   start;
   Int4   length;
} ID2SeqRange, PNTR ID2SeqRangePtr;


NLM_EXTERN ID2SeqRangePtr LIBCALL ID2SeqRangeFree PROTO ((ID2SeqRangePtr ));
NLM_EXTERN ID2SeqRangePtr LIBCALL ID2SeqRangeNew PROTO (( void ));
NLM_EXTERN ID2SeqRangePtr LIBCALL ID2SeqRangeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SeqRangeAsnWrite PROTO (( ID2SeqRangePtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _id2sgen_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

