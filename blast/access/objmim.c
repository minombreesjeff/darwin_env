#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include <objmim.h>

static Boolean loaded = FALSE;

#include <asnmim.h>

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

NLM_EXTERN Boolean LIBCALL
objmimAsnLoad(void)
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
*    Generated object loaders for Module NCBI-Mim
*    Generated using ASNCODE Revision: 6.9 at Aug 16, 2000  6:14 PM
*
**************************************************/


/**************************************************
*
*    MimSetNew()
*
**************************************************/
NLM_EXTERN 
MimSetPtr LIBCALL
MimSetNew(void)
{
   MimSetPtr ptr = MemNew((size_t) sizeof(MimSet));

   return ptr;

}


/**************************************************
*
*    MimSetFree()
*
**************************************************/
NLM_EXTERN 
MimSetPtr LIBCALL
MimSetFree(MimSetPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MimDateFree(ptr -> releaseDate);
   AsnGenericUserSeqOfFree(ptr -> mimEntries, (AsnOptFreeFunc) MimEntryFree);
   return MemFree(ptr);
}


/**************************************************
*
*    MimSetAsnRead()
*
**************************************************/
NLM_EXTERN 
MimSetPtr LIBCALL
MimSetAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimSetPtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimSet ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_SET);
   } else {
      atp = AsnLinkType(orig, MIM_SET);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimSetNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_SET_releaseDate) {
      ptr -> releaseDate = MimDateAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_SET_mimEntries) {
      ptr -> mimEntries = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimEntryAsnRead, (AsnOptFreeFunc) MimEntryFree);
      if (isError && ptr -> mimEntries == NULL) {
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
   ptr = MimSetFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimSetAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimSetAsnWrite(MimSetPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_SET);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> releaseDate != NULL) {
      if ( ! MimDateAsnWrite(ptr -> releaseDate, aip, MIM_SET_releaseDate)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> mimEntries, (AsnWriteFunc) MimEntryAsnWrite, aip, MIM_SET_mimEntries, MIM_SET_mimEntries_E);
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
*    MimDateNew()
*
**************************************************/
NLM_EXTERN 
MimDatePtr LIBCALL
MimDateNew(void)
{
   MimDatePtr ptr = MemNew((size_t) sizeof(MimDate));

   return ptr;

}


/**************************************************
*
*    MimDateFree()
*
**************************************************/
NLM_EXTERN 
MimDatePtr LIBCALL
MimDateFree(MimDatePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   return MemFree(ptr);
}


/**************************************************
*
*    MimDateAsnRead()
*
**************************************************/
NLM_EXTERN 
MimDatePtr LIBCALL
MimDateAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimDatePtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimDate ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_DATE);
   } else {
      atp = AsnLinkType(orig, MIM_DATE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimDateNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_DATE_year) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> year = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_DATE_month) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> month = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_DATE_day) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> day = av.intvalue;
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
   ptr = MimDateFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimDateAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimDateAsnWrite(MimDatePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_DATE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> year;
   retval = AsnWrite(aip, MIM_DATE_year,  &av);
   av.intvalue = ptr -> month;
   retval = AsnWrite(aip, MIM_DATE_month,  &av);
   av.intvalue = ptr -> day;
   retval = AsnWrite(aip, MIM_DATE_day,  &av);
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
*    MimEntryNew()
*
**************************************************/
NLM_EXTERN 
MimEntryPtr LIBCALL
MimEntryNew(void)
{
   MimEntryPtr ptr = MemNew((size_t) sizeof(MimEntry));

   return ptr;

}


/**************************************************
*
*    MimEntryFree()
*
**************************************************/
NLM_EXTERN 
MimEntryPtr LIBCALL
MimEntryFree(MimEntryPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> mimNumber);
   MemFree(ptr -> title);
   MemFree(ptr -> copyright);
   MemFree(ptr -> symbol);
   MemFree(ptr -> locus);
   AsnGenericBaseSeqOfFree(ptr -> synonyms ,ASNCODE_PTRVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> aliases ,ASNCODE_PTRVAL_SLOT);
   AsnGenericBaseSeqOfFree(ptr -> included ,ASNCODE_PTRVAL_SLOT);
   AsnGenericUserSeqOfFree(ptr -> seeAlso, (AsnOptFreeFunc) MimCitFree);
   AsnGenericUserSeqOfFree(ptr -> text, (AsnOptFreeFunc) MimTextFree);
   AsnGenericUserSeqOfFree(ptr -> textfields, (AsnOptFreeFunc) MimTextFree);
   AsnGenericUserSeqOfFree(ptr -> summary, (AsnOptFreeFunc) MimTextFree);
   AsnGenericUserSeqOfFree(ptr -> summaryAttribution, (AsnOptFreeFunc) MimEditItemFree);
   AsnGenericUserSeqOfFree(ptr -> summaryEditHistory, (AsnOptFreeFunc) MimEditItemFree);
   MimEditItemFree(ptr -> summaryCreationDate);
   AsnGenericUserSeqOfFree(ptr -> allelicVariants, (AsnOptFreeFunc) MimAllelicVariantFree);
   AsnGenericUserSeqOfFree(ptr -> clinicalSynopsis, (AsnOptFreeFunc) MimIndexTermFree);
   AsnGenericUserSeqOfFree(ptr -> synopsisAttribution, (AsnOptFreeFunc) MimEditItemFree);
   AsnGenericUserSeqOfFree(ptr -> synopsisEditHistory, (AsnOptFreeFunc) MimEditItemFree);
   MimEditItemFree(ptr -> synopsisCreationDate);
   AsnGenericUserSeqOfFree(ptr -> editHistory, (AsnOptFreeFunc) MimEditItemFree);
   MimEditItemFree(ptr -> creationDate);
   AsnGenericUserSeqOfFree(ptr -> references, (AsnOptFreeFunc) MimReferenceFree);
   AsnGenericUserSeqOfFree(ptr -> attribution, (AsnOptFreeFunc) MimEditItemFree);
   MimLinkFree(ptr -> medlineLinks);
   MimLinkFree(ptr -> proteinLinks);
   MimLinkFree(ptr -> nucleotideLinks);
   MimLinkFree(ptr -> structureLinks);
   MimLinkFree(ptr -> genomeLinks);
   return MemFree(ptr);
}


