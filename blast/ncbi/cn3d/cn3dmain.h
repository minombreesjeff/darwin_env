/*   cn3dmain.h
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
* File Name:  cn3dmain.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   1/31/96
*
* $Revision: 6.30 $
*
* File Description: Main entry point for Cn3d 
*                   
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: cn3dmain.h,v $
* Revision 6.30  2000/03/08 21:46:14  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.29  2000/03/06 15:33:53  lewisg
* add new hydrophobic palette, update visual c++ project to move seqcons
*
* Revision 6.28  2000/03/01 16:17:55  thiessen
* improved handling of colors; many small fixes
*
* Revision 6.27  2000/02/10 17:47:02  thiessen
* added: color-by-sequence-conservation menu item, zoom-out to OpenGL, misc fixes
*
* Revision 6.26  2000/02/10 15:51:58  lewisg
* cn3d responds and send correct update messages.  many coloring bug fixes
*
* Revision 6.25  2000/01/18 22:49:16  lewisg
* send OM_MSG_FLUSH to ddv/udv, tweak CPK coloration, misc bugs
*
* Revision 6.24  2000/01/14 21:40:41  lewisg
* add translucent spheres, ion labels, new cpk, fix misc bugs
*
* Revision 6.23  2000/01/04 15:55:50  lewisg
* don't hang on disconnected network and fix memory leak/hang at exit
*
* Revision 6.22  1999/12/11 01:30:34  lewisg
* fix bugs with sharing colors between ddv and cn3d
*
* Revision 6.21  1999/11/22 14:46:43  thiessen
* moved _OPENGL code blocks to only vibrant and ncbicn3d libraries
*
* Revision 6.20  1999/11/18 00:21:42  lewisg
* draw speedups and selection on mouseup
*
* Revision 6.19  1999/11/10 23:19:40  lewisg
* rewrite of selection code for ddv
*
* Revision 6.18  1999/11/02 23:06:07  lewisg
* fix cn3d to launch correctly if there is no seqentry associated with bioseq
*
* Revision 6.17  1999/11/02 13:47:32  kans
* no star between DDV_ColorFunc and Function
*
* Revision 6.16  1999/11/01 22:10:27  lewisg
* add ability to call color functions by type, and add this to cn3d
*
* Revision 6.15  1999/10/29 14:15:28  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.14  1999/10/15 20:56:39  lewisg
* append DDV_ColorGlobal as userdata.  free memory when cn3d terminates.
*
* Revision 6.13  1999/10/05 23:18:19  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.12  1999/09/21 18:09:16  lewisg
* binary search added to color manager, various bug fixes, etc.
*
* Revision 6.11  1999/04/06 16:06:11  coremake
* Fixes of typos
*
* Revision 6.10  1999/04/06 14:23:30  lewisg
* add opengl replacement for viewer3d
*
* Revision 6.9  1999/02/11 18:48:15  lewisg
* delete color index functions
*
* Revision 6.8  1999/02/10 23:49:43  lewisg
* use RGB values instead of indexed palette
*
* Revision 6.7  1999/01/14 19:07:17  kans
* network availability is configurable
*
* Revision 6.6  1998/08/26 18:28:37  kans
* fixed -v -fd warnings
*
* Revision 6.5  1998/04/28 15:14:27  lewisg
* moved OpenMimeFileWithDeletion to cn3dopen
*
* Revision 6.4  1998/04/27 23:23:04  lewisg
* added ability to open mime files
*
* Revision 6.3  1998/04/20 18:36:10  lewisg
* moved extern for Viewer3d to cn3dmain.h
*
* Revision 6.2  1998/03/06 23:19:16  lewisg
* codewarrior fixes
*
* Revision 6.1  1998/03/06 01:19:47  lewisg
* merge
*
* Revision 6.0  1997/08/25 18:13:33  madden
* Revision changed to 6.0
*
* Revision 5.4  1997/07/29 21:17:09  vakatov
* [WIN32,DLL]  Made Cn3D's stubbed functions be DLL-exportable
*
* Revision 5.3  1997/03/20 19:03:44  vakatov
* Non-standalone(Entrez-specific) protos has been moved to "cn3dentr.h":
* Cn3D_SetQueryCallback() and Cn3DWin(), and the latter has been renamed
* to Cn3DWin_Entrez().
* The remainig protos concern to the "cn3dwin.c" code.
*
 * Revision 5.2  1996/07/29  21:13:02  epstein
 * add prototype for function Cn3D_SetQueryCallback()
 *
 * Revision 5.1  1996/06/12  14:31:50  hogue
 * Added Cn3DWin function for integration into Entrez
 *
 * Revision 5.0  1996/05/28  14:05:44  ostell
 * Set to revision 5.0
 *
 * Revision 1.4  1996/04/18  16:56:37  hogue
 * Altered color palette for multi-structure display
 *
 * Revision 1.3  1996/03/30  23:40:45  hogue
 * Redraw now saves camera
 *
 * Revision 1.2  1996/03/29  20:00:45  hogue
 * Integrated 3d viewing, menus & controls for algorithmic rendering
 *
 * Revision 1.1  1996/02/01  18:47:38  kans
 * Initial revision
 *
*
* ==========================================================================
*/

#ifndef _CN3DMAIN_
#define _CN3DMAIN_

/* number of colors used for secondary structure */
#define CN3D_COLOR_SS 4
/* colors used for secondary structure */
#define CN3D_COLOR_HELIX 0
#define CN3D_COLOR_STRAND 1
#define CN3D_COLOR_TURN 2
#define CN3D_COLOR_COIL 3
/* total number of colors in fixed palette */
#define CN3D_COLOR_MAX 54
/* total number of colors allowed in indexed palettes. keep room for background */
#define CN3D_MAX_PALETTE 100

#include <vibrant.h>

#include <math.h>
#include <objalign.h>
#include <mmdbapi.h>
#include <salmedia.h>
#include <ddvcolor.h>

 
#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* the typedef of a list used to keep track of the color functions */
typedef struct _Cn3D_ColorFuncList {
    Int2 Value;
    Char *Name;
    DDV_ColorFunc Function;
} Cn3D_ColorFuncList;

/* list of the color functions */
extern Cn3D_ColorFuncList Cn3D_ColorList[];

extern Uint1 Cn3d_PaletteRGB[CN3D_COLOR_MAX][3];

extern void LIBCALL Cn3D_EnableFileOps(void);
extern void LIBCALL Cn3D_DisableFileOps(void);
extern void LIBCALL Cn3D_DisableMenus(void);
extern void LIBCALL Cn3D_EnableMenus(void);
extern Boolean LIBCALL readErrors(void);

NLM_EXTERN void LIBCALL Cn3D_Redraw(Boolean New);
NLM_EXTERN void LIBCALL Cn3D_RedrawEx(Boolean New);
NLM_EXTERN void Cn3D_RedrawNUpdate(Boolean New);
NLM_EXTERN void LIBCALL Cn3D_ResetActiveStrucProc(void);
extern WindoW LIBCALL Cn3DWin(WndActnProc on_close, MenU * file_menu,
                              ItmActnProc netconfig, Boolean StandAlone);

extern Boolean LIBCALL VASTInit(void);
extern BiostrucAnnotSetPtr LIBCALL VASTBsAnnotSetGet(Int4 uid);
extern void Cn3DResizeProc(WindoW w);

#ifdef __cplusplus
}
#endif
#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif
#endif
