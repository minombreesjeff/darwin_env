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
*    Generated using ASNCODE Revision: 6.15 at Oct 7, 2004  2:31 PM
*
**************************************************/


/**************************************************
*
*    PssmNew()
*
**************************************************/
NLM_EXTERN 
PssmPtr LIBCALL
PssmNew(void)
{
   PssmPtr ptr = MemNew((size_t) sizeof(Pssm));

   ptr -> isProtein = 1;
   ptr -> byRow = 0;
   return ptr;

}


/**************************************************
*
*    PssmFree()
*
**************************************************/
NLM_EXTERN 
PssmPtr LIBCALL
PssmFree(PssmPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ObjectIdFree(ptr -> identifier);
   AsnGenericBaseSeqOfFree(ptr -> rowLabels ,ASNCODE_PTRVAL_SLOT);
   SeqEntryFree(ptr -> query);
   PssmIntermediateDataFree(ptr -> intermediateData);
   PssmFinalDataFree(ptr -> finalData);
   return MemFree(ptr);
}


/**************************************************
*
*    PssmAsnRead()
*
**************************************************/
NLM_EXTERN 
PssmPtr LIBCALL
PssmAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   PssmPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Pssm ::= (self contained) */
      atp = AsnReadId(aip, amp, PSSM);
   } else {
      atp = AsnLinkType(orig, PSSM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = PssmNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == PSSM_isProtein) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> isProtein = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSM_identifier) {
      ptr -> identifier = ObjectIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSM_numRows) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> numRows = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSM_numColumns) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> numColumns = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSM_rowLabels) {
      ptr -> rowLabels = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> rowLabels == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSM_byRow) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> byRow = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSM_query) {
      ptr -> query = SeqEntryAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSM_intermediateData) {
      ptr -> intermediateData = PssmIntermediateDataAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSM_finalData) {
      ptr -> finalData = PssmFinalDataAsnRead(aip, atp);
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
   ptr = PssmFree(ptr);
   goto ret;
}



/**************************************************
*
*    PssmAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
PssmAsnWrite(PssmPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, PSSM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.boolvalue = ptr -> isProtein;
   retval = AsnWrite(aip, PSSM_isProtein,  &av);
   if (ptr -> identifier != NULL) {
      if ( ! ObjectIdAsnWrite(ptr -> identifier, aip, PSSM_identifier)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> numRows;
   retval = AsnWrite(aip, PSSM_numRows,  &av);
   av.intvalue = ptr -> numColumns;
   retval = AsnWrite(aip, PSSM_numColumns,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> rowLabels ,ASNCODE_PTRVAL_SLOT, aip, PSSM_rowLabels, PSSM_rowLabels_E);
   av.boolvalue = ptr -> byRow;
   retval = AsnWrite(aip, PSSM_byRow,  &av);
   if (ptr -> query != NULL) {
      if ( ! SeqEntryAsnWrite(ptr -> query, aip, PSSM_query)) {
         goto erret;
      }
   }
   if (ptr -> intermediateData != NULL) {
      if ( ! PssmIntermediateDataAsnWrite(ptr -> intermediateData, aip, PSSM_intermediateData)) {
         goto erret;
      }
   }
   if (ptr -> finalData != NULL) {
      if ( ! PssmFinalDataAsnWrite(ptr -> finalData, aip, PSSM_finalData)) {
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
*    PssmIntermediateDataNew()
*
**************************************************/
NLM_EXTERN 
PssmIntermediateDataPtr LIBCALL
PssmIntermediateDataNew(void)
{
   PssmIntermediateDataPtr ptr = MemNew((size_t) sizeof(PssmIntermediateData));

   return ptr;

}


