/*  $Id: cn3dwin.c,v 6.162 2001/05/25 19:16:15 vakatov Exp $
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
* Author:  Christopher Hogue, Yanli Wang, Lewis Geer, Jonathan Kans
*
* First Version Creation Date:   1/31/96
*
* File Description:  Cn3D GUI API
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cn3dwin.c,v $
* Revision 6.162  2001/05/25 19:16:15  vakatov
* Nested comment typo fixed
*
* Revision 6.161  2001/04/27 20:08:07  juran
* Heed warnings.
*
* Revision 6.160  2000/08/17 16:09:53  thiessen
* fix X/OpenGL initialization bug
*
* Revision 6.159  2000/05/17 22:27:36  lewisg
* rename viewer controls
*
* Revision 6.158  2000/05/16 17:38:43  thiessen
* do glGenLists after context init on X11 - for Mesa 3.2
*
* Revision 6.157  2000/05/09 19:51:01  lewisg
* add new blast header to file>properties
*
* Revision 6.156  2000/04/27 22:21:57  lewisg
* misc bugs/features
*
* Revision 6.155  2000/04/25 00:22:35  thiessen
* save quality settings in config
*
* Revision 6.154  2000/04/20 23:27:45  lewisg
* misc bug fixes
*
* Revision 6.153  2000/04/19 17:56:48  thiessen
* added background color in OpenGL
*
* Revision 6.152  2000/04/17 21:46:56  lewisg
* do not do double index on viewmgr update, rename menus
*
* Revision 6.151  2000/04/10 21:41:26  lewisg
* move alignment menus into ddv, udv from cn3d
*
* Revision 6.150  2000/04/04 22:18:43  lewisg
* add defline to ddv, fix seq import bugs, set boundbox
*
* Revision 6.149  2000/03/27 22:15:05  lewisg
* add show/hide row dialog
*
* Revision 6.148  2000/03/24 20:34:55  lewisg
* add blast from file, bug fixes, get rid of redundant code, etc.
*
* Revision 6.147  2000/03/24 19:59:20  thiessen
* draw new logo in OpenGL
*
* Revision 6.146  2000/03/23 14:53:04  thiessen
* fix bug in viewer3d camera; added conservation color algorithm to ARS
*
* Revision 6.145  2000/03/18 00:06:00  lewisg
* add blast, new help, new menus
*
* Revision 6.144  2000/03/17 22:49:00  thiessen
* fix for multi-chain / multi-model features ; added feature-move ; misc bug fixes
*
* Revision 6.143  2000/03/15 18:49:07  thiessen
* fixed viewer3d color bug
*
* Revision 6.142  2000/03/15 16:59:55  thiessen
* fix highlighting, other minor bugs
*
* Revision 6.141  2000/03/15 04:43:56  thiessen
* various minor fixes
*
* Revision 6.140  2000/03/10 18:46:59  lewisg
* add show/hide
*
* Revision 6.139  2000/03/09 18:58:55  thiessen
* fixed typo
*
* Revision 6.138  2000/03/09 17:56:58  thiessen
* changes to palette handling, feature implementation, PARS storage
*
* Revision 6.137  2000/03/08 21:46:15  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.136  2000/03/06 15:33:53  lewisg
* add new hydrophobic palette, update visual c++ project to move seqcons
*
* Revision 6.135  2000/03/03 20:05:17  thiessen
* removal of palette-building pass if in 24-bit color
*
* Revision 6.134  2000/03/01 16:17:55  thiessen
* improved handling of colors; many small fixes
*
* Revision 6.133  2000/02/26 00:01:41  thiessen
* OpenGL improvements, progress on cleanup of Show/Hide
*
* Revision 6.132  2000/02/22 17:10:00  thiessen
* fix typecast error
*
* Revision 6.131  2000/02/19 21:25:57  thiessen
* split of cn3dmodl into cn3dmodl and cn3dstyl
*
* Revision 6.130  2000/02/19 01:23:59  lewisg
* use ibm, add row tracking code, various bug fixes
*
* Revision 6.129  2000/02/17 15:41:11  thiessen
* added CSC algorithm selection submenu
*
* Revision 6.128  2000/02/10 19:04:25  thiessen
* added checkbox to change altConf behaviour
*
* Revision 6.127  2000/02/10 17:47:02  thiessen
* added: color-by-sequence-conservation menu item, zoom-out to OpenGL, misc fixes
*
* Revision 6.126  2000/02/10 15:51:59  lewisg
* cn3d responds and send correct update messages.  many coloring bug fixes
*
* Revision 6.125  2000/01/18 22:49:16  lewisg
* send OM_MSG_FLUSH to ddv/udv, tweak CPK coloration, misc bugs
*
* Revision 6.124  2000/01/14 21:40:41  lewisg
* add translucent spheres, ion labels, new cpk, fix misc bugs
*
* Revision 6.123  2000/01/11 01:16:47  lewisg
* fix color selection in Cn3D, other misc. bugs
*
* Revision 6.122  2000/01/06 00:04:43  lewisg
* selection bug fixes, update message outbound, animation APIs moved to vibrant
*
* Revision 6.121  2000/01/04 15:55:51  lewisg
* don't hang on disconnected network and fix memory leak/hang at exit
*
* Revision 6.120  1999/12/28 15:08:44  lewisg
* remove remaining mediainfo code
*
* Revision 6.119  1999/12/23 22:22:53  lewisg
* fix Cn3D_Align undefined for non-opengl build
*
* Revision 6.118  1999/12/23 21:40:33  lewisg
* move animation controls to dialog
*
* Revision 6.117  1999/12/13 23:20:44  lewisg
* bug fixes: duplicate color structures eliminated, clear color doesn't clear case
*
* Revision 6.116  1999/12/11 01:30:35  lewisg
* fix bugs with sharing colors between ddv and cn3d
*
* Revision 6.115  1999/12/08 22:58:01  thiessen
* added quality settings for OpenGL rendering
*
* Revision 6.114  1999/12/03 23:17:23  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 6.113  1999/12/01 16:15:54  lewisg
* interim checkin to fix blocking memory leak
*
* Revision 6.112  1999/11/30 22:32:10  vakatov
* Changed "#if _OPENGL" to "#ifdef _OPENGL" to suit Borland 5.0 compiler
*
* Revision 6.111  1999/11/24 15:23:20  lewisg
* added color selection dialogs for SS
*
* Revision 6.110  1999/11/23 19:23:09  thiessen
* change OGL_Redraw to OGL_DrawViewer3D
*
* Revision 6.109  1999/11/22 17:29:54  lewisg
* add back color selection dialogs, fix viewer3d half-selection bug
*
* Revision 6.108  1999/11/22 15:44:05  thiessen
* added image.h header to fix Mac compiler error
*
* Revision 6.107  1999/11/22 14:46:45  thiessen
* moved _OPENGL code blocks to only vibrant and ncbicn3d libraries
*
* Revision 6.105  1999/11/15 18:30:09  lewisg
* get rid of extra redraws when selecting
*
* Revision 6.104  1999/11/10 23:19:42  lewisg
* rewrite of selection code for ddv
*
* Revision 6.103  1999/11/03 16:24:21  lewisg
* get rid of non-existing includes
*
* Revision 6.102  1999/11/02 23:06:08  lewisg
* fix cn3d to launch correctly if there is no seqentry associated with bioseq
*
* Revision 6.101  1999/11/01 22:10:27  lewisg
* add ability to call color functions by type, and add this to cn3d
*
* Revision 6.100  1999/10/29 14:15:31  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.99  1999/10/15 20:56:40  lewisg
* append DDV_ColorGlobal as userdata.  free memory when cn3d terminates.
*
* Revision 6.98  1999/10/08 23:20:35  lewisg
* add case and box attributes to DDV_ColorCell
*
* Revision 6.97  1999/10/05 23:18:25  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.96  1999/09/21 18:09:15  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 6.95  1999/09/21 13:59:22  thiessen
* port of Lewis's OpenGL code to X/Motif
*
* Revision 6.94  1999/09/16 17:16:21  ywang
* open multiple salsa window for data with multiple seq-annot data
*
* Revision 6.93  1999/09/15 14:22:13  ywang
* modify Cn3D_AlignEdit to open multiple salsa window for alignment
*
* Revision 6.92  1999/07/09 20:50:56  ywang
* set highlight color for salsa in Cn3dObjRegiste
*
* Revision 6.91  1999/07/01 21:44:49  ywang
* *** empty log message ***
*
* Revision 6.90  1999/07/01 14:07:25  ywang
* cn3dwin.c
*
* Revision 6.89  1999/06/30 16:51:08  ywang
* comment out 'Salsa_BioseqUpdate = TRUE' in AlignEdit
*
* Revision 6.88  1999/06/15 18:18:16  ywang
* loop over all the existed models for highligt
*
* Revision 6.87  1999/05/27 16:12:22  ywang
* synchronize highlight color change in the cn3d viewer
*
* Revision 6.86  1999/05/04 23:12:47  ywang
* fix selection retaining problem on show/hide
*
* Revision 6.85  1999/04/08 15:08:02  ywang
* disable color by SeqConservation
*
* Revision 6.84  1999/04/06 14:23:30  lewisg
* add opengl replacement for viewer3d
*
* Revision 6.83  1999/03/30 22:36:19  ywang
* add functions to color salsa for NcbiMimeAsn1_strucseqs & code reorganization
*
* Revision 6.82  1999/03/22 22:41:53  ywang
* redesign feature page, fix bugs
*
* Revision 6.81  1999/03/03 23:17:22  lewisg
* one master struct at a time, list slaves in structure info, bug fixes
*
* Revision 6.80  1999/03/02 22:05:58  lewisg
* warn on less than 256 entry palette
*
* Revision 6.79  1999/02/25 23:14:32  ywang
* move around menu item and callback function, change menu item names
*
* Revision 6.78  1999/02/24 23:16:03  ywang
* minor name change
*
* Revision 6.77  1999/02/12 15:11:52  ywang
* send color message to salsa when user changes highlight color from cn3d
*
* Revision 6.76  1999/02/11 22:39:47  ywang
* fix bug on display highlight residues only--if no res are highlighted, cn3d sets that button status as FALSE and draw whole structures
*
* Revision 6.75  1999/02/11 18:48:15  lewisg
* delete color index functions
*
* Revision 6.74  1999/02/10 23:49:43  lewisg
* use RGB values instead of indexed palette
*
* Revision 6.73  1999/01/27 16:23:34  ywang
* return on NULL pdnmsMaster in Cn3D_AlignEdit
*
* Revision 6.72  1999/01/26 17:14:34  ywang
* redesign Display menu and add 'display highlight residues only' function
*
* Revision 6.71  1999/01/20 22:57:23  ywang
* customize color for secondary structure & rearrange Option menu
*
* Revision 6.70  1999/01/20 18:21:20  ywang
* include salmedia.h due to the move around of MediaInfo from cn3dmsg.h to the new created salmedia.h
*
* Revision 6.69  1999/01/14 19:07:17  kans
* network availability is configurable
*
* Revision 6.68  1998/12/16 19:32:19  ywang
* improve highlight residues function when rerendering
*
 * Revision 6.67  1998/11/24  17:01:01  kans
 * put render/label/view/model controls in separate window
 *
* Revision 6.66  1998/11/06 23:01:05  ywang
* fix bugs for modeling
*
 * Revision 6.65  1998/11/04  00:06:24  ywang
 * add function for modeling: change render/color for special residue(s)
 *
 * Revision 6.64  1998/10/30  19:42:38  ywang
 * turn animation on by default
 *
 * Revision 6.63  1998/10/27  15:55:53  ywang
 * add functions for testing color by sequence conservation
 *
 * Revision 6.62  1998/10/21  21:16:56  ywang
 * update highlight RGB in the global application property data structure
 *
 * Revision 6.61  1998/10/21  15:51:27  ywang
 * reset residue color for salsa before cn3d redraws so that residues shown in salsa will become black if they are not shown in cn3d window
 *
 * Revision 6.60  1998/10/16  22:06:09  ywang
 * make global color array for sequence display
 *
 * Revision 6.59  1998/10/07  23:10:46  ywang
 * merge align control with general display control
 *
 * Revision 6.58  1998/10/05  21:24:36  ywang
 * re-arrange menu
 *
 * Revision 6.57  1998/09/30  22:10:48  ywang
 * control display on three levels: structure, chain, domain
 *
 * Revision 6.56  1998/09/23  18:38:50  ywang
 * add functions to control display on domain level
 *
 * Revision 6.55  1998/09/22  17:53:13  ywang
 * add menu for display control
 *
 * Revision 6.54  1998/09/17  20:51:17  ywang
 * add functions for edit background and highlight color
 *
 * Revision 6.53  1998/09/04  19:50:53  ywang
 * reorganize codes for highlighting
 *
 * Revision 6.52  1998/09/01  18:19:15  ywang
 * initialize IteM i = NULL in LaunchSequenceWindow
 *
 * Revision 6.51  1998/08/26  18:28:38  kans
 * fixed -v -fd warnings
 *
* Revision 6.50  1998/08/17 18:45:48  lewisg
* change version to 2.01
*
* Revision 6.49  1998/08/05 19:12:36  ywang
* comment out MK_Shift/MA_DClick, MK_Ctrl/MA_DClick
*
 * Revision 6.48  1998/07/23  14:34:30  chappey
 * resize of salsa window
 *
* Revision 6.47  1998/07/22 20:18:34  lewisg
* resize of salsa window
*
* Revision 6.46  1998/07/16 18:18:39  chappey
* SeqIsIn -> SeqIdForSameBioseq
*
* Revision 6.45  1998/07/13 23:19:57  ywang
* check hidden sequence whenever launching salsa
*
 * Revision 6.44  1998/07/09  21:47:37  ywang
 * reduce color messages for salsa
 *
 * Revision 6.43  1998/06/30  23:29:22  ywang
 * fix bugs regarding to read in more structures
 *
 * Revision 6.42  1998/06/30  20:49:02  ywang
 * improve performance and prepare for close salsa
 *
 * Revision 6.41  1998/06/22  18:58:07  chappey
 * moved GetAppProperty SeqEditDisplayForm to cn3dwin.c
 *
* Revision 6.40  1998/06/17 17:42:25  lewisg
* moved a menu item
*
* Revision 6.39  1998/06/16 18:00:30  lewisg
* moved rendering menus and created a reset presentation menu item
*
* Revision 6.38  1998/06/10 22:04:32  ywang
* remove obsolete code
*
 * Revision 6.37  1998/06/10  22:01:08  ywang
 * remove obsolete code
 *
 * Revision 6.36  1998/06/04  16:33:26  ywang
 * autamatially launch salsa window
 *
 * Revision 6.35  1998/05/28  22:06:00  ywang
 * maintain highlight upon rendering switch when salsa is on
 *
 * Revision 6.33  1998/05/26  22:06:13  ywang
 * salsa get cn3d color when it is launched
 *
 * Revision 6.32  1998/05/26  21:35:21  lewisg
 * added defaults to render menu, got rid of mouse 3D actions menu item
 *
* Revision 6.31  1998/05/18 22:09:14  ywang
* move codes around
*
 * Revision 6.30  1998/05/18  16:45:52  ywang
 * allocate memory for mediadata
 *
 * Revision 6.29  1998/05/14  14:53:42  ywang
 * fix bugs
 *
 * Revision 6.27  1998/05/06  23:50:27  lewisg
 * fixed launching problem with sequin
 *
* Revision 6.26  1998/05/05 20:06:36  ywang
* set yellow as highlight color in cn3d
*
 * Revision 6.25  1998/04/30  15:22:29  ywang
 * start to store Num_ActiveSlave
 *
 * Revision 6.24  1998/04/29  18:03:06  lewisg
 * new menus
 *
* Revision 6.23  1998/04/28 22:47:24  lewisg
* master/slave color in sync
*
* Revision 6.22  1998/04/28 18:51:07  ywang
* slight modification
*
 * Revision 6.21  1998/04/27  17:49:57  lewisg
 * added color by conservation
 *
* Revision 6.20  1998/04/20 16:03:27  ywang
* launch sequence viewer correctly in one biostruc/multiple chain case
*
 * Revision 6.19  1998/04/18  00:33:51  lewisg
 * added ability to turn slaves on/off
 *
* Revision 6.18  1998/04/17 01:08:13  ywang
* call LaunchAlignEditor in one pair alignment case while call LaunchAnnotAlignEditor in multiple alignment case
*
 * Revision 6.16  1998/04/15  19:22:51  chappey
 * Add REGISTER_NEW_SEQANNOT_EDIT to be able to launch multiple alignment from the SeqAnnot
 *
* Revision 6.15  1998/04/14 21:16:56  ywang
* try to pass the head of alignment data link list to salsa
*
 * Revision 6.14  1998/04/09  17:07:23  lewisg
 * added a version # and got rid of File/Vast Alignments
 *
* Revision 6.13  1998/04/03 18:06:24  ywang
* show NA sequence also
*
 * Revision 6.12  1998/04/02  23:01:51  kans
 * code warrior distinguishes 0 from NULL
 *
* Revision 6.11  1998/04/02 22:27:18  ywang
* multiple sequence viewer for multiple chain protein
*
 * Revision 6.9  1998/04/01  01:26:19  ywang
 * sequence view and get blast alignment-Colombe
 *
* Revision 6.8  1998/03/30 23:32:28  ywang
* Set Hightlight Color as Red instead of Blue; Associate simple double click action with one residue highlight
*
 * Revision 6.7  1998/03/30  16:02:11  kans
 * changed printf to ErrPostEx
 *
* Revision 6.6  1998/03/27 23:17:38  ywang
* Get psaAlignment from pmsdMaster
*
 * Revision 6.5  1998/03/26  22:27:42  kans
 * fixed CodeWarrior complaints and missing prototypes
 *
* Revision 6.4  1998/03/26 20:44:20  ywang
* start cn3d messager
*
 * Revision 6.3  1998/03/07  20:43:53  kans
 * moved Cn3D_fEntrezOn to cn3dwin.c
 *
* Revision 6.2  1998/03/06 19:03:00  kans
* needed to add two includes
*
* Revision 6.1  1998/03/06 01:22:59  lewisg
* merge
*
* Revision 6.0  1997/08/25 18:13:48  madden
* Revision changed to 6.0
*
* Revision 1.4  1997/07/29 21:17:11  vakatov
* [WIN32,DLL]  Made Cn3D's stubbed functions be DLL-exportable
*
* Revision 1.3  1997/03/31 16:53:14  vakatov
* Use Z-rotation scrollbar in the 3D-viewer controls.
* Changed the CN3D release from 1.0 to 1.1.
*
 * Revision 1.2  1997/03/20  21:21:12  vakatov
 * [WIN_MAC] Cn3DResizeProc():  take into account the window menubar height
 *
 * Revision 1.1  1997/03/20  16:24:36  vakatov
 * Initial revision
 *
 * Revision 5.18  1996/08/19  21:05:26  vakatov
 * Made functions of type "pNodeFunc" to be of that type(sharp)
 * (and thus fixed fatal bug under Borland/WIN32); removed all
 * castings to (pNodeFunc) -- to let compiler to check for such bugs
 *
 * Revision 5.17  1996/07/31  18:36:32  hogue
 * Segment highlighting code added, MIME-type code from Network Entrez
 * adapted for STANDALONE Cn3D.
 *
 * Revision 5.16  1996/07/29  21:12:44  epstein
 * add logic to hide query window when cn3d MIME viewer starts up
 *
 * Revision 5.15  1996/07/26  18:57:41  kans
 * hide and update, then show and update
 *
 * Revision 5.14  1996/07/22  00:26:37  hogue
 * Default 3D viewer made smaller, Help menu refers to WWW-site,
 * Quit Entrez menu item, and About Structure menu item which
 * displays Structure Summary + PDB remarks.  Also trapped
 * no-primitives condition.
 *
 * Revision 5.13  1996/06/14  14:54:19  vakatov
 * [WIN_MOTIF]  RestrictMotifColorsTo( 32 ) call added before creating
 * 3D-Viewer shell window -- to provide proper Motif/3D-V color sharing
 *
 * Revision 5.12  1996/06/13  21:26:45  kans
 * fixed mac resize
 *
 * Revision 5.11  1996/06/13  21:05:31  kans
 * fixed mac-specific typos
 *
 * Revision 5.10  1996/06/13  20:49:10  hogue
 * Removed Beta designation .
 *
 * Revision 5.9  1996/06/13  20:44:17  hogue
 * Hides window now when starting up in Entrez, fix for Mac window
 * resize problem.
 *
 * Revision 5.8  1996/06/13  16:36:42  kans
 * menus now in the window for all platforms, including Mac
 *
 * Revision 5.7  1996/06/12  14:31:33  hogue
 * Added Cn3DWin function for integration into Entrez
 *
 * Revision 5.6  1996/06/05  16:28:45  hogue
 * Fixed a gif file name save bug.
 *
 * Revision 5.5  1996/06/03  22:36:08  hogue
 * Rearragned top menus for ease of use & cosmetic reasons.
 *
 * Revision 5.4  1996/06/03  21:20:09  hogue
 * Changed FixedWindow to DocumentWindwo for Win/Mac resize-ability,
 * Added call to MyQuit to stop playing layers if was on.  Also
 * added an initial call to the resizer to clean up sizing problems
 * on startup.
 *
 * Revision 5.3  1996/06/03  20:00:57  kans
 * command item was missing a closing double quo;te
 *
 * Revision 5.2  1996/06/03  19:43:52  vakatov
 * Quit the program more careful
 *
 * Revision 5.1  1996/05/29  19:16:02  vakatov
 * Cn3DResizeProc():  LinkAnimCtrls3D() call added to relink 3D-controls
 * to the new 3D-viewer
 *
 * Revision 5.0  1996/05/28  14:05:44  ostell
 * Set to revision 5.0
 *
 * Revision 1.17  1996/05/23  14:33:49  hogue
 * Added Image menu with Zoom, Move, All, Save fn;
 * Added MS-Win Edit menu (Copy only) and Print fn on Image menu
 * Added call to stop animation playing for redraw.
 *
 * Revision 1.16  1996/05/22  21:23:54  hogue
 * Added Watch/Arrow cursor calls, ProgMon.
 *
 * Revision 1.15  1996/05/21  22:50:10  hogue
 * Capture camera of current structure before resize.
 *
 * Revision 1.14  1996/05/21  22:18:49  hogue
 * Added call to ResetLabelCtrls
 *
 * Revision 1.13  1996/05/21  22:12:28  vakatov
 * "Cn3DResizeProc()" rewritten and it is now able to count the menu-bar height
 * "Viewer3DGroups()" modified to better control over the groups positioning
 *
 * Revision 1.12  1996/05/14  15:45:16  hogue
 * Added UpdateColorTable call & changed palette structure accordingly,
 * Added Cn3dResizeProc and set up the Label,
 * Render & Viewer Controls and the 3D window to move around as planned-
 *  which uncovers new bugs in Viewer3d code...
 *
 * Revision 1.11  1996/05/09  15:41:10  hogue
 * Domain rendering enabled.
 *
 * Revision 1.10  1996/05/07  18:30:58  vakatov
 * Viewer3DGroups() -- slightly changed;  + casting...
 *
 * Revision 1.9  1996/04/26  21:44:21  vakatov
 * Tune the 3D-viewer size to fit the screen
 *
 * Revision 1.8  1996/04/26  18:42:24  vakatov
 * CN3D sources ported to MS-Windows;
 * the portability errors and warnings fixed, etc.
 *
 * Revision 1.7  1996/04/18  16:57:01  hogue
 * Altered color palette for multi-structure display, preparing for neighbors...
 *
 * Revision 1.6  1996/04/04  21:05:21  hogue
 * rearranged menus, fixed camera calls, added NCBI logo
 *
 * Revision 1.5  1996/03/30  23:40:19  hogue
 * Redraw now saves camera
 *
 * Revision 1.4  1996/03/29  20:00:06  hogue
 * Integrated 3d viewing, menus & controls for algorithmic rendering
 *
 * Revision 1.2  1996/02/02  19:39:32  hogue
 * Initial Revision
 *
*
* ==========================================================================
*/

