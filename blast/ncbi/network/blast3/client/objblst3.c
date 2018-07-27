#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <blastpat.h>
#include <objblst3.h>

static Boolean loaded = FALSE;

#include <blstspc.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objblst3AsnLoad(void)
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
*    Generated object loaders for Module NCBI-Blast
*    Generated using ASNCODE Revision: 6.14 at Apr 16, 2002 11:50 AM
*
**************************************************/


/**************************************************
*
*    BlastSearchNew()
*
**************************************************/
NLM_EXTERN 
BlastSearchPtr LIBCALL
BlastSearchNew(void)
{
   BlastSearchPtr ptr = MemNew((size_t) sizeof(BlastSearch));

   return ptr;

}


/**************************************************
*
*    BlastSearchFree()
*
**************************************************/
NLM_EXTERN 
BlastSearchPtr LIBCALL
BlastSearchFree(BlastSearchPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   BioseqFree(ptr -> query);
   MemFree(ptr -> database);
   BlastParametersFree(ptr -> parameters);
   SeqLocFree(ptr -> mask);
   BlastMatrixFree(ptr -> matrix);
   BioseqSetFree(ptr -> query_set);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastSearchAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastSearchPtr LIBCALL
BlastSearchAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastSearchPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastSearch ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_SEARCH);
   } else {
      atp = AsnLinkType(orig, BLAST_SEARCH);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastSearchNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_SEARCH_program) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> program = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_SEARCH_query) {
      ptr -> query = BioseqAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_SEARCH_database) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> database = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_SEARCH_parameters) {
      ptr -> parameters = BlastParametersAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_SEARCH_mask) {
      ptr -> mask = SeqLocAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_SEARCH_matrix) {
      ptr -> matrix = BlastMatrixAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_SEARCH_return_parts) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> return_parts = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_SEARCH_query_set) {
      ptr -> query_set = BioseqSetAsnRead(aip, atp);
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
   ptr = BlastSearchFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastSearchAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastSearchAsnWrite(BlastSearchPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_SEARCH);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> program;
   retval = AsnWrite(aip, BLAST_SEARCH_program,  &av);
   if (ptr -> query != NULL) {
      if ( ! BioseqAsnWrite(ptr -> query, aip, BLAST_SEARCH_query)) {
         goto erret;
      }
   }
   if (ptr -> database != NULL) {
      av.ptrvalue = ptr -> database;
      retval = AsnWrite(aip, BLAST_SEARCH_database,  &av);
   }
   if (ptr -> parameters != NULL) {
      if ( ! BlastParametersAsnWrite(ptr -> parameters, aip, BLAST_SEARCH_parameters)) {
         goto erret;
      }
   }
   if (ptr -> mask != NULL) {
      if ( ! SeqLocAsnWrite(ptr -> mask, aip, BLAST_SEARCH_mask)) {
         goto erret;
      }
   }
   if (ptr -> matrix != NULL) {
      if ( ! BlastMatrixAsnWrite(ptr -> matrix, aip, BLAST_SEARCH_matrix)) {
         goto erret;
      }
   }
   av.boolvalue = ptr -> return_parts;
   retval = AsnWrite(aip, BLAST_SEARCH_return_parts,  &av);
   if (ptr -> query_set != NULL) {
      if ( ! BioseqSetAsnWrite(ptr -> query_set, aip, BLAST_SEARCH_query_set)) {
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
*    BlastRequestFree()
*
**************************************************/
NLM_EXTERN 
BlastRequestPtr LIBCALL
BlastRequestFree(ValNodePtr anp)
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
   case BlastRequest_init:
      MemFree(anp -> data.ptrvalue);
      break;
   case BlastRequest_db_info_specific:
      BlastDbinfoGetFree(anp -> data.ptrvalue);
      break;
   case BlastRequest_matrix_get:
      MemFree(anp -> data.ptrvalue);
      break;
   case BlastRequest_search:
      BlastSearchFree(anp -> data.ptrvalue);
      break;
   case BlastRequest_db_seq_get:
      BlastSeqIdFree(anp -> data.ptrvalue);
      break;
   case BlastRequest_db_redundant_ids_get:
      BlastSeqIdFree(anp -> data.ptrvalue);
      break;
   case BlastRequest_db_redundant_descr_get:
      BlastSeqIdFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    BlastRequestAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastRequestPtr LIBCALL
BlastRequestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastRequest ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_REQUEST);
   } else {
      atp = AsnLinkType(orig, BLAST_REQUEST);    /* link in local tree */
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
   if (atp == BLAST_REQUEST_init) {
      choice = BlastRequest_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BLAST_REQUEST_motd) {
      choice = BlastRequest_motd;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == BLAST_REQUEST_db_info) {
      choice = BlastRequest_db_info;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == BLAST_REQUEST_db_info_specific) {
      choice = BlastRequest_db_info_specific;
      func = (AsnReadFunc) BlastDbinfoGetAsnRead;
   }
   else if (atp == BLAST_REQUEST_matrix_get) {
      choice = BlastRequest_matrix_get;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BLAST_REQUEST_search) {
      choice = BlastRequest_search;
      func = (AsnReadFunc) BlastSearchAsnRead;
   }
   else if (atp == BLAST_REQUEST_db_seq_get) {
      choice = BlastRequest_db_seq_get;
      func = (AsnReadFunc) BlastSeqIdAsnRead;
   }
   else if (atp == BLAST_REQUEST_db_redundant_ids_get) {
      choice = BlastRequest_db_redundant_ids_get;
      func = (AsnReadFunc) BlastSeqIdAsnRead;
   }
   else if (atp == BLAST_REQUEST_db_redundant_descr_get) {
      choice = BlastRequest_db_redundant_descr_get;
      func = (AsnReadFunc) BlastSeqIdAsnRead;
   }
   else if (atp == BLAST_REQUEST_fini) {
      choice = BlastRequest_fini;
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
*    BlastRequestAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastRequestAsnWrite(BlastRequestPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, BLAST_REQUEST);   /* link local tree */
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
   case BlastRequest_init:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BLAST_REQUEST_init, &av);
      break;
   case BlastRequest_motd:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, BLAST_REQUEST_motd, &av);
      break;
   case BlastRequest_db_info:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, BLAST_REQUEST_db_info, &av);
      break;
   case BlastRequest_db_info_specific:
      writetype = BLAST_REQUEST_db_info_specific;
      func = (AsnWriteFunc) BlastDbinfoGetAsnWrite;
      break;
   case BlastRequest_matrix_get:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BLAST_REQUEST_matrix_get, &av);
      break;
   case BlastRequest_search:
      writetype = BLAST_REQUEST_search;
      func = (AsnWriteFunc) BlastSearchAsnWrite;
      break;
   case BlastRequest_db_seq_get:
      writetype = BLAST_REQUEST_db_seq_get;
      func = (AsnWriteFunc) BlastSeqIdAsnWrite;
      break;
   case BlastRequest_db_redundant_ids_get:
      writetype = BLAST_REQUEST_db_redundant_ids_get;
      func = (AsnWriteFunc) BlastSeqIdAsnWrite;
      break;
   case BlastRequest_db_redundant_descr_get:
      writetype = BLAST_REQUEST_db_redundant_descr_get;
      func = (AsnWriteFunc) BlastSeqIdAsnWrite;
      break;
   case BlastRequest_fini:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, BLAST_REQUEST_fini, &av);
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
*    BlastResponseFree()
*
**************************************************/
NLM_EXTERN 
BlastResponsePtr LIBCALL
BlastResponseFree(ValNodePtr anp)
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
   case BlastResponse_init:
      BlastVersionFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_motd:
      MemFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_error:
      BlastErrorFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_db_seq_get:
      BioseqFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_db_redundant_ids_get:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqIdFree);
      break;
   case BlastResponse_db_redundant_descr_get:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) BlastDeflineFree);
      break;
   case BlastResponse_db_info:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) BlastDbinfoFree);
      break;
   case BlastResponse_db_info_specific:
      BlastDbinfoFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_matrix:
      BlastMatrixFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_alignment:
      SeqAlignSetFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_mask:
      BlastMaskFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_kablk:
      BlastKABlkFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_parameters:
      MemFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_queued:
      BlastQueuedFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_start:
      BlastProgressFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_progress:
      BlastProgressFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_done:
      BlastProgressFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_phialign:
      BlastPhialignFree(anp -> data.ptrvalue);
      break;
   case BlastResponse_parts:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) BlastPartsFree);
      break;
   case BlastResponse_mbalign:
      MegaBlastResultsFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    BlastResponseAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastResponsePtr LIBCALL
BlastResponseAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastResponse ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_RESPONSE);
   } else {
      atp = AsnLinkType(orig, BLAST_RESPONSE);    /* link in local tree */
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
   if (atp == BLAST_RESPONSE_init) {
      choice = BlastResponse_init;
      func = (AsnReadFunc) BlastVersionAsnRead;
   }
   else if (atp == BLAST_RESPONSE_motd) {
      choice = BlastResponse_motd;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BLAST_RESPONSE_error) {
      choice = BlastResponse_error;
      func = (AsnReadFunc) BlastErrorAsnRead;
   }
   else if (atp == BLAST_RESPONSE_db_seq_get) {
      choice = BlastResponse_db_seq_get;
      func = (AsnReadFunc) BioseqAsnRead;
   }
   else if (atp == BLAST_RESPONSE_db_redundant_ids_get) {
      choice = BlastResponse_db_redundant_ids_get;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqIdAsnRead,             (AsnOptFreeFunc) SeqIdFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == BLAST_RESPONSE_db_redundant_descr_get) {
      choice = BlastResponse_db_redundant_descr_get;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BlastDeflineAsnRead,             (AsnOptFreeFunc) BlastDeflineFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == BLAST_RESPONSE_db_info) {
      choice = BlastResponse_db_info;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BlastDbinfoAsnRead,             (AsnOptFreeFunc) BlastDbinfoFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == BLAST_RESPONSE_db_info_specific) {
      choice = BlastResponse_db_info_specific;
      func = (AsnReadFunc) BlastDbinfoAsnRead;
   }
   else if (atp == BLAST_RESPONSE_matrix) {
      choice = BlastResponse_matrix;
      func = (AsnReadFunc) BlastMatrixAsnRead;
   }
   else if (atp == BLAST_RESPONSE_alignment) {
      choice = BlastResponse_alignment;
      func = (AsnReadFunc) SeqAlignSetAsnRead;
   }
   else if (atp == BLAST_RESPONSE_mask) {
      choice = BlastResponse_mask;
      func = (AsnReadFunc) BlastMaskAsnRead;
   }
   else if (atp == BLAST_RESPONSE_kablk) {
      choice = BlastResponse_kablk;
      func = (AsnReadFunc) BlastKABlkAsnRead;
   }
   else if (atp == BLAST_RESPONSE_parameters) {
      choice = BlastResponse_parameters;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BLAST_RESPONSE_queued) {
      choice = BlastResponse_queued;
      func = (AsnReadFunc) BlastQueuedAsnRead;
   }
   else if (atp == BLAST_RESPONSE_start) {
      choice = BlastResponse_start;
      func = (AsnReadFunc) BlastProgressAsnRead;
   }
   else if (atp == BLAST_RESPONSE_progress) {
      choice = BlastResponse_progress;
      func = (AsnReadFunc) BlastProgressAsnRead;
   }
   else if (atp == BLAST_RESPONSE_done) {
      choice = BlastResponse_done;
      func = (AsnReadFunc) BlastProgressAsnRead;
   }
   else if (atp == BLAST_RESPONSE_fini) {
      choice = BlastResponse_fini;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == BLAST_RESPONSE_phialign) {
      choice = BlastResponse_phialign;
      func = (AsnReadFunc) BlastPhialignAsnRead;
   }
   else if (atp == BLAST_RESPONSE_parts) {
      choice = BlastResponse_parts;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BlastPartsAsnRead,             (AsnOptFreeFunc) BlastPartsFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == BLAST_RESPONSE_mbalign) {
      choice = BlastResponse_mbalign;
      func = (AsnReadFunc) MegaBlastResultsAsnRead;
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
*    BlastResponseAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastResponseAsnWrite(BlastResponsePtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, BLAST_RESPONSE);   /* link local tree */
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
   case BlastResponse_init:
      writetype = BLAST_RESPONSE_init;
      func = (AsnWriteFunc) BlastVersionAsnWrite;
      break;
   case BlastResponse_motd:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BLAST_RESPONSE_motd, &av);
      break;
   case BlastResponse_error:
      writetype = BLAST_RESPONSE_error;
      func = (AsnWriteFunc) BlastErrorAsnWrite;
      break;
   case BlastResponse_db_seq_get:
      writetype = BLAST_RESPONSE_db_seq_get;
      func = (AsnWriteFunc) BioseqAsnWrite;
      break;
   case BlastResponse_db_redundant_ids_get:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqIdAsnWrite, aip, BLAST_RESPONSE_db_redundant_ids_get, BLAST_RESPONSE_db_redundant_ids_get_E);
      break;
   case BlastResponse_db_redundant_descr_get:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) BlastDeflineAsnWrite, aip, BLAST_RESPONSE_db_redundant_descr_get, BLAST_RESPONSE_db_redundant_descr_get_E);
      break;
   case BlastResponse_db_info:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) BlastDbinfoAsnWrite, aip, BLAST_RESPONSE_db_info, BLAST_RESPONSE_db_info_E);
      break;
   case BlastResponse_db_info_specific:
      writetype = BLAST_RESPONSE_db_info_specific;
      func = (AsnWriteFunc) BlastDbinfoAsnWrite;
      break;
   case BlastResponse_matrix:
      writetype = BLAST_RESPONSE_matrix;
      func = (AsnWriteFunc) BlastMatrixAsnWrite;
      break;
   case BlastResponse_alignment:
      writetype = BLAST_RESPONSE_alignment;
      func = (AsnWriteFunc) SeqAlignSetAsnWrite;
      break;
   case BlastResponse_mask:
      writetype = BLAST_RESPONSE_mask;
      func = (AsnWriteFunc) BlastMaskAsnWrite;
      break;
   case BlastResponse_kablk:
      writetype = BLAST_RESPONSE_kablk;
      func = (AsnWriteFunc) BlastKABlkAsnWrite;
      break;
   case BlastResponse_parameters:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BLAST_RESPONSE_parameters, &av);
      break;
   case BlastResponse_queued:
      writetype = BLAST_RESPONSE_queued;
      func = (AsnWriteFunc) BlastQueuedAsnWrite;
      break;
   case BlastResponse_start:
      writetype = BLAST_RESPONSE_start;
      func = (AsnWriteFunc) BlastProgressAsnWrite;
      break;
   case BlastResponse_progress:
      writetype = BLAST_RESPONSE_progress;
      func = (AsnWriteFunc) BlastProgressAsnWrite;
      break;
   case BlastResponse_done:
      writetype = BLAST_RESPONSE_done;
      func = (AsnWriteFunc) BlastProgressAsnWrite;
      break;
   case BlastResponse_fini:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, BLAST_RESPONSE_fini, &av);
      break;
   case BlastResponse_phialign:
      writetype = BLAST_RESPONSE_phialign;
      func = (AsnWriteFunc) BlastPhialignAsnWrite;
      break;
   case BlastResponse_parts:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) BlastPartsAsnWrite, aip, BLAST_RESPONSE_parts, BLAST_RESPONSE_parts_E);
      break;
   case BlastResponse_mbalign:
      writetype = BLAST_RESPONSE_mbalign;
      func = (AsnWriteFunc) MegaBlastResultsAsnWrite;
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
*    BlastParametersNew()
*
**************************************************/
NLM_EXTERN 
BlastParametersPtr LIBCALL
BlastParametersNew(void)
{
   BlastParametersPtr ptr = MemNew((size_t) sizeof(BlastParameters));

   return ptr;

}


