/*
	sybbind.h	High level Sybase row loading and binding support

	MODULE:	sybbind
	FILES:	sybbind.c and sybbind.h (this one).

	This module provides a high-level interface to the Sybase procedures
	for binding rows to C variables and for loading rows in form the
	database.  These procedures permit a single procedure call to bind
	many variables in a single call.  The SybaseLoad procedure call
	adds row retrieval as well.  This reduce the amounts of coding
	detail required by lumping the dbbind() and dbnextresults() calls
	into a single call with only a single return code to check.

	Note:	This module was adapted from the module of the same
		name from the GenInfo Backbone database project.  It
		had to be slightly modified to meet Medline project
		coding conventions.  (module nomenclature)

	Edit History:
	    25 July 1991 - Rand S. Huntzinger, NLM/NCBI
		Modified to comply with Medline project coding conventions.

	Original version: circa 1989-1990, Rand S. Huntzinger, NLM/NCBI
*
*
* RCS Modification History:
* $Log: sybbind.h,v $
* Revision 6.0  1997/08/25 18:37:03  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:56:05  epstein
* add RCS log revision history
*
*/

#ifndef	DEF_MODULE_SYBBIND
#define	DEF_MODULE_SYBBIND

/* Define the procedural entry points to the sybbind module */

RETCODE	VSybaseBind( DBPROCESS *dbproc, void *args );
RETCODE	SybaseBind( DBPROCESS *dbproc, ... );
RETCODE	SybaseLoad( DBPROCESS *dbproc, ... );

#endif /* DEF_MODULE_SYBBIND */