#include <ncbi.h>
#include <accentr.h>
#include <cn3dmain.h>
#include <cn3dopen.h>
#include <cn3dxprt.h>
#include <cn3dsave.h>
#include <algorend.h>
#include <objalign.h>
#include <objseq.h>
#include <objmgr.h>
#include <saledit.h>
#include <lsqfetch.h>
#include <salutil.h>
#include <cn3dmsg.h>
#include <salmedia.h>
#include <cn3dshim.h>
#include <diagnost.h>
#include <cn3dmesh.h>
#include <udviewer.h>
#include <vsm.h>
#include <image.h>
#include <seqcons.h>
#include <cn3dmodl.h>
#include <cn3dstyl.h>
#include <bspview.h>
#ifdef _OPENGL
#include <shim3d.h>
#endif

static Uint2 Cn3D_Vy, Cn3D_Rx;
static MenU Cn3D_sOpen;
static IteM Cn3D_iOpen;
static IteM Cn3D_iSelStruc;
static IteM Cn3D_iClearStruc;
static IteM Cn3D_iRendCtrl = NULL;
static IteM Cn3D_iLabelCtrl = NULL;
static IteM Cn3D_iAlignCtrl = NULL;
static IteM Cn3D_iDisplayCtrl = NULL; /* For display control, Yanli */
#ifndef _OPENGL
static IteM Cn3D_iViewCtrl = NULL;
#endif
static IteM Cn3D_iModelCtrl = NULL;
static IteM Cn3D_sSave = NULL;
static MenU Cn3D_sExport;
static WindoW Cn3D_w = NULL;
static MenU Cn3D_ma_group_menu;
static MenU Cn3D_ma_action_menu;
/*static MenU     Cn3D_sNaybor;*/
static MenU Cn3D_mRender;
static MenU Cn3D_mColor;
static MenU Cn3D_mControls;
static WindoW Cn3D_wCtrls = NULL;

