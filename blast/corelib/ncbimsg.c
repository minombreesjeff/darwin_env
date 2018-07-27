/*   ncbimsg.c
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
* File Name:  ncbimsg.c
*
* Author:  Gish, Kans, Ostell, Schuler
*
* Version Creation Date:   2/13/91
*
* $Revision: 6.9 $
*
* File Description:
*   	user alert and error messages
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 2/13/91  Kans        Now only used for non-Vibrant versions
* 09-19-91 Schuler     Added LIBCALL to Nlm_Beep()
* 09-20-91 Schuler     Include <conio.h> for getche() prototype
* 04-15-93 Schuler     Changed _cdecl to LIBCALL
* 05-28-93 Schuler     New function: SetMonitorHook()
* 06-06-93 Schuler     Added code to set/check magic_value for Monitors
* 01-13-94 Schuler     Converted ErrPost to ErrPostEx
* 01-13-94 Schuler     Added SetMsgHook and SetBeepHook
* 01-31-94 Schuler     Modified Message and _DefMessageHook (see below)
* 02-07-94 Schuler     Added element to _sev_code[] array for SEV_NONE
* 06-29-94 Schuler     Moved VSPRINTF macros to corepriv.h
* 06-07-95 Epstein     Change UNIX's GetOneChar() to burn fewer CPU cycles
* 07-19-95 Epstein     Bug fix to previous GetOneChar() so that user's
*                      input will be read properly.
*
* $Log: ncbimsg.c,v $
* Revision 6.9  2000/08/28 18:46:24  vakatov
* Made internal hooks "static" and "C" -- to pass C++ compilation
*
* Revision 6.8  1999/10/01 14:41:40  kans
* added SEV_REJECT between SEV_ERROR and SEV_FATAL
*
* Revision 6.7  1999/08/31 21:36:25  vakatov
* Nlm_Message(), Nlm_MsgAlert() -- pre-save the temporary "scratch_str"
* buffer to avoid its overriding on calling the "ncbierr.c"-related code
*
* Revision 6.6  1999/08/23 19:16:08  vakatov
* Nlm_Message():  handle MSG_FATAL as SEV_MAX (was SEV_FATAL)
*
* Revision 6.5  1998/08/24 17:42:01  kans
* fixed old style function definition warnings
*
* Revision 6.4  1998/06/11 21:02:29  shavirin
* Fixed few warnings
*
* Revision 6.3  1997/11/26 21:26:23  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.2  1997/10/29 02:44:04  vakatov
* Type castings to pass through the C++ compiler
*
* Revision 6.1  1997/09/10 21:35:30  vakatov
* [OS_MSWIN] _DefMessageHook(): Removed extraneous AbnormalExit()
*
* Revision 6.0  1997/08/25 18:16:51  madden
* Revision changed to 6.0
*
* Revision 5.7  1997/07/22 19:11:39  vakatov
* Separated Main() from GetArg[svc]() functions;  [WIN_MSWIN] converged
* console and GUI libraries; [for WIN32-DLL] encapsulated global variables
*
* Revision 5.6  1997/07/15 16:57:08  vakatov
* Nlm_Message() -- assign "ans" to ANS_NONE if no message(0 or "\0") posted
*
* Revision 5.5  1997/01/27 18:10:24  shavirin
* Removed bug in function GetOneChar()
*
 * Revision 5.4  1996/12/03  21:48:33  vakatov
 * Adopted for 32-bit MS-Windows DLLs
 *
 * Revision 5.3  1996/11/25  19:04:58  vakatov
 * Made 'sProgramName' WIN_MSWIN-only
 *
 * Revision 5.2  1996/07/16  19:57:02  vakatov
 * VSPRINTF macros has been replaced by the TSPRINTF macros in the
 * code responsible for the error message composing -- in order to
 * avoid memory limits overrun when printing the messages (by
 * the means of function "vsprintf()") to a memory-located string.
 * The application name is added as the message caption.
 * Added ReleaseAppMsgInfo() function to clean dynamically allocated
 * memory used by the message posing routines;  +some casting
 *
 * Revision 5.1  1996/07/05  16:26:10  kans
 * changed MonitorXXNew functions to MonitorXXXNewEx, with new hasCancelBtn
 * parameter, defined old functions to call new ones with hasCancelBtn TRUE.
 *
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.30  1995/07/19  17:48:06  epstein
 * bug fix to GetOneChar() so that a non-null input string will be read correctly
 *
 * Revision 2.29  1995/07/05  14:22:37  kans
 * move #include <sys/select.h> to ncbilcl.r6k
 *
 * Revision 2.28  1995/06/07  15:13:22  epstein
 * re-add Kans's changes to permit cancelling of Monitors (changes were erroneously removed by Epstein)
 *
 * Revision 2.26  95/06/05  18:28:32  kans
 * *** empty log message ***
 * 
 * Revision 2.25  1995/05/31  20:34:06  ostell
 * made the WIN_DUMB default MsgAlertStr case insensitive for responses
 *
 * Revision 2.24  1995/05/15  18:45:58  ostell
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

#ifdef __cplusplus
extern "C" {
#endif

    static MsgAnswer PASCAL _DefMessageHook
    (MsgKey key, ErrSev sev, const char *caption, const char *message);

    static void PASCAL _DefBeepHook(void);

    static int PASCAL _DefMonitorHook(MonitorPtr pMon, MonCode code);

#ifdef __cplusplus
} /* extern "C" */
#endif



