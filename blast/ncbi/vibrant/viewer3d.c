/*   viewer3d.c
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
* File Name:  viewer3d.c
*
* Author:  Alex Smirnov, Denis Vakatov
*
* Version Creation Date:   04/03/95
*
* $Revision: 6.5 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: viewer3d.c,v $
* Revision 6.5  2000/01/11 01:16:45  lewisg
* fix color selection in Cn3D, other misc. bugs
*
* Revision 6.4  1998/08/12 23:04:06  vakatov
* [64-bit platforms]  Made "idBuffer" be VoidPtr[] rather than Int4[];
* Fixed bugs mostly caused by casting between Int4 vars and 8-byte pointers
*
* Revision 6.3  1998/08/07 22:27:00  vakatov
* +FitZ() -- makes sure that picture does not outrun the "Z=0" plane
* +GetSphere3D() -- to keep in-sync the viewer and static Nlm_stCon
*
* Revision 6.2  1998/04/20 18:30:36  lewisg
* moved typedef for _Nlm_Viewer3D to include file
*
* Revision 6.1  1997/11/26 21:30:56  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 5.17  1997/06/12 20:56:57  vakatov
* Nlm_SetPosition3D() -- don't fail if the drawing area gets too small
* ScreenToWorld() -- fixed memory leak
*
* Revision 5.16  1997/06/11 17:07:36  vakatov
* DrawViewer3D():  redraw if p_version has changed
*
* Revision 5.15  1997/05/29 17:54:13  vakatov
* Provided unconditional redraw of highlighted primitives
*
* Revision 5.14  1997/05/20 15:15:08  vakatov
* Rotate():  on Z-rotation, now rotate around the center of currently
* visible area(instead of the picture center);  added ScreenToWorld()
* function to map screen coordinates to the world ones
*
* Revision 5.13  1997/03/31 16:52:37  vakatov
* Z-rotation implemented and added to alternative standard mouse
* groups(-ZX and -YZ). Added Z-rotation scrollbar.
* Moved all picture-rotation code from rotateXY(), QRotate() and
* RotateViewer() to the new universal Rotate().
*
 * Revision 1.25  1997/03/31  16:18:12  vakatov
 * Fixed scrollbar updating (using ResetClip) in ViewerRotate. Other
 * minor fixes.
 *
 * Revision 1.24  1997/03/25  22:59:41  vakatov
 * All coord-rotation code from rotateXY(), QRotate() and RotateVierer()
 * moved to the new universal Rotate()
 * Z-rotation added to alternative standard mouse groups(-ZX and -YZ)
 *
 * Revision 1.23  1997/03/25  19:40:38  vakatov
 * Z-rotation works;  wanna revise the former rotateXY() function to let
 * it rotate around an arbitrary point
 *
 * Revision 5.12  1997/03/21  16:17:05  vakatov
 * For SunOS compiler, casted most of the MemCpy() #1,#2 arguments to VoidPtr
 *
 * Revision 5.11  1997/03/20  19:03:16  vakatov
 * Heavily redesigned the 3D-Viewer code and internal data structures to work
 * with the new panel-mouse management.
 * Incapsulated code&data of the 3D-Viewer controls.
 * Added code for the "soft viewer resize".  General code cleaning.
 *
 * Revision 5.10  1996/11/22  19:47:29  vakatov
 * Nlm_CreateViewer3D():  set image-dependent background color;
 * + extensive type casting
 *
 * Revision 5.9  1996/09/20  13:54:47  vakatov
 * Nlm_AttachPicture3D(): assign new picture *at first*; then do anyth. else
 *
 * Revision 5.8  1996/09/19  18:54:18  vakatov
 * Replace Reset() by CorrectBarValue() to avoid undesirable call to
 * the scrollbar callbacks
 *
 * Revision 5.7  1996/07/25  14:52:34  epstein
 * BigScalar change for OSF1 portability
 *
 * Revision 5.6  1996/07/24  16:59:59  vakatov
 * Trapped crashes caused by attempts to rotate and zoom blank 3D-pictures
 *
 * Revision 5.5  1996/07/23  21:51:29  vakatov
 * [WIN_MOTIF]  Reserve 32th(instead of 0th) color for the background
 * color; it lets CN3D do not confuse other parts of the program and allows
 * to save the background color to GIF metafile
 *
 * Revision 5.3  1996/06/14  20:16:38  vakatov
 * [WIN_MSWIN]  Slight modification in the bg-color setting
 *
 * Revision 5.2  1996/06/14  14:33:04  vakatov
 * Added a set of functions to control the 3D-Viewer background color
 * [WIN_MOTIF] Special precautions made to avoid Motif/3D-Viewer color conflict
 *
 * Revision 5.1  1996/06/07  15:19:39  hogue
 * Removed the PlayLayer button and sundry stuff from MAC.
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 1.23  1996/05/23  14:36:35  hogue
 * Fixed PrintViewer/PasteViewer discrepancy in .h file, added
 * IsPlaying3D, StopPlaying3D, StartPlaying3D for more
 * external control of the animation
 *
 * Revision 1.22  1996/05/21  22:00:40  vakatov
 * The viewer scrollbar width/height are now treated more carefully.
 * Do not store viewer width and height in the camera position storage as
 * it caused a coredump when resizing the viewer.
 * Viewer rotation flag is ON(was OFF) in the very begining.
 *
 * Revision 1.21  1996/05/14  15:29:08  hogue
 * Made the Horizontal viewer controls more compact.
 *
 * Revision 1.20  1996/05/13  15:04:21  hogue
 * Save button in Viewer Controls now optional
 *
 * Revision 1.19  1996/05/09  18:45:35  vakatov
 * There was a bug resulting the program crash while switching between
 * pictures with different number of layers(and then restoring the
 * camera position) -- fixed.
 *
 * Revision 1.18  1996/05/07  18:30:03  vakatov
 * Nlm_CreateViewer3D() -- slightly changed;
 * Nlm_AllLayerSetStates3D() function -- added;
 * Nlm_AllLayerSet3D() -- modified, "state" parameter added
 * Nlm_RewindLayer3D() -- prototype and content modified;
 * + a lot of variable casting and code simplifying
 *
 * Revision 1.17  1996/04/23  16:30:47  vakatov
 * Nlm_PlayLayer3D() -- fixed an extra(some cycles) delay responding to
 * the V3D_bPLAY("Go") toggle button release event
 *
 * Revision 1.16  1996/04/17  21:31:58  vakatov
 * Implemented "Nlm_SaveViewer3D()" function which allows one to save
 * presently visible part of the 3D-viewer picture into a GIF metafile.
 * Added "Copy" button to the 3D-viewer controls.
 * Fixed a bug caused by "the asynchroneous commenting" of "Print" and "Copy"
 * controls.
 * Added "Nlm_GetViewerImage3D()" function returning image of the 3D-viewer.
 *
 * Revision 1.15  1996/04/09  15:24:49  vakatov
 * Most of the basic, commonly used colors (0 - 15, eventually including
 * white and black) are not to be overwritten by the 3D-viewer image
 * drawing routine now.
 * The initial brightness level has been increased from 3 to 4 (43% to 57%).
 *
 * Revision 1.14  1996/04/07  20:04:56  kans
 * getlayertop3d needed (void) as prototype
 *
 * Revision 1.13  1996/04/04  18:58:39  vakatov
 * Slight corrections to the camera storing/restoring algorithm and
 * the relevant data to avoid the object/camera position mismatching --
 * while setting the camera attributes stored during the work with another
 * picture.
 *
 * Revision 1.12  1996/04/03  16:03:33  vakatov
 * Now ZoomAll() can keep the camera angle (ZOOM_KEEP_ANGLE method)
 *
 * Revision 1.11  1996/04/02  22:30:02  vakatov
 * AttachZoomPic3D() removed; now previously stored camera position
 * is to be restored within AttachPicture3D automatically, as soon as
 * valid (non-NULL and initialized) "camera" argument is provided
 *
 * Revision 1.10  1996/04/01  23:07:03  vakatov
 * Now one can use prevously stored camera position
 * in the "AttachZoomPic3D(...,TRUE)"
 *
 * Revision 1.8  1996/03/30  23:48:02  hogue
 * removed unused variables, fixed a return value.
 *
 * Revision 1.7  1996/03/30  23:24:47  hogue
 * Cast MemCpy and Memset, changed camera to Int4, fixed nonportable uses of MemCpy, Added an AttachZoomPic .  Cameras are still not always saved and restored correctly...
 *
* ==========================================================================
*/

#include <math.h>

#ifndef _VIBRANT_
#include <vibrant.h>
#endif

#ifndef _VIEWER3D_
#include <viewer3d.h>
#endif

#ifndef _PPICT3D_
#include <ppict3d.h>
#endif

#include <matrix.h>


/*****************************************************************************
*
*   DEFINES
*
*****************************************************************************/

#define CAMERA_KEY      8071967
#define FIND_BLOCK_SIZE 8


/*****************************************************************************
*
*   TYPEDEFS and DECLARATIONS
*
*****************************************************************************/

typedef struct _Nlm_Viewer3D structViewer3D;

typedef struct
{
  Uint1  perspectN;
  Uint1  brighN;
  Uint1  colorPicN;
  Uint1  colorHLR, colorHLG, colorHLB;
  Uint1  colorFon;
  Uint1  adjust;
  Int4   scale;
  Uint1  colorFonR, colorFonG, colorFonB, dummy_align;
}  PCamera3DUnion;


struct _Nlm_Controls3D
{
  GrouP   group;

  ButtoN  zoomAll;
  ButtoN  Move3D;
  ButtoN  Zoom3D;
  GrouP   adjustButtons;
  ButtoN  plusButton;
  ButtoN  minusButton;

/* layer - animation controls */
  ButtoN  allButton;
  ButtoN  rewindButton;
  ButtoN  prevButton;
  ButtoN  nextButton;
  ButtoN  playButton;
}  structControls3D;



typedef enum
{
  ZOOM_ALL,
  ZOOM_KEEP_ANGLE,
  ZOOM_KEEP_ALL
}
enumZoom3D;


typedef enum
{
  ROTATE_X,
  ROTATE_Y,
  ROTATE_Z
}
enumRotate3D;


/*****************************************************************************
*
*   GLOBAL VARIABLES
*
*****************************************************************************/

Uint1  Nlm_Isqrt[129*129];
extern Nlm_Context3D Nlm_stCon;



/*****************************************************************************
*
*   STATIC VARIABLES
*
*****************************************************************************/

