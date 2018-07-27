#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <strimprt.h>
#include <objmmdb1.h>

static Boolean loaded = FALSE;

#include <mmdb1.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

Boolean LIBCALL
objmmdb1AsnLoad(void)
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
*    Generated object loaders for Module MMDB-Chemical-graph
*    Generated using ASNCODE Revision: 4.2 at Aug 1, 1996  4:27 PM
*
**************************************************/


/**************************************************
*
*    BiostrucNew()
*
**************************************************/

BiostrucPtr LIBCALL
BiostrucNew(void)
{
   BiostrucPtr ptr = MemNew((size_t) sizeof(Biostruc));

   return ptr;

}


/**************************************************
*
*    BiostrucFree()
*
**************************************************/

BiostrucPtr LIBCALL
BiostrucFree(BiostrucPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> id, (AsnOptFreeFunc) BiostrucIdFree);
   AsnGenericChoiceSeqOfFree(ptr -> descr, (AsnOptFreeFunc) BiostrucDescrFree);
   BiostrucGraphFree(ptr -> chemical_graph);
   AsnGenericUserSeqOfFree(ptr -> features, (AsnOptFreeFunc) BiostrucFeatureSetFree);
   AsnGenericUserSeqOfFree(ptr -> model, (AsnOptFreeFunc) BiostrucModelFree);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucAsnRead()
*
**************************************************/

BiostrucPtr LIBCALL
BiostrucAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Biostruc ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_id) {
      ptr -> id = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucIdAsnRead, (AsnOptFreeFunc) BiostrucIdFree);
      if (isError && ptr -> id == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_descr) {
      ptr -> descr = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucDescrAsnRead, (AsnOptFreeFunc) BiostrucDescrFree);
      if (isError && ptr -> descr == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_chemical_graph) {
      ptr -> chemical_graph = BiostrucGraphAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_features) {
      ptr -> features = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucFeatureSetAsnRead, (AsnOptFreeFunc) BiostrucFeatureSetFree);
      if (isError && ptr -> features == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_model) {
      /* begin manual insertion */
      AsnOptionPtr aop;
      int option = -1;

      if ((aop = AsnIoOptionGet(aip, OP_NCBIOBJSTR, BIOSTRUC_CHECK_MODEL, NULL)) != NULL)
      {
         AsnTypePtr start_atp;
         BiostrucModelPtr model;
         BiostrucModelPtr last = NULL;
         Int4 maxModels = INT2_MAX;
         Boolean retain;
         Boolean quitAfterRetainingNext = FALSE;
         Boolean quitNow = FALSE;

         option = aop->data.intvalue;
         if ((aop = AsnIoOptionGet(aip, OP_NCBIOBJSTR, BIOSTRUC_MAX_MODELS, NULL)) != NULL)
         {
             maxModels = aop->data.intvalue;
         }

         if (option == ONECOORDATOM || option == ONECOORDRES ||
             option == ALLMDL || option == VECMODEL)
         {
            if (AsnReadVal (aip, atp, &av) <= 0) /* START_STRUCT */
               goto erret;
      
            start_atp = atp;
       
            while ((atp = AsnReadId (aip, amp, atp)) != start_atp) {

               if ( ! ProgMon("caller of BiostrucModelAsnRead") )
               {
                   return NULL;
               }

               model = BiostrucModelAsnRead(aip, atp);
               if (model == NULL)
                  break;

               retain = FALSE;
               quitAfterRetainingNext = TRUE;
               switch(option) {
               case VECMODEL:
                   retain = model->type == 1;
                   break;
               case ONECOORDRES:
                   retain = model->type == 2;
                   break;
               case ALLMDL:
                   retain = model->type == 4 && maxModels-- > 0;
                   quitAfterRetainingNext = maxModels <= 0;
                   break;
               case ONECOORDATOM: 
               default:
                     retain = model->type == 3;
                   break;
               }

               if (quitNow)
                   break;

               if (retain)
               {
                  if (ptr->model == NULL)
                  {
                     ptr->model = model;
                     last = model;
                  } else { /* insert at the end */
                     last->next = model;
                     last = model;
                  }
                  if (quitAfterRetainingNext)
                    break;

               }
               else {  /* fix - if didn't retain model, free it */
                 BiostrucModelFree(model);
               }
            }
            return ptr;
         }
      }
      /* end manual insertion */
      ptr -> model = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucModelAsnRead, (AsnOptFreeFunc) BiostrucModelFree);
      if (isError && ptr -> model == NULL) {
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
   ptr = BiostrucFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucAsnWrite(BiostrucPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericChoiceSeqOfAsnWrite(ptr -> id, (AsnWriteFunc) BiostrucIdAsnWrite, aip, BIOSTRUC_id, BIOSTRUC_id_E);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> descr, (AsnWriteFunc) BiostrucDescrAsnWrite, aip, BIOSTRUC_descr, BIOSTRUC_descr_E);
   if (ptr -> chemical_graph != NULL) {
      if ( ! BiostrucGraphAsnWrite(ptr -> chemical_graph, aip, BIOSTRUC_chemical_graph)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> features, (AsnWriteFunc) BiostrucFeatureSetAsnWrite, aip, BIOSTRUC_features, BIOSTRUC_features_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> model, (AsnWriteFunc) BiostrucModelAsnWrite, aip, BIOSTRUC_model, BIOSTRUC_model_E);
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
*    BiostrucIdFree()
*
**************************************************/

BiostrucIdPtr LIBCALL
BiostrucIdFree(ValNodePtr anp)
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
   case BiostrucId_other_database:
      DbtagFree(anp -> data.ptrvalue);
      break;
   case BiostrucId_local_id:
      ObjectIdFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    BiostrucIdAsnRead()
*
**************************************************/

BiostrucIdPtr LIBCALL
BiostrucIdAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucId ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_ID);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_ID);    /* link in local tree */
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
   if (atp == BIOSTRUC_ID_mmdb_id) {
      choice = BiostrucId_mmdb_id;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == BIOSTRUC_ID_other_database) {
      choice = BiostrucId_other_database;
      func = (AsnReadFunc) DbtagAsnRead;
   }
   else if (atp == BIOSTRUC_ID_local_id) {
      choice = BiostrucId_local_id;
      func = (AsnReadFunc) ObjectIdAsnRead;
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
*    BiostrucIdAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucIdAsnWrite(BiostrucIdPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, BIOSTRUC_ID);   /* link local tree */
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
   case BiostrucId_mmdb_id:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, BIOSTRUC_ID_mmdb_id, &av);
      break;
   case BiostrucId_other_database:
      writetype = BIOSTRUC_ID_other_database;
      func = (AsnWriteFunc) DbtagAsnWrite;
      break;
   case BiostrucId_local_id:
      writetype = BIOSTRUC_ID_local_id;
      func = (AsnWriteFunc) ObjectIdAsnWrite;
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
*    BiostrucSetNew()
*
**************************************************/

