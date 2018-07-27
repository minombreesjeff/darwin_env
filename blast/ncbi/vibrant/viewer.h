/*   viewer.h
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
* File Name:  viewer.h
*
* Author:  Jonathan Kans, Alex Smirnov, Jill Shermer
*
* Version Creation Date:   10/25/92
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
* $Log: viewer.h,v $
* Revision 6.2  2002/03/20 22:10:52  kans
* set small/medium/large font prototyped in drawingp.h and viewer.h
*
* Revision 6.1  1997/11/17 23:56:32  kans
* added PictureHasEnlarged, call on scroll if PictureGrew dynamically
*
* Revision 6.0  1997/08/25 18:58:13  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/01/16 22:04:39  vakatov
* Added proto for CopyAllViewer()
*
 * Revision 5.1  1996/12/13  17:10:07  vakatov
 * +Nlm_PrintAllViewer()
 *
 * Revision 5.0  1996/05/28  13:45:08  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:51:04  ostell
 * force revision to 4.0
 *
 * Revision 1.24  1995/05/17  15:15:14  kans
 * added Log line
 *
*
* ==========================================================================
*/

#ifndef _VIEWER_
#define _VIEWER_

#ifndef _VIBRANT_
#include <vibrant.h>
#endif

#ifndef _PICTURE_
#include <picture.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   STRUCTURE TYPEDEFS
*
*****************************************************************************/

typedef struct viewer {
  Nlm_VoidPtr  dummy;
} HNDL Nlm_VieweR;

/*  Viewer callback type */

typedef void (*Nlm_VwrClckProc) PROTO((Nlm_VieweR, Nlm_SegmenT, Nlm_PoinT));
typedef void (*Nlm_VwrPanProc) PROTO((Nlm_VieweR, Nlm_SegmenT));
typedef void (*Nlm_VwrDrawProc) PROTO((Nlm_VieweR, Nlm_SegmenT));
typedef void (*Nlm_VwrFreeProc) PROTO ((Nlm_VieweR, Nlm_VoidPtr));

/*****************************************************************************
*
*   FUNCTION PROTOTYPES
*
*****************************************************************************/

extern Nlm_VieweR  Nlm_CreateViewer       PROTO((Nlm_GrouP prnt,
Nlm_Int2 width, Nlm_Int2 height, Nlm_Boolean vscroll, Nlm_Boolean hscroll));
extern void        Nlm_ResetViewer        PROTO((Nlm_VieweR viewer));
extern Nlm_VieweR  Nlm_DeleteViewer       PROTO((Nlm_VieweR viewer));
extern void        Nlm_AttachPicture      PROTO((Nlm_VieweR viewer,
Nlm_SegmenT picture, Nlm_Int4 pntX, Nlm_Int4 pntY, Nlm_Int2 align,
Nlm_Int4 scaleX, Nlm_Int4 scaleY, Nlm_VwrDrawProc draw));
extern void        Nlm_AttachInstance     PROTO((Nlm_VieweR viewer,
Nlm_SegmenT picture, Nlm_Int4 pntX, Nlm_Int4 pntY, Nlm_Int2 align,
Nlm_Int4 scaleX, Nlm_Int4 scaleY, Nlm_VwrDrawProc draw, Nlm_VoidPtr data,
Nlm_VwrFreeProc cleanup));
extern void        Nlm_EstimateScaling    PROTO((Nlm_VieweR viewer,
Nlm_SegmenT picture, Nlm_Int4 pntX, Nlm_Int4 pntY, Nlm_Int2 align,
Nlm_Int4Ptr scaleX, Nlm_Int4Ptr scaleY));
extern void        Nlm_ViewerWasResized   PROTO((Nlm_VieweR viewer));
extern void        Nlm_PictureHasEnlarged PROTO((Nlm_VieweR viewer));
extern void        Nlm_SetViewerProcs     PROTO((Nlm_VieweR viewer,
Nlm_VwrClckProc click, Nlm_VwrClckProc drag, Nlm_VwrClckProc release,
Nlm_VwrPanProc pan));
extern void        Nlm_SetViewerData      PROTO((Nlm_VieweR viewer,
Nlm_VoidPtr data, Nlm_VwrFreeProc cleanup));
extern Nlm_VoidPtr Nlm_GetViewerData      PROTO((Nlm_VieweR viewer));
extern void        Nlm_ViewerBox          PROTO((Nlm_VieweR viewer,
BoxPtr world, BoxPtr port, RectPtr view, Nlm_Int4Ptr scaleX,
Nlm_Int4Ptr scaleY));
extern void        Nlm_PrintViewer        PROTO((Nlm_VieweR viewer));
extern void        Nlm_PrintAllViewer     PROTO((Nlm_VieweR viewer));
extern void        Nlm_CopyViewer         PROTO((Nlm_VieweR viewer));
extern void        Nlm_CopyAllViewer      PROTO((Nlm_VieweR viewer));
extern Nlm_SegmenT Nlm_FindSegPrim        PROTO((Nlm_VieweR viewer, Nlm_PoinT pt,
Nlm_Uint2Ptr segIDPtr, Nlm_Uint2Ptr primIDPtr, PrimitivE PNTR primPtr));
extern Nlm_SegmenT Nlm_FindSegment        PROTO((Nlm_VieweR viewer,
Nlm_PoinT pt, Nlm_Uint2Ptr segID, Nlm_Uint2Ptr primID, Nlm_Uint2Ptr primCt));
extern void        Nlm_ShowSegment        PROTO((Nlm_VieweR viewer,
Nlm_SegmenT segment));
extern void        Nlm_HideSegment        PROTO((Nlm_VieweR viewer,
Nlm_SegmenT segment));
extern void        Nlm_HighlightSegment   PROTO((Nlm_VieweR viewer,
Nlm_SegmenT segment, Nlm_Int1 highlight));

