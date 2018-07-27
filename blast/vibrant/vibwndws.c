/*   vibwndws.c
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
* File Name:  vibwndws.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.43 $
*
* File Description:
*       Vibrant main, event loop, and window functions
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: vibwndws.c,v $
* Revision 6.43  2001/11/26 21:25:04  juran
* Define type AERefCon as SInt32 for AE handler prototypes.
* It may need to be UInt32 for pre-3.4 Universal Interfaces under Carbon, or something.
*
* Revision 6.42  2001/08/29 17:35:38  juran
* Universal Interfaces 3.4 changes:
* AE handler refCon is an SInt32.
* convertClipboardFlag doesn't exist under Carbon.
*
* Revision 6.41  2001/05/14 20:50:09  juran
* Remove Mac clipboard code -- the scrap is updated whenever it's used.
*
* Revision 6.40  2001/04/18 18:39:29  juran
* AE handlers take a UInt32 refCon.
*
* Revision 6.39  2001/04/05 20:32:34  juran
* Major Carbon changes, including the new Scrap Manager (not tested).
*
* Revision 6.38  2000/06/15 20:51:45  vakatov
* Use "const" in Args code
*
* Revision 6.37  2000/03/31 16:14:13  thiessen
* fix modal window lockout bug
*
* Revision 6.36  2000/02/03 22:29:16  lewisg
* make windows version of vibrant use standard color
*
* Revision 6.35  2000/01/13 23:37:14  beloslyu
* changes because of port to HP-UX 11.0
*
* Revision 6.34  2000/01/07 00:22:47  thiessen
* fixes for LessTif and OpenGL X visual selection
*
* Revision 6.33  1999/12/30 16:47:10  kans
* Carbon changes (Churchill)
*
* Revision 6.32  1999/12/21 18:04:24  kans
* removed MPW/THINKC conditional code, starting upgrade to Carbon compatibility - Churchill
*
* Revision 6.31  1999/12/07 19:18:58  thiessen
* fixed font color problem in OpenGL on SGI
*
* Revision 6.30  1999/10/27 20:17:32  thiessen
* when _OPENGL is defined, make Motif choose a better-than-8-bit color depth, if possible
*
* Revision 6.29  1999/07/21 17:57:42  vakatov
* GetArgs_ST():  fixed array boundary read
*
* Revision 6.28  1999/07/08 14:49:26  kans
* MultiLinePrompt copies string to avoid modifying read-only string passed in from GetArgs when Mac virtual memory is on
*
* Revision 6.27  1999/06/22 15:14:53  lewisg
* fix image library so that works on linux with > 8 bits
*
* Revision 6.26  1999/04/22 15:19:01  vakatov
* Call XtUnrealizeWidget() before XtDestroyWidget() to make sure no
* "post-mortem" callbacks(registered by XtAddEventHandler()) get
* triggered for the destroyed widget. Reason: the widget may not be
* immediately destroyed if XtDestroyWidget() was called in a nested
* event dispatch loop.
*
* Revision 6.25  1999/04/06 14:23:27  lewisg
* add opengl replacement for viewer3d
*
* Revision 6.24  1999/03/17 15:10:50  vakatov
* + Nlm_XLoadStandardFont() to find a "last-resort" font
*
* Revision 6.23  1999/02/11 21:34:37  kans
* check __profile__ symbol to turn on Mac profiling
*
* Revision 6.22  1999/02/10 22:22:14  kans
* added commented-out CodeWarrior Profiler setup instructions
*
* Revision 6.21  1999/01/29 18:42:57  kans
* appearancelib and navigationlib ifdef OS_MAC and PROC_PPC
*
* Revision 6.20  1999/01/21 23:58:32  kans
* Mac navigation services code ifdefed out because is available function does not appear to work properly
*
* Revision 6.19  1999/01/06 02:52:00  kans
* support for Mac Navigation Services file selection dialogs
*
* Revision 6.18  1998/12/14 18:39:11  kans
* okayToDrawContents extern, used to inhibit slate drawing
*
* Revision 6.17  1998/12/10 21:28:50  kans
* okayToDrawContents accessed only in WIN_MAC section
*
* Revision 6.16  1998/12/10 17:45:09  kans
* okayToDrawContents to suppress drawing after return from Main
*
* Revision 6.15  1998/12/01 17:41:58  vakatov
* Use just "0" instead of "(caddr_t)NULL" -- (a compilation fix)
*
* Revision 6.14  1998/07/14 16:44:29  vakatov
* Added VibrantIsGUI() and <internal> Nlm_VibrantSetGUI()
*
* Revision 6.13  1998/03/22 03:01:11  kans
* changed names to RegisterServiceProc and RegisterResultProc
*
* Revision 6.12  1998/03/22 02:33:30  kans
* added request proc, result proc, message handlers to support, and send open doc event, launch app now work with file names or signatures
*
* Revision 6.11  1998/03/09 21:03:47  vakatov
* FetchArg(), float|int:  fixed for assignment of default value
*
* Revision 6.10  1998/01/08 21:33:32  kans
* OS_AXP_VMS includes <Xm/StdCmap.h>
*
* Revision 6.9  1997/11/27 05:52:16  vakatov
* [WIN_X] "defaultXerror_handler()" -- fixed non-portable(internal X11) code
*
* Revision 6.7  1997/11/21 19:31:23  vakatov
* [WIN_X] "defaultXerror_handler()" -- issue a std X error msg, don't crash
*
* Revision 6.6  1997/11/12 20:56:14  kans
* added SetMouseMoveCallback and SetMouseMoveRegion, implemented first on Mac
*
* Revision 6.5  1997/10/30 19:30:47  kans
* added Nlm_DisplayEnvironmentVariables for Solaris
*
* Revision 6.4  1997/10/01 21:51:52  vakatov
* Trying to catch strayed Expose or MapNotify(for SetVisible(...)) event
*
* Revision 6.3  1997/09/29 20:00:51  vakatov
* [WIN_X11]  Do not parse "-d" cmd.-line, use "-dpy" instead(or "-display")
*
* Revision 6.2  1997/09/22 23:51:23  vakatov
* [WIN_MSWIN] ParseSetupArguments(): remove leading quote from cmd-line args
*
* Revision 6.1  1997/09/17 21:09:05  vakatov
* [WIN_X]  Parse command-line "-d" / "-display" options to choose X-display
*
* Revision 6.0  1997/08/25 18:58:01  madden
* Revision changed to 6.0
*
* Revision 5.71  1997/08/14 18:54:45  vakatov
* [WIN_MOTIF] Let X11 parse the command-line args at first;  leave the
* rest(not fetched by X11) for NCBI-wise arg. processing
*
* Revision 5.70  1997/08/08 14:43:08  vakatov
* [WIN_X] Nlm_XCheckUpdateEvent() -- check for mapping and expose events
*
* Revision 5.69  1997/08/01 16:47:32  vakatov
* [WIN_MOTIF,WIN_MSWIN]  Added IconifyWindow() and IconicWindow()
*
* Revision 5.68  1997/07/23 16:39:02  vakatov
* [WIN_MSWIN] ParseSetupArguments() -- fixed a quote("') handling bug
*
* Revision 5.67  1997/07/22 19:12:17  vakatov
* Separated Main() from GetArg[svc]() functions;  [WIN_MSWIN] converged
* console and GUI libraries; [for WIN32-DLL] encapsulated global variables
*
* Revision 5.66  1997/07/18 17:01:17  vakatov
* [WIN_MOTIF]  Removed "arg[cv]";  use "Nlm_GetArg[cv]()" instead
*
* Revision 5.65  1997/07/14 19:31:04  vakatov
* [WIN_X] Nlm_NewWindow() -- use common Vibrant colormap(and visual, and
* depth) if failed to create Nlm_BusyColormap()
*
* Revision 5.64  1997/07/14 16:25:02  vakatov
* Nlm_RemoveDyingWindows() -- look for Nlm_currentXWindow starting from
* the shell level(was -- from the BullBoard level)
*
* Revision 5.63  1997/07/10 21:49:50  vakatov
* [WIN_X]  Now able to manage windows having different depths(and
* different visuals and GC).
*
* Revision 5.62  1997/07/02 18:46:07  vakatov
* [WIN_X11]  If non-default colormap is used for a window -- make the
* window to use the relevant visual(PseudoColor or GrayScale)
*
* Revision 5.61  1997/06/24 18:20:41  vakatov
* Keep Nlm_dialogTextHeight value in-sync with the "vibtexts.c" R5.8 fixes
*
* Revision 5.60  1997/06/23 22:10:52  vakatov
* [WIN_MOTIF,WIN_MSWIN]  GetArgs_ST() -- parse command line arguments
* close to the console version
*
* Revision 5.59  1997/06/19 20:34:58  vakatov
* [WIN_MSWIN]  Do not set the window class cursor to IDC_ARROW -- to avoid
* confusing with the alternative cursors set by Vibrant application
*
* Revision 5.58  1997/06/18 20:00:27  kans
* removed extra parenthesis
*
* Revision 5.57  1997/06/18 19:05:54  kans
* protect against NULL curarg->to or curarg->from
*
* Revision 5.56  1997/06/09 21:54:49  vakatov
* [WIN_X] #include <X11/Xatom.h> to fit old versions of X11
*
* Revision 5.55  1997/06/09 18:53:39  vakatov
* [WIN_X]  Use(create if necessary) standard RGB_DEFAULT_MAP colormap as
* the Vibrant default colormap, if possible.  Nlm_XAllocColor() moved from
* "ncbidraw.c" and made external.
*
* Revision 5.54  1997/05/27 21:51:00  vakatov
* Added Nlm_PopupParentWindow() to popup window w/o switching input focus
*
* Revision 5.53  1997/05/23 17:32:53  kans
* added GetArgv, GetArgc for Mac
*
* Revision 5.52  1997/05/16 21:00:12  vakatov
* [WIN_X11]  WindowStructFocusCallback() -- replaced XtFree() by XFree()
*
* Revision 5.51  1997/05/16 16:44:47  vakatov
* [WIN_MSWIN,WIN32]  Addition to R5.49 -- do not abort() the failed
* application; just popup an error message and then rely on the default
* exception handler
*
* Revision 5.50  1997/05/15 15:14:17  vakatov
* [WIN_MSWIN] Tune the StaticPrompt, etc. background color to match the
* parent window background color
*
* Revision 5.49  1997/05/08 15:45:05  vakatov
* [WIN_MSWIN,WIN32]  Abort() application in the case of unhandled
* exception -- to prevent endless loop with the diagnostic dialog box
*
 * Revision 5.48  1997/05/06  19:00:16  epstein
 * don't modify window title if environment variable is 0; this is a workaround for subtool
 *
 * Revision 5.47  1997/04/29  14:45:13  epstein
 *  corrections to tagged window titles
 *
 * Revision 5.46  1997/04/28  19:36:49  epstein
 * when VIBRANT_USE_APPLICATION_NAME environment variable is set, prepend program name unto each window
 *
 * Revision 5.45  1997/04/25  16:11:28  vakatov
 * [WIN_MOTIF] Register navigation key translation actions
 * [WIN_MOTIF, WIN_MSWIN] Catch and render(DoSendFocus) navigation key events
 *
 * Revision 5.44  1997/04/17  16:17:50  kans
 * InitForms and FreeForms called
 *
 * Revision 5.43  1997/03/19  16:00:18  vakatov
 * Added QuittingProgram() -- useful to timely break internal event loops
 *
 * Revision 5.42  1997/02/28  20:10:03  vakatov
 * Nlm_CleanUpWindows():  call to Nlm_RemoveDyingWindows() at first!
 *
 * Revision 5.41  1997/02/28  19:09:58  vakatov
 * [NLM_RISKY] Nlm_Add/DelSubwindowShell():  set "allShells" properly in
 * order to avoid discrepancy with later use of Nlm_GetWindowData()
 *
 * Revision 5.40  1997/02/21  19:24:16  vakatov
 * Do not apply fallback resources when running on a B&W screen
 *
 * Revision 5.39  1997/02/13  22:26:30  vakatov
 * Nlm_CleanUpWindows(), Nlm_RemoveDyingWindows() -- remove the window
 * menubar *after*(not *before*) removal of other window children
 * [WIN_MOTIF] Nlm_RemoveDyingWindows() -- let the removed shell be
 * dangling(see R5.38) only if NLM_MOTIF_CASCADEB_BUG is set
 *
 * Revision 5.38  1997/01/14  22:09:10  vakatov
 * Workaround for the MOTIF/X11 bug(see also "vibmenus.c", Rev. 5.17) --
 * do not completely destroy the shells having menubars -- let them
 * dangling. It still looks better than a program crash...
 * Fixed inaccurate string copying -- <mostly potential> 1-byte exceeding of
 * the string size by StringNCat;  missing terminating '\0' by StringNCpy.
 * Removed some unused variables;  set preprocessor conditions more
 * thoroughly to exclude some functions from the compilation on some platforms.
 *
 * Revision 5.37  1997/01/02  16:03:19  vakatov
 * [WIN_MOTIF]  Revert input focus to "PointerRoot" rather than to "None"
 *
 * Revision 5.36  1996/12/30  17:42:38  vakatov
 * [WIN_MOTIF]  Fixed Nlm_ProcessKeyPress() -- do not put uninitialized(stack-
 * located) input arguments to the XLookupString();  use NULL instead.
 *
 * Revision 5.35  1996/12/30  15:12:35  vakatov
 * Made all argument conversions be performed in the <new> FetchArg() routine.
 * Nlm_GetArgs() function rearranged:  added command-line arguments parsing
 * for MS-Windows and let one avoid posting of the arg-query dialog if all
 * arguments can be resolved by default or using the program command line.
 *
 * Revision 5.34  1996/12/13  17:27:38  kans
 * added GetArgv and GetArgc for WIN_MOTIF only
 *
 * Revision 5.33  1996/12/03  18:30:05  vakatov
 * [WIN_MSWIN]  Added (static) function win2client() to calculate client
 * rectangle on the base of the known top-level window rectangle.  Added
 * (static) function OnSizeMove() to re-implement(and fix) the MyCls_OnSize()
 * and MyCls_OnMove() window callbacks.
 *
 * Revision 5.32  1996/11/22  19:15:13  vakatov
 * [WIN_MSWIN]  Nlm_SetColorCell():  invalidate rather than animate
 *
 * Revision 5.31  1996/11/12  21:59:46  vakatov
 * [WIN_MOTIF]  Nlm_ResizeWindow():  count the menubar height when SetRect()
 *
 * Revision 5.30  1996/11/07  15:25:52  vakatov
 * [WIN_MOTIF]  IsXParentOf():  test for "children" != NULL
 *
 * Revision 5.29  1996/11/05  23:32:35  vakatov
 * [WIN_MSWIN]  Fit window into the screen borders on the window creation
 * or resizing;  changed the style definition(but not appearance) of
 * ShadowWindow; +extensive type casting
 *
 * Revision 5.28  1996/10/30  16:06:42  vakatov
 * Nlm_XrmGetResource1():  replaced XrmGetDatabase() by XtDatabase() to
 * suit X11R4 function set.
 *
 * Revision 5.27  1996/10/29  22:09:29  vakatov
 * [WIN_MOTIF]  Nlm_VibrantDefaultColormap():  Replaced X resource value
 * converters(which are absent in X releases < X11R5) by the hand-written
 * code; and made the "privateColormap" resorce value "false" by default.
 *
 * Revision 5.26  1996/10/28  21:24:24  vakatov
 * [WIN_MOTIF][_DEBUG]  Removed extra debug print
 *
 * Revision 5.25  1996/10/28  19:32:14  vakatov
 * [WIN_MOTIF]
 * Nlm_BusyColorMap():  added special case to create new colormap where
 * only specified number of colorcells "inherited" from the default
 * will be allocated.
 * Added Nlm_XrmGetResource2(), Nlm_XrmGetResource1() and
 * Nlm_XrmGetResource() functions and modified Nlm_SetupWindows()
 * to handle user-specified X-resources.
 * Added Nlm_VibrantDefaultColormap() to initialize the application
 * colormap according to the newly introduced Vibrant/X11 recources
 * "nPrivateColormap" and "nInheritColors".
 * Made a lot of type castings to get rid of all compiler warnings.
 *
 * Revision 5.24  1996/10/21  21:38:16  vakatov
 * [WIN_MOTIF]  Renamed WindowResizeCallback() to WindowStructFocusCallback()
 * and made it call DoActivate/DoDeactivate when accepting/loosing
 * input focus -- to sync. with [WIN_MSWIN] behaviour.  Removed explicit
 * (not quite correct) calls to DoActivate/DoDeactivate.
 *
 * Revision 5.23  1996/10/21  15:21:11  vakatov
 * [WIN_MOTIF]  WindowResizeCallback() <on the mapping event>:  protected
 *              from setting input focus to yet(or already) unmapped window
 *
 * Revision 5.22  1996/10/01  16:11:27  kans
 * wasn't setting ctrlKey on Mac
 *
 * Revision 5.21  1996/09/26  00:40:05  kans
 * on remove window, immediately null out window timer
 *
 * Revision 5.20  1996/09/26  00:29:23  kans
 * added SetWindowTimer
 *
 * Revision 5.19  1996/08/29  20:46:46  vakatov
 * [WIN_MSWIN]  Synchronized Nlm_Get- and Nlm_SetWindowPosition
 *
 * Revision 5.18  1996/08/27  16:34:24  vakatov
 * [WIN_MOTIF]  Nlm_SelectWindow() : fixed a case when it did not set input
 * focus to the selected window sometimes.
 *
 * Revision 5.17  1996/08/19  21:24:00  vakatov
 * [WIN_MOTIF]  Set default ToggleButton's foreground to "blue" --
 * "dark blue" is absent sometimes, somewhere.
 *
 * Revision 5.16  1996/07/26  17:56:57  vakatov
 * [WIN_MOTIF]  Nlm_NewWindow() : had to specify the MainWindow's colormap
 * explicitly to make LINUX/MOO-TIFF/X11 properly accept the colormap
 *
 * Revision 5.15  1996/07/23  21:36:05  vakatov
 * [WIN_MOTIF]  Nlm_BusyColorMap() : do not redefine 0th color
 * [WIN_MOTIF]  Nlm_HideWindow()   : do not change input focus is it
 * does not belong to the window being hided
 *
 * Revision 5.14  1996/07/19  19:44:51  vakatov
 * [WIN_MOTIF]  Nlm_RemoveDyingWindows() : set Nlm_currentXWindow to NULL
 * if it points to either the dying window or one of its children;
 * added new function IsXParentOf() determining whether the given window
 * is the parent of another one.
 *
 * Revision 5.13  1996/07/18  16:49:03  vakatov
 * [WIN_MOTIF]  Nlm_SelectWindow() : Removed extra call of XSetInputFocus()
 *
 * Revision 5.12  1996/07/17  16:24:05  vakatov
 * [WIN_MOTIF]  Nlm_Hide()/WM/Nlm_Select() case -- the input focus quirk fixed
 *
 * Revision 5.11  1996/06/24  17:00:21  vakatov
 * [WIN_MOTIF]  Nlm_Select() now seems to work properly for all cases
 *
 * Revision 5.10  1996/06/24  16:05:08  vakatov
 * [WIN_MOTIF]  Fixed bug in Nlm_Select()
 *
 * Revision 5.9  1996/06/19  21:15:28  vakatov
 * [WIN_MOTIF]  Made Nlm_Select() to select(pop-up) the specified window
 * in most cases; there is still a problem when Nlm_Select is called just AFTER
 * (not before) hiding of another Vibrant window which has current input focus --
 * some WMs are "too smart" and in some special conditions takes control over
 * the "focus inheritance" after the dying window.
 *
 * Revision 5.8  1996/06/18  21:57:42  vakatov
 * [WIN_MOTIF]  Made ModalWindow to be really modal
 *
 * Revision 5.7  1996/06/17  21:57:17  kans
 * InvalObject in Nlm_SetColorCell on Mac
 *
 * Revision 5.6  1996/06/17  17:00:47  vakatov
 * [WIN_MSWIN]  The shell window's border/caption/menu-bar dimensions are to
 * be taken into account inside the Nlm_ResizeWindow() function
 *
 * Revision 5.5  1996/06/14  14:32:24  vakatov
 * [WIN_MOTIF] Added cMap_fixed field to WindowData structure to indicate
 * fixed(having reserved cells for Motif colors) colormap attached to
 * the window.
 * SetColorMap() function partially rewritten to treate the fixed colormap
 * under WIN_MOTIF and to reserve colorcells for animation under WIN_MSWIN.
 * Added Nlm_SetColorCell() -- to immediately change the color appearence.
 * [WIN_MOTIF]  Added Nlm_RestrictMotifColorsTo() -- to avoid Motif/3D-Viewer
 * color overlapping; it is now called inside "Nlm_NewWindow()" before
 * initializing Motif resources
 *
 * Revision 5.3  1996/06/03  19:12:32  vakatov
 * [WIN_MOTIF]  Added hard-coding fallbacks to provide default set of
 * basic colors in Vibrant-based applications
 *
 * Revision 5.2  1996/05/30  20:06:07  vakatov
 * [WIN_MSWIN]  <Shift> and <Ctrl> key status checking added to MainProc()
 *
 * Revision 5.1  1996/05/30  14:02:46  vakatov
 * [WIN_MOTIF]  Corrected fallback resources to provide proper setting of the
 * background color under CDE
 *
 * Revision 4.29  1996/05/21  13:58:06  epstein
 * fix type for XtFree() calls
 *
 * Revision 4.28  1996/05/21  12:32:16  kans
 * prototypes for add, del subwindow shell
 *
 * Revision 4.27  1996/05/20  21:38:13  vakatov
 * [WIN_MOTIF]  All "Widget" / "NULL" comparisons and assignments replaced
 * by the "Widget" / "(Widget)0" these
 *
 * Revision 4.26  1996/05/13  16:00:50  vakatov
 * WindowResizeCallback() modified to take into account the shell window
 * repositioning -- it is usually made by window manager when framing the
 * shell window with the WM-specific window border and title bar.
 *
 * Revision 4.24  1996/05/06  18:28:03  vakatov
 * Nlm_SetWindowDefaultButton() modified;  some extra stuff removed
 *
 * Revision 4.23  1996/05/03  21:03:43  kans
 * removed erroneous textscrapfull = FALSE lines
 *
 * Revision 4.22  1996/05/03  20:27:25  kans
 * put back scrap fixes
 *
 * Revision 4.21  1996/05/03  20:05:22  vakatov
 * [WIN_MOTIF/_DEBUG]  Added event handler to allow the Vibrant windows
 * to interact with the standard X-utility "editres";
 * this utility provides runtime read/write access to all widget attributes
 *
 * Revision 4.20  1996/05/02  20:59:16  kans
 * sets textScrapFull flag properly on startup, resume
 *
 * Revision 4.19  1996/04/30  19:52:19  vakatov
 * GetPosition() now returns actual position for a top-level window having menu bar
 *
 * Revision 4.18  1996/04/30  14:27:29  kans
 * copies text clipboard into text scrap on startup (Mac)
 *
 * Revision 4.17  1996/04/25  17:06:48  vakatov
 * Nlm_ShowDocument():   extraw = extrah = 0;  for WIN_MSWIN
 *
 * Revision 4.16  1996/04/12  19:26:14  vakatov
 * "WindowResizeCallback()" corrected to provide regular extraction of
 * new window position (for the case of the window resizing --
 * rather than just moving)
 *
 * Revision 4.15  1996/03/27  23:10:13  vakatov
 * Function Nlm_SetCursorShape() added to let programmer
 * easily store/restore current cursor shape
 *
 * Revision 4.14  1996/03/19  23:19:36  kans
 * used proper gestalt selector, test, before registering apple events
 *
 * Revision 4.13  1996/03/13  21:40:00  epstein
 * export command-line arguments
 *
 * Revision 4.12  1996/03/12  20:06:10  epstein
 * fix drag-and-drop for Windows
 *
 * Revision 4.11  1996/03/11  23:25:23  kans
 * apple event open file handler added (modified from RasMol code)
 *
 * Revision 4.10  1996/03/11  20:37:56  epstein
 * add support for Drag & Drop
 *
 * Revision 4.9  1996/03/08  16:10:43  vakatov
 * Made Nlm_Update() for Motif/X11 to act more like that for
 * MS-Windows and Mac.  However, it still avoid automatic (without a
 * Nlm_Update() call) recursive synchronization...
 *
 * Revision 4.8  1996/03/07  15:55:35  kans
 * invalidate whole content of resized window in mswin version (DV)
 *
 * Revision 4.7  1996/03/02  22:36:38  kans
 * reduction of X traffic (DV)
 *
 * Revision 4.6  1996/02/13  17:24:07  kans
 * accelerated set position prior to realization (Denis Vakatov)
 *
 * Revision 4.5  1996/01/19  00:55:46  kans
 * UseWindow sets theWindow, returned (on the Mac) by ActiveWindow
 *
 * Revision 4.4  1995/11/29  15:45:23  smirnov
 * Position of window (MOTIF only)
 *
 * Revision 4.3  1995/11/28  19:02:36  smirnov
 * GetWindowRect()/GetClientRect problem (AS) (MS-Windows only)
 *
 * Revision 4.2  1995/11/08  23:30:31  kans
 * removed edit block fields, which belong in the application
 *
 * Revision 4.1  1995/09/06  20:30:24  kans
 * fixed bug due to GetWindowRect vs. GetClientRect (AS + GS)
 *
 * Revision 2.88  1995/07/24  20:03:03  kans
 * removed duplicate ClearEditProcs, reference X11 colormap as integer
 *
 * Revision 2.87  1995/07/19  20:42:23  kans
 * window rectangle takes Motif menu bar height into account
 *
 * Revision 2.86  1995/06/28  21:48:57  kans
 * added Add and Del SubwindowShell (Alex)
 *
 * Revision 2.85  1995/06/13  16:30:38  kans
 * Motif version of GetArgs can parse remaining arguments in lieu of dialog
 *
 * Revision 2.83  1995/05/31  21:24:29  kans
 * XFreeColormap fix (AS)
 *
 * Revision 2.82  1995/05/31  18:00:58  kans
 * added SetColorMap and related code (AS)
 *
* 08-24-94 Schuler     Change try and except to __try and _except for
*                      compatibility with Visual C 2.0
*
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>

#ifdef WIN_MAC
#include <Appearance.h>
#include <Navigation.h>
#include <Profiler.h>
# include "MoreCarbonAccessors.h"
#endif

#if defined(WIN_MOTIF) && defined(_DEBUG) && !defined(__hpux)
#include <X11/Xmu/Editres.h>
#endif

#ifdef WIN_X
#include <X11/Xatom.h>
#ifdef OS_AXP_VMS
#include <Xm/StdCmap.h>
#else
#if !defined(__hpux)
#include <X11/Xmu/StdCmap.h>
#endif /* !defined(__hpux) */
#endif
#endif


#ifdef WIN_MAC
#ifndef Nlm_WindowTool
#define Nlm_WindowTool  WindowPtr
#endif
#ifndef Nlm_PortTool
#define Nlm_PortTool    GrafPtr
#endif
#ifndef Nlm_ShellTool
#define Nlm_ShellTool   Nlm_Handle
#endif
#ifndef Nlm_MainTool
#define Nlm_MainTool    Nlm_Handle
#endif
#endif

#ifdef WIN_MSWIN
#ifndef Nlm_WindowTool
#define Nlm_WindowTool  HWND
#endif
#ifndef Nlm_PortTool
#define Nlm_PortTool    HDC
#endif
#ifndef Nlm_ShellTool
#define Nlm_ShellTool   Nlm_Handle
#endif
#ifndef Nlm_MainTool
#define Nlm_MainTool    Nlm_Handle
#endif
#endif

#ifdef WIN_MOTIF
#ifndef Nlm_WindowTool
#define Nlm_WindowTool  Widget
#endif
#ifndef Nlm_PortTool
#define Nlm_PortTool    GC
#endif
#ifndef Nlm_ShellTool
#define Nlm_ShellTool   Widget
#endif
#ifndef Nlm_MainTool
#define Nlm_MainTool    Widget
#endif
#endif

typedef  struct  Nlm_shelldata {
  struct Nlm_shelldata PNTR next;
  Nlm_ShellTool             shell;
} Nlm_ShellData, PNTR Nlm_ShellDataPtr;

typedef  struct  Nlm_windowdata {
  Nlm_WindowTool   handle;
  Nlm_ShellTool    shell;
  Nlm_PortTool     port;
  Nlm_MainTool     main;
  Nlm_ColorMTool   cMap;
  Nlm_ShellDataPtr allShells;
  Nlm_Int2         cMapStatus;
  Nlm_RecT         dragArea;
  Nlm_RecT         growArea;
  Nlm_RecT         zoomArea;
  Nlm_WndActnProc  close;
  Nlm_WndActnProc  activate;
  Nlm_WndActnProc  deactivate;
  Nlm_WndActnProc  resize;
  Nlm_DisplaY      charDisplay;
  Nlm_MenuBaR      menuBar;
  Nlm_ButtoN       defaultButton;
  Nlm_Boolean      notYetShown;
  Nlm_Boolean      dying;
  Nlm_CharPtr      title;
  Nlm_VoidPtr      data;
  Nlm_WndFreeProc  cleanup;
  Nlm_WndActnProc  timer;
  Nlm_WindoW       modalOwner; /* esl++ to support nested modal dialogs */
#ifdef WIN_MOTIF
  Visual      *visual;
  Nlm_Boolean cMap_fixed;
#endif
#ifdef WIN_MSWIN
  Nlm_Uint4        style;
  Nlm_Uint4        ex_style;
#endif
} Nlm_WindowData;

typedef  struct  Nlm_windowrec {
  Nlm_BoxRec      boxR;
  Nlm_WindowData  window;
} Nlm_WindowRec, PNTR Nlm_WndPtr;

Nlm_WindoW   Nlm_desktopWindow;
Nlm_WindoW   Nlm_systemWindow;

Nlm_RecT     Nlm_screenRect;

Nlm_WindoW   Nlm_theWindow;

extern Nlm_Boolean okayToDrawContents;
Nlm_Boolean okayToDrawContents = TRUE; /* set to false by CleanUpWindows */

extern Nlm_Boolean Nlm_usesMacNavServices;
Nlm_Boolean Nlm_usesMacNavServices = FALSE;

#ifdef WIN_MAC
EventRecord  Nlm_currentEvent;
#ifdef __CONDITIONALMACROS__
/* Global RoutineDescriptors - from RasMol */
AEEventHandlerUPP HandleAEIgnorePtr;
AEEventHandlerUPP HandleAEOpenDocPtr;
AEEventHandlerUPP HandleAEQuitAppPtr;
AEEventHandlerUPP HandleAEAnswerPtr;
#endif
typedef SInt32 AERefCon;
//typedef UInt32 AERefCon;
#endif

#ifdef WIN_MSWIN
WPARAM       Nlm_currentWParam;
HINSTANCE    Nlm_currentHInst;
LPARAM       Nlm_currentLParam;
MSG          Nlm_currentMssg;
int          Nlm_currentId;
HWND         Nlm_currentHwndCtl;
UINT         Nlm_currentCode;
int          Nlm_currentPos;
#endif

#ifdef WIN_MOTIF
XEvent        Nlm_currentEvent;
XtAppContext  Nlm_appContext;
XmFontList    Nlm_XfontList;
Widget        Nlm_fileDialogShell;

static const char Nlm_VibName[]            = "vibrant";
static const char Nlm_VibClass[]           = "Vibrant";
static       char Nlm_AppName[PATH_MAX+1]  = "vibrant";
#endif

Nlm_WindowTool  Nlm_currentWindowTool;
Nlm_Boolean     Nlm_processUpdatesFirstVal = TRUE;

static Nlm_GphPrcsPtr  gphprcsptr = NULL;

static Nlm_GphPrcsPtr  documentProcs;
static Nlm_GphPrcsPtr  fixedProcs;
static Nlm_GphPrcsPtr  frozenProcs;
static Nlm_GphPrcsPtr  roundProcs;
static Nlm_GphPrcsPtr  alertProcs;
static Nlm_GphPrcsPtr  modalProcs;
static Nlm_GphPrcsPtr  movableModalProcs;
static Nlm_GphPrcsPtr  floatingProcs;
static Nlm_GphPrcsPtr  shadowProcs;
static Nlm_GphPrcsPtr  plainProcs;
static Nlm_GphPrcsPtr  desktopProcs;
static Nlm_GphPrcsPtr  systemProcs;

static Nlm_Boolean     quitProgram;
static Nlm_Boolean     getArgsBoxUp;
static Nlm_Boolean     getArgsOk;

static Nlm_WindoW      recentWindow = NULL;
static Nlm_WindowData  recentWindowData;

#ifdef WIN_MAC
static Nlm_WindoW      chosenWindow;
static Nlm_WindoW      frontWindow;
static Nlm_WindoW      theActiveWindow;
#endif

static Nlm_WindoW      dyingWindow;
static Nlm_RecT        screenBitBounds;

#ifndef WIN_MOTIF
static Nlm_Boolean     inNotice;
#endif

static Nlm_VoidProc    timerAction = NULL;
static Nlm_KeyProc     keyAction = NULL;
static Nlm_ClckProc    mouseMovedAction = NULL;

static Nlm_WndActnProc  appWndAct = NULL;
static Nlm_WndActnProc  appWndDeact = NULL;

static Nlm_DropProc        registeredDropProc = NULL;
static Nlm_ServiceProc  registeredServiceProc = NULL;
static Nlm_ResultProc   registeredResultProc = NULL;

#ifdef WIN_MAC
static Cursor       cross;
static Cursor       iBeam;
static Cursor       plus;
static Cursor       watch;
static Nlm_Int4     lastTimerTime;
static Nlm_Boolean  hasColorQD = FALSE;
#endif

#ifdef WIN_MSWIN
HCURSOR       Nlm_currentCursor;
static UINT          timerID;
static Nlm_Char      windowclass [32];
static int           discard_count = 0;
static Nlm_Boolean   handlechar;
#endif

#ifdef WIN_MOTIF
Cursor        Nlm_currentCursor;
static Cursor        arrow;
static Cursor        cross;
static Cursor        iBeam;
static Cursor        plus;
static Cursor        watch;
static XFontStruct   *font;
static XtIntervalId  windowTimer;
static Widget        selectShell;
#endif

#ifdef WIN_MOTIF
#ifdef MISSING_X_SYMBOLS_BUG
int get_wmShellWidgetClass (void)

{
}

int get_applicationShellWidgetClass (void)

{
}

int XShapeCombineMask (void)

{
}
#endif
#endif

static void Nlm_LoadWindowData (Nlm_WindoW w, Nlm_WindowTool hdl,
                                Nlm_ShellTool shl, Nlm_PortTool prt,
                                Nlm_MainTool man, Nlm_RectPtr drag,
                                Nlm_RectPtr grow, Nlm_RectPtr zoom,
                                Nlm_WndActnProc closeProc, Nlm_WndActnProc actProc,
                                Nlm_WndActnProc deactProc, Nlm_WndActnProc resiz,
                                Nlm_DisplaY charDisp, Nlm_MenuBaR mnuBar,
                                Nlm_ButtoN dfltBtn, Nlm_Boolean notYet,
                                Nlm_Boolean die, Nlm_CharPtr ttl,
                                Nlm_VoidPtr dat, Nlm_WndFreeProc cln,
                                Nlm_WndActnProc tmr)