/**************************************************
*
*    MimEntryAsnRead()
*
**************************************************/
NLM_EXTERN 
MimEntryPtr LIBCALL
MimEntryAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimEntryPtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimEntry ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_ENTRY);
   } else {
      atp = AsnLinkType(orig, MIM_ENTRY);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimEntryNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_ENTRY_mimNumber) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> mimNumber = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_mimType) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> mimType = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_title) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> title = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_copyright) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> copyright = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_symbol) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> symbol = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_locus) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> locus = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_synonyms) {
      ptr -> synonyms = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> synonyms == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_aliases) {
      ptr -> aliases = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> aliases == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_included) {
      ptr -> included = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> included == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_seeAlso) {
      ptr -> seeAlso = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimCitAsnRead, (AsnOptFreeFunc) MimCitFree);
      if (isError && ptr -> seeAlso == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_text) {
      ptr -> text = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimTextAsnRead, (AsnOptFreeFunc) MimTextFree);
      if (isError && ptr -> text == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_textfields) {
      ptr -> textfields = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimTextAsnRead, (AsnOptFreeFunc) MimTextFree);
      if (isError && ptr -> textfields == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_hasSummary) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hasSummary = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_summary) {
      ptr -> summary = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimTextAsnRead, (AsnOptFreeFunc) MimTextFree);
      if (isError && ptr -> summary == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_summaryAttribution) {
      ptr -> summaryAttribution = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimEditItemAsnRead, (AsnOptFreeFunc) MimEditItemFree);
      if (isError && ptr -> summaryAttribution == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_summaryEditHistory) {
      ptr -> summaryEditHistory = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimEditItemAsnRead, (AsnOptFreeFunc) MimEditItemFree);
      if (isError && ptr -> summaryEditHistory == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_summaryCreationDate) {
      ptr -> summaryCreationDate = MimEditItemAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_allelicVariants) {
      ptr -> allelicVariants = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimAllelicVariantAsnRead, (AsnOptFreeFunc) MimAllelicVariantFree);
      if (isError && ptr -> allelicVariants == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_hasSynopsis) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> hasSynopsis = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_clinicalSynopsis) {
      ptr -> clinicalSynopsis = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimIndexTermAsnRead, (AsnOptFreeFunc) MimIndexTermFree);
      if (isError && ptr -> clinicalSynopsis == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_synopsisAttribution) {
      ptr -> synopsisAttribution = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimEditItemAsnRead, (AsnOptFreeFunc) MimEditItemFree);
      if (isError && ptr -> synopsisAttribution == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_synopsisEditHistory) {
      ptr -> synopsisEditHistory = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimEditItemAsnRead, (AsnOptFreeFunc) MimEditItemFree);
      if (isError && ptr -> synopsisEditHistory == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_synopsisCreationDate) {
      ptr -> synopsisCreationDate = MimEditItemAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_editHistory) {
      ptr -> editHistory = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimEditItemAsnRead, (AsnOptFreeFunc) MimEditItemFree);
      if (isError && ptr -> editHistory == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_creationDate) {
      ptr -> creationDate = MimEditItemAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_references) {
      ptr -> references = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimReferenceAsnRead, (AsnOptFreeFunc) MimReferenceFree);
      if (isError && ptr -> references == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_attribution) {
      ptr -> attribution = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimEditItemAsnRead, (AsnOptFreeFunc) MimEditItemFree);
      if (isError && ptr -> attribution == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_numGeneMaps) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> numGeneMaps = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_medlineLinks) {
      ptr -> medlineLinks = MimLinkAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_proteinLinks) {
      ptr -> proteinLinks = MimLinkAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_nucleotideLinks) {
      ptr -> nucleotideLinks = MimLinkAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_structureLinks) {
      ptr -> structureLinks = MimLinkAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ENTRY_genomeLinks) {
      ptr -> genomeLinks = MimLinkAsnRead(aip, atp);
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
   ptr = MimEntryFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimEntryAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimEntryAsnWrite(MimEntryPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_ENTRY);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> mimNumber != NULL) {
      av.ptrvalue = ptr -> mimNumber;
      retval = AsnWrite(aip, MIM_ENTRY_mimNumber,  &av);
   }
   av.intvalue = ptr -> mimType;
   retval = AsnWrite(aip, MIM_ENTRY_mimType,  &av);
   if (ptr -> title != NULL) {
      av.ptrvalue = ptr -> title;
      retval = AsnWrite(aip, MIM_ENTRY_title,  &av);
   }
   if (ptr -> copyright != NULL) {
      av.ptrvalue = ptr -> copyright;
      retval = AsnWrite(aip, MIM_ENTRY_copyright,  &av);
   }
   if (ptr -> symbol != NULL) {
      av.ptrvalue = ptr -> symbol;
      retval = AsnWrite(aip, MIM_ENTRY_symbol,  &av);
   }
   if (ptr -> locus != NULL) {
      av.ptrvalue = ptr -> locus;
      retval = AsnWrite(aip, MIM_ENTRY_locus,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> synonyms ,ASNCODE_PTRVAL_SLOT, aip, MIM_ENTRY_synonyms, MIM_ENTRY_synonyms_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> aliases ,ASNCODE_PTRVAL_SLOT, aip, MIM_ENTRY_aliases, MIM_ENTRY_aliases_E);
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> included ,ASNCODE_PTRVAL_SLOT, aip, MIM_ENTRY_included, MIM_ENTRY_included_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> seeAlso, (AsnWriteFunc) MimCitAsnWrite, aip, MIM_ENTRY_seeAlso, MIM_ENTRY_seeAlso_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> text, (AsnWriteFunc) MimTextAsnWrite, aip, MIM_ENTRY_text, MIM_ENTRY_text_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> textfields, (AsnWriteFunc) MimTextAsnWrite, aip, MIM_ENTRY_textfields, MIM_ENTRY_textfields_E);
   av.boolvalue = ptr -> hasSummary;
   retval = AsnWrite(aip, MIM_ENTRY_hasSummary,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> summary, (AsnWriteFunc) MimTextAsnWrite, aip, MIM_ENTRY_summary, MIM_ENTRY_summary_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> summaryAttribution, (AsnWriteFunc) MimEditItemAsnWrite, aip, MIM_ENTRY_summaryAttribution, MIM_ENTRY_summaryAttribution_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> summaryEditHistory, (AsnWriteFunc) MimEditItemAsnWrite, aip, MIM_ENTRY_summaryEditHistory, MIM_ENTRY_summaryEditHistory_E);
   if (ptr -> summaryCreationDate != NULL) {
      if ( ! MimEditItemAsnWrite(ptr -> summaryCreationDate, aip, MIM_ENTRY_summaryCreationDate)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> allelicVariants, (AsnWriteFunc) MimAllelicVariantAsnWrite, aip, MIM_ENTRY_allelicVariants, MIM_ENTRY_allelicVariants_E);
   av.boolvalue = ptr -> hasSynopsis;
   retval = AsnWrite(aip, MIM_ENTRY_hasSynopsis,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> clinicalSynopsis, (AsnWriteFunc) MimIndexTermAsnWrite, aip, MIM_ENTRY_clinicalSynopsis, MIM_ENTRY_clinicalSynopsis_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> synopsisAttribution, (AsnWriteFunc) MimEditItemAsnWrite, aip, MIM_ENTRY_synopsisAttribution, MIM_ENTRY_synopsisAttribution_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> synopsisEditHistory, (AsnWriteFunc) MimEditItemAsnWrite, aip, MIM_ENTRY_synopsisEditHistory, MIM_ENTRY_synopsisEditHistory_E);
   if (ptr -> synopsisCreationDate != NULL) {
      if ( ! MimEditItemAsnWrite(ptr -> synopsisCreationDate, aip, MIM_ENTRY_synopsisCreationDate)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> editHistory, (AsnWriteFunc) MimEditItemAsnWrite, aip, MIM_ENTRY_editHistory, MIM_ENTRY_editHistory_E);
   if (ptr -> creationDate != NULL) {
      if ( ! MimEditItemAsnWrite(ptr -> creationDate, aip, MIM_ENTRY_creationDate)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> references, (AsnWriteFunc) MimReferenceAsnWrite, aip, MIM_ENTRY_references, MIM_ENTRY_references_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> attribution, (AsnWriteFunc) MimEditItemAsnWrite, aip, MIM_ENTRY_attribution, MIM_ENTRY_attribution_E);
   av.intvalue = ptr -> numGeneMaps;
   retval = AsnWrite(aip, MIM_ENTRY_numGeneMaps,  &av);
   if (ptr -> medlineLinks != NULL) {
      if ( ! MimLinkAsnWrite(ptr -> medlineLinks, aip, MIM_ENTRY_medlineLinks)) {
         goto erret;
      }
   }
   if (ptr -> proteinLinks != NULL) {
      if ( ! MimLinkAsnWrite(ptr -> proteinLinks, aip, MIM_ENTRY_proteinLinks)) {
         goto erret;
      }
   }
   if (ptr -> nucleotideLinks != NULL) {
      if ( ! MimLinkAsnWrite(ptr -> nucleotideLinks, aip, MIM_ENTRY_nucleotideLinks)) {
         goto erret;
      }
   }
   if (ptr -> structureLinks != NULL) {
      if ( ! MimLinkAsnWrite(ptr -> structureLinks, aip, MIM_ENTRY_structureLinks)) {
         goto erret;
      }
   }
   if (ptr -> genomeLinks != NULL) {
      if ( ! MimLinkAsnWrite(ptr -> genomeLinks, aip, MIM_ENTRY_genomeLinks)) {
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
*    MimCitNew()
*
**************************************************/
NLM_EXTERN 
MimCitPtr LIBCALL
MimCitNew(void)
{
   MimCitPtr ptr = MemNew((size_t) sizeof(MimCit));

   return ptr;

}


/**************************************************
*
*    MimCitFree()
*
**************************************************/
NLM_EXTERN 
MimCitPtr LIBCALL
MimCitFree(MimCitPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> author);
   MemFree(ptr -> others);
   return MemFree(ptr);
}


/**************************************************
*
*    MimCitAsnRead()
*
**************************************************/
NLM_EXTERN 
MimCitPtr LIBCALL
MimCitAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimCitPtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimCit ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_CIT);
   } else {
      atp = AsnLinkType(orig, MIM_CIT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimCitNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_CIT_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_CIT_author) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> author = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_CIT_others) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> others = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_CIT_year) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> year = av.intvalue;
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
   ptr = MimCitFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimCitAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimCitAsnWrite(MimCitPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_CIT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> number;
   retval = AsnWrite(aip, MIM_CIT_number,  &av);
   if (ptr -> author != NULL) {
      av.ptrvalue = ptr -> author;
      retval = AsnWrite(aip, MIM_CIT_author,  &av);
   }
   if (ptr -> others != NULL) {
      av.ptrvalue = ptr -> others;
      retval = AsnWrite(aip, MIM_CIT_others,  &av);
   }
   av.intvalue = ptr -> year;
   retval = AsnWrite(aip, MIM_CIT_year,  &av);
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
*    MimTextNew()
*
**************************************************/
NLM_EXTERN 
MimTextPtr LIBCALL
MimTextNew(void)
{
   MimTextPtr ptr = MemNew((size_t) sizeof(MimText));

   return ptr;

}


/**************************************************
*
*    MimTextFree()
*
**************************************************/
NLM_EXTERN 
MimTextPtr LIBCALL
MimTextFree(MimTextPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> label);
   MemFree(ptr -> text);
   MimLinkFree(ptr -> neighbors);
   return MemFree(ptr);
}


/**************************************************
*
*    MimTextAsnRead()
*
**************************************************/
NLM_EXTERN 
MimTextPtr LIBCALL
MimTextAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimTextPtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimText ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_TEXT);
   } else {
      atp = AsnLinkType(orig, MIM_TEXT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimTextNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_TEXT_label) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> label = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_TEXT_text) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> text = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_TEXT_neighbors) {
      ptr -> neighbors = MimLinkAsnRead(aip, atp);
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
   ptr = MimTextFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimTextAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimTextAsnWrite(MimTextPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_TEXT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> label != NULL) {
      av.ptrvalue = ptr -> label;
      retval = AsnWrite(aip, MIM_TEXT_label,  &av);
   }
   if (ptr -> text != NULL) {
      av.ptrvalue = ptr -> text;
      retval = AsnWrite(aip, MIM_TEXT_text,  &av);
   }
   if (ptr -> neighbors != NULL) {
      if ( ! MimLinkAsnWrite(ptr -> neighbors, aip, MIM_TEXT_neighbors)) {
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
*    MimEditItemNew()
*
**************************************************/
NLM_EXTERN 
MimEditItemPtr LIBCALL
MimEditItemNew(void)
{
   MimEditItemPtr ptr = MemNew((size_t) sizeof(MimEditItem));

   return ptr;

}


/**************************************************
*
*    MimEditItemFree()
*
**************************************************/
NLM_EXTERN 
MimEditItemPtr LIBCALL
MimEditItemFree(MimEditItemPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> author);
   MimDateFree(ptr -> modDate);
   return MemFree(ptr);
}


/**************************************************
*
*    MimEditItemAsnRead()
*
**************************************************/
NLM_EXTERN 
MimEditItemPtr LIBCALL
MimEditItemAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimEditItemPtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimEditItem ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_EDIT_ITEM);
   } else {
      atp = AsnLinkType(orig, MIM_EDIT_ITEM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimEditItemNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_EDIT_ITEM_author) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> author = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_EDIT_ITEM_modDate) {
      ptr -> modDate = MimDateAsnRead(aip, atp);
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
   ptr = MimEditItemFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimEditItemAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimEditItemAsnWrite(MimEditItemPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_EDIT_ITEM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> author != NULL) {
      av.ptrvalue = ptr -> author;
      retval = AsnWrite(aip, MIM_EDIT_ITEM_author,  &av);
   }
   if (ptr -> modDate != NULL) {
      if ( ! MimDateAsnWrite(ptr -> modDate, aip, MIM_EDIT_ITEM_modDate)) {
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
*    MimAllelicVariantNew()
*
**************************************************/
NLM_EXTERN 
MimAllelicVariantPtr LIBCALL
MimAllelicVariantNew(void)
{
   MimAllelicVariantPtr ptr = MemNew((size_t) sizeof(MimAllelicVariant));

   return ptr;

}


/**************************************************
*
*    MimAllelicVariantFree()
*
**************************************************/
NLM_EXTERN 
MimAllelicVariantPtr LIBCALL
MimAllelicVariantFree(MimAllelicVariantPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> number);
   MemFree(ptr -> name);
   AsnGenericBaseSeqOfFree(ptr -> aliases ,ASNCODE_PTRVAL_SLOT);
   AsnGenericUserSeqOfFree(ptr -> mutation, (AsnOptFreeFunc) MimTextFree);
   AsnGenericUserSeqOfFree(ptr -> description, (AsnOptFreeFunc) MimTextFree);
   return MemFree(ptr);
}


/**************************************************
*
*    MimAllelicVariantAsnRead()
*
**************************************************/
NLM_EXTERN 
MimAllelicVariantPtr LIBCALL
MimAllelicVariantAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimAllelicVariantPtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimAllelicVariant ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_ALLELIC_VARIANT);
   } else {
      atp = AsnLinkType(orig, MIM_ALLELIC_VARIANT);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimAllelicVariantNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_ALLELIC_VARIANT_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ALLELIC_VARIANT_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ALLELIC_VARIANT_aliases) {
      ptr -> aliases = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> aliases == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ALLELIC_VARIANT_mutation) {
      ptr -> mutation = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimTextAsnRead, (AsnOptFreeFunc) MimTextFree);
      if (isError && ptr -> mutation == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_ALLELIC_VARIANT_description) {
      ptr -> description = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimTextAsnRead, (AsnOptFreeFunc) MimTextFree);
      if (isError && ptr -> description == NULL) {
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
   ptr = MimAllelicVariantFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimAllelicVariantAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimAllelicVariantAsnWrite(MimAllelicVariantPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_ALLELIC_VARIANT);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> number != NULL) {
      av.ptrvalue = ptr -> number;
      retval = AsnWrite(aip, MIM_ALLELIC_VARIANT_number,  &av);
   }
   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, MIM_ALLELIC_VARIANT_name,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> aliases ,ASNCODE_PTRVAL_SLOT, aip, MIM_ALLELIC_VARIANT_aliases, MIM_ALLELIC_VARIANT_aliases_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> mutation, (AsnWriteFunc) MimTextAsnWrite, aip, MIM_ALLELIC_VARIANT_mutation, MIM_ALLELIC_VARIANT_mutation_E);
   AsnGenericUserSeqOfAsnWrite(ptr -> description, (AsnWriteFunc) MimTextAsnWrite, aip, MIM_ALLELIC_VARIANT_description, ALLELIC_VARIANT_description_E);
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
*    MimIndexTermNew()
*
**************************************************/
NLM_EXTERN 
MimIndexTermPtr LIBCALL
MimIndexTermNew(void)
{
   MimIndexTermPtr ptr = MemNew((size_t) sizeof(MimIndexTerm));

   return ptr;

}


