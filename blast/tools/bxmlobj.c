#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <bxmlobj.h>

static Boolean loaded = FALSE;

#include <blstxml.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
bxmlobjAsnLoad(void)
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
*    Generated object loaders for Module NCBI-BlastOutput
*    Generated using ASNCODE Revision: 6.10 at May 3, 2001  1:46 PM
*
**************************************************/


/**************************************************
*
*    BlastOutputNew()
*
**************************************************/
NLM_EXTERN 
BlastOutputPtr LIBCALL
BlastOutputNew(void)
{
   BlastOutputPtr ptr = MemNew((size_t) sizeof(BlastOutput));

   return ptr;

}


/**************************************************
*
*    BlastOutputFree()
*
**************************************************/
NLM_EXTERN 
BlastOutputPtr LIBCALL
BlastOutputFree(BlastOutputPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> program);
   MemFree(ptr -> version);
   MemFree(ptr -> reference);
   MemFree(ptr -> db);
   MemFree(ptr -> query_ID);
   MemFree(ptr -> query_def);
   MemFree(ptr -> query_seq);
   ParametersFree(ptr -> param);
   AsnGenericUserSeqOfFree(ptr -> iterations, (AsnOptFreeFunc) IterationFree);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastOutputAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastOutputPtr LIBCALL
BlastOutputAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastOutputPtr ptr;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastOutput ::= (self contained) */
      atp = AsnReadId(aip, amp, BLASTOUTPUT);
   } else {
      atp = AsnLinkType(orig, BLASTOUTPUT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastOutputNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLASTOUTPUT_program) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> program = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLASTOUTPUT_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> version = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLASTOUTPUT_reference) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> reference = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLASTOUTPUT_db) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLASTOUTPUT_query_ID) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> query_ID = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLASTOUTPUT_query_def) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> query_def = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLASTOUTPUT_query_len) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> query_len = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLASTOUTPUT_query_seq) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> query_seq = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLASTOUTPUT_param) {
      ptr -> param = ParametersAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLASTOUTPUT_iterations) {
      ptr -> iterations = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) IterationAsnRead, (AsnOptFreeFunc) IterationFree);
      if (isError && ptr -> iterations == NULL) {
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
   ptr = BlastOutputFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastOutputAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastOutputAsnWrite(BlastOutputPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLASTOUTPUT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> program != NULL) {
      av.ptrvalue = ptr -> program;
      retval = AsnWrite(aip, BLASTOUTPUT_program,  &av);
   }
   if (ptr -> version != NULL) {
      av.ptrvalue = ptr -> version;
      retval = AsnWrite(aip, BLASTOUTPUT_version,  &av);
   }
   if (ptr -> reference != NULL) {
      av.ptrvalue = ptr -> reference;
      retval = AsnWrite(aip, BLASTOUTPUT_reference,  &av);
   }
   if (ptr -> db != NULL) {
      av.ptrvalue = ptr -> db;
      retval = AsnWrite(aip, BLASTOUTPUT_db,  &av);
   }
   if (ptr -> query_ID != NULL) {
      av.ptrvalue = ptr -> query_ID;
      retval = AsnWrite(aip, BLASTOUTPUT_query_ID,  &av);
   }
   if (ptr -> query_def != NULL) {
      av.ptrvalue = ptr -> query_def;
      retval = AsnWrite(aip, BLASTOUTPUT_query_def,  &av);
   }
   av.intvalue = ptr -> query_len;
   retval = AsnWrite(aip, BLASTOUTPUT_query_len,  &av);
   if (ptr -> query_seq != NULL) {
      av.ptrvalue = ptr -> query_seq;
      retval = AsnWrite(aip, BLASTOUTPUT_query_seq,  &av);
   }
   if (ptr -> param != NULL) {
      if ( ! ParametersAsnWrite(ptr -> param, aip, BLASTOUTPUT_param)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> iterations, (AsnWriteFunc) IterationAsnWrite, aip, BLASTOUTPUT_iterations, BLASTOUTPUT_iterations_E);
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
*    ParametersNew()
*
**************************************************/
NLM_EXTERN 
ParametersPtr LIBCALL
ParametersNew(void)
{
   ParametersPtr ptr = MemNew((size_t) sizeof(Parameters));

   return ptr;

}


/**************************************************
*
*    ParametersFree()
*
**************************************************/
NLM_EXTERN 
ParametersPtr LIBCALL
ParametersFree(ParametersPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> matrix);
   MemFree(ptr -> filter);
   MemFree(ptr -> pattern);
   MemFree(ptr -> entrez_query);
   return MemFree(ptr);
}


/**************************************************
*
*    ParametersAsnRead()
*
**************************************************/
NLM_EXTERN 
ParametersPtr LIBCALL
ParametersAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ParametersPtr ptr;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Parameters ::= (self contained) */
      atp = AsnReadId(aip, amp, PARAMETERS);
   } else {
      atp = AsnLinkType(orig, PARAMETERS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ParametersNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == PARAMETERS_matrix) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> matrix = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PARAMETERS_expect) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> expect = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PARAMETERS_include) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> include = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PARAMETERS_sc_match) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sc_match = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PARAMETERS_sc_mismatch) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sc_mismatch = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PARAMETERS_gap_open) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gap_open = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PARAMETERS_gap_extend) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gap_extend = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PARAMETERS_filter) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> filter = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PARAMETERS_pattern) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pattern = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PARAMETERS_entrez_query) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> entrez_query = av.ptrvalue;
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
   ptr = ParametersFree(ptr);
   goto ret;
}



