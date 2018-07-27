/*   ncbifile.c
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
* File Name:  ncbifile.c
*
* Author:  Gish, Kans, Ostell, Schuler
*
* Version Creation Date:   3/4/91
*
* $Revision: 6.34 $
*
* File Description: 
*     portable file routines
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 04-15-93 Schuler     Changed _cdecl to LIBCALL
* 12-20-93 Schuler     Converted ErrPost to ErrPostEx
* 11-27-94 Ostell      moved includes to ncbiwin.h to avoid conflict MSC
*
* $Log: ncbifile.c,v $
* Revision 6.34  2004/01/23 20:07:16  kans
* fix to FileGets under Darwin, was losing last character if buffer was shorter than line being read
*
* Revision 6.33  2003/11/17 17:17:57  kans
* changed C++ comments to C comments
*
* Revision 6.32  2003/05/05 11:53:37  rsmith
* Codewarrior compiling for Win32 does not know about setmode or tempnam.
*
* Revision 6.31  2003/03/11 14:26:42  rsmith
* previous change to Nlm_DirCatalog made for only OS_UNIX_DARWIN, not all OS_UNIX
*
* Revision 6.30  2003/03/10 15:55:49  rsmith
* for OS_UNIX change implementation of DirCatalog from using popen with a ls command to using the opendir and readdir library calls.
*
* Revision 6.29  2003/02/25 15:31:03  rsmith
* OS_UNIX_DARWIN (Mach on Macs) uses tempnam for temporary files, not tmpnam.
*
* Revision 6.28  2002/11/06 21:23:04  ucko
* Don't assume MIPS is IRIX; allow Linux too.
*
* Revision 6.27  2002/10/03 17:22:29  kans
* for OS_MAC or OS_UNIX_DARWIN, Nlm_FileGets does fgetc loop, stopping at the first \n or \r character, and leaving a \n as the last character before the null byte
*
* Revision 6.26  2002/10/02 14:34:17  kans
* Nlm_GetOSType and Nlm_FileCreate have ifdef OS_MAC, not WIN_MAC
*
* Revision 6.25  2002/06/13 16:14:07  kans
* fix includes for OS_UNIX_DARWIN with WIN_MAC (EN)
*
* Revision 6.24  2002/04/05 19:02:47  ivanov
* Changed L_tmpnam to PATH_MAX in Nlm_TmpNam()
*
* Revision 6.23  2001/08/29 17:33:15  juran
* Cleanup.
*
* Revision 6.22  2001/04/26 16:47:40  juran
* Refactored CreateDir to quash warnings.
*
* Revision 6.21  2001/04/06 20:09:00  juran
* Disabled #include of FullPath.h.
#if'ed out MacFSSpec2FullPathname(), which uses FullPath,
but which we're not using.
Disabled includes of other MoreFiles headers from MoreFilesExtras.c.
We should be entirely free of MoreFiles now.
Removed disabled CD routines.
*
* Revision 6.20  2001/04/05 22:30:27  juran
* Inserted five functions from MoreFileExtras.c.
*
* Revision 6.19  2001/04/05 21:59:13  juran
* Added back #include of MoreFilesExtras.h.
*
* Revision 6.18  2001/04/05 21:36:08  juran
* Mac changes:
* Added support for converting between an FSSpec an an arbitrarily long C 
* string containing a pathname.
* Added a wrapper for DirCreate() that takes a pathname and uses said 
* support.  Changed Nlm_CreateDir() to use it.
* Disabled CD routines.
*
* Revision 6.17  2000/10/16 17:54:46  kans
* null out param block for FileLengthEx on Mac
*
* Revision 6.16  2000/03/08 17:55:47  vakatov
* Use Int8 for the file size.
* Also, get rid of the WIN16 code, do other cleanup.
*
* Revision 6.15  1999/12/30 16:36:38  kans
* additional cleanup (Churchill)
*
* Revision 6.14  1999/12/27 22:21:01  shavirin
* Fixed FileWrite() function for size == 0
*
* Revision 6.13  1999/12/21 17:52:39  kans
* removed MPW/THINKC conditional code, starting upgrade to Carbon compatibility - Churchill
*
* Revision 6.12  1998/06/26 20:39:41  vakatov
* Added FilePathFind() -- complimentary to FileNameFind()
*
* Revision 6.11  1998/06/26 14:11:56  madden
* tempnam called with NULLs rather than empty strings
*
* Revision 6.10  1998/06/25 19:39:17  vakatov
* Added "FileLengthEx()" -- it returns -1(not 0!) if the file does not exist
*
* Revision 6.9  1998/05/28 15:59:55  vakatov
* [WIN16-BORLAND]  Nlm_DirCatalog:  "_find_t" -> "find_t"(Borland-specific)
*
* Revision 6.8  1998/05/28 14:21:53  vakatov
* [WIN16,WIN32] Nlm_DirCatalog -- tested, fixed
*
* Revision 6.7  1998/05/27 18:50:04  kans
* DirCat UNIX version redirects stderr to /dev/null to avoid printing unwanted error message when no directory exists
*
* Revision 6.6  1998/05/27 11:43:24  kans
* implemented DirCatalog for WIN16
*
* Revision 6.5  1998/05/26 20:28:17  kans
* stripped newline at end of DirCat strings in UNIX
*
* Revision 6.4  1998/05/26 17:42:45  vakatov
* [WIN32] Nlm_DirCatalog -- implemented, but not tested yet
*
* Revision 6.3  1998/05/26 15:17:14  kans
* implemented DirCatalog for OS_UNIX
*
* Revision 6.2  1998/05/24 19:20:56  kans
* added Nlm_DirCatalog (Mac implementation only so far)
*
* Revision 6.1  1998/03/31 20:31:33  vakatov
* FileRead()/FileWrite(): get rid of the redundant type cast that caused
* Int4 overflow under Solaris 2.6
*
* Revision 6.0  1997/08/25 18:15:30  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/07/22 19:11:26  vakatov
* Separated Main() from GetArg[svc]() functions;  [WIN_MSWIN] converged
* console and GUI libraries; [for WIN32-DLL] encapsulated global variables
*
* Revision 5.2  1997/01/14 21:57:14  vakatov
* Fixed inaccurate string copying -- <mostly potential> 1-byte exceeding of
* the string size by StringNCat;  missing terminating '\0' by StringNCpy.
*
 * Revision 5.1  1996/12/03  21:48:33  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 4.2  1996/02/29  14:40:29  kans
 * added USE_MPW_FILE_OPEN symbol to simplify ifdefs in FileOpen
 *
 * Revision 4.1  1996/02/28  21:55:54  kans
 * "MPW" file open also used for CodeWarrior, possible path limit otherwise
 *
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.41  1995/05/15  18:45:58  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#define THIS_MODULE g_corelib
#define THIS_FILE  _this_file

#include <ncbilcl.h>

#include "corepriv.h"
#ifdef OS_MAC
#ifdef OS_UNIX_DARWIN
#include <Carbon.h>
#endif
#include <Navigation.h>
#include <Script.h>
#endif

#ifdef OS_UNIX_SUN
#define DEFAULT_CDROM "/dev/sr0"
#define DEFAULT_RAW_CDROM "/dev/rsr0"
#endif

#if defined(PROC_MIPS) && !defined(OS_UNIX_LINUX)
#define DEFAULT_CDROM "/dev/scsi/sc0d4l0"
#endif

#ifdef OS_UNIX
#ifndef DEFAULT_CDROM
#define DEFAULT_CDROM "/dev/cdrom"
#endif
#endif

#if defined(OS_MSWIN) || defined (OS_NT)
#ifdef COMP_MSC
#ifndef mkdir
#define mkdir _mkdir
#endif
#ifndef stat
#define stat _stat
#endif
#endif
#endif

#ifdef OS_VMS
#ifndef DEFAULT_CDROM
#define DEFAULT_CDROM "cdrom:"
#endif
#endif

extern char *g_corelib;
static char * _this_file = __FILE__;

#ifdef OS_MAC
#define INLINE_MOREFILES
/* #include "FullPath.h" */
/* #include "MoreFilesExtra.h" */
#ifdef INLINE_MOREFILES

