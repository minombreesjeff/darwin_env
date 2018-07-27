#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <objent2.h>

static Boolean loaded = FALSE;

#include <asnent2.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objent2AsnLoad(void)
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
*    Generated object loaders for Module NCBI-Entrez2
*    Generated using ASNCODE Revision: 6.13 at Jan 28, 2002 12:12 PM
*    Manual addition to swap bytes in id list if IS_LITTLE_ENDIAN
*
**************************************************/


/**************************************************
*
*    Entrez2IdListNew()
*
**************************************************/
NLM_EXTERN 
Entrez2IdListPtr LIBCALL
Entrez2IdListNew(void)
{
   Entrez2IdListPtr ptr = MemNew((size_t) sizeof(Entrez2IdList));

   return ptr;

}


/**************************************************
*
*    Entrez2IdListFree()
*
**************************************************/
NLM_EXTERN 
Entrez2IdListPtr LIBCALL
Entrez2IdListFree(Entrez2IdListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> db);
   BSFree(ptr -> uids);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2IdListAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2IdListPtr LIBCALL
Entrez2IdListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2IdListPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2IdList ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_ID_LIST);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_ID_LIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2IdListNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_ID_LIST_db) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_ID_LIST_num) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> num = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_ID_LIST_uids) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      /* ptr -> uids = av.ptrvalue; */

      /* manually added */
      ptr -> uids = BSDupAndSwapUint4 ((ByteStorePtr) av.ptrvalue);
      BSFree ((ByteStorePtr) av.ptrvalue);

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
   ptr = Entrez2IdListFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2IdListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2IdListAsnWrite(Entrez2IdListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_ID_LIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> db != NULL) {
      av.ptrvalue = ptr -> db;
      retval = AsnWrite(aip, ENTREZ2_ID_LIST_db,  &av);
   }
   av.intvalue = ptr -> num;
   retval = AsnWrite(aip, ENTREZ2_ID_LIST_num,  &av);
   if (ptr -> uids != NULL) {
      /* av.ptrvalue = ptr -> uids; */

      /* manually added */
      av.ptrvalue = BSDupAndSwapUint4 ((ByteStorePtr) ptr -> uids);

      retval = AsnWrite(aip, ENTREZ2_ID_LIST_uids,  &av);

      /* manually added */
      BSFree ((ByteStorePtr) av.ptrvalue);
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
*    Entrez2BooleanExpNew()
*
**************************************************/
NLM_EXTERN 
Entrez2BooleanExpPtr LIBCALL
Entrez2BooleanExpNew(void)
{
   Entrez2BooleanExpPtr ptr = MemNew((size_t) sizeof(Entrez2BooleanExp));

   return ptr;

}


/**************************************************
*
*    Entrez2BooleanExpFree()
*
**************************************************/
NLM_EXTERN 
Entrez2BooleanExpPtr LIBCALL
Entrez2BooleanExpFree(Entrez2BooleanExpPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> db);
   AsnGenericChoiceSeqOfFree(ptr -> exp, (AsnOptFreeFunc) Entrez2BooleanElementFree);
   Entrez2LimitsFree(ptr -> limits);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2BooleanExpAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2BooleanExpPtr LIBCALL
Entrez2BooleanExpAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2BooleanExpPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2BooleanExp ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_BOOLEAN_EXP);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_BOOLEAN_EXP);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2BooleanExpNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_BOOLEAN_EXP_db) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_BOOLEAN_EXP_exp) {
      ptr -> exp = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Entrez2BooleanElementAsnRead, (AsnOptFreeFunc) Entrez2BooleanElementFree);
      if (isError && ptr -> exp == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_BOOLEAN_EXP_limits) {
      ptr -> limits = Entrez2LimitsAsnRead(aip, atp);
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
   ptr = Entrez2BooleanExpFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2BooleanExpAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2BooleanExpAsnWrite(Entrez2BooleanExpPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_BOOLEAN_EXP);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> db != NULL) {
      av.ptrvalue = ptr -> db;
      retval = AsnWrite(aip, ENTREZ2_BOOLEAN_EXP_db,  &av);
   }
   AsnGenericChoiceSeqOfAsnWrite(ptr -> exp, (AsnWriteFunc) Entrez2BooleanElementAsnWrite, aip, ENTREZ2_BOOLEAN_EXP_exp, ENTREZ2_BOOLEAN_EXP_exp_E);
   if (ptr -> limits != NULL) {
      if ( ! Entrez2LimitsAsnWrite(ptr -> limits, aip, ENTREZ2_BOOLEAN_EXP_limits)) {
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
*    Entrez2BooleanElementFree()
*
**************************************************/
NLM_EXTERN 
Entrez2BooleanElementPtr LIBCALL
Entrez2BooleanElementFree(ValNodePtr anp)
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
   case Entrez2BooleanElement_str:
      MemFree(anp -> data.ptrvalue);
      break;
   case Entrez2BooleanElement_term:
      Entrez2BooleanTermFree(anp -> data.ptrvalue);
      break;
   case Entrez2BooleanElement_ids:
      Entrez2IdListFree(anp -> data.ptrvalue);
      break;
   case Entrez2BooleanElement_key:
      MemFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    Entrez2BooleanElementAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2BooleanElementPtr LIBCALL
Entrez2BooleanElementAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2BooleanElement ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_BOOLEAN_ELEMENT);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_BOOLEAN_ELEMENT);    /* link in local tree */
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
   if (atp == ENTREZ2_BOOLEAN_ELEMENT_str) {
      choice = Entrez2BooleanElement_str;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == ENTREZ2_BOOLEAN_ELEMENT_op) {
      choice = Entrez2BooleanElement_op;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ENTREZ2_BOOLEAN_ELEMENT_term) {
      choice = Entrez2BooleanElement_term;
      func = (AsnReadFunc) Entrez2BooleanTermAsnRead;
   }
   else if (atp == ENTREZ2_BOOLEAN_ELEMENT_ids) {
      choice = Entrez2BooleanElement_ids;
      func = (AsnReadFunc) Entrez2IdListAsnRead;
   }
   else if (atp == ENTREZ2_BOOLEAN_ELEMENT_key) {
      choice = Entrez2BooleanElement_key;
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
*    Entrez2BooleanElementAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2BooleanElementAsnWrite(Entrez2BooleanElementPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ENTREZ2_BOOLEAN_ELEMENT);   /* link local tree */
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
   case Entrez2BooleanElement_str:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, ENTREZ2_BOOLEAN_ELEMENT_str, &av);
      break;
   case Entrez2BooleanElement_op:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ENTREZ2_BOOLEAN_ELEMENT_op, &av);
      break;
   case Entrez2BooleanElement_term:
      writetype = ENTREZ2_BOOLEAN_ELEMENT_term;
      func = (AsnWriteFunc) Entrez2BooleanTermAsnWrite;
      break;
   case Entrez2BooleanElement_ids:
      writetype = ENTREZ2_BOOLEAN_ELEMENT_ids;
      func = (AsnWriteFunc) Entrez2IdListAsnWrite;
      break;
   case Entrez2BooleanElement_key:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, ENTREZ2_BOOLEAN_ELEMENT_key, &av);
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
*    Entrez2LimitsNew()
*
**************************************************/
NLM_EXTERN 
Entrez2LimitsPtr LIBCALL
Entrez2LimitsNew(void)
{
   Entrez2LimitsPtr ptr = MemNew((size_t) sizeof(Entrez2Limits));

   return ptr;

}