{
  Nlm_WindowData  PNTR wdptr;
  Nlm_WndPtr      wp;

  if (w != NULL) {
    wp = (Nlm_WndPtr) Nlm_HandLock (w);
    wdptr = &(wp->window);
    wdptr->handle = hdl;
    wdptr->shell = shl;
    wdptr->port = prt;
    wdptr->main = man;
    if (drag != NULL) {
      wdptr->dragArea = *drag;
    } else {
      Nlm_LoadRect (&(wdptr->dragArea), 0, 0, 0, 0);
    }
    if (grow != NULL) {
      wdptr->growArea = *grow;
    } else {
      Nlm_LoadRect (&(wdptr->growArea), 0, 0, 0, 0);
    }
    if (zoom != NULL) {
      wdptr->zoomArea = *zoom;
    } else {
      Nlm_LoadRect (&(wdptr->zoomArea), 0, 0, 0, 0);
    }
    wdptr->close = closeProc;
    wdptr->activate = actProc;
    wdptr->deactivate = deactProc;
    wdptr->resize = resiz;
    wdptr->charDisplay = charDisp;
    wdptr->menuBar = mnuBar;
    wdptr->defaultButton = dfltBtn;
    wdptr->notYetShown = notYet;
    wdptr->dying = die;
    wdptr->title = ttl;
    wdptr->data = dat;
    wdptr->cleanup = cln;
    wdptr->timer = tmr;
    wdptr->modalOwner = NULL; /* esl++ */
#ifdef WIN_MOTIF
    wdptr->visual = NULL;
    wdptr->cMap_fixed = FALSE;
#endif
    Nlm_HandUnlock (w);
    recentWindow = NULL;
  }
}

static void Nlm_SetWindowData (Nlm_WindoW w, Nlm_WindowData * wdata)

{
  Nlm_WndPtr  wp;

  if (w != NULL && wdata != NULL) {
    wp = (Nlm_WndPtr) Nlm_HandLock (w);
    wp->window = *wdata;
    Nlm_HandUnlock (w);
    recentWindow = w;
    recentWindowData = *wdata;
  }
}

static void Nlm_GetWindowData (Nlm_WindoW w, Nlm_WindowData * wdata)

{
  Nlm_WndPtr  wp;

  if (w != NULL && wdata != NULL) {
    if (w == recentWindow && NLM_RISKY) {
      *wdata = recentWindowData;
    } else {
      wp = (Nlm_WndPtr) Nlm_HandLock (w);
      *wdata = wp->window;
      Nlm_HandUnlock (w);
      recentWindow = w;
      recentWindowData = *wdata;
    }
  }
}

static Nlm_WindowTool Nlm_GetWindowPtr (Nlm_WindoW w)

{
  Nlm_WindowTool  rsult;
  Nlm_WindowData  wdata;

  rsult = (Nlm_WindowTool) 0;
  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    rsult = wdata.handle;
  }
  return rsult;
}

extern Nlm_ShellTool Nlm_GetWindowShell (Nlm_WindoW w)

{
  Nlm_ShellTool   rsult;
  Nlm_WindowData  wdata;

  rsult = (Nlm_ShellTool) 0;
  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    rsult = wdata.shell;
  }
  return rsult;
}


extern void Nlm_AddSubwindowShell (Nlm_WindoW w, Nlm_ShellTool shell );
extern void Nlm_AddSubwindowShell (Nlm_WindoW w, Nlm_ShellTool shell )
{
  Nlm_WindowData   wdata;
  Nlm_ShellDataPtr sptr;

  sptr = (Nlm_ShellDataPtr)MemNew( sizeof(Nlm_ShellData) );
  sptr->shell = shell;

  Nlm_GetWindowData(w, &wdata);
  sptr->next = wdata.allShells;
  wdata.allShells = sptr;
  Nlm_SetWindowData(w, &wdata);
}


extern void Nlm_DelSubwindowShell (Nlm_WindoW w, Nlm_ShellTool shell );
extern void Nlm_DelSubwindowShell (Nlm_WindoW w, Nlm_ShellTool shell )
{
  Nlm_WindowData   wdata;
  Nlm_ShellDataPtr sptr;
  Nlm_ShellDataPtr sPrevptr = NULL;

  Nlm_GetWindowData(w, &wdata);
  for (sptr = wdata.allShells;
       sptr != NULL  &&  sptr->shell != shell;
       sPrevptr = sptr, sptr = sptr->next) continue;
  if (sptr == NULL)
    return;

  if (sPrevptr == NULL)
    wdata.allShells = sptr->next;
  else
    sPrevptr->next = sptr->next;

  MemFree( sptr );
  Nlm_SetWindowData(w, &wdata);
}


static Nlm_PortTool Nlm_GetWindowPort (Nlm_WindoW w)

{
  Nlm_PortTool    rsult;
  Nlm_WindowData  wdata;

  rsult = NULL;
  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    rsult = wdata.port;
  }
  return rsult;
}

static Nlm_MainTool Nlm_GetWindowMain (Nlm_WindoW w)

{
  Nlm_MainTool    rsult;
  Nlm_WindowData  wdata;

  rsult = (Nlm_MainTool) 0;
  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    rsult = wdata.main;
  }
  return rsult;
}

extern void Nlm_SetWindowCharDisplay (Nlm_WindoW w, Nlm_DisplaY d)

{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    wdata.charDisplay = d;
    Nlm_SetWindowData (w, &wdata);
  }
}

extern Nlm_DisplaY Nlm_GetWindowCharDisplay (Nlm_WindoW w)

{
  Nlm_DisplaY     rsult;
  Nlm_WindowData  wdata;

  rsult = NULL;
  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    rsult = wdata.charDisplay;
  }
  return rsult;
}

extern void Nlm_SetWindowMenuBar (Nlm_WindoW w, Nlm_MenuBaR mb)

{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    wdata.menuBar = mb;
    Nlm_SetWindowData (w, &wdata);
  }
}

extern Nlm_MenuBaR Nlm_GetWindowMenuBar (Nlm_WindoW w)

{
  Nlm_MenuBaR     rsult;
  Nlm_WindowData  wdata;

  rsult = NULL;
  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    rsult = wdata.menuBar;
  }
  return rsult;
}

extern void Nlm_SetButtonDefault (Nlm_ButtoN b, Nlm_Boolean dflt);
extern void Nlm_SetWindowDefaultButton (Nlm_WindoW w, Nlm_ButtoN b)
{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    if (wdata.defaultButton != NULL)
      Nlm_SetButtonDefault(wdata.defaultButton, FALSE);
    wdata.defaultButton = b;
    if (wdata.defaultButton != NULL)
      Nlm_SetButtonDefault(wdata.defaultButton, TRUE);
    Nlm_SetWindowData (w, &wdata);
  }
}

extern Nlm_ButtoN Nlm_GetWindowDefaultButton (Nlm_WindoW w)

{
  Nlm_ButtoN      rsult;
  Nlm_WindowData  wdata;

  rsult = NULL;
  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    rsult = wdata.defaultButton;
  }
  return rsult;
}

static void Nlm_SetNotYetShown (Nlm_WindoW w, Nlm_Boolean notYet)

{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    wdata.notYetShown = notYet;
    Nlm_SetWindowData (w, &wdata);
  }
}

extern Nlm_Boolean Nlm_WindowHasBeenShown (Nlm_WindoW w)

{
  Nlm_Boolean     rsult;
  Nlm_WindowData  wdata;

  rsult = FALSE;
  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    rsult = (Nlm_Boolean) (! wdata.notYetShown);
  }
  return rsult;
}

static void Nlm_SetWindowDying (Nlm_WindoW w, Nlm_Boolean die)

{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    wdata.dying = die;
    Nlm_SetWindowData (w, &wdata);
  }
}

extern Nlm_Boolean Nlm_IsWindowDying (Nlm_WindoW w)

{
  Nlm_Boolean     rsult;
  Nlm_WindowData  wdata;

  rsult = FALSE;
  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    rsult = wdata.dying;
  }
  return rsult;
}

extern Nlm_WindoW Nlm_GetParentWindow (Nlm_GraphiC a)

{
  Nlm_GraphiC  g;
  Nlm_GraphiC  p;

  g = a;
  if (a != NULL) {
    p = Nlm_GetParent (g);
    while (p != NULL) {
      g = p;
      p = Nlm_GetParent (g);
    }
  }
  return (Nlm_WindoW) g;
}

extern Nlm_WindoW Nlm_ParentWindow (Nlm_Handle a)

{
  return Nlm_GetParentWindow ((Nlm_GraphiC) a);
}

extern Nlm_WindowTool Nlm_ParentWindowPtr (Nlm_GraphiC a)

{
  Nlm_WindowTool  rsult;
  Nlm_WindoW      w;

  rsult = (Nlm_WindowTool) 0;
  if (a != NULL) {
    w = Nlm_GetParentWindow (a);
    rsult = Nlm_GetWindowPtr (w);
  }
  return rsult;
}

extern Nlm_PortTool Nlm_ParentWindowPort (Nlm_GraphiC a)

{
  Nlm_PortTool  rsult;
  Nlm_WindoW    w;

  rsult = NULL;
  if (a != NULL) {
    w = Nlm_GetParentWindow (a);
    rsult = Nlm_GetWindowPort (w);
  }
  return rsult;
}

extern Nlm_MainTool Nlm_ParentWindowMain (Nlm_GraphiC a)

{
  Nlm_MainTool  rsult;
  Nlm_WindoW    w;

  rsult = (Nlm_MainTool) 0;
  if (a != NULL) {
    w = Nlm_GetParentWindow (a);
    rsult = Nlm_GetWindowMain (w);
  }
  return rsult;
}

extern Nlm_ShellTool Nlm_ParentWindowShell (Nlm_GraphiC a)

{
  Nlm_ShellTool  rsult;
  Nlm_WindoW     w;

  rsult = (Nlm_ShellTool) 0;
  if (a != NULL) {
    w = Nlm_GetParentWindow (a);
    rsult = Nlm_GetWindowShell (w);
  }
  return rsult;
}

extern Nlm_WindoW Nlm_SavePortIfNeeded (Nlm_GraphiC a, Nlm_Boolean savePort)

{
  Nlm_WindoW  newwindow;
  Nlm_WindoW  oldwindow;
  Nlm_WindoW  rsult;

  rsult = NULL;
  if (a != NULL && savePort) {
    savePort = Nlm_GetAllParentsVisible (a);
    if (savePort) {
      oldwindow = Nlm_CurrentWindow ();
      newwindow = Nlm_GetParentWindow (a);
      savePort = (Nlm_Boolean) (oldwindow != newwindow);
      if (savePort) {
        Nlm_UseWindow (newwindow);
        rsult = oldwindow;
      }
    }
  }
  return rsult;
}

extern Nlm_WindoW Nlm_SavePort (Nlm_Handle a)

{
  return Nlm_SavePortIfNeeded ((Nlm_GraphiC) a, TRUE);
}

extern void Nlm_RestorePort (Nlm_WindoW w)

{
  if (Nlm_processUpdatesFirstVal) {
    Nlm_Update ();
  }
  if (w != NULL) {
    Nlm_UseWindow (w);
  }
}

extern void Nlm_SetClose (Nlm_WindoW w, Nlm_WndActnProc cls)

{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    wdata.close = cls;
    Nlm_SetWindowData (w, &wdata);
  }
}

extern void Nlm_SetActivate (Nlm_WindoW w, Nlm_WndActnProc act)

{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    wdata.activate = act;
    Nlm_SetWindowData (w, &wdata);
  } else {
    appWndAct = act;
  }
}

extern void Nlm_SetDeactivate (Nlm_WindoW w, Nlm_WndActnProc deact)

{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    wdata.deactivate = deact;
    Nlm_SetWindowData (w, &wdata);
  } else {
    appWndDeact = deact;
  }
}

/*
 * Test X windows to see what kind of color buffer is available.
 * Necessary because Linux supports only the type available
 * from the hardware.  Returns TRUE if match and visinfo.
 */

#ifdef WIN_MOTIF

typedef struct _TNlm_MatchListX {
    int class;
    unsigned int depth;
} TNlm_MatchListX;

#define XMATCHLENGTH 6

/* list of display classes and depths to match, in order.
 * we prefer 8 bit for Cn3D for speed. 
 */
static const TNlm_MatchListX Nlm_MatchListX[] = { 
    { PseudoColor, 8  },
    { GrayScale, 8 },
    { PseudoColor, 16 },
    { TrueColor, 24 },
    { GrayScale, 16 },
    { TrueColor, 16 } };
    
extern Nlm_Boolean Nlm_CheckX(XVisualInfo * visinfo)
{
    int visualsmatched, i, j, defaultdepth;
    XVisualInfo *VisualList, VisualTemplate;
    
    /* check display modes in order of preference */
    
    VisualTemplate.screen = DefaultScreen(Nlm_currentXDisplay);
    
    /* enforce vibrant convention of using default depth for windows */
    defaultdepth = DefaultDepth(Nlm_currentXDisplay, VisualTemplate.screen);
    
    VisualList = XGetVisualInfo(Nlm_currentXDisplay, VisualScreenMask,
            &VisualTemplate, &visualsmatched);
    
    for(j = 0; j < XMATCHLENGTH; j++) {
        for(i = 0; i < visualsmatched; i++) {
         if (VisualList[i].depth == Nlm_MatchListX[j].depth 
                 && VisualList[i].class == Nlm_MatchListX[j].class
                 && VisualList[i].depth == defaultdepth) {
                MemCpy(visinfo, &VisualList[i], sizeof(XVisualInfo));
                XFree(VisualList);
                return TRUE;
            }
        }
    }
    return FALSE;
}
#endif /* WIN_MOTIF */

extern void Nlm_SetResize (Nlm_WindoW w, Nlm_WndActnProc resiz)

{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    wdata.resize = resiz;
    Nlm_SetWindowData (w, &wdata);
  }
}

extern void Nlm_SetColorMap (Nlm_WindoW w, Nlm_Uint2 totalColors,
                             Nlm_Uint1Ptr red, Nlm_Uint1Ptr green,
                             Nlm_Uint1Ptr blue)
{
  Nlm_WindowData  wdata;

#ifdef WIN_MSWIN
  Nlm_Uint2       i;
  LOGPALETTE    * palette;
  PALETTEENTRY  * p;
#endif
#ifdef WIN_MAC
  Nlm_Int2        i;
  RGBColor        col;
#endif


  if (w == NULL  ||  totalColors > 256)
    return;

  Nlm_GetWindowData (w, &wdata);

#ifdef WIN_MSWIN
  if ( GetDeviceCaps (Nlm_currentHDC, RASTERCAPS) &  RC_PALETTE ){
    if ( wdata.cMap != NULL ){
      DeleteObject( wdata.cMap );
      wdata.cMap = NULL;
    }
    if ( totalColors!=0 ) {
      palette = (LOGPALETTE*)MemNew (sizeof(LOGPALETTE) +
                                     totalColors*sizeof(PALETTEENTRY));
      if ( palette != NULL ) {
        palette->palVersion = 0x300;
        palette->palNumEntries = totalColors;
        for( i=0; i<totalColors; i++ ) {
          p = &(palette->palPalEntry[i]);
          p->peFlags = (BYTE)((i != 0) ? 0 : PC_RESERVED);
          p->peRed   = red[i];
          p->peGreen = green[i];
          p->peBlue  = blue[i];
        }
        wdata.cMap = CreatePalette(palette);
        MemFree (palette);
        if ( wdata.cMap != NULL ) {
          SelectPalette ( Nlm_currentHDC,wdata.cMap,FALSE );
          RealizePalette ( Nlm_currentHDC );

        }
      }
    }
  }
#endif

#ifdef WIN_MAC
  if ( wdata.cMap != NULL ){
    if ( wdata.cMapStatus ){
      SetPalette( wdata.handle, NULL, FALSE);
    }
    DisposePalette ( wdata.cMap );
    wdata.cMap = NULL;
  }
  wdata.cMapStatus = 0;
  if ( totalColors!=0 ) {
    wdata.cMap = NewPalette(totalColors,(CTabHandle)0,pmTolerant,0);
    if ( wdata.cMap != NULL ) {
      for( i=0; i<totalColors; i++ ) {
        col.red   = (Nlm_Uint2)red[i]<<8 | (Nlm_Uint2)red[i];
        col.green = (Nlm_Uint2)green[i]<<8 | (Nlm_Uint2)green[i];
        col.blue  = (Nlm_Uint2)blue[i]<<8 | (Nlm_Uint2)blue[i];
        SetEntryColor(wdata.cMap,i,&col);
      }
      SetPalette( wdata.handle, wdata.cMap, FALSE);
      ActivatePalette ( wdata.handle );
      wdata.cMapStatus = 1;
    }
  }
#endif

#ifdef WIN_MOTIF
  do { /* TRY */
    unsigned long  pixel;
    int            n_savedColors = 0;
    XColor         colorCells[256];

    /* Uninstall, store first several colors and free current
     * colormap -- if necessary */ 
    if ( wdata.cMap )
      {
        n_savedColors = 32;
        if (n_savedColors > (int)totalColors)
          return;

        if ( wdata.cMapStatus )
          {
            XUninstallColormap(Nlm_currentXDisplay, wdata.cMap);
            wdata.cMapStatus = 0;
          }

        if ( !wdata.cMap_fixed )
          {
            if (totalColors != 0)
              {            
                for (pixel = 0;  pixel < n_savedColors;  pixel++)
                  colorCells[pixel].pixel = pixel;
                XQueryColors(Nlm_currentXDisplay, wdata.cMap,
                             colorCells, n_savedColors);
              }

            XFreeColormap(Nlm_currentXDisplay, wdata.cMap);
            wdata.cMap = (Nlm_ColorMTool) 0;
          }
      }

    if (totalColors == 0)
      break;    /* no colors specified for the new colormap */

    /* Create new colormap, if necessary */
    if ( !wdata.cMap )
      {
        XVisualInfo    visinfo;
        unsigned long  plane_m[1];
        unsigned long  pixels[256];
        int defaultdepth;
        Boolean testvisual;

        defaultdepth = DefaultDepth(Nlm_currentXDisplay,
                  DefaultScreen(Nlm_currentXDisplay));
#ifdef OS_UNIX_LINUX
        if(!Nlm_CheckX(&visinfo))
#else /* OS_UNIX_LINUX */
        if( !(XMatchVisualInfo(Nlm_currentXDisplay,
                         DefaultScreen(Nlm_currentXDisplay),
                         8,PseudoColor,&visinfo) ||
            XMatchVisualInfo(Nlm_currentXDisplay,
                         DefaultScreen(Nlm_currentXDisplay),
                         8,GrayScale,&visinfo)) )
#endif /* else OS_UNIX_LINUX */      
        break;  /* no matching visuals found */
          
#ifdef OS_UNIX_LINUX
        if(visinfo.class > PseudoColor) break;  /* no palette needed */  
#endif
        wdata.cMap = XCreateColormap(Nlm_currentXDisplay,
                                     RootWindow(Nlm_currentXDisplay,
                                                Nlm_currentXScreen),
                                     visinfo.visual, AllocNone);
        if (wdata.cMap == DefaultColormap(Nlm_currentXDisplay,
                                          Nlm_currentXScreen))
          {
            wdata.cMap = (Nlm_ColorMTool) 0;
            break;  /* hardware colormap is immutable */
          }

        if ( !XAllocColorCells ( Nlm_currentXDisplay, wdata.cMap, FALSE,
                                 (unsigned long*) plane_m, 0,
                                 (unsigned long*) pixels, totalColors) )
          {
            XFreeColormap (Nlm_currentXDisplay,wdata.cMap);
            wdata.cMap = (Nlm_ColorMTool) 0;
            break;  /* cannot allocate color cells for the new colormap */
          }
      }

    for (pixel = n_savedColors;  pixel < totalColors;  pixel++)
      {
        colorCells[pixel].red   = (((Nlm_Uint2)red  [pixel]) << 8) |
          (Nlm_Uint2)red  [pixel];
        colorCells[pixel].green = (((Nlm_Uint2)green[pixel]) << 8) |
          (Nlm_Uint2)green[pixel];
        colorCells[pixel].blue  = (((Nlm_Uint2)blue [pixel]) << 8) |
          (Nlm_Uint2)blue [pixel];
        
        colorCells[pixel].flags = DoRed | DoGreen | DoBlue;
        colorCells[pixel].pixel = pixel;
        colorCells[pixel].pad = 0;
      }
    XStoreColors(Nlm_currentXDisplay, wdata.cMap,
                 colorCells + n_savedColors,
                 (int)(pixel - n_savedColors));

    XInstallColormap(Nlm_currentXDisplay, wdata.cMap);
    XSetWindowColormap (Nlm_currentXDisplay, XtWindow( wdata.shell ),
                        wdata.cMap);
    {{
      Nlm_ShellDataPtr sptr = wdata.allShells;
      while (sptr != NULL)
        {
          XSetWindowColormap (Nlm_currentXDisplay, XtWindow( sptr->shell ),
                              wdata.cMap);
          sptr = sptr->next;
        }
    }}

    wdata.cMapStatus = 1;
  }  while ( 0 );
#endif

  Nlm_SetWindowData (w, &wdata);
}


extern void Nlm_SetColorCell(Nlm_GraphiC gr, Nlm_Uint1 pixel,
                             Nlm_Uint1 red, Nlm_Uint1 green, Nlm_Uint1 blue)
{
#ifdef WIN_MOTIF
  Colormap cMap;
  XColor colorCell;

  /* get colormap, if fixed */
  if ( !Nlm_GetFixedColormap(gr, &cMap, NULL) )
    return;

  /* compose and store the new color cell */
  colorCell.red   = ((Nlm_Uint2)red   << 8) | (Nlm_Uint2)red;
  colorCell.green = ((Nlm_Uint2)green << 8) | (Nlm_Uint2)green;
  colorCell.blue  = ((Nlm_Uint2)blue  << 8) | (Nlm_Uint2)blue;
  colorCell.flags = DoRed | DoGreen | DoBlue;
  colorCell.pixel = pixel;
  colorCell.pad   = 0;
  XStoreColor(Nlm_currentXDisplay, cMap, &colorCell);
#endif

#ifdef WIN_MSWIN_obsolete
  Nlm_WindowData  wdata;
  PALETTEENTRY    colorCell;
  Nlm_WindoW      w = Nlm_GetParentWindow( gr );     

  if (w == NULL  ||
      (GetDeviceCaps(Nlm_currentHDC, RASTERCAPS) &  RC_PALETTE) == 0)
    return;

  Nlm_GetWindowData (w, &wdata);
  if (wdata.cMap == NULL)
    return;
    
  colorCell.peFlags = PC_RESERVED;
  colorCell.peRed   = red;
  colorCell.peGreen = green;
  colorCell.peBlue  = blue;

  AnimatePalette(wdata.cMap, pixel, 1, &colorCell);
#endif

#if defined(WIN_MAC)  || defined(WIN_MSWIN)
  Nlm_InvalObject ((Nlm_Handle) gr);
#endif
}



#ifdef WIN_MOTIF
static Nlm_CharPtr resource_nm = NULL;
extern void Nlm_SetMotifWindowName (Nlm_CharPtr nm)
{
  resource_nm = (Nlm_CharPtr) Nlm_MemFree (resource_nm);
  if (nm != NULL && *nm != '\0') {
    resource_nm = StringSave (nm);
  }
}

static Nlm_Uchar motif_n_colorcells = (Nlm_Uchar)0;

extern void Nlm_RestrictMotifColorsTo (Nlm_Uchar n_colorcells)
{
  motif_n_colorcells = n_colorcells;
}

extern Nlm_Boolean Nlm_GetFixedColormap(Nlm_GraphiC w,
                                        Colormap *cMap, Visual **visual)
{
  Nlm_WindowData wdata;
  Nlm_GetWindowData(Nlm_ParentWindow( w ), &wdata);
  if ( !wdata.cMap_fixed )
    return FALSE;

  if ( cMap )
    *cMap = wdata.cMap;
  if ( visual )
    *visual = wdata.visual;

  return TRUE;
}

static Nlm_Boolean Nlm_BusyColormap(unsigned long fromColor,
                                    unsigned long toColor,
                                    Colormap *cMap,
                                    Visual  **visual,
                                    int      *depth)
{
#define MAX_toColor 256
#define N_DEF_COL   2
  unsigned long nInherit = N_DEF_COL;

  if (!Nlm_hasColor  ||
      fromColor < N_DEF_COL  || fromColor > toColor  || toColor > MAX_toColor)
    return FALSE;

  if (fromColor == toColor)
    { /* Special case:  create new colormap and inherit
         first "toColor" color from the system colormap */
      nInherit = toColor;
    }

  {{ /* Create new colormap, if possible */
    XVisualInfo visinfo;
    Window      rootwin = RootWindow(Nlm_currentXDisplay, Nlm_currentXScreen);
    if ( !XMatchVisualInfo(Nlm_currentXDisplay,Nlm_currentXScreen,
                           8,PseudoColor,&visinfo)  &&
         !XMatchVisualInfo(Nlm_currentXDisplay,Nlm_currentXScreen,
                           8,GrayScale,&visinfo) )
      return FALSE;

    *cMap = XCreateColormap(Nlm_currentXDisplay, rootwin, visinfo.visual,
                            AllocNone);
    if ( visual )
      *visual = visinfo.visual;
    if ( depth )
      *depth = 8;
  }}

  {{ /* Allocate colors */
    unsigned long  plane_m;
    unsigned long  pixels[MAX_toColor];
    if ( !XAllocColorCells(Nlm_currentXDisplay, *cMap,
                           FALSE, &plane_m, 0, pixels, toColor) )
      {
        if (*cMap != DefaultColormap(Nlm_currentXDisplay,
                                     Nlm_currentXScreen) )
          XFreeColormap(Nlm_currentXDisplay, *cMap);
        return FALSE;
      }
  }}

  {{ /* Inherit first "nInherit" colors from the system default colormap */
    XColor defColors[MAX_toColor];
    unsigned long pixel;
    for (pixel = 0;  pixel < nInherit;  pixel++)
      defColors[pixel].pixel = pixel;
    XQueryColors(Nlm_currentXDisplay,
                 DefaultColormap(Nlm_currentXDisplay, Nlm_currentXScreen),
                 defColors, nInherit);
    XStoreColors(Nlm_currentXDisplay, *cMap, defColors, nInherit);
  }}

  {{ /* Free first "fromColor" but the very first "N_DEF_COL" colorcells */
    unsigned long pixel;
    unsigned long pixels[256];
    for(pixel = nInherit;  pixel < fromColor;  pixel++)
      {
        pixels[pixel-nInherit] = pixel;
      }
    XFreeColors(Nlm_currentXDisplay, *cMap, pixels, fromColor-nInherit, 0);
  }}

  return TRUE;
}


static Boolean Nlm_XGetRGBColormap(VisualID visualid, XStandardColormap *xcmap)
{ /* Look for an already existing XA_RGB_DEFAULT_MAP */
    XStandardColormap *xcmaps;
    int i, n_xcmaps;
    if ( !XGetRGBColormaps(Nlm_currentXDisplay,
                           RootWindow(Nlm_currentXDisplay,
                                      Nlm_currentXScreen),
                           &xcmaps, &n_xcmaps, XA_RGB_DEFAULT_MAP))
      return FALSE;

    for (i = 0;  i < n_xcmaps  &&  xcmaps[i].visualid != visualid;  i++);
    if (i == n_xcmaps  ||  !xcmaps[i].red_max  ||  !xcmaps [i].visualid)
      return FALSE;

    *xcmap = xcmaps[i];
    XFree( xcmaps );
    return TRUE;
}


static XStandardColormap Nlm_stdColormapRGB;
static Boolean           Nlm_stdGrayScale = False;

static Boolean Nlm_StandardColormap(Colormap *cMap,
                                    Visual **visual, int *depth)
{ /* Try use the XA_RGB_DEFAULT_MAP standard colormap of 8-bit depth */
  XVisualInfo visinfo;

  ASSERT ( !Nlm_stdColormapRGB.visualid );

  /* The relevant 8-bit visual */
  if ( !XMatchVisualInfo(Nlm_currentXDisplay, Nlm_currentXScreen,
                        8, PseudoColor, &visinfo) )
    {
      if ( !XMatchVisualInfo(Nlm_currentXDisplay, Nlm_currentXScreen,
                             8, GrayScale, &visinfo) )
        return FALSE;
      Nlm_stdGrayScale = True;
    }

  /* Use already existing or create new default standard colormap*/
  if (Nlm_XGetRGBColormap(visinfo.visualid, &Nlm_stdColormapRGB)  ||
      (XmuLookupStandardColormap(Nlm_currentXDisplay, Nlm_currentXScreen,
                                 visinfo.visualid, visinfo.depth,
                                 XA_RGB_DEFAULT_MAP, False, True)  &&
       Nlm_XGetRGBColormap(visinfo.visualid, &Nlm_stdColormapRGB)))
    {
      *cMap = Nlm_stdColormapRGB.colormap;
      if ( visual )
        *visual = visinfo.visual;
      if ( depth )
        *depth = visinfo.depth;
      return TRUE;
    }

  return FALSE;
}


static Boolean Nlm_XrmGetResource2 PROTO((const char *_basename,
                                          const char *_resource,
                                          XrmValuePtr value));

extern XVisualInfo * Nlm_GetBestOGLVisual(void); /* in vibslate.c */

static Colormap Nlm_VibrantDefaultColormapEx(Visual **visual, int *depth)
{
  static Nlm_Boolean vibrant_cMap_ready = FALSE;
  static Colormap    vibrant_cMap;
  static Visual     *vibrant_visual = NULL;
  static int         vibrant_depth = -1;

  Boolean  privateColormap;
  int      nInheritColors = 64;  /* default */
  char    *value;

  while ( !vibrant_cMap_ready )
    {{ /* THE FIRST TIME ONLY */
      vibrant_cMap_ready = TRUE;

      value = Nlm_XrmGetResource( "privateColormap" );
      privateColormap = (value != NULL  && 
                         (strcmp(value, "true") == 0  ||
                          strcmp(value, "yes" ) == 0  ||
                          strcmp(value, "on"  ) == 0  ||
                          strcmp(value, "1"   ) == 0));
      Nlm_MemFree( value );
      
      if (privateColormap  &&
          (value = Nlm_XrmGetResource( "nInheritColors" )) != NULL )
        {
          int int_value = atoi( value );
          Nlm_MemFree( value );

          if (1 < int_value ||  int_value < 256)
            nInheritColors = int_value;
        }

      /* Compose private colormap, if specified */
      if (privateColormap  &&
          Nlm_BusyColormap(nInheritColors, nInheritColors,
                           &vibrant_cMap, &vibrant_visual, &vibrant_depth))
        break;

#ifdef _OPENGL
      /* we want better-than-8-bit color for OpenGL, if possible, so force
         vibrant to try those first, using glX selection of a visual/depth
         appropriate for OpenGL */
      {
        XVisualInfo *visinfo = Nlm_GetBestOGLVisual();
        if (!visinfo) {
          puts("Can't find OpenGL-appropriate visual!");
          exit(1);
        }
        vibrant_visual = visinfo->visual;
        vibrant_depth = visinfo->depth;
        /* create an X colormap since probably not using default visual */
        vibrant_cMap = XCreateColormap(
          Nlm_currentXDisplay,
          RootWindow(Nlm_currentXDisplay, visinfo->screen), \
          visinfo->visual, AllocNone);
      }
#else
      /* Try use the XA_RGB_DEFAULT_MAP standard colormap of 8-bit depth */
      if ( Nlm_StandardColormap(&vibrant_cMap,&vibrant_visual,&vibrant_depth) )
        break;

      /* By default, use default colormap and visual */
      vibrant_cMap  = DefaultColormap(Nlm_currentXDisplay, Nlm_currentXScreen);
      vibrant_visual= DefaultVisual  (Nlm_currentXDisplay, Nlm_currentXScreen);
      vibrant_depth = DefaultDepth   (Nlm_currentXDisplay, Nlm_currentXScreen);
#endif /* ! _OPENGL */

    }}

  if ( visual )
    *visual = vibrant_visual;
  if ( depth )
    *depth = vibrant_depth;
  return vibrant_cMap;
}

extern Colormap Nlm_VibrantDefaultColormap( void )
{
  return Nlm_VibrantDefaultColormapEx(NULL, NULL);
}

extern Visual *Nlm_VibrantDefaultVisual( void )
{
  Visual *visual;
  Nlm_VibrantDefaultColormapEx(&visual, NULL);
  return visual;
}

extern int Nlm_VibrantDefaultDepth( void )
{
  int depth;
  Nlm_VibrantDefaultColormapEx(NULL, &depth);
  return depth;
}


extern void Nlm_XAllocColor(XColor *xcolor, Colormap colormap,
                            Nlm_Uint1 red, Nlm_Uint1 green, Nlm_Uint1 blue)
{
  if (Nlm_currentXDisplay == NULL)
    return;

  if (Nlm_stdColormapRGB.visualid  &&  Nlm_stdColormapRGB.colormap == colormap)
    { /* Use standard colormap */
      if ( Nlm_stdGrayScale )
        xcolor->pixel = (30 * red + 59 * green + 11 * blue) *
          Nlm_stdColormapRGB.red_max / 25500 * Nlm_stdColormapRGB.red_mult;
      else
        xcolor->pixel =
          ((red   * Nlm_stdColormapRGB.red_max   + 128) / 255) *
           Nlm_stdColormapRGB.red_mult   +
          ((green * Nlm_stdColormapRGB.green_max + 128) / 255) *
          Nlm_stdColormapRGB.green_mult +
          ((blue  * Nlm_stdColormapRGB.blue_max  + 128) / 255) *
          Nlm_stdColormapRGB.blue_mult;

      xcolor->pixel += Nlm_stdColormapRGB.base_pixel;
      XQueryColor(Nlm_currentXDisplay, colormap, xcolor); 
      return;
    }

  xcolor->red   = red   << 8 | red;
  xcolor->green = green << 8 | green;
  xcolor->blue  = blue  << 8 | blue;

  if (!Nlm_hasColor  ||
      !XAllocColor(Nlm_currentXDisplay,
                   (colormap != 0) ? colormap : Nlm_VibrantDefaultColormap(),
                   xcolor))
    {
      xcolor->pixel =
        (30 * (int)red + 59 * (int)green + 11 * (int)blue < 12800) ?
        BlackPixel(Nlm_currentXDisplay, Nlm_currentXScreen) :
        WhitePixel(Nlm_currentXDisplay, Nlm_currentXScreen);
    }
}
#endif /* WIN_MOTIF */


extern void Nlm_SetWindowExtra (Nlm_WindoW w, Nlm_VoidPtr data, Nlm_WndFreeProc cleanup)

{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    wdata.data = data;
    wdata.cleanup = cleanup;
    Nlm_SetWindowData (w, &wdata);
  }
}

extern Nlm_VoidPtr Nlm_GetWindowExtra (Nlm_WindoW w)

{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    return wdata.data;
  } else {
    return NULL;
  }
}

#ifdef WIN_MAC
static void Nlm_ClearKeys (void)
{
  Nlm_cmmdKey = FALSE;
  Nlm_ctrlKey = FALSE;
  Nlm_optKey = FALSE;
  Nlm_shftKey = FALSE;
  Nlm_dblClick = FALSE;
}
#endif


static Nlm_WindoW Nlm_FindWindowRec (Nlm_WindowTool wptr)

