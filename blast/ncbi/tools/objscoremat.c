#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <objgen.h>
#include <objsset.h>
#include <objscoremat.h>

static Boolean loaded = FALSE;

#include <scoremat.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objscorematAsnLoad(void)
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
*    Generated object loaders for Module NCBI-ScoreMat
*    Generated using ASNCODE Revision: 6.14 at Aug 26, 2003  5:18 PM
*
**************************************************/


/**************************************************
*
*    ScoreMatrixNew()
*
**************************************************/
NLM_EXTERN 
ScoreMatrixPtr LIBCALL
ScoreMatrixNew(void)
{
   ScoreMatrixPtr ptr = MemNew((size_t) sizeof(ScoreMatrix));

   return ptr;

}


/**************************************************
*
*    ScoreMatrixFree()
*
**************************************************/
NLM_EXTERN 
ScoreMatrixPtr LIBCALL
ScoreMatrixFree(ScoreMatrixPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ObjectIdFree(ptr -> identifier);
   AsnGenericBaseSeqOfFree(ptr -> comments ,ASNCODE_PTRVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> row_labels ,ASNCODE_PTRVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> scores ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> posFreqs ,ASNCODE_INTVAL_SLOT);
   SeqEntryFree(ptr -> query);
   AsnGenericBaseSeqOfFree(ptr -> rawFreqs ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> weights ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    ScoreMatrixAsnRead()
*
**************************************************/
NLM_EXTERN 
ScoreMatrixPtr LIBCALL
ScoreMatrixAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ScoreMatrixPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ScoreMatrix ::= (self contained) */
      atp = AsnReadId(aip, amp, SCORE_MATRIX);
   } else {
      atp = AsnLinkType(orig, SCORE_MATRIX);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ScoreMatrixNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == SCORE_MATRIX_is_protein) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> is_protein = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_identifier) {
      ptr -> identifier = ObjectIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_comments) {
      ptr -> comments = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> comments == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_nrows) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> nrows = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_ncolumns) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ncolumns = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_row_labels) {
      ptr -> row_labels = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> row_labels == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_scores) {
      ptr -> scores = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> scores == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_score_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> score_scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_posFreqs) {
      ptr -> posFreqs = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> posFreqs == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_posFreqs_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> posFreqs_scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_pseudocounts) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pseudocounts = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_freq_Ratios) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> freq_Ratios = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_gapOpen) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gapOpen = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_gapExtend) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gapExtend = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_query) {
      ptr -> query = SeqEntryAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_rawFreqs) {
      ptr -> rawFreqs = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> rawFreqs == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_rawFreqs_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rawFreqs_scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_weights) {
      ptr -> weights = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> weights == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_weights_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> weights_scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_byrow) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> byrow = av.boolvalue;
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
   ptr = ScoreMatrixFree(ptr);
   goto ret;
}