/* MoreFilesExtras.c */
/* ----------------- */

/*
**	Apple Macintosh Developer Technical Support
**
**	A collection of useful high-level File Manager routines.
**
**	by Jim Luther, Apple Developer Technical Support Emeritus
**
**	File:		MoreFilesExtras.c
**
**	Copyright © 1992-1999 Apple Computer, Inc.
**	All rights reserved.
**
**	You may incorporate this sample code into your applications without
**	restriction, though the sample code has been provided "AS IS" and the
**	responsibility for its operation is 100% yours.  However, what you are
**	not permitted to do is to redistribute the source as "DSC Sample Code"
**	after having made changes. If you're going to re-distribute the source,
**	we require that you make it clear in the source that the code was
**	descended from Apple Sample Code, but that you've made changes.
*/

#include <Types.h>
#include <Traps.h>
#include <OSUtils.h>
#include <Errors.h>
#include <Files.h>
#include <Devices.h>
#include <Finder.h>
#include <Folders.h>
#include <FSM.h>
#include <Disks.h>
#include <Gestalt.h>
#include <TextUtils.h>
#include <Script.h>
#include <Math64.h>
#include <CodeFragments.h>
#include <stddef.h>

#define	__COMPILINGMOREFILES

#if 0
#include "MoreFiles.h"
#include "MoreFilesExtras.h"
#include "MoreDesktopMgr.h"
#include "FSpCompat.h"
#endif

/*
**	GetVolumeInfoNoName uses pathname and vRefNum to call PBHGetVInfoSync
**	in cases where the returned volume name is not needed by the caller.
**	The pathname and vRefNum parameters are not touched, and the pb
**	parameter is initialized by PBHGetVInfoSync except that ioNamePtr in
**	the parameter block is always returned as NULL (since it might point
**	to the local tempPathname).
**
**	I noticed using this code in several places, so here it is once.
**	This reduces the code size of MoreFiles.
*/
static pascal	OSErr	GetVolumeInfoNoName(ConstStr255Param pathname,
									short vRefNum,
									HParmBlkPtr pb)
{
	Str255 tempPathname;
	OSErr error;
	
	/* Make sure pb parameter is not NULL */ 
	if ( pb != NULL )
	{
		pb->volumeParam.ioVRefNum = vRefNum;
		if ( pathname == NULL )
		{
			pb->volumeParam.ioNamePtr = NULL;
			pb->volumeParam.ioVolIndex = 0;		/* use ioVRefNum only */
		}
		else
		{
			BlockMoveData(pathname, tempPathname, pathname[0] + 1);	/* make a copy of the string and */
			pb->volumeParam.ioNamePtr = (StringPtr)tempPathname;	/* use the copy so original isn't trashed */
			pb->volumeParam.ioVolIndex = -1;	/* use ioNamePtr/ioVRefNum combination */
		}
		error = PBHGetVInfoSync(pb);
		pb->volumeParam.ioNamePtr = NULL;	/* ioNamePtr may point to local	tempPathname, so don't return it */
	}
	else
	{
		error = paramErr;
	}
	return ( error );
}

