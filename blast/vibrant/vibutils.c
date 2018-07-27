/*   vibutils.c
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  vibutils.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.41 $
*
* File Description:
*       Vibrant miscellaneous functions
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: vibutils.c,v $
* Revision 6.41  2001/11/01 14:25:53  kans
* use times function for UNIX ComputerTime
*
* Revision 6.40  2001/05/17 17:52:34  juran
* Implement Nlm_ClipPrintingRect() for Carbon.
* General cleanup -- heed all warnings.
*
* Revision 6.39  2001/05/16 23:44:31  juran
* Segregate Nlm_PrintingRect() into four functions, implement for Carbon.
*
* Revision 6.38  2001/05/14 20:36:46  juran
* Refactor Mac scrap-thwapping code.
*
* Revision 6.37  2001/04/05 20:00:43  juran
* Major Carbon changes.
Most notably, we attempt to use the new Carbon Printing Manager under 
Carbon, though this has not been tested.
Pre-Carbon printing should work fine, though this has not been tested 
either.
*
* Revision 6.36  2000/07/31 13:28:46  lewisg
* Nlm_GetExecPath
*
* Revision 6.34  2000/07/25 19:18:36  lewisg
* get pathname for executable on windows
*
* Revision 6.33  2000/07/14 22:28:59  lewisg
* fix typo
*
* Revision 6.32  2000/07/14 20:30:41  kans
* added Nlm_MSWin_OpenApplication (for launching Cn3D with a data file)
*
* Revision 6.31  2000/06/29 17:29:11  vakatov
* [MSWIN]  Fixed printf format mismatches
*
* Revision 6.30  2000/02/23 16:21:55  kans
* switched to Churchill fix for nav services file dialogs
*
* Revision 6.29  2000/02/22 16:47:59  kans
* get input/output file name dialog accidentally failed for powerpc without nav services
*
* Revision 6.28  2000/01/07 00:22:47  thiessen
* fixes for LessTif and OpenGL X visual selection
*
* Revision 6.27  1999/12/30 16:58:52  kans
* Carbon changes - printing still not resolved (Churchill)
*
* Revision 6.26  1999/12/21 18:48:44  kans
* rearranged mac get input/output filename functions, soon to expect nav services and to retire SFGetFile and SFPutFile as not Carbon compliant
*
* Revision 6.25  1999/08/23 19:36:39  vakatov
* Nlm_VibMessageHook():  the error posting window should not disappear
* momentarily on FATAL ERROR -- as we want user to see error message(s)
*
* Revision 6.24  1999/08/19 19:05:43  vakatov
* Nlm_VibMessageHook():  always show the "caption" as a title
*
* Revision 6.23  1999/07/04 00:21:13  kans
* Mac Navigation.h changes
*
* Revision 6.22  1999/03/21 18:44:05  kans
* needed ampsersand before creatorType in MemSet
*
* Revision 6.19  1999/02/03 23:26:18  vakatov
* Nlm_ProcessKeydown() to return "TRUE" when moving the input focus
*
* Revision 6.15  1999/01/13 20:49:37  vakatov
* Nlm_Execv():  precations for the MT applications
*
* Revision 6.14  1999/01/13 18:03:24  kans
* Nlm_Execv calls fork1 if SOLARIS_THREADS_AVAIL
*
* Revision 6.13  1999/01/11 18:59:05  kans
* hidden Mac function Nlm_GetFileTypeAndCreator - did not solve MIME detection problem
*
* Revision 6.12  1999/01/07 22:32:38  kans
* added Mac-specific Nlm_SendURLAppleEvent
*
* Revision 6.11  1999/01/06 20:13:00  kans
* MyNavTextFilterProc for TEXT files
*
* Revision 6.10  1999/01/06 02:51:59  kans
* support for Mac Navigation Services file selection dialogs
*
* Revision 6.9  1998/09/15 15:28:15  vakatov
* [WIN_MSWIN]  GetOpen/SaveFileName():  removed OFN_NOCHANGEDIR flag
*
* Revision 6.8  1998/07/07 23:03:36  vakatov
* Added Nlm_Execv() to spawn applications with cmd-line parameters;
* now implemented for [OS_MSWIN, OS_UNIX].
*
* Revision 6.7  1998/03/27 14:01:57  kans
* StrngPrintable on Mac now converts 015 from text object to newline for StringPrintable, then back to 015 to put in text
*
* Revision 6.6  1998/03/25 18:10:17  kans
* VibMessageHook adds newline if post, on Mac it must be octal 15
*
* Revision 6.5  1998/03/22 03:01:04  kans
* changed names to RegisterServiceProc and RegisterResultProc
*
* Revision 6.4  1998/03/22 02:33:22  kans
* added request proc, result proc, message handlers to support, and send open doc event, launch app now work with file names or signatures
*
* Revision 6.3  1998/03/17 21:08:03  kans
* added Nlm_SendOpenDocAppleEventEx, and private Nlm_LaunchAppEx
*
* Revision 6.2  1997/12/12 21:08:48  kans
* a number of symbols changed in the latest CodeWarrior release, now using headers from Apple
*
* Revision 6.1  1997/11/26 21:30:47  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:57:53  madden
* Revision changed to 6.0
*
* Revision 5.31  1997/08/25 16:43:04  vakatov
* [WIN_MSWIN]  Nlm_StartPrinting() -- initialize unused DOCINFO fields
* with 0(using sizeof(DOCINFO), thus avoid the use of preprocessor #ifdef
*
* Revision 5.30  1997/08/21 15:17:40  vakatov
* [WIN_MSWIN]  Nlm_SetupPrinterDeviceContext() -- fixed Int2 overflow in
* the display/printer scaling for 16-bit(Win16 and Win-95) SDK libraries
*
* Revision 5.29  1997/07/24 17:08:09  vakatov
* [WIN_MOTIF]  Explicitly set visual, colormap and depth for FileDialog
* rather than for its parent top-level shell
*
* Revision 5.28  1997/07/21 21:52:41  vakatov
* [WIN_MOTIF] Added Nlm_[Un]RegisterIO() functions to wait for I/O
* events and call(via X) user-defined callback(s)
*
* Revision 5.27  1997/07/16 20:09:52  vakatov
* Added Nlm_StrngPrintable();  use that in Nlm_VibMessageHook() to
* provide proper End-Of-Lines in the ScrolledText object
*
* Revision 5.26  1997/07/16 18:00:48  kans
* VibMessageHook newline on Mac is 015
*
* Revision 5.25  1997/07/16 14:10:59  vakatov
* [WIN_MAC] Nlm_VibMessageHook(KEY_NONE):  do not use '\r' before '\n'
*
* Revision 5.24  1997/07/15 17:13:48  vakatov
* Nlm_VibMessageHook(KEY_NONE):  accumulate incoming messages in the
* text object until the message box is(explicitly) closed by the user
*
* Revision 5.23  1997/07/15 12:24:07  kans
* vibmessagehook len value increased
*
* Revision 5.22  1997/07/10 21:49:44  vakatov
* [WIN_X]  Now able to manage windows having different depths(and
* different visuals and GC).
*
* Revision 5.21  1997/06/23 22:14:18  vakatov
* VibMessageHook() -- [key == KEY_NONE]  use scrolled text instead of
* static prompt to provide printout of the whole message
*
* Revision 5.20  1997/05/27 21:55:32  vakatov
* Use Nlm_PopupParentWindow() to avoid flicking on the message box pop-up
*
* Revision 5.19  1997/05/27 18:12:29  kans
* use post message window if key_none, regardless of severity
*
* Revision 5.18  1997/05/19 21:48:11  vakatov
* Do not deliver input focus to the [KEY_NONE,SEV_INFO] message box
*
* Revision 5.17  1997/04/28 14:05:39  vakatov
* [WIN_MOTIF]  Replaced XK_Page_Up/Down(undef for SunOS X4) by XK_Prior/Next
*
 * Revision 5.16  1997/04/25  16:10:42  vakatov
 * [WIN_MOTIF,WIN_MSWIN]  Added functions to catch, convert(to Mac keycodes)
 * and render navigation and special key events
 *
 * Revision 5.15  1997/04/10  20:23:54  kans
 * monitor only selects window to front on creation, not change value, to
 * allow user to work on other windows without constant focus change
 *
 * Revision 5.14  1997/04/04  22:37:11  vakatov
 * [WIN_MSWIN] In SetupPrinterDeviceContext(), set the window and viewport
 * extents with a more accurate way.
 *
 * Revision 5.13  1997/01/29  17:53:59  kans
 * minor changes due to StringNCpy_0 change
 *
 * Revision 5.12  1997/01/29  16:41:22  kans
 * using StringNCpy_0
 *
 * Revision 5.11  1997/01/16  21:59:20  vakatov
 * [WIN_MSWIN]  StartPicture() -- set picture origin coords(SetWindowOrgEx)
 *
 * Revision 5.10  1997/01/03  16:11:44  vakatov
 * Fixed inaccurate string copying -- <mostly potential> 1-byte exceeding of
 * the string size by StringNCat;  missing terminating '\0' by StringNCpy.
 *
 * Revision 5.9  1996/12/12  23:11:10  kans
 * added Nlm_ClipPrintingRect (DV)
 *
 * Revision 5.8  1996/12/04  19:54:20  vakatov
 * [WIN_MSWIN]  Nlm_CopyDefaultName():  fixed the cases resulting in "."
 *
 * Revision 5.7  1996/11/08  16:43:10  vakatov
 * Nlm_RemoveLink():  do not forget to re-assign "lastChild" when removing
 * the tailing link in the children list(...crash!).
 * Got rid of all compiler warnings.
 *
 * Revision 5.6  1996/10/28  21:21:51  vakatov
 * Nlm_CreateFileDialogShell():  colormap set to Nlm_VibrantDefaultColormap()
 *
 * Revision 5.5  1996/10/09  19:53:18  vakatov
 * [WIN_MOTIF]  Nlm_SelectMonitor() :  explicit call to XMapRaised() led to
 * a side effect causing X-error; -- now replaced by the call to Nlm_Select()
 *
 * Revision 5.4  1996/09/09  20:46:44  vakatov
 * [WIN_MSWIN]  GetOpen/SaveFileName():  added OFN_NOCHANGEDIR flag
 *
 * Revision 5.3  1996/08/30  18:44:09  kans
 * protect against NULL HandFree in SetTitle
 *
 * Revision 5.2  1996/07/05  16:27:45  kans
 * monitor now checks hasCancelBtn param in making cancel button
 *
 * Revision 5.1  1996/06/24  19:24:28  vakatov
 * [WIN_MOTIF]  Nlm_VibMessageHook() -- Set default buttons for the
 * pop-up messages
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.17  1996/05/20  21:30:57  vakatov
 * [WIN_MOTIF]  All "Widget" / "NULL" comparisons and assignments replaced
 * by the "Widget" / "(Widget)0" these
 *
 * Revision 4.16  1996/05/03  21:03:43  kans
 * removed erroneous textscrapfull = FALSE lines
 *
 * Revision 4.15  1996/05/03  17:58:29  vakatov
 * [WIN_MSWIN only]  Check for the MS-Window version when operating with
 * the _DOCINFOA structure
 *
 * Revision 4.14  1996/04/24  21:22:39  vakatov
 * New function Nlm_WaitForXEvent( void ) added (for WIN_MOTIF only!)
 *
 * Revision 4.13  1996/03/02  22:36:38  kans
 * reduction of X traffic (DV)
 *
 * Revision 4.12  1996/02/21  19:46:19  kans
 * GetDriveType endif was in the wrong place
 *
 * Revision 4.11  1996/02/14  16:54:17  kans
 * hidden Nlm_GetDriveType function for entrezcf.c
 *
 * Revision 4.10  1996/02/13  19:49:04  kans
 * set realized calls get/set graphic data, because of gdata cache
 *
 * Revision 4.9  1996/02/13  17:24:07  kans
 * accelerated set position prior to realization (Denis Vakatov)
 *
 * Revision 4.8  1995/12/06  19:45:20  kans
 * removed ArrowCursor from Mac version of VibMessageHook
 *
 * Revision 4.7  1995/11/21  17:37:26  smirnov
 * Alex: Win95 printing problem
 *
 * Revision 4.6  1995/11/14  13:42:05  kans
 * fixes to FntGetFontStyle (VL)
 *
 * Revision 4.5  1995/11/08  23:30:31  kans
 * removed edit block fields, which belong in the application
 *
 * Revision 4.4  1995/10/06  16:46:51  epstein
 * break-out 'patient' modal-window loop into a new macro Nlm_WaitForCondition, and add function Nlm_FineGranularitySleep
 *
 * Revision 4.3  1995/09/12  17:59:35  ostell
 * fixes for MS windows text to metafile
 *
 * Revision 4.2  1995/08/29  18:19:09  kans
 * changes to clipboard functions under Motif (AS)
 *
 * Revision 4.1  1995/08/14  21:38:37  kans
 * removal of remaining C str... functions
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 2.80  1995/07/19  16:21:33  kans
 * fixed IRIX4 XmClipboardStartCopy complaint
 *
 * Revision 2.79  1995/07/17  22:02:19  kans
 * Motif clipboard cut and paste supported (AS)
 *
 * Revision 2.78  1995/07/05  14:23:19  kans
 * move #include <sys/select.h> to ncbilcl.r6k
 *
 * Revision 2.77  1995/06/08  15:46:15  kans
 * added special event loop to monitor functions for X responsiveness
 *
 * Revision 2.76  1995/06/07  20:14:13  kans
 * better performance of monitor, message loops
 *
 * Revision 2.75  1995/06/05  21:32:25  kans
 * added cancel button to progress monitor
 *
 * Revision 2.74  1995/05/31  18:00:58  kans
 * added SetColorMap (AS)
 *
 * Revision 2.73  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>
#include <ncbiport.h>

#ifdef WIN_MAC
# include <Navigation.h>
# if TARGET_API_MAC_CARBON
// Use non-session APIs of the Carbon Printing Manager, for easy porting
#  define PM_USE_SESSION_APIS 0
#  include <PMApplication.h>
# endif
#endif

#ifdef WIN_MOTIF
#include <sys/times.h>
#include <limits.h>
#endif

#ifdef WIN_MAC
Nlm_Int2     Nlm_nextIdNumber = 2;
#endif
#ifdef WIN_MSWIN
Nlm_Int2     Nlm_nextIdNumber = 102;
#endif
#ifdef WIN_MOTIF
Nlm_Int2     Nlm_nextIdNumber = 1;
#endif

Nlm_PoinT    Nlm_globalMouse;
Nlm_PoinT    Nlm_localMouse;

Nlm_Int2     Nlm_hScrollBarHeight;
Nlm_Int2     Nlm_vScrollBarWidth;

Nlm_Int2     Nlm_dialogTextHeight;
Nlm_Int2     Nlm_popupMenuHeight;

Nlm_Char     Nlm_currentKey = '\0';

Nlm_Boolean  Nlm_cmmdKey = FALSE;
Nlm_Boolean  Nlm_ctrlKey = FALSE;
Nlm_Boolean  Nlm_optKey = FALSE;
Nlm_Boolean  Nlm_shftKey = FALSE;
Nlm_Boolean  Nlm_dblClick = FALSE;

static Nlm_Boolean      errorBoxUp;
static Nlm_Int2         errorBoxRsult;

static Nlm_GraphiC      recentGraphic = NULL;
static Nlm_GraphicData  recentGraphicData;

static Nlm_BoX          recentBox = NULL;
static Nlm_BoxData      recentBoxData;

#ifdef WIN_MAC
# if TARGET_API_MAC_CARBON
static PMPageFormat    pageFormat = kPMNoPageFormat;
static PMPrintSettings printSettings = kPMNoPrintSettings;
static PMPrintContext  thePrintingPort = kPMNoReference;
# else
static THPrint   prHdl = NULL;
static TPPrPort  prPort = NULL;
# endif
static Nlm_Int2  prerr;
static Nlm_Char  fileTypes [32] = {0};
#endif

#ifdef WIN_MSWIN
static Nlm_Boolean   abortPrint;
static HDC           hPr = NULL;
static int           prerr;
static int           vibrant_disabled = 0;
static int           disabled_count = 0;
static PRINTDLG      pd;
static OPENFILENAME  ofn;
#endif

#ifdef WIN_MOTIF
static Widget       fileDialog = NULL;
static Nlm_Boolean  fileBoxUp;
static Nlm_Boolean  fileBoxRsult;
static Nlm_Char     filePath [256];
#endif

extern Nlm_Boolean Nlm_usesMacNavServices;

extern Nlm_CharPtr Nlm_StrngPrintable(const Nlm_Char PNTR str)
{
#ifdef WIN_MAC
  Nlm_CharPtr s, x_str, tmp;

  if (str == NULL) return NULL;
  tmp = Nlm_StringSave (str);
  for (s = tmp;  *s;  s++) {
    if (*s == '\r') {
      *s = '\n'; /* convert 015 in text object to newline for StringPrintable */
    }
  }
  x_str = Nlm_StringPrintable(tmp, FALSE);
  Nlm_MemFree (tmp);
  if ( !x_str )
    return NULL;

  for (s = x_str;  *s;  s++) {
    if (*s == '\n' || *s == '\r') {
      *s = '\015'; /* convert back for text object */
    }
  }
  return x_str;
#else
  return Nlm_StringPrintable(str, TRUE);
#endif
}


extern Nlm_Uint4 Nlm_StrngLen (Nlm_CharPtr string)
{
  Nlm_Uint4  len;

  len = 0;
  if (string != NULL) {
    while (*string != '\0') {
      string++;
      len++;
    }
  }
  return len;
}

extern void Nlm_StrngCat (Nlm_CharPtr dest, Nlm_CharPtr source, size_t maxsize)

{
  Nlm_Uint4  count;
  Nlm_Uint4  i;

  if (dest != NULL && source != NULL && maxsize > 0) {
    count = Nlm_StrngLen (source);
    i = Nlm_StrngLen (dest);
    if (count + i >= maxsize) {
      count = maxsize - i - 1;
    }
    dest += i;
    while (count > 0) {
      *dest = *source;
      dest++;
      source++;
      count--;
    }
    *dest = '\0';
  }
}

extern void Nlm_StrngCpy (Nlm_CharPtr dest, Nlm_CharPtr source, size_t maxsize)

{
  Nlm_Uint4  count;

  if (dest != NULL && maxsize > 0) {
    count = Nlm_StrngLen (source);
    if (count >= maxsize) {
      count = maxsize - 1;
    }
    while (count > 0) {
      *dest = *source;
      dest++;
      source++;
      count--;
    }
    *dest = '\0';
  }
}

static Nlm_Boolean Nlm_InNumber (Nlm_Char ch)

{
  return (Nlm_Boolean) (ch >= '0' && ch <= '9') ;
}

static Nlm_Char Nlm_Cap (Nlm_Char ch, Nlm_Boolean caseCounts)

{
  if (caseCounts) {
    return ch;
  } else if (ch >= 'a' && ch <= 'z') {
    return (Nlm_Char) (ch - ' ');
  } else {
    return ch;
  }
}

extern Nlm_Boolean Nlm_StrngPos (Nlm_CharPtr str, Nlm_CharPtr sub,
                                 Nlm_Uint4 start, Nlm_Boolean caseCounts,
                                 Nlm_Uint4Ptr match)

{
  Nlm_Char     ch1;
  Nlm_Char     ch2;
  Nlm_Uint4    count;
  Nlm_Boolean  found;
  Nlm_Uint4    i;
  Nlm_Uint4    j;
  Nlm_Uint4    len;
  Nlm_Uint4    max;
  Nlm_Uint4    sublen;

  found = FALSE;
  if (str != NULL && sub != NULL) {
    len = Nlm_StrngLen (str);
    sublen = Nlm_StrngLen (sub);
    if (len < sublen) {
      sublen = len;
    }
    max = len - sublen;
    i = start;
    if (match != NULL) {
      *match = 0;
    }
    if (start < len) {
      do {
        found = TRUE;
        j = 0;
        count = sublen;
        while (found && count > 0) {
          ch1 = Nlm_Cap (str [i + j], caseCounts);
          ch2 = Nlm_Cap (sub [j], caseCounts);
          if (ch1 != ch2) {
            found = FALSE;
          }
          j++;
          count--;
        }
        if (found && match != NULL) {
          *match = i;
        }
        i++;
      } while ((! found) && (i <= max));
    }
    else {
      found = FALSE;
    }
  }
  return found;
}

extern void Nlm_StrngSeg (Nlm_CharPtr dest, Nlm_CharPtr source,
                          Nlm_Uint4 start, Nlm_Uint4 length,
                          size_t maxsize)

{
  Nlm_Uint4  count;
  Nlm_Uint4  len;

  if (dest != NULL && source != NULL && maxsize > 0) {
    count = length;
    len = Nlm_StrngLen (source);
    if (start + length > len) {
      count = len - start;
    }
    if (count >= maxsize) {
      count = maxsize - 1;
    }
    source += start;
    while (count > 0) {
      *dest = *source;
      dest++;
      source++;
      count--;
    }
    *dest = '\0';
  }
}

extern void Nlm_StrngRep (Nlm_CharPtr dest, Nlm_CharPtr source,
                          Nlm_Uint4 start, size_t maxsize)

{
  Nlm_Uint4  count;

  if (dest != NULL && source != NULL && maxsize > 0) {
    count = Nlm_StrngLen (source);
    if (count + start >= maxsize) {
      count = maxsize - start - 1;
    }
    dest += start;
    while (count > 0) {
      *dest = *source;
      dest++;
      source++;
      count--;
    }
  }
}

extern Nlm_Boolean Nlm_StrngEql (Nlm_CharPtr str1, Nlm_CharPtr str2,
                                 Nlm_Boolean caseCounts)

{
  return (Nlm_Boolean) (Nlm_StrngCmp (str1, str2, caseCounts) == 0);
}

extern Nlm_Int2 Nlm_StrngCmp (Nlm_CharPtr str1, Nlm_CharPtr str2,
                              Nlm_Boolean caseCounts)

{
  Nlm_Uint4  count;
  Nlm_Uint4  i;
  Nlm_Uint4  len1;
  Nlm_Uint4  len2;
  Nlm_Int2   rsult;

  rsult = 0;
  if (str1 != NULL && str2 != NULL) {
    len1 = Nlm_StrngLen (str1);
    len2 = Nlm_StrngLen (str2);
    if (len1 > len2) {
      count = len2;
    } else {
      count = len1;
    }
    i = 0;
    while (count > 0 && Nlm_Cap (str1 [i], caseCounts) == Nlm_Cap (str2 [i], caseCounts)) {
      i++;
      count--;
    }
    if (count > 0) {
      if (Nlm_Cap (str1 [i], caseCounts) > Nlm_Cap (str2 [i], caseCounts)) {
        rsult = 1;
      } else {
        rsult = -1;
      }
    } else {
      if (len1 > len2) {
        rsult = 1;
      } else if (len1 < len2) {
        rsult = -1;
      } else {
        rsult = 0;
      }
    }
  }
  return rsult;
}

extern Nlm_Int2 Nlm_SymblCmp (Nlm_CharPtr str1, Nlm_CharPtr str2,
                              Nlm_Boolean caseCounts)

{
  Nlm_Boolean  cont;
  Nlm_Boolean  done;
  Nlm_Uint4    i;
  Nlm_Uint4    j;
  Nlm_Uint4    len1;
  Nlm_Uint4    len2;
  Nlm_Uint4    num1;
  Nlm_Uint4    num2;
  Nlm_Int2     rsult;

  rsult = 0;
  if (str1 != NULL && str2 != NULL) {
    done = FALSE;
    len1 = Nlm_StrngLen (str1);
    len2 = Nlm_StrngLen (str2);
    i = 0;
    j = 0;
    cont = (Nlm_Boolean) (len1 > 0 && len2 > 0);
    while (cont) {
      if (Nlm_InNumber (str1 [i]) && Nlm_InNumber (str2 [j])) {
        num1 = 0;
        while (i < len1 && Nlm_InNumber (str1 [i]) && (num1 < 429496729)) {
          num1 *= 10;
          num1 += (str1 [i] - '0');
          i++;
        }
        num2 = 0;
        while (j < len2 && Nlm_InNumber (str2 [j]) && (num2 < 429496729)) {
          num2 *= 10;
          num2 += (str2 [j] - '0');
          j++;
        }
        if (num1 > num2) {
          rsult = 1;
          done = TRUE;
          cont = FALSE;
        } else if (num1 < num2) {
          rsult = -1;
          done = TRUE;
          cont = FALSE;
        } else {
          cont = (Nlm_Boolean) (i < len1 && j < len2);
        }
      } else if (Nlm_Cap (str1 [i], caseCounts) > Nlm_Cap (str2 [j], caseCounts)) {
        rsult = 1;
        done = TRUE;
        cont = FALSE;
      } else if (Nlm_Cap (str1 [i], caseCounts) < Nlm_Cap (str2 [j], caseCounts)) {
        rsult = -1;
        done = TRUE;
        cont = FALSE;
      } else {
        i++;
        j++;
        cont = (Nlm_Boolean) (i < len1 && j < len2);
      }
    }
    if (done) {
    } else if (i < len1 && j == len2) {
      rsult = 1;
    } else if (i == len1 && j < len2) {
      rsult = -1;
    } else {
      rsult = 0;
    }
  }
  return rsult;
}

extern Nlm_Handle Nlm_SetString (Nlm_Handle h, Nlm_CharPtr str)

{
  size_t       len;
  Nlm_CharPtr  pp;

  len = 0;
  if (str != NULL) {
    len = Nlm_StringLen (str);
    if (len > 0) {
      if (h != NULL) {
        h = Nlm_HandMore (h, len + 1);
      } else {
        h = Nlm_HandNew (len + 1);
      }
    } else if (h != NULL) {
      Nlm_HandFree (h);
      h = NULL;
    }
  } else if (h != NULL) {
    Nlm_HandFree (h);
    h = NULL;
  }
  if (h != NULL) {
    pp = (Nlm_CharPtr) Nlm_HandLock (h);
    if (pp != NULL) {
      Nlm_StringNCpy (pp, str, len + 1); /* remains StringNCpy, not _0 */
    }
    Nlm_HandUnlock (h);
  }
  return h;
}


extern void Nlm_GetString (Nlm_Handle h, Nlm_CharPtr str, size_t maxsize)
{
  if (str == NULL  ||  maxsize <= 0)
    return;

  str[0] = '\0';

  if (h == NULL)
    return;

  {{
    Nlm_CharPtr pp = (Nlm_CharPtr)Nlm_HandLock( h );
    if (pp != NULL)
      Nlm_StringNCpy_0 (str, pp, maxsize);
    Nlm_HandUnlock( h );
  }}
}

#ifndef WIN_MSWIN
static void Nlm_MssgErrorProc (Nlm_ButtoN b)
{
  errorBoxUp = FALSE;
  errorBoxRsult = ANS_NO;
}

