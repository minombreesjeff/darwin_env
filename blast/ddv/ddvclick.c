/*  $Id: ddvclick.c,v 1.65 2000/07/25 20:30:23 hurwitz Exp $
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
* File Name:  ddvclick.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   09/20/99
*
* $Revision: 1.65 $
*
* File Description: mouse management code for DeuxD-Viewer (DDV)
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ddvclick.c,v $
* Revision 1.65  2000/07/25 20:30:23  hurwitz
* bug fixes: panel update when file is closed, double-click launches UDV from DDE, seqName agrees in status line and left col
*
* Revision 1.64  2000/07/24 22:00:08  hurwitz
* fixed bug that 1/2 characters were displayed sometimes
*
* Revision 1.63  2000/07/20 22:25:57  hurwitz
* bug fix.  mouse-mode query working again
*
* Revision 1.62  2000/07/14 22:24:55  lewisg
* fix scroll, click, visual c++ build bugs.  add command line arg parse to ddv
*
* Revision 1.61  2000/07/12 22:22:41  hurwitz
* added delete block to DDV
*
* Revision 1.60  2000/07/12 21:18:13  hurwitz
* fixed display problems with select rectangle
*
* Revision 1.59  2000/07/12 15:38:08  hurwitz
* made rectangle select much faster.  it's almost working.
*
* Revision 1.58  2000/07/05 19:23:13  lewisg
* add two panes to ddv, update msvc project files
*
* Revision 1.57  2000/07/05 18:42:16  hurwitz
* added split block function to DDV
*
* Revision 1.56  2000/06/30 22:31:51  hurwitz
* added merge block function for DDV
*
* Revision 1.55  2000/06/28 19:41:31  hurwitz
* auto vertical scroll working for rectangle selection
*
* Revision 1.54  2000/06/27 20:46:38  hurwitz
* fixed bugs with select rectangle, added select row option
*
* Revision 1.53  2000/06/26 19:32:17  hurwitz
* can select a rectangle instead of a single row, auto horizontal scroll works but not vertical scroll yet
*
* Revision 1.52  2000/06/22 20:56:52  hurwitz
* assorted bug fixes
*
* Revision 1.51  2000/06/21 21:20:56  hurwitz
* a couple bug fixes
*
* Revision 1.50  2000/06/20 19:35:11  hurwitz
* use indexed seqAlign when necessary, make multiple when redrawing
*
* Revision 1.49  2000/06/16 18:40:02  lewisg
* send mouseup message when drag ends off of sequence
*
* Revision 1.48  2000/06/15 20:26:05  hurwitz
* added left/right/center justify for DDE
*
* Revision 1.47  2000/06/13 18:23:54  hurwitz
* made ViewMgr_MakeMultiple routine, call this on each launch of DDE rather than launch of DDV
*
* Revision 1.46  2000/06/12 23:02:40  hurwitz
* enable launch of DDE from Cn3D, swith from DDV_ComputeRuler to DDE_ReMakeRuler, get rid of styles option for DDE
*
* Revision 1.45  2000/06/06 20:09:22  lewisg
* fix double redraw bug
*
* Revision 1.44  2000/05/31 23:07:26  hurwitz
* made NoGaps a runtime parameter, fixed bug with vertical scroll of show/hide list, save edits query is not performed if nothing to save
*
* Revision 1.43  2000/05/25 21:40:42  hurwitz
* rows hidden in DDV are hidden in DDE, can save edits when rows are hidden in DDE
*
* Revision 1.42  2000/05/24 21:42:59  hurwitz
* getting hide/show rows to work with DDV and DDE together
*
* Revision 1.41  2000/05/19 13:48:31  hurwitz
* made a version of DDE that doesn't allow aligned gaps, changed wording for adding new rows
*
* Revision 1.40  2000/05/16 19:43:01  hurwitz
* grey out create block, delete block, undo, and redo as needed
*
* Revision 1.39  2000/05/08 22:48:33  hurwitz
* on launch of DDE, merge nodes that are same MsaTxtDisp alignment and gap
*
* Revision 1.38  2000/05/05 20:24:13  hurwitz
* some bug fixes, also redraw proper block in DDE after a save operation that causes a merge of 2 blocks
*
* Revision 1.37  2000/05/04 22:43:38  hurwitz
* don't launch DDE on top of DDV, change some wording, redraw DDE after save to AlnMgr
*
* Revision 1.36  2000/05/02 19:50:38  hurwitz
* fixed some bugs with launching DDE from DDV, added new alnMgr fn for positioning DDE on proper column
*
* Revision 1.35  2000/04/27 17:01:44  hurwitz
* small fixes to shift row and shift boundary
*
* Revision 1.34  2000/04/26 21:54:27  hurwitz
* added save function to tell AlnMgr about edits made in DDE
*
* Revision 1.33  2000/04/21 23:00:51  hurwitz
* can launch DDE from DDV
*
* Revision 1.32  2000/04/13 18:57:03  hurwitz
* for DDE: many bug fixes, also get rid of columns that are all unaligned gaps
*
* Revision 1.31  2000/04/10 20:58:42  hurwitz
* added GUI controls for DeleteBlock in DDE
*
* Revision 1.30  2000/04/07 16:21:08  hurwitz
* made delete block faster, added delete block to edit menu
*
* Revision 1.29  2000/04/05 20:52:35  hurwitz
* added GUI control for shifting left and right alignment boundaries
*
* Revision 1.28  2000/04/04 22:18:43  lewisg
* add defline to ddv, fix seq import bugs, set boundbox
*
* Revision 1.27  2000/04/03 22:26:31  hurwitz
* can now shift a row with click and drag
*
* Revision 1.26  2000/03/30 23:35:22  hurwitz
* fixed a bug involving scrolling while moving a row
*
* Revision 1.25  2000/03/29 20:02:48  hurwitz
* keep track of master during move row operations
*
* Revision 1.24  2000/03/28 21:03:14  hurwitz
* added gui control to re-order rows
*
* Revision 1.23  2000/03/27 18:07:48  hurwitz
* page up and page down working, give panel focus on launch
*
* Revision 1.22  2000/03/25 00:22:09  hurwitz
* put DDE_StackPtr in DDV_Main, add to stack inside DDE api's, added insert char, delete char, home and end keyboard control
*
* Revision 1.21  2000/03/01 22:49:41  lewisg
* import bioseq, neatlyindex, get rid of dead code
*
* Revision 1.20  2000/03/01 20:23:39  durand
* update select/deselect code to allow DDV sending correct message to Cn3D
*
* Revision 1.19  2000/02/15 22:40:57  lewisg
* add ability to launch udv so that it colors by row, fixes to colormgr, track rows from viewmgr, fix visual c projects
*
* Revision 1.18  2000/02/07 18:57:58  durand
*  fixed a selection problem
*
* Revision 1.17  2000/02/04 16:05:40  durand
* add click action to select a row
*
* Revision 1.16  2000/02/02 14:44:29  durand
* added function to create data structure for block editor, fixed some bugs
*
* Revision 1.15  2000/01/12 15:49:42  durand
* add About dlg box and fix a bug in selection
*
* Revision 1.14  2000/01/10 15:09:45  durand
* Use Entrez instead of ID1
*
* Revision 1.13  2000/01/05 21:11:13  durand
* update mouse click actions and DrawSequence function for a better use from ddv and cn3d
*
* Revision 1.12  1999/12/07 21:40:13  durand
* add mouse modes menu and caret facility for the editor
*
* Revision 1.11  1999/12/06 16:19:19  durand
* add GoTo facility to DDV
*
* Revision 1.10  1999/12/03 23:17:22  lewisg
* Patrick's new global update msg, argument passing when launching ddv, experimental editing
*
* Revision 1.9  1999/11/29 15:26:25  durand
* designed a new GUI to fix problems under MacOS, Linux and SGI
*
* Revision 1.8  1999/11/18 00:21:42  lewisg
* draw speedups and selection on mouseup
*
* Revision 1.7  1999/11/17 22:44:01  durand
* speed up the selection manager for large SeqAlign
*
* Revision 1.6  1999/11/03 21:29:48  durand
* add CTRL and SHFT keys for mouse selection. redesign the loader functions of DDV to properly register the message callbacks
*
* Revision 1.5  1999/10/29 14:15:39  durand
* add simple mouse selection functions
*
* Revision 1.4  1999/10/22 14:19:05  durand
* add new code for mouse selection
*
* Revision 1.3  1999/10/21 13:12:43  durand
* update selection system
*
* Revision 1.2  1999/10/12 15:01:28  lewisg
* resolve confict with internal/ddv
*
* Revision 1.1  1999/09/30 14:10:24  durand
* add ddv to toolkit
*
* Revision 1.2  1999/09/22 20:41:17  durand
* add mouse-click and drag functions
*
* Revision 1.1  1999/09/21 14:19:59  durand
* add basic mouse management functions
*
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <vibrant.h>

#include <udviewer.h>
#include <ddvpanel.h>
#include <ddvclick.h>
#include <ddvgraph.h>
#include <alignmgr.h>
#include <sqnutils.h>
#include <seqmgr.h>
#include <pgppop.h>
#include <samutil.h>
#include <tofasta.h>
#include <viewmgr.h>

/*static MonitorPtr  mon=NULL;*/
static Boolean OnAlignmentBoundary(PoinT pt, PaneL p, DdvMainPtr dmp,
                                   Int4* pBlockIndex, Boolean* pLeftBoundary,
                                   Int4* pCol, Int4* pHPos);
static Boolean GetBlockAndAlignment(PoinT pt, PaneL p, DdvMainPtr dmp,
                                    Int4* pBlockIndex, Boolean* pIsUnAligned,
                                    Int4* pCol, Int4* pOffset, Int4* pWidth, RecT* rcP);
static Boolean GetBlockAndAlignmentForCol(DdvMainPtr dmp, Int4 Col, Int4* pBlockIndex,
                                          Boolean* pIsUnAligned, Int4* pOffset, Int4* pWidth);
static RecT DrawOutline(PaneL p, Int4 Line_num_start, Int4 Disp_coord_start,
                        Int4 Line_num_stop, Int4 Disp_coord_stop, Boolean Draw);
static RecT DrawOutlineGivenScroll(PaneL p, Int4 Line_num_start, Int4 Disp_coord_start,
                                   Int4 Line_num_stop, Int4 Disp_coord_stop,
                                   Int4 VScroll, Int4 HScroll, Boolean Draw);
static void EraseOutline(PaneL p);
static Boolean RectanglesAreSame(RecT rc1, RecT rc2);
static void DDV_SelectRegion(DdvMainPtr dmp, PoinT pt, RecT rc,
                             Int4 row_start, Int4 row_stop,
                             Int4 col_start, Int4 col_stop);


NLM_EXTERN Int4 DDV_GetHPixelPosGivenColNumberAndScroll(DdvMainPtr dmp, RecT rc,
                                                        Int4 Col, Int4 ScrollPos) {
/*****************************************************************************
*  return number of pixels right to (0-based) Col.
*****************************************************************************/
  Int4  NumPixels;

	InsetRect(&rc,4,4);
	DDV_AdjustDrawingRect(&rc,&(dmp->GrData.udv_font),dmp);
  NumPixels = (Col+1-ScrollPos) * dmp->GrData.udv_font.ColWidth;
  NumPixels += dmp->GrData.udv_panel.cxName + dmp->GrData.udv_scale.cxLeftScale;
  NumPixels += rc.left;
  return(NumPixels);
}


NLM_EXTERN Int4 DDV_GetHPixelPosGivenColNumber(DdvMainPtr dmp, RecT rc, Int4 Col) {
/*****************************************************************************
*  return number of pixels right to (0-based) Col for current ScrollPos.
*****************************************************************************/
  return(DDV_GetHPixelPosGivenColNumberAndScroll(dmp, rc, Col, dmp->GrData.udv_hscrl.ScrollPos));
}


NLM_EXTERN Int4 DDV_GetVPixelPosGivenRowNumberAndScroll(DdvMainPtr dmp, RecT rc,
                                                        Int4 Row, Int4 ScrollPos) {
/*****************************************************************************
*  return number of pixels down to (0-based) Row.
*****************************************************************************/
  Int4  NumPixels;

	InsetRect(&rc,4,4);
	DDV_AdjustDrawingRect(&rc,&(dmp->GrData.udv_font), dmp);
  NumPixels = (Row+1-ScrollPos) * dmp->GrData.udv_font.LineHeight;
  /* next line is copied from DDV_GetRowGivenMousePos */
  NumPixels += 3*dmp->GrData.udv_panel.cyScale/2;
  NumPixels += rc.top;
  return(NumPixels);
}


NLM_EXTERN Int4 DDV_GetVPixelPosGivenRowNumber(DdvMainPtr dmp, RecT rc, Int4 Row) {
/*****************************************************************************
*  return number of pixels down to (0-based) Row for current scrollPos.
*****************************************************************************/
  return(DDV_GetVPixelPosGivenRowNumberAndScroll(dmp, rc, Row, dmp->GrData.udv_vscrl.ScrollPos));
}


NLM_EXTERN void DDV_GetVPixelPosOfEmptySpace(DdvMainPtr dmp, RecT rc, Int4* pTop, Int4* pBot) {
/*****************************************************************************
*  return Top:  number of pixels down to top of empty space.
*  return Bot:  number of pixels down to bot of empty space.
*****************************************************************************/
	InsetRect(&rc,4,4);
  *pTop = rc.top;
  *pBot = rc.top + dmp->GrData.udv_panel.cyScale/2;
  return;
}


NLM_EXTERN Int4 DDV_GetColNumberGivenMousePos(DdvMainPtr dmp, RecT rc, PoinT pt) {
/*****************************************************************************
*  return the display coordinate at this mouse position.
*****************************************************************************/
  Int4  mouse_col;

	InsetRect(&rc,4,4);
	DDV_AdjustDrawingRect(&rc,&(dmp->GrData.udv_font),dmp);
	rc.left+=dmp->GrData.udv_panel.cxName + dmp->GrData.udv_scale.cxLeftScale;
	mouse_col=(pt.x-rc.left)/dmp->GrData.udv_font.ColWidth+
		dmp->GrData.udv_hscrl.ScrollPos;
  return(mouse_col);
}


/*****************************************************************************

Function: DDV_GetRowGivenMousePos()

Purpose: given some graphical info, this function returns line_num, the row
		number (absolute value) and the ParaGLine_Num (i.e. sequence/row

		number; this value must be use to get data from TableHead).
					
Return value: line_num (-1 if not found ; otherwise a one-based value)

*****************************************************************************/
static Int4 DDV_GetRowGivenMousePos(DdvMainPtr dmp, RecT * rcP, PoinT * pt,

			Int4Ptr ParaGLine_Num)
{
Int4 Line_num=(Int4)-1,mouse_row,i,n;
ParaGPtr pgp;

	/*size of the Panel; see also DDV_DrawPanelContent_H()*/
	InsetRect(rcP,4,4);
	DDV_AdjustDrawingRect(rcP,&(dmp->GrData.udv_font),dmp);
	rcP->left+=dmp->GrData.udv_panel.cxName+dmp->GrData.udv_scale.cxLeftScale;
	rcP->top+=3*dmp->GrData.udv_panel.cyScale/2;

	/*mouse position*/
	mouse_row=(pt->y-rcP->top)/dmp->GrData.udv_font.LineHeight+
		dmp->GrData.udv_vscrl.ScrollPos;

	/*use the first ParaG of each sequence and mouse_row to find 
	row (==sequence) number*/
	for(i=0;i<dmp->MSA_d.pgp_l.nBsp;i++){
		pgp=(ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[i]->data.ptrvalue);
		if (pgp->ScaleStyle==SCALE_POS_NONE) n=0;
		else n=1;
		/*within each ParaG, the seq. in on line 0 or 1 (if ruler)*/
		if (mouse_row>=pgp->StartLine && mouse_row<=(pgp->StartLine+n)){
			Line_num=mouse_row+1;

			*ParaGLine_Num=i+1;
			break;
		}
	}

	return(Line_num);
}

