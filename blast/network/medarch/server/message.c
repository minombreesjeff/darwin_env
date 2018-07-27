/*
	message.c		General purpose message handler

	MODULE:	 message
	FILES	 message.c (this one) and message.h.

	The routines in this module implement a general purpose message
	handler.  This module contains variable argument calls which
	can be used to generate an error message and then pass control
	to a client supplied routine which is to be used to display the
	resulting string.  A flag word is also passed so the user can
	determine the type of error and if desired exit or perform other
	processing.

	This module was taken from the Backbone database project code
	and slightly modified to bring it up to the standards required
	for the Medline project.

	Edit history:
	    25 July 1991	Rand S. Huntzinger, NLM/NCBI
		Adapted to meet Medline project standards.

	Original version: 17 Jan 1990 - Rand S. Huntzinger
	    Adapted from IRx error.c module which was written 26 May 1987
	    by Rand S. Huntzinger.


	Required modules:	voutf.c
*
*
* RCS Modification History:
* $Log: message.c,v $
* Revision 6.0  1997/08/25 18:36:40  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:21  epstein
* add RCS log revision history
*
*/
 
#include	<stdarg.h>
#include	<string.h>
#include	<stdio.h>
#include	"voutf.h"
#include	"message.h"
#include	"xalloc.h"

/* Define a few basic types and parameters */

#ifndef	NULL
#define	NULL	((char *) 0)		/* If NULL isn't defined. */
#endif

/* Forward declarations */

static	int	use_stderr();

/**************************************************************************

				Local Variables

**************************************************************************/

static	char	msg_buf[ERROR_BUF_SIZE];	/* Message buffer */
static	char	*msg_ptr = msg_buf;		/* Advancing ptr */
static	int	msg_cnt = 0;			/* Space left in buffer */

/* This is the preloaded "implementation hook".  It may be read & altered
   using the GetMessageHook and SetMessageHook functions respectively. */

static	int	(*message_func)() = use_stderr;	/* Default error handler */
static	char	*message_data = NULL;		/* Data from client call */


/***************************************************************************

	The following code is used by voutf to implement a buffersize
	checked 'sprintf' type function.  This function is only used
	internally.

***************************************************************************/

/*	Macro: Initializes the message buffer for use by to_message_buf */

#define	setup_message_buf() {msg_ptr = msg_buf; msg_cnt = ERROR_BUF_SIZE-1;}

/*
	to_message_buf		Add data to a message buffer.

	This routine adds data to the error message buffer.  Unlike
	the standard I/O facilities, we do test for buffer overflow
	and do not overfill the buffer.

	Parameters:

		s		The string to add to the buffer.

	Returns:

		0		If no error occurred.
		1		If the buffer overflowed.

	NOTE:	The macro, setup_message_buf() can be used to clear the
		message buffer before accumulating text.
*/

static
/*
*		to_message_buf(chars)
*/
int	to_message_buf(char *s)
{
	int	len;

	/* Handle the case of the message buffer being full */

	if((len = strlen(s)) > msg_cnt) {
		strncpy(msg_ptr, s, msg_cnt);
		msg_ptr += msg_cnt;
		msg_cnt = 0;
		return(1);		/* Failure, output truncated */
	}

	/* Normal case */

	strcpy(msg_ptr, s);
	msg_ptr += len;
	msg_cnt -= len;
	return( 0 );
}


/**************************************************************************

	This following is the default error processing routine.  It writes
	error messages on standard error.  This routine is installed in the
	implementation hook at compile time, and reinstalled if NULL is
	passed as a function pointer.  They ignore the ptr parameter.

**************************************************************************/

/*
	use_stderr		Write error message to stderr

	This is the default error display function.  It writes an
	error message to stderr and returns success.  If another
	error display function is to be used, the SetDpyError
	function must be used to override this declaration.

	Parameters:

		ptr		A pointer to an arbitrary
				structure which is passed from
				the client code.

		flags		The flags passed to the calling
				LogError routine.

		where		A string stating where the error
				occurred.

		msg		The error message to be logged.

	Returns:

		0		Success.
		1		Failure.

	NOTE:	This is the standard argument list for all
		error message output functions.
*/

/*ARGSUSED*/
static
/*
*		use_stderr(voidclient_data,intflags,charwhere,charmsg)
*/
int	use_stderr(void *client_data, int flags, char *where, char *msg)
{
	/* Simply print the message */

	return(fprintf(stderr, "%s: %s\n", where, msg) >= 0 ? 0 : 1);
}


