#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <objsset.h>
#include <objmmdb1.h>
#include <objmmdb3.h>
#include <objcn3d.h>
#include <objscoremat.h>
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
*    Generated using ASNCODE Revision: 6.15 at Oct 12, 2004 11:03 AM
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
   AsnGenericUserSeqOfFree(ptr -> seqannot, (AsnOptFreeFunc) SeqAnnotFree);
   BiostrucAnnotSetFree(ptr -> features);
   SeqEntryFree(ptr -> sequences);
   SeqIntFree(ptr -> profile_range);
   BioseqFree(ptr -> trunc_master);
   MatrixFree(ptr -> posfreq);
   MatrixFree(ptr -> scoremat);
   TriangleFree(ptr -> distance);
   CddIdFree(ptr -> parent);
   CddIdSetFree(ptr -> children);
   CddIdSetFree(ptr -> siblings);
   CddIdSetFree(ptr -> neighbors);
   AsnGenericUserSeqOfFree(ptr -> pending, (AsnOptFreeFunc) UpdateAlignFree);
   AsnGenericUserSeqOfFree(ptr -> rejects, (AsnOptFreeFunc) RejectIdFree);
   AsnGenericChoiceSeqOfFree(ptr -> master3d, (AsnOptFreeFunc) SeqIdFree);
   AlignAnnotSetFree(ptr -> alignannot);
   Cn3dStyleDictionaryFree(ptr -> style_dictionary);
   Cn3dUserAnnotationsFree(ptr -> user_annotations);
   AsnGenericUserSeqOfFree(ptr -> ancestors, (AsnOptFreeFunc) DomainParentFree);
   PssmWithParametersFree(ptr -> scoreparams);
   SequenceTreeFree(ptr -> seqtree);
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
   if (atp == CDD_parent) {
      ptr -> parent = CddIdAsnRead(aip, atp);
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
   if (atp == CDD_ancestors) {
      ptr -> ancestors = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) DomainParentAsnRead, (AsnOptFreeFunc) DomainParentFree);
      if (isError && ptr -> ancestors == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_scoreparams) {
      ptr -> scoreparams = PssmWithParametersAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_seqtree) {
      ptr -> seqtree = SequenceTreeAsnRead(aip, atp);
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
   AsnGenericUserSeqOfAsnWrite(ptr -> seqannot, (AsnWriteFunc) SeqAnnotAsnWrite, aip, CDD_seqannot, CDD_seqannot_E);
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
   if (ptr -> parent != NULL) {
      if ( ! CddIdAsnWrite(ptr -> parent, aip, CDD_parent)) {
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
   AsnGenericUserSeqOfAsnWrite(ptr -> ancestors, (AsnWriteFunc) DomainParentAsnWrite, aip, CDD_ancestors, CDD_ancestors_E);
   if (ptr -> scoreparams != NULL) {
      if ( ! PssmWithParametersAsnWrite(ptr -> scoreparams, aip, CDD_scoreparams)) {
         goto erret;
      }
   }
   if (ptr -> seqtree != NULL) {
      if ( ! SequenceTreeAsnWrite(ptr -> seqtree, aip, CDD_seqtree)) {
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
   CddIdFree(ptr -> parent);
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
   if (atp == CDD_TREE_parent) {
      ptr -> parent = CddIdAsnRead(aip, atp);
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
   if (ptr -> parent != NULL) {
      if ( ! CddIdAsnWrite(ptr -> parent, aip, CDD_TREE_parent)) {
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
*    CddPrefNodesNew()
*
**************************************************/
NLM_EXTERN 
CddPrefNodesPtr LIBCALL
CddPrefNodesNew(void)
{
   CddPrefNodesPtr ptr = MemNew((size_t) sizeof(CddPrefNodes));

   return ptr;

}


/**************************************************
*
*    CddPrefNodesFree()
*
**************************************************/
NLM_EXTERN 
CddPrefNodesPtr LIBCALL
CddPrefNodesFree(CddPrefNodesPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   CddOrgRefSetFree(ptr -> preferred_nodes);
   CddOrgRefSetFree(ptr -> model_organisms);
   CddOrgRefSetFree(ptr -> optional_nodes);
   CddPrefNodeDescrSetFree(ptr -> description);
   return MemFree(ptr);
}


/**************************************************
*
*    CddPrefNodesAsnRead()
*
**************************************************/
NLM_EXTERN 
CddPrefNodesPtr LIBCALL
CddPrefNodesAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddPrefNodesPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddPrefNodes ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_PREF_NODES);
   } else {
      atp = AsnLinkType(orig, CDD_PREF_NODES);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CddPrefNodesNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CDD_PREF_NODES_preferred_nodes) {
      ptr -> preferred_nodes = CddOrgRefSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_PREF_NODES_model_organisms) {
      ptr -> model_organisms = CddOrgRefSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_PREF_NODES_optional_nodes) {
      ptr -> optional_nodes = CddOrgRefSetAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_PREF_NODES_description) {
      ptr -> description = CddPrefNodeDescrSetAsnRead(aip, atp);
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
   ptr = CddPrefNodesFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddPrefNodesAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddPrefNodesAsnWrite(CddPrefNodesPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, CDD_PREF_NODES);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> preferred_nodes != NULL) {
      if ( ! CddOrgRefSetAsnWrite(ptr -> preferred_nodes, aip, CDD_PREF_NODES_preferred_nodes)) {
         goto erret;
      }
   }
   if (ptr -> model_organisms != NULL) {
      if ( ! CddOrgRefSetAsnWrite(ptr -> model_organisms, aip, CDD_PREF_NODES_model_organisms)) {
         goto erret;
      }
   }
   if (ptr -> optional_nodes != NULL) {
      if ( ! CddOrgRefSetAsnWrite(ptr -> optional_nodes, aip, CDD_PREF_NODES_optional_nodes)) {
         goto erret;
      }
   }
   if (ptr -> description != NULL) {
      if ( ! CddPrefNodeDescrSetAsnWrite(ptr -> description, aip, CDD_PREF_NODES_description)) {
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
*    CddProjectNew()
*
**************************************************/
NLM_EXTERN 
CddProjectPtr LIBCALL
CddProjectNew(void)
{
   CddProjectPtr ptr = MemNew((size_t) sizeof(CddProject));

   return ptr;

}


/**************************************************
*
*    CddProjectFree()
*
**************************************************/
NLM_EXTERN 
CddProjectPtr LIBCALL
CddProjectFree(CddProjectPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> cds, (AsnOptFreeFunc) CddFree);
   AsnGenericBaseSeqOfFree(ptr -> cdcolor ,ASNCODE_INTVAL_SLOT);
   AsnGenericUserSeqOfFree(ptr -> viewers, (AsnOptFreeFunc) CddViewerFree);
   MemFree(ptr -> log);
   AsnGenericUserSeqOfFree(ptr -> scripts, (AsnOptFreeFunc) CddScriptFree);
   return MemFree(ptr);
}


/**************************************************
*
*    CddProjectAsnRead()
*
**************************************************/
NLM_EXTERN 
CddProjectPtr LIBCALL
CddProjectAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddProjectPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddProject ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_PROJECT);
   } else {
      atp = AsnLinkType(orig, CDD_PROJECT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CddProjectNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CDD_PROJECT_cds) {
      ptr -> cds = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) CddAsnRead, (AsnOptFreeFunc) CddFree);
      if (isError && ptr -> cds == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_PROJECT_cdcolor) {
      ptr -> cdcolor = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && ptr -> cdcolor == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_PROJECT_viewers) {
      ptr -> viewers = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) CddViewerAsnRead, (AsnOptFreeFunc) CddViewerFree);
      if (isError && ptr -> viewers == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_PROJECT_log) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> log = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_PROJECT_scripts) {
      ptr -> scripts = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) CddScriptAsnRead, (AsnOptFreeFunc) CddScriptFree);
      if (isError && ptr -> scripts == NULL) {
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
   ptr = CddProjectFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddProjectAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddProjectAsnWrite(CddProjectPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, CDD_PROJECT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericUserSeqOfAsnWrite(ptr -> cds, (AsnWriteFunc) CddAsnWrite, aip, CDD_PROJECT_cds, CDD_PROJECT_cds_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> cdcolor ,ASNCODE_INTVAL_SLOT, aip, CDD_PROJECT_cdcolor, CDD_PROJECT_cdcolor_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> viewers, (AsnWriteFunc) CddViewerAsnWrite, aip, CDD_PROJECT_viewers, CDD_PROJECT_viewers_E);
   if (ptr -> log != NULL) {
      av.ptrvalue = ptr -> log;
      retval = AsnWrite(aip, CDD_PROJECT_log,  &av);
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> scripts, (AsnWriteFunc) CddScriptAsnWrite, aip, CDD_PROJECT_scripts, CDD_PROJECT_scripts_E);
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
*    CddOrgRefNew()
*
**************************************************/
NLM_EXTERN 
CddOrgRefPtr LIBCALL
CddOrgRefNew(void)
{
   CddOrgRefPtr ptr = MemNew((size_t) sizeof(CddOrgRef));

   ptr -> active = 1;
   return ptr;

}


/**************************************************
*
*    CddOrgRefFree()
*
**************************************************/
NLM_EXTERN 
CddOrgRefPtr LIBCALL
CddOrgRefFree(CddOrgRefPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   OrgRefFree((OrgRefPtr)ptr -> reference);
   MemFree(ptr -> rank);
   return MemFree(ptr);
}


/**************************************************
*
*    CddOrgRefAsnRead()
*
**************************************************/
NLM_EXTERN 
CddOrgRefPtr LIBCALL
CddOrgRefAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddOrgRefPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddOrgRef ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_ORG_REF);
   } else {
      atp = AsnLinkType(orig, CDD_ORG_REF);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CddOrgRefNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CDD_ORG_REF_reference) {
      ptr -> reference = OrgRefAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_ORG_REF_active) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> active = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_ORG_REF_parent_tax_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> parent_tax_id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_ORG_REF_rank) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rank = av.ptrvalue;
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
   ptr = CddOrgRefFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddOrgRefAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddOrgRefAsnWrite(CddOrgRefPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, CDD_ORG_REF);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> reference != NULL) {
      if ( ! OrgRefAsnWrite((OrgRefPtr)ptr -> reference, aip, CDD_ORG_REF_reference)) {
         goto erret;
      }
   }
   av.boolvalue = ptr -> active;
   retval = AsnWrite(aip, CDD_ORG_REF_active,  &av);
   av.intvalue = ptr -> parent_tax_id;
   retval = AsnWrite(aip, CDD_ORG_REF_parent_tax_id,  &av);
   if (ptr -> rank != NULL) {
      av.ptrvalue = ptr -> rank;
      retval = AsnWrite(aip, CDD_ORG_REF_rank,  &av);
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
*    CddOrgRefSetFree()
*
**************************************************/
NLM_EXTERN 
CddOrgRefSetPtr LIBCALL
CddOrgRefSetFree(CddOrgRefSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) CddOrgRefFree);
   return NULL;
}


