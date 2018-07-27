#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <mappubme.h>
#include <objpubme.h>

static Boolean loaded = FALSE;

#include <asnpubme.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objpubmeAsnLoad(void)
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
*    Generated object loaders for Module NCBI-PubMed
*    Generated using ASNCODE Revision: 5.2 at Feb 4, 1997  9:49 AM
*
**************************************************/


/**************************************************
*
*    PubmedEntryNew()
*
**************************************************/
NLM_EXTERN 
PubmedEntryPtr LIBCALL
PubmedEntryNew(void)
{
   PubmedEntryPtr ptr = MemNew((size_t) sizeof(PubmedEntry));

   return ptr;

}


/**************************************************
*
*    PubmedEntryFree()
*
**************************************************/
NLM_EXTERN 
PubmedEntryPtr LIBCALL
PubmedEntryFree(PubmedEntryPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MedlineEntryFree(ptr -> medent);
   MemFree(ptr -> publisher);
   AsnGenericUserSeqOfFree(ptr -> urls, (AsnOptFreeFunc) PubmedUrlFree);
   MemFree(ptr -> pubid);
   return MemFree(ptr);
}


/**************************************************
*
*    PubmedEntryAsnRead()
*
**************************************************/
NLM_EXTERN 
PubmedEntryPtr LIBCALL
PubmedEntryAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   PubmedEntryPtr ptr;

   if (! loaded)
   {
      if (! objpubmeAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* PubmedEntry ::= (self contained) */
      atp = AsnReadId(aip, amp, PUBMED_ENTRY);
   } else {
      atp = AsnLinkType(orig, PUBMED_ENTRY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = PubmedEntryNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == PUBMED_ENTRY_pmid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pmid = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PUBMED_ENTRY_medent) {
      ptr -> medent = MedlineEntryAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PUBMED_ENTRY_publisher) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> publisher = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PUBMED_ENTRY_urls) {
      ptr -> urls = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) PubmedUrlAsnRead, (AsnOptFreeFunc) PubmedUrlFree);
      if (isError && ptr -> urls == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PUBMED_ENTRY_pubid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pubid = av.ptrvalue;
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
   ptr = PubmedEntryFree(ptr);
   goto ret;
}



/**************************************************
*
*    PubmedEntryAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
PubmedEntryAsnWrite(PubmedEntryPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objpubmeAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, PUBMED_ENTRY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> pmid;
   retval = AsnWrite(aip, PUBMED_ENTRY_pmid,  &av);
   if (ptr -> medent != NULL) {
      if ( ! MedlineEntryAsnWrite(ptr -> medent, aip, PUBMED_ENTRY_medent)) {
         goto erret;
      }
   }
   if (ptr -> publisher != NULL) {
      av.ptrvalue = ptr -> publisher;
      retval = AsnWrite(aip, PUBMED_ENTRY_publisher,  &av);
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> urls, (AsnWriteFunc) PubmedUrlAsnWrite, aip, PUBMED_ENTRY_urls, PUBMED_ENTRY_urls_E);
   if (ptr -> pubid != NULL) {
      av.ptrvalue = ptr -> pubid;
      retval = AsnWrite(aip, PUBMED_ENTRY_pubid,  &av);
   }
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
*    PubmedUrlNew()
*
**************************************************/
NLM_EXTERN 
PubmedUrlPtr LIBCALL
PubmedUrlNew(void)
{
   PubmedUrlPtr ptr = MemNew((size_t) sizeof(PubmedUrl));

   return ptr;

}


/**************************************************
*
*    PubmedUrlFree()
*
**************************************************/
NLM_EXTERN 
PubmedUrlPtr LIBCALL
PubmedUrlFree(PubmedUrlPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> location);
   MemFree(ptr -> url);
   return MemFree(ptr);
}


/**************************************************
*
*    PubmedUrlAsnRead()
*
**************************************************/
NLM_EXTERN 
PubmedUrlPtr LIBCALL
PubmedUrlAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   PubmedUrlPtr ptr;

   if (! loaded)
   {
      if (! objpubmeAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* PubmedUrl ::= (self contained) */
      atp = AsnReadId(aip, amp, PUBMED_URL);
   } else {
      atp = AsnLinkType(orig, PUBMED_URL);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = PubmedUrlNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == PUBMED_URL_location) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> location = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PUBMED_URL_url) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> url = av.ptrvalue;
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
   ptr = PubmedUrlFree(ptr);
   goto ret;
}



/**************************************************
*
*    PubmedUrlAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
PubmedUrlAsnWrite(PubmedUrlPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objpubmeAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, PUBMED_URL);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> location != NULL) {
      av.ptrvalue = ptr -> location;
      retval = AsnWrite(aip, PUBMED_URL_location,  &av);
   }
   if (ptr -> url != NULL) {
      av.ptrvalue = ptr -> url;
      retval = AsnWrite(aip, PUBMED_URL_url,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;
erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