static void Nlm_MssgFatalProc (Nlm_ButtoN b)
{
  errorBoxUp = FALSE;
  errorBoxRsult = ANS_NO;
}

static void Nlm_MssgNoProc (Nlm_ButtoN b)
{
  errorBoxUp = FALSE;
  errorBoxRsult = ANS_NO;
}

static void Nlm_MssgYesProc (Nlm_ButtoN b)
{
  errorBoxUp = FALSE;
  errorBoxRsult = ANS_YES;
}

static void Nlm_MssgOkayProc (Nlm_ButtoN b)
{
  errorBoxUp = FALSE;
  errorBoxRsult = ANS_OK;
}

static void Nlm_MssgRetryProc (Nlm_ButtoN b)
{
  errorBoxUp = FALSE;
  errorBoxRsult = ANS_RETRY;
}

static void Nlm_MssgAbortProc (Nlm_ButtoN b)
{
  errorBoxUp = FALSE;
  errorBoxRsult = ANS_ABORT;
}

static void Nlm_MssgCancelProc (Nlm_ButtoN b)
{
  errorBoxUp = FALSE;
  errorBoxRsult = ANS_CANCEL;
}

static void Nlm_MssgIgnoreProc (Nlm_ButtoN b)
{
  errorBoxUp = FALSE;
  errorBoxRsult = ANS_IGNORE;
}
#endif /* ndef WIN_MSWIN */


static void s_CloseAWindow(Nlm_WindoW w)
{
  Nlm_Reset((Nlm_TexT) Nlm_GetObjectExtra(w));
  Nlm_Hide(w);
}


static Nlm_Boolean s_FatalModalWindowUp;
static void s_CloseFatalModalWindow_W(Nlm_WindoW w)
{
  s_FatalModalWindowUp = FALSE;
}
static void s_CloseFatalModalWindow_B(Nlm_ButtoN b)
{
  s_CloseFatalModalWindow_W( Nlm_Parent(b) );
}


static MsgAnswer LIBCALLBACK Nlm_VibMessageHook (MsgKey key, ErrSev severity,
                                                 const char * caption,
                                                 const char * message)
{
  Nlm_Int2     rsult = 0;
#if defined(WIN_MAC) || defined(WIN_MOTIF)
  size_t       len;
  Nlm_CharPtr  buf = NULL;
  Nlm_ButtoN   b[3];
  Nlm_Char     ch;
  Nlm_Int2     delta;
  Nlm_GrouP    g;
  Nlm_Int2     i;
  Nlm_Int2     j;
  Nlm_Int2     k;
  Nlm_Int2     margin;
  Nlm_Int2     maxWidth;
  Nlm_PoinT    npt;
  Nlm_Int4     percent;
  Nlm_RecT     r;
  Nlm_WindoW   w;
  Nlm_Int2     width;
#endif
#ifdef WIN_MSWIN
  Nlm_Int4     answer;
  Nlm_Int2     wtype;
#endif

  const char* x_caption = caption ? caption : "Message";

  /* Use a window (or a modal dialog) to accumulate & post the message */
  if (key == KEY_NONE  ||  severity >= ErrGetFatalLevel()) {
    static Nlm_WindoW postWindow = 0;
    static Nlm_TexT   postText   = 0;
    Nlm_WindoW tempPort = Nlm_CurrentWindow();

    /* Prepare the message text (concat with the previous text, if any) */
    char* x_text;
    const char* add_mess = message ? message : "<no message posted>";
    if ( postText ) {
      size_t curr_len = Nlm_TextLength(postText);
      size_t add_len  = Nlm_StringLen( message );
      char*  buf    = (char*) Nlm_MemNew(add_len+1 + curr_len + 1);
      Nlm_MemCpy(buf, add_mess, add_len);
      buf[add_len++] = '\n';
      Nlm_GetTitle(postText, buf+add_len, curr_len+1);
      x_text = Nlm_StrngPrintable(buf);
      Nlm_MemFree(buf);
    } else {
      x_text = Nlm_StrngPrintable(add_mess);
    }

    /* Fatal or non-fatal error severity (modal or non-modal dialog) */
    if (severity >= ErrGetFatalLevel()) {
      /* Fatal error -- use modal dialog window */
      Nlm_WindoW modalWindow = Nlm_ModalWindow(-50, -90, -20, -20,
                                               s_CloseFatalModalWindow_W);
      Nlm_TexT   modalText = Nlm_ScrollText(modalWindow, 64, 16,
                                            Nlm_systemFont, TRUE, NULL);
      Nlm_ButtoN modalButton = Nlm_DefaultButton(modalWindow, "Close",
                                                 s_CloseFatalModalWindow_B);
      Nlm_SetObjectExtra(modalWindow, modalText, NULL);
      Nlm_SetTextEditable(modalText, FALSE);
      Nlm_SetTitle(modalText, x_text);      

      /* hide the regular message posting window */
      if (postWindow  &&  Nlm_Visible(postWindow))
        Nlm_Hide(postWindow);

      /* show the modal dialog */
      Nlm_Show(modalWindow);
      Nlm_PopupParentWindow(modalWindow);
      s_FatalModalWindowUp = TRUE;
      Nlm_WaitForCondition(s_FatalModalWindowUp);
      Nlm_ProcessAnEvent();
      Nlm_Remove(modalWindow);
    } else {
      /* Non-Fatal error -- use normal window */
      if ( !postWindow ) {
        /* create normal message posting window */
        postWindow = Nlm_FixedWindow(-50, -90, -20, -20, (char*)x_caption,
                                     s_CloseAWindow);
        postText = Nlm_ScrollText(postWindow,  (Nlm_Int2)32, (Nlm_Int2)5,
                                  Nlm_systemFont, TRUE, NULL);
        Nlm_SetObjectExtra(postWindow, postText, NULL);
        if ( postText )
          Nlm_SetTextEditable(postText, FALSE);
      }

      /* set the message text and popup the window */
      if ( postText )
          Nlm_SetTitle(postText, x_text);
      Nlm_PopupParentWindow( postWindow );
    }

    Nlm_MemFree(x_text);
    Nlm_RestorePort( tempPort );
    return (MsgAnswer)ANS_NONE;
  }

#ifdef WIN_MSWIN
  wtype = MB_OK;
  switch (key) {
    case KEY_OK:
      if (severity == SEV_ERROR) {
        wtype = MB_OK;
      } else if (severity == SEV_FATAL) {
        wtype = MB_OK;
      } else {
        wtype = MB_OK;
      }
      break;
    case KEY_RC:
      wtype = MB_RETRYCANCEL;
      break;
    case KEY_ARI:
      wtype = MB_ABORTRETRYIGNORE;
      break;
    case KEY_YN:
      wtype = MB_YESNO;
      break;
    case KEY_YNC:
      wtype = MB_YESNOCANCEL;
      break;
    case KEY_OKC:
      wtype = MB_OKCANCEL;
      break;
    case KEY_NONE:
      wtype = MB_OK;
      break;
    default:
      break;
  }

  answer = MessageBox(NULL, message, x_caption, wtype | MB_TASKMODAL);
  switch (answer) {
    case IDNO:
      rsult = ANS_NO;
      break;
    case IDYES:
      rsult = ANS_YES;
      break;
    case IDOK:
      rsult = ANS_OK;
      break;
    case IDRETRY:
      rsult = ANS_RETRY;
      break;
    case IDABORT:
      rsult = ANS_ABORT;
      break;
    case IDCANCEL:
      rsult = ANS_CANCEL;
      break;
    case IDIGNORE:
      rsult = ANS_IGNORE;
      break;
    default:
      break;
  }
#endif /* WIN_MSWIN */

#if defined(WIN_MOTIF) || defined(WIN_MAC)
  len = MIN(Nlm_StringLen(message) + 1, 4096);
  buf = (Nlm_CharPtr) Nlm_MemNew(len);
  Nlm_StringNCpy_0(buf, message, len);

  percent = (Nlm_Int4)Nlm_GetAppProperty( "VibrantMessageWidthMax" );
  if (percent <= 0  ||  95 < percent)
    percent = 95;
  else if (percent < 25)
    percent = 25;

  maxWidth = (Nlm_Int2)(percent *
                      (Nlm_screenRect.right - Nlm_screenRect.left) / 100 - 40);
  maxWidth = MAX (maxWidth, 100);

  w = Nlm_MovableModalWindow (-50, -20, -20, -20, (char*)x_caption, NULL);
  g = Nlm_HiddenGroup (w, 0, 10, NULL);
  Nlm_GetNextPosition (g, &npt);
  npt.x += 6;
  Nlm_SetNextPosition (g, npt);

  i = 0;
  while (buf[i] != '\0') {
    width = 0;
    j = 0;
    while (buf [i + j] == ' ') {
      i++;
    }
    ch = buf [i + j];
    width += Nlm_CharWidth (ch);
    while (ch != '\0' && ch != '\n' && ch != '\r' && width <= maxWidth && j < 125) {
      j++;
      ch = buf [i + j];
      width += Nlm_CharWidth (ch);
    }
    if (width > maxWidth) {
      ch = buf [i + j];
      while (j > 0 && ch != ' ' && ch != '-') {
        j--;
        ch = buf [i + j];
      }
    } else if (j >= 125) {
      k = j;
      ch = buf [i + k];
      while (k > 0 && ch != ' ' && ch != '-') {
        k--;
        ch = buf [i + k];
      }
      if (k > 80) {
        j = k;
      }
      ch = buf [i + j];
    }
    if (ch == '\n' || ch == '\r') {
      buf [i + j] = '\0';
      Nlm_StaticPrompt (g, buf + i, 0, 0, Nlm_systemFont, 'l');
      i += j + 1;
    } else {
      buf [i + j] = '\0';
      Nlm_StaticPrompt (g, buf + i, 0, 0, Nlm_systemFont, 'l');
      buf [i + j] = ch;
      i += j;
    }
  }
  Nlm_MemFree( buf );

  Nlm_GetPosition (g, &r);
  margin = r.right;
  Nlm_Break ((Nlm_GraphiC) w);
  b [0] = NULL;
  b [1] = NULL;
  b [2] = NULL;
  switch (key) {
    case KEY_OK:
      if (severity == SEV_ERROR) {
        b [0] = Nlm_DefaultButton (w, "OK", Nlm_MssgErrorProc);
      } else if (severity == SEV_FATAL) {
        b [0] = Nlm_DefaultButton (w, "OK", Nlm_MssgFatalProc);
      } else {
        b [0] = Nlm_DefaultButton (w, "OK", Nlm_MssgOkayProc);
      }
      break;
    case KEY_RC:
      b [0] = Nlm_PushButton (w, "Retry", Nlm_MssgRetryProc);
      Nlm_Advance ((Nlm_GraphiC) w);
      b [1] = Nlm_DefaultButton (w, "Cancel", Nlm_MssgCancelProc);
      break;
    case KEY_ARI:
      b [0] = Nlm_PushButton (w, "Abort", Nlm_MssgAbortProc);
      Nlm_Advance ((Nlm_GraphiC) w);
      b [1] = Nlm_PushButton (w, "Retry", Nlm_MssgRetryProc);
      Nlm_Advance ((Nlm_GraphiC) w);
      b [2] = Nlm_DefaultButton (w, "Ignore", Nlm_MssgIgnoreProc);
      break;
    case KEY_YN:
      b [0] = Nlm_DefaultButton (w, "Yes", Nlm_MssgYesProc);
      Nlm_Advance ((Nlm_GraphiC) w);
      b [1] = Nlm_PushButton (w, "No", Nlm_MssgNoProc);
      break;
    case KEY_YNC:
      b [0] = Nlm_DefaultButton (w, "Yes", Nlm_MssgYesProc);
      Nlm_Advance ((Nlm_GraphiC) w);
      b [1] = Nlm_PushButton (w, "No", Nlm_MssgNoProc);
      Nlm_Advance ((Nlm_GraphiC) w);
      b [2] = Nlm_PushButton (w, "Cancel", Nlm_MssgCancelProc);
      break;
    case KEY_OKC:
      b [1] = Nlm_DefaultButton (w, "OK", Nlm_MssgOkayProc);
      Nlm_Advance ((Nlm_GraphiC) w);
      b [2] = Nlm_PushButton (w, "Cancel", Nlm_MssgCancelProc);
      break;
    case KEY_NONE:
      b [0] = Nlm_DefaultButton (w, "OK", Nlm_MssgOkayProc);
      break;
    default:
      b [0] = Nlm_DefaultButton (w, "OK", Nlm_MssgOkayProc);
      break;
  }
  i = 2;
  while (i >= 0 && b [i] == NULL) {
    i--;
  }
  if (i >= 0) {
    Nlm_GetPosition (b [i], &r);
    delta = (margin - r.right) / 2;
    if (delta > 0) {
      while (i >= 0) {
        Nlm_GetPosition (b [i], &r);
        Nlm_OffsetRect (&r, delta, 0);
        Nlm_SetPosition (b [i], &r);
        i--;
      }
    } else if (delta < 0) {
      Nlm_GetPosition (g, &r);
      Nlm_OffsetRect (&r, -delta, 0);
      Nlm_SetPosition (g, &r);
    }
  }
  Nlm_DoShow ((Nlm_GraphiC) w, TRUE, TRUE);
  errorBoxUp = TRUE;
  errorBoxRsult = 0;

#ifdef WIN_MAC
  while (errorBoxUp) {
    Nlm_ProcessExternalEvent ();
    Nlm_Update ();
  }
#else /* i.e. WIN_MOTIF */
  Nlm_WaitForCondition(errorBoxUp);
#endif

  Nlm_ProcessAnEvent ();
  Nlm_DoRemove ((Nlm_GraphiC) w, TRUE);
  rsult = errorBoxRsult;
#endif /* WIN_MOTIF || WIN_MAC */

  return (MsgAnswer) rsult;
}


typedef struct monitextra {
  Nlm_WindoW   wind;
  Nlm_PaneL    pnl;
  Nlm_Boolean  cancel;
} Nlm_VibMonExtra, PNTR Nlm_VibMonPtr;

static void Nlm_SelectMonitor (Nlm_WindoW w)
{
  Nlm_WindowTool wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) w);
#ifdef WIN_MAC
  /* SelectWindow (wptr); */
  SetPortWindowPort(wptr);
  Nlm_SetUpdateRegion (wptr);
  Nlm_ResetDrawingTools ();
#endif
#ifdef WIN_MSWIN
  /* BringWindowToTop (wptr); */
  Nlm_currentHDC = Nlm_ParentWindowPort ((Nlm_GraphiC) w);
  Nlm_currentHWnd = wptr;
#endif
#ifdef WIN_MOTIF
  /* Nlm_Select ( w ); */
#endif
  Nlm_currentWindowTool = wptr;
}

static int Nlm_VibMonStrValue (Nlm_MonitorPtr mon)

{
  Nlm_VibMonPtr  vmp;
  Nlm_PaneL      pnl;
  Nlm_RecT       r;
  Nlm_WindoW     tempPort;
  Nlm_WindoW     wind;
#ifdef WIN_MOTIF
  XEvent         event;
  Nlm_Uint4      tempBackColor;
  Nlm_Uint4      tempForeColor;
  Nlm_Int2       tempXOffset;
  Nlm_Int2       tempYOffset;
  Window         tempXWind;
  GC             tempXGC;
#endif

  if (mon == NULL || mon->type != MonType_Str || mon->extra == NULL) {
    return 0;
  }
  vmp = (Nlm_VibMonPtr) mon->extra;
  pnl = vmp->pnl;
  wind = vmp->wind;
  tempPort = Nlm_CurrentWindow ();
#ifdef WIN_MOTIF
  tempXWind = Nlm_currentXWindow;
  tempXGC   = Nlm_currentXGC;
  tempBackColor = Nlm_XbackColor;
  tempForeColor = Nlm_XforeColor;
  tempXOffset = Nlm_XOffset;
  tempYOffset = Nlm_YOffset;
#endif
  Nlm_UseWindow (wind);
  Nlm_SelectMonitor (wind);
  Nlm_Select (pnl);
  Nlm_GetRect ((Nlm_GraphiC) pnl, &r);
  Nlm_InsetRect (&r, 2, 2);
  Nlm_DrawString (&r, (Nlm_CharPtr) mon->strValue, 'l', FALSE);
  Nlm_UseWindow (tempPort);
#ifdef WIN_MOTIF
  Nlm_currentXWindow = tempXWind;
  Nlm_currentXGC = tempXGC;
  Nlm_XbackColor = tempBackColor;
  Nlm_XforeColor = tempForeColor;
  Nlm_XOffset = tempXOffset;
  Nlm_YOffset = tempYOffset;
#endif

  Nlm_Update ();

#if defined(WIN_MAC) ||  defined(WIN_MSWIN)
  while (Nlm_MouseButton ()) {
    Nlm_ProcessExternalEvent ();
  }
#endif
#ifdef WIN_MOTIF
  while (XCheckTypedEvent (Nlm_currentXDisplay, ButtonPress, &event) ||
	 XCheckTypedEvent (Nlm_currentXDisplay, ButtonRelease, &event))
    {
      XtDispatchEvent( &event );
    }
#endif
  Nlm_ProcessAnEvent ();

  mon->cancel = vmp->cancel;
  return 0;
}

static int Nlm_VibMonIntValue (Nlm_MonitorPtr mon)

{
  Nlm_Int4       from;
  Nlm_VibMonPtr  vmp;
  Nlm_PaneL      pnl;
  Nlm_RecT       r;
  Nlm_Int4       range;
  Nlm_Int2       right;
  Nlm_WindoW     tempPort;
  Nlm_Int4       to;
  Nlm_Int4       value;
  Nlm_Int4       width;
  Nlm_WindoW     wind;
#ifdef WIN_MOTIF
  XEvent         event;
  Nlm_Uint4      tempBackColor;
  Nlm_Uint4      tempForeColor;
  Nlm_Int2       tempXOffset;
  Nlm_Int2       tempYOffset;
  Window         tempXWind;
  GC             tempXGC;
#endif

  if (mon == NULL || mon->type != MonType_Int || mon->extra == NULL) {
    return 0;
  }
  vmp = (Nlm_VibMonPtr) mon->extra;
  from = MIN (mon->num1, mon->num2);
  to = MAX (mon->num1, mon->num2);
  range = to - from;
  value = mon->intValue;
  value = MAX (value, from);
  value = MIN (value, to);
  value -= from;
  pnl = vmp->pnl;
  wind = vmp->wind;
  tempPort = Nlm_CurrentWindow ();
#ifdef WIN_MOTIF
  tempXWind = Nlm_currentXWindow;
  tempXGC   = Nlm_currentXGC;
  tempBackColor = Nlm_XbackColor;
  tempForeColor = Nlm_XforeColor;
  tempXOffset = Nlm_XOffset;
  tempYOffset = Nlm_YOffset;
#endif
  Nlm_UseWindow (wind);
  Nlm_SelectMonitor (wind);
  Nlm_Select (pnl);
  Nlm_GetRect ((Nlm_GraphiC) pnl, &r);
  Nlm_FrameRect (&r);
  Nlm_InsetRect (&r, 2, 2);
  value = MAX (value, 0);
  value = MIN (value, range);
  width = (Nlm_Int4) (r.right - r.left);
  right = r.right;
  r.right = (Nlm_Int2)(r.left + (width * value / range));
  Nlm_Black ();
  Nlm_PaintRect (&r);
  r.left = r.right;
  r.right = right;
  Nlm_EraseRect (&r);
  Nlm_UseWindow (tempPort);
#ifdef WIN_MOTIF
  Nlm_currentXWindow = tempXWind;
  Nlm_currentXGC = tempXGC;
  Nlm_XbackColor = tempBackColor;
  Nlm_XforeColor = tempForeColor;
  Nlm_XOffset = tempXOffset;
  Nlm_YOffset = tempYOffset;
#endif

  Nlm_Update ();

#if defined(WIN_MAC) || defined(WIN_MSWIN)
  while (Nlm_MouseButton ()) {
    Nlm_ProcessExternalEvent ();
  }
#endif
#ifdef WIN_MOTIF
  while (XCheckTypedEvent (Nlm_currentXDisplay, ButtonPress, &event) ||
	 XCheckTypedEvent (Nlm_currentXDisplay, ButtonRelease, &event))
    {
      XtDispatchEvent( &event );
    }
#endif
  Nlm_ProcessAnEvent ();

  mon->cancel = vmp->cancel;
  return 0;
}

static void Nlm_CancelMonitorProc (Nlm_ButtoN b)

{
  Nlm_VibMonPtr  vmp;

  vmp = (Nlm_VibMonPtr) Nlm_GetObjectExtra (b);
  if (vmp != NULL) {
    vmp->cancel = TRUE;
  }
}

static int Nlm_VibMonCreate (Nlm_MonitorPtr mon)

{
  Nlm_ButtoN     b;
  Nlm_VibMonPtr  vmp;
  Nlm_PaneL      pnl;
  Nlm_WindoW     wind;

  if (mon != NULL) {
    vmp = (Nlm_VibMonPtr) Nlm_MemNew (sizeof (Nlm_VibMonExtra));
    mon->extra = (Nlm_VoidPtr) vmp;
    if (vmp != NULL) {
      wind = Nlm_FixedWindow (-50, -90, -10, -10, (Nlm_CharPtr) mon->strTitle, NULL);
      if (mon->type == MonType_Int) {
        pnl = Nlm_SimplePanel (wind, 200, 20, NULL);
      } else if (mon->type == MonType_Str) {
        pnl = Nlm_SimplePanel(wind, (Nlm_Int2)(Nlm_stdCharWidth*mon->num1+6),
                              (Nlm_Int2)(Nlm_stdLineHeight + 4), NULL);
      } else {
        pnl = Nlm_SimplePanel (wind, 200, 20, NULL);
      }
      Nlm_Advance (wind);
      b = NULL;
      if ((Nlm_Boolean) mon->hasCancelBtn) {
        b = Nlm_DefaultButton (wind, "Cancel", Nlm_CancelMonitorProc);
        Nlm_SetObjectExtra (b, (Nlm_VoidPtr) vmp, NULL);
      }
      Nlm_AlignObjects (ALIGN_MIDDLE, (Nlm_HANDLE) pnl, (Nlm_HANDLE) b, NULL);
      vmp->wind = wind;
      vmp->pnl = pnl;
      vmp->cancel = FALSE;
      Nlm_Show (wind);
      Nlm_Select (wind);
      Nlm_Update ();
    }
  }
  return TRUE;
}

static int Nlm_VibMonDestroy (Nlm_MonitorPtr mon)

{
  Nlm_VibMonPtr  vmp;

  if (mon != NULL) {
    vmp = (Nlm_VibMonPtr) mon->extra;
    if (vmp != NULL) {
      Nlm_Remove (vmp->wind);
      Nlm_MemFree (vmp);
    }
  }
  return 0;
}


static int LIBCALLBACK Nlm_VibMonitorHook(Nlm_MonitorPtr mon, MonCode code)
{
  switch ( code )
    {
    case MonCode_Create:
      return Nlm_VibMonCreate( mon );
    case MonCode_Destroy:
      return Nlm_VibMonDestroy( mon );
    case MonCode_IntValue:
      return Nlm_VibMonIntValue( mon );
    case MonCode_StrValue:
      return Nlm_VibMonStrValue( mon );
    }

  return 0;
}

static void LIBCALLBACK Nlm_VibBeepHook (void)

{
#ifdef OS_MAC
  SysBeep (60);
#endif
#ifdef WIN_MSWIN
  MessageBeep (0);
#endif
#ifdef WIN_MOTIF
  if (Nlm_currentXDisplay != NULL) {
    XBell (Nlm_currentXDisplay, 0);
  }
#endif
}

extern void Nlm_InitVibrantHooks (void)

{
  Nlm_SetBeepHook (Nlm_VibBeepHook);
  Nlm_SetMonitorHook (Nlm_VibMonitorHook);
  Nlm_SetMessageHook (Nlm_VibMessageHook);
}

extern void Nlm_MousePosition (Nlm_PointPtr pt)

{
#ifdef WIN_MAC
  Nlm_PointTool  ptool;

  GetMouse (&ptool);
  if (pt != NULL) {
    Nlm_PointToolToPoinT (ptool, pt);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_PointTool  ptool;

  if (pt != NULL && Nlm_currentHWnd != NULL) {
    GetCursorPos (&ptool);
    ScreenToClient (Nlm_currentHWnd, &ptool);
    Nlm_PointToolToPoinT (ptool, pt);
  }
#endif
#ifdef WIN_MOTIF
#endif
}

extern Nlm_Boolean Nlm_MouseButton (void)

{
#ifdef WIN_MAC
  return (Button ());
#endif
#ifdef WIN_MSWIN
  return (Nlm_Boolean) ((GetAsyncKeyState (VK_LBUTTON) & 0x8000) != 0);
#endif
#ifdef WIN_MOTIF
  return FALSE;
#endif
}

extern Nlm_Int4 Nlm_ComputerTime (void)

{
#ifdef WIN_MAC
  return (TickCount ());
#endif
#ifdef WIN_MSWIN
  return (GetCurrentTime ());
#endif
#ifdef WIN_MOTIF
  struct tms buffer;
  return (Nlm_Int4) times (&buffer);
#endif
}


extern void Nlm_Version (Nlm_CharPtr vsn, size_t maxsize)
{
  Nlm_StringNCpy_0 (vsn, "NCBI VIBRANT Version 2.0 (29 Jan 97)", maxsize);
}


extern void Nlm_Advance (Nlm_Handle a)

{
  Nlm_BoxData  bdata;

  if (a != NULL) {
    Nlm_GetBoxData ((Nlm_BoX) a, &bdata);
    bdata.nextPoint.y = bdata.topRow;
    bdata.nextPoint.x = bdata.nextCol;
    Nlm_SetBoxData ((Nlm_BoX) a, &bdata);
  }
}

extern void Nlm_Break (Nlm_Handle a)

{
  Nlm_BoxData  bdata;

  if (a != NULL) {
    Nlm_GetBoxData ((Nlm_BoX) a, &bdata);
    bdata.topRow = (Nlm_Int2)(bdata.limitPoint.y + bdata.ySpacing);
    bdata.nextCol = bdata.resetPoint.x;
    bdata.nextPoint.y = bdata.topRow;
    bdata.nextPoint.x = bdata.nextCol;
    Nlm_SetBoxData ((Nlm_BoX) a, &bdata);
  }
}

extern void Nlm_RecordRect (Nlm_GraphiC a, Nlm_RectPtr r)

{
  Nlm_BoxData  bdata;
  Nlm_Int2     nc;
  Nlm_Int2     nr;

  if (a != NULL && r != NULL) {
    Nlm_GetBoxData ((Nlm_BoX) a, &bdata);
    nr = r->bottom;
    if (nr > bdata.limitPoint.y) {
      bdata.limitPoint.y = nr;
    }
    nc = r->right;
    if (nc > bdata.limitPoint.x) {
      bdata.limitPoint.x = nc;
    }
    nc = (Nlm_Int2)(r->right + bdata.xSpacing);
    if (nc > bdata.nextCol) {
      bdata.nextCol = nc;
    }
    Nlm_SetBoxData ((Nlm_BoX) a, &bdata);
  }
}

extern void Nlm_NextPosition (Nlm_GraphiC a, Nlm_RectPtr r)

{
  Nlm_BoxData  bdata;

  if (a != NULL && r != NULL) {
    Nlm_GetBoxData ((Nlm_BoX) a, &bdata);
    bdata.nextPoint.y = (Nlm_Int2)(r->bottom + bdata.ySpacing);
    Nlm_SetBoxData ((Nlm_BoX) a, &bdata);
  }
}

extern void Nlm_SetNextPosition (Nlm_Handle a, Nlm_PoinT nps)

{
  Nlm_BoxData  bdata;

  if (a != NULL) {
    Nlm_GetBoxData ((Nlm_BoX) a, &bdata);
    bdata.nextPoint = nps;
    bdata.topRow = nps.y;
    Nlm_SetBoxData ((Nlm_BoX) a, &bdata);
  }
}

extern void Nlm_GetNextPosition (Nlm_Handle a, Nlm_PointPtr nps)

{
  Nlm_BoxData  bdata;

  if (a != NULL && nps != NULL) {
    Nlm_GetBoxData ((Nlm_BoX) a, &bdata);
    *nps = bdata.nextPoint;
  }
}

extern Nlm_GphPrcsPtr Nlm_GetClassPtr (Nlm_GraphiC a)

{
  Nlm_GphPrcsPtr   classPtr;
  Nlm_GraphicData  gdata;

  classPtr = NULL;
  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    classPtr = gdata.classptr;
  }
  return classPtr;
}

