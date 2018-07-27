#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <objsset.h>
#include <objmmdb1.h>
#include <objmmdb3.h>
#include <objcn3d.h>
#include <objcdd.h>

static Boolean loaded = FALSE;

#include <cdd.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objcddAsnLoad(void)
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
*    Generated object loaders for Module NCBI-Cdd
*    Generated using ASNCODE Revision: 6.12 at Oct 17, 2001  2:17 PM
*
**************************************************/


/**************************************************
*
*    CddIdFree()
*
**************************************************/
NLM_EXTERN 
CddIdPtr LIBCALL
CddIdFree(ValNodePtr anp)
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
   case CddId_gid:
      GlobalIdFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    CddIdAsnRead()
*
**************************************************/
NLM_EXTERN 
CddIdPtr LIBCALL
CddIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddId ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_ID);
   } else {
      atp = AsnLinkType(orig, CDD_ID);    /* link in local tree */
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
   if (atp == CDD_ID_uid) {
      choice = CddId_uid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == CDD_ID_gid) {
      choice = CddId_gid;
      func = (AsnReadFunc) GlobalIdAsnRead;
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
*    CddIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddIdAsnWrite(CddIdPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, CDD_ID);   /* link local tree */
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
   case CddId_uid:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, CDD_ID_uid, &av);
      break;
   case CddId_gid:
      writetype = CDD_ID_gid;
      func = (AsnWriteFunc) GlobalIdAsnWrite;
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
*    CddIdSetFree()
*
**************************************************/
NLM_EXTERN 
CddIdSetPtr LIBCALL
CddIdSetFree(CddIdSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr, (AsnOptFreeFunc) CddIdFree);
   return NULL;
}


