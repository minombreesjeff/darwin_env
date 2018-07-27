/*
	xalloc.c		Extended memory allocation routines.

	This package provides an interface to the standard Unix memory
	allocation routines to give better error handling facilities
	as well as facilities for keeping track of heap usage.

	History:

	  10/18/91	Converted to ANSI C.

	  12/31/86	Rand S. Huntzinger - Original version.

	Basic entry points:

	  xalloc(size)		  Allocate size bytes (malloc equivalent)
	  xrealloc(p, size)	  Reallocate ptr to size bytes (realloc).
	  xcalloc(nelem, esize)	  Allocate an array of elements (calloc).
	  xfree(p)		  Free space at this pointer (free).
	  xmemerr(func)		  Pass name of function to call on error.
	  xmemstat(srec)	  Keep memory access statistics.
	  xnullok(null_ok)	  If argument TRUE, allow xalloc routines
				  to return NULL, else abort the program.

	NOTE:
	Associated packages:

	  heapman	The heap manager.  Records memory requests and
			keeps records of what needs to be freed.

	  malloc	System package for memory allocation.  This
			package interfaces to these routines.

	---------------------------------------------------------------

	The error hook: (routine to call on an error).

	The error hook function is called whenever a call to one of the
	system memory allocation routines fails (returns NULL).  The
	error hook can do several things.  It can free some memory and
	attempt to reallocate.  It can issue an error diagnostic and
	either return NULL to the calling program, or it can abort the
	program via exit or a quit signal (to force a core dump).

	The error hook is passed the following parameters:

	  ptr		Type (char *).  If this was a realloc call,
			this is the pointer to reallocate; otherwise,
			NULL is passed.

	  nelem		Type size_t.  The number of elements to allocate.
			If a calloc call, the 1st argument to calloc is
			passed; otherwise, a 1 is passed.

	  size		Type size_t.  The number of bytes to be allocated
			for each element if calloc, the allocation size if
			malloc or realloc.

	  errnum	The number of the error returned by the allocation
			routine.

	  func		Type (char *).  The name of the function which
			returned the error as a text string.  Always one
			of "malloc", "calloc" or "realloc".  You can
			determine which one by testing the first character.

	If the error hook returns, it should return:

		A pointer to the allocated memory of the error hook found
		a way to allocate it or NULL if the memory is still
		unallocated. 
*
*
* RCS Modification History:
* $Log: xalloc.c,v $
* Revision 6.0  1997/08/25 18:37:12  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:56:20  epstein
* add RCS log revision history
*
*/

#include	<stdlib.h>
#include	<stdio.h>		/* For error message printout */
#include	"xalloc.h"		/* Header file for these routines */

#ifndef	NULL
#define	NULL	0
#endif
#ifndef	TRUE
#define	TRUE	1
#endif
#ifndef	FALSE
#define	FALSE	0
#endif
#ifndef	ALLOC_ERR
#define	ALLOC_ERR	255
#endif

/* These declarations are used to define the types of the underlying Unix
   memory allocation functions.  They'll probably change soon of course
   with the new type codes becoming available. */

extern	int	errno;			/* Unix error number */

/* These variables hold the data stored by the hook setting functions.  The
   error logging procedure, the statistics accumulator buffers and the
   variable which permits xalloc routines to return NULL */

static	XMESTAT	*mestat = NULL;		/* Memory status register */
static	CPFUNC	*err_hook = NULL;	/* Error function */
static	short	null_is_OK = FALSE;	/* If TRUE, we'll return with NULL */


/*
	err_handler		Error handler function.

	This routine calls the user's error handler function and
	handles errors if necessary.  In particular it dispatches to
	the proper error handler and makes sure than we never return
	NULL unless we do something special to permit it.

	Parameters:

	  ptr		Type (char *).  If this was a realloc call,
			this is the pointer to reallocate; otherwise,
			NULL is passed.

	  ni		Type size_t.  The number of elements to allocate.
			If a calloc call, the 1st argument to calloc is
			passed; otherwise, a 1 is passed.

	  sz		Type size_t.  The number of bytes to be allocated
			for each element if calloc, the allocation size if
			malloc or realloc.

	  en		The number of the error returned by the allocation
			routine.

	  fn		Type (char *).  The name of the function which
			returned the error as a text string.  Always one
			of "malloc", "calloc" or "realloc".  You can
			determine which one by testing the first character.

	Returns:

	    The value returned by the error handler procedure called.
	    We do not exit (ie. we abort the program) if the null_is_OK
	    variable is false (the normal case) and err_handler returns
	    NULL.
*/