/**************************************************
*
*    ScoreMatrixAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ScoreMatrixAsnWrite(ScoreMatrixPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, SCORE_MATRIX);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.boolvalue = ptr -> is_protein;
   retval = AsnWrite(aip, SCORE_MATRIX_is_protein,  &av);
   if (ptr -> identifier != NULL) {
      if ( ! ObjectIdAsnWrite(ptr -> identifier, aip, SCORE_MATRIX_identifier)) {
         goto erret;
      }
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> comments ,ASNCODE_PTRVAL_SLOT, aip, SCORE_MATRIX_comments, SCORE_MATRIX_comments_E);
   av.intvalue = ptr -> nrows;
   retval = AsnWrite(aip, SCORE_MATRIX_nrows,  &av);
   av.intvalue = ptr -> ncolumns;
   retval = AsnWrite(aip, SCORE_MATRIX_ncolumns,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> row_labels ,ASNCODE_PTRVAL_SLOT, aip, SCORE_MATRIX_row_labels, SCORE_MATRIX_row_labels_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> scores ,ASNCODE_INTVAL_SLOT, aip, SCORE_MATRIX_scores, SCORE_MATRIX_scores_E);
   av.intvalue = ptr -> score_scale_factor;
   retval = AsnWrite(aip, SCORE_MATRIX_score_scale_factor,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> posFreqs ,ASNCODE_INTVAL_SLOT, aip, SCORE_MATRIX_posFreqs, SCORE_MATRIX_posFreqs_E);
   av.intvalue = ptr -> posFreqs_scale_factor;
   retval = AsnWrite(aip, SCORE_MATRIX_posFreqs_scale_factor,  &av);
   av.intvalue = ptr -> pseudocounts;
   retval = AsnWrite(aip, SCORE_MATRIX_pseudocounts,  &av);
   av.intvalue = ptr -> freq_Ratios;
   retval = AsnWrite(aip, SCORE_MATRIX_freq_Ratios,  &av);
   av.intvalue = ptr -> gapOpen;
   retval = AsnWrite(aip, SCORE_MATRIX_gapOpen,  &av);
   av.intvalue = ptr -> gapExtend;
   retval = AsnWrite(aip, SCORE_MATRIX_gapExtend,  &av);
   if (ptr -> query != NULL) {
      if ( ! SeqEntryAsnWrite(ptr -> query, aip, SCORE_MATRIX_query)) {
         goto erret;
      }
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> rawFreqs ,ASNCODE_INTVAL_SLOT, aip, SCORE_MATRIX_rawFreqs, SCORE_MATRIX_rawFreqs_E);
   av.intvalue = ptr -> rawFreqs_scale_factor;
   retval = AsnWrite(aip, SCORE_MATRIX_rawFreqs_scale_factor,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> weights ,ASNCODE_INTVAL_SLOT, aip, SCORE_MATRIX_weights, SCORE_MATRIX_weights_E);
   av.intvalue = ptr -> weights_scale_factor;
   retval = AsnWrite(aip, SCORE_MATRIX_weights_scale_factor,  &av);
   av.boolvalue = ptr -> byrow;
   retval = AsnWrite(aip, SCORE_MATRIX_byrow,  &av);
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
*    ScoreMatrixParametersNew()
*
**************************************************/
NLM_EXTERN 
ScoreMatrixParametersPtr LIBCALL
ScoreMatrixParametersNew(void)
{
   ScoreMatrixParametersPtr ptr = MemNew((size_t) sizeof(ScoreMatrixParameters));

   return ptr;

}


/**************************************************
*
*    ScoreMatrixParametersFree()
*
**************************************************/
NLM_EXTERN 
ScoreMatrixParametersPtr LIBCALL
ScoreMatrixParametersFree(ScoreMatrixParametersPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ScoreMatrixFree(ptr -> matrix);
   CoreDefFree(ptr -> constraints);
   return MemFree(ptr);
}


/**************************************************
*
*    ScoreMatrixParametersAsnRead()
*
**************************************************/
NLM_EXTERN 
ScoreMatrixParametersPtr LIBCALL
ScoreMatrixParametersAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ScoreMatrixParametersPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ScoreMatrixParameters ::= (self contained) */
      atp = AsnReadId(aip, amp, SCORE_MATRIX_PARAMETERS);
   } else {
      atp = AsnLinkType(orig, SCORE_MATRIX_PARAMETERS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ScoreMatrixParametersNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == SCORE_MATRIX_PARAMETERS_matrix) {
      ptr -> matrix = ScoreMatrixAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_PARAMETERS_lambda) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> lambda = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_PARAMETERS_kappa) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> kappa = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_PARAMETERS_h) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> h = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SCORE_MATRIX_PARAMETERS_constraints) {
      ptr -> constraints = CoreDefAsnRead(aip, atp);
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
   ptr = ScoreMatrixParametersFree(ptr);
   goto ret;
}



