/*
	sqlstring.h	Handle quotes in strings for SQL expressions.

	MODULE:	sqlstring
	FILES:	sqlstring.c and sqlstring.h (this one).

	This package adds SQL style quoting notations so an arbitrary
	ASCII string can be passed to Sybase without confusing the
	query parser.  This is done by doubling the quote character
	within the string.

	Note:	This module was adapted from the module of the same
		name from the GenInfo Backbone database project.  It
		had to be slightly modified to meet Medline project
		coding conventions.  (module nomenclature)

	Edit History:
	     7 August 1991 - Rand S. Huntzinger
		Added SQLbinary() to the external calls.

	    25 July 1991 - Rand S. Huntzinger, NLM/NCBI
		Modified to comply with Medline project coding conventions.

	Original version: 22 Mar 91, Rand S. Huntzinger, NLM/NCBI
*
*
* RCS Modification History:
* $Log: sqlstring.h,v $
* Revision 6.0  1997/08/25 18:36:58  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:57  epstein
* add RCS log revision history
*
*/

#ifndef	DEFS_MODULE_SQLSTRING
#define	DEFS_MODULE_SQLSTRING

/* Define the entry points to the SQLstring module */

void	SQLstringFree( char *text );
void	SQLstringClear();
void	SQLstringReset();
char	*SQLstring( char *text, int width, char quote );
char	*SQLbinary( unsigned char *buffer, int length );

#endif /* DEFS_MODULE_SQLSTRING */