/**************************************************
*
*    Entrez2LimitsFree()
*
**************************************************/
NLM_EXTERN 
Entrez2LimitsPtr LIBCALL
Entrez2LimitsFree(Entrez2LimitsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Entrez2DtFilterFree(ptr -> filter_date);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2LimitsAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2LimitsPtr LIBCALL
Entrez2LimitsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2LimitsPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2Limits ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_LIMITS);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_LIMITS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2LimitsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_LIMITS_filter_date) {
      ptr -> filter_date = Entrez2DtFilterAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_LIMITS_max_UIDs) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> max_UIDs = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_LIMITS_offset_UIDs) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> offset_UIDs = av.intvalue;
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
   ptr = Entrez2LimitsFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2LimitsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2LimitsAsnWrite(Entrez2LimitsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_LIMITS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> filter_date != NULL) {
      if ( ! Entrez2DtFilterAsnWrite(ptr -> filter_date, aip, ENTREZ2_LIMITS_filter_date)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> max_UIDs;
   retval = AsnWrite(aip, ENTREZ2_LIMITS_max_UIDs,  &av);
   av.intvalue = ptr -> offset_UIDs;
   retval = AsnWrite(aip, ENTREZ2_LIMITS_offset_UIDs,  &av);
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
*    Entrez2BooleanTermNew()
*
**************************************************/
NLM_EXTERN 
Entrez2BooleanTermPtr LIBCALL
Entrez2BooleanTermNew(void)
{
   Entrez2BooleanTermPtr ptr = MemNew((size_t) sizeof(Entrez2BooleanTerm));

   ptr -> do_not_explode = 0;
   ptr -> do_not_translate = 0;
   return ptr;

}


/**************************************************
*
*    Entrez2BooleanTermFree()
*
**************************************************/
NLM_EXTERN 
Entrez2BooleanTermPtr LIBCALL
Entrez2BooleanTermFree(Entrez2BooleanTermPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> field);
   MemFree(ptr -> term);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2BooleanTermAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2BooleanTermPtr LIBCALL
Entrez2BooleanTermAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2BooleanTermPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2BooleanTerm ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_BOOLEAN_TERM);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_BOOLEAN_TERM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2BooleanTermNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_BOOLEAN_TERM_field) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_BOOLEAN_TERM_term) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> term = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_BOOLEAN_TERM_term_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> term_count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BOOLEAN_TERM_do_not_explode) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> do_not_explode = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BOOLEAN_TERM_do_not_translate) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> do_not_translate = av.boolvalue;
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
   ptr = Entrez2BooleanTermFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2BooleanTermAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2BooleanTermAsnWrite(Entrez2BooleanTermPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_BOOLEAN_TERM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> field != NULL) {
      av.ptrvalue = ptr -> field;
      retval = AsnWrite(aip, ENTREZ2_BOOLEAN_TERM_field,  &av);
   }
   if (ptr -> term != NULL) {
      av.ptrvalue = ptr -> term;
      retval = AsnWrite(aip, ENTREZ2_BOOLEAN_TERM_term,  &av);
   }
   av.intvalue = ptr -> term_count;
   retval = AsnWrite(aip, ENTREZ2_BOOLEAN_TERM_term_count,  &av);
   av.boolvalue = ptr -> do_not_explode;
   retval = AsnWrite(aip, BOOLEAN_TERM_do_not_explode,  &av);
   av.boolvalue = ptr -> do_not_translate;
   retval = AsnWrite(aip, BOOLEAN_TERM_do_not_translate,  &av);
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
*    Entrez2RequestNew()
*
**************************************************/
NLM_EXTERN 
Entrez2RequestPtr LIBCALL
Entrez2RequestNew(void)
{
   Entrez2RequestPtr ptr = MemNew((size_t) sizeof(Entrez2Request));

   ptr -> use_history = 0;
   return ptr;

}


