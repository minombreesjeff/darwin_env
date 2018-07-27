/*
*
*
* RCS Modification History:
* $Log: objspell.c,v $
* Revision 6.0  1997/08/25 18:40:24  madden
* Revision changed to 6.0
*
* Revision 4.0  1995/07/26 13:55:42  ostell
* force revision to 4.0
*
 * Revision 1.2  1995/05/17  17:59:51  epstein
 * add RCS log revision history
 *
*/

#include <asn.h>

#define NLM_GENERATED_CODE_PROTO

#include "objspell.h"

static Boolean loaded = FALSE;

#include "spell.h"

#ifndef NLM_EXTERN_LOADS
#define NLM_EXTERN_LOADS {}
#endif

static Boolean
objspellAsnLoad(void)
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
*    Generated object loaders for Module NCBI-SPELL
*
**************************************************/


/**************************************************
*
*    SpellRequestFree()
*
**************************************************/

SpellRequestPtr LIBCALL
SpellRequestFree(ValNodePtr anp)
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
   case SpellRequest_spelltext:
      MemFree(anp -> data.ptrvalue);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    SpellRequestAsnRead()
*
**************************************************/

SpellRequestPtr LIBCALL
SpellRequestAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objspellAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* SpellRequest ::= (self contained) */
      atp = AsnReadId(aip, amp, SPELL_REQUEST);
   } else {
      atp = AsnLinkType(orig, SPELL_REQUEST);    /* link in local tree */
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
   if (atp == SPELL_REQUEST_init) {
      choice = SpellRequest_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == SPELL_REQUEST_spelltext) {
      choice = SpellRequest_spelltext;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.ptrvalue = av.ptrvalue;
   }
   else if (atp == SPELL_REQUEST_fini) {
      choice = SpellRequest_fini;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   goto ret;
}


/**************************************************
*
*    SpellRequestAsnWrite()
*
**************************************************/
Boolean LIBCALL 
SpellRequestAsnWrite(SpellRequestPtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objspellAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, SPELL_REQUEST);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   av.intvalue = anp->data.intvalue;
   switch (anp->choice)
   {
   case SpellRequest_init:
      retval = AsnWrite(aip, SPELL_REQUEST_init, &av);
      break;
   case SpellRequest_spelltext:
      retval = AsnWrite(aip, SPELL_REQUEST_spelltext, &av);
      break;
   case SpellRequest_fini:
      retval = AsnWrite(aip, SPELL_REQUEST_fini, &av);
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
*    SpellResponseFree()
*
**************************************************/

SpellResponsePtr LIBCALL
SpellResponseFree(ValNodePtr anp)
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
   case SpellResponse_spelltext:
      AsnGenericBaseSeqOfFree((ValNodePtr) pnt,ASNCODE_PTRVAL_SLOT);
      break;
   }
   return MemFree(anp);
}


/**************************************************
*
*    SpellResponseAsnRead()
*
**************************************************/

SpellResponsePtr LIBCALL
SpellResponseAsnRead(AsnIoPtr aip, AsnTypePtr orig)
{
   DataVal av;
   AsnTypePtr atp;
   ValNodePtr anp;
   Uint1 choice;
   Boolean isError;
   AsnReadFunc func;

   if (! loaded)
   {
      if (! objspellAsnLoad()) {
         return NULL;
      }
   }

   if (aip == NULL) {
      return NULL;
   }

   if (orig == NULL) {         /* SpellResponse ::= (self contained) */
      atp = AsnReadId(aip, amp, SPELL_RESPONSE);
   } else {
      atp = AsnLinkType(orig, SPELL_RESPONSE);    /* link in local tree */
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
   if (atp == SPELL_RESPONSE_error) {
      choice = SpellResponse_error;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == SPELL_RESPONSE_init) {
      choice = SpellResponse_init;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   else if (atp == SPELL_RESPONSE_spelltext) {
      choice = SpellResponse_spelltext;
      anp -> data.ptrvalue = 
      AsnGenericBaseSeqOfAsnRead(aip, amp, atp, ASNCODE_PTRVAL_SLOT, &isError);
      if (isError && anp -> data.ptrvalue == NULL) {
         goto erret;
      }
   }
   else if (atp == SPELL_RESPONSE_fini) {
      choice = SpellResponse_fini;
      if (AsnReadVal(aip, atp, &av) <= 0) {
         goto erret;
      }
      anp->data.intvalue = av.intvalue;
   }
   anp->choice = choice;
   if (func != NULL)
   {
      anp->data.ptrvalue = (* func)(aip, atp);
      if (anp->data.ptrvalue == NULL) {
         goto erret;
      }
   }

ret:
   AsnUnlinkType(orig);       /* unlink local tree */
   return anp;

erret:
   anp = MemFree(anp);
   goto ret;
}


/**************************************************
*
*    SpellResponseAsnWrite()
*
**************************************************/
Boolean LIBCALL 
SpellResponseAsnWrite(SpellResponsePtr anp, AsnIoPtr aip, AsnTypePtr orig)

{
   DataVal av;
   AsnTypePtr atp, writetype = NULL;
   Pointer pnt;
   AsnWriteFunc func = NULL;
   Boolean retval = FALSE;

   if (! loaded)
   {
      if (! objspellAsnLoad())
      return FALSE;
   }

   if (aip == NULL)
   return FALSE;

   atp = AsnLinkType(orig, SPELL_RESPONSE);   /* link local tree */
   if (atp == NULL) {
      return FALSE;
   }

   if (anp == NULL) { AsnNullValueMsg(aip, atp); goto erret; }

   av.ptrvalue = (Pointer)anp;
   if (! AsnWriteChoice(aip, atp, (Int2)anp->choice, &av)) {
      goto erret;
   }

   pnt = anp->data.ptrvalue;
   av.intvalue = anp->data.intvalue;
   switch (anp->choice)
   {
   case SpellResponse_error:
      retval = AsnWrite(aip, SPELL_RESPONSE_error, &av);
      break;
   case SpellResponse_init:
      retval = AsnWrite(aip, SPELL_RESPONSE_init, &av);
      break;
   case SpellResponse_spelltext:
      retval = AsnGenericBaseSeqOfAsnWrite((Pointer) pnt,ASNCODE_PTRVAL_SLOT, aip, SPELL_RESPONSE_spelltext, SPELL_RESPONSE_spelltext_E);            break;
   case SpellResponse_fini:
      retval = AsnWrite(aip, SPELL_RESPONSE_fini, &av);
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
