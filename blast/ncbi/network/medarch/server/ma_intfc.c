/*
*
*
* RCS Modification History:
* $Log: ma_intfc.c,v $
* Revision 6.0  1997/08/25 18:36:18  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:54:55  epstein
* add RCS log revision history
*
*/

#include <stdlib.h>
#include "xalloc.h"
#include "ma_intfc.h"

extern	ma_Obj	mfmt_ASN1_text, mfmt_ASN1_binary;

typedef	struct	ma_Hand {
	char	*cmd_flag;
	char	*handler_name;
	char	*load_path;
	char	*loadob_sym;
	ma_Obj	*object;
	void	*handle;
	void	*data;
	int	 unusable;
} ma_Hand;

ma_Hand	default_handler_table[] = {

   {	/* Handle the ASN1 text interface */
	"asn1",
	"ASN1 Text",
	NULL,
	"_mfmt_ASN1_text",
	&mfmt_ASN1_text,
	(void *) NULL,
	(void *) NULL,
	FALSE
    },

   {	/* Handle the ASN1 binary interface */
	"asn1b",
	"ASN1 Binary",
	NULL,
	"_mfmt_ASN1_binary",
	&mfmt_ASN1_binary,
	(void *) NULL,
	(void *) NULL,
	FALSE
    }
};
static	int	n_default_handlers =
			sizeof(default_handler_table)/sizeof(ma_Hand);


static
ma_Hand	handler_table[ MAX_FORMAT_HANDLERS ];
static	int	n_handlers = 0;


static
char	*strdup( char *s ) {
	char	*r;

	if( s == NULL ) return( NULL );

	r = xalloc( strlen(s) + 1 );
	strcpy(r, s );
	return( r );
}

static
int	install_format( char *file_name, char *cmd_flag, char *load_path,
			ma_Obj *object, char *load_symbol, char *handler_name )
{
    char	*config_file;

    /* Check the arguments */

    config_file = (file_name == NULL) ? "built-in's" : file_name;
    if( cmd_flag != NULL && handler_name == NULL ) handler_name = cmd_flag;
    if( handler_name == NULL ) handler_name = "<<Unknown handler>>";
    if( cmd_flag == NULL ) {
	fprintf(stderr, "%s: No format name flag specified for %s handler\n",
		config_file, handler_name );
	return( FALSE );
    }
    if( load_path == NULL && file_name != NULL ) {
	fprintf(stderr, "%s: No load path given for %s handler\n",
		config_file, handler_name );
	return( FALSE );
    }
    if( load_symbol == NULL ) {
	fprintf(stderr, "%s: No load symbol given for %s handler\n",
		config_file, handler_name );
	return( FALSE );
    }

    /* Install the entry */

    if( n_handlers >= MAX_FORMAT_HANDLERS ) {
	fprintf(stderr, "%s: Too many handlers at handler %s (%d MAX)\n",
		config_file, handler_name, MAX_FORMAT_HANDLERS );
	return( FALSE );
    }
    handler_table[n_handlers].cmd_flag = cmd_flag;
    handler_table[n_handlers].handler_name = handler_name;
    handler_table[n_handlers].load_path = load_path;
    handler_table[n_handlers].object = object;
    handler_table[n_handlers].loadob_sym = load_symbol;
    n_handlers++;

    return( TRUE );
}

typedef enum { begin_scan, in_token, quoted_char, quoted_string, end_scan }
	State;

static
char	*get_token( char *s, char **next )
{
    static char	 token[MAX_TOKEN_SIZE];
    char	*t = &token[0];
    State 	 state, old_state;
    char	 mark;

    /* Scan out the token */

    if( s != NULL ) {
	for( state = begin_scan; state != end_scan; ) {

	    /* White space is all equivalent */

	    if( isspace( *s ) || *s == '\r' || *s == '\n' )
		*s = ' ';

	    switch( state ) {
	    case begin_scan:
		switch( *s ) {
		    case '"':
		    case '\'':			/* Quoted strings */
			mark = *s;
			state = quoted_string;
			break;
		    case 0:			/* End of string */
			state = end_scan;
			/* DROP THROUGH */
		    case ' ':			/* Spaces - ignore */
			break;
		    default:			/* Standard token */
			state = in_token;
			continue;		/* We'll retest this char */
		}
		break;
	    case in_token:			/* Standard token */
		switch( *s ) {
		    case ' ':	/* Space - terminate the token */
		    case 0:			/* End of input */
			 state = end_scan;
			 break;
		    case '\\':			/* Quoted character? */
			old_state = state;
			state = quoted_char;
			break;
		    default:
			*t++ = *s;
			break;		/* Continue collecting */		
		}
		break;
	    case quoted_string:		/* String inside quotes */
		if( *s == 0 )
		    state == end_scan;
		else if( *s == mark ) {
		    state = old_state;
		    break;
		} else if( *s == '\\' ) {
		    old_state = state;
		    state = quoted_char;
		} else {
		    *t++ = *s;
		}
		break;
	    case quoted_char:		/* Quoted char (\x) */
		if( *s == 0 )
		    state = end_scan;
		else {
		    state = old_state;
		    *t++ = *s;
		}
		break;
	    }
	    if( *s != (char) 0 ) s++;
	}
    }
    *t++ = (char) 0;			/* Terminate token */

    /* Figure out where the next token is */

    if( next != (char **) NULL ) {
	*next = ( s == NULL || *s == (char) 0 ) ? NULL : s;
    }

    /* Done */

    return( (*token == (char) 0) ? NULL : token );
}


