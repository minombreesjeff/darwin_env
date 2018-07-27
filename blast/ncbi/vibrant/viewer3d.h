/*   viewer3d.h
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
* File Name:  viewer3d.h
*
* Author:  Alex Smirnov
*
* Version Creation Date:   03/29/95
*
* $Revision: 6.3 $
*
* File Description:  3D-Viewer API
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: viewer3d.h,v $
* Revision 6.3  2000/01/11 01:16:45  lewisg
* fix color selection in Cn3D, other misc. bugs
*
* Revision 6.2  1998/08/12 23:04:07  vakatov
* [64-bit platforms]  Made "idBuffer" be VoidPtr[] rather than Int4[];
* Fixed bugs mostly caused by casting between Int4 vars and 8-byte pointers
*
* Revision 6.1  1998/04/20 18:30:40  lewisg
* moved typedef for _Nlm_Viewer3D to include file
*
* Revision 6.0  1997/08/25 18:58:20  madden
* Revision changed to 6.0
*
* Revision 5.4  1997/03/31 16:52:56  vakatov
* Changed the Nlm_CreateViewer3D() proto to reduce a number of arguments
* and to allow one to specify appearance of any(incl. Z-)rotation scrollbars.
* Splitted std. mouse group Mouse3D_Rotate --> Mouse3D_Rotate[YX,ZX,YZ].
*
 * Revision 1.13  1997/03/31  16:19:31  vakatov
 * XZ --> ZX
 *
 * Revision 1.12  1997/03/25  22:59:44  vakatov
 * All coord-rotation code from rotateXY(), QRotate() and RotateVierer()
 * moved to the new universal Rotate()
 * Z-rotation added to alternative standard mouse groups(-XZ and -YZ)
 *
 * Revision 1.11  1997/03/25  19:40:42  vakatov
 * Z-rotation works;  wanna revise the former rotateXY() function to let
 * it rotate around an arbitrary point
 *
 * Revision 5.3  1997/03/20  19:02:55  vakatov
 * Heavily redesigned to work with the new panel-mouse management.
 * Revised API for work with the 3D-Viewer controls.
 * Hided internal structure definitions and the Viewer3D handle itself.
 *
 * Revision 5.2  1996/06/14  16:39:21  kans
 * copy and print 3d viewer now ifndef win_motif
 *
 * Revision 5.1  1996/06/14  14:29:58  vakatov
 * Nlm_SetBackground3D() added to change 3D-Viewer background color.
 * "Copy" and "Print" buttons related stuff carefully commented everywhere.
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 1.13  1996/05/23  14:36:35  hogue
 * Fixed PrintViewer/PasteViewer discrepancy in .h file, added
 * IsPlaying3D, StopPlaying3D, StartPlaying3D for more
 * external control of the animation
 *
 * Revision 1.12  1996/05/13  15:04:38  hogue
 * Save button in Viewer Controls now optional
 *
 * Revision 1.11  1996/05/09  18:30:38  vakatov
 * Added CAMERA_SIZE_I4 constant for an external use
 *
 * Revision 1.10  1996/05/07  18:30:25  vakatov
 * Nlm_CreateViewer3D() prototype -- modified
 *
 * Revision 1.9  1996/04/17  21:31:33  vakatov
 * Added "Nlm_GetViewerImage3D()" function returning image of the 3D-viewer.
 *
 * Revision 1.8  1996/04/07  20:04:56  kans
 * getlayertop3d needed (void) as prototype
 *
 * Revision 1.7  1996/04/04  18:52:45  vakatov
 * The camera position storing structure size is increased
 *
 * Revision 1.6  1996/04/02  22:30:53  vakatov
 * AttachZoomPic3D() removed; now previously stored camera position
 * is to be restored within AttachPicture3D automatically, as soon as
 * valid (non-NULL and initialized) "camera" argument is provided
 *
 * Revision 1.5  1996/04/01  23:03:09  vakatov
 * Added "dscale" member to the "Nlm_camera3d" structure
 *
 * Revision 1.4  1996/03/30  23:25:55  hogue
 * Added AttachZoomPic3D
 *
*
*/

#ifndef _VIEWER3D_
#define _VIEWER3D_

