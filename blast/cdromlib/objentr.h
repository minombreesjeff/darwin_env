/*   objentr.h
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
* RCS $Id: objentr.h,v 6.3 1999/08/17 16:31:27 kans Exp $
*
* Author:  Ostell, Schuler
*
* Version Creation Date:   6/28/94
*
* File Description:         
*       entrez index access library for Entrez
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 06-28-94 Schuler     This file created from definitions extracted
*                      from accentr.h
* 11-21-94 Schuler     Changed DocUid from long to Int4
* 11-30-94 Schuler     Added cd_count field to EntrezInfo
* 03-13-95 Schuler     Bumped DocType_MAX up to 8 (was 5)
* 05-16-94 Schuler     Added RCS Log directive.  Comments will henceforth
*                      be inserted automatically into the source at the
*                      time it is checked into the system.

$Log: objentr.h,v $
Revision 6.3  1999/08/17 16:31:27  kans
increased field tag size to handle current data

Revision 6.2  1998/03/26 23:23:19  yaschenk
adding CdRomAsnLoad() as extern function to load Asn tree

Revision 6.1  1997/10/07 22:09:46  volodya
new data fields (entrezDate, pStatus, pLanguage, pPubType) have added to DocSum structure

Revision 6.0  1997/08/25 18:13:18  madden
Revision changed to 6.0

Revision 5.3  1997/06/26 21:55:48  vakatov
[PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"

Revision 5.2  1997/03/05 16:48:47  brandon
added link information to EntrezInfo structure

 * Revision 5.1  1996/07/22  15:31:40  brandon
 * *** empty log message ***
 *
 * Revision 5.0  1996/05/28  13:55:34  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 1.8  1995/07/10  19:42:49  epstein
 * add object loaders for DocSum
 *
 * Revision 1.7  1995/05/16  14:12:46  schuler
 * Automatic comment insertion enabled
 *
* ==========================================================================
*/

#ifndef _ObjEntr_h_
#define _ObjEntr_h_

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


#define DocType_MAX 5

/*typedef long DocUid;*/
#define DocUid Int4

typedef struct CdDate
{
	unsigned year   : 12;
	unsigned month  : 5;
	unsigned day    : 6;
}
CdDate;

typedef struct DocSum 
{
	DocUid uid;
	DocUid secondaryUid;
	unsigned non_document :1;
	unsigned translated_title :1;
	unsigned no_authors :1;
	unsigned no_abstract :1;
	unsigned is_segmented :1;
	unsigned is_partial :1;
	unsigned not_yet_neighbored :1;
	CdDate create;
	CdDate modify;
	char *caption;
	char *title;
	char *extra;
	short link_count[DocType_MAX];
        Uint4    entrezDate; /* Entrez Date */
        CharPtr  pStatus;    /* Article processing status */
        CharPtr  pLanguage;  /* Language abbreviations */
        CharPtr  pPubType;   /* Publication type */
}
DocSum, PNTR DocSumPtr;

typedef enum PostType
{
	PostType_sort, 
	PostType_unsort,
	PostType_hier,
	PostType_text,
	PostType_other =255
}
PostType;

typedef struct EntrezFieldData 
{
	long  num_terms;      /* number of terms in this field */
	short num_bucket;     /* number of buckets of terms */
}
EntrezFieldData, *EntrezFieldDataPtr;
    
typedef struct EntrezTypeData 
{
	long   num;                   /* number of entries */
	long   num_uids;              /* number of uids */
	DocUid minuid;                /* minimum uid for this type */
	DocUid maxuid;                /* maxuid for this type */
	short  num_bucket;            /* number of uid buckets */
	EntrezFieldData *fields;
}
EntrezTypeData, *EntrezTypeDataPtr;

typedef struct EntrezTypeInfo
{
	short id;                 /* id number to pass to APIs         */
	char  tag[4];             /* 2-letter tag (for internal use)   */
	char *name;               /* short string to go on menu        */
	char *descr;              /* longer, descriptive string        */
	char *asntype;            /* ASN.1 type (e.g. "Medline-entry") */
}
EntrezTypeInfo;
    
typedef struct EntrezFieldInfo
{
	short id;                 /* id number to pass to APIs         */
	char  tag[7];             /* 4-letter tag (for internal use)   */
	char *name;               /* short string to go on menu        */
	char *descr;              /* longer, descriptive string        */
	unsigned single_token :1; /* all terms are single-token        */ 
	unsigned has_special  :1; /* terms may have special postings   */
	unsigned hier_avail   :1; /* a hierarchical counterpart exists */
	short hier_id;            /* field-id of hierarchy             */
	PostType post_type;       /* postings type                     */
}
EntrezFieldInfo;

typedef struct EntrezLinkInfo
{
        Int4 id;                    /* id number to pass to APIs (volatile) */
	CharPtr tag;                /* identifier string (for internal use) */
	CharPtr name;               /* short string to go on menu        */
	CharPtr descr;              /* longer, descriptive string        */
	Int4 dbfrom;                /* database link points from */
	Int4 dbto;                  /* database link points to */
	Int4 datasize;              /* size of link-associated data (bytes) */
	Int4 reciprocal;            /* id number of reciprocal link(volatile)*/
}
EntrezLinkInfo;

typedef struct EntrezDivInfo
{
	char  tag[4];             /* 3-letter tag (for internal use)   */
	char *descr;              /* Division description              */
	char *reldate;            /* Release number and/or date string */
	CdDate date;              /* Structured release date           */
	long *docs;               /* Document counts for each type     */
}
EntrezDivInfo;

typedef struct EntrezInfo 
{
	char   *volume_label;        /* name */
	char   *descr;               /* description */
	short   version;             /* major release number */
	short   issue;               /* minor release number */
	short   format;              /* (for internal use) */
	CdDate  release_date;        /* Entrez release date */
	CdDate  close_date;          /* close-of-date date for Entrez release */
	Boolean no_compression;      /* **DEFUNCT** */
	short   huff_count;          /* **DEFUNCT** */
	short  *huff_left;           /* **DEFUNCT** */
	short  *huff_right;          /* **DEFUNCT** */
	short   type_count;          /* number of document types */
	EntrezTypeData *types;
	EntrezTypeInfo *type_info;
	char  **type_names;          /* **DEFUNCT**  Use type_info[n]->name instead */
	short   type_bucket_size;
	short   field_count;
	EntrezFieldInfo *field_info;
	char  **field_names;         /* **DEFUNCT**  Use field_info[n]->name instead */
	short   field_bucket_size;
	short   div_count;           /* number of divisions */
	EntrezDivInfo *div_info;     /* division-specific info */
	int  cd_count;               /* how many CD-ROMs in distribution?  zero for net */
	Int4 link_count;
	EntrezLinkInfo *link_info;
}
EntrezInfo, *EntrezInfoPtr;

NLM_EXTERN Boolean LIBCALL CdRomAsnLoad PROTO((void));
NLM_EXTERN EntrezInfoPtr LIBCALL EntrezInfoFree PROTO((EntrezInfo *info));
NLM_EXTERN EntrezInfoPtr LIBCALL EntrezInfoAsnRead PROTO((AsnIo *aip, AsnType *orig));
NLM_EXTERN Boolean LIBCALL EntrezInfoAsnWrite PROTO((EntrezInfo *info, AsnIo *aip, AsnType *orig));


NLM_EXTERN DocSumPtr LIBCALL DocSumNew PROTO (( void ));
NLM_EXTERN DocSumPtr LIBCALL DocSumAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL DocSumAsnWrite PROTO (( DocSumPtr , AsnIoPtr, AsnTypePtr));

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


