/*   ncbienv.c
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
* File Name:  ncbienv.c
*
* Author:  Ostell
*
* Version Creation Date:   7/7/91
*
* $Revision: 6.36 $
*
* File Description:
*       portable environment functions, companions for ncbimain.c
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ncbienv.c,v $
* Revision 6.36  2004/02/11 18:40:00  kans
* enhanced GetOpSysString to report specific version of MS Windows
*
* Revision 6.35  2003/09/19 15:49:20  coulouri
* NetBSD fixes
*
* Revision 6.34  2003/06/24 19:45:39  ucko
* GCC/Darwin: Restore Nlm_IsApplicationPackage and the skeleton of
* Nlm_SetupArguments_ST_Mac.
*
* Revision 6.33  2003/06/24 15:39:42  ucko
* Switch back to the generic Unix ProgramPath code when building Darwin
* binaries with GCC until we find a framework that defines the relevant
* symbols without breaking support for remote execution.
*
* Revision 6.32  2003/06/06 15:01:03  rsmith
* fixed ProgramPath for OS Darwin & Mac, Added IsApplicationPackage for Darwin. ProgramPath on Darwin now returns the seen application not the executable buried in the package.
*
* Revision 6.31  2003/01/29 20:57:36  kans
* added linux, sgi, and solaris intel to GetOpSysString
*
* Revision 6.30  2003/01/29 19:37:14  kans
* added GetOpSysString to return allocated string describing operating system running the program
*
* Revision 6.29  2003/01/27 14:54:24  kans
* for darwin, also check /Contents/Resources/ of Mac OS X package for initial prepackaged config file
*
* Revision 6.28  2003/01/17 20:59:02  kans
* for Mach-O, try home/Library/Preferences/xxx.cnf first
*
* Revision 6.27  2002/10/03 16:22:03  kans
* changed fgets to Nlm_FileGets
*
* Revision 6.26  2002/03/28 13:29:08  kans
* checks for OS_UNIX_DARWIN (EN)
*
* Revision 6.25  2001/08/02 14:44:10  vakatov
* [OSF1]  NLM_XOPEN_SOURCE_500:: Kludge-fix for the weak-minded native
* preprocessor
*
* Revision 6.24  2001/08/01 16:15:38  vakatov
* Rollback to R6.22
*
* Revision 6.22  2001/05/25 17:58:27  vakatov
* [MAC] OpenConfigFile() -- replaced C++ style comments by the C-style ones
*
* Revision 6.21  2001/05/25 15:44:26  vakatov
* [AIX]  Tweaked conditions for NLM_POSIX1B
*
* Revision 6.20  2001/01/19 20:14:44  kans
* added checks for OS_UNIX_DARWIN (contributed by William Van Etten)
*
* Revision 6.19  2000/10/30 18:11:41  beloslyu
* FreeBSD was added
*
* Revision 6.18  2000/08/30 16:43:23  vakatov
* Nlm_WorkGetAppParam() -- do not try to open the same config file
* again (and again, and again) if failed to open it just before.
*
* Revision 6.17  2000/08/28 21:35:48  vakatov
* Figure out #LOGNAME_MAX value a little earlier in the code
*
* Revision 6.16  2000/08/28 18:32:18  vakatov
* Nlm_GetHome() -- split for s_GetHomeByUID() + s_GetHomeByLOGIN().
* Try s_GetHomeByUID() *before* s_GetHomeByLOGIN() to fix a problem
* reported by "Ray.Hookway@compaq.com" for LSF's Remote Execution Server.
*
* Revision 6.15  2000/06/28 14:50:43  vakatov
* Nlm_GetHome() -- IRIX, NLM_POSIX1B, getpwnam_r():  dont rely on the
* returned value only;  check for non-zero "pwd_ptr"
*
* Revision 6.14  2000/06/01 16:57:03  vakatov
* [IRIX]  Nlm_GetHome() -- added an extra check for the "pwd_ptr"
* (as per request by "Lack Mr G M" from SGI, gml4410@ggr.co.uk)
*
* Revision 6.13  2000/03/15 20:59:53  kans
* Mac version of Nlm_OpenConfigFile was opening file for reading even under writeMode - fixed
*
* Revision 6.12  1999/12/30 16:36:37  kans
* additional cleanup (Churchill)
*
* Revision 6.11  1999/12/21 18:22:17  kans
* new blessed file finding function needed to convert filename back to C string for fopen
*
* Revision 6.10  1999/12/21 17:52:39  kans
* removed MPW/THINKC conditional code, starting upgrade to Carbon compatibility - Churchill
*
* Revision 6.9  1999/11/29 19:58:49  vakatov
* To parse command-line arguments:
* + ECmdLineQuote, Nlm_ParseCmdLineArguments(), Nlm_FreeCmdLineArguments()
*
* Revision 6.8  1999/03/24 22:12:49  vakatov
* Nlm_ReadConfigFile():  free "Nlm_bottomComment" to avoid mem.leak
*
* Revision 6.7  1999/02/12 16:01:39  vakatov
* Added a draft version of Nlm_GetEnvParamEx()
* Got rid of the old "PROTO" and "LIBCALL" prototype junk, etc.
*
* Revision 6.6  1998/12/10 17:04:06  vakatov
* Fixed to compile under LINUX(Red Hat 2.XX, gcc, with POSIX threads)
*
* Revision 6.5  1997/11/26 21:26:10  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.4  1997/10/29 02:43:31  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.3  1997/10/27 21:58:11  vakatov
* Added Nlm_FreeArgs() to reset args earlier set by GetArgs[Silent]()
*
* Revision 6.2  1997/09/09 00:05:43  vakatov
* Nlm_x_HasConsole() -- "fileno" instead of "_fileno" to pass Borland compiler
*
* Revision 6.1  1997/08/27 16:16:38  vakatov
* [WIN32] Nlm_x_HasConsole() -- fixed for the case when the
* Vibrant-based(GUI) application is run from the console(command prompt)
*
* Revision 5.18  1997/07/22 19:11:18  vakatov
* Separated Main() from GetArg[svc]() functions;  [WIN_MSWIN] converged
* console and GUI libraries; [for WIN32-DLL] encapsulated global variables
*
* Revision 5.17  1997/06/02 15:28:36  vakatov
* [WIN32]  Read/write config(*.ini) files via WinSDK calls([WIN_MSWIN]-like)
*
* Revision 5.16  1997/05/08 19:37:08  vakatov
* [WIN_MSWIN]  Nlm_SetupArguments_ST() -- interpret all text(including
* space symbols) embraced by a pair of ""(or '') quote marks as a
* single command argument;  strip the embracing quote marks
*
* Revision 5.15  1997/03/05 20:32:55  vakatov
* Nlm_WorkGetAppParam: now free Nlm_lastParamFile before reset, avoid mem.leak
*
 * Revision 5.14  1997/03/03  16:44:00  kans
 * cleanup of memory leaks accidentally freed the cached file name,
 * so now only the Nlm_envList is freed with Nlm_FreeEnvData
 *
 * Revision 5.13  1997/02/27  19:33:48  vakatov
 * Nlm_ReadConfigFile():  free the obsolete config data --> fight memory leaks
 *
 * Revision 5.12  1997/01/28  21:19:12  kans
 * <Desk.h>, <OSEvents.h> and <GestaltEqu.h> are obsolete in CodeWarrior
 *
 * Revision 5.11  1997/01/24  17:03:49  epstein
 * make threaded version compatible with OSF/1
 *
 * Revision 5.10  1997/01/03  16:12:07  vakatov
 * Fixed inaccurate string copying -- <mostly potential> 1-byte exceeding of
 * the string size by StringNCat;  missing terminating '\0' by StringNCpy.
 *
 * Revision 5.9  1996/12/30  15:13:12  vakatov
 * [WIN_MSWIN]  Command-line parsing implemented inside Nlm_SetupArguments()
 *
 * Revision 5.8  1996/12/16  22:38:53  vakatov
 * Rolled back the changes made in "* Revision 5.4.  1996/11/27  20:38:14
 * epstein"
 *
 * Revision 5.7  1996/12/04  21:44:47  vakatov
 * [OS_UNIX][POSIX_THREADS_AVAIL]  Added _POSIX1C case (see Rev.5.5)
 *
 * Revision 5.6  1996/12/03  21:48:33  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.5  1996/11/27  21:55:17  vakatov
 * [OS_UNIX][POSIX_THREADS_AVAIL]  Added (_POSIX_C_SOURCE - 0 >= 199506L)
 * preprocessor condition to match POSIX.1c function interface.
 *
 * Revision 5.4  1996/11/27  20:38:14  epstein
 * disable error logging when opening files in parameter fetching functions
 *
 * Revision 5.3  1996/11/25  19:04:26  vakatov
 * Wrapped all basic functions to MT-safe wrappers(named by adding '_ST'
 * suffix to the original function names).
 *
 * Revision 5.2  1996/08/19  18:46:06  vakatov
 * [WIN32]  Made modifications to let one create console applications
 *
 * Revision 4.9  1996/02/15  22:00:49  kans
 * changed platform symbol back to OS_NT
 *
 * Revision 4.8  1996/01/31  20:29:57  epstein
 * fix uninitialized ptr in Nlm_GetHome()
 *
 * Revision 4.7  1996/01/29  22:33:37  epstein
 * Added GetHome() changes per Mr. G.M. Lack (gml4410@ggr.co.uk)
 *
 * Revision 4.6  1995/12/13  17:18:51  kans
 * fixed caching bug (JE & JK)
 *
 * Revision 4.5  1995/10/28  15:03:20  ostell
 * added casts to quiet DOS compile warnings
 *
 * Revision 4.4  1995/10/11  13:53:05  kans
 * made some variables static
 *
 * Revision 4.3  1995/10/06  19:59:18  epstein
 * more performance fixes
 *
 * Revision 4.2  1995/10/06  15:53:31  epstein
 * add CacheAppParam() and FlushAppParam() for improved performance
 *
 * Revision 4.1  1995/10/03  15:59:21  epstein
 * add NCBI_DONT_USE_LOCAL_CONFIG environment variable to avoid local config files
 *
*  7/7/91   Kans        Multiple configuration files, get and set functions
*  9-20-91  Schuler     GetAppParam takes default value as an argument
* 01-14-94  Epstein     Merged ncbienv.{unx,vms,dos,msw,mac} into a single file
* 06-14-94  Schuler     Put SetAppPropery("ProgramName",..) in SetupArguments
* 06-14-94  Schuler     Add LIBCALL to SetupArguments
* 08-23-94  Schuler     Add SetupArguments case for OS_NT/WIN_DUMB
* 01-29-96  Epstein     Added GetHome() changes per Mr. G.M. Lack
*
* ==========================================================================
*/