/**************************************************
*
*    MimIndexTermFree()
*
**************************************************/
NLM_EXTERN 
MimIndexTermPtr LIBCALL
MimIndexTermFree(MimIndexTermPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> key);
   AsnGenericBaseSeqOfFree(ptr -> terms ,ASNCODE_PTRVAL_SLOT);
   return MemFree(ptr);
}


/**************************************************
*
*    MimIndexTermAsnRead()
*
**************************************************/
NLM_EXTERN 
MimIndexTermPtr LIBCALL
MimIndexTermAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimIndexTermPtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimIndexTerm ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_INDEX_TERM);
   } else {
      atp = AsnLinkType(orig, MIM_INDEX_TERM);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimIndexTermNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_INDEX_TERM_key) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> key = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_INDEX_TERM_terms) {
      ptr -> terms = AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && ptr -> terms == NULL) {
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
   ptr = MimIndexTermFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimIndexTermAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimIndexTermAsnWrite(MimIndexTermPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_INDEX_TERM);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> key != NULL) {
      av.ptrvalue = ptr -> key;
      retval = AsnWrite(aip, MIM_INDEX_TERM_key,  &av);
   }
   retval = AsnGenericBaseSeqOfAsnWrite(ptr -> terms ,ASNCODE_PTRVAL_SLOT, aip, MIM_INDEX_TERM_terms, MIM_INDEX_TERM_terms_E);
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
*    MimReferenceNew()
*
**************************************************/
NLM_EXTERN 
MimReferencePtr LIBCALL
MimReferenceNew(void)
{
   MimReferencePtr ptr = MemNew((size_t) sizeof(MimReference));

   return ptr;

}


