/* cdnewlib.h
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
* RCS $Id: cdnewlib.h,v 6.0 1997/08/25 18:13:05 madden Exp $
*
* Author:  Gregory Schuler
*
* Version Creation Date: 08-22-94
*
* File Description:  Entrez low-level interface to CD-ROMs
*
* Modifications:  
* --------------------------------------------------------------------------
* Rev   Date      Name      Description of modification
* ----  --------  --------  ------------------------------------------------
* 1.1   08-22-94  Schuler   Initial revision
* 1.2   08-25-94  Schuler   Defined some more error codes
* 1.3   08-31-94  Schuler   Defined one more error code
* 1.4   09-01-94  Schuler   Added defines for directory names
* 1.5   11-22-94  Schuler   Changed arg in cd3_CdLinkUidGet fom long to DocUid
* 1.6   11-22-94  Schuler   Added rcs Id directive
* 1.7   11-22-94  Schuler   A few more tweaks for NT compiler warnings
* 1.8   11-20-94  Schuler   Added CdMountEntrezVolume and CdUnmountEntrezVolume
* 1.9   05-16-94  Schuler   Added RCS Log directive.  Comments will henceforth
*                           be inserted automatically into the source at the
*                           time it is checked into the system.

$Log: cdnewlib.h,v $
Revision 6.0  1997/08/25 18:13:05  madden
Revision changed to 6.0

Revision 5.1  1997/06/26 21:55:42  vakatov
[PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"

Revision 5.0  1996/05/28 13:55:34  ostell
Set to revision 5.0

 * Revision 4.1  1995/08/24  20:44:27  epstein
 * add more stuff for genomes
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 1.9  1995/05/16  14:12:46  schuler
 * Automatic comment insertion enabled
 *



* ==========================================================================
*/



#ifndef __cd3_CdEntrez__
#define __cd3_CdEntrez__

#ifdef _NEW_CdEntrez_


/******* ERROR CODES ******/
#define ERR_NotDefined  0    /* Some error for which I have not yet defined a code... */
#define ERR_DeepDooDoo  1    /* Catastrophic error -- unknown cause */
#define ERR_NotInited   2    /* The library has not beed initialized */
#define ERR_ConfigFile  3    /* Something wrong with configuration settings in .ncbirc */
#define    SUB_DevCount    1    /* [CdEntrez].DeviceCount zero or missing */
#define ERR_BadParam    4    /* Invalid parameter passed to library function */
#define    SUB_DocType     1    /* Invalid DocType value */
#define    SUB_DocField    2    /* Invalid DocField value */
#define ERR_BadFile     5    /* Something wrong with some Entrez file */
#define    SUB_BadVersion  1    /* Version number mismatch */
#define    SUB_Corrupt     2    /* File is corrupt or not recognized */
#define    SUB_NeedUpdate  3    /* File is from a future version -- need software upgrade */



/*
Note about DocTypes:  None of the functions in this module 
recognize TYP_SEQ as valid document type; you must use 
either TYP_AA, TYP_NT, or TYP_CH.
*/


/******* FUNCTIONS ******/

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

NLM_EXTERN Boolean cd3_CdInit (void);
NLM_EXTERN Boolean cd3_CdFini (void);
NLM_EXTERN AsnIo*  cd3_EntrezInfoOpen (const char *dirname);
NLM_EXTERN EntrezInfo*  cd3_CdGetInfo (void);
NLM_EXTERN char* cd3_CdDetailedInfo (void);
NLM_EXTERN int   cd3_CdTrmPageCt (DocType type, DocField field);
NLM_EXTERN int   cd3_CdTrmLookup (DocType type, DocField field, const char *term);
NLM_EXTERN int   cd3_CdLinkUidGet (LinkSetPtr *result, DocType type,
			DocType link_to_type, int numuid, DocUid *uids, 
			Boolean mark_missing, long maxlink);
NLM_EXTERN long  cd3_CdTrmUidsFil (DocType type, DocField field, long offset,
                                   long count, const char *filename, Boolean append);
NLM_EXTERN long  cd3_CdTrmUidsMem (DocType type, DocField field, long offset,
                                   long count, DocUid *mem);
NLM_EXTERN CdTerm* cd3_CdTrmFind (DocType type, DocField field, const char *term);
NLM_EXTERN AsnIo* cd3_CdDocAsnOpen (DocType type, DocUid uid);
NLM_EXTERN AsnIo* cd3_CdDocAsnClose (AsnIo* aio);
NLM_EXTERN int   cd3_CdTermScan (DocType type, DocField field, int page, int numpage, 
			CdTermProc proc);
NLM_EXTERN DocSum* cd3_CdGetDocSum (DocType type, DocUid uid);


typedef struct CdRomInfo
{
	short rel_major;     /* e.g. 13 for release 13.0     */
	short rel_minor;     /* e.g.  0 for release 13.0     */
	short cd_num;        /* CD-ROM number: 1, 2, or 3    */
	short cd_count;      /* Number of CDs in this set    */
	unsigned upper_case    :1; /* (primarily for internal use) */
	unsigned semicolon_one :1; /* (primarily for internal use) */
}
CdRomInfo;

NLM_EXTERN Boolean LIBCALL cd3_CdTestPath (const char *path, CdRomInfo *info);


typedef enum CdEntrezDir 
	{ CdDir_sys, CdDir_idx, CdDir_sum, CdDir_trm, CdDir_lnk, CdDir_rec }
CdEntrezDir;

#define CdDir_FIRST   CdDir_sys
#define CdDir_LAST    CdDir_rec

typedef Boolean (LIBCALLBACK *EntrezEnumFileProc)(int cdnum, const char *fdir,
		const char *fname, long fsize, void *opaque_data);

NLM_EXTERN long LIBCALL cd3_CdEnumFiles (CdEntrezDir dir, DocType type, const char *div, EntrezEnumFileProc proc, void *opaque_data);
			

NLM_EXTERN Boolean LIBCALL CdMountEntrezVolume (int cdnum, char *root, size_t buflen);
NLM_EXTERN Boolean LIBCALL CdUnmountEntrezVolume (int cdnum);

typedef struct CdDevInfo
{
	char *root;
	char *formal_name;
	char *device_name;
	char *raw_device_name;
	char *mount_point;
	char *mount_cmd;
}
CdDevInfo;

typedef int (LIBCALLBACK *CdDevHook)(const char *volname, const CdDevInfo *info);
NLM_EXTERN CdDevHook LIBCALL CdSetInsertHook (CdDevHook hook);
NLM_EXTERN CdDevHook LIBCALL CdSetEjectHook (CdDevHook hook);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif


#define SYS_KEYNAME "SYS"
#define IDX_KEYNAME "IDX"
#define SUM_KEYNAME "SUM"
#define TRM_KEYNAME "TRM"
#define LNK_KEYNAME "LNK"
#define REC_KEYNAME "REC"

#define SYS_DIRNAME "sysinfo"
#define IDX_DIRNAME "index"
#define SUM_DIRNAME "summary"
#define TRM_DIRNAME "terms"
#define LNK_DIRNAME "links"
#define REC_DIRNAME "asndata"



/******** ALIASES ********/
#define CdTestPath   cd3_CdTestPath
#define CdEnumFiles  cd3_CdEnumFiles
#define CdGetDocSum  cd3_CdGetDocSum

#endif
#endif