BiostrucSetPtr LIBCALL
BiostrucSetNew(void)
{
   BiostrucSetPtr ptr = MemNew((size_t) sizeof(BiostrucSet));

   return ptr;

}


/**************************************************
*
*    BiostrucSetFree()
*
**************************************************/

BiostrucSetPtr LIBCALL
BiostrucSetFree(BiostrucSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> id, (AsnOptFreeFunc) BiostrucIdFree);
   AsnGenericChoiceSeqOfFree(ptr -> descr, (AsnOptFreeFunc) BiostrucDescrFree);
   AsnGenericUserSeqOfFree(ptr -> biostrucs, (AsnOptFreeFunc) BiostrucFree);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucSetAsnRead()
*
**************************************************/

BiostrucSetPtr LIBCALL
BiostrucSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucSetPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucSet ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_SET);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucSetNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_SET_id) {
      ptr -> id = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucIdAsnRead, (AsnOptFreeFunc) BiostrucIdFree);
      if (isError && ptr -> id == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SET_descr) {
      ptr -> descr = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucDescrAsnRead, (AsnOptFreeFunc) BiostrucDescrFree);
      if (isError && ptr -> descr == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SET_biostrucs) {
      ptr -> biostrucs = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucAsnRead, (AsnOptFreeFunc) BiostrucFree);
      if (isError && ptr -> biostrucs == NULL) {
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
   ptr = BiostrucSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucSetAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucSetAsnWrite(BiostrucSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericChoiceSeqOfAsnWrite(ptr -> id, (AsnWriteFunc) BiostrucIdAsnWrite, aip, BIOSTRUC_SET_id, BIOSTRUC_SET_id_E);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> descr, (AsnWriteFunc) BiostrucDescrAsnWrite, aip, BIOSTRUC_SET_descr, BIOSTRUC_SET_descr_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> biostrucs, (AsnWriteFunc) BiostrucAsnWrite, aip, BIOSTRUC_SET_biostrucs, BIOSTRUC_SET_biostrucs_E);
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
*    BiostrucAnnotSetNew()
*
**************************************************/

BiostrucAnnotSetPtr LIBCALL
BiostrucAnnotSetNew(void)
{
   BiostrucAnnotSetPtr ptr = MemNew((size_t) sizeof(BiostrucAnnotSet));

   return ptr;

}


/**************************************************
*
*    BiostrucAnnotSetFree()
*
**************************************************/

BiostrucAnnotSetPtr LIBCALL
BiostrucAnnotSetFree(BiostrucAnnotSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> id, (AsnOptFreeFunc) BiostrucIdFree);
   AsnGenericChoiceSeqOfFree(ptr -> descr, (AsnOptFreeFunc) BiostrucDescrFree);
   AsnGenericUserSeqOfFree(ptr -> features, (AsnOptFreeFunc) BiostrucFeatureSetFree);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucAnnotSetAsnRead()
*
**************************************************/

BiostrucAnnotSetPtr LIBCALL
BiostrucAnnotSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucAnnotSetPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucAnnotSet ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_ANNOT_SET);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_ANNOT_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucAnnotSetNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_ANNOT_SET_id) {
      ptr -> id = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucIdAsnRead, (AsnOptFreeFunc) BiostrucIdFree);
      if (isError && ptr -> id == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_ANNOT_SET_descr) {
      ptr -> descr = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucDescrAsnRead, (AsnOptFreeFunc) BiostrucDescrFree);
      if (isError && ptr -> descr == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_ANNOT_SET_features) {
      ptr -> features = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucFeatureSetAsnRead, (AsnOptFreeFunc) BiostrucFeatureSetFree);
      if (isError && ptr -> features == NULL) {
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
   ptr = BiostrucAnnotSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucAnnotSetAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucAnnotSetAsnWrite(BiostrucAnnotSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_ANNOT_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericChoiceSeqOfAsnWrite(ptr -> id, (AsnWriteFunc) BiostrucIdAsnWrite, aip, BIOSTRUC_ANNOT_SET_id, BIOSTRUC_ANNOT_SET_id_E);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> descr, (AsnWriteFunc) BiostrucDescrAsnWrite, aip, BIOSTRUC_ANNOT_SET_descr, BIOSTRUC_ANNOT_SET_descr_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> features, (AsnWriteFunc) BiostrucFeatureSetAsnWrite, aip, BIOSTRUC_ANNOT_SET_features, BIOSTRUC_ANNOT_SET_features_E);
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
*    BiostrucResidueGraphSetNew()
*
**************************************************/

BiostrucResidueGraphSetPtr LIBCALL
BiostrucResidueGraphSetNew(void)
{
   BiostrucResidueGraphSetPtr ptr = MemNew((size_t) sizeof(BiostrucResidueGraphSet));

   return ptr;

}


/**************************************************
*
*    BiostrucResidueGraphSetFree()
*
**************************************************/

BiostrucResidueGraphSetPtr LIBCALL
BiostrucResidueGraphSetFree(BiostrucResidueGraphSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> id, (AsnOptFreeFunc) BiostrucIdFree);
   AsnGenericChoiceSeqOfFree(ptr -> descr, (AsnOptFreeFunc) BiomolDescrFree);
   AsnGenericUserSeqOfFree(ptr -> residue_graphs, (AsnOptFreeFunc) ResidueGraphFree);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucResidueGraphSetAsnRead()
*
**************************************************/

BiostrucResidueGraphSetPtr LIBCALL
BiostrucResidueGraphSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucResidueGraphSetPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucResidueGraphSet ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_RESIDUE_GRAPH_SET);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_RESIDUE_GRAPH_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucResidueGraphSetNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_RESIDUE_GRAPH_SET_id) {
      ptr -> id = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiostrucIdAsnRead, (AsnOptFreeFunc) BiostrucIdFree);
      if (isError && ptr -> id == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_RESIDUE_GRAPH_SET_descr) {
      ptr -> descr = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiomolDescrAsnRead, (AsnOptFreeFunc) BiomolDescrFree);
      if (isError && ptr -> descr == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_RESIDUE_GRAPH_SET_residue_graphs) {
      ptr -> residue_graphs = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ResidueGraphAsnRead, (AsnOptFreeFunc) ResidueGraphFree);
      if (isError && ptr -> residue_graphs == NULL) {
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
   ptr = BiostrucResidueGraphSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucResidueGraphSetAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucResidueGraphSetAsnWrite(BiostrucResidueGraphSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_RESIDUE_GRAPH_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericChoiceSeqOfAsnWrite(ptr -> id, (AsnWriteFunc) BiostrucIdAsnWrite, aip, BIOSTRUC_RESIDUE_GRAPH_SET_id, BIOSTRUC_RESIDUE_GRAPH_SET_id_E);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> descr, (AsnWriteFunc) BiomolDescrAsnWrite, aip, BIOSTRUC_RESIDUE_GRAPH_SET_descr, BIOSTRUC_RESIDUE_GRAPH_SET_descr_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> residue_graphs, (AsnWriteFunc) ResidueGraphAsnWrite, aip, BIOSTRUC_RESIDUE_GRAPH_SET_residue_graphs, BIOSTRUC_RESIDUE_GRAPH_SET_residue_graphs_E);
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
*    BiostrucDescrFree()
*
**************************************************/

BiostrucDescrPtr LIBCALL
BiostrucDescrFree(ValNodePtr anp)
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
   case BiostrucDescr_name:
      MemFree(anp -> data.ptrvalue);
      break;
   case BiostrucDescr_pdb_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case BiostrucDescr_other_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case BiostrucDescr_history:
      BiostrucHistoryFree(anp -> data.ptrvalue);
      break;
   case BiostrucDescr_attribution:
      PubFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    BiostrucDescrAsnRead()
*
**************************************************/

BiostrucDescrPtr LIBCALL
BiostrucDescrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucDescr ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_DESCR);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_DESCR);    /* link in local tree */
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
   if (atp == BIOSTRUC_DESCR_name) {
      choice = BiostrucDescr_name;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BIOSTRUC_DESCR_pdb_comment) {
      choice = BiostrucDescr_pdb_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BIOSTRUC_DESCR_other_comment) {
      choice = BiostrucDescr_other_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BIOSTRUC_DESCR_history) {
      choice = BiostrucDescr_history;
      func = (AsnReadFunc) BiostrucHistoryAsnRead;
   }
   else if (atp == BIOSTRUC_DESCR_attribution) {
      choice = BiostrucDescr_attribution;
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
*    BiostrucDescrAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucDescrAsnWrite(BiostrucDescrPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, BIOSTRUC_DESCR);   /* link local tree */
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
   case BiostrucDescr_name:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOSTRUC_DESCR_name, &av);
      break;
   case BiostrucDescr_pdb_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOSTRUC_DESCR_pdb_comment, &av);
      break;
   case BiostrucDescr_other_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOSTRUC_DESCR_other_comment, &av);
      break;
   case BiostrucDescr_history:
      writetype = BIOSTRUC_DESCR_history;
      func = (AsnWriteFunc) BiostrucHistoryAsnWrite;
      break;
   case BiostrucDescr_attribution:
      writetype = BIOSTRUC_DESCR_attribution;
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
*    BiostrucHistoryNew()
*
**************************************************/