/**************************************************
*
*    MimReferenceFree()
*
**************************************************/
NLM_EXTERN 
MimReferencePtr LIBCALL
MimReferenceFree(MimReferencePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   AsnGenericUserSeqOfFree(ptr -> authors, (AsnOptFreeFunc) MimAuthorFree);
   MemFree(ptr -> primaryAuthor);
   MemFree(ptr -> otherAuthors);
   MemFree(ptr -> citationTitle);
   MemFree(ptr -> bookTitle);
   AsnGenericUserSeqOfFree(ptr -> editors, (AsnOptFreeFunc) MimAuthorFree);
   MemFree(ptr -> volume);
   MemFree(ptr -> edition);
   MemFree(ptr -> journal);
   MemFree(ptr -> series);
   MemFree(ptr -> publisher);
   MemFree(ptr -> place);
   MemFree(ptr -> commNote);
   MimDateFree(ptr -> pubDate);
   AsnGenericUserSeqOfFree(ptr -> pages, (AsnOptFreeFunc) MimPageFree);
   MemFree(ptr -> miscInfo);
   return MemFree(ptr);
}


/**************************************************
*
*    MimReferenceAsnRead()
*
**************************************************/
NLM_EXTERN 
MimReferencePtr LIBCALL
MimReferenceAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimReferencePtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimReference ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_REFERENCE);
   } else {
      atp = AsnLinkType(orig, MIM_REFERENCE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimReferenceNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_REFERENCE_number) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> number = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_origNumber) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> origNumber = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_type) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> type = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_authors) {
      ptr -> authors = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimAuthorAsnRead, (AsnOptFreeFunc) MimAuthorFree);
      if (isError && ptr -> authors == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_primaryAuthor) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> primaryAuthor = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_otherAuthors) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> otherAuthors = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_citationTitle) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> citationTitle = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_citationType) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> citationType = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_bookTitle) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> bookTitle = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_editors) {
      ptr -> editors = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimAuthorAsnRead, (AsnOptFreeFunc) MimAuthorFree);
      if (isError && ptr -> editors == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_volume) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> volume = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_edition) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> edition = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_journal) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> journal = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_series) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> series = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_publisher) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> publisher = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_place) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> place = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_commNote) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> commNote = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_pubDate) {
      ptr -> pubDate = MimDateAsnRead(aip, atp);
      if (aip -> io_failure) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_pages) {
      ptr -> pages = AsnGenericUserSeqOfAsnRead(aip, amp, atp, &isError, (AsnReadFunc) MimPageAsnRead, (AsnOptFreeFunc) MimPageFree);
      if (isError && ptr -> pages == NULL) {
         goto erret;
      }
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_miscInfo) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> miscInfo = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_pubmedUID) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> pubmedUID = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_ambiguous) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> ambiguous = av.boolvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_REFERENCE_noLink) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> noLink = av.boolvalue;
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
   ptr = MimReferenceFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimReferenceAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimReferenceAsnWrite(MimReferencePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_REFERENCE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> number;
   retval = AsnWrite(aip, MIM_REFERENCE_number,  &av);
   av.intvalue = ptr -> origNumber;
   retval = AsnWrite(aip, MIM_REFERENCE_origNumber,  &av);
   av.intvalue = ptr -> type;
   retval = AsnWrite(aip, MIM_REFERENCE_type,  &av);
   AsnGenericUserSeqOfAsnWrite(ptr -> authors, (AsnWriteFunc) MimAuthorAsnWrite, aip, MIM_REFERENCE_authors, MIM_REFERENCE_authors_E);
   if (ptr -> primaryAuthor != NULL) {
      av.ptrvalue = ptr -> primaryAuthor;
      retval = AsnWrite(aip, MIM_REFERENCE_primaryAuthor,  &av);
   }
   if (ptr -> otherAuthors != NULL) {
      av.ptrvalue = ptr -> otherAuthors;
      retval = AsnWrite(aip, MIM_REFERENCE_otherAuthors,  &av);
   }
   if (ptr -> citationTitle != NULL) {
      av.ptrvalue = ptr -> citationTitle;
      retval = AsnWrite(aip, MIM_REFERENCE_citationTitle,  &av);
   }
   av.intvalue = ptr -> citationType;
   retval = AsnWrite(aip, MIM_REFERENCE_citationType,  &av);
   if (ptr -> bookTitle != NULL) {
      av.ptrvalue = ptr -> bookTitle;
      retval = AsnWrite(aip, MIM_REFERENCE_bookTitle,  &av);
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> editors, (AsnWriteFunc) MimAuthorAsnWrite, aip, MIM_REFERENCE_editors, MIM_REFERENCE_editors_E);
   if (ptr -> volume != NULL) {
      av.ptrvalue = ptr -> volume;
      retval = AsnWrite(aip, MIM_REFERENCE_volume,  &av);
   }
   if (ptr -> edition != NULL) {
      av.ptrvalue = ptr -> edition;
      retval = AsnWrite(aip, MIM_REFERENCE_edition,  &av);
   }
   if (ptr -> journal != NULL) {
      av.ptrvalue = ptr -> journal;
      retval = AsnWrite(aip, MIM_REFERENCE_journal,  &av);
   }
   if (ptr -> series != NULL) {
      av.ptrvalue = ptr -> series;
      retval = AsnWrite(aip, MIM_REFERENCE_series,  &av);
   }
   if (ptr -> publisher != NULL) {
      av.ptrvalue = ptr -> publisher;
      retval = AsnWrite(aip, MIM_REFERENCE_publisher,  &av);
   }
   if (ptr -> place != NULL) {
      av.ptrvalue = ptr -> place;
      retval = AsnWrite(aip, MIM_REFERENCE_place,  &av);
   }
   if (ptr -> commNote != NULL) {
      av.ptrvalue = ptr -> commNote;
      retval = AsnWrite(aip, MIM_REFERENCE_commNote,  &av);
   }
   if (ptr -> pubDate != NULL) {
      if ( ! MimDateAsnWrite(ptr -> pubDate, aip, MIM_REFERENCE_pubDate)) {
         goto erret;
      }
   }
   AsnGenericUserSeqOfAsnWrite(ptr -> pages, (AsnWriteFunc) MimPageAsnWrite, aip, MIM_REFERENCE_pages, MIM_REFERENCE_pages_E);
   if (ptr -> miscInfo != NULL) {
      av.ptrvalue = ptr -> miscInfo;
      retval = AsnWrite(aip, MIM_REFERENCE_miscInfo,  &av);
   }
   av.intvalue = ptr -> pubmedUID;
   retval = AsnWrite(aip, MIM_REFERENCE_pubmedUID,  &av);
   av.boolvalue = ptr -> ambiguous;
   retval = AsnWrite(aip, MIM_REFERENCE_ambiguous,  &av);
   av.boolvalue = ptr -> noLink;
   retval = AsnWrite(aip, MIM_REFERENCE_noLink,  &av);
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
*    MimLinkNew()
*
**************************************************/
NLM_EXTERN 
MimLinkPtr LIBCALL
MimLinkNew(void)
{
   MimLinkPtr ptr = MemNew((size_t) sizeof(MimLink));

   return ptr;

}


/**************************************************
*
*    MimLinkFree()
*
**************************************************/
NLM_EXTERN 
MimLinkPtr LIBCALL
MimLinkFree(MimLinkPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> uids);
   return MemFree(ptr);
}


