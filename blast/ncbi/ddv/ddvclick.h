/*  $Id: ddvclick.h,v 1.10 2000/07/12 15:38:08 hurwitz Exp $
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
* File Name:  ddvclick.h
*
* Author:  Patrick Durand
*
* Version Creation Date:   09/20/99
*
* $Revision: 1.10 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvclick.h,v $
* Revision 1.10  2000/07/12 15:38:08  hurwitz
* made rectangle select much faster.  it's almost working.
*
* Revision 1.9  2000/07/08 20:43:58  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 1.8  2000/07/05 19:23:13  lewisg
* add two panes to ddv, update msvc project files
*
* Revision 1.7  2000/07/05 18:42:16  hurwitz
* added split block function to DDV
*
* Revision 1.6  2000/05/05 20:24:13  hurwitz
* some bug fixes, also redraw proper block in DDE after a save operation that causes a merge of 2 blocks
*
* Revision 1.5  2000/05/04 22:43:38  hurwitz
* don't launch DDE on top of DDV, change some wording, redraw DDE after save to AlnMgr
*
* Revision 1.4  2000/04/05 20:52:35  hurwitz
* added GUI control for shifting left and right alignment boundaries
*
* Revision 1.3  1999/12/07 21:40:14  durand
* add mouse modes menu and caret facility for the editor
*
* Revision 1.2  1999/10/29 14:15:39  durand
* add simple mouse selection functions
*
* Revision 1.1  1999/09/30 14:10:25  durand
* add ddv to toolkit
*
* Revision 1.2  1999/09/22 20:41:18  durand
* add mouse-click and drag functions
*
* Revision 1.1  1999/09/21 14:20:00  durand
* add basic mouse management functions
*
*
*
*
* ==========================================================================
*/

#ifndef _DDVCLICK_
#define _DDVCLICK_

#include <ddvmain.h>

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
/*identifiers of the regions within a ParaG*/
#define PGP_REGION_NOWHERE ((Uint1)1)
#define PGP_REGION_SEQ ((Uint1)2)
#define PGP_REGION_FEAT ((Uint1)3)

/******************************************************************************

	Exported functions

******************************************************************************/
NLM_EXTERN void DDV_DispPositionInStatus(PrompT InfoPanel,Int4 bsp_pos,
		Int4 line_num,CharPtr szSeqName);
NLM_EXTERN void DDV_GetSeqNameGivenRow(ValNodePtr * ParaG_List, Int4 row_num,
	CharPtr szSeqName);
NLM_EXTERN void DDV_ClickProcUpper(PaneL p, PoinT pt);
NLM_EXTERN void DDV_ClickProcLower(PaneL p, PoinT pt);
NLM_EXTERN void DDV_ClickProc(PaneL p, PoinT pt);
NLM_EXTERN void DDV_DragProc(PaneL p, PoinT pt);
NLM_EXTERN void DDV_HoldProc(PaneL p, PoinT pt);
NLM_EXTERN void DDV_ReleaseProc(PaneL p, PoinT pt);
NLM_EXTERN void DDV_KeyboardProc (SlatE s, Char ch);
NLM_EXTERN Int4 DDV_GetHPixelPosGivenColNumber(DdvMainPtr dmp, RecT rc, Int4 Col);
NLM_EXTERN Int4 DDV_GetHPixelPosGivenColNumberAndScroll(DdvMainPtr dmp, RecT rc,
                                                        Int4 Col, Int4 ScrollPos);
NLM_EXTERN Int4 DDV_GetVPixelPosGivenRowNumber(DdvMainPtr dmp, RecT rc, Int4 Row);
NLM_EXTERN Int4 DDV_GetVPixelPosGivenRowNumberAndScroll(DdvMainPtr dmp, RecT rc,
                                                        Int4 Row, Int4 ScrollPos);
NLM_EXTERN void DDV_GetVPixelPosOfEmptySpace(DdvMainPtr dmp, RecT rc, Int4* pTop, Int4* pBot);
NLM_EXTERN Int4 DDV_GetColNumberGivenMousePos(DdvMainPtr dmp, RecT rc, PoinT pt);
NLM_EXTERN void DDV_ReDraw(DdvMainPtr dmp);
NLM_EXTERN void DDV_ReDrawAtCol(DdvMainPtr dmp, Int4 Col, Boolean DDE);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* ndef _DDVCLICK_ */

