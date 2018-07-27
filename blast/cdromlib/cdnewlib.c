/* cdnewlib.c
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
 * RCS $Id: cdnewlib.c,v 6.2 1999/03/12 18:44:57 kans Exp $
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
 * 1.2   08-22-94  Schuler   Rearranged the #include directives
 * 1.3   08-25-94  Schuler   cd3_CdGetDocSum: fix for new *.sum format.
 * 1.3   08-25-94  Schuler   Added error codes to ErrPostEx in many places.
 * 1.4   08-25-94  Kans      Some typecasts to make compilers happy
 * 1.5   08-26-94  Schuler   Some typecasts to make compilers happy
 * 1.6   08-28-94  Ostell    Convert double-slash comment to slash-star
 * 1.7   08-31-94  Schuler   Fixed bug in CdTrmLookup
 * 1.8   08-31-94  Schuler   Added magic number/version number checking
 * 1.9   09-01-94  Schuler   Use defines for directory names throughout
 * 1.10  09-01-94  Kans      Fixed bug in CdEntrez_FileOpen
 * 1.11  09-02-94  Schuler   Fixed bug in TrmIndex_Destruct
 * 1.12  09-02-94  Schuler   Implemented the CdMap_Destruct function 
 * 1.13  09-05-94  Schuler   Implemented the ReadCdLayout function; Fixed bug
 *                           in CdEnumFiles; Fixed bug in CdEntrez_FileOpen
 * 1.14  09-05-94  Schuler   Fixed bug in CdTrmLookup
 * 1.15  09-05-94  Schuler   Fixed bug in CdTrmLookup (tricky!)
 * 1.16  09-06-94  Schuler   Added some more VERBOSEs and ASSERTs
 * 1.17  09-09-94  Schuler   Fixed bug in CD swapping logic
 * 1.18  09-21-94  Schuler   Fixed bug in CdFini (NULL out _huff_tab)
 * 1.19  09-22-94  Schuler   Another fix to swapping logic...
 * 1.20  10-04-94  Schuler   Fixed invalid redeclaration of SwapInt4
 * 1.21	10-08-94  ?         ?
 * 1.22  10-08-94  Kans      Eliminate prompt to insert CD when already there
 * 1.23  10-10-94  Schuler   Fixed bug in CdEnumFiles (wanted Entrez3 always)
 * 1.24  10-24-94  Schuler   Fixed CdDevice_FileOpen for to respect the
 *                           upper_case and semicolon_one flags.
 * 1.25  10-25-94  Schuler   Change to CdVolume_Mount to re-init settings
 *                           of upper_case and semicolon_one
 * 1.26  10-26-94  Schuler   Changed "long" to "Int4" in a couple places
 * 1.27  11-17-94  Schuler   Fixed bug in ReadCdLayout 
 * 1.28  11-17-94  Schuler   Fixed bug in FindFloatingEntrezVolumes
 * 1.29  11-17-94  Schuler   Scan for already-mounted Entrez CDs on
 *                           Mac desktop at startup
 * 1.30  11-17-94  Schuler   Fixed CdTestPath to properly handle cases where
 *                           CD-ROM files are upper case and case matters
 * 1.31  11-18-94  Schuler   Added verbose trace message logging
 * 1.32  11-19-94  Schuler   Plugged some memory leaks
 * 1.33  11-21-94  Schuler   Fixed FileReadSwap4 to support 8-byte longs
 * 1.34  11-22-94  Schuler   Changed arg in cd3_CdLinkUidGet fom long to DocUid
 * 1.35  11-22-94  Schuler   Fixed invalid redeclaration of SwapInt4
 * 1.36  11-22-94  Schuler   Changed "long" to "DocUid" in CdLinkUidGet
 * 1.37  11-25-94  Schuler   Fixed bug in UidIndex_ReadPage (re: 8-byte longs)
 * 1.38  11-30-94  Schuler   Populate new cd_count field EntrezInfo
 * 1.39  01-04-95  Kans      ?
 * 1.40  01-20-95  Schuler   Added the CdMountEntrezVolume() and 
 *                           CdUnmountEntrezVolume() functions.
 * 1.41  02-27-95  Schuler   CdVolume_Mount() calls CdDevice_Init() if needed
 * 1.42  02-28-95  Schuler   Added more verbose log messages
 * 1.43  02-28-95  Schuler   Plugged some memory leaks
 * 1.44  03-02-95  Schuler   Changed fopen/fclose to FileOpen/FileClose
 *
 * 05-19-95 Schuler     Added rcs Log directive for automatic insertion of
 *                      modification comments.
 *
 * Revision $Log: cdnewlib.c,v $
 * Revision Revision 6.2  1999/03/12 18:44:57  kans
 * Revision fixed ErrPostEx problem
 * Revision
 * Revision Revision 6.1  1998/08/24 18:42:16  kans
 * Revision fixed -v -fd warnings
 * Revision
 * Revision Revision 6.0  1997/08/25 18:13:01  madden
 * Revision Revision changed to 6.0
 * Revision
 * Revision Revision 5.3  1997/06/26 21:55:38  vakatov
 * Revision [PC] DLL'd "ncbicdr.lib", "ncbiacc.lib", "ncbinacc.lib" and "ncbicacc.lib"
 * Revision
 * Revision Revision 5.2  1997/02/14 22:34:49  epstein
 * Revision allocated more memory for detailed info
 * Revision
 * Revision 5.1  1996/11/19  21:46:45  shavirin
 * CdInit() CdFini() messages made optional (printed in
 * verbose mode)
 *
 * Revision 5.0  1996/05/28  13:55:34  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/04/02  19:02:49  epstein
 * change CDVOL_MAX and its use
 *
 * Revision 4.1  1996/03/26  16:29:11  epstein
 * migrate byte-swapping functions to ncbimisc.[ch]
 *
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 1.48  1995/06/05  21:32:56  kans
 * CdEnumFiles now uses safe fgets version (UnixFileGets)
 *
 * Revision 1.47  1995/05/18  17:27:46  kans
 * changed qsort to HeapSort to avoid corruption/hang on one platform
 *
 * Revision 1.46  1995/05/16  14:36:20  schuler
 * Automatic comment insertion enabled
 *
 *
 * ==========================================================================
 */

#define REVISION_STR "$Revision: 6.2 $"



/*
CONFIGURATION NOTES

[CdEntrez]
	
DeviceCount = (number)
	        This should be equal to the number of Entrez CD-ROM images
	        that may be simultaneosuly on-line, either as mounted CD-ROMs
	        or as hard disk images.  For example, suppose that you have a
	        single CD-ROM drive on which you keep one of the Entrez CD-ROMs
	        mounted plus a huge hard disk onto which you have copied the 
	        contents of the remaining two CD-ROMs.  In this case, the 
	        DeviceCount setting should be 3.
	        
IdxCacheSize = (kbytes)
TrmCacheSize = (kbytes)
	        Maximum amount of memory to use for caching index & term files
	        
LogVerbose = { Yes / No }
            Causes verbose logging of status information for use in debugging
            (default is No).  Note that error logging must be enabled by the
            application for anything to get logged.
            

[CdEntrez.device.#]            (one section for each device, # = 1,2,3..)
	
Type = { CD / HARDDISK / NET }
	        Type of device: CD-ROM drive, local hard drive, or network drive.
	        
Formal_Name = (some string)
			This setting is optional, but provides a name for the device (e.g.
			"Second CD-ROM drive") that can be used in user messages.

Root = (full path)
	        Full path corresponding to the root of the CD-ROM (excluding
	        the volume name when used with Insert_Volname=Yes; see below).
	        On the Macintosh, this should be omitted or left empty for
	        EJECTABLE devices with Insert_Volname=Yes.
	        
Insert_Volname = { Yes / No }
	        If Yes, the name of the volume will be inserted after the
	        string specified in the Root setting and before the names of
	        subdirectories on the CD.  This would be used for EJECTABLE
	        devices on Macintosh and Solaris systems (on the Mac, the
	        Root setting should be emtpy in this case as the path starts
	        with the volume name).
	        
Ejectable = { Yes / No }
	        Set to yes if the device should be considered ejectable.
	        
Bind = { Entrez1 / Entrez2 / Entrez 3 }
	        Use this setting to bind an Entrez CD-ROM to a device.  The
	        setting should be the volume name of the CD: "Entrez1", "Entrez2",
	        etc. (not sensitive to case).  Bound volumes are never ejected,
	        even if Ejectable=Yes. 

Device_Name =
Raw_Device_Name =
Mount_Point
Mount_Cmd =
            These are some parameters to be passed to the MountCd and
            EjectCd functions.  They may be required on certain UNIX
            and VMS systems.
            
*/

#include <cdromlib.h>

#ifdef _NEW_CdEntrez_


static char _this_module[] = "CdEntrez";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char _this_file[] = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

#undef VERBOSE
#define VERBOSE   if(_verbose_trace) Nlm_ErrLogPrintf

#define CDVOL_MAX  8
#define TYPE_MAX   DocType_MAX
#define DIV_MAX    64
#define FLD_MAX    32

#define TYPE_TAG_LEN  2
#define DIV_TAG_LEN   3
#define FLD_TAG_LEN   4

#define DEFAULT_IdxCacheSize  16
#define DEFAULT_SumCacheSize  16
#define DEFAULT_TrmCacheSize   8


#define VOLNUM_IS_VALID(x) ((x)<=_volume_ct && (x)>0)
#define TYPE_IS_VALID(x)   ((x)<_type_ct && (x)>=0)
#define FIELD_IS_VALID(x)  ((x)<_fld_ct && (x)>=0)

#define TYPTAG(x)  _cdinfo->type_info[x].tag
#define FLDTAG(x)  _cdinfo->field_info[x].tag
#define DIVTAG(x)  _cdinfo->div_info[x].tag

#define HCA 1
#define SUM 2
#define LNK 3
#define OFS 4
#define OIX 5
#define TRM 6
#define TIX 7
#define PST 8
#define INF 9

#define MAGIC_hca 0x11BD
#define MAGIC_sum 0x0025
#define MAGIC_lnk 0x0A02
#define MAGIC_ofs 0x0A03
#define MAGIC_oix 0x0A04
#define MAGIC_trm 0x0A05
#define MAGIC_tix 0x0A06
#define MAGIC_pst 0x0A07
#define MAGIC_inf 0x0A0A

#define FORMAT_hca 2
#define FORMAT_sum 2
#define FORMAT_ofs 2
#define FORMAT_oix 2
#define FORMAT_trm 2
#define FORMAT_tix 2
#define FORMAT_pst 2
#define FORMAT_lnk 2
#define FORMAT_inf 1

/* ========== CdDevice class ========== */
struct CdVolume;

typedef struct CdDevice 
{
	CdDevInfo inf;               /* device characteristics  */
	int   bound_cd;              /* Entrez volume number bound to this device, if any */
	int   hint;                  /* used during initialization for transient bind */
	struct CdVolume *volume;     /* Entrez volume currently mounted on this device */
	unsigned is_cdrom      :1;   /* TRUE if device is a CD-ROM reader */
	unsigned is_ejectable  :1;   /* TRUE if device should be considered ejectable */
	unsigned ins_volname   :1;   /* TRUE if the volume name gets inserted into path */
	unsigned semicolon_one :1;   /* TRUE if the version number ";1" should be appended */
	unsigned upper_case    :1;   /* TRUE if the file name should be upper case */
	unsigned is_inited     :1;   /* TRUE if semicolon_one and upper_case are known */
	time_t timestamp;            /* time this device was last used           */
}
CdDevice;

#define CdDevice_New(a)  CdDevice_Construct(MemNew(sizeof(CdDevice)),a)	
#define CdDevice_Free(a) MemFree((void*)CdDevice_Destruct(a))
static CdDevice* CdDevice_Construct (CdDevice *cddev, int num);
static CdDevice* CdDevice_Destruct (CdDevice *cddev);
static int   CdDevice_Init (CdDevice *cddev);
static int CdDevice_FileBuildPath (CdDevice *cddev, char *fpath, const char *fdir, const char *fname);
static FILE* CdDevice_FileOpen (CdDevice *cddev, const char *fdir, const char *fname, const char *fmode);
static void  CdDevice_Touch (CdDevice *cddev);
static CdDevice* GetLruDevice (void);

/* ==========  CdVolume class ========== */

typedef struct CdVolume
{
	char *volume_name;   /* "entrez1", "entrez2", or "entrez3"       */
	short volume_num;    /* 1, 2, or 3                               */
	CdDevice *device;    /* on which device is this volume mounted ? */
	time_t timestamp;    /* time this volume was last used           */
}
CdVolume;

#define CdVolume_New(a)  CdVolume_Construct(MemNew(sizeof(CdVolume)),a)	
#define CdVolume_Free(a) MemFree((void*)CdVolume_Destruct(a))
static CdVolume* CdVolume_Construct (CdVolume *cdvol, int number);
static CdVolume* CdVolume_Destruct (CdVolume *cdvol);
static CdDevice* CdVolume_Mount (CdVolume *cdvol, CdDevice *cddev, int verify);
static int  CdVolume_Unmount (CdVolume *cdvol);
static int  CdVolume_IsMounted (CdVolume *cdvol);
static int FindFloatingEntrezVolumes (int *outlist);


/* ==========  UidIndex class ========= */

typedef struct UidIndex 
{
	DocType  type;     /* Document type: TYP_ML, TYP_AA, etc */
	long  uid_ct;      /* Number of UIDs in index (can be > number of docs) */
	long  uid_min;     /* Smallest UID in index */
	long  uid_max;     /* Largest UID in index */
	int   pages;       /* number of index pages */
	long *index;       /* array of 1st UID on each page */
}
UidIndex;

typedef struct UidIndexRec
{
	long uid;          /* UID of document */
	int  divnum;       /* division number (lookup in EntrezInfo) */
	int  filnum;       /* hca file segment number (from 1) */
	long hca_offset;   /* offset into hca file for full ASN.1 record */
	long sum_offset;   /* offset into sum file for summary */
	long lnk_offset;   /* offset into lnk file for links */
}
UidIndexRec;

#define UidIndex_New(a,b) UidIndex_Construct(MemNew(sizeof(UidIndex)),a,b)	
#define UidIndex_Free(a)  MemFree((void*)UidIndex_Destruct(a))
static UidIndex* UidIndex_Construct (UidIndex *uidx, DocType type, EntrezTypeData *info);
static UidIndex* UidIndex_Destruct (UidIndex *uidx);
static int   UidIndex_ReadPageMap (UidIndex *uidx);
static long* UidIndex_ReadPage (UidIndex *uidx, int pagenum);
static int   UidIndex_Lookup (UidIndex *uidx, DocUid uid, UidIndexRec *rec);


/* ==========  TrmIndex class ========== */