{
  Nlm_WindoW      w;
#ifdef WIN_MAC
  Nlm_WindowData  wdata;
#endif

#ifdef WIN_MAC
  Nlm_GetWindowData (Nlm_systemWindow, &wdata);
  wdata.handle = wptr;
  Nlm_SetWindowData (Nlm_systemWindow, &wdata);
#endif
  w = Nlm_desktopWindow;
  while (w != NULL && Nlm_ParentWindowPtr ((Nlm_GraphiC) w) != wptr) {
    w = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) w);
  }
  if (w == NULL) {
    w = Nlm_desktopWindow;
  }
  return w;
}

static void Nlm_SetWindowTitle (Nlm_GraphiC w, Nlm_Int2 item,
                                Nlm_CharPtr title, Nlm_Boolean savePort)


{
  Nlm_Char        temp [256];
  Nlm_WindowTool  wptr;
#ifdef WIN_MOTIF
  Nlm_ShellTool   shl;
  Nlm_WindowData  wdata;
#endif

  wptr = Nlm_ParentWindowPtr (w);
  Nlm_StringNCpy_0(temp, title, sizeof(temp));
#ifdef WIN_MAC
  Nlm_CtoPstr (temp);
  SetWTitle (wptr, (StringPtr) temp);
#endif
#ifdef WIN_MSWIN
  SetWindowText (wptr, temp);
#endif
#ifdef WIN_MOTIF
  shl = Nlm_GetWindowShell ((Nlm_WindoW) w);
  if (temp [0] != '\0') {
      if(getenv("VIBRANT_USE_APPLICATION_NAME") != NULL && Nlm_StringCmp(getenv("VIBRANT_USE_APPLICATION_NAME"),"0")) {
          Nlm_ProgramPath(temp, sizeof(temp));
          Nlm_StringNCpy_0(temp,Nlm_StringRChr(temp,'/')+1,sizeof(temp));
          Nlm_StringNCat(temp, ": ", sizeof(temp));
          Nlm_StringNCat(temp, title, sizeof(temp));
      }
    XtVaSetValues (shl, XmNtitle, temp, NULL);
  } else {
    XtVaSetValues (shl, XmNtitle, " ", NULL);
  }
  XFlush( Nlm_currentXDisplay );
  Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
  if (wdata.title != NULL) {
    wdata.title = (Nlm_CharPtr) Nlm_MemFree (wdata.title);
  }
  wdata.title = Nlm_StringSave (title);
  Nlm_SetWindowData ((Nlm_WindoW) w, &wdata);
#endif
}

static void Nlm_GetWindowTitle (Nlm_GraphiC w, Nlm_Int2 item,
                                Nlm_CharPtr title, size_t maxsize)

{
  Nlm_Char        temp [256];
  Nlm_WindowTool  wptr;
#ifdef WIN_MOTIF
  Nlm_WindowData  wdata;
#endif

  wptr = Nlm_ParentWindowPtr (w);
  temp [0] = '\0';
#ifdef WIN_MAC
  GetWTitle (wptr, (StringPtr) temp);
  Nlm_PtoCstr (temp);
#endif
#ifdef WIN_MSWIN
  GetWindowText (wptr, temp, sizeof (temp));
#endif
#ifdef WIN_MOTIF
  Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
  Nlm_StringNCpy_0(temp, wdata.title, sizeof(temp));
#endif
  Nlm_StringNCpy_0(title, temp, maxsize);
}

static Nlm_WindoW Nlm_MakeWindowLink (Nlm_RectPtr r, Nlm_Int2 recordSize,
                                      Nlm_GphPrcs PNTR classPtr)

{
  Nlm_GraphiC  n;
  Nlm_WindoW   w;

  w = (Nlm_WindoW) Nlm_HandNew (recordSize);
  if (w != NULL) {
    n = Nlm_GetNext ((Nlm_GraphiC) Nlm_desktopWindow);
    Nlm_LoadGraphicData ((Nlm_GraphiC) w, n, NULL, NULL, NULL,
                         classPtr, NULL, r, TRUE, FALSE, NULL, NULL);
    if (Nlm_nextIdNumber < 32767) {
      Nlm_nextIdNumber++;
    }
    Nlm_SetNext ((Nlm_GraphiC) Nlm_desktopWindow, (Nlm_GraphiC) w);
  }
  return w;
}

#ifdef WIN_MAC
extern void Nlm_SetUpdateRegion (WindowPtr wptr)

{
  Rect bounds;

  if (wptr != NULL) {
    GetPortVisibleRegion(GetWindowPort(wptr), Nlm_updateRgn);
    //HLock ((Handle) Nlm_updateRgn);
    GetRegionBounds(Nlm_updateRgn, &bounds);
    Nlm_RectToolToRecT (&bounds, &Nlm_updateRect);
    //HUnlock ((Handle) Nlm_updateRgn);
  }
}
#endif

extern Nlm_Int2  Nlm_internalMenuBarHeight;

#ifdef WIN_MOTIF
static void WindowCloseCallback(Widget wd,
                                XtPointer client_data, XtPointer call_data)
{
  Nlm_WndActnProc  cls;
  Nlm_WindoW       w;
  Nlm_WindowData   wdata;

  w = (Nlm_WindoW) client_data;
  Nlm_GetWindowData (w, &wdata);
  cls = wdata.close;
  if (cls != NULL) {
    cls (w);
  }
}


static int ignoreXerror_handler(Display *dpy, XErrorEvent *xerr)
{
  return 0;
}

static int defaultXerror_handler(Display *dpy, XErrorEvent *event)
{
  char buffer[BUFSIZ];
  char mesg[BUFSIZ];
  char number[32];
  char *mtype = "XlibMessage";

  XGetErrorText(dpy, event->error_code, buffer, BUFSIZ);
  XGetErrorDatabaseText(dpy, mtype, "XError", "X Error", mesg, BUFSIZ);
  fprintf(stderr, "%s:  %s\n  ", mesg, buffer);
  XGetErrorDatabaseText(dpy, mtype, "MajorCode", "Request Major code %d", 
                        mesg, BUFSIZ);
  fprintf(stderr, mesg, event->request_code);
  if (event->request_code < 128) {
    sprintf(number, "%d", event->request_code);
    XGetErrorDatabaseText(dpy, "XRequest", number, "", buffer, BUFSIZ);
  } else {
    buffer[0] = '\0';
  }
  fprintf(stderr, " (%s)\n", buffer);

  if (event->request_code >= 128) {
    XGetErrorDatabaseText(dpy, mtype, "MinorCode", "Request Minor code %d",
                          mesg, BUFSIZ);
    fputs("  ", stderr);
    fprintf(stderr, mesg, event->minor_code);
    fputs("\n", stderr);

    strcpy(buffer, "Value");
    XGetErrorDatabaseText(dpy, mtype, buffer, "", mesg, BUFSIZ);
    if (mesg[0]) {
      fputs("  ", stderr);
      fprintf(stderr, mesg, event->resourceid);
      fputs("\n", stderr);
    }
  } else if ((event->error_code == BadWindow) ||
             (event->error_code == BadPixmap) ||
             (event->error_code == BadCursor) ||
             (event->error_code == BadFont) ||
             (event->error_code == BadDrawable) ||
             (event->error_code == BadColor) ||
             (event->error_code == BadGC) ||
             (event->error_code == BadIDChoice) ||
             (event->error_code == BadValue) ||
             (event->error_code == BadAtom)) {
    if (event->error_code == BadValue)
      XGetErrorDatabaseText(dpy, mtype, "Value", "Value 0x%x",
                            mesg, BUFSIZ);
    else if (event->error_code == BadAtom)
      XGetErrorDatabaseText(dpy, mtype, "AtomID", "AtomID 0x%x",
                            mesg, BUFSIZ);
    else
      XGetErrorDatabaseText(dpy, mtype, "ResourceID", "ResourceID 0x%x",
                            mesg, BUFSIZ);
    fputs("  ", stderr);
    fprintf(stderr, mesg, event->resourceid);
    fputs("\n", stderr);
  }

  XGetErrorDatabaseText(dpy, mtype, "ErrorSerial", "Error Serial #%d", 
                        mesg, BUFSIZ);
  fputs("  ", stderr);
  fprintf(stderr, mesg, event->serial);
  XGetErrorDatabaseText(dpy, mtype, "CurrentSerial", "Current Serial #%d",
                        mesg, BUFSIZ);
  fputs("\n  ", stderr);
  fprintf(stderr, mesg, (XNextRequest(dpy) - 1));
  fputs("\n", stderr);

  return (event->error_code == BadImplementation) ? 0 : 1;
}


static void WindowStructFocusCallback (Widget wd, XtPointer client_data,
                                       XEvent *event, Boolean *contin)
{
  Nlm_WindoW w = (Nlm_WindoW)client_data;

  switch ( event->type )
    {
    case FocusOut:
      {
        Nlm_DoDeactivate((Nlm_GraphiC)w, FALSE);
        break;
      }

    case FocusIn:
      {
        Nlm_DoActivate((Nlm_GraphiC)w, FALSE);
        break;
      }

    case MapNotify:
      {
        if (wd == selectShell)
          { /* Set input focus */
            XWindowAttributes attr;
            Window selectWin = XtWindow( selectShell );
      
            XGetWindowAttributes(Nlm_currentXDisplay, selectWin, &attr);
            if (attr.map_state != IsViewable) {
              Nlm_SetVisible((Nlm_GraphiC)w, TRUE);
              return;
            }

            XSetInputFocus(Nlm_currentXDisplay, selectWin,
                           RevertToPointerRoot, CurrentTime);
            selectShell = NULL;
          }
        Nlm_SetVisible((Nlm_GraphiC)w, TRUE);
        XtVaSetValues(wd, XtNiconic, (Boolean)False, NULL);
        break;
      }

    case UnmapNotify:
      {
        Nlm_SetVisible((Nlm_GraphiC)w, FALSE);
        break;
      }

    case ConfigureNotify:
      { /* Move & Resize */
        Nlm_RecT         oldrect;
        Nlm_RecT         r;
        Nlm_RectTool     rtool;
        Window           xw, root, parent, child, *children = NULL;
        unsigned int     nchildren;
        int              x, y;
        Nlm_WindowData   wdata;

        XSetErrorHandler( ignoreXerror_handler );
        for (xw = XtWindow( wd );
             xw  &&  XQueryTree(Nlm_currentXDisplay, xw, &root, &parent,
                                &children, &nchildren)  &&
               root != parent;
             xw = parent)
          {
            XFree( (char *)children );
            children = NULL;
          }
        XSetErrorHandler( defaultXerror_handler );
        if ( children )
          XFree( (char *)children );

        if (root != parent)
          break;

        XTranslateCoordinates(Nlm_currentXDisplay, xw, root,
                              (Position)0, (Position)0,
                              &x, &y,  &child);

        rtool.x = x;
        rtool.y = y;
        rtool.width  = event->xconfigure.width;
        rtool.height = event->xconfigure.height;
        if (Nlm_GetWindowMenuBar( w ) != NULL)
          rtool.height -= Nlm_internalMenuBarHeight;

        Nlm_RectToolToRecT(&rtool, &r);
        Nlm_GetRect((Nlm_GraphiC)w, &oldrect);
        Nlm_SetRect((Nlm_GraphiC)w, &r);
        Nlm_GetWindowData(w, &wdata);

        if (wdata.resize != NULL  &&
            (oldrect.right  - oldrect.left != r.right  - r.left ||
             oldrect.bottom - oldrect.top  != r.bottom - r.top) &&
            Nlm_WindowHasBeenShown( w )  &&
            Nlm_GetVisible((Nlm_GraphiC) w))
          {
            wdata.resize( w );
          }
        break;
      }
    }
}


static void WindowEnterCallback(Widget wd, XtPointer client_data,
                                XEvent *ev, Boolean *contin)
{
  XCrossingEvent  *event;
  Nlm_WindoW       w;
  Nlm_WindowData   wdata;

  event = (XCrossingEvent*) ev;
  if (event->type != EnterNotify) return;
  w = (Nlm_WindoW) client_data;
  Nlm_GetWindowData (w, &wdata);
  if ( wdata.cMapStatus ) return;
  if ( !wdata.cMap )  return;
  XInstallColormap(Nlm_currentXDisplay, wdata.cMap);
  wdata.cMapStatus = 1;
  Nlm_SetWindowData (w, &wdata);
}

static void WindowLeaveCallback(Widget wd, XtPointer client_data,
                                XEvent *ev, Boolean *contin)
{
  XCrossingEvent  *event;
  Nlm_WindoW       w;
  Nlm_WindowData   wdata;

  event = (XCrossingEvent*) ev;
  if (event->type != LeaveNotify) return;
  w = (Nlm_WindoW) client_data;
  Nlm_GetWindowData (w, &wdata);
  if ( wdata.cMapStatus == 0 ) return;
  XUninstallColormap(Nlm_currentXDisplay, wdata.cMap);
  wdata.cMapStatus = 0;
  Nlm_SetWindowData (w, &wdata);
}


static GC vibrant_GC[25];

static GC Nlm_GetGC(int depth)
{
  ASSERT ( 0 <= depth  &&  depth < sizeof(vibrant_GC)/sizeof(GC) );
  if ( !vibrant_GC[depth] )
    {
      Pixmap fake_pixmap = XCreatePixmap(Nlm_currentXDisplay,
                                         RootWindow(Nlm_currentXDisplay,
                                                    Nlm_currentXScreen),
                                         4, 4, depth);
      vibrant_GC[depth] = XCreateGC(Nlm_currentXDisplay, fake_pixmap, 0, NULL);
      XFreePixmap(Nlm_currentXDisplay, fake_pixmap);
    }

  return vibrant_GC[depth];
}

static void Nlm_FreeGCs( void )
{
  size_t depth;
  for (depth = 0;  depth < sizeof(vibrant_GC)/sizeof(GC);  depth++)
    if ( vibrant_GC[depth] )
      {
        XFreeGC(Nlm_currentXDisplay, vibrant_GC[depth]);
        vibrant_GC[depth] = NULL;
      }
}
#endif



#ifdef WIN_MSWIN
static void client2win(Nlm_WindoW w,
                       Nlm_RectPtr client_rect, LPRECT win_rect)
{
  if (client_rect == NULL  ||  win_rect == NULL)
    return;

  win_rect->left   = client_rect->left;
  win_rect->top    = client_rect->top;
  win_rect->right  = client_rect->right;
  win_rect->bottom = client_rect->bottom;

  if (w == NULL)
    return;

  {{
  Nlm_Int2 dXleft, dXright, dYtop, dYbottom;
  Nlm_Int2 dX = 0, dY = 0;

  Nlm_WindowData wdata;
  Nlm_GetWindowData(w, &wdata);

  AdjustWindowRectEx(win_rect, wdata.style, (wdata.menuBar != NULL),
                     wdata.ex_style);

  dXleft  = (Nlm_Int2)(screenBitBounds.left  - win_rect->left );
  dXright = (Nlm_Int2)(screenBitBounds.right - win_rect->right);
  if (dXleft > 0)
    dX = dXleft;
  else if (dXright < 0)
    dX = dXright;
        
  dYtop    = (Nlm_Int2)(screenBitBounds.top    - win_rect->top   );
  dYbottom = (Nlm_Int2)(screenBitBounds.bottom - win_rect->bottom);
  if (dYtop > 0)
    dY = dYtop;
  else if (dYbottom < 0)
    dY = dYbottom;
        
  if (dX == 0  &&  dY == 0)
    return;

  Nlm_OffsetRect(client_rect, dX, dY);
  win_rect->left   += dX;
  win_rect->top    += dY;
  win_rect->right  += dX;
  win_rect->bottom += dY;
  }}
}


static void win2client(Nlm_WindoW w,
                       LPRECT win_rect, Nlm_RectPtr client_rect)
{
  RECT xclient_rect;

  ASSERT ( w           != NULL );
  ASSERT ( client_rect != NULL );
  ASSERT ( win_rect    != NULL );

  xclient_rect.left   = (Nlm_Int2)
    ((screenBitBounds.right - screenBitBounds.left) / 2 - 2);
  xclient_rect.right  = (Nlm_Int2)(xclient_rect.left + 4);
  xclient_rect.top    = (Nlm_Int2)
    ((screenBitBounds.bottom - screenBitBounds.top) / 2 - 2);
  xclient_rect.bottom = (Nlm_Int2)(xclient_rect.top + 4);


  {{
  RECT xwin_rect = xclient_rect;
  int  dXleft, dXright, dYtop, dYbottom;

  Nlm_WindowData wdata;
  Nlm_GetWindowData(w, &wdata);

  AdjustWindowRectEx(&xwin_rect, wdata.style, (wdata.menuBar != NULL),
                     wdata.ex_style);

  dXleft   = xclient_rect.left - xwin_rect.left;
  dXright  = (xwin_rect.right - xwin_rect.left) -
             (xclient_rect.right - xclient_rect.left) - dXleft;
  dYtop    = xclient_rect.top  - xwin_rect.top;
  dYbottom = (xwin_rect.bottom - xwin_rect.top) -
             (xclient_rect.bottom - xclient_rect.top) - dYtop;
    
  client_rect->left   = (Nlm_Int2)(win_rect->left   + dXleft  );
  client_rect->right  = (Nlm_Int2)(win_rect->right  - dXright );
  client_rect->top    = (Nlm_Int2)(win_rect->top    + dYtop   );
  client_rect->bottom = (Nlm_Int2)(win_rect->bottom - dYbottom);
  }}
}
#endif 



#define DOCUMENT_STYLE 1
#define FIXED_STYLE    2
#define FROZEN_STYLE   3
#define ROUND_STYLE    4
#define ALERT_STYLE    5
#define MODAL_STYLE    6
#define FLOATING_STYLE 7
#define SHADOW_STYLE   8
#define PLAIN_STYLE    9

static void Nlm_NewWindow (Nlm_WindoW w, Nlm_Int2 type, Nlm_Int2 procID,
                           Nlm_Boolean goAway, Nlm_CharPtr windowClass,
                           Nlm_Uint4 style, Nlm_CharPtr title,
                           Nlm_WndActnProc close, Nlm_WndActnProc resize)
{
  Nlm_ShellTool   dlg;
  Nlm_RecT        drag;
  Nlm_RecT        grow;
  Nlm_Int2        hsb;
  Nlm_MainTool    man;
  Nlm_Int2        margin;
  Nlm_RecT        p;
  Nlm_PortTool    prt;
  Nlm_PoinT       pt;
  Nlm_RecT        r;
  Nlm_ShellTool   shl;
  Nlm_Int2        spacing;
  Nlm_Char        temp [256];
  Nlm_CharPtr     ttl;
  Nlm_Int2        vsb;
  Nlm_WindowTool  wptr;
  Nlm_RecT        zoom;
#ifdef WIN_MAC
  Nlm_WindowTool  behindNone;
  CCTabHandle     colorHandle;
  CCTabPtr        colorPtr;
  Nlm_RectTool    rtool;
#endif
#ifdef WIN_MOTIF
  Atom            atom;
  Cardinal        n;
  Arg             wargs[32];
  int             screen_num;
  int             depth = -1;
  Visual         *visual;
  Colormap        cMap;
  Nlm_Boolean     is_cMap = FALSE;
#endif
#ifdef WIN_MSWIN
  Nlm_Uint4       ex_style = 0;
#endif

  Nlm_StringNCpy_0(temp, title, sizeof(temp));

#ifdef WIN_MAC
  Nlm_CtoPstr (temp);
#endif

  Nlm_LoadRect (&r, 50, 50, 150, 150);

#ifdef WIN_MOTIF
  r = Nlm_screenRect;
  Nlm_InsetRect (&r, 50, 50);
#endif

  Nlm_GetRect ((Nlm_GraphiC) w, &p);
  margin = 10;
  spacing = 10;
  if (p.right < 0)
    margin  = (Nlm_Int2)(-p.right);
  if (p.bottom < 0)
    spacing = (Nlm_Int2)(-p.bottom);
  if (p.right >= 0 || p.bottom >= 0)
    {
      r = p;
      r.bottom = (Nlm_Int2)(r.top  + r.bottom);
      r.right  = (Nlm_Int2)(r.left + r.right );
    }
  Nlm_LoadPt (&pt, margin, margin);
  shl  = (Nlm_ShellTool) 0;
  dlg  = (Nlm_ShellTool) 0;
  wptr = (Nlm_WindowTool) 0;
  prt  = NULL;
  man  = (Nlm_MainTool) 0;
  ttl  = NULL;
  hsb  = 0;
  vsb  = 0;

#ifdef WIN_MAC
  behindNone = (Nlm_WindowTool) (-1);
  TextFont (0);
  TextSize (0);
  Nlm_RecTToRectTool (&r, &rtool);
  if (close == NULL) {
    goAway = FALSE;
  }
  if (hasColorQD) {
    wptr = (WindowPtr) NewCWindow (NULL, &rtool, (StringPtr) temp,
                                   FALSE, procID,(Nlm_WindowTool) behindNone,
                                   goAway, 0);
  } else {
    wptr = NewWindow (NULL, &rtool, (StringPtr) temp, FALSE, procID,
                      (Nlm_WindowTool) behindNone, goAway, 0);
  }
  SetPortWindowPort(wptr);
  Nlm_currentWindowTool = wptr;
  prt = (Nlm_PortTool) wptr;
  Nlm_SetUpdateRegion (wptr);
  PenNormal ();
  MoveTo (2, 10);
  if (hasColorQD) {
    colorHandle = (CCTabHandle) GetResource ('cctb', 0);
    if (colorHandle != NULL) {
      LoadResource ((Handle) colorHandle);
      if (ResError () == noErr) {
        HLock ((Handle) colorHandle);
        colorPtr = *(colorHandle);
        if (colorPtr != NULL) {
          RGBForeColor (&(colorPtr->ctTable [2].rgb));
        }
        HUnlock ((Handle) colorHandle);
      }
    }
    GetForeColor (&Nlm_RGBforeColor);
    GetBackColor (&Nlm_RGBbackColor);
  }
#endif

#ifdef WIN_MSWIN
  if (close == NULL)
    style &= ~WS_SYSMENU;

  switch ( type )
    {
    case FLOATING_STYLE:
      ex_style = WS_EX_TOPMOST;
      break;
    case MODAL_STYLE:
      ex_style = WS_EX_DLGMODALFRAME;
      break;
    default:
      ex_style = 0;
    }
    
  wptr = CreateWindowEx(ex_style, windowClass, temp, style,
                        r.left, r.top, (r.right - r.left),
                        (r.bottom - r.top), NULL, NULL,
                        Nlm_currentHInst, NULL);

  if ( wptr )
    SetProp(wptr, (LPSTR)"Nlm_VibrantProp", (Nlm_HandleTool)w);

  prt = GetDC( wptr );
  ReleaseDC(wptr, prt);
  Nlm_currentHDC  = prt;
  Nlm_currentHWnd = wptr;
#endif

#ifdef WIN_MOTIF
  n = 0;
  if (temp [0] != '\0') {
    if(getenv("VIBRANT_USE_APPLICATION_NAME") != NULL && Nlm_StringCmp(getenv("VIBRANT_USE_APPLICATION_NAME"),"0")) {
      Nlm_ProgramPath(temp, sizeof(temp));
      Nlm_StringNCpy_0(temp,Nlm_StringRChr(temp,'/')+1,sizeof(temp));
      Nlm_StringNCat(temp, ": ", sizeof(temp));
      Nlm_StringNCat(temp, title, sizeof(temp));
    }
    XtSetArg (wargs[n], XmNtitle, temp); n++;
  } else {
    XtSetArg (wargs[n], XmNtitle, " "); n++;
  }
  XtSetArg (wargs[n], XmNx, (Position) r.left); n++;
  XtSetArg (wargs[n], XmNy, (Position) r.top); n++;
  XtSetArg (wargs[n], XmNdefaultFontList, Nlm_XfontList); n++;
  XtSetArg (wargs[n], XmNdeleteResponse,   XmDO_NOTHING); n++;
  XtSetArg (wargs[n], XmNallowShellResize, (Boolean)FALSE); n++;

#ifndef _OPENGL
  if (motif_n_colorcells != 0  &&
      Nlm_BusyColormap(motif_n_colorcells, 256, &cMap, &visual, &depth))
    is_cMap = TRUE;
  else
#endif
    cMap = Nlm_VibrantDefaultColormapEx(&visual, &depth);
  motif_n_colorcells = 0;

  XtSetArg(wargs[n], XtNdepth,    depth );  n++;
  XtSetArg(wargs[n], XtNvisual,   visual);  n++;
  XtSetArg(wargs[n], XtNcolormap, cMap  );  n++;

  shl = XtAppCreateShell ((String) resource_nm, (String) "Vibrant",
                          applicationShellWidgetClass,
                          Nlm_currentXDisplay, wargs, n);

  dlg = shl;
  if ( dlg ) {
    XtSetMappedWhenManaged (shl, FALSE);
#ifdef _DEBUG
    XtAddEventHandler(shl,
                      (EventMask) 0, True,
                      (XtEventHandler) _XEditResCheckMessages, NULL);
#endif
    XtAddEventHandler (shl, StructureNotifyMask|FocusChangeMask, FALSE,
                       WindowStructFocusCallback, (XtPointer) w);
    XtAddEventHandler (shl, EnterWindowMask, FALSE,
                       WindowEnterCallback, (XtPointer) w);
    XtAddEventHandler (shl, LeaveWindowMask, FALSE,
                       WindowLeaveCallback, (XtPointer) w);

    n = 0;
    XtSetArg (wargs[n], XmNwidth,  (Dimension) (r.right - r.left)); n++;
    XtSetArg (wargs[n], XmNheight, (Dimension) (r.bottom - r.top)); n++;
    XtSetArg (wargs[n], XmNmarginHeight, (Dimension)0); n++;
    XtSetArg (wargs[n], XmNmarginWidth,  (Dimension)0); n++;
    XtSetArg (wargs[n], XmNborderWidth,  (Dimension)0); n++;
    man = XmCreateMainWindow (dlg, (String) "", wargs, n);

    n = 0;
    XtSetArg (wargs[n], XmNwidth,  (Dimension) (r.right - r.left)); n++;
    XtSetArg (wargs[n], XmNheight, (Dimension) (r.bottom - r.top)); n++;
    XtSetArg (wargs[n], XmNbuttonFontList, Nlm_XfontList); n++;
    XtSetArg (wargs[n], XmNlabelFontList,  Nlm_XfontList); n++;
    XtSetArg (wargs[n], XmNtextFontList,   Nlm_XfontList); n++;
    XtSetArg (wargs[n], XmNmarginHeight, (Dimension)0); n++;
    XtSetArg (wargs[n], XmNmarginWidth,  (Dimension)0); n++;
    XtSetArg (wargs[n], XmNborderWidth,  (Dimension)0); n++;
    wptr = XmCreateBulletinBoard (man, (String) "", wargs, n);

    screen_num = DefaultScreen (Nlm_currentXDisplay);
    prt = Nlm_GetGC( depth );
    XtManageChild (man);
    XtManageChild (wptr);

    atom = XmInternAtom (Nlm_currentXDisplay, "WM_DELETE_WINDOW", TRUE);
    if (atom != 0) {
      XmAddWMProtocolCallback (shl, atom, WindowCloseCallback, (XtPointer) w);
    }
  }

  if (temp [0] != '\0') {
    ttl = Nlm_StringSave (temp);
  }
#endif

  Nlm_currentWindowTool = wptr;
  r = screenBitBounds;
  Nlm_LoadRect (&drag, (Nlm_Int2)(r.left  + 4), (Nlm_Int2)(r.top    + 24),
                       (Nlm_Int2)(r.right - 4), (Nlm_Int2)(r.bottom -  4));
  Nlm_LoadRect (&grow, r.left, (Nlm_Int2)(r.top + 24), r.right, r.bottom);
  Nlm_LoadRect (&zoom, (Nlm_Int2)(r.left  + 4), (Nlm_Int2)(r.top    + 40),
                       (Nlm_Int2)(r.right - 4), (Nlm_Int2)(r.bottom -  4));
  Nlm_LoadRect (&r, 0, 0, 0, 0);

#ifdef DCLAP
#ifdef WIN_MSWIN
  hsb = 6; vsb = 4;  /* dgg -- patch for mswin sizebar space */
#endif
#endif

  Nlm_LoadBoxData ((Nlm_BoX) w, pt, pt, pt, margin, margin,
                   (Nlm_Int2)(margin + hsb), (Nlm_Int2)(margin + vsb),
                   spacing, spacing, 0, 0);
  Nlm_LoadWindowData (w, wptr, shl, prt, man, &drag, &grow, &zoom,
                      close, NULL, NULL, resize, NULL, NULL, NULL,
                      TRUE, FALSE, ttl, NULL, NULL, NULL);


#ifdef WIN_MSWIN
  {{
  Nlm_WindowData wdata;
  Nlm_GetWindowData(w, &wdata);
  wdata.style    = style;
  wdata.ex_style = ex_style;
  Nlm_SetWindowData(w, &wdata);
  }}
#endif
#ifdef WIN_MOTIF
  if ( is_cMap )
    {
      Nlm_WindowData wdata;
      Nlm_GetWindowData(w, &wdata);
      wdata.cMap       = cMap;
      wdata.visual     = visual;
      wdata.cMap_fixed = TRUE;
      Nlm_SetWindowData(w, &wdata);
    }
  Nlm_UseWindow( w );
#endif
}


#ifdef WIN_MAC
static Nlm_WindoW Nlm_NextVisWindow (Nlm_WindoW w)

{
  WindowPtr  p;
  WindowPtr  q;

  if (w == Nlm_desktopWindow) {
    return Nlm_desktopWindow;
  } else {
    p = Nlm_ParentWindowPtr((Nlm_GraphiC) w);
    while (p != NULL) {
      q = GetNextWindow(p);
      if (q != NULL) {
        if (IsWindowVisible(q)) {
          p = NULL;
        } else {
          p = q;
        }
      } else {
        p = q;
      }
    }
    return Nlm_FindWindowRec ((Nlm_WindowTool) q);
  }
}
#endif

static void Nlm_HideWindow (Nlm_GraphiC w, Nlm_Boolean setFlag, Nlm_Boolean savePort)
{
  Nlm_WindowTool  wptr;
#ifdef WIN_MOTIF
  Nlm_ShellTool   shl;
#endif

  wptr = Nlm_ParentWindowPtr (w);
  Nlm_SetVisible (w, FALSE);
#ifdef WIN_MAC
  HideWindow (wptr);
  if (appWndDeact != NULL) {
    appWndDeact ((Nlm_WindoW) w);
  }
  Nlm_Update ();
#endif
#ifdef WIN_MSWIN
  ShowWindow (wptr, SW_HIDE);
  if (appWndDeact != NULL) {
    appWndDeact ((Nlm_WindoW) w);
  }
  Nlm_Update ();
#endif
#ifdef WIN_MOTIF
  shl = Nlm_GetWindowShell ((Nlm_WindoW) w);
  if (!NLM_QUIET  ||  Nlm_WindowHasBeenShown( (Nlm_WindoW)w ))
    {
      Window focus_win;
      int revert_to;
      XGetInputFocus(Nlm_currentXDisplay, &focus_win, &revert_to);
      if (focus_win == XtWindow( shl ))
        {
          XSetInputFocus(Nlm_currentXDisplay,
                         PointerRoot, RevertToNone, CurrentTime);
          XSync(Nlm_currentXDisplay, FALSE);
        }
      if ( Nlm_IconicWindow( (Nlm_WindoW)w ) )
        {
          XtVaSetValues(shl, XtNiconic, (Boolean)True,  NULL);
          XtVaSetValues(shl, XtNiconic, (Boolean)False, NULL);
          Nlm_Update();
        }
      XtUnmapWidget( shl );
    }
  Nlm_Update ();
#endif
}

/* Thanks to Mike Isaev for suggesting this solution to modal windows. */

static void Nlm_EnableOneWindow (Nlm_WindoW w, Nlm_Boolean onOffFlag)

{
#ifdef WIN_MSWIN
  Nlm_WindowTool  wptr;
#endif
#ifdef WIN_MOTIF
  Nlm_ShellTool   shl;
#endif

  if (w != NULL && w != Nlm_desktopWindow && w != Nlm_systemWindow) {
#ifdef WIN_MSWIN
    wptr = Nlm_GetWindowPtr (w);
    EnableWindow (wptr, onOffFlag);
#endif
#ifdef WIN_MOTIF
    shl = Nlm_GetWindowShell (w);
    XtSetSensitive(shl, onOffFlag);
#endif
  }
}

static void Nlm_EnableOtherWindows (Nlm_WindoW notThisWindow, Nlm_Boolean onOffFlag)

{
  Nlm_WindoW  w;

  w = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) Nlm_desktopWindow);
  while (w != NULL && w != Nlm_systemWindow) {
    if (w != notThisWindow) {
      Nlm_EnableOneWindow (w, onOffFlag);
    }
    w = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) w);
  }
}

/* esl++ */
extern Nlm_Boolean Nlm_IsWindowModal (Nlm_WindoW w)
{
  Nlm_GphPrcsPtr  classPtr;

  if (w != NULL) {
    classPtr = Nlm_GetClassPtr ((Nlm_GraphiC) w);
    if (classPtr == modalProcs || classPtr == movableModalProcs) return TRUE;
  }
  return FALSE;
}

/* esl++ */
static void Nlm_HideAndActivateOwner (Nlm_WindoW w, Nlm_WindoW owner)
{
  /* first of all, select owner window */
  Nlm_Select ((Nlm_GraphiC) owner);
  /* then hide window */
  if (w != NULL && w != Nlm_desktopWindow) {
    Nlm_WindowTool wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) w);
    if (wptr == Nlm_currentWindowTool) {
      Nlm_HideWindow ((Nlm_GraphiC) w, FALSE, FALSE);
      return;
    }
    if (wptr  &&  !Nlm_IsWindowDying((Nlm_WindoW) w)) {
      Nlm_SetVisible ((Nlm_GraphiC) w, FALSE);
#ifdef WIN_MAC
      ShowHide (wptr, FALSE);
      Nlm_Update ();
#endif
#ifdef WIN_MSWIN
      SetWindowPos (wptr, NULL, 0, 0, 0, 0,
                    SWP_HIDEWINDOW|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE|
                    SWP_NOZORDER);
      Nlm_Update ();
#endif
#ifdef WIN_MOTIF
      {
        Nlm_ShellTool shl = Nlm_GetWindowShell ((Nlm_WindoW) w);
        if (!NLM_QUIET  ||  Nlm_WindowHasBeenShown( (Nlm_WindoW)w ))
          XtUnmapWidget (shl);
        Nlm_Update ();
      }
#endif
    }
  }
}

static void Nlm_HideModal (Nlm_GraphiC w, Nlm_Boolean setFlag, Nlm_Boolean savePort)
{
  /* esl: handle nested modal dialogs */
  if (w != NULL) {
    Nlm_WindowData wdata;
    Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
    Nlm_HideAndActivateOwner ((Nlm_WindoW) w, wdata.modalOwner);
    if (Nlm_IsWindowModal (wdata.modalOwner)) {
      /* enable previous modal dialog only */
      Nlm_EnableOneWindow (wdata.modalOwner, TRUE);
    } else {
      /* enable all windows */
      Nlm_EnableOtherWindows ((Nlm_WindoW) w, TRUE);
    }
  }
}


static void Nlm_RemoveWindow (Nlm_GraphiC w, Nlm_Boolean savePort)

