#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <tax1map.h>
#include <objtax1.h>

static Boolean loaded = FALSE;

#include <asntax1.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objtax1AsnLoad(void)
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
*    Generated object loaders for Module NCBI-Taxon1
*    Generated using ASNCODE Revision: 6.12 at Sep 28, 2001 11:31 AM
*
**************************************************/


/**************************************************
*
*    Taxon1ReqFree()
*
**************************************************/
NLM_EXTERN 
Taxon1ReqPtr LIBCALL
Taxon1ReqFree(ValNodePtr anp)
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
   case Taxon1Req_findname:
      MemFree(anp -> data.ptrvalue);
      break;
   case Taxon1Req_getdesignator:
      MemFree(anp -> data.ptrvalue);
      break;
   case Taxon1Req_getunique:
      MemFree(anp -> data.ptrvalue);
      break;
   case Taxon1Req_getidbyorg:
      OrgRefFree(anp -> data.ptrvalue);
      break;
   case Taxon1Req_lookup:
      OrgRefFree(anp -> data.ptrvalue);
      break;
   case Taxon1Req_getorgmod:
      Taxon1InfoFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    Taxon1ReqAsnRead()
*
**************************************************/
NLM_EXTERN 
Taxon1ReqPtr LIBCALL
Taxon1ReqAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objtax1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Taxon1Req ::= (self contained) */
      atp = AsnReadId(aip, amp, TAXON1_REQ);
   } else {
      atp = AsnLinkType(orig, TAXON1_REQ);    /* link in local tree */
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
   if (atp == TAXON1_REQ_init) {
      choice = Taxon1Req_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == TAXON1_REQ_findname) {
      choice = Taxon1Req_findname;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == TAXON1_REQ_getdesignator) {
      choice = Taxon1Req_getdesignator;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == TAXON1_REQ_getunique) {
      choice = Taxon1Req_getunique;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == TAXON1_REQ_getidbyorg) {
      choice = Taxon1Req_getidbyorg;
      func = (AsnReadFunc) OrgRefAsnRead;
   }
   else if (atp == TAXON1_REQ_getorgnames) {
      choice = Taxon1Req_getorgnames;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == TAXON1_REQ_getcde) {
      choice = Taxon1Req_getcde;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == TAXON1_REQ_getranks) {
      choice = Taxon1Req_getranks;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == TAXON1_REQ_getdivs) {
      choice = Taxon1Req_getdivs;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == TAXON1_REQ_getgcs) {
      choice = Taxon1Req_getgcs;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == TAXON1_REQ_getlineage) {
      choice = Taxon1Req_getlineage;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == TAXON1_REQ_getchildren) {
      choice = Taxon1Req_getchildren;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == TAXON1_REQ_getbyid) {
      choice = Taxon1Req_getbyid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == TAXON1_REQ_lookup) {
      choice = Taxon1Req_lookup;
      func = (AsnReadFunc) OrgRefAsnRead;
   }
   else if (atp == TAXON1_REQ_getorgmod) {
      choice = Taxon1Req_getorgmod;
      func = (AsnReadFunc) Taxon1InfoAsnRead;
   }
   else if (atp == TAXON1_REQ_fini) {
      choice = Taxon1Req_fini;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == TAXON1_REQ_id4gi) {
      choice = Taxon1Req_id4gi;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == TAXON1_REQ_taxachildren) {
      choice = Taxon1Req_taxachildren;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == TAXON1_REQ_taxalineage) {
      choice = Taxon1Req_taxalineage;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == TAXON1_REQ_maxtaxid) {
      choice = Taxon1Req_maxtaxid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
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
*    Taxon1ReqAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Taxon1ReqAsnWrite(Taxon1ReqPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax1AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, TAXON1_REQ);   /* link local tree */
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
   case Taxon1Req_init:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, TAXON1_REQ_init, &av);
      break;
   case Taxon1Req_findname:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, TAXON1_REQ_findname, &av);
      break;
   case Taxon1Req_getdesignator:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, TAXON1_REQ_getdesignator, &av);
      break;
   case Taxon1Req_getunique:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, TAXON1_REQ_getunique, &av);
      break;
   case Taxon1Req_getidbyorg:
      writetype = TAXON1_REQ_getidbyorg;
      func = (AsnWriteFunc) OrgRefAsnWrite;
      break;
   case Taxon1Req_getorgnames:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_REQ_getorgnames, &av);
      break;
   case Taxon1Req_getcde:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, TAXON1_REQ_getcde, &av);
      break;
   case Taxon1Req_getranks:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, TAXON1_REQ_getranks, &av);
      break;
   case Taxon1Req_getdivs:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, TAXON1_REQ_getdivs, &av);
      break;
   case Taxon1Req_getgcs:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, TAXON1_REQ_getgcs, &av);
      break;
   case Taxon1Req_getlineage:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_REQ_getlineage, &av);
      break;
   case Taxon1Req_getchildren:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_REQ_getchildren, &av);
      break;
   case Taxon1Req_getbyid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_REQ_getbyid, &av);
      break;
   case Taxon1Req_lookup:
      writetype = TAXON1_REQ_lookup;
      func = (AsnWriteFunc) OrgRefAsnWrite;
      break;
   case Taxon1Req_getorgmod:
      writetype = TAXON1_REQ_getorgmod;
      func = (AsnWriteFunc) Taxon1InfoAsnWrite;
      break;
   case Taxon1Req_fini:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, TAXON1_REQ_fini, &av);
      break;
   case Taxon1Req_id4gi:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_REQ_id4gi, &av);
      break;
   case Taxon1Req_taxachildren:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_REQ_taxachildren, &av);
      break;
   case Taxon1Req_taxalineage:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_REQ_taxalineage, &av);
      break;
   case Taxon1Req_maxtaxid:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, TAXON1_REQ_maxtaxid, &av);
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
*    Taxon1InfoNew()
*
**************************************************/
NLM_EXTERN 
Taxon1InfoPtr LIBCALL
Taxon1InfoNew(void)
{
   Taxon1InfoPtr ptr = MemNew((size_t) sizeof(Taxon1Info));

   return ptr;

}


