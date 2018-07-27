/* 
 * ===========================================================================
 *
 *                             COPYRIGHT NOTICE
 *               National Center for Biotechnology Information
 *
 *  This software/database is a "United States Government Work" under the
 *  terms of the United States Copyright Act.  It was written as part of
 *  the author's official duties as a Government employee and thus cannot
 *  be copyrighted.  This software/database is freely available to the
 *  public for use without a copyright notice.  Restrictions cannot be
 *  placed on its present or future use.
 *
 *  Although all reasonable efforts have been taken to ensure the accuracy
 *  and reliability of the software and data, the National Library of
 *  Medicine (NLM) and the U. S. Government do not and cannot warrant the
 *  performance or results that may be obtained by using this software or
 *  data.  The NLM and the U. S. Government disclaim all warranties as to
 *  performance, merchantability or fitness for any particular purpose.
 *
 *  Please see that the author is suitably cited in any work or product
 *  based on this material.
 *
 * ===========================================================================
 *
 * RCS $Id: cdromlib.c,v 6.1 2004/04/01 13:43:05 lavr Exp $
 *
 * Authors: Greg Schuler, Jim Ostell, Jonathan Kans, Jonathan Epstein
 * 
 * Original Creation Date:   9-91
 *
 * File Description: 
 *  Functions in this file are the I/O primitives needed to retrieve data
 *  from the Entrez CD-ROMs, releases 12.0 and earlier.  For releases 13.0
 *  and later, use the functions in cdnewlib.c
 *
 *
 * Modifications:
 * --------------------------------------------------------------------------
 * Date     Name        Description of modification
 * -------  ----------  -----------------------------------------------------
 * 06-29-94 Schuler     AsnRead/AsnWrite functions moved to objentr.c
 * 07-11-94 Schuler     Removed #include <cdrom.h> (no longer needed!)
 * 07-13-94 Schuler     Moved CdTermFree to cdentrez.c
 * 08-04-94 Kans        Fixed bug resulting in reading too many term pages
 * 11-16-94 Schuler     Typecasts for picky compilers
 *
 * 05-19-95 Schuler     Added rcs Log directive for automatic insertion of
 *                      modification comments.
 *
 * Revision $Log: cdromlib.c,v $
 * Revision Revision 6.1  2004/04/01 13:43:05  lavr
 * Revision Spell "occurred", "occurrence", and "occurring"
 * Revision
 * Revision Revision 6.0  1997/08/25 18:13:10  madden
 * Revision Revision changed to 6.0
 * Revision
 * Revision Revision 5.0  1996/05/28 13:55:34  ostell
 * Revision Set to revision 5.0
 * Revision
 * Revision 4.0  1995/07/26  13:50:32  ostell
 * force revision to 4.0
 *
 * Revision 2.48  1995/05/16  14:36:20  schuler
 * Automatic comment insertion enabled
 *
 *
 * ==========================================================================
 */

#define REVISION_STR "$Revision: 6.1 $"

#include <cdromlib.h>

#ifdef _OLD_CdEntrez_

static char * _this_module = "CdEntrez";
#undef  THIS_MODULE
#define THIS_MODULE _this_module
static char * _this_file = __FILE__;
#undef  THIS_FILE
#define THIS_FILE _this_file

/*  =========================================================================
 *      CONSTANTS & MACROS
 */

#define CURRENT_FORMAT_VERSION 0

#define BLKSIZE   ((size_t)vi->field_bucket_size)  /* sizeof term index block on cdrom */

#define PREF_ML     TYP_ML
#define PREF_AA     TYP_AA
#define PREF_NT     TYP_NT
#define PREF_MED    (NTYPE+0)
#define PREF_SEQ    (NTYPE+1)

#define SUF_ML      TYP_ML
#define SUF_AA      TYP_AA
#define SUF_NT      TYP_NT
#define SUF_WORD    (NTYPE+FLD_WORD)
#define SUF_MESH    (NTYPE+FLD_MESH)
#define SUF_KYWD    (NTYPE+FLD_KYWD)
#define SUF_AUTH    (NTYPE+FLD_AUTH)
#define SUF_JOUR    (NTYPE+FLD_JOUR)
#define SUF_ORGN    (NTYPE+FLD_ORGN)
#define SUF_ACCN    (NTYPE+FLD_ACCN)
#define SUF_GENE    (NTYPE+FLD_GENE)
#define SUF_PROT    (NTYPE+FLD_PROT)
#define SUF_ECNO    (NTYPE+FLD_ECNO)
#define SUF_HIER    (NTYPE+FLD_ORGN_HIER)
#define SUF_DATE    (NTYPE+FLD_DATE)
#define SUF_ASN     (NTYPE+NFLD)
#define SUF_REC     (NTYPE+NFLD+1)
#define SUF_UID     (NTYPE+NFLD+2)

#define EXT_DAT     0
#define EXT_IDX     1
#define EXT_LST     2
#define EXT_PST     3
#define EXT_LNK     4

#define COMPR_NONE	0
#define COMPR_HUFFMAN	1
#define COMPR_LZW1	2
/* etc...*/
#define COMPR_DONT_KNOW	0xFF


#define HUFFMAN_SENTINEL 256


typedef struct {
    DocUid  uid;                  /* MEDLINE UI or seq-id of Bioseq       */
    DocType type;                  /* document type code (ml/nt/aa)        */
    Int4  entry_offset ,        /* offset into entry file(s)            */
        sum_offset ,            /* offset into summary file    */
        link_offset ;            /* offset into link file       */
} UidIdx,  PNTR UidIdxPtr;

typedef struct decompinfo {
    AsnIoPtr aip;
    FILE *fp;
    Uint1 compr;	/* compression protocol */
    unsigned int mask;		/* used internally for Huffman */
    unsigned int byte;		/* used internally for Huffman */
    Uint4 bytes_left;   /* count of remaining bytes for uncompressed protocol */
    struct decompinfo PNTR next;
} DecompInfo, PNTR DecompInfoPtr;
    

/*  =========================================================================
 *      VARIABLES
 */

static DecompInfoPtr DecompInfoList = NULL;
static Int4 numinits;
static CharPtr buffer;

static CharPtr sPath [NDIR];

static char  *sSdir[] = { "", "data", "sequence", "medline", "terms",
                          "index", "links", "" };
static char  *sPref[] = { "ml", "aa", "nt", "med", "seq" };
static char  *sSuff[] = {  "word", "mesh", "kywd", "auth", "jour", "orgn",
                           "accn", "gene", "prot", "ecno", "hier", "date",
                           "fkey", "prop", "subs", "mloc",
                           "ml", "aa", "nt",
                           "asn", "rec", "uid" };
static char  *sExtn[] = { "dat", "idx", "lst", "pst", "lnk" };

static Boolean bAppendVer = FALSE;
static Boolean upperCaseIt = FALSE;

static EntrezInfoPtr vi = NULL;
static Int4Ptr type_bucket_index[NTYPE];  /* from the .idx files */

static size_t detInfoCharCount;
static CharPtr CdDetailedBuf = NULL;
static Boolean countOnly;

                    /* for saving the last term.idx file used */

static DocType term_idx_type = -1;   
static DocField term_idx_field = -1;
static Int2 term_idx_count = 0;
static CharPtr PNTR term_idx_str;
static FILE *IdxFilePtr[NTYPE+2];
static Boolean HoldIdxOpen = FALSE;


#ifdef IS_BIG_ENDIAN
/* no swapping needed:  define do-nothing macros */
#define SwapInt2(X)  (X)
#define SwapInt4(X)  (X)
#else
/* give prototypes for byte swapping functions */
static Int2 NEAR  SwapInt2  PROTO((Int2));
static Int4 NEAR  SwapInt4  PROTO((Int4));
#endif


/*****************************************************************************
*
*   Private Function Prototypes
*
*****************************************************************************/
static Boolean NEAR CdInitialize PROTO((CharPtr,CharPtr,CharPtr,Int2Ptr));
static Boolean NEAR CdSetPath    PROTO((Int2,CharPtr));
static Boolean NEAR SaveCdMediaContext PROTO((CharPtr media_name));
static void    NEAR ExtraInitWork PROTO((void));
static Boolean NEAR ValidateType PROTO((DocType type));
static Boolean NEAR ValidateField PROTO((DocType type, DocField field));
static Boolean NEAR ValidateUid PROTO((DocType type, DocUid uid));
static CharPtr NEAR MakePath PROTO((Int2 nSdir,Int2 nPref,Int2 nSuf, Int2 nExtn));
static Boolean NEAR LoadUidIndex PROTO((DocType type));
static Int2    NEAR LoadTrmIndex PROTO((DocType type, DocField field));
static void    NEAR FreeTrmIndex PROTO((void));
/**** not used in reading cdrom ******
static Int4 NEAR MergeSegOffset PROTO((Int2 seg, Int4 offset));
*************************************/
static Boolean NEAR SplitSegOffset PROTO((Int4 value, Int2Ptr segptr, Int4Ptr offsetptr));
static FILE * NEAR CdDocFil PROTO((DocType type, DocUid uid, UidIdxPtr idx));

static Boolean SwapOutCd PROTO((VoidPtr med));
static Boolean SwapInCd PROTO((VoidPtr med));
static void NEAR ForceCdFini PROTO((void));
static Boolean CdInitMedia PROTO((VoidPtr med));
static Boolean CdFmtInfo PROTO((VoidPtr medName));

static CdTermPtr NEAR CdTrmLocate PROTO((CharPtr term, Int2 page));
static UidIdxPtr NEAR UidIdxGet PROTO((DocType type, DocUid uid, UidIdxPtr idx));
static void NEAR linksort PROTO((Int4Ptr uids, Int4Ptr wts, Int4 n));
static DecompInfoPtr NEAR DecompInit PROTO((FILE *fp));
static Boolean NEAR DecompFini PROTO((AsnIoPtr aip, DecompInfoPtr dip));
static void NEAR DecompInfoFree PROTO((DecompInfoPtr dcp));
static Int2 LIBCALLBACK DecompReadFunc PROTO((Pointer p, CharPtr buff, Uint2 count));
static Int2 HuffmanRead PROTO((DecompInfoPtr dcp, CharPtr buff, Uint2 count));
static Boolean NEAR IsOKMagic PROTO((Uint4 magic, CharPtr volume_label));
static CdTermPtr  CdTermRead PROTO((Int2 type, Int2 field, CharPtr ptr, CharPtr bufr, Int2 page));