{
  Nlm_WindoW      nxt;
  Nlm_WindoW      p;
  Nlm_WindoW      q;
  Nlm_WindowData  wdata;
#ifdef WIN_MOTIF
  Nlm_ShellTool   shl;
#endif

  if ((Nlm_WindoW) w != Nlm_desktopWindow && (Nlm_WindoW) w != Nlm_systemWindow) {
    q = Nlm_desktopWindow;
    p = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) Nlm_desktopWindow);
    while (p != NULL && p != (Nlm_WindoW) w) {
      q = p;
      p = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) p);
    }
    if (p != NULL) {
      Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
      wdata.timer = NULL;
      Nlm_SetWindowData ((Nlm_WindoW) w, &wdata);
#ifdef WIN_MAC
      HideWindow (Nlm_GetWindowPtr ((Nlm_WindoW) w));
      Nlm_Update ();
#endif
#ifdef WIN_MSWIN
      ShowWindow (Nlm_GetWindowPtr ((Nlm_WindoW) w), SW_HIDE);
#endif
#ifdef WIN_MOTIF
      shl = Nlm_GetWindowShell ((Nlm_WindoW) w);
      if (NLM_QUIET) {
        if (Nlm_WindowHasBeenShown ((Nlm_WindoW) w)) {
          XtUnmapWidget (shl);
        }
      } else {
        XtUnmapWidget (shl);
      }
#endif
      Nlm_SetWindowDying ((Nlm_WindoW) w, TRUE);
      nxt = (Nlm_WindoW) Nlm_GetNext (w);
      Nlm_SetNext ((Nlm_GraphiC) q, (Nlm_GraphiC) nxt);
      Nlm_SetNext (w, (Nlm_GraphiC) dyingWindow);
      dyingWindow = (Nlm_WindoW) w;
      /* prevents parent lockout under Linux/LessTif, maybe others (thiessen) */
      if (Nlm_IsWindowModal((Nlm_WindoW) w))
        Nlm_HideModal(w, FALSE, FALSE);
    }
  }
  Nlm_Update ();
}

/* esl: extraWidth parameter added */
static void Nlm_ResizeWindow (Nlm_GraphiC w, Nlm_Int2 dragHeight,
                              Nlm_Int2 scrollWidth, Nlm_Int2 minWidth,
                              Nlm_Int2 extraWidth, Nlm_Int2 extraHeight)

{
  Nlm_BoxData     bdata;
  Nlm_Int4        free;
  Nlm_Int4        height, width;
  Nlm_Int4        leftpix;
  Nlm_PoinT       lpt;
  Nlm_Int2        menuHeight = 0;
  Nlm_RecT        r;
  Nlm_Int4        rleft;
  Nlm_Int4        rtop;
  Nlm_Int4        toppix;
  Nlm_WindowTool  wptr;
#ifdef WIN_MOTIF
  Nlm_MainTool    man;
  Nlm_ShellTool   shl;
  Window          xtw;
#endif

  if ( Nlm_WindowHasBeenShown((Nlm_WindoW) w) )  return;

  Nlm_SetRealized(w, TRUE);

#ifdef WIN_MAC
  menuHeight = 21;
  TextFont (0);
  TextSize (0);
#endif
#ifdef WIN_MOTIF
  if (Nlm_GetWindowMenuBar( (Nlm_WindoW)w ) != NULL)
    menuHeight = Nlm_internalMenuBarHeight;
#endif

  Nlm_GetRect (w, &r);
  if (r.left < 0 || r.right < 0 || r.top < 0 || r.bottom < 0) {
    Nlm_GetBoxData ((Nlm_BoX) w, &bdata);
    lpt = bdata.limitPoint;
    if (r.right < 0)
      {
        width = lpt.x + scrollWidth + bdata.xMargin;
        if (width < minWidth)
          width = minWidth;
      }
    else
      width = r.right;
    width += extraWidth; /* esl */

    if (r.bottom < 0)
      height = lpt.y + scrollWidth + extraHeight + bdata.yMargin;
    else
      height = r.bottom + extraHeight;
    if (r.left < 0)
      {
        free = screenBitBounds.right - width;
        rleft = (Nlm_Int4)r.left;
        leftpix = free * (-rleft) / 100;
        r.left = (Nlm_Int2)leftpix;
      }
    if (r.top < 0)
      {
        free = screenBitBounds.bottom - height - menuHeight - dragHeight;
        rtop = (Nlm_Int4)r.top;
        toppix = free * (-rtop) / 100;
        r.top = (Nlm_Int2)(toppix + menuHeight + dragHeight);
      }
    r.right  = (Nlm_Int2)(r.left + width);
    r.bottom = (Nlm_Int2)(r.top  + height);
#ifdef WIN_MOTIF
    r.bottom -= menuHeight;
#endif
    Nlm_SetRect(w, &r);

    wptr = Nlm_GetWindowPtr( (Nlm_WindoW)w );
#ifdef WIN_MAC
    SizeWindow (wptr, width, height, FALSE);
    MoveWindow (wptr, r.left, r.top, FALSE);
#endif
#ifdef WIN_MSWIN
    {{
    RECT wr;
    client2win((Nlm_WindoW)w, &r, &wr);
    Nlm_SetRect(w, &r);
    MoveWindow(wptr, wr.left, wr.top,
               (wr.right - wr.left), (wr.bottom - wr.top), FALSE);
    }}
#endif
#ifdef WIN_MOTIF
    shl = Nlm_GetWindowShell ((Nlm_WindoW) w);
    man = Nlm_GetWindowMain ((Nlm_WindoW) w);
    XtVaSetValues (shl,
                   XmNx, (Position) r.left,
                   XmNy, (Position) r.top,
                   XmNwidth,  (Dimension) width,
                   XmNheight, (Dimension) height,
                   NULL);
    XtVaSetValues (man,
                   XmNwidth,  (Dimension) width,
                   XmNheight, (Dimension) height,
                   NULL);
    XtVaSetValues (wptr,
                   XmNwidth,  (Dimension) width,
                   XmNheight, (Dimension) height,
                   NULL);
#endif
  }

#ifdef WIN_MOTIF
  if (NLM_QUIET  &&  !Nlm_IsWindowDying((Nlm_WindoW) w))  {
    shl = Nlm_GetWindowShell ((Nlm_WindoW) w);
    XtRealizeWidget (shl);
    man = Nlm_GetWindowMain ((Nlm_WindoW) w);
    xtw = XtWindow( man );
    if ( xtw )
      XDefineCursor(Nlm_currentXDisplay, xtw, Nlm_currentCursor);
  }
#endif
}


static void Nlm_ShowNormal (Nlm_GraphiC w, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_Char        title [256];
  Nlm_WindowTool  wptr;
#ifdef WIN_MOTIF
  Nlm_Int2        extrah;
  Nlm_ShellTool   shl;
#endif

  wptr = Nlm_ParentWindowPtr (w);
  Nlm_GetWindowTitle (w, 0, title, sizeof (title));
#ifdef WIN_MAC
  Nlm_ResizeWindow (w, 20, 0, Nlm_StringWidth (title) + 70, 0, 0);
  Nlm_SetNotYetShown ((Nlm_WindoW) w, FALSE);
  if (setFlag) {
    Nlm_SetVisible (w, TRUE);
    ShowWindow (wptr);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_ResizeWindow (w, 0, 0, (Nlm_Int2)(Nlm_StringWidth(title) + 80), 0, 0);
  Nlm_SetNotYetShown ((Nlm_WindoW) w, FALSE);
  if (setFlag) {
    Nlm_SetVisible (w, TRUE);
    ShowWindow (wptr, SW_SHOWNORMAL);
    UpdateWindow (wptr);
  }
#endif
#ifdef WIN_MOTIF
  extrah = 0;
  if (Nlm_GetWindowMenuBar ((Nlm_WindoW) w) != NULL) {
    extrah += 31;
  }
  Nlm_ResizeWindow (w, 0, 0, Nlm_StringWidth (title) + 65, 0, extrah);
  Nlm_SetNotYetShown ((Nlm_WindoW) w, FALSE);
  if (setFlag) {
    Nlm_SetVisible (w, TRUE);
    shl = Nlm_GetWindowShell ((Nlm_WindoW) w);
    XtMapWidget (shl);
  }
#endif
  Nlm_Update ();
}

static void Nlm_ShowModal (Nlm_GraphiC w, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_Char        title [256];
  Nlm_WindowTool  wptr;
#ifdef WIN_MOTIF
  Nlm_Int2        extrah;
  Nlm_ShellTool   shl;
#endif

  wptr = Nlm_ParentWindowPtr (w);
  Nlm_GetWindowTitle (w, 0, title, sizeof (title));
#ifdef WIN_MAC
  Nlm_ResizeWindow (w, 20, 0, Nlm_StringWidth (title) + 70, 0, 0);
  Nlm_SetNotYetShown ((Nlm_WindoW) w, FALSE);
  if (setFlag) {
    Nlm_SetVisible (w, TRUE);
    ShowWindow (wptr);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_ResizeWindow (w, 0, 0, (Nlm_Int2)(Nlm_StringWidth(title) + 80), 0, 0);
  Nlm_SetNotYetShown ((Nlm_WindoW) w, FALSE);
  if (setFlag) {
    Nlm_SetVisible (w, TRUE);
    ShowWindow (wptr, SW_SHOWNORMAL);
    UpdateWindow (wptr);
  }
#endif
#ifdef WIN_MOTIF
  extrah = 0;
  if (Nlm_GetWindowMenuBar ((Nlm_WindoW) w) != NULL) {
    extrah += 31;
  }
  Nlm_ResizeWindow (w, 0, 0, Nlm_StringWidth (title) + 65, 0, extrah);
  Nlm_SetNotYetShown ((Nlm_WindoW) w, FALSE);
  if (setFlag) {
    Nlm_SetVisible (w, TRUE);
    shl = Nlm_GetWindowShell ((Nlm_WindoW) w);
    XtMapWidget (shl);
  }
#endif
  Nlm_Update ();
  Nlm_EnableOtherWindows ((Nlm_WindoW ) w, FALSE);
}

static void Nlm_ShowDocument (Nlm_GraphiC w, Nlm_Boolean setFlag, Nlm_Boolean savePort)

{
  Nlm_Char        title [256];
  Nlm_WindowTool  wptr;
#ifdef WIN_MOTIF
  Nlm_Int2        extrah;
  Nlm_ShellTool   shl;
#endif

  wptr = Nlm_ParentWindowPtr (w);
  Nlm_GetWindowTitle (w, 0, title, sizeof (title));
#ifdef WIN_MAC
  Nlm_ResizeWindow (w, 20, 0, Nlm_StringWidth (title) + 70, 0, 0);
  Nlm_SetNotYetShown ((Nlm_WindoW) w, FALSE);
  if (setFlag) {
    Nlm_SetVisible (w, TRUE);
    ShowWindow (wptr);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_ResizeWindow (w, 0, 0, (Nlm_Int2)(Nlm_StringWidth(title) + 80), 0, 0);
  Nlm_SetNotYetShown ((Nlm_WindoW) w, FALSE);
  if (setFlag) {
    Nlm_SetVisible (w, TRUE);
    ShowWindow (wptr, SW_SHOWNORMAL);
    UpdateWindow (wptr);
  }
#endif
#ifdef WIN_MOTIF
  extrah = 0;
  if (Nlm_GetWindowMenuBar ((Nlm_WindoW) w) != NULL) {
    extrah += 31;
  }
  Nlm_ResizeWindow (w, 0, 0, Nlm_StringWidth (title) + 65, 0, extrah);
  Nlm_SetNotYetShown ((Nlm_WindoW) w, FALSE);
  if (setFlag) {
    Nlm_SetVisible (w, TRUE);
    shl = Nlm_GetWindowShell ((Nlm_WindoW) w);
    XtMapWidget (shl);
  }
#endif
  Nlm_Update ();
}


static void Nlm_SetWindowPosition (Nlm_GraphiC w, Nlm_RectPtr r,
                                   Nlm_Boolean savePort, Nlm_Boolean force)
{
  Nlm_Int2        height;
  Nlm_RecT        rct;
  Nlm_Int2        width;
  Nlm_WindowTool  wptr;
#ifdef WIN_MOTIF
  Nlm_MainTool    man;
  Nlm_ShellTool   shl;
#endif

  if (r == NULL)  return;

  rct = *r;
  width  = (Nlm_Int2)(rct.right  - rct.left);
  height = (Nlm_Int2)(rct.bottom - rct.top );
  if (Nlm_GetWindowMenuBar( (Nlm_WindoW)w ) != NULL)
    rct.bottom -= Nlm_internalMenuBarHeight;
  Nlm_SetRect (w, &rct);

  if ( !Nlm_GetRealized( w ) )  return;

  wptr = Nlm_GetWindowPtr ((Nlm_WindoW) w);
#ifdef WIN_MAC
    MoveWindow (wptr, rct.left, rct.top, FALSE);
    SizeWindow (wptr, width, height, TRUE);
#endif
#ifdef WIN_MSWIN
    {{
    RECT wr;
    client2win((Nlm_WindoW)w, &rct, &wr);
    MoveWindow(wptr, wr.left, wr.top,
               (wr.right - wr.left), (wr.bottom - wr.top), TRUE);
    }}
#endif
#ifdef WIN_MOTIF
    shl = Nlm_GetWindowShell ((Nlm_WindoW) w);
    man = Nlm_GetWindowMain  ((Nlm_WindoW) w);
    XtVaSetValues (shl,
                   XmNx, (Position) rct.left,
                   XmNy, (Position) rct.top,
                   XmNwidth,  (Dimension) width,
                   XmNheight, (Dimension) height,
                   NULL);
    XtVaSetValues (man,
                   XmNwidth,  (Dimension) width,
                   XmNheight, (Dimension) height,
                   NULL);
    XtVaSetValues (wptr,
                   XmNwidth,  (Dimension) width,
                   XmNheight, (Dimension) height,
                   NULL);
#endif
}


static void Nlm_GetWindowPosition (Nlm_GraphiC w, Nlm_RectPtr r)

{
  if (r != NULL) {
    Nlm_GetRect (w, r);
    if (Nlm_GetWindowMenuBar ((Nlm_WindoW) w) != NULL)
      r->bottom += Nlm_internalMenuBarHeight;
  }
}

extern void Nlm_RealizeWindow (Nlm_WindoW w)

{
  if (w != NULL) {
    Nlm_DoShow ((Nlm_GraphiC) w, FALSE, TRUE);
  }
}

extern void Nlm_IconifyWindow(Nlm_WindoW w)
{
#if defined(WIN_MOTIF)
  XtVaSetValues(Nlm_GetWindowShell(w), XtNiconic, (Boolean)True, NULL);
  Nlm_Update();
#elif defined(WIN_MSWIN)
  ShowWindow(Nlm_ParentWindowPtr((Nlm_GraphiC)w), SW_MINIMIZE);
  Nlm_SetVisible((Nlm_GraphiC)w, FALSE);
#endif
}


extern Nlm_Boolean Nlm_IconicWindow(Nlm_WindoW w)
{
#if defined(WIN_MOTIF)
  /* (X-technique from Ken Sall's function CheckWinMgrState() */
  unsigned long *property = NULL;
  unsigned long nitems;
  unsigned long leftover;
  Atom          xa_WM_STATE, actual_type;
  int           actual_format;
  int           status;

  Widget   shl = Nlm_GetWindowShell( w );
  Display *dpy = XtDisplay( shl );
  Window   win = XtWindow ( shl );

  if ( !win )
    return FALSE;

  xa_WM_STATE = XInternAtom(dpy, "WM_STATE", False);
  status  = XGetWindowProperty(dpy, win, xa_WM_STATE, 0L, 1L,
                               False, xa_WM_STATE,
                               &actual_type, &actual_format,
                               &nitems, &leftover,
                               (unsigned char **)&property);

  status = (status == Success  &&  actual_type == xa_WM_STATE  &&
            nitems == 1  &&  property  &&  *property == IconicState);

  if ( property )
    XFree( (char *)property );
  return (Nlm_Boolean)status;
#elif defined(WIN_MSWIN)
  return (Nlm_Boolean)IsIconic( Nlm_ParentWindowPtr((Nlm_GraphiC)w) );
#else
  return FALSE;
#endif
}


extern Nlm_WindoW Nlm_WhichWindow (Nlm_PoinT mouseLoc)
{
#ifdef WIN_MAC
  Nlm_PoinT       point;
  Nlm_PointTool   ptool;
  Nlm_Int2        windowLoc;
  Nlm_WindowTool  wptr;

  point = mouseLoc;
  Nlm_LocalToGlobal (&point);
  Nlm_PoinTToPointTool (point, &ptool);
  windowLoc = FindWindow (ptool, &wptr);
  return Nlm_FindWindowRec (wptr);
#endif
#ifdef WIN_MSWIN
  Nlm_PoinT       point;
  Nlm_PointTool   ptool;
  Nlm_WindoW      rsult;
  Nlm_WindowTool  wptr;

  rsult = NULL;
  point = mouseLoc;
  Nlm_LocalToGlobal (&point);
  Nlm_PoinTToPointTool (point, &ptool);
  wptr = WindowFromPoint (ptool);
  if ( wptr ) {
    rsult = (Nlm_WindoW) GetProp (wptr, (LPSTR) "Nlm_VibrantProp");
  }
  return rsult;
#endif
#ifdef WIN_MOTIF
  return NULL;
#endif
}

extern Nlm_Boolean Nlm_InWindow (Nlm_PoinT mouseLoc)

{
  return (Nlm_Boolean) (Nlm_WhichWindow (mouseLoc) != Nlm_desktopWindow);
}

extern Nlm_WindoW Nlm_FrontWindow(void)

{
#ifdef WIN_MAC
  return (Nlm_FindWindowRec (FrontWindow ()));
#endif
#ifdef WIN_MSWIN
  return (Nlm_FindWindowRec (Nlm_currentHWnd));
#endif
#ifdef WIN_MOTIF
  return NULL;
#endif
}

extern Nlm_Boolean Nlm_InFront (Nlm_WindoW w)

{
#ifdef WIN_MAC
  return (w == Nlm_FrontWindow ());
#endif
#ifdef WIN_MSWIN
  return FALSE;
#endif
#ifdef WIN_MOTIF
  return FALSE;
#endif
}

extern void Nlm_UseWindow (Nlm_WindoW w)

{
  Nlm_WindowTool  wptr;
#ifdef WIN_MOTIF
  Nlm_ShellTool   shl;
#endif

  if (w != NULL && w != Nlm_desktopWindow) {
    wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) w);
    if (wptr  &&  !Nlm_IsWindowDying( w )) {
#ifdef WIN_MAC
      SetPortWindowPort(wptr);
      Nlm_SetUpdateRegion (wptr);
      Nlm_ResetDrawingTools();
      Nlm_theWindow = w;
#endif
#ifdef WIN_MSWIN
      Nlm_currentHDC = Nlm_GetWindowPort (w);
      Nlm_currentHWnd = wptr;
#endif
#ifdef WIN_MOTIF
      shl = Nlm_GetWindowShell (w);
      Nlm_currentXWindow = XtWindow (shl);
      Nlm_currentXGC = Nlm_GetWindowPort (w);
#endif
      Nlm_currentWindowTool = wptr;
    }
  }
}

extern Nlm_WindoW Nlm_CurrentWindow(void)

{
  Nlm_WindoW  rsult;

  rsult = Nlm_FindWindowRec (Nlm_currentWindowTool);
  return rsult;
}

extern Nlm_Boolean Nlm_UsingWindow (Nlm_WindoW w)

{
  return (Nlm_Boolean) (w == Nlm_CurrentWindow ());
}

extern Nlm_WindoW Nlm_ActiveWindow (void)

{
  Nlm_WindoW      rsult;
#ifdef WIN_MSWIN
  Nlm_WindowTool  wptr;
#endif

  rsult = NULL;
#ifdef WIN_MAC
  rsult = Nlm_theWindow;
#endif
#ifdef WIN_MSWIN
  wptr = GetActiveWindow ();
  if (wptr != NULL) {
    rsult = (Nlm_WindoW) GetProp (wptr, (LPSTR) "Nlm_VibrantProp");
  }
#endif
#ifdef WIN_MOTIF
#endif
  return rsult;
}

#ifdef WIN_MAC
static Nlm_WindoW Nlm_PrevVisWindow (Nlm_WindoW w)

{
  WindowPtr  p;
  WindowPtr  q;
  WindowPtr  t;

  if (frontWindow == Nlm_desktopWindow) {
    return Nlm_desktopWindow;
  } else if (frontWindow == w) {
    return frontWindow;
  } else {
    p = Nlm_ParentWindowPtr ((Nlm_GraphiC) frontWindow);
    q = p;
    t = Nlm_ParentWindowPtr ((Nlm_GraphiC) w);
    while (p != NULL) {
      if (p == t) {
        p = NULL;
      } else if (IsWindowVisible(p)) {
        q = p;
        p = GetNextWindow(p);
      } else {
        p = GetNextWindow(p);
      }
    }
    return (Nlm_FindWindowRec ((Nlm_WindowTool) q));
  }
}
#endif


static void Nlm_SelectWindow(Nlm_GraphiC w, Nlm_Boolean savePort)

{
  Nlm_WindowTool  wptr;

  if (w == NULL  ||  (Nlm_WindoW)w == Nlm_desktopWindow)
    return;

  wptr = Nlm_ParentWindowPtr( w );
  if (wptr == NULL  ||  Nlm_IsWindowDying( (Nlm_WindoW)w ))
    return;

#ifdef WIN_MAC
  SelectWindow (wptr);
  SetPortWindowPort(wptr);
  Nlm_SetUpdateRegion(wptr);
  Nlm_ResetDrawingTools();
#endif
#ifdef WIN_MSWIN
  BringWindowToTop (wptr);
/* M.I */
  if( IsIconic( wptr ) )
    ShowWindow( wptr, SW_RESTORE );  /* de-Iconize window */
/* M.I */
  Nlm_currentHDC = Nlm_GetWindowPort ((Nlm_WindoW) w);
  Nlm_currentHWnd = wptr;
#endif
#ifdef WIN_MOTIF
  selectShell = Nlm_GetWindowShell( (Nlm_WindoW)w );
  if (Nlm_currentXDisplay == NULL  ||  !XtIsRealized( selectShell ))
    return;

  {{
    Window selectWin = XtWindow( selectShell );
    Window focus_win;
    int revert_to;
    
    XMapRaised(Nlm_currentXDisplay, selectWin);
    XGetInputFocus(Nlm_currentXDisplay, &focus_win, &revert_to);
    if (focus_win != selectWin)
      {
        XWindowAttributes attr;
        XGetWindowAttributes(Nlm_currentXDisplay, selectWin, &attr);
        if (attr.map_state != IsViewable)
          return;
        
        XSetInputFocus(Nlm_currentXDisplay, selectWin,
                       RevertToPointerRoot, CurrentTime);
        selectShell = NULL;
      }
  }}
  Nlm_UseWindow( (Nlm_WindoW)w );
#endif

  Nlm_currentWindowTool = wptr;
  Nlm_Update ();
}


extern void Nlm_PopupParentWindow(Nlm_Handle a)
{
  Nlm_WindoW     w = Nlm_ParentWindow( a );
  Nlm_WindowTool wptr;

  if (w == NULL  ||  w == Nlm_desktopWindow  ||  Nlm_IsWindowDying( w ))
    return;

  if ( !Nlm_Visible( w ) )
    {
      Nlm_Show( w );
      return;
    }

  wptr = Nlm_ParentWindowPtr( (Nlm_GraphiC)w );
  if ( !wptr )
    return;

#ifdef WIN_MAC
  SelectWindow (wptr);
  Nlm_SetUpdateRegion (wptr);
  Nlm_ResetDrawingTools ();
#endif
#ifdef WIN_MSWIN
  if( IsIconic(wptr) )
    ShowWindow(wptr, SW_RESTORE);
  else
    SetWindowPos(wptr, HWND_TOP, 0, 0, 0, 0,
                 SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOMOVE|SWP_SHOWWINDOW);
#endif
#ifdef WIN_MOTIF
  {{
    Widget selShell = Nlm_GetWindowShell( w );
    if (Nlm_currentXDisplay == NULL  ||  !XtIsRealized( selShell ))
      return;

    XMapRaised(Nlm_currentXDisplay, XtWindow( selShell ));
  }}
#endif

  Nlm_Update ();
}


extern void Nlm_EraseWindow (Nlm_WindoW w)

{
#ifdef WIN_MAC
  Nlm_RecT        r;
  Nlm_PortTool    temp;
  Nlm_WindowTool  wptr;
  Rect bounds;

  if (w != NULL) {
    wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) w);
    GetPort(&temp);
    SetPortWindowPort(wptr);
    Nlm_ResetDrawingTools();
    Nlm_currentWindowTool = wptr;
    GetPortBounds(GetWindowPort(wptr), &bounds);
    Nlm_RectToolToRecT (&bounds, &r);
    Nlm_EraseRect(&r);
    SetPort(temp);
    Nlm_currentWindowTool = GetWindowFromPort(temp);
    Nlm_Update();
  }
#endif
#ifdef WIN_MSWIN
#endif
#ifdef WIN_MOTIF
#endif
}

#ifdef WIN_MAC
static Nlm_Boolean Nlm_DragClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  Nlm_PointTool   ptool;
  Nlm_RecT        r;
  Nlm_RectTool    rtool;
  Nlm_Boolean     rsult;
  Nlm_Int2        windowLoc;
  Nlm_WindowData  wdata;
  Nlm_WindowTool  wptr;
  Rect bounds;

  rsult = FALSE;
  Nlm_PoinTToPointTool (Nlm_globalMouse, &ptool);
  windowLoc = FindWindow (ptool, &wptr);
  if (windowLoc == inDrag) {
    Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
    Nlm_RecTToRectTool (&(wdata.dragArea), &rtool);
    DragWindow (wptr, ptool, &rtool);
    GetPortBounds(GetWindowPort(wptr), &bounds);
    Nlm_RectToolToRecT (&bounds, &r);
    Nlm_LocalToGlobal ((Nlm_PointPtr) &(r.left));
    Nlm_LocalToGlobal ((Nlm_PointPtr) &(r.right));
    Nlm_SetRect (w, &r);
    rsult = TRUE;
  }
  return rsult;
}

static void Nlm_UpdateScrollBar (Nlm_GraphiC w)

{
  Nlm_RecT        barArea;
  Nlm_WindowTool  wptr;
  Rect bounds;

  wptr = Nlm_ParentWindowPtr (w);
  GetPortBounds(GetWindowPort(wptr), &bounds);
  Nlm_RectToolToRecT (&bounds, &barArea);
  barArea.left = barArea.right - 16;
  if (Nlm_GetWindowMenuBar ((Nlm_WindoW) w) != NULL) {
    barArea.top = barArea.top + 21;
  }
  Nlm_InvalRect (&barArea);
  Nlm_RectToolToRecT (&bounds, &barArea);
  barArea.top = barArea.bottom - 16;
  Nlm_InvalRect (&barArea);
}

static Nlm_Boolean Nlm_GrowClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  Nlm_Int2         ht;
  Nlm_Int4         newSize;
  Nlm_PointTool    ptool;
  Nlm_RecT         r;
  Nlm_WndActnProc  resize;
  Nlm_RectTool     rtool;
  Nlm_Boolean      rsult;
  Nlm_Int2         wd;
  Nlm_WindowData   wdata;
  Nlm_Int2         windowLoc;
  Nlm_WindowTool   wptr;
  Rect bounds;

  rsult = FALSE;
  Nlm_PoinTToPointTool (Nlm_globalMouse, &ptool);
  windowLoc = FindWindow (ptool, &wptr);
  if (windowLoc == inGrow) {
    Nlm_LoadRect (&r, -32768, -32768, 32767, 32767);
    Nlm_RecTToRectTool (&r, &rtool);
    ClipRect (&rtool);
    Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
    Nlm_RecTToRectTool (&(wdata.growArea), &rtool);
    newSize = GrowWindow (wptr, ptool, &rtool);
    wd = LoWord (newSize);
    ht = HiWord (newSize);
 /* dgg -- bug fix: simple click on grow box caused it to resize to 0 --
    should check here for 0 == no change.  Also make minsize bigger.
 */
    GetPortBounds(GetWindowPort(wptr), &bounds);
    if (! newSize) {
      ClipRect (&bounds);
      return rsult;
    }
    if (wd < 50) {
      wd = 50;
    }
    if (ht < 32) {
      ht = 32;
    }
    bounds.right = bounds.left + wd;
    bounds.bottom = bounds.top + ht;
    Nlm_UpdateScrollBar (w);
    SizeWindow (wptr, wd, ht, TRUE);
    Nlm_UpdateScrollBar (w);
    wptr = Nlm_GetWindowPtr ((Nlm_WindoW) w);
    Nlm_RectToolToRecT (&bounds, &r);
    ClipRect (&bounds);
    Nlm_LocalToGlobal ((Nlm_PointPtr) &(r.left));
    Nlm_LocalToGlobal ((Nlm_PointPtr) &(r.right));
    Nlm_SetRect (w, &r);
    resize = wdata.resize;
    if (resize != NULL) {
      resize ((Nlm_WindoW) w);
    }
    rsult = TRUE;
  }
  return rsult;
}

static void Nlm_DrawGrowIcon (Nlm_GraphiC w, Nlm_Boolean drawgrow, Nlm_Boolean drawbar)

{
  Nlm_RecT        r;
  Nlm_RectTool    rtool;
  PenState        state;
  Nlm_PortTool    temp;
  Nlm_WindowTool  wptr;
  Rect bounds;

  GetPort (&temp);
  GetPenState (&state);
  wptr = Nlm_ParentWindowPtr (w);
  GetPortBounds(GetWindowPort(wptr), &bounds);
  Nlm_RectToolToRecT (&bounds, &r);
  if (drawbar && Nlm_GetWindowMenuBar ((Nlm_WindoW) w) != NULL) {
    Nlm_RecTToRectTool (&r, &rtool);
    ClipRect (&rtool);
    MoveTo (r.right - 16, 20);
    LineTo (r.right, 20);
    r.top = r.top + 21;
  }
  if (drawgrow) {
    Nlm_RecTToRectTool (&r, &rtool);
    rtool.left = rtool.right - 15;
    rtool.top = rtool.bottom - 15;
    ClipRect (&rtool);
    DrawGrowIcon (wptr);
  }
  Nlm_RecTToRectTool (&r, &rtool);
  r.top = bounds.top;
#ifdef DCLAP
  /* dgg- this cliprect is the culprit for preventing scrollbar updates by subviews */
  /* try clipping a region that excludes just the growicon?? */
#else
  /*
  r.right = r.right - 16;
  r.bottom = r.bottom - 16;
  */
#endif
  Nlm_RecTToRectTool (&r, &rtool);
  ClipRect (&rtool);
  SetPort(temp);
  Nlm_currentWindowTool = GetWindowFromPort(temp);
  Nlm_SetUpdateRegion (GetWindowFromPort(temp));
  SetPenState (&state);
}

static Nlm_Boolean Nlm_ZoomClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  Nlm_PointTool    ptool;
  Nlm_RecT         r;
  Nlm_WndActnProc  resize;
  Nlm_Boolean      rsult;
  Nlm_WindowData   wdata;
  Nlm_Int2         windowLoc;
  Nlm_WindowTool   wptr;

  rsult = FALSE;
  Nlm_PoinTToPointTool (Nlm_globalMouse, &ptool);
  windowLoc = FindWindow (ptool, &wptr);
  if (windowLoc == inZoomIn || windowLoc == inZoomOut) {
#if OPAQUE_TOOLBOX_STRUCTS
    Point idealSize;
    int part;
#else
    Rect stdRect;
    WStateDataHandle wshdl;
#endif
    Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
    r = wdata.zoomArea;
#if OPAQUE_TOOLBOX_STRUCTS
    idealSize.v = r.bottom - r.top;
    idealSize.h = r.right - r.left;
    
    part = IsWindowInStandardState(wptr, &idealSize, NULL) ? inZoomIn : inZoomOut;
    windowLoc = part;
#endif
    
    if (TrackBox (wptr, ptool, windowLoc)) {
      Rect bounds;
      // WindowPeek::dataHandle is not supported under Carbon.
#if OPAQUE_TOOLBOX_STRUCTS
      ZoomWindowIdeal(wptr, part, &idealSize);
#else
      wshdl = (WStateDataHandle) ((WindowPeek)wptr)->dataHandle;
      Nlm_RecTToRectTool (&r, &stdRect);
      (*wshdl)->stdState = stdRect;
      GetPortBounds(GetWindowPort(wptr), &bounds);
      Nlm_RectToolToRecT (&bounds, &r);
      ClipRect (&bounds);
      Nlm_EraseRect (&r);
      ZoomWindow (wptr, windowLoc, FALSE);
#endif
      GetPortBounds(GetWindowPort(wptr), &bounds);
      Nlm_UpdateScrollBar (w);
      wptr = Nlm_GetWindowPtr ((Nlm_WindoW) w);
      Nlm_RectToolToRecT (&bounds, &r);
      ClipRect (&bounds);
      Nlm_LocalToGlobal ((Nlm_PointPtr) &(r.left));
      Nlm_LocalToGlobal ((Nlm_PointPtr) &(r.right));
      Nlm_SetRect (w, &r);
      resize = wdata.resize;
      if (resize != NULL) {
        resize ((Nlm_WindoW) w);
      }
    }
    rsult = TRUE;
  }
  return rsult;
}

static Nlm_Boolean Nlm_CloseClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  Nlm_WndActnProc  cls;
  Nlm_PointTool    ptool;
  Nlm_Boolean      rsult;
  Nlm_WindowData   wdata;
  Nlm_Int2         windowLoc;
  Nlm_WindowTool   wptr;

  rsult = FALSE;
  Nlm_PoinTToPointTool (Nlm_globalMouse, &ptool);
  windowLoc = FindWindow (ptool, &wptr);
  if (windowLoc == inGoAway) {
    if (TrackGoAway (wptr, ptool)) {
      Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
      cls = wdata.close;
      if (cls != NULL) {
        cls ((Nlm_WindoW) w);
      }
    }
    rsult = TRUE;
  }
  return rsult;
}

typedef struct revItem {
  Nlm_GraphiC          graphic;
  struct revItem  PNTR next;
} Nlm_RevItem, PNTR Nlm_RevPtr;

static Nlm_Boolean Nlm_ContentClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  Nlm_GraphiC    g;
  Nlm_MenuBaR    mb;
  Nlm_RevPtr     next;
  Nlm_Boolean    notInside;
  Nlm_PointTool  ptool;
  Nlm_RevPtr     this;
  Nlm_RevPtr     top;

  Nlm_localMouse = Nlm_globalMouse;
  Nlm_GlobalToLocal (&Nlm_localMouse);
  g = Nlm_GetChild (w);
  notInside = TRUE;
  mb = Nlm_GetWindowMenuBar ((Nlm_WindoW) w);
  if (mb != NULL) {
    if (Nlm_GetEnabled ((Nlm_GraphiC) mb) && Nlm_GetVisible ((Nlm_GraphiC) mb) &&
        Nlm_DoClick ((Nlm_GraphiC) mb, Nlm_localMouse)) {
      notInside = FALSE;
    }
  }
  /*
  while (g != NULL && notInside) {
    n = Nlm_GetNext (g);
    if (Nlm_GetEnabled (g) && Nlm_GetVisible (g) && Nlm_DoClick (g, Nlm_localMouse)) {
      notInside = FALSE;
    }
    g = n;
  }
  */
  if (g != NULL && notInside) {
    top = NULL;
    while (g != NULL) {
      this = Nlm_MemNew (sizeof (Nlm_RevItem));
      if (this != NULL) {
        this->graphic = g;
        this->next = top;
        top = this;
      }
      g = Nlm_GetNext (g);
    }
    this = top;
    while (this != NULL && notInside) {
      next = this->next;
      g = this->graphic;
      if (Nlm_GetEnabled (g) && Nlm_GetVisible (g) && Nlm_DoClick (g, Nlm_localMouse)) {
        notInside = FALSE;
      }
      this = next;
    }
    this = top;
    while (this != NULL) {
      next = this->next;
      Nlm_MemFree (this);
      this = next;
    }
  }
  if (notInside) {
    GetMouse (&ptool);
    Nlm_PointToolToPoinT (ptool, &Nlm_localMouse);
    if ((Nlm_WindoW) w != Nlm_WhichWindow (Nlm_localMouse)) {
      notInside = FALSE;
    }
  }
  return (! notInside);
}