/*****************************************************************************

Function: DDV_GetClosetSeqLocGivenBspPos()

Purpose: given a position in the Bioseq, this function return the closest
         selected region on that bioseq.
		
Parameters:	sip, eID, iID; bsioseq identifiers
            bsp_pos; current position on that bioseq
			old_pos; returns old_pos of a selection (used only if bModify is TRUE)
			bModify; if TRUE, create a modified slp

Note : sip can be null ONLY if bModify if FALSE.

Return value: see Purpose.

*****************************************************************************/
static SeqLocPtr DDV_GetClosetSeqLocGivenBspPos(SeqIdPtr sip, Uint2 eID, 
		Uint2 iID, Int4 bsp_pos, Int4Ptr old_pos, Boolean bModify)
{
SelStructPtr ssp;
SeqLocPtr    slp=NULL;
ValNodePtr   vnp_bsp,vnp;
Int4         diff,old_diff,diff_l,diff_r,bsp_start,bsp_stop;
Uint1        strand;

	ssp=ObjMgrGetSelected();
	if (ssp==NULL) return(NULL);
	
	vnp_bsp=DDV_GetSelectedRegions(ssp,eID,iID);
	if (vnp_bsp==NULL) return(NULL);
	vnp=vnp_bsp;
	*old_pos=(Int4)-1;
	old_diff=(Int4)INT4_MAX;

	while(vnp){
		bsp_start=SeqLocStart((SeqLocPtr)vnp->data.ptrvalue);
		bsp_stop=SeqLocStop((SeqLocPtr)vnp->data.ptrvalue);
		strand=SeqLocStrand((SeqLocPtr)vnp->data.ptrvalue);
		if (bsp_pos<bsp_start){/*left of a selected region ?*/
			diff=bsp_start-bsp_pos;
			if (diff<old_diff){	
				if (slp) slp=SeqLocFree(slp);
				if (bModify){
					slp = SeqLocIntNew (bsp_pos, bsp_stop, strand/*Seq_strand_minus*/, sip);
					*old_pos=bsp_stop;
				}
				else{
					slp = SeqLocIntNew (bsp_start, bsp_stop, strand, sip);
					*old_pos=(Int4)-1;
				}
				old_diff=diff;
			}
		}
		else if (bsp_pos>bsp_stop){/*right of a selected region ?*/
			diff=bsp_pos-bsp_stop;
			if (diff<old_diff){	
				if (slp) slp=SeqLocFree(slp);
				if (bModify){
					slp = SeqLocIntNew (bsp_start, bsp_pos, strand/*Seq_strand_plus*/, sip);
					*old_pos=bsp_start;
				}
				else{
					slp = SeqLocIntNew (bsp_start, bsp_stop, strand, sip);
					*old_pos=(Int4)-1;
				}
				old_diff=diff;
			}
		}
		else{/*inside a selected region ?*/
			if(bModify){
				diff_l=bsp_pos-bsp_start;
				diff_r=bsp_stop-bsp_pos;
				if (diff_l<diff_r){
					bsp_start=bsp_pos;
					*old_pos=bsp_stop;
					/*strand=Seq_strand_plus;*/
				}
				else{
					bsp_stop=bsp_pos;
					*old_pos=bsp_start;
					/*strand=Seq_strand_minus;*/
				}
				slp = SeqLocIntNew (bsp_start, bsp_stop, strand, sip);
				break;
			}
		}
		vnp=vnp->next;
	}
	ValNodeFree(vnp_bsp);
	return(slp);
}

/*****************************************************************************

Function: DDV_IsFullBspAlreadySel()

Purpose: check if a row is already selected.
		
Parameters:	bsp_eID,bsp_iID; bsp identifiers
			bsp_start,bsp_stop; full sequence 
			
Return value: TRUE if the sequence is already fully selected

*****************************************************************************/
static Boolean DDV_IsFullBspAlreadySel(Uint2 bsp_eID,Uint2 bsp_iID,Int4 bsp_start,
		Int4 bsp_stop)
{
SelStructPtr ssp;
ValNodePtr   vnp_bsp,vnp;
Int4         sel_bsp_start,sel_bsp_stop;

	ssp=ObjMgrGetSelected();
	if (ssp==NULL) return(FALSE);
	
	vnp_bsp=DDV_GetSelectedRegions(ssp,bsp_eID,bsp_iID);
	if (vnp_bsp==NULL) return(FALSE);
	vnp=vnp_bsp;

	while(vnp){
		sel_bsp_start=SeqLocStart((SeqLocPtr)vnp->data.ptrvalue);
		sel_bsp_stop=SeqLocStop((SeqLocPtr)vnp->data.ptrvalue);
		if (bsp_start==sel_bsp_start && bsp_stop==sel_bsp_stop)
			return(TRUE);
		vnp=vnp->next;
	}
	return(FALSE);
}


static Uint1 DDV_GetRegionInParaG(ParaGPtr pgp, Int4 Line_num) {
/*****************************************************************************
*  get region within ParaG
*****************************************************************************/
  Int4  mouse_row, diff;

  mouse_row = Line_num-1;

  if (pgp->ScaleStyle==SCALE_POS_NONE)
    diff=0;
  else
    diff=1;

  if (mouse_row==(pgp->StartLine+diff)){
    return(PGP_REGION_SEQ);
  }
  else if (mouse_row >  (pgp->StartLine+ diff) && 
           mouse_row <= (pgp->StartLine+ pgp->nLines-1)){
    return(PGP_REGION_FEAT);
  }
  return(PGP_REGION_NOWHERE);
}


static void DDV_GetRowAndColGivenPtAndScroll(DdvMainPtr dmp, RecT rc, PoinT pt,
                                             Int4 HScroll, Int4 VScroll,
                                             Int4Ptr Disp_coord, Int4Ptr Line_num) {
/*****************************************************************************
*  get row and column where the mouse is clicked.  Provide scroll positions.
*****************************************************************************/
  Int4  mouse_row, mouse_col;

  InsetRect(&rc,4,4);
  DDV_AdjustDrawingRect(&rc, &(dmp->GrData.udv_font), dmp);
  rc.left += dmp->GrData.udv_panel.cxName + dmp->GrData.udv_scale.cxLeftScale;
  rc.top += 3*dmp->GrData.udv_panel.cyScale/2;

  mouse_row = (pt.y - rc.top)/dmp->GrData.udv_font.LineHeight + VScroll;
  mouse_col = (pt.x - rc.left)/dmp->GrData.udv_font.ColWidth + HScroll;

  *Line_num=mouse_row+1;
  *Disp_coord=mouse_col;
}


static void DDV_GetRowAndColGivenPt(DdvMainPtr dmp, RecT rc, PoinT pt,
                                    Int4Ptr Disp_coord, Int4Ptr Line_num) {
/***********************************************************************************
*  get row and column where the mouse is clicked.  Assume current scroll positions.
***********************************************************************************/
  DDV_GetRowAndColGivenPtAndScroll(dmp, rc, pt,
      dmp->GrData.udv_hscrl.ScrollPos, dmp->GrData.udv_vscrl.ScrollPos,
      Disp_coord, Line_num);
}


static Int4 DDV_GetParaGLineNumGivenLineNum(DdvMainPtr dmp, Int4 Line_num) {
/*****************************************************************************
*  get the ParaGLine_Num for Line_num.
*****************************************************************************/
  Int4      i, row;
  ParaGPtr  pgp;

  row = Line_num-1;
  for (i=0; i<dmp->MSA_d.pgp_l.nBsp; i++) {
    pgp = (ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[i]->data.ptrvalue);
    if (row>=pgp->StartLine && row<=(pgp->StartLine + pgp->nLines-1)) {
      return(i+1);
    }
  }
  return(-1);
}


static Int4 DDV_GetSeqAlignCoordGivenCol(DdvMainPtr dmp, Int4 Disp_coord) {
/*****************************************************************************
*  get the SeqAlignCoord for Disp_coord.
*****************************************************************************/
  ValNodePtr        vnp;
  DDVRulerDescrPtr  drdp;
  Int4              diff;

  vnp=dmp->MSA_d.pgp_l.RulerDescr;
  while(vnp){
    drdp=(DDVRulerDescrPtr)vnp->data.ptrvalue;
    if (!drdp->bUnAligned){
      diff = Disp_coord - drdp->disp_start;
      if (Disp_coord>=drdp->disp_start && Disp_coord<=drdp->disp_stop){
        return(drdp->align_start + diff);
      }
    }
    vnp=vnp->next;
  }
  return(-1);
}


static ParaGPtr DDV_GetPGPGivenRowAndCol(DdvMainPtr dmp, Int4 Line_num, Int4 Disp_coord) {
/*****************************************************************************
*  get the ParaG ptr that contains (Line_num, Disp_coord)
*****************************************************************************/
  ParaGPtr    pgp=NULL;
  ValNodePtr  vnp;

  vnp=dmp->MSA_d.pgp_l.TableHead[Line_num-1];
  while(vnp){
    pgp=(ParaGPtr)vnp->data.ptrvalue;
    if (Disp_coord>=pgp->StartLetter && Disp_coord<=pgp->StopLetter){
      return(pgp);
    }
    vnp=vnp->next;
  }
  return(pgp);
}


static Boolean DDV_GetCoordsGivenAClickAndScrollPos(DdvMainPtr dmp, RecT * rcP, PoinT * pt,
    Int4 HScroll, Int4 VScroll,
		Int4Ptr bsp_coord, Int4Ptr SeqAlign_coord, Int4Ptr Disp_coord,
		Int4Ptr Line_num, Int4Ptr ParaGLine_Num, Uint1Ptr uWhere, 
		ParaGPtr PNTR cur_pgp)
/*****************************************************************************
*  variation on DDV_GetCoordsGivenAClick.
*  pass: VScroll and HScroll -- the positions of the vertical and
*        horizontal scroll bars.
*****************************************************************************/
{
ValNodePtr     vnp;
ParaGPtr       pgp,TheParaG;
/*MsaTxtDispPtr  mtdp;*/
DDVRulerDescrPtr drdp;
Int4           mouse_row,mouse_col,i,diff;
Boolean        bFound;

  *ParaGLine_Num = -1;
	/*size of the Panel; see also DDV_DrawPanelContent_H()*/
	InsetRect(rcP,4,4);
	DDV_AdjustDrawingRect(rcP,&(dmp->GrData.udv_font),dmp);
	rcP->left+=dmp->GrData.udv_panel.cxName+dmp->GrData.udv_scale.cxLeftScale;
	rcP->top+=3*dmp->GrData.udv_panel.cyScale/2;

	/*mouse position*/
	mouse_row=(pt->y-rcP->top)/dmp->GrData.udv_font.LineHeight+VScroll;
	mouse_col=(pt->x-rcP->left)/dmp->GrData.udv_font.ColWidth+HScroll;

	bFound=FALSE;

	for(i=0;i<dmp->MSA_d.pgp_l.nBsp;i++){
		/*use the first ParaG of each sequence and mouse_row to find 
		row (==sequence) number; then go though the ParaG list of that
		sequence to find The ParaG, using mouse_col*/
		pgp=(ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[i]->data.ptrvalue);
		if (mouse_row>=pgp->StartLine && mouse_row<=(pgp->StartLine+pgp->nLines-1)){
			vnp=dmp->MSA_d.pgp_l.TableHead[i];
		    *ParaGLine_Num = i + 1;
			while(vnp){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				if (mouse_col>=pgp->StartLetter && mouse_col<=pgp->StopLetter){
					bFound=TRUE;
					TheParaG=pgp;
					break;
				}
				vnp=vnp->next;
			}
			if (bFound) break;
		}
	}
	/*ok : the mouse is inside a ParaG*/
	if (bFound){
		*cur_pgp=TheParaG;
		/*where is the mouse in the ParaG ?*/
		*uWhere=PGP_REGION_NOWHERE;
		if (TheParaG->ScaleStyle==SCALE_POS_NONE)
			diff=0;
		else
			diff=1;
		if (mouse_row==(TheParaG->StartLine+diff)){
			*uWhere=PGP_REGION_SEQ;
		}
		else if (mouse_row>(TheParaG->StartLine+diff) && 
				mouse_row<=(pgp->StartLine+pgp->nLines-1)){
			*uWhere=PGP_REGION_FEAT;
		}

		/*a. get Disp Coord*/
		*Line_num=mouse_row+1;/*base 1 value*/
		*Disp_coord=mouse_col;

		/*on the Sequence ? Then, get the coords. (BSP, DISP, SEQALIGN)*/
		/*PGP_REGION_FEAT is not yet implemented...  ;-) */
		if (*uWhere==PGP_REGION_SEQ){
			/*b. get BSP coord*/
			*bsp_coord=DDV_GetBspCoordGivenDispCoord(TheParaG,mouse_col);
			
			/*c. get SeqAlign Coord*/
			vnp=dmp->MSA_d.pgp_l.RulerDescr;
			while(vnp){
				drdp=(DDVRulerDescrPtr)vnp->data.ptrvalue;
				if (!drdp->bUnAligned){
					diff=mouse_col-drdp->disp_start;
					if (mouse_col>=drdp->disp_start && mouse_col<=drdp->disp_stop){
						*SeqAlign_coord=drdp->align_start+diff;
						break;
					}
				}
				else{
					*SeqAlign_coord=(Int4)-1;
				}
				vnp=vnp->next;
			}
		}
	}

	return(bFound);
}


/*****************************************************************************

Function: DDV_GetCoordsGivenAClick()

Purpose: given some graphical info, this function returns bsp_coord, SeqAlign_coord,
         Disp_coord. Future implementation : will also return eID, iID, idx of
		 a feature (if the nice user clicks on a feature).
		
Parameters:	dmp; panel main data block
            rcP; panel size
			pt; mouse position
			bsp_coord, SeqAlign_coord, Disp_coord,Line_num; return values
            uWhere; where the mouse is in a ParaG
			
Note : all but Line_num are zero-based values. Line_num is a one-based value.
			
Return value: TRUE if the user clicks within a ParaG (see also 
		bsp_coord, SeqAlign_coord, Disp_coord, uWhere). NOTE : if the function 
		returns FALSE then bsp_coord, SeqAlign_coord, Line_num, Disp_coord and 
		uWhere are undefined !

*****************************************************************************/
static Boolean DDV_GetCoordsGivenAClick(DdvMainPtr dmp, RecT * rcP, PoinT * pt,
		Int4Ptr bsp_coord, Int4Ptr SeqAlign_coord, Int4Ptr Disp_coord,
		Int4Ptr Line_num, Int4Ptr ParaGLine_Num, Uint1Ptr uWhere, 
		ParaGPtr PNTR cur_pgp)
{
  /* return coordinates for the current scroll position */
  return(DDV_GetCoordsGivenAClickAndScrollPos(dmp, rcP, pt, 
    dmp->GrData.udv_hscrl.ScrollPos, dmp->GrData.udv_vscrl.ScrollPos,
    bsp_coord, SeqAlign_coord, Disp_coord, Line_num, ParaGLine_Num, uWhere, cur_pgp));
}

/*****************************************************************************

Function: DDV_AutoHScroll()

Purpose: used from drag/hold mouse to auto-scroll the panel horizontally when
         the mouse reaches the left/right borders of the panel


Return value: none

*****************************************************************************/
static void DDV_AutoHScroll(DdvMainPtr dmp,Int4 Disp_coord)
{
Int4 from_col,to_col,from_row,to_row,old_pos;
BaR hsb;

	DDV_GetCurrentDispRange(dmp->hWndDDV,&(dmp->GrData),dmp->MSA_d.pgp_l.LengthAli,
			&from_col,&to_col,&from_row,&to_row);
	
	hsb = GetSlateHScrollBar ((SlatE) dmp->hWndDDV);
	old_pos=GetBarValue(hsb);
	if (Disp_coord>=to_col){/*scroll to the right*/
		SetValue(hsb,old_pos+1);
	}
	else if (Disp_coord<=from_col){
		SetValue(hsb,old_pos-1);
	}

}

