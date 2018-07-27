/*   algorend.c
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
* File Name:  algorend.c
*
* Authors:  Christopher Hogue, Lewis Geer, Yanli Wang
*
* Version Creation Date:
*
* File Description: Algorithmic rendering routines and Vibrant controls for Cn3d
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: algorend.c,v $
* Revision 6.167  2001/04/27 19:53:36  juran
* Heed some warnings.
*
* Revision 6.166  2000/07/28 21:05:56  lewisg
* more c++ fixes
*
* Revision 6.165  2000/07/27 13:37:32  lewisg
* more c++ fixes
*
* Revision 6.164  2000/07/21 18:55:14  thiessen
* allow dynamic slave->master transformation
*
* Revision 6.163  2000/05/19 18:19:09  thiessen
* fix label bug on show/hide
*
* Revision 6.162  2000/05/04 19:00:05  thiessen
* fix to worm capping
*
* Revision 6.161  2000/05/01 12:20:11  thiessen
* persist animation speed
*
* Revision 6.160  2000/04/25 00:22:34  thiessen
* save quality settings in config
*
* Revision 6.159  2000/04/20 23:27:44  lewisg
* misc bug fixes
*
* Revision 6.158  2000/04/20 18:53:56  thiessen
* misc tweaks/fixes
*
* Revision 6.157  2000/04/19 17:56:47  thiessen
* added background color in OpenGL
*
* Revision 6.156  2000/04/17 23:49:44  thiessen
* fix #ifdef indentation typo
*
* Revision 6.155  2000/04/17 15:54:25  thiessen
* add cylinder arrows; misc graphics tweaks
*
* Revision 6.154  2000/04/11 20:05:39  lewisg
* fix freeing bug on bioseq import, misc. tweaks
*
* Revision 6.153  2000/04/08 00:37:31  lewisg
* multiple seqentries, NEWSEQ message, etc.
*
* Revision 6.152  2000/04/06 19:03:20  thiessen
* new BLOSUM62 conservation coloring
*
* Revision 6.151  2000/04/05 23:23:28  lewisg
* fix viewer3d type
*
* Revision 6.150  2000/04/05 23:11:27  lewisg
* get rid of double redraw for hide row
*
* Revision 6.149  2000/04/05 16:59:22  lewisg
* get rid of animation frame when row is hid
*
* Revision 6.148  2000/04/03 18:23:46  thiessen
* add arrowheads to strand bricks
*
* Revision 6.147  2000/03/23 18:27:24  thiessen
* show-by-selected now works for hets/ions/water
*
* Revision 6.146  2000/03/23 15:22:33  thiessen
* fix cycle object color for non-residues
*
* Revision 6.145  2000/03/23 14:53:03  thiessen
* fix bug in viewer3d camera; added conservation color algorithm to ARS
*
* Revision 6.144  2000/03/22 23:42:22  lewisg
* timing loop for animation
*
* Revision 6.143  2000/03/22 23:17:48  thiessen
* added ability to save ARS in ASN1
*
* Revision 6.142  2000/03/20 15:56:00  thiessen
* fixed residue coloring bug
*
* Revision 6.141  2000/03/17 22:48:58  thiessen
* fix for multi-chain / multi-model features ; added feature-move ; misc bug fixes
*
* Revision 6.140  2000/03/17 00:08:19  thiessen
* fixed coloring bug in unaligned sequences
*
* Revision 6.139  2000/03/16 20:31:44  thiessen
* fixes to color; new color-by-variety algorithm
*
* Revision 6.138  2000/03/15 18:49:06  thiessen
* fixed viewer3d color bug
*
* Revision 6.137  2000/03/15 16:59:54  thiessen
* fix highlighting, other minor bugs
*
* Revision 6.136  2000/03/15 04:43:54  thiessen
* various minor fixes
*
* Revision 6.135  2000/03/14 18:03:12  thiessen
* add target row to MMD; remove rowmgr
*
* Revision 6.134  2000/03/13 16:04:25  thiessen
* re-implementation of features
*
* Revision 6.133  2000/03/09 17:56:57  thiessen
* changes to palette handling, feature implementation, PARS storage
*
* Revision 6.132  2000/03/08 21:46:14  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.131  2000/03/06 18:35:23  thiessen
* fixes for 8-bit color
*
* Revision 6.130  2000/03/06 15:33:53  lewisg
* add new hydrophobic palette, update visual c++ project to move seqcons
*
* Revision 6.129  2000/03/03 20:05:16  thiessen
* removal of palette-building pass if in 24-bit color
*
* Revision 6.128  2000/03/02 21:18:28  thiessen
* installed color-by-conservation on all alignment rows
*
* Revision 6.127  2000/03/01 21:03:00  thiessen
* fixed 'connection' display problem
*
* Revision 6.126  2000/03/01 16:17:54  thiessen
* improved handling of colors; many small fixes
*
* Revision 6.125  2000/02/26 13:53:00  thiessen
* fixed typecast error
*
* Revision 6.124  2000/02/26 13:30:40  thiessen
* capped cylinders and worms for visible ends
*
* Revision 6.123  2000/02/26 00:01:39  thiessen
* OpenGL improvements, progress on cleanup of Show/Hide
*
* Revision 6.122  2000/02/25 02:53:14  thiessen
* check AlignMgr rather than bAligned flag to see if a residue is aligned
*
* Revision 6.121  2000/02/23 20:09:22  thiessen
* fixed color-by-conservation
*
* Revision 6.120  2000/02/23 18:56:29  thiessen
* move to 1-based row numbers
*
* Revision 6.119  2000/02/19 21:25:56  thiessen
* split of cn3dmodl into cn3dmodl and cn3dstyl
*
* Revision 6.118  2000/02/19 01:23:58  lewisg
* use ibm, add row tracking code, various bug fixes
*
* Revision 6.117  2000/02/17 15:41:11  thiessen
* added CSC algorithm selection submenu
*
* Revision 6.116  2000/02/16 21:25:31  thiessen
* new row manager module; made Cn3D use row-wise color storage for aligned strucs
*
* Revision 6.115  2000/02/14 22:18:21  lewisg
* fix merge error
*
* Revision 6.114  2000/02/14 20:04:20  thiessen
* another fix for solvent; more filling in of seqcons
*
* Revision 6.113  2000/02/14 19:31:02  lewisg
* fix occupancy bug when temperature factors present
*
* Revision 6.112  2000/02/14 12:32:30  thiessen
* fix solvent identification; progress on seqcons module
*
* Revision 6.111  2000/02/11 15:52:28  thiessen
* transparent solvent if partial occupancy; move alt conf behaviour checkbox to Show/Hide panel
*
* Revision 6.110  2000/02/11 01:10:36  thiessen
* creating new module seqcons for sequence conservation coloring routines
*
* Revision 6.109  2000/02/10 19:04:25  thiessen
* added checkbox to change altConf behaviour
*
* Revision 6.108  2000/02/10 17:53:02  lewisg
* correct type bug
*
* Revision 6.107  2000/02/10 17:47:01  thiessen
* added: color-by-sequence-conservation menu item, zoom-out to OpenGL, misc fixes
*
* Revision 6.106  2000/02/10 15:51:58  lewisg
* cn3d responds and send correct update messages.  many coloring bug fixes
*
* Revision 6.105  2000/02/07 20:17:37  lewisg
* minor bug fixes, use gui font for win32
*
* Revision 6.104  2000/02/03 15:07:44  thiessen
* more correct handling of worm bonds
*
* Revision 6.103  2000/01/25 22:58:12  thiessen
* added animation of conf-ensembles
*
* Revision 6.102  2000/01/14 21:40:40  lewisg
* add translucent spheres, ion labels, new cpk, fix misc bugs
*
* Revision 6.101  2000/01/11 01:16:46  lewisg
* fix color selection in Cn3D, other misc. bugs
*
* Revision 6.100  2000/01/06 00:04:42  lewisg
* selection bug fixes, update message outbound, animation APIs moved to vibrant
*
* Revision 6.99  1999/12/29 22:55:02  lewisg
* get rid of seqalign id
*
* Revision 6.98  1999/12/27 23:14:10  lewisg
* add colormgr show/hide in Cn3D
*
* Revision 6.97  1999/12/22 22:49:20  lewisg
* fix color bugs: no color in color by domain, selection overrides cpk and temperature
*
* Revision 6.96  1999/12/16 15:25:21  thiessen
* removed unused buttons/popups from OpenGL version
*
* Revision 6.95  1999/12/13 23:20:43  lewisg
* bug fixes: duplicate color structures eliminated, clear color doesn't clear case
*
* Revision 6.94  1999/12/11 01:30:34  lewisg
* fix bugs with sharing colors between ddv and cn3d
*
* Revision 6.93  1999/12/08 22:58:00  thiessen
* added quality settings for OpenGL rendering
*
* Revision 6.92  1999/12/07 15:45:54  thiessen
* fonts working in OpenGL on Mac
*
* Revision 6.91  1999/12/06 14:43:59  thiessen
* made OpenGL font selection work in X/Motif
*
* Revision 6.90  1999/12/03 15:54:47  thiessen
* added font styles and prettified OpenGL label panel
*
* Revision 6.89  1999/12/02 23:56:52  thiessen
* added font selection for OpenGL/Win32 labels
*
* Revision 6.88  1999/11/30 22:32:08  vakatov
* Changed "#if _OPENGL" to "#ifdef _OPENGL" to suit Borland 5.0 compiler
*
* Revision 6.87  1999/11/22 17:29:54  lewisg
* add back color selection dialogs, fix viewer3d half-selection bug
*
* Revision 6.86  1999/11/18 00:21:42  lewisg
* draw speedups and selection on mouseup
*
* Revision 6.85  1999/11/15 18:30:07  lewisg
* get rid of extra redraws when selecting
*
* Revision 6.84  1999/11/10 23:19:39  lewisg
* rewrite of selection code for ddv
*
* Revision 6.83  1999/11/08 19:44:35  thiessen
* fix for OpenGL cylinder transformation
*
* Revision 6.82  1999/11/08 16:37:54  thiessen
* use 3-d brick instead of quad (OpenGL)
*
* Revision 6.81  1999/11/04 13:24:47  thiessen
* fixed bad connection menu setting
*
* Revision 6.80  1999/11/03 16:59:16  thiessen
* added capped cylinders for 'helix' object
*
* Revision 6.79  1999/11/03 16:24:19  lewisg
* get rid of non-existing includes
*
* Revision 6.78  1999/11/01 22:10:26  lewisg
* add ability to call color functions by type, and add this to cn3d
*
* Revision 6.77  1999/11/01 14:03:07  thiessen
* fixed bug in viewer3d version of wire worm
*
* Revision 6.76  1999/10/31 22:37:35  thiessen
* added worm render type to UI
*
* Revision 6.75  1999/10/29 14:15:26  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.74  1999/10/26 13:38:15  thiessen
* minor fix to worm calculation
*
* Revision 6.73  1999/10/15 20:44:23  thiessen
* minor bug fix to worm display
*
* Revision 6.72  1999/10/15 18:27:22  thiessen
* put in splined 'worm' model for virtual BB
*
* Revision 6.71  1999/10/05 23:18:18  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.70  1999/09/21 18:09:16  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 6.69  1999/09/21 13:59:21  thiessen
* port of Lewis's OpenGL code to X/Motif
*
* Revision 6.68  1999/05/11 17:54:17  ywang
* move Cn3d_AnyPrim out of OpenGL ifdef block in RenderAnAtom
*
* Revision 6.67  1999/05/05 21:12:57  ywang
* fix typing error bug in RenderGraph
*
* Revision 6.66  1999/04/14 16:40:00  ywang
* color objects by secondary structure for strucseqs
*
* Revision 6.65  1999/04/06 22:27:38  lewisg
* fix endless loop
*
* Revision 6.64  1999/04/06 16:06:10  coremake
* Fixes of typos
*
* Revision 6.63  1999/04/06 14:23:29  lewisg
* add opengl replacement for viewer3d
*
* Revision 6.62  1999/03/30 22:36:20  ywang
* add functions to color salsa for NcbiMimeAsn1_strucseqs & code reorganization
*
* Revision 6.61  1999/03/22 22:41:53  ywang
* redesign feature page, fix bugs
*
* Revision 6.60  1999/03/09 18:12:56  lewisg
* turn on termini labels
*
* Revision 6.59  1999/03/08 21:16:22  ywang
* initialize variables
*
* Revision 6.58  1999/03/01 20:21:31  ywang
* put in options for residue on/off control
*
* Revision 6.57  1999/02/24 23:06:37  ywang
* minor name change for MGD feature
*
* Revision 6.56  1999/02/10 23:49:41  lewisg
* use RGB values instead of indexed palette
*
* Revision 6.55  1999/01/27 21:51:51  ywang
* add label to 'Model' menu
*
* Revision 6.54  1999/01/26 17:14:35  ywang
* redesign Display menu and add 'display highlight residues only' function
*
* Revision 6.53  1999/01/20 22:57:24  ywang
* customize color for secondary structure & rearrange Option menu
*
 * Revision 6.52  1998/12/23  19:40:56  ywang
 * fix layer bug while drawing user-defined feature
 *
 * Revision 6.51  1998/12/16  19:44:00  kans
 * improvements to labels - combine name, letter, pdb controls into single popup
 *
* Revision 6.50  1998/12/16 01:32:08  kans
* first pass at improving label controls
*
* Revision 6.49  1998/12/14 18:39:55  kans
* enumerated alists to limit render and color choices
*
* Revision 6.48  1998/12/10 15:51:07  kans
* improved layout of render controls, safe set value/status to avoid flicker
*
* Revision 6.47  1998/12/10 01:32:06  kans
* first pass at improved render dialog
*
* Revision 6.46  1998/11/09 22:03:32  ywang
* fix bugs for modeling
*
 * Revision 6.45  1998/11/06  23:01:06  ywang
 * fix bugs for modeling
 *
 * Revision 6.44  1998/11/04  00:06:21  ywang
 * add function for modeling: change render/color for special residue(s)
 *
 * Revision 6.43  1998/10/28  19:02:46  kans
 * included salparam.h
 *
* Revision 6.42  1998/10/27 15:56:55  ywang
* add functions for testing color by sequence conservation
*
 * Revision 6.41  1998/10/21  21:18:00  ywang
 * color salsa by cycle_molecule  and by residue
 *
 * Revision 6.40  1998/10/21  15:51:28  ywang
 * reset residue color for salsa before cn3d redraws so that residues shown in salsa will become black if they are not shown in cn3d window
 *
 * Revision 6.39  1998/10/01  21:55:40  ywang
 * put function for object display control
 *
 * Revision 6.38  1998/09/30  22:10:47  ywang
 * control display on three levels: structure, chain, domain
 *
 * Revision 6.37  1998/09/24  15:52:49  kans
 * included cn3dmodl.h
 *
* Revision 6.36  1998/09/23 22:04:04  ywang
* synchronize show/hide between cn3d and salsa when display complexity is changed
*
 * Revision 6.35  1998/09/23  18:38:51  ywang
 * add functions to control display on domain level
 *
 * Revision 6.34  1998/09/22  17:53:46  ywang
 * add display control on MM level
 *
 * Revision 6.33  1998/09/17  20:53:51  ywang
 * set default color display as by conservation for struc+align case and replace the 'magenta' color with 'red'
 *
 * Revision 6.32  1998/08/26  18:28:35  kans
 * fixed -v -fd warnings
 *
* Revision 6.31  1998/07/29 22:17:36  lewisg
* change color of 1st aligned structure
*
* Revision 6.30  1998/07/28 22:17:39  lewisg
* turned on messaging when apply button pressed in render pane
*
* Revision 6.29  1998/07/28 21:14:17  lewisg
* renamed some items in the color by dropdown
*
* Revision 6.28  1998/07/09 21:50:23  ywang
* get rid of printf
*
 * Revision 6.26  1998/07/08  16:16:10  ywang
 * fix rendering bug for P backbone only model
 *
 * Revision 6.25  1998/07/07  19:31:59  ywang
 * fix rendering problem for model with Alpha Carbon onlyalgorend.c
 *
 * Revision 6.24  1998/07/02  15:05:17  lewisg
 * fix bug with resetting presentation on null structure
 *
* Revision 6.23  1998/06/16 18:00:23  lewisg
* moved rendering menus and created a reset presentation menu item
*
* Revision 6.22  1998/05/29 19:27:09  ywang
* fix potential bug
*
 * Revision 6.19  1998/05/26  21:51:00  ywang
 * limit color message
 *
 * Revision 6.18  1998/05/26  21:35:14  lewisg
 * added defaults to render menu, got rid of mouse 3D actions menu item
 *
* Revision 6.17  1998/05/22 22:23:21  lewisg
* always show heterogens
*
* Revision 6.16  1998/05/12 21:46:59  lewisg
* stricter conservation coloring
*
* Revision 6.15  1998/05/08 17:30:15  lewisg
* new conservation coloring
*
* Revision 6.14  1998/05/05 20:05:15  ywang
* add color by Protein to salsa
*
 * Revision 6.12  1998/04/30  15:23:40  ywang
 * call ColorSalsa from RenderGraph
 *
 * Revision 6.11  1998/04/28  22:47:16  lewisg
 * master/slave color in sync
 *
* Revision 6.10  1998/04/27 17:50:01  lewisg
* added color by conservation
*
* Revision 6.9  1998/04/21 23:00:51  lewisg
* added show aligned/unaligned
*
* Revision 6.8  1998/04/20 22:08:57  lewisg
* got rid of dead code
*
* Revision 6.7  1998/04/20 18:36:06  lewisg
* moved extern for Viewer3d to cn3dmain.h
*
* Revision 6.6  1998/04/18 00:33:46  lewisg
* added ability to turn slaves on/off
*
* Revision 6.5  1998/04/15 00:51:29  lewisg
* bug fixes for multiple alignment mode and alignment pane
*
* Revision 6.4  1998/04/06 04:25:20  lewisg
* added color by alignment
*
* Revision 6.3  1998/04/01 23:26:08  lewisg
* added new startup mode + fixed slave rendering
*
* Revision 6.2  1998/03/06 23:19:17  lewisg
* codewarrior fixes
*
* Revision 6.1  1998/03/06 01:16:43  lewisg
* merge
*
* Revision 6.0  1997/08/25 18:13:21  madden
* Revision changed to 6.0
*
* Revision 5.5  1996/08/19 21:04:29  vakatov
* Made functions of type "pNodeFunc" to be of that type(sharp)
* (and thus fixed fatal bug under Borland/WIN32); removed all
* castings to (pNodeFunc) -- to let compiler to check for such bugs
*
 * Revision 5.4  1996/08/06  16:04:06  hogue
 * Fixed a mistaken ==
 *
 * Revision 5.3  1996/07/31  18:35:50  hogue
 * Selection and segment-grouping to atom nodes added.
 *
 * Revision 5.2  1996/07/22  00:24:10  hogue
 * Added an origin 3D item for no-primitives condition and general use.
 *
 * Revision 5.1  1996/06/03  19:51:20  vakatov
 * VIEWSCALE decreased (caused "long" overfloat in the 3D drawing functions)
 *
 * Revision 5.0  1996/05/28  14:05:44  ostell
 * Set to revision 5.0
 *
 * Revision 1.13  1996/05/23  14:32:34  hogue
 * Made RenderLabel static for Codewarrior
 *
 * Revision 1.12  1996/05/22  22:10:34  kans
 * changed NULL to '\0'
 *
 * Revision 1.11  1996/05/22  21:46:25  hogue
 * Added White button to label controls.
 *
 * Revision 1.10  1996/05/22  20:48:53  hogue
 * Debugged the label controls, added some label features.
 *
 * Revision 1.9  1996/05/22  15:55:20  hogue
 * Changed label control panel, many other changes to support label drawing.
 * Also changed some FloatHi's to FloatLo's
 *
 * Revision 1.8  1996/05/14  15:19:41  hogue
 * Added label contols - but are not hooked up yet.
 *
 * Revision 1.7  1996/05/09  15:40:40  hogue
 * Domain rendering enabled.
 *
 * Revision 1.6  1996/04/26  21:44:48  vakatov
 * just casting...
 *
 * Revision 1.5  1996/04/26  18:42:08  vakatov
 * CN3D sources ported to MS-Windows;
 * the portability errors and warnings fixed, etc.
 *
*
* ==========================================================================
*/
#ifdef _OPENGL
#include <shim3d.h>
#else
#include <viewer3d.h>
#endif
#include <cn3dmain.h>
#include <math.h>
#include <mmdbapi.h>
#include <algorend.h>
#include <cn3dmsg.h>
#include <saledit.h>
#include <salparam.h>
#include <cn3dshim.h>
#include <cn3dmesh.h>
#include <seqcons.h>
#include <cn3dmodl.h>
#include <cn3dstyl.h>
#include <alignmgr.h>


static ENUM_ALIST(empty_alist)
  {"              ",     1},
END_ENUM_ALIST

static ENUM_ALIST(all_render_alist)
  {"Wireframe",          1},
  {"Tubes",              2},
  {"Ball & Stick",       3},
  {"Fat Tubes",          4},
  {"Space Fill   ",      5},
END_ENUM_ALIST

static ENUM_ALIST(backbone_render_alist)
  {"Wireframe",          1},
  {"Tubes",              2},
  {"Ball & Stick",       3},
  {"Fat Tubes",          4},
  {"Space Fill   ",      5},
  {"Wire Worm",          6}, /* leave worms at end of list - easier to test for */
#ifdef _OPENGL
  {"Worm",               7},
  {"Fat Worm",           8},
#endif
END_ENUM_ALIST

static ENUM_ALIST(ions_render_alist)
  {"Fat Tubes",          4},
  {"Space Fill   ",      5},
END_ENUM_ALIST

static ENUM_ALIST(connections_render_alist)
  {"Wireframe",          1},
  {"Tubes",              2},
  {"Ball & Stick",       3},
END_ENUM_ALIST

static ENUM_ALIST(solvents_render_alist)
  {"Ball & Stick",       3},
  {"Fat Tubes",          4},
  {"Space Fill   ",      5},
END_ENUM_ALIST

static ENUM_ALIST(object_arrow_alist)
  {"No Arrows",         0},
  {"Strand Arrows",     ARROW_BRICK},
  {"Helix Arrows",      ARROW_CYL},
  {"Arrows on Both",    (ARROW_BRICK | ARROW_CYL)},
END_ENUM_ALIST


/* color popup names are now being set according to Cn3D_ColorList; for now
   these will continue to define the list of appropriate color functions for
   each object type */
static ENUM_ALIST(all_color_alist)
  {"",  C_BYOBJECT},
  {"",  C_BYCHAIN},
  {"",  C_BYDOMAIN},
  {"",  C_BYSECSTRUC},
  {"",  C_BYSTRUCALIGN},
  {"",  C_BYSEQCONS},
  {"",  C_BYRES},
  {"",  C_BYHYDRO},
  {"",  C_CPK},
  {"",  C_BYTEMP},
END_ENUM_ALIST

static ENUM_ALIST(objects_color_alist)
  {"",  C_BYSECSTRUC},
  {"",  C_BYOBJECT},
  {"",  C_BYCHAIN},
  {"",  C_BYDOMAIN},
END_ENUM_ALIST

static ENUM_ALIST(ions_color_alist)
  {"",  C_CPK},
  {"",  C_BYOBJECT},
  {"",  C_BYTEMP},
END_ENUM_ALIST

static ENUM_ALIST(solvents_color_alist)
  {"",  C_CPK},
  {"",  C_BYOBJECT},
  {"",  C_BYTEMP},
END_ENUM_ALIST

static ENUM_ALIST(nt_color_alist)
  {"",  C_BYOBJECT},
  {"",  C_BYCHAIN},
  {"",  C_CPK},
  {"",  C_BYTEMP},
END_ENUM_ALIST

static ENUM_ALIST(heterogen_color_alist)
  {"",  C_CPK},
  {"",  C_BYOBJECT},
  {"",  C_BYTEMP},
END_ENUM_ALIST

#ifdef _OPENGL
static ENUM_ALIST(bgColorAList)
  {"Black",     1},
  {"White",     2},
  {"Select...", 3},
END_ENUM_ALIST
#endif

/*define DEBUG_N 1 */
#ifndef _OPENGL
static Picture3D pic = NULL;
#endif

static Int2 Cn3d_RenderNow = 0;
static Uint1 Cn3d_LabelNow = 1;
static Int2 Cn3d_ColorNow = 0;
static Int1 Cn3d_LayerNow = 0;
static Boolean Cn3d_DoHydrogens = FALSE;
static Boolean Cn3d_CopyToNode = FALSE;
static Boolean Cn3d_ColorPass = FALSE; /* gathering unique colors for palette */
static Boolean Cn3d_AnyPrim = FALSE;
static int Cn3d_lSlaveNum = 0;  /* which slave being iterated over 0=master */

static PopuP Cn3D_pupPBB;       /* protein backbone options and on/off */
static PopuP Cn3D_pupNABB;      /* nucl. acid bb options and on/off */
static ButtoN Cn3D_lOnOffItem[10]; /* pieces parts to draw - first 2 empty */

static PopuP Cn3D_pupRenderStyle[10];
static PopuP Cn3D_pupColorStyle[10];

#ifdef _OPENGL
static PopuP Cn3D_pupBackgroundColor;
#endif

static EnumFieldAssocPtr renderAlist[10] = {
    backbone_render_alist, backbone_render_alist,
    all_render_alist, all_render_alist,
    all_render_alist, ions_render_alist, connections_render_alist,
    solvents_render_alist, object_arrow_alist, empty_alist
};
static EnumFieldAssocPtr colorAlist[10] = {
    all_color_alist, nt_color_alist, all_color_alist, nt_color_alist,
    heterogen_color_alist, ions_color_alist, empty_alist,
    solvents_color_alist,
    objects_color_alist, empty_alist
};
static Int2 renderDefaults[10] = { 1, 1, 1, 1, 1, 5, 1, 5, ARROW_BRICK, 1 };
static Int2 colorDefaults[10] = { C_BYOBJECT, C_BYOBJECT, C_BYOBJECT, C_BYOBJECT, C_BYOBJECT, 
                                  C_BYOBJECT, C_BYOBJECT, C_BYOBJECT, C_BYOBJECT, C_BYOBJECT };


static ButtoN Cn3D_bRedraw;     /* the button to redraw */
static ButtoN Cn3D_immediateRedraw;

static ButtoN Cn3D_lOnOffLabel[4];
static PopuP Cn3D_pupLabelAA;
static PopuP Cn3D_pupLabelNT;
static PopuP Cn3D_pupLResName[2];
static ButtoN Cn3D_bLTermName[2];
static ButtoN Cn3D_bLNum[4];
static ButtoN Cn3D_bLWhite[4];

#ifndef _OPENGL
static ButtoN Cn3D_bLTop[4];
static PopuP Cn3D_pupLabelSize[4];
#else
static ButtoN Cn3D_bIonLabel;
static PopuP Cn3D_pupLabelFont;
static PopuP Cn3D_pupLabelPoints;
static ButtoN Cn3D_bLFontBold, Cn3D_bLFontItalic;
#ifndef WIN_MOTIF
static ButtoN Cn3D_bLFontUnderlined;
#endif

static void Nlm_GetCurrentOGLFontMenuSettings(Nlm_Int2 *font, Nlm_Int2 *size,
    Nlm_Boolean *isBold, Nlm_Boolean *isItalic, Nlm_Boolean *isUnderlined)
{
    *font = GetValue(Cn3D_pupLabelFont);
    *size = GetValue(Cn3D_pupLabelPoints);
    *isBold = GetStatus(Cn3D_bLFontBold);
    *isItalic = GetStatus(Cn3D_bLFontItalic);
#ifndef WIN_MOTIF
    *isUnderlined = GetStatus(Cn3D_bLFontUnderlined);
#else
    *isUnderlined = 0;
#endif
}