static CharPtr      viewerClass = "Viewer3D";
static Boolean      isqrtInit = FALSE;
static Uint1        perspectAll[6] = { 8,6,4,3,2,1 };
static Int2         brighAll[7]    = { -150,-100,-50,0,50,100,150 };
static Uint1        colorPicAll[6] = { 2,3,4,6,8,255 };
static Uint1        currlayer = 0;
static ButtoN       V3D_bPLAY = NULL;
static Uint1        toplayer  = 0;


/*****************************************************************************
*
*   STATIC FUNCTIONS
*
*****************************************************************************/

static void SetUpContext(Viewer3D vvv)
{
  Nlm_stCon.xmin = (Int4)0;
  Nlm_stCon.xmax = (Int4)vvv->width - 1;
  Nlm_stCon.ymin = (Int4)0;
  Nlm_stCon.ymax = (Int4)vvv->height - 1;
  Nlm_stCon.zmax = vvv->zmax;
  Nlm_stCon.pic  = vvv->pic;
  Nlm_stCon.width  = (Int4)vvv->width;
  Nlm_stCon.height = (Int4)vvv->height;
  MemCpy(Nlm_stCon.layerTable, vvv->layerTable, sizeof(Nlm_stCon.layerTable));
  MemCpy((VoidPtr)Nlm_stCon.a, (VoidPtr)vvv->a, sizeof(Nlm_stCon.a         ));
  MemCpy((VoidPtr)Nlm_stCon.c, (VoidPtr)vvv->c, sizeof(Nlm_stCon.c         ));
  Nlm_stCon.scale = vvv->scale;
  Nlm_stCon.colorOffset = vvv->colorOffset;
  Nlm_stCon.colorStep   = vvv->colorStep;
  Nlm_stCon.colorScale  = vvv->colorScale;
  Nlm_stCon.perspect   = vvv->perspect;
  Nlm_stCon.zmaxPersp  = Nlm_stCon.perspect  * Nlm_stCon.zmax;
  Nlm_stCon.zmaxPersp1 = Nlm_stCon.zmaxPersp + Nlm_stCon.zmax;
  if ( vvv->pic )
    Nlm_stCon.colorHL = (Uint1)((Nlm_PPict3DPtr)vvv->pic)->totalColors;
}

static Boolean GetSphere3D(Viewer3D vvv, Spher3D *sph)
{
  SetUpContext(vvv);
  return Nlm_GetSegSphere3D(vvv->pic, NULL, sph);
}


static void MakeIntMat(Viewer3D vvv)
{
  Int2   i;
  double PNTR dPtr = &vvv->dAOrt[0][0];
  Int4   PNTR iPtr = &vvv->a[0][0];

  for (i = 0;  i < 9;  i++)
    {
      double tmpDoub = *dPtr;
      if (tmpDoub < 1.0e-100  &&  tmpDoub > -1.0e-100)
        *iPtr = INT4_MAX;
      else
        {
          tmpDoub = vvv->dscale / tmpDoub;
          if      (tmpDoub > (double)INT4_MAX)
            *iPtr = INT4_MAX;
          else if (tmpDoub < (double)INT4_MIN)
            *iPtr = INT4_MIN;
          else
            *iPtr = (Int4)tmpDoub;
        }
      dPtr++;
      iPtr++;
    }

  dPtr = vvv->dC;
  iPtr = vvv->c;
  for (i = 0;  i < 3;  i++)
    *iPtr++ = (Int4)(*dPtr++);

  vvv->scale = (Int4)vvv->dscale;
}


/* Adjust viewer along the Z axis
 */
static Boolean FitZ(Viewer3D vvv)
{
  Spher3D sph;
  Int4    z_max;
  double  z_offset;

  if ( !GetSphere3D(vvv, &sph) )
    return FALSE; /* empty viewer picture */

  z_max = (Int4)((double)sph.radius * 2.0 / vvv->dscale) + 1;
  z_offset = (double)((z_max>>1) + 1) - 
    ((double)sph.x * vvv->dAOrt[2][0] +
     (double)sph.y * vvv->dAOrt[2][1] + 
     (double)sph.z * vvv->dAOrt[2][2]) / vvv->dscale;

  if ((Int4)z_offset <= (Int4)vvv->dC[2])
    return TRUE; /* picture fits the viewer(does not run out of Z=0 plane) */

  /* adjust */
  vvv->zmax  = z_max;
  vvv->dC[2] = z_offset;
  vvv->colorScale = (vvv->zmax<<8) / vvv->colorStep;
  MakeIntMat(vvv);
  SetUpContext(vvv);
  return TRUE;
}


static void DrawViewer3D(Viewer3D vvv)
{
  register Viewer3D       VVV = vvv;
  register Nlm_PPict3DPtr PPP = (Nlm_PPict3DPtr)(VVV ? VVV->pic : NULL);

  if (PPP == NULL)
    return;

  Nlm_stCon.image = LockPixMapImage ( VVV->image );
  if ( Nlm_stCon.image == NULL ) {
    Nlm_DiagPutRecord(DA_ERROR, viewerClass, "DrawViewer3D",
                      "Cannot lock memory block");
    return;
  }

  if (PPP->version != VVV->p_version  ||  VVV->c_version != VVV->c_versionPrev)
    {
      Uint1 i, k, colorStep;
      Uint2 j;
      Int4  red, green, blue;
      Int4  colorPic;
      Int4  brigh, kbrigh;

      VVV->colorOffset = 33;
      colorStep = (Uint1)((256 - VVV->colorOffset) / (PPP->totalColors + 1));
      if (colorStep > 32)
        colorStep = 32;
      VVV->colorStep = colorStep;
      i = (Uint1)VVV->colorOffset;
      colorPic = (Int4)colorPicAll[VVV->colorPicN];
      brigh = (Int4)brighAll[VVV->brighN];
      for (j = 0; j <= PPP->totalColors;  j++)
        {
          if (j != PPP->totalColors) {
            red   = PPP->colorR[j]; 
            green = PPP->colorG[j];
            blue  = PPP->colorB[j];
          }
          else {
            red   = VVV->colorHLR; 
            green = VVV->colorHLG;
            blue  = VVV->colorHLB;
          }

          red   = red   + 2 * (255 - red  ) / colorPic;
          green = green + 2 * (255 - green) / colorPic;
          blue  = blue  + 2 * (255 - blue ) / colorPic;
          for (k = 1;  k <= colorStep;  k++)
            {
              if (brigh >= 0)
                kbrigh = (256 - brigh) * k + brigh * colorStep;
              else
                kbrigh = (256 + brigh) * k;

              kbrigh /= (Int4)colorStep;
              SetColorImage(VVV->image, i++, (Uint1)((red   * kbrigh)>>8),
                                             (Uint1)((green * kbrigh)>>8),
                                             (Uint1)((blue  * kbrigh)>>8));
            }
        }
      SetColorImage(VVV->image, VVV->colorFon,
                    VVV->colorFonR, VVV->colorFonG, VVV->colorFonB);

      ImageSetPalette(VVV->image, VVV->parentWindow);
      VVV->c_versionPrev = VVV->c_version;

      if (VVV->p_version != PPP->version)
        {
          VVV->p_version = PPP->version;
          VVV->g_version++;
        }
    }

  if (VVV->g_version != VVV->g_versionPrev)
    {
      Uint2 iwidth, iheight;

      Nlm_stCon.zBuffer = (Uint2Ptr) HandLock( VVV->zBuffer );
      if ( Nlm_stCon.zBuffer == NULL ) {
        Nlm_DiagPutRecord(DA_ERROR, viewerClass, "DrawViewer3D",
                          "Can not lock memory block");
        return;
      }
      VVV->colorScale = (VVV->zmax<<8) / VVV->colorStep;
      GetImageSize(VVV->image, &iwidth, &iheight);
      MemSet((void*)Nlm_stCon.image,   VVV->colorFon,  iwidth*iheight );
      MemSet((void*)Nlm_stCon.zBuffer, 0, sizeof(Int2)*iwidth*iheight );
      if ( FitZ(VVV) ) {
        PPP->seg.base.fTable->draw( &PPP->seg );
      }
      ImageModified( VVV->image );
      VVV->g_versionPrev = VVV->g_version;
      HandUnlock( VVV->zBuffer );
    }

  {{
    RecT  r;
    PoinT p;
    ObjectRect(vvv->panel, &r);
#ifdef WIN_MOTIF
    p.x = 2;
    p.y = 2;
#else
    p.x = (Int2)(r.left + 2);
    p.y = (Int2)(r.top  + 2);
#endif
    ImageShow(VVV->image, p);
  }}

  UnlockPixMapImage( VVV->image );
}


static void DrawViewer3D_CB(PaneL panel)
{
  DrawViewer3D( PanelToViewer3D( panel ) );
}



static void WorldToScreen(Viewer3D vvv,
                          double  x,  double  y,  double  z,
                          double *xS, double *yS, double *zS)
{
  register Viewer3D VVV = vvv;

  *xS = VVV->dC[0] + (x * VVV->dAOrt[0][0] +
                      y * VVV->dAOrt[0][1] + 
                      z * VVV->dAOrt[0][2]) / VVV->dscale;
  *yS = VVV->dC[1] + (x * VVV->dAOrt[1][0] +
                      y * VVV->dAOrt[1][1] + 
                      z * VVV->dAOrt[1][2]) / VVV->dscale;
  *zS = VVV->dC[2] + (x * VVV->dAOrt[2][0] +
                      y * VVV->dAOrt[2][1] + 
                      z * VVV->dAOrt[2][2]) / VVV->dscale;
}


static void ScreenToWorld(Viewer3D vvv,
                          double  xS, double  yS, double  zS,
                          double *x,  double *y,  double *z)
{
  Uint4 i,j;
  Nlm_Matrix A  = Nlm_MatrixNew(3, 3);
  Nlm_Matrix sV = Nlm_MatrixNew(3, 1);
  Nlm_Matrix wV;

  Nlm_MatrixSetNode(sV, 0, 0, (xS - vvv->dC[0]) * vvv->dscale);
  Nlm_MatrixSetNode(sV, 1, 0, (yS - vvv->dC[1]) * vvv->dscale);
  Nlm_MatrixSetNode(sV, 2, 0, (zS - vvv->dC[2]) * vvv->dscale);

  for (i = 0;  i < 3;  i++)
  for (j = 0;  j < 3;  j++)
    Nlm_MatrixSetNode(A, i, j, vvv->dAOrt[i][j]);

  wV = Nlm_MatrixSolve(A, sV);

  *x = Nlm_MatrixNode(wV, 0, 0);
  *y = Nlm_MatrixNode(wV, 1, 0);
  *z = Nlm_MatrixNode(wV, 2, 0);

  Nlm_MatrixDelete( wV );
  Nlm_MatrixDelete( sV );
  Nlm_MatrixDelete( A );
}