/*****************************************************************************

Function: DDV_AutoVScroll()

Purpose: used from drag/hold mouse to auto-scroll the panel vertically when
         the mouse reaches the top/bottom borders of the panel


Return value: none

*****************************************************************************/
static void DDV_AutoVScroll(DdvMainPtr dmp,Int4 Line_num)
{
Int4 from_col,to_col,from_row,to_row,old_pos;
BaR vsb;

	DDV_GetCurrentDispRange(dmp->hWndDDV,&(dmp->GrData),dmp->MSA_d.pgp_l.LengthAli,
			&from_col,&to_col,&from_row,&to_row);
	
	vsb = GetSlateVScrollBar ((SlatE) dmp->hWndDDV);
	old_pos=GetBarValue(vsb);
	if (Line_num>=(to_row-1)){/*scroll down*/
		SetValue(vsb,old_pos+1);
	}
	else if (Line_num<=(from_row+1)){/*scroll up*/
		SetValue(vsb,old_pos-1);
	}
}


/*****************************************************************************

Function: DDV_DispSelRangeInStatus()

Purpose: display position while selection is in progress. (mouse mode : selection)

Return value: none

*****************************************************************************/
static void DDV_DispSelRangeInStatus(PrompT InfoPanel,Int4 bsp_start, Int4 bsp_stop,
		Int4 line_num,CharPtr szSeqName)
{
Char szText[250];

	sprintf(szText,"Selection : %s, [%i]:[%i..%i]",szSeqName,line_num,
		bsp_start,bsp_stop);
	SetTitle(InfoPanel,szText);
}


/*****************************************************************************

Function: DDV_DispPositionInStatus()

Purpose: display position (mouse mode : query).

Return value: none

*****************************************************************************/
NLM_EXTERN void DDV_DispPositionInStatus(PrompT InfoPanel,Int4 bsp_pos,
		Int4 line_num,CharPtr szSeqName)
{
Char szText[250];

	sprintf(szText,"Position : %s, [%i]:[%i]",szSeqName,line_num,bsp_pos);
	SetTitle(InfoPanel,szText);
}


static void DDV_DispStatus(DdvMainPtr dmp, Int4 Line_num_stop,
                           Int4 Disp_coord_start, Int4 Disp_coord_stop) {
/*****************************************************************************

Function: DDV_DispStatus()

Purpose: display position while selection is in progress. (mouse mode : selection)

Return value: none

*****************************************************************************/
  DdvMainWinPtr  mWin_d;
  Int4           ParaGLine_num, bsp_start, bsp_stop;
  ParaGPtr       pgp;
  char           szAccess[21];

  mWin_d = (DdvMainWinPtr) GetObjectExtra(dmp->hParent);
  if (mWin_d != NULL) {
    ParaGLine_num = DDV_GetParaGLineNumGivenLineNum(dmp, Line_num_stop);
    if (ParaGLine_num != -1) {
      DDV_GetSeqNameGivenRow(dmp->MSA_d.pgp_l.TableHead, ParaGLine_num, szAccess);
      pgp = DDV_GetPGPGivenRowAndCol(dmp, Line_num_stop, Disp_coord_stop);
      bsp_stop = DDV_GetBspCoordGivenDispCoord(pgp, Disp_coord_stop);
      if (dmp->MouseMode == DDV_MOUSEMODE_SELECT ||
          dmp->MouseMode == DDV_MOUSEMODE_SELECT_ONE_ROW) {
        pgp = DDV_GetPGPGivenRowAndCol(dmp, Line_num_stop, Disp_coord_start);
        bsp_start = DDV_GetBspCoordGivenDispCoord(pgp, Disp_coord_start);
        DDV_DispSelRangeInStatus(mWin_d->InfoPanel, bsp_start+1, bsp_stop+1,
          ParaGLine_num, szAccess);
      }
      else {
        DDV_DispPositionInStatus(mWin_d->InfoPanel, bsp_stop+1,
          ParaGLine_num, szAccess);
      }
    }
  }
}


/*****************************************************************************

Function: DDV_GetSeqNameGivenRow()

Purpose: get a sequence number, given a list of ParaG and a row number

Note: row_num is a one-based value; this value must be in the range 0 to nBsp.

(I don't check that here, so be carefull when calling this function)


Return value: -

*****************************************************************************/
NLM_EXTERN void DDV_GetSeqNameGivenRow(ValNodePtr * ParaG_List, Int4 row_num,
	CharPtr szSeqName)
{
  SeqIdPtr sip;
  ParaGPtr pgp;
  BioseqPtr bsp;
  
  pgp=(ParaGPtr)(ParaG_List[row_num-1]->data.ptrvalue);
  
  /*get a name*/	
  bsp = BioseqLockById(pgp->sip);
  if(bsp) {
    /* get name but not gi */
    sip = SeqIdFindBestAccession(bsp->id);
    BioseqUnlock(bsp);
  }
  
  if (!sip)
    sip = SeqIdFindBest(pgp->sip, 0);
  SeqIdWrite(sip, szSeqName,PRINTID_TEXTID_ACCESSION, 20);   
}

/*****************************************************************************

Function: DDV_SendBSPSelectMsg_OLD()

Purpose: send a message to select a range of letters on a bioseq.

Return value: none

*****************************************************************************/
static void DDV_SendBSPSelectMsg_OLD(DdvMainPtr dmp,Int4 bsp_coord,Int4 SeqAlign_coord,
		Int4 first_Disp_coord, Int4 Disp_coord,Int4 Line_num,Int4 ParaGLine_Num,
    ParaGPtr cur_pgp,PoinT * pt, Boolean Display)
{
Int4        first_bsp_coord, bsp_pos;
Uint1       direction;/*use to tell ObjMgr the direction of the mouse (left,right)*/
SeqLocPtr   slp;/*to send an AlsoSelectMsg*/
Uint2       bsp_eID,bsp_iID;
Boolean     bDeselectAll=TRUE;
DdvMainWinPtr dmwp;

	/*analyse the bsp coords*/
	if (bsp_coord==(Int4)-1){/*this is a gap*/
		dmp->ms.oldPos=dmp->ms.newPos=*pt;
		dmp->ms.old_row=Line_num;
		dmp->ms.old_col=Disp_coord;
		dmp->ms.old_pgp=cur_pgp;
		return;
	}

	/*get the very first click (from DDV_ClickProc)*/
	first_bsp_coord=
		DDV_GetBspCoordGivenDispCoord(dmp->ms.first_pgps[Line_num-1],
                                  dmp->ms.first_cols[Line_num-1]);
	if (first_bsp_coord==(Int4)-1){/*the first time the user 
		clicked, it was on a gap*/
		/*if we are in this scope, that means we have a bsp coord*/
		first_bsp_coord=bsp_coord;
		dmp->ms.first_cols[Line_num-1]=Disp_coord;
		dmp->ms.first_pgps[Line_num-1]=cur_pgp;
	}

	bsp_pos=bsp_coord;
	
	if (ctrlKey || shftKey)
		bDeselectAll=FALSE;

	/*the mouse is moving to the left or to the right ?*/
	/*don't forget to check the strand...  ;-) */
	if (AlnMgrGetNthStrand(dmp->MSA_d.pgp_l.sap, ParaGLine_Num)==Seq_strand_plus){
		if (dmp->ms.oldPos.x<pt->x)
			direction=Seq_strand_plus;
		else
			direction=Seq_strand_minus;
	}
	else{
		if (dmp->ms.oldPos.x>pt->x)
			direction=Seq_strand_minus;
		else
			direction=Seq_strand_plus;
	}
	/*'from' (first_bsp_coord) always less than 'to' (bsp_coord)*/
	if (first_bsp_coord>bsp_coord)
		swap(&first_bsp_coord,&bsp_coord);
	/*save the new position*/
	dmp->ms.oldPos=dmp->ms.newPos=*pt;
	dmp->ms.old_row=Line_num;
	dmp->ms.old_col=Disp_coord;
	dmp->ms.old_pgp=cur_pgp;

	/*now, we can send the Select message*/
	/* first_pgp, old_pgp & cur_pgp have the same sip... 
	they are on a same row; I can use one of them*/
	slp = SeqLocIntNew (first_bsp_coord, bsp_coord, direction, cur_pgp->sip);
	UDV_DecodeIdxFeat (dmp->MSA_d.pgp_l.entitiesTbl[ParaGLine_Num-1],&bsp_eID,&bsp_iID);
  if (bsp_eID!=0 && bsp_iID!=0 && slp){
    /*update InfoPanel*/
    dmwp=(DdvMainWinPtr)GetObjectExtra(dmp->hParent);
    if (dmwp){
      Char szAccess[21];
			
      DDV_GetSeqNameGivenRow(dmp->MSA_d.pgp_l.TableHead, ParaGLine_Num, szAccess);
	        
      switch(dmp->MouseMode){
        case DDV_MOUSEMODE_SELECT:
        case DDV_MOUSEMODE_SELECT_ONE_ROW:
          if (Display) {
            DDV_DispSelRangeInStatus(dmwp->InfoPanel,first_bsp_coord+1,
              bsp_coord+1,ParaGLine_Num,szAccess);/*+1 : switch to one-based value*/
          }
          break;
        case DDV_MOUSEMODE_QUERY:
        case DDV_MOUSEMODE_EDIT:
          DDV_DispPositionInStatus(dmwp->InfoPanel, 
            bsp_pos+1,ParaGLine_Num,szAccess);/*+1 : switch to one-based value*/
          break;
      }
    }
		/*send the message*/
    if (dmp->MouseMode==DDV_MOUSEMODE_SELECT ||
        dmp->MouseMode==DDV_MOUSEMODE_SELECT_ONE_ROW){
      ObjMgrAlsoSelect (bsp_eID, bsp_iID, OBJ_BIOSEQ,OM_REGION_SEQLOC, slp);
    }
  }
  else {
    if (slp) 
      SeqLocFree(slp);
  }
}


/*****************************************************************************

Function: DDV_SendBSPSelectMsg()

Purpose: send a message to select a range of letters on a bioseq.
         I've rewritten this.  Hope I did it right.  Dave H.

Return value: none

*****************************************************************************/
static void DDV_SendBSPSelectMsg(DdvMainPtr dmp,
		Int4 col_start, Int4 col_stop, Int4 row, Boolean Display)
{
  Int4           bsp_coord_start, bsp_coord_stop, ParaGLine_Num, col;
  Uint1          direction;/*used to tell ObjMgr the direction of the mouse (left,right)*/
  SeqLocPtr      slp;/*to send an AlsoSelectMsg*/
  Uint2          bsp_eID, bsp_iID;
  Boolean        bDeselectAll=TRUE;
  ParaGPtr       pgp;
  DdvMainWinPtr  dmwp;

  if (col_start > col_stop) swap(&col_start, &col_stop);

  /* calculate bsp_coord_start */
  for (col=col_start; col<=col_stop; col++) {
    pgp = DDV_GetPGPGivenRowAndCol(dmp, row, col);
    bsp_coord_start = DDV_GetBspCoordGivenDispCoord(pgp, col);
    if (bsp_coord_start != -1) break;
  }

  /* calculate bsp_coord_stop */
  for (col=col_stop; col>=col_start; col--) {
    pgp = DDV_GetPGPGivenRowAndCol(dmp, row, col);
    bsp_coord_stop = DDV_GetBspCoordGivenDispCoord(pgp, col);
    if (bsp_coord_stop != -1) break;
  }

  ParaGLine_Num = DDV_GetParaGLineNumGivenLineNum(dmp, row);

	if (ctrlKey || shftKey)
		bDeselectAll=FALSE;

	/*the mouse is moving to the left or to the right ?*/
	/*don't forget to check the strand...  ;-) */
  if (AlnMgrGetNthStrand(dmp->MSA_d.pgp_l.sap, ParaGLine_Num)==Seq_strand_plus) {
    direction=Seq_strand_plus;
  }
  else {
    direction=Seq_strand_minus;
  }

  /*now, we can send the Select message*/
  /* first_pgp, old_pgp & cur_pgp have the same sip... 
  they are on a same row; I can use one of them*/
  slp = SeqLocIntNew (bsp_coord_start, bsp_coord_stop, direction, pgp->sip);
  UDV_DecodeIdxFeat (dmp->MSA_d.pgp_l.entitiesTbl[ParaGLine_Num-1],&bsp_eID,&bsp_iID);
  if (bsp_eID!=0 && bsp_iID!=0 && slp){
    /*update InfoPanel*/
    dmwp=(DdvMainWinPtr)GetObjectExtra(dmp->hParent);
    if (dmwp){
      Char szAccess[21];
			
      DDV_GetSeqNameGivenRow(dmp->MSA_d.pgp_l.TableHead, ParaGLine_Num, szAccess);
	        
      switch(dmp->MouseMode){
        case DDV_MOUSEMODE_SELECT:
        case DDV_MOUSEMODE_SELECT_ONE_ROW:
          if (Display) {
            DDV_DispSelRangeInStatus(dmwp->InfoPanel, bsp_coord_start+1,
              bsp_coord_stop+1, ParaGLine_Num, szAccess); /*+1 : switch to one-based value*/
          }
          break;
        case DDV_MOUSEMODE_QUERY:
        case DDV_MOUSEMODE_EDIT:
          DDV_DispPositionInStatus(dmwp->InfoPanel, 
            bsp_coord_stop+1, ParaGLine_Num, szAccess);/*+1 : switch to one-based value*/
          break;
      }
    }
		/*send the message*/
    if (dmp->MouseMode==DDV_MOUSEMODE_SELECT ||
        dmp->MouseMode==DDV_MOUSEMODE_SELECT_ONE_ROW){
      ObjMgrAlsoSelect (bsp_eID, bsp_iID, OBJ_BIOSEQ, OM_REGION_SEQLOC, slp);
    }
  }
  else {
    if (slp) 
      SeqLocFree(slp);
  }
}


/*****************************************************************************

Functions: DDV_ClickProcUpper() and DDV_ClickProcLower()

Purpose: mouse click funcions attached to the upper
            and lower panels in 2-panel mode.  They switch focus between the upper
            and lower panels, then call the main DDV_ClickProc.
		
Parameters:	p; panel handle (currently unused)
			pt; new mouse position

Return value: none

*****************************************************************************/