static void Cn3D_SetOGLFont(TOGL_Data *OGL_Data)
{
    static Nlm_Int2 currentIndex = -1, currentSize = -1;
    static Nlm_Boolean currentBold = FALSE, currentItalic = FALSE,
                       currentUnderlined = FALSE;
    Nlm_Int2 fontNameIndex, fontSize;
    Nlm_Boolean isBold, isItalic, isUnderlined;

    Nlm_GetCurrentOGLFontMenuSettings(&fontNameIndex, &fontSize, &isBold,
                                      &isItalic, &isUnderlined);
    SetOGLFont(OGL_Data, fontNameIndex, fontSize, isBold,
                                      isItalic, isUnderlined);
}


#endif

static ButtoN Cn3D_bLRedraw;
static ButtoN Cn3D_LimmediateRedraw;

#ifdef _OPENGL
/* for Quality panel */
#define QUAL_SIZE_MIN 4
#define QUAL_SIZE_MAX 30
static PopuP Cn3D_pupQualBigCylSides;
static PopuP Cn3D_pupQualBondCylSides;
static PopuP Cn3D_pupQualSphSlices;
static PopuP Cn3D_pupQualSphStacks;
static PopuP Cn3D_pupQualWormSegments;
static PopuP Cn3D_pupQualWormSides;
static ButtoN Cn3D_bQualOverall[3];
static ButtoN Cn3D_bQRedraw;
static ButtoN Cn3D_QimmediateRedraw;
static PopuP Cn3D_pTick;
#endif

extern CharPtr NCBIstdaaUC;
extern CharPtr NCBI4naUC;
extern CharPtr NCBI4naLC;
extern CharPtr NCBIstdaaLC;

extern Int1 KinAAColor[];
extern Int2 KinNAColor[];
extern Int1 KinAtoms[];
extern Int1 ColorNumKinBB[];
extern Int1 ColorNumKinSC[];
extern Int1 ColorNumKinAC[];
extern CharPtr KineColors[];
extern Int1 ElementKinColors[];
extern Int1 ThermKine[];
extern Int4 TempsKine[];

static Uint1 Cn3D_UnalignedResidueColor[3] = { 175, 175, 175 };

/* the number of colors available to color aligned structures */
#define NUM_SLAVES 10
static Int1 bObjectColors[NUM_SLAVES] = /* object colors */
    { C_magenta, C_blue, C_brown, C_sea, C_gray, C_gold, C_pink, C_green, C_cyan, C_yellowtint };

static Int1 PhobeAAColor[MAX_NCBIstdaa] = { 20, 37, 38, 39, 39, 40, 41,
39, 42, 43, 44, 45, 41, 53, 46, 47, 48, 49, 48, 50, 51, 20, 52, 40, 39, 20 }; 


/**************************************/
/* stuff for conf-ensemble separation */
/**************************************/

#define MAXALTCONF 40
static Nlm_Int2 Cn3d_nEnsembles = 0, Cn3d_currentEnsemble;
static Nlm_Char Cn3d_altConfIDs[MAXALTCONF][MAXALTCONF];

/* find the PALD structure with the current cAltConf, if ensembles are on */
static PALD GetAtomAltLocs(PMAD pmadThis, Nlm_Int2 iModel)
{
    PALD pald, paldHead;
    PMMD pmmdThis;
    
    paldHead = pald = GetAtomLocs(pmadThis, iModel);
    if (!paldHead) return NULL;
    if (!Cn3d_nEnsembles) return pald;
    while (pald) {
        if (StrChr(Cn3d_altConfIDs[Cn3d_currentEnsemble], pald->cAltConf))
            return pald;
        pald = pald->next;
    }
    /* if not found, decide whether to return nothing, or just any 'ole PALD */
#define QUERY (pmmdThis->bWhat)
    pmmdThis = GetParentMol((PFB) paldHead);
    if ((QUERY & AM_SOL) || (QUERY & AM_WAT) ||
        !allowAltConfIdOverlay())
        return NULL;
    else
        return paldHead;
#undef QUERY
}

/* crudely ensures that the above function is always used as a replacement
   for GetAtomLocs in this file, so alternate conformation models will
   behave properly; generates a compiler error if GetAtomLocs is used */
#define GetAtomLocs DONT_USE_GetAtomLocs_USE_GetAtomAltLocs_IN_ALGOREND_C


/***********************************/

static DDV_ColorCell *LIBCALL ColorFromSS(PMGD pmgdThis)
{
    Int2 rsult = 0;

    switch ((int) (pmgdThis->bNCBISecStru)) {
    case SS_HELIX:
        return DDV_SearchColorCellbyName(Cn3D_ColorData.pDDVColorGlobal->
                                         pvnSpecialColors, "Helix");
    case SS_SHEET:
    case SS_STRAND:
        return DDV_SearchColorCellbyName(Cn3D_ColorData.pDDVColorGlobal->
                                         pvnSpecialColors, "Strand");
    case SS_TURN:
        return DDV_SearchColorCellbyName(Cn3D_ColorData.pDDVColorGlobal->
                                         pvnSpecialColors, "Turn");
    default:
        return DDV_SearchColorCellbyName(Cn3D_ColorData.pDDVColorGlobal->
                                         pvnSpecialColors, "Coil");
    }
    return NULL;

}

static void SetStructureRenderSettings(PARS pars)
{
    if (!pars)
        return;

    pars->HydrogensOn = FALSE;

    pars->PVirtualBBOn = TRUE;
    pars->PPartialBBOn = FALSE;
    pars->PCompleteBBOn = FALSE;
#ifdef _OPENGL
    pars->PBBRender = R_WORMTHIN;
#else
    pars->PBBRender = R_WORMWIRE;
#endif
    pars->PResiduesOn = FALSE;
    pars->PResRender = R_THICKWIRE;

    pars->NTVirtualBBOn = TRUE;
    pars->NTPartialBBOn = FALSE;
    pars->NTCompleteBBOn = FALSE;
#ifdef _OPENGL
    pars->NTBBRender = R_WORMFAT;
#else
    pars->NTBBRender = R_THICKWIRE;
#endif
    pars->NTResiduesOn = FALSE;
    pars->NTResRender = R_WIRE;

    pars->HeterogensOn = TRUE;
    pars->HetRender = R_THICKWIRE;
    pars->IonsOn = TRUE;
    pars->IonRender = R_SPACE;
    pars->ConnectOn = FALSE;
    pars->ConnectRender = R_STICK;
    pars->SolventOn = FALSE;
    pars->SolventRender = R_BALLNSTICK;
    pars->ObjectOn = TRUE;
    pars->ObjectArrows = ARROW_BRICK;
}

static void SetStructureColorSettings(PARS pars)
{
    if (!pars)
        return;

    pars->PBBColor = C_BYSECSTRUC;
    pars->PResColor = C_BYSECSTRUC;

    pars->NTBBColor = C_CPK;
    pars->NTResColor = C_CPK;

    pars->HetColor = C_CPK;
    pars->IonColor = C_CPK;
    pars->ConnectColor = 9;     /* all same color?? */
    pars->SolventColor = C_CPK;
    pars->ObjectColor = C_BYSECSTRUC;

    pars->BGColRGB[0] = Cn3d_PaletteRGB[C_black][0];
    pars->BGColRGB[1] = Cn3d_PaletteRGB[C_black][1];
    pars->BGColRGB[2] = Cn3d_PaletteRGB[C_black][2];
}

static void SetStructureLabelSettings(PARS pars)
{
    if (!pars)
        return;

    pars->PBBLabelInterval = 1;
    pars->PBBLabelJust = (Uint1) (LA_CENTER);
    pars->PBBLabelStyle = (Uint1) L_NAME | L_NUM | L_3LETR;
    pars->PBBLabelScale = 2;

    pars->PTermLabelOn = FALSE;
    pars->PTermLabelJust = (Uint1) (LA_CENTER);
    pars->PTermLabelStyle = (Uint1) L_NAME;
    pars->PTermLabelScale = 4;

    pars->IonLabelOn = TRUE;

    pars->NTBBLabelInterval = 1;
    pars->NTBBLabelJust = (Uint1) (LA_CENTER);
    pars->NTBBLabelStyle = (Uint1) L_NAME | L_NUM | L_1LETR;
    pars->NTBBLabelScale = 2;

    pars->NTTermLabelOn = FALSE;
    pars->NTTermLabelJust = (Uint1) (LA_CENTER);
    pars->NTTermLabelStyle = (Uint1) L_NAME;
    pars->NTTermLabelScale = 4;
}

void LIBCALL SetStructureAlgorRender(PARS pars)
{
    if (!pars)
        return;

    SetStructureRenderSettings(pars);
    SetStructureColorSettings(pars);
    SetStructureLabelSettings(pars);
}

static void SetAlignRenderSettings(PARS pars)
{
    if (!pars)
        return;

    pars->HydrogensOn = FALSE;

    pars->PVirtualBBOn = TRUE;
    pars->PPartialBBOn = FALSE;
    pars->PCompleteBBOn = FALSE;
    pars->PBBRender = R_THICKWIRE;
    pars->PResiduesOn = FALSE;
    pars->PResRender = R_WIRE;

    pars->NTVirtualBBOn = FALSE;
    pars->NTPartialBBOn = FALSE;
    pars->NTCompleteBBOn = TRUE;
    pars->NTBBRender = R_WIRE;
    pars->NTResiduesOn = TRUE;
    pars->NTResRender = R_WIRE;

    pars->HeterogensOn = TRUE;
    pars->HetRender = R_THICKWIRE;
    pars->IonsOn = TRUE;
    pars->IonRender = R_SPACE;
    pars->ConnectOn = FALSE;
    pars->ConnectRender = R_STICK;
    pars->SolventOn = FALSE;
    pars->SolventRender = R_BALLNSTICK;
    pars->ObjectOn = FALSE;
    pars->ObjectArrows = ARROW_BRICK;
}

static void SetAlignColorSettings(PARS pars)
{
    if (!pars)
        return;

    pars->PBBColor = C_BYSTRUCALIGN;
    pars->PResColor = C_BYSTRUCALIGN;

    pars->NTBBColor = C_CPK;
    pars->NTResColor = C_CPK;

    pars->HetColor = C_CPK;
    pars->IonColor = C_CPK;
    pars->ConnectColor = 9;     /* all same color?? */
    pars->SolventColor = C_CPK;
    pars->ObjectColor = C_BYOBJECT;

    pars->BGColRGB[0] = Cn3d_PaletteRGB[C_black][0];
    pars->BGColRGB[1] = Cn3d_PaletteRGB[C_black][1];
    pars->BGColRGB[2] = Cn3d_PaletteRGB[C_black][2];
}

static void SetAlignLabelSettings(PARS pars)
{
    if (!pars)
        return;

    pars->PBBLabelInterval = 1;
    pars->PBBLabelJust = (Uint1) (LA_CENTER);
    pars->PBBLabelStyle = (Uint1) L_NAME | L_NUM | L_3LETR;
    pars->PBBLabelScale = 2;

    pars->PTermLabelOn = FALSE;
    pars->PTermLabelJust = (Uint1) (LA_CENTER);
    pars->PTermLabelStyle = (Uint1) L_NAME;
    pars->PTermLabelScale = 4;

    pars->IonLabelOn = TRUE;

    pars->NTBBLabelInterval = 1;
    pars->NTBBLabelJust = (Uint1) (LA_CENTER);
    pars->NTBBLabelStyle = (Uint1) L_NAME | L_NUM | L_1LETR;
    pars->NTBBLabelScale = 2;

    pars->NTTermLabelOn = FALSE;
    pars->NTTermLabelJust = (Uint1) (LA_CENTER);
    pars->NTTermLabelStyle = (Uint1) L_NAME;
    pars->NTTermLabelScale = 4;
}

void LIBCALL SetAlignAlgorRender(PARS pars)
{
    if (!pars)
        return;
    
    SetAlignRenderSettings(pars);
    SetAlignColorSettings(pars);
    SetAlignLabelSettings(pars);
}


PARS LIBCALL NewStructureRenderSet(void)
{
    PARS par = NULL;
    par = (PARS) MemNew((size_t) (sizeof(ARS)));
    if (!par)
        return NULL;
    SetStructureAlgorRender(par);
    return par;
}

PARS LIBCALL NewAlignRenderSet(void)
{
    PARS par = NULL;
    par = (PARS) MemNew((size_t) (sizeof(ARS)));
    if (!par)
        return NULL;
    SetAlignAlgorRender(par);
    return par;
}

void LIBCALL FreeAlgorRenderSet(PARS pars)
{
    MemFree(pars);
}



static void MasterChangeRenderProc(PopuP p)
{
    Int2 i, j, k;
    PARS pars = NULL;
    PMSD pmsdThis = NULL;
    PDNMS pdnmsThis = NULL;
    UIEnum val;
    UIEnum eSetting;
#ifdef _OPENGL
    Uint1 r, g, b;
#endif

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis) return;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    pars = (PARS) pmsdThis->pGlobalPARS;
    if (!pars) return;

    /* deal with mutually exclusive menu settings first */
    GetEnumPopup(Cn3D_pupRenderStyle[0], renderAlist[0], &eSetting);
    i = GetValue(Cn3D_pupPBB);
    if ((i == 2 || i == 3) && eSetting >= 6) {
        Message(MSG_ERROR, "Worm rendering can only be used for C-alpha-only backbone");
        SafeSetValue(Cn3D_pupPBB, 1);
    }
    GetEnumPopup(Cn3D_pupRenderStyle[1], renderAlist[1], &eSetting);
    i = GetValue(Cn3D_pupNABB);
    if ((i == 2 || i == 3) && eSetting >= 6) {
        Message(MSG_ERROR, "Worm rendering can only be used for P-only backbone");
        SafeSetValue(Cn3D_pupNABB, 1);
    }


    pars->PResiduesOn = GetStatus(Cn3D_lOnOffItem[2]);
    pars->NTResiduesOn = GetStatus(Cn3D_lOnOffItem[3]);
    pars->HeterogensOn = GetStatus(Cn3D_lOnOffItem[4]);
    pars->IonsOn = GetStatus(Cn3D_lOnOffItem[5]);
    pars->ConnectOn = GetStatus(Cn3D_lOnOffItem[6]);
    pars->SolventOn = GetStatus(Cn3D_lOnOffItem[7]);
    pars->ObjectOn = GetStatus(Cn3D_lOnOffItem[8]);
    pars->HydrogensOn = GetStatus(Cn3D_lOnOffItem[9]);

    pars->PVirtualBBOn = FALSE;
    pars->PPartialBBOn = FALSE;
    pars->PCompleteBBOn = FALSE;

    i = GetValue(Cn3D_pupPBB);
    switch (i) {
    case 1:
        pars->PVirtualBBOn = TRUE; /* alpha c trace */
        break;
    case 2:
        pars->PPartialBBOn = TRUE; /* partial atoms */
        break;
    case 3:
        pars->PCompleteBBOn = TRUE; /* all atoms */
        break;
    default:;                  /* none */
    }

    pars->NTVirtualBBOn = FALSE;
    pars->NTPartialBBOn = FALSE;
    pars->NTCompleteBBOn = FALSE;

    j = GetValue(Cn3D_pupNABB);
    switch (j) {
    case 1:
        pars->NTVirtualBBOn = TRUE;
        break;
    case 2:
        pars->NTPartialBBOn = TRUE;
        break;
    case 3:
        pars->NTCompleteBBOn = TRUE;
        break;
    default:;
    }

    for (i = 0; i < 9; i++) {
        if (i == 6) {
            pars->ConnectColor = C_yellow;
            continue;
        }
        GetEnumPopup(Cn3D_pupColorStyle[i], colorAlist[i], &val);
        switch (i) {
        case 0:                /* prot bb */
            pars->PBBColor = val;
            break;
        case 1:                /* na bb */
            pars->NTBBColor = val;
            break;
        case 2:                /* prot sc */
            pars->PResColor = val;
            break;
        case 3:                /* na sc */
            pars->NTResColor = val;
            break;
        case 4:                /* hets */
            pars->HetColor = val;
            break;
        case 5:                /* ions */
            pars->IonColor = val;
            break;
        case 7:                /* solvent */
            pars->SolventColor = val;
            break;
        case 8:                /* object */
            pars->ObjectColor = val;
            break;
        default:
            ;
        }
    }

    for (i = 0; i < 9; i++) {
        if (GetEnumPopup(Cn3D_pupRenderStyle[i], renderAlist[i], &val)) {
            j = (Int2) val;
        } else {
            j = 1;
        }

        if (i == 8) { /* different values for 3d-object arrow settings */
            k = j;
        } else {
            switch (j) {
            case 1:
                k = R_WIRE;
                break;
            case 2:
                k = R_STICK;
                break;
            case 3:
                k = R_BALLNSTICK;
                break;
            case 4:
                k = R_THICKWIRE;
                break;
            case 5:
                k = R_SPACE;
                break;
            case 6:
                k = R_WORMWIRE;
                break;
#ifdef _OPENGL
            case 7:
                k = R_WORMTHIN;
                break;
            case 8:
                k = R_WORMFAT;
                break;
#endif
            default:
                ;
            }
        }

        switch (i) {
        case 0:                /* prot bb */
            pars->PBBRender = k;
            break;
        case 1:                /* na bb */
            pars->NTBBRender = k;
            break;
        case 2:                /* prot sc */
            pars->PResRender = k;
            break;
        case 3:                /* na sc */
            pars->NTResRender = k;
            break;
        case 4:                /* hets */
            pars->HetRender = k;
            break;
        case 5:                /* ions */
            pars->IonRender = k;
            break;
        case 6:                /* connections */
            pars->ConnectRender = k;
            break;
        case 7:                /* solvent */
            pars->SolventRender = k;
            break;
        case 8:                /* object */
            pars->ObjectArrows = k;
            break;
        default:
            ;
        }
    }
  
#ifdef _OPENGL
    if (GetEnumPopup(Cn3D_pupBackgroundColor, bgColorAList, &val)) {
        j = (Int2) val;
    } else {
        j = 1;
    }
    switch (j) {
    case 1:
        k = C_black;
        break;
    case 2:
        k = C_white;
        break;
    case 3:
    default:
        k = -1;
    }
    if (k == -1) {
        r = pars->BGColRGB[0];
        g = pars->BGColRGB[1];
        b = pars->BGColRGB[2];
        if (p == Cn3D_pupBackgroundColor)
            Nlm_ChooseColorDialog(&r, &g, &b, TRUE);
    } else {
        r = Cn3d_PaletteRGB[k][0];
        g = Cn3d_PaletteRGB[k][1];
        b = Cn3d_PaletteRGB[k][2];
    }
    pars->BGColRGB[0] = r;
    pars->BGColRGB[1] = g;
    pars->BGColRGB[2] = b;
#endif
    
    if (GetStatus(Cn3D_immediateRedraw)) {
        fnCn3D_RedrawWrapper(Cn3D_bRedraw);
    }
}

static void ButtonOnOffProc(ButtoN b)
 {
    MasterChangeRenderProc(NULL);
}

static void PopupOnOffProc(PopuP p)
 {
    MasterChangeRenderProc(p);
}

void Cn3D_RenStruc(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    /* use "default" Structure settings */
    SetStructureRenderSettings(pars);
    
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_Redraw(FALSE);
    return;
}

void Cn3D_RenWire(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;
    PMSD pmsdThis = NULL;
    PDNML pdnmlThis = NULL;
    PMLD pmldThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->HydrogensOn = FALSE;
    pars->ObjectOn = FALSE;
    pars->SolventOn = FALSE;
    pars->PPartialBBOn = FALSE;
    pars->PVirtualBBOn = FALSE;
    pars->PCompleteBBOn = TRUE;
    pars->PResiduesOn = TRUE;
    pars->NTVirtualBBOn = FALSE;
    pars->NTPartialBBOn = FALSE;
    pars->NTResiduesOn = TRUE;
    pars->NTCompleteBBOn = TRUE;
    pars->HeterogensOn = TRUE;
    pars->IonsOn = TRUE;
    pars->ConnectOn = TRUE;
    pars->PBBRender = R_WIRE;
    pars->PResRender = R_WIRE;
    pars->NTBBRender = R_WIRE;
    pars->NTResRender = R_WIRE;
    pars->HetRender = R_WIRE;
    pars->IonRender = R_SPACE;
    pars->ConnectRender = R_WIRE;
    pars->SolventRender = R_THICKWIRE;

    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_Redraw(FALSE);
    return;
}

void Cn3D_RenTube(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->HydrogensOn = FALSE;
    pars->ObjectOn = FALSE;
    pars->SolventOn = FALSE;
    pars->PPartialBBOn = FALSE;
    pars->PVirtualBBOn = FALSE;
    pars->PCompleteBBOn = TRUE;
    pars->PResiduesOn = TRUE;
    pars->NTVirtualBBOn = FALSE;
    pars->NTPartialBBOn = FALSE;
    pars->NTResiduesOn = TRUE;
    pars->NTCompleteBBOn = TRUE;
    pars->HeterogensOn = TRUE;
    pars->IonsOn = TRUE;
    pars->ConnectOn = TRUE;
    pars->PBBRender = R_THICKWIRE;
    pars->PResRender = R_STICK;
    pars->NTBBRender = R_THICKWIRE;
    pars->NTResRender = R_STICK;
    pars->HetRender = R_THICKWIRE;
    pars->IonRender = R_SPACE;
    pars->ConnectRender = R_STICK;
    pars->SolventRender = R_BALLNSTICK;
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_Redraw(FALSE);
    return;
}


void Cn3D_RenHier(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->HydrogensOn = FALSE;
    pars->ObjectOn = FALSE;
    pars->SolventOn = FALSE;
    pars->PPartialBBOn = FALSE;
    pars->PVirtualBBOn = FALSE;
    pars->PCompleteBBOn = TRUE;
    pars->PResiduesOn = TRUE;
    pars->NTVirtualBBOn = FALSE;
    pars->NTPartialBBOn = FALSE;
    pars->NTResiduesOn = TRUE;
    pars->NTCompleteBBOn = TRUE;
    pars->HeterogensOn = TRUE;
    pars->IonsOn = TRUE;
    pars->ConnectOn = TRUE;
    pars->PBBRender = R_THICKWIRE;
    pars->PResRender = R_WIRE;
    pars->NTBBRender = R_THICKWIRE;
    pars->NTResRender = R_WIRE;
    pars->HetRender = R_STICK;
    pars->IonRender = R_SPACE;
    pars->ConnectRender = R_STICK;
    pars->SolventRender = R_BALLNSTICK;
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_Redraw(FALSE);
    return;
}

void Cn3D_RenSpace(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->HydrogensOn = TRUE;
    pars->ObjectOn = FALSE;
    pars->SolventOn = FALSE;
    pars->PPartialBBOn = FALSE;
    pars->PVirtualBBOn = FALSE;
    pars->PCompleteBBOn = TRUE;
    pars->PResiduesOn = TRUE;
    pars->NTVirtualBBOn = FALSE;
    pars->NTPartialBBOn = FALSE;
    pars->NTResiduesOn = TRUE;
    pars->NTCompleteBBOn = TRUE;
    pars->HeterogensOn = TRUE;
    pars->IonsOn = TRUE;
    pars->ConnectOn = TRUE;
    pars->PBBRender = R_SPACE;
    pars->PResRender = R_SPACE;
    pars->NTBBRender = R_SPACE;
    pars->NTResRender = R_SPACE;
    pars->HetRender = R_SPACE;
    pars->IonRender = R_SPACE;
    pars->ConnectRender = R_STICK;
    pars->SolventRender = R_BALLNSTICK;
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_Redraw(FALSE);
    return;
}

void Cn3D_RenBS(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->HydrogensOn = FALSE;
    pars->ObjectOn = FALSE;
    pars->SolventOn = FALSE;
    pars->PPartialBBOn = FALSE;
    pars->PVirtualBBOn = FALSE;
    pars->PCompleteBBOn = TRUE;
    pars->PResiduesOn = TRUE;
    pars->NTVirtualBBOn = FALSE;
    pars->NTPartialBBOn = FALSE;
    pars->NTResiduesOn = TRUE;
    pars->NTCompleteBBOn = TRUE;
    pars->HeterogensOn = TRUE;
    pars->IonsOn = TRUE;
    pars->ConnectOn = TRUE;
    pars->PBBRender = R_BALLNSTICK;
    pars->PResRender = R_BALLNSTICK;
    pars->NTBBRender = R_BALLNSTICK;
    pars->NTResRender = R_BALLNSTICK;
    pars->HetRender = R_BALLNSTICK;
    pars->IonRender = R_SPACE;
    pars->ConnectRender = R_STICK;
    pars->SolventRender = R_BALLNSTICK;
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_Redraw(FALSE);
    return;
}

/**
Render with alignment settings.
*/

void Cn3D_RenAlign(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    /* use "default" Align settings instead */
    SetAlignRenderSettings(pars);

    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_Redraw(FALSE);
    return;
}

void Cn3D_RenDefault(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;
    PMSD pmsdThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    if (pmsdThis->pdnmsSlaves)
        SetAlignAlgorRender(pars);
    else
        SetStructureAlgorRender(pars);
    ResetRenderCtrls();
    ResetDisplayCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_Redraw(FALSE);
    return;
}



void Cn3D_ColCPK(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->PBBColor = C_CPK;
    pars->PResColor = C_CPK;
    pars->NTBBColor = C_CPK;
    pars->NTResColor = C_CPK;
    pars->HetColor = C_CPK;
    pars->IonColor = C_CPK;
    pars->SolventColor = C_CPK;
    pars->ObjectColor = C_BYSECSTRUC;
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_RedrawNUpdate(FALSE);
}


void Cn3D_ColDomain(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->PBBColor = C_BYDOMAIN;
    pars->PResColor = C_BYDOMAIN;
    pars->NTBBColor = C_CPK;
    pars->NTResColor = C_CPK;
    pars->HetColor = C_CPK;
    pars->IonColor = C_CPK;
    pars->SolventColor = C_CPK;
    pars->ObjectColor = C_BYDOMAIN;
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_RedrawNUpdate(FALSE);
    return;
}


void Cn3D_ColCyChain(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->PBBColor = C_BYCHAIN;
    pars->PResColor = C_BYCHAIN;
    pars->NTBBColor = C_BYCHAIN;
    pars->NTResColor = C_BYCHAIN;
    pars->HetColor = C_CPK;
    pars->IonColor = C_CPK;
    pars->SolventColor = C_CPK;
    pars->ObjectColor = C_BYCHAIN;
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_RedrawNUpdate(FALSE);
    return;
}

void Cn3D_ColSecStruc(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    /* use "default" Structure settings */
    SetStructureColorSettings(pars);

    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_RedrawNUpdate(FALSE);
    return;
}

void Cn3D_ColObject(IteM i)
{                              
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->PBBColor = C_BYOBJECT;
    pars->PResColor = C_BYOBJECT;
    pars->NTBBColor = C_BYOBJECT;
    pars->NTResColor = C_BYOBJECT;
    pars->HetColor = C_BYOBJECT;
    pars->IonColor = C_BYOBJECT;
    pars->SolventColor = C_BYOBJECT;
    pars->ObjectColor = C_BYOBJECT;
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_RedrawNUpdate(FALSE);
    return;
}

void Cn3D_ColStrucAlign(IteM i)
{                               /* menu function for setting up color by conservation */
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    /* use "default" Align settings */
    SetAlignColorSettings(pars);

    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_RedrawNUpdate(FALSE);
    return;
}

void Cn3D_ColSeqCons(IteM i)
{                               /* menu function for setting up color by conservation */
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->ConsColAlg = (Nlm_Int2) GetObjectExtra(i);
    pars->PBBColor = C_BYSEQCONS;
    pars->PResColor = C_BYSEQCONS;

    pars->NTBBColor = C_CPK;
    pars->NTResColor = C_CPK;
    pars->HetColor = C_CPK;
    pars->IonColor = C_CPK;
    pars->SolventColor = C_CPK;
    pars->ObjectColor = C_BYSECSTRUC;

    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_RedrawNUpdate(FALSE);
    return;
}