#ifndef _VIBRANT_
#include <vibrant.h>
#endif

#ifndef _PICTUR3D_
#include <pictur3d.h>
#endif

#include <image.h>
#include <vibmouse.h>

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   DEFINES
*
*****************************************************************************/
/* Types of adjust for Nlm_SetAdjust3D() */
#define ADJUST3D_PERSPECT 1
#define ADJUST3D_BRIGH    2
#define ADJUST3D_COLOR    3

#define enumStdMA3D Nlm_enumStdMA3D
#define MAX_FOUND_PRIM 32


/*****************************************************************************
*
*   TYPEDEFS
*
*****************************************************************************/

/* Standard set of the viewer mouse callback groups
 */
typedef enum
{
  Mouse3D_DoNothing = 0,

  Mouse3D_RotateYX,
  Mouse3D_RotateZX,
  Mouse3D_RotateYZ,
  Mouse3D_Move,
  Mouse3D_Zoom,

  Mouse3D_NumStd
}  Nlm_enumStdMA3D;

typedef struct _Nlm_Viewer3D
{
  double      dAOrt[3][3];
  double      dC[3];
  double      dscale;
  Int4        a[3][3];
  Int4        c[3];
  Int4        scale;
  Int4        zmax;
  Picture3D   pic;
  Image       image;
  Boolean     layerTable[256];
  Int4        p_version;
  Int4        g_version;
  Int4        g_versionPrev;
  Int4        c_version;
  Int4        c_versionPrev;
  Int4        colorOffset;
  Int4        colorStep;
  Int4        colorScale;
  MAPtr       ma;
  MA_GroupPtr ma_std_group[Mouse3D_NumStd];
  BaR         z_rotate;
  PaneL       panel;
  WindoW      parentWindow;
  Handle      zBuffer;
  Prim3D      foundPrim[MAX_FOUND_PRIM];
  Uint2       nPrimFound;
  Int2        width;
  Int2        height;
  Int2        is_zoomed;
  Uint1       perspectN;
  Uint1       brighN;
  Uint1       colorPicN;
  Uint1       colorHLR, colorHLG, colorHLB;
  Uint1       colorFon;
  Uint1       adjust;
  Uint1       perspect;
  Uint1       colorFonR, colorFonG, colorFonB;
}  PNTR Nlm_Viewer3D;

struct _Nlm_Controls3D;
typedef struct _Nlm_Controls3D PNTR Nlm_Controls3D;


#define CAMERA_SIZE_I4 17
typedef struct Nlm_camera3d {
  Int4 dummy[CAMERA_SIZE_I4];
} Nlm_Camera3D, PNTR Nlm_Camera3DPtr;


typedef Boolean (*Nlm_MAInit3DFunc) PROTO((MAPtr ma, VoidPtr data));

/*****************************************************************************
*
*   EXTERN FUNCTION
*
*****************************************************************************/

/* Create 3D-viewer as a child of group "prnt";
 * set its size to "width"(can be adjusted a bit) and "height".
 * Provide the viewer with the rotation scrollbars according to
 * the "flags"(see #[XYZ]_SCROLL).
 * If "ma_group_menu" or "ma_action_menu"(can be a menubar cascade item or
 * submenu(s)) are not NULL then attach mouse-control user interface to these;
 * if "ma_init_func" is not NULL then it will be called with the
 * "ma_init_data"(as a <2nd> argument) right after the standard initialization
 * of the 3D-Viewer mouse controls.
 */
#define X_ROTATE_SBAR 0x1
#define Y_ROTATE_SBAR 0x2
#define Z_ROTATE_SBAR 0x4

extern Nlm_Viewer3D  Nlm_CreateViewer3D    PROTO((Nlm_GrouP prnt, 
                                    Uint2Ptr width, Uint2 height, 
                                    Int4 flags,
                                    MenU ma_group_menu, MenU ma_action_menu,
                                    Nlm_MAInit3DFunc ma_init_func,
                                    VoidPtr          ma_init_data)); 

/* Resize/Move the Viewer according to the "rect".
 * NOTE: "rect->right" can be adjusted(decreased by some points only!);
 *       if the viewer has scrollbar(s) then the scrollbars will be fit
 *       into the specified "rect".
 */
