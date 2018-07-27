/*
	sybbind.c	High level Sybase row loading and binding support

	MODULE:	sybbind
	FILES:	sybbind.c (this one) and sybbind.h.

	This module provides a high-level interface to the Sybase procedures
	for binding rows to C variables and for loading rows in form the
	database.  These procedures permit a single procedure call to bind
	many variables in a single call.  The SybaseLoad procedure call
	adds row retrieval as well.  This reduce the amounts of coding
	detail required by lumping the dbbind() and dbnextresults() calls
	into a single call with only a single return code to check.

	Note:	This module was adapted from the module of the same
		name from the GenInfo Backbone database project.  It
		had to be slightly modified to meet Medline project
		coding conventions.  (module nomenclature)

	Edit History:
	    25 July 1991 - Rand S. Huntzinger, NLM/NCBI
		Modified to comply with Medline project coding conventions.

	Original version: circa 1989-1990, Rand S. Huntzinger, NLM/NCBI
*
*
* RCS Modification History:
* $Log: sybbind.c,v $
* Revision 6.0  1997/08/25 18:37:01  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:56:03  epstein
* add RCS log revision history
*
*/

#include <stdarg.h>
#include "sybase.h"
#include "sybbind.h"

/*
	VSybaseBind		Bind Sybase Fields to Variables (from vector)

	This procedure takes a vector of arguments which originated
	from a variable argument list from a calling procedure and uses
	it to call dbbind() several times to bind variables for loading
	from a Sybase database on the next dbgetnextrow() call.

	Interpretation of the argument vector:

	The argument vector contains groups of four parameters defined
	as the last four parameters in the Sybase dbbind procedure.  These
	parameters are:

	    Field		Contents
	    =====		========
	    field_no		The field number.  This must be a positive
				integer.  When this value is zero, we
				assume we've reached the end of the list.

	    type		The binding type of the field (see dbdind()
				documentation for valid values).

	    length		The length of the field.  Zero can be used
				if the length is implied by the type or
				the buffer used to hold the input is known
				to be long enough to hold the data.

	    varptr		A pointer to the location where the data
				retrieved from the database is to be
				stored.  It must be at least length bytes
				long, or long enough to hold this type.

	Note that we terminate processing when field_no is set to zero.
	Thus, the calling program should make sure that a zero argument
	is passed after the last valid group.

	Parameters:

	    dbproc		A pointer to the Sybase database process.

	    args		The argument vector which is interpreted
				as given above.

	Returns:

	    SUCCEED if all binds are successful, or the return code of
	    first dbbind() call which does not return SUCCEED (ie. which
	    fails).
*/

/*
*		VSybaseBind(dbproc,args)
*/
RETCODE	VSybaseBind( dbproc, args )
	DBPROCESS	*dbproc;
	va_list		 args;
{
    int		 field_no;
    int		 type;
    int		 length;
    char	*varptr;
    RETCODE	 status;

    /* Assume sets of field#, type, length, variable */

    for( status = SUCCEED; status == SUCCEED; )
    {
	/* When we get to a zero field code, we're at the end of
	   the variable length input list */

	field_no = va_arg( args, int );
	if( field_no == 0 ) break;

	/* Load the other arguments */

	type = va_arg( args, int );
	length = va_arg( args, int );
	varptr = va_arg( args, char *);	/* Must be a pointer! */

	/* Bind us up */

	status = dbbind(dbproc, field_no, type, length, varptr );
    }

    /* Done */

   return( status );
}


/*
	SybaseBind		Bind Sybase Fields to Variables

	This is a variable argument list procedure which repeatedly
	calls the Sybase dbbind() procedure to bind sets of parameters
	coded in it's variable argument list.  A single zero parameter
	following the last valid group terminates the list of arguments.

	Normally, the argument list of this function is coded as a
	table, with arguments for a single bind on each row, and the
	parameters for each bind in the columns.  This simply provides
	a neater way of coding the binds.

	Interpretation of the argument list:

	The first argument passed is a pointer to the Sybase database
	process where the binding is to be done.  The remaining parameters
	are grouped in sets of four, which are interpreted as follows:

	    Field		Contents
	    =====		========
	    field_no		The field number.  This must be a positive
				integer.  When this value is zero, we
				assume we've reached the end of the list.

	    type		The binding type of the field (see dbdind()
				documentation for valid values).

	    length		The length of the field.  Zero can be used
				if the length is implied by the type or
				the buffer used to hold the input is known
				to be long enough to hold the data.

	    varptr		A pointer to the location where the data
				retrieved from the database is to be
				stored.  It must be at least length bytes
				long, or long enough to hold this type.

	A single zero parameter following the last valid group (which
	ends up as field_no == 0, terminates the argument list.

	[NOTE:	This is simply a variable argument list wrapper for
		the VSybaseBind() procedure.]

	Returns:

	    SUCCEED if all binds are successful, or the return code of
	    first dbbind() call which does not return SUCCEED (ie. which
	    fails).
*/

/*
*		SybaseBind(DBPROCESSdbproc,...)
*/
RETCODE	SybaseBind( DBPROCESS *dbproc, ... )
{
    va_list	 args;
    RETCODE	 status;

    /* Extract the argument */

    va_start( args, dbproc );

    /* Use the vector routine to handle everything else */

    status = VSybaseBind( dbproc, args );
    va_end( args );

    /* Done */

   return( status );

}


/*
	SybaseLoad	Bind Sybase fields to data and Load a Record

	This procedure is identical to Sybase bind except that the
	dbnextrow() procedure is called after binding to load the
	bound variables from a row in the database.

	Parameters:

	    Identical to SybaseBind, see the comment on that procedure
	    (above) for details.

	Returns:

	    Identical to Sybase bind except that the return code is
	    the value returned from the dbnextrow() call unless one of
	    the bind operations fail.  In that case, the return code
	    is the value returned by the failed bind (should be FAIL).
*/

/*
*		SybaseLoad(DBPROCESSdbproc,...)
*/
RETCODE	SybaseLoad( DBPROCESS *dbproc, ... )
{
    va_list	 args;
    RETCODE	 status;

    /* Extract the 'dbproc' argument */

    va_start( args, dbproc );

    /* Use the vector routine to handle everything else */

    status = VSybaseBind( dbproc, args );
    va_end( args );
    if( status != SUCCEED ) return( status );

    /* Load a row from the database and exit */

   return( dbnextrow( dbproc ) );

}