void Cn3D_ColRes(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->PBBColor = C_BYRES;
    pars->PResColor = C_BYRES;
    pars->NTBBColor = C_CPK;
    pars->NTResColor = C_CPK;
    pars->HetColor = C_CPK;
    pars->IonColor = C_CPK;
    pars->SolventColor = C_CPK;
    pars->ObjectColor = C_BYSECSTRUC;
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_RedrawNUpdate(FALSE);
    return;
}

void Cn3D_ColHydro(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->PBBColor = C_BYHYDRO;
    pars->PResColor = C_BYHYDRO;
    pars->NTBBColor = C_CPK;
    pars->NTResColor = C_CPK;
    pars->HetColor = C_CPK;
    pars->IonColor = C_CPK;
    pars->SolventColor = C_CPK;
    pars->ObjectColor = C_BYSECSTRUC;
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_RedrawNUpdate(FALSE);
    return;
}


void Cn3D_ColTemp(IteM i)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (!pars) return;

    pars->PBBColor = C_BYTEMP;
    pars->PResColor = C_BYTEMP;
    pars->NTBBColor = C_BYTEMP;
    pars->NTResColor = C_BYTEMP;
    pars->HetColor = C_BYTEMP;
    pars->IonColor = C_BYTEMP;
    pars->SolventColor = C_BYTEMP;
    pars->ObjectColor = C_BYSECSTRUC;
    ResetRenderCtrls();
    Cn3dObjMgrGetSelected();
    Cn3D_RedrawNUpdate(FALSE);

    return;
}


void LIBCALL ResetRenderCtrls(void)
{
    Int2 i, j, k;
    PARS pars = NULL;
    PMSD pmsdThis = NULL;
    PDNMS pdnmsThis = NULL;
    PDNML pdnmlThis = NULL;
    PMLD pmldThis = NULL;


    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || 
        !(pmsdThis = (PMSD) pdnmsThis->data.ptrvalue) ||
        !(pars = (PARS) pmsdThis->pGlobalPARS)) {

        for (k = 2; k < 10; k++) {
            SafeSetStatus(Cn3D_lOnOffItem[k], FALSE);
        }
        SafeSetValue(Cn3D_pupPBB, 4);
        SafeSetValue(Cn3D_pupNABB, 4);
        for (k = 0; k < 9; k++) {
            SetEnumPopup(Cn3D_pupRenderStyle[k], renderAlist[k],
                         (UIEnum) renderDefaults[k]);
            if (k != 6)
                SetEnumPopup(Cn3D_pupColorStyle[k], colorAlist[k],
                            (UIEnum) colorDefaults[k]);
        }
#ifdef _OPENGL
        SetEnumPopup(Cn3D_pupBackgroundColor, bgColorAList, (UIEnum) 1);
#endif
        return;
    }

    SafeSetStatus(Cn3D_lOnOffItem[2], pars->PResiduesOn);
    SafeSetStatus(Cn3D_lOnOffItem[3], pars->NTResiduesOn);
    SafeSetStatus(Cn3D_lOnOffItem[4], pars->HeterogensOn);
    SafeSetStatus(Cn3D_lOnOffItem[5], pars->IonsOn);
    SafeSetStatus(Cn3D_lOnOffItem[6], pars->ConnectOn);
    SafeSetStatus(Cn3D_lOnOffItem[7], pars->SolventOn);
    SafeSetStatus(Cn3D_lOnOffItem[8], pars->ObjectOn);
    SafeSetStatus(Cn3D_lOnOffItem[9], pars->HydrogensOn);

/* yanli added */
/* alpha C trace only control */
    pdnmlThis = pmsdThis->pdnmlModels;
    pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
    if (pmldThis->iType == Model_type_ncbi_backbone) {
        if (GetValue(Cn3D_pupPBB) != 4) { /* so it can be off */
            pars->PVirtualBBOn = TRUE;
            pars->PPartialBBOn = FALSE;
            pars->PCompleteBBOn = FALSE;
            SafeSetValue(Cn3D_pupPBB, 1);
        }
        if (GetValue(Cn3D_pupNABB) != 4) {
            pars->NTVirtualBBOn = TRUE;
            pars->NTPartialBBOn = FALSE;
            pars->NTCompleteBBOn = FALSE;
            SafeSetValue(Cn3D_pupNABB, 1);
        }
    }


/*BACKBONE controls */
    if (pars->PVirtualBBOn)
        SafeSetValue(Cn3D_pupPBB, 1);
    else if (pars->PPartialBBOn)
        SafeSetValue(Cn3D_pupPBB, 2);
    else if (pars->PCompleteBBOn)
        SafeSetValue(Cn3D_pupPBB, 3);
    else
        SafeSetValue(Cn3D_pupPBB, 4);

/* set status of BB items from PARS */
    if (pars->NTVirtualBBOn)
        SafeSetValue(Cn3D_pupNABB, 1);
    else if (pars->NTPartialBBOn)
        SafeSetValue(Cn3D_pupNABB, 2);
    else if (pars->NTCompleteBBOn)
        SafeSetValue(Cn3D_pupNABB, 3);
    else
        SafeSetValue(Cn3D_pupNABB, 4);


    for (i = 0; i < 9; i++) {
        k = 0;
        j = R_DEFAULT;
        switch (i) {
        case 0:                /* prot bb */
            j = pars->PBBRender;
            k = pars->PBBColor;
            break;
        case 1:                /* na bb */
            j = pars->NTBBRender;
            k = pars->NTBBColor;
            break;
        case 2:                /* prot sc */
            j = pars->PResRender;
            k = pars->PResColor;
            break;
        case 3:                /* na sc */
            j = pars->NTResRender;
            k = pars->NTResColor;
            break;
        case 4:                /* hets */
            j = pars->HetRender;
            k = pars->HetColor;
            break;
        case 5:                /* ions */
            j = pars->IonRender;
            k = pars->IonColor;
            break;
        case 6:                /* connections */
            j = pars->ConnectRender;
            k = pars->ConnectColor;
            break;
        case 7:                /* solvent */
            j = pars->SolventRender;
            k = pars->SolventColor;
            break;
        case 8:                /* 3-d objects */
            j = pars->ObjectArrows;
            k = pars->ObjectColor;
            break;
        }

        if (i != 6) /* skip connections */
            SetEnumPopup(Cn3D_pupColorStyle[i], colorAlist[i], (UIEnum) k);

        if (i == 8) { /* special settings for 3d-object arrows */
            SetEnumPopup(Cn3D_pupRenderStyle[8], renderAlist[8], (UIEnum) j);
        } else {
            switch (j) {
            case R_STICK:
                SetEnumPopup(Cn3D_pupRenderStyle[i], renderAlist[i], (UIEnum) 2);
                break;
            case R_BALLNSTICK:
                SetEnumPopup(Cn3D_pupRenderStyle[i], renderAlist[i], (UIEnum) 3);
                break;
            case R_THICKWIRE:
                SetEnumPopup(Cn3D_pupRenderStyle[i], renderAlist[i], (UIEnum) 4);
                break;
            case R_SPACE:
                SetEnumPopup(Cn3D_pupRenderStyle[i], renderAlist[i], (UIEnum) 5);
                break;
            case R_WORMWIRE:
                SetEnumPopup(Cn3D_pupRenderStyle[i], renderAlist[i], (UIEnum) 6);
                break;
#ifdef _OPENGL
            case R_WORMTHIN:
                SetEnumPopup(Cn3D_pupRenderStyle[i], renderAlist[i], (UIEnum) 7);
                break;
            case R_WORMFAT:
                SetEnumPopup(Cn3D_pupRenderStyle[i], renderAlist[i], (UIEnum) 8);
                break;
#endif
            case R_DEFAULT:
            case R_WIRE:
            default:
                SetEnumPopup(Cn3D_pupRenderStyle[i], renderAlist[i], (UIEnum) 1);
                break;
            }
        }
    }

#ifdef _OPENGL
    if (pars->BGColRGB[0] == Cn3d_PaletteRGB[C_black][0] &&
        pars->BGColRGB[1] == Cn3d_PaletteRGB[C_black][1] &&
        pars->BGColRGB[2] == Cn3d_PaletteRGB[C_black][2])
        SetEnumPopup(Cn3D_pupBackgroundColor, bgColorAList, (UIEnum) 1);
    else if (pars->BGColRGB[0] == Cn3d_PaletteRGB[C_white][0] &&
             pars->BGColRGB[1] == Cn3d_PaletteRGB[C_white][1] &&
             pars->BGColRGB[2] == Cn3d_PaletteRGB[C_white][2])
        SetEnumPopup(Cn3D_pupBackgroundColor, bgColorAList, (UIEnum) 2);
    else
        SetEnumPopup(Cn3D_pupBackgroundColor, bgColorAList, (UIEnum) 3);
#endif

}


static void MasterChangeLabelsProc(void)
{
    Int2 i, k, nameval;
    Uint1 codeval;
    Boolean NumOn, NameOn, PDBOn, WhiteOn, TopOn;
    PARS pars = NULL;
    PMSD pmsdThis = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || 
        !(pmsdThis = (PMSD) pdnmsThis->data.ptrvalue) ||
        !(pars = (PARS) pmsdThis->pGlobalPARS))
        return;

    pars->PBBLabelInterval = (Uint1) GetValue(Cn3D_pupLabelAA);
    pars->NTBBLabelInterval = (Uint1) GetValue(Cn3D_pupLabelNT);

    /* set termini labels */
    pars->PTermLabelOn = (Uint1) GetStatus(Cn3D_lOnOffLabel[2]);
    pars->NTTermLabelOn = (Uint1) GetStatus(Cn3D_lOnOffLabel[3]);
#ifdef _OPENGL
    pars->IonLabelOn = (Uint1) GetStatus(Cn3D_bIonLabel);
#endif


    for (i = 0; i < 2; i++) {
        nameval = GetValue(Cn3D_pupLResName[i]);
        if (nameval == 3)
            codeval = (Uint1) L_1LETR;
        else
            codeval = (Uint1) L_3LETR;
        switch (i) {
        case 0:                /* aa */
            /* clear bits */
            pars->PBBLabelStyle =
                pars->PBBLabelStyle & ~((Uint1) (L_3LETR | L_1LETR));
            /* set bit */
            pars->PBBLabelStyle = pars->PBBLabelStyle | (Uint1) codeval;
            break;
        case 1:                /* na   */
            pars->NTBBLabelStyle =
                pars->NTBBLabelStyle & ~((Uint1) (L_3LETR | L_1LETR));
            pars->NTBBLabelStyle = pars->NTBBLabelStyle | (Uint1) codeval;
            break;
        default:;
        }

        NameOn = GetStatus(Cn3D_bLTermName[i]);
        switch (i) {
        case 0:                /* aa */
            if (NameOn)
                pars->PTermLabelStyle |= ((Uint1) L_NAME);
            else
                pars->PTermLabelStyle &= ~((Uint1) L_NAME);
            break;
        case 1:                /* na   */
            if (NameOn)
                pars->NTTermLabelStyle |= ((Uint1) L_NAME);
            else
                pars->NTTermLabelStyle &= ~((Uint1) L_NAME);
            break;
        default:;
        }

        NumOn = GetStatus(Cn3D_bLNum[i]);
        switch (i) {
        case 0:                /* aa */
            if (NumOn)
                pars->PBBLabelStyle = pars->PBBLabelStyle | (Uint1) L_NUM;
            else
                pars->PBBLabelStyle =
                    pars->PBBLabelStyle & (Uint1) ~ L_NUM;
            break;
        case 1:                /* na   */
            if (NumOn)
                pars->NTBBLabelStyle =
                    pars->NTBBLabelStyle | (Uint1) L_NUM;
            else
                pars->NTBBLabelStyle =
                    pars->NTBBLabelStyle & (Uint1) ~ L_NUM;
            break;
        default:;
        }
        NameOn = (Boolean) (nameval > 1);
        switch (i) {
        case 0:                /* aa */
            if (NameOn)
                pars->PBBLabelStyle = pars->PBBLabelStyle | (Uint1) L_NAME;
            else
                pars->PBBLabelStyle =
                    pars->PBBLabelStyle & (Uint1) ~ L_NAME;
            break;
        case 1:                /* na   */
            if (NameOn)
                pars->NTBBLabelStyle =
                    pars->NTBBLabelStyle | (Uint1) L_NAME;
            else
                pars->NTBBLabelStyle =
                    pars->NTBBLabelStyle & (Uint1) ~ L_NAME;
            break;
        default:;
        }
        PDBOn = (Boolean) (nameval == 4);
        switch (i) {
        case 0:                /* aa */
            if (PDBOn)
                pars->PBBLabelStyle = pars->PBBLabelStyle | (Uint1) L_PDB;
            else
                pars->PBBLabelStyle =
                    pars->PBBLabelStyle & (Uint1) ~ L_PDB;
            break;
        case 1:                /* na   */
            if (PDBOn)
                pars->NTBBLabelStyle =
                    pars->NTBBLabelStyle | (Uint1) L_PDB;
            else
                pars->NTBBLabelStyle =
                    pars->NTBBLabelStyle & (Uint1) ~ L_PDB;
            break;
        default:;
        }
        WhiteOn = GetStatus(Cn3D_bLWhite[i]);
        switch (i) {
        case 0:                /* aa */
            if (WhiteOn)
                pars->PBBLabelStyle =
                    pars->PBBLabelStyle | (Uint1) L_WHITE;
            else
                pars->PBBLabelStyle =
                    pars->PBBLabelStyle & (Uint1) ~ L_WHITE;
            break;
        case 1:                /* na   */
            if (WhiteOn)
                pars->NTBBLabelStyle =
                    pars->NTBBLabelStyle | (Uint1) L_WHITE;
            else
                pars->NTBBLabelStyle =
                    pars->NTBBLabelStyle & (Uint1) ~ L_WHITE;
            break;
        default:;
        }

#ifndef _OPENGL
        TopOn = GetStatus(Cn3D_bLTop[i]);
        switch (i) {
        case 0:                /* aa */
            if (TopOn)
                pars->PBBLabelJust = pars->PBBLabelJust | (Uint1) LA_FRONT;
            else
                pars->PBBLabelJust =
                    pars->PBBLabelJust & (Uint1) ~ LA_FRONT;
            break;
        case 1:                /* na   */
            if (TopOn)
                pars->NTBBLabelJust =
                    pars->NTBBLabelJust | (Uint1) LA_FRONT;
            else
                pars->NTBBLabelJust =
                    pars->NTBBLabelJust & (Uint1) ~ LA_FRONT;
            break;
        default:;
        }
        k = (Int2) GetValue(Cn3D_pupLabelSize[i]);
        switch (i) {
        case 0:                /* prot bb */
            pars->PBBLabelScale = k;
            break;
        case 1:                /* na bb */
            pars->NTBBLabelScale = k;
            break;
        default:
            ;
        }
#endif  /* ndef _OPENGL */

    }

    if (GetStatus(Cn3D_LimmediateRedraw)) {
        Cn3D_RedrawProc(Cn3D_bLRedraw);
    }
}

static void LabelPopupOnOffProc(PopuP p)
 {
    MasterChangeLabelsProc();
}

static void LabelButtonOnOffProc(ButtoN b)
 {
    MasterChangeLabelsProc();
}

static void LabelGroupOnOffProc(GrouP g)
 {
    MasterChangeLabelsProc();
}

void LIBCALL ResetLabelCtrls(void)
{
    Int2 i, val, style, size, termStyle;
    PARS pars = NULL;
    PMSD pmsdThis = NULL;
    PDNMS pdnmsThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || 
        !(pmsdThis = (PMSD) pdnmsThis->data.ptrvalue) ||
        !(pars = (PARS) pmsdThis->pGlobalPARS)) {

        SafeSetValue(Cn3D_pupLabelAA, 1);
        SafeSetValue(Cn3D_pupLabelNT, 1);

        SafeSetStatus(Cn3D_lOnOffLabel[2], FALSE);
        SafeSetStatus(Cn3D_lOnOffLabel[3], FALSE);

        for (i = 0; i < 2; i++) {
            SafeSetValue(Cn3D_pupLResName[i], 1);
            SafeSetStatus(Cn3D_bLTermName[i], FALSE);
            SafeSetStatus(Cn3D_bLNum[i], FALSE);
            SafeSetStatus(Cn3D_bLWhite[i], FALSE);
#ifndef _OPENGL
            SafeSetStatus(Cn3D_bLTop[i], FALSE);
            SafeSetValue(Cn3D_pupLabelSize[i], 1);
#endif
        }
        return;
    }

    SafeSetValue(Cn3D_pupLabelAA, pars->PBBLabelInterval);
    SafeSetValue(Cn3D_pupLabelNT, pars->NTBBLabelInterval);

    SafeSetStatus(Cn3D_lOnOffLabel[2], pars->PTermLabelOn);
    SafeSetStatus(Cn3D_lOnOffLabel[3], pars->NTTermLabelOn);
#ifdef _OPENGL
    SafeSetStatus(Cn3D_bIonLabel, pars->IonLabelOn);
#endif

    for (i = 0; i < 2; i++) {
        val = 1;
        switch (i) {
        case 0:
            style = pars->PBBLabelStyle;
            size = pars->PBBLabelScale;
            termStyle = pars->PTermLabelStyle;
            break;
        case 1:
            style = pars->NTBBLabelStyle;
            size = pars->NTBBLabelScale;
            termStyle = pars->NTTermLabelStyle;
            break;
        default:
            break;
        }
        if (!(Boolean) (style & (Uint1) (L_NAME))) {
            val = 1;
        } else if ((Boolean) (style & (Uint1) (L_3LETR))) {
            val = 2;
        } else if ((Boolean) (style & (Uint1) (L_1LETR))) {
            val = 3;
        } else if ((Boolean) (style & (Uint1) L_PDB)) {
            val = 4;
        }
        SafeSetValue(Cn3D_pupLResName[i], val);
        SafeSetStatus(Cn3D_bLTermName[i], (termStyle & L_NAME) ? TRUE : FALSE);
        SafeSetStatus(Cn3D_bLNum[i], (Boolean) (style & (Uint1) L_NUM));
        SafeSetStatus(Cn3D_bLWhite[i],
                      (Boolean) (style & (Uint1) ~ L_WHITE));
#ifndef _OPENGL
        SafeSetStatus(Cn3D_bLTop[i], (Boolean) (style & (Uint1) LA_FRONT));
        SafeSetValue(Cn3D_pupLabelSize[i], size);
#endif
    }
}


void Cn3D_RedrawProc(ButtoN b)
{
    Int2 k;
    PARS pars = NULL;
    PMSD pmsdThis = NULL;
    PDNMS pdnmsThis = NULL;
    PDNML pdnmlThis = NULL;
    PMLD pmldThis = NULL;

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis || 
        !(pmsdThis = (PMSD) pdnmsThis->data.ptrvalue) ||
        !(pars = (PARS) pmsdThis->pGlobalPARS)) {

        for (k = 2; k < 10; k++) {
            SafeSetStatus(Cn3D_lOnOffItem[k], FALSE);
        }
        SafeSetValue(Cn3D_pupPBB, 4);
        SafeSetValue(Cn3D_pupNABB, 4);
        for (k = 0; k < 9; k++) {
            SetEnumPopup(Cn3D_pupRenderStyle[k], renderAlist[k],
                         (UIEnum) renderDefaults[k]);
            if (k != 6)
                SetEnumPopup(Cn3D_pupColorStyle[k], colorAlist[k],
                            (UIEnum) colorDefaults[k]);
        }
#ifdef _OPENGL
        SetEnumPopup(Cn3D_pupBackgroundColor, bgColorAList, (UIEnum) 1);
#endif
        return;
    }

/* alpha C trace only control */
    pdnmlThis = pmsdThis->pdnmlModels;
    pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
    if (pmldThis->iType == Model_type_ncbi_backbone) {
        if (GetValue(Cn3D_pupPBB) != 4) { /* so it can be off */
            pars->PVirtualBBOn = TRUE;
            pars->PPartialBBOn = FALSE;
            pars->PCompleteBBOn = FALSE;
            SafeSetValue(Cn3D_pupPBB, 1);
        }
        if (GetValue(Cn3D_pupNABB) != 4) {
            pars->NTVirtualBBOn = TRUE;
            pars->NTPartialBBOn = FALSE;
            pars->NTCompleteBBOn = FALSE;
            SafeSetValue(Cn3D_pupNABB, 1);
        }
    }

    Cn3dObjMgrGetSelected();
    Cn3D_Redraw(FALSE);
}


static void AlignLabelButtons(Handle master, Handle slave)
 {
    RecT pptPos, btnPos;
    Int2 delta;

    GetPosition(master, &pptPos);
    GetPosition(slave, &btnPos);
    delta =
        (pptPos.bottom + pptPos.top) / 2 - (btnPos.bottom +
                                            btnPos.top) / 2;
    if (delta != 0) {
        OffsetRect(&btnPos, 0, delta);
        SetPosition(slave, &btnPos);
        AdjustPrnt((Nlm_GraphiC) slave, &btnPos, FALSE);
    }
}

#ifdef _OPENGL
extern void Nlm_FindAvailableFonts(PopuP);
#endif

GrouP LIBCALL LabelControls(Nlm_GrouP prnt)
{
    GrouP g, h;
    Int2 k;
    PrompT ppt[4];
    PrompT ppt1, ppt2, ppt3, ppt4, ppt7, ppt8, ppt9;
#ifdef _OPENGL
    GrouP h2, h3, h4, h5, h6, h7;
    PrompT ppt10, ppt11;
#endif

    g = HiddenGroup(prnt, -1, 0, NULL);
    StaticPrompt(g, "", 0, stdLineHeight, systemFont, 'l');
    StaticPrompt(g, "", 0, stdLineHeight, systemFont, 'l');
    if (!g)
        return NULL;
    SetGroupSpacing(g, 3, 9);
/*
#ifdef WIN_MOTIF
      SetGroupMargins ( g, 4, 1 );
      SetGroupSpacing ( g, 2, 1 );
#else
      SetGroupMargins ( g, 1, 1 );
      SetGroupSpacing ( g, 0, 0 );
#endif
*/

    h = HiddenGroup(g, 0, -5, NULL);
    SetGroupSpacing(h, 5, 3);

    Cn3D_lOnOffLabel[0] = NULL; /* Cn3D_pupLabelAA includes off setting */
    Cn3D_lOnOffLabel[1] = NULL; /* Cn3D_pupLabelNT includes off setting */

    ppt1 =
        StaticPrompt(h, "Category", 0, stdLineHeight + 5, systemFont, 'c');
    ppt[0] =
        StaticPrompt(h, "Amino Acid", 0, popupMenuHeight, systemFont, 'l');
    ppt[1] =
        StaticPrompt(h, "Nucleic Acid", 0, popupMenuHeight, systemFont, 'l');
    ppt[2] =
        StaticPrompt(h, "N & C Termini", 0, popupMenuHeight, systemFont, 'l');
    ppt[3] =
        StaticPrompt(h, "5' & 3' Termini", 0, popupMenuHeight, systemFont, 'l');
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt1, (HANDLE) ppt[3], NULL);

    ppt2 =
        StaticPrompt(h, "On/Off", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_pupLabelAA = PopupList(h, FALSE, LabelPopupOnOffProc);
    PopupItem(Cn3D_pupLabelAA, "none");
    PopupItem(Cn3D_pupLabelAA, "every AA");
    PopupItem(Cn3D_pupLabelAA, "every 5");
    PopupItem(Cn3D_pupLabelAA, "every 10");
    PopupItem(Cn3D_pupLabelAA, "every 20");
    PopupItem(Cn3D_pupLabelAA, "every 50");
    Cn3D_pupLabelNT = PopupList(h, FALSE, LabelPopupOnOffProc);
    PopupItem(Cn3D_pupLabelNT, "none");
    PopupItem(Cn3D_pupLabelNT, "every NA");
    PopupItem(Cn3D_pupLabelNT, "every 5");
    PopupItem(Cn3D_pupLabelNT, "every 10");
    PopupItem(Cn3D_pupLabelNT, "every 20");
    PopupItem(Cn3D_pupLabelNT, "every 50");
    Cn3D_lOnOffLabel[2] = CheckBox(h, "", LabelButtonOnOffProc);
    Cn3D_lOnOffLabel[3] = CheckBox(h, "", LabelButtonOnOffProc);
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt2, (HANDLE) Cn3D_pupLabelAA,
                 (HANDLE) Cn3D_pupLabelNT, (HANDLE) Cn3D_lOnOffLabel[2],
                 (HANDLE) Cn3D_lOnOffLabel[3], NULL);

    ppt3 = StaticPrompt(h, "Name", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 2; k++) {
        Cn3D_pupLResName[k] = PopupList(h, FALSE, LabelPopupOnOffProc);
        PopupItem(Cn3D_pupLResName[k], "none");
        PopupItem(Cn3D_pupLResName[k], "3 letter");
        PopupItem(Cn3D_pupLResName[k], "1 letter");
        PopupItem(Cn3D_pupLResName[k], "PDB");
    }
    for (k = 0; k < 2; k++) {
        Cn3D_bLTermName[k] = CheckBox(h, "", LabelButtonOnOffProc);
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt3, (HANDLE) Cn3D_pupLResName[0],
                 (HANDLE) Cn3D_pupLResName[1], (HANDLE) Cn3D_bLTermName[0],
                 (HANDLE) Cn3D_bLTermName[1], NULL);

    ppt4 = StaticPrompt(h, "Num", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 2; k++) {
        Cn3D_bLNum[k] = CheckBox(h, "", LabelButtonOnOffProc);
    }
    for (k = 2; k < 4; k++) { /* just placeholders */
        Cn3D_bLNum[k] = (ButtoN) StaticPrompt(h, "", 0, stdLineHeight + 5, systemFont, 'c');
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt4, (HANDLE) Cn3D_bLNum[0],
                 (HANDLE) Cn3D_bLNum[1], (HANDLE) Cn3D_bLNum[2], 
                 (HANDLE) Cn3D_bLNum[3], NULL);

#ifndef _OPENGL
    ppt7 =
        StaticPrompt(h, "On Top", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 4; k++) {
        Cn3D_bLTop[k] = CheckBox(h, "", LabelButtonOnOffProc);
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt7, (HANDLE) Cn3D_bLTop[0],
                 (HANDLE) Cn3D_bLTop[1], (HANDLE) Cn3D_bLTop[2],
                 (HANDLE) Cn3D_bLTop[3], NULL);
#endif

    ppt8 = StaticPrompt(h, "White", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 2; k++) {
        Cn3D_bLWhite[k] = CheckBox(h, "", LabelButtonOnOffProc);
    }
    for (k = 2; k < 4; k++) { /* just placeholders */
        Cn3D_bLWhite[k] = (ButtoN) StaticPrompt(h, "", 0, stdLineHeight + 5, systemFont, 'c');
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt8,
                 (HANDLE) Cn3D_bLWhite[0], (HANDLE) Cn3D_bLWhite[1], 
                 (HANDLE) Cn3D_bLWhite[2], (HANDLE) Cn3D_bLWhite[3], NULL);

#ifndef _OPENGL
    ppt9 = StaticPrompt(h, "Size", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 4; k++) {
        Cn3D_pupLabelSize[k] = PopupList(h, FALSE, LabelPopupOnOffProc);
        PopupItem(Cn3D_pupLabelSize[k], "1");
        PopupItem(Cn3D_pupLabelSize[k], "2");
        PopupItem(Cn3D_pupLabelSize[k], "3");
        PopupItem(Cn3D_pupLabelSize[k], "4");
        PopupItem(Cn3D_pupLabelSize[k], "5");
        PopupItem(Cn3D_pupLabelSize[k], "6");
        PopupItem(Cn3D_pupLabelSize[k], "7");
        PopupItem(Cn3D_pupLabelSize[k], "8");
        PopupItem(Cn3D_pupLabelSize[k], "9");
        PopupItem(Cn3D_pupLabelSize[k], "10");
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt9,
                 (HANDLE) Cn3D_pupLabelSize[0],
                 (HANDLE) Cn3D_pupLabelSize[1],
                 (HANDLE) Cn3D_pupLabelSize[2],
                 (HANDLE) Cn3D_pupLabelSize[3], NULL);
