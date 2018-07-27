/*   image.c
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
* File Name:  image.c
*
* Author:  Alex Smirnov,  Denis Vakatov
*
* $Revision: 6.8 $
*
* File Description:
*       Image(pixmap) processing.
*
* Version Creation Date:   04/03/95
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: image.c,v $
* Revision 6.8  2004/04/01 13:43:08  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 6.7  2003/11/17 17:03:29  kans
* changed C++ style comments to C comments
*
* Revision 6.6  2002/03/28 13:35:48  kans
* only include MoreCarbonAccessors.h if not OS_UNIX_DARWIN
*
* Revision 6.5  2001/04/05 03:24:12  juran
* Carbon fixes.
*
* Revision 6.4  1999/11/22 14:46:45  thiessen
* moved _OPENGL code blocks to only vibrant and ncbicn3d libraries
*
* Revision 6.3  1999/06/22 15:14:50  lewisg
* fix image library so that works on linux with > 8 bits
*
* Revision 6.2  1998/01/05 20:00:13  vakatov
* [WIN_X11] Nlm_AllocateImage() -- better diagnostics for mismatching visual
*
* Revision 6.1  1997/11/26 21:29:54  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:55:46  madden
* Revision changed to 6.0
*
* Revision 5.7  1997/06/12 20:52:03  vakatov
* Lock/UnlockPixMapImage -- check for the NULL image
*
* Revision 5.6  1997/06/09 18:55:35  vakatov
* [WIN_X]  Nlm_ImageShow():  by default, use standard RGB colormap for
* the image drawing.  +code cleaning.
*
* Revision 5.5  1997/05/28 21:15:29  vakatov
* Nlm_LoadImageGIF():  "pack" the image colors;; fixed Uint1-overflow bug
*
* Revision 5.4  1996/11/22 19:46:07  vakatov
* Implemented function Nlm_LoadImageGIF();
* added Nlm_ImageGetColorOffset() and Nlm_ImageGetColorMax();
* slightly modified Nlm_ImageGetBlack()
*
 * Revision 5.3  1996/07/23  21:57:32  vakatov
 * Do not reinstall colormap if this is identical to the previous one --
 * it eliminates a redundant screen blinking when redrawing the 3D-Viewer
 * [WIN_MOTIF]  Use 32th(instead of 0th) color for background
 *
 * Revision 5.2  1996/06/17  21:57:17  kans
 * use 255 instead of 254 for Nlm_ImageShow on Mac
 *
 * Revision 5.1  1996/06/14  14:30:42  vakatov
 * [WIN_MOTIF] Nlm_AllocateImage() function -- removed redundant color saving
 * [WIN_MOTIF] Nlm_ImageGetBlack() now always return zero
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 1.8  1996/05/07  18:31:23  vakatov
 * [WIN_MSWIN]  Nlm_AllocateImage() now adjusts the image width
 * (performs sizeof(LONG) alignment),thus avoiding the image picture distortion
 *
 * Revision 1.7  1996/04/23  23:26:24  kans
 * added Mac-specific code to Nlm_SaveImageGIF
 *
 * Revision 1.6  1996/04/23  15:44:48  vakatov
 * Nlm_SaveImageGIF()/X11 accelerated
 *
 * Revision 1.5  1996/04/17  21:40:34  vakatov
 * Implemented "Nlm_SaveImageGIF()" function for MS-Windows and
 * X11 platforms -- "I can save to GIF-file whatever I can see on
 * my monitor".
 * VC log inserted.
 *
* ==========================================================================
*/

#include <vibtypes.h>
#include <vibprocs.h>
#include <vibincld.h>
#include <math.h>

#ifdef WIN_MAC
# if !defined(OS_UNIX_DARWIN)
#include "MoreCarbonAccessors.h"
#endif
#endif

#ifndef _IMAGE_
#include <image.h>
#endif
#ifndef _PDIAGNOS_
#include <pdiagnos.h>
#endif

#include <gifgen.h>

/*****************************************************************************
*
*   TYPEDEFS
*
*****************************************************************************/
typedef struct Nlm_pimage {
  Nlm_Uint1        red[256];
  Nlm_Uint1        green[256];
  Nlm_Uint1        blue[256];
  Nlm_Int4         imageVer;
  Nlm_Int4         imageVerOld;
  Nlm_Int4         imageColorVer;
  Nlm_Int4         imageColorVerOld;
  Nlm_Uint1Ptr     curImagePtr;
  Nlm_Handle       image;
  Nlm_WindoW       curWin;
  Nlm_Uint2        width;
  Nlm_Uint2        height;
  Nlm_Uint2        totalColors;
  Nlm_Uint2        saveColors;
  Nlm_Uint2        imageLocked;
#ifdef WIN_MSWIN
  HBITMAP     pixMap;
  HDC         hMemDC;
  BITMAPINFO *bitInfo;
#endif
#ifdef WIN_MOTIF
  Nlm_Uint1   altcol[256];
  XImage     *imageX11;
#endif
#ifdef WIN_MAC
  PixMap     *pixelMap;
#endif
} Nlm_PImage, PNTR Nlm_PImagePtr;