/**************************************************
*
*    CddOrgRefSetAsnRead()
*
**************************************************/
NLM_EXTERN 
CddOrgRefSetPtr LIBCALL
CddOrgRefSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddOrgRefSetPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddOrgRefSet ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_ORG_REF_SET);
   } else {
      atp = AsnLinkType(orig, CDD_ORG_REF_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) CddOrgRefAsnRead, (AsnOptFreeFunc) CddOrgRefFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = CddOrgRefSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddOrgRefSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddOrgRefSetAsnWrite(CddOrgRefSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, CDD_ORG_REF_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) CddOrgRefAsnWrite, aip, atp, CDD_ORG_REF_SET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    CddPrefNodeDescrFree()
*
**************************************************/
NLM_EXTERN 
CddPrefNodeDescrPtr LIBCALL
CddPrefNodeDescrFree(ValNodePtr anp)
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
   case CddPrefNodeDescr_create_date:
      DateFree(anp -> data.ptrvalue);
      break;
   case CddPrefNodeDescr_description:
      MemFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    CddPrefNodeDescrAsnRead()
*
**************************************************/
NLM_EXTERN 
CddPrefNodeDescrPtr LIBCALL
CddPrefNodeDescrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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

   if (orig == NULL) {         /* CddPrefNodeDescr ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_PREF_NODE_DESCR);
   } else {
      atp = AsnLinkType(orig, CDD_PREF_NODE_DESCR);    /* link in local tree */
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
   if (atp == CDD_PREF_NODE_DESCR_create_date) {
      choice = CddPrefNodeDescr_create_date;
      func = (AsnReadFunc) DateAsnRead;
   }
   else if (atp == CDD_PREF_NODE_DESCR_description) {
      choice = CddPrefNodeDescr_description;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
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
*    CddPrefNodeDescrAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddPrefNodeDescrAsnWrite(CddPrefNodeDescrPtr anp, AsnIoPtr aip, AsnTypePtr orig)

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

   atp = AsnLinkType(orig, CDD_PREF_NODE_DESCR);   /* link local tree */
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
   case CddPrefNodeDescr_create_date:
      writetype = CDD_PREF_NODE_DESCR_create_date;
      func = (AsnWriteFunc) DateAsnWrite;
      break;
   case CddPrefNodeDescr_description:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, CDD_PREF_NODE_DESCR_description, &av);
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
*    CddPrefNodeDescrSetFree()
*
**************************************************/
NLM_EXTERN 
CddPrefNodeDescrSetPtr LIBCALL
CddPrefNodeDescrSetFree(CddPrefNodeDescrSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr, (AsnOptFreeFunc) CddPrefNodeDescrFree);
   return NULL;
}


/**************************************************
*
*    CddPrefNodeDescrSetAsnRead()
*
**************************************************/
NLM_EXTERN 
CddPrefNodeDescrSetPtr LIBCALL
CddPrefNodeDescrSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddPrefNodeDescrSetPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddPrefNodeDescrSet ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_PREF_NODE_DESCR_SET);
   } else {
      atp = AsnLinkType(orig, CDD_PREF_NODE_DESCR_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) CddPrefNodeDescrAsnRead, (AsnOptFreeFunc) CddPrefNodeDescrFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = CddPrefNodeDescrSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddPrefNodeDescrSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddPrefNodeDescrSetAsnWrite(CddPrefNodeDescrSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, CDD_PREF_NODE_DESCR_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericChoiceSeqOfAsnWrite(ptr , (AsnWriteFunc) CddPrefNodeDescrAsnWrite, aip, atp, CDD_PREF_NODE_DESCR_SET_E);
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
*    CddBookRefNew()
*
**************************************************/
NLM_EXTERN 
CddBookRefPtr LIBCALL
CddBookRefNew(void)
{
   CddBookRefPtr ptr = MemNew((size_t) sizeof(CddBookRef));

   return ptr;

}


/**************************************************
*
*    CddBookRefFree()
*
**************************************************/
NLM_EXTERN 
CddBookRefPtr LIBCALL
CddBookRefFree(CddBookRefPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> bookname);
   return MemFree(ptr);
}


/**************************************************
*
*    CddBookRefAsnRead()
*
**************************************************/
NLM_EXTERN 
CddBookRefPtr LIBCALL
CddBookRefAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddBookRefPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddBookRef ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_BOOK_REF);
   } else {
      atp = AsnLinkType(orig, CDD_BOOK_REF);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CddBookRefNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CDD_BOOK_REF_bookname) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> bookname = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_BOOK_REF_textelement) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> textelement = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_BOOK_REF_elementid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> elementid = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_BOOK_REF_subelementid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> subelementid = av.intvalue;
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
   ptr = CddBookRefFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddBookRefAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddBookRefAsnWrite(CddBookRefPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, CDD_BOOK_REF);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> bookname != NULL) {
      av.ptrvalue = ptr -> bookname;
      retval = AsnWrite(aip, CDD_BOOK_REF_bookname,  &av);
   }
   av.intvalue = ptr -> textelement;
   retval = AsnWrite(aip, CDD_BOOK_REF_textelement,  &av);
   av.intvalue = ptr -> elementid;
   retval = AsnWrite(aip, CDD_BOOK_REF_elementid,  &av);
   av.intvalue = ptr -> subelementid;
   retval = AsnWrite(aip, CDD_BOOK_REF_subelementid,  &av);
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
   case CddDescr_old_root:
      CddIdSetFree(anp -> data.ptrvalue);
      break;
   case CddDescr_book_ref:
      CddBookRefFree(anp -> data.ptrvalue);
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
   else if (atp == CDD_DESCR_old_root) {
      choice = CddDescr_old_root;
      func = (AsnReadFunc) CddIdSetAsnRead;
   }
   else if (atp == CDD_DESCR_curation_status) {
      choice = CddDescr_curation_status;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == CDD_DESCR_readonly_status) {
      choice = CddDescr_readonly_status;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == CDD_DESCR_book_ref) {
      choice = CddDescr_book_ref;
      func = (AsnReadFunc) CddBookRefAsnRead;
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
   case CddDescr_old_root:
      writetype = CDD_DESCR_old_root;
      func = (AsnWriteFunc) CddIdSetAsnWrite;
      break;
   case CddDescr_curation_status:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, CDD_DESCR_curation_status, &av);
      break;
   case CddDescr_readonly_status:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, CDD_DESCR_readonly_status, &av);
      break;
   case CddDescr_book_ref:
      writetype = CDD_DESCR_book_ref;
      func = (AsnWriteFunc) CddBookRefAsnWrite;
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
   case FeatureEvidence_book_ref:
      CddBookRefFree(anp -> data.ptrvalue);
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
   else if (atp == FEATURE_EVIDENCE_book_ref) {
      choice = FeatureEvidence_book_ref;
      func = (AsnReadFunc) CddBookRefAsnRead;
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
   case FeatureEvidence_book_ref:
      writetype = FEATURE_EVIDENCE_book_ref;
      func = (AsnWriteFunc) CddBookRefAsnWrite;
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



/**************************************************
*
*    DomainParentNew()
*
**************************************************/
NLM_EXTERN 
DomainParentPtr LIBCALL
DomainParentNew(void)
{
   DomainParentPtr ptr = MemNew((size_t) sizeof(DomainParent));

   return ptr;

}


/**************************************************
*
*    DomainParentFree()
*
**************************************************/
NLM_EXTERN 
DomainParentPtr LIBCALL
DomainParentFree(DomainParentPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   CddIdFree(ptr -> parentid);
   SeqAnnotFree(ptr -> seqannot);
   return MemFree(ptr);
}


/**************************************************
*
*    DomainParentAsnRead()
*
**************************************************/
NLM_EXTERN 
DomainParentPtr LIBCALL
DomainParentAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   DomainParentPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* DomainParent ::= (self contained) */
      atp = AsnReadId(aip, amp, DOMAIN_PARENT);
   } else {
      atp = AsnLinkType(orig, DOMAIN_PARENT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = DomainParentNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == DOMAIN_PARENT_parent_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> parent_type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOMAIN_PARENT_parentid) {
      ptr -> parentid = CddIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == DOMAIN_PARENT_seqannot) {
      ptr -> seqannot = SeqAnnotAsnRead(aip, atp);
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
   ptr = DomainParentFree(ptr);
   goto ret;
}



/**************************************************
*
*    DomainParentAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
DomainParentAsnWrite(DomainParentPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, DOMAIN_PARENT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> parent_type;
   retval = AsnWrite(aip, DOMAIN_PARENT_parent_type,  &av);
   if (ptr -> parentid != NULL) {
      if ( ! CddIdAsnWrite(ptr -> parentid, aip, DOMAIN_PARENT_parentid)) {
         goto erret;
      }
   }
   if (ptr -> seqannot != NULL) {
      if ( ! SeqAnnotAsnWrite(ptr -> seqannot, aip, DOMAIN_PARENT_seqannot)) {
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
*    SequenceTreeNew()
*
**************************************************/
NLM_EXTERN 
SequenceTreePtr LIBCALL
SequenceTreeNew(void)
{
   SequenceTreePtr ptr = MemNew((size_t) sizeof(SequenceTree));

   ptr -> isAnnotated = 0;
   return ptr;

}


/**************************************************
*
*    SequenceTreeFree()
*
**************************************************/
NLM_EXTERN 
SequenceTreePtr LIBCALL
SequenceTreeFree(SequenceTreePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> cdAccession);
   AlgorithmTypeFree(ptr -> algorithm);
   SeqTreeNodeFree(ptr -> root);
   return MemFree(ptr);
}