/**************************************************
*
*    BlastParametersFree()
*
**************************************************/
NLM_EXTERN 
BlastParametersPtr LIBCALL
BlastParametersFree(BlastParametersPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Cutoff_cutoffFree(ptr -> Cutoff_cutoff);
   Cutoff2_cutoff2Free(ptr -> Cutoff2_cutoff2);
   MemFree(ptr -> other_options);
   AsnGenericBaseSeqOfFree(ptr -> gilist ,ASNCODE_INTVAL_SLOT);
   MemFree(ptr -> gifile);
   MemFree(ptr -> matrix);
   MemFree(ptr -> filter_string);
   MemFree(ptr -> entrez_query);
   MemFree(ptr -> phi_pattern);
   AsnGenericChoiceSeqOfFree(ptr -> query_lcase_mask, (AsnOptFreeFunc) SeqLocFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Cutoff2_cutoff2Free()
*
**************************************************/
static 
Cutoff2_cutoff2Ptr LIBCALL
Cutoff2_cutoff2Free(ValNodePtr anp)
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
   }
   return MemFree(anp);
}


/**************************************************
*
*    BlastParametersAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastParametersPtr LIBCALL
BlastParametersAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastParametersPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastParameters ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_PARAMETERS);
   } else {
      atp = AsnLinkType(orig, BLAST_PARAMETERS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastParametersNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_PARAMETERS_first_threshold) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> first_threshold = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_second_threshold) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> second_threshold = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_cutoff) {
      ptr -> Cutoff_cutoff = Cutoff_cutoffAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_cutoff2) {
      ptr -> Cutoff2_cutoff2 = Cutoff2_cutoff2AsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_hitlist_size) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hitlist_size = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_nucl_penalty) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> nucl_penalty = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_nucl_reward) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> nucl_reward = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_genetic_code) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> genetic_code = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_db_genetic_code) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db_genetic_code = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_low_complexity_filtering) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> low_complexity_filtering = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_gapped_alignment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gapped_alignment = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_gap_open) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gap_open = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_gap_extend) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gap_extend = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_required_start) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> required_start = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_required_end) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> required_end = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_ethresh) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ethresh = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_max_num_passes) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> max_num_passes = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_pseudo_count_const) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pseudo_count_const = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_other_options) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> other_options = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_gilist) {
      ptr -> gilist = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> gilist == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_gifile) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gifile = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_matrix) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> matrix = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_filter_string) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> filter_string = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_entrez_query) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> entrez_query = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_word_size) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> word_size = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_db_length) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db_length = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_searchsp_eff) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> searchsp_eff = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_hsp_range_max) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hsp_range_max = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_block_width) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> block_width = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_perform_culling) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> perform_culling = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_strand_option) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> strand_option = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_phi_pattern) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> phi_pattern = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_use_real_db_size) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> use_real_db_size = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_use_best_align) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> use_best_align = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_is_rps_blast) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_rps_blast = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_tweak_parameters) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tweak_parameters = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_smith_waterman) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> smith_waterman = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_is_megablast) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_megablast = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_query_lcase_mask) {
      ptr -> query_lcase_mask = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqLocAsnRead, (AsnOptFreeFunc) SeqLocFree);
      if (isError && ptr -> query_lcase_mask == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_is_ooframe) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_ooframe = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_endpoint_results) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> endpoint_results = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_percent_identity) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> percent_identity = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_first_db_seq) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> first_db_seq = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_final_db_seq) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> final_db_seq = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_window_size) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> window_size = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_mb_template_length) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> mb_template_length = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARAMETERS_mb_disc_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> mb_disc_type = av.intvalue;
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
   ptr = BlastParametersFree(ptr);
   goto ret;
}



/**************************************************
*
*    Cutoff2_cutoff2AsnRead()
*
**************************************************/
static 
Cutoff2_cutoff2Ptr LIBCALL
Cutoff2_cutoff2AsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cutoff2_cutoff2 ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_PARAMETERS_cutoff2);
   } else {
      atp = AsnLinkType(orig, BLAST_PARAMETERS_cutoff2);    /* link in local tree */
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
   if (atp == BLAST_PARAMETERS_cutoff2_evalue) {
      choice = Cutoff2_cutoff2_evalue;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.realvalue = av.realvalue;
   }
   else if (atp == BLAST_PARAMETERS_cutoff2_score) {
      choice = Cutoff2_cutoff2_score;
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
*    BlastParametersAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastParametersAsnWrite(BlastParametersPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_PARAMETERS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> first_threshold;
   retval = AsnWrite(aip, BLAST_PARAMETERS_first_threshold,  &av);
   av.intvalue = ptr -> second_threshold;
   retval = AsnWrite(aip, BLAST_PARAMETERS_second_threshold,  &av);
   if (ptr -> Cutoff_cutoff != NULL) {
      if ( ! Cutoff_cutoffAsnWrite(ptr -> Cutoff_cutoff, aip, BLAST_PARAMETERS_cutoff)) {
         goto erret;
      }
   }
   if (ptr -> Cutoff2_cutoff2 != NULL) {
      if ( ! Cutoff2_cutoff2AsnWrite(ptr -> Cutoff2_cutoff2, aip, BLAST_PARAMETERS_cutoff2)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> hitlist_size;
   retval = AsnWrite(aip, BLAST_PARAMETERS_hitlist_size,  &av);
   av.intvalue = ptr -> nucl_penalty;
   retval = AsnWrite(aip, BLAST_PARAMETERS_nucl_penalty,  &av);
   av.intvalue = ptr -> nucl_reward;
   retval = AsnWrite(aip, BLAST_PARAMETERS_nucl_reward,  &av);
   av.intvalue = ptr -> genetic_code;
   retval = AsnWrite(aip, BLAST_PARAMETERS_genetic_code,  &av);
   av.intvalue = ptr -> db_genetic_code;
   retval = AsnWrite(aip, BLAST_PARAMETERS_db_genetic_code,  &av);
   av.intvalue = ptr -> low_complexity_filtering;
   retval = AsnWrite(aip, BLAST_PARAMETERS_low_complexity_filtering,  &av);
   av.boolvalue = ptr -> gapped_alignment;
   retval = AsnWrite(aip, BLAST_PARAMETERS_gapped_alignment,  &av);
   av.intvalue = ptr -> gap_open;
   retval = AsnWrite(aip, BLAST_PARAMETERS_gap_open,  &av);
   av.intvalue = ptr -> gap_extend;
   retval = AsnWrite(aip, BLAST_PARAMETERS_gap_extend,  &av);
   av.intvalue = ptr -> required_start;
   retval = AsnWrite(aip, BLAST_PARAMETERS_required_start,  &av);
   av.intvalue = ptr -> required_end;
   retval = AsnWrite(aip, BLAST_PARAMETERS_required_end,  &av);
   av.realvalue = ptr -> ethresh;
   retval = AsnWrite(aip, BLAST_PARAMETERS_ethresh,  &av);
   av.intvalue = ptr -> max_num_passes;
   retval = AsnWrite(aip, BLAST_PARAMETERS_max_num_passes,  &av);
   av.intvalue = ptr -> pseudo_count_const;
   retval = AsnWrite(aip, BLAST_PARAMETERS_pseudo_count_const,  &av);
   if (ptr -> other_options != NULL) {
      av.ptrvalue = ptr -> other_options;
      retval = AsnWrite(aip, BLAST_PARAMETERS_other_options,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> gilist ,ASNCODE_INTVAL_SLOT, aip, BLAST_PARAMETERS_gilist, BLAST_PARAMETERS_gilist_E);
   if (ptr -> gifile != NULL) {
      av.ptrvalue = ptr -> gifile;
      retval = AsnWrite(aip, BLAST_PARAMETERS_gifile,  &av);
   }
   if (ptr -> matrix != NULL) {
      av.ptrvalue = ptr -> matrix;
      retval = AsnWrite(aip, BLAST_PARAMETERS_matrix,  &av);
   }
   if (ptr -> filter_string != NULL) {
      av.ptrvalue = ptr -> filter_string;
      retval = AsnWrite(aip, BLAST_PARAMETERS_filter_string,  &av);
   }
   if (ptr -> entrez_query != NULL) {
      av.ptrvalue = ptr -> entrez_query;
      retval = AsnWrite(aip, BLAST_PARAMETERS_entrez_query,  &av);
   }
   av.intvalue = ptr -> word_size;
   retval = AsnWrite(aip, BLAST_PARAMETERS_word_size,  &av);
   av.intvalue = ptr -> db_length;
   retval = AsnWrite(aip, BLAST_PARAMETERS_db_length,  &av);
   av.realvalue = ptr -> searchsp_eff;
   retval = AsnWrite(aip, BLAST_PARAMETERS_searchsp_eff,  &av);
   av.intvalue = ptr -> hsp_range_max;
   retval = AsnWrite(aip, BLAST_PARAMETERS_hsp_range_max,  &av);
   av.intvalue = ptr -> block_width;
   retval = AsnWrite(aip, BLAST_PARAMETERS_block_width,  &av);
   av.boolvalue = ptr -> perform_culling;
   retval = AsnWrite(aip, BLAST_PARAMETERS_perform_culling,  &av);
   av.intvalue = ptr -> strand_option;
   retval = AsnWrite(aip, BLAST_PARAMETERS_strand_option,  &av);
   if (ptr -> phi_pattern != NULL) {
      av.ptrvalue = ptr -> phi_pattern;
      retval = AsnWrite(aip, BLAST_PARAMETERS_phi_pattern,  &av);
   }
   av.boolvalue = ptr -> use_real_db_size;
   retval = AsnWrite(aip, BLAST_PARAMETERS_use_real_db_size,  &av);
   av.boolvalue = ptr -> use_best_align;
   retval = AsnWrite(aip, BLAST_PARAMETERS_use_best_align,  &av);
   av.boolvalue = ptr -> is_rps_blast;
   retval = AsnWrite(aip, BLAST_PARAMETERS_is_rps_blast,  &av);
   av.boolvalue = ptr -> tweak_parameters;
   retval = AsnWrite(aip, BLAST_PARAMETERS_tweak_parameters,  &av);
   av.boolvalue = ptr -> smith_waterman;
   retval = AsnWrite(aip, BLAST_PARAMETERS_smith_waterman,  &av);
   av.boolvalue = ptr -> is_megablast;
   retval = AsnWrite(aip, BLAST_PARAMETERS_is_megablast,  &av);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> query_lcase_mask, (AsnWriteFunc) SeqLocAsnWrite, aip, BLAST_PARAMETERS_query_lcase_mask, BLAST_PARAMETERS_query_lcase_mask_E);
   av.intvalue = ptr -> is_ooframe;
   retval = AsnWrite(aip, BLAST_PARAMETERS_is_ooframe,  &av);
   av.boolvalue = ptr -> endpoint_results;
   retval = AsnWrite(aip, BLAST_PARAMETERS_endpoint_results,  &av);
   av.realvalue = ptr -> percent_identity;
   retval = AsnWrite(aip, BLAST_PARAMETERS_percent_identity,  &av);
   av.intvalue = ptr -> first_db_seq;
   retval = AsnWrite(aip, BLAST_PARAMETERS_first_db_seq,  &av);
   av.intvalue = ptr -> final_db_seq;
   retval = AsnWrite(aip, BLAST_PARAMETERS_final_db_seq,  &av);
   av.intvalue = ptr -> window_size;
   retval = AsnWrite(aip, BLAST_PARAMETERS_window_size,  &av);
   av.intvalue = ptr -> mb_template_length;
   retval = AsnWrite(aip, BLAST_PARAMETERS_mb_template_length,  &av);
   av.intvalue = ptr -> mb_disc_type;
   retval = AsnWrite(aip, BLAST_PARAMETERS_mb_disc_type,  &av);
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
*    Cutoff2_cutoff2AsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Cutoff2_cutoff2AsnWrite(Cutoff2_cutoff2Ptr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, BLAST_PARAMETERS_cutoff2);   /* link local tree */
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
   case Cutoff2_cutoff2_evalue:
      av.realvalue = anp->data.realvalue;
      retval = AsnWrite(aip, BLAST_PARAMETERS_cutoff2_evalue, &av);
      break;
   case Cutoff2_cutoff2_score:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, BLAST_PARAMETERS_cutoff2_score, &av);
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
*    Cutoff_cutoffAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Cutoff_cutoffAsnWrite(Cutoff_cutoffPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, BLAST_PARAMETERS_cutoff);   /* link local tree */
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
   case Cutoff_cutoff_evalue:
      av.realvalue = anp->data.realvalue;
      retval = AsnWrite(aip, BLAST_PARAMETERS_cutoff_evalue, &av);
      break;
   case Cutoff_cutoff_score:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, BLAST_PARAMETERS_cutoff_score, &av);
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
*    Cutoff_cutoffAsnRead()
*
**************************************************/
static 
Cutoff_cutoffPtr LIBCALL
Cutoff_cutoffAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cutoff_cutoff ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_PARAMETERS_cutoff);
   } else {
      atp = AsnLinkType(orig, BLAST_PARAMETERS_cutoff);    /* link in local tree */
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
   if (atp == BLAST_PARAMETERS_cutoff_evalue) {
      choice = Cutoff_cutoff_evalue;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.realvalue = av.realvalue;
   }
   else if (atp == BLAST_PARAMETERS_cutoff_score) {
      choice = Cutoff_cutoff_score;
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
*    Cutoff_cutoffFree()
*
**************************************************/
static 
Cutoff_cutoffPtr LIBCALL
Cutoff_cutoffFree(ValNodePtr anp)
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
   }
   return MemFree(anp);
}


/**************************************************
*
*    BlastMatrixNew()
*
**************************************************/
NLM_EXTERN 
BlastMatrixPtr LIBCALL
BlastMatrixNew(void)
{
   BlastMatrixPtr ptr = MemNew((size_t) sizeof(BlastMatrix));

   return ptr;

}


/**************************************************
*
*    BlastMatrixFree()
*
**************************************************/
NLM_EXTERN 
BlastMatrixPtr LIBCALL
BlastMatrixFree(BlastMatrixPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   AsnGenericBaseSeqOfFree(ptr -> comments ,ASNCODE_PTRVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> scores ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> posFreqs ,ASNCODE_REALVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastMatrixAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastMatrixPtr LIBCALL
BlastMatrixAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastMatrixPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastMatrix ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_MATRIX);
   } else {
      atp = AsnLinkType(orig, BLAST_MATRIX);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastMatrixNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_MATRIX_is_protein) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_protein = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_MATRIX_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_MATRIX_comments) {
      ptr -> comments = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> comments == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_MATRIX_row_length) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> row_length = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_MATRIX_column_length) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> column_length = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_MATRIX_scores) {
      ptr -> scores = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> scores == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_MATRIX_karlinK) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> karlinK = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_MATRIX_posFreqs) {
      ptr -> posFreqs = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_REALVAL_SLOT, &isError);
      if (isError && ptr -> posFreqs == NULL) {
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
   ptr = BlastMatrixFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastMatrixAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastMatrixAsnWrite(BlastMatrixPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_MATRIX);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.boolvalue = ptr -> is_protein;
   retval = AsnWrite(aip, BLAST_MATRIX_is_protein,  &av);
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, BLAST_MATRIX_name,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> comments ,ASNCODE_PTRVAL_SLOT, aip, BLAST_MATRIX_comments, BLAST_MATRIX_comments_E);
   av.intvalue = ptr -> row_length;
   retval = AsnWrite(aip, BLAST_MATRIX_row_length,  &av);
   av.intvalue = ptr -> column_length;
   retval = AsnWrite(aip, BLAST_MATRIX_column_length,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> scores ,ASNCODE_INTVAL_SLOT, aip, BLAST_MATRIX_scores, BLAST_MATRIX_scores_E);
   av.realvalue = ptr -> karlinK;
   retval = AsnWrite(aip, BLAST_MATRIX_karlinK,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> posFreqs ,ASNCODE_REALVAL_SLOT, aip, BLAST_MATRIX_posFreqs, BLAST_MATRIX_posFreqs_E);
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
*    BlastDbinfoNew()
*
**************************************************/
NLM_EXTERN 
BlastDbinfoPtr LIBCALL
BlastDbinfoNew(void)
{
   BlastDbinfoPtr ptr = MemNew((size_t) sizeof(BlastDbinfo));

   return ptr;

}