static Nlm_Boolean Nlm_CommonClick (Nlm_GraphiC w, Nlm_PoinT pt,
                                    Nlm_Boolean close, Nlm_Boolean drag,
                                    Nlm_Boolean grow, Nlm_Boolean zoom)

{
  Nlm_Boolean     rsult;
  PenState        state;
  Nlm_PortTool    temp;
  Nlm_WindowTool  wptr;

  rsult = FALSE;
  if (chosenWindow == (Nlm_WindoW) w) {
    wptr = Nlm_ParentWindowPtr (w);
    GetPort (&temp);
    GetPenState (&state);
    SetPortWindowPort(wptr);
    Nlm_currentWindowTool = wptr;
    Nlm_SetUpdateRegion (wptr);
    Nlm_ResetDrawingTools ();
    if ((close && Nlm_CloseClick (w, pt)) ||
       (drag && Nlm_DragClick (w, pt)) ||
       (grow && Nlm_GrowClick (w, pt)) ||
       (zoom && Nlm_ZoomClick (w, pt))) {
      rsult = TRUE;
    } else {
      rsult = Nlm_ContentClick (w, pt);
    }
    SetPort(temp);
    Nlm_currentWindowTool = GetWindowFromPort(temp);
    Nlm_SetUpdateRegion (GetWindowFromPort(temp));
    SetPenState (&state);
  } else if (chosenWindow != NULL) {
    Nlm_DoSelect ((Nlm_GraphiC) chosenWindow, TRUE);
  }
  return rsult;
}

static Nlm_Boolean Nlm_DocumentClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  return Nlm_CommonClick (w, pt, TRUE, TRUE, TRUE, TRUE);
}

static Nlm_Boolean Nlm_DialogClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  return Nlm_CommonClick (w, pt, TRUE, TRUE, FALSE, FALSE);
}

static Nlm_Boolean Nlm_FrozenClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  return Nlm_CommonClick (w, pt, TRUE, FALSE, FALSE, FALSE);
}

static Nlm_Boolean Nlm_PlainClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  return Nlm_CommonClick (w, pt, FALSE, FALSE, FALSE, FALSE);
}

static Nlm_Boolean Nlm_ModalClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  Nlm_WndActnProc  cls;
  Nlm_Boolean      rsult;
  PenState         state;
  Nlm_PortTool     temp;
  Nlm_WindowData   wdata;
  Nlm_WindowTool   wptr;

  rsult = FALSE;
  if (chosenWindow == (Nlm_WindoW) w) {
    rsult = Nlm_CommonClick (w, pt, FALSE, FALSE, FALSE, FALSE);
    if (! rsult) {
      Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
      cls = wdata.close;
      if (cls != NULL) {
        wptr = Nlm_ParentWindowPtr (w);
        GetPort (&temp);
        GetPenState (&state);
        SetPortWindowPort(wptr);
        Nlm_currentWindowTool = wptr;
        Nlm_SetUpdateRegion (wptr);
        Nlm_ResetDrawingTools ();
        cls ((Nlm_WindoW) w);
        SetPort(temp);
        Nlm_currentWindowTool = GetWindowFromPort(temp);
        Nlm_SetUpdateRegion (GetWindowFromPort(temp));
        SetPenState (&state);
      }
    }
  } else {
    Nlm_Beep ();
  }
  return rsult;
}

static Nlm_Boolean Nlm_MovableModalClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  Nlm_WndActnProc  cls;
  Nlm_Boolean      rsult;
  PenState         state;
  Nlm_PortTool     temp;
  Nlm_WindowData   wdata;
  Nlm_WindowTool   wptr;

  rsult = FALSE;
  if (chosenWindow == (Nlm_WindoW) w) {
    rsult = Nlm_CommonClick (w, pt, FALSE, TRUE, FALSE, FALSE);
    if (! rsult) {
      Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
      cls = wdata.close;
      if (cls != NULL) {
        wptr = Nlm_ParentWindowPtr (w);
        GetPort (&temp);
        GetPenState (&state);
        SetPortWindowPort(wptr);
        Nlm_currentWindowTool = wptr;
        Nlm_SetUpdateRegion (wptr);
        Nlm_ResetDrawingTools ();
        cls ((Nlm_WindoW) w);
        SetPort(temp);
        Nlm_currentWindowTool = GetWindowFromPort(temp);
        Nlm_SetUpdateRegion(GetWindowFromPort(temp));
        SetPenState (&state);
      }
    }
  } else {
    Nlm_Beep ();
  }
  return rsult;
}

static Nlm_Boolean Nlm_FloatingClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  Nlm_WindoW  nw;

  if (chosenWindow == (Nlm_WindoW) w) {
    Nlm_CommonClick (w, Nlm_globalMouse, TRUE, TRUE, FALSE, FALSE);
  } else {
    nw = Nlm_NextVisWindow ((Nlm_WindoW) w);
    Nlm_DoClick ((Nlm_GraphiC) nw, Nlm_globalMouse);
  }
  return TRUE;
}

// 2001-03-22:  Joshua Juran
// SystemClick() is not supported in Carbon.  It's unnecessary.
#if TARGET_API_MAC_CARBON
# define SystemClick(event, window)
#endif
static Nlm_Boolean Nlm_SystemClick (Nlm_GraphiC w, Nlm_PoinT pt)
{
  Nlm_WindowTool  wptr;

  if (chosenWindow == (Nlm_WindoW) w) {
    wptr = Nlm_ParentWindowPtr (w);
    SystemClick (&Nlm_currentEvent, wptr);
  } else {
    Nlm_DoSelect ((Nlm_GraphiC) chosenWindow, TRUE);
  }
  return TRUE;
}

static Nlm_Boolean Nlm_DesktopClick (Nlm_GraphiC w, Nlm_PoinT pt)

{
  Nlm_MenuBaR  mb;

  mb = Nlm_GetWindowMenuBar ((Nlm_WindoW) w);
  Nlm_localMouse = Nlm_globalMouse;
  Nlm_GlobalToLocal (&Nlm_localMouse);
  (void) (Nlm_GetEnabled ((Nlm_GraphiC) mb) && Nlm_GetVisible ((Nlm_GraphiC) mb) &&
          Nlm_DoClick ((Nlm_GraphiC) mb, Nlm_localMouse));
  return TRUE;
}
#endif

static void Nlm_NormalSelect (Nlm_GraphiC w, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  if ((! Nlm_DragClick (w, Nlm_localMouse)) || (! Nlm_cmmdKey)) {
    Nlm_SelectWindow (w, FALSE);
  }
#endif
#ifdef WIN_MSWIN
  Nlm_SelectWindow (w, FALSE);
#endif
#ifdef WIN_MOTIF
  Nlm_SelectWindow (w, FALSE);
#endif
}

static void Nlm_PlainSelect (Nlm_GraphiC w, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  Nlm_SelectWindow (w, FALSE);
#endif
#ifdef WIN_MSWIN
  Nlm_SelectWindow (w, FALSE);
#endif
#ifdef WIN_MOTIF
  Nlm_SelectWindow (w, FALSE);
#endif
}

static void Nlm_FloatingSelect (Nlm_GraphiC w, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  PenState        state;
  Nlm_PortTool    temp;
  Nlm_WindowTool  wptr;

  wptr = Nlm_ParentWindowPtr (w);
  GetPort (&temp);
  GetPenState (&state);
  SetPortWindowPort(wptr);
  Nlm_currentWindowTool = wptr;
  Nlm_SetUpdateRegion (wptr);
  Nlm_ResetDrawingTools ();
  Nlm_localMouse = Nlm_globalMouse;
  Nlm_GlobalToLocal (&Nlm_localMouse);
  if (Nlm_DragClick (w, Nlm_globalMouse) || Nlm_CloseClick (w, Nlm_globalMouse)) {
  } else {
    Nlm_ContentClick (w, Nlm_globalMouse);
  }
  SetPort(temp);
  Nlm_currentWindowTool = GetWindowFromPort(temp);
  Nlm_SetUpdateRegion (GetWindowFromPort(temp));
  SetPenState (&state);
  Nlm_localMouse = Nlm_globalMouse;
  Nlm_GlobalToLocal (&Nlm_localMouse);
#endif
#ifdef WIN_MSWIN
  Nlm_SelectWindow (w, FALSE);
#endif
#ifdef WIN_MOTIF
  Nlm_SelectWindow (w, FALSE);
#endif
}

static void Nlm_DesktopSelect (Nlm_GraphiC w, Nlm_Boolean savePort)

{
#ifdef WIN_MAC
  PenState      state;
  Nlm_PortTool  temp;

  GetPort (&temp);
  GetPenState (&state);
  PenNormal ();
  Nlm_DesktopClick (w, Nlm_globalMouse);
  SetPort(temp);
  Nlm_currentWindowTool = GetWindowFromPort(temp);
  Nlm_SetUpdateRegion (GetWindowFromPort(temp));
  SetPenState (&state);
#endif
}

#ifdef WIN_MAC
static Nlm_Boolean Nlm_DesktopKey (Nlm_GraphiC w, Nlm_Char ch)

{
  Nlm_MenuBaR  mb;

  mb = Nlm_GetWindowMenuBar ((Nlm_WindoW) w);
  Nlm_localMouse = Nlm_globalMouse;
  Nlm_GlobalToLocal (&Nlm_localMouse);
  if (Nlm_currentKey != '\0') {
    (void) (Nlm_GetEnabled ((Nlm_GraphiC) mb) && Nlm_GetVisible ((Nlm_GraphiC) mb) &&
            Nlm_DoKey ((Nlm_GraphiC) mb, ch));
  }
  return TRUE;
}

static Nlm_Boolean Nlm_NormalKey (Nlm_GraphiC w, Nlm_Char ch)

{
  Nlm_GraphiC     g;
  Nlm_MenuBaR     mb;
  Nlm_GraphiC     n;
  Nlm_Boolean     notInside;
  PenState        state;
  Nlm_PortTool    temp;
  Nlm_WindowTool  wptr;

  wptr = Nlm_ParentWindowPtr (w);
  GetPort (&temp);
  GetPenState (&state);
  SetPortWindowPort(wptr);
  Nlm_currentWindowTool = wptr;
  Nlm_SetUpdateRegion (wptr);
  Nlm_ResetDrawingTools ();
  Nlm_localMouse = Nlm_globalMouse;
  Nlm_GlobalToLocal (&Nlm_localMouse);
  if (Nlm_currentKey != '\0') {
    g = Nlm_GetChild (w);
    notInside = TRUE;
    mb = Nlm_GetWindowMenuBar ((Nlm_WindoW) w);
    if (mb != NULL) {
      if (Nlm_GetEnabled ((Nlm_GraphiC) mb) && Nlm_GetVisible ((Nlm_GraphiC) mb) &&
          Nlm_DoKey ((Nlm_GraphiC) mb, ch)) {
        notInside = FALSE;
      }
    }
    while (g != NULL && notInside) {
      n = Nlm_GetNext (g);
      if (Nlm_GetEnabled (g) && Nlm_GetVisible (g) && Nlm_DoKey (g, ch)) {
        notInside = FALSE;
      }
      g = n;
    }
    if (notInside && Nlm_cmmdKey) {
      Nlm_DesktopKey ((Nlm_GraphiC) Nlm_desktopWindow, ch);
    }
  }
  SetPort(temp);
  Nlm_currentWindowTool = GetWindowFromPort(temp);
  Nlm_SetUpdateRegion(GetWindowFromPort(temp));
  SetPenState (&state);
  return TRUE;
}

static Nlm_Boolean Nlm_FloatingKey (Nlm_GraphiC w, Nlm_Char ch)

{
  Nlm_GraphiC     g;
  Nlm_MenuBaR     mb;
  Nlm_GraphiC     n;
  Nlm_WindoW      nw;
  Nlm_Boolean     notInside;
  PenState        state;
  Nlm_PortTool    temp;
  Nlm_WindowTool  wptr;

  wptr = Nlm_ParentWindowPtr (w);
  GetPort (&temp);
  GetPenState (&state);
  SetPortWindowPort(wptr);
  Nlm_currentWindowTool = wptr;
  Nlm_SetUpdateRegion (wptr);
  Nlm_ResetDrawingTools ();
  Nlm_localMouse = Nlm_globalMouse;
  Nlm_GlobalToLocal (&Nlm_localMouse);
  if (Nlm_currentKey != '\0') {
    g = Nlm_GetChild (w);
    notInside = TRUE;
    mb = Nlm_GetWindowMenuBar ((Nlm_WindoW) w);
    if (mb != NULL) {
      if (Nlm_GetEnabled ((Nlm_GraphiC) mb) && Nlm_GetVisible ((Nlm_GraphiC) mb) &&
          Nlm_DoKey ((Nlm_GraphiC) mb, ch)) {
        notInside = FALSE;
      }
    }
    while (g != NULL && notInside) {
      n = Nlm_GetNext (g);
      if (Nlm_GetEnabled (g) && Nlm_GetVisible (g) && Nlm_DoKey (g, ch)) {
        notInside = FALSE;
      }
      g = n;
    }
    if (notInside) {
      nw = Nlm_NextVisWindow ((Nlm_WindoW) w);
      Nlm_DoKey ((Nlm_GraphiC) nw, ch);
    }
  }
  SetPort(temp);
  Nlm_currentWindowTool = GetWindowFromPort(temp);
  Nlm_SetUpdateRegion(GetWindowFromPort(temp));
  SetPenState (&state);
  return TRUE;
}

static Nlm_Boolean Nlm_ModalKey (Nlm_GraphiC w, Nlm_Char ch)

{
  if (Nlm_cmmdKey) {
    Nlm_Beep ();
  } else {
    Nlm_NormalKey (w, ch);
  }
  return TRUE;
}

static void Nlm_DrawWindow (Nlm_GraphiC w, Nlm_Boolean drawGrowIcon)

{
  Nlm_GraphiC     g;
  Nlm_MenuBaR     mb;
  Nlm_GraphiC     n;
  Nlm_RecT        r;
  PenState        state;
  Nlm_PortTool    temp;
  Nlm_WindowTool  wptr;
  Rect bounds;

  wptr = Nlm_ParentWindowPtr (w);
  GetPort (&temp);
  GetPenState (&state);
  SetPortWindowPort(wptr);
  Nlm_currentWindowTool = wptr;

  Nlm_ResetDrawingTools ();
  Nlm_ResetClip ();
  PenNormal ();

  BeginUpdate (wptr);
  Nlm_SetUpdateRegion (wptr);
  GetPortBounds(GetWindowPort(wptr), &bounds);
  Nlm_RectToolToRecT (&bounds, &r);
  Nlm_EraseRect (&r);

  if (okayToDrawContents) {
    if (drawGrowIcon) {
      Nlm_DrawGrowIcon (w, FALSE, TRUE);
    }
    mb = Nlm_GetWindowMenuBar ((Nlm_WindoW) w);
    if (mb != NULL) {
      Nlm_DoDraw ((Nlm_GraphiC) mb);
    }
    g = Nlm_GetChild (w);
    while (g != NULL) {
      n = Nlm_GetNext (g);
      Nlm_DoDraw (g);
      g = n;
    }
    if (drawGrowIcon) {
      Nlm_DrawGrowIcon (w, TRUE, FALSE);
    }
  }

  EndUpdate (wptr);
  Nlm_ResetDrawingTools ();
  Nlm_ResetClip ();
  SetPort(temp);
  Nlm_currentWindowTool = GetWindowFromPort(temp);
  Nlm_SetUpdateRegion(GetWindowFromPort(temp));
  SetPenState (&state);
}

static void Nlm_DocumentDraw (Nlm_GraphiC w)

{
  Nlm_DrawWindow (w, TRUE);
}

static void Nlm_NormalDraw (Nlm_GraphiC w)

{
  Nlm_DrawWindow (w, FALSE);
}
#endif

static void Nlm_ActivateWindow (Nlm_GraphiC w, Nlm_Boolean drawGrow)

{
  Nlm_WndActnProc  act;
  Nlm_GraphiC      g;
  Nlm_MenuBaR      mb;
  Nlm_GraphiC      n;
  Nlm_WindowData   wdata;

  Nlm_UseWindow ((Nlm_WindoW) w);
#ifdef WIN_MAC
  if (drawGrow) {
    Nlm_DrawGrowIcon (w, TRUE, TRUE);
  }
#endif
  mb = Nlm_GetWindowMenuBar ((Nlm_WindoW) w);
  if (mb != NULL) {
    Nlm_DoActivate ((Nlm_GraphiC) mb, FALSE);
  }
  g = Nlm_GetChild (w);
  while (g != NULL) {
    n = Nlm_GetNext (g);
    Nlm_DoActivate (g, FALSE);
    g = n;
  }
  Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
  act = wdata.activate;
  if (act != NULL) {
    act ((Nlm_WindoW) w);
  }
  if (appWndAct != NULL) {
    appWndAct ((Nlm_WindoW) w);
  }
}

static void Nlm_DocumentActivate (Nlm_GraphiC w, Nlm_Boolean savePort)

{
  Nlm_ActivateWindow (w, TRUE);
}

static void Nlm_NormalActivate (Nlm_GraphiC w, Nlm_Boolean savePort)

{
  Nlm_ActivateWindow (w, FALSE);
}

static void Nlm_DeactivateWindow (Nlm_GraphiC w, Nlm_Boolean drawGrow)

{
  Nlm_WndActnProc  deact;
  Nlm_GraphiC      g;
  Nlm_MenuBaR      mb;
  Nlm_GraphiC      n;
  Nlm_WindowData   wdata;

  Nlm_UseWindow ((Nlm_WindoW) w);
#ifdef WIN_MAC
  if (drawGrow) {
    Nlm_DrawGrowIcon (w, TRUE, TRUE);
  }
#endif
  mb = Nlm_GetWindowMenuBar ((Nlm_WindoW) w);
  if (mb != NULL) {
    Nlm_DoDeactivate ((Nlm_GraphiC) mb, FALSE);
  }
  g = Nlm_GetChild (w);
  while (g != NULL) {
    n = Nlm_GetNext (g);
    Nlm_DoDeactivate (g, FALSE);
    g = n;
  }
  Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
  deact = wdata.deactivate;
  if (deact != NULL) {
    deact ((Nlm_WindoW) w);
  }
  if (appWndDeact != NULL) {
    appWndDeact ((Nlm_WindoW) w);
  }
}

static void Nlm_DocumentDeactivate (Nlm_GraphiC w, Nlm_Boolean savePort)

{
  Nlm_DeactivateWindow (w, TRUE);
}

static void Nlm_NormalDeactivate (Nlm_GraphiC w, Nlm_Boolean savePort)

{
  Nlm_DeactivateWindow (w, FALSE);
}

#ifdef WIN_MAC
static Nlm_Boolean Nlm_NormalIdle (Nlm_GraphiC w, Nlm_PoinT pt)

{
  Nlm_GraphiC  g;
  Nlm_GraphiC  n;

  g = Nlm_GetChild (w);
  while (g != NULL) {
    n = Nlm_GetNext (g);
    Nlm_DoIdle (g, Nlm_localMouse);
    g = n;
  }
  return TRUE;
}

static Nlm_Boolean Nlm_FloatingIdle (Nlm_GraphiC w, Nlm_PoinT pt)

{
  Nlm_WindoW   nw;

  nw = Nlm_NextVisWindow ((Nlm_WindoW) w);
  Nlm_DoIdle ((Nlm_GraphiC) nw, pt);
  return TRUE;
}
#endif

static void Nlm_NormalDrawChar (Nlm_GraphiC w, Nlm_Char ch, Nlm_Boolean savePort)

{
  Nlm_DisplaY     d;
  Nlm_WindowData  wdata;

  Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
  d = wdata.charDisplay;
  if (d != NULL) {
    Nlm_DoSendChar ((Nlm_GraphiC) d, ch, TRUE);
  }
}

static void Nlm_FloatingDrawChar (Nlm_GraphiC w, Nlm_Char ch, Nlm_Boolean savePort)

{
  Nlm_DisplaY     d;
  Nlm_WindowData  wdata;
#ifdef WIN_MAC
  Nlm_WindoW      nw;
#endif

  Nlm_GetWindowData ((Nlm_WindoW) w, &wdata);
  d = wdata.charDisplay;
  if (d != NULL) {
    Nlm_DoSendChar ((Nlm_GraphiC) d, ch, TRUE);
  } else {
#ifdef WIN_MAC
    nw = Nlm_NextVisWindow ((Nlm_WindoW) w);
    Nlm_DoSendChar ((Nlm_GraphiC) nw, ch, TRUE);
#endif
#ifdef WIN_MOTIF
#endif
  }
}

static void Nlm_AdjustWindow (Nlm_GraphiC w, Nlm_RectPtr r,
                              Nlm_Boolean align, Nlm_Boolean savePort)

{
  Nlm_GraphiC  g;
  Nlm_WindoW   tempPort;

  tempPort = Nlm_SavePortIfNeeded (w, savePort);
  if (align) {
    g = Nlm_GetParent (w);
    Nlm_RecordRect (g, r);
    Nlm_NextPosition (g, r);
  }
  Nlm_RestorePort (tempPort);
}

static Nlm_GraphiC Nlm_WindowGainFocus (Nlm_GraphiC w, Nlm_Char ch, Nlm_Boolean savePort)

{
  Nlm_GraphiC  p;
  Nlm_GraphiC  q;

  q = NULL;
  p = Nlm_GetChild (w);
  while (p != NULL && q == NULL) {
    q = Nlm_DoGainFocus (p, ch, savePort);
    p = Nlm_GetNext (p);
  }
  return q;
}

static void Nlm_WindowLoseFocus (Nlm_GraphiC w, Nlm_GraphiC excpt, Nlm_Boolean savePort)

{
  Nlm_GraphiC  p;

  p = Nlm_GetChild (w);
  while (p != NULL) {
    Nlm_DoLoseFocus (p, excpt, savePort);
    p = Nlm_GetNext (p);
  }
}

extern Nlm_WindoW Nlm_DocumentWindow (Nlm_Int2 left, Nlm_Int2 top,
                                      Nlm_Int2 width, Nlm_Int2 height,
                                      Nlm_CharPtr title,
                                      Nlm_WndActnProc close,
                                      Nlm_WndActnProc resize)

{
  Nlm_RecT    r;
  Nlm_WindoW  w;

  Nlm_LoadRect (&r, left, top, width, height);
  w = Nlm_MakeWindowLink (&r, sizeof (Nlm_WindowRec), documentProcs);
  if (w != NULL) {
#ifdef WIN_MAC
    Nlm_NewWindow (w, DOCUMENT_STYLE, 8, TRUE, NULL, 0, title, close, resize);
#endif
#ifdef WIN_MSWIN
    Nlm_NewWindow (w, DOCUMENT_STYLE, 0, FALSE, windowclass, WS_OVERLAPPEDWINDOW, title, close, resize);
#endif
#ifdef WIN_MOTIF
    Nlm_NewWindow (w, DOCUMENT_STYLE, 0, FALSE, NULL, 0, title, close, resize);
#endif
  }
  return w;
}

extern Nlm_WindoW Nlm_FixedWindow (Nlm_Int2 left, Nlm_Int2 top,
                                   Nlm_Int2 width, Nlm_Int2 height,
                                   Nlm_CharPtr title, Nlm_WndActnProc close)

{
  Nlm_RecT    r;
  Nlm_WindoW  w;

  Nlm_LoadRect (&r, left, top, width, height);
  w = Nlm_MakeWindowLink (&r, sizeof (Nlm_WindowRec), fixedProcs);
  if (w != NULL) {
#ifdef WIN_MAC
    Nlm_NewWindow (w, FIXED_STYLE, 4, TRUE, NULL, 0, title, close, NULL);
#endif
#ifdef WIN_MSWIN
    Nlm_NewWindow (w, FIXED_STYLE, 0, FALSE, windowclass, WS_OVERLAPPED |
                   WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, title, close, NULL);
#endif
#ifdef WIN_MOTIF
    Nlm_NewWindow (w, FIXED_STYLE, 0, FALSE, NULL, 0, title, close, NULL);
#endif
  }
  return w;
}

extern Nlm_WindoW Nlm_FrozenWindow (Nlm_Int2 left, Nlm_Int2 top,
                                    Nlm_Int2 width, Nlm_Int2 height,
                                    Nlm_CharPtr title, Nlm_WndActnProc close)

{
  Nlm_RecT    r;
  Nlm_WindoW  w;

  Nlm_LoadRect (&r, left, top, width, height);
  w = Nlm_MakeWindowLink (&r, sizeof (Nlm_WindowRec), frozenProcs);
  if (w != NULL) {
#ifdef WIN_MAC
    Nlm_NewWindow (w, FROZEN_STYLE, 4, TRUE, NULL, 0, title, close, NULL);
#endif
#ifdef WIN_MSWIN
    Nlm_NewWindow (w, FROZEN_STYLE, 0, FALSE, windowclass, WS_OVERLAPPED |
                   WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, title, close, NULL);
#endif
#ifdef WIN_MOTIF
    Nlm_NewWindow (w, FROZEN_STYLE, 0, FALSE, NULL, 0, title, close, NULL);
#endif
  }
  return w;
}

extern Nlm_WindoW Nlm_RoundWindow (Nlm_Int2 left, Nlm_Int2 top,
                                   Nlm_Int2 width, Nlm_Int2 height,
                                   Nlm_CharPtr title, Nlm_WndActnProc close)

{
  Nlm_RecT    r;
  Nlm_WindoW  w;

  Nlm_LoadRect (&r, left, top, width, height);
  w = Nlm_MakeWindowLink (&r, sizeof (Nlm_WindowRec), roundProcs);
  if (w != NULL) {
#ifdef WIN_MAC
    Nlm_NewWindow (w, ROUND_STYLE, 16, TRUE, NULL, 0, title, close, NULL);
#endif
#ifdef WIN_MSWIN
    Nlm_NewWindow (w, ROUND_STYLE, 0, FALSE, windowclass, WS_OVERLAPPED |
                   WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, title, close, NULL);
#endif
#ifdef WIN_MOTIF
    Nlm_NewWindow (w, ROUND_STYLE, 0, FALSE, NULL, 0, title, close, NULL);
#endif
  }
  return w;
}

extern Nlm_WindoW Nlm_AlertWindow (Nlm_Int2 left, Nlm_Int2 top,
                                   Nlm_Int2 width, Nlm_Int2 height,
                                   Nlm_WndActnProc close)

{
  Nlm_RecT    r;
  Nlm_WindoW  w;

  Nlm_LoadRect (&r, left, top, width, height);
  w = Nlm_MakeWindowLink (&r, sizeof (Nlm_WindowRec), alertProcs);
  if (w != NULL) {
#ifdef WIN_MAC
    Nlm_NewWindow (w, ALERT_STYLE, 1, FALSE, NULL, 0, NULL, close, NULL);
#endif
#ifdef WIN_MSWIN
    Nlm_NewWindow (w, ALERT_STYLE, 0, FALSE, windowclass, WS_OVERLAPPED, "", close, NULL);
#endif
#ifdef WIN_MOTIF
    Nlm_NewWindow (w, ALERT_STYLE, 0, FALSE, NULL, 0, "", close, NULL);
#endif
  }
  return w;
}

/* esl: set the "owner" window of modal dialog */
extern void Nlm_SetModalWindowOwner (Nlm_WindoW w, Nlm_WindoW owner)
{
  if (w != NULL) {
    Nlm_WindowData wdata;
    Nlm_GetWindowData (w, &wdata);
    wdata.modalOwner = owner;
    Nlm_SetWindowData (w, &wdata);
  }
}

extern Nlm_WindoW Nlm_ModalWindow (Nlm_Int2 left, Nlm_Int2 top,
                                   Nlm_Int2 width, Nlm_Int2 height,
                                   Nlm_WndActnProc close)

{
  Nlm_RecT    r;
  Nlm_WindoW  w;
  Nlm_WindoW  owner = Nlm_ActiveWindow (); /* esl: reasonable guess */

  Nlm_LoadRect (&r, left, top, width, height);
  w = Nlm_MakeWindowLink (&r, sizeof (Nlm_WindowRec), modalProcs);
  if (w != NULL) {
#ifdef WIN_MAC
    Nlm_NewWindow (w, MODAL_STYLE, 1, FALSE, NULL, 0, NULL, close, NULL);
#endif
#ifdef WIN_MSWIN
    Nlm_NewWindow (w, MODAL_STYLE, 0, FALSE, windowclass, WS_POPUP |
                   WS_CAPTION | WS_SYSMENU, "", close, NULL);
#endif
#ifdef WIN_MOTIF
    Nlm_NewWindow (w, MODAL_STYLE, 0, FALSE, NULL, 0, "", close, NULL);
#endif
  }
  Nlm_SetModalWindowOwner (w, owner); /* esl */
  return w;
}

/* esl: movable dialog with caption */
extern Nlm_WindoW Nlm_MovableModalWindow (Nlm_Int2 left, Nlm_Int2 top,
                                          Nlm_Int2 width, Nlm_Int2 height,
                                          Nlm_CharPtr title,
                                          Nlm_WndActnProc close)
{
  Nlm_RecT    r;
  Nlm_WindoW  w;
  Nlm_WindoW  owner = Nlm_ActiveWindow (); /* esl: reasonable guess */

  Nlm_LoadRect (&r, left, top, width, height);
  w = Nlm_MakeWindowLink (&r, sizeof (Nlm_WindowRec), movableModalProcs);
  if (w != NULL) {
#ifdef WIN_MAC
    Nlm_NewWindow (w, MODAL_STYLE, 5, FALSE, NULL, 0, title, close, NULL);
#endif
#ifdef WIN_MSWIN
    Nlm_NewWindow (w, MODAL_STYLE, 0, FALSE, windowclass, WS_POPUP |
                   WS_CAPTION | WS_SYSMENU, title, close, NULL);
#endif
#ifdef WIN_MOTIF
    Nlm_NewWindow (w, MODAL_STYLE, 0, FALSE, NULL, 0, title, close, NULL);
#endif
  }
  Nlm_SetModalWindowOwner (w, owner); /* esl */
  return w;
}

extern Nlm_WindoW Nlm_FloatingWindow (Nlm_Int2 left, Nlm_Int2 top,
                                      Nlm_Int2 width, Nlm_Int2 height,
                                      Nlm_WndActnProc close)

{
  Nlm_RecT    r;
  Nlm_WindoW  w;

  Nlm_LoadRect (&r, left, top, width, height);
  w = Nlm_MakeWindowLink (&r, sizeof (Nlm_WindowRec), floatingProcs);
  if (w != NULL) {
#ifdef WIN_MAC
    Nlm_NewWindow (w, FLOATING_STYLE, 17, TRUE, NULL, 0, NULL, close, NULL);
#endif
#ifdef WIN_MSWIN
    Nlm_NewWindow (w, FLOATING_STYLE, 0, FALSE, windowclass, WS_POPUP |
                   WS_CAPTION | WS_SYSMENU, "", close, NULL);
#endif
#ifdef WIN_MOTIF
    Nlm_NewWindow (w, FLOATING_STYLE, 0, FALSE, NULL, 0, "", close, NULL);
#endif
  }
  return w;
}

extern Nlm_WindoW Nlm_ShadowWindow (Nlm_Int2 left, Nlm_Int2 top,
                                    Nlm_Int2 width, Nlm_Int2 height,
                                    Nlm_WndActnProc close)

{
  Nlm_RecT    r;
  Nlm_WindoW  w;

  Nlm_LoadRect (&r, left, top, width, height);
  w = Nlm_MakeWindowLink (&r, sizeof (Nlm_WindowRec), shadowProcs);
  if (w != NULL) {
#ifdef WIN_MAC
    Nlm_NewWindow (w, SHADOW_STYLE, 3, FALSE, NULL, 0, NULL, close, NULL);
#endif
#ifdef WIN_MSWIN
    Nlm_NewWindow (w, SHADOW_STYLE, 0, FALSE, windowclass,
      WS_CAPTION|WS_BORDER, "", close, NULL);
#endif
#ifdef WIN_MOTIF
    Nlm_NewWindow (w, SHADOW_STYLE, 0, FALSE, NULL, 0, "", close, NULL);
#endif
  }
  return w;
}

extern Nlm_WindoW Nlm_PlainWindow (Nlm_Int2 left, Nlm_Int2 top,
                                   Nlm_Int2 width, Nlm_Int2 height,
                                   Nlm_WndActnProc close)

{
  Nlm_RecT    r;
  Nlm_WindoW  w;

  Nlm_LoadRect (&r, left, top, width, height);
  w = Nlm_MakeWindowLink (&r, sizeof (Nlm_WindowRec), plainProcs);
  if (w != NULL) {
#ifdef WIN_MAC
    Nlm_NewWindow (w, PLAIN_STYLE, 2, FALSE, NULL, 0, NULL, close, NULL);
#endif
#ifdef WIN_MSWIN
    Nlm_NewWindow (w, PLAIN_STYLE, 0, FALSE, windowclass, WS_OVERLAPPED, "", close, NULL);
#endif
#ifdef WIN_MOTIF
    Nlm_NewWindow (w, PLAIN_STYLE, 0, FALSE, NULL, 0, "", close, NULL);
#endif
  }
  return w;
}

extern void Nlm_Metronome (Nlm_VoidProc actn)

{
  timerAction = actn;
}

extern void Nlm_SetWindowTimer (Nlm_WindoW w, Nlm_WndActnProc actn)

{
  Nlm_WindowData  wdata;

  if (w != NULL) {
    Nlm_GetWindowData (w, &wdata);
    wdata.timer = actn;
    Nlm_SetWindowData (w, &wdata);
  }
}

static void Nlm_CallWindowTimers (void)

{
  Nlm_WindoW      w;
  Nlm_WindowData  wdata;

  w = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) Nlm_desktopWindow);
  while (w != NULL && w != Nlm_systemWindow) {
    Nlm_GetWindowData (w, &wdata);
    if (wdata.timer != NULL) {
      wdata.timer (w);
    }
    w = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) w);
  }
}


#ifdef WIN_MOTIF
static void Nlm_SetCursor (Cursor cursor)

{
  Nlm_MainTool  man;
  Nlm_WindoW    w;
  Window        xtw;

  if (Nlm_currentXDisplay != NULL) {
    w = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) Nlm_desktopWindow);
    while (w != NULL) {
      if (! Nlm_IsWindowDying (w)) {
        man = Nlm_GetWindowMain (w);
        xtw = XtWindow (man);
        if ( xtw ) {
          XDefineCursor (Nlm_currentXDisplay, xtw, cursor);
        }
      }
      w = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) w);
    }
    Nlm_currentCursor = cursor;
    XFlush( Nlm_currentXDisplay );
  }
}
#endif

extern void Nlm_ArrowCursor (void)

{
#ifdef WIN_MAC
  Cursor cursor;
  GetQDGlobalsArrow(&cursor);
  SetCursor (&cursor);
#endif
#ifdef WIN_MSWIN
  Nlm_currentCursor = LoadCursor (NULL, IDC_ARROW);
  SetCursor (Nlm_currentCursor);
#endif
#ifdef WIN_MOTIF
  Nlm_SetCursor (arrow);
#endif
}