/**************************************************
*
*    SequenceTreeAsnRead()
*
**************************************************/
NLM_EXTERN 
SequenceTreePtr LIBCALL
SequenceTreeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   SequenceTreePtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* SequenceTree ::= (self contained) */
      atp = AsnReadId(aip, amp, SEQUENCE_TREE);
   } else {
      atp = AsnLinkType(orig, SEQUENCE_TREE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = SequenceTreeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == SEQUENCE_TREE_cdAccession) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> cdAccession = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SEQUENCE_TREE_algorithm) {
      ptr -> algorithm = AlgorithmTypeAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SEQUENCE_TREE_isAnnotated) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> isAnnotated = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SEQUENCE_TREE_root) {
      ptr -> root = SeqTreeNodeAsnRead(aip, atp);
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
   ptr = SequenceTreeFree(ptr);
   goto ret;
}



/**************************************************
*
*    SequenceTreeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
SequenceTreeAsnWrite(SequenceTreePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, SEQUENCE_TREE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> cdAccession != NULL) {
      av.ptrvalue = ptr -> cdAccession;
      retval = AsnWrite(aip, SEQUENCE_TREE_cdAccession,  &av);
   }
   if (ptr -> algorithm != NULL) {
      if ( ! AlgorithmTypeAsnWrite(ptr -> algorithm, aip, SEQUENCE_TREE_algorithm)) {
         goto erret;
      }
   }
   av.boolvalue = ptr -> isAnnotated;
   retval = AsnWrite(aip, SEQUENCE_TREE_isAnnotated,  &av);
   if (ptr -> root != NULL) {
      if ( ! SeqTreeNodeAsnWrite(ptr -> root, aip, SEQUENCE_TREE_root)) {
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
*    AlgorithmTypeNew()
*
**************************************************/
NLM_EXTERN 
AlgorithmTypePtr LIBCALL
AlgorithmTypeNew(void)
{
   AlgorithmTypePtr ptr = MemNew((size_t) sizeof(AlgorithmType));

   return ptr;

}


