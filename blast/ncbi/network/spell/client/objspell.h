/*
*
*
* RCS Modification History:
* $Log: objspell.h,v $
* Revision 6.0  1997/08/25 18:40:26  madden
* Revision changed to 6.0
*
* Revision 4.0  1995/07/26 13:55:42  ostell
* force revision to 4.0
*
 * Revision 1.2  1995/05/17  17:59:53  epstein
 * add RCS log revision history
 *
*/

#ifndef _objspell_ 
#define _objspell_ 

#ifdef __cplusplus
extern "C" { /* } */
#endif


/**************************************************
*
*    Generated objects for Module NCBI-SPELL
*
**************************************************/
typedef ValNodePtr SpellRequestPtr;
typedef ValNode SpellRequest;
#define SpellRequest_init 1
#define SpellRequest_spelltext 2
#define SpellRequest_fini 3


SpellRequestPtr LIBCALL SpellRequestFree PROTO ((SpellRequestPtr ));
SpellRequestPtr LIBCALL SpellRequestAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL SpellRequestAsnWrite PROTO (( SpellRequestPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr SpellResponsePtr;
typedef ValNode SpellResponse;
#define SpellResponse_error 1
#define SpellResponse_init 2
#define SpellResponse_spelltext 3
#define SpellResponse_fini 4


SpellResponsePtr LIBCALL SpellResponseFree PROTO ((SpellResponsePtr ));
SpellResponsePtr LIBCALL SpellResponseAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL SpellResponseAsnWrite PROTO (( SpellResponsePtr , AsnIoPtr, AsnTypePtr));

#ifdef __cplusplus
/* { */ }
#endif

#endif