extern void Nlm_CrossCursor (void)

{
#ifdef WIN_MAC
  SetCursor (&cross);
#endif
#ifdef WIN_MSWIN
  Nlm_currentCursor = LoadCursor (NULL, IDC_CROSS);
  SetCursor (Nlm_currentCursor);
#endif
#ifdef WIN_MOTIF
  Nlm_SetCursor (cross);
#endif
}

extern void Nlm_IBeamCursor (void)

{
#ifdef WIN_MAC
  SetCursor (&iBeam);
#endif
#ifdef WIN_MSWIN
  Nlm_currentCursor = LoadCursor (NULL, IDC_IBEAM);
  SetCursor (Nlm_currentCursor);
#endif
#ifdef WIN_MOTIF
  Nlm_SetCursor (iBeam);
#endif
}

extern void Nlm_PlusCursor (void)

{
#ifdef WIN_MAC
  SetCursor (&plus);
#endif
#ifdef WIN_MSWIN
  Nlm_currentCursor = LoadCursor (NULL, IDC_CROSS);
  SetCursor (Nlm_currentCursor);
#endif
#ifdef WIN_MOTIF
  Nlm_SetCursor (plus);
#endif
}

extern void Nlm_WatchCursor (void)

{
#ifdef WIN_MAC
  SetCursor (&watch);
#endif
#ifdef WIN_MSWIN
  Nlm_currentCursor = LoadCursor (NULL, IDC_WAIT);
  SetCursor (Nlm_currentCursor);
#endif
#ifdef WIN_MOTIF
  Nlm_SetCursor (watch);
#endif
}


typedef void (*Nlm_SetCursorFuncPtr)( void );

static enumCursorShape curr_cursor_shape = CURSOR_DEFAULT;
static Nlm_SetCursorFuncPtr Nlm_CursorArray[ N_CURSORS ];


static Nlm_Boolean Nlm_AddCursorShape(enumCursorShape      cursor_shape,
                                      Nlm_SetCursorFuncPtr set_function)
{
  if (cursor_shape < 0  ||  N_CURSORS <= cursor_shape  ||
      Nlm_CursorArray[cursor_shape] != NULL  ||
      set_function == NULL)
    return FALSE;

  Nlm_CursorArray[cursor_shape] = set_function;
  return TRUE;
}


Nlm_Boolean Nlm_InitCursorShapes( void )
{
  int i;
  for (i = 0;  i < N_CURSORS;  i++)
    Nlm_CursorArray[i] = NULL;

  {{
    Nlm_Boolean ok = (Nlm_Boolean)(
      Nlm_AddCursorShape (CURSOR_ARROW, Nlm_ArrowCursor)  &&
      Nlm_AddCursorShape (CURSOR_CROSS, Nlm_CrossCursor)  &&
      Nlm_AddCursorShape (CURSOR_IBEAM, Nlm_IBeamCursor)  &&
      Nlm_AddCursorShape (CURSOR_PLUS,  Nlm_PlusCursor )  &&
      Nlm_AddCursorShape (CURSOR_WATCH, Nlm_WatchCursor));
    ASSERT ( ok );
    if ( !ok )  return FALSE;
  }}

  for (i = 0;  i < N_CURSORS;  i++)
    if (Nlm_CursorArray[i] == NULL)
      {
        ASSERT ( FALSE );
        return FALSE;
      }

  return  (Nlm_Boolean)(SetCursorShape( curr_cursor_shape )  !=  BAD_CURSOR);
}


enumCursorShape Nlm_SetCursorShape(enumCursorShape cursor_shape)
{
  if (cursor_shape == WHAT_CURSOR)
    return curr_cursor_shape;

  if (0 <= cursor_shape  &&  cursor_shape < N_CURSORS  &&
      Nlm_CursorArray[cursor_shape] != NULL)
    {
      enumCursorShape prev_cursor_shape = curr_cursor_shape;
      (*Nlm_CursorArray[curr_cursor_shape = cursor_shape])();
      return prev_cursor_shape;
    }
  else
    {
      ASSERT ( FALSE );
      return BAD_CURSOR;
    }
}


extern void LIBCALL
Nlm_RegisterDropProc (Nlm_DropProc dropProc)
{
  registeredDropProc = dropProc;
}

extern void LIBCALL
Nlm_RegisterServiceProc (Nlm_ServiceProc serviceProc)
{
  registeredServiceProc = serviceProc;
}

extern void LIBCALL
Nlm_RegisterResultProc (Nlm_ResultProc resultProc)
{
  registeredResultProc = resultProc;
}

#ifdef WIN_MAC
static pascal OSErr HandleAEQuitApp (const AppleEvent *event, AppleEvent *reply, AERefCon ref)

{
  Nlm_QuitProgram ();
  return noErr;
}

static pascal OSErr HandleAEIgnore (const AppleEvent *event, AppleEvent *reply, AERefCon ref)

{
  return noErr;
}

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

static pascal OSErr HandleAEOpenDoc (const AppleEvent *event, AppleEvent *reply, AERefCon ref)

{
  register OSErr stat;
  register long i;
  AEDescList list;
  AEKeyword keywd;
  DescType dtype;
  FSSpec fss;
  long count;
  Size size;
  char filename [256];
  char tempfile [PATH_MAX];
  AEDesc docDesc;
  AEDescList theList;
  FSSpec rsultfss;
  OSErr theErr;
  
  stat = AEGetParamDesc (event, keyDirectObject, typeAEList, &list);
  if ( stat ) return ( stat );
  
  stat = AEGetAttributePtr (event, keyMissedKeywordAttr, typeWildCard,
                            &dtype, 0, 0, &size );
  if ( stat != errAEDescNotFound ) {   
    AEDisposeDesc( &list );
    return ( stat? stat : errAEEventNotHandled );
  }
  
  AECountItems ( &list, &count );
  for ( i = 1; i <= count; i++ ) {   
    stat = AEGetNthPtr (&list, i, typeFSS, &keywd,
                        &dtype, (Ptr) &fss, sizeof (fss),
                        &size);
    if ( !stat ) {   
      ConvertFilename (&fss, filename);

      if (registeredDropProc != NULL) {
        registeredDropProc (filename);
      }

      if (registeredServiceProc != NULL) {
        Nlm_TmpNam (tempfile);
        registeredServiceProc (filename, tempfile);

        theErr = AECreateList(NULL, 0, FALSE, &theList);
        if (theErr == noErr) {
          Nlm_CtoPstr ((Nlm_CharPtr) tempfile);
          theErr = FSMakeFSSpec (0, 0, (ConstStr255Param) tempfile, &rsultfss);
          if (theErr == noErr) {
            theErr = AECreateDesc(typeFSS, (Ptr)&rsultfss, sizeof(fss), &docDesc);
            if (theErr == noErr) {
              theErr = AEPutDesc(&theList, 0, &docDesc);
              if (theErr == noErr) {
                theErr = AEPutParamDesc(reply, keyDirectObject, &theList);
              }
            }
          }
        }

      }
    }
  }
  AEDisposeDesc ( &list );         
  return noErr;
}

static pascal OSErr HandleAEAnswer (const AppleEvent *event, AppleEvent *reply, AERefCon ref)

{
  register OSErr stat;
  register long i;
  AEDescList list;
  AEKeyword keywd;
  DescType dtype;
  FSSpec fss;
  long count;
  Size size;
  char filename [256];
  
  stat = AEGetParamDesc (event, keyDirectObject, typeAEList, &list);
  if ( stat ) return ( stat );
  
  stat = AEGetAttributePtr (event, keyMissedKeywordAttr, typeWildCard,
                            &dtype, 0, 0, &size );
  if ( stat != errAEDescNotFound ) {   
    AEDisposeDesc( &list );
    return ( stat? stat : errAEEventNotHandled );
  }
  
  AECountItems ( &list, &count );
  for ( i = 1; i <= count; i++ ) {   
    stat = AEGetNthPtr (&list, i, typeFSS, &keywd,
                        &dtype, (Ptr) &fss, sizeof (fss),
                        &size);
    if ( !stat ) {   
      ConvertFilename (&fss, filename);
      if (registeredResultProc != NULL) {
        registeredResultProc (filename);
        Nlm_FileRemove (filename); /* ? */
      } else if (registeredDropProc != NULL) {
        registeredDropProc (filename);
      }
    }
  }
  AEDisposeDesc ( &list );         
  return noErr;
}


static void Nlm_HandleEvent (void)

{
  OSErr           err;
  Nlm_Int2        key;
  Nlm_Uint4       mess;
  Nlm_PointTool   ptool;
  Nlm_Int2        windowLoc;
  Nlm_WindowTool  wptr;

  Nlm_PointToolToPoinT (Nlm_currentEvent.where, &Nlm_globalMouse);
  Nlm_localMouse = Nlm_globalMouse;
  Nlm_GlobalToLocal (&Nlm_localMouse);
  Nlm_currentKey = '\0';
  key = Nlm_currentEvent.modifiers;
  Nlm_cmmdKey = ((key & 256) != 0);
  Nlm_ctrlKey = ((key & 4096) != 0);
  Nlm_shftKey = ((key & 512) != 0);
  Nlm_optKey = ((key & 2048) != 0);
  Nlm_dblClick = FALSE;
  frontWindow = Nlm_FindWindowRec (FrontWindow ());
  Nlm_theWindow = Nlm_FindWindowRec ((Nlm_WindowTool) Nlm_currentEvent.message);
  chosenWindow = Nlm_theWindow;
  switch (Nlm_currentEvent.what) {
    case mouseDown:
      Nlm_PoinTToPointTool (Nlm_globalMouse, &ptool);
      windowLoc = FindWindow (ptool, &wptr);
      chosenWindow = Nlm_FindWindowRec (wptr);
      Nlm_DoClick ((Nlm_GraphiC) frontWindow, Nlm_globalMouse);
      break;
    case keyDown:
      Nlm_currentKey = (Nlm_Char) (Nlm_currentEvent.message % 256);
      if (keyAction != NULL) {
        keyAction (Nlm_currentKey);
      }
      Nlm_DoKey ((Nlm_GraphiC) frontWindow, Nlm_currentKey);
      break;
    case autoKey:
      if (! Nlm_cmmdKey) {
        Nlm_currentKey = (Nlm_Char) (Nlm_currentEvent.message % 256);
        Nlm_DoKey ((Nlm_GraphiC) frontWindow, Nlm_currentKey);
      }
      break;
    case updateEvt:
      Nlm_DoDraw ((Nlm_GraphiC) Nlm_theWindow);
      break;
    case osEvt:
      mess = (Nlm_currentEvent.message & osEvtMessageMask) >> 24;
      if (mess == suspendResumeMessage) {
        if (Nlm_currentEvent.message & resumeFlag) {
          // Resume
#if !TARGET_API_MAC_CARBON
          if (Nlm_currentEvent.message & convertClipboardFlag) {
            // 2001-05-14:  JDJ
            // We always convert the clipboard.
            // If it turns out that pasting is noticeably slow, I'll change it.
          }
#endif
        } else {
          // Suspend
        }
      } else if (mess == mouseMovedMessage) {
        if (mouseMovedAction != NULL) {
          Nlm_PoinTToPointTool (Nlm_globalMouse, &ptool);
          windowLoc = FindWindow (ptool, &wptr);
          chosenWindow = Nlm_FindWindowRec (wptr);
          mouseMovedAction ((Nlm_GraphiC) frontWindow, Nlm_globalMouse);
        }
      }
      break;
    case activateEvt:
      if (Nlm_currentEvent.modifiers & 01) {
        Nlm_DoActivate ((Nlm_GraphiC) Nlm_theWindow, FALSE);
      } else {
        Nlm_DoDeactivate ((Nlm_GraphiC) Nlm_theWindow, FALSE);
      }
      break;
// 2001-03-22:  Joshua Juran
// Carbon doesn't support DIBadMount() and will not send diskEvt in the first place.
#if !TARGET_API_MAC_CARBON
    case diskEvt:
      if (HiWord (Nlm_currentEvent.message) != 0) {
        Nlm_PoinT where;
        where.x = 90;
        where.y = 100;
        Nlm_PoinTToPointTool (where, &ptool);
        DIBadMount (ptool, Nlm_currentEvent.message);
      }
      break;
#endif
    case kHighLevelEvent:
      err = AEProcessAppleEvent (&Nlm_currentEvent);
      break;
    default:
      break;
  }
}

extern Nlm_Boolean Nlm_RegisterWindows (void)

{
  return TRUE;
}

static void Nlm_ReturnCursor (Cursor *cursor, Nlm_Int2 cursorID)

{
  CursHandle  hCurs;
  CursPtr     pCurs;

  hCurs = GetCursor (cursorID);
  if (hCurs != NULL) {
    HLock ((Handle) hCurs);
    pCurs = (CursPtr) *((Handle) hCurs);
    *cursor = *pCurs;
    HUnlock ((Handle) hCurs);
  } else {
    GetQDGlobalsArrow(cursor);
  }
}

static Nlm_Boolean Nlm_SetupWindows (void)

{
  Nlm_PoinT  pt;
  Nlm_RecT   r;
  long  gval;
  BitMap myScreenBits;

  Nlm_ReturnCursor (&cross, 2);
  Nlm_ReturnCursor (&iBeam, 1);
  Nlm_ReturnCursor (&plus, 3);
  Nlm_ReturnCursor (&watch, 4);
  Nlm_WatchCursor ();
  Nlm_ClearKeys ();
  Nlm_LoadPt (&pt, 0, 0);
  GetQDGlobalsScreenBits(&myScreenBits);
  Nlm_RectToolToRecT (&(myScreenBits.bounds), &screenBitBounds);
  r = screenBitBounds;
  Nlm_screenRect = screenBitBounds;
  Nlm_desktopWindow = (Nlm_WindoW) Nlm_HandNew (sizeof (Nlm_WindowRec));
  Nlm_systemWindow = (Nlm_WindoW) Nlm_HandNew (sizeof (Nlm_WindowRec));
  Nlm_LoadGraphicData ((Nlm_GraphiC) Nlm_desktopWindow, (Nlm_GraphiC) Nlm_systemWindow,
                       NULL, NULL, NULL, desktopProcs, NULL, &r, TRUE, TRUE, NULL, NULL);
  Nlm_LoadBoxData ((Nlm_BoX) Nlm_desktopWindow, pt, pt, pt, 0, 0, 0, 0, 0, 0, 0, 0);
  Nlm_LoadWindowData (Nlm_desktopWindow, NULL, NULL, NULL, NULL, &r, &r, &r, NULL, NULL,
                      NULL, NULL, NULL, NULL, NULL, TRUE, FALSE, NULL, NULL, NULL, NULL);
  Nlm_LoadRect (&r, 0, 0, 0, 0);
  Nlm_LoadGraphicData ((Nlm_GraphiC) Nlm_systemWindow, NULL, NULL,
                       NULL, NULL, systemProcs, NULL, &r, TRUE, TRUE, NULL, NULL);
  Nlm_LoadBoxData ((Nlm_BoX) Nlm_systemWindow, pt, pt, pt, 0, 0, 0, 0, 0, 0, 0, 0);
  Nlm_LoadWindowData (Nlm_systemWindow, NULL, NULL, NULL, NULL, &r, &r, &r, NULL, NULL,
                      NULL, NULL, NULL, NULL, NULL, TRUE, FALSE, NULL, NULL, NULL, NULL);
  dyingWindow = NULL;
  frontWindow = Nlm_desktopWindow;
  theActiveWindow = Nlm_desktopWindow;
  chosenWindow = Nlm_desktopWindow;
  Nlm_theWindow = Nlm_desktopWindow;
  Nlm_currentWindowTool = (Nlm_WindowTool) 0;
  quitProgram = FALSE;

  Nlm_SetUpDrawingTools ();

  Nlm_hScrollBarHeight = 16;
  Nlm_vScrollBarWidth = 16;

  Nlm_popupMenuHeight = Nlm_stdLineHeight + 4;
  Nlm_dialogTextHeight = Nlm_stdLineHeight + 4;
  
  lastTimerTime = Nlm_ComputerTime ();
  timerAction = NULL;
  keyAction = NULL;
  mouseMovedAction = NULL;
  registeredDropProc = NULL;
  registeredServiceProc = NULL;
  registeredResultProc = NULL;

    /* gestalt for quickdraw features are defined as bits in a bitfield
       for example gestaltHasColor = 0, thus we need to test for lsb set
     */
  if( Gestalt( gestaltQuickdrawFeatures, &gval) == noErr){
      hasColorQD = (gval && (1 << gestaltHasColor));
  }
  return TRUE;
}
#endif

#ifdef WIN_MSWIN

/* Message cracker functions */

static void MyCls_OnMouseMove (HWND hwnd, int x, int y, UINT keyFlags)

{
  SetCursor (Nlm_currentCursor);
}

static void MyCls_OnShowWindow(HWND hwnd, BOOL fShow, UINT status)
{
  Nlm_SetVisible((Nlm_GraphiC)Nlm_theWindow, (Nlm_Boolean)
                 (fShow ? (status != SW_PARENTCLOSING) : FALSE));
}

static void MyCls_OnCommand (HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)

{
  Nlm_GraphiC  g;
  Nlm_MenuBaR  mb;

  Nlm_currentId = id;
  Nlm_currentHwndCtl = hwndCtl;
  Nlm_currentCode = codeNotify;

  if (hwndCtl == NULL) {
    mb = Nlm_GetWindowMenuBar (Nlm_theWindow);
    Nlm_DoCommand ((Nlm_GraphiC) mb);
  } else {
    g = (Nlm_GraphiC) GetProp (hwndCtl, (LPSTR) "Nlm_VibrantProp");
    (void) (Nlm_GetEnabled (g) &&
            Nlm_GetVisible (g) &&
            Nlm_DoCommand (g));
  }
}

static void MyCls_OnHVScroll (HWND hwnd, HWND hwndCtl, UINT code, int pos)

{
  Nlm_GraphiC  g;

  Nlm_currentHwndCtl = hwndCtl;
  Nlm_currentCode = code;
  Nlm_currentPos = pos;

  if (hwndCtl != NULL) {
    g = (Nlm_GraphiC) GetProp (hwndCtl, (LPSTR) "Nlm_VibrantProp");
    (void) (Nlm_GetEnabled (g) &&
            Nlm_GetVisible (g) &&
            Nlm_DoCommand (g));
  }
}

static void MyCls_OnClose (HWND hwnd)

{
  Nlm_WndActnProc  cls;
  Nlm_WindowData   wdata;

  Nlm_GetWindowData (Nlm_theWindow, &wdata);
  cls = wdata.close;
  if (cls != NULL) {
    cls (Nlm_theWindow);
  }
}

static void MyCls_OnActivate(HWND hwnd, UINT state, HWND hwndActDeact, BOOL fMinimized)
{
  switch ( state )
    {
    case WA_ACTIVE:
    case WA_CLICKACTIVE:
      {
        Nlm_WindowData  wdata;
        Nlm_GetWindowData(Nlm_theWindow, &wdata);
        if ( wdata.cMap ) {
          SelectPalette(Nlm_currentHDC, wdata.cMap, FALSE);
          RealizePalette( Nlm_currentHDC );
        }
        Nlm_SetVisible((Nlm_GraphiC)Nlm_theWindow,
                       (Nlm_Boolean)(!fMinimized));
        Nlm_DoActivate((Nlm_GraphiC)Nlm_theWindow, FALSE);
        break;
      }
    case WA_INACTIVE:
      {
        Nlm_DoDeactivate((Nlm_GraphiC)Nlm_theWindow, FALSE);
        break;     
      }
    }
}


static void OnSizeMove(HWND hwnd, Nlm_Boolean resized)
{
  Nlm_RectTool rtool;

  if (!Nlm_WindowHasBeenShown( Nlm_theWindow )  ||
      !Nlm_GetVisible( (Nlm_GraphiC)Nlm_theWindow ))
    return;

  {{
  Nlm_RecT r;
  GetWindowRect(hwnd, &rtool);
  win2client(Nlm_theWindow, &rtool, &r);
  Nlm_SetRect((Nlm_GraphiC)Nlm_theWindow, &r);
  }}

  if ( resized )
    {
      Nlm_WindowData wdata;
      Nlm_GetWindowData(Nlm_theWindow, &wdata);
      if (wdata.resize == NULL)
        return;
      (*wdata.resize)( Nlm_theWindow );
      InvalidateRect(hwnd, &rtool, FALSE);
    }
}

static void MyCls_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
  if ( !Nlm_GetVisible( (Nlm_GraphiC)Nlm_theWindow ) )
    return;

  Nlm_SetVisible((Nlm_GraphiC)Nlm_theWindow,
                 (Nlm_Boolean)(state != SIZE_MINIMIZED));
  OnSizeMove(hwnd, TRUE);
}

static void MyCls_OnMove(HWND hwnd, int x, int y)
{
  OnSizeMove(hwnd, FALSE);
}


static void MyCls_OnChar (HWND hwnd, UINT ch, int cRepeat)

{
  handlechar = FALSE;
  if (ch == '\t') {
    Nlm_DoSendFocus ((Nlm_GraphiC) Nlm_theWindow, (Nlm_Char) ch);
  } else if (ch == '\n' || ch == '\r') {
    Nlm_DoSendFocus ((Nlm_GraphiC) Nlm_theWindow, (Nlm_Char) ch);
  } else {
    handlechar = TRUE;
  }
}

/*
*  Note that the WM_SIZE message will trigger the resize callback whenever
*  a visible window is resized, including the initial sizing when a window
*  is first displayed.
*/

LRESULT CALLBACK EXPORT MainProc (HWND hwnd, UINT message,
                                  WPARAM wParam, LPARAM lParam)

{
  MINMAXINFO FAR*  lpmmi;
  HDC              tempHDC;
  HWND             tempHWnd;
  LRESULT          mainwndrsult = 0;

  if (Nlm_VibrantDisabled ()) {
    return DefWindowProc (hwnd, message, wParam, lParam);
  }

  tempHWnd = Nlm_currentHWnd;
  tempHDC = Nlm_currentHDC;
  Nlm_theWindow = (Nlm_WindoW) GetProp (hwnd, (LPSTR) "Nlm_VibrantProp");
  Nlm_currentHWnd = hwnd;
  Nlm_currentHDC = Nlm_GetWindowPort (Nlm_theWindow);
  Nlm_currentWindowTool = hwnd;
  Nlm_currentKey = '\0';
  Nlm_currentWParam = wParam;
  Nlm_currentLParam = lParam;
  Nlm_cmmdKey = FALSE;
  Nlm_optKey = FALSE;
  Nlm_ctrlKey = (Nlm_Boolean) ((GetKeyState (VK_CONTROL) & 0x8000) != 0);
  Nlm_shftKey = (Nlm_Boolean) ((GetKeyState (VK_SHIFT)   & 0x8000) != 0);
  Nlm_dblClick = FALSE;

  switch ( message )
    {
    case WM_MOUSEMOVE:
      HANDLE_WM_MOUSEMOVE (hwnd, wParam, lParam, MyCls_OnMouseMove);
      break;
    case WM_SHOWWINDOW:
      HANDLE_WM_SHOWWINDOW (hwnd, wParam, lParam, MyCls_OnShowWindow);
      break;
    case WM_COMMAND:
      HANDLE_WM_COMMAND (hwnd, wParam, lParam, MyCls_OnCommand);
      break;
    case WM_HSCROLL:
      HANDLE_WM_HSCROLL (hwnd, wParam, lParam, MyCls_OnHVScroll);
      break;
    case WM_VSCROLL:
      HANDLE_WM_VSCROLL (hwnd, wParam, lParam, MyCls_OnHVScroll);
      break;
    case WM_CLOSE:
      HANDLE_WM_CLOSE (hwnd, wParam, lParam, MyCls_OnClose);
      break;
    case WM_ACTIVATE:
      mainwndrsult = DefWindowProc (hwnd, message, wParam, lParam);
      HANDLE_WM_ACTIVATE (hwnd, wParam, lParam, MyCls_OnActivate);
      break;
    case WM_SIZE:
      HANDLE_WM_SIZE (hwnd, wParam, lParam, MyCls_OnSize);
      break;
    case WM_MOVE:
      HANDLE_WM_MOVE (hwnd, wParam, lParam, MyCls_OnMove);
      break;
    case WM_GETMINMAXINFO:
      lpmmi = (MINMAXINFO FAR*) lParam;
      lpmmi->ptMaxTrackSize.x = 30000;
      lpmmi->ptMaxTrackSize.y = 30000;
      break;
    case WM_DESTROY:
      break;

#ifdef WIN32
    case WM_CTLCOLORSTATIC:
      {
              HDC hdc = GetDC( hwnd );
              SetBkMode((HDC)wParam, TRANSPARENT);
              ReleaseDC(hwnd, hdc); 
              mainwndrsult = GetClassLong(hwnd, GCL_HBRBACKGROUND);
              break;
      }
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLOREDIT:
      {
              SetBkColor((HDC)wParam, RGB(255, 255, 255));
              mainwndrsult = (long)GetStockObject( WHITE_BRUSH );
              break;
      }
#else
    case WM_CTLCOLOR:
      switch ( lParam )
            {
            case CTLCOLOR_STATIC:
              {
                HDC hdc = GetDC( hwnd );
                SetBkMode((HDC)wParam, TRANSPARENT);
                ReleaseDC(hwnd, hdc); 
                mainwndrsult = GetClassLong(hwnd, GCW_HBRBACKGROUND);
                break;
              }
            case CTLCOLOR_LISTBOX:
            case CTLCOLOR_EDIT:
              {
                SetBkColor((HDC)wParam, RGB(255, 255, 255));
                mainwndrsult = (long)(int)GetStockObject( WHITE_BRUSH );
                break;
              }
            default:
              mainwndrsult = DefWindowProc(hwnd, message, wParam, lParam);
            }
      break;
#endif

    case WM_KEYDOWN:
      if ( !Nlm_ProcessKeydown((Nlm_GraphiC)Nlm_theWindow, wParam,
                                VERT_PAGE|VERT_ARROW|HORIZ_PAGE|HORIZ_ARROW) )
        mainwndrsult = DefWindowProc(hwnd, message, wParam, lParam);
      break;
    case WM_CHAR:
      HANDLE_WM_CHAR (hwnd, wParam, lParam, MyCls_OnChar);
      if (handlechar) {
        mainwndrsult = DefWindowProc (hwnd, message, wParam, lParam);
      }
      break;
    case WM_DROPFILES:
      {
        Nlm_Char fnamebuf[PATH_MAX];

        DragQueryFile((HDROP)wParam,0,fnamebuf,sizeof(fnamebuf));
        if (registeredDropProc != NULL)
        {
          registeredDropProc(fnamebuf);
        }
        DragFinish((HDROP)wParam);
      }
      break;
    default:
      mainwndrsult = DefWindowProc (hwnd, message, wParam, lParam);
      break;
  }
  Nlm_currentHWnd = tempHWnd;
  Nlm_currentHDC = tempHDC;
  Nlm_currentWindowTool = tempHWnd;
  return mainwndrsult;
}

void FAR PASCAL EXPORT MetronomeProc (HWND hwnd, UINT message,
                                      WPARAM wParam, LPARAM lParam)

{
  if (Nlm_VibrantDisabled ()) {
    return;
  }
  if (message == WM_TIMER) {
    if (timerAction != NULL) {
      timerAction ();
    }
    Nlm_CallWindowTimers ();
  }
}

extern Nlm_Boolean Nlm_RegisterWindows (void)
{
  WNDCLASS     wc;

  wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = MainProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = Nlm_currentHInst;
  wc.hIcon = LoadIcon (NULL, IDI_APPLICATION);
  wc.hCursor = NULL;
  wc.hbrBackground = CreateSolidBrush( GetSysColor(COLOR_ACTIVEBORDER) );
  wc.lpszMenuName = NULL;
  sprintf (windowclass, "Nlm_WindowClass%ld", (long) (int) Nlm_currentHInst);
  wc.lpszClassName = windowclass;

  return (Nlm_Boolean)(RegisterClass(&wc) != 0);
}


static Nlm_Boolean Nlm_SetupWindows (void)

{
  Nlm_Int2   height;
  WNDPROC    lpfnTimerProc;
  Nlm_PoinT  pt;
  Nlm_RecT   r;
  Nlm_Int2   width;

  width  = (Nlm_Int2)GetSystemMetrics (SM_CXSCREEN);
  height = (Nlm_Int2)GetSystemMetrics (SM_CYSCREEN);
  Nlm_LoadRect (&screenBitBounds, 0, 0, width, height);
  Nlm_LoadPt (&pt, 0, 0);
  r = screenBitBounds;
  Nlm_screenRect = screenBitBounds;
  Nlm_desktopWindow = (Nlm_WindoW) Nlm_HandNew (sizeof (Nlm_WindowRec));
  Nlm_LoadGraphicData ((Nlm_GraphiC) Nlm_desktopWindow, NULL, NULL,
                       NULL, NULL, desktopProcs, NULL, &r, TRUE, TRUE, NULL, NULL);
  Nlm_LoadBoxData ((Nlm_BoX) Nlm_desktopWindow, pt, pt, pt, 0, 0, 0, 0, 0, 0, 0, 0);
  Nlm_LoadWindowData (Nlm_desktopWindow, NULL, NULL, NULL, NULL, &r, &r, &r, NULL, NULL,
                      NULL, NULL, NULL, NULL, NULL, TRUE, FALSE, NULL, NULL, NULL, NULL);
  Nlm_systemWindow = NULL;
  Nlm_currentHWnd = NULL;
  Nlm_currentHDC = NULL;
  Nlm_currentWindowTool = (Nlm_WindowTool) 0;
  quitProgram = FALSE;
  
  Nlm_SetUpDrawingTools ();

  Nlm_hScrollBarHeight = (Nlm_Int2)GetSystemMetrics (SM_CYHSCROLL);
  Nlm_vScrollBarWidth  = (Nlm_Int2)GetSystemMetrics (SM_CXVSCROLL);

  Nlm_popupMenuHeight  = (Nlm_Int2)(Nlm_stdLineHeight + 8);
  Nlm_dialogTextHeight = (Nlm_Int2)(Nlm_stdFontHeight * 3 / 2);
  
  Nlm_currentCursor = LoadCursor (NULL, IDC_WAIT);
  
  lpfnTimerProc = (WNDPROC) MakeProcInstance ((FARPROC) MetronomeProc, Nlm_currentHInst);
  if (lpfnTimerProc != NULL) {
    timerID = SetTimer (NULL, 1, 50, (TIMERPROC) lpfnTimerProc);
  }
  timerAction = NULL;
  keyAction = NULL;
  mouseMovedAction = NULL;
  registeredDropProc = NULL;
  registeredServiceProc = NULL;
  registeredResultProc = NULL;
  return TRUE;
}
#endif  /* WIN_MSWIN */


#ifdef WIN_MOTIF
static void Nlm_WindowTimer (XtPointer client_data, XtIntervalId *id)

{
  if (! quitProgram) {
    windowTimer = XtAppAddTimeOut (Nlm_appContext, 50, Nlm_WindowTimer, NULL);
  }
  if (timerAction != NULL) {
    timerAction ();
  }
  Nlm_CallWindowTimers ();
}

extern Nlm_Boolean Nlm_RegisterWindows (void)

{
  windowTimer = XtAppAddTimeOut (Nlm_appContext, 50, Nlm_WindowTimer, NULL);
  return TRUE;
}

char * defFallbackRes[] =
{
  "Vibrant*background:                      light grey",
  "Vibrant*foreground:                      black",
  "Vibrant*XmPushButton.background:         grey",
  "Vibrant*XmToggleButton.foreground:       blue",
  "Vibrant*XmText.background:               white",
  "Vibrant*XmTextField.background:          white",
  "Vibrant*XmScrolledWindow.background:     white",
  "Vibrant*XmList.background:               white",
  "Vibrant*XmDrawingArea.background:        white",
  "Vibrant*XmOptionButtonGadget.background: grey",

  NULL
};


static Boolean Nlm_XrmGetResource1(const char *_basename,
                                   const char *_resource,
                                   XrmValuePtr value)
{
  XrmDatabase rdb = XtDatabase( Nlm_currentXDisplay );
  XrmString type_str;
  Boolean ok;

  size_t basename_len = Nlm_StringLen( _basename );
  size_t resource_len = Nlm_StringLen( _resource );
  size_t len = basename_len + resource_len + 2;

  char *xx_name  = XtMalloc( len );
  char *xx_class = XtMalloc( len );

  Nlm_StrCpy(xx_name, _basename);
  xx_name[basename_len] = '.';
  Nlm_StrCpy(xx_name + basename_len + 1, _resource);

  Nlm_StrCpy(xx_class, xx_name);
  xx_class[0]                = TO_UPPER( _basename[0] );
  xx_class[basename_len + 1] = TO_UPPER( _resource[0] );

  ok = XrmGetResource(rdb, xx_name, xx_class, &type_str, value);

  XtFree( xx_class );
  XtFree( xx_name  );

  return ok;
}


static Boolean Nlm_XrmGetResource2(const char *_basename,
                                   const char *_resource,
                                   XrmValuePtr value)
{
  ASSERT ( _basename  &&  *_basename );
  ASSERT ( _resource  &&  *_resource );
  ASSERT ( value );

  return
    Nlm_XrmGetResource1(_basename,   _resource, value)  ||
    Nlm_XrmGetResource1(Nlm_VibName, _resource, value);
}


extern Nlm_CharPtr Nlm_XrmGetResource(const Nlm_Char PNTR _resource)
{
  XrmValue value;
  if (_resource  && Nlm_XrmGetResource2(Nlm_AppName, _resource, &value))
    return Nlm_StringSave( (Nlm_CharPtr)value.addr );

  return NULL;
}


extern XFontStruct *Nlm_XLoadStandardFont(void); /* <-- see in "ncbidraw.c" */

