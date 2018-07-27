/*
	sybintfc.h		Execute a Sybase command using a
				printf-style format for command building

	MODULE:	sybintf
	FILES:	sybintf.c and sybintf.h (this one).

	This module contains functions which permit a printf-style format
	to be used for building and executing Sybase queries.  This
	avoids having the application have to piece the command together
	from a bunch of little pieces using multiple calls to dbcmd.

	Note:	This package was adapted for the Medline project from
		the same module in the GenInfo Backbone database project.
		Some minor changes had to be made to conform to Medline
		style conventions.

	Dependencies:

	    voutf	A package supporting printf-style formats with
			output directed via function calls instead of
			to a file or memory buffer.

	Edit history:

	    25 July 1991 - Rand S. Huntzinger
		Moved to Medline project area and adapted the code to
		meet Medline project requirements.

	    5 April 1991 - Added a print query debugging option.

	Original Implementation: 7 Dec 89 - Rand S. Huntzinger.
*
*
* RCS Modification History:
* $Log: sybintfc.h,v $
* Revision 6.0  1997/08/25 18:37:06  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:56:11  epstein
* add RCS log revision history
*
*/

#ifndef DEFS_MODULE_SYBINTFC
#define	DEFS_MODULE_SYBINTFC

#include <stdarg.h>
#include <stdio.h>
#include "sybase.h"

/* These variables can be set in the debugger or by a program to help
   in debugging by printing out Sybase queries before they are executed. */

FILE	*SybaseQueryPrintFile;
int	SybaseQueryPrintCount;

/* The following are the entry points to the sybintfc module */

void	DumpSybaseCommand( DBPROCESS *db, FILE *fd );
RETCODE	VExtendSybaseCommand( DBPROCESS *db, char *fmt, void *args);
RETCODE	VLoadSybaseCommand( DBPROCESS *db, char *fmt, void *args);
RETCODE	LoadSybaseCommand( DBPROCESS *db, char *fmt, ... );
RETCODE	ExtendSybaseCommand( DBPROCESS *db, char *fmt, ... );
RETCODE	ExecSybaseCommand( DBPROCESS *db );
RETCODE	RunSybase( DBPROCESS *db, char *fmt, ... );
RETCODE	RunSybaseCheck( DBPROCESS *db, char *fmt, ... );


#endif DEFS_MODULE_SYBINTFC
