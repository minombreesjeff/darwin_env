/*   ncbierr.c
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
* $Id: ncbierr.c,v 6.21 2003/03/25 14:56:08 ivanov Exp $
*
* $Revision: 6.21 $
*
* Authors:  Schuler, Sirotkin (UserErr stuff)
*
* Version Creation Date:   9-19-91
*
* File Description:  Error handling functions
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 04-13-93 Schuler     Added TRACE, VERIFY, ASSERT macros.
* 04-13-93 Schuler     Added AbnormalExit function.
* 05-11-93 Schuler     Added ErrSetHandler function.
* 05-21-93 Schuler     Remove PROTO from ErrSetHandler implementation (oops)
* 05-26-93 Schuler     Nlm_TraceStr flushes stderr, if appropriate
* 07-26-93 Schuler     Moved globals into a (per-app) context struct
* 07-26-93 Schuler     Fixed ErrClear()
* 11-15-93 Schuler     Fixed double error reporting problem in ErrPost()
* 12-13-93 Schuler     Message file support and lots of other changes.
* 12-22-93 Schuler     Added log_level setting (min. severity to log)
* 01-03-94 Schuler     Added ErrSaveOptions and ErrRestoreOptions
* 01-04-94 Schuler     Added code to get settings from NCBI config file
* 01-07-94 Schuler     Fixed bug in ErrMsgRoot_Read().
* 01-10-94 Schuler     Added check for not_avail in ErrMsgRoot_fopen()
* 01-10-94 Schuler     Fixed bug in ErrGetMsgRoot()
* 01-13-94 Schuler     Added an handful of typecasts to supress warnings
* 01-23-94 Schuler     Fixed bug in ErrSetOpts
* 02-02-94 Schuler     ErrOpt structure has fields for actopt, logopt
* 02-02-94 Schuler     Revisions related to change in message file format
* 02-02-94 Schuler     Use TEST_BITS macro throughout
* 02-02-94 Schuler     Improved back.compatability of ErrGetOpts/ErrSetOpts
* 02-10-94 Schuler     Workaround for obsolete ERR_IGNORE option
* 02-10-94 Schuler     Fixed bug in ErrSetFatalLevel
* 02-18-94 Schuler     Fix to deal with possibility of userstrings being NULL
* 03-23-94 Schuler     Fix memory leak in ErrGetExplanation
* 04-19-94 Schuler     Keep error log file open all the time
* 08-23-94 Schuler     ErrLogStr, ErrLogPrintStr modified
* 08-09-94 Schuler     Dont try to open message file if FileLength ==0
* 11-15-94 Schuler     Tweaks to ErrPostStr to avoid acc errors
* 11-18-94 Schuler     Changed return type of Get/SetXXXLevel() family
*                      of functions from int to ErrSev to eliminate warnings
* 03-06-95 Schuler     Fixed problem with ErrMsgRoot_fopen
*
* $Log: ncbierr.c,v $
* Revision 6.21  2003/03/25 14:56:08  ivanov
* Nlm_AbnormalExitPure(): FatalAppExit() call changed to exit() under MS Windows (by Howard Feldman feldman@mshri.on.ca)
*
* Revision 6.20  2002/10/03 16:22:03  kans
* changed fgets to Nlm_FileGets
*
* Revision 6.19  2002/08/13 20:44:19  lavr
* Force logging to stderr/stdout if DEBUG_PRINTOUT env.var. is set as follows:
* STDOUT (case-insensitively) - log to stdout; anything else - log to stderr.
* By Denis Vakatov and Anton Lavrentiev.
*
* Revision 6.18  2002/07/11 20:26:03  lavr
* Prevent possible buffer overruns in storing text parts of a message
*
* Revision 6.17  2001/02/08 22:27:04  vakatov
* + Nlm_CallErrHandlerOnly() -- to allow processing of posted
* err.messages by the hook only.
*
* Revision 6.16  2000/06/14 22:27:52  vakatov
* Nlm_AbnormalExit() -- print message only if called for the first time
*
* Revision 6.15  1999/10/01 14:41:39  kans
* added SEV_REJECT between SEV_ERROR and SEV_FATAL
*
* Revision 6.14  1999/08/23 19:14:34  vakatov
* Let SEV_MAX not be an exception among the "SEV_*"
*
* Revision 6.13  1998/12/29 06:16:21  vakatov
* Nlm_ErrPostStr() -- Check for the severity code validity
*
* Revision 6.12  1998/10/22 16:52:40  shavirin
* Added function *Nlm_GetErrLongText able to retrieve long error messages
*
* Revision 6.11  1998/09/29 16:20:53  vakatov
* Nlm_AssertionFailed():  printout the module name and file/line in Message
*
* Revision 6.10  1998/08/24 17:42:00  kans
* fixed old style function definition warnings
*
* Revision 6.9  1998/06/11 21:02:23  shavirin
* Fixed few warnings
*
* Revision 6.8  1998/05/04 23:25:46  vakatov
* Raised the message posting level to SEV_ERROR(for non-_DEBUG only)
*
* Revision 6.7  1998/02/24 19:06:19  vakatov
* [DEBUG_HARD] Nlm_ErrShow(): init. "*message"(by garyt@timelogic.com)
*
* Revision 6.6  1998/02/17 20:57:45  vakatov
* Fixed delete_ErrNode() and started to use it; use Nlm_ErrUserClear()
* too(got rid of mem.leaks) // in cooperation with S.Bazhin.
*
* Revision 6.5  1998/02/13 15:22:40  vakatov
* ReleaseAppErrInfo():  dont forget to destroy list of error message;
* +fixed a minor mem.leak(free _szSeverity[SEV_MAX])
*
* Revision 6.4  1997/12/04 22:00:15  vakatov
* Nlm_ErrPostEx():  dont forget to zero "info->busy" flag when do nothing
*
* Revision 6.3  1997/11/26 21:26:14  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.2  1997/11/04 19:08:25  ostell
* added ErrGetUserStrings() to eliminate direct access to ErrDesc.userstr
* so this could become a thread specific chain
*
* Revision 6.1  1997/08/27 20:40:23  kans
* added ErrPathReset
*
* Revision 6.0  1997/08/25 18:15:24  madden
* Revision changed to 6.0
*
* Revision 5.12  1997/07/22 19:11:22  vakatov
* Separated Main() from GetArg[svc]() functions;  [WIN_MSWIN] converged
* console and GUI libraries; [for WIN32-DLL] encapsulated global variables
*
* Revision 5.11  1997/03/31 18:52:11  vakatov
* Minor fix to pass through the C++ compiler
*
 * Revision 5.10  1997/03/17  21:51:01  vakatov
 * Nlm_ErrSetLogfile():  put the logfile banner as a monolith string
 *
 * Revision 5.9  1997/01/29  16:52:48  vakatov
 * Nlm_ErrSetLogfile():  fixed an unsafe StrNCpy() use(might produce
 * a non-'\0'-terminated string)
 *
 * Revision 5.8  1996/12/17  05:49:04  vakatov
 * Got rid of some ugly warnings (JK)
 *
 * Revision 5.7  1996/12/16  22:37:54  vakatov
 * Added more protection from recursive error posing function calls. Rolled
 * back the changes made in "* Revision 5.6  1996/12/11  23:01:48  vakatov"
 *
 * Revision 5.6  1996/12/11  23:01:48  vakatov
 * ErrMsgRoot_fopen(): Suppressed error posting when opening "*.msg"
 * (often non-existing) auxiliary files.
 *
 * Revision 5.5  1996/12/03  21:48:33  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.4  1996/11/25  19:03:58  vakatov
 * Moved 'eo_save'(formerly MT-unsafe 'static _eo_save') and
 * 'busy'(formerly MT-unsafe 'static _busy') to the 'AppErrInfo' structure.
 * Added Nlm_ErrGetLogfile() and Nlm_AbnormalExitPure();  the latter
 * does nothing but exit without any error posting, etc.
 * "MT-safed" error context initialization and posting functions.
 *
 * Revision 5.3  1996/08/12  18:40:31  vakatov
 * Nlm_ErrPostEx(), Nlm_ErrPostEx() -- prevented from crashing when an
 * empty formatting string specified(<fmt> === "")
 *
 * Revision 5.2  1996/08/07  19:24:31  vakatov
 * [_DEBUG]  Nlm_AbnormalExit():  "exit()" replaced by "abort()"
 *
 * Revision 5.1  1996/07/16  19:56:37  vakatov
 * VSPRINTF macros has been replaced by the TSPRINTF macros in the
 * code responsible for the error message composing -- in order to
 * avoid the memory limits overrun when printing the error messages
 * (by the means of function "vsprintf()") to a memory-located string.
 * The application name is added as the error message caption.
 * Added ReleaseAppErrInfo() function to clean dynamically allocated
 * memory used by the error posing routines;  other memory-leak-
 * resulting flaws fixed; and eliminated multiple(duplicated) reading
 * of the parameter file.
 *
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.50  1995/06/27  16:21:45  kans
 * protect ErrPostEx against single strings > ERRTEXT_MAX in length
 *
 * Revision 2.49  1995/05/15  18:45:58  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/


static char *_filename = __FILE__;
#define THIS_FILE _filename

#include "corepriv.h"
#include <tsprintf.h>

char * g_corelib = "CoreLib";

struct AppErrInfo
{
	ErrDesc desc;
	ErrOpts opts;
	ErrHookProc	hook;
	unsigned long ini_mask;
	unsigned long ini_bits;
	unsigned int any_error :1;
	unsigned int err_formatted :1;
	unsigned int debug_mode :1;
	size_t ustrcnt;
	size_t ustrlen;
	FILE *fdLog;
	char  logfile[PATH_MAX];
	char  msgpath[PATH_MAX];
	ErrMsgRoot *idxlist;
	ErrMsgNode *node;
        ErrOpts _eo_save[2];
#define AEI_BUSY_1  0x1
#define AEI_BUSY_2  0x2
#define AEI_BUSY_3  0x4
        int busy;
};


typedef struct AppErrInfo *AppErrInfoPtr;


static char * _szPropKey = "_AppErrInfo";

static char * _szSevKey [SEV_MAX + 1] = {
  "", "SEV_INFO", "SEV_WARNING", "SEV_ERROR", "SEV_REJECT", "SEV_FATAL", "SEV_MAX"};

static char * _szSevDef [SEV_MAX + 1] = {
  "", "NOTE:", "WARNING:", "ERROR:", "REJECT:", "FATAL ERROR:", "FATAL ERROR:" };

static char * _szSeverity[SEV_MAX + 1];

static AppErrInfoPtr GetAppErrInfo PROTO((void));

#define FUSE_BITS(inf)  ( ((inf)->ini_bits & (inf)->ini_mask) | ((inf)->opts.flags & ~((inf)->ini_mask)) )
#define TEST_BITS(inf,x)  (FUSE_BITS(inf) & (x))


static Nlm_Boolean s_HookOnly = FALSE;



/***************************************************************************\
|                           POSTING AN ERROR                                |
\***************************************************************************/