extern Boolean       Nlm_SetPosition3D     PROTO((Nlm_Viewer3D vvv,
                                                  RectPtr rect));

extern Image         Nlm_GetViewerImage3D  PROTO((Nlm_Viewer3D viewer));
extern void          Nlm_DeleteViewer3D    PROTO((Nlm_Viewer3D viewer));
extern Boolean       Nlm_AttachPicture3D   PROTO((Nlm_Viewer3D viewer, 
                                                  Picture3D picture, 
                                                  Nlm_Camera3DPtr camera));
extern void          Nlm_GetViewerInfo3D   PROTO((Nlm_Viewer3D viewer,
                                                  Picture3D PNTR pic,
                                                  Nlm_Camera3DPtr camera,
                                                  RectPtr rectangle));
extern Boolean       Nlm_RedrawViewer3D    PROTO((Nlm_Viewer3D viewer));
extern Uint2         Nlm_FindPrim3D        PROTO((Nlm_Viewer3D viewer, 
                                                  PoinT pt));
extern Prim3D        Nlm_GetFoundPrim3D    PROTO((Nlm_Viewer3D viewer, 
                                                  Uint2 primIndex));
extern void          Nlm_SetHLColor3D      PROTO((Nlm_Viewer3D viewer,
                                                  Uint1 red,
                                                  Uint1 green,
                                                  Uint1 blue));
extern void          Nlm_HighlightSeg3D    PROTO((Nlm_Viewer3D viewer,
                                                  Segment3D segment,
                                                  Boolean highlight));
extern Boolean       Nlm_IsSeg3DHlighted   PROTO((Nlm_Viewer3D viewer, 
                                                  Segment3D segment));
extern void          Nlm_HighlightPrim3D   PROTO((Nlm_Viewer3D viewer,
                                                  Prim3D prim,
                                                  Boolean highlight));
extern Boolean       Nlm_IsPrim3DHlighted  PROTO((Nlm_Viewer3D viewer, 
                                                  Prim3D prim));
extern Boolean       Nlm_ResetHighlight3D  PROTO((Nlm_Viewer3D viewer));
extern Boolean       Nlm_MapWorldToViewer3D  PROTO((Nlm_Viewer3D viewer,
                                                    Int4 x, Int4 y, Int4 z,
                                                    PoinT PNTR vPoint));


extern void          Nlm_SetBackground3D   PROTO((Nlm_Viewer3D viewer,
                                                  Nlm_Uint1 red,
                                                  Nlm_Uint1 green,
                                                  Nlm_Uint1 blue));

/* Standard set of the viewer controls
 */
extern Nlm_Controls3D Nlm_CreateControls3D PROTO((GrouP prnt,
                                                  Boolean vertical,
                                                  Boolean anim_ctrl,
                                                  GrouP  *ctrl_group));
extern void          Nlm_LinkControls3D    PROTO((Nlm_Controls3D controls,
                                                  Nlm_Viewer3D viewer));

/* Remove the group containig the viewer controls;
 * whenever the group is removed, the controls' internal data gets destroyed
 */
extern void          Nlm_DeleteControls3D  PROTO((Nlm_Controls3D controls));


/* Aux. functions for the viewer MA and PaneL callbacks 
 */
extern Nlm_Viewer3D Nlm_MAToViewer3D       PROTO((MAPtr ma));
extern Nlm_Viewer3D Nlm_PanelToViewer3D    PROTO((PaneL panel));


extern Boolean       Nlm_SetStdMouse3D     PROTO((Nlm_Viewer3D viewer,
                                                  Nlm_enumStdMA3D action));

/* Auxiliary functions, that can be used in button callbacks
 */
extern void          BgColorDlg3D         PROTO((Nlm_Viewer3D viewer));
extern void          Nlm_ZoomAll3D        PROTO((Nlm_Viewer3D viewer)); 
extern void          Nlm_SetAdjust3D      PROTO((Nlm_Viewer3D viewer,
                                                 Uint1 ajust)); 
extern void          Nlm_Adjust3D         PROTO((Nlm_Viewer3D viewer,
                                                 Boolean plus));
