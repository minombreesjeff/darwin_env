#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <egkludge.h>
#include <objentgene.h>

static Boolean loaded = FALSE;

#include <asnentgene.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objentgeneAsnLoad(void)
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
*    Generated object loaders for Module NCBI-Entrezgene
*    Generated using ASNCODE Revision: 6.16 at Apr 19, 2005 10:50 AM
*
**************************************************/


/**************************************************
*
*    EntrezgeneNew()
*
**************************************************/
NLM_EXTERN 
EntrezgenePtr LIBCALL
EntrezgeneNew(void)
{
   EntrezgenePtr ptr = MemNew((size_t) sizeof(Entrezgene));

   return ptr;

}


/**************************************************
*
*    EntrezgeneFree()
*
**************************************************/
NLM_EXTERN 
EntrezgenePtr LIBCALL
EntrezgeneFree(EntrezgenePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   GeneTrackFree(ptr -> track_info);
   BioSourceFree(ptr -> source);
   GeneRefFree(ptr -> gene);
   ProtRefFree(ptr -> prot);
   RNARefFree(ptr -> rna);
   MemFree(ptr -> summary);
   AsnGenericUserSeqOfFree(ptr -> location, (AsnOptFreeFunc) MapsFree);
   GeneSourceFree(ptr -> gene_source);
   AsnGenericUserSeqOfFree(ptr -> locus, (AsnOptFreeFunc) GeneCommentaryFree);
   AsnGenericUserSeqOfFree(ptr -> properties, (AsnOptFreeFunc) GeneCommentaryFree);
   AsnGenericUserSeqOfFree(ptr -> refgene, (AsnOptFreeFunc) GeneCommentaryFree);
   AsnGenericUserSeqOfFree(ptr -> homology, (AsnOptFreeFunc) GeneCommentaryFree);
   AsnGenericUserSeqOfFree(ptr -> comments, (AsnOptFreeFunc) GeneCommentaryFree);
   AsnGenericUserSeqOfFree(ptr -> unique_keys, (AsnOptFreeFunc) DbtagFree);
   AsnGenericBaseSeqOfFree(ptr -> xtra_index_terms ,ASNCODE_PTRVAL_SLOT);
   AsnGenericUserSeqOfFree(ptr -> xtra_properties, (AsnOptFreeFunc) XtraTermsFree);
   AsnGenericUserSeqOfFree(ptr -> xtra_iq, (AsnOptFreeFunc) XtraTermsFree);
   AsnGenericUserSeqOfFree(ptr -> non_unique_keys, (AsnOptFreeFunc) DbtagFree);
   return MemFree(ptr);
}


/**************************************************
*
*    EntrezgeneAsnRead()
*
**************************************************/
NLM_EXTERN 
EntrezgenePtr LIBCALL
EntrezgeneAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   EntrezgenePtr ptr;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Entrezgene ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZGENE);
   } else {
      atp = AsnLinkType(orig, ENTREZGENE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = EntrezgeneNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == ENTREZGENE_track_info) {
      ptr -> track_info = GeneTrackAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_source) {
      ptr -> source = BioSourceAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_gene) {
      ptr -> gene = GeneRefAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_prot) {
      ptr -> prot = ProtRefAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_rna) {
      ptr -> rna = RNARefAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_summary) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> summary = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_location) {
      ptr -> location = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MapsAsnRead, (AsnOptFreeFunc) MapsFree);
      if (isError && ptr -> location == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_gene_source) {
      ptr -> gene_source = GeneSourceAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_locus) {
      ptr -> locus = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GeneCommentaryAsnRead, (AsnOptFreeFunc) GeneCommentaryFree);
      if (isError && ptr -> locus == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_properties) {
      ptr -> properties = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GeneCommentaryAsnRead, (AsnOptFreeFunc) GeneCommentaryFree);
      if (isError && ptr -> properties == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_refgene) {
      ptr -> refgene = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GeneCommentaryAsnRead, (AsnOptFreeFunc) GeneCommentaryFree);
      if (isError && ptr -> refgene == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_homology) {
      ptr -> homology = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GeneCommentaryAsnRead, (AsnOptFreeFunc) GeneCommentaryFree);
      if (isError && ptr -> homology == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_comments) {
      ptr -> comments = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GeneCommentaryAsnRead, (AsnOptFreeFunc) GeneCommentaryFree);
      if (isError && ptr -> comments == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_unique_keys) {
      ptr -> unique_keys = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) DbtagAsnRead, (AsnOptFreeFunc) DbtagFree);
      if (isError && ptr -> unique_keys == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_xtra_index_terms) {
      ptr -> xtra_index_terms = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> xtra_index_terms == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_xtra_properties) {
      ptr -> xtra_properties = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) XtraTermsAsnRead, (AsnOptFreeFunc) XtraTermsFree);
      if (isError && ptr -> xtra_properties == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_xtra_iq) {
      ptr -> xtra_iq = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) XtraTermsAsnRead, (AsnOptFreeFunc) XtraTermsFree);
      if (isError && ptr -> xtra_iq == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == ENTREZGENE_non_unique_keys) {
      ptr -> non_unique_keys = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) DbtagAsnRead, (AsnOptFreeFunc) DbtagFree);
      if (isError && ptr -> non_unique_keys == NULL) {
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
   ptr = EntrezgeneFree(ptr);
   goto ret;
}



