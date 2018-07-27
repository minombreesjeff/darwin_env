/*
	PROJECT: Medline database.
	MODULE:	 cspatch
	FILES:	 cspatch.c cstopatch.c, patchtoiso.c (this one),, cspatch.h

	This file contains support procedures for translating ASCII strings
	with their associated ISO patch strings into the richer ISO-8859
	character set.  The ISO-8859 character set is a superset of the
	ASCII character which includes many accented international characters.
	The ISO patch instructions are generated at the time the text is
	converted from the Medline EBCDIC character set into ASCII before
	loading the text into the database.  In addition to describing how
	to patch the ASCII text to produce the ISO character set, it also
	contains data describing EBCDIC characters which could not be
	represented in the ISO-8859 character set.  These can be used in
	the future if character sets become available which can be used
	to support these untranslated characters.

	Work started: 29 June 1991, Rand Huntzinger
	Original version completed: 24 July 1991, Rand S. Huntzinger
*
*
* RCS Modification History:
* $Log: patchtoiso.c,v $
* Revision 6.0  1997/08/25 18:36:53  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:49  epstein
* add RCS log revision history
*
*/

#include <stdio.h>
#include "sybase.h"
#include "patchtoiso.h"
#include "error.h"

/*
	ApplyIsoPatches		Translate and ASCII string into ISO-8859

	This procedure takes an ASCII input string and the ASCII->ISO-8859
	patch string associated with that ASCII input string and converts
	the text to the ISO character set.  It ignores the ISO patch codes
	which indicate untranslated EBCDIC characters during the EBCDIC
	to ASCII conversion.

	Parameters:

	    in			A pointer to the ASCII input string.

	    in_size		The number of characters in the ASCII
				input string.  [We don't assume NULL
				termination]

	    out			A pointer to the buffer to receive
				the ISO-8859 representation of the
				text created by this procedure.

	    out_size		The size of the output buffer.

	    patches		A pointer to the ASCII->ISO patch
				string which contains the conversion
				instructions.

	Returns:

	    The number of errors noted in the conversion.  Errors include
	    buffer overflows and invalid patch instructions.
*/

int 
/*
*	(charin,intin_size,charout,intout_size,unsignedcharpatches)
*/
ApplyIsoPatches (char *in, int in_size, char *out, int out_size, unsigned char *patches)
{
    int	ii, oi, pi;			/* in, out and patch indices */
    int running = TRUE;
    static unsigned char abort[] = { POP_STOP_CODE };
    int	   errors = 0;

    for(ii = oi = pi = 0; running; pi++) {
	register int	code = (int) patches[pi];
	register int	base_code;
	register int	advance = 0;		/* Set the character pointer */
	register int	skip = 0;		/* # input chars to skip */
	register int	install = 0;		/* # patch chars to copy in */

	/* Decode the basic opcode */

	if( (base_code = code & POP_BASIC_MASK) == POP_SPECIAL_GRP )
	    if( (base_code = code & POP_SPEC_MASK) == POP_MISC_GRP )
		if( (base_code = code & POP_MISC_MASK) == POP_SIMPLE_GRP )
		    base_code = code;

	/* Process the opcode */

	switch( base_code ) {
	    case POP_PATCH_CODE:
		skip = POP_PATCH_DELETE( code );
		install = POP_PATCH_INSERT( code );
		break;
	    case POP_OFFSET1_CODE:
		advance = code & POP_BASIC_ARGMASK;
		break;
	    case POP_OFFSET2_CODE:
		advance = code & (POP_BASIC_ARGMASK << 8) | patches[++pi];
		break;
	    case POP_CSET_CODE:
		break;			/* Not implemented */
	    case POP_OMIT_CODE:
	    case POP_INVALID_CODE:
	    case POP_NOISO_CODE:
		/* These are various types on uninterpretable codes.  They
		   are recorded for the future, but are ignored on output. */
		pi += code & POP_MISC_ARGMASK;
		break;
	    case POP_STOP_CODE:
		advance = in_size - ii;		/* Copy over rest of string */
		running = FALSE;
		break;
	    default:	/* Anything else is an error! */
		LogMessage( CHARSET_ERROR, "patchtoiso",
		    "Invalid ASCII->ISO patch instruction [%02x]", code );
		advance = in_size - ii;		/* Copy over rest of string */
		patches = abort;		/* Terminate patching */
		errors++;			/* Mark an error */
	}

	/* Check for buffer overflows */

	if( (ii + advance + skip) > in_size ) {
	    LogMessage( CHARSET_ERROR, "patchtoiso",
	        "Patch table input offset (%d) outside input string [%-.16s%s]",
	    ii + advance + skip, in, (in_size < 16) ? "" : "..." );

	    /* Adjust pointers to deal with overflow */

	    if( (ii + advance) > in_size ) {
		advance = in_size - ii;
		install = 0;
	    }
	    if( (ii + advance + skip) > in_size )
		skip = in_size - ii - advance;
	    patches = abort;			/* No more patches */
	    errors++;				/* Mark an error */
	}
	if( (oi + advance + install) > out_size ) {
	    LogMessage( CHARSET_ERROR, "patchtoiso",
	        "ISO translation of \"%-.16s%s\" truncated to %d characters",
	    in, (in_size < 16) ? "" : "..." );

	    /* Adjust pointers to deal with overflow */

	    if( (oi + advance) > out_size ) {
		advance = out_size - oi;
		install = 0;
	    }
	    if( (oi + advance + install) > out_size )
		install = out_size - oi - advance;
	    patches = abort;			/* No more patches */
	    errors++;				/* Mark an error */
	}

	/* Advance to the proper position */

	while( advance-- > 0 )
	    out[oi++] = in[ii++];
	ii += skip;

	/* Install patch characters */

	while( install-- > 0 )
	    out[oi++] = patches[++pi];
    }

    /* Terminate the output string */

    out[oi++] = (char) 0;

    return( errors );
}