/*****************************************************************************
*
*   GLOBAL VARIABLE
*
*****************************************************************************/
#ifdef WIN_MSWIN
extern HDC Nlm_currentHDC;
#endif
#ifdef WIN_MOTIF
extern Display *Nlm_currentXDisplay;
extern Window   Nlm_currentXWindow;
extern GC       Nlm_currentXGC;
#endif
#ifdef WIN_MAC
extern Nlm_Boolean  Nlm_hasColorQD;
#endif

/*****************************************************************************
*
*   STATIC VARIABLE
*
*****************************************************************************/
static Nlm_CharPtr imageClass = "Image";


/*****************************************************************************
*
*   FUNCTIONS
*
*****************************************************************************/
#ifdef WIN_MSWIN
static Nlm_CharPtr GetWinErrStr( void )
{
  static Nlm_Char winErrStr[128];

#ifdef BORLAND
  sprintf(winErrStr, "Windows error %d", GetLastError());
#else
  Nlm_StringCpy(winErrStr, "Windows error");
#endif

  return winErrStr;
}


static void AllocateColorMap(Nlm_PImagePtr image)
{
  register RGBQUAD *rgbq = image->bitInfo->bmiColors;
  register int i;
  int          totalColors = (int)image->totalColors;

  image->bitInfo->bmiHeader.biClrUsed = totalColors;
  for (i = 0;  i < totalColors;  i++, rgbq++)
    {
      rgbq->rgbRed   = image->red  [i];
      rgbq->rgbGreen = image->green[i];
      rgbq->rgbBlue  = image->blue [i];
      rgbq->rgbReserved = 0;
    }
}
#endif


Nlm_Image Nlm_CreateImage( void )
{
  Nlm_Image im;

  Nlm_DiagReset ();
  im = (Nlm_Image)MemNew( sizeof(Nlm_PImage) );
  if ( !im )
    Nlm_DiagPutRecord(DA_ERROR, imageClass, "CreateImage",
                      "Can not allocate memory block");
  return im;
}


