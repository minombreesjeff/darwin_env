/*   ncbigif.h
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
* File Name:  ncbigif.h
*
* Author:  Alex Smirnov
*
* Version Creation Date:   10/20/95
*
* $Revision: 6.2 $
*
* File Description: 
*       GIF generator header file
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ncbigif.h,v $
* Revision 6.2  1999/10/20 23:13:59  vakatov
* + Nlm_Picture2gdImage() -- to draw in gdImage.
* Nlm_PictureToGIF() -- restore current GIF image (if any) when the
* dump is complete.
*
* Revision 6.1  1998/07/02 18:19:28  vakatov
* Cleaned the code & made it pass through the C++ compilation
*
* ==========================================================================
*/


#ifndef NCBIGIF_H
#define NCBIGIF_H 1

#include <picture.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Dump a picture to GIF file
 */
NLM_EXTERN Boolean Nlm_PictureToGIF
(FILE* out, Int2 gifWidth, Int2 gifHeight,
 SegmenT picture, Int4 pntX, Int4 pntY,
 Int2 align, Int4 scaleX, Int4 scaleY, 
 Boolean transparent);

NLM_EXTERN Boolean Nlm_Picture2gdImage
(struct gdImageStruct* im, SegmenT picture,
 Int4 pntX, Int4 pntY, Int2 align, Int4 scaleX, Int4 scaleY);

NLM_EXTERN SegmenT Nlm_FindPrimGIF
(SegmenT picture, PoinT pt, Int2 gifWidth,
 Int2 gifHeight, Int4 pntX, Int4 pntY,
 Int2 align, Int4 scaleX, Int4 scaleY,
 Uint2Ptr segIDPtr, Uint2Ptr primIDPtr, 
 PrimitivE PNTR primPtr);

NLM_EXTERN SegmenT Nlm_FindSegGIF
(SegmenT picture, PoinT pt, Int2 gifWidth,
 Int2 gifHeight, Int4 pntX, Int4 pntY,
 Int2 align, Int4 scaleX, Int4 scaleY,
 Uint2Ptr segIDPtr, Uint2Ptr primIDPtr, 
 Uint2Ptr primCtPtr);


#define PictureToGIF    Nlm_PictureToGIF
#define Picture2gdImage Nlm_Picture2gdImage
#define FindPrimGIF     Nlm_FindPrimGIF
#define FindSegGIF      Nlm_FindSegGIF 
                 
#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* NCBIGIF_H */
