/*  medart2asn.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the public for use. The National Library of Medicine and the U.S.    
*  Government have not placed any restriction on its use or reproduction.  
*                                                                          
*  Although all reasonable efforts have been taken to ensure the accuracy  
*  and reliability of the software and data, the NLM and the U.S.          
*  Government do not and cannot warrant the performance or results that    
*  may be obtained by using this software or data. The NLM and the U.S.    
*  Government disclaim all warranties, express or implied, including       
*  warranties of performance, merchantability or fitness for any particular
*  purpose.                                                                
*                                                                          
*  Please cite the author in any work or product based on this material.   
*
* ===========================================================================
*
* File Name:  getasnmed.c
*
* Author:  Karl Sirotkin
*   
* Version Creation Date: 2/26/92
*
* $Revision: 6.1 $
*
* File Description:  interface between getmedart.h and ncbi asn objects
*
* RCS Modification History:
* $Log: medart2asn.c,v $
* Revision 6.1  2000/02/03 21:17:23  beloslyu
* fix the problem with unset hh:mm:ss in NCBI_Date
*
* Revision 6.0  1997/08/25 18:36:27  madden
* Revision changed to 6.0
*
* Revision 1.5  1995/07/12 14:47:47  tatiana
* memset 0 for volume, issue arrays
*
 * Revision 1.4  1995/07/07  19:09:49  tatiana
 * a bug fixed in ma_get_parts() issue wasn't populated
 *
 * Revision 1.3  1995/05/30  17:58:28  tatiana
 * fix memory leaks
 *
 * Revision 1.2  1995/05/17  17:55:05  epstein
 * add RCS log revision history
 *
*/


#ifndef DEFS_MODULE_GETMEDART_H
#include "getmedart.h"
#endif

	
#ifndef _NCBI_Medline_
#include <objmedli.h>
#endif

#define Med2Asn_RESTART -1
#define Med2Asn_CONTINUE 0

#define ERR_OUT_OF_SPACE 1
/*------
 * These structures will not be ..New() ed or ...Free()ed.
 *-----*/
static Affil	MedAffil;
static AuthList MedAsnAuthList;
static MedlineEntry asnmedline_article;
static AsnIoPtr aip = NULL;
static NCBI_Date entry_month, journal_date;
static CitArt asn_article;
static ValNode asn_title, asn_journal_title;
static CitJour asn_journal;
static Imprint asn_imprint;
static Char datebuf[81];
static DBINT article_ui;
extern Char * NCBI_months[12];

/* added 08/02/94 for pages */

typedef struct token_boundary {
	int start_pos;
	int end_pos;
	char data[20];
} TokenBound, *TokenBoundPtr;

/* added 10/10/94 for parts. Tatiana */
CharPtr TextSave();

static InitStaticVariables()
{
   asnmedline_article.uid 	= 0;
   asnmedline_article.em 	= NULL;
   asnmedline_article.cit	= NULL;
   asnmedline_article.abstract 	= NULL;
   asnmedline_article.mesh 	= NULL;
   asnmedline_article.substance = NULL;
   asnmedline_article.xref 	= NULL;
   asnmedline_article.idnum 	= NULL;
   asnmedline_article.gene 	= NULL;

   asn_article.title    = NULL;
   asn_article.authors  = NULL;
   asn_article.fromptr  = NULL;
   asn_article.from     = 0;

   asn_journal.title 	= NULL;
   asn_journal.imp 	= NULL;

   asn_imprint.date 	= NULL;
   asn_imprint.volume 	= NULL;
   asn_imprint.issue 	= NULL;
   asn_imprint.pages 	= NULL;
   asn_imprint.section  = NULL;
   asn_imprint.part_sup = NULL;
   asn_imprint.language = NULL;
   asn_imprint.cprt 	= NULL;
   asn_imprint.part_supi= NULL;
   asn_imprint.retract  = NULL;
   asn_imprint.pub 	= NULL;
   asn_imprint.prepub	= 0;

   MedAsnAuthList.names = NULL;
   MedAsnAuthList.affil = NULL;

   MedAffil.affil 	= NULL;
   MedAffil.div 	= NULL;
   MedAffil.city	= NULL;
   MedAffil.sub 	= NULL;
   MedAffil.country	= NULL;
   MedAffil.street	= NULL;
   MedAffil.email 	= NULL;
   MedAffil.fax 	= NULL;
   MedAffil.phone	= NULL;
}

