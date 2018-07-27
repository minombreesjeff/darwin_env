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
*    Generated object loaders for Module NCBI-Seq-split
*    Generated using ASNCODE Revision: 6.0 at May 3, 2004  6:18 PM
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
   ID2SeqLocFree(ptr -> seq_loc);
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
      ptr -> seq_loc = ID2SeqLocAsnRead(aip, atp);
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
      if ( ! ID2SeqLocAsnWrite(ptr -> seq_loc, aip, ID2S_SEQ_ANNOT_INFO_seq_loc)) {
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
   ID2IdRangeFree(ptr -> bioseqs);
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
      ptr -> bioseqs = ID2IdRangeAsnRead(aip, atp);
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
      if ( ! ID2IdRangeAsnWrite(ptr -> bioseqs, aip, ID2S_BIOSEQS_INFO_bioseqs)) {
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
   ID2SSequenceSplitInfoFree(ptr -> sequence_split);
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
   if (atp == ID2S_BIOSEQ_INFO_sequence_split) {
      ptr -> sequence_split = ID2SSequenceSplitInfoAsnRead(aip, atp);
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
   if (ptr -> sequence_split != NULL) {
      if ( ! ID2SSequenceSplitInfoAsnWrite(ptr -> sequence_split, aip, ID2S_BIOSEQ_INFO_sequence_split)) {
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
*    ID2IdRangeNew()
*
**************************************************/
NLM_EXTERN 
ID2IdRangePtr LIBCALL
ID2IdRangeNew(void)
{
   ID2IdRangePtr ptr = MemNew((size_t) sizeof(ID2IdRange));

   ptr -> count = 1;
   return ptr;

}


/**************************************************
*
*    ID2IdRangeFree()
*
**************************************************/
NLM_EXTERN 
ID2IdRangePtr LIBCALL
ID2IdRangeFree(ID2IdRangePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ID2IdRangeAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2IdRangePtr LIBCALL
ID2IdRangeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2IdRangePtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2IdRange ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_ID_RANGE);
   } else {
      atp = AsnLinkType(orig, ID2_ID_RANGE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2IdRangeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_ID_RANGE_start) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> start = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_ID_RANGE_count) {
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
   ptr = ID2IdRangeFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2IdRangeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2IdRangeAsnWrite(ID2IdRangePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, ID2_ID_RANGE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> start;
   retval = AsnWrite(aip, ID2_ID_RANGE_start,  &av);
   av.intvalue = ptr -> count;
   retval = AsnWrite(aip, ID2_ID_RANGE_count,  &av);
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
*    ID2SSequenceSplitInfoNew()
*
**************************************************/
NLM_EXTERN 
ID2SSequenceSplitInfoPtr LIBCALL
ID2SSequenceSplitInfoNew(void)
{
   ID2SSequenceSplitInfoPtr ptr = MemNew((size_t) sizeof(ID2SSequenceSplitInfo));

   return ptr;

}


/**************************************************
*
*    ID2SSequenceSplitInfoFree()
*
**************************************************/
NLM_EXTERN 
ID2SSequenceSplitInfoPtr LIBCALL
ID2SSequenceSplitInfoFree(ID2SSequenceSplitInfoPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> chunk_blocks ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SSequenceSplitInfoAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SSequenceSplitInfoPtr LIBCALL
ID2SSequenceSplitInfoAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SSequenceSplitInfoPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SSequenceSplitInfo ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2S_SEQUENCE_SPLIT_INFO);
   } else {
      atp = AsnLinkType(orig, ID2S_SEQUENCE_SPLIT_INFO);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SSequenceSplitInfoNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == SEQUENCE_SPLIT_INFO_block_size) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> block_size = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SPLIT_INFO_chunk_start) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> chunk_start = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SPLIT_INFO_chunk_blocks) {
      ptr -> chunk_blocks = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> chunk_blocks == NULL) {
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
   ptr = ID2SSequenceSplitInfoFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SSequenceSplitInfoAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SSequenceSplitInfoAsnWrite(ID2SSequenceSplitInfoPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, ID2S_SEQUENCE_SPLIT_INFO);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> block_size;
   retval = AsnWrite(aip, SEQUENCE_SPLIT_INFO_block_size,  &av);
   av.intvalue = ptr -> chunk_start;
   retval = AsnWrite(aip, SPLIT_INFO_chunk_start,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> chunk_blocks ,ASNCODE_INTVAL_SLOT, aip, SPLIT_INFO_chunk_blocks, SPLIT_INFO_chunk_blocks_E);
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
      ID2SeqLocFree(anp -> data.ptrvalue);
      break;
   case ID2SChunkContent_seq_data:
      ID2SeqLocFree(anp -> data.ptrvalue);
      break;
   case ID2SChunkContent_seq_annot_place:
      ID2SSeqAnnotPlaceInfoFree(anp -> data.ptrvalue);
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
      func = (AsnReadFunc) ID2SeqLocAsnRead;
   }
   else if (atp == ID2S_CHUNK_CONTENT_seq_data) {
      choice = ID2SChunkContent_seq_data;
      func = (AsnReadFunc) ID2SeqLocAsnRead;
   }
   else if (atp == CHUNK_CONTENT_seq_annot_place) {
      choice = ID2SChunkContent_seq_annot_place;
      func = (AsnReadFunc) ID2SSeqAnnotPlaceInfoAsnRead;
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
      func = (AsnWriteFunc) ID2SeqLocAsnWrite;
      break;
   case ID2SChunkContent_seq_data:
      writetype = ID2S_CHUNK_CONTENT_seq_data;
      func = (AsnWriteFunc) ID2SeqLocAsnWrite;
      break;
   case ID2SChunkContent_seq_annot_place:
      writetype = CHUNK_CONTENT_seq_annot_place;
      func = (AsnWriteFunc) ID2SSeqAnnotPlaceInfoAsnWrite;
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
   AsnGenericUserSeqOfFree(ptr -> bioseqs, (AsnOptFreeFunc) ID2IdRangeFree);
   AsnGenericUserSeqOfFree(ptr -> bioseq_sets, (AsnOptFreeFunc) ID2IdRangeFree);
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
      ptr -> bioseqs = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2IdRangeAsnRead, (AsnOptFreeFunc) ID2IdRangeFree);
      if (isError && ptr -> bioseqs == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_SEQ_DESCR_INFO_bioseq_sets) {
      ptr -> bioseq_sets = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2IdRangeAsnRead, (AsnOptFreeFunc) ID2IdRangeFree);
      if (isError && ptr -> bioseq_sets == NULL) {
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
   AsnGenericUserSeqOfAsnWrite(ptr -> bioseqs, (AsnWriteFunc) ID2IdRangeAsnWrite, aip, ID2S_SEQ_DESCR_INFO_bioseqs, ID2S_SEQ_DESCR_INFO_bioseqs_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> bioseq_sets, (AsnWriteFunc) ID2IdRangeAsnWrite, aip, ID2S_SEQ_DESCR_INFO_bioseq_sets, SEQ_DESCR_INFO_bioseq_sets_E);
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
   AsnGenericUserSeqOfFree(ptr -> bioseqs, (AsnOptFreeFunc) ID2IdRangeFree);
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
      ptr -> bioseqs = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2IdRangeAsnRead, (AsnOptFreeFunc) ID2IdRangeFree);
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

   AsnGenericUserSeqOfAsnWrite(ptr -> bioseqs, (AsnWriteFunc) ID2IdRangeAsnWrite, aip, ID2S_SEQ_ASSEMBLY_INFO_bioseqs, SEQ_ASSEMBLY_INFO_bioseqs_E);
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
   AsnGenericUserSeqOfFree(ptr -> bioseqs, (AsnOptFreeFunc) ID2IdRangeFree);
   AsnGenericUserSeqOfFree(ptr -> bioseq_sets, (AsnOptFreeFunc) ID2IdRangeFree);
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
      ptr -> bioseqs = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2IdRangeAsnRead, (AsnOptFreeFunc) ID2IdRangeFree);
      if (isError && ptr -> bioseqs == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ANNOT_PLACE_INFO_bioseq_sets) {
      ptr -> bioseq_sets = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2IdRangeAsnRead, (AsnOptFreeFunc) ID2IdRangeFree);
      if (isError && ptr -> bioseq_sets == NULL) {
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
   AsnGenericUserSeqOfAsnWrite(ptr -> bioseqs, (AsnWriteFunc) ID2IdRangeAsnWrite, aip, SEQ_ANNOT_PLACE_INFO_bioseqs, SEQ_ANNOT_PLACE_INFO_bioseqs_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> bioseq_sets, (AsnWriteFunc) ID2IdRangeAsnWrite, aip, ANNOT_PLACE_INFO_bioseq_sets, ANNOT_PLACE_INFO_bioseq_sets_E);
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
*    ID2SeqLocFree()
*
**************************************************/
NLM_EXTERN 
ID2SeqLocPtr LIBCALL
ID2SeqLocFree(ValNodePtr anp)
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
   case ID2SeqLoc_interval:
      ID2IntervalFree(anp -> data.ptrvalue);
      break;
   case ID2SeqLoc_packed_ints:
      ID2PackedSeqIntsFree(anp -> data.ptrvalue);
      break;
   case ID2SeqLoc_gi_whole_range:
      ID2IdRangeFree(anp -> data.ptrvalue);
      break;
   case ID2SeqLoc_loc_set:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) ID2SeqLocFree);
      break;
   case ID2SeqLoc_seq_loc:
      SeqLocFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ID2SeqLocAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SeqLocPtr LIBCALL
ID2SeqLocAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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

   if (orig == NULL) {         /* ID2SeqLoc ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_SEQ_LOC);
   } else {
      atp = AsnLinkType(orig, ID2_SEQ_LOC);    /* link in local tree */
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
   if (atp == ID2_SEQ_LOC_gi_whole) {
      choice = ID2SeqLoc_gi_whole;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == ID2_SEQ_LOC_interval) {
      choice = ID2SeqLoc_interval;
      func = (AsnReadFunc) ID2IntervalAsnRead;
   }
   else if (atp == ID2_SEQ_LOC_packed_ints) {
      choice = ID2SeqLoc_packed_ints;
      func = (AsnReadFunc) ID2PackedSeqIntsAsnRead;
   }
   else if (atp == ID2_SEQ_LOC_gi_whole_range) {
      choice = ID2SeqLoc_gi_whole_range;
      func = (AsnReadFunc) ID2IdRangeAsnRead;
   }
   else if (atp == ID2_SEQ_LOC_loc_set) {
      choice = ID2SeqLoc_loc_set;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SeqLocAsnRead,             (AsnOptFreeFunc) ID2SeqLocFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == ID2_SEQ_LOC_seq_loc) {
      choice = ID2SeqLoc_seq_loc;
      func = (AsnReadFunc) SeqLocAsnRead;
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
*    ID2SeqLocAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SeqLocAsnWrite(ID2SeqLocPtr anp, AsnIoPtr aip, AsnTypePtr orig)

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

   atp = AsnLinkType(orig, ID2_SEQ_LOC);   /* link local tree */
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
   case ID2SeqLoc_gi_whole:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, ID2_SEQ_LOC_gi_whole, &av);
      break;
   case ID2SeqLoc_interval:
      writetype = ID2_SEQ_LOC_interval;
      func = (AsnWriteFunc) ID2IntervalAsnWrite;
      break;
   case ID2SeqLoc_packed_ints:
      writetype = ID2_SEQ_LOC_packed_ints;
      func = (AsnWriteFunc) ID2PackedSeqIntsAsnWrite;
      break;
   case ID2SeqLoc_gi_whole_range:
      writetype = ID2_SEQ_LOC_gi_whole_range;
      func = (AsnWriteFunc) ID2IdRangeAsnWrite;
      break;
   case ID2SeqLoc_loc_set:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) ID2SeqLocAsnWrite, aip, ID2_SEQ_LOC_loc_set, ID2_SEQ_LOC_loc_set_E);
      break;
   case ID2SeqLoc_seq_loc:
      writetype = ID2_SEQ_LOC_seq_loc;
      func = (AsnWriteFunc) SeqLocAsnWrite;
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
   AsnGenericChoiceSeqOfFree(ptr -> descrs, (AsnOptFreeFunc) SeqDescrFree);
   AsnGenericUserSeqOfFree(ptr -> annots, (AsnOptFreeFunc) SeqAnnotFree);
   AsnGenericUserSeqOfFree(ptr -> assembly, (AsnOptFreeFunc) SeqAlignFree);
   AsnGenericUserSeqOfFree(ptr -> seq_map, (AsnOptFreeFunc) SeqLiteralFree);
   AsnGenericUserSeqOfFree(ptr -> seq_data, (AsnOptFreeFunc) SeqLiteralFree);
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
   if (atp == ID2S_CHUNK_DATA_descrs) {
      ptr -> descrs = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqDescrAsnRead, (AsnOptFreeFunc) SeqDescrFree);
      if (isError && ptr -> descrs == NULL) {
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
      ptr -> seq_map = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqLiteralAsnRead, (AsnOptFreeFunc) SeqLiteralFree);
      if (isError && ptr -> seq_map == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2S_CHUNK_DATA_seq_data) {
      ptr -> seq_data = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqLiteralAsnRead, (AsnOptFreeFunc) SeqLiteralFree);
      if (isError && ptr -> seq_data == NULL) {
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
   AsnGenericChoiceSeqOfAsnWrite(ptr -> descrs, (AsnWriteFunc) SeqDescrAsnWrite, aip, ID2S_CHUNK_DATA_descrs, ID2S_CHUNK_DATA_descrs_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> annots, (AsnWriteFunc) SeqAnnotAsnWrite, aip, ID2S_CHUNK_DATA_annots, ID2S_CHUNK_DATA_annots_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> assembly, (AsnWriteFunc) SeqAlignAsnWrite, aip, ID2S_CHUNK_DATA_assembly, ID2S_CHUNK_DATA_assembly_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> seq_map, (AsnWriteFunc) SeqLiteralAsnWrite, aip, ID2S_CHUNK_DATA_seq_map, ID2S_CHUNK_DATA_seq_map_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> seq_data, (AsnWriteFunc) SeqLiteralAsnWrite, aip, ID2S_CHUNK_DATA_seq_data, ID2S_CHUNK_DATA_seq_data_E);
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
*    ID2IntervalNew()
*
**************************************************/
NLM_EXTERN 
ID2IntervalPtr LIBCALL
ID2IntervalNew(void)
{
   ID2IntervalPtr ptr = MemNew((size_t) sizeof(ID2Interval));

   ptr -> length = 1;
   return ptr;

}


/**************************************************
*
*    ID2IntervalFree()
*
**************************************************/
NLM_EXTERN 
ID2IntervalPtr LIBCALL
ID2IntervalFree(ID2IntervalPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ID2IntervalAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2IntervalPtr LIBCALL
ID2IntervalAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2IntervalPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2Interval ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_INTERVAL);
   } else {
      atp = AsnLinkType(orig, ID2_INTERVAL);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2IntervalNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_INTERVAL_gi) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gi = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_INTERVAL_start) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> start = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_INTERVAL_length) {
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
   ptr = ID2IntervalFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2IntervalAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2IntervalAsnWrite(ID2IntervalPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, ID2_INTERVAL);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> gi;
   retval = AsnWrite(aip, ID2_INTERVAL_gi,  &av);
   av.intvalue = ptr -> start;
   retval = AsnWrite(aip, ID2_INTERVAL_start,  &av);
   av.intvalue = ptr -> length;
   retval = AsnWrite(aip, ID2_INTERVAL_length,  &av);
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
*    ID2PackedSeqIntsNew()
*
**************************************************/
NLM_EXTERN 
ID2PackedSeqIntsPtr LIBCALL
ID2PackedSeqIntsNew(void)
{
   ID2PackedSeqIntsPtr ptr = MemNew((size_t) sizeof(ID2PackedSeqInts));

   return ptr;

}


/**************************************************
*
*    ID2PackedSeqIntsFree()
*
**************************************************/
NLM_EXTERN 
ID2PackedSeqIntsPtr LIBCALL
ID2PackedSeqIntsFree(ID2PackedSeqIntsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> intervals, (AsnOptFreeFunc) ID2SeqRangeFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ID2PackedSeqIntsAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2PackedSeqIntsPtr LIBCALL
ID2PackedSeqIntsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2PackedSeqIntsPtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2PackedSeqInts ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_PACKED_SEQ_INTS);
   } else {
      atp = AsnLinkType(orig, ID2_PACKED_SEQ_INTS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2PackedSeqIntsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_PACKED_SEQ_INTS_gi) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gi = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_PACKED_SEQ_INTS_intervals) {
      ptr -> intervals = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ID2SeqRangeAsnRead, (AsnOptFreeFunc) ID2SeqRangeFree);
      if (isError && ptr -> intervals == NULL) {
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
   ptr = ID2PackedSeqIntsFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2PackedSeqIntsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2PackedSeqIntsAsnWrite(ID2PackedSeqIntsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, ID2_PACKED_SEQ_INTS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> gi;
   retval = AsnWrite(aip, ID2_PACKED_SEQ_INTS_gi,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> intervals, (AsnWriteFunc) ID2SeqRangeAsnWrite, aip, ID2_PACKED_SEQ_INTS_intervals, ID2_PACKED_SEQ_INTS_intervals_E);
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
*    ID2SeqRangeNew()
*
**************************************************/
NLM_EXTERN 
ID2SeqRangePtr LIBCALL
ID2SeqRangeNew(void)
{
   ID2SeqRangePtr ptr = MemNew((size_t) sizeof(ID2SeqRange));

   ptr -> length = 1;
   return ptr;

}


/**************************************************
*
*    ID2SeqRangeFree()
*
**************************************************/
NLM_EXTERN 
ID2SeqRangePtr LIBCALL
ID2SeqRangeFree(ID2SeqRangePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ID2SeqRangeAsnRead()
*
**************************************************/
NLM_EXTERN 
ID2SeqRangePtr LIBCALL
ID2SeqRangeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ID2SeqRangePtr ptr;

   if (! loaded)
   {
      if (! id2sgenAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ID2SeqRange ::= (self contained) */
      atp = AsnReadId(aip, amp, ID2_SEQ_RANGE);
   } else {
      atp = AsnLinkType(orig, ID2_SEQ_RANGE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ID2SeqRangeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ID2_SEQ_RANGE_start) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> start = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ID2_SEQ_RANGE_length) {
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
   ptr = ID2SeqRangeFree(ptr);
   goto ret;
}



/**************************************************
*
*    ID2SeqRangeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ID2SeqRangeAsnWrite(ID2SeqRangePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, ID2_SEQ_RANGE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> start;
   retval = AsnWrite(aip, ID2_SEQ_RANGE_start,  &av);
   av.intvalue = ptr -> length;
   retval = AsnWrite(aip, ID2_SEQ_RANGE_length,  &av);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