BiostrucHistoryPtr LIBCALL
BiostrucHistoryNew(void)
{
   BiostrucHistoryPtr ptr = MemNew((size_t) sizeof(BiostrucHistory));

   return ptr;

}


/**************************************************
*
*    BiostrucHistoryFree()
*
**************************************************/

BiostrucHistoryPtr LIBCALL
BiostrucHistoryFree(BiostrucHistoryPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   BiostrucReplaceFree(ptr -> replaces);
   BiostrucReplaceFree(ptr -> replaced_by);
   BiostrucSourceFree(ptr -> data_source);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucHistoryAsnRead()
*
**************************************************/

BiostrucHistoryPtr LIBCALL
BiostrucHistoryAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucHistoryPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucHistory ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_HISTORY);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_HISTORY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucHistoryNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_HISTORY_replaces) {
      ptr -> replaces = BiostrucReplaceAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_HISTORY_replaced_by) {
      ptr -> replaced_by = BiostrucReplaceAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_HISTORY_data_source) {
      ptr -> data_source = BiostrucSourceAsnRead(aip, atp);
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
   ptr = BiostrucHistoryFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucHistoryAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucHistoryAsnWrite(BiostrucHistoryPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_HISTORY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> replaces != NULL) {
      if ( ! BiostrucReplaceAsnWrite(ptr -> replaces, aip, BIOSTRUC_HISTORY_replaces)) {
         goto erret;
      }
   }
   if (ptr -> replaced_by != NULL) {
      if ( ! BiostrucReplaceAsnWrite(ptr -> replaced_by, aip, BIOSTRUC_HISTORY_replaced_by)) {
         goto erret;
      }
   }
   if (ptr -> data_source != NULL) {
      if ( ! BiostrucSourceAsnWrite(ptr -> data_source, aip, BIOSTRUC_HISTORY_data_source)) {
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
*    BiostrucReplaceNew()
*
**************************************************/

BiostrucReplacePtr LIBCALL
BiostrucReplaceNew(void)
{
   BiostrucReplacePtr ptr = MemNew((size_t) sizeof(BiostrucReplace));

   return ptr;

}


/**************************************************
*
*    BiostrucReplaceFree()
*
**************************************************/

BiostrucReplacePtr LIBCALL
BiostrucReplaceFree(BiostrucReplacePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   BiostrucIdFree(ptr -> id);
   DateFree(ptr -> date);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucReplaceAsnRead()
*
**************************************************/

BiostrucReplacePtr LIBCALL
BiostrucReplaceAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucReplacePtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucReplace ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_REPLACE);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_REPLACE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucReplaceNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_REPLACE_id) {
      ptr -> id = BiostrucIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_REPLACE_date) {
      ptr -> date = DateAsnRead(aip, atp);
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
   ptr = BiostrucReplaceFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucReplaceAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucReplaceAsnWrite(BiostrucReplacePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_REPLACE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> id != NULL) {
      if ( ! BiostrucIdAsnWrite(ptr -> id, aip, BIOSTRUC_REPLACE_id)) {
         goto erret;
      }
   }
   if (ptr -> date != NULL) {
      if ( ! DateAsnWrite(ptr -> date, aip, BIOSTRUC_REPLACE_date)) {
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
*    BiostrucSourceNew()
*
**************************************************/

BiostrucSourcePtr LIBCALL
BiostrucSourceNew(void)
{
   BiostrucSourcePtr ptr = MemNew((size_t) sizeof(BiostrucSource));

   return ptr;

}


/**************************************************
*
*    BiostrucSourceFree()
*
**************************************************/

BiostrucSourcePtr LIBCALL
BiostrucSourceFree(BiostrucSourcePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name_of_database);
   VersionOfDatabase_version_of_databaseFree(ptr -> VersionOfDatabase_version_of_database);
   BiostrucIdFree(ptr -> database_entry_id);
   DateFree(ptr -> database_entry_date);
   AsnGenericBaseSeqOfFree(ptr -> database_entry_history ,ASNCODE_PTRVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    VersionOfDatabase_version_of_databaseFree()
*
**************************************************/
static 
VersionOfDatabase_version_of_databasePtr LIBCALL
VersionOfDatabase_version_of_databaseFree(ValNodePtr anp)
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
   case VersionOfDatabase_version_of_database_release_date:
      DateFree(anp -> data.ptrvalue);
      break;
   case VersionOfDatabase_version_of_database_release_code:
      MemFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    BiostrucSourceAsnRead()
*
**************************************************/

BiostrucSourcePtr LIBCALL
BiostrucSourceAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucSourcePtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucSource ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_SOURCE);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_SOURCE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucSourceNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_SOURCE_name_of_database) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name_of_database = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SOURCE_version_of_database) {
      ptr -> VersionOfDatabase_version_of_database = VersionOfDatabase_version_of_databaseAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SOURCE_database_entry_id) {
      ptr -> database_entry_id = BiostrucIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SOURCE_database_entry_date) {
      ptr -> database_entry_date = DateAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_SOURCE_database_entry_history) {
      ptr -> database_entry_history = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> database_entry_history == NULL) {
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
   ptr = BiostrucSourceFree(ptr);
   goto ret;
}



/**************************************************
*
*    VersionOfDatabase_version_of_databaseAsnRead()
*
**************************************************/
static 
VersionOfDatabase_version_of_databasePtr LIBCALL
VersionOfDatabase_version_of_databaseAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* VersionOfDatabase_version_of_database ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_SOURCE_version_of_database);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_SOURCE_version_of_database);    /* link in local tree */
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
   if (atp == BIOSTRUC_SOURCE_version_of_database_release_date) {
      choice = VersionOfDatabase_version_of_database_release_date;
      func = (AsnReadFunc) DateAsnRead;
   }
   else if (atp == BIOSTRUC_SOURCE_version_of_database_release_code) {
      choice = VersionOfDatabase_version_of_database_release_code;
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
*    BiostrucSourceAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucSourceAsnWrite(BiostrucSourcePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_SOURCE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name_of_database != NULL) {
      av.ptrvalue = ptr -> name_of_database;
      retval = AsnWrite(aip, BIOSTRUC_SOURCE_name_of_database,  &av);
   }
   if (ptr -> VersionOfDatabase_version_of_database != NULL) {
      if ( ! VersionOfDatabase_version_of_databaseAsnWrite(ptr -> VersionOfDatabase_version_of_database, aip, BIOSTRUC_SOURCE_version_of_database)) {
         goto erret;
      }
   }
   if (ptr -> database_entry_id != NULL) {
      if ( ! BiostrucIdAsnWrite(ptr -> database_entry_id, aip, BIOSTRUC_SOURCE_database_entry_id)) {
         goto erret;
      }
   }
   if (ptr -> database_entry_date != NULL) {
      if ( ! DateAsnWrite(ptr -> database_entry_date, aip, BIOSTRUC_SOURCE_database_entry_date)) {
         goto erret;
      }
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> database_entry_history ,ASNCODE_PTRVAL_SLOT, aip, BIOSTRUC_SOURCE_database_entry_history, BIOSTRUC_SOURCE_database_entry_history_E);
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
*    VersionOfDatabase_version_of_databaseAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
VersionOfDatabase_version_of_databaseAsnWrite(VersionOfDatabase_version_of_databasePtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, BIOSTRUC_SOURCE_version_of_database);   /* link local tree */
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
   case VersionOfDatabase_version_of_database_release_date:
      writetype = BIOSTRUC_SOURCE_version_of_database_release_date;
      func = (AsnWriteFunc) DateAsnWrite;
      break;
   case VersionOfDatabase_version_of_database_release_code:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOSTRUC_SOURCE_version_of_database_release_code, &av);
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
*    BiostrucGraphNew()
*
**************************************************/

BiostrucGraphPtr LIBCALL
BiostrucGraphNew(void)
{
   BiostrucGraphPtr ptr = MemNew((size_t) sizeof(BiostrucGraph));

   return ptr;

}


/**************************************************
*
*    BiostrucGraphFree()
*
**************************************************/

BiostrucGraphPtr LIBCALL
BiostrucGraphFree(BiostrucGraphPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> descr, (AsnOptFreeFunc) BiomolDescrFree);
   AsnGenericUserSeqOfFree(ptr -> molecule_graphs, (AsnOptFreeFunc) MoleculeGraphFree);
   AsnGenericUserSeqOfFree(ptr -> inter_molecule_bonds, (AsnOptFreeFunc) InterResidueBondFree);
   AsnGenericUserSeqOfFree(ptr -> residue_graphs, (AsnOptFreeFunc) ResidueGraphFree);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucGraphAsnRead()
*
**************************************************/

BiostrucGraphPtr LIBCALL
BiostrucGraphAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucGraphPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucGraph ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_GRAPH);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_GRAPH);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucGraphNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_GRAPH_descr) {
      ptr -> descr = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiomolDescrAsnRead, (AsnOptFreeFunc) BiomolDescrFree);
      if (isError && ptr -> descr == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_GRAPH_molecule_graphs) {
      ptr -> molecule_graphs = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MoleculeGraphAsnRead, (AsnOptFreeFunc) MoleculeGraphFree);
      if (isError && ptr -> molecule_graphs == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_GRAPH_inter_molecule_bonds) {
      ptr -> inter_molecule_bonds = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) InterResidueBondAsnRead, (AsnOptFreeFunc) InterResidueBondFree);
      if (isError && ptr -> inter_molecule_bonds == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_GRAPH_residue_graphs) {
      ptr -> residue_graphs = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ResidueGraphAsnRead, (AsnOptFreeFunc) ResidueGraphFree);
      if (isError && ptr -> residue_graphs == NULL) {
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
   ptr = BiostrucGraphFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucGraphAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucGraphAsnWrite(BiostrucGraphPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_GRAPH);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   AsnGenericChoiceSeqOfAsnWrite(ptr -> descr, (AsnWriteFunc) BiomolDescrAsnWrite, aip, BIOSTRUC_GRAPH_descr, BIOSTRUC_GRAPH_descr_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> molecule_graphs, (AsnWriteFunc) MoleculeGraphAsnWrite, aip, BIOSTRUC_GRAPH_molecule_graphs, BIOSTRUC_GRAPH_molecule_graphs_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> inter_molecule_bonds, (AsnWriteFunc) InterResidueBondAsnWrite, aip, BIOSTRUC_GRAPH_inter_molecule_bonds, BIOSTRUC_GRAPH_inter_molecule_bonds_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> residue_graphs, (AsnWriteFunc) ResidueGraphAsnWrite, aip, BIOSTRUC_GRAPH_residue_graphs, BIOSTRUC_GRAPH_residue_graphs_E);
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
*    BiomolDescrFree()
*
**************************************************/

BiomolDescrPtr LIBCALL
BiomolDescrFree(ValNodePtr anp)
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
   case BiomolDescr_name:
      MemFree(anp -> data.ptrvalue);
      break;
   case BiomolDescr_pdb_class:
      MemFree(anp -> data.ptrvalue);
      break;
   case BiomolDescr_pdb_source:
      MemFree(anp -> data.ptrvalue);
      break;
   case BiomolDescr_pdb_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case BiomolDescr_other_comment:
      MemFree(anp -> data.ptrvalue);
      break;
   case BiomolDescr_organism:
      BioSourceFree(anp -> data.ptrvalue);
      break;
   case BiomolDescr_attribution:
      PubFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    BiomolDescrAsnRead()
*
**************************************************/

BiomolDescrPtr LIBCALL
BiomolDescrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiomolDescr ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOMOL_DESCR);
   } else {
      atp = AsnLinkType(orig, BIOMOL_DESCR);    /* link in local tree */
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
   if (atp == BIOMOL_DESCR_name) {
      choice = BiomolDescr_name;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BIOMOL_DESCR_pdb_class) {
      choice = BiomolDescr_pdb_class;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BIOMOL_DESCR_pdb_source) {
      choice = BiomolDescr_pdb_source;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BIOMOL_DESCR_pdb_comment) {
      choice = BiomolDescr_pdb_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BIOMOL_DESCR_other_comment) {
      choice = BiomolDescr_other_comment;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == BIOMOL_DESCR_organism) {
      choice = BiomolDescr_organism;
      func = (AsnReadFunc) BioSourceAsnRead;
   }
   else if (atp == BIOMOL_DESCR_attribution) {
      choice = BiomolDescr_attribution;
      func = (AsnReadFunc) PubAsnRead;
   }
   else if (atp == BIOMOL_DESCR_assembly_type) {
      choice = BiomolDescr_assembly_type;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == BIOMOL_DESCR_molecule_type) {
      choice = BiomolDescr_molecule_type;
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
*    BiomolDescrAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiomolDescrAsnWrite(BiomolDescrPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, BIOMOL_DESCR);   /* link local tree */
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
   case BiomolDescr_name:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOMOL_DESCR_name, &av);
      break;
   case BiomolDescr_pdb_class:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOMOL_DESCR_pdb_class, &av);
      break;
   case BiomolDescr_pdb_source:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOMOL_DESCR_pdb_source, &av);
      break;
   case BiomolDescr_pdb_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOMOL_DESCR_pdb_comment, &av);
      break;
   case BiomolDescr_other_comment:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, BIOMOL_DESCR_other_comment, &av);
      break;
   case BiomolDescr_organism:
      writetype = BIOMOL_DESCR_organism;
      func = (AsnWriteFunc) BioSourceAsnWrite;
      break;
   case BiomolDescr_attribution:
      writetype = BIOMOL_DESCR_attribution;
      func = (AsnWriteFunc) PubAsnWrite;
      break;
   case BiomolDescr_assembly_type:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, BIOMOL_DESCR_assembly_type, &av);
      break;
   case BiomolDescr_molecule_type:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, BIOMOL_DESCR_molecule_type, &av);
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
*    ResidueGraphNew()
*
**************************************************/