typedef struct TrmIndex
{
	struct TrmIndex *next;   /* next element in linked list */
	char tag[6];        /* tag for field (e.g. "mesh") */
	unsigned type  :6;  /* ??? */
	unsigned fld   :6;  /* ??? */
	unsigned style :4;  /* ??? */
	long trm_ct;        /* term count */
	int  page_ct;       /* page count */
	int  page_size;     /* page size */
	char **index;       /* array of 1st term on each page */
}
TrmIndex;

#define TrmIndex_New(a,b,c,d) TrmIndex_Construct(MemNew(sizeof(TrmIndex)),a,b,c,d)	
#define TrmIndex_Free(a)  MemFree((void*)TrmIndex_Destruct(a))
static TrmIndex* TrmIndex_Construct (TrmIndex *tidx, int type, int fld, const char *tag, EntrezFieldData *info);
static TrmIndex* TrmIndex_Destruct (TrmIndex *tidx);
static int   TrmIndex_ReadPageMap (TrmIndex *tidx);
static Byte* TrmIndex_ReadPage (TrmIndex *tidx, int pagenum);
static int   TrmIndex_Lookup (TrmIndex *tidx, const char *stem);
static CdTerm* TrmIndex_GetCdTerm (TrmIndex *tidx, const char *term);
static int TrmIndex_ScanPages (TrmIndex *tidx, int start, int count, CdTermProc proc);
static FILE * TrmIndex_PostingsFileOpen (TrmIndex *tidx);

static CdTerm* getcdtrm (TrmIndex *tidx, int pagenum, Byte **pptr);
static int trmcmp (const char *term1, const char *term2);
static int trmncmp (const char *term1, const char *term2, int n);


/* ==========  CdMap class ========= */

typedef struct CdMap
{
	unsigned user_set :1;  /* any user-supplied path set ? */
	unsigned multicd  :1;  /* files exist on multiple CDs ? */
	unsigned lnklist  :1;  /* list is a linked list ? (array if false) */
	short array_size;      /* array size for list */
	short id_num;          /* id_number for divisions */
	short cd_num;          /* CD number */
	short *cd_list;        /* list of CD numbers, if multicd */
	char *user_path;       /* user-supplied path */
	struct CdMap *list;    /* linked list or array */
}
CdMap;

#define CdMap_New(a)   CdMap_Construct((CdMap*)MemNew(sizeof(CdMap)),a)
#define CdMap_Free(x)   MemFree((void*)CdMap_Destruct(x))
static CdMap* CdMap_Construct (CdMap *map, int list_size);
static CdMap* CdMap_Destruct (CdMap *map);
static void CdMap_ParseCdNums (CdMap *map, char *nums);
static void CdMap_FindPath (CdMap *map, const char *key);
static int CdMap_GetSpecs (CdMap *map, int *cdlist, char *fdir);
static CdMap* CdMap_GetChild (CdMap *map, int nkid);


static Boolean ReadCdLayout (FILE *fd);


/* ========== LSet  class ========== */
typedef struct DocLink
{
	DocUid uid;
	int wt;
}
DocLink;

typedef struct LSet 
{
	short sorc_type;
	short dest_type;
	int   link_max;
	int   count;
	int   slots;
	DocLink *link;
}
LSet;

#define LSet_New(a,b,c)  LSet_Construct(MemNew(sizeof(LSet)),a,b,c)	
#define LSet_Free(x)     MemFree((void*)LSet_Destruct(x))
static LSet* LSet_Construct (LSet *lset, short sorc_type, short dest_type, int link_max);
static LSet * LSet_Destruct (LSet *lset);
static int LSet_Read (LSet *lset, FILE *fd);
static LinkSet* LSet_Convert (LSet *lset);


/* ==========  HuffTable class ========== */

typedef struct HuffTable
{
	int count;
	short *left;
	short *right;
}
HuffTable;

#define HuffTable_New(a)   HuffTable_Construct(MemNew(sizeof(HuffTable)),a)
#define HuffTable_Free(a)  MemFree((void*)HuffTable_Destruct(a))
static HuffTable* HuffTable_Construct (HuffTable *huff, int slots);
static HuffTable* HuffTable_Destruct (HuffTable *huff);
static HuffTable* HuffTable_Read (FILE *fd);


/* ========== DecompInfo class ========== */
typedef struct DecompInfo
{
	AsnIo *aio;
	FILE *fd;
	HuffTable *huff;
	unsigned int byte;
	unsigned int mask;
}
DecompInfo;

#define DecompInfo_New(a) DecompInfo_Construct(MemNew(sizeof(DecompInfo)),a)
#define DecompInfo_Free(a)  MemFree((void*)DecompInfo_Destruct(a))
static DecompInfo* DecompInfo_Construct (DecompInfo *info, HuffTable *huff);
/*static DecompInfo* DecompInfo_Destruct (DecompInfo *info);*/
#define DecompInfo_Destruct(info)  (info)
static AsnIo* DecompInfo_Attach (DecompInfo *info, FILE *fd);
static FILE* DecompInfo_Detach (DecompInfo *info);

static Int2 LIBCALLBACK DecompReadProc (void *p, char *buff, Uint2 count);

/* ==========  Cache class ========== */
#define Cache_MAGIC_VALUE 223445L                 

typedef struct CachePage
{
	long id;
	int  lock;
	void *data;
}
CachePage;

typedef void (PASCAL *CacheDataFreeProc)(void *data);
static void PASCAL DefCacheDataFreeProc (void *data);

typedef struct Cache 
{
	long magic;
	CachePage *page;
	int  page_slots;
	int  page_count;
	long page_size;    /* optional */
	long hits;
	long misses;
	CacheDataFreeProc fproc;
}
Cache;

#define Cache_New(a,b) Cache_Construct(MemNew(sizeof(Cache)),a,b)	
#define Cache_Free(a)  MemFree((void*)Cache_Destruct(a))
static Cache* Cache_Construct (Cache *cache, int size, CacheDataFreeProc fproc);
static Cache* Cache_Destruct (Cache *cache);
static int   Cache_Insert (Cache *cache, long id, void *data);
static int   Cache_Delete (Cache *cache, long id);
static int   Cache_Touch (Cache *cache, long id);
static void* Cache_Lock (Cache *cache, long id);
static int   Cache_Unlock (Cache *cache, long id);
static void* Cache_Peek (Cache *cache, long id);
static void  Cache_Purge (Cache*);
static void  Cache_LogStats (Cache *cache, const char *name);
static char* Cache_ReportStats (Cache *cache, char *buffer);
static int   Cache_IsValid (Cache *cache);


/* ========== Misc. structs ========== */

struct CdFInfo
{
	char *ext;
	unsigned short magic;
	unsigned short format;
};

struct CdDirInfo
{
	char *key;
	char *dir;
	char *user_dir;
	FILE **fd;
};

typedef struct DivInfo
{
	char tag[1+DIV_TAG_LEN];
	short cd_num[TYPE_MAX];
}
DivInfo;

/* ========== Misc. functions ========== */

static FILE * CdEntrez_FileOpen (CdEntrezDir dirnum, int doctype, int divnum, int ftype, const char *fname, const char *fmode);
static void CdEntrez_FileClose (CdEntrezDir dirnum, int doctype, FILE *fd);

static int InvalidConfiguration (int code);
static int FileCorrupt (const char *fname);
static int FileOutOfDate (const char *fname);
static int FileNotRecognized (const char *fname);
static int CatastrophicFailure (int code);

static Boolean IsInitialized (void);
static Boolean ValidType (int type);
static Boolean ValidField (int field);

static char * _GetAppParamStr (const char *filebase, const char *section, 
				const char *key, const char *dlft);
				
#ifndef GetAppParamStr
#define GetAppParamStr _GetAppParamStr
#endif

static int LIBCALLBACK default_CdInsertProc (const char *volname, const CdDevInfo *dev);
static int LIBCALLBACK default_CdEjectProc (const char *volname, const CdDevInfo *dev);


/* ========== Static Variables ========== */

static CdDevHook _hookInsert = default_CdInsertProc;
static CdDevHook _hookEject = default_CdEjectProc;

static char * _empty_string = "";

static HuffTable * _huff_tab[TYPE_MAX];

static FILE* _fd_idx[TYPE_MAX];
static FILE* _fd_sum[TYPE_MAX];
static FILE* _fd_lnk[TYPE_MAX];

#define HCA 1
#define SUM 2
#define LNK 3
#define OFS 4
#define OIX 5
#define TRM 6
#define TIX 7
#define PST 8
#define INF 9

struct CdFInfo _finfo[] = {
	"", 0, 0,
	"hca", MAGIC_hca, FORMAT_hca,
	"sum", MAGIC_sum, FORMAT_sum,
	"lnk", MAGIC_lnk, FORMAT_lnk,
	"ofs", MAGIC_ofs, FORMAT_ofs,
	"oix", MAGIC_oix, FORMAT_oix,
	"trm", MAGIC_trm, FORMAT_trm,
	"tix", MAGIC_tix, FORMAT_tix,
	"pst", MAGIC_pst, FORMAT_pst,
	"inf", MAGIC_inf, FORMAT_inf
};


struct CdDirInfo _dir[] = {
	SYS_KEYNAME, SYS_DIRNAME, NULL, NULL,
	IDX_KEYNAME, IDX_DIRNAME, NULL, _fd_idx,
	SUM_KEYNAME, SUM_DIRNAME, NULL, _fd_sum,
	TRM_KEYNAME, TRM_DIRNAME, NULL, NULL,
	LNK_KEYNAME, LNK_DIRNAME, NULL, _fd_lnk,
	REC_KEYNAME, REC_DIRNAME, NULL, NULL    
};


static EntrezInfo * _cdinfo;
static short _init_ct;
static short _rel_major;
static short _rel_minor;
static short _device_ct;
static short _volume_ct;
static short _type_ct;
static short _div_ct;
static short _fld_ct;
static short _verbose_trace;

static CdVolume * _cdvol; 
static CdDevice * _cddev;

static DivInfo * _div;
static UidIndex * _uidx;
static TrmIndex ** _tidx[TYPE_MAX];

#define IDX_REC_SIZE   20

static int _idx_page_size;
static int _idx_page_slots;
static int _trm_page_size;

static Cache * _idx_cache;
static Cache * _trm_cache;

static char * _rcfile = "ncbi";

static CdMap _map[CdDir_LAST+1];



static int FileReadSwapShort (unsigned short *buffer, int count, FILE *fd);
static int FileReadSwapLong (unsigned long *buffer, int count, FILE *fd);
static int FileReadSwapInt4 (Uint4 *buffer, int count, FILE *fd);



static char * FileReadStr (FILE *fd, int lbyte);




/****************************************************************************
*
*		EXPORTED APIs
*
*****************************************************************************/

NLM_EXTERN Boolean  cd3_CdInit (void)
{
	int n, j, k, pages;
	char buffer[256];
	unsigned short m[8];
	CdRomInfo info;
	CdDevice *dev;
	AsnIo *aio;
	FILE *fd;
	CdDevice *dev_startup = NULL; 
	

	_verbose_trace = GetAppParamBoolean(_rcfile,"CdEntrez","LogVerbose",FALSE);
	if (_verbose_trace)
          ErrLogPrintf("CdInit()     [%s %s]\n", THIS_MODULE, REVISION_STR);

	/*----- If already initialized, just increment counter -----*/
	if (_init_ct > 0)
	{
		_init_ct++;
		return TRUE;
	}
	

	/***if (!_verbose_trace)
	{
		ErrLogPrintf("      Add \"LogVerbose=YES\" below [CdEntrez] in your NCBI\n");
		ErrLogPrintf("      configuration file to log detailed information to this file.\n");
	}***/

	/*----- Gather information from config file -----*/
	_device_ct = (int) GetAppParamInt(_rcfile,"CdEntrez","DeviceCount",0);
	if (_device_ct == 0)
			return InvalidConfiguration(1);
	
	for (n=CdDir_FIRST; n<=CdDir_LAST; ++n)
	{
		if (FindPath(_rcfile,"CdEntrez.Paths",_dir[n].key,buffer,sizeof buffer))
			_dir[n].user_dir = StrSave(buffer);
	}

	/*----- Initialize the list of devices -----*/
	_cddev = (CdDevice*) MemNew(sizeof(CdDevice) * _device_ct);
	for (n=0, dev=_cddev; n<_device_ct; ++n)
	{
		if (!CdDevice_Construct(dev++,n+1))
			return FALSE;
	}
	
	/*----- Now we need locate a copy of cdvolume.inf -----*/
	memset((void*)&info,0,sizeof info);
	buffer[0] = '\0';
	if (_dir[CdDir_sys].user_dir !=NULL)
	{
		/* use the cdvolume.inf file on the hard disk */
		strcpy(buffer,_dir[CdDir_sys].user_dir);
		FileBuildPath(buffer,NULL,"cdvolume.inf");
	}
	else
	{
		/* first, look for a bound device */
		for (k=0, dev=_cddev; k<_device_ct; ++k, ++dev)
		{
			if (_cddev[k].bound_cd && CdDevice_Init(dev))
			{
				dev_startup = dev;
				CdDevice_FileBuildPath(dev,buffer,_dir[CdDir_sys].dir,"cdvolume.inf");
				break;
			}
		}		

		if (dev_startup == NULL)
		{		
			/* if we reach this point, there are no bound devices, so
				we now have to look through all of the devices, hoping
				that one of them will have an Entrez CD-ROM already
				inserted */

/*
#ifndef OS_DOS
*/
			for (k=0, dev=_cddev; k<_device_ct; ++k, ++dev)
			{
				if (CdDevice_Init(dev))
				{
					dev_startup = dev;
					break;
				}
			}
/*
#endif
*/
			
			if (dev_startup == NULL)
			{   
				/* Now were're going to have to ask the user
					to insert one of the CD's */
				
				dev_startup = &_cddev[0];
					
				if (!(*_hookInsert)("Entrez1",&dev_startup->inf))
					return FALSE;
				if (!CdDevice_Init(dev_startup))
				{
					VERBOSE("CdDevice_Init() failed, line %d\n",__LINE__);
					return FALSE;
				}
			}
			
			CdDevice_FileBuildPath(dev_startup,buffer,_dir[CdDir_sys].dir,"cdvolume.inf");
		}
	}

	/*----- OK, now we've found cdvolume.inf, let's read it -----*/
	if ((fd = FileOpen(buffer,"rb")) ==NULL)
		return FALSE;
	FileReadSwapShort(m,6,fd); 
	if (m[0] != MAGIC_inf)
		return FileCorrupt(buffer);
	if (m[1] > FORMAT_inf)
		ErrPostEx(SEV_INFO,0,0,"File %s format number greater than expected",buffer);
	_rel_major = m[2];
	_rel_minor = m[3];
	_volume_ct = m[5];
	FileClose(fd);
	VERBOSE("File cdvolume.inf read OK. \n"); 
	VERBOSE("Release %d.%d (on %d CDs)\n",_rel_major,_rel_minor,_volume_ct); 

	/*----- Locate & read the cdentrez.inf file -----*/
	if (_dir[CdDir_sys].user_dir !=NULL)
		FileBuildPath(strcpy(buffer,_dir[CdDir_sys].user_dir),NULL,"cdentrez.inf");
	else
		CdDevice_FileBuildPath(dev_startup,buffer,_dir[CdDir_sys].dir,"cdentrez.inf");
	if ((aio = AsnIoOpen(buffer,"r")) ==NULL)
		return InvalidConfiguration(1);
	_cdinfo = EntrezInfoAsnRead(aio,NULL);
	_cdinfo->cd_count = _volume_ct;
	AsnIoClose(aio);
 	if (_cdinfo == NULL)
	{
		ErrPostEx(SEV_INFO,0,0,"EntrezInfoAsnRead failure");
		return FALSE;
	}
	
	_type_ct = _cdinfo->type_count;
	_fld_ct = _cdinfo->field_count;
	_div_ct = _cdinfo->div_count;
	_idx_page_size = _cdinfo->type_bucket_size;
	_idx_page_slots = _idx_page_size / IDX_REC_SIZE;
	_trm_page_size = _cdinfo->field_bucket_size;
	VERBOSE("File cdentrez.inf read OK.  \n");

	/*----- initialize the list of volumes -----*/
	_cdvol = (CdVolume*) MemNew(sizeof(CdVolume) * _volume_ct);
	for (n=0; n<_volume_ct; ++n)
	{
		CdVolume_Construct(&(_cdvol[n]),n+1);
#ifdef OS_MAC
		if (_cdvol[n].device == NULL)  
		{   /* the volume is not associated with any device, see if it's on desktop */
			char volname[16];
			CdRomInfo info;

			sprintf(volname,"entrez%d",n+1);
			if (CdTestPath(volname,&info))
			{	/* the volume is indeed on the desktop, find a device to mount it on */
				int i;
				for (i=0; i<_device_ct; ++i)
				{
					if (_cddev[i].volume == NULL)
					{
						if (CdVolume_Mount(&_cdvol[n],&_cddev[i],FALSE))
							break;
					}
				}
			}
		}
#endif
	}

	/*----- If fewer devices than volumes, at least one must be ejectable -----*/
	if (_device_ct < _volume_ct)
	{
		for (n=0; n<_device_ct; ++n)
		{
			if (_cddev[n].is_ejectable)	
				break;
		}
		if (n == _device_ct)
			return InvalidConfiguration(2);
	}
	
	/*----- Initialize index and term caches -----*/
	n = (int) GetAppParamInt(_rcfile,"CdEntrez","IdxCacheSize",DEFAULT_IdxCacheSize);
	pages = (int) ( (long)n * (long)KBYTE / (long)_idx_page_size);
	pages = MAX(1,MIN(pages,256));
	_idx_cache = Cache_New(pages,DefCacheDataFreeProc);
	_idx_cache->page_size = _idx_page_size;
	n = (int) GetAppParamInt(_rcfile,"CdEntrez","TrmCacheSize",DEFAULT_TrmCacheSize);
	pages = (int) ( ((long)n * (long)KBYTE) / (long)_trm_page_size);
	pages = MAX(1,MIN(pages,256));
	_trm_cache = Cache_New(pages,DefCacheDataFreeProc);
	_trm_cache->page_size = _trm_page_size;

	/*----- Initialize the UidIndex & TrmIndex structures -----*/
	_uidx = (UidIndex*) MemNew(sizeof(UidIndex) * _type_ct);
	for (n=0; n<_type_ct; ++n)
	{
		UidIndex_Construct(&(_uidx[n]),(DocType)n,&(_cdinfo->types[n]));
		
		_tidx[n] = (TrmIndex**) MemNew(sizeof(TrmIndex*) * _fld_ct);
		for (j=0; j<_fld_ct; ++j)
		{
			if (_cdinfo->types[n].fields[j].num_terms > 0)
				_tidx[n][j] = TrmIndex_New(n,j,FLDTAG(j),&(_cdinfo->types[n].fields[j]));
		}
	}
	
	/*----- Read the cdlayout.inf file -----*/
	if (_dir[CdDir_sys].user_dir !=NULL)
		FileBuildPath(strcpy(buffer,_dir[CdDir_sys].user_dir),NULL,"cdlayout.inf");
	else
		CdDevice_FileBuildPath(dev_startup,buffer,_dir[CdDir_sys].dir,"cdlayout.inf");
	if ((fd = FileOpen(buffer,"r")) ==NULL)
		return FALSE;
	ReadCdLayout(fd);
	FileClose(fd);
	VERBOSE("File cdlayout.inf read OK \n");
	
	/*----- Done. return success -----*/
	_init_ct++;
	return TRUE;
}