/**************************************************
*
*    CddIdSetAsnRead()
*
**************************************************/
NLM_EXTERN 
CddIdSetPtr LIBCALL
CddIdSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddIdSetPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddIdSet ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_ID_SET);
   } else {
      atp = AsnLinkType(orig, CDD_ID_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) CddIdAsnRead, (AsnOptFreeFunc) CddIdFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = CddIdSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddIdSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddIdSetAsnWrite(CddIdSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CDD_ID_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericChoiceSeqOfAsnWrite(ptr , (AsnWriteFunc) CddIdAsnWrite, aip, atp, CDD_ID_SET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    CddNew()
*
**************************************************/
NLM_EXTERN 
CddPtr LIBCALL
CddNew(void)
{
   CddPtr ptr = MemNew((size_t) sizeof(Cdd));

   return ptr;

}


/**************************************************
*
*    CddFree()
*
**************************************************/
NLM_EXTERN 
CddPtr LIBCALL
CddFree(CddPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   CddIdSetFree(ptr -> id);
   CddDescrSetFree(ptr -> description);
   { SeqAnnotPtr sap, next; sap = ptr -> seqannot; while(sap) { next = sap->next; sap->next = NULL; sap = SeqAnnotFree(sap); sap = next; } }
   BiostrucAnnotSetFree(ptr -> features);
   SeqEntryFree(ptr -> sequences);
   SeqIntFree(ptr -> profile_range);
   BioseqFree(ptr -> trunc_master);
   MatrixFree(ptr -> posfreq);
   MatrixFree(ptr -> scoremat);
   TriangleFree(ptr -> distance);
   CddIdSetFree(ptr -> parents);
   CddIdSetFree(ptr -> children);
   CddIdSetFree(ptr -> siblings);
   CddIdSetFree(ptr -> neighbors);
   AsnGenericUserSeqOfFree(ptr -> pending, (AsnOptFreeFunc) UpdateAlignFree);
   AsnGenericUserSeqOfFree(ptr -> rejects, (AsnOptFreeFunc) RejectIdFree);
   AsnGenericChoiceSeqOfFree(ptr -> master3d, (AsnOptFreeFunc) SeqIdFree);
   AlignAnnotSetFree(ptr -> alignannot);
   Cn3dStyleDictionaryFree(ptr -> style_dictionary);
   Cn3dUserAnnotationsFree(ptr -> user_annotations);
   return MemFree(ptr);
}


/**************************************************
*
*    CddAsnRead()
*
**************************************************/
NLM_EXTERN 
CddPtr LIBCALL
CddAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Cdd ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD);
   } else {
      atp = AsnLinkType(orig, CDD);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CddNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CDD_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_id) {
      ptr -> id = CddIdSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_description) {
      ptr -> description = CddDescrSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_seqannot) {
      ptr -> seqannot = SeqAnnotSetAsnRead(aip,CDD_seqannot,CDD_seqannot_E);
      if (isError && ptr -> seqannot == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_features) {
      ptr -> features = BiostrucAnnotSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_sequences) {
      ptr -> sequences = SeqEntryAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_profile_range) {
      ptr -> profile_range = SeqIntAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_trunc_master) {
      ptr -> trunc_master = BioseqAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_posfreq) {
      ptr -> posfreq = MatrixAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_scoremat) {
      ptr -> scoremat = MatrixAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_distance) {
      ptr -> distance = TriangleAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_parents) {
      ptr -> parents = CddIdSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_children) {
      ptr -> children = CddIdSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_siblings) {
      ptr -> siblings = CddIdSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_neighbors) {
      ptr -> neighbors = CddIdSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_pending) {
      ptr -> pending = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) UpdateAlignAsnRead, (AsnOptFreeFunc) UpdateAlignFree);
      if (isError && ptr -> pending == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_rejects) {
      ptr -> rejects = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) RejectIdAsnRead, (AsnOptFreeFunc) RejectIdFree);
      if (isError && ptr -> rejects == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_master3d) {
      ptr -> master3d = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqIdAsnRead, (AsnOptFreeFunc) SeqIdFree);
      if (isError && ptr -> master3d == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_alignannot) {
      ptr -> alignannot = AlignAnnotSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_style_dictionary) {
      ptr -> style_dictionary = Cn3dStyleDictionaryAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_user_annotations) {
      ptr -> user_annotations = Cn3dUserAnnotationsAsnRead(aip, atp);
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
   ptr = CddFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddAsnWrite(CddPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CDD);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, CDD_name,  &av);
   }
   if (ptr -> id != NULL) {
      if ( ! CddIdSetAsnWrite(ptr -> id, aip, CDD_id)) {
         goto erret;
      }
   }
   if (ptr -> description != NULL) {
      if ( ! CddDescrSetAsnWrite(ptr -> description, aip, CDD_description)) {
         goto erret;
      }
   }
   if (ptr->seqannot != NULL) if (SeqAnnotSetAsnWrite(ptr->seqannot,aip,CDD_seqannot,CDD_seqannot_E)==FALSE) goto erret;
   if (ptr -> features != NULL) {
      if ( ! BiostrucAnnotSetAsnWrite(ptr -> features, aip, CDD_features)) {
         goto erret;
      }
   }
   if (ptr -> sequences != NULL) {
      if ( ! SeqEntryAsnWrite(ptr -> sequences, aip, CDD_sequences)) {
         goto erret;
      }
   }
   if (ptr -> profile_range != NULL) {
      if ( ! SeqIntAsnWrite(ptr -> profile_range, aip, CDD_profile_range)) {
         goto erret;
      }
   }
   if (ptr -> trunc_master != NULL) {
      if ( ! BioseqAsnWrite(ptr -> trunc_master, aip, CDD_trunc_master)) {
         goto erret;
      }
   }
   if (ptr -> posfreq != NULL) {
      if ( ! MatrixAsnWrite(ptr -> posfreq, aip, CDD_posfreq)) {
         goto erret;
      }
   }
   if (ptr -> scoremat != NULL) {
      if ( ! MatrixAsnWrite(ptr -> scoremat, aip, CDD_scoremat)) {
         goto erret;
      }
   }
   if (ptr -> distance != NULL) {
      if ( ! TriangleAsnWrite(ptr -> distance, aip, CDD_distance)) {
         goto erret;
      }
   }
   if (ptr -> parents != NULL) {
      if ( ! CddIdSetAsnWrite(ptr -> parents, aip, CDD_parents)) {
         goto erret;
      }
   }
   if (ptr -> children != NULL) {
      if ( ! CddIdSetAsnWrite(ptr -> children, aip, CDD_children)) {
         goto erret;
      }
   }
   if (ptr -> siblings != NULL) {
      if ( ! CddIdSetAsnWrite(ptr -> siblings, aip, CDD_siblings)) {
         goto erret;
      }
   }
   if (ptr -> neighbors != NULL) {
      if ( ! CddIdSetAsnWrite(ptr -> neighbors, aip, CDD_neighbors)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> pending, (AsnWriteFunc) UpdateAlignAsnWrite, aip, CDD_pending, CDD_pending_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> rejects, (AsnWriteFunc) RejectIdAsnWrite, aip, CDD_rejects, CDD_rejects_E);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> master3d, (AsnWriteFunc) SeqIdAsnWrite, aip, CDD_master3d, CDD_master3d_E);
   if (ptr -> alignannot != NULL) {
      if ( ! AlignAnnotSetAsnWrite(ptr -> alignannot, aip, CDD_alignannot)) {
         goto erret;
      }
   }
   if (ptr -> style_dictionary != NULL) {
      if ( ! Cn3dStyleDictionaryAsnWrite(ptr -> style_dictionary, aip, CDD_style_dictionary)) {
         goto erret;
      }
   }
   if (ptr -> user_annotations != NULL) {
      if ( ! Cn3dUserAnnotationsAsnWrite(ptr -> user_annotations, aip, CDD_user_annotations)) {
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
*    CddSetFree()
*
**************************************************/
NLM_EXTERN 
CddSetPtr LIBCALL
CddSetFree(CddSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) CddFree);
   return NULL;
}


