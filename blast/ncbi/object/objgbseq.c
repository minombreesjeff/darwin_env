#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <objgbseq.h>

static Boolean loaded = FALSE;

#include <asngbseq.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objgbseqAsnLoad(void)
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
*    Generated object loaders for Module NCBI-GBSeq
*    Generated using ASNCODE Revision: 6.14 at Apr 9, 2004 11:58 AM
*
**************************************************/


/**************************************************
*
*    GBSeqNew()
*
**************************************************/
NLM_EXTERN 
GBSeqPtr LIBCALL
GBSeqNew(void)
{
   GBSeqPtr ptr = MemNew((size_t) sizeof(GBSeq));

   ptr -> strandedness = 0;
   ptr -> moltype = 0;
   ptr -> topology = 1;
   return ptr;

}


/**************************************************
*
*    GBSeqFree()
*
**************************************************/
NLM_EXTERN 
GBSeqPtr LIBCALL
GBSeqFree(GBSeqPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> locus);
   MemFree(ptr -> division);
   MemFree(ptr -> update_date);
   MemFree(ptr -> create_date);
   MemFree(ptr -> update_release);
   MemFree(ptr -> create_release);
   MemFree(ptr -> definition);
   MemFree(ptr -> primary_accession);
   MemFree(ptr -> entry_version);
   MemFree(ptr -> accession_version);
   AsnGenericBaseSeqOfFree(ptr -> other_seqids ,ASNCODE_PTRVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> secondary_accessions ,ASNCODE_PTRVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> keywords ,ASNCODE_PTRVAL_SLOT);
   MemFree(ptr -> segment);
   MemFree(ptr -> source);
   MemFree(ptr -> organism);
   MemFree(ptr -> taxonomy);
   AsnGenericUserSeqOfFree(ptr -> references, (AsnOptFreeFunc) GBReferenceFree);
   MemFree(ptr -> comment);
   MemFree(ptr -> primary);
   MemFree(ptr -> source_db);
   MemFree(ptr -> database_reference);
   AsnGenericUserSeqOfFree(ptr -> feature_table, (AsnOptFreeFunc) GBFeatureFree);
   MemFree(ptr -> sequence);
   MemFree(ptr -> contig);
   return MemFree(ptr);
}


/**************************************************
*
*    GBSeqAsnRead()
*
**************************************************/
NLM_EXTERN 
GBSeqPtr LIBCALL
GBSeqAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GBSeqPtr ptr;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GBSeq ::= (self contained) */
      atp = AsnReadId(aip, amp, GBSEQ);
   } else {
      atp = AsnLinkType(orig, GBSEQ);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GBSeqNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GBSEQ_locus) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> locus = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_length) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> length = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_strandedness) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> strandedness = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_moltype) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> moltype = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_topology) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> topology = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_division) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> division = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_update_date) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> update_date = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_create_date) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> create_date = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_update_release) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> update_release = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_create_release) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> create_release = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_definition) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> definition = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_primary_accession) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> primary_accession = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_entry_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> entry_version = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_accession_version) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> accession_version = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_other_seqids) {
      ptr -> other_seqids = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> other_seqids == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_secondary_accessions) {
      ptr -> secondary_accessions = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> secondary_accessions == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_keywords) {
      ptr -> keywords = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> keywords == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_segment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> segment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_source) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> source = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_organism) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> organism = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_taxonomy) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> taxonomy = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_references) {
      ptr -> references = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GBReferenceAsnRead, (AsnOptFreeFunc) GBReferenceFree);
      if (isError && ptr -> references == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_comment) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> comment = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_primary) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> primary = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_source_db) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> source_db = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_database_reference) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> database_reference = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_feature_table) {
      ptr -> feature_table = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GBFeatureAsnRead, (AsnOptFreeFunc) GBFeatureFree);
      if (isError && ptr -> feature_table == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_sequence) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> sequence = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBSEQ_contig) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> contig = av.ptrvalue;
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
   ptr = GBSeqFree(ptr);
   goto ret;
}