/*****************************************************************************
*
*   General purpose public functions
*
*****************************************************************************/

/*****************************************************************************
*
*   CdInit()
*
*****************************************************************************/
static CharPtr trmbuf;   /* for term pages */
static DocType trmtype;  /* type of last term used in trmbuf */
static DocField trmfield;  /* field of last term used in trmbuf */
static Int2 trmpage,       /* page # of first page in trmbuf */
			trmpages; 			/* number of pages in memory */
static size_t trmpagesrequest;     /* how bytes to read (5 * BLKSIZE) */

static Boolean oldStyleCfgFile;


static Int2 nCdVer;
static char *sCdError [] = {
    "",
    "Memory allocation error",
    "File create error",
#ifdef WIN_MSWIN
	"File open error on %Fs",
#else
    "File open error on %s",
#endif
    "File seek error",
    "File read error",
    "File write error",
    "Bad database type code [%d]",
    "Bad field code [%d]",
    "No terms for type/field [%d/%d]",
    "Bad uid number [%ld]",
    "Bad directory number [%d]",
    "Cannot read new data format",
    "Index files out of date",
    "Data decompression error",
    "Programmer error"
};

static CdTermPtr cdtrmcache [10]; /* cache of most recent CdTrmFind results */

/*****************************************************************************
*
*   CdInit()
*     uses environment variables to configure initialization
*
*****************************************************************************/

Boolean  CdInit (void)

{
    char media[64];

	ConfigInit();

    if (nCdVer) {
        numinits++;
        return TRUE;   /* already setup */
    }

	oldStyleCfgFile = FALSE;

    GetAppParam ("ncbi", "NCBI", "MEDIA", "", media, sizeof media);

	/* This is a work-around to provide backwards compatibility for old       */
	/* config files which do not specify MEDIA                                */
	if (media[0] == '\0')
	{
		StrCpy(media, "NCBI");
		SetSoleMedia();
		oldStyleCfgFile = TRUE;
	}

    return (ParseMedia(CdInitMedia, MEDIUM_CD | MEDIUM_DISK) != 0);
}


static Boolean CdInitMedia(VoidPtr med)

{
    char CdRootPath[PATH_MAX];
    char sVol[32];
	char datvalpath[PATH_MAX];
	CharPtr mediaName = (CharPtr) med;

    GetAppParam ("ncbi", mediaName, "ROOT", "", CdRootPath, sizeof CdRootPath);

    vi = NULL;

    bAppendVer = FALSE;
    upperCaseIt = FALSE;

    /* "VAL" overrides "ROOT" for purposes of finding first copy of .val */
    if (GetAppParam ("ncbi", mediaName, "VAL", CdRootPath, datvalpath, sizeof datvalpath))
        CdSetPath (DIR_VAL, CdRootPath);

    FileBuildPath(datvalpath, NULL, NULL);
    if (! CdInitialize (CdRootPath, sVol, datvalpath, &nCdVer)) {
        return  FALSE;
    }

    trmpagesrequest =(size_t)(5 * BLKSIZE);   /* number of termpages to request */

    if (GetAppParam ("ncbi", mediaName, "IDX", "", CdRootPath, sizeof CdRootPath))
        CdSetPath (DIR_IDX, CdRootPath);

    /* work-around to find alternate index files when using old-style    */
    /* configuration file                                                */
    if (oldStyleCfgFile)
    {
        if (StrICmp(sVol, "SeqData") == 0 &&
            GetAppParam ("ncbi", mediaName, "SEQIDX", "", CdRootPath,
            sizeof CdRootPath))
        {
            CdSetPath (DIR_IDX, CdRootPath);
        }
        if (StrICmp(sVol, "MedData") == 0 &&
            GetAppParam ("ncbi", mediaName, "MEDIDX", "", CdRootPath,
            sizeof CdRootPath))
        {
            CdSetPath (DIR_IDX, CdRootPath);
        }
    }

    if (GetAppParam ("ncbi", mediaName, "LNK", "", CdRootPath, sizeof CdRootPath))
        CdSetPath (DIR_LNK, CdRootPath);
    if (GetAppParam ("ncbi", mediaName, "MED", "", CdRootPath, sizeof CdRootPath))
        CdSetPath (DIR_MED, CdRootPath);
    if (GetAppParam ("ncbi", mediaName, "SEQ", "", CdRootPath, sizeof CdRootPath))
        CdSetPath (DIR_SEQ, CdRootPath);
    if (GetAppParam ("ncbi", mediaName, "TRM", "", CdRootPath, sizeof CdRootPath))
        CdSetPath (DIR_TRM, CdRootPath);
    SaveCdMediaContext(mediaName);

    return TRUE;
}


static Boolean NEAR SaveCdMediaContext(CharPtr media_name)

{
	MediaPtr media;
	CdMediaInfoPtr cdm;
	int i;
	char ejectable[10];
	char buffer[100];

	media = PreInitMedia(media_name);

	if (media == NULL)
		return FALSE;

	if (media->inited_partial || (media->media_type != MEDIUM_CD &&
		media->media_type != MEDIUM_DISK))
		return TRUE;

	media->swapOutMedia = SwapOutCd;
	media->swapInMedia = SwapInCd;
    GetAppParam ("ncbi", media_name, "EJECTABLE", "0", ejectable, sizeof ejectable);

	cdm = (CdMediaInfoPtr) MemNew(sizeof(CdMediaInfo));
	cdm->ejectable = atoi(ejectable);
	cdm->device_name = NULL;
	cdm->raw_device_name = NULL;
	cdm->mount_point = NULL;
	cdm->mount_cmd = NULL;

    if (GetAppParam ("ncbi", media_name, "DEVICE_NAME", "", buffer, sizeof buffer))
	{
    	cdm->device_name = StringSave(buffer);
	}
    if (GetAppParam ("ncbi", media_name, "RAW_DEVICE_NAME", "", buffer, sizeof buffer))
	{
    	cdm->raw_device_name = StringSave(buffer);
	}
    if (GetAppParam ("ncbi", media_name, "MOUNT_POINT", "", buffer, sizeof buffer))
	{
    	cdm->mount_point = StringSave(buffer);
	}
    if (GetAppParam ("ncbi", media_name, "MOUNT_CMD", "", buffer, sizeof buffer))
	{
    	cdm->mount_cmd = StringSave(buffer);
	}
	cdm->hold_idx_open = FALSE;
    if (GetAppParam ("ncbi", media_name, "HOLD_IDX_OPEN", "", buffer, sizeof buffer))
	{
    	cdm->hold_idx_open = StringICmp(buffer, "TRUE") == 0;
	}

	media->media_info = (VoidPtr) cdm;

	for (i = 0; i < NDIR; i++)
	{
		cdm->sPath[i] = sPath[i];
		sPath[i] = NULL;
	}

	media->entrez_info = vi;
	cdm->bAppendVer = bAppendVer;
	cdm->upperCaseIt = upperCaseIt;

	media->inited_partial = TRUE;
	
	return TRUE;
}


static Boolean SwapOutCd(VoidPtr curm)
{
	int i;
	MediaPtr CurMedia = (MediaPtr) curm;
	CdMediaInfoPtr cmip;

	if (CurMedia != NULL)
	{
		cmip = (CdMediaInfoPtr) CurMedia->media_info;
		CurMedia->entrez_info = vi;
		vi = NULL; /* avoid freeing it */

		for (i = 0; i < NDIR; i++)
		{ /* copy and avoid freeing */
			cmip->sPath[i] = sPath[i];
			sPath[i] = NULL;
		}

		ForceCdFini();
	}

	return TRUE;
}


static Boolean SwapInCd(VoidPtr med)
{
	MediaPtr newMedia = (MediaPtr) med;
	int i;
	CdMediaInfoPtr cmip;

	cmip = (CdMediaInfoPtr) newMedia->media_info;

	for (i = 0; i < NDIR; i++)
	{ /* load up sPath */
		sPath[i] = cmip->sPath[i];
	}
	vi = newMedia->entrez_info;

	bAppendVer = cmip->bAppendVer;
	upperCaseIt = cmip->upperCaseIt;
    HoldIdxOpen = cmip->hold_idx_open;

	ExtraInitWork();

	return TRUE;
}


static void NEAR ExtraInitWork()

{
    size_t bufsize;
	int i;

    /* initialize cached CdTermPtr array */
    for (i = 0; i < 10; i++) {
      cdtrmcache [i] = NULL;
    }

    term_idx_type = -1;
    term_idx_field = -1;

    if (buffer == NULL) {
        bufsize = (size_t) MAX (MAX ((size_t) vi->type_bucket_size, (size_t) vi->field_bucket_size), sizeof (Int4) * 512);
        buffer = (CharPtr) MemNew(bufsize);
    }
}


static void NEAR ForceCdFini(void)

{
	Int4 sav_numinits = numinits;
	CharPtr savDetailedBuf;

	ConfigInit(); /* simulate Init() to balance Fini() */
    savDetailedBuf = CdDetailedBuf;
	CdDetailedBuf = NULL; /* avoid freeing in Fini() */
	numinits = 1;
	CdFini();
	numinits = sav_numinits;
    CdDetailedBuf = savDetailedBuf;
}


/*****************************************************************************
*
*   CdFini()
*      closes cdromlib session
*   
*****************************************************************************/
Boolean  CdFini (void)