/**************************************************
*
*    CddSetAsnRead()
*
**************************************************/
NLM_EXTERN 
CddSetPtr LIBCALL
CddSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddSetPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddSet ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_SET);
   } else {
      atp = AsnLinkType(orig, CDD_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) CddAsnRead, (AsnOptFreeFunc) CddFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = CddSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddSetAsnWrite(CddSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CDD_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) CddAsnWrite, aip, atp, CDD_SET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    CddTreeNew()
*
**************************************************/
NLM_EXTERN 
CddTreePtr LIBCALL
CddTreeNew(void)
{
   CddTreePtr ptr = MemNew((size_t) sizeof(CddTree));

   return ptr;

}


/**************************************************
*
*    CddTreeFree()
*
**************************************************/
NLM_EXTERN 
CddTreePtr LIBCALL
CddTreeFree(CddTreePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   CddIdSetFree(ptr -> id);
   CddDescrSetFree(ptr -> description);
   CddIdSetFree(ptr -> parents);
   CddIdSetFree(ptr -> children);
   CddIdSetFree(ptr -> siblings);
   CddIdSetFree(ptr -> neighbors);
   return MemFree(ptr);
}


/**************************************************
*
*    CddTreeAsnRead()
*
**************************************************/
NLM_EXTERN 
CddTreePtr LIBCALL
CddTreeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddTreePtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddTree ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_TREE);
   } else {
      atp = AsnLinkType(orig, CDD_TREE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CddTreeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CDD_TREE_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_TREE_id) {
      ptr -> id = CddIdSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_TREE_description) {
      ptr -> description = CddDescrSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_TREE_parents) {
      ptr -> parents = CddIdSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_TREE_children) {
      ptr -> children = CddIdSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_TREE_siblings) {
      ptr -> siblings = CddIdSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_TREE_neighbors) {
      ptr -> neighbors = CddIdSetAsnRead(aip, atp);
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
   ptr = CddTreeFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddTreeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddTreeAsnWrite(CddTreePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CDD_TREE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, CDD_TREE_name,  &av);
   }
   if (ptr -> id != NULL) {
      if ( ! CddIdSetAsnWrite(ptr -> id, aip, CDD_TREE_id)) {
         goto erret;
      }
   }
   if (ptr -> description != NULL) {
      if ( ! CddDescrSetAsnWrite(ptr -> description, aip, CDD_TREE_description)) {
         goto erret;
      }
   }
   if (ptr -> parents != NULL) {
      if ( ! CddIdSetAsnWrite(ptr -> parents, aip, CDD_TREE_parents)) {
         goto erret;
      }
   }
   if (ptr -> children != NULL) {
      if ( ! CddIdSetAsnWrite(ptr -> children, aip, CDD_TREE_children)) {
         goto erret;
      }
   }
   if (ptr -> siblings != NULL) {
      if ( ! CddIdSetAsnWrite(ptr -> siblings, aip, CDD_TREE_siblings)) {
         goto erret;
      }
   }
   if (ptr -> neighbors != NULL) {
      if ( ! CddIdSetAsnWrite(ptr -> neighbors, aip, CDD_TREE_neighbors)) {
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
*    CddTreeSetFree()
*
**************************************************/
NLM_EXTERN 
CddTreeSetPtr LIBCALL
CddTreeSetFree(CddTreeSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) CddTreeFree);
   return NULL;
}


/**************************************************
*
*    CddTreeSetAsnRead()
*
**************************************************/
NLM_EXTERN 
CddTreeSetPtr LIBCALL
CddTreeSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddTreeSetPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddTreeSet ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_TREE_SET);
   } else {
      atp = AsnLinkType(orig, CDD_TREE_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) CddTreeAsnRead, (AsnOptFreeFunc) CddTreeFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = CddTreeSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddTreeSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddTreeSetAsnWrite(CddTreeSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CDD_TREE_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) CddTreeAsnWrite, aip, atp, CDD_TREE_SET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    GlobalIdNew()
*
**************************************************/
NLM_EXTERN 
GlobalIdPtr LIBCALL
GlobalIdNew(void)
{
   GlobalIdPtr ptr = MemNew((size_t) sizeof(GlobalId));

   return ptr;

}


/**************************************************
*
*    GlobalIdFree()
*
**************************************************/
NLM_EXTERN 
GlobalIdPtr LIBCALL
GlobalIdFree(GlobalIdPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> accession);
   MemFree(ptr -> release);
   MemFree(ptr -> database);
   return MemFree(ptr);
}


