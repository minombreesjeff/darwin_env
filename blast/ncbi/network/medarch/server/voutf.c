/*
	voutf.c		Process formatted text through an output function.

	MODULE:	voutf
	FILES:	voutf.c (this one) and voutf.h.

	This package implements a flexible variable argument list
	output function similar to the System V vprintf function;
	however, it differs by using a function as an output sink
	instead of a string buffer or an output file.  This increases
	the flexibility of the package by allowing the supplied
	function to post-process and redirect the output as required
	by the application.

	Note:	This procedure was adapted from earlier projects and
		edited slightly to fit Medline project syle criteria.

	Edit History:

	    25 July 1991	Rand S. Huntzinger, NLM/NCBI
		Adapted to meet style criterial for modules in the
		Medline project.

	    14 Dec 1989		Modified to permit unlimited-length
				format statements and output of
				super long strings with unqualified
				%s format (ie. %s not %30s).

	Written: 5/21/87 by Rand S. Huntzinger
*
*
* RCS Modification History:
* $Log: voutf.c,v $
* Revision 6.0  1997/08/25 18:37:08  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:56:14  epstein
* add RCS log revision history
*
*/

#include	<stdio.h>
#include	<string.h>
#include	"voutf.h"

#define index(s,c) strchr(s,c)

/* Local definitions and declarations */

#define	MAX_ARG_EXPANSION	128	/* Largest single output field */
#define	MAX_FMT_ITEM		128	/* Largest single field format */
#define	EOS	'\0'			/* End of string marker */
#ifndef	TRUE
#define	TRUE	1			/* Boolean constants */
#define	FALSE	0
#endif

typedef	int	ifunc();

/* Macro to support error handling in the output function */

#define	OUTPUT(S) if((rv = out_func(S)) != 0) return (rv)

/* List of printf style format characters recognized by voutf */

static	char *fmt_chars = "sdcfgGeEduoxX%";

/* External functions required */


/*
	voutf		Variable argument list output function.

	This routine implements interprets a variable length argument
	list with respect to a format in a manner similar to the
	printf functions in the standard I/O package.  As each output
	field is interpreted, the resulting string is passed to a
	supplied output function, which processes it as required by
	the application [usually collects it into a string or outputs
	it].

	Parameters:

		out_func	The address of the function to be used
				as an output sink.  It is called for
				each output field (including constant
				fields).

		fmt		A printf-style format statement
				describing how to format the remaining
				arguments into strings to pass to
				out_func.

		args		A variable of type va_list (from a
				variable argument list in the calling
				program).

	Returns:

		0		If no error.
		NOT 0		An error status from out_func.
				Interpretation depends upon out_func.
*/

/*
*		voutf(int(out_func)(),charfmt,voidargs)
*/
int	voutf(int (*out_func)(), char *fmt, void *args)
{
	char	buf[MAX_ARG_EXPANSION+1];
	char	afmt[MAX_FMT_ITEM];
	int	rv;

	/* Scan the format and output text */

	rv = 0;			/* Assume success for now */
	while(*fmt) {
		char	*p;		/* Advancing char pointer */
		char	f_char;		/* Formatting character */
		short	longvar;	/* TRUE if a long integer */
		short	shortvar;	/* TRUE if a long integer */

		/* Variables to hold arguments of various types */

		double	dval;	char *	sval;
		int	ival;	long	lval;

		/* Non-formatting codes simply get output */
		if(*fmt != '%') {
			for(p = buf; *fmt && *fmt!= '%'; *p++ = *fmt++)
			    if(p == &buf[MAX_ARG_EXPANSION]) {
				*p = EOS;
				OUTPUT(buf);
				p = buf;
			    }
			*p = EOS;
			OUTPUT(buf);
		}
		/* Format items require conversion */
		if(*fmt == '%') {
			/* Extract the conversion specification */
			longvar = shortvar = FALSE;
			p = afmt;
			*p++ = *fmt++;
			while(*fmt && index(fmt_chars, *fmt) == NULL) {
				char ivstr[32], *q;
				if(*fmt == '*') {
					/* Get format width from data */
					ival= va_arg(args, int);
					sprintf(ivstr, "%d", ival);
					for(q=ivstr; *q; ) *p++ = *q++;
					fmt++;		/* Skip * */
					continue;
				}
				if(*fmt == 'l') longvar = TRUE;
				if(*fmt == 'h') shortvar = TRUE;
				*p++ = *fmt++;
			}
			*p++ = f_char = *fmt;
			*p = EOS;
			if(*fmt) fmt++;
			/* Convert next argument by format */
			switch(f_char) {
			  case '%':	/* Quoted % */
				strcpy(buf, "%");
				break;
			  case 'd':	/* Integer conversions */
			  case 'o':  case 'u':  case 'x':  case 'X':
			  case 'c':	/* char is an integer on stack */
				if(longvar) {
					lval = va_arg(args, long);
					sprintf(buf, afmt, lval);
				} else if(shortvar) {
					lval = va_arg(args, int);
					sprintf(buf, afmt, lval);
				} else {
					ival = va_arg(args, int);
					sprintf(buf, afmt, ival);
				}
				break;
			  case 'f':	/* Floating point conversions */
			  case 'e':  case 'E':  case 'g':  case 'G':
				/* NOTE - PROBLEMS if float <> double? */
				dval = va_arg(args, double);
				sprintf(buf, afmt, dval);
				break;
			  case 's':	/* String argument */
				sval = va_arg(args, char *);
				if(strcmp(afmt, "%s") == 0) {
				    OUTPUT( sval ); /* Unrestricted string */
				    continue;
				} else sprintf(buf, afmt, sval);
				break;
			  case EOS:	/* End of string */
			  default:	/* Unexpected character */
				strcpy(buf, afmt);
				break;
			}
			/* Output the resulting string */
			OUTPUT(buf);
		}
	}

	/* Exit at this point */

	return( rv );
}


/*
	voutput		Output with variable argument list.

	Call voutf with a direct variable argument list.  This is
	used when the function is not called from a variable argument
	list function.

	Parameters:

		out_func	The address of the function to be used
				as an output sync.  It is called for
				each output field (including constant
				fields).

		fmt		A printf-style format statement
				describing how to format the remaining
				arguments into strings to pass to
				out_func.

		args ...	The remaining arguments are interpreted
				according to the format passed as 'fmt'.

	Returns:

		0		If no error.
		NOT 0		An error status from out_func.
				Interpretation depends upon out_func.
*/

/*
*		voutput(int(out_func)(),charfmt,...)
*/
int	voutput(int (*out_func)(), char *fmt, ...)
{
	va_list	args;
	int	rv;

	/* Extract the fixed parameters */

	va_start(args, fmt);

	/* Now use voutf */

	rv = voutf(out_func, fmt, args);
	va_end( args );

	/* Done */

	return( rv );
}