/*****************************************************************************/

static pascal	OSErr	DetermineVRefNum(ConstStr255Param pathname,
								 short vRefNum,
								 short *realVRefNum)
{
	HParamBlockRec pb;
	OSErr error;

	error = GetVolumeInfoNoName(pathname,vRefNum, &pb);
	if ( error == noErr )
	{
		*realVRefNum = pb.volumeParam.ioVRefNum;
	}
	return ( error );
}

/*****************************************************************************/

static pascal	OSErr GetCatInfoNoName(short vRefNum,
							   long dirID,
							   ConstStr255Param name,
							   CInfoPBPtr pb)
{
	Str31 tempName;
	OSErr error;
	
	/* Protection against File Sharing problem */
	if ( (name == NULL) || (name[0] == 0) )
	{
		tempName[0] = 0;
		pb->dirInfo.ioNamePtr = tempName;
		pb->dirInfo.ioFDirIndex = -1;	/* use ioDirID */
	}
	else
	{
		pb->dirInfo.ioNamePtr = (StringPtr)name;
		pb->dirInfo.ioFDirIndex = 0;	/* use ioNamePtr and ioDirID */
	}
	pb->dirInfo.ioVRefNum = vRefNum;
	pb->dirInfo.ioDrDirID = dirID;
	error = PBGetCatInfoSync(pb);
	pb->dirInfo.ioNamePtr = NULL;
	return ( error );
}

/*****************************************************************************/

static pascal	OSErr	GetDirectoryID(short vRefNum,
							   long dirID,
							   ConstStr255Param name,
							   long *theDirID,
							   Boolean *isDirectory)
{
	CInfoPBRec pb;
	OSErr error;

	error = GetCatInfoNoName(vRefNum, dirID, name, &pb);
	if ( error == noErr )
	{
		*isDirectory = (pb.hFileInfo.ioFlAttrib & kioFlAttribDirMask) != 0;
		if ( *isDirectory )
		{
			*theDirID = pb.dirInfo.ioDrDirID;
		}
		else
		{
			*theDirID = pb.hFileInfo.ioFlParID;
		}
	}
	
	return ( error );
}

/*****************************************************************************/

static pascal	OSErr	FSpGetDirectoryID(const FSSpec *spec,
								  long *theDirID,
								  Boolean *isDirectory)
{
	return ( GetDirectoryID(spec->vRefNum, spec->parID, spec->name,
			 theDirID, isDirectory) );
}

#endif  /* INLINE_MOREFILES */
#endif  /* OS_MAC */

/*****************************************************************************
*
*   Macintosh file utilities
*
*****************************************************************************/

#ifdef OS_MAC

static OSErr MacPathname2FSSpec(const char *inPathname, FSSpec *outFSS)
{
	OSErr err;
	size_t len;
	char *p;
	short vRefNum;
	long dirID;
	FSSpec fss;
	
	if (inPathname == NULL || outFSS == NULL) {
		return paramErr;
	}
	
	err = HGetVol(NULL, &vRefNum, &dirID);  /* default volume and directory */
	if (err != noErr) return err;
	
	len = strlen(inPathname);
	
	p = strchr(inPathname, ':');
	if (p == NULL) {
		/* Partial pathname -- filename only */
		Str31 filename;
		assert(len <= 31);
		c2pstrcpy(filename, inPathname);
		err = FSMakeFSSpec(vRefNum, dirID, filename, outFSS);
	} else {
		Str31 name;
		int nameLen;
		if (inPathname[0] == ':') {
			/* Relative pathname including directory path */
			
		} else {
			/* Absolute pathname */
			/* Str31 volName;  We would use Str28 if it was defined -- 27, plus 1 for ':'. */
			nameLen = p - inPathname;
			assert(nameLen <= 27);
			name[0] = nameLen + 1;
			memcpy(name + 1, inPathname, nameLen + 1);  /* Copy the volume name and the colon. */
			err = DetermineVRefNum(name, 0, &vRefNum);
			if (err != noErr) return err;
			dirID = 2;
		}
		/* vRefNum and dirID now specify the directory in which we should descend
		   the path pointed to by p (pointing to the first colon). */
		p++;
		while (p != NULL && *p != '\0') {
			char *q = strchr(p, ':');
			if (q != NULL) {
				Boolean isDir;
				nameLen = q - p;
				assert(nameLen <= 31);
				name[0] = nameLen;
				memcpy(name + 1, p, nameLen);
				err = FSMakeFSSpec(vRefNum, dirID, name, &fss);
				if (err != noErr) return err;
				if (q[1] == '\0') {
					p = NULL;
					*outFSS = fss;
				} else {
					err = FSpGetDirectoryID(&fss, &dirID, &isDir);
					assert(isDir == true);
					if (err != noErr) return err;
					p = q + 1;
				}
			} else {
				q = strchr(p, '\0');  /* go to end of string */
				nameLen = q - p;
				assert(nameLen > 0);
				assert(nameLen <= 31);
				c2pstrcpy(name, p);
				p = NULL;
				err = FSMakeFSSpec(vRefNum, dirID, name, outFSS);
			}
		}
	}
	return err;
}