/**************************************************
*
*    Entrez2RequestFree()
*
**************************************************/
NLM_EXTERN 
Entrez2RequestPtr LIBCALL
Entrez2RequestFree(Entrez2RequestPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   E2RequestFree(ptr -> request);
   MemFree(ptr -> tool);
   MemFree(ptr -> cookie);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2RequestAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2RequestPtr LIBCALL
Entrez2RequestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2RequestPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2Request ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_REQUEST);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_REQUEST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2RequestNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_REQUEST_request) {
      ptr -> request = E2RequestAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_REQUEST_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> version = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_REQUEST_tool) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tool = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_REQUEST_cookie) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> cookie = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_REQUEST_use_history) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> use_history = av.boolvalue;
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
   ptr = Entrez2RequestFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2RequestAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2RequestAsnWrite(Entrez2RequestPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_REQUEST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> request != NULL) {
      if ( ! E2RequestAsnWrite(ptr -> request, aip, ENTREZ2_REQUEST_request)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> version;
   retval = AsnWrite(aip, ENTREZ2_REQUEST_version,  &av);
   if (ptr -> tool != NULL) {
      av.ptrvalue = ptr -> tool;
      retval = AsnWrite(aip, ENTREZ2_REQUEST_tool,  &av);
   }
   if (ptr -> cookie != NULL) {
      av.ptrvalue = ptr -> cookie;
      retval = AsnWrite(aip, ENTREZ2_REQUEST_cookie,  &av);
   }
   av.boolvalue = ptr -> use_history;
   if (ptr -> use_history) { /* temporary until server has new spec */
     retval = AsnWrite(aip, ENTREZ2_REQUEST_use_history,  &av);
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
*    E2RequestFree()
*
**************************************************/
NLM_EXTERN 
E2RequestPtr LIBCALL
E2RequestFree(ValNodePtr anp)
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
   case E2Request_eval_boolean:
      Entrez2EvalBooleanFree(anp -> data.ptrvalue);
      break;
   case E2Request_get_docsum:
      Entrez2IdListFree(anp -> data.ptrvalue);
      break;
   case E2Request_get_term_pos:
      Entrez2TermQueryFree(anp -> data.ptrvalue);
      break;
   case E2Request_get_term_list:
      Entrez2TermPosFree(anp -> data.ptrvalue);
      break;
   case E2Request_get_term_hierarchy:
      Entrez2HierQueryFree(anp -> data.ptrvalue);
      break;
   case E2Request_get_links:
      Entrez2GetLinksFree(anp -> data.ptrvalue);
      break;
   case E2Request_get_linked:
      Entrez2GetLinksFree(anp -> data.ptrvalue);
      break;
   case E2Request_get_link_counts:
      Entrez2IdFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    E2RequestAsnRead()
*
**************************************************/
NLM_EXTERN 
E2RequestPtr LIBCALL
E2RequestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* E2Request ::= (self contained) */
      atp = AsnReadId(aip, amp, E2REQUEST);
   } else {
      atp = AsnLinkType(orig, E2REQUEST);    /* link in local tree */
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
   if (atp == E2REQUEST_get_info) {
      choice = E2Request_get_info;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == E2REQUEST_eval_boolean) {
      choice = E2Request_eval_boolean;
      func = (AsnReadFunc) Entrez2EvalBooleanAsnRead;
   }
   else if (atp == E2REQUEST_get_docsum) {
      choice = E2Request_get_docsum;
      func = (AsnReadFunc) Entrez2IdListAsnRead;
   }
   else if (atp == E2REQUEST_get_term_pos) {
      choice = E2Request_get_term_pos;
      func = (AsnReadFunc) Entrez2TermQueryAsnRead;
   }
   else if (atp == E2REQUEST_get_term_list) {
      choice = E2Request_get_term_list;
      func = (AsnReadFunc) Entrez2TermPosAsnRead;
   }
   else if (atp == E2REQUEST_get_term_hierarchy) {
      choice = E2Request_get_term_hierarchy;
      func = (AsnReadFunc) Entrez2HierQueryAsnRead;
   }
   else if (atp == E2REQUEST_get_links) {
      choice = E2Request_get_links;
      func = (AsnReadFunc) Entrez2GetLinksAsnRead;
   }
   else if (atp == E2REQUEST_get_linked) {
      choice = E2Request_get_linked;
      func = (AsnReadFunc) Entrez2GetLinksAsnRead;
   }
   else if (atp == E2REQUEST_get_link_counts) {
      choice = E2Request_get_link_counts;
      func = (AsnReadFunc) Entrez2IdAsnRead;
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
*    E2RequestAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
E2RequestAsnWrite(E2RequestPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, E2REQUEST);   /* link local tree */
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
   case E2Request_get_info:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, E2REQUEST_get_info, &av);
      break;
   case E2Request_eval_boolean:
      writetype = E2REQUEST_eval_boolean;
      func = (AsnWriteFunc) Entrez2EvalBooleanAsnWrite;
      break;
   case E2Request_get_docsum:
      writetype = E2REQUEST_get_docsum;
      func = (AsnWriteFunc) Entrez2IdListAsnWrite;
      break;
   case E2Request_get_term_pos:
      writetype = E2REQUEST_get_term_pos;
      func = (AsnWriteFunc) Entrez2TermQueryAsnWrite;
      break;
   case E2Request_get_term_list:
      writetype = E2REQUEST_get_term_list;
      func = (AsnWriteFunc) Entrez2TermPosAsnWrite;
      break;
   case E2Request_get_term_hierarchy:
      writetype = E2REQUEST_get_term_hierarchy;
      func = (AsnWriteFunc) Entrez2HierQueryAsnWrite;
      break;
   case E2Request_get_links:
      writetype = E2REQUEST_get_links;
      func = (AsnWriteFunc) Entrez2GetLinksAsnWrite;
      break;
   case E2Request_get_linked:
      writetype = E2REQUEST_get_linked;
      func = (AsnWriteFunc) Entrez2GetLinksAsnWrite;
      break;
   case E2Request_get_link_counts:
      writetype = E2REQUEST_get_link_counts;
      func = (AsnWriteFunc) Entrez2IdAsnWrite;
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
*    Entrez2EvalBooleanNew()
*
**************************************************/
NLM_EXTERN 
Entrez2EvalBooleanPtr LIBCALL
Entrez2EvalBooleanNew(void)
{
   Entrez2EvalBooleanPtr ptr = MemNew((size_t) sizeof(Entrez2EvalBoolean));

   ptr -> return_UIDs = 0;
   ptr -> return_parse = 0;
   return ptr;

}


/**************************************************
*
*    Entrez2EvalBooleanFree()
*
**************************************************/
NLM_EXTERN 
Entrez2EvalBooleanPtr LIBCALL
Entrez2EvalBooleanFree(Entrez2EvalBooleanPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Entrez2BooleanExpFree(ptr -> query);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2EvalBooleanAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2EvalBooleanPtr LIBCALL
Entrez2EvalBooleanAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2EvalBooleanPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2EvalBoolean ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_EVAL_BOOLEAN);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_EVAL_BOOLEAN);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2EvalBooleanNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == EVAL_BOOLEAN_return_UIDs) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> return_UIDs = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == EVAL_BOOLEAN_return_parse) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> return_parse = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_EVAL_BOOLEAN_query) {
      ptr -> query = Entrez2BooleanExpAsnRead(aip, atp);
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
   ptr = Entrez2EvalBooleanFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2EvalBooleanAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2EvalBooleanAsnWrite(Entrez2EvalBooleanPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_EVAL_BOOLEAN);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.boolvalue = ptr -> return_UIDs;
   retval = AsnWrite(aip, EVAL_BOOLEAN_return_UIDs,  &av);
   av.boolvalue = ptr -> return_parse;
   retval = AsnWrite(aip, EVAL_BOOLEAN_return_parse,  &av);
   if (ptr -> query != NULL) {
      if ( ! Entrez2BooleanExpAsnWrite(ptr -> query, aip, ENTREZ2_EVAL_BOOLEAN_query)) {
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
*    Entrez2TermQueryNew()
*
**************************************************/
NLM_EXTERN 
Entrez2TermQueryPtr LIBCALL
Entrez2TermQueryNew(void)
{
   Entrez2TermQueryPtr ptr = MemNew((size_t) sizeof(Entrez2TermQuery));

   return ptr;

}


/**************************************************
*
*    Entrez2TermQueryFree()
*
**************************************************/
NLM_EXTERN 
Entrez2TermQueryPtr LIBCALL
Entrez2TermQueryFree(Entrez2TermQueryPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> db);
   MemFree(ptr -> field);
   MemFree(ptr -> term);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2TermQueryAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2TermQueryPtr LIBCALL
Entrez2TermQueryAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2TermQueryPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2TermQuery ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_TERM_QUERY);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_TERM_QUERY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2TermQueryNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_TERM_QUERY_db) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_TERM_QUERY_field) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_TERM_QUERY_term) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> term = av.ptrvalue;
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
   ptr = Entrez2TermQueryFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2TermQueryAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2TermQueryAsnWrite(Entrez2TermQueryPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_TERM_QUERY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> db != NULL) {
      av.ptrvalue = ptr -> db;
      retval = AsnWrite(aip, ENTREZ2_TERM_QUERY_db,  &av);
   }
   if (ptr -> field != NULL) {
      av.ptrvalue = ptr -> field;
      retval = AsnWrite(aip, ENTREZ2_TERM_QUERY_field,  &av);
   }
   if (ptr -> term != NULL) {
      av.ptrvalue = ptr -> term;
      retval = AsnWrite(aip, ENTREZ2_TERM_QUERY_term,  &av);
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
*    Entrez2TermPosNew()
*
**************************************************/
NLM_EXTERN 
Entrez2TermPosPtr LIBCALL
Entrez2TermPosNew(void)
{
   Entrez2TermPosPtr ptr = MemNew((size_t) sizeof(Entrez2TermPos));

   return ptr;

}


/**************************************************
*
*    Entrez2TermPosFree()
*
**************************************************/
NLM_EXTERN 
Entrez2TermPosPtr LIBCALL
Entrez2TermPosFree(Entrez2TermPosPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> db);
   MemFree(ptr -> field);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2TermPosAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2TermPosPtr LIBCALL
Entrez2TermPosAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2TermPosPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2TermPos ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_TERM_POS);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_TERM_POS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2TermPosNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_TERM_POS_db) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_TERM_POS_field) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_TERM_POS_first_term_pos) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> first_term_pos = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TERM_POS_number_of_terms) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_of_terms = av.intvalue;
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
   ptr = Entrez2TermPosFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2TermPosAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2TermPosAsnWrite(Entrez2TermPosPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_TERM_POS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> db != NULL) {
      av.ptrvalue = ptr -> db;
      retval = AsnWrite(aip, ENTREZ2_TERM_POS_db,  &av);
   }
   if (ptr -> field != NULL) {
      av.ptrvalue = ptr -> field;
      retval = AsnWrite(aip, ENTREZ2_TERM_POS_field,  &av);
   }
   av.intvalue = ptr -> first_term_pos;
   retval = AsnWrite(aip, ENTREZ2_TERM_POS_first_term_pos,  &av);
   av.intvalue = ptr -> number_of_terms;
   retval = AsnWrite(aip, TERM_POS_number_of_terms,  &av);
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
*    Entrez2HierQueryNew()
*
**************************************************/
NLM_EXTERN 
Entrez2HierQueryPtr LIBCALL
Entrez2HierQueryNew(void)
{
   Entrez2HierQueryPtr ptr = MemNew((size_t) sizeof(Entrez2HierQuery));

   return ptr;

}


/**************************************************
*
*    Entrez2HierQueryFree()
*
**************************************************/
NLM_EXTERN 
Entrez2HierQueryPtr LIBCALL
Entrez2HierQueryFree(Entrez2HierQueryPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> db);
   MemFree(ptr -> field);
   MemFree(ptr -> term);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2HierQueryAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2HierQueryPtr LIBCALL
Entrez2HierQueryAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2HierQueryPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2HierQuery ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_HIER_QUERY);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_HIER_QUERY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2HierQueryNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_HIER_QUERY_db) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_HIER_QUERY_field) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_HIER_QUERY_term) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> term = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_HIER_QUERY_txid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> txid = av.intvalue;
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
   ptr = Entrez2HierQueryFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2HierQueryAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2HierQueryAsnWrite(Entrez2HierQueryPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_HIER_QUERY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> db != NULL) {
      av.ptrvalue = ptr -> db;
      retval = AsnWrite(aip, ENTREZ2_HIER_QUERY_db,  &av);
   }
   if (ptr -> field != NULL) {
      av.ptrvalue = ptr -> field;
      retval = AsnWrite(aip, ENTREZ2_HIER_QUERY_field,  &av);
   }
   if (ptr -> term != NULL) {
      av.ptrvalue = ptr -> term;
      retval = AsnWrite(aip, ENTREZ2_HIER_QUERY_term,  &av);
   }
   av.intvalue = ptr -> txid;
   retval = AsnWrite(aip, ENTREZ2_HIER_QUERY_txid,  &av);
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
*    Entrez2GetLinksNew()
*
**************************************************/
NLM_EXTERN 
Entrez2GetLinksPtr LIBCALL
Entrez2GetLinksNew(void)
{
   Entrez2GetLinksPtr ptr = MemNew((size_t) sizeof(Entrez2GetLinks));

   return ptr;

}


