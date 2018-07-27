#ifndef _objtax1_ 
#define _objtax1_ 

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
*    Generated objects for Module NCBI-Taxon1
*    Generated using ASNCODE Revision: 6.12 at Sep 28, 2001 11:31 AM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objtax1AsnLoad PROTO((void));
typedef ValNodePtr Taxon1ReqPtr;
typedef ValNode Taxon1Req;
#define Taxon1Req_init 1
#define Taxon1Req_findname 2
#define Taxon1Req_getdesignator 3
#define Taxon1Req_getunique 4
#define Taxon1Req_getidbyorg 5
#define Taxon1Req_getorgnames 6
#define Taxon1Req_getcde 7
#define Taxon1Req_getranks 8
#define Taxon1Req_getdivs 9
#define Taxon1Req_getgcs 10
#define Taxon1Req_getlineage 11
#define Taxon1Req_getchildren 12
#define Taxon1Req_getbyid 13
#define Taxon1Req_lookup 14
#define Taxon1Req_getorgmod 15
#define Taxon1Req_fini 16
#define Taxon1Req_id4gi 17
#define Taxon1Req_taxachildren 18
#define Taxon1Req_taxalineage 19
#define Taxon1Req_maxtaxid 20


NLM_EXTERN Taxon1ReqPtr LIBCALL Taxon1ReqFree PROTO ((Taxon1ReqPtr ));
NLM_EXTERN Taxon1ReqPtr LIBCALL Taxon1ReqAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Taxon1ReqAsnWrite PROTO (( Taxon1ReqPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Taxon1Info
*
**************************************************/
typedef struct struct_Taxon1_info {
   struct struct_Taxon1_info PNTR next;
   Int4   ival1;
   Int4   ival2;
   CharPtr   sval;
} Taxon1Info, PNTR Taxon1InfoPtr;


NLM_EXTERN Taxon1InfoPtr LIBCALL Taxon1InfoFree PROTO ((Taxon1InfoPtr ));
NLM_EXTERN Taxon1InfoPtr LIBCALL Taxon1InfoNew PROTO (( void ));
NLM_EXTERN Taxon1InfoPtr LIBCALL Taxon1InfoAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Taxon1InfoAsnWrite PROTO (( Taxon1InfoPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr Taxon1RespPtr;
typedef ValNode Taxon1Resp;
#define Taxon1Resp_error 1
#define Taxon1Resp_init 2
#define Taxon1Resp_findname 3
#define Taxon1Resp_getdesignator 4
#define Taxon1Resp_getunique 5
#define Taxon1Resp_getidbyorg 6
#define Taxon1Resp_getorgnames 7
#define Taxon1Resp_getcde 8
#define Taxon1Resp_getranks 9
#define Taxon1Resp_getdivs 10
#define Taxon1Resp_getgcs 11
#define Taxon1Resp_getlineage 12
#define Taxon1Resp_getchildren 13
#define Taxon1Resp_getbyid 14
#define Taxon1Resp_lookup 15
#define Taxon1Resp_getorgmod 16
#define Taxon1Resp_fini 17
#define Taxon1Resp_id4gi 18
#define Taxon1Resp_taxabyid 19
#define Taxon1Resp_taxachildren 20
#define Taxon1Resp_taxalineage 21
#define Taxon1Resp_maxtaxid 22


NLM_EXTERN Taxon1RespPtr LIBCALL Taxon1RespFree PROTO ((Taxon1RespPtr ));
NLM_EXTERN Taxon1RespPtr LIBCALL Taxon1RespAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Taxon1RespAsnWrite PROTO (( Taxon1RespPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Taxon1Error
*
**************************************************/
typedef struct struct_Taxon1_error {
   Uint2   level;
   /* following #defines are for enumerated type, not used by object loaders */
#define Taxon1_error_level_none 0
#define Taxon1_error_level_info 1
#define Taxon1_error_level_warn 2
#define Taxon1_error_level_error 3
#define Taxon1_error_level_fatal 4

   CharPtr   msg;
} Taxon1Error, PNTR Taxon1ErrorPtr;


NLM_EXTERN Taxon1ErrorPtr LIBCALL Taxon1ErrorFree PROTO ((Taxon1ErrorPtr ));
NLM_EXTERN Taxon1ErrorPtr LIBCALL Taxon1ErrorNew PROTO (( void ));
NLM_EXTERN Taxon1ErrorPtr LIBCALL Taxon1ErrorAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Taxon1ErrorAsnWrite PROTO (( Taxon1ErrorPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Taxon1Name
*
**************************************************/
typedef struct struct_Taxon1_name {
   struct struct_Taxon1_name PNTR next;
   Int4   taxid;
   Int4   cde;
   CharPtr   oname;
   CharPtr   uname;
} Taxon1Name, PNTR Taxon1NamePtr;


NLM_EXTERN Taxon1NamePtr LIBCALL Taxon1NameFree PROTO ((Taxon1NamePtr ));
NLM_EXTERN Taxon1NamePtr LIBCALL Taxon1NameNew PROTO (( void ));
NLM_EXTERN Taxon1NamePtr LIBCALL Taxon1NameAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Taxon1NameAsnWrite PROTO (( Taxon1NamePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Taxon1Data
*
**************************************************/
typedef struct struct_Taxon1_data {
   struct struct_Org_ref PNTR   org;
   CharPtr   div;
   CharPtr   embl_code;
   Uint1   is_species_level;
} Taxon1Data, PNTR Taxon1DataPtr;


NLM_EXTERN Taxon1DataPtr LIBCALL Taxon1DataFree PROTO ((Taxon1DataPtr ));
NLM_EXTERN Taxon1DataPtr LIBCALL Taxon1DataNew PROTO (( void ));
NLM_EXTERN Taxon1DataPtr LIBCALL Taxon1DataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Taxon1DataAsnWrite PROTO (( Taxon1DataPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Taxon2Data
*
**************************************************/
typedef struct struct_Taxon2_data {
   struct struct_Org_ref PNTR   org;
   ValNodePtr   blast_name;
   Uint1   is_uncultured;
   Uint1   is_species_level;
} Taxon2Data, PNTR Taxon2DataPtr;


NLM_EXTERN Taxon2DataPtr LIBCALL Taxon2DataFree PROTO ((Taxon2DataPtr ));
NLM_EXTERN Taxon2DataPtr LIBCALL Taxon2DataNew PROTO (( void ));
NLM_EXTERN Taxon2DataPtr LIBCALL Taxon2DataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL Taxon2DataAsnWrite PROTO (( Taxon2DataPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objtax1_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