/**************************************************
*
*    EntrezgeneAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
EntrezgeneAsnWrite(EntrezgenePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZGENE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> track_info != NULL) {
      if ( ! GeneTrackAsnWrite(ptr -> track_info, aip, ENTREZGENE_track_info)) {
         goto erret;
      }
   }
   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, ENTREZGENE_type,  &av);
   if (ptr -> source != NULL) {
      if ( ! BioSourceAsnWrite(ptr -> source, aip, ENTREZGENE_source)) {
         goto erret;
      }
   }
   if (ptr -> gene != NULL) {
      if ( ! GeneRefAsnWrite(ptr -> gene, aip, ENTREZGENE_gene)) {
         goto erret;
      }
   }
   if (ptr -> prot != NULL) {
      if ( ! ProtRefAsnWrite(ptr -> prot, aip, ENTREZGENE_prot)) {
         goto erret;
      }
   }
   if (ptr -> rna != NULL) {
      if ( ! RNARefAsnWrite(ptr -> rna, aip, ENTREZGENE_rna)) {
         goto erret;
      }
   }
   if (ptr -> summary != NULL) {
      av.ptrvalue = ptr -> summary;
      retval = AsnWrite(aip, ENTREZGENE_summary,  &av);
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> location, (AsnWriteFunc) MapsAsnWrite, aip, ENTREZGENE_location, ENTREZGENE_location_E);
   if (ptr -> gene_source != NULL) {
      if ( ! GeneSourceAsnWrite(ptr -> gene_source, aip, ENTREZGENE_gene_source)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> locus, (AsnWriteFunc) GeneCommentaryAsnWrite, aip, ENTREZGENE_locus, ENTREZGENE_locus_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> properties, (AsnWriteFunc) GeneCommentaryAsnWrite, aip, ENTREZGENE_properties, ENTREZGENE_properties_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> refgene, (AsnWriteFunc) GeneCommentaryAsnWrite, aip, ENTREZGENE_refgene, ENTREZGENE_refgene_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> homology, (AsnWriteFunc) GeneCommentaryAsnWrite, aip, ENTREZGENE_homology, ENTREZGENE_homology_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> comments, (AsnWriteFunc) GeneCommentaryAsnWrite, aip, ENTREZGENE_comments, ENTREZGENE_comments_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> unique_keys, (AsnWriteFunc) DbtagAsnWrite, aip, ENTREZGENE_unique_keys, ENTREZGENE_unique_keys_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> xtra_index_terms ,ASNCODE_PTRVAL_SLOT, aip, ENTREZGENE_xtra_index_terms, ENTREZGENE_xtra_index_terms_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> xtra_properties, (AsnWriteFunc) XtraTermsAsnWrite, aip, ENTREZGENE_xtra_properties, ENTREZGENE_xtra_properties_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> xtra_iq, (AsnWriteFunc) XtraTermsAsnWrite, aip, ENTREZGENE_xtra_iq, ENTREZGENE_xtra_iq_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> non_unique_keys, (AsnWriteFunc) DbtagAsnWrite, aip, ENTREZGENE_non_unique_keys, ENTREZGENE_non_unique_keys_E);
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
*    EntrezgeneSetFree()
*
**************************************************/
NLM_EXTERN 
EntrezgeneSetPtr LIBCALL
EntrezgeneSetFree(EntrezgeneSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) EntrezgeneFree);
   return NULL;
}