/**************************************************
*
*    ScoreMatrixParametersAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ScoreMatrixParametersAsnWrite(ScoreMatrixParametersPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, SCORE_MATRIX_PARAMETERS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> matrix != NULL) {
      if ( ! ScoreMatrixAsnWrite(ptr -> matrix, aip, SCORE_MATRIX_PARAMETERS_matrix)) {
         goto erret;
      }
   }
   av.realvalue = ptr -> lambda;
   retval = AsnWrite(aip, SCORE_MATRIX_PARAMETERS_lambda,  &av);
   av.realvalue = ptr -> kappa;
   retval = AsnWrite(aip, SCORE_MATRIX_PARAMETERS_kappa,  &av);
   av.realvalue = ptr -> h;
   retval = AsnWrite(aip, SCORE_MATRIX_PARAMETERS_h,  &av);
   if (ptr -> constraints != NULL) {
      if ( ! CoreDefAsnWrite(ptr -> constraints, aip, SCORE_MATRIX_PARAMETERS_constraints)) {
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
*    BlockPropertyNew()
*
**************************************************/
NLM_EXTERN 
BlockPropertyPtr LIBCALL
BlockPropertyNew(void)
{
   BlockPropertyPtr ptr = MemNew((size_t) sizeof(BlockProperty));

   return ptr;

}


/**************************************************
*
*    BlockPropertyFree()
*
**************************************************/
NLM_EXTERN 
BlockPropertyPtr LIBCALL
BlockPropertyFree(BlockPropertyPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> textvalue);
   return MemFree(ptr);
}


/**************************************************
*
*    BlockPropertyAsnRead()
*
**************************************************/
NLM_EXTERN 
BlockPropertyPtr LIBCALL
BlockPropertyAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlockPropertyPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlockProperty ::= (self contained) */
      atp = AsnReadId(aip, amp, BLOCKPROPERTY);
   } else {
      atp = AsnLinkType(orig, BLOCKPROPERTY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlockPropertyNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLOCKPROPERTY_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLOCKPROPERTY_intvalue) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> intvalue = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLOCKPROPERTY_textvalue) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> textvalue = av.ptrvalue;
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
   ptr = BlockPropertyFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlockPropertyAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlockPropertyAsnWrite(BlockPropertyPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLOCKPROPERTY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, BLOCKPROPERTY_type,  &av);
   av.intvalue = ptr -> intvalue;
   retval = AsnWrite(aip, BLOCKPROPERTY_intvalue,  &av);
   if (ptr -> textvalue != NULL) {
      av.ptrvalue = ptr -> textvalue;
      retval = AsnWrite(aip, BLOCKPROPERTY_textvalue,  &av);
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
*    CoreBlockNew()
*
**************************************************/
NLM_EXTERN 
CoreBlockPtr LIBCALL
CoreBlockNew(void)
{
   CoreBlockPtr ptr = MemNew((size_t) sizeof(CoreBlock));

   return ptr;

}


/**************************************************
*
*    CoreBlockFree()
*
**************************************************/
NLM_EXTERN 
CoreBlockPtr LIBCALL
CoreBlockFree(CoreBlockPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> property, (AsnOptFreeFunc) BlockPropertyFree);
   return MemFree(ptr);
}


/**************************************************
*
*    CoreBlockAsnRead()
*
**************************************************/
NLM_EXTERN 
CoreBlockPtr LIBCALL
CoreBlockAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CoreBlockPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CoreBlock ::= (self contained) */
      atp = AsnReadId(aip, amp, COREBLOCK);
   } else {
      atp = AsnLinkType(orig, COREBLOCK);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CoreBlockNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == COREBLOCK_start) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> start = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == COREBLOCK_stop) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> stop = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == COREBLOCK_minstart) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> minstart = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == COREBLOCK_maxstop) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> maxstop = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == COREBLOCK_property) {
      ptr -> property = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BlockPropertyAsnRead, (AsnOptFreeFunc) BlockPropertyFree);
      if (isError && ptr -> property == NULL) {
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
   ptr = CoreBlockFree(ptr);
   goto ret;
}