/**************************************************
*
*    PssmIntermediateDataFree()
*
**************************************************/
NLM_EXTERN 
PssmIntermediateDataPtr LIBCALL
PssmIntermediateDataFree(PssmIntermediateDataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> resFreqsPerPos ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> weightedResFreqsPerPos ,ASNCODE_REALVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> freqRatios ,ASNCODE_REALVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    PssmIntermediateDataAsnRead()
*
**************************************************/
NLM_EXTERN 
PssmIntermediateDataPtr LIBCALL
PssmIntermediateDataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   PssmIntermediateDataPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* PssmIntermediateData ::= (self contained) */
      atp = AsnReadId(aip, amp, PSSMINTERMEDIATEDATA);
   } else {
      atp = AsnLinkType(orig, PSSMINTERMEDIATEDATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = PssmIntermediateDataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == PSSMINTERMEDIATEDATA_resFreqsPerPos) {
      ptr -> resFreqsPerPos = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> resFreqsPerPos == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSMINTERMEDIATEDATA_weightedResFreqsPerPos) {
      ptr -> weightedResFreqsPerPos = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_REALVAL_SLOT, &isError);
      if (isError && ptr -> weightedResFreqsPerPos == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSMINTERMEDIATEDATA_freqRatios) {
      ptr -> freqRatios = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_REALVAL_SLOT, &isError);
      if (isError && ptr -> freqRatios == NULL) {
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
   ptr = PssmIntermediateDataFree(ptr);
   goto ret;
}



/**************************************************
*
*    PssmIntermediateDataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
PssmIntermediateDataAsnWrite(PssmIntermediateDataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, PSSMINTERMEDIATEDATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> resFreqsPerPos ,ASNCODE_INTVAL_SLOT, aip, PSSMINTERMEDIATEDATA_resFreqsPerPos, PSSMINTERMEDIATEDATA_resFreqsPerPos_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> weightedResFreqsPerPos ,ASNCODE_REALVAL_SLOT, aip, PSSMINTERMEDIATEDATA_weightedResFreqsPerPos, PSSMINTERMEDIATEDATA_weightedResFreqsPerPos_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> freqRatios ,ASNCODE_REALVAL_SLOT, aip, PSSMINTERMEDIATEDATA_freqRatios, PSSMINTERMEDIATEDATA_freqRatios_E);
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
*    PssmFinalDataNew()
*
**************************************************/
NLM_EXTERN 
PssmFinalDataPtr LIBCALL
PssmFinalDataNew(void)
{
   PssmFinalDataPtr ptr = MemNew((size_t) sizeof(PssmFinalData));

   ptr -> scalingFactor = 1;
   return ptr;

}


/**************************************************
*
*    PssmFinalDataFree()
*
**************************************************/
NLM_EXTERN 
PssmFinalDataPtr LIBCALL
PssmFinalDataFree(PssmFinalDataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> scores ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    PssmFinalDataAsnRead()
*
**************************************************/
NLM_EXTERN 
PssmFinalDataPtr LIBCALL
PssmFinalDataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   PssmFinalDataPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* PssmFinalData ::= (self contained) */
      atp = AsnReadId(aip, amp, PSSMFINALDATA);
   } else {
      atp = AsnLinkType(orig, PSSMFINALDATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = PssmFinalDataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == PSSMFINALDATA_scores) {
      ptr -> scores = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> scores == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSMFINALDATA_lambda) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> lambda = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSMFINALDATA_kappa) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> kappa = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSMFINALDATA_h) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> h = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSMFINALDATA_scalingFactor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scalingFactor = av.intvalue;
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
   ptr = PssmFinalDataFree(ptr);
   goto ret;
}



/**************************************************
*
*    PssmFinalDataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
PssmFinalDataAsnWrite(PssmFinalDataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, PSSMFINALDATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> scores ,ASNCODE_INTVAL_SLOT, aip, PSSMFINALDATA_scores, PSSMFINALDATA_scores_E);
   av.realvalue = ptr -> lambda;
   retval = AsnWrite(aip, PSSMFINALDATA_lambda,  &av);
   av.realvalue = ptr -> kappa;
   retval = AsnWrite(aip, PSSMFINALDATA_kappa,  &av);
   av.realvalue = ptr -> h;
   retval = AsnWrite(aip, PSSMFINALDATA_h,  &av);
   av.intvalue = ptr -> scalingFactor;
   retval = AsnWrite(aip, PSSMFINALDATA_scalingFactor,  &av);
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
*    PssmParametersNew()
*
**************************************************/
NLM_EXTERN 
PssmParametersPtr LIBCALL
PssmParametersNew(void)
{
   PssmParametersPtr ptr = MemNew((size_t) sizeof(PssmParameters));

   return ptr;

}


/**************************************************
*
*    PssmParametersFree()
*
**************************************************/
NLM_EXTERN 
PssmParametersPtr LIBCALL
PssmParametersFree(PssmParametersPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   FormatRpsDbParametersFree(ptr -> rpsdbparams);
   CoreDefFree(ptr -> constraints);
   return MemFree(ptr);
}


/**************************************************
*
*    PssmParametersAsnRead()
*
**************************************************/
NLM_EXTERN 
PssmParametersPtr LIBCALL
PssmParametersAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   PssmParametersPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* PssmParameters ::= (self contained) */
      atp = AsnReadId(aip, amp, PSSMPARAMETERS);
   } else {
      atp = AsnLinkType(orig, PSSMPARAMETERS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = PssmParametersNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == PSSMPARAMETERS_pseudocount) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pseudocount = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSMPARAMETERS_rpsdbparams) {
      ptr -> rpsdbparams = FormatRpsDbParametersAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSMPARAMETERS_constraints) {
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
   ptr = PssmParametersFree(ptr);
   goto ret;
}