/**************************************************
*
*    GBSeqAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GBSeqAsnWrite(GBSeqPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GBSEQ);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> locus != NULL) {
      av.ptrvalue = ptr -> locus;
      retval = AsnWrite(aip, GBSEQ_locus,  &av);
   }
   av.intvalue = ptr -> length;
   retval = AsnWrite(aip, GBSEQ_length,  &av);
   av.intvalue = ptr -> strandedness;
   retval = AsnWrite(aip, GBSEQ_strandedness,  &av);
   av.intvalue = ptr -> moltype;
   retval = AsnWrite(aip, GBSEQ_moltype,  &av);
   av.intvalue = ptr -> topology;
   retval = AsnWrite(aip, GBSEQ_topology,  &av);
   if (ptr -> division != NULL) {
      av.ptrvalue = ptr -> division;
      retval = AsnWrite(aip, GBSEQ_division,  &av);
   }
   if (ptr -> update_date != NULL) {
      av.ptrvalue = ptr -> update_date;
      retval = AsnWrite(aip, GBSEQ_update_date,  &av);
   }
   if (ptr -> create_date != NULL) {
      av.ptrvalue = ptr -> create_date;
      retval = AsnWrite(aip, GBSEQ_create_date,  &av);
   }
   if (ptr -> update_release != NULL) {
      av.ptrvalue = ptr -> update_release;
      retval = AsnWrite(aip, GBSEQ_update_release,  &av);
   }
   if (ptr -> create_release != NULL) {
      av.ptrvalue = ptr -> create_release;
      retval = AsnWrite(aip, GBSEQ_create_release,  &av);
   }
   if (ptr -> definition != NULL) {
      av.ptrvalue = ptr -> definition;
      retval = AsnWrite(aip, GBSEQ_definition,  &av);
   }
   if (ptr -> primary_accession != NULL) {
      av.ptrvalue = ptr -> primary_accession;
      retval = AsnWrite(aip, GBSEQ_primary_accession,  &av);
   }
   if (ptr -> entry_version != NULL) {
      av.ptrvalue = ptr -> entry_version;
      retval = AsnWrite(aip, GBSEQ_entry_version,  &av);
   }
   if (ptr -> accession_version != NULL) {
      av.ptrvalue = ptr -> accession_version;
      retval = AsnWrite(aip, GBSEQ_accession_version,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> other_seqids ,ASNCODE_PTRVAL_SLOT, aip, GBSEQ_other_seqids, GBSEQ_other_seqids_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> secondary_accessions ,ASNCODE_PTRVAL_SLOT, aip, GBSEQ_secondary_accessions, GBSEQ_secondary_accessions_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> keywords ,ASNCODE_PTRVAL_SLOT, aip, GBSEQ_keywords, GBSEQ_keywords_E);
   if (ptr -> segment != NULL) {
      av.ptrvalue = ptr -> segment;
      retval = AsnWrite(aip, GBSEQ_segment,  &av);
   }
   if (ptr -> source != NULL) {
      av.ptrvalue = ptr -> source;
      retval = AsnWrite(aip, GBSEQ_source,  &av);
   }
   if (ptr -> organism != NULL) {
      av.ptrvalue = ptr -> organism;
      retval = AsnWrite(aip, GBSEQ_organism,  &av);
   }
   if (ptr -> taxonomy != NULL) {
      av.ptrvalue = ptr -> taxonomy;
      retval = AsnWrite(aip, GBSEQ_taxonomy,  &av);
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> references, (AsnWriteFunc) GBReferenceAsnWrite, aip, GBSEQ_references, GBSEQ_references_E);
   if (ptr -> comment != NULL) {
      av.ptrvalue = ptr -> comment;
      retval = AsnWrite(aip, GBSEQ_comment,  &av);
   }
   if (ptr -> primary != NULL) {
      av.ptrvalue = ptr -> primary;
      retval = AsnWrite(aip, GBSEQ_primary,  &av);
   }
   if (ptr -> source_db != NULL) {
      av.ptrvalue = ptr -> source_db;
      retval = AsnWrite(aip, GBSEQ_source_db,  &av);
   }
   if (ptr -> database_reference != NULL) {
      av.ptrvalue = ptr -> database_reference;
      retval = AsnWrite(aip, GBSEQ_database_reference,  &av);
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> feature_table, (AsnWriteFunc) GBFeatureAsnWrite, aip, GBSEQ_feature_table, GBSEQ_feature_table_E);
   if (ptr -> sequence != NULL) {
      av.ptrvalue = ptr -> sequence;
      retval = AsnWrite(aip, GBSEQ_sequence,  &av);
   }
   if (ptr -> contig != NULL) {
      av.ptrvalue = ptr -> contig;
      retval = AsnWrite(aip, GBSEQ_contig,  &av);
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
*    GBReferenceNew()
*
**************************************************/
NLM_EXTERN 
GBReferencePtr LIBCALL
GBReferenceNew(void)
{
   GBReferencePtr ptr = MemNew((size_t) sizeof(GBReference));

   return ptr;

}


/**************************************************
*
*    GBReferenceFree()
*
**************************************************/
NLM_EXTERN 
GBReferencePtr LIBCALL
GBReferenceFree(GBReferencePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> reference);
   AsnGenericBaseSeqOfFree(ptr -> authors ,ASNCODE_PTRVAL_SLOT);
   MemFree(ptr -> consortium);
   MemFree(ptr -> title);
   MemFree(ptr -> journal);
   MemFree(ptr -> remark);
   return MemFree(ptr);
}