NLM_EXTERN void DDV_ClickProcUpper(PaneL p, PoinT pt)
{
DdvMainWinPtr mWin_d;
WindoW		  hWinMain;
Boolean       tempClick;

	hWinMain=(WindoW)ParentWindow(p);
	if (hWinMain==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
	if (mWin_d==NULL) return;
	mWin_d->hWndDDV = mWin_d->hUpperPanel;
    DDV_SetMenuFocus(hWinMain);
	mWin_d->InfoPanel = mWin_d->UpperInfoPanel;
    tempClick = Nlm_dblClick;  /* keep vibrant from resetting double click */
    SetTitle(mWin_d->InfoPanel,"Ready !");
    SetTitle(mWin_d->LowerInfoPanel,"");
    Nlm_dblClick = tempClick;

    DDV_ClickProc(p, pt);
}

NLM_EXTERN void DDV_ClickProcLower(PaneL p, PoinT pt)
{
DdvMainWinPtr mWin_d;
WindoW		  hWinMain;
Boolean       tempClick;

	hWinMain=(WindoW)ParentWindow(p);
	if (hWinMain==NULL) return;
	mWin_d = (DdvMainWinPtr) GetObjectExtra (hWinMain);
	if (mWin_d==NULL) return;
	mWin_d->hWndDDV = mWin_d->hLowerPanel;
    DDV_SetMenuFocus(hWinMain);
	mWin_d->InfoPanel = mWin_d->LowerInfoPanel;
    tempClick = Nlm_dblClick;  /* keep vibrant from resetting double click */
    SetTitle(mWin_d->InfoPanel,"Ready !");
    SetTitle(mWin_d->UpperInfoPanel,"");
    Nlm_dblClick = tempClick;

    DDV_ClickProc(p, pt);
}

/*****************************************************************************

Function: DDV_ClickProc()

Purpose: manage Mouse-Click; this function is designed for the autonomous viewer
		to manage the InfoPanel and the Features List Dialog Box
		DON'T USE FOR EXTERNAL SOFTWARE...
		
Parameters:	p; panel handle (currently unused)
			pt; new mouse position

Return value: none

*****************************************************************************/
NLM_EXTERN void DDV_ClickProc(PaneL p, PoinT pt)
{
DdvMainPtr 	   dmp;
DdvMainWinPtr  mWin_d;
RecT           rcP, rcP2;
Int4           bsp_coord, SeqAlign_coord, Disp_coord, Line_num,bsp_start,bsp_stop,
               old_pos, ParaGLine_Num, VPos, HPos;
Int4           from_row, to_row, from_col, to_col;
Uint1          uWhere;
ParaGPtr       cur_pgp=NULL;
SeqLocPtr      slp;
Uint2          bsp_eID,bsp_iID;
Boolean        bDeselectAll=TRUE,bIsAlreadyFullSel;
Int4           BlockIndex, Col, NumBlocks;
Boolean        LeftBoundary, IsUnAligned;
DdvMainWinPtr  dmwp;
BioseqPtr      bsp;
Char           buf[81], szAccess[21];
ParaGPtr       pgp;
Int4           i, NumRows, ColCnt, Offset, Width;

MsaParaGPopListPtr  mpplp;
SeqAlignPtr         sap;
DDE_StackPtr        dsp;
Boolean             IsEditable, *pNoGaps, NoGaps, FoundIt;

	/*get the panel data*/
	dmp = (DdvMainPtr) GetObjectExtra(p);
	if (dmp==NULL) return;
  mWin_d = (DdvMainWinPtr) GetObjectExtra(dmp->hParent);
  if (mWin_d == NULL) return;
	
	ObjectRect(p,&rcP);

	if (ctrlKey || shftKey)
		bDeselectAll=FALSE;

  /* handle the case when a block is being deleted in DDV */
  if (dmp->MouseMode == DDV_MOUSEMODE_DELETEBLOCK) {
    /* get block index, etc */
    if (GetBlockAndAlignment(pt, p, dmp, &BlockIndex, &IsUnAligned, &Col, &Offset, &Width, &rcP)) {
      /* if block is aligned */
      if (!IsUnAligned) {
        /* create display for single block */
        sap = ViewMgr_GetBeginIndexed(dmp->MSA_d.pgp_l.sap);
        dmp->MSA_d.pgp_l.viewed_sap = dmp->MSA_d.pgp_l.sap;
        mpplp = DDE_CreateDisplay(sap, BlockIndex, IsUnAligned, &NumBlocks);
        dsp = DDE_NewStack(mpplp);
        /* record which block is being edited, etc */
        dsp->LaunchBlock = BlockIndex;
        dsp->NumBlocks =   NumBlocks;
        dsp->IsUnAligned = IsUnAligned;
        DDE_MergeNodesLists(dsp->pEdit);
        /* delete block, tell AlnMgr to save edits, redraw DDV */
        DDE_DeleteBlock(dsp, 0, FALSE);
        dmp->dsp = dsp;
        DDV_SaveEdits(dmp, TRUE);
        /* free stack */
        dsp = DDE_FreeStack(dsp);
        dmp->dsp = dsp;
      }
    }
    ArrowCursor();
    dmp->MouseMode = dmp->SavedMouseMode;
  }
  
  /* handle the click when splitting 2 blocks */
  if (dmp->MouseMode == DDV_MOUSEMODE_SPLITBLOCK) {
    FoundIt = FALSE;
    /* adjust x position so vertical bar jumps when mouse is on center of char */
    pt.x -= dmp->GrData.udv_font.ColWidth/2;
    /* if user clicked on an aligned block */
    if (GetBlockAndAlignment(pt, p, dmp, &BlockIndex, &IsUnAligned, &Col, &Offset, &Width, &rcP)) {
      /* save col where vertical bar was last drawn */
      dmp->SaveCol = Col;
      /* draw vertical bar after this column */
      HPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, Col);
      UDV_draw_vertical_bar(rcP, HPos, FALSE);
      dmp->ms.Action_type = MS_ACTION_SPLIT_BLOCK;
      FoundIt = TRUE;
    }
    if (!FoundIt) {
      ArrowCursor();
    }
    dmp->MouseMode = dmp->SavedMouseMode;
  }
  
  /* handle the 1st click when merging 2 blocks */
  /* for the 1st click just need to record the block */
  else if (dmp->MouseMode == DDV_MOUSEMODE_MERGEBLOCKS1) {
    FoundIt = FALSE;
    /* if mouse is in a valid place, get BlockIndex and alignment status */
    if (GetBlockAndAlignment(pt, p, dmp, &BlockIndex, &IsUnAligned, &Col, &Offset, &Width, &rcP)) {
      if (!IsUnAligned) {
        dmp->BlockIndex = BlockIndex;
        dmp->MouseMode = DDV_MOUSEMODE_MERGEBLOCKS2;
        FoundIt = TRUE;
      }
    }
    if (!FoundIt) {
      dmp->MouseMode = dmp->SavedMouseMode;
      ArrowCursor();
    }
  }

  /* handle the 2nd click when merging 2 blocks */
  else if (dmp->MouseMode == DDV_MOUSEMODE_MERGEBLOCKS2) {
    /* if mouse is in a valid place, get BlockIndex and alignment status */
    if (GetBlockAndAlignment(pt, p, dmp, &BlockIndex, &IsUnAligned, &Col, &Offset, &Width, &rcP)) {
      /* if 2 blocks are next to each other and both aligned */
      if ((!IsUnAligned) && (ABS(BlockIndex - dmp->BlockIndex) == 1)) {
        /* create a stack for the block on the right */
        BlockIndex = MAX(BlockIndex, dmp->BlockIndex);
        sap = ViewMgr_GetBeginIndexed(dmp->MSA_d.pgp_l.sap);
        dmp->MSA_d.pgp_l.viewed_sap = dmp->MSA_d.pgp_l.sap;
        mpplp = DDE_CreateDisplay(sap, BlockIndex, IsUnAligned, &NumBlocks);
        dsp = DDE_NewStack(mpplp);
        /* record which block is being edited, etc */
        dsp->LaunchBlock = BlockIndex;
        dsp->NumBlocks =   NumBlocks;
        dsp->IsUnAligned = IsUnAligned;
        DDE_MergeNodesLists(dsp->pEdit);
        /* if block on right is flush left */
        if (DDE_FirstColumnIsAligned(dsp->pEdit)) {
          /* delete block on right.  count number of cols that were in block. */
          ColCnt = DDE_DeleteBlock(dsp, 0, FALSE);
          /* tell AlnMgr to save edits, don't redraw yet */
          dmp->dsp = dsp;
          DDV_SaveEdits(dmp, FALSE);
          /* free stack for block on right.  create stack for block on left */
          DDE_FreeStack(dsp);
          sap = ViewMgr_GetBeginIndexed(dmp->MSA_d.pgp_l.sap);
          dmp->MSA_d.pgp_l.viewed_sap = dmp->MSA_d.pgp_l.sap;
          mpplp = DDE_CreateDisplay(sap, BlockIndex-1, IsUnAligned, &NumBlocks);
          dsp = DDE_NewStack(mpplp);
          /* record which block is being edited, etc */
          dsp->LaunchBlock = BlockIndex-1;
          dsp->NumBlocks =   NumBlocks;
          dsp->IsUnAligned = IsUnAligned;
          DDE_MergeNodesLists(dsp->pEdit);
          DDE_Verify(dsp->pEdit);
          /* shift right boundary of left block by number cols in right block */
          DDE_ShiftRightBoundary(dsp, 0, ColCnt, FALSE);
          /* tell AlnMgr to save edits, now redraw */
          dmp->dsp = dsp;
          DDV_SaveEdits(dmp, TRUE);
        }
        /* free remaining stack */
        dsp = DDE_FreeStack(dsp);
        dmp->dsp = dsp;
      }
    }
    dmp->MouseMode = dmp->SavedMouseMode;
    ArrowCursor();
  }

  
  /* handle the case when we're launching the editor on a block */
  else if (dmp->MouseMode == DDV_MOUSEMODE_LAUNCHEDITOR) {
    /* if mouse is in a valid place, get BlockIndex and alignment status */
    if (GetBlockAndAlignment(pt, p, dmp, &BlockIndex, &IsUnAligned, &Col, &Offset, &Width, &rcP)) {
      /* don't allow editing when there are gaps and no gaps are allowed */
      pNoGaps = (Boolean*) GetAppProperty("dde_nogaps");
      if (pNoGaps == NULL) NoGaps = TRUE;
      else NoGaps = *pNoGaps;
      IsEditable = AlnMgrIsEditable(dmp->MSA_d.pgp_l.sap);
      if (!(IsEditable == AM_EDITGAPS && NoGaps) && IsEditable) {
        ViewMgr_MakeMultiple(dmp->MSA_d.pgp_l.sap);
      }
      else {
        Message(MSG_OK, "%s", "Unable to convert this alignment\n into an editable form.");
        dmp->MouseMode = dmp->SavedMouseMode;
        ArrowCursor();
        return;
      }
      /* get pointer to original seqAlign */
      sap = ViewMgr_GetBeginIndexed(dmp->MSA_d.pgp_l.sap);
      /* create display for editor */
      mpplp = DDE_CreateDisplay(sap, BlockIndex, IsUnAligned, &NumBlocks);
      mpplp->viewed_sap = dmp->MSA_d.pgp_l.sap;
      /* create the stack used by editor */
      dsp = DDE_NewStack(mpplp);
      DDE_MergeNodesLists(dsp->pEdit);
      /* record which block is being edited, etc */
      dsp->LaunchBlock = BlockIndex;
      dsp->NumBlocks =   NumBlocks;
      dsp->IsUnAligned = IsUnAligned;
      /* hide the rows in DDE that are hidden in DDV */
      NumRows = dsp->pEdit->TotalNumRows;
      for (i=0; i<NumRows; i++) {
        if (ViewMgr_TRow2VRow(dmp->MSA_d.pgp_l.sap, i+1) == -1) {
          DDE_HideRow(dsp, i, FALSE);
        }
      }
      DDE_Add(dsp);
      dsp->SomethingToSave = FALSE;
      /* launch the editor as a slave */
      SetAppProperty("ddeinterndata",(void*)dsp);
      GatherProcLaunch(OMPROC_EDIT, FALSE, dmp->MSA_d.entityID, 
					dmp->MSA_d.itemID, OBJ_SEQALIGN, 0, 0, OBJ_SEQALIGN, 0);
      RemoveAppProperty("ddeinterndata");
    }
    dmp->MouseMode = dmp->SavedMouseMode;
    ArrowCursor();
  }
  
  /* handle the case when we're creating an aligned block */
  else if (dmp->MouseMode == DDV_MOUSEMODE_CREATEBLOCK) {
    ObjectRect(p, &rcP);
    InsetRect(&rcP,4,4);
    DDV_AdjustDrawingRect(&rcP,&(dmp->GrData.udv_font),dmp);
    rcP.left += dmp->GrData.udv_panel.cxName + dmp->GrData.udv_scale.cxLeftScale;
    /* if the point is in a valid spot */
    if (PtInRect(pt, &rcP)) {
      ObjectRect(p, &rcP);
      Col = DDV_GetColNumberGivenMousePos(dmp, rcP, pt);
      /* and the column's legal */
      if ((Col >= 0) && (Col < dmp->MSA_d.pgp_l.LengthAli)) {
        /* and there is no aligned block already */
        if (DDE_GetNumBlocks(dmp->dsp->pEdit) == 0) {
          /* save start col */
          dmp->dsp->FromCol = Col;
          /* save col where vertical bar was last drawn */
          dmp->dsp->SaveCol = Col;
          /* draw vertical bar before and after this column */
          HPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, dmp->dsp->SaveCol);
          UDV_draw_vertical_bar(rcP, HPos, TRUE);
          HPos -= dmp->GrData.udv_font.ColWidth;
          UDV_draw_vertical_bar(rcP, HPos, TRUE);
          dmp->ms.Action_type = MS_ACTION_CREATE_BLOCK;
        }
        else {
          dmp->MouseMode = dmp->SavedMouseMode;
          ArrowCursor();
        }
      }
      else {
        dmp->MouseMode = dmp->SavedMouseMode;
        ArrowCursor();
      }
    }
    else {
      dmp->MouseMode = dmp->SavedMouseMode;
      ArrowCursor();
    }
  }

  /*the mouse is located in a ParaG ?*/
	/*if YES : the user is selecting either sequence letter(s) or a feature*/
	else if ((pt.x>(rcP.left+dmp->GrData.udv_panel.cxName+
       dmp->GrData.udv_scale.cxLeftScale)) &&
		  (pt.y>(rcP.top+dmp->GrData.udv_panel.cyScale))) {
			/*get the ParaG list and look for the ParaG where the mouse is located*/
			/*note : only implemented for DDV_DISP_HORZ type of display...*/
			if (dmp->MSA_d.pgp_l.DisplayType==DDV_DISP_HORZ){
				if (DDV_GetCoordsGivenAClick(dmp,&rcP,&pt,
					&bsp_coord,&SeqAlign_coord,&Disp_coord,&Line_num,
					&ParaGLine_Num,&uWhere,&cur_pgp)){
					if (uWhere==PGP_REGION_SEQ){
						if (bDeselectAll && (dmp->MouseMode==DDV_MOUSEMODE_SELECT ||
                                 dmp->MouseMode==DDV_MOUSEMODE_SELECT_ONE_ROW))
							ObjMgrDeSelectAll ();
						if (shftKey && bsp_coord!=(Int4)-1){
							UDV_DecodeIdxFeat (dmp->MSA_d.pgp_l.entitiesTbl[ParaGLine_Num-1], 
								&bsp_eID,&bsp_iID);
							if (bsp_eID!=0 && bsp_iID!=0){
								slp=DDV_GetClosetSeqLocGivenBspPos(cur_pgp->sip,bsp_eID, 
									bsp_iID, bsp_coord, &old_pos, TRUE);
								if (slp){
									if (dmp->MouseMode==DDV_MOUSEMODE_SELECT ||
                      dmp->MouseMode==DDV_MOUSEMODE_SELECT_ONE_ROW)
										ObjMgrAlsoSelect (bsp_eID, bsp_iID, 
											OBJ_BIOSEQ,OM_REGION_SEQLOC, slp);
									if (old_pos!=(Int4)-1)
										Disp_coord=old_pos;
								}
							}
							dmp->ms.oldPos=pt;
							dmp->ms.Action_type=MS_ACTION_SELECT_SEQ;
							return;
						}

            /* display info about residues that's clicked */
            if (ParaGLine_Num != -1) {
              DDV_GetSeqNameGivenRow(dmp->MSA_d.pgp_l.TableHead, ParaGLine_Num, szAccess);
              DDV_DispPositionInStatus(mWin_d->InfoPanel, bsp_coord+1, ParaGLine_Num, szAccess);
            }

						/*save the positions */
						dmp->ms.oldPos=pt;
						dmp->ms.newPos=pt;
						dmp->ms.Action_type=MS_ACTION_SELECT_SEQ;
						dmp->ms.old_row=Line_num;
						dmp->ms.old_col=Disp_coord;
						dmp->ms.old_pgp=cur_pgp;
						dmp->ms.first_row=Line_num;
            dmp->ms.first_col=Disp_coord;
            dmp->ms.last_row=Line_num;
            dmp->ms.last_col=Disp_coord;

            /* show outline of highlighted area */
            CopyMode();
            DrawOutline(p, Line_num, Disp_coord, Line_num, Disp_coord, TRUE);

            if (dmp->ms.first_cols == NULL) {
              NumRows = dmp->MSA_d.pgp_l.nBsp;
              dmp->ms.first_cols = MemNew(NumRows * sizeof(Int4));
              dmp->ms.first_pgps = MemNew(NumRows * sizeof(ParaGPtr));
            }
						dmp->ms.first_cols[Line_num-1]=Disp_coord;
						dmp->ms.first_pgps[Line_num-1]=cur_pgp;
            dmp->ms.first_point=pt;
            dmp->ms.first_HScroll=dmp->GrData.udv_hscrl.ScrollPos;
            dmp->ms.first_VScroll=dmp->GrData.udv_vscrl.ScrollPos;
            dmp->ms.old_HScroll = dmp->GrData.udv_hscrl.ScrollPos;
            dmp->ms.old_VScroll = dmp->GrData.udv_vscrl.ScrollPos;
						/*update caret pos, if needed only*/
						if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
							dmp->dci.old_row=dmp->dci.new_row;
							dmp->dci.old_col=dmp->dci.new_col;
						}
            /* if we're in the editor and mousemode is query */
            /* click and drag on sequence will cause the whole row to shift */
            if (dmp->bEditor) {
              if (dmp->MouseMode == DDV_MOUSEMODE_QUERY) {
                /* make sure column is legit */
                DDV_GetCurrentDispRange(dmp->hWndDDV,&(dmp->GrData),
                  dmp->MSA_d.pgp_l.LengthAli,&from_col,&to_col,&from_row,&to_row);
                /* Don't shift row if clicking on first or last column of */
                /* display because then the screen scrolls */
                if (((Disp_coord>from_col) && (Disp_coord<=to_col)) || (Disp_coord==0)) {
                  /* draw a double rectangle around the selected char */
                  /* to mark the from spot */
                  ObjectRect(p, &rcP);
                  VPos = DDV_GetVPixelPosGivenRowNumber(dmp, rcP, Line_num-1); /* 1-based */
                  HPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, Disp_coord); /* 0-based */
                  rcP2.left =    HPos - dmp->GrData.udv_font.ColWidth;
                  rcP2.right =   HPos;
                  rcP2.top =     VPos - dmp->GrData.udv_font.LineHeight;
                  rcP2.bottom =  VPos;
                  UDV_draw_rectangle(rcP2, FALSE);
                  InsetRect(&rcP2, 1, 1);
                  UDV_draw_rectangle(rcP2, FALSE);
                  CrossCursor();
                  dmp->dsp->FromCol = Disp_coord;
                  dmp->dsp->FromRow = Line_num-1;
                  dmp->dsp->SaveCol = Disp_coord;
                  dmp->dsp->SaveRow = Line_num-1;
                  dmp->ms.Action_type = MS_ACTION_SHIFT_ROW;
                }
              }
            }
						/*select a single letter*/
            if (dmp->MouseMode == DDV_MOUSEMODE_SELECT ||
                dmp->MouseMode == DDV_MOUSEMODE_SELECT_ONE_ROW) {
/*
              DDV_SendBSPSelectMsg(dmp, bsp_coord, SeqAlign_coord,
                Disp_coord, Line_num, ParaGLine_Num, cur_pgp, &pt, TRUE);
*/
            }
					}
				}
			}
	}	
	/*the mouse is located within the Name list region (left of ParaG) ?*/
	else if (pt.x>=dmp->GrData.udv_panel.cxName-1 && 
			pt.x<=dmp->GrData.udv_panel.cxName+3){
				rcP.left=5*dmp->GrData.udv_font.cxChar; /*min value*/
				rcP.right=2*PANEL_NAME_WIDTH*dmp->GrData.udv_font.cxChar;/*max val*/
				pt.x=dmp->GrData.udv_panel.cxName;
				dmp->ms.oldPos=pt;
				dmp->ms.newPos=pt;
				dmp->ms.rcClip=rcP;
				dmp->ms.Action_type=MS_ACTION_RESIZE_WIN;
				InvertMode();	
				UDV_draw_double_cursor(dmp->ms.rcClip,
					dmp->ms.oldPos);
				CrossCursor();
	}
	/*the mouse is located on a sequence name ?*/
	else if (pt.x<dmp->GrData.udv_panel.cxName &&
			pt.y>(rcP.top+dmp->GrData.udv_panel.cyScale)){
		Line_num=DDV_GetRowGivenMousePos(dmp, &rcP, &pt,&ParaGLine_Num);
		if (Line_num!=(Int4)-1){
		    UDV_DecodeIdxFeat (dmp->MSA_d.pgp_l.entitiesTbl[ParaGLine_Num-1], 
				&bsp_eID,&bsp_iID);
			if (dblClick /*&& !dmp->bEditor*/){/*load the sequence viewer on that sequence*/
                {
                    SAM_ViewGlobal vg;
                    
                    MemSet(&vg, 0, sizeof(SAM_ViewGlobal));
                    vg.pCGlobal = dmp->Globals.colorp;
                    vg.Row = Line_num;
                    if(!dmp->bEditor) vg.MasterViewer = SAMVIEWDDV;
                    else vg.MasterViewer = SAMVIEWDDE;
                    SetAppProperty(SAM_ViewString, &vg);
                    GatherSpecificProcLaunch(0, "OneD-Viewer", OMPROC_VIEW,
                      FALSE, bsp_eID, bsp_iID, OBJ_BIOSEQ);
                    RemoveAppProperty(SAM_ViewString);
                }
            }
			else{
				if (dmp->bEditor){
					WindoW temport;
					dmp->deri.curEditRow=ParaGLine_Num-1;
					rcP.left=0;
					rcP.right=rcP.left+dmp->GrData.udv_panel.cxName;
					temport=SavePort(ParentWindow(p));
					Select(p);
					InvalRect(&rcP);
					Update();
					RestorePort(temport);
          /* record the row that's moving */
          dmp->dsp->FromRow = Line_num - 1;
          /* keep track of where the underline is */
          dmp->dsp->SaveRow = Line_num - 1;
					dmp->ms.Action_type = MS_ACTION_MOVE_ROW;
          /* make a rectangle of the sequence names region */
          ObjectRect(p,&rcP);
          rcP2.left =   rcP.left;
          rcP2.right =  rcP.left + dmp->GrData.udv_panel.cxName - 10;
          rcP2.top =    rcP.top;
          rcP2.bottom = rcP.bottom;
          /* draw the underline */
          VPos = DDV_GetVPixelPosGivenRowNumber(dmp, rcP2, Line_num-1);
          UDV_draw_horizontal_line(rcP2, VPos);
          CrossCursor();
				}
        else  /* not editor */ {
            dmwp=(DdvMainWinPtr)GetObjectExtra(dmp->hParent);
            if(dmwp) {
                pgp=(ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[Line_num-1]->data.ptrvalue);
		            bsp=BioseqLockById(pgp->sip);
                CreateDefLine(NULL,bsp,buf,80,0,NULL,NULL);
                BioseqUnlock(bsp);
                SetTitle(dmwp->InfoPanel,buf);
            }
        }
			}
		}
	}
	/*the mouse is located  just on the left of a sequence ? 
		If true, select the full sequence*/
	else if ((pt.x>=dmp->GrData.udv_panel.cxName+3) /*after the name of the sequence ?*/
			&&
			(pt.x<=dmp->GrData.udv_panel.cxName+3+dmp->GrData.udv_scale.cxLeftScale+
			dmp->GrData.udv_font.cxChar) /*before the sequence ?*/
			&& 
			(dmp->MouseMode==DDV_MOUSEMODE_SELECT ||
       dmp->MouseMode==DDV_MOUSEMODE_SELECT_ONE_ROW)/*valid only if selection mode*/
			){
		if (pt.y>(rcP.top+dmp->GrData.udv_panel.cyScale)){
			/*get the ParaG list and look for the ParaG where the mouse is located*/
			/*note : only implemented for DDV_DISP_HORZ type of display...*/
			if (dmp->MSA_d.pgp_l.DisplayType==DDV_DISP_HORZ){
				Line_num=DDV_GetRowGivenMousePos(dmp, &rcP, &pt,&ParaGLine_Num);
				if (Line_num!=(Int4)-1){
					/*get min,max coord. of BSP*/
					UDV_GetBspRangeinPGPList(dmp->MSA_d.pgp_l.TableHead[ParaGLine_Num-1],
						&bsp_start,&bsp_stop);
					if (bsp_start!=INT4_MAX && bsp_stop!=INT4_MIN){
						/*now, we can send the Select message*/
						/* first_pgp, old_pgp & cur_pgp have the same sip... 
						they are on a same row; I can use one of them*/
						cur_pgp=(ParaGPtr)(dmp->MSA_d.pgp_l.TableHead[ParaGLine_Num-1]->data.ptrvalue);
						slp = SeqLocIntNew (bsp_start, bsp_stop, Seq_strand_plus, cur_pgp->sip);
						UDV_DecodeIdxFeat (dmp->MSA_d.pgp_l.entitiesTbl[ParaGLine_Num-1], 
							&bsp_eID,&bsp_iID);
						if (bsp_eID!=0 && bsp_iID!=0 && slp){
							bIsAlreadyFullSel=DDV_IsFullBspAlreadySel(bsp_eID,bsp_iID,
								bsp_start,bsp_stop);
							if (!bIsAlreadyFullSel){
								if (!bDeselectAll){
									ObjMgrAlsoSelect (bsp_eID, bsp_iID, 
										OBJ_BIOSEQ,OM_REGION_SEQLOC, slp);
								}
								else{
									ObjMgrDeSelectAll ();
									ObjMgrSelect (bsp_eID, bsp_iID, 
										OBJ_BIOSEQ,OM_REGION_SEQLOC, slp);
								}
							}
							else{
								if (bDeselectAll)
									ObjMgrDeSelectAll ();
								ObjMgrDeSelect (bsp_eID, bsp_iID, 
									OBJ_BIOSEQ,OM_REGION_SEQLOC, slp);
							}
						}		
					}
					dmp->ms.Action_type=MS_ACTION_SELECT_FULL_SEQ;
				}
			}
		}
	}

  /* if the mouse is located on one of the vertical bars, on top, that */
  /* mark an alignment boundary */
  else if (OnAlignmentBoundary(pt, p, dmp, &BlockIndex, &LeftBoundary, &Col, &HPos)) {
    /* record the boundary and block that are being moved */
    dmp->dsp->FromCol = Col;
    dmp->dsp->BlockIndex = BlockIndex;
    dmp->dsp->LeftBoundary = LeftBoundary;
    /* record the column of the last drawn vertical bar */
    dmp->dsp->SaveCol = Col;
    dmp->ms.Action_type = MS_ACTION_SHIFT_BOUNDARY;
    /* draw a dotted outline of the vertical bar */
    ObjectRect(p, &rcP);
    UDV_draw_vertical_bar(rcP, HPos, TRUE);
    CrossCursor();
  }

	else{/*click outside "special" regions : deselect everything*/
		if (dmp->MouseMode==DDV_MOUSEMODE_SELECT ||
        dmp->MouseMode==DDV_MOUSEMODE_SELECT_ONE_ROW)
			ObjMgrDeSelectAll ();
			/*this line if for cn3d only*/
            ObjMgrSendMsg(OM_MSG_MOUSEUP, dmp->MSA_d.entityID, 
						dmp->MSA_d.itemID, OBJ_SEQALIGN);
	}

  if (dmp->bEditor) {
    DDV_GreyOut(mWin_d, DDE_AtStartOfStack(dmp->dsp), DDE_AtEndOfStack(dmp->dsp));
  }
}


