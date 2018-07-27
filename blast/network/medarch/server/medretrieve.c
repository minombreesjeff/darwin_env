/*
 *	Program:	medretrieve.c
 *
 * RCS Modification History:
 * $Log: medretrieve.c,v $
 * Revision 6.0  1997/08/25 18:36:33  madden
 * Revision changed to 6.0
 *
 * Revision 1.10  1995/07/12 18:00:33  tatiana
 * core dump in SybaseMedlineCitMatch fixed
 *
 * Revision 1.9  1995/07/12  14:48:38  tatiana
 * first page only in the query in SybaseMedlineCitMatch()
 *
 * Revision 1.8  1995/07/05  19:20:34  tatiana
 * no default server and database.
 *
 * Revision 1.7  1995/05/30  21:57:59  tatiana
 * remove TitleMsgNew() and TitleMsgListNew()
 *
 * Revision 1.6  1995/05/30  18:01:01  tatiana
 * *** empty log message ***
 *
 * Revision 1.5  1995/05/17  17:55:12  epstein
 * add RCS log revision history
 *
*/

#include <stdlib.h>
#include <objmedli.h>
#include <objbibli.h>
#include <sys/time.h>
#include <sys/timeb.h>

#include "ma_global.h"
#include "error.h"
#include "ma_intfc.h"

#include <sybfront.h>
#include <sybdb.h>
#include <syberror.h>

#include "mappings.h"
#include <objmla.h>

/* 
 *	Define default server names, databases, user names and passwords for
 *	G5 and MEDLINE machine.
 */

#define	T_SERVER 	0
#define	T_DATABASE 	1
#define	T_USER 		2 
#define	T_PASSWORD 	3 
#define G5_SERVER	4 
#define G5_DATABASE	5 
#define G5_USER		6 
#define G5_PASSWORD	7

static DBPROCESS	*db;
static MedArt		article;

extern MedlineEntryPtr medline2asn(MedArt *article, AsnOptionPtr optionHead);
extern MedlineEntryPtr medlinePub2asn(MedArt *article, int status);
/***************************************************************************
 *
 *  function:	CopyString
 *
 ***************************************************************************/
static
CopyString( char *new, char *text )
{
   char *p, *np;
   char quote = '\'';
   int  blank_data = 1;

   for( p = text, np = new; *p; *np++ = *p++ )
	if( *p == quote ) *np++ = *p;

   *np = '\0';

   for ( np = new; *np; np++ )
	if ( *np != ' ' ) { blank_data = 0; break; }

   if ( blank_data ) *new = '\0';
}

/*************************************************************************
 *
 *  function:	ma_err_handler
 *  desc:	Sybase error handler
 *
 **************************************************************************/

int ma_err_handler( dbproc, severity, dberr, oserr, dberrstr, oserrstr )
        DBPROCESS       *dbproc;
        int             severity, dberr, oserr;
        char            *dberrstr, *oserrstr;
{

        if ( ( dbproc == NULL ) ||  DBDEAD( dbproc ) )
                return( INT_EXIT );
        else
        {
                fprintf( stderr, "DB-Library error:\n\t%s\n", dberrstr );

                if ( oserr != DBNOERR )
                   fprintf( stderr, "Operating-system error:\n\t%s\n", oserrstr );

                return( INT_CANCEL );
        }
}

/*************************************************************************
 *
 *  function:	ma_msg_handler
 *  desc:	Sybase message handler
 *
 **************************************************************************/

int ma_msg_handler( dbproc, msgno, msgstate, severity, msgtext,
                 srvname, procname, line )
        DBPROCESS       *dbproc;
        DBINT           msgno;
        int             msgstate, severity;
        char            *msgtext, *srvname, *procname;
        DBUSMALLINT     line;
{
	if ( msgno >= 5701 && msgno <= 5704 ) return( 0 );
 
        fprintf( stderr, "Msg %ld, Level %d, State %d\n", 
				msgno, severity, msgstate );

        if ( strlen( srvname ) ) fprintf( stderr, "Server '%s', ", srvname );

        if ( strlen( procname ) ) fprintf( stderr, "Procedure '%s', ", procname );

        if ( line > 0 ) fprintf( stderr, "Line %d", line );

        fprintf( stderr, "\n\t%s\n", msgtext );

        return( 0 );
} 


/*************************************************************************
 *
 *  function:	MA_InitDBConnection
 *
 **************************************************************************/

