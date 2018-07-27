#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <id2map.h>
#include <id2sgen.h>

static Boolean loaded = FALSE;

#include <seqsplit.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
id2sgenAsnLoad(void)
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
*
**************************************************/


/**************************************************
*
*    ID2SSeqAnnotInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SSeqAnnotInfoPtr LIBCALL
ID2SSeqAnnotInfoNew(void)
{
   ID2SSeqAnnotInfoPtr ptr = MemNew((size_t) sizeof(ID2SSeqAnnotInfo));

   return ptr;

}


/**************************************************
*
*    ID2SSeqAnnotInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SSeqAnnotInfoPtr LIBCALL
ID2SSeqAnnotInfoFree(ID2SSeqAnnotInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   AsnGenericUserSeqOfFree(ptr -> feat, (AsnOptFreeFunc) ID2SFeatTypeInfoFree);
   ID2SSeqLocFree(ptr -> seq_loc);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SSeqAnnotInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SSeqAnnotInfoPtr LIBCALL
ID2SSeqAnnotInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SSeqAnnotInfoPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SSeqAnnotInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_SEQ_ANNOT_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_SEQ_ANNOT_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SSeqAnnotInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_SEQ_ANNOT_INFO_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SEQ_ANNOT_INFO_align) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> align = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SEQ_ANNOT_INFO_graph) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> graph = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SEQ_ANNOT_INFO_feat) {
      ptr -> feat = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SFeatTypeInfoAsnRead, (AsnOptFreeFunc) ID2SFeatTypeInfoFree);
      if (isError && ptr -> feat == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SEQ_ANNOT_INFO_seq_loc) {
      ptr -> seq_loc = ID2SSeqLocAsnRead(aip, atp);
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
   ptr = ID2SSeqAnnotInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SSeqAnnotInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SSeqAnnotInfoAsnWrite(ID2SSeqAnnotInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_SEQ_ANNOT_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, ID2S_SEQ_ANNOT_INFO_name,  &av);
   }
   av.boolvalue = ptr -> align;
   retval = AsnWrite(aip, ID2S_SEQ_ANNOT_INFO_align,  &av);
   av.boolvalue = ptr -> graph;
   retval = AsnWrite(aip, ID2S_SEQ_ANNOT_INFO_graph,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> feat, (AsnWriteFunc) ID2SFeatTypeInfoAsnWrite, aip, ID2S_SEQ_ANNOT_INFO_feat, ID2S_SEQ_ANNOT_INFO_feat_E);
   if (ptr -> seq_loc != NULL) {
      if ( ! ID2SSeqLocAsnWrite(ptr -> seq_loc, aip, ID2S_SEQ_ANNOT_INFO_seq_loc)) {
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
*    ID2SSplitInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SSplitInfoPtr LIBCALL
ID2SSplitInfoNew(void)
{
   ID2SSplitInfoPtr ptr = MemNew((size_t) sizeof(ID2SSplitInfo));

   return ptr;

}


/**************************************************
*
*    ID2SSplitInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SSplitInfoPtr LIBCALL
ID2SSplitInfoFree(ID2SSplitInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> bioseqs_info, (AsnOptFreeFunc) ID2SBioseqsInfoFree);
   AsnGenericUserSeqOfFree(ptr -> chunks, (AsnOptFreeFunc) ID2SChunkInfoFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SSplitInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SSplitInfoPtr LIBCALL
ID2SSplitInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SSplitInfoPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SSplitInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_SPLIT_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_SPLIT_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SSplitInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_SPLIT_INFO_bioseqs_info) {
      ptr -> bioseqs_info = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SBioseqsInfoAsnRead, (AsnOptFreeFunc) ID2SBioseqsInfoFree);
      if (isError && ptr -> bioseqs_info == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SPLIT_INFO_chunks) {
      ptr -> chunks = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SChunkInfoAsnRead, (AsnOptFreeFunc) ID2SChunkInfoFree);
      if (isError && ptr -> chunks == NULL) {
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
   ptr = ID2SSplitInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SSplitInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SSplitInfoAsnWrite(ID2SSplitInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_SPLIT_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericUserSeqOfAsnWrite(ptr -> bioseqs_info, (AsnWriteFunc) ID2SBioseqsInfoAsnWrite, aip, ID2S_SPLIT_INFO_bioseqs_info, ID2S_SPLIT_INFO_bioseqs_info_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> chunks, (AsnWriteFunc) ID2SChunkInfoAsnWrite, aip, ID2S_SPLIT_INFO_chunks, ID2S_SPLIT_INFO_chunks_E);
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
*    ID2SBioseqsInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqsInfoPtr LIBCALL
ID2SBioseqsInfoNew(void)
{
   ID2SBioseqsInfoPtr ptr = MemNew((size_t) sizeof(ID2SBioseqsInfo));

   return ptr;

}


/**************************************************
*
*    ID2SBioseqsInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqsInfoPtr LIBCALL
ID2SBioseqsInfoFree(ID2SBioseqsInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2SBioseqInfoFree(ptr -> info);
   ID2SBioseqIdsFree(ptr -> bioseqs);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SBioseqsInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqsInfoPtr LIBCALL
ID2SBioseqsInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SBioseqsInfoPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SBioseqsInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_BIOSEQS_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_BIOSEQS_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SBioseqsInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_BIOSEQS_INFO_info) {
      ptr -> info = ID2SBioseqInfoAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_BIOSEQS_INFO_bioseqs) {
      ptr -> bioseqs = ID2SBioseqIdsAsnRead(aip, atp);
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
   ptr = ID2SBioseqsInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SBioseqsInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SBioseqsInfoAsnWrite(ID2SBioseqsInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_BIOSEQS_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> info != NULL) {
      if ( ! ID2SBioseqInfoAsnWrite(ptr -> info, aip, ID2S_BIOSEQS_INFO_info)) {
         goto erret;
      }
   }
   if (ptr -> bioseqs != NULL) {
      if ( ! ID2SBioseqIdsAsnWrite(ptr -> bioseqs, aip, ID2S_BIOSEQS_INFO_bioseqs)) {
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
*    ID2SChunkInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SChunkInfoPtr LIBCALL
ID2SChunkInfoNew(void)
{
   ID2SChunkInfoPtr ptr = MemNew((size_t) sizeof(ID2SChunkInfo));

   return ptr;

}


/**************************************************
*
*    ID2SChunkInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SChunkInfoPtr LIBCALL
ID2SChunkInfoFree(ID2SChunkInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> content, (AsnOptFreeFunc) ID2SChunkContentFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SChunkInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SChunkInfoPtr LIBCALL
ID2SChunkInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SChunkInfoPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SChunkInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_CHUNK_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_CHUNK_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SChunkInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_CHUNK_INFO_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_CHUNK_INFO_content) {
      ptr -> content = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SChunkContentAsnRead, (AsnOptFreeFunc) ID2SChunkContentFree);
      if (isError && ptr -> content == NULL) {
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
   ptr = ID2SChunkInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SChunkInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SChunkInfoAsnWrite(ID2SChunkInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_CHUNK_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> id;
   retval = AsnWrite(aip, ID2S_CHUNK_INFO_id,  &av);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> content, (AsnWriteFunc) ID2SChunkContentAsnWrite, aip, ID2S_CHUNK_INFO_content, ID2S_CHUNK_INFO_content_E);
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
*    ID2SBioseqInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqInfoPtr LIBCALL
ID2SBioseqInfoNew(void)
{
   ID2SBioseqInfoPtr ptr = MemNew((size_t) sizeof(ID2SBioseqInfo));

   return ptr;

}


/**************************************************
*
*    ID2SBioseqInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqInfoPtr LIBCALL
ID2SBioseqInfoFree(ID2SBioseqInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SBioseqInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqInfoPtr LIBCALL
ID2SBioseqInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SBioseqInfoPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SBioseqInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_BIOSEQ_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_BIOSEQ_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SBioseqInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_BIOSEQ_INFO_gap_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gap_count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSEQ_INFO_seq_map_has_ref) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> seq_map_has_ref = av.boolvalue;
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
   ptr = ID2SBioseqInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SBioseqInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SBioseqInfoAsnWrite(ID2SBioseqInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_BIOSEQ_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> gap_count;
   retval = AsnWrite(aip, ID2S_BIOSEQ_INFO_gap_count,  &av);
   av.boolvalue = ptr -> seq_map_has_ref;
   retval = AsnWrite(aip, BIOSEQ_INFO_seq_map_has_ref,  &av);
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
*    ID2SBioseqIdsFree()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqIdsPtr LIBCALL
ID2SBioseqIdsFree(ValNodePtr anp)
{

   if (anp == NULL) {
      return NULL;
   }

   AsnGenericChoiceSeqOfFree(anp, (AsnOptFreeFunc) ID2SBioseqIds_elementFree);    
   return NULL;
}


/**************************************************
*
*    ID2SBioseqIdsAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqIdsPtr LIBCALL
ID2SBioseqIdsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{


   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError = FALSE;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SBioseqIds_element ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_BIOSEQ_IDS);
   } else {
      atp = AsnLinkType(orig, ID2S_BIOSEQ_IDS);    /* link in local tree */
   }
   if (atp == NULL) {
      return NULL;
   }

   anp =
   AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError,
   (AsnReadFunc) ID2SBioseqIds_elementAsnRead, (AsnOptFreeFunc) ID2SBioseqIds_elementFree);
   if (isError) 
   goto erret;


ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   aip -> io_failure = TRUE;
   goto ret;
}