extern void        Nlm_HighlightPrimitive PROTO((Nlm_VieweR viewer,
Nlm_SegmenT segment, Nlm_PrimitivE primitive, Nlm_Int1 highlight));
extern void        Nlm_ResetHighlight     PROTO((Nlm_VieweR viewer,
Nlm_SegmenT segment));

extern void        Nlm_MapWorldToViewer   PROTO((Nlm_VieweR viewer,
Nlm_PntInfo pnt, Nlm_PointPtr pt));
extern void        Nlm_MapViewerToWorld   PROTO((Nlm_VieweR viewer,
Nlm_PoinT pt, PntPtr pnt));

extern Nlm_FonT Nlm_SetSmallFont  PROTO((void));
extern Nlm_FonT Nlm_SetMediumFont PROTO((void));
extern Nlm_FonT Nlm_SetLargeFont  PROTO((void));

#define VieweR Nlm_VieweR
#define VwrClckProc Nlm_VwrClckProc
#define VwrPanProc Nlm_VwrPanProc
#define VwrDrawProc Nlm_VwrDrawProc
#define VwrFreeProc Nlm_VwrFreeProc
#define CreateViewer Nlm_CreateViewer
#define ResetViewer Nlm_ResetViewer
#define DeleteViewer Nlm_DeleteViewer
#define AttachPicture Nlm_AttachPicture
#define AttachInstance Nlm_AttachInstance
#define EstimateScaling Nlm_EstimateScaling
#define ViewerWasResized Nlm_ViewerWasResized
#define PictureHasEnlarged Nlm_PictureHasEnlarged
#define SetViewerProcs Nlm_SetViewerProcs
#define SetViewerData Nlm_SetViewerData
#define GetViewerData Nlm_GetViewerData
#define ViewerBox Nlm_ViewerBox
#define PrintViewer Nlm_PrintViewer
#define PrintAllViewer Nlm_PrintAllViewer
#define CopyViewer Nlm_CopyViewer
#define CopyAllViewer Nlm_CopyAllViewer
#define FindSegPrim Nlm_FindSegPrim
#define FindSegment Nlm_FindSegment
#define ShowSegment Nlm_ShowSegment
#define HideSegment Nlm_HideSegment
#define HighlightSegment Nlm_HighlightSegment
#define HighlightPrimitive Nlm_HighlightPrimitive
#define ResetHighlight Nlm_ResetHighlight
#define MapWorldToViewer Nlm_MapWorldToViewer
#define MapViewerToWorld Nlm_MapViewerToWorld
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