/**************************************************
*
*    Taxon1InfoFree()
*
**************************************************/
NLM_EXTERN 
Taxon1InfoPtr LIBCALL
Taxon1InfoFree(Taxon1InfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> sval);
   return MemFree(ptr);
}


/**************************************************
*
*    Taxon1InfoAsnRead()
*
**************************************************/
NLM_EXTERN 
Taxon1InfoPtr LIBCALL
Taxon1InfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Taxon1InfoPtr ptr;

   if (! loaded)
   {
      if (! objtax1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Taxon1Info ::= (self contained) */
      atp = AsnReadId(aip, amp, TAXON1_INFO);
   } else {
      atp = AsnLinkType(orig, TAXON1_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Taxon1InfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TAXON1_INFO_ival1) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ival1 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON1_INFO_ival2) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ival2 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON1_INFO_sval) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sval = av.ptrvalue;
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
   ptr = Taxon1InfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    Taxon1InfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Taxon1InfoAsnWrite(Taxon1InfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TAXON1_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> ival1;
   retval = AsnWrite(aip, TAXON1_INFO_ival1,  &av);
   av.intvalue = ptr -> ival2;
   retval = AsnWrite(aip, TAXON1_INFO_ival2,  &av);
   if (ptr -> sval != NULL) {
      av.ptrvalue = ptr -> sval;
      retval = AsnWrite(aip, TAXON1_INFO_sval,  &av);
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
*    Taxon1RespFree()
*
**************************************************/
NLM_EXTERN 
Taxon1RespPtr LIBCALL
Taxon1RespFree(ValNodePtr anp)
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
   case Taxon1Resp_error:
      Taxon1ErrorFree(anp -> data.ptrvalue);
      break;
   case Taxon1Resp_findname:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) Taxon1NameFree);
      break;
   case Taxon1Resp_getorgnames:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) Taxon1NameFree);
      break;
   case Taxon1Resp_getcde:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) Taxon1InfoFree);
      break;
   case Taxon1Resp_getranks:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) Taxon1InfoFree);
      break;
   case Taxon1Resp_getdivs:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) Taxon1InfoFree);
      break;
   case Taxon1Resp_getgcs:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) Taxon1InfoFree);
      break;
   case Taxon1Resp_getlineage:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) Taxon1InfoFree);
      break;
   case Taxon1Resp_getchildren:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) Taxon1InfoFree);
      break;
   case Taxon1Resp_getbyid:
      Taxon1DataFree(anp -> data.ptrvalue);
      break;
   case Taxon1Resp_lookup:
      Taxon1DataFree(anp -> data.ptrvalue);
      break;
   case Taxon1Resp_getorgmod:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) Taxon1InfoFree);
      break;
   case Taxon1Resp_taxabyid:
      Taxon2DataFree(anp -> data.ptrvalue);
      break;
   case Taxon1Resp_taxachildren:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) Taxon1NameFree);
      break;
   case Taxon1Resp_taxalineage:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) Taxon1NameFree);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    Taxon1RespAsnRead()