/**************************************************
*
*    GlobalIdAsnRead()
*
**************************************************/
NLM_EXTERN 
GlobalIdPtr LIBCALL
GlobalIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GlobalIdPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GlobalId ::= (self contained) */
      atp = AsnReadId(aip, amp, GLOBAL_ID);
   } else {
      atp = AsnLinkType(orig, GLOBAL_ID);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GlobalIdNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GLOBAL_ID_accession) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> accession = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLOBAL_ID_release) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> release = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLOBAL_ID_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> version = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GLOBAL_ID_database) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> database = av.ptrvalue;
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
   ptr = GlobalIdFree(ptr);
   goto ret;
}



/**************************************************
*
*    GlobalIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GlobalIdAsnWrite(GlobalIdPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GLOBAL_ID);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> accession != NULL) {
      av.ptrvalue = ptr -> accession;
      retval = AsnWrite(aip, GLOBAL_ID_accession,  &av);
   }
   if (ptr -> release != NULL) {
      av.ptrvalue = ptr -> release;
      retval = AsnWrite(aip, GLOBAL_ID_release,  &av);
   }
   av.intvalue = ptr -> version;
   retval = AsnWrite(aip, GLOBAL_ID_version,  &av);
   if (ptr -> database != NULL) {
      av.ptrvalue = ptr -> database;
      retval = AsnWrite(aip, GLOBAL_ID_database,  &av);
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
*    CddRepeatNew()
*
**************************************************/
NLM_EXTERN 
CddRepeatPtr LIBCALL
CddRepeatNew(void)
{
   CddRepeatPtr ptr = MemNew((size_t) sizeof(CddRepeat));

   return ptr;

}


/**************************************************
*
*    CddRepeatFree()
*
**************************************************/
NLM_EXTERN 
CddRepeatPtr LIBCALL
CddRepeatFree(CddRepeatPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SeqLocFree(ptr -> location);
   return MemFree(ptr);
}


/**************************************************
*
*    CddRepeatAsnRead()
*
**************************************************/
NLM_EXTERN 
CddRepeatPtr LIBCALL
CddRepeatAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddRepeatPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddRepeat ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_REPEAT);
   } else {
      atp = AsnLinkType(orig, CDD_REPEAT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CddRepeatNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CDD_REPEAT_count) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> count = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_REPEAT_location) {
      ptr -> location = SeqLocAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_REPEAT_avglen) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> avglen = av.intvalue;
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
   ptr = CddRepeatFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddRepeatAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddRepeatAsnWrite(CddRepeatPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CDD_REPEAT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> count;
   retval = AsnWrite(aip, CDD_REPEAT_count,  &av);
   if (ptr -> location != NULL) {
      if ( ! SeqLocAsnWrite(ptr -> location, aip, CDD_REPEAT_location)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> avglen;
   retval = AsnWrite(aip, CDD_REPEAT_avglen,  &av);
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
*    CddDescrFree()
*
**************************************************/
NLM_EXTERN 
CddDescrPtr LIBCALL
CddDescrFree(ValNodePtr anp)
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
   case CddDescr_othername:
      MemFree(anp -> data.ptrvalue);
      break;
   case CddDescr_category:
      MemFree(anp -> data.ptrvalue);
      break;
   case CddDescr_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case CddDescr_reference:
      PubFree(anp -> data.ptrvalue);
      break;
   case CddDescr_create_date:
      DateFree(anp -> data.ptrvalue);
      break;
   case CddDescr_tax_source:
      OrgRefFree(anp -> data.ptrvalue);
      break;
   case CddDescr_source:
      MemFree(anp -> data.ptrvalue);
      break;
   case CddDescr_update_date:
      DateFree(anp -> data.ptrvalue);
      break;
   case CddDescr_scrapbook:
      AsnGenericBaseSeqOfFree((ValNodePtr) pnt,ASNCODE_PTRVAL_SLOT);
      break;
   case CddDescr_source_id:
      CddIdSetFree(anp -> data.ptrvalue);
      break;
   case CddDescr_repeats:
      CddRepeatFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    CddDescrAsnRead()
*
**************************************************/
NLM_EXTERN 
CddDescrPtr LIBCALL
CddDescrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddDescr ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_DESCR);
   } else {
      atp = AsnLinkType(orig, CDD_DESCR);    /* link in local tree */
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
   if (atp == CDD_DESCR_othername) {
      choice = CddDescr_othername;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == CDD_DESCR_category) {
      choice = CddDescr_category;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == CDD_DESCR_comment) {
      choice = CddDescr_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == CDD_DESCR_reference) {
      choice = CddDescr_reference;
      func = (AsnReadFunc) PubAsnRead;
   }
   else if (atp == CDD_DESCR_create_date) {
      choice = CddDescr_create_date;
      func = (AsnReadFunc) DateAsnRead;
   }
   else if (atp == CDD_DESCR_tax_source) {
      choice = CddDescr_tax_source;
      func = (AsnReadFunc) OrgRefAsnRead;
   }
   else if (atp == CDD_DESCR_source) {
      choice = CddDescr_source;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == CDD_DESCR_status) {
      choice = CddDescr_status;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == CDD_DESCR_update_date) {
      choice = CddDescr_update_date;
      func = (AsnReadFunc) DateAsnRead;
   }
   else if (atp == CDD_DESCR_scrapbook) {
      choice = CddDescr_scrapbook;
      anp -> data.ptrvalue = 
      AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == CDD_DESCR_source_id) {
      choice = CddDescr_source_id;
      func = (AsnReadFunc) CddIdSetAsnRead;
   }
   else if (atp == CDD_DESCR_repeats) {
      choice = CddDescr_repeats;
      func = (AsnReadFunc) CddRepeatAsnRead;
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
*    CddDescrAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddDescrAsnWrite(CddDescrPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, CDD_DESCR);   /* link local tree */
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
   case CddDescr_othername:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, CDD_DESCR_othername, &av);
      break;
   case CddDescr_category:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, CDD_DESCR_category, &av);
      break;
   case CddDescr_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, CDD_DESCR_comment, &av);
      break;
   case CddDescr_reference:
      writetype = CDD_DESCR_reference;
      func = (AsnWriteFunc) PubAsnWrite;
      break;
   case CddDescr_create_date:
      writetype = CDD_DESCR_create_date;
      func = (AsnWriteFunc) DateAsnWrite;
      break;
   case CddDescr_tax_source:
      writetype = CDD_DESCR_tax_source;
      func = (AsnWriteFunc) OrgRefAsnWrite;
      break;
   case CddDescr_source:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, CDD_DESCR_source, &av);
      break;
   case CddDescr_status:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, CDD_DESCR_status, &av);
      break;
   case CddDescr_update_date:
      writetype = CDD_DESCR_update_date;
      func = (AsnWriteFunc) DateAsnWrite;
      break;
   case CddDescr_scrapbook:
      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,ASNCODE_PTRVAL_SLOT, aip, CDD_DESCR_scrapbook, CDD_DESCR_scrapbook_E);            break;
   case CddDescr_source_id:
      writetype = CDD_DESCR_source_id;
      func = (AsnWriteFunc) CddIdSetAsnWrite;
      break;
   case CddDescr_repeats:
      writetype = CDD_DESCR_repeats;
      func = (AsnWriteFunc) CddRepeatAsnWrite;
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
*    CddDescrSetFree()
*
**************************************************/
NLM_EXTERN 
CddDescrSetPtr LIBCALL
CddDescrSetFree(CddDescrSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr, (AsnOptFreeFunc) CddDescrFree);
   return NULL;
}


