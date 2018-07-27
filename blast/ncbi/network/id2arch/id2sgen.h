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
   ValNodePtr   bioseqs;
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
} ID2SBioseqInfo, PNTR ID2SBioseqInfoPtr;


NLM_EXTERN ID2SBioseqInfoPtr LIBCALL ID2SBioseqInfoFree PROTO ((ID2SBioseqInfoPtr ));
NLM_EXTERN ID2SBioseqInfoPtr LIBCALL ID2SBioseqInfoNew PROTO (( void ));
NLM_EXTERN ID2SBioseqInfoPtr LIBCALL ID2SBioseqInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SBioseqInfoAsnWrite PROTO (( ID2SBioseqInfoPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr ID2SBioseqIdsPtr;
typedef ValNode ID2SBioseqIds;

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN ID2SBioseqIdsPtr LIBCALL ID2SBioseqIdsFree PROTO ((ID2SBioseqIdsPtr ));
NLM_EXTERN ID2SBioseqIdsPtr LIBCALL ID2SBioseqIdsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SBioseqIdsAsnWrite PROTO (( ID2SBioseqIdsPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr ID2SBioseqIds_elementPtr;
typedef ValNode ID2SBioseqIds_element;

#endif /* NLM_GENERATED_CODE_PROTO */

#define ID2SBioseqIds_gi 1
#define ID2SBioseqIds_seq_id 2
#define ID2SBioseqIds_gi_range 3

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN ID2SBioseqIds_elementPtr LIBCALL ID2SBioseqIds_elementFree PROTO ((ID2SBioseqIds_elementPtr ));
NLM_EXTERN ID2SBioseqIds_elementPtr LIBCALL ID2SBioseqIds_elementAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SBioseqIds_elementAsnWrite PROTO (( ID2SBioseqIds_elementPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */

typedef ValNodePtr ID2SChunkContentPtr;
typedef ValNode ID2SChunkContent;
#define ID2SChunkContent_seq_descr 1
#define ID2SChunkContent_seq_annot 2
#define ID2SChunkContent_seq_assembly 3
#define ID2SChunkContent_seq_map 4
#define ID2SChunkContent_seq_data 5
#define ID2SChunkContent_seq_annot_place 6
#define ID2SChunkContent_bioseq_place 7


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
   ValNodePtr   bioseqs;
   ValNodePtr   bioseq_sets;
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
   ValNodePtr   bioseqs;
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
   ValNodePtr   bioseqs;
   ValNodePtr   bioseq_sets;
} ID2SSeqAnnotPlaceInfo, PNTR ID2SSeqAnnotPlaceInfoPtr;


NLM_EXTERN ID2SSeqAnnotPlaceInfoPtr LIBCALL ID2SSeqAnnotPlaceInfoFree PROTO ((ID2SSeqAnnotPlaceInfoPtr ));
NLM_EXTERN ID2SSeqAnnotPlaceInfoPtr LIBCALL ID2SSeqAnnotPlaceInfoNew PROTO (( void ));
NLM_EXTERN ID2SSeqAnnotPlaceInfoPtr LIBCALL ID2SSeqAnnotPlaceInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SSeqAnnotPlaceInfoAsnWrite PROTO (( ID2SSeqAnnotPlaceInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SBioseqPlaceInfo
*
**************************************************/
typedef struct struct_ID2S_Bioseq_place_Info {
   struct struct_ID2S_Bioseq_place_Info PNTR next;
   Int4   bioseq_set;
   ValNodePtr   seq_ids;
} ID2SBioseqPlaceInfo, PNTR ID2SBioseqPlaceInfoPtr;


NLM_EXTERN ID2SBioseqPlaceInfoPtr LIBCALL ID2SBioseqPlaceInfoFree PROTO ((ID2SBioseqPlaceInfoPtr ));
NLM_EXTERN ID2SBioseqPlaceInfoPtr LIBCALL ID2SBioseqPlaceInfoNew PROTO (( void ));
NLM_EXTERN ID2SBioseqPlaceInfoPtr LIBCALL ID2SBioseqPlaceInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SBioseqPlaceInfoAsnWrite PROTO (( ID2SBioseqPlaceInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SBioseqSetIds
*
**************************************************/
typedef ValNode ID2SBioseqSetIds;
typedef ValNodePtr ID2SBioseqSetIdsPtr;
#define ID2SBioseqSetIdsNew() ValNodeNew(NULL) 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN ID2SBioseqSetIdsPtr LIBCALL ID2SBioseqSetIdsFree PROTO ((ID2SBioseqSetIdsPtr ));
NLM_EXTERN ID2SBioseqSetIdsPtr LIBCALL ID2SBioseqSetIdsNew PROTO (( void ));
NLM_EXTERN ID2SBioseqSetIdsPtr LIBCALL ID2SBioseqSetIdsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SBioseqSetIdsAsnWrite PROTO (( ID2SBioseqSetIdsPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



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

typedef ValNodePtr ID2SSeqLocPtr;
typedef ValNode ID2SSeqLoc;
#define ID2SSeqLoc_whole_gi 1
#define ID2SSeqLoc_whole_seq_id 2
#define ID2SSeqLoc_whole_gi_range 3
#define ID2SSeqLoc_gi_interval 4
#define ID2SSeqLoc_seq_id_interval 5
#define ID2SSeqLoc_gi_ints 6
#define ID2SSeqLoc_seq_id_ints 7
#define ID2SSeqLoc_loc_set 8


NLM_EXTERN ID2SSeqLocPtr LIBCALL ID2SSeqLocFree PROTO ((ID2SSeqLocPtr ));
NLM_EXTERN ID2SSeqLocPtr LIBCALL ID2SSeqLocAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SSeqLocAsnWrite PROTO (( ID2SSeqLocPtr , AsnIoPtr, AsnTypePtr));



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
   ValNodePtr   descr;
   struct struct_Seq_annot PNTR   annots;
   struct struct_Seq_align PNTR   assembly;
   struct struct_ID2S_Sequence_Piece PNTR   seq_map;
   struct struct_ID2S_Sequence_Piece PNTR   seq_data;
   struct struct_Bioseq PNTR   bioseqs;
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
#define Id_id_seq_id 3

#ifdef NLM_GENERATED_CODE_PROTO

static Id_idPtr LIBCALL Id_idFree PROTO ((Id_idPtr ));
static Id_idPtr LIBCALL Id_idAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Id_idAsnWrite PROTO (( Id_idPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ID2SSequencePiece
*
**************************************************/
typedef struct struct_ID2S_Sequence_Piece {
   struct struct_ID2S_Sequence_Piece PNTR next;
   Int4   start;
   struct struct_Seq_literal PNTR   data;
} ID2SSequencePiece, PNTR ID2SSequencePiecePtr;


NLM_EXTERN ID2SSequencePiecePtr LIBCALL ID2SSequencePieceFree PROTO ((ID2SSequencePiecePtr ));
NLM_EXTERN ID2SSequencePiecePtr LIBCALL ID2SSequencePieceNew PROTO (( void ));
NLM_EXTERN ID2SSequencePiecePtr LIBCALL ID2SSequencePieceAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SSequencePieceAsnWrite PROTO (( ID2SSequencePiecePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SGiRange
*
**************************************************/
typedef struct struct_ID2S_Gi_Range {
   Int4   start;
   Int4   count;
} ID2SGiRange, PNTR ID2SGiRangePtr;


NLM_EXTERN ID2SGiRangePtr LIBCALL ID2SGiRangeFree PROTO ((ID2SGiRangePtr ));
NLM_EXTERN ID2SGiRangePtr LIBCALL ID2SGiRangeNew PROTO (( void ));
NLM_EXTERN ID2SGiRangePtr LIBCALL ID2SGiRangeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SGiRangeAsnWrite PROTO (( ID2SGiRangePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SGiInterval
*
**************************************************/
typedef struct struct_ID2S_Gi_Interval {
   Int4   gi;
   Int4   start;
   Int4   length;
} ID2SGiInterval, PNTR ID2SGiIntervalPtr;


NLM_EXTERN ID2SGiIntervalPtr LIBCALL ID2SGiIntervalFree PROTO ((ID2SGiIntervalPtr ));
NLM_EXTERN ID2SGiIntervalPtr LIBCALL ID2SGiIntervalNew PROTO (( void ));
NLM_EXTERN ID2SGiIntervalPtr LIBCALL ID2SGiIntervalAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SGiIntervalAsnWrite PROTO (( ID2SGiIntervalPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SSeqIdInterval
*
**************************************************/
typedef struct struct_ID2S_Seq_id_Interval {
   ValNodePtr   seq_id;
   Int4   start;
   Int4   length;
} ID2SSeqIdInterval, PNTR ID2SSeqIdIntervalPtr;


NLM_EXTERN ID2SSeqIdIntervalPtr LIBCALL ID2SSeqIdIntervalFree PROTO ((ID2SSeqIdIntervalPtr ));
NLM_EXTERN ID2SSeqIdIntervalPtr LIBCALL ID2SSeqIdIntervalNew PROTO (( void ));
NLM_EXTERN ID2SSeqIdIntervalPtr LIBCALL ID2SSeqIdIntervalAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SSeqIdIntervalAsnWrite PROTO (( ID2SSeqIdIntervalPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SGiInts
*
**************************************************/
typedef struct struct_ID2S_Gi_Ints {
   Int4   gi;
   struct struct_ID2S_Interval PNTR   ints;
} ID2SGiInts, PNTR ID2SGiIntsPtr;


NLM_EXTERN ID2SGiIntsPtr LIBCALL ID2SGiIntsFree PROTO ((ID2SGiIntsPtr ));
NLM_EXTERN ID2SGiIntsPtr LIBCALL ID2SGiIntsNew PROTO (( void ));
NLM_EXTERN ID2SGiIntsPtr LIBCALL ID2SGiIntsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SGiIntsAsnWrite PROTO (( ID2SGiIntsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SSeqIdInts
*
**************************************************/
typedef struct struct_ID2S_Seq_id_Ints {
   ValNodePtr   seq_id;
   struct struct_ID2S_Interval PNTR   ints;
} ID2SSeqIdInts, PNTR ID2SSeqIdIntsPtr;


NLM_EXTERN ID2SSeqIdIntsPtr LIBCALL ID2SSeqIdIntsFree PROTO ((ID2SSeqIdIntsPtr ));
NLM_EXTERN ID2SSeqIdIntsPtr LIBCALL ID2SSeqIdIntsNew PROTO (( void ));
NLM_EXTERN ID2SSeqIdIntsPtr LIBCALL ID2SSeqIdIntsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SSeqIdIntsAsnWrite PROTO (( ID2SSeqIdIntsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SInterval
*
**************************************************/
typedef struct struct_ID2S_Interval {
   struct struct_ID2S_Interval PNTR next;
   Int4   start;
   Int4   length;
} ID2SInterval, PNTR ID2SIntervalPtr;


NLM_EXTERN ID2SIntervalPtr LIBCALL ID2SIntervalFree PROTO ((ID2SIntervalPtr ));
NLM_EXTERN ID2SIntervalPtr LIBCALL ID2SIntervalNew PROTO (( void ));
NLM_EXTERN ID2SIntervalPtr LIBCALL ID2SIntervalAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SIntervalAsnWrite PROTO (( ID2SIntervalPtr , AsnIoPtr, AsnTypePtr));

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