{
	Int2 i;
	CdTermPtr trmptr;

	ConfigFini();
	numinits--;
	if (numinits)          /* haven't fixed all initializations yet */
		return TRUE;

    /* free cached CdTermPtr array */
    for (i = 0; i < 10; i++) {
      trmptr = cdtrmcache [i];
      if (trmptr != NULL) {
        if (trmptr->term != NULL) {
          MemFree (trmptr->term);
        }
        MemFree (trmptr);
      }
      cdtrmcache [i] = NULL;
    }

    buffer = (CharPtr) MemFree(buffer);
	FreeTrmIndex();
	for (i = 0; i < NDIR; i++)
		sPath[i] = (CharPtr) MemFree(sPath[i]);
	for (i = 0; i < NTYPE; i++)
		if (i != TYP_NT)
			type_bucket_index[i] = (Int4Ptr) MemFree(type_bucket_index[i]);
		else
			type_bucket_index[i] = NULL;  /* NT and AA use same index */
	vi = EntrezInfoFree(vi);
	trmbuf = (CharPtr) MemFree(trmbuf);
	trmpages = 0;
	nCdVer = 0;
	bAppendVer = FALSE;
	upperCaseIt = FALSE;

    for (i = 0; i < NTYPE+2; i++)
	{
        if (IdxFilePtr[i] != NULL)
		{
			FileClose(IdxFilePtr[i]);
			IdxFilePtr[i] = NULL;
		}
	}

    CdDetailedBuf = (CharPtr) MemFree(CdDetailedBuf);

	return TRUE;
}


/*  =========================================================================
 *      PUBLIC FUNCTION BODIES
 */


/*  -------------------- CdInitialize() --------------------------------
 *  CdInitialize -- Initializes the library
 *
 *  Parameters:    sCdRoot:   CD-ROM root path
 *                 sVolume:   pointer to volume name buffer (VOLUME_MAX)
 *                 ver:       pointer to version number buffer
 *
 *  Return value:  TRUE:      Success.
 *                 FALSE:     Failure;  refer to error code.
 *
 *  Notes:  1. The file cdromdat.val must be in the specified root path.
 *          2. Default paths strings for various subdirectories are 
 *             created by this function below the specified root path.
 *             Use CdSetPath() to override the defaults.
 */

static Boolean NEAR CdInitialize (CharPtr sCdRoot, CharPtr sVolume, CharPtr datvalpath, Int2Ptr ver)

{
    Int2   i;
    AsnIoPtr aip;
    Char drctry [16];
    CharPtr p;
    size_t bufsize;


    *sVolume = '\0';
    *ver = 0;

	numinits++;     /* count the number of initialization calls */

    if (vi != NULL) {          /* already initialized ! */
        StringCpy (sVolume, vi->volume_label);
        *ver = vi->version;
        return  TRUE;
    }

    /* initialize cached CdTermPtr array */
    for (i = 0; i < 10; i++) {
      cdtrmcache [i] = NULL;
    }

    term_idx_type = -1;
    term_idx_field = -1;
    
    for (i = 0; i < NTYPE+2; i++)
        IdxFilePtr[i] = NULL;

    /* initialize storage for path names */
    for (i = 0; i < NDIR; i++)
        if (sPath[i] == NULL)
            sPath[i] = (CharPtr) MemNew(PATH_MAX + 1);

    /* initialize root path string variable */
    StringNCpy (sPath[DIR_ROOT], sCdRoot, PATH_MAX);
    FileBuildPath(sPath[DIR_ROOT], NULL, NULL);

    /* read the CDROMLIB.INF file */
    if ((aip = EntrezInfoOpen (datvalpath)) == NULL)
        return FALSE;

    /* set default paths for subdirectories */
    for (i=1; i<NDIR; i++) {
        StringCpy (sPath[i], sPath[DIR_ROOT]);
        StringCpy (drctry, sSdir[i]);
        if (upperCaseIt) {
          p = drctry;
          while (*p != '\0') {
            *p = TO_UPPER (*p);
            p++;
          }
        }
        FileBuildPath(sPath[i], drctry, NULL);
    }

    vi = EntrezInfoAsnRead(aip, NULL);
    AsnIoClose(aip);
    if (vi == NULL)
        return FALSE;

    /* check for incompatible format */
    if (vi->format != CURRENT_FORMAT_VERSION) {
        ErrPostEx(SEV_ERROR, ERR_CD_BADFORMAT, 0, sCdError[ERR_CD_BADFORMAT]);
        return FALSE;
    }  

    if (buffer == NULL) {
        bufsize = (size_t) MAX (MAX ((size_t) vi->type_bucket_size, (size_t) vi->field_bucket_size), sizeof (Int4) * 512);
        buffer = (CharPtr) MemNew(bufsize);
    }

    StringCpy (sVolume, vi->volume_label);
    *ver = vi->version;
    return  TRUE;
}

/*****************************************************************************
*
*   CdGetInfo()
*   	Gets Entrez info pointer
*
*****************************************************************************/
EntrezInfoPtr CdGetInfo (void)

{
	return vi;
}


/*****************************************************************************
*
*   CdFmtInfo()
*   	Formats CD-ROM specific "detailed info" and either stores the number
*   	of characters required to format the text, or concatentates the
*   	formatted string to a global string
*
*****************************************************************************/
static Boolean CdFmtInfo(VoidPtr medName)
{
  char buf[256];
  MediaPtr media;
  CharPtr mediaName = (CharPtr) medName;
  CdMediaInfoPtr cdm;


  if ((media = PreInitMedia(mediaName)) == NULL || media->invalid ||
	  (cdm = (CdMediaInfoPtr) media->media_info) == NULL)
  {
    return FALSE;
  }

  if (media->media_type == MEDIUM_CD)
    StrCpy(buf, "\n  CD-ROM image from ");
  else
    StrCpy(buf, "\n  Hard disk image from ");
  if (cdm->sPath[DIR_ROOT] == NULL)
  {
	StrCat(buf, "<location unknown>");
  } else {
    StrCat(buf, cdm->sPath[DIR_ROOT]);
  }
  if (media->entrez_info != NULL && media->entrez_info->volume_label != NULL)
  {
    StrCat(buf, "\n    Volume label is ");
    StrCat(buf, media->entrez_info->volume_label);
  }
  if (media->formal_name == NULL)
  {
    StrCat(buf, "\n    [ this medium has no formal name ]");
  }
  else {
    StrCat(buf, "\n    Formal name is ");
    StrCat(buf, media->formal_name);
  }
  StrCat(buf, "\n");

  if (countOnly)
  {
	detInfoCharCount += StringLen(buf);
  } else {
	StrCat(CdDetailedBuf, buf);
  }

  /* always return FALSE, so that ParseMedia() will refrain from setting */
  /* validity flags                                                      */
  return FALSE;
}
	 
  
/*****************************************************************************
*
*   CdDetailedInfo()
*   	Gets formatted text information about the current status, or returns
*   	NULL; the text (if any) is stored in a statically allocated buffer
*
*****************************************************************************/

CharPtr CdDetailedInfo (void)

{
  if (numinits == 0) /* not yet initialized */
  {
	if (CdDetailedBuf == NULL)
	{
	  CdDetailedBuf = StringSave("CD-ROM and HARD DISK access information is not currently available\n");
	}
	return CdDetailedBuf;
  }
  detInfoCharCount = 0;
  countOnly = TRUE;
  ParseMedia(CdFmtInfo, MEDIUM_CD | MEDIUM_DISK);
  countOnly = FALSE;
  if (detInfoCharCount == 0)
    return NULL;
  if (CdDetailedBuf != NULL)
  {
    CdDetailedBuf = (CharPtr) MemFree(CdDetailedBuf);
  }
  CdDetailedBuf = (CharPtr) MemNew(detInfoCharCount + 200);
  StrCpy(CdDetailedBuf, "CD-ROM and HARD DISK ACCESS\n");
  if (CurMediaType() == MEDIUM_CD || CurMediaType() == MEDIUM_DISK)
  {
    StrCat(CdDetailedBuf, "  Currently active medium is ");
    StrCat(CdDetailedBuf, (GetCurMedia())->formal_name);
    StrCat(CdDetailedBuf, "\n");
  }
  ParseMedia(CdFmtInfo, MEDIUM_CD | MEDIUM_DISK);
  return CdDetailedBuf;
}

/*  -------------------- CdSetPath() ---------------------------------
 */
static Boolean NEAR CdSetPath (Int2 dir, CharPtr path)

{
    int  k = 0;

	if (path != NULL)
		k = StringLen(path);

    if ((dir<2) || (dir>=NDIR) || (k==0)) {
		ErrPostEx(SEV_ERROR, ERR_CD_BADDIR, 0, sCdError[ERR_CD_BADDIR], dir);
        return FALSE;
    }
    StringCpy (sPath[dir], path);
    FileBuildPath(sPath[dir], NULL, NULL);
    return TRUE;
}

/*****************************************************************************
*
*   UidIdxGet(type, uid, idx)
*
*****************************************************************************/
static UidIdxPtr NEAR UidIdxGet (DocType type, DocUid uid, UidIdxPtr idx)

