#include "ma_intfc.h"
#include "ma_global.h"
#include "error.h"

/*
*		ma_OpenOutputStreams()
*
*
* RCS Modification History:
* $Log: outmedart.c,v $
* Revision 6.0  1997/08/25 18:36:47  madden
* Revision changed to 6.0
*
* Revision 1.3  1995/05/17 17:55:38  epstein
* add RCS log revision history
*
*/
int	 ma_OpenOutputStreams()
{
    int		os;
    int		rc = TRUE;

    /* Loop over the input selection list from the command line, opening
	all of the desired channels.  Each one can be opened once. */

    for( os = 0; os < outputStreamCount; os++ ) {
	int	rv;

	/* Lookup the output handler in the handler table */

	outputStreams[os].handle = ma_ValidHandler( outputStreams[os].format );
	if( outputStreams[os].handle == (void *) NULL ) {
	    fprintf(stderr, "%s: Unable to open handler for %s format.\n",
		programName, outputStreams[os].format);
	    return( FALSE );
	}

	/* If we found one, determine if we're opening it or simply
	   passing standard output to it.  Then open the handler.  */

	if( strcmp( outputStreams[os].file, "-" ) == 0 )
	    rv = ma_OpenHandler( outputStreams[os].handle,
		"standard output", stdout );
	else
	    rv = ma_OpenHandler( outputStreams[os].handle,
		outputStreams[os].file, NULL );
	if( ! rv ) {
	    fprintf( stderr, "%s\n",
		ma_GetErrorString(outputStreams[os].handle) );
	    return( FALSE );
	}
	rc &= rv;
    }

    /* All handlers open */

    return( rc );
}


/*
*		ma_CloseOutputStreams()
*/
void	 ma_CloseOutputStreams()
{
    int		os;

    /* Loop over the handlers - closing each open one in turn */

    for( os = 0; os < outputStreamCount; os++ )
	if( outputStreams[os].handle != NULL )
	    (void) ma_CloseHandler( outputStreams[os].handle );
}


/*
*		ma_WriteOutput(MedArtarticle)
*/
int	 ma_WriteOutput( MedArt *article )
{
    int		os;
    int		rc = TRUE;

    /* Send output to each handler in turn */

    for( os = 0; os < outputStreamCount; os++ ) {
	int	rv;

	/* Ignore closed handles */

	if( outputStreams[os].handle == NULL ) continue;

	/* Write output to a handle */

	rv = ma_WriteArticle( outputStreams[os].handle, article );
	if( ! rv ) {
	    fprintf( stderr, "%s: %s writing UI\n", programName,
		ma_GetErrorString(outputStreams[os].handle) );
	}

	/* Make a summary status */

	rc &= rv;
    }

    /* Return the result */

    return( rc );
}