/**************************************************
*
*    ID2SBioseqIdsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SBioseqIdsAsnWrite(ValNodePtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ID2S_BIOSEQ_IDS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   retval = AsnGenericChoiceSeqOfAsnWrite(anp, 
   (AsnWriteFunc) ID2SBioseqIds_elementAsnWrite, aip, atp, ID2S_BIOSEQ_IDS_E);
erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}


/**************************************************
*
*    ID2SBioseqIds_elementAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SBioseqIds_elementAsnWrite(ID2SBioseqIds_elementPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ID2S_BIOSEQ_IDS_E);   /* link local tree */
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
   case ID2SBioseqIds_gi:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ID2S_BIOSEQ_IDS_E_gi, &av);
      break;
   case ID2SBioseqIds_seq_id:
      writetype = ID2S_BIOSEQ_IDS_E_seq_id;
      func = (AsnWriteFunc) SeqIdAsnWrite;
      break;
   case ID2SBioseqIds_gi_range:
      writetype = ID2S_BIOSEQ_IDS_E_gi_range;
      func = (AsnWriteFunc) ID2SGiRangeAsnWrite;
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
*    ID2SBioseqIds_elementAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqIds_elementPtr LIBCALL
ID2SBioseqIds_elementAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SBioseqIds_element ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_BIOSEQ_IDS_E);
   } else {
      atp = AsnLinkType(orig, ID2S_BIOSEQ_IDS_E);    /* link in local tree */
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
   if (atp == ID2S_BIOSEQ_IDS_E_gi) {
      choice = ID2SBioseqIds_gi;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ID2S_BIOSEQ_IDS_E_seq_id) {
      choice = ID2SBioseqIds_seq_id;
      func = (AsnReadFunc) SeqIdAsnRead;
   }
   else if (atp == ID2S_BIOSEQ_IDS_E_gi_range) {
      choice = ID2SBioseqIds_gi_range;
      func = (AsnReadFunc) ID2SGiRangeAsnRead;
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
*    ID2SBioseqIds_elementFree()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqIds_elementPtr LIBCALL
ID2SBioseqIds_elementFree(ValNodePtr anp)
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
   case ID2SBioseqIds_seq_id:
      SeqIdFree(anp -> data.ptrvalue);
      break;
   case ID2SBioseqIds_gi_range:
      ID2SGiRangeFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ID2SChunkContentFree()
*
**************************************************/
NLM_EXTERN 
ID2SChunkContentPtr LIBCALL
ID2SChunkContentFree(ValNodePtr anp)
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
   case ID2SChunkContent_seq_descr:
      ID2SSeqDescrInfoFree(anp -> data.ptrvalue);
      break;
   case ID2SChunkContent_seq_annot:
      ID2SSeqAnnotInfoFree(anp -> data.ptrvalue);
      break;
   case ID2SChunkContent_seq_assembly:
      ID2SSeqAssemblyInfoFree(anp -> data.ptrvalue);
      break;
   case ID2SChunkContent_seq_map:
      ID2SSeqLocFree(anp -> data.ptrvalue);
      break;
   case ID2SChunkContent_seq_data:
      ID2SSeqLocFree(anp -> data.ptrvalue);
      break;
   case ID2SChunkContent_seq_annot_place:
      ID2SSeqAnnotPlaceInfoFree(anp -> data.ptrvalue);
      break;
   case ID2SChunkContent_bioseq_place:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) ID2SBioseqPlaceInfoFree);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ID2SChunkContentAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SChunkContentPtr LIBCALL