ResidueGraphPtr LIBCALL
ResidueGraphNew(void)
{
   ResidueGraphPtr ptr = MemNew((size_t) sizeof(ResidueGraph));

   return ptr;

}


/**************************************************
*
*    ResidueGraphFree()
*
**************************************************/

ResidueGraphPtr LIBCALL
ResidueGraphFree(ResidueGraphPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> descr, (AsnOptFreeFunc) BiomolDescrFree);
   AsnGenericBaseSeqOfFree(ptr -> iupac_code ,ASNCODE_PTRVAL_SLOT);
   AsnGenericUserSeqOfFree(ptr -> atoms, (AsnOptFreeFunc) AtomFree);
   AsnGenericUserSeqOfFree(ptr -> bonds, (AsnOptFreeFunc) IntraResidueBondFree);
   AsnGenericUserSeqOfFree(ptr -> chiral_centers, (AsnOptFreeFunc) ChiralCenterFree);
   return MemFree(ptr);
}


/**************************************************
*
*    ResidueGraphAsnRead()
*
**************************************************/

ResidueGraphPtr LIBCALL
ResidueGraphAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ResidueGraphPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ResidueGraph ::= (self contained) */
      atp = AsnReadId(aip, amp, RESIDUE_GRAPH);
   } else {
      atp = AsnLinkType(orig, RESIDUE_GRAPH);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ResidueGraphNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == RESIDUE_GRAPH_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_GRAPH_descr) {
      ptr -> descr = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiomolDescrAsnRead, (AsnOptFreeFunc) BiomolDescrFree);
      if (isError && ptr -> descr == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_GRAPH_residue_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> residue_type = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_GRAPH_iupac_code) {
      ptr -> iupac_code = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> iupac_code == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_GRAPH_atoms) {
      ptr -> atoms = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) AtomAsnRead, (AsnOptFreeFunc) AtomFree);
      if (isError && ptr -> atoms == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_GRAPH_bonds) {
      ptr -> bonds = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) IntraResidueBondAsnRead, (AsnOptFreeFunc) IntraResidueBondFree);
      if (isError && ptr -> bonds == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_GRAPH_chiral_centers) {
      ptr -> chiral_centers = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ChiralCenterAsnRead, (AsnOptFreeFunc) ChiralCenterFree);
      if (isError && ptr -> chiral_centers == NULL) {
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
   ptr = ResidueGraphFree(ptr);
   goto ret;
}