Nlm_Boolean Nlm_AllocateImage ( Nlm_Image image, Nlm_Uint2Ptr width,
                                Nlm_Uint2 height, Nlm_Uint2 saveColors,
                                Nlm_Uint2 totalColors )
{
  register Nlm_PImagePtr im;
  Nlm_Int2               i;
#ifdef WIN_MSWIN
  BITMAPINFOHEADER PNTR bmpHeader;
  PALETTEENTRY     PNTR lppe;
#endif
#ifdef WIN_MOTIF
  XVisualInfo visinfo;
#endif
#ifdef WIN_MAC
  CTabHandle   cTable;
  CTabPtr      cTablePtr;
  ColorSpecPtr cSpecPtr;
#endif

  Nlm_DiagReset ();
  im = (Nlm_PImagePtr)image;
  im->imageVer++;
#ifdef WIN_MSWIN
  *width /= sizeof(LONG);
  *width *= sizeof(LONG);
#endif
  if ((*width == 0) || (height == 0) || (totalColors < saveColors) ||
       (totalColors > 256) ) {
    Nlm_DiagPutRecord ( DA_ERROR, imageClass, "AllocateImage",
                        "Invalid image or palette size" );
    return FALSE;
  }
  if ( im->image != NULL ) {
    if ( im->imageLocked ) HandUnlock ( im->image );
    HandFree ( im->image );
    im->imageLocked = 0;
  }
  im->image = HandNew(*width * height );
  if ( im->image == NULL ) {
    Nlm_DiagPutRecord ( DA_ERROR, imageClass, "AllocateImage",
                        "Can not allocate memory block" );
    return FALSE;
  }
#ifdef WIN_MSWIN
  if ( im->bitInfo == NULL ){
    im->bitInfo = (BITMAPINFO*)MemNew ( sizeof(BITMAPINFOHEADER) +
                               256*sizeof(RGBQUAD) );
  }
  if ( im->bitInfo == NULL ) {
    Nlm_DiagPutRecord ( DA_ERROR, imageClass, "AllocateImage",
                        "Can not allocate memory block" );
    return FALSE;
  }
  bmpHeader = &(im->bitInfo->bmiHeader);
  bmpHeader->biWidth  = *width;
  bmpHeader->biHeight = height;
  bmpHeader->biSize = sizeof(BITMAPINFOHEADER);
  bmpHeader->biCompression = BI_RGB;
  bmpHeader->biXPelsPerMeter = 2000;
  bmpHeader->biYPelsPerMeter = 2000;
  bmpHeader->biClrImportant = 0;
  bmpHeader->biSizeImage = 0;
  bmpHeader->biBitCount = 8;
  bmpHeader->biPlanes = 1;
  bmpHeader->biClrUsed = 0;
  lppe = (PALETTEENTRY PNTR)MemNew ( saveColors * sizeof(PALETTEENTRY));
  if ( lppe == NULL ) {
    Nlm_DiagPutRecord ( DA_ERROR, imageClass, "AllocateImage",
                        "Can not allocate memory block" );
  }
  GetSystemPaletteEntries ( Nlm_currentHDC, 0, saveColors, lppe );
  for ( i=0; i<saveColors; i++ ) {
    im->red  [i] = lppe[i].peRed;
    im->green[i] = lppe[i].peGreen;
    im->blue [i] = lppe[i].peBlue;
  }
  MemFree ( lppe );
#endif
#ifdef WIN_MOTIF
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
      {
    Nlm_DiagPutRecord ( DA_ERROR, imageClass, "AllocateImage",
"Your X display cannot support 8-bit(256 colors) neither PseudoColor nor GrayScale visual" );
    return FALSE;
  }
  im->curImagePtr = (Nlm_Uint1Ptr)HandLock ( im->image );
  if ( im->curImagePtr == NULL ){
    Nlm_DiagPutRecord ( DA_ERROR, imageClass, "AllocateImage",
                        "Can not lock memory block" );
    return FALSE;
  }
  HandUnlock ( im->image );
  if ( im->imageX11 == NULL ){
    im->imageX11 = XCreateImage(Nlm_currentXDisplay, 
                                visinfo.visual, visinfo.depth, ZPixmap, 0,
                                (char*)im->curImagePtr,
                                *width, height, 8, 0);
  }
  if ( im->imageX11 == NULL ){
    Nlm_DiagPutRecord ( DA_ERROR, imageClass, "AllocateImage",
                        "Can not create X11 image" );
    return FALSE;
  }
#endif
#ifdef WIN_MAC
  if ( im->pixelMap == NULL ) {
    im->pixelMap = (PixMap*)MemNew(sizeof(PixMap));
  }
  if ( im->pixelMap == NULL ) {
    Nlm_DiagPutRecord ( DA_ERROR, imageClass, "AllocateImage",
                        "Can not allocate memory block" );
    return FALSE;
  }
  im->pixelMap->hRes = 72;
  im->pixelMap->vRes = 72;
  im->pixelMap->bounds.left = 0;
  im->pixelMap->bounds.top = 0;
  im->pixelMap->cmpSize = 8;
  /* 2001-03-22:  Joshua Juran */
  /* Evidently these two members don't exist in Carbon.  So don't set them. */
#if !TARGET_API_MAC_CARBON
  im->pixelMap->planeBytes = 0;
  im->pixelMap->pmReserved = 0;
#endif
  im->pixelMap->pmVersion = 0;
  im->pixelMap->packType = 0;
  im->pixelMap->packSize = 0;
  im->pixelMap->pixelSize = 8;
  im->pixelMap->pixelType = 0;
  im->pixelMap->cmpCount = 1;
  im->pixelMap->rowBytes = *width | 0x8000;
  im->pixelMap->bounds.right = *width;
  im->pixelMap->bounds.bottom = height;
  cTablePtr = NULL;
  if ( Nlm_hasColorQD ) cTable = GetCTable ( 72 );
  else cTable = GetCTable ( 40 );
  if ( cTable != NULL ){
    HLock ( (Nlm_Handle)cTable );
    cTablePtr = (CTabPtr)(*((CTabPtr*)cTable));
  }
  if ( cTablePtr != NULL ){
    cSpecPtr = &(cTablePtr->ctTable[0]);
    for ( i=0; i<(Nlm_Int2)saveColors; i++ ) {
      im->red[i] = (cSpecPtr->rgb.red >> 8);
      im->green[i] = (cSpecPtr->rgb.green >> 8);
      im->blue[i] = (cSpecPtr->rgb.blue >> 8);
      cSpecPtr++;
    }
    HUnlock ((Nlm_Handle)cTable );
  } else {
    for ( i=0; i<(Nlm_Int2)saveColors; i++ ) {
      if ( i & 0x1 ){
        im->red[i] = im->green[i] = im->blue[i] = 0;
      } else {
        im->red[i] = im->green[i] = im->blue[i] = 0xFFFF;
      }
    }
  }
  if ( cTable != NULL ) DisposeCTable(cTable);
#endif

  for ( i=saveColors; i<256; i++ )
    im->red[i] = im->green[i] = im->blue[i] = 0;
  im->totalColors = totalColors;
  im->saveColors  = saveColors;
  im->width  = *width;
  im->height = height;
  return TRUE;
}