*
**************************************************/
NLM_EXTERN 
Taxon1RespPtr LIBCALL
Taxon1RespAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objtax1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Taxon1Resp ::= (self contained) */
      atp = AsnReadId(aip, amp, TAXON1_RESP);
   } else {
      atp = AsnLinkType(orig, TAXON1_RESP);    /* link in local tree */
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
   if (atp == TAXON1_RESP_error) {
      choice = Taxon1Resp_error;
      func = (AsnReadFunc) Taxon1ErrorAsnRead;
   }
   else if (atp == TAXON1_RESP_init) {
      choice = Taxon1Resp_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == TAXON1_RESP_findname) {
      choice = Taxon1Resp_findname;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Taxon1NameAsnRead,             (AsnOptFreeFunc) Taxon1NameFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == TAXON1_RESP_getdesignator) {
      choice = Taxon1Resp_getdesignator;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == TAXON1_RESP_getunique) {
      choice = Taxon1Resp_getunique;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == TAXON1_RESP_getidbyorg) {
      choice = Taxon1Resp_getidbyorg;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == TAXON1_RESP_getorgnames) {
      choice = Taxon1Resp_getorgnames;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Taxon1NameAsnRead,             (AsnOptFreeFunc) Taxon1NameFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == TAXON1_RESP_getcde) {
      choice = Taxon1Resp_getcde;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Taxon1InfoAsnRead,             (AsnOptFreeFunc) Taxon1InfoFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == TAXON1_RESP_getranks) {
      choice = Taxon1Resp_getranks;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Taxon1InfoAsnRead,             (AsnOptFreeFunc) Taxon1InfoFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == TAXON1_RESP_getdivs) {
      choice = Taxon1Resp_getdivs;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Taxon1InfoAsnRead,             (AsnOptFreeFunc) Taxon1InfoFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == TAXON1_RESP_getgcs) {
      choice = Taxon1Resp_getgcs;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Taxon1InfoAsnRead,             (AsnOptFreeFunc) Taxon1InfoFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == TAXON1_RESP_getlineage) {
      choice = Taxon1Resp_getlineage;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Taxon1InfoAsnRead,             (AsnOptFreeFunc) Taxon1InfoFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == TAXON1_RESP_getchildren) {
      choice = Taxon1Resp_getchildren;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Taxon1InfoAsnRead,             (AsnOptFreeFunc) Taxon1InfoFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == TAXON1_RESP_getbyid) {
      choice = Taxon1Resp_getbyid;
      func = (AsnReadFunc) Taxon1DataAsnRead;
   }
   else if (atp == TAXON1_RESP_lookup) {
      choice = Taxon1Resp_lookup;
      func = (AsnReadFunc) Taxon1DataAsnRead;
   }
   else if (atp == TAXON1_RESP_getorgmod) {
      choice = Taxon1Resp_getorgmod;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Taxon1InfoAsnRead,             (AsnOptFreeFunc) Taxon1InfoFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == TAXON1_RESP_fini) {
      choice = Taxon1Resp_fini;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == TAXON1_RESP_id4gi) {
      choice = Taxon1Resp_id4gi;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == TAXON1_RESP_taxabyid) {
      choice = Taxon1Resp_taxabyid;
      func = (AsnReadFunc) Taxon2DataAsnRead;
   }
   else if (atp == TAXON1_RESP_taxachildren) {
      choice = Taxon1Resp_taxachildren;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Taxon1NameAsnRead,             (AsnOptFreeFunc) Taxon1NameFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == TAXON1_RESP_taxalineage) {
      choice = Taxon1Resp_taxalineage;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Taxon1NameAsnRead,             (AsnOptFreeFunc) Taxon1NameFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == TAXON1_RESP_maxtaxid) {
      choice = Taxon1Resp_maxtaxid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
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
*    Taxon1RespAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Taxon1RespAsnWrite(Taxon1RespPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax1AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, TAXON1_RESP);   /* link local tree */
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
   case Taxon1Resp_error:
      writetype = TAXON1_RESP_error;
      func = (AsnWriteFunc) Taxon1ErrorAsnWrite;
      break;
   case Taxon1Resp_init:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, TAXON1_RESP_init, &av);
      break;
   case Taxon1Resp_findname:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) Taxon1NameAsnWrite, aip, TAXON1_RESP_findname, TAXON1_RESP_findname_E);
      break;
   case Taxon1Resp_getdesignator:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_RESP_getdesignator, &av);
      break;
   case Taxon1Resp_getunique:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_RESP_getunique, &av);
      break;
   case Taxon1Resp_getidbyorg:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_RESP_getidbyorg, &av);
      break;
   case Taxon1Resp_getorgnames:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) Taxon1NameAsnWrite, aip, TAXON1_RESP_getorgnames, TAXON1_RESP_getorgnames_E);
      break;
   case Taxon1Resp_getcde:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) Taxon1InfoAsnWrite, aip, TAXON1_RESP_getcde, TAXON1_RESP_getcde_E);
      break;
   case Taxon1Resp_getranks:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) Taxon1InfoAsnWrite, aip, TAXON1_RESP_getranks, TAXON1_RESP_getranks_E);
      break;
   case Taxon1Resp_getdivs:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) Taxon1InfoAsnWrite, aip, TAXON1_RESP_getdivs, TAXON1_RESP_getdivs_E);
      break;
   case Taxon1Resp_getgcs:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) Taxon1InfoAsnWrite, aip, TAXON1_RESP_getgcs, TAXON1_RESP_getgcs_E);
      break;
   case Taxon1Resp_getlineage:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) Taxon1InfoAsnWrite, aip, TAXON1_RESP_getlineage, TAXON1_RESP_getlineage_E);
      break;
   case Taxon1Resp_getchildren:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) Taxon1InfoAsnWrite, aip, TAXON1_RESP_getchildren, TAXON1_RESP_getchildren_E);
      break;
   case Taxon1Resp_getbyid:
      writetype = TAXON1_RESP_getbyid;
      func = (AsnWriteFunc) Taxon1DataAsnWrite;
      break;
   case Taxon1Resp_lookup:
      writetype = TAXON1_RESP_lookup;
      func = (AsnWriteFunc) Taxon1DataAsnWrite;
      break;
   case Taxon1Resp_getorgmod:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) Taxon1InfoAsnWrite, aip, TAXON1_RESP_getorgmod, TAXON1_RESP_getorgmod_E);
      break;
   case Taxon1Resp_fini:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, TAXON1_RESP_fini, &av);
      break;
   case Taxon1Resp_id4gi:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_RESP_id4gi, &av);
      break;
   case Taxon1Resp_taxabyid:
      writetype = TAXON1_RESP_taxabyid;
      func = (AsnWriteFunc) Taxon2DataAsnWrite;
      break;
   case Taxon1Resp_taxachildren:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) Taxon1NameAsnWrite, aip, TAXON1_RESP_taxachildren, TAXON1_RESP_taxachildren_E);
      break;
   case Taxon1Resp_taxalineage:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) Taxon1NameAsnWrite, aip, TAXON1_RESP_taxalineage, TAXON1_RESP_taxalineage_E);
      break;
   case Taxon1Resp_maxtaxid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, TAXON1_RESP_maxtaxid, &av);
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
*    Taxon1ErrorNew()
*
**************************************************/
NLM_EXTERN 
Taxon1ErrorPtr LIBCALL
Taxon1ErrorNew(void)
{
   Taxon1ErrorPtr ptr = MemNew((size_t) sizeof(Taxon1Error));

   return ptr;

}