/**************************************************
*
*    ParametersAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ParametersAsnWrite(ParametersPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, PARAMETERS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> matrix != NULL) {
      av.ptrvalue = ptr -> matrix;
      retval = AsnWrite(aip, PARAMETERS_matrix,  &av);
   }
   av.realvalue = ptr -> expect;
   retval = AsnWrite(aip, PARAMETERS_expect,  &av);
   av.realvalue = ptr -> include;
   retval = AsnWrite(aip, PARAMETERS_include,  &av);
   av.intvalue = ptr -> sc_match;
   retval = AsnWrite(aip, PARAMETERS_sc_match,  &av);
   av.intvalue = ptr -> sc_mismatch;
   retval = AsnWrite(aip, PARAMETERS_sc_mismatch,  &av);
   av.intvalue = ptr -> gap_open;
   retval = AsnWrite(aip, PARAMETERS_gap_open,  &av);
   av.intvalue = ptr -> gap_extend;
   retval = AsnWrite(aip, PARAMETERS_gap_extend,  &av);
   if (ptr -> filter != NULL) {
      av.ptrvalue = ptr -> filter;
      retval = AsnWrite(aip, PARAMETERS_filter,  &av);
   }
   if (ptr -> pattern != NULL) {
      av.ptrvalue = ptr -> pattern;
      retval = AsnWrite(aip, PARAMETERS_pattern,  &av);
   }
   if (ptr -> entrez_query != NULL) {
      av.ptrvalue = ptr -> entrez_query;
      retval = AsnWrite(aip, PARAMETERS_entrez_query,  &av);
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
*    IterationNew()
*
**************************************************/
NLM_EXTERN 
IterationPtr LIBCALL
IterationNew(void)
{
   IterationPtr ptr = MemNew((size_t) sizeof(Iteration));

   return ptr;

}


/**************************************************
*
*    IterationFree()
*
**************************************************/
NLM_EXTERN 
IterationPtr LIBCALL
IterationFree(IterationPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> hits, (AsnOptFreeFunc) HitFree);
   StatisticsFree(ptr -> stat);
   MemFree(ptr -> message);
   return MemFree(ptr);
}


/**************************************************
*
*    IterationAsnRead()
*
**************************************************/
NLM_EXTERN 
IterationPtr LIBCALL
IterationAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   IterationPtr ptr;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Iteration ::= (self contained) */
      atp = AsnReadId(aip, amp, ITERATION);
   } else {
      atp = AsnLinkType(orig, ITERATION);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = IterationNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ITERATION_iter_num) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> iter_num = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ITERATION_hits) {
      ptr -> hits = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) HitAsnRead, (AsnOptFreeFunc) HitFree);
      if (isError && ptr -> hits == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ITERATION_stat) {
      ptr -> stat = StatisticsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ITERATION_message) {
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
   ptr = IterationFree(ptr);
   goto ret;
}