/*************************************************************************\
|                    INSTANCE-SPECIFIC INFO STRUCTURE                     |
\*************************************************************************/

typedef struct AppMsgInfo
{
	MessageHook hookMessage;
	BeepHook hookBeep;
	MonitorHook hookMonitor;
}
AppMsgInfo;

static char * _szPropKey = "_AppMsgInfo";


extern void ReleaseAppMsgInfo( void )
{
  AppMsgInfo *info = (AppMsgInfo *)GetAppProperty( _szPropKey );

  if (info == NULL)
    return;

  Nlm_MemFree( info );
  RemoveAppProperty( _szPropKey );
}


static AppMsgInfo * GetAppMsgInfo (void)

{
	AppMsgInfo *info = (AppMsgInfo*) GetAppProperty(_szPropKey);
	
	if (info == NULL)
	{
		info = (AppMsgInfo*) MemGet(sizeof(struct AppMsgInfo), TRUE);
		if (info == NULL)  AbnormalExit(1);  
			
		info->hookMessage = _DefMessageHook;
		info->hookBeep = _DefBeepHook;
		info->hookMonitor = _DefMonitorHook;
		
		SetAppProperty(_szPropKey,(void*)info);
	}
	return info;
}


/*************************************************************************\
|                                ALERTS                                   |
\*************************************************************************/
#if defined(WIN_DUMB) || defined(OS_MAC)
static int GetOneChar PROTO((void));
#endif


/*-------------------------------------------------------------------------
* Message
*
*   Nlm_Message(key, format, ...)
*   	key = type of message
*   	MSG_ERROR 0 = non-fatal error
*   	MSG_FATAL 1 = fatal error
*   	MSG_OK    2 = OK
*   	MSG_RC    3 = Retry Cancel
*   	MSG_ARI   4 = Abort Retry Ignore
*   	MSG_YN    5 = Yes/No
*   	MSG_YNC   6 = Yes/No/Cancel
*       MSG_OKC   7 = OK/Cancel
*   	MSG_POST  8 = show message, no response required
*   	MSG_POSTERR 9 = beep, show message, no response required
*
*   reply is:
*   	0 = no
*   	1 = yes or ok or retry
*   	abort/cancel
*   	3 = ignore
*
*       #define ANS_NO 0
*       #define ANS_YES 1
*       #define ANS_OK 1
*       #define ANS_RETRY 1
*       #define ANS_ABORT 2
*       #define ANS_CANCEL 2
*       #define ANS_IGNORE 3
*
*
* MODIFICATIONS
* 01-31-94 Schuler   Added calls to Beep() and AbnormalExit() where
*                    appropriate instead of expecting the MessageHook
*                    to do this.
*/