{
	Int4Ptr ip;
	Int2 i, j, l, r;
    FILE * fp;
    CharPtr path;
	struct idxrec {
		DocUid uid;
		Int4 entry_offset,
			link_offset;
	} PNTR idxptr;

	if (! ValidateUid(type, uid))
		return NULL;

	if (type == TYP_SEQ)    /* AA, NT, SEQ all the same */
		type = TYP_AA;
		
    if (type_bucket_index[type] == NULL)
	{
        if (! LoadUidIndex(type))
			return NULL;
	}

	ip = type_bucket_index[type];
	r = vi->types[type].num_bucket - 1;
	l = 0;
	j = 0;
	while ((l <= r) && (! ((ip[j] <= uid) && (ip[j+1] > uid))))
	{
		j = (l + r) / 2;
		if (uid > ip[j])
			l = j + 1;
		else
			r = j - 1;
	}
		
	if (type != TYP_ML)
		type = TYP_SEQ;

	if ((fp = IdxFilePtr[type]) == NULL)
	{
		path = MakePath (DIR_IDX, type, SUF_UID, EXT_LST);
		if ((fp = FileOpen(path, "rb")) == NULL)
		{
			ErrPostEx(SEV_ERROR, ERR_CD_FILEOPEN, 0, sCdError[ERR_CD_FILEOPEN], path);
			return NULL;
		}
	}

	fseek(fp, (long)j * (long)vi->type_bucket_size, SEEK_SET);
	
	j = FileRead(buffer, 1, vi->type_bucket_size, fp);

	if (HoldIdxOpen)
	{
		IdxFilePtr[type] = fp;
	} else {
	    FileClose(fp);
	}

	if (j == 0)
	{
		ErrPostEx(SEV_ERROR, ERR_CD_FILEREAD, 0, sCdError[ERR_CD_FILEREAD]);
		return NULL;
	}

	idxptr = (struct idxrec PNTR) buffer;
	j = vi->type_bucket_size / sizeof(struct idxrec);
	for (i = 0; i < j; i++, idxptr++)
	{
		if (uid == SwapInt4(idxptr->uid))
		{
	 	    if (idx == NULL)
       		    idx = (UidIdxPtr) MemNew(sizeof(UidIdx));
	  	    else
        		MemFill(idx, '\0', sizeof(UidIdx));

		   	idx->type = type;
		    idx->uid = uid;
			idx->entry_offset = SwapInt4(idxptr->entry_offset);
			idx->sum_offset = 0;
			idx->link_offset = SwapInt4(idxptr->link_offset);
			if (type == TYP_SEQ)
			{
				if (idx->entry_offset & 0x80000000)
					idx->type = TYP_AA;
				else
					idx->type = TYP_NT;
			}
			return idx;
		}
	}

	return NULL;
}

/*  -------------------- CdTrmPageCt() --------------------------------
 *  CdTrmPageCt -- returns the number of term pages for a type/field pair.
 *
 *  Parameters:     type:     database code.
 *                  field:    field code.
 *
 *  Return value:   non-zero:   Success;  page count.
 *                  zero:       Failure;  refer to error code.
 */

Int2    CdTrmPageCt (DocType type, DocField field)

{
    if (!ValidateType (type))  return  0;
    if (!ValidateField (type, field))  return 0;
    return (Int2)  vi->types[type].fields[field].num_bucket;
}


/*  -------------------- CdTrmLookup() --------------------------------
 *  CdTrmLookup -- returns the first page that COULD contain a term.
 *
 *  Parameters:     type:     database code.
 *                  field:    field code.
 *                  term:   term (or term fragment) to lookup.
 *
 *  Return value:   non-negative:   Success;  page number. (zero-based)
 *                  negative:       Failure;  refer to error code.
 */

Int2    CdTrmLookup (DocType type, DocField field, CharPtr term)

{
    int  i;

    if (!LoadTrmIndex (type, field))
		return(-1);

    for (i=0; i< term_idx_count; i++) {
        if (MeshStringICmp (term_idx_str[i], term) >= 0) 
            return  MAX (0,i-2);
    }
    return  MAX (0,term_idx_count-2);
}

 
/*  -------------------- CdTrmPages() ---------------------------------
 *  CdTrmPages -- fetches a range of term pages from the CD-ROM.
 *
 *  Parameters:     type:     database code.
 *                  field:    field code.
 *                  pg:     page number of first page to read.
 *                  ct:     number of pages to read.
 *                  buffer: buffer to receive the data.
 *
 *  Return value:   non-zero:   Success;  number of pages read.
 *                  zero:       Failure;  refer to error code.
 *
 *  Notes:  The term pages contain a series of variable-length term records,
 *      each of which is an ASCII string with the following structure:
 *
 *      <term>\t<c1>\t<c2>\t<offset>\n
 *
 *      term:       term
 *      c1:         count of 'special' occurrences.
 *      c2:         count of total occurrences.  ** NOTE **
 *      offset:     offset in postings file of list of document numbers.
 *      \t:         tab character  (?).
 *      \n:         newline character  ('\x0A').
 *
 *      A term record may cross a page boundary.
 */

Int2    CdTrmPages (DocType type, DocField field, Int2 pg)

{
    CharPtr path, buff;
    FILE   *fd;
    Int4    offset;
    size_t    bytes;

	if ((type == trmtype) && (field == trmfield) && (pg == trmpage) && (trmpages))
		return trmpages;

	if (trmbuf == NULL)
		trmbuf = (CharPtr) MemNew(trmpagesrequest + 2); /* allow terminating 00 */
	buff = trmbuf;   /* use local static buffer */
    /* need to fill buffer with NULL's */
    MemFill(buff, 0, trmpagesrequest + 2);
	trmpages = 0;    /* no pages loaded */

    if (!ValidateType (type))  return 0;
    if (!ValidateField (type, field))  return 0;

    path = MakePath (DIR_TRM, type, field, EXT_LST);
    if ((fd = FileOpen(path, "rb")) ==NULL)  {
		ErrPostEx(SEV_WARNING, ERR_CD_FILEOPEN, 0, sCdError[ERR_CD_FILEOPEN], path);
        return  0;
    }
    offset = (long) pg * BLKSIZE;
    fseek (fd, offset, SEEK_SET);
	bytes = FileRead(buff, 1, trmpagesrequest, fd);
    FileClose (fd);

	if (bytes == trmpagesrequest)   /* got the extra page */
		bytes -= BLKSIZE;
	trmpages = (Int2)(bytes/BLKSIZE);
	if (bytes % BLKSIZE)   /* got a partial last page */
		trmpages++;
	trmtype = type;
	trmfield = field;
	trmpage = pg;
	                          /* may have to switch \n for \r */
    return  trmpages;
}


/*  -------------------- CdTrmUidsFil () --------------------------------
 *  CdTrmUids -- retrieves a list of uids for a term.
 *
 *  Parameters:     type:         database code.
 *                  field:        field code.
 *                  offset:     offset into postings file.
 *                  count:      number of uids.
 *                  filename:   name of file to receive the results.
 *
 *  Return value:   non-zero:   Success; number of documents (same as count).
 *                  zero:       Failure;  refer to error code.
 *
 *  Notes:   the offset value is obtained by:  
 *      1)  looking up a term (using CdTrmLookup()).
 *      2)  loading term pages (using CdTrmPages()).
 *      3)  finding the term in the loaded pages.
 */

Int4    CdTrmUidsFil (DocType type, DocField field, Int4 offset, Int4 count, CharPtr filename, Boolean append)

{
    Int4  i;
    FILE *fd1;
    FILE *fd2;
    Char mode [4];
    CharPtr path;
    Int4Ptr ptr;
    size_t cnt;
    Int4 cntr;

    if (!ValidateType (type))   return  0;
    if (!ValidateField (type, field))  return  0;

    path = MakePath (DIR_TRM, type, field, EXT_PST);
    if ((fd1 = FileOpen(path, "rb")) == NULL)
	{
		ErrPostEx(SEV_WARNING, ERR_CD_FILEOPEN, 0, sCdError[ERR_CD_FILEOPEN], path);
        return 0;
    }

    if (append) {
      StringCpy (mode, "ab");
    } else {
      StringCpy (mode, "wb");
    }
    if ((fd2 = FileOpen(filename, mode)) == NULL)
	{
        FileClose (fd1);
		ErrPostEx(SEV_WARNING, ERR_CD_FILEOPEN, 0, sCdError[ERR_CD_FILEOPEN], filename);
        return  0;
    }

    fseek (fd1, offset, SEEK_SET);

    cntr = count;
    cnt = (size_t) MIN (cntr, (Int4)(BLKSIZE / sizeof(Int4)));
	ptr = (Int4Ptr) buffer;
    while (cnt > 0)
	{
        FileRead (buffer, sizeof (Int4), cnt, fd1);
        for (i = 0; i < (Int4) cnt; i++)
			ptr[i] = SwapInt4(ptr[i]);
        if (! FileWrite (buffer, sizeof(Int4), cnt, fd2))
		{
			ErrPostEx(SEV_ERROR, ERR_CD_FILEWRITE, 0, sCdError[ERR_CD_FILEWRITE]);
            break;
        }
        cntr -= cnt;
	    cnt = (size_t) MIN (cntr, (Int4)(BLKSIZE / sizeof(Int4)));
    }

    FileClose (fd1);
    FileClose (fd2);
	if (cntr)    /* didn't finish */
		return 0;
	else
		return count;
}

/*  -------------------- CdTrmUidsMem () --------------------------------
 *  CdTrmUidsMem -- retrieves a list of uids for a term.
 *
 *  Parameters:     type:         database code.
 *                  field:        field code.
 *                  offset:     offset into postings file.
 *                  count:      number of uids.
 *                  mem:       storage to receive the results.
 *
 *  Return value:   non-zero:   Success; number of documents (same as count).
 *                  zero:       Failure;  refer to error code.
 *
 *  Notes:   the offset value is obtained by:  
 *      1)  looking up a term (using CdTrmLookup()).
 *      2)  loading term pages (using CdTrmPages()).
 *      3)  finding the term in the loaded pages.
 */

Int4    CdTrmUidsMem (DocType type, DocField field, Int4 offset, Int4 count, DocUidPtr mem)

{
    Int4  i;
    FILE *fd1;
    CharPtr path;
    size_t cnt;

    if (!ValidateField (type, field))
		return  0;
	if (mem == NULL)
		return 0;

    path = MakePath (DIR_TRM, type, field, EXT_PST);
    if ((fd1 = FileOpen(path, "rb")) == NULL)
	{
		ErrPostEx(SEV_WARNING, ERR_CD_FILEOPEN, 0, sCdError[ERR_CD_FILEOPEN], path);
        return 0;
    }

    fseek (fd1, offset, SEEK_SET);

    cnt = FileRead ((VoidPtr)mem, sizeof (Int4), (size_t) count, fd1);
    for (i = 0; i < (Int4) cnt; i++)
		mem[i] = SwapInt4(mem[i]);

    FileClose (fd1);
	return (Int4) cnt;
}