ID2SChunkContentAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SChunkContent ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_CHUNK_CONTENT);
   } else {
      atp = AsnLinkType(orig, ID2S_CHUNK_CONTENT);    /* link in local tree */
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
   if (atp == ID2S_CHUNK_CONTENT_seq_descr) {
      choice = ID2SChunkContent_seq_descr;
      func = (AsnReadFunc) ID2SSeqDescrInfoAsnRead;
   }
   else if (atp == ID2S_CHUNK_CONTENT_seq_annot) {
      choice = ID2SChunkContent_seq_annot;
      func = (AsnReadFunc) ID2SSeqAnnotInfoAsnRead;
   }
   else if (atp == ID2S_CHUNK_CONTENT_seq_assembly) {
      choice = ID2SChunkContent_seq_assembly;
      func = (AsnReadFunc) ID2SSeqAssemblyInfoAsnRead;
   }
   else if (atp == ID2S_CHUNK_CONTENT_seq_map) {
      choice = ID2SChunkContent_seq_map;
      func = (AsnReadFunc) ID2SSeqLocAsnRead;
   }
   else if (atp == ID2S_CHUNK_CONTENT_seq_data) {
      choice = ID2SChunkContent_seq_data;
      func = (AsnReadFunc) ID2SSeqLocAsnRead;
   }
   else if (atp == CHUNK_CONTENT_seq_annot_place) {
      choice = ID2SChunkContent_seq_annot_place;
      func = (AsnReadFunc) ID2SSeqAnnotPlaceInfoAsnRead;
   }
   else if (atp == ID2S_CHUNK_CONTENT_bioseq_place) {
      choice = ID2SChunkContent_bioseq_place;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SBioseqPlaceInfoAsnRead,             (AsnOptFreeFunc) ID2SBioseqPlaceInfoFree);
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
*    ID2SChunkContentAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SChunkContentAsnWrite(ID2SChunkContentPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ID2S_CHUNK_CONTENT);   /* link local tree */
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
   case ID2SChunkContent_seq_descr:
      writetype = ID2S_CHUNK_CONTENT_seq_descr;
      func = (AsnWriteFunc) ID2SSeqDescrInfoAsnWrite;
      break;
   case ID2SChunkContent_seq_annot:
      writetype = ID2S_CHUNK_CONTENT_seq_annot;
      func = (AsnWriteFunc) ID2SSeqAnnotInfoAsnWrite;
      break;
   case ID2SChunkContent_seq_assembly:
      writetype = ID2S_CHUNK_CONTENT_seq_assembly;
      func = (AsnWriteFunc) ID2SSeqAssemblyInfoAsnWrite;
      break;
   case ID2SChunkContent_seq_map:
      writetype = ID2S_CHUNK_CONTENT_seq_map;
      func = (AsnWriteFunc) ID2SSeqLocAsnWrite;
      break;
   case ID2SChunkContent_seq_data:
      writetype = ID2S_CHUNK_CONTENT_seq_data;
      func = (AsnWriteFunc) ID2SSeqLocAsnWrite;
      break;
   case ID2SChunkContent_seq_annot_place:
      writetype = CHUNK_CONTENT_seq_annot_place;
      func = (AsnWriteFunc) ID2SSeqAnnotPlaceInfoAsnWrite;
      break;
   case ID2SChunkContent_bioseq_place:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) ID2SBioseqPlaceInfoAsnWrite, aip, ID2S_CHUNK_CONTENT_bioseq_place, CHUNK_CONTENT_bioseq_place_E);
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
*    ID2SSeqDescrInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SSeqDescrInfoPtr LIBCALL
ID2SSeqDescrInfoNew(void)
{
   ID2SSeqDescrInfoPtr ptr = MemNew((size_t) sizeof(ID2SSeqDescrInfo));

   return ptr;

}