/**************************************************
*
*    EntrezgeneSetAsnRead()
*
**************************************************/
NLM_EXTERN 
EntrezgeneSetPtr LIBCALL
EntrezgeneSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   EntrezgeneSetPtr ptr;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* EntrezgeneSet ::= (self contained) */
      atp = AsnReadId(aip, amp, ENTREZGENE_SET);
   } else {
      atp = AsnLinkType(orig, ENTREZGENE_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) EntrezgeneAsnRead, (AsnOptFreeFunc) EntrezgeneFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = EntrezgeneSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    EntrezgeneSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
EntrezgeneSetAsnWrite(EntrezgeneSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, ENTREZGENE_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) EntrezgeneAsnWrite, aip, atp, ENTREZGENE_SET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}



/**************************************************
*
*    GeneTrackNew()
*
**************************************************/
NLM_EXTERN 
GeneTrackPtr LIBCALL
GeneTrackNew(void)
{
   GeneTrackPtr ptr = MemNew((size_t) sizeof(GeneTrack));

   ptr -> status = 0;
   return ptr;

}


/**************************************************
*
*    GeneTrackFree()
*
**************************************************/
NLM_EXTERN 
GeneTrackPtr LIBCALL
GeneTrackFree(GeneTrackPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> current_id, (AsnOptFreeFunc) DbtagFree);
   DateFree(ptr -> create_date);
   DateFree(ptr -> update_date);
   DateFree(ptr -> discontinue_date);
   return MemFree(ptr);
}


/**************************************************
*
*    GeneTrackAsnRead()
*
**************************************************/
NLM_EXTERN 
GeneTrackPtr LIBCALL
GeneTrackAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GeneTrackPtr ptr;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GeneTrack ::= (self contained) */
      atp = AsnReadId(aip, amp, GENE_TRACK);
   } else {
      atp = AsnLinkType(orig, GENE_TRACK);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GeneTrackNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GENE_TRACK_geneid) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> geneid = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_TRACK_status) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> status = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_TRACK_current_id) {
      ptr -> current_id = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) DbtagAsnRead, (AsnOptFreeFunc) DbtagFree);
      if (isError && ptr -> current_id == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_TRACK_create_date) {
      ptr -> create_date = DateAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_TRACK_update_date) {
      ptr -> update_date = DateAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_TRACK_discontinue_date) {
      ptr -> discontinue_date = DateAsnRead(aip, atp);
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
   ptr = GeneTrackFree(ptr);
   goto ret;
}



/**************************************************
*
*    GeneTrackAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GeneTrackAsnWrite(GeneTrackPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GENE_TRACK);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> geneid;
   retval = AsnWrite(aip, GENE_TRACK_geneid,  &av);
   av.intvalue = ptr -> status;
   retval = AsnWrite(aip, GENE_TRACK_status,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> current_id, (AsnWriteFunc) DbtagAsnWrite, aip, GENE_TRACK_current_id, GENE_TRACK_current_id_E);
   if (ptr -> create_date != NULL) {
      if ( ! DateAsnWrite(ptr -> create_date, aip, GENE_TRACK_create_date)) {
         goto erret;
      }
   }
   if (ptr -> update_date != NULL) {
      if ( ! DateAsnWrite(ptr -> update_date, aip, GENE_TRACK_update_date)) {
         goto erret;
      }
   }
   if (ptr -> discontinue_date != NULL) {
      if ( ! DateAsnWrite(ptr -> discontinue_date, aip, GENE_TRACK_discontinue_date)) {
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
*    MapsNew()
*
**************************************************/
NLM_EXTERN 
MapsPtr LIBCALL
MapsNew(void)
{
   MapsPtr ptr = MemNew((size_t) sizeof(Maps));

   return ptr;

}