/**************************************************
*
*    Entrez2GetLinksFree()
*
**************************************************/
NLM_EXTERN 
Entrez2GetLinksPtr LIBCALL
Entrez2GetLinksFree(Entrez2GetLinksPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Entrez2IdListFree(ptr -> uids);
   MemFree(ptr -> linktype);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2GetLinksAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2GetLinksPtr LIBCALL
Entrez2GetLinksAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2GetLinksPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2GetLinks ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_GET_LINKS);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_GET_LINKS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2GetLinksNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_GET_LINKS_uids) {
      ptr -> uids = Entrez2IdListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_GET_LINKS_linktype) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> linktype = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_GET_LINKS_max_UIDS) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> max_UIDS = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_GET_LINKS_count_only) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> count_only = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GET_LINKS_parents_persist) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> parents_persist = av.boolvalue;
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
   ptr = Entrez2GetLinksFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2GetLinksAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2GetLinksAsnWrite(Entrez2GetLinksPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_GET_LINKS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> uids != NULL) {
      if ( ! Entrez2IdListAsnWrite(ptr -> uids, aip, ENTREZ2_GET_LINKS_uids)) {
         goto erret;
      }
   }
   if (ptr -> linktype != NULL) {
      av.ptrvalue = ptr -> linktype;
      retval = AsnWrite(aip, ENTREZ2_GET_LINKS_linktype,  &av);
   }
   av.intvalue = ptr -> max_UIDS;
   retval = AsnWrite(aip, ENTREZ2_GET_LINKS_max_UIDS,  &av);
   av.boolvalue = ptr -> count_only;
   retval = AsnWrite(aip, ENTREZ2_GET_LINKS_count_only,  &av);
   av.boolvalue = ptr -> parents_persist;
   retval = AsnWrite(aip, GET_LINKS_parents_persist,  &av);
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
*    Entrez2IdNew()
*
**************************************************/
NLM_EXTERN 
Entrez2IdPtr LIBCALL
Entrez2IdNew(void)
{
   Entrez2IdPtr ptr = MemNew((size_t) sizeof(Entrez2Id));

   return ptr;

}


/**************************************************
*
*    Entrez2IdFree()
*
**************************************************/
NLM_EXTERN 
Entrez2IdPtr LIBCALL
Entrez2IdFree(Entrez2IdPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> db);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2IdAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2IdPtr LIBCALL
Entrez2IdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2IdPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2Id ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_ID);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_ID);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2IdNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_ID_db) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_ID_uid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> uid = av.intvalue;
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
   ptr = Entrez2IdFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2IdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2IdAsnWrite(Entrez2IdPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_ID);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> db != NULL) {
      av.ptrvalue = ptr -> db;
      retval = AsnWrite(aip, ENTREZ2_ID_db,  &av);
   }
   av.intvalue = ptr -> uid;
   retval = AsnWrite(aip, ENTREZ2_ID_uid,  &av);
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
*    Entrez2DtFilterNew()
*
**************************************************/
NLM_EXTERN 
Entrez2DtFilterPtr LIBCALL
Entrez2DtFilterNew(void)
{
   Entrez2DtFilterPtr ptr = MemNew((size_t) sizeof(Entrez2DtFilter));

   return ptr;

}