DBPROCESS *MA_InitDBConnection( char *appl, char *server, char *dbname, 
                             char *user, char *password )
{
   LOGINREC     *login;		  
   DBPROCESS	*dbproc;

   if ( dbinit() == FAIL ) exit( ERREXIT );

   dberrhandle( ma_err_handler );
   dbmsghandle( ma_msg_handler );

   if ( ( login = dblogin() ) == NULL ) return( NULL );

   DBSETLUSER( login, user );
   DBSETLPWD ( login, password );
   DBSETLAPP ( login, appl );

   if ( ( dbproc = dbopen( login, server ) ) == NULL ) return( NULL );

   dbloginfree( login );

   if ( dbuse( dbproc, dbname ) == FAIL )
	return( NULL );

   return( dbproc );
}

/**************************************************************************
 *
 *	CloseDatabaseConnection
 *
 **************************************************************************/

CloseDatabaseConnection()
{
   dbexit();
}

/*************************************************************************
 *
 *  function:	OutSybaseCommand
 *
 **************************************************************************/

static
void    OutSybaseCommand( dbproc )
        DBPROCESS       *dbproc;
{
        static FILE    *fd = NULL;
        char     buffer[61];
        int      len = dbstrlen( dbproc );
        char    *stub = "Query:";
        int      index = 0;
        int      copy_count;
        register char *p;


	if ( fd == NULL )  fd = fopen( "medserv.sql", "w" );

fprintf(fd, "length of dbproc = %d\n", len );

        /* Dump the query out to SybaseQueryPrintFile */
 
        while( index < len )
        {
            copy_count = (( len - index ) > (sizeof(buffer) - 1))
                ? (sizeof(buffer) - 1) : (len - index);
            dbstrcpy( dbproc, index, copy_count , buffer );
            for(p=buffer; *p; p++) if(isspace(*p)) *p = ' ';
            fprintf(fd, "%-12.12s || %s ||\n", stub, buffer);
            stub = "";
            index += copy_count;
        }
        fprintf(fd, "\n" );
        fflush(fd);
}

/***************************************************************************
 *
 *	Function: 	InitMedlineDB
 *
 ***************************************************************************/

InitMedlineDB()
{
/*    static char *def_server = "TECH2"; */
    static char *def_server = "MOZART_SYS10";
    static char *def_database = "molbiolit";


    if ( ( flagServer = getenv ( "SYB_MB_SERVER" ) ) == NULL ) {
/*		flagServer = def_server; */
		fprintf(stderr, "SYB_MB_SERVER is not set\n");
		return FALSE;
	}
    if ( ( flagDatabase = getenv( "SYB_MB_DATABASE" ) ) == NULL ) {
/*		flagDatabase = def_database; */
		fprintf(stderr, "SYB_MB_DATABASE is not set\n");
		return FALSE;
	}
    db = MA_InitDBConnection( "medretrieve",
	 flagServer, flagDatabase, "asnreport", "AsnReport" );

    if( db == (DBPROCESS *) NULL ) {
		return( FALSE );
    }

    return( TRUE );
}

/***************************************************************************
 *
 *	Function: 	CloseMedlineDB
 *
 ***************************************************************************/


void CloseMedlineDB()
{
    CloseDatabaseConnection();
}


/***************************************************************************
 *
 *	Function: 	MedlineArticleExists
 *
 ***************************************************************************/

static int MedlineArticleExists( Int4 ui )
{

    dbcancel( db );
    dbfcmd( db, "select ui from Article where ui = %d\n", ui );

    if( ExecSybaseCommand( db ) != SUCCEED )
        return( FALSE );

    if ( DBROWS( db ) == SUCCEED )
	return( TRUE );
    else
	return( FALSE );
}


/***************************************************************************
 *
 *	Function: 	CheckValidMedlineUids
 *	Description:	Given the number and the list of ui's,
 *			find out how many articles of interest exist in the
 *			database. 
 *
 ***************************************************************************/

int CheckValidMedlineUids( int numid, Int4Ptr uids )
{
    int entries_found = 0, i;

    for ( i = 0; i< numid; i++ )
	if ( MedlineArticleExists( uids[i] ) ) entries_found++;

    return( entries_found );
}

/***************************************************************************
 *
 *	Function: 	SybaseMedlineEntryGet
 *
 ***************************************************************************/