static void MakeOffsets(Viewer3D vvv,
                        double xWorld,  double yWorld,  double zWorld,
                        double xScreen, double yScreen, double zScreen)
{
  vvv->dC[0] = xScreen - 
               (xWorld * vvv->dAOrt[0][0] +
                yWorld * vvv->dAOrt[0][1] + 
                zWorld * vvv->dAOrt[0][2]) / vvv->dscale;
  vvv->dC[1] = yScreen - 
               (xWorld * vvv->dAOrt[1][0] +
                yWorld * vvv->dAOrt[1][1] + 
                zWorld * vvv->dAOrt[1][2]) / vvv->dscale;
  vvv->dC[2] = zScreen - 
               (xWorld * vvv->dAOrt[2][0] +
                yWorld * vvv->dAOrt[2][1] + 
                zWorld * vvv->dAOrt[2][2]) / vvv->dscale;

  vvv->g_version++;
}


static void MakeFloatMat(Viewer3D vvv)
{
  Int2   i;
  double PNTR dPtr = &vvv->dAOrt[0][0];
  Int4   PNTR iPtr = &vvv->a[0][0];

  vvv->dscale = (double)vvv->scale;

  for (i = 0;  i < 9;  i++, dPtr++, iPtr++)
    *dPtr = (iPtr != 0) ? (vvv->dscale / (double)*iPtr) : (vvv->dscale * 4);

  dPtr = vvv->dC;
  iPtr = vvv->c;
  for (i = 0;  i < 3;  i++)
    *dPtr++ = (double)(*iPtr++);
}


static void ZoomAll(Viewer3D vvv, enumZoom3D method)
{
  Spher3D  sph;
  Int4     diametr;

  if ( !GetSphere3D(vvv, &sph) )
    return;

  diametr = sph.radius << 1;

  if (method == ZOOM_ALL)
    {
      vvv->dAOrt[0][0] = vvv->dAOrt[1][1] = vvv->dAOrt[2][2] = (double)1;
      vvv->dAOrt[0][1] = vvv->dAOrt[0][2] =
      vvv->dAOrt[1][0] = vvv->dAOrt[1][2] =
      vvv->dAOrt[2][0] = vvv->dAOrt[2][1] = (double)0;
    }

  if (method == ZOOM_KEEP_ALL  &&  !vvv->is_zoomed)
    method = ZOOM_KEEP_ANGLE;

  if (method != ZOOM_KEEP_ALL)
    {
      double scale = (double)diametr / (double)MIN(vvv->width, vvv->height);
      if (scale < 10000.0)
        scale = 10000.0;
      vvv->dscale = (double)scale *
        (double)vvv->perspect /  ((double)vvv->perspect + 0.5);
      vvv->is_zoomed = FALSE;
    }

  vvv->zmax = (Int4)((double)diametr / vvv->dscale) + 1;

  if (method != ZOOM_KEEP_ALL)
    {
      MakeOffsets(vvv, (double)sph.x, (double)sph.y, (double)sph.z,
                  (double)(vvv->width>>1),
                  (double)(vvv->height>>1),
                  (double)((vvv->zmax>>1) + 1));
    }
  else
    {
      vvv->dC[2] = (double)((vvv->zmax>>1) + 1) - 
        ((double)sph.x * vvv->dAOrt[2][0] +
         (double)sph.y * vvv->dAOrt[2][1] + 
         (double)sph.z * vvv->dAOrt[2][2]) / vvv->dscale;
      vvv->g_version++;
    }

  MakeIntMat( vvv );
}


static void Move3D(Viewer3D vvv, Int2 dx, Int2 dy)
{
  vvv->dC[0] += (double)dx;
  vvv->dC[1] -= (double)dy;

  MakeIntMat( vvv );
  vvv->g_version++;
}


static void Zoom3D(Viewer3D vvv, Int2 x1, Int2 y1, Int2 x2, Int2 y2)
{
  Spher3D  sph;
  double   scale;
  int      width, height;

  if ( !GetSphere3D(vvv, &sph) )
    return;

  {{
    RecT r;
    ObjectRect(vvv->panel, &r);
    x1 -= r.left; x2 -= r.left;
    y1 -= r.top;  y2 -= r.top;
    width = x2 - x1;
    if (width < 0)
      { 
        width = -width;
        x1 = x2;
      }
    y1 = (Int2)(vvv->height - y1);
    y2 = (Int2)(vvv->height - y2);
    height = y2 - y1;
    if (height < 0)
      { 
        height = -height;
        y1 = y2;
      }
  }}

  {{
    double minscale = (double)sph.radius / 32000.0 / vvv->dscale;
    double d = (double)height / (double)vvv->height;
    scale    = (double)width  / (double)vvv->width;
    if (d > scale)
      scale = d;
    if (scale < minscale)
      scale = minscale;
  }}

  vvv->dC[0] = (vvv->dC[0] - (double)(x1+width /2)) / scale + vvv->width / 2;
  vvv->dC[1] = (vvv->dC[1] - (double)(y1+height/2)) / scale + vvv->height/ 2;
  vvv->dscale *= scale;
  vvv->zmax = (Int4)((double)sph.radius * 2.0 / vvv->dscale) + 1;
  vvv->dC[2] = (double)((vvv->zmax>>1) + 1) - 
    ((double)sph.x * vvv->dAOrt[2][0] +
     (double)sph.y * vvv->dAOrt[2][1] + 
     (double)sph.z * vvv->dAOrt[2][2]) / vvv->dscale;

  MakeIntMat( vvv );
  vvv->g_version++;
  vvv->is_zoomed = TRUE;
}


/***********************************************************************
 *  Generic ROTATION functions
 */

static void Rotate(Viewer3D vvv, Int4 dAngle, enumRotate3D pivot)
{
  double   dx, dy, dz;
  double   xx, yy, zz;
  double   d;
  double   ssin;
  double   ccos;
  double   a[3][3];
  Spher3D  sph;
  Int2     i,j;
  
  if ( !dAngle )
    return;

  /* store viewport position */
  if ( !GetSphere3D(vvv, &sph) )
    return;

  WorldToScreen(vvv, (double)sph.x, (double)sph.y, (double)sph.z, 
                     &dx, &dy, &dz);

  if (pivot == ROTATE_Z)
    { /* rotate around the center of the visible area */
      dx = vvv->width  / 2;
      dy = vvv->height / 2;
    }
    
  ScreenToWorld(vvv, dx, dy, dz, &xx, &yy, &zz); 


  /* compose rotation matrix */
  d = (double)dAngle * NCBIMATH_PI / 180.0;
  ssin = sin( d );
  ccos = cos( d );

  for (i = 0;  i < 3; i++)
  for (j = 0;  j < 3; j++)
    a[i][j] = 0.0;

  switch ( pivot )
    {
    case ROTATE_X:
      a[0][0] = 1.0;
      a[1][1] =  ccos;  a[1][2] = ssin;
      a[2][1] = -ssin;  a[2][2] = ccos;
      break;
    case ROTATE_Y:
      a[0][0] =  ccos;  a[0][2] = ssin;
      a[1][1] = 1.0;
      a[2][0] = -ssin;  a[2][2] = ccos;
      break;
    case ROTATE_Z:
      a[0][0] =  ccos;  a[0][1] = ssin;
      a[1][0] = -ssin;  a[1][1] = ccos;
      a[2][2] = 1.0;
      break;
    }

  {{ /* rotate */
    double b[3][3];
    for (j = 0;  j < 3;  j++)
    for (i = 0;  i < 3;  i++)
      {
        b[i][j] =
          a[i][0] * vvv->dAOrt[0][j] + 
          a[i][1] * vvv->dAOrt[1][j] + 
          a[i][2] * vvv->dAOrt[2][j];
      }
    MemCpy((VoidPtr)vvv->dAOrt, (VoidPtr)b, sizeof(vvv->dAOrt));
  }}

  /* restore viewport position */
  MakeOffsets(vvv, xx, yy, zz, dx, dy, dz);

  MakeIntMat( vvv );
  vvv->g_version++;
}


static void ViewerRotate(SlatE panel, Int4 delta,
                         enumRotate3D pivot,
                         Boolean adjust_scrollbar, Boolean redraw)
{
  Viewer3D vvv;
  if (panel == NULL  ||
      !Visible(panel)  ||  !AllParentsVisible(panel))
    return;

  vvv = PanelToViewer3D( (PaneL)panel );

  if ( adjust_scrollbar )
    { /* adjust the relevant rotation scrollbar, if any */
      BaR sbar = NULL;
      switch ( pivot )
        {
        case ROTATE_X:
          sbar = GetSlateVScrollBar( panel );
          break;
        case ROTATE_Y:
          sbar = GetSlateHScrollBar( panel );
          break;
        case ROTATE_Z:
          sbar = vvv->z_rotate;
          break;
        }

      if ( sbar )
        {
          ResetClip();
          CorrectBarValue(sbar, (GetValue(sbar) + delta + 360) % 360);
        }
    }

  /* the coordination transformation (rotation) */
  if (pivot == ROTATE_X)
    delta = -delta;
  Rotate(vvv, delta, pivot);

  if ( redraw )
    { /* Draw the viewer */
      WindoW   tempPort = CurrentWindow();
      UseWindow( ParentWindow(panel) );
      Select( panel );
      DrawViewer3D( vvv );
      UseWindow( tempPort );
    }

  Nlm_DiagReset();
}


static void ResetViewerProc_CB(PaneL panel)
{
  Viewer3D vvv = PanelToViewer3D( panel );
  if ( !vvv  )
    return;

  vvv->panel = NULL;
  DeleteViewer3D( PanelToViewer3D( panel ) );
}


static void ResetImage3D(Viewer3D vvv)
{
  vvv->width = vvv->height = 0;

  if ( vvv->image ) {
      DeleteImage( vvv->image );
      vvv->image = NULL;
    }

  if ( vvv->zBuffer ) {
    HandFree( vvv->zBuffer );
    vvv->zBuffer = NULL;
  }
}



/***********************************************************************
 *  SCROLLBARS
 */


static void ViewerVScrollProc(BaR sb, SlatE viewer, Int2 newval, Int2 oldval)
{
  ViewerRotate(viewer, newval - oldval, ROTATE_X, FALSE, TRUE);
}

