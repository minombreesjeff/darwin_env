/*---------------------------------------------------------------------------*\
  $Id: poll.c,v 1.2 2004/02/06 22:31:14 eseidel Exp $

  NAME

	poll - select(2)-based poll() emulation function for BSD systems.

  SYNOPSIS
	#include "poll.h"

	struct pollfd
	{
	    int     fd;
	    short   events;
	    short   revents;
	}

	int poll (struct pollfd *pArray, unsigned long n_fds, int timeout)

  DESCRIPTION

	This file, and the accompanying "poll.h", implement the System V
	poll(2) system call for BSD systems (which typically do not provide
	poll()).  Poll() provides a method for multiplexing input and output
	on multiple open file descriptors; in traditional BSD systems, that
	capability is provided by select().  While the semantics of select()
	differ from those of poll(), poll() can be readily emulated in terms
	of select() -- which is how this function is implemented.

  REFERENCES
	Stevens, W. Richard. Unix Network Programming.  Prentice-Hall, 1990.

  NOTES
	1. This software requires an ANSI C compiler.

  LICENSE

	This software is released under the following license:

		Copyright (c) 1995-2002 Brian M. Clapper
		All rights reserved.

		Redistribution and use in source and binary forms are
		permitted provided that: (1) source distributions retain
		this entire copyright notice and comment; (2) modifications
		made to the software are prominently mentioned, and a copy
		of the original software (or a pointer to its location) are
		included; and (3) distributions including binaries display
		the following acknowledgement: "This product includes
		software developed by Brian M. Clapper <bmc@clapper.org>"
		in the documentation or other materials provided with the
		distribution. The name of the author may not be used to
		endorse or promote products derived from this software
		without specific prior written permission.

		THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS
		OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE
		IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE.

	Effectively, this means you can do what you want with the software
	except remove this notice or take advantage of the author's name.
	If you modify the software and redistribute your modified version,
	you must indicate that your version is a modification of the
	original, and you must provide either a pointer to or a copy of the
	original.
\*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*\
				 Includes
\*---------------------------------------------------------------------------*/

#include <unistd.h>			     /* standard Unix definitions */
#include <sys/types.h>                       /* system types */
#include <sys/time.h>                        /* time definitions */
#include <string.h>                          /* string functions */
#include <errno.h>                           /* errno variable*/
#include "poll.h"                            /* this package */

/*---------------------------------------------------------------------------*\
				  Macros
\*---------------------------------------------------------------------------*/

#ifndef MAX
#define MAX(a,b)	((a) > (b) ? (a) : (b))
#endif


/*---------------------------------------------------------------------------*\
			     Private Functions
\*---------------------------------------------------------------------------*/

static int map_poll_spec
#if __STDC__ > 0
			(struct pollfd *pArray,
			  unsigned long  n_fds,
			  fd_set        **ppReadSet,
			  fd_set        **ppWriteSet,
			  fd_set        **ppExceptSet)
#else
			 (pArray, n_fds, pReadSet, pWriteSet, pExceptSet)
			  struct pollfd *pArray;
			  unsigned long  n_fds;
			  fd_set        **ppReadSet;
			  fd_set        **ppWriteSet;
			  fd_set        **ppExceptSet;
#endif
{
    register unsigned long  i;                   /* loop control */
    register struct	    pollfd *pCur;        /* current array element */
    register int	    max_fd = -1;         /* return value */
    fd_set *pReadSet = NULL, *pWriteSet = NULL, *pExceptSet = NULL;

    /* If pArray is bad, simply set them all to NULL and return. */
    if ( pArray == (struct pollfd *) NULL ) {
        pReadSet = pWriteSet = pExceptSet = NULL;
        return max_fd; // -1
    }
    
    /* Safely set-up the local arrays set pointers */
    if ( ppReadSet != (fd_set **) NULL )
        pReadSet = *ppReadSet;
        
    if ( ppWriteSet != (fd_set **) NULL )
        pWriteSet = *ppWriteSet;
        
    if ( ppExceptSet != (fd_set **) NULL )
        pExceptSet = *ppExceptSet;

    /*
       Map the poll() structures into the file descriptor sets required
       by select().
    */
    for (i = 0, pCur = pArray; i < n_fds; i++, pCur++)
    {
        /* Skip any bad FDs in the array. */

        if (pCur->fd < 0)
            continue;

	if ( (pCur->events & POLLIN) && (pReadSet != (fd_set *) NULL) )
	{
	    /* "Input Ready" notification desired. */
	    FD_SET (pCur->fd, pReadSet);
	}

	if ( (pCur->events & POLLOUT) && (pWriteSet != (fd_set *) NULL) )
	{
	    /* "Output Possible" notification desired. */
	    FD_SET (pCur->fd, pWriteSet);
	}

	if ( (pCur->events & POLLPRI) && (pExceptSet != (fd_set *) NULL) )
	{
	    /*
	       "Exception Occurred" notification desired.  (Exceptions
	       include out of band data.
	    */
	    FD_SET (pCur->fd, pExceptSet);
	}

	max_fd = MAX (max_fd, pCur->fd);
    }

    return max_fd;
}

static struct timeval *map_timeout
#if __STDC__ > 0
			(int poll_timeout, struct timeval *pSelTimeout)