/**************************************************
*
*    GBReferenceAsnRead()
*
**************************************************/
NLM_EXTERN 
GBReferencePtr LIBCALL
GBReferenceAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GBReferencePtr ptr;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GBReference ::= (self contained) */
      atp = AsnReadId(aip, amp, GBREFERENCE);
   } else {
      atp = AsnLinkType(orig, GBREFERENCE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GBReferenceNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GBREFERENCE_reference) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> reference = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBREFERENCE_authors) {
      ptr -> authors = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> authors == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBREFERENCE_consortium) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> consortium = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBREFERENCE_title) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> title = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBREFERENCE_journal) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> journal = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBREFERENCE_medline) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> medline = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBREFERENCE_pubmed) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pubmed = av.intvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBREFERENCE_remark) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> remark = av.ptrvalue;
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
   ptr = GBReferenceFree(ptr);
   goto ret;
}



/**************************************************
*
*    GBReferenceAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GBReferenceAsnWrite(GBReferencePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GBREFERENCE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> reference != NULL) {
      av.ptrvalue = ptr -> reference;
      retval = AsnWrite(aip, GBREFERENCE_reference,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> authors ,ASNCODE_PTRVAL_SLOT, aip, GBREFERENCE_authors, GBREFERENCE_authors_E);
   if (ptr -> consortium != NULL) {
      av.ptrvalue = ptr -> consortium;
      retval = AsnWrite(aip, GBREFERENCE_consortium,  &av);
   }
   if (ptr -> title != NULL) {
      av.ptrvalue = ptr -> title;
      retval = AsnWrite(aip, GBREFERENCE_title,  &av);
   }
   if (ptr -> journal != NULL) {
      av.ptrvalue = ptr -> journal;
      retval = AsnWrite(aip, GBREFERENCE_journal,  &av);
   }
   if (ptr -> medline || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> medline;
      retval = AsnWrite(aip, GBREFERENCE_medline,  &av);
   }
   if (ptr -> pubmed || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> pubmed;
      retval = AsnWrite(aip, GBREFERENCE_pubmed,  &av);
   }
   if (ptr -> remark != NULL) {
      av.ptrvalue = ptr -> remark;
      retval = AsnWrite(aip, GBREFERENCE_remark,  &av);
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
*    GBFeatureNew()
*
**************************************************/
NLM_EXTERN 
GBFeaturePtr LIBCALL
GBFeatureNew(void)
{
   GBFeaturePtr ptr = MemNew((size_t) sizeof(GBFeature));

   return ptr;

}


/**************************************************
*
*    GBFeatureFree()
*
**************************************************/
NLM_EXTERN 
GBFeaturePtr LIBCALL
GBFeatureFree(GBFeaturePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> key);
   MemFree(ptr -> location);
   AsnGenericUserSeqOfFree(ptr -> intervals, (AsnOptFreeFunc) GBIntervalFree);
   AsnGenericUserSeqOfFree(ptr -> quals, (AsnOptFreeFunc) GBQualifierFree);
   return MemFree(ptr);
}


/**************************************************
*
*    GBFeatureAsnRead()
*
**************************************************/
NLM_EXTERN 
GBFeaturePtr LIBCALL
GBFeatureAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GBFeaturePtr ptr;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GBFeature ::= (self contained) */
      atp = AsnReadId(aip, amp, GBFEATURE);
   } else {
      atp = AsnLinkType(orig, GBFEATURE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GBFeatureNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GBFEATURE_key) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> key = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBFEATURE_location) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> location = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBFEATURE_intervals) {
      ptr -> intervals = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GBIntervalAsnRead, (AsnOptFreeFunc) GBIntervalFree);
      if (isError && ptr -> intervals == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBFEATURE_quals) {
      ptr -> quals = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GBQualifierAsnRead, (AsnOptFreeFunc) GBQualifierFree);
      if (isError && ptr -> quals == NULL) {
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
   ptr = GBFeatureFree(ptr);
   goto ret;
}



/**************************************************
*
*    GBFeatureAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GBFeatureAsnWrite(GBFeaturePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GBFEATURE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> key != NULL) {
      av.ptrvalue = ptr -> key;
      retval = AsnWrite(aip, GBFEATURE_key,  &av);
   }
   if (ptr -> location != NULL) {
      av.ptrvalue = ptr -> location;
      retval = AsnWrite(aip, GBFEATURE_location,  &av);
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> intervals, (AsnWriteFunc) GBIntervalAsnWrite, aip, GBFEATURE_intervals, GBFEATURE_intervals_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> quals, (AsnWriteFunc) GBQualifierAsnWrite, aip, GBFEATURE_quals, GBFEATURE_quals_E);
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
*    GBIntervalNew()
*
**************************************************/
NLM_EXTERN 
GBIntervalPtr LIBCALL
GBIntervalNew(void)
{
   GBIntervalPtr ptr = MemNew((size_t) sizeof(GBInterval));

   return ptr;

}