/**************************************************
*
*    Taxon1ErrorFree()
*
**************************************************/
NLM_EXTERN 
Taxon1ErrorPtr LIBCALL
Taxon1ErrorFree(Taxon1ErrorPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> msg);
   return MemFree(ptr);
}


/**************************************************
*
*    Taxon1ErrorAsnRead()
*
**************************************************/
NLM_EXTERN 
Taxon1ErrorPtr LIBCALL
Taxon1ErrorAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Taxon1ErrorPtr ptr;

   if (! loaded)
   {
      if (! objtax1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Taxon1Error ::= (self contained) */
      atp = AsnReadId(aip, amp, TAXON1_ERROR);
   } else {
      atp = AsnLinkType(orig, TAXON1_ERROR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Taxon1ErrorNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TAXON1_ERROR_level) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> level = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON1_ERROR_msg) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> msg = av.ptrvalue;
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
   ptr = Taxon1ErrorFree(ptr);
   goto ret;
}



/**************************************************
*
*    Taxon1ErrorAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Taxon1ErrorAsnWrite(Taxon1ErrorPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TAXON1_ERROR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> level;
   retval = AsnWrite(aip, TAXON1_ERROR_level,  &av);
   if (ptr -> msg != NULL) {
      av.ptrvalue = ptr -> msg;
      retval = AsnWrite(aip, TAXON1_ERROR_msg,  &av);
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
*    Taxon1NameNew()
*
**************************************************/
NLM_EXTERN 
Taxon1NamePtr LIBCALL
Taxon1NameNew(void)
{
   Taxon1NamePtr ptr = MemNew((size_t) sizeof(Taxon1Name));

   return ptr;

}