static Uint1 errNum;
static Int1 errType;
static CharPtr errMsg;

#ifndef _OPENGL
Viewer3D Cn3D_v3d = NULL;
static Picture3D Cn3D_pMain;
static Camera3D Cn3D_Camera3D;
#endif
TCn3D_ColorData Cn3D_ColorData;


static GrouP Cn3D_gWinGP;
static GrouP Cn3D_gViewer;
static GrouP Cn3D_gRendCtrl;
static GrouP Cn3D_gLabelCtrl;
static GrouP Cn3D_gAlignCtrl;
static GrouP Cn3D_gDisplayCtrl; /* For display control, Yanli  */
static GrouP Cn3D_gModelCtrl;   /* For modeling, Yanli  */
static GrouP Cn3D_gViewCtrl;
#ifdef _OPENGL
/*static GrouP Cn3D_gOGLCtrl;*/     /* group that controls 3D view, including animation */
static GrouP Cn3D_gOGLQuality;  /* group that controls rendering quality */
#endif                          /* _OPENGL */

#ifndef _OPENGL
static Nlm_Controls3D Cn3D_left;
#endif

ValNodePtr Cn3d_ColorNames = NULL; /* choice holds table number */

/* the default palette */
Uint1 Cn3d_PaletteRGB[CN3D_COLOR_MAX][3] =
{
  255, 255, 255, /* default     0 */
  255,  20, 147, /* hotpink     1 */
  255,   0, 255, /* magenta     2 */
  155,	48, 255, /* purple      3 */
    0,   0, 255, /* blue        4 */
   30, 144, 255, /* sky         5 */
    0, 255, 255, /* cyan        6 */
    0, 255, 127, /* sea         7 */
    0, 255,   0, /* green       8 */
  255, 255,   0, /* yellow      9 */
  255, 165,   0, /* gold       10 */
  255,  69,   0, /* orange     11 */
  255,   0,   0, /* red        12 */
  255, 114,  86, /* pink       13 */
  255, 174, 185, /* pinktint   14 */
  255, 255, 255, /* white      15 */
    0,   0,   0, /* black      16 */
  176, 226, 255, /* bluetint   17 */
  154, 255, 154, /* greentint  18 */
  255, 236, 139, /* yellowtint 19 */
  125, 125, 125, /* gray       20 */
  139,  87,  66, /* brown      21 */
  255,   0,   0, /* perm colors 22: red */
    0, 255,   0, /* perm colors 23: green */
  255,   0, 255, /* perm colors 24: magenta */
   30, 144, 255, /* perm colors 25: sky */
  155,	48, 255, /* perm colors 26: purple */
    0, 255,   0, /* SS colors 27: helix, green */
  255, 165,   0, /* SS colors 28: strand, gold */
  255,  69,   0, /* SS colors 29: turn, orange */
    0, 255, 255, /* SS colors 30: coil, cyan */
  255, 255,   0, /* highlight colors 31: yellow */
    0,   0,   0, /* background colors 32: black */
  160,  82,  45, /* sienna 33 */
  240, 230, 140, /* khaki 34 */
  171, 130, 255, /* light purple 35 */
  255, 200,   0, /* orange yellow 36 */
  117, 128, 160, /* 37 */
    5,  53, 239, /* 38 */
   27,  62, 255, /* 39 */
   84, 110, 203, /* 40 */
  216,  77,  84, /* 41 */
  125, 128, 135, /* 42 */
  237,  29,  61, /* 43 */
  184, 118, 120, /* 44 */
  155,   9,   0, /* 45 */
  155, 119, 107, /* 46 */
   80, 110, 196, /* 47 */
  122, 127, 157, /* 48 */
   50,  85, 241, /* 49 */
  201,  93,  91, /* 50 */
  254,   0,   5, /* 51 */
  155, 137, 133, /* 52 */
    3,  53, 246  /* 53 */
};






extern Boolean LIBCALL readErrors(void)
{
    Uint1 i;

    errNum = DiagGetRecordCount();
    for (i = 0; i < errNum; i++) {
        errType = DiagGetRecordType(i);
        errMsg = DiagGetRecordStr(i);
        Message(MSG_OK, "%d %d %s", errNum, errType, errMsg);
    }

    return DiagHasErrorRec();
}


void LIBCALL Cn3D_EnableFileOps(void)
{
    if (GetFirstModelstruc() == NULL) { /* nothing in memory - disable stuff */
        Cn3D_DisableFileOps();
        /* just leave the "opener enabled" */
        Enable(Cn3D_sOpen);
        Enable(Cn3D_iOpen);
    } else {
        Enable(Cn3D_sOpen);
        Enable(Cn3D_iOpen);
        Enable(Cn3D_sSave);
        Enable(Cn3D_sExport);
        Enable(Cn3D_iSelStruc);
        Enable(Cn3D_iClearStruc);
    }
    return;
}

void LIBCALL Cn3D_DisableFileOps(void)
{
    Disable(Cn3D_sOpen);
    Disable(Cn3D_iOpen);
    Disable(Cn3D_sSave);
    Disable(Cn3D_sExport);
    Disable(Cn3D_iSelStruc);
    Disable(Cn3D_iClearStruc);
    return;
}


void LIBCALL Cn3D_DisableMenus(void)
{
    Cn3D_DisableFileOps();
    Disable(Cn3D_mRender);
    Disable(Cn3D_mColor);
    Disable(Cn3D_mControls);
}


void LIBCALL Cn3D_EnableMenus(void)
{
    Cn3D_EnableFileOps();
    Enable(Cn3D_mRender);
    Enable(Cn3D_mColor);
    Enable(Cn3D_mControls);
}


static void Cn3D_AboutProc(IteM i)
{
    MsgAlert(KEY_OK, SEV_INFO, "About Cn3D",
             "Cn3D\n\nA 3-D Viewer for\nNCBI Databases\nVersion 3.0\n\nThe National Center for Biotechnology Information\nhttp://www.ncbi.nlm.nih.gov\ninfo@ncbi.nlm.nih.gov");
}


static void Cn3D_HelpProcOK(ButtoN g)
{
    WindoW hOpenDlg;

    hOpenDlg=(WindoW)ParentWindow(g);
    if (!hOpenDlg) return;
    Remove(hOpenDlg);

    Nlm_LaunchWebPage("http://www.ncbi.nlm.nih.gov/Structure/CN3D/cn3dhelp.html");
}

static void Cn3D_HelpProcCancel(ButtoN g)
{
    WindoW hOpenDlg;

    hOpenDlg=(WindoW)ParentWindow(g);
    if (!hOpenDlg) return;
    Remove(hOpenDlg);
}