/**************************************************
*
*    ID2SSeqDescrInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SSeqDescrInfoPtr LIBCALL
ID2SSeqDescrInfoFree(ID2SSeqDescrInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2SBioseqIdsFree(ptr -> bioseqs);
   ID2SBioseqSetIdsFree(ptr -> bioseq_sets);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SSeqDescrInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SSeqDescrInfoPtr LIBCALL
ID2SSeqDescrInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SSeqDescrInfoPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SSeqDescrInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_SEQ_DESCR_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_SEQ_DESCR_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SSeqDescrInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_SEQ_DESCR_INFO_type_mask) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type_mask = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SEQ_DESCR_INFO_bioseqs) {
      ptr -> bioseqs = ID2SBioseqIdsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SEQ_DESCR_INFO_bioseq_sets) {
      ptr -> bioseq_sets = ID2SBioseqSetIdsAsnRead(aip, atp);
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
   ptr = ID2SSeqDescrInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SSeqDescrInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SSeqDescrInfoAsnWrite(ID2SSeqDescrInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_SEQ_DESCR_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> type_mask;
   retval = AsnWrite(aip, ID2S_SEQ_DESCR_INFO_type_mask,  &av);
   if (ptr -> bioseqs != NULL) {
      if ( ! ID2SBioseqIdsAsnWrite(ptr -> bioseqs, aip, ID2S_SEQ_DESCR_INFO_bioseqs)) {
         goto erret;
      }
   }
   if (ptr -> bioseq_sets != NULL) {
      if ( ! ID2SBioseqSetIdsAsnWrite(ptr -> bioseq_sets, aip, ID2S_SEQ_DESCR_INFO_bioseq_sets)) {
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
*    ID2SSeqAssemblyInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SSeqAssemblyInfoPtr LIBCALL
ID2SSeqAssemblyInfoNew(void)
{
   ID2SSeqAssemblyInfoPtr ptr = MemNew((size_t) sizeof(ID2SSeqAssemblyInfo));

   return ptr;

}


/**************************************************
*
*    ID2SSeqAssemblyInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SSeqAssemblyInfoPtr LIBCALL
ID2SSeqAssemblyInfoFree(ID2SSeqAssemblyInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2SBioseqIdsFree(ptr -> bioseqs);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SSeqAssemblyInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SSeqAssemblyInfoPtr LIBCALL
ID2SSeqAssemblyInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SSeqAssemblyInfoPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SSeqAssemblyInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_SEQ_ASSEMBLY_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_SEQ_ASSEMBLY_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SSeqAssemblyInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_SEQ_ASSEMBLY_INFO_bioseqs) {
      ptr -> bioseqs = ID2SBioseqIdsAsnRead(aip, atp);
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
   ptr = ID2SSeqAssemblyInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SSeqAssemblyInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SSeqAssemblyInfoAsnWrite(ID2SSeqAssemblyInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_SEQ_ASSEMBLY_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> bioseqs != NULL) {
      if ( ! ID2SBioseqIdsAsnWrite(ptr -> bioseqs, aip, ID2S_SEQ_ASSEMBLY_INFO_bioseqs)) {
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
*    ID2SSeqAnnotPlaceInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SSeqAnnotPlaceInfoPtr LIBCALL
ID2SSeqAnnotPlaceInfoNew(void)
{
   ID2SSeqAnnotPlaceInfoPtr ptr = MemNew((size_t) sizeof(ID2SSeqAnnotPlaceInfo));

   return ptr;

}


/**************************************************
*
*    ID2SSeqAnnotPlaceInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SSeqAnnotPlaceInfoPtr LIBCALL
ID2SSeqAnnotPlaceInfoFree(ID2SSeqAnnotPlaceInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   ID2SBioseqIdsFree(ptr -> bioseqs);
   ID2SBioseqSetIdsFree(ptr -> bioseq_sets);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SSeqAnnotPlaceInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SSeqAnnotPlaceInfoPtr LIBCALL
ID2SSeqAnnotPlaceInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SSeqAnnotPlaceInfoPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SSeqAnnotPlaceInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_SEQ_ANNOT_PLACE_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_SEQ_ANNOT_PLACE_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SSeqAnnotPlaceInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_SEQ_ANNOT_PLACE_INFO_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SEQ_ANNOT_PLACE_INFO_bioseqs) {
      ptr -> bioseqs = ID2SBioseqIdsAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ANNOT_PLACE_INFO_bioseq_sets) {
      ptr -> bioseq_sets = ID2SBioseqSetIdsAsnRead(aip, atp);
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
   ptr = ID2SSeqAnnotPlaceInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SSeqAnnotPlaceInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SSeqAnnotPlaceInfoAsnWrite(ID2SSeqAnnotPlaceInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_SEQ_ANNOT_PLACE_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, ID2S_SEQ_ANNOT_PLACE_INFO_name,  &av);
   }
   if (ptr -> bioseqs != NULL) {
      if ( ! ID2SBioseqIdsAsnWrite(ptr -> bioseqs, aip, SEQ_ANNOT_PLACE_INFO_bioseqs)) {
         goto erret;
      }
   }
   if (ptr -> bioseq_sets != NULL) {
      if ( ! ID2SBioseqSetIdsAsnWrite(ptr -> bioseq_sets, aip, ANNOT_PLACE_INFO_bioseq_sets)) {
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
*    ID2SBioseqPlaceInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqPlaceInfoPtr LIBCALL
ID2SBioseqPlaceInfoNew(void)
{
   ID2SBioseqPlaceInfoPtr ptr = MemNew((size_t) sizeof(ID2SBioseqPlaceInfo));

   return ptr;

}


/**************************************************
*
*    ID2SBioseqPlaceInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqPlaceInfoPtr LIBCALL
ID2SBioseqPlaceInfoFree(ID2SBioseqPlaceInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ID2SBioseqIdsFree(ptr -> seq_ids);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SBioseqPlaceInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqPlaceInfoPtr LIBCALL
ID2SBioseqPlaceInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SBioseqPlaceInfoPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SBioseqPlaceInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_BIOSEQ_PLACE_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_BIOSEQ_PLACE_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SBioseqPlaceInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSEQ_PLACE_INFO_bioseq_set) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> bioseq_set = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_BIOSEQ_PLACE_INFO_seq_ids) {
      ptr -> seq_ids = ID2SBioseqIdsAsnRead(aip, atp);
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
   ptr = ID2SBioseqPlaceInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SBioseqPlaceInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SBioseqPlaceInfoAsnWrite(ID2SBioseqPlaceInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_BIOSEQ_PLACE_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> bioseq_set;
   retval = AsnWrite(aip, BIOSEQ_PLACE_INFO_bioseq_set,  &av);
   if (ptr -> seq_ids != NULL) {
      if ( ! ID2SBioseqIdsAsnWrite(ptr -> seq_ids, aip, ID2S_BIOSEQ_PLACE_INFO_seq_ids)) {
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
*    ID2SBioseqSetIdsFree()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqSetIdsPtr LIBCALL
ID2SBioseqSetIdsFree(ID2SBioseqSetIdsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr,ASNCODE_INTVAL_SLOT);
   return NULL;
}


/**************************************************
*
*    ID2SBioseqSetIdsAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SBioseqSetIdsPtr LIBCALL
ID2SBioseqSetIdsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SBioseqSetIdsPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SBioseqSetIds ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_BIOSEQ_SET_IDS);
   } else {
      atp = AsnLinkType(orig, ID2S_BIOSEQ_SET_IDS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = ID2SBioseqSetIdsFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SBioseqSetIdsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SBioseqSetIdsAsnWrite(ID2SBioseqSetIdsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_BIOSEQ_SET_IDS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr, ASNCODE_INTVAL_SLOT, aip, atp, ID2S_BIOSEQ_SET_IDS_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    ID2SFeatTypeInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SFeatTypeInfoPtr LIBCALL
ID2SFeatTypeInfoNew(void)
{
   ID2SFeatTypeInfoPtr ptr = MemNew((size_t) sizeof(ID2SFeatTypeInfo));

   return ptr;

}


/**************************************************
*
*    ID2SFeatTypeInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SFeatTypeInfoPtr LIBCALL
ID2SFeatTypeInfoFree(ID2SFeatTypeInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> subtypes ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SFeatTypeInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SFeatTypeInfoPtr LIBCALL
ID2SFeatTypeInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SFeatTypeInfoPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SFeatTypeInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_FEAT_TYPE_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_FEAT_TYPE_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SFeatTypeInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_FEAT_TYPE_INFO_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_FEAT_TYPE_INFO_subtypes) {
      ptr -> subtypes = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> subtypes == NULL) {
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
   ptr = ID2SFeatTypeInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SFeatTypeInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SFeatTypeInfoAsnWrite(ID2SFeatTypeInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_FEAT_TYPE_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, ID2S_FEAT_TYPE_INFO_type,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> subtypes ,ASNCODE_INTVAL_SLOT, aip, ID2S_FEAT_TYPE_INFO_subtypes, ID2S_FEAT_TYPE_INFO_subtypes_E);
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
*    ID2SSeqLocFree()
*
**************************************************/
NLM_EXTERN 
ID2SSeqLocPtr LIBCALL
ID2SSeqLocFree(ValNodePtr anp)
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
   case ID2SSeqLoc_whole_seq_id:
      SeqIdFree(anp -> data.ptrvalue);
      break;
   case ID2SSeqLoc_whole_gi_range:
      ID2SGiRangeFree(anp -> data.ptrvalue);
      break;
   case ID2SSeqLoc_gi_interval:
      ID2SGiIntervalFree(anp -> data.ptrvalue);
      break;
   case ID2SSeqLoc_seq_id_interval:
      ID2SSeqIdIntervalFree(anp -> data.ptrvalue);
      break;
   case ID2SSeqLoc_gi_ints:
      ID2SGiIntsFree(anp -> data.ptrvalue);
      break;
   case ID2SSeqLoc_seq_id_ints:
      ID2SSeqIdIntsFree(anp -> data.ptrvalue);
      break;
   case ID2SSeqLoc_loc_set:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) ID2SSeqLocFree);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ID2SSeqLocAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SSeqLocPtr LIBCALL