static void ViewerHScrollProc(BaR sb, SlatE viewer, Int2 newval, Int2 oldval)
{
  ViewerRotate(viewer, newval - oldval, ROTATE_Y, FALSE, TRUE);
}

static void ViewerZScrollProc(BaR sb, GraphiC group, Int2 newval, Int2 oldval)
{
  SlatE viewer = (SlatE)GetObjectExtra( sb );
  ViewerRotate(viewer, newval - oldval, ROTATE_Z, FALSE, TRUE);
}


/***********************************************************************
 * PRIMITIVEs
 */

static Boolean Nlm_AddFoundPrim(Viewer3D vvv, Prim3D primId)
{
  Uint2 i;

  if (primId == 0)
    return TRUE;

  for (i = 0;  i < vvv->nPrimFound;  i++)
    if (vvv->foundPrim[i] == primId)
      return TRUE;

  if (vvv->nPrimFound >= MAX_FOUND_PRIM)
    return FALSE;

  vvv->foundPrim[vvv->nPrimFound++] = primId;
  return TRUE;
}


static void Nlm_RedrawPrim3D(Viewer3D vvv, Nlm_Base3DPtr pBase)
{
  if (vvv->pic == NULL)
    return;

  Nlm_stCon.image = LockPixMapImage( vvv->image );
  if (Nlm_stCon.image == NULL) {
    Nlm_DiagPutRecord(DA_ERROR, viewerClass, "RedrawPrim3D",
                      "Cannot lock image memory block");
    return;
  }
  Nlm_stCon.zBuffer = (Uint2Ptr) HandLock( vvv->zBuffer );
  if (Nlm_stCon.zBuffer == NULL) {
    UnlockPixMapImage( vvv->image );
    Nlm_DiagPutRecord(DA_ERROR, viewerClass, "RedrawPrim3D",
                      "Cannot lock Z-buffer memory block" );
    return;
  }

  vvv->colorScale = (vvv->zmax<<8) / vvv->colorStep;
  SetUpContext( vvv );
  if (pBase->status & HL_MASK)
    {
      Uint1 color = pBase->color;
      pBase->color = Nlm_stCon.colorHL;
      pBase->fTable->draw( (Nlm_VoidPtr)pBase );
      pBase->color = color;
    }
  else
    {
      pBase->fTable->draw( (Nlm_VoidPtr)pBase );
    }

  ImageModified( vvv->image );
  HandUnlock( vvv->zBuffer );
  UnlockPixMapImage( vvv->image );
}


static void Nlm_ResetHLSeg3D(Segment3D segment)
{
  register Nlm_Base3DPtr prim = (Nlm_Base3DPtr)segment;

  prim->status &= ~HL_MASK;
  for (prim = ((Nlm_PSeg3DPtr)prim)->first;  prim;  prim = prim->next) {
    if (prim->fTable->code == 0)
      Nlm_ResetHLSeg3D((Segment3D)prim);
    else
      prim->status &= ~HL_MASK;
  }
}


/***********************************************************************
 *  LAYERing
 */

static void Nlm_AllLayerSetStates3D(Viewer3D vvv, Nlm_Boolean state)
{
  register Uint1 level = 0;
  do
    {
      SetLayer3D(vvv->pic, level, state);
      vvv->layerTable[level] = state;
    }
  while (level++ != 255);
}


NLM_EXTERN void Nlm_AllLayerSet3D(Viewer3D vvv, Nlm_Boolean state)
{
  Spher3D  sph;
  double   dx, dy, dz;
  
  if (vvv == NULL  ||  vvv->pic == NULL)
    return;

  Nlm_AllLayerSetStates3D(vvv, state);

  if ( GetSphere3D(vvv, &sph) )
    {
      WorldToScreen (vvv,
                     (double)sph.x, (double)sph.y, (double)sph.z,
                     &dx, &dy, &dz );
      MakeOffsets   (vvv,
                     (double)sph.x, (double)sph.y, (double)sph.z,
                     dx,  dy,  dz );
      MakeIntMat( vvv ); 
    }

  RedrawViewer3D( vvv );
}


static void Nlm_RewindLayer3D(Viewer3D vvv)
{
  if (vvv == NULL  ||  vvv->pic == NULL)
    return;

  Nlm_AllLayerSetStates3D(vvv, FALSE);
  
  SetLayer3D(vvv->pic, 1, TRUE); 
  vvv->layerTable[0] = TRUE; 
  currlayer = 0;
  RedrawViewer3D( vvv );
}


extern void Nlm_PrevLayer3D (Viewer3D vvv)
{
  if (vvv == NULL  ||  vvv->pic == NULL)
    return;

  Nlm_AllLayerSetStates3D(vvv, FALSE);

  currlayer = (Uint1)((currlayer != 0) ? (currlayer - 1) : toplayer);
  SetLayer3D(vvv->pic, currlayer, TRUE); 
  vvv->layerTable[currlayer] = TRUE; 

  RedrawViewer3D( vvv );
}


extern void Nlm_NextLayer3D(Viewer3D vvv)
{
  if (vvv == NULL  ||  vvv->pic == NULL)
    return;

  Nlm_AllLayerSetStates3D(vvv, FALSE);

  currlayer = (Uint1)((currlayer != toplayer) ? (currlayer + 1) : 0);
  SetLayer3D(vvv->pic, currlayer, TRUE); 
  vvv->layerTable[currlayer] = TRUE; 

  RedrawViewer3D( vvv );
}


extern Boolean Nlm_IsPlaying3D(void)
{
#ifdef WIN_MAC
   return FALSE;
#else
   return (Boolean)(V3D_bPLAY  &&  GetStatus( V3D_bPLAY ));
#endif
}


extern void  Nlm_StopPlaying3D(void) 
{
  if ( V3D_bPLAY )
    SetStatus(V3D_bPLAY, FALSE);
}


extern void Nlm_StartPlaying3D(Viewer3D vvv)
{
#ifndef WIN_MAC
  if ( V3D_bPLAY )
    {
      SetStatus(V3D_bPLAY, TRUE);
      Nlm_PlayLayer3D( vvv );
    }
#endif
}


extern void Nlm_PlayLayer3D(Viewer3D vvv)
{
#ifndef WIN_MAC
  if (vvv == NULL  ||  vvv->pic == NULL)
    return;

  Nlm_AllLayerSetStates3D(vvv, FALSE);
  
  SetLayer3D(vvv->pic, currlayer, TRUE);  
  vvv->layerTable[currlayer] = TRUE; 
  while (!QuittingProgram()  &&  IsPlaying3D())
    {
      SetLayer3D(vvv->pic, currlayer, FALSE); 
      vvv->layerTable[currlayer] = FALSE;  

      currlayer = (Uint1)((currlayer != toplayer) ? (currlayer + 1) : 0);
      SetLayer3D(vvv->pic, currlayer, TRUE); 
      vvv->layerTable[currlayer] = TRUE; 

      RedrawViewer3D( vvv );
      while ( Nlm_EventAvail() )
        Nlm_ProcessAnEvent();
    }
#endif
}

       	
extern void Nlm_SetLayerTop3D(Uint1 top)
{
  toplayer = top;
}


extern Uint1 Nlm_GetLayerTop3D(void)
{
  return toplayer;
}	
	



/***********************************************************************
 *  VIEWER CONTROLS
 ***********************************************************************/

/* Callbacks
 */

/* Animation callbacks */

static void AllLayerOnProc( ButtoN b )
{
  Nlm_AllLayerSet3D((Viewer3D)GetObjectExtra(b), TRUE);
}

static void RewindLayerProc(ButtoN b)
{
  Nlm_RewindLayer3D( (Viewer3D)GetObjectExtra( b ) );
}

static void PrevLayerProc(ButtoN b)
{
  Nlm_PrevLayer3D( (Viewer3D)GetObjectExtra( b ) );
}

static void NextLayerProc(ButtoN b)
{
  Nlm_PrevLayer3D( (Viewer3D)GetObjectExtra( b ) );
}

static void PlayLayerProc(ButtoN b)
{
  Nlm_PlayLayer3D( (Viewer3D)GetObjectExtra( b ) );
}


/* Navigation callbacks */

static void UnzoomProc(ButtoN b)
{
  Nlm_ZoomAll3D( (Viewer3D)GetObjectExtra( b ) );
}

static void MoveProc(ButtoN b) 
{
  SetStdMouse3D((Viewer3D)GetObjectExtra( b ), Mouse3D_Move);
  MsgAlert(KEY_OK, SEV_INFO, "Cn3D Online Manual",
           "Use the [Shift] modifier key to move with mouse");
}

static void ZoomProc(ButtoN b) 
{
  SetStdMouse3D((Viewer3D)GetObjectExtra( b ), Mouse3D_Zoom);
  MsgAlert(KEY_OK, SEV_INFO, "Cn3D Online Manual",
           "Use the [Control] modifier key to zoom with mouse");
}


/* Adjustment callbacks */

static void AdjustProc(GrouP g)
{
  Nlm_SetAdjust3D((Viewer3D)GetObjectExtra(g), (Uint1)GetValue(g));
}

static void PlusProc(ButtoN b)
{
  Nlm_Adjust3D((Viewer3D)GetObjectExtra( b ), TRUE);
}

static void MinusProc(ButtoN b)
{
  Nlm_Adjust3D((Viewer3D)GetObjectExtra( b ), FALSE);
}


/* Create and place the viewer controls
 */