/**************************************************
*
*    PssmParametersAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
PssmParametersAsnWrite(PssmParametersPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, PSSMPARAMETERS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> pseudocount;
   retval = AsnWrite(aip, PSSMPARAMETERS_pseudocount,  &av);
   if (ptr -> rpsdbparams != NULL) {
      if ( ! FormatRpsDbParametersAsnWrite(ptr -> rpsdbparams, aip, PSSMPARAMETERS_rpsdbparams)) {
         goto erret;
      }
   }
   if (ptr -> constraints != NULL) {
      if ( ! CoreDefAsnWrite(ptr -> constraints, aip, PSSMPARAMETERS_constraints)) {
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
*    PssmWithParametersNew()
*
**************************************************/
NLM_EXTERN 
PssmWithParametersPtr LIBCALL
PssmWithParametersNew(void)
{
   PssmWithParametersPtr ptr = MemNew((size_t) sizeof(PssmWithParameters));

   return ptr;

}


/**************************************************
*
*    PssmWithParametersFree()
*
**************************************************/
NLM_EXTERN 
PssmWithParametersPtr LIBCALL
PssmWithParametersFree(PssmWithParametersPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   PssmFree(ptr -> pssm);
   PssmParametersFree(ptr -> params);
   return MemFree(ptr);
}


/**************************************************
*
*    PssmWithParametersAsnRead()
*
**************************************************/
NLM_EXTERN 
PssmWithParametersPtr LIBCALL
PssmWithParametersAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   PssmWithParametersPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* PssmWithParameters ::= (self contained) */
      atp = AsnReadId(aip, amp, PSSMWITHPARAMETERS);
   } else {
      atp = AsnLinkType(orig, PSSMWITHPARAMETERS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = PssmWithParametersNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == PSSMWITHPARAMETERS_pssm) {
      ptr -> pssm = PssmAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PSSMWITHPARAMETERS_params) {
      ptr -> params = PssmParametersAsnRead(aip, atp);
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
   ptr = PssmWithParametersFree(ptr);
   goto ret;
}



/**************************************************
*
*    PssmWithParametersAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
PssmWithParametersAsnWrite(PssmWithParametersPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, PSSMWITHPARAMETERS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> pssm != NULL) {
      if ( ! PssmAsnWrite(ptr -> pssm, aip, PSSMWITHPARAMETERS_pssm)) {
         goto erret;
      }
   }
   if (ptr -> params != NULL) {
      if ( ! PssmParametersAsnWrite(ptr -> params, aip, PSSMWITHPARAMETERS_params)) {
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



/**************************************************
*
*    FormatRpsDbParametersNew()
*
**************************************************/
NLM_EXTERN 
FormatRpsDbParametersPtr LIBCALL
FormatRpsDbParametersNew(void)
{
   FormatRpsDbParametersPtr ptr = MemNew((size_t) sizeof(FormatRpsDbParameters));

   return ptr;

}


/**************************************************
*
*    FormatRpsDbParametersFree()
*
**************************************************/
NLM_EXTERN 
FormatRpsDbParametersPtr LIBCALL
FormatRpsDbParametersFree(FormatRpsDbParametersPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> matrixName);
   return MemFree(ptr);
}


/**************************************************
*
*    FormatRpsDbParametersAsnRead()
*
**************************************************/
NLM_EXTERN 
FormatRpsDbParametersPtr LIBCALL
FormatRpsDbParametersAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   FormatRpsDbParametersPtr ptr;

   if (! loaded)
   {
      if (! objscorematAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* FormatRpsDbParameters ::= (self contained) */
      atp = AsnReadId(aip, amp, FORMATRPSDBPARAMETERS);
   } else {
      atp = AsnLinkType(orig, FORMATRPSDBPARAMETERS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = FormatRpsDbParametersNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == FORMATRPSDBPARAMETERS_matrixName) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> matrixName = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FORMATRPSDBPARAMETERS_gapOpen) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gapOpen = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == FORMATRPSDBPARAMETERS_gapExtend) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gapExtend = av.intvalue;
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
   ptr = FormatRpsDbParametersFree(ptr);
   goto ret;
}



/**************************************************
*
*    FormatRpsDbParametersAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
FormatRpsDbParametersAsnWrite(FormatRpsDbParametersPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, FORMATRPSDBPARAMETERS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> matrixName != NULL) {
      av.ptrvalue = ptr -> matrixName;
      retval = AsnWrite(aip, FORMATRPSDBPARAMETERS_matrixName,  &av);
   }
   av.intvalue = ptr -> gapOpen;
   retval = AsnWrite(aip, FORMATRPSDBPARAMETERS_gapOpen,  &av);
   av.intvalue = ptr -> gapExtend;
   retval = AsnWrite(aip, FORMATRPSDBPARAMETERS_gapExtend,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