ID2SSeqLocAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SSeqLoc ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_SEQ_LOC);
   } else {
      atp = AsnLinkType(orig, ID2S_SEQ_LOC);    /* link in local tree */
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
   if (atp == ID2S_SEQ_LOC_whole_gi) {
      choice = ID2SSeqLoc_whole_gi;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ID2S_SEQ_LOC_whole_seq_id) {
      choice = ID2SSeqLoc_whole_seq_id;
      func = (AsnReadFunc) SeqIdAsnRead;
   }
   else if (atp == ID2S_SEQ_LOC_whole_gi_range) {
      choice = ID2SSeqLoc_whole_gi_range;
      func = (AsnReadFunc) ID2SGiRangeAsnRead;
   }
   else if (atp == ID2S_SEQ_LOC_gi_interval) {
      choice = ID2SSeqLoc_gi_interval;
      func = (AsnReadFunc) ID2SGiIntervalAsnRead;
   }
   else if (atp == ID2S_SEQ_LOC_seq_id_interval) {
      choice = ID2SSeqLoc_seq_id_interval;
      func = (AsnReadFunc) ID2SSeqIdIntervalAsnRead;
   }
   else if (atp == ID2S_SEQ_LOC_gi_ints) {
      choice = ID2SSeqLoc_gi_ints;
      func = (AsnReadFunc) ID2SGiIntsAsnRead;
   }
   else if (atp == ID2S_SEQ_LOC_seq_id_ints) {
      choice = ID2SSeqLoc_seq_id_ints;
      func = (AsnReadFunc) ID2SSeqIdIntsAsnRead;
   }
   else if (atp == ID2S_SEQ_LOC_loc_set) {
      choice = ID2SSeqLoc_loc_set;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SSeqLocAsnRead,             (AsnOptFreeFunc) ID2SSeqLocFree);
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
*    ID2SSeqLocAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SSeqLocAsnWrite(ID2SSeqLocPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ID2S_SEQ_LOC);   /* link local tree */
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
   case ID2SSeqLoc_whole_gi:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ID2S_SEQ_LOC_whole_gi, &av);
      break;
   case ID2SSeqLoc_whole_seq_id:
      writetype = ID2S_SEQ_LOC_whole_seq_id;
      func = (AsnWriteFunc) SeqIdAsnWrite;
      break;
   case ID2SSeqLoc_whole_gi_range:
      writetype = ID2S_SEQ_LOC_whole_gi_range;
      func = (AsnWriteFunc) ID2SGiRangeAsnWrite;
      break;
   case ID2SSeqLoc_gi_interval:
      writetype = ID2S_SEQ_LOC_gi_interval;
      func = (AsnWriteFunc) ID2SGiIntervalAsnWrite;
      break;
   case ID2SSeqLoc_seq_id_interval:
      writetype = ID2S_SEQ_LOC_seq_id_interval;
      func = (AsnWriteFunc) ID2SSeqIdIntervalAsnWrite;
      break;
   case ID2SSeqLoc_gi_ints:
      writetype = ID2S_SEQ_LOC_gi_ints;
      func = (AsnWriteFunc) ID2SGiIntsAsnWrite;
      break;
   case ID2SSeqLoc_seq_id_ints:
      writetype = ID2S_SEQ_LOC_seq_id_ints;
      func = (AsnWriteFunc) ID2SSeqIdIntsAsnWrite;
      break;
   case ID2SSeqLoc_loc_set:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) ID2SSeqLocAsnWrite, aip, ID2S_SEQ_LOC_loc_set, ID2S_SEQ_LOC_loc_set_E);
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
*    ID2SChunkNew()
*
**************************************************/
NLM_EXTERN 
ID2SChunkPtr LIBCALL
ID2SChunkNew(void)
{
   ID2SChunkPtr ptr = MemNew((size_t) sizeof(ID2SChunk));

   return ptr;

}