static void Cn3D_HelpProc(IteM i)
{
    WindoW d;
    GrouP h, h1, h2;
    
    d = MovableModalWindow(-50, -33 ,-10, -10, "Help", NULL);
    if (d != NULL){
        h = HiddenGroup(d, 1, 2,  NULL);

        h1 = HiddenGroup(h, 1, 5,  NULL);
        StaticPrompt(h1,"An WWW Manual for Cn3D can be viewed at:",0,0,systemFont,'l');
        StaticPrompt(h1,"http://www.ncbi.nlm.nih.gov/Structure/CN3D/cn3dhelp.html",0,0,systemFont,'l');
        StaticPrompt(h1,"Send questions to:",0,0,systemFont,'l');
        StaticPrompt(h1,"info@ncbi.nlm.nih.gov",0,0,systemFont,'l');
        StaticPrompt(h1,"",0,0,systemFont,'l');

        h2 = HiddenGroup(h, 2, 0,  NULL);
        PushButton(h2, "Launch", Cn3D_HelpProcOK);
        PushButton(h2, "Cancel", Cn3D_HelpProcCancel);
        RealizeWindow(d);
        Show(d);
    }
}


static void Cn3D_AllCB(IteM i)
{
#ifdef _OPENGL
    OGL_Reset(Cn3D_ColorData.OGL_Data);
    if (!GetSelectedModelstruc())
        OGL_DrawLogo(Cn3D_ColorData.OGL_Data);
    OGL_DrawViewer3D(Cn3D_ColorData.OGL_Data);
#else
    ZoomAll3D(Cn3D_v3d);
#endif
}

static void Cn3D_ZoomCB(IteM i)
{
    MsgAlert(KEY_OK, SEV_INFO, "Cn3D Online Manual",
             "Use the [Control] modifier key to zoom with mouse\n\n\
(see also Image/Mouse3D_Groups)");
}

static void Cn3D_MoveCB(IteM i)
{
    MsgAlert(KEY_OK, SEV_INFO, "Cn3D Online Manual",
             "Use the [Shift] modifier key to move with mouse\n\n\
(see also Image/Mouse3D_Groups)");
}

static void Cn3D_Default_BgColor(IteM i)
{
#ifdef _OPENGL
    Cn3D_ColorData.OGL_Data->Background.rgb[0] = 0;
    Cn3D_ColorData.OGL_Data->Background.rgb[1] = 0;
    Cn3D_ColorData.OGL_Data->Background.rgb[2] = 0;
    if (Cn3D_ColorData.OGL_Data->IndexMode)
        Cn3D_Redraw(FALSE);     /* new palette */
    else
        OGL_DrawViewer3D(Cn3D_ColorData.OGL_Data);
#else                           /* _OPENGL */
    if (Cn3D_v3d != NULL)
        Nlm_SetBackground3D(Cn3D_v3d, 0, 0, 0);
#endif

}

static void Cn3D_BgColor(IteM i)
{

#ifdef _OPENGL
/* not currently used in OpenGL... done in Render panel instead */
    DDV_ColorCell *pColorCell;

    pColorCell =
        DDV_SearchColorCellbyName(Cn3D_ColorData.pDDVColorGlobal->
                                  pvnSpecialColors, "Background");
    if (pColorCell == NULL) return;
    ChooseColorDialog(&(Cn3D_ColorData.OGL_Data->Background.rgb[0]),
                      &(Cn3D_ColorData.OGL_Data->Background.rgb[1]),
                      &(Cn3D_ColorData.OGL_Data->Background.rgb[2]), TRUE);
    DDV_SetPaletteColor(Cn3D_ColorData.pDDVColorGlobal, "Background", "CN3D",
        pColorCell->rgb[0], pColorCell->rgb[1], pColorCell->rgb[2]);

        Cn3D_Redraw(FALSE);     /* new palette */
#else                           /* _OPENGL */
    if (Cn3D_v3d != NULL)
        BgColorDlg3D(Cn3D_v3d);
#endif

}


static void Cn3D_Default_HLColor(IteM i)
{

    Uint1 colorR, colorG, colorB;

    colorR = 255;
    colorG = 255;
    colorB = 0;
    DDV_SetPaletteColor(Cn3D_ColorData.pDDVColorGlobal, "Highlight", "CN3D",
        255, 255, 0);

#ifndef _OPENGL
    SetHLColor3D(Cn3D_v3d, colorR, colorG, colorB);
#endif /* ndef _OPENGL */
    Cn3D_RedrawNUpdate(FALSE);

}

static void Cn3D_HLColor(IteM i)
{
    DDV_ColorCell *pColorCell;

    pColorCell =
        DDV_SearchColorCellbyName(Cn3D_ColorData.pDDVColorGlobal->
                                  pvnSpecialColors, "Highlight");
    if (pColorCell == NULL)
        return;

    ChooseColorDialog(&pColorCell->rgb[0], &pColorCell->rgb[1],
                      &pColorCell->rgb[2], TRUE);
    DDV_SetPaletteColor(Cn3D_ColorData.pDDVColorGlobal, "Highlight", "CN3D",
        pColorCell->rgb[0], pColorCell->rgb[1], pColorCell->rgb[2]);

#ifndef _OPENGL
    SetHLColor3D(Cn3D_v3d, pColorCell->rgb[0], pColorCell->rgb[1],
                 pColorCell->rgb[2]);
#endif /* ndef _OPENGL */
    Cn3D_RedrawNUpdate(FALSE);
}

static void Cn3D_Default_SSColor(IteM i)
{
#ifdef _OPENGL
    DDV_DefaultSSColor(Cn3D_ColorData.pDDVColorGlobal, "CN3D");
#else  /* _OPENGL */
    DDV_SetPaletteColor(Cn3D_ColorData.pDDVColorGlobal, "Helix", "CN3D", 0, 255, 0);
    DDV_SetPaletteColor(Cn3D_ColorData.pDDVColorGlobal, "Strand", "CN3D", 255, 165, 0);
    DDV_SetPaletteColor(Cn3D_ColorData.pDDVColorGlobal, "Turn", "CN3D", 255, 69, 0);
    DDV_SetPaletteColor(Cn3D_ColorData.pDDVColorGlobal, "Coil", "CN3D", 0, 255, 255);
#endif  /* else _OPENGL */
    Cn3D_RedrawNUpdate(FALSE);
}

static void Cn3D_HelixColor(IteM i)
{
    DDV_ColorCell *pColorCell;

    pColorCell =
        DDV_SearchColorCellbyName(Cn3D_ColorData.pDDVColorGlobal->
                                  pvnSpecialColors, "Helix");
    if (pColorCell == NULL)
        return;

    ChooseColorDialog(&pColorCell->rgb[0], &pColorCell->rgb[1],
                      &pColorCell->rgb[2], TRUE);
    DDV_SetPaletteColor(Cn3D_ColorData.pDDVColorGlobal, "Helix", "CN3D",
        pColorCell->rgb[0], pColorCell->rgb[1], pColorCell->rgb[2]);

    Cn3D_RedrawNUpdate(FALSE);
}

static void Cn3D_StrandColor(IteM i)
{
    DDV_ColorCell *pColorCell;

    pColorCell =
        DDV_SearchColorCellbyName(Cn3D_ColorData.pDDVColorGlobal->
                                  pvnSpecialColors, "Strand");
    if (pColorCell == NULL)
        return;

    ChooseColorDialog(&pColorCell->rgb[0], &pColorCell->rgb[1],
        &pColorCell->rgb[2], TRUE);
    DDV_SetPaletteColor(Cn3D_ColorData.pDDVColorGlobal, "Strand", "CN3D",
        pColorCell->rgb[0], pColorCell->rgb[1], pColorCell->rgb[2]);
    
    Cn3D_RedrawNUpdate(FALSE);
}

static void Cn3D_TurnColor(IteM i)
{
    DDV_ColorCell *pColorCell;

    pColorCell =
        DDV_SearchColorCellbyName(Cn3D_ColorData.pDDVColorGlobal->
                                  pvnSpecialColors, "Turn");
    if (pColorCell == NULL)
        return;

    ChooseColorDialog(&pColorCell->rgb[0], &pColorCell->rgb[1],
                      &pColorCell->rgb[2], TRUE);
    DDV_SetPaletteColor(Cn3D_ColorData.pDDVColorGlobal, "Turn", "CN3D",
        pColorCell->rgb[0], pColorCell->rgb[1], pColorCell->rgb[2]);

    Cn3D_RedrawNUpdate(FALSE);
}

static void Cn3D_CoilColor(IteM i)
{
    DDV_ColorCell *pColorCell;

    pColorCell =
        DDV_SearchColorCellbyName(Cn3D_ColorData.pDDVColorGlobal->
                                  pvnSpecialColors, "Coil");
    if (pColorCell == NULL)
        return;

    ChooseColorDialog(&pColorCell->rgb[0], &pColorCell->rgb[1],
                      &pColorCell->rgb[2], TRUE);
    DDV_SetPaletteColor(Cn3D_ColorData.pDDVColorGlobal, "Coil", "CN3D",
        pColorCell->rgb[0], pColorCell->rgb[1], pColorCell->rgb[2]);
    
    Cn3D_RedrawNUpdate(FALSE);
}


/* Compose the starting(NCBI-Logo) 3D-Picture */
static void LogoProc(Nlm_ButtoN b)
{
#ifdef _OPENGL
    OGL_DrawLogo(Cn3D_ColorData.OGL_Data);
#else
    Nlm_Prim3D p[4];
    Int4 x1, x2;
    double ang1, ang2;
    double r;

    if (Cn3D_pMain != NULL)
        DeletePicture3D(Cn3D_pMain);
    Cn3D_pMain = CreatePicture3D();
    if (readErrors())
        return;

    ResetPicture3D(Cn3D_pMain);
    if (readErrors())
        return;

    AllocPalette3D(Cn3D_pMain, 4);
    if (readErrors())
        return;

    SetColor3D(Cn3D_pMain, 0, 255, 0, 255);
    SetColor3D(Cn3D_pMain, 1, 0, 0, 255);
    SetColor3D(Cn3D_pMain, 2, 255, 255, 0);
    SetColor3D(Cn3D_pMain, 3, 0, 0, 255);

    x1 = 0;
    x2 = 4000000;
    ang1 = 0.2;
    p[0] = AddPoly3D(Cn3D_pMain, NULL, 0, 0, 0,
                     x2, (Int4) (15000000 * cos(ang1)),
                     (Int4) (15000000 * sin(ang1)), x1,
                     (Int4) (15000000 * cos(ang1)),
                     (Int4) (15000000 * sin(ang1)));
    ang2 = 3.2;
    p[1] = AddPoly3D(Cn3D_pMain, NULL, 0, 0, 1,
                     x2, (Int4) (15000000 * cos(ang2)),
                     (Int4) (15000000 * sin(ang2)), x1,
                     (Int4) (15000000 * cos(ang2)),
                     (Int4) (15000000 * sin(ang2)));

    ang1 = 0.0;
    for (r = 1; r <= 63; r++, ang1 += 0.2) {
        x1 += 700000;
        x2 += 700000;
        AddVertPoly3D(Cn3D_pMain, p[0],
                      x2, (Int4) (15000000 * cos(ang1)),
                      (Int4) (15000000 * sin(ang1)));
        AddVertPoly3D(Cn3D_pMain, p[0], x1, (Int4) (15000000 * cos(ang1)),
                      (Int4) (15000000 * sin(ang1)));
    }

    x1 = 0;
    x2 = 4000000;
    ang1 = 3.0;
    for (r = 1; r <= 79; r++, ang1 += 0.2) {
        x1 += 700000;
        x2 += 700000;
        AddVertPoly3D(Cn3D_pMain, p[1],
                      x2, (Int4) (15000000 * cos(ang1)),
                      (Int4) (15000000 * sin(ang1)));
        AddVertPoly3D(Cn3D_pMain, p[1], x1, (Int4) (15000000 * cos(ang1)),
                      (Int4) (15000000 * sin(ang1)));
    }

    AddSphere3D(Cn3D_pMain, NULL, (BigScalar) 0, 0, 2, 45000000, 0, 0,
                5000000);
    AddCylinder3D(Cn3D_pMain, NULL, (BigScalar) 0, 0, 2, 52000000, 5000000,
                  0, 52000000, -5000000, 0, 1000000);
    AddCylinder3D(Cn3D_pMain, NULL, (BigScalar) 0, 0, 2, 58000000, 5000000,
                  0, 58000000, -5000000, 0, 1000000);
    AddSphere3D(Cn3D_pMain, NULL, (BigScalar) 0, 0, 2, 65000000, 0, 0,
                5000000);
    AddCylinder3D(Cn3D_pMain, NULL, (BigScalar) 0, 0, 2, 72000000, 5000000,
                  0, 72000000, -5000000, 0, 1000000);
    AddSphere3D(Cn3D_pMain, NULL, (BigScalar) 0, 0, 2, 80000000, 0, 0,
                5000000);
    AddCylinder3D(Cn3D_pMain, NULL, (BigScalar) 0, 0, 2, 87000000, 5000000,
                  0, 87000000, -5000000, 0, 1000000);

    AddCylinder3D(Cn3D_pMain, NULL, (BigScalar) 0, 0, 2,
                  93000000, 5000000, 0, 93000000, -5000000, 0, 1000000);


    if (readErrors())
        return;
    SetHLColor3D(Cn3D_v3d, 255, 255, 0); /* use yellow color for highlight by default */

    AttachPicture3D(Cn3D_v3d, Cn3D_pMain, NULL);

    readErrors();
    SetLayerTop3D(0);
#endif                          /* ndef _OPENGL */
}