static Nlm_Boolean Nlm_SetupWindows (void)
{
  Nlm_Int4   height;
  Nlm_PoinT  pt;
  Nlm_RecT   r;
  Nlm_Int4   width;
  int        xx_argc = (int)Nlm_GetArgc();
  char     **xx_argv =      Nlm_GetArgv();

  Nlm_desktopWindow = NULL;
  Nlm_systemWindow = NULL;
  Nlm_currentXDisplay = NULL;
  Nlm_currentXScreen = 0;
  Nlm_currentXWindow = 0;
  Nlm_currentXGC = NULL;
  Nlm_currentWindowTool = (Nlm_WindowTool) 0;
  quitProgram = FALSE;
  timerAction = NULL;
  keyAction = NULL;
  mouseMovedAction = NULL;
  registeredDropProc = NULL;
  registeredServiceProc = NULL;
  registeredResultProc = NULL;
  XtToolkitInitialize ();
  Nlm_appContext = XtCreateApplicationContext ();
  if (Nlm_appContext == NULL) {
    return FALSE;
  }

  if (xx_argc  &&  *xx_argv[0])
    {/* Get the application basename(cut off path and extention(s), locase) */
      const char *s     = xx_argv[0];
      const char *start = s;

      while ( *s )
        if (*s++ == '/') {
          while (*s  &&  (*s == '/'  ||  *s == '.'))
            s++;
          start = s;
        }

      for (s = start;  *s && *s != '.';  s++);
      ASSERT ( s  >=  start );

      if (s != start) {
        size_t len = s - start;
        ASSERT ( len  <  sizeof(Nlm_AppName) );
        Nlm_AppName[len] = '\0';
        while (len-- > 0)
          Nlm_AppName[len] = TO_LOWER( start[len] );
      }
    }

  {{
#define disp_opTable_SIZE 2
    static XrmOptionDescRec disp_opTable[disp_opTable_SIZE] = {
      {"-display", ".display", XrmoptionSepArg, 0},
      {"-dpy",     ".display", XrmoptionSepArg, 0}
    };
    XrmString type_str;
    XrmDatabase disp_db = NULL;
    XrmValue value;
    XrmInitialize();
    XrmParseCommand(&disp_db, disp_opTable, disp_opTable_SIZE,
                    "vibrant", &xx_argc, xx_argv);
    if ( !XrmGetResource(disp_db,  "vibrant.display", "Vibrant.Display",
                         &type_str, &value) )
      value.addr = NULL;
    Nlm_currentXDisplay = XOpenDisplay( (char *)value.addr );
    XrmDestroyDatabase( disp_db );
  }}

  if (Nlm_currentXDisplay == NULL) {
    fprintf (stderr, "Vibrant applications require X Windows\n");
    return FALSE;
  }

  XSetErrorHandler( defaultXerror_handler );

  Nlm_currentXScreen = DefaultScreen (Nlm_currentXDisplay);
  width =  DisplayWidth (Nlm_currentXDisplay, Nlm_currentXScreen);
  height = DisplayHeight (Nlm_currentXDisplay, Nlm_currentXScreen);

  Nlm_LoadRect (&screenBitBounds, 0, 0, (Nlm_Int2) width, (Nlm_Int2) height);
  Nlm_LoadPt (&pt, 0, 0);
  r = screenBitBounds;
  Nlm_screenRect = screenBitBounds;
  Nlm_desktopWindow = (Nlm_WindoW) Nlm_HandNew (sizeof (Nlm_WindowRec));
  Nlm_LoadGraphicData((Nlm_GraphiC)Nlm_desktopWindow, NULL, NULL,
                      NULL, NULL, desktopProcs, NULL, &r,
                      TRUE, TRUE, NULL, NULL);
  Nlm_LoadBoxData((Nlm_BoX)Nlm_desktopWindow,
                  pt, pt, pt, 0, 0, 0, 0, 0, 0, 0, 0);
  Nlm_LoadWindowData(Nlm_desktopWindow,
                     NULL, NULL, NULL, NULL, &r, &r, &r, NULL, NULL,
                     NULL, NULL, NULL, NULL, NULL, TRUE, FALSE, NULL,
                     NULL, NULL, NULL);

  if (DisplayPlanes(Nlm_currentXDisplay, Nlm_currentXScreen) != 1)
    XtAppSetFallbackResources (Nlm_appContext, (String*)defFallbackRes );

  XtDisplayInitialize (Nlm_appContext, Nlm_currentXDisplay,
                       (String)Nlm_AppName, (String)Nlm_VibClass,
                       (XrmOptionDescRec *)NULL, (Cardinal)0,
                       &xx_argc, xx_argv);
  Nlm_SetupArguments(xx_argc, xx_argv);

  Nlm_currentXGC = Nlm_GetGC(DefaultDepth(Nlm_currentXDisplay,
                                          Nlm_currentXScreen));
                             
  Nlm_SetUpDrawingTools();

  Nlm_RegisterStdTranslations();

  Nlm_hScrollBarHeight = 15;
  Nlm_vScrollBarWidth  = 15;

  Nlm_popupMenuHeight  = Nlm_stdLineHeight + 18;
  Nlm_dialogTextHeight = Nlm_stdLineHeight + 6;
  
  arrow = XCreateFontCursor (Nlm_currentXDisplay, XC_top_left_arrow);
  cross = XCreateFontCursor (Nlm_currentXDisplay, XC_cross);
  iBeam = XCreateFontCursor (Nlm_currentXDisplay, XC_xterm);
  plus  = XCreateFontCursor (Nlm_currentXDisplay, XC_crosshair);
  watch = XCreateFontCursor (Nlm_currentXDisplay, XC_watch);

  Nlm_currentCursor = arrow;
  
  font = Nlm_XLoadStandardFont();

  Nlm_XfontList = XmFontListCreate(font, "dummy");

  Nlm_fileDialogShell = NULL;

  return TRUE;
}
#endif  /* WIN_MOTIF */


#ifndef WIN_MOTIF
static void Nlm_WindowGo (Nlm_WindoW w)
{
  inNotice = FALSE;
  Nlm_DoRemove ((Nlm_GraphiC) w, TRUE);
}

static void Nlm_PanelGo (Nlm_PaneL p, Nlm_PoinT pt)
{
  Nlm_WindoW  w;

  inNotice = FALSE;
  w = Nlm_ParentWindow (p);
  Nlm_DoRemove ((Nlm_GraphiC) w, TRUE);
}


static void Nlm_CenterLine (Nlm_RectPtr rptr, Nlm_CharPtr text, Nlm_FonT fnt)
{
  if (fnt != NULL) {
    Nlm_SelectFont (fnt);
  }
  rptr->bottom = (Nlm_Int2)(rptr->top + Nlm_LineHeight());
  Nlm_DrawString (rptr, text, 'c', FALSE);
  rptr->top = rptr->bottom;
}


static void Nlm_DrawAbout (Nlm_PaneL p)

{
  Nlm_RecT  r;

  Nlm_ObjectRect (p, &r);
  Nlm_InsetRect (&r, 4, 4);
  r.top += 10;
  Nlm_Blue ();
  Nlm_CenterLine (&r, "VIBRANT", Nlm_systemFont);
  r.top += 10;
  Nlm_Red ();
  Nlm_CenterLine (&r, "National Center for Biotechnology Information", Nlm_systemFont);
  r.top += 5;
  Nlm_CenterLine (&r, "National Library of Medicine", Nlm_systemFont);
  r.top += 5;
  Nlm_CenterLine (&r, "National Institutes of Health", Nlm_systemFont);
  r.top += 10;
  Nlm_CenterLine (&r, "(301) 496-2475", Nlm_systemFont);
  r.top += 5;
  Nlm_CenterLine (&r, "info@ncbi.nlm.nih.gov", Nlm_systemFont);
}


static void Nlm_GetSet (void)
{
  Nlm_PaneL   p;
  Nlm_WindoW  w;

  w = Nlm_ModalWindow (-50, -33, -1, -1, Nlm_WindowGo);
  p = Nlm_SimplePanel (w, (Nlm_Int2)(28 * Nlm_stdCharWidth),
                          (Nlm_Int2)(10 * Nlm_stdLineHeight), Nlm_DrawAbout);
  Nlm_SetPanelClick (p, NULL, NULL, NULL, Nlm_PanelGo);
  Nlm_DoShow ((Nlm_GraphiC) w, TRUE, TRUE);
  inNotice = TRUE;
  while (inNotice) {
    Nlm_ProcessAnEvent ();
  }
}
#endif  /* !WIN_MOTIF */

#ifdef WIN_MSWIN
static void ParseSetupArguments(HINSTANCE hInstance, Nlm_CharPtr lpszCmdLine)
{
  int    xx_argc;
  char **xx_argv;

  static char winfile[128];
  char *p;

  GetModuleFileName(hInstance, winfile, sizeof(winfile));

  if ((p = strrchr(winfile, DIRDELIMCHR)) != NULL)
    p++;
  else
    p = winfile;
  SetAppProperty("ProgramName", p);

  if (!lpszCmdLine  ||  !*lpszCmdLine)
    {
      xx_argc = 1;
      xx_argv = (char **)Nlm_MemNew( sizeof(char*) );
      xx_argv[0] = winfile;
      Nlm_SetupArguments(xx_argc, xx_argv);
      return;
    }

  /* Count command-line arguments and separate them by '\0' */
  xx_argc = 1;
  for (p = lpszCmdLine;  *p; )
    {
      if ( isspace(*p) )
        {
          *p++ = '\0';
          continue;
        }

      if (*p == '\''  ||  *p == '"')
        {
          char quote = *p;
          xx_argc++;
          while (*(++p)  &&  *p != quote);
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
    int   n = 0;
    char *s = lpszCmdLine;
    xx_argv = (char **)Nlm_MemNew(xx_argc * sizeof(char*));
    xx_argv[n++] = winfile;
    while (n < xx_argc)
      {
        while ( !*s )
          s++;
        if (*s == '\''  ||  *s == '"')
          s++; /* -- skip the leading quote */
        xx_argv[n++] = s;
        while ( *s )
          s++;
      }
#ifdef _DEBUG
    while (s < p  &&  !*s)
      s++;
    ASSERT ( s == p );
#endif
  }}

  Nlm_SetupArguments(xx_argc, xx_argv);
}
#endif


static void Nlm_GetReady (void)
{
/* p_churchill removed conditional compilation for Think C and MPW 12/99
 */
#ifdef OS_MAC
  KeyMap  keys;

  GetKeys (keys);
  if ((keys [1] & 1) != 0) {
    Nlm_GetSet ();
  }
#endif
#ifdef WIN_MSWIN
  if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
    Nlm_GetSet();
#endif
  Nlm_VibrantSetGUI();
  Nlm_InitCursorShapes();
  Nlm_textScrapFull = FALSE;
  Nlm_processUpdatesFirstVal = TRUE;
}

static void Nlm_CleanUpWindows (void)

/*
*  Since submenus goes into the Macintosh desktop menu bar, to allow for
*  command key equivalents, it can be very dangerous to remove the menu
*  bar.  The submenu linked into the menu bar now no longer removes its
*  children, leaving that up to the submenu item.
*/

{
  Nlm_GraphiC  g;
  Nlm_MenuBaR  mb;
  Nlm_GraphiC  n;
  Nlm_WindoW   w;

  okayToDrawContents = FALSE; /* suppress all further slate content drawing */

  Nlm_RemoveDyingWindows();

#ifdef WIN_MSWIN
  if (timerID != 0) {
    KillTimer (NULL, timerID);
  }
#endif
#ifdef WIN_MOTIF
  XtRemoveTimeOut (windowTimer);
#endif
  w = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) Nlm_desktopWindow);
  while (w != NULL && w != Nlm_systemWindow) {
    Nlm_UseWindow (w);
    Nlm_DoHide ((Nlm_GraphiC) w, TRUE, FALSE);
    g = Nlm_GetChild ((Nlm_GraphiC) w);
    while (g != NULL) {
      n = Nlm_GetNext (g);
      Nlm_DoRemove (g, FALSE);
      g = n;
    }
    mb = Nlm_GetWindowMenuBar (w);
    if (mb != NULL) {
      Nlm_DoRemove ((Nlm_GraphiC) mb, FALSE);
      Nlm_SetWindowMenuBar (w, NULL);
    }
    w = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) w);
  }
  w = Nlm_desktopWindow;
  mb = Nlm_GetWindowMenuBar (w);
  if (mb != NULL) {
    Nlm_DoRemove ((Nlm_GraphiC) mb, FALSE);
    Nlm_SetWindowMenuBar (w, NULL);
  }
#ifdef WIN_MOTIF
  resource_nm = (Nlm_CharPtr) Nlm_MemFree (resource_nm);
#endif
}

#ifdef WIN_MAC
void main ()
{
  long gval;
  OSErr  err;

#if __profile__
  ProfilerInit (collectDetailed, bestTimeBase, 100, 20);
  ProfilerSetStatus (FALSE);
#endif

#if TARGET_API_MAC_CARBON >= 1
  // carbon changes the API: pass the number of master pointers to allocate
  //MoreMasters (1280);
  // 2001-03-22:  Joshua Juran
  // CarbonDater report says to use MoreMasterPointers() instead of MoreMasters().
  // Universal Interfaces 3.3.2 declares MoreMasters(void) under Carbon.
  MoreMasterPointers(1280);
  FlushEvents (everyEvent, 0);
  // the rest of the toolbox is done for us can't init them...
#else
  MaxApplZone ();
  if (1) {
    Nlm_Int2 i;
    for (i = 0; i < 20; i++) {
      MoreMasters ();
    }
  }
  InitGraf (&qd.thePort);
  InitFonts ();
  FlushEvents (everyEvent, 0);
  InitWindows ();
  InitMenus ();
  TEInit ();
  InitDialogs (0);
#endif
  InitCursor ();

  err = Gestalt (gestaltAppleEventsAttr, &gval);
  if (err == noErr && ((short) gval & (1 << gestaltAppleEventsPresent)) != 0) {
    /* Create Routine Descriptors - from RasMol */
    HandleAEIgnorePtr = NewAEEventHandlerUPP(HandleAEIgnore);
    HandleAEOpenDocPtr = NewAEEventHandlerUPP(HandleAEOpenDoc);
    HandleAEQuitAppPtr = NewAEEventHandlerUPP(HandleAEQuitApp);
    HandleAEAnswerPtr = NewAEEventHandlerUPP(HandleAEAnswer);

    /* Install Required Event Handlers */
    AEInstallEventHandler(kCoreEventClass,kAEOpenApplication,
                          HandleAEIgnorePtr, 0, FALSE);
    AEInstallEventHandler(kCoreEventClass,kAEOpenDocuments,
                          HandleAEOpenDocPtr, 0, FALSE);
    AEInstallEventHandler(kCoreEventClass,kAEPrintDocuments,
                          HandleAEIgnorePtr, 1, FALSE);
    AEInstallEventHandler(kCoreEventClass,kAEQuitApplication,
                          HandleAEQuitAppPtr, 0, FALSE);
    AEInstallEventHandler(kCoreEventClass,kAEAnswer,
                          HandleAEAnswerPtr, 0, FALSE);
  }

  Nlm_usesMacNavServices = FALSE;
  err = Gestalt (gestaltAppleEventsAttr, &gval);
  if (err == noErr && ((short) gval & (1 << gestaltAppearanceExists)) != 0) {
    if (NavServicesAvailable ()) {
      Nlm_usesMacNavServices = TRUE;
    }
  }
  if (Nlm_usesMacNavServices) {
    NavLoad ();
  }

  Nlm_InitVibrantHooks ();

  Nlm_InitWindows ();
  Nlm_InitBars ();
  Nlm_InitButtons ();
  Nlm_InitExtras ();
  Nlm_InitForms ();
  Nlm_InitGroup ();
  Nlm_InitLists ();
  Nlm_InitMenus ();
  Nlm_InitPrompt ();
  Nlm_InitSlate ();
  Nlm_InitTexts ();
  Nlm_SetupWindows ();
  Nlm_RegisterWindows ();
  Nlm_RegisterTexts ();
  Nlm_RegisterSlates ();
  Nlm_SetupArguments(0, NULL);
  Nlm_GetReady ();

  // There is no need to initialize the TE private scrap because
  // we never assume it's current.

  Nlm_Main ();

  Nlm_CleanUpWindows ();
  Nlm_CleanUpDrawingTools ();
  Nlm_FreeWindows ();
  Nlm_FreeBars ();
  Nlm_FreeButtons ();
  Nlm_FreeExtras ();
  Nlm_FreeForms ();
  Nlm_FreeGroup ();
  Nlm_FreeLists ();
  Nlm_FreeMenus ();
  Nlm_FreePrompt ();
  Nlm_FreeSlate ();
  Nlm_FreeTexts ();
  Nlm_FreeConfigStruct ();
  Nlm_ErrSetLogfile (NULL,0);
  if (Nlm_usesMacNavServices) {
    NavUnload ();
  }

#if __profile__
  ProfilerDump ("\pvibrant.prof");
  ProfilerTerm ();
#endif

  ExitToShell ();
}
#endif

#ifdef WIN_MSWIN
#ifdef WIN32
static int Nlm_HandleException (DWORD code)

{
  Nlm_Message (MSG_OK, "WIN32 exception %ld", (long)code);
  return EXCEPTION_CONTINUE_SEARCH;
}
#endif

int CALLBACK WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPSTR lpszCmdLine, int nCmdShow)
{
  Nlm_Char  str [32];
  WNDCLASS  wc;

#ifdef WIN32
  __try {
#endif

  Nlm_currentHInst = hInstance;

  Nlm_InitVibrantHooks ();

  Nlm_InitWindows ();
  Nlm_InitBars ();
  Nlm_InitButtons ();
  Nlm_InitExtras ();
  Nlm_InitForms ();
  Nlm_InitGroup ();
  Nlm_InitLists ();
  Nlm_InitMenus ();
  Nlm_InitPrompt ();
  Nlm_InitSlate ();
  Nlm_InitTexts ();
  Nlm_SetupWindows ();
  if (! (Nlm_RegisterWindows ())) {
    return FALSE;
  }
  if (! (Nlm_RegisterTexts ())) {
    return FALSE;
  }

  if (! (Nlm_RegisterSlates ())) {
    sprintf (str, "Nlm_WindowClass%ld", (long) (int) Nlm_currentHInst);
    UnregisterClass (str, Nlm_currentHInst);
    return FALSE;
  }


  ParseSetupArguments(Nlm_currentHInst, StringSave(lpszCmdLine));
  Nlm_GetReady ();

  Nlm_Main ();

  Nlm_CleanUpWindows ();
  Nlm_CleanUpDrawingTools ();
  Nlm_FreeWindows ();
  Nlm_FreeBars ();
  Nlm_FreeButtons ();
  Nlm_FreeExtras ();
  Nlm_FreeForms ();
  Nlm_FreeGroup ();
  Nlm_FreeLists ();
  Nlm_FreeMenus ();
  Nlm_FreePrompt ();
  Nlm_FreeSlate ();
  Nlm_FreeTexts ();
  Nlm_FreeConfigStruct ();
  Nlm_ErrSetLogfile (NULL,0);

  sprintf (str, "Nlm_WindowClass%ld", (long) (int) Nlm_currentHInst);
  if (GetClassInfo (Nlm_currentHInst, str, &wc)) {
    DeleteObject (wc.hbrBackground);
  }
  UnregisterClass (str, Nlm_currentHInst);
  sprintf (str, "Nlm_SlateClass%ld", (long) (int) Nlm_currentHInst);
  UnregisterClass (str, Nlm_currentHInst);
  Nlm_MemFree( Nlm_GetArgv() );

#ifdef WIN32
  }
  __except ( Nlm_HandleException( GetExceptionCode() ) )  { }
#endif

  return TRUE;
}
#endif

#ifdef WIN_MOTIF
#if defined(OS_UNIX) && defined(COMP_SUNPRO)
static char **tenvp = NULL;

extern void Nlm_DisplayEnvironmentVariables (void);
extern void Nlm_DisplayEnvironmentVariables (void)

{
  FILE  *f;
  int   i;

  if (tenvp == NULL) return;
  f = Nlm_FileOpen ("EnvLogFile", "a");
  if (f == NULL) return;
  for (i = 0; tenvp [i] != NULL; i++) {
    fprintf (f, "%s\n", tenvp [i]);
  }
  Nlm_FileClose (f);
}
#endif
#endif

#ifdef WIN_MOTIF
#if defined(OS_UNIX) && defined(COMP_SUNPRO)
main (int argc, char *argv[], char *envp[])
#else
main (int argc, char *argv[])
#endif
{
  Nlm_Int2  retval;

  Nlm_SetupArguments(argc, argv);

#if defined(OS_UNIX) && defined(COMP_SUNPRO)
  tenvp = envp;
#endif

  Nlm_InitVibrantHooks ();

  Nlm_InitWindows ();
  Nlm_InitButtons ();
  Nlm_InitExtras ();
  Nlm_InitForms ();
  Nlm_InitGroup ();
  Nlm_InitBars ();
  Nlm_InitLists ();
  Nlm_InitMenus ();
  Nlm_InitPrompt ();
  Nlm_InitSlate ();
  Nlm_InitTexts ();
  if (! Nlm_SetupWindows ()) {
    return FALSE;
  }
  Nlm_RegisterWindows ();
  Nlm_RegisterTexts ();
  Nlm_RegisterSlates ();

  Nlm_GetReady ();
  retval = Nlm_Main ();

  Nlm_CleanUpWindows ();
  Nlm_CleanUpDrawingTools ();
  Nlm_FreeWindows ();
  Nlm_FreeBars ();
  Nlm_FreeButtons ();
  Nlm_FreeExtras ();
  Nlm_FreeForms ();
  Nlm_FreeGroup ();
  Nlm_FreeLists ();
  Nlm_FreeMenus ();
  Nlm_FreePrompt ();
  Nlm_FreeSlate ();
  Nlm_FreeTexts ();
  Nlm_FreeConfigStruct ();
  Nlm_currentWindowTool = (Nlm_WindowTool) 0;
  Nlm_currentXWindow = 0;
  Nlm_currentXGC = NULL;
  if (Nlm_currentXDisplay != NULL) {
    if (Nlm_fileDialogShell != NULL) {
      XtDestroyWidget (Nlm_fileDialogShell);
    }
    if (Nlm_XfontList != NULL) {
      XmFontListFree (Nlm_XfontList);
    }
    XFreeCursor (Nlm_currentXDisplay, arrow);
    XFreeCursor (Nlm_currentXDisplay, cross);
    XFreeCursor (Nlm_currentXDisplay, iBeam);
    XFreeCursor (Nlm_currentXDisplay, plus);
    XFreeCursor (Nlm_currentXDisplay, watch);
    Nlm_FreeGCs ();
    XCloseDisplay (Nlm_currentXDisplay);
  }
  Nlm_ErrSetLogfile (NULL,0);
  exit (retval);
}
#endif

#ifdef WIN_MOTIF 
static Nlm_Boolean IsXParentOf(Window is_parent, Window w)
{
  Window root, parent, *children;
  unsigned int nchildren, i;

  if (is_parent == (Window)0  ||  w == (Window)0) 
    return FALSE;
  else if (is_parent == w)
    return TRUE;

  if ( !XQueryTree(Nlm_currentXDisplay, is_parent, &root, &parent,
                   &children, &nchildren) )
    return FALSE;

  for (i = 0;  i < nchildren  &&  !IsXParentOf(children[i], w);  i++);
  if ( children )
    XFree( children );

  return (i != nchildren);
}
#endif


extern void Nlm_RemoveDyingWindows (void)

/*
*  Since submenus goes into the Macintosh desktop menu bar, to allow for
*  command key equivalents, it can be very dangerous to remove the menu
*  bar.  The submenu linked into the menu bar now no longer removes its
*  children, leaving that up to the submenu item.
*/

{
  Nlm_GraphiC      g;
  Nlm_GraphicData  gdata;
  Nlm_MenuBaR      mb;
  Nlm_GraphiC      n;
#ifndef WIN_MOTIF
  Nlm_PortTool     tempPort;
#endif
  Nlm_WindoW       w;
  Nlm_WindowData   wdata;
  Nlm_WindowTool   wptr;
#ifdef WIN_MAC
  PenState         state;
#endif

  while (dyingWindow != NULL) {
    w = dyingWindow;
    wptr = Nlm_ParentWindowPtr ((Nlm_GraphiC) w);
#ifdef WIN_MAC
    GetPort (&tempPort);
    GetPenState (&state);
    SetPortWindowPort(wptr);
    Nlm_currentWindowTool = wptr;
    Nlm_SetUpdateRegion (wptr);
#endif
#ifdef WIN_MSWIN
    tempPort = Nlm_currentHDC;
    Nlm_currentHDC = Nlm_ParentWindowPort ((Nlm_GraphiC) w);
    Nlm_currentWindowTool = wptr;
#endif
#ifdef WIN_MOTIF
    XSync (Nlm_currentXDisplay, FALSE);
    {{
      Window xw = XtWindow(  Nlm_GetWindowShell(w) );
      if ( IsXParentOf(xw, Nlm_currentXWindow) )
        {
          Nlm_currentWindowTool = (Nlm_WindowTool)0;
          Nlm_currentXWindow = 0;
        }
    }}
#endif
    g = Nlm_GetChild ((Nlm_GraphiC) w);
    while (g != NULL) {
      n = Nlm_GetNext (g);
      Nlm_DoRemove (g, FALSE);
      g = n;
    }

    mb = Nlm_GetWindowMenuBar (w);
    if (mb != NULL) {
      Nlm_DoRemove ((Nlm_GraphiC) mb, FALSE);
      Nlm_SetWindowMenuBar (w, NULL);
    }

#ifndef WIN_MOTIF
    Nlm_ResetDrawingTools ();
#endif

    Nlm_GetWindowData (w, &wdata);
    if (wdata.data != NULL && wdata.cleanup != NULL) {
      wdata.cleanup (w, wdata.data);
    }
#ifdef WIN_MAC
    if ( wdata.cMap != NULL ){
      DisposePalette ( wdata.cMap );
      wdata.cMap = NULL;
    }
    SetPort(tempPort);
    Nlm_currentWindowTool = GetWindowFromPort(tempPort);
    Nlm_SetUpdateRegion (GetWindowFromPort(tempPort));
    SetPenState (&state);
    // 2001-03-22:  Joshua Juran
    // Carbon does not support application-supplied storage for windows.
    DisposeWindow(wptr);
#endif
#ifdef WIN_MSWIN
    if ( wdata.cMap != NULL ){
      DeleteObject( wdata.cMap );
      wdata.cMap = NULL;
    }
    Nlm_currentHDC = tempPort;
    RemoveProp (wptr, (LPSTR) "Nlm_VibrantProp");
    DestroyWindow (wptr);
#endif
#ifdef WIN_MOTIF
    if ( wdata.cMap )  {
      if ( wdata.cMapStatus ) {
        XUninstallColormap(Nlm_currentXDisplay,wdata.cMap);
        wdata.cMapStatus = 0;
      }
      XFreeColormap (Nlm_currentXDisplay,wdata.cMap);
      wdata.cMap = (Nlm_ColorMTool) 0;
    }
    if (wdata.title != NULL) {
      Nlm_MemFree (wdata.title);
    }

    {{
      Nlm_ShellTool shl = Nlm_GetWindowShell( w );
      XtUnrealizeWidget(shl);
      if (
#ifdef NLM_MOTIF_CASCADEB_BUG
          !mb  &&
#endif
          (!NLM_QUIET  ||  Nlm_WindowHasBeenShown( w ))) {
        XtDestroyWidget( shl );
      }
    }}
    XSync (Nlm_currentXDisplay, FALSE);
#endif
    dyingWindow = (Nlm_WindoW) Nlm_GetNext ((Nlm_GraphiC) w);
    Nlm_GetGraphicData ((Nlm_GraphiC) w, &gdata);
    if (gdata.data != NULL && gdata.cleanup != NULL) {
      gdata.cleanup ((Nlm_GraphiC) w, gdata.data);
    }
    Nlm_HandFree (w);
    recentWindow = NULL;
    Nlm_Update ();
  }
}


#ifdef WIN_X
static Boolean Nlm_XCheckUpdateEvent(XEvent *event)
{
  return XCheckMaskEvent(Nlm_currentXDisplay, ExposureMask|StructureNotifyMask,
                         event);
}
#endif


#ifdef WIN_MAC
static Nlm_RegioN macMouseRgn = NULL;
#endif

extern void Nlm_SetMouseMoveCallback (Nlm_ClckProc actn)

{
  mouseMovedAction = actn;
}

extern void Nlm_SetMouseMoveRegion (Nlm_RegioN rgn)

{
#ifdef WIN_MAC
  if (macMouseRgn != NULL) {
    macMouseRgn = Nlm_DestroyRgn (macMouseRgn);
  }
  if (rgn == NULL) return;
  macMouseRgn = Nlm_CreateRgn ();
  Nlm_UnionRgn (macMouseRgn, rgn, macMouseRgn);
#endif
}

extern void Nlm_Update (void)
{
#ifdef WIN_MAC
  while (EventAvail (updateMask, &Nlm_currentEvent)) {
    if (WaitNextEvent (updateMask, &Nlm_currentEvent, 0, (Nlm_RgnTool) macMouseRgn)) {
      Nlm_HandleEvent ();
    }
  }
#endif
#ifdef WIN_MSWIN
  while (PeekMessage (&Nlm_currentMssg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE)) {
    if (GetMessage (&Nlm_currentMssg, NULL, WM_PAINT, WM_PAINT)) {
      if (Nlm_VibrantDisabled ()) {
        discard_count++; /* really just a place to set a breakpoint */
      }
      TranslateMessage (&Nlm_currentMssg);
      DispatchMessage (&Nlm_currentMssg);
    }
  }
#endif
#ifdef WIN_MOTIF
  XEvent event;
  XSync (Nlm_currentXDisplay, FALSE);
  while ( Nlm_XCheckUpdateEvent(&event) )
    {
      XtDispatchEvent (&event);
    }
  XFlush( Nlm_currentXDisplay );
#endif
}

extern void Nlm_KeyboardView (Nlm_KeyProc key)

{
  keyAction = key;
}

#ifdef WIN_MSWIN
static void Nlm_ProcessKeyPress (LPMSG lpMsg)

{
  Nlm_Char  ch;

  if (lpMsg->message == WM_CHAR) {
    ch = (Nlm_Char) lpMsg->wParam;
    if (keyAction != NULL) {
      keyAction (ch);
    }
  }
}
#endif

#ifdef WIN_MOTIF
static void Nlm_ProcessKeyPress (XEvent *event)

{
  Nlm_Char buffer[2];

  if (event->type == KeyPress  &&  keyAction != NULL  &&
      XLookupString(&event->xkey, buffer, sizeof(buffer), NULL, NULL) == 1)
    keyAction( *buffer );
}
#endif

extern void Nlm_ProcessAnEvent (void)

{
#ifdef WIN_MAC
  if (EventAvail (everyEvent, &Nlm_currentEvent)) {
    if (WaitNextEvent (everyEvent, &Nlm_currentEvent, 0, (Nlm_RgnTool) macMouseRgn)) {
      Nlm_HandleEvent ();
    }
  }
#endif
#ifdef WIN_MSWIN
  if (PeekMessage (&Nlm_currentMssg, NULL, 0, 0, PM_NOREMOVE)) {
    if (GetMessage (&Nlm_currentMssg, NULL, 0, 0)) {
      TranslateMessage (&Nlm_currentMssg);
      Nlm_ProcessKeyPress (&Nlm_currentMssg);
      DispatchMessage (&Nlm_currentMssg);
    }
  }
#endif
#ifdef WIN_MOTIF
  XEvent  event;

  if (XtAppPending (Nlm_appContext) & XtIMXEvent) {
    XtAppNextEvent (Nlm_appContext, &event);
    Nlm_ProcessKeyPress (&event);
    XtDispatchEvent (&event);
  }
#endif
}

extern void Nlm_ProcessTimerEvent (void)

{
#ifdef WIN_MAC
  Nlm_Int4  delta;
  Nlm_Int4  ticks;

  ticks = Nlm_ComputerTime ();
  delta = ticks - lastTimerTime;
  if (delta >= 3) {
    lastTimerTime = ticks;
    if (timerAction != NULL) {
      timerAction ();
    }
    Nlm_CallWindowTimers ();
  }
#endif
}

#ifdef WIN_MAC
static void Nlm_ProcessIdle (void)

{
  Nlm_PointTool  ptool;

  Nlm_ProcessTimerEvent ();
  GetMouse (&ptool);
  LocalToGlobal (&ptool);
  Nlm_PointToolToPoinT (ptool, &Nlm_globalMouse);
  frontWindow = Nlm_FindWindowRec (FrontWindow ());
  Nlm_DoIdle ((Nlm_GraphiC) frontWindow, Nlm_globalMouse);
  Nlm_ClearKeys ();
}
#endif

extern void Nlm_ProcessEventOrIdle (void)

{
#ifdef WIN_MAC
  if (EventAvail (everyEvent, &Nlm_currentEvent)) {
    if (WaitNextEvent (everyEvent, &Nlm_currentEvent, 0, (Nlm_RgnTool) macMouseRgn)) {
      Nlm_HandleEvent ();
    }
  } else {
    Nlm_ProcessIdle ();
  }
#endif
#ifdef WIN_MSWIN
  if (Nlm_processUpdatesFirstVal &&
      PeekMessage (&Nlm_currentMssg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE)) {
    if (GetMessage (&Nlm_currentMssg, NULL, WM_PAINT, WM_PAINT)) {
      TranslateMessage (&Nlm_currentMssg);
      DispatchMessage (&Nlm_currentMssg);
    }
  } else if (PeekMessage (&Nlm_currentMssg, NULL, 0, 0, PM_NOREMOVE)) {
    if (GetMessage (&Nlm_currentMssg, NULL, 0, 0)) {
      TranslateMessage (&Nlm_currentMssg);
      Nlm_ProcessKeyPress (&Nlm_currentMssg);
      DispatchMessage (&Nlm_currentMssg);
    }
  }
#endif
#ifdef WIN_MOTIF
  XEvent  event;

  if (XtAppPending (Nlm_appContext) & XtIMXEvent) {
    if (Nlm_processUpdatesFirstVal &&
        Nlm_XCheckUpdateEvent(&event)) {
      XtDispatchEvent (&event);
    } else {
      XtAppNextEvent (Nlm_appContext, &event);
      Nlm_ProcessKeyPress (&event);
      XtDispatchEvent (&event);
    }
  }
#endif
}

extern void Nlm_ProcessExternalEvent (void)

{
#ifdef WIN_MAC
  if (WaitNextEvent (everyEvent, &Nlm_currentEvent, 10, (Nlm_RgnTool) macMouseRgn)) {
    Nlm_HandleEvent ();
  }
#endif
#ifdef WIN_MSWIN
  if (Nlm_processUpdatesFirstVal &&
      PeekMessage (&Nlm_currentMssg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE)) {
    if (GetMessage (&Nlm_currentMssg, NULL, WM_PAINT, WM_PAINT)) {
      TranslateMessage (&Nlm_currentMssg);
      DispatchMessage (&Nlm_currentMssg);
    }
  } else if (PeekMessage (&Nlm_currentMssg, NULL, 0, 0, PM_NOREMOVE)) {
    if (GetMessage (&Nlm_currentMssg, NULL, 0, 0)) {
      TranslateMessage (&Nlm_currentMssg);
      Nlm_ProcessKeyPress (&Nlm_currentMssg);
      DispatchMessage (&Nlm_currentMssg);
    }
  }
#endif
#ifdef WIN_MOTIF
  XEvent  event;

  if (XtAppPending (Nlm_appContext) & XtIMXEvent) {
    if (Nlm_processUpdatesFirstVal && 
        Nlm_XCheckUpdateEvent(&event)) {
      XtDispatchEvent (&event);
    } else {
      XtAppNextEvent (Nlm_appContext, &event);
      Nlm_ProcessKeyPress (&event);
      XtDispatchEvent (&event);
    }
  }
#endif
}

extern void Nlm_ProcessEvents (void)

