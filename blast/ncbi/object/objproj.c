#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <mapproj.h>
#include <objproj.h>

static Boolean loaded = FALSE;

#include <asnproj.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

static Pointer LIBCALLBACK ProjectNewFunc (void)
{
   return (Pointer) ProjectNew();
}

static Pointer LIBCALLBACK ProjectFreeFunc (Pointer data)
{
   return (Pointer) ProjectFree ((ProjectPtr) data);
}

static Boolean LIBCALLBACK ProjectAsnWriteFunc (Pointer data, AsnIoPtr aip, AsnTypePtr atp)
{
   return ProjectAsnWrite ((ProjectPtr) data, aip, atp);
}

static Pointer LIBCALLBACK ProjectAsnReadFunc (AsnIoPtr aip, AsnTypePtr atp)
{
   return (Pointer) ProjectAsnRead (aip, atp);
}

static Int2 LIBCALLBACK ProjectLabelFunc (Pointer data, CharPtr buffer, Int2 buflen, Uint1 content)
{
   return ProjectLabel ((ProjectPtr) data, buffer, buflen, content);
}

NLM_EXTERN Boolean LIBCALL
objprojAsnLoad(void)
{

   if ( ! loaded) {
      NLM_EXTERN_LOADS

      if ( ! AsnLoad ())
      return FALSE;
      loaded = TRUE;
   }

   ObjMgrTypeLoad (OBJ_PROJECT, "Project", "Project", "NCBI Project",
   PROJECT, ProjectNewFunc, ProjectAsnReadFunc, ProjectAsnWriteFunc,
   ProjectFreeFunc, ProjectLabelFunc, ProjectSubTypeFunc);

   return TRUE;
}



/**************************************************
*    Generated object loaders for Module NCBI-Project
*    Generated using ASNCODE Revision: 6.1 at Mar 25, 1998  6:32 PM
*
**************************************************/


/**************************************************
*
*    ProjectNew()
*
**************************************************/
NLM_EXTERN 
ProjectPtr LIBCALL
ProjectNew(void)
{
   ProjectPtr ptr = MemNew((size_t) sizeof(Project));

   return ptr;

}


/**************************************************
*
*    ProjectFree()
*
**************************************************/
NLM_EXTERN 
ProjectPtr LIBCALL
ProjectFree(ProjectPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   ProjectDescrFree(ptr -> descr);
   ProjectItemFree(ptr -> data);
   return MemFree(ptr);
}


/**************************************************
*
*    ProjectAsnRead()
*
**************************************************/
NLM_EXTERN 
ProjectPtr LIBCALL
ProjectAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ProjectPtr ptr;

   if (! loaded)
   {
      if (! objprojAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Project ::= (self contained) */
      atp = AsnReadId(aip, amp, PROJECT);
   } else {
      atp = AsnLinkType(orig, PROJECT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ProjectNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == PROJECT_descr) {
      ptr -> descr = ProjectDescrAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PROJECT_data) {
      ptr -> data = ProjectItemAsnRead(aip, atp);
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
   ptr = ProjectFree(ptr);
   goto ret;
}



/**************************************************
*
*    ProjectAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ProjectAsnWrite(ProjectPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objprojAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, PROJECT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> descr != NULL) {
      if ( ! ProjectDescrAsnWrite(ptr -> descr, aip, PROJECT_descr)) {
         goto erret;
      }
   }
   if (ptr -> data != NULL) {
      if ( ! ProjectItemAsnWrite(ptr -> data, aip, PROJECT_data)) {
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
*    ProjectItemFree()
*
**************************************************/
NLM_EXTERN 
ProjectItemPtr LIBCALL
ProjectItemFree(ValNodePtr anp)
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
   case ProjectItem_pmuid:
      AsnGenericBaseSeqOfFree((ValNodePtr) pnt,ASNCODE_INTVAL_SLOT);
      break;
   case ProjectItem_protuid:
      AsnGenericBaseSeqOfFree((ValNodePtr) pnt,ASNCODE_INTVAL_SLOT);
      break;
   case ProjectItem_nucuid:
      AsnGenericBaseSeqOfFree((ValNodePtr) pnt,ASNCODE_INTVAL_SLOT);
      break;
   case ProjectItem_sequid:
      AsnGenericBaseSeqOfFree((ValNodePtr) pnt,ASNCODE_INTVAL_SLOT);
      break;
   case ProjectItem_genomeuid:
      AsnGenericBaseSeqOfFree((ValNodePtr) pnt,ASNCODE_INTVAL_SLOT);
      break;
   case ProjectItem_structuid:
      AsnGenericBaseSeqOfFree((ValNodePtr) pnt,ASNCODE_INTVAL_SLOT);
      break;
   case ProjectItem_pmid:
      AsnGenericBaseSeqOfFree((ValNodePtr) pnt,ASNCODE_INTVAL_SLOT);
      break;
   case ProjectItem_protid:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqIdFree);
      break;
   case ProjectItem_nucid:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqIdFree);
      break;
   case ProjectItem_seqid:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqIdFree);
      break;
   case ProjectItem_genomeid:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqIdFree);
      break;
   case ProjectItem_pment:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) PubmedEntryFree);
      break;
   case ProjectItem_protent:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqEntryFree);
      break;
   case ProjectItem_nucent:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqEntryFree);
      break;
   case ProjectItem_seqent:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqEntryFree);
      break;
   case ProjectItem_genomeent:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqEntryFree);
      break;
   case ProjectItem_seqannot:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqAnnotFree);
      break;
   case ProjectItem_loc:
      AsnGenericChoiceSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) SeqLocFree);
      break;
   case ProjectItem_proj:
      AsnGenericUserSeqOfFree((Pointer) pnt, (AsnOptFreeFunc) ProjectFree);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ProjectItemAsnRead()
