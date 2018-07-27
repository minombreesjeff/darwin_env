#ifndef _objmdrs_ 
#define _objmdrs_ 

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
*    Generated objects for Module NCBI-Medlars
*    Generated using ASNCODE Revision: 5.2 at Jan 30, 1997 10:13 AM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL objmdrsAsnLoad PROTO((void));


/**************************************************
*
*    MedlarsEntry
*
**************************************************/
typedef struct struct_Medlars_entry {
   Int4   pmid;
   Int4   muid;
   struct struct_Medlars_record PNTR   recs;
} MedlarsEntry, PNTR MedlarsEntryPtr;


NLM_EXTERN MedlarsEntryPtr LIBCALL MedlarsEntryFree PROTO ((MedlarsEntryPtr ));
NLM_EXTERN MedlarsEntryPtr LIBCALL MedlarsEntryNew PROTO (( void ));
NLM_EXTERN MedlarsEntryPtr LIBCALL MedlarsEntryAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MedlarsEntryAsnWrite PROTO (( MedlarsEntryPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MedlarsRecord
*
**************************************************/
typedef struct struct_Medlars_record {
   struct struct_Medlars_record PNTR next;
   Int4   code;
   CharPtr   abbr;
   CharPtr   data;
} MedlarsRecord, PNTR MedlarsRecordPtr;


NLM_EXTERN MedlarsRecordPtr LIBCALL MedlarsRecordFree PROTO ((MedlarsRecordPtr ));
NLM_EXTERN MedlarsRecordPtr LIBCALL MedlarsRecordNew PROTO (( void ));
NLM_EXTERN MedlarsRecordPtr LIBCALL MedlarsRecordAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL MedlarsRecordAsnWrite PROTO (( MedlarsRecordPtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objmdrs_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

