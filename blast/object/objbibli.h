/*  objbibli.h
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
* File Name:  objbibli.h
*
* Author:  James Ostell
*   
* Version Creation Date: 1/1/91
*
* $Revision: 6.1 $
*
* File Description:  Object manager interface for module NCBI-Biblio
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-17-93 Schuler     struct affil  --->  struct _affil_  (because one of
*                      the fields in the struct is also called affil, which
*                      C++ compilers take to be a constructor function).
*
* $Log: objbibli.h,v $
* Revision 6.1  2000/01/18 19:40:52  ostell
* added support for PubStatusDate, ArticleIds
*
* Revision 6.0  1997/08/25 18:49:23  madden
* Revision changed to 6.0
*
* Revision 4.2  1997/06/19 18:40:50  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.1  1996/03/29 21:09:59  ostell
* added support for PubMedId
*
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.2  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_Biblio_
#define _NCBI_Biblio_

#ifndef _ASNTOOL_
#include <asn.h>
#endif
#ifndef _NCBI_General_
#include <objgen.h>
#endif

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   loader
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL BiblioAsnLoad PROTO((void));

/*****************************************************************************
*
*   ArticleId
*
*****************************************************************************/
typedef ValNode ArticleId, PNTR ArticleIdPtr;

#define ARTICLEID_PUBMED 1    /* pubmed id - integer */
#define ARTICLEID_MEDLINE 2   /* medline uid - integer */
#define ARTICLEID_DOI 3       /* doi - string */
#define ARTICLEID_PII 4       /* pii  - string */
#define ARTICLEID_PMCID 5     /* pubmed central id - integer */
#define ARTICLEID_PMCPID 6    /* publisherid from Pubmed central - string */
#define ARTICLEID_PMPID 7     /* publisherId from pubmed - string */
#define ARTICLEID_OTHER 8     /* generic - DbTag */

NLM_EXTERN ArticleIdPtr LIBCALL ArticleIdNew PROTO((void));
NLM_EXTERN ArticleIdPtr LIBCALL ArticleIdFree PROTO((ArticleIdPtr aidp));
NLM_EXTERN ArticleIdPtr LIBCALL ArticleIdAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean  LIBCALL ArticleIdAsnWrite PROTO((ArticleIdPtr aidp, AsnIoPtr aip, AsnTypePtr atp));


/*****************************************************************************
*
*   PubStatusDate
*
*****************************************************************************/
#define PUBSTATUS_received 1
#define PUBSTATUS_accepted 2
#define PUBSTATUS_epublish 3
#define PUBSTATUS_ppublish 4
#define PUBSTATUS_revised 5
#define PUBSTATUS_pmc 6
#define PUBSTATUS_pmcr 7
#define PUBSTATUS_pubmed 8
#define PUBSTATUS_pubmedr 9
#define PUBSTATUS_aheadofprint 10
#define PUBSTATUS_premedline 11
#define PUBSTATUS_medline 12
#define PUBSTATUS_other 255

typedef struct _pubstatusdate_ {
	Uint1 pubstatus;  
	DatePtr date;
        struct _pubstatusdate_ * next;
} PubStatusDate, PNTR PubStatusDatePtr;