static Boolean GetBlockAndAlignmentForCol(DdvMainPtr dmp, Int4 Col, Int4* pBlockIndex,
                                          Boolean* pIsUnAligned, Int4* pOffset, Int4* pWidth) {
/*****************************************************************************
*  get the BlockIndex and Alignment status for Col.
*****************************************************************************/
  /* if Col is legal */
  if ((Col >= 0) && (Col < dmp->MSA_d.pgp_l.LengthAli)) {
    /* get the block or unaligned-region index */
    if (DDE_IsColValid(&(dmp->MSA_d.pgp_l), Col, pBlockIndex, pIsUnAligned, pOffset, pWidth)) {
      return(TRUE);
    }
  }
  *pBlockIndex = -1;
  *pIsUnAligned = TRUE;
  return(FALSE);
}


static Boolean GetBlockAndAlignment(PoinT pt, PaneL p, DdvMainPtr dmp,
                                    Int4* pBlockIndex, Boolean* pIsUnAligned,
                                    Int4* pCol, Int4* pOffset, Int4* pWidth, RecT* rcP) {
/*****************************************************************************
*  get the BlockIndex and Alignment status for point pt.
*****************************************************************************/
  ObjectRect(p, rcP);
  InsetRect(rcP,4,4);
  DDV_AdjustDrawingRect(rcP, &(dmp->GrData.udv_font), dmp);
  rcP->left += dmp->GrData.udv_panel.cxName + dmp->GrData.udv_scale.cxLeftScale;
  /* if the point is in a valid spot */
  if (PtInRect(pt, rcP)) {
    ObjectRect(p, rcP);
    *pCol = DDV_GetColNumberGivenMousePos(dmp, *rcP, pt);
    /* and the column's legal */
    if ((*pCol >= 0) && (*pCol < dmp->MSA_d.pgp_l.LengthAli)) {
      /* get the block or unaligned-region index */
      if (DDE_IsColValid(&(dmp->MSA_d.pgp_l), *pCol, pBlockIndex, pIsUnAligned, pOffset, pWidth)) {
        return(TRUE);
      }
    }
  }
  *pCol=-1;
  *pBlockIndex = -1;
  *pIsUnAligned = TRUE;
  return(FALSE);
}


static Boolean RectanglesAreSame(RecT rc1, RecT rc2) {
/*****************************************************************************
*  return TRUE if 2 rectangles are identical.
*****************************************************************************/
  if (rc1.left   != rc2.left)    return(FALSE);
  if (rc1.right  != rc2.right)   return(FALSE);
  if (rc1.top    != rc2.top)     return(FALSE);
  if (rc1.bottom != rc2.bottom)  return(FALSE);
  return(TRUE);
}


static void EraseOutline(PaneL p) {
/*****************************************************************************
*  erase the old outline of the region selected for highlighting.
*****************************************************************************/
  DdvMainPtr  dmp;
  BaR         hsb, vsb;
  Int4        HScroll, VScroll;

  dmp = (DdvMainPtr) GetObjectExtra(p);
	if (dmp==NULL) return;

  hsb = GetSlateHScrollBar((SlatE) dmp->hWndDDV);
  vsb = GetSlateVScrollBar((SlatE) dmp->hWndDDV);

  /* save current scroll positions, set scroll positions to saved scroll positions */
  HScroll = dmp->GrData.udv_hscrl.ScrollPos;
  VScroll = dmp->GrData.udv_vscrl.ScrollPos;
  SetValue(hsb, dmp->ms.old_HScroll);
  SetValue(vsb, dmp->ms.old_VScroll);

  /* erase the old rectangle */
  InvertMode();
  UDV_draw_rectangle(dmp->ms.outline, FALSE);

  /* restore scroll positions */
  SetValue(hsb, HScroll);
  SetValue(vsb, VScroll);
}


static RecT DrawOutline(PaneL p, Int4 Line_num_start, Int4 Disp_coord_start,
                        Int4 Line_num_stop, Int4 Disp_coord_stop, Boolean Draw) {
/*****************************************************************************
*  use current scroll postions.
*****************************************************************************/
  RecT        rcP2;
  DdvMainPtr  dmp;

  rcP2.left = rcP2.right = rcP2.top = rcP2.bottom = 0;

  dmp = (DdvMainPtr) GetObjectExtra(p);
	if (dmp==NULL) return(rcP2);

  return(DrawOutlineGivenScroll(p,
    Line_num_start, Disp_coord_start, Line_num_stop, Disp_coord_stop,
    dmp->GrData.udv_vscrl.ScrollPos, dmp->GrData.udv_hscrl.ScrollPos, Draw));
}