/*  -------------------- CdDocAsnOpen() -----------------------------------
 *  CdDocAsnOpen -- returns an active AsnIoPtr for a document.
 *
 *  Parameters:     type:        class code (ML/AA/NT).
*   				uid:         unique identifier
 *
 *  Return value:   non-null:   Success;  active asnioptr
 *                  null:       Failure;  refer to error code.
 *
 *  For TYP_ML, the value is a Medline-entry
*   For TYP_AA or TYP_NT it is a Bioseq-set.
 */

AsnIoPtr  CdDocAsnOpen (DocType type, DocUid uid)

{
    FILE * fd2;
    AsnIoPtr aip;
    DecompInfoPtr decomp;

    fd2 = CdDocFil (type, uid, NULL);
    if (fd2 == NULL)
        return NULL;

	if (vi->no_compression)
	{ /* no compression on this data source */
		aip = AsnIoNew(ASNIO_BIN_IN, fd2, NULL, NULL, NULL);
	}
	else { /* use alternate read function for compressed data sources */
    	decomp = DecompInit(fd2);
    	aip = AsnIoNew(ASNIO_BIN_IN, fd2, decomp, DecompReadFunc, NULL);
    	if (aip == NULL)
			DecompFini(NULL, decomp);
    	decomp->aip = aip;
	}

    return aip;
}

/*****************************************************************************
*
*   CdDocAsnClose(aip)
*       closes an aip opened by CdDocAsnOpen
*
*****************************************************************************/
AsnIoPtr  CdDocAsnClose (AsnIoPtr aip)

{
	if (!vi->no_compression)
	{
    	DecompFini(aip, NULL);
	}
	
    AsnIoClose(aip);

    return NULL;
}

/*  =========================================================================
 *      PRIVATE FUNCTION BODIES
 */

static Boolean NEAR  ValidateUid (DocType type, DocUid uid)

{
	EntrezTypeDataPtr tdp;
	DocType tmp;

	if (! ValidateType(type))
		return FALSE;

	tmp = type;
	if (tmp == TYP_SEQ)
		tmp = TYP_AA;

	tdp = &vi->types[tmp];
	if ((uid >= tdp->minuid) && (uid <= tdp->maxuid))
		return TRUE;

	if (type == TYP_SEQ)
	{
		tdp = &vi->types[TYP_NT];
		if ((uid >= tdp->minuid) && (uid <= tdp->maxuid))
			return TRUE;
	}

	return FALSE;
}

static Boolean NEAR  ValidateType (DocType type)

{
    if (((type < 0) || (type >= NTYPE)) && (type != TYP_SEQ)) {
		ErrPostEx(SEV_ERROR, ERR_CD_BADTYPE, 0, sCdError[ERR_CD_BADTYPE], type);
        return  FALSE;
    }
    return  TRUE;
}

static Boolean NEAR  ValidateField (DocType type, DocField field)

{
    if (type<0 || type>=NTYPE) {
		ErrPostEx(SEV_ERROR, ERR_CD_BADTYPE, 0, sCdError[ERR_CD_BADTYPE], type);
        return  FALSE;
    }
    if (field<0 || field>=NFLD) {
		ErrPostEx(SEV_ERROR, ERR_CD_BADFIELD, 0, sCdError[ERR_CD_BADFIELD], field);
        return  FALSE;
    }
    if (vi->types[type].fields[field].num_bucket == 0) {
		ErrPostEx(SEV_ERROR, ERR_CD_NOTERMS, 0, sCdError[ERR_CD_NOTERMS], type, field);
        return  FALSE;
    }
    return  TRUE;
}

static CharPtr NEAR  MakePath (Int2 nSdir, Int2 nPref, Int2 nSuff, Int2 nExtn)

{
    Char ltemp[8], filename[60];
    Char   c;
    CharPtr p;

    StringCpy (buffer, sPath[nSdir]);

    StringCpy (filename, sPref[nPref]);
    StringCat (filename, sSuff[nSuff]);
    StringCat (filename, ".");
        if (nExtn <0) {
        c = (char) -nExtn;
        ltemp[0] = (char) ('0' + (c/100));
        ltemp[1] = (char) ('0' + ((c%100)/10));
        ltemp[2] = (char) ('0' + (c%10));
        ltemp[3] = '\0';
        StringCat (filename, ltemp);
    }
    else
        StringCat (filename, sExtn[nExtn]);

    if (bAppendVer) 
        StringCat (filename, ";1");

	if (upperCaseIt) {
      p = filename;
	  while (*p != '\0') {
	    *p = TO_UPPER (*p);
	    p++;
	  }
	}

    FileBuildPath(buffer, NULL, filename);
    return  buffer;
}

static Boolean NEAR  LoadUidIndex (DocType type)

{
    Int2  i;
    size_t n;
    Int4Ptr p;
    CharPtr path;
    FILE   *fd;
    Int4 header [3];
    Int4 version;
    Int4 issue;

    if (!ValidateType (type))
		return FALSE;

	if ((type == TYP_SEQ) || (type == TYP_NT))
		type = TYP_AA;

    n = (size_t) vi->types[type].num_bucket + 1; 
    p = type_bucket_index[type];
    if (p != NULL)
        return TRUE;

	p = (Int4Ptr) MemNew(sizeof(Int4) * n);
	p[n-1] = INT4_MAX;            /* put sentinel at end */
	n--;         

	if (type == TYP_AA)
	{
		type = TYP_SEQ;
	}

    path = MakePath (DIR_IDX, type, SUF_UID, EXT_IDX);
    if ((fd = FileOpen (path, "rb")) ==NULL)  {
		MemFree (p);
		ErrPostEx(SEV_ERROR, ERR_CD_FILEOPEN, 0, sCdError[ERR_CD_FILEOPEN], path);
        return FALSE;
    }
    if (vi->version != 0 || vi->issue != 0) { /* for compatibility with pre-release 6 data */
      if (FileRead ((CharPtr)header, sizeof(Int4), 3, fd) != 3) {
        FileClose (fd);
        MemFree (p);
        ErrPostEx(SEV_ERROR, ERR_CD_FILEREAD, 0, sCdError[ERR_CD_FILEREAD]);
        return FALSE;
      }
	  if (! IsOKMagic((Uint4) SwapInt4(header[1]), vi->volume_label))
	  {
        ErrPostEx(SEV_ERROR,  ERR_CD_BADINDEX, 0, sCdError[ERR_CD_BADINDEX]);
		return FALSE;
      }
      header [2] = SwapInt4 (header [2]);
      version = (Int4) vi->version;
      issue = (Int4) vi->issue;
      if (header [2] != ((version << 16) | issue)) {
        ErrPostEx(SEV_ERROR,  ERR_CD_BADINDEX, 0, sCdError[ERR_CD_BADINDEX]);
		return FALSE;
      }
    }
    if (FileRead ((CharPtr)p, sizeof(Int4), n, fd) !=n) {
        FileClose (fd);
		MemFree (p);
		ErrPostEx(SEV_ERROR, ERR_CD_FILEREAD, 0, sCdError[ERR_CD_FILEREAD]);
        return FALSE;
    }
    FileClose (fd);

	if (type == TYP_SEQ)
	{
		type_bucket_index[TYP_NT] = p;
		type_bucket_index[TYP_AA] = p;
	} else {
		type_bucket_index[type] = p;
	}
    for (i=0; i< (Int2) n; i++, p++) 
        *p = SwapInt4 (*p);
    return TRUE;
}

static Int2 NEAR  LoadTrmIndex (DocType type, DocField field)

{
    Int2   i, k, c, buckets;
    CharPtr path, p;
    Int4  bytes;
    FILE   *fd;

    if (!ValidateType(type))  return 0;
    if (!ValidateField(type, field))  return  0;

    if (type == term_idx_type && field == term_idx_field)
        return  term_idx_count;

    path = MakePath (DIR_TRM, type, field, EXT_IDX);
    if ((fd = FileOpen (path, "r")) ==NULL)  {
		ErrPostEx(SEV_ERROR, ERR_CD_FILEOPEN, 0, sCdError[ERR_CD_FILEOPEN], path);
        return  0;
    }

    if (term_idx_count > 0)  FreeTrmIndex();

    buckets = (Int2)vi->types[type].fields[field].num_bucket;
    bytes = (buckets + 1) * sizeof(CharPtr);
    if ((term_idx_str = (CharPtr PNTR) MemNew((size_t)bytes)) ==NULL) {
        FileClose (fd);
		ErrPostEx(SEV_ERROR, ERR_CD_MEMORY, 0, sCdError[ERR_CD_MEMORY]);
        return(0);
    }

    for (i=0,c=0; c!=EOF; ) {
        for (p=buffer, k=0; k<128; k++) {
            c = fgetc(fd);
            if (c == EOF)  break;
            if (c == '\n' || c == '\r') {
                *p = '\0';
                break;
            }
            *p++ = (char) TO_LOWER(c);
        }
        while (c != '\n' && c != '\r' && c != EOF) {
            c = fgetc(fd);
        }
        *p = '\0';
        if (c != EOF && i < buckets) {
            if ((term_idx_str[i] = StringSave(buffer)) ==NULL)  {
                FileClose(fd);
                term_idx_count = i;
                FreeTrmIndex();
				ErrPostEx(SEV_ERROR, ERR_CD_MEMORY, 0, sCdError[ERR_CD_MEMORY]);
                return  0;
            }
			i++;
        }
    }
    FileClose (fd);
    term_idx_count = i;
    term_idx_type = type;
    term_idx_field = field;
    return  term_idx_count;
}

static void NEAR  FreeTrmIndex (void)

{
    int  i;

    for (i=0; i<term_idx_count; i++)
	{
		MemFree(term_idx_str[i]);
	}
    term_idx_str = (CharPtr PNTR) MemFree(term_idx_str);
    term_idx_count = 0;
    term_idx_type = -1;
    term_idx_field = -1;
}

extern AsnIoPtr   EntrezInfoOpen (CharPtr dirname)