*
**************************************************/
NLM_EXTERN 
ProjectItemPtr LIBCALL
ProjectItemAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objprojAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ProjectItem ::= (self contained) */
      atp = AsnReadId(aip, amp, PROJECT_ITEM);
   } else {
      atp = AsnLinkType(orig, PROJECT_ITEM);    /* link in local tree */
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
   if (atp == PROJECT_ITEM_pmuid) {
      choice = ProjectItem_pmuid;
      anp -> data.ptrvalue = 
      AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_protuid) {
      choice = ProjectItem_protuid;
      anp -> data.ptrvalue = 
      AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_nucuid) {
      choice = ProjectItem_nucuid;
      anp -> data.ptrvalue = 
      AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_sequid) {
      choice = ProjectItem_sequid;
      anp -> data.ptrvalue = 
      AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_genomeuid) {
      choice = ProjectItem_genomeuid;
      anp -> data.ptrvalue = 
      AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_structuid) {
      choice = ProjectItem_structuid;
      anp -> data.ptrvalue = 
      AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_pmid) {
      choice = ProjectItem_pmid;
      anp -> data.ptrvalue = 
      AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_INTVAL_SLOT, &isError);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_protid) {
      choice = ProjectItem_protid;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqIdAsnRead,             (AsnOptFreeFunc) SeqIdFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_nucid) {
      choice = ProjectItem_nucid;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqIdAsnRead,             (AsnOptFreeFunc) SeqIdFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_seqid) {
      choice = ProjectItem_seqid;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqIdAsnRead,             (AsnOptFreeFunc) SeqIdFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_genomeid) {
      choice = ProjectItem_genomeid;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqIdAsnRead,             (AsnOptFreeFunc) SeqIdFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_structid) {
      choice = ProjectItem_structid;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == PROJECT_ITEM_pment) {
      choice = ProjectItem_pment;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) PubmedEntryAsnRead,             (AsnOptFreeFunc) PubmedEntryFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_protent) {
      choice = ProjectItem_protent;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqEntryAsnRead,             (AsnOptFreeFunc) SeqEntryFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_nucent) {
      choice = ProjectItem_nucent;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqEntryAsnRead,             (AsnOptFreeFunc) SeqEntryFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_seqent) {
      choice = ProjectItem_seqent;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqEntryAsnRead,             (AsnOptFreeFunc) SeqEntryFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_genomeent) {
      choice = ProjectItem_genomeent;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqEntryAsnRead,             (AsnOptFreeFunc) SeqEntryFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_structent) {
      choice = ProjectItem_structent;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.boolvalue = av.boolvalue;
   }
   else if (atp == PROJECT_ITEM_seqannot) {
      choice = ProjectItem_seqannot;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqAnnotAsnRead,             (AsnOptFreeFunc) SeqAnnotFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_loc) {
      choice = ProjectItem_loc;
      anp -> data.ptrvalue =
      AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqLocAsnRead,             (AsnOptFreeFunc) SeqLocFree);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == PROJECT_ITEM_proj) {
      choice = ProjectItem_proj;
      anp -> data.ptrvalue =
      AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ProjectAsnRead,             (AsnOptFreeFunc) ProjectFree);
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
*    ProjectItemAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ProjectItemAsnWrite(ProjectItemPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objprojAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, PROJECT_ITEM);   /* link local tree */
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
   case ProjectItem_pmuid:
      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,ASNCODE_INTVAL_SLOT, aip, PROJECT_ITEM_pmuid, PROJECT_ITEM_pmuid_E);            break;
   case ProjectItem_protuid:
      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,ASNCODE_INTVAL_SLOT, aip, PROJECT_ITEM_protuid, PROJECT_ITEM_protuid_E);            break;
   case ProjectItem_nucuid:
      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,ASNCODE_INTVAL_SLOT, aip, PROJECT_ITEM_nucuid, PROJECT_ITEM_nucuid_E);            break;
   case ProjectItem_sequid:
      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,ASNCODE_INTVAL_SLOT, aip, PROJECT_ITEM_sequid, PROJECT_ITEM_sequid_E);            break;
   case ProjectItem_genomeuid:
      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,ASNCODE_INTVAL_SLOT, aip, PROJECT_ITEM_genomeuid, PROJECT_ITEM_genomeuid_E);            break;
   case ProjectItem_structuid:
      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,ASNCODE_INTVAL_SLOT, aip, PROJECT_ITEM_structuid, PROJECT_ITEM_structuid_E);            break;
   case ProjectItem_pmid:
      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,ASNCODE_INTVAL_SLOT, aip, PROJECT_ITEM_pmid, PROJECT_ITEM_pmid_E);            break;
   case ProjectItem_protid:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqIdAsnWrite, aip, PROJECT_ITEM_protid, PROJECT_ITEM_protid_E);
      break;
   case ProjectItem_nucid:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqIdAsnWrite, aip, PROJECT_ITEM_nucid, PROJECT_ITEM_nucid_E);
      break;
   case ProjectItem_seqid:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqIdAsnWrite, aip, PROJECT_ITEM_seqid, PROJECT_ITEM_seqid_E);
      break;
   case ProjectItem_genomeid:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqIdAsnWrite, aip, PROJECT_ITEM_genomeid, PROJECT_ITEM_genomeid_E);
      break;
   case ProjectItem_structid:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, PROJECT_ITEM_structid, &av);
      break;
   case ProjectItem_pment:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) PubmedEntryAsnWrite, aip, PROJECT_ITEM_pment, PROJECT_ITEM_pment_E);
      break;
   case ProjectItem_protent:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqEntryAsnWrite, aip, PROJECT_ITEM_protent, PROJECT_ITEM_protent_E);
      break;
   case ProjectItem_nucent:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqEntryAsnWrite, aip, PROJECT_ITEM_nucent, PROJECT_ITEM_nucent_E);
      break;
   case ProjectItem_seqent:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqEntryAsnWrite, aip, PROJECT_ITEM_seqent, PROJECT_ITEM_seqent_E);
      break;
   case ProjectItem_genomeent:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqEntryAsnWrite, aip, PROJECT_ITEM_genomeent, PROJECT_ITEM_genomeent_E);
      break;
   case ProjectItem_structent:
      av.boolvalue = anp->data.boolvalue;
      retval = AsnWrite(aip, PROJECT_ITEM_structent, &av);
      break;
   case ProjectItem_seqannot:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqAnnotAsnWrite, aip, PROJECT_ITEM_seqannot, PROJECT_ITEM_seqannot_E);
      break;
   case ProjectItem_loc:
      retval = AsnGenericChoiceSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) SeqLocAsnWrite, aip, PROJECT_ITEM_loc, PROJECT_ITEM_loc_E);
      break;
   case ProjectItem_proj:
      retval = AsnGenericUserSeqOfAsnWrite((Pointer) pnt, (AsnWriteFunc) ProjectAsnWrite, aip, PROJECT_ITEM_proj, PROJECT_ITEM_proj_E);
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
*    ProjectDescrNew()
*
**************************************************/
NLM_EXTERN 
ProjectDescrPtr LIBCALL
ProjectDescrNew(void)
{
   ProjectDescrPtr ptr = MemNew((size_t) sizeof(ProjectDescr));

   return ptr;

}