/**************************************************
*
*    MimLinkAsnRead()
*
**************************************************/
NLM_EXTERN 
MimLinkPtr LIBCALL
MimLinkAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimLinkPtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimLink ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_LINK);
   } else {
      atp = AsnLinkType(orig, MIM_LINK);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimLinkNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_LINK_num) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> num = av.intvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_LINK_uids) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> uids = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_LINK_numRelevant) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> numRelevant = av.intvalue;
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
   ptr = MimLinkFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimLinkAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimLinkAsnWrite(MimLinkPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_LINK);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   av.intvalue = ptr -> num;
   retval = AsnWrite(aip, MIM_LINK_num,  &av);
   if (ptr -> uids != NULL) {
      av.ptrvalue = ptr -> uids;
      retval = AsnWrite(aip, MIM_LINK_uids,  &av);
   }
   av.intvalue = ptr -> numRelevant;
   retval = AsnWrite(aip, MIM_LINK_numRelevant,  &av);
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
*    MimAuthorNew()
*
**************************************************/
NLM_EXTERN 
MimAuthorPtr LIBCALL
MimAuthorNew(void)
{
   MimAuthorPtr ptr = MemNew((size_t) sizeof(MimAuthor));

   return ptr;

}


/**************************************************
*
*    MimAuthorFree()
*
**************************************************/
NLM_EXTERN 
MimAuthorPtr LIBCALL
MimAuthorFree(MimAuthorPtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> name);
   return MemFree(ptr);
}


