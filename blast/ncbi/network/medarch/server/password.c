/*
	password.c		Enter a password (no echo).

	MODULE:	 password
	FILES	 password.c (this one) and password.h.

	Obtain the password from the environment or from input from
	the user's terminal.

	This module was taken from the Backbone database project code
	in May 1991.  An interface file (password.h) the external entry
	points defined and comments were added to the source to bring the
	module up to Medline project standards.

	Edit History:

	   24 May 1991 - Rand S. Huntzinger, NLM/NCBI
		Integrated into the Medline project.

	    4 April 1990 - Rand S. Huntzinger
		Added support for the SYBASE_PASSWORD environment variable,
		which permits the password to be passed from a shell script
		without passing it on the command line which makes it
		trivially available via ps.

	Original version:	12 March 1990 - Rand S. Huntzinger, NLM/NCBI.

	ANSI CONVERSION NOTES:

	    18 Oct 1991 - Rand S. Huntzinger
		We are generating warnings because one of the include files
		in the system area has an illegal macro substitution.  Does
		this generate usable code?  I think so - I don't think the
		macro is used.
*
*
* RCS Modification History:
* $Log: password.c,v $
* Revision 6.0  1997/08/25 18:36:50  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:43  epstein
* add RCS log revision history
*
*/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/termios.h>
#include <sys/ttold.h>		/* For exclusive access - what is better? */
#include <fcntl.h>
#include "password.h"

#define index(s,c) strchr(s,c)

/* These variables are used to hold the TTY states for echo on (new state)
   and echo off (old state) so we can change and restore the TTY during
   the call. */

static	struct	termios	old_state;	/* Original TTY state */
static	struct	termios	new_state;	/* New TTY state */

/* This external defines the type of the getenv() procedure */

extern	char	*getenv();		/* Get an environment variable */


/*
	InitializeTtyPort		Turn off echo

	This procedure initializes the TTY port by turning off the
	echo so we can enter a password without seeing what was entered.

	Parameters:

	   tty		The integer file descriptor (system call level
			instead of standard i/o) of the tty to be
			changed.

	Returns:
	    1		If successful.
	    0		On an error.
*/

static
/*
*		InitializeTtyPort(inttty)
*/
int	InitializeTtyPort( int tty )
{
    /* Load the initial terminal state */

    if(ioctl( tty, TCGETS, &old_state ) < 0)
	return( 0 );
    new_state = old_state;		/* Duplicate for mods */

    /* Turn off echo */

    new_state.c_lflag &= ~( ISIG | ECHO );
    new_state.c_lflag |= ( FLUSHO | ECHONL );

    /* Change the settings */

    if(ioctl( tty, TCSETS, &new_state ) < 0)
	return( 0 );

    /* Done */

    return( 1 );
}


/*
	ResetTtyPort		Restore original TTY port settings

	This procedure resets the TTY port to the state which existed
	before the InitializeTtyPort call was executed.

	Parameters:

	   tty		The integer file descriptor (system call level
			instead of standard i/o) of the tty to be
			changed.

	Returns:
	    1		If successful.
	    0		On an error.
*/

static
/*
*		ResetTtyPort(inttty)
*/
int	ResetTtyPort( int tty )
{
    /* Flush the barcode Input buffer */

    if(ioctl( tty, TCSETS, &old_state ) < 0 )
	return( 0 );

    /* Done */

    return( 1 );
}


/*
	ClearTtyPort		Discard any pending TTY input

	This procedure deletes any input waiting in the TTY port's
	input queues.  This permits us to start input from a known
	state (buffers empty).

	Parameters:

	   tty		The integer file descriptor (system call level
			instead of standard i/o) of the tty to be
			changed.

	Returns:
	    1		If successful.
	    0		On an error.
*/

static
/*
*		ClearTtyPort(inttty)
*/
int	ClearTtyPort( int tty )
{
    /* Flush the tty input buffer */

    if(ioctl( tty, TCFLSH, 0 ) < 0)	/* Flush input queue */
	return( 0 );

    /* Done */

    return( 1 );
}


/*
	GetPassword		Read a line of text with echo disabled.

	This procedure reads a line of text from a TTY into a buffer
	with input echoing disabled.  It is normally used for entering
	passwords, for security reasons.  If the input port is not a
	terminal, the procedure will always return a NULL string.

	Parameters:

	    tty			A standard i/o file descriptor (type FILE)
				which is opened onto a terminal device.

	    buffer		The buffer to receive the input string.

	    buffer_size		The size of the input buffer in bytes.

	Returns:

	    A pointer to the input buffer.  The contents of the buffer
	    will be the string input minus the line terminator.  If
	    the file descriptor was not a TTY, the buffer will be
	    empty (*buffer == (char) 0).
*/

/*
*		GetPassword(FILEtty,charbuffer,intbuffer_size)
*/
char	*GetPassword( FILE *tty, char *buffer, int buffer_size )
{
    char	*p;

    /* Clear the buffer */

    *buffer = (char) 0;

    /* This only works on a TTY port */

    if( isatty( fileno(tty) ) )
    {
	InitializeTtyPort( fileno(tty) );
	ClearTtyPort( fileno(tty) );
	(void) fgets( buffer, buffer_size, tty );
	ResetTtyPort( fileno(tty) );
	if( (p = index( buffer, '\n' )) != 0 )
		*p = (char) 0;			/* Get rid of new line */
	fprintf( tty, "\r\n" );	
    }

    /* Done */

    return( buffer );
}


/*
	RequestPassword			Obtain a Sybase password.

	This procedure will obtain the Sybase password from the SYBASE_PASSWORD
	environment variable (if present) and then zip the environment
	variable out to avoid security problems using "ps -ae".  If the
	environment variable is not present, RequestPassword() will prompt
	the user for it and read the password with the terminal's echo
	turned off.

	Parameters:

	    prompt		The string to be used to prompt for the
				password.

	    buffer		The buffer to receive the input string.

	    buffer_size		The size of the input buffer in bytes.

	Returns:

	    A pointer to the buffer if the password was read or NULL
	    if there was an error (unable to access the TTY).  If
	    stdin is not a TTY, the buffer will always be returned
	    empty.  If the account has a password, it will have to
	    be specified via the command line or an environment
	    variable.
*/

/*
*		RequestPassword(charprompt,charbuffer,intbuffer_size)
*/
char	*RequestPassword( char *prompt, char *buffer, int buffer_size )
{
    char	*result;
    FILE	*fd;
    char	*p;

    /* Clear input buffer */

    *buffer = (char) 0;

    /*	SECURITY MEASURE: The SYBASE_PASSWORD environment variable avoids
	having to put the -Ppassword option on the command line where a
	routine ps can intercept it.  Unfortunately, it doesn't fully
	protect against ps -e.  Clearing the environment variable after
	use may at least limit liability though. */

    if( (p = getenv("SYBASE_PASSWORD")) != NULL && strlen( p ) <= buffer_size )
    {
	strcpy( buffer, p );
	while( *p ) *p++ = (char) 0x7f;	/* Zap SYBASE_PASSWORD for ps -e */
	return( buffer );
    }

    /* Open the TTY */

    if( ! isatty( fileno( stdin ) ) )
	return( buffer );
    fd = fopen( ttyname(fileno(stdin)), "w" );
    if( fd == (FILE *) NULL )
	return( NULL );

    /* Prompt the user */

    fprintf( fd, "%s", prompt );
    fflush( fd );

    /* Get the input */

    result = GetPassword( stdin, buffer, buffer_size );
    fprintf( fd, "\r\n" );

    /* Close up shop */

    (void) fclose( fd );

    /* Done */

    return( result );
}