NLM_EXTERN void LIBCALL Cn3D_ResetActiveStrucProc(void)
{
    Cn3D_CountDomainProc();

    ResetRenderCtrls();
    ResetLabelCtrls();
/*    ResetAlignCtrls(); */
    ResetDisplayCtrls();
    Cn3dObjMgrGetSelected();
}

void /*LIBCALL*/ Cn3D_SaveActiveCam(void)
{
#ifndef _OPENGL
    PARS pars;
    PDNMS pdnmsThis = GetSelectedModelstruc();

    if (pdnmsThis && pdnmsThis->data.ptrvalue &&
        (pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS))
        GetViewerInfo3D(Cn3D_v3d, NULL, &Cn3D_Camera3D, NULL);
#endif                          /* ndef _OPENGL */
}




NLM_EXTERN void Cn3D_RedrawNUpdate(Boolean New)
{
    DDV_Clear2Default(Cn3D_ColorData.pDDVColorGlobal); 
    Cn3D_Redraw(New);
    Cn3D_SendUpdate();
}

NLM_EXTERN void LIBCALL Cn3D_Redraw(Boolean New)
{
    Cn3D_RedrawEx(New);
#ifdef _OPENGL
    OGL_DrawViewer3D(Cn3D_ColorData.OGL_Data); /* force a window redraw */
#endif
}

NLM_EXTERN void LIBCALL Cn3D_RedrawEx(Boolean New)
{
    /* fetch the active structure */
    PARS pars = NULL;
    DDV_ColorCell iColor;
    PDNMS pdnmsThis = GetSelectedModelstruc();

    if (pdnmsThis == NULL || 
        (pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS) == NULL) {
        LogoProc(NULL);
        return;
    }
#ifndef _OPENGL
    if (!New)
        GetViewerInfo3D(Cn3D_v3d, NULL, &Cn3D_Camera3D, NULL);
#endif                          /* ndef _OPENGL */

    WatchCursor();
    ProgMon("Removing 3D image ...");
#ifdef _OPENGL
    if (OGL_IsPlaying(Cn3D_ColorData.OGL_Data))
        OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
#else
    if (IsPlaying3D())
        StopPlaying3D();
#endif
    Cn3D_DisableFileOps();

#ifndef _OPENGL
    if (Cn3D_pMain)
        DeletePicture3D(Cn3D_pMain);
    Cn3D_pMain = CreatePicture3D();
    if (readErrors())
        return;

    ResetPicture3D(Cn3D_pMain);
    if (readErrors())
        return;
#endif                          /* ndef _OPENGL */

#ifdef _OPENGL
    /* resize the bound box */
    if (New) {
        OGL_ClearOGL_Data(Cn3D_ColorData.OGL_Data);
        Cn3D_Size(&(Cn3D_ColorData.OGL_Data->BoundBox), pdnmsThis);
        /* to do: add slaves to traverse */
        OGL_Reset(Cn3D_ColorData.OGL_Data);
    }
    /* set background color */
    Cn3D_ColorData.OGL_Data->Background.rgb[0] = pars->BGColRGB[0];
    Cn3D_ColorData.OGL_Data->Background.rgb[1] = pars->BGColRGB[1];
    Cn3D_ColorData.OGL_Data->Background.rgb[2] = pars->BGColRGB[2];
#endif                          /* _OPENGL */


/*
    if (Cn3D_DisplayHighlight)
        Cn3DCheckHighlighted();
    if (Cn3D_NoSingleHL) {
        Cn3D_DisplayHighlight = FALSE;
        Cn3D_bDisplayHighlightStatusSet(FALSE);
    }
*/
    /* execute all color functions */
    /*DDV_ColorExecute(Cn3D_ColorData.pDDVColorGlobal, NULL, NULL, Cn3D_ColorFuncFind());*/

    ProgMon("Rendering Structure...");
#ifdef _OPENGL
    AlgorithmicRendering();
#else                           /* _OPENGL */
    Cn3D_pMain = AlgorithmicRendering(Cn3D_pMain);

    if (readErrors())
        return;
    if (Cn3D_pMain == NULL) {   /* must have something; do xyz origin... reset palette with white */
        Cn3D_pMain = CreatePicture3D();
        if (readErrors())
            return;
        ResetPicture3D(Cn3D_pMain);
        if (readErrors())
            return;
        DDV_FreePalette(&(Cn3D_ColorData.pDDVColorGlobal->Palette)); /* free global palette */
        DDV_SetColorInCell(&iColor, Cn3d_PaletteRGB[C_white]); /* use white */
        DDV_RequestColor(&Cn3D_ColorData.pDDVColorGlobal->Palette,
                         &iColor);
        Cn3d_Lock3DPalette(Cn3D_pMain); /* Allocates the Picture3D palette  */
        Cn3D_pMain = Do3DOrigin(Cn3D_pMain); /* make the origin */
    }

    ProgMon("Redrawing 3D image ...");
    AttachPicture3D(Cn3D_v3d, Cn3D_pMain, &Cn3D_Camera3D);
#endif                          /* else _OPENGL */

    readErrors();
    Cn3D_EnableFileOps();
    ArrowCursor();
}



/* Mouse Action Callbacks */

static void LIBCALLBACK DoHighlightSeg(PFB pfbThis,
                                       Int4 iModel, Int4 iIndex,
                                       Pointer ptr)
/* used only for molecule wide highlighting.  not used at present time */
{
    PMAD pmadAtom = (PMAD) pfbThis;
#ifndef _OPENGL
    Viewer3D vvv = (Viewer3D) ptr;
#endif
    PALD paldLoc;

    if (!IsAtomNode(pfbThis))
        return;

    paldLoc = GetAtomLocs(pmadAtom, (Int2) iModel);
    while (paldLoc) {
#ifdef _OPENGL
#else                           /* _OPENGL */
        Segment3D seg = (Segment3D) paldLoc->pGraphic;
        if (seg != NULL)
            HighlightSeg3D(vvv, seg,
                           (Boolean) (!IsSeg3DHlighted(vvv, seg)));
#endif                          /* else _OPENGL */

        paldLoc = paldLoc->next; /* get next location */
    }
}

#ifndef _OPENGL
static Boolean GetPointData(MAPtr ma, PoinT point,
                            Prim3D PNTR prim,
                            Segment3D PNTR seg, BigScalar PNTR data)
{
    Prim3D x_prim = NULL;
    Segment3D x_seg = NULL;
    BigScalar x_data = 0;

    do { /* TRY */  {
            Viewer3D vvv = Nlm_MAToViewer3D(ma);
            Picture3D ppp = NULL;

            Nlm_GetViewerInfo3D(vvv, &ppp, NULL, NULL);
            if (!ppp)
                break;

            { {
                    Uint2 n_prim = FindPrim3D(vvv, point);
                    if (!n_prim)
                        break;
            }
            }

            x_prim = GetFoundPrim3D(vvv, 0);
            if (!x_prim)
                break;

            if (data || seg)
                GetPrimInfo3D(ppp, x_prim, &x_data, NULL, NULL, &x_seg,
                              NULL);
    }
    } while (0);

    if (prim)
        *prim = x_prim;
    if (seg)
        *seg = x_seg;
    if (data)
        *data = x_data;

    return (Boolean) (x_data != 0);
}

#endif                          /* ndef _OPENGL */


/*
 *  for highlighting an atom.  not presently used or ported to opengl
 */

static void HLatom_MA(MAPtr ma,
                      MA_TracePtr trace, PoinT point, VoidPtr extra)
{
#ifndef _OPENGL
    Viewer3D vvv = MAToViewer3D(ma);
    Prim3D prim = NULL;
    Segment3D seg = NULL;
    BigScalar data = 0;

    if (GetPointData(ma, point, &prim, &seg, &data) &&
        IsAtomLocNode((PFB) data))
            HighlightSeg3D(vvv, seg,
                           (Boolean) (!IsSeg3DHlighted(vvv, seg)));

    else if (data && IsObjectNode((PFB) data))
        HighlightPrim3D(vvv, prim,
                        (Boolean) (!IsPrim3DHlighted(vvv, prim)));

    else {
        if (!prim)
            BgColorDlg3D(vvv);
        return;
    }

    RedrawViewer3D(vvv);
#endif                          /* ndef _OPENGL */
}


/*
 *  the following functions are for highlighting a residue
 */

void DoCHighlightSeg(PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr,
                     Boolean highlight)
{
    PMAD pmadAtom = (PMAD) pfbThis;
#ifndef _OPENGL
    Viewer3D vvv = (Viewer3D) ptr;
#endif
    PALD paldLoc;

    if (!IsAtomNode(pfbThis))
        return;

    paldLoc = GetAtomLocs(pmadAtom, (Int2) iModel);
    while (paldLoc) {
#ifdef _OPENGL
#else                           /* _OPENGL */
        Segment3D seg = (Segment3D) paldLoc->pGraphic;
        if (seg != NULL) {
            if (highlight && !IsSeg3DHlighted(vvv, seg)) {
                HighlightSeg3D(vvv, seg,
                               (Boolean) (!IsSeg3DHlighted(vvv, seg)));
            } else if (!highlight && IsSeg3DHlighted(vvv, seg)) {
                HighlightSeg3D(vvv, seg,
                               (Boolean) (!IsSeg3DHlighted(vvv, seg)));
            }
        }
#endif                          /* else _OPENGL */

        paldLoc = paldLoc->next; /* get next location */
    }
}