/**************************************************
*
*    CddDescrSetAsnRead()
*
**************************************************/
NLM_EXTERN 
CddDescrSetPtr LIBCALL
CddDescrSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddDescrSetPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddDescrSet ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_DESCR_SET);
   } else {
      atp = AsnLinkType(orig, CDD_DESCR_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) CddDescrAsnRead, (AsnOptFreeFunc) CddDescrFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = CddDescrSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddDescrSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddDescrSetAsnWrite(CddDescrSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CDD_DESCR_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericChoiceSeqOfAsnWrite(ptr , (AsnWriteFunc) CddDescrAsnWrite, aip, atp, CDD_DESCR_SET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    MatrixNew()
*
**************************************************/
NLM_EXTERN 
MatrixPtr LIBCALL
MatrixNew(void)
{
   MatrixPtr ptr = MemNew((size_t) sizeof(Matrix));

   return ptr;

}


/**************************************************
*
*    MatrixFree()
*
**************************************************/
NLM_EXTERN 
MatrixPtr LIBCALL
MatrixFree(MatrixPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> row_labels ,ASNCODE_PTRVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> columns ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    MatrixAsnRead()
*
**************************************************/
NLM_EXTERN 
MatrixPtr LIBCALL
MatrixAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MatrixPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Matrix ::= (self contained) */
      atp = AsnReadId(aip, amp, MATRIX);
   } else {
      atp = AsnLinkType(orig, MATRIX);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MatrixNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MATRIX_ncolumns) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ncolumns = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MATRIX_nrows) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> nrows = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MATRIX_row_labels) {
      ptr -> row_labels = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> row_labels == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MATRIX_scale_factor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scale_factor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MATRIX_columns) {
      ptr -> columns = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> columns == NULL) {
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
   ptr = MatrixFree(ptr);
   goto ret;
}



/**************************************************
*
*    MatrixAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MatrixAsnWrite(MatrixPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MATRIX);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> ncolumns;
   retval = AsnWrite(aip, MATRIX_ncolumns,  &av);
   av.intvalue = ptr -> nrows;
   retval = AsnWrite(aip, MATRIX_nrows,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> row_labels ,ASNCODE_PTRVAL_SLOT, aip, MATRIX_row_labels, MATRIX_row_labels_E);
   av.intvalue = ptr -> scale_factor;
   retval = AsnWrite(aip, MATRIX_scale_factor,  &av);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> columns ,ASNCODE_INTVAL_SLOT, aip, MATRIX_columns, MATRIX_columns_E);
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
*    TriangleNew()
*
**************************************************/
NLM_EXTERN 
TrianglePtr LIBCALL
TriangleNew(void)
{
   TrianglePtr ptr = MemNew((size_t) sizeof(Triangle));

   return ptr;

}


/**************************************************
*
*    TriangleFree()
*
**************************************************/
NLM_EXTERN 
TrianglePtr LIBCALL
TriangleFree(TrianglePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ScoreSetFree(ptr -> scores);
   AsnGenericBaseSeqOfFree(ptr -> div_ranks ,ASNCODE_INTVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    TriangleAsnRead()
*
**************************************************/
NLM_EXTERN 
TrianglePtr LIBCALL
TriangleAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   TrianglePtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Triangle ::= (self contained) */
      atp = AsnReadId(aip, amp, TRIANGLE);
   } else {
      atp = AsnLinkType(orig, TRIANGLE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = TriangleNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == TRIANGLE_nelements) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> nelements = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRIANGLE_scores) {
      ptr -> scores = ScoreSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == TRIANGLE_div_ranks) {
      ptr -> div_ranks = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> div_ranks == NULL) {
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
   ptr = TriangleFree(ptr);
   goto ret;
}



/**************************************************
*
*    TriangleAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
TriangleAsnWrite(TrianglePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, TRIANGLE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> nelements;
   retval = AsnWrite(aip, TRIANGLE_nelements,  &av);
   if (ptr -> scores != NULL) {
      if ( ! ScoreSetAsnWrite(ptr -> scores, aip, TRIANGLE_scores)) {
         goto erret;
      }
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> div_ranks ,ASNCODE_INTVAL_SLOT, aip, TRIANGLE_div_ranks, TRIANGLE_div_ranks_E);
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
*    UpdateCommentFree()
*
**************************************************/
NLM_EXTERN 
UpdateCommentPtr LIBCALL
UpdateCommentFree(ValNodePtr anp)
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
   case UpdateComment_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case UpdateComment_addthis:
      SeqLocFree(anp -> data.ptrvalue);
      break;
   case UpdateComment_replaces:
      SeqLocFree(anp -> data.ptrvalue);
      break;
   case UpdateComment_reject_loc:
      SeqLocFree(anp -> data.ptrvalue);
      break;
   case UpdateComment_reference:
      PubFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    UpdateCommentAsnRead()
*
**************************************************/
NLM_EXTERN 
UpdateCommentPtr LIBCALL
UpdateCommentAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* UpdateComment ::= (self contained) */
      atp = AsnReadId(aip, amp, UPDATE_COMMENT);
   } else {
      atp = AsnLinkType(orig, UPDATE_COMMENT);    /* link in local tree */
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
   if (atp == UPDATE_COMMENT_comment) {
      choice = UpdateComment_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == UPDATE_COMMENT_addthis) {
      choice = UpdateComment_addthis;
      func = (AsnReadFunc) SeqLocAsnRead;
   }
   else if (atp == UPDATE_COMMENT_replaces) {
      choice = UpdateComment_replaces;
      func = (AsnReadFunc) SeqLocAsnRead;
   }
   else if (atp == UPDATE_COMMENT_reject_loc) {
      choice = UpdateComment_reject_loc;
      func = (AsnReadFunc) SeqLocAsnRead;
   }
   else if (atp == UPDATE_COMMENT_reference) {
      choice = UpdateComment_reference;
      func = (AsnReadFunc) PubAsnRead;
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
*    UpdateCommentAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
UpdateCommentAsnWrite(UpdateCommentPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, UPDATE_COMMENT);   /* link local tree */
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
   case UpdateComment_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, UPDATE_COMMENT_comment, &av);
      break;
   case UpdateComment_addthis:
      writetype = UPDATE_COMMENT_addthis;
      func = (AsnWriteFunc) SeqLocAsnWrite;
      break;
   case UpdateComment_replaces:
      writetype = UPDATE_COMMENT_replaces;
      func = (AsnWriteFunc) SeqLocAsnWrite;
      break;
   case UpdateComment_reject_loc:
      writetype = UPDATE_COMMENT_reject_loc;
      func = (AsnWriteFunc) SeqLocAsnWrite;
      break;
   case UpdateComment_reference:
      writetype = UPDATE_COMMENT_reference;
      func = (AsnWriteFunc) PubAsnWrite;
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
*    UpdateAlignNew()
*
**************************************************/
NLM_EXTERN 
UpdateAlignPtr LIBCALL
UpdateAlignNew(void)
{
   UpdateAlignPtr ptr = MemNew((size_t) sizeof(UpdateAlign));

   return ptr;

}


/**************************************************
*
*    UpdateAlignFree()
*
**************************************************/
NLM_EXTERN 
UpdateAlignPtr LIBCALL
UpdateAlignFree(UpdateAlignPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> description, (AsnOptFreeFunc) UpdateCommentFree);
   SeqAnnotFree(ptr -> seqannot);
   return MemFree(ptr);
}


/**************************************************
*
*    UpdateAlignAsnRead()
*
**************************************************/
NLM_EXTERN 
UpdateAlignPtr LIBCALL
UpdateAlignAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   UpdateAlignPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* UpdateAlign ::= (self contained) */
      atp = AsnReadId(aip, amp, UPDATE_ALIGN);
   } else {
      atp = AsnLinkType(orig, UPDATE_ALIGN);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = UpdateAlignNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == UPDATE_ALIGN_description) {
      ptr -> description = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) UpdateCommentAsnRead, (AsnOptFreeFunc) UpdateCommentFree);
      if (isError && ptr -> description == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == UPDATE_ALIGN_seqannot) {
      ptr -> seqannot = SeqAnnotAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == UPDATE_ALIGN_type) {
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
   ptr = UpdateAlignFree(ptr);
   goto ret;
}