#ifdef OS_MAC
#ifdef PROC_MC680X0
#define OBSOLETE
#endif
#endif

#include "corepriv.h"

#ifdef OS_UNIX
#include <pwd.h>
#endif /* OS_UNIX */
#ifdef OS_MAC
#include <Gestalt.h>
#include <Folders.h>
#include <Processes.h>
#include <Script.h>
#endif /* OS_MAC */


typedef struct nlm_env_item {
  struct nlm_env_item* next;
  Nlm_Char*            name;
  Nlm_Char*            comment;
  Nlm_Char*            value;
} Nlm_env_item, *Nlm_env_itemPtr;

typedef struct nlm_env_sect {
  struct nlm_env_sect* next;
  Nlm_Char*            name;
  Nlm_Char*            comment;
  Nlm_Boolean          transientOnly; /* this field used only by Transient */
  struct nlm_env_item* children;
} Nlm_env_sect, *Nlm_env_sectPtr;

typedef struct nlm_env_file {
  struct nlm_env_file* next;
  Nlm_Char*            name;
  Nlm_env_sectPtr      envList;
} Nlm_env_file, *Nlm_env_filePtr;


static Nlm_env_filePtr Nlm_transientFileList = NULL;
static Nlm_Boolean caching = FALSE;
static Nlm_Boolean dirty = FALSE;
static Nlm_Boolean mustCreateFile = TRUE;

static Nlm_Boolean Nlm_Qualified(const Nlm_Char* path);
static Nlm_Boolean Nlm_TransientLookup(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* dflt, Nlm_Char* buf, size_t buflen);
static void Nlm_TransientLogSetApp(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* value);
static void Nlm_FreeEnvData(Nlm_env_sectPtr esp);
static void Nlm_FreeTransientData(void);

#ifndef OS_MSWIN
static FILE* Nlm_OpenConfigFile(const Nlm_Char* file, Nlm_Boolean writeMode, Nlm_Boolean create);
static Nlm_Char* Nlm_TrimString(Nlm_Char* str);
static Nlm_Boolean Nlm_ReadConfigFile(FILE* fp);
static Nlm_env_sectPtr Nlm_FindConfigSection(const Nlm_Char* section);
static Nlm_env_itemPtr Nlm_FindConfigItem(const Nlm_Char* section, const Nlm_Char* type, Nlm_Boolean create);
static Nlm_Boolean Nlm_WriteConfigFile(FILE* fp);
static void Nlm_PutComment(const Nlm_Char* s, FILE* fp);
static void Nlm_FreeConfigFileData(void);
static void Nlm_FlushConfigFile(const Nlm_Char* file, Nlm_Boolean create);

static Nlm_env_sectPtr Nlm_envList = NULL;
static Nlm_Char* Nlm_lastParamFile = NULL;
static Nlm_Char* Nlm_bottomComment = NULL;

/* always FALSE, because this file is trying to emulating MS Windows's  */
/* handling of comments in Param files; however, just change this value */
/* to TRUE to turn this approach around                                 */
static Nlm_Boolean destroyDeadComments = FALSE;

/*****************************************************************************
*
* The "guts" of:
*   Nlm_GetAppParam (file, section, type, dflt, buf, buflen)
*      finds parameters from configuration files
*      this version, searching for configuration file(s) in a
*      platform-dependent basis as handled by Nlm_OpenConfigFile()
*
*      if configuration file is found, tries to read the parameter from it.
*
*****************************************************************************/

static Nlm_Int2 
Nlm_WorkGetAppParam(const Nlm_Char* file, 
                    const Nlm_Char* section, 
                    const Nlm_Char* type, 
                    const Nlm_Char* dflt, 
                    Nlm_Char* buf, 
                    Nlm_Int2 buflen, 
                    Nlm_Boolean searchTransient)
{
  Nlm_env_itemPtr  eip;
  FILE             *fp;

  if (buf == NULL  ||  buflen <= 0)
    return 0;

  *buf = '\0';
  if (searchTransient  &&
      Nlm_TransientLookup(file, section, type, dflt, buf, buflen))
    {
      return (Nlm_Int2)Nlm_StringLen(buf);
    }

  if ( dflt )
    Nlm_StringNCpy_0(buf, dflt, buflen);

  if (file    == NULL   ||  *file    == '\0'  ||
      section == NULL   ||  *section == '\0')
    return (Nlm_Int2)Nlm_StringLen( buf );

  if (Nlm_lastParamFile == NULL  ||
      Nlm_StringICmp(Nlm_lastParamFile, file) != 0)
    {
      mustCreateFile = TRUE;
      if ( caching )
        Nlm_FlushAppParam();
      Nlm_FreeConfigFileData();
      fp = Nlm_OpenConfigFile(file, FALSE, FALSE);
      MemFree( Nlm_lastParamFile );
      Nlm_lastParamFile = Nlm_StringSave( file );
      if (fp != NULL)
        {
          Nlm_ReadConfigFile( fp );
          Nlm_FileClose( fp );
        }
    }

  if (type != NULL  &&  *type != '\0')
    {
      eip = Nlm_FindConfigItem(section, type, FALSE);
      if (eip != NULL)
        Nlm_StringNCpy_0(buf, eip->value, buflen);
    }
  else
    { /* return all the types in that section */
      Nlm_env_sectPtr  esp    = Nlm_FindConfigSection( section );
      Nlm_Int2         totlen = 0;
      *buf = '\0';
      if (esp == NULL)
        return 0;

      /* traverse the children, allowing the null chars to be inserted */
      /* in between each type-name                                     */
      for (eip = esp->children;
           eip != NULL  &&  totlen != buflen;  eip = eip->next)
        {
          Nlm_Int2 bytesToAppend = StrLen(eip->name) + 1;
          bytesToAppend = MIN(bytesToAppend, buflen - totlen);
          StrNCpy(&buf[totlen], eip->name, bytesToAppend - 1);
          totlen += bytesToAppend;
          buf[totlen - 1] = '\0';
        }
    }

  return (Nlm_Int2)Nlm_StringLen(buf);
}


/*****************************************************************************
*
*   Nlm_SetAppParam (file, section, type, value)
*      finds paths for types of data and fills in path in buf
*      this version
*      1)  looks in the current directory for ".filerc", but will not
*          create a new file in this directory.
*      2)  then looks in the home directory for ".filerc".
*      3)  then looks for an environment variable "NCBI" and takes its
*          value as a complete path to a directory containing the
*          configuration file ".filerc".
*      if configuration file is found, tries to write the parameter to it.
*
*****************************************************************************/

static Nlm_Boolean Nlm_SetAppParam_ST(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* value)
{
  Nlm_env_itemPtr  eip;
  Nlm_env_sectPtr  esp;
  FILE             *fp = NULL;
  Nlm_Boolean      rsult;

  rsult = FALSE;
  if (file != NULL && *file != '\0' && section != NULL && *section != '\0') {
    Nlm_TransientLogSetApp (file, section, type, value);

    if (Nlm_lastParamFile == NULL || Nlm_StringICmp(Nlm_lastParamFile, file) != 0) {
      mustCreateFile = TRUE;
    }
    if (mustCreateFile)
      fp = Nlm_OpenConfigFile (file, FALSE, TRUE);

    if (TRUE) {
      if (fp != NULL) {
        Nlm_FlushAppParam();
        Nlm_FreeConfigFileData();
        Nlm_lastParamFile = Nlm_StringSave(file);
        Nlm_ReadConfigFile (fp);
        Nlm_FileClose (fp);
        mustCreateFile = FALSE;
      }
      if (type != NULL && *type != '\0')
      {
        eip = Nlm_FindConfigItem (section, type, TRUE);
        if (eip != NULL) {
          if (eip->value != NULL) {
            eip->value = (Nlm_Char*) Nlm_MemFree (eip->value);
          }
          eip->value = Nlm_StringSave (value);
          rsult = TRUE;
        }
      }
      else { /* wipe out that section */
        esp = Nlm_FindConfigSection (section);
        if (esp != NULL) { /* kill section by deleting name (leave comments)*/
          esp->name = (Nlm_Char*) Nlm_MemFree(esp->name);
          rsult = TRUE;
        }
      }

      if (rsult) {
        dirty = TRUE;
      }
      if (! caching) {
        Nlm_FlushConfigFile(file, TRUE);
      }
    }
  }

  return rsult;
}

/*****************************************************************************
*
*   Nlm_FlushAppParam()
*      flush the current parameter file's parameters to disk
*
*****************************************************************************/
static void Nlm_FlushAppParam_ST(void)
{
  if (Nlm_lastParamFile != NULL)
    Nlm_FlushConfigFile(Nlm_lastParamFile, TRUE);
}

/*****************************************************************************
*
*   Nlm_CacheAppParam()
*      Indicates whether data should be flushed to disk after each call
*      to SetAppParam()
*
*****************************************************************************/
static Nlm_Boolean Nlm_CacheAppParam_ST(Nlm_Boolean value)
{
  Nlm_Boolean oldvalue = caching;

  caching = value;
  if (! value)
    Nlm_FlushAppParam();

  return oldvalue;
}


#ifdef OS_UNIX

/* This is a special kludge for OSF1 native compiler which apparently
 * cannot handle (_XOPEN_SOURCE == 500) when _XOPEN_SOURCE is defined
 * to nothing
 */
#if (_XOPEN_SOURCE == 500)
#  define NLM_XOPEN_SOURCE_500 1
#else
#  define NLM_XOPEN_SOURCE_500 0
#endif

