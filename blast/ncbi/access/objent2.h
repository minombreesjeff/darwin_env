#ifndef _objent2_ 
#define _objent2_ 

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
*    Generated objects for Module NCBI-Entrez2
*    Generated using ASNCODE Revision: 6.13 at Jan 28, 2002 12:12 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objent2AsnLoad PROTO((void));


/**************************************************
*
*    Entrez2IdList
*
**************************************************/
typedef struct struct_Entrez2_id_list {
   CharPtr   db;
   Int4   num;
   ByteStorePtr   uids;
} Entrez2IdList, PNTR Entrez2IdListPtr;


NLM_EXTERN Entrez2IdListPtr LIBCALL Entrez2IdListFree PROTO ((Entrez2IdListPtr ));
NLM_EXTERN Entrez2IdListPtr LIBCALL Entrez2IdListNew PROTO (( void ));
NLM_EXTERN Entrez2IdListPtr LIBCALL Entrez2IdListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2IdListAsnWrite PROTO (( Entrez2IdListPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2BooleanExp
*
**************************************************/
typedef struct struct_Entrez2_boolean_exp {
   CharPtr   db;
   ValNodePtr   exp;
   struct struct_Entrez2_limits PNTR   limits;
} Entrez2BooleanExp, PNTR Entrez2BooleanExpPtr;


NLM_EXTERN Entrez2BooleanExpPtr LIBCALL Entrez2BooleanExpFree PROTO ((Entrez2BooleanExpPtr ));
NLM_EXTERN Entrez2BooleanExpPtr LIBCALL Entrez2BooleanExpNew PROTO (( void ));
NLM_EXTERN Entrez2BooleanExpPtr LIBCALL Entrez2BooleanExpAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2BooleanExpAsnWrite PROTO (( Entrez2BooleanExpPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr Entrez2BooleanElementPtr;
typedef ValNode Entrez2BooleanElement;
#define Entrez2BooleanElement_str 1
#define Entrez2BooleanElement_op 2
#define Entrez2BooleanElement_term 3
#define Entrez2BooleanElement_ids 4
#define Entrez2BooleanElement_key 5


NLM_EXTERN Entrez2BooleanElementPtr LIBCALL Entrez2BooleanElementFree PROTO ((Entrez2BooleanElementPtr ));
NLM_EXTERN Entrez2BooleanElementPtr LIBCALL Entrez2BooleanElementAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2BooleanElementAsnWrite PROTO (( Entrez2BooleanElementPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2Limits
*
**************************************************/
typedef struct struct_Entrez2_limits {
   struct struct_Entrez2_dt_filter PNTR   filter_date;
   Int4   max_UIDs;
   Int4   offset_UIDs;
} Entrez2Limits, PNTR Entrez2LimitsPtr;


NLM_EXTERN Entrez2LimitsPtr LIBCALL Entrez2LimitsFree PROTO ((Entrez2LimitsPtr ));
NLM_EXTERN Entrez2LimitsPtr LIBCALL Entrez2LimitsNew PROTO (( void ));
NLM_EXTERN Entrez2LimitsPtr LIBCALL Entrez2LimitsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2LimitsAsnWrite PROTO (( Entrez2LimitsPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2BooleanTerm
*
**************************************************/
typedef struct struct_Entrez2_boolean_term {
   CharPtr   field;
   CharPtr   term;
   Int4   term_count;
   Uint1   do_not_explode;
   Uint1   do_not_translate;
} Entrez2BooleanTerm, PNTR Entrez2BooleanTermPtr;


NLM_EXTERN Entrez2BooleanTermPtr LIBCALL Entrez2BooleanTermFree PROTO ((Entrez2BooleanTermPtr ));
NLM_EXTERN Entrez2BooleanTermPtr LIBCALL Entrez2BooleanTermNew PROTO (( void ));
NLM_EXTERN Entrez2BooleanTermPtr LIBCALL Entrez2BooleanTermAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2BooleanTermAsnWrite PROTO (( Entrez2BooleanTermPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2Request
*
**************************************************/
typedef struct struct_Entrez2_request {
   ValNodePtr   request;
   Int4   version;
   CharPtr   tool;
   CharPtr   cookie;
   Uint1   use_history;
} Entrez2Request, PNTR Entrez2RequestPtr;


NLM_EXTERN Entrez2RequestPtr LIBCALL Entrez2RequestFree PROTO ((Entrez2RequestPtr ));
NLM_EXTERN Entrez2RequestPtr LIBCALL Entrez2RequestNew PROTO (( void ));
NLM_EXTERN Entrez2RequestPtr LIBCALL Entrez2RequestAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2RequestAsnWrite PROTO (( Entrez2RequestPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr E2RequestPtr;
typedef ValNode E2Request;
#define E2Request_get_info 1
#define E2Request_eval_boolean 2
#define E2Request_get_docsum 3
#define E2Request_get_term_pos 4
#define E2Request_get_term_list 5
#define E2Request_get_term_hierarchy 6
#define E2Request_get_links 7
#define E2Request_get_linked 8
#define E2Request_get_link_counts 9


NLM_EXTERN E2RequestPtr LIBCALL E2RequestFree PROTO ((E2RequestPtr ));
NLM_EXTERN E2RequestPtr LIBCALL E2RequestAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL E2RequestAsnWrite PROTO (( E2RequestPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2EvalBoolean
*
**************************************************/
typedef struct struct_Entrez2_eval_boolean {
   Uint1   return_UIDs;
   Uint1   return_parse;
   struct struct_Entrez2_boolean_exp PNTR   query;
} Entrez2EvalBoolean, PNTR Entrez2EvalBooleanPtr;


NLM_EXTERN Entrez2EvalBooleanPtr LIBCALL Entrez2EvalBooleanFree PROTO ((Entrez2EvalBooleanPtr ));
NLM_EXTERN Entrez2EvalBooleanPtr LIBCALL Entrez2EvalBooleanNew PROTO (( void ));
NLM_EXTERN Entrez2EvalBooleanPtr LIBCALL Entrez2EvalBooleanAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2EvalBooleanAsnWrite PROTO (( Entrez2EvalBooleanPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2TermQuery
*
**************************************************/
typedef struct struct_Entrez2_term_query {
   CharPtr   db;
   CharPtr   field;
   CharPtr   term;
} Entrez2TermQuery, PNTR Entrez2TermQueryPtr;


NLM_EXTERN Entrez2TermQueryPtr LIBCALL Entrez2TermQueryFree PROTO ((Entrez2TermQueryPtr ));
NLM_EXTERN Entrez2TermQueryPtr LIBCALL Entrez2TermQueryNew PROTO (( void ));
NLM_EXTERN Entrez2TermQueryPtr LIBCALL Entrez2TermQueryAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2TermQueryAsnWrite PROTO (( Entrez2TermQueryPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2TermPos
*
**************************************************/
typedef struct struct_Entrez2_term_pos {
   CharPtr   db;
   CharPtr   field;
   Int4   first_term_pos;
   Int4   number_of_terms;
} Entrez2TermPos, PNTR Entrez2TermPosPtr;


NLM_EXTERN Entrez2TermPosPtr LIBCALL Entrez2TermPosFree PROTO ((Entrez2TermPosPtr ));
NLM_EXTERN Entrez2TermPosPtr LIBCALL Entrez2TermPosNew PROTO (( void ));
NLM_EXTERN Entrez2TermPosPtr LIBCALL Entrez2TermPosAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2TermPosAsnWrite PROTO (( Entrez2TermPosPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2HierQuery
*
**************************************************/
typedef struct struct_Entrez2_hier_query {
   CharPtr   db;
   CharPtr   field;
   CharPtr   term;
   Int4   txid;
} Entrez2HierQuery, PNTR Entrez2HierQueryPtr;


NLM_EXTERN Entrez2HierQueryPtr LIBCALL Entrez2HierQueryFree PROTO ((Entrez2HierQueryPtr ));
NLM_EXTERN Entrez2HierQueryPtr LIBCALL Entrez2HierQueryNew PROTO (( void ));
NLM_EXTERN Entrez2HierQueryPtr LIBCALL Entrez2HierQueryAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2HierQueryAsnWrite PROTO (( Entrez2HierQueryPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2GetLinks
*
**************************************************/
typedef struct struct_Entrez2_get_links {
   struct struct_Entrez2_id_list PNTR   uids;
   CharPtr   linktype;
   Int4   max_UIDS;
   Uint1   count_only;
   Uint1   parents_persist;
} Entrez2GetLinks, PNTR Entrez2GetLinksPtr;


NLM_EXTERN Entrez2GetLinksPtr LIBCALL Entrez2GetLinksFree PROTO ((Entrez2GetLinksPtr ));
NLM_EXTERN Entrez2GetLinksPtr LIBCALL Entrez2GetLinksNew PROTO (( void ));
NLM_EXTERN Entrez2GetLinksPtr LIBCALL Entrez2GetLinksAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2GetLinksAsnWrite PROTO (( Entrez2GetLinksPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2Id
*
**************************************************/
typedef struct struct_Entrez2_id {
   CharPtr   db;
   Int4   uid;
} Entrez2Id, PNTR Entrez2IdPtr;


NLM_EXTERN Entrez2IdPtr LIBCALL Entrez2IdFree PROTO ((Entrez2IdPtr ));
NLM_EXTERN Entrez2IdPtr LIBCALL Entrez2IdNew PROTO (( void ));
NLM_EXTERN Entrez2IdPtr LIBCALL Entrez2IdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2IdAsnWrite PROTO (( Entrez2IdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2DtFilter
*
**************************************************/
typedef struct struct_Entrez2_dt_filter {
   Int4   begin_date;
   Int4   end_date;
   CharPtr   type_date;
} Entrez2DtFilter, PNTR Entrez2DtFilterPtr;


NLM_EXTERN Entrez2DtFilterPtr LIBCALL Entrez2DtFilterFree PROTO ((Entrez2DtFilterPtr ));
NLM_EXTERN Entrez2DtFilterPtr LIBCALL Entrez2DtFilterNew PROTO (( void ));
NLM_EXTERN Entrez2DtFilterPtr LIBCALL Entrez2DtFilterAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2DtFilterAsnWrite PROTO (( Entrez2DtFilterPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2Reply
*
**************************************************/
typedef struct struct_Entrez2_reply {
   ValNodePtr   reply;
   Int4   dt;
   CharPtr   server;
   CharPtr   msg;
   CharPtr   key;
   CharPtr   cookie;
} Entrez2Reply, PNTR Entrez2ReplyPtr;


NLM_EXTERN Entrez2ReplyPtr LIBCALL Entrez2ReplyFree PROTO ((Entrez2ReplyPtr ));
NLM_EXTERN Entrez2ReplyPtr LIBCALL Entrez2ReplyNew PROTO (( void ));
NLM_EXTERN Entrez2ReplyPtr LIBCALL Entrez2ReplyAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2ReplyAsnWrite PROTO (( Entrez2ReplyPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr E2ReplyPtr;
typedef ValNode E2Reply;
#define E2Reply_error 1
#define E2Reply_get_info 2
#define E2Reply_eval_boolean 3
#define E2Reply_get_docsum 4
#define E2Reply_get_term_pos 5
#define E2Reply_get_term_list 6
#define E2Reply_get_term_hierarchy 7
#define E2Reply_get_links 8
#define E2Reply_get_linked 9
#define E2Reply_get_link_counts 10


NLM_EXTERN E2ReplyPtr LIBCALL E2ReplyFree PROTO ((E2ReplyPtr ));
NLM_EXTERN E2ReplyPtr LIBCALL E2ReplyAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL E2ReplyAsnWrite PROTO (( E2ReplyPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2Info
*
**************************************************/
typedef struct struct_Entrez2_info {
   Int4   db_count;
   Int4   build_date;
   struct struct_Entrez2_db_info PNTR   db_info;
} Entrez2Info, PNTR Entrez2InfoPtr;


NLM_EXTERN Entrez2InfoPtr LIBCALL Entrez2InfoFree PROTO ((Entrez2InfoPtr ));
NLM_EXTERN Entrez2InfoPtr LIBCALL Entrez2InfoNew PROTO (( void ));
NLM_EXTERN Entrez2InfoPtr LIBCALL Entrez2InfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2InfoAsnWrite PROTO (( Entrez2InfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2BooleanReply
*
**************************************************/
typedef struct struct_Entrez2_boolean_reply {
   Int4   count;
   struct struct_Entrez2_id_list PNTR   uids;
   struct struct_Entrez2_boolean_exp PNTR   query;
} Entrez2BooleanReply, PNTR Entrez2BooleanReplyPtr;


NLM_EXTERN Entrez2BooleanReplyPtr LIBCALL Entrez2BooleanReplyFree PROTO ((Entrez2BooleanReplyPtr ));
NLM_EXTERN Entrez2BooleanReplyPtr LIBCALL Entrez2BooleanReplyNew PROTO (( void ));
NLM_EXTERN Entrez2BooleanReplyPtr LIBCALL Entrez2BooleanReplyAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2BooleanReplyAsnWrite PROTO (( Entrez2BooleanReplyPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2DocsumList
*
**************************************************/
typedef struct struct_Entrez2_docsum_list {
   Int4   count;
   struct struct_Entrez2_docsum PNTR   list;
} Entrez2DocsumList, PNTR Entrez2DocsumListPtr;


NLM_EXTERN Entrez2DocsumListPtr LIBCALL Entrez2DocsumListFree PROTO ((Entrez2DocsumListPtr ));
NLM_EXTERN Entrez2DocsumListPtr LIBCALL Entrez2DocsumListNew PROTO (( void ));
NLM_EXTERN Entrez2DocsumListPtr LIBCALL Entrez2DocsumListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2DocsumListAsnWrite PROTO (( Entrez2DocsumListPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2TermList
*
**************************************************/
typedef struct struct_Entrez2_term_list {
   Int4   pos;
   Int4   num;
   struct struct_Entrez2_term PNTR   list;
} Entrez2TermList, PNTR Entrez2TermListPtr;


NLM_EXTERN Entrez2TermListPtr LIBCALL Entrez2TermListFree PROTO ((Entrez2TermListPtr ));
NLM_EXTERN Entrez2TermListPtr LIBCALL Entrez2TermListNew PROTO (( void ));
NLM_EXTERN Entrez2TermListPtr LIBCALL Entrez2TermListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2TermListAsnWrite PROTO (( Entrez2TermListPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2HierNode
*
**************************************************/
typedef struct struct_Entrez2_hier_node {
   CharPtr   cannonical_form;
   Int4   lineage_count;
   struct struct_Entrez2_term PNTR   lineage;
   Int4   child_count;
   struct struct_Entrez2_term PNTR   children;
   Uint1   is_ambiguous;
} Entrez2HierNode, PNTR Entrez2HierNodePtr;


NLM_EXTERN Entrez2HierNodePtr LIBCALL Entrez2HierNodeFree PROTO ((Entrez2HierNodePtr ));
NLM_EXTERN Entrez2HierNodePtr LIBCALL Entrez2HierNodeNew PROTO (( void ));
NLM_EXTERN Entrez2HierNodePtr LIBCALL Entrez2HierNodeAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2HierNodeAsnWrite PROTO (( Entrez2HierNodePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2LinkSet
*
**************************************************/
typedef struct struct_Entrez2_link_set {
   struct struct_Entrez2_id_list PNTR   ids;
   Int4   data_size;
   ByteStorePtr   data;
} Entrez2LinkSet, PNTR Entrez2LinkSetPtr;


NLM_EXTERN Entrez2LinkSetPtr LIBCALL Entrez2LinkSetFree PROTO ((Entrez2LinkSetPtr ));
NLM_EXTERN Entrez2LinkSetPtr LIBCALL Entrez2LinkSetNew PROTO (( void ));
NLM_EXTERN Entrez2LinkSetPtr LIBCALL Entrez2LinkSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2LinkSetAsnWrite PROTO (( Entrez2LinkSetPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2LinkCountList
*
**************************************************/
typedef struct struct_Entrez2_link_count_list {
   Int4   link_type_count;
   struct struct_Entrez2_link_count PNTR   links;
} Entrez2LinkCountList, PNTR Entrez2LinkCountListPtr;


NLM_EXTERN Entrez2LinkCountListPtr LIBCALL Entrez2LinkCountListFree PROTO ((Entrez2LinkCountListPtr ));
NLM_EXTERN Entrez2LinkCountListPtr LIBCALL Entrez2LinkCountListNew PROTO (( void ));
NLM_EXTERN Entrez2LinkCountListPtr LIBCALL Entrez2LinkCountListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2LinkCountListAsnWrite PROTO (( Entrez2LinkCountListPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2DbInfo
*
**************************************************/
typedef struct struct_Entrez2_db_info {
   struct struct_Entrez2_db_info PNTR next;
   CharPtr   db_name;
   CharPtr   db_menu;
   CharPtr   db_descr;
   Int4   doc_count;
   Int4   field_count;
   struct struct_Entrez2_field_info PNTR   fields;
   Int4   link_count;
   struct struct_Entrez2_link_info PNTR   links;
   Int4   docsum_field_count;
   struct struct_Entrez2_docsum_field_info PNTR   docsum_fields;
} Entrez2DbInfo, PNTR Entrez2DbInfoPtr;


NLM_EXTERN Entrez2DbInfoPtr LIBCALL Entrez2DbInfoFree PROTO ((Entrez2DbInfoPtr ));
NLM_EXTERN Entrez2DbInfoPtr LIBCALL Entrez2DbInfoNew PROTO (( void ));
NLM_EXTERN Entrez2DbInfoPtr LIBCALL Entrez2DbInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2DbInfoAsnWrite PROTO (( Entrez2DbInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2FieldInfo
*
**************************************************/
typedef struct struct_Entrez2_field_info {
   struct struct_Entrez2_field_info PNTR next;
   CharPtr   field_name;
   CharPtr   field_menu;
   CharPtr   field_descr;
   Int4   term_count;
   Uint1   is_date;
   Uint1   is_numerical;
   Uint1   single_token;
   Uint1   hierarchy_avail;
   Uint1   is_rangable;
   Uint1   is_truncatable;
} Entrez2FieldInfo, PNTR Entrez2FieldInfoPtr;


NLM_EXTERN Entrez2FieldInfoPtr LIBCALL Entrez2FieldInfoFree PROTO ((Entrez2FieldInfoPtr ));
NLM_EXTERN Entrez2FieldInfoPtr LIBCALL Entrez2FieldInfoNew PROTO (( void ));
NLM_EXTERN Entrez2FieldInfoPtr LIBCALL Entrez2FieldInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2FieldInfoAsnWrite PROTO (( Entrez2FieldInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2LinkInfo
*
**************************************************/
typedef struct struct_Entrez2_link_info {
   struct struct_Entrez2_link_info PNTR next;
   CharPtr   link_name;
   CharPtr   link_menu;
   CharPtr   link_descr;
   CharPtr   db_to;
   Int4   data_size;
} Entrez2LinkInfo, PNTR Entrez2LinkInfoPtr;


NLM_EXTERN Entrez2LinkInfoPtr LIBCALL Entrez2LinkInfoFree PROTO ((Entrez2LinkInfoPtr ));
NLM_EXTERN Entrez2LinkInfoPtr LIBCALL Entrez2LinkInfoNew PROTO (( void ));
NLM_EXTERN Entrez2LinkInfoPtr LIBCALL Entrez2LinkInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2LinkInfoAsnWrite PROTO (( Entrez2LinkInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2DocsumFieldInfo
*
**************************************************/
typedef struct struct_Entrez2_docsum_field_info {
   struct struct_Entrez2_docsum_field_info PNTR next;
   CharPtr   field_name;
   CharPtr   field_description;
   Int4   field_type;
} Entrez2DocsumFieldInfo, PNTR Entrez2DocsumFieldInfoPtr;


NLM_EXTERN Entrez2DocsumFieldInfoPtr LIBCALL Entrez2DocsumFieldInfoFree PROTO ((Entrez2DocsumFieldInfoPtr ));
NLM_EXTERN Entrez2DocsumFieldInfoPtr LIBCALL Entrez2DocsumFieldInfoNew PROTO (( void ));
NLM_EXTERN Entrez2DocsumFieldInfoPtr LIBCALL Entrez2DocsumFieldInfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2DocsumFieldInfoAsnWrite PROTO (( Entrez2DocsumFieldInfoPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2Docsum
*
**************************************************/
typedef struct struct_Entrez2_docsum {
   struct struct_Entrez2_docsum PNTR next;
   Int4   uid;
   struct struct_Entrez2_docsum_data PNTR   docsum_data;
} Entrez2Docsum, PNTR Entrez2DocsumPtr;


NLM_EXTERN Entrez2DocsumPtr LIBCALL Entrez2DocsumFree PROTO ((Entrez2DocsumPtr ));
NLM_EXTERN Entrez2DocsumPtr LIBCALL Entrez2DocsumNew PROTO (( void ));
NLM_EXTERN Entrez2DocsumPtr LIBCALL Entrez2DocsumAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2DocsumAsnWrite PROTO (( Entrez2DocsumPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2DocsumData
*
**************************************************/
typedef struct struct_Entrez2_docsum_data {
   struct struct_Entrez2_docsum_data PNTR next;
   CharPtr   field_name;
   CharPtr   field_value;
} Entrez2DocsumData, PNTR Entrez2DocsumDataPtr;


NLM_EXTERN Entrez2DocsumDataPtr LIBCALL Entrez2DocsumDataFree PROTO ((Entrez2DocsumDataPtr ));
NLM_EXTERN Entrez2DocsumDataPtr LIBCALL Entrez2DocsumDataNew PROTO (( void ));
NLM_EXTERN Entrez2DocsumDataPtr LIBCALL Entrez2DocsumDataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2DocsumDataAsnWrite PROTO (( Entrez2DocsumDataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2Term
*
**************************************************/
typedef struct struct_Entrez2_term {
   struct struct_Entrez2_term PNTR next;
   CharPtr   term;
   Int4   txid;
   Int4   count;
   Uint1   is_leaf_node;
} Entrez2Term, PNTR Entrez2TermPtr;


NLM_EXTERN Entrez2TermPtr LIBCALL Entrez2TermFree PROTO ((Entrez2TermPtr ));
NLM_EXTERN Entrez2TermPtr LIBCALL Entrez2TermNew PROTO (( void ));
NLM_EXTERN Entrez2TermPtr LIBCALL Entrez2TermAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2TermAsnWrite PROTO (( Entrez2TermPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Entrez2LinkCount
*
**************************************************/
typedef struct struct_Entrez2_link_count {
   struct struct_Entrez2_link_count PNTR next;
   CharPtr   link_type;
   Int4   link_count;
} Entrez2LinkCount, PNTR Entrez2LinkCountPtr;


NLM_EXTERN Entrez2LinkCountPtr LIBCALL Entrez2LinkCountFree PROTO ((Entrez2LinkCountPtr ));
NLM_EXTERN Entrez2LinkCountPtr LIBCALL Entrez2LinkCountNew PROTO (( void ));
NLM_EXTERN Entrez2LinkCountPtr LIBCALL Entrez2LinkCountAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Entrez2LinkCountAsnWrite PROTO (( Entrez2LinkCountPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objent2_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

