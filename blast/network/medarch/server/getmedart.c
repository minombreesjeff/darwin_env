#include "getmedart.h"

#include <sys/types.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <string.h>

#define bzero(b,length) memset(b,0,length)
#define bcopy(b1,b2,length) memmove(b2,b1,length)

/* NOTE: The mr_get_article script will return sections is numerical order! */

#define	MR_STATE_ARTICLE	0		/* Basic article data */
#define	MR_STATE_ISO_VTITLE	1
#define	MR_STATE_ISO_ADDRESS	2
#define	MR_STATE_AUTHOR		3
#define	MR_STATE_ISO_AUTHOR	4
#define	MR_STATE_ISO_ABSTRACT	5		/* We send ISO first for the */
#define	MR_STATE_ABSTRACT	6		/* abstract only */
#define	MR_STATE_MESH		7
#define	MR_STATE_SUBHEADING	8
#define	MR_STATE_SUBSTANCE	9
#define	MR_STATE_CROSSREFERENCE	10
#define	MR_STATE_SUPPORT	11
#define	MR_STATE_GENESYMBOL	12
#define	MR_STATE_DONE		255

#define	PATCH_BUFFER_SIZE	8192
#define	TEXT_BUFFER_SIZE	8192
#define	MAX_ABSTRACT_PATCHES	64

extern FILE * Id_timing_file;

#ifdef SYSV
hrtime_t Time_start, Time_back;
#define MACRO_before_exec \
	if (Id_timing_file) \
	Time_start = gethrtime();