Nlm_Image Nlm_LoadImageGIF (Nlm_CharPtr fileName)
{
#if defined(WIN_MOTIF) || defined(WIN_MSWIN) || defined(WIN_MAC)
  Nlm_PImagePtr nlm_image = NULL;
  gdImagePtr    gd_image  = NULL;
  int           convert[256];
  Nlm_Uint2     width, height;
  Nlm_Uint2     nColors, saveColors;

  Nlm_DiagReset();

  {{ /* Read "gd"-wise image from the GIF metafile */
    FILE *inp_stream = FileOpen(fileName, "rb");
    if ( !inp_stream ) {
      Nlm_DiagPutRecord(DA_ERROR, imageClass, "LoadImageGIF",
                        "Can't open GIF file for reading");
      return NULL;
    }

    gd_image  = gdImageCreateFromGif( inp_stream );
    if ( !gd_image ) {
      Nlm_DiagPutRecord(DA_ERROR, imageClass, "LoadImageGIF",
                        "Error occurred while reading GIF file");
      FileClose( inp_stream );
      return NULL;
    }
    FileClose( inp_stream );
  }}

  {{ /* Count number of valid colors in the image */
    int gd_color;

    nColors = 0;
    for (gd_color = 0;  gd_color < gdImageColorsTotal(gd_image);  gd_color++)
      {
        if ( gdGetImageColor(gd_image, gd_color, NULL, NULL, NULL) )
          nColors++;
      }
  }}

  {{ /* Create and allocate "Nlm-wise" image */
    width   = (Nlm_Uint2)gdImageSX( gd_image );
    height  = (Nlm_Uint2)gdImageSY( gd_image );
    saveColors = (Nlm_Uint2)(256 - nColors);
    if (saveColors > 32)
      saveColors = 32;
    nlm_image = (Nlm_PImagePtr)Nlm_CreateImage();
    if ( !AllocateImage((Nlm_Image)nlm_image, &width, height, saveColors,
                        (Nlm_Uint2)(saveColors + nColors)) )
    {
      Nlm_DiagPutRecord(DA_ERROR, imageClass, "LoadImageGIF",
                        "Cannot allocate image");
      Nlm_DeleteImage( (Nlm_Image)nlm_image );
      gdImageDestroy( gd_image );
      return NULL;
    }
  }}

  {{ /* Copy palette */
    int color = saveColors;
    int gd_color;
    int red, green, blue;
    for (gd_color = 0;  gd_color < gdImageColorsTotal(gd_image);  gd_color++)
      {
#ifdef _DEBUG
        convert[gd_color] = -1;
#endif
        if ( gdGetImageColor(gd_image, gd_color, &red, &green, &blue) )
          {
            VERIFY ( Nlm_SetColorImage((Nlm_Image)nlm_image, (Nlm_Uint1)color,
                                       (Nlm_Uint1)red, (Nlm_Uint1)green,
                                       (Nlm_Uint1)blue) );
            convert[gd_color] = color++;
          }
      }
    ASSERT ( color == saveColors + nColors );
  }}


  {{ /* Copy gdImage to Nlm_Image */
    Nlm_Uint2 y;
    Nlm_Uint1Ptr pixmap = Nlm_LockPixMapImage( (Nlm_Image)nlm_image );
    for (y = 0;  y < nlm_image->height;  y++)
      {
#if   defined(WIN_MOTIF) || defined(WIN_MAC)
        Nlm_Uint4 y_base = nlm_image->width * y;
#elif defined(WIN_MSWIN)
        Nlm_Uint4 y_base = nlm_image->width * (nlm_image->height - y - 1);
#endif
        Nlm_Uint2 x;
        for (x = 0;  x < nlm_image->width;  x++)
          {
            int color = gdImageGetPixel(gd_image, x, y);
            ASSERT ( color < gdImageColorsTotal(gd_image) );
            ASSERT ( convert[color] != -1 );
            pixmap[y_base + x] = (Nlm_Uint1)convert[color];
          }
      }
    Nlm_UnlockPixMapImage( (Nlm_Image)nlm_image );
  }}

  gdImageDestroy( gd_image );
  return (Nlm_Image)nlm_image;

#else
  Nlm_DiagReset();
  Nlm_DiagPutRecord(DA_ERROR, imageClass, "LoadImageGIF", "Not implemented");
  return NULL;
#endif
}


Nlm_Image Nlm_LoadImageBMP(Nlm_CharPtr fileName)
{
  Nlm_DiagReset();
  Nlm_DiagPutRecord(DA_ERROR, imageClass, "LoadImageBMP", "Not implemented");
  return FALSE;
}


Nlm_Boolean Nlm_LoadImageClip(Nlm_Image image)
{
  Nlm_DiagReset();
  Nlm_DiagPutRecord(DA_ERROR, imageClass, "LoadImageClip", "Not implemented");
  return FALSE;
}


Nlm_Boolean Nlm_SaveImageGIF(Nlm_Image image, Nlm_CharPtr fileName)
{
#if !defined(_OPENGL) && (defined(WIN_MOTIF) || defined(WIN_MSWIN) || defined(WIN_MAC))
  Nlm_PImagePtr nlm_image = (Nlm_PImagePtr)image;
  gdImagePtr gd_image;
  Nlm_Int2 convert[256];

  Nlm_DiagReset();
  if ( !nlm_image ) {
    Nlm_DiagPutRecord(DA_ERROR, imageClass, "SaveImageGIF", "NULL image");
    return FALSE;
  }

  gd_image = gdImageCreate(nlm_image->width, nlm_image->height);

  {{ /* Set palette */
    Nlm_Uint2 color;
    for (color = nlm_image->saveColors;  color < nlm_image->totalColors;
         color++)
      {
        Nlm_Uint1 red=0, green=0, blue=0;
        Nlm_GetColorImage(image, (Nlm_Uint1)color, &red, &green, &blue);
        convert[color] = (Nlm_Int2)gdImageColorAllocate(gd_image,
                                                        red, green, blue);
        ASSERT ( 0 <= convert[color]  &&  convert[color] < 256 );
      }
  }}

  {{ /* Copy Nlm_Image to gdImage */
    Nlm_Uint2 y;
    Nlm_Uint1Ptr pixmap = Nlm_LockPixMapImage( (Nlm_Image)nlm_image );
    for (y = 0;  y < nlm_image->height;  y++)
      {
#if   defined(WIN_MOTIF) || defined(WIN_MAC)
        Nlm_Uint4 y_base = nlm_image->width * y;
#elif defined(WIN_MSWIN)
        Nlm_Uint4 y_base = nlm_image->width * (nlm_image->height - y - 1);
#endif
        Nlm_Uint2 x;
        for (x = 0;  x < nlm_image->width;  x++)
          {
            Nlm_Uint1 color = pixmap[y_base + x];
            gdImageSetPixel(gd_image, x, y, convert[color]);
          }
      }
    Nlm_UnlockPixMapImage( (Nlm_Image)nlm_image );
  }}

  {{ /* Write out */
    FILE *out_stream = FileOpen(fileName, "wb");
    if (out_stream == NULL)
      {
        gdImageDestroy( gd_image );
        Nlm_DiagPutRecord(DA_ERROR, imageClass, "SaveImageGIF",
                          "Can't open GIF file for writing");
        return FALSE;
      }

    gdImageGif(gd_image, out_stream);
    FileClose( out_stream );
  }}

  gdImageDestroy( gd_image );
  return TRUE;

#else
  Nlm_DiagReset();
  Nlm_DiagPutRecord(DA_ERROR, imageClass, "SaveImageGIF", "Not implemented");
  return FALSE;
#endif
}


