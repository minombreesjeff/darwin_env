/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
* File Name: suggen.c
*
* Author:  Yuri Sadykov
*
* Version Creation Date:   08/14/95
*
* $Revision: 6.0 $
*
* File Description: 
*
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* ==========================================================================
*
*
* RCS Modification History:
*/

#define NLM_GENERATED_CODE_PROTO

#include "sugprefx.h"
#include "suggen.h"

static Boolean loaded = FALSE;

#include "sugmap.h"

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

Boolean LIBCALL
suggenAsnLoad(void)
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
*    Generated object loaders for Module NCBI-Suggest
*    Generated using ASNCODE Revision: 4.0 at Aug 16, 1995  2:17 PM
*
**************************************************/


/**************************************************
*
*    SuggestRequestFree()
*
**************************************************/

SuggestRequestPtr LIBCALL
SuggestRequestFree(ValNodePtr anp)
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
   case SuggestRequest_intervals:
      SuggestIntervalsFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    SuggestRequestAsnRead()
*
**************************************************/

SuggestRequestPtr LIBCALL
SuggestRequestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! suggenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* SuggestRequest ::= (self contained) */
      atp = AsnReadId(aip, amp, SUGGEST_REQUEST);
   } else {
      atp = AsnLinkType(orig, SUGGEST_REQUEST);    /* link in local tree */
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
   if (atp == SUGGEST_REQUEST_init) {
      choice = SuggestRequest_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == SUGGEST_REQUEST_intervals) {
      choice = SuggestRequest_intervals;
      func = (AsnReadFunc) SuggestIntervalsAsnRead;
   }
   else if (atp == SUGGEST_REQUEST_fini) {
      choice = SuggestRequest_fini;
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
*    SuggestRequestAsnWrite()
*
**************************************************/
Boolean LIBCALL 
SuggestRequestAsnWrite(SuggestRequestPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! suggenAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, SUGGEST_REQUEST);   /* link local tree */
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
   case SuggestRequest_init:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, SUGGEST_REQUEST_init, &av);
      break;
   case SuggestRequest_intervals:
      writetype = SUGGEST_REQUEST_intervals;
      func = (AsnWriteFunc) SuggestIntervalsAsnWrite;
      break;
   case SuggestRequest_fini:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, SUGGEST_REQUEST_fini, &av);
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
*    SuggestIntervalsNew()
*
**************************************************/

SuggestIntervalsPtr LIBCALL
SuggestIntervalsNew(void)
{
   SuggestIntervalsPtr ptr = MemNew((size_t) sizeof(SuggestIntervals));

   return ptr;

}


/**************************************************
*
*    SuggestIntervalsFree()
*
**************************************************/

SuggestIntervalsPtr LIBCALL
SuggestIntervalsFree(SuggestIntervalsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SuggestParametersFree(ptr -> params);
   SeqEntryFree(ptr -> dna);
   SeqEntryFree(ptr -> protein);
   return MemFree(ptr);
}


/**************************************************
*
*    SuggestIntervalsAsnRead()
*
**************************************************/