/**************************************************
*
*    MimAuthorAsnRead()
*
**************************************************/
NLM_EXTERN 
MimAuthorPtr LIBCALL
MimAuthorAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimAuthorPtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimAuthor ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_AUTHOR);
   } else {
      atp = AsnLinkType(orig, MIM_AUTHOR);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimAuthorNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_AUTHOR_name) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> name = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_AUTHOR_index) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> index = av.intvalue;
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
   ptr = MimAuthorFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimAuthorAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimAuthorAsnWrite(MimAuthorPtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_AUTHOR);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> name != NULL) {
      av.ptrvalue = ptr -> name;
      retval = AsnWrite(aip, MIM_AUTHOR_name,  &av);
   }
   av.intvalue = ptr -> index;
   retval = AsnWrite(aip, MIM_AUTHOR_index,  &av);
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
*    MimPageNew()
*
**************************************************/
NLM_EXTERN 
MimPagePtr LIBCALL
MimPageNew(void)
{
   MimPagePtr ptr = MemNew((size_t) sizeof(MimPage));

   return ptr;

}


/**************************************************
*
*    MimPageFree()
*
**************************************************/
NLM_EXTERN 
MimPagePtr LIBCALL
MimPageFree(MimPagePtr ptr)
{

   if(ptr == NULL) {
      return NULL;
   }
   MemFree(ptr -> from);
   MemFree(ptr -> to);
   return MemFree(ptr);
}