/**************************************************
*
*    ResidueGraphAsnWrite()
*
**************************************************/
Boolean LIBCALL 
ResidueGraphAsnWrite(ResidueGraphPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, RESIDUE_GRAPH);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> id;
   retval = AsnWrite(aip, RESIDUE_GRAPH_id,  &av);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> descr, (AsnWriteFunc) BiomolDescrAsnWrite, aip, RESIDUE_GRAPH_descr, RESIDUE_GRAPH_descr_E);
   if (ptr -> residue_type || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> residue_type;
      retval = AsnWrite(aip, RESIDUE_GRAPH_residue_type,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> iupac_code ,ASNCODE_PTRVAL_SLOT, aip, RESIDUE_GRAPH_iupac_code, RESIDUE_GRAPH_iupac_code_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> atoms, (AsnWriteFunc) AtomAsnWrite, aip, RESIDUE_GRAPH_atoms, RESIDUE_GRAPH_atoms_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> bonds, (AsnWriteFunc) IntraResidueBondAsnWrite, aip, RESIDUE_GRAPH_bonds, RESIDUE_GRAPH_bonds_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> chiral_centers, (AsnWriteFunc) ChiralCenterAsnWrite, aip, RESIDUE_GRAPH_chiral_centers, RESIDUE_GRAPH_chiral_centers_E);
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
*    MoleculeGraphNew()
*
**************************************************/

MoleculeGraphPtr LIBCALL
MoleculeGraphNew(void)
{
   MoleculeGraphPtr ptr = MemNew((size_t) sizeof(MoleculeGraph));

   return ptr;

}


/**************************************************
*
*    MoleculeGraphFree()
*
**************************************************/

MoleculeGraphPtr LIBCALL
MoleculeGraphFree(MoleculeGraphPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericChoiceSeqOfFree(ptr -> descr, (AsnOptFreeFunc) BiomolDescrFree);
   SeqIdFree(ptr -> seq_id);
   AsnGenericUserSeqOfFree(ptr -> residue_sequence, (AsnOptFreeFunc) ResidueFree);
   AsnGenericUserSeqOfFree(ptr -> inter_residue_bonds, (AsnOptFreeFunc) InterResidueBondFree);
   return MemFree(ptr);
}


/**************************************************
*
*    MoleculeGraphAsnRead()
*
**************************************************/

MoleculeGraphPtr LIBCALL
MoleculeGraphAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MoleculeGraphPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MoleculeGraph ::= (self contained) */
      atp = AsnReadId(aip, amp, MOLECULE_GRAPH);
   } else {
      atp = AsnLinkType(orig, MOLECULE_GRAPH);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MoleculeGraphNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MOLECULE_GRAPH_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MOLECULE_GRAPH_descr) {
      ptr -> descr = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) BiomolDescrAsnRead, (AsnOptFreeFunc) BiomolDescrFree);
      if (isError && ptr -> descr == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MOLECULE_GRAPH_seq_id) {
      ptr -> seq_id = SeqIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MOLECULE_GRAPH_residue_sequence) {
      ptr -> residue_sequence = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) ResidueAsnRead, (AsnOptFreeFunc) ResidueFree);
      if (isError && ptr -> residue_sequence == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MOLECULE_GRAPH_inter_residue_bonds) {
      ptr -> inter_residue_bonds = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) InterResidueBondAsnRead, (AsnOptFreeFunc) InterResidueBondFree);
      if (isError && ptr -> inter_residue_bonds == NULL) {
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
   ptr = MoleculeGraphFree(ptr);
   goto ret;
}