/**************************************************
*
*    GBIntervalFree()
*
**************************************************/
NLM_EXTERN 
GBIntervalPtr LIBCALL
GBIntervalFree(GBIntervalPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> accession);
   return MemFree(ptr);
}


/**************************************************
*
*    GBIntervalAsnRead()
*
**************************************************/
NLM_EXTERN 
GBIntervalPtr LIBCALL
GBIntervalAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GBIntervalPtr ptr;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GBInterval ::= (self contained) */
      atp = AsnReadId(aip, amp, GBINTERVAL);
   } else {
      atp = AsnLinkType(orig, GBINTERVAL);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GBIntervalNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GBINTERVAL_from) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> from = av.intvalue;
      ptr -> OBbits__ |= 1<<0;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBINTERVAL_to) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> to = av.intvalue;
      ptr -> OBbits__ |= 1<<1;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBINTERVAL_point) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> point = av.intvalue;
      ptr -> OBbits__ |= 1<<2;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBINTERVAL_accession) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> accession = av.ptrvalue;
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
   ptr = GBIntervalFree(ptr);
   goto ret;
}



/**************************************************
*
*    GBIntervalAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GBIntervalAsnWrite(GBIntervalPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GBINTERVAL);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> from || (ptr -> OBbits__ & (1<<0) )){   av.intvalue = ptr -> from;
      retval = AsnWrite(aip, GBINTERVAL_from,  &av);
   }
   if (ptr -> to || (ptr -> OBbits__ & (1<<1) )){   av.intvalue = ptr -> to;
      retval = AsnWrite(aip, GBINTERVAL_to,  &av);
   }
   if (ptr -> point || (ptr -> OBbits__ & (1<<2) )){   av.intvalue = ptr -> point;
      retval = AsnWrite(aip, GBINTERVAL_point,  &av);
   }
   if (ptr -> accession != NULL) {
      av.ptrvalue = ptr -> accession;
      retval = AsnWrite(aip, GBINTERVAL_accession,  &av);
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
*    GBQualifierNew()
*
**************************************************/
NLM_EXTERN 
GBQualifierPtr LIBCALL
GBQualifierNew(void)
{
   GBQualifierPtr ptr = MemNew((size_t) sizeof(GBQualifier));

   return ptr;

}


/**************************************************
*
*    GBQualifierFree()
*
**************************************************/
NLM_EXTERN 
GBQualifierPtr LIBCALL
GBQualifierFree(GBQualifierPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   MemFree(ptr -> value);
   return MemFree(ptr);
}


/**************************************************
*
*    GBQualifierAsnRead()
*
**************************************************/
NLM_EXTERN 
GBQualifierPtr LIBCALL
GBQualifierAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GBQualifierPtr ptr;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GBQualifier ::= (self contained) */
      atp = AsnReadId(aip, amp, GBQUALIFIER);
   } else {
      atp = AsnLinkType(orig, GBQUALIFIER);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = GBQualifierNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == GBQUALIFIER_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == GBQUALIFIER_value) {
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
   ptr = GBQualifierFree(ptr);
   goto ret;
}



/**************************************************
*
*    GBQualifierAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GBQualifierAsnWrite(GBQualifierPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GBQUALIFIER);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, GBQUALIFIER_name,  &av);
   }
   if (ptr -> value != NULL) {
      av.ptrvalue = ptr -> value;
      retval = AsnWrite(aip, GBQUALIFIER_value,  &av);
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
*    GBSetFree()
*
**************************************************/
NLM_EXTERN 
GBSetPtr LIBCALL
GBSetFree(GBSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr,  (AsnOptFreeFunc) GBSeqFree);
   return NULL;
}


/**************************************************
*
*    GBSetAsnRead()
*
**************************************************/
NLM_EXTERN 
GBSetPtr LIBCALL
GBSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   GBSetPtr ptr;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* GBSet ::= (self contained) */
      atp = AsnReadId(aip, amp, GBSET);
   } else {
      atp = AsnLinkType(orig, GBSET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   func = NULL;

   ptr  = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) GBSeqAsnRead, (AsnOptFreeFunc) GBSeqFree);
   if (isError && ptr  == NULL) {
      goto erret;
   }



ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return ptr;

erret:
   aip -> io_failure = TRUE;
   ptr = GBSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    GBSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
GBSetAsnWrite(GBSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objgbseqAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, GBSET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   retval = AsnGenericUserSeqOfAsnWrite(ptr , (AsnWriteFunc) GBSeqAsnWrite, aip, atp, GBSET_E);
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