Boolean all_space(CharPtr beg, CharPtr end)
{
	Int2 i;
	CharPtr s;
	Boolean ret = FALSE;
	
	if (beg == NULL) {
		return ret;
	}
	for (s = beg; s < end && IS_WHITESP(*s); s++);
	if (s >= end) {
		ret = TRUE;
	}
	return ret;
}

/**************************************************************************
*  get_parts:
*     return a PARTS 
**************************************************************************/
Boolean ma_get_parts(CharPtr bptr, CharPtr eptr, ImprintPtr imp)
{
	static Char vv[255], vp[255], vi[255], vs[255];
	CharPtr s;
	CharPtr volume = NULL, end_vol, issue = NULL, end_issue;
	CharPtr sup_i = NULL, end_sup_i;
	CharPtr sup = NULL, end_sup;
	
	memset(vv, 0, 255);
	memset(vp, 0, 255);
	memset(vi, 0, 255);
	memset(vs, 0, 255);
	s = eptr;
	volume = bptr;
	for (end_vol = volume; IS_DIGIT(*end_vol); end_vol++);
	if (volume == end_vol) {
		sup = volume;
		volume = NULL;
	} else {
		sup = end_vol;
	}
	end_sup = eptr;
	if (*(s-1) == ')') {
		end_sup_i = s-1;
		for (s--; s >= bptr && *s != '('; s--);
		if (*s != '(') {
			    return FALSE;
		}
		end_sup = s;
		while (*end_sup == ' ')
			end_sup--;
		issue = s+1;
		if (*issue == ')') {
			issue = NULL;
			sup_i = NULL;
		} else {
			for (end_issue = issue; IS_DIGIT(*end_issue) || 
								*end_issue == '-'; end_issue++);
			if (issue == end_issue) {
				sup_i = issue;
				issue = NULL;
			} else {
				sup_i = end_issue;
			}
		}
	} 
	if (volume != NULL && *volume != '0' && !all_space(volume, end_vol)) {
		strncpy(vv, volume, end_vol-volume);
		imp->volume = vv;
	}
	if (sup != NULL && !all_space(sup, end_sup)) {
		strncpy(vp, sup, end_sup-sup);
		imp->part_sup = vp;
	}
	if (issue != NULL && !all_space(issue, end_issue)) {
		strncpy(vi, issue, end_issue-issue);
		imp->issue = vi;
	}
	if (sup_i != NULL && !all_space(sup_i, end_sup_i)) {
		strncpy(vs, sup_i, end_sup_i-sup_i);
		imp->part_supi = vs;
	} 
	return TRUE;
}

/*-----Med2AsnAuthNew()--------*/

ValNodePtr
Med2AsnAuthNew(restart)
	int * restart;
{
	static dex=0;
	static ValNode MedAsnAuth[MAX_AUTHOR_COUNT];
	ValNodePtr retval = NULL;

	if ( * restart == Med2Asn_RESTART){
		dex = 0;
		* restart = Med2Asn_CONTINUE;
	}
	if ( dex >= MAX_AUTHOR_COUNT){
		char	msg[80];
		sprintf( msg, "NO MORE AUTHOR SPACE FOR UI=%d", article_ui );
		ErrPost( CTX_NCBIMED2ASN, ERR_OUT_OF_SPACE, msg );
	}else {
		retval = & MedAsnAuth[dex ++ ];
	}

	return retval;
}

/*-----Med2AsnSubNew()--------*/

MedlineRnPtr
Med2AsnSubNew(restart)
	int * restart;
{
	static dex=0;
	static MedlineRn asnmedline_substance [MAX_SUBSTANCE_COUNT];
	MedlineRnPtr retval = NULL;

	if ( * restart == Med2Asn_RESTART){
		* restart = Med2Asn_CONTINUE;
		dex = 0;
	}
	if ( dex >= MAX_SUBSTANCE_COUNT){
		char	msg[80];
		sprintf( msg, "NO MORE SUBSTANCE SPACE FOR UI=%d", article_ui );
		ErrPost( CTX_NCBIMED2ASN, ERR_OUT_OF_SPACE, msg );
	}else {
		retval = & asnmedline_substance[dex ++ ];
	}

	return retval;
}


/*-----Med2AsnXrefNew()--------*/