/**************************************************
*
*    AlgorithmTypeFree()
*
**************************************************/
NLM_EXTERN 
AlgorithmTypePtr LIBCALL
AlgorithmTypeFree(AlgorithmTypePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    AlgorithmTypeAsnRead()
*
**************************************************/
NLM_EXTERN 
AlgorithmTypePtr LIBCALL
AlgorithmTypeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AlgorithmTypePtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AlgorithmType ::= (self contained) */
      atp = AsnReadId(aip, amp, ALGORITHM_TYPE);
   } else {
      atp = AsnLinkType(orig, ALGORITHM_TYPE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AlgorithmTypeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ALGORITHM_TYPE_scoring_Scheme) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> scoring_Scheme = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALGORITHM_TYPE_clustering_Method) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> clustering_Method = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALGORITHM_TYPE_score_Matrix) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> score_Matrix = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALGORITHM_TYPE_gapOpen) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gapOpen = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALGORITHM_TYPE_gapExtend) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gapExtend = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALGORITHM_TYPE_gapScaleFactor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gapScaleFactor = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALGORITHM_TYPE_nTerminalExt) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> nTerminalExt = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ALGORITHM_TYPE_cTerminalExt) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> cTerminalExt = av.intvalue;
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
   ptr = AlgorithmTypeFree(ptr);
   goto ret;
}