#ifdef WIN_MAC
extern Nlm_Boolean Nlm_DoClick (Nlm_GraphiC a, Nlm_PoinT pt)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  Nlm_Boolean     (*clk) PROTO((Nlm_GraphiC, Nlm_PoinT));
  Nlm_Boolean     rsult;

  rsult = FALSE;
  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    clk = classPtr->click;
    if (clk != NULL) {
      rsult = clk (a, pt);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
  return rsult;
}

extern Nlm_Boolean Nlm_DoKey (Nlm_GraphiC a, Nlm_Char ch)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  Nlm_Boolean     (*ky) PROTO((Nlm_GraphiC, Nlm_Char));
  Nlm_Boolean     rsult;

  rsult = FALSE;
  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    ky = classPtr->key;
    if (ky != NULL) {
      rsult = ky (a, ch);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
  return rsult;
}

extern void Nlm_DoDraw (Nlm_GraphiC a)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*drw) PROTO((Nlm_GraphiC));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    drw = classPtr->draw;
    if (drw != NULL) {
      drw (a);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern Nlm_Boolean Nlm_DoIdle (Nlm_GraphiC a, Nlm_PoinT pt)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  Nlm_Boolean     (*idl) PROTO((Nlm_GraphiC, Nlm_PoinT));
  Nlm_Boolean     rsult;

  rsult = FALSE;
  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    idl = classPtr->idle;
    if (idl != NULL) {
      rsult = idl (a, pt);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
  return rsult;
}
#endif

#ifdef WIN_MSWIN
extern Nlm_Boolean Nlm_DoCommand (Nlm_GraphiC a)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  Nlm_Boolean     (*cmd) PROTO((Nlm_GraphiC));
  Nlm_Boolean     rsult;

  rsult = FALSE;
  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    cmd = classPtr->command;
    if (cmd != NULL) {
      rsult = cmd (a);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
  return rsult;
}
#endif

#ifdef WIN_MOTIF
extern void Nlm_DoCallback (Nlm_GraphiC a)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*clb) PROTO((Nlm_GraphiC));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    clb = classPtr->callback;
    if (clb != NULL) {
      clb (a);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}
#endif

