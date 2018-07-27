#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <id1map.h>
#include <id1gen.h>

static Boolean loaded = FALSE;

#include <id1.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
id1genAsnLoad(void)
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
*    Generated object loaders for Module NCBI-ID1Access
*    Generated using ASNCODE Revision: 6.0 at Jul 30, 1998  4:04 PM
*
**************************************************/


/**************************************************
*
*    ID1serverRequestFree()
*
**************************************************/
NLM_EXTERN 
ID1serverRequestPtr LIBCALL
ID1serverRequestFree(ValNodePtr anp)
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
   case ID1serverRequest_getgi:
      SeqIdFree(anp -> data.ptrvalue);
      break;
   case ID1serverRequest_getsefromgi:
      ID1serverMaxcomplexFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ID1serverRequestAsnRead()
*
**************************************************/
NLM_EXTERN 
ID1serverRequestPtr LIBCALL
ID1serverRequestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! id1genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID1serverRequest ::= (self contained) */
      atp = AsnReadId(aip, amp, ID1SERVER_REQUEST);
   } else {
      atp = AsnLinkType(orig, ID1SERVER_REQUEST);    /* link in local tree */
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
   if (atp == ID1SERVER_REQUEST_init) {
      choice = ID1serverRequest_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == ID1SERVER_REQUEST_getgi) {
      choice = ID1serverRequest_getgi;
      func = (AsnReadFunc) SeqIdAsnRead;
   }
   else if (atp == ID1SERVER_REQUEST_getsefromgi) {
      choice = ID1serverRequest_getsefromgi;
      func = (AsnReadFunc) ID1serverMaxcomplexAsnRead;
   }
   else if (atp == ID1SERVER_REQUEST_fini) {
      choice = ID1serverRequest_fini;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == REQUEST_getseqidsfromgi) {
      choice = ID1serverRequest_getseqidsfromgi;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ID1SERVER_REQUEST_getgihist) {
      choice = ID1serverRequest_getgihist;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ID1SERVER_REQUEST_getgirev) {
      choice = ID1serverRequest_getgirev;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ID1SERVER_REQUEST_getgistate) {
      choice = ID1serverRequest_getgistate;
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
*    ID1serverRequestAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID1serverRequestAsnWrite(ID1serverRequestPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id1genAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ID1SERVER_REQUEST);   /* link local tree */
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
   case ID1serverRequest_init:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, ID1SERVER_REQUEST_init, &av);
      break;
   case ID1serverRequest_getgi:
      writetype = ID1SERVER_REQUEST_getgi;
      func = (AsnWriteFunc) SeqIdAsnWrite;
      break;
   case ID1serverRequest_getsefromgi:
      writetype = ID1SERVER_REQUEST_getsefromgi;
      func = (AsnWriteFunc) ID1serverMaxcomplexAsnWrite;
      break;
   case ID1serverRequest_fini:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, ID1SERVER_REQUEST_fini, &av);
      break;
   case ID1serverRequest_getseqidsfromgi:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, REQUEST_getseqidsfromgi, &av);
      break;
   case ID1serverRequest_getgihist:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ID1SERVER_REQUEST_getgihist, &av);
      break;
   case ID1serverRequest_getgirev:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ID1SERVER_REQUEST_getgirev, &av);
      break;
   case ID1serverRequest_getgistate:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ID1SERVER_REQUEST_getgistate, &av);
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
*    ID1serverMaxcomplexNew()
*
**************************************************/
NLM_EXTERN 
ID1serverMaxcomplexPtr LIBCALL
ID1serverMaxcomplexNew(void)
{
   ID1serverMaxcomplexPtr ptr = MemNew((size_t) sizeof(ID1serverMaxcomplex));

   return ptr;

}


/**************************************************
*
*    ID1serverMaxcomplexFree()
*
**************************************************/
NLM_EXTERN 
ID1serverMaxcomplexPtr LIBCALL
ID1serverMaxcomplexFree(ID1serverMaxcomplexPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> sat);
   return MemFree(ptr);
}