/**************************************************
*
*    BlastDbinfoFree()
*
**************************************************/
NLM_EXTERN 
BlastDbinfoPtr LIBCALL
BlastDbinfoFree(BlastDbinfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   MemFree(ptr -> definition);
   MemFree(ptr -> date);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastDbinfoAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastDbinfoPtr LIBCALL
BlastDbinfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastDbinfoPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastDbinfo ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_DBINFO);
   } else {
      atp = AsnLinkType(orig, BLAST_DBINFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastDbinfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_DBINFO_is_protein) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_protein = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DBINFO_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DBINFO_definition) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> definition = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DBINFO_date) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> date = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DBINFO_total_length) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> total_length = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DBINFO_number_seqs) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number_seqs = av.intvalue;
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
   ptr = BlastDbinfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastDbinfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastDbinfoAsnWrite(BlastDbinfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_DBINFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.boolvalue = ptr -> is_protein;
   retval = AsnWrite(aip, BLAST_DBINFO_is_protein,  &av);
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, BLAST_DBINFO_name,  &av);
   }
   if (ptr -> definition != NULL) {
      av.ptrvalue = ptr -> definition;
      retval = AsnWrite(aip, BLAST_DBINFO_definition,  &av);
   }
   if (ptr -> date != NULL) {
      av.ptrvalue = ptr -> date;
      retval = AsnWrite(aip, BLAST_DBINFO_date,  &av);
   }
   av.intvalue = ptr -> total_length;
   retval = AsnWrite(aip, BLAST_DBINFO_total_length,  &av);
   av.intvalue = ptr -> number_seqs;
   retval = AsnWrite(aip, BLAST_DBINFO_number_seqs,  &av);
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
*    BlastMaskNew()
*
**************************************************/
NLM_EXTERN 
BlastMaskPtr LIBCALL
BlastMaskNew(void)
{
   BlastMaskPtr ptr = MemNew((size_t) sizeof(BlastMask));

   return ptr;

}


