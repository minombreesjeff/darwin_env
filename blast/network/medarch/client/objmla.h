#ifndef _objmla_ 
#define _objmla_ 

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
*    Generated objects for Module NCBI-MedArchive
*    Generated using ASNCODE Revision: 5.2 at Feb 6, 1997  4:41 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objmlaAsnLoad PROTO((void));
typedef ValNodePtr MlaRequestPtr;
typedef ValNode MlaRequest;
#define MlaRequest_init 1
#define MlaRequest_getmle 2
#define MlaRequest_getpub 3
#define MlaRequest_gettitle 4
#define MlaRequest_citmatch 5
#define MlaRequest_fini 6
#define MlaRequest_getmriuids 7
#define MlaRequest_getaccuids 8
#define MlaRequest_uidtopmid 9
#define MlaRequest_pmidtouid 10
#define MlaRequest_getmlepmid 11
#define MlaRequest_getpubpmid 12
#define MlaRequest_citmatchpmid 13
#define MlaRequest_getmripmids 14
#define MlaRequest_getaccpmids 15
#define MlaRequest_citlstpmids 16
#define MlaRequest_getmleuid 17
#define MlaRequest_getmlrpmid 18
#define MlaRequest_getmlruid 19


NLM_EXTERN MlaRequestPtr LIBCALL MlaRequestFree PROTO ((MlaRequestPtr ));
NLM_EXTERN MlaRequestPtr LIBCALL MlaRequestAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MlaRequestAsnWrite PROTO (( MlaRequestPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    TitleMsg
*
**************************************************/
typedef struct struct_Title_msg {
   struct struct_Title_msg PNTR next;
   Uint2   type;
   ValNodePtr   title;
} TitleMsg, PNTR TitleMsgPtr;


NLM_EXTERN TitleMsgPtr LIBCALL TitleMsgFree PROTO ((TitleMsgPtr ));
NLM_EXTERN TitleMsgPtr LIBCALL TitleMsgNew PROTO (( void ));
NLM_EXTERN TitleMsgPtr LIBCALL TitleMsgAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TitleMsgAsnWrite PROTO (( TitleMsgPtr , AsnIoPtr, AsnTypePtr));

/* following #defines are for enumerated type, not used by object loaders */
#define Title_type_not_set 0
#define Title_type_name 1
#define Title_type_tsub 2
#define Title_type_trans 3
#define Title_type_jta 4
#define Title_type_iso_jta 5
#define Title_type_ml_jta 6
#define Title_type_coden 7
#define Title_type_issn 8
#define Title_type_abr 9
#define Title_type_isbn 10
#define Title_type_all 255



/**************************************************
*
*    TitleMsgList
*
**************************************************/
typedef struct struct_Title_msg_list {
   Int4   num;
   struct struct_Title_msg PNTR   titles;
} TitleMsgList, PNTR TitleMsgListPtr;


NLM_EXTERN TitleMsgListPtr LIBCALL TitleMsgListFree PROTO ((TitleMsgListPtr ));
NLM_EXTERN TitleMsgListPtr LIBCALL TitleMsgListNew PROTO (( void ));
NLM_EXTERN TitleMsgListPtr LIBCALL TitleMsgListAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL TitleMsgListAsnWrite PROTO (( TitleMsgListPtr , AsnIoPtr, AsnTypePtr));

/* following #defines are for enumerated type, not used by object loaders */
#define Error_val_not_found 0
#define Error_val_operational_error 1
#define Error_val_cannot_connect_jrsrv 2
#define Error_val_cannot_connect_pmdb 3
#define Error_val_journal_not_found 4
#define Error_val_citation_not_found 5
#define Error_val_citation_ambiguous 6
#define Error_val_citation_too_many 7

typedef ValNodePtr MlaBackPtr;
typedef ValNode MlaBack;
#define MlaBack_init 1
#define MlaBack_error 2
#define MlaBack_getmle 3
#define MlaBack_getpub 4
#define MlaBack_gettitle 5
#define MlaBack_citmatch 6
#define MlaBack_fini 7
#define MlaBack_getuids 8
#define MlaBack_getpmids 9
#define MlaBack_outuid 10
#define MlaBack_outpmid 11
#define MlaBack_getpme 12
#define MlaBack_getmlr 13


NLM_EXTERN MlaBackPtr LIBCALL MlaBackFree PROTO ((MlaBackPtr ));
NLM_EXTERN MlaBackPtr LIBCALL MlaBackAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MlaBackAsnWrite PROTO (( MlaBackPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objmla_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