extern void Nlm_DoShow (Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*shw) PROTO((Nlm_GraphiC, Nlm_Boolean, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    shw = classPtr->show;
    if (shw != NULL) {
      shw (a, setFlag, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern void Nlm_DoHide (Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*hid) PROTO((Nlm_GraphiC, Nlm_Boolean, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    hid = classPtr->hide;
    if (hid != NULL) {
      hid (a, setFlag, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern void Nlm_DoEnable (Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*enbl) PROTO((Nlm_GraphiC, Nlm_Boolean, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    enbl = classPtr->enable;
    if (enbl != NULL) {
      enbl (a, setFlag, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern void Nlm_DoDisable (Nlm_GraphiC a, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*dsbl) PROTO((Nlm_GraphiC, Nlm_Boolean, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    dsbl = classPtr->disable;
    if (dsbl != NULL) {
      dsbl (a, setFlag, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern void Nlm_DoActivate (Nlm_GraphiC a, Nlm_Boolean savePort)

{
  void            (*actvate) PROTO((Nlm_GraphiC, Nlm_Boolean));
  Nlm_Boolean     cont;
  Nlm_GphPrcsPtr  classPtr;

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    actvate = classPtr->activate;
    if (actvate != NULL) {
      actvate (a, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern void Nlm_DoDeactivate (Nlm_GraphiC a, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*deactvate) PROTO((Nlm_GraphiC, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    deactvate = classPtr->deactivate;
    if (deactvate != NULL) {
      deactvate (a, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern Nlm_Handle Nlm_DoRemove (Nlm_GraphiC a, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*rmv) PROTO((Nlm_GraphiC, Nlm_Boolean));

  cont = TRUE;
  
#if !defined(WIN_MOTIF) || !defined(LESSTIF_VERSION)
  /* using LessTif (on Linux), this eventually causes a segfault down the
     line when quitting the application - is this really necessary? (thiessen) */
  Nlm_DoHide (a, TRUE, savePort);
#endif

  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    rmv = classPtr->remove;
    if (rmv != NULL) {
      rmv (a, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
    recentGraphic = NULL;
    recentBox = NULL;
  }
  return NULL;
}

extern void Nlm_DoReset (Nlm_GraphiC a, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*rst) PROTO((Nlm_GraphiC, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    rst = classPtr->reset;
    if (rst != NULL) {
      rst (a, TRUE);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern void Nlm_DoSelect (Nlm_GraphiC a, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*sel) PROTO((Nlm_GraphiC, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    sel = classPtr->select;
    if (sel != NULL) {
      sel (a, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern Nlm_Int2 Nlm_CountItems (Nlm_Handle a)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  Nlm_Int2        (*cntitm) PROTO((Nlm_GraphiC));
  Nlm_Int2        len;

  cont = TRUE;
  len = 0;
  classPtr = Nlm_GetClassPtr ((Nlm_GraphiC)a);
  while (classPtr != NULL && cont) {
    cntitm = classPtr->countItems;
    if (cntitm != NULL) {
      len = cntitm ((Nlm_GraphiC)a);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
  return len;
}

extern Nlm_GraphiC Nlm_DoLinkIn (Nlm_GraphiC a, Nlm_GraphiC prnt)

{
  Nlm_GphPrcsPtr  prntClassPtr;
  Nlm_Boolean     cont;
  Nlm_GraphiC     (*lnkIn) PROTO((Nlm_GraphiC, Nlm_GraphiC));
  Nlm_GraphiC     rsult;

  cont = TRUE;
  rsult = NULL;
  if (a != NULL && prnt != NULL) {
    cont = TRUE;
    prntClassPtr = Nlm_GetClassPtr (prnt);
    while (prntClassPtr != NULL && cont) {
      lnkIn = prntClassPtr->linkIn;
      if (lnkIn != NULL) {
        rsult = lnkIn (a, prnt);
        cont = FALSE;
      } else {
        prntClassPtr = prntClassPtr->ancestor;
      }
    }
  }
  return rsult;
}

extern void Nlm_DoAdjustPrnt (Nlm_GraphiC a, Nlm_RectPtr r,
                              Nlm_Boolean align, Nlm_Boolean savePort)

{
  void            (*adjst) PROTO((Nlm_GraphiC, Nlm_RectPtr, Nlm_Boolean, Nlm_Boolean));
  Nlm_Boolean     cont;
  Nlm_GraphiC     p;
  Nlm_GphPrcsPtr  prntClassPtr;

  cont = TRUE;
  if (a != NULL) {
    p = Nlm_GetParent (a);
    prntClassPtr = Nlm_GetClassPtr (p);
    while (prntClassPtr != NULL && cont) {
      adjst = prntClassPtr->adjustPrnt;
      if (adjst != NULL) {
        adjst (a, r, align, savePort);
        cont = FALSE;
      } else {
        prntClassPtr = prntClassPtr->ancestor;
      }
    }
  }
}


extern void Nlm_DoSetTitle (Nlm_GraphiC a, Nlm_Int2 item,
                            Nlm_CharPtr title, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*stttl) PROTO((Nlm_GraphiC, Nlm_Int2, Nlm_CharPtr, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    stttl = classPtr->setTitle;
    if (stttl != NULL) {
      stttl (a, item, title, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern void Nlm_DoGetTitle (Nlm_GraphiC a, Nlm_Int2 item,
                            Nlm_CharPtr title, size_t maxsize)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  Nlm_Boolean     failed;
  void            (*gtttl) PROTO((Nlm_GraphiC, Nlm_Int2, Nlm_CharPtr, size_t));

  cont = TRUE;
  failed = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    gtttl = classPtr->getTitle;
    if (gtttl != NULL) {
      gtttl (a, item, title, maxsize);
      failed = FALSE;
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }

  if (failed  &&  maxsize > 0)
    title[0] = '\0';
}

extern void Nlm_DoSetValue (Nlm_GraphiC a, Nlm_Int2 value, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*stval) PROTO((Nlm_GraphiC, Nlm_Int2, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    stval = classPtr->setValue;
    if (stval != NULL) {
      stval (a, value, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern Nlm_Int2 Nlm_DoGetValue (Nlm_GraphiC a)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  Nlm_Int2        (*gtval) PROTO((Nlm_GraphiC));
  Nlm_Int2        value;

  cont = TRUE;
  value = 0;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    gtval = classPtr->getValue;
    if (gtval != NULL) {
      value = gtval (a);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
  return value;
}

extern void Nlm_DoSetStatus (Nlm_GraphiC a, Nlm_Int2 item,
                             Nlm_Boolean status, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*ststs) PROTO((Nlm_GraphiC, Nlm_Int2, Nlm_Boolean, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    ststs = classPtr->setStatus;
    if (ststs != NULL) {
      ststs (a, item, status, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern Nlm_Boolean Nlm_DoGetStatus (Nlm_GraphiC a, Nlm_Int2 item)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  Nlm_Boolean     (*gtsts) PROTO((Nlm_GraphiC, Nlm_Int2));
  Nlm_Boolean     val;

  cont = TRUE;
  val = FALSE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    gtsts = classPtr->getStatus;
    if (gtsts != NULL) {
      val = gtsts (a, item);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
  return val;
}

extern void Nlm_DoSetOffset (Nlm_GraphiC a, Nlm_Int2 horiz,
                             Nlm_Int2 vert,Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*stoff) PROTO((Nlm_GraphiC, Nlm_Int2, Nlm_Int2, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    stoff = classPtr->setOffset;
    if (stoff != NULL) {
      stoff (a, horiz, vert, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern void Nlm_DoGetOffset (Nlm_GraphiC a, Nlm_Int2Ptr horiz, Nlm_Int2Ptr vert)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  Nlm_Boolean     failed;
  void            (*gtoff) PROTO((Nlm_GraphiC, Nlm_Int2Ptr, Nlm_Int2Ptr));

  cont = TRUE;
  failed = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    gtoff = classPtr->getOffset;
    if (gtoff != NULL) {
      gtoff (a, horiz, vert);
      failed = FALSE;
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
  if (failed) {
    if (horiz != NULL) {
      *horiz = 0;
    }
    if (vert != NULL) {
      *vert = 0;
    }
  }
}

extern void Nlm_DoSetPosition (Nlm_GraphiC a, Nlm_RectPtr r, Nlm_Boolean savePort, Nlm_Boolean force)

{
  Nlm_GphPrcsPtr  classPtr;

  classPtr = Nlm_GetClassPtr( a );
  while (classPtr != NULL)
    {
      if ( classPtr->setPosition )
	{
	  (*classPtr->setPosition)(a, r, savePort, force);
	  break;
	}
      else
	{
	  classPtr = classPtr->ancestor;
	}
    }
}

extern void Nlm_DoGetPosition (Nlm_GraphiC a, Nlm_RectPtr r)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  Nlm_Boolean     failed;
  void            (*gtpos) PROTO((Nlm_GraphiC, Nlm_RectPtr));

  cont = TRUE;
  failed = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    gtpos = classPtr->getPosition;
    if (gtpos != NULL) {
      gtpos (a, r);
      failed = FALSE;
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
  if (failed) {
    if (r != NULL) {
      Nlm_LoadRect (r, 0, 0, 0, 0);
    }
  }
}

extern void Nlm_DoSetRange (Nlm_GraphiC a, Nlm_Int2 pgUp,
                            Nlm_Int2 pgDn, Nlm_Int2 max,
                            Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*rng) PROTO((Nlm_GraphiC, Nlm_Int2, Nlm_Int2, Nlm_Int2, Nlm_Boolean));


  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    rng = classPtr->setRange;
    if (rng != NULL) {
      rng (a, pgUp, pgDn, max, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern Nlm_GraphiC Nlm_DoGainFocus (Nlm_GraphiC a, Nlm_Char ch, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  Nlm_GraphiC     rsult;
  Nlm_GraphiC     (*gainFcs) PROTO((Nlm_GraphiC, Nlm_Char, Nlm_Boolean));

  cont = TRUE;
  rsult = NULL;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    gainFcs = classPtr->gainFocus;
    if (gainFcs != NULL) {
      rsult = gainFcs (a, ch, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
  return rsult;
}

extern void Nlm_DoLoseFocus (Nlm_GraphiC a, Nlm_GraphiC excpt, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*loseFcs) PROTO((Nlm_GraphiC, Nlm_GraphiC, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    loseFcs = classPtr->loseFocus;
    if (loseFcs != NULL) {
      loseFcs (a, excpt, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

extern void Nlm_DoSendChar (Nlm_GraphiC a, Nlm_Char ch, Nlm_Boolean savePort)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Boolean     cont;
  void            (*sendCh) PROTO((Nlm_GraphiC, Nlm_Char, Nlm_Boolean));

  cont = TRUE;
  classPtr = Nlm_GetClassPtr (a);
  while (classPtr != NULL && cont) {
    sendCh = classPtr->sendChar;
    if (sendCh != NULL) {
      sendCh (a, ch, savePort);
      cont = FALSE;
    } else {
      classPtr = classPtr->ancestor;
    }
  }
}

static Nlm_GraphiC Nlm_CheckThisLevel (Nlm_GraphiC a, Nlm_Char ch)

{
  Nlm_GraphiC  p;
  Nlm_GraphiC  q;

  q = NULL;
  p = a;
  while (p != NULL && q == NULL) {
    q = Nlm_DoGainFocus (p, ch, FALSE);
    p = Nlm_GetNext (p);
  }
  return q;
}

extern void Nlm_DoSendFocus (Nlm_GraphiC a, Nlm_Char ch)

{
  Nlm_GraphiC   cur;
  Nlm_GraphiC   nxt;
  Nlm_GraphiC   q;

  q = NULL;
  nxt = (Nlm_GraphiC) a;
  while (q == NULL && nxt != NULL) {
    cur = nxt;
    nxt = Nlm_GetParent (cur);
    if (nxt != NULL) {
      cur = Nlm_GetNext (cur);
    }
    q = Nlm_CheckThisLevel (cur, ch);
  }
}

extern void Nlm_DoAction (Nlm_GraphiC a)

{
  void             (*act) PROTO((Nlm_GraphiC));
  Nlm_GraphicData  gdata;

  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    act = gdata.action;
    if (act != NULL) {
      act (a);
    }
  }
}

extern void Nlm_Show (Nlm_Handle a)

{
  Nlm_DoShow ((Nlm_GraphiC) a, TRUE, TRUE);
}

extern void Nlm_Hide (Nlm_Handle a)

{
  Nlm_DoHide ((Nlm_GraphiC) a, TRUE, TRUE);
}

extern void Nlm_Enable (Nlm_Handle a)

{
  Nlm_DoEnable ((Nlm_GraphiC) a, TRUE, TRUE);
}

extern void Nlm_Disable (Nlm_Handle a)

{
  Nlm_DoDisable ((Nlm_GraphiC) a, TRUE, TRUE);
}

extern Nlm_Handle Nlm_Remove (Nlm_Handle a)

{
  return Nlm_DoRemove ((Nlm_GraphiC) a, TRUE);
}

extern void Nlm_Reset (Nlm_Handle a)

{
  Nlm_DoReset ((Nlm_GraphiC) a, TRUE);
}

extern void Nlm_Select (Nlm_Handle a)

{
  Nlm_DoSelect ((Nlm_GraphiC) a, TRUE);
}

extern void Nlm_SetTitle (Nlm_Handle a, Nlm_CharPtr title)

{
  Nlm_DoSetTitle ((Nlm_GraphiC) a, 0, title, TRUE);
}

extern void Nlm_GetTitle (Nlm_Handle a, Nlm_CharPtr title, size_t maxsize)

{
  Nlm_DoGetTitle ((Nlm_GraphiC) a, 0, title, maxsize);
}

extern void Nlm_SetValue (Nlm_Handle a, Nlm_Int2 value)

{
  Nlm_DoSetValue ((Nlm_GraphiC) a, value, TRUE);
}

extern Nlm_Int2 Nlm_GetValue (Nlm_Handle a)

{
  return (Nlm_DoGetValue ((Nlm_GraphiC) a));
}

extern void Nlm_SetStatus (Nlm_Handle a, Nlm_Boolean status)

{
  Nlm_DoSetStatus ((Nlm_GraphiC) a, 0, status, TRUE);
}

extern Nlm_Boolean Nlm_GetStatus (Nlm_Handle a)

{
  return (Nlm_DoGetStatus ((Nlm_GraphiC) a, 0));
}

extern void Nlm_SetOffset (Nlm_Handle a, Nlm_Int2 horiz, Nlm_Int2 vert)

{
  Nlm_DoSetOffset ((Nlm_GraphiC) a, horiz, vert, TRUE);
}

extern void Nlm_GetOffset (Nlm_Handle a, Nlm_Int2Ptr horiz, Nlm_Int2Ptr vert)

{
  Nlm_DoGetOffset ((Nlm_GraphiC) a, horiz, vert);
}

extern void Nlm_SetPosition (Nlm_Handle a, Nlm_RectPtr r)

{
  Nlm_DoSetPosition ((Nlm_GraphiC) a, r, TRUE, FALSE);
}

extern void Nlm_GetPosition (Nlm_Handle a, Nlm_RectPtr r)

{
  Nlm_DoGetPosition ((Nlm_GraphiC) a, r);
}

extern void Nlm_SetRange (Nlm_Handle a, Nlm_Int2 pgUp,
                          Nlm_Int2 pgDn, Nlm_Int2 max)

{
  Nlm_DoSetRange ((Nlm_GraphiC) a, pgUp, pgDn, max, TRUE);
}

extern void Nlm_AdjustPrnt (Nlm_Handle a, Nlm_RectPtr r, Nlm_Boolean align)

{
  Nlm_DoAdjustPrnt ((Nlm_GraphiC) a, r, align, TRUE);
}

extern void Nlm_SetItemTitle (Nlm_Handle a, Nlm_Int2 item, Nlm_CharPtr title)

{
  Nlm_DoSetTitle ((Nlm_GraphiC) a, item, title, TRUE);
}

extern void Nlm_GetItemTitle (Nlm_Handle a, Nlm_Int2 item, Nlm_CharPtr title, size_t maxsize)

{
  Nlm_DoGetTitle ((Nlm_GraphiC) a, item, title, maxsize);
}

extern void Nlm_SetItemStatus (Nlm_Handle a, Nlm_Int2 item, Nlm_Boolean status)

{
  Nlm_DoSetStatus ((Nlm_GraphiC) a, item, status, TRUE);
}

extern Nlm_Boolean Nlm_GetItemStatus (Nlm_Handle a, Nlm_Int2 item)

{
  return (Nlm_DoGetStatus ((Nlm_GraphiC) a, item));
}

extern Nlm_Int2 Nlm_GetNextItem (Nlm_Handle a, Nlm_Int2 prev)

{
  Nlm_GphPrcsPtr  classPtr;
  Nlm_Int2        (*cntitm) PROTO((Nlm_GraphiC));
  Nlm_Int2        count;
  Nlm_Boolean     (*gtsts) PROTO((Nlm_GraphiC, Nlm_Int2));
  Nlm_Boolean     found;
  Nlm_Int2        rsult;

  rsult = 0;
  if (a != NULL && prev >= 0) {
    classPtr = Nlm_GetClassPtr ((Nlm_GraphiC) a);
    if (classPtr != NULL) {
      cntitm = classPtr->countItems;
      gtsts = classPtr->getStatus;
      if (cntitm != NULL && gtsts != NULL) {
        count = cntitm ((Nlm_GraphiC) a);
        found = FALSE;
        while (prev < count && (! found)) {
          prev++;
          found = gtsts ((Nlm_GraphiC) a, prev);
        }
        if (found) {
          rsult = prev;
        }
      }
    }
  }
  return rsult;
}

extern void Nlm_ClearItemsInGroup (Nlm_GraphiC a, Nlm_GraphiC excpt, Nlm_Boolean savePort)

{
  Nlm_GraphiC  g;

  if (a != NULL) {
    g = Nlm_GetChild (a);
    while (g != NULL) {
      if (g != excpt) {
        Nlm_DoSetStatus (g, 0, FALSE, savePort);
      }
      g = Nlm_GetNext (g);
    }
  }
}

extern Nlm_Int2 Nlm_CountGroupItems (Nlm_GraphiC a)

{
  Nlm_GraphiC  g;
  Nlm_Int2     i;

  i = 0;
  if (a != NULL) {
    g = Nlm_GetChild (a);
    while (g != NULL) {
      i++;
      g = Nlm_GetNext (g);
    }
  }
  return i;
}

extern Nlm_GraphiC Nlm_LinkIn (Nlm_GraphiC a, Nlm_GraphiC prnt)

{
  Nlm_GraphiC  first;
  Nlm_GraphiC  g;
  Nlm_GphPtr   gp;
  Nlm_GraphiC  last;
  Nlm_GphPtr   lastp;
  Nlm_GraphiC  p;
  Nlm_GphPtr   prntp;

  if (prnt != NULL) {
    g = (Nlm_GraphiC) a;
    prntp = (Nlm_GphPtr) Nlm_HandLock (prnt);
    first = prntp->children;
    last = prntp->lastChild;
    Nlm_HandUnlock (prnt);
    if (last != NULL) {
      lastp = (Nlm_GphPtr) Nlm_HandLock (last);
      lastp->next = g;
      Nlm_HandUnlock (last);
      prntp = (Nlm_GphPtr) Nlm_HandLock (prnt);
      prntp->lastChild = g;
      Nlm_HandUnlock (prnt);
    } else if (first != NULL) {
      p = first;
      while (p != NULL) {
        last = p;
        p = Nlm_GetNext (p);
      }
      lastp = (Nlm_GphPtr) Nlm_HandLock (last);
      lastp->next = g;
      Nlm_HandUnlock (last);
      prntp = (Nlm_GphPtr) Nlm_HandLock (prnt);
      prntp->lastChild = g;
      Nlm_HandUnlock (prnt);
    } else {
      prntp = (Nlm_GphPtr) Nlm_HandLock (prnt);
      prntp->children = g;
      prntp->lastChild = g;
      Nlm_HandUnlock (prnt);
    }
    gp = (Nlm_GphPtr) Nlm_HandLock (g);
    gp->next = NULL;
    gp->parent = prnt;
    gp->children = NULL;
    gp->lastChild = NULL;
    Nlm_HandUnlock (g);
  }
  recentGraphic = NULL;
  recentBox = NULL;
  return a;
}

extern void Nlm_LoadAction (Nlm_GraphiC a, Nlm_ActnProc actnProc)

{
  Nlm_GraphicData  gdata;

  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    gdata.action = actnProc;
    Nlm_SetGraphicData (a, &gdata);
  }
}

extern void Nlm_LoadGraphicData (Nlm_GraphiC a, Nlm_GraphiC nxt,
                                 Nlm_GraphiC prnt, Nlm_GraphiC chld,
                                 Nlm_GraphiC lstchd, Nlm_GphPrcs PNTR classPtr,
                                 Nlm_ActnProc actnProc, Nlm_RectPtr r,
                                 Nlm_Boolean enabl, Nlm_Boolean vis,
                                 Nlm_VoidPtr dat, Nlm_FreeProc cln)
{
  Nlm_GphPtr  ptr;

  if (a != NULL) {
    ptr = (Nlm_GphPtr) Nlm_HandLock (a);
    ptr->next = nxt;
    ptr->parent = prnt;
    ptr->children = chld;
    ptr->lastChild = lstchd;
    ptr->classptr = classPtr;
    ptr->action = actnProc;
    if (r != NULL) {
      ptr->rect = *r;
    } else {
      Nlm_LoadRect (&(ptr->rect), 0, 0, 0, 0);
    }
    ptr->enabled = enabl;
    ptr->visible = vis;
    ptr->data = dat;
    ptr->cleanup = cln;
    ptr->realized = (Nlm_Boolean)(prnt ? Nlm_GetRealized(prnt) : FALSE);
    Nlm_HandUnlock (a);
    recentGraphic = NULL;
  }
}

extern void Nlm_LoadBoxData (Nlm_BoX a, Nlm_PoinT nxt,
                             Nlm_PoinT lmt, Nlm_PoinT rst,
                             Nlm_Int2 top, Nlm_Int2 ncol,
                             Nlm_Int2 xMrg, Nlm_Int2 yMrg,
                             Nlm_Int2 xSpc, Nlm_Int2 ySpc,
                             Nlm_Int2 wid, Nlm_Int2 hgt)

{
  Nlm_BoxPtr   bp;
  Nlm_BoxData  PNTR bptr;

  if (a != NULL) {
    bp = (Nlm_BoxPtr) Nlm_HandLock (a);
    bptr = &(bp->box);
    bptr->nextPoint = nxt;
    bptr->limitPoint = lmt;
    bptr->resetPoint = rst;
    bptr->topRow = top;
    bptr->nextCol = ncol;
    bptr->xMargin = xMrg;
    bptr->yMargin = yMrg;
    bptr->xSpacing = xSpc;
    bptr->ySpacing = ySpc;
    bptr->boxWidth = wid;
    bptr->boxHeight = hgt;
    Nlm_HandUnlock (a);
    recentBox = NULL;
  }
}

extern void Nlm_SetAction (Nlm_Handle a, Nlm_ActnProc actn)

{
  Nlm_GraphiC      g;
  Nlm_GraphicData  gdata;

  if (a != NULL) {
    g = (Nlm_GraphiC) a;
    if (g != NULL) {
      Nlm_GetGraphicData (g, &gdata);
      gdata.action = actn;
      Nlm_SetGraphicData (g, &gdata);
    }
  }
}

extern Nlm_GraphiC Nlm_CreateLink (Nlm_GraphiC prnt, Nlm_RectPtr r,
                                   Nlm_Int2 recordSize, Nlm_GphPrcs PNTR classPtr)

{
  Nlm_GraphiC  a;
  Nlm_GraphiC  rsult;

  rsult = NULL;
  a = NULL;
  if (prnt != NULL) {
    a = (Nlm_GraphiC) Nlm_HandNew (recordSize);
    if (a != NULL) {
      Nlm_LoadGraphicData (a, NULL, prnt, NULL, NULL, classPtr,
                           NULL, r, TRUE, FALSE, NULL, NULL);
      rsult = Nlm_DoLinkIn (a, prnt);
      if (rsult != NULL) {
        if (Nlm_nextIdNumber < 32767) {
          Nlm_nextIdNumber++;
        }
      } else {
        Nlm_HandFree (a);
      }
    }
  }
  return rsult;
}

extern void Nlm_RemoveLink (Nlm_GraphiC g)

{
  Nlm_GraphiC      prev;
  Nlm_GraphicData  gdata;
  Nlm_GraphiC      prnt;
  Nlm_GphPtr       prntp;

  if (g == NULL)
    return;

  /* exclude from the parent's children list */
  prnt  = Nlm_GetParent( g );
  prntp = (Nlm_GphPtr)Nlm_HandLock( prnt );
  prev  = Nlm_GetChild( prnt );
  if (g == prev)
    {
      prntp->children = Nlm_GetNext( g );
      if (prntp->lastChild == g)
        {
          ASSERT ( prntp->children == NULL );
          prntp->lastChild = NULL;
        }
    }
  else if (prev != NULL)
    {
      Nlm_GraphiC next = Nlm_GetNext( prev );
      while (next != NULL  &&  next != g)
        {
          prev = next;
          next = Nlm_GetNext( prev );
        }
      if (next == g)
        {
          Nlm_GphPtr prev_ptr;
          next = Nlm_GetNext( g );
          prev_ptr = (Nlm_GphPtr)Nlm_HandLock( prev );
          prev_ptr->next = next;
          Nlm_HandUnlock( prev );
          if (prntp->lastChild == g)
            {
              ASSERT ( prev != NULL  &&  prev_ptr->next == NULL );
              prntp->lastChild = prev;
            }
        }
    }
  Nlm_HandUnlock( prnt );

  /* call cleanup procedure;  then dispose the object */
  Nlm_GetGraphicData(g, &gdata);
  if (gdata.data != NULL  &&  gdata.cleanup != NULL)
    gdata.cleanup((Nlm_GraphiC)g, gdata.data);
  Nlm_HandFree( g );

  recentGraphic = NULL;
  recentBox = NULL;
}

extern Nlm_GraphiC Nlm_FindItem (Nlm_GraphiC a, Nlm_Int2 item)

{
  Nlm_GraphiC  g;

  g = NULL;
  if (a != NULL && item > 0) {
    g = Nlm_GetChild (a);
    while (g != NULL && item > 1) {
      item--;
      g = Nlm_GetNext (g);
    }
  }
  return g;
}

extern Nlm_Int2 Nlm_GetItemIndex (Nlm_GraphiC a)

{
  Nlm_GraphiC  g;
  Nlm_Int2     index;
  Nlm_Int2     rsult;

  rsult = 0;
  if (a != NULL) {
    index = 1;
    g = Nlm_GetChild (a);
    while (g != NULL && g != a) {
      index++;
      g = Nlm_GetNext (g);
    }
    if (g == a) {
      rsult = index;
    } else {
      rsult = 0;
    }
  }
  return rsult;
}

extern Nlm_Boolean Nlm_GetAllParentsEnabled (Nlm_GraphiC a)

{
  Nlm_GraphiC  p;

  p = NULL;
  if (a != NULL) {
    p = Nlm_GetParent (a);
    while (p != NULL && Nlm_GetEnabled (p)) {
      p = Nlm_GetParent (p);
    }
  }
  return (Nlm_Boolean) (p == NULL);
}

extern Nlm_Boolean Nlm_AllParentsEnabled (Nlm_Handle a)

{
  return Nlm_GetAllParentsEnabled ((Nlm_GraphiC) a);
}

extern Nlm_Boolean Nlm_AllParentsButWindowVisible (Nlm_GraphiC a)

{
  Nlm_GraphiC  p;

  p = NULL;
  if (a != NULL) {
    p = Nlm_GetParent (a);
    while (p != NULL && Nlm_GetVisible (p)) {
      p = Nlm_GetParent (p);
    }
  }
  return (Nlm_Boolean) (p == NULL || Nlm_GetParent (p) == NULL);
}

extern Nlm_Boolean Nlm_GetAllParentsVisible (Nlm_GraphiC a)

{
  Nlm_GraphiC  p;

  p = NULL;
  if (a != NULL) {
    p = Nlm_GetParent (a);
    while (p != NULL && Nlm_GetVisible (p)) {
      p = Nlm_GetParent (p);
    }
  }
  return (Nlm_Boolean) (p == NULL);
}

extern Nlm_Boolean Nlm_AllParentsVisible (Nlm_Handle a)

{
  return Nlm_GetAllParentsVisible ((Nlm_GraphiC) a);
}

extern void Nlm_SetNext (Nlm_GraphiC a, Nlm_GraphiC nxt)

{
  Nlm_GraphicData  gdata;

  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    gdata.next = nxt;
    Nlm_SetGraphicData (a, &gdata);
  }
}

extern Nlm_GraphiC Nlm_GetNext (Nlm_GraphiC a)

{
  Nlm_GraphicData  gdata;
  Nlm_GraphiC      rsult;

  rsult = NULL;
  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    rsult = gdata.next;
  }
  return rsult;
}

extern void Nlm_SetParent (Nlm_GraphiC a, Nlm_GraphiC prnt)

{
  Nlm_GraphicData  gdata;

  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    gdata.parent = prnt;
    Nlm_SetGraphicData (a, &gdata);
  }
}

extern Nlm_GraphiC Nlm_GetParent (Nlm_GraphiC a)

{
  Nlm_GraphicData  gdata;
  Nlm_GraphiC      rsult;

  rsult = NULL;
  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    rsult = gdata.parent;
  }
  return rsult;
}

extern Nlm_Handle Nlm_Parent (Nlm_Handle a)

{
  return Nlm_GetParent ((Nlm_GraphiC) a);
}

extern void Nlm_SetChild (Nlm_GraphiC a, Nlm_GraphiC chld)

{
  Nlm_GraphicData  gdata;

  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    gdata.children = chld;
    Nlm_SetGraphicData (a, &gdata);
  }
}

extern Nlm_GraphiC Nlm_GetChild (Nlm_GraphiC a)

{
  Nlm_GraphicData  gdata;
  Nlm_GraphiC      rsult;

  rsult = NULL;
  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    rsult = gdata.children;
  }
  return rsult;
}

extern void Nlm_SetRect (Nlm_GraphiC a, Nlm_RectPtr r)

{
  Nlm_GraphicData  gdata;

  if (a != NULL && r != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    gdata.rect = *r;
    Nlm_SetGraphicData (a, &gdata);
  }
}

extern void Nlm_GetRect (Nlm_GraphiC a, Nlm_RectPtr r)

{
  Nlm_GraphicData  gdata;

  if (a != NULL && r != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    *r = gdata.rect;
  }
}

extern void Nlm_ObjectRect (Nlm_Handle a, Nlm_RectPtr r)

{
  Nlm_GetRect ((Nlm_GraphiC) a, r);
}

extern void Nlm_InvalObject (Nlm_Handle a)

{
  Nlm_RecT  r;
  Nlm_WindoW   tempPort;                     /* M.I */

  if (a != NULL) {
    tempPort = Nlm_CurrentWindow ();         /* M.I */
    Nlm_GetRect ((Nlm_GraphiC) a, &r);
    Nlm_InsetRect (&r, -1, -1);
    Nlm_UseWindow( Nlm_ParentWindow( a ) );  /* M.I */
    Nlm_InvalRect (&r);
    Nlm_RestorePort (tempPort);              /* M.I */
  }
}

extern void Nlm_SetEnabled (Nlm_GraphiC a, Nlm_Boolean enabld)

{
  Nlm_GraphicData  gdata;

  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    gdata.enabled = enabld;
    Nlm_SetGraphicData (a, &gdata);
  }
}

extern Nlm_Boolean Nlm_GetEnabled (Nlm_GraphiC a)

{
  Nlm_GraphicData  gdata;
  Nlm_Boolean      rsult;

  rsult = FALSE;
  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    rsult = gdata.enabled;
  }
  return rsult;
}

extern Nlm_Boolean Nlm_Enabled (Nlm_Handle a)

{
  return Nlm_GetEnabled ((Nlm_GraphiC) a);
}

extern void Nlm_SetVisible (Nlm_GraphiC a, Nlm_Boolean visibl)

{
  Nlm_GraphicData  gdata;

  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    gdata.visible = visibl;
    Nlm_SetGraphicData (a, &gdata);
  }
}

extern Nlm_Boolean Nlm_GetVisible (Nlm_GraphiC a)

{
  Nlm_GraphicData  gdata;
  Nlm_Boolean      rsult;

  rsult = FALSE;
  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    rsult = gdata.visible;
  }
  return rsult;
}

extern Nlm_Boolean Nlm_Visible (Nlm_Handle a)

{
  return Nlm_GetVisible ((Nlm_GraphiC) a);
}

extern void Nlm_SetObjectExtra (Nlm_Handle a, Nlm_VoidPtr data, Nlm_FreeProc cleanup)

{
  Nlm_GraphicData  gdata;

  if (a != NULL) {
    Nlm_GetGraphicData ((Nlm_GraphiC) a, &gdata);
    gdata.data = data;
    gdata.cleanup = cleanup;
    Nlm_SetGraphicData ((Nlm_GraphiC) a, &gdata);
  }
}

extern Nlm_VoidPtr Nlm_GetObjectExtra (Nlm_Handle a)

{
  Nlm_GraphicData  gdata;

  if (a != NULL) {
    Nlm_GetGraphicData ((Nlm_GraphiC) a, &gdata);
    return gdata.data;
  } else {
    return NULL;
  }
}

extern void Nlm_SetGraphicData (Nlm_GraphiC a, Nlm_GraphicData PNTR gdata)

{
  Nlm_GphPtr  gp;

  if (a != NULL && gdata != NULL) {
    gp = (Nlm_GphPtr) Nlm_HandLock (a);
    *gp = *gdata;
    Nlm_HandUnlock (a);
    recentGraphic = a;
    recentGraphicData = *gdata;
  }
}

extern void Nlm_GetGraphicData (Nlm_GraphiC a, Nlm_GraphicData PNTR gdata)

{
  Nlm_GphPtr  gp;

  if (a != NULL && gdata != NULL) {
    if (a == recentGraphic && NLM_RISKY) {
      *gdata = recentGraphicData;
    } else {
      gp = (Nlm_GphPtr) Nlm_HandLock (a);
      *gdata = *gp;
      Nlm_HandUnlock (a);
      recentGraphic = a;
      recentGraphicData = *gdata;
    }
  }
}

extern void Nlm_SetBoxData (Nlm_BoX a, Nlm_BoxData PNTR bdata)

{
  Nlm_BoxPtr  bp;

  if (a != NULL && bdata != NULL) {
    bp = (Nlm_BoxPtr) Nlm_HandLock (a);
    bp->box = *bdata;
    Nlm_HandUnlock (a);
    recentBox = a;
    recentBoxData = *bdata;
  }
}

extern void Nlm_GetBoxData (Nlm_BoX a, Nlm_BoxData PNTR bdata)

{
  Nlm_BoxPtr  bp;

  if (a != NULL && bdata != NULL) {
    if (a == recentBox && NLM_RISKY) {
      *bdata = recentBoxData;
    } else {
      bp = (Nlm_BoxPtr) Nlm_HandLock (a);
      *bdata = bp->box;
      Nlm_HandUnlock (a);
      recentBox = a;
      recentBoxData = *bdata;
    }
  }
}

extern void Nlm_SetExtraData (Nlm_GraphiC a, Nlm_VoidPtr dptr,
                              Nlm_Int2 start, Nlm_Int2 extra)

{
  Nlm_BytePtr  dst;
  Nlm_GphPtr   gp;
  Nlm_BytePtr  src;

  if (a != NULL && dptr != NULL) {
    gp = (Nlm_GphPtr) Nlm_HandLock (a);
    dst = (Nlm_BytePtr) gp + start;
    src = (Nlm_BytePtr) dptr;
    while (extra > 0) {
      *dst = *src;
      dst++;
      src++;
      extra--;
    }
    Nlm_HandUnlock (a);
  }
}

extern void Nlm_GetExtraData (Nlm_GraphiC a, Nlm_VoidPtr dptr,
                              Nlm_Int2 start, Nlm_Int2 extra)

{
  Nlm_BytePtr  dst;
  Nlm_GphPtr   gp;
  Nlm_BytePtr  src;

  if (a != NULL && dptr != NULL) {
    gp = (Nlm_GphPtr) Nlm_HandLock (a);
    src = (Nlm_BytePtr) gp + start;
    dst = (Nlm_BytePtr) dptr;
    while (extra > 0) {
      *dst = *src;
      dst++;
      src++;
      extra--;
    }
    Nlm_HandUnlock (a);
  }
}

extern void Nlm_PointToolToPoinT (Nlm_PointTool src, Nlm_PointPtr dst)

{
  if (dst != NULL) {
#ifdef WIN_MAC
    dst->x = src.h;
    dst->y = src.v;
#endif
#ifdef WIN_MSWIN
    dst->x = (Nlm_Int2) src.x;
    dst->y = (Nlm_Int2) src.y;
#endif
#ifdef WIN_MOTIF
    dst->x = src.x;
    dst->y = src.y;
#endif
  }
}

extern void Nlm_PoinTToPointTool (Nlm_PoinT src, Nlm_PointTool PNTR dst)

{
  if (dst != NULL) {
#ifdef WIN_MAC
    dst->h = src.x;
    dst->v = src.y;
#endif
#ifdef WIN_MSWIN
    dst->x = src.x;
    dst->y = src.y;
#endif
#ifdef WIN_MOTIF
    dst->x = src.x;
    dst->y = src.y;
#endif
  }
}

extern void Nlm_RectToolToRecT (Nlm_RectTool PNTR src, Nlm_RectPtr dst)

{
  if (dst != NULL && src != NULL) {
#ifdef WIN_MAC
    dst->left = src->left;
    dst->top = src->top;
    dst->right = src->right;
    dst->bottom = src->bottom;
#endif
#ifdef WIN_MSWIN
    dst->left = (Nlm_Int2) src->left;
    dst->top = (Nlm_Int2) src->top;
    dst->right = (Nlm_Int2) src->right;
    dst->bottom = (Nlm_Int2) src->bottom;
#endif
#ifdef WIN_MOTIF
    dst->left = src->x;
    dst->top = src->y;
    dst->right = src->x + src->width;
    dst->bottom = src->y + src->height;
#endif
  }
}

extern void Nlm_RecTToRectTool (Nlm_RectPtr src, Nlm_RectTool PNTR dst)

{
  if (dst != NULL && src != NULL) {
#ifdef WIN_MAC
    dst->left = MIN (src->left, src->right);
    dst->top = MIN (src->top, src->bottom);
    dst->right = MAX (src->left, src->right);
    dst->bottom = MAX (src->top, src->bottom);
#endif
#ifdef WIN_MSWIN
    dst->left = MIN (src->left, src->right);
    dst->top = MIN (src->top, src->bottom);
    dst->right = MAX (src->left, src->right);
    dst->bottom = MAX (src->top, src->bottom);
#endif
#ifdef WIN_MOTIF
    dst->x = MIN (src->left, src->right);
    dst->y = MIN (src->top, src->bottom);
    dst->width = ABS (src->right - src->left);
    dst->height = ABS (src->bottom - src->top);
#endif
  }
}

extern void Nlm_LocalToGlobal (Nlm_PointPtr pt)

{
#ifdef WIN_MAC
  Nlm_PointTool  ptool;

  if (pt != NULL) {
    Nlm_PoinTToPointTool (*pt, &ptool);
    LocalToGlobal (&ptool);
    Nlm_PointToolToPoinT (ptool, pt);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_PointTool  ptool;

  if (pt != NULL && Nlm_currentHWnd != NULL) {
    Nlm_PoinTToPointTool (*pt, &ptool);
    ClientToScreen (Nlm_currentHWnd, &ptool);
    Nlm_PointToolToPoinT (ptool, pt);
  }
#endif
#ifdef WIN_MOTIF
#endif
}

extern void Nlm_GlobalToLocal (Nlm_PointPtr pt)

{
#ifdef WIN_MAC
  Nlm_PointTool  ptool;

  if (pt != NULL) {
    Nlm_PoinTToPointTool (*pt, &ptool);
    GlobalToLocal (&ptool);
    Nlm_PointToolToPoinT (ptool, pt);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_PointTool  ptool;

  if (pt != NULL && Nlm_currentHWnd != NULL) {
    Nlm_PoinTToPointTool (*pt, &ptool);
    ScreenToClient (Nlm_currentHWnd, &ptool);
    Nlm_PointToolToPoinT (ptool, pt);
  }
#endif
#ifdef WIN_MOTIF
#endif
}

#ifdef WIN_MSWIN
extern Nlm_Boolean Nlm_VibrantDisabled (void)

{
  if (Nlm_GetAppProperty("disable_vibrant") != NULL) {
    disabled_count++;
    return TRUE;
  }
  return FALSE;
}
#endif

#ifdef WIN_MAC
static PicHandle  picHdl = NULL;
#endif

#ifdef WIN_MSWIN
static HDC       picHDC = NULL;
static HDC       picWinHDC = NULL;
static HWND      picHWND = NULL;
static Nlm_RecT  picRect;
#endif

extern void Nlm_CopyWindowImage (void)

{
  Nlm_RecT    r;
  Nlm_WindoW  w;
#ifdef WIN_MSWIN
  Nlm_RecT    realr;
  HBITMAP     winBitMap, oldBitMap;
  HDC         hdcMemory;
  HDC         hdcWindow;
  int         fStatus = 0;
#endif

  w = Nlm_CurrentWindow ();
  if (w != NULL) {
    Nlm_ObjectRect (w, &r);
#ifdef WIN_MSWIN
    Nlm_SectRect ( &r, &Nlm_screenRect, &realr );
    hdcWindow = GetDC(NULL) ;
    if ( hdcWindow != NULL ) {
      fStatus = 1;
      hdcMemory = CreateCompatibleDC(hdcWindow);
      if ( hdcMemory != NULL ){
        fStatus = 2;
        winBitMap = CreateCompatibleBitmap ( hdcWindow,
          realr.right - realr.left, realr.bottom - realr.top );
        if ( winBitMap != NULL ){
          fStatus = 3;
          oldBitMap = SelectObject ( hdcMemory, winBitMap );
          BitBlt ( hdcMemory, 0, 0, realr.right - realr.left,
                   realr.bottom - realr.top,  hdcWindow,
                   realr.left, realr.top, SRCCOPY );
          SelectObject ( hdcMemory, oldBitMap );
          if ( OpenClipboard (Nlm_currentHWnd) ){
            EmptyClipboard ();
            if ( SetClipboardData (CF_BITMAP, winBitMap) != NULL ){
              fStatus = 2;
            }
            CloseClipboard ();
          }
        }
      }
    }
    switch ( fStatus ) {
      case 3:
        DeleteObject ( (HGDIOBJ)winBitMap );
      case 2:
        DeleteDC (hdcMemory);
      case 1:
        ReleaseDC (NULL,hdcWindow);
    }
#else
    Nlm_OffsetRect (&r, -r.left, -r.top);
    Nlm_InvalRect (&r);
    w = Nlm_StartPicture (&r);
    Nlm_Update ();
    Nlm_EndPicture (w);
#endif
  }
}

#ifdef WIN_MSWIN
extern HDC Nlm_GetPicWinHDC ( void )

{
  return picWinHDC;
}
#endif


extern Nlm_WindoW Nlm_StartPicture (Nlm_RectPtr r)
{
  Nlm_WindoW    w;
#ifdef WIN_MAC
  Nlm_RectTool  rtool;
#endif

  w = Nlm_CurrentWindow ();
  if (r == NULL)
    return w;

#ifdef WIN_MAC
  Nlm_RecTToRectTool (r, &rtool);
  picHdl = OpenPicture (&rtool);
#endif
#ifdef WIN_MSWIN
  picRect = *r;
  picHWND = Nlm_currentHWnd;
  picWinHDC = GetWindowDC(picHWND);
  picHDC    = CreateMetaFile (NULL);
  if (picHDC != NULL)
    {
      SetWindowOrgEx(picHDC, picRect.left, picRect.top, NULL);
      SetWindowExtEx(picHDC, picRect.right - picRect.left,
                     picRect.bottom - picRect.top, NULL);
      Nlm_SetPort((HWND) NULL, picHDC);
    }
#endif
#ifdef WIN_MOTIF
#endif

  return w;
}


extern void Nlm_EndPicture (Nlm_WindoW w)

{
#ifdef WIN_MAC
  long    len;
  PicPtr  picPtr;

  ClosePicture ();
#if TARGET_API_MAC_CARBON
  { OSStatus status = ClearCurrentScrap(); }
#else
  ZeroScrap ();
#endif
  Nlm_textScrapFull = FALSE;
  if (picHdl != NULL) {
    len = GetHandleSize ((Handle) picHdl);
    picPtr = (PicPtr) Nlm_HandLock (picHdl);
#if TARGET_API_MAC_CARBON
    {
      OSStatus status;
      ScrapRef scrap;
      ScrapFlavorFlags flags = 0;
      status = GetCurrentScrap(&scrap);
      status = PutScrapFlavor(scrap, kScrapFlavorTypePicture, flags, len, picPtr);
    }
#else
    { OSErr err = PutScrap (len, 'PICT', (Ptr) picPtr); }
#endif
    Nlm_HandUnlock (picHdl);
    KillPicture (picHdl);
  }
  picHdl = NULL;
#endif
#ifdef WIN_MSWIN
  GLOBALHANDLE    hGMem = NULL;
  HMETAFILE       hmf = NULL;
  LPMETAFILEPICT  lpMFP;
  int             fileSend = 0;

  if (picWinHDC != NULL) {
    ReleaseDC(picHWND,picWinHDC);
    picWinHDC = NULL;
  }
  if (picHDC != NULL) {
    hmf = CloseMetaFile (picHDC);
    if ( hmf != NULL ) {
      hGMem = GlobalAlloc (GHND, (DWORD) sizeof (METAFILEPICT));
      if ( hGMem != NULL ) {
        lpMFP = (LPMETAFILEPICT) GlobalLock (hGMem);
        if ( lpMFP != NULL ){
          lpMFP->mm = MM_ANISOTROPIC;
          lpMFP->xExt = (picRect.right - picRect.left) * 25;
          lpMFP->yExt = (picRect.bottom - picRect.top) * 25;
          lpMFP->hMF = hmf;
          GlobalUnlock (hGMem);
          if ( OpenClipboard (picHWND) ){
            EmptyClipboard ();
            if ( SetClipboardData (CF_METAFILEPICT, hGMem) != NULL ){
              fileSend = 1;
            }
            CloseClipboard ();
          }
        }
      }
    }
  }
  if ( !fileSend ){
    if ( hGMem != NULL ) GlobalFree ( hGMem );
    if ( hmf != NULL ) DeleteMetaFile ( hmf );
  }
  picHDC = NULL;
  picHWND = NULL;
#endif
#ifdef WIN_MOTIF
#endif
  if (w != NULL) {
    Nlm_UseWindow (w);
  }
}

#ifdef WIN_MOTIF
static Window Nlm_GetAnyX11Window (void)
{
  Window         wX11;
  Nlm_WindoW     wVibrant;
  Nlm_ShellTool  wShell;

  wX11   = (Window) 0;
  wShell = (Nlm_ShellTool) 0;
  wVibrant = Nlm_desktopWindow;
  while ( wVibrant != NULL ){
    wShell = Nlm_GetWindowShell ( wVibrant );
    if ( wShell )
      break;
    wVibrant = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC)wVibrant);
  }
  if ( wShell ){
    wX11 = XtWindow ( wShell );
  }
  return wX11;
}
#endif

extern void Nlm_StringToClipboard (Nlm_CharPtr str)

{
#ifdef WIN_MAC
  long    len;
  OSErr err;

# if TARGET_API_MAC_CARBON
  { OSStatus status = ClearCurrentScrap(); }
# else
  ZeroScrap ();
# endif
  Nlm_textScrapFull = FALSE;
  len = (long) Nlm_StringLen (str);
  if (len > 0) {
# if TARGET_API_MAC_CARBON
    OSStatus status;
    ScrapRef scrap;
    ScrapFlavorFlags flags = 0;
    status = GetCurrentScrap(&scrap);
    status = PutScrapFlavor(scrap, kScrapFlavorTypeText, flags, len, str);
# else
    PutScrap (len, 'TEXT', (Ptr) str);
# endif
    err = TEFromScrap ();
  }
#endif
#ifdef WIN_MSWIN
  HGLOBAL  hMem;
  DWORD    len;
  DWORD    n;
  LPSTR    pMem;
  LPSTR    pStr;

  if (Nlm_currentHWnd != NULL) {
    if (OpenClipboard (Nlm_currentHWnd)) {
      EmptyClipboard ();
      len = (long) Nlm_StringLen (str);
      if (len > 0) {
        hMem = GlobalAlloc (GHND, (DWORD) len + 1);
        if (hMem != NULL) {
          pMem = GlobalLock (hMem);
          if (pMem != NULL) {
            pStr = (LPSTR) str;
            for (n = 0; n < len; n++) {
              *pMem++ = *pStr++;
            }
          }
          GlobalUnlock (hMem);
          SetClipboardData (CF_TEXT, hMem);
        }
      }
      CloseClipboard ();
    }
  }
#endif
#ifdef WIN_MOTIF
  XmString  clip_label;
  long      item_id;
  Window    w;
  int       status;
  char      nchar = 0;

  if (Nlm_currentXDisplay != NULL  &&
      (w = Nlm_GetAnyX11Window()) != (Window)0)  {
    clip_label = XmStringCreateSimple ( "vibrant_to_clibboard" );
    do {
      status = XmClipboardStartCopy ( Nlm_currentXDisplay, w,
                                      clip_label, CurrentTime, NULL, NULL,
                                      &item_id );
    } while ( status == ClipboardLocked );
    XmStringFree ( clip_label );
    do {
      if ( str == NULL ){
        status = XmClipboardCopy ( Nlm_currentXDisplay, w,
                                   item_id, "STRING", &nchar,
                                   (long)1,
                                   0, NULL );
      } else {
        status = XmClipboardCopy ( Nlm_currentXDisplay, w,
                                   item_id, "STRING", str,
                                   (long)Nlm_StringLen(str),
                                   0, NULL );
      }
    } while ( status == ClipboardLocked );
    do {
      status = XmClipboardEndCopy ( Nlm_currentXDisplay, w,
                                    item_id);
    } while ( status == ClipboardLocked );
  }
#endif
}

extern Nlm_Boolean Nlm_ClipboardHasString (void)

{
#ifdef WIN_MAC
  long  len;
  OSErr err;

  err = TEFromScrap();
  len = TEGetScrapLength();
  return (Nlm_Boolean) (len > 0);
#endif

#ifdef WIN_MSWIN
  return (Nlm_Boolean) (IsClipboardFormatAvailable (CF_TEXT));
#endif

#ifdef WIN_MOTIF
  Nlm_Boolean   result = FALSE;
  Nlm_Char      str[2];
  unsigned long len;
  unsigned long rlen;
  Window        w;
  int           status;

  if (Nlm_currentXDisplay != NULL  &&
      (w = Nlm_GetAnyX11Window()) != (Window)0)  {
    do {
      status = XmClipboardLock ( Nlm_currentXDisplay, w );
    } while ( status == ClipboardLocked );
    status = XmClipboardInquireLength ( Nlm_currentXDisplay, w,
                                        "STRING", &len );
    if ( (status != ClipboardSuccess) || (len==0) ) {
      XmClipboardUnlock ( Nlm_currentXDisplay, w, FALSE );
      return result;
    }
    str[0] = 0;
    XmClipboardRetrieve ( Nlm_currentXDisplay, w,
                          "STRING", str, 1, &rlen, NULL );
    XmClipboardUnlock ( Nlm_currentXDisplay, w, FALSE );
    if ( str[0] != 0 ) result = TRUE;
  }
  return result;
#endif
}

extern Nlm_CharPtr Nlm_ClipboardToString (void)

{
#ifdef WIN_MAC
  long         len;
  Nlm_CharPtr  str;
  Handle hdl;

# if TARGET_API_MAC_CARBON
  { OSErr err = TEFromScrap(); }
# endif
  hdl = TEScrapHandle();
  len = TEGetScrapLength();
  if (len > 0 && len < 32766) {
    str = (Nlm_CharPtr) Nlm_MemNew ((size_t) len + 2);
    if (str != NULL) {
      str [len] = '\0';
      if (hdl != NULL) {
        Nlm_CharPtr  ptr;
        ptr = (Nlm_CharPtr) *hdl;
        Nlm_MemCpy (str, ptr, (size_t) len);
      }
    }
  }
  return str;
#endif
#ifdef WIN_MSWIN
  HANDLE       hClip;
  LPSTR        pStr;
  Nlm_CharPtr  str;

  str = NULL;
  if (Nlm_currentHWnd != NULL && IsClipboardFormatAvailable (CF_TEXT)) {
    if (OpenClipboard (Nlm_currentHWnd)) {
      hClip = GetClipboardData (CF_TEXT);
      if (hClip != NULL) {
        pStr = GlobalLock (hClip);
        if (pStr != NULL) {
          str = Nlm_StringSave (pStr);
        }
        GlobalUnlock (hClip);
      }
      CloseClipboard ();
    }
  }
  return str;
#endif
#ifdef WIN_MOTIF
  Nlm_CharPtr   str;
  unsigned long len;
  unsigned long rlen;
  Window        w;
  int           status;

  if (Nlm_currentXDisplay != NULL  &&
      (w=Nlm_GetAnyX11Window()) != (Window)0)  {
    do {
      status = XmClipboardLock ( Nlm_currentXDisplay, w );
    } while ( status == ClipboardLocked );
    status = XmClipboardInquireLength ( Nlm_currentXDisplay, w,
                                        "STRING", &len );
    if ( status != ClipboardSuccess ) {
      XmClipboardUnlock ( Nlm_currentXDisplay, w, FALSE );
      return NULL;
    }
    str = (Nlm_CharPtr)MemNew(len+1);
    XmClipboardRetrieve ( Nlm_currentXDisplay, w,
                          "STRING", str, len+1, &rlen, NULL );
    XmClipboardUnlock ( Nlm_currentXDisplay, w, FALSE );
    return str;
  }
  return NULL;
#endif
}

#ifdef WIN_MSWIN
static void Nlm_SetupPrinterDeviceContext(HDC prHDC)
{
  if ( !prHDC )
    return;

  Nlm_SetPort((HWND)0, prHDC);
  SetMapMode(prHDC, MM_ANISOTROPIC);

  {{
    HWND screenHwnd = GetDesktopWindow();
    HDC  screenDC   = GetDC( screenHwnd );

    /* NOTE:  for video devices, the logical inch IS NOT == physical inch! */
    SetWindowExtEx(prHDC, GetDeviceCaps(screenDC, LOGPIXELSX),
                   GetDeviceCaps(screenDC, LOGPIXELSY), NULL);
    SetViewportExtEx(prHDC, GetDeviceCaps(prHDC, LOGPIXELSX),
                     GetDeviceCaps(prHDC, LOGPIXELSY), NULL);

    ReleaseDC(screenHwnd, screenDC);
  }}
}
#endif

#ifdef WIN_MAC
# if TARGET_API_MAC_CARBON
extern Nlm_WindoW Nlm_StartPrinting (void)
{
  OSStatus status;
  Boolean accepted;
  
  status = PMBegin();
  if (status != noErr) return NULL;
  status = PMNewPageFormat(&pageFormat);
  if (status != noErr || pageFormat == kPMNoPageFormat) return NULL;
  status = PMDefaultPageFormat(pageFormat);
  if (status != noErr) return NULL;
  status = PMNewPrintSettings(&printSettings);
  if (status != noErr || printSettings == kPMNoPrintSettings) return NULL;
  status = PMDefaultPrintSettings(printSettings);
  if (status != noErr) return NULL;
  status = PMPrintDialog(printSettings, pageFormat, &accepted);
  if (!accepted) status = kPMCancel;
  if (status != noErr) return NULL;
  
  status = PMBeginDocument(printSettings, pageFormat, &thePrintingPort);
  if ((status != noErr) || (thePrintingPort == kPMNoReference)) return NULL;
  
  return Nlm_CurrentWindow();
}

# else  /* not TARGET_API_MAC_CARBON */

extern Nlm_WindoW Nlm_StartPrinting (void)
{
  Nlm_WindoW  w = Nlm_CurrentWindow ();

  PrOpen ();
  if (prHdl == NULL) {
    prHdl = (THPrint) Nlm_HandNew (sizeof (TPrint));
    if (prHdl != NULL) {
      PrintDefault (prHdl);
      prerr = PrError ();
      if (prerr != noErr) {
        Nlm_Message (MSG_ERROR, "PrintDefault error %d", (int) prerr);
      }
      if (! PrStlDialog (prHdl)) {
        prHdl = Nlm_HandFree (prHdl);
        w = NULL;
      }
    } else {
      Nlm_Message (MSG_ERROR, "Unable to create print handle");
    }
  }
  if (prHdl != NULL) {
    if (PrJobDialog (prHdl)) {
      prPort = PrOpenDoc (prHdl, NULL, NULL);
      Nlm_SetPort ((GrafPtr) prPort);
      prerr = PrError ();
      if (prerr != noErr) {
        Nlm_Message (MSG_ERROR, "PrOpenDoc error %d", prerr);
      } else {
        Nlm_nowPrinting = TRUE;
      }
    } else {
      w = NULL;
    }
  } else {
    w = NULL;
  }
  return w;
}
# endif  /* not TARGET_API_MAC_CARBON */
#endif  /* WIN_MAC */

#ifdef WIN_MSWIN
extern Nlm_WindoW Nlm_StartPrinting (void)
{
  Nlm_WindoW  w = Nlm_CurrentWindow ();
  DWORD       commdlgerr;
  DOCINFO     di;
  char        docName [256];
  
  abortPrint = FALSE;
  memset (&pd, 0, sizeof (PRINTDLG));
  pd.lStructSize = sizeof (PRINTDLG);
  pd.hwndOwner = Nlm_currentHWnd;
  pd.Flags = PD_RETURNDC;
  if (PrintDlg (&pd) != 0) {
    hPr = pd.hDC;
    if (hPr != NULL) {
      Nlm_SetupPrinterDeviceContext (hPr);
      Nlm_StringCpy (docName, "Vibrant");
      Nlm_MemSet(&di, '\0', sizeof(DOCINFO));
      di.cbSize = sizeof (DOCINFO);
      di.lpszDocName = (LPCSTR) docName;
      prerr = StartDoc (hPr, &di);
      if (prerr < 1) {
        abortPrint = TRUE;
        Nlm_Message (MSG_ERROR, "StartDoc error %d", prerr);
        w = NULL;
      } else {
        Nlm_nowPrinting = TRUE;
      }
    } else {
      commdlgerr = CommDlgExtendedError ();
      Nlm_Message (MSG_ERROR, "Unable to create print context, error %lu",
                   (unsigned long) commdlgerr);
      w = NULL;
    }
  } else {
    w = NULL;
  }
  return w;
}
#endif  /* WIN_MSWIN */

#ifdef WIN_MOTIF
extern Nlm_WindoW Nlm_StartPrinting (void)
{
  return NULL;
}
#endif


#ifdef WIN_MAC
# if TARGET_API_MAC_CARBON

extern void Nlm_EndPrinting (Nlm_WindoW w)
{
  OSStatus status;

  Nlm_nowPrinting = FALSE;
  if (w != NULL) {
    (void)PMEndDocument(thePrintingPort);
    status = PMError();
    if (status != noErr) {
      Nlm_Message (MSG_ERROR, "PMEndDocument error %d", status);
    }
// This call is not supported under Carbon, need to figure out
// how Apple wants us to deal with this ...  churchill 12/28/99
#if 0
    PrPicFile (prHdl, 0L, 0L, 0L, &prStat);
    prerr = PrError ();
    if (prerr != noErr) {
      Nlm_Message (MSG_ERROR, "PrPicFile error %d", prerr);
    }
    prPort = NULL;
    Nlm_UseWindow (w);
#endif
  }
  if (pageFormat != kPMNoPageFormat) {
    (void)PMDisposePageFormat(pageFormat);
    pageFormat = kPMNoPageFormat;
  }
  if (printSettings != kPMNoPrintSettings) {
    (void)PMDisposePrintSettings(printSettings);
    printSettings = kPMNoPrintSettings;
  }
  (void)PMEnd();
}

# else  // not TARGET_API_MAC_CARBON

extern void Nlm_EndPrinting (Nlm_WindoW w)
{
  TPrStatus  prStat;

  Nlm_nowPrinting = FALSE;
  if (w != NULL) {
    PrCloseDoc (prPort);
    prerr = PrError ();
    if (prerr != noErr) {
      Nlm_Message (MSG_ERROR, "PrCloseDoc error %d", prerr);
    }
// This call is not supported under Carbon, need to figure out
// how Apple wants us to deal with this ...  churchill 12/28/99
    PrPicFile (prHdl, 0L, 0L, 0L, &prStat);
    prerr = PrError ();
    if (prerr != noErr) {
      Nlm_Message (MSG_ERROR, "PrPicFile error %d", prerr);
    }
    prPort = NULL;
    Nlm_UseWindow (w);
  }
  PrClose ();
}

# endif  /* not TARGET_API_MAC_CARBON */
#endif


#ifdef WIN_MSWIN
extern void Nlm_EndPrinting (Nlm_WindoW w)
{
  Nlm_nowPrinting = FALSE;
  if (w != NULL) {
    if (hPr != NULL) {
      if (! abortPrint) {
        prerr = EndDoc (hPr);
        if (prerr < 0) {
          Nlm_Message (MSG_ERROR, "EndDoc error %d", prerr);
        }
      }
      DeleteDC (hPr);
      if (pd.hDevMode != NULL) {
        GlobalFree (pd.hDevMode);
      }
      if (pd.hDevNames != NULL) {
        GlobalFree (pd.hDevNames);
      }
    }
    Nlm_UseWindow (w);
  }
}
#endif

#ifdef WIN_MOTIF
extern void Nlm_EndPrinting (Nlm_WindoW w)
{
  Nlm_nowPrinting = FALSE;
}
#endif


#ifdef WIN_MAC

extern Nlm_Boolean Nlm_ClipPrintingRect(const Nlm_RecT PNTR rpt)
{
	Nlm_RecT r;
	
	if (rpt == NULL || 
#if TARGET_API_MAC_CARBON
		thePrintingPort == kPMNoReference)
#else
		prHdl == NULL || *prHdl == NULL)
#endif
	{
		return FALSE;
	}
	
	r = *rpt;
	Nlm_InsetRect(&r, -1, -1);
	
	Nlm_ClipRect( &r );
	return TRUE;
}

#endif  /* WIN_MAC */


#ifdef WIN_MSWIN

extern Nlm_Boolean Nlm_ClipPrintingRect(const Nlm_RecT PNTR rpt)
{
  Nlm_RecT r;

  POINT    pt;

  if (rpt == NULL)
    return FALSE;

  r = *rpt;
  Nlm_InsetRect(&r, -1, -1);

  if (hPr == NULL)
    return FALSE;

  pt.x = r.left;
  pt.y = r.bottom;
  LPtoDP(hPr, &pt, 1);
  r.left   = (Nlm_Int2)pt.x;
  r.bottom = (Nlm_Int2)pt.y;

  pt.x = r.right;
  pt.y = r.top;
  LPtoDP(hPr, &pt, 1);
  r.right = (Nlm_Int2)pt.x;
  r.top   = (Nlm_Int2)pt.y;

  Nlm_ClipRect( &r );
  return TRUE;
}

#endif

#if !defined(WIN_MAC) && !defined(WIN_MSWIN)

extern Nlm_Boolean Nlm_ClipPrintingRect(const Nlm_RecT PNTR rpt)
{
  return FALSE;
}

#endif


#ifdef WIN_MAC
# if TARGET_API_MAC_CARBON

extern Nlm_Boolean Nlm_PrintingRect(Nlm_RectPtr rpt)
{
	OSStatus status;
	PMRect pmRect;
	SInt16 left, top, right, bottom;
	
	if (rpt == NULL)
		return FALSE;
	
	Nlm_LoadRect (rpt, 0, 0, 0, 0);
	
	status = PMGetAdjustedPageRect(pageFormat, &pmRect);
	if (status != noErr) return FALSE;
	
	left   = pmRect.left;
	top    = pmRect.top;
	right  = pmRect.right;
	bottom = pmRect.bottom;
	
	Nlm_LoadRect (rpt, left, top, right, bottom);
	
	return TRUE;
}

# else  /* not TARGET_API_MAC_CARBON */

extern Nlm_Boolean Nlm_PrintingRect(Nlm_RectPtr rpt)
{
  if (rpt == NULL)
    return FALSE;

  Nlm_LoadRect (rpt, 0, 0, 0, 0);
  
  if (prHdl == NULL) {
    return FALSE;
  } else {
    TPPrint       prPtr;
    Nlm_RectTool  rtool;
    prPtr = (TPPrint)Nlm_HandLock( prHdl );
    if (prPtr == NULL)
      return FALSE;
  
    rtool = prPtr->prInfo.rPage;
    Nlm_RectToolToRecT(&rtool, rpt);
    Nlm_InsetRect(rpt, 10, 10);
    Nlm_HandUnlock (prHdl);
  }
  
  return TRUE;
}

# endif  /* not TARGET_API_MAC_CARBON */
#endif  /* WIN_MAC */

#ifdef WIN_MSWIN

extern Nlm_Boolean Nlm_PrintingRect(Nlm_RectPtr rpt)
{
  POINT         physPageSize;
  POINT         pixelsPerInch;
  Nlm_PoinT     pt;
  
  if (rpt == NULL)
    return FALSE;

  Nlm_LoadRect (rpt, 0, 0, 0, 0);
  
  if (hPr == NULL)
    return FALSE;

  physPageSize.x = GetDeviceCaps(hPr, HORZRES);
  physPageSize.y = GetDeviceCaps(hPr, VERTRES);
  DPtoLP(hPr, &physPageSize, 1);
  Nlm_PointToolToPoinT(physPageSize, &pt);
  rpt->right  = pt.x;
  rpt->bottom = pt.y;
  pixelsPerInch.x = GetDeviceCaps(hPr, LOGPIXELSX);
  pixelsPerInch.y = GetDeviceCaps(hPr, LOGPIXELSY);
  DPtoLP(hPr, &pixelsPerInch, 1);
  Nlm_InsetRect(rpt, (Nlm_Int2)(pixelsPerInch.x / 4),
                     (Nlm_Int2)(pixelsPerInch.y / 4));
  
  return TRUE;
}

#endif  /* WIN_MSWIN */

#ifdef WIN_MOTIF

extern Nlm_Boolean Nlm_PrintingRect(Nlm_RectPtr rpt)
{
  if (rpt != NULL)
    Nlm_LoadRect (rpt, 0, 0, 0, 0);
  
  return FALSE;
}

#endif  /* WIN_MOTIF */


#ifdef WIN_MAC
# if TARGET_API_MAC_CARBON

extern Nlm_Boolean Nlm_StartPage (void)
{
  OSStatus status;
  
  status = PMBeginPage(thePrintingPort, NULL);
  if (status != noErr) return false;  // ??
  
  return true;
}

# else

extern Nlm_Boolean Nlm_StartPage (void)
{
  Nlm_Boolean  rsult;

  rsult = TRUE;
  if (prPort != NULL) {
    PrOpenPage (prPort, NULL);
    prerr = PrError ();
    if (prerr != noErr) {
      Nlm_Message (MSG_ERROR, "PrOpenPage error %d", prerr);
      rsult = FALSE;
    }
  } else {
    rsult = FALSE;
  }
  return rsult;
}

# endif
#endif


#ifdef WIN_MSWIN

extern Nlm_Boolean Nlm_StartPage (void)
{
  Nlm_Boolean  rsult;

  rsult = TRUE;
  if (hPr != NULL) {
    Nlm_SetupPrinterDeviceContext (hPr);
    prerr = StartPage (hPr);
    if (prerr < 0) {
      Nlm_Message (MSG_ERROR, "StartPage error %d", prerr);
      rsult = FALSE;
    }
    Nlm_SetPort ((HWND) NULL, (HDC) hPr);
  }
  return rsult;
}

#endif


#ifdef WIN_MOTIF

extern Nlm_Boolean Nlm_StartPage (void)
{
  return FALSE;
}

#endif


#ifdef WIN_MAC
# if TARGET_API_MAC_CARBON

extern Nlm_Boolean Nlm_EndPage (void)
{
  OSStatus status;
  
  status = PMEndPage(thePrintingPort);
  if (status != noErr) return false;  // ??
  
  return true;
}

# else

extern Nlm_Boolean Nlm_EndPage (void)
{
  Nlm_Boolean  rsult;

  rsult = TRUE;
  if (prPort != NULL) {
    PrClosePage (prPort);
    prerr = PrError ();
    if (prerr != noErr) {
      Nlm_Message (MSG_ERROR, "PrClosePage error %d", prerr);
      rsult = FALSE;
    }
  } else {
    rsult = FALSE;
  }
  return rsult;
}

# endif
#endif


#ifdef WIN_MSWIN
extern Nlm_Boolean Nlm_EndPage (void)
{
  Nlm_Boolean  rsult;

  rsult = TRUE;
  if (hPr != NULL) {
    prerr = EndPage (hPr);
    if (prerr < 0) {
      abortPrint = TRUE;
      Nlm_Message (MSG_ERROR, "EndPage error %d", prerr);
      rsult = FALSE;
    }
  } else {
    rsult = FALSE;
  }
  return rsult;
}
#endif

#ifdef WIN_MOTIF
extern Nlm_Boolean Nlm_EndPage (void)
{
  return FALSE;
}
#endif

#ifdef WIN_MAC
static OSType Nlm_GetOSType (Nlm_CharPtr str, OSType dfault)

{
  OSType  rsult;

  rsult = dfault;
  if (str != NULL && str [0] != '\0') {
    rsult = *(OSType*) str;
  }
  return rsult;
}

// 2001-03-22:  Joshua Juran
// Working directory records are gone in Carbon.
// However, so is Standard File.  The code which calls Nav Services instead of SF
// doesn't need this function, so we don't define it.
# if !TARGET_API_MAC_CARBON
static void Nlm_GetFilePath (Nlm_Int2 currentVol, Nlm_CharPtr path, size_t maxsize)

{
  WDPBRec     block;
  Nlm_Char    directory [256];
  Nlm_Int4    dirID;
  OSErr       err;
  CInfoPBRec  params;
  Nlm_Char    temp [256];
  Nlm_Int2    vRefNum;

  block.ioNamePtr = NULL;
  block.ioVRefNum = currentVol;
  block.ioWDIndex = 0;
  block.ioWDProcID = 0;
  PBGetWDInfoSync(&block);
  dirID = block.ioWDDirID;
  vRefNum = block.ioWDVRefNum;
  temp [0] = '\0';
  params.dirInfo.ioNamePtr = (StringPtr) directory;
  params.dirInfo.ioDrParID = dirID;
  do {
    params.dirInfo.ioVRefNum = vRefNum;
    params.dirInfo.ioFDirIndex = -1;
    params.dirInfo.ioDrDirID = params.dirInfo.ioDrParID;
    err = PBGetCatInfo (&params, FALSE);
    Nlm_PtoCstr (directory);
    Nlm_StrngCat (directory, ":", sizeof (directory));
    Nlm_StrngCat (directory, temp, sizeof (directory));
    Nlm_StrngCpy (temp, directory, sizeof (temp));
  } while (params.dirInfo.ioDrDirID != fsRtDirID);
  Nlm_StringNCpy_0 (path, temp, maxsize);
}
# endif  /* !TARGET_API_MAC_CARBON */
#endif

#ifdef WIN_MOTIF
static void Nlm_CreateFileDialogShell (void)
{
  Cardinal  n = 0;
  Arg       wargs[8];

  if (Nlm_fileDialogShell != NULL)
    return;

  XtSetArg (wargs[n], XmNdefaultFontList, Nlm_XfontList); n++;
  XtSetArg (wargs[n], XmNdeleteResponse,  XmDO_NOTHING);  n++;
  Nlm_fileDialogShell = XtAppCreateShell((String) NULL, (String) "Vibrant",
                                         applicationShellWidgetClass,
                                         Nlm_currentXDisplay, wargs, n);
}


static void Nlm_FileCancelCallback (Widget fs, XtPointer client_data, XtPointer call_data)

{
  fileBoxUp = FALSE;
  fileBoxRsult = FALSE;
}

static void Nlm_FileOkCallback (Widget fs, XtPointer client_data, XtPointer call_data)

{
  XmFileSelectionBoxCallbackStruct  *cbs;
  char                              *filename;

  cbs = (XmFileSelectionBoxCallbackStruct *) call_data;
  fileBoxUp = FALSE;
  if (! XmStringGetLtoR (cbs->value, XmSTRING_DEFAULT_CHARSET, &filename)) {
    return;
  }
  if (! *filename) {
    XtFree (filename);
    fileBoxRsult = FALSE;
    return;
  }
  Nlm_StringNCpy_0(filePath, filename, sizeof(filePath));
  XtFree (filename);
  fileBoxRsult = TRUE;
}

static void Nlm_FileNoMatchCallback (Widget fs, XtPointer client_data, XtPointer call_data)

{
  fileBoxUp = FALSE;
  fileBoxRsult = FALSE;
}

static void Nlm_FileMapCallback (Widget fs, XtPointer client_data, XtPointer call_data)

{
  Position   x, y;
  Dimension  w, h;

  XtVaGetValues (fs, XmNwidth, &w, XmNheight, &h, NULL);
  x = (Position) (Nlm_screenRect.right - w) / (Position) 2;
  y = (Position) (Nlm_screenRect.bottom - h) / (Position) 3;
  XtVaSetValues (fs, XmNx, x, XmNy, y, NULL);
}
#endif

#ifdef WIN_MAC
/* AppleEvent handlers modified from Roger Sayle's RasMol code */
static void ConvertFilename ( FSSpec *fss, Nlm_CharPtr filename )

{
  register char *src;
  register char *dst;
  register int i;
  char buffer [256];
  
  Str255 dirname;
  DirInfo dinfo;
  
  src = buffer;
  dinfo.ioDrParID = fss->parID;
  dinfo.ioNamePtr = dirname;
  do {
    dinfo.ioVRefNum = fss->vRefNum;
    dinfo.ioFDirIndex = -1;
    dinfo.ioDrDirID = dinfo.ioDrParID;
    PBGetCatInfo ((CInfoPBPtr) &dinfo, 0);
    
    *src++ = ':';
    for ( i=dirname[0]; i; i-- )
      *src++ = dirname [i];
  } while ( dinfo.ioDrDirID != 2 );
  
  /* Reverse the file path! */
  dst = filename;
  while ( src != buffer )
    *dst++ = *(--src);
  for( i = 1; i <= fss->name [0]; i++ )
    *dst++ = fss->name [i];
  *dst = '\0';
}
#endif

#ifdef OS_MAC
#ifdef PROC_PPC
static pascal void MyNavEventProc (NavEventCallbackMessage callBackSelector,
                                   NavCBRecPtr callBackParms,
                                   NavCallBackUserData callBackUD)

{
  WindowPtr window = (WindowPtr) callBackParms->eventData.eventDataParms.event->message;
  switch (callBackSelector) {
    case kNavCBEvent:
      switch (callBackParms->eventData.eventDataParms.event->what) {
        case updateEvt:
          /*
          MyHandleUpdateEvent (window, (EventRecord*) callBackParms->eventData.eventDataParms.event);
          */
          break;
      }
      break;
  }
}

static pascal Boolean MyNavTextFilterProc (AEDesc* theItem, void* info,
                                           NavCallBackUserData callBackUD,
                                           NavFilterModes filterMode)

{
    OSErr theErr = noErr;
    Boolean display = true;
    NavFileOrFolderInfo* theInfo = (NavFileOrFolderInfo*)info;

    if (theItem->descriptorType == typeFSS)
        if (!theInfo->isFolder)
            if (theInfo->fileAndFolder.fileInfo.finderInfo.fdType
                != 'TEXT')
                display = false;
    return display;
}

static pascal Boolean MyNavFilterProc (AEDesc* theItem, void* info,
                                       NavCallBackUserData callBackUD,
                                       NavFilterModes filterMode)

{
  return true;
}
#endif
#endif

#ifdef OS_MAC
#ifdef PROC_PPC
static Nlm_Boolean Nlm_NavServGetInputFileName (Nlm_CharPtr fileName, size_t maxsize,
                                                Nlm_CharPtr extType, Nlm_CharPtr macType)

{
    NavDialogOptions    dialogOptions;
    AEDesc              defaultLocation;
    NavEventUPP         eventProc = NewNavEventUPP(MyNavEventProc);
    NavObjectFilterUPP  filterProc;
    OSErr               anErr = noErr;
    FSSpec              fss;
	char                filename [256];
	Nlm_Boolean         rsult = FALSE;

    if (StringCmp (macType, "TEXT") == 0) {
        filterProc = NewNavObjectFilterUPP(MyNavTextFilterProc);
    } else {
        filterProc = NewNavObjectFilterUPP(MyNavFilterProc);
    }

    //  Specify default options for dialog box
    anErr = NavGetDefaultDialogOptions(&dialogOptions);
    if (anErr == noErr)
    {
        //  Adjust the options to fit our needs
        //  Set this option
        dialogOptions.dialogOptionFlags |= kNavSelectDefaultLocation;
        //  Clear this one
        dialogOptions.dialogOptionFlags ^= kNavAllowPreviews;

        anErr = AECreateDesc(typeFSS, &fss,
                             sizeof(fss),
                             &defaultLocation );
        if (anErr == noErr)
        {
            // Get 'open' resource. A nil handle being returned is OK,
            // this simply means no automatic file filtering.
            NavTypeListHandle typeList = (NavTypeListHandle)GetResource(
                                        'open', 128);
            NavReplyRecord reply;

            anErr = NavChooseFile (&defaultLocation, &reply, &dialogOptions,
                                   eventProc, nil, filterProc,
                                   typeList, 0);
            if (anErr == noErr && reply.validRecord)
            {
                //  Deal with multiple file selection
                long    count;

                anErr = AECountItems(&(reply.selection), &count);
                if (count > 1) {
                  count = 1;  /* force to 1 */
                }
                if (anErr == noErr)
                {
                    long index;

                    for (index = 1; index <= count; index++)
                    {
                        AEKeyword   theKeyword;
                        DescType    actualType;
                        Size        actualSize;
                        FSSpec      documentFSSpec;

                        anErr = AEGetNthPtr(&(reply.selection), index,
                                            typeFSS, &theKeyword,
                                            &actualType,&documentFSSpec,
                                            sizeof(documentFSSpec),
                                            &actualSize);
                        if (anErr == noErr)
                        {
                            ConvertFilename (&documentFSSpec, filename);
                            Nlm_StringNCpy_0 (fileName, filename, maxsize);
                            rsult = TRUE;
                        }
                    }
                }
                //  Dispose of NavReplyRecord, resources, descriptors
                anErr = NavDisposeReply(&reply);
            }
            if (typeList != NULL)
            {
                ReleaseResource( (Handle)typeList);
            }
            (void) AEDisposeDesc(&defaultLocation);
        }
    }
    DisposeNavEventUPP(eventProc);
    DisposeNavObjectFilterUPP(filterProc);
    return rsult;
}
#endif
#endif

extern Nlm_Boolean Nlm_GetInputFileName (Nlm_CharPtr fileName, size_t maxsize,
                                         Nlm_CharPtr extType, Nlm_CharPtr macType)

{
#ifdef WIN_MAC
# if TARGET_API_MAC_CARBON
    return Nlm_NavServGetInputFileName (fileName, maxsize, extType, macType);
# else
  Nlm_Char       currentFileName [64];
  Nlm_Char       currentPath [256];
  SFTypeList     fTypeList;
  Nlm_Int2       i;
  Nlm_Int2       lengthTypes;
  Nlm_Int2       numTypes;
  Nlm_PointTool  ptool;
  Nlm_RecT       r;
  SFReply        reply;
  Nlm_Boolean    rsult;
  Nlm_RectTool   rtool;
  GrafPtr        tempPort;
  PenState       state;
  Nlm_Char       str [5];
  Nlm_PoinT      where;

  if (Nlm_usesMacNavServices) {
    return Nlm_NavServGetInputFileName (fileName, maxsize, extType, macType);
  }
  where.x = 90;
  where.y = 100;
  lengthTypes = sizeof (fileTypes);
  for (i = 0; i < lengthTypes; i++) {
    fileTypes [i] = '\0';
  }
  if (macType != NULL && macType [0] != '\0') {
    Nlm_StringNCpy (fileTypes, macType, 6); /* remains StringNCpy, not _0 */
    fileTypes [4] = '\0';
  }
  if (fileTypes [0] != '\0') {
    numTypes = 0;
    i = 0;
    while (numTypes < 4 && i <= lengthTypes) {
      Nlm_StrngSeg (str, fileTypes, i, 4, sizeof (str));
      fTypeList [numTypes] = Nlm_GetOSType (str, '    ');
      numTypes++;
      i += 4;
    }
  } else {
    numTypes = -1;
  }
  GetPenState (&state);
  GetPort (&tempPort);
  Nlm_PoinTToPointTool (where, &ptool);
  SFGetFile (ptool, NULL, NULL, numTypes, fTypeList, NULL, &reply);
  SetPort (tempPort);
  SetPenState (&state);
  Nlm_GetRect ((Nlm_GraphiC) Nlm_desktopWindow, &r);
  Nlm_RecTToRectTool (&r, &rtool);
  ClipRect (&rtool);
  if (reply.good != 0 && fileName != NULL  &&  maxsize > 0) {
    Nlm_StringNCpy_0(currentFileName, (Nlm_CharPtr) &(reply.fName),
                     sizeof(currentFileName));
    Nlm_PtoCstr (currentFileName);
    currentPath[0] = '\0';
    Nlm_GetFilePath (reply.vRefNum, currentPath, sizeof (currentPath));
    Nlm_StringNCat(currentPath, currentFileName,
                   sizeof(currentPath) - Nlm_StringLen(currentPath) - 1);
    Nlm_StringNCpy_0(fileName, currentPath, maxsize);
    rsult = TRUE;
  } else {
    rsult = FALSE;
  }
  Nlm_Update ();
  return rsult;
# endif
#endif	/* WIN_MAC */

#ifdef WIN_MSWIN
  char  szDirName [256];
  char  szFile [256];
  char  szFileTitle [256];
  UINT  i;
  UINT  cbString;
  char  chReplace;
  char  szFilter [256];
  char  *lastSlash;

  GetModuleFileName (Nlm_currentHInst, szDirName, sizeof (szDirName));
  lastSlash = Nlm_StringRChr (szDirName, DIRDELIMCHR);
  if (lastSlash != NULL) {
    lastSlash [1] = '\0';
  }
  szFile [0] = '\0';
  if (extType != NULL && extType [0] != '\0') {
    Nlm_StringCpy (szFilter, "Filtered Files (*");
    if (extType [0] != '.') {
      Nlm_StringCat (szFilter, ".");
    }
    Nlm_StringNCat (szFilter, extType, 5);
    Nlm_StringCat (szFilter, ")|*");
    if (extType [0] != '.') {
      Nlm_StringCat (szFilter, ".");
    }
    Nlm_StringNCat (szFilter, extType, 5);
    Nlm_StringCat (szFilter, "|");
  } else {
    Nlm_StringCpy (szFilter, "All Files (*.*)|*.*|");
  }
  cbString = (UINT) Nlm_StringLen (szFilter);
  chReplace = szFilter [cbString - 1];
  for (i = 0; szFilter [i] != '\0'; i++) {
    if (szFilter [i] == chReplace) {
      szFilter [i] = '\0';
    }
  }
  memset (&ofn, 0, sizeof (OPENFILENAME));
  ofn.lStructSize = sizeof (OPENFILENAME);
  ofn.hwndOwner = Nlm_currentHWnd;
  ofn.lpstrFilter = szFilter;
  ofn.nFilterIndex = 1;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof (szFile);
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = sizeof (szFileTitle);
  ofn.lpstrInitialDir = szDirName;
  ofn.Flags =
    OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
  if (GetOpenFileName (&ofn) && fileName != NULL  &&  maxsize > 0) {
    Nlm_StringNCpy_0(fileName, ofn.lpstrFile, maxsize);
    AnsiToOemBuff (fileName, fileName, maxsize);
    return TRUE;
  } else {
    return FALSE;
  }
#endif

#ifdef WIN_MOTIF
  XmString  dirmask;
  char      *lastSlash;
  char      str [256];
  char      *text;
  Widget    txt;

  Nlm_CreateFileDialogShell ();
  if (Nlm_fileDialogShell != NULL && fileName != NULL) {
    if (fileDialog == NULL) {
      Cardinal  n = 0;
      Arg       wargs[4];
      XtSetArg (wargs[n], XmNcolormap, Nlm_VibrantDefaultColormap());  n++;
      XtSetArg (wargs[n], XmNvisual,   Nlm_VibrantDefaultVisual  ());  n++;
      XtSetArg (wargs[n], XmNdepth,    Nlm_VibrantDefaultDepth   ());  n++;
      fileDialog = XmCreateFileSelectionDialog (Nlm_fileDialogShell,
                                                (String) "file_selection", wargs, n);
      XtAddCallback (fileDialog, XmNcancelCallback, Nlm_FileCancelCallback, NULL);
      XtAddCallback (fileDialog, XmNokCallback, Nlm_FileOkCallback, NULL);
      XtAddCallback (fileDialog, XmNnoMatchCallback, Nlm_FileNoMatchCallback, NULL);
      XtAddCallback (fileDialog, XmNmapCallback, Nlm_FileMapCallback, NULL);
      XtVaSetValues (fileDialog, XmNdefaultPosition, FALSE,
                     XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL, NULL);
    }
    if (fileDialog != NULL) {
      txt = XmFileSelectionBoxGetChild (fileDialog, XmDIALOG_FILTER_TEXT);
      text = XmTextGetString (txt);
      Nlm_StringNCpy_0(str, text, sizeof(str));
      lastSlash = Nlm_StringRChr (str, DIRDELIMCHR);
      if (lastSlash != NULL) {
        lastSlash [1] = '\0';
      } else {
        lastSlash = &(str [Nlm_StringLen (str) - 1]);
      }
      if (extType != NULL && extType [0] != '\0') {
        Nlm_StringCat (str, "*");
        if (extType [0] != '.') {
          Nlm_StringCat (str, ".");
        }
        Nlm_StringNCat (str, extType, 5);
        dirmask = XmStringCreateLtoR (str, XmSTRING_DEFAULT_CHARSET);
        XmFileSelectionDoSearch (fileDialog, dirmask);
        XmStringFree (dirmask);
        if (lastSlash != NULL) {
          lastSlash [1] = '\0';
        }
      }
      XtFree (text);
      txt = XmFileSelectionBoxGetChild (fileDialog, XmDIALOG_TEXT);
      XmTextSetString (txt, str);
      XtVaSetValues (fileDialog, XmNmustMatch, TRUE, NULL);
      fileBoxUp = TRUE;
      fileBoxRsult = FALSE;
      XtManageChild (fileDialog);
      XSync (Nlm_currentXDisplay, FALSE);
      while (fileBoxUp) {
        XEvent event;
        XPeekEvent(Nlm_currentXDisplay, &event);
        Nlm_ProcessAnEvent ();
      }
      if (fileBoxRsult) {
        Nlm_StringNCpy_0(fileName, filePath, maxsize);
      }
      XtUnmanageChild (fileDialog);
      XSync (Nlm_currentXDisplay, FALSE);
    }
  }
  return fileBoxRsult;
#endif
}

#ifdef WIN_MSWIN
static void Nlm_CopyDefaultName (Nlm_CharPtr dst, Nlm_CharPtr src)
{
  Nlm_Char  ch;
  Nlm_Int2  i;
  Nlm_Int2  j;
  Nlm_Int2  k;

  if (dst == NULL  ||  src == NULL)
    return;

  i = 0;
  j = 0;
  k = 0;
  ch = src[i];
  while (k < 8 && ch != '\0' && (IS_ALPHANUM(ch) || ch == '_'))
    {
      dst [j] = ch;
      i++;  j++;  k++;
      ch = src [i];
    }
  while (ch != '\0' && ch != '.')
    {
      i++;
      ch = src [i];
    }
  if (ch == '.')
    {
      dst[j] = ch;
      i++;
      j++;
      ch = src[i];
      k = 0;
      while (k < 3 && ch != '\0' && (IS_ALPHANUM(ch) || ch == '_'))
        {
          dst [j] = ch;
          i++; j++; k++;
          ch = src[i];
        }
      if (k == 0)
        j--;
    }

  dst[j] = '\0';
}
#endif

#ifdef OS_MAC
#ifdef PROC_PPC
static Nlm_Boolean Nlm_NavServGetOutputFileName (Nlm_CharPtr fileName, size_t maxsize,
                                                 Nlm_CharPtr dfault)

{
    OSErr               anErr = noErr;
    NavReplyRecord      reply;
    NavDialogOptions    dialogOptions;
    AEDesc              defaultLocation;
    FSSpec              fss;
    OSType              fileTypeToSave = 'TEXT';
    OSType              creatorType;
    NavEventUPP         eventProc = NewNavEventUPP (MyNavEventProc);
	char                filename [256];
	Nlm_Boolean         rsult = FALSE;

    anErr = NavGetDefaultDialogOptions (&dialogOptions);
    if (anErr == noErr)
    {
        //  Adjust the options to fit our needs
        //  Set this option
        dialogOptions.dialogOptionFlags |= kNavNoTypePopup;
        //  Clear this one
        dialogOptions.dialogOptionFlags ^= kNavAllowStationery;

        anErr = AECreateDesc(typeFSS, &fss,
                             sizeof(fss),
                             &defaultLocation );
        if (anErr == noErr) {

            //  One way to get the name for the file to be saved.
            Nlm_StringNCpy_0 ((Nlm_CharPtr) dialogOptions.savedFileName, dfault, 255);
            Nlm_CtoPstr ((Nlm_CharPtr) dialogOptions.savedFileName);

            Nlm_MemSet ((void *) &creatorType, '?', 4);
            anErr = NavPutFile( &defaultLocation, &reply, &dialogOptions, eventProc,
                                fileTypeToSave, creatorType, 0 );
            if (anErr == noErr && reply.validRecord)
            {
                AEKeyword   theKeyword;
                DescType    actualType;
                Size        actualSize;
                FSSpec      documentFSSpec;

                anErr = AEGetNthPtr(&(reply.selection), 1, typeFSS,
                                    &theKeyword, &actualType,
                                    &documentFSSpec, sizeof(documentFSSpec),
                                    &actualSize );
                if (anErr == noErr)
                {
                    if (reply.replacing)
                    {
                        // Make sure you save a temporary file
                        // so you can check for problems before replacing
                        // an existing file. Once the save is confirmed,
                        // swap the files and delete the original.
                        ConvertFilename (&documentFSSpec, filename);
                        Nlm_StringNCpy_0 (fileName, filename, maxsize);
                        rsult = TRUE;
                    }
                    else
                    {
                        ConvertFilename (&documentFSSpec, filename);
                        Nlm_StringNCpy_0 (fileName, filename, maxsize);
                        rsult = TRUE;
                    }

                    if ( anErr == noErr)
                    {
                        // DO NOT call NavCompleteSave() to complete
                        /* anErr = NavCompleteSave(&reply,
                                                kNavTranslateInPlace); */
                    }
                }
                (void) NavDisposeReply(&reply);
            }
        }
        (void) AEDisposeDesc(&defaultLocation);
        DisposeNavEventUPP(eventProc);
    }
    return rsult;
}
#endif
#endif

extern Nlm_Boolean Nlm_GetOutputFileName (Nlm_CharPtr fileName, size_t maxsize,
                                          Nlm_CharPtr dfault)

{
#ifdef WIN_MAC
# if TARGET_API_MAC_CARBON
    return Nlm_NavServGetOutputFileName (fileName, maxsize, dfault);
# else  /* not TARGET_API_MAC_CARBON */
  Nlm_Char       currentFileName [64];
  Nlm_Char       currentPath [256];
  unsigned char  original [256];
  unsigned char  promptStr [256];
  Nlm_PointTool  ptool;
  Nlm_RecT       r;
  SFReply        reply;
  Nlm_Boolean    rsult;
  Nlm_RectTool   rtool;
  GrafPtr        tempPort;
  PenState       state;
  Nlm_PoinT      where;

  if (Nlm_usesMacNavServices) {
    return Nlm_NavServGetOutputFileName (fileName, maxsize, dfault);
  }

  where.x = 90;
  where.y = 100;
  GetPenState (&state);
  GetPort (&tempPort);
  Nlm_PoinTToPointTool (where, &ptool);
  Nlm_StringNCpy_0((Nlm_CharPtr)original, dfault, sizeof(original));
  Nlm_CtoPstr ((Nlm_CharPtr) original);
  Nlm_StringNCpy_0((Nlm_CharPtr)promptStr, "Save File As:", sizeof(promptStr));
  Nlm_CtoPstr ((Nlm_CharPtr) promptStr);
  SFPutFile (ptool, promptStr, original, NULL, &reply);
  SetPort (tempPort);
  SetPenState (&state);
  Nlm_GetRect ((Nlm_GraphiC) Nlm_desktopWindow, &r);
  Nlm_RecTToRectTool (&r, &rtool);
  ClipRect (&rtool);
  if (reply.good != 0 && fileName != NULL  &&  maxsize > 0) {
    Nlm_StringNCpy_0(currentFileName, (Nlm_CharPtr) &(reply.fName),
                     sizeof (currentFileName));
    Nlm_PtoCstr (currentFileName);
    currentPath[0] = '\0';
    Nlm_GetFilePath (reply.vRefNum, currentPath, sizeof (currentPath));
    Nlm_StringNCat(currentPath, currentFileName,
                   sizeof(currentPath) - Nlm_StringLen(currentPath) - 1);
    Nlm_StringNCpy_0(fileName, currentPath, maxsize);
    rsult = TRUE;
  } else {
    rsult = FALSE;
  }
  Nlm_Update ();
  return rsult;
# endif  /* not TARGET_API_MAC_CARBON */
#endif /* ifdef WIN_MAC */

#ifdef WIN_MSWIN
  FILE  *f;
  char  szDirName [256];
  char  szFile [256];
  char  szFileTitle [256];
  UINT  i;
  UINT  cbString;
  char  chReplace;
  char  szFilter [256];
  char  *lastSlash;

  GetModuleFileName (Nlm_currentHInst, szDirName, sizeof (szDirName));
  lastSlash = Nlm_StringRChr (szDirName, DIRDELIMCHR);
  if (lastSlash != NULL) {
    lastSlash [1] = '\0';
  }
  szFile [0] = '\0';
  Nlm_CopyDefaultName ((Nlm_CharPtr) szFile, dfault);
  Nlm_StringCpy (szFilter, "All Files (*.*)|*.*|");
  cbString = (UINT) Nlm_StringLen (szFilter);
  chReplace = szFilter [cbString - 1];
  for (i = 0; szFilter [i] != '\0'; i++) {
    if (szFilter [i] == chReplace) {
      szFilter [i] = '\0';
    }
  }
  memset (&ofn, 0, sizeof (OPENFILENAME));
  ofn.lStructSize = sizeof (OPENFILENAME);
  ofn.hwndOwner = Nlm_currentHWnd;
  ofn.lpstrFilter = szFilter;
  ofn.lpstrFile = szFile;
  ofn.nMaxFile = sizeof (szFile);
  ofn.lpstrFileTitle = szFileTitle;
  ofn.nMaxFileTitle = sizeof (szFileTitle);
  ofn.lpstrInitialDir = szDirName;
  /*
  ofn.Flags = OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
  */
  /* OFN_OVERWRITEPROMPT causes crashes for some unknown reason */
  ofn.Flags = OFN_SHOWHELP | OFN_NOCHANGEDIR;
  if (GetSaveFileName (&ofn) && fileName != NULL) {
    Nlm_StringNCpy_0(fileName, ofn.lpstrFile, maxsize);
    AnsiToOemBuff (fileName, fileName, maxsize);
    f = Nlm_FileOpen (fileName, "r");
    if (f != NULL) {
      Nlm_FileClose (f);
      if (Nlm_Message (MSG_YN, "Replace existing file?") == ANS_NO) {
        return FALSE;
      }
    }
    return TRUE;
  } else {
    return FALSE;
  }
#endif

#ifdef WIN_MOTIF
  char   *lastSlash;
  char    str [256];
  char    *text;
  Widget  txt;
  char    *lastDot;
  XmString dirmask;

  Nlm_CreateFileDialogShell ();
  if (Nlm_fileDialogShell != NULL && fileName != NULL) {
    if (fileDialog == NULL) {
      Cardinal  n = 0;
      Arg       wargs[4];
      XtSetArg (wargs[n], XmNcolormap, Nlm_VibrantDefaultColormap());  n++;
      XtSetArg (wargs[n], XmNvisual,   Nlm_VibrantDefaultVisual  ());  n++;
      XtSetArg (wargs[n], XmNdepth,    Nlm_VibrantDefaultDepth   ());  n++;
      fileDialog = XmCreateFileSelectionDialog (Nlm_fileDialogShell,
                                                (String) "file_selection", wargs, n);
      XtAddCallback (fileDialog, XmNcancelCallback, Nlm_FileCancelCallback, NULL);
      XtAddCallback (fileDialog, XmNokCallback, Nlm_FileOkCallback, NULL);
      XtAddCallback (fileDialog, XmNnoMatchCallback, Nlm_FileNoMatchCallback, NULL);
      XtAddCallback (fileDialog, XmNmapCallback, Nlm_FileMapCallback, NULL);
      XtVaSetValues (fileDialog, XmNdefaultPosition, FALSE,
                     XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL, NULL);
    }
    if (fileDialog != NULL) {
      txt = XmFileSelectionBoxGetChild (fileDialog, XmDIALOG_FILTER_TEXT);
      text = XmTextGetString (txt);
      Nlm_StringNCpy_0(str, text, sizeof(str));
      lastSlash = Nlm_StringRChr (str, DIRDELIMCHR);
      if (lastSlash != NULL) {
        lastSlash [1] = '\0';
      }

      /* V.L. */
      lastDot=dfault ? Nlm_StringRChr(dfault,'.') : (Nlm_CharPtr)NULL;
      Nlm_StringNCat(str, "*", sizeof(str) - Nlm_StringLen(str) - 1);
      if (lastDot)
        Nlm_StringNCat(str, lastDot, sizeof(str) - Nlm_StringLen(str) - 1);
      dirmask = XmStringCreateLtoR (str, XmSTRING_DEFAULT_CHARSET);
      XmFileSelectionDoSearch (fileDialog, dirmask);
      XmStringFree (dirmask);
      if (lastSlash != NULL) {
        lastSlash [1] = '\0';
      }

      XtFree (text);
      if (dfault != NULL) {
        Nlm_StringNCat(str, dfault, sizeof(str) - Nlm_StringLen(str) - 1);
      }
      txt = XmFileSelectionBoxGetChild (fileDialog, XmDIALOG_TEXT);
      XmTextSetString (txt, str);
      XtVaSetValues (fileDialog, XmNmustMatch, FALSE, NULL);
      fileBoxUp = TRUE;
      fileBoxRsult = FALSE;
      XtManageChild (fileDialog);
      while (fileBoxUp) {
        XEvent event;
        XPeekEvent(Nlm_currentXDisplay, &event);
        Nlm_ProcessAnEvent ();
      }
      if (fileBoxRsult) {
        Nlm_StringNCpy_0(fileName, filePath, maxsize);
      }
      XtUnmanageChild (fileDialog);
    }
  }
  return fileBoxRsult;
#endif
}

extern Nlm_Boolean Nlm_LaunchAppEx (Nlm_CharPtr fileName, Nlm_VoidPtr serialNumPtr, Nlm_CharPtr sig);
extern Nlm_Boolean Nlm_LaunchAppEx (Nlm_CharPtr fileName, Nlm_VoidPtr serialNumPtr, Nlm_CharPtr sig)

{
#ifdef WIN_MAC
  OSErr                   err;
  FSSpec                  fsspec;
  long                    gval;
  LaunchParamBlockRec     myLaunchParams;
  Nlm_Boolean             okay = FALSE;
  unsigned char           pathname [256];
  ProcessSerialNumberPtr  psnp;
  Nlm_CharPtr             ptr;
  Nlm_Boolean             rsult;
  OSType                  theSignature;
  ProcessSerialNumber     psn;
  ProcessInfoRec          pir;
  Nlm_Boolean             found;
  DTPBRec                 dtpb;
  short                   ioDTRefNum;
  long                    dirID;
  short                   vRefNum;
  long                    ioAPPLParID;

  rsult = FALSE;
  if (Gestalt (gestaltSystemVersion, &gval) == noErr && (short) gval >= 7 * 256) {
    if (fileName != NULL && fileName [0] != '\0') {
      if (Nlm_StringChr (fileName, DIRDELIMCHR) != NULL) {
        Nlm_StringNCpy_0((Nlm_CharPtr)pathname, fileName, sizeof(pathname));
      } else {
        Nlm_ProgramPath ((Nlm_CharPtr) pathname, sizeof (pathname));
        ptr = Nlm_StringRChr ((Nlm_CharPtr) pathname, DIRDELIMCHR);
        if (ptr != NULL) {
          *ptr = '\0';
        }
        Nlm_FileBuildPath ((Nlm_CharPtr) pathname, NULL, fileName);
      }
      Nlm_CtoPstr ((Nlm_CharPtr) pathname);
      err = FSMakeFSSpec (0, 0, pathname, &(fsspec));
      okay = (Nlm_Boolean) (err == noErr);
    } else if (sig != NULL && sig [0] != '\0') {
      theSignature = *(OSType*) sig;
      psn.highLongOfPSN = 0;
      psn.lowLongOfPSN = kNoProcess;
      found = FALSE;
      while (GetNextProcess (&psn) != noErr) {
        if (GetProcessInformation (&psn, &pir) != noErr) {
          if (pir.processSignature == theSignature) {
            found = TRUE;
          }
        }
      }
      if (found) return TRUE; /* no need to launch if already running */
      if (! found) {
        err = FindFolder (kOnSystemDisk, kTemporaryFolderType,
                          kCreateFolder, &vRefNum, &dirID);
        if (err == noErr) {
          memset (&dtpb, 0, sizeof (DTPBRec));
          dtpb.ioNamePtr = NULL;
          dtpb.ioVRefNum = vRefNum;
          err = PBDTGetPath (&dtpb);
          if (err == noErr) {
            ioDTRefNum = dtpb.ioDTRefNum;
            if (ioDTRefNum != 0) {
              memset (&dtpb, 0, sizeof (DTPBRec));
              dtpb.ioDTRefNum = ioDTRefNum;
              dtpb.ioFileCreator = theSignature;
              dtpb.ioNamePtr = (StringPtr) pathname;
              dtpb.ioIndex = 0;
              memset (&pathname, 0, sizeof (pathname));
              err = PBDTGetAPPL (&dtpb, FALSE);
              if (err == noErr) {
                ioAPPLParID = dtpb.ioAPPLParID;
                if (pathname [0] != '\0') {
                  err = FSMakeFSSpec (vRefNum, ioAPPLParID,
                                      (ConstStr255Param) pathname, &fsspec);
                  okay = (Nlm_Boolean) (err == noErr);
                }
              }
            }
          }
        }
      }
    }
    if (okay) {
      myLaunchParams.launchBlockID = extendedBlock;
      myLaunchParams.launchEPBLength = extendedBlockLen;
      myLaunchParams.launchFileFlags = 0;
      myLaunchParams.launchControlFlags = launchContinue + launchNoFileFlags;
      myLaunchParams.launchAppParameters = NULL;
      myLaunchParams.launchAppSpec = &fsspec;
      rsult = (Nlm_Boolean) (LaunchApplication (&myLaunchParams) == noErr);
      if (serialNumPtr != NULL) {
        psnp = (ProcessSerialNumberPtr) serialNumPtr;
        *psnp = myLaunchParams.launchProcessSN;
      }
      SetFrontProcess (&myLaunchParams.launchProcessSN);
    }
  }
  return rsult;
#endif
#ifdef WIN_MSWIN
  Nlm_Char     ch;
  Nlm_Int2     i;
  Nlm_Int2     j;
  Nlm_Int2     k;
  char         lpszCmdLine [256];
  Nlm_Boolean  rsult;

  rsult = FALSE;
  if (fileName != NULL && fileName [0] != '\0') {
    Nlm_StringNCpy_0((Nlm_CharPtr)lpszCmdLine, fileName, sizeof(lpszCmdLine));
    i = 0;
    j = 0;
    k = 0;
    ch = fileName [j];
    while (i < 8 && ch != '\0') {
      if (ch == '_' || IS_ALPHANUM (ch)) {
        lpszCmdLine [k] = ch;
        j++;
        k++;
        ch = fileName [j];
      }
      i++;
    }
    while (ch != '\0' && ch != '.') {
      j++;
      ch = fileName [j];
    }
    if (ch == '.') {
      lpszCmdLine [k] = ch;
      j++;
      k++;
      ch = fileName [j];
      i = 0;
      while (i < 3 && ch != '\0') {
        if (ch == '_' || IS_ALPHANUM (ch)) {
          lpszCmdLine [k] = ch;
          j++;
          k++;
          ch = fileName [j];
        }
        i++;
      }
    }
    if (k > 0 && lpszCmdLine [k - 1] == '.') {
      lpszCmdLine [k - 1] = '\0';
    }
    lpszCmdLine [k] = '\0';
    rsult = (Nlm_Boolean) (WinExec (lpszCmdLine, SW_SHOW) >= 32);
  }
  return rsult;
#endif
#ifdef WIN_MOTIF
  Nlm_Boolean  rsult;
  int          status;

  rsult = FALSE;
  if (fileName != NULL && fileName [0] != '\0') {
#ifdef OS_UNIX
    status = (int) fork ();
    if (status == 0) {
      execlp (fileName, fileName, (char *) 0);
      Nlm_Message (MSG_FATAL, "Application launch failed");
    } else if (status != -1) {
      rsult = TRUE;
    }
#endif
#ifdef OS_VMS
    status = execl (fileName, fileName, (char *) 0);
    if ( status == -1 ) {
      Nlm_Message (MSG_FATAL, "Application launch failed");
    } else {
      rsult = TRUE;
    }
#endif
  }
  return rsult;
#endif
}

extern Nlm_Boolean Nlm_LaunchApp (Nlm_CharPtr fileName)

{
  return Nlm_LaunchAppEx (fileName, NULL, NULL);
}


extern Nlm_Boolean Nlm_Execv(const Nlm_Char* path, Nlm_Char *const *argv)
{
  if (!path  ||  !*path)
    return FALSE;

#if defined(WIN_MSWIN)
  {{
    int argc;
    char cmdline[256];

    size_t len = Nlm_StrLen(path);
    for (argc = 0;  argv[argc];  argc++) {
      len += 1 + Nlm_StrLen(argv[argc]);
    }
    if (len >= sizeof(cmdline)) {
      Nlm_Message(MSG_ERROR, "Nlm_Execv() failed: command line is too long");
      return FALSE;
    }

    Nlm_StrCpy(cmdline, path);
    for (argc = 0;  argv[argc];  argc++) {
      Nlm_StrCat(cmdline, " ");
      Nlm_StrCat(cmdline, argv[argc]);
    }
    ASSERT( StrLen(cmdline) < sizeof(cmdline) );

    return (Nlm_Boolean)(WinExec(cmdline, SW_SHOW) > 31);
  }}

#elif defined(OS_UNIX)
  {{
    int fork_code;
    /* Attention!
     * -- do not move this code to the "child part";
     *    it's tempting, but dangerous in the case of MT applications!
     */
    int x_argc;
    char **x_argv;
    for (x_argc = 0;  argv[x_argc];  x_argc++);
    x_argv = (char**)Nlm_MemNew((1 + x_argc + 1) * sizeof(char*));
    x_argv[0] = (char *)path;
    Nlm_MemCpy(&x_argv[1], &argv[0], (x_argc + 1) * sizeof(char*));

    /* Fork the process
     */
#ifdef SOLARIS_THREADS_AVAIL
    fork_code = fork1();
#else
    fork_code = fork();
#endif

    /* Child
     */
    if (fork_code == 0) {
      execv(path, x_argv);
      Nlm_Message(MSG_FATAL, "Nlm_Execv() failed: cannot execv()");
      ASSERT( 0 );
      return FALSE;
    }

    /* Parent
     */
    Nlm_MemFree(x_argv);
    if (fork_code == -1) {
      Nlm_Message(MSG_ERROR, "Nlm_Execv() failed: cannot fork()");
      return FALSE;
    }
    return TRUE;
  }}

#else
  return FALSE;
#endif
}


/* ++dgg -- added by d.gilbert, dec'93, for use w/ DCLAP c++ library */

extern void Nlm_SetObject (Nlm_GraphiC a, Nlm_VoidPtr thisobject)
{
  Nlm_GraphicData  gdata;
  if (a != NULL && thisobject != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    gdata.thisobject = thisobject;
    Nlm_SetGraphicData (a, &gdata);
  }
}

extern Nlm_VoidPtr Nlm_GetObject (Nlm_GraphiC a)
{
  Nlm_GraphicData  gdata;
  if (a != NULL) {
    Nlm_GetGraphicData (a, &gdata);
    return gdata.thisobject;
  } else {
    return NULL;
  }
}

#ifdef WIN_MAC
extern void Nlm_SendOpenDocAppleEventEx (Nlm_CharPtr datafile, Nlm_CharPtr sig, Nlm_CharPtr prog, Nlm_Boolean wantReply)

{
  long gval;
  OSErr theErr = noErr;
  AEAddressDesc theAddress;
  AEDesc docDesc;
  AppleEvent theEvent;
  AppleEvent theReply;
  OSType theSignature;
  AEDescList theList;
  FSSpec fss;
  Nlm_Char  temp [256];
  ProcessSerialNumber  psn;
  Nlm_Boolean  okay = FALSE;

  if (Gestalt (gestaltSystemVersion, &gval) == noErr && (short) gval >= 7 * 256) {
    theSignature = 'RSML';
    if (sig != NULL && sig [0] != '\0') {
      theSignature = *(OSType*) sig;
    }
    if (prog != NULL && prog [0] != '\0') {
      if (Nlm_LaunchAppEx (prog, (Nlm_VoidPtr) &psn, NULL)) {
        theErr = AECreateDesc(typeProcessSerialNumber, (Ptr)&psn,
                              sizeof(psn), &theAddress);
        okay = (Nlm_Boolean) (theErr == noErr);
      }
    } else if (sig != NULL && sig [0] != '\0') {
      if (Nlm_LaunchAppEx (prog, NULL, sig)) {
        theSignature = *(OSType*) sig;
        theErr = AECreateDesc(typeApplSignature, (Ptr)&theSignature,
                              sizeof(theSignature), &theAddress);
        okay = (Nlm_Boolean) (theErr == noErr);
      }
    }
    if (okay) {
      theErr = AECreateAppleEvent(kCoreEventClass, kAEOpenDocuments, &theAddress,
                                  kAutoGenerateReturnID, kAnyTransactionID, &theEvent);
      if (theErr == noErr) {
        theErr = AECreateList(NULL, 0, FALSE, &theList);
        if (theErr == noErr) {
          Nlm_StringNCpy_0(temp, datafile, sizeof(temp) - 1);
          Nlm_CtoPstr ((Nlm_CharPtr) temp);
          theErr = FSMakeFSSpec (0, 0, (ConstStr255Param) temp, &fss);
          if (theErr == noErr) {
            theErr = AECreateDesc(typeFSS, (Ptr)&fss, sizeof(fss), &docDesc);
            if (theErr == noErr) {
              theErr = AEPutDesc(&theList, 0, &docDesc);
              if (theErr == noErr) {
                theErr = AEPutParamDesc(&theEvent, keyDirectObject, &theList);
                if (theErr == noErr) {
                  if (wantReply) {
                    AESend (&theEvent, &theReply, kAEQueueReply, kAENormalPriority, 120, NULL, NULL);
                  } else {
                    AESend (&theEvent, &theReply, kAENoReply, kAENormalPriority, 120, NULL, NULL);
                  }
                }
              }
              AEDisposeDesc(&docDesc);
            }
          }
          AEDisposeDesc(&theList);
        }
        AEDisposeDesc(&theEvent);
      }
      AEDisposeDesc(&theAddress);
    }
  }
}

extern void Nlm_SendOpenDocAppleEvent (Nlm_CharPtr datafile, Nlm_CharPtr sig)

{
  Nlm_SendOpenDocAppleEventEx (datafile, sig, NULL, FALSE);
}

extern void Nlm_SendURLAppleEvent (Nlm_CharPtr urlString, Nlm_CharPtr sig, Nlm_CharPtr prog)

{
  long gval;
  OSErr theErr = noErr;
  AEAddressDesc theAddress;
  AEDesc urlEvent;
  AppleEvent theEvent;
  AppleEvent theReply;
  OSType theSignature;
  ProcessSerialNumber  psn;
  Nlm_Boolean  okay = FALSE;

  if (Gestalt (gestaltSystemVersion, &gval) == noErr && (short) gval >= 7 * 256) {
    theSignature = 'RSML';
    if (sig != NULL && sig [0] != '\0') {
      theSignature = *(OSType*) sig;
    }
    if (prog != NULL && prog [0] != '\0') {
      if (Nlm_LaunchAppEx (prog, (Nlm_VoidPtr) &psn, NULL)) {
        theErr = AECreateDesc(typeProcessSerialNumber, (Ptr)&psn,
                              sizeof(psn), &theAddress);
        okay = (Nlm_Boolean) (theErr == noErr);
      }
    } else if (sig != NULL && sig [0] != '\0') {
      if (Nlm_LaunchAppEx (prog, NULL, sig)) {
        theSignature = *(OSType*) sig;
        theErr = AECreateDesc(typeApplSignature, (Ptr)&theSignature,
                              sizeof(theSignature), &theAddress);
        okay = (Nlm_Boolean) (theErr == noErr);
      }
    }
    if (okay) {
      theErr = AECreateAppleEvent('WWW!', 'OURL', &theAddress,
                                  kAutoGenerateReturnID, kAnyTransactionID, &theEvent);
      if (theErr == noErr) {
        theErr = AECreateDesc(typeChar, urlString, Nlm_StringLen (urlString), &urlEvent);
        if (theErr == noErr) {
          theErr = AEPutParamDesc(&theEvent, keyDirectObject, &urlEvent);
          if (theErr == noErr) {
            AESend (&theEvent, &theReply, kAEWaitReply, kAEHighPriority, 500, NULL, NULL);
            AEDisposeDesc(&theReply);
          }
          AEDisposeDesc(&urlEvent);
        }
        AEDisposeDesc(&theEvent);
      }
      AEDisposeDesc(&theAddress);
    }
  }
}

extern void Nlm_GetFileTypeAndCreator (Nlm_CharPtr filename, Nlm_CharPtr type, Nlm_CharPtr creator);
extern void Nlm_GetFileTypeAndCreator (Nlm_CharPtr filename, Nlm_CharPtr type, Nlm_CharPtr creator)
{
  OSType    fCreator;
  Nlm_Int2  fError;
  FInfo     fInfo;
  OSType    fType;
  Nlm_Char  temp [256];

  if (type != NULL) {
    *type = '\0';
  }
  if (creator != NULL) {
    *creator = '\0';
  }
  Nlm_StringNCpy_0 (temp, filename, sizeof(temp));
  Nlm_CtoPstr ((Nlm_CharPtr) temp);
  fError = HGetFInfo ( 0, 0, (StringPtr) temp, &fInfo);
  if (fError == 0) {
    fType = fInfo.fdType;
    fCreator = fInfo.fdCreator;
    StringNCpy_0 (type, (Nlm_CharPtr) (&fType), 5);
    StringNCpy_0 (creator, (Nlm_CharPtr) (&fCreator), 5);
  }
}
#endif


#ifdef WIN_MSWIN
extern Nlm_Boolean Nlm_MSWin_OpenDocument(const Nlm_Char* doc_name)
{
  int status = (int)ShellExecute(0, "open", doc_name,
                                 NULL, NULL, SW_SHOWNORMAL);
  if (status <= 32) {
    Nlm_ErrPostEx(SEV_WARNING, 0, 0,
                  "Unable to open document \"%s\", error=%d",
                  doc_name, status);
    return FALSE;
  }
  return TRUE;
}

extern Nlm_Boolean Nlm_MSWin_OpenApplication(const Nlm_Char* program, const Nlm_Char* parameters)
{
  int status = (int)ShellExecute(0, "open", program,
                                 parameters, NULL, SW_SHOWNORMAL);
  if (status <= 32) {
    Nlm_ErrPostEx(SEV_WARNING, 0, 0,
                  "Unable to open document, error=%d", status);
    return FALSE;
  }
  return TRUE;
}

/*****************************************************************************

Function: Nlm_GetExecPath()

Purpose: Gets the path for an executable under Windows
  
Parameters: filetype: the name of the filetype (e.g. "valfile" for Cn3D)
            buf: the buffer to put the path into
            buflen: the length of buf


*****************************************************************************/

extern void Nlm_GetExecPath(char *filetype, char *buf, int buflen)
{
    HKEY hkResult;
    int i;
    char key[256];

    if(buf == NULL || filetype == NULL || buflen < 1) return;
    if(strlen(filetype)>220) return;
    key[0] = '\0';
    strcat(key, filetype);
    strcat(key, "\\Shell\\open\\command");
    RegOpenKeyEx(HKEY_CLASSES_ROOT, key, 0, KEY_READ,
        &hkResult);
    if(hkResult == NULL) {
        buf[0] = '\0';
        return;
    }

    RegQueryValueEx(hkResult,"", NULL, NULL, buf, &buflen);
    RegCloseKey(hkResult);

    for (i=1; i<buflen && buf[i] != '"'; i++);
    buf[i-1] = '\0';
}

#endif


/* esl: ChooseFont implementation */

#ifdef WIN_MSWIN

static void Nlm_LOGFONTToFontSpec (LOGFONT *lfp, Nlm_FontSpecPtr fsp)
{
  if (fsp == NULL || lfp == NULL) return;

  /* size */
  if (lfp->lfHeight == 0) { /* lfHeight is not specified */
    fsp->size = 12; /* use default height */
  } else {
    int sizelp; /* size in logical points */
    if (lfp->lfHeight < 0) { /* -lfHeight is character height */
      sizelp = -lfp->lfHeight;
    } else { /* lfHeight is cell height */
      sizelp = lfp->lfHeight;
      sizelp -= sizelp / 10;  /* adhoc: 10% for internal leading */
    }
    { /* determine size in typographical points */
      HDC hDC = GetDC (NULL);
      fsp->size = (Nlm_Int2)MulDiv(sizelp, 72, GetDeviceCaps(hDC, LOGPIXELSY));
      ReleaseDC (NULL, hDC);
    }
  }

  { /* style */
    int style = 0;
    if (lfp->lfWeight > 600) style |= STYLE_BOLD;
    if (lfp->lfItalic != 0) style |= STYLE_ITALIC;
    if (lfp->lfUnderline != 0) style |= STYLE_UNDERLINE;
    if (lfp->lfStrikeOut != 0) style |= 128; /* Windows-specific */
    fsp->style = (Nlm_Uint1)style;
  }

  /* character set */
  switch (lfp->lfCharSet) {
    case SYMBOL_CHARSET: fsp->charset = CHARSET_SYMBOL; break;
    case ANSI_CHARSET: fsp->charset = CHARSET_ANSI; break;
    default: fsp->charset = CHARSET_NULL;
  }

  /* pitch */
  switch (lfp->lfPitchAndFamily & 0x0F) {
    case FIXED_PITCH: fsp->pitch = PITCH_FIXED; break;
    case VARIABLE_PITCH: fsp->pitch = PITCH_VARIABLE; break;
    default: fsp->pitch = PITCH_NULL;
  }

  /* family */
  switch (lfp->lfPitchAndFamily & 0xF0) {
    case FF_ROMAN: fsp->family = FAMILY_ROMAN; break;
    case FF_SWISS: fsp->family = FAMILY_SWISS; break;
    case FF_MODERN: fsp->family = FAMILY_MODERN; break;
    case FF_SCRIPT: fsp->family = FAMILY_SCRIPT; break;
    case FF_DECORATIVE: fsp->family = FAMILY_DECORATIVE; break;
    default: fsp->family = FAMILY_NULL;
  }

  /* name */
  Nlm_StringNCpy_0(fsp->name, lfp->lfFaceName, FONT_NAME_SIZE);
}

extern Nlm_Boolean Nlm_ChooseFont (Nlm_FontSpecPtr fsp,
                                   Nlm_Uint2 flags,
                                   Nlm_VoidPtr null)
{
  Nlm_Boolean initToFsp = (Nlm_Boolean)((flags & CFF_READ_FSP) != 0);
  Nlm_Boolean fixedOnly = (Nlm_Boolean)((flags & CFF_MONOSPACE) != 0);
  LOGFONT lf;
  CHOOSEFONT cf;
  if (fsp == NULL) { /* ERROR */  return FALSE; }
  if (initToFsp) Nlm_FontSpecToLOGFONT (fsp, &lf);
  memset (&cf, 0, sizeof (CHOOSEFONT));
  cf.lStructSize = sizeof (CHOOSEFONT);
  cf.hwndOwner = Nlm_currentHWnd;
  cf.hDC = NULL;
  cf.lpLogFont = &lf;
  cf.Flags = CF_SCREENFONTS | CF_NOVECTORFONTS | CF_EFFECTS;
  if (initToFsp) cf.Flags |= CF_INITTOLOGFONTSTRUCT;
  if (fixedOnly) cf.Flags |= CF_FIXEDPITCHONLY;
  if (ChooseFont (&cf)) {
    Nlm_LOGFONTToFontSpec (&lf, fsp);
    return TRUE;
  } else
    return FALSE;
}

#else /* !WIN_MSWIN */

static Nlm_FontSpec     fntDlgFontSpec;
static Nlm_Boolean      fntDlgUp;
static Nlm_Boolean      fntDlgResult;
#ifdef WIN_MAC
static Nlm_PopuP        fntDlgFontList = NULL;
static Nlm_PopuP        fntDlgSizeList = NULL;
#endif
#ifdef WIN_MOTIF
static Nlm_LisT         fntDlgFontList = NULL;
static Nlm_LisT         fntDlgSizeList = NULL;
#endif
static Nlm_ButtoN       fntDlgBoldBox = NULL;
static Nlm_ButtoN       fntDlgItalicBox = NULL;
static Nlm_ButtoN       fntDlgUnderlineBox = NULL;
static Nlm_PaneL        fntDlgSample = NULL;

#define NLM_STYLE_BOLD_ON        0x1
#define NLM_STYLE_BOLD_OFF       0x2
#define NLM_STYLE_ITALIC_ON      0x4
#define NLM_STYLE_ITALIC_OFF     0x8
#define NLM_STYLE_UNDERLINE_ON   0x10
#define NLM_STYLE_UNDERLINE_OFF  0x20

static void Nlm_FntDlgDrawSampleProc (Nlm_PaneL p)
{
  Nlm_FonT font = Nlm_CreateFont (&fntDlgFontSpec);
  Nlm_RecT r;
  Nlm_ObjectRect (p, &r);
  Nlm_SelectFont (font);
  Nlm_DrawString (&r, "The Quick Brown Fox", 'c', FALSE);
  Nlm_SelectFont (Nlm_systemFont);
  Nlm_DeleteFont (font);
}

static void Nlm_FntDlgRedrawSample (void)
{
  Nlm_RecT r;
  Nlm_WindoW tmpPort;

  Nlm_ObjectRect (fntDlgSample, &r);
  tmpPort = Nlm_SavePort (fntDlgSample);
  Nlm_Select (fntDlgSample);
  Nlm_InvalRect (&r);
  Nlm_RestorePort (tmpPort);
}

/* simple font name/size enum interface (platform-dependent) */
/* alexs (01-16-95): add platform-dependent for MAC */
#ifdef WIN_MAC

static MenuHandle menuHforFontList = 0;

static Nlm_Int2 Nlm_FntDlgGetSysFontList (Nlm_Boolean monoSpace )
{
  int      curItemNum;
  short    famId;
  char     fontName[256];

  menuHforFontList = NewMenu ( 100, (StringPtr) "" );
  AppendResMenu ( menuHforFontList, 'FONT' );
  if ( monoSpace ){
    for (curItemNum = (Nlm_Int2)CountMenuItems(menuHforFontList);
         curItemNum > 0; curItemNum-- ){
      GetMenuItemText ( menuHforFontList, curItemNum, (StringPtr) fontName );
      GetFNum((StringPtr) fontName, &famId);
      TextSize (12);
      TextFace (0);
      TextFont (famId);
      if ( (CharWidth('i') != CharWidth('M')) ||
           (CharWidth('i') != CharWidth(' '))){
        DeleteMenuItem (menuHforFontList,curItemNum);
      }

    }
    Nlm_SelectFont (Nlm_systemFont);
  }
  return (Nlm_Int2)CountMenuItems(menuHforFontList);
}

static void Nlm_FntDlgGetSysFontName (Nlm_Int2 n, Nlm_CharPtr buf,
                                      Nlm_Int2 blen)
{
  char fontName[256];

  GetMenuItemText ( menuHforFontList, n+1, (StringPtr) fontName );
  Nlm_PtoCstr ( fontName );
  Nlm_StringNCpy_0(buf, fontName, blen);
}

static Nlm_Byte fontSizes [] = {9, 10, 12, 14, 18, 20, 24, 36};
static Nlm_Int2 fontSizesCnt = sizeof(fontSizes) / sizeof(fontSizes[0]);

static Nlm_Int2 Nlm_FntDlgGetSysFontSize (Nlm_Int2 n,
                                          Nlm_BytePtr PNTR sizePtr )
{
  n = n;
  *sizePtr = &(fontSizes[0]);
  return fontSizesCnt;
}

static Nlm_Byte Nlm_FntDlgGetSysFontStyle (Nlm_Int2 n)
{
  return ( NLM_STYLE_BOLD_ON |
           NLM_STYLE_BOLD_OFF |
           NLM_STYLE_ITALIC_ON |
           NLM_STYLE_ITALIC_OFF |
           NLM_STYLE_UNDERLINE_ON |
           NLM_STYLE_UNDERLINE_OFF );
}

static void Nlm_FntDlgFreeSysFontList ()
{
  DisposeMenu (menuHforFontList);
}

#endif
#ifdef WIN_MOTIF

/* alexs (01-16-95): add platform-dependent for X_WIN */
#define NLM_X11_MAXSIZECOUNT   64

typedef struct {
  Nlm_Char  fontName[FONT_NAME_SIZE];
  Nlm_Byte  styleInter;
  Nlm_Byte  sizeFound;
  Nlm_Byte  sizes[NLM_X11_MAXSIZECOUNT];
} Nlm_X11_FontDsc, * Nlm_X11_FontDscPtr;

static Nlm_X11_FontDscPtr fontNamesArray = NULL;

static Nlm_Byte Nlm_FntGetFontStyle ( char * fontSpec )
{
  Nlm_Byte fStyle = 0;

  fontSpec = Nlm_StringChr ( fontSpec, '-' );
  if ( fontSpec == NULL ) return 0;
  fontSpec = Nlm_StringChr ( fontSpec+1, '-');
  if ( fontSpec == NULL ) return 0;
  fontSpec = Nlm_StringChr ( fontSpec+1, '-');
  if ( fontSpec == NULL ) return 0;
  fontSpec++;
  if ( *fontSpec == 'b' ) fStyle |= NLM_STYLE_BOLD_ON;
  else fStyle |= NLM_STYLE_BOLD_OFF;
  fontSpec = Nlm_StringChr ( fontSpec, '-');
  if ( fontSpec == NULL ) return fStyle;
  fontSpec++;
  /* VL */
  if ( *fontSpec == 'i' || *fontSpec == 'o' ) fStyle |= NLM_STYLE_ITALIC_ON;
  else fStyle |= NLM_STYLE_ITALIC_OFF;
  return fStyle;
}

static void Nlm_FntAddSize ( Nlm_X11_FontDscPtr fontDsc,
                             char * fontSpec )
{
  int      i;
  int      curSize = 12;
  char     sizeStr[16];

  if ( fontDsc->sizeFound == NLM_X11_MAXSIZECOUNT ) return;
  for ( i=0; i<8; i++ ){
    fontSpec = Nlm_StringChr ( fontSpec, '-' );
    if ( fontSpec == NULL ) break;
    fontSpec++;
  }
  if ( fontSpec != NULL ){
    Nlm_StringNCpy_0(sizeStr, fontSpec, sizeof(sizeStr));
    fontSpec = Nlm_StringChr ( sizeStr, '-' );
    if ( fontSpec != NULL ) *fontSpec = '\0';
    curSize = atoi ( sizeStr ) / 10 ;
    if ( (curSize <= 0) || (curSize > 128) ) curSize = 12;
  }
  for ( i=0; i < (int)fontDsc->sizeFound; i++ ){
     if ( curSize == (int)fontDsc->sizes[i] ) break;
     if ( curSize < (int)fontDsc->sizes[i] ) {
        Nlm_MemMove ( &(fontDsc->sizes[i+1]), &(fontDsc->sizes[i]),
                      fontDsc->sizeFound - i );
        fontDsc->sizes[i] = (Nlm_Byte)curSize;
        fontDsc->sizeFound++;
        break;
     }
  }
  if ( i == fontDsc->sizeFound ){
    fontDsc->sizes[fontDsc->sizeFound] = (Nlm_Byte)curSize;
    fontDsc->sizeFound++;
  }
}

static Nlm_Int2 Nlm_FntDlgGetSysFontList (Nlm_Boolean mono)
{
  char ** fontNames;
  char *  curFontPtr;
  Nlm_X11_FontDscPtr  curFontDsc;
  int     fontNamesCount;
  int     fontsFound = 0;
  int     i, ifound;
  char    curFontName[FONT_NAME_SIZE];

  if (mono) {
    fontNames = XListFonts ( Nlm_currentXDisplay,
			     "-*-*-*-*-*-*-*-*-*-M-*",
			     4096, &fontNamesCount );
  } else {
    fontNames = XListFonts ( Nlm_currentXDisplay,
			     "-*-*-*-*-*-*-*-*-*-*-*",
			     4096, &fontNamesCount );
  }
  if ( fontNames == NULL ) return 0;
  fontNamesArray = (Nlm_X11_FontDscPtr)Nlm_MemNew (
      sizeof(Nlm_X11_FontDsc) * fontNamesCount );
  for ( i=0; i<fontNamesCount; i++ ){
    curFontPtr = Nlm_StringChr (fontNames[i], '-' );
    if ( curFontPtr == NULL ) continue;
    curFontPtr++;
    curFontPtr = Nlm_StringChr (curFontPtr, '-' );
    if ( curFontPtr == NULL ) continue;
    curFontPtr++;
    Nlm_StringNCpy_0(curFontName, curFontPtr, FONT_NAME_SIZE);
    curFontPtr = Nlm_StringChr ( curFontName, '-' );
    if ( curFontPtr != NULL ) *curFontPtr = '\0';
    if ( Nlm_StringLen(curFontName) == 0 ) continue;
    for ( ifound = 0, curFontDsc=fontNamesArray;
          ifound < fontsFound;
          ifound++, curFontDsc ++ ){
      if ( Nlm_StrCmp ( curFontDsc->fontName, curFontName ) == 0 ) {
        curFontDsc->styleInter |= Nlm_FntGetFontStyle ( (char*)fontNames[i]) ;
        Nlm_FntAddSize ( curFontDsc, (char*)fontNames[i] );
        break;
      }
    }
    if ( ifound == fontsFound ){
      Nlm_StrCpy ( curFontDsc->fontName, curFontName );
      curFontDsc->styleInter = Nlm_FntGetFontStyle ( (char*)fontNames[i]) ;
      Nlm_FntAddSize ( curFontDsc, (char*)fontNames[i] );
      fontsFound++;
    }
  }
  XFreeFontNames ( fontNames );
  return fontsFound;
}

static void Nlm_FntDlgGetSysFontName (Nlm_Int2 n, Nlm_CharPtr buf,
                                      Nlm_Int2 blen)
{
  Nlm_StringNCpy_0(buf, fontNamesArray[n].fontName, blen);
}

static Nlm_Int2 Nlm_FntDlgGetSysFontSize (Nlm_Int2 n,
                                          Nlm_BytePtr PNTR sizePtr )
{
  *sizePtr = fontNamesArray[n].sizes;
  return (fontNamesArray[n].sizeFound );
}

static Nlm_Byte Nlm_FntDlgGetSysFontStyle (Nlm_Int2 n)
{
  return ( fontNamesArray[n].styleInter );
}

static void Nlm_FntDlgFreeSysFontList(void)
{
  if ( fontNamesArray != NULL ){
    Nlm_MemFree ( fontNamesArray );
    fontNamesArray = NULL;
  }
}
#endif

/* simple font name/size enum interface (platform-independent) */
static Nlm_Int2 sysFontCnt;


/* init FontList */
static void Nlm_FntDlgInitFontList (void)
{
  Nlm_Int2 i;
  Nlm_Char buf [FONT_NAME_SIZE];
  for (i = 0; i < sysFontCnt; i++) {
    Nlm_FntDlgGetSysFontName (i, buf, FONT_NAME_SIZE);
#ifdef WIN_MAC
    Nlm_PopupItem (fntDlgFontList, buf);
#endif
#ifdef WIN_MOTIF
    Nlm_ListItem (fntDlgFontList, buf);
#endif
  }
}

static void Nlm_FntDlgInitSizeList ( Nlm_Int2 n, Nlm_Int2 oldSize )
{
  Nlm_BytePtr sizePtr;
  Nlm_Int2 i;
  Nlm_Int2 setThis;
  Nlm_Int2 fontCnt;
  Nlm_Char buf [10];

  fontCnt = Nlm_FntDlgGetSysFontSize ( n, &sizePtr );
  setThis = 0;
  for (i = 0; i < fontCnt; i++) {
    if ( !setThis ){
      if ( (Nlm_Int2)sizePtr[i] >= oldSize ) setThis = i+1;
    }
    sprintf (buf, "%d", (int)sizePtr[i]);
#ifdef WIN_MAC
    Nlm_PopupItem (fntDlgSizeList, buf);
#endif
#ifdef WIN_MOTIF
    Nlm_ListItem (fntDlgSizeList, buf);
#endif
  }
  if ( !setThis ) setThis = fontCnt;
  Nlm_SetValue (fntDlgSizeList, setThis);
}

static void Nlm_FntDlgInitStyles ( Nlm_Int2 n, Nlm_Int2 oldStyle )
{
  Nlm_Byte styles;

  styles = Nlm_FntDlgGetSysFontStyle (n);
  if ( (styles & NLM_STYLE_BOLD_ON) && (styles & NLM_STYLE_BOLD_OFF) ){
    Nlm_Enable (fntDlgBoldBox);
    if ( oldStyle & STYLE_BOLD ){
      Nlm_SetStatus (fntDlgBoldBox, TRUE);
    } else {
      Nlm_SetStatus (fntDlgBoldBox, FALSE);
    }
  }else{
    Nlm_Disable (fntDlgBoldBox);
    if ( styles & NLM_STYLE_BOLD_ON ){
      Nlm_SetStatus (fntDlgBoldBox, TRUE);
    } else {
      Nlm_SetStatus (fntDlgBoldBox, FALSE);
    }
  }
  if ( (styles & NLM_STYLE_ITALIC_ON) && (styles & NLM_STYLE_ITALIC_OFF) ){
    Nlm_Enable (fntDlgItalicBox);
    if ( oldStyle & STYLE_ITALIC ){
      Nlm_SetStatus (fntDlgItalicBox, TRUE);
    } else {
      Nlm_SetStatus (fntDlgItalicBox, FALSE);
    }
  }else{
    Nlm_Disable (fntDlgItalicBox);
    if ( styles & NLM_STYLE_ITALIC_ON ){
      Nlm_SetStatus (fntDlgItalicBox, TRUE);
    } else {
      Nlm_SetStatus (fntDlgItalicBox, FALSE);
    }
  }
  if ( (styles & NLM_STYLE_UNDERLINE_ON) &&
       (styles & NLM_STYLE_UNDERLINE_OFF) ){
    Nlm_Enable (fntDlgUnderlineBox);
    if ( oldStyle & STYLE_UNDERLINE ){
      Nlm_SetStatus (fntDlgUnderlineBox, TRUE);
    } else {
      Nlm_SetStatus (fntDlgUnderlineBox, FALSE);
    }
  }else{
    Nlm_Disable (fntDlgUnderlineBox);
    if ( styles & NLM_STYLE_UNDERLINE_ON ){
      Nlm_SetStatus (fntDlgUnderlineBox, TRUE);
    } else {
      Nlm_SetStatus (fntDlgUnderlineBox, FALSE);
    }
  }
}

static void Nlm_FntDlgUpdate (Nlm_Boolean writep)
{
  if (writep) {
    Nlm_BytePtr sizes;
    Nlm_Int2    i;
    Nlm_Int2    sizeCount;

    /* name and family/pitch/charset */
    i = Nlm_GetValue (fntDlgFontList);
    if (i > 0 && i <= sysFontCnt) {
      Nlm_MemSet (fntDlgFontSpec.name, 0, FONT_NAME_SIZE);
      Nlm_FntDlgGetSysFontName (i - 1, fntDlgFontSpec.name, FONT_NAME_SIZE);
      /* name is set! clear family/pitch/charset fields */
      fntDlgFontSpec.family = FAMILY_NULL;
      fntDlgFontSpec.pitch = PITCH_NULL;
      fntDlgFontSpec.charset = CHARSET_NULL;
      /* size */
      sizeCount = Nlm_FntDlgGetSysFontSize (i-1, &sizes );
      i = Nlm_GetValue (fntDlgSizeList);
      if (i > 0 && i <=  sizeCount ){
        fntDlgFontSpec.size = sizes[i - 1];
      }
    }
    /* style */
    i = 0;
    if (Nlm_GetStatus (fntDlgBoldBox)) i |= STYLE_BOLD;
    if (Nlm_GetStatus (fntDlgItalicBox)) i |= STYLE_ITALIC;
    if (Nlm_GetStatus (fntDlgUnderlineBox)) i |= STYLE_UNDERLINE;
    fntDlgFontSpec.style = (Nlm_Uint1) i;
  } else {
    Nlm_Int2 i = 0;
    Nlm_Int2 valSet = 0;
    Nlm_Char buf[FONT_NAME_SIZE];
    /* name */
    if (fntDlgFontSpec.name [0] != '\0'){
      for (i = 0; i < sysFontCnt; i++) {
        Nlm_FntDlgGetSysFontName (i, buf, FONT_NAME_SIZE);
        if (Nlm_StrNICmp (buf, fntDlgFontSpec.name, FONT_NAME_SIZE) == 0) {
          Nlm_SetValue (fntDlgFontList, i + 1);
          valSet = 1;
          break;
        }
      }
    }
    if ( !valSet ){
      Nlm_SetValue (fntDlgFontList,1);
      i = 0;
    }
    Nlm_FntDlgInitSizeList ( i, fntDlgFontSpec.size );
    Nlm_FntDlgInitStyles ( i, fntDlgFontSpec.style );
  }
}

static void Nlm_FntDlgOKProc (Nlm_ButtoN b)
{
  fntDlgUp = FALSE; fntDlgResult = TRUE;
}

static void Nlm_FntDlgCancelProc (Nlm_ButtoN b)
{
  fntDlgUp = FALSE; fntDlgResult = FALSE;
}

static void Nlm_FntDlgCheckBoxProc (Nlm_ButtoN b)
{
  Nlm_FntDlgUpdate (TRUE);
  Nlm_FntDlgRedrawSample ();
}

#ifdef WIN_MAC
static void Nlm_FntDlgPopupProc (Nlm_PopuP p)
{
  Nlm_FntDlgUpdate (TRUE);
  Nlm_FntDlgRedrawSample ();
}
#endif

#ifdef WIN_MOTIF
static void Nlm_FntDlgListProc (Nlm_LisT l)
{
  Nlm_Int2 i;

  i = Nlm_GetValue (fntDlgFontList);
  if (i > 0 && i <= sysFontCnt) {
    Nlm_Hide ( fntDlgSizeList );
    Nlm_Reset ( fntDlgSizeList );
    Nlm_FntDlgInitSizeList ( i-1, fntDlgFontSpec.size );
    Nlm_Show ( fntDlgSizeList );
    Nlm_FntDlgInitStyles ( i-1, fntDlgFontSpec.style );
  }
  Nlm_FntDlgUpdate (TRUE);
  Nlm_FntDlgRedrawSample ();
}

static void Nlm_FntDlgListSizeProc (Nlm_LisT l)
{
  Nlm_FntDlgUpdate (TRUE);
  Nlm_FntDlgRedrawSample ();
}
#endif

extern Nlm_Boolean Nlm_ChooseFont (Nlm_FontSpecPtr fsp,
                                   Nlm_Uint2 flags,
                                   Nlm_VoidPtr null)
{
  Nlm_Boolean initToFsp = (Nlm_Boolean)((flags & CFF_READ_FSP) != 0);
  Nlm_Boolean fixedOnly = (Nlm_Boolean)((flags & CFF_MONOSPACE) != 0);
  Nlm_WindoW w = Nlm_MovableModalWindow (-50, -20, -20, -20, "Font", NULL);
  Nlm_GrouP gfontsize, gstysamp, gbuttons;

  sysFontCnt = Nlm_FntDlgGetSysFontList (fixedOnly);
  { Nlm_GrouP g = Nlm_HiddenGroup (w, 10, 0, NULL);
    gfontsize = g;
    Nlm_SetGroupSpacing (g, 10, 5);
    Nlm_StaticPrompt (g, "Font:", 0, 0, Nlm_systemFont, 'r');
#ifdef WIN_MAC
    fntDlgFontList = Nlm_PopupList (g, TRUE, Nlm_FntDlgPopupProc);
#endif
#ifdef WIN_MOTIF
    fntDlgFontList = Nlm_SingleList (g, 16, 6, Nlm_FntDlgListProc);
#endif
    Nlm_FntDlgInitFontList ();
    Nlm_StaticPrompt (g, "Size:", 0, 0, Nlm_systemFont, 'r');
#ifdef WIN_MAC
    fntDlgSizeList = Nlm_PopupList (g, TRUE, Nlm_FntDlgPopupProc);
#endif
#ifdef WIN_MOTIF
    fntDlgSizeList = Nlm_SingleList (g, 3, 5, Nlm_FntDlgListSizeProc);
#endif
  }
  { Nlm_GrouP g = Nlm_HiddenGroup (w, 10, 0, NULL);
    Nlm_GrouP gstyle, gsample;
    gstysamp = g;
    Nlm_SetGroupSpacing (g, 10, 5);
    { Nlm_GrouP s = Nlm_NormalGroup (g, 0, 3, "Style:", NULL, NULL);
      gstyle = s;
      Nlm_SetGroupMargins (s, 10, 5);
      Nlm_SetGroupSpacing (s, 10, 5);
      fntDlgBoldBox = Nlm_CheckBox (s, "Bold", Nlm_FntDlgCheckBoxProc);
      fntDlgItalicBox = Nlm_CheckBox (s, "Italic", Nlm_FntDlgCheckBoxProc);
      fntDlgUnderlineBox = Nlm_CheckBox (s, "Underline", Nlm_FntDlgCheckBoxProc);
    }
    { Nlm_GrouP s = Nlm_NormalGroup (g, 0, 10, "Sample", NULL, NULL);
      Nlm_Int2 pixwidth = Nlm_stdCharWidth * 15;
      Nlm_Int2 pixheight = Nlm_stdLineHeight * 5;
      gsample = s;
      Nlm_SetGroupSpacing (s, 10, 5);
      fntDlgSample = Nlm_SimplePanel (s, pixwidth, pixheight,
                                      Nlm_FntDlgDrawSampleProc);
    }
    /*
    Nlm_AlignObjects (ALIGN_RIGHT, (Nlm_HANDLE) fntDlgSizeList,
                      (Nlm_HANDLE) gsample, NULL);
    */
    Nlm_AlignObjects (ALIGN_LOWER, (Nlm_HANDLE) gstyle,
                                    (Nlm_HANDLE) gsample, NULL);
  }
  { Nlm_GrouP g = Nlm_HiddenGroup (w, 10, 0, NULL);
    gbuttons = g;
    Nlm_SetGroupSpacing (g, 10, 5);
    Nlm_DefaultButton (g, "  OK  ", Nlm_FntDlgOKProc);
    Nlm_PushButton (g, "Cancel", Nlm_FntDlgCancelProc);
  }
  Nlm_AlignObjects (ALIGN_CENTER, (Nlm_HANDLE) gfontsize,
                                  (Nlm_HANDLE) gstysamp,
                                  (Nlm_HANDLE) gbuttons, NULL);
  /* init fontspec */
  if (initToFsp && fsp != NULL) {
    fntDlgFontSpec = *fsp;
  } else {
    Nlm_GetFontSpec (fixedOnly ? Nlm_programFont : Nlm_systemFont,
                     &fntDlgFontSpec);
  }
  /* update controls */
  Nlm_FntDlgUpdate (FALSE);
  /* show dialog */
  Nlm_DoShow ((Nlm_GraphiC) w, TRUE, TRUE);
  Nlm_ArrowCursor ();
  fntDlgUp = TRUE;
  fntDlgResult = FALSE;
  Nlm_WaitForCondition( fntDlgUp );
  Nlm_ProcessAnEvent ();
  /* update FontSpec */
  Nlm_FntDlgUpdate (TRUE);
  Nlm_DoRemove ((Nlm_GraphiC) w, TRUE);
  if (fntDlgResult == TRUE && fsp != NULL) *fsp = fntDlgFontSpec;
  Nlm_FntDlgFreeSysFontList ();
  return fntDlgResult;
}

#endif /* !WIN_MSWIN */

/* FontSpec/string conversions */

extern Nlm_Boolean Nlm_StrToFontSpec (Nlm_CharPtr str, Nlm_FontSpecPtr fsp)
{
  Nlm_CharPtr cp;

  if (fsp == NULL  ||  str == NULL)
    return FALSE;

  /* get name */
  if ((cp = Nlm_StrChr(str, ',')) == NULL)
    return FALSE;

  Nlm_StringNCpy_0(fsp->name, str, MIN(cp - str + 1, FONT_NAME_SIZE));

  {{ /* get other parameters */
    int size, style, charset, pitch, family;
    int n = sscanf (cp, ",%d,%d,%d,%d,%d",
                    &size, &style, &charset, &pitch, &family);
    fsp->size = (Nlm_Int2) size;
    fsp->style = (Nlm_Uint1) style;
    fsp->charset = (Nlm_Uint1) charset;
    fsp->pitch = (Nlm_Uint1) pitch;
    fsp->family = (Nlm_Uint1) family;
    return (Nlm_Boolean)(n == 5);
  }}
}

extern void Nlm_FontSpecToStr (Nlm_FontSpecPtr fsp, Nlm_CharPtr str, size_t maxsize)
{
  Nlm_Char  temp [80];
  if (fsp == NULL) {
    temp[0] = '\0';
  } else {
    sprintf (temp, "%s,%d,%d,%d,%d,%d", fsp->name,
                   (int) fsp->size, (int) fsp->style,
                   (int) fsp->charset, (int) fsp->pitch,
                   (int) fsp->family);
  }
  Nlm_StringNCpy_0(str, temp, maxsize);
}



#ifdef OS_UNIX
extern void Nlm_FineGranularitySleep (Nlm_Int4 seconds, Nlm_Int4 microseconds)
{
      struct timeval tv;

      tv.tv_sec = seconds;
      tv.tv_usec = microseconds;
      select(1, NULL, NULL, NULL, &tv); /* fine granularity sleep */
}
#endif /* OS_UNIX */

#ifdef WIN_MOTIF
extern void Nlm_WaitForXEvent( void )
{
  XEvent event;
  XPeekEvent(Nlm_currentXDisplay, &event);
}
#endif

extern void Nlm_SetRealized(Nlm_GraphiC a, Nlm_Boolean realizd)
{
  Nlm_Boolean  switched;

  if ( !a )  return;

  {{
    Nlm_GraphicData  gdata;
    Nlm_GetGraphicData (a, &gdata);
    switched = (Nlm_Boolean)(gdata.realized != realizd);
    gdata.realized = realizd;
    Nlm_SetGraphicData (a, &gdata);
  }}

  if (switched  &&  realizd)
    {
      Nlm_RecT r;
      Nlm_GetPosition(a, &r);
      Nlm_DoSetPosition(a, &r, TRUE, TRUE);
    }

  for (a = Nlm_GetChild( a );  a;  a = Nlm_GetNext( a ))
    {
      Nlm_SetRealized(a, realizd);
    }
}


extern Nlm_Boolean Nlm_GetRealized(Nlm_GraphiC a)
{
  Nlm_GraphicData  gdata;

  if ( a )
    {
      Nlm_GetGraphicData(a, &gdata);
      return gdata.realized;
    }

  return FALSE;
}

#ifdef WIN_MSWIN
#ifdef WIN16
extern Nlm_Uint2 Nlm_GetDriveType (int driveNumber);
extern Nlm_Uint2 Nlm_GetDriveType (int driveNumber)

{
  return (Nlm_Uint2) GetDriveType (driveNumber);
}
#else
extern Nlm_Uint2 Nlm_GetDriveType (char* path);
extern Nlm_Uint2 Nlm_GetDriveType (char* path)

{
  return (Nlm_Uint2) GetDriveType (path);
}
#endif
#endif

/*
 * Navigation and special key translation and rendering
 */


#ifdef WIN_MOTIF
extern Nlm_Char Nlm_GetInputChar(XKeyEvent *event)
{
  KeySym keysym;
  char   keystr[16];
  int    charcount = XLookupString(event, keystr, sizeof(keystr),
                                   &keysym, NULL);
  switch ( keysym )
    {
    case XK_Left:
      return NLM_LEFT;
    case XK_Right:
      return NLM_RIGHT;
    case XK_Up:
      return NLM_UP;
    case XK_Down:
      return NLM_DOWN;
    case XK_Prior:
      return NLM_PREV;
    case XK_Next:
      return NLM_NEXT;
    case XK_End:      
      return NLM_END;
    case XK_Home:    
      return NLM_HOME;
    case XK_Delete:   
      return NLM_DEL;
    case XK_Insert:   
      return NLM_INS;
    case XK_Escape:   
      return NLM_ESC;
    }

  return (charcount == 1) ? keystr[0] : '\0';
}


static void Nlm_StdTranslationCB(Widget w, XEvent *event,
                                 String *args, Cardinal *num_args)
{
  Nlm_Char ch;
  XtPointer ptr;

  XtVaGetValues(w, XmNuserData, &ptr, NULL);
  ASSERT ( ptr );

  ch = Nlm_GetInputChar( &event->xkey );
  ASSERT ( ch );

  Nlm_DoSendFocus((Nlm_GraphiC)ptr, ch);
}


extern void Nlm_RegisterStdTranslations( void )
{
  XtActionsRec actions;

  actions.string = "Nlm_StdTranslation";
  actions.proc   = Nlm_StdTranslationCB;
  XtAppAddActions(Nlm_appContext, &actions, 1);
}


extern void Nlm_OverrideStdTranslations(Nlm_GraphiC g, Widget w,
                                        Nlm_Int4 filter)
{
  if ( !(filter & (VERT_PAGE | VERT_ARROW | HORIZ_PAGE | HORIZ_ARROW)) )
    return;

#ifdef _DEBUG
  {{
    XtPointer ptr;
    XtVaGetValues(w, XmNuserData, &ptr, NULL);
    ASSERT ( !ptr );
  }}
#endif
  XtVaSetValues(w, XmNuserData, (XtPointer)g, NULL);

  if (filter & VERT_PAGE)
    { 
      XtOverrideTranslations(w, XtParseTranslationTable( "\
<Key>osfPageUp:    Nlm_StdTranslation() \n\
<Key>osfPageDown:  Nlm_StdTranslation()" ));
    }

  if (filter & VERT_ARROW)
    { 
      XtOverrideTranslations(w, XtParseTranslationTable( "\
<Key>osfUp:    Nlm_StdTranslation() \n\
<Key>osfDown:  Nlm_StdTranslation()" ));
    }

  if (filter & HORIZ_PAGE)
    { 
      XtOverrideTranslations(w, XtParseTranslationTable( "\
<Key>osfBeginLine:  Nlm_StdTranslation() \n\
<Key>osfEndLine:    Nlm_StdTranslation()" ));
    }

  if (filter & HORIZ_ARROW)
    { 
      XtOverrideTranslations(w, XtParseTranslationTable( "\
<Key>osfLeft:   Nlm_StdTranslation() \n\
<Key>osfRight:  Nlm_StdTranslation()" ));
    }
}
#endif


#ifdef WIN_MSWIN
extern Nlm_Char Nlm_KeydownToChar(WPARAM wParam)
{
  switch ( wParam )
    {
    case VK_LEFT:
      return NLM_LEFT;
    case VK_RIGHT:
      return NLM_RIGHT;
    case VK_UP:
      return NLM_UP;
    case VK_DOWN:
      return NLM_DOWN;
    case VK_PRIOR:
      return NLM_PREV;
    case VK_NEXT:
      return NLM_NEXT;
    case VK_HOME:
      return NLM_HOME;
    case VK_END:
      return NLM_END;
    case VK_INSERT:
      return NLM_INS;
    case VK_DELETE:
      return NLM_DEL;
    case VK_ESCAPE:
      return NLM_ESC;
    }

  return '\0';
}


extern Nlm_Boolean Nlm_ProcessKeydown(Nlm_GraphiC g, WPARAM wParam,
                                      Nlm_Int4 filter)
{
  Nlm_Char ch = Nlm_KeydownToChar( wParam );

  switch ( ch )
    {
    case '\0':
      return FALSE;

    case NLM_LEFT:
    case NLM_RIGHT:
      if (filter & HORIZ_ARROW)
        break;
      return FALSE;

    case NLM_UP:
    case NLM_DOWN:
      if (filter & VERT_ARROW)
        break;
      return FALSE;


    case NLM_HOME:
    case NLM_END:
      if (filter & HORIZ_PAGE)
        break;
      return FALSE;

    case NLM_PREV:
    case NLM_NEXT:
      if (filter & VERT_PAGE)
        break;
      return FALSE;

    default:
      return FALSE;
    }

  Nlm_DoSendFocus(g, ch);
  return TRUE;
}
#endif


#ifdef WIN_MOTIF
typedef struct Nlm_IOstruct
{
  int            fd;
  Nlm_IOtype     type;
  XtInputId      id;
  Nlm_IOcallback ufunc;
  Nlm_VoidPtr    udata;
} Nlm_IOstruct;


static void io_callback(XtPointer client_data, int *source, XtInputId *id)
{
  Nlm_IO io = (Nlm_IO)client_data;
  (*io->ufunc)(io, io->fd, io->type, io->udata);
}

/* register IO callback "ufunc";  return IO handler.
 * NOTE:  "type" cannot be IO_CLEANUP!
 */
extern Nlm_IO Nlm_RegisterIO(int fd,
                             Nlm_IOcallback ufunc, Nlm_VoidPtr udata,
                             Nlm_IOtype type)
{
  Nlm_IO    io;
  XtPointer cond;

  if ( !ufunc )
    return NULL;

  switch ( type )
    {
    case Nlm_IO_READ:
      cond = (XtPointer)XtInputReadMask;
      break;
    case Nlm_IO_WRITE:
      cond = (XtPointer)XtInputWriteMask;
      break;
    case Nlm_IO_ERROR:
      cond = (XtPointer)XtInputExceptMask;
      break;
    default:
      ASSERT ( FALSE );
      return NULL;
    }

  io = (Nlm_IO)Nlm_MemNew( sizeof(Nlm_IOstruct) );
  io->fd    = fd;
  io->type  = type;
  io->ufunc = ufunc;
  io->udata = udata;
  io->id = XtAppAddInput(Nlm_appContext, fd, cond, io_callback, io);
  return io;
}

extern void Nlm_UnregisterIO(Nlm_IO io)
{
  if ( !io )
    return;

  if ( io->ufunc )
    (*io->ufunc)(io, io->fd, Nlm_IO_CLEANUP, io->udata);

  XtRemoveInput( io->id );
  Nlm_MemFree( io );
}
#endif