NLM_EXTERN PubStatusDatePtr LIBCALL PubStatusDateNew PROTO((void));
NLM_EXTERN PubStatusDatePtr LIBCALL PubStatusDateFree PROTO((PubStatusDatePtr psdp));
NLM_EXTERN PubStatusDatePtr LIBCALL PubStatusDateAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean  LIBCALL PubStatusDateAsnWrite PROTO((PubStatusDatePtr psdp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   Affil
*
*****************************************************************************/
typedef struct _affil_ {
	Uint1 choice;           /* [1]=str,[2]=std */
	CharPtr affil,          /* also used for str */
		div,
		city,
		sub,
		country,
		street,
		email,
		fax,
		phone,
                postal_code;
} Affil, PNTR AffilPtr;

NLM_EXTERN AffilPtr LIBCALL AffilNew PROTO((void));
NLM_EXTERN AffilPtr LIBCALL AffilFree PROTO((AffilPtr afp));
NLM_EXTERN AffilPtr LIBCALL AffilAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean  LIBCALL AffilAsnWrite PROTO((AffilPtr afp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   AuthList
*
*****************************************************************************/
typedef struct authors {
	Uint1 choice;        /* [1]=std, [2]=ml, [3]=str (only on Cit-art,gen) */
	ValNodePtr names;    /* the SEQUENCE OF */
	AffilPtr affil;
} AuthList, PNTR AuthListPtr;

NLM_EXTERN AuthListPtr LIBCALL AuthListNew PROTO((void));
NLM_EXTERN AuthListPtr LIBCALL AuthListFree PROTO((AuthListPtr asp));
NLM_EXTERN AuthListPtr LIBCALL AuthListAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean     LIBCALL AuthListAsnWrite PROTO((AuthListPtr afp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2        LIBCALL AuthListMatch PROTO((AuthListPtr a, AuthListPtr b, Boolean all));

/*****************************************************************************
*
*   Author
*
*****************************************************************************/
typedef struct author {
	PersonIdPtr name;
	Uint1 lr[2];       /* level[0], role[1] as in spec. 0=not used */
	Uint1 is_corr;     /* corresponding author? 255=not set, 0=false, 1=true */
	AffilPtr affil;
} Author, PNTR AuthorPtr;

NLM_EXTERN AuthorPtr LIBCALL AuthorNew PROTO((void));
NLM_EXTERN AuthorPtr LIBCALL AuthorFree PROTO((AuthorPtr ap));
NLM_EXTERN AuthorPtr LIBCALL AuthorAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean   LIBCALL AuthorAsnWrite PROTO((AuthorPtr ap, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   Cit-art
*
*****************************************************************************/
typedef struct citart {
	ValNodePtr title;       /* choice[1]=name,[2]=tsub,[3]=trans */
	AuthListPtr authors;
	Uint1 from;             /* [1]=journal,[2]=book,[3]=proc */
	Pointer fromptr;
	ArticleIdPtr ids;
} CitArt, PNTR CitArtPtr;

NLM_EXTERN CitArtPtr LIBCALL CitArtNew PROTO((void));
NLM_EXTERN CitArtPtr LIBCALL CitArtFree PROTO((CitArtPtr cap));
NLM_EXTERN CitArtPtr LIBCALL CitArtAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean   LIBCALL CitArtAsnWrite PROTO((CitArtPtr cap, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2      LIBCALL CitArtMatch PROTO((CitArtPtr a, CitArtPtr b));

/*****************************************************************************
*
*   Retraction/Erratum
*
*****************************************************************************/
typedef struct citretract {
	Uint1 type;  /* 1=retracted, 2=retraction notice, 3=in error, 4=erratum */
    CharPtr exp; /* explanation */
} CitRetract, PNTR CitRetractPtr;

NLM_EXTERN CitRetractPtr LIBCALL CitRetractNew PROTO((void));
NLM_EXTERN CitRetractPtr LIBCALL CitRetractFree PROTO((CitRetractPtr crp));
NLM_EXTERN CitRetractPtr LIBCALL CitRetractAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean   LIBCALL CitRetractAsnWrite PROTO((CitRetractPtr crp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   Imprint
*
*****************************************************************************/
typedef struct imprint {
	DatePtr date;
    CharPtr volume,
        issue,
        pages,
        section,
        part_sup,	  /* part/sup of volume */
        language;
    DatePtr cprt;     /* copy right date (for books) */
    AffilPtr pub;   /* publisher (for books)  */
	Uint1 prepub;   /* 0=not set 1=submitted 2=in-press 255=other */
	CharPtr part_supi;   /* part/sup of issue */
	CitRetractPtr retract;
        Uint1 pubstatus;
        PubStatusDatePtr history;
} Imprint, PNTR ImprintPtr;

NLM_EXTERN ImprintPtr LIBCALL ImprintNew PROTO((void));
NLM_EXTERN ImprintPtr LIBCALL ImprintFree PROTO((ImprintPtr cap));
NLM_EXTERN ImprintPtr LIBCALL ImprintAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean    LIBCALL ImprintAsnWrite PROTO((ImprintPtr cap, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2       LIBCALL ImprintMatch PROTO((ImprintPtr a, ImprintPtr b, Boolean all));

/*****************************************************************************
*
*   Cit-jour
*
*****************************************************************************/
typedef struct citjour {
	ValNodePtr title;     /* choice in order of spec, 1=name,2=trans,etc */
	ImprintPtr imp;
} CitJour, PNTR CitJourPtr;

NLM_EXTERN CitJourPtr LIBCALL CitJourNew PROTO((void));
NLM_EXTERN CitJourPtr LIBCALL CitJourFree PROTO((CitJourPtr cjp));
NLM_EXTERN CitJourPtr LIBCALL CitJourAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean    LIBCALL CitJourAsnWrite PROTO((CitJourPtr cjp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2       LIBCALL CitJourMatch PROTO((CitJourPtr a, CitJourPtr b));

/*****************************************************************************
*
*   Cit-book
*
*****************************************************************************/
typedef struct citbook {
	ValNodePtr title,      /* choice in order of spec, 1=name, 2=tsub, etc */
			   coll;       /* ditto */
	AuthListPtr authors;
	ImprintPtr imp;
	Uint1 othertype,      /* 0=Cit-book, 1=Cit-proc, 2=Cit-let */
		let_type;         /* if Cit-let, 1=manuscript,2=letter,3=thesis */
	Pointer otherdata;    /* NULL,  ValNodes, CharPtr man-id */
} CitBook, PNTR CitBookPtr;

NLM_EXTERN CitBookPtr LIBCALL CitBookNew PROTO((void));
NLM_EXTERN CitBookPtr LIBCALL CitBookFree PROTO((CitBookPtr cbp));
NLM_EXTERN CitBookPtr LIBCALL CitBookAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean    LIBCALL CitBookAsnWrite PROTO((CitBookPtr cbp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2       LIBCALL CitBookMatch PROTO((CitBookPtr a, CitBookPtr b));

/*****************************************************************************
*
*   Cit-sub
*   	Direct submission of data
*
*****************************************************************************/
typedef struct citsub {
	AuthListPtr authors;
	ImprintPtr imp;		   /* WARNING: imp will be removed next year */
	Uint1 medium;
	DatePtr date;          /* this will replace imp */
	CharPtr descr;         /* description of what happened  */
} CitSub, PNTR CitSubPtr;

NLM_EXTERN CitSubPtr LIBCALL CitSubNew PROTO((void));
NLM_EXTERN CitSubPtr LIBCALL CitSubFree PROTO((CitSubPtr cbp));
NLM_EXTERN CitSubPtr LIBCALL CitSubAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean   LIBCALL CitSubAsnWrite PROTO((CitSubPtr cbp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2      LIBCALL CitSubMatch PROTO((CitSubPtr a, CitSubPtr b));


/*****************************************************************************
*
*   Cit-proc
*     uses otherdata in Cit-book
*     chain of ValNodes
*     choice       ident      Pointer type
*       1          number      CharPtr
*       2          date        DatePtr
*       3          place       AffilPtr
*
*****************************************************************************/
NLM_EXTERN CitBookPtr LIBCALL CitProcAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean    LIBCALL CitProcAsnWrite PROTO((CitBookPtr cpp, AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   Cit-let
*     uses otherdata in Cit-book as CharPtr for man-id
*
*****************************************************************************/
NLM_EXTERN CitBookPtr LIBCALL CitLetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean    LIBCALL CitLetAsnWrite PROTO((CitBookPtr cpp, AsnIoPtr aip, AsnTypePtr atp));



/*****************************************************************************
*
*   Cit-pat
*
*****************************************************************************/

typedef struct patpriority {
	CharPtr country,
		number;
	DatePtr date;
	struct patpriority PNTR next;
} PatPriority, PNTR PatPriorityPtr;

typedef struct citpat {
	CharPtr title;
	AuthListPtr authors;
	CharPtr country,
		doc_type,
		number;
	DatePtr date_issue;
	ValNodePtr _class;
	CharPtr	app_number;
	DatePtr app_date;
	AuthListPtr applicants;
	AuthListPtr assignees;
	PatPriorityPtr priority;
	CharPtr abstract;
} CitPat, PNTR CitPatPtr;

NLM_EXTERN CitPatPtr LIBCALL CitPatNew PROTO((void));
NLM_EXTERN CitPatPtr LIBCALL CitPatFree PROTO((CitPatPtr cpp));
NLM_EXTERN CitPatPtr LIBCALL CitPatAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean   LIBCALL CitPatAsnWrite PROTO((CitPatPtr cpp, AsnIoPtr aip, AsnTypePtr atp));

NLM_EXTERN PatPriorityPtr LIBCALL PatPriorityNew PROTO((void));
NLM_EXTERN PatPriorityPtr LIBCALL PatPrioritySetFree PROTO((PatPriorityPtr ppp));
NLM_EXTERN PatPriorityPtr LIBCALL PatPrioritySetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN Boolean   LIBCALL PatPrioritySetAsnWrite PROTO((PatPriorityPtr ppp, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
/*****************************************************************************
*
*   Id-pat
*
*****************************************************************************/
typedef struct idpat {
	CharPtr country,
		number,				 /** actually CHOICE of number or app_number */
		app_number;
	CharPtr doc_type;
} IdPat, PNTR IdPatPtr;

NLM_EXTERN IdPatPtr LIBCALL IdPatNew PROTO((void));
NLM_EXTERN IdPatPtr LIBCALL IdPatFree PROTO((IdPatPtr ipp));
NLM_EXTERN IdPatPtr LIBCALL IdPatAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean  LIBCALL IdPatAsnWrite PROTO((IdPatPtr ipp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean  LIBCALL IdPatMatch PROTO((IdPatPtr a, IdPatPtr b));

/*****************************************************************************
*
*   Cit-gen
*
*****************************************************************************/
typedef struct cit_gen {
	CharPtr cit;
	AuthListPtr authors;
    Int4 muid;                  /* medline uid, -1 if not set */
    ValNodePtr journal;         /* journal/book Title */
    CharPtr volume,
        issue,
        pages;
	DatePtr date;
    Int2 serial_number;      /* for GenBank style references (-1 = not used)*/
	CharPtr title;           /* a specific title (in addition to cit or journal) */
	Int4 pmid;               /* PubMedId */
} CitGen, PNTR CitGenPtr;

NLM_EXTERN CitGenPtr LIBCALL CitGenNew PROTO((void));
NLM_EXTERN CitGenPtr LIBCALL CitGenFree PROTO((CitGenPtr cgp));
NLM_EXTERN CitGenPtr LIBCALL CitGenAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean   LIBCALL CitGenAsnWrite PROTO((CitGenPtr cgp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2      LIBCALL CitGenMatch PROTO((CitGenPtr a, CitGenPtr b, Boolean all));

/*****************************************************************************
*
*   Title
*
*****************************************************************************/

NLM_EXTERN ValNodePtr LIBCALL TitleFree PROTO((ValNodePtr anp));
NLM_EXTERN ValNodePtr LIBCALL TitleAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Boolean    LIBCALL TitleAsnWrite PROTO((ValNodePtr anp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN Int2       LIBCALL TitleMatch PROTO((ValNodePtr a, ValNodePtr b, Uint1 type));

#define Cit_title_name ( (Uint1) 1)
#define Cit_title_tsub ( (Uint1) 2)
#define Cit_title_trans ( (Uint1) 3)
#define Cit_title_jta ( (Uint1) 4)
#define Cit_title_iso_jta ( (Uint1) 5)
#define Cit_title_ml_jta ( (Uint1) 6)
#define Cit_title_coden ( (Uint1) 7)
#define Cit_title_issn ( (Uint1) 8)
#define Cit_title_abr ( (Uint1) 9)
#define Cit_title_isbn ( (Uint1) 10)

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
