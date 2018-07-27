/*
	PROJECT: Medline database.
	MODULE:  ma_global
	FILES:	 ma_global.c (this one), ma_global.h

	This file contains the global variable definitions for the
	medline retrieval programs.

	Original version completed: 24 February 1992, Rand S. Huntzinger

*
*
* RCS Modification History:
* $Log: ma_global.c,v $
* Revision 6.0  1997/08/25 18:36:15  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:54:48  epstein
* add RCS log revision history
*
*/

#define	COMPILING_GLOBAL_C

#include "ma_global.h"

/******************************************************************************
				COMMAND LINE PARSING

	The following variables receive the results of compiling the
	medline_load command line.  The cmdline module is used to fill
	in the data and other modules (particularly at the top level)
	have access to it.

******************************************************************************/

char		 *programName = NULL;	/* This program's name (from argv[0]) */

char		 *flagDatabase = NULL;	/* Database name from command line */

char		 *flagUser = NULL;	/* Sybase user name from command line */

char		 *flagServer = NULL;	/* Sybase server from command line */

char		 *flagPassword = NULL;	/* Sybase password  from command line */

char		 *flagConfigFile=NULL;	/* Configuration file for formats */

short		  flagVerbose = FALSE;	/* State of the -verbose flag */

short		  flagDebug = FALSE;	/* State of the -debug flag */

short		  flagISO = FALSE;	/* State of the -iso flag */

short		  flagTrace = 0;	/* -trace flag, default = no trace */

int		  argumentCount = 0;	/* Number of command line arguments */

char		**argumentList = (char **) NULL;
					/* Pointer to vector of arguments. */

int		  outputStreamCount = 0;	/* # of output streams */

OutStream	  outputStreams[ MAX_OUTPUT_STREAMS ];	/* Stream table */