/**************************************************
*
*    Taxon1NameFree()
*
**************************************************/
NLM_EXTERN 
Taxon1NamePtr LIBCALL
Taxon1NameFree(Taxon1NamePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> oname);
   MemFree(ptr -> uname);
   return MemFree(ptr);
}


/**************************************************
*
*    Taxon1NameAsnRead()
*
**************************************************/
NLM_EXTERN 
Taxon1NamePtr LIBCALL
Taxon1NameAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Taxon1NamePtr ptr;

   if (! loaded)
   {
      if (! objtax1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Taxon1Name ::= (self contained) */
      atp = AsnReadId(aip, amp, TAXON1_NAME);
   } else {
      atp = AsnLinkType(orig, TAXON1_NAME);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Taxon1NameNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TAXON1_NAME_taxid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> taxid = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON1_NAME_cde) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> cde = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON1_NAME_oname) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> oname = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON1_NAME_uname) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> uname = av.ptrvalue;
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
   ptr = Taxon1NameFree(ptr);
   goto ret;
}



/**************************************************
*
*    Taxon1NameAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Taxon1NameAsnWrite(Taxon1NamePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TAXON1_NAME);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> taxid;
   retval = AsnWrite(aip, TAXON1_NAME_taxid,  &av);
   av.intvalue = ptr -> cde;
   retval = AsnWrite(aip, TAXON1_NAME_cde,  &av);
   if (ptr -> oname != NULL) {
      av.ptrvalue = ptr -> oname;
      retval = AsnWrite(aip, TAXON1_NAME_oname,  &av);
   }
   if (ptr -> uname != NULL) {
      av.ptrvalue = ptr -> uname;
      retval = AsnWrite(aip, TAXON1_NAME_uname,  &av);
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
*    Taxon1DataNew()
*
**************************************************/
NLM_EXTERN 
Taxon1DataPtr LIBCALL
Taxon1DataNew(void)
{
   Taxon1DataPtr ptr = MemNew((size_t) sizeof(Taxon1Data));

   return ptr;

}


