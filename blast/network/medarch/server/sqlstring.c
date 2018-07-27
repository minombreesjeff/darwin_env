/*
	sqlstring.c		Translate C strings into a form where they
				can be passed as part of SQL commands.

	MODULE:	sqlstring
	FILES:	sqlstring.c (this one) and sqlstring.h.

	This package translates strings passed to it into a form suitable
	for use in SQL commands.  In particular, it doubles quote characters
	within strings so that the string will not cause SQL command errors.

	In order to make the package maximally useful, the package will
	allocate data for each string which has to be modified, and then
	all allocated strings can be freed with a single call after the
	fact.  This allows the command to be used multiple times within
	a single worrying about freeing up multiple buffers.

	Note:	This module was adapted from the module of the same
		name from the GenInfo Backbone database project.  It
		had to be slightly modified to meet Medline project
		coding conventions.  (module nomenclature)

	Edit History:
	     7 August 1991 - Rand S. Huntzinger, NLM/NCBI
		Added the SQLbinary procedure, which allows us to use the
		same procedures for handling binary types.

	    25 July 1991 - Rand S. Huntzinger, NLM/NCBI
		Modified to comply with Medline project coding conventions.

	Original version: 22 Mar 91, Rand S. Huntzinger, NLM/NCBI
*
*
* RCS Modification History:
* $Log: sqlstring.c,v $
* Revision 6.0  1997/08/25 18:36:56  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:55  epstein
* add RCS log revision history
*
*/

#include <string.h>
#include "xalloc.h"

#define index(s,c) strchr(s,c)

#ifndef	NULL
#define	NULL	((char *) 0)
#endif

/* The StringList type is simply a singly linked list of string pointers
   (char *) which we used to keep track of the strings which are to be
   cleared by the next SQLstringClear() operation. */

typedef	struct StringList {
	struct	StringList	*next;
	char			*text;
} StringList;

static	StringList	*free_list = (StringList *) NULL;
static	StringList	*in_use = (StringList *) NULL;


/*
	SQLstringAlloc		INTERNAL string allocation function.

	This procedure allocates a string of a given size and places it
	into the in_use string list.  This list can later be cleared by
	the SQLstringClear() or SQLstringReset() calls.

	Parameters:

	    size		The size of the string to be allocated.

	Returns:

	    A pointer to the allocated string.  Should not be NULL unless
	    the xalloc() support routine can return NULL, which should not
	    normally be the case.
*/

static
/*
*	SQLstringAlloc(intsize)
*/
char *SQLstringAlloc( int size )
{
    register	StringList	*node;

    /* Is there a free node? */

    if( free_list != (StringList *) NULL ) {
	node = free_list;
	free_list = node->next;
    } else {
	node = (StringList *) xalloc( sizeof( StringList ) );
    }

    /* Allocate the text */

    node->text = xcalloc( size + 1, 1 );

    /* Insert the node into the in_use list */

    node->next = in_use;
    in_use = node;

    /* Return a pointer to the text */

    return( in_use->text );
}


/*
	SQLstringFree		Free a string allocated with SQLstring

	This procedure finds the string allocated by the SQLstring function
	and frees it.  This involves calling xfree() on the text of the
	string and putting the link structure into the free list.

	This procedure is not normally used.  Normally one would use
	SQLstringClear() to free up all of the strings generated for
	a command or a series of commands.

	NOTE:	Since SQLstring does not always allocate a string, this
		procedure will not always do anything.  You can safely
		pass strings not allocated by SQLstring; however, they
		will not be freed.

	Parameters:

		text		The address of the allocated string to
				be freed.

	Returns:	nothing
*/

/*
*		SQLstringFree(chartext)
*/
void	SQLstringFree( char *text )
{
    register	StringList	*node, *p;

    /* If the string list is empty, simply exit */

    if( in_use == (StringList *) NULL ) return;

    /* Unlink the node from the in_use list */

    if( in_use->text == text ) {
	/* Special case, first in the list */
	node = in_use;
	in_use = node->next;
    } else {
	/* Not first, unlink from the middle or end */
	node = (StringList *) NULL;
	for( p = in_use; p->next != (StringList *) NULL; p = p->next )
	    if( p->next->text == text ) {
		node = p->next;
		p->next = node->next;
		break;
	    }
    }

    /* If we found it, free it */

    if( node != (StringList *) NULL ) {
	xfree( node->text );
	node->next = free_list;
	free_list = node;
    }

    /* Done */

    return;
}


/*
	SQLstringClear		Free memory allocated by SQLstring calls

	This command frees all strings allocated by SQLstring calls.
	The normal procedure would be to generate the strings to be used
	in a SQL command (often in the argument list using SQLstring as
	a "wrapper") and then call SQLstringClear when the command, or a
	series of commands have been completed.

	Parameters:	none
	Returns:	nothing
*/

