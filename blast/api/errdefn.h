/****************************************************************************
*   errdefn.h:
*
*
* $Log: errdefn.h,v $
* Revision 6.0  1997/08/25 18:05:31  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.1  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 1.4  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*                                     
****************************************************************************/
#ifndef _ERRDEFN_
#define _ERRDEFN_

#include "validatr.h"

/*  -- write error message to filename.err                          3-30-93 */

static char *this_module ="validatr";

#ifdef THIS_MODULE
#undef THIS_MODULE
#endif
#define THIS_MODULE this_module

#endif