/**************************************************
*
*    BlastMaskFree()
*
**************************************************/
NLM_EXTERN 
BlastMaskPtr LIBCALL
BlastMaskFree(BlastMaskPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> location, (AsnOptFreeFunc) SeqLocFree);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastMaskAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastMaskPtr LIBCALL
BlastMaskAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastMaskPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastMask ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_MASK);
   } else {
      atp = AsnLinkType(orig, BLAST_MASK);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastMaskNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_MASK_location) {
      ptr -> location = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqLocAsnRead, (AsnOptFreeFunc) SeqLocFree);
      if (isError && ptr -> location == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_MASK_frame) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> frame = av.intvalue;
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
   ptr = BlastMaskFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastMaskAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastMaskAsnWrite(BlastMaskPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_MASK);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericChoiceSeqOfAsnWrite(ptr -> location, (AsnWriteFunc) SeqLocAsnWrite, aip, BLAST_MASK_location, BLAST_MASK_location_E);
   av.intvalue = ptr -> frame;
   retval = AsnWrite(aip, BLAST_MASK_frame,  &av);
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
*    BlastKABlkNew()
*
**************************************************/
NLM_EXTERN 
BlastKABlkPtr LIBCALL
BlastKABlkNew(void)
{
   BlastKABlkPtr ptr = MemNew((size_t) sizeof(BlastKABlk));

   return ptr;

}