/**************************************************
*
*    Entrez2DtFilterFree()
*
**************************************************/
NLM_EXTERN 
Entrez2DtFilterPtr LIBCALL
Entrez2DtFilterFree(Entrez2DtFilterPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> type_date);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2DtFilterAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2DtFilterPtr LIBCALL
Entrez2DtFilterAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2DtFilterPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2DtFilter ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_DT_FILTER);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_DT_FILTER);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2DtFilterNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_DT_FILTER_begin_date) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> begin_date = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DT_FILTER_end_date) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> end_date = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DT_FILTER_type_date) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type_date = av.ptrvalue;
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
   ptr = Entrez2DtFilterFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2DtFilterAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2DtFilterAsnWrite(Entrez2DtFilterPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_DT_FILTER);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> begin_date;
   retval = AsnWrite(aip, ENTREZ2_DT_FILTER_begin_date,  &av);
   av.intvalue = ptr -> end_date;
   retval = AsnWrite(aip, ENTREZ2_DT_FILTER_end_date,  &av);
   if (ptr -> type_date != NULL) {
      av.ptrvalue = ptr -> type_date;
      retval = AsnWrite(aip, ENTREZ2_DT_FILTER_type_date,  &av);
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
*    Entrez2ReplyNew()
*
**************************************************/
NLM_EXTERN 
Entrez2ReplyPtr LIBCALL
Entrez2ReplyNew(void)
{
   Entrez2ReplyPtr ptr = MemNew((size_t) sizeof(Entrez2Reply));

   return ptr;

}


/**************************************************
*
*    Entrez2ReplyFree()
*
**************************************************/
NLM_EXTERN 
Entrez2ReplyPtr LIBCALL
Entrez2ReplyFree(Entrez2ReplyPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   E2ReplyFree(ptr -> reply);
   MemFree(ptr -> server);
   MemFree(ptr -> msg);
   MemFree(ptr -> key);
   MemFree(ptr -> cookie);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2ReplyAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2ReplyPtr LIBCALL
Entrez2ReplyAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2ReplyPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2Reply ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_REPLY);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_REPLY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2ReplyNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_REPLY_reply) {
      ptr -> reply = E2ReplyAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_REPLY_dt) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> dt = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_REPLY_server) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> server = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_REPLY_msg) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> msg = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_REPLY_key) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> key = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_REPLY_cookie) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> cookie = av.ptrvalue;
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
   ptr = Entrez2ReplyFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2ReplyAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2ReplyAsnWrite(Entrez2ReplyPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_REPLY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> reply != NULL) {
      if ( ! E2ReplyAsnWrite(ptr -> reply, aip, ENTREZ2_REPLY_reply)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> dt;
   retval = AsnWrite(aip, ENTREZ2_REPLY_dt,  &av);
   if (ptr -> server != NULL) {
      av.ptrvalue = ptr -> server;
      retval = AsnWrite(aip, ENTREZ2_REPLY_server,  &av);
   }
   if (ptr -> msg != NULL) {
      av.ptrvalue = ptr -> msg;
      retval = AsnWrite(aip, ENTREZ2_REPLY_msg,  &av);
   }
   if (ptr -> key != NULL) {
      av.ptrvalue = ptr -> key;
      retval = AsnWrite(aip, ENTREZ2_REPLY_key,  &av);
   }
   if (ptr -> cookie != NULL) {
      av.ptrvalue = ptr -> cookie;
      retval = AsnWrite(aip, ENTREZ2_REPLY_cookie,  &av);
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
*    E2ReplyFree()
*
**************************************************/
NLM_EXTERN 
E2ReplyPtr LIBCALL
E2ReplyFree(ValNodePtr anp)
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
   case E2Reply_error:
      MemFree(anp -> data.ptrvalue);
      break;
   case E2Reply_get_info:
      Entrez2InfoFree(anp -> data.ptrvalue);
      break;
   case E2Reply_eval_boolean:
      Entrez2BooleanReplyFree(anp -> data.ptrvalue);
      break;
   case E2Reply_get_docsum:
      Entrez2DocsumListFree(anp -> data.ptrvalue);
      break;
   case E2Reply_get_term_list:
      Entrez2TermListFree(anp -> data.ptrvalue);
      break;
   case E2Reply_get_term_hierarchy:
      Entrez2HierNodeFree(anp -> data.ptrvalue);
      break;
   case E2Reply_get_links:
      Entrez2LinkSetFree(anp -> data.ptrvalue);
      break;
   case E2Reply_get_linked:
      Entrez2IdListFree(anp -> data.ptrvalue);
      break;
   case E2Reply_get_link_counts:
      Entrez2LinkCountListFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    E2ReplyAsnRead()
*
**************************************************/
NLM_EXTERN 
E2ReplyPtr LIBCALL
E2ReplyAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* E2Reply ::= (self contained) */
      atp = AsnReadId(aip, amp, E2REPLY);
   } else {
      atp = AsnLinkType(orig, E2REPLY);    /* link in local tree */
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
   if (atp == E2REPLY_error) {
      choice = E2Reply_error;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == E2REPLY_get_info) {
      choice = E2Reply_get_info;
      func = (AsnReadFunc) Entrez2InfoAsnRead;
   }
   else if (atp == E2REPLY_eval_boolean) {
      choice = E2Reply_eval_boolean;
      func = (AsnReadFunc) Entrez2BooleanReplyAsnRead;
   }
   else if (atp == E2REPLY_get_docsum) {
      choice = E2Reply_get_docsum;
      func = (AsnReadFunc) Entrez2DocsumListAsnRead;
   }
   else if (atp == E2REPLY_get_term_pos) {
      choice = E2Reply_get_term_pos;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == E2REPLY_get_term_list) {
      choice = E2Reply_get_term_list;
      func = (AsnReadFunc) Entrez2TermListAsnRead;
   }
   else if (atp == E2REPLY_get_term_hierarchy) {
      choice = E2Reply_get_term_hierarchy;
      func = (AsnReadFunc) Entrez2HierNodeAsnRead;
   }
   else if (atp == E2REPLY_get_links) {
      choice = E2Reply_get_links;
      func = (AsnReadFunc) Entrez2LinkSetAsnRead;
   }
   else if (atp == E2REPLY_get_linked) {
      choice = E2Reply_get_linked;
      func = (AsnReadFunc) Entrez2IdListAsnRead;
   }
   else if (atp == E2REPLY_get_link_counts) {
      choice = E2Reply_get_link_counts;
      func = (AsnReadFunc) Entrez2LinkCountListAsnRead;
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
*    E2ReplyAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
E2ReplyAsnWrite(E2ReplyPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, E2REPLY);   /* link local tree */
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
   case E2Reply_error:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, E2REPLY_error, &av);
      break;
   case E2Reply_get_info:
      writetype = E2REPLY_get_info;
      func = (AsnWriteFunc) Entrez2InfoAsnWrite;
      break;
   case E2Reply_eval_boolean:
      writetype = E2REPLY_eval_boolean;
      func = (AsnWriteFunc) Entrez2BooleanReplyAsnWrite;
      break;
   case E2Reply_get_docsum:
      writetype = E2REPLY_get_docsum;
      func = (AsnWriteFunc) Entrez2DocsumListAsnWrite;
      break;
   case E2Reply_get_term_pos:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, E2REPLY_get_term_pos, &av);
      break;
   case E2Reply_get_term_list:
      writetype = E2REPLY_get_term_list;
      func = (AsnWriteFunc) Entrez2TermListAsnWrite;
      break;
   case E2Reply_get_term_hierarchy:
      writetype = E2REPLY_get_term_hierarchy;
      func = (AsnWriteFunc) Entrez2HierNodeAsnWrite;
      break;
   case E2Reply_get_links:
      writetype = E2REPLY_get_links;
      func = (AsnWriteFunc) Entrez2LinkSetAsnWrite;
      break;
   case E2Reply_get_linked:
      writetype = E2REPLY_get_linked;
      func = (AsnWriteFunc) Entrez2IdListAsnWrite;
      break;
   case E2Reply_get_link_counts:
      writetype = E2REPLY_get_link_counts;
      func = (AsnWriteFunc) Entrez2LinkCountListAsnWrite;
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
*    Entrez2InfoNew()
*
**************************************************/
NLM_EXTERN 
Entrez2InfoPtr LIBCALL
Entrez2InfoNew(void)
{
   Entrez2InfoPtr ptr = MemNew((size_t) sizeof(Entrez2Info));

   return ptr;

}


/**************************************************
*
*    Entrez2InfoFree()
*
**************************************************/
NLM_EXTERN 
Entrez2InfoPtr LIBCALL
Entrez2InfoFree(Entrez2InfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> db_info, (AsnOptFreeFunc) Entrez2DbInfoFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2InfoAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2InfoPtr LIBCALL
Entrez2InfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2InfoPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2Info ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_INFO);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2InfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_INFO_db_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db_count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_INFO_build_date) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> build_date = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_INFO_db_info) {
      ptr -> db_info = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Entrez2DbInfoAsnRead, (AsnOptFreeFunc) Entrez2DbInfoFree);
      if (isError && ptr -> db_info == NULL) {
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
   ptr = Entrez2InfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2InfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2InfoAsnWrite(Entrez2InfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> db_count;
   retval = AsnWrite(aip, ENTREZ2_INFO_db_count,  &av);
   av.intvalue = ptr -> build_date;
   retval = AsnWrite(aip, ENTREZ2_INFO_build_date,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> db_info, (AsnWriteFunc) Entrez2DbInfoAsnWrite, aip, ENTREZ2_INFO_db_info, ENTREZ2_INFO_db_info_E);
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
*    Entrez2BooleanReplyNew()
*
**************************************************/
NLM_EXTERN 
Entrez2BooleanReplyPtr LIBCALL
Entrez2BooleanReplyNew(void)
{
   Entrez2BooleanReplyPtr ptr = MemNew((size_t) sizeof(Entrez2BooleanReply));

   return ptr;

}


/**************************************************
*
*    Entrez2BooleanReplyFree()
*
**************************************************/
NLM_EXTERN 
Entrez2BooleanReplyPtr LIBCALL
Entrez2BooleanReplyFree(Entrez2BooleanReplyPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Entrez2IdListFree(ptr -> uids);
   Entrez2BooleanExpFree(ptr -> query);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2BooleanReplyAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2BooleanReplyPtr LIBCALL
Entrez2BooleanReplyAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2BooleanReplyPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2BooleanReply ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_BOOLEAN_REPLY);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_BOOLEAN_REPLY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2BooleanReplyNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_BOOLEAN_REPLY_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_BOOLEAN_REPLY_uids) {
      ptr -> uids = Entrez2IdListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_BOOLEAN_REPLY_query) {
      ptr -> query = Entrez2BooleanExpAsnRead(aip, atp);
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
   ptr = Entrez2BooleanReplyFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2BooleanReplyAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2BooleanReplyAsnWrite(Entrez2BooleanReplyPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_BOOLEAN_REPLY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> count;
   retval = AsnWrite(aip, ENTREZ2_BOOLEAN_REPLY_count,  &av);
   if (ptr -> uids != NULL) {
      if ( ! Entrez2IdListAsnWrite(ptr -> uids, aip, ENTREZ2_BOOLEAN_REPLY_uids)) {
         goto erret;
      }
   }
   if (ptr -> query != NULL) {
      if ( ! Entrez2BooleanExpAsnWrite(ptr -> query, aip, ENTREZ2_BOOLEAN_REPLY_query)) {
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
*    Entrez2DocsumListNew()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumListPtr LIBCALL
Entrez2DocsumListNew(void)
{
   Entrez2DocsumListPtr ptr = MemNew((size_t) sizeof(Entrez2DocsumList));

   return ptr;

}


/**************************************************
*
*    Entrez2DocsumListFree()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumListPtr LIBCALL
Entrez2DocsumListFree(Entrez2DocsumListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> list, (AsnOptFreeFunc) Entrez2DocsumFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2DocsumListAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumListPtr LIBCALL
Entrez2DocsumListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2DocsumListPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2DocsumList ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_DOCSUM_LIST);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_DOCSUM_LIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2DocsumListNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_DOCSUM_LIST_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DOCSUM_LIST_list) {
      ptr -> list = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Entrez2DocsumAsnRead, (AsnOptFreeFunc) Entrez2DocsumFree);
      if (isError && ptr -> list == NULL) {
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
   ptr = Entrez2DocsumListFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2DocsumListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2DocsumListAsnWrite(Entrez2DocsumListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_DOCSUM_LIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> count;
   retval = AsnWrite(aip, ENTREZ2_DOCSUM_LIST_count,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> list, (AsnWriteFunc) Entrez2DocsumAsnWrite, aip, ENTREZ2_DOCSUM_LIST_list, ENTREZ2_DOCSUM_LIST_list_E);
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
*    Entrez2TermListNew()
*
**************************************************/
NLM_EXTERN 
Entrez2TermListPtr LIBCALL
Entrez2TermListNew(void)
{
   Entrez2TermListPtr ptr = MemNew((size_t) sizeof(Entrez2TermList));

   return ptr;

}


/**************************************************
*
*    Entrez2TermListFree()
*
**************************************************/
NLM_EXTERN 
Entrez2TermListPtr LIBCALL
Entrez2TermListFree(Entrez2TermListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> list, (AsnOptFreeFunc) Entrez2TermFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2TermListAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2TermListPtr LIBCALL
Entrez2TermListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2TermListPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2TermList ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_TERM_LIST);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_TERM_LIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2TermListNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_TERM_LIST_pos) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pos = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_TERM_LIST_num) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> num = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_TERM_LIST_list) {
      ptr -> list = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Entrez2TermAsnRead, (AsnOptFreeFunc) Entrez2TermFree);
      if (isError && ptr -> list == NULL) {
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
   ptr = Entrez2TermListFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2TermListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2TermListAsnWrite(Entrez2TermListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_TERM_LIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> pos;
   retval = AsnWrite(aip, ENTREZ2_TERM_LIST_pos,  &av);
   av.intvalue = ptr -> num;
   retval = AsnWrite(aip, ENTREZ2_TERM_LIST_num,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> list, (AsnWriteFunc) Entrez2TermAsnWrite, aip, ENTREZ2_TERM_LIST_list, ENTREZ2_TERM_LIST_list_E);
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
*    Entrez2HierNodeNew()
*
**************************************************/
NLM_EXTERN 
Entrez2HierNodePtr LIBCALL
Entrez2HierNodeNew(void)
{
   Entrez2HierNodePtr ptr = MemNew((size_t) sizeof(Entrez2HierNode));

   return ptr;

}


/**************************************************
*
*    Entrez2HierNodeFree()
*
**************************************************/
NLM_EXTERN 
Entrez2HierNodePtr LIBCALL
Entrez2HierNodeFree(Entrez2HierNodePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> cannonical_form);
   AsnGenericUserSeqOfFree(ptr -> lineage, (AsnOptFreeFunc) Entrez2TermFree);
   AsnGenericUserSeqOfFree(ptr -> children, (AsnOptFreeFunc) Entrez2TermFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2HierNodeAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2HierNodePtr LIBCALL
Entrez2HierNodeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2HierNodePtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2HierNode ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_HIER_NODE);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_HIER_NODE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2HierNodeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == HIER_NODE_cannonical_form) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> cannonical_form = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_HIER_NODE_lineage_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> lineage_count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_HIER_NODE_lineage) {
      ptr -> lineage = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Entrez2TermAsnRead, (AsnOptFreeFunc) Entrez2TermFree);
      if (isError && ptr -> lineage == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_HIER_NODE_child_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> child_count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_HIER_NODE_children) {
      ptr -> children = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Entrez2TermAsnRead, (AsnOptFreeFunc) Entrez2TermFree);
      if (isError && ptr -> children == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_HIER_NODE_is_ambiguous) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_ambiguous = av.boolvalue;
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
   ptr = Entrez2HierNodeFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2HierNodeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2HierNodeAsnWrite(Entrez2HierNodePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_HIER_NODE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> cannonical_form != NULL) {
      av.ptrvalue = ptr -> cannonical_form;
      retval = AsnWrite(aip, HIER_NODE_cannonical_form,  &av);
   }
   av.intvalue = ptr -> lineage_count;
   retval = AsnWrite(aip, ENTREZ2_HIER_NODE_lineage_count,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> lineage, (AsnWriteFunc) Entrez2TermAsnWrite, aip, ENTREZ2_HIER_NODE_lineage, ENTREZ2_HIER_NODE_lineage_E);
   av.intvalue = ptr -> child_count;
   retval = AsnWrite(aip, ENTREZ2_HIER_NODE_child_count,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> children, (AsnWriteFunc) Entrez2TermAsnWrite, aip, ENTREZ2_HIER_NODE_children, ENTREZ2_HIER_NODE_children_E);
   av.boolvalue = ptr -> is_ambiguous;
   retval = AsnWrite(aip, ENTREZ2_HIER_NODE_is_ambiguous,  &av);
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
*    Entrez2LinkSetNew()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkSetPtr LIBCALL
Entrez2LinkSetNew(void)
{
   Entrez2LinkSetPtr ptr = MemNew((size_t) sizeof(Entrez2LinkSet));

   return ptr;

}


/**************************************************
*
*    Entrez2LinkSetFree()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkSetPtr LIBCALL
Entrez2LinkSetFree(Entrez2LinkSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Entrez2IdListFree(ptr -> ids);
   BSFree(ptr -> data);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2LinkSetAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkSetPtr LIBCALL
Entrez2LinkSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2LinkSetPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2LinkSet ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_LINK_SET);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_LINK_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2LinkSetNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_LINK_SET_ids) {
      ptr -> ids = Entrez2IdListAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_LINK_SET_data_size) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> data_size = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_LINK_SET_data) {
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
   ptr = Entrez2LinkSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2LinkSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2LinkSetAsnWrite(Entrez2LinkSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_LINK_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> ids != NULL) {
      if ( ! Entrez2IdListAsnWrite(ptr -> ids, aip, ENTREZ2_LINK_SET_ids)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> data_size;
   retval = AsnWrite(aip, ENTREZ2_LINK_SET_data_size,  &av);
   if (ptr -> data != NULL) {
      av.ptrvalue = ptr -> data;
      retval = AsnWrite(aip, ENTREZ2_LINK_SET_data,  &av);
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
*    Entrez2LinkCountListNew()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkCountListPtr LIBCALL
Entrez2LinkCountListNew(void)
{
   Entrez2LinkCountListPtr ptr = MemNew((size_t) sizeof(Entrez2LinkCountList));

   return ptr;

}


/**************************************************
*
*    Entrez2LinkCountListFree()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkCountListPtr LIBCALL
Entrez2LinkCountListFree(Entrez2LinkCountListPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> links, (AsnOptFreeFunc) Entrez2LinkCountFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2LinkCountListAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkCountListPtr LIBCALL
Entrez2LinkCountListAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2LinkCountListPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2LinkCountList ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_LINK_COUNT_LIST);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_LINK_COUNT_LIST);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2LinkCountListNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == COUNT_LIST_link_type_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> link_type_count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_LINK_COUNT_LIST_links) {
      ptr -> links = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Entrez2LinkCountAsnRead, (AsnOptFreeFunc) Entrez2LinkCountFree);
      if (isError && ptr -> links == NULL) {
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
   ptr = Entrez2LinkCountListFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2LinkCountListAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2LinkCountListAsnWrite(Entrez2LinkCountListPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_LINK_COUNT_LIST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> link_type_count;
   retval = AsnWrite(aip, COUNT_LIST_link_type_count,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> links, (AsnWriteFunc) Entrez2LinkCountAsnWrite, aip, ENTREZ2_LINK_COUNT_LIST_links, ENTREZ2_LINK_COUNT_LIST_links_E);
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
*    Entrez2DbInfoNew()
*
**************************************************/
NLM_EXTERN 
Entrez2DbInfoPtr LIBCALL
Entrez2DbInfoNew(void)
{
   Entrez2DbInfoPtr ptr = MemNew((size_t) sizeof(Entrez2DbInfo));

   return ptr;

}


/**************************************************
*
*    Entrez2DbInfoFree()
*
**************************************************/
NLM_EXTERN 
Entrez2DbInfoPtr LIBCALL
Entrez2DbInfoFree(Entrez2DbInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> db_name);
   MemFree(ptr -> db_menu);
   MemFree(ptr -> db_descr);
   AsnGenericUserSeqOfFree(ptr -> fields, (AsnOptFreeFunc) Entrez2FieldInfoFree);
   AsnGenericUserSeqOfFree(ptr -> links, (AsnOptFreeFunc) Entrez2LinkInfoFree);
   AsnGenericUserSeqOfFree(ptr -> docsum_fields, (AsnOptFreeFunc) Entrez2DocsumFieldInfoFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2DbInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2DbInfoPtr LIBCALL
Entrez2DbInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2DbInfoPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2DbInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_DB_INFO);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_DB_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2DbInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_DB_INFO_db_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db_name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DB_INFO_db_menu) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db_menu = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DB_INFO_db_descr) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db_descr = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DB_INFO_doc_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> doc_count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DB_INFO_field_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field_count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DB_INFO_fields) {
      ptr -> fields = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Entrez2FieldInfoAsnRead, (AsnOptFreeFunc) Entrez2FieldInfoFree);
      if (isError && ptr -> fields == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DB_INFO_link_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> link_count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DB_INFO_links) {
      ptr -> links = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Entrez2LinkInfoAsnRead, (AsnOptFreeFunc) Entrez2LinkInfoFree);
      if (isError && ptr -> links == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DB_INFO_docsum_field_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> docsum_field_count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DB_INFO_docsum_fields) {
      ptr -> docsum_fields = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Entrez2DocsumFieldInfoAsnRead, (AsnOptFreeFunc) Entrez2DocsumFieldInfoFree);
      if (isError && ptr -> docsum_fields == NULL) {
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
   ptr = Entrez2DbInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2DbInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2DbInfoAsnWrite(Entrez2DbInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_DB_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> db_name != NULL) {
      av.ptrvalue = ptr -> db_name;
      retval = AsnWrite(aip, ENTREZ2_DB_INFO_db_name,  &av);
   }
   if (ptr -> db_menu != NULL) {
      av.ptrvalue = ptr -> db_menu;
      retval = AsnWrite(aip, ENTREZ2_DB_INFO_db_menu,  &av);
   }
   if (ptr -> db_descr != NULL) {
      av.ptrvalue = ptr -> db_descr;
      retval = AsnWrite(aip, ENTREZ2_DB_INFO_db_descr,  &av);
   }
   av.intvalue = ptr -> doc_count;
   retval = AsnWrite(aip, ENTREZ2_DB_INFO_doc_count,  &av);
   av.intvalue = ptr -> field_count;
   retval = AsnWrite(aip, ENTREZ2_DB_INFO_field_count,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> fields, (AsnWriteFunc) Entrez2FieldInfoAsnWrite, aip, ENTREZ2_DB_INFO_fields, ENTREZ2_DB_INFO_fields_E);
   av.intvalue = ptr -> link_count;
   retval = AsnWrite(aip, ENTREZ2_DB_INFO_link_count,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> links, (AsnWriteFunc) Entrez2LinkInfoAsnWrite, aip, ENTREZ2_DB_INFO_links, ENTREZ2_DB_INFO_links_E);
   av.intvalue = ptr -> docsum_field_count;
   retval = AsnWrite(aip, DB_INFO_docsum_field_count,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> docsum_fields, (AsnWriteFunc) Entrez2DocsumFieldInfoAsnWrite, aip, ENTREZ2_DB_INFO_docsum_fields, ENTREZ2_DB_INFO_docsum_fields_E);
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
*    Entrez2FieldInfoNew()
*
**************************************************/
NLM_EXTERN 
Entrez2FieldInfoPtr LIBCALL
Entrez2FieldInfoNew(void)
{
   Entrez2FieldInfoPtr ptr = MemNew((size_t) sizeof(Entrez2FieldInfo));

   return ptr;

}


/**************************************************
*
*    Entrez2FieldInfoFree()
*
**************************************************/
NLM_EXTERN 
Entrez2FieldInfoPtr LIBCALL
Entrez2FieldInfoFree(Entrez2FieldInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> field_name);
   MemFree(ptr -> field_menu);
   MemFree(ptr -> field_descr);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2FieldInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2FieldInfoPtr LIBCALL
Entrez2FieldInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2FieldInfoPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2FieldInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_FIELD_INFO);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_FIELD_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2FieldInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_FIELD_INFO_field_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field_name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_FIELD_INFO_field_menu) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field_menu = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_FIELD_INFO_field_descr) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field_descr = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_FIELD_INFO_term_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> term_count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_FIELD_INFO_is_date) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_date = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_FIELD_INFO_is_numerical) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_numerical = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_FIELD_INFO_single_token) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> single_token = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FIELD_INFO_hierarchy_avail) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hierarchy_avail = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_FIELD_INFO_is_rangable) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_rangable = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FIELD_INFO_is_truncatable) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_truncatable = av.boolvalue;
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
   ptr = Entrez2FieldInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2FieldInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2FieldInfoAsnWrite(Entrez2FieldInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_FIELD_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> field_name != NULL) {
      av.ptrvalue = ptr -> field_name;
      retval = AsnWrite(aip, ENTREZ2_FIELD_INFO_field_name,  &av);
   }
   if (ptr -> field_menu != NULL) {
      av.ptrvalue = ptr -> field_menu;
      retval = AsnWrite(aip, ENTREZ2_FIELD_INFO_field_menu,  &av);
   }
   if (ptr -> field_descr != NULL) {
      av.ptrvalue = ptr -> field_descr;
      retval = AsnWrite(aip, ENTREZ2_FIELD_INFO_field_descr,  &av);
   }
   av.intvalue = ptr -> term_count;
   retval = AsnWrite(aip, ENTREZ2_FIELD_INFO_term_count,  &av);
   av.boolvalue = ptr -> is_date;
   retval = AsnWrite(aip, ENTREZ2_FIELD_INFO_is_date,  &av);
   av.boolvalue = ptr -> is_numerical;
   retval = AsnWrite(aip, ENTREZ2_FIELD_INFO_is_numerical,  &av);
   av.boolvalue = ptr -> single_token;
   retval = AsnWrite(aip, ENTREZ2_FIELD_INFO_single_token,  &av);
   av.boolvalue = ptr -> hierarchy_avail;
   retval = AsnWrite(aip, FIELD_INFO_hierarchy_avail,  &av);
   av.boolvalue = ptr -> is_rangable;
   retval = AsnWrite(aip, ENTREZ2_FIELD_INFO_is_rangable,  &av);
   av.boolvalue = ptr -> is_truncatable;
   retval = AsnWrite(aip, FIELD_INFO_is_truncatable,  &av);
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
*    Entrez2LinkInfoNew()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkInfoPtr LIBCALL
Entrez2LinkInfoNew(void)
{
   Entrez2LinkInfoPtr ptr = MemNew((size_t) sizeof(Entrez2LinkInfo));

   return ptr;

}