NLM_EXTERN Boolean cd3_CdFini (void)
{
	int i, j;
	CdEntrezDir dir;
        
	_verbose_trace = GetAppParamBoolean(_rcfile,"CdEntrez","LogVerbose",FALSE);
	if (_verbose_trace)	
          ErrLogPrintf("CdFini()\n");

	if (!IsInitialized())
		return FALSE;		
	if ((--_init_ct) > 0)
		return TRUE;
	
	/*----- Free index and cache data -----*/
	if (Cache_IsValid(_idx_cache))
	{
		Cache_LogStats(_idx_cache,"idx");
		Cache_Free(_idx_cache);
	}
	if (Cache_IsValid(_trm_cache))
	{
		Cache_LogStats(_trm_cache,"trm");
		Cache_Free(_trm_cache); 
	}
	for (i=0; i<_type_ct; ++i)
	{
		UidIndex_Destruct(&_uidx[i]);
		for (j=0; j<_fld_ct; ++j)
			TrmIndex_Free(_tidx[i][j]);
		MemFree((void*)_tidx[i]);
		_tidx[i] = NULL;
		HuffTable_Destruct(_huff_tab[i]);
		_huff_tab[i] = NULL;
	}
	MemFree((void*)_uidx);
	
	/*----- Free CdVolume and CdDevice data -----*/
	for (i=0; i<_volume_ct; ++i)
		CdVolume_Destruct(&_cdvol[i]);
	for (i=0; i<_device_ct; ++i)
		CdDevice_Destruct(&_cddev[i]); 
	MemFree((void*)_cdvol);
	MemFree((void*)_cddev);
	
	/*----- Free cdlayout stuff -----*/
	for (dir=CdDir_FIRST; dir<=CdDir_LAST; ++dir)
	{
		CdMap_Destruct(&_map[dir]);
		MemFree(_dir[dir].user_dir);
	}

	/*----- Free the EntrezInfo struct -----*/
	EntrezInfoFree(_cdinfo);
	_cdinfo = NULL;
	return TRUE;
}


NLM_EXTERN AsnIo* cd3_EntrezInfoOpen (const char *dirname)
{
	VERBOSE("EntrezInfoOpen(%s)\n", dirname ? dirname : "NULL");
	
	/* NOT IMPLEMENTED */
	
	/* Do we need this?  Things seem to be working fine without it! */
	
	return NULL;
}


NLM_EXTERN EntrezInfo* cd3_CdGetInfo (void)
{
	VERBOSE("CdGetInfo()\n");
	
	return _cdinfo;
}


NLM_EXTERN char* cd3_CdDetailedInfo (void)
{
	char *detailed;
	
	VERBOSE("CdDetailedInfo()\n");

	if ((detailed  = (char*)Malloc(8192)) == NULL)
		return NULL;
		
	if (!IsInitialized())
	{
		strcpy(detailed,"*** NOT INITIALIZED ***");
	}
	else
	{
		char *p = detailed;
		sprintf(p,"Entrez release %d.%d\n",_rel_major,_rel_minor);
		
		if (_cdinfo->div_info != NULL)
		{
			EntrezDivInfo *div;
			int i, j;

			sprintf(p=strchr(p,0),"\nDIVISION INFORMATION\n");

			for (i=0, div=_cdinfo->div_info; i<_cdinfo->div_count; ++i, ++div)
			{
				sprintf(p=strchr(p,0),"   %s:  ",div->tag);
				sprintf(p=strchr(p,0),"%-36s  ",div->descr);
				sprintf(p=strchr(p,0),"%-25s\n",div->reldate ? div->reldate : "?");
			}

			sprintf(p=strchr(p,0),"\n   div        Document Counts by Type\n");
			sprintf(p=strchr(p,0),"   tag         ml      aa      nt      st\n");
			sprintf(p=strchr(p,0),"   ---     ------  ------  ------  ------\n");
			for (i=0, div=_cdinfo->div_info; i<_cdinfo->div_count; ++i, ++div)
			{
				sprintf(p=strchr(p,0),"   %s:   ",div->tag);
				if (div->docs != NULL)
				{
					for (j=0; j<_type_ct; ++j)
						sprintf(p=strchr(p,0),"%7ld ",div->docs[j]);
				}
				sprintf(p=strchr(p,0),"\n");
			}
		}
		
		if (_idx_cache != NULL && _trm_cache != NULL)
		{
			sprintf(p=strchr(p,0),"\nCACHE STATISTICS\n");
			sprintf(p=strchr(p,0),"\nIndex page cache:\n");			
			Cache_ReportStats(_idx_cache,strchr(p,0));
			sprintf(p=strchr(p,0),"\nTerm page cache:\n");			
			Cache_ReportStats(_trm_cache,strchr(p,0));
		}
	}		
	return detailed;
}

NLM_EXTERN int  cd3_CdTrmPageCt (DocType type, DocField fld)
{
	VERBOSE("CdTrmPageCt(%d,%d)\n",type,fld);
	
	if (IsInitialized() && ValidType(type) && ValidField(fld))
	{
		return _cdinfo->types[type].fields[fld].num_bucket; 
	}
	return 0;
}

NLM_EXTERN int  cd3_CdTrmLookup (DocType type, DocField fld, const char *term)
{
	VERBOSE("CdTrmLookup(%d,%d,%s)\n",type,fld,term);

	if (IsInitialized() && ValidType(type) && ValidField(fld))
	{
		TrmIndex *tidx = _tidx[type][fld];
		if (tidx != NULL)
			return TrmIndex_Lookup(tidx,term);
	}
	return -1;
}

NLM_EXTERN CdTerm* cd3_CdTrmFind (DocType type, DocField fld, const char *term)
{
	CdTerm *trm = NULL;
	
	VERBOSE("CdTrmFind(%d,%d,%s)\n",type,fld,term);
	
	if (IsInitialized() && ValidType(type) && ValidField(fld))
	{
		TrmIndex *tidx = _tidx[type][fld];
		if (tidx != NULL)
			trm = TrmIndex_GetCdTerm(tidx,term);
	}
	return trm;
}

NLM_EXTERN int cd3_CdTermScan (DocType type, DocField fld, int page_start, 
			int page_count, CdTermProc proc)
{
	int count=0;
	
	VERBOSE("CdTermScan(%d,%d,%d,%d,[proc])\n",type,fld,page_start,page_count);
	
	if (IsInitialized() && ValidType(type) && ValidField(fld))
	{
		TrmIndex *tidx = _tidx[type][fld];
		if (tidx != NULL)
		{
			if (page_count <=0)
				page_count = INT_MAX;
			count = TrmIndex_ScanPages(tidx,page_start,page_count,proc);
		}
	}
	return (Int2)count;
}

NLM_EXTERN long cd3_CdTrmUidsFil (DocType type, DocField fld, long offset,
						long count, const char *filename, Boolean append)
{
	long ct =0;
	
	VERBOSE("CdTrmUidsFil(%d,%d,%ld,%ld,%s,%s)\n",type,fld,offset,count,
					filename, append ? "TRUE" : "FALSE" );
	
	if (IsInitialized() && ValidType(type) && ValidField(fld))
	{
		TrmIndex *tidx = _tidx[type][fld];
		if (tidx != NULL)
		{
			FILE *fd1 = TrmIndex_PostingsFileOpen(tidx);
			FILE *fd2 = FileOpen(filename,(append) ? "ab":"wb");
			if (fd1 != NULL && fd2 != NULL)
			{
				Uint4 arr[64];
				long m = DIM(arr);
				long n1, n2;

				VERIFY(fseek(fd1,offset,SEEK_SET) ==0);

				for (n1=count; n1>0; n1-=n2)
				{
					n2 = MIN(n1,m);
					n2 = FileReadSwapInt4(arr,(int)n2,fd1);
					FileWrite((void*)arr,sizeof(Int4),(size_t)n2,fd2);
				}
				ct = count - n1;
			}
			else VERBOSE("   * TrmIndex_PostingsFileOpen failed\n");
			FileClose(fd1);
			FileClose(fd2);
		}
	}
	return ct;
}


NLM_EXTERN long cd3_CdTrmUidsMem (DocType type, DocField fld, long offset,
								long count, DocUid *mem)
{
	VERBOSE("CdTrmUidsMem(%d,%d,%ld,%ld,[mem])\n",type,fld,
					offset,count);
	
	ASSERT(count < (long)(UINT_MAX/sizeof(long)));
	
	if (IsInitialized() && ValidType(type) && ValidField(fld))
	{
		TrmIndex *tidx = _tidx[type][fld];
		if (tidx != NULL)
		{
			FILE *fd = TrmIndex_PostingsFileOpen(tidx);
			if (fd != NULL)
			{
				VERIFY(fseek(fd,offset,SEEK_SET) ==0);
				count = FileReadSwapInt4((Uint4*)mem,(int)count,fd);
				FileClose(fd);
				return count;
			}
		}
	}
	return 0;
}


NLM_EXTERN int cd3_CdLinkUidGet (LinkSetPtr *result, DocType type,
		DocType link_to_type, int numuid, DocUid *uid_list, 
		Boolean mark_missing, long maxlink)
{
	int i, cnt;
	DocUid uid, *puid = uid_list;
	UidIndex *uidx;
	UidIndexRec rec;
	FILE *fd=NULL;
	LSet *lset = NULL;
	
	VERBOSE("CdLinkUidGet([result],%d,%d,%d,[list],%s,%ld)\n",
				type,link_to_type,numuid,
				mark_missing ? "TRUE" : "FALSE", maxlink);
	
	if (! (IsInitialized() && ValidType(type) && ValidType(link_to_type)) )
	{
		*result = NULL;
		return 0;
	}
		
	uidx = &(_uidx[type]);
	
	for (i=cnt=0; i<numuid; ++i, ++puid)
	{
		uid = ABS(*puid);
		if (UidIndex_Lookup(uidx,uid,&rec))
		{
			cnt++;

			if (rec.lnk_offset > 0)
			{		
				if (fd == NULL)
				{
					char fname[16];
					strcpy(fname,TYPTAG(type));
					strcat(fname,".lnk");
					if ((fd = CdEntrez_FileOpen(CdDir_lnk,type,rec.divnum,LNK,fname,"rb")) ==NULL)
						break;
					maxlink = MIN(maxlink,INT_MAX);
					if ((lset = LSet_New(type,link_to_type,(int)maxlink)) ==NULL)
						break;
				}
				if (fseek(fd,rec.lnk_offset,SEEK_SET) !=0)
				{
					ErrPostEx(SEV_INFO,0,0,"fseek failure");
					break;
				}
				if (!LSet_Read(lset,fd))
					break;
			}
		}
		else if (mark_missing)
		{
			*puid = -uid;
		}
	}
	
	CdEntrez_FileClose(CdDir_lnk,type,fd);
	*result = (lset==NULL) ? NULL : LSet_Convert(lset);
	return cnt;
}

