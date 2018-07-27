/*
	PROJECT: Medline database.
	MODULE:	 error
	FILES	 error.c and error.h (this one).

	This file contains defintions which pertain to the medline_load
	error module, which handles all error messages (except for usage
	errors) for the program.

	Work started: 22 May 1991
	Original version completed: 24 May 1991, Rand S. Huntzinger
*
*
* RCS Modification History:
* $Log: error.h,v $
* Revision 6.0  1997/08/25 18:36:08  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:54:37  epstein
* add RCS log revision history
*
*/

#ifndef	DEFS_MODULE_ERROR
#define	DEFS_MODULE_ERROR

#include <stdio.h>
#include "message.h"

/* Define the various error handling options - these are basically issue
   an error, issue a warning or just ignore it altogether */

typedef enum { issue_error, issue_warning, ignore_error } ErrorOption;


/* Define error message flags to be passed to the error handler.  These
   flags are used by the error handler to control how the error message
   is handled.  Programmer errors will get a reassuring "this isn't your
   fault" message added, database errors will get query dumps, fatal errors
   will cause the program to die, process errors will get logged to the
   database, etc. */

#define	ERR_PROGRAMMER		(1 << 9)	/* Program logic error */
#define	ERR_DATABASE		(1 << 10)	/* Database access error */
#define	ERR_FATAL		(1 << 11)	/* Fatal error */
#define	ERR_SYSTEM		(1 << 12)	/* Non-database system error */
#define	ERR_PROCESS		(1 << 13)	/* Error in processing */
#define	ERR_USAGE		(1 << 14)	/* Error from command line */
#define	ERR_FILE_FORMAT		(1 << 15)	/* Error in input format */
#define ERR_NO_QUERY_DUMP	(1 << 16)	/* Don't dump query */
#define	ERR_CHARSET		(1 << 17)	/* Character set error */
#define	ERR_LOG_TO_DATABASE	(1 << 18)	/* Log the error in database */

/* This mask is used to extract the error type value for the LogMessage
   flags word so it can be written into the BBErrors table. */

#define	ERR_TYPE_MASK		0xff		/* Low 8 bits - error type */

/* Define the error classes.  These are the values which you will get when
   you mask out the flag bits (above) using ERR_TYPE_MASK. */

#define	ERR_MESSAGE		(0)		/* A message */
#define ERR_REQUEST		(1)		/* Request user intervention */
#define	ERR_WARNING		(2)		/* A warning message */
#define	ERR_ERROR		(3)		/* Error - abort entry */
#define	ERR_TYPE_COUNT		(4)		/* Number of error types */

/* Define names for specific error types (groups of flags).  These are
   convenience definitions, so you don't have to or in everything on each
   LogMessage call. */

#define	PROGRAM_ERROR	( ERR_PROGRAMMER | ERR_ERROR | ERR_FATAL )
#define	UNIX_IO_ERROR	( ERR_SYSTEM | ERR_ERROR | ERR_FATAL )
#define	DB_ERROR	( ERR_DATABASE | ERR_ERROR | ERR_FATAL )
#define	DB_WARNING	( ERR_DATABASE | ERR_WARNING | ERR_LOG_TO_DATABASE )
#define	PROCESS_ERROR	( ERR_PROCESS | ERR_LOG_TO_DATABASE | ERR_ERROR )
#define	PROCESS_WARNING	( ERR_PROCESS | ERR_LOG_TO_DATABASE | ERR_WARNING )
#define	PROCESS_MESSAGE	( ERR_PROCESS | ERR_LOG_TO_DATABASE | ERR_MESSAGE )
#define	USAGE_ERROR	( ERR_USAGE | ERR_ERROR | ERR_FATAL )
#define	INPUT_ERROR	( ERR_FILE_FORMAT | ERR_ERROR | ERR_FATAL )
#define INPUT_WARNING	( ERR_FILE_FORMAT | ERR_LOG_TO_DATABASE | ERR_WARNING )
#define	CHARSET_ERROR	( ERR_CHARSET | ERR_LOG_TO_DATABASE | ERR_ERROR )
#define	CHARSET_WARNING	( ERR_CHARSET | ERR_LOG_TO_DATABASE | ERR_WARNING )


#endif	/* DEFS_MODULE_ERROR */