static GrouP BasicControls3D(GrouP      prnt,
                             Boolean    vertical,
                             Controls3D controls)
{
  GrouP mGroup;
  GrouP tmpGroup;
  GrouP tmptmpGroup;

  Nlm_DiagReset();

  if ( vertical )
    {
      mGroup = HiddenGroup(prnt, 1, 0, NULL);
#ifdef WIN_MOTIF
      SetGroupMargins(mGroup, 0, 0);
#else
      SetGroupMargins(mGroup, 1, 1);
#endif
      SetGroupSpacing(mGroup, 0, 0);

      StaticPrompt(mGroup, "Zoom:", 0, 0, Nlm_systemFont, 'c');
      controls->zoomAll = PushButton(mGroup, "All",  UnzoomProc);
      controls->Move3D  = PushButton(mGroup, "Move", MoveProc  );
      controls->Zoom3D  = PushButton(mGroup, "Zoom", ZoomProc  );
      StaticPrompt(mGroup, "------", 0, 0, Nlm_systemFont, 'c');

      controls->adjustButtons = tmpGroup =
        HiddenGroup(mGroup, 1, 3, AdjustProc);
      RadioButton(tmpGroup, "Persp");
      RadioButton(tmpGroup, "Brigh");
      RadioButton(tmpGroup, "Color");
      SetValue(tmpGroup, 1);

      tmpGroup = HiddenGroup(mGroup, 2, 1, NULL);
      controls->minusButton = PushButton(tmpGroup, "-",  MinusProc);
      controls->plusButton  = PushButton(tmpGroup, "+",  PlusProc );
      StaticPrompt(mGroup, "------", 0, 0, Nlm_systemFont, 'c');
    }
  else
    {  /* horizontal*/
      /* rearranged to use a single horizontal group */
      mGroup = HiddenGroup(prnt, 0, 1, NULL);
      SetGroupSpacing(mGroup, 6, 1);
   
      tmpGroup = HiddenGroup(mGroup, 3, 0, NULL);
      SetGroupSpacing(tmpGroup, 1, 0);
      controls->minusButton = PushButton(tmpGroup, "-", MinusProc);
      controls->adjustButtons = tmptmpGroup = 
        HiddenGroup(tmpGroup, 3, 0, AdjustProc);
      RadioButton(tmptmpGroup, "p");
      RadioButton(tmptmpGroup, "b");
      RadioButton(tmptmpGroup, "c");
      SetValue(tmptmpGroup, 1);      
      controls->plusButton = PushButton(tmpGroup, "+", PlusProc);
      
      tmpGroup = HiddenGroup(mGroup, 4, 0, NULL);
      controls->zoomAll = PushButton(tmpGroup, "All",  UnzoomProc);
      controls->Move3D  = PushButton(tmpGroup, "Move", MoveProc  );
      controls->Zoom3D  = PushButton(tmpGroup, "Zoom", ZoomProc  );
    }
  
  return mGroup;
}


static void FreeControls(GraphiC group, VoidPtr controls)
{
  MemFree( controls );
}


extern Controls3D Nlm_CreateControls3D(GrouP prnt,
                                       Boolean vertical,
                                       Boolean anim_ctrl,
                                       GrouP  *ctrl_group)
{
  Controls3D controls = (Controls3D)MemNew( sizeof(structControls3D) );
  GrouP      mGroup   = BasicControls3D(prnt, vertical, controls);

  controls->group = mGroup;
  SetObjectExtra(controls->group, controls, FreeControls);

  if ( anim_ctrl )
    {
      GrouP tmpGroup = mGroup;
      if ( vertical )
        {
          StaticPrompt(mGroup, "Animate:", 0, 0, Nlm_systemFont, 'r');
          tmpGroup = HiddenGroup(mGroup, 4, 1, NULL);
        }

      controls->allButton    = PushButton(tmpGroup, "*",  AllLayerOnProc);
      controls->rewindButton = PushButton(tmpGroup, "|<", RewindLayerProc);
      controls->prevButton   = PushButton(tmpGroup, "<",  PrevLayerProc);
      controls->nextButton   = PushButton(tmpGroup, ">",  NextLayerProc);
#ifndef WIN_MAC
      V3D_bPLAY = controls->playButton = CheckBox(mGroup, "Go", PlayLayerProc);
#endif
    }

  if ( ctrl_group )
    *ctrl_group = mGroup;
  return controls;
}


extern void Nlm_DeleteControls3D(Controls3D controls)
{
  Remove( controls->group );
}


/* Link the viewer controls to a viewer
 */

static void Nlm_LinkBasicControls3D(Controls3D controls, Viewer3D vvv)
{
  SetObjectExtra(controls->zoomAll,       vvv, NULL);
  SetObjectExtra(controls->Move3D,        vvv, NULL);
  SetObjectExtra(controls->Zoom3D,        vvv, NULL);
  SetObjectExtra(controls->adjustButtons, vvv, NULL);
  SetObjectExtra(controls->plusButton,    vvv, NULL);
  SetObjectExtra(controls->minusButton,   vvv, NULL);
}

static void Cleanup_V3D_bPLAY(GraphiC playButton, VoidPtr vvv)
{
  if ((ButtoN)playButton == V3D_bPLAY)
    V3D_bPLAY = NULL;
}


extern void Nlm_LinkControls3D(Controls3D controls, Viewer3D vvv)
{
   if (controls == NULL)
     return;

   Nlm_LinkBasicControls3D(controls, vvv);
   if ( !controls->allButton )
     return;

   SetObjectExtra(controls->allButton,    vvv, NULL);
   SetObjectExtra(controls->rewindButton, vvv, NULL);
   SetObjectExtra(controls->prevButton,   vvv, NULL);
   SetObjectExtra(controls->nextButton,   vvv, NULL);
#ifndef WIN_MAC
   SetObjectExtra(controls->playButton,   vvv, Cleanup_V3D_bPLAY);
#endif    
}


/* ADJUSTMENT (color, brightness, perspective)
 */

extern void Nlm_Adjust3D(Viewer3D vvv, Boolean plus)
{
  Boolean needRedraw = FALSE;

  switch ( vvv->adjust )
    {
    case ADJUST3D_PERSPECT:
      if ( plus ) {
        if (vvv->perspectN < 5) {
          vvv->perspectN++;
          needRedraw = TRUE;
        }
      } else {
        if (vvv->perspectN > 0) {
          vvv->perspectN--;
          needRedraw = TRUE;
        }
      }
      if ( needRedraw )
        {
          Spher3D sph;
          double dx,dy,dz;

          vvv->g_version++;
          if ( !GetSphere3D(vvv, &sph) )
            break;

          WorldToScreen(vvv, (double)sph.x, (double)sph.y, (double)sph.z, 
                        &dx, &dy, &dz);
          vvv->dscale = (vvv->dscale * (2.0 * (double)vvv->perspect + 1.0)) /
            (2.0 * (double)vvv->perspect);
          vvv->perspect = perspectAll[vvv->perspectN];
          vvv->dscale = (vvv->dscale * 2.0 * (double)vvv->perspect) / 
            (2.0 * (double)vvv->perspect + 1.0);
          vvv->zmax = (Int4)((double)(sph.radius<<1) / vvv->dscale) + 1;
          MakeOffsets(vvv, (double)sph.x, (double)sph.y, (double)sph.z,
                      dx, dy, (double)((vvv->zmax>>1) + 1));
          MakeIntMat( vvv );
        }
      break;

    case ADJUST3D_BRIGH:
      if ( plus ) {
        if ( vvv->brighN < 6 ) {
          vvv->brighN++; vvv->c_version++;
          needRedraw = TRUE;
        }
      } else {
        if ( vvv->brighN > 0 ) {
          vvv->brighN--; vvv->c_version++;
          needRedraw = TRUE;
        }
      }
      break;

    case ADJUST3D_COLOR:
      if ( plus ) {
        if ( vvv->colorPicN < 5 ) {
          vvv->colorPicN++; vvv->c_version++;
          needRedraw = TRUE;
        }
      } else {
        if ( vvv->colorPicN > 0 ) {
          vvv->colorPicN--; vvv->c_version++;
          needRedraw = TRUE;
        }
      }
  }

  if ( needRedraw )
    {
      WindoW tmpPort = SavePort( vvv->panel );
      Select( vvv->panel );
      DrawViewer3D ( vvv );
      RestorePort( tmpPort );
      Nlm_DiagReset();
    }
}


/***********************************************************************
 *  MOUSE EVENT HANDLERS
 ***********************************************************************/

/* MOVE
 */

static void Move_DrawTrace(MA_TracePtr trace)
{
  if ( EqualPt(trace->start, trace->end) )
    return;

#ifdef WIN_MOTIF
  SetColor( 0xf1 );
#endif
  InvertMode();
  DrawLine(trace->start, trace->end);
  CopyMode();
}

static void Move_PressMA(MAPtr ma,
                         MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  trace->start = trace->end = point;
}

static void Move_DragMA(MAPtr ma,
                        MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  Move_DrawTrace( trace );
  trace->end = point;
  Move_DrawTrace( trace );
}

static void Move_ReleaseMA(MAPtr ma,
                           MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  Move_DrawTrace( trace );
  trace->end = point;

  if ( EqualPt(trace->start, trace->end) )
    return;

  {{
    Viewer3D vvv = MAToViewer3D( ma );
    Move3D(vvv, (Int2)(trace->end.x - trace->start.x),
                (Int2)(trace->end.y - trace->start.y));
    DrawViewer3D( vvv );
  }}

  trace->start = trace->end;
}

static void Move_CancelMA(MAPtr ma,
                          MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  Move_DrawTrace( trace );
}


/* ZOOM
 */

static void Zoom_DrawTrace(MA_TracePtr trace)
{
  RecT rubber_box;
  if ( EqualPt(trace->start, trace->end) )
    return;

#ifdef WIN_MOTIF
  SetColor( 0xf1 );
#endif
  InvertMode();
  LoadRect(&rubber_box, trace->start.x, trace->start.y,
                        trace->end.x,   trace->end.y);
  FrameRect( &rubber_box );
  CopyMode();
}

static void Zoom_PressMA(MAPtr ma,
                         MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  trace->start = trace->end = point;
}

static void Zoom_DragMA(MAPtr ma,
                        MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  Zoom_DrawTrace( trace );
  trace->end = point;
  Zoom_DrawTrace( trace );
}

static void Zoom_ReleaseMA(MAPtr ma,
                           MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  Zoom_DrawTrace( trace );
  trace->end = point;

  if ( EqualPt(trace->start, trace->end) )
    return;

  {{
    Viewer3D vvv = MAToViewer3D( ma );
    Zoom3D(vvv, trace->start.x, trace->start.y,
                trace->end.x,   trace->end.y);
    DrawViewer3D( vvv );
  }}
}

static void Zoom_CancelMA(MAPtr ma,
                          MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  Zoom_DrawTrace( trace );
}


/* ROTATE
 */

typedef struct
{
  enumRotate3D H; /* horizontal dragging */
  enumRotate3D V; /* vertical   dragging */
}
RotatePivots3D, PNTR RotatePivots3DPtr;

static void Rotate_PressMA(MAPtr ma,
                           MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  trace->start = point;
}

static void Rotate_DragMA(MAPtr ma,
                          MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  Viewer3D vvv = MAToViewer3D( ma );
  RotatePivots3DPtr pivot = (RotatePivots3DPtr)extra;

  if ( EqualPt(trace->start, point) )
    return;

  ViewerRotate((SlatE)vvv->panel,
               (Int4)((180.0 * (point.x - trace->start.x)) / vvv->width),
               pivot->H, TRUE, FALSE);

  ViewerRotate((SlatE)vvv->panel,
               (Int4)((180.0 * (point.y - trace->start.y)) / vvv->height),
               pivot->V, TRUE, TRUE);

  trace->start = point;
}



