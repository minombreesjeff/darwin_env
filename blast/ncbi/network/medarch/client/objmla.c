#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <mapmla.h>
#include <objmla.h>

static Boolean loaded = FALSE;

#include <asnmla.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objmlaAsnLoad(void)
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
*    Generated object loaders for Module NCBI-MedArchive
*    Generated using ASNCODE Revision: 5.2 at Feb 6, 1997  4:41 PM
*
**************************************************/


/**************************************************
*
*    MlaRequestFree()
*
**************************************************/
NLM_EXTERN 
MlaRequestPtr LIBCALL
MlaRequestFree(ValNodePtr anp)
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
   case MlaRequest_gettitle:
      TitleMsgFree(anp -> data.ptrvalue);
      break;
   case MlaRequest_citmatch:
      PubFree(anp -> data.ptrvalue);
      break;
   case MlaRequest_getaccuids:
      MedlineSiFree(anp -> data.ptrvalue);
      break;
   case MlaRequest_citmatchpmid:
      PubFree(anp -> data.ptrvalue);
      break;
   case MlaRequest_getaccpmids:
      MedlineSiFree(anp -> data.ptrvalue);
      break;
   case MlaRequest_citlstpmids:
      PubFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    MlaRequestAsnRead()
*
**************************************************/
NLM_EXTERN 
MlaRequestPtr LIBCALL
MlaRequestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmlaAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MlaRequest ::= (self contained) */
      atp = AsnReadId(aip, amp, MLA_REQUEST);
   } else {
      atp = AsnLinkType(orig, MLA_REQUEST);    /* link in local tree */
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
   if (atp == MLA_REQUEST_init) {
      choice = MlaRequest_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == MLA_REQUEST_getmle) {
      choice = MlaRequest_getmle;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_REQUEST_getpub) {
      choice = MlaRequest_getpub;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_REQUEST_gettitle) {
      choice = MlaRequest_gettitle;
      func = (AsnReadFunc) TitleMsgAsnRead;
   }
   else if (atp == MLA_REQUEST_citmatch) {
      choice = MlaRequest_citmatch;
      func = (AsnReadFunc) PubAsnRead;
   }
   else if (atp == MLA_REQUEST_fini) {
      choice = MlaRequest_fini;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == MLA_REQUEST_getmriuids) {
      choice = MlaRequest_getmriuids;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_REQUEST_getaccuids) {
      choice = MlaRequest_getaccuids;
      func = (AsnReadFunc) MedlineSiAsnRead;
   }
   else if (atp == MLA_REQUEST_uidtopmid) {
      choice = MlaRequest_uidtopmid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_REQUEST_pmidtouid) {
      choice = MlaRequest_pmidtouid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_REQUEST_getmlepmid) {
      choice = MlaRequest_getmlepmid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_REQUEST_getpubpmid) {
      choice = MlaRequest_getpubpmid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_REQUEST_citmatchpmid) {
      choice = MlaRequest_citmatchpmid;
      func = (AsnReadFunc) PubAsnRead;
   }
   else if (atp == MLA_REQUEST_getmripmids) {
      choice = MlaRequest_getmripmids;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_REQUEST_getaccpmids) {
      choice = MlaRequest_getaccpmids;
      func = (AsnReadFunc) MedlineSiAsnRead;
   }
   else if (atp == MLA_REQUEST_citlstpmids) {
      choice = MlaRequest_citlstpmids;
      func = (AsnReadFunc) PubAsnRead;
   }
   else if (atp == MLA_REQUEST_getmleuid) {
      choice = MlaRequest_getmleuid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_REQUEST_getmlrpmid) {
      choice = MlaRequest_getmlrpmid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_REQUEST_getmlruid) {
      choice = MlaRequest_getmlruid;
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
*    MlaRequestAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MlaRequestAsnWrite(MlaRequestPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmlaAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, MLA_REQUEST);   /* link local tree */
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
   case MlaRequest_init:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, MLA_REQUEST_init, &av);
      break;
   case MlaRequest_getmle:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_REQUEST_getmle, &av);
      break;
   case MlaRequest_getpub:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_REQUEST_getpub, &av);
      break;
   case MlaRequest_gettitle:
      writetype = MLA_REQUEST_gettitle;
      func = (AsnWriteFunc) TitleMsgAsnWrite;
      break;
   case MlaRequest_citmatch:
      writetype = MLA_REQUEST_citmatch;
      func = (AsnWriteFunc) PubAsnWrite;
      break;
   case MlaRequest_fini:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, MLA_REQUEST_fini, &av);
      break;
   case MlaRequest_getmriuids:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_REQUEST_getmriuids, &av);
      break;
   case MlaRequest_getaccuids:
      writetype = MLA_REQUEST_getaccuids;
      func = (AsnWriteFunc) MedlineSiAsnWrite;
      break;
   case MlaRequest_uidtopmid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_REQUEST_uidtopmid, &av);
      break;
   case MlaRequest_pmidtouid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_REQUEST_pmidtouid, &av);
      break;
   case MlaRequest_getmlepmid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_REQUEST_getmlepmid, &av);
      break;
   case MlaRequest_getpubpmid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_REQUEST_getpubpmid, &av);
      break;
   case MlaRequest_citmatchpmid:
      writetype = MLA_REQUEST_citmatchpmid;
      func = (AsnWriteFunc) PubAsnWrite;
      break;
   case MlaRequest_getmripmids:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_REQUEST_getmripmids, &av);
      break;
   case MlaRequest_getaccpmids:
      writetype = MLA_REQUEST_getaccpmids;
      func = (AsnWriteFunc) MedlineSiAsnWrite;
      break;
   case MlaRequest_citlstpmids:
      writetype = MLA_REQUEST_citlstpmids;
      func = (AsnWriteFunc) PubAsnWrite;
      break;
   case MlaRequest_getmleuid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_REQUEST_getmleuid, &av);
      break;
   case MlaRequest_getmlrpmid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_REQUEST_getmlrpmid, &av);
      break;
   case MlaRequest_getmlruid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_REQUEST_getmlruid, &av);
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
*    TitleMsgNew()
*
**************************************************/
NLM_EXTERN 
TitleMsgPtr LIBCALL
TitleMsgNew(void)
{
   TitleMsgPtr ptr = MemNew((size_t) sizeof(TitleMsg));

   return ptr;

}