static void die_if_necessary(ErrSev sev, AppErrInfoPtr info)
{
  if (sev >= info->opts.die_level)
    Nlm_AbnormalExitPure( info->desc.errcode );
}


/*-------------------------------------------------------------------------
* ErrPost   [Schuler]
*
* MODIFICATIONS:
* 04-13-93 Schuler
* 07-26-93 Schuler   Modified to use AppErrInfo struct
* 11-15-93 Schuler   Fixed bug that resulted in reporting errors twice
* 12-14-93 Schuler   Modified to call the new ErrPostStr function
* 01-14-94 Schuler   Check _busy flag
*/

#ifdef VAR_ARGS
NLM_EXTERN void CDECL  Nlm_ErrPost (context, errcode, fmt, va_alist)
  int context;
  int errcode;
  const char *fmt;
  va_dcl
#else
NLM_EXTERN void CDECL Nlm_ErrPost (int context, int errcode, const char *fmt, ...)
#endif
{
  AppErrInfoPtr info = GetAppErrInfo();
  ErrSev sev = (context==CTX_DEBUG) ? SEV_INFO : SEV_FATAL;

  if ( info->busy )
    {
      die_if_necessary(sev, info);
      return;
    }
  info->busy = AEI_BUSY_1;

  info->desc.context = (short)context;

  {{
    const Nlm_Char PNTR scratch_str = NULL;
    TSPRINTF(scratch_str, fmt);
    if (scratch_str == NULL)
      {
        ErrClear();
        return;
      }
    Nlm_StrNCpy(info->desc.errtext, scratch_str,
                sizeof(info->desc.errtext) - 1);
    info->desc.errtext[sizeof(info->desc.errtext) - 1] = '\0';
  }}

  info->err_formatted = 1;
  Nlm_ErrPostStr(sev,errcode,0,NULL);

  info->busy = 0;
}


/*-------------------------------------------------------------------------
*	ErrPostEx   [Schuler, 12-13-93]
*
* MODIFICATIONS:
* 01-14-94 Schuler   Check _busy flag
*/

#ifdef VAR_ARGS
NLM_EXTERN int CDECL  Nlm_ErrPostEx (sev, lev1, lev2, fmt, va_alist)
  ErrSev sev;
  int lev1;
  int lev2;
  const char *fmt;
  va_dcl
#else
NLM_EXTERN int CDECL Nlm_ErrPostEx (ErrSev sev, int lev1, int lev2, const char *fmt, ...)
#endif
{
  AppErrInfoPtr info = GetAppErrInfo();

  if ( info->busy )
    {
      die_if_necessary(sev, info);
      return ANS_NONE;
    }
  info->busy = AEI_BUSY_1;

  {{
    const Nlm_Char PNTR scratch_str = NULL;
    TSPRINTF(scratch_str, fmt);
    if (scratch_str == NULL) {
      ErrClear();
      info->busy = 0;
      return ANS_NONE;
    }
    Nlm_StrNCpy(info->desc.errtext, scratch_str,
                sizeof(info->desc.errtext) - 1);
    info->desc.errtext[sizeof(info->desc.errtext) - 1] = '\0';
  }}

  info->err_formatted =1;
  {{
    int code = Nlm_ErrPostStr(sev,lev1,lev2,NULL);
    info->busy = 0;
    return code;
  }}
}


/*-------------------------------------------------------------------------
* ErrPostStr   [Schuler, 12-13-93]
*
* MODIFICATIONS:
* 12-22-93 Schuler   Only logs error if severity >= log_level
* 01-14-94 Schuler   Check _busy flag
* 02-03-94 Schuler   Use severity from message file if there is one
* 02-10-94 Schuler   Workaround for obsolete ERR_IGNORE option
* 02-18-94 Schuler   Check for NULL user strings before printing
* 08-23-94 Schuler   Clear the err_formatted flag after use
* 11-15-94 Schuler   De-const some pointers that need to be lvalues
*/

NLM_EXTERN int LIBCALL Nlm_ErrPostStr (ErrSev sev, int lev1, int lev2, const char *str)
{
  AppErrInfoPtr info = GetAppErrInfo();
  ErrMsgRoot *root  = NULL;
  ErrMsgNode *node1 = NULL;
  ErrMsgNode *node2 = NULL;

  int severity;
  if (sev < 0  ||  SEV_MAX < sev)
    sev = SEV_FATAL;
  severity = sev;

  if (info->busy & AEI_BUSY_2)
    {
      die_if_necessary(sev, info);
      return ANS_NONE;
    }
  info->busy |= AEI_BUSY_2;

  /* ----- Fill in the fields of the error descriptor ----- */
  info->any_error = 1;
  info->desc.severity = (short)sev;
  info->desc.errcode = lev1;
  info->desc.subcode = lev2;

  /* ----- set up the root and node for message file ----- */
  if ( (info->desc.module[0] != '\0') )
    {
      root = (ErrMsgRoot*) ErrGetMsgRoot(info->desc.module);
      for (node1=root->list; node1; node1=node1->next)
        {
          if (node1->code == info->desc.errcode)
            {
              for (node2=node1->list; node2; node2=node2->next)
                if (node2->code == info->desc.subcode)
                  break;
              break;
            }
        }
    }
  info->desc.root = root;
  info->desc.node = node2 ? node2 : node1;
  if (info->desc.node != NULL && info->desc.node->sev != SEV_NONE)
    severity = info->desc.node->sev;

  /* ----- format some strings ----- */
  if (!info->err_formatted)
    {
      info->desc.errtext[0] = '\0';
      if (str != NULL)
        strncat(info->desc.errtext,str,ERRTEXT_MAX - 1);
    }
  info->err_formatted = 0;

  if (info->desc.context != 0)
    sprintf(info->desc.codestr,"[%03d:%03d] ",info->desc.context,info->desc.errcode);
  else if (node1 != NULL && (TEST_BITS(info,EO_XLATE_CODES)))
    {
      if (node2 != NULL)
        sprintf(info->desc.codestr,"[%s.%s] ",node1->name,node2->name);
      else
        sprintf(info->desc.codestr,"[%s] ",node1->name);
    }
  else
    {
      sprintf(info->desc.codestr,"[%03d.%03d] ",info->desc.errcode,info->desc.subcode);
    }


  /* ----- Log the error according to the current options ----- */
  if (!s_HookOnly  &&  severity >= info->opts.log_level)
    {
      NlmMutexLockEx( &corelibMutex );

#ifdef WIN_DUMB
      if (TEST_BITS(info,EO_LOGTO_STDOUT))
        fflush(stderr);
      if (TEST_BITS(info,EO_LOGTO_STDERR))
        fflush(stdout);
#endif

      if (TEST_BITS(info,EO_LOG_SEVERITY))
        {
          ErrLogPrintf("%s ",_szSeverity[severity]);
        }
      if (TEST_BITS(info,EO_LOG_CODES))
        {
          if (info->desc.module[0])
            ErrLogPrintf("%s ",info->desc.module);
          ErrLogPrintStr(info->desc.codestr);
        }
      if (TEST_BITS(info,EO_LOG_FILELINE))
        {
          ErrLogPrintf("{%s, line %d} ",info->desc.srcfile,info->desc.srcline);
        }
      if (TEST_BITS(info,EO_LOG_USERSTR))
        {
          const ValNode *node;
          for (node=info->desc.userstr; node; node=node->next)
            {
              if (node->data.ptrvalue != NULL)
                ErrLogPrintf("%s ",(char*)node->data.ptrvalue);
            }
        }
      if (TEST_BITS(info,EO_LOG_ERRTEXT))
        {
          ErrLogPrintStr(info->desc.errtext);
          ErrLogPrintStr("\n");
        }
      if (node1 != NULL && TEST_BITS(info,EO_LOG_MSGTEXT))
        {
          ErrMsgNodePtr node = (node2==NULL) ? node1 : node2;
          const char *text = ErrGetExplanation(root,node);
          ErrLogPrintStr(text);
        }

      NlmMutexUnlock( corelibMutex );
    }

  info->busy &= ~AEI_BUSY_2;

  /* ----- Call the user-installed hook function if there is one ----- */
  if (info->hook != NULL)
    {
      int retval;
      if ((retval = (*info->hook)(&info->desc)) != 0)
        {
          ErrClear();
          return retval;
        }
    }

  if ( s_HookOnly ) {
      die_if_necessary(sev, info);
      ErrClear();
      return ANS_NONE;
  }

  /* ----- Workaround for obsolete ERR_IGNORE option ----- */
  if (info->opts.actopt == ERR_IGNORE)
    {
      ErrClear();
      return ANS_NONE;
    }

  /* ----- If not already handled, perform default error handling ----- */
  if (severity >= info->opts.msg_level || severity >= info->opts.die_level)
    return ErrShow();

  return ANS_OK;
}