void fnCHLresidue(PDNMG pdnmgThis,
#ifndef _OPENGL
                  Viewer3D vvv,
#endif
                  Boolean highlight)
{
    /* highlight residues corresponding to those in Salsa Window */

    PMGD pmgdThis = NULL;
    PVNMA pvnmaThis = NULL;
    PMAD pmadThis = NULL;
    PVNAL pvnalThis = NULL;
    PALD paldThis = NULL;
    PVNAL pvnalLinkThis = NULL;
#ifdef _OPENGL
    Nlm_VoidPtr vvv = NULL;
#endif

    pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
    pvnmaThis = pmgdThis->pvnmaAHead;

    while (pvnmaThis) {
        pmadThis = (PMAD) pvnmaThis->data.ptrvalue;
        if (pmadThis == NULL)
            goto setout;
        pvnalThis = pmadThis->pvnalLocate;
        if (pvnalThis == NULL)
            goto setout;
        paldThis = pvnalThis->data.ptrvalue;
        if (paldThis == NULL)
            goto setout;
        pvnalLinkThis = paldThis->pvnalLink;
        while (pvnalLinkThis) {
            DoCHighlightSeg((PFB) pmadThis, (Int4) pvnalLinkThis->choice,
                            0, vvv, highlight);
            pvnalLinkThis = pvnalLinkThis->next;
        }
      setout:
        pvnmaThis = pvnmaThis->next;
    }
    return;
}

/*
 * this is the main function called to highlight a particular residue
 */

void fnPreCHLresidue(PDNMG pdnmgThis, Boolean highlight)
{
    PMGD pmgdThis = NULL;
    PMSD pmsdThis = NULL;

    pmgdThis = pdnmgThis->data.ptrvalue;
    if (pmgdThis) {
        if (highlight)
            pmgdThis->bHighlighted = 1;
        else
            pmgdThis->bHighlighted = 0;
#ifndef _OPENGL

        pmsdThis = ToMSDParent((PFB) pmgdThis);

        if (pmsdThis && pmgdThis && pmsdThis->bVisible && pmgdThis->bVisible) {
            fnCHLresidue(pdnmgThis, Cn3D_v3d, highlight);
        }
#endif
    }
}

static void HLresidue_MA(MAPtr ma,
                         MA_TracePtr trace, PoinT point, VoidPtr extra)
/* highlight a residue */
{
#ifndef _OPENGL
    Viewer3D vvv = MAToViewer3D(ma);
    Prim3D prim = NULL;
    BigScalar data = 0;

    Boolean highlight = FALSE;
#endif                          /* ndef _OPENGL */

#ifdef _OPENGL
    {
        PMGD pmgdThis;
        PDNMG pdnmgThis;

        OGL_SetSelectPoint(Cn3D_ColorData.OGL_Data, point);
        OGL_Select(Cn3D_ColorData.OGL_Data, TRUE);
        OGL_DrawViewer3D(Cn3D_ColorData.OGL_Data);
        OGL_Select(Cn3D_ColorData.OGL_Data, FALSE);

        pmgdThis = OGL_Hit(Cn3D_ColorData.OGL_Data);
        if (pmgdThis == NULL || !IsGraphNode(pmgdThis)) /* can highlight anything */
            return;

        pdnmgThis = pmgdThis->pdnmgLink;
        MediaObjSelect(pdnmgThis, !pmgdThis->bHighlighted);
    }

#else /* _OPENGL */

    if (GetPointData(ma, point, &prim, NULL, &data) &&
        IsAtomLocNode((PFB) data)) {
        PALD paldThis = (PALD) data;
        PMGD pmgdThis = GetParentGraph((PFB) data);
        PDNMG pdnmgThis = pmgdThis->pdnmgLink;
        Segment3D seq = paldThis->pGraphic;
        if (!IsSeg3DHlighted(vvv, seq))
            highlight = TRUE;
        else
            highlight = FALSE;

        MediaObjSelect(pdnmgThis, highlight);

    }

    else if (data && IsObjectNode((PFB) data))
        HighlightPrim3D(vvv, prim,
                        (Boolean) (!IsPrim3DHlighted(vvv, prim)));

    else
        return;

    /* RedrawViewer3D(vvv); */
#endif                          /* else _OPENGL */

}

/*
 *   for highlighting an entire molecule.  not presently used or ported
 *   to opengl
 */

static void HLmolecule_MA(MAPtr ma,
                          MA_TracePtr trace, PoinT point, VoidPtr extra)
{
#ifndef _OPENGL
    Viewer3D vvv = Nlm_MAToViewer3D(ma);
    Prim3D prim = NULL;
    BigScalar data = 0;

    if (GetPointData(ma, point, &prim, NULL, &data) &&
        IsAtomLocNode((PFB) data)) {
        PALD paldThis = (PALD) data;
        PMMD pmmdThis = GetParentMol((PFB) data);
        if (pmmdThis)
            TraverseAtoms(pmmdThis->pdnmgHead,
                          (Int4) paldThis->pvnalLink->choice,
                          0, vvv, DoHighlightSeg);
    }

    else if (data && IsObjectNode((PFB) data))
        HighlightPrim3D(vvv, prim,
                        (Boolean) (!IsPrim3DHlighted(vvv, prim)));

    else
        return;

    RedrawViewer3D(vvv);
#endif                          /* ndef _OPENGL */
}


static Boolean Cn3D_InitMA(MAPtr ma, VoidPtr data)
{
/*MActionPtr hl_atom     = MA_AddAction(ma, MK_Shift, MA_DClick,
                                        HLatom_MA, data, "Highlight-Atom"); */

    MActionPtr hl_residue = MA_AddAction(ma, MK_Normal, MA_DClick,
                                         HLresidue_MA, data,
                                         "Highlight-Res");

/*MActionPtr hl_molecule = MA_AddAction(ma, MK_Ctrl,   MA_DClick,
                                        HLmolecule_MA, data, "Highlight-Mol"); */

    return (Boolean) (MA_SetAction(hl_residue, TRUE));
/*return (Boolean)(MA_SetAction(hl_atom,     TRUE)  &&
                   MA_SetAction(hl_residue,  TRUE)  &&
                   MA_SetAction(hl_molecule, TRUE)); */
}


#ifndef _OPENGL
static void Cn3D_ViewerCtrlProc(IteM i);
#endif
static void Cn3D_ShowCtrlProc(IteM i);


static CharPtr cn3dControlFormTabs[] = {
    "Style", "Label", "Show/Hide", "Annotate",
#ifdef _OPENGL
    "Quality",
#endif                          /* _OPENGL */
    NULL
};

static void ChangeCn3DCtrlPage(VoidPtr data, Int2 newval, Int2 oldval)
 {
    SafeHide(Cn3D_gRendCtrl);
    SafeHide(Cn3D_gLabelCtrl);
    SafeHide(Cn3D_gDisplayCtrl);
    SafeHide(Cn3D_gModelCtrl);
    SafeHide(Cn3D_gAlignCtrl);
#ifdef _OPENGL
/*    SafeHide(Cn3D_gOGLCtrl); */
    SafeHide(Cn3D_gOGLQuality);
#endif                          /* _OPENGL */
    switch (newval) {
    case 0:
        SafeShow(Cn3D_gRendCtrl);
        break;
    case 1:
        SafeShow(Cn3D_gLabelCtrl);
        break;
    case 2:
        SafeShow(Cn3D_gDisplayCtrl);
        break;
    case 3:
        SafeShow(Cn3D_gModelCtrl);
        break;
#ifdef _OPENGL
    case 4:
/*        SafeShow(Cn3D_gOGLCtrl);
        break;
    case 5:*/
        SafeShow(Cn3D_gOGLQuality);
        break;
#endif                          /* _OPENGL */
    default:
        break;
    }
}

static GrouP Viewer3DGroups(WindoW w)
{
    Int2 groups = 0;
    /* PoinT pnt; */
    RecT Cn3D_rRC;
    GrouP g, h;
    DialoG tabs;

    h = HiddenGroup(w, 1, 0, NULL);
    tabs = CreateFolderTabs(h, cn3dControlFormTabs, 0, 0, 0,
                            SYSTEM_FOLDER_TAB, ChangeCn3DCtrlPage, NULL);

    g = HiddenGroup(w, 0, 0, NULL);

#ifdef WIN_MOTIF
    SetGroupMargins(g, 0, 0);
    SetGroupSpacing(g, 8, 1);
#else
    SetGroupMargins(g, 1, 1);
    SetGroupSpacing(g, 0, 0);
#endif

    Cn3D_gRendCtrl = RenderControls(g);
    GetPosition(Cn3D_gRendCtrl, &Cn3D_rRC);
    Cn3D_Rx = (Uint2) (Cn3D_rRC.right - Cn3D_rRC.left + 10);

    Cn3D_gLabelCtrl = LabelControls(g);
    Hide(Cn3D_gLabelCtrl);

    Cn3D_gDisplayCtrl = DisplayControls(g);
    Hide(Cn3D_gDisplayCtrl);

    Cn3D_gModelCtrl = ModelControls(g);
    Hide(Cn3D_gModelCtrl);

#ifdef _OPENGL
/*    Cn3D_gOGLCtrl = OGL_Controls(g);
    Hide(Cn3D_gOGLCtrl); */

    Cn3D_gOGLQuality = OGL_Quality(g);
    Hide(Cn3D_gOGLQuality);
#endif                          /* _OPENGL */

    AlignObjects(ALIGN_CENTER, (HANDLE) tabs,
                 (HANDLE) Cn3D_gRendCtrl, (HANDLE) Cn3D_gLabelCtrl,
                 (HANDLE) Cn3D_gDisplayCtrl, (HANDLE) Cn3D_gModelCtrl,
#ifdef _OPENGL
                 /*(HANDLE) Cn3D_gOGLCtrl,*/ (HANDLE) Cn3D_gOGLQuality,
#endif                          /* _OPENGL */
                 NULL);


    return w;
}