/* MISC
 */

static void Bg_HL_DClickMA(MAPtr ma,
                           MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  Viewer3D vvv = MAToViewer3D( ma );
  Uint2    n_prim = FindPrim3D(vvv, point);

  if ( n_prim )
    { /* Highlight primitive */
      Prim3D prim = GetFoundPrim3D(vvv, 0);
      SetHLColor3D(vvv, 0, 255, 0);
      HighlightPrim3D(vvv, prim, (Boolean)(!IsPrim3DHlighted(vvv, prim)));
      RedrawViewer3D( vvv );
    }
  else
    { /* Choose and change viewer background color */
      if ( ChooseColorDialog(&vvv->colorFonR,
                             &vvv->colorFonG,
                             &vvv->colorFonB,
                             0) )
        Nlm_SetBackground3D(vvv,
                            vvv->colorFonR,
                            vvv->colorFonG,
                            vvv->colorFonB);
    }
}


/* RESET
 */

static void ResetMA(MAPtr ma,
                    MA_TracePtr trace, PoinT point, VoidPtr extra)
{
  VERIFY ( MA_UnsetAll( ma ) );
}



/* Initialize MA for the viewer
 */

static Boolean InitializeMA(Viewer3D vvv)
{
  MAPtr ma = vvv->ma; 

  /* rotate */
  MActionPtr rotate_press   =
    MA_AddAction(ma, MK_Normal, MA_Press,   Rotate_PressMA, NULL, NULL);

  static RotatePivots3D RotateDrag_YX = { ROTATE_Y, ROTATE_X };
  MActionPtr rotate_drag_YX =
    MA_AddAction(ma, MK_Normal, MA_Drag, Rotate_DragMA, &RotateDrag_YX, NULL);
  MA_GroupPtr rotate_group_YX = MA_AddGroup(ma, "Rotate_YX",
                                            rotate_press,   MA_ONLY,
                                            rotate_drag_YX, MA_ONLY,
                                            NULL);

  static RotatePivots3D RotateDrag_ZX = { ROTATE_Z, ROTATE_X };
  MActionPtr rotate_drag_ZX =
    MA_AddAction(ma, MK_Normal, MA_Drag, Rotate_DragMA, &RotateDrag_ZX, NULL);
  MA_GroupPtr rotate_group_ZX = MA_AddGroup(ma, "Rotate_ZX",
                                            rotate_press,   MA_ONLY,
                                            rotate_drag_ZX, MA_ONLY,
                                            NULL);

  static RotatePivots3D RotateDrag_YZ = { ROTATE_Y, ROTATE_Z };
  MActionPtr rotate_drag_YZ =
    MA_AddAction(ma, MK_Normal, MA_Drag, Rotate_DragMA, &RotateDrag_YZ, NULL);
  MA_GroupPtr rotate_group_YZ = MA_AddGroup(ma, "Rotate_YZ",
                                            rotate_press,   MA_ONLY,
                                            rotate_drag_YZ, MA_ONLY,
                                            NULL);

  /* move */
  MActionPtr move_press   =
    MA_AddAction(ma, MK_Shift, MA_Press,   Move_PressMA,   NULL, NULL);
  MActionPtr move_drag    =
    MA_AddAction(ma, MK_Shift, MA_Drag,    Move_DragMA,    NULL, NULL);
  MActionPtr move_release =
    MA_AddAction(ma, MK_Shift, MA_Release, Move_ReleaseMA, NULL, NULL);
  MActionPtr move_cancel  =
    MA_AddAction(ma, MK_Shift, MA_Cancel,  Move_CancelMA,  NULL, NULL);

  MA_GroupPtr move_group = MA_AddGroup(ma, "Move",
                                       move_press,   MA_ONLY,
                                       move_drag,    MA_ONLY,
                                       move_release, MA_ONLY,
                                       move_cancel,  MA_ONLY,
                                       NULL);

  /* zoom */
  MActionPtr zoom_press   =
    MA_AddAction(ma, MK_Ctrl, MA_Press,   Zoom_PressMA,   NULL, NULL);
  MActionPtr zoom_drag    =
    MA_AddAction(ma, MK_Ctrl, MA_Drag,    Zoom_DragMA,    NULL, NULL);
  MActionPtr zoom_release =
    MA_AddAction(ma, MK_Ctrl, MA_Release, Zoom_ReleaseMA, NULL, NULL);
  MActionPtr zoom_cancel  =
    MA_AddAction(ma, MK_Ctrl, MA_Cancel,  Zoom_CancelMA,  NULL, NULL);

  MA_GroupPtr zoom_group = MA_AddGroup(ma, "Zoom",
                                       zoom_press,   MA_ONLY,
                                       zoom_drag,    MA_ONLY,
                                       zoom_release, MA_ONLY,
                                       zoom_cancel,  MA_ONLY,
                                       NULL);

  /* miscellaneous actions */
  MActionPtr bg_hl_dclick =
    MA_AddAction(ma, MK_Normal, MA_DClick,  Bg_HL_DClickMA, NULL,
                 "Highlight-Prim or Background");

  /* this group disables all mouse actions when set */
  MActionPtr reset_init =
    MA_AddAction(ma, MK_Normal, MA_Init,  ResetMA, NULL, NULL);

  MA_GroupPtr reset_group = MA_AddGroup(ma, "No Action",
                                        reset_init, MA_SHARED,
                                        NULL);


  {{ /* "No-Action"s */
    int i, j;
    for (i = 0;  i < MK_Default;  i++)
    for (j = 0;  j < MA_Init;     j++)
      {
        VERIFY ( MA_AddAction(ma, (enumMKey)i, (enumMAction)j,
                              DoNothingMA,   NULL, "No Action") );
      }
  }}


  /* register the set of standard 3D-viewer groups */
  vvv->ma_std_group[Mouse3D_DoNothing] = reset_group;
  vvv->ma_std_group[Mouse3D_RotateYX ] = rotate_group_YX;
  vvv->ma_std_group[Mouse3D_RotateZX ] = rotate_group_ZX;
  vvv->ma_std_group[Mouse3D_RotateYZ ] = rotate_group_YZ;
  vvv->ma_std_group[Mouse3D_Move     ] = move_group;
  vvv->ma_std_group[Mouse3D_Zoom     ] = zoom_group;


  /* Test, Setup defaults and Link viewer panel to MA */
  if (!rotate_press  ||
      !rotate_drag_YX  ||  !rotate_group_YX  ||
      !rotate_drag_ZX  ||  !rotate_group_ZX  ||
      !rotate_drag_YZ  ||  !rotate_group_YZ  ||
      !move_press      ||  !move_drag        ||  !move_release  ||
      !move_cancel     ||  !move_group       ||
      !zoom_press      ||  !zoom_drag        ||  !zoom_release  ||
      !zoom_cancel     ||  !zoom_group       ||
      !bg_hl_dclick    ||
      !reset_group     ||

      !SetStdMouse3D(vvv, Mouse3D_RotateYX)  ||
      !SetStdMouse3D(vvv, Mouse3D_Move    )  ||
      !SetStdMouse3D(vvv, Mouse3D_Zoom    )  ||
      !MA_SetAction(bg_hl_dclick, FALSE)  ||
      !MA_LinkPanel(ma, vvv->panel))
    {
      MA_Reset( ma );
      return FALSE;
    }

  return TRUE;
}



/*****************************************************************************
*
*   EXTERNAL FUNCTIONS
*
*****************************************************************************/

/*
 * Viewer data access
 */

extern Viewer3D MAToViewer3D(MAPtr ma)
{
  return (Viewer3D)MA_GetExtra( ma );
}


extern Viewer3D PanelToViewer3D(PaneL panel)
{
  MAPtr ma;
  GetPanelExtra(panel, &ma);

  return MAToViewer3D( ma );
}


extern void Nlm_SetAdjust3D(Viewer3D vvv, Uint1 ajust)
{
  if ( vvv )
    vvv->adjust = ajust;
}


extern void Nlm_SetHLColor3D(Viewer3D vvv, Uint1 red, Uint1 green, Uint1 blue )
{
  Nlm_DiagReset();
  if (vvv == NULL)
    return;

  vvv->colorHLR = red;
  vvv->colorHLG = green;
  vvv->colorHLB = blue;
  vvv->c_version++;
}


extern Nlm_Image Nlm_GetViewerImage3D(Viewer3D vvv)
{
  return vvv->image;
}


extern void Nlm_GetViewerInfo3D(Viewer3D vvv, Picture3D PNTR pic,
                                Nlm_Camera3DPtr camera, RectPtr rectangle)
{
  PCamera3DUnion camUnn;

  Nlm_DiagReset();
  if (pic != NULL)
    *pic = NULL;

  if (vvv == NULL)
    return;

  if (pic != NULL)
    *pic = vvv->pic;

  if (camera != NULL  &&  vvv->pic != NULL)
    {
      camera->dummy[0] = CAMERA_KEY;

      MemCpy((VoidPtr)&camera->dummy[1],  (VoidPtr)vvv->a, sizeof(vvv->a));
      MemCpy((VoidPtr)&camera->dummy[10], (VoidPtr)vvv->c, sizeof(vvv->c));

      camUnn.colorHLR   = vvv->colorHLR;
      camUnn.colorHLG   = vvv->colorHLG;
      camUnn.colorHLB   = vvv->colorHLB;
      camUnn.perspectN  = vvv->perspectN;
      camUnn.brighN     = vvv->brighN;
      camUnn.colorPicN  = vvv->colorPicN;
      camUnn.colorFon   = vvv->colorFon;
      camUnn.colorFonR  = vvv->colorFonR;
      camUnn.colorFonG  = vvv->colorFonG;
      camUnn.colorFonB  = vvv->colorFonB;
      camUnn.adjust     = vvv->adjust;
      camUnn.scale      = vvv->scale;

      MemCpy((VoidPtr)&camera->dummy[13], (VoidPtr)&camUnn, sizeof(camUnn));
    }

  if (rectangle != NULL)
    ObjectRect(vvv->panel, rectangle);
}


extern Prim3D Nlm_GetFoundPrim3D(Viewer3D vvv, Uint2 primIndex)
{
  Nlm_DiagReset();
  if (vvv == NULL  ||  primIndex >= vvv->nPrimFound)
    return NULL;

  return vvv->foundPrim[primIndex];
}


