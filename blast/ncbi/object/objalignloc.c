#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <alignloc.h>
#include <objalignloc.h>

static Boolean loaded = FALSE;

#include <objgen.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objalignlocAsnLoad(void)
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
*    Generated object loaders for Module NCBI-Alignloc
*    Generated using ASNCODE Revision: 6.8 at Aug 13, 1999  2:44 PM
*
**************************************************/


/**************************************************
*
*    AlignLocSetFree()
*
**************************************************/
NLM_EXTERN 
AlignLocSetPtr LIBCALL
AlignLocSetFree(AlignLocSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) AlignLocFree);
   return NULL;
}


/**************************************************
*
*    AlignLocSetAsnRead()
*
**************************************************/
NLM_EXTERN 
AlignLocSetPtr LIBCALL
AlignLocSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AlignLocSetPtr ptr;

   if (! loaded)
   {
      if (! objalignlocAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AlignLocSet ::= (self contained) */
      atp = AsnReadId(aip, amp, ALIGN_LOC_SET);
   } else {
      atp = AsnLinkType(orig, ALIGN_LOC_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) AlignLocAsnRead, (AsnOptFreeFunc) AlignLocFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AlignLocSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    AlignLocSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AlignLocSetAsnWrite(AlignLocSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objalignlocAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ALIGN_LOC_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) AlignLocAsnWrite, aip, atp, ALIGN_LOC_SET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    AlignLocNew()
*
**************************************************/
NLM_EXTERN 
AlignLocPtr LIBCALL
AlignLocNew(void)
{
   AlignLocPtr ptr = MemNew((size_t) sizeof(AlignLoc));

   return ptr;

}


/**************************************************
*
*    AlignLocFree()
*
**************************************************/
NLM_EXTERN 
AlignLocPtr LIBCALL
AlignLocFree(AlignLocPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AlignIdFree(ptr -> id);
   Loc_locFree(ptr -> Loc_loc);
   return MemFree(ptr);
}


/**************************************************
*
*    Loc_locFree()
*
**************************************************/
static 
Loc_locPtr LIBCALL
Loc_locFree(ValNodePtr anp)
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
   case Loc_loc_box:
      AlignBoxFree(anp -> data.ptrvalue);
      break;
   case Loc_loc_rows:
      AlignRowsFree(anp -> data.ptrvalue);
      break;
   case Loc_loc_columns:
      AlignColumnsFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    AlignLocAsnRead()
*
**************************************************/
NLM_EXTERN 
AlignLocPtr LIBCALL
AlignLocAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AlignLocPtr ptr;

   if (! loaded)
   {
      if (! objalignlocAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AlignLoc ::= (self contained) */
      atp = AsnReadId(aip, amp, ALIGN_LOC);
   } else {
      atp = AsnLinkType(orig, ALIGN_LOC);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AlignLocNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ALIGN_LOC_id) {
      ptr -> id = AlignIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_LOC_loc) {
      ptr -> Loc_loc = Loc_locAsnRead(aip, atp);
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
   ptr = AlignLocFree(ptr);
   goto ret;
}



/**************************************************
*
*    Loc_locAsnRead()
*
**************************************************/
static 
Loc_locPtr LIBCALL
Loc_locAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objalignlocAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Loc_loc ::= (self contained) */
      atp = AsnReadId(aip, amp, ALIGN_LOC_loc);
   } else {
      atp = AsnLinkType(orig, ALIGN_LOC_loc);    /* link in local tree */
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
   if (atp == ALIGN_LOC_loc_whole) {
      choice = Loc_loc_whole;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == ALIGN_LOC_loc_box) {
      choice = Loc_loc_box;
      func = (AsnReadFunc) AlignBoxAsnRead;
   }
   else if (atp == ALIGN_LOC_loc_rows) {
      choice = Loc_loc_rows;
      func = (AsnReadFunc) AlignRowsAsnRead;
   }
   else if (atp == ALIGN_LOC_loc_columns) {
      choice = Loc_loc_columns;
      func = (AsnReadFunc) AlignColumnsAsnRead;
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
*    AlignLocAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AlignLocAsnWrite(AlignLocPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objalignlocAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ALIGN_LOC);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> id != NULL) {
      if ( ! AlignIdAsnWrite(ptr -> id, aip, ALIGN_LOC_id)) {
         goto erret;
      }
   }
   if (ptr -> Loc_loc != NULL) {
      if ( ! Loc_locAsnWrite(ptr -> Loc_loc, aip, ALIGN_LOC_loc)) {
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
*    Loc_locAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Loc_locAsnWrite(Loc_locPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objalignlocAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ALIGN_LOC_loc);   /* link local tree */
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
   case Loc_loc_whole:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, ALIGN_LOC_loc_whole, &av);
      break;
   case Loc_loc_box:
      writetype = ALIGN_LOC_loc_box;
      func = (AsnWriteFunc) AlignBoxAsnWrite;
      break;
   case Loc_loc_rows:
      writetype = ALIGN_LOC_loc_rows;
      func = (AsnWriteFunc) AlignRowsAsnWrite;
      break;
   case Loc_loc_columns:
      writetype = ALIGN_LOC_loc_columns;
      func = (AsnWriteFunc) AlignColumnsAsnWrite;
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
*    AlignIdFree()
*
**************************************************/
NLM_EXTERN 
AlignIdPtr LIBCALL
AlignIdFree(ValNodePtr anp)
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
   case AlignId_id:
      ObjectIdFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    AlignIdAsnRead()
*
**************************************************/
NLM_EXTERN 
AlignIdPtr LIBCALL
AlignIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objalignlocAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AlignId ::= (self contained) */
      atp = AsnReadId(aip, amp, ALIGN_ID);
   } else {
      atp = AsnLinkType(orig, ALIGN_ID);    /* link in local tree */
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
   if (atp == ALIGN_ID_id) {
      choice = AlignId_id;
      func = (AsnReadFunc) ObjectIdAsnRead;
   }
   else if (atp == ALIGN_ID_itemid) {
      choice = AlignId_itemid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ALIGN_ID_gi) {
      choice = AlignId_gi;
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
*    AlignIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AlignIdAsnWrite(AlignIdPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objalignlocAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ALIGN_ID);   /* link local tree */
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
   case AlignId_id:
      writetype = ALIGN_ID_id;
      func = (AsnWriteFunc) ObjectIdAsnWrite;
      break;
   case AlignId_itemid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ALIGN_ID_itemid, &av);
      break;
   case AlignId_gi:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ALIGN_ID_gi, &av);
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
*    AlignBoxNew()
*
**************************************************/
NLM_EXTERN 
AlignBoxPtr LIBCALL
AlignBoxNew(void)
{
   AlignBoxPtr ptr = MemNew((size_t) sizeof(AlignBox));

   return ptr;

}


/**************************************************
*
*    AlignBoxFree()
*
**************************************************/
NLM_EXTERN 
AlignBoxPtr LIBCALL
AlignBoxFree(AlignBoxPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    AlignBoxAsnRead()
*
**************************************************/
NLM_EXTERN 
AlignBoxPtr LIBCALL
AlignBoxAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AlignBoxPtr ptr;

   if (! loaded)
   {
      if (! objalignlocAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AlignBox ::= (self contained) */
      atp = AsnReadId(aip, amp, ALIGN_BOX);
   } else {
      atp = AsnLinkType(orig, ALIGN_BOX);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AlignBoxNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ALIGN_BOX_row_from) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> row_from = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_BOX_row_to) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> row_to = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_BOX_column_from) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> column_from = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_BOX_column_to) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> column_to = av.intvalue;
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
   ptr = AlignBoxFree(ptr);
   goto ret;
}



/**************************************************
*
*    AlignBoxAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AlignBoxAsnWrite(AlignBoxPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objalignlocAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ALIGN_BOX);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> row_from;
   retval = AsnWrite(aip, ALIGN_BOX_row_from,  &av);
   av.intvalue = ptr -> row_to;
   retval = AsnWrite(aip, ALIGN_BOX_row_to,  &av);
   av.intvalue = ptr -> column_from;
   retval = AsnWrite(aip, ALIGN_BOX_column_from,  &av);
   av.intvalue = ptr -> column_to;
   retval = AsnWrite(aip, ALIGN_BOX_column_to,  &av);
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
*    AlignRowsNew()
*
**************************************************/
NLM_EXTERN 
AlignRowsPtr LIBCALL
AlignRowsNew(void)
{
   AlignRowsPtr ptr = MemNew((size_t) sizeof(AlignRows));

   return ptr;

}


/**************************************************
*
*    AlignRowsFree()
*
**************************************************/
NLM_EXTERN 
AlignRowsPtr LIBCALL
AlignRowsFree(AlignRowsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    AlignRowsAsnRead()
*
**************************************************/
NLM_EXTERN 
AlignRowsPtr LIBCALL
AlignRowsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AlignRowsPtr ptr;

   if (! loaded)
   {
      if (! objalignlocAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AlignRows ::= (self contained) */
      atp = AsnReadId(aip, amp, ALIGN_ROWS);
   } else {
      atp = AsnLinkType(orig, ALIGN_ROWS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AlignRowsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ALIGN_ROWS_from) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> from = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_ROWS_to) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> to = av.intvalue;
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
   ptr = AlignRowsFree(ptr);
   goto ret;
}



/**************************************************
*
*    AlignRowsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AlignRowsAsnWrite(AlignRowsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objalignlocAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ALIGN_ROWS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> from;
   retval = AsnWrite(aip, ALIGN_ROWS_from,  &av);
   av.intvalue = ptr -> to;
   retval = AsnWrite(aip, ALIGN_ROWS_to,  &av);
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
*    AlignColumnsNew()
*
**************************************************/
NLM_EXTERN 
AlignColumnsPtr LIBCALL
AlignColumnsNew(void)
{
   AlignColumnsPtr ptr = MemNew((size_t) sizeof(AlignColumns));

   return ptr;

}


/**************************************************
*
*    AlignColumnsFree()
*
**************************************************/
NLM_EXTERN 
AlignColumnsPtr LIBCALL
AlignColumnsFree(AlignColumnsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    AlignColumnsAsnRead()
*
**************************************************/
NLM_EXTERN 
AlignColumnsPtr LIBCALL
AlignColumnsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AlignColumnsPtr ptr;

   if (! loaded)
   {
      if (! objalignlocAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AlignColumns ::= (self contained) */
      atp = AsnReadId(aip, amp, ALIGN_COLUMNS);
   } else {
      atp = AsnLinkType(orig, ALIGN_COLUMNS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AlignColumnsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ALIGN_COLUMNS_from) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> from = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_COLUMNS_to) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> to = av.intvalue;
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
   ptr = AlignColumnsFree(ptr);
   goto ret;
}



/**************************************************
*
*    AlignColumnsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AlignColumnsAsnWrite(AlignColumnsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objalignlocAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ALIGN_COLUMNS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> from;
   retval = AsnWrite(aip, ALIGN_COLUMNS_from,  &av);
   av.intvalue = ptr -> to;
   retval = AsnWrite(aip, ALIGN_COLUMNS_to,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