#define NLM_POSIX1B \
    (_POSIX1B || _POSIX1C || \
    (_POSIX_C_SOURCE - 0 >= 199309L) || \
    defined(_POSIX_PTHREAD_SEMANTICS) || \
    (defined(OS_UNIX_AIX)  &&  NLM_XOPEN_SOURCE_500  &&  !defined(_UNIX95)))

#ifndef LOGNAME_MAX
#  if defined(MAXLOGNAME)
#    define LOGNAME_MAX MAXLOGNAME
#  elif defined(_POSIX_LOGIN_NAME_MAX)
#    define LOGNAME_MAX _POSIX_LOGIN_NAME_MAX
#  endif
#endif /* ndef LOGNAME_MAX */


/*****************************************************************************
*
*   Nlm_GetHome (buf, buflen)
*      returns the path of the home directory
*
*****************************************************************************/


/* This function is used by Nlm_GetHome(), see below
 */
static Nlm_Boolean s_GetHomeByUID(Nlm_Char* buf, size_t buf_size)
{
    struct passwd* pwd_ptr = 0;

    /* Get the info using user ID */
#if  (defined(SOLARIS_THREADS_AVAIL) || defined(POSIX_THREADS_AVAIL)) && !defined(OS_UNIX_FREEBSD) && !defined(OS_UNIX_DARWIN) && !defined(OS_UNIX_NETBSD)
    struct passwd pwd;
    Nlm_Char      pwd_buffer[LOGNAME_MAX + PATH_MAX + 1024 + 1];

#  if NLM_POSIX1B
    if (getpwuid_r(getuid(), &pwd, pwd_buffer, sizeof(pwd_buffer),
                   &pwd_ptr) != 0) {
        pwd_ptr = 0;
    }
#  else
    pwd_ptr = getpwuid_r(getuid(), &pwd, pwd_buffer, sizeof(pwd_buffer));
#  endif
#else
    pwd_ptr = getpwuid(getuid());
#endif

    if (!pwd_ptr  ||  pwd_ptr->pw_dir[0] == '\0')
        return FALSE;
    Nlm_StringNCpy_0(buf, pwd_ptr->pw_dir, buf_size);
    return TRUE;
}


/* This function is used by Nlm_GetHome(), see below
 */
static Nlm_Boolean s_GetHomeByLOGIN(Nlm_Char* buf, Nlm_Int2 buf_size)
{
    struct passwd* pwd_ptr = 0;

    /* Get the user login name */
#if (defined(SOLARIS_THREADS_AVAIL) || defined(POSIX_THREADS_AVAIL)) && !defined(OS_UNIX_FREEBSD) && !defined(OS_UNIX_DARWIN) && !defined(OS_UNIX_NETBSD)
    struct passwd pwd;
    Nlm_Char      login_name[LOGNAME_MAX + 1];
    Nlm_Char      pwd_buffer[LOGNAME_MAX + PATH_MAX + 1024 + 1];
    Nlm_Boolean   ok = getlogin_r(login_name, sizeof(login_name)) ?
#  if NLM_POSIX1B
        FALSE : TRUE;
#  else
    TRUE : FALSE;
#  endif
#else

    Nlm_Char* login_name = getlogin();
    Nlm_Boolean ok       = (login_name != NULL);
#endif

    /* Get the info using user login-name */
    if ( !ok )
        return FALSE;

#if (defined(SOLARIS_THREADS_AVAIL) || defined(POSIX_THREADS_AVAIL)) && !defined(OS_UNIX_FREEBSD) && !defined(OS_UNIX_DARWIN) && !defined(OS_UNIX_NETBSD)
    pwd_ptr = &pwd;
#  if NLM_POSIX1B
    if (getpwnam_r(login_name, &pwd, pwd_buffer, sizeof(pwd_buffer),
                   &pwd_ptr) != 0) {
        pwd_ptr = 0;
    }
#  else
    pwd_ptr = getpwnam_r(login_name, &pwd, pwd_buffer, sizeof(pwd_buffer));
#  endif
#else
    pwd_ptr = getpwnam(login_name);
#endif

    if (!pwd_ptr  ||  pwd_ptr->pw_dir[0] == '\0')
        return FALSE;
    Nlm_StringNCpy_0(buf, pwd_ptr->pw_dir, buf_size);
    return TRUE;
}


static Nlm_Boolean Nlm_GetHome(Nlm_Char* buf, Nlm_Int2 buflen)
{
  static Nlm_Boolean s_Saved = FALSE;
  static Nlm_Char    s_SaveHome[PATH_MAX + 1];
  static TNlmMutex   s_SaveHomeMutex;

  /* Have we passed this way before?  If not, then try get the info. */
  if ( !s_Saved ) {
#if  defined(SOLARIS_THREADS_AVAIL)  ||  defined(POSIX_THREADS_AVAIL)
    VERIFY_HARD( NlmMutexLockEx( &s_SaveHomeMutex ) == 0 );
    if ( !s_Saved ) {
#endif
      s_Saved = TRUE;

      /* Try to retrieve the home dir -- first use user's ID, 
       * and if failed, then use user's login name.
       */
      if (!s_GetHomeByUID(s_SaveHome, sizeof(s_SaveHome))  &&
          !s_GetHomeByLOGIN(s_SaveHome, sizeof(s_SaveHome))) {
        s_SaveHome[0] = '\0';
      }
#if  defined(SOLARIS_THREADS_AVAIL)  ||  defined(POSIX_THREADS_AVAIL)
    }
    VERIFY_HARD( NlmMutexUnlock(s_SaveHomeMutex) == 0 );
#endif
  }
  Nlm_StringNCpy_0(buf, s_SaveHome, buflen);
  return (Nlm_Boolean) (*buf != '\0');
}



/*****************************************************************************
*
*   Nlm_OpenConfigFile (file, writeMode, create)
*      returns a file pointer to the specified configuration file.
*      1)  looks in the current directory for ".filerc", but will not
*          create a new file in this directory.
*      2)  then looks in the home directory for ".filerc".
*      3)  then looks for an environment variable "NCBI" and takes its
*          value as a complete path to a directory containing the
*          configuration file "filerc" or ".filerc".
*
*      Steps (1) and (2) above are omitted if the NCBI_DONT_USE_LOCAL_CONFIG
*      environment variable is set.  This can be used to allow specific
*      production applications to avoid stray .ncbirc files which may have
*      been erroneously generated.
*
*****************************************************************************/

static FILE* Nlm_OpenConfigFile(const Nlm_Char* file, Nlm_Boolean writeMode, Nlm_Boolean create)

{
  FILE      *fp;
  Nlm_Int2  i;
  Nlm_Int2  len;
  FILE      *newfp;
  Nlm_Char  path [PATH_MAX+1];
  char      *pth;
  Nlm_Char  str [FILENAME_MAX+1];
  Nlm_Boolean dontUseLocalConfig;

  fp = NULL;
  if (file != NULL) {
#ifdef OS_UNIX_DARWIN
    /* For Mach-O executables, check username/Library/Preferences/xxx.cnf first */
    Nlm_StringNCpy_0(str, file, sizeof(str) - 4);
    if ( ! Nlm_Qualified (str) ) {
        /* if the user has already supplied a name with .xxx use that name
         * otherwise add the .cnf here */
        Nlm_StringCat(str, ".cnf");
    }
    /* if the name isn't all lowercase, make it so now */
    len = (Nlm_Int2) Nlm_StringLen (str);
    for (i = 0; i < len; i++) {
      str [i] = TO_LOWER (str [i]);
    }
    if (Nlm_GetHome (path, sizeof (path))) {
      Nlm_FileBuildPath(path, "Library", NULL);
      Nlm_FileBuildPath(path, "Preferences", NULL);
      Nlm_FileBuildPath(path, NULL, str);
      fp = Nlm_FileOpen (path, "r");
      if (fp == NULL && create) {
        fp = Nlm_FileOpen (path, "w");
        Nlm_FileClose (fp);
        fp = Nlm_FileOpen (path, "r");
      }
      if (writeMode && fp != NULL) {
        Nlm_FileClose (fp);
        fp = Nlm_FileOpen (path, "w");
      }
      if (fp != NULL) {
        return fp;
      }
    }
    /* also check within Contents/Resources of Mac OS X package */
    ProgramPath (path, sizeof (path));
    if (IsApplicationPackage(path)) {
      FileBuildPath(path, "Contents", NULL);
      FileBuildPath (path, "Resources", NULL);
      Nlm_FileBuildPath (path, NULL, str);
      fp = Nlm_FileOpen (path, "r");
      if (fp != NULL) {
        return fp;
      }
    }
#endif
    dontUseLocalConfig = getenv("NCBI_DONT_USE_LOCAL_CONFIG") != NULL;
    newfp = NULL;
    Nlm_StringMove(str, ".");
    Nlm_StringNCat(str, file, sizeof(str) - 4);
    if ( ! Nlm_Qualified (str))
    { /* use the user's extension instead of the "rc" extension */
      Nlm_StringCat(str, "rc");
    }
    len = (Nlm_Int2) Nlm_StringLen (str);
    for (i = 0; i < len; i++) {
      str [i] = TO_LOWER (str [i]);
    }
    Nlm_StringNCpy_0(path, str, sizeof(path));

    if (! dontUseLocalConfig)
      fp = Nlm_FileOpen (path, "r");
    if (fp == NULL) {
      if (Nlm_GetHome (path, sizeof (path))) {
        Nlm_FileBuildPath(path, NULL, str);
      } else {
        Nlm_StringNCpy_0(path, str, sizeof(path));
      }
      if (! dontUseLocalConfig)
        fp = Nlm_FileOpen (path, "r");
      if (fp == NULL && create) {
        newfp = Nlm_FileOpen (path, "w");
        Nlm_FileClose (newfp);
        newfp = Nlm_FileOpen (path, "r");
      }
    }
    if (fp == NULL) {
      path[0] = '\0';
      pth = getenv ("NCBI");
      if (pth != NULL) {
        Nlm_FileBuildPath(path, pth, str + 1);
        fp = Nlm_FileOpen (path, "r");
        if (fp == NULL) {
          path[0] = '\0';
          Nlm_FileBuildPath(path, pth, str);
          fp = Nlm_FileOpen (path, "r");
        }
      }
    }
    if (newfp != NULL) {
      if (fp != NULL) {
        Nlm_FileClose (newfp);
        newfp = NULL;
      } else {
        fp = newfp;
      }
    }
    if (writeMode && fp != NULL) {
      Nlm_FileClose (fp);
      fp = Nlm_FileOpen (path, "w");
    }
  }
  return fp;
}

