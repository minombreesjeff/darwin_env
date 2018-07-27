/*   image.h
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
* File Name:  image.h
*
* Author:  Alex Smirnov
*
* Version Creation Date:   03/29/95
*
* $Revision: 6.0 $
*
* File Description: 
*  image public header file.
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: image.h,v $
* Revision 6.0  1997/08/25 18:55:48  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/05/28 18:30:19  vakatov
* Fixed typo in #ImageGetColorMax
*
* Revision 5.1  1996/11/22 19:46:34  vakatov
* Changed prototypes of Nlm_LoadImageGIF(), Nlm_LoadImageBMP() and
* Nlm_ImageGetBlack();  added Nlm_ImageGetColorOffset() and
* Nlm_ImageGetColorMax()
*
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 1.3  1996/05/07  18:38:24  vakatov
 * RCS log added
 *
* ==========================================================================
*/

#ifndef _IMAGE_
#define _IMAGE_

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   DEFINES
*
*****************************************************************************/

/*****************************************************************************
*
*   TYPEDEFS
*
*****************************************************************************/
typedef struct Nlm_image {
  Nlm_VoidPtr  dummy;
}PNTR Nlm_Image;

/*****************************************************************************
*
*   EXTERN FUNCTION
*
*****************************************************************************/
extern Nlm_Image     Nlm_CreateImage       PROTO((void));
extern Nlm_Boolean   Nlm_AllocateImage     PROTO((Nlm_Image image, Nlm_Uint2Ptr width, 
                                    Nlm_Uint2 height, Nlm_Uint2 saveColors,
                                    Nlm_Uint2 totalColors));
extern Nlm_Image     Nlm_LoadImageGIF      PROTO((Nlm_CharPtr fileName));
extern Nlm_Image     Nlm_LoadImageBMP      PROTO((Nlm_CharPtr fileName));
extern Nlm_Boolean   Nlm_LoadImageClip     PROTO((Nlm_Image image)); 
extern Nlm_Boolean   Nlm_SaveImageGIF      PROTO((Nlm_Image image,
                                    Nlm_CharPtr fileName));
extern Nlm_Boolean   Nlm_SaveImageBMP      PROTO((Nlm_Image image,
                                    Nlm_CharPtr fileName));
extern Nlm_Boolean   Nlm_SaveImageClip     PROTO((Nlm_Image image));
extern Nlm_Boolean   Nlm_PrintImage        PROTO((Nlm_Image image));
extern void          Nlm_DeleteImage       PROTO((Nlm_Image image));
extern Nlm_Boolean   Nlm_SetColorImage     PROTO((Nlm_Image image,
                                    Nlm_Uint1 color, 
                                    Nlm_Uint1 red, Nlm_Uint1 green, Nlm_Uint1 blue));
extern Nlm_Boolean   Nlm_GetColorImage     PROTO((Nlm_Image image,
                                    Nlm_Uint1 color, 
                                    Nlm_Uint1Ptr red, Nlm_Uint1Ptr green, 
                                    Nlm_Uint1Ptr blue));
extern Nlm_Uint1Ptr  Nlm_LockPixMapImage     PROTO((Nlm_Image image));
extern void          Nlm_UnlockPixMapImage   PROTO((Nlm_Image image));
extern void          Nlm_ImageModified       PROTO((Nlm_Image image));
extern void          Nlm_GetImageSize        PROTO((Nlm_Image image,
                                    Nlm_Uint2Ptr width, Nlm_Uint2Ptr height));
extern Nlm_Boolean   Nlm_ImageSetPalette     PROTO((Nlm_Image image,
                                    Nlm_WindoW w));
extern Nlm_Boolean   Nlm_ImageShow           PROTO((Nlm_Image image,
                                    Nlm_PoinT p));
extern Nlm_Uint1     Nlm_ImageGetBlack       PROTO((Nlm_Image image));
extern Nlm_Uint1     Nlm_ImageGetColorOffset PROTO((Nlm_Image image));
extern Nlm_Uint1     Nlm_ImageGetColorMax    PROTO((Nlm_Image image));

/*****************************************************************************
*
*   NAME REDIFINITION
*
*****************************************************************************/
#define Image             Nlm_Image
#define CreateImage       Nlm_CreateImage
#define AllocateImage     Nlm_AllocateImage
#define LoadImageGIF      Nlm_LoadImageGIF
#define LoadImageBMP      Nlm_LoadImageBMP
#define LoadImageClip     Nlm_LoadImageClip
#define SaveImageGIF      Nlm_SaveImageGIF
#define SaveImageBMP      Nlm_SaveImageBMP
#define SaveImageClip     Nlm_SaveImageClip
#define PrintImage        Nlm_PrintImage
#define DeleteImage       Nlm_DeleteImage
#define SetColorImage     Nlm_SetColorImage
#define GetColorImage     Nlm_GetColorImage
#define LockPixMapImage   Nlm_LockPixMapImage
#define UnlockPixMapImage Nlm_UnlockPixMapImage
#define ImageModified     Nlm_ImageModified
#define GetImageSize      Nlm_GetImageSize
#define ImageSetPalette   Nlm_ImageSetPalette
#define ImageShow         Nlm_ImageShow
#define ImageGetBlack     Nlm_ImageGetBlack
#define ImageGetColorOffset Nlm_ImageGetColorOffset
#define ImageGetColorMax  Nlm_ImageGetColorMax

#ifdef __cplusplus
}
#endif

#endif

