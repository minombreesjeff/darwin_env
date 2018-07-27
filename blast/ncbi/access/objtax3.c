#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <strimprt.h>
#include <objtax3.h>

static Boolean loaded = FALSE;

#include <asntax3.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objtax3AsnLoad(void)
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
*    Generated object loaders for Module NCBI-Taxon3
*    Generated using ASNCODE Revision: 6.14 at Jul 8, 2004 11:45 AM
*
**************************************************/


/**************************************************
*
*    Taxon3RequestNew()
*
**************************************************/
NLM_EXTERN 
Taxon3RequestPtr LIBCALL
Taxon3RequestNew(void)
{
   Taxon3RequestPtr ptr = MemNew((size_t) sizeof(Taxon3Request));

   return ptr;

}


/**************************************************
*
*    Taxon3RequestFree()
*
**************************************************/
NLM_EXTERN 
Taxon3RequestPtr LIBCALL
Taxon3RequestFree(Taxon3RequestPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> request, (AsnOptFreeFunc) T3RequestFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Taxon3RequestAsnRead()
*
**************************************************/
NLM_EXTERN 
Taxon3RequestPtr LIBCALL
Taxon3RequestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Taxon3RequestPtr ptr;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Taxon3Request ::= (self contained) */
      atp = AsnReadId(aip, amp, TAXON3_REQUEST);
   } else {
      atp = AsnLinkType(orig, TAXON3_REQUEST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Taxon3RequestNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TAXON3_REQUEST_request) {
      ptr -> request = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) T3RequestAsnRead, (AsnOptFreeFunc) T3RequestFree);
      if (isError && ptr -> request == NULL) {
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
   ptr = Taxon3RequestFree(ptr);
   goto ret;
}