#if 0
static OSErr MacFSSpec2FullPathname(const FSSpec *inFSS, char **outPathname)
{
	OSErr err;
	Handle h;
	short fullPathLength;
	static char *fullPath = NULL;
	
	if (fullPath != NULL) {
		Nlm_Free(fullPath);
		fullPath = NULL;
	}
	err = FSpGetFullPath(inFSS, &fullPathLength, &h);
	if (err != noErr) return err;
	
	assert(fullPathLength >= 2);  /* An absolute pathname must be at least two chars long */
	fullPath = (char *)Nlm_Malloc(fullPathLength + 1);
	if (fullPath == NULL) {
		err = memFullErr;
	} else {
		strncpy(fullPath, *h, fullPathLength);
	}
	
	DisposeHandle(h);
	
	*outPathname = fullPath;
	return err;
}
#endif

static OSErr MacCreateDirectory(const char *inPathname)
{
	OSErr err;
	FSSpec fss;
	ScriptCode scriptTag = 0;
	long createdDirID;
	
	err = MacPathname2FSSpec(inPathname, &fss);
	if (err != noErr) return err;
	
	err = FSpDirCreate(&fss, scriptTag, &createdDirID);
	return err;
}

#endif

/*****************************************************************************
*
*   FileOpen(filename, mode)
*     if (filename == "stdin" or "stdout" or "stderr"
*           returns those predefined
*           streams on non-windowing systems)
*
*****************************************************************************/

/* p_churchill 12/99 removed MPW conditional compilation
 */

static Nlm_FileOpenHook _hookFile = NULL;


NLM_EXTERN FILE * LIBCALL Nlm_FileOpen(const char *filename, const char *mode)
{
  FILE *f = NULL;

  if ( _hookFile )
    return _hookFile(filename, mode);

#if defined(WIN_DUMB)
    if ( Nlm_HasConsole )
      {
        if      ( !StringCmp("stdin",  filename))
          f = stdin;
        else if ( !StringCmp("stdout", filename) )
          f = stdout;
        else if ( !StringCmp("stderr", filename))
          f = stderr;
        else
          f = fopen(filename, mode);

#if defined(WIN32)  &&  ! defined(COMP_METRO) 
        if (strchr(mode, 'b')  &&
            (f == stdin  ||  f == stdout  ||  f == stderr))
          setmode(fileno(f), O_BINARY);
#endif
      }
    else
      f = fopen(filename, mode);

#elif defined(OS_MAC)
  {
    OSType    fCreator;
    Nlm_Int2  fError;
    FInfo     fInfo;
    OSType    fType;
    Nlm_Char  temp [256];

    Nlm_StringNCpy_0(temp, filename, sizeof(temp));
    Nlm_CtoPstr ((Nlm_CharPtr) temp);
    fError = HGetFInfo( 0, 0, (StringPtr) temp, &fInfo);
    if (fError == noErr) {
      fCreator = fInfo.fdCreator;
      fType = fInfo.fdType;
    } else {
        fCreator = '    ';
        if (strchr(mode, 'b') != NULL)
            fType = '    ';
        else
            fType = 'TEXT';
    }
    f = fopen( filename, mode);

    fError = HGetFInfo( 0, 0, (StringPtr) temp, &fInfo);
    if (fError == noErr) {
      fInfo.fdCreator = fCreator;
      fInfo.fdType = fType;
      fError = HSetFInfo ( 0, 0, (StringPtr) temp,&fInfo);
    }
  } /* def OS_MAC */

#elif defined(OS_VMS) && defined(DCC4DW12)
  /* never used */ 
  f = fopen (filename, mode);
  if (f  &&
      fstat(fileno(f), &statbuf) == 0  &&
      statbuf.st_fab_rfm == FAB$C_UDF)
    {
      fclose(f);
      f = fopen(filename,mode,"ctx=stm");
    }

#else
    f = fopen(filename, mode);  
#endif

  if (f == NULL)
    ErrPostEx(SEV_INFO, E_File, E_FOpen, "FileOpen(\"%s\",\"%s\") failed",
              filename, mode);
		
  return f;
}

/*****************************************************************************
*
*   SetFileOpenHook(hook)
*
*****************************************************************************/

NLM_EXTERN void LIBCALL Nlm_SetFileOpenHook (Nlm_FileOpenHook hook)
{
	_hookFile = hook;
}

/*****************************************************************************
*
*   FileClose(fp)
*
*****************************************************************************/

NLM_EXTERN void LIBCALL  Nlm_FileClose (FILE *stream)
{
  if (stream == NULL)
    return;

#ifdef WIN_DUMB    
  if (stream == stdin  ||  stream == stdout  ||  stream == stderr)
    {
#if defined(WIN32)  &&  ! defined(COMP_METRO) 
      setmode(fileno(stream), O_TEXT);
#endif
      return;
    }
#endif

  fclose(stream);
}