ValNodePtr
Med2AsnXrefNew(restart)
	int * restart;
{
	static dex=0;
	static ValNode asnxrefs [MAX_XREF_COUNT];
	ValNodePtr retval = NULL;

	if ( * restart == Med2Asn_RESTART){
		* restart = Med2Asn_CONTINUE;
		dex = 0;
	}
	if ( dex >= MAX_XREF_COUNT){
		char	msg[80];
		sprintf( msg, "NO MORE XREF SPACE FOR UI=%d", article_ui );
		ErrPost( CTX_NCBIMED2ASN, ERR_OUT_OF_SPACE, msg );
	}else {
		retval = & asnxrefs[dex ++ ];
	}

	return retval;
}

/*-----Med2AsnSupportNew()--------*/

ValNodePtr
Med2AsnSupportNew(restart)
	int * restart;
{
	static dex=0;
	static ValNode SupportIds [MAX_IDNUM_COUNT];
	ValNodePtr retval = NULL;

	if ( * restart == Med2Asn_RESTART){
		* restart = Med2Asn_CONTINUE;
		dex = 0;
	}
	if ( dex >= MAX_IDNUM_COUNT){
		char	msg[80];
		sprintf( msg, "NO MORE SUPPORT_ID SPACE FOR UI=%d", article_ui );
		ErrPost( CTX_NCBIMED2ASN, ERR_OUT_OF_SPACE, msg );
	}else {
		retval = & SupportIds[dex ++ ];
	}

	return retval;
}

/*-----Med2AsnGeneSymNew()--------*/

ValNodePtr
Med2AsnGeneSymNew(restart)
	int * restart;
{
	static dex=0;
	static ValNode GeneSyms [MAX_GENESYM_COUNT];
	ValNodePtr retval = NULL;

	if ( * restart == Med2Asn_RESTART){
		* restart = Med2Asn_CONTINUE;
		dex = 0;
	}
	if ( dex >= MAX_GENESYM_COUNT){
		char	msg[80];
		sprintf( msg, "NO MORE GENESYM SPACE FOR UI=%d", article_ui );
		ErrPost( CTX_NCBIMED2ASN, ERR_OUT_OF_SPACE, msg );
	}else {
		retval = & GeneSyms[dex ++ ];
	}

	return retval;
}

/*-----Med2AsnMeshNew()--------*/

MedlineMeshPtr
Med2AsnMeshNew(restart)
	int * restart;
{
	static dex=0;
	static MedlineMesh asnmesh[MAX_MESH_COUNT];
	MedlineMeshPtr retval = NULL;

	if ( * restart == Med2Asn_RESTART){
		* restart = Med2Asn_CONTINUE;
		dex = 0;
	}
	if ( dex >= MAX_MESH_COUNT){
		char	msg[80];
		sprintf( msg, "NO MORE MESH SPACE FOR UI=%d", article_ui );
		ErrPost( CTX_NCBIMED2ASN, ERR_OUT_OF_SPACE, msg );
	}else {
		retval = & asnmesh[dex ++ ];
	}

	return retval;
}


/*-----Med2AsnQualNew()--------*/

ValNodePtr
Med2AsnQualNew(restart)
	int * restart;
{
	static dex=0;
	static ValNode MeshQual 
		[MAX_QUALIFIER_COUNT * MAX_MESH_COUNT];
	ValNodePtr retval = NULL;

	if ( * restart == Med2Asn_RESTART){
		* restart = Med2Asn_CONTINUE;
		dex = 0;
	}
	if ( dex >= MAX_QUALIFIER_COUNT * MAX_MESH_COUNT ){
		char	msg[80];
		sprintf( msg, "NO MORE QUAL SPACE FOR UI=%d", article_ui );
		ErrPost( CTX_NCBIMED2ASN, ERR_OUT_OF_SPACE, msg );
	}else {
		retval = & MeshQual[dex ++ ];
	}

	return retval;
}