/**************************************************
*
*    MapsFree()
*
**************************************************/
NLM_EXTERN 
MapsPtr LIBCALL
MapsFree(MapsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> display_str);
   Method_methodFree(ptr -> Method_method);
   return MemFree(ptr);
}


/**************************************************
*
*    Method_methodFree()
*
**************************************************/
static 
Method_methodPtr LIBCALL
Method_methodFree(ValNodePtr anp)
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
   case Method_method_proxy:
      MemFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    MapsAsnRead()
*
**************************************************/
NLM_EXTERN 
MapsPtr LIBCALL
MapsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MapsPtr ptr;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Maps ::= (self contained) */
      atp = AsnReadId(aip, amp, MAPS);
   } else {
      atp = AsnLinkType(orig, MAPS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MapsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MAPS_display_str) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> display_str = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MAPS_method) {
      ptr -> Method_method = Method_methodAsnRead(aip, atp);
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
   ptr = MapsFree(ptr);
   goto ret;
}



/**************************************************
*
*    Method_methodAsnRead()
*
**************************************************/
static 
Method_methodPtr LIBCALL
Method_methodAsnRead(AsnIoPtr aip, AsnTypePtr orig)
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
      if (! objentgeneAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* Method_method ::= (self contained) */
      atp = AsnReadId(aip, amp, MAPS_method);
   } else {
      atp = AsnLinkType(orig, MAPS_method);    /* link in local tree */
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
   if (atp == MAPS_method_proxy) {
      choice = Method_method_proxy;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == MAPS_method_map_type) {
      choice = Method_method_map_type;
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
*    MapsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MapsAsnWrite(MapsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MAPS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> display_str != NULL) {
      av.ptrvalue = ptr -> display_str;
      retval = AsnWrite(aip, MAPS_display_str,  &av);
   }
   if (ptr -> Method_method != NULL) {
      if ( ! Method_methodAsnWrite(ptr -> Method_method, aip, MAPS_method)) {
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
*    Method_methodAsnWrite()
*
**************************************************/
static Boolean LIBCALL 
Method_methodAsnWrite(Method_methodPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objentgeneAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, MAPS_method);   /* link local tree */
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
   case Method_method_proxy:
      av.ptrvalue = anp->data.ptrvalue;
      retval = AsnWrite(aip, MAPS_method_proxy, &av);
      break;
   case Method_method_map_type:
      av.intvalue = anp->data.intvalue;
      retval = AsnWrite(aip, MAPS_method_map_type, &av);
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
*    GeneSourceNew()
*
**************************************************/
NLM_EXTERN 
GeneSourcePtr LIBCALL
GeneSourceNew(void)
{
   GeneSourcePtr ptr = MemNew((size_t) sizeof(GeneSource));

   ptr -> gene_display = 0;
   ptr -> locus_display = 0;
   ptr -> extra_terms = 0;
   return ptr;

}


/**************************************************
*
*    GeneSourceFree()
*
**************************************************/
NLM_EXTERN 
GeneSourcePtr LIBCALL
GeneSourceFree(GeneSourcePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> src);
   MemFree(ptr -> src_str1);
   MemFree(ptr -> src_str2);
   return MemFree(ptr);
}


/**************************************************
*
*    GeneSourceAsnRead()
*
**************************************************/
NLM_EXTERN 
GeneSourcePtr LIBCALL
GeneSourceAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GeneSourcePtr ptr;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GeneSource ::= (self contained) */
      atp = AsnReadId(aip, amp, GENE_SOURCE);
   } else {
      atp = AsnLinkType(orig, GENE_SOURCE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GeneSourceNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GENE_SOURCE_src) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> src = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_SOURCE_src_int) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> src_int = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_SOURCE_src_str1) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> src_str1 = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_SOURCE_src_str2) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> src_str2 = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_SOURCE_gene_display) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> gene_display = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_SOURCE_locus_display) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> locus_display = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_SOURCE_extra_terms) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> extra_terms = av.boolvalue;
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
   ptr = GeneSourceFree(ptr);
   goto ret;
}