/**************************************************
*
*    AlgorithmTypeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
AlgorithmTypeAsnWrite(AlgorithmTypePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, ALGORITHM_TYPE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> scoring_Scheme;
   retval = AsnWrite(aip, ALGORITHM_TYPE_scoring_Scheme,  &av);
   av.intvalue = ptr -> clustering_Method;
   retval = AsnWrite(aip, ALGORITHM_TYPE_clustering_Method,  &av);
   av.intvalue = ptr -> score_Matrix;
   retval = AsnWrite(aip, ALGORITHM_TYPE_score_Matrix,  &av);
   av.intvalue = ptr -> gapOpen;
   retval = AsnWrite(aip, ALGORITHM_TYPE_gapOpen,  &av);
   av.intvalue = ptr -> gapExtend;
   retval = AsnWrite(aip, ALGORITHM_TYPE_gapExtend,  &av);
   av.intvalue = ptr -> gapScaleFactor;
   retval = AsnWrite(aip, ALGORITHM_TYPE_gapScaleFactor,  &av);
   av.intvalue = ptr -> nTerminalExt;
   retval = AsnWrite(aip, ALGORITHM_TYPE_nTerminalExt,  &av);
   av.intvalue = ptr -> cTerminalExt;
   retval = AsnWrite(aip, ALGORITHM_TYPE_cTerminalExt,  &av);
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
*    SeqTreeNodeNew()
*
**************************************************/
NLM_EXTERN 
SeqTreeNodePtr LIBCALL
SeqTreeNodeNew(void)
{
   SeqTreeNodePtr ptr = MemNew((size_t) sizeof(SeqTreeNode));

   ptr -> isAnnotated = 0;
   return ptr;

}