/*****************************************************************************
*   FileRead(buf, size, fp)
*****************************************************************************/
NLM_EXTERN size_t LIBCALL Nlm_FileRead
(void *ptr, size_t size, size_t n, FILE *stream)
{
  if (n  &&  (SIZE_MAX / n) < size) {
    ErrPostEx(SEV_WARNING,E_Programmer,0,"FileRead: size > SIZE_MAX");
    return 0;
  }
  if (!ptr  ||  !stream)
    return 0;

  return fread(ptr,size,n,stream);
}

/*****************************************************************************
*   FileWrite(buf, size, fp)
*****************************************************************************/
NLM_EXTERN size_t LIBCALL Nlm_FileWrite
(const void *ptr, size_t size, size_t n, FILE *stream)
{
    size_t cnt;
    if (n   &&  (SIZE_MAX / n)  <  size) {
        ErrPostEx(SEV_WARNING,E_Programmer,0,"FileWrite:  size > SIZE_MAX");
        return 0;
    }
    if (!ptr  ||  !stream || !size)
        return 0;
    
    cnt = fwrite(ptr,size,n,stream);
    if (cnt != n)
        ErrPostEx(SEV_FATAL,E_File,E_FWrite,"File write error");
    
    return cnt;
}

/*****************************************************************************
*
*   FilePuts(ptr, fp)
*
*****************************************************************************/
NLM_EXTERN int LIBCALL  Nlm_FilePuts (const char *ptr, FILE *fp)
{
	int retval;

	if ((ptr == NULL) || (fp == NULL))
    	return EOF;
	if ((retval = fputs(ptr,fp)) ==EOF)
		ErrPostEx(SEV_FATAL,E_File,E_FWrite,"File write error");
	return retval;
}

/*****************************************************************************
*
*   FileGets()
*
*****************************************************************************/
NLM_EXTERN char * LIBCALL  Nlm_FileGets (Nlm_CharPtr ptr, size_t size, FILE *fp)
{
#if defined(OS_MAC) || defined (OS_UNIX_DARWIN)
    int         ch;
	int         count;
	Nlm_CharPtr tmp;
#endif

	if ((ptr == NULL) || (size < 1) || (fp == NULL))
		return NULL;
#if defined(OS_MAC) || defined (OS_UNIX_DARWIN)
	ch = fgetc (fp);
	count = 0;
	tmp = ptr;
	while (ch != EOF && ch != '\0' && ch != '\n' && ch != '\r' && count < size - 2) {
	  *tmp = ch;
	  tmp++;
	  count++;
	  ch = fgetc (fp);
	}
	if (ch == '\n' || ch == '\r') {
	  *tmp = '\n';
	  tmp++;
	  count++;
	} else if (ch != EOF && ch != '\0') {
	  *tmp = ch;
	  tmp++;
	  count++;
	}
	*tmp = '\0';
	if (count < 1)
		return NULL;
	return ptr;
#else
	return fgets(ptr,size,fp);
#endif
}


/*****************************************************************************
*
*   FileBuildPath()
*
*****************************************************************************/
NLM_EXTERN Nlm_CharPtr LIBCALL  Nlm_FileBuildPath (Nlm_CharPtr root, Nlm_CharPtr sub_path, Nlm_CharPtr filename)

{
    Nlm_CharPtr tmp;
    Nlm_Boolean dir_start = FALSE;
#ifdef OS_VMS
  Nlm_Boolean had_root = FALSE;
#endif

    if (root == NULL)              /* no place to put it */
        return NULL;

    tmp = root;
    if (*tmp != '\0')                /* if not empty */
    {
#ifndef OS_VMS
        dir_start = TRUE;
#else
        had_root = TRUE;
#endif
        while (*tmp != '\0')
        {
#ifdef OS_VMS
            if (*tmp == '[')
                dir_start = TRUE;
#endif
            tmp++;
        }

        if ((*(tmp - 1) != DIRDELIMCHR) && (dir_start))
        {
            *tmp = DIRDELIMCHR;
            tmp++; *tmp = '\0';
        }
    }

    if (sub_path != NULL)
    {
#ifdef OS_VMS
        if (dir_start)
        {
            *(tmp-1) = '.';
            if (*sub_path == '[')
                sub_path++;
        }
        else if ((had_root) && (*sub_path != '['))
        {
            *tmp = '[';
            tmp++; *tmp = '\0';
        }
#else
        if ((dir_start) && (*sub_path == DIRDELIMCHR))
            sub_path++;
#endif
        tmp = StringMove(tmp, sub_path);
        if (*(tmp-1) != DIRDELIMCHR)
        {
            *tmp = DIRDELIMCHR;
            tmp++; *tmp = '\0';
        }
    }

    if (filename != NULL)
        StringMove(tmp, filename);

    return root;
}

/*****************************************************************************
*
*   FileNameFind()
*
*****************************************************************************/
NLM_EXTERN Nlm_CharPtr LIBCALL Nlm_FileNameFind (Nlm_CharPtr pathname)

{
  Nlm_CharPtr  filename;
  Nlm_Int2     len;

  if (pathname != NULL) {
    len = Nlm_StringLen (pathname);
    filename = &(pathname [len]);
    while (len > 0 && pathname [len - 1] != DIRDELIMCHR) {
      len--;
      filename--;
    }
    return filename;
  } else {
    return NULL;
  }
}