/**************************************************
*
*    GeneSourceAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GeneSourceAsnWrite(GeneSourcePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GENE_SOURCE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> src != NULL) {
      av.ptrvalue = ptr -> src;
      retval = AsnWrite(aip, GENE_SOURCE_src,  &av);
   }
   av.intvalue = ptr -> src_int;
   retval = AsnWrite(aip, GENE_SOURCE_src_int,  &av);
   if (ptr -> src_str1 != NULL) {
      av.ptrvalue = ptr -> src_str1;
      retval = AsnWrite(aip, GENE_SOURCE_src_str1,  &av);
   }
   if (ptr -> src_str2 != NULL) {
      av.ptrvalue = ptr -> src_str2;
      retval = AsnWrite(aip, GENE_SOURCE_src_str2,  &av);
   }
   av.boolvalue = ptr -> gene_display;
   retval = AsnWrite(aip, GENE_SOURCE_gene_display,  &av);
   av.boolvalue = ptr -> locus_display;
   retval = AsnWrite(aip, GENE_SOURCE_locus_display,  &av);
   av.boolvalue = ptr -> extra_terms;
   retval = AsnWrite(aip, GENE_SOURCE_extra_terms,  &av);
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
*    GeneCommentaryNew()
*
**************************************************/
NLM_EXTERN 
GeneCommentaryPtr LIBCALL
GeneCommentaryNew(void)
{
   GeneCommentaryPtr ptr = MemNew((size_t) sizeof(GeneCommentary));

   return ptr;

}


/**************************************************
*
*    GeneCommentaryFree()
*
**************************************************/
NLM_EXTERN 
GeneCommentaryPtr LIBCALL
GeneCommentaryFree(GeneCommentaryPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> heading);
   MemFree(ptr -> label);
   MemFree(ptr -> text);
   MemFree(ptr -> accession);
   AsnGenericUserSeqOfFree(ptr -> xtra_properties, (AsnOptFreeFunc) XtraTermsFree);
   AsnGenericChoiceSeqOfFree(ptr -> refs, (AsnOptFreeFunc) PubFree);
   AsnGenericUserSeqOfFree(ptr -> source, (AsnOptFreeFunc) OtherSourceFree);
   AsnGenericChoiceSeqOfFree(ptr -> genomic_coords, (AsnOptFreeFunc) SeqLocFree);
   AsnGenericChoiceSeqOfFree(ptr -> seqs, (AsnOptFreeFunc) SeqLocFree);
   AsnGenericUserSeqOfFree(ptr -> products, (AsnOptFreeFunc) GeneCommentaryFree);
   AsnGenericUserSeqOfFree(ptr -> properties, (AsnOptFreeFunc) GeneCommentaryFree);
   AsnGenericUserSeqOfFree(ptr -> comment, (AsnOptFreeFunc) GeneCommentaryFree);
   DateFree(ptr -> create_date);
   DateFree(ptr -> update_date);
   return MemFree(ptr);
}


