/****************************************************************************
*   a2ferrdf.h:
*
*                                     
****************************************************************************/
/* $Revision: 6.0 $ */ 
/************************************
*
* $Log: a2ferrdf.h,v $
* Revision 6.0  1997/08/25 18:04:29  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.1  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 1.3  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
************************************/


#ifndef _A2FERRDF_
#define _A2FERRDF_

#include "a2ferr.h"

/*  -- write error message to filename.err                          3-30-93 */

static char *this_module ="a2ferr";

#ifdef THIS_MODULE
#undef THIS_MODULE
#endif
#define THIS_MODULE this_module

#endif