/**************************************************
*
*    BlastKABlkFree()
*
**************************************************/
NLM_EXTERN 
BlastKABlkPtr LIBCALL
BlastKABlkFree(BlastKABlkPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    BlastKABlkAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastKABlkPtr LIBCALL
BlastKABlkAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastKABlkPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastKABlk ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_KABLK);
   } else {
      atp = AsnLinkType(orig, BLAST_KABLK);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastKABlkNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_KABLK_lambda) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> lambda = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_KABLK_k) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> k = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_KABLK_h) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> h = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_KABLK_gapped) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gapped = av.boolvalue;
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
   ptr = BlastKABlkFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastKABlkAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastKABlkAsnWrite(BlastKABlkPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_KABLK);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.realvalue = ptr -> lambda;
   retval = AsnWrite(aip, BLAST_KABLK_lambda,  &av);
   av.realvalue = ptr -> k;
   retval = AsnWrite(aip, BLAST_KABLK_k,  &av);
   av.realvalue = ptr -> h;
   retval = AsnWrite(aip, BLAST_KABLK_h,  &av);
   av.boolvalue = ptr -> gapped;
   retval = AsnWrite(aip, BLAST_KABLK_gapped,  &av);
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
*    BlastErrorNew()
*
**************************************************/
NLM_EXTERN 
BlastErrorPtr LIBCALL
BlastErrorNew(void)
{
   BlastErrorPtr ptr = MemNew((size_t) sizeof(BlastError));

   return ptr;

}


/**************************************************
*
*    BlastErrorFree()
*
**************************************************/
NLM_EXTERN 
BlastErrorPtr LIBCALL
BlastErrorFree(BlastErrorPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> msg);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastErrorAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastErrorPtr LIBCALL
BlastErrorAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastErrorPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastError ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_ERROR);
   } else {
      atp = AsnLinkType(orig, BLAST_ERROR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastErrorNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_ERROR_level) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> level = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_ERROR_msg) {
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
   ptr = BlastErrorFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastErrorAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastErrorAsnWrite(BlastErrorPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_ERROR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> level;
   retval = AsnWrite(aip, BLAST_ERROR_level,  &av);
   if (ptr -> msg != NULL) {
      av.ptrvalue = ptr -> msg;
      retval = AsnWrite(aip, BLAST_ERROR_msg,  &av);
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
*    BlastPhialignNew()
*
**************************************************/
NLM_EXTERN 
BlastPhialignPtr LIBCALL
BlastPhialignNew(void)
{
   BlastPhialignPtr ptr = MemNew((size_t) sizeof(BlastPhialign));

   return ptr;

}


/**************************************************
*
*    BlastPhialignFree()
*
**************************************************/
NLM_EXTERN 
BlastPhialignPtr LIBCALL
BlastPhialignFree(BlastPhialignPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> seqloc, (AsnOptFreeFunc) SeqLocFree);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastPhialignAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastPhialignPtr LIBCALL
BlastPhialignAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastPhialignPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastPhialign ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_PHIALIGN);
   } else {
      atp = AsnLinkType(orig, BLAST_PHIALIGN);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastPhialignNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_PHIALIGN_numaligns) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> numaligns = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PHIALIGN_seqloc) {
      ptr -> seqloc = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqLocAsnRead, (AsnOptFreeFunc) SeqLocFree);
      if (isError && ptr -> seqloc == NULL) {
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
   ptr = BlastPhialignFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastPhialignAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastPhialignAsnWrite(BlastPhialignPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_PHIALIGN);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> numaligns;
   retval = AsnWrite(aip, BLAST_PHIALIGN_numaligns,  &av);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> seqloc, (AsnWriteFunc) SeqLocAsnWrite, aip, BLAST_PHIALIGN_seqloc, BLAST_PHIALIGN_seqloc_E);
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
*    BlastDbinfoGetNew()
*
**************************************************/
NLM_EXTERN 
BlastDbinfoGetPtr LIBCALL
BlastDbinfoGetNew(void)
{
   BlastDbinfoGetPtr ptr = MemNew((size_t) sizeof(BlastDbinfoGet));

   return ptr;

}


/**************************************************
*
*    BlastDbinfoGetFree()
*
**************************************************/
NLM_EXTERN 
BlastDbinfoGetPtr LIBCALL
BlastDbinfoGetFree(BlastDbinfoGetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastDbinfoGetAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastDbinfoGetPtr LIBCALL
BlastDbinfoGetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastDbinfoGetPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastDbinfoGet ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_DBINFO_GET);
   } else {
      atp = AsnLinkType(orig, BLAST_DBINFO_GET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastDbinfoGetNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_DBINFO_GET_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DBINFO_GET_type) {
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
   ptr = BlastDbinfoGetFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastDbinfoGetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastDbinfoGetAsnWrite(BlastDbinfoGetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_DBINFO_GET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, BLAST_DBINFO_GET_name,  &av);
   }
   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, BLAST_DBINFO_GET_type,  &av);
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
*    BlastSeqIdNew()
*
**************************************************/
NLM_EXTERN 
BlastSeqIdPtr LIBCALL
BlastSeqIdNew(void)
{
   BlastSeqIdPtr ptr = MemNew((size_t) sizeof(BlastSeqId));

   return ptr;

}