#ifdef VAR_ARGS
NLM_EXTERN MsgAnswer CDECL Nlm_Message (sevkey, fmt, va_alist)
Nlm_Int2 sevkey;
const char *fmt;
va_dcl
#else
NLM_EXTERN MsgAnswer CDECL Nlm_Message (Nlm_Int2 sevkey, const char *fmt, ...)
#endif
{
  char *caption = (char*) GetAppProperty("ProgramName");
  MsgKey key = KEY_OK;
  ErrSev sev = SEV_INFO;
  MsgAnswer ans;

  if (sevkey < KEY_other)
    {
      key = (MsgKey) sevkey;
    }
  else 
    {
      switch (sevkey)
        {   
        case MSG_ERROR :
          Nlm_Beep();
          sev = SEV_ERROR;
          break;
        case MSG_FATAL :
          Nlm_Beep();
          sev = SEV_MAX;
          break;
        case MSG_POSTERR :
          Nlm_Beep();
          sev = SEV_ERROR;
          key = KEY_NONE;
          break;
        case MSG_POST :
          key = KEY_NONE;
          break;
        }
    }

  {{
    const Nlm_Char PNTR scratch_str = NULL;
    Nlm_Char PNTR message;
    TSPRINTF(scratch_str, fmt);
    message = Nlm_StringSave(scratch_str);
    ans = message ? MsgAlertStr(key, sev, caption, message) : ANS_NONE;
    Nlm_MemFree(message);
  }}

  if (sevkey == MSG_FATAL)
    AbnormalExit(1);

  return ans;
}


/*-------------------------------------------------------------------------
* MsgAlert  [Schuler, 01-13-94]
*/

#ifdef VAR_ARGS
NLM_EXTERN MsgAnswer CDECL Nlm_MsgAlert (key, sev, caption, fmt, va_alist)
	MsgKey key;
	ErrSev sev;
	const char *caption;
	const char *fmt;
	va_dcl
#else
NLM_EXTERN MsgAnswer CDECL Nlm_MsgAlert (MsgKey key, ErrSev sev, const char *caption, const char *fmt, ...)
#endif
{
  MsgAnswer ans;
  const Nlm_Char PNTR scratch_str = NULL;
  Nlm_Char PNTR message;
  TSPRINTF(scratch_str, fmt);
  message = Nlm_StringSave(scratch_str);
  ans = message ? MsgAlertStr(key, sev, caption, message) : ANS_NONE;
  Nlm_MemFree(message);
  return ans;
}


/*-------------------------------------------------------------------------
* MsgAlertStr  [Schuler, 01-13-94]
*/
NLM_EXTERN MsgAnswer LIBCALL Nlm_MsgAlertStr (MsgKey key, ErrSev sev, 
			const char *caption, const char *message)
{
	MessageHook hook = GetAppMsgInfo()->hookMessage;
	return (*hook)(key,sev,caption,message);
}


/*-------------------------------------------------------------------------
* _DefMessageHook  [Schuler, 01-13-94, from the old Message]
*
* MODIFICATIONS:
* 01-24-94 Schuler   Check for NULL message string
* 01-31-94 Schuler   Removed Beep() and AbnormalExit() calls
*/