static Uint1
xref_lookup(name)
	CharPtr name;
{
	Uint1 retval = 0;
	register int dex;
	static char * known_names [13] ={
        "ddbj",
        "carbbank",
        "embl",
        "hdb",
        "genbank",
        "hgml",
        "mim",
        "msd",
        "pdb",
        "pir",
        "prfseqdb",
        "psd",
        "swissprot"
};
/*
        ddbj (1) ,              -- DNA Data Bank of Japan
        carbbank (2) ,          -- Carbohydrate Structure Database
        embl (3) ,              -- EMBL Data Library
        hdb (4) ,               -- Hybridoma Data Bank
        genbank (5) ,           -- GenBank
        hgml (6) ,              -- Human Gene Map Library
        mim (7) ,               -- Mendelian Inheritance in Man
        msd (8) ,               -- Microbial Strains Database
        pdb (9) ,               -- Protein Data Bank (Brookhaven)
        pir (10) ,              -- Protein Identification Resource
        prfseqdb (11) ,         -- Protein Research Foundation (Japan)
        psd (12) ,              -- Protein Sequence Database (Japan)
        swissprot (13) } ,      -- SwissProt
*/
		for (dex = 0; dex < 13; dex ++ ){
			if (StringICmp(known_names[dex],name) == 0){
				retval = (Uint1) dex + 1;
				break;
			}
		}

	return retval;
}
/*----------- parsemeddate()--------*/
void parsemeddate(str)
	CharPtr str;
{
	CharPtr strt, tmp, tmp2, tmp3;
	int i;

	DateClean(&journal_date); /* fix the problem with unset hh:mm:ss */

	strt = StringNCpy(datebuf, str, 80);

	while ((*strt != '\0') && (*strt <= ' '))   /* skip leading blanks */
		strt++;

	if ((*strt == '1') && (*(strt+1) == '9') &&	   /* 19xx */
		(IS_DIGIT(*(strt+2))) && (IS_DIGIT(*(strt+3))))
	{                                              /* try to get date */
		journal_date.data[0] = 1;                  /* std date */
		sscanf(strt, "%d", &i);
		journal_date.data[1] = (Uint1)(i - 1900);

		while (*strt > ' ')
			strt++;
		while ((*strt != '\0') && (*strt <= ' '))   /* skip leading blanks */
			strt++;
		
		tmp = strt;
		while ((! journal_date.data[2]) &&
			(*tmp != '\0'))               /* try to find a month */
		{
			for (i = 0; i < 12; i++)
			{
			   if (! StringNICmp(tmp, NCBI_months[i], 3))
			   {
				journal_date.data[2] = i + 1;
				tmp2 = (tmp+3);         /* excise month string */
				while (IS_ALPHA(*tmp2))
					tmp2++;
				tmp3 = tmp2;
				if (*tmp3 != '\0')      /* look for day */
				{
				   while ((*tmp3 != '\0') && (*tmp3 <= ' '))
						tmp3++;
				   if (IS_DIGIT(*tmp3))
				   {
					sscanf(tmp3, "%d", &i);
					if ((i >= 1) && (i <= 31))
					{
					   journal_date.data[3] = i;
					   while (IS_DIGIT(*tmp3))
						tmp3++;
					   tmp2 = tmp3;
					}
				   }
				}
				if (tmp != strt)     /* remove leading blanks */
				{
				   tmp--;
				   while ((tmp != strt) && (*tmp <= ' '))
					tmp--;
				   if ((! journal_date.data[3]) && (tmp != strt))
				   {				  /* look for day before month */
					if (IS_DIGIT(*tmp))
					{
					   tmp3 = tmp;
					   while (IS_DIGIT(*tmp3) && (tmp3 != strt))
						tmp3--;
					   sscanf(tmp3, "%d", &i);
					   if ((i >= 1) && (i <= 31))
					   {
					      journal_date.data[3] = i;
					      tmp = tmp3;
					   }
					}
				   }
				}
				StringCpy(tmp, tmp2);
				i = 12;
			   }
			}
			
			if (! journal_date.data[2])  /* didn't find month */
			{
				while (*tmp > ' ')    /* skip to next word */
tmp++;
				while ((*tmp != '\0') && (*tmp <= ' '))   /* skip leading blanks */
					tmp++;
			}
		}

		while (*strt == ' ')   /* anything left for season? */
			strt++;
		tmp = strt;
		while ((*tmp != '\0') && (journal_date.str == NULL))
		{
			if (IS_ALPHANUM(*tmp))
				journal_date.str = strt;
			tmp++;
		}
	}
	else                                        /* only a string date */
		journal_date.str = tmp;
	return;
}

/*****************************************************************************/
/*  Date Added: 08/02/1994 by C. Chung
 *  Purpose:    To extend full page numbers, 311-3 to 311-313.
 ****************************************************************************/