extern void          Nlm_SaveViewer3D     PROTO((Nlm_Viewer3D viewer));
#ifndef WIN_MOTIF
extern void          Nlm_CopyViewer3D     PROTO((Nlm_Viewer3D viewer));
extern void          Nlm_PrintViewer3D    PROTO((Nlm_Viewer3D viewer));
#endif

extern void 	     Nlm_PlayLayer3D      PROTO((Nlm_Viewer3D viewer));
extern void 	     Nlm_NextLayer3D      PROTO((Nlm_Viewer3D viewer));
extern void    	     Nlm_PrevLayer3D      PROTO((Nlm_Viewer3D viewer));
extern void	     Nlm_SetLayerTop3D    PROTO((Uint1 top));
extern Uint1         Nlm_GetLayerTop3D    PROTO((void));
extern Boolean       Nlm_IsPlaying3D      PROTO((void));
extern void          Nlm_StopPlaying3D    PROTO((void));
extern void          Nlm_StartPlaying3D   PROTO((Nlm_Viewer3D viewer));
extern void          Nlm_AllLayerSet3D(Nlm_Viewer3D viewer, Nlm_Boolean state);



/*****************************************************************************
*
*   NAME REDEFINITION
*
*****************************************************************************/
#define Viewer3D            Nlm_Viewer3D
#define Camera3D            Nlm_Camera3D
#define Controls3D          Nlm_Controls3D
#define Camera3DPtr         Nlm_Camera3DPtr
#define V3DControlsPtr      Nlm_V3DControlsPtr
#define CreateViewer3D      Nlm_CreateViewer3D
#define SetPosition3D       Nlm_SetPosition3D
#define CreateControls3D    Nlm_CreateControls3D
#define LinkControls3D      Nlm_LinkControls3D
#define DeleteControls3D    Nlm_DeleteControls3D
#define GetViewerImage3D    Nlm_GetViewerImage3D
#define DeleteViewer3D      Nlm_DeleteViewer3D
#define AttachPicture3D     Nlm_AttachPicture3D
#define GetViewerInfo3D     Nlm_GetViewerInfo3D
#define RedrawViewer3D      Nlm_RedrawViewer3D
#define FindPrim3D          Nlm_FindPrim3D
#define GetFoundPrim3D      Nlm_GetFoundPrim3D
#define SetHLColor3D        Nlm_SetHLColor3D
#define HighlightSeg3D      Nlm_HighlightSeg3D
#define IsSeg3DHlighted     Nlm_IsSeg3DHlighted
#define HighlightPrim3D     Nlm_HighlightPrim3D
#define IsPrim3DHlighted    Nlm_IsPrim3DHlighted
#define ResetHighlight3D    Nlm_ResetHighlight3D
#define MapWorldToViewer3D  Nlm_MapWorldToViewer3D
#define SetBackground3D     Nlm_SetBackground3D

#define MAToViewer3D        Nlm_MAToViewer3D
#define PanelToViewer3D     Nlm_PanelToViewer3D
#define SetStdMouse3D       Nlm_SetStdMouse3D
#define GetStdMouse3D       Nlm_SetStdMouse3D

#define ZoomAll3D           Nlm_ZoomAll3D
#define SetAdjust3D         Nlm_SetAdjust3D
#define Adjust3D            Nlm_Adjust3D
#define SaveViewer3D        Nlm_SaveViewer3D
#ifndef WIN_MOTIF
#define CopyViewer3D        Nlm_CopyViewer3D
#define PrintViewer3D       Nlm_PrintViewer3D
#endif
#define PlayLayer3D	    Nlm_PlayLayer3D   
#define NextLayer3D         Nlm_NextLayer3D      
#define PrevLayer3D   	    Nlm_PrevLayer3D     
#define SetLayerTop3D       Nlm_SetLayerTop3D
#define GetLayerTop3D       Nlm_GetLayerTop3D 
#define IsPlaying3D         Nlm_IsPlaying3D
#define StopPlaying3D       Nlm_StopPlaying3D
#define StartPlaying3D      Nlm_StartPlaying3D


#ifdef __cplusplus
}
#endif

#endif