#endif

#ifndef _OPENGL
    AlignLabelButtons(Cn3D_pupLabelSize[2], Cn3D_lOnOffLabel[2]);
    AlignLabelButtons(Cn3D_pupLabelSize[3], Cn3D_lOnOffLabel[3]);
    for (k = 0; k < 2; k++) {
        AlignLabelButtons(Cn3D_pupLabelSize[k], Cn3D_pupLResName[k]);
        AlignLabelButtons(Cn3D_pupLabelSize[k+2], Cn3D_bLTermName[k]);
        AlignLabelButtons(Cn3D_pupLabelSize[k], Cn3D_bLNum[k]);
    }
    for (k = 0; k < 4; k++) {
        AlignLabelButtons(Cn3D_pupLabelSize[k], Cn3D_bLTop[k]);
        AlignLabelButtons(Cn3D_pupLabelSize[k], Cn3D_bLWhite[k]);
    }

#else
    AlignLabelButtons(ppt[2], Cn3D_lOnOffLabel[2]);
    AlignLabelButtons(ppt[3], Cn3D_lOnOffLabel[3]);
    for (k = 0; k < 2; k++) {
        AlignLabelButtons(ppt[k], Cn3D_pupLResName[k]);
        AlignLabelButtons(ppt[k+2], Cn3D_bLTermName[k]);
        AlignLabelButtons(ppt[k], Cn3D_bLNum[k]);
        AlignLabelButtons(ppt[k], Cn3D_bLWhite[k]);
    }

    h6 = HiddenGroup(g, 0, -1, NULL); /* blank horiz. space */
    SetGroupSpacing(h6, 5, 3);

    h7 = HiddenGroup(g, 0, -1, NULL);
    Cn3D_bIonLabel = CheckBox(h7, "Show Ion Labels", LabelButtonOnOffProc);

    h5 = HiddenGroup(g, 0, -1, NULL); /* blank horiz. space */
    SetGroupSpacing(h5, 5, 3);


    h2 = HiddenGroup(g, 0, -1, NULL);
    SetGroupSpacing(h2, 5, 3);
    ppt9 = StaticPrompt(h2, "Font:  ", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_pupLabelFont = PopupList(h2, FALSE, LabelPopupOnOffProc);
    Nlm_FindAvailableFonts(Cn3D_pupLabelFont);
    SetValue(Cn3D_pupLabelFont, 1);
    AlignLabelButtons(ppt9,Cn3D_pupLabelFont);
    ppt10 = StaticPrompt(h2, "   Size: ", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_pupLabelPoints = PopupList(h2, FALSE, LabelPopupOnOffProc);
    for (k=1; k<=30; k++) {
        char ss[3];
        sprintf(ss,"%i",k);
        PopupItem(Cn3D_pupLabelPoints, ss);
    }
    SetValue(Cn3D_pupLabelPoints, 12);
    AlignLabelButtons(ppt10,Cn3D_pupLabelPoints);

    h3 = HiddenGroup(g, 0, -1, NULL);
    SetGroupSpacing(h3, 5, 3);
    ppt11 = StaticPrompt(h3, "Style:  ", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_bLFontBold = CheckBox(h3, "bold", LabelButtonOnOffProc);
    Cn3D_bLFontItalic = CheckBox(h3, "italic", LabelButtonOnOffProc);
#ifndef WIN_MOTIF
    Cn3D_bLFontUnderlined = CheckBox(h3, "underlined", LabelButtonOnOffProc);
#endif

    h4 = HiddenGroup(g, 0, -1, NULL); /* blank horiz. space */
    SetGroupSpacing(h4, 5, 3);

    AlignObjects(ALIGN_CENTER, (HANDLE) h5, (HANDLE) h2, (HANDLE) h3,
                (HANDLE) h4, (HANDLE) h6, (HANDLE) h7, (HANDLE) h, NULL);

#endif /* _OPENGL */

    Cn3D_LimmediateRedraw =
        CheckBox(g, "Immediate Redraw on Every Change", NULL);
    Cn3D_bLRedraw = PushButton(g, "Apply!", Cn3D_RedrawProc);
    AlignObjects(ALIGN_CENTER, (HANDLE) Cn3D_bLRedraw,
                 (HANDLE) Cn3D_LimmediateRedraw, (HANDLE) h, NULL);

    ResetLabelCtrls();
    return g;
}

#ifdef _OPENGL

#define CN3DTICKDEFAULT (6)
static FloatHi Cn3D_Tick[] = {
    0.01L, 0.025L, 0.05L, 0.1L, 0.25L, 0.5L, 1.0L, 2.5L, 5.0L, 10.0L, 0.0L};

void Cn3D_SetQualityFromAppParams(void)
{
    Char str[64];
    int i;

    if (GetAppParam("CN3D", "QUALITY", "BIGCYLSIDES", NULL, str, sizeof(str)))
        SafeSetValue(Cn3D_pupQualBigCylSides, atoi(str) - QUAL_SIZE_MIN + 1);
    if (GetAppParam("CN3D", "QUALITY", "BONDCYLSIDES", NULL, str, sizeof(str)))
        SafeSetValue(Cn3D_pupQualBondCylSides, atoi(str) - QUAL_SIZE_MIN + 1);
    if (GetAppParam("CN3D", "QUALITY", "SPHERESLICES", NULL, str, sizeof(str)))
        SafeSetValue(Cn3D_pupQualSphSlices, atoi(str) - QUAL_SIZE_MIN + 1);
    if (GetAppParam("CN3D", "QUALITY", "SPHERESTACKS", NULL, str, sizeof(str)))
        SafeSetValue(Cn3D_pupQualSphStacks, atoi(str) - QUAL_SIZE_MIN + 1);
    if (GetAppParam("CN3D", "QUALITY", "WORMSEGMENTS", NULL, str, sizeof(str)))
        SafeSetValue(Cn3D_pupQualWormSegments, atoi(str)/2 + 1 - QUAL_SIZE_MIN/2);
    if (GetAppParam("CN3D", "QUALITY", "WORMSIDES", NULL, str, sizeof(str)))
        SafeSetValue(Cn3D_pupQualWormSides, atoi(str)/2 + 1 - QUAL_SIZE_MIN/2);

    if (GetAppParam("CN3D", "SETTINGS", "ANIMATIONSPEED", NULL, str, sizeof(str))) {
        FloatHi tick;
        int i;

        tick = atof(str);
        for (i=0; Cn3D_Tick[i] != 0.0L; i++)
            if (tick == Cn3D_Tick[i]) break;
        if (Cn3D_Tick[i] == 0.0L) i = CN3DTICKDEFAULT - 1;
        Cn3D_ColorData.OGL_Data->Tick = Cn3D_Tick[i];
        SetValue(Cn3D_pTick, i + 1);
    }
}

static void QualityChangePopupProc(Nlm_PopuP p)
{
    Char str[8];

    sprintf(str, "%i", GetValue(Cn3D_pupQualBigCylSides) + QUAL_SIZE_MIN - 1);
    SetAppParam("CN3D", "QUALITY", "BIGCYLSIDES", str);
    sprintf(str, "%i", GetValue(Cn3D_pupQualBondCylSides) + QUAL_SIZE_MIN - 1);
    SetAppParam("CN3D", "QUALITY", "BONDCYLSIDES", str);
    sprintf(str, "%i", GetValue(Cn3D_pupQualSphSlices) + QUAL_SIZE_MIN - 1);
    SetAppParam("CN3D", "QUALITY", "SPHERESLICES", str);
    sprintf(str, "%i", GetValue(Cn3D_pupQualSphStacks) + QUAL_SIZE_MIN - 1);
    SetAppParam("CN3D", "QUALITY", "SPHERESTACKS", str);
    sprintf(str, "%i", 2 * (GetValue(Cn3D_pupQualWormSegments) - 1) + QUAL_SIZE_MIN);
    SetAppParam("CN3D", "QUALITY", "WORMSEGMENTS", str);
    sprintf(str, "%i", 2 * (GetValue(Cn3D_pupQualWormSides) - 1) + QUAL_SIZE_MIN);
    SetAppParam("CN3D", "QUALITY", "WORMSIDES", str);

    if (GetStatus(Cn3D_QimmediateRedraw)) {
        Cn3D_RedrawProc(Cn3D_bQRedraw);
    }
}

static Nlm_Int2 qSet[3][6] = {
    {  12,  6,  6,  4,  4,  6 }, /* low/fast  */
    { 12,  6,  10,  8,  6,  6 }, /* med/med   */
    { 30, 16, 16, 10, 10, 20 }  /* high/slow */
};

static void OverallQualityChangeProc(Nlm_ButtoN pressed)
{
    int i, which = -1;

    for (i=0; i<3; i++) {
        if (pressed == Cn3D_bQualOverall[i]) {
            which = i;
            break;
        }
    }
    SafeSetValue(Cn3D_pupQualBigCylSides, qSet[which][0] - QUAL_SIZE_MIN + 1);
    SafeSetValue(Cn3D_pupQualBondCylSides, qSet[which][1] - QUAL_SIZE_MIN + 1);
    SafeSetValue(Cn3D_pupQualSphSlices, qSet[which][2] - QUAL_SIZE_MIN + 1);
    SafeSetValue(Cn3D_pupQualSphStacks, qSet[which][3] - QUAL_SIZE_MIN + 1);
    SafeSetValue(Cn3D_pupQualWormSegments, qSet[which][4]/2 + 1 - QUAL_SIZE_MIN/2);
    SafeSetValue(Cn3D_pupQualWormSides, qSet[which][5]/2 + 1 - QUAL_SIZE_MIN/2);
    QualityChangePopupProc(NULL);
}

static void Cn3D_TickPopupProc(PopuP value)
{
    Int4 n;
    Char str[64];

    OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
    n = GetValue(Cn3D_pTick);
    Cn3D_ColorData.OGL_Data->Tick = Cn3D_Tick[n - 1];

    sprintf(str, "%g", Cn3D_ColorData.OGL_Data->Tick);
    SetAppParam("CN3D", "SETTINGS", "ANIMATIONSPEED", str);
}

static Nlm_GrouP qualityItem(Nlm_GrouP h, char *title, Nlm_PopuP *pup, int def)
{
    char numstr[3];
    Nlm_PrompT prompt;
    Nlm_GrouP i;
    int k;

    i = Nlm_HiddenGroup(h, -2, 1, NULL);
    SetGroupSpacing(i, 15, 3);
    *pup = PopupList(i, FALSE, QualityChangePopupProc);
    if (*pup == Cn3D_pupQualWormSegments || *pup == Cn3D_pupQualWormSides) {
        /* only even numbers */
        for (k=QUAL_SIZE_MIN; k<=QUAL_SIZE_MAX; k+=2) {
            sprintf(numstr,"%i",k);
            PopupItem(*pup, numstr);
        }
        SetValue(*pup, def/2 + 1 - QUAL_SIZE_MIN/2);
    } else {
        for (k=QUAL_SIZE_MIN; k<=QUAL_SIZE_MAX; k++) {
            sprintf(numstr,"%i",k);
            PopupItem(*pup, numstr);
        }
        SetValue(*pup, def - QUAL_SIZE_MIN + 1);
    }
    prompt = StaticPrompt(i, title, 0, stdLineHeight + 5, systemFont, 'c');
    AlignLabelButtons(prompt, *pup);
    return i;
}

Nlm_GrouP LIBCALL OGL_Quality(Nlm_GrouP prnt)
{
    Nlm_GrouP g, h, i[8], bg;
    Nlm_PrompT bp, p, q;
    int n=-1;
    char numstr[32];

    g = Nlm_HiddenGroup(prnt, -1, 0, NULL);
    if (!g)
        return NULL;
    SetGroupSpacing(g, 3, 9);

    h = Nlm_HiddenGroup(g, 1, 6, NULL);
    SetGroupSpacing(h, 5, 3);

    /* blank space */
    bg = Nlm_HiddenGroup(h, 1, 6, NULL);
    SetGroupSpacing(bg, 5, 10);
    bp = StaticPrompt(bg, "", 0, stdLineHeight + 5, systemFont, 'c');

    i[++n] = qualityItem(h, "Helix cylinder sides",
                         &Cn3D_pupQualBigCylSides, qSet[1][0]);
    i[++n] = qualityItem(h, "Bond cylinder sides",
                         &Cn3D_pupQualBondCylSides, qSet[1][1]);
    i[++n] = qualityItem(h, "Sphere slices",
                         &Cn3D_pupQualSphSlices, qSet[1][2]);
    i[++n] = qualityItem(h, "Sphere stacks",
                         &Cn3D_pupQualSphStacks, qSet[1][3]);
    i[++n] = qualityItem(h, "Worm segments",
                         &Cn3D_pupQualWormSegments, qSet[1][4]);
    i[++n] = qualityItem(h, "Worm sides",
                         &Cn3D_pupQualWormSides, qSet[1][5]);

    bg = Nlm_HiddenGroup(h, 1, 6, NULL);
    SetGroupSpacing(bg, 5, 10);
    bp = StaticPrompt(bg, "", 0, stdLineHeight + 5, systemFont, 'c');

    i[++n] = Nlm_HiddenGroup(h, -1, 6, NULL);
    SetGroupSpacing(i[n], 5, 10);
    p = StaticPrompt(i[n], "Set Overall Quality / Rendering Speed to:",
                    0, stdLineHeight + 5, systemFont, 'c');

    i[++n] = Nlm_HiddenGroup(h, -3, 6, NULL);
    SetGroupSpacing(i[n], 5, 10);
    Cn3D_bQualOverall[0] = PushButton(i[n], "low/fast", OverallQualityChangeProc);
    Cn3D_bQualOverall[1] = PushButton(i[n], "med/med", OverallQualityChangeProc);
    Cn3D_bQualOverall[2] = PushButton(i[n], "high/slow", OverallQualityChangeProc);
    AlignObjects(ALIGN_CENTER, (HANDLE) i[6], (HANDLE) i[7], (HANDLE) h, NULL);

    bg = Nlm_HiddenGroup(h, 1, 6, NULL);
    SetGroupSpacing(bg, 5, 10);
    bp = StaticPrompt(bg, "", 0, stdLineHeight + 5, systemFont, 'c');

    bg = HiddenGroup(h, 2, 1, NULL);
    StaticPrompt(bg, "Animation speed (seconds per image):",
                    0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_pTick = PopupList(bg, FALSE, Cn3D_TickPopupProc);
    for(n = 0; Cn3D_Tick[n] != 0.0L; n++) {
        sprintf(numstr,"%lg", Cn3D_Tick[n]);
        PopupItem(Cn3D_pTick, numstr);
    }
    SetValue(Cn3D_pTick, CN3DTICKDEFAULT);
    Cn3D_ColorData.OGL_Data->Tick = Cn3D_Tick[CN3DTICKDEFAULT];

    bg = Nlm_HiddenGroup(h, 1, 6, NULL);
    SetGroupSpacing(bg, 5, 10);
    bp = StaticPrompt(bg, "", 0, stdLineHeight + 5, systemFont, 'c');

    Cn3D_QimmediateRedraw =
        CheckBox(g, "Immediate Redraw on Every Change", NULL);
    Cn3D_bQRedraw = PushButton(g, "Apply!", Cn3D_RedrawProc);
    AlignObjects(ALIGN_CENTER, (HANDLE) Cn3D_bQRedraw,
                 (HANDLE) Cn3D_QimmediateRedraw, (HANDLE) h, NULL);

    return g;
}
#endif

NLM_EXTERN void fnCn3D_RedrawWrapper(ButtoN b)
{
    Cn3D_RedrawProc(b);
    Cn3D_SendUpdate();
}

static CharPtr categorynames[] = {
    "", "", "Protein Sidechains", "Nucleotide Sidechains", "Heterogen",
    "Ions", "Connections", "Solvents", "3D Objects", "Hydrogens"
};

static PopuP RenderStyle(GrouP h, Int2 i)
 {
    PopuP p;

    if (renderAlist[i] != empty_alist) {
        p = PopupList(h, FALSE, PopupOnOffProc);
    } else {
        p = PopupList(h, FALSE, NULL);
    }
    InitEnumPopup(p, renderAlist[i], NULL);
    return p;
}

static PopuP ColorStyle(GrouP h, Int2 i)
 {
    PopuP p;
    int j;

    if (colorAlist[i] != empty_alist) {
        p = PopupList(h, FALSE, PopupOnOffProc);
    } else {
        p = PopupList(h, FALSE, NULL);
    }
    for (j=0; colorAlist[i][j].name; j++)
        PopupItem(p, Cn3D_ColorFuncName(colorAlist[i][j].value));
    return p;
}

GrouP LIBCALL RenderControls(Nlm_GrouP prnt)
{
    GrouP g, h;
    Int2 k;
    PrompT ppt[10];
    PrompT ppt1, ppt2, ppt3, ppt4;
    RecT pptPos, btnPos;
    Int2 delta;
#ifdef _OPENGL
    GrouP bg;
    PrompT ppt5;
#endif

    g = HiddenGroup(prnt, -1, 0, NULL);
    if (!g)
        return NULL;
    SetGroupSpacing(g, 3, 9);
/*
#ifdef WIN_MOTIF
      SetGroupMargins ( g, 4, 1 );
      SetGroupSpacing ( g, 2, 1 );
#else
      SetGroupMargins ( g, 1, 1 );
      SetGroupSpacing ( g, 0, 0 );
#endif
*/

    h = HiddenGroup(g, 0, -11, NULL);
    SetGroupSpacing(h, 5, 3);

    Cn3D_lOnOffItem[0] = NULL;  /* Cn3D_pupPBB includes off setting */
    Cn3D_lOnOffItem[1] = NULL;  /* Cn3D_pupNABB includes off setting */

    ppt1 =
        StaticPrompt(h, "Category", 0, stdLineHeight + 5, systemFont, 'c');
    ppt[0] =
        StaticPrompt(h, "Protein Backbone", 0, popupMenuHeight,
                     systemFont, 'l');
    ppt[1] =
        StaticPrompt(h, "Nucleotide Backbone", 0, popupMenuHeight,
                     systemFont, 'l');
    for (k = 2; k < 10; k++) {
        ppt[k] =
            StaticPrompt(h, categorynames[k], 0, popupMenuHeight,
                         systemFont, 'l');
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt1, (HANDLE) ppt[1], NULL);

    ppt2 =
        StaticPrompt(h, "On/Off", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_pupPBB = PopupList(h, FALSE, PopupOnOffProc);
    PopupItem(Cn3D_pupPBB, "alpha C trace");
    PopupItem(Cn3D_pupPBB, "partial atom");
    PopupItem(Cn3D_pupPBB, "all atoms");
    PopupItem(Cn3D_pupPBB, "none");
    Cn3D_pupNABB = PopupList(h, FALSE, PopupOnOffProc);
    PopupItem(Cn3D_pupNABB, "P trace");
    PopupItem(Cn3D_pupNABB, "partial atom");
    PopupItem(Cn3D_pupNABB, "all atoms");
    PopupItem(Cn3D_pupNABB, "none");
    for (k = 2; k < 10; k++) {
        Cn3D_lOnOffItem[k] = CheckBox(h, "", ButtonOnOffProc);
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt2, (HANDLE) Cn3D_pupPBB, NULL);

    ppt3 =
        StaticPrompt(h, "Drawing Style", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 10; k++) {
        if (k == 9) {
            StaticPrompt(h, " ", 0, popupMenuHeight, systemFont, 'l');
            Cn3D_pupRenderStyle[k] = NULL;
        } else {
            Cn3D_pupRenderStyle[k] = RenderStyle(h, k);
        }
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt3,
                 (HANDLE) Cn3D_pupRenderStyle[0], NULL);

    ppt4 = StaticPrompt(h, "Color", 0, stdLineHeight + 5, systemFont, 'c');
    for (k = 0; k < 10; k++) {
        if (k == 6 || k == 9) {
            StaticPrompt(h, " ", 0, popupMenuHeight, systemFont, 'l');
            Cn3D_pupColorStyle[k] = NULL;
        } else {
            Cn3D_pupColorStyle[k] = ColorStyle(h, k);
        }
    }
    AlignObjects(ALIGN_CENTER, (HANDLE) ppt4,
                 (HANDLE) Cn3D_pupColorStyle[0], NULL);

#ifdef _OPENGL
    bg = HiddenGroup(g, 2, 1, NULL);
    SetGroupSpacing(bg, 10, 3);
    ppt5 = StaticPrompt(bg, "Background Color:", 0, stdLineHeight + 5, systemFont, 'c');
    Cn3D_pupBackgroundColor = PopupList(bg, FALSE, PopupOnOffProc);
    for (k=0; bgColorAList[k].name; k++)
        PopupItem(Cn3D_pupBackgroundColor, bgColorAList[k].name);
    SetEnumPopup(Cn3D_pupBackgroundColor, bgColorAList, (UIEnum) 1);
#endif

    Cn3D_immediateRedraw =
        CheckBox(g, "Immediate Redraw on Every Change", NULL);

    Cn3D_bRedraw = PushButton(g, "Apply!", fnCn3D_RedrawWrapper);

    for (k = 2; k < 10; k++) {
        GetPosition(Cn3D_lOnOffItem[k], &btnPos);
        GetPosition(ppt[k], &pptPos);
        delta =
            (pptPos.bottom + pptPos.top) / 2 - (btnPos.bottom +
                                                btnPos.top) / 2;
        if (delta != 0) {
            OffsetRect(&btnPos, 0, delta);
            SetPosition(Cn3D_lOnOffItem[k], &btnPos);
            AdjustPrnt((Nlm_GraphiC) Cn3D_lOnOffItem[k], &btnPos, FALSE);
        }
    }

    AlignObjects(ALIGN_CENTER, (HANDLE) Cn3D_pupPBB, (HANDLE) Cn3D_pupNABB,
                 (HANDLE) Cn3D_lOnOffItem[2], (HANDLE) Cn3D_lOnOffItem[3],
                 (HANDLE) Cn3D_lOnOffItem[4], (HANDLE) Cn3D_lOnOffItem[5],
                 (HANDLE) Cn3D_lOnOffItem[6], (HANDLE) Cn3D_lOnOffItem[7],
                 (HANDLE) Cn3D_lOnOffItem[8], (HANDLE) Cn3D_lOnOffItem[9],
                 NULL);

#ifdef _OPENGL
    AlignObjects(ALIGN_CENTER, (HANDLE) bg, (HANDLE) g, NULL);
#endif

    AlignObjects(ALIGN_CENTER, (HANDLE) Cn3D_bRedraw,
                 (HANDLE) Cn3D_immediateRedraw, (HANDLE) h, NULL);

    ResetRenderCtrls();
    return g;
}


/****************************************************************************/


static void Cn3D_SetRenderKeep(PRK prkThis)
{
    if (prkThis == NULL) return;
    prkThis->NodeWhat = (Byte) CONVERT_ALL;
    prkThis->NodeType = (Byte) CONVERT_ALL;
    prkThis->Bond = (Byte) NO_BOND; /*  use define */
    prkThis->Atom = (Byte) ATOM_NONE;
    prkThis->BondWidth = (float) 0;
    prkThis->LJust = 0;
    prkThis->LStyle = 0;
    prkThis->LScale = 1;
    return;
}


PRK LIBCALL NewRenderKeep(void)
{
    PRK prkThis = NULL;
    prkThis = (PRK) MemNew((size_t) (sizeof(RK)));
    if (!prkThis)
        return NULL;
    Cn3D_SetRenderKeep(prkThis);
    return prkThis;
}

static void Cn3D_DupRenderKeep(PRK prkDest, PRK prkSrc)
{
    if (prkDest == NULL || prkSrc == NULL) return;
    prkDest->NodeWhat = prkSrc->NodeWhat;
    prkDest->NodeType = prkSrc->NodeType;
    DDV_CopyColorCell(&(prkDest->Color), &(prkSrc->Color)); /* a fixed color */
    prkDest->Bond = prkSrc->Bond; /*  use define */
    prkDest->Atom = prkSrc->Atom;
    prkDest->BondWidth = prkSrc->BondWidth;
    prkDest->LJust = prkSrc->LJust;
    prkDest->LStyle = prkSrc->LStyle;
    prkDest->LScale = prkSrc->LScale;

}

PRK LIBCALL CopyRenderKeep(PRK prkThis)
{
    PRK prkNew = NULL;
    prkNew = NewRenderKeep();
    if (!prkNew) return NULL;
    Cn3D_DupRenderKeep(prkNew, prkThis);
    return prkNew;
}

void LIBCALL FreeRenderKeep(PRK prkThis)
{
    MemFree(prkThis);
}

/*
static void RotTransScale(PMSD pmsdThis, FloatLo fX,
                          FloatLo fY,
                          FloatLo fZ,
                          Int4Ptr piX, Int4Ptr piY, Int4Ptr piZ)
{
    FloatHi fXTemp, fYTemp, fZTemp;

    if (!pmsdThis)
        return;

    *piX = 0;
    *piY = 0;
    *piZ = 0;
    if (pmsdThis->pflTranslate)
    {
        fX = fX - pmsdThis->pflTranslate[0];
        fY = fY - pmsdThis->pflTranslate[1];
        fZ = fZ - pmsdThis->pflTranslate[2];
    }
    if (pmsdThis->ppflRotate)
    {
        fXTemp = (FloatHi) (fX * pmsdThis->ppflRotate[0][0] +
                            fY * pmsdThis->ppflRotate[1][0] +
                            fZ * pmsdThis->ppflRotate[2][0]);
        fYTemp = (FloatHi) (fX * pmsdThis->ppflRotate[0][1] +
                            fY * pmsdThis->ppflRotate[1][1] +
                            fZ * pmsdThis->ppflRotate[2][1]);
        fZTemp = (FloatHi) (fX * pmsdThis->ppflRotate[0][2] +
                            fY * pmsdThis->ppflRotate[1][2] +
                            fZ * pmsdThis->ppflRotate[2][2]);
        fX = (FloatLo) fXTemp;
        fY = (FloatLo) fYTemp;
        fZ = (FloatLo) fZTemp;
    }
    *piX = (Int4) (fX * VIEWSCALE);
    *piY = (Int4) (fY * VIEWSCALE);
    *piZ = (Int4) (fZ * VIEWSCALE);
    return;
}
*/

/***********************************************************/
/* These are the lowest-level calls to the 3D drawing      */
/* at this point all the decisions have been made          */


void LIBCALL RenderObject(PVNMO pvnmoThis)
{
    PMOD pmodThis = NULL;
    PMSD pmsdThis = NULL;
    PMGD pmgdThis = NULL;
    PMMD pmmdThis = NULL;
    PFB pfbThis = NULL;
    PFB pfbParent = NULL;
    PARS pars = NULL;
    Int4 i = 0;
    FloatLo fXFrom, fYFrom, fZFrom;
    FloatLo fXTo, fYTo, fZTo;
    Int4 iXFrom, iYFrom, iZFrom;
    Int4 iXTo, iYTo, iZTo;
    Int4 iCylRadius;
#ifndef _OPENGL
    Nlm_Prim3D poly;
#endif
    DDV_ColorCell iColor;

#ifdef DEBUG_N
    printf("RenderObject\n");
#endif
    if (!pvnmoThis)
        return;
    pmodThis = (PMOD) pvnmoThis->data.ptrvalue;
    if (!pmodThis)
        return;

    if (!pmodThis->ppflObject)
        return;
    if (!pmodThis->bVisible)
        return;

    pmsdThis = ToMSDParent((PFB) pmodThis);
    if (!pmsdThis)
        return;

    pars = (PARS) pmsdThis->pGlobalPARS;
    if (!pars)
        return;

    if (Cn3d_ColorNow == C_BYCHAIN) {
        if (pmodThis->pvnContains == NULL)
            DDV_SetColorInCell(&iColor, Cn3d_PaletteRGB[ColorNumKinBB[0]]);
        else {
            pfbThis = (PFB) pmodThis->pvnContains->data.ptrvalue;
            if (!pfbThis)
                return;
            if (!IsMoleculeNode(pfbThis))
                pfbParent = (PFB) GetParentMol(pfbThis);
            else
                pfbParent = pfbThis;
            if (pfbParent) {
                pmmdThis = (PMMD) pfbParent;
                DDV_SetColorInCell(&iColor,
                    Cn3d_PaletteRGB[
                        ColorNumKinBB[(pmmdThis->pdnmmLink->choice % KIN_COLOR_NUM)]]);
            } else {
                DDV_SetColorInCell(&iColor, Cn3d_PaletteRGB[ColorNumKinBB[0]]);
            }
        }
    }

    else if (Cn3d_ColorNow == C_BYOBJECT) {
        DDV_SetColorInCell(&iColor,
            Cn3d_PaletteRGB[bObjectColors[(Cn3d_lSlaveNum % NUM_SLAVES)]]);
    }

    else if (Cn3d_ColorNow == C_BYSECSTRUC || Cn3d_ColorNow == C_BYDOMAIN) {
        if (pmodThis->pvnContains == NULL) {
            DDV_SetColorInCell(&iColor, Cn3d_PaletteRGB[ColorNumKinBB[0]]);
        } else {
            pfbThis = (PFB) pmodThis->pvnContains->data.ptrvalue;
            if (!pfbThis)
                return;
            if (!IsGraphNode(pfbThis))
                pfbParent = (PFB) GetParentGraph(pfbThis);
            else
                pfbParent = pfbThis;
            if (pfbParent && IsGraphAminoAcid(pfbParent)) {
                pmgdThis = (PMGD) pfbParent;
                if (Cn3d_ColorNow == C_BYSECSTRUC)
                    DDV_CopyColorCell(&iColor, ColorFromSS(pmgdThis));
                if (Cn3d_ColorNow == C_BYDOMAIN)
                    DDV_SetColorInCell(&iColor,
                        Cn3d_PaletteRGB[ColorNumKinBB[(pmgdThis->iDomain % KIN_COLOR_NUM)]]);
            } else {
                DDV_SetColorInCell(&iColor, Cn3d_PaletteRGB[ColorNumKinBB[0]]);
            }
        }
    }

    else DDV_SetColorInCell(&iColor, Cn3d_PaletteRGB[ColorNumKinBB[0]]);

    if (Cn3d_ColorPass) {
        DDV_RequestColor(&Cn3D_ColorData.pDDVColorGlobal->Palette, &iColor);
        return;
    }

#ifdef _OPENGL
    OGL_LoadName(NULL); /* disable highlighting of objects */
#endif

    switch (pmodThis->bWhat) {
    case OBJ_CYLINDER:
        /* switch to/from order, so that arrow is pointing from N to C */
        fXTo = (FloatLo) (pmodThis->ppflObject[0][0]);
        fYTo = (FloatLo) (pmodThis->ppflObject[0][1]);
        fZTo = (FloatLo) (pmodThis->ppflObject[0][2]);
        fXFrom = (FloatLo) (pmodThis->ppflObject[1][0]);
        fYFrom = (FloatLo) (pmodThis->ppflObject[1][1]);
        fZFrom = (FloatLo) (pmodThis->ppflObject[1][2]);
        /*
        RotTransScale(pmsdThis, fXFrom, fYFrom, fZFrom, &iXFrom, &iYFrom, &iZFrom);
        RotTransScale(pmsdThis, fXTo, fYTo, fZTo, &iXTo, &iYTo, &iZTo);
        */

#ifdef _OPENGL
        /*
        fXTo = iXFrom / VIEWSCALE;
        fYTo = iYFrom / VIEWSCALE;
        fZTo = iZFrom / VIEWSCALE;
        fXFrom = iXTo / VIEWSCALE;
        fYFrom = iYTo / VIEWSCALE;
        fZFrom = iZTo / VIEWSCALE;
        */
        OGL_AddCylinder3D(Cn3D_ColorData.OGL_Data, &iColor,
                          (FloatHi) fXFrom, (FloatHi) fYFrom, (FloatHi) fZFrom, TRUE,
                          (FloatHi) fXTo, (FloatHi) fYTo, (FloatHi) fZTo, TRUE,
                          /*(FloatHi) pmodThis->flRadius*/ 1.8,
                          GetValue(Cn3D_pupQualBigCylSides) + QUAL_SIZE_MIN - 1,
                          (pars->ObjectArrows & ARROW_CYL));

#else                           /* _OPENGL */
        iCylRadius = (Int4) (pmodThis->flRadius * VIEWSCALE);
        AddCylinder3D(pic, NULL, (BigScalar) pmodThis,
                      Cn3d_LayerNow,
                      DDV_ColorIndex(Cn3D_ColorData.pDDVColorGlobal->
                                     Palette, &iColor), iXFrom, iYFrom,
                      iZFrom, iXTo, iYTo, iZTo, iCylRadius);
#endif                          /* else _OPENGL */
        return;
    case OBJ_BRICK:
#ifdef _OPENGL
        {
            FloatHi Nterm[3], Cterm[3], norm[3];
            /*Int4 xv, yv, zv;*/
            
            /*RotTransScale(pmsdThis,
                          pmodThis->ppflObject[0][0],
                          pmodThis->ppflObject[0][1],
                          pmodThis->ppflObject[0][2], &xv, &yv, &zv);
            Nterm[0] = xv; Nterm[1] = yv; Nterm[2] = zv;*/
            /*RotTransScale(pmsdThis,
                          pmodThis->ppflObject[3][0],
                          pmodThis->ppflObject[3][1],
                          pmodThis->ppflObject[3][2], &xv, &yv, &zv);
            Nterm[0] += xv; Nterm[1] += yv; Nterm[2] += zv;
            Nterm[0] /= 2000000.0; Nterm[1] /= 2000000.0; Nterm[2] /= 2000000.0;*/
            Nterm[0] = (pmodThis->ppflObject[0][0] + pmodThis->ppflObject[3][0]) / 2;
            Nterm[1] = (pmodThis->ppflObject[0][1] + pmodThis->ppflObject[3][1]) / 2;
            Nterm[2] = (pmodThis->ppflObject[0][2] + pmodThis->ppflObject[3][2]) / 2;
            
            /*RotTransScale(pmsdThis,
                          pmodThis->ppflObject[4][0],
                          pmodThis->ppflObject[4][1],
                          pmodThis->ppflObject[4][2], &xv, &yv, &zv);
            Cterm[0] = xv; Cterm[1] = yv; Cterm[2] = zv;
            RotTransScale(pmsdThis,
                          pmodThis->ppflObject[7][0],
                          pmodThis->ppflObject[7][1],
                          pmodThis->ppflObject[7][2], &xv, &yv, &zv);
            Cterm[0] += xv; Cterm[1] += yv; Cterm[2] += zv;
            Cterm[0] /= 2000000.0; Cterm[1] /= 2000000.0; Cterm[2] /= 2000000.0;*/
            Cterm[0] = (pmodThis->ppflObject[4][0] + pmodThis->ppflObject[7][0]) / 2;
            Cterm[1] = (pmodThis->ppflObject[4][1] + pmodThis->ppflObject[7][1]) / 2;
            Cterm[2] = (pmodThis->ppflObject[4][2] + pmodThis->ppflObject[7][2]) / 2;
            
            /*RotTransScale(pmsdThis,
                          pmodThis->ppflObject[2][0],
                          pmodThis->ppflObject[2][1],
                          pmodThis->ppflObject[2][2], &xv, &yv, &zv);
            norm[0] = xv; norm[1] = yv; norm[2] = zv;
            RotTransScale(pmsdThis,
                          pmodThis->ppflObject[0][0],
                          pmodThis->ppflObject[0][1],
                          pmodThis->ppflObject[0][2], &xv, &yv, &zv);
            norm[0] -= xv; norm[1] -= yv; norm[2] -= zv;*/
            norm[0] = pmodThis->ppflObject[2][0] - pmodThis->ppflObject[0][0];
            norm[1] = pmodThis->ppflObject[2][1] - pmodThis->ppflObject[0][1];
            norm[2] = pmodThis->ppflObject[2][2] - pmodThis->ppflObject[0][2];
            
            OGL_AddBrick3D(Cn3D_ColorData.OGL_Data, &iColor,
                           Nterm, Cterm, norm, 2.0, 0.5,
                           (pars->ObjectArrows & ARROW_BRICK));

        }
#else                           /* _OPENGL */
        fXFrom = pmodThis->ppflObject[0][0];
        fYFrom = pmodThis->ppflObject[0][1];
        fZFrom = pmodThis->ppflObject[0][2];
        fXTo = pmodThis->ppflObject[1][0];
        fYTo = pmodThis->ppflObject[1][1];
        fZTo = pmodThis->ppflObject[1][2];
        /*
        RotTransScale(pmsdThis, fXFrom,
                      fYFrom, fZFrom, &iXFrom, &iYFrom, &iZFrom);
        RotTransScale(pmsdThis, fXTo, fYTo, fZTo, &iXTo, &iYTo, &iZTo);
        */
        Cn3d_AnyPrim = TRUE;
        poly = AddPoly3D(pic, NULL, (BigScalar) pmodThis,
                         Cn3d_LayerNow,
                         DDV_ColorIndex(Cn3D_ColorData.pDDVColorGlobal->
                                        Palette, &iColor), iXFrom, iYFrom,
                         iZFrom, iXTo, iYTo, iZTo);
#ifdef DEBUG_N
        printf("%ld %ld %ld\n", (long) iXFrom, (long) iYFrom,
               (long) iZFrom);
        printf("%ld %ld %ld\n", (long) iXTo, (long) iYTo, (long) iZTo);
#endif
        for (i = 2; i < 8; i++) {
            fXTo = pmodThis->ppflObject[i][0];
            fYTo = pmodThis->ppflObject[i][1];
            fZTo = pmodThis->ppflObject[i][2];
            /*RotTransScale(pmsdThis, fXTo, fYTo, fZTo, &iXTo, &iYTo, &iZTo);*/
            Cn3d_AnyPrim = TRUE;
            AddVertPoly3D(pic, poly, iXTo, iYTo, iZTo);
#ifdef DEBUG_N
            printf("%ld %ld %ld\n", (long) iXTo, (long) iYTo, (long) iZTo);
#endif
        }
#endif                          /* else _OPENGL */

#ifdef DEBUG_N
        printf("%ld %ld %ld\n", (long) iXFrom, (long) iYFrom,
               (long) iZFrom);
#endif
        return;
    case OBJ_SPHERE:
    case OBJ_CONE:
    case OBJ_TMESH:
    case OBJ_TRIANGLES:
    default:
#ifdef DEBUG_N
        printf("Other  \n");
#endif
        return;
    }
}



  /* Mode determines how to combine PALD's */
#define RL_CENTER      1        /* put at paldCenter */
#define RL_CENTERPLUSY 2        /* put at paldCenter plus 1x Scale in y direction */
#define RL_EXTRAPOL    3        /* paldTo + (paldTo - paldFrom)/2 */
#define RL_BETACARB    4        /* approximates direction of H-alphs */

static void RenderLabel(PDNMS pdnmsThis, CharPtr pclabel,
                        PALD paldCenter, PALD paldFrom, PALD paldTo,
                        DDV_ColorCell * iColor, Uint1 Just, Int2 Scale,
                        Int2 Mode)
{
    FloatLo fXFrom, fYFrom, fZFrom;
    Int4 iX, iY, iZ;
    FloatLo fXTo, fYTo, fZTo;
    FloatLo fXCen, fYCen, fZCen;
    FloatLo fXt, fYt, fZt, l;
    PMSD pmsdThis = NULL;
    PALD paldPrim = NULL;
    Int2 flags;

    if (!pdnmsThis)
        return;
    if (!pclabel)
        return;
    if (((Mode == RL_CENTER) || (Mode == RL_BETACARB))
        && (paldCenter == NULL)) return;
    if ((Mode == RL_CENTERPLUSY) && (paldCenter == NULL))
        return;
    if (((Mode == RL_EXTRAPOL) || (Mode == RL_BETACARB))
        && ((paldFrom == NULL) || (paldTo == NULL)))
        return;

    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

    if (paldFrom) {
        fXFrom = (FloatLo) (paldFrom->pflvData[0]);
        fYFrom = (FloatLo) (paldFrom->pflvData[1]);
        fZFrom = (FloatLo) (paldFrom->pflvData[2]);
    }
    if (paldTo) {
        fXTo = (FloatLo) (paldTo->pflvData[0]);
        fYTo = (FloatLo) (paldTo->pflvData[1]);
        fZTo = (FloatLo) (paldTo->pflvData[2]);
        paldPrim = paldTo;
    }
    if (paldCenter) {
        fXCen = (FloatLo) (paldCenter->pflvData[0]);
        fYCen = (FloatLo) (paldCenter->pflvData[1]);
        fZCen = (FloatLo) (paldCenter->pflvData[2]);
        paldPrim = paldCenter;
    }


    switch (Mode) {
    case RL_CENTER:
        break;
    case RL_CENTERPLUSY:
        fYCen = fYCen + Scale;
        break;
    case RL_EXTRAPOL:
        fXCen = fXTo + (fXTo - fXFrom) / 2.0;
        fYCen = fYTo + (fYTo - fYFrom) / 2.0;
        fZCen = fZTo + (fZTo - fZFrom) / 2.0;
        break;
    case RL_BETACARB:
        fXt = (fXCen - fXFrom) + (fXCen - fXTo);
        fYt = (fYCen - fYFrom) + (fYCen - fYTo);
        fZt = (fZCen - fZFrom) + (fZCen - fZTo);
        l = sqrt(fXt*fXt + fYt*fYt + fZt*fZt) / 1.5;
        fXCen += fXt / l;
        fYCen += fYt / l;
        fZCen += fZt / l;
        fXt = (fYCen - fYFrom) * (fZCen - fZTo) - (fZCen - fZFrom) * (fYCen - fYTo);
        fYt = (fZCen - fZFrom) * (fXCen - fXTo) - (fXCen - fXFrom) * (fZCen - fZTo);
        fZt = (fXCen - fXFrom) * (fYCen - fYTo) - (fYCen - fYFrom) * (fXCen - fXTo);
        l = sqrt(fXt*fXt + fYt*fYt + fZt*fZt) / 1.1;
        fXCen += fXt / l;
        fYCen += fYt / l;
        fZCen += fZt / l;
        break;
    }

    /*RotTransScale(pmsdThis, fXCen, fYCen, fZCen, &iX, &iY, &iZ);*/

#ifdef _OPENGL
    flags = 0;
    if (Just & LA_CENTER)
        flags = OGLTEXT3D_MIDDLE | OGLTEXT3D_CENTER;
    /* if (Just & LA_FRONT) flags = flags | OGLTEXT3D_FRONT;  hard to do in OpenGL
       todo: get rid of the ui for this
     */

    OGL_AddText3D(Cn3D_ColorData.OGL_Data, iColor, pclabel,
                  (FloatHi) fXCen, (FloatHi) fYCen, (FloatHi) fZCen,
                  flags);
#else                           /* _OPENGL */
    flags = 0;
    if (Just & LA_LEFT)
        flags = flags | TEXT3D_LEFT;
    if (Just & LA_RIGHT)
        flags = flags | TEXT3D_RIGTH;
    if (Just & LA_UPPER)
        flags = flags | TEXT3D_UPPER;
    if (Just & LA_LOWER)
        flags = flags | TEXT3D_LOWER;
    /* only LA_CENTER and LA_FRONT are used */
    if (Just & LA_CENTER)
        flags = TEXT3D_MIDDLE | TEXT3D_CENTER;
    if (Just & LA_FRONT)
        flags = flags | TEXT3D_FRONT;
    Cn3d_AnyPrim = TRUE;
    if (paldPrim->pGraphic == NULL)
        paldPrim->pGraphic =
            (Pointer) AddSegment3D(pic, NULL, (BigScalar) paldPrim,
                                   Cn3d_LayerNow);

    AddText3D(pic, (Segment3D) paldPrim->pGraphic, (BigScalar) 0,
              Cn3d_LayerNow,
              DDV_ColorIndex(Cn3D_ColorData.pDDVColorGlobal->Palette,
                             iColor), pclabel, iX, iY, iZ,
              (Uint4) (Scale * VIEWSCALE / 2), 0, flags);
#endif                          /* else _OPENGL */
    return;

}

/*
void ApplyTranslationRotationF(FloatLo * x, FloatLo * y, FloatLo * z,
                               PMSD pmsdParent)
{
    if (pmsdParent->pflTranslate) {
        Message(MSG_POST, "non-NULL pmsdParent->pflTranslate");
        *x = (*x) - pmsdParent->pflTranslate[0];
        *y = (*y) - pmsdParent->pflTranslate[1];
        *z = (*z) - pmsdParent->pflTranslate[2];
    }
    if (pmsdParent->ppflRotate) {
        FloatLo tx, ty, tz;
        Message(MSG_POST, "non-NULL pmsdParent->ppflRotate");
        tx = (FloatLo) (*x * pmsdParent->ppflRotate[0][0] +
                        *y * pmsdParent->ppflRotate[1][0] +
                        *z * pmsdParent->ppflRotate[2][0]);
        ty = (FloatLo) (*x * pmsdParent->ppflRotate[0][1] +
                        *y * pmsdParent->ppflRotate[1][1] +
                        *z * pmsdParent->ppflRotate[2][1]);
        tz = (FloatLo) (*x * pmsdParent->ppflRotate[0][2] +
                        *y * pmsdParent->ppflRotate[1][2] +
                        *z * pmsdParent->ppflRotate[2][2]);
        *x = tx;
        *y = ty;
        *z = tz;
    }
}
*/

#ifdef _OPENGL
static void RenderBond(PALD paldFrom, Boolean bCapFrom, PALD paldTo, Boolean bCapTo,
                       DDV_ColorCell * iColor, FloatLo fCylRadius)
#else
static void RenderBond(PALD paldFrom, PALD paldTo, DDV_ColorCell * iColor,
                       FloatLo fCylRadius)
#endif
{

    /* paldFrom owns the bond */

    FloatLo fXFrom, fYFrom, fZFrom;
    /*
    FloatLo fXTempF, fYTempF, fZTempF;
    FloatLo fXTempT, fYTempT, fZTempT;
    */
    FloatLo fXTo, fYTo, fZTo;
#ifndef _OPENGL
    Int4 iXTo, iYTo, iZTo;
    Int4 iXFrom, iYFrom, iZFrom;
    Int4 iCylRadius;
#endif
    PMSD pmsdParent = NULL;
    PMAD pmadFrom = NULL;
    PMAD pmadTo = NULL;

#ifdef DEBUG_N
/*printf("%f\n", (float) fCylRadius);*/
#endif

    pmadFrom = (PMAD) paldFrom->pfbParent;
    pmadTo = (PMAD) paldTo->pfbParent;
    if ((!pmadFrom) || (!pmadTo))
        return;
    if ((AtomicNumber(pmadFrom) == Atom_element_h) &&
        (Cn3d_DoHydrogens == FALSE)) return;
    if ((AtomicNumber(pmadTo) == Atom_element_h) &&
        (Cn3d_DoHydrogens == FALSE)) return;
    pmsdParent = ToMSDParent((PFB) pmadFrom);
    fXFrom = (FloatLo) (paldFrom->pflvData[0]);
    fYFrom = (FloatLo) (paldFrom->pflvData[1]);
    fZFrom = (FloatLo) (paldFrom->pflvData[2]);
    fXTo = (FloatLo) (paldTo->pflvData[0]);
    fYTo = (FloatLo) (paldTo->pflvData[1]);
    fZTo = (FloatLo) (paldTo->pflvData[2]);

/*
    ApplyTranslationRotationF(&fXFrom, &fYFrom, &fZFrom, pmsdParent);
    ApplyTranslationRotationF(&fXTo, &fYTo, &fZTo, pmsdParent);
*/

    Cn3d_AnyPrim = TRUE;
    /* scale */
#ifndef _OPENGL
    iXFrom = (Int4) (fXFrom * VIEWSCALE);
    iYFrom = (Int4) (fYFrom * VIEWSCALE);
    iZFrom = (Int4) (fZFrom * VIEWSCALE);
    iXTo = (Int4) (fXTo * VIEWSCALE);
    iYTo = (Int4) (fYTo * VIEWSCALE);
    iZTo = (Int4) (fZTo * VIEWSCALE);
    iCylRadius = (Int4) (fCylRadius * VIEWSCALE);
    if (paldFrom->pGraphic == NULL)
        paldFrom->pGraphic = (Pointer) AddSegment3D(pic, NULL,
                                                    (BigScalar) paldFrom,
                                                    Cn3d_LayerNow);
#endif                          /* ndef _OPENGL */

    if (fCylRadius < CYL_THRESHOLD) {
#ifdef _OPENGL
        OGL_AddLine3D(Cn3D_ColorData.OGL_Data, iColor, (FloatHi) fXFrom,
                      (FloatHi) fYFrom, (FloatHi) fZFrom, (FloatHi) fXTo,
                      (FloatHi) fYTo, (FloatHi) fZTo);

#else                           /* _OPENGL */
        AddLine3D(pic, (Segment3D) paldFrom->pGraphic,
                  (BigScalar) paldFrom, Cn3d_LayerNow,
                  DDV_ColorIndex(Cn3D_ColorData.pDDVColorGlobal->Palette, iColor),
                  iXFrom, iYFrom, iZFrom, iXTo, iYTo, iZTo);
#endif                          /* else _OPENGL */
    } else {
#ifdef _OPENGL
        OGL_AddCylinder3D(Cn3D_ColorData.OGL_Data, iColor,
                          (FloatHi) fXFrom, (FloatHi) fYFrom, (FloatHi) fZFrom, bCapFrom,
                          (FloatHi) fXTo, (FloatHi) fYTo, (FloatHi) fZTo, bCapTo,
                          (FloatHi) fCylRadius,
                          GetValue(Cn3D_pupQualBondCylSides) + QUAL_SIZE_MIN - 1, FALSE);

#else                           /* _OPENGL */
        AddCylinder3D(pic, (Segment3D) paldFrom->pGraphic,
                      (BigScalar) paldFrom, Cn3d_LayerNow,
                      DDV_ColorIndex(Cn3D_ColorData.pDDVColorGlobal->
                                     Palette, iColor), iXFrom, iYFrom,
                      iZFrom, iXTo, iYTo, iZTo, iCylRadius);
#endif
    }                          /* else _OPENGL */
    return;
}


/* returns a pointer to the next atom down a linear chain of
   virual bonds 1-2-3; used when bonds describe a virtualBB  (thiessen) */
static PMAD GetAtomAfter(PALD pald1, PALD pald2)
{
    ValNodePtr pvnB2;
    PMAD pmad1, pmad2, pmad3 = NULL;
    PMBD pmbd;

    pmad1 = (PMAD) pald1->pfbParent;
    pmad2 = (PMAD) pald2->pfbParent;
    pvnB2 = pmad2->pvnBonds;

    /* find virtual bond from 2 to an atom other than 1 */
    while (pvnB2) {
        pmbd = (PMBD) pvnB2->data.ptrvalue;
        if (IsBondVirtual(pmbd)) {
            if (pmbd->pmadTo == pmad2)
                pmad3 = pmbd->pmadFrom;
            else
                pmad3 = pmbd->pmadTo;
            if (pmad3 && pmad3 != pmad1)
                break;
            pmad3 = NULL;
        }
        pvnB2 = pvnB2->next;
    }
    return pmad3;
}

#ifdef _OPENGL
static void RenderHalfWormBond(PALD pald1, Boolean bCap1, PALD pald2, Boolean bCap2,
                               DDV_ColorCell * iColor, FloatLo fCylRadius,
                               Int4 iModel)
#else
static void RenderHalfWormBond(PALD pald1, PALD pald2,
                               DDV_ColorCell * iColor, FloatLo fCylRadius,
                               Int4 iModel)
#endif
{
    FloatLo fX1, fY1, fZ1, fX2, fY2, fZ2, fX3, fY3, fZ3, fX0, fY0, fZ0;
    PMAD pmad0, pmad1, pmad2, pmad3;
    PALD pald0, pald3;
    PMSD pmsdParent;

    pmad1 = (PMAD) pald1->pfbParent;
    pmad2 = (PMAD) pald2->pfbParent;
    pmsdParent = ToMSDParent((PFB) pmad1);

    fX1 = (FloatLo) (pald1->pflvData[0]);
    fY1 = (FloatLo) (pald1->pflvData[1]);
    fZ1 = (FloatLo) (pald1->pflvData[2]);
    fX2 = (FloatLo) (pald2->pflvData[0]);
    fY2 = (FloatLo) (pald2->pflvData[1]);
    fZ2 = (FloatLo) (pald2->pflvData[2]);
/*
    ApplyTranslationRotationF(&fX1, &fY1, &fZ1, pmsdParent);
    ApplyTranslationRotationF(&fX2, &fY2, &fZ2, pmsdParent);
*/

    pmad0 = GetAtomAfter(pald2, pald1);
    if (pmad0) {
        pald0 = GetAtomAltLocs(pmad0, iModel);
        if (!pald0) pald0 = pald1;
        fX0 = (FloatLo) (pald0->pflvData[0]);
        fY0 = (FloatLo) (pald0->pflvData[1]);
        fZ0 = (FloatLo) (pald0->pflvData[2]);
        /*ApplyTranslationRotationF(&fX0, &fY0, &fZ0, pmsdParent);*/
    } else {
        fX0 = fX1;
        fY0 = fY1;
        fZ0 = fZ1;
    }
    pmad3 = GetAtomAfter(pald1, pald2);
    if (pmad3) {
        pald3 = GetAtomAltLocs(pmad3, iModel);
        if (!pald3) pald3 = pald2;
        fX3 = (FloatLo) (pald3->pflvData[0]);
        fY3 = (FloatLo) (pald3->pflvData[1]);
        fZ3 = (FloatLo) (pald3->pflvData[2]);
        /*ApplyTranslationRotationF(&fX3, &fY3, &fZ3, pmsdParent);*/
    } else {
        fX3 = fX2;
        fY3 = fY2;
        fZ3 = fZ2;
    }

    if (fCylRadius < CYL_THRESHOLD)
        fCylRadius = 0.0;
    Cn3d_AnyPrim = TRUE;

#ifdef _OPENGL
    Nlm_AddHalfWorm3D(Cn3D_ColorData.OGL_Data, iColor,
                      (FloatHi) fX0, (FloatHi) fY0, (FloatHi) fZ0,
                      (FloatHi) fX1, (FloatHi) fY1, (FloatHi) fZ1,
                      (FloatHi) fX2, (FloatHi) fY2, (FloatHi) fZ2,
                      (FloatHi) fX3, (FloatHi) fY3, (FloatHi) fZ3,
                      bCap1, bCap2, (FloatHi) fCylRadius,
                      GetValue(Cn3D_pupQualWormSegments) - 1 + QUAL_SIZE_MIN/2,
                      2 * (GetValue(Cn3D_pupQualWormSides) - 1) + QUAL_SIZE_MIN);
#else
    if (pald1->pGraphic == NULL)
        pald1->pGraphic = (Pointer) AddSegment3D(pic, NULL,
                                                 (BigScalar) pald1,
                                                 Cn3d_LayerNow);
    Nlm_AddHalfWorm3D(pic, (Segment3D) pald1->pGraphic,
                      (BigScalar) pald1, Cn3d_LayerNow,
                      DDV_ColorIndex(Cn3D_ColorData.pDDVColorGlobal->Palette, iColor),
                      (FloatHi) fX0, (FloatHi) fY0, (FloatHi) fZ0,
                      (FloatHi) fX1, (FloatHi) fY1, (FloatHi) fZ1,
                      (FloatHi) fX2, (FloatHi) fY2, (FloatHi) fZ2,
                      (FloatHi) fX3, (FloatHi) fY3, (FloatHi) fZ3,
                      (FloatHi) fCylRadius, 3);
#endif /* _OPENGL */

    return;
}


void LIBCALL RenderAnAtom(PALD paldAtom, DDV_ColorCell * iColor,
                          FloatLo fRadius, FloatHi lfAlpha)
{
#ifndef _OPENGL
    Int4 iXAtom, iYAtom, iZAtom;
    Int4 iRadius;
#endif
    FloatLo fXAtom, fYAtom, fZAtom;
    PMSD pmsdParent = NULL;
    PMAD pmadFrom = NULL;
    PMAD pmadAtom = NULL;
    if (!paldAtom)
        return;
    pmadAtom = (PMAD) paldAtom->pfbParent;
    if (!pmadAtom)
        return;
    if ((AtomicNumber(pmadAtom) == Atom_element_h) &&
        (Cn3d_DoHydrogens == FALSE)) return;
    pmsdParent = ToMSDParent((PFB) pmadAtom);
    fXAtom = (FloatLo) (paldAtom->pflvData[0]);
    fYAtom = (FloatLo) (paldAtom->pflvData[1]);
    fZAtom = (FloatLo) (paldAtom->pflvData[2]);

    /*ApplyTranslationRotationF(&fXAtom, &fYAtom, &fZAtom, pmsdParent);*/

#ifdef _OPENGL
    OGL_AddSphere3D(Cn3D_ColorData.OGL_Data, (DDV_ColorCell *) iColor,
                    (FloatHi) fXAtom, (FloatHi) fYAtom, (FloatHi) fZAtom,
                    (FloatHi) fRadius,
                    GetValue(Cn3D_pupQualSphSlices) + QUAL_SIZE_MIN - 1,
                    GetValue(Cn3D_pupQualSphStacks) + QUAL_SIZE_MIN - 1,
                    lfAlpha, pmsdParent->pdnSlaveToMasterTransforms);

#else                           /* _OPENGL */
    Cn3d_AnyPrim = TRUE;        /* this probably can be deleted */
    /* yanli moved it out of OpenGL ifdef block */
    /* scale */
    iXAtom = (Int4) (fXAtom * VIEWSCALE);
    iYAtom = (Int4) (fYAtom * VIEWSCALE);
    iZAtom = (Int4) (fZAtom * VIEWSCALE);
    iRadius = (Int4) (fRadius * VIEWSCALE);
    if (paldAtom->pGraphic == NULL)
        paldAtom->pGraphic =
            (Pointer) AddSegment3D(pic, NULL, (BigScalar) paldAtom,
                                   Cn3d_LayerNow);
    AddSphere3D(pic, (Segment3D) paldAtom->pGraphic, (BigScalar) paldAtom,
                Cn3d_LayerNow,
                DDV_ColorIndex(Cn3D_ColorData.pDDVColorGlobal->Palette,
                               iColor), iXAtom, iYAtom, iZAtom, iRadius);
#endif                          /* else _OPENGL */

    return;
}



/*******************************************************/

static void RenderAllAtom(PFB pfbThis, Int4 iModel, Int4 iRenderWhat,
                          Pointer ptr)
{
    PRK prKeep;
    PVNMA pvnmaThis = NULL;
    PMGD pmgdThis = NULL;
    PMAD pmadThis = NULL;
    PALD paldThis = NULL;
    PVNMB pvnmbThis = NULL;
    PMBD pmbdThis = NULL;
    PMMD pmmdTo = NULL;
    PMMD pmmdThis;
    FloatLo flAtemp = (FloatLo) 0;
    Int4 iTemp = 0;
    Int2 iBin = 0;
    FloatLo fRadius = (FloatLo) 0;
    Boolean bDraw = TRUE;
    ValNodePtr pvnB;
    PALD paldMid = NULL;
    PALD paldDrawTo = NULL;
    PMAD pmadDrawTo = NULL;
    DDV_ColorCell iColor;
    int iMenu;
    FloatHi lfAlpha;

    prKeep = (PRK) ptr;
    if (!prKeep)
        return;
    if (!IsAtomNode(pfbThis))
        return;
    if (!(pfbThis->bMe & prKeep->NodeType))
        return;
    if (prKeep->NodeWhat != CONVERT_ALL && !(pfbThis->bWhat & prKeep->NodeWhat))
        return;
    pmadThis = (PMAD) pfbThis;

    /* render only appropriate parts of backbones */
    if (iRenderWhat == RESIDUES && IsAtomBackBone(pfbThis))
        return;
    if (iRenderWhat == PARTIALBB && !IsAtomPartialBackBone(pfbThis))
        return;
    if (iRenderWhat == COMPLETEBB && !IsAtomBackBone(pfbThis))
        return;
    if (iRenderWhat == VIRTUALBB && !(IsAtomCAlpha(pfbThis) || IsAtomPAlpha(pfbThis)))
        return;

    if (iRenderWhat == IONSON && !(pmadThis->bUpdate & AM_ION))
        return;
    if (iRenderWhat == HETSON && (pmadThis->bUpdate & AM_ION))
        return;

    paldThis = GetAtomAltLocs(pmadThis, iModel);
    if (!paldThis)
        return;                 /* no location corresponding to this model */


    /* make the atomic-level color decisions here */
    DDV_CopyColorCell(&iColor, &(prKeep->Color));
    pmgdThis = GetParentGraph(pfbThis);
    if(!pmgdThis->bHighlighted) {

        if (Cn3d_ColorNow == C_CPK) {
            DDV_SetColorInCell(&iColor,
                Cn3d_PaletteRGB[ElementKinColors[(Int1) pmadThis->pvnmaLink->choice]]);
            if (Cn3d_ColorPass) {
                DDV_RequestColor(&Cn3D_ColorData.pDDVColorGlobal->Palette, &iColor);
                return;
            }
        }

        else if (Cn3d_ColorNow == C_BYTEMP) {
            /* calculate temp factor */
            if (paldThis->iFloatNo == 4) { /* istotropic */
                iTemp = (Int4) paldThis->pflvData[4] * 100;
            }
            if (paldThis->iFloatNo == 9) { /* anisotropic *//* calculate the isotropic temp factor */
                flAtemp = (FloatLo) (((paldThis->pflvData[4] +
                    paldThis->pflvData[5] +
                    paldThis->pflvData[6]) / 3));
                iTemp = (Int4) flAtemp *100;
            }
#ifdef DEBUG_Q
            printf("temp=%d\n", (int) iTemp);
#endif
            if (iTemp < TempsKine[0])
                return;             /* too low */
            if (iTemp > TempsKine[15])
                return;             /* too high */
            /* find the bin */
            for (iBin = 1; iBin < KIN_COLOR_THERM; iBin++) {
                if ((iTemp < TempsKine[iBin + 1])
                    && (iTemp >= TempsKine[iBin])) break;
            }
            DDV_SetColorInCell(&iColor, Cn3d_PaletteRGB[ThermKine[iBin]]);
            if (Cn3d_ColorPass) {
                DDV_RequestColor(&Cn3D_ColorData.pDDVColorGlobal->Palette, &iColor);
                return;
            }
        }
    }
    /* otherwise the color should have been set by the calling routine */

    bDraw = TRUE;
    /* set up the atom drawing size */
    switch (prKeep->Atom) {
    case ATOM_SPACE:
        fRadius =
            (FloatLo) ((ElementSize((Int1) pmadThis->pvnmaLink->choice)));
        break;
    case ATOM_2XBOND:
        fRadius = prKeep->BondWidth * 2;
        break;
    case ATOM_ISBOND:
#ifdef _OPENGL
        fRadius = prKeep->BondWidth;
#else
        fRadius = prKeep->BondWidth * (float) 0.9;
#endif
        break;
    case ATOM_NONE:
    default:
        bDraw = FALSE;
    }
#ifdef DEBUG_N
/*printf("atom radius %f ", (float) fRadius);*/
#endif
    if ((Cn3d_DoHydrogens == FALSE) && (pmadThis->pvnmaLink->choice == 1))
        bDraw = FALSE;
    if (Cn3d_ColorPass) {
        DDV_RequestColor(&Cn3D_ColorData.pDDVColorGlobal->Palette, &iColor);
        return;
    }

    if (IsProtein(pfbThis))
        iMenu = 0;
    else if (IsNA(pfbThis))
        iMenu = 1;
    else
        iMenu = -1;

    pmmdThis = GetParentMol((PFB) paldThis);
    if (bDraw) {
        lfAlpha = 1.0;
#ifdef _OPENGL
        if (pmadThis->bUpdate & AM_ION) {
            PMSD pmsdThis = ToMSDParent(pfbThis);
            if(GetStatus(Cn3D_bIonLabel))
                RenderLabel(pmsdThis->pdnmsLink,
                ElementName(AtomicNumber(pmadThis)), paldThis, NULL, NULL,
                &iColor, LA_CENTER, prKeep->LScale, RL_CENTER);
            lfAlpha = 0.55;
        }
        else if (pmmdThis && ((pmmdThis->bWhat & AM_SOL) || (pmmdThis->bWhat & AM_WAT))) {
            /* use alpha proportional to occupancy (if present, clamped to 1.0) */
            if (paldThis->iFloatNo >= 3 && paldThis->pflvData[3] < 1.0)
                lfAlpha = paldThis->pflvData[3] * 0.6;
            else
                lfAlpha = 0.6;
        }
#endif
        RenderAnAtom(paldThis, &iColor, fRadius, lfAlpha);
    }

    /* the rest of this routine handles half-bond drawing where the bonds */
    /* are owned by the parent atom */
    bDraw = TRUE;
    if (prKeep->Bond) {
        pvnB = pmadThis->pvnBonds; /* local bond list */
        while (pvnB) {
            pmbdThis = (PMBD) pvnB->data.ptrvalue;
            if (pmbdThis->pmadTo == pmadThis)
                pmadDrawTo = pmbdThis->pmadFrom;
            else
                pmadDrawTo = pmbdThis->pmadTo;
            if (!pmadDrawTo)
                goto nextbond;
            /*  bond handler */
            bDraw = TRUE;

            /* don't draw non-virtual bonds */
            if ((int) iRenderWhat == RESIDUES && IsBondVirtual(pmbdThis))
                bDraw = FALSE;
            if ((int) iRenderWhat == VIRTUALBB && !IsBondVirtual(pmbdThis))
                bDraw = FALSE;
            /* don't draw to other non backbone atoms */
            if ((int) iRenderWhat == COMPLETEBB && 
                (!IsAtomBackBone(pmadDrawTo) || IsBondVirtual(pmbdThis)))
                bDraw = FALSE;
            /* don't draw to non-backbone or carbonyls */
            if ((int) iRenderWhat == PARTIALBB && 
                (!IsAtomPartialBackBone(pmadDrawTo) || IsBondVirtual(pmbdThis)))
                bDraw = FALSE;
            /* no bonds to "ions" */
            /*if ((int) iRenderWhat == CONNECTON) {
                if (pmadThis->bUpdate & AM_ION) bDraw = FALSE;
                if (pmadDrawTo->bUpdate & AM_ION) bDraw = FALSE;
            }*/
            if ((Cn3d_DoHydrogens == FALSE) && (pmadDrawTo->pvnmaLink->choice == 1))
                bDraw = FALSE;

            if (bDraw) {
#ifdef _OPENGL
                Boolean bCapTo;
                PMGD pmgdTo;
#endif
                paldDrawTo = GetAtomAltLocs(pmadDrawTo, iModel);
                if (!paldDrawTo)
                    goto nextbond;
                if (!paldDrawTo->pflvData)
                    goto nextbond;
                pmmdTo = GetParentMol((PFB) paldDrawTo);
                if ((pmmdTo != pmmdThis) && ((int) iRenderWhat != CONNECTON)) {
                    goto nextbond; /* don't connect inter-mol bonds */
                }
#ifdef _OPENGL
                /* try to avoid visibly open-ended cylinders/worms */
                bCapTo = FALSE;
                pmgdTo = GetParentGraph((PFB) pmadDrawTo);
                if (pmgdTo && !pmgdTo->bVisible)
                    bCapTo = TRUE;
#endif
                if (((int) iRenderWhat == RESIDUES) && (IsAtomBackBone(pmadDrawTo))) {
                    /* draw a whole bond to bridge */
#ifdef _OPENGL
                    if (!bCapTo && pmgdTo &&
                        ((IsGraphAminoAcid(pmgdTo) && GetStatus(Cn3D_pupPBB) != 3) ||
                         (IsGraphNABase(pmgdTo) && GetStatus(Cn3D_pupNABB) != 3)))
                        bCapTo = TRUE; /* cap if bond to undrawn backbone atoms */
                    RenderBond(paldThis, FALSE, paldDrawTo, bCapTo, 
                               &iColor, prKeep->BondWidth);
#else
                    RenderBond(paldThis, paldDrawTo, &iColor, prKeep->BondWidth);
#endif
                               
                } else {
                    if (prKeep->Bond == HALF_WORM_BOND) {
#ifdef _OPENGL
                        /* need cap at atom location if there exists only one 
                           virtual bond from this atom (to another defined atom) */
                        Boolean bCapFrom = FALSE;
                        ValNodePtr pvnTmp;
                        PMBD pmbdTmp;
                        for (pvnTmp = pmadThis->pvnBonds; pvnTmp; pvnTmp = pvnTmp->next) {
                            pmbdTmp = (PMBD) pvnTmp->data.ptrvalue;
                            if (IsBondVirtual(pmbdTmp) &&
                                pmbdTmp->pmadFrom && pmbdTmp->pmadFrom->pvnalLocate &&
                                pmbdTmp->pmadTo && pmbdTmp->pmadTo->pvnalLocate) {
                                if (!bCapFrom) {
                                    bCapFrom = TRUE;
                                } else {
                                    bCapFrom = FALSE;
                                    break;
                                }
                            }
                        }

                        RenderHalfWormBond(paldThis, bCapFrom, paldDrawTo, bCapTo, 
                                           &iColor, prKeep->BondWidth, iModel);
#else
                        RenderHalfWormBond(paldThis, paldDrawTo, &iColor,
                                           prKeep->BondWidth, iModel);
#endif
                    } else {
                        paldMid = NewALD(); /* phoney location */
                        if (!paldMid)
                            goto nextbond;
                        paldMid->pfbParent = (PFB) pmadThis; /* link to this  atom */
                        paldMid->iFloatNo = (Int1) 3;
                        paldMid->cAltConf = ' ';
                        paldMid->pflvData = FLVector(0, 3);
                        if (!paldMid->pflvData) {
                            FreeALD(paldMid);
                            return;
                        }
                        /* midpoint between the two atoms */
                        paldMid->pflvData[0] =
                            (paldThis->pflvData[0] +
                             paldDrawTo->pflvData[0]) / (float) 2.0;
                        paldMid->pflvData[1] =
                            (paldThis->pflvData[1] +
                             paldDrawTo->pflvData[1]) / (float) 2.0;
                        paldMid->pflvData[2] =
                            (paldThis->pflvData[2] +
                             paldDrawTo->pflvData[2]) / (float) 2.0;
                        if (paldMid && paldThis) {

#ifdef _OPENGL
                            /* the reason is a bit obscure, but this comparison
                               ensures that ends of OpenGL cylinders for bonds
                               match up correctly  (thiessen) */
                            if (paldThis > paldDrawTo)
                                RenderBond(paldThis, FALSE, paldMid, bCapTo,
                                           &iColor, prKeep->BondWidth);
                            else
                                RenderBond(paldMid, bCapTo, paldThis, FALSE,
                                           &iColor, prKeep->BondWidth);
#else
                            RenderBond(paldThis, paldMid, &iColor, prKeep->BondWidth);
#endif   /* _OPENGL */
                        }
                        FreeALD(paldMid);
                    }
                }
            }
          nextbond:
            pvnB = pvnB->next;
        }
    }
    return;
}


Int2 LIBCALL GetGraphNCBIstdaa(PMGD pmgdThis)
{

    Int2 i;

    if (!(IsGraphAminoAcid(pmgdThis)))
        return 21;
    for (i = 0; i < MAX_NCBIstdaa; i++)
        if (pmgdThis->pcIUPAC[0] == NCBIstdaaUC[i]) {
            return i;
        }
    return 21;                  /* Xxx */
}

Int2 LIBCALL GetGraphNCBI4na(PMGD pmgdThis)
{
    Int2 i;
    if (!(IsGraphNABase(pmgdThis)))
        return 16;
    for (i = 0; i < MAX_NCBIstdaa; i++)
        if (pmgdThis->pcIUPAC[0] == NCBI4naUC[i])
            return i;
    return 16;                  /* N (any) */
}


static void SetAtomAndBondGeometry(PRK prKeep, Int2 iRenderNow)
{
    switch (iRenderNow) {
    case R_NAME:
    case R_NUMBER:
    case R_PDBNUMBER:
        prKeep->Bond = (Byte) NO_BOND;
        prKeep->BondWidth = (FloatLo) 0;
        prKeep->Atom = (Byte) ATOM_NONE;
        break;
    case R_SPACE:
        prKeep->Bond = (Byte) NO_BOND;
        prKeep->BondWidth = (FloatLo) 0;
        prKeep->Atom = (Byte) ATOM_SPACE;
        break;
    case R_STICK:
        prKeep->Bond = (Byte) HALF_BOND;
        prKeep->BondWidth = HET_BOND_WIDTH;
        prKeep->Atom = (Byte) ATOM_ISBOND;
        break;
    case R_BALLNSTICK:
        prKeep->Bond = (Byte) HALF_BOND;
        prKeep->BondWidth = HET_BOND_WIDTH;
        prKeep->Atom = (Byte) ATOM_2XBOND;
        break;
    case R_THICKWIRE:
        prKeep->Bond = (Byte) HALF_BOND;
        prKeep->BondWidth = VIRT_BOND_WIDTH;
        prKeep->Atom = (Byte) ATOM_ISBOND;
        break;
#ifdef _OPENGL
    case R_WORMTHIN:
        prKeep->Bond = (Byte) HALF_WORM_BOND;
        prKeep->BondWidth = HET_BOND_WIDTH;
        prKeep->Atom = (Byte) ATOM_NONE;
        break;
    case R_WORMFAT:
        prKeep->Bond = (Byte) HALF_WORM_BOND;
        prKeep->BondWidth = VIRT_BOND_WIDTH;
        prKeep->Atom = (Byte) ATOM_NONE;
        break;
#endif
    case R_WORMWIRE:
        prKeep->Bond = (Byte) HALF_WORM_BOND;
        prKeep->BondWidth = (FloatLo) 0;
        prKeep->Atom = (Byte) ATOM_NONE;
        break;
    case R_DEFAULT:
    case R_WIRE:
    default:
        prKeep->Bond = (Byte) HALF_BOND;
        prKeep->BondWidth = (FloatLo) 0;
        prKeep->Atom = (Byte) ATOM_NONE;
    }
}

static void DoResidueLabel(PRK prkNew, Int4 iRenderWhat, PDNMG pdnmgThis,
                           PMSD pmsdThis, Int4 iModel)
{
    if ((iRenderWhat == NTBBLABELS) || (iRenderWhat == PBBLABELS)) {

        Int2 iEnd;
        Int2 iSkip;
        PMAD pmadThis = NULL;
        CharPtr pcLabel = NULL;
        CharPtr pcLNum = NULL;
        CharPtr pcDash = NULL;
        CharPtr pcTemp = NULL;
        CharPtr pcTemp2 = NULL;
        CharPtr pcL = NULL;
        PDNMG pdnmgFrom = NULL;
        PDNMG pdnmgTo = NULL;
        PMAD pmadFrom = NULL;
        PMAD pmadTo = NULL;
        PALD paldThis = NULL;
        PALD paldFrom = NULL;
        PALD paldTo = NULL;
        PMGD pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;

        switch (iRenderWhat) {
        case PBBLABELS:
            prkNew->LJust = ((PARS) pmgdThis->pvnPARSList->data.ptrvalue)->PBBLabelJust;
            prkNew->LStyle = ((PARS) pmgdThis->pvnPARSList->data.ptrvalue)->PBBLabelStyle;
            prkNew->LScale = ((PARS) pmgdThis->pvnPARSList->data.ptrvalue)->PBBLabelScale;
            break;
        case NTBBLABELS:
            prkNew->LJust = ((PARS) pmgdThis->pvnPARSList->data.ptrvalue)->NTBBLabelJust;
            prkNew->LStyle = ((PARS) pmgdThis->pvnPARSList->data.ptrvalue)->NTBBLabelStyle;
            prkNew->LScale = ((PARS) pmgdThis->pvnPARSList->data.ptrvalue)->NTBBLabelScale;
            break;
        }

        if (Cn3d_ColorNow == C_CPK || Cn3d_ColorNow == C_BYTEMP ||
            (prkNew->LStyle & L_WHITE))
            DDV_SetColorInCell((&prkNew->Color), Cn3d_PaletteRGB[C_white]);
        if (Cn3d_ColorPass) {
            DDV_RequestColor(&Cn3D_ColorData.pDDVColorGlobal->Palette, &(prkNew->Color));
            return;
        }

        switch ((int) Cn3d_LabelNow) {
        case 2:
            iSkip = 1;
            break;
        case 3:
            iSkip = 5;
            break;
        case 4:
            iSkip = 10;
            break;
        case 5:
            iSkip = 20;
            break;
        case 6:
            iSkip = 50;
            break;
        default:
            iSkip = 1;
        }
        if (((int) pdnmgThis->choice % (Int4) iSkip) != 0)
            return;

        if (IsGraphAminoAcid(pmgdThis) && (prkNew->LStyle & L_NAME)) {
            Int2 iResId;
            iResId = GetGraphNCBIstdaa(pmgdThis);
            if (prkNew->LStyle & L_PDB) {
                pcLabel = StringSave(pmgdThis->pcGraphName);
                if (!pcLabel)
                    pcLabel = StringSave("UNK");
                if (StringLen(pcLabel) > 3)
                    pcLabel[3] = '\0'; /* truncate SER COOH dict. */
            } else if (prkNew->LStyle & L_1LETR)
                pcLabel = StringSave(pmgdThis->pcIUPAC);
            else
                pcLabel = StringSave(AminoAcidNameFromIdx(iResId, USE_MIXCASE));
        }

        if (IsGraphNABase(pmgdThis) && (prkNew->LStyle & L_NAME)) {
            if ((prkNew->LStyle & L_1LETR) && (!(prkNew->LStyle & L_PDB))) {
                pcLabel = StringSave(pmgdThis->pcIUPAC);
            } else {
                pcLabel = StringSave(pmgdThis->pcGraphName);
            }
            if (!pcLabel)
                pcLabel = StringSave("UNK");
            pcTemp = pcLabel;
            if ((pmgdThis->bWhat & (Byte) DICT_LOCAL) &&
                ((pcLabel[0] == 'D') || (pcLabel[0] == 'R')) &&
                (pcLabel[1] == 'N') && (pcLabel[2] == 'A')) {
                pcTemp = (CharPtr) & pcLabel[3];
            }
            if (StringLen(pcTemp) > 3)
                pcTemp[3] = '\0'; /* truncate SER COOH dict. */
            pcTemp2 = pcLabel;
            pcLabel = StringSave(pcTemp);
            MemFree(pcTemp2);
        }

        if (prkNew->LStyle & L_NUM) {
            if (prkNew->LStyle & L_PDB) {
                pcLNum = StringSave(pmgdThis->pcGraphNum);
                if (!pcLNum)
                    pcLNum = StringSave("?");
                /* remove leading spaces */
                pcTemp = pcLNum;
                while ((pcTemp[0] == ' ') && (pcTemp[1] != '\0')) {
                    pcTemp = (CharPtr) & pcTemp[1];
                }
                pcTemp2 = pcLNum;
                pcLNum = StringSave(pcTemp);
                MemFree(pcTemp2);
            } else {
                pcLNum = (CharPtr) MemNew((size_t) (INTSTRLEN));
                if (!pcLNum)
                    return;
                sprintf(pcLNum, "%ld", (long) pdnmgThis->choice);
            }
        }

        if ((pcLabel == NULL) && (pcLNum == NULL)) {
            pcL = StringSave(" ");
        } else if (pcLabel == NULL) {
            pcL = StringSave(pcLNum);
            MemFree(pcLNum);
        } else if (pcLNum == NULL) {
            pcL = StringSave(pcLabel);
            MemFree(pcLabel);
        } else {
            pcDash = StringSave("-");
            if (pcLNum == NULL)
                pcLNum = StringSave(" ");
            pcL = (CharPtr)
                MemNew((size_t)
                       (StringLen(pcLabel) + StringLen(pcDash) +
                        StringLen(pcLNum) + 4));
            if (!pcL)
                return;
            StringCpy(pcL, pcLabel);
            StringCat(pcL, pcDash);
            StringCat(pcL, pcLNum);
            MemFree(pcLabel);
            MemFree(pcDash);
            MemFree(pcLNum);
        }
        /* pcL is label ready to go */

        pmadThis = GetMainAtom(pdnmgThis);
        paldThis = GetAtomAltLocs(pmadThis, iModel);
        if (!paldThis)
            return;
        iEnd = RL_BETACARB;

        pdnmgTo = GetPrevGraph(pdnmgThis);
        pmadTo = GetMainAtom(pdnmgTo);
        paldTo = GetAtomAltLocs(pmadTo, iModel);
        if (!paldTo)
            iEnd = RL_CENTERPLUSY;

        pdnmgFrom = GetNextGraph(pdnmgThis);
        pmadFrom = GetMainAtom(pdnmgFrom);
        paldFrom = GetAtomAltLocs(pmadFrom, iModel);
        if (!paldFrom)
            iEnd = RL_CENTERPLUSY;

        RenderLabel(pmsdThis->pdnmsLink, pcL,
                    paldThis, paldFrom, paldTo,
                    &(prkNew->Color), prkNew->LJust, prkNew->LScale, iEnd);
        MemFree(pcL);
    }
}


static Uint1* SelectColorForResidue(PDNMG pdnmgThis, Int4 currentRow,
                                    Int4 currentAlignLoc, PRK prkNew,
                                    PMMD pmmdThis, PARS pars, Int4 iRenderWhat)
{
    Int2 iResId;
    Uint1 *residueColor = NULL;
    PDNMM pdnmmThis;
    PMGD pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;

    switch (Cn3d_ColorNow) {
    case C_BYRES:
        if IsGraphAminoAcid(pmgdThis) {
            iResId = GetGraphNCBIstdaa(pmgdThis);
            residueColor = Cn3d_PaletteRGB[KinAAColor[iResId]];
        } else if IsGraphNABase(pmgdThis) {
            iResId = GetGraphNCBI4na(pmgdThis);
            residueColor = Cn3d_PaletteRGB[KinNAColor[iResId]];
        }
        break;
        
    case C_BYSECSTRUC:
        if (IsGraphAminoAcid(pmgdThis)) {
            DDV_ColorCell *pMyCell = NULL;
            pMyCell = ColorFromSS(pmgdThis);
            if (pMyCell == NULL) return NULL;
            residueColor = pMyCell->rgb;
        }
        break;

    case C_BYOBJECT:
        residueColor = Cn3d_PaletteRGB[bObjectColors[(Cn3d_lSlaveNum % NUM_SLAVES)]];
        break;

    case C_BYSTRUCALIGN:
        if (IsGraphAminoAcid(pmgdThis)) {
            if (currentAlignLoc >= 0) {
                residueColor = Cn3d_PaletteRGB[C_red]; /* change conserved color to red */
            } else {
                /* use C_BYOBJECT colors for unaligned */
                residueColor = Cn3d_PaletteRGB[bObjectColors[(Cn3d_lSlaveNum % NUM_SLAVES)]];
            }
        }
        break;

    case C_BYSEQCONS:
        if (IsGraphAminoAcid(pmgdThis)) {
            if (currentAlignLoc >= 0) {
                static Uint1 alignedColor[3];
                Nlm_Int4 RGBint =
                    CSC_GetColumnColorByRow(currentRow, pdnmgThis->choice - 1);
                alignedColor[0] = (RGBint >> 16) & 0xFF;
                alignedColor[1] = (RGBint >> 8) & 0xFF;
                alignedColor[2] = RGBint & 0xFF;
                residueColor = alignedColor;
            } else {
                residueColor = Cn3D_UnalignedResidueColor;
            }
        }
        break;

    case C_BYDOMAIN:
        if (IsGraphAminoAcid(pmgdThis)) {
            residueColor = Cn3d_PaletteRGB[ColorNumKinBB[(pmgdThis->iDomain % KIN_COLOR_NUM)]];
        }
        break;
        
    case C_BYHYDRO:
        if (IsGraphAminoAcid(pmgdThis)) {
            iResId = GetGraphNCBIstdaa(pmgdThis);
            residueColor = Cn3d_PaletteRGB[PhobeAAColor[iResId]];
        } else if (IsGraphNABase(pmgdThis)) {
            iResId = GetGraphNCBI4na(pmgdThis);
            residueColor = Cn3d_PaletteRGB[KinNAColor[iResId]];
        }
        break;
        
    case C_BYCHAIN:
        pdnmmThis = DNFromPFB((PFB) pmmdThis);
        residueColor =
            Cn3d_PaletteRGB[ColorNumKinBB[(pdnmmThis->choice % KIN_COLOR_NUM)]];
        break;

    case C_BYCHOICE:
        if (prkNew->NodeWhat && AM_PROT) {
            if (iRenderWhat == RESIDUES) {
                residueColor = pars->PResColRGB;
            } else { /* backbone */
                residueColor = pars->PBBColRGB;
            }
        } else { /* nucleotide */
            if (iRenderWhat == RESIDUES) {
                residueColor = pars->NTResColRGB;
            } else { /* backbone */
                residueColor = pars->NTBBColRGB;
            }
        }
        break;

    case C_CPK:
    case C_BYTEMP: /* atom-level colors will be set later in RenderAllAtom */
    default:
        residueColor = NULL;
    }
    return residueColor;
}


/* Sets color scheme for rendering a graph */
static void LIBCALLBACK RenderGraph(PFB pfbThis, Int4 iModel, Int4 iRenderWhat,
                                    Pointer ptr)
{                               /* traverser callback */
    PRK prKeep;
    PRK prkNew = NULL;
    PDNMG pdnmgThis = NULL;
    PMSD pmsdThis = NULL;
    PMMD pmmdThis = NULL;
    PMGD pmgdThis = NULL;
    PVNMA pvnmaThis = NULL;
    PMAD pmadThis = NULL;
    PVNMB pvnmbThis = NULL;
    PMBD pmbdThis = NULL;
    Byte bReservedThis = 0;
    static RK rkNew;
    Boolean addedResidueColors = FALSE;
    PARS parsResidue = NULL;

    prkNew = &rkNew;
    prKeep = (PRK) ptr;
    if (!prKeep) return;
    Cn3D_DupRenderKeep(prkNew, prKeep);

    if (!IsGraphNode(pfbThis))
        return;
    if (!(pfbThis->bMe & prKeep->NodeType))
        return;
    /*if (!(pfbThis->bWhat & prKeep->NodeWhat)) return;*/
    pmgdThis = (PMGD) pfbThis;

    /* set up rendering styles for residue-wise structure elements */
    if (iRenderWhat == ANYBACKBONE || iRenderWhat == RESIDUES ||
        iRenderWhat == PBBLABELS || iRenderWhat == NTBBLABELS) {

        /* use PARS structure at head of list to set residue-wise rendering styles */
        parsResidue = (PARS) pmgdThis->pvnPARSList->data.ptrvalue;

        if (prKeep->NodeWhat & AM_PROT) {
            if (iRenderWhat == ANYBACKBONE) {
                if (parsResidue->PVirtualBBOn || parsResidue->PPartialBBOn ||
                    parsResidue->PCompleteBBOn) {
                    Cn3d_RenderNow = parsResidue->PBBRender;
                    Cn3d_ColorNow = parsResidue->PBBColor;
                    if (parsResidue->PVirtualBBOn) iRenderWhat = VIRTUALBB;
                    else if (parsResidue->PPartialBBOn) iRenderWhat = PARTIALBB;
                    else iRenderWhat = COMPLETEBB;
                } else return;
            }
            else if (iRenderWhat == RESIDUES) {
                if (parsResidue->PResiduesOn) {
                    Cn3d_RenderNow = parsResidue->PResRender;
                    Cn3d_ColorNow = parsResidue->PResColor;
                } else return;
            }
            else if (iRenderWhat == PBBLABELS) {
                if (parsResidue->PBBLabelInterval > 1) {
                    Cn3d_RenderNow = R_NAME;
                    Cn3d_ColorNow = parsResidue->PBBColor;
                    Cn3d_LabelNow = parsResidue->PBBLabelInterval;
                } else return;
            }
            else return;
        }

        else if (prKeep->NodeWhat & (AM_DNA | AM_RNA)) {
            if (iRenderWhat == ANYBACKBONE) {
                if (parsResidue->NTVirtualBBOn || parsResidue->NTPartialBBOn ||
                    parsResidue->NTCompleteBBOn) {
                    Cn3d_RenderNow = parsResidue->NTBBRender;
                    Cn3d_ColorNow = parsResidue->NTBBColor;
                    if (parsResidue->NTVirtualBBOn) iRenderWhat = VIRTUALBB;
                    else if (parsResidue->NTPartialBBOn) iRenderWhat = PARTIALBB;
                    else iRenderWhat = COMPLETEBB;
                } else return;
            }
            else if (iRenderWhat == RESIDUES) {
                if (parsResidue->NTResiduesOn) {
                    Cn3d_RenderNow = parsResidue->NTResRender;
                    Cn3d_ColorNow = parsResidue->NTResColor;
                } else return;
            }
            else if (iRenderWhat == NTBBLABELS) {
                if (parsResidue->NTBBLabelInterval > 1) {
                    Cn3d_RenderNow = R_NAME;
                    Cn3d_ColorNow = parsResidue->NTBBColor;
                    Cn3d_LabelNow = parsResidue->NTBBLabelInterval;
                } else return;
            }
            else return;
        }
        else return;
    }
    prkNew->NodeWhat = (Byte) CONVERT_ALL;
    SetAtomAndBondGeometry(prkNew, Cn3d_RenderNow);
 
    pdnmgThis = DNFromPFB(pfbThis);
    ASSERT(pdnmgThis != NULL);
    pmsdThis = ToMSDParent(pfbThis);
    ASSERT(pmsdThis != NULL);
    pmmdThis = GetParentMol((PFB) pmgdThis);
    ASSERT(pmmdThis != NULL);

    if (IsGraphAminoAcid(pmgdThis) || IsGraphNABase(pmgdThis)) {

        Boolean lowercase = FALSE;
        Int4 currentRow, currentAlignLoc = -2;
        SeqIdPtr currentSip;
        Uint1 *residueColor = NULL;
        SeqAlignPtr salp = NULL;
        SeqAnnotPtr sanp;

        /* get pointer to current SeqAlign */
        for (sanp = Cn3D_ColorData.sap; sanp != NULL; sanp = sanp->next) {
            if (sanp->data != NULL) {
                salp = sanp->data;
                break;
            }
        }

        /* see if we're doing by-row or by-SeqId storage */
        currentRow = -1;
        currentSip = pmmdThis->pSeqId;
        if (salp) {
            currentRow = ViewMgr_TRow2VRow(salp, pmmdThis->iTargetRow);
            /* if this sequence is part of the alignment, but is hidden, do nothing */
            if (currentRow <= 0 && pmmdThis->iTargetRow > 0)
                return;
            currentSip = NULL;
        }

        /* find out whether this residue is aligned by querying AlignMgr */
        if (salp && currentRow > 0) {
            currentAlignLoc = AlnMgrMapBioseqToSeqAlign(salp, 
                                                        pdnmgThis->choice - 1,
                                                        currentRow,
                                                        NULL);
            lowercase = (currentAlignLoc < 0);
        }

        /* figure out what residue-wise color to use */
        residueColor = SelectColorForResidue(pdnmgThis, currentRow,
                                             currentAlignLoc, prkNew,
                                             pmmdThis, parsResidue, iRenderWhat);
        if (!residueColor)
            residueColor = Cn3d_PaletteRGB[C_white];

        /* set color/case in sequence window if this is an aligned (or only) structure. */
        /* always color in DDV cell with backbone color. */    
        if ((iRenderWhat == COMPLETEBB || iRenderWhat == PARTIALBB || iRenderWhat == VIRTUALBB) &&
            ((currentSip && currentRow <= 0) || (!currentSip && currentRow > 0))) {

            DDV_ColorCell *pColorTo =
                DDV_GetColor(Cn3D_ColorData.pDDVColorGlobal,
                                currentSip, currentRow, pdnmgThis->choice - 1);
            if (pColorTo == NULL) return;

            /* can't display white or nearly-white letters in sequence window */
            if (residueColor[0] > 230 && residueColor[1] > 230 && residueColor[2] > 230)
                DDV_SetColorInCell(pColorTo, Cn3d_PaletteRGB[C_gray]);
            else
                DDV_SetColorInCell(pColorTo, residueColor);

            if (Cn3d_ColorPass) {
                DDV_RequestColor(&Cn3D_ColorData.pDDVColorGlobal->Palette, pColorTo);
                addedResidueColors = TRUE;
            } else {    
                pColorTo->LowerCase = lowercase;
                DDV_SetColor(Cn3D_ColorData.pDDVColorGlobal, currentSip,
                            currentRow, pdnmgThis->choice - 1, pColorTo);
            }
        }
            
        /* check to see if we want to show aligned/unaligned regions in 
        structure window (ignore if single structure) */
        if (salp && IsGraphAminoAcid(pmgdThis) &&
            ((currentAlignLoc >= 0 && !Cn3D_fAlignOn) ||
            (currentAlignLoc < 0 && !Cn3D_fUnalignOn))) {
            return;
        }

        DDV_SetColorInCell(&(prkNew->Color), residueColor);
    }

    /* special case so that non-residues can be colored by object */
    else if (Cn3d_ColorNow == C_BYOBJECT) {
        DDV_SetColorInCell(&(prkNew->Color),
            Cn3d_PaletteRGB[bObjectColors[(Cn3d_lSlaveNum % NUM_SLAVES)]]);
    }

    /* ... then set the highlight color for whole residue if highlighted */
    if (pmgdThis->bHighlighted
/*       let any 'ole molecule be highlighted...
         && (IsGraphAminoAcid(pmgdThis) || IsGraphNABase(pmgdThis) ||
         IsHeterogen(pmmdThis) || IsIon(pmmdThis) || IsSolvent(pmmdThis))
*/
        ) {

        DDV_ColorEntry *pColorEntry = DDV_SearchColorbyName(
            Cn3D_ColorData.pDDVColorGlobal->pvnSpecialColors, "Highlight");
        if (pColorEntry != NULL) {
            DDV_CopyColorCell(&(prkNew->Color), &(pColorEntry->ColorCell));
            if (Cn3d_ColorPass) {
                DDV_RequestColor(&Cn3D_ColorData.pDDVColorGlobal->Palette,
                                    &(prkNew->Color));
                addedResidueColors = TRUE;
            }
        }
    } 

    if (addedResidueColors) return;
    
    /* control display show/off on MG level-- Yanli */
    if (pmgdThis->bVisible != 1)
        return;

    DoResidueLabel(prkNew, iRenderWhat, pdnmgThis, pmsdThis, iModel);

#ifdef _OPENGL
    OGL_LoadName((Nlm_VoidPtr) pmgdThis);
#endif

    /* do the rendering */

    /* deal with setting the NodeType/NodeWhat for virtual/backbone/realbb/residues */
    if ((prkNew->Bond == HALF_BOND) || (prkNew->Bond == HALF_WORM_BOND) ||
        (prkNew->Atom != ATOM_NONE)) { /* draw atoms or half-bonds */
        prkNew->NodeType = (Byte) AM_MAD;
        switch ((int) iRenderWhat) {
        case COMPLETEBB:
            prkNew->NodeWhat = (Byte) AM_BACKBONE;
            break;
        case PARTIALBB:
            prkNew->NodeWhat = (Byte) (AM_BACKBONE | AM_PARTIALBACKBONE);
            break;
        case VIRTUALBB:
            prkNew->NodeWhat = (Byte) (AM_CALPHA | AM_PALPHA); /* virtual */
            break;
        default:
            prkNew->NodeWhat = (Byte) CONVERT_ALL; /* do all the atoms */
        }

        /* render all the atoms in this residue */
        pvnmaThis = pmgdThis->pvnmaAHead;
        while (pvnmaThis) {
            pmadThis = (PMAD) pvnmaThis->data.ptrvalue;
            if (pmadThis) RenderAllAtom((PFB) pmadThis, iModel, iRenderWhat, prkNew);
            pvnmaThis = pvnmaThis->next;
        }
    }

    /* Traverse the InterResidueBonds & draw them as half bonds,
       connecting to this residue in its residue color */
    if (!(((int) iRenderWhat == VIRTUALBB) ||
          ((int) iRenderWhat == PARTIALBB) ||
          ((int) iRenderWhat == COMPLETEBB))
        && (prkNew->Bond)) {    
        prkNew->NodeType = (Byte) AM_MAD;
        prkNew->NodeWhat = (Byte) CONVERT_ALL;
        pvnmbThis = pmmdThis->pvnmbIRBHead;
        while (pvnmbThis) {     /* walk the inter-res bond list by hand */
            pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
            if (GetParentGraph((PFB) pmbdThis->pmadFrom) == pmgdThis) { /* this inter-res bond is in this graph */
                RenderAllAtom((PFB) pmbdThis->pmadFrom, iModel, iRenderWhat, prkNew);
            }
            if (GetParentGraph((PFB) pmbdThis->pmadTo) == pmgdThis) {
                RenderAllAtom((PFB) pmbdThis->pmadTo, iModel, iRenderWhat, prkNew);
            }
            pvnmbThis = pvnmbThis->next;
        }
    }

    return;
}


static void LabelChainTermini(Int4 iRenderWhat, Int4 iModel, 
                              PMMD pmmdThis, PRK prkNew)
{
    if (iRenderWhat == NTTERMLABELS || iRenderWhat == PTERMLABELS) {

        PDNMG pdnmgFrom;
        PMAD pmadFrom;
        PALD paldFrom;
        PDNMG pdnmgTo;
        PMAD pmadTo;
        PALD paldTo;
        PVNMB pvnmbThis = NULL;
        PMBD pmbdThis = NULL;
        CharPtr pcN, pcC;
        CharPtr pcChain = NULL;
        Char pcTemp[32];

        PMSD pmsdThis = ToMSDParent((PFB) pmmdThis);
        ASSERT(pmsdThis != NULL);

        /* always do white termini labels - too hard to track with other colors,
           since we don't have residue-wise info at this point */
        DDV_SetColorInCell((&prkNew->Color), Cn3d_PaletteRGB[C_white]);
        if (Cn3d_ColorPass) {
            DDV_RequestColor(&Cn3D_ColorData.pDDVColorGlobal->Palette, &(prkNew->Color));
            return;
        }

        switch (iRenderWhat) {
        case PTERMLABELS:
            prkNew->LJust = ((PARS) pmsdThis->pGlobalPARS)->PTermLabelJust;
            prkNew->LStyle = ((PARS) pmsdThis->pGlobalPARS)->PTermLabelStyle;
            prkNew->LScale = ((PARS) pmsdThis->pGlobalPARS)->PTermLabelScale;
            break;
        case NTTERMLABELS:
            prkNew->LJust = ((PARS) pmsdThis->pGlobalPARS)->NTTermLabelJust;
            prkNew->LStyle = ((PARS) pmsdThis->pGlobalPARS)->NTTermLabelStyle;
            prkNew->LScale = ((PARS) pmsdThis->pGlobalPARS)->NTTermLabelScale;
            break;
        }

        if ((iRenderWhat == PTERMLABELS)) {
            pcN = "N";
            pcC = "C";
        }
        else if (iRenderWhat == NTTERMLABELS) {
            pcN = "5'";
            pcC = "3'";
        }
        if (prkNew->LStyle & L_NAME)
            pcChain = pmmdThis->pcMolName;
        if (!pcChain)
            pcChain = " ";
        StringCpy(pcTemp, pcChain);
        StringCat(pcTemp, " ");
        StringCat(pcTemp, pcN);
        pdnmgTo = GetFirstGraph(pmmdThis);
        if (!pdnmgTo || !pdnmgTo->data.ptrvalue ||
            !((PMGD) pdnmgTo->data.ptrvalue)->bVisible)
            return;
        pmadTo = GetMainAtom(pdnmgTo);
        paldTo = GetAtomAltLocs(pmadTo, iModel);
        while (paldTo == NULL) { /* walk in */
            pdnmgTo = GetNextGraph(pdnmgTo);
            if (!pdnmgTo)
                return; /* bail if no next graph */
            pmadTo = GetMainAtom(pdnmgTo);
            paldTo = GetAtomAltLocs(pmadTo, iModel);
        }
        pdnmgFrom = GetNextGraph(pdnmgTo);
        if (!pdnmgFrom)
            return;
        pmadFrom = GetMainAtom(pdnmgFrom);
        paldFrom = GetAtomAltLocs(pmadFrom, iModel);
        RenderLabel(pmsdThis->pdnmsLink, pcTemp,
                    NULL, paldFrom, paldTo,
                    &(prkNew->Color), prkNew->LJust, prkNew->LScale,
                    RL_EXTRAPOL);
        StringCpy(pcTemp, pcChain);
        StringCat(pcTemp, " ");
        StringCat(pcTemp, pcC);
        pdnmgTo = GetLastGraph(pmmdThis);
        if (!pdnmgTo || !pdnmgTo->data.ptrvalue ||
            !((PMGD) pdnmgTo->data.ptrvalue)->bVisible)
            return;
        pmadTo = GetMainAtom(pdnmgTo);
        paldTo = GetAtomAltLocs(pmadTo, iModel);
        while (paldTo == NULL) {
            pdnmgTo = GetPrevGraph(pdnmgTo);
            if (!pdnmgTo)
                return;
            pmadTo = GetMainAtom(pdnmgTo);
            paldTo = GetAtomAltLocs(pmadTo, iModel);
        }
        pdnmgFrom = GetPrevGraph(pdnmgTo);
        if (!pdnmgFrom)
            return;
        pmadFrom = GetMainAtom(pdnmgFrom);
        paldFrom = GetAtomAltLocs(pmadFrom, iModel);
        RenderLabel(pmsdThis->pdnmsLink, pcTemp,
                    NULL, paldFrom, paldTo,
                    &(prkNew->Color), prkNew->LJust, prkNew->LScale,
                    RL_EXTRAPOL);
    }
}

static void LIBCALLBACK RenderMolecule(PFB pfbThis, Int4 iModel,
                                       Int4 iRenderWhat, Pointer ptr)
{                               /* traverser callback */
    PRK prKeep;
    PRK prkNew = NULL;
    PDNMM pdnmmThis = NULL;
    PMMD pmmdThis = NULL;
    Byte bHold;
    static RK rkNew;

    prkNew = &rkNew;

    prKeep = (PRK) ptr;
    if (!prKeep) return;
    Cn3D_DupRenderKeep(prkNew, prKeep);

    if (!IsMoleculeNode(pfbThis))
        return;
    if (!(pfbThis->bMe & prKeep->NodeType))
        return;          /* same test for molecule */

    bHold = prKeep->NodeWhat;

    /* special case for anded DNA/RNA */
    if ((prKeep->NodeWhat == (Byte) (AM_DNA | AM_RNA)) &&
        ((pfbThis->bWhat & AM_DNA) || (pfbThis->bWhat & AM_RNA)))
        bHold = pfbThis->bWhat;

    /* special case for anded WAT/SOL */
    else if ((prKeep->NodeWhat == (Byte) (AM_WAT | AM_SOL)) &&
             ((pfbThis->bWhat & AM_WAT) || (pfbThis->bWhat & AM_SOL)))
        bHold = pfbThis->bWhat;

    /* special case for HET/ION some ions are also hets - e.g. heme(Fe) */
    else if ((prKeep->NodeWhat == (Byte) (AM_HET)) &&
             ((pfbThis->bWhat == (Byte) AM_ION) ||
              (pfbThis->bWhat == (Byte) AM_HET) ||
              (pfbThis->bWhat == (Byte) AM_POLY)))
        bHold = pfbThis->bWhat;

    if (pfbThis->bWhat != bHold)
        return;

    if ((iRenderWhat == HETSON) || (iRenderWhat == IONSON))
        if (!((pfbThis->bWhat == AM_HET) ||
              (pfbThis->bWhat == AM_POLY) || (pfbThis->bWhat == AM_ION)))
            return;

    pdnmmThis = DNFromPFB(pfbThis);
    pmmdThis = (PMMD) pfbThis;

    if (!Cn3D_IsVisible(Cn3D_ColorData.pDDVColorGlobal, pmmdThis))
        return;

    LabelChainTermini(iRenderWhat, iModel, pmmdThis, prkNew);

    prkNew->NodeType = (Byte) AM_MGD;
    /*prkNew->NodeWhat = (Byte) CONVERT_ALL;*/
    TraverseOneModel(pmmdThis->pdnmgHead, TRAVERSE_GRAPH, iModel,
                             iRenderWhat, prkNew, RenderGraph);
    return;
}


static void LIBCALLBACK DoGraphicNull(PFB pfbThis, Int4 iModel,
                                      Int4 iIndex, Pointer ptr)
{
    PMAD pmadAtom;
    PALD paldLoc;

    if (IsAtomNode(pfbThis)) {
        pmadAtom = (PMAD) pfbThis;
        paldLoc = GetAtomAltLocs(pmadAtom, iModel);
        while (paldLoc) {
            paldLoc->pGraphic = NULL;
            paldLoc = paldLoc->next; /* get next location */
        }
    }
}



Cn3D_ColorFuncList Cn3D_ColorList[CN3DFUNCNUM] = {
{C_CPK,         "CPK",                              Cn3D_Color_CPK},
{C_BYCHAIN,     "Cycle Chain",                      Cn3D_Color_BYCHAIN},
{C_BYTEMP,      "Temperature",                      Cn3D_Color_BYTEMP},
{C_BYRES,       "Residue",                          Cn3D_Color_BYRES},
{C_BYSECSTRUC,  "Secondary Structure",              Cn3D_Color_BYSECSTRUC},
{C_BYHYDRO,     "Hydrophobicity",                   Cn3D_Color_BYHYDRO},
{C_BYOBJECT,    "Cycle Object                    ", Cn3D_Color_BYOBJECT}, /* make long so menus sizes in Render Panel match */
{C_BYDOMAIN,    "Cycle Domain",                     Cn3D_Color_BYDOMAIN},
{C_BYSTRUCALIGN,"Alignment",                        Cn3D_Color_BYSTRUCCONS},
{C_BYSEQCONS,   "Sequence Conservation",            Cn3D_Color_BYSEQCONS}
};


Char * Cn3D_ColorFuncName(Int4 which)
{
    int i;
    for(i = 0; i < CN3DFUNCNUM; i++)
        if(Cn3D_ColorList[i].Value == which) break;
    if (i < CN3DFUNCNUM)
        return Cn3D_ColorList[i].Name;
    Message(MSG_POST, "Bad color function number passed to Cn3D_ColorFuncFindName");
    return NULL;
}

Char * Cn3D_ColorFuncFind(void)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    pdnmsThis = GetSelectedModelstruc(); /* fetch the active structure */
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return NULL;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (pars == NULL) return NULL;
    return Cn3D_ColorFuncName(pars->PBBColor);
}

static void Cn3D_ColorStandard(Int2 Setting)
{
    PARS pars = NULL;
    PDNMS pdnmsThis = NULL;

    pdnmsThis = GetSelectedModelstruc(); /* fetch the active structure */
    if (!pdnmsThis || !pdnmsThis->data.ptrvalue) return;
    pars = (PARS) ((PMSD) pdnmsThis->data.ptrvalue)->pGlobalPARS;
    if (pars == NULL) return;

    pars->PBBColor = Setting;
}

void Cn3D_Color_CPK(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange)
{
    Cn3D_ColorStandard(C_CPK);
}

void Cn3D_Color_BYCHAIN(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange)
{
    Cn3D_ColorStandard(C_BYCHAIN);
}

void Cn3D_Color_BYTEMP(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange)
{
    Cn3D_ColorStandard(C_BYTEMP);
}

void Cn3D_Color_BYRES(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange)
{
    Cn3D_ColorStandard(C_BYRES);
}

void Cn3D_Color_BYSECSTRUC(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange)
{
    Cn3D_ColorStandard(C_BYSECSTRUC);
}

void Cn3D_Color_BYHYDRO(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange)
{
    Cn3D_ColorStandard(C_BYHYDRO);
}

void Cn3D_Color_BYOBJECT(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange)
{
    Cn3D_ColorStandard(C_BYOBJECT);
}

void Cn3D_Color_BYDOMAIN(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange)
{
    Cn3D_ColorStandard(C_BYDOMAIN);
}

void Cn3D_Color_BYSTRUCCONS(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange)
{
    Cn3D_ColorStandard(C_BYSTRUCALIGN);
}

void Cn3D_Color_BYSEQCONS(DDV_ColorGlobal * pColorGlobal, void *pData,
                          DDV_Range * pRange)
{
    Cn3D_ColorStandard(C_BYSEQCONS);
}

void Cn3D_ColorSpecial(DDV_ColorGlobal * pColorGlobal, void *pData,
                       DDV_Range * pRange)
{
    ;
}


static void RenderConnection(PRK prKeep, PMSD pmsdThis, Int2 iModel)
{
    PVNMB pvnmbThis;

    DDV_SetColorInCell(&(prKeep->Color), Cn3d_PaletteRGB[C_yellow]);
    if (Cn3d_ColorPass) {
        DDV_RequestColor(&Cn3D_ColorData.pDDVColorGlobal->Palette, &(prKeep->Color));
        return;
    }

#ifdef _OPENGL
    if (Cn3d_RenderNow == R_WORMWIRE || Cn3d_RenderNow == R_WORMTHIN ||
        Cn3d_RenderNow == R_WORMFAT)
        prKeep->Bond = (Byte) HALF_WORM_BOND;
    else
#endif
        prKeep->Bond = (Byte) HALF_BOND;
    prKeep->Atom = (Byte) ATOM_NONE;
    switch (Cn3d_RenderNow) {
    case R_STICK:
#ifdef _OPENGL
    case R_WORMTHIN:
#endif
        prKeep->BondWidth = HET_BOND_WIDTH;
        break;
    case R_BALLNSTICK:
    case R_THICKWIRE:
#ifdef _OPENGL
    case R_WORMFAT:
#endif
        prKeep->BondWidth = VIRT_BOND_WIDTH;
        break;
    case R_DEFAULT:
    case R_WIRE:
    case R_WORMWIRE:
    default:
        prKeep->BondWidth = (FloatLo) 0;
    }

#ifdef _OPENGL
    OGL_LoadName(NULL); /* disable highlighting of connections */
#endif

    pvnmbThis = pmsdThis->pvnmbIMBHead;
    while (pvnmbThis) {     /* walk the inter-mol bond list by hand */
        PMBD pmbdThis;
        PALD paldThis, paldDrawTo;

        pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
        paldThis = GetAtomAltLocs(pmbdThis->pmadFrom, iModel);
        paldDrawTo = GetAtomAltLocs(pmbdThis->pmadTo, iModel);

        if (paldThis && paldDrawTo) {
            int i;
            PALD pald;
            PMAD pmad;

            for (i = 0; i < 2; i++) { /* check atoms at both ends of bond */
                PMMD pmmd = NULL;
                PMGD pmgd;
                if (i == 0) {
                    pmad = pmbdThis->pmadFrom;
                    pald = paldThis;
                } else {
                    pmad = pmbdThis->pmadTo;
                    pald = paldDrawTo;
                }

                /* hide connections to undrawn atoms: if this is a connection to solvent, and solvent
                   isn't on, don't draw. Ditto for ions, hets, sidechains, backbones... */
                if (!((PARS) pmsdThis->pGlobalPARS)->IonsOn && (pmad->bUpdate & AM_ION)) {
                    goto skip_bond;
                }
                if (!((PARS) pmsdThis->pGlobalPARS)->HeterogensOn) {
                    if (!pmmd) pmmd = GetParentMol((PFB) pald);
                    if (pmmd && (pmmd->bWhat == AM_HET || pmmd->bWhat == AM_POLY))
                        goto skip_bond;
                }
                if (!((PARS) pmsdThis->pGlobalPARS)->SolventOn) {
                    if (!pmmd) pmmd = GetParentMol((PFB) pald);
                    if (pmmd && ((pmmd->bWhat & AM_SOL) || (pmmd->bWhat & AM_WAT)))
                        goto skip_bond;
                }
                pmgd = GetParentGraph((PFB) pmad);
                if (!pmgd) break;
                if (!((PARS) pmgd->pvnPARSList->data.ptrvalue)->PResiduesOn &&
                    IsGraphAminoAcid(pmgd) && !IsAtomBackBone(pmad)) {
                    goto skip_bond;
                }
                if (!((PARS) pmgd->pvnPARSList->data.ptrvalue)->NTResiduesOn &&
                    IsGraphNABase(pmgd) && !IsAtomBackBone(pmad)) {
                    goto skip_bond;
                }
                if (IsGraphAminoAcid(pmgd) && IsAtomBackBone(pmad)) {
                    if (!((PARS) pmgd->pvnPARSList->data.ptrvalue)->PCompleteBBOn &&
                        !((PARS) pmgd->pvnPARSList->data.ptrvalue)->PPartialBBOn)
                        goto skip_bond;
                    if (!IsAtomPartialBackBone(pmad) &&
                        ((PARS) pmgd->pvnPARSList->data.ptrvalue)->PPartialBBOn)
                        goto skip_bond;
                }
                if (IsGraphNABase(pmgd) && IsAtomBackBone(pmad)) {
                    if (!((PARS) pmgd->pvnPARSList->data.ptrvalue)->NTCompleteBBOn &&
                        !((PARS) pmgd->pvnPARSList->data.ptrvalue)->NTPartialBBOn)
                        goto skip_bond;
                    if (!IsAtomPartialBackBone(pmad) &&
                        ((PARS) pmgd->pvnPARSList->data.ptrvalue)->NTPartialBBOn)
                        goto skip_bond;
                }
            }

#ifdef _OPENGL
            RenderBond(paldThis, TRUE, paldDrawTo, TRUE,
                        &(prKeep->Color), prKeep->BondWidth);
#else
            RenderBond(paldThis, paldDrawTo,
                        &(prKeep->Color), prKeep->BondWidth);
#endif
        }
    skip_bond:
        pvnmbThis = pvnmbThis->next;
    }
    return;
}

static void RenderParts(PDNMS pdnmsThis, Int2 iModel,
                         Byte bMolecule, Int2 iRenderWhat)
{
    PRK prKeep;
    PMSD pmsdThis;
    static RK rKeep;

    prKeep = &rKeep;

    if (!pdnmsThis ||
        !(pmsdThis = (PMSD) pdnmsThis->data.ptrvalue))
        return;

    Cn3D_SetRenderKeep(prKeep);

    if (iRenderWhat == CONNECTON) {
        RenderConnection(prKeep, pmsdThis, iModel);
        return;
    }
    
    prKeep->NodeType = (Byte) AM_MMD;
    prKeep->NodeWhat = (Byte) bMolecule;
    TraverseOneModel(pdnmsThis, TRAVERSE_MOLECULE,
                     iModel, (Int4) iRenderWhat, prKeep,
                     RenderMolecule);
}


static void fnARModelLoop(PDNMS pdnmsThis, PDNML pdnmlThis)
{
    PARS parsGlobal;
    PMSD pmsdThis = NULL;
    Int2 iModel = 0;

    if (!pdnmsThis || !pdnmlThis) return;

    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    if (!pmsdThis) return;
    parsGlobal = (PARS) pmsdThis->pGlobalPARS;
    if (!parsGlobal) return;

    iModel = (Int2) pdnmlThis->choice;
    SetActiveModel(((PFB) pmsdThis), iModel);

    Cn3d_DoHydrogens = parsGlobal->HydrogensOn;

    /*
     * We're now doing a single set of traversals, even when special features
     * are present. Thus, decisions about which elements are enabled (i.e. PPartialBBOn,
     * NTResiduesOn, etc., are made on a residue-by-residue basis, so here
     * we must go ahead with all RenderPartss without testing these booleans
     * yet - that work will be postponed until RenderGraph.
     */
    RenderParts(pdnmsThis, iModel, AM_PROT, ANYBACKBONE);
    RenderParts(pdnmsThis, iModel, AM_PROT, RESIDUES);
    RenderParts(pdnmsThis, iModel, AM_PROT, PBBLABELS);
     
    RenderParts(pdnmsThis, iModel, (AM_RNA | AM_DNA), ANYBACKBONE);
    RenderParts(pdnmsThis, iModel, (AM_RNA | AM_DNA), RESIDUES);
    RenderParts(pdnmsThis, iModel, (AM_DNA | AM_RNA), NTBBLABELS);


    /*
     * all the rest of this stuff is rendered based on each MSD's "global" PARS
     */
    if (parsGlobal->ObjectOn) {
        PVNMO pvnmoThis = pmsdThis->pvnmoHead;
        Cn3d_RenderNow = R_DEFAULT;
        Cn3d_ColorNow = parsGlobal->ObjectColor;
        while (pvnmoThis) { /* do the object traversal */
            if ((Int2) pvnmoThis->choice == iModel) {
                RenderObject(pvnmoThis);
            }
            pvnmoThis = pvnmoThis->next;
        }
    }

    if (parsGlobal->HeterogensOn) {
        Cn3d_RenderNow = parsGlobal->HetRender;
        Cn3d_ColorNow = parsGlobal->HetColor;
        RenderParts(pdnmsThis, iModel, (Byte) (AM_HET), HETSON);
        RenderParts(pdnmsThis, iModel, (Byte) (AM_POLY), HETSON);
    }
    if (parsGlobal->ConnectOn) {
        Cn3d_RenderNow = parsGlobal->ConnectRender;
        Cn3d_ColorNow = parsGlobal->ConnectColor;
        RenderParts(pdnmsThis, iModel, 0, CONNECTON);
    }
    if (parsGlobal->SolventOn) {
        Cn3d_RenderNow = parsGlobal->SolventRender;
        Cn3d_ColorNow = parsGlobal->SolventColor;
        RenderParts(pdnmsThis, iModel, (Byte) (AM_SOL | AM_WAT), SOLVENTON);
    }
    if (parsGlobal->PTermLabelOn) {
        Cn3d_RenderNow = R_NAME;
        Cn3d_ColorNow = parsGlobal->PBBColor;
        RenderParts(pdnmsThis, iModel, (Byte) (AM_PROT), PTERMLABELS);
    }
    if (parsGlobal->NTTermLabelOn) {
        Cn3d_RenderNow = R_NAME;
        Cn3d_ColorNow = parsGlobal->NTBBColor;
        RenderParts(pdnmsThis, iModel, (Byte) (AM_DNA | AM_RNA), NTTERMLABELS);
    }
    if (parsGlobal->IonsOn) {
        Cn3d_RenderNow = parsGlobal->IonRender;
        Cn3d_ColorNow = parsGlobal->IonColor;
        RenderParts(pdnmsThis, iModel, (Byte) (AM_ION), IONSON);
    }

}

void LIBCALL fnMSPLoop(PDNMS pdnmsThis)
{
    PMSD pmsdThis = NULL;
    PDNML pdnmlThis = NULL;
    PMLD pmldThis = NULL;

    if (!pdnmsThis) return;

    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    pdnmlThis = pmsdThis->pdnmlModels;

    while (pdnmlThis) {
        pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
        if (pmldThis->bSelected & (Byte) 0x01) {
            fnARModelLoop(pdnmsThis, pdnmlThis);
        }
        pdnmlThis = pdnmlThis->next;
    }
    return;
}


#ifdef _OPENGL
static void Cn3D_MakeOGLPalette(TCn3D_ColorData * ColorData)
{
    TOGL_Data *OGL_Data = ColorData->OGL_Data;
    ValNodePtr Palette, PaletteIndex, PaletteExpanded;
    Int4 i, j, TotalColors = 0;
    Int4 nShades;                /* the number of palette entries per color */
    Uint1 *red, *green, *blue;

    Palette = ColorData->pDDVColorGlobal->Palette;
    if (!Palette)
        return;

    DDV_SetColorChoice(Palette); /* set up the indices */

    /* count the colors */
    for (Palette = ColorData->pDDVColorGlobal->Palette; Palette;
         Palette = Palette->next) {
        TotalColors++;
        if (TotalColors > CN3D_MAX_PALETTE)
            break;              /* limit the maximum number of colors for now */
    }

    nShades = CN3D_MAX_PALETTE / TotalColors;
    ValNodeFreeData(OGL_Data->PaletteIndex);
    OGL_Data->PaletteIndex = NULL;
    ValNodeFreeData(OGL_Data->PaletteExpanded);
    OGL_Data->PaletteExpanded = NULL;

    Palette = ColorData->pDDVColorGlobal->Palette;
    for (i = 0; Palette && i < TotalColors; i++, Palette = Palette->next) {
        /* create a color entry in the index */
        PaletteIndex =
            ValNodeAddPointer(&(OGL_Data->PaletteIndex), 0,
                              MemNew(sizeof(TOGL_PaletteIndex)));
        DDV_CopyColorCell(&(((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->ColorCell),
                          &(((DDV_ColorEntry *) (Palette->data.ptrvalue))->ColorCell));

        ((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->Begin = i * nShades; /* start ramp */
        /* create a color ramp */
        for (j = 0; j < nShades; j++) {
            PaletteExpanded =
                ValNodeAddPointer(&(OGL_Data->PaletteExpanded), 0,
                                  MemNew(sizeof(DDV_ColorCell)));

            ((DDV_ColorCell *) (PaletteExpanded->data.ptrvalue))->rgb[0] =
                (Uint1) (((DDV_ColorEntry *) (Palette->data.ptrvalue))->
                         ColorCell.rgb[0] * (((float)(j + 1)) / nShades));
            ((DDV_ColorCell *) (PaletteExpanded->data.ptrvalue))->rgb[1] =
                (Uint1) (((DDV_ColorEntry *) (Palette->data.ptrvalue))->
                         ColorCell.rgb[1] * (((float)(j + 1)) / nShades));
            ((DDV_ColorCell *) (PaletteExpanded->data.ptrvalue))->rgb[2] =
                (Uint1) (((DDV_ColorEntry *) (Palette->data.ptrvalue))->
                         ColorCell.rgb[2] * (((float)(j + 1)) / nShades));
        }
        ((TOGL_PaletteIndex *) (PaletteIndex->data.ptrvalue))->End =
            i * nShades + nShades - 1; /* end ramp */
    }

    /* set the system palette to the expanded OGL palette */
    TotalColors = ValNodeLen(OGL_Data->PaletteExpanded);
    red = Calloc((size_t) TotalColors + 1, sizeof(Uint1));
    blue = Calloc((size_t) TotalColors + 1, sizeof(Uint1));
    green = Calloc((size_t) TotalColors + 1, sizeof(Uint1));

    for (Palette = OGL_Data->PaletteExpanded, i = 0; 
            Palette;
            Palette = Palette->next, i++) {
        red[i] =
            ((DDV_ColorCell *) (Palette->data.ptrvalue))->rgb[0];
        green[i] =
            ((DDV_ColorCell *) (Palette->data.ptrvalue))->rgb[1];
        blue[i] =
            ((DDV_ColorCell *) (Palette->data.ptrvalue))->rgb[2];
    }

    red[TotalColors] = OGL_Data->Background.rgb[0];
    green[TotalColors] = OGL_Data->Background.rgb[1];
    blue[TotalColors] = OGL_Data->Background.rgb[2];
            
    Nlm_Set3DColorMap(OGL_Data->Panel, (Uint1) (TotalColors + 1),
                        red, green, blue, &OGL_Data->display);
    MemFree(red);
    MemFree(green);
    MemFree(blue);
}
#endif                          /* _OPENGL */


void LIBCALL MakeStrucPalette(PDNMS pdnmsThis)
{
    PMSD pmsdThis = NULL;
    PDNMS pdnmsThisSlave = NULL;
    PARS pars = NULL;
    SeqAnnotPtr sanp;
    SeqAlignPtr salp = NULL;

    if (!pdnmsThis) return;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

    pars = (PARS) pmsdThis->pGlobalPARS;
    if (!pars)return;

    /* make column colors available */
    for (sanp = Cn3D_ColorData.sap; sanp != NULL; sanp = sanp->next) {
        if (sanp->data != NULL) {
            salp = sanp->data;
            CSC_CalculateColumnColors(salp, pars->ConsColAlg);
            break;
        }
    }

#ifdef _OPENGL
    /* if 8-bit color: clear palette; do palette construction loops first */
    if (Cn3D_ColorData.OGL_Data->IndexMode) {
#endif
    DDV_FreePalette(&(Cn3D_ColorData.pDDVColorGlobal->Palette));

    Cn3d_ColorPass = TRUE;
    Cn3d_lSlaveNum = 0;        

    fnMSPLoop(pdnmsThis); /* go over the master */

    if (AreNeighborsOn()) {
        pdnmsThisSlave = pmsdThis->pdnmsSlaves; /* go through the slave structures */
        while (pdnmsThisSlave) {
            Cn3d_lSlaveNum++;
            if (((PMSD) (pdnmsThisSlave->data.ptrvalue))->bVisible)
                fnMSPLoop(pdnmsThisSlave);
            pdnmsThisSlave = pdnmsThisSlave->next;
        }
    }

    Cn3d_ColorPass = FALSE;
    ResetDisplayCtrls();  /* in case anything has been hidden */

#ifdef _OPENGL
        Cn3D_MakeOGLPalette(&Cn3D_ColorData); /* expand palette for 8-bit OpenGL */
    }
#endif

    /* tell the color-by-conservation module to apply appropriate colors
    to any rows that don't have structure - i.e., that haven't already
    been colored by the above structure traversals */
    if (salp) {
        /* use "show alignment" algorithm simply to color (red) aligned residues
           for unaligned rows, to match structure alignment view */
        if (pars->PBBColor == C_BYSTRUCALIGN)
                CSC_CalculateColumnColors(salp, CSC_SHOWALIGNED);
        CSC_SetNonStructureDDVRowCells(pmsdThis,
            (pars->PBBColor == C_BYSEQCONS || pars->PBBColor == C_BYSTRUCALIGN),
            (pars->PBBColor == C_BYSEQCONS)
                ? Cn3D_UnalignedResidueColor
                : Cn3d_PaletteRGB[C_black], 
            Cn3D_ColorData.pDDVColorGlobal);
    }

    return;
}


/*
 * Set up separate layers for each member of conf-ensembles, if and only
 * if there's a single pdb-model (and, of course, it has conf-ensembles).
 */
static void CheckForConfEnsembles(PDNMS pdnmsThis)
{
    PDNML pdnmlThis = NULL;
    PMLD pmldThis;
    int nPDBModels = 0;
    BiostrucModelPtr pbsmModel;
    ModelCoordinateSetPtr pmcsCoordSet;
    CoordinatesPtr pcCoords;
    AtomicCoordinatesPtr pacCoords;
    ConformationEnsemblePtr pceEnsembles;
    AlternateConformationIdsPtr paciAltConfIDs;
    Nlm_CharPtr pcId;
    int nIDs;

    Cn3d_nEnsembles = Cn3d_currentEnsemble = 0;
    if (!pdnmsThis ||
        !(pdnmlThis = ((PMSD) pdnmsThis->data.ptrvalue)->pdnmlModels))
        return;

    while (pdnmlThis) { /* look for a pdb-model */
        pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
        pbsmModel = pmldThis->pbsmModel;

        if (pbsmModel->type == Model_type_pdb_model) {
            nPDBModels++;
            if (nPDBModels > 1) goto skip_it;

            /* descend the asn1 object to see if this pdb-model has
               conf-ensembles; if so, build a string of alt-conf-ids for
               each ensemble into the Cn3d_altConfIDs array */
            if (!(pmcsCoordSet = pbsmModel->model_coordinates)) goto skip_it;
            if (pmcsCoordSet->Coordinates_coordinates->choice !=
                Coordinates_coordinates_literal) goto skip_it;
            pcCoords = (ValNodePtr)
                pmcsCoordSet->Coordinates_coordinates->data.ptrvalue;
            if (pcCoords->choice != Coordinates_atomic) goto skip_it;
            pacCoords = (AtomicCoordinatesPtr) pcCoords->data.ptrvalue;
            if (!(pceEnsembles = pacCoords->conf_ensembles)) goto skip_it;
            while (pceEnsembles) {
                paciAltConfIDs = pceEnsembles->alt_conf_ids;
                nIDs = 0;
                while (paciAltConfIDs) {
                    pcId = (Nlm_CharPtr) paciAltConfIDs->data.ptrvalue;
                    Cn3d_altConfIDs[Cn3d_nEnsembles][nIDs] = *pcId;
                    if (nIDs == MAXALTCONF - 2) break;
                    nIDs++;
                    Cn3d_altConfIDs[Cn3d_nEnsembles][nIDs] = '\0';
                    paciAltConfIDs = paciAltConfIDs->next;
                }
                Cn3d_nEnsembles++;
                if (Cn3d_nEnsembles == MAXALTCONF) break;
                pceEnsembles = pceEnsembles->next;
            }
            if (Cn3d_nEnsembles < 2) goto skip_it;
        }

        pdnmlThis = pdnmlThis->next;
    }
    return;

skip_it:
    Cn3d_nEnsembles = Cn3d_currentEnsemble = 0;
}

void LIBCALL fnARLoop(PDNMS pdnmsThis)
{
    /* the rendering engine  */
    PDNML pdnmlThis = NULL;
    Int2 iTest;
    PMSD pmsdThis = NULL;
    PMLD pmldThis = NULL;


    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

    iTest =
        TraverseModels(pdnmsThis, TRAVERSE_ATOM, 0, NULL, DoGraphicNull);

    CheckForConfEnsembles(pdnmsThis);

    pdnmlThis = pmsdThis->pdnmlModels;
    while (pdnmlThis) {
        pmldThis = (PMLD) pdnmlThis->data.ptrvalue;

        if (pmldThis->bSelected & (Byte) 0x01) {

#ifdef _OPENGL
            Int4 i;
            DDV_ColorCell Color;

            /* set font display lists according to current label control panel menu */
            Cn3D_SetOGLFont(Cn3D_ColorData.OGL_Data);

            OGL_Start(Cn3D_ColorData.OGL_Data, Cn3d_LayerNow);
            OGL_PushTransformation(((PMSD) (pdnmsThis->data.ptrvalue))->pdnSlaveToMasterTransforms);

            if (MeshLoaded) {
                Color.rgb[0] = 255;
                Color.rgb[1] = 0;
                Color.rgb[2] = 0;
                for (i = 0; i < Mesh.NumTri; i++)
                    OGL_AddTri3D(Cn3D_ColorData.OGL_Data, &Color,
                                 Mesh.Vertices[Mesh.Tri[i].Vertex[0] - 1].Vertex,
                                 Mesh.Vertices[Mesh.Tri[i].Vertex[1] - 1].Vertex,
                                 Mesh.Vertices[Mesh.Tri[i].Vertex[2] - 1].Vertex,
                                 Mesh.Vertices[Mesh.Tri[i].Vertex[0] - 1].Normal);
            }
#endif                          /* _OPENGL */

            fnARModelLoop(pdnmsThis, pdnmlThis);

#ifdef _OPENGL
            OGL_PopTransformation();
            OGL_End();
#endif                          /* _OPENGL */

            Cn3d_LayerNow++;

            if (Cn3d_nEnsembles &&
                pmldThis->pbsmModel->type == Model_type_pdb_model) {
#ifdef _OPENGL
                OGL_SetLayer(Cn3D_ColorData.OGL_Data, Cn3d_LayerNow - 1,
                             (Cn3d_currentEnsemble == 0) ? TRUE : FALSE);
#endif
                Cn3d_currentEnsemble++;
                if (Cn3d_currentEnsemble < Cn3d_nEnsembles) {
                    continue; /* don't go to next pndml until ensembles done */
                } else {
                    Cn3d_nEnsembles = 0; /* turn off ensembles */
                }
            }

        } /* if model selected */

        pdnmlThis = pdnmlThis->next;
    }                           /* while(pdnmlThis) */
}


#ifndef _OPENGL
void Cn3d_Lock3DPalette(Picture3D ppic)
{
    ValNodePtr Palette;
    Int4 i = 0;

    Palette = Cn3D_ColorData.pDDVColorGlobal->Palette;
    if (!Palette) {
        AllocPalette3D(ppic, 1);
        SetColor3D(ppic, 0, 255, 255, 255);
        return;
    }

    DDV_SetColorChoice(Palette);

    while (Palette) {
        i++;
        if (i > CN3D_MAX_PALETTE)
            break;              /* limit the maximum number of colors for now */
        Palette = Palette->next;
    }

    AllocPalette3D(ppic, (Uint1) i);
    if (readErrors())
        return;

    Palette = Cn3D_ColorData.pDDVColorGlobal->Palette;
    i = 0;

    while (Palette) {
        SetColor3D(ppic, (Uint1) i,
                   ((DDV_ColorEntry *) (Palette->data.ptrvalue))->
                   ColorCell.rgb[0],
                   ((DDV_ColorEntry *) (Palette->data.ptrvalue))->
                   ColorCell.rgb[1],
                   ((DDV_ColorEntry *) (Palette->data.ptrvalue))->
                   ColorCell.rgb[2]);
        if (readErrors())
            return;
        i++;
        if (i > CN3D_MAX_PALETTE)
            break;              /* limit the maximum number of colors for now */
        Palette = Palette->next;
    }
}
#endif                          /* ndef _OPENGL */


#ifdef _OPENGL
void LIBCALL AlgorithmicRendering(void)
#else
Picture3D LIBCALL AlgorithmicRendering(Picture3D p3d)
#endif
{
    PDNMS pdnmsThis = NULL, pdnmsThisSlave = NULL;
    PMSD pmsdThis = NULL;
    Int2 Cn3d_LayerNow_sofar = 0;
    Int2 i = 0;

#ifndef _OPENGL
    pic = p3d;                  /* pic is the global 3d picture for algorend.c */
#endif

    /* fetch the active structure */
    pdnmsThis = GetSelectedModelstruc();
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    if (!pdnmsThis || !pmsdThis)
        return
#ifndef _OPENGL
            NULL
#endif
            ;
    Cn3d_LayerNow = 0;
    Cn3d_AnyPrim = FALSE;       /* has any primitive been drawn? */
    Cn3d_ColorPass = FALSE;
    Cn3d_lSlaveNum = 0;

    MakeStrucPalette(pdnmsThis);

#ifndef _OPENGL
    Cn3d_Lock3DPalette(p3d); /* Allocates the Picture3D palette  */
#endif

#ifdef _OPENGL
    OGL_SetLayers(Cn3D_ColorData.OGL_Data, FALSE); /* null all the layers out */
    OGL_ClearTransparentSpheres();
#endif                          /* _OPENGL */

    /* do master */
    if (pmsdThis->bVisible)
        fnARLoop(pdnmsThis);

    /* then go through the slaves */
    if (AreNeighborsOn()) {
        pdnmsThisSlave = pmsdThis->pdnmsSlaves;
        while (pdnmsThisSlave) {
            Cn3d_lSlaveNum++;
            if (((PMSD) (pdnmsThisSlave->data.ptrvalue))->bVisible) {
                fnARLoop(pdnmsThisSlave);
            }
            pdnmsThisSlave = pdnmsThisSlave->next;
        }
    }

#ifdef _OPENGL
    OGL_SetLayerTop3D(Cn3D_ColorData.OGL_Data, Cn3d_LayerNow - 1);
    return;
#else                           /* _OPENGL */
    SetLayerTop3D(Cn3d_LayerNow - 1);
    if (Cn3d_AnyPrim)
        return p3d;             /* has a primitive been drawn? */
    else {
        if (p3d != NULL)
            DeletePicture3D(p3d);
        return NULL;
    }
#endif                          /* else _OPENGL */
}


#ifndef _OPENGL
Picture3D LIBCALL Do3DOrigin(Picture3D p3d)
{
    FloatLo fFrom[3];
    FloatLo fTo[3];
    Int4 iXFrom, iYFrom, iZFrom;
    Int4 iXTo, iYTo, iZTo;
    CharPtr pclabel[] = { "X", "Y", "Z" };
    PDNMS pdnmsThis = NULL;
    PMSD pmsdThis = NULL;
    Int4 i;
    DDV_ColorCell iColor;

    /* appends origin onto p3d */

    if (p3d == NULL) {          /* origin forced because no other primitives */
        Cn3d_LayerNow = 0;
        SetLayerTop3D(0);
        Cn3d_ColorPass = FALSE;
    }

    pdnmsThis = GetSelectedModelstruc();
    if (!pdnmsThis)
        return NULL;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    DDV_SetColorInCell(&iColor, Cn3d_PaletteRGB[C_white]);
    if (Cn3d_ColorPass) {
        DDV_RequestColor(&Cn3D_ColorData.pDDVColorGlobal->Palette, &iColor);
        return NULL;
    }


    for (i = 0; i < 3; i++) {
        fFrom[i] = 0.0;
        fTo[i] = 0.0;
    }
    for (i = 0; i < 3; i++) {
        fFrom[i] = 5.0;
        fTo[i] = -5.0;
        /*
        RotTransScale(pmsdThis, fFrom[0],
                      fFrom[1], fFrom[2], &iXFrom, &iYFrom, &iZFrom);
        RotTransScale(pmsdThis, fTo[0],
                      fTo[1], fTo[2], &iXTo, &iYTo, &iZTo);
        */
        AddLine3D(p3d, NULL, (BigScalar) 0, Cn3d_LayerNow,
                  DDV_ColorIndex(Cn3D_ColorData.pDDVColorGlobal->Palette,
                                 &iColor), iXFrom, iYFrom, iZFrom, iXTo,
                  iYTo, iZTo);
        AddText3D(p3d, NULL, (BigScalar) 0, Cn3d_LayerNow,
                  DDV_ColorIndex(Cn3D_ColorData.pDDVColorGlobal->Palette,
                                 &iColor), pclabel[i], iXFrom, iYFrom,
                  iZFrom, (Uint4) (4 * VIEWSCALE / 2), 0, 0);
        fFrom[i] = 0.0;
        fTo[i] = 0.0;
    }

    return p3d;
}

#endif                          /* ndef _OPENGL */