/**************************************************
*
*    BlastSeqIdFree()
*
**************************************************/
NLM_EXTERN 
BlastSeqIdPtr LIBCALL
BlastSeqIdFree(BlastSeqIdPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> database);
   SeqIdFree(ptr -> id);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastSeqIdAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastSeqIdPtr LIBCALL
BlastSeqIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastSeqIdPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastSeqId ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_SEQ_ID);
   } else {
      atp = AsnLinkType(orig, BLAST_SEQ_ID);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastSeqIdNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_SEQ_ID_is_protein) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_protein = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_SEQ_ID_database) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> database = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_SEQ_ID_id) {
      ptr -> id = SeqIdAsnRead(aip, atp);
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
   ptr = BlastSeqIdFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastSeqIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastSeqIdAsnWrite(BlastSeqIdPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_SEQ_ID);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.boolvalue = ptr -> is_protein;
   retval = AsnWrite(aip, BLAST_SEQ_ID_is_protein,  &av);
   if (ptr -> database != NULL) {
      av.ptrvalue = ptr -> database;
      retval = AsnWrite(aip, BLAST_SEQ_ID_database,  &av);
   }
   if (ptr -> id != NULL) {
      if ( ! SeqIdAsnWrite(ptr -> id, aip, BLAST_SEQ_ID_id)) {
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
*    BlastSequenceNew()
*
**************************************************/
NLM_EXTERN 
BlastSequencePtr LIBCALL
BlastSequenceNew(void)
{
   BlastSequencePtr ptr = MemNew((size_t) sizeof(BlastSequence));

   return ptr;

}


/**************************************************
*
*    BlastSequenceFree()
*
**************************************************/
NLM_EXTERN 
BlastSequencePtr LIBCALL
BlastSequenceFree(BlastSequencePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SeqAlignFree(ptr -> align);
   BSFree(ptr -> db_seq);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastSequenceAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastSequencePtr LIBCALL
BlastSequenceAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastSequencePtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastSequence ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_SEQUENCE);
   } else {
      atp = AsnLinkType(orig, BLAST_SEQUENCE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastSequenceNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_SEQUENCE_align) {
      ptr -> align = SeqAlignAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_SEQUENCE_db_seq) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db_seq = av.ptrvalue;
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
   ptr = BlastSequenceFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastSequenceAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastSequenceAsnWrite(BlastSequencePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_SEQUENCE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> align != NULL) {
      if ( ! SeqAlignAsnWrite(ptr -> align, aip, BLAST_SEQUENCE_align)) {
         goto erret;
      }
   }
   if (ptr -> db_seq != NULL) {
      av.ptrvalue = ptr -> db_seq;
      retval = AsnWrite(aip, BLAST_SEQUENCE_db_seq,  &av);
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
*    BlastPartsNew()
*
**************************************************/
NLM_EXTERN 
BlastPartsPtr LIBCALL
BlastPartsNew(void)
{
   BlastPartsPtr ptr = MemNew((size_t) sizeof(BlastParts));

   return ptr;

}


/**************************************************
*
*    BlastPartsFree()
*
**************************************************/
NLM_EXTERN 
BlastPartsPtr LIBCALL
BlastPartsFree(BlastPartsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> defline);
   AsnGenericUserSeqOfFree(ptr -> sequence, (AsnOptFreeFunc) BlastSequenceFree);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastPartsAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastPartsPtr LIBCALL
BlastPartsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastPartsPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastParts ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_PARTS);
   } else {
      atp = AsnLinkType(orig, BLAST_PARTS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastPartsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_PARTS_defline) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> defline = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_PARTS_sequence) {
      ptr -> sequence = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BlastSequenceAsnRead, (AsnOptFreeFunc) BlastSequenceFree);
      if (isError && ptr -> sequence == NULL) {
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
   ptr = BlastPartsFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastPartsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastPartsAsnWrite(BlastPartsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_PARTS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> defline != NULL) {
      av.ptrvalue = ptr -> defline;
      retval = AsnWrite(aip, BLAST_PARTS_defline,  &av);
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> sequence, (AsnWriteFunc) BlastSequenceAsnWrite, aip, BLAST_PARTS_sequence, BLAST_PARTS_sequence_E);
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
*    BlastQueuedNew()
*
**************************************************/
NLM_EXTERN 
BlastQueuedPtr LIBCALL
BlastQueuedNew(void)
{
   BlastQueuedPtr ptr = MemNew((size_t) sizeof(BlastQueued));

   return ptr;

}


/**************************************************
*
*    BlastQueuedFree()
*
**************************************************/
NLM_EXTERN 
BlastQueuedPtr LIBCALL
BlastQueuedFree(BlastQueuedPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    BlastQueuedAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastQueuedPtr LIBCALL
BlastQueuedAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastQueuedPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastQueued ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_QUEUED);
   } else {
      atp = AsnLinkType(orig, BLAST_QUEUED);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastQueuedNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_QUEUED_length) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> length = av.intvalue;
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
   ptr = BlastQueuedFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastQueuedAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastQueuedAsnWrite(BlastQueuedPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_QUEUED);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> length;
   retval = AsnWrite(aip, BLAST_QUEUED_length,  &av);
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
*    BlastProgressNew()
*
**************************************************/
NLM_EXTERN 
BlastProgressPtr LIBCALL
BlastProgressNew(void)
{
   BlastProgressPtr ptr = MemNew((size_t) sizeof(BlastProgress));

   return ptr;

}


/**************************************************
*
*    BlastProgressFree()
*
**************************************************/
NLM_EXTERN 
BlastProgressPtr LIBCALL
BlastProgressFree(BlastProgressPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    BlastProgressAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastProgressPtr LIBCALL
BlastProgressAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastProgressPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastProgress ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_PROGRESS);
   } else {
      atp = AsnLinkType(orig, BLAST_PROGRESS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastProgressNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_PROGRESS_completed) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> completed = av.intvalue;
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
   ptr = BlastProgressFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastProgressAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastProgressAsnWrite(BlastProgressPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_PROGRESS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> completed;
   retval = AsnWrite(aip, BLAST_PROGRESS_completed,  &av);
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
*    BlastDeflineNew()
*
**************************************************/
NLM_EXTERN 
BlastDeflinePtr LIBCALL
BlastDeflineNew(void)
{
   BlastDeflinePtr ptr = MemNew((size_t) sizeof(BlastDefline));

   return ptr;

}


/**************************************************
*
*    BlastDeflineFree()
*
**************************************************/
NLM_EXTERN 
BlastDeflinePtr LIBCALL
BlastDeflineFree(BlastDeflinePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SeqIdFree(ptr -> id);
   MemFree(ptr -> defline);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastDeflineAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastDeflinePtr LIBCALL
BlastDeflineAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastDeflinePtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastDefline ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_DEFLINE);
   } else {
      atp = AsnLinkType(orig, BLAST_DEFLINE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastDeflineNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_DEFLINE_id) {
      ptr -> id = SeqIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DEFLINE_defline) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> defline = av.ptrvalue;
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
   ptr = BlastDeflineFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastDeflineAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastDeflineAsnWrite(BlastDeflinePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_DEFLINE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> id != NULL) {
      if ( ! SeqIdAsnWrite(ptr -> id, aip, BLAST_DEFLINE_id)) {
         goto erret;
      }
   }
   if (ptr -> defline != NULL) {
      av.ptrvalue = ptr -> defline;
      retval = AsnWrite(aip, BLAST_DEFLINE_defline,  &av);
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
*    BlastVersionNew()
*
**************************************************/
NLM_EXTERN 
BlastVersionPtr LIBCALL
BlastVersionNew(void)
{
   BlastVersionPtr ptr = MemNew((size_t) sizeof(BlastVersion));

   return ptr;

}


/**************************************************
*
*    BlastVersionFree()
*
**************************************************/
NLM_EXTERN 
BlastVersionPtr LIBCALL
BlastVersionFree(BlastVersionPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> version);
   MemFree(ptr -> date);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastVersionAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastVersionPtr LIBCALL
BlastVersionAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastVersionPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastVersion ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_VERSION);
   } else {
      atp = AsnLinkType(orig, BLAST_VERSION);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastVersionNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_VERSION_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> version = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_VERSION_date) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> date = av.ptrvalue;
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
   ptr = BlastVersionFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastVersionAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastVersionAsnWrite(BlastVersionPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_VERSION);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> version != NULL) {
      av.ptrvalue = ptr -> version;
      retval = AsnWrite(aip, BLAST_VERSION_version,  &av);
   }
   if (ptr -> date != NULL) {
      av.ptrvalue = ptr -> date;
      retval = AsnWrite(aip, BLAST_VERSION_date,  &av);
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
*    MegaBlastHitNew()
*
**************************************************/
NLM_EXTERN 
MegaBlastHitPtr LIBCALL
MegaBlastHitNew(void)
{
   MegaBlastHitPtr ptr = MemNew((size_t) sizeof(MegaBlastHit));

   return ptr;

}


/**************************************************
*
*    MegaBlastHitFree()
*
**************************************************/
NLM_EXTERN 
MegaBlastHitPtr LIBCALL
MegaBlastHitFree(MegaBlastHitPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> id1);
   MemFree(ptr -> id2);
   return MemFree(ptr);
}


/**************************************************
*
*    MegaBlastHitAsnRead()
*
**************************************************/
NLM_EXTERN 
MegaBlastHitPtr LIBCALL
MegaBlastHitAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MegaBlastHitPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MegaBlastHit ::= (self contained) */
      atp = AsnReadId(aip, amp, MEGABLAST_HIT);
   } else {
      atp = AsnLinkType(orig, MEGABLAST_HIT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MegaBlastHitNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MEGABLAST_HIT_id1) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id1 = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEGABLAST_HIT_id2) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id2 = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEGABLAST_HIT_query_offset) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> query_offset = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEGABLAST_HIT_subject_offset) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> subject_offset = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEGABLAST_HIT_query_end) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> query_end = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEGABLAST_HIT_subject_end) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> subject_end = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MEGABLAST_HIT_score) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> score = av.intvalue;
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
   ptr = MegaBlastHitFree(ptr);
   goto ret;
}