/**************************************************
*
*    TitleMsgFree()
*
**************************************************/
NLM_EXTERN 
TitleMsgPtr LIBCALL
TitleMsgFree(TitleMsgPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   TitleFree(ptr -> title);
   return MemFree(ptr);
}


/**************************************************
*
*    TitleMsgAsnRead()
*
**************************************************/
NLM_EXTERN 
TitleMsgPtr LIBCALL
TitleMsgAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TitleMsgPtr ptr;

   if (! loaded)
   {
      if (! objmlaAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TitleMsg ::= (self contained) */
      atp = AsnReadId(aip, amp, TITLE_MSG);
   } else {
      atp = AsnLinkType(orig, TITLE_MSG);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TitleMsgNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TITLE_MSG_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TITLE_MSG_title) {
      ptr -> title = TitleAsnRead(aip, atp);
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
   ptr = TitleMsgFree(ptr);
   goto ret;
}



/**************************************************
*
*    TitleMsgAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TitleMsgAsnWrite(TitleMsgPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmlaAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TITLE_MSG);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, TITLE_MSG_type,  &av);
   if (ptr -> title != NULL) {
      if ( ! TitleAsnWrite(ptr -> title, aip, TITLE_MSG_title)) {
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
*    TitleMsgListNew()
*
**************************************************/
NLM_EXTERN 
TitleMsgListPtr LIBCALL
TitleMsgListNew(void)
{
   TitleMsgListPtr ptr = MemNew((size_t) sizeof(TitleMsgList));

   return ptr;

}


/**************************************************
*
*    TitleMsgListFree()
*
**************************************************/
NLM_EXTERN 
TitleMsgListPtr LIBCALL
TitleMsgListFree(TitleMsgListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> titles, (AsnOptFreeFunc) TitleMsgFree);
   return MemFree(ptr);
}


/**************************************************
*
*    TitleMsgListAsnRead()
*
**************************************************/
NLM_EXTERN 
TitleMsgListPtr LIBCALL
TitleMsgListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TitleMsgListPtr ptr;

   if (! loaded)
   {
      if (! objmlaAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* TitleMsgList ::= (self contained) */
      atp = AsnReadId(aip, amp, TITLE_MSG_LIST);
   } else {
      atp = AsnLinkType(orig, TITLE_MSG_LIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TitleMsgListNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TITLE_MSG_LIST_num) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> num = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TITLE_MSG_LIST_titles) {
      ptr -> titles = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) TitleMsgAsnRead, (AsnOptFreeFunc) TitleMsgFree);
      if (isError && ptr -> titles == NULL) {
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
   ptr = TitleMsgListFree(ptr);
   goto ret;
}



