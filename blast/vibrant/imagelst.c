/*  imagelst.c
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
* File Name:  imagelst.c
*
* Author:  Vladimir Soussov
*   
* File Description:  image list
*
*
* $Log: imagelst.c,v $
* Revision 1.4  2001/03/28 01:33:52  juran
* Added "continue" to "for (...);" to avoid warnings.
*
* Revision 1.3  1998/07/02 18:24:32  vakatov
* Cleaned the code & made it pass through the C++ compilation
*
* Revision 1.2  1998/04/01 17:44:03  soussov
* changed tp include <>
*
* Revision 1.1  1998/03/31 21:22:23  sirotkin
* With Vladimir - moved from distrib/internal/taxonomy/tree
*
* Revision 6.0  1997/08/25 18:29:43  madden
* Revision changed to 6.0
*
* Revision 1.2  1997/06/03 17:59:45  soussov
* Minor changes for SGI C-compiler
*
* Revision 1.1  1997/05/30 16:16:13  soussov
* Set of files for ncbitree library
*
* Revision 1.1  1997/05/29 20:44:22  soussov
* Initial version of tree library
*
*
*/

#include <imagelst.h>

_ImageListPtr ilst_new(Uint2 n, Uint2 size_x, Uint2 size_y)
{
    _ImageListPtr il;
    Uint2 i;

    il= (_ImageListPtr) MemNew(sizeof(_ImageList));
    if(il == NULL) return NULL;
    if(n == 0) n= NOF_IMG_DEFAULT;

    if((il->icon= (Image*) MemNew(n*sizeof(Image))) == NULL) {
	MemFree(il);
	return NULL;
    }

    il->nof_images= n;
    il->img_size_x= size_x;
    il->img_size_y= size_y;

    for (i = 0; i < n; il->icon[i++] = NULL) continue;

    return il;
}

/*****************************************************
 * free image list
 */
void ilst_free(_ImageListPtr il)
{
    Uint2 i;

    for(i= 0; i < il->nof_images; i++) {
	if(il->icon[i] != NULL) {
	    DeleteImage(il->icon[i]);
	}
    }

    MemFree(il->icon);
    MemFree(il);
}

/*****************************************************
 * add image to image list
 * this function adds new image to image list using either img or file
 */
Uint2 ilst_add(_ImageListPtr il, Image img, CharPtr fileName, ImageFileFormat f)
{
    Uint2 i, k;

    for(i= 0; i < il->nof_images; i++) {
	if(il->icon[i] == NULL) break;
    }

    if(i >= il->nof_images) {
	/* we need more memory */
	i= il->nof_images;
	il->nof_images+= i/2;
	if ((il->icon= (Image*) MemMore(il->icon, il->nof_images*sizeof(Image))) == NULL) return 0xFFFF;
	for(k= i; k < il->nof_images; il->icon[k++]= NULL) continue;
    }


    if((img == NULL) && (fileName != NULL)) {
	switch(f) {
	case IMAGE_GIF: img= LoadImageGIF(fileName); break;
	case IMAGE_BMP: img= LoadImageBMP(fileName); break;
	}
    }

    il->icon[i]= img;
    return i;
}

/*****************************************************
 * replace image in image list
 */
Boolean ilst_replace(_ImageListPtr il, Uint2 i_ind, Image img, CharPtr fileName, ImageFileFormat f)
{

    if(i_ind >= il->nof_images) return FALSE;

    if(il->icon[i_ind] != NULL) DeleteImage(il->icon[i_ind]);

    if((img == NULL) && (fileName != NULL)) {
	switch(f) {
	case IMAGE_GIF: img= LoadImageGIF(fileName); break;
	case IMAGE_BMP: img= LoadImageBMP(fileName); break;
	}
    }

    il->icon[i_ind]= img;

    return TRUE;
}

/*****************************************************
 * remove image from the list
 */
void ilst_remove(_ImageListPtr il, Uint2 i_ind)
{
    if((il != NULL) && (i_ind < il->nof_images) && (il->icon[i_ind] != NULL)) {
	DeleteImage(il->icon[i_ind]);
	il->icon[i_ind] = NULL;
    }
}
    
/*****************************************************
 * get image size
 */
Boolean ilst_size(_ImageListPtr il, Uint2 i_ind, Uint2Ptr x, Uint2Ptr y)
{
    if(il == (_ImageListPtr)0x8) {
	*x= *y= 8;
    }
    else if(il != NULL) {
	*x= il->img_size_x;
	*y= il->img_size_y;
    }

    if((*x != 0) && (*y != 0)) return TRUE;

    if((il != NULL) && (i_ind < il->nof_images) && (il->icon[i_ind] != NULL)) {
	GetImageSize(il->icon[i_ind], x, y);
	return TRUE;
    }

    return FALSE;
}

static void draw_check(Int2 x0, Int2 y0)
{
    Black();
    Solid();
    MoveTo(x0, y0+3);
    LineTo(x0+1, y0+3);
    LineTo(x0+1, y0+5);
    MoveTo(x0+2, y0+4);
    LineTo(x0+2, y0+7);
    LineTo(x0+3, y0+7);
    LineTo(x0+3, y0+5);
    LineTo(x0+4, y0+5);
    LineTo(x0+4, y0+3);
    MoveTo(x0+5, y0+4);
    LineTo(x0+5, y0+2);
    MoveTo(x0+6, y0+1);
    LineTo(x0+6, y0+3);
    MoveTo(x0+7, y0);
    LineTo(x0+7, y0+1);
}

static Boolean draw8(Uint2 ind, Int2 x, Int2 y)
{
	draw_check(x, y);
	return TRUE;
}

/*****************************************************
 * draw image
 */
Boolean ilst_draw(_ImageListPtr il, Uint2 i_ind, Int2 x, Int2 y)
{
    if(il == (_ImageListPtr)0x8) return draw8(i_ind, x, y);

    if((il != NULL) && (i_ind < il->nof_images) && (il->icon[i_ind] != NULL)) {
	PoinT p;

	p.x= x;
	p.y= y;

	ImageShow(il->icon[i_ind], p);

	return TRUE;
    }
    return FALSE;
}

	
    
    