/**************************************************
*
*    MimPageAsnRead()
*
**************************************************/
NLM_EXTERN 
MimPagePtr LIBCALL
MimPageAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean isError = FALSE;
   AsnReadFunc func;
   MimPagePtr ptr;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* MimPage ::= (self contained) */
      atp = AsnReadId(aip, amp, MIM_PAGE);
   } else {
      atp = AsnLinkType(orig, MIM_PAGE);
   }
   /* link in local tree */
   if (atp == NULL) {
      return NULL;
   }

   ptr = MimPageNew();
   if (ptr == NULL) {
      goto erret;
   }
   if (AsnReadVal(aip, atp, &av) <= 0) { /* read the start struct */
      goto erret;
   }

   atp = AsnReadId(aip,amp, atp);
   func = NULL;

   if (atp == MIM_PAGE_from) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> from = av.ptrvalue;
      atp = AsnReadId(aip,amp, atp);
   }
   if (atp == MIM_PAGE_to) {
      if ( AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      ptr -> to = av.ptrvalue;
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
   ptr = MimPageFree(ptr);
   goto ret;
}



/**************************************************
*
*    MimPageAsnWrite()
*
**************************************************/
NLM_EXTERN Boolean LIBCALL 
MimPageAsnWrite(MimPagePtr ptr, AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objmimAsnLoad()) {
         return FALSE;
      }
   }

   if (aip == NULL) {
      return FALSE;
   }

   atp = AsnLinkType(orig, MIM_PAGE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (ptr == NULL) { AsnNullValueMsg(aip, atp); goto erret; }
   if (! AsnOpenStruct(aip, atp, (Pointer) ptr)) {
      goto erret;
   }

   if (ptr -> from != NULL) {
      av.ptrvalue = ptr -> from;
      retval = AsnWrite(aip, MIM_PAGE_from,  &av);
   }
   if (ptr -> to != NULL) {
      av.ptrvalue = ptr -> to;
      retval = AsnWrite(aip, MIM_PAGE_to,  &av);
   }
   if (! AsnCloseStruct(aip, atp, (Pointer)ptr)) {
      goto erret;
   }
   retval = TRUE;

erret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return retval;
}