MedlineEntryPtr SybaseMedlineEntryGet( Int4 ui )
{
    int		flags=0;

    if ( ! MedlineArticleExists( ui ) ) 
	return( (MedlineEntryPtr)NULL );

    if ( GetMedlineArticle(db, (int)ui, flags, &article) == True )
	return( medline2asn( &article, NULL ) );
    else
	return( (MedlineEntryPtr)NULL );
}


/***************************************************************************
 *
 *	Function: 	SybaseMedlinePubGet
 *	Description:	This function retrieves data for article->cit
 *
 ***************************************************************************/

CitArtPtr SybaseMedlinePubGet( Int4 ui )
{
    int		flags=0, status;
    MedlineEntryPtr	asn_article;


    if ( ! MedlineArticleExists( ui ) ) 
	return( (CitArtPtr)NULL );

    status = GetMedlinePub(db, (int)ui, flags, &article);

    asn_article = medlinePub2asn( &article, status );

    if ( asn_article == NULL )
	return( (CitArtPtr)NULL );
    else
	return( asn_article->cit );
}

/***************************************************************************
 *
 *	Functions for	SybaseMedlineGetTitle
 *
 ***************************************************************************/

#define syb_med_code	0
#define syb_med_abbr	1
#define syb_coden	2
#define syb_issn	3
#define syb_iso_abbr	4
#define syb_name	5

#define MAX_TITLE_DATA	6
#define MAX_TITLE_ROWS	10

#define TITLE_NOT_SET	0
#define TITLE_NAME	1
#define TITLE_JTA	4
#define TITLE_ISO_JTA	5
#define TITLE_ML_JTA	6
#define TITLE_CODEN	7
#define TITLE_ISSN	8
#define TITLE_ALL	255

#define MAX_TITLE_SIZE  250

typedef struct SybTitleNode {
	char data[ MAX_TITLE_DATA ][ MAX_TITLE_SIZE ];
} SybTitle, *SybTitlePtr;

typedef struct SybTitleArray {
	int	  index;
	SybTitle  s[ MAX_TITLE_ROWS ];
} SybTitleArray;


static int which_index = 0;	/* for sorting */

/***************************************************************************
 *
 *	Function: 	TitleCompare
 *	Description:	Sort title rows by the title field specified in
 *			which_index.
 *
 ***************************************************************************/

static
int	TitleCompare( const void *t1, const void *t2 )
{
    return( strcmp( ((SybTitlePtr) t1)->data[which_index], 
		    ((SybTitlePtr) t2)->data[which_index] ) );
}

/***************************************************************************
 *
 *	Function: 	GetTitleColumnIndex
 *	Description:	Given ans.1 title_type, return index of respective
 *			title field.
 *
 ***************************************************************************/

static 
GetTitleColumnIndex( int title_type )
{
   int index = -1;

   switch( title_type )
   {
	case TITLE_NAME:	index = syb_name; 	break;
	case TITLE_JTA:		index = syb_med_abbr;	break;
	case TITLE_ISO_JTA:	index = syb_iso_abbr;	break;
	case TITLE_ML_JTA:	index = syb_med_abbr;	break;
	case TITLE_CODEN:	index = syb_coden;	break;
	case TITLE_ISSN:	index = syb_issn;	break;
	default: 		break;
   }

   return( index );
}


/***************************************************************************
 *
 *	Function: 	InsertTitleToList
 *	Description:	Insert the title field of interest into return list.
 *
 *			type: return title type ( can be all )
 *			title_type: specify what type of data returned
 *			data: returned title data
 *
 ***************************************************************************/

static 
int InsertTitleToList( TitleMsgListPtr tmlp, int type, int title_type, char *data )
{
	int len;
 	TitleMsgPtr t;

	if ( data == NULL ) return( 0 );

	if ( strlen( data ) == 0 ) return( 0 );

	t = (TitleMsgPtr) MemNew(sizeof(TitleMsg));
	t->type = type;	
	t->next = tmlp->titles;		/* insert at head of the list */

	t->title = ( TitlePtr ) ValNodeNew(NULL);
	t->title->choice = title_type;
	t->title->data.ptrvalue = StringSave( data );

	tmlp->titles = t;

	++( tmlp->num );
}


/***************************************************************************
 *
 *	Function: 	BuildTitleMsgList
 *	Description:	Insert the non-null title field(s) specified by
 *			title_type into the returned list; return number
 *			of nodes in the list.  Duplicate data will be 
 *			eliminated.
 *
 ***************************************************************************/