static
/*
*		err_handler(charptr,size_tni,size_tsz,size_ten,charfn)
*/
char	*err_handler(char *ptr, size_t ni, size_t sz, size_t en, char *fn)
{
	char	*p;			/* Temporary pointer */

	/* Call the user error handler (if any) */

	if(err_hook != NULL)
		p = err_hook(ptr, ni, sz, en, fn);
	else if(! null_is_OK) {
		/* This is the default error handler! */

		fprintf(stderr, "Memory allocation error: ");
		switch(*fn) {
		  case 'm':	/* malloc */
			fprintf(stderr, "malloc(size=%d)\n", sz); break;
		  case 'r':	/* realloc */
			fprintf(stderr, "realloc(ptr=%X, size=%d)\n", ptr, sz);
			break;
		  case 'c':	/* calloc */
			fprintf(stderr, "calloc(n=%d, size=%d)\n", ni, sz);
			break;
		  default:	/* Weird value */
			fprintf(stderr, "function = %s\n", fn);
		}
		fprintf(stderr, "Exiting program\n");
		p = NULL;		/* Failure */
	}

	/* Do we permit NULL to be returned? */

	if(p != NULL || null_is_OK)
		return(p);

	exit(ALLOC_ERR);		/* Exit status for alloc error */
	/*NOTREACHED*/
}

	
/*
	xalloc			Allocate a block of a given size.

	This routine uses malloc to allocate a block of memory of a
	given size.  If an error hook function exists, it is called
	prior to issuing a diagnostic.  Memory statistics are kept
	if this feature has been enabled.

	Parameters:

	  size		size_t	The number of bytes to allocate.

	Returns:

	  A pointer to the block of memory allocated or NULL if the
	  allocation failed and the error hook returned NULL.
*/

/*
*		xalloc(size_tsize)
*/
char	*xalloc(size_t size)
{
	register char	*p;

	/* Do the actual memory allocation */

	if((p = malloc(size)) == NULL) {
		/* Failure, call the error hook */
		if(mestat != NULL) mestat->xerrcals++;
		p = err_handler(p, (size_t) 1, size, errno,
			"malloc");
	}

	/* Record statistics */

	if(mestat != NULL) {
		mestat->xallocs++;
		if(p == NULL) mestat->xfails++;
	}

	/* Exit with the pointer */

	return(p);
}


/*
	xcalloc			Allocate an array of a given size.

	This routine uses calloc to allocate an array of memory a
	given size.  If an error hook function exists, it is called
	prior to issuing a diagnostic.  Memory statistics are kept
	if this feature has been enabled.

	Parameters:

	  nelem		size_t	The number of items in the array.

	  size		size_t	The number of bytes to allocate for
					each item.

	Returns:

	  A pointer to the block of memory allocated or NULL if the
	  allocation failed and the error hook returned NULL.
*/

/*
*		xcalloc(size_tnelem,size_tsize)
*/
char	*xcalloc(size_t nelem, size_t size)
{
	register char	*p;

	/* Do the actual memory allocation */

	if((p = calloc(nelem, size)) == NULL) {
		/* Failure, call the error hook */
		if(mestat != NULL) mestat->xerrcals++;
		p = err_handler(p, nelem, size, errno, "calloc");
	}

	/* Record statistics */

	if(mestat != NULL) {
		mestat->xcallocs++;
		if(p == NULL) mestat->xfails++;
	}

	/* Exit with the pointer */

	return(p);
}

/*
	xrealloc		Adjust a pointers memory allocation.

	This routine uses realloc to alter the block of memory associated
	with a given memory pointer.  If an error hook function exists, it
	is called prior to issuing a diagnostic.  Memory statistics are
	kept if this feature has been enabled.

	Parameters:

	  ptr		(char *)	The number of items in the array.

	  size		size_t	The number of bytes to allocated
					to this pointer after the call.

	Returns:

	  A pointer to the block of memory allocated or NULL if the
	  allocation failed and the error hook returned NULL.
*/