/*
*		SQLstringClear()
*/
void	SQLstringClear()
{
    register	StringList	*node;

    /* Free all text areas allocated */

    while( in_use != (StringList *) NULL ) {
	xfree( in_use->text );			/* Free the text */
	node = in_use;				/* Unlink the node */
	in_use = in_use->next;
	node->next = free_list;			/* Put node in free list */
	free_list = node;
    }
}


/*
	SQLstringReset		Free all memory allocated by SQLstring package.

	This procedure is slightly more radical than SQLstringClear.  It
	calls SQLstringClear to free all of the text strings allocated by
	SQLstring calls and then frees the link nodes as well.  Normally
	the link nodes are kept around for reuse.

	This procedure is seldom used.  The link nodes do not consume much
	space and reusing them saves xalloc and xfree calls.  It can be used
	if we're keeping allocation statistics and we want to make sure all
	allocated data has been freed.

	Parameters:	none
	Returns:	nothing
*/

/*
*		SQLstringReset()
*/
void	SQLstringReset()
{
    register	StringList	*node;

    /* Free all existing strings */

    SQLstringClear();

    /* Clear the free list */

    while( free_list != (StringList *) NULL ) {
	node = free_list;
	free_list = free_list->next;
	xfree( (char *) node );
    }
}


/*
	SQLstring		Translate a C string to a SQL-usable form.

	This procedure processes a C language string (a pointer to text
	terminated by a zero byte) into a SQL-usable string by truncating
	the string (if necessary) and doubling quotes to avoid SQL command
	errors.  We allocate strings if necessary, so one must clean up
	afterwards using the SQLstringClear() function.

	Parameters:
	    text		A pointer to the text string to be processed.

	    width		The maximum width of the string to be
				passed to the server.  If negative, the
				width of the text will be used.

	    quote		The character to be used in quoting the
				string.  This character will be doubled
				in the sting itself.

	Returns:

	    A pointer to the processed string.  It may be the same as
	    the input string, or may be allocated in the string space.
*/
	    
/*
*		SQLstring(chartext,intwidth,intquote)
*/
char	*SQLstring( char *text, int width, int quote )
{
    register	char	*p, *np, *ep;
    char		*new;
    register	int	 n;
    int			 tw;

    /*	If there are no embedded quotes, simply return the text.
	Happly, this should be the typical case. */

    p = index(text, quote);
    tw = strlen(text);
    if( (width < 0 || tw <= width) && p == NULL )
	return( text );

    /*	Figure out how long the result string will have to be. */

    tw = (tw > width && width > 0) ? width : tw;
    ep = text + tw;
    for(n = 0; p != NULL && p < ep; p = index(p + 1, quote))
	n++;

    /*	Create a new string with the proper length and quote doubling */

    new = SQLstringAlloc( tw + n );
    for( p = text, np = new; tw-- > 0; *np++ = *p++ )
	if( *p == quote ) *np++ = *p;
    *np = (char) 0;

    return( new );
}


/*
	SQLbinary		Translate a binary buffer SQL-usable form.

	This procedure is analogous to the SQLstring procedure for text,
	except it prepares data to be loaded into SQL binary or image
	types instead of char, varchar or text.  The result is 0x followed
	by an arbitrary length string of hexadecimal digits, each pair of
	which represent one input byte.  This procedure will always allocate
	a new buffer, so the calling program will have to clean up using
	the SQLstringClear() procedure.

	Parameters:

	    buffer		A pointer to a buffer containing the binary
				text to be converted into a SQL usable string.

	    length		The number of bytes in the binary string
				to be converted.

	Returns:

	    A pointer to the processed string.
*/
	    
/*
*		SQLbinary(unsignedcharbuffer,intlength)
*/
char	*SQLbinary( unsigned char *buffer, int length )
{
    char			*new;
    static char			 prefix[] = "0x";
    static char			 hex[] = "0123456789ABCDEF";
    register unsigned char	*inp;
    register char		*outp;

    /*	Allocate space for the hex string output.  It requires 2 bytes for
	each input byte, plus 2 bytes for the 0x and one byte for the
	string terminating null. */

    if( length < 0 ) length = 0;
    new = SQLstringAlloc( 2 * length + 3 );

    /* Convert the binary buffer into a string */

    strcpy( new, prefix );
    for( inp = buffer, outp = new + strlen(prefix); length-- > 0; inp++ ) {
	*outp++ = hex[(*inp >> 4) & 0x0f];
	*outp++ = hex[*inp & 0x0f];
    }
    *outp = (char) 0;

    /* Done */

    return( new );
}
