/*  imagelst.h
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
* File Name:  imagelst.h
*
* Author:  Vladimir Soussov
*   
* File Description:  Data types for image list
*
*
* $Log: imagelst.h,v $
* Revision 1.1  1998/03/31 21:22:24  sirotkin
* With Vladimir - moved from distrib/internal/taxonomy/tree
*
* Revision 6.0  1997/08/25 18:29:45  madden
* Revision changed to 6.0
*
* Revision 1.1  1997/05/30 16:16:15  soussov
* Set of files for ncbitree library
*
* Revision 1.1  1997/05/29 20:44:24  soussov
* Initial version of tree library
*
*
*/

#ifndef IMAGELST_H_DONE
#define IMAGELST_H_DONE

#include <ncbi.h>
#include <vibrant.h>
#include "image.h"

#define NOF_IMG_DEFAULT 16;

typedef enum {
    IMAGE_GIF = 0,
    IMAGE_BMP
} ImageFileFormat;

typedef struct t_ImageList {
    Uint2 nof_images;
    Uint2 img_size_x, img_size_y;
    Image *icon;
} _ImageList, PNTR _ImageListPtr;

/*****************************************************
 * Create new image list
 * n - approximate number of images in this list (0 means we don't know)
 * size_x, size_y - image size (0 means that the image size can be variable)
 */
_ImageListPtr ilst_new(Uint2 n, Uint2 size_x, Uint2 size_y);

/*****************************************************
 * free image list
 */
void ilst_free(_ImageListPtr il);

/*****************************************************
 * add image to image list
 * this function adds new image to image list using either img or file
 */
Uint2 ilst_add(_ImageListPtr il, Image img, CharPtr fileName, ImageFileFormat f);

/*****************************************************
 * replace image in image list
 */
Boolean ilst_replace(_ImageListPtr il, Uint2 i_ind, Image img, CharPtr fileName, ImageFileFormat f);

/*****************************************************
 * remove image from the list
 */
void ilst_remove(_ImageListPtr il, Uint2 i_ind);

/*****************************************************
 * get image size
 */
Boolean ilst_size(_ImageListPtr il, Uint2 i_ind, Uint2Ptr w, Uint2Ptr h);

/*****************************************************
 * draw image
 */
Boolean ilst_draw(_ImageListPtr il, Uint2 i_ind, Int2 x, Int2 y);


#endif