/*  instances of pages:
3351-76 contd
74passim-75p
9C-10C
32P-3P
1921A-1921B
Suppl:1255-62
Suppl 1:163-74
1contd-3c
xxxv-xli
vii-x
E15-9
376-80; discussion 380-2
2035; discussion 2035-6
83-5, 89-90
414-16, 418-20, 422
1a-721a
6091-164 and i-xxi
53-62; quiz 163-4

comments: for the following three instances, 
	  only skip the '-', if it's the first char in the token
          didn't convert "f-21-f-3" to "f-21-f-23"

concil-1-3
F-13-17
f-21-f-23        
*/

static
ConvertPageToken( char * pages, TokenBoundPtr tbp )
{
   char         s[20], s2[20];
   char         *p;
   int          i, d1, d2;
   int		all_chars = TRUE;

   /* get the data of interest */
   
   sprintf( s, "%.*s", tbp->end_pos - tbp->start_pos, pages+tbp->start_pos );

   /* check '-' again for safegurad */

   if ( ( p = strchr( s, '-' ) ) == NULL ) {
	strcpy( tbp->data, s );
        return(0);
   }

   /* no processing needed if data containing no digits */

   for ( i = 0; i < (int)strlen(s); i++ )
	if ( isdigit( s[i] ) ) {
	   all_chars = FALSE;
	   break;
	}

   if ( all_chars ) {
	strcpy( tbp->data, s );
        return(0);
   }

   /* split data into two parts, s and s2 */

   strcpy( s2, p+1 );
   *(p+1) = '\0';

   /* count number of digits in the first part of data */

   for ( i=0, d1=0; s[i]; i++ )
	if ( isdigit(s[i]) ) d1++;
 
   /* count number of digits in the second part of data */

   for ( i=0, d2=0; s2[i]; i++ )
	if ( isdigit(s2[i]) ) d2++;

   for ( i=0; s[i]; i++ )	/* find first digit in the first part */
	if ( isdigit(s[i]) ) break;;

   if ( d1 - d2 <= 0 )		/* no need to extend pages */
	sprintf( tbp->data, "%s%s", s, s2 );
   else
	sprintf( tbp->data, "%s%.*s%s", s, d1-d2, s+i, s2 );
}

static
GetPageToken( char * pages, int offset, TokenBoundPtr cur_token )
{
   char *tpages=pages+offset;
   char *p, *t;
   int  i;

   if ( ( p = strchr( tpages, '-' ) ) == NULL )
	return( FALSE );

   if ( *p == *tpages )			/* skip first byte, it it's a '-' */
      if ( ( p = strchr( tpages+1, '-' ) ) == NULL )
	return( FALSE );

   for ( t=p-1; t != tpages && isalnum( *t ); t-- ) ;
   for ( i=0; tpages+i != t; i++ ) ;
   cur_token->start_pos = (i==0) ? offset: i+offset+1;

   for ( t=p+1; t < tpages+strlen(tpages)-1 && isalnum( *t ); t++ ) ; 
   for ( i=0; tpages+i != t; i++ ) ;

   if ( t == pages+strlen(pages)-1 )
	cur_token->end_pos = i+offset+1;
   else
	cur_token->end_pos = i+offset;

   return( TRUE );
}


static char *
ConvertPages( char * pages )
{
   TokenBound	tb[10];
   int		which_token=0, i, n;
   char		*p;
   char		s[30];
   char         res[80];

   tb[0].start_pos = 0;
   tb[0].end_pos = 0;

   while(1) {
	++which_token;
	if ( GetPageToken( pages, tb[which_token-1].end_pos, &tb[which_token] )  )
		ConvertPageToken( pages, &tb[which_token] );
	else {
	   --which_token;
	   break;
	}
   }

   res[0] = '\0';

   for ( i=1; i <= which_token; i++ ) {
	if ( ( n = tb[i].start_pos - tb[i-1].end_pos ) > 0 ) {
	   sprintf( s, "%.*s", n, pages+tb[i-1].end_pos );
	   strcat( res, s );
	}
	strcat( res, tb[i].data );
   }

   strcat( res, pages+tb[which_token].end_pos );
   strcpy( pages, res );

   return( ( pages[0]=='\0' ? NULL : pages ) );
}

/*****************************************************************************/

/*
	Date Added: 08/03/1994 by C. Chung
*/

static char *valid_volume_issue_chars="0123456789-?,";

