/*
	Edit History:

	    27 February 1992	Rand S. Huntzinger
		Changed typedef of Author to be AuthName to avoid a naming
		conflict the the ASN1 code.
*
*
* RCS Modification History:
* $Log: getmedart.h,v $
* Revision 6.0  1997/08/25 18:36:14  madden
* Revision changed to 6.0
*
* Revision 1.2  1995/05/17 17:54:45  epstein
* add RCS log revision history
*
*/

#ifndef	DEFS_MODULE_GETMEDART_H
#define	DEFS_MODULE_GETMEDART_H

#include "medschema.h"

#define	True	(1)
#define	False	(0)
#define	Error	(-1)

/* These flags can be or'ed together to control the data obtained from
   the database using either the mr_get_article SQL procedure (used int
   the GetMedlineArticle() procedure) or via the bulk access mechanism. */

#define	MED_ISO8859_FLAG	(1 << 0)
#define	MED_ABSTRACT_FLAG	(1 << 1)
#define	MED_MESH_FLAG		(1 << 2)
#define	MED_QUALIFIER_FLAG	(1 << 3)
#define	MED_SUBSTANCE_FLAG	(1 << 4)
#define	MED_CROSSREF_FLAG	(1 << 5)
#define	MED_SUPPORT_FLAG	(1 << 6)
#define	MED_GENESYMBOL_FLAG	(1 << 7)

typedef DbNColumn(Name,name,AuthName);

typedef DbNColumn(ResearchSupport,id,SupportId);

typedef	DbNColumn(GeneSymbol,symbol,GeneSymbol);

typedef	struct Qual {
	DbColumn	(Subheading,main_point);
	DbColumn	(Subheading,code);
	DbColumn	(Subheading,name);
	DbColumn	(Subheading,description);
} Qual;

typedef struct Mesh {
	DBINT		id_mesh;
	DbColumn	(Mesh,main_point);
	DbColumn	(Mesh,heading);
	int		n_qual;
	Qual		qualifiers[MAX_QUALIFIER_COUNT];
} Mesh;

typedef	struct Substance {
	DbColumn	(Substance,number);
	DbColumn	(Substance,name);

} Substance;

typedef	struct Xref {
	DbNColumn	(Databases,database_name,database);
	DbColumn	(CrossReference,accession);
} Xref;

typedef struct MedlineArticle {
	/* Basic data */
	DBINT		ui;
	DBINT		mri;
	DBINT		title_type;
	DbTitle		(title);
	DbTitle		(vtitle);
	DbColumn	(Article,address);
	DbColumn	(Article,pages);
	DBSMALLINT	entry_month;
	DbColumn	(Issue,volume);
	DbColumn	(Issue,issue);
	DbColumn	(Issue,pubdate);
	DbColumn	(Journal,med_abbr);
	DbColumn	(Language,language_code);
	/* Abstract */
	DBCHAR		abstract[MAX_ABSTRACT_SIZE];
	/* Authors */
	int		n_authors;
	AuthName	authors[MAX_AUTHOR_COUNT];
	/* Substances */
	int		n_substances;
	Substance	substances[MAX_SUBSTANCE_COUNT];
	/* CrossReferences */
	int		n_xrefs;
	Xref		xrefs[MAX_XREF_COUNT];
	/* MeSH terms */
	int		n_mesh;
	Mesh		mesh[MAX_MESH_COUNT];
	/* Research grant/contract id's */
	int		n_idnum;
	SupportId	idnums[MAX_IDNUM_COUNT];
	/* Gene Symbols */
	int		n_gene_symbols;
	GeneSymbol	gene_symbols[MAX_GENESYM_COUNT];
	char		part_sup[25];	/* part/sup of volume */
	char		part_supi[25];	/* part/sup of issue  */
} MedArt;


#ifdef	__STDC_
extern int GetMedlineArticle( DBPROCESS *db, int ui, int flags, MedArt *rec );
#else
extern int GetMedlineArticle();
#endif

#endif /* DEFS_MODULE_GETMEDART_H */