/**************************************************
*
*    TitleMsgListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TitleMsgListAsnWrite(TitleMsgListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmlaAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TITLE_MSG_LIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> num;
   retval = AsnWrite(aip, TITLE_MSG_LIST_num,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> titles, (AsnWriteFunc) TitleMsgAsnWrite, aip, TITLE_MSG_LIST_titles, TITLE_MSG_LIST_titles_E);
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
*    MlaBackFree()
*
**************************************************/
NLM_EXTERN 
MlaBackPtr LIBCALL
MlaBackFree(ValNodePtr anp)
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
   case MlaBack_getmle:
      MedlineEntryFree(anp -> data.ptrvalue);
      break;
   case MlaBack_getpub:
      PubFree(anp -> data.ptrvalue);
      break;
   case MlaBack_gettitle:
      TitleMsgListFree(anp -> data.ptrvalue);
      break;
   case MlaBack_getuids:
      AsnGenericBaseSeqOfFree((ValNodePtr) pnt,ASNCODE_INTVAL_SLOT);
      break;
   case MlaBack_getpmids:
      AsnGenericBaseSeqOfFree((ValNodePtr) pnt,ASNCODE_INTVAL_SLOT);
      break;
   case MlaBack_getpme:
      PubmedEntryFree(anp -> data.ptrvalue);
      break;
   case MlaBack_getmlr:
      MedlarsEntryFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    MlaBackAsnRead()
*
**************************************************/
NLM_EXTERN 
MlaBackPtr LIBCALL
MlaBackAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmlaAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MlaBack ::= (self contained) */
      atp = AsnReadId(aip, amp, MLA_BACK);
   } else {
      atp = AsnLinkType(orig, MLA_BACK);    /* link in local tree */
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
   if (atp == MLA_BACK_init) {
      choice = MlaBack_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == MLA_BACK_error) {
      choice = MlaBack_error;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_BACK_getmle) {
      choice = MlaBack_getmle;
      func = (AsnReadFunc) MedlineEntryAsnRead;
   }
   else if (atp == MLA_BACK_getpub) {
      choice = MlaBack_getpub;
      func = (AsnReadFunc) PubAsnRead;
   }
   else if (atp == MLA_BACK_gettitle) {
      choice = MlaBack_gettitle;
      func = (AsnReadFunc) TitleMsgListAsnRead;
   }
   else if (atp == MLA_BACK_citmatch) {
      choice = MlaBack_citmatch;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_BACK_fini) {
      choice = MlaBack_fini;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == MLA_BACK_getuids) {
      choice = MlaBack_getuids;
      anp -> data.ptrvalue = 
      AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == MLA_BACK_getpmids) {
      choice = MlaBack_getpmids;
      anp -> data.ptrvalue = 
      AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == MLA_BACK_outuid) {
      choice = MlaBack_outuid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_BACK_outpmid) {
      choice = MlaBack_outpmid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == MLA_BACK_getpme) {
      choice = MlaBack_getpme;
      func = (AsnReadFunc) PubmedEntryAsnRead;
   }
   else if (atp == MLA_BACK_getmlr) {
      choice = MlaBack_getmlr;
      func = (AsnReadFunc) MedlarsEntryAsnRead;
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
*    MlaBackAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MlaBackAsnWrite(MlaBackPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmlaAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, MLA_BACK);   /* link local tree */
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
   case MlaBack_init:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, MLA_BACK_init, &av);
      break;
   case MlaBack_error:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_BACK_error, &av);
      break;
   case MlaBack_getmle:
      writetype = MLA_BACK_getmle;
      func = (AsnWriteFunc) MedlineEntryAsnWrite;
      break;
   case MlaBack_getpub:
      writetype = MLA_BACK_getpub;
      func = (AsnWriteFunc) PubAsnWrite;
      break;
   case MlaBack_gettitle:
      writetype = MLA_BACK_gettitle;
      func = (AsnWriteFunc) TitleMsgListAsnWrite;
      break;
   case MlaBack_citmatch:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_BACK_citmatch, &av);
      break;
   case MlaBack_fini:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, MLA_BACK_fini, &av);
      break;
   case MlaBack_getuids:
      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,ASNCODE_INTVAL_SLOT, aip, MLA_BACK_getuids, MLA_BACK_getuids_E);            break;
   case MlaBack_getpmids:
      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,ASNCODE_INTVAL_SLOT, aip, MLA_BACK_getpmids, MLA_BACK_getpmids_E);            break;
   case MlaBack_outuid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_BACK_outuid, &av);
      break;
   case MlaBack_outpmid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MLA_BACK_outpmid, &av);
      break;
   case MlaBack_getpme:
      writetype = MLA_BACK_getpme;
      func = (AsnWriteFunc) PubmedEntryAsnWrite;
      break;
   case MlaBack_getmlr:
      writetype = MLA_BACK_getmlr;
      func = (AsnWriteFunc) MedlarsEntryAsnWrite;
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