#define MACRO_after_exec(type) \
	if (Id_timing_file) {\
	Time_back = gethrtime();\
	fprintf(Id_timing_file,"%s %ld %ld\n", #type, (long) (Time_start/1000000.), (long) (( Time_back- Time_start)/ 1000000. )); fflush(Id_timing_file);}
#else
struct timeb Time_start, Time_back;
#define MACRO_before_exec \
	if (Id_timing_file) \
	ftime(&Time_start);

#define MACRO_after_exec(type) \
	if (Id_timing_file) {\
	ftime (&Time_back);\
	fprintf(Id_timing_file,"%s %ld %ld\n", #type, (long) Time_start.time, (long) (( Time_back.time - Time_start.time)* 1000 + (Time_back.millitm - Time_start.millitm))); fflush(Id_timing_file);}
#endif	

typedef	struct AbPatch {
	DBINT		 line_no;
	DBINT		 length;
	DBBINARY	*start;
} AbPatch;

typedef	struct	AbPList {
	int		n_patches;
	AbPatch		patches[MAX_ABSTRACT_PATCHES];
	int		used;
	DBBINARY	buffer[PATCH_BUFFER_SIZE];
} AbPList;


char *strstr();
static
/*
*		skip_undefined_section(DBPROCESSdb)
* RCS Modification History:
* $Log: getmedart.c,v $
* Revision 6.0  1997/08/25 18:36:12  madden
* Revision changed to 6.0
*
* Revision 1.3  1995/05/30 18:00:10  tatiana
* cleanup title
*
* Revision 1.2  1995/05/17  17:54:43  epstein
* add RCS log revision history
*
*/
int	skip_undefined_section( DBPROCESS *db )
{
    RETCODE	rc;

    /* This simply skips to the end of the data */

    while( (rc = dbnextrow( db )) != NO_MORE_ROWS && rc != FAIL );
    return( (rc == FAIL) ? Error : True );
}


static
/*
*		title_append(DbTitle(title),charrest)
*/
void	title_append( DbTitle(title), char *rest )
{
    int	len;

    if( *rest == 0 ) return;
    for( len = strlen( title ); len < DBSIZE_Article_title; len++ )
	title[len] = ' ';
    strcpy( &title[len], rest );
}

static void check_title(char *title)
{
	char *s;
	
	if ((s = strstr(title, "[see comments]")) != NULL) {
			s--;
			while( s >= title && (*s == ' ') ) {
				*s = '\0';
				s--;
			}
	}
}

void check_pages(char *pages)
{
	char	*s;
	
	for (; isspace(*pages); pages++);
	if ((s = strchr(pages, ' ')) != NULL) {
		*s = '\0';
	}
}

static
/*
*		process_article(DBPROCESSdb,MedArtrec)
*/
int	process_article( DBPROCESS *db, MedArt *rec )
{
    RETCODE		rc;
    DbColumn	(Article,title1);
    DbColumn	(Article,vtitle1);
    char		*chr;

    /* Load the article row from the database */
	/* TODO - CHECK THE FIELDS IN THE BIND! */
    rc = SybaseLoad( db,
	    1,	INTBIND,	sizeof(rec->ui),	&rec->ui,
	    2,	INTBIND,	sizeof(rec->mri),	&rec->mri,
	    3,	NTBSTRINGBIND,	sizeof(rec->title),	rec->title,
	    4,	NTBSTRINGBIND,	sizeof(title1),		title1,
	    5,	NTBSTRINGBIND,	sizeof(rec->vtitle),	rec->vtitle,
 	    6,	NTBSTRINGBIND,	sizeof(vtitle1),	vtitle1,
 	    7,	NTBSTRINGBIND,	sizeof(rec->address),	rec->address,
 	    8,	NTBSTRINGBIND,	sizeof(rec->pages),	rec->pages,
 	    9,	SMALLBIND,	0,			&rec->entry_month,
 	    10,	NTBSTRINGBIND,	sizeof(rec->volume),	rec->volume,
 	    11,	NTBSTRINGBIND,	sizeof(rec->issue),	rec->issue,
	    12,	NTBSTRINGBIND,	sizeof(rec->pubdate),	rec->pubdate,
	    13,	NTBSTRINGBIND,	sizeof(rec->med_abbr),	rec->med_abbr,
	    14,	NTBSTRINGBIND,	sizeof(rec->language_code), rec->language_code,
 	    0 );
    if( rc != REG_ROW ) return( False );

    /* Add the extension columns to the title and vtitle fields */

    rec->title_type = 6; /* ml-jta */
    title_append( rec->title,  title1  );
    title_append( rec->vtitle, vtitle1 );
    
    check_title(rec->title);
    check_title(rec->vtitle);
    
	if (strncmp(rec->pages, "Suppl", 5) == 0) {
		if ((chr = strchr(rec->pages, ':')) != NULL) {
			*chr = '\0';
			if (rec->issue[0] != '\0') {
				strncat(rec->issue, rec->pages, 
						DBSIZE_Issue_issue - strlen(rec->issue));
			} else {
				strncat(rec->volume, rec->pages, 
						DBSIZE_Issue_volume - strlen(rec->volume));
			}
			strcpy(rec->pages, chr + 1);
		}
	}
	check_pages(rec->pages);

    /* Done */

    return( True );
}


static
/*
*		process_pub_article(DBPROCESSdb,MedArtrec)
*/
int	process_pub_article( DBPROCESS *db, MedArt *rec )
{
    RETCODE		rc;
    DbColumn	(Article,title1);
    DbColumn	(Article,vtitle1);
    char		*chr;

    /* Load the article row from the database */
	/* TODO - CHECK THE FIELDS IN THE BIND! */
    rc = SybaseLoad( db,
	    1,	INTBIND,	sizeof(rec->ui),	&rec->ui,
	    2,	INTBIND,	sizeof(rec->mri),	&rec->mri,
	    3,	NTBSTRINGBIND,	sizeof(rec->title),	rec->title,
	    4,	NTBSTRINGBIND,	sizeof(title1),		title1,
	    5,	NTBSTRINGBIND,	sizeof(rec->vtitle),	rec->vtitle,
 	    6,	NTBSTRINGBIND,	sizeof(vtitle1),	vtitle1,
 	    7,	NTBSTRINGBIND,	sizeof(rec->address),	rec->address,
 	    8,	NTBSTRINGBIND,	sizeof(rec->pages),	rec->pages,
 	    9,	SMALLBIND,	0,			&rec->entry_month,
 	    10,	NTBSTRINGBIND,	sizeof(rec->volume),	rec->volume,
 	    11,	NTBSTRINGBIND,	sizeof(rec->issue),	rec->issue,
	    12,	NTBSTRINGBIND,	sizeof(rec->pubdate),	rec->pubdate,
	    13,	NTBSTRINGBIND,	sizeof(rec->med_abbr),	rec->med_abbr,
	    14,	NTBSTRINGBIND,	sizeof(rec->language_code), rec->language_code,
	    15,	INTBIND,	sizeof(rec->title_type),&rec->title_type,
 	    0 );
    if( rc != REG_ROW ) return( False );

    /* Add the extension columns to the title and vtitle fields */

    title_append( rec->title,  title1  );
    title_append( rec->vtitle, vtitle1 );

	check_title(rec->title);
	check_title(rec->vtitle);

	if (strncmp(rec->pages, "Suppl", 5) == 0) {
		if ((chr = strchr(rec->pages, ':')) != NULL) {
			*chr = '\0';
			if (rec->issue[0] != '\0') {
				strncat(rec->issue, rec->pages, 
						DBSIZE_Issue_issue - strlen(rec->issue));
			} else {
				strncat(rec->volume, rec->pages, 
						DBSIZE_Issue_volume - strlen(rec->volume));
			}
			strcpy(rec->pages, chr + 1);
		}
	}
	check_pages(rec->pages);

    /* Done */

    return( True );
}


static	DBINT		patch_sequence = 0;
static	DBINT		patch_item = 0;
static	DBINT		patch_length;
static	MaxBinary	patch_line;

static
/*
*		load_patch(DBPROCESSdb,DBBINARYpatches,intsize,
*/
int	load_patch( DBPROCESS *db, DBBINARY *patches, int size,
		DBINT	*item, DBINT *length, int first )
{
    RETCODE	rc;
    DBINT	sequence;

    /* If we have buffered a line - get it */

    if( ! first && patch_sequence == 1 ) {
	*item = patch_item;
	bcopy( patch_line, patches, patch_length );
	*length = patch_length;
	sequence = 1;
    } else {
	*length = patch_length = patch_item = sequence = 0;
    }

    while ( (rc = SybaseLoad( db,
	1,	INTBIND,	0,			&patch_item,
	2,	INTBIND,	0,			&patch_sequence,
	3,	INTBIND,	0,			&patch_length,
	4,	BINARYBIND,	sizeof(patch_line),	 patch_line,
	0)) == REG_ROW )
    {
	/* If this is the first line - init the setup */

	if( sequence == 0 ) {
	    if( patch_sequence != 1 ) continue;		/* Skip junk */
	    *item = patch_item;
	}

	/* If we get this far, test for a continuation line */

	if( patch_sequence == (sequence + 1) && patch_item == *item ) {
	    if( ( patch_length + *length ) > size )
		goto error;			/* Overflow */
	    bcopy( patch_line, patches + *length, patch_length );
	    *length += patch_length;
	    continue;
	} else if( patch_sequence != 1 )
	    goto error;				/* Patch sequence error */

	/* If we get here - we're at the start of the next patch */

	break;
    }
    if( rc == FAIL ) goto error;
    if( rc != REG_ROW ) patch_sequence = 0;	/* Reset on end */

    /* Normal exit */

    return( (*length == 0) ? False : True );

    /* Error exit */

error:
    patch_sequence = 0;
    return( Error );
}


/*ARGSUSED*/
static
/*
*		patch_string(DBPROCESSdb,charstr,intsize)
*/
int	patch_string( DBPROCESS *db, char *str, int size )
{
    DBBINARY	patches[PATCH_BUFFER_SIZE];
    DBINT	patch_length = 0, item = 0;
    DBCHAR	buffer[TEXT_BUFFER_SIZE];
    int		rv;
    char	*ptr;

    /* Get the title patches from the input string */

    if( ! DBROWS( db ) ) return( False );
    rv = load_patch( db, patches, sizeof(patches), &item, &patch_length, True );
    if( rv != True ) return( rv );

    /* We have the patches, apply them */

    if( ApplyIsoPatches( str, size, buffer, sizeof(buffer),
	patches, patch_length ) > 0 )
    {
	return( Error );
    }

  /* Done - put the stuff back */
   strcpy( str, buffer );
    return( True );
}


static
/*
*		process_author(DBPROCESSdb,MedArtrec)
*/
int	process_author( DBPROCESS *db, MedArt *rec )
{
    DbColumn(Name,name);
    RETCODE	rc;
    DBINT	seq;

    /* Read the author list */

    while( (rc = SybaseLoad( db,
		1,	INTBIND,	0,		&seq,
		2,	NTBSTRINGBIND,	sizeof(name),	name,
		0 )) == REG_ROW )
    {
	if( seq > MAX_AUTHOR_COUNT )
	    continue;			/* Author index too large */
	strcpy( rec->authors[seq], name );
	if( (seq+1) > rec->n_authors ) rec->n_authors = seq+1;
    }

    return((rc == NO_MORE_ROWS) ? ((rec->n_authors == 0) ? False: True): Error);
}


/*ARGSUSED*/
static
/*
*		patch_author(DBPROCESSdb,MedArtrec)
*/
int	patch_author( DBPROCESS *db, MedArt *rec )
{
    DBBINARY	patches[PATCH_BUFFER_SIZE];
    DBINT	patch_length = 0, item = 0;
    DBCHAR	buffer[TEXT_BUFFER_SIZE];
    int		rv = False;
    int	first;

    for( first = 1;; first = 0 ) {
	switch( load_patch( db, patches, sizeof(patches), &item,
				&patch_length, first ) )
	{
	    case True:	/* We found a patch */
		if( ApplyIsoPatches( rec->authors[item-1],
			sizeof(rec->authors[0]), buffer, sizeof(buffer),
			patches, patch_length ) == 0 )
		{
		    strcpy( rec->authors[item-1], buffer );
		    rv = True;
		}
		break;
	    case False:	/* No patch found */
		return( rv );
	    case Error:	/* Oops - */
		return( Error );
	}
    }
}


/*ARGSUSED*/
static
/*
*		process_abstract(DBPROCESSdb,MedArtrec,AbPListplist)
*/
int	process_abstract( DBPROCESS *db, MedArt *rec, AbPList *plist )
{
    DbNColumn(Abstract,abstract,line);	/* Line buffer */
    int	pi = 0;				/* Index to next patch */
    int	ti = 0;				/* Index to next text line */
    int next_line = 1;			/* Expect this line next */
    int	rv = False;
    RETCODE	rc;
    DBINT	line_no;

    while( (sizeof(rec->abstract) - ti) >= sizeof(line) ) {

	/* Skip patches to lines alsready passed */

	while(pi < plist->n_patches && next_line > plist->patches[pi].line_no)
		pi++;
	/* If a patch is expected, prepare for it */

	if( pi < plist->n_patches && next_line == plist->patches[pi].line_no ) {

	    /* The next line should be a patch line */
	    rc = SybaseLoad( db,
		1,	INTBIND,	sizeof(line_no), &line_no,
		2,	NTBSTRINGBIND,	sizeof(line),	 line,
		0 );

	    /* Apply the patch */

	    if( rc == REG_ROW && line_no == next_line ) {
		(void) ApplyIsoPatches( line, strlen(line), &rec->abstract[ti],
		    sizeof(rec->abstract) - ti, plist->patches[pi].start,
		    plist->patches[pi].length );
	    }
	} else {
	    rc = SybaseLoad( db,
		1,	INTBIND,	sizeof(line_no), &line_no,
		2,	NTBSTRINGBIND,	sizeof(line),	 &rec->abstract[ti],
		0 );
	    if( rc != REG_ROW ) break;
	}
	ti += strlen( &rec->abstract[ti] );	/* New abstract length */
	next_line = line_no + 1;

	/* Exit */
    }

    /* Return the status */

    return( (rv == Error) ? rv : ((ti > 0) ? True : False) );
}


/*ARGSUSED*/
static
/*
*		patch_abstract(DBPROCESSdb,MedArtrec,AbPListplist)
*/
int	patch_abstract( DBPROCESS *db, MedArt *rec, AbPList *plist )
{
    int	first = 1;
    int	rv;

    /* Load the patches */

    for( plist->n_patches = 0; plist->n_patches < MAX_ABSTRACT_PATCHES &&
	 (rv = load_patch( db, &plist->buffer[plist->used],
		sizeof(plist->buffer) - plist->used,
		&plist->patches[plist->n_patches].line_no,
		&plist->patches[plist->n_patches].length,
		first )) == True;
	 plist->n_patches++ )
    {
	/* A patch was loaded - record it */
	plist->used += plist->patches[plist->n_patches].length;
	first = 0;
    }

    /* Done */

    return( (rv == Error) ? Error : ((plist->n_patches > 0) ? True : False) );
}


static
int	mesh_compare( const void *m1, const void *m2 )
{
    return( strcmp( ((Mesh *) m1)->heading, ((Mesh *) m2)->heading ) );
}

static
/*
*		process_mesh(DBPROCESSdb,MedArtrec)
*/
int	process_mesh( DBPROCESS *db, MedArt *rec )
{
    DBINT	id_mesh;
    DbColumn	(Mesh,main_point);
    DbColumn	(Mesh,heading);
    RETCODE	rc;


    /* Read the main mesh headings into the table */

    rec->n_mesh = 0;

    while( (rc = SybaseLoad( db,
		1,	INTBIND,	0,			&id_mesh,
		2,	NTBSTRINGBIND,	sizeof(main_point),	 main_point,
		3,	NTBSTRINGBIND,	sizeof(heading),	 heading,
		0 )) == REG_ROW )
    {
	/* Load the MeSH heading into the table. */
	if( rec->n_mesh >= MAX_MESH_COUNT ) continue;	/* Too many */
	rec->mesh[rec->n_mesh].id_mesh = id_mesh;
	strcpy( rec->mesh[rec->n_mesh].main_point, main_point );
	strcpy( rec->mesh[rec->n_mesh].heading, heading );
	( rec->n_mesh )++;
    }

    /* Done */

    if ( rec->n_mesh > 0 )
	qsort( rec->mesh, rec->n_mesh, sizeof(Mesh), mesh_compare );

    return( rec->n_mesh == 0 ? False : True );
}


/*ARGSUSED*/
static
/*
*		process_subheading(DBPROCESSdb,MedArtrec)
*/
int	process_subheading( DBPROCESS *db, MedArt *rec )
{
    RETCODE	rc;
    DbColumn	(Subheading,main_point);
    DbColumn	(Subheading,code);
    DbColumn	(Subheading,name);
    DbColumn	(Subheading,description);
    int		id_mesh, i, seq;

    while( (rc = SybaseLoad( db,
		1,	INTBIND,	0,			&id_mesh,
		2,	NTBSTRINGBIND,	sizeof(main_point),	 main_point,
		3,	NTBSTRINGBIND,	sizeof(code),		 code,
		4,	NTBSTRINGBIND,	sizeof(name),		 name,
		5,	NTBSTRINGBIND,	sizeof(description),	 description,
		0 )) == REG_ROW )
    {
	Qual	*q;
	Mesh	*m;

	for ( i = 0, seq = -1; i< rec->n_mesh; i++ )
	   if ( id_mesh == rec->mesh[i].id_mesh )
	   {
		seq = i; break;
	   }

	if( seq == -1 ) continue;
	m = &rec->mesh[seq];

	if( m->n_qual >= MAX_QUALIFIER_COUNT ) continue;
	q = &m->qualifiers[m->n_qual++];
	strcpy(q->main_point, main_point);
	strcpy(q->code, code);
	strcpy(q->name, name);
	strcpy(q->description, description);
    }
    return( True );
}


/*ARGSUSED*/
static
/*
*		process_substance(DBPROCESSdb,MedArtrec)
*/
int	process_substance( DBPROCESS *db, MedArt *rec )
{
    int		i;
    RETCODE	rc;

    /* Read the subsances from the database */

    for( i = 0; i < MAX_SUBSTANCE_COUNT; i++ ) {
	rc = SybaseLoad( db,
		1,	NTBSTRINGBIND,	sizeof(rec->substances[0].number),
					rec->substances[i].number,
		2,	NTBSTRINGBIND,	sizeof(rec->substances[0].name),
					rec->substances[i].name,
		0 );
	if( rc != REG_ROW ) break;
    }
    rec->n_substances = i;

    return( (rc == FAIL) ? Error : ( (i==0) ? False : True ) );
}


/*ARGSUSED*/
static
/*
*		process_xref(DBPROCESSdb,MedArtrec)
*/
int	process_xref( DBPROCESS *db, MedArt *rec )
{
    int		i;
    RETCODE	rc;

    /* Read the subsances from the database */

    for( i = 0; i < MAX_XREF_COUNT; i++ ) {
	rc = SybaseLoad( db,
		1,	NTBSTRINGBIND,	sizeof(rec->xrefs[0].database),
					rec->xrefs[i].database,
		2,	NTBSTRINGBIND,	sizeof(rec->xrefs[0].accession),
					rec->xrefs[i].accession,
		0 );
	if( rc != REG_ROW ) break;
    }

    /* Make sure that + is last */

    rec->n_xrefs = i;

    return( (rc == FAIL) ? Error : ( (i==0) ? False : True ) );
}


/*ARGSUSED*/
static
/*
*		process_support(DBPROCESSdb,MedArtrec)
*/
int	process_support( DBPROCESS *db, MedArt *rec )
{
    int		i;
    RETCODE	rc;
    int		have_plus = 0;

    /* Read the subsances from the database */

    for( i = 0; i < MAX_IDNUM_COUNT; i++ ) {
	rc = SybaseLoad( db,
		1,	NTBSTRINGBIND,	sizeof(SupportId),	rec->idnums[i],
		0 );
	if( rc != REG_ROW ) break;

	/* If we get a +, just remeber that we saw it.  We'll put it last */

	if( strcmp( rec->idnums[i], "+" ) == 0 ) {
	    i--;
	    have_plus++;
	}
    }

    /* Add the plus back in */

    if( i < MAX_IDNUM_COUNT && have_plus > 0 ) {
	strcpy( rec->idnums[i], "+" );
	i++;
    }
    rec->n_idnum = i;

    return( (rc == FAIL) ? Error : ( (i==0) ? False : True ) );
}


/*ARGSUSED*/
static
/*
*		process_genesymbol(DBPROCESSdb,MedArtrec)
*/
int	process_genesymbol( DBPROCESS *db, MedArt *rec )
{
    int		i;
    RETCODE	rc;

    /* Read the subsances from the database */

    for( i = 0; i < MAX_GENESYM_COUNT; i++ ) {
	rc = SybaseLoad( db,
	    1,	NTBSTRINGBIND,	sizeof(GeneSymbol),	rec->gene_symbols[i],
	    0 );
	if( rc != REG_ROW ) break;
    }
    rec->n_gene_symbols = i;

    return( (rc == FAIL) ? Error : ( (i==0) ? False : True ) );
}


/*
*		ProcessMedlineArticle(DBPROCESS *db, MedArt *rec)
*/

int ProcessMedlineArticle( DBPROCESS *db, MedArt *rec, int is_pub )
{
    AbPList	plist;
    RETCODE	rc;
    DBCHAR	state_msg[40];		/* Text description of state */
    DBINT	state;
    int		status;

    /* Clear out the input record */

    bzero( rec, sizeof(MedArt) );
    bzero( &plist, sizeof(plist) );	/* Get rid of all patches */

    /* Collect the data from the database */

    for(status = True; status != Error; rc = dbresults( db )) {

	/* Get the next header row from the batch and then skip to
	   the results batch associated with the header (dbresults call) */

	rc = SybaseLoad( db,
		1,	INTBIND,	0,			&state,
		2,	NTBSTRINGBIND,	sizeof(state_msg),	state_msg,
		0 );
	if( rc == REG_ROW ) {
	    /* EOF marked with MR_STATE_DONE as the state */
	    if( state == MR_STATE_DONE ) 
		return( True );

	    /* Prepare to read the results section */
	    rc = dbresults(db);
	}
	if( rc != SUCCEED ) return( Error );

	/* Dispatch to the handler for this block of input */

	switch( state ) {
	    case MR_STATE_ARTICLE:	/* This one should be first */
		if ( is_pub )
			status = process_pub_article( db, rec );
		else
			status = process_article( db, rec );
		if( status != True ) return( status );
		break;
	    case MR_STATE_ISO_VTITLE:
		status = patch_string(db, rec->vtitle, sizeof(rec->vtitle));
		break;
	    case MR_STATE_ISO_ADDRESS:
		status = patch_string(db, rec->address, sizeof(rec->address));
		break;
	    case MR_STATE_AUTHOR:
		status = process_author( db, rec );
		break;
	    case MR_STATE_ISO_AUTHOR:
		status = patch_author( db, rec );
		break;
	    case MR_STATE_ISO_ABSTRACT:
		if( plist.n_patches > 0 )
		    bzero( &plist, sizeof(plist) );
		status = patch_abstract( db, rec, &plist );
		break;
	    case MR_STATE_ABSTRACT:
		status = process_abstract( db, rec, &plist );
		break;
	    case MR_STATE_MESH:
		status = process_mesh( db, rec );
		break;
	    case MR_STATE_SUBHEADING:
		status = process_subheading( db, rec );
		break;
	    case MR_STATE_SUBSTANCE:
		status = process_substance( db, rec );
		break;
	    case MR_STATE_CROSSREFERENCE:
		status = process_xref( db, rec );
		break;
	    case MR_STATE_SUPPORT:
		status = process_support( db, rec );
		break;
	    case MR_STATE_GENESYMBOL:
		status = process_genesymbol( db, rec );
		break;
	    default:			/* Unknown state - ignore it */
		status = skip_undefined_section( db );
		break;
	}
    }

    /* Return the proper status */

    return( Error );
}


/*
*		GetMedlineArticle(DBPROCESSdb,intui,intflags,MedArtrec)
*/
int	GetMedlineArticle( DBPROCESS *db, int ui, int flags, MedArt *rec )
{
    RETCODE	rc;

    dbfreebuf( db );
MACRO_before_exec
    rc = RunSybase( db, "execute mr_get_article %d, %d", ui, flags );
MACRO_after_exec(GetMedlineArticle)
    if( rc != SUCCEED ) return( Error );

    return( ProcessMedlineArticle( db, rec, 0 ) );
}

/*
*		GetMedlinePub(DBPROCESSdb,intui,intflags,MedArtrec)
*/
int	GetMedlinePub( DBPROCESS *db, int ui, int flags, MedArt *rec )
{
    RETCODE	rc;


    dbfreebuf( db );
MACRO_before_exec
    rc = RunSybase( db, "execute mr_get_pub %d, %d", ui, flags );
MACRO_after_exec(GetMedlinePub)
    if( rc != SUCCEED ) return( Error );

    return( ProcessMedlineArticle( db, rec, 1 ) );
}