#define BIT_non_document   0x001
#define BIT_no_abstract    0x002
#define BIT_no_authors     0x004
#define BIT_trans_title    0x008
#define BIT_is_partial     0x010
#define BIT_is_segmented   0x020

NLM_EXTERN DocSum* cd3_CdGetDocSum (DocType type, DocUid uid)
{
	DocSum *sum = NULL;
	UidIndex *uidx;
	UidIndexRec rec;                            
	
	VERBOSE("CdGetDocSum(%d,%ld)\n",type,uid);
		
	if (!IsInitialized() || !ValidType(type))
		return NULL;		
	
	uidx = &(_uidx[type]);
	if (UidIndex_Lookup(uidx,uid,&rec) && rec.sum_offset > 0)
	{
		char fname[16];
		FILE *fd;

		strcpy(fname,TYPTAG(type));
		strcat(fname,".sum");
		if ((fd = CdEntrez_FileOpen(CdDir_sum,type,0,SUM,fname,"rb")) !=NULL)
		{
			if (fseek(fd,rec.sum_offset,SEEK_SET) !=0)
				ErrPostEx(SEV_INFO,0,0,"fseek failure");

			if ((sum = (DocSum*)MemNew(sizeof(DocSum))) !=NULL)
			{
				unsigned short m[4];
				FileReadSwapShort(m,3,fd);
				
				/* m[0] is doc-flags */
				if (m[0] & BIT_non_document)  sum->non_document = TRUE;
				if (m[0] & BIT_no_abstract)  sum->no_abstract = TRUE;
				if (m[0] & BIT_no_authors)  sum->no_authors = TRUE;
				if (m[0] & BIT_trans_title)  sum->translated_title = TRUE;
				if (m[0] & BIT_is_partial)  sum->is_partial = TRUE;
				if (m[0] & BIT_is_segmented)  sum->is_segmented = TRUE;

				/* m[1] is create date */
				if (m[1] != 0)
				{
					sum->create.year =  1950 + ((int) (m[1] & 0xFE00) >> 9);
					sum->create.month = ((int) (m[1] & 0x01E0) >> 5);
					sum->create.day = (m[1] & 0x001F);
				}
				
				/* m[2] is modify date */
				if (m[2] != 0)
				{
					sum->modify.year =  1950 + ((int) (m[2] & 0xFE00) >> 9);
					sum->modify.month = ((int) (m[2] & 0x01E0) >> 5);
					sum->modify.day = (m[2] & 0x001F);
				}
				
				FileReadSwapShort((unsigned short*)sum->link_count,_type_ct,fd);
				sum->caption = FileReadStr(fd,1);
				sum->title = FileReadStr(fd,2);
				sum->extra = FileReadStr(fd,1);
				sum->uid = uid;
			}
			CdEntrez_FileClose(CdDir_sum,type,fd);
		}
	}
	else
	{
		VERBOSE("   * bad UID %ld ?\n", uid);
	}
	return sum;
}

NLM_EXTERN AsnIo* cd3_CdDocAsnOpen (DocType type, DocUid uid)
{
	char fname[16];
	AsnIo *aio = NULL;
	UidIndex *uidx;
	UidIndexRec rec;
	
	VERBOSE("CdDocAsnOpen(%d,%ld)\n",type,uid);
		
	if (!IsInitialized() || !ValidType(type))
		return NULL;

	uidx = &(_uidx[type]);
	if (UidIndex_Lookup(uidx,uid,&rec))
	{
		char *fdir = NULL;
		FILE *fd;
		DecompInfo *decomp;
		int c;

		sprintf(fname,"%s%s%03d.hca",TYPTAG(type),DIVTAG(rec.divnum-1),rec.filnum);
		if ((fd = CdEntrez_FileOpen(CdDir_rec,uidx->type,rec.divnum,HCA,fname,"rb")) ==NULL)
			return NULL;

		if (_huff_tab[type] == NULL)
		{
			fseek(fd,22,SEEK_SET);
			_huff_tab[type] = HuffTable_Read(fd);
			if (_huff_tab[type] == NULL)
			{
				return (AsnIo*) CatastrophicFailure(1);
			}
		}
		if (fseek(fd,rec.hca_offset,SEEK_SET) !=0)
		{
			FileClose(fd);
			ErrPostEx(SEV_INFO,0,0,"fseek failure");
		}
		else
		{
			c = fgetc(fd);
			if ((c & 0x0F) != 1)
				ErrPostEx(SEV_INFO,0,0,"unknown error");
			c = fgetc(fd);
			c = fgetc(fd);
			c = fgetc(fd);
			decomp = DecompInfo_New(_huff_tab[type]);
			aio = DecompInfo_Attach(decomp,fd);
		}
	}
	return aio;
}

NLM_EXTERN AsnIo* cd3_CdDocAsnClose (AsnIo* aio)
{
	DecompInfo *info;
	FILE *fd;
	
	VERBOSE("CdDocAsnClose([aio])\n");
	
	ASSERT(aio != NULL);
	info = (DecompInfo*) aio->iostruct;
	ASSERT(info!= NULL);
	fd = DecompInfo_Detach(info);
	FileClose(fd);
	DecompInfo_Free(info);
	return NULL;
}


Boolean LIBCALL CdTestPath (const char *path, CdRomInfo *info)
{
	static char *fdir = SYS_DIRNAME;
	static char *fname = "cdvolume.inf";
	char fpath[256], *p;
	unsigned short m[8];
	FILE *fd;
	
	VERBOSE("CdTestPath(%s,[info])\n",path);
	ASSERT(info != NULL);
	
	memset((void*)info,0,sizeof(CdRomInfo));
	
	/*----- lowercase, without ";1" -----*/
	strcpy(fpath,path);
	p = strchr(fpath,'\0');
	FileBuildPath(fpath,fdir,fname);
	StrLower(p);
	if (FileLength(fpath) >0)
		goto DisneyLand;
	VERBOSE("   access attempt [%s] failed\n",fpath);

	/*----- lowercase, with ";1" -----*/
	strcat(fpath,";1");
	if (FileLength(fpath) >0)
	{
		info->semicolon_one = 1;
		goto DisneyLand;
	}
	VERBOSE("   access attempt [%s] failed\n",fpath);

	/*----- uppercase, without ";1" -----*/
	info->upper_case = 1;
	strcpy(fpath,path);
	FileBuildPath(fpath,fdir,(char*)fname);
	StrUpper(p);
	if (FileLength(fpath) >0)
		goto DisneyLand;
	VERBOSE("   access attempt [%s] failed\n",fpath);

	/*----- uppercase, with ";1" -----*/
	strcat(fpath,";1");
	if (FileLength(fpath) >0)
	{
		info->semicolon_one = 1;
		goto DisneyLand;
	}

	VERBOSE("   access attempt [%s] failed\n",fpath);
	VERBOSE("   struck out on [%s].  must be either invalid or not mounted\n",path);
	return FALSE;

	
/*----- success -----*/
 DisneyLand:       
	VERBOSE("   access attempt [%s] succeeded\n",fpath);
	
	if ((fd = FileOpen(fpath,"rb")) == NULL)
	{
		/*ErrPostEx(SEV_ERROR,2,3,"fopen(\"%s\",\"rb\") failed\n",fpath);*/
		return FALSE;
	}

	/*----- read first 5 values in file -----
	*	m[0]  magic value for this file
	*	m[1]  format code for this file
	*	m[2]  cdrom release number (major portion)
	*	m[3]  cdrom release number (minor portion)
	*	m[4]  cdrom volume number (1, 2, or 3)
	*	m[5]  number of CD-ROMs in the set
	*/
		
	FileReadSwapShort(m,6,fd);   /**** need to check for error ****/
	FileClose(fd);
	if (m[0] != MAGIC_inf)
		return FileCorrupt(fpath);
	if (m[1] != FORMAT_inf)
		VERBOSE("   unexpected format number (%d) for file %s\n",(int)m[1],fpath);
	info->rel_major = m[2];
	info->rel_minor = m[3];
	info->cd_num = m[4];
	info->cd_count = m[5];
	VERBOSE("   release %d.%d; cd %d of %d\n",(int)m[2],(int)m[3],(int)m[4],(int)m[5]);
	return TRUE;	
}


static char UnixReadChar (FILE *f)

{
  char  ch;
  int   getcrsult;

  ch = '\0';
  if (f != NULL) {
    getcrsult = fgetc (f);
    ch = (char) getcrsult;
    if (getcrsult == EOF && feof (f)) {
      ch = '\0';
    }
  }
  return ch;
}

static char *UnixFileGets(char *buff, int maxsize, FILE *f)

{
  int  ch;
  int  i;

  if (buff != NULL && maxsize > 0) {
    *buff = '\0';
    if (f != NULL) {
      i = 0;
      ch = UnixReadChar (f);
      while (ch != '\0' && ch != '\r' && ch != '\n' && i < maxsize) {
        buff [i] = ch;
        i++;
        ch = UnixReadChar (f);
      }
      if (ch != '\0') {
        buff [i] = ch;
        i++;
      }
      buff [i] = '\0';
    }
  }
  if (ch == '\0') {
    return NULL;
  } else {
    return buff;
  }
}

NLM_EXTERN long LIBCALL cd3_CdEnumFiles (CdEntrezDir dir, DocType type, const char *div,
			EntrezEnumFileProc proc, void *opaque_data)
{
	FILE *fd;
	
	VERBOSE("CdEnumFiles(%d,%d,%s,[proc],[opaque])\n",dir,type,
				div ? div : "NULL");
	
	if ((fd = CdEntrez_FileOpen(CdDir_sys,0,0,0,"cdlayout.inf","r")) != NULL)
	{
		char prefix[32], *p;
		char line[80];	
		long size, total_size = 0;
		int doit = FALSE;
		int count =0;
		int cdnum;
			
		strcpy(prefix,_dir[dir].key);
		if (type >= 0)
		{
			strcat(prefix,"-");
			strcat(prefix,TYPTAG(type));
			if (div != NULL)
			{
				strcat(prefix,"-");
				strcat(prefix,div);
			}
		}
		
		while (UnixFileGets(line,sizeof line,fd))
		{
			if (strchr("#\r\n",line[0])) 
				continue;  /* comment line */
				
			if (isalpha(line[0]))
			{
				if (doit)
					break;
					
				if (strncmp(line,prefix,strlen(prefix))==0)
				{
					doit = TRUE;
					VERIFY(p=strchr(line+1,'\t'));
					*p++ = '\0';
					cdnum = atoi(p);
				}
				else doit = FALSE;
			}
			else if (line[0] == '\t')
			{
				if (doit)
				{
					VERIFY(p=strchr(line+1,'\t'));
					*p++ = '\0';
					size = atol(p);
					total_size += size;
					count++;
					if (proc != NULL)
					{
						if (!(*proc)(cdnum,_dir[dir].dir,line+1,size,opaque_data))
							break;
					}
				}
			}
		}
		FileClose(fd);

		if (count==0 && dir==CdDir_sys)
		{
			static char * fn[] = { "cdentrez.inf", "cdlayout.inf", "cdvolume.inf" };
			static long   fs[] = { 9865, 4504, 12 };
			int i;
			
			/* The list of filenames and filesizes for the sysinfo directory
				was inadvertently omitted from the cdlayout.inf file on the 
				sample CD-ROMs distributed to developers.  This section of
				code should never be executed in the final release. */
			
			VERBOSE("   HACK (faking cdlayout.inf information for SYSINFO directory)\n");
			for (i=0; i<DIM(fs); ++i, ++count)
			{
				if (proc != NULL)
				{
					if (!(*proc)(1,_dir[dir].dir,fn[i],fs[i],opaque_data))
						break;
				}
				total_size += fs[i];
			}
		}

		VERBOSE("   %d files enumerated\n",count);
		return total_size;
	}
	return 0;
}


NLM_EXTERN CdDevHook LIBCALL CdSetInsertHook (CdDevHook hook)
{
    CdDevHook prev = _hookInsert;

	VERBOSE("CdSetInsertHook\n");

	_hookInsert = (hook==NULL) ? hook : default_CdInsertProc;
	return prev;
}


NLM_EXTERN CdDevHook LIBCALL CdSetEjectHook (CdDevHook hook)
{
    CdDevHook prev = _hookEject;

	VERBOSE("CdSetEjectHook\n");

	_hookEject = (hook==NULL) ? hook : default_CdEjectProc;
	return prev;
}


NLM_EXTERN Boolean LIBCALL CdMountEntrezVolume (int cdnum, char *root, size_t buflen)
{
	if (root != NULL)  *root = '\0';

	if (cdnum < 1 || cdnum > _volume_ct)
		ErrPostEx(SEV_WARNING,1,0,"Entrez CD number (%d) out of range",cdnum);
	else
	{
		CdVolume *cdvol = & _cdvol[cdnum-1];
		CdDevice *cddev;

		if ((cddev = cdvol->device) ==NULL)
			cddev = CdVolume_Mount(cdvol,NULL,TRUE);

		if (cddev != NULL)
		{
			if (root != NULL)
			{
				char temp[256];
				CdDevice_FileBuildPath(cddev,temp,NULL,NULL);
				strncat(root,temp,buflen);
			}
			return TRUE;
		}
	}
	return FALSE;
}


NLM_EXTERN Boolean LIBCALL CdUnmountEntrezVolume (int cdnum)
{
	CdVolume *cdvol;
	CdDevice *cddev;

	if (cdnum < 1 || cdnum > _volume_ct)
	{
		ErrPostEx(SEV_WARNING,1,0,"Entrez CD number (%d) out of range",cdnum);
		return FALSE;
	}

	cdvol = & _cdvol[cdnum-1];
	cddev = cdvol->device;
	if (cddev != NULL && cddev->is_ejectable)
	{
		char volume_to_eject[32];
		volume_to_eject[0] = '\0';
		if (cddev->ins_volname)
			sprintf(volume_to_eject,"entrez%d",cddev->volume->volume_num);
		else if (cdvol->volume_name != NULL)
			strcpy(volume_to_eject,cdvol->volume_name);

		if (!(*_hookEject)(volume_to_eject,&cddev->inf))
			ErrPostEx(SEV_INFO,0,0,"Unable to eject %s",cdvol->volume_name);

		return CdVolume_Unmount(cdvol);
	}
	return FALSE;
}


/****************************************************************************
*
*		STATIC FUNCTIONS
*
*****************************************************************************/


static Boolean IsInitialized (void)
{
	if (_cdinfo == NULL)
	{
		ErrPostEx(SEV_INFO,ERR_NotInited,1,"Library not initialized");
		return FALSE;
	}
	return TRUE;
}