/**************************************************
*
*    ID2SChunkFree()
*
**************************************************/
NLM_EXTERN 
ID2SChunkPtr LIBCALL
ID2SChunkFree(ID2SChunkPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> data, (AsnOptFreeFunc) ID2SChunkDataFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SChunkAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SChunkPtr LIBCALL
ID2SChunkAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SChunkPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SChunk ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_CHUNK);
   } else {
      atp = AsnLinkType(orig, ID2S_CHUNK);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SChunkNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_CHUNK_data) {
      ptr -> data = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SChunkDataAsnRead, (AsnOptFreeFunc) ID2SChunkDataFree);
      if (isError && ptr -> data == NULL) {
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
   ptr = ID2SChunkFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SChunkAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SChunkAsnWrite(ID2SChunkPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_CHUNK);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericUserSeqOfAsnWrite(ptr -> data, (AsnWriteFunc) ID2SChunkDataAsnWrite, aip, ID2S_CHUNK_data, ID2S_CHUNK_data_E);
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
*    ID2SChunkDataNew()
*
**************************************************/
NLM_EXTERN 
ID2SChunkDataPtr LIBCALL
ID2SChunkDataNew(void)
{
   ID2SChunkDataPtr ptr = MemNew((size_t) sizeof(ID2SChunkData));

   return ptr;

}


/**************************************************
*
*    ID2SChunkDataFree()
*
**************************************************/
NLM_EXTERN 
ID2SChunkDataPtr LIBCALL
ID2SChunkDataFree(ID2SChunkDataPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   Id_idFree(ptr -> Id_id);
   SeqDescrFree(ptr -> descr);
   AsnGenericUserSeqOfFree(ptr -> annots, (AsnOptFreeFunc) SeqAnnotFree);
   AsnGenericUserSeqOfFree(ptr -> assembly, (AsnOptFreeFunc) SeqAlignFree);
   AsnGenericUserSeqOfFree(ptr -> seq_map, (AsnOptFreeFunc) ID2SSequencePieceFree);
   AsnGenericUserSeqOfFree(ptr -> seq_data, (AsnOptFreeFunc) ID2SSequencePieceFree);
   AsnGenericUserSeqOfFree(ptr -> bioseqs, (AsnOptFreeFunc) BioseqFree);
   return MemFree(ptr);
}


/**************************************************
*
*    Id_idFree()
*
**************************************************/
static 
Id_idPtr LIBCALL
Id_idFree(ValNodePtr anp)
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
   case Id_id_seq_id:
      SeqIdFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ID2SChunkDataAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SChunkDataPtr LIBCALL
ID2SChunkDataAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SChunkDataPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SChunkData ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_CHUNK_DATA);
   } else {
      atp = AsnLinkType(orig, ID2S_CHUNK_DATA);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SChunkDataNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_CHUNK_DATA_id) {
      ptr -> Id_id = Id_idAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_CHUNK_DATA_descr) {
      ptr -> descr = SeqDescrAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_CHUNK_DATA_annots) {
      ptr -> annots = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqAnnotAsnRead, (AsnOptFreeFunc) SeqAnnotFree);
      if (isError && ptr -> annots == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_CHUNK_DATA_assembly) {
      ptr -> assembly = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqAlignAsnRead, (AsnOptFreeFunc) SeqAlignFree);
      if (isError && ptr -> assembly == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_CHUNK_DATA_seq_map) {
      ptr -> seq_map = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SSequencePieceAsnRead, (AsnOptFreeFunc) ID2SSequencePieceFree);
      if (isError && ptr -> seq_map == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_CHUNK_DATA_seq_data) {
      ptr -> seq_data = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SSequencePieceAsnRead, (AsnOptFreeFunc) ID2SSequencePieceFree);
      if (isError && ptr -> seq_data == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_CHUNK_DATA_bioseqs) {
      ptr -> bioseqs = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BioseqAsnRead, (AsnOptFreeFunc) BioseqFree);
      if (isError && ptr -> bioseqs == NULL) {
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
   ptr = ID2SChunkDataFree(ptr);
   goto ret;
}