MsgAnswer PASCAL _DefMessageHook (MsgKey key, ErrSev sev, 
                                  const char *caption, const char *message)
{
  MsgAnswer answer = ANS_NONE;

#ifdef OS_MSWIN
  if ( !Nlm_HasConsole )
    {
      static UINT _sev_code[SEV_MAX+1] = { 
        /* SEV_NONE */    MB_OK,
        /* SEV_INFO */    MB_ICONINFORMATION,
        /* SEV_WARNING */ MB_ICONASTERISK, /* same as MB_ICONINFORMATION */
        /* SEV_ERROR */   MB_ICONEXCLAMATION,
        /* SEV_REJECT */  MB_ICONEXCLAMATION,
        /* SEV_FATAL */   MB_ICONHAND,
        /* SEV_MAX */     MB_ICONHAND
      };

      UINT flags = MB_TASKMODAL | _sev_code[(int)sev];
      if (key > 0)
        flags |= (key-1);                    
      answer = MessageBox(NULL,message,caption,flags);
      return answer;
    }
#endif

#if defined(WIN_DUMB) || defined(OS_MAC)
  {{
    static char * _key_str [] = { 
      /* KEY_NONE */ "",
      /* KEY_OK   */ "Hit Return  ", 
      /* KEY_OKC  */ "C = Cancel, Anything else = OK  ",
      /* KEY_ARI  */ "A = abort, R = retry, I = ignore  ",
      /* KEY_YNC  */ "Y = yes, N = no, C = cancel  ",
      /* KEY_YN   */ "Y = yes, N = no  ",
      /* KEY_RC   */ "R = retry, C = Cancel  "
    };

    fflush(stdout);
    fprintf(stderr,"[%s] %s\n",
            caption ? caption : "NULL_Caption",
            message ? message : "NULL_Message");
	
    if (key>KEY_NONE && key<KEY_other)
      {
        int ch;
		
        /* show prompt */		
        fprintf(stderr,"%s  ",_key_str[(int)key]);	

        /* set default value */
        switch (key)  
          {
          case KEY_OK:   case KEY_OKC:
            answer = ANS_OK;     break;
          case KEY_ARI:  case KEY_RC:
            answer = ANS_RETRY;  break;
          case KEY_YNC:  case KEY_YN:
            answer = ANS_YES;    break;
          case KEY_NONE: case KEY_other:
            ASSERT_HARD ( FALSE );
          }
	
        /* get response */
        ch = GetOneChar();
        ch = isalpha(ch) ? toupper(ch) : ch;
        switch (ch)
          {
          case 'A' :
            answer = ANS_ABORT;   break;
          case 'C' :
            answer = ANS_CANCEL;  break;
          case 'I' :
            answer = ANS_IGNORE;  break;
          case 'N' :
            answer = ANS_NO;      break;
          }
      }
  }}
#endif /* WIN_DUMB || OS_MAC */

  return answer;
}

/*-------------------------------------------------------------------------
* SetMessageHook  [Schuler, 01-13-94]
*/
NLM_EXTERN MessageHook LIBCALL Nlm_SetMessageHook (MessageHook hook)
{
	AppMsgInfo *info = GetAppMsgInfo();
	MessageHook hookPrev = info->hookMessage;
	if (hookPrev ==_DefMessageHook) hookPrev = NULL;
	info->hookMessage = (hook == NULL) ? _DefMessageHook : hook;
	return hookPrev;
}


#ifndef WIN16
/*-------------------------------------------------------------------------
* GetOneChar
*
* Gets a single character from the console
*
* MODIFICATIONS
* 01-13-94 Schuler   Simplified (old version preserved below)
*/
#if defined(COMP_MSC) || defined(COMP_BOR)
#include <conio.h>		// for getche prototype
#endif
#ifdef OS_UNIX
#include <sys/time.h>
#include <sys/types.h>
#endif /* OS_UNIX */