/**************************************************
*
*    UpdateAlignAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
UpdateAlignAsnWrite(UpdateAlignPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, UPDATE_ALIGN);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericChoiceSeqOfAsnWrite(ptr -> description, (AsnWriteFunc) UpdateCommentAsnWrite, aip, UPDATE_ALIGN_description, UPDATE_ALIGN_description_E);
   if (ptr -> seqannot != NULL) {
      if ( ! SeqAnnotAsnWrite(ptr -> seqannot, aip, UPDATE_ALIGN_seqannot)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, UPDATE_ALIGN_type,  &av);
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
*    RejectIdNew()
*
**************************************************/
NLM_EXTERN 
RejectIdPtr LIBCALL
RejectIdNew(void)
{
   RejectIdPtr ptr = MemNew((size_t) sizeof(RejectId));

   return ptr;

}


/**************************************************
*
*    RejectIdFree()
*
**************************************************/
NLM_EXTERN 
RejectIdPtr LIBCALL
RejectIdFree(RejectIdPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> description, (AsnOptFreeFunc) UpdateCommentFree);
   AsnGenericChoiceSeqOfFree(ptr -> ids, (AsnOptFreeFunc) SeqIdFree);
   return MemFree(ptr);
}


/**************************************************
*
*    RejectIdAsnRead()
*
**************************************************/
NLM_EXTERN 
RejectIdPtr LIBCALL
RejectIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   RejectIdPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* RejectId ::= (self contained) */
      atp = AsnReadId(aip, amp, REJECT_ID);
   } else {
      atp = AsnLinkType(orig, REJECT_ID);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = RejectIdNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == REJECT_ID_description) {
      ptr -> description = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) UpdateCommentAsnRead, (AsnOptFreeFunc) UpdateCommentFree);
      if (isError && ptr -> description == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == REJECT_ID_ids) {
      ptr -> ids = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqIdAsnRead, (AsnOptFreeFunc) SeqIdFree);
      if (isError && ptr -> ids == NULL) {
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
   ptr = RejectIdFree(ptr);
   goto ret;
}