/*****************************************************************************
*
*   FilePathFind()
*
*****************************************************************************/
NLM_EXTERN Nlm_CharPtr LIBCALL Nlm_FilePathFind(const Nlm_Char* fullname)
{
  Nlm_CharPtr str;
  size_t	     len = Nlm_StringLen(fullname);
  if ( !len )
    return 0;

  while (len  &&  fullname[len] != DIRDELIMCHR)
    len--;

  str = (Nlm_Char*)Nlm_MemGet(len + 1, MGET_ERRPOST);
  Nlm_MemCpy(str, fullname, len);
  str[len] = '\0';
  return str;
}


/*****************************************************************************
*
*   FileLength()
*
*****************************************************************************/
NLM_EXTERN Nlm_Int8 LIBCALL Nlm_FileLength(Nlm_CharPtr fileName)
{
  Nlm_Int8 file_len = Nlm_FileLengthEx(fileName);
  return (file_len > 0) ? file_len : 0;
}


/*****************************************************************************
*
*   FileLengthEx()
*
*****************************************************************************/
NLM_EXTERN Nlm_Int8 LIBCALL Nlm_FileLengthEx(const Nlm_Char* fileName)
{
  if (!fileName  ||  !*fileName)
    return -1;

#ifdef OS_MAC
  {{
    OSErr           err;
    HParamBlockRec  params;
    Nlm_Char        path[256];

	Nlm_MemSet ((Nlm_VoidPtr) &params, 0, sizeof (HParamBlockRec));
    Nlm_StringNCpy_0(path, fileName, sizeof(path));
    Nlm_CtoPstr((Nlm_CharPtr) path);
    params.fileParam.ioNamePtr = (StringPtr)path;
    params.fileParam.ioVRefNum = 0;
    params.fileParam.ioFDirIndex = 0;
    err = PBHGetFInfo(&params, FALSE);
    return (err == noErr) ?
            params.fileParam.ioFlLgLen : -1;
  }}
#else
  {{
    struct stat sbuf;
    return (stat(fileName, &sbuf) == 0) ? sbuf.st_size : -1;
  }}
#endif
}


/*****************************************************************************
*
*   FileDelete()
*
*****************************************************************************/
NLM_EXTERN Nlm_Boolean LIBCALL Nlm_FileRemove (Nlm_CharPtr fileName)

{
  Nlm_Char  local [256];

  if (fileName != NULL && fileName [0] != '\0') {
    Nlm_StringNCpy_0(local, fileName, sizeof(local));
    return (Nlm_Boolean) (remove (local) == 0);
  } else {
    return FALSE;
  }
}

/*****************************************************************************
*
*   FileRename()
*
*****************************************************************************/
NLM_EXTERN Nlm_Boolean LIBCALL Nlm_FileRename (Nlm_CharPtr oldFileName, Nlm_CharPtr newFileName)

{
  Nlm_Char  localnew [256];
  Nlm_Char  localold [256];

  if (oldFileName != NULL && oldFileName [0] != '\0'
    && newFileName != NULL && newFileName [0] != '\0') {
    Nlm_StringNCpy_0(localold, oldFileName, sizeof(localold));
    Nlm_StringNCpy_0(localnew, newFileName, sizeof(localnew));
    return (Nlm_Boolean) (rename (localold, localnew) == 0);
  } else {
    return FALSE;
  }
}

/*****************************************************************************
*
*   FileCreate()
*
*****************************************************************************/
#ifdef OS_MAC
static OSType Nlm_GetOSType (Nlm_CharPtr str, OSType dfault)

{
  OSType  rsult;

  rsult = dfault;
  if (str != NULL && str [0] != '\0') {
    rsult = *(OSType*) str;
  }
  return rsult;
}
#endif

NLM_EXTERN void LIBCALL Nlm_FileCreate (Nlm_CharPtr fileName, Nlm_CharPtr type, Nlm_CharPtr creator)

{
#ifdef OS_MAC
  Nlm_Int2  fError;
  Nlm_Char  temp [256];
  OSType    fType;
  OSType    fCreator;
  FSSpec    spec;
#else
  FILE      *fp;
#endif

  if (fileName != NULL && fileName [0] != '\0') {

#ifdef OS_MAC
    /* note: the following assumes either full pathname or that the current
       directory is the proper location to find/create the file */

    Nlm_StringNCpy_0(temp, fileName, sizeof(temp));
    Nlm_CtoPstr ( temp);
    fError = FSMakeFSSpec( 0, 0, (StringPtr)temp, &spec);
    
    /* file not found, so create it... */
    if( fError == fnfErr){
        fType = Nlm_GetOSType (type, 'TEXT');
        fCreator = Nlm_GetOSType (creator, '    ');
        FSpCreate( &spec, fCreator, fType, smSystemScript);
    }
#else
    fp = Nlm_FileOpen (fileName, "w");
    if (fp != NULL) {
      Nlm_FileClose (fp);
    }
#endif
  }
}

/*****************************************************************************
*
*   CreateDir(pathname)
*
*****************************************************************************/

#ifdef OS_MAC
NLM_EXTERN Nlm_Boolean LIBCALL  Nlm_CreateDir (Nlm_CharPtr pathname)
{
  if (pathname != NULL && pathname [0] != '\0') {
    OSErr err;
    err = MacCreateDirectory(pathname);
    return (Nlm_Boolean) (err == noErr || err == dupFNErr);
  }
  return FALSE;
}
#endif

