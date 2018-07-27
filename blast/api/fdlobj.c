#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <fdlKludge.h>
#include <fdlobj.h>

static Boolean loaded = FALSE;

#include <fastadl.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
fdlobjAsnLoad(void)
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
*    Generated object loaders for Module NCBI-BlastDL
*    Generated using ASNCODE Revision: 6.12 at Nov 2, 2001  5:01 PM
*
**************************************************/


/**************************************************
*
*    BlastDefLineSetFree()
*
**************************************************/
NLM_EXTERN 
BlastDefLineSetPtr LIBCALL
BlastDefLineSetFree(BlastDefLineSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) BlastDefLineFree);
   return NULL;
}


/**************************************************
*
*    BlastDefLineSetAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastDefLineSetPtr LIBCALL
BlastDefLineSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastDefLineSetPtr ptr;

   if (! loaded)
   {
      if (! fdlobjAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastDefLineSet ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_DEF_LINE_SET);
   } else {
      atp = AsnLinkType(orig, BLAST_DEF_LINE_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BlastDefLineAsnRead, (AsnOptFreeFunc) BlastDefLineFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = BlastDefLineSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastDefLineSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastDefLineSetAsnWrite(BlastDefLineSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! fdlobjAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_DEF_LINE_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) BlastDefLineAsnWrite, aip, atp, BLAST_DEF_LINE_SET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    BlastDefLineNew()
*
**************************************************/
NLM_EXTERN 
BlastDefLinePtr LIBCALL
BlastDefLineNew(void)
{
   BlastDefLinePtr ptr = MemNew((size_t) sizeof(BlastDefLine));

   return ptr;

}


/**************************************************
*
*    BlastDefLineFree()
*
**************************************************/
NLM_EXTERN 
BlastDefLinePtr LIBCALL
BlastDefLineFree(BlastDefLinePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> title);
   AsnGenericChoiceSeqOfFree(ptr -> seqid, (AsnOptFreeFunc) SeqIdFree);
   AsnGenericBaseSeqOfFree(ptr -> memberships ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> links ,ASNCODE_INTVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> other_info ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    BlastDefLineAsnRead()
*
**************************************************/
NLM_EXTERN 
BlastDefLinePtr LIBCALL
BlastDefLineAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BlastDefLinePtr ptr;

   if (! loaded)
   {
      if (! fdlobjAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BlastDefLine ::= (self contained) */
      atp = AsnReadId(aip, amp, BLAST_DEF_LINE);
   } else {
      atp = AsnLinkType(orig, BLAST_DEF_LINE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BlastDefLineNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BLAST_DEF_LINE_title) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> title = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DEF_LINE_seqid) {
      ptr -> seqid = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqIdAsnRead, (AsnOptFreeFunc) SeqIdFree);
      if (isError && ptr -> seqid == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DEF_LINE_taxid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> taxid = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DEF_LINE_memberships) {
      ptr -> memberships = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> memberships == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DEF_LINE_links) {
      ptr -> links = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> links == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BLAST_DEF_LINE_other_info) {
      ptr -> other_info = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> other_info == NULL) {
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
   ptr = BlastDefLineFree(ptr);
   goto ret;
}



/**************************************************
*
*    BlastDefLineAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
BlastDefLineAsnWrite(BlastDefLinePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! fdlobjAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BLAST_DEF_LINE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> title != NULL) {
      av.ptrvalue = ptr -> title;
      retval = AsnWrite(aip, BLAST_DEF_LINE_title,  &av);
   }
   AsnGenericChoiceSeqOfAsnWrite(ptr -> seqid, (AsnWriteFunc) SeqIdAsnWrite, aip, BLAST_DEF_LINE_seqid, BLAST_DEF_LINE_seqid_E);
   av.intvalue = ptr -> taxid;
   retval = AsnWrite(aip, BLAST_DEF_LINE_taxid,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> memberships ,ASNCODE_INTVAL_SLOT, aip, BLAST_DEF_LINE_memberships, BLAST_DEF_LINE_memberships_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> links ,ASNCODE_INTVAL_SLOT, aip, BLAST_DEF_LINE_links, BLAST_DEF_LINE_links_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> other_info ,ASNCODE_INTVAL_SLOT, aip, BLAST_DEF_LINE_other_info, BLAST_DEF_LINE_other_info_E);
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