static int GetOneChar (void)
{
  int value = 0;

#if defined(COMP_MSC) || defined(COMP_BOR)
  value = getche();
  putchar('\n');

#else /* COMP_MSC || COMP_BOR */
  int tvalue;
  do
    {
#ifdef OS_UNIX
      if (value == 0)
        {
          fd_set rfds;

          FD_ZERO(&rfds);
          FD_SET(0 /* standard input */, &rfds);
          /* wait for some data so that we don't overwork the CPU */
          while (select(1, &rfds, NULL, NULL, NULL) < 0)
            {
              sleep(1);
            }
        }
#endif /* OS_UNIX */
      if((tvalue = getchar()) == EOF) {
        return 0;
      }

      if (! value)
        value = tvalue;
    } while (tvalue != '\n' && tvalue != '\r');
#endif /* else COMP_MSC || COMP_BOR */

  return value;
}
#endif /* !WIN16 */


/*************************************************************************\
|                                 BEEPS                                   |
\*************************************************************************/


/*-------------------------------------------------------------------------
* Beep
*
* MODIFICATIONS
* 01-13-94 Schuler   Modified to use BeepHook (old version preserved below)
*/

NLM_EXTERN void LIBCALL  Nlm_Beep (void)
{
	BeepHook hook = GetAppMsgInfo()->hookBeep;
	(*hook)();
}


/*-------------------------------------------------------------------------
* _DefBeepHook  [Schuler, 01-13-94, from old Beep code]
*
* Default beep function
*/
void LIBCALLBACK _DefBeepHook (void)
{
#if   defined(OS_MAC)
  SysBeep( 60 );

#elif defined(OS_MSWIN)
#ifdef WIN32
  Beep(60, 10);
#else
  MessageBeep( 0 );
#endif

#elif defined(OS_UNIX) || defined(OS_VMS)
  putc(7, stderr);
#endif
}

/*-------------------------------------------------------------------------
* SetBeepHook  [Schuler, 01-13-94]
*/
NLM_EXTERN BeepHook LIBCALL Nlm_SetBeepHook (BeepHook hook)
{
	AppMsgInfo *info = GetAppMsgInfo();
	BeepHook hookPrev = info->hookBeep;
	if (hookPrev == (BeepHook)_DefBeepHook) hookPrev = NULL;
	info->hookBeep = (hook == NULL) ? _DefBeepHook : hook;
	return hookPrev;
}

/*************************************************************************\
|                            PROGRESS MONITORS                            |
\*************************************************************************/

#define MON_MAGIC_VALUE		1234


#define MON_SET_MAGIC(x)	(x)->magic_value = MON_MAGIC_VALUE
#define MON_IS_VALID(x)		((x)!=NULL  && (x)->magic_value==MON_MAGIC_VALUE)

/**
#define MON_MAGIC(x)	   *((int*)((char*)(x) + sizeof(Monitor)))
#define MON_SET_MAGIC(x)   MON_MAGIC(x) = MON_MAGIC_VALUE
#define MON_IS_VALID(x)    (MON_MAGIC(x) == MON_MAGIC_VALUE)
***/

/*-------------------------------------------------------------------------
* MonitorIntNew
*
* Creates an integer range monitor
*
* MODIFICATIONS
* 05-27-93 Schuler   Support for applications hooking into monitors
* 06-03-93 Schuler   Use of magic number to detect invalid pointers
*/

NLM_EXTERN MonitorPtr LIBCALL Nlm_MonitorIntNewEx (Nlm_CharPtr title, Nlm_Int4 n1, Nlm_Int4 n2, Nlm_Boolean hasCancelBtn)
{
	AppMsgInfo *info = GetAppMsgInfo();
	Monitor *pMon = (Monitor*) MemNew(sizeof(Monitor));

	if (pMon != NULL)
	{
		MON_SET_MAGIC(pMon);
		pMon->type = MonType_Int;
		pMon->strTitle = title ? StrSave(title) : 0;
		pMon->num1 = n1;
		pMon->num2 = n2;
		pMon->cancel = FALSE;
		pMon->hasCancelBtn = (int) hasCancelBtn;
		if (!(*info->hookMonitor)(pMon,MonCode_Create))
		{
		    MonitorFree(pMon);
		    /* only post an information message here; it is expected
		    	that the hook function would report the real reason
		    	that the monitor creation failed. */
			ErrPostEx(SEV_INFO,0,0,"Unable to create monitor");
			return NULL;
		}
	}
	return pMon;
}