/**************************************************
*
*    MoleculeGraphAsnWrite()
*
**************************************************/
Boolean LIBCALL 
MoleculeGraphAsnWrite(MoleculeGraphPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MOLECULE_GRAPH);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> id;
   retval = AsnWrite(aip, MOLECULE_GRAPH_id,  &av);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> descr, (AsnWriteFunc) BiomolDescrAsnWrite, aip, MOLECULE_GRAPH_descr, MOLECULE_GRAPH_descr_E);
   if (ptr -> seq_id != NULL) {
      if ( ! SeqIdAsnWrite(ptr -> seq_id, aip, MOLECULE_GRAPH_seq_id)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> residue_sequence, (AsnWriteFunc) ResidueAsnWrite, aip, MOLECULE_GRAPH_residue_sequence, MOLECULE_GRAPH_residue_sequence_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> inter_residue_bonds, (AsnWriteFunc) InterResidueBondAsnWrite, aip, MOLECULE_GRAPH_inter_residue_bonds, MOLECULE_GRAPH_inter_residue_bonds_E);
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
*    InterResidueBondNew()
*
**************************************************/

InterResidueBondPtr LIBCALL
InterResidueBondNew(void)
{
   InterResidueBondPtr ptr = MemNew((size_t) sizeof(InterResidueBond));

   return ptr;

}


/**************************************************
*
*    InterResidueBondFree()
*
**************************************************/

InterResidueBondPtr LIBCALL
InterResidueBondFree(InterResidueBondPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AtomPntrFree(ptr -> atom_id_1);
   AtomPntrFree(ptr -> atom_id_2);
   return MemFree(ptr);
}


/**************************************************
*
*    InterResidueBondAsnRead()
*
**************************************************/

InterResidueBondPtr LIBCALL
InterResidueBondAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   InterResidueBondPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* InterResidueBond ::= (self contained) */
      atp = AsnReadId(aip, amp, INTER_RESIDUE_BOND);
   } else {
      atp = AsnLinkType(orig, INTER_RESIDUE_BOND);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = InterResidueBondNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == INTER_RESIDUE_BOND_atom_id_1) {
      ptr -> atom_id_1 = AtomPntrAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == INTER_RESIDUE_BOND_atom_id_2) {
      ptr -> atom_id_2 = AtomPntrAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == INTER_RESIDUE_BOND_bond_order) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> bond_order = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
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
   ptr = InterResidueBondFree(ptr);
   goto ret;
}



