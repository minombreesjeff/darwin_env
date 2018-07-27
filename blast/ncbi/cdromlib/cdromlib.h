/* cdromlib.h
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
* RCS $Id: cdromlib.h,v 6.0 1997/08/25 18:13:13 madden Exp $
*
* Author:  Greg Schuler, Jim Ostell, Jonathan Epstein
*
* Version Creation Date: 9/23/92
*
* File Description:
	header for cdrom specific functions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* July'94  Schuler     Multiple changes to support new CD-ROM formats
* 11-25-94 Schuler     Moved CdTerm structure from here to accentr.h
* 05-16-94 Schuler     Added RCS Log directive.  Comments will henceforth
*                      be inserted automatically into the source at the
*                      time it is checked into the system.

$Log: cdromlib.h,v $
Revision 6.0  1997/08/25 18:13:13  madden
Revision changed to 6.0

Revision 5.1  1997/06/26 21:55:44  vakatov
[PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"

Revision 5.0  1996/05/28 13:55:34  ostell
Set to revision 5.0

 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 2.11  1995/05/16  14:12:46  schuler
 * Automatic comment insertion enabled
 *



* ==========================================================================
*/

#ifndef _CDROMLIB_
#define _CDROMLIB_

#ifndef _NCBI_Access_
#include <objacces.h>
#endif

#ifndef _CDENTREZ_
#include <cdentrez.h>
#endif

#ifndef _CDCONFIG_
#include <cdconfig.h>
#endif


/* --- Constants --- */

/* error codes */
#define ERR_CD_MEMORY       1     /*  Memory allocation error   */
#define ERR_CD_FILECREATE   2     /*  File create error         */
#define ERR_CD_FILEOPEN     3     /*  File open error           */
#define ERR_CD_FILESEEK     4     /*  File seek error           */
#define ERR_CD_FILEREAD     5     /*  File read error           */
#define ERR_CD_FILEWRITE    6     /*  File write error          */
#define ERR_CD_BADTYPE      7     /*  Bad doc type code         */
#define ERR_CD_BADFIELD     8     /*  Bad field code            */
#define ERR_CD_NOTERMS      9     /*  No terms for type/field   */
#define ERR_CD_BADUID      10     /*  Bad unique identifier     */
#define ERR_CD_BADDIR      11     /*  Bad directory number      */
#define ERR_CD_BADFORMAT   12     /*  Format incompatible       */
#define ERR_CD_BADINDEX    13     /*  Index out of date         */
#define ERR_CD_BADDECOMP   14     /*  Decompression error       */
#define ERR_CD_PROGRAMMER  15     /*  Programmer error          */
#define ERR_CD_NO_DATASRC  16     /*  No data source (cdconfig) */
#define ERR_CD_LEX         17     /*  Lexical evaluation error  */
#define ERR_CD_BOOL        18     /*  Boolean operation error   */

/* directory codes for CdSetPath */
#define NDIR        8
#define DIR_ROOT    0
#define DIR_DAT     1
#define DIR_SEQ     2
#define DIR_MED     3
#define DIR_TRM     4
#define DIR_IDX     5
#define DIR_LNK     6
#define DIR_VAL     7
#define DIR_SUM     8

/* sizes & limits */
#define VLABEL_MAX  16
#define TERM_MAX    80
#define CAPT_MAX    20 
#define TITLE_MAX  180
#define LNK_MAX      4

/* magic number used for some CD-ROM files */
#define CD_MAGIC_BASE 0x4e434249  /* "NCBI" */

typedef struct cdmediainfo {
	CharPtr sPath[NDIR];
	Boolean ejectable;
	CharPtr device_name;
	CharPtr raw_device_name;
	CharPtr mount_point;
	CharPtr mount_cmd;
	Boolean bAppendVer;
	Boolean upperCaseIt;
	Boolean hold_idx_open;
} CdMediaInfo, PNTR CdMediaInfoPtr;

typedef Boolean ( *CdTermProc) PROTO((CdTermPtr trmp));

#ifdef _NEW_CdEntrez_
#include <cdnewlib.h>
#endif

/* --- Function Prototypes --- */

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

AsnIoPtr  EntrezInfoOpen PROTO((CharPtr dirname));

Boolean  CdInit	      PROTO((void));
Boolean  CdFini       PROTO((void));

EntrezInfoPtr CdGetInfo PROTO((void));
CharPtr CdDetailedInfo PROTO((void));

Int2  CdTrmPageCt  PROTO((DocType type, DocField field));
Int2  CdTrmLookup  PROTO((DocType type, DocField field, CharPtr term));
Int2  CdTrmPages   PROTO((DocType type, DocField field, Int2 pg));
Int2  CdLinkUidGet PROTO((LinkSetPtr PNTR result, DocType type,
DocType link_to_type, Int2 numuid, Int4Ptr uids, Boolean mark_missing,
Int4 maxlink));

Int4  CdTrmUidsFil PROTO((DocType type, DocField field, Int4 offset,
								Int4 count, CharPtr filename, Boolean append));
Int4  CdTrmUidsMem PROTO((DocType type, DocField field, Int4 offset,
								Int4 count, DocUidPtr mem));
CdTermPtr  CdTrmFind PROTO((DocType type, DocField field, CharPtr term));

AsnIoPtr  CdDocAsnOpen  PROTO((DocType type, DocUid uid));
AsnIoPtr  CdDocAsnClose PROTO((AsnIoPtr aip));

Int2  CdTermScan PROTO((DocType type, DocField field, Int2 page, Int2 numpage, CdTermProc proc));
NLM_EXTERN CdTermPtr  CdTermFree PROTO((CdTermPtr trmp));

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