/*
*		xrealloc(voidptr,size_tsize)
*/
char	*xrealloc(void *ptr, size_t size)
{
	register void	*p;

	/* Do the actual memory allocation */

	if((p = realloc(ptr, size)) == NULL) {
		/* Failure, call the error hook */
		if(mestat != NULL) mestat->xerrcals++;
		p = err_handler(ptr, (size_t) 1, size, errno,
			"realloc");
	}

	/* Record statistics */

	if(mestat != NULL) {
		mestat->xreallocs++;
		if(p == NULL) mestat->xfails++;
	}

	/* Exit with the pointer */

	return(p);
}


/*
	xfree			Free memory allocated by malloc.

	This routine frees a block of memory allocated by malloc.
	It is identical to the system free routine except that
	statistics are logged and the system avoids passing NULL
	to free.  [On some systems this is OK, on others, this
	can be disasterous!]

	Parameters:

		ptr		Pointer to the data to be freed.

	Returns:

		Nothing.  No errors are returned since I see no
		consistent definition of errors from free itself.
*/

/*
*		xfree(voidptr)
*/
void	xfree(void *ptr)
{
	/* If the pointer is not NULL, free it */

	if(ptr == NULL) {
		if(mestat != NULL) mestat->xfree0s++;
	} else {
		(void) free(ptr);
		if(mestat != NULL) mestat->xfrees++;
	}
}


/*
	xmemstat	Enable/disable memory allocation statistics

	This procedure is used to enable or disable memory allocation
	statistics and to clear the counters (when desired).  The
	procedure passes a pointer to the buffer to receive the statistics
	and returns the previous pointer.  This allows the calling program
	to swap out a counter temporarily.

	Parameters:

	    sptr		A pointer to the statistics accumulation
				buffer to be used for future calls or
				NULL to disable statistics gathering.

	    zero		True => zero the accumulators before
				collecting statistics.  Otherwise, future
				calls will be added to the data in the
				buffer.

	Returns:

	    A pointer to the statistics accumulation buffer used prior
	    to this call or NULL if statistics were disabled.
*/
	    
/*
*		xmemstat(sptr,zero)
*/
XMESTAT	*xmemstat(sptr, zero)
	XMESTAT	*sptr;
	int	zero;
{
	XMESTAT	*tmp;

	/* Update the memory allocation statistics record */

	tmp = mestat;
	mestat = sptr;
	if(sptr != NULL && zero)
		sptr->xallocs = sptr->xcallocs = sptr->xreallocs =
        	sptr->xfrees  = sptr->xfree0s  = sptr->xerrcals  =
        	sptr->xfails  = 0;

	return(tmp);		/* Return previous value */
}


/*
	xmemerr		Replace the memory allocation error handler.

	This procedure installs the procedure passed as an argument as the
	memory allocation error handler and returns the address of the
	previously defined error handler (so it can be reestablished later
	if necessary).  This procedure will be called, with a standard set
	of arguments, when a memory allocation of free function fails.

	Parameters:

	    func		The address of the new error handler
				function.

	Returns:

	    The address of the the error handler which was in place before
	    the call.  The default error handler is returned as NULL.
*/

/*
*		xmemerr(func)
*/
CPFUNC	*xmemerr(func)
	CPFUNC	*func;
{
	CPFUNC	*tmp;

	/* Save the new function and return the old one */

	tmp = err_hook;
	err_hook = func;
	return(tmp);
}


/*
	xnullok		Permit xalloc routines to return NULL or not.

	This procedure is used to permit the xalloc returns to return
	NULL if a memory allocation of free operation fails.  The default
	setting causes xalloc to exit the program if a memory allocation
	fails.

	Parameters:

	    null_ok		If True, the xalloc routines can return
				null if a memory allocation fails.  If
				False, the xalloc routines will cause the
				program to exit on a memory allocation
				failure.

	Returns:

	    The setting of this parameter prior to this call.
*/

/*
*		xnullok(null_ok)
*/
int	xnullok(null_ok)
	int	null_ok;
{
	int	x;
	x = null_is_OK;
	null_is_OK = (null_ok != 0);
	return(x);
}