static
int	get_handlers( char *file_name )
{
    FILE	*fd;
    char	 buffer[MAX_FORMAT_LINE];
    int		 rv = TRUE;
    char	*token;

    /* Open the configuration file */

    fd = fopen( file_name, "r" );
    if( fd == NULL ) {
	perror( file_name );
	return( FALSE );
    }

    /* Read the file and extract the handler lines */

    while( fgets( buffer, sizeof(buffer), fd ) != NULL ) {
	char	*next;
	char	*label = get_token( buffer, &next );
	if( label == NULL || *label == '#' )
		continue;		/* Ignore me type lines */
	if( strcmp( label, "format" ) == 0 ) {
	    char	*cmd_flag, *load_path, *load_symbol, *handler_name;

	    cmd_flag = strdup( get_token( next, &next ) );
	    load_path = strdup( get_token( next, &next ) );
	    load_symbol = strdup( get_token( next, &next ) );
	    handler_name = strdup( get_token( next, &next ) );
	    rv &= install_format( file_name, cmd_flag, load_path,
		(ma_Obj *) NULL, load_symbol, handler_name);

	} else if( strcmp( label, "include" ) == 0 ) {
	    while( (token = get_token( next, &next )) != NULL ) {
		char	file_name[MAX_TOKEN_SIZE];

		strcpy( file_name, token );
		rv &= get_handlers( file_name );
	    }
	}
    }

    /* Return the status */

    return( rv );
}


/*ARGSUSED*/
int	ma_GetHandlers( char *file_name )
{
#ifdef	DEBUG
	/* DEBUG VERSION - Uses hard coded table */

	n_handlers = sizeof(handler_table)/sizeof(ma_Hand);
	return( TRUE );
#else
    int	rv = TRUE;		/* Assume everything OK */
    int i;

    /* Open the configuration file and read the data */

    if( file_name != NULL ) rv = get_handlers( file_name );

    /* Install the default handlers */

    for( i = 0; i < n_default_handlers; i++ ) {
	ma_Hand *h = &default_handler_table[i];
	rv &= install_format( NULL, h->cmd_flag, h->load_path,
		h->object, h->loadob_sym, h->handler_name );
    }
#endif
}


void	*ma_ValidHandler( char *cmd_flag )
{
    register int i;

    /* Search the handler table for a key.  Only one file can be open
       on a channel, so we'll look only at closed channels */

    for( i = 0; i < n_handlers; i++ )
	if( handler_table[i].data == NULL &&
	    	strcmp( handler_table[i].cmd_flag, cmd_flag ) == 0 )
	    return( &handler_table[i] );

    /* If we didn't find one - we didn't have a slot for it. */

    return( NULL );
}


int	ma_OpenHandler( void *x_handler, char *file_name, FILE *fd )
{
    ma_Hand	*handler = x_handler;
    int		 rv;

    /* Open the library */

    if( handler->load_path != NULL ) {
	handler->handle = dlopen( handler->load_path, RTLD_LAZY );
	if( handler->handle == NULL ) goto err;

	/* Get the object from the module */

	handler->object = dlsym( handler->handle, handler->loadob_sym );
    }
    if( handler->object == NULL )
	goto err;

    /* Initialize the channel */

    handler->data = handler->object->initialize( handler->handler_name,
	handler->object->data );
    if( handler->data == NULL )
	goto err;

    /* If the file pointer is NULL - we'll open it; otherwise, we assume
	that the file is already open (usually stdout) */

    if( fd != NULL)
	rv = handler->object->setup_file( handler->data, file_name, fd );
    else
	rv = handler->object->open_file( handler->data, file_name );

    /* Successful exit */

    if( rv ) return( True );

    /* Come here on an error */
err:
	handler->unusable = TRUE;
	return( FALSE );
}


int	ma_WriteArticle( void *x_handler, MedArt *article )
{
    ma_Hand	*handler = x_handler;
    /* If the handler is usable - use it */

    if( handler->unusable )
	return( FALSE );

    return( handler->object->process( handler->data, article ) );
}


int	ma_CloseHandler( void *x_handler )
{
    ma_Hand	*handler = x_handler;

    /* We don't have to do anything if it was never opened */

    if( handler->object != NULL && handler->data != NULL ) {
	(void) handler->object->close( handler->data );
	handler->data = NULL;
    }

    /* Close the object module */

    if( handler->handle != NULL ) {
	dlclose( handler->handle );
	handler->handle = NULL;
	handler->object = NULL;
    }

    /* Finished */

    return( TRUE );
}


char	*ma_GetErrorString( void *x_handler )
{
    ma_Hand	*handler = x_handler;

    /* Make sure that the handler is open */

    if( handler->handle == NULL || handler->object == NULL ||
	handler->data == NULL )
    {
	static char	message[512];
	sprintf( message, "MEDLINE %s output handler is not open",
		handler->handler_name );
	return( message );
    }

    /* We have the handler - get the error string */

    return( handler->object->get_error( handler->data ) );
}
