/*
*
*
* RCS Modification History:
* $Log: mappings.h,v $
* Revision 6.0  1997/08/25 18:36:25  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:02  epstein
* add RCS log revision history
*
*/

#include <objmedli.h>
#include <objpub.h>
#include <objfeat.h>

/* legitimate substitutions to handle differences between existing object */
/* loaders and symbol names generated for automatically-generated object  */
/* loaders (for imported types)                                           */
#define TitlePtr ValNodePtr

#define NLM_EXTERN_LOADS { if (! MedlineAsnLoad()) return FALSE; \
                         if (! PubAsnLoad()) return FALSE; \
                         if (! BiblioAsnLoad()) return FALSE; }