{
    CharPtr p, buf, endpath;
    AsnIoPtr aip = NULL;
    FILE * fp;
	

	buf = (CharPtr) MemNew(PATH_MAX);
    p = StringMove(buf, dirname);
    endpath = buf + StringLen (buf);
    p = StringMove(p , "cdromdat.val;1");  
    p -= 2;           /* point to the semi-colon */
    *p = '\0';        /* wipe-out the semi-colon */
    if ((fp = FileOpen(buf, "rb")) == NULL)
    {
        *p = ';';     /* put back the semi-colon */
        if ((fp = FileOpen(buf, "rb")) != NULL)
            bAppendVer = TRUE;
    }

    if (fp == NULL) {
    	StringCat (buf, ";1");
    	p = endpath;
    	while (*p != '\0') {
    	  *p = TO_UPPER (*p);
    	  p++;
    	}
    	upperCaseIt = TRUE;
        p -= 2;           /* point to the semi-colon */
        *p = '\0';        /* wipe-out the semi-colon */
        if ((fp = FileOpen (buf, "rb")) == NULL) {
          *p = ';';     /* put back the semi-colon */
          if ((fp = FileOpen(buf, "rb")) != NULL)
            bAppendVer = TRUE;
        }
    }

    if (fp != NULL)
        aip = AsnIoNew(ASNIO_BIN_IN, fp, NULL, NULL, NULL);
	else
		ErrPostEx(SEV_WARNING, ERR_CD_FILEOPEN, 0, sCdError[ERR_CD_FILEOPEN], (CharPtr) "cdromdat.val");
	MemFree(buf);
    return aip;
}


#ifdef IS_LITTLE_ENDIAN

static Int2 NEAR  SwapInt2 (Int2 k)

{
    Uint2  j, l;
	Int2 m;

	l = (Uint2)k;
    j  = ((l & (Uint2)0xFF00) >> 8);
    j |= ((l & (Uint2)0x00FF) << 8);
	m = (Int2)j;
    return  m;
}

static Int4 NEAR  SwapInt4 (Int4 k)

{
    Uint4  j, l;
	Int4 m;

	l = (Uint4)k;
    j  = ((l & (Uint4)0xFF000000) >> 24);
    j |= ((l & (Uint4)0x00FF0000) >> 8);
    j |= ((l & (Uint4)0x0000FF00) << 8);
    j |= ((l & (Uint4)0x000000FF) << 24);
	m = (Int4)j;
    return  m;
}

#endif

/****** not used in reading cdrom **********************
static Int4 NEAR MergeSegOffset (Int2 seg, Int4 offset)

{
	Int4 value;

	value = (seg - 1) << 25;
	value += offset;
	return value;
}
******************************************************/
/***
bit 31 = if 1, is a protein, else is not
bits 30-25 = segment (file number)
bits 24-0  = offset into file up to 32 mbytes big
****************/
static Boolean NEAR SplitSegOffset (Int4 value, Int2Ptr segptr, Int4Ptr offsetptr)

{
	*segptr = (Int2)(((value >> 25) & 0x0000003F) + 1);
	*offsetptr = value & 0x01FFFFFF;
	return TRUE;
}

/*****************************************************************************
*
*   FILE * CdDocFil (type, uid, dat, &size)
*       opens a binary asn file, seeks to doc, returns a FILE * and size
*
*****************************************************************************/
static FILE * NEAR CdDocFil (DocType type, DocUid uid, UidIdxPtr idx)

{
    Int4 offset;
	Int2 seg, dir, db;
    CharPtr path;
    FILE   *fd2;
    UidIdx ui;

	if (idx == NULL)
	{
		idx = UidIdxGet(type, uid, &ui);
		if (idx == NULL)
			return NULL;
		if ((type == TYP_AA || type == TYP_NT) && idx->type != type)
			return NULL;
	}

	SplitSegOffset(idx->entry_offset, &seg, &offset);

    dir = (idx->type==TYP_ML) ? DIR_MED : DIR_SEQ;
    db = (idx->type==TYP_ML) ? PREF_MED : PREF_SEQ;

    path = MakePath (dir, db, SUF_ASN, (Int2) (-seg));
    if ((fd2=FileOpen (path, "rb")) == NULL)
	{
		ErrPostEx(SEV_ERROR, ERR_CD_FILEOPEN, 0, sCdError[ERR_CD_FILEOPEN], path);
        return  NULL;
    }
    fseek (fd2, offset, SEEK_SET);
    return fd2;
}


/*****************************************************************************
*
*   CdTrmFind(type, field, term)
*      returns a pointer to a CdTerm structure
*
*****************************************************************************/

static CdTermPtr  CdTrmDup (CdTermPtr trmptr)

{
	CdTermPtr ctp;

	ctp = NULL;
	if (trmptr != NULL) {
		ctp = (CdTermPtr) MemNew (sizeof (CdTerm));
		if (ctp != NULL) {
			ctp->type = trmptr->type;
			ctp->field = trmptr->field;
			ctp->term = StringSave (trmptr->term);
			ctp->special_count = trmptr->special_count;
			ctp->total_count = trmptr->total_count;
			ctp->offset = trmptr->offset;
			ctp->page = trmptr->page;
			ctp->next = NULL;
		}
	}
	return ctp;
}

static CdTermPtr  CdTrmCache (CdTermPtr trmptr)

{
	CdTermPtr ctp;
	Int2 i;

	ctp = cdtrmcache [9];
	if (ctp != NULL) {
		if (ctp->term != NULL) {
			MemFree (ctp->term);
		}
		MemFree (ctp);
		cdtrmcache [9] = NULL;
	}
	for (i = 9; i > 0; i--) {
		cdtrmcache [i] = cdtrmcache [i - 1];
	}
	ctp = CdTrmDup (trmptr);
	cdtrmcache [0] = ctp;
	return trmptr;
}

CdTermPtr  CdTrmFind (DocType type, DocField field, CharPtr term)

{
    Int2 i;
	Int2 termpage;
	CdTermPtr ctp = NULL;
	CdTermPtr trmptr;

    for (i = 0; i < 10; i++) {
      trmptr = cdtrmcache [i];
      if (trmptr != NULL && trmptr->type == type && trmptr->field == field &&
          StringICmp (trmptr->term, term) == 0) {
            return CdTrmDup (trmptr);
      }
    }
	termpage = CdTrmLookup(type,field,term);
	if (termpage < 0)
		return NULL;

				/** could it already be cached? ***/
	if ((trmtype == type) && (trmfield == field) && (trmpages > 0))
	{
	 	if ((termpage <= (trmpage + trmpages - 1)) &&
			((termpage + 3) >= trmpage))     /* overlapping range */
		{
			ctp = CdTrmLocate(term, termpage);
			if (ctp != NULL)	/* found it */
				return CdTrmCache (ctp);
			if (termpage == trmpage)   /* not possible to find it */
				return NULL;
		}
	}

				/** Load term pages from disk ***/

	termpage = CdTrmPages(type, field, termpage);
	if (termpage == 0)
		return NULL;

	ctp = CdTrmLocate(term, termpage);
	return CdTrmCache(ctp);
}

/*****************************************************************************
*
*   CdTrmLocate(term, page)
*   	locates a term in a term list already in cache
*
*****************************************************************************/
static CdTermPtr NEAR CdTrmLocate (CharPtr term, Int2 page)

{
	Int2 size, ctr, cmpval;
	CharPtr ret;

    size = trmpages * BLKSIZE;     /* bytes in term cache */
    ctr = 0;
    ret = trmbuf;
	size--;    /* have to have at least one space for test below */
    while (ctr < size)
    {
	    while (*ret != '\n' && *ret != '\r')
		{
    	  ret++;
	      ctr++;
		  if (ctr >= size)
			return NULL;
    	}
	    ret++;
    	ctr++;

		cmpval = MeshStringICmp(ret, term);
		if (! cmpval)     /* found it */
			return CdTermRead(trmtype, trmfield, ret, trmbuf, page);
		else if (cmpval > 0)   /* gone past */
			return NULL;
    }
	return NULL;
}

/*****************************************************************************
*
*   CdTermRead(type, field, ptr, bufr, page)
*   	creates and returns a CdTermPtr from a CdTermPage
*   	ptr should point at the start of a record (the term)
*
*****************************************************************************/
static CdTermPtr  CdTermRead (Int2 type, Int2 field, CharPtr ptr, CharPtr bufr, Int2 page)

{
	CdTermPtr trmptr;
	CharPtr tmp, tmp2;
	Char localbuf[10];
	Int4 vals[3];
	Int2 i;

	if (ptr == NULL)
		return NULL;
	if (*ptr == '\0')
		return NULL;
	trmptr = (CdTermPtr) MemNew(sizeof(CdTerm));
	trmptr->type = type;
	trmptr->field = field;
	tmp = ptr;
	tmp2 = tmp;
	while (*tmp2 != '\t')
		tmp2++;
	*tmp2 = '\0';
	trmptr->term = StringSave(tmp);
	*tmp2 = '\t';
	tmp2++;
	for (i = 0; i < 3; i++)
	{
		tmp = &localbuf[0];
		while (*tmp2 >= ' ')
		{
			*tmp = *tmp2;
			tmp++; tmp2++;
		}
		*tmp = '\0';
		vals[i] = atol(localbuf);
		tmp2++;
	}
	trmptr->special_count = vals[0];
	trmptr->total_count = vals[1];
	trmptr->offset = vals[2];
	trmptr->page = page + (Int2) (((size_t) (ptr - bufr - 1)) / (size_t) BLKSIZE);
	return trmptr;
}

/*****************************************************************************
*
*   CdTermScan(type, field, page, numpage, proc)
*   	returns terms found to proc until
*   	1) no more pages
*   	2) numpage pages have been read
*   	3) proc returns FALSE
*   returns number of complete pages read
*   if numpage=0, scans until EOF or proc returns FALSE
*
*****************************************************************************/
Int2  CdTermScan (DocType type, DocField field, Int2 page, Int2 numpage, CdTermProc proc)