extern Boolean Nlm_IsSeg3DHlighted(Viewer3D vvv, Segment3D segment)
{
  Nlm_DiagReset();

  return (Boolean)(segment != NULL  && 
                   (((Nlm_Base3DPtr)segment)->status & HL_SEGMENT));
}


extern Boolean Nlm_IsPrim3DHlighted(Viewer3D vvv, Prim3D prim)
{
  Nlm_DiagReset();
  return (Boolean)
    (prim != NULL  &&  (((Nlm_Base3DPtr)prim)->status & HL_PRIMITIVE));
}


extern void Nlm_SetBackground3D(Nlm_Viewer3D vvv,
                                Nlm_Uint1 red, Nlm_Uint1 green, Nlm_Uint1 blue)
{
  if (vvv == NULL)
      return;

  SetColorCell((Nlm_GraphiC)vvv->panel, vvv->colorFon,
               (vvv->colorFonR = red),
               (vvv->colorFonG = green),
               (vvv->colorFonB = blue));

  vvv->c_version++;
}


/*
 * Viewer auxiliary routines
 */

extern void BgColorDlg3D(Viewer3D vvv)
{ /* Choose and change the viewer background color */
  if ( ChooseColorDialog(&vvv->colorFonR,
                         &vvv->colorFonG,
                         &vvv->colorFonB,
                         0) )
    Nlm_SetBackground3D(vvv,
                        vvv->colorFonR,
                        vvv->colorFonG,
                        vvv->colorFonB);
}


extern void Nlm_ZoomAll3D(Viewer3D vvv)
{
  WindoW tmpPort;
  if (!Visible(vvv->panel) ||  !AllParentsVisible(vvv->panel))
    return;

  ZoomAll(vvv, ZOOM_KEEP_ANGLE);
  tmpPort = SavePort( vvv->panel );
  Select( vvv->panel );
  DrawViewer3D( vvv );
  RestorePort( tmpPort );
  Nlm_DiagReset();
}
 

#ifndef WIN_MOTIF
extern void Nlm_CopyViewer3D(Viewer3D vvv)
{
  SaveImageClip( vvv->image );
}

extern void Nlm_PrintViewer3D(Viewer3D vvv)
{
  PrintImage( vvv->image );
}
#endif


extern void Nlm_SaveViewer3D(Viewer3D vvv)
{
  Char fname[PATH_MAX];
  if ( !GetOutputFileName(fname, sizeof(fname), "*.gif") )
    return;

  SaveImageGIF(vvv->image, fname);
}


extern Uint2 Nlm_FindPrim3D(Viewer3D vvv, PoinT pt)
{
  Nlm_PPict3DPtr PPP;

  RecT r;
  int  i,j,k;
  int  x, y;

  Nlm_DiagReset();
  if (vvv == NULL)
    return 0;

  ObjectRect(vvv->panel, &r);

  Nlm_stCon.zBuffer  =
    (Uint2Ptr)MemNew((2*FIND_BLOCK_SIZE+1) * 
                     (2*FIND_BLOCK_SIZE+1) * sizeof(Uint2));
  Nlm_stCon.idBuffer =
    (VoidPtr*)MemNew((2*FIND_BLOCK_SIZE+1) * 
                     (2*FIND_BLOCK_SIZE+1) * sizeof(VoidPtr));
  if (!Nlm_stCon.zBuffer  ||  !Nlm_stCon.idBuffer) {
    Nlm_DiagPutRecord(DA_ERROR, viewerClass, "FindPrim3D",
                      "Cannot lock memory block");
    Nlm_stCon.zBuffer  = (Uint2Ptr)MemFree(Nlm_stCon.zBuffer);
    Nlm_stCon.idBuffer = (VoidPtr*)MemFree(Nlm_stCon.idBuffer);
    return 0;
  }

  PPP = (Nlm_PPict3DPtr)vvv->pic;

  pt.x -= r.left + 2;
  pt.y -= r.top  + 4;
#ifdef NEGYSCREEN
  pt.y = (Int2)(vvv->height - pt.y);
#endif
  SetUpContext( vvv );
  Nlm_stCon.widthCur = Nlm_stCon.heightCur = FIND_BLOCK_SIZE * 2 + 1;
  Nlm_UpdateGver( PPP );
  Nlm_stCon.xmin = pt.x - FIND_BLOCK_SIZE;
  Nlm_stCon.xmax = pt.x + FIND_BLOCK_SIZE;
  Nlm_stCon.ymin = pt.y - FIND_BLOCK_SIZE;
  Nlm_stCon.ymax = pt.y + FIND_BLOCK_SIZE;
  PPP->seg.base.fTable->hittest( (Nlm_VoidPtr)&PPP->seg );
  vvv->nPrimFound = 0;
  for (i =  0;  i <= (FIND_BLOCK_SIZE - 1);  i++)
  for (j = -i;  j <= i;  j++)
  for (k =  0;  k <  4;  k++) {
    switch ( k ) {
    case 0:
      x = -j;  y = -i;  break;
    case 1:
      x =  j;  y =  i;  break;
    case 2:
      x = -i;  y =  j;  break;
    default:
      x =  i;  y = -j;
    }
    x += FIND_BLOCK_SIZE;
    y += FIND_BLOCK_SIZE;
    Nlm_AddFoundPrim(vvv,
                     (Prim3D)Nlm_stCon.idBuffer[y * (FIND_BLOCK_SIZE+1) + x]);
  }

  Nlm_stCon.zBuffer  = (Uint2Ptr) MemFree( Nlm_stCon.zBuffer  );
  Nlm_stCon.idBuffer = (VoidPtr*) MemFree( Nlm_stCon.idBuffer );
  return vvv->nPrimFound;
}


extern void Nlm_HighlightSeg3D(Viewer3D vvv,
                               Segment3D segment, Boolean highlight)
{
  register Nlm_Base3DPtr prim = (Nlm_Base3DPtr)segment;

  Nlm_DiagReset();
  if (vvv == NULL  ||  prim == 0)
    return;

  if ( highlight )
    prim->status |= HL_SEGMENT;
  else
    prim->status &= ~HL_MASK;

  for (prim = ((Nlm_PSeg3DPtr)prim)->first;  prim;  prim = prim->next) {
    if (prim->fTable->code == 0)
      Nlm_HighlightSeg3D(vvv, (Segment3D)prim, highlight);
    else if ( highlight )
      prim->status |= HL_SEGMENT;
    else
      prim->status &= ~HL_SEGMENT;
  }

  {{
    register Nlm_PPict3DPtr PPP = (Nlm_PPict3DPtr)vvv->pic;
    if (PPP->version == vvv->p_version)
      {
        Nlm_RedrawPrim3D(vvv, (Nlm_Base3DPtr)segment);
        vvv->p_version++;
      }
    PPP->version++;
  }}
}


extern void Nlm_HighlightPrim3D(Viewer3D vvv, Prim3D prim, Boolean highlight)
{
  Nlm_DiagReset();
  if (vvv == NULL  ||  prim == NULL)
    return;

  if ( highlight )
    ((Nlm_Base3DPtr)prim)->status |= HL_PRIMITIVE;
  else
    ((Nlm_Base3DPtr)prim)->status &= ~HL_PRIMITIVE;

  {{
    register Nlm_PPict3DPtr PPP = (Nlm_PPict3DPtr)vvv->pic;
    if (PPP->version == vvv->p_version)
      {
        Nlm_RedrawPrim3D(vvv, (Nlm_Base3DPtr)prim);
        vvv->p_version++;
      }
    PPP->version++;
  }}
}


extern Boolean Nlm_ResetHighlight3D(Viewer3D vvv)
{
  Nlm_DiagReset();

  if (vvv != NULL  &&  vvv->pic != NULL)
    {
      register Nlm_PPict3DPtr PPP = (Nlm_PPict3DPtr)vvv->pic;
      Nlm_ResetHLSeg3D( (Segment3D)&PPP->seg );
      PPP->version++;
    }

  return TRUE;
}


extern Boolean Nlm_MapWorldToViewer3D(Viewer3D vvv, Int4 x, Int4 y, Int4 z,
                                      PoinT PNTR vPoint)
{
  register Viewer3D VVV = vvv;
  Int4 xs, ys, zs;
  Int4 t4, zmaxPersp, zmaxPersp1;

  Nlm_DiagReset();
  if (VVV == NULL)
    return FALSE;

  zmaxPersp  = VVV->perspect * VVV->zmax;
  zmaxPersp1 = zmaxPersp + VVV->zmax;
  xs = x/VVV->a[0][0] + y/VVV->a[0][1] + z/VVV->a[0][2] + VVV->c[0];
  ys = x/VVV->a[1][0] + y/VVV->a[1][1] + z/VVV->a[1][2] + VVV->c[1];
  zs = x/VVV->a[2][0] + y/VVV->a[2][1] + z/VVV->a[2][2] + VVV->c[2];
  t4 = xs - (VVV->width >>1);
  xs = (VVV->width >>1) + (t4 * zmaxPersp) / (zmaxPersp1 - zs);
  t4 = ys - (VVV->height>>1);
  ys = (VVV->height>>1) - (t4 * zmaxPersp) / (zmaxPersp1 - zs);

  vPoint->x = (Int2)xs;
  vPoint->y = (Int2)ys;
  return TRUE;
}


extern Boolean Nlm_SetStdMouse3D(Viewer3D vvv, enumStdMA3D action)
{
  return MA_SetGroup( vvv->ma_std_group[action] );
}


/*
 * Viewer general management
 */

