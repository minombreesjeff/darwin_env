/* cdconfig.h
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
* RCS $Id: cdconfig.h,v 6.0 1997/08/25 18:12:48 madden Exp $
*
* Author:  Jonathan Epstein
*
* Version Creation Date: 11/24/92
*
* File Description:
	header for cdrom-library complex configuration mechanism
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 05-16-94 Schuler     Added RCS Log directive.  Comments will henceforth
*                      be inserted automatically into the source at the
*                      time it is checked into the system.

$Log: cdconfig.h,v $
Revision 6.0  1997/08/25 18:12:48  madden
Revision changed to 6.0

Revision 5.1  1997/06/26 21:55:27  vakatov
[PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"

Revision 5.0  1996/05/28 13:55:34  ostell
Set to revision 5.0

 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 1.7  1995/05/16  14:12:46  schuler
 * Automatic comment insertion enabled
 *



*
* ==========================================================================
*/

#ifndef _CDCONFIG_

#define _CDCONFIG_ 1

#define ENTR_REF_CHAN        "ENTR_REF"
#define ENTR_SEQ_CHAN        "ENTR_SEQ"
#define ENTR_LINKS_CHAN      "ENTR_LINK"

typedef Boolean (* ConfCtlProc) PROTO((VoidPtr parm));

typedef struct Media {
	CharPtr media_alias; /* name in config file */
	Int2    media_type;  /* CD or NET or ... */
#define MEDIUM_UNKNOWN  0
#define MEDIUM_CD       1
#define MEDIUM_NETWORK  2
#define MEDIUM_DISK     4
	Boolean inited_partial; /* e.g., initialization which can be performed */
							/* without actually inserting the CD-ROM       */
	Boolean invalid;
	Boolean inited_total;
	Boolean in_use;
	Boolean is_live;     /* Is the device "live", e.g., a CD-ROM which is  */
	                     /* assumed to be currently inserted?              */
	CharPtr formal_name; /* Formal name to be used when talking to user */
	EntrezInfoPtr entrez_info;
    ConfCtlProc swapOutMedia;
    ConfCtlProc swapInMedia;
    ConfCtlProc finiMedia;
	VoidPtr media_info;  /* media-specific information */
	struct Media PNTR next;
	struct Media PNTR next_lru; /* doubly-linked pointers in LRU chain */
	struct Media PNTR prev_lru;
	Boolean LRUineligible;      /* media ineligible for Least Recently Used */
} Media, PNTR MediaPtr;

typedef struct DrasticActions {
	MediaPtr from_media;
	MediaPtr to_media;
	Boolean  is_drastic;
	struct DrasticActions PNTR next; /* next in linked list */
} DrasticActions, PNTR DrasticActionsPtr;

typedef struct DataSourceDbKey {
    CharPtr section;
    CharPtr field1;
    CharPtr field2;
} DataSourceDbKey;

typedef struct DataSource {
	Int2            priority;
	Boolean         no_drastic_action;
	Boolean         already_searched; /* reset upon each call to SelectDataSource */
	MediaPtr        media;
	CharPtr         channel; /* name in config file */
	struct DataSource PNTR next;
} DataSource, PNTR DataSourcePtr;

typedef struct DataSourceDb {
	DataSourceDbKey key;
	DataSourcePtr   list;
	struct DataSourceDb PNTR next;
} DataSourceDb, PNTR DataSourceDbPtr;


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

NLM_EXTERN void    SetSoleMedia               PROTO((void));
NLM_EXTERN Boolean SelectDataSource           PROTO((CharPtr section,
											  CharPtr field1, CharPtr field2));
NLM_EXTERN Boolean SelectDataSourceByType     PROTO((DocType type,
											  CharPtr field1, CharPtr field2));
NLM_EXTERN Boolean SelectDataLinksByTypes     PROTO((DocType type,
											  DocType link_to_type));
NLM_EXTERN Boolean SelectNextDataSource       PROTO((void));
NLM_EXTERN void ConfigInit                    PROTO((void));
NLM_EXTERN void ConfigFini                    PROTO((void));
NLM_EXTERN MediaPtr GetCurMedia               PROTO((void));
NLM_EXTERN Int2     CurMediaType              PROTO((void));
NLM_EXTERN MediaPtr SetCurMedia               PROTO((MediaPtr NewMedia));
NLM_EXTERN MediaPtr PreInitMedia              PROTO((CharPtr buf));
NLM_EXTERN Boolean  CdIsInserted              PROTO((MediaPtr media));
NLM_EXTERN Int2     ParseMedia                PROTO((ConfCtlProc initfunc, Int2 media_mask));
NLM_EXTERN EntrezInfoPtr EntrezInfoMerge      PROTO((void));

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _CDCONFIG_ */