/**************************************************
*
*    Id_idAsnRead()
*
**************************************************/
static 
Id_idPtr LIBCALL
Id_idAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Id_id ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_CHUNK_DATA_id);
   } else {
      atp = AsnLinkType(orig, ID2S_CHUNK_DATA_id);    /* link in local tree */
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
   if (atp == ID2S_CHUNK_DATA_id_bioseq_set) {
      choice = Id_id_bioseq_set;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ID2S_CHUNK_DATA_id_gi) {
      choice = Id_id_gi;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ID2S_CHUNK_DATA_id_seq_id) {
      choice = Id_id_seq_id;
      func = (AsnReadFunc) SeqIdAsnRead;
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
*    ID2SChunkDataAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SChunkDataAsnWrite(ID2SChunkDataPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_CHUNK_DATA);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> Id_id != NULL) {
      if ( ! Id_idAsnWrite(ptr -> Id_id, aip, ID2S_CHUNK_DATA_id)) {
         goto erret;
      }
   }
   if (ptr -> descr != NULL) {
      if ( ! SeqDescrAsnWrite(ptr -> descr, aip, ID2S_CHUNK_DATA_descr)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> annots, (AsnWriteFunc) SeqAnnotAsnWrite, aip, ID2S_CHUNK_DATA_annots, ID2S_CHUNK_DATA_annots_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> assembly, (AsnWriteFunc) SeqAlignAsnWrite, aip, ID2S_CHUNK_DATA_assembly, ID2S_CHUNK_DATA_assembly_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> seq_map, (AsnWriteFunc) ID2SSequencePieceAsnWrite, aip, ID2S_CHUNK_DATA_seq_map, ID2S_CHUNK_DATA_seq_map_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> seq_data, (AsnWriteFunc) ID2SSequencePieceAsnWrite, aip, ID2S_CHUNK_DATA_seq_data, ID2S_CHUNK_DATA_seq_data_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> bioseqs, (AsnWriteFunc) BioseqAsnWrite, aip, ID2S_CHUNK_DATA_bioseqs, ID2S_CHUNK_DATA_bioseqs_E);
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
*    Id_idAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Id_idAsnWrite(Id_idPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, ID2S_CHUNK_DATA_id);   /* link local tree */
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
   case Id_id_bioseq_set:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ID2S_CHUNK_DATA_id_bioseq_set, &av);
      break;
   case Id_id_gi:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ID2S_CHUNK_DATA_id_gi, &av);
      break;
   case Id_id_seq_id:
      writetype = ID2S_CHUNK_DATA_id_seq_id;
      func = (AsnWriteFunc) SeqIdAsnWrite;
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
*    ID2SSequencePieceNew()
*
**************************************************/
NLM_EXTERN 
ID2SSequencePiecePtr LIBCALL
ID2SSequencePieceNew(void)
{
   ID2SSequencePiecePtr ptr = MemNew((size_t) sizeof(ID2SSequencePiece));

   return ptr;

}