/**************************************************
*
*    Entrez2LinkInfoFree()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkInfoPtr LIBCALL
Entrez2LinkInfoFree(Entrez2LinkInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> link_name);
   MemFree(ptr -> link_menu);
   MemFree(ptr -> link_descr);
   MemFree(ptr -> db_to);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2LinkInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkInfoPtr LIBCALL
Entrez2LinkInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2LinkInfoPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2LinkInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_LINK_INFO);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_LINK_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2LinkInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_LINK_INFO_link_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> link_name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_LINK_INFO_link_menu) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> link_menu = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_LINK_INFO_link_descr) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> link_descr = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_LINK_INFO_db_to) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db_to = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_LINK_INFO_data_size) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> data_size = av.intvalue;
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
   ptr = Entrez2LinkInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2LinkInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2LinkInfoAsnWrite(Entrez2LinkInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_LINK_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> link_name != NULL) {
      av.ptrvalue = ptr -> link_name;
      retval = AsnWrite(aip, ENTREZ2_LINK_INFO_link_name,  &av);
   }
   if (ptr -> link_menu != NULL) {
      av.ptrvalue = ptr -> link_menu;
      retval = AsnWrite(aip, ENTREZ2_LINK_INFO_link_menu,  &av);
   }
   if (ptr -> link_descr != NULL) {
      av.ptrvalue = ptr -> link_descr;
      retval = AsnWrite(aip, ENTREZ2_LINK_INFO_link_descr,  &av);
   }
   if (ptr -> db_to != NULL) {
      av.ptrvalue = ptr -> db_to;
      retval = AsnWrite(aip, ENTREZ2_LINK_INFO_db_to,  &av);
   }
   av.intvalue = ptr -> data_size;
   retval = AsnWrite(aip, ENTREZ2_LINK_INFO_data_size,  &av);
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
*    Entrez2DocsumFieldInfoNew()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumFieldInfoPtr LIBCALL
Entrez2DocsumFieldInfoNew(void)
{
   Entrez2DocsumFieldInfoPtr ptr = MemNew((size_t) sizeof(Entrez2DocsumFieldInfo));

   return ptr;

}


/**************************************************
*
*    Entrez2DocsumFieldInfoFree()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumFieldInfoPtr LIBCALL
Entrez2DocsumFieldInfoFree(Entrez2DocsumFieldInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> field_name);
   MemFree(ptr -> field_description);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2DocsumFieldInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumFieldInfoPtr LIBCALL
Entrez2DocsumFieldInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2DocsumFieldInfoPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2DocsumFieldInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_DOCSUM_FIELD_INFO);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_DOCSUM_FIELD_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2DocsumFieldInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == DOCSUM_FIELD_INFO_field_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field_name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FIELD_INFO_field_description) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field_description = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOCSUM_FIELD_INFO_field_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field_type = av.intvalue;
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
   ptr = Entrez2DocsumFieldInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2DocsumFieldInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2DocsumFieldInfoAsnWrite(Entrez2DocsumFieldInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_DOCSUM_FIELD_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> field_name != NULL) {
      av.ptrvalue = ptr -> field_name;
      retval = AsnWrite(aip, DOCSUM_FIELD_INFO_field_name,  &av);
   }
   if (ptr -> field_description != NULL) {
      av.ptrvalue = ptr -> field_description;
      retval = AsnWrite(aip, FIELD_INFO_field_description,  &av);
   }
   av.intvalue = ptr -> field_type;
   retval = AsnWrite(aip, DOCSUM_FIELD_INFO_field_type,  &av);
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
*    Entrez2DocsumNew()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumPtr LIBCALL
Entrez2DocsumNew(void)
{
   Entrez2DocsumPtr ptr = MemNew((size_t) sizeof(Entrez2Docsum));

   return ptr;

}


/**************************************************
*
*    Entrez2DocsumFree()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumPtr LIBCALL
Entrez2DocsumFree(Entrez2DocsumPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> docsum_data, (AsnOptFreeFunc) Entrez2DocsumDataFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2DocsumAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumPtr LIBCALL
Entrez2DocsumAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2DocsumPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2Docsum ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_DOCSUM);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_DOCSUM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2DocsumNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_DOCSUM_uid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> uid = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DOCSUM_docsum_data) {
      ptr -> docsum_data = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) Entrez2DocsumDataAsnRead, (AsnOptFreeFunc) Entrez2DocsumDataFree);
      if (isError && ptr -> docsum_data == NULL) {
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
   ptr = Entrez2DocsumFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2DocsumAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2DocsumAsnWrite(Entrez2DocsumPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_DOCSUM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> uid;
   retval = AsnWrite(aip, ENTREZ2_DOCSUM_uid,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> docsum_data, (AsnWriteFunc) Entrez2DocsumDataAsnWrite, aip, ENTREZ2_DOCSUM_docsum_data, ENTREZ2_DOCSUM_docsum_data_E);
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
*    Entrez2DocsumDataNew()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumDataPtr LIBCALL
Entrez2DocsumDataNew(void)
{
   Entrez2DocsumDataPtr ptr = MemNew((size_t) sizeof(Entrez2DocsumData));

   return ptr;

}


/**************************************************
*
*    Entrez2DocsumDataFree()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumDataPtr LIBCALL
Entrez2DocsumDataFree(Entrez2DocsumDataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> field_name);
   MemFree(ptr -> field_value);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2DocsumDataAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2DocsumDataPtr LIBCALL
Entrez2DocsumDataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2DocsumDataPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2DocsumData ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_DOCSUM_DATA);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_DOCSUM_DATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2DocsumDataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_DOCSUM_DATA_field_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field_name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_DOCSUM_DATA_field_value) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> field_value = av.ptrvalue;
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
   ptr = Entrez2DocsumDataFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2DocsumDataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2DocsumDataAsnWrite(Entrez2DocsumDataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_DOCSUM_DATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> field_name != NULL) {
      av.ptrvalue = ptr -> field_name;
      retval = AsnWrite(aip, ENTREZ2_DOCSUM_DATA_field_name,  &av);
   }
   if (ptr -> field_value != NULL) {
      av.ptrvalue = ptr -> field_value;
      retval = AsnWrite(aip, ENTREZ2_DOCSUM_DATA_field_value,  &av);
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
*    Entrez2TermNew()
*
**************************************************/
NLM_EXTERN 
Entrez2TermPtr LIBCALL
Entrez2TermNew(void)
{
   Entrez2TermPtr ptr = MemNew((size_t) sizeof(Entrez2Term));

   return ptr;

}