static
BuildTitleMsgList( TitleMsgListPtr tmlp, int title_type, SybTitleArray titles )
{
   int  i, j, index;
   char prev_data[ MAX_TITLE_SIZE ];

   if ( ( index = GetTitleColumnIndex( title_type ) ) < 0 )
	return( 0 );

   if ( titles.index > 1 ) { /* sorting */
	which_index = index;
	qsort( titles.s, titles.index, sizeof(SybTitle), TitleCompare );
   }

   prev_data[ 0 ] = '\0';

   for ( i=titles.index-1; i >= 0; i-- )
   {
	if ( strcmp( titles.s[i].data[index], prev_data ) == 0 )
	     continue;

	InsertTitleToList( tmlp, title_type, title_type, titles.s[i].data[index] );

	strcpy( prev_data, titles.s[i].data[index] );
   }

   return( tmlp->num );
}

/***************************************************************************
 *
 *	Function: 	MoreTitleRows
 *	Description:	Check if only one unique title row retrieved from 
 *			Sybase.
 *
 ***************************************************************************/

MoreTitleRows( SybTitleArray titles )
{
   int i, j;

   for ( i = 1; i < titles.index; i++ )
	for ( j = syb_med_abbr; j <= syb_name; j++ ) {
	   if ( strcmp( titles.s[0].data[j], titles.s[i].data[j] ) )
		return( 1 );
	}

   return( 0 );
}

/***************************************************************************
 *
 *	Function: 	BuildAllTitleMsgList
 *	Description:
 *	
 *	   For title_type = all,
 *		If only one unique title row retrieved from Sybase,
 *			return all title fields 
 *		else
 *			return ml-jta( med_abbr ) from each title row.
 *
 ***************************************************************************/

static
BuildAllTitleMsgList( TitleMsgListPtr tmlp, SybTitleArray titles )
{
   int i;
   static int title_type_list[6] = 
	{ 0, TITLE_ML_JTA, TITLE_CODEN, TITLE_ISSN, TITLE_ISO_JTA, TITLE_NAME };

   if ( MoreTitleRows( titles ) )
	return( 0 );

   for ( i = syb_med_abbr; i <= syb_name; i++ ) {
	InsertTitleToList(tmlp, TITLE_ALL, title_type_list[i], titles.s[0].data[i]);
   }

   return( 1 );
}


/***************************************************************************
 *
 *	Function: 	RetrieveTitleInfoFromSybase
 *	Description:	The stored proc mr_get_title returns status, and
 *			title information if status > 0
 *
 ***************************************************************************/

RetrieveTitleInfoFromSybase( TitleMsgPtr tmsg )
{
    if ( tmsg == NULL || tmsg->title->data.ptrvalue == NULL )
	return( NULL );

    dbfreebuf( db );
    dbfcmd( db, "exec mr_get_title %d,  %d, '%s'\n", tmsg->type,
		tmsg->title->choice, tmsg->title->data.ptrvalue );

OutSybaseCommand( db );

    if ( ExecSybaseCommand( db ) != SUCCEED )
	return( NULL );

    return( SUCCEED );
}


/***************************************************************************
 *
 *	Function: 	SybaseMedlineGetTitle
 *	Description:	
 *		1. retrieve title data from Sybase
 *		2. check status, if < 0, return NULL ( indicate error )
 *				 if = 0, return not found
 *		3. store all retrieved data into temporary array
 *		4. if request = all
 *			if rows returned = 1, 
 *			   return type=all, and every title column
 *			else
 *			   return type=ml-jta, and med_abbr for each row
 *		5. if request = not-set
 *			return type=ml-jta
 *		6. otherwise,
 *			if request != ml-jta
 *			   if column exist
 *				return column
 *			   else
 *				return ml-jta
 *
 ***************************************************************************/