Nlm_Boolean Nlm_SaveImageBMP(Nlm_Image image, Nlm_CharPtr fileName)
{
  Nlm_DiagReset();
  Nlm_DiagPutRecord(DA_ERROR, imageClass, "SaveImageBMP", "Not implemented");
  return FALSE;
}


Nlm_Boolean Nlm_SaveImageClip(Nlm_Image image)
{
  register Nlm_PImagePtr im = (Nlm_PImagePtr)image;
#ifdef WIN_MSWIN
  BITMAPINFO *bInfo;
  Nlm_Handle  bInfoHND;
  Nlm_Int4    sizeHead;
#endif

  Nlm_DiagReset();
  if ( !im->image ) {
    Nlm_DiagPutRecord(DA_ERROR, imageClass, "SaveImageClip", "Image is empty");
    return FALSE;
  }

#ifdef WIN_MSWIN
  if ( !Nlm_LockPixMapImage(image) )
    return FALSE;

  sizeHead = sizeof(BITMAPINFOHEADER) + im->totalColors * sizeof(RGBQUAD);
  bInfoHND = GlobalAlloc ( GMEM_MOVEABLE | GMEM_DDESHARE,
                           sizeHead + im->width * im->height);
  if ( bInfoHND == NULL ) {
      Nlm_DiagPutRecord(DA_ERROR, imageClass, "SaveImageClip",
                        "Cannot allocate memory" );
      UnlockPixMapImage( image );
      return FALSE;
  }
  bInfo = (BITMAPINFO*)HandLock( bInfoHND );
  MemCpy(bInfo, im->bitInfo, sizeHead);
  MemCpy((Nlm_Uint1Ptr)bInfo + sizeHead, im->curImagePtr,
         im->width * im->height);

  if( !OpenClipboard(NULL) ) {
    Nlm_DiagPutRecord(DA_ERROR, imageClass, "SaveImageClip", GetWinErrStr());
    GlobalFree( bInfoHND );
    UnlockPixMapImage( image );
    return FALSE;
  }
  EmptyClipboard();
  if ( !SetClipboardData(CF_DIB, bInfoHND) ) {
    Nlm_DiagPutRecord(DA_ERROR, imageClass, "SaveImageClip", GetWinErrStr());
    GlobalFree( bInfoHND );
    CloseClipboard();
    UnlockPixMapImage( image );
    return FALSE;
  }
  CloseClipboard();
  UnlockPixMapImage( image );
  return TRUE;

#else
  Nlm_DiagPutRecord(DA_ERROR, imageClass, "SaveImageClip", "Not implemented");
  return FALSE;
#endif
}

Nlm_Boolean Nlm_PrintImage ( Nlm_Image image )
{
#ifdef WIN_MSWIN
  register Nlm_PImagePtr im = (Nlm_PImagePtr)image;
  PRINTDLG               pd;
  Nlm_Uint1Ptr           fBuffer;
  Nlm_Int2               xsize, ysize, xres, yres;
  Nlm_Int2               dx, dy, caps;
  DOCINFO                info;
  RECT                   rect;
  HDC                    hDC;
#endif

  Nlm_DiagReset();

#ifdef WIN_MSWIN
  memset (&pd, 0, sizeof (PRINTDLG));
  pd.lStructSize = sizeof (PRINTDLG);
  pd.hwndOwner = Nlm_currentHWnd;
  pd.Flags = PD_RETURNDC;
  if (PrintDlg (&pd) == 0) return FALSE;
  hDC = pd.hDC;
  if( !hDC ) return FALSE;
  caps = GetDeviceCaps( hDC, RASTERCAPS );
  if( !(caps & RC_STRETCHDIB) ) return FALSE;
  xres = GetDeviceCaps( hDC, LOGPIXELSX );
  yres = GetDeviceCaps( hDC, LOGPIXELSY );
  xsize = GetDeviceCaps( hDC, HORZRES );
  ysize = GetDeviceCaps( hDC, VERTRES );
  dx = (Nlm_Int2)(xsize - xres);
  dy = (Nlm_Int2)(((Nlm_Int4)dx*im->height)/im->width);
  rect.top = yres;        rect.bottom = rect.top + dy;
  rect.left = xres>>1;    rect.right = rect.left + dx;
  Escape( hDC, SET_BOUNDS, sizeof(RECT), (Nlm_CharPtr)&rect, NULL );
  info.cbSize = sizeof(DOCINFO);
  info.lpszDocName = "Vibrant";
  info.lpszOutput = NULL;
  StartDoc( hDC, &info );
  StartPage( hDC );
  fBuffer = Nlm_LockPixMapImage ( image );
  StretchDIBits( hDC, xres>>1, yres, dx, dy,
                 0, 0, im->width, im->height,
                 fBuffer, im->bitInfo, DIB_RGB_COLORS, SRCCOPY );
  Nlm_UnlockPixMapImage ( image );
  EndPage( hDC );
  EndDoc( hDC );
  DeleteDC( hDC );
  if (pd.hDevMode != NULL) {
    GlobalFree (pd.hDevMode);
  }
  if (pd.hDevNames != NULL) {
    GlobalFree (pd.hDevNames);
  }
  return TRUE;

#else
  Nlm_DiagPutRecord(DA_ERROR, imageClass, "PrintImage", "Not implemented");
  return FALSE;
#endif
}