/**************************************************
*
*    Entrez2TermFree()
*
**************************************************/
NLM_EXTERN 
Entrez2TermPtr LIBCALL
Entrez2TermFree(Entrez2TermPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> term);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2TermAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2TermPtr LIBCALL
Entrez2TermAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2TermPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2Term ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_TERM);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_TERM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2TermNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_TERM_term) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> term = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_TERM_txid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> txid = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_TERM_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_TERM_is_leaf_node) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_leaf_node = av.boolvalue;
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
   ptr = Entrez2TermFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2TermAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2TermAsnWrite(Entrez2TermPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_TERM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> term != NULL) {
      av.ptrvalue = ptr -> term;
      retval = AsnWrite(aip, ENTREZ2_TERM_term,  &av);
   }
   av.intvalue = ptr -> txid;
   retval = AsnWrite(aip, ENTREZ2_TERM_txid,  &av);
   av.intvalue = ptr -> count;
   retval = AsnWrite(aip, ENTREZ2_TERM_count,  &av);
   av.boolvalue = ptr -> is_leaf_node;
   retval = AsnWrite(aip, ENTREZ2_TERM_is_leaf_node,  &av);
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
*    Entrez2LinkCountNew()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkCountPtr LIBCALL
Entrez2LinkCountNew(void)
{
   Entrez2LinkCountPtr ptr = MemNew((size_t) sizeof(Entrez2LinkCount));

   return ptr;

}