static void
SeparatePartSuppl( CharPtr vol_issue, CharPtr part_sub )
{
   int i;

   *part_sub = '\0';

   /* skip leading spaces */
   for ( i=0; vol_issue[i] && vol_issue[i]==' '; i++ ) ;

   for (    ; vol_issue[i]; i++ ) 
	if ( StringChr( valid_volume_issue_chars, vol_issue[i] ) == NULL )
	   break;

   if ( vol_issue[i] ) {	/* not end of vol_issue */

	/* need to take care of 1st, 2nd, 3th ... */

	if ( StringLen( vol_issue+i ) > 2 ) {
	   if ( StringNICmp( vol_issue+i, "st", 2 ) == 0 ||
		StringNICmp( vol_issue+i, "nd", 2 ) == 0 ||
		StringNICmp( vol_issue+i, "th", 2 ) == 0 ) {
	      for ( i=i-1 ; i >=0 ; i-- )
		if ( ! isdigit( vol_issue[i] ) ) break;
	      if ( i == -1 ) i = 0;
	   }
	}

	/* for "1-B", "127, Pt 1" */

	if ( i > 0 && ( vol_issue[i-1]=='-' ||
			vol_issue[i-1]==',' ) ) i--;

	/* copy part data */

	StringCpy( part_sub, vol_issue+i );
	vol_issue[i] = '\0';
   }
}

/*****************************************************************************/

#define AssignValue( s ) ( s[0]=='\0' ? NULL : s )

MedlineEntryPtr
medlinePub2asn ( MedArt *article, int status )
{
	register int art_dex;
	MedlineEntryPtr medline_obj = & asnmedline_article, retval=NULL;
	int restart_Med2AsnAuthNew = Med2Asn_RESTART;
	CharPtr eptr, s;
	int len;

	InitStaticVariables();	/* added 08/04/94 */

	if ( status != True ) {
		medline_obj->cit = NULL;
		return( medline_obj );
	}
	article_ui = article->ui;		/* Fudge for error messages */

/*   MRI is discarded */
	medline_obj -> uid = article -> ui;
	medline_obj -> em = & entry_month;
	DateClean(&entry_month); /* fix problems with unset hh:mm:ss */
	entry_month.data[0] = 1;
	entry_month.data[1] = article->entry_month / 100;
	entry_month.data[2] = article->entry_month - 100 * entry_month.data[1] ;
	entry_month.data[3] = 0;

	medline_obj -> cit = & asn_article;
	asn_article.title = & asn_title;
	asn_title.choice = 1; /* name */
	asn_title.data.ptrvalue = NULL;
	if ( * article -> title) {
		asn_title.data.ptrvalue = article->title; /* when take vtitle?*/
	}
	if ( ! asn_title.data.ptrvalue ) {
		if ( * article -> vtitle) {
			asn_title.data.ptrvalue = article->vtitle;
		}
	}
	asn_article. authors = NULL;
	if (article -> n_authors > 0){
		ValNodePtr this_auth, last_auth=NULL;
		asn_article. authors = & MedAsnAuthList;
		MedAsnAuthList.names = NULL;
		MedAsnAuthList.choice = 2;
		for ( art_dex = 0; art_dex < article -> n_authors;
				 art_dex ++ ) {
			if (* (article -> authors) [art_dex]){
				this_auth = Med2AsnAuthNew( &
					restart_Med2AsnAuthNew);
				this_auth -> next = NULL;
				if ( ! MedAsnAuthList.names){
					MedAsnAuthList.names = this_auth;
				}
				if ( last_auth){
					last_auth -> next = this_auth;
				}
				this_auth ->  data.ptrvalue = 
					(article -> authors) [art_dex];
				last_auth = this_auth;
			}
		}
	}else{
/*---fudge if no authors ---*/
		ValNodePtr this_auth, last_auth=NULL;
		asn_article. authors = & MedAsnAuthList;
		MedAsnAuthList.names = NULL;
		MedAsnAuthList.choice = 3;
		for ( art_dex = 0; art_dex < article -> n_authors;
				 art_dex ++ ) {
				this_auth = Med2AsnAuthNew( &
					restart_Med2AsnAuthNew);
				this_auth -> next = NULL;
				if ( ! MedAsnAuthList.names){
					MedAsnAuthList.names = this_auth;
				}
				this_auth ->  data.ptrvalue = StringSave("");
		}
	}

	if ( strlen( article->address ) == 0 )
		MedAsnAuthList.affil = NULL;
	else
	{
		MedAsnAuthList.affil = &MedAffil;
		MedAffil.choice = 1;
		MedAffil.affil = article->address;
	}

	asn_article.from = 1;
	asn_article.fromptr = & asn_journal;
	asn_journal.title = & asn_journal_title;
	asn_journal_title.choice = article->title_type; /* iso/ml jta */
	asn_journal_title.data.ptrvalue = AssignValue( article->med_abbr );
	asn_journal.imp = & asn_imprint;
	asn_imprint.date = & journal_date;

	parsemeddate(article -> pubdate);
	if (article->issue != NULL && *(article->issue) != '\0') {
		len = StringLen(article->volume) + StringLen(article->issue);
		s = MemNew(len+5);
		sprintf(s, "%s (%s)", article->volume, article->issue);
		eptr = s + len + 3;
	} else {
		s = Nlm_StringSave(article->volume);
		eptr = s + StringLen(s);
	}
	if (!ma_get_parts(s, eptr, &asn_imprint)) {
		/* may be error message later */
	}
	MemFree(s);
	asn_imprint.pages = ConvertPages( article->pages );

	asn_imprint.section = NULL; /* not in medline? */
	asn_imprint.language = AssignValue( article->language_code );
	asn_imprint.cprt = NULL; /* not in medline? */
	asn_imprint.pub = NULL; /* not in medline? */

	retval = medline_obj;
	return retval;
}