extern Viewer3D Nlm_CreateViewer3D(GrouP prnt,
                                   Uint2Ptr width, Uint2 height,
                                   Int4 flags,
                                   MenU ma_group_menu, MenU ma_action_menu,
                                   Nlm_MAInit3DFunc ma_init_func,
                                   VoidPtr          ma_init_data) 
{
  Viewer3D vvv     = NULL;
  CharPtr  err_msg = NULL;

  while ( TRUE ) {{{ /* TRY! */

  Int4  i;
  Uint2 x_width;

  Nlm_DiagReset();

  if ( !isqrtInit )
    {
      Int4 j, k = 0;
      for (i =  0; i < 128; i++)
      for (j = -i; j <= i;  j++)
        Nlm_Isqrt[k++] = (Uint1)(sqrt( (double)(i*i - j*j) ));

      isqrtInit = TRUE;
    }

  vvv = (Viewer3D)MemNew( sizeof(structViewer3D) );
  if (vvv == NULL) {
    err_msg = "Cannot allocate memory block for the viewer";
    break;
  }

  vvv->parentWindow = ParentWindow( (Handle)prnt );
  vvv->panel = AutonomousPanel(prnt,
                              (Int2)*width, (Int2)height,
                               DrawViewer3D_CB,
                               ((flags & Y_ROTATE_SBAR) ?
                                ViewerVScrollProc : NULL),
                               ((flags & X_ROTATE_SBAR) ?
                                ViewerHScrollProc : NULL),
                               sizeof(MAPtr), ResetViewerProc_CB, NULL);
  if ( !vvv->panel ) {
    err_msg = "Cannot create panel";
    break;
  }

  if (flags & Z_ROTATE_SBAR)
    {
      vvv->z_rotate = ScrollBar(prnt, 1, 0, ViewerZScrollProc);
      if ( !vvv->z_rotate ) {
        err_msg = "Cannot create Z-rotation scrollbar";
        break;
      }
      SetObjectExtra(vvv->z_rotate, vvv->panel, NULL);
    }

  {{
    RecT rect;
    GetPosition(vvv->panel, &rect);
    rect.right  = (Int2)(rect.left + *width);
    rect.bottom = (Int2)(rect.top  + height);
    if ( !SetPosition3D(vvv, &rect) ) {
      err_msg = "Cannot allocate memory for the viewer image & zBuffer";
      break;
    }
    x_width = (Uint2)(rect.right - rect.left);
  }}

  vvv->colorHLR  = vvv->colorHLG  = vvv->colorHLB = (Uint1)255;
  vvv->colorFon  = ImageGetBlack( vvv->image );
  vvv->colorFonR = vvv->colorFonG = vvv->colorFonB = (Uint1)0;
  for ( i=0; i<256; i++ )
    vvv->layerTable[i] = TRUE;

  if (flags & X_ROTATE_SBAR) {
    BaR sb = GetSlateHScrollBar( (SlatE)vvv->panel );
    CorrectBarValue(sb, 0);
    SetRange(sb, 10, 10, 360);
  }
  if (flags & Y_ROTATE_SBAR) {
    BaR sb = GetSlateVScrollBar( (SlatE)vvv->panel );
    CorrectBarValue(sb, 0);
    SetRange(sb, 10, 10, 360);
  }
  if (flags & Z_ROTATE_SBAR) {
    SetRange(vvv->z_rotate, 10, 10, 360);
    CorrectBarValue(vvv->z_rotate, 180);
  }

  vvv->adjust = ADJUST3D_PERSPECT;

  vvv->ma = MA_Create(ma_group_menu, ma_action_menu);
  MA_SetExtra(vvv->ma, vvv);

  if ( !InitializeMA( vvv ) ) {
    err_msg = "Cannot create/initialize standard mouse controls";
    break;
  }

  if (ma_init_func  &&  !(*ma_init_func)(vvv->ma, ma_init_data)) {
    err_msg = "Cannot create/initialize user-specified mouse controls";
    break;
  }

  *width = x_width;
  return vvv;
  }}} /* End of TRY;  below is the CATCH block(post-error cleaning code): */

  
  ASSERT ( err_msg );
  Nlm_DiagPutRecord(DA_ERROR, viewerClass, "CreateViewer3D", err_msg);
  if ( vvv )
    {
      if ( vvv->panel )
        Remove( vvv->panel );
      if ( vvv->z_rotate )
        Remove( vvv->z_rotate );
      ResetImage3D( vvv );
      MemFree( vvv );
    }

  return NULL;
}


extern Boolean Nlm_SetPosition3D(Viewer3D vvv, RectPtr rect)
{
  Int4 width  = rect->right - rect->left;
  Int4 height = rect->bottom - rect->top;

  Nlm_DiagReset();

  ResetImage3D( vvv );

  if ( vvv->z_rotate )
    {
      rect->top += Nlm_hScrollBarHeight;
      height    -= Nlm_hScrollBarHeight;
    }

  if ( GetSlateVScrollBar((SlatE)vvv->panel) )
    width  -= Nlm_vScrollBarWidth  + 3;
  if ( GetSlateHScrollBar((SlatE)vvv->panel) )
    height -= Nlm_hScrollBarHeight + 3;

  if (width < 16  ||  height < 16
#ifdef WIN16
      ||  width * height > (Int4)0x7FFF
#endif
      )
    return FALSE;

  vvv->image = CreateImage();
  if (vvv->image == NULL)
    return FALSE;

  {{
    Uint2 x_width = (Uint2)width;
    if ( !AllocateImage(vvv->image, &x_width, (Uint2)height, 32, 256) ) {
      ResetImage3D( vvv );
      return FALSE;
    }
    width = x_width;
  }}

  vvv->width  = (Int2)width;
  vvv->height = (Int2)height;
  vvv->zBuffer = HandNew(sizeof(Int2) * vvv->width * vvv->height);
  if (vvv->zBuffer == NULL) {
    ResetImage3D( vvv );
    return FALSE;
  }

  vvv->g_version++;
  vvv->c_version++;

  ZoomAll(vvv, ZOOM_KEEP_ALL);

  rect->right  = (Int2)(rect->left + vvv->width  + 3);
  rect->bottom = (Int2)(rect->top  + vvv->height + 3);
  if ( GetSlateVScrollBar((SlatE)vvv->panel) )
    rect->right  += Nlm_vScrollBarWidth;
  if ( GetSlateHScrollBar((SlatE)vvv->panel) )
    rect->bottom += Nlm_hScrollBarHeight;

  SetPosition(vvv->panel, rect);
  ProcessUpdatesFirst( FALSE );
  AdjustPrnt(vvv->panel, rect, FALSE);

  if ( vvv->z_rotate )
    {
      rect->bottom = rect->top;
      rect->top   -= Nlm_hScrollBarHeight;
      SetPosition(vvv->z_rotate, rect);
    }

  return TRUE;
}


extern Boolean Nlm_AttachPicture3D(Viewer3D vvv, Picture3D picture, 
                                   Nlm_Camera3DPtr camera)
{
  Nlm_PPict3DPtr PPP = (Nlm_PPict3DPtr)picture;
  
  Nlm_DiagReset();
  if (vvv == NULL)
    return FALSE;

  vvv->pic = NULL;
  {{
    BaR sb;
    sb = GetSlateHScrollBar( (SlatE)vvv->panel );
    if ( sb ) {
      CorrectBarValue(sb, 0);
      SetRange(sb, 10, 10, 360);
    }
    sb = GetSlateVScrollBar( (SlatE)vvv->panel );
    if ( sb ) {
      CorrectBarValue(sb, 0);
      SetRange(sb, 10, 10, 360);
    }
    if ( vvv->z_rotate ) {
      CorrectBarValue(vvv->z_rotate, 180);
      SetRange(vvv->z_rotate, 10, 10, 360);
    }
  }}
  vvv->pic = picture;

  ASSERT ( sizeof(vvv->layerTable)  ==  sizeof(PPP->layerTable) );
  MemCpy(vvv->layerTable, PPP->layerTable, sizeof(vvv->layerTable));
  SetLayer3D(picture, 1, TRUE);
  vvv->layerTable[0] = TRUE; 
  currlayer = 0;


  ASSERT ( CAMERA_SIZE_I4 * sizeof(Int4)  ==  sizeof(camera->dummy) );
  if (camera != NULL  &&  camera->dummy[0] == CAMERA_KEY)
    {
      PCamera3DUnion camUnn;

      ASSERT ( sizeof(vvv->a) <=
	       sizeof(camera->dummy) - 1*sizeof(camera->dummy[0]) ); 
      MemCpy((VoidPtr)vvv->a,  (VoidPtr)&camera->dummy[1],  sizeof(vvv->a));

      ASSERT ( sizeof(vvv->c) <=
	       sizeof(camera->dummy) - 10*sizeof(camera->dummy[0]) ); 
      MemCpy((VoidPtr)vvv->c,  (VoidPtr)&camera->dummy[10], sizeof(vvv->c));

      ASSERT ( sizeof(camUnn) <=
	       sizeof(camera->dummy) - 13*sizeof(camera->dummy[0]) ); 
      MemCpy((VoidPtr)&camUnn, (VoidPtr)&camera->dummy[13], sizeof(camUnn));

      vvv->perspectN  = camUnn.perspectN;
      vvv->perspect   = perspectAll[vvv->perspectN];
      vvv->brighN     = camUnn.brighN;
      vvv->colorPicN  = camUnn.colorPicN;
      vvv->colorHLR   = camUnn.colorHLR;
      vvv->colorHLG   = camUnn.colorHLG;
      vvv->colorHLB   = camUnn.colorHLB;
      vvv->colorFon   = camUnn.colorFon;
      vvv->colorFonR  = camUnn.colorFonR;
      vvv->colorFonG  = camUnn.colorFonG;
      vvv->colorFonB  = camUnn.colorFonB;
      vvv->adjust     = camUnn.adjust;
      vvv->scale      = camUnn.scale;
      MakeFloatMat( vvv );
      ZoomAll(vvv, ZOOM_KEEP_ALL);
    }
  else
    {
      vvv->perspectN = 2;
      vvv->perspect  = perspectAll[vvv->perspectN];
      vvv->brighN    = 5;
      vvv->colorPicN = 5;
      ZoomAll(vvv, ZOOM_ALL);
    }
  vvv->c_version++;
  vvv->g_version++;
  SetUpContext( vvv );

  if (Visible(vvv->panel)  &&  AllParentsVisible(vvv->panel))
    {
      RecT   r;
      WindoW tempPort = SavePort( vvv->panel );
      Select( vvv->panel );
      ObjectRect(vvv->panel, &r);
      InvalRect( &r );
      RestorePort( tempPort );
    }

  return DiagHasErrorRec();
}


extern Boolean Nlm_RedrawViewer3D(Viewer3D vvv)
{
  Nlm_DiagReset();

  if (!Visible( vvv->panel )  ||  !AllParentsVisible( vvv->panel ))
    return FALSE;

  {{
    WindoW tempPort = SavePort( vvv->panel );
    Select( vvv->panel );
    DrawViewer3D( vvv );
    RestorePort( tempPort );
  }}

  return DiagHasErrorRec();
}


extern void Nlm_DeleteViewer3D(Nlm_Viewer3D vvv)
{
  if (vvv == NULL)
    return;

  Nlm_DiagReset();

  MA_Destroy( vvv->ma );

  ResetImage3D( vvv );

  if ( vvv->panel )
    Remove( vvv->panel );

  MemFree( vvv );
}