static RecT DrawOutlineGivenScroll(PaneL p, Int4 Line_num_start, Int4 Disp_coord_start,
                                   Int4 Line_num_stop, Int4 Disp_coord_stop, 
                                   Int4 VScroll, Int4 HScroll, Boolean Draw) {
/*****************************************************************************
*  draw the outline of the region being selected for highlighting.
*  Line_num_stop and Disp_coord_stop are for the current mouse position.
*  Line_num_start and Disp_coord_start are for the first click position.
*  if Draw is FALSE, just return the rectangle and don't draw it.
*  Line_num's are 1-based.  Disp_coord's are 0-based.
*  treat VScroll and HScroll as the current scroll positions.
*  return rectangle that's drawn.
*****************************************************************************/
  RecT        rcP, rcP2;
  DdvMainPtr  dmp;
  Int4        Line_num_stop_save, Disp_coord_stop_save;

  rcP2.left = rcP2.right = rcP2.top = rcP2.bottom = 0;
  Line_num_stop_save = Line_num_stop;
  Disp_coord_stop_save = Disp_coord_stop;
  
  dmp = (DdvMainPtr) GetObjectExtra(p);
	if (dmp==NULL) return(rcP2);

  if (Line_num_start > Line_num_stop) {
    swap(&Line_num_start, &Line_num_stop);
  }
  if (Disp_coord_start > Disp_coord_stop) {
    swap(&Disp_coord_start, &Disp_coord_stop);
  }
  if (Line_num_start < 1)
    Line_num_start=1;
  if (Line_num_stop > dmp->MSA_d.pgp_l.nBsp)
    Line_num_stop=dmp->MSA_d.pgp_l.nBsp;
  if (Disp_coord_start < 0)
    Disp_coord_start=0;
  if (Disp_coord_stop >= dmp->MSA_d.pgp_l.LengthAli)
    Disp_coord_stop=dmp->MSA_d.pgp_l.LengthAli;

  if (dmp->MouseMode==DDV_MOUSEMODE_SELECT || dmp->MouseMode==DDV_MOUSEMODE_SELECT_ONE_ROW) {
    /* calculate new rectangle */
    ObjectRect(p, &rcP);
    rcP2.top =    DDV_GetVPixelPosGivenRowNumber(dmp, rcP, Line_num_start-1);
    rcP2.bottom = DDV_GetVPixelPosGivenRowNumber(dmp, rcP, Line_num_stop-1);
    rcP2.left =   DDV_GetHPixelPosGivenColNumber(dmp, rcP, Disp_coord_start);
    rcP2.right =  DDV_GetHPixelPosGivenColNumber(dmp, rcP, Disp_coord_stop);
    rcP2.top -=   dmp->GrData.udv_font.LineHeight;
    rcP2.left -=  dmp->GrData.udv_font.ColWidth;
    if (Draw) {
      /* make clip region */
	    InsetRect(&rcP,4,4);
	    DDV_AdjustDrawingRect(&rcP,&(dmp->GrData.udv_font),dmp);
	    rcP.left+=dmp->GrData.udv_panel.cxName+dmp->GrData.udv_scale.cxLeftScale;
	    rcP.top+=3*dmp->GrData.udv_panel.cyScale/2;
      rcP.bottom+=2; /* so bottom of rectangle not cut */
      ClipRect(&rcP);
      /* draw new rectangle */
      UDV_draw_rectangle(rcP2, FALSE);
      ResetClip();
      /* save row and col for current mouse pos */
      dmp->ms.last_row = Line_num_stop_save;
      dmp->ms.last_col = Disp_coord_stop_save;
    }
  }
  return(rcP2);
}


static Boolean OnAlignmentBoundary(PoinT pt, PaneL p, DdvMainPtr dmp,
                                   Int4* pBlockIndex, Boolean* pLeftBoundary,
                                   Int4* pCol, Int4* pHPos) {
/*****************************************************************************
*  check if point pt is on one of the vertical bars that mark
*  alignment boundaries.  return TRUE if it is.
*  whether it's TRUE or not, ...
*  return BlockIndex indicating which aligned block,
*  return LeftBoundary = TRUE if p is on a left boundary,
*  return LeftBoundary = FALSE if p is on a right boundary.
*  return Col; the display coordinate for the alignment boundary.
*  return HPos; the horizontal pixel position of the boundary.
*****************************************************************************/
  RecT  rcP, rcBanner;
  Int4  TopVPos, BotVPos, LeftHPos, RightHPos;
  Int4  i, NumBlocks;

  /* this only works in the editor */
  if (!dmp->bEditor) return(FALSE);

  /* make rectangle for banner across top.  make it extra wide */
  ObjectRect(p, &rcP);
  DDV_GetVPixelPosOfEmptySpace(dmp, rcP, &TopVPos, &BotVPos);
  rcBanner = rcP;
  rcBanner.left += dmp->GrData.udv_panel.cxName + dmp->GrData.udv_scale.cxLeftScale;
  rcBanner.top =    TopVPos-4;
  rcBanner.bottom = BotVPos+8;
  rcBanner.left -= 4;

  /* for each aligned block */
  NumBlocks = DDE_GetNumBlocks(dmp->dsp->pEdit);
  for (i=0; i<NumBlocks; i++) {
    /* check if the pt is reasonably close to the left vertical bar */
    *pCol = DDE_GetAlignStart(dmp->dsp->pEdit, i);
    LeftHPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, *pCol);
    LeftHPos -= dmp->GrData.udv_font.ColWidth;
    if ((pt.x >= LeftHPos-5) && (pt.x <= LeftHPos+5)) {
      *pBlockIndex = i;
      *pLeftBoundary = TRUE;
      *pHPos = LeftHPos;
      if (PtInRect(pt, &rcBanner)) return(TRUE);
      return(FALSE);
    }
    /* check if the pt is reasonably close to the right vertical bar */
    *pCol = DDE_GetAlignStop(dmp->dsp->pEdit, i);
    RightHPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, *pCol);
    if ((pt.x >= RightHPos-5) && (pt.x <= RightHPos+5)) {
      *pBlockIndex = i;
      *pLeftBoundary = FALSE;
      *pHPos = RightHPos;
      if (PtInRect(pt, &rcBanner)) return(TRUE);
      return(FALSE);
    }
  }
  return(FALSE);
}


/*****************************************************************************

Function: DDV_DragProc()

Purpose: manage Mouse-Drag

Parameters:	p; panel handle (currently unused)
			pt; new position

Return value: none

*****************************************************************************/
NLM_EXTERN void DDV_DragProc(PaneL p, PoinT pt)
{
DdvMainPtr 	dmp;
RecT        rcP, rcP2;
ParaGPtr    cur_pgp=NULL;
Int4        Jump;
Int4        Line_num, ParaGLine_num;/*to get the coordinates*/
Int4        VPos, SavedVPos, from_col, to_col, from_row, to_row, old_VPos;
Int4        HPos, SavedHPos, Col;
Int4        Disp_coord_stop;
Int4        Line_num_stop, BlockIndex, Offset, Width;
BaR         vsb;
Boolean     IsUnAligned;
static Boolean  LineToErase=TRUE;

	/*get the panel data*/
	dmp = (DdvMainPtr) GetObjectExtra(p);
	if (dmp==NULL) return;
	
	/*what is happening ?*/
	switch(dmp->ms.Action_type){
		case MS_ACTION_SELECT_SEQ:/*selection on a sequence*/
			if (shftKey) break;
			ObjectRect(p,&rcP);
			/*get the ParaG list and look for the ParaG where the mouse is located*/
			/*note : only implemented for DDV_DISP_HORZ type of display...*/
			if (dmp->MSA_d.pgp_l.DisplayType==DDV_DISP_HORZ) {
        /* get line num and col for current mouse position */
        DDV_GetRowAndColGivenPt(dmp, rcP, pt, &Disp_coord_stop, &Line_num_stop);
        /* force single row selection when needed */
        if (dmp->MouseMode == DDV_MOUSEMODE_SELECT_ONE_ROW) Line_num_stop = dmp->ms.first_row;
        /* erase the old rectangle */
        InvertMode();
        DrawOutline(p, dmp->ms.first_row, dmp->ms.first_col,
                       dmp->ms.last_row,  dmp->ms.last_col, TRUE);
        /* get display info */
        DDV_GetCurrentDispRange(dmp->hWndDDV,&(dmp->GrData),
          dmp->MSA_d.pgp_l.LengthAli,&from_col,&to_col,&from_row,&to_row);
        /* draw the new rectangle */
        CopyMode();
        DrawOutline(p, dmp->ms.first_row, dmp->ms.first_col, 
                       Line_num_stop,     Disp_coord_stop, TRUE);
        /* display info about residues that mouse is over */
        DDV_DispStatus(dmp, Line_num_stop, dmp->ms.first_col, Disp_coord_stop);
      }

      break;
		case MS_ACTION_RESIZE_WIN:/*the user moves the 3D line located between 
			name area and SeqAlign area*/
			InvertMode();	
			UDV_draw_double_cursor(dmp->ms.rcClip, dmp->ms.oldPos);
			dmp->ms.newPos=pt;
			UDV_draw_double_cursor(dmp->ms.rcClip, dmp->ms.newPos);
 			dmp->ms.oldPos=dmp->ms.newPos;
			Update();
			break;
		case MS_ACTION_SELECT_FULL_SEQ:
			break;
    case MS_ACTION_MOVE_ROW:
			ObjectRect(p,&rcP);
      rcP2.left =   rcP.left;
      rcP2.right =  rcP.left + dmp->GrData.udv_panel.cxName - 10;
      rcP2.top =    rcP.top;
      rcP2.bottom = rcP.bottom;
      Line_num=DDV_GetRowGivenMousePos(dmp, &rcP, &pt,&ParaGLine_num);

      DDV_GetCurrentDispRange(dmp->hWndDDV,&(dmp->GrData),
        dmp->MSA_d.pgp_l.LengthAli,&from_col,&to_col,&from_row,&to_row);
      vsb = GetSlateVScrollBar ((SlatE)dmp->hWndDDV);
      old_VPos = GetBarValue(vsb);

      if ((Line_num != -1) && (Line_num-1 >= from_row) && (Line_num-1 <= to_row)) {
        /* scroll vertically */
        Jump = ABS((Line_num-1) - dmp->dsp->SaveRow);
        if (Line_num >= to_row) {
          SetValue(vsb, old_VPos + Jump);
        }
        if (Line_num <= from_row+1) {
          SetValue(vsb, old_VPos - Jump);
        }
        /* get new line number, erase last underline, draw new one */
        SavedVPos = DDV_GetVPixelPosGivenRowNumber(dmp, rcP2, dmp->dsp->SaveRow);
        if ((Line_num-1) <= dmp->dsp->FromRow) {
          /* move before new row */
          VPos = DDV_GetVPixelPosGivenRowNumber(dmp, rcP2, Line_num-2);
          dmp->dsp->SaveRow = Line_num-2;
        }
        else {
          /* move after new row */
          VPos = DDV_GetVPixelPosGivenRowNumber(dmp, rcP2, Line_num-1);
          dmp->dsp->SaveRow = Line_num-1;
        }
        if (LineToErase) {
          InvertMode();
          UDV_draw_horizontal_line(rcP2, SavedVPos);
        }
        LineToErase = TRUE;
        UDV_draw_horizontal_line(rcP2, VPos);
      }
      else {
        /* just one time, xor the horizontal line to turn it off */
        if (LineToErase) {
          InvertMode();
          SavedVPos = DDV_GetVPixelPosGivenRowNumber(dmp, rcP2, dmp->dsp->SaveRow);
          UDV_draw_horizontal_line(rcP2, SavedVPos);
          LineToErase = FALSE;
        }
      }
      break;
    case MS_ACTION_SHIFT_ROW:
			ObjectRect(p,&rcP);
      Col = DDV_GetColNumberGivenMousePos(dmp, rcP, pt);
      DDV_GetCurrentDispRange(dmp->hWndDDV,&(dmp->GrData),
        dmp->MSA_d.pgp_l.LengthAli,&from_col,&to_col,&from_row,&to_row);
      /* if we're still in a legal area */
      if ((Col>=from_col) && (Col<=to_col) && PtInRect(pt, &rcP)) {
        /* if a new box needs to be drawn */
        if (dmp->dsp->SaveCol != Col) {
          ObjectRect(p,&rcP);
          SavedVPos = DDV_GetVPixelPosGivenRowNumber(dmp, rcP, dmp->dsp->SaveRow);
          SavedHPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, dmp->dsp->SaveCol);
          /* erase old box */
          rcP2.left =    SavedHPos - dmp->GrData.udv_font.ColWidth;
          rcP2.right =   SavedHPos;
          rcP2.top =     SavedVPos - dmp->GrData.udv_font.LineHeight;
          rcP2.bottom =  SavedVPos;
          InvertMode();
          UDV_draw_rectangle(rcP2, FALSE);
          /* draw new box */
          dmp->dsp->SaveCol = Col;
          HPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, Col);
          rcP2.left =    HPos - dmp->GrData.udv_font.ColWidth;
          rcP2.right =   HPos;
          rcP2.top =     SavedVPos - dmp->GrData.udv_font.LineHeight;
          rcP2.bottom =  SavedVPos;
          UDV_draw_rectangle(rcP2, FALSE);
        }
      }
      break;
    case MS_ACTION_SHIFT_BOUNDARY:
      /* if we're still in a legit area */
      ObjectRect(p, &rcP);
      InsetRect(&rcP,4,4);
      DDV_AdjustDrawingRect(&rcP,&(dmp->GrData.udv_font),dmp);
      rcP.left += dmp->GrData.udv_panel.cxName + dmp->GrData.udv_scale.cxLeftScale;
      if (PtInRect(pt, &rcP)) {
        ObjectRect(p, &rcP);
        /* get column of new vertical bar */
        Col = DDV_GetColNumberGivenMousePos(dmp, rcP, pt);
        /* if column has changed since vertical bar was last drawn */
        if (Col != dmp->dsp->SaveCol) {
          /* and the column's legal */
          if ((Col >= 0) && (Col < dmp->MSA_d.pgp_l.LengthAli)) {
            /* get horizontal pixel position of old column */
            HPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, dmp->dsp->SaveCol);
            /* adjust for drawing bar preceeding left boundary */
            if (dmp->dsp->LeftBoundary) HPos -= dmp->GrData.udv_font.ColWidth;
            /* erase old vertical bar */
            InvertMode();
            UDV_draw_vertical_bar(rcP, HPos, TRUE);
            /* get horizontal pixel position of new column */
            HPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, Col);
            /* adjust for drawing bar preceeding left boundary */
            if (dmp->dsp->LeftBoundary) HPos -= dmp->GrData.udv_font.ColWidth;
            /* draw the new vertical bar */
            UDV_draw_vertical_bar(rcP, HPos, TRUE);
            /* save the column */
            dmp->dsp->SaveCol = Col;
          }
        }
      }
      break;
    case MS_ACTION_CREATE_BLOCK:
      ObjectRect(p, &rcP);
      InsetRect(&rcP,4,4);
      DDV_AdjustDrawingRect(&rcP,&(dmp->GrData.udv_font),dmp);
      rcP.left += dmp->GrData.udv_panel.cxName + dmp->GrData.udv_scale.cxLeftScale;
      /* if we're still in a legit area */
      if (PtInRect(pt, &rcP)) {
        ObjectRect(p, &rcP);
        Col = DDV_GetColNumberGivenMousePos(dmp, rcP, pt);
        /* and the column has changed since vertical bar was last drawn */
        if (Col != dmp->dsp->SaveCol) {
          /* and the column's legal */
          if ((Col >= 0) && (Col < dmp->MSA_d.pgp_l.LengthAli)) {
            if (Col >= dmp->dsp->FromCol) {
              /* get horizontal pixel position of old column */
              HPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, dmp->dsp->SaveCol);
              /* erase old vertical bar */
              InvertMode();
              UDV_draw_vertical_bar(rcP, HPos, TRUE);
              /* get horizontal pixel position of new column */
              HPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, Col);
              /* draw the new vertical bar */
              UDV_draw_vertical_bar(rcP, HPos, TRUE);
              /* save the column */
              dmp->dsp->SaveCol = Col;
            }
          }
        }
      }
      break;
    case MS_ACTION_SPLIT_BLOCK:
      /* adjust x position so vertical bar jumps when mouse is on center of char */
      pt.x -= dmp->GrData.udv_font.ColWidth/2;
      /* get block, col, etc for new mouse position */
      if (GetBlockAndAlignment(pt, p, dmp, &BlockIndex, &IsUnAligned, &Col, &Offset, &Width, &rcP)) {
        /* if mouse col changed */
        if (Col != dmp->SaveCol) {
          /* get horizontal pixel position of old column */
          HPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, dmp->SaveCol);
          /* erase old vertical bar */
          InvertMode();
          UDV_draw_vertical_bar(rcP, HPos, FALSE);
          /* get horizontal pixel position of new column */
          HPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, Col);
          /* draw the new vertical bar */
          UDV_draw_vertical_bar(rcP, HPos, FALSE);
          /* save the column */
          dmp->SaveCol = Col;
        }
      }
	}
}