#ifndef _OPENGL
static GrouP Viewer3DViewer(WindoW w, Uint2Ptr width, Uint2 height)
{
    Int2 groups = 0;
    PoinT pnt;
    RecT Cn3D_rVC;

    GrouP g = HiddenGroup(w, 0, 0, NULL);

#ifdef WIN_MOTIF
    SetGroupMargins(g, 0, 0);
    SetGroupSpacing(g, 8, 1);
#else
    SetGroupMargins(g, 1, 1);
    SetGroupSpacing(g, 0, 0);
#endif

    Cn3D_gViewCtrl = HiddenGroup(g, 0, 0, NULL);

    GetPosition(Cn3D_gViewCtrl, &Cn3D_rVC);
    Cn3D_Vy = (Uint2) (Cn3D_rVC.bottom - Cn3D_rVC.top + 10);

    Cn3D_left = CreateControls3D(Cn3D_gViewCtrl, FALSE, TRUE, NULL);
    Hide(Cn3D_gViewCtrl);

    GetPosition(Cn3D_gViewCtrl, &Cn3D_rVC);
    Cn3D_Vy = (Uint2) (Cn3D_rVC.bottom - Cn3D_rVC.top + 10);
    Break(g);

    pnt.x = 10;
    pnt.y = 15;
    SetNextPosition(g, pnt);
    Cn3D_gViewer = HiddenGroup(g, 0, 0, NULL); /* the viewer */

    Cn3D_v3d = CreateViewer3D(Cn3D_gViewer, width, height,
                              X_ROTATE_SBAR | Y_ROTATE_SBAR |
                              Z_ROTATE_SBAR, Cn3D_ma_group_menu,
                              Cn3D_ma_action_menu, Cn3D_InitMA, NULL);
    if (readErrors())
        return NULL;

    if (Cn3D_v3d == NULL) {
        Message(MSG_OK,
                "Cn3D Viewer - Insufficient Memory For Structures");
        return NULL;
    }

    if (GetStatus(Cn3D_iViewCtrl) == TRUE)
        LinkControls3D(Cn3D_left, Cn3D_v3d);

    return HiddenGroup(w, 1, 0, NULL);
}

#endif                          /* ndef _OPENGL */


void Cn3DResizeProc(WindoW w)
{
    RecT r;
    ObjectRect(Cn3D_w, &r);
    OffsetRect(&r, (Int2) (-r.left), (Int2) (-r.top));

    InsetRect(&r, 5, 5);

#ifdef WIN_MAC
    { {
            extern Handle Nlm_GetWindowMenuBar(WindoW w);

            Handle hdl = Nlm_GetWindowMenuBar(Cn3D_w);
            if (hdl) {
                RecT mbr;
                ObjectRect(hdl, &mbr);
                r.top += (mbr.bottom - mbr.top);
            }
    }
    }
#endif


#ifdef _OPENGL
    OGL_SetPosition3D(Cn3D_ColorData.OGL_Data, &r);
#else                           /* _OPENGL */
    if (GetStatus(Cn3D_iViewCtrl) == TRUE)
        r.top += Cn3D_Vy;
    SetPosition3D(Cn3D_v3d, &r);
#endif                          /* else _OPENGL */
}


#ifndef _OPENGL
#ifdef WIN_MSWIN
static void Cn3D_ImageCopyProc(IteM I)
{
    CopyViewer3D(Cn3D_v3d);
}

#endif
#endif                          /* ndef _OPENGL */



#include <document.h>
static ParData Cn3D_ParFmt = { FALSE, FALSE, FALSE, FALSE, TRUE, 0, 0 };
static ColData Cn3D_ColFmt =
    { 0, 0, 40, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE };

static void Cn3D_AboutQuit(WindoW w)
{
    Remove(w);
    return;
}


static void Cn3D_AboutSize(WindoW w)
{
    Int4 height, width;
    RecT r;

    DoC d = (DoC) GetObjectExtra(w);
    if (!d)
        return;

    WatchCursor();

    ObjectRect(w, &r);
    width = r.right - r.left;
    height = r.bottom - r.top;

    GetPosition(d, &r);
    r.right = (Int2) (width - r.left);
    r.bottom = (Int2) (height - r.top);
    SetPosition(d, &r);

    AdjustPrnt(d, &r, FALSE);
    ObjectRect(d, &r);
    InsetRect(&r, 4, 4);
    Cn3D_ColFmt.pixWidth = (Int2) (screenRect.right - screenRect.left);
    Cn3D_ColFmt.pixInset = 8;
    if (Visible(d) && AllParentsVisible(d))
        UpdateDocument(d, 0, 0);

    ArrowCursor();
    Update();
}

static void Cn3D_AlignEdit(IteM i)
/* launches the sequin editor, salsa */
{
    PDNMS pdnmsMaster = NULL;
    PDNMS pdnmsSlave = NULL;
    PMSD pmsdMaster = NULL, pmsdSlave = NULL;
    SeqAnnotPtr psaAlign = NULL;
    SeqAlignPtr salp = NULL;
    PDNMM pdnmmHead = NULL;
    PMMD pmmdThis = NULL;
    Int2 iCount = 0;


    pdnmsMaster = GetSelectedModelstruc();
    if (pdnmsMaster == NULL)
        return;
    pmsdMaster = (PMSD) pdnmsMaster->data.ptrvalue;

    pdnmmHead = pmsdMaster->pdnmmHead;
    pmmdThis = pdnmmHead->data.ptrvalue;

    if (pmsdMaster->pdnmsSlaves != NULL) {
        pmsdSlave = pmsdMaster->pdnmsSlaves->data.ptrvalue;
    }
    if (Cn3D_ColorData.sap != NULL)
        psaAlign = Cn3D_ColorData.sap;

    if (psaAlign == NULL) {
        Cn3D_LaunchSeqEntry(Cn3D_ColorData.pvnsep);
        Cn3dObjMgrGetSelected();
    }

    if (psaAlign != NULL && psaAlign->data != NULL) {
        Cn3dObjMgrGetSelected();
        Cn3D_LaunchSeqAnnot(psaAlign);
    }

    return;
}

void LaunchSequenceWindow(void)
{
    IteM i = NULL;
    Cn3D_AlignEdit(i);
}

static void Cn3D_AboutPrint(PDNMS pdnmsThisSlave, FILE *fp) 
{
    WritePDBHeader(pdnmsThisSlave, fp);
    WritePDBRemarks(pdnmsThisSlave, fp);
    WritePDBMotifs(pdnmsThisSlave, fp);
    WriteStrucHTMLSeq(pdnmsThisSlave, fp);
    fprintf(fp, "\n\n\n\n\n\n\n\n\n\n");
}

static void Cn3D_AboutStruc(IteM i)
{
    PMSD pmsdThis;
    DoC d;
    Char path[PATH_MAX];
    FILE *fp;
    WindoW Cn3D_wAbout;
    PDNMS pdnmsThisSlave;       /* pointer to slave structures */

    PDNMS pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis)
        return;

    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

    Cn3D_wAbout = DocumentWindow(-66, -50, -10, -10,
                                 GetStrucStrings(pdnmsThis, PDB_ACC),
                                 Cn3D_AboutQuit, Cn3D_AboutSize);

    d = DocumentPanel(Cn3D_wAbout,
                      (Int2) (35 * stdCharWidth + 17),
                      (Int2) (14 * stdLineHeight));
    SetObjectExtra(Cn3D_wAbout, (Pointer) d, NULL);
    Cn3D_ColFmt.pixWidth = (Int2) (60 * stdCharWidth);
    Cn3D_ColFmt.font = programFont;

    path[0] = '\0';
    TmpNam(path);
    fp = FileOpen(path, "w");
    if (fp != NULL) {
        Cn3D_AboutPrint(pdnmsThis, fp);
        /* loop through the slave structures */
        if (pmsdThis->pdnmsSlaves != NULL) {
            pdnmsThisSlave = pmsdThis->pdnmsSlaves; /* go through the slave structures */
            while (pdnmsThisSlave) {
                Cn3D_AboutPrint(pdnmsThisSlave, fp);
                pdnmsThisSlave = pdnmsThisSlave->next;
            }
        }
        fflush(fp);
        FileClose(fp);
    }

    DisplayFancy(d, path, &Cn3D_ParFmt, &Cn3D_ColFmt, programFont, 4);
    FileRemove(path);
    Show(Cn3D_wAbout);
}

#ifdef _OPENGL
static void Cn3D_ZoomOutCB(IteM i)
{
    OGL_ZoomOut(Cn3D_ColorData.OGL_Data);
}

static void Cn3D_ZoomInCB(IteM i)
{
    OGL_ZoomIn(Cn3D_ColorData.OGL_Data);
}
#endif

/* Create the generic CN3D menu system;
 * return handler to the "File" top menu
 */