{
	Boolean    goOn;
	CharPtr    ptr;
	Int2       pages, size, pagectr, startpage;
	CdTermPtr  trmptr;

	startpage = page;
	pagectr = 0;
	if (proc == NULL)
		return pagectr;

	goOn = TRUE;
    while (goOn)
	{
		startpage = page;
		pages = CdTrmPages (type, field, page);
		if (pages == 0)
			return pagectr;
		ptr = trmbuf;
		size = pages * BLKSIZE;     /* bytes available */
		pages = BLKSIZE;      /* bytes per page */
		while ((size > 0) && (goOn))
		{
			while (*ptr != '\n' && *ptr != '\r' && *ptr != '\0')
			{
				size--;
				pages--;
				ptr++;
			}
			if (*ptr == '\0')
				return (Int2) (pagectr + 1);   /* last page */
			size--;
			pages--;
			ptr++;
			if (size > 0)
			{
				trmptr = CdTermRead(type, field, ptr, trmbuf, startpage);
				if (trmptr != NULL) {
					goOn = proc (trmptr);
				}
			}
			while (*ptr != '\n' && *ptr != '\r' && *ptr != '\0')
			{
				size--;
				pages--;
				ptr++;
			}
			if (pages < 0)   /* crossed a page boundary */
			{
				pages = BLKSIZE + pages;
				numpage--;
				pagectr++;
				page++;
				if (! numpage)
					goOn = FALSE;
			}
		}
	}
	return pagectr;
}

/*****************************************************************************
*
*   CdLinkUidGet(type, link_to_type, numuid, uids, max)
*   	returns count of input uids processed
*       returns -1 on error
*       if neighbors (type == link_to_type)
*   		sums weights for same uids
*   	if (more than max uids, frees uids and weights, but leaves num set)
*
*****************************************************************************/
Int2  CdLinkUidGet (LinkSetPtr PNTR result, DocType type, DocType link_to_type, Int2 numuid, Int4Ptr uids, Boolean mark_missing, Int4 maxlink)

{
	UidIdxPtr query;
    UidIdx local;
	DocType querytype;
	LinkSetPtr lsp = NULL;
	Int2 counts[NTYPE];
	FILE * fp;
	Int4 offset;
	CharPtr path;
	Uint1Ptr ptr1;
	Int2 numfound = 0;
	Int4 j, l, r, k;
	Boolean first = TRUE;
	Boolean sorted;
	Int4 cursize = 0, finalsize, finalcount = 0, count, i;
	Int4Ptr newuids = NULL,
			newwts = NULL,
			finaluids = NULL,
			finalwts = NULL,
			tmp;

	*result = NULL;
	
	if (! ValidateType(link_to_type))
		return -1;

	for (i = 0, query = NULL; i < numuid && query == NULL; i++)
	{
		query = UidIdxGet(type, uids[i], &local);
		if ((mark_missing) && (query == NULL))
			uids[i] *= -1;
		if (query != NULL)
			j = i;
	}
	if ((i == numuid) && (query == NULL)) { /* none found */
		lsp = (LinkSetPtr) MemNew(sizeof(LinkSet));
		lsp->uids = NULL;
		lsp->weights = NULL;
		*result = lsp;
		return 0;
	}

	querytype = query->type;   /* record, to allow for TYP_SEQ */
	if (link_to_type == TYP_SEQ)
	{
		if (type != TYP_SEQ)
			return -1;           /* can't do it */
		else
			link_to_type = querytype;   /* neighbors */
	}

	path = MakePath (DIR_LNK, query->type, SUF_REC, EXT_LNK);
    if ((fp = FileOpen(path, "rb")) == NULL)
	{
		ErrPostEx(SEV_ERROR, ERR_CD_FILEOPEN, 0, sCdError[ERR_CD_FILEOPEN], path);
        return -1;
	}

	if (numuid > 1)
	{
		if (numuid > 320) {
			finalsize = 16000;
		} else {
			finalsize = MIN((numuid * 50), 16000);     /* make a guess */
		}
		finaluids = (Int4Ptr) MemNew((size_t)(sizeof(Int4) * finalsize));  /* make a guess */
		if (link_to_type == querytype)
			finalwts = (Int4Ptr) MemNew((size_t)(sizeof(Int4) * finalsize));
	}

	for (; j < numuid; j++)
	{
		
		if (! first)
		{
		    if ((query = UidIdxGet(type, uids[j], &local)) == NULL)
			{
				if (mark_missing)
					uids[j] *= -1;
				continue; /* must examine remaining UIDs */
			}
		}
		else
			first = FALSE;

		numfound++;         /* count how many uids we process */

	                            /* read the link counts for all types */

	    fseek (fp, query->link_offset, SEEK_SET);
		FileRead((CharPtr)&counts[0], sizeof(Int2), NTYPE, fp);
		for (i = 0; i < NTYPE; i++)
			counts[i] = SwapInt2(counts[i]);

		offset = 0;
		for (i = 0; i < link_to_type; i++)
		{
			offset += counts[i] * sizeof(DocUid);
			if (i == query->type) {  /* has weights */
				offset += counts[i] * sizeof(Uint1);
			}
		}
		if (offset)						   /* skip preceeding link types */
			fseek(fp, offset, SEEK_CUR);

		count = (Int4)counts[link_to_type];

		if (count > cursize)
		{
			MemFree(newuids);
			newuids = (Int4Ptr) MemNew((size_t)(sizeof(Int4) * (count + 1)));
			if (querytype == link_to_type)
			{
				MemFree(newwts);
				newwts = (Int4Ptr) MemNew((size_t)(sizeof(Int4) * (count + 1)));
			}
			cursize = count;
		}
		FileRead((CharPtr)newuids, sizeof(DocUid), (size_t)count, fp);
		for (i = 0; i < count; i++)
			newuids[i] = SwapInt4(newuids[i]);
		if (link_to_type == querytype)    /* get the weights */
		{
			ptr1 = (Uint1Ptr) newwts;
			FileRead((CharPtr)ptr1, sizeof(Uint1), (size_t)count, fp);
			for (i = count - 1; i >= 0; i--) {
				newwts[i] = (Int4) (ptr1[i]);
			}
		}
		if (numuid > 1)           /* merging lists */
		{
			if ((finalcount + count) > finalsize)
			{
				finalsize += count;
				if (finalsize > 16000)
				{
					MemFree(newuids);
					MemFree(newwts);
					MemFree(finaluids);
					MemFree(finalwts);
					ErrPostEx(SEV_WARNING, ERR_CD_MEMORY, 0, sCdError[ERR_CD_MEMORY]);
					return -1;
				}
				tmp = finaluids;
				finaluids = (Int4Ptr) MemNew((size_t)(sizeof(Int4) * finalsize));
				MemCopy(finaluids, tmp, (size_t)(finalcount * sizeof(Int4)));
				MemFree(tmp);
				if (querytype == link_to_type)
				{
				    tmp = finalwts;
					finalwts = (Int4Ptr) MemNew((size_t)(sizeof(Int4) * finalsize));
					MemCopy(finalwts, tmp, (size_t)(finalcount * sizeof(Int4)));
					MemFree(tmp);
				}
			}
			for (i = 0; i < count; i++)
			{
				l = 0;               /* binary search */
				r = (finalcount - 1);
				k = 0;
				while ((l <= r) && (finaluids[k] != newuids[i]))
				{
					k = (l + r)/ 2;
					if (newuids[i] < finaluids[k])
						r = k - 1;
					else
						l = k + 1;
				}
				if (finaluids[k] == newuids[i])   /* merge */
				{
					if (querytype == link_to_type)
						finalwts[k] += newwts[i];
				}
				else
				{
					if (finalcount)
					{
						if (finaluids[k] < newuids[i])
							k++;
						l = (finalcount - k);
						r = l;
						tmp = &finaluids[finalcount];
						while (r)
						{
							*tmp = *(tmp-1);
							tmp--; r--;
						}
						if (querytype == link_to_type)
						{
							r = l;
							tmp = &finalwts[finalcount];
							while (r)
							{
								*tmp = *(tmp-1);
								tmp--; r--;
							}
						}
					}
					finaluids[k] = newuids[i];
					if (querytype == link_to_type)
						finalwts[k] = newwts[i];
					finalcount++;
				}
			}
		}
	}

	FileClose(fp);

	lsp = (LinkSetPtr) MemNew(sizeof(LinkSet));
	if (maxlink <= 0)
		maxlink = 16000;    /* default */

 	if (numuid == 1)
	{
		lsp->num = count;
		if (lsp->num <= maxlink)
		{
			lsp->uids = newuids;
			lsp->weights = newwts;
		}
		else
		{
			MemFree(newuids);
			MemFree(newwts);
		}
	}
	else
	{
		MemFree(newuids);
		MemFree(newwts);
		lsp->num = finalcount;
		if (lsp->num <= maxlink)
		{
			lsp->uids = (Int4Ptr) MemNew((size_t)(sizeof(Int4) * (finalcount + 1)));
			MemCopy(lsp->uids, finaluids, (size_t)(finalcount * sizeof(Int4)));
			MemFree(finaluids);
			if (querytype == link_to_type)
			{
				lsp->weights = (Int4Ptr) MemNew((size_t)(sizeof(Int4) * (finalcount + 1)));
				MemCopy(lsp->weights, finalwts, (size_t)(finalcount * sizeof(Int4)));
				MemFree(finalwts);
				linksort(lsp->uids, lsp->weights, lsp->num);
			}
		}
		else
		{
			MemFree(finaluids);
			MemFree(finalwts);
		}
	}

	if (lsp->num > 1 && querytype != link_to_type && link_to_type == TYP_ML)
	{
		/* try to sort MEDLINE uids in descending order */

		for (sorted = TRUE, k = 1; k < lsp->num; k++)
		{
			if (lsp->uids[k-1] < lsp->uids[k])
			{
				sorted = FALSE;
				break;
			}
		}

		if (! sorted)
		{   /* assume that the existing order is reversed */
		    for (k = (lsp->num / 2) - 1; k >= 0; k--)
		    {
		    	j = lsp->uids[k];
		    	lsp->uids[k] = lsp->uids[lsp->num - 1 - k];
		    	lsp->uids[lsp->num - 1 - k] = j;
		    }

			/* now check that it's sorted */
	    	for (sorted = TRUE, k = 1; k < lsp->num; k++)
	    	{
	    		if (lsp->uids[k-1] < lsp->uids[k])
	    		{
	    			sorted = FALSE;
	    			break;
	    		}
	    	}

			if (! sorted)
			{ /* as a last resort, sort them using quicksort */
				/* dummy array */
				finaluids = (Int4Ptr) MemDup(lsp->uids, (size_t) (sizeof(Int4) * lsp->num));
				linksort(finaluids, lsp->uids, lsp->num);
				MemFree(finaluids);
			}
		}
	}

	*result = lsp;
	return numfound;
}