#ifndef OS_MAC
NLM_EXTERN Nlm_Boolean LIBCALL  Nlm_CreateDir (Nlm_CharPtr pathname)
{
#ifndef OS_VMS
  size_t          len;
  Nlm_Char        path[PATH_MAX];
#endif
#ifdef OS_UNIX
  mode_t          oldmask;
#endif
  Nlm_Boolean     rsult = FALSE;

  if (pathname != NULL && pathname [0] != '\0') {
#if defined(OS_MSWIN) || defined(OS_NT)
    Nlm_StringNCpy_0(path, pathname, sizeof(path));
    len = Nlm_StringLen (path);
    if (len > 0 && path [len - 1] == DIRDELIMCHR) {
        path [len - 1] = '\0';
    }
    rsult = (Nlm_Boolean) (mkdir ((char *) path) == 0);
    if (errno == EACCES) { /* it's O.K. if it was already there */
	rsult = TRUE;
    }
#endif
#ifdef OS_UNIX
    oldmask = umask (0000);
    Nlm_StringNCpy_0(path, pathname, sizeof(path));
    len = Nlm_StringLen (path);
    if (len > 0 && path [len - 1] == DIRDELIMCHR) {
        path [len - 1] = '\0';
    }
    rsult = (Nlm_Boolean) (mkdir ((char *) path, 0755) == 0);
    if (errno == EEXIST) { /* it's O.K. if it was already there */
	rsult = TRUE;
    }
    umask (oldmask);
#endif
#ifdef OS_VMS
    rsult = (Nlm_Boolean) (mkdir ((char *) pathname, 0755) == 0);
#endif
  }
  return rsult;
}
#endif

/*****************************************************************************
*
*   DirectoryContents()
*
*****************************************************************************/

#ifdef OS_UNIX_DARWIN
#include <dirent.h>
#endif

NLM_EXTERN ValNodePtr LIBCALL Nlm_DirCatalog (Nlm_CharPtr pathname)