/**************************************************
*
*    Children_footprintNew()
*
**************************************************/
static 
Children_footprintPtr LIBCALL
Children_footprintNew(void)
{
   Children_footprintPtr ptr = MemNew((size_t) sizeof(Children_footprint));

   return ptr;

}


/**************************************************
*
*    SeqTreeNodeFree()
*
**************************************************/
NLM_EXTERN 
SeqTreeNodePtr LIBCALL
SeqTreeNodeFree(SeqTreeNodePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   Children_childrenFree(ptr -> Children_children);
   NodeAnnotationFree(ptr -> annotation);
   return MemFree(ptr);
}


/**************************************************
*
*    Children_childrenFree()
*
**************************************************/
static 
Children_childrenPtr LIBCALL
Children_childrenFree(ValNodePtr anp)
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
   case Children_children_children:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqTreeNodeFree);
      break;
   case Children_children_Children_Footprint:
      Children_footprintFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    Children_footprintFree()
*
**************************************************/
static 
Children_footprintPtr LIBCALL
Children_footprintFree(Children_footprintPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   SeqIntFree(ptr -> seqRange);
   return MemFree(ptr);
}


/**************************************************
*
*    SeqTreeNodeAsnRead()
*
**************************************************/
NLM_EXTERN 
SeqTreeNodePtr LIBCALL
SeqTreeNodeAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   SeqTreeNodePtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* SeqTreeNode ::= (self contained) */
      atp = AsnReadId(aip, amp, SEQTREE_NODE);
   } else {
      atp = AsnLinkType(orig, SEQTREE_NODE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = SeqTreeNodeNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == SEQTREE_NODE_isAnnotated) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> isAnnotated = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SEQTREE_NODE_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SEQTREE_NODE_distance) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> distance = av.realvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SEQTREE_NODE_children) {
      ptr -> Children_children = Children_childrenAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SEQTREE_NODE_annotation) {
      ptr -> annotation = NodeAnnotationAsnRead(aip, atp);
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
   ptr = SeqTreeNodeFree(ptr);
   goto ret;
}