/**************************************************
*
*    IterationAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
IterationAsnWrite(IterationPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ITERATION);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> iter_num;
   retval = AsnWrite(aip, ITERATION_iter_num,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> hits, (AsnWriteFunc) HitAsnWrite, aip, ITERATION_hits, ITERATION_hits_E);
   if (ptr -> stat != NULL) {
      if ( ! StatisticsAsnWrite(ptr -> stat, aip, ITERATION_stat)) {
         goto erret;
      }
   }
   if (ptr -> message != NULL) {
      av.ptrvalue = ptr -> message;
      retval = AsnWrite(aip, ITERATION_message,  &av);
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
*    HitNew()
*
**************************************************/
NLM_EXTERN 
HitPtr LIBCALL
HitNew(void)
{
   HitPtr ptr = MemNew((size_t) sizeof(Hit));

   return ptr;

}


/**************************************************
*
*    HitFree()
*
**************************************************/
NLM_EXTERN 
HitPtr LIBCALL
HitFree(HitPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> id);
   MemFree(ptr -> def);
   MemFree(ptr -> accession);
   AsnGenericUserSeqOfFree(ptr -> hsps, (AsnOptFreeFunc) HspFree);
   return MemFree(ptr);
}


/**************************************************
*
*    HitAsnRead()
*
**************************************************/
NLM_EXTERN 
HitPtr LIBCALL
HitAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   HitPtr ptr;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Hit ::= (self contained) */
      atp = AsnReadId(aip, amp, HIT);
   } else {
      atp = AsnLinkType(orig, HIT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = HitNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == HIT_num) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> num = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HIT_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HIT_def) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> def = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HIT_accession) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> accession = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HIT_len) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> len = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HIT_hsps) {
      ptr -> hsps = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) HspAsnRead, (AsnOptFreeFunc) HspFree);
      if (isError && ptr -> hsps == NULL) {
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
   ptr = HitFree(ptr);
   goto ret;
}



/**************************************************
*
*    HitAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
HitAsnWrite(HitPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, HIT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> num;
   retval = AsnWrite(aip, HIT_num,  &av);
   if (ptr -> id != NULL) {
      av.ptrvalue = ptr -> id;
      retval = AsnWrite(aip, HIT_id,  &av);
   }
   if (ptr -> def != NULL) {
      av.ptrvalue = ptr -> def;
      retval = AsnWrite(aip, HIT_def,  &av);
   }
   if (ptr -> accession != NULL) {
      av.ptrvalue = ptr -> accession;
      retval = AsnWrite(aip, HIT_accession,  &av);
   }
   av.intvalue = ptr -> len;
   retval = AsnWrite(aip, HIT_len,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> hsps, (AsnWriteFunc) HspAsnWrite, aip, HIT_hsps, HIT_hsps_E);
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
*    StatisticsNew()
*
**************************************************/
NLM_EXTERN 
StatisticsPtr LIBCALL
StatisticsNew(void)
{
   StatisticsPtr ptr = MemNew((size_t) sizeof(Statistics));

   return ptr;

}


/**************************************************
*
*    StatisticsFree()
*
**************************************************/
NLM_EXTERN 
StatisticsPtr LIBCALL
StatisticsFree(StatisticsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    StatisticsAsnRead()
*
**************************************************/
NLM_EXTERN 
StatisticsPtr LIBCALL
StatisticsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   StatisticsPtr ptr;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Statistics ::= (self contained) */
      atp = AsnReadId(aip, amp, STATISTICS);
   } else {
      atp = AsnLinkType(orig, STATISTICS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = StatisticsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == STATISTICS_db_num) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db_num = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STATISTICS_db_len) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> db_len = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STATISTICS_hsp_len) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hsp_len = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STATISTICS_eff_space) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> eff_space = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STATISTICS_kappa) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> kappa = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STATISTICS_lambda) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> lambda = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == STATISTICS_entropy) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> entropy = av.realvalue;
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
   ptr = StatisticsFree(ptr);
   goto ret;
}



/**************************************************
*
*    StatisticsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
StatisticsAsnWrite(StatisticsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, STATISTICS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> db_num;
   retval = AsnWrite(aip, STATISTICS_db_num,  &av);
   av.intvalue = ptr -> db_len;
   retval = AsnWrite(aip, STATISTICS_db_len,  &av);
   av.intvalue = ptr -> hsp_len;
   retval = AsnWrite(aip, STATISTICS_hsp_len,  &av);
   av.realvalue = ptr -> eff_space;
   retval = AsnWrite(aip, STATISTICS_eff_space,  &av);
   av.realvalue = ptr -> kappa;
   retval = AsnWrite(aip, STATISTICS_kappa,  &av);
   av.realvalue = ptr -> lambda;
   retval = AsnWrite(aip, STATISTICS_lambda,  &av);
   av.realvalue = ptr -> entropy;
   retval = AsnWrite(aip, STATISTICS_entropy,  &av);
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
*    HspNew()
*
**************************************************/
NLM_EXTERN 
HspPtr LIBCALL
HspNew(void)
{
   HspPtr ptr = MemNew((size_t) sizeof(Hsp));

   return ptr;

}