/**************************************************
*
*    ID2SSequencePieceFree()
*
**************************************************/
NLM_EXTERN 
ID2SSequencePiecePtr LIBCALL
ID2SSequencePieceFree(ID2SSequencePiecePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> data, (AsnOptFreeFunc) SeqLiteralFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SSequencePieceAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SSequencePiecePtr LIBCALL
ID2SSequencePieceAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SSequencePiecePtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SSequencePiece ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_SEQUENCE_PIECE);
   } else {
      atp = AsnLinkType(orig, ID2S_SEQUENCE_PIECE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SSequencePieceNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_SEQUENCE_PIECE_start) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> start = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SEQUENCE_PIECE_data) {
      ptr -> data = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqLiteralAsnRead, (AsnOptFreeFunc) SeqLiteralFree);
      if (isError && ptr -> data == NULL) {
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
   ptr = ID2SSequencePieceFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SSequencePieceAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SSequencePieceAsnWrite(ID2SSequencePiecePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_SEQUENCE_PIECE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> start;
   retval = AsnWrite(aip, ID2S_SEQUENCE_PIECE_start,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> data, (AsnWriteFunc) SeqLiteralAsnWrite, aip, ID2S_SEQUENCE_PIECE_data, ID2S_SEQUENCE_PIECE_data_E);
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
*    ID2SGiRangeNew()
*
**************************************************/
NLM_EXTERN 
ID2SGiRangePtr LIBCALL
ID2SGiRangeNew(void)
{
   ID2SGiRangePtr ptr = MemNew((size_t) sizeof(ID2SGiRange));

   ptr -> count = 1;
   return ptr;

}


/**************************************************
*
*    ID2SGiRangeFree()
*
**************************************************/
NLM_EXTERN 
ID2SGiRangePtr LIBCALL
ID2SGiRangeFree(ID2SGiRangePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SGiRangeAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SGiRangePtr LIBCALL
ID2SGiRangeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SGiRangePtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SGiRange ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_GI_RANGE);
   } else {
      atp = AsnLinkType(orig, ID2S_GI_RANGE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SGiRangeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_GI_RANGE_start) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> start = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_GI_RANGE_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> count = av.intvalue;
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
   ptr = ID2SGiRangeFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SGiRangeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SGiRangeAsnWrite(ID2SGiRangePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_GI_RANGE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> start;
   retval = AsnWrite(aip, ID2S_GI_RANGE_start,  &av);
   av.intvalue = ptr -> count;
   retval = AsnWrite(aip, ID2S_GI_RANGE_count,  &av);
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
*    ID2SGiIntervalNew()
*
**************************************************/
NLM_EXTERN 
ID2SGiIntervalPtr LIBCALL
ID2SGiIntervalNew(void)
{
   ID2SGiIntervalPtr ptr = MemNew((size_t) sizeof(ID2SGiInterval));

   ptr -> length = 1;
   return ptr;

}


/**************************************************
*
*    ID2SGiIntervalFree()
*
**************************************************/
NLM_EXTERN 
ID2SGiIntervalPtr LIBCALL
ID2SGiIntervalFree(ID2SGiIntervalPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SGiIntervalAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SGiIntervalPtr LIBCALL
ID2SGiIntervalAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SGiIntervalPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SGiInterval ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_GI_INTERVAL);
   } else {
      atp = AsnLinkType(orig, ID2S_GI_INTERVAL);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SGiIntervalNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_GI_INTERVAL_gi) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gi = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_GI_INTERVAL_start) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> start = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_GI_INTERVAL_length) {
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
   ptr = ID2SGiIntervalFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SGiIntervalAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SGiIntervalAsnWrite(ID2SGiIntervalPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_GI_INTERVAL);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> gi;
   retval = AsnWrite(aip, ID2S_GI_INTERVAL_gi,  &av);
   av.intvalue = ptr -> start;
   retval = AsnWrite(aip, ID2S_GI_INTERVAL_start,  &av);
   av.intvalue = ptr -> length;
   retval = AsnWrite(aip, ID2S_GI_INTERVAL_length,  &av);
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
*    ID2SSeqIdIntervalNew()
*
**************************************************/
NLM_EXTERN 
ID2SSeqIdIntervalPtr LIBCALL
ID2SSeqIdIntervalNew(void)
{
   ID2SSeqIdIntervalPtr ptr = MemNew((size_t) sizeof(ID2SSeqIdInterval));

   ptr -> length = 1;
   return ptr;

}


/**************************************************
*
*    ID2SSeqIdIntervalFree()
*
**************************************************/
NLM_EXTERN 
ID2SSeqIdIntervalPtr LIBCALL
ID2SSeqIdIntervalFree(ID2SSeqIdIntervalPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SeqIdFree(ptr -> seq_id);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SSeqIdIntervalAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SSeqIdIntervalPtr LIBCALL
ID2SSeqIdIntervalAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SSeqIdIntervalPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SSeqIdInterval ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_SEQ_ID_INTERVAL);
   } else {
      atp = AsnLinkType(orig, ID2S_SEQ_ID_INTERVAL);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SSeqIdIntervalNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_SEQ_ID_INTERVAL_seq_id) {
      ptr -> seq_id = SeqIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SEQ_ID_INTERVAL_start) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> start = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SEQ_ID_INTERVAL_length) {
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
   ptr = ID2SSeqIdIntervalFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SSeqIdIntervalAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SSeqIdIntervalAsnWrite(ID2SSeqIdIntervalPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_SEQ_ID_INTERVAL);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> seq_id != NULL) {
      if ( ! SeqIdAsnWrite(ptr -> seq_id, aip, ID2S_SEQ_ID_INTERVAL_seq_id)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> start;
   retval = AsnWrite(aip, ID2S_SEQ_ID_INTERVAL_start,  &av);
   av.intvalue = ptr -> length;
   retval = AsnWrite(aip, ID2S_SEQ_ID_INTERVAL_length,  &av);
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
*    ID2SGiIntsNew()
*
**************************************************/
NLM_EXTERN 
ID2SGiIntsPtr LIBCALL
ID2SGiIntsNew(void)
{
   ID2SGiIntsPtr ptr = MemNew((size_t) sizeof(ID2SGiInts));

   return ptr;

}


/**************************************************
*
*    ID2SGiIntsFree()
*
**************************************************/
NLM_EXTERN 
ID2SGiIntsPtr LIBCALL
ID2SGiIntsFree(ID2SGiIntsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> ints, (AsnOptFreeFunc) ID2SIntervalFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SGiIntsAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SGiIntsPtr LIBCALL
ID2SGiIntsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SGiIntsPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SGiInts ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_GI_INTS);
   } else {
      atp = AsnLinkType(orig, ID2S_GI_INTS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SGiIntsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_GI_INTS_gi) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gi = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_GI_INTS_ints) {
      ptr -> ints = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SIntervalAsnRead, (AsnOptFreeFunc) ID2SIntervalFree);
      if (isError && ptr -> ints == NULL) {
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
   ptr = ID2SGiIntsFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SGiIntsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SGiIntsAsnWrite(ID2SGiIntsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_GI_INTS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> gi;
   retval = AsnWrite(aip, ID2S_GI_INTS_gi,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> ints, (AsnWriteFunc) ID2SIntervalAsnWrite, aip, ID2S_GI_INTS_ints, ID2S_GI_INTS_ints_E);
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
*    ID2SSeqIdIntsNew()
*
**************************************************/
NLM_EXTERN 
ID2SSeqIdIntsPtr LIBCALL
ID2SSeqIdIntsNew(void)
{
   ID2SSeqIdIntsPtr ptr = MemNew((size_t) sizeof(ID2SSeqIdInts));

   return ptr;

}


/**************************************************
*
*    ID2SSeqIdIntsFree()
*
**************************************************/
NLM_EXTERN 
ID2SSeqIdIntsPtr LIBCALL
ID2SSeqIdIntsFree(ID2SSeqIdIntsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SeqIdFree(ptr -> seq_id);
   AsnGenericUserSeqOfFree(ptr -> ints, (AsnOptFreeFunc) ID2SIntervalFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SSeqIdIntsAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SSeqIdIntsPtr LIBCALL
ID2SSeqIdIntsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SSeqIdIntsPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SSeqIdInts ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_SEQ_ID_INTS);
   } else {
      atp = AsnLinkType(orig, ID2S_SEQ_ID_INTS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SSeqIdIntsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_SEQ_ID_INTS_seq_id) {
      ptr -> seq_id = SeqIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SEQ_ID_INTS_ints) {
      ptr -> ints = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SIntervalAsnRead, (AsnOptFreeFunc) ID2SIntervalFree);
      if (isError && ptr -> ints == NULL) {
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
   ptr = ID2SSeqIdIntsFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SSeqIdIntsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SSeqIdIntsAsnWrite(ID2SSeqIdIntsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_SEQ_ID_INTS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> seq_id != NULL) {
      if ( ! SeqIdAsnWrite(ptr -> seq_id, aip, ID2S_SEQ_ID_INTS_seq_id)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> ints, (AsnWriteFunc) ID2SIntervalAsnWrite, aip, ID2S_SEQ_ID_INTS_ints, ID2S_SEQ_ID_INTS_ints_E);
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
*    ID2SIntervalNew()
*
**************************************************/
NLM_EXTERN 
ID2SIntervalPtr LIBCALL
ID2SIntervalNew(void)
{
   ID2SIntervalPtr ptr = MemNew((size_t) sizeof(ID2SInterval));

   ptr -> length = 1;
   return ptr;

}


/**************************************************
*
*    ID2SIntervalFree()
*
**************************************************/
NLM_EXTERN 
ID2SIntervalPtr LIBCALL
ID2SIntervalFree(ID2SIntervalPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SIntervalAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SIntervalPtr LIBCALL
ID2SIntervalAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SIntervalPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SInterval ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_INTERVAL);
   } else {
      atp = AsnLinkType(orig, ID2S_INTERVAL);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SIntervalNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2S_INTERVAL_start) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> start = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_INTERVAL_length) {
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
   ptr = ID2SIntervalFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SIntervalAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SIntervalAsnWrite(ID2SIntervalPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ID2S_INTERVAL);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> start;
   retval = AsnWrite(aip, ID2S_INTERVAL_start,  &av);
   av.intvalue = ptr -> length;
   retval = AsnWrite(aip, ID2S_INTERVAL_length,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