/*-------------------------------------------------------------------------
*	ErrLogPrintf   [Schuler, 12-13-93]
*
*  Formats the string and passes it along to ErrLogPrintStr().
*/
#ifdef VAR_ARGS
NLM_EXTERN void CDECL Nlm_ErrLogPrintf (fmt, va_alist)
  const char *fmt;
  va_dcl
#else
NLM_EXTERN void CDECL Nlm_ErrLogPrintf (const char *fmt, ...)
#endif
{
  const Nlm_Char PNTR scratch_str = NULL;
  TSPRINTF(scratch_str, fmt);
  if (scratch_str != NULL)
    ErrLogPrintStr( scratch_str );
}


/*-------------------------------------------------------------------------
* ErrLogPrintStr   [Schuler, 12-13-93]
*
* Sends a string of text to whichever output streams have been enabled
* for error logging (stderr, trace, or logfile).
*
* MODIFICATIONS
* 12-15-93 Schuler   Added fflush(stdout) before writing to stderr.
* 04-19-94 Schuler   Open file if not already open, but don't close
* 08-23-94 Schuler   stdout/stderr output only #ifndef _WINDLL
*/
NLM_EXTERN void LIBCALL Nlm_ErrLogPrintStr (const char *str)
{
  AppErrInfoPtr info;
  size_t bytes;

  if (str == NULL  ||  *str == '\0'  ||
      (info = GetAppErrInfo()) == NULL  ||  info->busy & AEI_BUSY_3)
    return;
  info->busy |= AEI_BUSY_3;

  bytes = strlen( str );

#ifdef WIN_DUMB
  {{
      static unsigned long s_ForceBits = EO_ALL_FLAGS;
      if (s_ForceBits == EO_ALL_FLAGS) {
          char buf[64];
          static const char s_SecretDefault[] = "_NoOneCanGuessThisValue_Duh_";
          Nlm_GetEnvParam(0, 0, "DEBUG_PRINTOUT",
                          buf, sizeof(buf), s_SecretDefault);
          if (Nlm_StrCmp(buf, s_SecretDefault) == 0) {
              s_ForceBits = 0;
          } else if (Nlm_StrICmp(buf, "stdout") == 0) {
              s_ForceBits = EO_LOGTO_STDOUT;
          } else {
              s_ForceBits = EO_LOGTO_STDERR;
          }
      }

      if ((s_ForceBits & EO_LOGTO_STDOUT)  ||  TEST_BITS(info,EO_LOGTO_STDOUT))
          {
              fflush(stderr);
              fwrite(str,1,bytes,stdout);
              fflush(stdout);
          }

      if ((s_ForceBits & EO_LOGTO_STDERR)  ||  TEST_BITS(info,EO_LOGTO_STDERR))
          {
              fflush(stdout);
              fwrite(str,1,bytes,stderr);
              fflush(stderr);
          }
  }}
#endif

  info->busy &= ~AEI_BUSY_3;

  if (TEST_BITS(info,EO_LOGTO_TRACE))
    {
      Nlm_TraceStr(str);
    }

  if (TEST_BITS(info,EO_LOGTO_USRFILE))
    {
      if (info->fdLog == NULL  &&  info->logfile[0] != '\0')
        info->fdLog = FileOpen(info->logfile, "a+");

      if (info->fdLog != NULL)
        {
          fwrite(str, 1, bytes, info->fdLog);
          fflush(info->fdLog);
        }
    }
}


/*-------------------------------------------------------------------------
*	ErrSetContext   [Schuler, 12-13-93]
*
*  NOTE: Don't call this function directly
*
* MODIFICATIONS
* 02-03-94 Schuler   Return 1 if busy
*/
NLM_EXTERN int LIBCALL Nlm_ErrSetContext (const char *ctx, const char *fname, int line, int db,
                                        Nlm_Uint2 entityID, Nlm_Uint2 itemID, Nlm_Uint2 itemtype)
{
	AppErrInfoPtr info = GetAppErrInfo();

        if ( info->busy )
          return 1;

	info->desc.module[0] = '\0';
	if (ctx != NULL)
		strncat(info->desc.module,ctx,MODSTR_MAX - 1);
	info->desc.srcfile[0] = '\0';
	if (fname != NULL)
	{
		const char *p;
		if ((p = strrchr(fname,DIRDELIMCHR)) != NULL)
			++p;
		else
			p = fname;
		strncat(info->desc.srcfile,p,SRCFILE_MAX - 1);
	}
	info->desc.srcline = line;
	info->desc.entityID = entityID;
	info->desc.itemID = itemID;
	info->desc.itemtype = itemtype;
	info->debug_mode = (unsigned)db;
	return 0;
}


/***************************************************************************\
|                     FETCHING AND REPORTING ERRORS                         |
\***************************************************************************/

/*-------------------------------------------------------------------------
 * ErrFetch  [Schuler]
 *
 * MODIFICATIONS
 * 07-26-93 Schuler   Modified to use AppErrInfo struct
 */
NLM_EXTERN int LIBCALL  Nlm_ErrFetch (ErrDesc *err)
{
	if (!Nlm_ErrCopy(err))
		return FALSE;
   	ErrClear();
	return TRUE;
}


/*-------------------------------------------------------------------------
 * ErrCopy  [Schuler, 07-26-93]
 *
 * MODIFICATIONS:
 * 12-12-93 Schuler   Check info->any_error flag before copying
 */
NLM_EXTERN int LIBCALL  Nlm_ErrCopy (ErrDesc FAR *err)
{
	AppErrInfoPtr info = GetAppErrInfo();
	if (info->any_error)
	{
		if (err != NULL)
			memcpy((void*)err,(void*)&(info->desc),sizeof(ErrDesc));
		return TRUE;
	}
	return FALSE;
}


/*-------------------------------------------------------------------------
 * ErrClear   [Schuler, 07-26-93]
 *
 * MODIFICATIONS
 * 12-13-93 Schuler   Just clear flags instead of zeroing the whole struct.
 */
NLM_EXTERN void LIBCALL Nlm_ErrClear (void)
{
	AppErrInfoPtr info = GetAppErrInfo();
	info->any_error = 0;
	info->err_formatted = 0;
	info->debug_mode = 0;
	info->desc.context = 0;
	info->desc.entityID = 0;
	info->desc.itemID = 0;
	info->desc.itemtype = 0;
}