/** TO DO:  define error codes for these conditions **/
static char * _invalid_mon = "invalid monitor pointer";
static char * _invalid_type = "invalid monitor type";

/*-------------------------------------------------------------------------
* MonitorStrNew
*
* Creates a string monitor.  The len argument is the maximum string length.
*
* MODIFICATIONS
* 05-27-93 Schuler   Support for applications hooking into monitors
* 06-03-93 Schuler   Use of magic number to detect invalid pointers
*/

NLM_EXTERN MonitorPtr LIBCALL Nlm_MonitorStrNewEx (Nlm_CharPtr title, Nlm_Int2 len, Nlm_Boolean hasCancelBtn)
{
	AppMsgInfo *info = GetAppMsgInfo();
	Monitor *pMon = (Monitor*) MemNew(sizeof(Monitor));

	if (pMon != NULL)
	{
		MON_SET_MAGIC(pMon);
		pMon->type = MonType_Str;
		pMon->strTitle = title ? StrSave(title) : 0;
		pMon->num1 = MAX(0,MIN(len,72));
		pMon->cancel = FALSE;
		pMon->hasCancelBtn = (int) hasCancelBtn;
		(*info->hookMonitor)(pMon,MonCode_Create);
	}
	return pMon;
}


/*-------------------------------------------------------------------------
* MonitorStrValue
*
* Sets the string value for a string monitor.
*
* MODIFICATIONS
* 05-27-93 Schuler   Support for applications hooking into monitors
* 06-03-93 Schuler   Use of magic number to detect invalid pointers
* 06-03-93 Schuler   Check that monitor is correct type (MonType_Str)
*/

NLM_EXTERN Nlm_Boolean LIBCALL Nlm_MonitorStrValue (MonitorPtr pMon, Nlm_CharPtr sval)
{
	AppMsgInfo *info = GetAppMsgInfo();
	if ( ! MON_IS_VALID(pMon) )
	{
		ErrPostEx(SEV_WARNING,0,0,"MonitorStrValue: %s",_invalid_mon);
		return FALSE;
	}
	if (pMon->type != MonType_Str)
	{
		ErrPostEx(SEV_WARNING,0,0,"MonitorStrValue: %s",_invalid_type);
		return (Nlm_Boolean)(!pMon->cancel);
	}

	if (pMon->strValue) MemFree((void*)pMon->strValue);
	pMon->strValue = sval ? StrSave(sval) : 0;
	(*info->hookMonitor)(pMon,MonCode_StrValue);
	return (Nlm_Boolean)(!pMon->cancel);
}


/*-------------------------------------------------------------------------
* MonitorIntValue
*
* Sets the integer value for an integer range monitor
*
* MODIFICATIONS
* 05-27-93 Schuler   Support for applications hooking into monitors
* 06-03-93 Schuler   Use of magic number to detect invalid pointers
* 06-03-93 Schuler   Check that monitor is correct type (MonType_Int)
*/

NLM_EXTERN Nlm_Boolean LIBCALL Nlm_MonitorIntValue (MonitorPtr pMon, Nlm_Int4 ival)
{
	AppMsgInfo *info = GetAppMsgInfo();
	if ( !MON_IS_VALID(pMon) )
	{
		ErrPostEx(SEV_WARNING,0,0,"MonitorIntValue: %s",_invalid_mon);
		return FALSE;
	}
	if (pMon->type != MonType_Int)
	{
		ErrPostEx(SEV_WARNING,0,0,"MonitorStrValue: %s",_invalid_type);
		return (Nlm_Boolean)(!pMon->cancel);
	}

	pMon->intValue = ival;
	(*info->hookMonitor)(pMon,MonCode_IntValue);
	return (Nlm_Boolean)(!pMon->cancel);
}


