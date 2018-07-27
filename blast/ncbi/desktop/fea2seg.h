/*   fea2seg.h
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
* File Name:  fea2seg.h
*
* Author:  Denis Vakatov, Jinghui Zhang
*
* $Revision: 6.1 $
*
* File Description: 
*  Draw style-dependent feature images for the feature legend map
*  Draw gene images for the gene legend map
*  SmartDrawText() -- draws text in a rectangular area
*
* ----------------------------------------------------------------------
* $Log: fea2seg.h,v $
* Revision 6.1  1997/09/16 21:20:23  vakatov
* +SmartDrawText() description
*
* Revision 6.0  1997/08/25 18:21:59  madden
* Revision changed to 6.0
*
* Revision 1.4  1997/07/18 21:48:17  zjing
* make SmartDrawText() external
*
* Revision 1.3  1997/03/06 17:27:17  vakatov
* Added funcs gene2segment() and compose_g_legend() to draw gene legend map.
* Now use "Int4"(instead of "Int2") for all coords and dimensions.
*
* Revision 1.2  1996/12/11  16:51:56  vakatov
* <fstyle.h> moved to "fea2seg.c";
* NCBI header, log and other standard stuff added
* ----------------------------------------------------------------------
*/

#ifndef FEA2SEG_H
#define FEA2SEG_H

#include <picture.h>
#include <objgen.h>


#ifdef __cplusplus
extern "C" {
#endif

/*  Prepare image+label of the feature having ID "feature" --
 *  according to the given style "style_name";
 *  draw the result within the segment/picture "parent" starting
 *  from the ("left", "top") position.
 *  Parameters "label_width" &  "image_width" specify the feature's
 *  label and image widths, respectively.
 *  Height of the resulting segment will be written at the "height"
 *  pointer(only if the latter is not NULL).
 *  Return the resulting segment on success, NULL on error.
 */
extern SegmenT feature2segment PROTO((SegmenT parent,
                                      const Char PNTR style_name,
                                      Int2 feature,
                                      Int4 left, Int4 top,
                                      Int4 label_width, Int4 image_width,
                                      Int4Ptr height));

extern SegmenT pic_for_f_legend PROTO((ValNodePtr list,
                                       CharPtr style_name,
                                       Int4 image_width));



/*  Draw legend(image+label) of the gene according to the description
 *  given in "gene";
 *  place the result into the segment/picture "parent", starting
 *  from the ("left", "top") position.
 *  Parameter "label_width" specifies the maximum width of the
 *  gene legend label.
 *  Height of the resulting segment will be written at the "height"
 *  pointer(only if the latter is not NULL).
 *  Return the resulting segment on success, NULL on error.
 *  NOTE:  do not traverse the whole list;  draw the first gene's legend only
 */
extern SegmenT gene2segment PROTO((SegmenT parent,
                                   const UserField *gene,
                                   Int4 left, Int4 top,
                                   Int4 image_width, Int4 label_width,
                                   Int4Ptr height));


extern SegmenT compose_g_legend  PROTO((SegmenT     parent,
                                        const Char *title,
                                        FonT        title_font,
                                        const UserObject *genes,
                                        Int4 left, Int4 top,
                                        Int4 max_width, Int4Ptr height));

/* Print/Wrap the text "text" with font "font" starting from
 * the ("left", "top") position in segment "seg" so that
 * the printed text would fit into a box having maximum dimensions
 * "*width" and "*height".  If "*height" <= 0 then the Y-dimension
 * is not limited.
 * Set "*width" to the actual maximum width and "*height"(if non-NULL)
 * to the actual height of the drawn text.
 * Return FALSE if no text printed out. 
 */
extern Boolean SmartDrawText PROTO((SegmenT seg,
                             const Char *text, FonT font,
                             Int4 left, Int4 top,
                             Int4Ptr width, Int4Ptr height));

#ifdef __cplusplus
}
#endif

#endif  /* FEA2SEG_H */