/*****************************************************************************
*
*   linksort(uids, wts, n)
*   	quicksort into descending wts order
*
*****************************************************************************/
static void NEAR linksort (Int4Ptr uids, Int4Ptr wts, Int4 n)

{
   Int4 tp, tp2;
   Int4 l, r, i, j, m, scnt;
   Int4 pstack[100];
   Int4Ptr p;

   if (n < 2)
       return;

   scnt = 2;
   l = 0; r = n - 1; p = pstack + 2;

   do
   {
       if ((r - l) > 15)
       {
           i = l; j = r;
                                     /* median of three */

           m = ((j - i) / 2) + i;    /* get middle element */
           /* partitioning operation */
           do
           {
               while((j > i) && (wts[j] <= wts[i]))
                   j--;
               if(j != i)
               {
                   tp = wts[j]; wts[j] = wts[i]; wts[i] = tp;
                   tp = uids[j]; uids[j] = uids[i]; uids[i] = tp;
                   while((i < j) && (wts[i] >= wts[j]))
                       i++;
                   if(i != j)
                      {tp = wts[j]; wts[j] = wts[i]; wts[i] = tp;
                       tp = uids[j]; uids[j] = uids[i]; uids[i] = tp;}
               }
           }while(i != j); /* end do */

           /* recursion elimination */
           if(i)
           {
               if((i - l) > (r - i))  /* put long segment on "stack" */
                   {*p = l; p++; *p = i - 1; p++; l = i + 1;}
               else
                   {*p = i + 1; p++; *p = r; p++; r = i - 1;}
               scnt += 2;
               if (scnt >= 100)
				{
					ErrPostEx(SEV_ERROR, ERR_CD_MEMORY, 0, "linksort > 100");
					return;
				}
           }
           else
           {
               l = i + 1;
           }
       }
                     /* if done with this segment, "pop" next */
       else
       {
            p--; r = *p; p--; l = *p; scnt -= 2;
       }
   }
   while (p > pstack);      /* end do */


   /* do the final insertion sort */

   for(i = 1; i < n; i++)
   {
       tp = wts[i]; tp2 = uids[i]; j = i; m = j - 1;
       while ((j > 0) && (wts[m] < tp))
           {wts[j] = wts[m]; uids[j] = uids[m]; j--; m--;}
       wts[j] = tp;
	   uids[j] = tp2;
   }
   return;
}

/*****************************************************************************
*
*   DecompReadFunc:
*   	substituted read function for compressed data sources (for Sequence
*   	and Medline data).
*
*****************************************************************************/
static Int2 LIBCALLBACK DecompReadFunc (Pointer p, CharPtr buff, Uint2 count)
{
	DecompInfoPtr dcp = (DecompInfoPtr) p;
	Uint1 loc_buff[3];
	int bytes_to_request;
	int bytes_read;

	if (dcp->compr == COMPR_DONT_KNOW)
	{
		int c;

		/* read the "decompression protocol identifier" */
		if ((c = fgetc(dcp->fp)) == EOF)  
			return 0;
		dcp->compr = (Uint1) c;

		if (dcp->compr == COMPR_NONE)
		{
			/* for no decompression, we still have 4 bytes of overhead;     */
			/* 1 byte for the protocol identifier, and 3 bytes for a length */
			/* field of what follows                                        */
			if (FileRead((CharPtr) loc_buff,1,3,dcp->fp) != 3)
			{
				ErrPostEx(SEV_ERROR, ERR_CD_BADDECOMP, 0,
				    	"No length field detected for uncompressed data");
				return 0;
			}

			/* interpret the 3-byte length in a machine-independant order;  */
			/* BIG ENDIAN (first byte is most significant)                  */
			dcp->bytes_left = (((int) loc_buff[0]) * 256 + loc_buff[1]) * 256 +
					          loc_buff[2];
		}
	}

	switch (dcp->compr)
	{
		case COMPR_NONE :
			/* based on knowledge of how many bytes are in this compressed  */
			/* ASN.1 object, return only as many bytes as the caller really */
			/* needs                                                        */
			bytes_to_request = (int) MIN((Uint4) count, dcp->bytes_left);
			bytes_read = FileRead(buff,1,bytes_to_request,dcp->fp);
			dcp->bytes_left -= bytes_read;
			if (dcp->bytes_left <= 0)
			{
				/* reset for stream read of next entry */
				dcp->compr = COMPR_DONT_KNOW;
			}
			return bytes_read;
		
		case COMPR_HUFFMAN :
			return HuffmanRead(dcp,buff,count);
		
		/* others ?? */

		default:
			ErrPostEx(SEV_ERROR, ERR_CD_BADDECOMP, 0,
				    "Invalid decompression code detected <%d>", dcp->compr);
			return 0;
	}
}

/*****************************************************************************
*
*   HuffmanRead:
*   	read Huffman compressed data
*
*****************************************************************************/
static Int2 HuffmanRead (DecompInfoPtr dcp, CharPtr buff, Uint2 count)
{
	register unsigned int mask = dcp->mask;
	register unsigned int byte = dcp->byte;
	CharPtr p = buff;
	int i, cnt = 0;
	int c;
	int k;
	FILE *fd1 = dcp->fp;


	while (cnt < (int) count)
	{
		for (i=0; i>=0; )
		{
			if (mask == 0)
			{
				if ((c = fgetc(fd1)) == EOF)
				{
					/* should never reach this point */
					i = HUFFMAN_SENTINEL - 257;
					break;
				}
				else
				{
					byte = (unsigned int) c;
					mask = 0x80;
				}
			}

			if (byte & mask)
				i = vi->huff_left[i];
			else
				i = vi->huff_right[i];

			mask >>= 1;
		}

		if ((k = i + 257) == HUFFMAN_SENTINEL)
		{
			mask = 0; /* to skip remaining bits in current byte */
			dcp->compr = COMPR_DONT_KNOW; /* reset for next record */
			break;
		}

		*p++ = (char) k;
		cnt++;
	}

	dcp->mask = mask;
	dcp->byte = byte;
	return cnt;
}

/*****************************************************************************
*
*   DecompInit:
*   	Create a data structure to be used in decompression; the data structures
*   	are stored in a linked list. While no mutual exclusion is provided on
*   	list access, each decompression is independent ... therefore, many
*   	compressed ASN.1 data streams may be open and used simultaneously
*
*****************************************************************************/
static DecompInfoPtr NEAR DecompInit (FILE *fp)
{
	DecompInfoPtr dcp;

	dcp = (DecompInfoPtr) MemNew(sizeof(DecompInfo));

	if (dcp == NULL)
		return NULL;

	dcp->fp = fp;
	dcp->compr = COMPR_DONT_KNOW;
	dcp->mask = 0;
	dcp->bytes_left = 0;

	/* insert it */
	dcp->next = DecompInfoList;
	DecompInfoList = dcp;

	return dcp;
}

/*****************************************************************************
*
*   DecompFini:
*   	Find and destroy the specified decompression data structure. The
*   	data structures, in addition to having an address known to its user,
*   	also contains a copy of the AsnIoPtr for that data stream. This
*   	enables the Fini() operation to be performed using either the address
*   	of this structure as a key, or the address of the AsnIoPtr as a key.
*
*****************************************************************************/
static Boolean NEAR DecompFini (AsnIoPtr aip, DecompInfoPtr dcp)
{
	DecompInfoPtr dtrail;
	DecompInfoPtr temp;

	if (DecompInfoList == NULL)
		return FALSE; /* not found */
	
	/* check for first element in list */
	if ((DecompInfoList == dcp && dcp != NULL) ||
	    (DecompInfoList->aip == aip && aip != NULL))
	{ /* unlink and delete */
		temp = DecompInfoList->next;
		DecompInfoFree(DecompInfoList);
		DecompInfoList = temp;
		return TRUE;
	}

	if (DecompInfoList->next == NULL)
	{ /* single-element list, and it's not the first element in list */
		return FALSE;
	}

	for (dtrail = DecompInfoList; dtrail->next != NULL;
	     dtrail = dtrail->next)
	{ /* search remainder of list */
		if ((dtrail->next == dcp && dcp != NULL) ||
		    (dtrail->next->aip == aip && aip != NULL))
		{
			temp = dtrail->next->next;
			DecompInfoFree(dtrail->next);
			dtrail->next = temp;
			return TRUE;
		}
	}

	return FALSE;
}


/*****************************************************************************
*
*   DecompInfoFree:
*   	Free a decompression data structure
*****************************************************************************/
static void NEAR DecompInfoFree(DecompInfoPtr dcp)
{
	MemFree(dcp);
}


/*****************************************************************************
*
*   IsOKMagic:
*   	Validate the magic number for a file
*****************************************************************************/
static Boolean NEAR IsOKMagic(Uint4 magic, CharPtr volume_label)
{
	/* check for a match with the "base" magic number; supported for        */
	/* backwards compatability                                              */
	if (magic == CD_MAGIC_BASE)
		return TRUE;

	/* now check if the magic number equals the "base" plus the checksum of */
    /* the volume-label (so as to be able to distinguish between index      */
	/* files associated with different CDs)                                 */
	while (*volume_label)
	{
		magic -= (int) (*volume_label++);
	}
	return (magic == CD_MAGIC_BASE);
}


#endif