{
#ifdef WIN_MAC
  while (! quitProgram) {
    if (! EventAvail (everyEvent, &Nlm_currentEvent)) {
      Nlm_ProcessIdle ();
    }
    while (WaitNextEvent (everyEvent, &Nlm_currentEvent, 1, (Nlm_RgnTool) macMouseRgn)) {
      Nlm_HandleEvent ();
      Nlm_RemoveDyingWindows ();
    }
  }
#endif
#ifdef WIN_MSWIN
  if (registeredDropProc != NULL) {
    DragAcceptFiles(Nlm_currentHWnd, TRUE);
  }
  while (! quitProgram) {
    while (Nlm_processUpdatesFirstVal &&
           PeekMessage (&Nlm_currentMssg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE)) {
      if (GetMessage (&Nlm_currentMssg, NULL, WM_PAINT, WM_PAINT)) {
        TranslateMessage (&Nlm_currentMssg);
        DispatchMessage (&Nlm_currentMssg);
      }
    }
    if (GetMessage (&Nlm_currentMssg, NULL, 0, 0)) {
      TranslateMessage (&Nlm_currentMssg);
      Nlm_ProcessKeyPress (&Nlm_currentMssg);
      DispatchMessage (&Nlm_currentMssg);
      Nlm_RemoveDyingWindows ();
    }
  }
#endif
#ifdef WIN_MOTIF
  XEvent       event;
  XtInputMask  mask;

  while (! quitProgram) {
    mask = XtAppPending (Nlm_appContext);
    if (mask != 0) { 
      while (Nlm_processUpdatesFirstVal &&
             Nlm_XCheckUpdateEvent(&event)) {
        XtDispatchEvent (&event);
      }
    }
    XtAppNextEvent (Nlm_appContext, &event);
    Nlm_ProcessKeyPress (&event);
    XtDispatchEvent (&event);
    Nlm_RemoveDyingWindows ();
  }
  if (Nlm_currentXDisplay != NULL) {
    XSync (Nlm_currentXDisplay, TRUE);
  }
#endif
  Nlm_RemoveDyingWindows ();
}

extern Nlm_Boolean Nlm_EventAvail (void)

{
#ifdef WIN_MAC
  return (EventAvail (everyEvent, &Nlm_currentEvent));
#endif
#ifdef WIN_MSWIN
  return (Nlm_Boolean) (PeekMessage (&Nlm_currentMssg, NULL, 0, 0, PM_NOREMOVE | PM_NOYIELD));
#endif
#ifdef WIN_MOTIF
  XtInputMask  mask;
  mask = XtAppPending (Nlm_appContext);
  return ((mask & XtIMXEvent) != 0); /* M.Gouy */
#endif
}

extern void Nlm_FlushEvents (void)

{
#ifdef WIN_MAC
  FlushEvents (everyEvent - updateMask, 0);
#endif
#ifdef WIN_MSWIN
  Nlm_Boolean  goOn;

  goOn = TRUE;
  while (goOn) {
    goOn = (Nlm_Boolean) PeekMessage (&Nlm_currentMssg, NULL, 0, 0, PM_REMOVE | PM_NOYIELD);
    if (goOn && Nlm_currentMssg.message == WM_PAINT) {
      goOn = FALSE;
    }
  }
#endif
#ifdef WIN_MOTIF
  if (Nlm_currentXDisplay != NULL) {
    XSync (Nlm_currentXDisplay, FALSE);
  }
#endif
}

extern void Nlm_ProcessUpdatesFirst (Nlm_Boolean val)

{
  Nlm_processUpdatesFirstVal = val;
}


extern void Nlm_QuitProgram (void)
{
  quitProgram = TRUE;
#ifdef WIN_MSWIN
  PostQuitMessage (0);
#endif
}

extern Nlm_Boolean Nlm_QuittingProgram (void)
{
  return quitProgram;
}


extern void Nlm_UnloadSegment (Nlm_VoidProc routineAddr)

{
#ifdef WIN_MAC
  if (routineAddr != NULL) {
    UnloadSeg ((void *) routineAddr);
  }
#endif
}

static void Nlm_GetArgsOkProc (Nlm_ButtoN b)

{
  getArgsBoxUp = FALSE;
  getArgsOk = TRUE;
}

static void Nlm_GetArgsCancelProc (Nlm_ButtoN b)

{
  getArgsBoxUp = FALSE;
}

static Nlm_GraphiC Nlm_GetPrevObject (Nlm_GraphiC g)

{
  Nlm_GraphiC  prev = NULL;
  if (g != NULL) {
    Nlm_GraphiC prnt = Nlm_GetParent (g);
    Nlm_GraphiC chld = Nlm_GetChild (prnt);
    while (chld  &&  chld != g) {
      prev = chld;
      chld = Nlm_GetNext(chld);
    }
  }
  return prev;
}

static void Nlm_GetArgsFileInProc (Nlm_ButtoN b)

{
  Nlm_Char  path [PATH_MAX];
  Nlm_TexT  t;

  t = (Nlm_TexT) Nlm_GetPrevObject ((Nlm_GraphiC) b);
  if (t != NULL) {
    if (Nlm_GetInputFileName (path, sizeof (path), "", "")) {
      Nlm_SetTitle ((Nlm_Handle) t, path);
    }
  }
}

static void Nlm_GetArgsFileOutProc (Nlm_ButtoN b)

{
  Nlm_Char  path [PATH_MAX];
  Nlm_TexT  t;

  t = (Nlm_TexT) Nlm_GetPrevObject ((Nlm_GraphiC) b);
  if (t != NULL) {
    if (Nlm_GetOutputFileName (path, sizeof (path), "")) {
      Nlm_SetTitle ((Nlm_Handle) t, path);
    }
  }
}


typedef Nlm_Handle PNTR Nlm_HandlePtr;

static void Nlm_MultiLinePrompt (Nlm_GrouP prnt, Nlm_CharPtr str, Nlm_Int2 width,
                                 Nlm_Int2 height, Nlm_FonT fnt, Nlm_Char just)

{
  Nlm_CharPtr    buf;
  Nlm_Char       ch;
  Nlm_Int2       k;
  Nlm_Int2       l;
  Nlm_GrouP      pg;

  if (prnt != NULL && str != NULL) {
    pg = Nlm_HiddenGroup (prnt, 0, 10, NULL);
    Nlm_SetGroupMargins (pg, 1, 1);
    Nlm_SetGroupSpacing (pg, 1, 1);
    buf = StringSave (str);
    k = 0;
    while (Nlm_StringLen (buf + k) > 0) {
      l = 0;
      ch = buf [k + l];
      while (ch != '\0' && ch != '\n' && ch != '\r') {
        l++;
        ch = buf [k + l];
      }
      if (ch == '\n' || ch == '\r') {
        buf [k + l] = '\0';
        if (Nlm_StringLen (buf + k) > 0) {
          Nlm_StaticPrompt (pg, buf + k, width, height, fnt, just);
        }
        k += l + 1;
      } else {
        if (Nlm_StringLen (buf + k) > 0) {
          Nlm_StaticPrompt (pg, buf + k, width, height, fnt, just);
        }
        k += l;
      }
    }
    MemFree (buf);
  }
}


typedef enum
{
  FetchArg_SILENT,
  FetchArg_ERRPOST,
  FetchArg_MESSAGE
}  enumFetchArgDiag;

static Nlm_Boolean FetchArg(Nlm_ArgPtr arg, const char *str,
                            enumFetchArgDiag diag, Nlm_Boolean check_range)
{
  static char *xxx = "...";
  char *min_str = arg->from ? arg->from : xxx;
  char *max_str = arg->to   ? arg->to   : xxx;
  Nlm_Boolean valid = TRUE;

  switch ( arg->type )
    {
    case ARG_BOOLEAN:
      {
        arg->intvalue =
          (str == NULL  ||  *str == '\0'  ||  TO_UPPER(*str) == 'T') ? 1 : 0;
        break;
      }

    case ARG_INT:
      {
        long value;
        if (str == NULL  ||  sscanf(str, "%ld", &value) != 1)
          valid = FALSE;

        if (valid  &&  !check_range) {
          arg->intvalue = value;
          break;
        }

        /* range checking */
        if (valid  &&  arg->from != NULL)
          {
            long min_value;
            if (sscanf(arg->from, "%ld", &min_value) != 1  ||
                value < min_value)
              valid = FALSE;
          }
        if (valid  &&  arg->to != NULL)
          {
            long max_value;
            if (sscanf(arg->to,   "%ld", &max_value) != 1  ||
                value > max_value)
              valid = FALSE;
          }

        if ( valid )
          arg->intvalue = value;
        break;
      }

    case ARG_FLOAT:
      {
        double value;
        if (str == NULL  ||  sscanf(str, "%lf", &value) != 1)
          valid = FALSE;

        if (valid  &&  !check_range) {
          arg->floatvalue = value;
          break;
        }

        /* range checking */
        if (valid  &&  arg->from != NULL)
          {
            double min_value;
            if (sscanf(arg->from, "%lg", &min_value) != 1  ||
                value < min_value)
              valid = FALSE;
          }
        if (valid  &&  arg->to != NULL)
          {
            double max_value;
            if (sscanf(arg->to,   "%lg", &max_value) != 1  ||
                value > max_value)
              valid = FALSE;
          }

        if ( valid )
          arg->floatvalue = value;
        break;
      }

    case ARG_STRING:
    case ARG_FILE_IN:
    case ARG_FILE_OUT:
    case ARG_DATA_IN:
    case ARG_DATA_OUT:
      if (str == NULL  ||  *str == '\0')
        arg->strvalue = NULL;
      else
        arg->strvalue = StringSave( str );
      break;

    default:
      valid = FALSE;
    }

  if ( !valid )
    {
      switch ( diag )
        {
        case FetchArg_SILENT:
          break;
        case FetchArg_ERRPOST:
          ErrPostEx(SEV_ERROR, 0,0,
                    "%s ::  '%s' is bad or out of range [%s to %s]",
                    arg->prompt, str, min_str, max_str);
          break;
        case FetchArg_MESSAGE:
          Message(MSG_ERROR,
                  "%s ::  '%s' is bad or out of range [%s to %s]",
                  arg->prompt, str, min_str, max_str);
          break;
        }
      return FALSE;
    }

  return TRUE;
}


static Nlm_Boolean GetArgs_ST(const char* progname,
                              Nlm_Int2 numargs, Nlm_ArgPtr ap,
                              Nlm_Boolean silent)
{
  static const char* s_TypeStrings[] = {
    "",
    "",
    "Integer: ",
    "Float: ",
    "String: ",
    "File In: ",
    "File Out: ",
    "Data In: ",
    "Data Out: "
  };

  static const char* s_ValueStrings[] = {
    "",
    "T/F",
    "Integer",
    "Float",
    "String",
    "Input-File",
    "Output-File",
    "Input-Data",
    "Output-Data"
  };

  Nlm_Int2       i, j;
  Nlm_ArgPtr     curarg;
  Nlm_Boolean   *resolved;
  Nlm_Char       arg[256];
  Nlm_Int2       delta;
  Nlm_GrouP      g;
  Nlm_GrouP      h;
  Nlm_HandlePtr  hp;
  Nlm_RecT       r1;
  Nlm_RecT       r2;
  Nlm_Boolean    smallScreen;
  Nlm_WindoW     w;
  Nlm_TexT       firstText;
  Nlm_CharPtr    tmp;

  int    xx_argc = (int)Nlm_GetArgc();
  char **xx_argv =      Nlm_GetArgv();

  if (ap == NULL  &&  numargs == 0)
    return TRUE;

  if (ap == NULL  ||  numargs <= 0)
    return FALSE;

  resolved = (Nlm_Boolean*) Nlm_MemNew(numargs * sizeof(Nlm_Boolean));
  if ( !resolved )
    return FALSE;

  /* Initialize arguments using default value */
  for (i = 0, curarg = ap;  i < numargs;  i++, curarg++)
    {
      curarg->intvalue   = 0;
      curarg->floatvalue = 0.0;
      curarg->strvalue   = NULL;

      if (curarg->defaultvalue == NULL)
        continue;

      switch ( curarg->type )
        {
        case ARG_BOOLEAN:
        case ARG_INT:
        case ARG_FLOAT:
        case ARG_STRING:
        case ARG_FILE_IN:
        case ARG_FILE_OUT:
        case ARG_DATA_IN:
        case ARG_DATA_OUT:
          resolved[i] = FetchArg(curarg, curarg->defaultvalue,
                                 FetchArg_ERRPOST, FALSE);
          break;
        default:
          ErrPostEx(SEV_ERROR, 0,0, "Invalid Arg->type in %s", curarg->prompt);
          Nlm_MemFree( resolved );
          return FALSE;
      }
    }


  /* Print the usage info */
#ifdef WIN_MOTIF
  if (xx_argc == 2  &&  xx_argv[1][1] == '\0')
    {
      printf("\n%s   arguments:\n\n", progname);
      curarg = ap;

      for (i = 0, j = 0;  i < numargs;  i++, j++, curarg++)
        {
          printf("  -%c  %s [%s]",
                 curarg->tag, curarg->prompt, s_ValueStrings[curarg->type]);
          if (curarg->optional )
            printf( "  Optional" );
          printf("\n");
          if (curarg->defaultvalue != NULL)
            printf("    default = %s\n", curarg->defaultvalue);
          if (curarg->from != NULL  ||  curarg->to != NULL)
            {
              if (curarg->type == ARG_DATA_IN  || curarg->type == ARG_DATA_OUT)
                printf("    Data Type = %s\n",
                       (curarg->from ? curarg->from : "<NULL>"));
              else
                printf("    range from %s to %s\n",
                       (curarg->from ? curarg->from : "<NULL>"),
                       (curarg->to   ? curarg->to   : "<NULL>"));
            }
        }

      printf("\n");
      Nlm_MemFree( resolved );
      return FALSE;
    }
#endif


  /* Parse command-line arguments */
  for (i = 1;  i < xx_argc;  i++)
    {
      Nlm_CharPtr arg_str = xx_argv[i];
      if (*arg_str != '-') {
        ErrPostEx(SEV_ERROR, 0, 0,
                  "\n%s\n(offending argument #%d was: '%s')",
                  "Vibrant command line arguments must start with '-'",
                  (int)i, arg_str);
        Nlm_MemFree( resolved );
        return FALSE;
      }

      arg_str++;
      curarg = ap;
      for (j = 0;  j < numargs  &&  *arg_str != curarg->tag;  j++, curarg++) continue;
      if (j == numargs) {
        ErrPostEx(SEV_ERROR, 0, 0, "Invalid argument: %s", xx_argv[i]);
        Nlm_MemFree( resolved );
        return FALSE;
      }

      arg_str++;
      if (*arg_str == '\0'  &&  curarg->type != ARG_BOOLEAN)
        {
          if (++i == xx_argc)  {
            ErrPostEx(SEV_ERROR, 0, 0,
                      "No argument given for %s", curarg->prompt);
            Nlm_MemFree( resolved );
            return FALSE;
          }
          arg_str = xx_argv[i];
          ASSERT( arg_str  &&  *arg_str );
      }

      if ( !FetchArg(curarg, arg_str, FetchArg_ERRPOST, TRUE) )
        {
          Nlm_MemFree( resolved );
          return FALSE;
        }
      resolved[j] = TRUE;
    }       

  silent = (Nlm_Boolean)(silent  ||  xx_argc > 1);

  if ( silent )
    {
      for (i = 0;
           i < numargs  &&  (ap[i].optional  ||  resolved[i]);
           i++) continue;
      if (i == numargs) {
        Nlm_MemFree( resolved );
        return TRUE; /* all args resolved from command line or by default */
      }
    }


  /* Arg-Query Dialog Box */ 
  w = Nlm_FixedWindow (-50, -33, -10, -10, (Nlm_CharPtr) progname, NULL);
  smallScreen = FALSE;
#ifdef WIN_MAC
  if (Nlm_screenRect.right < 513  ||  Nlm_screenRect.bottom < 343)
    smallScreen = TRUE;
#endif
#ifdef WIN_MSWIN
  if (Nlm_screenRect.bottom < 352)
    smallScreen = TRUE;
#endif
  g = Nlm_HiddenGroup (w, 4, 0, NULL);
  hp = (Nlm_HandlePtr) Nlm_MemNew (numargs * sizeof (Nlm_Handle));

  firstText = NULL;
  curarg = ap;
  for (i = 0, j = 0; i < numargs; i++, j++, curarg++) {
    if ((smallScreen && j >= 10) || j >= 15) {
      j = 0;
      Nlm_Advance (w);
      g = Nlm_HiddenGroup (w, 4, 0, NULL);
    }
    Nlm_StaticPrompt(g, (char*)s_TypeStrings[curarg->type], 0,
                     Nlm_dialogTextHeight, Nlm_systemFont, 'l');

    /* Populate the Arg-Query Dialog Box's input controls and
       initialize these by default values */
    switch (curarg->type) {
      case ARG_BOOLEAN:
        hp[i] = (Nlm_Handle)
          Nlm_CheckBox(g, (Nlm_CharPtr) curarg->prompt, NULL);
        Nlm_StaticPrompt (g, "", 0, 0, Nlm_systemFont, 'l');
        if (curarg->intvalue == 1) {
          Nlm_SetStatus (hp [i], TRUE);
        }
        break;
      case ARG_INT:
      case ARG_FLOAT:
      case ARG_STRING:
      case ARG_FILE_IN:
      case ARG_FILE_OUT:
      case ARG_DATA_IN:
      case ARG_DATA_OUT:
        Nlm_MultiLinePrompt (g, (Nlm_CharPtr) curarg->prompt, 0, Nlm_dialogTextHeight, Nlm_systemFont, 'l');
        hp[i] = (Nlm_Handle) Nlm_DialogText(g, (Nlm_CharPtr) curarg->defaultvalue, 10, NULL);
        if (firstText == NULL) {
          firstText = (Nlm_TexT) hp[i];
        }
        break;
    }

    arg[0] = '\0';
    if ((curarg->from != NULL) || (curarg->to != NULL)) {
      tmp = Nlm_StringMove(arg, " (");
      tmp = Nlm_StringMove(tmp, ((curarg->from) != NULL ? (curarg->from): "<NULL>"));
      if ((curarg->type == ARG_INT) || (curarg->type == ARG_FLOAT)) {
        tmp = Nlm_StringMove(tmp, " to ");
        tmp = Nlm_StringMove(tmp, ((curarg->to) != NULL ? ( curarg->to): "<NULL>"));
      }
      Nlm_StringMove(tmp, ")");
    }

    if (curarg->type == ARG_FILE_IN) {
      Nlm_PushButton (g, "File IN", Nlm_GetArgsFileInProc);
    } else if (curarg->type == ARG_FILE_OUT) {
      Nlm_PushButton (g, "File OUT", Nlm_GetArgsFileOutProc);
    } else {
      Nlm_StaticPrompt(g, arg, 0,  Nlm_dialogTextHeight, Nlm_systemFont, 'l');
    }
  }

  Nlm_Break (w);
  h = Nlm_HiddenGroup (w, 10, 0, NULL);
  Nlm_PushButton (h, "OK", Nlm_GetArgsOkProc);
  Nlm_PushButton (h, "Cancel", Nlm_GetArgsCancelProc);
  Nlm_GetRect ((Nlm_GraphiC) g, &r1);
  Nlm_DoGetPosition ((Nlm_GraphiC) h, &r2);
  delta = (Nlm_Int2)(r1.right - r2.right);
  if (delta > 0) {
    Nlm_OffsetRect (&r2, (Nlm_Int2)(delta / 2), 0);
    Nlm_DoSetPosition ((Nlm_GraphiC) h, &r2, TRUE, FALSE);
  }
  Nlm_DoShow ((Nlm_GraphiC) w, TRUE, TRUE);
  if (firstText != NULL)
    Nlm_DoSelect ((Nlm_GraphiC) firstText, TRUE);

  /* Get & handle user input */
  getArgsBoxUp = TRUE;
  while ( getArgsBoxUp )
    {
      getArgsOk = FALSE;
      while (getArgsBoxUp  &&  !getArgsOk)
        Nlm_ProcessEventOrIdle();

      if ( !getArgsOk )
        continue;

      curarg = ap;
      for (i = 0;  i < numargs  &&  !getArgsBoxUp;  i++, curarg++)
        {
          resolved[i] = TRUE;
          switch ( curarg->type )
            {
            case ARG_BOOLEAN:
              {
                if ( Nlm_GetStatus( hp[i] ) )
                  curarg->intvalue = 1;
                else
                  curarg->intvalue = 0;
                break;
              }

            case ARG_INT:
            case ARG_FLOAT:
            case ARG_STRING:
            case ARG_FILE_IN:
            case ARG_FILE_OUT:
            case ARG_DATA_IN:
            case ARG_DATA_OUT:
              {
                Nlm_Char str_value[256];
                Nlm_GetTitle(hp[i], str_value, sizeof(str_value));
                if ( !FetchArg(curarg, str_value, FetchArg_MESSAGE, TRUE) )
                  {
                    Nlm_DoSelect((Nlm_GraphiC)hp[i], TRUE);
                    getArgsBoxUp = TRUE;
                  }
                break;
              }

            default:
              ASSERT ( FALSE );
              Nlm_MemFree( resolved );
              return FALSE;
            }
        }

      curarg = ap;
      if ( getArgsBoxUp )
        continue;

      for (i = 0;  i < numargs;  i++, curarg++)
        if (!curarg->optional  &&  !resolved[i])
          {
            Message(MSG_ERROR, "%s was not given an argument", curarg->prompt);
            getArgsBoxUp = TRUE;
            Nlm_DoSelect((Nlm_GraphiC)hp[i], TRUE);
            break;
          }
    }  
  Nlm_DoRemove ((Nlm_GraphiC) w, TRUE);
  Nlm_Update ();

  if (getArgsOk) {  /* leave up a little window with the program name */
    w = Nlm_FixedWindow (-50, -33, -10, -10, (Nlm_CharPtr) progname, NULL);
    Nlm_DoShow ((Nlm_GraphiC) w, TRUE, TRUE);
  }

  Nlm_MemFree( resolved );
  return getArgsOk;
}


extern Nlm_Boolean Nlm_GetArgs(const char* progname,
                               Nlm_Int2 numargs, Nlm_ArgPtr ap)
{
  return GetArgs_ST(progname, numargs, ap, FALSE);
}


extern Nlm_Boolean Nlm_GetArgsSilent(const char* progname,
                                     Nlm_Int2 numargs, Nlm_ArgPtr ap)
{
  return GetArgs_ST(progname, numargs, ap, TRUE);
}


extern void Nlm_FreeWindows (void)
{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemFree (gphprcsptr);
}

extern void Nlm_InitWindows (void)
{
  gphprcsptr = (Nlm_GphPrcsPtr) Nlm_MemNew (sizeof (Nlm_GphPrcs) * 12);

  documentProcs = &(gphprcsptr [0]);
#ifdef WIN_MAC
  documentProcs->click = Nlm_DocumentClick;
  documentProcs->key = Nlm_NormalKey;
  documentProcs->draw = Nlm_DocumentDraw;
  documentProcs->idle = Nlm_NormalIdle;
#endif
  documentProcs->show = Nlm_ShowDocument;
  documentProcs->hide = Nlm_HideWindow;
  documentProcs->activate = Nlm_DocumentActivate;
  documentProcs->deactivate = Nlm_DocumentDeactivate;
  documentProcs->remove = Nlm_RemoveWindow;
  documentProcs->select = Nlm_NormalSelect;
  documentProcs->linkIn = Nlm_LinkIn;
  documentProcs->adjustPrnt = Nlm_AdjustWindow;
  documentProcs->setTitle = Nlm_SetWindowTitle;
  documentProcs->getTitle = Nlm_GetWindowTitle;
  documentProcs->setPosition = Nlm_SetWindowPosition;
  documentProcs->getPosition = Nlm_GetWindowPosition;
  documentProcs->gainFocus = Nlm_WindowGainFocus;
  documentProcs->loseFocus = Nlm_WindowLoseFocus;
  documentProcs->sendChar = Nlm_NormalDrawChar;

  fixedProcs = &(gphprcsptr [1]);
#ifdef WIN_MAC
  fixedProcs->click = Nlm_DialogClick;
  fixedProcs->key = Nlm_NormalKey;
  fixedProcs->draw = Nlm_NormalDraw;
  fixedProcs->idle = Nlm_NormalIdle;
#endif
  fixedProcs->show = Nlm_ShowNormal;
  fixedProcs->hide = Nlm_HideWindow;
  fixedProcs->activate = Nlm_NormalActivate;
  fixedProcs->deactivate = Nlm_NormalDeactivate;
  fixedProcs->remove = Nlm_RemoveWindow;
  fixedProcs->select = Nlm_NormalSelect;
  fixedProcs->countItems = NULL;
  fixedProcs->linkIn = Nlm_LinkIn;
  fixedProcs->adjustPrnt = Nlm_AdjustWindow;
  fixedProcs->setTitle = Nlm_SetWindowTitle;
  fixedProcs->getTitle = Nlm_GetWindowTitle;
  fixedProcs->setPosition = Nlm_SetWindowPosition;
  fixedProcs->getPosition = Nlm_GetWindowPosition;
  fixedProcs->gainFocus = Nlm_WindowGainFocus;
  fixedProcs->loseFocus = Nlm_WindowLoseFocus;
  fixedProcs->sendChar = Nlm_NormalDrawChar;

  frozenProcs = &(gphprcsptr [2]);
#ifdef WIN_MAC
  frozenProcs->click = Nlm_FrozenClick;
  frozenProcs->key = Nlm_NormalKey;
  frozenProcs->draw = Nlm_NormalDraw;
  frozenProcs->idle = Nlm_NormalIdle;
#endif
  frozenProcs->show = Nlm_ShowNormal;
  frozenProcs->hide = Nlm_HideWindow;
  frozenProcs->activate = Nlm_NormalActivate;
  frozenProcs->deactivate = Nlm_NormalDeactivate;
  frozenProcs->remove = Nlm_RemoveWindow;
  frozenProcs->select = Nlm_NormalSelect;
  frozenProcs->linkIn = Nlm_LinkIn;
  frozenProcs->adjustPrnt = Nlm_AdjustWindow;
  frozenProcs->setTitle = Nlm_SetWindowTitle;
  frozenProcs->getTitle = Nlm_GetWindowTitle;
  frozenProcs->setPosition = Nlm_SetWindowPosition;
  frozenProcs->getPosition = Nlm_GetWindowPosition;
  frozenProcs->gainFocus = Nlm_WindowGainFocus;
  frozenProcs->loseFocus = Nlm_WindowLoseFocus;
  frozenProcs->sendChar = Nlm_NormalDrawChar;

  roundProcs = &(gphprcsptr [3]);
#ifdef WIN_MAC
  roundProcs->click = Nlm_DialogClick;
  roundProcs->key = Nlm_NormalKey;
  roundProcs->draw = Nlm_NormalDraw;
  roundProcs->idle = Nlm_NormalIdle;
#endif
  roundProcs->show = Nlm_ShowNormal;
  roundProcs->hide = Nlm_HideWindow;
  roundProcs->activate = Nlm_NormalActivate;
  roundProcs->deactivate = Nlm_NormalDeactivate;
  roundProcs->remove = Nlm_RemoveWindow;
  roundProcs->select = Nlm_NormalSelect;
  roundProcs->linkIn = Nlm_LinkIn;
  roundProcs->adjustPrnt = Nlm_AdjustWindow;
  roundProcs->setTitle = Nlm_SetWindowTitle;
  roundProcs->getTitle = Nlm_GetWindowTitle;
  roundProcs->setPosition = Nlm_SetWindowPosition;
  roundProcs->getPosition = Nlm_GetWindowPosition;
  roundProcs->gainFocus = Nlm_WindowGainFocus;
  roundProcs->loseFocus = Nlm_WindowLoseFocus;
  roundProcs->sendChar = Nlm_NormalDrawChar;

  alertProcs = &(gphprcsptr [4]);
#ifdef WIN_MAC
  alertProcs->click = Nlm_PlainClick;
  alertProcs->key = Nlm_NormalKey;
  alertProcs->draw = Nlm_NormalDraw;
  alertProcs->idle = Nlm_NormalIdle;
#endif
  alertProcs->show = Nlm_ShowNormal;
  alertProcs->hide = Nlm_HideWindow;
  alertProcs->activate = Nlm_NormalActivate;
  alertProcs->deactivate = Nlm_NormalDeactivate;
  alertProcs->remove = Nlm_RemoveWindow;
  alertProcs->select = Nlm_PlainSelect;
  alertProcs->linkIn = Nlm_LinkIn;
  alertProcs->adjustPrnt = Nlm_AdjustWindow;
  alertProcs->setTitle = Nlm_SetWindowTitle;
  alertProcs->getTitle = Nlm_GetWindowTitle;
  alertProcs->setPosition = Nlm_SetWindowPosition;
  alertProcs->getPosition = Nlm_GetWindowPosition;
  alertProcs->gainFocus = Nlm_WindowGainFocus;
  alertProcs->loseFocus = Nlm_WindowLoseFocus;
  alertProcs->sendChar = Nlm_NormalDrawChar;

  modalProcs = &(gphprcsptr [5]);
#ifdef WIN_MAC
  modalProcs->click = Nlm_ModalClick;
  modalProcs->key = Nlm_ModalKey;
  modalProcs->draw = Nlm_NormalDraw;
  modalProcs->idle = Nlm_NormalIdle;
#endif
  modalProcs->show = Nlm_ShowModal;
  modalProcs->hide = Nlm_HideModal;
  modalProcs->activate = Nlm_NormalActivate;
  modalProcs->deactivate = Nlm_NormalDeactivate;
  modalProcs->remove = Nlm_RemoveWindow;
  modalProcs->select = Nlm_PlainSelect;
  modalProcs->linkIn = Nlm_LinkIn;
  modalProcs->adjustPrnt = Nlm_AdjustWindow;
  modalProcs->setTitle = Nlm_SetWindowTitle;
  modalProcs->getTitle = Nlm_GetWindowTitle;
  modalProcs->setPosition = Nlm_SetWindowPosition;
  modalProcs->getPosition = Nlm_GetWindowPosition;
  modalProcs->gainFocus = Nlm_WindowGainFocus;
  modalProcs->loseFocus = Nlm_WindowLoseFocus;
  modalProcs->sendChar = Nlm_NormalDrawChar;

  movableModalProcs = &(gphprcsptr [6]);
#ifdef WIN_MAC
  movableModalProcs->click = Nlm_MovableModalClick;
  movableModalProcs->key = Nlm_ModalKey;
  movableModalProcs->draw = Nlm_NormalDraw;
  movableModalProcs->idle = Nlm_NormalIdle;
#endif
  movableModalProcs->show = Nlm_ShowModal;
  movableModalProcs->hide = Nlm_HideModal;
  movableModalProcs->activate = Nlm_NormalActivate;
  movableModalProcs->deactivate = Nlm_NormalDeactivate;
  movableModalProcs->remove = Nlm_RemoveWindow;
  movableModalProcs->select = Nlm_PlainSelect;
  movableModalProcs->linkIn = Nlm_LinkIn;
  movableModalProcs->adjustPrnt = Nlm_AdjustWindow;
  movableModalProcs->setTitle = Nlm_SetWindowTitle;
  movableModalProcs->getTitle = Nlm_GetWindowTitle;
  movableModalProcs->setPosition = Nlm_SetWindowPosition;
  movableModalProcs->getPosition = Nlm_GetWindowPosition;
  movableModalProcs->gainFocus = Nlm_WindowGainFocus;
  movableModalProcs->loseFocus = Nlm_WindowLoseFocus;
  movableModalProcs->sendChar = Nlm_NormalDrawChar;

  floatingProcs = &(gphprcsptr [7]);
#ifdef WIN_MAC
  floatingProcs->click = Nlm_FloatingClick;
  floatingProcs->key = Nlm_FloatingKey;
  floatingProcs->draw = Nlm_NormalDraw;
  floatingProcs->idle = Nlm_FloatingIdle;
#endif
  floatingProcs->show = Nlm_ShowNormal;
  floatingProcs->hide = Nlm_HideWindow;
  floatingProcs->activate = Nlm_NormalActivate;
  floatingProcs->deactivate = Nlm_NormalDeactivate;
  floatingProcs->remove = Nlm_RemoveWindow;
  floatingProcs->select = Nlm_FloatingSelect;
  floatingProcs->linkIn = Nlm_LinkIn;
  floatingProcs->adjustPrnt = Nlm_AdjustWindow;
  floatingProcs->setTitle = Nlm_SetWindowTitle;
  floatingProcs->getTitle = Nlm_GetWindowTitle;
  floatingProcs->setPosition = Nlm_SetWindowPosition;
  floatingProcs->getPosition = Nlm_GetWindowPosition;
  floatingProcs->gainFocus = Nlm_WindowGainFocus;
  floatingProcs->loseFocus = Nlm_WindowLoseFocus;
  floatingProcs->sendChar = Nlm_FloatingDrawChar;

  shadowProcs = &(gphprcsptr [8]);
#ifdef WIN_MAC
  shadowProcs->click = Nlm_PlainClick;
  shadowProcs->key = Nlm_NormalKey;
  shadowProcs->draw = Nlm_NormalDraw;
  shadowProcs->idle = Nlm_NormalIdle;
#endif
  shadowProcs->show = Nlm_ShowNormal;
  shadowProcs->hide = Nlm_HideWindow;
  shadowProcs->activate = Nlm_NormalActivate;
  shadowProcs->deactivate = Nlm_NormalDeactivate;
  shadowProcs->remove = Nlm_RemoveWindow;
  shadowProcs->select = Nlm_PlainSelect;
  shadowProcs->linkIn = Nlm_LinkIn;
  shadowProcs->adjustPrnt = Nlm_AdjustWindow;
  shadowProcs->setTitle = Nlm_SetWindowTitle;
  shadowProcs->getTitle = Nlm_GetWindowTitle;
  shadowProcs->setPosition = Nlm_SetWindowPosition;
  shadowProcs->getPosition = Nlm_GetWindowPosition;
  shadowProcs->gainFocus = Nlm_WindowGainFocus;
  shadowProcs->loseFocus = Nlm_WindowLoseFocus;
  shadowProcs->sendChar = Nlm_NormalDrawChar;

  plainProcs = &(gphprcsptr [9]);
#ifdef WIN_MAC
  plainProcs->click = Nlm_PlainClick;
  plainProcs->key = Nlm_NormalKey;
  plainProcs->draw = Nlm_NormalDraw;
  plainProcs->idle = Nlm_NormalIdle;
#endif
  plainProcs->show = Nlm_ShowNormal;
  plainProcs->hide = Nlm_HideWindow;
  plainProcs->activate = Nlm_NormalActivate;
  plainProcs->deactivate = Nlm_NormalDeactivate;
  plainProcs->remove = Nlm_RemoveWindow;
  plainProcs->select = Nlm_PlainSelect;
  plainProcs->linkIn = Nlm_LinkIn;
  plainProcs->adjustPrnt = Nlm_AdjustWindow;
  plainProcs->setTitle = Nlm_SetWindowTitle;
  plainProcs->getTitle = Nlm_GetWindowTitle;
  plainProcs->setPosition = Nlm_SetWindowPosition;
  plainProcs->getPosition = Nlm_GetWindowPosition;
  plainProcs->gainFocus = Nlm_WindowGainFocus;
  plainProcs->loseFocus = Nlm_WindowLoseFocus;
  plainProcs->sendChar = Nlm_NormalDrawChar;

  desktopProcs = &(gphprcsptr [10]);
#ifdef WIN_MAC
  desktopProcs->click = Nlm_DesktopClick;
  desktopProcs->key = Nlm_DesktopKey;
#endif
  desktopProcs->show = Nlm_ShowNormal;
  desktopProcs->hide = Nlm_HideWindow;
  desktopProcs->remove = Nlm_RemoveWindow;
  desktopProcs->select = Nlm_DesktopSelect;
  desktopProcs->linkIn = Nlm_LinkIn;

  systemProcs = &(gphprcsptr [11]);
#ifdef WIN_MAC
  systemProcs->click = Nlm_SystemClick;
  systemProcs->draw = Nlm_NormalDraw;
#endif
  systemProcs->show = Nlm_ShowNormal;
  systemProcs->hide = Nlm_HideWindow;
  systemProcs->remove = Nlm_RemoveWindow;
  systemProcs->select = Nlm_NormalSelect;
  systemProcs->linkIn = Nlm_LinkIn;
}