/**************************************************
*
*    InterResidueBondAsnWrite()
*
**************************************************/
Boolean LIBCALL 
InterResidueBondAsnWrite(InterResidueBondPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, INTER_RESIDUE_BOND);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> atom_id_1 != NULL) {
      if ( ! AtomPntrAsnWrite(ptr -> atom_id_1, aip, INTER_RESIDUE_BOND_atom_id_1)) {
         goto erret;
      }
   }
   if (ptr -> atom_id_2 != NULL) {
      if ( ! AtomPntrAsnWrite(ptr -> atom_id_2, aip, INTER_RESIDUE_BOND_atom_id_2)) {
         goto erret;
      }
   }
   if (ptr -> bond_order || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> bond_order;
      retval = AsnWrite(aip, INTER_RESIDUE_BOND_bond_order,  &av);
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
*    ResidueNew()
*
**************************************************/

ResiduePtr LIBCALL
ResidueNew(void)
{
   ResiduePtr ptr = MemNew((size_t) sizeof(Residue));

   return ptr;

}


/**************************************************
*
*    ResidueFree()
*
**************************************************/

ResiduePtr LIBCALL
ResidueFree(ResiduePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   ResidueGraphPntrFree(ptr -> residue_graph);
   return MemFree(ptr);
}


/**************************************************
*
*    ResidueAsnRead()
*
**************************************************/

ResiduePtr LIBCALL
ResidueAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ResiduePtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Residue ::= (self contained) */
      atp = AsnReadId(aip, amp, RESIDUE);
   } else {
      atp = AsnLinkType(orig, RESIDUE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ResidueNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == RESIDUE_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == RESIDUE_residue_graph) {
      ptr -> residue_graph = ResidueGraphPntrAsnRead(aip, atp);
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
   ptr = ResidueFree(ptr);
   goto ret;
}



/**************************************************
*
*    ResidueAsnWrite()
*
**************************************************/
Boolean LIBCALL 
ResidueAsnWrite(ResiduePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, RESIDUE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> id;
   retval = AsnWrite(aip, RESIDUE_id,  &av);
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, RESIDUE_name,  &av);
   }
   if (ptr -> residue_graph != NULL) {
      if ( ! ResidueGraphPntrAsnWrite(ptr -> residue_graph, aip, RESIDUE_residue_graph)) {
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
*    ResidueGraphPntrFree()
*
**************************************************/

ResidueGraphPntrPtr LIBCALL
ResidueGraphPntrFree(ValNodePtr anp)
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
   case ResidueGraphPntr_biostruc:
      BiostrucGraphPntrFree(anp -> data.ptrvalue);
      break;
   case ResidueGraphPntr_standard:
      BiostrucResidueGraphSetPntrFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    ResidueGraphPntrAsnRead()
*
**************************************************/

ResidueGraphPntrPtr LIBCALL
ResidueGraphPntrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ResidueGraphPntr ::= (self contained) */
      atp = AsnReadId(aip, amp, RESIDUE_GRAPH_PNTR);
   } else {
      atp = AsnLinkType(orig, RESIDUE_GRAPH_PNTR);    /* link in local tree */
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
   if (atp == RESIDUE_GRAPH_PNTR_local) {
      choice = ResidueGraphPntr_local;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == RESIDUE_GRAPH_PNTR_biostruc) {
      choice = ResidueGraphPntr_biostruc;
      func = (AsnReadFunc) BiostrucGraphPntrAsnRead;
   }
   else if (atp == RESIDUE_GRAPH_PNTR_standard) {
      choice = ResidueGraphPntr_standard;
      func = (AsnReadFunc) BiostrucResidueGraphSetPntrAsnRead;
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
*    ResidueGraphPntrAsnWrite()
*
**************************************************/
Boolean LIBCALL 
ResidueGraphPntrAsnWrite(ResidueGraphPntrPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, RESIDUE_GRAPH_PNTR);   /* link local tree */
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
   case ResidueGraphPntr_local:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, RESIDUE_GRAPH_PNTR_local, &av);
      break;
   case ResidueGraphPntr_biostruc:
      writetype = RESIDUE_GRAPH_PNTR_biostruc;
      func = (AsnWriteFunc) BiostrucGraphPntrAsnWrite;
      break;
   case ResidueGraphPntr_standard:
      writetype = RESIDUE_GRAPH_PNTR_standard;
      func = (AsnWriteFunc) BiostrucResidueGraphSetPntrAsnWrite;
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
*    BiostrucGraphPntrNew()
*
**************************************************/

BiostrucGraphPntrPtr LIBCALL
BiostrucGraphPntrNew(void)
{
   BiostrucGraphPntrPtr ptr = MemNew((size_t) sizeof(BiostrucGraphPntr));

   return ptr;

}


/**************************************************
*
*    BiostrucGraphPntrFree()
*
**************************************************/

BiostrucGraphPntrPtr LIBCALL
BiostrucGraphPntrFree(BiostrucGraphPntrPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   BiostrucIdFree(ptr -> biostruc_id);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucGraphPntrAsnRead()
*
**************************************************/

BiostrucGraphPntrPtr LIBCALL
BiostrucGraphPntrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucGraphPntrPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucGraphPntr ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_GRAPH_PNTR);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_GRAPH_PNTR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucGraphPntrNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_GRAPH_PNTR_biostruc_id) {
      ptr -> biostruc_id = BiostrucIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_GRAPH_PNTR_residue_graph_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> residue_graph_id = av.intvalue;
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
   ptr = BiostrucGraphPntrFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucGraphPntrAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucGraphPntrAsnWrite(BiostrucGraphPntrPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_GRAPH_PNTR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> biostruc_id != NULL) {
      if ( ! BiostrucIdAsnWrite(ptr -> biostruc_id, aip, BIOSTRUC_GRAPH_PNTR_biostruc_id)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> residue_graph_id;
   retval = AsnWrite(aip, BIOSTRUC_GRAPH_PNTR_residue_graph_id,  &av);
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
*    BiostrucResidueGraphSetPntrNew()
*
**************************************************/

BiostrucResidueGraphSetPntrPtr LIBCALL
BiostrucResidueGraphSetPntrNew(void)
{
   BiostrucResidueGraphSetPntrPtr ptr = MemNew((size_t) sizeof(BiostrucResidueGraphSetPntr));

   return ptr;

}


/**************************************************
*
*    BiostrucResidueGraphSetPntrFree()
*
**************************************************/

BiostrucResidueGraphSetPntrPtr LIBCALL
BiostrucResidueGraphSetPntrFree(BiostrucResidueGraphSetPntrPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   BiostrucIdFree(ptr -> biostruc_residue_graph_set_id);
   return MemFree(ptr);
}


/**************************************************
*
*    BiostrucResidueGraphSetPntrAsnRead()
*
**************************************************/

BiostrucResidueGraphSetPntrPtr LIBCALL
BiostrucResidueGraphSetPntrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   BiostrucResidueGraphSetPntrPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* BiostrucResidueGraphSetPntr ::= (self contained) */
      atp = AsnReadId(aip, amp, BIOSTRUC_RESIDUE_GRAPH_SET_PNTR);
   } else {
      atp = AsnLinkType(orig, BIOSTRUC_RESIDUE_GRAPH_SET_PNTR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = BiostrucResidueGraphSetPntrNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == BIOSTRUC_RESIDUE_GRAPH_SET_PNTR_biostruc_residue_graph_set_id) {
      ptr -> biostruc_residue_graph_set_id = BiostrucIdAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == BIOSTRUC_RESIDUE_GRAPH_SET_PNTR_residue_graph_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> residue_graph_id = av.intvalue;
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
   ptr = BiostrucResidueGraphSetPntrFree(ptr);
   goto ret;
}



/**************************************************
*
*    BiostrucResidueGraphSetPntrAsnWrite()
*
**************************************************/
Boolean LIBCALL 
BiostrucResidueGraphSetPntrAsnWrite(BiostrucResidueGraphSetPntrPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, BIOSTRUC_RESIDUE_GRAPH_SET_PNTR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> biostruc_residue_graph_set_id != NULL) {
      if ( ! BiostrucIdAsnWrite(ptr -> biostruc_residue_graph_set_id, aip, BIOSTRUC_RESIDUE_GRAPH_SET_PNTR_biostruc_residue_graph_set_id)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> residue_graph_id;
   retval = AsnWrite(aip, BIOSTRUC_RESIDUE_GRAPH_SET_PNTR_residue_graph_id,  &av);
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
*    AtomNew()
*
**************************************************/

AtomPtr LIBCALL
AtomNew(void)
{
   AtomPtr ptr = MemNew((size_t) sizeof(Atom));

   return ptr;

}


/**************************************************
*
*    AtomFree()
*
**************************************************/

AtomPtr LIBCALL
AtomFree(AtomPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   AsnGenericBaseSeqOfFree(ptr -> iupac_code ,ASNCODE_PTRVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    AtomAsnRead()
*
**************************************************/

AtomPtr LIBCALL
AtomAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AtomPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Atom ::= (self contained) */
      atp = AsnReadId(aip, amp, ATOM);
   } else {
      atp = AsnLinkType(orig, ATOM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AtomNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ATOM_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOM_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOM_iupac_code) {
      ptr -> iupac_code = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> iupac_code == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOM_element) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> element = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOM_ionizable_proton) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ionizable_proton = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
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
   ptr = AtomFree(ptr);
   goto ret;
}



/**************************************************
*
*    AtomAsnWrite()
*
**************************************************/
Boolean LIBCALL 
AtomAsnWrite(AtomPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ATOM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> id;
   retval = AsnWrite(aip, ATOM_id,  &av);
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, ATOM_name,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> iupac_code ,ASNCODE_PTRVAL_SLOT, aip, ATOM_iupac_code, ATOM_iupac_code_E);
   av.intvalue = ptr -> element;
   retval = AsnWrite(aip, ATOM_element,  &av);
   if (ptr -> ionizable_proton || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> ionizable_proton;
      retval = AsnWrite(aip, ATOM_ionizable_proton,  &av);
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
*    IntraResidueBondNew()
*
**************************************************/

IntraResidueBondPtr LIBCALL
IntraResidueBondNew(void)
{
   IntraResidueBondPtr ptr = MemNew((size_t) sizeof(IntraResidueBond));

   return ptr;

}


/**************************************************
*
*    IntraResidueBondFree()
*
**************************************************/

IntraResidueBondPtr LIBCALL
IntraResidueBondFree(IntraResidueBondPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    IntraResidueBondAsnRead()
*
**************************************************/

IntraResidueBondPtr LIBCALL
IntraResidueBondAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   IntraResidueBondPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* IntraResidueBond ::= (self contained) */
      atp = AsnReadId(aip, amp, INTRA_RESIDUE_BOND);
   } else {
      atp = AsnLinkType(orig, INTRA_RESIDUE_BOND);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = IntraResidueBondNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == INTRA_RESIDUE_BOND_atom_id_1) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> atom_id_1 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == INTRA_RESIDUE_BOND_atom_id_2) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> atom_id_2 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == INTRA_RESIDUE_BOND_bond_order) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> bond_order = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
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
   ptr = IntraResidueBondFree(ptr);
   goto ret;
}



/**************************************************
*
*    IntraResidueBondAsnWrite()
*
**************************************************/
Boolean LIBCALL 
IntraResidueBondAsnWrite(IntraResidueBondPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, INTRA_RESIDUE_BOND);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> atom_id_1;
   retval = AsnWrite(aip, INTRA_RESIDUE_BOND_atom_id_1,  &av);
   av.intvalue = ptr -> atom_id_2;
   retval = AsnWrite(aip, INTRA_RESIDUE_BOND_atom_id_2,  &av);
   if (ptr -> bond_order || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> bond_order;
      retval = AsnWrite(aip, INTRA_RESIDUE_BOND_bond_order,  &av);
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
*    ChiralCenterNew()
*
**************************************************/

ChiralCenterPtr LIBCALL
ChiralCenterNew(void)
{
   ChiralCenterPtr ptr = MemNew((size_t) sizeof(ChiralCenter));

   return ptr;

}


/**************************************************
*
*    ChiralCenterFree()
*
**************************************************/

ChiralCenterPtr LIBCALL
ChiralCenterFree(ChiralCenterPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    ChiralCenterAsnRead()
*
**************************************************/

ChiralCenterPtr LIBCALL
ChiralCenterAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   ChiralCenterPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* ChiralCenter ::= (self contained) */
      atp = AsnReadId(aip, amp, CHIRAL_CENTER);
   } else {
      atp = AsnLinkType(orig, CHIRAL_CENTER);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = ChiralCenterNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == CHIRAL_CENTER_c) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> c = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHIRAL_CENTER_n1) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> n1 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHIRAL_CENTER_n2) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> n2 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHIRAL_CENTER_n3) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> n3 = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == CHIRAL_CENTER_sign) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sign = av.intvalue;
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
   ptr = ChiralCenterFree(ptr);
   goto ret;
}



