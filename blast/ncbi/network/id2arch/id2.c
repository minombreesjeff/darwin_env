#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <id2map.h>
#include <id2gen.h>

static Boolean loaded = FALSE;

#include <id2.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
id2genAsnLoad(void)
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
*
**************************************************/


/**************************************************
*
*    ID2RequestPacketFree()
*
**************************************************/
NLM_EXTERN 
ID2RequestPacketPtr LIBCALL
ID2RequestPacketFree(ID2RequestPacketPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) ID2RequestFree);
   return NULL;
}


/**************************************************
*
*    ID2RequestPacketAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2RequestPacketPtr LIBCALL
ID2RequestPacketAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2RequestPacketPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2RequestPacket ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REQUEST_PACKET);
   } else {
      atp = AsnLinkType(orig, ID2_REQUEST_PACKET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2RequestAsnRead, (AsnOptFreeFunc) ID2RequestFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ID2RequestPacketFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2RequestPacketAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2RequestPacketAsnWrite(ID2RequestPacketPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REQUEST_PACKET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) ID2RequestAsnWrite, aip, atp, ID2_REQUEST_PACKET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ID2RequestNew()
*
**************************************************/
NLM_EXTERN 
ID2RequestPtr LIBCALL
ID2RequestNew(void)
{
   ID2RequestPtr ptr = MemNew((size_t) sizeof(ID2Request));

   return ptr;

}


/**************************************************
*
*    ID2RequestFree()
*
**************************************************/
NLM_EXTERN 
ID2RequestPtr LIBCALL
ID2RequestFree(ID2RequestPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2ParamsFree(ptr -> params);
   Request_requestFree(ptr -> Request_request);
   return MemFree(ptr);
}


/**************************************************
*
*    Request_requestFree()
*
**************************************************/
static 
Request_requestPtr LIBCALL
Request_requestFree(ValNodePtr anp)
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
   case Request_request_get_packages:
      ID2RequestGetPackagesFree(anp -> data.ptrvalue);
      break;
   case Request_request_get_seq_id:
      ID2RequestGetSeqIdFree(anp -> data.ptrvalue);
      break;
   case Request_request_get_blob_id:
      ID2RequestGetBlobIdFree(anp -> data.ptrvalue);
      break;
   case Request_request_get_blob_info:
      ID2RequestGetBlobInfoFree(anp -> data.ptrvalue);
      break;
   case Request_request_reget_blob:
      ID2RequestReGetBlobFree(anp -> data.ptrvalue);
      break;
   case Request_request_get_chunks:
      ID2SRequestGetChunksFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ID2RequestAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2RequestPtr LIBCALL