/**************************************************
*
*    HspFree()
*
**************************************************/
NLM_EXTERN 
HspPtr LIBCALL
HspFree(HspPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> qseq);
   MemFree(ptr -> hseq);
   MemFree(ptr -> midline);
   return MemFree(ptr);
}


/**************************************************
*
*    HspAsnRead()
*
**************************************************/
NLM_EXTERN 
HspPtr LIBCALL
HspAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   HspPtr ptr;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Hsp ::= (self contained) */
      atp = AsnReadId(aip, amp, HSP);
   } else {
      atp = AsnLinkType(orig, HSP);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = HspNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == HSP_num) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> num = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_bit_score) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> bit_score = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_score) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> score = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_evalue) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> evalue = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_query_from) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> query_from = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_query_to) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> query_to = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_hit_from) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hit_from = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_hit_to) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hit_to = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_pattern_from) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pattern_from = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_pattern_to) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pattern_to = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_query_frame) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> query_frame = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_hit_frame) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hit_frame = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_identity) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> identity = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_positive) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> positive = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_gaps) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gaps = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_align_len) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> align_len = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_density) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> density = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_qseq) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> qseq = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_hseq) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hseq = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == HSP_midline) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> midline = av.ptrvalue;
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
   ptr = HspFree(ptr);
   goto ret;
}



/**************************************************
*
*    HspAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
HspAsnWrite(HspPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! bxmlobjAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, HSP);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> num;
   retval = AsnWrite(aip, HSP_num,  &av);
   av.realvalue = ptr -> bit_score;
   retval = AsnWrite(aip, HSP_bit_score,  &av);
   av.realvalue = ptr -> score;
   retval = AsnWrite(aip, HSP_score,  &av);
   av.realvalue = ptr -> evalue;
   retval = AsnWrite(aip, HSP_evalue,  &av);
   av.intvalue = ptr -> query_from;
   retval = AsnWrite(aip, HSP_query_from,  &av);
   av.intvalue = ptr -> query_to;
   retval = AsnWrite(aip, HSP_query_to,  &av);
   av.intvalue = ptr -> hit_from;
   retval = AsnWrite(aip, HSP_hit_from,  &av);
   av.intvalue = ptr -> hit_to;
   retval = AsnWrite(aip, HSP_hit_to,  &av);
   av.intvalue = ptr -> pattern_from;
   retval = AsnWrite(aip, HSP_pattern_from,  &av);
   av.intvalue = ptr -> pattern_to;
   retval = AsnWrite(aip, HSP_pattern_to,  &av);
   av.intvalue = ptr -> query_frame;
   retval = AsnWrite(aip, HSP_query_frame,  &av);
   av.intvalue = ptr -> hit_frame;
   retval = AsnWrite(aip, HSP_hit_frame,  &av);
   av.intvalue = ptr -> identity;
   retval = AsnWrite(aip, HSP_identity,  &av);
   av.intvalue = ptr -> positive;
   retval = AsnWrite(aip, HSP_positive,  &av);
   av.intvalue = ptr -> gaps;
   retval = AsnWrite(aip, HSP_gaps,  &av);
   av.intvalue = ptr -> align_len;
   retval = AsnWrite(aip, HSP_align_len,  &av);
   av.intvalue = ptr -> density;
   retval = AsnWrite(aip, HSP_density,  &av);
   if (ptr -> qseq != NULL) {
      av.ptrvalue = ptr -> qseq;
      retval = AsnWrite(aip, HSP_qseq,  &av);
   }
   if (ptr -> hseq != NULL) {
      av.ptrvalue = ptr -> hseq;
      retval = AsnWrite(aip, HSP_hseq,  &av);
   }
   if (ptr -> midline != NULL) {
      av.ptrvalue = ptr -> midline;
      retval = AsnWrite(aip, HSP_midline,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