/**************************************************************************

	The following routines are the entry points for generating
	error messages using the routines in this function.  The
	basic function, LogError, is called by the special system
	error handling function, LogSysMessage, which handles system
	error numbers.

**************************************************************************/


/*
	LogMessage		Output a message - make sure no error.

	This routine is identical to LogError except that the message
	only the message logging functions occur.  Error processing
	options (core dumps, exiting) is suppressed.

	Parameters:

	    flags		A set of flags used to control how the
				error is to be handled.  Bits are either
				interpreted here or in implementation
				hooks.

	    where		A pointer to a character string giving
				the place where the error occurred.

	    fmt			The format used to generate the error
				message.  This format is interpeted by
				voutf and is similar to a printf
				format.

	    ...			The remaining arguments are used to
				generate the output string according to
				the format in fmt.

	Returns:	nothing.
*/

/*
*		LogMessage(intflags,charwhere,charfmt,...)
*/
void	LogMessage(int flags, char *where, char *fmt, ...)
{
	va_list	args;

	/* Get the fixed arguments */

	va_start(args, fmt);

	/* Generate the message string */

	setup_message_buf();
	voutf(to_message_buf, fmt, args);
	va_end(args);

	/* Call the message trapping function */

	message_func(message_data, flags, where, msg_buf);

	/* No exit functions - just quit */
}


/*
	LogSysMessage		Log a system generated error message.

	This routine is similar to LogError except that it is used
	to generate an error message using a system error number
	in addition to the formatted message.

	Parameters:

	    flags		A set of flags used to control how the
				error is to be handled.  Bits are
				interpreted by the implementation
				routine

	    where		A pointer to a character string giving
				the place where the error occurred.

	    err_no		The system error number to report.
				If zero, this value is read from the
				system error variable.

	    fmt			The format used to generate the error
				message.  This format is interpeted by
				voutf and is similar to a printf
				format.  The notation "%%s" should
				appear where the text expansion of the
				system error number should appear.

	    ...			The remaining arguments are used to
				generate the output string according to
				the format in fmt.

	Returns nothing.
*/

/*
*		LogSysMessage(intflags,charwhere,interr_no,charfmt,...)
*/
void	LogSysMessage(int flags, char *where, int err_no, char *fmt, ...)
{
	va_list	args;
	char	*afmt, *err_msg;
	extern	int	errno;
	extern	int	sys_nerr;
	extern	char	*sys_errlist[];

	/* Get the fixed arguments */

	va_start(args, fmt);
	if(err_no == 0) err_no = errno;

	/* Use remaining arguments to produce a format */

	setup_message_buf();
	voutf(to_message_buf, fmt, args);
	afmt = xalloc((unsigned int) strlen(msg_buf)+1);
	strcpy(afmt, msg_buf);
	va_end( args );

	/* Lookup the system error message */

	if(err_no < 0 || err_no >= sys_nerr) {
		static char ebuf[30];
		err_msg = ebuf;
		sprintf(ebuf, "Unrecognized error #%d", err_no);
	} else	err_msg = sys_errlist[err_no];

	/* Log the error */

	LogMessage(flags, where, afmt, err_msg);
	xfree(afmt);
}


/**************************************************************************

	The following routines are used to set the implementation
	hooks for the LogError routine.  Routines are provided to
	retrieve the current hook value so the value can be reset
	to the previous value as necessary.

	The output hooks also support an arbitrary pointer parameter
	which can be used to feed data to the user supplied routine.
	For example, an output file could be passed in this way.

**************************************************************************/



/*
	SetMessageHook		Set the error display implementation hook

	This routine sets the function name and data pointer used to
	write messages from LogMessage & LogSysMessage.

	Parameters:

		error_handler	A pointer to the user's error output
				routine.

		ptr		A pointer to an arbitrary data structure
				to be used by the user's error output
				routine.

	Returns nothing.
*/


/*
*		SetMessageHook(int(error_handler)(),voidptr)
*/
void	SetMessageHook(int (*error_handler)(), void *ptr)
{
	if(error_handler == (int (*)()) NULL) {
		message_func = use_stderr;
		message_data = NULL;
	} else {
		message_func = error_handler;
		message_data = ptr;
	}
}


/*
	GetMessageHook		Returns the current error display function.

	This routine returns to the user a pointer to the current error
	display function and it's parameter record.

	Parameters:

		error_handler	A pointer to a memory location where
				the function address is to be stored.

		ptr		A pointer to a location where the
				data pointer is to be stored.
*/

/*
*		GetMessageHook(int(error_handler)(),voidptr)
*/
void	GetMessageHook(int (**error_handler)(), void **ptr)
{
	*error_handler = message_func;
	*ptr = message_data;
}
