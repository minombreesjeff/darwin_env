/*  $Id: ddvgraph.h,v 1.14 2000/07/24 22:00:08 hurwitz Exp $
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
* File Name:  ddvgraph.h
*
* Author:  Patrick Durand
*
* Version Creation Date:   06/19/99
*
* $Revision: 1.14 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvgraph.h,v $
* Revision 1.14  2000/07/24 22:00:08  hurwitz
* fixed bug that 1/2 characters were displayed sometimes
*
* Revision 1.13  2000/07/08 20:43:58  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 1.12  2000/05/16 19:43:01  hurwitz
* grey out create block, delete block, undo, and redo as needed
*
* Revision 1.11  2000/02/15 15:31:47  lewisg
* move DDVRulerDescr to pgppop
*
* Revision 1.10  2000/02/03 15:56:47  hurwitz
* added constructor and destructor for DDVRulerDescrPtr
*
* Revision 1.9  2000/01/26 13:38:54  durand
* update the GUI for the editor. Add functions to create the data to be used by the editor
*
* Revision 1.8  1999/11/18 14:37:15  durand
* avoid flashing sequence during selection
*
* Revision 1.7  1999/11/09 17:09:00  durand
* transfer some functions from ddvgraph to ddvcreate, so that ddvcreate remains Vibrant free and can be compiled with BLAST
*
* Revision 1.6  1999/11/03 21:29:49  durand
* add CTRL and SHFT keys for mouse selection. redesign the loader functions of DDV to properly register the message callbacks
*
* Revision 1.5  1999/10/29 14:15:39  durand
* add simple mouse selection functions
*
* Revision 1.4  1999/10/20 13:17:18  durand
* add display for disc. SeqAlign tails
*
* Revision 1.3  1999/10/15 21:57:36  durand
* add a UI for display options
*
* Revision 1.2  1999/10/12 15:07:52  durand
* resolve problems with CVS
*
* Revision 1.1  1999/09/30 14:10:27  durand
* add ddv to toolkit
*
* Revision 1.7  1999/09/22 20:40:20  durand
* update the drawing procedure to deal with discontinuous seqalign
*
* Revision 1.6  1999/09/09 21:54:24  durand
* create a display for disconitnuous SeqAlign
*
* Revision 1.5  1999/08/04 18:01:51  wheelan
* changes to support new seqalign indexing
*
* Revision 1.4  1999/07/27 13:11:44  durand
* transfer defines to udvdef.h
*
* Revision 1.3  1999/07/20 14:58:01  durand
* use the Color Manager to display colored MSA
*
* Revision 1.2  1999/06/21 20:33:28  durand
* add display type
*
* Revision 1.1  1999/06/19 17:21:07  durand
* add Vibrant DDV code
*
*
*
* ==========================================================================
*/

#ifndef _DDVGRAPH_
#define _DDVGRAPH_

#include <udviewer.h>
#include <udvdef.h>
#include <ddvmain.h>
#include <ddvcolor.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************

	structures

******************************************************************************/

/******************************************************************************

	defines

******************************************************************************/

/******************************************************************************

	Exported functions

******************************************************************************/

extern Int2 DDV_ComputeColWidth(Int2 cxChar);
extern void  DDV_DrawPanelContent (PaneL p,DdvMainPtr dmp);
extern void  DDV_DrawViewer (PaneL p);
extern void DDV_InvalRegion(PaneL hWndDDV,UnDViewerGraphDataPtr GrData,
		Int4 disp_from,Int4 disp_to,Int4 disp_row,Boolean IsSelect);
extern void DDV_GetCurrentDispRange(PaneL hWndDDV,UnDViewerGraphDataPtr GrData,
		Int4 LengthAli,Int4Ptr from_col,Int4Ptr to_col,Int4Ptr from_row,
		Int4Ptr to_row);
NLM_EXTERN ValNodePtr DDV_GetRulerForEditor(ValNodePtr descr_head,Int4 from_disp,
		Int4 to_disp);
extern ValNodePtr DDV_ComputeRuler(SeqAlignPtr sap,DDV_Disp_OptPtr ddop);
extern void	DDV_AdjustDrawingRect(RecT * rcP,UDVFontDataPtr udv_font, DdvMainPtr dmp);
extern void DDV_DrawPanelContent_H (PaneL p,DdvMainPtr dmp,RecT PNTR MyUpdateRect,
		Boolean bSelect);
extern void DDV_GreyOut(DdvMainWinPtr mWin_d, Boolean Start, Boolean End);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* ndef _DDVGRAPH_ */