/**************************************************
*
*    Taxon3RequestAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Taxon3RequestAsnWrite(Taxon3RequestPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TAXON3_REQUEST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericChoiceSeqOfAsnWrite(ptr -> request, (AsnWriteFunc) T3RequestAsnWrite, aip, TAXON3_REQUEST_request, TAXON3_REQUEST_request_E);
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
*    T3RequestFree()
*
**************************************************/
NLM_EXTERN 
T3RequestPtr LIBCALL
T3RequestFree(ValNodePtr anp)
{
   Pointer pnt;

   if (anp == NULL) {
      return NULL;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   default:
      break;
   case T3Request_name:
      MemFree(anp -> data.ptrvalue);
      break;
   case T3Request_org:
      OrgRefFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    T3RequestAsnRead()
*
**************************************************/
NLM_EXTERN 
T3RequestPtr LIBCALL
T3RequestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   Boolean nullIsError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* T3Request ::= (self contained) */
      atp = AsnReadId(aip, amp, T3REQUEST);
   } else {
      atp = AsnLinkType(orig, T3REQUEST);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp = ValNodeNew(NULL);
   if (anp == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */
      goto erret;
   }

   func = NULL;

   atp = AsnReadId(aip, amp, atp);  /* find the choice */
   if (atp == NULL) {
      goto erret;
   }
   if (atp == T3REQUEST_taxid) {
      choice = T3Request_taxid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == T3REQUEST_name) {
      choice = T3Request_name;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == T3REQUEST_org) {
      choice = T3Request_org;
      func = (AsnReadFunc) OrgRefAsnRead;
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (aip -> io_failure) goto erret;

      if (nullIsError && anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    T3RequestAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
T3RequestAsnWrite(T3RequestPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, T3REQUEST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   case T3Request_taxid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, T3REQUEST_taxid, &av);
      break;
   case T3Request_name:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, T3REQUEST_name, &av);
      break;
   case T3Request_org:
      writetype = T3REQUEST_org;
      func = (AsnWriteFunc) OrgRefAsnWrite;
      break;
   }
   if (writetype != NULL) {
      retval = (* func)(pnt, aip, writetype);   /* write it out */
   }
   if (!retval) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    Taxon3ReplyNew()
*
**************************************************/
NLM_EXTERN 
Taxon3ReplyPtr LIBCALL
Taxon3ReplyNew(void)
{
   Taxon3ReplyPtr ptr = MemNew((size_t) sizeof(Taxon3Reply));

   return ptr;

}


/**************************************************
*
*    Taxon3ReplyFree()
*
**************************************************/
NLM_EXTERN 
Taxon3ReplyPtr LIBCALL
Taxon3ReplyFree(Taxon3ReplyPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> reply, (AsnOptFreeFunc) T3ReplyFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Taxon3ReplyAsnRead()
*
**************************************************/
NLM_EXTERN 
Taxon3ReplyPtr LIBCALL
Taxon3ReplyAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Taxon3ReplyPtr ptr;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Taxon3Reply ::= (self contained) */
      atp = AsnReadId(aip, amp, TAXON3_REPLY);
   } else {
      atp = AsnLinkType(orig, TAXON3_REPLY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Taxon3ReplyNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TAXON3_REPLY_reply) {
      ptr -> reply = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) T3ReplyAsnRead, (AsnOptFreeFunc) T3ReplyFree);
      if (isError && ptr -> reply == NULL) {
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
   ptr = Taxon3ReplyFree(ptr);
   goto ret;
}



/**************************************************
*
*    Taxon3ReplyAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Taxon3ReplyAsnWrite(Taxon3ReplyPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TAXON3_REPLY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericChoiceSeqOfAsnWrite(ptr -> reply, (AsnWriteFunc) T3ReplyAsnWrite, aip, TAXON3_REPLY_reply, TAXON3_REPLY_reply_E);
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
*    T3ReplyFree()
*
**************************************************/
NLM_EXTERN 
T3ReplyPtr LIBCALL
T3ReplyFree(ValNodePtr anp)
{
   Pointer pnt;

   if (anp == NULL) {
      return NULL;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   default:
      break;
   case T3Reply_error:
      T3ErrorFree(anp -> data.ptrvalue);
      break;
   case T3Reply_data:
      T3DataFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    T3ReplyAsnRead()
*
**************************************************/
NLM_EXTERN 
T3ReplyPtr LIBCALL
T3ReplyAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   Boolean nullIsError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* T3Reply ::= (self contained) */
      atp = AsnReadId(aip, amp, T3REPLY);
   } else {
      atp = AsnLinkType(orig, T3REPLY);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp = ValNodeNew(NULL);
   if (anp == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */
      goto erret;
   }

   func = NULL;

   atp = AsnReadId(aip, amp, atp);  /* find the choice */
   if (atp == NULL) {
      goto erret;
   }
   if (atp == T3REPLY_error) {
      choice = T3Reply_error;
      func = (AsnReadFunc) T3ErrorAsnRead;
   }
   else if (atp == T3REPLY_data) {
      choice = T3Reply_data;
      func = (AsnReadFunc) T3DataAsnRead;
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (aip -> io_failure) goto erret;

      if (nullIsError && anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    T3ReplyAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
T3ReplyAsnWrite(T3ReplyPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, T3REPLY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   case T3Reply_error:
      writetype = T3REPLY_error;
      func = (AsnWriteFunc) T3ErrorAsnWrite;
      break;
   case T3Reply_data:
      writetype = T3REPLY_data;
      func = (AsnWriteFunc) T3DataAsnWrite;
      break;
   }
   if (writetype != NULL) {
      retval = (* func)(pnt, aip, writetype);   /* write it out */
   }
   if (!retval) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    T3ErrorNew()
*
**************************************************/
NLM_EXTERN 
T3ErrorPtr LIBCALL
T3ErrorNew(void)
{
   T3ErrorPtr ptr = MemNew((size_t) sizeof(T3Error));

   return ptr;

}


/**************************************************
*
*    T3ErrorFree()
*
**************************************************/
NLM_EXTERN 
T3ErrorPtr LIBCALL
T3ErrorFree(T3ErrorPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> message);
   MemFree(ptr -> name);
   OrgRefFree(ptr -> org);
   return MemFree(ptr);
}


/**************************************************
*
*    T3ErrorAsnRead()
*
**************************************************/
NLM_EXTERN 
T3ErrorPtr LIBCALL
T3ErrorAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   T3ErrorPtr ptr;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* T3Error ::= (self contained) */
      atp = AsnReadId(aip, amp, T3ERROR);
   } else {
      atp = AsnLinkType(orig, T3ERROR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = T3ErrorNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == T3ERROR_level) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> level = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3ERROR_message) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> message = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3ERROR_taxid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> taxid = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3ERROR_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3ERROR_org) {
      ptr -> org = OrgRefAsnRead(aip, atp);
      if (aip -> io_failure) {
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
   ptr = T3ErrorFree(ptr);
   goto ret;
}



/**************************************************
*
*    T3ErrorAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
T3ErrorAsnWrite(T3ErrorPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, T3ERROR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> level;
   retval = AsnWrite(aip, T3ERROR_level,  &av);
   if (ptr -> message != NULL) {
      av.ptrvalue = ptr -> message;
      retval = AsnWrite(aip, T3ERROR_message,  &av);
   }
   av.intvalue = ptr -> taxid;
   retval = AsnWrite(aip, T3ERROR_taxid,  &av);
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, T3ERROR_name,  &av);
   }
   if (ptr -> org != NULL) {
      if ( ! OrgRefAsnWrite(ptr -> org, aip, T3ERROR_org)) {
         goto erret;
      }
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
*    T3DataNew()
*
**************************************************/
NLM_EXTERN 
T3DataPtr LIBCALL
T3DataNew(void)
{
   T3DataPtr ptr = MemNew((size_t) sizeof(T3Data));

   return ptr;

}


/**************************************************
*
*    T3DataFree()
*
**************************************************/
NLM_EXTERN 
T3DataPtr LIBCALL
T3DataFree(T3DataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   OrgRefFree(ptr -> org);
   AsnGenericBaseSeqOfFree(ptr -> blast_name_lineage ,ASNCODE_PTRVAL_SLOT);
   AsnGenericUserSeqOfFree(ptr -> status, (AsnOptFreeFunc) T3StatusFlagsFree);
   T3RefreshFlagsFree(ptr -> refresh);
   return MemFree(ptr);
}


/**************************************************
*
*    T3DataAsnRead()
*
**************************************************/
NLM_EXTERN 
T3DataPtr LIBCALL
T3DataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   T3DataPtr ptr;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* T3Data ::= (self contained) */
      atp = AsnReadId(aip, amp, T3DATA);
   } else {
      atp = AsnLinkType(orig, T3DATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = T3DataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == T3DATA_org) {
      ptr -> org = OrgRefAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3DATA_blast_name_lineage) {
      ptr -> blast_name_lineage = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> blast_name_lineage == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3DATA_status) {
      ptr -> status = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) T3StatusFlagsAsnRead, (AsnOptFreeFunc) T3StatusFlagsFree);
      if (isError && ptr -> status == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3DATA_refresh) {
      ptr -> refresh = T3RefreshFlagsAsnRead(aip, atp);
      if (aip -> io_failure) {
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
   ptr = T3DataFree(ptr);
   goto ret;
}



/**************************************************
*
*    T3DataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
T3DataAsnWrite(T3DataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, T3DATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> org != NULL) {
      if ( ! OrgRefAsnWrite(ptr -> org, aip, T3DATA_org)) {
         goto erret;
      }
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> blast_name_lineage ,ASNCODE_PTRVAL_SLOT, aip, T3DATA_blast_name_lineage, T3DATA_blast_name_lineage_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> status, (AsnWriteFunc) T3StatusFlagsAsnWrite, aip, T3DATA_status, T3DATA_status_E);
   if (ptr -> refresh != NULL) {
      if ( ! T3RefreshFlagsAsnWrite(ptr -> refresh, aip, T3DATA_refresh)) {
         goto erret;
      }
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
*    T3StatusFlagsNew()
*
**************************************************/
NLM_EXTERN 
T3StatusFlagsPtr LIBCALL
T3StatusFlagsNew(void)
{
   T3StatusFlagsPtr ptr = MemNew((size_t) sizeof(T3StatusFlags));

   return ptr;

}


/**************************************************
*
*    T3StatusFlagsFree()
*
**************************************************/
NLM_EXTERN 
T3StatusFlagsPtr LIBCALL
T3StatusFlagsFree(T3StatusFlagsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> property);
   Value_valueFree(ptr -> Value_value);
   return MemFree(ptr);
}


/**************************************************
*
*    Value_valueFree()
*
**************************************************/
static 
Value_valuePtr LIBCALL
Value_valueFree(ValNodePtr anp)
{
   Pointer pnt;

   if (anp == NULL) {
      return NULL;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   default:
      break;
   case Value_value_str:
      MemFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    T3StatusFlagsAsnRead()
*
**************************************************/
NLM_EXTERN 
T3StatusFlagsPtr LIBCALL
T3StatusFlagsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   T3StatusFlagsPtr ptr;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* T3StatusFlags ::= (self contained) */
      atp = AsnReadId(aip, amp, T3STATUSFLAGS);
   } else {
      atp = AsnLinkType(orig, T3STATUSFLAGS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = T3StatusFlagsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == T3STATUSFLAGS_property) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> property = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3STATUSFLAGS_value) {
      ptr -> Value_value = Value_valueAsnRead(aip, atp);
      if (aip -> io_failure) {
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
   ptr = T3StatusFlagsFree(ptr);
   goto ret;
}



/**************************************************
*
*    Value_valueAsnRead()
*
**************************************************/
static 
Value_valuePtr LIBCALL
Value_valueAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   Boolean nullIsError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Value_value ::= (self contained) */
      atp = AsnReadId(aip, amp, T3STATUSFLAGS_value);
   } else {
      atp = AsnLinkType(orig, T3STATUSFLAGS_value);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp = ValNodeNew(NULL);
   if (anp == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the CHOICE or OpenStruct value (nothing) */
      goto erret;
   }

   func = NULL;

   atp = AsnReadId(aip, amp, atp);  /* find the choice */
   if (atp == NULL) {
      goto erret;
   }
   if (atp == T3STATUSFLAGS_value_bool) {
      choice = Value_value_bool;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == T3STATUSFLAGS_value_int) {
      choice = Value_value_int__;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == T3STATUSFLAGS_value_str) {
      choice = Value_value_str;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (aip -> io_failure) goto erret;

      if (nullIsError && anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    T3StatusFlagsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
T3StatusFlagsAsnWrite(T3StatusFlagsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, T3STATUSFLAGS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> property != NULL) {
      av.ptrvalue = ptr -> property;
      retval = AsnWrite(aip, T3STATUSFLAGS_property,  &av);
   }
   if (ptr -> Value_value != NULL) {
      if ( ! Value_valueAsnWrite(ptr -> Value_value, aip, T3STATUSFLAGS_value)) {
         goto erret;
      }
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
*    Value_valueAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Value_valueAsnWrite(Value_valuePtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, T3STATUSFLAGS_value);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   switch (anp->choice)
   {
   case Value_value_bool:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, T3STATUSFLAGS_value_bool, &av);
      break;
   case Value_value_int__:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, T3STATUSFLAGS_value_int, &av);
      break;
   case Value_value_str:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, T3STATUSFLAGS_value_str, &av);
      break;
   }
   if (writetype != NULL) {
      retval = (* func)(pnt, aip, writetype);   /* write it out */
   }
   if (!retval) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    T3RefreshFlagsNew()
*
**************************************************/
NLM_EXTERN 
T3RefreshFlagsPtr LIBCALL
T3RefreshFlagsNew(void)
{
   T3RefreshFlagsPtr ptr = MemNew((size_t) sizeof(T3RefreshFlags));

   return ptr;

}


/**************************************************
*
*    T3RefreshFlagsFree()
*
**************************************************/
NLM_EXTERN 
T3RefreshFlagsPtr LIBCALL
T3RefreshFlagsFree(T3RefreshFlagsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    T3RefreshFlagsAsnRead()
*
**************************************************/
NLM_EXTERN 
T3RefreshFlagsPtr LIBCALL
T3RefreshFlagsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   T3RefreshFlagsPtr ptr;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* T3RefreshFlags ::= (self contained) */
      atp = AsnReadId(aip, amp, T3REFRESHFLAGS);
   } else {
      atp = AsnLinkType(orig, T3REFRESHFLAGS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = T3RefreshFlagsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == T3REFRESHFLAGS_taxid_changed) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> taxid_changed = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3REFRESHFLAGS_taxname_changed) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> taxname_changed = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3REFRESHFLAGS_division_changed) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> division_changed = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3REFRESHFLAGS_lineage_changed) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> lineage_changed = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3REFRESHFLAGS_gc_changed) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gc_changed = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3REFRESHFLAGS_mgc_changed) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> mgc_changed = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == T3REFRESHFLAGS_orgmod_changed) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> orgmod_changed = av.boolvalue;
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
   ptr = T3RefreshFlagsFree(ptr);
   goto ret;
}



/**************************************************
*
*    T3RefreshFlagsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
T3RefreshFlagsAsnWrite(T3RefreshFlagsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, T3REFRESHFLAGS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.boolvalue = ptr -> taxid_changed;
   retval = AsnWrite(aip, T3REFRESHFLAGS_taxid_changed,  &av);
   av.boolvalue = ptr -> taxname_changed;
   retval = AsnWrite(aip, T3REFRESHFLAGS_taxname_changed,  &av);
   av.boolvalue = ptr -> division_changed;
   retval = AsnWrite(aip, T3REFRESHFLAGS_division_changed,  &av);
   av.boolvalue = ptr -> lineage_changed;
   retval = AsnWrite(aip, T3REFRESHFLAGS_lineage_changed,  &av);
   av.boolvalue = ptr -> gc_changed;
   retval = AsnWrite(aip, T3REFRESHFLAGS_gc_changed,  &av);
   av.boolvalue = ptr -> mgc_changed;
   retval = AsnWrite(aip, T3REFRESHFLAGS_mgc_changed,  &av);
   av.boolvalue = ptr -> orgmod_changed;
   retval = AsnWrite(aip, T3REFRESHFLAGS_orgmod_changed,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

