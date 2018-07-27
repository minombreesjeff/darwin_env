#ifndef _id2gen_ 
#define _id2gen_ 

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
*    Generated objects for Module NCBI-ID2Access
*    Generated using ASNCODE Revision: 6.0 at May 3, 2004  6:18 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
id2genAsnLoad PROTO((void));


/**************************************************
*
*    ID2RequestPacket
*
**************************************************/
typedef struct struct_ID2Request ID2RequestPacket;
typedef struct struct_ID2Request PNTR ID2RequestPacketPtr;
#define ID2RequestPacketNew() ID2RequestNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN ID2RequestPacketPtr LIBCALL ID2RequestPacketFree PROTO ((ID2RequestPacketPtr ));
NLM_EXTERN ID2RequestPacketPtr LIBCALL ID2RequestPacketNew PROTO (( void ));
NLM_EXTERN ID2RequestPacketPtr LIBCALL ID2RequestPacketAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2RequestPacketAsnWrite PROTO (( ID2RequestPacketPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ID2Request
*
**************************************************/
typedef struct struct_ID2_Request {
   struct struct_ID2_Request PNTR next;
   Int4   serial_number;
   struct struct_ID2_Param PNTR   params;
   ValNodePtr   Request_request;
} ID2Request, PNTR ID2RequestPtr;


NLM_EXTERN ID2RequestPtr LIBCALL ID2RequestFree PROTO ((ID2RequestPtr ));
NLM_EXTERN ID2RequestPtr LIBCALL ID2RequestNew PROTO (( void ));
NLM_EXTERN ID2RequestPtr LIBCALL ID2RequestAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2RequestAsnWrite PROTO (( ID2RequestPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Request_requestPtr;
typedef ValNode Request_request;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Request_request_init 1
#define Request_request_get_packages 2
#define Request_request_get_seq_id 3
#define Request_request_get_blob_id 4
#define Request_request_get_blob_info 5
#define Request_request_reget_blob 6
#define Request_request_get_chunks 7

#ifdef NLM_GENERATED_CODE_PROTO

static Request_requestPtr LIBCALL Request_requestFree PROTO ((Request_requestPtr ));
static Request_requestPtr LIBCALL Request_requestAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Request_requestAsnWrite PROTO (( Request_requestPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ID2Params
*
**************************************************/
typedef struct struct_ID2Param ID2Params;
typedef struct struct_ID2Param PNTR ID2ParamsPtr;
#define ID2ParamsNew() ID2ParamNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN ID2ParamsPtr LIBCALL ID2ParamsFree PROTO ((ID2ParamsPtr ));
NLM_EXTERN ID2ParamsPtr LIBCALL ID2ParamsNew PROTO (( void ));
NLM_EXTERN ID2ParamsPtr LIBCALL ID2ParamsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2ParamsAsnWrite PROTO (( ID2ParamsPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ID2RequestGetPackages
*
**************************************************/
typedef struct struct_ID2_Request_Get_Packages {
   ValNodePtr   names;
   Uint1   no_contents;
} ID2RequestGetPackages, PNTR ID2RequestGetPackagesPtr;


NLM_EXTERN ID2RequestGetPackagesPtr LIBCALL ID2RequestGetPackagesFree PROTO ((ID2RequestGetPackagesPtr ));
NLM_EXTERN ID2RequestGetPackagesPtr LIBCALL ID2RequestGetPackagesNew PROTO (( void ));
NLM_EXTERN ID2RequestGetPackagesPtr LIBCALL ID2RequestGetPackagesAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2RequestGetPackagesAsnWrite PROTO (( ID2RequestGetPackagesPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2RequestGetSeqId
*
**************************************************/
typedef struct struct_ID2_Request_Get_Seq_id {
   ValNodePtr   seq_id;
   Int4   seq_id_type;
} ID2RequestGetSeqId, PNTR ID2RequestGetSeqIdPtr;


NLM_EXTERN ID2RequestGetSeqIdPtr LIBCALL ID2RequestGetSeqIdFree PROTO ((ID2RequestGetSeqIdPtr ));
NLM_EXTERN ID2RequestGetSeqIdPtr LIBCALL ID2RequestGetSeqIdNew PROTO (( void ));
NLM_EXTERN ID2RequestGetSeqIdPtr LIBCALL ID2RequestGetSeqIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2RequestGetSeqIdAsnWrite PROTO (( ID2RequestGetSeqIdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2RequestGetBlobId
*
**************************************************/
typedef struct struct_ID2_Request_Get_Blob_Id {
   struct struct_ID2_Request_Get_Seq_id PNTR   seq_id;
   ValNodePtr   sources;
   Uint1   external;
} ID2RequestGetBlobId, PNTR ID2RequestGetBlobIdPtr;


NLM_EXTERN ID2RequestGetBlobIdPtr LIBCALL ID2RequestGetBlobIdFree PROTO ((ID2RequestGetBlobIdPtr ));
NLM_EXTERN ID2RequestGetBlobIdPtr LIBCALL ID2RequestGetBlobIdNew PROTO (( void ));
NLM_EXTERN ID2RequestGetBlobIdPtr LIBCALL ID2RequestGetBlobIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2RequestGetBlobIdAsnWrite PROTO (( ID2RequestGetBlobIdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2RequestGetBlobInfo
*
**************************************************/
typedef struct struct_ID2_Request_Get_Blob_Info {
   ValNodePtr   BlobId_blob_id;
   Uint1   get_seq_ids;
   struct struct_ID2_Get_Blob_Details PNTR   get_data;
} ID2RequestGetBlobInfo, PNTR ID2RequestGetBlobInfoPtr;


NLM_EXTERN ID2RequestGetBlobInfoPtr LIBCALL ID2RequestGetBlobInfoFree PROTO ((ID2RequestGetBlobInfoPtr ));
NLM_EXTERN ID2RequestGetBlobInfoPtr LIBCALL ID2RequestGetBlobInfoNew PROTO (( void ));
NLM_EXTERN ID2RequestGetBlobInfoPtr LIBCALL ID2RequestGetBlobInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2RequestGetBlobInfoAsnWrite PROTO (( ID2RequestGetBlobInfoPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr BlobId_blob_idPtr;
typedef ValNode BlobId_blob_id;

#endif /* NLM_GENERATED_CODE_PROTO */

#define BlobId_blob_id_blob_id 1
#define BlobId_blob_id_BlobId_Resolve 2

#ifdef NLM_GENERATED_CODE_PROTO

static BlobId_blob_idPtr LIBCALL BlobId_blob_idFree PROTO ((BlobId_blob_idPtr ));
static BlobId_blob_idPtr LIBCALL BlobId_blob_idAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL BlobId_blob_idAsnWrite PROTO (( BlobId_blob_idPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    BlobId_resolve
*
**************************************************/

#ifdef NLM_GENERATED_CODE_PROTO

typedef struct struct_BlobId_Resolve {
   struct struct_ID2_Request_Get_Blob_Id PNTR   request;
   struct struct_ID2_Blob_Id PNTR   exclude_blobs;
} BlobId_resolve, PNTR BlobId_resolvePtr;
#endif /* NLM_GENERATED_CODE_PROTO */

#ifdef NLM_GENERATED_CODE_PROTO

static BlobId_resolvePtr LIBCALL BlobId_resolveFree PROTO ((BlobId_resolvePtr ));
static BlobId_resolvePtr LIBCALL BlobId_resolveNew PROTO (( void ));
static BlobId_resolvePtr LIBCALL BlobId_resolveAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL BlobId_resolveAsnWrite PROTO (( BlobId_resolvePtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ID2RequestReGetBlob
*
**************************************************/
typedef struct struct_ID2_Request_ReGet_Blob {
   struct struct_ID2_Blob_Id PNTR   blob_id;
   Int4   split_version;
   Int4   offset;
} ID2RequestReGetBlob, PNTR ID2RequestReGetBlobPtr;


NLM_EXTERN ID2RequestReGetBlobPtr LIBCALL ID2RequestReGetBlobFree PROTO ((ID2RequestReGetBlobPtr ));
NLM_EXTERN ID2RequestReGetBlobPtr LIBCALL ID2RequestReGetBlobNew PROTO (( void ));
NLM_EXTERN ID2RequestReGetBlobPtr LIBCALL ID2RequestReGetBlobAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2RequestReGetBlobAsnWrite PROTO (( ID2RequestReGetBlobPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SRequestGetChunks
*
**************************************************/
typedef struct struct_ID2S_Request_Get_Chunks {
   struct struct_ID2_Blob_Id PNTR   blob_id;
   ValNodePtr   chunks;
} ID2SRequestGetChunks, PNTR ID2SRequestGetChunksPtr;


NLM_EXTERN ID2SRequestGetChunksPtr LIBCALL ID2SRequestGetChunksFree PROTO ((ID2SRequestGetChunksPtr ));
NLM_EXTERN ID2SRequestGetChunksPtr LIBCALL ID2SRequestGetChunksNew PROTO (( void ));
NLM_EXTERN ID2SRequestGetChunksPtr LIBCALL ID2SRequestGetChunksAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SRequestGetChunksAsnWrite PROTO (( ID2SRequestGetChunksPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr ID2SeqIdPtr;
typedef ValNode ID2SeqId;
#define ID2SeqId_string 1
#define ID2SeqId_seq_id 2


NLM_EXTERN ID2SeqIdPtr LIBCALL ID2SeqIdFree PROTO ((ID2SeqIdPtr ));
NLM_EXTERN ID2SeqIdPtr LIBCALL ID2SeqIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SeqIdAsnWrite PROTO (( ID2SeqIdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2BlobId
*
**************************************************/
typedef struct struct_ID2_Blob_Id {
   struct struct_ID2_Blob_Id PNTR next;
   Int4   sat;
   Int4   sub_sat;
   Int4   sat_key;
   Int4   version;
} ID2BlobId, PNTR ID2BlobIdPtr;


NLM_EXTERN ID2BlobIdPtr LIBCALL ID2BlobIdFree PROTO ((ID2BlobIdPtr ));
NLM_EXTERN ID2BlobIdPtr LIBCALL ID2BlobIdNew PROTO (( void ));
NLM_EXTERN ID2BlobIdPtr LIBCALL ID2BlobIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2BlobIdAsnWrite PROTO (( ID2BlobIdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2GetBlobDetails
*
**************************************************/
typedef struct struct_ID2_Get_Blob_Details {
   ValNodePtr   location;
   Int4   seq_class_level;
   Int4   descr_level;
   Int4   descr_type_mask;
   Int4   annot_type_mask;
   Int4   feat_type_mask;
   Uint2   sequence_level;
   /* following #defines are for enumerated type, not used by object loaders */
#define ID2_Get_Blob_Details_sequence_level_none 0
#define ID2_Get_Blob_Details_sequence_level_seq_map 1
#define ID2_Get_Blob_Details_sequence_level_seq_data 2

} ID2GetBlobDetails, PNTR ID2GetBlobDetailsPtr;


NLM_EXTERN ID2GetBlobDetailsPtr LIBCALL ID2GetBlobDetailsFree PROTO ((ID2GetBlobDetailsPtr ));
NLM_EXTERN ID2GetBlobDetailsPtr LIBCALL ID2GetBlobDetailsNew PROTO (( void ));
NLM_EXTERN ID2GetBlobDetailsPtr LIBCALL ID2GetBlobDetailsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2GetBlobDetailsAsnWrite PROTO (( ID2GetBlobDetailsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2Reply
*
**************************************************/
typedef struct struct_ID2_Reply {
   Int4   serial_number;
   struct struct_ID2_Param PNTR   params;
   struct struct_ID2_Error PNTR   error;
   Uint1   end_of_reply;
   ValNodePtr   Reply_reply;
} ID2Reply, PNTR ID2ReplyPtr;


NLM_EXTERN ID2ReplyPtr LIBCALL ID2ReplyFree PROTO ((ID2ReplyPtr ));
NLM_EXTERN ID2ReplyPtr LIBCALL ID2ReplyNew PROTO (( void ));
NLM_EXTERN ID2ReplyPtr LIBCALL ID2ReplyAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2ReplyAsnWrite PROTO (( ID2ReplyPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Reply_replyPtr;
typedef ValNode Reply_reply;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Reply_reply_init 1
#define Reply_reply_empty 2
#define Reply_reply_get_package 3
#define Reply_reply_get_seq_id 4
#define Reply_reply_get_blob_id 5
#define Reply_reply_get_blob_seq_ids 6
#define Reply_reply_get_blob 7
#define Reply_reply_reget_blob 8
#define Reply_reply_get_split_info 9
#define Reply_reply_get_chunk 10

#ifdef NLM_GENERATED_CODE_PROTO

static Reply_replyPtr LIBCALL Reply_replyFree PROTO ((Reply_replyPtr ));
static Reply_replyPtr LIBCALL Reply_replyAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Reply_replyAsnWrite PROTO (( Reply_replyPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ID2Error
*
**************************************************/
typedef struct struct_ID2_Error {
   struct struct_ID2_Error PNTR next;
   Uint2   severity;
   /* following #defines are for enumerated type, not used by object loaders */
#define ID2_Error_severity_warning 1
#define ID2_Error_severity_failed_command 2
#define ID2_Error_severity_failed_connection 3
#define ID2_Error_severity_failed_server 4
#define ID2_Error_severity_no_data 5
#define ID2_Error_severity_restricted_data 6
#define ID2_Error_severity_unsupported_command 7
#define ID2_Error_severity_invalid_arguments 8

   Int4   retry_delay;
   CharPtr   message;
} ID2Error, PNTR ID2ErrorPtr;


NLM_EXTERN ID2ErrorPtr LIBCALL ID2ErrorFree PROTO ((ID2ErrorPtr ));
NLM_EXTERN ID2ErrorPtr LIBCALL ID2ErrorNew PROTO (( void ));
NLM_EXTERN ID2ErrorPtr LIBCALL ID2ErrorAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2ErrorAsnWrite PROTO (( ID2ErrorPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2ReplyGetPackage
*
**************************************************/
typedef struct struct_ID2_Reply_Get_Package {
   CharPtr   name;
   struct struct_ID2_Param PNTR   params;
} ID2ReplyGetPackage, PNTR ID2ReplyGetPackagePtr;


NLM_EXTERN ID2ReplyGetPackagePtr LIBCALL ID2ReplyGetPackageFree PROTO ((ID2ReplyGetPackagePtr ));
NLM_EXTERN ID2ReplyGetPackagePtr LIBCALL ID2ReplyGetPackageNew PROTO (( void ));
NLM_EXTERN ID2ReplyGetPackagePtr LIBCALL ID2ReplyGetPackageAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2ReplyGetPackageAsnWrite PROTO (( ID2ReplyGetPackagePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2ReplyGetSeqId
*
**************************************************/
typedef struct struct_ID2_Reply_Get_Seq_id {
   struct struct_ID2_Request_Get_Seq_id PNTR   request;
   ValNodePtr   seq_id;
} ID2ReplyGetSeqId, PNTR ID2ReplyGetSeqIdPtr;


NLM_EXTERN ID2ReplyGetSeqIdPtr LIBCALL ID2ReplyGetSeqIdFree PROTO ((ID2ReplyGetSeqIdPtr ));
NLM_EXTERN ID2ReplyGetSeqIdPtr LIBCALL ID2ReplyGetSeqIdNew PROTO (( void ));
NLM_EXTERN ID2ReplyGetSeqIdPtr LIBCALL ID2ReplyGetSeqIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2ReplyGetSeqIdAsnWrite PROTO (( ID2ReplyGetSeqIdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2ReplyGetBlobId
*
**************************************************/
typedef struct struct_ID2_Reply_Get_Blob_Id {
   ValNodePtr   seq_id;
   struct struct_ID2_Blob_Id PNTR   blob_id;
   Int4   split_version;
   struct struct_ID2S_Seq_annot_Info PNTR   annot_info;
} ID2ReplyGetBlobId, PNTR ID2ReplyGetBlobIdPtr;


NLM_EXTERN ID2ReplyGetBlobIdPtr LIBCALL ID2ReplyGetBlobIdFree PROTO ((ID2ReplyGetBlobIdPtr ));
NLM_EXTERN ID2ReplyGetBlobIdPtr LIBCALL ID2ReplyGetBlobIdNew PROTO (( void ));
NLM_EXTERN ID2ReplyGetBlobIdPtr LIBCALL ID2ReplyGetBlobIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2ReplyGetBlobIdAsnWrite PROTO (( ID2ReplyGetBlobIdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2ReplyGetBlobSeqIds
*
**************************************************/
typedef struct struct_ID2_Reply_Get_Blob_Seq_ids {
   struct struct_ID2_Blob_Id PNTR   blob_id;
   struct struct_ID2_Reply_Data PNTR   ids;
} ID2ReplyGetBlobSeqIds, PNTR ID2ReplyGetBlobSeqIdsPtr;


NLM_EXTERN ID2ReplyGetBlobSeqIdsPtr LIBCALL ID2ReplyGetBlobSeqIdsFree PROTO ((ID2ReplyGetBlobSeqIdsPtr ));
NLM_EXTERN ID2ReplyGetBlobSeqIdsPtr LIBCALL ID2ReplyGetBlobSeqIdsNew PROTO (( void ));
NLM_EXTERN ID2ReplyGetBlobSeqIdsPtr LIBCALL ID2ReplyGetBlobSeqIdsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2ReplyGetBlobSeqIdsAsnWrite PROTO (( ID2ReplyGetBlobSeqIdsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2ReplyGetBlob
*
**************************************************/
typedef struct struct_ID2_Reply_Get_Blob {
   struct struct_ID2_Blob_Id PNTR   blob_id;
   Int4   split_version;
   struct struct_ID2_Reply_Data PNTR   data;
} ID2ReplyGetBlob, PNTR ID2ReplyGetBlobPtr;


NLM_EXTERN ID2ReplyGetBlobPtr LIBCALL ID2ReplyGetBlobFree PROTO ((ID2ReplyGetBlobPtr ));
NLM_EXTERN ID2ReplyGetBlobPtr LIBCALL ID2ReplyGetBlobNew PROTO (( void ));
NLM_EXTERN ID2ReplyGetBlobPtr LIBCALL ID2ReplyGetBlobAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2ReplyGetBlobAsnWrite PROTO (( ID2ReplyGetBlobPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2ReplyReGetBlob
*
**************************************************/
typedef struct struct_ID2_Reply_ReGet_Blob {
   struct struct_ID2_Blob_Id PNTR   blob_id;
   Int4   split_version;
   Int4   offset;
   struct struct_ID2_Reply_Data PNTR   data;
} ID2ReplyReGetBlob, PNTR ID2ReplyReGetBlobPtr;


NLM_EXTERN ID2ReplyReGetBlobPtr LIBCALL ID2ReplyReGetBlobFree PROTO ((ID2ReplyReGetBlobPtr ));
NLM_EXTERN ID2ReplyReGetBlobPtr LIBCALL ID2ReplyReGetBlobNew PROTO (( void ));
NLM_EXTERN ID2ReplyReGetBlobPtr LIBCALL ID2ReplyReGetBlobAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2ReplyReGetBlobAsnWrite PROTO (( ID2ReplyReGetBlobPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SReplyGetSplitInfo
*
**************************************************/
typedef struct struct_ID2S_Reply_Get_Split_Info {
   struct struct_ID2_Blob_Id PNTR   blob_id;
   Int4   split_version;
   struct struct_ID2_Reply_Data PNTR   data;
} ID2SReplyGetSplitInfo, PNTR ID2SReplyGetSplitInfoPtr;


NLM_EXTERN ID2SReplyGetSplitInfoPtr LIBCALL ID2SReplyGetSplitInfoFree PROTO ((ID2SReplyGetSplitInfoPtr ));
NLM_EXTERN ID2SReplyGetSplitInfoPtr LIBCALL ID2SReplyGetSplitInfoNew PROTO (( void ));
NLM_EXTERN ID2SReplyGetSplitInfoPtr LIBCALL ID2SReplyGetSplitInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SReplyGetSplitInfoAsnWrite PROTO (( ID2SReplyGetSplitInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2SReplyGetChunk
*
**************************************************/
typedef struct struct_ID2S_Reply_Get_Chunk {
   struct struct_ID2_Blob_Id PNTR   blob_id;
   Int4   chunk_id;
   struct struct_ID2_Reply_Data PNTR   data;
} ID2SReplyGetChunk, PNTR ID2SReplyGetChunkPtr;


NLM_EXTERN ID2SReplyGetChunkPtr LIBCALL ID2SReplyGetChunkFree PROTO ((ID2SReplyGetChunkPtr ));
NLM_EXTERN ID2SReplyGetChunkPtr LIBCALL ID2SReplyGetChunkNew PROTO (( void ));
NLM_EXTERN ID2SReplyGetChunkPtr LIBCALL ID2SReplyGetChunkAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2SReplyGetChunkAsnWrite PROTO (( ID2SReplyGetChunkPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2ReplyData
*
**************************************************/
typedef struct struct_ID2_Reply_Data {
   Int4   data_type;
   Int4   data_format;
   Int4   data_compression;
   ValNodePtr   data;
} ID2ReplyData, PNTR ID2ReplyDataPtr;


NLM_EXTERN ID2ReplyDataPtr LIBCALL ID2ReplyDataFree PROTO ((ID2ReplyDataPtr ));
NLM_EXTERN ID2ReplyDataPtr LIBCALL ID2ReplyDataNew PROTO (( void ));
NLM_EXTERN ID2ReplyDataPtr LIBCALL ID2ReplyDataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2ReplyDataAsnWrite PROTO (( ID2ReplyDataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2BlobSeqIds
*
**************************************************/
typedef struct struct_ID2BlobSeqId ID2BlobSeqIds;
typedef struct struct_ID2BlobSeqId PNTR ID2BlobSeqIdsPtr;
#define ID2BlobSeqIdsNew() ID2BlobSeqIdNew() 

#ifdef NLM_GENERATED_CODE_PROTO

NLM_EXTERN ID2BlobSeqIdsPtr LIBCALL ID2BlobSeqIdsFree PROTO ((ID2BlobSeqIdsPtr ));
NLM_EXTERN ID2BlobSeqIdsPtr LIBCALL ID2BlobSeqIdsNew PROTO (( void ));
NLM_EXTERN ID2BlobSeqIdsPtr LIBCALL ID2BlobSeqIdsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2BlobSeqIdsAsnWrite PROTO (( ID2BlobSeqIdsPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    ID2BlobSeqId
*
**************************************************/
typedef struct struct_ID2_Blob_Seq_id {
   struct struct_ID2_Blob_Seq_id PNTR next;
   ValNodePtr   seq_id;
   Uint1   replaced;
} ID2BlobSeqId, PNTR ID2BlobSeqIdPtr;


NLM_EXTERN ID2BlobSeqIdPtr LIBCALL ID2BlobSeqIdFree PROTO ((ID2BlobSeqIdPtr ));
NLM_EXTERN ID2BlobSeqIdPtr LIBCALL ID2BlobSeqIdNew PROTO (( void ));
NLM_EXTERN ID2BlobSeqIdPtr LIBCALL ID2BlobSeqIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2BlobSeqIdAsnWrite PROTO (( ID2BlobSeqIdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ID2Param
*
**************************************************/
typedef struct struct_ID2_Param {
   struct struct_ID2_Param PNTR next;
   CharPtr   name;
   ValNodePtr   value;
   Uint2   type;
   /* following #defines are for enumerated type, not used by object loaders */
#define ID2_Param_type_set_value 1
#define ID2_Param_type_get_value 2
#define ID2_Param_type_force_value 3
#define ID2_Param_type_use_package 4

} ID2Param, PNTR ID2ParamPtr;


NLM_EXTERN ID2ParamPtr LIBCALL ID2ParamFree PROTO ((ID2ParamPtr ));
NLM_EXTERN ID2ParamPtr LIBCALL ID2ParamNew PROTO (( void ));
NLM_EXTERN ID2ParamPtr LIBCALL ID2ParamAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL ID2ParamAsnWrite PROTO (( ID2ParamPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _id2gen_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