static Boolean ValidType (int type)
{
	if (!TYPE_IS_VALID(type))
	{
		ErrPostEx(SEV_INFO,ERR_BadParam,SUB_DocType,
					"DocType value (%d) out of range ",type);
		return FALSE;
	}
	return TRUE;
}


static Boolean ValidField (int field)
{
	if (!FIELD_IS_VALID(field))
	{
		ErrPostEx(SEV_INFO,ERR_BadParam,SUB_DocField,
					"DocField value (%d) out of range ",field);
		return FALSE;
	}
	return TRUE;
}

static FILE * CdEntrez_FileOpen (CdEntrezDir dirnum, int doctype, int divnum, 
			int ftype, const char *fname, const char *fmode)
{
	static int cdnum[CDVOL_MAX];
	FILE *fd = NULL;
	CdMap *map;
	int cdcnt;
	char fpath[256];
	
	ASSERT(dirnum >= CdDir_FIRST && dirnum <= CdDir_LAST);
	ASSERT(doctype < _type_ct);
	ASSERT(divnum <= _div_ct);

	fpath[0] = '\0';
	cdcnt = 0;
	map = &_map[dirnum];
	cdcnt = CdMap_GetSpecs(map,cdnum,fpath);
	if ((map = CdMap_GetChild(map,doctype)) != NULL)
	{
		cdcnt = CdMap_GetSpecs(map,cdnum,fpath);
		if ((map = CdMap_GetChild(map,divnum)) != NULL)
		{
			cdcnt = CdMap_GetSpecs(map,cdnum,fpath);
		}
	}
	
	if (fpath[0] != '\0' && fpath[0] != '#')
	{
		FileBuildPath(fpath,NULL,(char*)fname);
		fd = FileOpen(fpath,fmode);
	}
	else if (cdcnt > 0)
	{
		CdVolume *cdvol;
		CdDevice *cddev;
		int n;
		
		if (cdcnt == 1)
		{
			n = cdnum[0];
			ASSERT(VOLNUM_IS_VALID(n));
			cdvol = &_cdvol[n-1];
		}
		else
		{
			/* If the information sought exists on more than one volume, select
				a mounted volume if possible.  Favor most-recently-used volumes. */
			int i;
			CdVolume *cdv;
			
			for (i=0, cdvol=NULL; i<cdcnt; ++i)
			{
				n = cdnum[i];
				ASSERT(VOLNUM_IS_VALID(n));
				cdv = & _cdvol[n-1];
				if (cdvol == NULL)
					cdvol = cdv;
				else if (!CdVolume_IsMounted(cdvol) && CdVolume_IsMounted(cdv))
					cdvol = cdv;
				else if (cdv->timestamp > cdvol->timestamp)
					cdvol = cdv;
			}
		}
		ASSERT(cdvol != NULL);
					
		if ((cddev=cdvol->device) ==NULL)
		{
			if ((cddev=CdVolume_Mount(cdvol,NULL,TRUE)) ==NULL)
			{
				VERBOSE("   * CdVolume_Mount failed\n");
				return NULL;
			}
		}
		fd = CdDevice_FileOpen(cddev,_dir[dirnum].dir,fname,fmode);	
	}
	
	if (fd != NULL)
	{
		if (ftype == 0)
			return fd;
		else
		{
			unsigned short m[4];
			
			if (FileReadSwapShort(m,4,fd) != 4)
			{
				FileClose(fd);
				FileCorrupt(fname);
				return NULL;
			}
			fseek(fd,0,SEEK_SET);
			/**VERBOSE("HEADER %s:  %d %d %d %d\n",fname,m[0],m[1],m[2],m[3]);**/
			
			if (m[0] && m[0] != _finfo[ftype].magic)
				FileCorrupt(fname);
			else if (m[1] && m[1] > _finfo[ftype].format)
				FileNotRecognized(fname);
			else if (m[2] && m[2] != (unsigned short) _cdinfo->version)
				FileOutOfDate(fname);
			else if (m[3] && m[3] != (unsigned short) _cdinfo->issue && ftype != HCA)
				FileOutOfDate(fname);
			else
				return fd;
		}
	}
	FileClose(fd);
	return NULL;
}


