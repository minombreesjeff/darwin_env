/*
	PROJECT: Medline database.
	MODULE:  ma_global
	FILES:	 ma_global.c, ma_global.h (this one)

	This file contains the global variable definitions for the
	medline retrieval programs.  It is used to reference the global
	variables as externs.  This file also contains type definitions
	which are limited to the global name space.

	Original version completed: 24 February 1992, Rand S. Huntzinger
*
*
* RCS Modification History:
* $Log: ma_global.h,v $
* Revision 6.0  1997/08/25 18:36:17  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:54:51  epstein
* add RCS log revision history
*
*/

#ifndef	DEFS_MODULE_GLOBAL
#define	DEFS_MODULE_GLOBAL

#include <unistd.h>
#include <stdlib.h>
#include "config.h"

/******************************************************************************

			     MACRO DECLARATIONS

******************************************************************************/

/* The GLOBAL macro is set to nothing if we are compiling the global.c
   option (as noted by the COMPILING_GLOBAL_C flag) or to extern if
   compiling any other module.  Therefore, the declaration acts either
   as a forward of an extern declaration, depending upon the module.  */

#ifdef	COMPILING_GLOBAL_C
#define	GLOBAL
#else
#define	GLOBAL	extern
#endif /* COMPILING_GLOBAL_C */


/******************************************************************************

			     TYPE DECLARATIONS

******************************************************************************/

typedef	struct OutStream {
    char	*format;		/* The name of the output format */
    char	*file;			/* The output file name (- => stdout) */
    void	*handle;		/* Output file handle (null=>none) */
} OutStream;

/******************************************************************************

			GLOBAL VARIABLE DECLARATIONS

******************************************************************************/

/* COMMAND LINE PARSING - The following variables are set by the cmdline
   as the command line is parsed.  The may be used by other modules in the
   medline_load program.  */

GLOBAL char       *programName;		/* This program's name (from argv[0]) */

GLOBAL char	  *flagDatabase;	/* Database name from command line */

GLOBAL char	  *flagUser;		/* Sybase user name from command line */

GLOBAL char	  *flagServer;		/* Sybase server from command line */

GLOBAL char	  *flagPassword;	/* Sybase password  from command line */

GLOBAL char	  *flagConfigFile;	/* Configuration file for formats */

GLOBAL short       flagVerbose;		/* State of the -verbose flag */

GLOBAL short       flagDebug;		/* State of the -debug flag */

GLOBAL short       flagISO;		/* State of the -iso flag */

GLOBAL short       flagTrace;		/* -trace flag, default = no trace */

GLOBAL int	   argumentCount;	/* Number of command line arguments */

GLOBAL char	 **argumentList;	/* Pointer to vector of arguments.
					   Loaded only if inputFileCount > 0 */

GLOBAL int	   outputStreamCount;	/* Number of output streams given */

GLOBAL OutStream   outputStreams[ MAX_OUTPUT_STREAMS];
					/* Table of output stream arguments */

#endif /* DEFS_MODULE_GLOBAL */