/**************************************************
*
*    RejectIdAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
RejectIdAsnWrite(RejectIdPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, REJECT_ID);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericChoiceSeqOfAsnWrite(ptr -> description, (AsnWriteFunc) UpdateCommentAsnWrite, aip, REJECT_ID_description, REJECT_ID_description_E);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> ids, (AsnWriteFunc) SeqIdAsnWrite, aip, REJECT_ID_ids, REJECT_ID_ids_E);
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
*    FeatureEvidenceFree()
*
**************************************************/
NLM_EXTERN 
FeatureEvidencePtr LIBCALL
FeatureEvidenceFree(ValNodePtr anp)
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
   case FeatureEvidence_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case FeatureEvidence_reference:
      PubFree(anp -> data.ptrvalue);
      break;
   case FeatureEvidence_bsannot:
      BiostrucAnnotSetFree(anp -> data.ptrvalue);
      break;
   case FeatureEvidence_seqfeat:
      SeqFeatFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    FeatureEvidenceAsnRead()
*
**************************************************/
NLM_EXTERN 
FeatureEvidencePtr LIBCALL
FeatureEvidenceAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* FeatureEvidence ::= (self contained) */
      atp = AsnReadId(aip, amp, FEATURE_EVIDENCE);
   } else {
      atp = AsnLinkType(orig, FEATURE_EVIDENCE);    /* link in local tree */
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
   if (atp == FEATURE_EVIDENCE_comment) {
      choice = FeatureEvidence_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == FEATURE_EVIDENCE_reference) {
      choice = FeatureEvidence_reference;
      func = (AsnReadFunc) PubAsnRead;
   }
   else if (atp == FEATURE_EVIDENCE_bsannot) {
      choice = FeatureEvidence_bsannot;
      func = (AsnReadFunc) BiostrucAnnotSetAsnRead;
   }
   else if (atp == FEATURE_EVIDENCE_seqfeat) {
      choice = FeatureEvidence_seqfeat;
      func = (AsnReadFunc) SeqFeatAsnRead;
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
*    FeatureEvidenceAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
FeatureEvidenceAsnWrite(FeatureEvidencePtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, FEATURE_EVIDENCE);   /* link local tree */
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
   case FeatureEvidence_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, FEATURE_EVIDENCE_comment, &av);
      break;
   case FeatureEvidence_reference:
      writetype = FEATURE_EVIDENCE_reference;
      func = (AsnWriteFunc) PubAsnWrite;
      break;
   case FeatureEvidence_bsannot:
      writetype = FEATURE_EVIDENCE_bsannot;
      func = (AsnWriteFunc) BiostrucAnnotSetAsnWrite;
      break;
   case FeatureEvidence_seqfeat:
      writetype = FEATURE_EVIDENCE_seqfeat;
      func = (AsnWriteFunc) SeqFeatAsnWrite;
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
*    AlignAnnotNew()
*
**************************************************/
NLM_EXTERN 
AlignAnnotPtr LIBCALL
AlignAnnotNew(void)
{
   AlignAnnotPtr ptr = MemNew((size_t) sizeof(AlignAnnot));

   return ptr;

}


/**************************************************
*
*    AlignAnnotFree()
*
**************************************************/
NLM_EXTERN 
AlignAnnotPtr LIBCALL
AlignAnnotFree(AlignAnnotPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SeqLocFree(ptr -> location);
   MemFree(ptr -> description);
   AsnGenericChoiceSeqOfFree(ptr -> evidence, (AsnOptFreeFunc) FeatureEvidenceFree);
   return MemFree(ptr);
}


/**************************************************
*
*    AlignAnnotAsnRead()
*
**************************************************/
NLM_EXTERN 
AlignAnnotPtr LIBCALL
AlignAnnotAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AlignAnnotPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AlignAnnot ::= (self contained) */
      atp = AsnReadId(aip, amp, ALIGN_ANNOT);
   } else {
      atp = AsnLinkType(orig, ALIGN_ANNOT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AlignAnnotNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ALIGN_ANNOT_location) {
      ptr -> location = SeqLocAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_ANNOT_description) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> description = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALIGN_ANNOT_evidence) {
      ptr -> evidence = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) FeatureEvidenceAsnRead, (AsnOptFreeFunc) FeatureEvidenceFree);
      if (isError && ptr -> evidence == NULL) {
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
   ptr = AlignAnnotFree(ptr);
   goto ret;
}