void Nlm_DeleteImage(Nlm_Image image)
{
  register Nlm_PImagePtr im = (Nlm_PImagePtr)image;

  if ( im->image )
    {
      if ( im->imageLocked )
        HandUnlock( im->image );
      HandFree( im->image );
    }

  if ( im->curWin != NULL )
    Nlm_SetColorMap(im->curWin, 0, NULL, NULL, NULL);

#ifdef WIN_MSWIN
  if ( im->hMemDC  )  DeleteDC( im->hMemDC );
  if ( im->pixMap  )  DeleteObject( im->pixMap );
  if ( im->bitInfo )  MemFree( im->bitInfo );
#endif
#ifdef WIN_MOTIF
  if ( im->imageX11 )
    {
      im->imageX11->data = NULL;
      XDestroyImage( im->imageX11 );
    }
#endif
#ifdef WIN_MAC
  if ( im->pixelMap )
    {
      if ( im->pixelMap->pmTable )
        DisposeCTable( im->pixelMap->pmTable );
      MemFree( im->pixelMap );
    }
#endif

  MemFree( im );
}


Nlm_Boolean Nlm_SetColorImage(Nlm_Image image, Nlm_Uint1 color,
                              Nlm_Uint1 red, Nlm_Uint1 green, Nlm_Uint1 blue)
{
  register Nlm_PImagePtr im = (Nlm_PImagePtr)image;

  Nlm_DiagReset();
  if (color >= im->totalColors) {
    Nlm_DiagPutRecord(DA_ERROR, imageClass, "SetColorImage",
                      "Color is out of range");
    return FALSE;
  }

  if (im->red  [color] != red    ||
      im->green[color] != green  ||
      im->blue [color] != blue)
    {
      im->red  [color] = red;
      im->green[color] = green;
      im->blue [color] = blue;
      im->imageColorVer++;
    }

  return TRUE;
}


Nlm_Boolean Nlm_GetColorImage(Nlm_Image image, Nlm_Uint1 color,
                              Nlm_Uint1Ptr red, Nlm_Uint1Ptr green,
                              Nlm_Uint1Ptr blue)
{
  register Nlm_PImagePtr im = (Nlm_PImagePtr)image;

  Nlm_DiagReset();
  if (color >= im->totalColors) {
    Nlm_DiagPutRecord(DA_ERROR, imageClass, "GetColorImage",
                      "Color is out of range");
    return FALSE;
  }
  if ( red   )  *red   = im->red  [color];
  if ( green )  *green = im->green[color];
  if ( blue  )  *blue  = im->blue [color];
  return TRUE;
}


Nlm_Uint1Ptr Nlm_LockPixMapImage(Nlm_Image image)
{
  Nlm_PImagePtr im = (Nlm_PImagePtr)image;
  if ( !im )
    return NULL;

  Nlm_DiagReset();

  if ( im->imageLocked ) {
    im->imageLocked++;
    return im->curImagePtr;
  }
  im->curImagePtr = (Nlm_Uint1Ptr)HandLock( im->image );
  if (im->curImagePtr == NULL)
    Nlm_DiagPutRecord(DA_ERROR, imageClass, "LockPixMapImage",
                      "Cannot lock memory block");
  else
    im->imageLocked = 1;

  return im->curImagePtr;
}


void  Nlm_UnlockPixMapImage(Nlm_Image image)
{
  Nlm_PImagePtr im = (Nlm_PImagePtr)image;
  if ( !im )
    return;

  Nlm_DiagReset();
  switch ( im->imageLocked ){
    case 0:
      break;
    case 1:
      HandUnlock ( im->image );
      im->imageLocked = 0;
      break;
    default:
      im->imageLocked--;
  }
}


void Nlm_ImageModified(Nlm_Image image)
{
  ((Nlm_PImagePtr)image)->imageVer++;
}


void Nlm_GetImageSize(Nlm_Image image, Nlm_Uint2Ptr width, Nlm_Uint2Ptr height)
{
  if ( width  )  *width  = ((Nlm_PImagePtr)image)->width;
  if ( height )  *height = ((Nlm_PImagePtr)image)->height;
}


Nlm_Boolean Nlm_ImageSetPalette(Nlm_Image image, Nlm_WindoW w)
{
  ((Nlm_PImagePtr)image)->curWin = w;
  return TRUE;
}