/*-------------------------------------------------------------------------
* MonitorFree
*
* MODIFICATIONS
* 05-27-93 Schuler   Support for applications hooking into monitors
* 06-03-93 Schuler   Use of magic number to detect invalid pointers
*/

NLM_EXTERN MonitorPtr LIBCALL Nlm_MonitorFree (MonitorPtr pMon)
{
	AppMsgInfo *info = GetAppMsgInfo();
	if ( ! MON_IS_VALID(pMon) )
	{
		ErrPostEx(SEV_WARNING,0,0,"MonitorFree: %s",_invalid_mon);
	}
	else
	{
		(*info->hookMonitor)(pMon,MonCode_Destroy);
		MemFree((void*)pMon->strTitle);
		MemFree((void*)pMon->strValue);
		MemFree(pMon);
	}
	return NULL;
}


/*-------------------------------------------------------------------------
* SetMonitorHook	[Schuler, 05-27-93]
*
* Allows the application to set a hook procedure that will be called
* when a monitor is created, destroyed, or changes value.
*
* MODIFICATIONS
* 01-13-94 Schuler   No longer allows you to set the hook to NULL.
*/

NLM_EXTERN MonitorHook LIBCALL Nlm_SetMonitorHook (MonitorHook hook)
{
	AppMsgInfo *info = GetAppMsgInfo();
	MonitorHook hookPrev = info->hookMonitor;
	if (hookPrev ==_DefMonitorHook) hookPrev = NULL;
	info->hookMonitor = (hook == NULL) ? _DefMonitorHook : hook;
	return hookPrev;
}



/*-------------------------------------------------------------------------
*	_DefMonitorHook	[Schuler, 05-27-93]
*
*	IMPORTANT:  If your program is NOT a console-style (WIN_DUMB), you
*	should either (1) use Vibrant or (2) write a monitor hook function
*	and install it with SetMonitorHook.  Otherwise, the following
*	"do-nothing" function will be used and you will not see anything.
*/

#ifdef WIN16

int PASCAL _DefMonitorHook (MonitorPtr pMon, MonCode code)
{
	switch (code)
	{
		case MonCode_Create :
			TRACE("[%s]\n",pMon->strTitle);
			return TRUE;
		case MonCode_StrValue :
			TRACE("%s\n",pMon->strValue);
			break;
	}
	return 0;
}

#else  /* WIN16 */

static int  Dumb_MonCreate PROTO((MonitorPtr pMon));
static void Dumb_MonDestroy PROTO((MonitorPtr pMon));
static void Dumb_MonIntValue PROTO((MonitorPtr pMon));
static void Dumb_MonStrValue PROTO((MonitorPtr pMon));



int PASCAL _DefMonitorHook (MonitorPtr pMon, MonCode code)
{
	switch (code)
	{
		case MonCode_Create :
			return Dumb_MonCreate(pMon);
		case MonCode_Destroy :
			Dumb_MonDestroy(pMon);
			break;
		case MonCode_IntValue :
			Dumb_MonIntValue(pMon);
			break;
		case MonCode_StrValue :
			Dumb_MonStrValue(pMon);
			break;
	}
	return 0;
}

static int Dumb_MonCreate (MonitorPtr pMon)
{
	char *buf;

	if ((buf = (char *)Nlm_Calloc(81,1)) == NULL)
            return FALSE;
	buf[80] = '\0';
	pMon->extra = buf;

	fprintf(stderr, "\n\n<<< %s >>>\n", pMon->strTitle);

	if (pMon->type == MonType_Int)
	{
		fprintf(stderr, "<-%-8ld--------------------%8ld->\n",pMon->num1,pMon->num2);
	}
	else
	{
		size_t len = (size_t) pMon->num1;

		MemSet(buf, '<', 3);
		MemSet(&buf[3], ' ', len+2);
		MemSet(&buf[len+5], '>', 3);
		buf[80] = '\0';
		fprintf(stderr, "%s", buf);
	}

	fflush(stderr);
	return TRUE;
}