/**************************************************
*
*    GeneCommentaryAsnRead()
*
**************************************************/
NLM_EXTERN 
GeneCommentaryPtr LIBCALL
GeneCommentaryAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GeneCommentaryPtr ptr;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GeneCommentary ::= (self contained) */
      atp = AsnReadId(aip, amp, GENE_COMMENTARY);
   } else {
      atp = AsnLinkType(orig, GENE_COMMENTARY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GeneCommentaryNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GENE_COMMENTARY_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_heading) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> heading = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_label) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> label = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_text) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> text = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_accession) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> accession = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> version = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_xtra_properties) {
      ptr -> xtra_properties = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) XtraTermsAsnRead, (AsnOptFreeFunc) XtraTermsFree);
      if (isError && ptr -> xtra_properties == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_refs) {
      ptr -> refs = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) PubAsnRead, (AsnOptFreeFunc) PubFree);
      if (isError && ptr -> refs == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_source) {
      ptr -> source = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) OtherSourceAsnRead, (AsnOptFreeFunc) OtherSourceFree);
      if (isError && ptr -> source == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_genomic_coords) {
      ptr -> genomic_coords = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqLocAsnRead, (AsnOptFreeFunc) SeqLocFree);
      if (isError && ptr -> genomic_coords == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_seqs) {
      ptr -> seqs = AsnGenericChoiceSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) SeqLocAsnRead, (AsnOptFreeFunc) SeqLocFree);
      if (isError && ptr -> seqs == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_products) {
      ptr -> products = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GeneCommentaryAsnRead, (AsnOptFreeFunc) GeneCommentaryFree);
      if (isError && ptr -> products == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_properties) {
      ptr -> properties = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GeneCommentaryAsnRead, (AsnOptFreeFunc) GeneCommentaryFree);
      if (isError && ptr -> properties == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_comment) {
      ptr -> comment = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GeneCommentaryAsnRead, (AsnOptFreeFunc) GeneCommentaryFree);
      if (isError && ptr -> comment == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_create_date) {
      ptr -> create_date = DateAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GENE_COMMENTARY_update_date) {
      ptr -> update_date = DateAsnRead(aip, atp);
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
   ptr = GeneCommentaryFree(ptr);
   goto ret;
}



/**************************************************
*
*    GeneCommentaryAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GeneCommentaryAsnWrite(GeneCommentaryPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GENE_COMMENTARY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, GENE_COMMENTARY_type,  &av);
   if (ptr -> heading != NULL) {
      av.ptrvalue = ptr -> heading;
      retval = AsnWrite(aip, GENE_COMMENTARY_heading,  &av);
   }
   if (ptr -> label != NULL) {
      av.ptrvalue = ptr -> label;
      retval = AsnWrite(aip, GENE_COMMENTARY_label,  &av);
   }
   if (ptr -> text != NULL) {
      av.ptrvalue = ptr -> text;
      retval = AsnWrite(aip, GENE_COMMENTARY_text,  &av);
   }
   if (ptr -> accession != NULL) {
      av.ptrvalue = ptr -> accession;
      retval = AsnWrite(aip, GENE_COMMENTARY_accession,  &av);
   }
   av.intvalue = ptr -> version;
   retval = AsnWrite(aip, GENE_COMMENTARY_version,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> xtra_properties, (AsnWriteFunc) XtraTermsAsnWrite, aip, GENE_COMMENTARY_xtra_properties, COMMENTARY_xtra_properties_E);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> refs, (AsnWriteFunc) PubAsnWrite, aip, GENE_COMMENTARY_refs, GENE_COMMENTARY_refs_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> source, (AsnWriteFunc) OtherSourceAsnWrite, aip, GENE_COMMENTARY_source, GENE_COMMENTARY_source_E);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> genomic_coords, (AsnWriteFunc) SeqLocAsnWrite, aip, GENE_COMMENTARY_genomic_coords, COMMENTARY_genomic_coords_E);
   AsnGenericChoiceSeqOfAsnWrite(ptr -> seqs, (AsnWriteFunc) SeqLocAsnWrite, aip, GENE_COMMENTARY_seqs, GENE_COMMENTARY_seqs_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> products, (AsnWriteFunc) GeneCommentaryAsnWrite, aip, GENE_COMMENTARY_products, GENE_COMMENTARY_products_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> properties, (AsnWriteFunc) GeneCommentaryAsnWrite, aip, GENE_COMMENTARY_properties, GENE_COMMENTARY_properties_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> comment, (AsnWriteFunc) GeneCommentaryAsnWrite, aip, GENE_COMMENTARY_comment, GENE_COMMENTARY_comment_E);
   if (ptr -> create_date != NULL) {
      if ( ! DateAsnWrite(ptr -> create_date, aip, GENE_COMMENTARY_create_date)) {
         goto erret;
      }
   }
   if (ptr -> update_date != NULL) {
      if ( ! DateAsnWrite(ptr -> update_date, aip, GENE_COMMENTARY_update_date)) {
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
*    XtraTermsNew()
*
**************************************************/
NLM_EXTERN 
XtraTermsPtr LIBCALL
XtraTermsNew(void)
{
   XtraTermsPtr ptr = MemNew((size_t) sizeof(XtraTerms));

   return ptr;

}