SuggestIntervalsPtr LIBCALL
SuggestIntervalsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   SuggestIntervalsPtr ptr;

   if (! loaded)
   {
      if (! suggenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* SuggestIntervals ::= (self contained) */
      atp = AsnReadId(aip, amp, SUGGEST_INTERVALS);
   } else {
      atp = AsnLinkType(orig, SUGGEST_INTERVALS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = SuggestIntervalsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == SUGGEST_INTERVALS_params) {
      ptr -> params = SuggestParametersAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SUGGEST_INTERVALS_dna) {
      ptr -> dna = SeqEntryAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SUGGEST_INTERVALS_protein) {
      ptr -> protein = SeqEntryAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SUGGEST_INTERVALS_code) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> code = av.intvalue;
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
   ptr = SuggestIntervalsFree(ptr);
   goto ret;
}



/**************************************************
*
*    SuggestIntervalsAsnWrite()
*
**************************************************/
Boolean LIBCALL 
SuggestIntervalsAsnWrite(SuggestIntervalsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! suggenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, SUGGEST_INTERVALS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> params != NULL) {
      if ( ! SuggestParametersAsnWrite(ptr -> params, aip, SUGGEST_INTERVALS_params)) {
         goto erret;
      }
   }
   if (ptr -> dna != NULL) {
      if ( ! SeqEntryAsnWrite(ptr -> dna, aip, SUGGEST_INTERVALS_dna)) {
         goto erret;
      }
   }
   if (ptr -> protein != NULL) {
      if ( ! SeqEntryAsnWrite(ptr -> protein, aip, SUGGEST_INTERVALS_protein)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> code;
   retval = AsnWrite(aip, SUGGEST_INTERVALS_code,  &av);
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
*    SuggestParametersNew()
*
**************************************************/

SuggestParametersPtr LIBCALL
SuggestParametersNew(void)
{
   SuggestParametersPtr ptr = MemNew((size_t) sizeof(SuggestParameters));

   return ptr;

}


/**************************************************
*
*    SuggestParametersFree()
*
**************************************************/

SuggestParametersPtr LIBCALL
SuggestParametersFree(SuggestParametersPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    SuggestParametersAsnRead()
*
**************************************************/

SuggestParametersPtr LIBCALL
SuggestParametersAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   SuggestParametersPtr ptr;

   if (! loaded)
   {
      if (! suggenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* SuggestParameters ::= (self contained) */
      atp = AsnReadId(aip, amp, SUGGEST_PARAMETERS);
   } else {
      atp = AsnLinkType(orig, SUGGEST_PARAMETERS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = SuggestParametersNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == SUGGEST_PARAMETERS_size) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> size = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SUGGEST_PARAMETERS_begin_search) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> begin_search = av.intvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SUGGEST_PARAMETERS_end_search) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> end_search = av.intvalue;
      ptr -> OBbits__ |= 1<<2;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SUGGEST_PARAMETERS_term_stop) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> term_stop = av.boolvalue;
      ptr -> OBbits__ |= 1<<3;
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
   ptr = SuggestParametersFree(ptr);
   goto ret;
}



/**************************************************
*
*    SuggestParametersAsnWrite()
*
**************************************************/
Boolean LIBCALL 
SuggestParametersAsnWrite(SuggestParametersPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! suggenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, SUGGEST_PARAMETERS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> size || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> size;
      retval = AsnWrite(aip, SUGGEST_PARAMETERS_size,  &av);
   }
   if (ptr -> begin_search || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> begin_search;
      retval = AsnWrite(aip, SUGGEST_PARAMETERS_begin_search,  &av);
   }
   if (ptr -> end_search || (ptr -> OBbits__ & (1<<2) )){   av.intvalue = ptr -> end_search;
      retval = AsnWrite(aip, SUGGEST_PARAMETERS_end_search,  &av);
   }
   if (ptr -> term_stop || (ptr -> OBbits__ & (1<<3) )){   av.boolvalue = ptr -> term_stop;
      retval = AsnWrite(aip, SUGGEST_PARAMETERS_term_stop,  &av);
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
*    SuggestErrorNew()
*
**************************************************/

SuggestErrorPtr LIBCALL
SuggestErrorNew(void)
{
   SuggestErrorPtr ptr = MemNew((size_t) sizeof(SuggestError));

   return ptr;

}


/**************************************************
*
*    SuggestErrorFree()
*
**************************************************/

SuggestErrorPtr LIBCALL
SuggestErrorFree(SuggestErrorPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> msg);
   return MemFree(ptr);
}


/**************************************************
*
*    SuggestErrorAsnRead()
*
**************************************************/

SuggestErrorPtr LIBCALL
SuggestErrorAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   SuggestErrorPtr ptr;

   if (! loaded)
   {
      if (! suggenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* SuggestError ::= (self contained) */
      atp = AsnReadId(aip, amp, SUGGEST_ERROR);
   } else {
      atp = AsnLinkType(orig, SUGGEST_ERROR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = SuggestErrorNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == SUGGEST_ERROR_level) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> level = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SUGGEST_ERROR_msg) {
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
   ptr = SuggestErrorFree(ptr);
   goto ret;
}



/**************************************************
*
*    SuggestErrorAsnWrite()
*
**************************************************/
Boolean LIBCALL 
SuggestErrorAsnWrite(SuggestErrorPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! suggenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, SUGGEST_ERROR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> level;
   retval = AsnWrite(aip, SUGGEST_ERROR_level,  &av);
   if (ptr -> msg != NULL) {
      av.ptrvalue = ptr -> msg;
      retval = AsnWrite(aip, SUGGEST_ERROR_msg,  &av);
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
*    SuggestResponseFree()
*
**************************************************/

SuggestResponsePtr LIBCALL
SuggestResponseFree(ValNodePtr anp)
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
   case SuggestResponse_error:
      SuggestErrorFree(anp -> data.ptrvalue);
      break;
   case SuggestResponse_intervals:
      SeqAnnotFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    SuggestResponseAsnRead()
*
**************************************************/

SuggestResponsePtr LIBCALL
SuggestResponseAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! suggenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* SuggestResponse ::= (self contained) */
      atp = AsnReadId(aip, amp, SUGGEST_RESPONSE);
   } else {
      atp = AsnLinkType(orig, SUGGEST_RESPONSE);    /* link in local tree */
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
   if (atp == SUGGEST_RESPONSE_init) {
      choice = SuggestResponse_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == SUGGEST_RESPONSE_error) {
      choice = SuggestResponse_error;
      func = (AsnReadFunc) SuggestErrorAsnRead;
   }
   else if (atp == SUGGEST_RESPONSE_intervals) {
      choice = SuggestResponse_intervals;
      func = (AsnReadFunc) SeqAnnotAsnRead;
   }
   else if (atp == SUGGEST_RESPONSE_fini) {
      choice = SuggestResponse_fini;
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
*    SuggestResponseAsnWrite()
*
**************************************************/
Boolean LIBCALL 
SuggestResponseAsnWrite(SuggestResponsePtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! suggenAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, SUGGEST_RESPONSE);   /* link local tree */
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
   case SuggestResponse_init:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, SUGGEST_RESPONSE_init, &av);
      break;
   case SuggestResponse_error:
      writetype = SUGGEST_RESPONSE_error;
      func = (AsnWriteFunc) SuggestErrorAsnWrite;
      break;
   case SuggestResponse_intervals:
      writetype = SUGGEST_RESPONSE_intervals;
      func = (AsnWriteFunc) SeqAnnotAsnWrite;
      break;
   case SuggestResponse_fini:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, SUGGEST_RESPONSE_fini, &av);
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
