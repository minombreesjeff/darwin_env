#ifndef _objtax3_ 
#define _objtax3_ 

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
*    Generated objects for Module NCBI-Taxon3
*    Generated using ASNCODE Revision: 6.14 at Jul 8, 2004 11:45 AM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objtax3AsnLoad PROTO((void));


/**************************************************
*
*    Taxon3Request
*
**************************************************/
typedef struct struct_Taxon3_request {
   ValNodePtr   request;
} Taxon3Request, PNTR Taxon3RequestPtr;


NLM_EXTERN Taxon3RequestPtr LIBCALL Taxon3RequestFree PROTO ((Taxon3RequestPtr ));
NLM_EXTERN Taxon3RequestPtr LIBCALL Taxon3RequestNew PROTO (( void ));
NLM_EXTERN Taxon3RequestPtr LIBCALL Taxon3RequestAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Taxon3RequestAsnWrite PROTO (( Taxon3RequestPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr T3RequestPtr;
typedef ValNode T3Request;
#define T3Request_taxid 1
#define T3Request_name 2
#define T3Request_org 3


NLM_EXTERN T3RequestPtr LIBCALL T3RequestFree PROTO ((T3RequestPtr ));
NLM_EXTERN T3RequestPtr LIBCALL T3RequestAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL T3RequestAsnWrite PROTO (( T3RequestPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Taxon3Reply
*
**************************************************/
typedef struct struct_Taxon3_reply {
   ValNodePtr   reply;
} Taxon3Reply, PNTR Taxon3ReplyPtr;


NLM_EXTERN Taxon3ReplyPtr LIBCALL Taxon3ReplyFree PROTO ((Taxon3ReplyPtr ));
NLM_EXTERN Taxon3ReplyPtr LIBCALL Taxon3ReplyNew PROTO (( void ));
NLM_EXTERN Taxon3ReplyPtr LIBCALL Taxon3ReplyAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Taxon3ReplyAsnWrite PROTO (( Taxon3ReplyPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr T3ReplyPtr;
typedef ValNode T3Reply;
#define T3Reply_error 1
#define T3Reply_data 2


NLM_EXTERN T3ReplyPtr LIBCALL T3ReplyFree PROTO ((T3ReplyPtr ));
NLM_EXTERN T3ReplyPtr LIBCALL T3ReplyAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL T3ReplyAsnWrite PROTO (( T3ReplyPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    T3Error
*
**************************************************/
typedef struct struct_T3Error {
   Uint2   level;
   /* following #defines are for enumerated type, not used by object loaders */
#define T3Error_level_none 0
#define T3Error_level_info 1
#define T3Error_level_warn 2
#define T3Error_level_error 3
#define T3Error_level_fatal 4

   CharPtr   message;
   Int4   taxid;
   CharPtr   name;
   struct struct_Org_ref PNTR   org;
} T3Error, PNTR T3ErrorPtr;


NLM_EXTERN T3ErrorPtr LIBCALL T3ErrorFree PROTO ((T3ErrorPtr ));
NLM_EXTERN T3ErrorPtr LIBCALL T3ErrorNew PROTO (( void ));
NLM_EXTERN T3ErrorPtr LIBCALL T3ErrorAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL T3ErrorAsnWrite PROTO (( T3ErrorPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    T3Data
*
**************************************************/
typedef struct struct_T3Data {
   struct struct_Org_ref PNTR   org;
   ValNodePtr   blast_name_lineage;
   struct struct_T3StatusFlags PNTR   status;
   struct struct_T3RefreshFlags PNTR   refresh;
} T3Data, PNTR T3DataPtr;


NLM_EXTERN T3DataPtr LIBCALL T3DataFree PROTO ((T3DataPtr ));
NLM_EXTERN T3DataPtr LIBCALL T3DataNew PROTO (( void ));
NLM_EXTERN T3DataPtr LIBCALL T3DataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL T3DataAsnWrite PROTO (( T3DataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    T3StatusFlags
*
**************************************************/
typedef struct struct_T3StatusFlags {
   struct struct_T3StatusFlags PNTR next;
   CharPtr   property;
   ValNodePtr   Value_value;
} T3StatusFlags, PNTR T3StatusFlagsPtr;


NLM_EXTERN T3StatusFlagsPtr LIBCALL T3StatusFlagsFree PROTO ((T3StatusFlagsPtr ));
NLM_EXTERN T3StatusFlagsPtr LIBCALL T3StatusFlagsNew PROTO (( void ));
NLM_EXTERN T3StatusFlagsPtr LIBCALL T3StatusFlagsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL T3StatusFlagsAsnWrite PROTO (( T3StatusFlagsPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Value_valuePtr;
typedef ValNode Value_value;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Value_value_bool 1
#define Value_value_int__ 2
#define Value_value_str 3

#ifdef NLM_GENERATED_CODE_PROTO

static Value_valuePtr LIBCALL Value_valueFree PROTO ((Value_valuePtr ));
static Value_valuePtr LIBCALL Value_valueAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Value_valueAsnWrite PROTO (( Value_valuePtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    T3RefreshFlags
*
**************************************************/
typedef struct struct_T3RefreshFlags {
   Uint1   taxid_changed;
   Uint1   taxname_changed;
   Uint1   division_changed;
   Uint1   lineage_changed;
   Uint1   gc_changed;
   Uint1   mgc_changed;
   Uint1   orgmod_changed;
} T3RefreshFlags, PNTR T3RefreshFlagsPtr;


NLM_EXTERN T3RefreshFlagsPtr LIBCALL T3RefreshFlagsFree PROTO ((T3RefreshFlagsPtr ));
NLM_EXTERN T3RefreshFlagsPtr LIBCALL T3RefreshFlagsNew PROTO (( void ));
NLM_EXTERN T3RefreshFlagsPtr LIBCALL T3RefreshFlagsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL T3RefreshFlagsAsnWrite PROTO (( T3RefreshFlagsPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objtax3_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