static void Dumb_MonDestroy (MonitorPtr pMon)
{
	MemFree(pMon->extra);
	fprintf(stderr, "\n\n");
	fflush(stderr);
}

static void Dumb_MonIntValue (MonitorPtr pMon)
{
	char *buf  = (char*) pMon->extra;
	long value = pMon->intValue;
	long range = ABS(pMon->num2 - pMon->num1);
	size_t diff = 0;
	int bRefresh = FALSE;

	if (range ==0)  return;  /* watch out for divide-by-zero! */

	if (pMon->num2 >= pMon->num1)
	{
		if (value > pMon->num1)
		{
			if (value >= pMon->num2)
				diff = 40;
			else
				diff = (size_t) (((value - pMon->num1) * 40) / range);

			if (diff==0 || diff==40 || buf[diff-1] != '#' || buf[diff] != ' ')
			{
				MemSet(&buf[0], '#', diff);
				MemSet(&buf[diff], ' ', 40-diff);
				bRefresh = TRUE;
			}
		}
	}
	else
	{
		if (value < pMon->num1)
		{
			if (value <= pMon->num2)
				diff = 40;
			else
				diff = (size_t) (((value - pMon->num2) * 40) / range);

			if (diff==0 || diff==40 || buf[40-diff] != '#' || buf[40-diff-1] != ' ')
			{
				MemSet(&buf[40 - diff],'#',diff);
				MemSet(&buf[0], ' ', 40-diff);
				bRefresh = TRUE;
			}
		}
	}
	if (bRefresh)
	{
		fprintf(stderr, "\r%s", buf);
		fflush(stderr);
	}
}

static void Dumb_MonStrValue (MonitorPtr pMon)
{
	char *buf = (char *) pMon->extra;
	size_t len0 = (int)pMon->num1;
	size_t len1 = MIN(StringLen(pMon->strValue),len0);
	size_t diff = (len0-len1)/2;
	Nlm_MemFill(&buf[3],' ',len0+2);
	Nlm_MemCopy(&buf[4+diff],pMon->strValue,len1);
	fprintf(stderr,"\r%s",buf);
	fflush(stderr);
}

#endif  /* else WIN16 */


/*****************************************************************************
*
*   Default Monitor
*      This will be moved to ncbimsg soon !!!!
*
*****************************************************************************/


static Nlm_VoidPtr stdmondata = NULL;
static Nlm_ProgMonFunc stdmonfunc = NULL;

/*****************************************************************************
*
*   ProgMon(str)
*   	Default progress monitor
*
*****************************************************************************/
NLM_EXTERN Nlm_Boolean LIBCALL Nlm_ProgMon(Nlm_CharPtr str)
{
	Nlm_Boolean retval;

	if (stdmonfunc == NULL)
		return TRUE;

	retval = (* stdmonfunc)(stdmondata, str);
	
	return retval;
}


/*****************************************************************************
*
*   SetProgMon(func, data)
*
*****************************************************************************/
NLM_EXTERN Nlm_Boolean LIBCALL Nlm_SetProgMon (ProgMonFunc func, Nlm_VoidPtr data)
{
	stdmonfunc = func;
	stdmondata = data;
	return TRUE;
}


/*****************************************************************************
*
*   StdProgMon(data, str)
*
*****************************************************************************/
NLM_EXTERN Nlm_Boolean LIBCALLBACK Nlm_StdProgMon(Nlm_VoidPtr data, Nlm_CharPtr str)
{
	return Nlm_MonitorStrValue((Nlm_MonitorPtr) data, str);
}