/**************************************************
*
*    Children_childrenAsnRead()
*
**************************************************/
static 
Children_childrenPtr LIBCALL
Children_childrenAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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

   if (orig == NULL) {         /* Children_children ::= (self contained) */
      atp = AsnReadId(aip, amp, SEQTREE_NODE_children);
   } else {
      atp = AsnLinkType(orig, SEQTREE_NODE_children);    /* link in local tree */
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
   if (atp == SEQTREE_NODE_children_children) {
      choice = Children_children_children;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqTreeNodeAsnRead,             (AsnOptFreeFunc) SeqTreeNodeFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == SEQTREE_NODE_children_footprint) {
      choice = Children_children_Children_Footprint;
      func = (AsnReadFunc) Children_footprintAsnRead;
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
*    Children_footprintAsnRead()
*
**************************************************/
static 
Children_footprintPtr LIBCALL
Children_footprintAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   Children_footprintPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Children_footprint ::= (self contained) */
      atp = AsnReadId(aip, amp, SEQTREE_NODE_children_footprint);
   } else {
      atp = AsnLinkType(orig, SEQTREE_NODE_children_footprint);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = Children_footprintNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == SEQTREE_NODE_children_footprint_seqRange) {
      ptr -> seqRange = SeqIntAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == SEQTREE_NODE_children_footprint_rowId) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> rowId = av.intvalue;
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
   ptr = Children_footprintFree(ptr);
   goto ret;
}



/**************************************************
*
*    SeqTreeNodeAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
SeqTreeNodeAsnWrite(SeqTreeNodePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, SEQTREE_NODE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.boolvalue = ptr -> isAnnotated;
   retval = AsnWrite(aip, SEQTREE_NODE_isAnnotated,  &av);
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, SEQTREE_NODE_name,  &av);
   }
   av.realvalue = ptr -> distance;
   retval = AsnWrite(aip, SEQTREE_NODE_distance,  &av);
   if (ptr -> Children_children != NULL) {
      if ( ! Children_childrenAsnWrite(ptr -> Children_children, aip, SEQTREE_NODE_children)) {
         goto erret;
      }
   }
   if (ptr -> annotation != NULL) {
      if ( ! NodeAnnotationAsnWrite(ptr -> annotation, aip, SEQTREE_NODE_annotation)) {
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
*    Children_childrenAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Children_childrenAsnWrite(Children_childrenPtr anp, AsnIoPtr aip, AsnTypePtr orig)

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

   atp = AsnLinkType(orig, SEQTREE_NODE_children);   /* link local tree */
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
   case Children_children_children:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqTreeNodeAsnWrite, aip, SEQTREE_NODE_children_children, SEQTREE_NODE_children_children_E);
      break;
   case Children_children_Children_Footprint:
      writetype = SEQTREE_NODE_children_footprint;
      func = (AsnWriteFunc) Children_footprintAsnWrite;
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
*    Children_footprintAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Children_footprintAsnWrite(Children_footprintPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, SEQTREE_NODE_children_footprint);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> seqRange != NULL) {
      if ( ! SeqIntAsnWrite(ptr -> seqRange, aip, SEQTREE_NODE_children_footprint_seqRange)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> rowId;
   retval = AsnWrite(aip, SEQTREE_NODE_children_footprint_rowId,  &av);
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
*    NodeAnnotationNew()
*
**************************************************/
NLM_EXTERN 
NodeAnnotationPtr LIBCALL
NodeAnnotationNew(void)
{
   NodeAnnotationPtr ptr = MemNew((size_t) sizeof(NodeAnnotation));

   return ptr;

}


/**************************************************
*
*    NodeAnnotationFree()
*
**************************************************/
NLM_EXTERN 
NodeAnnotationPtr LIBCALL
NodeAnnotationFree(NodeAnnotationPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> presentInChildCD);
   MemFree(ptr -> note);
   return MemFree(ptr);
}


/**************************************************
*
*    NodeAnnotationAsnRead()
*
**************************************************/
NLM_EXTERN 
NodeAnnotationPtr LIBCALL
NodeAnnotationAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   NodeAnnotationPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* NodeAnnotation ::= (self contained) */
      atp = AsnReadId(aip, amp, NODE_ANNOTATION);
   } else {
      atp = AsnLinkType(orig, NODE_ANNOTATION);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = NodeAnnotationNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == NODE_ANNOTATION_presentInChildCD) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> presentInChildCD = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == NODE_ANNOTATION_note) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> note = av.ptrvalue;
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
   ptr = NodeAnnotationFree(ptr);
   goto ret;
}



/**************************************************
*
*    NodeAnnotationAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
NodeAnnotationAsnWrite(NodeAnnotationPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, NODE_ANNOTATION);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> presentInChildCD != NULL) {
      av.ptrvalue = ptr -> presentInChildCD;
      retval = AsnWrite(aip, NODE_ANNOTATION_presentInChildCD,  &av);
   }
   if (ptr -> note != NULL) {
      av.ptrvalue = ptr -> note;
      retval = AsnWrite(aip, NODE_ANNOTATION_note,  &av);
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
*    CddViewerRectNew()
*
**************************************************/
NLM_EXTERN 
CddViewerRectPtr LIBCALL
CddViewerRectNew(void)
{
   CddViewerRectPtr ptr = MemNew((size_t) sizeof(CddViewerRect));

   return ptr;

}


