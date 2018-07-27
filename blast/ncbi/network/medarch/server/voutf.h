/*
	voutf.h		Process formatted text through an output function.

	MODULE:	voutf
	FILES:	voutf.c and voutf.h (this one).

	This package implements a flexible variable argument list
	output function similar to the System V vprintf function;
	however, it differs by using a function as an output sink
	instead of a string buffer or an output file.  This increases
	the flexibility of the package by allowing the supplied
	function to post-process and redirect the output as required
	by the application.

	Note:	This file was added to meet Medline project module
		criteria (include file with entry points defined)
		when the voutf.c program was adapted from the GenInfo
		Backbone database project.

	Original version completed: 25 July 1991, Rand S. Huntzinger NLM/NCBI
*
*
* RCS Modification History:
* $Log: voutf.h,v $
* Revision 6.0  1997/08/25 18:37:10  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:56:17  epstein
* add RCS log revision history
*
*/

#ifndef	DEFS_MODULE_VOUTF
#define	DEFS_MODULE_VOUTF

#include	<stdarg.h>

/* Define the entry points to the voutf module.  In the statements assume
   IFUNC refers to the name of an integer function. */

typedef int IFUNC();
extern	int	voutf( int (*func)(), char *fmt, void *list);
extern	int	voutput( int (*func)(), char *fmt, ...);

#endif	/* DEFS_MODULE_VOUTF */
