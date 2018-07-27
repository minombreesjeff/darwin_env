#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <objmdrs.h>

static Boolean loaded = FALSE;

#include <asnmdrs.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objmdrsAsnLoad(void)
{

   if ( ! loaded) {
      NLM_EXTERN_LOADS

      if ( ! AsnLoad ())
      return FALSE;
      loaded = TRUE;
   }

   return TRUE;
}



/**************************************************
*    Generated object loaders for Module NCBI-Medlars
*    Generated using ASNCODE Revision: 5.2 at Jan 30, 1997 10:13 AM
*
**************************************************/


/**************************************************
*
*    MedlarsEntryNew()
*
**************************************************/
NLM_EXTERN 
MedlarsEntryPtr LIBCALL
MedlarsEntryNew(void)
{
   MedlarsEntryPtr ptr = MemNew((size_t) sizeof(MedlarsEntry));

   return ptr;

}


/**************************************************
*
*    MedlarsEntryFree()
*
**************************************************/
NLM_EXTERN 
MedlarsEntryPtr LIBCALL
MedlarsEntryFree(MedlarsEntryPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> recs, (AsnOptFreeFunc) MedlarsRecordFree);
   return MemFree(ptr);
}


/**************************************************
*
*    MedlarsEntryAsnRead()
*
**************************************************/
NLM_EXTERN 
MedlarsEntryPtr LIBCALL
MedlarsEntryAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MedlarsEntryPtr ptr;

   if (! loaded)
   {
      if (! objmdrsAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MedlarsEntry ::= (self contained) */
      atp = AsnReadId(aip, amp, MEDLARS_ENTRY);
   } else {
      atp = AsnLinkType(orig, MEDLARS_ENTRY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MedlarsEntryNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MEDLARS_ENTRY_pmid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pmid = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEDLARS_ENTRY_muid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> muid = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEDLARS_ENTRY_recs) {
      ptr -> recs = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MedlarsRecordAsnRead, (AsnOptFreeFunc) MedlarsRecordFree);
      if (isError && ptr -> recs == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = MedlarsEntryFree(ptr);
   goto ret;
}



/**************************************************
*
*    MedlarsEntryAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MedlarsEntryAsnWrite(MedlarsEntryPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmdrsAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MEDLARS_ENTRY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> pmid;
   retval = AsnWrite(aip, MEDLARS_ENTRY_pmid,  &av);
   av.intvalue = ptr -> muid;
   retval = AsnWrite(aip, MEDLARS_ENTRY_muid,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> recs, (AsnWriteFunc) MedlarsRecordAsnWrite, aip, MEDLARS_ENTRY_recs, MEDLARS_ENTRY_recs_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    MedlarsRecordNew()
*
**************************************************/
NLM_EXTERN 
MedlarsRecordPtr LIBCALL
MedlarsRecordNew(void)
{
   MedlarsRecordPtr ptr = MemNew((size_t) sizeof(MedlarsRecord));

   return ptr;

}


/**************************************************
*
*    MedlarsRecordFree()
*
**************************************************/
NLM_EXTERN 
MedlarsRecordPtr LIBCALL
MedlarsRecordFree(MedlarsRecordPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> abbr);
   MemFree(ptr -> data);
   return MemFree(ptr);
}


/**************************************************
*
*    MedlarsRecordAsnRead()
*
**************************************************/
NLM_EXTERN 
MedlarsRecordPtr LIBCALL
MedlarsRecordAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MedlarsRecordPtr ptr;

   if (! loaded)
   {
      if (! objmdrsAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MedlarsRecord ::= (self contained) */
      atp = AsnReadId(aip, amp, MEDLARS_RECORD);
   } else {
      atp = AsnLinkType(orig, MEDLARS_RECORD);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MedlarsRecordNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MEDLARS_RECORD_code) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> code = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEDLARS_RECORD_abbr) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> abbr = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEDLARS_RECORD_data) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> data = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }

   if (AsnReadVal(aip, atp, &av) <= 0) {
      goto erret;
   }
   /* end struct */

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = MedlarsRecordFree(ptr);
   goto ret;
}



/**************************************************
*
*    MedlarsRecordAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MedlarsRecordAsnWrite(MedlarsRecordPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmdrsAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MEDLARS_RECORD);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> code;
   retval = AsnWrite(aip, MEDLARS_RECORD_code,  &av);
   if (ptr -> abbr != NULL) {
      av.ptrvalue = ptr -> abbr;
      retval = AsnWrite(aip, MEDLARS_RECORD_abbr,  &av);
   }
   if (ptr -> data != NULL) {
      av.ptrvalue = ptr -> data;
      retval = AsnWrite(aip, MEDLARS_RECORD_data,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