/**************************************************
*
*    ID1serverMaxcomplexAsnRead()
*
**************************************************/
NLM_EXTERN 
ID1serverMaxcomplexPtr LIBCALL
ID1serverMaxcomplexAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID1serverMaxcomplexPtr ptr;

   if (! loaded)
   {
      if (! id1genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID1serverMaxcomplex ::= (self contained) */
      atp = AsnReadId(aip, amp, ID1SERVER_MAXCOMPLEX);
   } else {
      atp = AsnLinkType(orig, ID1SERVER_MAXCOMPLEX);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID1serverMaxcomplexNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID1SERVER_MAXCOMPLEX_maxplex) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> maxplex = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID1SERVER_MAXCOMPLEX_gi) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gi = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID1SERVER_MAXCOMPLEX_ent) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ent = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID1SERVER_MAXCOMPLEX_sat) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sat = av.ptrvalue;
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
   ptr = ID1serverMaxcomplexFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID1serverMaxcomplexAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID1serverMaxcomplexAsnWrite(ID1serverMaxcomplexPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id1genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID1SERVER_MAXCOMPLEX);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> maxplex;
   retval = AsnWrite(aip, ID1SERVER_MAXCOMPLEX_maxplex,  &av);
   av.intvalue = ptr -> gi;
   retval = AsnWrite(aip, ID1SERVER_MAXCOMPLEX_gi,  &av);
   av.intvalue = ptr -> ent;
   retval = AsnWrite(aip, ID1SERVER_MAXCOMPLEX_ent,  &av);
   if (ptr -> sat != NULL) {
      av.ptrvalue = ptr -> sat;
      retval = AsnWrite(aip, ID1SERVER_MAXCOMPLEX_sat,  &av);
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
*    ID1SeqHistNew()
*
**************************************************/
NLM_EXTERN 
ID1SeqHistPtr LIBCALL
ID1SeqHistNew(void)
{
   ID1SeqHistPtr ptr = MemNew((size_t) sizeof(ID1SeqHist));

   return ptr;

}


/**************************************************
*
*    ID1SeqHistFree()
*
**************************************************/
NLM_EXTERN 
ID1SeqHistPtr LIBCALL
ID1SeqHistFree(ID1SeqHistPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SeqHistFree(ptr -> hist);
   return MemFree(ptr);
}


/**************************************************
*
*    ID1SeqHistAsnRead()
*
**************************************************/
NLM_EXTERN 
ID1SeqHistPtr LIBCALL
ID1SeqHistAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID1SeqHistPtr ptr;

   if (! loaded)
   {
      if (! id1genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID1SeqHist ::= (self contained) */
      atp = AsnReadId(aip, amp, ID1SEQ_HIST);
   } else {
      atp = AsnLinkType(orig, ID1SEQ_HIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID1SeqHistNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID1SEQ_HIST_hist) {
      ptr -> hist = SeqHistAsnRead(aip, atp);
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
   ptr = ID1SeqHistFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID1SeqHistAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID1SeqHistAsnWrite(ID1SeqHistPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id1genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID1SEQ_HIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> hist != NULL) {
      if ( ! SeqHistAsnWrite(ptr -> hist, aip, ID1SEQ_HIST_hist)) {
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
*    ID1serverBackFree()
*
**************************************************/
NLM_EXTERN 
ID1serverBackPtr LIBCALL
ID1serverBackFree(ValNodePtr anp)
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
   case ID1serverBack_gotseqentry:
      SeqEntryFree(anp -> data.ptrvalue);
      break;
   case ID1serverBack_gotdeadseqentry:
      SeqEntryFree(anp -> data.ptrvalue);
      break;
   case ID1serverBack_ids:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqIdFree);
      break;
   case ID1serverBack_gihist:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) ID1SeqHistFree);
      break;
   case ID1serverBack_girevhist:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) ID1SeqHistFree);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ID1serverBackAsnRead()
*
**************************************************/
NLM_EXTERN 
ID1serverBackPtr LIBCALL
ID1serverBackAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! id1genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID1serverBack ::= (self contained) */
      atp = AsnReadId(aip, amp, ID1SERVER_BACK);
   } else {
      atp = AsnLinkType(orig, ID1SERVER_BACK);    /* link in local tree */
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
   if (atp == ID1SERVER_BACK_init) {
      choice = ID1serverBack_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == ID1SERVER_BACK_error) {
      choice = ID1serverBack_error;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ID1SERVER_BACK_gotgi) {
      choice = ID1serverBack_gotgi;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ID1SERVER_BACK_gotseqentry) {
      choice = ID1serverBack_gotseqentry;
      func = (AsnReadFunc) SeqEntryAsnRead;
   }
   else if (atp == ID1SERVER_BACK_gotdeadseqentry) {
      choice = ID1serverBack_gotdeadseqentry;
      func = (AsnReadFunc) SeqEntryAsnRead;
   }
   else if (atp == ID1SERVER_BACK_fini) {
      choice = ID1serverBack_fini;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == ID1SERVER_BACK_gistate) {
      choice = ID1serverBack_gistate;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ID1SERVER_BACK_ids) {
      choice = ID1serverBack_ids;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqIdAsnRead,             (AsnOptFreeFunc) SeqIdFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == ID1SERVER_BACK_gihist) {
      choice = ID1serverBack_gihist;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID1SeqHistAsnRead,             (AsnOptFreeFunc) ID1SeqHistFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == ID1SERVER_BACK_girevhist) {
      choice = ID1serverBack_girevhist;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID1SeqHistAsnRead,             (AsnOptFreeFunc) ID1SeqHistFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
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
*    ID1serverBackAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID1serverBackAsnWrite(ID1serverBackPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id1genAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ID1SERVER_BACK);   /* link local tree */
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
   case ID1serverBack_init:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, ID1SERVER_BACK_init, &av);
      break;
   case ID1serverBack_error:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ID1SERVER_BACK_error, &av);
      break;
   case ID1serverBack_gotgi:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ID1SERVER_BACK_gotgi, &av);
      break;
   case ID1serverBack_gotseqentry:
      writetype = ID1SERVER_BACK_gotseqentry;
      func = (AsnWriteFunc) SeqEntryAsnWrite;
      break;
   case ID1serverBack_gotdeadseqentry:
      writetype = ID1SERVER_BACK_gotdeadseqentry;
      func = (AsnWriteFunc) SeqEntryAsnWrite;
      break;
   case ID1serverBack_fini:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, ID1SERVER_BACK_fini, &av);
      break;
   case ID1serverBack_gistate:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ID1SERVER_BACK_gistate, &av);
      break;
   case ID1serverBack_ids:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqIdAsnWrite, aip, ID1SERVER_BACK_ids, ID1SERVER_BACK_ids_E);
      break;
   case ID1serverBack_gihist:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) ID1SeqHistAsnWrite, aip, ID1SERVER_BACK_gihist, ID1SERVER_BACK_gihist_E);
      break;
   case ID1serverBack_girevhist:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) ID1SeqHistAsnWrite, aip, ID1SERVER_BACK_girevhist, ID1SERVER_BACK_girevhist_E);
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
*    ID1serverDebugFree()
*
**************************************************/
NLM_EXTERN 
ID1serverDebugPtr LIBCALL
ID1serverDebugFree(ID1serverDebugPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr, (AsnOptFreeFunc) ID1serverBackFree);
   return NULL;
}


/**************************************************
*
*    ID1serverDebugAsnRead()
*
**************************************************/
NLM_EXTERN 
ID1serverDebugPtr LIBCALL
ID1serverDebugAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID1serverDebugPtr ptr;

   if (! loaded)
   {
      if (! id1genAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID1serverDebug ::= (self contained) */
      atp = AsnReadId(aip, amp, ID1SERVER_DEBUG);
   } else {
      atp = AsnLinkType(orig, ID1SERVER_DEBUG);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID1serverBackAsnRead, (AsnOptFreeFunc) ID1serverBackFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ID1serverDebugFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID1serverDebugAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID1serverDebugAsnWrite(ID1serverDebugPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{

   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id1genAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID1SERVER_DEBUG);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericChoiceSeqOfAsnWrite(ptr , (AsnWriteFunc) ID1serverBackAsnWrite, aip, atp, ID1SERVER_DEBUG_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