/*-------------------------------------------------------------------------
* ErrShow   [Schuler]
*
* MODIFICATIONS
* 12-13-93 Schuler   Rewritten to use msg_level and die_level settings.
* 12-14-93 Schuler   Now returns the result of Message().
* 12-15-93 Schuler   Added proper handling of Abort/Retry/Ignore choice.
* 12-21-93 Schuler   Added special case for WIN_DUMB + logging to stderr
* 12-24-94 Schuler   Changed Message to MsgAlert
* 12-27-94 Schuler   Removed Beep() because MsgAlert is taking care of it
* 01-31-94 Schuler   Put Beep() back because MsgAlert is no longer doing it.
* 02-03-94 Schuler   Now honors all EO_MSG_... option flags.
* 02-18-94 Schuler   Check for NULL user strings before copying.
*
* TO DO
* - allocate buffer for the message
*/
NLM_EXTERN int LIBCALL Nlm_ErrShow (void)
{
  AppErrInfoPtr info = GetAppErrInfo();
  if (info->any_error)
    {
      int answer = ANS_OK;
      int severity = info->desc.severity;
      if (info->desc.node != NULL && info->desc.node->sev != SEV_NONE)
        severity = info->desc.node->sev;
      if (severity >= info->opts.msg_level)
        {
          size_t bytes;
          const char *caption = (char*)GetAppProperty("ProgramName");
          char *message;
          const char *msgtext;
          char *p;
          MsgKey key;

          /* ----- beep if requested ----- */
          if (TEST_BITS(info,EO_BEEP))
            Nlm_Beep();

          /* ----- set up the buffer to hold the error message ---- */
          if (TEST_BITS(info,EO_MSG_MSGTEXT))
            msgtext = ErrGetExplanation(info->desc.root,info->desc.node);
          else
            msgtext = NULL;
          /* forget about the options for now -- this is the max we would need*/
          bytes = 2 + strlen(info->desc.module)
            + 32    /* severity string.  TEMPORARY (need to keep track of longest string) */
            + 2  + strlen(info->desc.codestr)  /* error codes */
            + 16 + strlen(info->desc.srcfile)  /* source file and line number */
            + 2  + info->ustrcnt + info->ustrlen  /* user strings */
            + 2  + strlen(info->desc.errtext)  /* error message */
            + 2  + ((msgtext) ? strlen(msgtext) : 0);  /* verbose message */

          if ((message = (char*)Malloc(bytes)) == NULL)
            message = info->desc.errtext;
          else
            {
              /* ----- format the message in the buffer ----- */
              *message = '\0';
              p = message;
              if (TEST_BITS(info,EO_MSG_SEVERITY))
                {
                  NlmMutexLockEx( &corelibMutex );
                  p = strchr(strcpy(p,_szSeverity[severity]),'\0');
                  NlmMutexUnlock( corelibMutex );
                  *p++ = ' ';
                }
              if (TEST_BITS(info,EO_MSG_CODES))
                {
                  p = strchr(strcpy(p,info->desc.module),'\0');
                  *p++ = ' ';
                  p = strchr(strcpy(p,info->desc.codestr),'\0');
                  *p++ = ' ';
                }
              if (TEST_BITS(info,EO_MSG_FILELINE))
                {
                  /*p = strchr(strcpy(p,info->desc.srcfile),'\0');*/
                  sprintf(p,"{%s line %d} \n",info->desc.srcfile,info->desc.srcline);
                  p = strchr(p,'\0');
                }
              if (TEST_BITS(info,EO_MSG_USERSTR))
                {
                  const ValNode *node;
                  for (node=info->desc.userstr; node; node=node->next)
                    {
                      if (node->data.ptrvalue != NULL)
                        {
                          p = strchr(strcpy(p,(char*)node->data.ptrvalue),'\0');
                          *p++ = ' ';
                        }
                    }
                }
              if (TEST_BITS(info,EO_MSG_ERRTEXT))
                {
                  p = strchr(strcpy(p,info->desc.errtext),'\0');
                }
              if (TEST_BITS(info,EO_MSG_MSGTEXT) && msgtext!=NULL)
                {
                  if (p != message)  *p++ = '\n';
                  p = strchr(strcpy(p,msgtext),'\0');
                }
              ASSERT_HARD ( strlen(p) < bytes );
            }

          /* ----- show the message ----- */
          if (TEST_BITS(info,EO_PROMPT_ABORT)) key = KEY_ARI;
          else if (TEST_BITS(info,EO_WAIT_KEY)) key = KEY_OK;
          else key = KEY_NONE;
          answer = MsgAlertStr(key,(ErrSev)severity,caption,message);

          /* ----- clean up ----- */
          if (message != info->desc.errtext)
            Free(message);
        }

      /* ----- die if appropriate ----- */
      die_if_necessary((ErrSev)severity, info);

      ErrClear();
      return answer;
    }
  return 0;
}



/***************************************************************************\
|                           MESSAGE FILE SUPPORT                            |
\***************************************************************************/
static FILE * ErrMsgRoot_fopen PROTO((ErrMsgRoot *ctx));
static ErrMsgNode * new_ErrMsgNode PROTO((const char *name, int code, ErrSev sev));
static void delete_ErrMsgNode PROTO((ErrMsgNode *item));
static ErrSev atosev PROTO((const char *sevstr));


static ErrMsgRoot * new_ErrMsgRoot (const char *context)
{
	ErrMsgRoot *idx = (ErrMsgRoot*) MemNew(sizeof(ErrMsgRoot));
	if (idx != NULL)
          idx->name = (context) ? StrSave(context) : 0;
	return idx;
}

static void delete_ErrMsgRoot(ErrMsgRoot *idx)
{
  if ( idx->name )
    MemFree((void *)idx->name);
  if ( idx->name )
    delete_ErrMsgNode(idx->list);
  MemFree(idx);
}


/*-------------------------------------------------------------------------
* ErrMsgRoot_fopen    [Schuler, 12-09-93]
*
* Opens an error message file corresponding to an error context.
*
* MODIFICATIONS
* 01-07-94 Schuler   Looks in the current directory first.
* 01-10-94 Schuler   Uses FileOpen() instead of fopen() (now that
*                    reentrancy problem is solved).
* 09-08-94 Schuler   Don't try to open file if FileLength ==0
* 03-06-95 Schuler   Eliminated the mode argument to this function.  The
*                    required for proper operation depends on the platform.
*/

#if defined(OS_MSWIN) || defined(OS_NT)
static char * s_msg_mode = "rb";
#else
static char * s_msg_mode = "r";
#endif

#define MODFNAME_MAX 32


static FILE * ErrMsgRoot_fopen (ErrMsgRoot *ctx)
{
	AppErrInfoPtr info = GetAppErrInfo();
	FILE *fd;
	char file[MODFNAME_MAX];
	char path[PATH_MAX];
	char *p1 = file;
	const char *p2 = ctx->name;
	int i, ch;

	for (i=0; (ch= *p2++) != 0 && i<MODFNAME_MAX-5; ++i)
	{
		if (isalpha(ch)) ch = tolower(ch);
		*p1++ = (char)ch;
	}
	strcpy(p1,".msg");

	if (FileLength(file)==0 || (fd = FileOpen(file,s_msg_mode)) == NULL)
          {
            strcpy(path,info->msgpath);
            strncat(path,file,sizeof(path));
            fd = FileOpen(path,s_msg_mode);
          }

	return fd;
}


/*-------------------------------------------------------------------------
* ErrMsgRoot_Read    [Schuler, 12-08-93]
*
* Scans an error message file and builds a data structure that contains
* the mappings between integer error codes and strings as well as file
* offset for the verbose error explanations so they can later be retrieved
* when (or if) needed.
*
* MODIFICATIONS
* 01-07-94 Schuler   Fixed bug that resulted in failure to set the text
*                    length for the last item in the file.
* 01-10-94 Schuler   Added _busy flag to indicate that we are in the process
*                    of parsing the file.  Otherwise, there is a problem if
*                    you post an error while attempting to read corelib.msg
*                    (leads to infinite recursion).
* 01-10-94 Schuler   Added linenumber reporting for all error messages.
* 01-23-94 Schuler   Changed all ErrPostEx to ErrLogPrintf (don't want to
*                    be posting errors in here -- will recurse ad infinitum).
* 01-21-94 Schuler   Changed fclose() to FileClose()
* 02-02-94 Schuler   Revisions for new file format
*
* TO DO
* Check for missing integer code
* Capture comments
*/

static ErrSev atosev (const char *sevstr)
{
	int i;
	if (sevstr)
	{
		for (i=SEV_MIN;  i <= SEV_MAX;  ++i)
		{
			if (strcmp(sevstr,_szSevKey[i]) == 0)
				return (ErrSev)i;
		}
	}
	return SEV_NONE;
}

