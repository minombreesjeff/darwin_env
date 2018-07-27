#ifndef	DEFS_MODULE_MFMT_SIMPLE_H
#define	DEFS_MODULE_MFMT_SIMPLE_H

#include <stdio.h>
#include "ma_intfc.h"

typedef	struct {
    char	*name;
    char	*file;
    FILE	*fd;
    char	*error;
    void	*data;
    int		used_open;
} SimpleOut;

/*
  NOTE:	There is no mfmt_simple_process() procedure - you'll have to
	write that itself.  However, these procedures will handle most
	everything else, except for complicated output formats which
	require special open and close code.
*
*
* RCS Modification History:
* $Log: mfmt_simple.h,v $
* Revision 6.0  1997/08/25 18:36:45  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:31  epstein
* add RCS log revision history
*
*/

void	*mfmt_simple_init( char *handler_name, void *data );
int	 mfmt_simple_open( void *handle, char *file_name );
int	 mfmt_simple_setup( void *handle, char *file_name, FILE *fd );
int	 mfmt_simple_close( void *handle );
char	*mfmt_simple_get_error( void *handle );

void	 mfmt_simple_error( void *handle, char *message );
void	 mfmt_simple_syserror( void *handle );

#endif /* DEFS_MODULE_MFMT_SIMPLE_H */