#endif /* OS_UNIX */


#if defined(OS_MAC) && !defined(OS_UNIX_DARWIN)
/*****************************************************************************
 *
 *   Nlm_OpenConfigFile (file, writeMode, create)
 *
 *       file        char string with name of file to open or optionally create
 *       writeMode   don't know what this does, but all calling functions seem
 *                   to pass FALSE??
 *       create      Boolean to create the file if it doesn't already exist
 *
 *  Returns:
 *      A ncbi FILE pointer to the specified configuration file.  NULL if all
 *      fails for some reason
 *
 *      1)  Finds "System Folder:Preferences" for "file.cnf"
 *
 *  Implementation Notes:
 *      Implicit assumptions:
 *      System softare is 6.0.5 or newer (Gestalt)
 *      System softare is 7.0 or newer (FindFolder)
 *
 *  We find the active preferences folder and either open a pre-existing file
 *  or create a new one with type 'TEXT' and creator '    '.  The absolute
 *  pathname is not derived or needed, since we can use HSetVol to perform the
 *  MacOS equivalent of a "cd" command.
 *
 *   pchurchill 12/10/99
 *
 *****************************************************************************/

static FILE* 
Nlm_OpenConfigFile(const Nlm_Char* file,
                   Nlm_Boolean writeMode,
                   Nlm_Boolean create )
{
    Nlm_Char    str [FILENAME_MAX+1];
    Nlm_Int2    len;
    long        gesResponse;
    OSErr       err;
    long        dirID, saveDirID;
    short       vRefNum, saveVRefNum;
    FSSpec      spec;
    FILE        *fp = NULL;
    int         i;

    if( file == NULL || *file == '\0' ){
        return NULL;
    }

    /* copy no more than (FILENAME_MAX - 4) to allow for the length of
     * our postfix */
    Nlm_StringNCpy_0(str, file, sizeof(str) - 4);
    if ( ! Nlm_Qualified (str) ) {
        /* if the user has already supplied a name with .xxx use that name
         * otherwise add the .cnf here */
        Nlm_StringCat(str, ".cnf");
    }
    /* if the name isn't all lowercase, make it so now */
    len = (Nlm_Int2) Nlm_StringLen (str);
    for (i = 0; i < len; i++) {
      str [i] = TO_LOWER (str [i]);
    }
    
    /* convert to pascal string for Mac toolbox */
    Nlm_CtoPstr( str);

    /* Make sure we can use FindFolder() if not, then report error and
     * return NULL */
    if ( Gestalt (gestaltFindFolderAttr, &gesResponse) != noErr ||
        (gesResponse & (1 << gestaltFindFolderPresent) == 0)) {
        /* notify user of the error */
        Nlm_Message(MSG_OK,
                    "We need Mac OS 7.0 or newer, continue at your own risk.");
        return NULL;
    }

    /* store the current active directory */
    HGetVol( (StringPtr) 0, &saveVRefNum, &saveDirID);

    /* first look for file in "system", then "preferences".  Only create it
     * in prefs if both of those fail... */
    err = FindFolder(kOnSystemDisk, kSystemFolderType,
                       kDontCreateFolder, &vRefNum, &dirID);
    if (err == noErr) {
        err = FSMakeFSSpec( vRefNum, dirID, (StringPtr)str, &spec);
    }

    if( err != noErr){
        /* i.e. file not in "system"
         * find the preferences folder in the active System folder */
        err = FindFolder(kOnSystemDisk, kPreferencesFolderType,
                       kCreateFolder, &vRefNum, &dirID);
        if (err == noErr) {
            err = FSMakeFSSpec( vRefNum, dirID, (StringPtr)str, &spec);
        }
    }

    /* convert to back to C string for fopen */
    Nlm_PtoCstr( str);

    if( err == noErr){      /* the file is already there */
        HSetVol( (StringPtr) 0, vRefNum, dirID);
        if (writeMode) {
            fp = fopen (str, "w");
        } else {
            fp = fopen (str, "r");
        }
        HSetVol( (StringPtr) 0, saveVRefNum, saveDirID);
    }
    else if( err == fnfErr && create){
        /* no file with that name was found, create one */
        err = FSpCreate( &spec, '    ', 'TEXT', smSystemScript);
        if( err == noErr){
            /* set the default directory (same as doing "cd" in unix)
             * and actually open the file */
            HSetVol( (StringPtr) 0, vRefNum, dirID);
            fp = fopen (str, "w");
            HSetVol( (StringPtr) 0, saveVRefNum, saveDirID);
        }
        if( fp == NULL){
            Nlm_Message( MSG_OK, 
                         "Couldn't create the preferences file, "
                         "is the boot volume locked?");
        }
    }
  return fp;
}
#endif /* defined(OS_MAC) && !defined(OS_UNIX_DARWIN) */

#ifdef OS_VMS
/*****************************************************************************
*
*   Nlm_GetHome (buf, buflen)
*      returns the path of the home directory
*
*****************************************************************************/

static Nlm_Boolean Nlm_GetHome(Nlm_Char* buf, Nlm_Int2 buflen)
{
  Nlm_StringNCpy_0(buf, getenv("SYS$LOGIN"), buflen);
  return TRUE;
}

/*****************************************************************************
*
*   Nlm_OpenConfigFile (file, writeMode, create)
*      returns a file pointer to the specified configuration file.
*      1)  looks in the current directory for "file.cfg", but will not
*          create a new file in this directory.
*      2)  then looks in the home directory for "file.cfg".
*      3)  then looks for an environment variable "NCBI" and takes its
*          value as a complete path to a directory containing the
*          configuration file "file.cfg".
*
*****************************************************************************/

static FILE* Nlm_OpenConfigFile(const Nlm_Char* file, Nlm_Boolean writeMode, Nlm_Boolean create)

{
  FILE      *fp;
  Nlm_Int2  i;
  Nlm_Int2  len;
  FILE      *newfp;
  Nlm_Char  path [PATH_MAX+1];
  char      *pth;
  Nlm_Char  str [FILENAME_MAX+1];

  fp = NULL;

  if (file != NULL) {
    newfp = NULL;
    Nlm_StringNCpy_0(str, file, sizeof(str) - 4);
    if ( ! Nlm_Qualified (str) ) {
      Nlm_StringCat (str, ".cfg");
    }
    len = (Nlm_Int2) Nlm_StringLen (str);
    for (i = 0; i < len; i++) {
      str [i] = TO_LOWER (str [i]);
    }
    Nlm_StringNCpy_0(path, str, sizeof(path));

    fp = Nlm_FileOpen (path, "r");  /* File exists? */
    if (fp == NULL) {
      if (Nlm_GetHome (path, sizeof (path))) {
        Nlm_FileBuildPath(path, NULL, str);
      } else {
        Nlm_StringNCpy_0(path, str, sizeof(path));
      }
      fp = Nlm_FileOpen (path, "r");   /* File exists? */
      if (fp == NULL && create) {
        newfp = Nlm_FileOpen (path, "w");
        Nlm_FileClose (newfp);
        newfp = Nlm_FileOpen (path, "r");
      }
    }

    if (fp == NULL) {
      path[0] = '\0';
      pth = getenv ("NCBI");
      if (pth != NULL) {
        Nlm_FileBuildPath(path, pth, str);
        fp = Nlm_FileOpen (path, "r");
      }
    }

    if (newfp != NULL) {
      if (fp != NULL) {
        Nlm_FileClose (newfp);
        newfp = NULL;
      } else {
        fp = newfp;
      }
    }

    /*
    ** On VMS if a file is opened for write a new version is created.
    ** This section of code check for "writeMode" and an existing file
    ** if both are true.  Get the currently open file's name and delete
    ** it.  Open a new one in write mode.
    **
    ** Side effects: This will replace the highest existing file version,
    ** but not older version.  There exists the possibility that a user's
    ** custom change may get lost.  A possible workaround for this would
    ** be to have the calling program make a new copy (one higher version)
    ** of the existing file before doing extensive write to the params
    ** file OR keep a static flag in this routine which does  delete the
    ** first time time.
    */

    if (writeMode && fp != NULL) {
      char temp[256];
      fgetname(fp,temp);
      Nlm_FileClose (fp);
      delete(temp);
      fp = Nlm_FileOpen (path, "w");
    }
  }
  return fp;
}

#endif /* OS_VMS */


/*****************************************************************************
*
*   Nlm_TrimString (str)
*      strips trailing spaces, \r, \n
*
*****************************************************************************/

static Nlm_Char* Nlm_TrimString(Nlm_Char* str)

{
  Nlm_Char   ch;
  Nlm_Char*  spc;
  Nlm_Char*  tmp;

  if (str != NULL) {
    ch = *str;
    while (ch == ' ' || ch == '\t') {
      str++;
      ch = *str;
    }
    tmp = str;
    spc = NULL;
    ch = *tmp;
    while (ch != '\0' && ch != '\r' && ch != '\n') {
      if (ch == ' ' || ch == '\t') {
        if (spc == NULL) {
          spc = tmp;
        }
      } else {
        spc = NULL;
      }
      tmp++;
      ch = *tmp;
    }
    *tmp = '\0';
    if (spc != NULL) {
      *spc = '\0';
    }
  }
  return str;
}

/*****************************************************************************
*
*   Nlm_ReadConfigFile (fp)
*      reads parameters from configuration file to memory structure
*
*****************************************************************************/

static Nlm_Boolean Nlm_ReadConfigFile(FILE* fp)