/*----------- medline2asn()---------*/

MedlineEntryPtr
medline2asn (article, optionHead)
	MedArt * article;
	AsnOptionPtr optionHead;
{
	register int art_dex, dex;
	register int art_qual_dex, qual_dex;
	MedlineEntryPtr medline_obj = & asnmedline_article, retval=NULL;
	int restart_Med2AsnSubNew = Med2Asn_RESTART;
	int restart_Med2AsnXrefNew = Med2Asn_RESTART;
	int restart_Med2AsnSupportNew = Med2Asn_RESTART;
	int restart_Med2AsnGeneSymNew = Med2Asn_RESTART;
	int restart_Med2AsnMeshNew = Med2Asn_RESTART;
	int restart_Med2AsnQualNew = Med2Asn_RESTART;


	medline_obj = medlinePub2asn( article, True );

	medline_obj -> abstract = (*(article->abstract) == (char) 0)
		? NULL : article -> abstract;
	
	medline_obj -> mesh = NULL;
	if ( article -> n_mesh){
		MedlineMeshPtr last_mesh = NULL, this_mesh;
		Mesh * this_art_mesh;

		for ( art_dex = 0; art_dex < article -> n_mesh; art_dex ++ ) {
			this_art_mesh = & (article -> mesh[art_dex]);
			if (* ( this_art_mesh -> heading) ){
				this_mesh = Med2AsnMeshNew( 
					&restart_Med2AsnMeshNew);
				this_mesh -> next = NULL;
				if ( ! medline_obj -> mesh){
					medline_obj -> mesh = this_mesh;
				}
				if ( last_mesh){
					last_mesh -> next = this_mesh;
				}
				this_mesh -> term = this_art_mesh -> heading;
				last_mesh = this_mesh;
				if ( ( this_art_mesh -> main_point)[0] == '*'){
					this_mesh -> mp = TRUE;
				}else{
					this_mesh -> mp = FALSE;
				}
				this_mesh -> qual = NULL;
				if ( this_art_mesh -> n_qual > 0){
					ValNodePtr this_qual, last_qual = NULL;
					Qual * this_art_qual;

					for ( qual_dex = art_qual_dex = 0; 
							art_qual_dex < this_art_mesh -> n_qual; 
							art_qual_dex ++ ) {
						this_art_qual = & 
						  ((this_art_mesh->qualifiers)[art_qual_dex]);

						if (* (this_art_qual -> description)){
							this_qual = Med2AsnQualNew ( &
								restart_Med2AsnQualNew);
							if ( ( this_art_qual -> main_point)[0] == '*'){
								this_qual -> choice = TRUE;
							}else{
								this_qual -> choice = FALSE;
							}
							this_qual -> next = NULL;
							if ( ! this_mesh -> qual){
								this_mesh -> qual = this_qual;
							}
							if (last_qual){
								last_qual -> next = this_qual;
							}
							last_qual = this_qual;
							this_qual -> data.ptrvalue 
								= this_art_qual -> description;
						}
					}
				}
			}
		}
	}
/*----
    type ENUMERATED {           -- type of record
        nameonly (0) ,
        cas (1) ,               -- CAS number
        ec (2) } ,              -- EC number
---*/
	medline_obj -> substance = NULL;
	if ( article -> n_substances){
		MedlineRnPtr this_substance, last_substance=NULL;
		for ( art_dex = 0; art_dex < article -> n_substances;
				 art_dex ++ ) {
			if (* ((article -> substances) [art_dex]).name){
				this_substance = Med2AsnSubNew( & 
					restart_Med2AsnSubNew);
				this_substance -> next = NULL;
				if ( ! medline_obj -> substance){
					medline_obj -> substance = this_substance;
				}
				if ( last_substance){
					last_substance -> next = this_substance;
				}
				this_substance -> type = 0;
				this_substance ->  name = 
					((article -> substances) [art_dex]).name;
				this_substance ->  cit = 
					((article -> substances) [art_dex]).number;
				last_substance = this_substance;
				if ( this_substance -> cit) 
					if (* (this_substance -> cit ) ){
						if (strcmp (this_substance -> cit,"0" ) == 0 ){
							this_substance -> type = 0;
							this_substance ->  cit = NULL;
						}else{
							this_substance -> type = 1;
							if (* (this_substance -> cit )== 'E' &&
									*(this_substance -> cit +1 ) == 'C' ){
								this_substance -> type = 2;
								(this_substance -> cit) += 2;
								while( isspace( *(this_substance -> cit) ) )
									(this_substance -> cit) ++;
							}
						}
					}
			}
		}
	}

	medline_obj -> xref = NULL;
	if ( article -> n_xrefs){
		ValNodePtr this_xref, last_xref=NULL;
		Uint1 choice;

		for ( art_dex = 0; art_dex < article -> n_xrefs;
				 art_dex ++ ) {
			choice = xref_lookup( ((article -> xrefs) [art_dex]).database);
			if (* ((article -> xrefs) [art_dex]).accession && choice > 0){
				this_xref = Med2AsnXrefNew( & 
					restart_Med2AsnXrefNew);
				this_xref -> next = NULL;
				if ( ! medline_obj -> xref){
					medline_obj -> xref = this_xref;
				}
				if ( last_xref){
					last_xref -> next = this_xref;
				}
				last_xref = this_xref;
				this_xref -> choice = choice;
				this_xref ->  data.ptrvalue = 
					((article -> xrefs) [art_dex]).accession;
			}
		}
	}

	medline_obj -> idnum = NULL;
	if ( article -> n_idnum){
		ValNodePtr this_idnum, last_idnum=NULL;
		for ( art_dex = 0; art_dex < article -> n_idnum;
				 art_dex ++ ) {
			if ( ((article -> idnums) [art_dex])[0]){
				this_idnum = Med2AsnSupportNew( & 
					restart_Med2AsnSupportNew);
				this_idnum -> next = NULL;
				if ( ! medline_obj -> idnum){
					medline_obj -> idnum = this_idnum;
				}
				if ( last_idnum){
					last_idnum -> next = this_idnum;
				}
				last_idnum = this_idnum;
				this_idnum -> choice = 1; 
				this_idnum ->  data.ptrvalue = 
					 ((article -> idnums) [art_dex]);
			}
		}
	}

	medline_obj -> gene = NULL;
	if ( article -> n_gene_symbols){
		ValNodePtr this_gene, last_gene=NULL;
		for ( art_dex = 0; art_dex < article -> n_gene_symbols;
				 art_dex ++ ) {
			if ( ((article -> gene_symbols) [art_dex])[0]){
				this_gene = Med2AsnGeneSymNew( & 
					restart_Med2AsnGeneSymNew);
				this_gene -> next = NULL;
				if ( ! medline_obj -> gene){
					medline_obj -> gene = this_gene;
				}
				if ( last_gene){
					last_gene -> next = this_gene;
				}
				last_gene = this_gene;
				this_gene -> choice = 1; 
				this_gene ->  data.ptrvalue = 
					 ((article -> gene_symbols) [art_dex]);
			}
		}
	}

	retval = medline_obj;
	return retval;
}

/*----------MedArt_ASNTTY()-----------*/

int
MedArt_ASNTTY(fp, article)
	FILE * fp;  /* ignored for this hack */
	MedArt * article;
{
	MedlineEntryPtr asnmed;
	Boolean retval;

	asnmed = medline2asn(article, NULL);
	if ( ! aip){
		aip = AsnIoOpen("stdout","w");
	}

	if (aip && asnmed){
		retval = MedlineEntryAsnWrite(asnmed, aip, NULL);
		AsnIoClose(aip);
		return retval;
	}else{
		return FALSE;
	}
}