/**************************************************
*
*    ProjectDescrFree()
*
**************************************************/
NLM_EXTERN 
ProjectDescrPtr LIBCALL
ProjectDescrFree(ProjectDescrPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericBaseSeqOfFree(ptr -> id ,ASNCODE_PTRVAL_SLOT);
   MemFree(ptr -> name);
   AsnGenericChoiceSeqOfFree(ptr -> descr, (AsnOptFreeFunc) ProjdescFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ProjectDescrAsnRead()
*
**************************************************/
NLM_EXTERN 
ProjectDescrPtr LIBCALL
ProjectDescrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ProjectDescrPtr ptr;

   if (! loaded)
   {
      if (! objprojAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ProjectDescr ::= (self contained) */
      atp = AsnReadId(aip, amp, PROJECT_DESCR);
   } else {
      atp = AsnLinkType(orig, PROJECT_DESCR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ProjectDescrNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == PROJECT_DESCR_id) {
      ptr -> id = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> id == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PROJECT_DESCR_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == PROJECT_DESCR_descr) {
      ptr -> descr = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ProjdescAsnRead, (AsnOptFreeFunc) ProjdescFree);
      if (isError && ptr -> descr == NULL) {
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
   ptr = ProjectDescrFree(ptr);
   goto ret;
}



/**************************************************
*
*    ProjectDescrAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ProjectDescrAsnWrite(ProjectDescrPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objprojAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, PROJECT_DESCR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> id ,ASNCODE_PTRVAL_SLOT, aip, PROJECT_DESCR_id, PROJECT_DESCR_id_E);
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, PROJECT_DESCR_name,  &av);
   }
   AsnGenericChoiceSeqOfAsnWrite(ptr -> descr, (AsnWriteFunc) ProjdescAsnWrite, aip, PROJECT_DESCR_descr, PROJECT_DESCR_descr_E);
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
*    ProjdescFree()
*
**************************************************/
NLM_EXTERN 
ProjdescPtr LIBCALL
ProjdescFree(ValNodePtr anp)
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
   case Projdesc_pub:
      PubdescFree(anp -> data.ptrvalue);
      break;
   case Projdesc_date:
      DateFree(anp -> data.ptrvalue);
      break;
   case Projdesc_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case Projdesc_title:
      MemFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ProjdescAsnRead()
*
**************************************************/
NLM_EXTERN 
ProjdescPtr LIBCALL
ProjdescAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objprojAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Projdesc ::= (self contained) */
      atp = AsnReadId(aip, amp, PROJDESC);
   } else {
      atp = AsnLinkType(orig, PROJDESC);    /* link in local tree */
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
   if (atp == PROJDESC_pub) {
      choice = Projdesc_pub;
      func = (AsnReadFunc) PubdescAsnRead;
   }
   else if (atp == PROJDESC_date) {
      choice = Projdesc_date;
      func = (AsnReadFunc) DateAsnRead;
   }
   else if (atp == PROJDESC_comment) {
      choice = Projdesc_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == PROJDESC_title) {
      choice = Projdesc_title;
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
*    ProjdescAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
ProjdescAsnWrite(ProjdescPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objprojAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, PROJDESC);   /* link local tree */
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
   case Projdesc_pub:
      writetype = PROJDESC_pub;
      func = (AsnWriteFunc) PubdescAsnWrite;
      break;
   case Projdesc_date:
      writetype = PROJDESC_date;
      func = (AsnWriteFunc) DateAsnWrite;
      break;
   case Projdesc_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, PROJDESC_comment, &av);
      break;
   case Projdesc_title:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, PROJDESC_title, &av);
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