ID2RequestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2RequestPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2Request ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REQUEST);
   } else {
      atp = AsnLinkType(orig, ID2_REQUEST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2RequestNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REQUEST_serial_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> serial_number = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REQUEST_params) {
      ptr -> params = ID2ParamsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REQUEST_request) {
      ptr -> Request_request = Request_requestAsnRead(aip, atp);
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
   ptr = ID2RequestFree(ptr);
   goto ret;
}



/**************************************************
*
*    Request_requestAsnRead()
*
**************************************************/
static 
Request_requestPtr LIBCALL
Request_requestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Request_request ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REQUEST_request);
   } else {
      atp = AsnLinkType(orig, ID2_REQUEST_request);    /* link in local tree */
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
   if (atp == ID2_REQUEST_request_init) {
      choice = Request_request_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == REQUEST_request_get_packages) {
      choice = Request_request_get_packages;
      func = (AsnReadFunc) ID2RequestGetPackagesAsnRead;
   }
   else if (atp == ID2_REQUEST_request_get_seq_id) {
      choice = Request_request_get_seq_id;
      func = (AsnReadFunc) ID2RequestGetSeqIdAsnRead;
   }
   else if (atp == ID2_REQUEST_request_get_blob_id) {
      choice = Request_request_get_blob_id;
      func = (AsnReadFunc) ID2RequestGetBlobIdAsnRead;
   }
   else if (atp == REQUEST_request_get_blob_info) {
      choice = Request_request_get_blob_info;
      func = (AsnReadFunc) ID2RequestGetBlobInfoAsnRead;
   }
   else if (atp == ID2_REQUEST_request_reget_blob) {
      choice = Request_request_reget_blob;
      func = (AsnReadFunc) ID2RequestReGetBlobAsnRead;
   }
   else if (atp == ID2_REQUEST_request_get_chunks) {
      choice = Request_request_get_chunks;
      func = (AsnReadFunc) ID2SRequestGetChunksAsnRead;
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
*    ID2RequestAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2RequestAsnWrite(ID2RequestPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REQUEST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> serial_number;
   retval = AsnWrite(aip, ID2_REQUEST_serial_number,  &av);
   if (ptr -> params != NULL) {
      if ( ! ID2ParamsAsnWrite(ptr -> params, aip, ID2_REQUEST_params)) {
         goto erret;
      }
   }
   if (ptr -> Request_request != NULL) {
      if ( ! Request_requestAsnWrite(ptr -> Request_request, aip, ID2_REQUEST_request)) {
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
*    Request_requestAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Request_requestAsnWrite(Request_requestPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ID2_REQUEST_request);   /* link local tree */
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
   case Request_request_init:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, ID2_REQUEST_request_init, &av);
      break;
   case Request_request_get_packages:
      writetype = REQUEST_request_get_packages;
      func = (AsnWriteFunc) ID2RequestGetPackagesAsnWrite;
      break;
   case Request_request_get_seq_id:
      writetype = ID2_REQUEST_request_get_seq_id;
      func = (AsnWriteFunc) ID2RequestGetSeqIdAsnWrite;
      break;
   case Request_request_get_blob_id:
      writetype = ID2_REQUEST_request_get_blob_id;
      func = (AsnWriteFunc) ID2RequestGetBlobIdAsnWrite;
      break;
   case Request_request_get_blob_info:
      writetype = REQUEST_request_get_blob_info;
      func = (AsnWriteFunc) ID2RequestGetBlobInfoAsnWrite;
      break;
   case Request_request_reget_blob:
      writetype = ID2_REQUEST_request_reget_blob;
      func = (AsnWriteFunc) ID2RequestReGetBlobAsnWrite;
      break;
   case Request_request_get_chunks:
      writetype = ID2_REQUEST_request_get_chunks;
      func = (AsnWriteFunc) ID2SRequestGetChunksAsnWrite;
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
*    ID2ParamsFree()
*
**************************************************/
NLM_EXTERN 
ID2ParamsPtr LIBCALL
ID2ParamsFree(ID2ParamsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) ID2ParamFree);
   return NULL;
}


/**************************************************
*
*    ID2ParamsAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2ParamsPtr LIBCALL
ID2ParamsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2ParamsPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2Params ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_PARAMS);
   } else {
      atp = AsnLinkType(orig, ID2_PARAMS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2ParamAsnRead, (AsnOptFreeFunc) ID2ParamFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ID2ParamsFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2ParamsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2ParamsAsnWrite(ID2ParamsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_PARAMS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) ID2ParamAsnWrite, aip, atp, ID2_PARAMS_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ID2RequestGetPackagesNew()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetPackagesPtr LIBCALL
ID2RequestGetPackagesNew(void)
{
   ID2RequestGetPackagesPtr ptr = MemNew((size_t) sizeof(ID2RequestGetPackages));

   return ptr;

}


/**************************************************
*
*    ID2RequestGetPackagesFree()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetPackagesPtr LIBCALL
ID2RequestGetPackagesFree(ID2RequestGetPackagesPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> names ,ASNCODE_PTRVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2RequestGetPackagesAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetPackagesPtr LIBCALL
ID2RequestGetPackagesAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2RequestGetPackagesPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2RequestGetPackages ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REQUEST_GET_PACKAGES);
   } else {
      atp = AsnLinkType(orig, ID2_REQUEST_GET_PACKAGES);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2RequestGetPackagesNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REQUEST_GET_PACKAGES_names) {
      ptr -> names = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> names == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GET_PACKAGES_no_contents) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> no_contents = av.boolvalue;
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
   ptr = ID2RequestGetPackagesFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2RequestGetPackagesAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2RequestGetPackagesAsnWrite(ID2RequestGetPackagesPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REQUEST_GET_PACKAGES);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> names ,ASNCODE_PTRVAL_SLOT, aip, ID2_REQUEST_GET_PACKAGES_names, REQUEST_GET_PACKAGES_names_E);
   av.boolvalue = ptr -> no_contents;
   retval = AsnWrite(aip, GET_PACKAGES_no_contents,  &av);
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
*    ID2RequestGetSeqIdNew()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetSeqIdPtr LIBCALL
ID2RequestGetSeqIdNew(void)
{
   ID2RequestGetSeqIdPtr ptr = MemNew((size_t) sizeof(ID2RequestGetSeqId));

   ptr -> seq_id_type = 0;
   return ptr;

}


/**************************************************
*
*    ID2RequestGetSeqIdFree()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetSeqIdPtr LIBCALL
ID2RequestGetSeqIdFree(ID2RequestGetSeqIdPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2SeqIdFree(ptr -> seq_id);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2RequestGetSeqIdAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetSeqIdPtr LIBCALL
ID2RequestGetSeqIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2RequestGetSeqIdPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2RequestGetSeqId ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REQUEST_GET_SEQ_ID);
   } else {
      atp = AsnLinkType(orig, ID2_REQUEST_GET_SEQ_ID);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2RequestGetSeqIdNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REQUEST_GET_SEQ_ID_seq_id) {
      ptr -> seq_id = ID2SeqIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REQUEST_GET_SEQ_ID_seq_id_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> seq_id_type = av.intvalue;
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
   ptr = ID2RequestGetSeqIdFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2RequestGetSeqIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2RequestGetSeqIdAsnWrite(ID2RequestGetSeqIdPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REQUEST_GET_SEQ_ID);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> seq_id != NULL) {
      if ( ! ID2SeqIdAsnWrite(ptr -> seq_id, aip, ID2_REQUEST_GET_SEQ_ID_seq_id)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> seq_id_type;
   retval = AsnWrite(aip, REQUEST_GET_SEQ_ID_seq_id_type,  &av);
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
*    ID2RequestGetBlobIdNew()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetBlobIdPtr LIBCALL
ID2RequestGetBlobIdNew(void)
{
   ID2RequestGetBlobIdPtr ptr = MemNew((size_t) sizeof(ID2RequestGetBlobId));

   return ptr;

}


/**************************************************
*
*    ID2RequestGetBlobIdFree()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetBlobIdPtr LIBCALL
ID2RequestGetBlobIdFree(ID2RequestGetBlobIdPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2RequestGetSeqIdFree(ptr -> seq_id);
   AsnGenericBaseSeqOfFree(ptr -> sources ,ASNCODE_PTRVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2RequestGetBlobIdAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetBlobIdPtr LIBCALL
ID2RequestGetBlobIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2RequestGetBlobIdPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2RequestGetBlobId ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REQUEST_GET_BLOB_ID);
   } else {
      atp = AsnLinkType(orig, ID2_REQUEST_GET_BLOB_ID);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2RequestGetBlobIdNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REQUEST_GET_BLOB_ID_seq_id) {
      ptr -> seq_id = ID2RequestGetSeqIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REQUEST_GET_BLOB_ID_sources) {
      ptr -> sources = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> sources == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REQUEST_GET_BLOB_ID_external) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> external = av.boolvalue;
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
   ptr = ID2RequestGetBlobIdFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2RequestGetBlobIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2RequestGetBlobIdAsnWrite(ID2RequestGetBlobIdPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REQUEST_GET_BLOB_ID);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> seq_id != NULL) {
      if ( ! ID2RequestGetSeqIdAsnWrite(ptr -> seq_id, aip, ID2_REQUEST_GET_BLOB_ID_seq_id)) {
         goto erret;
      }
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> sources ,ASNCODE_PTRVAL_SLOT, aip, ID2_REQUEST_GET_BLOB_ID_sources, REQUEST_GET_BLOB_ID_sources_E);
   av.boolvalue = ptr -> external;
   retval = AsnWrite(aip, REQUEST_GET_BLOB_ID_external,  &av);
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
*    ID2RequestGetBlobInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetBlobInfoPtr LIBCALL
ID2RequestGetBlobInfoNew(void)
{
   ID2RequestGetBlobInfoPtr ptr = MemNew((size_t) sizeof(ID2RequestGetBlobInfo));

   return ptr;

}


/**************************************************
*
*    BlobId_resolveNew()
*
**************************************************/
static 
BlobId_resolvePtr LIBCALL
BlobId_resolveNew(void)
{
   BlobId_resolvePtr ptr = MemNew((size_t) sizeof(BlobId_resolve));

   return ptr;

}


/**************************************************
*
*    ID2RequestGetBlobInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetBlobInfoPtr LIBCALL
ID2RequestGetBlobInfoFree(ID2RequestGetBlobInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   BlobId_blob_idFree(ptr -> BlobId_blob_id);
   ID2GetBlobDetailsFree(ptr -> get_data);
   return MemFree(ptr);
}


/**************************************************
*
*    BlobId_blob_idFree()
*
**************************************************/
static 
BlobId_blob_idPtr LIBCALL
BlobId_blob_idFree(ValNodePtr anp)
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
   case BlobId_blob_id_blob_id:
      ID2BlobIdFree(anp -> data.ptrvalue);
      break;
   case BlobId_blob_id_BlobId_Resolve:
      BlobId_resolveFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    BlobId_resolveFree()
*
**************************************************/
static 
BlobId_resolvePtr LIBCALL
BlobId_resolveFree(BlobId_resolvePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2RequestGetBlobIdFree(ptr -> request);
   AsnGenericUserSeqOfFree(ptr -> exclude_blobs, (AsnOptFreeFunc) ID2BlobIdFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2RequestGetBlobInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2RequestGetBlobInfoPtr LIBCALL
ID2RequestGetBlobInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2RequestGetBlobInfoPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2RequestGetBlobInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REQUEST_GET_BLOB_INFO);
   } else {
      atp = AsnLinkType(orig, ID2_REQUEST_GET_BLOB_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2RequestGetBlobInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == REQUEST_GET_BLOB_INFO_blob_id) {
      ptr -> BlobId_blob_id = BlobId_blob_idAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GET_BLOB_INFO_get_seq_ids) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> get_seq_ids = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REQUEST_GET_BLOB_INFO_get_data) {
      ptr -> get_data = ID2GetBlobDetailsAsnRead(aip, atp);
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
   ptr = ID2RequestGetBlobInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlobId_blob_idAsnRead()
*
**************************************************/
static 
BlobId_blob_idPtr LIBCALL
BlobId_blob_idAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlobId_blob_id ::= (self contained) */
      atp = AsnReadId(aip, amp, REQUEST_GET_BLOB_INFO_blob_id);
   } else {
      atp = AsnLinkType(orig, REQUEST_GET_BLOB_INFO_blob_id);    /* link in local tree */
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
   if (atp == GET_BLOB_INFO_blob_id_blob_id) {
      choice = BlobId_blob_id_blob_id;
      func = (AsnReadFunc) ID2BlobIdAsnRead;
   }
   else if (atp == GET_BLOB_INFO_blob_id_resolve) {
      choice = BlobId_blob_id_BlobId_Resolve;
      func = (AsnReadFunc) BlobId_resolveAsnRead;
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
*    BlobId_resolveAsnRead()
*
**************************************************/
static 
BlobId_resolvePtr LIBCALL
BlobId_resolveAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlobId_resolvePtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlobId_resolve ::= (self contained) */
      atp = AsnReadId(aip, amp, GET_BLOB_INFO_blob_id_resolve);
   } else {
      atp = AsnLinkType(orig, GET_BLOB_INFO_blob_id_resolve);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlobId_resolveNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == INFO_blob_id_resolve_request) {
      ptr -> request = ID2RequestGetBlobIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == blob_id_resolve_exclude_blobs) {
      ptr -> exclude_blobs = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2BlobIdAsnRead, (AsnOptFreeFunc) ID2BlobIdFree);
      if (isError && ptr -> exclude_blobs == NULL) {
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
   ptr = BlobId_resolveFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2RequestGetBlobInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2RequestGetBlobInfoAsnWrite(ID2RequestGetBlobInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REQUEST_GET_BLOB_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> BlobId_blob_id != NULL) {
      if ( ! BlobId_blob_idAsnWrite(ptr -> BlobId_blob_id, aip, REQUEST_GET_BLOB_INFO_blob_id)) {
         goto erret;
      }
   }
   av.boolvalue = ptr -> get_seq_ids;
   retval = AsnWrite(aip, GET_BLOB_INFO_get_seq_ids,  &av);
   if (ptr -> get_data != NULL) {
      if ( ! ID2GetBlobDetailsAsnWrite(ptr -> get_data, aip, REQUEST_GET_BLOB_INFO_get_data)) {
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
*    BlobId_blob_idAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
BlobId_blob_idAsnWrite(BlobId_blob_idPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, REQUEST_GET_BLOB_INFO_blob_id);   /* link local tree */
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
   case BlobId_blob_id_blob_id:
      writetype = GET_BLOB_INFO_blob_id_blob_id;
      func = (AsnWriteFunc) ID2BlobIdAsnWrite;
      break;
   case BlobId_blob_id_BlobId_Resolve:
      writetype = GET_BLOB_INFO_blob_id_resolve;
      func = (AsnWriteFunc) BlobId_resolveAsnWrite;
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
*    BlobId_resolveAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
BlobId_resolveAsnWrite(BlobId_resolvePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GET_BLOB_INFO_blob_id_resolve);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> request != NULL) {
      if ( ! ID2RequestGetBlobIdAsnWrite(ptr -> request, aip, INFO_blob_id_resolve_request)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> exclude_blobs, (AsnWriteFunc) ID2BlobIdAsnWrite, aip, blob_id_resolve_exclude_blobs, blob_id_resolve_exclude_blobs_E);
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
*    ID2RequestReGetBlobNew()
*
**************************************************/
NLM_EXTERN 
ID2RequestReGetBlobPtr LIBCALL
ID2RequestReGetBlobNew(void)
{
   ID2RequestReGetBlobPtr ptr = MemNew((size_t) sizeof(ID2RequestReGetBlob));

   return ptr;

}


/**************************************************
*
*    ID2RequestReGetBlobFree()
*
**************************************************/
NLM_EXTERN 
ID2RequestReGetBlobPtr LIBCALL
ID2RequestReGetBlobFree(ID2RequestReGetBlobPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2BlobIdFree(ptr -> blob_id);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2RequestReGetBlobAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2RequestReGetBlobPtr LIBCALL
ID2RequestReGetBlobAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2RequestReGetBlobPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2RequestReGetBlob ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REQUEST_REGET_BLOB);
   } else {
      atp = AsnLinkType(orig, ID2_REQUEST_REGET_BLOB);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2RequestReGetBlobNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REQUEST_REGET_BLOB_blob_id) {
      ptr -> blob_id = ID2BlobIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REGET_BLOB_split_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> split_version = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REQUEST_REGET_BLOB_offset) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> offset = av.intvalue;
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
   ptr = ID2RequestReGetBlobFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2RequestReGetBlobAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2RequestReGetBlobAsnWrite(ID2RequestReGetBlobPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REQUEST_REGET_BLOB);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> blob_id != NULL) {
      if ( ! ID2BlobIdAsnWrite(ptr -> blob_id, aip, ID2_REQUEST_REGET_BLOB_blob_id)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> split_version;
   retval = AsnWrite(aip, REGET_BLOB_split_version,  &av);
   av.intvalue = ptr -> offset;
   retval = AsnWrite(aip, ID2_REQUEST_REGET_BLOB_offset,  &av);
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
*    ID2SRequestGetChunksNew()
*
**************************************************/
NLM_EXTERN 
ID2SRequestGetChunksPtr LIBCALL
ID2SRequestGetChunksNew(void)
{
   ID2SRequestGetChunksPtr ptr = MemNew((size_t) sizeof(ID2SRequestGetChunks));

   return ptr;

}


/**************************************************
*
*    ID2SRequestGetChunksFree()
*
**************************************************/
NLM_EXTERN 
ID2SRequestGetChunksPtr LIBCALL
ID2SRequestGetChunksFree(ID2SRequestGetChunksPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2BlobIdFree(ptr -> blob_id);
   AsnGenericBaseSeqOfFree(ptr -> chunks ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SRequestGetChunksAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SRequestGetChunksPtr LIBCALL
ID2SRequestGetChunksAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SRequestGetChunksPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SRequestGetChunks ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_REQUEST_GET_CHUNKS);
   } else {
      atp = AsnLinkType(orig, ID2S_REQUEST_GET_CHUNKS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SRequestGetChunksNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_REQUEST_GET_CHUNKS_blob_id) {
      ptr -> blob_id = ID2BlobIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_REQUEST_GET_CHUNKS_chunks) {
      ptr -> chunks = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> chunks == NULL) {
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
   ptr = ID2SRequestGetChunksFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SRequestGetChunksAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SRequestGetChunksAsnWrite(ID2SRequestGetChunksPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_REQUEST_GET_CHUNKS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> blob_id != NULL) {
      if ( ! ID2BlobIdAsnWrite(ptr -> blob_id, aip, ID2S_REQUEST_GET_CHUNKS_blob_id)) {
         goto erret;
      }
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> chunks ,ASNCODE_INTVAL_SLOT, aip, ID2S_REQUEST_GET_CHUNKS_chunks, REQUEST_GET_CHUNKS_chunks_E);
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
*    ID2SeqIdFree()
*
**************************************************/
NLM_EXTERN 
ID2SeqIdPtr LIBCALL
ID2SeqIdFree(ValNodePtr anp)
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
   case ID2SeqId_string:
      MemFree(anp -> data.ptrvalue);
      break;
   case ID2SeqId_seq_id:
      SeqIdFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ID2SeqIdAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SeqIdPtr LIBCALL
ID2SeqIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SeqId ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_SEQ_ID);
   } else {
      atp = AsnLinkType(orig, ID2_SEQ_ID);    /* link in local tree */
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
   if (atp == ID2_SEQ_ID_string) {
      choice = ID2SeqId_string;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == ID2_SEQ_ID_seq_id) {
      choice = ID2SeqId_seq_id;
      func = (AsnReadFunc) SeqIdAsnRead;
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
*    ID2SeqIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SeqIdAsnWrite(ID2SeqIdPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ID2_SEQ_ID);   /* link local tree */
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
   case ID2SeqId_string:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, ID2_SEQ_ID_string, &av);
      break;
   case ID2SeqId_seq_id:
      writetype = ID2_SEQ_ID_seq_id;
      func = (AsnWriteFunc) SeqIdAsnWrite;
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
*    ID2BlobIdNew()
*
**************************************************/
NLM_EXTERN 
ID2BlobIdPtr LIBCALL
ID2BlobIdNew(void)
{
   ID2BlobIdPtr ptr = MemNew((size_t) sizeof(ID2BlobId));

   ptr -> sub_sat = 0;
   return ptr;

}


/**************************************************
*
*    ID2BlobIdFree()
*
**************************************************/
NLM_EXTERN 
ID2BlobIdPtr LIBCALL
ID2BlobIdFree(ID2BlobIdPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ID2BlobIdAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2BlobIdPtr LIBCALL
ID2BlobIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2BlobIdPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2BlobId ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_BLOB_ID);
   } else {
      atp = AsnLinkType(orig, ID2_BLOB_ID);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2BlobIdNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_BLOB_ID_sat) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sat = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_BLOB_ID_sub_sat) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sub_sat = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_BLOB_ID_sat_key) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sat_key = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_BLOB_ID_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> version = av.intvalue;
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
   ptr = ID2BlobIdFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2BlobIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2BlobIdAsnWrite(ID2BlobIdPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_BLOB_ID);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> sat;
   retval = AsnWrite(aip, ID2_BLOB_ID_sat,  &av);
   av.intvalue = ptr -> sub_sat;
   retval = AsnWrite(aip, ID2_BLOB_ID_sub_sat,  &av);
   av.intvalue = ptr -> sat_key;
   retval = AsnWrite(aip, ID2_BLOB_ID_sat_key,  &av);
   av.intvalue = ptr -> version;
   retval = AsnWrite(aip, ID2_BLOB_ID_version,  &av);
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
*    ID2GetBlobDetailsNew()
*
**************************************************/
NLM_EXTERN 
ID2GetBlobDetailsPtr LIBCALL
ID2GetBlobDetailsNew(void)
{
   ID2GetBlobDetailsPtr ptr = MemNew((size_t) sizeof(ID2GetBlobDetails));

   ptr -> seq_class_level = 1;
   ptr -> descr_level = 1;
   ptr -> descr_type_mask = 0;
   ptr -> annot_type_mask = 0;
   ptr -> feat_type_mask = 0;
   ptr -> sequence_level = 0;
   return ptr;

}


/**************************************************
*
*    ID2GetBlobDetailsFree()
*
**************************************************/
NLM_EXTERN 
ID2GetBlobDetailsPtr LIBCALL
ID2GetBlobDetailsFree(ID2GetBlobDetailsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SeqLocFree(ptr -> location);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2GetBlobDetailsAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2GetBlobDetailsPtr LIBCALL
ID2GetBlobDetailsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2GetBlobDetailsPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2GetBlobDetails ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_GET_BLOB_DETAILS);
   } else {
      atp = AsnLinkType(orig, ID2_GET_BLOB_DETAILS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2GetBlobDetailsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_GET_BLOB_DETAILS_location) {
      ptr -> location = SeqLocAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLOB_DETAILS_seq_class_level) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> seq_class_level = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GET_BLOB_DETAILS_descr_level) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> descr_level = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLOB_DETAILS_descr_type_mask) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> descr_type_mask = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLOB_DETAILS_annot_type_mask) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> annot_type_mask = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLOB_DETAILS_feat_type_mask) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> feat_type_mask = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLOB_DETAILS_sequence_level) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sequence_level = av.intvalue;
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
   ptr = ID2GetBlobDetailsFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2GetBlobDetailsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2GetBlobDetailsAsnWrite(ID2GetBlobDetailsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_GET_BLOB_DETAILS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> location != NULL) {
      if ( ! SeqLocAsnWrite(ptr -> location, aip, ID2_GET_BLOB_DETAILS_location)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> seq_class_level;
   retval = AsnWrite(aip, BLOB_DETAILS_seq_class_level,  &av);
   av.intvalue = ptr -> descr_level;
   retval = AsnWrite(aip, GET_BLOB_DETAILS_descr_level,  &av);
   av.intvalue = ptr -> descr_type_mask;
   retval = AsnWrite(aip, BLOB_DETAILS_descr_type_mask,  &av);
   av.intvalue = ptr -> annot_type_mask;
   retval = AsnWrite(aip, BLOB_DETAILS_annot_type_mask,  &av);
   av.intvalue = ptr -> feat_type_mask;
   retval = AsnWrite(aip, BLOB_DETAILS_feat_type_mask,  &av);
   av.intvalue = ptr -> sequence_level;
   retval = AsnWrite(aip, BLOB_DETAILS_sequence_level,  &av);
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
*    ID2ReplyNew()
*
**************************************************/
NLM_EXTERN 
ID2ReplyPtr LIBCALL
ID2ReplyNew(void)
{
   ID2ReplyPtr ptr = MemNew((size_t) sizeof(ID2Reply));

   return ptr;

}


/**************************************************
*
*    ID2ReplyFree()
*
**************************************************/
NLM_EXTERN 
ID2ReplyPtr LIBCALL
ID2ReplyFree(ID2ReplyPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2ParamsFree(ptr -> params);
   AsnGenericUserSeqOfFree(ptr -> error, (AsnOptFreeFunc) ID2ErrorFree);
   Reply_replyFree(ptr -> Reply_reply);
   return MemFree(ptr);
}


/**************************************************
*
*    Reply_replyFree()
*
**************************************************/
static 
Reply_replyPtr LIBCALL
Reply_replyFree(ValNodePtr anp)
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
   case Reply_reply_get_package:
      ID2ReplyGetPackageFree(anp -> data.ptrvalue);
      break;
   case Reply_reply_get_seq_id:
      ID2ReplyGetSeqIdFree(anp -> data.ptrvalue);
      break;
   case Reply_reply_get_blob_id:
      ID2ReplyGetBlobIdFree(anp -> data.ptrvalue);
      break;
   case Reply_reply_get_blob_seq_ids:
      ID2ReplyGetBlobSeqIdsFree(anp -> data.ptrvalue);
      break;
   case Reply_reply_get_blob:
      ID2ReplyGetBlobFree(anp -> data.ptrvalue);
      break;
   case Reply_reply_reget_blob:
      ID2ReplyReGetBlobFree(anp -> data.ptrvalue);
      break;
   case Reply_reply_get_split_info:
      ID2SReplyGetSplitInfoFree(anp -> data.ptrvalue);
      break;
   case Reply_reply_get_chunk:
      ID2SReplyGetChunkFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ID2ReplyAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2ReplyPtr LIBCALL
ID2ReplyAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2ReplyPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2Reply ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REPLY);
   } else {
      atp = AsnLinkType(orig, ID2_REPLY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2ReplyNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REPLY_serial_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> serial_number = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_params) {
      ptr -> params = ID2ParamsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_error) {
      ptr -> error = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2ErrorAsnRead, (AsnOptFreeFunc) ID2ErrorFree);
      if (isError && ptr -> error == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_end_of_reply) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> end_of_reply = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_reply) {
      ptr -> Reply_reply = Reply_replyAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_discard) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> discard = av.intvalue;
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
   ptr = ID2ReplyFree(ptr);
   goto ret;
}