/**************************************************
*
*    XtraTermsFree()
*
**************************************************/
NLM_EXTERN 
XtraTermsPtr LIBCALL
XtraTermsFree(XtraTermsPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> tag);
   MemFree(ptr -> value);
   return MemFree(ptr);
}


/**************************************************
*
*    XtraTermsAsnRead()
*
**************************************************/
NLM_EXTERN 
XtraTermsPtr LIBCALL
XtraTermsAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   XtraTermsPtr ptr;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* XtraTerms ::= (self contained) */
      atp = AsnReadId(aip, amp, XTRA_TERMS);
   } else {
      atp = AsnLinkType(orig, XTRA_TERMS);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = XtraTermsNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == XTRA_TERMS_tag) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> tag = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == XTRA_TERMS_value) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> value = av.ptrvalue;
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
   ptr = XtraTermsFree(ptr);
   goto ret;
}



/**************************************************
*
*    XtraTermsAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
XtraTermsAsnWrite(XtraTermsPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, XTRA_TERMS);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> tag != NULL) {
      av.ptrvalue = ptr -> tag;
      retval = AsnWrite(aip, XTRA_TERMS_tag,  &av);
   }
   if (ptr -> value != NULL) {
      av.ptrvalue = ptr -> value;
      retval = AsnWrite(aip, XTRA_TERMS_value,  &av);
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
*    OtherSourceNew()
*
**************************************************/
NLM_EXTERN 
OtherSourcePtr LIBCALL
OtherSourceNew(void)
{
   OtherSourcePtr ptr = MemNew((size_t) sizeof(OtherSource));

   return ptr;

}


/**************************************************
*
*    OtherSourceFree()
*
**************************************************/
NLM_EXTERN 
OtherSourcePtr LIBCALL
OtherSourceFree(OtherSourcePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   DbtagFree(ptr -> src);
   MemFree(ptr -> pre_text);
   MemFree(ptr -> anchor);
   MemFree(ptr -> url);
   MemFree(ptr -> post_text);
   return MemFree(ptr);
}


/**************************************************
*
*    OtherSourceAsnRead()
*
**************************************************/
NLM_EXTERN 
OtherSourcePtr LIBCALL
OtherSourceAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   OtherSourcePtr ptr;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* OtherSource ::= (self contained) */
      atp = AsnReadId(aip, amp, OTHER_SOURCE);
   } else {
      atp = AsnLinkType(orig, OTHER_SOURCE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = OtherSourceNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == OTHER_SOURCE_src) {
      ptr -> src = DbtagAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == OTHER_SOURCE_pre_text) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pre_text = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == OTHER_SOURCE_anchor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> anchor = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == OTHER_SOURCE_url) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> url = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == OTHER_SOURCE_post_text) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> post_text = av.ptrvalue;
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
   ptr = OtherSourceFree(ptr);
   goto ret;
}



/**************************************************
*
*    OtherSourceAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
OtherSourceAsnWrite(OtherSourcePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objentgeneAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, OTHER_SOURCE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> src != NULL) {
      if ( ! DbtagAsnWrite(ptr -> src, aip, OTHER_SOURCE_src)) {
         goto erret;
      }
   }
   if (ptr -> pre_text != NULL) {
      av.ptrvalue = ptr -> pre_text;
      retval = AsnWrite(aip, OTHER_SOURCE_pre_text,  &av);
   }
   if (ptr -> anchor != NULL) {
      av.ptrvalue = ptr -> anchor;
      retval = AsnWrite(aip, OTHER_SOURCE_anchor,  &av);
   }
   if (ptr -> url != NULL) {
      av.ptrvalue = ptr -> url;
      retval = AsnWrite(aip, OTHER_SOURCE_url,  &av);
   }
   if (ptr -> post_text != NULL) {
      av.ptrvalue = ptr -> post_text;
      retval = AsnWrite(aip, OTHER_SOURCE_post_text,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