{
#ifdef OS_MAC
  long            dirID;
  OSErr           err;
  short           index;
  unsigned short  num;
  Nlm_Char        path[PATH_MAX];
  CInfoPBRec      pbc;
  HParamBlockRec  pbh;
  short           vRefNum;
#endif
#ifdef OS_UNIX
  Nlm_Uint1       choice;
#ifdef OS_UNIX_DARWIN
  DIR             *dirp;
  struct dirent   *dep;
#else
  Nlm_Char        buf [256];
  Nlm_Char        ch;
  Nlm_Char        cmmd [PATH_MAX + 20];
  FILE            *fp;
  Nlm_CharPtr     ptr;
#endif
#endif
  ValNodePtr      vnp = NULL;

  if (pathname != NULL && pathname [0] != '\0') {
#ifdef OS_MAC
    Nlm_StringNCpy_0 (path, pathname, sizeof (path));
    Nlm_CtoPstr ((Nlm_CharPtr) path);
    Nlm_MemSet ((Nlm_VoidPtr) (&pbh), 0, sizeof (HParamBlockRec));
    pbh.volumeParam.ioNamePtr = (StringPtr) path;
    pbh.volumeParam.ioVolIndex = -1;
    err = PBHGetVInfo (&pbh, FALSE);
    if (err != noErr) return NULL;
    vRefNum = pbh.volumeParam.ioVRefNum;
    Nlm_StringNCpy_0 (path, pathname, sizeof (path));
    Nlm_CtoPstr ((Nlm_CharPtr) path);
    Nlm_MemSet ((Nlm_VoidPtr) (&pbc), 0, sizeof (CInfoPBRec));
    pbc.dirInfo.ioNamePtr = (StringPtr) path;
    pbc.dirInfo.ioVRefNum = vRefNum;
    err = PBGetCatInfo (&pbc, FALSE);
    if (err != noErr) return NULL;
    if (pbc.dirInfo.ioFlAttrib & 16) {
      num = pbc.dirInfo.ioDrNmFls;
      dirID = pbc.dirInfo.ioDrDirID;
      for (index = 1; index <= num; index++) {
        Nlm_MemSet ((Nlm_VoidPtr) (&pbc), 0, sizeof (CInfoPBRec));
        pbc.dirInfo.ioNamePtr = (StringPtr) path;
        pbc.dirInfo.ioVRefNum = vRefNum;
        pbc.dirInfo.ioFDirIndex = index;
        pbc.dirInfo.ioDrDirID = dirID;
        pbc.dirInfo.ioACUser = 0;
        err = PBGetCatInfo (&pbc, FALSE);
        if (err == noErr) {
          Nlm_PtoCstr ((Nlm_CharPtr) path);
          if (pbc.dirInfo.ioFlAttrib & 16) {
            ValNodeCopyStr (&vnp, 1, path);
          } else {
            ValNodeCopyStr (&vnp, 0, path);
          }
        }
      }
    }
#endif
#if defined(WIN32)
    {{
      Nlm_Char x_path[PATH_MAX];
      WIN32_FIND_DATA fData;
      HANDLE hFindFile;
      Nlm_StringNCpy_0(x_path, pathname, sizeof(x_path) - 5);
      Nlm_StringCat(x_path, "\\*.*");
      hFindFile = FindFirstFile(x_path, &fData);
      if (hFindFile == INVALID_HANDLE_VALUE)
        return 0;
      do {
        if (fData.cFileName[0] != '.'  ||
            (fData.cFileName[1] != '.'  &&  fData.cFileName[1] != '\0'))
          ValNodeCopyStr
            (&vnp, (fData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0,
             fData.cFileName);
      } while ( FindNextFile(hFindFile, &fData) );
      FindClose(hFindFile);
    }}
#endif
#ifdef OS_UNIX
#ifdef OS_UNIX_DARWIN
    dirp = opendir(pathname);
    if (dirp == NULL) return NULL;
    while ((dep = readdir(dirp)) != NULL) {
      /* ignore 'invisible' files. */
      if (dep->d_namlen < 1 || dep->d_name[0] == '.')
        continue;
      if (dep->d_type == DT_DIR) /* directory */
        choice = 1;
      else          /* all other file types. */
        choice = 0;
      ValNodeCopyStr (&vnp, choice, dep->d_name);
    }
    closedir(dirp);
#else
    sprintf (cmmd, "ls -1p %s 2>/dev/null", pathname);
    fp = popen (cmmd, "r");
    if (fp == NULL) return NULL;
    while (Nlm_FileGets (buf, sizeof (buf), fp) != NULL) {
      ptr = buf;
      ch = *ptr;
      while (ch != '\0' && ch != '\n' && ch != '\r') {
        ptr++;
        ch = *ptr;
      }
      *ptr = '\0';
      choice = 0;
      ptr = Nlm_StringChr (buf, '/');
      if (ptr != NULL) {
        *ptr = '\0';
        choice = 1;
      }
      ValNodeCopyStr (&vnp, choice, buf);
    }
    pclose (fp);
#endif
#endif
#ifdef OS_VMS
#endif
  }
  return vnp;
}

/*****************************************************************************
*
*   TmpNam()
*
*****************************************************************************/
NLM_EXTERN Nlm_CharPtr LIBCALL Nlm_TmpNam (Nlm_CharPtr s)

{
#ifdef TEMPNAM_AVAIL
    char *filename;
    static Nlm_Char save_filename[PATH_MAX];

    /* emulate tmpnam(), except get the benefits of tempnam()'s ability to */
    /* place the files in another directory specified by the environment   */
    /* variable TMPDIR                                                     */
#ifdef OS_UNIX_DARWIN
    filename = tempnam("/tmp", "ncbi.");
#else
    filename = tempnam(NULL, NULL);
#endif
    if (s == NULL)
    { /* return pointer to static string */
        if (filename != NULL) {
          strcpy ((char *) save_filename, (char *) filename);
          free ((void *) filename);
        } else {
          save_filename [0] = '\0';
        }
        return save_filename;
    } else {
        if (filename != NULL) {
          strcpy ((char *) save_filename, (char *) filename);
          Nlm_StrCpy (s, save_filename);
          free ((void *) filename);
        } else {
          *s = '\0';
        }
        return s;
    }
#else
#ifdef OS_MAC
    static Nlm_Char  directory [PATH_MAX];
    OSErr        err;
    long         gesResponse;
    long         newDirID;
    short        newVRefNum;
    CInfoPBRec   params;
    Nlm_Char     temp [PATH_MAX];
    Nlm_CharPtr  tmp;
    Nlm_Boolean  useTempFolder;
    char * filename;

    useTempFolder = FALSE;
    if (! Gestalt (gestaltFindFolderAttr, &gesResponse) &&
        (gesResponse & (1 << gestaltFindFolderPresent))) {
      err = FindFolder(kOnSystemDisk, kTemporaryFolderType,
                       kCreateFolder, &newVRefNum, &newDirID);
      if (err == noErr) {
        useTempFolder = TRUE;
      }
    }
    filename = tmpnam (NULL);
    if (useTempFolder) {
      temp [0] = '\0';
      params.dirInfo.ioNamePtr = (StringPtr) directory;
      params.dirInfo.ioDrParID = newDirID;
      do {
        params.dirInfo.ioVRefNum = newVRefNum;
        params.dirInfo.ioFDirIndex = -1;
        params.dirInfo.ioDrDirID = params.dirInfo.ioDrParID;
        err = PBGetCatInfo (&params, FALSE);
        Nlm_PtoCstr ((Nlm_CharPtr) directory);
        Nlm_StringCat (directory, DIRDELIMSTR);
        Nlm_StringCat (directory, temp);
        Nlm_StringCpy (temp, directory);
      } while (params.dirInfo.ioDrDirID != fsRtDirID);
      tmp = Nlm_StringMove (directory, temp);
      tmp = Nlm_StringMove (tmp, (Nlm_CharPtr) filename);
      if (s == NULL) {
          return (Nlm_CharPtr) directory;
      } else {
          s [0] = '\0';
          Nlm_StringCpy (s, directory);
          return s;
      }
    } else {
      if (s == NULL) {
          return (Nlm_CharPtr) filename;
      } else {
          s [0] = '\0';
          Nlm_StringCpy (s, filename);
          return s;
      }
    }
#else
    char * filename;

    filename = tmpnam (NULL);
    if (s == NULL) {
        return (Nlm_CharPtr) filename;
    } else {
        s [0] = '\0';
        Nlm_StringCpy (s, filename);
        return s;
    }
#endif
#endif
}