static int ErrMsgRoot_Read (ErrMsgRoot *idx)
{
  FILE *fd;

  if (idx->list != NULL)
    return TRUE;
  if ( idx->busy )
    return FALSE;

  idx->busy = 1;

  if ((fd = ErrMsgRoot_fopen(idx)) == NULL)
    {
      idx->busy = 0;
      return FALSE;
    }

  {{
    char line[80], *p;
    int  linenum = 0;
    long tofs = 0, cofs = 0, tmpofs;
    ErrMsgNode *lev1;
    ErrMsgNode *lev2;
    int any_text;
    int any_comt;

    /* ----- Look for MODULE line ----- */
    while (Nlm_FileGets(line,sizeof line,fd))
      {
        linenum++;
        if ( strchr("#\r\n", line[0]) )
          continue;

        if (strncmp(line,"MODULE",6) == 0)
          {
            p = strtok(line," \t\r\n");
            p = strtok(NULL," \t\r\n");
            if (strcmp(idx->name,p) !=0)
              ErrLogPrintf("Context string mismatch (%s vs %s)\n",idx->name,p);
            break;
          }
        else
          {
            idx->busy = 0;
            ErrLogPrintf("Syntax error: \"MODULE\" expected, line %d\n",linenum);
            return FALSE;
          }
      }

    /* ----- Process the rest of the file ----- */
    any_text = any_comt = FALSE;
    lev1 = lev2 = NULL;
    tmpofs = ftell(fd);

    while (Nlm_FileGets(line,sizeof line,fd))
      {
        linenum++;

        if (line[0] == '$')
          {
            if (any_text)
              {
                ErrMsgNode *n = lev2 ? lev2 : lev1;
                if (n != NULL)
                  {
                    n->tofs = tofs;
                    n->tlen = tmpofs - tofs;
                  }
                any_text = FALSE;
              }
          }

        if (line[0]=='$' && line[1]=='$')
          {
            /*** NEED ERROR CHECKING HERE ***/
            char *tok1 = strtok(line+2,", \t\r\n");
            char *tok2 = strtok(NULL,", \t\r\n");
            char *tok3 = strtok(NULL,", \t\r\n");

            ASSERT(tok1 && tok2);

            lev2 = NULL;
            lev1 = new_ErrMsgNode(tok1,atoi(tok2),atosev(tok3));
            if (idx->list == NULL)
              idx->list = lev1;
            else
              {
                ErrMsgNode *t0=NULL, *t1;
                for (t1=idx->list; TRUE; t1=t1->next)
                  {
                    if (t1==NULL || lev1->code < t1->code)
                      {
                        if (t0 == NULL)
                          idx->list = lev1;
                        else
                          t0->next = lev1;
                        lev1->next = t1;
                        break;
                      }
                    if (lev1->code == t1->code)
                      {
                        ErrLogPrintf("Code %d duplicated, line %d\n",t1->code,linenum);
                        break;
                      }
                    t0 = t1;
                  }
              }
            /*any_text = FALSE;*/
          }
        else if (line[0]=='$' && line[1]=='^')
          {
            /*** NEED ERROR CHECKING HERE ***/
            char *tok1 = strtok(line+2,", \t\r\n");
            char *tok2 = strtok(NULL,", \t\r\n");
            char *tok3 = strtok(NULL,", \t\r\n");

            ASSERT(tok1 && tok2);

            lev2 = new_ErrMsgNode(tok1,atoi(tok2),atosev(tok3));
            if (lev1->list == NULL)
              lev1->list = lev2;
            else
              {
                ErrMsgNode *t0=NULL, *t1;
                for (t1=lev1->list; TRUE; t1=t1->next)
                  {
                    if (t1==NULL || lev2->code < t1->code)
                      {
                        if (t0 == NULL)
                          lev1->list = lev2;
                        else
                          t0->next = lev2;
                        lev2->next = t1;
                        break;
                      }
                    if (lev2->code == t1->code)
                      {
                        ErrLogPrintf("Code %d duplicated, line %d\n",t1->code,linenum);
                        break;
                      }
                    t0 = t1;
                  }
              }
            /*any_text = FALSE;*/
          }
        else if (line[0] == '#')
          {
            if (!any_comt)
              {
                cofs = tmpofs;
                any_comt = TRUE;
              }
          }
        else
          {
            if (any_comt)
              {
                ErrMsgNode *n = lev2 ? lev2 : lev1;
                if (n != NULL)
                  {
                    n->cofs = cofs;
                    n->clen = tmpofs - cofs;
                  }
              }
            if (!any_text)
              {
                /* if (not a blank line) */
                tofs = tmpofs;
                any_text = TRUE;
              }
          }
        tmpofs = ftell(fd);
      }

    if (any_text)
      {
        ErrMsgNode *n = lev2 ? lev2 : lev1;
        if (n != NULL)
          {
            n->tofs = tofs;
            n->tlen = tmpofs - tofs;
          }
      }

    FileClose(fd);
  }}

  idx->busy = 0;
  return TRUE;
}


/*-------------------------------------------------------------------------
 * new_ErrMsgNode, delete_ErrMsgNode
 * Constructor/Destructor for ErrMsgNode
 */

static ErrMsgNode * new_ErrMsgNode(const char *name, int code, ErrSev sev)
{
  ErrMsgNode *item = (ErrMsgNode*)MemNew(sizeof(ErrMsgNode));
  if ( item ) {
    item->name = (name) ? StrSave(name) : 0;
    item->code = code;
    item->sev  = sev;
  }
  return item;
}

static void delete_ErrMsgNode(ErrMsgNode *item)
{
  ErrMsgNode *next;

  while ( item ) {
    next = item->next;

    delete_ErrMsgNode(item->list);
    MemFree((void *)item->name);
    MemFree((void *)item->tstr);
    MemFree((void *)item->cstr);
    MemSet(item, '\xFF', sizeof(*item));
    MemFree(item);

    item = next;
  }
}


/*-------------------------------------------------------------------------
 * ErrGetMsgRoot    [Schuler, 12-09-93]
 *
 * Gets the index structure for an error context, creating and initializing
 * it if necessary.
 *
 * MODIFICATIONS
 * 01-10-94 Schuler   Changed to call ErrMsgRoot_Read() *after* linking
 *                    into list.  Otherwise, if an error is posted from
 *                    within ErrMsgRoot_Read(), it won't find the CoreLib
 *                    modlue and will read it again (ad infinitum).
 */

NLM_EXTERN ErrMsgRootPtr LIBCALL ErrGetMsgRoot (const char *context)
{
  AppErrInfoPtr info = GetAppErrInfo();
  ErrMsgRoot *idx, *idx0=NULL;
  int d;

  for (idx=info->idxlist; TRUE; idx=idx->next)
    {
      if (idx==NULL || (d = strcmp(idx->name,context)) > 0)
        {
          ErrMsgRoot *idx2 = new_ErrMsgRoot(context);
          idx2->next = idx;
          if (idx0 == NULL)
            info->idxlist = idx2;
          else
            idx0->next = idx2;
          ErrMsgRoot_Read(idx2);
          return (ErrMsgRootPtr) idx2;
        }

      if (d == 0)
        break;

      idx0 = idx;
    }

  return (ErrMsgRootPtr)idx;
}


/*-------------------------------------------------------------------------
 * ErrGetExplanation    [Schuler, 12-09-93]
 *
 * Gets the explanatory text for a particular error from an error
 * message file.
 *
 * MODIFICATIONS
 * 02-02-94 Schuler   Fix for memory leak when length==0
 * 02-02-94 Schuler   Now exported (was static)
 */

NLM_EXTERN const char* LIBCALL ErrGetExplanation (ErrMsgRootPtr idx, ErrMsgNodePtr item)
{
	if (idx != NULL && item != NULL && item->tlen >0)
	{
		size_t bytes;
		char *text;

		if (item->tstr != NULL)
			return item->tstr;

		bytes = (size_t)item->tlen;
		if ((text=(char *)MemNew(bytes+1)) != NULL)
		{
			FILE *fd = ErrMsgRoot_fopen((ErrMsgRoot *) idx);
			if (fd != NULL)
			{
				if (fseek(fd,item->tofs,SEEK_SET) !=0)
					goto ErrReturn;
				if (fread(text,1,bytes,fd) != bytes)
					goto ErrReturn;
				fclose(fd);
				*(text+bytes) = '\0';    /* make sure null-terminated */
				((ErrMsgNode*)item)->tstr = text;
				return text;
			}
		ErrReturn :
			fclose(fd);
			MemFree(text);
			return NULL;
		}
	}
	return NULL;
}


/***************************************************************************\
|                             CUSTOMIZATION                                 |
\***************************************************************************/

/*-------------------------------------------------------------------------
* ErrSetLogfile  [Schuler]
*
* MODIFICATIONS
* 07-26-93 Schuler   Modified to use AppErrInfo struct
* 12-17-93 Schuler   Changed back to returning a Boolean
* 01-21-94 Schuler   Renamed & added flags argument.
* 01-21-94 Schuler   Changed fopen/fclose to FileOpen/FileClose
* 04-19-94 Schuler   Only open
*
* TO DO
* - honor ELOG_NOCREATE flag
*/

