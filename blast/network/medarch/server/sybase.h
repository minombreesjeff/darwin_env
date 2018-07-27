/*
*
*
* RCS Modification History:
* $Log: sybase.h,v $
* Revision 6.0  1997/08/25 18:36:59  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:56:00  epstein
* add RCS log revision history
*
*/

/* The following is the Sybase include file package */

#ifndef	DEFS_MODULE_SYBASE_H
#define	DEFS_MODULE_SYBASE_H

#include <sybfront.h>
#include <sybdb.h>
#ifdef	HAVE_SRV_H
#include <srv.h>
#else
#define	SRV_MAXCHAR	(DBMAXCHAR-1)
#endif

#endif /* DEFS_MODULE_SYBASE_H */