/**************************************************
*
*    Entrez2LinkCountFree()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkCountPtr LIBCALL
Entrez2LinkCountFree(Entrez2LinkCountPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> link_type);
   return MemFree(ptr);
}


/**************************************************
*
*    Entrez2LinkCountAsnRead()
*
**************************************************/
NLM_EXTERN 
Entrez2LinkCountPtr LIBCALL
Entrez2LinkCountAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Entrez2LinkCountPtr ptr;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrez2LinkCount ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZ2_LINK_COUNT);
   } else {
      atp = AsnLinkType(orig, ENTREZ2_LINK_COUNT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Entrez2LinkCountNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZ2_LINK_COUNT_link_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> link_type = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZ2_LINK_COUNT_link_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> link_count = av.intvalue;
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
   ptr = Entrez2LinkCountFree(ptr);
   goto ret;
}



/**************************************************
*
*    Entrez2LinkCountAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
Entrez2LinkCountAsnWrite(Entrez2LinkCountPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objent2AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZ2_LINK_COUNT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> link_type != NULL) {
      av.ptrvalue = ptr -> link_type;
      retval = AsnWrite(aip, ENTREZ2_LINK_COUNT_link_type,  &av);
   }
   av.intvalue = ptr -> link_count;
   retval = AsnWrite(aip, ENTREZ2_LINK_COUNT_link_count,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