/**************************************************
*
*    CddViewerRectFree()
*
**************************************************/
NLM_EXTERN 
CddViewerRectPtr LIBCALL
CddViewerRectFree(CddViewerRectPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    CddViewerRectAsnRead()
*
**************************************************/
NLM_EXTERN 
CddViewerRectPtr LIBCALL
CddViewerRectAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddViewerRectPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddViewerRect ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_VIEWER_RECT);
   } else {
      atp = AsnLinkType(orig, CDD_VIEWER_RECT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CddViewerRectNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CDD_VIEWER_RECT_top) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> top = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_VIEWER_RECT_left) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> left = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_VIEWER_RECT_width) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> width = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_VIEWER_RECT_height) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> height = av.intvalue;
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
   ptr = CddViewerRectFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddViewerRectAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddViewerRectAsnWrite(CddViewerRectPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, CDD_VIEWER_RECT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> top;
   retval = AsnWrite(aip, CDD_VIEWER_RECT_top,  &av);
   av.intvalue = ptr -> left;
   retval = AsnWrite(aip, CDD_VIEWER_RECT_left,  &av);
   av.intvalue = ptr -> width;
   retval = AsnWrite(aip, CDD_VIEWER_RECT_width,  &av);
   av.intvalue = ptr -> height;
   retval = AsnWrite(aip, CDD_VIEWER_RECT_height,  &av);
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
*    CddViewerNew()
*
**************************************************/
NLM_EXTERN 
CddViewerPtr LIBCALL
CddViewerNew(void)
{
   CddViewerPtr ptr = MemNew((size_t) sizeof(CddViewer));

   return ptr;

}


/**************************************************
*
*    CddViewerFree()
*
**************************************************/
NLM_EXTERN 
CddViewerPtr LIBCALL
CddViewerFree(CddViewerPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   CddViewerRectFree(ptr -> rect);
   AsnGenericBaseSeqOfFree(ptr -> accessions ,ASNCODE_PTRVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    CddViewerAsnRead()
*
**************************************************/
NLM_EXTERN 
CddViewerPtr LIBCALL
CddViewerAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddViewerPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddViewer ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_VIEWER);
   } else {
      atp = AsnLinkType(orig, CDD_VIEWER);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CddViewerNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CDD_VIEWER_ctrl) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ctrl = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_VIEWER_rect) {
      ptr -> rect = CddViewerRectAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_VIEWER_accessions) {
      ptr -> accessions = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> accessions == NULL) {
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
   ptr = CddViewerFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddViewerAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddViewerAsnWrite(CddViewerPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, CDD_VIEWER);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> ctrl;
   retval = AsnWrite(aip, CDD_VIEWER_ctrl,  &av);
   if (ptr -> rect != NULL) {
      if ( ! CddViewerRectAsnWrite(ptr -> rect, aip, CDD_VIEWER_rect)) {
         goto erret;
      }
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> accessions ,ASNCODE_PTRVAL_SLOT, aip, CDD_VIEWER_accessions, CDD_VIEWER_accessions_E);
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
*    CddScriptNew()
*
**************************************************/
NLM_EXTERN 
CddScriptPtr LIBCALL
CddScriptNew(void)
{
   CddScriptPtr ptr = MemNew((size_t) sizeof(CddScript));

   return ptr;

}


/**************************************************
*
*    CddScriptFree()
*
**************************************************/
NLM_EXTERN 
CddScriptPtr LIBCALL
CddScriptFree(CddScriptPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   MemFree(ptr -> commands);
   return MemFree(ptr);
}


/**************************************************
*
*    CddScriptAsnRead()
*
**************************************************/
NLM_EXTERN 
CddScriptPtr LIBCALL
CddScriptAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   CddScriptPtr ptr;

   if (! loaded)
   {
      if (! objcddAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* CddScript ::= (self contained) */
      atp = AsnReadId(aip, amp, CDD_SCRIPT);
   } else {
      atp = AsnLinkType(orig, CDD_SCRIPT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = CddScriptNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CDD_SCRIPT_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_SCRIPT_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CDD_SCRIPT_commands) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> commands = av.ptrvalue;
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
   ptr = CddScriptFree(ptr);
   goto ret;
}



/**************************************************
*
*    CddScriptAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
CddScriptAsnWrite(CddScriptPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
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

   atp = AsnLinkType(orig, CDD_SCRIPT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, CDD_SCRIPT_type,  &av);
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, CDD_SCRIPT_name,  &av);
   }
   if (ptr -> commands != NULL) {
      av.ptrvalue = ptr -> commands;
      retval = AsnWrite(aip, CDD_SCRIPT_commands,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

