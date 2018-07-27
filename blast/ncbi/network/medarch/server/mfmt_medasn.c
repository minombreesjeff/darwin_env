/*
*
*
* RCS Modification History:
* $Log: mfmt_medasn.c,v $
* Revision 6.0  1997/08/25 18:36:43  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:28  epstein
* add RCS log revision history
*
*/

#include "ma_intfc.h"
#include "getmedart.h"
#include "xalloc.h"
#include "medart2asn.h"

#include <objmedli.h>
#include <all.h>

#define	CTX_MFMT_MEDASN	(CTX_RESERVED+1)
#define	ASN_OUTPUT_NOT_OPEN	1

extern	MedlineEntryPtr	medline2asn(MedArt *article, AsnOptionPtr optionHead);

/* This is merely a short-hand notation to avoid having to cast this stuff
   all of the time.  It casts the handle parameter (always a void *) to
   type AsnHand before using. */

#define	asn_hand	((AsnHand *) handle)

typedef enum { uninitialized, binary, text } IoMode;
typedef	enum { not_open, open_output, init_output } OpenState;
typedef struct AsnHand {
    char	*name;			/* Name of the handler */
    char	*file;			/* Name of the output file */
    int		 have_error;		/* True => we have an error here! */
    ErrDesc	 error;			/* Error description */
    IoMode	 mode;			/* Binary or text */
    AsnIoPtr	 ioptr;			/* ASN I/O pointer */
    OpenState	 open_state;		/* How the file was initialized */
    int		 count;			/* We need to determine set stuff */
} AsnHand;

static
AsnHand	*mfmt_initialize( char *handler_name, IoMode mode )
{
    AsnHand	*handle;

    /* Allocate a handler record */

    handle = (AsnHand *) xalloc( sizeof( AsnHand ) );

    /* Initialize the handler record */

    handle->name = handler_name;
    handle->file = NULL;
    handle->have_error = FALSE;
    handle->mode = mode;
    handle->ioptr = NULL;
    handle->open_state = not_open;
    handle->count = 0;

    /* Done */

    return( handle );
}

/*ARGSUSED*/
static
void	*mfmt_init_binary( char *handler_name, void *data )
{
    /* Initialize a binary mode handle */

    return( mfmt_initialize( handler_name, binary ) );
}

/*ARGSUSED*/
static
void	*mfmt_init_text( char *handler_name, void *data )
{
    /* Initialize a text mode handle */

    return( mfmt_initialize( handler_name, text ) );
}

static
int	mfmt_open_output( void *handle, char *file_name )
{
    /* Open the file using the AsnIoOpen routine */

    asn_hand->ioptr = AsnIoOpen( file_name, (asn_hand->mode == binary) ?
	"wb" : "w" );
    if( asn_hand->ioptr == NULL ) {
	asn_hand->have_error = ErrFetch( &asn_hand->error );
    } else {
	asn_hand->have_error = FALSE;
	asn_hand->open_state = open_output;
	asn_hand->count = 0;
    }
    return( asn_hand->ioptr != NULL );
}

static
int	mfmt_init_output( void *handle, char *file_name, FILE *fd )
{
    /* Open the file using the AsnIoOpen routine */

    asn_hand->ioptr = AsnIoNew( (asn_hand->mode == binary) ?
	ASNIO_BIN_OUT : ASNIO_TEXT_OUT, fd, NULL, NULL, NULL );
    if( asn_hand->ioptr == NULL ) {
	asn_hand->have_error = ErrFetch( &asn_hand->error );
    } else {
	asn_hand->have_error = FALSE;
	asn_hand->open_state = init_output;
	asn_hand->count = 0;
    }
    return( asn_hand->ioptr != NULL );
}

static
int	mfmt_process_article( void *handle, MedArt *article )
{
    MedlineEntryPtr	asn_article;
    int			rv;

    /* Clear error status */

    asn_hand->have_error = FALSE;

    /* Is the file open? */

    if( asn_hand->open_state != open_output &&
	asn_hand->open_state != init_output ) {

	/* The file is closed - this is an error */

	ErrPost(CTX_MFMT_MEDASN, ASN_OUTPUT_NOT_OPEN,
	    "%s: ASN output to %s was not successfully opened",
	    (asn_hand->name == NULL) ? "ASN output" : asn_hand->name,
	    (asn_hand->file == NULL) ? "???" : asn_hand->file );
	return( FALSE );
    }

    /* If this is the first article - output the PubSet */

    if( asn_hand->count == 0 ) {
	DataVal	v;
	AsnWrite(asn_hand->ioptr, PUB_SET, &v);
	AsnStartStruct(asn_hand->ioptr, PUB_SET_medline);
    }

    /* Convert the article to ASN1 notation */

    asn_article = medline2asn( article, NULL );
    asn_hand->have_error = ErrFetch( &asn_hand->error );
    if( asn_hand->have_error ) return( FALSE );

    /* Write out the article */

    asn_hand->count++;
    rv = MedlineEntryAsnWrite(asn_article, asn_hand->ioptr, PUB_SET_medline_E);
    asn_hand->have_error = ErrFetch( &asn_hand->error );

    /* Handle errors */

    return( rv );
}


static
int	mfmt_close_output( void *handle )
{
    /* Irregardless of how we started it - close it */

    if( asn_hand->count > 0 )
	AsnEndStruct( asn_hand->ioptr, PUB_SET_medline);

    switch( asn_hand->open_state ) {
	case init_output:
	    AsnIoFlush( asn_hand->ioptr );
	    /* This trick is to fool AsnIoClose() */
	    asn_hand->ioptr->fp = fopen( "/dev/null", "w" );
	    /* Drop through */
	case open_output:
	    AsnIoClose( asn_hand->ioptr );
	case not_open:
	default:
	    ; /* Do nothing - it is not open */
    }

    /* Reinitialize the record */

    asn_hand->file = NULL;
    asn_hand->have_error = FALSE;
    asn_hand->ioptr = NULL;
    asn_hand->open_state = not_open;

    /* We always assume success */

    return( True );
}

char	*mfmt_get_error_string( void * handle )
{
    if( asn_hand->have_error )
	return( asn_hand->error.errtext );
    else
	return( NULL );
}


ma_Obj	mfmt_ASN1_text = {
	mfmt_init_text,
	mfmt_open_output,
	mfmt_init_output,
	mfmt_process_article,
	mfmt_close_output,
	mfmt_get_error_string,
	NULL,
};

ma_Obj	mfmt_ASN1_binary = {
	mfmt_init_binary,
	mfmt_open_output,
	mfmt_init_output,
	mfmt_process_article,
	mfmt_close_output,
	mfmt_get_error_string,
	NULL,
};

