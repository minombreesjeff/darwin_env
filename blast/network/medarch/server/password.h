/*
	password.h		Definitions for password module.

	MODULE:	 password
	FILES	 password.c and password.h (this one).

	Obtain the Sybase password from the environment or from input from
	the user's terminal.

	This module was taken from the Backbone database project code
	in May 1991 and this interface file was added to fit the
	Medline project's module requirement to have an include file
	with at least the external entry points defined.

	Edit history:
	    25 July 1991	Rand S. Huntzinger, NLM/NCBI
		Adapted to meet Medline project standards.

	Work started: 24 May 1991
	Original version completed: 24 May 1991, Rand S. Huntzinger

*
*
* RCS Modification History:
* $Log: password.h,v $
* Revision 6.0  1997/08/25 18:36:51  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:46  epstein
* add RCS log revision history
*
*/
#ifndef	DEFS_MODULE_PASSWORD
#define	DEFS_MODULE_PASSWORD

/* The GetPassword procedure will read a line from the tty without echoing
   it.  It does not do prompting, it merely turns echo off, reads the
   input line and restores the tty */

extern char	*GetPassword( FILE *tty, char *buffer, int buffer_size );

/* The Request password will attempt to get the password from the
   SYBASE_PASSWORD environment variable and failing that prompt for
   and receive the password from the user's terminal.  It also clears
   the password from the environment to confuse the ps -e command which
   could be used for snooping. */

extern char	*RequestPassword( char *prompt, char *buffer, int buffer_size );

#endif DEFS_MODULE_PASSWORD