/**************************************************
*
*    Taxon1DataFree()
*
**************************************************/
NLM_EXTERN 
Taxon1DataPtr LIBCALL
Taxon1DataFree(Taxon1DataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   OrgRefFree(ptr -> org);
   MemFree(ptr -> div);
   MemFree(ptr -> embl_code);
   return MemFree(ptr);
}


/**************************************************
*
*    Taxon1DataAsnRead()
*
**************************************************/
NLM_EXTERN 
Taxon1DataPtr LIBCALL
Taxon1DataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Taxon1DataPtr ptr;

   if (! loaded)
   {
      if (! objtax1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Taxon1Data ::= (self contained) */
      atp = AsnReadId(aip, amp, TAXON1_DATA);
   } else {
      atp = AsnLinkType(orig, TAXON1_DATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Taxon1DataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TAXON1_DATA_org) {
      ptr -> org = OrgRefAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON1_DATA_div) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> div = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON1_DATA_embl_code) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> embl_code = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON1_DATA_is_species_level) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_species_level = av.boolvalue;
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
   ptr = Taxon1DataFree(ptr);
   goto ret;
}



/**************************************************
*
*    Taxon1DataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Taxon1DataAsnWrite(Taxon1DataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TAXON1_DATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> org != NULL) {
      if ( ! OrgRefAsnWrite(ptr -> org, aip, TAXON1_DATA_org)) {
         goto erret;
      }
   }
   if (ptr -> div != NULL) {
      av.ptrvalue = ptr -> div;
      retval = AsnWrite(aip, TAXON1_DATA_div,  &av);
   }
   if (ptr -> embl_code != NULL) {
      av.ptrvalue = ptr -> embl_code;
      retval = AsnWrite(aip, TAXON1_DATA_embl_code,  &av);
   }
   av.boolvalue = ptr -> is_species_level;
   retval = AsnWrite(aip, TAXON1_DATA_is_species_level,  &av);
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
*    Taxon2DataNew()
*
**************************************************/
NLM_EXTERN 
Taxon2DataPtr LIBCALL
Taxon2DataNew(void)
{
   Taxon2DataPtr ptr = MemNew((size_t) sizeof(Taxon2Data));

   return ptr;

}


/**************************************************
*
*    Taxon2DataFree()
*
**************************************************/
NLM_EXTERN 
Taxon2DataPtr LIBCALL
Taxon2DataFree(Taxon2DataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   OrgRefFree(ptr -> org);
   AsnGenericBaseSeqOfFree(ptr -> blast_name ,ASNCODE_PTRVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    Taxon2DataAsnRead()
*
**************************************************/
NLM_EXTERN 
Taxon2DataPtr LIBCALL
Taxon2DataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Taxon2DataPtr ptr;

   if (! loaded)
   {
      if (! objtax1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Taxon2Data ::= (self contained) */
      atp = AsnReadId(aip, amp, TAXON2_DATA);
   } else {
      atp = AsnLinkType(orig, TAXON2_DATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Taxon2DataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TAXON2_DATA_org) {
      ptr -> org = OrgRefAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON2_DATA_blast_name) {
      ptr -> blast_name = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> blast_name == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON2_DATA_is_uncultured) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_uncultured = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TAXON2_DATA_is_species_level) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_species_level = av.boolvalue;
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
   ptr = Taxon2DataFree(ptr);
   goto ret;
}



/**************************************************
*
*    Taxon2DataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Taxon2DataAsnWrite(Taxon2DataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objtax1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TAXON2_DATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> org != NULL) {
      if ( ! OrgRefAsnWrite(ptr -> org, aip, TAXON2_DATA_org)) {
         goto erret;
      }
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> blast_name ,ASNCODE_PTRVAL_SLOT, aip, TAXON2_DATA_blast_name, TAXON2_DATA_blast_name_E);
   av.boolvalue = ptr -> is_uncultured;
   retval = AsnWrite(aip, TAXON2_DATA_is_uncultured,  &av);
   av.boolvalue = ptr -> is_species_level;
   retval = AsnWrite(aip, TAXON2_DATA_is_species_level,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

