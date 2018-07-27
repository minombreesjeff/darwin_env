/*
	PROJECT: Medline database.
	MODULE:	 cmdline
	FILES:	 cmdline.c, cmdline.h (this one)

	This file contains the definitions related to the command line
	processor for the medline_load program.

	Work started: 8 May 1991, Rand Huntzinger
	Original version completed: 8 May 1991, Rand S. Huntzinger
*
*
* RCS Modification History:
* $Log: cmdline.h,v $
* Revision 6.0  1997/08/25 18:36:05  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:54:32  epstein
* add RCS log revision history
*
*/

#ifndef	DEFS_MODULE_CMDLINE
#define	DEFS_MODULE_CMDLINE

/*	ProcessComandLine - Parse the tokens on the command line and
	store the results in various variables in the global area.	*/

void	ProcessCommandLine( /* int argc, char *argv[] */ );


#endif /* DEFS_MODULE_CMDLINE */