NLM_EXTERN int LIBCALL  Nlm_ErrSetLogfile (const char *filename, unsigned long flags)
{
  static char *fmode_append = "a+";
  static char *fmode_overwrite = "w";

  AppErrInfoPtr info = GetAppErrInfo();

  if (info->fdLog != NULL)  /* close if already open */
    {
      FileClose(info->fdLog);
      info->fdLog = NULL;
    }

  if (filename == NULL)
    info->logfile[0] = '\0';
  else
    {
      if ( !(flags & ELOG_NOCREATE) )
        {
          char *fmode = (flags & ELOG_APPEND) ? fmode_append : fmode_overwrite;
          FILE *fp;

          if ((fp = FileOpen(filename,fmode)) == NULL)
            return FALSE;
          if (flags & ELOG_BANNER)
            {
              char buffer[64];
              Nlm_DayTimeStr(buffer,TRUE,TRUE);
              fprintf(fp, "\n========================[ %s ]========================\n", buffer);
            }
          FileClose(fp);
        }
      strncpy(info->logfile,filename,PATH_MAX - 1);
      info->logfile[PATH_MAX - 1] = '\0';
    }
  return TRUE;
}


const char* LIBCALL Nlm_ErrGetLogfile( void )
{
  AppErrInfoPtr info = GetAppErrInfo();

  if (info->logfile != NULL  &&  *info->logfile == '\0')
    return NULL;

  return info->logfile;
}


/*-------------------------------------------------------------------------
 * ErrSetHandler	[Schuler, 05-11-93]
 *
 * Allows the application to set a hook procedure that will be called
 * when an error is posted via ErrPost.  It is always called regardless
 * of any error reporting/logging options that may have been set.  The
 * return value is the pointer to the previous error hook procedure
 * if there was one.
 *
 * MODIFICATIONS
 * 05-21-93 Schuler
 */
NLM_EXTERN ErrHookProc LIBCALL Nlm_ErrSetHandler (ErrHookProc hookNew)
{
	AppErrInfoPtr info = GetAppErrInfo();
	ErrHookProc hookOld =info->hook;
	info->hook = hookNew;
	return hookOld;
}


NLM_EXTERN Nlm_Boolean LIBCALL Nlm_CallErrHandlerOnly(Nlm_Boolean hook_only)
{
    Nlm_Boolean prev;
    NlmMutexLockEx(&corelibMutex);
    prev       = s_HookOnly;
    s_HookOnly = hook_only;
    NlmMutexUnlock(corelibMutex);
    return prev;
}


/*-------------------------------------------------------------------------
 * ErrUserInstall   [Sirotkin]
 *
 * Add or replace a user-string.
 *
 * MODIFICATIONS
 * 12-15-93 Schuler   No longer keeps track of string lengths.
 */

static char *_strNull = "(null)";

NLM_EXTERN ErrStrId LIBCALL Nlm_ErrUserInstall (const char *msg, ErrStrId magic_cookie)
{
	AppErrInfoPtr info = GetAppErrInfo();
	ValNode *list = (ValNode *) info->desc.userstr;
	ValNode *node;
	ErrStrId cookie;

	if (msg == NULL)
		msg = _strNull;

	if (magic_cookie)
	{
		for (node=list; node; node=node->next)
		{
			if (node->choice == magic_cookie)
			{
				/** replace **/
				info->ustrlen -= strlen((char*)node->data.ptrvalue);
				MemFree(node->data.ptrvalue);
				node->data.ptrvalue = (void*) StringSave(msg);
				info->ustrlen += strlen(msg);
				return magic_cookie;
			}
		}
		ErrPostEx(SEV_WARNING,-1,0,
				"ErrUserInstall:  bad string id (%d)",
				(int) magic_cookie);
		return 0;
	}
	else
	{
		for (cookie = 1; cookie < 255; ++cookie)
		{
			for (node=list; node; node=node->next)
			{
				if (node->choice == cookie)
					break;
			}
			if (node==NULL)  /* free cookie is magic */
			{
				node = ValNodeNew((ValNode*)info->desc.userstr);
				if ( ! info->desc.userstr)
					info->desc.userstr = node;
				node->choice = cookie;
				node->data.ptrvalue = StringSave(msg);
				info->ustrlen += strlen(msg);
				info->ustrcnt ++;
				return cookie;
			}
		}
		/*ErrPostEx(SEV_WARNING,-1,0,"ErrUserInstall:  no more string id's");*/
		return 0;
	}
}


/*-------------------------------------------------------------------------
 * ErrUserDelete   [Sirotkin]
 *
 * Delete a single user-string.
 *
 * MODIFICATIONS
 * 12-15-93 Schuler   No longer keeps track of string lengths.
 */

NLM_EXTERN Nlm_Boolean LIBCALL Nlm_ErrUserDelete (ErrStrId magic_cookie)
{
	AppErrInfoPtr info = GetAppErrInfo();
	ValNodePtr node = ValNodeExtract((ValNode**)(&info->desc.userstr), magic_cookie);

	if (node)
	{
		ASSERT(node->data.ptrvalue != NULL);
		info->ustrlen -= strlen((char*)node->data.ptrvalue);
		info->ustrcnt --;
		ValNodeFreeData(node);
		return TRUE;
	}
	return FALSE;
}


/*-------------------------------------------------------------------------
 * ErrUserClear   [Sirotkin]
 *
 * Deletes the entire list of user-strings.
 */
NLM_EXTERN void LIBCALL Nlm_ErrUserClear (void)
{
	AppErrInfoPtr info = GetAppErrInfo();
	ValNodeFreeData((ValNode*)info->desc.userstr);
	info->ustrlen = 0;
	info->ustrcnt = 0;
}

/**************************************************************************
*
*  ErrGetUserStrings(ErrDescPtr err)
*    returns ValNodePtr to chain of user strings for this thread
*
***************************************************************************/
NLM_EXTERN ValNodePtr LIBCALL Nlm_ErrGetUserStrings(ErrDescPtr err)
{
	ValNodePtr vnp = NULL;

	if (err != NULL)
		vnp = (ValNodePtr)(err->userstr);
	return vnp;
}



/*-------------------------------------------------------------------------
 * ErrGetLogLevel, ErrSetLogLevel   [Schuler, 12-22-93]
 *
 * Get/Set the minimum severity level that will be logged.
 */
NLM_EXTERN ErrSev LIBCALL ErrSetLogLevel (ErrSev level)
{
	AppErrInfoPtr info = GetAppErrInfo();
	ErrSev prev = (ErrSev) info->opts.log_level;
	info->opts.log_level = (short) MAX(SEV_INFO,MIN(level,SEV_MAX));
	return prev;
}

NLM_EXTERN ErrSev LIBCALL ErrGetLogLevel (void)
{
	return (ErrSev)GetAppErrInfo()->opts.log_level;
}


/*-------------------------------------------------------------------------
 * ErrGetMessageLevel, ErrSetMessageLevel   [Schuler, 12-14-93]
 *
 * Get/Set the minimum severity level that will be reported via Message().
 */
NLM_EXTERN ErrSev LIBCALL ErrSetMessageLevel (ErrSev level)
{
	AppErrInfoPtr info = GetAppErrInfo();
	ErrSev prev = (ErrSev) info->opts.msg_level;
	info->opts.msg_level = (short) MAX(SEV_INFO,MIN(level,SEV_MAX));
	return prev;
}

NLM_EXTERN ErrSev LIBCALL ErrGetMessageLevel (void)
{
	return (ErrSev) GetAppErrInfo()->opts.msg_level;
}


/*-------------------------------------------------------------------------
 * ErrGetFatalLevel, ErrSetFatalLevel   [Schuler, 12-14-93]
 *
 * Get/Set the minimum severity level that will cause an abnormal exit.
 */
NLM_EXTERN ErrSev LIBCALL ErrSetFatalLevel (ErrSev level)
{
	AppErrInfoPtr info = GetAppErrInfo();
	ErrSev prev = (ErrSev) info->opts.die_level;
	info->opts.die_level = (short) MAX(SEV_INFO,MIN(level,SEV_MAX));
	return prev;
}

NLM_EXTERN ErrSev LIBCALL ErrGetFatalLevel (void)
{
	return (ErrSev) GetAppErrInfo()->opts.die_level;
}


/*-------------------------------------------------------------------------
 * ErrSetOptFlags, ErrClearOptFlags, ErrTestOptFlags   [Schuler, 12-14-93]
 *
 * Set, clear, or test the current state of any error option flag.
 */