TitleMsgListPtr SybaseMedlineGetTitle( TitleMsgPtr tmsg )
{
    int 	    status, i;
    TitleMsgListPtr tmlp = NULL;
    SybTitleArray   titles;
    SybTitle	    title_value;

    if ( RetrieveTitleInfoFromSybase( tmsg ) == NULL )
	return( NULL );

    dbbind( db, 1, INTBIND, 0, &status );
    dbnextrow( db );

    if ( status < 0 ) return( NULL );

    tmlp = (TitleMsgListPtr) MemNew(sizeof(TitleMsgList));
    tmlp-> num = 0;
    tmlp->titles = NULL;

    titles.index = 0;

    if ( status > 0 && dbresults( db ) == SUCCEED )
    {
	for ( i = syb_med_code; i <= syb_name; i++ )
	    dbbind( db, i+1, NTBSTRINGBIND, 0, title_value.data[i] );

	while( dbnextrow( db ) != NO_MORE_ROWS )
	{
	    for ( i = syb_med_code; i <= syb_name; i++ )
		strcpy( titles.s[ titles.index ].data[i], title_value.data[i] );
	    ++titles.index;
    	}

	if ( titles.index )
	{
    	  switch( tmsg->type )
          {
	    case TITLE_NOT_SET:
	    case TITLE_ML_JTA:
		BuildTitleMsgList( tmlp, TITLE_ML_JTA, titles );
		break;

	    case TITLE_ALL:
		if ( ! BuildAllTitleMsgList( tmlp, titles ) )
		    BuildTitleMsgList( tmlp, TITLE_ML_JTA, titles );
		break;

	    default:
	 	if ( ! BuildTitleMsgList( tmlp, tmsg->type, titles ) )
		    BuildTitleMsgList( tmlp, TITLE_ML_JTA, titles );
		break;
    	  }
	}
    }

    if ( tmlp->titles == NULL )
    {
	tmlp->num = 1;
	tmlp->titles = tmsg;
	tmlp->titles->type = 0;	/* not-set: means not-found */
	tmlp->titles->next = NULL;
	tmsg->next = NULL;
    }

    return( tmlp );
}

/***************************************************************************
 *
 *	Functions for 	SybaseMedlineCitMatch
 *
 ***************************************************************************/

#define FROM_JOURNAL		1
#define RET_ERROR		0

int CheckCitNullVal( char *s )
{
    char ts[255];

    if ( s == NULL || s[0] == '\0' )
	dbcmd( db, ", NULL " );
    else
    {
	CopyString( ts, s );
	dbfcmd( db, ", '%s' ", ts );
    }
}

/***************************************************************************
 *
 *	Function: 	SybaseMedlineCitMatch
 *	Description:	Call get_muid2 stored procedure.
 *			Return only the first ui found.  
 *
 ***************************************************************************/

int SybaseMedlineCitMatch( CitArtPtr citation )
{
    CitJourPtr  cit_jou;
    AuthListPtr authors;
	AuthorPtr auth;
	static char authname[122], page[122];
	CharPtr  p;
    int		ui, method, ret_ui=0;
    CharPtr	first_author = NULL;
    CharPtr	first_page = NULL;
    RETCODE	ret;


    if ( ! citation || citation->from != FROM_JOURNAL ) 
	return( RET_ERROR );

    if ( ! ( cit_jou = ( CitJourPtr )citation->fromptr ) ) 
	return( RET_ERROR );

    if ( ! cit_jou->title || ! cit_jou->imp ) 
	return( RET_ERROR );

    if ( ( authors = ( AuthListPtr )citation->authors ) != NULL &&
	   authors->names != NULL ) {
	   if (authors->choice == 1) {
	   		auth = authors->names->data.ptrvalue;
			PersonIdLabel(auth->name, authname, 121, PIDLABEL_GENBANK);
			first_author = authname;
	   } else {
			first_author = authors->names->data.ptrvalue; 
	   }
	 }
	if (cit_jou->imp->pages != NULL) {
		StringCpy(page, cit_jou->imp->pages);
		for (first_page = p = page; *p != '\0' && *p != '-'; p++);
		if (*p == '-') {
			*p = '\0';
		}
	}
    dbfreebuf( db );
    dbfcmd( db, "exec get_muid2 %d, '%s' ", 
		cit_jou->title->choice, cit_jou->title->data.ptrvalue );
    CheckCitNullVal( cit_jou->imp->volume ),
    CheckCitNullVal( first_page ),
    dbfcmd( db,	 ", %d ", cit_jou->imp->date->data[1]+1900 );
    CheckCitNullVal( first_author );
    dbcmd ( db, ", 1\n" );

OutSybaseCommand( db );

    ret = dbsqlexec( db );

    if ( ret != SUCCEED || dbresults( db ) != SUCCEED )
	return( RET_ERROR );

    dbbind( db, 1, INTBIND, 0, &ui );
    dbbind( db, 2, INTBIND, 0, &method );

    while( dbnextrow( db ) != NO_MORE_ROWS )
    {
	if ( ret_ui ) continue;

	ret_ui = ( ( ui >= 0 ) ? ui : 0 );
    }

    return( ret_ui );
}