/**************************************************
*
*    ChiralCenterAsnWrite()
*
**************************************************/
Boolean LIBCALL 
ChiralCenterAsnWrite(ChiralCenterPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, CHIRAL_CENTER);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> c;
   retval = AsnWrite(aip, CHIRAL_CENTER_c,  &av);
   av.intvalue = ptr -> n1;
   retval = AsnWrite(aip, CHIRAL_CENTER_n1,  &av);
   av.intvalue = ptr -> n2;
   retval = AsnWrite(aip, CHIRAL_CENTER_n2,  &av);
   av.intvalue = ptr -> n3;
   retval = AsnWrite(aip, CHIRAL_CENTER_n3,  &av);
   av.intvalue = ptr -> sign;
   retval = AsnWrite(aip, CHIRAL_CENTER_sign,  &av);
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
*    AtomPntrNew()
*
**************************************************/

AtomPntrPtr LIBCALL
AtomPntrNew(void)
{
   AtomPntrPtr ptr = MemNew((size_t) sizeof(AtomPntr));

   return ptr;

}


/**************************************************
*
*    AtomPntrFree()
*
**************************************************/

AtomPntrPtr LIBCALL
AtomPntrFree(AtomPntrPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    AtomPntrAsnRead()
*
**************************************************/

AtomPntrPtr LIBCALL
AtomPntrAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AtomPntrPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AtomPntr ::= (self contained) */
      atp = AsnReadId(aip, amp, ATOM_PNTR);
   } else {
      atp = AsnLinkType(orig, ATOM_PNTR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = AtomPntrNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ATOM_PNTR_molecule_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> molecule_id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOM_PNTR_residue_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> residue_id = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ATOM_PNTR_atom_id) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> atom_id = av.intvalue;
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
   ptr = AtomPntrFree(ptr);
   goto ret;
}



/**************************************************
*
*    AtomPntrAsnWrite()
*
**************************************************/
Boolean LIBCALL 
AtomPntrAsnWrite(AtomPntrPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ATOM_PNTR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> molecule_id;
   retval = AsnWrite(aip, ATOM_PNTR_molecule_id,  &av);
   av.intvalue = ptr -> residue_id;
   retval = AsnWrite(aip, ATOM_PNTR_residue_id,  &av);
   av.intvalue = ptr -> atom_id;
   retval = AsnWrite(aip, ATOM_PNTR_atom_id,  &av);
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
*    AtomPntrSetFree()
*
**************************************************/

AtomPntrSetPtr LIBCALL
AtomPntrSetFree(AtomPntrSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) AtomPntrFree);
   return NULL;
}


/**************************************************
*
*    AtomPntrSetAsnRead()
*
**************************************************/

AtomPntrSetPtr LIBCALL
AtomPntrSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   AtomPntrSetPtr ptr;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* AtomPntrSet ::= (self contained) */
      atp = AsnReadId(aip, amp, ATOM_PNTR_SET);
   } else {
      atp = AsnLinkType(orig, ATOM_PNTR_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) AtomPntrAsnRead, (AsnOptFreeFunc) AtomPntrFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = AtomPntrSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    AtomPntrSetAsnWrite()
*
**************************************************/
Boolean LIBCALL 
AtomPntrSetAsnWrite(AtomPntrSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmmdb1AsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ATOM_PNTR_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) AtomPntrAsnWrite, aip, atp, ATOM_PNTR_SET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