#else
			(poll_timeout, pSelTimeout)
			 int             poll_timeout;
			 struct timeval *pSelTimeout;
#endif
{
    struct timeval *pResult;

    /*
       Map the poll() timeout value into a select() timeout.  The possible
       values of the poll() timeout value, and their meanings, are:

       VALUE	MEANING

       -1	wait indefinitely (until signal occurs)
        0	return immediately, don't block
       >0	wait specified number of milliseconds

       select() uses a "struct timeval", which specifies the timeout in
       seconds and microseconds, so the milliseconds value has to be mapped
       accordingly.
    */
    
    /* If we're given a bad timeout pointer, simply return NULL */
    /* This will cause us to wait indefinately -- which is wrong, */
    /* but we don't really have any other option. */
    if (pSelTimeout == (struct timeval *) NULL)
        return NULL;

    switch (poll_timeout)
    {
	case -1:
	    /*
	       A NULL timeout structure tells select() to wait indefinitely.
	    */
	    pResult = (struct timeval *) NULL;
	    break;

	case 0:
	    /*
	       "Return immediately" (test) is specified by all zeros in
	       a timeval structure.
	    */
	    pSelTimeout->tv_sec  = 0;
	    pSelTimeout->tv_usec = 0;
	    pResult = pSelTimeout;
	    break;

	default:
	    /* Wait the specified number of milliseconds. */
	    pSelTimeout->tv_sec  = poll_timeout / 1000; /* get seconds */
	    poll_timeout        %= 1000;                /* remove seconds */
	    pSelTimeout->tv_usec = poll_timeout * 1000; /* get microseconds */
	    pResult = pSelTimeout;
	    break;
    }


    return pResult;
}

static void map_select_results
#if __STDC__ > 0
			 (struct pollfd *pArray,
			  unsigned long  n_fds,
			  fd_set        *pReadSet,
			  fd_set        *pWriteSet,
			  fd_set        *pExceptSet)
#else
			 (pArray, n_fds, pReadSet, pWriteSet, pExceptSet)
			  struct pollfd *pArray;
			  unsigned long  n_fds;
			  fd_set        *pReadSet;
			  fd_set        *pWriteSet;
			  fd_set        *pExceptSet;
#endif
{
    register unsigned long  i;                   /* loop control */
    register struct	    pollfd *pCur;        /* current array element */

    for (i = 0, pCur = pArray; i < n_fds; i++, pCur++)
    {
        /* Skip any bad FDs in the array. */

        if (pCur->fd < 0)
            continue;

	/* Exception events take priority over input events. */

	pCur->revents = 0;
	if ( (pExceptSet != (fd_set *) NULL) && FD_ISSET (pCur->fd, pExceptSet) )
	    pCur->revents |= POLLPRI;

	else if ( (pReadSet != (fd_set *) NULL) && FD_ISSET (pCur->fd, pReadSet))
	    pCur->revents |= POLLIN;

	if ( (pWriteSet != (fd_set *) NULL) && FD_ISSET (pCur->fd, pWriteSet))
	    pCur->revents |= POLLOUT;
    }

    return;
}

/*---------------------------------------------------------------------------*\
			     Public Functions
\*---------------------------------------------------------------------------*/

int poll

#if __STDC__ > 0
	(struct pollfd *pArray, unsigned long n_fds, int timeout)
#else
	(pArray, n_fds, timeout)
	 struct	       pollfd *pArray;
	 unsigned long n_fds;
	 int	       timeout;
#endif

{
    fd_set  read_descs;                          /* input file descs */
    fd_set  write_descs;                         /* output file descs */
    fd_set  except_descs;                        /* exception descs */
    fd_set  *pRead_descs = &read_descs;           /* ptr to input file descs */
    fd_set  *pWrite_descs = &write_descs;         /* ptr to output file descs */
    fd_set  *pExcept_descs = &except_descs;       /* ptr to exception descs */
    
    struct  timeval stime;                       /* select() timeout value */
    int	    ready_descriptors;                   /* function result */
    int	    max_fd;                              /* maximum fd value */
    struct  timeval *pTimeout;                   /* actually passed */

    FD_ZERO (pRead_descs);
    FD_ZERO (pWrite_descs);
    FD_ZERO (pExcept_descs);

    /* If we've been passed an empty array, but told it's not empty, abort! */
    if ( (pArray == (struct pollfd *) NULL) && n_fds != 0 ) {
        errno = EFAULT;
        return -1;
    }
    /* Otherwise continue, and select should do the right thing. */

    /* Map the poll() file descriptor list in the select() data structures. */

    max_fd = map_poll_spec (pArray, n_fds,
			    &pRead_descs, &pWrite_descs, &pExcept_descs);

    /* Map the poll() timeout value in the select() timeout structure. */

    pTimeout = map_timeout (timeout, &stime);

    /* Make the select() call. */

    ready_descriptors = select (max_fd + 1, pRead_descs, pWrite_descs,
				pExcept_descs, pTimeout);

    if (ready_descriptors >= 0)
    {
	map_select_results (pArray, n_fds,
			    pRead_descs, pWrite_descs, pExcept_descs);
    }

    return ready_descriptors;
}