/*****************************************************************************

Function: DDV_ReleaseProc()

Purpose: manage Mouse-Release

Parameters:	p; panel handle (currently unused)
			pt; new position

Return value: none

*****************************************************************************/
NLM_EXTERN void DDV_ReleaseProc(PaneL p, PoinT pt)
{
WindoW 	       temport;
DdvMainPtr 	   dmp;
DdvMainWinPtr  dmwp;
DDVUpdateMSGPtr dump;
RecT         rcP, rcP2;
ParaGPtr     cur_pgp=NULL;
Int4         bsp_coord, SeqAlign_coord;
Int4         Disp_coord, Line_num,ParaGLine_Num;/*to get the coordinates*/
Uint1        uWhere; /*where the use clicked (seq, feat, name,...)*/
Uint2        bsp_eID, bsp_iID;
Int4         VPos, Shift, SavedVPos, SavedHPos, BlockIndex, NumBlocks;
Boolean      IsUnAligned;
MsaParaGPopListPtr  mpplp;
SeqAlignPtr   sap;
DDE_StackPtr  dsp;
Int4          Offset, Width;
BaR           hsb;
RecT          rc;


	/*get the panel data*/
	dmp = (DdvMainPtr) GetObjectExtra(p);
	if (dmp==NULL) return;

    switch(dmp->ms.Action_type){
		case MS_ACTION_SELECT_FULL_SEQ:
            ObjectRect(p,&rcP);
			Line_num=DDV_GetRowGivenMousePos(dmp,&rcP,&pt,&ParaGLine_Num);
			if (Line_num!=(Int4)-1){
                UDV_DecodeIdxFeat (dmp->MSA_d.pgp_l.entitiesTbl[ParaGLine_Num-1], 
                    &bsp_eID,&bsp_iID);                    
                ObjMgrSendMsg(OM_MSG_MOUSEUP, bsp_eID, bsp_iID, OBJ_BIOSEQ);
			}
			break;
        case MS_ACTION_SELECT_SEQ:
            /* erase outline */
            InvertMode();
            rc = DrawOutline(p, dmp->ms.first_row, dmp->ms.first_col,
                                dmp->ms.last_row,   dmp->ms.last_col, TRUE);
            CopyMode();
            DDV_SelectRegion(dmp, pt, rc, dmp->ms.first_row, dmp->ms.last_row,
                                          dmp->ms.first_col, dmp->ms.last_col);

            ObjectRect(p,&rcP);
            pt.y=dmp->ms.oldPos.y;
            if (dmp->MSA_d.pgp_l.DisplayType==DDV_DISP_HORZ){            
                if (DDV_GetCoordsGivenAClick(dmp,&rcP,&pt,
                        &bsp_coord,&SeqAlign_coord,&Disp_coord,&Line_num,

						&ParaGLine_Num,&uWhere,&cur_pgp)){
					if (uWhere==PGP_REGION_SEQ){
                        UDV_DecodeIdxFeat (dmp->MSA_d.pgp_l.entitiesTbl[ParaGLine_Num-1], 
                            &bsp_eID,&bsp_iID);                    
                        ObjMgrSendMsg(OM_MSG_MOUSEUP, bsp_eID, bsp_iID, OBJ_BIOSEQ);
						/*send a OM_MSG_UPDATE message to move the caret (if needed)*/
						if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
							dump=(DDVUpdateMSGPtr)MemNew(sizeof(DDVUpdateMSG));
							if (dump){
								dmp->dci.new_row=Line_num;
								dmp->dci.new_col=Disp_coord;
								dump->type=UPDATE_TYPE_CARETPOS;
								dump->data=NULL;
								ObjMgrSendProcMsg(OM_MSG_UPDATE, bsp_eID, bsp_iID, OBJ_BIOSEQ,
										0,0,(Pointer)dump);
								MemFree(dump);
							}
						}
                    }
                } else {  /* send mouse up anyway */
                    if(ParaGLine_Num > 0) {
                        UDV_DecodeIdxFeat (dmp->MSA_d.pgp_l.entitiesTbl[ParaGLine_Num-1], 
                            &bsp_eID,&bsp_iID);                    
                        ObjMgrSendMsg(OM_MSG_MOUSEUP, bsp_eID, bsp_iID, OBJ_BIOSEQ);
                    }
                }
            }
			break;
		case MS_ACTION_RESIZE_WIN:
			temport=SavePort((WindoW)p);
			Select(p);
			InvertMode();	
			UDV_draw_double_cursor(dmp->ms.rcClip,
				dmp->ms.oldPos);
			/*redraw panel with new 'cxName' value*/
			if (PtInRect(dmp->ms.newPos,&dmp->ms.rcClip)){
				RecT rc;

				ObjectRect(p,&rc);
				rc.left=0;/*for obscure reasons, not == 0*/
				rc.top=0;
				dmp->GrData.udv_panel.cxName=dmp->ms.newPos.x;
				DDV_Resize_DDV(p,FALSE); /*DDV_SetupWin (p,FALSE,NULL); */
				InvalRect(&rc);
			}
			RestorePort(temport);
			break;
    case MS_ACTION_MOVE_ROW:
      ObjectRect(p, &rcP);
      /* if the mouse is released in sequence name section */
      if (pt.x<dmp->GrData.udv_panel.cxName &&
          pt.y>(rcP.top+dmp->GrData.udv_panel.cyScale)){
        /* get the line number where the mouse is released */
        Line_num=DDV_GetRowGivenMousePos(dmp, &rcP, &pt,&ParaGLine_Num);
        if (Line_num != -1) {
          /* if the line number is valid */
          if (dmp->dsp->FromRow != Line_num-1) {
            /* move the saved row to this line number */
            DDE_MoveRow(dmp->dsp, dmp->dsp->FromRow, Line_num-1, TRUE);
            dmp->deri.curEditRow = Line_num-1;
            DDV_ReDraw(dmp);
          }
          /* otherwise */
          else {
            /* since screen's not redrawn, turn off the last drawn horizontal line */
            ObjectRect(p, &rcP);
            rcP2.left =   rcP.left;
            rcP2.right =  rcP.left + dmp->GrData.udv_panel.cxName - 10;
            rcP2.top =    rcP.top;
            rcP2.bottom = rcP.bottom;
            VPos = DDV_GetVPixelPosGivenRowNumber(dmp, rcP2, dmp->dsp->SaveRow);
            InvertMode();
            UDV_draw_horizontal_line(rcP2, VPos);
          }
        }
      }
      ArrowCursor();
      break;
    case MS_ACTION_SHIFT_ROW:
      /* shift row by diff between original col position and last position */
      Shift = dmp->dsp->SaveCol - dmp->dsp->FromCol;
      if (DDE_ShiftRow(dmp->dsp, dmp->dsp->SaveRow, Shift, TRUE, TRUE)) {
        DDV_ReDraw(dmp);
      }
      else {
        /* if no shift was done, erase the boxes */
        ObjectRect(p,&rcP);
        SavedVPos = DDV_GetVPixelPosGivenRowNumber(dmp, rcP, dmp->dsp->SaveRow);
        SavedHPos = DDV_GetHPixelPosGivenColNumber(dmp, rcP, dmp->dsp->SaveCol);
        rcP2.left =    SavedHPos - dmp->GrData.udv_font.ColWidth;
        rcP2.right =   SavedHPos;
        rcP2.top =     SavedVPos - dmp->GrData.udv_font.LineHeight;
        rcP2.bottom =  SavedVPos;
        InsetRect(&rcP2, -2, -2);
        InvalRect(&rcP2);
      }
      ArrowCursor();
      break;
    case MS_ACTION_SHIFT_BOUNDARY:
      /* shift boundary by diff between original col position and last position */
      Shift = dmp->dsp->SaveCol - dmp->dsp->FromCol;
      if (dmp->dsp->LeftBoundary) {
        DDE_ShiftLeftBoundary(dmp->dsp, dmp->dsp->BlockIndex, Shift, TRUE);
        DDV_ReDraw(dmp);
      }
      else {
        DDE_ShiftRightBoundary(dmp->dsp, dmp->dsp->BlockIndex, Shift, TRUE);
        DDV_ReDraw(dmp);
      }
      ArrowCursor();
      break;
    case MS_ACTION_CREATE_BLOCK:
      DDE_CreateBlock(dmp->dsp, dmp->dsp->FromCol, dmp->dsp->SaveCol, TRUE);
      dmp->MouseMode = dmp->SavedMouseMode;
      DDV_ReDraw(dmp);
      ArrowCursor();
      break;
    case MS_ACTION_SPLIT_BLOCK:
      /* get block and alignment for last saved col */
      /* also get offset into block and width of block */
      GetBlockAndAlignmentForCol(dmp, dmp->SaveCol, &BlockIndex, &IsUnAligned, &Offset, &Width);
      /* if col is in aligned region */
      if (!IsUnAligned && (Offset < (Width-1))) {
        /* create a stack for the block */
        sap = ViewMgr_GetBeginIndexed(dmp->MSA_d.pgp_l.sap);
        dmp->MSA_d.pgp_l.viewed_sap = dmp->MSA_d.pgp_l.sap;
        mpplp = DDE_CreateDisplay(sap, BlockIndex, FALSE, &NumBlocks);
        dsp = DDE_NewStack(mpplp);
        DDE_MergeNodesLists(dsp->pEdit);
        /* record which block is being edited, etc */
        dsp->LaunchBlock = BlockIndex;
        dsp->NumBlocks =   NumBlocks;
        dsp->IsUnAligned = IsUnAligned;
        /* shift right boundary of block left, to the split line */
        DDE_ShiftRightBoundary(dsp, 0, -(Width-(Offset+1)), FALSE);
        /* tell AlnMgr to save edits, don't redraw yet */
        dmp->dsp = dsp;
        DDV_SaveEdits(dmp, FALSE);
        /* free stack for block.  create stack for unaligned region to right of block */
        DDE_FreeStack(dsp);
        sap = ViewMgr_GetBeginIndexed(dmp->MSA_d.pgp_l.sap);
        dmp->MSA_d.pgp_l.viewed_sap = dmp->MSA_d.pgp_l.sap;
        mpplp = DDE_CreateDisplay(sap, BlockIndex+1, TRUE, &NumBlocks);
        dsp = DDE_NewStack(mpplp);
        DDE_MergeNodesLists(dsp->pEdit);
        /* create new block of part that was just unaligned */
        DDE_CreateBlock(dsp, 0, (Width-(Offset+1)-1), FALSE);
        /* tell AlnMgr to save edits, now redraw */
        dmp->dsp = dsp;
        DDV_SaveEdits(dmp, TRUE);
        /* free remaining stack */
        dsp = DDE_FreeStack(dsp);
        dmp->dsp = dsp;
        dmp->MouseMode = dmp->SavedMouseMode;
      }
      /* get rid of vertical bar */
      else {
        hsb = GetSlateHScrollBar((SlatE) dmp->hWndDDV);
        DDV_ReDrawAtCol(dmp, GetBarValue(hsb), FALSE);
      }
	}	

  ClearUDV_mouse_select(&(dmp->ms));
	dmp->ms.Action_type=MS_ACTION_FEAT_NOTHING;
	/*update InfoPanel*/
	dmwp=(DdvMainWinPtr)GetObjectExtra(dmp->hParent);
  if (dmp->bEditor) {
    DDV_GreyOut(dmwp, DDE_AtStartOfStack(dmp->dsp), DDE_AtEndOfStack(dmp->dsp));
  }
	if (dmwp && dmp->MouseMode!=DDV_MOUSEMODE_EDIT)
		SetTitle(dmwp->InfoPanel,"Ready !");

  if (dmp->MouseMode != DDV_MOUSEMODE_MERGEBLOCKS2)
    ArrowCursor();
}

/*****************************************************************************

Function: DDV_HoldProc()

Purpose: manage Mouse-Hold

Parameters:	p; panel handle (currently unused)
			pt; new position

Return value: none

*****************************************************************************/
NLM_EXTERN void DDV_HoldProc(PaneL p, PoinT pt)
{
DdvMainPtr 	dmp;
RecT        rcP;
ParaGPtr    cur_pgp=NULL;
Int4        Disp_coord_stop;
Int4        Line_num_stop;
Int4        from_col, to_col, from_row, to_row;
Boolean     Scroll=FALSE;

	/*get the panel data*/
	dmp = (DdvMainPtr) GetObjectExtra(p);
	if (dmp==NULL) return;

	/*what is happening ?*/
	switch(dmp->ms.Action_type){
		case MS_ACTION_SELECT_SEQ:
			if (shftKey) break;
			ObjectRect(p,&rcP);
			/*get the ParaG list and look for the ParaG where the mouse is located*/
			/*note : only implemented for DDV_DISP_HORZ type of display...*/
			if (dmp->MSA_d.pgp_l.DisplayType==DDV_DISP_HORZ) {
        /* get line num and col for current mouse position */
        DDV_GetRowAndColGivenPt(dmp, rcP, pt, &Disp_coord_stop, &Line_num_stop);
        /* force single row selection when needed */
        if (dmp->MouseMode == DDV_MOUSEMODE_SELECT_ONE_ROW) Line_num_stop = dmp->ms.first_row;
        /* erase the old rectangle */
        InvertMode();
        DrawOutline(p, dmp->ms.first_row, dmp->ms.first_col,
                       dmp->ms.last_row,  dmp->ms.last_col, TRUE);
        /* get display info */
        DDV_GetCurrentDispRange(dmp->hWndDDV,&(dmp->GrData),
          dmp->MSA_d.pgp_l.LengthAli,&from_col,&to_col,&from_row,&to_row);
        /* draw the new rectangle after scrolling */
        DDV_AutoHScroll(dmp, Disp_coord_stop);
        DDV_AutoVScroll(dmp, Line_num_stop);
        CopyMode();
        DrawOutline(p, dmp->ms.first_row, dmp->ms.first_col, 
                       Line_num_stop,     Disp_coord_stop, TRUE);
        /* display info about residues that mouse is over */
        DDV_DispStatus(dmp, Line_num_stop, dmp->ms.first_col, Disp_coord_stop);
      }
      break;
		case MS_ACTION_RESIZE_WIN:
		case MS_ACTION_SELECT_FULL_SEQ:
			break;
	}/*end switch()*/
}