{
  Nlm_Char         ch;
  Nlm_env_itemPtr  eip;
  Nlm_env_sectPtr  esp;
  Nlm_env_itemPtr  lastEip;
  Nlm_env_sectPtr  lastEsp;
  Nlm_Char*      mid;
  Nlm_Char         str [256];
  Nlm_Char*      tmp;
  Nlm_Char*      comment;

  if (fp != NULL) {
    if (Nlm_envList != NULL) {
      Nlm_FreeEnvData (Nlm_envList);
      Nlm_envList = NULL;
    }
    esp = NULL;
    lastEsp = NULL;
    eip = NULL;
    lastEip = NULL;
    comment = NULL;
    while (Nlm_FileGets (str, sizeof (str), fp)) {
      ch = *str;
      if (ch != '\n' && ch != '\r') {
        if (ch == ';') { /* comment */
          if (comment == NULL) { /* first comment */
             comment = Nlm_StringSave(str);
          }
          else { /* append to existing comment */
             tmp = (Nlm_Char*) Nlm_MemNew(StrLen(comment) + StrLen(str) + 1);
             StrCpy(tmp, comment);
             StrCat(tmp, str);
             comment = (Nlm_Char*) Nlm_MemFree(comment);
             comment = tmp;
          }
        } else if (ch == '[') {
          if (esp == NULL) {
            esp = (Nlm_env_sectPtr) Nlm_MemNew (sizeof (Nlm_env_sect));
            lastEsp = esp;
            Nlm_envList = esp;
          } else {
            esp = (Nlm_env_sectPtr) Nlm_MemNew (sizeof (Nlm_env_sect));
            lastEsp->next = esp;
            lastEsp = esp;
          }
          esp->comment = comment;
          comment = NULL;
          tmp = str;
          ch = *tmp;
          while (ch != '\0' && ch != ']') {
            tmp++;
            ch = *tmp;
          }
          *tmp = '\0';
          esp->name = Nlm_StringSave (str + 1);
          eip = NULL;
          lastEip = NULL;
        } else if (esp != NULL) {
          if (eip == NULL) {
            eip = (Nlm_env_itemPtr) Nlm_MemNew (sizeof (Nlm_env_item));
            lastEip = eip;
            esp->children = eip;
          } else {
            eip = (Nlm_env_itemPtr) Nlm_MemNew (sizeof (Nlm_env_item));
            lastEip->next = eip;
            lastEip = eip;
          }
          eip->comment = comment;
          comment = NULL;
          tmp = str;
          mid = str;
          ch = *tmp;
          while (ch != '\0' && ch != '\n' && ch != '\r') {
            if (ch == '=' && mid == str) {
              mid = tmp;
              *mid++ = '\0';
            }
            tmp++;
            ch = *tmp;
          }
          *tmp = '\0';
          eip->name = Nlm_StringSave (Nlm_TrimString (str));
          eip->value = Nlm_StringSave (Nlm_TrimString (mid));
        }
      }
    }

    /* any comments which appeared after the final key of the final section */
    Nlm_bottomComment = (Nlm_Char*)Nlm_MemFree(Nlm_bottomComment);
    Nlm_bottomComment = comment;
  }
  return TRUE;
}

static Nlm_env_sectPtr Nlm_FindConfigSection(const Nlm_Char* section)
{
  Nlm_env_sectPtr esp;

  if (section == NULL)
    return NULL;

  for (esp = Nlm_envList; esp != NULL; esp = esp->next)
  {
    if (esp->name != NULL && Nlm_StringICmp(section, esp->name) == 0)
       return esp;
  }

  return NULL;
}

/*****************************************************************************
*
*   Nlm_FindConfigItem (section, type, create)
*      finds parameter in memory structure
*
*****************************************************************************/

static Nlm_env_itemPtr Nlm_FindConfigItem(const Nlm_Char* section, const Nlm_Char* type, Nlm_Boolean create)

{
  Nlm_env_itemPtr  eip;
  Nlm_env_sectPtr  esp;
  Nlm_Boolean      goOn;
  Nlm_env_itemPtr  lastEip;
  Nlm_env_sectPtr  lastEsp;

  eip = NULL;
  if (section != NULL && type != NULL) {
    goOn = TRUE;
    esp = Nlm_envList;
    lastEsp = esp;
    while (esp != NULL && goOn) {
      if (esp->name != NULL && Nlm_StringICmp (section, esp->name) == 0) {
        goOn = FALSE;
      } else {
        lastEsp = esp;
        esp = esp->next;
      }
    }
    if (goOn && create) {
      if (Nlm_envList != NULL) {
        esp = (Nlm_env_sectPtr) Nlm_MemNew (sizeof (Nlm_env_sect));
        if (esp != NULL) {
          esp->name = Nlm_StringSave (section);
          lastEsp->next = esp;
        }
      } else {
        esp = (Nlm_env_sectPtr) Nlm_MemNew (sizeof (Nlm_env_sect));
        if (esp != NULL) {
          esp->name = Nlm_StringSave (section);
        }
        Nlm_envList = esp;
      }
    }
    if (esp != NULL) {
      eip = esp->children;
      if (eip != NULL) {
        goOn = TRUE;
        lastEip = eip;
        while (eip != NULL && goOn) {
          if (eip->name != NULL && Nlm_StringICmp (type, eip->name) == 0) {
            goOn = FALSE;
          } else {
            lastEip = eip;
            eip = eip->next;
          }
        }
        if (goOn && create) {
          eip = (Nlm_env_itemPtr) Nlm_MemNew (sizeof (Nlm_env_item));
          if (eip != NULL) {
            eip->name = Nlm_StringSave (type);
            lastEip->next = eip;
          }
        }
      } else if (create) {
        eip = (Nlm_env_itemPtr) Nlm_MemNew (sizeof (Nlm_env_item));
        if (eip != NULL) {
          eip->name = Nlm_StringSave (type);
          esp->children = eip;
        }
      }
    }
  }
  return eip;
}

/*****************************************************************************
*
*   Nlm_WriteConfigFile (fp)
*      writes parameters to configuration file from memory structure
*
*****************************************************************************/

static Nlm_Boolean Nlm_WriteConfigFile(FILE* fp)

{
  Nlm_env_itemPtr  eip;
  Nlm_env_sectPtr  esp;

  if (Nlm_envList != NULL && fp != NULL) {
    esp = Nlm_envList;
    while (esp != NULL) {
      if (! destroyDeadComments || esp->name != NULL)
      {
        Nlm_PutComment (esp->comment, fp);
      }
      if (esp->name != NULL)
      {
        fputc ('[', fp);
        fputs (esp->name, fp);
        fputs ("]\n", fp);
      }
      eip = esp->children;
      while (eip != NULL) {
        if (! destroyDeadComments)
        {
          Nlm_PutComment (eip->comment, fp);
        }
        if (esp->name != NULL && eip->name != NULL && eip->value != NULL) {
          if (destroyDeadComments)
          {
            Nlm_PutComment (eip->comment, fp);
          }
          fputs (eip->name, fp);
          fputc ('=', fp);
          fputs (eip->value, fp);
          fputc ('\n', fp);
        }
        eip = eip->next;
      }
      if (esp->name != NULL)
      {
        fputc ('\n', fp);
      }
      esp = esp->next;
    }
  }

  if (fp != NULL)
    Nlm_PutComment(Nlm_bottomComment, fp);

  return TRUE;
}

/*****************************************************************************
*
*   Nlm_FreeConfigFileData ()
*      frees parameter structure in memory
*
*****************************************************************************/

static void Nlm_FreeConfigFileData(void)

{
  mustCreateFile = TRUE;
  Nlm_bottomComment = (Nlm_Char*) Nlm_MemFree(Nlm_bottomComment);
  if (Nlm_lastParamFile != NULL)
    Nlm_lastParamFile = (Nlm_Char*) Nlm_MemFree(Nlm_lastParamFile);

  if (Nlm_envList != NULL) {
    Nlm_FreeEnvData (Nlm_envList);
    Nlm_envList = NULL;
  }
}


/*****************************************************************************
*
*   Nlm_FlushConfigFile()
*      flush the specified file's parameters to disk
*
*****************************************************************************/

static void Nlm_FlushConfigFile(const Nlm_Char* file, Nlm_Boolean create)
{
  FILE* fp;

  if (dirty && file != NULL)
  {
    fp = Nlm_OpenConfigFile (file, TRUE, create);
    if (fp != NULL) {
      Nlm_WriteConfigFile (fp);
      Nlm_FileClose (fp);
    }
  }
  dirty = FALSE;
}

/*****************************************************************************
*
*   Nlm_FreeConfigStruct ()
*      frees parameter structure in memory, and perform other cleanup
*
*****************************************************************************/

static void Nlm_FreeConfigStruct_ST(void)
{
  Nlm_FlushAppParam();
  Nlm_FreeConfigFileData ();
  Nlm_FreeTransientData ();
}


/*****************************************************************************
*
*   Nlm_PutComment()
*      output a comment to the config file
*
*****************************************************************************/

static void Nlm_PutComment(const Nlm_Char* s, FILE* fp)
{
  if (s != NULL)
    fputs(s, fp);
}

#else /* ndef OS_MSWIN */

static void        Nlm_FlushAppParam_ST(void) {}
static Nlm_Boolean Nlm_CacheAppParam_ST(Nlm_Boolean value)  { return TRUE; }

/*****************************************************************************
*
* The "guts" of:
*   Nlm_GetAppParam (file, section, type, buf, buflen)
*      finds parameters from configuration files
*      if configuration file is found, trys to read the parameter from it.
*
*****************************************************************************/
static Nlm_Int2 Nlm_WorkGetAppParam(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* dflt, Nlm_Char* buf, Nlm_Int2 buflen, Nlm_Boolean searchTransient)
{
  static char _empty_string[] = "";
  Nlm_Char path[PATH_MAX + 1];

  if (buf == NULL  ||  buflen <= 0)
    return 0;

  *buf = '\0';
  if (searchTransient  &&
      Nlm_TransientLookup(file, section, type, dflt, buf, buflen))
    {
      return (Nlm_Int2)Nlm_StringLen(buf);
    }

  if ( dflt )
    Nlm_StringNCpy_0(buf, dflt, buflen);

  if (file != NULL && *file != '\0' && section != NULL && *section != '\0')
    {
      Nlm_StringNCpy_0(path, file, sizeof(path) - 4);
      if ( !Nlm_Qualified( path ) )
        Nlm_StringCat(path, ".INI");
      if (dflt == NULL)
        dflt = _empty_string;  /* can't use NULL, must be empty string */
      return (Nlm_Int2)GetPrivateProfileString(section,
                                               type, dflt, buf, buflen, path);
    }
  else
    return (Nlm_Int2)Nlm_StringLen( buf );
}

