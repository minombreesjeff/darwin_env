/*

	xalloc.h		Declarations for xalloc.c

	MODULE:	xalloc
	FILES:	xalloc.c and xalloc.h (this one).

	This file contains definitions used by the memory allocation
	functions implemented by xalloc.c.  This package uses malloc
	and related system library functions to do memory allocation
	and recovery, but adds error recovery and statistics gathering
	functions.

	Note:	This module was adapted from the module of the same
		name from the GenInfo Backbone database project.  It
		had to be slightly modified to meet Medline project
		coding conventions.  (module nomenclature)

	Edit History:

	    18 October 1991 - Rand S. Huntzinger
		Converted to ANSI-C

	    25 July 1991 - Rand S. Huntzinger, NLM/NCBI
		Modified to comply with Medline project coding conventions.

	Original version: unknown date, Rand S. Huntzinger, NLM/NCBI
*
*
* RCS Modification History:
* $Log: xalloc.h,v $
* Revision 6.0  1997/08/25 18:37:14  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:56:23  epstein
* add RCS log revision history
*
*/

#ifndef	DEFS_MODULE_XALLOC
#define	DEFS_MODULE_XALLOC

#include	<stdlib.h>

/* The following structure defines the memory allocation statistics
   record.  It contains counts of the various types of memory requests. */

typedef struct XMESTAT {
	long	xallocs;
	long	xcallocs;
	long	xreallocs;
	long	xfrees;
	long	xfree0s;
	long	xerrcals;
	long	xfails;
} XMESTAT;

/* Type CPFUNC refers to a function which returns a character pointer */

typedef	char	*CPFUNC();

/* The following are forward declarations for the functions defined in
   the xalloc module. The arguments to each routine are the same as the
   arguments to the underlying module. */

char    *xalloc(size_t);		/* Interface to malloc() function */
char    *xcalloc(size_t, size_t);	/* Interface to calloc() function */
char    *xrealloc(void *, size_t);	/* Interface to realloc() function */
void    xfree(void *);			/* Interface to free() function */

/* These stubs are used for enabling statistics and error handling procedures */

XMESTAT *xmemstat(XMESTAT *, int);	/* Enable/disable statistics */
CPFUNC  *xmemerr(CPFUNC func);		/* Set memory error handler function */
int	xnullok(int);		/* Permit/Prohibit NULL return values */

#endif /* DEFS_MODULE_XALLOC */