/**************************************************
*
*    MegaBlastHitAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MegaBlastHitAsnWrite(MegaBlastHitPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MEGABLAST_HIT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> id1 != NULL) {
      av.ptrvalue = ptr -> id1;
      retval = AsnWrite(aip, MEGABLAST_HIT_id1,  &av);
   }
   if (ptr -> id2 != NULL) {
      av.ptrvalue = ptr -> id2;
      retval = AsnWrite(aip, MEGABLAST_HIT_id2,  &av);
   }
   av.intvalue = ptr -> query_offset;
   retval = AsnWrite(aip, MEGABLAST_HIT_query_offset,  &av);
   av.intvalue = ptr -> subject_offset;
   retval = AsnWrite(aip, MEGABLAST_HIT_subject_offset,  &av);
   av.intvalue = ptr -> query_end;
   retval = AsnWrite(aip, MEGABLAST_HIT_query_end,  &av);
   av.intvalue = ptr -> subject_end;
   retval = AsnWrite(aip, MEGABLAST_HIT_subject_end,  &av);
   av.intvalue = ptr -> score;
   retval = AsnWrite(aip, MEGABLAST_HIT_score,  &av);
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
*    MegaBlastResultsNew()
*
**************************************************/
NLM_EXTERN 
MegaBlastResultsPtr LIBCALL
MegaBlastResultsNew(void)
{
   MegaBlastResultsPtr ptr = MemNew((size_t) sizeof(MegaBlastResults));

   return ptr;

}


/**************************************************
*
*    MegaBlastResultsFree()
*
**************************************************/
NLM_EXTERN 
MegaBlastResultsPtr LIBCALL
MegaBlastResultsFree(MegaBlastResultsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> mbhits, (AsnOptFreeFunc) MegaBlastHitFree);
   return MemFree(ptr);
}


/**************************************************
*
*    MegaBlastResultsAsnRead()
*
**************************************************/
NLM_EXTERN 
MegaBlastResultsPtr LIBCALL
MegaBlastResultsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MegaBlastResultsPtr ptr;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MegaBlastResults ::= (self contained) */
      atp = AsnReadId(aip, amp, MEGABLAST_RESULTS);
   } else {
      atp = AsnLinkType(orig, MEGABLAST_RESULTS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MegaBlastResultsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MEGABLAST_RESULTS_mbhits) {
      ptr -> mbhits = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MegaBlastHitAsnRead, (AsnOptFreeFunc) MegaBlastHitFree);
      if (isError && ptr -> mbhits == NULL) {
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
   ptr = MegaBlastResultsFree(ptr);
   goto ret;
}



/**************************************************
*
*    MegaBlastResultsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MegaBlastResultsAsnWrite(MegaBlastResultsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objblst3AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MEGABLAST_RESULTS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericUserSeqOfAsnWrite(ptr -> mbhits, (AsnWriteFunc) MegaBlastHitAsnWrite, aip, MEGABLAST_RESULTS_mbhits, MEGABLAST_RESULTS_mbhits_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