/*****************************************************************************

Function: DDV_MoveCaretLR()

Purpose: move the caret to the right or to the left

Parameters:	

Return value: none

*****************************************************************************/
static void DDV_MoveCaretLR(DdvMainPtr dmp,Int4 decal_Hscroll,
	Boolean bMoveRight,BaR hsb,Int4 old_Hpos,Int4 from_col,Int4 to_col)
{
DDVUpdateMSGPtr dump;
Uint2           bsp_eID, bsp_iID;
Int4            shift, new_from_col, new_to_col;

	dump=(DDVUpdateMSGPtr)MemNew(sizeof(DDVUpdateMSG));
	if (dump){
		/*get bsp IDs*/
		UDV_DecodeIdxFeat(dmp->MSA_d.pgp_l.entitiesTbl[dmp->dci.new_row-1], 
            	&bsp_eID,&bsp_iID);
		/*save old position*/
		dmp->dci.old_col=dmp->dci.new_col;
		dmp->dci.old_row=dmp->dci.new_row;
		/*update the position*/
		if (bMoveRight)
			dmp->dci.new_col+=decal_Hscroll;
		else
			dmp->dci.new_col-=decal_Hscroll;
		if (dmp->dci.new_col<0){
			dmp->dci.new_col=0;
			Beep();
			return;
		}
		if (dmp->dci.new_col>dmp->MSA_d.pgp_l.LengthAli-1){
			dmp->dci.new_col=dmp->MSA_d.pgp_l.LengthAli-1;
			Beep();
			return;
		}
		/*auto-scroll the panel if needed*/
    if (dmp->dci.new_col<=from_col){
      shift = (to_col-from_col)/2;
      new_from_col = from_col - shift;
      new_to_col = to_col - shift;
      if ((dmp->dci.new_col >= new_from_col) && (dmp->dci.new_col <= new_to_col)) {
        /* shift left by 1/2 the window width */
        SetValue(hsb,old_Hpos-shift);
      }
      else {
        /* shifting by more than 1/2 the window width */
        SetValue(hsb, dmp->dci.new_col);
      }
    }
    if (dmp->dci.new_col>=to_col){
      shift = (to_col-from_col)/2;
      new_from_col = from_col + shift;
      new_to_col = to_col + shift;
      if ((dmp->dci.new_col >= new_from_col) && (dmp->dci.new_col <= new_to_col)) {
        /* shift right by 1/2 the window width */
        SetValue(hsb,old_Hpos+shift);
      }
      else {
        /* shifting by more than 1/2 the window width */
        SetValue(hsb, dmp->dci.new_col);
      }
    }
		/*if ok, move the caret*/
		dump->type=UPDATE_TYPE_CARETPOS;
		dump->data=NULL;
		ObjMgrSendProcMsg(OM_MSG_UPDATE, bsp_eID, bsp_iID, OBJ_BIOSEQ,
				0,0,(Pointer)dump);
		MemFree(dump);
	}
}

/*****************************************************************************

Function: DDV_MoveCaretUD()

Purpose: move the caret up or down

Parameters:	

Return value: none

*****************************************************************************/
static void DDV_MoveCaretUD(DdvMainPtr dmp,Int4 decal_Vscroll,
	Boolean bMoveDown,BaR vsb,Int4 old_Vpos,Int4 from_row,Int4 to_row)
{
DDVUpdateMSGPtr dump;
Uint2           bsp_eID, bsp_iID;
Int4            shift;

	dump=(DDVUpdateMSGPtr)MemNew(sizeof(DDVUpdateMSG));
	if (dump){
		/*get bsp IDs*/
		UDV_DecodeIdxFeat(dmp->MSA_d.pgp_l.entitiesTbl[dmp->dci.new_row-1], 
            	&bsp_eID,&bsp_iID);
		/*save old position*/
		dmp->dci.old_row=dmp->dci.new_row;
		dmp->dci.old_col=dmp->dci.new_col;
		/*update the position*/
		if (bMoveDown)
			dmp->dci.new_row+=decal_Vscroll;
		else
			dmp->dci.new_row-=decal_Vscroll;

		if (dmp->dci.new_row<1){
			dmp->dci.new_row=1;
      SetValue(vsb, 0);
			Beep();
      goto send_msg;
		}
		if (dmp->dci.new_row>dmp->MSA_d.pgp_l.nBsp){
			dmp->dci.new_row=dmp->MSA_d.pgp_l.nBsp;
      SetValue(vsb, dmp->MSA_d.pgp_l.nBsp);
			Beep();
      goto send_msg;
		}

		/*auto-scroll the panel if needed*/
    if (ABS(decal_Vscroll) == 1) {
		  if (dmp->dci.new_row<=from_row){
        shift = (to_row-from_row)/2;
        SetValue(vsb, old_Vpos-shift);
		  }
		  if (dmp->dci.new_row>=to_row-1){
        shift = (to_row-from_row)/2;
        SetValue(vsb, old_Vpos+shift);
		  }
    }
    else {
      if (bMoveDown) {
        SetValue(vsb, old_Vpos+decal_Vscroll);
      }
      else {
        SetValue(vsb, old_Vpos-decal_Vscroll);
      }
    }

		/*if ok, move the caret*/
send_msg:
		dump->type=UPDATE_TYPE_CARETPOS;
		dump->data=NULL;
		ObjMgrSendProcMsg(OM_MSG_UPDATE, bsp_eID, bsp_iID, OBJ_BIOSEQ,
				0,0,(Pointer)dump);
		MemFree(dump);
	}
}

/*****************************************************************************

Function: DDV_KeyboardProc()

Purpose: manage keyboard actions

Parameters:	s; panel handle (DDV panel)
			ch; key

Return value: none

*****************************************************************************/
NLM_EXTERN void DDV_KeyboardProc (SlatE s, Char ch)
{
DdvMainWinPtr dmwp; /*main window*/
DdvMainPtr dmp;     /*DDV panel associated data*/
BaR        hsb,vsb; /*DDV' scroll bars*/
Int4       old_Hpos,old_Vpos,/*scrolls positions*/
           decal_Hscroll,decal_Vscroll,/*used to scroll DDV's panel*/
           from_col,to_col,from_row,to_row,/*display size*/
           bsp_pos, height;
		   
	if ( (int) ch == 0 ) return;
	
	/*get the panel data*/
	dmp = (DdvMainPtr) GetObjectExtra(s);
	if (dmp==NULL) return;
	dmwp = (DdvMainWinPtr) GetObjectExtra(dmp->hParent);
  if (dmwp == NULL) return;
	
	/*get scroll handles and data*/
	hsb = GetSlateHScrollBar ((SlatE) dmp->hWndDDV);
	old_Hpos=GetBarValue(hsb);
	vsb = GetSlateVScrollBar ((SlatE) dmp->hWndDDV);
	old_Vpos=GetBarValue(vsb);

	/*get the display size*/
	DDV_GetCurrentDispRange(dmp->hWndDDV,&(dmp->GrData),
		dmp->MSA_d.pgp_l.LengthAli,&from_col,&to_col,&from_row,&to_row);
	
	/*if pressed, scroll panel by one 'panel' size*/
	if (ctrlKey) {
		decal_Hscroll=to_col-from_col;
		decal_Vscroll=to_row-from_row;
	}
	else{
		decal_Hscroll=1;
		decal_Vscroll=1;
	}

	/*switch to one-bsed value because dmp->dci values 
	  are one-based values*/
	from_row++;to_row++;

	switch ((int) TO_UPPER(ch)){
    case NLM_PREV:
      /* page-up -- scoll page up one page height */
      height = (to_row-from_row) - 1;
			if (dmp->MouseMode==DDV_MOUSEMODE_EDIT) {
        DDV_MoveCaretUD(dmp, height, FALSE, vsb, old_Vpos, from_row, to_row);
      }
      else {
        SetValue(vsb, old_Vpos - height);
      }
      break;
    case NLM_NEXT:
      /* page-down -- scoll page down one page height */
      height = (to_row-from_row) - 1;
			if (dmp->MouseMode==DDV_MOUSEMODE_EDIT) {
				DDV_MoveCaretUD(dmp, height, TRUE, vsb, old_Vpos, from_row, to_row);
      }
      else {
        SetValue(vsb, old_Vpos + height);
      }
      break;
    case NLM_HOME:
      /* for home key, move cursor to start of row */
			if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
        /* move caret left by current col pos */
				DDV_MoveCaretLR(dmp, dmp->dci.new_col, FALSE, hsb, old_Hpos, from_col, to_col);
      }
      else {
        SetValue(hsb, 0);
      }
      break;
    case NLM_END:
      /* for end key, move cursor to end of row */
			if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
        /* move caret right by diff between last col and current col */
        DDV_MoveCaretLR(dmp, ((dmp->MSA_d.pgp_l.LengthAli-1) - dmp->dci.new_col), TRUE,
                        hsb, old_Hpos, from_col, to_col);
      }
      else {
        SetValue(hsb, dmp->MSA_d.pgp_l.LengthAli-1);
      }
      break;
    case 0x1a:
      /* for ctrl-z, undo the last edit */
      if (DDE_Prev(dmp->dsp)) {
        DDV_ReDraw(dmp);
      }
      break;
    case 0x19:
      /* for ctrl-y, redo the last undone edit */
      if (DDE_Next(dmp->dsp)) {
        DDV_ReDraw(dmp);
      }
      break;
    case NLM_DEL:
      /* delete key pressed -- remove a gap and leave carat in place */
			if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
        if (DDE_RemoveGap(dmp->dsp, dmp->dci.new_row-1, dmp->dci.new_col+1, TRUE)) {
          DDV_ReDraw(dmp);
        }
      }
      break;
    case 0x08:
      /* backspace key pressed -- remove gap to the left and move carat left */
			if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
        if (DDE_RemoveGap(dmp->dsp, dmp->dci.new_row-1, dmp->dci.new_col, TRUE)) {
          DDV_ReDraw(dmp);
        }
      }
		case NLM_LEFT:
			/*move the caret ?*/
			if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
				decal_Hscroll=1;/*doesn't accept ctrlKey motions*/
				DDV_MoveCaretLR(dmp,decal_Hscroll,FALSE,hsb,old_Hpos,
					from_col,to_col);
			}
			/*scroll the panel*/
			else{
				SetValue(hsb,old_Hpos-decal_Hscroll);
			}
			break;
    case 0x20:
      /* space bar pressed -- insert a gap and move carat to the right */
			if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
        if (DDE_InsertGap(dmp->dsp, dmp->dci.new_row-1, dmp->dci.new_col+1, TRUE)) {
          DDV_ReDraw(dmp);
        }
      }
		case NLM_RIGHT:
			/*move the caret ?*/
			if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
				decal_Hscroll=1;/*doesn't accept ctrlKey motions*/
				DDV_MoveCaretLR(dmp,decal_Hscroll,TRUE,hsb,old_Hpos,
					from_col,to_col);
			}
			/*scroll the panel*/
			else {
				SetValue(hsb,old_Hpos+decal_Hscroll);
			}
			break;
		case NLM_UP:
			/*move the caret ?*/
			if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
				decal_Vscroll=1;/*doesn't accept ctrlKey motions*/
				DDV_MoveCaretUD(dmp,decal_Vscroll,FALSE,vsb,old_Vpos,
					from_row,to_row);
			}
			/*scroll the panel*/
			else{
				SetValue(vsb,old_Vpos-decal_Vscroll);
			}
			break;
		case NLM_DOWN:
			/*move the caret ?*/
			if (dmp->MouseMode==DDV_MOUSEMODE_EDIT){
				decal_Vscroll=1;/*doesn't accept ctrlKey motions*/
				DDV_MoveCaretUD(dmp,decal_Vscroll,TRUE,vsb,old_Vpos,
					from_row,to_row);
			}
			/*scroll the panel*/
			else{
				SetValue(vsb,old_Vpos+decal_Vscroll);
			}
			break;
		default:
			Beep ();
			break;
	}
  if (dmp->bEditor) {
    DDV_GreyOut(dmwp, DDE_AtStartOfStack(dmp->dsp), DDE_AtEndOfStack(dmp->dsp));
  }

	/*update InfoPanel with position*/
	if (dmwp && dmp->MouseMode==DDV_MOUSEMODE_EDIT){
		Char szAccess[21];
		DDV_GetSeqNameGivenRow(dmp->MSA_d.pgp_l.TableHead, dmp->dci.new_row,
			szAccess);
		bsp_pos=DDV_GetBspCoordGivenPgpList(
			dmp->MSA_d.pgp_l.TableHead[dmp->dci.new_row-1],
			dmp->dci.new_col);
		if (bsp_pos!=(Int4)-1){
			DDV_DispPositionInStatus(dmwp->InfoPanel, 
				bsp_pos+1,dmp->dci.new_row,szAccess);
					/*+1 : switch to one-based value*/
		}
	}
}


NLM_EXTERN void DDV_ReDrawAtCol(DdvMainPtr dmp, Int4 Col, Boolean DDE) {
/*------------------------------------------
*  resize, redraw.
*------------------------------------------*/
  DdvMainWinPtr  dmwp;
  WindoW         temport;
  RecT 	         rcP;
  BaR            hsb;
  Int4           NumBlocks;

  if (DDE) {
    MemCopy(&(dmp->MSA_d.pgp_l), dmp->dsp->pEdit->pPopList, sizeof(MsaParaGPopList));
  }

  DDV_InitColour_When_Start(dmp->MSA_d.pgp_l.sap,
    &(dmp->MSA_d.pgp_l),&(dmp->Globals.colorp), FALSE);
  /* recalculate window size */
  DDV_WhatSize(dmp);
  /* adjust horizontal scroll bar */
  hsb = GetSlateHScrollBar((SlatE) dmp->hWndDDV);
  DDV_UpdateHScrollVal(dmp->hWndDDV, FALSE, GetBarValue(hsb));

  dmwp=(DdvMainWinPtr)GetObjectExtra(dmp->hParent);
  temport=SavePort(dmwp->hWndDDV);
  Select(dmwp->hWndDDV);
  ObjectRect(dmwp->hWndDDV, &rcP);
  InvalRect(&rcP);
  if (Col >= 0) {
    SetValue(hsb, Col);
  }
  Update();
  RestorePort(temport);

  if (DDE) {
    /* decide about greying out undo and redo functions */
    DDV_GreyOut(dmwp, DDE_AtStartOfStack(dmp->dsp), DDE_AtEndOfStack(dmp->dsp));

    /* decide about greying out left/right/center functions */
    NumBlocks = DDE_GetNumBlocks(dmp->dsp->pEdit);
    if (NumBlocks == 0) {
      Enable(dmwp->MainMenu.Justify);
    }
    else {
      Disable(dmwp->MainMenu.Justify);
    }
  }

 	return;
}


NLM_EXTERN void DDV_ReDraw(DdvMainPtr dmp) {
/*------------------------------------------
*  resize, redraw.
*------------------------------------------*/
  /* give an illegal column so the column's not set */
  /* specify that this redraw is for DDE */
  DDV_ReDrawAtCol(dmp, -1, TRUE);
}


static void DDV_SelectRegion(DdvMainPtr dmp, PoinT pt, RecT rc,
                             Int4 row_start, Int4 row_stop,
                             Int4 col_start, Int4 col_stop) {
/*----------------------------------------------------------------------------
*  select from row_start to row_stop between columns col_start and col_stop.
*  rows are 1-based, cols are 0-based.  why?  go figure.
*  
*----------------------------------------------------------------------------*/
  Int4           row;
  Uint1          uWhere;
  ParaGPtr       pgp;

  if (row_start > row_stop) swap(&row_start, &row_stop);
  if (row_start < 1) row_start=1;
  if (row_stop > dmp->MSA_d.pgp_l.nBsp) row_stop=dmp->MSA_d.pgp_l.nBsp;

  for (row=row_start; row<=row_stop; row++) {
    /* send message to select all intervening columns on this row */
    pgp = DDV_GetPGPGivenRowAndCol(dmp, row, col_stop);
    uWhere = DDV_GetRegionInParaG(pgp, row);
    if (uWhere == PGP_REGION_SEQ) {
      DDV_SendBSPSelectMsg(dmp, col_start, col_stop, row, TRUE);
    }
  }

  /* redraw section that's been highlighted */
/*
  InsetRect(&rc, -4, -4);
  InvalRect(&rc);
  Update();
*/
}