#ifdef WIN_MOTIF
static int ShiftXMask(unsigned long mask, int bitmap_unit)
{
    int low, hi;
    for(low = 0; low < bitmap_unit; low++) {
      if((mask >> low) & (unsigned long)1) break;
    }
    for (hi = 1; hi < bitmap_unit - low; hi++) {
        if(!((mask >> hi + low) & (unsigned long)1)) break;
    }
    return low - ( 8 - hi);  /* correct for default 8 bit palette */
}
#endif /* WIN_MOTIF */

Nlm_Boolean Nlm_ImageShow(Nlm_Image image, Nlm_PoinT p)
{
  register Nlm_PImagePtr im = (Nlm_PImagePtr)image;
#ifdef WIN_MAC
  Rect         rectSrc;
  Rect         rectDst;
  GrafPtr      port;
  CTabHandle   tabHandle;
  CTabPtr      tabPtr;
  Nlm_Int2     i;
#endif
#ifdef WIN_MOTIF
  XVisualInfo visinfo;
#endif /* WIN_MOTIF */


  Nlm_DiagReset();
  if ( !im->image ) {
    Nlm_DiagPutRecord(DA_ERROR, imageClass, "ImageShow", "Image is empty");
    return FALSE;
  }
#ifdef WIN_MSWIN
  if (im->imageColorVer != im->imageColorVerOld)
    {
      if ( im->curWin )
        Nlm_SetColorMap(im->curWin, im->totalColors,
                        im->red, im->green, im->blue);
      AllocateColorMap( im );
      if (im->hMemDC ) {
        DeleteDC( im->hMemDC );
        im->hMemDC = NULL;
      }
      im->imageColorVerOld = im->imageColorVer;
      im->imageVer = im->imageVerOld + 1;
    }

  if (im->imageVer != im->imageVerOld)
    {
      if ( !Nlm_LockPixMapImage(image) )
        return FALSE;
      if ( im->pixMap ) {
        DeleteObject( im->pixMap );
        im->pixMap = NULL;
      }
      im->pixMap = CreateDIBitmap(Nlm_currentHDC,
                                  (BITMAPINFOHEADER*)im->bitInfo,
                                  CBM_INIT, im->curImagePtr, im->bitInfo,
                                  DIB_RGB_COLORS);
      Nlm_UnlockPixMapImage( image );

      if ( !im->pixMap ) {
        Nlm_DiagPutRecord(DA_ERROR, imageClass, "ImageShow",
                          "Can not create pixmap");
        return FALSE;
      }

      if ( !im->hMemDC )
        im->hMemDC = CreateCompatibleDC( Nlm_currentHDC );
      if ( !im->hMemDC ) {
        Nlm_DiagPutRecord(DA_ERROR, imageClass, "ImageShow",
                          "Can not create DC");
        return FALSE;
      }

      SelectObject(im->hMemDC, im->pixMap);
      im->imageVerOld = im->imageVer;
    }

  BitBlt(Nlm_currentHDC,
         p.x, p.y, im->width, im->height, im->hMemDC, 0, 0, SRCCOPY);
#endif

#ifdef WIN_MOTIF
  {{
    if (im->imageColorVer != im->imageColorVerOld)
      {
        if (im->curWin != NULL)
          Nlm_SetColorMap(im->curWin, im->totalColors,
                          im->red, im->green, im->blue);
        else
          {
            int col;
            for (col = im->saveColors;  col < (int)im->totalColors;  col++)
              im->altcol[col] = Nlm_GetColorRGB(im->red  [col],
                                                im->green[col],
                                                im->blue [col]);
          }
        im->imageColorVerOld = im->imageColorVer;
      }

    if (im->curWin == NULL)
      {
        register int i, im_size = im->width * im->height;
        register Nlm_Uint1Ptr  altcol = im->altcol;
        register Nlm_Uint1Ptr  src    = im->curImagePtr;
        register Nlm_Uint1Ptr  dst    = (Nlm_Uint1Ptr)
          Nlm_MemGet(im_size, MGET_ERRPOST);

        im->imageX11->data = (char *)dst;
        if ( !im->imageX11->data )
          return FALSE;

        for (i = 0;  i < im_size;  i++)
          *dst++ = altcol[*src++];
      }
#ifdef OS_UNIX_LINUX
      else if (Nlm_CheckX(&visinfo)){
          Nlm_Int4 im_size, i, xx, yy;
          Nlm_Uint1Ptr src = im->curImagePtr;
          unsigned long dst;
          int red_shift, blue_shift, green_shift;
          
          im_size = (im->imageX11->height * im->imageX11->bytes_per_line
                  + im->imageX11->xoffset * im->imageX11->bitmap_unit/8);
          
          red_shift = ShiftXMask(im->imageX11->red_mask,
                                im->imageX11->bitmap_unit);
          green_shift = ShiftXMask(im->imageX11->green_mask,
                                im->imageX11->bitmap_unit);
          blue_shift = ShiftXMask(im->imageX11->blue_mask,
                                im->imageX11->bitmap_unit);
           
          if((visinfo.class == PseudoColor || visinfo.class == GrayScale)
                  && visinfo.depth == 16) {
            im->imageX11->data = (char *) Nlm_MemNew((size_t)im_size);
            for (xx = 0; xx < im->imageX11->width; xx++) {
              for( yy = 0;  yy < im->imageX11->height; yy++) {
                  i = yy * im->imageX11->width + xx;
                  XPutPixel(im->imageX11, xx, yy, (unsigned long)src[i]);
              }
            }
          }
          else if(visinfo.class == TrueColor) {
            im->imageX11->data = (char *) Nlm_MemNew((size_t)im_size);
            for (xx = 0; xx < im->imageX11->width; xx++) {
              for( yy = 0;  yy < im->imageX11->height; yy++) {
                  i = yy * im->imageX11->width + xx;
                  dst = (((red_shift > 0 ?
                          (((unsigned long)im->red[src[i]]) << red_shift) :
                          (((unsigned long)im->red[src[i]])>>-red_shift)) &
                          im->imageX11->red_mask) |
                          ((green_shift > 0 ?
                          (((unsigned long)im->green[src[i]]) << green_shift) :
                          (((unsigned long)im->green[src[i]])>>-green_shift)) &
                          im->imageX11->green_mask) |
                          ((blue_shift > 0 ?
                          (((unsigned long)im->blue[src[i]]) << blue_shift) :
                          (((unsigned long)im->blue[src[i]])>>-blue_shift)) &
                          im->imageX11->blue_mask) );
                  XPutPixel(im->imageX11, xx, yy, dst);           
              }
            }
          }
          else if((visinfo.class == PseudoColor || visinfo.class == GrayScale)
                  && visinfo.depth == 8) {
              dst = dst;  /* placeholder */
          }
          else {
            Nlm_MemFree(im->imageX11->data);
            Nlm_DiagPutRecord(DA_ERROR, imageClass, "ImageShow",
                          "Can't handle display class");
            return FALSE;
          }
      }
      else {
          Nlm_DiagPutRecord(DA_ERROR, imageClass, "ImageShow",
                          "Not valid display class");
          return FALSE;
      }
#endif /* OS_UNIX_LINUX */
          
    XPutImage(Nlm_currentXDisplay, Nlm_currentXWindow,
              Nlm_currentXGC, im->imageX11, 0, 0, p.x, p.y,
              im->imageX11->width, im->imageX11->height);

    if (im->curWin == NULL)
      {
        Nlm_MemFree( im->imageX11->data );
        im->imageX11->data = (char *)im->curImagePtr;
      }
#ifdef OS_UNIX_LINUX
      else if (Nlm_CheckX(&visinfo)) {
          if(!((visinfo.class == PseudoColor || visinfo.class == GrayScale)
                  && visinfo.depth == 8)) {
            MemFree(im->imageX11->data);
            im->imageX11->data = (char *)im->curImagePtr;
        }
      }
#endif /* OS_UNIX_LINUX */

    XFlush( Nlm_currentXDisplay );
  }}
#endif

#ifdef WIN_MAC
  if ( Nlm_LockPixMapImage(image) == NULL ) return FALSE;
  if ( im->imageColorVer != im->imageColorVerOld ){
    if ( im->curWin != NULL ){
      Nlm_SetColorMap ( im->curWin, im->totalColors,
                        im->red, im->green, im->blue );
    }
    tabHandle = im->pixelMap->pmTable;
    if ( tabHandle != NULL ) DisposeCTable ( tabHandle );
    im->pixelMap->pmTable = tabHandle = GetCTable(72);
    HLock ( (Nlm_Handle)tabHandle );
    tabPtr = *tabHandle;
    for ( i=0; i<(Nlm_Int2)im->totalColors; i++ ){
      tabPtr->ctTable[i].rgb.red   = (Nlm_Uint2)im->red  [i] << 8
        | (Nlm_Uint2)im->red[i];
      tabPtr->ctTable[i].rgb.green = (Nlm_Uint2)im->green[i] << 8
        | (Nlm_Uint2)im->green[i];
      tabPtr->ctTable[i].rgb.blue  = (Nlm_Uint2)im->blue [i] << 8
        | (Nlm_Uint2)im->blue[i];
      if (i >= 255) break;
    }
    tabPtr->ctSeed = GetCTSeed();
    HUnlock((Nlm_Handle)tabHandle );
    im->imageColorVerOld = im->imageColorVer;
  }
  im->pixelMap->baseAddr = (Ptr)im->curImagePtr;
  rectSrc.top = 0;   rectSrc.bottom = im->height;
  rectSrc.left = 0;  rectSrc.right = im->width;
  rectDst.top = p.y;   rectDst.bottom = im->height+p.y;
  rectDst.left = p.x;  rectDst.right = im->width+p.x;
  GetPort (&port);
  CopyBits((BitMap*)im->pixelMap,
           GetPortBitMapForCopyBits(port),
           &rectSrc, &rectDst, srcCopy, NULL);
  BackColor(blackColor);
  Nlm_UnlockPixMapImage(image);
#endif

  return TRUE;
}


Nlm_Uint1 Nlm_ImageGetBlack(Nlm_Image image)
{
  return (Nlm_Uint1)((Nlm_PImagePtr)image)->saveColors;
}

Nlm_Uint1 Nlm_ImageGetColorOffset(Nlm_Image image)
{
  return (Nlm_Uint1)(((Nlm_PImagePtr)image)->saveColors + 1);
}

Nlm_Uint1 Nlm_ImageGetColorMax(Nlm_Image image)
{
  return (Nlm_Uint1)(((Nlm_PImagePtr)image)->totalColors - 1);
}