/**************************************************
*
*    CoreBlockAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CoreBlockAsnWrite(CoreBlockPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, COREBLOCK);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> start;
   retval = AsnWrite(aip, COREBLOCK_start,  &av);
   av.intvalue = ptr -> stop;
   retval = AsnWrite(aip, COREBLOCK_stop,  &av);
   av.intvalue = ptr -> minstart;
   retval = AsnWrite(aip, COREBLOCK_minstart,  &av);
   av.intvalue = ptr -> maxstop;
   retval = AsnWrite(aip, COREBLOCK_maxstop,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> property, (AsnWriteFunc) BlockPropertyAsnWrite, aip, COREBLOCK_property, COREBLOCK_property_E);
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
*    LoopConstraintNew()
*
**************************************************/
NLM_EXTERN 
LoopConstraintPtr LIBCALL
LoopConstraintNew(void)
{
   LoopConstraintPtr ptr = MemNew((size_t) sizeof(LoopConstraint));

   ptr -> minlength = 0;
   ptr -> maxlength = 100000;
   return ptr;

}


/**************************************************
*
*    LoopConstraintFree()
*
**************************************************/
NLM_EXTERN 
LoopConstraintPtr LIBCALL
LoopConstraintFree(LoopConstraintPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    LoopConstraintAsnRead()
*
**************************************************/
NLM_EXTERN 
LoopConstraintPtr LIBCALL
LoopConstraintAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   LoopConstraintPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* LoopConstraint ::= (self contained) */
      atp = AsnReadId(aip, amp, LOOPCONSTRAINT);
   } else {
      atp = AsnLinkType(orig, LOOPCONSTRAINT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = LoopConstraintNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == LOOPCONSTRAINT_minlength) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> minlength = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == LOOPCONSTRAINT_maxlength) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> maxlength = av.intvalue;
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
   ptr = LoopConstraintFree(ptr);
   goto ret;
}



/**************************************************
*
*    LoopConstraintAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
LoopConstraintAsnWrite(LoopConstraintPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, LOOPCONSTRAINT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> minlength;
   retval = AsnWrite(aip, LOOPCONSTRAINT_minlength,  &av);
   av.intvalue = ptr -> maxlength;
   retval = AsnWrite(aip, LOOPCONSTRAINT_maxlength,  &av);
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
*    CoreDefNew()
*
**************************************************/
NLM_EXTERN 
CoreDefPtr LIBCALL
CoreDefNew(void)
{
   CoreDefPtr ptr = MemNew((size_t) sizeof(CoreDef));

   return ptr;

}


/**************************************************
*
*    CoreDefFree()
*
**************************************************/
NLM_EXTERN 
CoreDefPtr LIBCALL
CoreDefFree(CoreDefPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> blocks, (AsnOptFreeFunc) CoreBlockFree);
   AsnGenericUserSeqOfFree(ptr -> loops, (AsnOptFreeFunc) LoopConstraintFree);
   return MemFree(ptr);
}


/**************************************************
*
*    CoreDefAsnRead()
*
**************************************************/
NLM_EXTERN 
CoreDefPtr LIBCALL
CoreDefAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CoreDefPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CoreDef ::= (self contained) */
      atp = AsnReadId(aip, amp, COREDEF);
   } else {
      atp = AsnLinkType(orig, COREDEF);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CoreDefNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == COREDEF_nblocks) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> nblocks = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == COREDEF_blocks) {
      ptr -> blocks = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) CoreBlockAsnRead, (AsnOptFreeFunc) CoreBlockFree);
      if (isError && ptr -> blocks == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == COREDEF_loops) {
      ptr -> loops = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) LoopConstraintAsnRead, (AsnOptFreeFunc) LoopConstraintFree);
      if (isError && ptr -> loops == NULL) {
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
   ptr = CoreDefFree(ptr);
   goto ret;
}



/**************************************************
*
*    CoreDefAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CoreDefAsnWrite(CoreDefPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, COREDEF);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> nblocks;
   retval = AsnWrite(aip, COREDEF_nblocks,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> blocks, (AsnWriteFunc) CoreBlockAsnWrite, aip, COREDEF_blocks, COREDEF_blocks_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> loops, (AsnWriteFunc) LoopConstraintAsnWrite, aip, COREDEF_loops, COREDEF_loops_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