/*****************************************************************************
*
*   Nlm_SetAppParam (file, section, type, value)
*      finds paths for types of data and fills in path in buf
*      if configuration file is found, trys to write the parameter to it.
*
*****************************************************************************/

static Nlm_Boolean Nlm_SetAppParam_ST(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* value)
{
  Nlm_Char     path [PATH_MAX+1];
  Nlm_Boolean  rsult;

  rsult = FALSE;
  if (file != NULL && *file != '\0' && section != NULL && *section != '\0') {
    Nlm_StringNCpy_0(path, file, sizeof(path) - 4);
    if ( ! Nlm_Qualified (path) ) {
      Nlm_StringCat (path, ".INI");
    }
    Nlm_TransientLogSetApp (file, section, type, value);
    if (WritePrivateProfileString (section, type, value, path)) {
      rsult = TRUE;
    }
  }
  return rsult;
}

/*****************************************************************************
*   Nlm_FreeConfigStruct ()
*      frees parameter structure in memory
*****************************************************************************/
static void Nlm_FreeConfigStruct_ST(void)
{
  Nlm_FreeTransientData ();
}

#endif /* else !OS_MSWIN */


/*****************************************************************************
*   Nlm_Qualified ()
*      Appears to check if we've got an n.3 notation sting (i.e. if there is
*       a "." in the last 4 chars of the string passed)
*****************************************************************************/
static Nlm_Boolean Nlm_Qualified( const Nlm_Char* path )
{
  Nlm_Int4 l,k;
  const Nlm_Char*  p;

  l = Nlm_StrLen(path);
  p = path+l;
  k = 0;
  while (k < l && k <= 4) {
     if (*p-- == '.') return TRUE;
     k++;
  }
  return FALSE;
}


/*****************************************************************************
*
*   Nlm_FindPath (file, section, type, buf, buflen)
*      finds paths for types of data from configuration files.
*      if configuration file is found, tries to read the parameter from it,
*      then appends a directory delimiter character, if necessary.
*
*****************************************************************************/

NLM_EXTERN Nlm_Boolean Nlm_FindPath(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, Nlm_Char* buf, Nlm_Int2 buflen)   /* length of path buffer */
{
  Nlm_Boolean rsult = FALSE;

  if (file == NULL  ||  section == 0  ||  type == NULL  ||
      buf == NULL  ||  buflen <= 0)
    return FALSE;

  NlmMutexLockEx( &corelibMutex );

  *buf = '\0';
  if (*file != '\0'  &&  *section != '\0'  &&  *type != '\0'  &&
      Nlm_GetAppParam(file, section, type, "", buf, (Nlm_Int2)(buflen - 1))  &&
      *buf != '\0')
    {
      Nlm_FileBuildPath(buf, NULL, NULL);
      rsult = TRUE;
    }

  NlmMutexUnlock( corelibMutex );
  return rsult;
}


static Nlm_Boolean Nlm_TransientSetAppParam_ST(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* value)
{
  Nlm_env_filePtr  theFile;
  Nlm_env_itemPtr  eip;
  Nlm_env_sectPtr  esp;
  Nlm_env_itemPtr  nextEip;

  if (file == NULL || *file == '\0' || section == NULL || *section == '\0')
    return FALSE;

  for (theFile = Nlm_transientFileList; theFile != NULL; theFile = theFile->next)
  {
    if (StringICmp(theFile->name, file) == 0)
    {
      for (esp = theFile->envList; esp != NULL; esp = esp->next)
      {
        if (esp->name != NULL && StringICmp(esp->name, section) == 0)
        {
          if (type == NULL || type[0] == '\0')
          {
            /* free all children */
            for (eip = esp->children; eip != NULL; eip = nextEip)
            {
              nextEip = eip->next;
              eip->next    = (Nlm_env_itemPtr)(-1);
              eip->name    = (Nlm_Char*) Nlm_MemFree (eip->name);
              eip->comment = (Nlm_Char*) Nlm_MemFree (eip->comment);
              eip->value   = (Nlm_Char*) Nlm_MemFree (eip->value);
              Nlm_MemFree (eip);
            }
            esp->children = NULL;
            esp->transientOnly = TRUE;
          } else { /* append this type to the section */
            eip = (Nlm_env_itemPtr) MemNew(sizeof(*eip));
            eip->name = StringSave(type);
            eip->value = StringSave(value);
            eip->next = esp->children;
            esp->children = eip;
          }
          return TRUE;
        }
      }
      break;
    }
  }

  /* create the file data structure if needed */
  if (theFile == NULL)
  {
    theFile = (Nlm_env_filePtr) MemNew(sizeof(*theFile));
    theFile->name = StringSave(file);
    theFile->next = Nlm_transientFileList;
    Nlm_transientFileList = theFile;
  }

  /* create the section and type */
  esp = (Nlm_env_sectPtr) MemNew(sizeof(*esp));
  esp->name = StringSave(section);
  esp->next = theFile->envList;
  theFile->envList = esp;
  if (type == NULL || type[0] == '\0')
  {
    esp->transientOnly = TRUE;
  } else { /* create the section */
    esp->transientOnly = FALSE;
    eip = (Nlm_env_itemPtr) MemNew(sizeof(*eip));
    eip->name = StringSave(type);
    eip->value = StringSave(value);
    eip->next = NULL;
    esp->children = eip;
  }

  return TRUE;
}


/* SetAppParam is writing a value to the real config file, so log this value,
   if necessary, into the "transient" data structures */
static void Nlm_TransientLogSetApp(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* value)

{
  Nlm_env_filePtr  theFile;
  Nlm_env_itemPtr  eip;
  Nlm_env_sectPtr  esp;

  if (file == NULL || *file == '\0' || section == NULL || *section == '\0')
    return;

  if (type == NULL || type[0] == '\0')
  {
    for (theFile = Nlm_transientFileList; theFile != NULL; theFile = theFile->next)
    {
      if (StringICmp(theFile->name, file) == 0)
      {
        for (esp = theFile->envList; esp != NULL; esp = esp->next)
        {
          if (esp->name != NULL && StringICmp(esp->name, section) == 0)
          { /* delete the section by removing section name */
            esp->name = (Nlm_Char*) MemFree(esp->name);
          }
        }
      }
    }
  } else {
    for (theFile = Nlm_transientFileList; theFile != NULL; theFile = theFile->next)
    {
      if (StringICmp(theFile->name, file) == 0)
      {
        for (esp = theFile->envList; esp != NULL; esp = esp->next)
        {
          if (esp->name != NULL && StringICmp(esp->name, section) == 0 &&
              esp->transientOnly)
          { /* append this type to the section */
            eip = (Nlm_env_itemPtr) MemNew(sizeof(*eip));
            eip->name = StringSave(type);
            eip->value = StringSave(value);
            eip->next = esp->children;
            esp->children = eip;
          }
        }
      }
    }
  }
}

static Nlm_Boolean Nlm_TransientLookup(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* dflt, Nlm_Char* buf, size_t buflen)
{
  Nlm_env_filePtr  theFile;
  Nlm_env_itemPtr  eip;
  Nlm_env_sectPtr  esp;
  Nlm_Int4         totlen;
  Nlm_Int4         bytesToAppend;

  if (file == NULL || *file == '\0' || section == NULL || *section == '\0')
    return FALSE;

  for (theFile = Nlm_transientFileList; theFile != NULL; theFile = theFile->next)
  {
    if (StringICmp(theFile->name, file) == 0)
    {
      for (esp = theFile->envList; esp != NULL; esp = esp->next)
      {
        if (esp->name != NULL && StringICmp(esp->name, section) == 0)
        {
          if (type == NULL || type[0] == '\0')
          { /* concatenate all types (keys) within section */
            *buf = '\0';
            totlen = 0;
            for (eip = esp->children; eip != NULL; eip = eip->next)
            {
              bytesToAppend = StrLen(eip->name) + 1;
              bytesToAppend = MIN(bytesToAppend, (Nlm_Int4)buflen - totlen);
              StrNCpy(&buf[totlen], eip->name, bytesToAppend);
              totlen += bytesToAppend;
            }
            if (totlen > 0 && buf[totlen] == '\0')
            {
                totlen--; /* account for final null character */
            }
            /* now append the GetAppParam() data */
            if (! esp->transientOnly)
            { /* GetAppParam data can be trusted ... append it to buf */
              Nlm_WorkGetAppParam(file, section, NULL, "", &buf[totlen],
                                  (Nlm_Int2)(buflen - totlen), FALSE);
            }
            return TRUE;
          } else {
            for (eip = esp->children; eip != NULL; eip = eip->next)
            {
              if (StringICmp(eip->name, type) == 0)
              {
                Nlm_StringNCpy_0(buf, eip->value, buflen);
                return TRUE;
              }
            }
            if (esp->transientOnly)
            { /* GetAppParam data cannot be trusted ... use the default */
              Nlm_StringNCpy_0(buf, dflt, buflen);
              return TRUE;
            }
          }
        }
      }
    }
  }

  /* not found ... GetAppParam() should search the real config file */
  return FALSE;
}

static void Nlm_FreeEnvData(Nlm_env_sectPtr esp)

{
  Nlm_env_itemPtr  eip;
  Nlm_env_itemPtr  nextEip;
  Nlm_env_sectPtr  nextEsp;

  while (esp != NULL) {
    nextEsp = esp->next;
    eip = esp->children;
    while (eip != NULL) {
      nextEip = eip->next;
      eip->next    = (Nlm_env_itemPtr)(-1);
      eip->name    = (Nlm_Char*) Nlm_MemFree (eip->name);
      eip->comment = (Nlm_Char*) Nlm_MemFree (eip->comment);
      eip->value   = (Nlm_Char*) Nlm_MemFree (eip->value);
      Nlm_MemFree (eip);
      eip = nextEip;
    }
    esp->next    = (Nlm_env_sectPtr)(-1);
    esp->name    = (Nlm_Char*) Nlm_MemFree (esp->name);
    esp->comment = (Nlm_Char*) Nlm_MemFree (esp->comment);
    Nlm_MemFree (esp);
    esp = nextEsp;
  }
}


