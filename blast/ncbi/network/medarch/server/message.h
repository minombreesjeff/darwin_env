/*
	message.h		Definitions for the message.c module.

	MODULE:	 message
	FILES	 message.c and message.h (this one).

	This file contains definitions used for the generic message
	handling module.  It should be included in all programs which
	use this module.

	This module was taken from the Backbone database project code
	and slightly modified to bring it up to the standards required
	for the Medline project.

	Edit history:
	    25 July 1991	Rand S. Huntzinger, NLM/NCBI
		Adapted to meet Medline project standards.

	Original version: 17 Jan 1990 - Rand S. Huntzinger
		Adapted from IRx error.h module which was written 26 May 1987
		by Rand S. Huntzinger.
*
*
* RCS Modification History:
* $Log: message.h,v $
* Revision 6.0  1997/08/25 18:36:42  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:24  epstein
* add RCS log revision history
*
*/

#ifndef	DEFS_MODULE_MESSAGE
#define	DEFS_MODULE_MESSAGE

/* Basic definitions used by message.c */

#define	ERROR_BUF_SIZE	1024		/* Size of error buffer */

/* Define the functions in the message.c file */

extern	void LogMessage( int flags, char *where, char *fmt, ... );
extern	void LogSysMessage(int flags, char *where, int errno, char *fmt, ...);
extern	void GetMessageHook( int (**error_handler_address)(), void **ptr );

#endif	/* DEFS_MODULE_MESSAGE */