/**************************************************
*
*    AlignAnnotAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AlignAnnotAsnWrite(AlignAnnotPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ALIGN_ANNOT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> location != NULL) {
      if ( ! SeqLocAsnWrite(ptr -> location, aip, ALIGN_ANNOT_location)) {
         goto erret;
      }
   }
   if (ptr -> description != NULL) {
      av.ptrvalue = ptr -> description;
      retval = AsnWrite(aip, ALIGN_ANNOT_description,  &av);
   }
   AsnGenericChoiceSeqOfAsnWrite(ptr -> evidence, (AsnWriteFunc) FeatureEvidenceAsnWrite, aip, ALIGN_ANNOT_evidence, ALIGN_ANNOT_evidence_E);
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
*    AlignAnnotSetFree()
*
**************************************************/
NLM_EXTERN 
AlignAnnotSetPtr LIBCALL
AlignAnnotSetFree(AlignAnnotSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) AlignAnnotFree);
   return NULL;
}


/**************************************************
*
*    AlignAnnotSetAsnRead()
*
**************************************************/
NLM_EXTERN 
AlignAnnotSetPtr LIBCALL
AlignAnnotSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AlignAnnotSetPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AlignAnnotSet ::= (self contained) */
      atp = AsnReadId(aip, amp, ALIGN_ANNOT_SET);
   } else {
      atp = AsnLinkType(orig, ALIGN_ANNOT_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) AlignAnnotAsnRead, (AsnOptFreeFunc) AlignAnnotFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AlignAnnotSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    AlignAnnotSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AlignAnnotSetAsnWrite(AlignAnnotSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ALIGN_ANNOT_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) AlignAnnotAsnWrite, aip, atp, ALIGN_ANNOT_SET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