static MenU Cn3D_Menus(WindoW w, ItmActnProc netconfig)
{
    MenU file_menu, color_menu, color_submenu, menu;
    int i;

    /* FILE top menu
     */

    file_menu = menu = PulldownMenu(w, "File/F");

    if (!Cn3D_ColorData.UseEntrez) {
        Cn3D_iOpen = CommandItem(menu, "Open.../O", Cn3D_OpenBiostruc);
        Cn3D_sOpen = NULL;
    } else {
        Cn3D_sOpen = SubMenu(menu, "Open/0");
        CommandItem(Cn3D_sOpen, "File.../B", Cn3D_OpenBiostruc);
        CommandItem(Cn3D_sOpen, "by Network.../N", Cn3D_NetOpenBiostruc);
        Cn3D_iOpen = NULL;
    }


    /* Import menu item would go here */
    /*Cn3D_sSave = Cn3D_SaveSub(menu);*/
    Cn3D_sSave = CommandItem(menu, "Save.../S", Cn3D_SaveBiostruc);
    Cn3D_sExport = Cn3D_ExportSub(menu);
    /*CommandItem(menu, "Mesh /M", Cn3D_OpenMesh);*/

    SeparatorItem(menu);
    CommandItem(menu, "Properties.../P", Cn3D_AboutStruc);

    /* EDIT top menu */

    menu = PulldownMenu(w, "View/V");
#ifndef _OPENGL
#ifdef WIN_MSWIN
    CommandItem(menu, "Copy/C", Cn3D_ImageCopyProc);
    SeparatorItem(menu);
#endif
#endif                          /* ndef _OPENGL */


    Cn3D_iDisplayCtrl =
        StatusItem(menu, "Drawing Settings.../N", Cn3D_ShowCtrlProc);
#ifndef _OPENGL
    Cn3D_iViewCtrl = StatusItem(menu,
                                "Animation Controls/A",
                                Cn3D_ViewerCtrlProc);
#else
    CommandItem(menu, "Animation.../A", Cn3D_Animate);
#endif                          /* ndef _OPENGL */
    CommandItem(menu, "Sequence Window.../S", Cn3D_AlignEdit);

    SeparatorItem(menu);
#ifdef _OPENGL
    CommandItem(menu, "Zoom Out", Cn3D_ZoomOutCB);
    CommandItem(menu, "Zoom In", Cn3D_ZoomInCB);
#endif
    CommandItem(menu, "Reset", Cn3D_AllCB);

#ifdef _DEBUG
    SeparatorItem(menu);
    VSMAddToMenu(menu, VSM_DESKTOP);
#endif  /* _DEBUG */

    /* RENDER top menu
     */

    menu = PulldownMenu(w, "Style/S");

    CommandItem(menu, "Sec. Structure/S", Cn3D_RenStruc);
    CommandItem(menu, "Neighbor/N", Cn3D_RenAlign);
    CommandItem(menu, "WireFrame/W", Cn3D_RenWire);
    CommandItem(menu, "Tubular/T", Cn3D_RenTube);
    CommandItem(menu, "Hierarchy/H", Cn3D_RenHier);
    CommandItem(menu, "Spacefill/P", Cn3D_RenSpace);
    CommandItem(menu, "Ball and Stick/B", Cn3D_RenBS);

    /* COLOR top menu */
    menu = PulldownMenu(w, "Color/C");
    CommandItem(menu, Cn3D_ColorFuncName(C_BYSECSTRUC), Cn3D_ColSecStruc);
    CommandItem(menu, Cn3D_ColorFuncName(C_BYSTRUCALIGN), Cn3D_ColStrucAlign);
    color_submenu = SubMenu(menu, Cn3D_ColorFuncName(C_BYSEQCONS));

        /* sequence conservation submenu */
    for (i = 0; i < CSC_NUMALGORITHMS; i++) {
        SetObjectExtra(
          CommandItem(color_submenu, (Nlm_CharPtr) CSC_GetAlgorithmName(i),  Cn3D_ColSeqCons), 
          (Nlm_VoidPtr) i, NULL);
    }

    CommandItem(menu, Cn3D_ColorFuncName(C_BYOBJECT), Cn3D_ColObject);
    CommandItem(menu, Cn3D_ColorFuncName(C_BYCHAIN), Cn3D_ColCyChain);
    CommandItem(menu, Cn3D_ColorFuncName(C_BYDOMAIN), Cn3D_ColDomain);
    CommandItem(menu, Cn3D_ColorFuncName(C_BYRES), Cn3D_ColRes);
    CommandItem(menu, Cn3D_ColorFuncName(C_BYHYDRO), Cn3D_ColHydro);
    CommandItem(menu, Cn3D_ColorFuncName(C_CPK), Cn3D_ColCPK);
    CommandItem(menu, Cn3D_ColorFuncName(C_BYTEMP), Cn3D_ColTemp);

    /* OPTIONS top menu */
    menu = PulldownMenu(w, "Option/O");
    if (netconfig != NULL) {
        CommandItem(menu, "Net Configure.../N", netconfig);
    }
    color_menu = SubMenu(menu, "Palette Settings");

#ifndef _OPENGL
    color_submenu = SubMenu(color_menu, "Background/B");
    CommandItem(color_submenu, "Default", Cn3D_Default_BgColor);
    CommandItem(color_submenu, "User Defined...", Cn3D_BgColor);
    SeparatorItem(color_menu);
    color_submenu = SubMenu(color_menu, "Highlight");
    CommandItem(color_submenu, "Default", Cn3D_Default_HLColor);
    CommandItem(color_submenu, "User Defined...", Cn3D_HLColor);
    SeparatorItem(color_menu);

    color_submenu = SubMenu(color_menu,  "Secondary Structure");
    CommandItem(color_submenu, "Default", Cn3D_Default_SSColor);
    CommandItem(color_submenu, "Helix...", Cn3D_HelixColor);
    CommandItem(color_submenu, "Strand/Sheet...", Cn3D_StrandColor);
    CommandItem(color_submenu, "Turn...", Cn3D_TurnColor);
    CommandItem(color_submenu, "Coil...", Cn3D_CoilColor);
#else /* _OPENGL */
    CommandItem(color_menu, "Helix...", Cn3D_HelixColor);
    CommandItem(color_menu, "Strand/Sheet...", Cn3D_StrandColor);
    CommandItem(color_menu, "Turn...", Cn3D_TurnColor);
    CommandItem(color_menu, "Coil...", Cn3D_CoilColor);
    CommandItem(color_menu, "Highlight...", Cn3D_HLColor);
    SeparatorItem(color_menu);
    CommandItem(color_menu, "Default", Cn3D_Default_SSColor);
#endif

    Cn3D_ma_group_menu = NULL /*SubMenu(menu, "Mouse Settings/M")*/;
    Cn3D_ma_action_menu = NULL /* SubMenu(menu, "Mouse3D Actions") */ ;

    /* CONTROLS top menu
     */

    menu = PulldownMenu(w, "Help/H");

    CommandItem(menu, "Help.../H", Cn3D_HelpProc);
    CommandItem(menu, "About Cn3D.../B", Cn3D_AboutProc);

    SetStatus(Cn3D_iRendCtrl, FALSE);
#ifndef _OPENGL
    SetStatus(Cn3D_iViewCtrl, FALSE);
#endif
    SetStatus(Cn3D_iAlignCtrl, FALSE);


    return file_menu;
}


static void Cn3D_ShowCtrlProc(IteM i)
 {
    if (GetStatus(i)) {
        SafeShow(Cn3D_wCtrls);
        Select(Cn3D_wCtrls);
    } else {
        SafeHide(Cn3D_wCtrls);
    }
    Update();
}

#ifndef _OPENGL
static void Cn3D_ViewerCtrlProc(IteM i)
{
    if (i == NULL) {
        SetStatus(Cn3D_iViewCtrl, FALSE);
        Hide(Cn3D_gViewCtrl);
        Update();
        return;
    }

    if (GetStatus(Cn3D_iViewCtrl) == TRUE) {
        Show(Cn3D_gViewCtrl);
        Update();
        LinkControls3D(Cn3D_left, Cn3D_v3d);
    } else {
        Hide(Cn3D_gViewCtrl);
        Update();
    }

    Cn3DResizeProc(Cn3D_w);
}

#endif                          /* ndef _OPENGL */

extern void Cn3D_HideCtrl(WindoW w);
extern void Cn3D_HideCtrl(WindoW w)
 {
    SetStatus(Cn3D_iDisplayCtrl, FALSE);
    SafeHide(Cn3D_wCtrls);      /* w may be NULL if called from cn3dentr.c */
    Update();
}


static void Cn3D_QuitAlready()
{
    ClearStructures();
    ClearSequences();
    QuitProgram();
}

/*  Default Quits
 */
static void Cn3D_QuitProc(IteM i)
{
    Cn3D_QuitAlready();
}

static void Cn3D_Quit(WindoW w)
{
    Cn3D_QuitAlready();
}


/* Create a complete CN3D window and GUI environment
   StandAlone determines whether Cn3D was called or is standalone
 */
extern WindoW LIBCALL Cn3DWin(WndActnProc on_close, MenU * file_menu,
                              ItmActnProc netconfig, Boolean StandAlone)
{
    static Boolean Cn3D_Window_Alive = FALSE;
#ifdef _OPENGL
    TOGL_Data *OGL_Data;
#endif

    /* register bioseq callback */
    REGISTER_BIOSEQ_BIOSTRUC_MEDIA;
    REGISTER_SEQANNOT_BIOSTRUC_MEDIA;
    Cn3D_ColorData.userkey = OMGetNextUserKey(); /* uniquely ids process */

    fnMMDBCn3Dmode();           /* make mmdbapi run in cn3d mode */

    Cn3D_fAlignOn = TRUE;       /* turn on aligned and unaligned residues */
    Cn3D_fUnalignOn = TRUE;

    if (Cn3D_Window_Alive)
        return (Handle) Cn3D_w;

    /* to ensure that Motif will allocate only the colors having
     * index less than 32;  all other colorcells will be used (and
     * are to be redefined) by 3D-viewer
     */
    RestrictMotifColorsTo(32);

    /* CN3d window and menus */
    { {
            MenU menu;

            /* CN3d window and menubar
             */
            Cn3D_w = DocumentWindow(-33, -10, -10, -10, "Cn3D 3.0",
                                    (on_close ? on_close : Cn3D_Quit),
                                    NULL);

            /* CN3D general menu set
             */
            menu = Cn3D_Menus(Cn3D_w, netconfig);

            if (file_menu)
                *file_menu = menu;
            else {              /* standard quit */
                SeparatorItem(menu);
                CommandItem(menu, "Quit/Q", Cn3D_QuitProc);
            }
    }
    }


    { {
            Uint2 Cn3D_uSize;
            Int2 Cn3D_size =
                (Int2) MIN(screenRect.right, screenRect.bottom);

            Cn3D_size -= 128;
            if (Cn3D_size < 200)
                Cn3D_size = 200;
            else if (Cn3D_size > 400)
                Cn3D_size = 400;

            Cn3D_uSize = (Uint2) Cn3D_size;
#ifdef _OPENGL
            OGL_Data =
                OGL_CreateViewer(Cn3D_w, &Cn3D_uSize, Cn3D_uSize,
                                 X_ROTATE_SBAR | Y_ROTATE_SBAR |
                                 Z_ROTATE_SBAR, Cn3D_ma_group_menu,
                                 Cn3D_ma_action_menu, Cn3D_InitMA, NULL);
#else
            Cn3D_gWinGP = Viewer3DViewer(Cn3D_w, &Cn3D_uSize, Cn3D_uSize);
#endif
    }
    }
    Cn3D_ConstructColorData(&Cn3D_ColorData
#ifdef _OPENGL
        , OGL_Data
#endif
        , StandAlone); /* create default palette */
    Cn3D_ColorData.Cn3D_w = Cn3D_w;

    /*
       todo: both Viewer3dViewer and Cn3D_ColorData need to be deallocated.  CloseMMDBAPI()
       probably should be called also.  This means we need some sort of callback from Entrez
       and Sequin to do this right.  lyg

       also need to break out OGL_Data from Viewer 3D when it gets its own initialization routine.

     */

    ProcessUpdatesFirst(FALSE);
#ifndef _OPENGL
    if (Cn3D_gWinGP == NULL)
        return (Handle) NULL;
#endif

    Cn3D_EnableFileOps();
    RealizeWindow(Cn3D_w);
#if defined(_OPENGL) && defined(WIN_MOTIF)
    /* now that all windows are realized, set X OpenGL context  (thiessen) */
    Nlm_SetOGLContext(NULL, NULL, NULL, NULL);
#endif
#ifdef _OPENGL
    OGL_InitializeLists(OGL_Data);
#endif

    Cn3dObjMgrGetSelected();
    Cn3D_Redraw(TRUE);
    SetResize(Cn3D_w, Cn3DResizeProc);
    Cn3DResizeProc(Cn3D_w);
    Cn3D_Window_Alive = TRUE;

    Cn3D_wCtrls =
        FixedWindow(-10, -33, -10, -10, "Drawing settings", Cn3D_HideCtrl);
    Cn3D_gWinGP = Viewer3DGroups(Cn3D_wCtrls);
    RealizeWindow(Cn3D_wCtrls);

#ifdef _OPENGL
    /* restore saved quality settings */
    Cn3D_SetQualityFromAppParams();
#endif

    return Cn3D_w;
}


Nlm_Boolean Nlm_Call_SetPosition3D(Nlm_RecT *rect)
{
#ifdef _OPENGL
    return OGL_SetPosition3D(Cn3D_ColorData.OGL_Data, rect);
#else                           /* _OPENGL */
    return SetPosition3D(Cn3D_v3d, rect);
#endif                          /* else _OPENGL */
}

Nlm_Boolean Nlm_Call_SaveImageGIF(CharPtr defname)
{
#ifndef _OPENGL
    return Nlm_SaveImageGIF(Nlm_GetViewerImage3D(Cn3D_v3d), defname);
#else
    return Nlm_SaveImageGIF(NULL, NULL);
#endif
}