static void Nlm_FreeTransientData(void)
{
  Nlm_env_filePtr efp, nextEfp;

  efp = Nlm_transientFileList;
  while (efp != NULL) {
    nextEfp = efp->next;
    Nlm_FreeEnvData (efp->envList);
    efp->envList = NULL;
    efp->next = (Nlm_env_filePtr)(-1);
    efp->name = (Nlm_Char*) Nlm_MemFree (efp->name);
    Nlm_MemFree (efp);
    efp = nextEfp;
  }
  Nlm_transientFileList = NULL;
}



extern size_t Nlm_GetEnvParamEx
(const Nlm_Char* conf_file, const Nlm_Char* conf_section,
 const Nlm_Char* env_name, const Nlm_Char* conf_name,
 Nlm_Char* buf, size_t bufsize, const Nlm_Char* dflt)
{
  static const Nlm_Char s_DefConfigFile   [] = "ncbi";
  static const Nlm_Char s_DefConfigSection[] = "NCBI";

  size_t len = 0;
  ASSERT ( (env_name  &&  *env_name)  ||  (conf_name  &&  *conf_name) );
  ASSERT ( buf  &&  bufsize );
  buf[0] = '\0';

  /* arg fallbacks */
  if (conf_name  &&  *conf_name) {
    if ( !conf_file )
      conf_file = s_DefConfigFile;
    if ( !conf_section )
      conf_section = s_DefConfigSection;
  }

  /* Search in the list of transient parameters */
  if (conf_name  &&  *conf_name  &&
      Nlm_TransientLookup(conf_file, conf_section, conf_name,
                          dflt, buf, bufsize)) {
    return Nlm_StrLen(buf);
  }

  /* Fetch from the environment variable */
#if defined(OS_UNIX) || defined(OS_MSWIN)
  if (env_name  &&  *env_name) {
    const Nlm_Char* str = getenv(env_name);
    if (str  &&  *str) {
      len = Nlm_StrLen(str);
      if (len >= bufsize)
        len = 0;
      else
        Nlm_StrCpy(buf, str);
    }
  }
#endif

  /* Search in the configuration file */
  if (!len  &&  conf_name  &&  *conf_name) {
    len = GetAppParam(conf_file, conf_section, conf_name, dflt, buf, bufsize);
  }

  /* Store the value in the transient parameter list */
  ASSERT(len == Nlm_StrLen(buf));
  if ( len )
    VERIFY(Nlm_TransientSetAppParam(conf_file, conf_section, conf_name, buf));

  return len;
}


NLM_EXTERN size_t Nlm_GetEnvParam
(const Nlm_Char* conf_file, const Nlm_Char* conf_section,
 const Nlm_Char* env_conf_name,
 Nlm_Char* buf, size_t bufsize, const Nlm_Char* dflt)
{
  return Nlm_GetEnvParamEx(conf_file, conf_section,
                           env_conf_name, env_conf_name,
                           buf, bufsize, dflt);
}



/*****************************************************************************
*
*   Command-line arguments
*     &
*   Nlm_ProgramPath (buf, maxsize)
*   	returns full path to executing program
*
*****************************************************************************/

static Nlm_Boolean wasSetup = FALSE;
static int    targc = 0;
static char **targv = NULL;


#if defined(WIN_MAC)
static FSSpec       apFileSpec;
static Str255       apName;
static Handle       apParam;
static short        apRefNum;

static Nlm_Boolean Nlm_SetupArguments_ST_Mac(void)
{
#ifndef __GNUC__
/* At least in 10.1, this seems to introduce an unwanted dep. on Carbon. */
  ProcessInfoRec       pirec;
  ProcessSerialNumber  psn;

  GetCurrentProcess (&psn);
  pirec.processInfoLength = sizeof (ProcessInfoRec);
  pirec.processName = apName;
  pirec.processAppSpec = &apFileSpec;
  GetProcessInformation (&psn, &pirec);
  Nlm_PtoCstr ((Nlm_Char*) apFileSpec.name);
  Nlm_PtoCstr ((Nlm_Char*) apName);

  SetAppProperty("ProgramName",(void*)apName);
#endif
  return TRUE;
}

#if defined(OS_UNIX_DARWIN)
#ifndef __GNUC__
/* At least in 10.1, this seems to introduce an unwanted dep. on Carbon. */
static void Nlm_ProgramPath_ST(Nlm_Char* appPath, size_t pathSize)
{
    OSErr               err;
    ProcessSerialNumber psn;
    
    if (appPath != NULL) {
        FSRef       fsRef;
        appPath[0] = '\0';
        GetCurrentProcess (&psn);
        err = GetProcessBundleLocation (&psn, &fsRef);
        if (err == noErr) {
            FSRefMakePath (&fsRef, (UInt8 *) appPath, pathSize);
        }
    }
}
#endif

/*
  is the application at filePath actually a application bundle/package?
  i.e. a folder containing subfolders, resource files and the actual executable.
  Pass the value returned by ProgramPath for best results.
*/
NLM_EXTERN Nlm_Boolean Nlm_IsApplicationPackage(char *filePath)
{
    OSErr   err;
    char    aPath[1024];
    FSRef   contentsFRef;
    Boolean isDirectory;
    
    StrCpy(aPath, filePath);
    FileBuildPath(aPath, "Contents", NULL);
    err = FSPathMakeRef ((unsigned char *) aPath, &contentsFRef, &isDirectory);
    if (err == noErr  &&  isDirectory) {
        return TRUE;   
    }
    /* else gets err == -120, no such directory. */
    return FALSE;
}

#else
static void Nlm_ProgramPath_ST(Nlm_Char* buf, size_t maxsize)
{
  CInfoPBRec  block;
  Nlm_Char    path [256];
  Nlm_Char    temp [256];
  short nErr;

  if (buf != NULL && maxsize > 0) {
    *buf = '\0';
    if (wasSetup) {
      memset (&block, 0, sizeof (CInfoPBRec));
      Nlm_StringNCpy_0(path, (Nlm_Char*)apFileSpec.name, sizeof (path));

      block.dirInfo.ioNamePtr = (StringPtr) path;
      block.dirInfo.ioDrParID = apFileSpec.parID;

      do {
        Nlm_StringCpy (temp, path);
        block.dirInfo.ioVRefNum = apFileSpec.vRefNum;
        block.dirInfo.ioFDirIndex = -1;
        block.dirInfo.ioDrDirID = block.dirInfo.ioDrParID;
        nErr = PBGetCatInfo (&block, FALSE);
        if (nErr != noErr) break;
        Nlm_PtoCstr ((Nlm_Char*) path);
        Nlm_StringCat (path, DIRDELIMSTR);
        Nlm_StringCat (path, temp);
      } while (block.dirInfo.ioDrDirID != fsRtDirID);

      Nlm_StringNCpy_0(buf, path, maxsize);
    }
  }
}
#endif /* defined(OS_UNIX_DARWIN) */
#endif /* defined(WIN_MAC) */


#if defined(OS_MSWIN) || defined(OS_VMS)
static void Nlm_ProgramPath_ST(Nlm_Char* buf, size_t maxsize)
{
  if (!buf  ||  maxsize <= 0)
    return;

  *buf = '\0';
  if (wasSetup  &&  targv  &&  targv[0])
    Nlm_StringNCpy_0(buf, targv[0], maxsize);
}
#endif  /* OS_MSWIN || OS_VMS */


#if defined(OS_UNIX)  &&  (defined(__GNUC__)  ||  !defined(OS_UNIX_DARWIN))
static void Nlm_ProgramPath_ST(Nlm_Char* buf, size_t maxsize)
{
  Nlm_Char     path [PATH_MAX];
  Nlm_Char*  pth;
  Nlm_Char*  ptr;

  if (buf != NULL && maxsize > 0) {
    *buf = '\0';
    if (wasSetup) {
      ptr = targv [0];
      if (ptr [0] == DIRDELIMCHR) {
        Nlm_StringNCpy_0(buf, targv[0], maxsize);
      } else if (getcwd (path, sizeof (path)) != NULL) {
        ptr = targv [0];
        while (ptr [0] == '.' || ptr [0] == DIRDELIMCHR) {
          if (ptr [0] == '.') {
            if (ptr [1] == '.' && ptr [2] == DIRDELIMCHR) {
              ptr += 3;
              pth = StringRChr (path, DIRDELIMCHR);
              if (pth != NULL) {
                *pth = '\0';
              }
            } else if (ptr [1] == DIRDELIMCHR) {
              ptr += 2;
            } else {
              ptr++;
            }
          } else if (ptr [0] == DIRDELIMCHR) {
            ptr++;
          } else {
            ptr++;
          }
        }
        FileBuildPath (path, NULL, ptr);
        Nlm_StringNCpy_0(buf, path, maxsize);
      } else {
        Nlm_StringNCpy_0(buf, targv[0], maxsize);
      }
    }
  }
}
#endif


/*****************************************************************************
* Multi-Thread protected external functions
*****************************************************************************/

NLM_EXTERN Nlm_Boolean Nlm_TransientSetAppParam(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* value)
{
  Nlm_Boolean rsult;
  NlmMutexLockEx( &corelibMutex );

  rsult = Nlm_TransientSetAppParam_ST(file, section, type, value);

  NlmMutexUnlock( corelibMutex );
  return rsult;
}

NLM_EXTERN void Nlm_FreeConfigStruct(void)
{
  NlmMutexLockEx( &corelibMutex );
  Nlm_FreeConfigStruct_ST();
  NlmMutexUnlock( corelibMutex );
}


