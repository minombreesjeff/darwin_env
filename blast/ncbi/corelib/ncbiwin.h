/*   ncbiwin.h
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
* File Name:  ncbiwin.h
*
* Author:  Gish, Kans, Ostell, Schuler
*
* Version Creation Date:   1/1/91
*
* $Revision: 6.9 $
*
* File Description:
*               underlying window toolbox import
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ncbiwin.h,v $
* Revision 6.9  2003/11/07 15:54:44  rsmith
* Remove obsolete and uneeded Macintosh headers Types.h and Windows.h
*
* Revision 6.8  2003/05/05 12:00:10  rsmith
* Change definitions before including windows.h when compiling with Codewarrior for Windows.
*
* Revision 6.7  2002/12/17 23:00:48  kans
* support for OS_UNIX_DARWIN for Mach-O executables (RGS)
*
* Revision 6.6  2002/06/13 16:14:07  kans
* fix includes for OS_UNIX_DARWIN with WIN_MAC (EN)
*
* Revision 6.5  2002/02/15 20:19:50  beloslyu
* bug fixed
*
* Revision 6.4  2002/02/15 19:53:28  beloslyu
* fix for HP-UX
*
* Revision 6.3  1999/12/21 17:34:56  kans
* Added ControlDefinitions.h to the Mac client portion to support universal headers version 3.3 (in preparation for Carbon compatibility) - churchill
*
* Revision 6.2  1999/02/12 16:01:41  vakatov
* Added a draft version of Nlm_GetEnvParamEx()
* Got rid of the old "PROTO" and "LIBCALL" prototype junk, etc.
*
* Revision 6.1  1997/12/12 20:32:55  kans
* several new MacOS includes
*
* Revision 5.6  1997/07/22 19:11:41  vakatov
* Separated Main() from GetArg[svc]() functions;  [WIN_MSWIN] converged
* console and GUI libraries; [for WIN32-DLL] encapsulated global variables
*
* Revision 5.5  1997/01/28 21:19:12  kans
* <Desk.h>, <OSEvents.h> and <GestaltEqu.h> are obsolete in CodeWarrior
*
* Revision 5.4  1996/12/30  15:13:32  vakatov
* [WIN_MSWIN]  Added argument "lpszCmdLine" to Nlm_SetupArguments()
*
* Revision 5.3  1996/12/03  21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
* Revision 5.2  1996/08/21  20:48:55  vakatov
* OS_NT --> WIN32
*
* Revision 5.1  1996/08/19  18:46:25  vakatov
* [WIN32]  Made modifications to let one create console applications
*
* Revision 4.2  1996/02/17  17:19:44  ostell
* added OS_NT to include direct.h
*
* Revision 4.1  1996/02/15  22:00:49  kans
* changed platform symbol back to OS_NT
*
* 06-14-94 Schuler     Add LIBCALL to SetupArguments
* ==========================================================================
*/

#ifndef _NCBIWIN_
#define _NCBIWIN_

#undef VoidPtr
#undef Pointer
#undef Handle
#undef Char
#undef CharPtr
#undef Uchar
#undef UcharPtr
#undef Boolean
#undef BoolPtr
#undef Byte
#undef BytePtr
#undef Int1
#undef Int1Ptr
#undef Uint1
#undef Uint1Ptr
#undef Int2
#undef Int2Ptr
#undef Uint2
#undef Uint2Ptr
#undef Int4
#undef Int4Ptr
#undef Uint4
#undef Uint4Ptr
#undef FloatLo
#undef FloatLoPtr
#undef FloatHi
#undef FloatHiPtr

#ifdef WIN_MAC
#include <Controls.h>
#if UNIVERSAL_INTERFACES_VERSION > 0x0320
#include <ControlDefinitions.h>  /* pjc added 11/20/99 */
#endif
#ifndef OS_UNIX_DARWIN
#include <DiskInit.h>
#include <Packages.h>
#include <SegLoad.h>
#include <StandardFile.h>  /* dgg added 12dec93 */
#endif
#include <Devices.h>
#include <Dialogs.h>
#include <Events.h>
#include <Files.h>
#include <Fonts.h>
#include <Lists.h>
#include <Memory.h>
#include <Menus.h>
#include <OSUtils.h>
#include <Quickdraw.h>
#include <TextEdit.h>
#include <ToolUtils.h>
/*
#include <Types.h>
#include <Windows.h>
*/
#include <Sound.h>
#include <Folders.h>
#endif

	                   /* used in ncbifile.c *****/
#ifdef OS_MAC
#include <Errors.h>
#include <Gestalt.h>
#include <MacMemory.h>
#include <Processes.h>
#endif

#ifdef OS_UNIX_SUN
#include <sys/file.h>
#include <sun/dkio.h>
#include <sys/buf.h>
#include <scsi/targets/srdef.h>
#endif

#ifdef OS_MSWIN
#ifndef WIN16
#include <fcntl.h>         /* for setmode() */
#include <io.h>
#endif
#ifdef COMP_MSC
#include <direct.h>
#endif
#ifdef COMP_BOR
#include <dir.h>
#endif
#endif

#ifdef OS_VMS
#include <stat.h>   /* fstat function and buffer definition */
#include <fab.h>    /* RFM (Record Format) definitions      */
#endif

	                  /* end of ncbifile section */

#ifdef OS_MSWIN
#ifndef WIN16
#undef Beep
#endif
#ifndef COMP_METRO
#undef TRUE
#undef FALSE
#undef NULL
#undef FAR
#undef NEAR
#undef PASCAL
#undef CDECL
#define STRICT
#endif
#define NOKANJI
#define NODBCS
#define NOCOMM
#define NOSOUND
#define NOPROFILER
#define NOKEYBOARDINFO
#include <windows.h>
#include <windowsx.h>
#endif /* OS_MSWIN */

#ifdef WIN_MOTIF
#ifndef WIN_X
#define WIN_X
#endif
#endif

#if defined(WIN_X)
#if !defined(OS_UNIX_HPUX)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xresource.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#endif
#include <X11/cursorfont.h>
#endif

#ifdef WIN_MOTIF
#include <Xm/Xm.h>
#include <Xm/AtomMgr.h>
#include <Xm/BulletinB.h>
#include <Xm/CascadeB.h>
#include <Xm/DrawingA.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/List.h>
#include <Xm/MainW.h>
#include <Xm/MenuShell.h>
#include <Xm/PanedW.h>
#include <Xm/Protocols.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/ScrollBar.h>
#include <Xm/ScrolledW.h>
#include <Xm/Separator.h>
#include <Xm/Text.h>
#include <Xm/TextF.h>
#include <Xm/ToggleB.h>
#endif

#include <time.h>


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif

NLM_EXTERN void Nlm_SetupArguments(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _NCBIWIN_ */