/**************************************************
*
*    Reply_replyAsnRead()
*
**************************************************/
static 
Reply_replyPtr LIBCALL
Reply_replyAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Reply_reply ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REPLY_reply);
   } else {
      atp = AsnLinkType(orig, ID2_REPLY_reply);    /* link in local tree */
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
   if (atp == ID2_REPLY_reply_init) {
      choice = Reply_reply_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == ID2_REPLY_reply_empty) {
      choice = Reply_reply_empty;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == ID2_REPLY_reply_get_package) {
      choice = Reply_reply_get_package;
      func = (AsnReadFunc) ID2ReplyGetPackageAsnRead;
   }
   else if (atp == ID2_REPLY_reply_get_seq_id) {
      choice = Reply_reply_get_seq_id;
      func = (AsnReadFunc) ID2ReplyGetSeqIdAsnRead;
   }
   else if (atp == ID2_REPLY_reply_get_blob_id) {
      choice = Reply_reply_get_blob_id;
      func = (AsnReadFunc) ID2ReplyGetBlobIdAsnRead;
   }
   else if (atp == REPLY_reply_get_blob_seq_ids) {
      choice = Reply_reply_get_blob_seq_ids;
      func = (AsnReadFunc) ID2ReplyGetBlobSeqIdsAsnRead;
   }
   else if (atp == ID2_REPLY_reply_get_blob) {
      choice = Reply_reply_get_blob;
      func = (AsnReadFunc) ID2ReplyGetBlobAsnRead;
   }
   else if (atp == ID2_REPLY_reply_reget_blob) {
      choice = Reply_reply_reget_blob;
      func = (AsnReadFunc) ID2ReplyReGetBlobAsnRead;
   }
   else if (atp == ID2_REPLY_reply_get_split_info) {
      choice = Reply_reply_get_split_info;
      func = (AsnReadFunc) ID2SReplyGetSplitInfoAsnRead;
   }
   else if (atp == ID2_REPLY_reply_get_chunk) {
      choice = Reply_reply_get_chunk;
      func = (AsnReadFunc) ID2SReplyGetChunkAsnRead;
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
*    ID2ReplyAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2ReplyAsnWrite(ID2ReplyPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REPLY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> serial_number;
   retval = AsnWrite(aip, ID2_REPLY_serial_number,  &av);
   if (ptr -> params != NULL) {
      if ( ! ID2ParamsAsnWrite(ptr -> params, aip, ID2_REPLY_params)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> error, (AsnWriteFunc) ID2ErrorAsnWrite, aip, ID2_REPLY_error, ID2_REPLY_error_E);
   av.boolvalue = ptr -> end_of_reply;
   retval = AsnWrite(aip, ID2_REPLY_end_of_reply,  &av);
   if (ptr -> Reply_reply != NULL) {
      if ( ! Reply_replyAsnWrite(ptr -> Reply_reply, aip, ID2_REPLY_reply)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> discard;
   retval = AsnWrite(aip, ID2_REPLY_discard,  &av);
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
*    Reply_replyAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Reply_replyAsnWrite(Reply_replyPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ID2_REPLY_reply);   /* link local tree */
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
   case Reply_reply_init:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, ID2_REPLY_reply_init, &av);
      break;
   case Reply_reply_empty:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, ID2_REPLY_reply_empty, &av);
      break;
   case Reply_reply_get_package:
      writetype = ID2_REPLY_reply_get_package;
      func = (AsnWriteFunc) ID2ReplyGetPackageAsnWrite;
      break;
   case Reply_reply_get_seq_id:
      writetype = ID2_REPLY_reply_get_seq_id;
      func = (AsnWriteFunc) ID2ReplyGetSeqIdAsnWrite;
      break;
   case Reply_reply_get_blob_id:
      writetype = ID2_REPLY_reply_get_blob_id;
      func = (AsnWriteFunc) ID2ReplyGetBlobIdAsnWrite;
      break;
   case Reply_reply_get_blob_seq_ids:
      writetype = REPLY_reply_get_blob_seq_ids;
      func = (AsnWriteFunc) ID2ReplyGetBlobSeqIdsAsnWrite;
      break;
   case Reply_reply_get_blob:
      writetype = ID2_REPLY_reply_get_blob;
      func = (AsnWriteFunc) ID2ReplyGetBlobAsnWrite;
      break;
   case Reply_reply_reget_blob:
      writetype = ID2_REPLY_reply_reget_blob;
      func = (AsnWriteFunc) ID2ReplyReGetBlobAsnWrite;
      break;
   case Reply_reply_get_split_info:
      writetype = ID2_REPLY_reply_get_split_info;
      func = (AsnWriteFunc) ID2SReplyGetSplitInfoAsnWrite;
      break;
   case Reply_reply_get_chunk:
      writetype = ID2_REPLY_reply_get_chunk;
      func = (AsnWriteFunc) ID2SReplyGetChunkAsnWrite;
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
*    ID2ErrorNew()
*
**************************************************/
NLM_EXTERN 
ID2ErrorPtr LIBCALL
ID2ErrorNew(void)
{
   ID2ErrorPtr ptr = MemNew((size_t) sizeof(ID2Error));

   return ptr;

}


/**************************************************
*
*    ID2ErrorFree()
*
**************************************************/
NLM_EXTERN 
ID2ErrorPtr LIBCALL
ID2ErrorFree(ID2ErrorPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> message);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2ErrorAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2ErrorPtr LIBCALL
ID2ErrorAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2ErrorPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2Error ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_ERROR);
   } else {
      atp = AsnLinkType(orig, ID2_ERROR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2ErrorNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_ERROR_severity) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> severity = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_ERROR_retry_delay) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> retry_delay = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_ERROR_message) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> message = av.ptrvalue;
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
   ptr = ID2ErrorFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2ErrorAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2ErrorAsnWrite(ID2ErrorPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_ERROR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> severity;
   retval = AsnWrite(aip, ID2_ERROR_severity,  &av);
   av.intvalue = ptr -> retry_delay;
   retval = AsnWrite(aip, ID2_ERROR_retry_delay,  &av);
   if (ptr -> message != NULL) {
      av.ptrvalue = ptr -> message;
      retval = AsnWrite(aip, ID2_ERROR_message,  &av);
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
*    ID2ReplyGetPackageNew()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetPackagePtr LIBCALL
ID2ReplyGetPackageNew(void)
{
   ID2ReplyGetPackagePtr ptr = MemNew((size_t) sizeof(ID2ReplyGetPackage));

   return ptr;

}


/**************************************************
*
*    ID2ReplyGetPackageFree()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetPackagePtr LIBCALL
ID2ReplyGetPackageFree(ID2ReplyGetPackagePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   ID2ParamsFree(ptr -> params);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2ReplyGetPackageAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetPackagePtr LIBCALL
ID2ReplyGetPackageAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2ReplyGetPackagePtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2ReplyGetPackage ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REPLY_GET_PACKAGE);
   } else {
      atp = AsnLinkType(orig, ID2_REPLY_GET_PACKAGE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2ReplyGetPackageNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REPLY_GET_PACKAGE_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_GET_PACKAGE_params) {
      ptr -> params = ID2ParamsAsnRead(aip, atp);
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
   ptr = ID2ReplyGetPackageFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2ReplyGetPackageAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2ReplyGetPackageAsnWrite(ID2ReplyGetPackagePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REPLY_GET_PACKAGE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, ID2_REPLY_GET_PACKAGE_name,  &av);
   }
   if (ptr -> params != NULL) {
      if ( ! ID2ParamsAsnWrite(ptr -> params, aip, ID2_REPLY_GET_PACKAGE_params)) {
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
*    ID2ReplyGetSeqIdNew()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetSeqIdPtr LIBCALL
ID2ReplyGetSeqIdNew(void)
{
   ID2ReplyGetSeqIdPtr ptr = MemNew((size_t) sizeof(ID2ReplyGetSeqId));

   return ptr;

}


/**************************************************
*
*    ID2ReplyGetSeqIdFree()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetSeqIdPtr LIBCALL
ID2ReplyGetSeqIdFree(ID2ReplyGetSeqIdPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2RequestGetSeqIdFree(ptr -> request);
   AsnGenericChoiceSeqOfFree(ptr -> seq_id, (AsnOptFreeFunc) SeqIdFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2ReplyGetSeqIdAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetSeqIdPtr LIBCALL
ID2ReplyGetSeqIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2ReplyGetSeqIdPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2ReplyGetSeqId ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REPLY_GET_SEQ_ID);
   } else {
      atp = AsnLinkType(orig, ID2_REPLY_GET_SEQ_ID);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2ReplyGetSeqIdNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REPLY_GET_SEQ_ID_request) {
      ptr -> request = ID2RequestGetSeqIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_GET_SEQ_ID_seq_id) {
      ptr -> seq_id = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqIdAsnRead, (AsnOptFreeFunc) SeqIdFree);
      if (isError && ptr -> seq_id == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REPLY_GET_SEQ_ID_end_of_reply) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> end_of_reply = av.boolvalue;
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
   ptr = ID2ReplyGetSeqIdFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2ReplyGetSeqIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2ReplyGetSeqIdAsnWrite(ID2ReplyGetSeqIdPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REPLY_GET_SEQ_ID);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> request != NULL) {
      if ( ! ID2RequestGetSeqIdAsnWrite(ptr -> request, aip, ID2_REPLY_GET_SEQ_ID_request)) {
         goto erret;
      }
   }
   AsnGenericChoiceSeqOfAsnWrite(ptr -> seq_id, (AsnWriteFunc) SeqIdAsnWrite, aip, ID2_REPLY_GET_SEQ_ID_seq_id, ID2_REPLY_GET_SEQ_ID_seq_id_E);
   av.boolvalue = ptr -> end_of_reply;
   retval = AsnWrite(aip, REPLY_GET_SEQ_ID_end_of_reply,  &av);
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
*    ID2ReplyGetBlobIdNew()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetBlobIdPtr LIBCALL
ID2ReplyGetBlobIdNew(void)
{
   ID2ReplyGetBlobIdPtr ptr = MemNew((size_t) sizeof(ID2ReplyGetBlobId));

   ptr -> split_version = 0;
   return ptr;

}


/**************************************************
*
*    ID2ReplyGetBlobIdFree()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetBlobIdPtr LIBCALL
ID2ReplyGetBlobIdFree(ID2ReplyGetBlobIdPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SeqIdFree(ptr -> seq_id);
   ID2BlobIdFree(ptr -> blob_id);
   AsnGenericUserSeqOfFree(ptr -> annot_info, (AsnOptFreeFunc) ID2SSeqAnnotInfoFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2ReplyGetBlobIdAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetBlobIdPtr LIBCALL
ID2ReplyGetBlobIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2ReplyGetBlobIdPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2ReplyGetBlobId ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REPLY_GET_BLOB_ID);
   } else {
      atp = AsnLinkType(orig, ID2_REPLY_GET_BLOB_ID);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2ReplyGetBlobIdNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REPLY_GET_BLOB_ID_seq_id) {
      ptr -> seq_id = SeqIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_GET_BLOB_ID_blob_id) {
      ptr -> blob_id = ID2BlobIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GET_BLOB_ID_split_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> split_version = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REPLY_GET_BLOB_ID_annot_info) {
      ptr -> annot_info = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SSeqAnnotInfoAsnRead, (AsnOptFreeFunc) ID2SSeqAnnotInfoFree);
      if (isError && ptr -> annot_info == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REPLY_GET_BLOB_ID_end_of_reply) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> end_of_reply = av.boolvalue;
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
   ptr = ID2ReplyGetBlobIdFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2ReplyGetBlobIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2ReplyGetBlobIdAsnWrite(ID2ReplyGetBlobIdPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REPLY_GET_BLOB_ID);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> seq_id != NULL) {
      if ( ! SeqIdAsnWrite(ptr -> seq_id, aip, ID2_REPLY_GET_BLOB_ID_seq_id)) {
         goto erret;
      }
   }
   if (ptr -> blob_id != NULL) {
      if ( ! ID2BlobIdAsnWrite(ptr -> blob_id, aip, ID2_REPLY_GET_BLOB_ID_blob_id)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> split_version;
   retval = AsnWrite(aip, GET_BLOB_ID_split_version,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> annot_info, (AsnWriteFunc) ID2SSeqAnnotInfoAsnWrite, aip, REPLY_GET_BLOB_ID_annot_info, REPLY_GET_BLOB_ID_annot_info_E);
   av.boolvalue = ptr -> end_of_reply;
   retval = AsnWrite(aip, REPLY_GET_BLOB_ID_end_of_reply,  &av);
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
*    ID2ReplyGetBlobSeqIdsNew()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetBlobSeqIdsPtr LIBCALL
ID2ReplyGetBlobSeqIdsNew(void)
{
   ID2ReplyGetBlobSeqIdsPtr ptr = MemNew((size_t) sizeof(ID2ReplyGetBlobSeqIds));

   return ptr;

}


/**************************************************
*
*    ID2ReplyGetBlobSeqIdsFree()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetBlobSeqIdsPtr LIBCALL
ID2ReplyGetBlobSeqIdsFree(ID2ReplyGetBlobSeqIdsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2BlobIdFree(ptr -> blob_id);
   ID2ReplyDataFree(ptr -> ids);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2ReplyGetBlobSeqIdsAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetBlobSeqIdsPtr LIBCALL
ID2ReplyGetBlobSeqIdsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2ReplyGetBlobSeqIdsPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2ReplyGetBlobSeqIds ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REPLY_GET_BLOB_SEQ_IDS);
   } else {
      atp = AsnLinkType(orig, ID2_REPLY_GET_BLOB_SEQ_IDS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2ReplyGetBlobSeqIdsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == REPLY_GET_BLOB_SEQ_IDS_blob_id) {
      ptr -> blob_id = ID2BlobIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_GET_BLOB_SEQ_IDS_ids) {
      ptr -> ids = ID2ReplyDataAsnRead(aip, atp);
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
   ptr = ID2ReplyGetBlobSeqIdsFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2ReplyGetBlobSeqIdsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2ReplyGetBlobSeqIdsAsnWrite(ID2ReplyGetBlobSeqIdsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REPLY_GET_BLOB_SEQ_IDS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> blob_id != NULL) {
      if ( ! ID2BlobIdAsnWrite(ptr -> blob_id, aip, REPLY_GET_BLOB_SEQ_IDS_blob_id)) {
         goto erret;
      }
   }
   if (ptr -> ids != NULL) {
      if ( ! ID2ReplyDataAsnWrite(ptr -> ids, aip, ID2_REPLY_GET_BLOB_SEQ_IDS_ids)) {
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
*    ID2ReplyGetBlobNew()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetBlobPtr LIBCALL
ID2ReplyGetBlobNew(void)
{
   ID2ReplyGetBlobPtr ptr = MemNew((size_t) sizeof(ID2ReplyGetBlob));

   ptr -> split_version = 0;
   return ptr;

}


/**************************************************
*
*    ID2ReplyGetBlobFree()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetBlobPtr LIBCALL
ID2ReplyGetBlobFree(ID2ReplyGetBlobPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2BlobIdFree(ptr -> blob_id);
   ID2ReplyDataFree(ptr -> data);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2ReplyGetBlobAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2ReplyGetBlobPtr LIBCALL
ID2ReplyGetBlobAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2ReplyGetBlobPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2ReplyGetBlob ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REPLY_GET_BLOB);
   } else {
      atp = AsnLinkType(orig, ID2_REPLY_GET_BLOB);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2ReplyGetBlobNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REPLY_GET_BLOB_blob_id) {
      ptr -> blob_id = ID2BlobIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REPLY_GET_BLOB_split_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> split_version = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_GET_BLOB_data) {
      ptr -> data = ID2ReplyDataAsnRead(aip, atp);
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
   ptr = ID2ReplyGetBlobFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2ReplyGetBlobAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2ReplyGetBlobAsnWrite(ID2ReplyGetBlobPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REPLY_GET_BLOB);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> blob_id != NULL) {
      if ( ! ID2BlobIdAsnWrite(ptr -> blob_id, aip, ID2_REPLY_GET_BLOB_blob_id)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> split_version;
   retval = AsnWrite(aip, REPLY_GET_BLOB_split_version,  &av);
   if (ptr -> data != NULL) {
      if ( ! ID2ReplyDataAsnWrite(ptr -> data, aip, ID2_REPLY_GET_BLOB_data)) {
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
*    ID2ReplyReGetBlobNew()
*
**************************************************/
NLM_EXTERN 
ID2ReplyReGetBlobPtr LIBCALL
ID2ReplyReGetBlobNew(void)
{
   ID2ReplyReGetBlobPtr ptr = MemNew((size_t) sizeof(ID2ReplyReGetBlob));

   return ptr;

}


/**************************************************
*
*    ID2ReplyReGetBlobFree()
*
**************************************************/
NLM_EXTERN 
ID2ReplyReGetBlobPtr LIBCALL
ID2ReplyReGetBlobFree(ID2ReplyReGetBlobPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2BlobIdFree(ptr -> blob_id);
   ID2ReplyDataFree(ptr -> data);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2ReplyReGetBlobAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2ReplyReGetBlobPtr LIBCALL
ID2ReplyReGetBlobAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2ReplyReGetBlobPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2ReplyReGetBlob ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REPLY_REGET_BLOB);
   } else {
      atp = AsnLinkType(orig, ID2_REPLY_REGET_BLOB);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2ReplyReGetBlobNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REPLY_REGET_BLOB_blob_id) {
      ptr -> blob_id = ID2BlobIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REPLY_REGET_BLOB_split_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> split_version = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_REGET_BLOB_offset) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> offset = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_REGET_BLOB_data) {
      ptr -> data = ID2ReplyDataAsnRead(aip, atp);
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
   ptr = ID2ReplyReGetBlobFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2ReplyReGetBlobAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2ReplyReGetBlobAsnWrite(ID2ReplyReGetBlobPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REPLY_REGET_BLOB);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> blob_id != NULL) {
      if ( ! ID2BlobIdAsnWrite(ptr -> blob_id, aip, ID2_REPLY_REGET_BLOB_blob_id)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> split_version;
   retval = AsnWrite(aip, REPLY_REGET_BLOB_split_version,  &av);
   av.intvalue = ptr -> offset;
   retval = AsnWrite(aip, ID2_REPLY_REGET_BLOB_offset,  &av);
   if (ptr -> data != NULL) {
      if ( ! ID2ReplyDataAsnWrite(ptr -> data, aip, ID2_REPLY_REGET_BLOB_data)) {
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
*    ID2SReplyGetSplitInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SReplyGetSplitInfoPtr LIBCALL
ID2SReplyGetSplitInfoNew(void)
{
   ID2SReplyGetSplitInfoPtr ptr = MemNew((size_t) sizeof(ID2SReplyGetSplitInfo));

   return ptr;

}


/**************************************************
*
*    ID2SReplyGetSplitInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SReplyGetSplitInfoPtr LIBCALL
ID2SReplyGetSplitInfoFree(ID2SReplyGetSplitInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2BlobIdFree(ptr -> blob_id);
   ID2ReplyDataFree(ptr -> data);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SReplyGetSplitInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SReplyGetSplitInfoPtr LIBCALL
ID2SReplyGetSplitInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SReplyGetSplitInfoPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SReplyGetSplitInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_REPLY_GET_SPLIT_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_REPLY_GET_SPLIT_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SReplyGetSplitInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == REPLY_GET_SPLIT_INFO_blob_id) {
      ptr -> blob_id = ID2BlobIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GET_SPLIT_INFO_split_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> split_version = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_REPLY_GET_SPLIT_INFO_data) {
      ptr -> data = ID2ReplyDataAsnRead(aip, atp);
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
   ptr = ID2SReplyGetSplitInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SReplyGetSplitInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SReplyGetSplitInfoAsnWrite(ID2SReplyGetSplitInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_REPLY_GET_SPLIT_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> blob_id != NULL) {
      if ( ! ID2BlobIdAsnWrite(ptr -> blob_id, aip, REPLY_GET_SPLIT_INFO_blob_id)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> split_version;
   retval = AsnWrite(aip, GET_SPLIT_INFO_split_version,  &av);
   if (ptr -> data != NULL) {
      if ( ! ID2ReplyDataAsnWrite(ptr -> data, aip, ID2S_REPLY_GET_SPLIT_INFO_data)) {
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
*    ID2SReplyGetChunkNew()
*
**************************************************/
NLM_EXTERN 
ID2SReplyGetChunkPtr LIBCALL
ID2SReplyGetChunkNew(void)
{
   ID2SReplyGetChunkPtr ptr = MemNew((size_t) sizeof(ID2SReplyGetChunk));

   return ptr;

}


/**************************************************
*
*    ID2SReplyGetChunkFree()
*
**************************************************/
NLM_EXTERN 
ID2SReplyGetChunkPtr LIBCALL
ID2SReplyGetChunkFree(ID2SReplyGetChunkPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2BlobIdFree(ptr -> blob_id);
   ID2ReplyDataFree(ptr -> data);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SReplyGetChunkAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SReplyGetChunkPtr LIBCALL
ID2SReplyGetChunkAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SReplyGetChunkPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SReplyGetChunk ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_REPLY_GET_CHUNK);
   } else {
      atp = AsnLinkType(orig, ID2S_REPLY_GET_CHUNK);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SReplyGetChunkNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_REPLY_GET_CHUNK_blob_id) {
      ptr -> blob_id = ID2BlobIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_REPLY_GET_CHUNK_chunk_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> chunk_id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_REPLY_GET_CHUNK_data) {
      ptr -> data = ID2ReplyDataAsnRead(aip, atp);
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
   ptr = ID2SReplyGetChunkFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SReplyGetChunkAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SReplyGetChunkAsnWrite(ID2SReplyGetChunkPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_REPLY_GET_CHUNK);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> blob_id != NULL) {
      if ( ! ID2BlobIdAsnWrite(ptr -> blob_id, aip, ID2S_REPLY_GET_CHUNK_blob_id)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> chunk_id;
   retval = AsnWrite(aip, ID2S_REPLY_GET_CHUNK_chunk_id,  &av);
   if (ptr -> data != NULL) {
      if ( ! ID2ReplyDataAsnWrite(ptr -> data, aip, ID2S_REPLY_GET_CHUNK_data)) {
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
*    ID2ReplyDataNew()
*
**************************************************/
NLM_EXTERN 
ID2ReplyDataPtr LIBCALL
ID2ReplyDataNew(void)
{
   ID2ReplyDataPtr ptr = MemNew((size_t) sizeof(ID2ReplyData));

   ptr -> data_type = 0;
   ptr -> data_format = 0;
   ptr -> data_compression = 0;
   return ptr;

}


/**************************************************
*
*    ID2ReplyDataFree()
*
**************************************************/
NLM_EXTERN 
ID2ReplyDataPtr LIBCALL
ID2ReplyDataFree(ID2ReplyDataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> data ,ASNCODE_BYTEVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2ReplyDataAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2ReplyDataPtr LIBCALL
ID2ReplyDataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2ReplyDataPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2ReplyData ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_REPLY_DATA);
   } else {
      atp = AsnLinkType(orig, ID2_REPLY_DATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2ReplyDataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_REPLY_DATA_data_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> data_type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_DATA_data_format) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> data_format = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_DATA_data_compression) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> data_compression = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_REPLY_DATA_data) {
      ptr -> data = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_BYTEVAL_SLOT, &isError);
      if (isError && ptr -> data == NULL) {
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
   ptr = ID2ReplyDataFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2ReplyDataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2ReplyDataAsnWrite(ID2ReplyDataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_REPLY_DATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> data_type;
   retval = AsnWrite(aip, ID2_REPLY_DATA_data_type,  &av);
   av.intvalue = ptr -> data_format;
   retval = AsnWrite(aip, ID2_REPLY_DATA_data_format,  &av);
   av.intvalue = ptr -> data_compression;
   retval = AsnWrite(aip, ID2_REPLY_DATA_data_compression,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> data ,ASNCODE_BYTEVAL_SLOT, aip, ID2_REPLY_DATA_data, ID2_REPLY_DATA_data_E);
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
*    ID2BlobSeqIdsFree()
*
**************************************************/
NLM_EXTERN 
ID2BlobSeqIdsPtr LIBCALL
ID2BlobSeqIdsFree(ID2BlobSeqIdsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) ID2BlobSeqIdFree);
   return NULL;
}


/**************************************************
*
*    ID2BlobSeqIdsAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2BlobSeqIdsPtr LIBCALL
ID2BlobSeqIdsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2BlobSeqIdsPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2BlobSeqIds ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_BLOB_SEQ_IDS);
   } else {
      atp = AsnLinkType(orig, ID2_BLOB_SEQ_IDS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2BlobSeqIdAsnRead, (AsnOptFreeFunc) ID2BlobSeqIdFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ID2BlobSeqIdsFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2BlobSeqIdsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2BlobSeqIdsAsnWrite(ID2BlobSeqIdsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_BLOB_SEQ_IDS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) ID2BlobSeqIdAsnWrite, aip, atp, ID2_BLOB_SEQ_IDS_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ID2BlobSeqIdNew()
*
**************************************************/
NLM_EXTERN 
ID2BlobSeqIdPtr LIBCALL
ID2BlobSeqIdNew(void)
{
   ID2BlobSeqIdPtr ptr = MemNew((size_t) sizeof(ID2BlobSeqId));

   return ptr;

}


/**************************************************
*
*    ID2BlobSeqIdFree()
*
**************************************************/
NLM_EXTERN 
ID2BlobSeqIdPtr LIBCALL
ID2BlobSeqIdFree(ID2BlobSeqIdPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SeqIdFree(ptr -> seq_id);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2BlobSeqIdAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2BlobSeqIdPtr LIBCALL
ID2BlobSeqIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2BlobSeqIdPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2BlobSeqId ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_BLOB_SEQ_ID);
   } else {
      atp = AsnLinkType(orig, ID2_BLOB_SEQ_ID);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2BlobSeqIdNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_BLOB_SEQ_ID_seq_id) {
      ptr -> seq_id = SeqIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_BLOB_SEQ_ID_replaced) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> replaced = av.boolvalue;
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
   ptr = ID2BlobSeqIdFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2BlobSeqIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2BlobSeqIdAsnWrite(ID2BlobSeqIdPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_BLOB_SEQ_ID);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> seq_id != NULL) {
      if ( ! SeqIdAsnWrite(ptr -> seq_id, aip, ID2_BLOB_SEQ_ID_seq_id)) {
         goto erret;
      }
   }
   av.boolvalue = ptr -> replaced;
   retval = AsnWrite(aip, ID2_BLOB_SEQ_ID_replaced,  &av);
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
*    ID2ParamNew()
*
**************************************************/
NLM_EXTERN 
ID2ParamPtr LIBCALL
ID2ParamNew(void)
{
   ID2ParamPtr ptr = MemNew((size_t) sizeof(ID2Param));

   ptr -> type = 1;
   return ptr;

}


/**************************************************
*
*    ID2ParamFree()
*
**************************************************/
NLM_EXTERN 
ID2ParamPtr LIBCALL
ID2ParamFree(ID2ParamPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   AsnGenericBaseSeqOfFree(ptr -> value ,ASNCODE_PTRVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2ParamAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2ParamPtr LIBCALL
ID2ParamAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2ParamPtr ptr;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2Param ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_PARAM);
   } else {
      atp = AsnLinkType(orig, ID2_PARAM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2ParamNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_PARAM_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_PARAM_value) {
      ptr -> value = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> value == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_PARAM_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
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
   ptr = ID2ParamFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2ParamAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2ParamAsnWrite(ID2ParamPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2_PARAM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, ID2_PARAM_name,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> value ,ASNCODE_PTRVAL_SLOT, aip, ID2_PARAM_value, ID2_PARAM_value_E);
   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, ID2_PARAM_type,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