NLM_EXTERN unsigned long LIBCALL ErrSetOptFlags (unsigned long flags)
{
	AppErrInfoPtr info = GetAppErrInfo();
	info->opts.flags |= (flags & EO_ALL_FLAGS);
	return info->opts.flags;
}

NLM_EXTERN unsigned long LIBCALL ErrClearOptFlags (unsigned long flags)
{
	AppErrInfoPtr info = GetAppErrInfo();
	info->opts.flags &= ~(flags & EO_ALL_FLAGS);
	return info->opts.flags;
}

NLM_EXTERN unsigned long LIBCALL ErrTestOptFlags (unsigned long flags)
{
	AppErrInfoPtr info = GetAppErrInfo();
	return (info->opts.flags & flags);
}


/*-------------------------------------------------------------------------
 * ErrSaveOptions  [Schuler, 01-03-94]
 *
 */
NLM_EXTERN void LIBCALL ErrSaveOptions (ErrOpts *opts)
{
	AppErrInfoPtr info = GetAppErrInfo();

	ASSERT(opts != NULL);
	memcpy((void*)opts,(void*)(&info->opts),sizeof(ErrOpts));
}


/*-------------------------------------------------------------------------
 * ErrRestoreOptions  [Schuler, 01-03-94]
 */

NLM_EXTERN void LIBCALL ErrRestoreOptions (const ErrOpts *opts)
{
	AppErrInfoPtr info = GetAppErrInfo();

	ASSERT(opts != NULL);
	memcpy((void*)(&info->opts),(void*)opts,sizeof(ErrOpts));
}


/*-------------------------------------------------------------------------
* ErrGetOpts  [Schuler]
*
* MODIFICATIONS
* 07-26-93 Schuler   Modified to use AppErrInfo struct
* 02-02-94 Schuler   Added local option saving strategy
*/

NLM_EXTERN void LIBCALL  Nlm_ErrGetOpts (short * erract, short * errlog)
{
	AppErrInfoPtr info = GetAppErrInfo();

	if (erract != NULL)
	{
		size_t i;
		for (i=0; i<DIM(info->_eo_save); ++i)
		{
			if (info->_eo_save[i].flags ==0)
				break;
		}
		if (i<DIM(info->_eo_save))
		{
			info->_eo_save[i] = info->opts;
			*erract = (short)(-1-i);
		}
		else
		{
			TRACE("ErrGetOpts: overflow\n");
			*erract = info->opts.actopt;
		}
	}
	if (errlog != NULL)
		*errlog = info->opts.logopt;
}


/*-------------------------------------------------------------------------
* ErrSetOpts  [Schuler]
*
* MODIFICATIONS
* 07-26-93 Schuler   Modified to use AppErrInfo struct
* 12-15-93 Schuler   Modified to map old settings to the new options.
* 12-21-93 Schuler   Now always starts from defaults and then twiddles.
* 01-27-94 Schuler   Fixed bug whereby logopt not always saved.
* 02-02-94 Schuler   Added local option saving strategy
*/


NLM_EXTERN void LIBCALL  Nlm_ErrSetOpts (short actopt, short logopt)
{
	AppErrInfoPtr info = GetAppErrInfo();

	if (actopt < 0)
	{
		int i = -(actopt+1);
		ASSERT(i>=0 && i<DIM(info->_eo_save));
		if (info->_eo_save[i].flags !=0)
		{
			info->opts = info->_eo_save[i];
			info->_eo_save[i].flags =0;
			return;
		}
		ErrLogPrintf("*** Improper use of ErrGetOpts/ErrSetOpts ***\n");
	}

	/****info->opts.flags = EO_DEFAULTS;
	info->opts.log_level = SEV_INFO;****/

	if (actopt !=0)
		info->opts.flags &= ~EO_PROMPT_ABORT;

	switch (actopt)
	{
		case ERR_CONTINUE:
		case ERR_IGNORE:
			info->opts.msg_level = SEV_MAX;
			info->opts.die_level = SEV_MAX;
			break;
		case ERR_TEE:
			info->opts.msg_level = SEV_WARNING;
			info->opts.die_level = SEV_MAX;
			break;
		case ERR_ADVISE:
			info->opts.msg_level = SEV_WARNING;
			info->opts.die_level = SEV_MAX;
			break;
		case ERR_ABORT:
			info->opts.msg_level = SEV_WARNING;
			info->opts.die_level = SEV_FATAL;
			break;
		case ERR_PROMPT:
			info->opts.msg_level = SEV_WARNING;
			info->opts.die_level = SEV_FATAL;
			info->opts.flags |= EO_PROMPT_ABORT;
			break;
	}
	if (actopt != 0)
		info->opts.actopt = actopt;


	switch(logopt)
	{
		case ERR_LOG_ON:
			info->opts.flags |=  EO_LOGTO_USRFILE;
			break;
		case ERR_LOG_OFF:
			info->opts.flags &= ~EO_LOGTO_USRFILE;
			break;
		default:
			logopt = 0;
	}
	if (logopt != 0)
		info->opts.logopt = logopt;


	/***** if anybody liked this, I can put it back...
	if (logopt != 0)
	{
		int   i;
		FILE *fp;
		if ((fp = FileOpen(info->logfile, "a+")) != NULL)
		{
			fputc ('\n', fp);
			for (i=0; i<4; i++)  fputc (' ', fp);
			for (i=0; i<21; i++)  fputc ('-', fp);
			fprintf (fp, " error logging %sabled ",
			                logopt==ERR_LOG_ON ? " en" : "dis");
			for (i=0; i<21; i++)  fputc ('-', fp);
			fputc ('\n', fp);
			FileClose(fp);
		}
	}
	*****/
}


/***************************************************************************\
|                               DEBUGGING                                   |
\***************************************************************************/


/*-------------------------------------------------------------------------
 * Trace   [Schuler, 04-13-93]
 *
 * Formats a string and sends it to the "trace device" (see the
 * description of Nlm_TraceStr about "trace device").  Normally, it
 * is desirable to trace some (possibly verbose) informational messages
 * during the development phase of a program, but inhibit them in the
 * final version that is released to end users.  The TRACE macro supports
 * this style of usage, calling Nlm_Trace only if _DEBUG is defined.
 * For example:
 *
 *		TRACE("!@#$ screwed up (%s,%d)\n",__FILE__,___LINE__);
 *
 * Note that it is declared as CDECL (because of the variable argument
 * list) and may therefore not be callable from some other programming
 * languages, such as Basic and Pascal.
 *
 * MODIFICATIONS
 * Schuler 07-26-93
 * Schuler 12-14-93   Merged varargs and stdargs versions of the function
 */

#ifdef VAR_ARGS
NLM_EXTERN void Nlm_Trace (fmt, va_alist)
 const char *fmt;
 va_dcl
#else
NLM_EXTERN void CDECL  Nlm_Trace (const char *fmt, ...)
#endif
{
  const Nlm_Char PNTR scratch_str = NULL;
  TSPRINTF(scratch_str, fmt);
  if (scratch_str != NULL)
    Nlm_TraceStr( scratch_str );
}



/*-------------------------------------------------------------------------
* TraceStr   [Schuler, 04-13-93]
*
* Users of the C and C++ should use the TRACE macro and not call this
* function directly.  This function, unlike Nlm_Trace, it may be called
* other programming languages, such as Basic and Pascal.
*
* MODIFICATIONS
* 05-26-93 Schuler
* 12-15-93 Schuler   Changed to use TRACE_TO_FILE macro.
* 12-15-93 Schuler   Removed tracing to stderr (caused an echo echo ...).
* 01-21-94 Schuler   Changed fopen/fclose to FileOpen/FileClose
*/

NLM_EXTERN void LIBCALL Nlm_TraceStr (const char *str)
{
	if (str==NULL)  return;

#ifdef TRACE_TO_STDOUT
	fprintf(stdout,"%s",str);
#endif

#ifdef TRACE_TO_AUX
#ifdef OS_MSWIN
	OutputDebugString(str);
#endif
#endif /* TRACE_TO_AUX */

#ifdef TRACE_TO_FILE
	{
		FILE *fd = FileOpen("trace.log","a+");
		if (fd != NULL)
		{
			fprintf(fd,"%s",str);
			FileClose(fd);
		}
	}
#endif /* TRACE_TO_FILE */
}


/*-------------------------------------------------------------------------
* Nlm_AssertionFailed   [Schuler, 04-13-93]
*
* Function needed to support the ASSERT and VERIFY macros.
*
* MODIFICATIONS
* 05-11-93 Schuler
* 12-15-93 Schuler   Use ErrLogPrintf() inseead of Nlm_Trace()
* 12-15-93 Schuler   Added call to Message() so user knows what happened!
* 01-21-94 Schuler   Changed MSG_OK to MSG_POST
*/
NLM_EXTERN void LIBCALL Nlm_AssertionFailed
(const char *expression, const char *module,
 const char *fname, int linenum)
{
	ErrLogPrintf("\nAssertion Failed:  %s\n", expression);
	if (module  &&  *module)
     ErrLogPrintf("  %s",module);
	if ( fname )
     ErrLogPrintf("  %s, line %d", fname, linenum);
	ErrLogPrintStr("\n");

	Message(MSG_POST, "Assertion Failed:\n%s", expression);
	if (module  &&  *module)
     Message(MSG_POST, "Module \"%s\"", module);
	if ( fname )
	  Message(MSG_POST, "File \"%s\", line %d", fname, linenum);

	AbnormalExit(1);
}