static void CdEntrez_FileClose (CdEntrezDir dirnum, int doctype, FILE *fd)
{

	/************ NOT IMPLEMENTED *************/
	
    
    /*  This function will close some files and leave others
    	open depending on whether the device on which they 
    	reside is ejectable or not.   Right now it just closes
    	everything. */
    
    
	FileClose(fd);
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* UidIndex Functions.        Schuler 06-13-94                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
#define UIDX_KEY(a,b)   (((unsigned long)(a) << 24) | (b))


static UidIndex* UidIndex_Construct (UidIndex *uidx, DocType type, EntrezTypeData *info)
{
	if (uidx != NULL)
	{
		uidx->type = type;
		uidx->uid_ct = info->num_uids;
		uidx->uid_min = info->minuid;
		uidx->uid_max = info->maxuid;
		uidx->pages = info->num_bucket;
		uidx->index = NULL;
	}
	return uidx;
}


static UidIndex* UidIndex_Destruct (UidIndex *uidx)
{
	if (uidx != NULL)
		MemFree((void*)uidx->index);

	return uidx;
}


static int UidIndex_ReadPageMap (UidIndex *uidx)
{
	char fname[32];
	long *index;
	FILE *fd;
	
	ASSERT(uidx != NULL);
	ASSERT(TYPE_IS_VALID(uidx->type));
	ASSERT(uidx->index == NULL);
	
	if ((index = (long*) MemNew(sizeof(long) * uidx->pages)) ==NULL)
		return FALSE;

	sprintf(fname,"%s.oix",TYPTAG(uidx->type));
	if ((fd = CdEntrez_FileOpen(CdDir_idx,uidx->type,0,OIX,fname,"rb")) ==NULL)
		return FALSE;
	fseek(fd,8,SEEK_SET); /* to skip over header */
	if (FileReadSwapLong((unsigned long*)index,uidx->pages,fd) != uidx->pages)
		return CatastrophicFailure(1);
	FileClose(fd);
	uidx->index = index;
	return TRUE;
}


static long* UidIndex_ReadPage (UidIndex *uidx, int pagenum)
{
	int recs, vals;
	size_t bytes;
	char fname[32];
	FILE *fd;
	long *page;
	
	ASSERT(uidx != NULL);
	ASSERT(pagenum < uidx->pages && pagenum >= 0);
	
	/*----- open file, check magic value, version, etc -----*/
	sprintf(fname,"%s.ofs",TYPTAG(uidx->type));
	if ((fd = CdEntrez_FileOpen(CdDir_idx,uidx->type,0,OFS,fname,"rb")) ==NULL)
		return NULL;
	fseek(fd,8,SEEK_SET);  /* to skip over header */
	
	/*----- seek to the right position, allocate memory, read the page -----*/
	if (fseek(fd,(long)pagenum*_idx_page_size,SEEK_CUR) != 0)
		return (long*)CatastrophicFailure(1);
	recs = (pagenum < uidx->pages-1) ? _idx_page_slots :
				(int)(uidx->uid_ct - (long)pagenum*_idx_page_slots);
	vals = 5 * recs;
	bytes = vals * sizeof(long);
	if ((page = MemGet(bytes,MGET_ERRPOST)) ==NULL)
		return NULL;
	if (FileReadSwapLong((unsigned long*)page,vals,fd) != vals)
		return (long*)CatastrophicFailure(1);
	FileClose(fd);
	return page;
} 


static int UidIndex_Lookup (UidIndex *uidx, DocUid uid, UidIndexRec *rec)
{
	int lo, hi, mid;
	int page_num, page_slots;
	unsigned long cache_key;
	long *page, *p;
	Boolean missed =FALSE;
	
	ASSERT(rec != NULL);
	memset((void*)rec,0,sizeof(UidIndexRec));
	
	if (uid < uidx->uid_min || uid > uidx->uid_max)
		return FALSE;
	
	/*----- load the index, if necessary -----*/
	if (uidx->index == NULL)
	{
		if (!UidIndex_ReadPageMap(uidx))
			return FALSE;
	}

	/*----- binary search the index -----*/
	lo = 0;
	hi = (int)uidx->pages -1;
	while (lo <= hi)
	{
		mid = (lo + hi) /2;
		p = uidx->index + mid;
		if (*p == uid)
			break;
		if (uid < *p)
			hi = mid-1;
		else
			lo = mid+1;
	}
	page_num = (uid < *p && mid >0) ? mid-1 : mid;
	page_slots = (page_num < uidx->pages -1) ? _idx_page_slots :
			(int)(uidx->uid_ct - (long)page_num * _idx_page_slots);

	
	/*----- check to see if desired page is in cache -----*/
	cache_key = UIDX_KEY(uidx->type,page_num);
	page = (long*) Cache_Lock(_idx_cache,cache_key);
	if (page == NULL)
	{
		missed = TRUE;
		if ((page = UidIndex_ReadPage(uidx,page_num)) ==NULL)
			return FALSE;
		if (!Cache_Insert(_idx_cache,cache_key,page))
			return CatastrophicFailure(1);
	}
	
	
	/*----- Binary search the page using the uid as a key -----*/
	lo = 0;
	hi = page_slots -1;
	
	while (lo <= hi)
	{
		mid = (lo + hi) /2;
		p = page + mid*5;
		if (*p == uid)
		{
			rec->uid = *p++;
			rec->divnum = (short) ((*p & 0x0FFF0000) >>16);
			rec->filnum = (short) (*p++ & 0x0000FFFF);
			rec->hca_offset = *p++;	
			rec->sum_offset = *p++;	
			rec->lnk_offset = *p;	
			break;
		}
		if (uid < *p)
			hi = mid-1;
		else
			lo = mid+1;
	}
	if (!missed)
		Cache_Unlock(_idx_cache,cache_key);
	return (rec->uid != 0);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* TrmIndex Functions.        Schuler 07-02-94                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
#define TIDX_KEY(a,b,c)   (((unsigned long)(a) << 24) | ((unsigned long)(b) << 16) | (c))
#define T_HEAD_SIZE  16


static TrmIndex* TrmIndex_Construct (TrmIndex *tidx, int type, int fld, 
				const char *tag, EntrezFieldData *info)
{
	if (tidx != NULL)
	{
		tidx->type = type;
		tidx->fld = fld;
		strcpy(tidx->tag,tag);
		tidx->trm_ct = info->num_terms;
		tidx->page_ct = info->num_bucket;
	}
	return tidx;
}


static TrmIndex* TrmIndex_Destruct (TrmIndex *tidx)
{
	if (tidx != NULL)
	{
		char **p = tidx->index;
		if (p != NULL)
		{
			int i;
			for (i=0; i<tidx->page_ct; ++i, ++p)
				MemFree((void*)*p);
			MemFree((void*)tidx->index);
		}
		tidx = NULL;
	}
	return tidx;
}


static int TrmIndex_ReadPageMap (TrmIndex *tidx)
{
	char fname[32];
	unsigned short m[8];
	char **p;
	FILE *fd;
	int i;
	
	ASSERT(tidx != NULL);
	ASSERT(tidx->index == NULL);
	
	sprintf(fname,"%s%s.tix",TYPTAG(tidx->type),FLDTAG(tidx->fld));
	if ((fd = CdEntrez_FileOpen(CdDir_trm,tidx->type,0,TIX,fname,"rb")) ==NULL)
		return FALSE;
	FileReadSwapShort(m,8,fd);
	tidx->page_size = (m[4]==0) ? _trm_page_size : m[4];
	if ((tidx->index = (char**) MemNew(sizeof(char*) * tidx->page_ct)) !=NULL)
	{
		for (i=0, p=tidx->index; i<tidx->page_ct; ++i, ++p)
		{
			if ((*p = FileReadStr(fd,1)) ==NULL)
			{
				TrmIndex_Destruct(tidx);
				break;
			}
		}
	}
	FileClose(fd);
	return (tidx->index != NULL);
}


static Byte* TrmIndex_ReadPage (TrmIndex *tidx, int pagenum)
{
	Byte *page = NULL;
	char fname[32];
	long offset;
	FILE *fd;
	
	ASSERT(tidx != NULL);
	ASSERT(pagenum < tidx->page_ct);

	sprintf(fname,"%s%s.trm",TYPTAG(tidx->type),FLDTAG(tidx->fld));
	if ((fd = CdEntrez_FileOpen(CdDir_trm,tidx->type,0,TRM,fname,"rb")) ==NULL)
		return FALSE;
	offset = (long)T_HEAD_SIZE + (long)tidx->page_size * pagenum;
	VERIFY(fseek(fd,offset,SEEK_SET) ==0);
		
	page = MemGet(tidx->page_size+1, MGET_ERRPOST);
	if (page != NULL)
	{
		size_t n = fread((void*)page,1,tidx->page_size,fd);  /*check for error*/
		*(page+n) = '\0';   /* sentinel */
		if (*page != 0xAB)
			FileCorrupt(fname);
	}
	FileClose(fd);
	return page;
}


static int TrmIndex_Lookup (TrmIndex *tidx, const char *stem)
{
	int lo, hi, mid, len, d;
	char *term;
	
	
	ASSERT(stem != NULL);
	len = strlen(stem);

	/*----- load the index, if necessary -----*/
	if (tidx->index == NULL)
	{
		if (!TrmIndex_ReadPageMap(tidx))
			return -1;
	}

	/*----- binary search the index -----*/
	lo = 0;
	hi = (int)tidx->page_ct -1;
	while (lo <= hi)
	{
		mid = (lo + hi) /2;
		term = *(tidx->index + mid);
		d = trmncmp(stem,term,len);
		if (d == 0)
			break;
		if (d < 0)
			hi = mid-1;
		else
			lo = mid+1;
	}

	for ( ; d <= 0 && mid >0; --mid)
	{
		if (d==0 && len == (int)strlen(term))
			break;
		term = *(tidx->index + (mid-1));
		d = trmncmp(stem,term,len);
	}
	return mid;
}


static CdTerm* TrmIndex_GetCdTerm (TrmIndex *tidx, const char *term)
{
	CdTerm *trm = NULL;
	int page_num;
	
	ASSERT(tidx != NULL);
	
	if ((page_num = TrmIndex_Lookup(tidx,term)) >=0)
	{
		unsigned long cache_key = TIDX_KEY(tidx->type,tidx->fld,page_num);
		Byte *page = Cache_Lock(_trm_cache,cache_key);
		int missed;
		if (page != NULL)
			missed = FALSE;
		else
		{
			missed = TRUE;
			page = TrmIndex_ReadPage(tidx,page_num);
		}
		if (page != NULL)
		{
			Byte *ptr = page +1;
			int flags, len, d;
			
			while ((flags = *ptr++) != 0)
			{
				len = *ptr++;
				d = trmcmp(term,(char*)ptr);
				if (d == 0)
				{
					ptr -= 2;
					trm = getcdtrm(tidx,page_num,&ptr);
					break; 
				}
				ptr += len;
				if (flags & 0x01)  ptr +=4;
				if (flags & 0x02)  ptr +=4;
				ptr += 4;
			}
			
			if (missed)
				Cache_Insert(_trm_cache,cache_key,page);
			else
				Cache_Unlock(_trm_cache,cache_key);
		}
	}
	return trm;
}


static int TrmIndex_ScanPages (TrmIndex *tidx, int start, int count, CdTermProc proc)
{
	int page_num = start;
	int cancel = FALSE;
	int ct;
	
	/*----- load the index, if necessary -----*/
	if (tidx->index == NULL)
	{
		if (!TrmIndex_ReadPageMap(tidx))
			return 0;
	}

	for (ct=0; ct<count && page_num<tidx->page_ct && !cancel; ++ct, ++page_num)
	{
		unsigned long cache_key = TIDX_KEY(tidx->type,tidx->fld,page_num);
		Byte *page = Cache_Lock(_trm_cache,cache_key);
		int missed;
		if (page != NULL)
			missed = FALSE;
		else
		{
			missed = TRUE;
			page = TrmIndex_ReadPage(tidx,page_num);
		}
		if (page != NULL)
		{
			Byte *ptr = page +1;
			CdTerm *trm;
			while ((trm = getcdtrm(tidx,page_num,&ptr)) != NULL)
			{
				if (!(*proc)(trm))
				{
					cancel = TRUE;
					break;
				}
			}
		}
		if (missed)
			Cache_Insert(_trm_cache,cache_key,page);
		else
			Cache_Unlock(_trm_cache,cache_key);
	}
	return ct;
}


static FILE * TrmIndex_PostingsFileOpen (TrmIndex *tidx)
{
	char fname[32];
	FILE *fd;
	
	ASSERT(tidx != NULL);

	sprintf(fname,"%s%s.pst",TYPTAG(tidx->type),FLDTAG(tidx->fld));
	if ((fd = CdEntrez_FileOpen(CdDir_trm,tidx->type,0,PST,fname,"rb")) ==NULL)
		return NULL;
	return fd;
}


#define DECODE_LONG(ptr,val) 	{ int i;    \
		for (i=0, val = 0; i<4; ++i)        \
		{ val <<= 8; val |= (unsigned long)*ptr++;	} } 


static CdTerm* getcdtrm (TrmIndex *tidx, int pagenum, Byte **pptr)
{
	CdTerm *trm;
	Byte *ptr = *pptr;
	int flags = *ptr++;
	if (flags != 0)
	{
		int len = *ptr++;
		char *str = MemGet(1+len,MGET_ERRPOST);
		if (str!=NULL && (trm = MemNew(sizeof(CdTerm))) != NULL)
		{
			unsigned long val;
			int i;
			ASSERT((flags & 0xF0) ==0xE0);
			trm->type = tidx->type;
			trm->field = tidx->fld;
			trm->page = pagenum;
			trm->term = str;
			for (i=0; i<len; ++i)
				*str++ = (char) *ptr++;
			*str = '\0';
					
			if (flags & 0x01)
			{
				DECODE_LONG(ptr,val);
				trm->total_count = (long)val;
			}
			if (flags & 0x02)
			{
				DECODE_LONG(ptr,val);
				trm->special_count = (long)val;
			}
			DECODE_LONG(ptr,val);
			trm->offset = (long)val;
			*pptr = ptr;
			return trm;
		}
	}
	return NULL;
}

static int trmcmp (const char *term1, const char *term2)
{
	const char *p1 = term1;
	const char *p2 = term2;
	int c1 =0, c2 =0;
	
	while (*p1 && *p2)
	{
		c1 = *p1++;
		if (isalpha(c1))  c1 = tolower(c1);
		c2 = *p2++;
		if (isalpha(c2))  c2 = tolower(c2);
		if (c1 != c2)
		{
			if (c1 == '/')
				return (c2 == '/') ? 0 : -1;
			else if (c2 == '/')
				return 1;
			return (c1 - c2);
		}
	}
	return ((int)*p1 - (int)*p2);
}

static int trmncmp (const char *term1, const char *term2, int n)
{
	const char *p1 = term1;
	const char *p2 = term2;
	int i, c1, c2;
	
	for (i=0; i<n; ++i)
	{
		c1 = *p1++;
		if (isalpha(c1))  c1=tolower(c1);
		c2 = *p2++;
		if (isalpha(c2))  c2=tolower(c2);
		if (c1==0 || c2==0 || c1!=c2)
			break;
	}
	if (i==n) 
		return 0;
	if (c1 == '/')
		return (c2 == '/') ? 0 : -1;
	else if (c2 == '/')
		return 1;
	return (c1 - c2);
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* CdVolume Functions.        Schuler 05-21-94                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static CdVolume* CdVolume_Construct (CdVolume *cdvol, int number)
{
	if (cdvol != NULL)
	{
		char volname[16];
		int i;
		CdDevice *cddev;
		
		sprintf(volname,"entrez%d",number);
		cdvol->volume_name = StrSave(volname);
		cdvol->volume_num = number;
		for (i=0, cddev=_cddev; i<_device_ct; ++i, ++cddev)
		{
			if (cddev->bound_cd == number)
			{
				CdVolume_Mount(cdvol,cddev,TRUE);
				break;
			}
			if (cddev->hint == number)
			{
				cddev->hint = 0;
				CdVolume_Mount(cdvol,cddev,FALSE);
				break;
			}
		}
	}
	return cdvol;
}


static CdVolume* CdVolume_Destruct (CdVolume *cdvol)
{
	if (cdvol != NULL)
	{
		MemFree((void*)cdvol->volume_name);
		cdvol->volume_name = NULL;
	}

	return cdvol;
}


static CdDevice* GetLruDevice (void)
{
	CdDevice *dev_lru =NULL;
	time_t time_lru;
	CdDevice *dev;
	int i;
	
	for (i=0, dev=_cddev; i<_device_ct; ++i, ++dev)
	{
		if (dev->is_ejectable)
		{
			if (dev->volume == NULL)
			{
				dev_lru = dev;
				break;
			}
			if (dev_lru == NULL || dev->timestamp < time_lru)
			{
				dev_lru = dev;
				time_lru = dev->timestamp;
			}
		}
	}
	if (dev_lru != NULL && dev_lru->volume != NULL)
	{
		CdVolume *vol_to_eject = dev_lru->volume;
		CdVolume_Unmount(dev_lru->volume);
		if (dev_lru->is_ejectable)
		{
			if (!(*_hookEject)(vol_to_eject->volume_name,&dev_lru->inf))
				ErrPostEx(SEV_INFO,0,0,"Unable to eject %s",vol_to_eject->volume_name);
        }
	}
	return dev_lru;
}


static CdDevice* CdVolume_Mount (CdVolume *cdvol, CdDevice *cddev, int verify)
{
	ASSERT(cdvol != NULL);
	
	if (cddev == NULL)
	{
		cddev = GetLruDevice();
		ASSERT(cddev != NULL);
		ASSERT(cddev->volume == NULL);
	}

	if (!cddev->is_ejectable)
	{
		if (cdvol->device != NULL)
		{
			ErrPostEx(SEV_INFO,0,0,"CdVolume_Mount;  Device busy");
			return NULL;
		}
		if (!cddev->is_inited)
		{
			if (!CdDevice_Init(cddev))  return NULL;
		}
		cdvol->device = cddev;
		cddev->volume = cdvol;
	}
	else
	{	
		while (cdvol->device == NULL)
		{
			if (verify)
			{
				if (! (*_hookInsert)(cdvol->volume_name,&cddev->inf))
					return NULL;
			}
	
			if (!cddev->is_inited)
			{
				if (CdDevice_Init(cddev) && cddev->hint == cdvol->volume_num)
				{
					cdvol->device = cddev;
					cddev->volume = cdvol;
				}
			}
			else if (verify)
			{
				char fpath[256];
				CdRomInfo  info;
				if (cddev->inf.root == NULL)
					fpath[0] = '\0';
				else
					strcpy(fpath,cddev->inf.root);
				if (cddev->ins_volname)
					FileBuildPath(fpath,cdvol->volume_name,NULL);
				if (!CdTestPath(fpath,&info))
				{
					ErrLogPrintf("CdTestPath [%s] FAILED\n",fpath);
					ErrPostEx(SEV_INFO,0,0,"Unable to get CD-ROM volume info");
				}
				else
				{
					/*ErrLogPrintf("CdTestPath [%s] OK; rel %d.%d; cd %d of %d\n", fpath,
								info.rel_major, info.rel_minor, info.cd_num, info.cd_count);*/
					cddev->upper_case = info.upper_case;
					cddev->semicolon_one = info.semicolon_one;
					if (info.rel_major != _rel_major || info.rel_minor != _rel_minor)
						ErrPostEx(SEV_INFO,0,0,"The inserted CD-ROM is from the wrong release");
					else if (cdvol->volume_num == info.cd_num)
					{
						cdvol->device = cddev;
						cddev->volume = cdvol;
					}
				}
				if (cdvol->device == NULL && cddev->is_ejectable)
				{
					char volume_to_eject[32];
					volume_to_eject[0] = '\0';
					if (cddev->ins_volname)
					{
						int ct, freecds[CDVOL_MAX];
						ct = FindFloatingEntrezVolumes(freecds);
						if (ct > 0)
							sprintf(volume_to_eject,"entrez%d",freecds[0]);
					}
					else if (cdvol->volume_name != NULL)
						strcpy(volume_to_eject,cdvol->volume_name);

					if (!(*_hookEject)(volume_to_eject,&cddev->inf))
						ErrPostEx(SEV_INFO,0,0,"Unable to eject %s",cdvol->volume_name);
		        }
			}
			else
			{
				cdvol->device = cddev;
				cddev->volume = cdvol;
			}
		}
	}
	CdDevice_Touch(cdvol->device);
	return cdvol->device;
}


static int  CdVolume_Unmount (CdVolume *cdvol)
{
	CdDevice *cddev;
	
	ASSERT(cdvol != NULL);

	cddev=cdvol->device;
	if (cddev != NULL && cddev->is_ejectable)
	{
		cddev->volume = NULL;
		cdvol->device = NULL;
		CdDevice_Touch(cddev);
		return TRUE;
	}
	return FALSE;
}


static int CdVolume_IsMounted (CdVolume *cdvol)
{
	ASSERT(cdvol != NULL);
	return cdvol->device != NULL;
}


static int FindFloatingEntrezVolumes (int *outlist)
{
	int i, ct=0;
	char testpath[16];
	CdRomInfo info;

	for (i=0; i<_volume_ct; ++i)
	{
		if (_cdvol[i].device == NULL)
		{
			sprintf(testpath,"entrez%d",i+1);
			if (CdTestPath(testpath,&info))
			{
				VERBOSE("FindFloatingEntrezVolumes: Entrez%d found\n",info.cd_num);
				outlist[ct++] = info.cd_num;
			}
		}
	}
	return ct;
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* CdDevice Functions         Schuler 05-21-94                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static CdDevice* CdDevice_Construct (CdDevice *cddev, int number)
{
	if (cddev != NULL)
	{
		char section[32];
		char buffer[64];
	
		sprintf(section,"CdEntrez.Device.%d",number);
		cddev->inf.root = GetAppParamStr(_rcfile,section,"Root",NULL);
		cddev->inf.formal_name = GetAppParamStr(_rcfile,section,"Formal_Name",NULL);
		cddev->inf.device_name = GetAppParamStr(_rcfile,section,"Device_Name",NULL);
		cddev->inf.raw_device_name = GetAppParamStr(_rcfile,section,"Raw_Device_Name",NULL);
		cddev->inf.mount_point = GetAppParamStr(_rcfile,section,"Mount_Point",NULL);
		cddev->inf.mount_cmd = GetAppParamStr(_rcfile,section,"Mount_Cmd",NULL);
		cddev->is_ejectable = GetAppParamBoolean(_rcfile,section,"Ejectable",TRUE);
		cddev->ins_volname = GetAppParamBoolean(_rcfile,section,"Insert_Volname",FALSE);

		if (GetAppParam(_rcfile,section,"Bind",NULL,buffer,sizeof buffer))
		{
			if (StrNICmp(buffer,"entrez",6) ==0)
				cddev->bound_cd = atoi(buffer+6);
		}
	}
	return cddev;
}

static CdDevice* CdDevice_Destruct (CdDevice *cddev)
{
	if (cddev != NULL)
	{
		MemFree((void*)cddev->inf.root);
		MemFree((void*)cddev->inf.formal_name);
		MemFree((void*)cddev->inf.device_name);
		MemFree((void*)cddev->inf.raw_device_name);
		MemFree((void*)cddev->inf.mount_point);
		MemFree((void*)cddev->inf.mount_cmd);
	}
	return cddev;
}


static int CdDevice_FileBuildPath (CdDevice *cddev, char *fpath, const char *fdir, const char *fname)
{
	ASSERT(cddev != NULL);
	ASSERT(fpath != NULL);
	
	*fpath = '\0';
	if (cddev->is_inited)
	{
		char *p;
		
		if (cddev->inf.root != NULL)
			strcpy(fpath,cddev->inf.root);
		if (cddev->ins_volname)
		{
			char volname[16];
			volname[0] = '\0';
			if (cddev->volume != NULL)
				strncat(volname,cddev->volume->volume_name,sizeof volname);
			else
				sprintf(volname,"entrez%d",cddev->hint);
			if (!FileBuildPath(fpath,volname,NULL))
				return FALSE;
		}
		VERIFY(p = strchr(fpath,'\0'));
		if (!FileBuildPath(fpath,(char*)fdir,(char*)fname))
			return FALSE;
		if (cddev->upper_case)
			StrUpper(p);
		if (cddev->semicolon_one)
			strcat(p,";1");
		return TRUE;
	}
	return FALSE;	
}


static FILE * CdDevice_FileOpen (CdDevice *cddev, const char *fdir, const char *fname, const char *fmode)
{
	char fpath[256];
	char ldir[32];
	char lname[32];
	FILE *fd;
	
	ASSERT(cddev != NULL);
	
	if (cddev->volume == NULL)	
		return NULL;
	
	if (!cddev->is_inited)
	{
		if (!CdDevice_Init(cddev))
			return NULL;
	}
	if (cddev->inf.root == NULL)
		fpath[0] = '\0';
	else
		strcpy(fpath,cddev->inf.root);
	if (cddev->ins_volname)
		FileBuildPath(fpath,cddev->volume->volume_name,NULL);
	strcpy(ldir,fdir);
	strcpy(lname,fname);
	if (cddev->upper_case)
	{
		StrUpper(ldir);
		StrUpper(lname);
	}
	FileBuildPath(fpath,ldir,lname);
	if (cddev->semicolon_one)
		strcat(fpath,";1");
	if ((fd = FileOpen(fpath,fmode)) != NULL)
		CdDevice_Touch(cddev);
	return fd;
}


static int CdDevice_Init (CdDevice *cddev)
{                                    
	int cd_num;
	char fpath[256];
	CdRomInfo info;
	
	ASSERT(cddev != NULL);
	if (cddev->is_inited)
		return TRUE;

	cd_num = cddev->bound_cd;
	memset((void*)&info,0,sizeof info);
	fpath[0] = '\0';
	if (cddev->inf.root !=NULL)
		strncat(fpath,cddev->inf.root,sizeof fpath);
		
	if (cddev->ins_volname)
	{
		char volname[16];
		if (cddev->bound_cd !=0)
		{
			sprintf(volname,"entrez%d",cddev->bound_cd);
			FileBuildPath(fpath,volname,NULL);
		}
		else				
		{
			int vol_ct = (_volume_ct ==0) ? 3 : _volume_ct;
			int j;
			for (j=0; j<vol_ct; ++j)
			{
				fpath[0] = '\0';
				if (cddev->inf.root !=NULL)
					strncat(fpath,cddev->inf.root,sizeof fpath);
				sprintf(volname,"entrez%d",j+1);
				FileBuildPath(fpath,volname,NULL);
				if (CdTestPath(fpath,&info))
					break;
			}
		}
	}

	if (info.cd_num == 0)
	{
		if (!(CdTestPath(fpath,&info)))
			return FALSE;
	}
		
	cddev->upper_case = info.upper_case;
	cddev->semicolon_one = info.semicolon_one;
	cddev->hint = info.cd_num;
	cddev->is_inited = TRUE;
	CdDevice_Touch(cddev);
	return TRUE;
}


static void  CdDevice_Touch (CdDevice *cddev)
{
	time_t timestamp = time(NULL);
	ASSERT(cddev != NULL);
	cddev->timestamp = timestamp;
	if (cddev->volume != NULL)
		cddev->volume->timestamp = timestamp;
}


static int LIBCALLBACK default_CdInsertProc (const char *volname, const CdDevInfo *dev)
{
	char msg[80];
	
	ASSERT(volname != NULL);
	ASSERT(dev != NULL);
	
	sprintf(msg,"Please insert volume \"%s\"",volname);
	if (dev->formal_name)
		sprintf(strchr(msg,0)," into %s",dev->formal_name);
	if (MsgAlert(KEY_OKC,SEV_INFO,GetProgramName(),msg) == ANS_CANCEL)
		return FALSE;
	if (dev->mount_cmd != NULL)
	{
		return MountCd((char*)volname, dev->device_name, 
						dev->mount_point, dev->mount_cmd);
	}
	return TRUE;
}


static int LIBCALLBACK default_CdEjectProc (const char *volname, const CdDevInfo *dev)
{
  return EjectCd((char*)volname, dev->device_name, 
                 dev->raw_device_name, dev->mount_point, dev->mount_cmd);
}



/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* CdMap   Schuler 7-29-94                                                     */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static CdMap* CdMap_Construct (CdMap *map, int array_size)
{
	if (map != NULL)
	{   
		map->array_size = (short)array_size;
		if (array_size > 0)
			map->list = (CdMap*) MemNew((size_t)array_size * sizeof(CdMap));
	}
	return map;
}

static CdMap* CdMap_Destruct (CdMap *map)
{
	if (map != NULL)
	{
		CdMap *m;
		if (map->lnklist)
		{
			CdMap *m2;
			for (m=map->list; m; m=m2)
			{
				m2 = m->list;
				m->list = NULL;
				CdMap_Free(m);
			}
		}
		else
		{
			int i;
			for (i=0, m=map->list; i<map->array_size; ++i, ++m)
				CdMap_Destruct(m);
			MemFree((void*)map->list);
		}
		MemFree((void*)map->cd_list);
		MemFree((void*)map->user_path);
		memset((void*)map,0,sizeof(CdMap));
	}
	return map;
}


static void CdMap_ParseCdNums (CdMap *map, char *nums)
{
	int cd, n=0;
	short cd_list[8];
	char *p, *p2 = nums;
		
	ASSERT(map != NULL);
	ASSERT(nums != NULL);

	while ((p = p2) != NULL)
	{
		if ((p2 = strpbrk(p,",\10")) != NULL)
			*p2++ = '\0';
		cd = atoi(p);
		ASSERT(VOLNUM_IS_VALID(cd));
		cd_list[n++] = (short) cd;
	}
	
	if (n > 1)
	{
		map->multicd = TRUE;
		map->cd_num = n;
		map->cd_list = (short*) MemGet(n*sizeof(short),MGET_ERRPOST);
		ASSERT(map->cd_list != NULL);
		memcpy((void*)map->cd_list,(void*)cd_list,n*sizeof(short));
	}
	else
	{
		map->multicd = FALSE;
		map->cd_num = cd_list[0];
		map->cd_list = NULL;
	}
}


static CdMap* CdMap_GetChild (CdMap *map, int nkid)
{
	ASSERT(map != NULL);
	
	if (map->lnklist)
	{
		CdMap *m2;
		for (m2=map->list; m2; m2=m2->list)
		{
			if (m2->id_num == nkid)
				return m2;
		}
	}	
	else if (map->list != NULL)
	{
		return &(map->list[nkid]);
	}
	return NULL;
}


static int CdMap_GetSpecs (CdMap *map, int *cdlist, char *fdir)
{
	int count =0;

	ASSERT(map != NULL);
	ASSERT(cdlist != NULL);
	ASSERT(fdir != NULL);
	
	if (map->user_path != NULL)
		strcpy(fdir,map->user_path);
		
	if (map->cd_num != 0)
	{
		if (map->multicd)
		{
			int i;
			count = map->cd_num;
			ASSERT(map->cd_num < CDVOL_MAX);
			for (i=0; i<count; ++i)
			{
				cdlist[i] = map->cd_list[i];
			}
		}
		else
		{
			count = 1;
			*cdlist = map->cd_num;
		}
	}
	return count;
}


static void CdMap_FindPath (CdMap *map, const char *key)
{
	char fpath[256];
	if (FindPath(_rcfile,"CdEntrez.Paths",(char*)key,fpath,sizeof fpath))
	{
		map->user_set = TRUE;
		map->user_path = StrSave(fpath);
	}
}




static Boolean ReadCdLayout (FILE *fd)
{                                              
	CdEntrezDir dir;
	char line[64], key[32];
	int i,j;
		
	for (dir=CdDir_FIRST; dir<=CdDir_LAST; ++dir)
	{
		CdMap_FindPath(&_map[dir],_dir[dir].key);
		if (dir == CdDir_sys)
			CdMap_Construct(&_map[dir],0);
		else
		{
			CdMap_Construct(&_map[dir],_type_ct);
			for (i=0; i<_type_ct; ++i)
			{
				CdMap_Construct(&_map[dir].list[i],0);
				sprintf(key,"%s-%s",_dir[dir].key,TYPTAG(i));
				CdMap_FindPath(&_map[dir].list[i],key);
			}
		}
	}
	
	while (UnixFileGets(line,sizeof line,fd))
	{
		if (isalpha(line[0]))
		{
			char *pt, *pd, *pn;
			CdMap *map = NULL;

			VERIFY((pn = strchr(line,'\t')) != NULL);
			*pn++ = '\0';   /* pn points to the number(s) */
			
			if ((pt = strchr(line,'-')) != NULL)
				*pt++ = '\0';    /* pt points to the type tag */

			for (dir=CdDir_FIRST; dir<=CdDir_LAST; ++dir)
			{
				if (strcmp(line,_dir[dir].key)==0)
					break;
			}
			ASSERT(dir <= CdDir_LAST);
			map = &_map[dir];

			if (pt != NULL)
			{
				if ((pd = strchr(pt,'-')) != NULL)
					*pd++ = '\0';   /* pd points to the division tag */

				for (i=0; i<_type_ct; ++i)
				{
					if (strcmp(pt,TYPTAG(i)) ==0)
						break;
				}
				ASSERT(i < _type_ct);
				map = &(map->list[i]);
				
				if (pd != NULL)
				{
					/* lookup division tag */
					for (j=0; j<_div_ct; ++j)
					{
						if (strcmp(pd,DIVTAG(j)) ==0)
							break;
					}
					ASSERT(j < _div_ct);
					if (_cdinfo->div_info[j].docs[i] != 0)
					{
						CdMap *m2;
						
						/* create node and link into list */
						map->lnklist = TRUE;
						m2 = CdMap_New(0);
						m2->id_num = j +1;
						m2->list = map->list;
						map->list = m2;
						map = m2;
						sprintf(key,"%s-%s-%s",_dir[dir].key,TYPTAG(i),DIVTAG(j));
						CdMap_FindPath(map,key);
					}
				}
			}
			CdMap_ParseCdNums(map,pn);
		}
	}
	
	if (_map[CdDir_sys].cd_num == 0)
	{
		/* The list of filenames and filesizes for the sysinfo directory
			was inadvertently omitted from the cdlayout.inf file on the 
			sample CD-ROMs distributed to developers.  This section of
			code should never be executed in the final release. */

		VERBOSE("   HACK (faking cdlayout.inf information for SYSINFO directory)\n");
		_map[CdDir_sys].cd_list = (short*) MemNew(_volume_ct * sizeof(short));
		for (i=0; i<_volume_ct; ++i)
		{
			_map[CdDir_sys].cd_list[i] = i +1;
		}
		_map[CdDir_sys].cd_num = _volume_ct;
		_map[CdDir_sys].multicd = TRUE;
	}
	
	return TRUE;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* LSet (similar to LinkSet)  Schuler 06-01-94                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
#define FLUFF 2

static LSet* LSet_Construct (LSet *lset, short sorc_type, short dest_type, int link_max)
{
	if (lset != NULL)
	{
		lset->link = (DocLink*) Malloc(FLUFF*sizeof(DocLink));
		if (lset->link == NULL)
		{
			MemFree((void*)lset);
			return NULL;
		}
		lset->slots = FLUFF;
		lset->sorc_type = sorc_type;
		lset->dest_type = dest_type;
		lset->link_max = link_max;
	}
	return lset;
}

static LSet * LSet_Destruct (LSet *lset)
{
	if (lset != NULL)	
		Free((void*)lset->link); 
	return lset;
}


static int LSet_Read (LSet *lset, FILE *fd)
{
	unsigned short m[TYPE_MAX];
	DocUid uid;
	int link_ct, max_ct;
	int i, j, k, wt;
	DocLink *link, *p1, *p2, *p3;
	
	ASSERT(lset != NULL);
	link = lset->link;
	ASSERT(link != NULL);

	/* read the link counts */
	FileReadSwapShort(m,_type_ct,fd);
	link_ct = m[lset->dest_type];
	max_ct = MIN(lset->link_max,lset->count+link_ct);

	/* grow the array, if necessary */
	if (max_ct > lset->slots)
	{
		int cd3_slots = max_ct + FLUFF;
		size_t bytes = sizeof(DocLink) * cd3_slots;
		void *array = Realloc((void*)link,bytes);
		if (array == NULL)
		{
			ErrPostEx(SEV_FATAL,0,0,"Out of memory");
			return FALSE;
		}
		lset->link = link = (DocLink*)array;
		lset->slots = max_ct + FLUFF;
	}
	
	/* skip over link lists for other types that preceed this one */
	for (i=0; i<lset->dest_type; ++i)
	{
		fseek(fd,4*m[i],SEEK_CUR);
		if (i==lset->sorc_type)
			fseek(fd,m[i],SEEK_CUR);
	}

	/* process links, one at a time */	
	for (i=0; i<link_ct; ++i)
	{
		/* read next link from the file */
		/* NEED TO CHECK FOR ERROR HERE! */
		FileReadSwapInt4((Uint4*)&uid,1,fd);
		wt = (lset->sorc_type == lset->dest_type) ? fgetc(fd) : 1;
		if (lset->count == lset->link_max)  continue;
		
		/* insert into array, keeping sorted by UID and summing weights */
		for (j=0, p1=link; j<lset->count; ++j, ++p1)
		{
			if (p1->uid == uid)
				break;
			if (p1->uid > uid)
			{
				p2 = link + lset->count;
				p3 = p2 -1;
				for (k=j; k<lset->count; ++k)
					*p2-- = *p3--;
				break;
			}
		}                
		if (j==lset->count || p1->uid != uid)
		{
			p1->wt = 0;
			lset->count ++;
		}
		p1->uid = uid;
		p1->wt += wt;
	}
	return TRUE;
}

static int LIBCALLBACK linkcmp (VoidPtr ptr1, VoidPtr ptr2);
/*
static int linkcmp (const void *ptr1, const void *ptr2);
*/

static LinkSet* LSet_Convert (LSet *lset)
{
	LinkSet *lnkset = NULL;
	ASSERT(lset != NULL);
	if ((lnkset = (LinkSet*) MemNew(sizeof(LinkSet))) != NULL)
	{
		int i;
		DocLink *p = lset->link;
		
		/*
		qsort((void*)p,lset->count,sizeof(DocLink),linkcmp);
		*/
		HeapSort ((VoidPtr) p,lset->count,sizeof(DocLink),linkcmp);
		lnkset->num = lset->count;
		lnkset->uids = (Int4*) MemGet(sizeof(Int4)*lset->count, MGET_ERRPOST);
		lnkset->weights = (Int4*) MemGet(sizeof(Int4)*lset->count, MGET_ERRPOST);
		for (i=0; i<lset->count; ++i, ++p)
		{
			lnkset->uids[i] = p->uid;
			lnkset->weights[i] = p->wt;
		}
		LSet_Free(lset);		
	}
	return lnkset;
}

static int LIBCALLBACK linkcmp (VoidPtr ptr1, VoidPtr ptr2)
/*
static int linkcmp (const void *ptr1, const void *ptr2)
*/
{
	DocLink *lnk1 = (DocLink*)ptr1;
	DocLink *lnk2 = (DocLink*)ptr2;
	int d;
	
	if ((d = lnk2->wt - lnk1->wt) ==0)
		d = (lnk1->uid > lnk2->uid) ? -1 : 1;
	return d;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* HuffTable Functions.   Schuler 06-13-94                                      */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static HuffTable* HuffTable_Construct (HuffTable *huff, int n)
{
	if (huff != NULL)
	{
		huff->count = n;
		if ( ((huff->left = (short*)MemGet(sizeof(short)*n,MGET_ERRPOST)) != NULL)
				&& ((huff->right= (short*)MemGet(sizeof(short)*n,MGET_ERRPOST)) != NULL) )
			return huff;
		huff = NULL;
	}
	return huff;
}

static HuffTable* HuffTable_Destruct (HuffTable *huff)
{
	if (huff != NULL)
	{
		MemFree((void*)huff->right);
		MemFree((void*)huff->left);
		memset((void*)huff,0,sizeof(HuffTable));
	}
	return huff;
}

static HuffTable* HuffTable_Read (FILE *fd)
{
	short n;
	if (FileReadSwapShort((unsigned short*)&n,1,fd))
	{
		HuffTable *huff = HuffTable_New(n);
		if (huff != NULL)
		{
			if ( (FileReadSwapShort((unsigned short*)huff->left,n,fd) == n)
						&& FileReadSwapShort((unsigned short*)huff->right,n,fd) == n )
			{
				return huff;
			}
			HuffTable_Free(huff);
		}
	}
	return NULL;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* DecompInfo Functions     Schuler 06-21-94                                   */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static DecompInfo* DecompInfo_Construct (DecompInfo *info, HuffTable *huff)
{
	if (info != NULL)
		info->huff = huff;
	return info;
}

static AsnIo* DecompInfo_Attach (DecompInfo *info, FILE *fd)
{
	AsnIo *aio = AsnIoNew(ASNIO_BIN_IN,fd,info,DecompReadProc,NULL);
	if (aio != NULL)
	{
		info->aio = aio;
		info->fd = fd;
		return aio;
	}
	return NULL;
}

static FILE* DecompInfo_Detach (DecompInfo *info)
{
	FILE *fd = info->fd;
	info->fd = NULL;
	info->aio->fp = NULL;
	AsnIoClose(info->aio);
	info->aio = NULL;
	return fd;
}

static Int2 LIBCALLBACK DecompReadProc (void *opaque, char *buff, Uint2 count)
{
	DecompInfo *dcp = (DecompInfo*)opaque;
	register unsigned int mask = dcp->mask;
	register unsigned int byte = dcp->byte;
	char *p = buff;
	int i, cnt = 0;
	int c;
	int k;
	FILE *fd1 = dcp->fd;
	int sentinel = dcp->huff->count;

	while (cnt < (int) count)
	{
		for (i=0; i>=0; )
		{
			if (mask == 0)
			{
				if ((c = fgetc(fd1)) == EOF)
				{
					/* should never reach this point */
					ErrPostEx(SEV_INFO,0,0,"Unexpected EOF");
					i = sentinel - 257;
					break;
				}
				else
				{
					byte = (unsigned int) c;
					mask = 0x80;
				}
			}

			if (byte & mask)
				i = dcp->huff->left[i];
			else
				i = dcp->huff->right[i];

			mask >>= 1;
		}

		if ((k = i + 257) == sentinel)
		{
			mask = 0; /* to skip remaining bits in current byte */
			break;
		}

		*p++ = (char) k;
		cnt++;
	}

	dcp->mask = mask;
	dcp->byte = byte;
	return cnt;
}

/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
* CACHE FUNCTIONS.   Schuler 05-17-94
*
* Functions for caching arbitrary pages of data.  Pages are uniquely
* identified by a long integer.
*
* MODIFICATIONS
* When      Who       What
* --------  --------  ------------------------------------------------------
* 06-01-94  Schuler  Added new argument to Cache_New() that is a pointer to
*                    a function to be called to free cached data items.
* 
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static CachePage * Cache_FindPage (Cache *cache, long id, int *index);
static void Cache_Delete_ByIndex (Cache *cache, int index);
static void Cache_Touch_ByIndex (Cache *cache, int index);
static int Cache_DeleteLRU (Cache *cache);

#define Cache_ISVALID(x)  ((x) != NULL && (x)->magic == Cache_MAGIC_VALUE )

static int Cache_IsValid (Cache *cache)
{
	return Cache_ISVALID(cache);
}

static void PASCAL DefCacheDataFreeProc (void *data)
{
	MemFree(data);
}

static Cache* Cache_Construct (Cache *cache, int slots, CacheDataFreeProc fproc)
{
	if (cache != NULL)
	{
		size_t bytes = (size_t)slots * sizeof(CachePage);
		CachePage *p = (CachePage*) MemGet(bytes,MGET_ERRPOST);
		if (p == NULL)
		{
			cache = NULL;
		}
		else
		{
			cache->magic = Cache_MAGIC_VALUE;
			cache->page = p;
			cache->page_slots = slots;
			cache->fproc = fproc;
		}
	}
	return cache;
}

static Cache* Cache_Destruct (Cache *cache)
{
	if (cache != NULL)
	{
		int i;
		CacheDataFreeProc fproc;
		
		ASSERT(Cache_ISVALID(cache));
	
		if ((fproc = cache->fproc) != NULL)
		{
			for (i=0; i<cache->page_count; ++i)
				(*fproc)(cache->page[i].data);
		}
		(void)MemFree((void*)cache->page);
		cache->magic = 0;
	}
	return cache;
}

static CachePage * Cache_FindPage (Cache *cache, long id, int *index)
{
	CachePage *p = cache->page;
	int i;
	for (i=0; i<cache->page_count; ++i, ++p)
	{
		if (p->id == id)
		{
			if (index != NULL)
				*index = i;
			return p;
		}
	}
	return NULL;
}

static int Cache_Insert (Cache *cache, long id, void *data)
{
    int i;
    
	ASSERT(Cache_ISVALID(cache));

	if ((Cache_FindPage(cache,id,NULL)) != NULL)
	{
		ErrPostEx(SEV_INFO,0,0,"Cache_Insert;  Duplicate IDs");
		return FALSE;
	}
	
	if (cache->page_count == cache->page_slots)
	{
		if (!Cache_DeleteLRU(cache))
		{
			ErrPostEx(SEV_INFO,0,0,"Cache_Insert;  All pages locked");
			return FALSE;
		}
	}
	
	i = cache->page_count;
	cache->page[i].id = id;
	cache->page[i].lock = 0;
	cache->page[i].data = data;
	cache->page_count ++;
	if (i > 0)
		Cache_Touch_ByIndex(cache,i);
	return TRUE;
}

static void Cache_Delete_ByIndex (Cache *cache, int index)
{
	CacheDataFreeProc fproc = cache->fproc;
	CachePage temp = cache->page[index];
	int i;
	cache->page_count--;
	for (i=index; i<cache->page_count; ++i)
	{
		cache->page[i] = cache->page[i+1];
	}
	if (fproc != NULL)
		(*fproc)(temp.data);
}


static int Cache_Delete (Cache *cache, long id)
{
	int index;
	ASSERT(Cache_ISVALID(cache));
	if (Cache_FindPage(cache,id,&index) != NULL)
	{
		Cache_Delete_ByIndex(cache,index);
		return TRUE;
	}
	return FALSE;
}


static int Cache_DeleteLRU (Cache *cache)
{
	int i;
	for (i=cache->page_count-1; i>=0; --i)
	{
		if (cache->page[i].lock ==0)
		{
			Cache_Delete_ByIndex(cache,i);
			return TRUE;
		}
	}
	return FALSE;
}


static void* Cache_Lock (Cache *cache, long id)
{
	CachePage *page;
	
	ASSERT(Cache_ISVALID(cache));
	if ((page = Cache_FindPage(cache,id,NULL)) != NULL)
	{
		page->lock++;
		cache->hits++;
		return page->data;
	}
	cache->misses++;
	return NULL;
}

static int Cache_Unlock (Cache *cache, long id)
{
	CachePage *page;
	int index;
	
	ASSERT(Cache_ISVALID(cache));
	if ((page = Cache_FindPage(cache,id,&index)) != NULL)
	{
		if (page->lock > 0)
		{	
			page->lock--;
			if (index > 0)
				Cache_Touch_ByIndex(cache,index);
			return TRUE;
		}
		ErrPostEx(SEV_INFO,0,0,"Cache_Unlock;  Page(%ld) was not locked",id);
	}
	return FALSE;
}

static int Cache_Touch (Cache *cache, long id)
{
	int index;
	ASSERT(Cache_ISVALID(cache));
	if (Cache_FindPage(cache,id,&index) != NULL)
	{
		Cache_Touch_ByIndex(cache,index);
		return TRUE;
	}
	return FALSE;
}

static void Cache_Touch_ByIndex (Cache *cache, int index)
{
	CachePage temp = cache->page[index];
	int i;
	for (i=index; i>0; --i)
	{
		cache->page[i] = cache->page[i-1];
	}
	cache->page[0] = temp;
}

static void* Cache_Peek (Cache *cache, long id)
{
	CachePage *page;
	
	ASSERT(Cache_ISVALID(cache));
	if ((page = Cache_FindPage(cache,id,NULL)) != NULL)
	{
		return page->data;
	}
	return NULL;
}


static void Cache_Purge (Cache *cache)
{
	ASSERT(Cache_ISVALID(cache));
	
	while (Cache_DeleteLRU(cache))
		/* empty statement */ ;
}


/**** get rid of this function!  use Cache_ReportStats instead ****/
static void  Cache_LogStats (Cache *cache, const char *name)
{
	long total;
	int pct1, pct2;
	
	ASSERT(Cache_ISVALID(cache));
	total = cache->hits + cache->misses;
	pct1 = (total==0) ? 0 : (int)((cache->hits*100L)/total);
	pct2 = (total==0) ? 0 : (int)((cache->misses*100L)/total);
	
	VERBOSE("\n   Cache Statistics: %s\n",name);
	VERBOSE("      %d slots, %d of them currently occupied\n",
				cache->page_slots, cache->page_count);
	VERBOSE("      %ld hits   (%d%%)\n",cache->hits,pct1);
	VERBOSE("      %ld misses (%d%%)\n",cache->misses,pct2);
	VERBOSE("      %ld total access attempts\n", total);
}


#define LONGDIV(x,y) (long)((0.5 + (double)(x)/(double)(y)))
#define PERCENT(x,y) ((y)==0) ? 0 : (int)LONGDIV((x)*100L,(y))

static char * Cache_ReportStats (Cache *cache, char *buffer)
{
	char *p = buffer;
	
	ASSERT(buffer != NULL);
	
	if (Cache_ISVALID(cache))
	{
		long total = cache->hits + cache->misses;
		if (cache->page_size != 0)
		{
			int kbytes_total, kbytes_used;
			kbytes_total = (int) LONGDIV(cache->page_size*cache->page_slots, KBYTE);
			sprintf(p=strchr(p,0),"   cache memory:   %5d K\n", kbytes_total);
			kbytes_used = (int) LONGDIV(cache->page_size*cache->page_count, KBYTE);
			sprintf(p=strchr(p,0),"   memory in use:  %5d K (%d%%)\n", kbytes_used,
						PERCENT(kbytes_used,kbytes_total));
		}
		sprintf(p=strchr(p,0),"   cache hits:     %5ld   (%d%%)\n",
					cache->hits, PERCENT(cache->hits,total));
		sprintf(p=strchr(p,0),"   cache misses:   %5ld   (%d%%)\n",
					cache->misses, PERCENT(cache->misses,total));
		sprintf(p=strchr(p,0),"   total attempts: %5ld\n",total);
	}
	return strchr(p,0);
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* Error reporting functions                                                    */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static int InvalidConfiguration (int code)
{
#ifdef _OLD_CdEntrez_
	ErrSev sev = SEV_INFO;
#else
	ErrSev sev = SEV_ERROR;
#endif
	ErrPostEx(sev,ERR_ConfigFile,code,"CdEntrez module is not configured correctly.  "
				"Please run EntrezCf to correct the problem.");
	return FALSE;
}

static int FileOutOfDate (const char *fname)
{
	ErrPostEx(SEV_ERROR,ERR_BadFile,SUB_BadVersion,
				"The file %s does not come from the expected "
				"Entrez version (%d.%d).",fname,_rel_major,_rel_minor);
	return FALSE;
}

static int FileCorrupt (const char *fname)
{
	ErrPostEx(SEV_ERROR,ERR_BadFile,SUB_Corrupt,
				"The file %s appears to be corrupted",fname);
	return FALSE;
}

static int FileNotRecognized (const char *fname)
{
	ErrPostEx(SEV_ERROR,ERR_BadFile,SUB_NeedUpdate,
				"The file %s cannot be read by this release of "
				"the software.  Please obtain a newer version.", fname);
	return FALSE;
}

static int CatastrophicFailure (int code)
{
	ErrPostEx(SEV_ERROR,ERR_DeepDooDoo,code,
				"Catastrophic Failure in CdEntrez module");
	return FALSE;
}


/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/
/* Misc. Utility Functions.   Schuler 05-16-94                                 */
/*-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-*/

static char * _GetAppParamStr (const char *filebase, const char *section, 
				const char *key, const char *dflt)
{
	char buffer[256];
	GetAppParam((char*)filebase,(char*)section,(char*)key,
					(char*)dflt,buffer,sizeof buffer);
	return buffer[0] ? StrSave(buffer) : NULL;
}


static int FileReadSwapShort (unsigned short *buffer, int count, FILE *fd)
{
	int n;
	if (sizeof(short) != 2)
	{
		unsigned short *ptr, val;
		int i, c;
	
		for (ptr=buffer, n=0; n<count; ++n)
		{
			for (i=0, val=0; i<2; ++i)
			{
				if ((c = fgetc(fd)) ==EOF)  
					break;
				val <<= 8;
				val |= (unsigned short)c;
			}
			if (i<2)
				break;
			*ptr++ = val;
		}
	}
	else
	{
		n = FileRead((void*)buffer,sizeof(short),count,fd);
		SwapUint2Buff(buffer,n);
	}
	return n;
}


static int FileReadSwapLong (unsigned long *buffer, int count, FILE *fd)
{
	int n;
	if (sizeof(long) != 4)
	{
		unsigned long *ptr, val;
		int i, c;
	
		for (ptr=buffer, n=0; n<count; ++n)
		{
			for (i=0, val=0; i<4; ++i)
			{
				if ((c = fgetc(fd)) ==EOF)  
					break;
				val <<= 8;
				val |= (unsigned long)c;
			}
			if (i<4)
				break;
			*ptr++ = val;
		}
	}
	else
	{
		n = FileRead((void*)buffer,sizeof(long),count,fd);
		SwapLongBuff(buffer,n);
	}
	return n;
}


static int FileReadSwapInt4 (Uint4Ptr buffer, int count, FILE *fd)
{
	int n;
	if (sizeof(Uint4) != 4)
	{
		Uint4 *ptr, val;
		int i, c;
	
		for (ptr=buffer, n=0; n<count; ++n)
		{
			for (i=0, val=0; i<4; ++i)
			{
				if ((c = fgetc(fd)) ==EOF)  
					break;
				val <<= 8;
				val |= (Uint4)c;
			}
			if (i<4)
				break;
			*ptr++ = val;
		}
	}
	else
	{
		n = FileRead((void*)buffer,sizeof(Uint4),count,fd);
		SwapUint4Buff(buffer,n);
	}
	return n;
}


static char * FileReadStr (FILE *fd, int lbyte)
{
	unsigned short len;
	
	if (lbyte == 2)
		FileReadSwapShort(&len,1,fd);
	else
		len = (unsigned short) fgetc(fd);
	
	if (len > 0)
	{
		char *str = MemGet(1+len,MGET_ERRPOST);
		if (str != NULL)
		{
			if (fread(str,1,len,fd) != (size_t)len)
			{
				MemFree((void*)str);
				ErrPostEx(SEV_INFO,0,0,"Unexpected EOF");
			}
			else
			{
				*(str+len) = '\0';
				return str;
			}
		}
	}
	return NULL;
}


#endif 