NLM_EXTERN Nlm_Boolean Nlm_ParseCmdLineArguments
(const char* prog_name, const char* cmd_line, int* argc_ptr, char*** argv_ptr,
 ECmdLineQuote quote_handling)
{
  char*  str;
  char*  p;
  int    xx_argc;
  char** xx_argv;

  /* Check args */
  if (!argc_ptr  ||  !argv_ptr)
    return FALSE;

  /* Figure out program name */
  if ( !prog_name )
    prog_name = (const char *) GetAppProperty("ProgramName");
  if ( !prog_name )
    prog_name = "";

  /* Special case -- no cmd.-line parameters */
  if ( cmd_line ) {
    const char* sss;
    for (sss = cmd_line;  *sss  &&  isspace(*sss);  sss++) continue;
    if ( !*sss )
      cmd_line = 0;
  }
  if ( !cmd_line ) {
    *argc_ptr = 1;
    *argv_ptr = (char**) Nlm_MemNew(2 * sizeof(char*));
    (*argv_ptr[0]) = Nlm_StringSave(prog_name);
    return TRUE;
  }

  /* Allocate string to hold "argv[]" values and fill it with
   * the program name and the command string */
  {{
    size_t size = strlen(prog_name) + strlen(cmd_line) + 2;
    str = (char*) Nlm_MemNew(size);

    StrCpy(str, prog_name);
    StrCpy(str + strlen(prog_name) + 1, cmd_line);
  }}

  /* Count command-line arguments and separate them by '\0' */
  xx_argc = 1;
  for (p = str + strlen(prog_name) + 1;  *p; ) {
    if ( isspace(*p) ) {
      *p++ = '\0';
      continue;
    }

    if (quote_handling == eProcessQuotes  &&  (*p == '\''  ||  *p == '"')) {
      char quote = *p;
      xx_argc++;
      while (*(++p)  &&  *p != quote) continue;
      if ( *p )
        *p++ = '\0';
      continue;
    }

    xx_argc++;
    while (*p  &&  !isspace(*p))
      p++;
  }

  /* Allocate and fill out "xx_argv" */
  {{
    int   n = 1;
    char *s = str + strlen(prog_name) + 1;
    xx_argv = (char**) Nlm_MemNew((xx_argc + 1) * sizeof(char*));
    xx_argv[0] = str;
    while (n < xx_argc) {
      while ( !*s )
        s++;
      if (quote_handling == eProcessQuotes  &&  (*s == '\''  ||  *s == '"'))
        s++; /* -- skip the leading quote */
      xx_argv[n++] = s;
      while ( *s )
        s++;
    }
    xx_argv[n] = 0;
#ifdef _DEBUG
    while (s < p  &&  !*s)
      s++;
    ASSERT ( s == p );
#endif
  }}

  *argc_ptr = xx_argc;
  *argv_ptr = xx_argv;
  return TRUE;
}


NLM_EXTERN void Nlm_FreeCmdLineArguments(char** argv)
{
  if ( !argv )
    return;

  ASSERT( argv[0] );
  Nlm_MemFree( argv[0] );
  Nlm_MemFree( argv );
}


NLM_EXTERN void Nlm_SetupArguments(int argc, char *argv[])
{
  NlmMutexLockEx( &corelibMutex );
  wasSetup = TRUE;
#if defined(WIN_MAC)
  wasSetup = Nlm_SetupArguments_ST_Mac();
#elif defined(OS_UNIX)
  {{
    char *p;
    if ((p = strrchr(argv[0],DIRDELIMCHR)) != NULL)  p++;
    else
      p = argv[0];
    SetAppProperty("ProgramName", (void*)p);  
  }}
#endif
  targc = argc;
  targv = argv;
  NlmMutexUnlock( corelibMutex );
}

NLM_EXTERN Nlm_Char** Nlm_GetArgv(void)
{
  return targv;
}

NLM_EXTERN Nlm_Int4 Nlm_GetArgc(void)
{
  return targc;
}

NLM_EXTERN void Nlm_ProgramPath(Nlm_Char* buf, size_t maxsize)
{
  NlmMutexLockEx( &corelibMutex );
  Nlm_ProgramPath_ST(buf, maxsize);
  NlmMutexUnlock( corelibMutex );
}

NLM_EXTERN void Nlm_FlushAppParam(void)
{
  NlmMutexLockEx( &corelibMutex );
  Nlm_FlushAppParam_ST();
  NlmMutexUnlock( corelibMutex );
}

NLM_EXTERN Nlm_Boolean Nlm_CacheAppParam(Nlm_Boolean value)
{
  Nlm_Boolean rsult;
  NlmMutexLockEx( &corelibMutex );

  rsult = Nlm_CacheAppParam_ST( value );

  NlmMutexUnlock( corelibMutex );
  return rsult;
}

NLM_EXTERN Nlm_Int2 Nlm_GetAppParam(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* dflt, Nlm_Char* buf, Nlm_Int2 buflen)
{
  Nlm_Int2 rsult;
  NlmMutexLockEx( &corelibMutex );

  rsult = Nlm_WorkGetAppParam(file, section, type, dflt, buf, buflen, TRUE);

  NlmMutexUnlock( corelibMutex );
  return rsult;
}

NLM_EXTERN Nlm_Boolean Nlm_SetAppParam(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* value)
{
  Nlm_Boolean rsult;
  NlmMutexLockEx( &corelibMutex );

  rsult = Nlm_SetAppParam_ST(file, section, type, value);

  NlmMutexUnlock( corelibMutex );
  return rsult;
}


/*****************************************************************************
*
*   GetAppParamBoolean()
*       SetAppParamBoolean()
*   GetAppParamLong()
*       SetAppParamLong()
*
*****************************************************************************/

NLM_EXTERN Nlm_Boolean GetAppParamBoolean
(const Nlm_Char* filebase, const Nlm_Char* sect, const Nlm_Char* key,
 Nlm_Boolean dflt)
{
  Nlm_Char buffer[32];
  if ( GetAppParam(filebase, sect, key, "", buffer, sizeof(buffer)) ) {
    if (strchr("1yYtT",buffer[0]))
      return TRUE;
    if (strchr("0nNfF",buffer[0]))
      return FALSE;
  }
  return dflt;
}

NLM_EXTERN Nlm_Boolean SetAppParamBoolean
(const Nlm_Char* filebase, const Nlm_Char* sect, const Nlm_Char* key,
 Nlm_Boolean value)
{
  return SetAppParam(filebase, sect, key, value ? "Yes" : "No");
}

NLM_EXTERN long GetAppParamLong(const Nlm_Char* filebase, const Nlm_Char* sect,
                                const Nlm_Char* key, long dflt)
{
  Nlm_Char buffer[32];
  return GetAppParam(filebase, sect, key, "", buffer, sizeof(buffer)) ?
    atol(buffer) : dflt;
}

NLM_EXTERN Nlm_Boolean SetAppParamLong
(const Nlm_Char* filebase, const Nlm_Char* sect, const Nlm_Char* key,
 long value)
{
	char buffer[32];
	sprintf(buffer,"%ld",value);
	return SetAppParam((char*)filebase,(char*)sect,(char*)key,buffer);
}


#ifdef WIN32
extern int Nlm_x_HasConsole(void)
{
  static int has_console = -1;
  if (has_console == -1)
    has_console = fileno(stdin) >= 0 ? 1 : 0;

  return has_console;
}
#endif


NLM_EXTERN Nlm_Boolean Nlm_FreeArgs(Nlm_Int2 numargs, Nlm_ArgPtr ap)
{
  Nlm_Int2 i;
  for (i = 0;  i < numargs;  i++, ap++)
    {
      switch ( ap->type )
        {
        case ARG_BOOLEAN:
          ap->intvalue = 0;
          break;
        case ARG_INT:
          ap->intvalue = 0;
          break;
        case ARG_FLOAT:
          ap->floatvalue = 0.0;
          break;
        case ARG_STRING:
        case ARG_FILE_IN:
        case ARG_FILE_OUT:
        case ARG_DATA_IN:
        case ARG_DATA_OUT:
          ap->strvalue = (Nlm_Char*) Nlm_MemFree( ap->strvalue );
          break;
        default:
          ASSERT ( FALSE );
          return FALSE;
        }
    }
  return TRUE;
}

NLM_EXTERN Nlm_CharPtr Nlm_GetOpSysString (void)

{
  Nlm_CharPtr  str = "unknown";

#if defined(OS_MAC) && !defined(OS_UNIX_DARWIN)
  long  sysVer;

  if ( Gestalt (gestaltSystemVersion, &sysVer) == noErr) {
    /* system version in low order word is hexadecimal */
    if (sysVer >= 4096) {
      str = "MAC Carbon on OS X";
    } else if (sysVer >= 2304) {
      str = "MAC Carbon on OS 9";
    } else {
      str = "MAC Carbon on OS 8";
    }
  }
#endif

#ifdef OS_UNIX
  /* initial nonspecific UNIX string */
  str = "UNIX";
#ifdef OS_UNIX_DARWIN
  str = "MAC UNIX on OS X";
#endif
#ifdef OS_UNIX_SYSV
  str = "SYSV UNIX";
#endif

#ifdef OS_UNIX_SOL
  str = "SOLARIS UNIX";
#ifdef PROC_I80X86
  str = "SOLARIS INTEL UNIX";
#endif
#endif

#ifdef OS_UNIX_SUN
  str = "SUN UNIX";
#endif
#ifdef OS_UNIX_IRIX
  str = "SGI UNIX";
#endif
#ifdef OS_UNIX_LINUX
  str = "LINUX UNIX";
#endif
#endif

#ifdef OS_MSWIN
  DWORD  version, lowbyte;

  str = "MS WINDOWS";
  version = GetVersion ();
  lowbyte = (version & 0x0000FF);
  if ((version & 0x80000000) == 0) {
    if (lowbyte == 5) {
      str = "MS WINDOWS 2000/XP";
    } else if (lowbyte == 4) {
      str = "MS WINDOWS NT 4.0";
    } else if (lowbyte == 3) {
      str = "MS WINDOWS NT 3.51";
    }
  } else {
    if (lowbyte == 4) {
      str = "MS WINDOWS 95/98/Me";
    } else if (lowbyte == 3) {
      str = "MS WINDOWS 3.1";
    }
  }
#endif

  return Nlm_StringSave (str);
}

