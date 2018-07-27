/*
*
*
* RCS Modification History:
* $Log: medschema.h,v $
* Revision 6.0  1997/08/25 18:36:35  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:55:15  epstein
* add RCS log revision history
*
*/

#ifndef	DEFS_MODULE_MEDSCHEMA_H

#include	"sybase.h"

#define	MAX_STRING_SIZE		255
#define	MAX_BINARY_SIZE		255
#define	MAX_ABSTRACT_SIZE	(6*1024)
#define	MAX_AUTHOR_COUNT	150
#define	MAX_MESH_COUNT		40
#define	MAX_QUALIFIER_COUNT	10
#define	MAX_SUBSTANCE_COUNT	50
#define	MAX_XREF_COUNT		50
#define	MAX_IDNUM_COUNT		20
#define	MAX_GENESYM_COUNT	25

#define	DbString(Name,Size) DBCHAR	Name[Size+1]
#define	DbBinary(Name,Size) DBBINARY	Name[Size+1]

/* Define the maximum character column size */

typedef DBCHAR          MaxString[SRV_MAXCHAR+1];
typedef unsigned char   MaxBinary[SRV_MAXCHAR];

/* Define the sizes of various fields in various tables */

/* Table Article */

#define	DBSIZE_Article_pages	40
#define	DBSIZE_Article_title	245
#define	DBSIZE_Article_title1	255
#define	DBSIZE_Article_vtitle	245
#define	DBSIZE_Article_vtitle1	255
#define	DBSIZE_Article_address	255

#define	DBSIZE_Abstract_abstract 255

#define	DBSIZE_CrossReference_accession 30

#define	DBSIZE_Databases_database_name	30

#define	DBSIZE_Issue_volume	25
#define	DBSIZE_Issue_issue	25
#define	DBSIZE_Issue_pubdate	30
#define	DBSIZE_Issue_special_list 1

#define	DBSIZE_Language_language_code	3
#define	DBSIZE_Mesh_heading	128
#define	DBSIZE_Mesh_main_point	1

#define	DBSIZE_Journal_med_abbr	128

#define	DBSIZE_Name_name	128

#define	DBSIZE_Subheading_code	2
#define	DBSIZE_Subheading_main_point 1
#define	DBSIZE_Subheading_name	30
#define	DBSIZE_Subheading_description	60

#define	DBSIZE_Substance_number	30
#define	DBSIZE_Substance_name	250

#define	DBSIZE_ResearchSupport_id	255

#define	DBSIZE_GeneSymbol_symbol	72

#if defined(__STDC__)
	/* NOTE: ANSI-C has a portable concatenation mechanism, unlike the
	   non ANSI-C compilers which use the code below. */

#define	DbNColumn(Table,Column,Name) \
	DBCHAR Name[DBSIZE_ ## Table ## _ ## Column + 1]
#define	DbTitle(Title) \
    DBCHAR Title[DBSIZE_Article_ ## Title + DBSIZE_Article_ ## Title ## 1 + 1]

#else
	/* WARNING- The method used for concatenation used in the macros
	   below is not portable.  This method works for Sun compilers.
	   Your compiler may use a different method.  If so, substitute
	   your concatenation operator for the slash,star,star,slash
	   below.  If your compiler doesn't support concatenation, you'll
	   have to recode the types by hand. */

#define	DbNColumn(Table,Column,Name) \
	DBCHAR Name[DBSIZE_/**/Table/**/_/**/Column + 1]
#define	DbTitle(Title) \
    DBCHAR Title[DBSIZE_Article_/**/Title + DBSIZE_Article_/**/Title/**/1 + 1]

#endif
#define	DbColumn(Table,Column) DbNColumn(Table,Column,Column)

#endif	/* DEFS_MODULE_MEDSCHEMA_H */