/*-------------------------------------------------------------------------
 * AbnormalExit   [Schuler, 04-13-93]
 *
 * Terminates the application immediately.  This should only be done
 * as a last resort since some (possibly necessary) cleanup code will
 * not get executed.
 *
 * MODIFICATIONS
 * Schuler  05-11-93
 * Schuler  07-26-93
 */
NLM_EXTERN void LIBCALL Nlm_AbnormalExitPure(int code)
{
#if defined(_DEBUG)
  abort();
#elif defined(OS_MAC)
  ExitToShell();
#else
  exit( code );
#endif
}


NLM_EXTERN void LIBCALL  Nlm_AbnormalExit (int code)
{
  static Nlm_Boolean s_Exiting = FALSE;
  if ( !s_Exiting ) {
    s_Exiting = TRUE;
    ErrLogPrintStr( "\n***** ABNORMAL PROGRAM EXIT *****\n" );
  }
  Nlm_AbnormalExitPure( code );
}


/***************************************************************************\
|                         STATIC HELPER FUNCTIONS                           |
\***************************************************************************/

/*-------------------------------------------------------------------------
* GetAppErrInfo  [Schuler, 07-26-93]
*
* MODIFICATIONS
* 12-13-93 Schuler   Added initialization for new settings.
* 01-04-94 Schuler   Added code to get settings from ncbi.ini
* 01-27-94 Schuler   Set & clear _busy when reading from ncbi.ini
* 02-01-94 Schuler   Updated _flag array
*/

static char * _file = "ncbi";
static char * _section = "ErrorProcessing";

struct FlagInf { char *key; unsigned long bits; };
static struct FlagInf _flag [] =
{
  { "EO_LOG_SEVERITY",  EO_LOG_SEVERITY  },
  { "EO_LOG_CODES",     EO_LOG_CODES     },
  { "EO_LOG_FILELINE",  EO_LOG_FILELINE  },
  { "EO_LOG_USERSTR",   EO_LOG_USERSTR   },
  { "EO_LOG_ERRTEXT",   EO_LOG_ERRTEXT   },
  { "EO_LOG_MSGTEXT",   EO_LOG_MSGTEXT   },
  { "EO_MSG_SEVERITY",  EO_MSG_SEVERITY  },
  { "EO_MSG_CODES",     EO_MSG_CODES     },
  { "EO_MSG_FILELINE",  EO_MSG_FILELINE  },
  { "EO_MSG_USERSTR",   EO_MSG_USERSTR   },
  { "EO_MSG_ERRTEXT",   EO_MSG_ERRTEXT   },
  { "EO_MSG_MSGTEXT",   EO_MSG_MSGTEXT   },
  { "EO_LOGTO_STDOUT",  EO_LOGTO_STDOUT  },
  { "EO_LOGTO_STDERR",  EO_LOGTO_STDERR  },
  { "EO_LOGTO_TRACE",   EO_LOGTO_TRACE   },
  { "EO_LOGTO_USRFILE", EO_LOGTO_USRFILE },
  { "EO_XLATE_CODES",   EO_XLATE_CODES   },
  { "EO_WAIT_KEY",      EO_WAIT_KEY      },
  { "EO_PROMPT_ABORT",  EO_PROMPT_ABORT  },
  { "EO_BEEP",          EO_BEEP          }
};


static int _nAppErrInfo = 0;

extern void ReleaseAppErrInfo( void )
{
  AppErrInfoPtr info = (AppErrInfoPtr)GetAppProperty( _szPropKey );

  if (info == NULL)
    return;

  NlmMutexLockEx( &corelibMutex );
  ASSERT ( _nAppErrInfo > 0 );

  if (--_nAppErrInfo == 0)
    {
      int i;
      for (i=SEV_MIN; i<=SEV_MAX; ++i)
        {
          Nlm_MemFree( _szSeverity[i] );
          _szSeverity[i] = NULL;
        }
    }

  if (info->fdLog != NULL) {
    FileClose( info->fdLog );
  }

  while ( info->idxlist ) {
    ErrMsgRoot *idx = info->idxlist;
    info->idxlist = idx->next;
    delete_ErrMsgRoot(idx);
  }

  Nlm_ErrUserClear();

  Nlm_MemFill(info, '\xFF', sizeof(*info));
  Nlm_MemFree( info );

  RemoveAppProperty( _szPropKey );

  NlmMutexUnlock( corelibMutex );
}


static struct AppErrInfo * GetAppErrInfo(void)
{
  AppErrInfoPtr info = (AppErrInfoPtr) GetAppProperty(_szPropKey);

  if (info == NULL)
    {
      char buffer[128], *p;
      int i;

      int severity_init = 0;
      NlmMutexLockEx( &corelibMutex );

      info = (struct AppErrInfo*) MemGet(sizeof(struct AppErrInfo), TRUE);
      if (info == NULL)
        Message(MSG_FATAL,"Out of memory");
      info->opts.actopt = ERR_ABORT;       /* OBSOLETE */
      info->opts.logopt = ERR_LOG_OFF;     /* OBSOLETE */
      info->opts.flags = EO_DEFAULTS;
      info->opts.log_level = SEV_INFO;
#ifdef _DEBUG
      info->opts.msg_level = SEV_WARNING;
#else
      info->opts.msg_level = SEV_ERROR;
#endif
      info->opts.die_level = SEV_FATAL;
      strcpy(info->logfile,"error.log");
      for (i = SEV_MIN;  i <= SEV_MAX;  i++)
        {
          if (_szSeverity[i] == NULL)
            {
              severity_init = 1;
              sprintf(buffer, "*SEV*%d", i);
              _szSeverity[i] = StrSave( buffer );
            }
        }
      ++_nAppErrInfo;
      SetAppProperty(_szPropKey, (void*)info);

      if (GetAppParam(_file, _section, "MsgPath", "",
                      info->msgpath, PATH_MAX-2))
        FileBuildPath(info->msgpath, NULL, NULL);

      if ( severity_init )
        {
          for (i = SEV_MIN;  i <= SEV_MAX;  i++)
            {
              GetAppParam(_file, _section, _szSevKey[i], _szSevDef[i],
                          buffer,sizeof buffer);
              if (buffer[0] == '"')
                {
                  if ((p = strchr(&buffer[1],'"')) != NULL)  *p = '\0';
                  p = &buffer[1];
                }
              else
                p = buffer;

              {{
                char *dummy_sev = _szSeverity[i];
                _szSeverity[i] = StrSave(p);
                Nlm_MemFree( dummy_sev );
              }}
            }
        }
      NlmMutexUnlock( corelibMutex );

      for (i=0; i<DIM(_flag); ++i)
        {
          p = _flag[i].key;
          if (GetAppParam(_file,_section,p,"",buffer,sizeof buffer))
            {
              info->ini_mask |= _flag[i].bits;
              if (strchr("1TtYy",buffer[0]))
                info->ini_bits |= _flag[i].bits;
            }
        }

      SetAppProperty(_szPropKey,(void*)info);
    }

  return info;
}

/*-------------------------------------------------------------------------
 * ErrPathReset   [Kans]
 *
 * Resets info->msgpath
 */
NLM_EXTERN void LIBCALL Nlm_ErrPathReset (void)
{
	AppErrInfoPtr info = GetAppErrInfo();
    if (GetAppParam(_file, _section, "MsgPath", "",
                    info->msgpath, PATH_MAX-2))
      FileBuildPath(info->msgpath, NULL, NULL);
}

NLM_EXTERN char *Nlm_GetErrLongText (char *module, 
                                     int errcode, int subcode)
{
    ErrMsgRoot *root  = NULL;
    ErrMsgNode *node1 = NULL;
    ErrMsgNode *node2 = NULL;
    ErrMsgNode *the_node = NULL;
    char *text = NULL;
    
    if (module != NULL) {
        root = (ErrMsgRoot*) ErrGetMsgRoot(module);
        for (node1=root->list; node1; node1=node1->next) {
            if (node1->code == errcode) {
                for (node2=node1->list; node2; node2=node2->next)
                    if (node2->code == subcode)
                        break;
                break;
            }
        }
    }
    
    the_node = node2 ? node2 : node1;
    
    text = (char *) ErrGetExplanation(root, the_node);
    
    return text;
}

