/*   drawingP.h
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
* File Name:  drawingP.h
*
* Author:  Jonathan Kans, Alex Smirnov, Jill Shermer
*
* Version Creation Date:   11/13/92
*
* $Revision: 6.2 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: drawingp.h,v $
* Revision 6.2  2002/03/20 22:10:52  kans
* set small/medium/large font prototyped in drawingp.h and viewer.h
*
* Revision 6.1  1999/10/04 17:16:32  kans
* include ncbidraw.h instead of vibrant.h, a couple Nlm_ prefixes
*
* Revision 6.0  1997/08/25 18:55:43  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:45:08  ostell
* Set to revision 5.0
*
 * Revision 4.1  1995/11/07  23:21:53  kans
 * moved Nlm_DrawSegment from viewer.c to drawing.c (for GIF without Vibrant)
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 1.12  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _DRAWINGP_
#define _DRAWINGP_

#ifndef _NCBIDRAW_
#include <ncbidraw.h>
#endif

#ifndef _PICTURE_
#include <picture.h>
#endif

#ifndef _PICTUREP_
#include <picturep.h>
#endif

#ifndef _MAPPINGP_
#include <mappingp.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   EXTERNAL VARIABLES
*
*****************************************************************************/

extern Nlm_FonT  Nlm_smallFont;
extern Nlm_FonT  Nlm_mediumFont;
extern Nlm_FonT  Nlm_largeFont;

extern AttPData blackAttPData;
extern AttPData whiteAttPData;

/*****************************************************************************
*
*   FUNCTION PROTOTYPES
*
*****************************************************************************/

extern void Nlm_DrawPrimitive PROTO((Nlm_BasePPtr item, 
Nlm_DrawInfoPtr drawinfo));
extern Nlm_Boolean Nlm_PrimitiveIsCloseToPoint PROTO((Nlm_BasePPtr item, 
Nlm_ScalePtr scale));
extern Nlm_Boolean Nlm_TryHighlightPrimitive PROTO((Nlm_BasePPtr item, 
Nlm_Int1 highlight));
extern Nlm_Boolean Nlm_TryOffsetPrimitive PROTO((Nlm_BasePPtr item, 
Nlm_Int4 deltaX, Nlm_Int4 deltaY));
extern Nlm_Boolean Nlm_TryGetPrimitiveLimits PROTO((Nlm_BasePPtr item, 
Nlm_Int4 scaleX, Nlm_Int4 scaleY, Nlm_BoxPtr pLimits));
extern void Nlm_CleanupPrimitive PROTO((Nlm_BasePPtr item));

extern void Nlm_DrawSegment PROTO((SegPPtr seg, Nlm_DrawInfoPtr drawinfoPtr ));

extern Nlm_FonT Nlm_SetSmallFont  PROTO((void));
extern Nlm_FonT Nlm_SetMediumFont PROTO((void));
extern Nlm_FonT Nlm_SetLargeFont  PROTO((void));

#define smallFont Nlm_smallFont
#define mediumFont Nlm_mediumFont
#define largeFont Nlm_largeFont
#define DrawPrimitive Nlm_DrawPrimitive

#define PrimitiveIsCloseToPoint Nlm_PrimitiveIsCloseToPoint
#define TryHighlightPrimitive Nlm_TryHighlightPrimitive
#define TryOffsetPrimitive Nlm_TryOffsetPrimitive
#define TryGetPrimitiveLimits Nlm_TryGetPrimitiveLimits
#define CleanupPrimitive Nlm_CleanupPrimitive
#ifndef SetSmallFont
#define SetSmallFont Nlm_SetSmallFont
#endif
#ifndef SetMediumFont
#define SetMediumFont Nlm_SetMediumFont
#endif
#ifndef SetLargeFont
#define SetLargeFont Nlm_SetLargeFont
#endif

#ifdef __cplusplus
}
#endif

#endif
